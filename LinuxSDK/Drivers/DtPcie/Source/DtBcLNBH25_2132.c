//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcLNBH25_2132.c *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2017 DekTec Digital Video B.V.
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
#include "DtBcLNBH25_2132.h"
#include "DtBcLNBH25_2132_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcLNBH25_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_LNBH25_2132_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcLNBH25_2132))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_LNBH25_2132_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(LNBH25_2132, pBc)

// Helper macro to cast a DtBc* to a DtBcLNBH25_2132*
#define BC_LNBH25_2132         ((DtBcLNBH25_2132*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcLNBH25_2132_Init(DtBc*);
static void  DtBcLNBH25_2132_InterruptDpc(DtDpcArgs* pArgs);
static DtStatus  DtBcLNBH25_2132_InterruptHandler(DtBc*, Int, Int, void*);
static DtStatus  DtBcLNBH25_2132_RegisterIntHandlers(DtBcLNBH25_2132*);

//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcLNBH25_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLNBH25_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcLNBH25_2132_Close(DtBc*  pBc)
{
    BC_LNBH25_2132_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLNBH25_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcLNBH25_2132*  DtBcLNBH25_2132_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_LNBH25_2132_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcLNBH25_2132, Id, DT_BLOCK_TYPE_LNBH25_2132, 
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcLNBH25_2132_Close;
    OpenParams.m_InitFunc = DtBcLNBH25_2132_Init;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcLNBH25_2132*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLNBH25_2132_SendToneBurst -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcLNBH25_2132_SendToneBurst(DtBcLNBH25_2132* pBc, Int ToneBurst)
{
     DtStatus  Status = DT_STATUS_OK;
     UInt32  RegData;

    // Sanity check
    BC_LNBH25_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (ToneBurst!=DT_LNBH25_2132_TONEBURST_A && ToneBurst!=DT_LNBH25_2132_TONEBURST_B)
        return DT_STATUS_INVALID_PARAMETER;

    // Protect LNB access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Reset the transmitter
    RegData = LNBH25_TxControl_SET_Command(0, LNBH25_CMD_NOP);
    RegData = LNBH25_TxControl_SET_Reset(RegData, LNBH25_CMD_RESET);
    LNBH25_TxControl_WRITE(pBc, RegData);

    // Clear failure flag
    LNBH25_FaultControl_PULSE_Reset(pBc);
    pBc->m_Failure = FALSE;

    // Reset done event
    DtEventReset(&pBc->m_TxDoneEvent);

    // Enable interrupts
    Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_LNBH25_2132_TXDONE);
    if (DT_SUCCESS(Status))
        Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_LNBH25_2132_FAULT);

    if (!DT_SUCCESS(Status))
    {
       // Enabling interrupts failed, disable them all
        DtDbgOutBc(ERR, LNBH25_2132, pBc, "Failed to register interrupts");
        DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_LNBH25_2132_TXDONE);
        DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_LNBH25_2132_FAULT);
        // Release mutex
        DtFastMutexRelease(&pBc->m_AccessMutex);
        DT_ASSERT(FALSE);
        return Status;
    }

    // Send tone burst
    RegData = LNBH25_TxControl_SET_Command(0, ToneBurst==DT_LNBH25_2132_TONEBURST_A 
                             ? LNBH25_CMD_SEND_TONEBURST_A : LNBH25_CMD_SEND_TONEBURST_B);
    RegData = LNBH25_TxControl_SET_Reset(RegData, LNBH25_CMD_NOP);
    LNBH25_TxControl_WRITE(pBc, RegData);

    // Wait for done interrupt
    Status = DtEventWaitUnInt(&pBc->m_TxDoneEvent, 5000);
    if (!DT_SUCCESS(Status))
    { 
        DtDbgOutBc(ERR, LNBH25_2132, pBc,
                                   "ERROR: Wait for done failed (Status=0x%08X)", Status);
    }

    // Failed?
    if (pBc->m_Failure)
    { 
        DtDbgOutBc(ERR, LNBH25_2132, pBc, "LNB indicated failure");
        Status = DT_STATUS_FAIL;
    }

    // Release mutex
    DtFastMutexRelease(&pBc->m_AccessMutex);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLNBH25_2132_SendReceiveMessage -.-.-.-.-.-.-.-.-.-.-.-.-.
