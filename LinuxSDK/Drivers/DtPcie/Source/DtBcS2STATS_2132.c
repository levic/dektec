//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcS2STATS_2132.c *#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
#include "DtBcS2STATS_2132.h"
#include "DtBcS2STATS_2132_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS2STATS_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_S2STATS_2132_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcS2STATS_2132))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_S2STATS_2132_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(S2STATS_2132, pBc)

// Helper macro to cast a DtBc* to a DtBcS2STATS_2132*
#define BC_S2STATS_2132         ((DtBcS2STATS_2132*)pBc)

DtStatus DtBcS2STATS_2132_SetOperationalMode(DtBcS2STATS_2132 * pBc, Int OpMode);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcS2STATS_2132_Init(DtBc*);
static DtStatus  DtBcS2STATS_2132_InterruptHandler(DtBc*, Int Id, Int Index, void*);
static void  DtBcS2STATS_2132_InterruptDpc(DtDpcArgs* pArgs);
static DtStatus  DtBcS2STATS_2132_OnEnable(DtBc*, Bool);
static DtStatus  DtBcS2STATS_2132_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcS2STATS_2132_RegisterIntHandlers(DtBcS2STATS_2132* pBc);
static void  DtBcS2STATS_2132_UpdateFrameStatus(DtBcS2STATS_2132 * pBc);
static void  DtBcS2STATS_2132_FrameStatusBuffer_Clear(
                                                     DtBcS2STATS_2132_FrameStatusBuffer*);
static Bool  DtBcS2STATS_2132_FrameStatusBuffer_GetOverflow(
                                                     DtBcS2STATS_2132_FrameStatusBuffer*);
static void  DtBcS2STATS_2132_FrameStatusBuffer_PopFront(
                                                      DtBcS2STATS_2132_FrameStatusBuffer*,
                                                      DtS2Stats_2132_FrameStatus*);
static void  DtBcS2STATS_2132_FrameStatusBuffer_PushBack(
                                                      DtBcS2STATS_2132_FrameStatusBuffer*,
                                                      DtS2Stats_2132_FrameStatus*);
static void  DtBcS2STATS_2132_FrameStatusBuffer_SetOverflow(
                                                     DtBcS2STATS_2132_FrameStatusBuffer*);
