// =+=+=+=+=+=+=+=+=+ DtBcDISEQC.c *#*#*#*#*#*#*#*#*#* (C) 2020 DekTec +=+=+=+=+=+=+=+=+=+
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
#define NULL 0
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBcDISEQC.h"
#include "DtBcDISEQC_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDISEQC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_DISEQC_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcDISEQC))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_DISEQC_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(DISEQC, pBc)

// Helper macro to cast a DtBc* to a DtBcDISEQC*
#define BC_DISEQC         ((DtBcDISEQC*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcDISEQC_Init(DtBc*);
static void  DtBcDISEQC_InterruptDpc(DtDpcArgs* pArgs);
static DtStatus  DtBcDISEQC_InterruptHandler(DtBc*, Int, Int, void*);
static DtStatus  DtBcDISEQC_RegisterIntHandlers(DtBcDISEQC*);

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDISEQC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDISEQC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcDISEQC_Close(DtBc*  pBc)
{
    BC_DISEQC_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDISEQC_EnableTone -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcDISEQC_EnableTone(DtBcDISEQC* pBc, Int EnableTone)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32  RegData;

    // Sanity check
    BC_DISEQC_DEFAULT_PRECONDITIONS(pBc);


    // Protect LNB access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Reset the transmitter
    RegData = DISEQC_TxControl_READ(pBc);
    RegData = DISEQC_TxControl_SET_Command(RegData, DISEQC_CMD_NOP);
    RegData = DISEQC_TxControl_SET_Reset(RegData, DISEQC_CMD_RESET);
    DISEQC_TxControl_WRITE(pBc, RegData);

    // Generate a 22 kHz tone.
    RegData = DISEQC_TxControl_SET_Command(0, (Bool)EnableTone
                                     ? DISEQC_CMD_GENERATE_22KHZ : DISEQC_CMD_STOP_22KHZ);
    RegData = DISEQC_TxControl_SET_Reset(RegData, DISEQC_CMD_NOP);
    DISEQC_TxControl_WRITE(pBc, RegData);

    // Release mutex
    DtFastMutexRelease(&pBc->m_AccessMutex);
    return Status;

}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDISEQC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcDISEQC*  DtBcDISEQC_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_DISEQC_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcDISEQC, Id, DT_BLOCK_TYPE_DISEQC, 
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcDISEQC_Close;
    OpenParams.m_InitFunc = DtBcDISEQC_Init;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcDISEQC*)DtBc_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDISEQC_SendToneBurst -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcDISEQC_SendToneBurst(DtBcDISEQC* pBc, Int ToneBurst)
{
     DtStatus  Status = DT_STATUS_OK;
     UInt32  RegData;

    // Sanity check
    BC_DISEQC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (ToneBurst!=DT_DISEQC_TONEBURST_A && ToneBurst!=DT_DISEQC_TONEBURST_B)
        return DT_STATUS_INVALID_PARAMETER;

    // Protect LNB access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Reset the transmitter
    RegData = DISEQC_TxControl_READ(pBc);
    RegData = DISEQC_TxControl_SET_Command(RegData, DISEQC_CMD_NOP);
    RegData = DISEQC_TxControl_SET_Reset(RegData, DISEQC_CMD_RESET);
    DISEQC_TxControl_WRITE(pBc, RegData);

    // Clear failure flag
    RegData = DISEQC_FaultControl_READ(pBc);
    RegData = DISEQC_FaultControl_SET_Reset(RegData, 1);
    DISEQC_FaultControl_WRITE(pBc, RegData);
    pBc->m_Failure = FALSE;

    // Reset done event
    DtEventReset(&pBc->m_TxDoneEvent);

    // Enable interrupts
    Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_DISEQC_TXDONE);
    if (DT_SUCCESS(Status))
        Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_DISEQC_FAULT);

    if (!DT_SUCCESS(Status))
    {
       // Enabling interrupts failed, disable them all
        DtDbgOutBc(ERR, DISEQC, pBc, "Failed to register interrupts");
        DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_DISEQC_TXDONE);
        DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_DISEQC_FAULT);
        // Release mutex
        DtFastMutexRelease(&pBc->m_AccessMutex);
        DT_ASSERT(FALSE);
        return Status;
    }

    // Send tone burst
    RegData = DISEQC_TxControl_SET_Command(0, ToneBurst==DT_DISEQC_TONEBURST_A 
                             ? DISEQC_CMD_SEND_TONEBURST_A : DISEQC_CMD_SEND_TONEBURST_B);
    RegData = DISEQC_TxControl_SET_Reset(RegData, DISEQC_CMD_NOP);
    DISEQC_TxControl_WRITE(pBc, RegData);

    // Wait for done interrupt
    Status = DtEventWaitUnInt(&pBc->m_TxDoneEvent, 5000);
    if (!DT_SUCCESS(Status))
    { 
        DtDbgOutBc(ERR, DISEQC, pBc,
                                   "ERROR: Wait for done failed (Status=0x%08X)", Status);
    }

    // Failed?
    if (pBc->m_Failure)
    { 
        DtDbgOutBc(ERR, DISEQC, pBc, "LNB indicated failure");
        Status = DT_STATUS_FAIL;
    }

    // Release mutex
    DtFastMutexRelease(&pBc->m_AccessMutex);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDISEQC_SendReceiveMessage -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