//
//
DtStatus DtBcLNBH25_2132_SendReceiveMessage(DtBcLNBH25_2132* pBc, const UInt8* pMsgSend, 
                                             Int NumToSend, UInt8* pMsgRcv, Int* pNumRcvd)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32  RegData;
    Bool  NeedToReceive;
    Int  i;

    // Sanity check
    BC_LNBH25_2132_DEFAULT_PRECONDITIONS(pBc);

     // Check parameters
    if (pMsgSend==NULL || NumToSend<1 || NumToSend>pBc->m_TxFifoSize)
        return DT_STATUS_INVALID_PARAMETER;
    if ((pMsgRcv==NULL && pNumRcvd!=NULL) || (pMsgRcv!=NULL && pNumRcvd==NULL))
        return DT_STATUS_INVALID_PARAMETER;
    if (pMsgRcv!=NULL && *pNumRcvd<0)
        return DT_STATUS_INVALID_PARAMETER;

    NeedToReceive = pMsgRcv!=NULL && *pNumRcvd>0;

    // Protect LNB access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Reset the transmitter
    RegData = LNBH25_TxControl_SET_Command(0, LNBH25_CMD_NOP);
    RegData = LNBH25_TxControl_SET_Reset(RegData, LNBH25_CMD_RESET);
    LNBH25_TxControl_WRITE(pBc, RegData);

    // Reset the receiver
    RegData = LNBH25_RxControl_SET_RxEnable(0, LNBH25_CMD_DISABLED);
    RegData = LNBH25_RxControl_SET_RxFifoClear(RegData, LNBH25_CMD_CLEAR);
    LNBH25_RxControl_WRITE(pBc, RegData);

    // Clear failure flag
    LNBH25_FaultControl_PULSE_Reset(pBc);
    pBc->m_Failure = FALSE;

    // Reset done event
    DtEventReset(&pBc->m_TxDoneEvent);
    DtEventReset(&pBc->m_RxDoneEvent);

    // Enable interrupts
    Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_LNBH25_2132_TXDONE);
    if (DT_SUCCESS(Status) && NeedToReceive)
        Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_LNBH25_2132_RXDONE);
    if (DT_SUCCESS(Status))
        Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_LNBH25_2132_FAULT);
    if (!DT_SUCCESS(Status))
    {
        // Enabling interrupts failed, disable them all
        DtDbgOutBc(ERR, LNBH25_2132, pBc, "Failed to register interrupts");
        DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_LNBH25_2132_TXDONE);
        DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_LNBH25_2132_RXDONE);
        DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_LNBH25_2132_FAULT);
        // Release mutex
        DtFastMutexRelease(&pBc->m_AccessMutex);
        DT_ASSERT(FALSE);
        return Status;
    }


    // Write transmit message
    for (i=0; i<NumToSend; i++)
        LNBH25_TxData_WRITE(pBc, pMsgSend[i]);

    // Transmit FIFO
    RegData = LNBH25_TxControl_SET_Command(0, LNBH25_CMD_SEND_TX_FIFO);
    RegData = LNBH25_TxControl_SET_Reset(RegData, LNBH25_CMD_NOP);
    LNBH25_TxControl_WRITE(pBc, RegData);

    // Wait for TX done interrupt
    Status = DtEventWaitUnInt(&pBc->m_TxDoneEvent, 5000);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, LNBH25_2132, pBc,
            "ERROR: Wait for TX done failed (Status=0x%08X)", Status);
    }
    // Failed?
    if (pBc->m_Failure)
    {
        DtDbgOutBc(ERR, LNBH25_2132, pBc, "LNB indicated TX failure");
        Status = DT_STATUS_FAIL;
    }

    // Do we need to receive a message?
    if (DT_SUCCESS(Status) && NeedToReceive)
    {
        // Enable the receiver
        RegData = LNBH25_RxControl_SET_RxEnable(0, LNBH25_CMD_ENABLED);
        RegData = LNBH25_RxControl_SET_RxFifoClear(RegData, LNBH25_CMD_NOP);
        LNBH25_RxControl_WRITE(pBc, RegData);

        // Wait for RX done interrupt
        Status = DtEventWaitUnInt(&pBc->m_RxDoneEvent, 5000);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, LNBH25_2132, pBc,
                "ERROR: Wait for RX done failed (Status=0x%08X)", Status);
        }
        // Failed?
        if (pBc->m_Failure)
        {
            DtDbgOutBc(ERR, LNBH25_2132, pBc, "LNB indicated RX failure");
            Status = DT_STATUS_FAIL;
        }

        // Check message size and copy data
        if (DT_SUCCESS(Status))
        {
            Int RxFifoLoad = (Int)LNBH25_RxStatus_READ_FifoLoad(pBc);
            if (RxFifoLoad <= *pNumRcvd)
            {
                // Read message
                for (i=0; i<RxFifoLoad; i++)
                    pMsgRcv[i] = (UInt8)LNBH25_RxData_READ(pBc);
                *pNumRcvd = RxFifoLoad;
            }
            else
                Status = DT_STATUS_BUF_TOO_SMALL;
        }
    }

    if (!DT_SUCCESS(Status) && NeedToReceive)
        *pNumRcvd = 0;

    // Make sure all interrupts are disabled
    DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_LNBH25_2132_TXDONE);
    DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_LNBH25_2132_RXDONE);
    DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_LNBH25_2132_FAULT);

    // Release mutex
    DtFastMutexRelease(&pBc->m_AccessMutex);

    return Status;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcLNBH25_2132 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLNBH25_2132_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcLNBH25_2132_Init(DtBc*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity checks
    BC_LNBH25_2132_DEFAULT_PRECONDITIONS(pBc);

    // Init interrupt DPC
    Status = DtDpcInit(&BC_LNBH25_2132->m_IntDpc, DtBcLNBH25_2132_InterruptDpc, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, LNBH25_2132, pBc, 
                                     "ERROR: failed to init DPC (Status=0x%08X)", Status);
        return Status;
    }

    // Read config
    BC_LNBH25_2132->m_TxFifoSize = LNBH25_Configuration_READ_TxFifoSize(BC_LNBH25_2132);
    BC_LNBH25_2132->m_RxFifoSize = LNBH25_Configuration_READ_RxFifoSize(BC_LNBH25_2132);

    // Init mutex
    DtFastMutexInit(&BC_LNBH25_2132->m_AccessMutex);

    // Init TX completion event
    Status = DtEventInit(&BC_LNBH25_2132->m_TxDoneEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, LNBH25_2132, pBc, 
                           "ERROR: failed to init tX done event (Status=0x%08X)", Status);
        return Status;
    }
    // Init completion event
    Status = DtEventInit(&BC_LNBH25_2132->m_RxDoneEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, LNBH25_2132, pBc, 
                           "ERROR: failed to init RX done event (Status=0x%08X)", Status);
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    Status = DtBcLNBH25_2132_RegisterIntHandlers(BC_LNBH25_2132);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, LNBH25_2132, pBc, "ERROR: failed to register interrupt handler");
        return Status;
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLNBH25_2132_InterruptDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcLNBH25_2132_InterruptDpc(DtDpcArgs * pArgs)
{
    DtBcLNBH25_2132*  pBc = (DtBcLNBH25_2132*)pArgs->m_pContext;
    Int Id = (Int)pArgs->m_Data1.m_UInt32_1;

    if (Id == DT_INTERRUPT_LNBH25_2132_TXDONE)
    { 
        DtDbgOutBc(MAX, LNBH25_2132, pBc, "Transfer completed");
        DtEventSet(&pBc->m_TxDoneEvent);
    }
    else if (Id == DT_INTERRUPT_LNBH25_2132_RXDONE)
    { 
        DtDbgOutBc(MAX, LNBH25_2132, pBc, "Receive completed");
        DtEventSet(&pBc->m_RxDoneEvent);
    }
    else if (Id == DT_INTERRUPT_LNBH25_2132_FAULT)
    { 
        DtDbgOutBc(MIN, LNBH25_2132, pBc, "Transfer failed");
        pBc->m_Failure = TRUE;
        DtEventSet(&pBc->m_TxDoneEvent);
        DtEventSet(&pBc->m_RxDoneEvent);
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLNBH25_2132_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcLNBH25_2132_InterruptHandler(DtBc*  pBc, Int Id, Int Index, void*  pContext)
{
    DtStatus  Status=DT_STATUS_OK;
    DtDpcArgs  DpcArgs;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcLNBH25_2132));

    // Check ID is one of ours
    if (Id!=DT_INTERRUPT_LNBH25_2132_TXDONE && Id!=DT_INTERRUPT_LNBH25_2132_RXDONE
                                                    && Id!=DT_INTERRUPT_LNBH25_2132_FAULT)
    {
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Disable the LNBH25 interrupt that was fired. 
    // NOTE: is re-enabled upon next transfer
    DtBc_InterruptDisable(pBc, Id);

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    DpcArgs.m_Data1.m_UInt32_2 = 0;
    
    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&((DtBcLNBH25_2132*)pBc)->m_IntDpc, &DpcArgs);
    DT_ASSERT(DT_SUCCESS(Status));

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLNBH25_2132_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcLNBH25_2132_RegisterIntHandlers(DtBcLNBH25_2132* pBc)
{
    Int  i=0;
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_LNBH25_2132_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks
    for (i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch (Id)
        {
        case DT_INTERRUPT_LNBH25_2132_RXDONE:
        case DT_INTERRUPT_LNBH25_2132_TXDONE:
        case DT_INTERRUPT_LNBH25_2132_FAULT:
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id,
                                                  DtBcLNBH25_2132_InterruptHandler, NULL);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, LNBH25_2132, pBc,
                              "ERROR: failed to register interrupt handler (ID=%d)", Id);
            }
            break;

        default:
            DT_ASSERT(FALSE);      // Unknown ID. New version of the block??
            break;
        }
    }
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcLNBH25_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcLNBH25_2132
#define LNBH25_2132_STUB   ((DtIoStubBcLNBH25_2132*)pStub)
#define LNBH25_2132_BC  ((DtBcLNBH25_2132*)LNBH25_2132_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcLNBH25_2132_AppendDynamicSize(const DtIoStub*, 
                                                                      DtIoStubIoParams *);
