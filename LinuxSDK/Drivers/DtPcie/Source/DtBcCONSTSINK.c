//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcCONSTSINK.c *#*#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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
#include "DtBcCONSTSINK.h"
#include "DtBcCONSTSINK_RegAccess.h"


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcCONSTSINK implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcCONSTSINK))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_CONSTSINK_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(CONSTSINK, pBc)

// Helper macro to cast a DtBc* to a DtBcCONSTSINK*
#define BC_CONSTSINK         ((DtBcCONSTSINK*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcCONSTSINK_Init(DtBc*);
static DtStatus  DtBcCONSTSINK_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcCONSTSINK_OnEnable(DtBc*, Bool  Enable);
static DtStatus  DtBcCONSTSINK_RegisterIntHandlers(DtBcCONSTSINK*);
static void  DtBcCONSTSINK_InterruptDpcTestDone(DtDpcArgs* pArgs);
static DtStatus  DtBcCONSTSINK_InterruptHandler(DtBc*, Int, Int, void*);
static void  DtBcCONSTSINK_SetControlRegs(DtBcCONSTSINK* pBc, Bool BlkEna, Int OpMode, 
                        Int DataPattern, Int TestInteval, Bool StartTest, Bool CheckData);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcCONSTSINK - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSINK_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcCONSTSINK_Close(DtBc*  pBc)
{
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSINK_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcCONSTSINK*  DtBcCONSTSINK_Open(Int  Address, DtCore*  pCore, DtPt*  pPt,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_CONSTSINK_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcCONSTSINK, Id, DT_BLOCK_TYPE_CONSTSINK,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcCONSTSINK_Close;
    OpenParams.m_InitFunc = DtBcCONSTSINK_Init;
    OpenParams.m_OnEnableFunc = DtBcCONSTSINK_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcCONSTSINK_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcCONSTSINK*)DtBc_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSINK_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCONSTSINK_GetOperationalMode(DtBcCONSTSINK* pBc, Int* pOpMode)
{
    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSINK_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSINK_GetDataPattern .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCONSTSINK_GetDataPattern(DtBcCONSTSINK* pBc, Int* pDataPattern)
{
    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pDataPattern == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSINK_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pDataPattern = pBc->m_DataPattern;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSINK_GetTestInterval -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCONSTSINK_GetTestInterval(DtBcCONSTSINK* pBc, Int* pTestItv)
{
    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pTestItv == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSINK_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pTestItv = pBc->m_TestInterval;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSINK_GetCheckData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This returns the boolean value CheckData as an Int to simplify invocation from the Stub
// interface which doesn't support Bool (Bool is implimented as a native bool with no
// guarantee as to storage size which is needed for IOCTL to pass user/kernel boundary).
//
DtStatus DtBcCONSTSINK_GetCheckData(DtBcCONSTSINK* pBc, Int* pCheckData)
{
    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pCheckData == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSINK_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pCheckData = pBc->m_CheckData;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSINK_GetDataRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCONSTSINK_GetDataRate(DtBcCONSTSINK* pBc, Int64* pDataRate)
{
    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pDataRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSINK_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pDataRate = pBc->m_DataRate;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSINK_GetUnderflowCnt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCONSTSINK_GetUnderflowCnt(DtBcCONSTSINK* pBc, UInt32* pUnderflowCnt)
{
    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pUnderflowCnt == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSINK_MUST_BE_ENABLED(pBc);

    *pUnderflowCnt = CONSTSINK_UnderflowCnt_READ(pBc);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSINK_GetDataErrorCnt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCONSTSINK_GetDataErrorCnt(DtBcCONSTSINK* pBc, UInt32* pDataErrorCnt)
{
    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pDataErrorCnt == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSINK_MUST_BE_ENABLED(pBc);

    *pDataErrorCnt = CONSTSINK_DataErrorCnt_READ(pBc);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSINK_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCONSTSINK_SetOperationalMode(DtBcCONSTSINK* pBc, Int OpMode)
{
    DtStatus Status;

    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSINK_MUST_BE_ENABLED(pBc);
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Change Operational Mode
    if (OpMode == DT_BLOCK_OPMODE_RUN)
    {
        // IDLE or STANDBY -> RUN
        // Enable interrupt
        Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_CONSTSINK_TESTDONE);
        DT_ASSERT(DT_SUCCESS(Status));

        pBc->m_TestIntervalStarted = FALSE;
    }
    else if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    {
        // RUN -> IDLE or STANDBY
        // Disable interrupt
        Status = DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_CONSTSINK_TESTDONE);
        DT_ASSERT(DT_SUCCESS(Status));

        // Make sure that TestIntervalDoneEvent doesn't wait forever
        DtEventSet(&pBc->m_TestIntervalDoneEvent);
    }

    // Cache value
    pBc->m_OperationalMode = OpMode;

    DtBcCONSTSINK_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
          pBc->m_DataPattern, pBc->m_TestInterval, /*StartTest*/ FALSE, pBc->m_CheckData);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSINK_SetDataPattern -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCONSTSINK_SetDataPattern(DtBcCONSTSINK* pBc, Int DataPattern)
{
    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Paramaters must be valid
    if (DataPattern != DT_CONSTSINK_INCREMENTING && DataPattern != DT_CONSTSINK_PRBS31)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSINK_MUST_BE_ENABLED(pBc);

    // Must be idle
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    // No change?
    if (pBc->m_DataPattern == DataPattern)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_DataPattern = DataPattern;

    DtBcCONSTSINK_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
          pBc->m_DataPattern, pBc->m_TestInterval, /*StartTest*/ FALSE, pBc->m_CheckData);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSINK_SetTestInteval -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCONSTSINK_SetTestInterval(DtBcCONSTSINK* pBc, Int TestItv)
{
    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Paramaters must be valid
    if (TestItv < 0 || TestItv >= 1<<20)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_CONSTSINK_MUST_BE_ENABLED(pBc);

    // Must be idle
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    // No change?
    if (pBc->m_TestInterval == TestItv)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_TestInterval = TestItv;

    DtBcCONSTSINK_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
          pBc->m_DataPattern, pBc->m_TestInterval, /*StartTest*/ FALSE, pBc->m_CheckData);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSINK_SetCheckData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This takes the boolean value CheckData as an Int to simplify invocation from the Stub
// interface which doesn't support Bool (Bool is implimented as a native bool with no
// guarantee as to storage size which is needed for IOCTL to pass user/kernel boundary).
//
DtStatus DtBcCONSTSINK_SetCheckData(DtBcCONSTSINK* pBc, Int CheckData)
{
    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_CONSTSINK_MUST_BE_ENABLED(pBc);

    // Must be idle
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    // No change?
    if (pBc->m_CheckData == (Bool)CheckData)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_CheckData = (Bool)CheckData;

    DtBcCONSTSINK_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
          pBc->m_DataPattern, pBc->m_TestInterval, /*StartTest*/ FALSE, pBc->m_CheckData);

    return DT_STATUS_OK;
}
    
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSINK_SetDataRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCONSTSINK_SetDataRate(DtBcCONSTSINK* pBc, Int64 DataRate)
{
    const Int64  MaxPhaseIncr = 1LL<<23;
    Int64  DataRatePhaseIncr;

    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_CONSTSINK_MUST_BE_ENABLED(pBc);

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

    CONSTSINK_DataRatePhaseIncr_WRITE(pBc, (UInt32) DataRatePhaseIncr);

    // Store cached value
    pBc->m_DataRate = DataRate;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSINK_StartTestInterval -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCONSTSINK_StartTestInterval(DtBcCONSTSINK* pBc)
{
    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_CONSTSINK_MUST_BE_ENABLED(pBc);

    // Operational Mode must be run to start
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    // The event is used to synchronize with the owning thread
    DtEventReset(&pBc->m_TestIntervalDoneEvent);

    // Ensure that StartTest is called before WaitTestDone
    pBc->m_TestIntervalStarted = TRUE;

    // start the test interval
    DtBcCONSTSINK_SetControlRegs(pBc, pBc->m_BlockEnabled,
                          pBc->m_OperationalMode, pBc->m_DataPattern, pBc->m_TestInterval,
                                                    /*StartTest*/ TRUE, pBc->m_CheckData);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSINK_WaitForTestIntervalDone -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCONSTSINK_WaitForTestIntervalDone(DtBcCONSTSINK* pBc)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_CONSTSINK_MUST_BE_ENABLED(pBc);

    // Operational mode must be run
    if (pBc->m_OperationalMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Ensure StartTestInterval called before WaitForTestDone
    if (!pBc->m_TestIntervalStarted)
        return DT_STATUS_NOT_STARTED;

    // StartTest was called correctly, no longer needs to be called
    pBc->m_TestIntervalStarted = FALSE;

    // Wait for interrupt (Max TestInterval is 1 second)
    Status = DtEventWaitUnInt(&pBc->m_TestIntervalDoneEvent, 1100 /*ms*/);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, CONSTSINK, pBc, "Wait on event failure, Status:0x%X", Status);
        return Status;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSINK_RunTest .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcCONSTSINK_RunTest(DtBcCONSTSINK* pBc, Int64 DataRate, Int TestItv,
                                             UInt32* pUnderflowCnt, UInt32* pDataErrorCnt)
{
    UInt UnderflowBefore;
    UInt UnderflowAfter;
    UInt ErrorBefore;
    UInt ErrorAfter;

    DT_RETURN_ON_ERROR(DtBcCONSTSINK_GetUnderflowCnt(pBc, &UnderflowBefore));
    DT_RETURN_ON_ERROR(DtBcCONSTSINK_GetDataErrorCnt(pBc, &ErrorBefore));
    DT_RETURN_ON_ERROR(DtBcCONSTSINK_SetDataRate(pBc, DataRate));
    DT_RETURN_ON_ERROR(DtBcCONSTSINK_SetTestInterval(pBc, TestItv));
    DT_RETURN_ON_ERROR(DtBcCONSTSINK_StartTestInterval(pBc));
    DT_RETURN_ON_ERROR(DtBcCONSTSINK_WaitForTestIntervalDone(pBc));
    DT_RETURN_ON_ERROR(DtBcCONSTSINK_GetUnderflowCnt(pBc, &UnderflowAfter));
    DT_RETURN_ON_ERROR(DtBcCONSTSINK_GetDataErrorCnt(pBc, &ErrorAfter));

    *pUnderflowCnt = UnderflowAfter - UnderflowBefore;
    *pDataErrorCnt = ErrorAfter - ErrorBefore;

    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcCONSTSINK - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSINK_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcCONSTSINK_Init(DtBc*  pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcCONSTSINK* pBc = (DtBcCONSTSINK*)pBcBase;

    // Set defaults
    // Set Data Rate Phase Increment before Block Enable Bit
    pBc->m_DataRate = 0;
    CONSTSINK_DataRatePhaseIncr_WRITE(pBc, 0);

    // Set control register (including enable)
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_DataPattern = DT_CONSTSINK_INCREMENTING;
    pBc->m_TestInterval = 0;                              // Continuous
    pBc->m_CheckData = FALSE;

    pBc->m_BlockEnabled = FALSE;

    pBc->m_TestIntervalStarted = FALSE;

    // Save new setting
    DtBcCONSTSINK_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
          pBc->m_DataPattern, pBc->m_TestInterval, /*StartTest*/ FALSE, pBc->m_CheckData);


    // Read Configuration into Cache
    pBc->m_SysClockFreq = CONSTSINK_Config1_READ_SysClockFreq(pBc); 
    pBc->m_DataWidth = (Int) CONSTSINK_Config2_READ_DataWidth(pBc);

    // Sanity check the configuration to prevent divide by 0
    DT_ASSERT(pBc->m_SysClockFreq != 0);
    DT_ASSERT(pBc->m_DataWidth != 0);

    // Init interrupt DPC
    Status = DtDpcInit(&BC_CONSTSINK->m_IntDpc, DtBcCONSTSINK_InterruptDpcTestDone, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, CONSTSINK, pBc, "ERROR: failed to init DPC (Status=0x%08X)", Status);
        return Status;
    }

    // Initialize Test Done event
    Status = DtEventInit(&BC_CONSTSINK->m_TestIntervalDoneEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, CONSTSINK, pBc, 
                              "ERROR: failed to init done event (Status=0x%08X)", Status);
        return Status;
    }
    
    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    Status = DtBcCONSTSINK_RegisterIntHandlers(pBc);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, CONSTSINK, pBc, "ERROR: failed to register interrupt handlers");
        return Status;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSINK_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcCONSTSINK_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcCONSTSINK* pBc = (DtBcCONSTSINK*)pBcBase;

    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, CONSTSINK, pBc, "Block Enable from disable -> enable");

        // Set Data Rate Phase Increment before Block Enable Bit

        pBc->m_DataRate = 0;
        CONSTSINK_DataRatePhaseIncr_WRITE(pBc, 0);

        // Set control register (including enable)
        
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
        pBc->m_DataPattern = DT_CONSTSINK_INCREMENTING;
        pBc->m_TestInterval = 0;                              // Continuous
        pBc->m_CheckData = FALSE;

        pBc->m_BlockEnabled = TRUE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, CONSTSINK, pBc, "Block Enable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcCONSTSINK_SetOperationalMode(BC_CONSTSINK, DT_BLOCK_OPMODE_IDLE);

        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, CONSTSINK, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }

        pBc->m_BlockEnabled = FALSE;
    }

    // Save new setting
    DtBcCONSTSINK_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
          pBc->m_DataPattern, pBc->m_TestInterval, /*StartTest*/ FALSE, pBc->m_CheckData);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSINK_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcCONSTSINK_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled(pBc))
    {
        DtDbgOutBc(AVG, CONSTSINK, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcCONSTSINK_SetOperationalMode(BC_CONSTSINK, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, CONSTSINK, pBc, "ERROR: failed to set opmode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSINK_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcCONSTSINK_RegisterIntHandlers(DtBcCONSTSINK*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;
    Int  i=0;

    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks, but do not enable the interrupts. We will enable 
    // them on demand
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch(Id)
        {
        case DT_INTERRUPT_CONSTSINK_TESTDONE:
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id, 
                                                     DtBcCONSTSINK_InterruptHandler, NULL);
            if(!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, CONSTSINK, pBc,
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

//-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSINK_InterruptDpcTestDone -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtBcCONSTSINK_InterruptDpcTestDone(DtDpcArgs* pArgs)
{
    DtBcCONSTSINK*  pBc = (DtBcCONSTSINK*)pArgs->m_pContext;

    // Sanity check
#ifdef DEBUG
    UInt32  IntId = pArgs->m_Data1.m_UInt32_1;
    DT_ASSERT(IntId == DT_INTERRUPT_CONSTSINK_TESTDONE);
#endif  // #ifdef DEBUG
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    DtEventSet(&pBc->m_TestIntervalDoneEvent);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSINK_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcCONSTSINK_InterruptHandler(DtBc*  pBc, Int  Id, Int  Index, void*  pContext)
{
    DtStatus  Status=DT_STATUS_OK;
    DtDpcArgs  DpcArgs;

    // Sanity check
    BC_CONSTSINK_DEFAULT_PRECONDITIONS(pBc);

    // Check ID is one of ours
    switch (Id)
    {
    case DT_INTERRUPT_CONSTSINK_TESTDONE:
        break;

        // Not a CONSTSINK interrupt
    default:
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    DpcArgs.m_Data1.m_UInt32_2 = 0;
    
    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&((DtBcCONSTSINK*)pBc)->m_IntDpc, &DpcArgs);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSINK_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcCONSTSINK_SetControlRegs(DtBcCONSTSINK* pBc, Bool BlkEna, Int OpMode, 
                        Int DataPattern, Int TestInterval, Bool StartTest, Bool CheckData)
{
    UInt32 FwBlkEna=0, FwOpMode=0, FwDataPattern=0, RegData=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? CONSTSINK_BLKENA_ENABLED : CONSTSINK_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = CONSTSINK_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = CONSTSINK_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = CONSTSINK_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Convert data pattern to BB-type
    switch (DataPattern)
    {
    case DT_CONSTSINK_INCREMENTING:
        FwDataPattern = CONSTSINK_PATTERN_INCREMENTING;
        break;
    case DT_CONSTSINK_PRBS31:
        FwDataPattern = CONSTSINK_PATTERN_PRBS31;
        break;
    default: DT_ASSERT(FALSE);
    }

    // Update control
    RegData = CONSTSINK_Control_READ(pBc);
    RegData = CONSTSINK_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = CONSTSINK_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = CONSTSINK_Control_SET_DataPattern(RegData, FwDataPattern);
    RegData = CONSTSINK_Control_SET_TestInterval(RegData, (UInt32) TestInterval);
    RegData = CONSTSINK_Control_SET_StartTestInterval(RegData, StartTest ? 1 : 0);
    RegData = CONSTSINK_Control_SET_CheckData(RegData, CheckData ? 1 : 0);

    CONSTSINK_Control_WRITE(pBc, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcCONSTSINK implementation =+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcCONSTSINK
#define CONSTSINK_STUB   ((DtIoStubBcCONSTSINK*)pStub)
#define CONSTSINK_BC  ((DtBcCONSTSINK*)CONSTSINK_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcCONSTSINK_AppendDynamicSize(const DtIoStub*,
                                                                       DtIoStubIoParams*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmdGetCheckData(
                                                  const DtIoStubBcCONSTSINK*, 
                                                  DtIoctlConstSinkCmdGetCheckDataOutput*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmdGetDataErrorCnt(
                                               const DtIoStubBcCONSTSINK*, 
                                               DtIoctlConstSinkCmdGetDataErrorCntOutput*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmdGetDataPattern(
                                                const DtIoStubBcCONSTSINK*, 
                                                DtIoctlConstSinkCmdGetDataPatternOutput*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmdGetDataRate(
                                                   const DtIoStubBcCONSTSINK*, 
                                                   DtIoctlConstSinkCmdGetDataRateOutput*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmdGetOperationalMode(
                                                     const DtIoStubBcCONSTSINK*, 
                                                     DtIoctlConstSinkCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmdGetTestInterval(
                                               const DtIoStubBcCONSTSINK*, 
                                               DtIoctlConstSinkCmdGetTestIntervalOutput*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmdGetUnderflowCnt(
                                               const DtIoStubBcCONSTSINK*, 
                                               DtIoctlConstSinkCmdGetUnderflowCntOutput*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmdRunTest( const DtIoStubBcCONSTSINK*,
                                                   const DtIoctlConstSinkCmdRunTestInput*,
                                                   DtIoctlConstSinkCmdRunTestOutput*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmdSetCheckData(
                                             const DtIoStubBcCONSTSINK*,
                                             const DtIoctlConstSinkCmdSetCheckDataInput*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmdSetDataPattern(
                                           const DtIoStubBcCONSTSINK*,
                                           const DtIoctlConstSinkCmdSetDataPatternInput*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmdSetDataRate(
                                              const DtIoStubBcCONSTSINK*,
                                              const DtIoctlConstSinkCmdSetDataRateInput*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmdSetOperationalMode(
                                                const DtIoStubBcCONSTSINK*,
                                                const DtIoctlConstSinkCmdSetOpModeInput*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmdSetTestInterval(
                                          const DtIoStubBcCONSTSINK*,
                                          const DtIoctlConstSinkCmdSetTestIntervalInput*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmdStartTestInterval(const DtIoStubBcCONSTSINK*);
static DtStatus  DtIoStubBcCONSTSINK_OnCmdWaitForTestItvDone(const DtIoStubBcCONSTSINK*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_CONSTSINK;

static const DtIoctlProperties  IOSTUB_BC_CONSTSINK_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_CONSTSINK_CMD(
        DtIoStubBcCONSTSINK_OnCmd,
        DtIoStubBcCONSTSINK_AppendDynamicSize,
        NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+= DtIoStubBcCONSTSINK - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcCONSTSINK_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcCONSTSINK*  DtIoStubBcCONSTSINK_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcCONSTSINK));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcCONSTSINK, pBc, NULL, 
                                                           DtIoStubBcCONSTSINK_Close,
                                                           NULL,  // Use default IOCTL
                                                           IOSTUB_BC_CONSTSINK_IOCTLS);
    return (DtIoStubBcCONSTSINK*)DtIoStubBc_Open(&OpenParams);
}

//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcCONSTSINK - Private functions =+=+=+=+=+=+=+=+=+=+=+=+


//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSINK_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlConstSinkCmdInput*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));
    DT_ASSERT(pIoParams!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_CONSTSINK_CMD);
    
    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_ConstSinkCmd;

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


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSINK_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlConstSinkCmdInput*  pInData = NULL;
    DtIoctlConstSinkCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_CONSTSINK_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, CONSTSINK, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_ConstSinkCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_ConstSinkCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_CONSTSINK_CMD_GET_CHECK_DATA:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcCONSTSINK_OnCmdGetCheckData(CONSTSINK_STUB,  
                                                               &pOutData->m_GetCheckData);
        break;
    case DT_CONSTSINK_CMD_GET_DATA_ERROR_CNT:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcCONSTSINK_OnCmdGetDataErrorCnt(CONSTSINK_STUB,  
                                                            &pOutData->m_GetDataErrorCnt);
        break;
    case DT_CONSTSINK_CMD_GET_DATA_PATTERN:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcCONSTSINK_OnCmdGetDataPattern(CONSTSINK_STUB,  
                                                             &pOutData->m_GetDataPattern);
        break;
    case DT_CONSTSINK_CMD_GET_DATA_RATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcCONSTSINK_OnCmdGetDataRate(CONSTSINK_STUB,  
                                                                &pOutData->m_GetDataRate);
        break;
    case DT_CONSTSINK_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcCONSTSINK_OnCmdGetOperationalMode(CONSTSINK_STUB,  
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_CONSTSINK_CMD_GET_TEST_INTERVAL:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcCONSTSINK_OnCmdGetTestInterval(CONSTSINK_STUB,  
                                                            &pOutData->m_GetTestInterval);
        break;
    case DT_CONSTSINK_CMD_GET_UNDERFLOW_CNT:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcCONSTSINK_OnCmdGetUnderflowCnt(CONSTSINK_STUB,  
                                                            &pOutData->m_GetUnderflowCnt);
        break;
    case DT_CONSTSINK_CMD_RUN_TEST:
        DT_ASSERT(pInData != NULL);
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcCONSTSINK_OnCmdRunTest(CONSTSINK_STUB, &pInData->m_RunTest,
                                                                    &pOutData->m_RunTest);
        break;
    case DT_CONSTSINK_CMD_SET_CHECK_DATA:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcCONSTSINK_OnCmdSetCheckData(CONSTSINK_STUB,  
                                                                &pInData->m_SetCheckData);
        break;
    case DT_CONSTSINK_CMD_SET_DATA_PATTERN:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcCONSTSINK_OnCmdSetDataPattern(CONSTSINK_STUB,  
                                                              &pInData->m_SetDataPattern);
        break;
    case DT_CONSTSINK_CMD_SET_DATA_RATE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcCONSTSINK_OnCmdSetDataRate(CONSTSINK_STUB,  
                                                                 &pInData->m_SetDataRate);
        break;
    case DT_CONSTSINK_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcCONSTSINK_OnCmdSetOperationalMode(CONSTSINK_STUB,  
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_CONSTSINK_CMD_SET_TEST_INTERVAL:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcCONSTSINK_OnCmdSetTestInterval(CONSTSINK_STUB,  
                                                             &pInData->m_SetTestInterval);
        break;
    case DT_CONSTSINK_CMD_START_TEST_INTERVAL:
        Status = DtIoStubBcCONSTSINK_OnCmdStartTestInterval(CONSTSINK_STUB);
        break;
    case DT_CONSTSINK_CMD_WAIT_FOR_TEST_ITV_DONE:
        Status = DtIoStubBcCONSTSINK_OnCmdWaitForTestItvDone(CONSTSINK_STUB);
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_OnCmdGetCheckData -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSINK_OnCmdGetCheckData(
    const DtIoStubBcCONSTSINK* pStub,
    DtIoctlConstSinkCmdGetCheckDataOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));
    DT_ASSERT(pOutData != NULL);

    return DtBcCONSTSINK_GetCheckData(CONSTSINK_BC, &pOutData->m_CheckData);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_OnCmdGetDataErrorCnt .-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSINK_OnCmdGetDataErrorCnt(
    const DtIoStubBcCONSTSINK* pStub,
    DtIoctlConstSinkCmdGetDataErrorCntOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));
    DT_ASSERT(pOutData != NULL);

    return DtBcCONSTSINK_GetDataErrorCnt(CONSTSINK_BC, &pOutData->m_DataErrorCnt);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_OnCmdGetDataPattern -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCONSTSINK_OnCmdGetDataPattern(
    const DtIoStubBcCONSTSINK* pStub,
    DtIoctlConstSinkCmdGetDataPatternOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));
    DT_ASSERT(pOutData != NULL);

    return DtBcCONSTSINK_GetDataPattern(CONSTSINK_BC, &pOutData->m_DataPattern);
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_OnCmdGetDataRate .-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSINK_OnCmdGetDataRate(
    const DtIoStubBcCONSTSINK* pStub,
    DtIoctlConstSinkCmdGetDataRateOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));
    DT_ASSERT(pOutData != NULL);

    return DtBcCONSTSINK_GetDataRate(CONSTSINK_BC, &pOutData->m_DataRate);
}

//-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCONSTSINK_OnCmdGetOperationalMode(
    const DtIoStubBcCONSTSINK* pStub,
    DtIoctlConstSinkCmdGetOpModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));
    DT_ASSERT(pOutData != NULL);

    return DtBcCONSTSINK_GetOperationalMode(CONSTSINK_BC, &pOutData->m_OpMode);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_OnCmdGetTestInterval -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSINK_OnCmdGetTestInterval(
    const DtIoStubBcCONSTSINK* pStub,
    DtIoctlConstSinkCmdGetTestIntervalOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));
    DT_ASSERT(pOutData != NULL);

    return DtBcCONSTSINK_GetTestInterval(CONSTSINK_BC, &pOutData->m_TestItv);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_OnCmdGetUnderflowCnt .-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSINK_OnCmdGetUnderflowCnt(
    const DtIoStubBcCONSTSINK* pStub,
    DtIoctlConstSinkCmdGetUnderflowCntOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));
    DT_ASSERT(pOutData != NULL);

    return DtBcCONSTSINK_GetUnderflowCnt(CONSTSINK_BC, &pOutData->m_UnderflowCnt);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-. DtIoStubBcCONSTSINK_OnCmdRunTest .-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSINK_OnCmdRunTest(
    const DtIoStubBcCONSTSINK* pStub,
    const DtIoctlConstSinkCmdRunTestInput* pInData,
    DtIoctlConstSinkCmdRunTestOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));
    DT_ASSERT(pInData != NULL);
    DT_ASSERT(pOutData != NULL);

    return DtBcCONSTSINK_RunTest(CONSTSINK_BC, pInData->m_DataRate, pInData->m_TestItv,
                                    &pOutData->m_UnderflowCnt, &pOutData->m_DataErrorCnt);

}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_OnCmdSetCheckData .-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSINK_OnCmdSetCheckData(
    const DtIoStubBcCONSTSINK* pStub,
    const DtIoctlConstSinkCmdSetCheckDataInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));
    DT_ASSERT(pInData != NULL);

    return DtBcCONSTSINK_SetCheckData(CONSTSINK_BC, pInData->m_CheckData);
}

//.--.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_OnCmdSetDataPattern .-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSINK_OnCmdSetDataPattern(
    const DtIoStubBcCONSTSINK* pStub,
    const DtIoctlConstSinkCmdSetDataPatternInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));
    DT_ASSERT(pInData != NULL);

    return DtBcCONSTSINK_SetDataPattern(CONSTSINK_BC, pInData->m_DataPattern);
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_OnCmdSetDataRate .-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSINK_OnCmdSetDataRate(
    const DtIoStubBcCONSTSINK* pStub,
    const DtIoctlConstSinkCmdSetDataRateInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));
    DT_ASSERT(pInData != NULL);

    return DtBcCONSTSINK_SetDataRate(CONSTSINK_BC, pInData->m_DataRate);
}

//-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_OnCmdSetOperationalMode .-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSINK_OnCmdSetOperationalMode(
    const DtIoStubBcCONSTSINK* pStub,
    const DtIoctlConstSinkCmdSetOpModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));
    DT_ASSERT(pInData != NULL);

    return DtBcCONSTSINK_SetOperationalMode(CONSTSINK_BC, pInData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_OnCmdSetTestInterval .-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcCONSTSINK_OnCmdSetTestInterval(
    const DtIoStubBcCONSTSINK* pStub,
    const DtIoctlConstSinkCmdSetTestIntervalInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));
    DT_ASSERT(pInData != NULL);

    return DtBcCONSTSINK_SetTestInterval(CONSTSINK_BC, pInData->m_TestItv);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_OnCmdStartTestInterval -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCONSTSINK_OnCmdStartTestInterval(
    const DtIoStubBcCONSTSINK* pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));

    return DtBcCONSTSINK_StartTestInterval(CONSTSINK_BC);
}

//-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK_OnCmdWaitForTestItvDone -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcCONSTSINK_OnCmdWaitForTestItvDone(
    const DtIoStubBcCONSTSINK* pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcCONSTSINK));

    return DtBcCONSTSINK_WaitForTestIntervalDone(CONSTSINK_BC);
}
