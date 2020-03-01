//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcCDMAC.c *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
#include "DtBc.h"
#include "DtBcCDMAC.h"
#include "DtBcCDMAC_RegAccess.h"
#include "DtDma.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcCDMAC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

#define CDMAC_PAGE_SIZE         4096
#define CDMAC_DMA_MAX_TRANSFER_SIZE (128*1024*1024)

// MACRO with default precondition checks for the Bc
#define BC_CDMAC_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcCDMAC))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_CDMAC_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(CDMAC, pBc)

// Helper macro to cast a DtBc* to a DtBcCDMAC*
#define BC_CDMAC         ((DtBcCDMAC*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcCDMAC_Init(DtBc*);
static void  DtBcCDMAC_SetControlRegs(DtBcCDMAC*, Bool BlkEnable, Int OpMode, 
                                          Int Direction, Int TestMode, Bool ChannelFlush);
static DtStatus  DtBcCDMAC_OnEnable(DtBc*, Bool  Enable);
static DtStatus  DtBcCDMAC_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcCDMAC_PrepareBuffers(DtBcCDMAC*, UInt8*  pDmaBuffer, 
                                                         Int  DmaBufferSize, Int  RxOrTx);
static void  DtBcCDMAC_PeriodicIntervalHandler(DtObject*, DtTodTime  Time);
static void  DtBcCDMAC_AddMeasurement(DtBcCDMACProfiling*, DtTodTime Time, 
                                               UInt NumTransferred, UInt64 TransferSpeed);
static void  DtBcCDMAC_ClearMeasurement(DtBcCDMACProfiling*, UInt64 Threshold);
static void  DtBcCDMAC_GetMeasurement(DtBcCDMACProfiling*, UInt64* pMax, UInt64* pMin, 
                                                                            UInt64* pAvg);