static DtStatus  DtIoStubBcLNBH25_2132_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);

static DtStatus DtIoStubBcLNBH25_2132_OnCmdSendToneBurst(
                                          const DtIoStubBcLNBH25_2132* , 
                                          const DtIoctlLnbh25Cmd_2132SendToneBurstInput*);

static DtStatus DtIoStubBcLNBH25_2132_OnCmdSendMessage(
                                            const DtIoStubBcLNBH25_2132* , 
                                            const DtIoctlLnbh25Cmd_2132SendMessageInput*);

static DtStatus DtIoStubBcLNBH25_2132_OnCmdSendReceiveMessage(
                                      const DtIoStubBcLNBH25_2132* , 
                                      const DtIoctlLnbh25Cmd_2132SendReceiveMessageInput*,
                                      DtIoctlLnbh25Cmd_2132SendReceiveMessageOutput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_LNBH25_2132;

static const DtIoctlProperties  IOSTUB_BC_LNBH25_2132_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_LNBH25_CMD_2132(
        DtIoStubBcLNBH25_2132_OnCmd,
        DtIoStubBcLNBH25_2132_AppendDynamicSize,
        NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcLNBH25_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLNBH25_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcLNBH25_2132_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcLNBH25_2132));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLNBH25_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcLNBH25_2132*  DtIoStubBcLNBH25_2132_Open(DtBc*  pBc)
{
    DtIoStubBcLNBH25_2132*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcLNBH25_2132));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcLNBH25_2132, pBc, NULL, 
                                                           DtIoStubBcLNBH25_2132_Close,
                                                           NULL,  // Use default IOCTL
                                                           IOSTUB_BC_LNBH25_2132_IOCTLS);
    pStub = (DtIoStubBcLNBH25_2132*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcLNBH25_2132- Private functions +=+=+=+=+=+=+=+=+=+=+=+


//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLNBH25_2132_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcLNBH25_2132_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlLnbh25CmdInput_2132*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcLNBH25_2132));
    DT_ASSERT(pIoParams != NULL);                 
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_LNBH25_CMD_2132);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_Lnbh25Cmd_2132;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_LNBH25_CMD_2132_SEND_MESSAGE:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize 
                                        >= sizeof(DtIoctlLnbh25Cmd_2132SendMessageInput));

        // Add dynamic size (i.e. #bytes to send)
        pIoParams->m_InReqSize += pInData->m_SndMsg.m_NumToSend;
        break;

    case DT_LNBH25_CMD_2132_SEND_RECEIVE_MESSAGE:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize 
                                 >= sizeof(DtIoctlLnbh25Cmd_2132SendReceiveMessageInput));
        DT_ASSERT(pIoParams->m_OutReqSize 
                                >= sizeof(DtIoctlLnbh25Cmd_2132SendReceiveMessageOutput));

        // Add dynamic size (i.e. #bytes to send)
        pIoParams->m_InReqSize += pInData->m_SndRcvMsg.m_NumToSend;
        // Add dynamic size (i.e. #bytes to receive)
        pIoParams->m_OutReqSize += pInData->m_SndRcvMsg.m_RcvBufSize;
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLNBH25_2132_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcLNBH25_2132_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlLnbh25CmdInput_2132*  pInData = NULL;
    DtIoctlLnbh25CmdOutput_2132*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcLNBH25_2132));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_LNBH25_CMD_2132);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, LNBH25_2132, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_Lnbh25Cmd_2132;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_Lnbh25Cmd_2132;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_LNBH25_CMD_2132_SEND_TONEBURST:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcLNBH25_2132_OnCmdSendToneBurst(LNBH25_2132_STUB,
                                                                &pInData->m_SndToneBurst);
        break;
    case DT_LNBH25_CMD_2132_SEND_MESSAGE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcLNBH25_2132_OnCmdSendMessage(LNBH25_2132_STUB,
                                                                      &pInData->m_SndMsg);
        break;
    case DT_LNBH25_CMD_2132_SEND_RECEIVE_MESSAGE:
        DT_ASSERT(pInData!=NULL && pOutData!=NULL);
        Status = DtIoStubBcLNBH25_2132_OnCmdSendReceiveMessage(LNBH25_2132_STUB,
                                           &pInData->m_SndRcvMsg, &pOutData->m_SndRcvMsg);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLNBH25_2132_OnCmdSendToneBurst -.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcLNBH25_2132_OnCmdSendToneBurst(
    const DtIoStubBcLNBH25_2132* pStub,
    const DtIoctlLnbh25Cmd_2132SendToneBurstInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcLNBH25_2132));
    DT_ASSERT(pInData!=NULL);

    return DtBcLNBH25_2132_SendToneBurst(LNBH25_2132_BC, pInData->m_ToneBurst);
}

