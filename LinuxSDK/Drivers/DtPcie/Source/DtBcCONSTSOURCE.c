//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcCONSTSOURCE.c *#*#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018 DekTec Digital Video B.V.
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
#include "DtBcCONSTSOURCE.h"
#include "DtBcCONSTSOURCE_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+= DtBcCONSTSOURCE implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcCONSTSOURCE))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_CONSTSOURCE_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(CONSTSOURCE, pBc)

// Helper macro to cast a DtBc* to a DtBcCONSTSOURCE*
#define BC_CONSTSOURCE         ((DtBcCONSTSOURCE*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcCONSTSOURCE_Init(DtBc*);
static DtStatus  DtBcCONSTSOURCE_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcCONSTSOURCE_OnEnable(DtBc*, Bool  Enable);
static DtStatus  DtBcCONSTSOURCE_RegisterIntHandlers(DtBcCONSTSOURCE*);
static void  DtBcCONSTSOURCE_InterruptDpcTestDone(DtDpcArgs* pArgs);
static DtStatus  DtBcCONSTSOURCE_InterruptHandler(DtBc*, Int, Int, void*);
static void  DtBcCONSTSOURCE_SetControlRegs(DtBcCONSTSOURCE* pBc, Bool BlkEna, Int OpMode, 
                                Int DataPattern, Int TestInteval, Bool StartTestInterval);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcCONSTSOURCE - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSOURCE_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcCONSTSOURCE_Close(DtBc*  pBc)
{
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSOURCE_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcCONSTSOURCE*  DtBcCONSTSOURCE_Open(Int  Address, DtCore*  pCore, DtPt*  pPt,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_CONSTSOURCE_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcCONSTSOURCE, Id, DT_BLOCK_TYPE_CONSTSOURCE,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcCONSTSOURCE_Close;
    OpenParams.m_InitFunc = DtBcCONSTSOURCE_Init;
    OpenParams.m_OnEnableFunc = DtBcCONSTSOURCE_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcCONSTSOURCE_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcCONSTSOURCE*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSOURCE_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCONSTSOURCE_GetOperationalMode(DtBcCONSTSOURCE* pBc, Int* pOpMode)
{
    // Sanity check
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSOURCE_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSOURCE_GetDataPattern .-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCONSTSOURCE_GetDataPattern(DtBcCONSTSOURCE* pBc, Int* pDataPattern)
{
    // Sanity check
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pDataPattern == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSOURCE_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pDataPattern = pBc->m_DataPattern;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSOURCE_GetTestInterval -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCONSTSOURCE_GetTestInterval(DtBcCONSTSOURCE* pBc, Int* pTestItv)
{
    // Sanity check
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pTestItv == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSOURCE_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pTestItv = pBc->m_TestInterval;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSOURCE_GetDataRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCONSTSOURCE_GetDataRate(DtBcCONSTSOURCE* pBc, Int64* pDataRate)
{
    // Sanity check
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pDataRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSOURCE_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pDataRate = pBc->m_DataRate;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSOURCE_GetOverflowCnt -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCONSTSOURCE_GetOverflowCnt(DtBcCONSTSOURCE* pBc, UInt32* pOverflowCnt)
{
    // Sanity check
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOverflowCnt == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSOURCE_MUST_BE_ENABLED(pBc);

    *pOverflowCnt = CONSTSOURCE_OverflowCnt_READ(pBc);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSOURCE_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCONSTSOURCE_SetOperationalMode(DtBcCONSTSOURCE* pBc, Int OpMode)
{
    DtStatus Status;

    // Sanity check
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY 
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSOURCE_MUST_BE_ENABLED(pBc);
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Change Operational Mode
    if (OpMode == DT_BLOCK_OPMODE_RUN)
    {
        // IDLE or STANDBY -> RUN
        // Enable interrupt
        Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_CONSTSOURCE_TESTDONE);
        DT_ASSERT(DT_SUCCESS(Status));

        pBc->m_TestIntervalStarted = FALSE;
    }
    else if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    {
        // RUN -> STANDBY or IDLE
        // Disable interrupt
        Status = DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_CONSTSOURCE_TESTDONE);
        DT_ASSERT(DT_SUCCESS(Status));

        // Make sure that TestIntervalDoneEvent doesn't wait forever
        DtEventSet(&pBc->m_TestIntervalDoneEvent);
    }

    // Cache value
    pBc->m_OperationalMode = OpMode;

    DtBcCONSTSOURCE_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                    pBc->m_DataPattern, pBc->m_TestInterval, /*StartTestInterval*/ FALSE);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSOURCE_SetDataPattern -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCONSTSOURCE_SetDataPattern(DtBcCONSTSOURCE* pBc, Int DataPattern)
{
    // Sanity check
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    // Paramaters must be valid
    if (DataPattern != DT_CONSTSOURCE_INCREMENTING &&
                                                     DataPattern != DT_CONSTSOURCE_PRBS31)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSOURCE_MUST_BE_ENABLED(pBc);
    
    // Must be idle
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    // No change?
    if (pBc->m_DataPattern == DataPattern)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_DataPattern = DataPattern;

    DtBcCONSTSOURCE_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                    pBc->m_DataPattern, pBc->m_TestInterval, /*StartTestInterval*/ FALSE);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSOURCE_SetTestInteval -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCONSTSOURCE_SetTestInterval(DtBcCONSTSOURCE* pBc, Int TestItv)
{
    // Sanity check
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    // Paramaters must be valid
    if (TestItv < 0 || TestItv >= 1<<20)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSOURCE_MUST_BE_ENABLED(pBc);
    
    // Must be idle
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    // No change?
    if (pBc->m_TestInterval == TestItv)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_TestInterval = TestItv;

    DtBcCONSTSOURCE_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                    pBc->m_DataPattern, pBc->m_TestInterval, /*StartTestInterval*/ FALSE);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSOURCE_SetDataRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCONSTSOURCE_SetDataRate(DtBcCONSTSOURCE* pBc, Int64 DataRate)
{
    const Int64  MaxPhaseIncr = 1LL<<23;
    Int64  DataRatePhaseIncr;

    // Sanity check
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_CONSTSOURCE_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_DataRate == DataRate)
        return DT_STATUS_OK;

    // Use maximum rate?
    if (DataRate == -1)
        DataRatePhaseIncr = MaxPhaseIncr;
    else
    { 
        // Compute Data Rate Phase Increment
        DataRatePhaseIncr = DtDivideS64(MaxPhaseIncr * (Int64)DataRate,
                                           (Int64)pBc->m_SysClockFreq * pBc->m_DataWidth);
        // DataRatePhaseIncr is a 24 bit register
        if (DataRatePhaseIncr > MaxPhaseIncr)
            return DT_STATUS_INVALID_PARAMETER;
    }

    CONSTSOURCE_DataRatePhaseIncr_WRITE(pBc, (UInt32) DataRatePhaseIncr);

    // Store cached value
    pBc->m_DataRate = DataRate;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSOURCE_StartTestInterval -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCONSTSOURCE_StartTestInterval(DtBcCONSTSOURCE* pBc)
{
    // Sanity check
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_CONSTSOURCE_MUST_BE_ENABLED(pBc);

    // Operational Mode must be run to start
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    // The event is used to synchronize with the owning thread
    DtEventReset(&pBc->m_TestIntervalDoneEvent);

    // Ensure that StartTest is called before WaitTestDone
    pBc->m_TestIntervalStarted = TRUE;

    // start the test interval
    DtBcCONSTSOURCE_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                     pBc->m_DataPattern, pBc->m_TestInterval, /*StartTestInterval*/ TRUE);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSOURCE_WaitForTestIntervalDone -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCONSTSOURCE_WaitForTestIntervalDone(DtBcCONSTSOURCE* pBc)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_CONSTSOURCE_MUST_BE_ENABLED(pBc);

    // Operational mode must be run
    if (pBc->m_OperationalMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Ensure StartTestInterval is called before WaitForTestDone
    if (! pBc->m_TestIntervalStarted)
        return DT_STATUS_NOT_STARTED;

    // StartTest was called correctly, no longer needs to be called
    pBc->m_TestIntervalStarted = FALSE;

    // Wait for interrupt (Max TestInterval is 1 second)
    Status = DtEventWaitUnInt(&pBc->m_TestIntervalDoneEvent, 1100 /*ms*/);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, CONSTSOURCE, pBc, "Wait on event failure, Status:0x%X", Status);
        return Status;
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSOURCE_RunTest .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCONSTSOURCE_RunTest(DtBcCONSTSOURCE* pBc, Int64 DataRate, Int TestItv,
                                                                     UInt32* pOverflowCnt)
{
    UInt OverflowBefore;
    UInt OverflowAfter;

    DT_RETURN_ON_ERROR(DtBcCONSTSOURCE_GetOverflowCnt(pBc, &OverflowBefore));
    DT_RETURN_ON_ERROR(DtBcCONSTSOURCE_SetDataRate(pBc, DataRate));
    DT_RETURN_ON_ERROR(DtBcCONSTSOURCE_SetTestInterval(pBc, TestItv));
    DT_RETURN_ON_ERROR(DtBcCONSTSOURCE_StartTestInterval(pBc));
    DT_RETURN_ON_ERROR(DtBcCONSTSOURCE_WaitForTestIntervalDone(pBc));
    DT_RETURN_ON_ERROR(DtBcCONSTSOURCE_GetOverflowCnt(pBc, &OverflowAfter));

    *pOverflowCnt = OverflowAfter - OverflowBefore;

    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcCONSTSOURCE - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSOURCE_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcCONSTSOURCE_Init(DtBc*  pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcCONSTSOURCE* pBc = (DtBcCONSTSOURCE*)pBcBase;

    // Set defaults
    // Set Data Rate Phase Increment before Block Enable Bit
    pBc->m_DataRate = 0;
    CONSTSOURCE_DataRatePhaseIncr_WRITE(pBc, 0);

    // Set control register (including enable)
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_DataPattern = DT_CONSTSOURCE_INCREMENTING;
    pBc->m_TestInterval = 0;                              // Continuous

    pBc->m_BlockEnabled = FALSE;

    pBc->m_TestIntervalStarted = FALSE;

    // Save new setting
    DtBcCONSTSOURCE_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                    pBc->m_DataPattern, pBc->m_TestInterval, /*StartTestInterval*/ FALSE);

    // Read Configuration into Cache
    pBc->m_SysClockFreq = CONSTSOURCE_Config1_READ_SysClockFreq(pBc); 
    pBc->m_DataWidth = (Int) CONSTSOURCE_Config2_READ_DataWidth(pBc);

    // Sanity check the configuration to prevent divide by 0
    DT_ASSERT(pBc->m_SysClockFreq != 0);
    DT_ASSERT(pBc->m_DataWidth != 0);

    // Init interrupt DPC
    Status = DtDpcInit(&BC_CONSTSOURCE->m_IntDpc, DtBcCONSTSOURCE_InterruptDpcTestDone,
                                                                                    TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, CONSTSOURCE, pBc, "ERROR: failed to init DPC (Status=0x%08X)",
                                                                                  Status);
        return Status;
    }

    // Initialize Test Done event
    Status = DtEventInit(&BC_CONSTSOURCE->m_TestIntervalDoneEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, CONSTSOURCE, pBc, 
                              "ERROR: failed to init done event (Status=0x%08X)", Status);
        return Status;
    }
    
    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    Status = DtBcCONSTSOURCE_RegisterIntHandlers(pBc);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, CONSTSOURCE, pBc, "ERROR: failed to register interrupt handlers");
        return Status;
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSOURCE_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCONSTSOURCE_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcCONSTSOURCE* pBc = (DtBcCONSTSOURCE*)pBcBase;

    // Sanity check
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, CONSTSOURCE, pBc, "Block Enable from disable -> enable");

        // Set Data Rate Phase Increment before Block Enable Bit

        pBc->m_DataRate = 0;
        CONSTSOURCE_DataRatePhaseIncr_WRITE(pBc, 0);

        // Set control register (including enable)
        
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
        pBc->m_DataPattern = DT_CONSTSOURCE_INCREMENTING;
        pBc->m_TestInterval = 0;                              // Continuous

        pBc->m_BlockEnabled = TRUE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, CONSTSOURCE, pBc, "Block Enable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcCONSTSOURCE_SetOperationalMode(BC_CONSTSOURCE, DT_BLOCK_OPMODE_IDLE);

        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, CONSTSOURCE, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }

        pBc->m_BlockEnabled = FALSE;
    }

    // Save new setting
    DtBcCONSTSOURCE_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                    pBc->m_DataPattern, pBc->m_TestInterval, /*StartTestInterval*/ FALSE);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSOURCE_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcCONSTSOURCE_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled(pBc))
    {
        DtDbgOutBc(AVG, CONSTSOURCE, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcCONSTSOURCE_SetOperationalMode(BC_CONSTSOURCE, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, CONSTSOURCE, pBc, "ERROR: failed to set opmode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSOURCE_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcCONSTSOURCE_RegisterIntHandlers(DtBcCONSTSOURCE*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;
    Int  i=0;

    // Sanity check
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks, but do not enable the interrupts. We will enable 
    // them on demand
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch(Id)
        {
        case DT_INTERRUPT_CONSTSOURCE_TESTDONE:
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id, 
                                                     DtBcCONSTSOURCE_InterruptHandler, NULL);
            if(!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, CONSTSOURCE, pBc,
                               "ERROR: failed to register interrupt handler (ID=%d)", Id);
            }
            break;

        default:
            DT_ASSERT(FALSE);      // Unknown ID. New version of the block??
            break;
        }
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSOURCE_InterruptDpcTestDone -.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcCONSTSOURCE_InterruptDpcTestDone(DtDpcArgs* pArgs)
{
    DtBcCONSTSOURCE*  pBc = (DtBcCONSTSOURCE*)pArgs->m_pContext;

    // Sanity check
#ifdef DEBUG
    UInt32  IntId = pArgs->m_Data1.m_UInt32_1;
    DT_ASSERT(IntId == DT_INTERRUPT_CONSTSOURCE_TESTDONE);
#endif  // #ifdef DEBUG
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    DtEventSet(&pBc->m_TestIntervalDoneEvent);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSOURCE_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcCONSTSOURCE_InterruptHandler(DtBc*  pBc, Int  Id, Int  Index, void*  pContext)
{
    DtStatus  Status=DT_STATUS_OK;
    DtDpcArgs  DpcArgs;

    // Sanity check
    BC_CONSTSOURCE_DEFAULT_PRECONDITIONS(pBc);

    // Check ID is one of ours
    switch (Id)
    {
    case DT_INTERRUPT_CONSTSOURCE_TESTDONE:
        break;

       // Not a CONSTSOURCE interrupt
    default:
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    DpcArgs.m_Data1.m_UInt32_2 = 0;
    
    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&((DtBcCONSTSOURCE*)pBc)->m_IntDpc, &DpcArgs);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSOURCE_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcCONSTSOURCE_SetControlRegs(DtBcCONSTSOURCE* pBc, Bool BlkEna, Int OpMode, 
                                Int DataPattern, Int TestInterval, Bool StartTestInterval)
{
    UInt32 FwBlkEna=0, FwOpMode=0, FwDataPattern=0, RegData=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? CONSTSOURCE_BLKENA_ENABLED : CONSTSOURCE_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = CONSTSOURCE_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = CONSTSOURCE_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = CONSTSOURCE_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Convert data pattern to BB-type
    switch (DataPattern)
    {
    case DT_CONSTSOURCE_INCREMENTING:
        FwDataPattern = CONSTSOURCE_PATTERN_INCREMENTING;
        break;
    case DT_CONSTSOURCE_PRBS31:
        FwDataPattern = CONSTSOURCE_PATTERN_PRBS31;
        break;
    default: DT_ASSERT(FALSE);
    }

    // Update control
    RegData = CONSTSOURCE_Control_READ(pBc);
    RegData = CONSTSOURCE_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = CONSTSOURCE_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = CONSTSOURCE_Control_SET_DataPattern(RegData, FwDataPattern);
    RegData = CONSTSOURCE_Control_SET_TestInterval(RegData, (UInt32) TestInterval);
    RegData = CONSTSOURCE_Control_SET_StartTestInterval(RegData,
                                                               StartTestInterval ? 1 : 0);

    CONSTSOURCE_Control_WRITE(pBc, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcCONSTSOURCE implementation =+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcCONSTSOURCE
#define CONSTSOURCE_STUB   ((DtIoStubBcCONSTSOURCE*)pStub)
#define CONSTSOURCE_BC  ((DtBcCONSTSOURCE*)CONSTSOURCE_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcCONSTSOURCE_AppendDynamicSize(const DtIoStub*,
                                                                       DtIoStubIoParams*);
static DtStatus  DtIoStubBcCONSTSOURCE_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcCONSTSOURCE_OnCmdGetDataPattern(
                                              const DtIoStubBcCONSTSOURCE*, 
                                              DtIoctlConstSourceCmdGetDataPatternOutput*);
static DtStatus  DtIoStubBcCONSTSOURCE_OnCmdGetDataRate(
                                                 const DtIoStubBcCONSTSOURCE*, 
                                                 DtIoctlConstSourceCmdGetDataRateOutput*);
static DtStatus  DtIoStubBcCONSTSOURCE_OnCmdGetOperationalMode(
                                                   const DtIoStubBcCONSTSOURCE*, 
                                                   DtIoctlConstSourceCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcCONSTSOURCE_OnCmdGetOverflowCnt(
                                              const DtIoStubBcCONSTSOURCE*, 
                                              DtIoctlConstSourceCmdGetOverflowCntOutput*);
static DtStatus  DtIoStubBcCONSTSOURCE_OnCmdGetTestInterval(
                                             const DtIoStubBcCONSTSOURCE*, 
                                             DtIoctlConstSourceCmdGetTestIntervalOutput*);
static DtStatus  DtIoStubBcCONSTSOURCE_OnCmdRunTest( const DtIoStubBcCONSTSOURCE*,
                                                 const DtIoctlConstSourceCmdRunTestInput*,
                                                 DtIoctlConstSourceCmdRunTestOutput*);
static DtStatus  DtIoStubBcCONSTSOURCE_OnCmdSetDataPattern(
                                         const DtIoStubBcCONSTSOURCE*,
                                         const DtIoctlConstSourceCmdSetDataPatternInput*);
static DtStatus  DtIoStubBcCONSTSOURCE_OnCmdSetDataRate(
                                            const DtIoStubBcCONSTSOURCE*,
                                            const DtIoctlConstSourceCmdSetDataRateInput*);
static DtStatus  DtIoStubBcCONSTSOURCE_OnCmdSetOperationalMode(
                                              const DtIoStubBcCONSTSOURCE*,
                                              const DtIoctlConstSourceCmdSetOpModeInput*);
static DtStatus  DtIoStubBcCONSTSOURCE_OnCmdSetTestInterval(
                                        const DtIoStubBcCONSTSOURCE*,
                                        const DtIoctlConstSourceCmdSetTestIntervalInput*);
static DtStatus  DtIoStubBcCONSTSOURCE_OnCmdStartTestInterval(
                                                            const DtIoStubBcCONSTSOURCE*);
static DtStatus  DtIoStubBcCONSTSOURCE_OnCmdWaitForTestItvDone(
                                                            const DtIoStubBcCONSTSOURCE*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_CONSTSOURCE;

static const DtIoctlProperties  IOSTUB_BC_CONSTSOURCE_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_CONSTSOURCE_CMD(
        DtIoStubBcCONSTSOURCE_OnCmd,
        DtIoStubBcCONSTSOURCE_AppendDynamicSize,
        NULL),
};

//+=+=+=+=+=+=+=+=+=+=+= DtIoStubBcCONSTSOURCE - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcCONSTSOURCE_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSOURCE));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcCONSTSOURCE*  DtIoStubBcCONSTSOURCE_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcCONSTSOURCE));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcCONSTSOURCE, pBc, NULL, 
                                                            DtIoStubBcCONSTSOURCE_Close,
                                                            NULL,  // Use default IOCTL
                                                            IOSTUB_BC_CONSTSOURCE_IOCTLS);
    return (DtIoStubBcCONSTSOURCE*)DtIoStubBc_Open(&OpenParams);
}

//=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcCONSTSOURCE - Private functions =+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCONSTSOURCE_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlConstSourceCmdInput*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSOURCE));
    DT_ASSERT(pIoParams!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_CONSTSOURCE_CMD);
    
    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_ConstSourceCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCONSTSOURCE_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlConstSourceCmdInput*  pInData = NULL;
    DtIoctlConstSourceCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSOURCE));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_CONSTSOURCE_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, CONSTSOURCE, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_ConstSourceCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_ConstSourceCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_CONSTSOURCE_CMD_GET_DATA_PATTERN:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcCONSTSOURCE_OnCmdGetDataPattern(CONSTSOURCE_STUB,  
                                                             &pOutData->m_GetDataPattern);
        break;
    case DT_CONSTSOURCE_CMD_GET_DATA_RATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcCONSTSOURCE_OnCmdGetDataRate(CONSTSOURCE_STUB,  
                                                                &pOutData->m_GetDataRate);
        break;
    case DT_CONSTSOURCE_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcCONSTSOURCE_OnCmdGetOperationalMode(CONSTSOURCE_STUB,  
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_CONSTSOURCE_CMD_GET_OVERFLOW_CNT:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcCONSTSOURCE_OnCmdGetOverflowCnt(CONSTSOURCE_STUB,  
                                                             &pOutData->m_GetOverflowCnt);
        break;
    case DT_CONSTSOURCE_CMD_GET_TEST_INTERVAL:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcCONSTSOURCE_OnCmdGetTestInterval(CONSTSOURCE_STUB,  
                                                            &pOutData->m_GetTestInterval);
        break;
    case DT_CONSTSOURCE_CMD_RUN_TEST:
        DT_ASSERT(pInData != NULL);
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcCONSTSOURCE_OnCmdRunTest(CONSTSOURCE_STUB, &pInData->m_RunTest,
                                                                    &pOutData->m_RunTest);
        break;
    case DT_CONSTSOURCE_CMD_SET_DATA_PATTERN:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcCONSTSOURCE_OnCmdSetDataPattern(CONSTSOURCE_STUB,  
                                                              &pInData->m_SetDataPattern);
        break;
    case DT_CONSTSOURCE_CMD_SET_DATA_RATE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcCONSTSOURCE_OnCmdSetDataRate(CONSTSOURCE_STUB,  
                                                                 &pInData->m_SetDataRate);
        break;
    case DT_CONSTSOURCE_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcCONSTSOURCE_OnCmdSetOperationalMode(CONSTSOURCE_STUB,  
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_CONSTSOURCE_CMD_SET_TEST_INTERVAL:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcCONSTSOURCE_OnCmdSetTestInterval(CONSTSOURCE_STUB,  
                                                             &pInData->m_SetTestInterval);
        break;
    case DT_CONSTSOURCE_CMD_START_TEST_INTERVAL:
        Status = DtIoStubBcCONSTSOURCE_OnCmdStartTestInterval(CONSTSOURCE_STUB);
        break;
    case DT_CONSTSOURCE_CMD_WAIT_FOR_TEST_ITV_DONE:
        Status = DtIoStubBcCONSTSOURCE_OnCmdWaitForTestItvDone(CONSTSOURCE_STUB);
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE_OnCmdGetDataPattern -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSOURCE_OnCmdGetDataPattern(
    const DtIoStubBcCONSTSOURCE* pStub,
    DtIoctlConstSourceCmdGetDataPatternOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSOURCE));
    DT_ASSERT(pOutData != NULL);

    return DtBcCONSTSOURCE_GetDataPattern(CONSTSOURCE_BC, &pOutData->m_DataPattern);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE_OnCmdGetDataRate .-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCONSTSOURCE_OnCmdGetDataRate(
    const DtIoStubBcCONSTSOURCE* pStub,
    DtIoctlConstSourceCmdGetDataRateOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSOURCE));
    DT_ASSERT(pOutData != NULL);

    return DtBcCONSTSOURCE_GetDataRate(CONSTSOURCE_BC, &pOutData->m_DataRate);
}