static void  DtBcCDMAC_InitMeasurement(DtBcCDMACProfiling*);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcCDMAC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcCDMAC_Close(DtBc*  pBc)
{
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    //Unregister periodic interrupt handler
    DtCore_TOD_PeriodicItvUnregister(pBc->m_pCore, (DtObject*)pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcCDMAC*  DtBcCDMAC_Open(Int  Address, DtCore*  pCore, DtPt*  pPt,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_CDMAC_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcCDMAC, Id, DT_BLOCK_TYPE_CDMAC, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcCDMAC_Close;
    OpenParams.m_InitFunc = DtBcCDMAC_Init;
    OpenParams.m_OnEnableFunc = DtBcCDMAC_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcCDMAC_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcCDMAC*)DtBc_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_AllocateBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCDMAC_AllocateBuffer(DtBcCDMAC* pBc, Int Direction, UInt8 * pDmaBuffer,
                                                                        Int DmaBufferSize)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check    
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    // Direction must be RX or TS
    if (Direction!=DT_CDMAC_DIR_RX && Direction!=DT_CDMAC_DIR_TX)
        return DT_STATUS_INVALID_PARAMETER;
    // DMA buffer must start at page boudary
    if (pDmaBuffer==NULL || (((UIntPtr)pDmaBuffer) & (CDMAC_PAGE_SIZE-1))!=0)
        return DT_STATUS_INVALID_PARAMETER;
    // Size must be multiple of page size * prefetch size
    if (DmaBufferSize<=0 || (DmaBufferSize%(CDMAC_PAGE_SIZE*pBc->m_PrefetchSize))!=0)
        return DT_STATUS_INVALID_PARAMETER;
    // Check capability
    if (Direction==DT_CDMAC_DIR_RX && (pBc->m_Capabilities&DT_CDMAC_CAP_RX)==0)
        return DT_STATUS_NOT_SUPPORTED;
    if (Direction==DT_CDMAC_DIR_TX && (pBc->m_Capabilities&DT_CDMAC_CAP_TX)==0)
        return DT_STATUS_NOT_SUPPORTED;

    // Must be enabled
    BC_CDMAC_MUST_BE_ENABLED(pBc);

    // Prevent mutual access to CDMAC
    DtFastMutexAcquire(&pBc->m_DmaMutex);
    
    // Check current state
    if (pBc->m_pDmaBuffer != NULL)
    {
        // Buffers are already allocated
        DtDbgOutBc(ERR, CDMAC, pBc, "ERROR: DMA buffer already allocated");
        DtFastMutexRelease(&pBc->m_DmaMutex);
        return DT_STATUS_IN_USE;
    }

    // Set direction
    DtBcCDMAC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, Direction, 
                                                                  pBc->m_TestMode, FALSE);
    pBc->m_Direction = Direction;

    // Prepare DMA buffer and Pointer buffer
    Status = DtBcCDMAC_PrepareBuffers(pBc, pDmaBuffer, DmaBufferSize, Direction);

    DtFastMutexRelease(&pBc->m_DmaMutex);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_ClearProfiling -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCDMAC_ClearProfiling(DtBcCDMAC* pBc)
{
    // Sanity check    
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_CDMAC_MUST_BE_ENABLED(pBc);

    // Clear current profiling data
     DtBcCDMAC_ClearMeasurement(&pBc->m_DmaProfiling, pBc->m_ReorderBufSize);

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_ClearReorderBufMinMax -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCDMAC_ClearReorderBufMinMax(DtBcCDMAC* pBc)
{
    UInt32 RegData = 0;

    // Sanity check    
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_CDMAC_MUST_BE_ENABLED(pBc);

    // Clear minimum/maximum reorder buffer foad
    RegData = CDMAC_ReorderBufLoadMinMax_SET_Clear(0, 1);
    CDMAC_ReorderBufLoadMinMax_WRITE(pBc, RegData);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_FreeBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCDMAC_FreeBuffer(DtBcCDMAC* pBc)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  DmaDirection;

    // Sanity check    
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_CDMAC_MUST_BE_ENABLED(pBc);

    // Prevent mutual access to CDMAC
    DtFastMutexAcquire(&pBc->m_DmaMutex);

    // Must be in idle
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
    { 
        DtDbgOutBc(ERR, CDMAC, pBc, "ERROR: Not in idle");
        DtFastMutexRelease(&pBc->m_DmaMutex);
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // Already freed?
    if (pBc->m_pDmaBuffer == NULL)
    {
        DtDbgOutBc(AVG, CDMAC, pBc, "Already freed");
        DtFastMutexRelease(&pBc->m_DmaMutex);
        return DT_STATUS_OK;
    }

    // Unlock the DMA buffer and clear the pagelist
    DtUnlockUserBuffer(&pBc->m_DmaBufferPageList);
    DtDeletePageList(&pBc->m_DmaBufferPageList);
    pBc->m_pDmaBuffer = NULL;
    pBc->m_DmaBufferSize = 0;

    // Delete scatter gather list
    DmaDirection = (pBc->m_Direction==DT_CDMAC_DIR_RX) ? DT_DMA_DIRECTION_FROM_DEVICE
                                                       : DT_DMA_DIRECTION_TO_DEVICE;
    DtDmaFreeSgList(&pBc->m_pCore->m_Device, &pBc->m_OsSgList, DmaDirection);

    DtFastMutexRelease(&pBc->m_DmaMutex);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_GetReorderBufStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcCDMAC_GetReorderBufStatus(DtBcCDMAC* pBc, Int* pBufLoad,
                                                                      Int* pBufMinMaxLoad)
{
    // Sanity check    
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pBufLoad ==NULL || pBufMinMaxLoad ==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CDMAC_MUST_BE_ENABLED(pBc);
    
    // Read reorder buffer load registers
    *pBufLoad = (Int)CDMAC_ReorderBufLoad_READ(pBc);
    *pBufMinMaxLoad = (Int)CDMAC_ReorderBufLoadMinMax_READ_LoadMinMax(pBc);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_GetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCDMAC_GetProfiling(DtBcCDMAC* pBc, UInt64* pMaxSpeed, UInt64* pMinSpeed,
                                                    UInt64* pAvgSpeed, UInt32* pPerfCount)
{
    // Sanity check    
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pMaxSpeed==NULL || pMinSpeed==NULL || pAvgSpeed==NULL || pPerfCount==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CDMAC_MUST_BE_ENABLED(pBc);

    // If in testmode get profiling data
    if (pBc->m_TestMode != DT_CDMAC_TESTMODE_NORMAL)
        DtBcCDMAC_GetMeasurement(&pBc->m_DmaProfiling, pMaxSpeed, pMinSpeed, pAvgSpeed);
    else
        *pMaxSpeed = *pMinSpeed =*pAvgSpeed = 0;

 
    // Read performance counter
    *pPerfCount = (Int)CDMAC_Status_READ_PerformanceCnt(pBc);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_GetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCDMAC_GetProperties(DtBcCDMAC* pBc, UInt32* pCapabilities, 
                        Int* pPrefetchSize, Int* pPcieItfDataWidth,  Int* pReorderBufSize)
{
    // Sanity check    
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pCapabilities==NULL || pPrefetchSize==NULL || pPcieItfDataWidth==NULL 
                                                               || pReorderBufSize ==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return properties
    *pCapabilities = pBc->m_Capabilities;
    *pPrefetchSize = pBc->m_PrefetchSize;
    *pPcieItfDataWidth = pBc->m_PcieItfDataWidth;
    *pReorderBufSize = pBc->m_ReorderBufSize;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_GetRxWriteOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCDMAC_GetRxWriteOffset(DtBcCDMAC* pBc, UInt* pRxWrOffset)
{
    // Sanity check    
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pRxWrOffset == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Must be enabled
    BC_CDMAC_MUST_BE_ENABLED(pBc);

    // return current DMA RX write offset
    *pRxWrOffset = CDMAC_RxWriteOffset_READ(pBc);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_GetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCDMAC_GetStatus(DtBcCDMAC* pBc, Int* pOpStatus,
                                               UInt32* pStatusFlags,  UInt32* pErrorFlags)
{
    UInt FwStatus;
    UInt FwErrorStatus;

    // Sanity check    
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpStatus==NULL || pStatusFlags==NULL || pErrorFlags==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CDMAC_MUST_BE_ENABLED(pBc);

    // Get CMDAC status and error status
    FwStatus = CDMAC_Status_READ(pBc);
    FwErrorStatus = CDMAC_ErrorStatus_READ(pBc);
 
    // Convert to driver type
    switch (CDMAC_Status_GET_OperationalStatus(FwStatus))
    {
    case CDMAC_OPSTATUS_IDLE:  *pOpStatus = DT_BLOCK_OPSTATUS_IDLE; break;
    case CDMAC_OPSTATUS_RUN:   *pOpStatus = DT_BLOCK_OPSTATUS_RUN; break;
    case CDMAC_OPSTATUS_ERROR: *pOpStatus = DT_BLOCK_OPSTATUS_ERROR; break;
    default: DT_ASSERT(FALSE); return DT_STATUS_FAIL;
    }

    // Return status flags
    *pStatusFlags = 0;
    if (CDMAC_Status_GET_WaitPcie(FwStatus) != 0)
        *pStatusFlags |= DT_CDMAC_STATUS_WAIT_PCIE;
    if (CDMAC_Status_GET_WaitLocal(FwStatus) != 0)
        *pStatusFlags |= DT_CDMAC_STATUS_WAIT_LOCAL;

    // Return error flags
    *pErrorFlags = 0;
    if (CDMAC_ErrorStatus_GET_AlignError(FwErrorStatus) != 0)
        *pErrorFlags |= DT_CDMAC_ERROR_ALIGN;
    if (CDMAC_ErrorStatus_GET_EndTimeout(FwErrorStatus) != 0)
        *pErrorFlags |= DT_CDMAC_ERROR_TIMEOUT;
    if (CDMAC_ErrorStatus_GET_EndUndefined(FwErrorStatus) != 0)
        *pErrorFlags |= DT_CDMAC_ERROR_UNDEFINED;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_GetTxReadOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCDMAC_GetTxReadOffset(DtBcCDMAC* pBc, UInt* pTxRdOffset)
{
    // Sanity check    
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pTxRdOffset == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CDMAC_MUST_BE_ENABLED(pBc);

    // return current DMA TX read offset
    *pTxRdOffset = CDMAC_TxReadOffset_READ(pBc);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_IssueChannelFlush -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCDMAC_IssueChannelFlush(DtBcCDMAC* pBc)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check    
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_CDMAC_MUST_BE_ENABLED(pBc);

    // Must be in IDLE
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
    {
        DtDbgOutBc(ERR, CDMAC, pBc, "OpMode not IDLE");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // Issue ChannelFlush
    DtBcCDMAC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                 pBc->m_Direction, pBc->m_TestMode, TRUE);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCDMAC_SetOperationalMode(DtBcCDMAC* pBc, Int OpMode)
{
     DtStatus  Status = DT_STATUS_OK;
     Int TimeoutCount=0;
     UInt32 ExpectFwOpStatus=0, FwOpStatus=0;

    // Sanity check    
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);
    
    // Must be enabled
    BC_CDMAC_MUST_BE_ENABLED(pBc);
    
    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Prevent mutual access to CDMAC
    DtFastMutexAcquire(&pBc->m_DmaMutex);

    // Check opmode transition
    if (OpMode == pBc->m_OperationalMode)
    {
        DtFastMutexRelease(&pBc->m_DmaMutex);

        // No change but from RUN -> RUN => already in use
        if (OpMode == DT_BLOCK_OPMODE_RUN)
        {
            DtDbgOutBc(ERR, CDMAC, pBc, "ERROR: Cannot change OpMode");
            return DT_STATUS_IN_USE;
        }
        else
            return DT_STATUS_OK;
    }

    // From IDLE/STANDBY -> RUN only allowed if buffers are allocated
    if ((OpMode==DT_BLOCK_OPMODE_STANDBY || OpMode==DT_BLOCK_OPMODE_RUN) 
                                                               && pBc->m_pDmaBuffer==NULL)
    {
        DtDbgOutBc(ERR, CDMAC, pBc, "ERROR: Cannot change OpMode");
        DtFastMutexRelease(&pBc->m_DmaMutex);
        return DT_STATUS_NOT_INITIALISED;
    }

    // Update control registers
    DtBcCDMAC_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode, pBc->m_Direction, 
                                                                  pBc->m_TestMode, FALSE);
    // Save new block enable and operational mode
    pBc->m_OperationalMode = OpMode;

    // Clear profiling data when going to RUN mode
    if (OpMode == DT_BLOCK_OPMODE_RUN)
    {
        BC_CDMAC->m_PrevTransferOffset  = 0;
        DtBcCDMAC_ClearMeasurement(&BC_CDMAC->m_DmaProfiling,  pBc->m_ReorderBufSize);
    }

    // Determine the expected operational status  
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    ExpectFwOpStatus = CDMAC_OPSTATUS_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: ExpectFwOpStatus = CDMAC_OPSTATUS_RUN; break;
    case DT_BLOCK_OPMODE_RUN:     ExpectFwOpStatus = CDMAC_OPSTATUS_RUN; break;
    default:                      DT_ASSERT(FALSE); break;
    }

    // Wait till the DMAC has reached the requested operational mode  
    FwOpStatus = CDMAC_Status_READ_OperationalStatus(pBc);
    for (TimeoutCount=100; TimeoutCount>0 && FwOpStatus!=ExpectFwOpStatus; TimeoutCount--)
    {
        DtWaitBlock(10);
        FwOpStatus = CDMAC_Status_READ_OperationalStatus(pBc);
    }
    if (FwOpStatus == ExpectFwOpStatus)
        DtDbgOutBc(AVG, CDMAC, pBc, "Entered new operational mode");
    else if (FwOpStatus == CDMAC_OPSTATUS_ERROR)
    { 
        DtDbgOutBc(ERR, CDMAC, pBc, "ERROR: failed to set new OpMode");
        Status = DT_STATUS_FAIL;
    }
    else
    { 
        DtDbgOutBc(ERR, CDMAC, pBc, "ERROR: timeout set new OpMode");
        Status = DT_STATUS_TIMEOUT;
    }
    DtFastMutexRelease(&pBc->m_DmaMutex);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_SetRxReadOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCDMAC_SetRxReadOffset(DtBcCDMAC* pBc, UInt RxRdOffset)
{
    // Sanity check    
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (RxRdOffset!=0 && RxRdOffset>=(UInt)pBc->m_DmaBufferSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Check direction
    if (pBc->m_Direction != DT_CDMAC_DIR_RX)
        return DT_STATUS_NOT_SUPPORTED;

    // Must be enabled
    BC_CDMAC_MUST_BE_ENABLED(pBc);
    
    // Set DMA RX read offset
    CDMAC_RxReadOffset_WRITE(pBc, RxRdOffset);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_SetTestMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCDMAC_SetTestMode(DtBcCDMAC* pBc, Int TestMode)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check    
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (TestMode!=DT_CDMAC_TESTMODE_NORMAL && TestMode!=DT_CDMAC_TESTMODE_TEST_INT
                                                  && TestMode!=DT_CDMAC_TESTMODE_TEST_EXT)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CDMAC_MUST_BE_ENABLED(pBc);
    
    // Prevent mutual access to CDMAC
    DtFastMutexAcquire(&pBc->m_DmaMutex);

    // Must in idle
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        Status = DT_STATUS_INVALID_IN_OPMODE;
    else
    {
        // Set testmode
        pBc->m_TestMode = TestMode;
        // Clear current profiling data
        DtBcCDMAC_ClearMeasurement(&pBc->m_DmaProfiling,  pBc->m_ReorderBufSize);
    }
    
    // Check current state
    DtFastMutexRelease(&pBc->m_DmaMutex);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_SetTxWriteOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCDMAC_SetTxWriteOffset(DtBcCDMAC* pBc, UInt TxWrOffset)
{
    // Sanity check
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (TxWrOffset!=0 && TxWrOffset>=(UInt)pBc->m_DmaBufferSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Check direction
    if (pBc->m_Direction != DT_CDMAC_DIR_TX)
        return DT_STATUS_NOT_SUPPORTED;

    // Must be enabled
    BC_CDMAC_MUST_BE_ENABLED(pBc);
    
    // Set DMA TX write offset
    CDMAC_TxWriteOffset_WRITE(pBc, TxWrOffset);

    return DT_STATUS_OK;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcCDMAC - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcCDMAC_Init(DtBc*  pBcBase)
{
    DtBcCDMAC* pBc = (DtBcCDMAC*)pBcBase;
    DtStatus  Status = DT_STATUS_OK;
    UInt32 RegConfig1;
    DtOnPeriodicIntervalRegData  RegData;

    // Sanity check    
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Read configuration registers
    RegConfig1 = CDMAC_Config1_READ(pBc);

    // Get RX/TX capabilities
    pBc->m_Capabilities  = 0;
    if (CDMAC_Config1_GET_RxTxCapability(RegConfig1) == CDMAC_RXTXCAP_RX_ONLY)
        pBc->m_Capabilities  = DT_CDMAC_CAP_RX;
    else if (CDMAC_Config1_GET_RxTxCapability(RegConfig1) == CDMAC_RXTXCAP_TX_ONLY)
        pBc->m_Capabilities = DT_CDMAC_CAP_TX;
    else if (CDMAC_Config1_GET_RxTxCapability(RegConfig1) == CDMAC_RXTXCAP_RX_OR_TX)
        pBc->m_Capabilities  = DT_CDMAC_CAP_RX | DT_CDMAC_CAP_TX;


    // Get prefetch size
    pBc->m_PrefetchSize = CDMAC_Config1_GET_PrefetchSize(RegConfig1);

    // Get PCIe interface data width
    pBc->m_PcieItfDataWidth = CDMAC_Config1_GET_PcieItfDataWidth(RegConfig1);

    // Get reorder buffer size
    pBc->m_ReorderBufSize = CDMAC_Config2_READ_ReorderBufSize(pBc);
      
    // Initialize fast mutex
    DtFastMutexInit(&pBc->m_DmaMutex);

    // Initialize structures to 0
    DtMemZero(&pBc->m_DmaDevice, sizeof(pBc->m_DmaDevice));
    DtMemZero(&pBc->m_PtrTable, sizeof(pBc->m_PtrTable));
    DtMemZero(&pBc->m_DmaBufferPageList, sizeof(pBc->m_DmaBufferPageList));
    
    // Initialize cached settings
    pBc->m_pDmaBuffer = NULL;
    pBc->m_DmaBufferSize = 0;
    pBc->m_PtrTable.m_Allocated = FALSE;
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    if ((pBc->m_Capabilities & DT_CDMAC_CAP_RX) != 0)
        pBc->m_Direction = DT_CDMAC_DIR_RX;
    else
        pBc->m_Direction = DT_CDMAC_DIR_TX;
    pBc->m_TestMode = DT_CDMAC_TESTMODE_NORMAL;

    // Update control registers
    DtBcCDMAC_SetControlRegs(pBc, pBc->m_BlockEnabled,
                                        pBc->m_OperationalMode,pBc->m_Direction, 
                                        pBc->m_TestMode, FALSE);

    // Initialize profiling data
    DtBcCDMAC_InitMeasurement(&pBc->m_DmaProfiling);
    
    // Register periodic interval handler
    RegData.m_OnPeriodicFunc = DtBcCDMAC_PeriodicIntervalHandler;
    RegData.m_pObject = (DtObject*)pBc;
    Status = DtCore_TOD_PeriodicItvRegister(pBc->m_pCore, &RegData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, CDMAC, pBc, "Failed to initialize");
        return Status;
    }

    // Initialise the DMA adapter
    Status = DtDmaInit(&pBc->m_pCore->m_Device, &pBc->m_DmaDevice, 
                                                       CDMAC_DMA_MAX_TRANSFER_SIZE, TRUE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, CDMAC, pBc, "Failed to initialize");
        return Status;
    }

    DtDbgOutBc(AVG, CDMAC, pBc, "Initialized successfully");

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCDMAC_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcCDMAC* pBc = (DtBcCDMAC*)pBcBase;
    
    // Sanity check
    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);


    if (Enable)
    {
        // DISABLE-> ENABLE
        // No special action needed
    }
    else
    {
        // ENABLE -> DISABLE' set operational mode to IDLE and disable
        Status = DtBcCDMAC_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, CDMAC, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
        // Free DMA pointer table 
        Status = DtDmaFreeDirectBuffer(&pBc->m_pCore->m_Device, &BC_CDMAC->m_PtrTable);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, CDMAC, pBc, "ERROR: DtDmaFreeDirectBuffer failed");
            DT_ASSERT(FALSE);
        }
        pBc->m_PtrTable.m_Allocated = FALSE;
    }

    // Set default operational mode, direction and testmode in cache
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    if ((pBc->m_Capabilities & DT_CDMAC_CAP_RX) != 0)
        pBc->m_Direction = DT_CDMAC_DIR_RX;
    else
        pBc->m_Direction = DT_CDMAC_DIR_TX;
    pBc->m_TestMode = DT_CDMAC_TESTMODE_NORMAL;

    // Set block enable, operational mode, direction and testmode
    DtBcCDMAC_SetControlRegs(pBc, Enable, pBc->m_OperationalMode,
                                                pBc->m_Direction, pBc->m_TestMode, FALSE);
    pBc->m_BlockEnabled = Enable;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcCDMAC_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_CDMAC_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled(pBc))
    {
        DtDbgOutBc(AVG, CDMAC, pBc, "Go to IDLE and free buffers");

        // Go to IDLE
        Status = DtBcCDMAC_SetOperationalMode(BC_CDMAC, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, CDMAC, pBc, "ERROR: failed to set operational mode");
        }
        // And free allocated buffers
        Status = DtBcCDMAC_FreeBuffer(BC_CDMAC);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, CDMAC, pBc, "ERROR: failed to free buffers");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_PrepareBuffers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcCDMAC_PrepareBuffers(
    DtBcCDMAC* pBc,
    UInt8*  pDmaBuffer, 
    Int  DmaBufferSize,
    Int   RxOrTx)
{
    DtStatus  Status;
    UInt  PtrTableBufSize;
    UInt64* pPtrTable;
    UInt  NumSgls, SglIdx, TblIdx, NumTblEntries;
    UInt DmaDirection;

    // Just to be sure
    DT_ASSERT(pBc->m_pDmaBuffer == NULL);
    DT_ASSERT(BC_CDMAC->m_PrefetchSize > 0);
    DT_ASSERT(DmaBufferSize%(BC_CDMAC->m_PrefetchSize*CDMAC_PAGE_SIZE) == 0);

    // Compute the required size of the DMA pointer table
    NumTblEntries = DmaBufferSize/CDMAC_PAGE_SIZE;
    PtrTableBufSize = NumTblEntries*sizeof(UInt64);

    // Check if we can reuse the DMA pointer table buffer
    if (BC_CDMAC->m_PtrTable.m_Allocated
                                   && BC_CDMAC->m_PtrTable.m_BufferLength<PtrTableBufSize)
    {
        // Buffer cannot be reused, release current DMA pointer table 
        DtDmaFreeDirectBuffer(&pBc->m_pCore->m_Device, &BC_CDMAC->m_PtrTable);
        BC_CDMAC->m_PtrTable.m_Allocated = FALSE;
    }

    // Check if we have to allocate a new DMA pointer table buffer 
    if (!BC_CDMAC->m_PtrTable.m_Allocated)
    { 
        // Create a new DMA pointer table, it will be destroyed on Close()
        Status = DtDmaCreateDirectBuffer(&pBc->m_pCore->m_Device, &BC_CDMAC->m_DmaDevice,
                                                   &BC_CDMAC->m_PtrTable, PtrTableBufSize,
                                                   BC_CDMAC->m_PrefetchSize*8);

        if (DT_SUCCESS(Status))
            DtDbgOutBc(AVG, CDMAC, pBc, "Created new DMA pointer table buffer");
        else
        {
            DtDbgOutBc(ERR, CDMAC, pBc, "ERROR: failed to create pointer table buffer");
            return Status;
        }

        // Set DMA pointer table entry
        CDMAC_PointerTableAddrLow_WRITE(BC_CDMAC,
                                          BC_CDMAC->m_PtrTable.m_PhysicalAddress.LowPart); 
        CDMAC_PointerTableAddrHigh_WRITE(BC_CDMAC,
                                         BC_CDMAC->m_PtrTable.m_PhysicalAddress.HighPart); 
    }

    // Create a pagelist for the shared buffer
    Status = DtCreatePageList(pDmaBuffer, DmaBufferSize, DT_BUFTYPE_USER,
                                                               &pBc->m_DmaBufferPageList);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, CDMAC, pBc, "ERROR: failed to create page list");
        return Status;
    }

    // Lock buffer into kernel memory
    Status = DtLockUserBuffer(&pBc->m_DmaBufferPageList, pDmaBuffer);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, CDMAC, pBc, "ERROR: failed to lock user buffer");
        DtDeletePageList(&pBc->m_DmaBufferPageList);
        return Status;
    }
       
    // Store DMA buffer pointer and size
    pBc->m_pDmaBuffer = pBc->m_DmaBufferPageList.m_pVirtualKernel;
    pBc->m_DmaBufferSize = DmaBufferSize;
       
    // Set DMA buffer size
    CDMAC_BufferSize_WRITE(pBc, DmaBufferSize);

    // Create SGList from buffer
    DmaDirection = (RxOrTx==DT_CDMAC_DIR_RX) ? DT_DMA_DIRECTION_FROM_DEVICE
                                             : DT_DMA_DIRECTION_TO_DEVICE;
    DtMemZero(&pBc->m_OsSgList, sizeof(&pBc->m_OsSgList));
    pBc->m_OsSgList.m_PageList = pBc->m_DmaBufferPageList;
    Status = DtDmaCreateSgList(&pBc->m_pCore->m_Device, &pBc->m_DmaDevice, pDmaBuffer,
                                           DmaBufferSize, DmaDirection, &pBc->m_OsSgList);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, CDMAC, pBc, "ERROR: failed to create SG list");
        // Failed; Unlock the DMA buffer and clear the pagelist
        DtUnlockUserBuffer(&pBc->m_DmaBufferPageList);
        DtDeletePageList(&pBc->m_DmaBufferPageList);
        pBc->m_pDmaBuffer = NULL;
        pBc->m_DmaBufferSize = 0;
        return Status;
    }
   
    // Initialize the pointer table
    pPtrTable =  (UInt64*)pBc->m_PtrTable.m_pVirtualAddress;
    TblIdx = 0;
    NumSgls = DtDmaOsSglGetNumElements(&pBc->m_OsSgList);
    for (SglIdx=0; SglIdx<NumSgls && TblIdx<NumTblEntries; SglIdx++)
    {
        DtPhysicalAddress PciAddress = DtDmaOsSglGetAddress(&pBc->m_OsSgList, SglIdx);
        UInt Length = DtDmaOsSglGetLength(&pBc->m_OsSgList, SglIdx);
        UInt i=0;
        DT_ASSERT((Length%CDMAC_PAGE_SIZE) == 0);
        for (i=0; i<Length/CDMAC_PAGE_SIZE; i++)
        {
            DT_ASSERT((PciAddress.QuadPart%CDMAC_PAGE_SIZE) == 0);
            pPtrTable[TblIdx++] = PciAddress.QuadPart;

            PciAddress.QuadPart += CDMAC_PAGE_SIZE;
        }
    }

    DT_ASSERT(SglIdx==NumSgls && TblIdx==NumTblEntries);

    DtDbgOutBc(AVG, CDMAC, pBc, "Successfully prepared DMA buffers");
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcCDMAC_SetControlRegs(DtBcCDMAC* pBc, Bool BlkEnable, Int OpMode, 
                                           Int Direction, Int TestMode, Bool ChannelFlush)
{
    UInt32  RegData=0, FldBlkEnable=0, FldOpMode=0, FldDirection=0, FldTestMode=0;

    // Convert to BB types
    FldBlkEnable =  BlkEnable ? CDMAC_BLKENA_ENABLED : CDMAC_BLKENA_DISABLED;
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FldOpMode = CDMAC_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FldOpMode = CDMAC_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FldOpMode = CDMAC_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE); break;
    }
    FldDirection = (Direction==DT_CDMAC_DIR_RX) ? CDMAC_DIR_RX : CDMAC_DIR_TX;
    switch (TestMode)
    {
    case DT_CDMAC_TESTMODE_NORMAL:   FldTestMode = CDMAC_TEST_NORMAL; break;
    case DT_CDMAC_TESTMODE_TEST_INT: FldTestMode = CDMAC_TEST_INTERNAL; break;
    case DT_CDMAC_TESTMODE_TEST_EXT: FldTestMode = CDMAC_TEST_EXTERNAL; break;
    default: DT_ASSERT(FALSE); break;
    }

    // Set control 
    RegData = CDMAC_Control_READ(pBc);
    RegData = CDMAC_Control_SET_BlockEnable(RegData, FldBlkEnable);
    RegData = CDMAC_Control_SET_OperationalMode(RegData, FldOpMode);
    RegData = CDMAC_Control_SET_RxOrTx(RegData, FldDirection);
    RegData = CDMAC_Control_SET_TestMode(RegData, FldTestMode);
    RegData = CDMAC_Control_SET_ChannelFlush(RegData, ChannelFlush ? 1 : 0);
    CDMAC_Control_WRITE(pBc, RegData);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_PeriodicIntervalHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcCDMAC_PeriodicIntervalHandler(DtObject* pObj, DtTodTime Time)
{
    UInt TransferOffset, NumTransferred;
    UInt64 TransferSpeed;
    DtBcCDMAC*  pBc = (DtBcCDMAC*)pObj;
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcCDMAC));

    // Must be enabled
    if (!DtBc_IsEnabled((DtBc*)pBc))
        return;

    // Must be in RUN mode
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
        return;

    // Get transfer offset
    if (pBc->m_Direction == DT_CDMAC_DIR_RX)
        TransferOffset =  CDMAC_RxWriteOffset_READ(pBc);
    else
        TransferOffset =  CDMAC_TxReadOffset_READ(pBc);

    // Get current time
    DtCore_TOD_GetTime(pBc->m_pCore, &Time);

    // Compute number transferred
    if (TransferOffset >= pBc->m_PrevTransferOffset)
        NumTransferred = TransferOffset - pBc->m_PrevTransferOffset;
    else
        NumTransferred = TransferOffset + pBc->m_DmaBufferSize 
                                                              - pBc->m_PrevTransferOffset;
    pBc->m_PrevTransferOffset = TransferOffset;

    // Compute current transferspeed in bits per second
    TransferSpeed = CDMAC_Status_READ_PerformanceCnt(pBc);
    TransferSpeed *= (20 * pBc->m_PcieItfDataWidth);
    // Update bitrate measurement
    DtBcCDMAC_AddMeasurement(&pBc->m_DmaProfiling, Time, NumTransferred, TransferSpeed);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_AddMeasurement -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcCDMAC_AddMeasurement(DtBcCDMACProfiling* pProfiling, DtTodTime Time, 
                                               UInt NumTransferred,  UInt64 TransferSpeed)
{
    UInt64  Duration;
    UInt64  TimeNs = Time.m_Seconds*1000*1000*1000LL + Time.m_Nanoseconds;

    DT_ASSERT(pProfiling != NULL);

    DtSpinLockAcquireAtDpc(&pProfiling->m_PerfSpinLock);
    if (!pProfiling->m_ProfilingStarted)
    {
        pProfiling->m_TotalTransferred += NumTransferred;
        if (pProfiling->m_TotalTransferred > pProfiling->m_NumTransferredThreshold)
        {
            // Store first sample
            pProfiling->m_TotalTransferred = 0;
            pProfiling->m_FirstTimeNs = TimeNs;
            pProfiling->m_ProfilingStarted = TRUE;
        }
        DtSpinLockReleaseFromDpc(&pProfiling->m_PerfSpinLock);
        return;
    }

    // Time period
    Duration = (TimeNs - pProfiling->m_FirstTimeNs);

    // Update total, max and min transfer speed
    pProfiling->m_TotalTime = Duration;
    pProfiling->m_TotalTransferred += NumTransferred;

    // Discard first 50ms
    if (Duration > 50*1000*1000LL)
    {
        if (pProfiling->m_MaxTransferSpeed < TransferSpeed)
            pProfiling->m_MaxTransferSpeed = TransferSpeed;
        if (pProfiling->m_MinTransferSpeed > TransferSpeed)
            pProfiling->m_MinTransferSpeed = TransferSpeed;
    }
    DtSpinLockReleaseFromDpc(&pProfiling->m_PerfSpinLock);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_ClearMeasurement -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtBcCDMAC_ClearMeasurement(DtBcCDMACProfiling* pProfiling, UInt64 Threshold)
{
    DT_ASSERT(pProfiling != NULL);
    DtSpinLockAcquire(&pProfiling->m_PerfSpinLock);
    pProfiling->m_TotalTime = 0;
    pProfiling->m_TotalTransferred = 0;
    pProfiling->m_MaxTransferSpeed = 0;
    pProfiling->m_MinTransferSpeed = ~0;
    pProfiling->m_FirstTimeNs = 0;
    pProfiling->m_NumTransferredThreshold =  Threshold;
    pProfiling->m_ProfilingStarted = FALSE;
    DtSpinLockRelease(&pProfiling->m_PerfSpinLock);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_GetMeasurement -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcCDMAC_GetMeasurement(DtBcCDMACProfiling* pProfiling, UInt64* pMax, 
                                                              UInt64* pMin,  UInt64* pAvg)
{
    DT_ASSERT(pProfiling!=NULL && pMax!=NULL && pMin!=NULL && pAvg!=NULL);
    DtSpinLockAcquire(&pProfiling->m_PerfSpinLock);
    *pMax = pProfiling->m_MaxTransferSpeed;
    *pMin = pProfiling->m_MinTransferSpeed;
    // Compute average
    if (pProfiling->m_TotalTime == 0)
        *pAvg = 0;
    else if (pProfiling->m_TotalTime > 1000*1000*1000*1000LL)
    {
        UInt64 TimeSec = DtDivide64(pProfiling->m_TotalTime,1000*1000*1000LL, NULL);
        *pAvg = DtDivide64(pProfiling->m_TotalTransferred*8, TimeSec, NULL);
    }
    else if (pProfiling->m_TotalTime > 1000*1000*1000LL)
    {
        UInt64 TimeMs = DtDivide64(pProfiling->m_TotalTime,1000*1000LL, NULL);
        *pAvg = DtDivide64(pProfiling->m_TotalTransferred*8*1000LL, TimeMs, NULL);
    }
    else
    {
        *pAvg = DtDivide64(pProfiling->m_TotalTransferred*8*1000*1000*1000LL,
                                                           pProfiling->m_TotalTime, NULL);
    }
    DtSpinLockRelease(&pProfiling->m_PerfSpinLock);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCDMAC_InitMeasurement -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcCDMAC_InitMeasurement(DtBcCDMACProfiling* pProfiling)
{
    DT_ASSERT(pProfiling != NULL);
    DtSpinLockInit(&pProfiling->m_PerfSpinLock);
    DtBcCDMAC_ClearMeasurement(pProfiling, 0);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcCDMAC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcCDMAC
#define CDMAC_STUB   ((DtIoStubBcCDMAC*)pStub)
#define CDMAC_BC  ((DtBcCDMAC*)CDMAC_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

static DtStatus  DtIoStubBcCDMAC_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
static DtStatus  DtIoStubBcCDMAC_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcCDMAC_OnCmdAllocateBuffer(const DtIoStubBcCDMAC*, 
                                                const DtIoctlCDmaCCmdAllocateBufferInput*,
                                                UInt8* pBuffer, Int BufSize);
static DtStatus  DtIoStubBcCDMAC_OnCmdClearReorderBufMinMax(const DtIoStubBcCDMAC*);
static DtStatus  DtIoStubBcCDMAC_OnCmdClearProfiling(const DtIoStubBcCDMAC*);
static DtStatus  DtIoStubBcCDMAC_OnCmdFreeBuffer(const DtIoStubBcCDMAC*);
static DtStatus  DtIoStubBcCDMAC_OnCmdGetReorderBufStatus(const DtIoStubBcCDMAC*,
                                                DtIoctlCDmaCCmdGetReorderBufStatusOutput*);
static DtStatus  DtIoStubBcCDMAC_OnCmdGetProfiling(const DtIoStubBcCDMAC*,
                                                      DtIoctlCDmaCCmdGetProfilingOutput*);
static DtStatus  DtIoStubBcCDMAC_OnCmdGetProperties(const DtIoStubBcCDMAC*,
                                                DtIoctlCDmaCCmdGetPropertiesOutput*);
static DtStatus  DtIoStubBcCDMAC_OnCmdGetRxWriteOffset(const DtIoStubBcCDMAC*, 
                                                     DtIoctlCDmaCCmdGetRxWrOffsetOutput*);
static DtStatus  DtIoStubBcCDMAC_OnCmdGetStatus(const DtIoStubBcCDMAC*, 
                                                         DtIoctlCDmaCCmdGetStatusOutput*);
static DtStatus  DtIoStubBcCDMAC_OnCmdGetTxReadOffset(const DtIoStubBcCDMAC*, 
                                                     DtIoctlCDmaCCmdGetTxRdOffsetOutput*);
static DtStatus  DtIoStubBcCDMAC_OnCmdIssueChannelFlush(const DtIoStubBcCDMAC*);
static DtStatus  DtIoStubBcCDMAC_OnCmdSetOperationalMode(const DtIoStubBcCDMAC*, 
                                                    const DtIoctlCDmaCCmdSetOpModeInput*);
static DtStatus  DtIoStubBcCDMAC_OnCmdSetRxReadOffset(const DtIoStubBcCDMAC*,
                                                const DtIoctlCDmaCCmdSetRxRdOffsetInput*);
static DtStatus  DtIoStubBcCDMAC_OnCmdSetTestMode(const DtIoStubBcCDMAC*, 
                                                  const DtIoctlCDmaCCmdSetTestModeInput*);
static DtStatus  DtIoStubBcCDMAC_OnCmdSetTxWriteOffset(const DtIoStubBcCDMAC*, 
                                                const DtIoctlCDmaCCmdSetTxWrOffsetInput*);
static DtStatus  DtIoStubBcCDMAC_OnExclAccessCmd(const DtIoStub*, DtIoStubIoParams*,
                                                                          Int*  pOutSize);
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_CDMAC;

static const DtIoctlProperties  IOSTUB_BC_CDMAC_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBcCDMAC_OnExclAccessCmd,
        NULL, NULL),
    DT_IOCTL_PROPS_CDMAC_CMD(
        DtIoStubBcCDMAC_OnCmd,
        DtIoStubBcCDMAC_AppendDynamicSize,
        NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcCDMAC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcCDMAC_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));
    
    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcCDMAC*  DtIoStubBcCDMAC_Open(DtBc*  pBc)
{
    DtIoStubBcCDMAC*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcCDMAC));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcCDMAC, pBc, NULL, 
                                                             DtIoStubBcCDMAC_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_BC_CDMAC_IOCTLS);
    pStub = (DtIoStubBcCDMAC*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcCDMAC- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCDMAC_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlCDmaCCmdInput*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));
    DT_ASSERT(pIoParams!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_CDMAC_CMD);
    
    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, CDMAC, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_CDmaCCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_CDMAC_CMD_ALLOCATE_BUFFER:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlCDmaCCmdAllocateBufferInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlCDmaCCmdAllocateBufferOutput));
        // Add dynamic size (i.e. #bytes to read)
