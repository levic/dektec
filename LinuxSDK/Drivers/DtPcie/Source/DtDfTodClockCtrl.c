// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfTodClockCtrl.c *#*#*#*#*#*#*#*#*# (C) 2021 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2021 DekTec Digital Video B.V.
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
#include "DtDfTodClockCtrl.h"
#include "DtCfTod.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        
#define BLKCTRL_ROLE_NONE      NULL
#define  DIV64(x, y)      DtDivideS64((x), (y))
#define  MOD64(x, y)      DtModuloS64((x), (y))

static const Int64  Exp12 = 1000LL*1000*1000*1000; 
static const Int64  Exp9 = 1000LL*1000*1000; 
static const Int64  Exp6 = 1000LL*1000; 

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfTodClockCtrl implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiTx function
#define DF_TODCLOCKCTRL_DEFAULT_PRECONDITIONS(pDf)      \
                             DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfTodClockCtrl))

// MACRO that checks the function has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_TODCLOCKCTRL_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(TODCLOCKCTRL, pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfTodClockCtrl_Init(DtDf*);

static DtStatus  DtDfTodClockCtrl_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfTodClockCtrl_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfTodClockCtrl_OnEnablePreChildren(DtDf*, Bool  Enable);
static void  DtDfTodClockCtrl_ControlThreadEntry(DtThread*, void* pContext);
static void DtDfTodClockCtrl_TodFreqControlInit(DtDfTodClockCtrl*);
static void DtDfTodClockCtrl_TodFreqControlCrashLock(DtDfTodClockCtrl*);
static void DtDfTodClockCtrl_TodFreqControlLocked(DtDfTodClockCtrl*);
static void  DtDfTodClockCtrl_EnablePeriodicIntervalHandler(DtDfTodClockCtrl*, Bool);
static void  DtDfTodClockCtrl_PeriodicIntervalHandler(DtObject*, DtTodTime  Time);
static Bool DtDfTodClockCtrl_GetTimeStamps(DtDfTodClockCtrl*, Int64* pTodTimeNs,
                                                                       Int64* pRefTimeNs);
static Int64 DtDfTodClockCtrl_GetRefTimeStamp(DtDfTodClockCtrl*);
static void DtDfTodClockCtrl_AddTimeSample(DtDfTodClockCtrl*, DtDfTodClockCtrlTimeSamps*, 
                                                        Int64 TodTimeNs, Int64 RefTimeNs);
