// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfGenLockCtrl.c *#*#*#*#*#*#*#*#*# (C) 2019 DekTec
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2019 DekTec Digital Video B.V.
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
#include "DtDfGenLockCtrl.h"
#include "DtAudioVideo.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        
#define  GENL_ROLE_NONE      NULL
#define  SI534X_ROLE_NONE    NULL
#define  MAX_DEVIATION_PPM   120       // Maximum 120ppm deviation
#define  MAX_CRASHLOCK_STEP_PPM   1    // Maximum 1ppm step
#define  DIV64(x, y)      DtDivideS64((x), (y))
#define  MOD64(x, y)      DtModuloS64((x), (y))

static const Int64  Exp12 = 1000LL*1000LL*1000LL*1000LL; 
static const Int64  Exp11 = 100LL*1000LL*1000LL*1000LL; 
static const Int64  Exp10 = 10LL*1000LL*1000LL*1000LL;  
static const Int64  Exp9 = 1000LL*1000*1000; 
static const Int  Exp8 = 100*1000*1000; 
static const Int  Exp7 = 10*1000*1000; 
static const Int  Exp6 = 1000*1000; 
static const Int  Exp5 = 100*1000; 
static const Int  Exp4 = 10*1000; 
static const Int  Exp3 = 1000; 

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfGenLockCtrl implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiTx function
#define DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf)      \
                              DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfGenLockCtrl))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_GENLOCKCTRL_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(GENLOCKCTRL, pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfGenLockCtrl_Init(DtDf*);
static DtStatus  DtDfGenLockCtrl_LoadParameters(DtDf*);
static DtStatus  DtDfGenLockCtrl_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfGenLockCtrl_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfGenLockCtrl_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfGenLockCtrl_OpenChildren(DtDfGenLockCtrl*);
static Int  DtDfGenLockCtrl_GetCurrentClockIdx(DtDfGenLockCtrl*);
static void  DtDfGenLockCtrl_InitGenRefInfo(DtDfGenLockCtrl*);
static DtStatus DtDfGenLockCtrl_SetVideoStandard(DtDfGenLockCtrl * pDf, Int VidStd);
static void  DtDfGenLockCtrl_DcoControlThreadEntry(DtThread*, void* pContext);
static void  DtDfGenLockCtrl_DcoControl(DtDfGenLockCtrl*,
                             DtDfGenLockCtrlGenRefType, Int  SofOffset, 
                             const DtDfGenLockCtrlSofTods*, 
                             Int64 GenRefAvgFramePeriodPs, const DtTodTime*);
static void  DtDfGenLockCtrl_DcoControlInit(DtDfGenLockCtrl*,
                             DtDfGenLockCtrlDcoState*, DtDfGenLockCtrlGenRefStatus,
                             DtDfGenLockCtrlGenRefType, Int  SofOffset, 
                             const DtDfGenLockCtrlSofTods*, 
                             Int64 GenRefAvgFramePeriodPs, const DtTodTime*);
static void  DtDfGenLockCtrl_DcoControlCrashLockFreq(DtDfGenLockCtrl*,
                             DtDfGenLockCtrlDcoState*, DtDfGenLockCtrlGenRefStatus,
                             DtDfGenLockCtrlGenRefType, Int  SofOffset, 
                             const DtDfGenLockCtrlSofTods*, 
                             Int64 GenRefAvgFramePeriodPs, const DtTodTime*);
static void  DtDfGenLockCtrl_DcoControlCrashLockPhase(DtDfGenLockCtrl*,
                             DtDfGenLockCtrlDcoState*, DtDfGenLockCtrlGenRefStatus,
                             DtDfGenLockCtrlGenRefType, Int  SofOffset, 
                             const DtDfGenLockCtrlSofTods*, 
                             Int64 GenRefAvgFramePeriodPs, const DtTodTime*);
static void  DtDfGenLockCtrl_DcoControlLocked(DtDfGenLockCtrl*,
                             DtDfGenLockCtrlDcoState*, DtDfGenLockCtrlGenRefStatus,
                             DtDfGenLockCtrlGenRefType, Int  SofOffset, 
                             const DtDfGenLockCtrlSofTods*, 
                             Int64 GenRefAvgFramePeriodPs, const DtTodTime*);
static void  DtDfGenLockCtrl_DcoFreeRunning(DtDfGenLockCtrl*,
                             DtDfGenLockCtrlDcoState*, DtDfGenLockCtrlGenRefStatus,
                             DtDfGenLockCtrlGenRefType, Int  SofOffset, 
                             const DtDfGenLockCtrlSofTods*, 
                             Int64 GenRefAvgFramePeriodPs, const DtTodTime*);
static DtDfGenLockCtrlGenRefStatus  DtDfGenLockCtrl_DetermineGenRefStatus(
                                          DtDfGenLockCtrl*, DtDfGenLockCtrlGenRefType,
                                          const DtDfGenLockCtrlSofTods*, 
                                          Int64 GenRefAvgFramePeriodPs, const DtTodTime*);
static void DtDfGenLockCtrl_DcoControlAddDeltaFreq(DtDfGenLockCtrl*, Int DeltaPpt);
static Int DtDfGenLockCtrl_DcoControlGetFrequencyHz(DtDfGenLockCtrl*);
static Int64 DtDfGenLockCtrl_DcoControlGetFrequencyMilliHz(DtDfGenLockCtrl*);
static Bool DtDfGenLockCtrl_DcoControlComputePhaseDiff(DtDfGenLockCtrl*, Int SofOffset,
                                const DtDfGenLockCtrlSofTods*, Int GenRefAvgFramePeriodNs, 
                                const DtTodTime*, Int* pPhaseDiffNs);
static void  DtDfGenLockCtrl_GenLockStartOfFrameHandler(DtObject*, const DtTodTime*);
static void DtDfGenLockCtrl_SofTodsClear(DtDfGenLockCtrlSofTods*);
static void DtDfGenLockCtrl_SofTodsAdd(DtDfGenLockCtrl*, DtDfGenLockCtrlSofTods*,
                                       const DtTodTime * pSofTod, Int ExpectFramePeriod);
static DtTodTime  DtDfGenLockCtrl_SofTodsGetLast(const DtDfGenLockCtrlSofTods*);
static Int  DtDfGenLockCtrl_SofTodsNumAvailable(const DtDfGenLockCtrlSofTods*);
static DtTodTime  DtDfGenLockCtrl_SofTodsGet(const DtDfGenLockCtrlSofTods*, Int Idx);
static Int64  DtDfGenLockCtrl_SofTodsTimeDiff(const DtTodTime*,
                                                           const DtDfGenLockCtrlSofTods*);
static void  DtDfGenLockCtrl_FramePeriodMeasureClear(DtDfGenLockCtrlFramePeriodMeasure*);
static void  DtDfGenLockCtrl_FramePeriodMeasureAdd(DtDfGenLockCtrlFramePeriodMeasure*,
                                                                         Int FramePeriod);
static Int64  DtDfGenLockCtrl_FramePeriodMeasureGetAverage(
                                                const DtDfGenLockCtrlFramePeriodMeasure*);

static DtDfGenLockCtrlDcoControlPars DtDfGenLockCtrl_GetDcoControlPars(Int VidStd);
static Int  DtDfGenLockCtrl_GetGenLockStatusInternal(DtDfGenLockCtrl*);
static void  DtDfGenLockCtrl_NotifyStatusChange(DtDfGenLockCtrl*, Int Status);
static void  DtDfGenLockCtrl_NotifyLockChange(DtDfGenLockCtrl*, Bool Lock);

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtDfGenLockCtrl - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfGenLockCtrl_Close(DtDf*  pDfBase)
{
    DtDfGenLockCtrl* pDf = (DtDfGenLockCtrl*)pDfBase;

    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(!DtDf_IsEnabled(pDfBase));

    // Clean-up vector of GenLock lock changed handlers
    if (pDf->m_pOnLockChangedHandlers != NULL)
    {
        DtVector_Cleanup(pDf->m_pOnLockChangedHandlers);
        pDf->m_pOnLockChangedHandlers = NULL;
    }

    // Let base function perform final clean-up
    DtDf_Close(pDfBase);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfGenLockCtrl*  DtDfGenLockCtrl_Open(DtCore* pCore, DtPt* pPt, const char* pRole, 
                                                  Int Instance, Int Uuid, Bool CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_GENLOCKCTRL_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfGenLockCtrl, Id, DT_FUNC_TYPE_GENLOCKCTRL, pPt,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfGenLockCtrl_Close;
    OpenParams.m_InitFunc = DtDfGenLockCtrl_Init;
    OpenParams.m_LoadParsFunc = DtDfGenLockCtrl_LoadParameters;
    OpenParams.m_OnCloseFileFunc = DtDfGenLockCtrl_OnCloseFile;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfGenLockCtrl_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfGenLockCtrl_OnEnablePreChildren;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfGenLockCtrl*)DtDf_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_GetGenLockState -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfGenLockCtrl_GetGenLockState(DtDfGenLockCtrl* pDf,  Int* pGenLockStatus, 
                     Int* pRefVidStd, Int* pDetVidStd, Int64* pSofCount,
                     Int* pIsSofTodValid, DtTodTime* pLastSofTod, Int* pTimeSinceLastSof)
{

    // Sanity check
    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pGenLockStatus==NULL || pRefVidStd==NULL || pDetVidStd==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_GENLOCKCTRL_MUST_BE_ENABLED(pDf);

    // Get current GenLock state
    DtSpinLockAcquire(&pDf->m_SofSpinLock);
    *pGenLockStatus = DtDfGenLockCtrl_GetGenLockStatusInternal(pDf);
    *pRefVidStd = pDf->m_GenRefVidStd;
    *pDetVidStd = pDf->m_GenRefDetectVidStd;
    *pSofCount = pDf->m_GenRefSofTods.m_TotalNumSofTods;
    if (pDf->m_GenRefSofTods.m_NumSofTods > 0)
    {
        // Return information about last SOF time stamp
        Int64 TimeDiff;
        DtTodTime  CurrTime;
        DtCore_TOD_GetTime(pDf->m_pCore, &CurrTime);
        *pLastSofTod = DtDfGenLockCtrl_SofTodsGetLast(&pDf->m_GenRefSofTods);
        TimeDiff = DtCore_TOD_TimeDiff(CurrTime, *pLastSofTod);
        *pIsSofTodValid =  (TimeDiff < Exp9);
        *pTimeSinceLastSof = (Int)TimeDiff;
    } else {
        // No valid time stamp
        *pIsSofTodValid  = FALSE;
        pLastSofTod->m_Seconds = 0;
        pLastSofTod->m_Nanoseconds = 0;
        *pTimeSinceLastSof = 0;
    }

    DtSpinLockRelease(&pDf->m_SofSpinLock);

    DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "State: %d; CfgVidStd: %d; DetVidStd: %d; SofCount: %d; Valid: %d; Time: %d:%d; TimeDiff: %d", 
                            *pGenLockStatus, *pRefVidStd, *pDetVidStd, *pSofCount, *pIsSofTodValid, 
                            pLastSofTod->m_Seconds, pLastSofTod->m_Nanoseconds/1000000, *pTimeSinceLastSof/1000000);




    return DT_STATUS_OK;
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_IsLocked -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfGenLockCtrl_IsLocked(DtDfGenLockCtrl* pDf,  Bool* pIsLocked)
{
    Int GenLockStatus;
    // Sanity check
    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pIsLocked == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_GENLOCKCTRL_MUST_BE_ENABLED(pDf);

    DtSpinLockAcquire(&pDf->m_SofSpinLock);
    GenLockStatus = DtDfGenLockCtrl_GetGenLockStatusInternal(pDf);
    DtSpinLockRelease(&pDf->m_SofSpinLock);
    
    *pIsLocked = (GenLockStatus==DT_GENLOCKCTRL_STATE_FREE_RUN
                                          ||  GenLockStatus==DT_GENLOCKCTRL_STATE_LOCKED);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_ReLockDtDfGenLockCtrl -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfGenLockCtrl_ReLock(DtDfGenLockCtrl* pDf)
{
    // Sanity check
    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    // Must be enabled
    DF_GENLOCKCTRL_MUST_BE_ENABLED(pDf);

    // Restart locking
    DtSpinLockAcquire(&pDf->m_SofSpinLock);
    pDf->m_DcoControlState = DT_DF_GENLOCKCTRL_STATE_INITIAL;
    DtSpinLockRelease(&pDf->m_SofSpinLock);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_GetDcoClockProperties -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfGenLockCtrl_GetDcoClockProperties(DtDfGenLockCtrl* pDf,
                   Int  MaxNumElems, Int* pNumElems, DtIoctlGenLockCtrlClockProps* pProps)
{
    Int i= 0;
    // Sanity check
    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    // Must be enabled
    DF_GENLOCKCTRL_MUST_BE_ENABLED(pDf);

    // Check parameter
    if (pNumElems==NULL || pProps==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return the number of elems available
    *pNumElems = pDf->m_NumSi534XClockProps;

    // Buffer too small; return number of elements only
    if (MaxNumElems < pDf->m_NumSi534XClockProps)
        return DT_STATUS_OK;

    // Copy the clock properties
    for (i = 0; i<pDf->m_NumSi534XClockProps; i++)
    {
        pProps[i].m_ClockIdx = i;
        pProps[i].m_FrequencyuHz = pDf->m_pSi534XClockProps[i].m_FrequencyuHz;
        switch (pDf->m_pSi534XClockProps[i].m_ClockType)
        {
        case DT_DF_SI534X_CLK_FRACTIONAL:
            pProps[i].m_ClockType = DT_GENLOCKCTR_CLKTYPE_FRACTIONAL;
            break;
        case DT_DF_SI534X_CLK_NON_FRACTIONAL:
            pProps[i].m_ClockType = DT_GENLOCKCTR_CLKTYPE_NON_FRACTIONAL;
            break;
        default:
            DT_ASSERT(FALSE);
            break;
        }
        pProps[i].m_RangePpt = pDf->m_pSi534XClockProps[i].m_RangePpt;
        pProps[i].m_StepSizePpt = pDf->m_pSi534XClockProps[i].m_StepSizePpt;
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_GetDcoFreqOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfGenLockCtrl_GetDcoFreqOffset(DtDfGenLockCtrl* pDf, Int  ClockIdx,
                                          Int* pFreqOffsetPpt, Int64*  pFrequencyMicroHz)
{
    const DtDfSi534XClockProps*  pClkProps=NULL;
    Bool  IsFractional=FALSE;
    Int64  OutFreq=0, BaseFreq = 0, DeltaFreq=0;
    Int  FreqOffsetPpt=0;

    // Sanity check
    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);
    // Must be enabled
    DF_GENLOCKCTRL_MUST_BE_ENABLED(pDf);
    DT_ASSERT(pDf->m_pSi534XClockProps != NULL);

    // Check parameter
    if (pFreqOffsetPpt==NULL || pFrequencyMicroHz==NULL)
        return DT_STATUS_INVALID_PARAMETER;
    if (ClockIdx<-1 || ClockIdx>=pDf->m_NumSi534XClockProps)
        return DT_STATUS_INVALID_PARAMETER;

    // Get properties  under SofSpinLock
    DtSpinLockAcquire(&pDf->m_SofSpinLock);
    if (ClockIdx == -1)
        ClockIdx = DtDfGenLockCtrl_GetCurrentClockIdx(pDf);
    pClkProps = &pDf->m_pSi534XClockProps[ClockIdx];
    IsFractional = (pClkProps->m_ClockType == DT_DF_SI534X_CLK_FRACTIONAL);
    BaseFreq = pClkProps->m_FrequencyuHz;
    DtSpinLockRelease(&pDf->m_SofSpinLock);

    // Get current offset
    DT_RETURN_ON_ERROR(DtDfSi534X_GetFreqOffsetPpt(pDf->m_pDfSi534X, IsFractional, 
                                                                         &FreqOffsetPpt));
    // Compute output frequency in micro Hertz
    if  (FreqOffsetPpt < 0)
    { 
        DT_ASSERT(DIV64(BaseFreq, Exp9)<(3*Exp9) && -FreqOffsetPpt<(3*Exp9));
        DeltaFreq =  DIV64(BaseFreq, Exp9) * -FreqOffsetPpt;
        DeltaFreq += DIV64(MOD64(BaseFreq,Exp9) * -FreqOffsetPpt + 5*Exp8, Exp9);
        DeltaFreq = DIV64(DeltaFreq + 500, Exp3);
        OutFreq = BaseFreq - DeltaFreq;
    } else {
        DT_ASSERT(DIV64(BaseFreq, Exp9)<(3*Exp9) && FreqOffsetPpt<(3*Exp9));
        DeltaFreq =  DIV64(BaseFreq, Exp9) * FreqOffsetPpt;
        DeltaFreq += DIV64(MOD64(BaseFreq,Exp9) * FreqOffsetPpt + 5*Exp8, Exp9);
        DeltaFreq = DIV64(DeltaFreq + 500, Exp3);
        OutFreq = BaseFreq + DeltaFreq;
    }
    *pFrequencyMicroHz = OutFreq;
    *pFreqOffsetPpt = FreqOffsetPpt;

    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_SetDcoFreqOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfGenLockCtrl_SetDcoFreqOffset(DtDfGenLockCtrl* pDf, Int ClockIdx,
                                                                        Int FreqOffsetPpt)
{
    DtStatus Status = DT_STATUS_OK;
    const DtDfSi534XClockProps*  pClkProps = NULL;
    Bool IsFractional = FALSE;

    // Sanity check
    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);
    // Must be enabled
    DF_GENLOCKCTRL_MUST_BE_ENABLED(pDf);
    DT_ASSERT(pDf->m_pSi534XClockProps != NULL);

    // Check paramters
    if (ClockIdx<-1 || ClockIdx>=pDf->m_NumSi534XClockProps)
        return DT_STATUS_INVALID_PARAMETER;

    // Get properties and check state under SofSpinLock
    DtSpinLockAcquire(&pDf->m_SofSpinLock);
    if (ClockIdx == -1)
        ClockIdx = DtDfGenLockCtrl_GetCurrentClockIdx(pDf);
    pClkProps = &pDf->m_pSi534XClockProps[ClockIdx];
    pClkProps = &pDf->m_pSi534XClockProps[ClockIdx];
    IsFractional = (pClkProps->m_ClockType == DT_DF_SI534X_CLK_FRACTIONAL);
    // Internal virtual genref must be used and free running
    if (pDf->m_GenRefType!=DT_DF_GENLOCKCTRL_GENREF_VIRTUAL
                          || pDf->m_DcoControlState!=DT_DF_GENLOCKCTRL_STATE_FREE_RUNNING)
        Status = DT_STATUS_IN_USE;
    DtSpinLockRelease(&pDf->m_SofSpinLock);

    if (!DT_SUCCESS(Status))
        return Status;

    // Check offset
    if (FreqOffsetPpt<-pClkProps->m_RangePpt || FreqOffsetPpt>pClkProps->m_RangePpt)
        return DT_STATUS_INVALID_PARAMETER;

    // Update offset
     return DtDfSi534X_SetFreqOffsetPpt(pDf->m_pDfSi534X, FreqOffsetPpt, IsFractional);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_LockChangedRegister -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfGenLockCtrl_LockChangedRegister(DtDfGenLockCtrl* pDf,
                                        const DtDfGenLockCtrlLockChangedRegData* pRegData)
{
    DtStatus Status = DT_STATUS_OK;
    Int  i, NumElems = 0;

    // Sanity check
    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_pOnLockChangedHandlers != NULL);
    
    // Parameter check
    if (pRegData==NULL || pRegData->m_pObject==NULL || 
                               (pRegData->m_pObject->m_ObjectType!=DT_OBJECT_TYPE_BC &&
                                    pRegData->m_pObject->m_ObjectType!=DT_OBJECT_TYPE_DF))
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_GENLOCKCTRL_MUST_BE_ENABLED(pDf);


    // Update list under spinlock protection
    DtSpinLockAcquire(&pDf->m_SofSpinLock);

    // Check if we already have an entry for this object
    NumElems = DtVector_Size(pDf->m_pOnLockChangedHandlers);
    for (i=0; i<NumElems; i++)
    {
        DtDfGenLockCtrlLockChangedRegData*  p =
        (DtDfGenLockCtrlLockChangedRegData*)DtVector_At(pDf->m_pOnLockChangedHandlers, i);
        if (p->m_pObject != pRegData->m_pObject)
            continue;

        // Overwrite existing element
        p->m_OnLockChangedFunc = pRegData->m_OnLockChangedFunc;
        DtSpinLockRelease(&pDf->m_SofSpinLock);
        return DT_STATUS_OK;
    }
    // Add new element
    Status = DtVector_PushBack(pDf->m_pOnLockChangedHandlers, pRegData);
    DtSpinLockRelease(&pDf->m_SofSpinLock);
    return Status;
}


// .-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_LockChangedUnregister -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfGenLockCtrl_LockChangedUnregister(DtDfGenLockCtrl* pDf, 
                                                                  const DtObject* pObject)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i, NumElems = 0;

    // Sanity check
    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_pOnLockChangedHandlers != NULL);
    
    // Parameter check
    if (pObject==NULL || (pObject->m_ObjectType!=DT_OBJECT_TYPE_BC &&
                                                pObject->m_ObjectType!=DT_OBJECT_TYPE_DF))
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_GENLOCKCTRL_MUST_BE_ENABLED(pDf);

    // Update list under spinlock protection
    DtSpinLockAcquire(&pDf->m_SofSpinLock);

    // Find entry for the specified object and erase it
    NumElems = DtVector_Size(pDf->m_pOnLockChangedHandlers);
    for (i=0; i<NumElems; i++)
    {
        const DtDfGenLockCtrlLockChangedRegData*  p =
        (DtDfGenLockCtrlLockChangedRegData*)DtVector_At(pDf->m_pOnLockChangedHandlers, i);
        if (p->m_pObject != pObject)
            continue;

        // Erase the element
        Status = DtVector_Erase(pDf->m_pOnLockChangedHandlers, i);
        DtSpinLockRelease(&pDf->m_SofSpinLock);
        return Status;
    }
    DtSpinLockRelease(&pDf->m_SofSpinLock);
    return DT_STATUS_NOT_FOUND;
}


// +=+=+=+=+=+=+=+=+=+=+=+=+ DtDfGenLockCtrl - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+
//
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfGenLockCtrl_Init(DtDf* pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfGenLockCtrl* pDf = (DtDfGenLockCtrl*)pDfBase;

    // Sanity checks
    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    // Set defaults
    pDf->m_DcoControlState = DT_DF_GENLOCKCTRL_STATE_INITIAL;
    pDf->m_GenRefStatus = DT_DF_GENLOCKCTRL_GENREFSTATUS_NO_SIGNAL;
    pDf->m_GenLockControlEnabled = FALSE;
    pDf->m_VideoStandard = DT_VIDSTD_1080I50;
    pDf->m_FrameLength = DtAvVidStd2FrameLength(pDf->m_VideoStandard);
    pDf->m_FractionalClock =  DtAvVidStdUsesFractionalClock(pDf->m_VideoStandard);
    pDf->m_FramePeriod = DtAvVidStd2FramePeriod(pDf->m_VideoStandard);
    pDf->m_DcoControlPars = DtDfGenLockCtrl_GetDcoControlPars(pDf->m_VideoStandard);
    pDf->m_DcoFreqOffsetPpt = 0;
    pDf->m_pSi534XClockProps = NULL;
    pDf->m_NumSi534XClockProps = 0;

    // Init GenRef info
    DtDfGenLockCtrl_InitGenRefInfo(pDf);
    // Clear GenRef frame period measurments
    DtDfGenLockCtrl_FramePeriodMeasureClear(&pDf->m_GenRefFpMeasure);
    // Clear GenRef timestamps
    pDf->m_GenRefSofTods.m_TotalNumSofTods = 0;
    DtDfGenLockCtrl_SofTodsClear(&pDf->m_GenRefSofTods);

    // Clear GenLock timestamps
    pDf->m_GenLockSofTod[0].m_Seconds=-1; pDf->m_GenLockSofTod[0].m_Nanoseconds=-1;
    pDf->m_GenLockSofTod[1].m_Seconds=-1; pDf->m_GenLockSofTod[1].m_Nanoseconds=-1;

    // Create the genlock status changed registration list
    DT_ASSERT(pDf->m_pOnLockChangedHandlers == NULL);
    pDf->m_pOnLockChangedHandlers = 
                        DtVector_Create(8, sizeof(DtDfGenLockCtrlLockChangedRegData), 4);
    if (pDf->m_pOnLockChangedHandlers == NULL)
    {
        DtDbgOutDf(ERR, GENLOCKCTRL, pDf,  "ERROR: failed to create reg. data list");
        return DT_STATUS_OUT_OF_MEMORY;
    }

    //Open children
    Status = DtDfGenLockCtrl_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, GENLOCKCTRL, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    // Initialize the helper thread
    DT_RETURN_ON_ERROR(DtThreadInit(&pDf->m_DcoControlThread, 
                                             DtDfGenLockCtrl_DcoControlThreadEntry, pDf));
    DtEventInit(&pDf->m_DcoControlSofEvent, TRUE);
    pDf->m_StopDcoControlThread = FALSE;

    // Initialize the start-of-frame handler spinlock
    DtSpinLockInit(&pDf->m_SofSpinLock);

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Note: LoadParameters() is called before the Init(). The loaded parameters can be used
// in the Init().
//
DtStatus  DtDfGenLockCtrl_LoadParameters(DtDf*  pDfBase)
{
    return DT_STATUS_OK;
}
// .-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfGenLockCtrl_OnEnablePostChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfGenLockCtrl* pDf = (DtDfGenLockCtrl*)pDfBase;
    DtBcGENLOnStartOfFrameRegData  GenlockRegData;
    DtStatus  Status = DT_STATUS_OK;
     
    // Sanity check
    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "DISABLE -> ENABLE");

        // Genlocking control must be disabled

        DT_ASSERT(!pDf->m_GenLockControlEnabled);

        // Set operational mode of children to IDLE
        if (pDf->m_pBcGenLock != NULL)
            DT_RETURN_ON_ERROR(DtBcGENL_SetOperationalMode(pDf->m_pBcGenLock,
                                                                   DT_BLOCK_OPMODE_IDLE));

        // Get the clock properties
        DT_RETURN_ON_ERROR(DtDfSi534X_GetClockProperties(pDf->m_pDfSi534X,
                                 &pDf->m_NumSi534XClockProps, &pDf->m_pSi534XClockProps));
        // Set defaults
        pDf->m_VideoStandard = DT_VIDSTD_1080I50;
        pDf->m_FrameLength = DtAvVidStd2FrameLength(pDf->m_VideoStandard);
        pDf->m_FractionalClock =  DtAvVidStdUsesFractionalClock(pDf->m_VideoStandard);
        pDf->m_FramePeriod = DtAvVidStd2FramePeriod(pDf->m_VideoStandard);
        pDf->m_DcoControlPars = DtDfGenLockCtrl_GetDcoControlPars(pDf->m_VideoStandard);
        pDf->m_DcoFreqOffsetPpt = 0;
        DT_RETURN_ON_ERROR(DtDfSi534X_SetFreqOffsetPpt(pDf->m_pDfSi534X, 0, FALSE));
        DT_RETURN_ON_ERROR(DtDfSi534X_SetFreqOffsetPpt(pDf->m_pDfSi534X, 0, TRUE));

        if (pDf->m_pBcGenLock != NULL)
        {
            DT_RETURN_ON_ERROR(DtBcGENL_SetFrameLength(pDf->m_pBcGenLock,
                                            pDf->m_FrameLength, pDf->m_FractionalClock));
            // Register Genlock start-of-frame handler
            GenlockRegData.m_OnStartOfFrameFunc 
                                             = DtDfGenLockCtrl_GenLockStartOfFrameHandler;
            GenlockRegData.m_pObject = (DtObject*)pDf;
            Status = DtBcGENL_StartOfFrameRegister(pDf->m_pBcGenLock, &GenlockRegData);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOutDf(ERR, GENLOCKCTRL, pDf,
                                      "ERROR: failed to register start-of-frame handler");
                return Status;
            }
        }

        // Start the DCO-control thread
        DtEventReset(&pDf->m_DcoControlSofEvent);
        pDf->m_StopDcoControlThread = FALSE;
        Status = DtThreadStart(&pDf->m_DcoControlThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, GENLOCKCTRL, pDf, "ERROR: failed to start Thread");
            return Status;
        }

        // Start genlocking
        if (pDf->m_pBcGenLock != NULL)
        {
            Status = DtBcGENL_SetOperationalMode(pDf->m_pBcGenLock, DT_BLOCK_OPMODE_RUN);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOutDf(ERR, GENLOCKCTRL, pDf, "ERROR: failed to set GENL opmode");
                return Status;
            }
        }

        // Init GenRef info
        DtSpinLockAcquire(&pDf->m_SofSpinLock);

        DtDfGenLockCtrl_InitGenRefInfo(pDf);
        // Clear GenRef frame period measurements
        DtDfGenLockCtrl_FramePeriodMeasureClear(&pDf->m_GenRefFpMeasure);

        // Clear GenRef timestamps
        pDf->m_GenRefSofTods.m_TotalNumSofTods = 0;
        DtDfGenLockCtrl_SofTodsClear(&pDf->m_GenRefSofTods);

        // Clear GenLock timestamps
        pDf->m_GenLockSofTod[0].m_Seconds=-1; pDf->m_GenLockSofTod[0].m_Nanoseconds=-1;
        pDf->m_GenLockSofTod[1].m_Seconds=-1; pDf->m_GenLockSofTod[1].m_Nanoseconds=-1;

        // Enable genlocking control
        pDf->m_DcoControlState = DT_DF_GENLOCKCTRL_STATE_INITIAL;
        pDf->m_GenRefStatus = DT_DF_GENLOCKCTRL_GENREFSTATUS_NO_SIGNAL;
        pDf->m_GenLockControlEnabled = TRUE;
        DtSpinLockRelease(&pDf->m_SofSpinLock);
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_OnEnablePreChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfGenLockCtrl_OnEnablePreChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfGenLockCtrl* pDf = (DtDfGenLockCtrl*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
    // Sanity check
    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    if (!Enable)
    {
        // ENABLE -> DISABLE
        DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "ENABLE -> DISABLE");

        // Disable genlocking control
        DtSpinLockAcquire(&pDf->m_SofSpinLock);
        pDf->m_GenLockControlEnabled = FALSE;

        // Clear list of  GenLock status changed handlers
        DtVector_Clear(pDf->m_pOnLockChangedHandlers);

        DtSpinLockRelease(&pDf->m_SofSpinLock);

        // Stop genlocking
        if (pDf->m_pBcGenLock != NULL)
        {
            Status = DtBcGENL_SetOperationalMode(pDf->m_pBcGenLock, DT_BLOCK_OPMODE_IDLE);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOutDf(ERR, GENLOCKCTRL, pDf, "ERROR: failed to set GENL opmode");
                return Status;
            }
        }

        // Unregister start of frame handlers
        if (pDf->m_pBcGenLock != NULL)
            DtBcGENL_StartOfFrameUnregister(pDf->m_pBcGenLock, (DtObject*)pDf);

        // Stop DCO-control thread
        pDf->m_StopDcoControlThread = TRUE;
        // Thread wake-up through start-of-frame event
        DtEventSet(&pDf->m_DcoControlSofEvent);
        Status = DtThreadStop(&pDf->m_DcoControlThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, GENLOCKCTRL, pDf, "ERROR: failed to stop Thread");
            return Status;
        }
    }
    return DT_STATUS_OK;
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtDfGenLockCtrl_OnCloseFile(DtDf* pDfBase, const DtFileObject* pFile)
{
    DtDfGenLockCtrl* pDf = (DtDfGenLockCtrl*)pDfBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    // Sanity checks
    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);


    // Use base function to release exclusive access
    return DtDf_OnCloseFile((DtDf*)pDf, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfGenLockCtrl_OpenChildren(DtDfGenLockCtrl*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // List of children supported by the the GENLOCKCTRL function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_GENL, DT_BC_GENL_NAME,
                           GENL_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcGenLock), FALSE},
        { DT_OBJECT_TYPE_DF, DT_FUNC_TYPE_SI534X, DT_DF_SI534X_NAME,
                           SI534X_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pDfSi534X), TRUE},
    };

    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    // Children should be present
    DT_ASSERT(pDf->m_pDfSi534X != NULL);

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_GetCurrentClockIdx -.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int DtDfGenLockCtrl_GetCurrentClockIdx(DtDfGenLockCtrl* pDf)
{
    // Return the clock index that is currently used
    Int  ClkIdx;
    for (ClkIdx = 0; ClkIdx<pDf->m_NumSi534XClockProps; ClkIdx++)
    {
        Bool IsFractional = (pDf->m_pSi534XClockProps[ClkIdx].m_ClockType
                                                     == DT_GENLOCKCTR_CLKTYPE_FRACTIONAL);
        if (IsFractional == pDf->m_FractionalClock)
            return ClkIdx;
    }
    DT_ASSERT(FALSE);
    return 0;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_InitGenRefInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtDfGenLockCtrl_InitGenRefInfo(DtDfGenLockCtrl* pDf)
{
    // Init GenRef info
    pDf->m_GenRefType = DT_DF_GENLOCKCTRL_GENREF_UNDEFINED;
    pDf->m_GenRefPortIndex = -1;
    pDf->m_GenRefVidStd = DT_VIDSTD_UNKNOWN;
    pDf->m_GenRefFramePeriod = -1;
    pDf->m_GenRefDetectVidStd = DT_VIDSTD_UNKNOWN;
    pDf->m_GenRefStartOfFrameOffset = 0;
    pDf->m_GenRefParChanged = TRUE;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_SetFrameLength -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfGenLockCtrl_SetVideoStandard(DtDfGenLockCtrl* pDf, Int  VidStd)
{
    Int  FrameLength = DtAvVidStd2FrameLength(VidStd);
    Bool FracClk =  DtAvVidStdUsesFractionalClock(VidStd);

    // Sanity checks
    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    // Must be enabled
    DF_GENLOCKCTRL_MUST_BE_ENABLED(pDf);

    // Check parameter
    if (FrameLength < 0)
        return DT_STATUS_INVALID_PARAMETER;

    // No change?
    if (VidStd == pDf->m_VideoStandard)
        return DT_STATUS_OK;

    // Only supported if there is a genlock block
    if (pDf->m_pBcGenLock == NULL)
       return DT_STATUS_NOT_SUPPORTED;

    // First go to IDLE
    DT_RETURN_ON_ERROR(DtBcGENL_SetOperationalMode(pDf->m_pBcGenLock,
                                                                   DT_BLOCK_OPMODE_IDLE));

    // Change settings
    DT_RETURN_ON_ERROR(DtBcGENL_SetFrameLength(pDf->m_pBcGenLock, FrameLength, FracClk));

    // Update cache
    pDf->m_VideoStandard = VidStd;
    pDf->m_FrameLength = FrameLength;
    pDf->m_FractionalClock = FracClk;
    pDf->m_FramePeriod = DtAvVidStd2FramePeriod(VidStd);
    pDf->m_DcoControlPars = DtDfGenLockCtrl_GetDcoControlPars(VidStd);

    DT_ASSERT(pDf->m_FramePeriod > 0);

    // Go to RUN
    DT_RETURN_ON_ERROR(DtBcGENL_SetOperationalMode(pDf->m_pBcGenLock,
                                                                    DT_BLOCK_OPMODE_RUN));

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_DcoControlThreadEntry -.-.-.-.-.-.-.-.-.-.-.-.
//
//
void  DtDfGenLockCtrl_DcoControlThreadEntry(DtThread* pThread, void* pContext)
{
    DtDfGenLockCtrl*  pDf = (DtDfGenLockCtrl*)pContext;
    Bool  StopThread = FALSE;
    DtDfGenLockCtrlSofTods  GenRefSofTods;
    DtTodTime  GenLockSofTod;
    DtDfGenLockCtrlGenRefType  GenRefType;
    Int  GenRefVidStd, GenRefSofOffset=0;
    Int64  GenRefAvgFramePeriodPs;
    Bool  GenRefParChanged;
    const Int LoopTimeOut = 100;    // 100ms timeout

    // Start with no reference
    DtDfGenLockCtrl_NotifyStatusChange(pDf, DT_GENLOCKCTRL_STATE_NO_REF);
    DtDfGenLockCtrl_NotifyLockChange(pDf, FALSE);

    while (!StopThread)
    {
        // Wait for start-of-frame event
        DtStatus Status = DtEventWaitUnInt(&pDf->m_DcoControlSofEvent, LoopTimeOut);
        if ((!DT_SUCCESS(Status) && Status!=DT_STATUS_TIMEOUT) || pThread->m_StopThread 
                                                           || pDf->m_StopDcoControlThread)
        {
            // Fatal error occurred or stop request received
            if (!DT_SUCCESS(Status))
                DtDbgOutDf(ERR, GENLOCKCTRL, pDf, "ERROR: DcoControlThread failure");
            if (pThread->m_StopThread || pDf->m_StopDcoControlThread)
                DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "DcoControlThread stop request");
            StopThread = TRUE;
            continue;
        }
        // Copy GenRef/GenLock information under spinlock
        DtSpinLockAcquire(&pDf->m_SofSpinLock);
        // GenRef start-of-frame timstamps
        GenRefSofTods = pDf->m_GenRefSofTods;
        // GenLock start-of-frame timstamp
        GenLockSofTod = pDf->m_GenLockSofTod[1];
        // GenRef type
        GenRefType = pDf->m_GenRefType;
        // GenRef average frameperiod
        GenRefAvgFramePeriodPs = DtDfGenLockCtrl_FramePeriodMeasureGetAverage( 
                                                                 &pDf->m_GenRefFpMeasure);
        // GenRef video standard
        GenRefVidStd = pDf->m_GenRefVidStd;
        // Start-of-frame offset in nano seconds
        GenRefSofOffset = pDf->m_GenRefStartOfFrameOffset;
        // GenRef changed
        GenRefParChanged = pDf->m_GenRefParChanged;
        pDf->m_GenRefParChanged = FALSE;
        DtSpinLockRelease(&pDf->m_SofSpinLock);

        // Unknown video standard? Nothing to do
        if (GenRefVidStd == DT_VIDSTD_UNKNOWN)
            continue;

        // Check for GenRef parameter changes
        if (GenRefParChanged)
        {
            // Update video standard this cannot be spinlock protected. 
            Status = DtDfGenLockCtrl_SetVideoStandard(pDf, GenRefVidStd);
            DT_ASSERT(DT_SUCCESS(Status));

            DtSpinLockAcquire(&pDf->m_SofSpinLock);

            // Restart synchronizing
            pDf->m_DcoControlState = DT_DF_GENLOCKCTRL_STATE_INITIAL;

            // Clear GenLock timestamps
            pDf->m_GenLockSofTod[0].m_Seconds = pDf->m_GenLockSofTod[0].m_Nanoseconds=-1;
            pDf->m_GenLockSofTod[1].m_Seconds = pDf->m_GenLockSofTod[1].m_Nanoseconds=-1;
            
            DtSpinLockRelease(&pDf->m_SofSpinLock);
            continue;
        }
        
        // No valid Genlock SOF-timestamp? Nothing to do
        if (GenLockSofTod.m_Seconds==-1 || GenLockSofTod.m_Nanoseconds==-1)
            continue;

        // Control the DCO
        DtDfGenLockCtrl_DcoControl(pDf,GenRefType, GenRefSofOffset, &GenRefSofTods,
                                                  GenRefAvgFramePeriodPs, &GenLockSofTod);
    }
    // We have to wait until the thread received a stop command.
    DtThreadWaitForStop(pThread);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_DcoControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfGenLockCtrl_DcoControl(DtDfGenLockCtrl* pDf, 
                                            DtDfGenLockCtrlGenRefType  GenRefType,
                                            Int  SofOffset,
                                            const DtDfGenLockCtrlSofTods* pGenRefSofTods,
                                            Int64  GenRefAvgFramePeriodPs,
                                            const DtTodTime* pGenLockSofTod)
{
    DtDfGenLockCtrlDcoState  OldDcoState, DcoState;
    DtDfGenLockCtrlGenRefStatus  GenRefStatus;
    Int  OldGenLockStatus, GenLockStatus;

    DtSpinLockAcquire(&pDf->m_SofSpinLock);
    DcoState = pDf->m_DcoControlState;
    OldDcoState = DcoState;
    OldGenLockStatus = DtDfGenLockCtrl_GetGenLockStatusInternal(pDf);
    DtSpinLockRelease(&pDf->m_SofSpinLock);

    // Determine GenRef signal status using the latest timestamps
    GenRefStatus = DtDfGenLockCtrl_DetermineGenRefStatus(pDf, GenRefType, pGenRefSofTods,
                                    GenRefAvgFramePeriodPs, pGenLockSofTod);

    switch (DcoState)
    {
    default:
    case  DT_DF_GENLOCKCTRL_STATE_INITIAL:
        DtDfGenLockCtrl_DcoControlInit(pDf, &DcoState, GenRefStatus,
                                             GenRefType,  SofOffset, pGenRefSofTods,
                                             GenRefAvgFramePeriodPs, pGenLockSofTod);
        break;
        break;
    case DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_FREQ:
    case DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_FREQ_CHECK:
        DtDfGenLockCtrl_DcoControlCrashLockFreq(pDf, &DcoState, GenRefStatus,
                                             GenRefType,  SofOffset, pGenRefSofTods,
                                             GenRefAvgFramePeriodPs, pGenLockSofTod);
        break;
    case DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_PHASE:
    case DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_PHASE_WAIT_FOR_DONE:
        DtDfGenLockCtrl_DcoControlCrashLockPhase(pDf, &DcoState, GenRefStatus,
                                             GenRefType,  SofOffset, pGenRefSofTods,
                                             GenRefAvgFramePeriodPs, pGenLockSofTod);
        break;
    case DT_DF_GENLOCKCTRL_STATE_LOCKED:
        DtDfGenLockCtrl_DcoControlLocked(pDf, &DcoState, GenRefStatus,
                                             GenRefType,  SofOffset, pGenRefSofTods,
                                             GenRefAvgFramePeriodPs, pGenLockSofTod);
        break;
    case DT_DF_GENLOCKCTRL_STATE_FREE_RUNNING:
        DtDfGenLockCtrl_DcoFreeRunning(pDf, &DcoState, GenRefStatus,
                                             GenRefType,  SofOffset, pGenRefSofTods,
                                             GenRefAvgFramePeriodPs, pGenLockSofTod);
        break;
    }

    DtSpinLockAcquire(&pDf->m_SofSpinLock);
    // No changes made outside the statemachine?
    if (OldDcoState == pDf->m_DcoControlState)
    {
        // Update the state
        pDf->m_DcoControlState = DcoState;
        // Save new GenRef status
        pDf->m_GenRefStatus = GenRefStatus;
    }
    // Determine new GenLock status
    GenLockStatus = DtDfGenLockCtrl_GetGenLockStatusInternal(pDf);
    DtSpinLockRelease(&pDf->m_SofSpinLock);
    
    // Is there a GenLock status change?
    if (OldGenLockStatus != GenLockStatus)
    {
        // Signal GenLock status change
        DtDfGenLockCtrl_NotifyStatusChange(pDf, GenLockStatus);
    }
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_DcoControlInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfGenLockCtrl_DcoControlInit(DtDfGenLockCtrl* pDf,
                                             DtDfGenLockCtrlDcoState* pDcoState, 
                                             DtDfGenLockCtrlGenRefStatus GenRefStatus,
                                             DtDfGenLockCtrlGenRefType  GenRefType,
                                             Int  SofOffset,
                                             const DtDfGenLockCtrlSofTods* pGenRefSofTods,
                                             Int64  GenRefAvgFramePeriodPs,
                                             const DtTodTime* pGenLockSofTod)
{
    if (GenRefType==DT_DF_GENLOCKCTRL_GENREF_UNDEFINED ||
                                             GenRefType==DT_DF_GENLOCKCTRL_GENREF_VIRTUAL)
    {
        DtStatus  Status;
        // Free running set nominal DCO frequency
        pDf->m_DcoFreqOffsetPpt = 0;
        Status = DtDfSi534X_SetFreqOffsetPpt(pDf->m_pDfSi534X, 0, FALSE);
        DT_ASSERT(DT_SUCCESS(Status));
        Status = DtDfSi534X_SetFreqOffsetPpt(pDf->m_pDfSi534X, 0, TRUE);
        DT_ASSERT(DT_SUCCESS(Status));
        DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "DCO State => FREE RUNNING");
        *pDcoState = DT_DF_GENLOCKCTRL_STATE_FREE_RUNNING;

        // Signal in lock
        DtDfGenLockCtrl_NotifyLockChange(pDf, TRUE);
    }
    else
    {
        DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "DCO State => CRASH LOCK FREQUENCY");
        *pDcoState = DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_FREQ;

        // Signal no lock
        DtDfGenLockCtrl_NotifyLockChange(pDf, FALSE);
    }
}

// -.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_DcoControlCrashLockFreq -.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfGenLockCtrl_DcoControlCrashLockFreq(DtDfGenLockCtrl* pDf, 
                                             DtDfGenLockCtrlDcoState* pDcoState, 
                                             DtDfGenLockCtrlGenRefStatus GenRefStatus,
                                             DtDfGenLockCtrlGenRefType  GenRefType,
                                             Int  SofOffset,
                                             const DtDfGenLockCtrlSofTods* pGenRefSofTods,
                                             Int64  GenRefAvgFramePeriodPs,
                                             const DtTodTime* pGenLockSofTod)
{
    Int64  DcoClockFreqMilliHz, RefClockFreqHz, Remainder, FreqDiffPpb, FreqAdjustPpt;
    Int64  RefClockFreqMilliHz;
    Int  GenRefAvgFramePeriodNs = (Int)DIV64(GenRefAvgFramePeriodPs+500, 1000);
    Int  PhaseDiffNs;
    Bool  ValidPhaseDiff;

    // Wait for valid GenRef
    if (GenRefStatus != DT_DF_GENLOCKCTRL_GENREFSTATUS_OK)
        return;

    // Determine phase difference
    ValidPhaseDiff = DtDfGenLockCtrl_DcoControlComputePhaseDiff(pDf, SofOffset, 
                                                   pGenRefSofTods, GenRefAvgFramePeriodNs, 
                                                   pGenLockSofTod, &PhaseDiffNs);
    // Wait for valid phase difference
    if (!ValidPhaseDiff)
        return;
 
    // Get DCO-frequency in milli Hz
    DcoClockFreqMilliHz = DtDfGenLockCtrl_DcoControlGetFrequencyMilliHz(pDf);

    // Compute GenRef (148.5MHz) clock in milliHz
    RefClockFreqHz = DIV64(pDf->m_FrameLength*Exp12, GenRefAvgFramePeriodPs);
    Remainder = pDf->m_FrameLength*Exp12 - RefClockFreqHz*GenRefAvgFramePeriodPs;
    RefClockFreqMilliHz = RefClockFreqHz*1000 + DIV64(Remainder*1000,
                                                                  GenRefAvgFramePeriodPs);

    // Compute frequency deviation in ppb
    FreqDiffPpb = DIV64((RefClockFreqMilliHz-DcoClockFreqMilliHz)*Exp9,
                                                                     RefClockFreqMilliHz);
    DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "PpbDiff: %d", (Int)FreqDiffPpb);

    FreqAdjustPpt = FreqDiffPpb*1000;

    // Limitate the step size during crashlock
    if (DIV64(FreqAdjustPpt, Exp6) > MAX_CRASHLOCK_STEP_PPM)
        FreqAdjustPpt = MAX_CRASHLOCK_STEP_PPM*Exp6;
    else if (DIV64(FreqAdjustPpt, Exp6) < -MAX_CRASHLOCK_STEP_PPM)
        FreqAdjustPpt = -MAX_CRASHLOCK_STEP_PPM*Exp6;

    // Update DCO-frequency
    DtDfGenLockCtrl_DcoControlAddDeltaFreq(pDf, (Int)FreqAdjustPpt);

    if (*pDcoState == DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_FREQ)
    {
        DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "DCO State => CRASH LOCK FREQ CHECK");
        *pDcoState = DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_FREQ_CHECK;
        pDf->m_Count = 0;
    } 
    // When the deviation is four times less than 1ppm enter the next state
    else if (FreqDiffPpb>-1000 && FreqDiffPpb<1000)
    {
        pDf->m_Count++;
        if (pDf->m_Count >= 4)
        {
            DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "DCO State => CRASH LOCK PHASE");
            *pDcoState = DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_PHASE;
        }
    } 
    else
        pDf->m_Count = 0;
}