//
DtStatus DtBcDISEQC_SendReceiveMessage(DtBcDISEQC* pBc, const UInt8* pMsgSend, 
                                             Int NumToSend, UInt8* pMsgRcv, Int* pNumRcvd)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32  RegData;
    Bool  NeedToReceive;
    Int  i;

    // Sanity check
    BC_DISEQC_DEFAULT_PRECONDITIONS(pBc);

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
    RegData = DISEQC_TxControl_SET_Command(0, DISEQC_CMD_NOP);
    RegData = DISEQC_TxControl_SET_Reset(RegData, DISEQC_CMD_RESET);
    DISEQC_TxControl_WRITE(pBc, RegData);

    // Reset the receiver
    RegData = DISEQC_RxControl_SET_RxEnable(0, DISEQC_CMD_DISABLED);
    RegData = DISEQC_RxControl_SET_RxFifoClear(RegData, DISEQC_CMD_CLEAR);
    DISEQC_RxControl_WRITE(pBc, RegData);

    // Clear failure flag
    RegData = DISEQC_FaultControl_READ(pBc);
    RegData = DISEQC_FaultControl_SET_Reset(RegData, 1);
    DISEQC_FaultControl_WRITE(pBc, RegData);
    pBc->m_Failure = FALSE;

    // Reset done event
    DtEventReset(&pBc->m_TxDoneEvent);
    DtEventReset(&pBc->m_RxDoneEvent);

    // Enable interrupts
    Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_DISEQC_TXDONE);
    if (DT_SUCCESS(Status) && NeedToReceive)
        Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_DISEQC_RXDONE);
    if (DT_SUCCESS(Status))
        Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_DISEQC_FAULT);
    if (!DT_SUCCESS(Status))
    {
        // Enabling interrupts failed, disable them all
        DtDbgOutBc(ERR, DISEQC, pBc, "Failed to register interrupts");
        DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_DISEQC_TXDONE);
        DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_DISEQC_RXDONE);
        DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_DISEQC_FAULT);
        // Release mutex
        DtFastMutexRelease(&pBc->m_AccessMutex);
        DT_ASSERT(FALSE);
        return Status;
    }


    // Write transmit message
    for (i=0; i<NumToSend; i++)
        DISEQC_TxData_WRITE(pBc, pMsgSend[i]);

    // Transmit FIFO
    RegData = DISEQC_TxControl_SET_Command(0, DISEQC_CMD_SEND_TX_FIFO);
    RegData = DISEQC_TxControl_SET_Reset(RegData, DISEQC_CMD_NOP);
    DISEQC_TxControl_WRITE(pBc, RegData);

    // Wait for TX done interrupt
    Status = DtEventWaitUnInt(&pBc->m_TxDoneEvent, 5000);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, DISEQC, pBc,
            "ERROR: Wait for TX done failed (Status=0x%08X)", Status);
    }
    // Failed?
    if (pBc->m_Failure)
    {
        DtDbgOutBc(ERR, DISEQC, pBc, "LNB indicated TX failure");
        Status = DT_STATUS_FAIL;
    }

    // Do we need to receive a message?
    if (DT_SUCCESS(Status) && NeedToReceive)
    {
        // Enable the receiver
        RegData = DISEQC_RxControl_SET_RxEnable(0, DISEQC_CMD_ENABLED);
        RegData = DISEQC_RxControl_SET_RxFifoClear(RegData, DISEQC_CMD_NOP);
        DISEQC_RxControl_WRITE(pBc, RegData);

        // Wait for RX done interrupt
        Status = DtEventWaitUnInt(&pBc->m_RxDoneEvent, 5000);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, DISEQC, pBc,
                "ERROR: Wait for RX done failed (Status=0x%08X)", Status);
        }
        // Failed?
        if (pBc->m_Failure)
        {
            DtDbgOutBc(ERR, DISEQC, pBc, "LNB indicated RX failure");
            Status = DT_STATUS_FAIL;
        }

        // Check message size and copy data
        if (DT_SUCCESS(Status))
        {
            Int RxFifoLoad = (Int)DISEQC_RxStatus_READ_FifoLoad(pBc);
            if (RxFifoLoad <= *pNumRcvd)
            {
                // Read message
                for (i=0; i<RxFifoLoad; i++)
                    pMsgRcv[i] = (UInt8)DISEQC_RxData_READ(pBc);
                *pNumRcvd = RxFifoLoad;
            }
            else
                Status = DT_STATUS_BUF_TOO_SMALL;
        }
    }

    if (!DT_SUCCESS(Status) && NeedToReceive)
        *pNumRcvd = 0;

    // Make sure all interrupts are disabled
    DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_DISEQC_TXDONE);
    DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_DISEQC_RXDONE);
    DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_DISEQC_FAULT);

    // Release mutex
    DtFastMutexRelease(&pBc->m_AccessMutex);

    return Status;
}


// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDISEQC - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDISEQC_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcDISEQC_Init(DtBc*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity checks
    BC_DISEQC_DEFAULT_PRECONDITIONS(pBc);

    // Init interrupt DPC
    Status = DtDpcInit(&BC_DISEQC->m_IntDpc, DtBcDISEQC_InterruptDpc, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, DISEQC, pBc, 
                                     "ERROR: failed to init DPC (Status=0x%08X)", Status);
        return Status;
    }

    // Read config
    BC_DISEQC->m_TxFifoSize = DISEQC_Config_READ_TxFifoSize(BC_DISEQC);
    BC_DISEQC->m_RxFifoSize = DISEQC_Config_READ_RxFifoSize(BC_DISEQC);

    // Init mutex
    DtFastMutexInit(&BC_DISEQC->m_AccessMutex);

    // Init TX completion event
    Status = DtEventInit(&BC_DISEQC->m_TxDoneEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, DISEQC, pBc, 
                           "ERROR: failed to init tX done event (Status=0x%08X)", Status);
        return Status;
    }
    // Init completion event
    Status = DtEventInit(&BC_DISEQC->m_RxDoneEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, DISEQC, pBc, 
                           "ERROR: failed to init RX done event (Status=0x%08X)", Status);
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    Status = DtBcDISEQC_RegisterIntHandlers(BC_DISEQC);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, DISEQC, pBc, "ERROR: failed to register interrupt handler");
        return Status;
    }

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDISEQC_InterruptDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcDISEQC_InterruptDpc(DtDpcArgs * pArgs)
{
    DtBcDISEQC*  pBc = (DtBcDISEQC*)pArgs->m_pContext;
    Int Id = (Int)pArgs->m_Data1.m_UInt32_1;

    if (Id == DT_INTERRUPT_DISEQC_TXDONE)
    { 
        DtDbgOutBc(MAX, DISEQC, pBc, "Transfer completed");
        DtEventSet(&pBc->m_TxDoneEvent);
    }
    else if (Id == DT_INTERRUPT_DISEQC_RXDONE)
    { 
        DtDbgOutBc(MAX, DISEQC, pBc, "Receive completed");
        DtEventSet(&pBc->m_RxDoneEvent);
    }
    else if (Id == DT_INTERRUPT_DISEQC_FAULT)
    { 
        DtDbgOutBc(MIN, DISEQC, pBc, "Transfer failed");
        pBc->m_Failure = TRUE;
        DtEventSet(&pBc->m_TxDoneEvent);
        DtEventSet(&pBc->m_RxDoneEvent);
    }
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDISEQC_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcDISEQC_InterruptHandler(DtBc*  pBc, Int Id, Int Index, void*  pContext)
{
    DtStatus  Status=DT_STATUS_OK;
    DtDpcArgs  DpcArgs;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcDISEQC));

    // Check ID is one of ours
    if (Id!=DT_INTERRUPT_DISEQC_TXDONE && Id!=DT_INTERRUPT_DISEQC_RXDONE
                                                    && Id!=DT_INTERRUPT_DISEQC_FAULT)
    {
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Disable the DISEQC interrupt that was fired. 
    // NOTE: is re-enabled upon next transfer
    DtBc_InterruptDisable(pBc, Id);

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    DpcArgs.m_Data1.m_UInt32_2 = 0;
    
    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&((DtBcDISEQC*)pBc)->m_IntDpc, &DpcArgs);
    DT_ASSERT(DT_SUCCESS(Status));

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDISEQC_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDISEQC_RegisterIntHandlers(DtBcDISEQC* pBc)
{
    Int  i=0;
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_DISEQC_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks
    for (i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch (Id)
        {
        case DT_INTERRUPT_DISEQC_RXDONE:
        case DT_INTERRUPT_DISEQC_TXDONE:
        case DT_INTERRUPT_DISEQC_FAULT:
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id,
                                                       DtBcDISEQC_InterruptHandler, NULL);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, DISEQC, pBc,
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
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDISEQC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcDISEQC
#define DISEQC_STUB   ((DtIoStubBcDISEQC*)pStub)
#define DISEQC_BC  ((DtBcDISEQC*)DISEQC_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcDISEQC_AppendDynamicSize(const DtIoStub*, 
                                                                      DtIoStubIoParams *);
static DtStatus  DtIoStubBcDISEQC_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);