#ifdef WINBUILD
        // Windows specific
        pIoParams->m_OutReqSize += pInData->m_AllocBuf.m_BufferSize -
                                             sizeof (DtIoctlCDmaCCmdAllocateBufferOutput);
#else
        // Linux specific
        pIoParams->m_OutReqSize += 0;
#endif
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCDMAC_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlCDmaCCmdInput*  pInData = NULL;
    DtIoctlCDmaCCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_CDMAC_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, CDMAC, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_CDmaCCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_CDmaCCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_CDMAC_CMD_ALLOCATE_BUFFER:
    {
        UInt8*  pDmaBuffer;
        UInt  DmaBufferSize;
#if defined(WINBUILD)
        PMDL  pMdl;
        NTSTATUS  NtStatus;
        // Retrieve MDL and virtual buffer from request object
        NtStatus = WdfRequestRetrieveOutputWdmMdl(pIoParams->m_pIoctl->m_WdfRequest,
                                                                                   &pMdl);
        if (NtStatus != STATUS_SUCCESS)
        {
            DtDbgOutIoStubBc(ERR, CDMAC, pStub, "ERROR: WdfRequestRetrieveOutputWdmMdl "
                                                           "error code:  %08x", NtStatus);
            Status = DT_STATUS_OUT_OF_RESOURCES;
        }
        if (DT_SUCCESS(Status))
        {
            pDmaBuffer = MmGetMdlVirtualAddress(pMdl);
            if (pDmaBuffer == NULL)
                Status = DT_STATUS_OUT_OF_MEMORY;
            DmaBufferSize = MmGetMdlByteCount(pMdl);
        }
#else // LINBUILD
        DmaBufferSize = (UInt)pInData->m_AllocBuf.m_BufferSize;
#if defined(LIN32)
        pDmaBuffer = (char*)(UInt32)pInData->m_AllocBuf.m_BufferAddr;
#else
        pDmaBuffer = (char*)(UInt64)pInData->m_AllocBuf.m_BufferAddr;
#endif
#endif
        if (DT_SUCCESS(Status))
            Status = DtIoStubBcCDMAC_OnCmdAllocateBuffer(CDMAC_STUB, &pInData->m_AllocBuf,
                                                               pDmaBuffer, DmaBufferSize);
    }
    break;
    case DT_CDMAC_CMD_CLEAR_REORDER_BUF_MIN_MAX:
        Status = DtIoStubBcCDMAC_OnCmdClearReorderBufMinMax(CDMAC_STUB);
        break;
    case DT_CDMAC_CMD_CLEAR_PROFILING:
        DT_ASSERT(pOutData!=NULL);
        Status = DtIoStubBcCDMAC_OnCmdClearProfiling(CDMAC_STUB);
        break;
    case DT_CDMAC_CMD_FREE_BUFFER:
        Status = DtIoStubBcCDMAC_OnCmdFreeBuffer(CDMAC_STUB);
        break;
    case DT_CDMAC_CMD_GET_REORDER_BUF_STATUS:
        DT_ASSERT(pOutData!=NULL);
        Status = DtIoStubBcCDMAC_OnCmdGetReorderBufStatus(CDMAC_STUB, 
                                                               &pOutData->m_GetBufStatus);
        break;
    case DT_CDMAC_CMD_GET_PROFILING:
        DT_ASSERT(pOutData!=NULL);
        Status = DtIoStubBcCDMAC_OnCmdGetProfiling(CDMAC_STUB, &pOutData->m_GetProfiling);
        break;
    case DT_CDMAC_CMD_GET_PROPERTIES:
        DT_ASSERT(pOutData!=NULL);
        Status = DtIoStubBcCDMAC_OnCmdGetProperties(CDMAC_STUB, 
                                                              &pOutData->m_GetProperties);
        break;
    case DT_CDMAC_CMD_GET_RX_WRITE_OFFSET:
        DT_ASSERT(pOutData!=NULL);
        Status = DtIoStubBcCDMAC_OnCmdGetRxWriteOffset(CDMAC_STUB, 
                                                              &pOutData->m_GetRxWrOffset);
        break;
    case DT_CDMAC_CMD_GET_STATUS:
        DT_ASSERT(pOutData!=NULL);
        Status = DtIoStubBcCDMAC_OnCmdGetStatus(CDMAC_STUB, &pOutData->m_GetStatus);
        break;
    case DT_CDMAC_CMD_GET_TX_READ_OFFSET:
        DT_ASSERT(pOutData!=NULL);
        Status = DtIoStubBcCDMAC_OnCmdGetTxReadOffset(CDMAC_STUB, 
                                                              &pOutData->m_GetTxRdOffset);
        break;
    case DT_CDMAC_CMD_ISSUE_CHANNEL_FLUSH:
        Status = DtIoStubBcCDMAC_OnCmdIssueChannelFlush(CDMAC_STUB);
        break;
    case DT_CDMAC_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData!=NULL);
        Status = DtIoStubBcCDMAC_OnCmdSetOperationalMode(CDMAC_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_CDMAC_CMD_SET_RX_READ_OFFSET:
        DT_ASSERT(pInData!=NULL);
        Status = DtIoStubBcCDMAC_OnCmdSetRxReadOffset(CDMAC_STUB, 
                                                               &pInData->m_SetRxRdOffset);
        break;
    case DT_CDMAC_CMD_SET_TEST_MODE:
        DT_ASSERT(pInData!=NULL);
        Status = DtIoStubBcCDMAC_OnCmdSetTestMode(CDMAC_STUB, &pInData->m_SetTestMode);
        break;
    case DT_CDMAC_CMD_SET_TX_WRITE_OFFSET:
        DT_ASSERT(pInData!=NULL);
        Status = DtIoStubBcCDMAC_OnCmdSetTxWriteOffset(CDMAC_STUB,
                                                               &pInData->m_SetTxWrOffset);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmdAllocateBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCDMAC_OnCmdAllocateBuffer(
    const DtIoStubBcCDMAC* pStub,
    const DtIoctlCDmaCCmdAllocateBufferInput* pInData,
    UInt8* pBuffer, Int BufSize)
{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));
    DT_ASSERT(pInData!=NULL && pBuffer!=NULL);

    // Allocate buffer
    Status = DtBcCDMAC_AllocateBuffer(CDMAC_BC, pInData->m_Direction, pBuffer, BufSize);

    return  Status;
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmdClearReorderBufMinMax -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcCDMAC_OnCmdClearReorderBufMinMax(
    const DtIoStubBcCDMAC*  pStub)
{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));

    // Clear burst fifo min/max
    Status = DtBcCDMAC_ClearReorderBufMinMax(CDMAC_BC);

    return  Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmdClearProfiling -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcCDMAC_OnCmdClearProfiling(
    const DtIoStubBcCDMAC* pStub)
{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));

    // Clear Profiling measurement
    Status = DtBcCDMAC_ClearProfiling(CDMAC_BC);
    return  Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmdFreeBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcCDMAC_OnCmdFreeBuffer(
    const DtIoStubBcCDMAC*  pStub)
{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));

    // Free DMA buffers
    Status = DtBcCDMAC_FreeBuffer(CDMAC_BC);

    return  Status;
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmdGetReorderBufStatus -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCDMAC_OnCmdGetReorderBufStatus(
    const DtIoStubBcCDMAC* pStub,
    DtIoctlCDmaCCmdGetReorderBufStatusOutput* pOutData)

