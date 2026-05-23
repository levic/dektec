//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcFMBC.c *#*#*#*#*#*#*#*#*#*#*# (C) 2024 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2024 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL DEKTEC DIGITAL VIDEO BV, ITS AGENTS OR ITS EMPLOYEES BE LIABLE FOR
// ANY DIRECT, INDIRECT, CONSEQUENTIAL, INCIDENTAL, OR OTHER DAMAGES (INCLUDING DAMAGES
// FOR THE LOSS OF USE, INFORMATION, GOODWILL, PROFIT, WORK STOPPAGE, DATA, BUSINESS OR
// REVENUE) UNDER ANY CIRCUMSTANCES, OR UNDER ANY LEGAL THEORY, WHETHER IN CONTRACT, IN
// TORT, IN NEGLIGENCE, OR OTHERWISE, ARISING FROM THE USE OF, OR INABILITY TO USE THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"
#include "DtBcFMBC.h"
#include "DtBcFMBC_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcFMBC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_FMBC_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcFMBC))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_FMBC_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(FMBC, pBc)

// Helper macro to cast a DtBc* to a DtBcFMBC*
#define BC_FMBC         ((DtBcFMBC*)pBc)


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcFMBC_EmtptyResponseFifo(DtBcFMBC* pBc);
static DtStatus  DtBcFMBC_Init(DtBc*);
static DtStatus  DtBcFMBC_OnEnable(DtBc*, Bool  Enable);
static void  DtBcFMBC_InterruptDpc(DtDpcArgs* pArgs);
static DtStatus  DtBcFMBC_InterruptHandler(DtBc*, Int, Int, void*);
static DtStatus  DtBcFMBC_RegisterIntHandlers(DtBcFMBC*);
static DtStatus  DtBcFMBC_Reset(DtBcFMBC* pBc);
static DtStatus  DtBcFMBC_WriteReadTransfer(DtBcFMBC* pBc, DtBcFMBC_Command,
                                        Int WriteLength, const UInt32* pWriteBuffer,
                                        Int ReadLength, UInt32* pReadBuffer, Int Timeout);
static DtStatus  DtBcFMBC_ReadCommandResponse(DtBcFMBC* pBc, Int MsgId, Int ReadLength,
                                                        UInt32* pReadBuffer, Int Timeout); 
static DtStatus  DtBcFMBC_WriteCommandPacket(DtBcFMBC* pBc, Int MsgId, Int Command,
                               Int WriteLength,  const UInt32* pWriteBuffer, Int Timeout);