static void DtDfTodClockCtrl_ClearTimeSamps(DtDfTodClockCtrlTimeSamps*);
static DtStatus DtDfTodClockCtrl_SetSampBufSize(DtDfTodClockCtrlTimeSamps*, Int);
static Int64 DtDfTodClockCtrl_GetTotalTodTime(DtDfTodClockCtrlTimeSamps*);
static Int64 DtDfTodClockCtrl_GetTotalRefTime(DtDfTodClockCtrlTimeSamps*);
static Int64 DtDfTodClockCtrl_GetPhaseDifference(DtDfTodClockCtrlTimeSamps*);
static void DtDfTodClockCtrl_AdjustTodClockPhase(DtDfTodClockCtrl*, Int64);
static void DtDfTodClockCtrl_SetTodClockFrequency(DtDfTodClockCtrl*, Int64);

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtDfTodClockCtrl - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfTodClockCtrl_Close(DtDf*  pDf)
{
    DF_TODCLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    // Unregister periodic interrupt handler
    DtCore_TOD_PeriodicItvUnregister(pDf->m_pCore, (DtObject*)pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfTodClockCtrl*  DtDfTodClockCtrl_Open(DtCore* pCore, DtPt* pPt, const char* pRole, 
                                                  Int Instance, Int Uuid, Bool CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_TODCLOCKCTRL_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfTodClockCtrl, Id, DT_FUNC_TYPE_TODCLKCTRL, 
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfTodClockCtrl_Close;
    OpenParams.m_InitFunc = DtDfTodClockCtrl_Init;
    OpenParams.m_OnCloseFileFunc = DtDfTodClockCtrl_OnCloseFile;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfTodClockCtrl_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfTodClockCtrl_OnEnablePreChildren;
    
    // Use base function to allocate and perform standard initialization of function data
    return (DtDfTodClockCtrl*)DtDf_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_GetTodReference -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfTodClockCtrl_GetTodReference(DtDfTodClockCtrl* pDf, Int* pTodRef)
{
    // Sanity check
    DF_TODCLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pTodRef == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_TODCLOCKCTRL_MUST_BE_ENABLED(pDf);

    // Return cached reference
    DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
    *pTodRef = pDf->m_TodReference;
    DtSpinLockRelease(&pDf->m_PerItvSpinLock);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_GetState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfTodClockCtrl_GetState(DtDfTodClockCtrl* pDf, Int* pState, 
                                               Int* pDeviationPpm, Int* pTodRef,
                                               Int64* pTodTimestamp, Int64* pRefTimestamp)
{
    // Sanity check
    DF_TODCLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (pState==NULL || pDeviationPpm==NULL || pTodTimestamp==NULL || pRefTimestamp==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_TODCLOCKCTRL_MUST_BE_ENABLED(pDf);

    DtSpinLockAcquire(&pDf->m_PerItvSpinLock);

    // Return state
    switch (pDf->m_State)
    {
    default: DT_ASSERT(FALSE);
    case DT_DF_TODCLOCKCTRL_STATE_INITIAL:
        *pState = DT_TODCLOCKCTRL_STATE_LOCKING;
        break;
    case DT_DF_TODCLOCKCTRL_STATE_CRASH_LOCK:
        *pState = DT_TODCLOCKCTRL_STATE_LOCKING;
        break;
    case DT_DF_TODCLOCKCTRL_STATE_LOCKED:
        *pState = DT_TODCLOCKCTRL_STATE_LOCKED;
        break;
    case DT_DF_TODCLOCKCTRL_STATE_FREE_RUNNING:
        *pState = DT_TODCLOCKCTRL_STATE_FREE_RUN;
        break;
    case DT_DF_TODCLOCKCTRL_STATE_INVALID_REF:
        *pState = DT_TODCLOCKCTRL_STATE_INVALID_REF;
        break;
    }
    *pDeviationPpm = pDf->m_RefDeviationPpm;
    *pTodRef = pDf->m_TodReference;
    DtDfTodClockCtrl_GetTimeStamps(pDf, pTodTimestamp, pRefTimestamp);

    DtSpinLockRelease(&pDf->m_PerItvSpinLock);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_SetTodReference -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfTodClockCtrl_SetTodReference(DtDfTodClockCtrl* pDf, Int TodRef)
{
    // Sanity check
    DF_TODCLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (TodRef!=DT_TODCLOCKCTRL_REF_INTERNAL && TodRef!=DT_TODCLOCKCTRL_REF_STEADYCLOCK)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_TODCLOCKCTRL_MUST_BE_ENABLED(pDf);

#ifdef LINBUILD
// Linux CLOCK_MONOTONIC available from kernel 3.17.8
#if LINUX_VERSION_CODE < KERNEL_VERSION(3,17,8)
     #warning IOCONFIG_TODREF_MONOTONIC not supported for kernel version < 3.17.8.
    if (TodRef != DT_TODCLOCKCTRL_REF_INTERNAL)
        return DT_STATUS_NOT_SUPPORTED;
    else
        return DT_STATUS_OK;
#endif
#endif

    DtSpinLockAcquire(&pDf->m_PerItvSpinLock);

    // Don't restart synchronization when already locking/locked to this reference
    if (pDf->m_TodReference!=TodRef || pDf->m_State==DT_DF_TODCLOCKCTRL_STATE_INVALID_REF)
    {
        pDf->m_State = DT_DF_TODCLOCKCTRL_STATE_INITIAL; 
        pDf->m_TodReference = TodRef;
    }

    DtSpinLockRelease(&pDf->m_PerItvSpinLock);

    return DT_STATUS_OK;
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtDfTodClockCtrl - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+
//

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfTodClockCtrl_Init(DtDf* pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32 SysClockFreqHz, Accuracy;
    Int IntervalMs;
    DtDfTodClockCtrl* pDf = (DtDfTodClockCtrl*)pDfBase;
    DtOnPeriodicIntervalRegData  RegData;

    // Sanity checks
    DF_TODCLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);


    // Initialize state and measurement data
    pDf->m_State = DT_DF_TODCLOCKCTRL_STATE_INITIAL; 
    pDf->m_TodReference = DT_TODCLOCKCTRL_REF_INTERNAL;
    pDf->m_RefDeviationPpm = 0;
    pDf->m_pCfTod = (DtCfTod*)DtCore_DF_Find(pDf->m_pCore, NULL, 
                                                               DT_FUNC_TYPE_CF_TOD, NULL);
    if (pDf->m_pCfTod == NULL)
    {
        DtDbgOutDf(ERR, TODCLOCKCTRL, pDf, "ERROR: no CfTod found");
        return DT_STATUS_CONFIG_ERROR;
    }

    DT_RETURN_ON_ERROR(DtCfTod_GetProperties(pDf->m_pCfTod, &SysClockFreqHz, &Accuracy));
    pDf->m_SysClockFreqMilliHz = SysClockFreqHz*1000LL;
    pDf->m_TodClockFreqMilliHz = SysClockFreqHz*1000LL;

    // Set control parameters dependent on interval period
    DT_RETURN_ON_ERROR(DtCfTod_GetPeriodicItv(pDf->m_pCfTod, &IntervalMs));
    pDf->m_ControlPars.m_FreqMult = (1600*IntervalMs)/1000;
    pDf->m_ControlPars.m_PhaseMult = (18000*IntervalMs)/1000;;
    pDf->m_ControlPars.m_MaxStepPpt = 5000;
    pDf->m_ControlPars.m_MaxPhaseDiffNs = 20000;    // 20 microseconds
    pDf->m_ControlPars.m_MaxDeviationPpm = 250;     // Maximum 250ppm deviation allowed

    // Buffer one second of samples
    DT_RETURN_ON_ERROR(DtDfTodClockCtrl_SetSampBufSize(&pDf->m_TimeSamples, 
                                                                         1000/IntervalMs));
    DtDfTodClockCtrl_ClearTimeSamps(&pDf->m_TimeSamples);

    // Initialize the helper thread
    DT_RETURN_ON_ERROR(DtThreadInit(&pDf->m_ControlThread, 
                                              DtDfTodClockCtrl_ControlThreadEntry, pDf));
    DtEventInit(&pDf->m_NewTimeSampEvent, TRUE);


    // Initialize the periodic interval handler spinlock and enable flag
    DtSpinLockInit(&pDf->m_PerItvSpinLock);
    pDf->m_PerItvEnable = FALSE;

    // Register periodic interval handler
    RegData.m_OnPeriodicFunc = DtDfTodClockCtrl_PeriodicIntervalHandler;
    RegData.m_pObject = (DtObject*)pDf;
    Status = DtCore_TOD_PeriodicItvRegister(pDf->m_pCore, &RegData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, TODCLOCKCTRL, pDf, "ERROR: failed to register perioditv handler");
        return Status;
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfTodClockCtrl_OnEnablePostChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfTodClockCtrl* pDf = (DtDfTodClockCtrl*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
     
    // Sanity check
    DF_TODCLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutDf(AVG, TODCLOCKCTRL, pDf, "DISABLE -> ENABLE");

        // Start the control thread
        pDf->m_StopControlThread = FALSE;
        DtEventReset(&pDf->m_NewTimeSampEvent);
        Status = DtThreadStart(&pDf->m_ControlThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, TODCLOCKCTRL, pDf, "ERROR: failed to start HelperThread");
            return Status;
        }

        // Enable periodic interval handler
        DtDfTodClockCtrl_EnablePeriodicIntervalHandler(pDf, TRUE);
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_OnEnablePreChildren -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfTodClockCtrl_OnEnablePreChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfTodClockCtrl* pDf = (DtDfTodClockCtrl*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
    // Sanity check
    DF_TODCLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);
    if (!Enable)
    {
        // ENABLE -> DISABLE
        DtDbgOutDf(AVG, TODCLOCKCTRL, pDf, "ENABLE -> DISABLE");

        // Disable periodic interval handler
        DtDfTodClockCtrl_EnablePeriodicIntervalHandler(pDf, FALSE);

        // Stop the control thread
        pDf->m_StopControlThread = TRUE;
        // Thread wake-up through new time sample event
        DtEventSet(&pDf->m_NewTimeSampEvent);
        Status = DtThreadStop(&pDf->m_ControlThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, TODCLOCKCTRL, pDf, "ERROR: failed to stop ControlThread");
            return Status;
        }
    }

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtDfTodClockCtrl_OnCloseFile(DtDf* pDfBase, const DtFileObject* pFile)
{
    DtDfTodClockCtrl* pDf = (DtDfTodClockCtrl*)pDfBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    // Sanity checks
    DF_TODCLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to release exclusive access
    return DtDf_OnCloseFile((DtDf*)pDf, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_ControlThreadEntry -.-.-.-.-.-.-.-.-.-.-.-.-
//
//
void  DtDfTodClockCtrl_ControlThreadEntry(DtThread* pThread, void* pContext)
{
    DtDfTodClockCtrl*  pDf = (DtDfTodClockCtrl*)pContext;
    Bool  StopThread = FALSE;
    const Int  LoopTimeOut = 100;   // 100ms timeout

    while (!StopThread)
    {
        // Wait for start-of-frame event
        DtStatus Status = DtEventWaitUnInt(&pDf->m_NewTimeSampEvent, LoopTimeOut);
        if ((!DT_SUCCESS(Status) && Status!=DT_STATUS_TIMEOUT) || pThread->m_StopThread 
                                                              || pDf->m_StopControlThread)
        {
            // Fatal error occurred or stop request received
            if (!DT_SUCCESS(Status))
                DtDbgOutDf(ERR, TODCLOCKCTRL, pDf, "ERROR: ControlThread failure");
            if (pThread->m_StopThread || pDf->m_StopControlThread)
                DtDbgOutDf(AVG, TODCLOCKCTRL, pDf, "ControlThread stop request");
            StopThread = TRUE;
            continue;
        }

        switch (pDf->m_State)
        {
        default:
        case DT_DF_TODCLOCKCTRL_STATE_INITIAL:
            DtDfTodClockCtrl_TodFreqControlInit(pDf);
            break;
        case DT_DF_TODCLOCKCTRL_STATE_CRASH_LOCK:
            DtDfTodClockCtrl_TodFreqControlCrashLock(pDf);
            break;
        case DT_DF_TODCLOCKCTRL_STATE_LOCKED:
             DtDfTodClockCtrl_TodFreqControlLocked(pDf);
            break;
        case DT_DF_TODCLOCKCTRL_STATE_INVALID_REF:
        case DT_DF_TODCLOCKCTRL_STATE_FREE_RUNNING:
            // Nothing to do
            break;
        }
    }
    // We have to wait until the thread received a stop command.
    DtThreadWaitForStop(pThread);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_TodFreqControlInit -.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfTodClockCtrl_TodFreqControlInit(DtDfTodClockCtrl* pDf)
{
    DtSpinLockAcquire(&pDf->m_PerItvSpinLock);

    // Clear all historical timestamps
    DtDfTodClockCtrl_ClearTimeSamps(&pDf->m_TimeSamples);

    if (pDf->m_TodReference == DT_TODCLOCKCTRL_REF_INTERNAL)
    {
        // Free running set to nominal frequency
        pDf->m_RefDeviationPpm = 0;
        pDf->m_TodClockFreqMilliHz = pDf->m_SysClockFreqMilliHz;
        DtDfTodClockCtrl_SetTodClockFrequency(pDf, pDf->m_TodClockFreqMilliHz);

        DtDbgOutDf(MIN, TODCLOCKCTRL, pDf, "State => FREE RUNNING");
        pDf->m_State = DT_DF_TODCLOCKCTRL_STATE_FREE_RUNNING;
    }
    else
    {
        // Start with crash lock
        DtDbgOutDf(MIN, TODCLOCKCTRL, pDf, "State => CRASH LOCK");
        pDf->m_State = DT_DF_TODCLOCKCTRL_STATE_CRASH_LOCK;
    }
    DtSpinLockRelease(&pDf->m_PerItvSpinLock);
}

// .-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_TodFreqControlCrashLock -.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfTodClockCtrl_TodFreqControlCrashLock(DtDfTodClockCtrl* pDf)
{
    Int64 TotalRefTime, TotalTodTime, FreqDiffPpb, PhaseDiffNs;
    Int64 FreqDiffMilliHz, NewTodClockFreqMilliHz;

    // Get measurement data under spinlock
    DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
    TotalTodTime = DtDfTodClockCtrl_GetTotalTodTime(&pDf->m_TimeSamples);
    TotalRefTime = DtDfTodClockCtrl_GetTotalRefTime(&pDf->m_TimeSamples);
    PhaseDiffNs = DtDfTodClockCtrl_GetPhaseDifference(&pDf->m_TimeSamples);
    DtSpinLockRelease(&pDf->m_PerItvSpinLock);

    // There must be at least 500ms of samples available before performing the crash lock.
    if (TotalRefTime < 500*Exp6)
        return;

    // Compute frequency difference
    FreqDiffPpb = DIV64((TotalRefTime-TotalTodTime)*Exp9, TotalRefTime);

    // Compute new TOD clock frequency
    FreqDiffMilliHz = pDf->m_TodClockFreqMilliHz * FreqDiffPpb;
    FreqDiffMilliHz = DIV64(FreqDiffMilliHz, Exp9);
    NewTodClockFreqMilliHz = pDf->m_TodClockFreqMilliHz - FreqDiffMilliHz;

    // Compute reference deviation
    pDf->m_RefDeviationPpm = (Int)(DIV64(
                               (NewTodClockFreqMilliHz - pDf->m_SysClockFreqMilliHz)*Exp6,
                               pDf->m_SysClockFreqMilliHz));
    // Check deviation
    if (pDf->m_RefDeviationPpm < -pDf->m_ControlPars.m_MaxDeviationPpm 
                          || pDf->m_RefDeviationPpm> pDf->m_ControlPars.m_MaxDeviationPpm)
    {
        // Deviation is too much
        DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
        if (pDf->m_State == DT_DF_TODCLOCKCTRL_STATE_CRASH_LOCK)
        {
            DtDbgOutDf(ERR, TODCLOCKCTRL, pDf, "State => INVALID_REF");
            pDf->m_State = DT_DF_TODCLOCKCTRL_STATE_INVALID_REF;
        }
        DtSpinLockRelease(&pDf->m_PerItvSpinLock);
        return;
    } 

    // Adjust for phase difference
    DtDfTodClockCtrl_AdjustTodClockPhase(pDf, PhaseDiffNs);

    // Set new frequency
    pDf->m_TodClockFreqMilliHz = NewTodClockFreqMilliHz;
    DtDfTodClockCtrl_SetTodClockFrequency(pDf, pDf->m_TodClockFreqMilliHz);

    // State change
    DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
    if (pDf->m_State == DT_DF_TODCLOCKCTRL_STATE_CRASH_LOCK)
    {
        // Restart measurements
        DtDfTodClockCtrl_ClearTimeSamps(&pDf->m_TimeSamples);
        DtDbgOutDf(MIN, TODCLOCKCTRL, pDf, "State => LOCKED");
        pDf->m_State = DT_DF_TODCLOCKCTRL_STATE_LOCKED;
    }
    DtSpinLockRelease(&pDf->m_PerItvSpinLock);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_TodFreqControlLocked -.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfTodClockCtrl_TodFreqControlLocked(DtDfTodClockCtrl* pDf)
{
    Int64  PhaseDiffNs, TotalRefTime, TotalTodTime, FreqDiffMilliHz, FreqDiffPpb;
    Int64  FreqAdjustPpt, FreqAdjustMilliHz, NewTodClockFreqMilliHz;
    Int PhaseBoost;

    // Get measurement data under spinlock
    DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
    PhaseDiffNs = DtDfTodClockCtrl_GetPhaseDifference(&pDf->m_TimeSamples);
    TotalRefTime = DtDfTodClockCtrl_GetTotalRefTime(&pDf->m_TimeSamples);
    TotalTodTime = DtDfTodClockCtrl_GetTotalTodTime(&pDf->m_TimeSamples);
    DtSpinLockRelease(&pDf->m_PerItvSpinLock);

    // There must be at least 100ms of samples available before locking.
    if (TotalRefTime < 100*Exp6)
        return;

    // Check phase difference
    if (PhaseDiffNs<-pDf->m_ControlPars.m_MaxPhaseDiffNs
                                       || PhaseDiffNs>pDf->m_ControlPars.m_MaxPhaseDiffNs)
    {
        // The phase difference is too much
        DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
        if (pDf->m_State == DT_DF_TODCLOCKCTRL_STATE_LOCKED)
        {
            // Perform crash lock
            DtDbgOutDf(MIN, TODCLOCKCTRL, pDf, "State => CRASH LOCK");
            pDf->m_State = DT_DF_TODCLOCKCTRL_STATE_CRASH_LOCK;
        }
        DtSpinLockRelease(&pDf->m_PerItvSpinLock);
        return;
    }

    // Compute frequency difference in milli Hz
    FreqDiffPpb = DIV64((TotalRefTime-TotalTodTime)*Exp9, TotalRefTime);
    FreqDiffMilliHz = pDf->m_TodClockFreqMilliHz * FreqDiffPpb;
    FreqDiffMilliHz = DIV64(FreqDiffMilliHz, Exp9);

    // Use phase boost to reduce the phase error to less than 10 us
    PhaseBoost = 1;
    if (PhaseDiffNs<-10000 || PhaseDiffNs>10000)
        PhaseBoost = 5;

    // Compute frequency adjustment
    FreqAdjustPpt = DIV64(FreqDiffMilliHz*pDf->m_ControlPars.m_FreqMult, 1000)
              + DIV64((Int64)PhaseBoost*PhaseDiffNs*pDf->m_ControlPars.m_PhaseMult, 1000);

    // Clip the adjustment to maximum step
    if (FreqAdjustPpt > pDf->m_ControlPars.m_MaxStepPpt)
        FreqAdjustPpt = pDf->m_ControlPars.m_MaxStepPpt;
    else if (FreqAdjustPpt < -pDf->m_ControlPars.m_MaxStepPpt)
        FreqAdjustPpt = -pDf->m_ControlPars.m_MaxStepPpt;

    // Compute new frequency
    FreqAdjustMilliHz = pDf->m_TodClockFreqMilliHz * FreqAdjustPpt;
    FreqAdjustMilliHz = DIV64(FreqAdjustMilliHz, Exp12);
    NewTodClockFreqMilliHz = pDf->m_TodClockFreqMilliHz - FreqAdjustMilliHz;

    // Compute and check reference deviation
    pDf->m_RefDeviationPpm = (Int)(DIV64(
                               (NewTodClockFreqMilliHz - pDf->m_SysClockFreqMilliHz)*Exp6,
                               pDf->m_SysClockFreqMilliHz));
    if (pDf->m_RefDeviationPpm < -pDf->m_ControlPars.m_MaxDeviationPpm 
                          || pDf->m_RefDeviationPpm> pDf->m_ControlPars.m_MaxDeviationPpm)
    {
        // Deviation is too much; 
        DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
        if (pDf->m_State == DT_DF_TODCLOCKCTRL_STATE_LOCKED)
        {
            DtDbgOutDf(ERR, TODCLOCKCTRL, pDf, "State => INVALID_REF");
            pDf->m_State = DT_DF_TODCLOCKCTRL_STATE_INVALID_REF;
        }
        DtSpinLockRelease(&pDf->m_PerItvSpinLock);
    } else {
        // Deviation is OK; set new frequency
        pDf->m_TodClockFreqMilliHz = NewTodClockFreqMilliHz;
        DtDfTodClockCtrl_SetTodClockFrequency(pDf, pDf->m_TodClockFreqMilliHz);
    }
}


// -.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_EnablePeriodicIntervalHandler -.-.-.-.-.-.-.-.-.-.
//
void DtDfTodClockCtrl_EnablePeriodicIntervalHandler(DtDfTodClockCtrl* pDf, Bool Enable)
{
    // Enable/disable handling periodic interval
    // A spinlock is used to prevent that handling is disabled while the
    // periodic interval handler is running
    DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
    pDf->m_PerItvEnable = Enable;
    DtSpinLockRelease(&pDf->m_PerItvSpinLock);
}

// .-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_PeriodicIntervalHandler -.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfTodClockCtrl_PeriodicIntervalHandler(DtObject* pObj, DtTodTime  Time)
{
    DtDfTodClockCtrl*  pDf = (DtDfTodClockCtrl*)pObj;
    DF_TODCLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    DtSpinLockAcquireAtDpc(&pDf->m_PerItvSpinLock);

    // Only when enabled and external reference used
    if (pDf->m_PerItvEnable)
    {
        Int64 TodTimeNs, RefTimeNs;
        if (DtDfTodClockCtrl_GetTimeStamps(pDf, &TodTimeNs, &RefTimeNs))
        {
            DtDfTodClockCtrl_AddTimeSample(pDf, &pDf->m_TimeSamples, TodTimeNs,RefTimeNs);
            DtEventSet(&pDf->m_NewTimeSampEvent);
        }
    }
    DtSpinLockReleaseFromDpc(&pDf->m_PerItvSpinLock);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_GetTimeStamps -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool DtDfTodClockCtrl_GetTimeStamps(DtDfTodClockCtrl* pDf, Int64* pTodTimeNs,
                                                                        Int64* pRefTimeNs)
{
    // We need timestamps with 10us accuracy or better
    const Int MaxTimeDiffNs = 10000;
    DtTodTime TodTime;
    Int64 RefTimeNs1, RefTimeNs2;
    Int  Count = 0;

    // Special case when using internal reference
    if (pDf->m_TodReference == DT_TODCLOCKCTRL_REF_INTERNAL)
    {
        *pTodTimeNs = *pRefTimeNs = DtDfTodClockCtrl_GetRefTimeStamp(pDf);
        return TRUE;
    }

    // Get two accurate timestamps
    do
    {
        // First reference timestamp
        RefTimeNs1 = DtDfTodClockCtrl_GetRefTimeStamp(pDf);
        // TOD timestamp
        DtCore_TOD_GetTime(pDf->m_pCore, &TodTime);
        // Second reference timestamp
        RefTimeNs2 = DtDfTodClockCtrl_GetRefTimeStamp(pDf);;
    } while (Count<10 && (RefTimeNs2-RefTimeNs1) > MaxTimeDiffNs);

    if (Count > 10)
        return FALSE;   // No valid timestamp found

    *pTodTimeNs = TodTime.m_Seconds*1000LL*1000*1000 + TodTime.m_Nanoseconds;
    *pRefTimeNs = DIV64(RefTimeNs1+RefTimeNs2, 2);
    return TRUE;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_GetRefTimeStamp -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int64 DtDfTodClockCtrl_GetRefTimeStamp(DtDfTodClockCtrl* pDf)
{
    switch(pDf->m_TodReference)
    {
    case DT_TODCLOCKCTRL_REF_INTERNAL:
    {
        DtTodTime TodTime;
        DtCore_TOD_GetTime(pDf->m_pCore, &TodTime);
        return TodTime.m_Seconds*1000LL*1000*1000 + TodTime.m_Nanoseconds;
    }
    break;
    case DT_TODCLOCKCTRL_REF_STEADYCLOCK:
    {
#ifdef WINBUILD
        // Windows uses the QueryPerfermanceCounter
        LARGE_INTEGER  Freq, Counter;
        Int Seconds, NanoSeconds, Remainder;
        Counter = KeQueryPerformanceCounter(&Freq);
        Seconds = (Int)DIV64(Counter.QuadPart, Freq.QuadPart);
        Remainder = (Int)MOD64(Counter.QuadPart, Freq.QuadPart);
        NanoSeconds = (Int)DIV64(Remainder*1000000000LL, Freq.QuadPart);
        return Seconds*1000LL*1000*1000 + NanoSeconds;
#else
        // Linux uses the CLOCK_MONOTONIC available from kernel 3.17.8
        #if LINUX_VERSION_CODE >= KERNEL_VERSION(3,17,8)
            return ktime_get_raw_ns();
        #else
            return 0;
        #endif
#endif
    }
    break;
    default: DT_ASSERT(FALSE); return 0;
    }
}

// .-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_FramePeriodMeasureClear -.-.-.-.-.-.-.-.-.-.-.-
//
//  Caller should have spinlock
//
void DtDfTodClockCtrl_ClearTimeSamps(DtDfTodClockCtrlTimeSamps* pTimeSamps)
{
    pTimeSamps->m_Index = 0;
    pTimeSamps->m_NumValid = 0;
    // pTimeSamps->m_BufSize is not changed
    pTimeSamps->m_TotalTod = 0;
    pTimeSamps->m_TotalRef = 0;
    pTimeSamps->m_PrevTod = -1;
    pTimeSamps->m_PrevRef = -1;
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_SetSampBufSize -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfTodClockCtrl_SetSampBufSize(DtDfTodClockCtrlTimeSamps* pTimeSamps, Int Size)
{
    if (Size < 2)
        return DT_STATUS_BUF_TOO_SMALL;
    if (Size > DT_DF_TODCLOCKCTRL_MAX_NUM_TIME_SAMPLES)
        return DT_STATUS_BUF_TOO_LARGE;

    pTimeSamps->m_BufSize = Size;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_AddTimeSample -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
//  Caller should have spinlock
//
void DtDfTodClockCtrl_AddTimeSample(DtDfTodClockCtrl* pDf, 
                  DtDfTodClockCtrlTimeSamps* pTimeSamps, Int64 TodTimeNs, Int64 RefTimeNs)
{
    Int  Index;
    DT_ASSERT(pTimeSamps != NULL);
    DT_ASSERT(pTimeSamps->m_BufSize>=2 
                       && pTimeSamps->m_BufSize<=DT_DF_TODCLOCKCTRL_MAX_NUM_TIME_SAMPLES);

    // No previous samples available?
    if (pTimeSamps->m_PrevRef==-1 || pTimeSamps->m_PrevRef==-1)
    {
        pTimeSamps->m_PrevTod = TodTimeNs;
        pTimeSamps->m_PrevRef = RefTimeNs;
        return;
    }

    if (pTimeSamps->m_BufSize > 0)
    {
        Int64  IntervalTod =  TodTimeNs - pTimeSamps->m_PrevTod;
        Int64  IntervalRef =  RefTimeNs - pTimeSamps->m_PrevRef;
        pTimeSamps->m_PrevTod = TodTimeNs;
        pTimeSamps->m_PrevRef = RefTimeNs;

        // Check validity of samples
        if (IntervalTod<0 || IntervalRef<0)
        {
            DtDfTodClockCtrl_ClearTimeSamps(pTimeSamps);
            return;
        }

        // If buffer is full => subtract eldest sample
        Index = pTimeSamps->m_Index;
        if (pTimeSamps->m_NumValid == pTimeSamps->m_BufSize)
        {
            pTimeSamps->m_TotalTod -= pTimeSamps->m_Tod[Index];
            pTimeSamps->m_TotalRef -= pTimeSamps->m_Ref[Index];
        }
        else // Buffer not full => we have an extra sample
            pTimeSamps->m_NumValid++;

        // Store time Interval
        pTimeSamps->m_Tod[Index] = IntervalTod;
        pTimeSamps->m_Ref[Index] = IntervalRef;
        pTimeSamps->m_TotalTod += IntervalTod;
        pTimeSamps->m_TotalRef += IntervalRef;

        // Update index
        pTimeSamps->m_Index = (Index+1) % pTimeSamps->m_BufSize;
    }
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_GetTotalTodTime -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int64 DtDfTodClockCtrl_GetTotalTodTime(DtDfTodClockCtrlTimeSamps* pTimeSamps)
{
    return pTimeSamps->m_TotalTod;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_GetTotalRefTime -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int64 DtDfTodClockCtrl_GetTotalRefTime(DtDfTodClockCtrlTimeSamps* pTimeSamps)
{
    return pTimeSamps->m_TotalRef;
}


// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_GetPhaseDifference -.-.-.-.-.-.-.-.-.-.-.-.-
//
//  Caller should have spinlock
//
Int64 DtDfTodClockCtrl_GetPhaseDifference(DtDfTodClockCtrlTimeSamps* pTimeSamps)
{
    return pTimeSamps->m_PrevRef - pTimeSamps->m_PrevTod;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_AdjustTodClockPhase -.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtDfTodClockCtrl_AdjustTodClockPhase(DtDfTodClockCtrl* pDf, Int64 PhaseAdjustNs)
{
    DtCfTod_AdjustTime(pDf->m_pCfTod, PhaseAdjustNs);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl_SetTodClockFrequency -.-.-.-.-.-.-.-.-.-.-.-.
//
void DtDfTodClockCtrl_SetTodClockFrequency(DtDfTodClockCtrl* pDf, Int64 TodFreqMilliHz)
{
    // Compute new clock phase increment
    UInt32 ClockPhaseIncr = (UInt32)DIV64((1LL<<58)*10, DIV64(TodFreqMilliHz,100));
    DtCfTod_SetPhaseIncr(pDf->m_pCfTod, ClockPhaseIncr);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfTodClockCtrl implementation +=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Df's stub
#define TODCLOCKCTRL_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                  DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfTodClockCtrl));
// Helper macro to cast stub's DtDf member to DtDfTodClockCtrl
#define STUB_TODCLOCKCTRL   ((DtIoStubDfTodClockCtrl*)pStub)
#define STUB_DF  ((DtDfTodClockCtrl*)STUB_TODCLOCKCTRL->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus DtIoStubDfTodClockCtrl_OnCmd(const DtIoStub* pStub, 
                                             DtIoStubIoParams* pIoParams, Int * pOutSize);
static DtStatus  DtIoStubDfTodClockCtrl_OnCmdGetState(const DtIoStubDfTodClockCtrl*,
                                                  DtIoctlTodClockCtrlCmdGetStateOutput*);
static DtStatus  DtIoStubDfTodClockCtrl_OnCmdSetReference(const DtIoStubDfTodClockCtrl*,
                                       const DtIoctlTodClockCtrlCmdSetTodReferenceInput*);
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_TODCLOCKCTRL;

static const DtIoctlProperties  IOSTUB_DF_TODCLOCKCTRL_IOCTLS[] = 
{
    DT_IOCTL_PROPS_TODCLOCKCTRL_CMD(
        DtIoStubDfTodClockCtrl_OnCmd, 
        NULL, NULL),
};

// =+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfTodClockCtrl - Public functions +=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTodClockCtrl_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubDfTodClockCtrl_Close(DtIoStub* pStub)
{
    TODCLOCKCTRL_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTodClockCtrl_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubDfTodClockCtrl*  DtIoStubDfTodClockCtrl_Open(DtDf*  pDf)
{
    DtIoStubDfTodClockCtrl*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    DF_TODCLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfTodClockCtrl, pDf, NULL, 
                                                           DtIoStubDfTodClockCtrl_Close,
                                                           NULL,  // Use default IOCTL
                                                           IOSTUB_DF_TODCLOCKCTRL_IOCTLS);
    pStub = (DtIoStubDfTodClockCtrl*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

// +=+=+=+=+=+=+=+=+=+=+ DtIoStubDfTodClockCtrl - Private functions +=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTodClockCtrl_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfTodClockCtrl_OnCmd(
    const DtIoStub*  pStub, 
    DtIoStubIoParams*  pIoParams, 
    Int* pOutSize)
{
    DtStatus Status= DT_STATUS_OK;
    const DtIoctlTodClockCtrlCmdInput*  pInData = NULL;
    DtIoctlTodClockCtrlCmdOutput*  pOutData = NULL;

    TODCLOCKCTRL_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_TODCLOCKCTRL_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, TODCLOCKCTRL, pStub,
                                                   "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_TodClkCtrlCmd;
    if (pIoParams->m_OutReqSize > 0)
    { 
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_TodClkCtrlCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_TODCLOCKCTRL_CMD_GET_STATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfTodClockCtrl_OnCmdGetState(STUB_TODCLOCKCTRL, 
                                                                  &pOutData->m_GetState);
        break;
    case DT_TODCLOCKCTRL_CMD_SET_REFERENCE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubDfTodClockCtrl_OnCmdSetReference(STUB_TODCLOCKCTRL,
                                                                &pInData->m_SetReference);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTodClockCtrl_OnCmdGetState -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfTodClockCtrl_OnCmdGetState(
    const DtIoStubDfTodClockCtrl*  pStub, 
    DtIoctlTodClockCtrlCmdGetStateOutput*  pOutData)
{
    TODCLOCKCTRL_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtDfTodClockCtrl_GetState(STUB_DF, &pOutData->m_TodClockCtrlState, 
                                    &pOutData->m_DeviationPpm, &pOutData->m_TodReference,
                                    &pOutData->m_TodTimestamp, &pOutData->m_RefTimestamp);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTodClockCtrl_OnCmdSetReference -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfTodClockCtrl_OnCmdSetReference(
    const DtIoStubDfTodClockCtrl* pStub,
    const DtIoctlTodClockCtrlCmdSetTodReferenceInput* pInData)
{
    TODCLOCKCTRL_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtDfTodClockCtrl_SetTodReference(STUB_DF, pInData->m_TodReference); 
}