Int DtBcS2STATS_2132_FrameStatusBuffer_GetFree(DtBcS2STATS_2132_FrameStatusBuffer*);
Int DtBcS2STATS_2132_FrameStatusBuffer_GetLoad(DtBcS2STATS_2132_FrameStatusBuffer*);

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS2STATS_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcS2STATS_2132_Close(DtBc*  pBc)
{
    BC_S2STATS_2132_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcS2STATS_2132*  DtBcS2STATS_2132_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_S2STATS_2132_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcS2STATS_2132, Id, DT_BLOCK_TYPE_S2STATS_2132, 
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcS2STATS_2132_Close;
    OpenParams.m_InitFunc = DtBcS2STATS_2132_Init;
    OpenParams.m_OnEnableFunc = DtBcS2STATS_2132_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcS2STATS_2132_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcS2STATS_2132*)DtBc_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS2STATS_2132_GetOperationalMode(DtBcS2STATS_2132* pBc, Int* pOpMode)
{
    // Sanity check
    BC_S2STATS_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S2STATS_2132_MUST_BE_ENABLED(pBc);

    // Return last cached operational mode
    *pOpMode = pBc->m_OperationalMode;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS2STATS_2132_SetOperationalMode(DtBcS2STATS_2132* pBc, Int OpMode)
{
    // Sanity check
    BC_S2STATS_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, S2STATS_2132, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_S2STATS_2132_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // IDLE -> RUN
    if (OpMode == DT_BLOCK_OPMODE_RUN)
    {
        DtDbgOutBc(AVG, S2STATS_2132, pBc, "OpMode from IDLE -> RUN");

        // Clear statistics
        DtBcS2STATS_2132_ClearStatistics(pBc);
    }
    else
        // RUN->IDLE
        DtDbgOutBc(AVG, S2STATS_2132, pBc, "OpMode from RUN -> IDLE");


    // Get spinlock
    DtSpinLockAcquire(&pBc->m_Lock);

    if (OpMode == DT_BLOCK_OPMODE_RUN)
        // Enable the fifo-half-full interrupt
        DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_S2STATS_2132_FIFOHALFFULL);
    else
        // Disable the fifo-half-full interrupt
        DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_S2STATS_2132_FIFOHALFFULL);

    // Save new setting
    pBc->m_OperationalMode = OpMode;

    // Release spinlock
    DtSpinLockRelease(&pBc->m_Lock);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_ClearStatistics -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcS2STATS_2132_ClearStatistics(DtBcS2STATS_2132* pBc)
{
    UInt32 RegControl;

    // Sanity check
    BC_S2STATS_2132_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_S2STATS_2132_MUST_BE_ENABLED(pBc);

    // Get spinlock
    DtSpinLockAcquire(&pBc->m_Lock);

    // Clear buffered statistics
    DtBcS2STATS_2132_FrameStatusBuffer_Clear(&pBc->m_FrameStatusBuffer);

    // Clear hardware FIFO
    RegControl = S2STATS_Control_READ(pBc);
    RegControl = S2STATS_Control_SET_ClearFifo(RegControl, 1);
    S2STATS_Control_WRITE(pBc, RegControl);

    // Release spinlock
    DtSpinLockRelease(&pBc->m_Lock);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_GetStatisticsStatus -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcS2STATS_2132_GetStatisticsStatus(DtBcS2STATS_2132* pBc, Int* pBufferSize, 
                                                       Int* pNumAvailable, Int* pOverflow)
{
    DtBcS2STATS_2132_FrameStatusBuffer* pStatsBuf = &pBc->m_FrameStatusBuffer;
    // Sanity check
    BC_S2STATS_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pBufferSize==NULL|| pNumAvailable==NULL || pOverflow==NULL)
    { 
        DtDbgOutBc(ERR, S2STATS_2132, pBc, "Invalid parameter");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_S2STATS_2132_MUST_BE_ENABLED(pBc);

    // Read the latest frame statuses from the firmware
    DtBcS2STATS_2132_UpdateFrameStatus(pBc);

    // Get spinlock
    DtSpinLockAcquire(&pBc->m_Lock);

    // Maximum number that can be buffered is buffer size -1
    *pBufferSize = DT_BC_S2STATS_2132_FRAME_STATUS_BUF_SIZE-1;

    // Determine how many are available
    *pNumAvailable = DtBcS2STATS_2132_FrameStatusBuffer_GetLoad(pStatsBuf);
    
    // Get the overflow
    *pOverflow =  (Int)DtBcS2STATS_2132_FrameStatusBuffer_GetOverflow(pStatsBuf);

    // Release spinlock
    DtSpinLockRelease(&pBc->m_Lock);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_ReadStatistics -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS2STATS_2132_ReadStatistics(DtBcS2STATS_2132* pBc,
                    DtS2Stats_2132_FrameStatus* pStatistics, Int NumToRead, Int* pNumRead)
{
     DtBcS2STATS_2132_FrameStatusBuffer* pStatsBuf = &pBc->m_FrameStatusBuffer;
     Int NumAvailable, i;

    // Sanity check
    BC_S2STATS_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pStatistics==NULL|| pNumRead==NULL || NumToRead<0)
    { 
        DtDbgOutBc(ERR, S2STATS_2132, pBc, "Invalid parameter");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_S2STATS_2132_MUST_BE_ENABLED(pBc);

    // Read the latest frame statuses from the firmware
    DtBcS2STATS_2132_UpdateFrameStatus(pBc);

    // Get spinlock
    DtSpinLockAcquire(&pBc->m_Lock);

    // Determine how many we can read
    NumAvailable = DtBcS2STATS_2132_FrameStatusBuffer_GetLoad(pStatsBuf);
    *pNumRead = (NumToRead < NumAvailable) ? NumToRead : NumAvailable;
    
    // Read statistics
    for (i=0; i<*pNumRead; i++)
        DtBcS2STATS_2132_FrameStatusBuffer_PopFront(pStatsBuf, &pStatistics[i]);

    // Release spinlock
    DtSpinLockRelease(&pBc->m_Lock);
    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS2STATS_2132 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcS2STATS_2132_Init(DtBc*  pBcBase)
{
    DtBcS2STATS_2132* pBc = (DtBcS2STATS_2132*)pBcBase;
    DtStatus  Status=DT_STATUS_OK;
    UInt32 RegControl;

    // Sanity checks
    BC_S2STATS_2132_DEFAULT_PRECONDITIONS(pBc);

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    // Get configuration
    pBc->m_FifoSize = S2STATS_Configuration_READ_FifoSize(pBc);

    // Initialize the spin lock
    DtSpinLockInit(&pBc->m_Lock);

    // Clear frame status buffer
    DtBcS2STATS_2132_FrameStatusBuffer_Clear(&pBc->m_FrameStatusBuffer);

    // Clear hardware FIFO
    RegControl = S2STATS_Control_READ(pBc);
    RegControl = S2STATS_Control_SET_ClearFifo(RegControl, 1);
    S2STATS_Control_WRITE(pBc, RegControl);

    // Init interrupt DPC
    Status = DtDpcInit(&pBc->m_IntDpc, DtBcS2STATS_2132_InterruptDpc, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, S2STATS_2132, pBc, "ERROR: failed to init DPC (Status=0x%08X)", 
                                                                                  Status);
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-. 
    Status = DtBcS2STATS_2132_RegisterIntHandlers(pBc);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, S2STATS_2132, pBc, "Failed to register interrupt handlers");
        return Status;
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcS2STATS_2132_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcS2STATS_2132* pBc = (DtBcS2STATS_2132*)pBcBase;
    UInt32  RegControl;

    // Sanity check
    BC_S2STATS_2132_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE' set cached values and enable
        DtDbgOutBc(AVG, S2STATS_2132, pBc, "Block Enable from disable -> enable");

        // Clear frame status buffer
        DtBcS2STATS_2132_FrameStatusBuffer_Clear(&BC_S2STATS_2132->m_FrameStatusBuffer);
        // Clear hardware FIFO
        RegControl = S2STATS_Control_READ(pBc);
        RegControl = S2STATS_Control_SET_ClearFifo(RegControl, 1);
        S2STATS_Control_WRITE(pBc, RegControl);
    }
    else
    {
        // ENABLE -> DISABLE' disable and set defaults
        DtDbgOutBc(AVG, S2STATS_2132, pBc, "Block Enable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcS2STATS_2132_SetOperationalMode(BC_S2STATS_2132,
                                                                    DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DT_ASSERT(FALSE);
            DtDbgOutBc(ERR, S2STATS_2132, pBc, "Failed to set opmode to idle");
        }  
    }
    // Save new setting
    pBc->m_BlockEnabled = Enable;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcS2STATS_2132_InterruptHandler(DtBc* pBc, Int Id, Int Index, void* pContext)
{
    DtStatus  Status=DT_STATUS_OK;
    DtDpcArgs  DpcArgs;

    BC_S2STATS_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check ID is one of ours
    if (Id != DT_INTERRUPT_S2STATS_2132_FIFOHALFFULL)
    {
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Disable the interrupt that was fired. 
    // NOTE: is re-enabled afte all data is handled by the DPC
    DtBc_InterruptDisable(pBc, Id);

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    DpcArgs.m_Data1.m_UInt32_2 = 0;
    
    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&((DtBcS2STATS_2132*)pBc)->m_IntDpc, &DpcArgs);
    DT_ASSERT(DT_SUCCESS(Status));

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_InterruptDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcS2STATS_2132_InterruptDpc(DtDpcArgs* pArgs)
{
    DtBcS2STATS_2132*  pBc = (DtBcS2STATS_2132*)pArgs->m_pContext;
    DtDbgOutBc(MAX, S2STATS_2132, pBc, "S2Stats fifo half full");

    // Read frame statuses
    DtBcS2STATS_2132_UpdateFrameStatus(pBc);

    // Get spinlock
    DtSpinLockAcquire(&pBc->m_Lock);

    // Check if we need to re-enable the fifo-half-full interrupt
    if (pBc->m_OperationalMode==DT_BLOCK_OPMODE_RUN && DtBc_IsEnabled((DtBc*)pBc))
        DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_S2STATS_2132_FIFOHALFFULL);

    // Release spinlock
    DtSpinLockRelease(&pBc->m_Lock);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcS2STATS_2132_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_S2STATS_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutBc(AVG, S2STATS_2132, pBc, "Set operational mode to IDLE");

        // Set operational mode to IDLE
        Status = DtBcS2STATS_2132_SetOperationalMode(BC_S2STATS_2132, 
                                                                    DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, S2STATS_2132, pBc, "ERROR: failed to set opmode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcS2STATS_2132_RegisterIntHandlers(DtBcS2STATS_2132*  pBc)
{
    Int  i=0;
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_S2STATS_2132_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch(Id)
        {
        case DT_INTERRUPT_S2STATS_2132_FIFOHALFFULL:
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id,
                                                 DtBcS2STATS_2132_InterruptHandler, NULL);
            if(!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, S2STATS_2132, pBc,
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


//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_UpdateFrameStatus -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtBcS2STATS_2132_UpdateFrameStatus(DtBcS2STATS_2132* pBc)
{
    DtBcS2STATS_2132_FrameStatusBuffer* pStatsBuf = &pBc->m_FrameStatusBuffer;
    DtS2Stats_2132_FrameStatus FrameStatus;
    Int FifoLoad, NumFree, NumToCopy, i;

    // Get spinlock
    DtSpinLockAcquire(&pBc->m_Lock);

    // Determine howmany we can write
    NumFree =  DtBcS2STATS_2132_FrameStatusBuffer_GetFree(pStatsBuf);
    
    // Read howmany are availalbe in the firmware
    FifoLoad  = (Int)S2STATS_Status_READ_FifoLoad(pBc);

    // Check overflow conditions
    if (FifoLoad == pBc->m_FifoSize)
    {
        DtDbgOutBc(MIN, S2STATS_2132, pBc, "Fifo overflow detected");
        DtBcS2STATS_2132_FrameStatusBuffer_SetOverflow(pStatsBuf);
    }
    if (FifoLoad > NumFree)
    {
        DtDbgOutBc(MIN, S2STATS_2132, pBc, "Buffer overflow detected");
        DtBcS2STATS_2132_FrameStatusBuffer_SetOverflow(pStatsBuf);
    }
    
    NumToCopy = (FifoLoad < NumFree) ? FifoLoad : NumFree;

    // TT#3279: Workaround for invalid last item. When receiving 2 different streams, the
    // last framestat in the buffer sometimes contains a PLS-value from the first stream.
    // Probably the FIFO-counter is wrong (1 too big): If the last item is not read, 
    // the next read normally would deliver the skipped last faulty item, but it doesn't.
    // TODO: Remove workaround when the firmware is fixed!
    if(NumToCopy>0)
        NumToCopy--;
    for (i=0; i<NumToCopy; i++)
    {
        // Read Frame status
        UInt32 RegData = S2STATS_FrameStatus_READ(pBc);
        // Get the status fields
        FrameStatus.m_Pls = (UInt8)S2STATS_FrameStatus_GET_PLS(RegData);
        FrameStatus.m_InputStreamId = 
                                    (UInt8)S2STATS_FrameStatus_GET_InputStreamId(RegData);
        FrameStatus.m_Mer = (Int)S2STATS_FrameStatus_GET_MER(RegData);
        FrameStatus.m_IsBchOk = S2STATS_FrameStatus_GET_IsBchOk(RegData)==S2STATS_CMD_OK
                                                                           ? TRUE : FALSE;
        FrameStatus.m_IsLdpcOk = S2STATS_FrameStatus_GET_IsLdpcOk(RegData)==S2STATS_CMD_OK
                                                                           ? TRUE : FALSE;
        FrameStatus.m_CcmAcm = S2STATS_FrameStatus_GET_CcmAcm(RegData)==S2STATS_CMD_CCM ?
                                                DT_S2STATS_2132_CCM : DT_S2STATS_2132_ACM;

        // Add status to the frame status buffer
        DtBcS2STATS_2132_FrameStatusBuffer_PushBack(pStatsBuf, &FrameStatus);
    }
    
    // Release spinlock
    DtSpinLockRelease(&pBc->m_Lock);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_FrameStatusBuffer_Clear -.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcS2STATS_2132_FrameStatusBuffer_Clear(
    DtBcS2STATS_2132_FrameStatusBuffer* pStatsBuf)
{
    pStatsBuf->m_ReadIdx = pStatsBuf->m_WriteIdx =  0;
    pStatsBuf->m_Overflow = FALSE;
}

//.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_FrameStatusBuffer_GetOverflow -.-.-.-.-.-.-.-.-.-.
//
Bool DtBcS2STATS_2132_FrameStatusBuffer_GetOverflow(
    DtBcS2STATS_2132_FrameStatusBuffer* pStatsBuf)
{
    return pStatsBuf->m_Overflow;
}

//.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_FrameStatusBuffer_SetOverflow -.-.-.-.-.-.-.-.-.-.
//
void DtBcS2STATS_2132_FrameStatusBuffer_SetOverflow(
    DtBcS2STATS_2132_FrameStatusBuffer* pStatsBuf)
{
    pStatsBuf->m_Overflow = TRUE;
}

//-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_FrameStatusBuffer_PopFront -.-.-.-.-.-.-.-.-.-.-.
//
void DtBcS2STATS_2132_FrameStatusBuffer_PopFront(
    DtBcS2STATS_2132_FrameStatusBuffer* pStatsBuf,
    DtS2Stats_2132_FrameStatus* pStat)
{
    Int  RdIdx = pStatsBuf->m_ReadIdx;
#ifdef DEBUG
    Int  WrIdx = pStatsBuf->m_WriteIdx;
    // Load should have been checked before calling this function
    DT_ASSERT(RdIdx != WrIdx);
#endif  // #ifdef DEBUG

    // Get element and increment read index
    *pStat = pStatsBuf->m_Statistics[RdIdx++];
    if (RdIdx >= DT_BC_S2STATS_2132_FRAME_STATUS_BUF_SIZE)
        RdIdx = 0;
    pStatsBuf->m_ReadIdx = RdIdx;
}

//-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_FrameStatusBuffer_PushBack -.-.-.-.-.-.-.-.-.-.-.
//
void DtBcS2STATS_2132_FrameStatusBuffer_PushBack(
    DtBcS2STATS_2132_FrameStatusBuffer* pStatsBuf,
    DtS2Stats_2132_FrameStatus* pStat)
{
    Int  WrIdx = pStatsBuf->m_WriteIdx;
    
    // Load should have been checked before calling this function
    DT_ASSERT((DtBcS2STATS_2132_FrameStatusBuffer_GetFree(pStatsBuf) > 0));
    
    // Add element and increment write index
    pStatsBuf->m_Statistics[WrIdx++] = *pStat;
    if (WrIdx >= DT_BC_S2STATS_2132_FRAME_STATUS_BUF_SIZE)
        WrIdx = 0;
    pStatsBuf->m_WriteIdx = WrIdx;
}

//.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_FrameStatusBuffer_GetFree -.-.-.-.-.-.-.-.-.-.-.
//
Int DtBcS2STATS_2132_FrameStatusBuffer_GetFree(
    DtBcS2STATS_2132_FrameStatusBuffer* pStatsBuf)
{
    // Get number of elements that can be written. Maximum number is one less than the 
    // FIFO size.
    return (DT_BC_S2STATS_2132_FRAME_STATUS_BUF_SIZE -1) -  
                                    DtBcS2STATS_2132_FrameStatusBuffer_GetLoad(pStatsBuf);
}

//.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132_FrameStatusBuffer_GetLoad -.-.-.-.-.-.-.-.-.-.-.
//
Int DtBcS2STATS_2132_FrameStatusBuffer_GetLoad(
    DtBcS2STATS_2132_FrameStatusBuffer* pStatsBuf)
{
    Int  RdIdx = pStatsBuf->m_ReadIdx;
    Int  WrIdx = pStatsBuf->m_WriteIdx;
    return (WrIdx >= RdIdx) ? (WrIdx - RdIdx) 
                            : DT_BC_S2STATS_2132_FRAME_STATUS_BUF_SIZE - (RdIdx - WrIdx);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcS2STATS_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcS2STATS_2132
#define S2STATS_2132_STUB   ((DtIoStubBcS2STATS_2132*)pStub)
#define S2STATS_2132_BC  ((DtBcS2STATS_2132*)S2STATS_2132_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcS2STATS_2132_AppendDynamicSize(const DtIoStub*,
                                                                       DtIoStubIoParams*);
static DtStatus  DtIoStubBcS2STATS_2132_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcS2STATS_2132_OnCmdClearStatistics(
                                                          const DtIoStubBcS2STATS_2132*);
static DtStatus  DtIoStubBcS2STATS_2132_OnCmdGetOperationalMode(
                                                  const DtIoStubBcS2STATS_2132*, 
                                                  DtIoctlS2StatsCmd_2132GetOpModeOutput*);
static DtStatus  DtIoStubBcS2STATS_2132_OnCmdGetStatisticsStatus(
                                        const DtIoStubBcS2STATS_2132*, 
                                        DtIoctlS2StatsCmd_2132GetStatisticsStatusOutput*);
static DtStatus  DtIoStubBcS2STATS_2132_OnCmdReadStatistics(
                                        const DtIoStubBcS2STATS_2132*, 
                                        const DtIoctlS2StatsCmd_2132ReadStatisticsInput*,
                                        DtIoctlS2StatsCmd_2132ReadStatisticsOutput*);
static DtStatus  DtIoStubBcS2STATS_2132_OnCmdSetOperationalMode(
                                             const DtIoStubBcS2STATS_2132*, 
                                             const DtIoctlS2StatsCmd_2132SetOpModeInput*);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_S2STATS_2132;

static const DtIoctlProperties  IOSTUB_BC_S2STATS_2132_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_S2STATS_CMD_2132(
        DtIoStubBcS2STATS_2132_OnCmd,
        DtIoStubBcS2STATS_2132_AppendDynamicSize,
        NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcS2STATS_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS2STATS_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcS2STATS_2132_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcS2STATS_2132));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS2STATS_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcS2STATS_2132*  DtIoStubBcS2STATS_2132_Open(DtBc*  pBc)
{
    DtIoStubBcS2STATS_2132*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcS2STATS_2132));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcS2STATS_2132, pBc, NULL, 
                                                           DtIoStubBcS2STATS_2132_Close,
                                                           NULL,  // Use default IOCTL
                                                           IOSTUB_BC_S2STATS_2132_IOCTLS);
    pStub = (DtIoStubBcS2STATS_2132*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcS2STATS_2132- Private functions +=+=+=+=+=+=+=+=+=+=+=+



//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS2STATS_2132_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcS2STATS_2132_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlS2StatsCmdInput_2132*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcS2STATS_2132));
    DT_ASSERT(pIoParams!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_S2STATS_CMD_2132);
    
    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_S2StatsCmd_2132;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_S2STATS_CMD_2132_READ_STATISTICS:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize 
                                    >= sizeof(DtIoctlS2StatsCmd_2132ReadStatisticsInput));
        DT_ASSERT(pIoParams->m_OutReqSize 
                                   >= sizeof(DtIoctlS2StatsCmd_2132ReadStatisticsOutput));
        // Add dynamic size (i.e. #bytes to read)
        pIoParams->m_OutReqSize += (pInData->m_ReadStats.m_NumToRead 
                                                    * sizeof(DtS2Stats_2132_FrameStatus));
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS2STATS_2132_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcS2STATS_2132_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlS2StatsCmdInput_2132*  pInData = NULL;
    DtIoctlS2StatsCmdOutput_2132*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcS2STATS_2132));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_S2STATS_CMD_2132);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, S2STATS_2132, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_S2StatsCmd_2132;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_S2StatsCmd_2132;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_S2STATS_CMD_2132_CLEAR_STATISTICS:
        Status = DtIoStubBcS2STATS_2132_OnCmdClearStatistics(S2STATS_2132_STUB);
        break;
    case DT_S2STATS_CMD_2132_GET_STATISTICS_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcS2STATS_2132_OnCmdGetStatisticsStatus(S2STATS_2132_STUB,  
                                                                  &pOutData->m_GetStatus);
        break;
    case DT_S2STATS_CMD_2132_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcS2STATS_2132_OnCmdGetOperationalMode(S2STATS_2132_STUB,  
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_S2STATS_CMD_2132_READ_STATISTICS:
        DT_ASSERT(pInData != NULL);
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcS2STATS_2132_OnCmdReadStatistics(S2STATS_2132_STUB, 
                                          &pInData->m_ReadStats,  &pOutData->m_ReadStats);
        break;
    case DT_S2STATS_CMD_2132_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcS2STATS_2132_OnCmdSetOperationalMode(S2STATS_2132_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS2STATS_2132_OnCmdClearStatistics -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcS2STATS_2132_OnCmdClearStatistics(const DtIoStubBcS2STATS_2132* pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcS2STATS_2132));
    return DtBcS2STATS_2132_ClearStatistics(S2STATS_2132_BC);
}

//.-.-.-.-.-.-.-.-.-.- DtIoStubBcS2STATS_2132_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcS2STATS_2132_OnCmdGetOperationalMode(
    const DtIoStubBcS2STATS_2132* pStub,
    DtIoctlS2StatsCmd_2132GetOpModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcS2STATS_2132));
    DT_ASSERT(pOutData != NULL);

    return DtBcS2STATS_2132_GetOperationalMode(S2STATS_2132_BC, &pOutData->m_OpMode);
}