static DtStatus DtBcFMBC_WaitForEmptyCmdSpace(DtBcFMBC* pBc, Int* pNumFree, Int Timeout);
static DtStatus DtBcFMBC_WaitForResponseFifoload(DtBcFMBC* pBc, Int* pLoad, Int Timeout);
static Int DtBcFMBC_ToFMBCCommand(DtBcFMBC_Command Command);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcFMBC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcFMBC_Close(DtBc*  pBc)
{
    BC_FMBC_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcFMBC*  DtBcFMBC_Open(Int  Address, DtCore*  pCore, DtPt*  pPt,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_FMBC_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcFMBC, Id, DT_BLOCK_TYPE_FMBC, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcFMBC_Close;
    OpenParams.m_InitFunc = DtBcFMBC_Init;
    OpenParams.m_OnEnableFunc = DtBcFMBC_OnEnable;
    
    // Use base function to allocate and perform standard initialization of block data
    return (DtBcFMBC*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_GetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcFMBC_GetProperties(DtBcFMBC* pBc, Int* pCmdFifoSize, Int* pRespFifoSize,
                                                                        Int* pClockFreqHz)
{
    // Sanity check    
    BC_FMBC_DEFAULT_PRECONDITIONS(pBc);

    if (pCmdFifoSize==NULL || pRespFifoSize==NULL || pClockFreqHz==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    *pCmdFifoSize = pBc->m_CmdFifoSize;
    *pRespFifoSize = pBc->m_RespFifoSize;
    *pClockFreqHz = pBc->m_ClockFreqHz;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_Write -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcFMBC_Write(DtBcFMBC* pBc, DtBcFMBC_Command Command, Int Length,
                                                       const UInt32* pBuffer, Int Timeout)
{
    DtStatus  Status = DT_STATUS_OK;
 
    // Sanity check    
    BC_FMBC_DEFAULT_PRECONDITIONS(pBc);


    DtDbgOutBc(MAX, FMBC, pBc, "Write Command: 0x%02X, RespLength: %d", Command, Length);
    
    // Protect Mailbox access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Transfer data
    Status = DtBcFMBC_WriteReadTransfer(pBc, Command, Length, pBuffer,  0, NULL, Timeout);
    
    if (!DT_SUCCESS(Status))
        // Transfer failed, reset
        DtBcFMBC_Reset(pBc);

    // Release Mailbox mutex
    DtFastMutexRelease(&pBc->m_AccessMutex);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_WriteRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcFMBC_WriteRead(DtBcFMBC* pBc, DtBcFMBC_Command Command,
                                         Int WriteLength, const UInt32* pWriteBuffer,
                                         Int ReadLength, UInt32* pReadBuffer, Int Timeout)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check    
    BC_FMBC_DEFAULT_PRECONDITIONS(pBc);

     
    // Protect Mailbox access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Transfer data
    Status = DtBcFMBC_WriteReadTransfer(pBc, Command, WriteLength, pWriteBuffer,
                                                        ReadLength, pReadBuffer, Timeout);
    
    if (!DT_SUCCESS(Status))
       // Transfer failed, reset
        DtBcFMBC_Reset(pBc);

    // Release Mailbox mutex
    DtFastMutexRelease(&pBc->m_AccessMutex);
    return Status;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcFMBC - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcFMBC_Init(DtBc* pBcBase)
{  
    DtBcFMBC* pBc = (DtBcFMBC*)pBcBase;
    DtStatus  Status=DT_STATUS_OK;
    UInt32 RegData=0;

    pBc->m_MessageId = 1;

    // Init interrupt DPC
    Status = DtDpcInit(&pBc->m_IntDpc, DtBcFMBC_InterruptDpc, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, FMBC, pBc, "ERROR: failed to init DPC (Status=0x%08X)", Status);
        return Status;
    }

    // Initialize Mailbox mutex against concurrent access
    DtFastMutexInit(&pBc->m_AccessMutex);

    // Init completion event
    Status = DtEventInit(&pBc->m_StatusEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, FMBC, pBc, 
                            "ERROR: failed to init status event (Status=0x%08X)", Status);
        return Status;
    }

    // Read properties
    RegData = FMBC_Config_READ(pBc);
    pBc->m_CmdFifoSize = FMBC_Config_GET_CmdFifoDepth(RegData);
    pBc->m_RespFifoSize = FMBC_Config_GET_RespFifoDepth(RegData);
    pBc->m_ClockFreqHz = FMBC_Config_GET_ClkFreqMHz(RegData)*1000000;

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    Status = DtBcFMBC_RegisterIntHandlers(pBc);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, FMBC, pBc, "ERROR: failed to register interrupt handlers");
        return Status;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcFMBC_InterruptHandler(DtBc*  pBc, Int  Id, Int  Index, void*  pContext)
{
    DtStatus  Status=DT_STATUS_OK;
    DtDpcArgs  DpcArgs;
    
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcFMBC));

    // Check RespId is one of ours
    if (Id != DT_INTERRUPT_MAILBOX_STATUS)
    {
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Disable all Mailbox interrupts because the interrupts are level interrupts
    // so they are not fired ones but continuous
    // NOTE: is re-enabled upon next transfer
    FMBC_MailboxClientS10_IntEnable_WRITE((DtBcFMBC*)pBc, 0);
    
    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    DpcArgs.m_Data1.m_UInt32_2 = 0;
    
    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&((DtBcFMBC*)pBc)->m_IntDpc, &DpcArgs);
    DT_ASSERT(DT_SUCCESS(Status));

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_InterruptDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcFMBC_InterruptDpc(DtDpcArgs* pArgs)
{
    DtBcFMBC* pBc = (DtBcFMBC*)pArgs->m_pContext;
    DtDbgOutBc(MAX, FMBC, pBc, "Mailbox status changed");
    DtEventSet(&pBc->m_StatusEvent);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcFMBC_OnEnable(DtBc* pBcBase, Bool  Enable)
{
    DtBcFMBC* pBc = (DtBcFMBC*)pBcBase;
    if (Enable)
    {
        DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_MAILBOX_STATUS);
    }
    return DtBcFMBC_Reset(pBc);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcFMBC_RegisterIntHandlers(DtBcFMBC*  pBc)
{
    Int  i=0;
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_FMBC_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known RespId's
        switch(Id)
        {
        case DT_INTERRUPT_MAILBOX_STATUS:
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id,
                                                         DtBcFMBC_InterruptHandler, NULL);
            if(!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, FMBC, pBc,
                           "ERROR: failed to register interrupt handler (RespId=%d)", Id);
            }
            break;

        default:
            DT_ASSERT(FALSE);      // Unknown RespId. New version of the block??
            break;
        }
    }
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_Reset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcFMBC_Reset(DtBcFMBC* pBc)
{
    // Sanity check
    BC_FMBC_DEFAULT_PRECONDITIONS(pBc);

    // Don't issue a reset, the reset causes a BUS hang.
    // It's removed from the firmware (for now). 
    // We need to clear the response fifo if data is still in there
    return DtBcFMBC_EmtptyResponseFifo(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_EmtptyResponseFifo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcFMBC_EmtptyResponseFifo(DtBcFMBC* pBc)
{
    UInt32 Load;
    
    // Check the response fifo load
    Load = FMBC_MailboxClientS10_RespFifoStat_READ_RespFillLevel(pBc);
    while (Load > 0)
    {
        FMBC_MailboxClientS10_ResponseData_READ_ResponseData(pBc);
        // Re-read the load every time to be sure we have the actual value
        Load = FMBC_MailboxClientS10_RespFifoStat_READ_RespFillLevel(pBc);
    }
    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_WriteReadTransfer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcFMBC_WriteReadTransfer(DtBcFMBC* pBc, DtBcFMBC_Command Command,
                                         Int WriteLength,  const UInt32* pWriteBuffer,
                                         Int ReadLength, UInt32* pReadBuffer, Int Timeout)
{
    DtStatus Status = DT_STATUS_OK;
    Int MsgId = 0;

    // Convert to FMBC command
    Int FmbcCmd = DtBcFMBC_ToFMBCCommand(Command);
    if (FmbcCmd < 0)
    {
        DtDbgOutBc(ERR, FMBC, pBc, "ERROR: Unsupported command: %d", Command);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check command and response size
    if (WriteLength>pBc->m_CmdFifoSize || ReadLength>pBc->m_RespFifoSize)
    {
        DtDbgOutBc(ERR, FMBC, pBc, "ERROR: Invalid write (%d) or read (%d) length",
                                                                 WriteLength, ReadLength);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Increment message-id
    MsgId = pBc->m_MessageId;
    pBc->m_MessageId =  (pBc->m_MessageId +1) % 0xF;

    // Be sure the response fifo is empty before writing the command
    DtBcFMBC_EmtptyResponseFifo(pBc);

    // Write the command
    Status = DtBcFMBC_WriteCommandPacket(pBc, MsgId, FmbcCmd, WriteLength, pWriteBuffer,
                                                                                 Timeout);
    if (Status != DT_STATUS_OK)
    {
        DtDbgOutBc(ERR, FMBC, pBc, "ERROR: DtBcFMBC_WriteCommandPacket failed: 0x%x",
                                                                                  Status);
        return Status;
    }
    
    // Read response
    Status = DtBcFMBC_ReadCommandResponse(pBc, MsgId, ReadLength, pReadBuffer, Timeout);
    if (Status != DT_STATUS_OK)
        DtDbgOutBc(ERR, FMBC, pBc, "ERROR: DtBcFMBC_ReadCommandResponse failed: 0x%x",
                                                                            Status);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_ReadCommandResponse -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// A read with length = 0, just waits for the response message without data.
// ReadLength is in number of words
//
DtStatus DtBcFMBC_ReadCommandResponse(DtBcFMBC* pBc, Int MsgId, Int ReadLength, 
                                                         UInt32* pReadBuffer, Int Timeout)
{
    DtStatus Status = DT_STATUS_OK;
    Int Load = 0;
    UInt32 RespFifoStat=0, ResponseHdr=0;
    UInt32 SOP=0, ErrCode=0, RespId=0, RespLength=0, RemainLength=0;

    // Sanity check
    BC_FMBC_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(ReadLength==0 || pReadBuffer!=NULL);

    Status = DtBcFMBC_WaitForResponseFifoload(pBc, &Load, Timeout);
    if (Status != DT_STATUS_OK)
    {
        DtDbgOutBc(ERR, FMBC, pBc,"ERROR: timeout waiting for data valid with load");
        return Status;
    }

    // Check Fifo status
    RespFifoStat = FMBC_MailboxClientS10_RespFifoStat_READ(pBc);
    Load = (Int)FMBC_MailboxClientS10_RespFifoStat_GET_RespFillLevel(RespFifoStat);
    SOP = FMBC_MailboxClientS10_RespFifoStat_GET_SOP(RespFifoStat);
    if (SOP==0 || Load==0)
    {
        DtDbgOutBc(ERR, FMBC, pBc, "ERROR: FIFO status SOP: %d, Load: %d", SOP, Load);
        return DT_STATUS_FAIL;
    }

    // Special case when length is 0
    ResponseHdr = FMBC_MailboxClientS10_ResponseHdr_READ(pBc);
    RespLength = FMBC_MailboxClientS10_ResponseHdr_GET_Length(ResponseHdr);
    if (RespLength == 0)
    {
        // EOP must be 1
        if (FMBC_MailboxClientS10_RespFifoStat_GET_EOP(RespFifoStat) == 0)
        {
            DtDbgOutBc(ERR, FMBC, pBc, "ERROR: FIFO status EOP=0");
            return DT_STATUS_FAIL;
        }
        // Check error code
        ErrCode = FMBC_MailboxClientS10_ResponseHdr_GET_ErrCode(ResponseHdr);
        if (ErrCode != FMBC_RESP_OK)
        {
            DtDbgOutBc(ERR, FMBC, pBc, "ERROR: response error code: %d", ErrCode);
            return DT_STATUS_FAIL;
        }
        // Check message RespId
        RespId = FMBC_MailboxClientS10_ResponseHdr_GET_ID(ResponseHdr);
        if (RespId != MsgId)
        {
            DtDbgOutBc(ERR, FMBC, pBc, "ERROR: unexpected message RespId: %d, expected: %d",
                                                                           RespId, MsgId);
            return DT_STATUS_FAIL;
        }
        // Check expected length
        if (ReadLength != RespLength)
        {
            DtDbgOutBc(ERR, FMBC, pBc, "ERROR: unexpected response length: %d, expected: %d",
                                                                  RespLength, ReadLength);
            return DT_STATUS_FAIL;
        }
        return DT_STATUS_OK;
    }

    // Check RespLength
    if (ReadLength < (Int)RespLength)
    {
        DtDbgOutBc(ERR, FMBC, pBc, "ERROR: unexpected response length: %d, expected: %d",
                                                                  RespLength, ReadLength);
        return DT_STATUS_BUF_TOO_SMALL;
    }

    // Read data
    RespFifoStat = FMBC_MailboxClientS10_RespFifoStat_READ(pBc);
    Load = (Int)FMBC_MailboxClientS10_RespFifoStat_GET_RespFillLevel(RespFifoStat);
    RemainLength = RespLength;
    for(;;)
    {
        while (Load == 0 && Status==DT_STATUS_OK)
        {
            Status = DtBcFMBC_WaitForResponseFifoload(pBc, &Load, Timeout);
        }
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutBc(ERR, FMBC, pBc, "ERROR: timeout waiting for load");
            return Status;
        }
        // Don't read the last word
        if (RemainLength > 1)
        {
            *pReadBuffer++ = FMBC_MailboxClientS10_ResponseData_READ_ResponseData(pBc);
            Load--;
            RemainLength--;
        } else
            break;
    }

    // EOP must be 1
    RespFifoStat = FMBC_MailboxClientS10_RespFifoStat_READ(pBc);
    if (FMBC_MailboxClientS10_RespFifoStat_GET_EOP(RespFifoStat) == 0)
    {
        DtDbgOutBc(ERR, FMBC, pBc, "ERROR: FIFO status EOP=0");
        return DT_STATUS_FAIL;
    }

    // Read last word
    *pReadBuffer++ = FMBC_MailboxClientS10_ResponseData_READ_ResponseData(pBc);

    // Check error code
    ErrCode = FMBC_MailboxClientS10_ResponseHdr_GET_ErrCode(ResponseHdr);
    if (ErrCode != FMBC_RESP_OK)
    {
        DtDbgOutBc(ERR, FMBC, pBc, "ERROR: response error code: %d", ErrCode);
        return DT_STATUS_FAIL;
    }
    // Check message RespId
    RespId = FMBC_MailboxClientS10_ResponseHdr_GET_ID(ResponseHdr);
    if (RespId != MsgId)
    {
        DtDbgOutBc(ERR, FMBC, pBc, "ERROR: unexpected message RespId: %d, expected: %d",
                                                                           RespId, MsgId);
        return DT_STATUS_FAIL;
    }
    // Check expected length
    if (ReadLength > (Int)RespLength)
    {
        DtDbgOutBc(ERR, FMBC, pBc, "ERROR: unexpected response length: %d, expected: %d",
                                                                  RespLength, ReadLength);
        return DT_STATUS_BUF_TOO_LARGE;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_WriteCommandPacket -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// WriteLength is in number of words
//
DtStatus DtBcFMBC_WriteCommandPacket(DtBcFMBC* pBc, Int MsgId, Int Command, 
                                 Int WriteLength, const UInt32* pWriteBuffer, Int Timeout)
{
    DtStatus Status = DT_STATUS_OK;
    Int NumFree = 0;
    UInt32 Data = 0;

    // Sanity check
    BC_FMBC_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(WriteLength==0 || pWriteBuffer!=NULL);

    // Wait for space
    Status = DtBcFMBC_WaitForEmptyCmdSpace(pBc, &NumFree, Timeout);
    if (Status != DT_STATUS_OK)
    {
        DtDbgOutBc(ERR, FMBC, pBc, "ERROR: timeout waiting for command space");
        return Status;
    }

    // Special case when length is 0
    if (WriteLength == 0)
    {
        // Write the Command-End-Of-Packet header
        UInt32 EopData = 0;
        EopData = FMBC_MailboxClientS10_CommandEopHdr_SET_CmdCode(EopData, Command);
        EopData = FMBC_MailboxClientS10_CommandEopHdr_SET_Length(EopData, 0);
        EopData = FMBC_MailboxClientS10_CommandEopHdr_SET_ID(EopData, MsgId);
        FMBC_MailboxClientS10_CommandEopHdr_WRITE(pBc, EopData);
        return DT_STATUS_OK;
    }

    // Write the command header
    Data = 0;
    Data = FMBC_MailboxClientS10_CommandHdr_SET_CmdCode(Data, Command);
    Data = FMBC_MailboxClientS10_CommandHdr_SET_Length(Data, WriteLength);
    Data = FMBC_MailboxClientS10_CommandHdr_SET_ID(Data, MsgId);
    FMBC_MailboxClientS10_CommandHdr_WRITE(pBc, Data);
    NumFree--;

    // Write data, except last data word
    while (WriteLength > 1)
    {
        if (NumFree <= 0)
        {
            // Wait for space
            Status = DtBcFMBC_WaitForEmptyCmdSpace(pBc, &NumFree, Timeout);
            if (Status != DT_STATUS_OK)
            {
                DtDbgOutBc(ERR, FMBC, pBc, "ERROR: timeout waiting for command space");
                return Status;
            }
        }
        FMBC_MailboxClientS10_CommandData_WRITE_CommandData(pBc, *pWriteBuffer++);
        NumFree--;
        WriteLength--;
    }
    if (NumFree <= 0)
    {
        // Wait for space
        Status = DtBcFMBC_WaitForEmptyCmdSpace(pBc, &NumFree, Timeout);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutBc(ERR, FMBC, pBc, "ERROR: timeout waiting for command space");
            return Status;
        }
    }
    // Write last data word
    FMBC_MailboxClientS10_CommandEopData_WRITE_CommandEopData(pBc, *pWriteBuffer++);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_WaitForEmptyCmdSpace -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
//
DtStatus DtBcFMBC_WaitForEmptyCmdSpace(DtBcFMBC* pBc, Int* pNumFree, Int Timeout)
{
    Int CmdSpace = (Int)FMBC_MailboxClientS10_CmdSpace_READ_CmdSpace(pBc);
    while (CmdSpace<=0 && Timeout>0)
    {
        // It is not expected that there is no space.
        DtSleep(1);
        Timeout -= 1;
        CmdSpace = (Int)FMBC_MailboxClientS10_CmdSpace_READ_CmdSpace(pBc);
    }
    *pNumFree = CmdSpace;
    if (CmdSpace > 0)
        return DT_STATUS_OK;
    else
        return DT_STATUS_TIMEOUT;
}



// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_WaitForResponseFifoload -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcFMBC_WaitForResponseFifoload(DtBcFMBC* pBc, Int* pLoad, Int Timeout)
{
    DtStatus Status;
    UInt64 StartTime;

    Int DataValid = (Int)FMBC_MailboxClientS10_IntStatus_READ_DataValid(pBc);
    if (DataValid == 0)
    {
        // Wait for data valid
        Status = DtEventReset(&pBc->m_StatusEvent);

        // Enable data valid interrupt
        FMBC_MailboxClientS10_IntEnable_WRITE(pBc,
                                   FMBC_MailboxClientS10_IntEnable_SET_EnDataValid(0, 1));
        
        // Wait for the interrupt or Timeout
        Status = DtEventWaitUnInt(&pBc->m_StatusEvent, Timeout);

        // Disable all interrupts 
        FMBC_MailboxClientS10_IntEnable_WRITE(pBc, 0);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, FMBC, pBc, "[%i:%0x] Error waiting for data valid interrupt.",
                                              pBc->m_IntProps[0].m_Index, pBc->m_Address);

            // Check if datavalid is active without getting the data valid interrupt
            // If so, we can ignore the timeout
            DataValid = (Int)FMBC_MailboxClientS10_IntStatus_READ_DataValid(pBc);
            if (DataValid != 0)
                Status = DT_STATUS_OK;
        }

        if (!DT_SUCCESS(Status))
            return Status;
    }
    
    *pLoad = (Int)FMBC_MailboxClientS10_RespFifoStat_READ_RespFillLevel(pBc);
    StartTime = DtGetTickCount();

    // Wait for load with timeout
    while (*pLoad == 0 && (DtGetTickCount()-StartTime < Timeout))
    {
        DtSleep(1);
        *pLoad = (Int)FMBC_MailboxClientS10_RespFifoStat_READ_RespFillLevel(pBc);
    }
    if (*pLoad == 0)
    {
        DtDbgOutBc(ERR, FMBC, pBc, "Error waiting for load. Timeout!");
        return DT_STATUS_TIMEOUT;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFMBC_ToFMBCCommand -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int DtBcFMBC_ToFMBCCommand(DtBcFMBC_Command Command)
{
    switch (Command)
    {
    case DT_BC_FMBC_CMD_NOOP:                   return FMBC_CMD_NOOP;
    case DT_BC_FMBC_CMD_CONFIG_STATUS:          return FMBC_CMD_CONFIG_STATUS;
    case DT_BC_FMBC_CMD_GET_IDCODE:             return FMBC_CMD_GET_IDCODE;
    case DT_BC_FMBC_CMD_GET_CHIPID:             return FMBC_CMD_GET_CHIPID;
    case DT_BC_FMBC_CMD_GET_USERCODE:           return FMBC_CMD_GET_USERCODE;
    case DT_BC_FMBC_CMD_GET_VOLTAGE:            return FMBC_CMD_GET_VOLTAGE;
    case DT_BC_FMBC_CMD_GET_TEMPERATURE:        return FMBC_CMD_GET_TEMPERATURE;
    case DT_BC_FMBC_CMD_QSPI_OPEN:              return FMBC_CMD_QSPI_OPEN;
    case DT_BC_FMBC_CMD_QPSI_CLOSE:             return FMBC_CMD_QPSI_CLOSE;
    case DT_BC_FMBC_CMD_QSPI_SET_CS:            return FMBC_CMD_QSPI_SET_CS;
    case DT_BC_FMBC_CMD_QSPI_READ_DEVICE_REG:   return FMBC_CMD_QSPI_READ_DEVICE_REG;
    case DT_BC_FMBC_CMD_QPSI_WRITE_DEVICE_REG:  return FMBC_CMD_QPSI_WRITE_DEVICE_REG;
    case DT_BC_FMBC_CMD_QPSI_SEND_DEVICE_OP:    return FMBC_CMD_QPSI_SEND_DEVICE_OP;
    case DT_BC_FMBC_CMD_QPSI_ERASE:             return FMBC_CMD_QPSI_ERASE;
    case DT_BC_FMBC_CMD_QSPI_WRITE:             return FMBC_CMD_QSPI_WRITE;
    case DT_BC_FMBC_CMD_QSPI_READ:              return FMBC_CMD_QSPI_READ;
    case DT_BC_FMBC_CMD_READ_SEU_ERROR:         return FMBC_CMD_READ_SEU_ERROR;
    case DT_BC_FMBC_CMD_RSU_GET_SPT:            return FMBC_CMD_RSU_GET_SPT;
    case DT_BC_FMBC_CMD_RSU_STATUS:             return FMBC_CMD_RSU_STATUS;
    case DT_BC_FMBC_CMD_RSU_IMAGE_UPDATE:       return FMBC_CMD_RSU_IMAGE_UPDATE;
    case DT_BC_FMBC_CMD_RSU_NOTIFY:             return FMBC_CMD_RSU_NOTIFY;
    case DT_BC_FMBC_CMD_GET_CONFIGURATION_TIME: return FMBC_CMD_GET_CONFIGURATION_TIME;
    case DT_BC_FMBC_CMD_QPSI_READ_SHA:          return FMBC_CMD_QPSI_READ_SHA;
    }
    return -1;
}