{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));
    DT_ASSERT(pOutData != NULL);

    // Get fifo status
    Status = DtBcCDMAC_GetReorderBufStatus(CDMAC_BC, &pOutData->m_ReorderBufLoad,
                                                       &pOutData->m_ReorderBufMinMaxLoad);
    return  Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmdGetProfiling -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcCDMAC_OnCmdGetProfiling(
    const DtIoStubBcCDMAC* pStub,
    DtIoctlCDmaCCmdGetProfilingOutput* pOutData)
{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));
    DT_ASSERT(pOutData != NULL);

    // Get Profiling measurement
    Status = DtBcCDMAC_GetProfiling(CDMAC_BC, &pOutData->m_MaxSpeed, 
                                             &pOutData->m_MinSpeed, &pOutData->m_AvgSpeed,
                                             &pOutData->m_PerfCount);
    return  Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmdGetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcCDMAC_OnCmdGetProperties(
    const DtIoStubBcCDMAC* pStub,
    DtIoctlCDmaCCmdGetPropertiesOutput* pOutData)
{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));
    DT_ASSERT(pOutData != NULL);

    // Get properties
    Status = DtBcCDMAC_GetProperties(CDMAC_BC, &pOutData->m_Capabilities, 
                                    &pOutData->m_PrefetchSize, &pOutData->m_PcieDataWidth,
                                    &pOutData->m_ReorderBufSize);
    return  Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmdGetRxWriteOffset -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcCDMAC_OnCmdGetRxWriteOffset(
    const DtIoStubBcCDMAC* pStub,
    DtIoctlCDmaCCmdGetRxWrOffsetOutput* pOutData)