//-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLNBH25_2132_OnCmdSendMessage -.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcLNBH25_2132_OnCmdSendMessage(
                                    const DtIoStubBcLNBH25_2132*  pStub, 
                                    const DtIoctlLnbh25Cmd_2132SendMessageInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcLNBH25_2132));
    DT_ASSERT(pInData != NULL);

    return DtBcLNBH25_2132_SendReceiveMessage(LNBH25_2132_BC, pInData->m_Buf, 
                                                        pInData->m_NumToSend, NULL, NULL);

}

//.-.-.-.-.-.-.-.-.-.- DtIoStubBcLNBH25_2132_OnCmdSendReceiveMessage -.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcLNBH25_2132_OnCmdSendReceiveMessage(
                              const DtIoStubBcLNBH25_2132*  pStub, 
                              const DtIoctlLnbh25Cmd_2132SendReceiveMessageInput* pInData,
                              DtIoctlLnbh25Cmd_2132SendReceiveMessageOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcLNBH25_2132));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);


    pOutData->m_NumRcvd = pInData->m_RcvBufSize;
    return DtBcLNBH25_2132_SendReceiveMessage(LNBH25_2132_BC, pInData->m_Buf,
                             pInData->m_NumToSend, pOutData->m_Buf, &pOutData->m_NumRcvd);
}