//.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSOURCE_OnCmdGetOperationalMode(
    const DtIoStubBcCONSTSOURCE* pStub,
    DtIoctlConstSourceCmdGetOpModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSOURCE));
    DT_ASSERT(pOutData != NULL);

    return DtBcCONSTSOURCE_GetOperationalMode(CONSTSOURCE_BC, &pOutData->m_OpMode);
}

//-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE_OnCmdGetOverflowCnt .-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSOURCE_OnCmdGetOverflowCnt(
    const DtIoStubBcCONSTSOURCE* pStub,
    DtIoctlConstSourceCmdGetOverflowCntOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSOURCE));
    DT_ASSERT(pOutData != NULL);

    return DtBcCONSTSOURCE_GetOverflowCnt(CONSTSOURCE_BC, &pOutData->m_OverflowCnt);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE_OnCmdGetTestInterval -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCONSTSOURCE_OnCmdGetTestInterval(
    const DtIoStubBcCONSTSOURCE* pStub,
    DtIoctlConstSourceCmdGetTestIntervalOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSOURCE));
    DT_ASSERT(pOutData != NULL);

    return DtBcCONSTSOURCE_GetTestInterval(CONSTSOURCE_BC, &pOutData->m_TestItv);
}

//.-.-.-.-.-.-.-.-.-.-.-.-. DtIoStubBcCONSTSOURCE_OnCmdRunTest .-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCONSTSOURCE_OnCmdRunTest(
    const DtIoStubBcCONSTSOURCE* pStub,
    const DtIoctlConstSourceCmdRunTestInput* pInData,
    DtIoctlConstSourceCmdRunTestOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSOURCE));
    DT_ASSERT(pInData != NULL);
    DT_ASSERT(pOutData != NULL);

    return DtBcCONSTSOURCE_RunTest(CONSTSOURCE_BC, pInData->m_DataRate,
                                            pInData->m_TestItv, &pOutData->m_OverflowCnt);

}