{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));
    DT_ASSERT(pOutData != NULL);

    // Get RX write offset
    Status = DtBcCDMAC_GetRxWriteOffset(CDMAC_BC, &pOutData->m_RxWriteOffset);
    return  Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmdGetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcCDMAC_OnCmdGetStatus(
    const DtIoStubBcCDMAC* pStub,
    DtIoctlCDmaCCmdGetStatusOutput* pOutData)

{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));
    DT_ASSERT(pOutData != NULL);

    // Get status
    Status = DtBcCDMAC_GetStatus(CDMAC_BC, &pOutData->m_OpStatus, 
                                      &pOutData->m_StatusFlags,  &pOutData->m_ErrorFlags);
    return  Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmdGetTxReadOffset -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcCDMAC_OnCmdGetTxReadOffset(
    const DtIoStubBcCDMAC* pStub,
    DtIoctlCDmaCCmdGetTxRdOffsetOutput* pOutData)

{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));
    DT_ASSERT(pOutData != NULL);

    // Get TX read offset
    Status = DtBcCDMAC_GetTxReadOffset(CDMAC_BC, &pOutData->m_TxReadOffset);
    return  Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmdIssueChannelFlush -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcCDMAC_OnCmdIssueChannelFlush(
    const DtIoStubBcCDMAC* pStub)
{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));

    // Issue channel reset
    Status = DtBcCDMAC_IssueChannelFlush(CDMAC_BC);
    return  Status;
}