//-.-.-.-.-.-.-.-.-.- DtIoStubBcS2STATS_2132_OnCmdGetStatisticsStatus -.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcS2STATS_2132_OnCmdGetStatisticsStatus(
    const DtIoStubBcS2STATS_2132* pStub,
    DtIoctlS2StatsCmd_2132GetStatisticsStatusOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcS2STATS_2132));
    DT_ASSERT(pOutData != NULL);
    return DtBcS2STATS_2132_GetStatisticsStatus(S2STATS_2132_BC, &pOutData->m_BufferSize,
                                        &pOutData->m_NumAvailable, &pOutData->m_Overflow);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS2STATS_2132_OnCmdReadStatistics -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcS2STATS_2132_OnCmdReadStatistics(
    const DtIoStubBcS2STATS_2132* pStub,
    const DtIoctlS2StatsCmd_2132ReadStatisticsInput* pInData,
    DtIoctlS2StatsCmd_2132ReadStatisticsOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcS2STATS_2132));
    DT_ASSERT(pInData != NULL);
    DT_ASSERT(pOutData != NULL);
    return DtBcS2STATS_2132_ReadStatistics(S2STATS_2132_BC, pOutData->m_Buf,
                                              pInData->m_NumToRead, &pOutData->m_NumRead);
}

//.-.-.-.-.-.-.-.-.-.- DtIoStubBcS2STATS_2132_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcS2STATS_2132_OnCmdSetOperationalMode(
    const DtIoStubBcS2STATS_2132* pStub,
    const DtIoctlS2StatsCmd_2132SetOpModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcS2STATS_2132));
    DT_ASSERT(pInData != NULL);

    return DtBcS2STATS_2132_SetOperationalMode(S2STATS_2132_BC, pInData->m_OpMode);
}