//--.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE_OnCmdSetDataPattern .-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCONSTSOURCE_OnCmdSetDataPattern(
    const DtIoStubBcCONSTSOURCE* pStub,
    const DtIoctlConstSourceCmdSetDataPatternInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSOURCE));
    DT_ASSERT(pInData != NULL);

    return DtBcCONSTSOURCE_SetDataPattern(CONSTSOURCE_BC, pInData->m_DataPattern);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE_OnCmdSetDataRate .-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCONSTSOURCE_OnCmdSetDataRate(
    const DtIoStubBcCONSTSOURCE* pStub,
    const DtIoctlConstSourceCmdSetDataRateInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSOURCE));
    DT_ASSERT(pInData != NULL);

    return DtBcCONSTSOURCE_SetDataRate(CONSTSOURCE_BC, pInData->m_DataRate);
}

//.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE_OnCmdSetOperationalMode .-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCONSTSOURCE_OnCmdSetOperationalMode(
    const DtIoStubBcCONSTSOURCE* pStub,
    const DtIoctlConstSourceCmdSetOpModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSOURCE));
    DT_ASSERT(pInData != NULL);

    return DtBcCONSTSOURCE_SetOperationalMode(CONSTSOURCE_BC, pInData->m_OpMode);
}

//-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE_OnCmdSetTestInterval .-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCONSTSOURCE_OnCmdSetTestInterval(
    const DtIoStubBcCONSTSOURCE* pStub,
    const DtIoctlConstSourceCmdSetTestIntervalInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSOURCE));
    DT_ASSERT(pInData != NULL);

    return DtBcCONSTSOURCE_SetTestInterval(CONSTSOURCE_BC, pInData->m_TestItv);
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE_OnCmdStartTest -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCONSTSOURCE_OnCmdStartTestInterval(
    const DtIoStubBcCONSTSOURCE* pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSOURCE));

    return DtBcCONSTSOURCE_StartTestInterval(CONSTSOURCE_BC);
}

//.-.-.-.-.-.-.-.-.-.-. DtIoStubBcCONSTSOURCE_OnCmdWaitForTestDone .-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCONSTSOURCE_OnCmdWaitForTestItvDone(
    const DtIoStubBcCONSTSOURCE* pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSOURCE));

    return DtBcCONSTSOURCE_WaitForTestIntervalDone(CONSTSOURCE_BC);
}