//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcCDMAC_OnCmdSetOperationalMode(
    const DtIoStubBcCDMAC* pStub,
    const DtIoctlCDmaCCmdSetOpModeInput* pInData)
{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));
    DT_ASSERT(pInData!=NULL);

    // Set operational mode
    Status = DtBcCDMAC_SetOperationalMode(CDMAC_BC, pInData->m_OpMode);

    return  Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmdSetRxReadOffset -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcCDMAC_OnCmdSetRxReadOffset(
    const DtIoStubBcCDMAC* pStub,
    const DtIoctlCDmaCCmdSetRxRdOffsetInput* pInData)
{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));
    DT_ASSERT(pInData != NULL);

    // Set RX read offset
    Status = DtBcCDMAC_SetRxReadOffset(CDMAC_BC, pInData->m_RxReadOffset);

    return  Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmdSetTestMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcCDMAC_OnCmdSetTestMode(
    const DtIoStubBcCDMAC* pStub,
    const DtIoctlCDmaCCmdSetTestModeInput* pInData)
{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));
    DT_ASSERT(pInData!=NULL);

    // Set operational mode
    Status = DtBcCDMAC_SetTestMode(CDMAC_BC, pInData->m_TestMode);

    return  Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnCmdSetTxWriteOffset -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcCDMAC_OnCmdSetTxWriteOffset(
    const DtIoStubBcCDMAC* pStub,
    const DtIoctlCDmaCCmdSetTxWrOffsetInput* pInData)
{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));
    DT_ASSERT(pInData != NULL);

    // Set TX write offset
    Status = DtBcCDMAC_SetTxWriteOffset(CDMAC_BC, pInData->m_TxWriteOffset);

    return  Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCDMAC_OnExclAccessCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCDMAC_OnExclAccessCmd(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams,
    Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCDMAC));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_EXCL_ACCESS_CMD);

    // Only handle the release of exclusive access command
    if (pIoParams->m_Cmd == DT_EXCLUSIVE_ACCESS_CMD_RELEASE)
    {
        // Check if the owner releases exclusive access
        Status = DtBc_ExclAccessCheck((DtBc*)CDMAC_BC, &pIoParams->m_ExclAccessObj);
        if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)CDMAC_BC))
        {
            // To be sure clean-up allocated resources
            DtDbgOutBc(AVG, CDMAC, CDMAC_BC, "Before releasing exclusive access; "
                                                           "go to IDLE and free buffers");
            // Go to IDLE
            Status = DtBcCDMAC_SetOperationalMode(CDMAC_BC, DT_BLOCK_OPMODE_IDLE);
            if (!DT_SUCCESS(Status))
                DtDbgOutBc(ERR, CDMAC, CDMAC_BC, "ERROR: failed to set operational mode");
            // And free allocated buffers
            Status = DtBcCDMAC_FreeBuffer(CDMAC_BC);
            if (!DT_SUCCESS(Status))
                DtDbgOutBc(ERR, CDMAC, CDMAC_BC, "ERROR: failed to free buffers");
        }
    }

    // Use base function to handle the  exclusive access commands
    return DtIoStubBc_OnExclAccessCmd(pStub, pIoParams, pOutSize);
}
