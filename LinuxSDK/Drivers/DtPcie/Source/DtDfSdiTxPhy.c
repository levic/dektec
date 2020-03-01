//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSdiTxPhy.c *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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
#include "DtDfSdiTxPhy.h"
#include "DtAudioVideo.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        
#define SDITXPHY_ROLE_NONE      NULL
#define SDITXPLL_ROLE_NONE      NULL
#define SDIXCFGMGR_ROLE_NONE    NULL


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSdiTxPhy implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiTx function
#define DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfSdiTxPhy))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_SDITXPHY_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(SDITXPHY, pDf)

// Helper macro to cast a DtDf* to a DtDfSdiTxPhy*
#define DF_SDITXPHY      ((DtDfSdiTxPhy*)pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfSdiTxPhy_Init(DtDf*);
static void  DtDfSdiTxPhy_GenLockStartOfFrameHandler(DtObject*, const DtTodTime*);
static DtStatus  DtDfSdiTxPhy_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfSdiTxPhy_LoadParameters(DtDf*);
static DtStatus  DtDfSdiTxPhy_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfSdiTxPhy_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfSdiTxPhy_OpenChildren(DtDfSdiTxPhy*);
static void  DtDfSdiTxPhy_PeriodicIntervalHandler(DtObject*, DtTodTime  Time);
static DtStatus  DtDfSdiTxPhy_CheckSdiRate(DtDfSdiTxPhy*, Int SdiRate);
static DtStatus  DtDfSdiTxPhy_ConfigureRate(DtDfSdiTxPhy*, Int SdiRate, Bool Fractional);
static DtStatus  DtDfSdiTxPhy_ConfigureRateC10A10(DtDfSdiTxPhy*, Int SdiRate, Bool Fract);
static DtStatus  DtDfSdiTxPhy_ConfigureRateCV(DtDfSdiTxPhy*, Int SdiRate, Bool Fract);
static void  DtDfSdiTxPhy_SdiLockStateUpdate(DtDfSdiTxPhy*  pDf);
static void  DtDfSdiTxPhy_SdiLockStateUpdateCV(DtDfSdiTxPhy*  pDf);
static void  DtDfSdiTxPhy_OnGenLockChanged(DtObject* , Bool Lock);
static Int  DtDfSdiTxPhy_VidStd2SdiRate(Int  VidStd);
static DtStatus  DtDfSdiTxPhy_ComputeSofDelay(DtDfSdiTxPhy*, Int VidStd, Int* pSofDelay);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSdiTxPhy - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfSdiTxPhy_Close(DtDf*  pDf)
{
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Unregister periodic interrupt handler
    DtCore_TOD_PeriodicItvUnregister(pDf->m_pCore, (DtObject*)pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfSdiTxPhy*  DtDfSdiTxPhy_Open(DtCore*  pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_SDITXPHY_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfSdiTxPhy, Id, DT_FUNC_TYPE_SDITXPHY, pPt,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfSdiTxPhy_Close;
    OpenParams.m_InitFunc = DtDfSdiTxPhy_Init;
    OpenParams.m_LoadParsFunc = DtDfSdiTxPhy_LoadParameters;
    OpenParams.m_OnCloseFileFunc = DtDfSdiTxPhy_OnCloseFile;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfSdiTxPhy_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfSdiTxPhy_OnEnablePreChildren;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfSdiTxPhy*)DtDf_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_ClearUnderflowFlag -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSdiTxPhy_ClearUnderflowFlag(DtDfSdiTxPhy* pDf)
{
    // Sanity check
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);

    return  DtBcSDITXPHY_ClearUnderflowFlag(pDf->m_pBcSdiTxPhy);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_GetGenLockStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiTxPhy_GetGenLockStatus(DtDfSdiTxPhy* pDf, Int* pGenLockStatus)
{
    // Sanity check
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pGenLockStatus == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);

    if (pDf->m_pDfGenLockCtrl==NULL || !pDf->m_CapGenLocked)
        return DT_STATUS_NOT_SUPPORTED;

    *pGenLockStatus = pDf->m_PhyIsGenLocked ? DT_SDITXPHY_GENLOCK_LOCKED 
                                                            : DT_SDITXPHY_GENLOCK_NO_LOCK;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_GetMaxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiTxPhy_GetMaxSdiRate(DtDfSdiTxPhy* pDf, Int* pMaxSdiRate)
{
    // Sanity check
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pMaxSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);

    // Return cached value
    *pMaxSdiRate = pDf->m_PhyMaxSdiRate;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiTxPhy_GetOperationalMode(DtDfSdiTxPhy* pDf, Int* pOpMode)
{
    // Sanity check
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);

    // Return cached operational mode
    *pOpMode = pDf->m_OperationalMode;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_GetVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiTxPhy_GetVidStd(DtDfSdiTxPhy* pDf, Int* pVidStd)
{
    // Sanity check
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pVidStd == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);
    
    // Cannot get video standard if TX-mode is ASI
    if (pDf->m_TxMode == DT_SDITXPHY_TXMODE_ASI)
        return DT_STATUS_NOT_SUPPORTED;

    // Return cached configured video standard
    *pVidStd = pDf->m_VidStd;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_GetSdiStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSdiTxPhy_GetSdiStatus(DtDfSdiTxPhy*  pDf, Int*  pIsTxLocked)
{
    DtStatus  Status=DT_STATUS_OK;
    Bool  IsPllLocked = FALSE;
    Int PllId = -1;

    // Sanity check
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pIsTxLocked == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);

    // Get locked status
    DtSpinLockAcquire(&pDf->m_SpinLock);

    // Call device family specific configure function
    switch (pDf->m_PhyDeviceFamily)
    {
    case DT_BC_SDITXPHY_FAMILY_A10:
    case DT_BC_SDITXPHY_FAMILY_C10:
        Status = DtBcSDITXPHY_GetTxPllId(pDf->m_pBcSdiTxPhy, &PllId);
        if (DT_SUCCESS(Status))
            Status = DtDfSi534X_IsPllLocked(pDf->m_pDfSi534X, PllId, &IsPllLocked);
        break;
    case DT_BC_SDITXPHY_FAMILY_CV:
        Status = DtBcSDITXPLL_IsPllLocked(pDf->m_pBcSdiTxPll, &IsPllLocked);
        break;
    default:
        DT_ASSERT(FALSE);
        DtDbgOutDf(ERR, SDITXPHY, pDf, "Unsupported device family");
        Status = DT_STATUS_NOT_SUPPORTED;
    }
    DtSpinLockRelease(&pDf->m_SpinLock);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_GetStartOfFrameOffset -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiTxPhy_GetStartOfFrameOffset(DtDfSdiTxPhy* pDf, Int* pSofOffset)
{
    // Sanity check
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pSofOffset == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);

    // Return cached start-of-frame offset
    *pSofOffset = pDf->m_PhySofOffsetNs;

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_GetTxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiTxPhy_GetTxMode(DtDfSdiTxPhy* pDf, Int* pTxMode)
{
    // Sanity check
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pTxMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);

    // Return cached TX-mode
    *pTxMode = pDf->m_TxMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_GetUnderflowFlag -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiTxPhy_GetUnderflowFlag(DtDfSdiTxPhy* pDf, Bool* pUflFlag)
{
    // Sanity check
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);

    return DtBcSDITXPHY_GetUnderflowFlag(pDf->m_pBcSdiTxPhy, pUflFlag);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSdiTxPhy_SetOperationalMode(DtDfSdiTxPhy*  pDf, Int  OpMode)
{
    const DtTodTime  Immediately = {0, 0};
    return DtDfSdiTxPhy_SetOperationalModeTimed( pDf, OpMode, Immediately);
}
// .-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_SetOperationalModeTimed -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSdiTxPhy_SetOperationalModeTimed(DtDfSdiTxPhy*  pDf, Int  OpMode, 
                                                                     DtTodTime  StartTime)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (OpMode!=DT_FUNC_OPMODE_IDLE && OpMode!=DT_FUNC_OPMODE_STANDBY
                                                    && OpMode!=DT_FUNC_OPMODE_RUN)
    {
        DtDbgOutDf(ERR, SDITXPHY, pDf, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }
    if ((StartTime.m_Nanoseconds!=0 && StartTime.m_Seconds!=0) && (!pDf->m_CapGenLocked 
                                                           || OpMode!=DT_FUNC_OPMODE_RUN))
    {
        DtDbgOutDf(ERR, SDITXPHY, pDf, "Invalid start time");
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);

    // No change?
    if (pDf->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Lock the LockState
    DtSpinLockAcquire(&pDf->m_SpinLock);

    // When going to standby/run, we need to wait for TX-lock for Cyclone V TxPhy
    if ((OpMode==DT_FUNC_OPMODE_STANDBY || OpMode==DT_FUNC_OPMODE_RUN)
                                      && pDf->m_LockState!=SDITXPHY_STATE_TX_LOCKED 
                                      && pDf->m_PhyDeviceFamily==DT_BC_SDITXPHY_FAMILY_CV)
    {
        // Wait maximum 100ms
        Int  TimeoutCount = 100;
        while (pDf->m_LockState!=SDITXPHY_STATE_TX_LOCKED && TimeoutCount>0)
        {
            DtSpinLockRelease(&pDf->m_SpinLock);
            DtSleep(1);
            DtSpinLockAcquire(&pDf->m_SpinLock);
            TimeoutCount--;
        }
        if (TimeoutCount <=  0)
        { 
            DtDbgOutDf(ERR, SDITXPHY, pDf, "TXPHY not locked");
            Status = DT_STATUS_TIMEOUT;
        }
    }

    if (DT_SUCCESS(Status))
    {
        Int  PhyOpMode = DT_BLOCK_OPMODE_IDLE;
        switch (OpMode)
        {
        case DT_FUNC_OPMODE_IDLE:     PhyOpMode = DT_BLOCK_OPMODE_IDLE; break;
        case DT_FUNC_OPMODE_STANDBY:  PhyOpMode = DT_BLOCK_OPMODE_STANDBY; break;
        case DT_FUNC_OPMODE_RUN:      PhyOpMode = DT_BLOCK_OPMODE_RUN; break;
        default: DT_ASSERT(FALSE);
        }
        
        // Do we need to start on start-of-frame (SDI-GenLock)
        if (pDf->m_CapGenLocked && pDf->m_TxMode==DT_SDITXPHY_TXMODE_SDI
                                                        && PhyOpMode==DT_BLOCK_OPMODE_RUN)
        {
            // Make sure PHY is in STANDBY
            Int  CurPhyOpStatus;
            Status = DtBcSDITXPHY_GetOperationalStatus(pDf->m_pBcSdiTxPhy, 
                                                                         &CurPhyOpStatus);
            if (DT_SUCCESS(Status) && CurPhyOpStatus!=DT_BLOCK_OPSTATUS_STANDBY)
                Status = DtBcSDITXPHY_SetOperationalMode(pDf->m_pBcSdiTxPhy,
                                                                 DT_BLOCK_OPMODE_STANDBY);
            // Update the TxPhy GenLock status
            pDf->m_PhyIsGenLocked = pDf->m_GenLockIsLocked;

            // Arm for start-on-frame
            if (DT_SUCCESS(Status))
                Status = DtBcSDITXPHY_ArmForSof(pDf->m_pBcSdiTxPhy);
        }
        if (DT_SUCCESS(Status))
        {
            // Perform immediately?
            if (StartTime.m_Nanoseconds==0 && StartTime.m_Seconds==0)
            {
                Status = DtBcSDITXPHY_SetOperationalMode(pDf->m_pBcSdiTxPhy, PhyOpMode);
                pDf->m_DelayedStart = FALSE;
            }
            else
            {   
                // Start can only be delayed for genlocked capable ports
                DT_ASSERT(OpMode==DT_FUNC_OPMODE_RUN && pDf->m_CapGenLocked);
                pDf->m_DelayedStart = TRUE;
                pDf->m_DelayedStartTime = StartTime;
            }
        }

        // Save new operational mode
        if (DT_SUCCESS(Status))
            pDf->m_OperationalMode = OpMode;
    }
    DtSpinLockRelease(&pDf->m_SpinLock);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_SetVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiTxPhy_SetVidStd(DtDfSdiTxPhy* pDf, Int VidStd)
{
    DtStatus  Status=DT_STATUS_OK;
    Int  SofDelayClks = 0;
    Int  SdiRate = DT_DRV_SDIRATE_UNKNOWN;
    Bool Fractional = FALSE;

    // Sanity checks
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);

    // Must be in IDLE
    if (pDf->m_OperationalMode != DT_FUNC_OPMODE_IDLE)
    {
        DtDbgOutDf(ERR, SDITXPHY, pDf, "Function not in IDLE");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // Cannot change SDI-rate if TX-mode is ASI
    if (pDf->m_TxMode == DT_SDITXPHY_TXMODE_ASI)
        return DT_STATUS_NOT_SUPPORTED;

    // No change?
    if (pDf->m_VidStd == VidStd)
        return DT_STATUS_OK;

    //  Update Start-of-Frame delay
    DT_RETURN_ON_ERROR(DtDfSdiTxPhy_ComputeSofDelay(pDf, VidStd, &SofDelayClks));
    DT_RETURN_ON_ERROR(DtBcSDITXPHY_SetSofDelay(pDf->m_pBcSdiTxPhy, SofDelayClks));

    // -.-.-.-.- Check whether SDI-rate has changed with the new video standard -.-.-.-.-.
    SdiRate = DtDfSdiTxPhy_VidStd2SdiRate(VidStd);
    Fractional =  DtAvVidStdUsesFractionalClock(VidStd);

    // Check whether SDI-rate is supported
    DT_RETURN_ON_ERROR(DtDfSdiTxPhy_CheckSdiRate(pDf, SdiRate));

    // No SDI-rate or fractional change?
    if (pDf->m_SdiRate==SdiRate && pDf->m_FractionalClock==Fractional)
    { 
        // Update cache
        pDf->m_VidStd = VidStd;
        return DT_STATUS_OK;
    }

    // Configure new rate
    Status = DtDfSdiTxPhy_ConfigureRate(pDf, SdiRate, Fractional);
    if (DT_SUCCESS(Status))
    {
        // Update cache
        pDf->m_VidStd = VidStd;
        pDf->m_SdiRate = SdiRate;
        pDf->m_FractionalClock = Fractional;
    }


    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_SetStartOfFrameOffset -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiTxPhy_SetStartOfFrameOffset(DtDfSdiTxPhy* pDf, Int SofOffset)
{
    Int  SofDelayClks = 0;

    // Sanity checks
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);

    // Check parameters
    // Maximum +/- 100us
    if (SofOffset<-100000 || SofOffset>100000)
    {
        DtDbgOutDf(ERR, SDITXPHY, pDf, "Invalid start-of-frame offset");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be in IDLE
    if (pDf->m_OperationalMode != DT_FUNC_OPMODE_IDLE)
    {
        DtDbgOutDf(ERR, SDITXPHY, pDf, "Function not in IDLE");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // No change?
    if (pDf->m_PhySofOffsetNs == SofOffset)
        return DT_STATUS_OK;

    // Save new setting
    pDf->m_PhySofOffsetNs = SofOffset;

    //  Update Start-of-Frame delay
    DT_RETURN_ON_ERROR(DtDfSdiTxPhy_ComputeSofDelay(pDf, pDf->m_VidStd, &SofDelayClks));
    DT_RETURN_ON_ERROR(DtBcSDITXPHY_SetSofDelay(pDf->m_pBcSdiTxPhy, SofDelayClks));


    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_SetTxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiTxPhy_SetTxMode(DtDfSdiTxPhy* pDf, Int TxMode)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  SofDelayClks;


    // Sanity checks
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);

    // Check parameters
    if (TxMode!=DT_SDITXPHY_TXMODE_ASI && TxMode!=DT_SDITXPHY_TXMODE_SDI)
    {
        DtDbgOutDf(ERR, SDITXPHY, pDf, "Invalid TX-mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be in IDLE
    if (pDf->m_OperationalMode != DT_FUNC_OPMODE_IDLE)
    {
        DtDbgOutDf(ERR, SDITXPHY, pDf, "Function not in IDLE");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // No change?
    if (pDf->m_TxMode == TxMode)
        return DT_STATUS_OK;

    // Set new Start-of-Frame delay
    if (TxMode == DT_SDITXPHY_TXMODE_SDI)
        DT_RETURN_ON_ERROR(DtDfSdiTxPhy_ComputeSofDelay(pDf, pDf->m_VidStd,
                                                                          &SofDelayClks));
    else
       SofDelayClks = 0;
    DT_RETURN_ON_ERROR(DtBcSDITXPHY_SetSofDelay(pDf->m_pBcSdiTxPhy, SofDelayClks));

    // Configure new TxMode
    pDf->m_TxMode = TxMode;

    if (pDf->m_TxMode == DT_SDITXPHY_TXMODE_SDI)
        Status = DtDfSdiTxPhy_ConfigureRate(pDf, pDf->m_SdiRate, pDf->m_FractionalClock);
    else
        // ASI uses SD-SDI-rate
        Status = DtDfSdiTxPhy_ConfigureRate(pDf, DT_DRV_SDIRATE_SD, FALSE);

    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSdiTxPhy - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSdiTxPhy_Init(DtDf*  pDfBase)
{
    DtDfSdiTxPhy* pDf = (DtDfSdiTxPhy*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
    DtOnPeriodicIntervalRegData  RegData;
    DtIoCaps  IoCaps;
    Int PortIndex;

    // Sanity checks
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Initialize the spin lock
    DtSpinLockInit(&pDf->m_SpinLock);

    // Set defaults
    pDf->m_LockState = SDITXPHY_STATE_RECONFIG;
    pDf->m_OperationalMode = DT_FUNC_OPMODE_IDLE;
    pDf->m_TxMode = DT_SDITXPHY_TXMODE_SDI;
    pDf->m_VidStd = DT_VIDSTD_1080I50;
    pDf->m_SdiRate = DT_DRV_SDIRATE_HD;
    pDf->m_FractionalClock = FALSE;
    pDf->m_PhyNumClocks = 0;
    pDf->m_PhyDeviceFamily = DT_BC_SDITXPHY_FAMILY_UNKNOWN;
    pDf->m_PhyMaxSdiRate = DT_DRV_SDIRATE_HD;
    pDf->m_PhyIsGenLocked = FALSE;
    pDf->m_GenLockIsLocked = FALSE;
    pDf->m_DelayedStart = FALSE;
    pDf->m_DelayedStartTime.m_Nanoseconds = 0;
    pDf->m_DelayedStartTime.m_Seconds = 0;
    pDf->m_PhySofOffsetNs = 0;
    // Get the GenLocked-IO-capability of the port
    pDf->m_CapGenLocked = FALSE;
    PortIndex = DtCore_PT_GetPortIndex(pDf->m_pPt);
    Status = DtCore_PT_GetIoCaps(pDf->m_pPt, &IoCaps);
    if (DT_SUCCESS(Status))
    {
        pDf->m_CapGenLocked = DtIoCapsHasCap(&IoCaps, DT_IOCAP_GENLOCKED);
    }
  
    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfSdiTxPhy_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SDITXPHY, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    // Init periodic interval handler enable flag and spinlock
    DtSpinLockInit(&pDf->m_PerItvSpinLock);
    pDf->m_PerItvEnable = FALSE;

    // Register periodic interval handler
    RegData.m_OnPeriodicFunc = DtDfSdiTxPhy_PeriodicIntervalHandler;
    RegData.m_pObject = (DtObject*)pDf;
    Status = DtCore_TOD_PeriodicItvRegister(pDf->m_pCore, &RegData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SDITXPHY, pDf, "ERROR: failed to register period itv handler");
        return Status;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_GenLockStartOfFrameHandler -.-.-.-.-.-.-.-.-.-.-.-
//
void DtDfSdiTxPhy_GenLockStartOfFrameHandler(DtObject* pObj, const DtTodTime* pSofTod)
{
    DtDfSdiTxPhy*  pDf = (DtDfSdiTxPhy*)pObj;
    DtStatus  Status=DT_STATUS_OK;
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);
    DtSpinLockAcquireAtDpc(&pDf->m_SpinLock);
    if (pDf->m_DelayedStart)
    {
        Int64 TimeDiff = DtCore_TOD_TimeDiff(pDf->m_DelayedStartTime, *pSofTod);
        if (TimeDiff <= 0)
        { 
            Status = DtBcSDITXPHY_SetOperationalMode(pDf->m_pBcSdiTxPhy, 
                                                                     DT_BLOCK_OPMODE_RUN);
            DT_ASSERT(DT_SUCCESS(Status));
            pDf->m_DelayedStart = FALSE;
        }
    }
    DtSpinLockReleaseFromDpc(&pDf->m_SpinLock);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Note: LoadParameters() is called before the Init(). The loaded parameters can be used
// in the Init().
//
DtStatus  DtDfSdiTxPhy_LoadParameters(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfSdiTxPhy* pDf = (DtDfSdiTxPhy*)pDfBase;

    // List of SdiTxPhy function parameters
    DtDfParameters  DFSDITXPHY_PARS[] =
    {
        // Name,  Value Type,  Value*
        { "TX_PHY_DELAY_SD",  PROPERTY_VALUE_TYPE_INT, &(pDf->m_PhyDelayNsSd)  },
        { "TX_PHY_DELAY_HD",  PROPERTY_VALUE_TYPE_INT, &(pDf->m_PhyDelayNsHd)  },
        { "TX_PHY_DELAY3G",  PROPERTY_VALUE_TYPE_INT, &(pDf->m_PhyDelayNs3g)  },
        { "TX_PHY_DELAY6G",  PROPERTY_VALUE_TYPE_INT, &(pDf->m_PhyDelayNs6g)  },
        { "TX_PHY_DELAY12G", PROPERTY_VALUE_TYPE_INT, &(pDf->m_PhyDelayNs12g) },
    };

    // Sanity checks
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Set defaults
    pDf->m_PhyDelayNsSd = pDf->m_PhyDelayNsHd = pDf->m_PhyDelayNs3g = 0;
    pDf->m_PhyDelayNs6g = pDf->m_PhyDelayNs12g = 0;

    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFSDITXPHY_PARS), 
                                                                         DFSDITXPHY_PARS);
    if (!DT_SUCCESS(Status))
        return Status;

    return DT_STATUS_OK;
}
// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiTxPhy_OnEnablePostChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtDfSdiTxPhy* pDf = (DtDfSdiTxPhy*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
    Int  SofDelayClks;
     
    // Sanity check
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);


    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutDf(AVG, SDITXPHY, pDf, "DISABLE -> ENABLE");

        // Operational mode must be IDLE
        DT_ASSERT(pDf->m_OperationalMode == DT_FUNC_OPMODE_IDLE);

        //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Start in IDLE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
        // Set operational mode of SDITXPHY to IDLE
        DT_RETURN_ON_ERROR(DtBcSDITXPHY_SetOperationalMode(pDf->m_pBcSdiTxPhy,
                                                                   DT_BLOCK_OPMODE_IDLE));
        // Get TXPHY configuration
        // Num clocks
        DT_RETURN_ON_ERROR(DtBcSDITXPHY_GetNumClocks(pDf->m_pBcSdiTxPhy, 
                                                                   &pDf->m_PhyNumClocks));
        DT_ASSERT(pDf->m_PhyNumClocks==1 || pDf->m_PhyNumClocks==2);
        // Phy's device family
        DT_RETURN_ON_ERROR(DtBcSDITXPHY_GetDeviceFamily(pDf->m_pBcSdiTxPhy, 
                                                                &pDf->m_PhyDeviceFamily));
        // Maximum SDI-rate
        DT_RETURN_ON_ERROR(DtBcSDITXPHY_GetMaxSdiRate(pDf->m_pBcSdiTxPhy, 
                                                                  &pDf->m_PhyMaxSdiRate));

        // Configure defaults
        pDf->m_TxMode = DT_SDITXPHY_TXMODE_SDI;
        pDf->m_VidStd = DT_VIDSTD_1080I50;
        pDf->m_SdiRate = DT_DRV_SDIRATE_HD;
        pDf->m_FractionalClock = FALSE;
        pDf->m_PhySofOffsetNs = 0;

        //  Update Start-of-Frame delay and configure the rate
        Status = DtDfSdiTxPhy_ComputeSofDelay(pDf, pDf->m_VidStd, &SofDelayClks);
        if (DT_SUCCESS(Status))
            Status = DtBcSDITXPHY_SetSofDelay(pDf->m_pBcSdiTxPhy, SofDelayClks);
        if (DT_SUCCESS(Status))
            Status = DtDfSdiTxPhy_ConfigureRate(pDf, pDf->m_SdiRate, 
                                                                  pDf->m_FractionalClock);

        // Enable periodic interval handler
        DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
        pDf->m_PerItvEnable = TRUE;
        DtSpinLockRelease(&pDf->m_PerItvSpinLock);

        if (DT_SUCCESS(Status) && pDf->m_CapGenLocked && pDf->m_pDfGenLockCtrl!=NULL)
        {
            // Register GenLock status changed callback
            DtDfGenLockCtrlLockChangedRegData  RegData;
            RegData.m_pObject = (DtObject*)pDf;
            RegData.m_OnLockChangedFunc = DtDfSdiTxPhy_OnGenLockChanged;
            Status = DtDfGenLockCtrl_LockChangedRegister(pDf->m_pDfGenLockCtrl, &RegData);
            if (DT_SUCCESS(Status))
            {
                // Update GenLock status
                Bool IsLocked;
                Status = DtDfGenLockCtrl_IsLocked(pDf->m_pDfGenLockCtrl, &IsLocked);
                pDf->m_GenLockIsLocked = IsLocked;
                pDf->m_PhyIsGenLocked = FALSE;
            }
        }
        // Register Genlock start-of-frame handler
        if (DT_SUCCESS(Status) && pDf->m_CapGenLocked && pDf->m_pBcGenLock!=NULL)
        {
            DtBcGENLOnStartOfFrameRegData RegData;
            RegData.m_OnStartOfFrameFunc = DtDfSdiTxPhy_GenLockStartOfFrameHandler;
            RegData.m_pObject = (DtObject*)pDf;
            Status = DtBcGENL_StartOfFrameRegister(pDf->m_pBcGenLock, &RegData);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOutDf(ERR, SDITXPHY, pDf,
                                      "ERROR: failed to register start-of-frame handler");
                return Status;
    }
        }
    }
    return Status;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_OnEnablePreChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSdiTxPhy_OnEnablePreChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtDfSdiTxPhy* pDf = (DtDfSdiTxPhy*)pDfBase;
    // Sanity check
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);
    if (!Enable)
    {
        // ENABLE -> DISABLE
        DtDbgOutDf(AVG, SDITXPHY, pDf, "ENABLE -> DISABLE");

        // Unregister start of frame handlers
        if (pDf->m_CapGenLocked && pDf->m_pBcGenLock!=NULL)
            DtBcGENL_StartOfFrameUnregister(pDf->m_pBcGenLock, (DtObject*)pDf);
        // Unregister GenLock status changed callback
        if(pDf->m_CapGenLocked && pDf->m_pDfGenLockCtrl!=NULL)
        { 
            DtStatus Status;
            Status = DtDfGenLockCtrl_LockChangedUnregister(pDf->m_pDfGenLockCtrl,
                                                                          (DtObject*)pDf);
            if (!DT_SUCCESS(Status))
                DtDbgOutDf(ERR, SDITXPHY, pDf, "Failed to unregister");
        }
        // Stop relocking
        DtSpinLockAcquire(&pDf->m_SpinLock);
        pDf->m_LockState = SDITXPHY_STATE_RECONFIG;
        DtSpinLockRelease(&pDf->m_SpinLock);

        // Disable periodic interval handler
        DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
        pDf->m_PerItvEnable = FALSE;
        DtSpinLockRelease(&pDf->m_PerItvSpinLock);

        //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Return to IDLE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        DT_RETURN_ON_ERROR(DtDfSdiTxPhy_SetOperationalMode(pDf, DT_FUNC_OPMODE_IDLE));
    }

    return DT_STATUS_OK;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtDfSdiTxPhy_OnCloseFile(DtDf* pDf, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    // Sanity checks
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Check if the owner closed the file handle
    Status = DtDf_ExclAccessCheck(pDf, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutDf(AVG, SDITXPHY, pDf, "Go to IDLE");

        // Go to IDLE
        Status = DtDfSdiTxPhy_SetOperationalMode(DF_SDITXPHY, DT_FUNC_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, SDITXPHY, pDf, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtDf_OnCloseFile(pDf, pFile);
}

 
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSdiTxPhy_OpenChildren(DtDfSdiTxPhy*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

     // List of children supported by the the SDITXPHY function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_SDITXPHY, DT_BC_SDITXPHY_NAME,
                       SDITXPHY_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcSdiTxPhy), TRUE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_SDITXPLL, DT_BC_SDITXPLL_NAME,
                      SDITXPLL_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcSdiTxPll), FALSE},
    };

    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    // SDITXPHY should be present
    DT_ASSERT(pDf->m_pBcSdiTxPhy != NULL);

    // Find the shared device level SdiXcvrReconfig manager (optional)
    pDf->m_pDfSdiXCfgMgr = (DtDfSdiXCfgMgr*)DtCore_DF_Find(pDf->m_pCore, NULL,
                                           DT_FUNC_TYPE_SDIXCFGMGR, SDIXCFGMGR_ROLE_NONE);

    // Find device level clock generator (optional)
    pDf->m_pDfSi534X  = (DtDfSi534X*)DtCore_DF_Find(pDf->m_pCore, NULL,
                                                               DT_FUNC_TYPE_SI534X, NULL);
    // Find device level GenLock controller (optional)
    pDf->m_pDfGenLockCtrl  = (DtDfGenLockCtrl*)DtCore_DF_Find(pDf->m_pCore, NULL,
                                                          DT_FUNC_TYPE_GENLOCKCTRL, NULL);
    // Find device level GenLock (optional)
    pDf->m_pBcGenLock  = (DtBcGENL*)DtCore_BC_Find(pDf->m_pCore, NULL,
                                                                DT_BLOCK_TYPE_GENL, NULL);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_PeriodicIntervalHandler -.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfSdiTxPhy_PeriodicIntervalHandler(DtObject* pObj, DtTodTime  Time)
{
    DtDfSdiTxPhy*  pDf = (DtDfSdiTxPhy*)pObj;
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfSdiTxPhy));

    DtSpinLockAcquireAtDpc(&pDf->m_PerItvSpinLock);
    // Only when enabled
    if (pDf->m_PerItvEnable)
        DtDfSdiTxPhy_SdiLockStateUpdate(pDf);  // Update lock
    DtSpinLockReleaseFromDpc(&pDf->m_PerItvSpinLock);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_CheckSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiTxPhy_CheckSdiRate(DtDfSdiTxPhy* pDf, Int SdiRate)
{
    // Check whether it is a valid SDI-rate
    if (   SdiRate!=DT_DRV_SDIRATE_SD && SdiRate!=DT_DRV_SDIRATE_HD 
        && SdiRate!=DT_DRV_SDIRATE_3G && SdiRate!=DT_DRV_SDIRATE_6G 
        && SdiRate!=DT_DRV_SDIRATE_12G)
        return DT_STATUS_INVALID_PARAMETER;

    // Assumption we can keep the comparison simple
    DT_ASSERT(DT_DRV_SDIRATE_SD<DT_DRV_SDIRATE_HD 
              && DT_DRV_SDIRATE_HD<DT_DRV_SDIRATE_3G
              && DT_DRV_SDIRATE_3G<DT_DRV_SDIRATE_6G
              && DT_DRV_SDIRATE_6G<DT_DRV_SDIRATE_12G);
    
    // Check whether the SDI-rate is supported
    if (SdiRate > pDf->m_PhyMaxSdiRate)
        return DT_STATUS_NOT_SUPPORTED;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_ConfigureRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Configures the block controllers for the new SDI-rate
//
DtStatus DtDfSdiTxPhy_ConfigureRate(DtDfSdiTxPhy*  pDf, Int  SdiRate, Bool  FracClk)
{
    // Sanity checks
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    DT_RETURN_ON_ERROR(DtDfSdiTxPhy_CheckSdiRate(pDf, SdiRate));

    // Call device family specific configure function
    switch (pDf->m_PhyDeviceFamily)
    {
    case DT_BC_SDITXPHY_FAMILY_A10:
    case DT_BC_SDITXPHY_FAMILY_C10:
        return DtDfSdiTxPhy_ConfigureRateC10A10(pDf, SdiRate, FracClk);
    case DT_BC_SDITXPHY_FAMILY_CV:
        return DtDfSdiTxPhy_ConfigureRateCV(pDf, SdiRate, FracClk);
    default:
        DT_ASSERT(FALSE);
        DtDbgOutDf(ERR, SDITXPHY, pDf, "Unsupported device family");
        return DT_STATUS_NOT_SUPPORTED;
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_ConfigureRateC10A10 -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Configures the block controllers for the new SDI-rate
// Aria 10 architecture specific implementation
//
DtStatus DtDfSdiTxPhy_ConfigureRateC10A10(DtDfSdiTxPhy*  pDf, Int  SdiRate, Bool  FracClk)
{
    DtStatus  Status = DT_STATUS_OK;
    Int PllId=0, Timeout=0;
    Bool  IsPllLocked=FALSE,  IsResetInProgress=FALSE;

    // Sanity checks
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Assumption there are 2 clocks
    DT_ASSERT(pDf->m_PhyNumClocks == 2);

    // Reset  PHY clock
    DT_RETURN_ON_ERROR(DtBcSDITXPHY_SetClockReset(pDf->m_pBcSdiTxPhy, TRUE));

    // Set SDI-rate
    DT_RETURN_ON_ERROR(DtBcSDITXPHY_SetSdiRate(pDf->m_pBcSdiTxPhy, SdiRate));

    // Set SDI fractional clock rate
    DT_RETURN_ON_ERROR(DtBcSDITXPHY_C10A10_SetSdiFractionalClock(pDf->m_pBcSdiTxPhy,
                                                                                FracClk));

    // Check PLL-lock
    Status = DtBcSDITXPHY_GetTxPllId(pDf->m_pBcSdiTxPhy, &PllId);
    DT_ASSERT(DT_SUCCESS(Status));
    Status = DtDfSi534X_IsPllLocked(pDf->m_pDfSi534X,  PllId, &IsPllLocked);
    DT_ASSERT(DT_SUCCESS(Status));
    DT_ASSERT(IsPllLocked);
    if (!DT_SUCCESS(Status) || !IsPllLocked)
        return DT_STATUS_NOT_INITIALISED;

    // Perform calibration initially and after fractional/non-fractional rate change
    if (FracClk!=pDf->m_FractionalClock || !DtDf_IsEnabled((DtDf*)pDf))
    { 
        Bool  CalDone;
        // Start calibration
        DT_RETURN_ON_ERROR(DtBcSDITXPHY_C10A10_StartCalibration(pDf->m_pBcSdiTxPhy));
    
        // Wait until calibration is completed
        Timeout = 200;
        CalDone = FALSE;
        while (Timeout>0 && !CalDone)
        {
            DtSleep(1);
            Timeout--;
            Status = DtBcSDITXPHY_C10A10_GetCalibrationDone(pDf->m_pBcSdiTxPhy, &CalDone);
            DT_ASSERT(DT_SUCCESS(Status));
        }
        if (!CalDone)
        { 
            DtDbgOutDf(ERR, SDITXPHY, pDf, "Calibration timeout");
            return DT_STATUS_TIMEOUT;
        }
    }

    // Calibration completed deassert clock-reset
    Status = DtBcSDITXPHY_SetClockReset(pDf->m_pBcSdiTxPhy, FALSE);
    DT_ASSERT(DT_SUCCESS(Status));

  
    // Wait until PHY reset in progress is deasserted
    Timeout = 100;
    IsResetInProgress = TRUE;
    Status = DtBcSDITXPHY_IsResetInProgress(pDf->m_pBcSdiTxPhy, &IsResetInProgress);
    while (Timeout>0 && IsResetInProgress)
    {
        DtSleep(1);
        Timeout--;
        Status = DtBcSDITXPHY_IsResetInProgress(pDf->m_pBcSdiTxPhy, &IsResetInProgress);
        DT_ASSERT(DT_SUCCESS(Status));
    }
    if (IsResetInProgress)
    {
        DtDbgOutDf(ERR, SDITXPHY, pDf, "Reset in progress timeout");
        return DT_STATUS_TIMEOUT;
    }
    return Status;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_ConfigureRateCV -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Configures the block controllers for the new SDI-rate
// Cyclone V architecture specific implementation
//
DtStatus DtDfSdiTxPhy_ConfigureRateCV(DtDfSdiTxPhy*  pDf, Int  SdiRate, Bool  FracClk)
{
    // Sanity checks
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Start clock reconfiguration
    DtSpinLockAcquire(&pDf->m_SpinLock);
    pDf->m_LockState = SDITXPHY_STATE_RECONFIG;
    DtSpinLockRelease(&pDf->m_SpinLock);

    // Assumption there is one clock
    DT_ASSERT(pDf->m_PhyNumClocks == 1);
    // Check required block controllers
    DT_ASSERT(pDf->m_pBcSdiTxPll!=NULL && pDf->m_pDfSi534X!=NULL 
                                                           && pDf->m_pDfSdiXCfgMgr!=NULL);
    // Reset  PHY clock
     DT_RETURN_ON_ERROR(DtBcSDITXPHY_SetClockReset(pDf->m_pBcSdiTxPhy, TRUE));
    
    // Set SDI-rate
    DT_RETURN_ON_ERROR(DtBcSDITXPHY_SetSdiRate(pDf->m_pBcSdiTxPhy, SdiRate));

    // Change clock frequency
    DT_RETURN_ON_ERROR(DtDfSi534X_SetConfig(pDf->m_pDfSi534X,
                                    FracClk ? DT_DF_SI534X_CFG_FREE_RUN_FRAC_CLOCK
                                            : DT_DF_SI534X_CFG_FREE_RUN_NONFRAC_CLOCK));
    // Reset the PLL-clock
    DT_RETURN_ON_ERROR(DtBcSDITXPLL_ResetClock(pDf->m_pBcSdiTxPll));

    // Change clock rate through reconfig controller; always 3G
    DT_RETURN_ON_ERROR(DtDfSdiXCfgMgr_SetXcvrMode(pDf->m_pDfSdiXCfgMgr,
                                        DtCore_PT_GetPortIndex(pDf->m_pPt),
                                        DT_SDIXCFG_CHANTYPE_TX_CHAN, DT_SDIXCFG_MODE_3G));

    // Change fractional mode through reconfig controller; always non-fractional
    DT_RETURN_ON_ERROR(DtDfSdiXCfgMgr_SetXcvrMode(pDf->m_pDfSdiXCfgMgr,
                                   DtCore_PT_GetPortIndex(pDf->m_pPt),
                                   DT_SDIXCFG_CHANTYPE_TX_PLL, DT_SDIXCFG_MODE_NON_FRAC));

    // Restart locking
    DtSpinLockAcquire(&pDf->m_SpinLock);
    pDf->m_LockState = SDITXPHY_STATE_WAIT_FOR_PLL_LOCK;
    DtSpinLockRelease(&pDf->m_SpinLock);

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_SdiLockStateUpdate -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtDfSdiTxPhy_SdiLockStateUpdate(DtDfSdiTxPhy*  pDf)
{
    // Call device family specific configure function
    switch (pDf->m_PhyDeviceFamily)
    {
    case DT_BC_SDITXPHY_FAMILY_A10:
    case DT_BC_SDITXPHY_FAMILY_C10:
        break;
    case DT_BC_SDITXPHY_FAMILY_CV:   DtDfSdiTxPhy_SdiLockStateUpdateCV(pDf);  break;
    default:
        DT_ASSERT(FALSE);
        DtDbgOutDf(ERR, SDITXPHY, pDf, "Unsupported device family");
        break;
    }
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_SdiLockStateUpdateCV -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Called from DPC
//
void DtDfSdiTxPhy_SdiLockStateUpdateCV(DtDfSdiTxPhy*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool  IsPllLocked=FALSE, IsResetInProgress=TRUE;

    DtSpinLockAcquireAtDpc(&pDf->m_SpinLock);

    switch (pDf->m_LockState)
    {
    case SDITXPHY_STATE_RECONFIG:
        // Reconfiguration busy; nothing to do
        break;
    default:
    case SDITXPHY_STATE_WAIT_FOR_PLL_LOCK:
        // Check PLL-lock
        DT_ASSERT(pDf->m_pBcSdiTxPll != NULL);
        Status = DtBcSDITXPLL_IsPllLocked(pDf->m_pBcSdiTxPll, &IsPllLocked);
        DT_ASSERT(DT_SUCCESS(Status));
        if (IsPllLocked)
        {
            // PLL locked, deassert clock-reset
            Status = DtBcSDITXPHY_SetClockReset(pDf->m_pBcSdiTxPhy, FALSE);
            DT_ASSERT(DT_SUCCESS(Status));
            // State = WAIT_FOR_PHY_READY
            DtDbgOutDf(MIN, SDITXPHY, pDf, "Entered: STATE_WAIT_FOR_PHY_READY");
            pDf->m_LockState = SDITXPHY_STATE_WAIT_FOR_PHY_READY;
        }
        break;

    case SDITXPHY_STATE_WAIT_FOR_PHY_READY:
        // Check PHY reset in progress
        Status = DtBcSDITXPHY_IsResetInProgress(pDf->m_pBcSdiTxPhy, &IsResetInProgress);
        DT_ASSERT(DT_SUCCESS(Status));
            
        if (!IsResetInProgress)
        {
            // Update the PHY operational mode
            Int  PhyOpMode = DT_BLOCK_OPMODE_IDLE;
            switch (pDf->m_OperationalMode)
            {
            case DT_FUNC_OPMODE_IDLE:     PhyOpMode = DT_BLOCK_OPMODE_IDLE; break;
            case DT_FUNC_OPMODE_STANDBY:  PhyOpMode = DT_BLOCK_OPMODE_STANDBY; break;
            case DT_FUNC_OPMODE_RUN:      PhyOpMode = DT_BLOCK_OPMODE_RUN; break;
            default: DT_ASSERT(FALSE);
            }
            // Do we need to start on start-of-frame (SDI-GenLock)
            if (pDf->m_CapGenLocked && pDf->m_TxMode==DT_SDITXPHY_TXMODE_SDI
                                                        && PhyOpMode==DT_BLOCK_OPMODE_RUN)
            {
                // Make sure PHY is in STANDBY
                Int  CurPhyOpStatus;
                Status = DtBcSDITXPHY_GetOperationalStatus(pDf->m_pBcSdiTxPhy, 
                                                                         &CurPhyOpStatus);
                if (DT_SUCCESS(Status) && CurPhyOpStatus!=DT_BLOCK_OPSTATUS_STANDBY)
                    Status = DtBcSDITXPHY_SetOperationalMode(pDf->m_pBcSdiTxPhy,
                                                                 DT_BLOCK_OPMODE_STANDBY);
                // Update the TxPhy GenLock status
                pDf->m_PhyIsGenLocked = pDf->m_GenLockIsLocked;

                // Arm for start-on-frame
                if (DT_SUCCESS(Status))
                    Status = DtBcSDITXPHY_ArmForSof(pDf->m_pBcSdiTxPhy);
            }
            if (DT_SUCCESS(Status))
                Status = DtBcSDITXPHY_SetOperationalMode(pDf->m_pBcSdiTxPhy, PhyOpMode);
            if (!DT_SUCCESS(Status))
                DtDbgOutDf(ERR, SDITXPHY, pDf, "ERROR: DtBcSDITXPHY_SetOperationalMode"
                                               " failed (Status=0x%08X)", Status);
            else
            {
                // State = STATE_TX_LOCKED
                DtDbgOutDf(MIN, SDITXPHY, pDf, "Entered: STATE_TX_LOCKED");
                pDf->m_LockState = SDITXPHY_STATE_TX_LOCKED;
            }
        }
        break;

    case SDITXPHY_STATE_TX_LOCKED:
        // Check PLL-lock
        DT_ASSERT(pDf->m_pBcSdiTxPll != NULL);
        Status = DtBcSDITXPLL_IsPllLocked(pDf->m_pBcSdiTxPll, &IsPllLocked);
        DT_ASSERT(DT_SUCCESS(Status));
        if (!IsPllLocked)
        {
            // PLL lost lock, start re-initialization
            // Set operational mode of SDITXPHY to IDLE
            Status = DtBcSDITXPHY_SetOperationalMode(pDf->m_pBcSdiTxPhy, 
                                                                    DT_BLOCK_OPMODE_IDLE);
            DT_ASSERT(DT_SUCCESS(Status));
            // Reset  PHY-clock
            Status = DtBcSDITXPHY_SetClockReset(pDf->m_pBcSdiTxPhy, TRUE);
            DT_ASSERT(DT_SUCCESS(Status));
            // Reset the PLL-clock
            if (pDf->m_pBcSdiTxPll != NULL)
            {
                Status = DtBcSDITXPLL_ResetClock(pDf->m_pBcSdiTxPll);
                DT_ASSERT(DT_SUCCESS(Status));
            }
            // State = STATE_WAIT_FOR_PLL_LOCK
            pDf->m_LockState = SDITXPHY_STATE_WAIT_FOR_PLL_LOCK;
            DtDbgOutDf(MIN, SDITXPHY, pDf, "Entered: STATE_WAIT_FOR_PLL_LOCK");
        }
        break;
    }
    DtSpinLockReleaseFromDpc(&pDf->m_SpinLock);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_OnGenLockChanged -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfSdiTxPhy_OnGenLockChanged(DtObject*  pObj, Bool Locked)
{
    DtDfSdiTxPhy*  pDf = (DtDfSdiTxPhy*)pObj; 

    // Sanity check
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Update the GenLock status
    pDf->m_GenLockIsLocked = Locked;

    // Update the TxPhy GenLock status
    if (!Locked)
        pDf->m_PhyIsGenLocked = Locked;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_VidStd2SdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int DtDfSdiTxPhy_VidStd2SdiRate(Int VidStd)
{
    if      (DtAvVidStdIsSdSdi(VidStd))   return DT_DRV_SDIRATE_SD;
    else if (DtAvVidStdIsHdSdi(VidStd))   return DT_DRV_SDIRATE_HD;
    else if (DtAvVidStdIs3gSdi(VidStd))   return DT_DRV_SDIRATE_3G;
    else if (DtAvVidStdIs6gSdi(VidStd))   return DT_DRV_SDIRATE_6G;
    else if (DtAvVidStdIs12gSdi(VidStd))  return DT_DRV_SDIRATE_12G;

    DT_ASSERT(FALSE);
    return DT_DRV_SDIRATE_UNKNOWN;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_ComputeSofDelay -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSdiTxPhy_ComputeSofDelay(DtDfSdiTxPhy* pDf, Int VidStd, Int* pSofDelay)
{
    DtStatus  Status = DT_STATUS_OK;
    DtAvFrameProps  VidProps;
    Int  SofDelayNs, SofDelayClks=0, SyncPointSymOffset=0;
    *pSofDelay = 0;

    // GenLocking generates the Start-of-Frame too early. The SofDelay has to compensate
    // for this.
    SofDelayNs = DT_DF_GENLOCKCTRL_START_OF_FRAME_OFFSET;

    // Add the PHY start-of-frame offset
    SofDelayNs += pDf->m_PhySofOffsetNs;

    // Get properties of video standard
    Status = DtAvGetFrameProps(VidStd, &VidProps);
    if (!DT_SUCCESS(Status))
        return DT_STATUS_INVALID_PARAMETER;

    // Compute sync-point symbol offset
    SyncPointSymOffset = VidProps.m_SyncPointPixelOff*2;
    if (DtAvVidStdIs6gSdi(VidStd) || DtAvVidStdIs12gSdi(VidStd))
        SyncPointSymOffset *= 4;

    // Compute delay needed for the syncpoint offset in nanoseconds
    SofDelayNs -= DtAvVidStdSymbOffset2TimeOffset(VidStd, SyncPointSymOffset);

    // Additional compensation for Level-B
    if (DtAvVidStdIs3glvlBSdi(VidStd))
    { 
        // TODOTD not sure how much compensation is needed for the LevelA->B conversion
        Int NumSymb = VidProps.m_EavNumS + VidProps.m_HancNumS
                                               + VidProps.m_SavNumS + VidProps.m_VancNumS;
        SofDelayNs -= DtAvVidStdSymbOffset2TimeOffset(VidStd, NumSymb);
        SofDelayNs -= DtAvVidStdSymbOffset2TimeOffset(VidStd, SyncPointSymOffset);
        // 20 pixel compensation necessary
        SofDelayNs += DtAvVidStdSymbOffset2TimeOffset(VidStd, 40);
    }

    // Compensate for the pipe-line delay in the TxPhy
    switch (DtDfSdiTxPhy_VidStd2SdiRate(VidStd))
    {
    case DT_DRV_SDIRATE_12G: SofDelayNs -= pDf->m_PhyDelayNs12g; break;
    case DT_DRV_SDIRATE_6G:  SofDelayNs -= pDf->m_PhyDelayNs6g; break;
    case DT_DRV_SDIRATE_3G:  SofDelayNs -= pDf->m_PhyDelayNs3g; break;
    case DT_DRV_SDIRATE_HD:  SofDelayNs -= pDf->m_PhyDelayNsHd; break;
    case DT_DRV_SDIRATE_SD:  SofDelayNs -= pDf->m_PhyDelayNsSd; break;
    default: DT_ASSERT(FALSE);
    }
    DT_ASSERT(SofDelayNs >= 0);

    // Convert to clock ticks
    SofDelayClks = (SofDelayNs*1485 + 5)/10000;
    if (DtAvVidStdUsesFractionalClock(VidStd))
        SofDelayClks = SofDelayClks*1000/1001;
    *pSofDelay = SofDelayClks;
    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfSdiTxPhy implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtDf member to DtDfSdiTxPhy
#define STUB_SDITXPHY   ((DtIoStubDfSdiTxPhy*)pStub)
#define STUB_DF  ((DtDfSdiTxPhy*)STUB_SDITXPHY->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus DtIoStubDfSdiTxPhy_OnCmd(const DtIoStub* pStub, 
                                            DtIoStubIoParams* pIoParams, Int * pOutSize);
static DtStatus  DtIoStubDfSdiTxPhy_OnCmdClearUnderflowFlag(const DtIoStubDfSdiTxPhy*);
static DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetGenLockStatus(const DtIoStubDfSdiTxPhy *,
                                               DtIoctlSdiTxPhyCmdGetGenLockStatusOutput*);
static DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetMaxSdiRate(const DtIoStubDfSdiTxPhy*, 
                                                  DtIoctlSdiTxPhyCmdGetMaxSdiRateOutput*);
static DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetOperationalMode(const DtIoStubDfSdiTxPhy*,
                                                      DtIoctlSdiTxPhyCmdGetOpModeOutput*);
static DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetSdiRate(const DtIoStubDfSdiTxPhy*, 
                                                     DtIoctlSdiTxPhyCmdGetSdiRateOutput*);
static DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetSdiStatus(const DtIoStubDfSdiTxPhy*, 
                                                   DtIoctlSdiTxPhyCmdGetSdiStatusOutput*);
static DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetStartOfFrameOffset(const DtIoStubDfSdiTxPhy*, 
                                          DtIoctlSdiTxPhyCmdGetStartOfFrameOffsetOutput*);
static DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetUnderflowFlag(const DtIoStubDfSdiTxPhy*,
                                               DtIoctlSdiTxPhyCmdGetUnderflowFlagOutput*);
static DtStatus  DtIoStubDfSdiTxPhy_OnCmdSetOperationalMode(const DtIoStubDfSdiTxPhy*,
                                                 const DtIoctlSdiTxPhyCmdSetOpModeInput*);
static DtStatus  DtIoStubDfSdiTxPhy_OnCmdSetOperationalModeTimed(
                                            const DtIoStubDfSdiTxPhy*,
                                            const DtIoctlSdiTxPhyCmdSetOpModeTimedInput*);
static DtStatus DtIoStubDfSdiTxPhy_OnCmdSetSdiRate(const DtIoStubDfSdiTxPhy*, 
                                                const DtIoctlSdiTxPhyCmdSetSdiRateInput*);
static DtStatus DtIoStubDfSdiTxPhy_OnCmdSetStartOfFrameOffset(const DtIoStubDfSdiTxPhy*, 
                                     const DtIoctlSdiTxPhyCmdSetStartOfFrameOffsetInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_SDITXPHY;

static const DtIoctlProperties  IOSTUB_DF_SDITXPHY_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubDf_OnExclAccessCmd,
        NULL, NULL),
    DT_IOCTL_PROPS_SDITXPHY_CMD(
        DtIoStubDfSdiTxPhy_OnCmd, 
        NULL, NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfSdiTxPhy - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubDfSdiTxPhy_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiTxPhy));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubDfSdiTxPhy*  DtIoStubDfSdiTxPhy_Open(DtDf*  pDf)
{
    DtIoStubDfSdiTxPhy*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfSdiTxPhy, pDf, NULL, 
                                                             DtIoStubDfSdiTxPhy_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_DF_SDITXPHY_IOCTLS);
    pStub = (DtIoStubDfSdiTxPhy*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfSdiTxPhy - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfSdiTxPhy_OnCmd(
    const DtIoStub*  pStub, 
    DtIoStubIoParams*  pIoParams, 
    Int* pOutSize)
{
    DtStatus Status= DT_STATUS_OK;
    const DtIoctlSdiTxPhyCmdInput*  pInData = NULL;
    DtIoctlSdiTxPhyCmdOutput*  pOutData = NULL;
    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiTxPhy));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_SDITXPHY_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, SDITXPHY, pStub, "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SdiTxPhyCmd;
    if (pIoParams->m_OutReqSize > 0)
    { 
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_SdiTxPhyCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_SDITXPHY_CMD_CLEAR_UNDERFLOW_FLAG:
        Status = DtIoStubDfSdiTxPhy_OnCmdClearUnderflowFlag(STUB_SDITXPHY);
        break;

    case DT_SDITXPHY_CMD_GET_GENLOCK_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSdiTxPhy_OnCmdGetGenLockStatus(STUB_SDITXPHY, 
                                                           &pOutData->m_GetGenLockStatus);
        break;
    case DT_SDITXPHY_CMD_GET_MAX_SDIRATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSdiTxPhy_OnCmdGetMaxSdiRate(STUB_SDITXPHY, 
                                                              &pOutData->m_GetMaxSdiRate);
        break;
    case DT_SDITXPHY_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSdiTxPhy_OnCmdGetOperationalMode(STUB_SDITXPHY, 
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_SDITXPHY_CMD_GET_SDIRATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSdiTxPhy_OnCmdGetSdiRate(STUB_SDITXPHY, 
                                                                 &pOutData->m_GetSdiRate);
        break;
    case DT_SDITXPHY_CMD_GET_SDI_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSdiTxPhy_OnCmdGetSdiStatus(STUB_SDITXPHY, 
                                                               &pOutData->m_GetSdiStatus);
        break;
    case DT_SDITXPHY_CMD_GET_START_OF_FRAME_OFFSET:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSdiTxPhy_OnCmdGetStartOfFrameOffset(STUB_SDITXPHY, 
                                                               &pOutData->m_GetSofOffset);
        break;
    case DT_SDITXPHY_CMD_GET_UNDERFLOW_FLAG:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSdiTxPhy_OnCmdGetUnderflowFlag(STUB_SDITXPHY, 
                                                                 &pOutData->m_GetUflFlag);
        break;

    case DT_SDITXPHY_CMD_SET_OPERATIONAL_MODE:
        Status = DtIoStubDfSdiTxPhy_OnCmdSetOperationalMode(STUB_SDITXPHY, 
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_SDITXPHY_CMD_SET_OPERATIONAL_MODE_TIMED:
        Status = DtIoStubDfSdiTxPhy_OnCmdSetOperationalModeTimed(STUB_SDITXPHY, 
                                                              &pInData->m_SetOpModeTimed);
        break;
    case DT_SDITXPHY_CMD_SET_SDIRATE:
        Status = DtIoStubDfSdiTxPhy_OnCmdSetSdiRate(STUB_SDITXPHY, 
                                                                  &pInData->m_SetSdiRate);
        break;
    case DT_SDITXPHY_CMD_SET_START_OF_FRAME_OFFSET:
        Status = DtIoStubDfSdiTxPhy_OnCmdSetStartOfFrameOffset(STUB_SDITXPHY, 
                                                                &pInData->m_SetSofOffset);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}


// -.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_OnCmdClearUnderflowFlag -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSdiTxPhy_OnCmdClearUnderflowFlag(
    const DtIoStubDfSdiTxPhy*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiTxPhy));
    return DtDfSdiTxPhy_ClearUnderflowFlag(STUB_DF);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_OnCmdGetGenLockStatus -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetGenLockStatus(
    const DtIoStubDfSdiTxPhy*  pStub, 
    DtIoctlSdiTxPhyCmdGetGenLockStatusOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiTxPhy));
    DT_ASSERT(pOutData != NULL);

    return DtDfSdiTxPhy_GetGenLockStatus(STUB_DF, &pOutData->m_GenLockStatus);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_OnCmdGetMaxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetMaxSdiRate(
    const DtIoStubDfSdiTxPhy*  pStub, 
    DtIoctlSdiTxPhyCmdGetMaxSdiRateOutput* pOutData)
{
    return DtDfSdiTxPhy_GetMaxSdiRate(STUB_DF, &pOutData->m_MaxSdiRate);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetOperationalMode(
    const DtIoStubDfSdiTxPhy*  pStub, 
    DtIoctlSdiTxPhyCmdGetOpModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiTxPhy));
    DT_ASSERT(pOutData != NULL);

    return DtDfSdiTxPhy_GetOperationalMode(STUB_DF, &pOutData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_OnCmdGetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetSdiRate(
    const DtIoStubDfSdiTxPhy*  pStub, 
    DtIoctlSdiTxPhyCmdGetSdiRateOutput* pOutData)
{
    return DT_STATUS_NOT_SUPPORTED; // Not supported anymore; video standard must be set
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_OnCmdGetSdiStatus -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetSdiStatus(
    const DtIoStubDfSdiTxPhy*  pStub, 
    DtIoctlSdiTxPhyCmdGetSdiStatusOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiTxPhy));
    DT_ASSERT(pOutData != NULL);

    return DtDfSdiTxPhy_GetSdiStatus(STUB_DF, &pOutData->m_TxLock);
}

// .-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_OnCmdGetStartOfFrameOffset -.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfSdiTxPhy_OnCmdGetStartOfFrameOffset(
    const DtIoStubDfSdiTxPhy* pStub,
    DtIoctlSdiTxPhyCmdGetStartOfFrameOffsetOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiTxPhy));
    DT_ASSERT(pOutData != NULL);
    return DtDfSdiTxPhy_GetStartOfFrameOffset(STUB_DF, &pOutData->m_StartOfFrameOffsetNs);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_OnCmdGetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetUnderflowFlag(
    const DtIoStubDfSdiTxPhy*  pStub, 
    DtIoctlSdiTxPhyCmdGetUnderflowFlagOutput* pOutData)
{
    Bool  UflFlag = FALSE;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiTxPhy));
    DT_ASSERT(pOutData != NULL);

    DT_RETURN_ON_ERROR(DtDfSdiTxPhy_GetUnderflowFlag(STUB_DF, &UflFlag));
    pOutData->m_UflFlag = (Int)UflFlag;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSdiTxPhy_OnCmdSetOperationalMode(
    const DtIoStubDfSdiTxPhy*  pStub, 
    const DtIoctlSdiTxPhyCmdSetOpModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiTxPhy));
    DT_ASSERT(pInData != NULL);

    return DtDfSdiTxPhy_SetOperationalMode(STUB_DF, pInData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_OnCmdSetOperationalModeTimed -.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfSdiTxPhy_OnCmdSetOperationalModeTimed(
    const DtIoStubDfSdiTxPhy*  pStub, 
    const DtIoctlSdiTxPhyCmdSetOpModeTimedInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiTxPhy));
    DT_ASSERT(pInData != NULL);
    return DtDfSdiTxPhy_SetOperationalModeTimed(STUB_DF, pInData->m_OpMode, 
                                                                    pInData->m_StartTime);
}
//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_OnCmdSetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSdiTxPhy_OnCmdSetSdiRate(
    const DtIoStubDfSdiTxPhy*  pStub, 
    const DtIoctlSdiTxPhyCmdSetSdiRateInput* pInData)
{
    return DT_STATUS_NOT_SUPPORTED; // Not supported anymore; video standard must be set
}

// .-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_OnCmdSetStartOfFrameOffset -.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfSdiTxPhy_OnCmdSetStartOfFrameOffset(
    const DtIoStubDfSdiTxPhy* pStub,
    const DtIoctlSdiTxPhyCmdSetStartOfFrameOffsetInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiTxPhy));
    DT_ASSERT(pInData != NULL);
    return DtDfSdiTxPhy_SetStartOfFrameOffset(STUB_DF, pInData->m_StartOfFrameOffsetNs);
}