// .-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_DcoControlCrashLockPhase -.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfGenLockCtrl_DcoControlCrashLockPhase(DtDfGenLockCtrl* pDf, 
                                             DtDfGenLockCtrlDcoState* pDcoState, 
                                             DtDfGenLockCtrlGenRefStatus GenRefStatus,
                                             DtDfGenLockCtrlGenRefType  GenRefType,
                                             Int  SofOffset,
                                             const DtDfGenLockCtrlSofTods* pGenRefSofTods,
                                             Int64  GenRefAvgFramePeriodPs,
                                             const DtTodTime* pGenLockSofTod)
{
    Int  GenRefAvgFramePeriodNs = (Int)DIV64(GenRefAvgFramePeriodPs+500, 1000);
    Int  PhaseDiffNs;
    Bool  ValidPhaseDiff;

    DT_ASSERT(pDf->m_pBcGenLock != NULL);
    if (pDf->m_pBcGenLock == NULL)
        return;

    // Invalid GenRef?
    if (GenRefStatus != DT_DF_GENLOCKCTRL_GENREFSTATUS_OK)
    {
        // No valid signal=> restart crash lock frequency
        DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "DCO State => CRASH LOCK FREQUENCY");
        *pDcoState = DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_FREQ;
        return;
    }

    // Determine phase difference
    ValidPhaseDiff = DtDfGenLockCtrl_DcoControlComputePhaseDiff(pDf, SofOffset, 
                                                   pGenRefSofTods, GenRefAvgFramePeriodNs, 
                                                   pGenLockSofTod, &PhaseDiffNs);
    if (!ValidPhaseDiff)
    {
        // No valid phase => restart crash lock frequency
        DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "DCO State => CRASH LOCK FREQUENCY");
        *pDcoState = DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_FREQ;
        return;
    }

    // Need to perform the phase adjustment?
    if (*pDcoState == DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_PHASE)
    { 
        DtStatus  Status;
        Int64 DcoFreqHz, Adjustment;

        // Only positive adjustments
        PhaseDiffNs = GenRefAvgFramePeriodNs - PhaseDiffNs;
        DT_ASSERT(PhaseDiffNs >= 0);

        // Convert nanoseconds to clockticks
        DcoFreqHz = DtDfGenLockCtrl_DcoControlGetFrequencyHz(pDf);
        Adjustment = DIV64(DcoFreqHz * PhaseDiffNs, Exp9);
        DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "DcoFreq: %d Adjustment: %d", 
                                                         (Int)DcoFreqHz, (Int)Adjustment);
        Status = DtBcGENL_SetCrashLockAdjust(pDf->m_pBcGenLock, (Int)Adjustment);
        if (!DT_SUCCESS(Status))
            DtDbgOutDf(ERR, GENLOCKCTRL, pDf, "SetCrashLockAdjust failed");
        else
        { 
            DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "DCO State => WAIT FOR DONE");
            *pDcoState = DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_PHASE_WAIT_FOR_DONE;
        }
    }
    else if (*pDcoState == DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_PHASE_WAIT_FOR_DONE)
    { 
        DtStatus  Status;
        Int  CrashLockStatus;
        // Get crashlock status
        Status = DtBcGENL_GetCrashLockStatus(pDf->m_pBcGenLock, &CrashLockStatus);
        if (DT_SUCCESS(Status) && CrashLockStatus==DT_BC_GENL_CRASHLOCK_NORMAL)
        {
            // Crashlock succeeded
            Int  PhaseDiffNs;
            Bool  ValidPhaseDiff;
            ValidPhaseDiff = DtDfGenLockCtrl_DcoControlComputePhaseDiff(pDf, SofOffset, 
                                                   pGenRefSofTods, GenRefAvgFramePeriodNs, 
                                                   pGenLockSofTod, &PhaseDiffNs);
            if (ValidPhaseDiff)
                DtDbgOutDf(MAX, GENLOCKCTRL, pDf, "PhaseDiff: %d ns", PhaseDiffNs);
            else
                DtDbgOutDf(ERR, GENLOCKCTRL, pDf, "No valid PhaseDiff");
            
            // Enter locked state
            DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "DCO State => LOCKED");
            *pDcoState = DT_DF_GENLOCKCTRL_STATE_LOCKED;

            // Signal in lock
            DtDfGenLockCtrl_NotifyLockChange(pDf, TRUE);
        }
    }
    else
        DT_ASSERT(FALSE);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_DcoControlLocked -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfGenLockCtrl_DcoControlLocked(DtDfGenLockCtrl* pDf, 
                                             DtDfGenLockCtrlDcoState* pDcoState, 
                                             DtDfGenLockCtrlGenRefStatus GenRefStatus,
                                             DtDfGenLockCtrlGenRefType  GenRefType,
                                             Int  SofOffset,
                                             const DtDfGenLockCtrlSofTods* pGenRefSofTods,
                                             Int64  GenRefAvgFramePeriodPs,
                                             const DtTodTime* pGenLockSofTod)
{
    Int64  FreqDiffMilliHz, FreqAdjustPpt;
    Int  PhaseDiffNs, PhaseBoost;
    Bool ValidPhaseDiff;
    Int64  DcoClockFreqMilliHz, RefClockFreqMilliHz, RefClockFreqHz, Remainder;
    Int  GenRefAvgFramePeriodNs = (Int)DIV64(GenRefAvgFramePeriodPs+500, 1000);

    // Invalid GenRef? 
    if (GenRefStatus != DT_DF_GENLOCKCTRL_GENREFSTATUS_OK)
        return;

    // Determine phase difference
    ValidPhaseDiff = DtDfGenLockCtrl_DcoControlComputePhaseDiff(pDf, SofOffset, 
                                                   pGenRefSofTods, GenRefAvgFramePeriodNs, 
                                                   pGenLockSofTod, &PhaseDiffNs);
    if (!ValidPhaseDiff)
        return;
    
    // Check phase difference
    if (PhaseDiffNs<-pDf->m_DcoControlPars.m_MaxPhaseDiffNs
                                    || PhaseDiffNs>pDf->m_DcoControlPars.m_MaxPhaseDiffNs)
    {
        // Maximum phase difference exceeded; perform crash lock
        DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "DCO State => CRASH LOCK FREQUENCY");
        *pDcoState = DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_FREQ;

        // Signal no lock
        DtDfGenLockCtrl_NotifyLockChange(pDf, FALSE);
        return;
    }

    // Get DCO-frequency in milli Hz
    DcoClockFreqMilliHz = DtDfGenLockCtrl_DcoControlGetFrequencyMilliHz(pDf);

    // Compute GenRef (148.5MHz) clock in milliHz
    RefClockFreqHz = DIV64(pDf->m_FrameLength*Exp12, GenRefAvgFramePeriodPs);
    Remainder = pDf->m_FrameLength*Exp12 - RefClockFreqHz*GenRefAvgFramePeriodPs;
    RefClockFreqMilliHz = RefClockFreqHz*1000 + DIV64(Remainder*1000,
                                                                  GenRefAvgFramePeriodPs);

    // Compute frequency difference
    FreqDiffMilliHz = RefClockFreqMilliHz - DcoClockFreqMilliHz;

    // Use phase boost to reduce the phase error to 1 pixel
    PhaseBoost = 1;
    if (PhaseDiffNs>-100 && PhaseDiffNs<100)
        PhaseBoost = 5;

    // Compute frequency adjustment
    FreqAdjustPpt =  
            DIV64(FreqDiffMilliHz*pDf->m_DcoControlPars.m_FreqMult, 1000) 
          + DIV64((Int64)PhaseBoost*PhaseDiffNs*pDf->m_DcoControlPars.m_PhaseMult, 1000);

    // Clip the step size
    if (FreqAdjustPpt > pDf->m_DcoControlPars.m_MaxStepPpt)
        FreqAdjustPpt = pDf->m_DcoControlPars.m_MaxStepPpt;
    else if (FreqAdjustPpt < -pDf->m_DcoControlPars.m_MaxStepPpt)
        FreqAdjustPpt = -pDf->m_DcoControlPars.m_MaxStepPpt;
    
    // Apply frequency adjustment
    DtDfGenLockCtrl_DcoControlAddDeltaFreq(pDf, (Int)FreqAdjustPpt);
    
    pDf->m_Count++;
    if (pDf->m_Count%100 == 1)
        DtDbgOutDf(AVG, GENLOCKCTRL, pDf, "FreqDiff: %d Hz; PhaseDiff: %d ns; Offset: %d", 
                                            (Int)DIV64(FreqDiffMilliHz,1000), PhaseDiffNs,
                                            (Int)DIV64(pDf->m_DcoFreqOffsetPpt,1000));
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_DcoFreeRunning -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfGenLockCtrl_DcoFreeRunning(DtDfGenLockCtrl* pDf, 
                                             DtDfGenLockCtrlDcoState* pDcoState, 
                                             DtDfGenLockCtrlGenRefStatus GenRefStatus,
                                             DtDfGenLockCtrlGenRefType  GenRefType,
                                             Int  SofOffset,
                                             const DtDfGenLockCtrlSofTods* pGenRefSofTods,
                                             Int64  GenRefAvgFramePeriodPs,
                                             const DtTodTime* pGenLockSofTod)
{
    // Nothing to do
}
// .-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_DetermineGenRefStatus -.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfGenLockCtrlGenRefStatus  DtDfGenLockCtrl_DetermineGenRefStatus(DtDfGenLockCtrl* pDf, 
                                             DtDfGenLockCtrlGenRefType  GenRefType,
                                             const DtDfGenLockCtrlSofTods* pGenRefSofTods,
                                             Int64  GenRefAvgFramePeriodPs,
                                             const DtTodTime* pGenLockSofTod)
{
    // Undefined GenRef?
    if  (GenRefType == DT_DF_GENLOCKCTRL_GENREF_UNDEFINED)
        return DT_DF_GENLOCKCTRL_GENREFSTATUS_NO_SIGNAL;

    // Check absence of signal for more than 200 milliseconds
    if (DtDfGenLockCtrl_SofTodsTimeDiff(pGenLockSofTod, pGenRefSofTods) > 2*Exp8)
        return DT_DF_GENLOCKCTRL_GENREFSTATUS_NO_SIGNAL;

    // Virtual GenRef doesn't have to be accurate
    if (GenRefType == DT_DF_GENLOCKCTRL_GENREF_VIRTUAL)
        return DT_DF_GENLOCKCTRL_GENREFSTATUS_OK;

    // Insufficient valid and recent reference timestamps?
    if (DtDfGenLockCtrl_SofTodsNumAvailable(pGenRefSofTods)<2
                                                             || GenRefAvgFramePeriodPs<=0)
        return DT_DF_GENLOCKCTRL_GENREFSTATUS_INVALID_SIGNAL;

    return DT_DF_GENLOCKCTRL_GENREFSTATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_DcoControlAddDeltaFreq -.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfGenLockCtrl_DcoControlAddDeltaFreq(DtDfGenLockCtrl* pDf, Int DeltaPpt)
{
    DtStatus  Status = DtDfSi534X_SetFreqOffsetPpt(pDf->m_pDfSi534X, 
                              pDf->m_DcoFreqOffsetPpt + DeltaPpt, pDf->m_FractionalClock);
    DT_ASSERT(DT_SUCCESS(Status));
    if (DT_SUCCESS(Status))
        pDf->m_DcoFreqOffsetPpt += DeltaPpt;
}

// .-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_DcoControlGetFrequencyHz -.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtDfGenLockCtrl_DcoControlGetFrequencyHz(DtDfGenLockCtrl* pDf)
{
    Int64  Freq = 0;
    if  (pDf->m_DcoFreqOffsetPpt > 0)
        Freq = 148500000LL + DIV64(1485LL * pDf->m_DcoFreqOffsetPpt + 5*Exp6, 10*Exp6);
    else
        Freq = 148500000LL + DIV64(1485LL * pDf->m_DcoFreqOffsetPpt - 5*Exp6, 10*Exp6);
    if (pDf->m_FractionalClock)
        Freq = DIV64(Freq*1000, 1001);
    return (Int)Freq;
}

// -.-.-.-.-.-.-.-.-.- <DtDfGenLockCtrl_DcoControlGetFrequencyMilliHz -.-.-.-.-.-.-.-.-.-.
//
Int64  DtDfGenLockCtrl_DcoControlGetFrequencyMilliHz(DtDfGenLockCtrl* pDf)
{
    Int64  Freq = 0;
    if  (pDf->m_DcoFreqOffsetPpt > 0)  
        Freq = 148500000000LL + DIV64(1485LL * pDf->m_DcoFreqOffsetPpt + 5*Exp3, 10*Exp3);
    else
        Freq = 148500000000LL + DIV64(1485LL * pDf->m_DcoFreqOffsetPpt - 5*Exp3, 10*Exp3);
    if (pDf->m_FractionalClock)
        Freq = DIV64(Freq*1000, 1001);
    return Freq;
}

// -.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_DcoControlComputePhaseDiff -.-.-.-.-.-.-.-.-.-.-.
//
Bool DtDfGenLockCtrl_DcoControlComputePhaseDiff(DtDfGenLockCtrl* pDf, Int SofOffset,
                                          const DtDfGenLockCtrlSofTods* pGenRefSofTods,
                                          Int  GenRefAvgFramePeriodNs,
                                          const DtTodTime* pGenLockSofTod,
                                          Int* pPhaseDiffNs)
{
    Int i, NumGenRefSofTodsAva;
    DtTodTime  GenLockSofTod, GenRefSofTod;
    Int64  PhaseDiff=0;
    Bool  Valid = FALSE;

    // Start-of-Frame will be generated with an offset
    SofOffset = DT_DF_GENLOCKCTRL_START_OF_FRAME_OFFSET - SofOffset;

    GenLockSofTod = *pGenLockSofTod;
    GenLockSofTod = DtCore_TOD_Add(GenLockSofTod, SofOffset);
    // Find the related GenRef Start-of-Frame timestamp
    NumGenRefSofTodsAva = DtDfGenLockCtrl_SofTodsNumAvailable(pGenRefSofTods);
    for (i=0; i<NumGenRefSofTodsAva && !Valid; i++)
    {
        GenRefSofTod = DtDfGenLockCtrl_SofTodsGet(pGenRefSofTods, i);
        PhaseDiff = DtCore_TOD_TimeDiff(GenLockSofTod, GenRefSofTod) 
                                                                 - GenRefAvgFramePeriodNs;
        Valid = (PhaseDiff>-GenRefAvgFramePeriodNs/2 
                                                   && PhaseDiff<GenRefAvgFramePeriodNs/2);
    }
    *pPhaseDiffNs = (Int)PhaseDiff;
    return Valid;
}

// .-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_GenRefStartOfFrameHandler -.-.-.-.-.-.-.-.-.-.-.
//
void DtDfGenLockCtrl_GenRefStartOfFrameHandler(DtDfGenLockCtrl* pDf, Int PortIdx,
                                   const DtTodTime*  pSofTod, Int SofOffset, Int VidStd,
                                   Int DetectVidStd, DtDfGenLockCtrlGenRefType GenRefType)
{
    // Sanity check
    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    if (pSofTod == NULL)
        return;

    // Start-of-frame offset range: +/- 20ms StartOfFrame offset
    if (SofOffset<-20000000 || SofOffset>20000000)
    {
        DT_ASSERT(FALSE);
        SofOffset = 0;
    }

    DtSpinLockAcquire(&pDf->m_SofSpinLock);
    // Only when genlock control is enabled
    if (pDf->m_GenLockControlEnabled)
    {
        // Check on genref change?
        if (PortIdx!=pDf->m_GenRefPortIndex || VidStd!=pDf->m_GenRefVidStd
                                   || SofOffset!=pDf->m_GenRefStartOfFrameOffset 
                                   || GenRefType!=pDf->m_GenRefType)
        {
            // -.-.-.-.-.-.-.-.-.-.-.-.- GenRef change detected -.-.-.-.-.-.-.-.-.-.-.-.-.
            DtDbgOutDf(MIN, GENLOCKCTRL, pDf, "GenRef change detected");
            
            // Clear GenRef timestamps
            DtDfGenLockCtrl_SofTodsClear(&pDf->m_GenRefSofTods);
            // Clear GenRef frame period measurements
            DtDfGenLockCtrl_FramePeriodMeasureClear(&pDf->m_GenRefFpMeasure);
            // Use 1 second buffer
            pDf->m_GenRefFpMeasure.m_BufSize = DtAvVidStd2Fps(VidStd);

            // Update other information
            pDf->m_GenRefType = GenRefType;
            pDf->m_GenRefVidStd = VidStd;
            pDf->m_GenRefStartOfFrameOffset = SofOffset;
            pDf->m_GenRefPortIndex = PortIdx;
            pDf->m_GenRefFramePeriod = DtAvVidStd2FramePeriod(VidStd);
            // Start pessimistic
            pDf->m_GenRefStatus = DT_DF_GENLOCKCTRL_GENREFSTATUS_NO_SIGNAL;

            // Change detected
            pDf->m_GenRefParChanged = TRUE;
        }

        // Store detected video standard
        pDf->m_GenRefDetectVidStd = DetectVidStd;

        // Add new GenRef timestamp
        DtDfGenLockCtrl_SofTodsAdd(pDf, &pDf->m_GenRefSofTods, pSofTod, 
                                                                pDf->m_GenRefFramePeriod);
        // Frame period available? Add sample to frame measurement data
        if (pDf->m_GenRefSofTods.m_LastFramePeriod > 0)
            DtDfGenLockCtrl_FramePeriodMeasureAdd(&pDf->m_GenRefFpMeasure,
                                                  pDf->m_GenRefSofTods.m_LastFramePeriod);
    }
    DtSpinLockRelease(&pDf->m_SofSpinLock);
}

// -.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_GenLockStartOfFrameHandler -.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfGenLockCtrl_GenLockStartOfFrameHandler(DtObject* pObj, const DtTodTime* pSofTod)
{
    DtDfGenLockCtrl*  pDf = (DtDfGenLockCtrl*)pObj;
    
    // Sanity check
    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    DtSpinLockAcquire(&pDf->m_SofSpinLock);
    // Only when genlocking is enabled
    if (pDf->m_GenLockControlEnabled)
    {
        // Store timestamp
        pDf->m_GenLockSofTod[1] = pDf->m_GenLockSofTod[0];
        pDf->m_GenLockSofTod[0] = *pSofTod;

        // If we have valid timestamps signal the start-of-frame event
        if (pDf->m_GenLockSofTod[1].m_Seconds!=-1 
                                             && pDf->m_GenLockSofTod[1].m_Nanoseconds!=-1)
            DtEventSet(&pDf->m_DcoControlSofEvent);
    }
    DtSpinLockRelease(&pDf->m_SofSpinLock);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_SofTodsClear -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtDfGenLockCtrl_SofTodsClear(DtDfGenLockCtrlSofTods* pSofTods)
{
    DT_ASSERT(pSofTods != NULL);
        // Clear GenLock timestamps
    pSofTods->m_NumSofTods = 0;
    pSofTods->m_SofTodIdx = 0;
    pSofTods->m_LastFramePeriod = 0;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_SofTodsAdd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtDfGenLockCtrl_SofTodsAdd(DtDfGenLockCtrl* pDf,  DtDfGenLockCtrlSofTods* pSofTods,
                                          const DtTodTime* pSofTod, Int ExpectFramePeriod)
{
    const Int SOFTODS_SIZE = DT_DF_GENLOCKCTRL_SOFTOD_SIZE;
    // Check validity of timestamp 
    if (pSofTods->m_NumSofTods > 0)
    {
        Int64  FramePeriod, PpmDiff;
        // Get previous timestamp
        Int  PrevIdx = (pSofTods->m_SofTodIdx+SOFTODS_SIZE-1)%SOFTODS_SIZE;
        DtTodTime  PrevSofTod =  pSofTods->m_SofTods[PrevIdx];

        // Compute frameperiod
        FramePeriod = DtCore_TOD_TimeDiff(*pSofTod, PrevSofTod);

        // Store frameperiod
        pSofTods->m_LastFramePeriod = (Int)FramePeriod;
        
        if (ExpectFramePeriod > 0)
        {
            // Compute deviation in ppm
            PpmDiff = DIV64(FramePeriod*Exp6, ExpectFramePeriod) - 1*Exp6;
            // If the deviation is too much, clear the history of timestamps
            if (PpmDiff<-MAX_DEVIATION_PPM || PpmDiff>MAX_DEVIATION_PPM)
            { 
                if ( pDf->m_GenRefType!=DT_DF_GENLOCKCTRL_GENREF_UNDEFINED
                                   && pDf->m_GenRefType!=DT_DF_GENLOCKCTRL_GENREF_VIRTUAL)
                    DtDbgOutDf(MIN, GENLOCKCTRL, pDf, "Unexpected GenRef deviation: %d",
                                                                                 PpmDiff);
                DtDfGenLockCtrl_SofTodsClear(pSofTods);
            }
        }
    }

    // Store new timestamp
    DT_ASSERT(pSofTods->m_SofTodIdx>=0 && pSofTods->m_SofTodIdx < SOFTODS_SIZE);
    pSofTods->m_SofTods[pSofTods->m_SofTodIdx++] = *pSofTod;
    pSofTods->m_SofTodIdx %= SOFTODS_SIZE;
    if (pSofTods->m_NumSofTods < SOFTODS_SIZE)
        pSofTods->m_NumSofTods++;
    pSofTods->m_TotalNumSofTods++;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_SofTodsGetLast -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtTodTime  DtDfGenLockCtrl_SofTodsGetLast(const DtDfGenLockCtrlSofTods* pSofTods)
{
    const Int SOFTODS_SIZE = DT_DF_GENLOCKCTRL_SOFTOD_SIZE;
    Int  PrevIdx = (pSofTods->m_SofTodIdx+SOFTODS_SIZE-1)%SOFTODS_SIZE;
    DT_ASSERT(pSofTods->m_NumSofTods > 0);
    return pSofTods->m_SofTods[PrevIdx];
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_SofTodsNumAvailable -.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtDfGenLockCtrl_SofTodsNumAvailable(const DtDfGenLockCtrlSofTods* pSofTods)
{
    return pSofTods->m_NumSofTods;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_SofTodsGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtTodTime  DtDfGenLockCtrl_SofTodsGet(const DtDfGenLockCtrlSofTods* pSofTods, Int Idx)
{
    DT_ASSERT(DtDfGenLockCtrl_SofTodsNumAvailable(pSofTods) > Idx);
    return pSofTods->m_SofTods[Idx];
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_SofTodsTimeDiff -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
//  Returns Time - latest SofTod
//
Int64 DtDfGenLockCtrl_SofTodsTimeDiff(const DtTodTime* pTime, 
                                                   const DtDfGenLockCtrlSofTods* pSofTods)
{
    DtTodTime  LastSofTod;
    if (pSofTods->m_NumSofTods <= 0)
        return (Int64)((1ULL<<63) - 1);
    LastSofTod = DtDfGenLockCtrl_SofTodsGetLast(pSofTods);
    return DtCore_TOD_TimeDiff(*pTime , LastSofTod);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_FramePeriodMeasureClear -.-.-.-.-.-.-.-.-.-.-.-
//
void DtDfGenLockCtrl_FramePeriodMeasureClear(DtDfGenLockCtrlFramePeriodMeasure* 
                                                                               pFpMeasure)
{
    pFpMeasure->m_Index = 0;
    pFpMeasure->m_NumValid = 0;
    pFpMeasure->m_BufSize = DT_DF_GENLOCKCTRL_MAX_NUM_FRAME_PERIODS;
    pFpMeasure->m_TotalFramePeriods = 0;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_FramePeriodMeasureAdd -.-.-.-.-.-.-.-.-.-.-.-.
//
void DtDfGenLockCtrl_FramePeriodMeasureAdd(DtDfGenLockCtrlFramePeriodMeasure* pFpMeasure,
                                                                          Int FramePeriod)
{
    DT_ASSERT(pFpMeasure != NULL);
    if (pFpMeasure->m_BufSize > 0)
    {
        // Store frame period
        Int  Index = pFpMeasure->m_Index;
        // If buffer is full => subtract eldest element
        if (pFpMeasure->m_NumValid == pFpMeasure->m_BufSize)
            pFpMeasure->m_TotalFramePeriods -= pFpMeasure->m_FramePeriods[Index];
        else // Buffer not full => we have an extra sample
            pFpMeasure->m_NumValid++;

        // Process the frame period
        pFpMeasure->m_FramePeriods[Index] = FramePeriod;
        pFpMeasure->m_TotalFramePeriods += FramePeriod;

        // Update index
        pFpMeasure->m_Index = (Index+1) % pFpMeasure->m_BufSize;
    }
}

// .-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_FramePeriodMeasureGetAverage -.-.-.-.-.-.-.-.-.-.-
//
//  Returns avarate frame period in pico seconds
//
Int64 DtDfGenLockCtrl_FramePeriodMeasureGetAverage(
                                      const DtDfGenLockCtrlFramePeriodMeasure* pFpMeasure)
{
    DT_ASSERT(pFpMeasure != NULL);
    if (pFpMeasure->m_NumValid > 0)
        return  DIV64(pFpMeasure->m_TotalFramePeriods*1000, pFpMeasure->m_NumValid);
    else
        return 0;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_GetDcoControlPars -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfGenLockCtrlDcoControlPars  DtDfGenLockCtrl_GetDcoControlPars(Int  VidStd)
{
    DtDfGenLockCtrlDcoControlPars  ControlPars = { 4800, 6, 1000, 17780};
    DtAvFrameProps  VidProps;
    Int FramePeriodNs, NumLines;
    DtStatus Status = DtAvGetFrameProps(VidStd, &VidProps);

    DT_ASSERT(DT_SUCCESS(Status));

    if (!DT_SUCCESS(Status))
        return ControlPars;
    
    // Control parameters depend on framerate   
    switch (VidProps.m_Fps)
    {
        case 24: ControlPars.m_FreqMult=68;  ControlPars.m_PhaseMult=750; break;
        case 25: ControlPars.m_FreqMult=65;  ControlPars.m_PhaseMult=720; break;
        case 30: ControlPars.m_FreqMult=54;  ControlPars.m_PhaseMult=600; break;
        case 50: ControlPars.m_FreqMult=32;  ControlPars.m_PhaseMult=360; break;
        case 60: ControlPars.m_FreqMult=27;  ControlPars.m_PhaseMult=300; break;
        default: DT_ASSERT(FALSE);
    }

    // Maximum step size 5ppb for HD and SD else 1ppb
    if (DtAvVidStdIsSdSdi(VidStd) || DtAvVidStdIsHdSdi(VidStd))
         ControlPars.m_MaxStepPpt = 5000;
    else
         ControlPars.m_MaxStepPpt = 1000;

    // Maximum 1/2 line phase difference
    FramePeriodNs = DtAvVidStd2FramePeriod(VidStd);
    NumLines = VidProps.m_NumLines;
    if (FramePeriodNs>0 && NumLines>0)
        ControlPars.m_MaxPhaseDiffNs = FramePeriodNs/(NumLines*2);

    return ControlPars;
}

// .-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_GetGenLockStatusInternal -.-.-.-.-.-.-.-.-.-.-.-
//
//  Must be called under spinlock protection
//
Int DtDfGenLockCtrl_GetGenLockStatusInternal(DtDfGenLockCtrl* pDf)
{
    if (pDf->m_GenRefStatus == DT_DF_GENLOCKCTRL_GENREFSTATUS_NO_SIGNAL)
        return DT_GENLOCKCTRL_STATE_NO_REF;

    if (pDf->m_GenRefStatus == DT_DF_GENLOCKCTRL_GENREFSTATUS_INVALID_SIGNAL)
        return DT_GENLOCKCTRL_STATE_INVALID_REF;

    switch (pDf->m_DcoControlState)
    {
    default:  DT_ASSERT(FALSE);
    case DT_DF_GENLOCKCTRL_STATE_INITIAL:          return DT_GENLOCKCTRL_STATE_LOCKING;
    case DT_DF_GENLOCKCTRL_STATE_FREE_RUNNING:     return DT_GENLOCKCTRL_STATE_FREE_RUN;
    case DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_FREQ_CHECK:
    case DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_FREQ:  return DT_GENLOCKCTRL_STATE_LOCKING;
    case DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_PHASE_WAIT_FOR_DONE:
    case DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_PHASE: return DT_GENLOCKCTRL_STATE_LOCKING;
    case DT_DF_GENLOCKCTRL_STATE_LOCKED:           return DT_GENLOCKCTRL_STATE_LOCKED;
    }
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_NotifyStatusChange -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtDfGenLockCtrl_NotifyStatusChange(DtDfGenLockCtrl* pDf, Int GenLockStatus)
{
    Bool Locked = FALSE;
    DtDriverEvent GenLockEvent;
    switch (GenLockStatus)
    {
    default:  DT_ASSERT(FALSE);
    case DT_GENLOCKCTRL_STATE_NO_REF:      Locked = FALSE; break;
    case DT_GENLOCKCTRL_STATE_INVALID_REF: Locked = FALSE; break;
    case DT_GENLOCKCTRL_STATE_LOCKING:     Locked = FALSE; break;
    case DT_GENLOCKCTRL_STATE_FREE_RUN:    Locked = TRUE; break;
    case DT_GENLOCKCTRL_STATE_LOCKED:      Locked = TRUE; break;
    }

    // Send GENLOCK event
    GenLockEvent.m_EventType = DT_DRV_EVENT_TYPE_GENLOCK;
    GenLockEvent.m_EventValue1 = Locked ? DT_DRV_EVENT_VALUE1_LOCKED 
                                                            : DT_DRV_EVENT_VALUE1_NO_LOCK;
    GenLockEvent.m_EventValue2 = 0;
    DtCore_EVENTS_Set(pDf->m_pCore, GenLockEvent , FALSE);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_NotifyLockChange -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtDfGenLockCtrl_NotifyLockChange(DtDfGenLockCtrl* pDf, Bool Locked)
{
    Int NumElems, i;

    // Send notification
    DtSpinLockAcquire(&pDf->m_SofSpinLock);
    // Notify all
    NumElems = DtVector_Size(pDf->m_pOnLockChangedHandlers);
    for (i=0; i<NumElems; i++)
    {
        const DtDfGenLockCtrlLockChangedRegData*  pRegData = 
        (DtDfGenLockCtrlLockChangedRegData*)DtVector_At(pDf->m_pOnLockChangedHandlers, i);
        DT_ASSERT(pRegData!=NULL && pRegData->m_OnLockChangedFunc!=NULL);
        if (pRegData==NULL || pRegData->m_OnLockChangedFunc==NULL)
            continue;
        DT_ASSERT(pRegData->m_pObject != NULL);
        pRegData->m_OnLockChangedFunc(pRegData->m_pObject, Locked);
    }
    DtSpinLockRelease(&pDf->m_SofSpinLock);
}
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfGenLockCtrl implementation +=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Df's stub
#define GENLOCKCTRL_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                   DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfGenLockCtrl));
// Helper macro to cast stub's DtDf member to DtDfGenLockCtrl
#define STUB_GENLOCKCTRL   ((DtIoStubDfGenLockCtrl*)pStub)
#define STUB_DF  ((DtDfGenLockCtrl*)STUB_GENLOCKCTRL->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtIoStubDfGenLockCtrl_AppendDynamicSize(const DtIoStub*,
                                                            DtIoStubIoParams * pIoParams);
static DtStatus  DtIoStubDfGenLockCtrl_OnCmd(const DtIoStub* pStub, 
                                             DtIoStubIoParams* pIoParams, Int * pOutSize);
static DtStatus  DtIoStubDfGenLockCtrl_OnCmdGetGenLockState(const DtIoStubDfGenLockCtrl*,
                                                   DtIoctlGenLockCtrlCmdGetStateOutput*);
static DtStatus  DtIoStubDfGenLockCtrl_OnCmdGetGenLockState2(const DtIoStubDfGenLockCtrl*,
                                                   DtIoctlGenLockCtrlCmdGetState2Output*);
static DtStatus  DtIoStubDfGenLockCtrl_OnCmdReLock(const DtIoStubDfGenLockCtrl*);
static DtStatus  DtIoStubDfGenLockCtrl_OnCmdGetDcoClockProperties(
                                        const DtIoStubDfGenLockCtrl*,
                                        const DtIoctlGenLockCtrlCmdGetDcoClockPropsInput*,
                                        DtIoctlGenLockCtrlCmdGetDcoClockPropsOutput*);
static DtStatus  DtIoStubDfGenLockCtrl_OnCmdGetDcoFreqOffset(const DtIoStubDfGenLockCtrl*,
                                        const DtIoctlGenLockCtrlCmdGetDcoFreqOffsetInput*,
                                        DtIoctlGenLockCtrlCmdGetDcoFreqOffsetOutput*);
static DtStatus  DtIoStubDfGenLockCtrl_OnCmdSetDcoFreqOffset(const DtIoStubDfGenLockCtrl*,
                                       const DtIoctlGenLockCtrlCmdSetDcoFreqOffsetInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_GENLOCKCTRL;

static const DtIoctlProperties  IOSTUB_DF_GENLOCKCTRL_IOCTLS[] = 
{
    DT_IOCTL_PROPS_GENLOCKCTRL_CMD(
        DtIoStubDfGenLockCtrl_OnCmd, 
        DtIoStubDfGenLockCtrl_AppendDynamicSize,
        NULL),
};

// =+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfGenLockCtrl - Public functions +=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfGenLockCtrl_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubDfGenLockCtrl_Close(DtIoStub* pStub)
{
    GENLOCKCTRL_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfGenLockCtrl_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubDfGenLockCtrl*  DtIoStubDfGenLockCtrl_Open(DtDf*  pDf)
{
    DtIoStubDfGenLockCtrl*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    DF_GENLOCKCTRL_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfGenLockCtrl, pDf, NULL, 
                                                            DtIoStubDfGenLockCtrl_Close,
                                                            NULL,  // Use default IOCTL
                                                            IOSTUB_DF_GENLOCKCTRL_IOCTLS);
    pStub = (DtIoStubDfGenLockCtrl*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

// =+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfGenLockCtrl - Private functions +=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfGenLockCtrl_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfGenLockCtrl_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlGenLockCtrlCmdInput*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfGenLockCtrl));
    DT_ASSERT(pIoParams != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_GENLOCKCTRL_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_GenLockCtrlCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_GENLOCKCTRL_CMD_GET_DCO_CLK_PROPS:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize 
                                  >= sizeof(DtIoctlGenLockCtrlCmdGetDcoClockPropsInput));
        DT_ASSERT(pIoParams->m_OutReqSize
                                  >= sizeof(DtIoctlGenLockCtrlCmdGetDcoClockPropsOutput));
        // Add dynamic size (i.e. #bytes to get)
        pIoParams->m_OutReqSize += pInData->m_GetDcoClkProps.m_MaxNumEntries 
                                                   * sizeof(DtIoctlGenLockCtrlClockProps);
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfGenLockCtrl_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfGenLockCtrl_OnCmd(
    const DtIoStub*  pStub, 
    DtIoStubIoParams*  pIoParams, 
    Int* pOutSize)
{
    DtStatus Status= DT_STATUS_OK;
    const DtIoctlGenLockCtrlCmdInput*  pInData = NULL;
    DtIoctlGenLockCtrlCmdOutput*  pOutData = NULL;
    
    GENLOCKCTRL_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_GENLOCKCTRL_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, GENLOCKCTRL, pStub, 
                                                   "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_GenLockCtrlCmd;
    if (pIoParams->m_OutReqSize > 0)
    { 
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_GenLockCtrlCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_GENLOCKCTRL_CMD_GET_STATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfGenLockCtrl_OnCmdGetGenLockState(STUB_GENLOCKCTRL, 
                                                                  &pOutData->m_GetState);
        break;
    case DT_GENLOCKCTRL_CMD_GET_STATE2:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfGenLockCtrl_OnCmdGetGenLockState2(STUB_GENLOCKCTRL, 
                                                                  &pOutData->m_GetState2);
        break;
    case DT_GENLOCKCTRL_CMD_RELOCK:
        Status = DtIoStubDfGenLockCtrl_OnCmdReLock(STUB_GENLOCKCTRL);
        break;
    case DT_GENLOCKCTRL_CMD_GET_DCO_CLK_PROPS:
        Status = DtIoStubDfGenLockCtrl_OnCmdGetDcoClockProperties(STUB_GENLOCKCTRL,
                                                             &pInData->m_GetDcoClkProps,
                                                             &pOutData->m_GetDcoClkProps);
        break;
    case DT_GENLOCKCTRL_CMD_GET_DCO_FREQ_OFFSET:
        Status = DtIoStubDfGenLockCtrl_OnCmdGetDcoFreqOffset(STUB_GENLOCKCTRL,
                                                           &pInData->m_GetDcoFreqOffset,
                                                           &pOutData->m_GetDcoFreqOffset);
        break;
    case DT_GENLOCKCTRL_CMD_SET_DCO_FREQ_OFFSET:
        Status = DtIoStubDfGenLockCtrl_OnCmdSetDcoFreqOffset(STUB_GENLOCKCTRL,
                                                            &pInData->m_SetDcoFreqOffset);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubDfGenLockCtrl_OnCmdGetGenLockState -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfGenLockCtrl_OnCmdGetGenLockState(
    const DtIoStubDfGenLockCtrl*  pStub, 
    DtIoctlGenLockCtrlCmdGetStateOutput*  pOutData)
{
    Int64 SofCount;
    Int IsSofTodValid;
    DtTodTime LastSofTod;
    Int TimeSinceLastSof;

    GENLOCKCTRL_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtDfGenLockCtrl_GetGenLockState(STUB_DF, &pOutData->m_GenLockState,
                               &pOutData->m_RefVidStd, &pOutData->m_DetVidStd,
                               &SofCount, &IsSofTodValid, &LastSofTod, &TimeSinceLastSof);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubDfGenLockCtrl_OnCmdGetGenLockState2 -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfGenLockCtrl_OnCmdGetGenLockState2(
    const DtIoStubDfGenLockCtrl*  pStub, 
    DtIoctlGenLockCtrlCmdGetState2Output*  pOutData)
{
    GENLOCKCTRL_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);


    return DtDfGenLockCtrl_GetGenLockState(STUB_DF, &pOutData->m_GenLockState,
                                     &pOutData->m_RefVidStd, &pOutData->m_DetVidStd,
                                     &pOutData->m_SofCount, &pOutData->m_IsSofValid,
                                     &pOutData->m_SofTime, &pOutData->m_TimeSinceLastSof);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfGenLockCtrl_OnCmdReLock -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfGenLockCtrl_OnCmdReLock(
    const DtIoStubDfGenLockCtrl*  pStub)
{
    GENLOCKCTRL_STUB_DEFAULT_PRECONDITIONS(pStub);

    return DtDfGenLockCtrl_ReLock(STUB_DF);
}

// .-.-.-.-.-.-.-.-.- DtIoStubDfGenLockCtrl_OnCmdGetDcoClockProperties -.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfGenLockCtrl_OnCmdGetDcoClockProperties(
    const DtIoStubDfGenLockCtrl* pStub,
    const DtIoctlGenLockCtrlCmdGetDcoClockPropsInput*  pInData,
    DtIoctlGenLockCtrlCmdGetDcoClockPropsOutput*  pOutData)
{
    GENLOCKCTRL_STUB_DEFAULT_PRECONDITIONS(pStub);
    return DtDfGenLockCtrl_GetDcoClockProperties(STUB_DF, pInData->m_MaxNumEntries,
                                                        &pOutData->m_NumEntries,
                                                        pOutData->m_Properties);
}
// -.-.-.-.-.-.-.-.-.-.- DtIoStubDfGenLockCtrl_OnCmdGetDcoFreqOffset -.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfGenLockCtrl_OnCmdGetDcoFreqOffset(
    const DtIoStubDfGenLockCtrl* pStub,
    const DtIoctlGenLockCtrlCmdGetDcoFreqOffsetInput* pInData,
    DtIoctlGenLockCtrlCmdGetDcoFreqOffsetOutput* pOutData)
{
    GENLOCKCTRL_STUB_DEFAULT_PRECONDITIONS(pStub);
    return DtDfGenLockCtrl_GetDcoFreqOffset(STUB_DF, pInData->m_ClockIdx,
                                                        &pOutData->m_DcoFreqOffsetPpt,
                                                        &pOutData->m_DcoFrequencyMicroHz);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubDfGenLockCtrl_OnCmdSetDcoFreqOffset -.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfGenLockCtrl_OnCmdSetDcoFreqOffset(
    const DtIoStubDfGenLockCtrl* pStub,
    const DtIoctlGenLockCtrlCmdSetDcoFreqOffsetInput* pInData)
{
    GENLOCKCTRL_STUB_DEFAULT_PRECONDITIONS(pStub);
    return DtDfGenLockCtrl_SetDcoFreqOffset(STUB_DF, pInData->m_ClockIdx, 
                                                             pInData->m_DcoFreqOffsetPpt);
}