static DtStatus DtIoStubBcDISEQC_OnCmdEnableTone(
                                          const DtIoStubBcDISEQC* , 
                                          const DtIoctlDiseqcCmd_EnableToneInput*);

static DtStatus DtIoStubBcDISEQC_OnCmdSendToneBurst(
                                          const DtIoStubBcDISEQC* , 
                                          const DtIoctlDiseqcCmd_SendToneBurstInput*);

static DtStatus DtIoStubBcDISEQC_OnCmdSendMessage(
                                            const DtIoStubBcDISEQC* , 
                                            const DtIoctlDiseqcCmd_SendMessageInput*);

static DtStatus DtIoStubBcDISEQC_OnCmdSendReceiveMessage(
                                      const DtIoStubBcDISEQC* , 
                                      const DtIoctlDiseqcCmd_SendReceiveMessageInput*,
                                      DtIoctlDiseqcCmd_SendReceiveMessageOutput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_DISEQC;

static const DtIoctlProperties  IOSTUB_BC_DISEQC_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_DISEQC_CMD(
        DtIoStubBcDISEQC_OnCmd,
        DtIoStubBcDISEQC_AppendDynamicSize,
        NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDISEQC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDISEQC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcDISEQC_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcDISEQC));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDISEQC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcDISEQC*  DtIoStubBcDISEQC_Open(DtBc*  pBc)
{
    DtIoStubBcDISEQC*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcDISEQC));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcDISEQC, pBc, NULL, 
                                                           DtIoStubBcDISEQC_Close,
                                                           NULL,  // Use default IOCTL
                                                           IOSTUB_BC_DISEQC_IOCTLS);
    pStub = (DtIoStubBcDISEQC*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDISEQC- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+


// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDISEQC_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcDISEQC_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlDiseqcCmdInput*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcDISEQC));
    DT_ASSERT(pIoParams != NULL);                 
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_DISEQC_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_DiseqcCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_DISEQC_CMD_SEND_MESSAGE:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize 
                                        >= sizeof(DtIoctlDiseqcCmd_SendMessageInput));

        // Add dynamic size (i.e. #bytes to send)
        pIoParams->m_InReqSize += pInData->m_SndMsg.m_NumToSend;
        break;

    case DT_DISEQC_CMD_SEND_RECEIVE_MESSAGE:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize 
                                 >= sizeof(DtIoctlDiseqcCmd_SendReceiveMessageInput));
        DT_ASSERT(pIoParams->m_OutReqSize 
                                >= sizeof(DtIoctlDiseqcCmd_SendReceiveMessageOutput));

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

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDISEQC_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcDISEQC_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlDiseqcCmdInput*  pInData = NULL;
    DtIoctlDiseqcCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcDISEQC));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_DISEQC_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, DISEQC, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_DiseqcCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_DiseqcCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_DISEQC_CMD_ENABLE_TONE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcDISEQC_OnCmdEnableTone(DISEQC_STUB, &pInData->m_EnableTone);
        break;
    case DT_DISEQC_CMD_SEND_TONEBURST:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcDISEQC_OnCmdSendToneBurst(DISEQC_STUB,
                                                                &pInData->m_SndToneBurst);
        break;
    case DT_DISEQC_CMD_SEND_MESSAGE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcDISEQC_OnCmdSendMessage(DISEQC_STUB, &pInData->m_SndMsg);
        break;
    case DT_DISEQC_CMD_SEND_RECEIVE_MESSAGE:
        DT_ASSERT(pInData!=NULL && pOutData!=NULL);
        Status = DtIoStubBcDISEQC_OnCmdSendReceiveMessage(DISEQC_STUB,
                                           &pInData->m_SndRcvMsg, &pOutData->m_SndRcvMsg);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDISEQC_OnCmdEnableTone -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcDISEQC_OnCmdEnableTone(
    const DtIoStubBcDISEQC* pStub,
    const DtIoctlDiseqcCmd_EnableToneInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcDISEQC));
    DT_ASSERT(pInData!=NULL);

    return DtBcDISEQC_EnableTone(DISEQC_BC, pInData->m_EnableTone);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDISEQC_OnCmdSendToneBurst -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcDISEQC_OnCmdSendToneBurst(
    const DtIoStubBcDISEQC* pStub,
    const DtIoctlDiseqcCmd_SendToneBurstInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcDISEQC));
    DT_ASSERT(pInData!=NULL);

    return DtBcDISEQC_SendToneBurst(DISEQC_BC, pInData->m_ToneBurst);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDISEQC_OnCmdSendMessage -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcDISEQC_OnCmdSendMessage(
                                    const DtIoStubBcDISEQC*  pStub, 
                                    const DtIoctlDiseqcCmd_SendMessageInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcDISEQC));
    DT_ASSERT(pInData != NULL);

    return DtBcDISEQC_SendReceiveMessage(DISEQC_BC, pInData->m_Buf, 
                                                        pInData->m_NumToSend, NULL, NULL);

}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDISEQC_OnCmdSendReceiveMessage -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcDISEQC_OnCmdSendReceiveMessage(
                              const DtIoStubBcDISEQC*  pStub, 
                              const DtIoctlDiseqcCmd_SendReceiveMessageInput* pInData,
                              DtIoctlDiseqcCmd_SendReceiveMessageOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcDISEQC));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);


    pOutData->m_NumRcvd = pInData->m_RcvBufSize;
    return DtBcDISEQC_SendReceiveMessage(DISEQC_BC, pInData->m_Buf,
                             pInData->m_NumToSend, pOutData->m_Buf, &pOutData->m_NumRcvd);
}
