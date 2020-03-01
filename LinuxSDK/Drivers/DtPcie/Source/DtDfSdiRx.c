//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSdiRx.c *#*#*#*#*#*#*#*#* (C) 2017-2018 DekTec
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
#include "DtDfSdiRx.h"


// #define  DISABLE_AUTO_SDIRATE_DETECTION     1
// #define  CONFIGURE_SDIRATE_ONCE     1
// #define  CONFIGURE_SDIRATE_DISABLED     1
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        
#define SDIRXPHY_ROLE_NONE      NULL
#define SDIRXP_ROLE_NONE        NULL
#define SDIXCFGMGR_ROLE_NONE    NULL


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSdiRx implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiRx function
#define DF_SDIRX_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfSdiRx))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_SDIRX_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(SDIRX, pDf)

// Helper macro to cast a DtDf* to a DtDfSdiRx*
#define DF_SDIRX      ((DtDfSdiRx*)pDf)


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfSdiRx_StartConfigureRate(DtDfSdiRx* pDf, Int* pSdiRate, Bool Next);
static DtStatus  DtDfSdiRx_StartConfigureRateC10A10(DtDfSdiRx* pDf, Int SdiRate);
static DtStatus  DtDfSdiRx_StartConfigureRateCV(DtDfSdiRx* pDf, Int SdiRate);
static DtStatus  DtDfSdiRx_FinishConfigureRate(DtDfSdiRx* pDf, Int SdiRate);
static DtStatus  DtDfSdiRx_Init(DtDf*);
static DtStatus  DtDfSdiRx_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfSdiRx_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfSdiRx_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfSdiRx_LoadParameters(DtDf*);
static DtStatus  DtDfSdiRx_OpenChildren(DtDfSdiRx*);
static DtStatus  DtDfSdiRx_CheckSdiRate(DtDfSdiRx*, Int SdiRate);
static void  DtDfSdiRx_PeriodicIntervalHandler(DtObject*, DtTodTime  Time);
static void  DtDfSdiRx_SdiLockStateUpdate(DtDfSdiRx*,  DtTodTime  Time);
static Bool  DtDfSdiRx_UsesLockToData(Int Rate);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSdiRx - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfSdiRx_Close(DtDf*  pDf)
{
    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);

    //Unregister periodic interrupt handler
    DtCore_TOD_PeriodicItvUnregister(pDf->m_pCore, (DtObject*)pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfSdiRx*  DtDfSdiRx_Open(DtCore*  pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_SDIRX_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfSdiRx, Id, DT_FUNC_TYPE_SDIRX, pPt,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfSdiRx_Close;
    OpenParams.m_InitFunc = DtDfSdiRx_Init;
    OpenParams.m_OnCloseFileFunc = DtDfSdiRx_OnCloseFile;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfSdiRx_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfSdiRx_OnEnablePreChildren;
    OpenParams.m_LoadParsFunc = DtDfSdiRx_LoadParameters;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfSdiRx*)DtDf_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_GetMaxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiRx_GetMaxSdiRate(DtDfSdiRx* pDf, Int* pMaxSdiRate)
{
    // Sanity check
    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pMaxSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SDIRX_MUST_BE_ENABLED(pDf);

    // Return cached value
    *pMaxSdiRate = pDf->m_PhyMaxSdiRate;

    return DT_STATUS_OK;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSdiRx_GetOperationalMode(DtDfSdiRx* pDf, Int* pOpMode)
{
    // Sanity check
    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SDIRX_MUST_BE_ENABLED(pDf);

    // Return cached operational mode
    *pOpMode = pDf->m_OperationalMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_GetRxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiRx_GetRxMode(DtDfSdiRx* pDf, Int* pRxMode)
{
    // Sanity check
    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pRxMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SDIRX_MUST_BE_ENABLED(pDf);

    // Return cached RX-mode
    *pRxMode = pDf->m_RxMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_GetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSdiRx_GetSdiRate(DtDfSdiRx* pDf, Int* pSdiRate)
{
    // Sanity check
    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SDIRX_MUST_BE_ENABLED(pDf);

    // Return cached configured SDI-rate
    *pSdiRate = pDf->m_ConfigSdiRate;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_GetSdiStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiRx_GetSdiStatus(DtDfSdiRx* pDf, Int* pCarrierDetect, Int* pSdiLock,
                            Int* pLineLock, Int* pValid, Int* pSdiRate, Int* pNumSymsHanc, 
                            Int* pNumSymsVidVanc, Int*  pNumLinesF1, Int* pNumLinesF2,
                            Int* pIsLevelB, UInt32* pPayloadId, Int* pFramePeriod)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool CarrierDetected;

    // Sanity check
    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_SDIRX_MUST_BE_ENABLED(pDf);

    // Get carrier detect status
    Status = DtBcSDIRXPHY_IsCarrierDetect(pDf->m_pBcSdiRxPhy, &CarrierDetected);
    if (!DT_SUCCESS(Status))
    { 
        DtDbgOutDf(ERR, SDIRX, pDf, "DtBcSDIRXPHY_IsCarrierDetect failed");
        return Status;
    }
    *pCarrierDetect = (Int)CarrierDetected;

    // For ASI only carrier detect is supported
    if (pDf->m_RxMode == DT_SDIRX_RXMODE_ASI)
    { 
        *pSdiLock = *pLineLock = *pValid  =  *pIsLevelB =  (Int)FALSE;
        *pNumSymsHanc = *pNumSymsVidVanc = *pNumLinesF1 = *pNumLinesF2 = *pPayloadId 
                                                                      = *pFramePeriod = 0;
        *pSdiRate =DT_DRV_SDIRATE_UNKNOWN;
        return DT_STATUS_OK;
    }

    // Get SDI status
    Status = DtBcSDIRXP_GetSdiStatus(pDf->m_pBcSdiRxProt, pSdiLock, pLineLock, pValid,
                                     pSdiRate, pNumSymsHanc, pNumSymsVidVanc, pNumLinesF1,
                                     pNumLinesF2, pIsLevelB, pPayloadId, pFramePeriod);
    DtDbgOutDf(MIN, SDIRX, pDf, "SDILOCK:%d; LineLock:%d; Valid:%d; Rate:%d, NumH:%d; "
                            "NumV:%d; F1:%d; F2:%d; LvlB:%d; PID:0x%x; Fp:%d; Carrier:%d",
                                    *pSdiLock, *pLineLock, *pValid, *pSdiRate,
                                    *pNumSymsHanc, *pNumSymsVidVanc, *pNumLinesF1,
                                    *pNumLinesF2, *pIsLevelB, *pPayloadId, *pFramePeriod,
                                    *pCarrierDetect);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSdiRx_SetOperationalMode(DtDfSdiRx*  pDf, Int  OpMode)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (OpMode!=DT_FUNC_OPMODE_IDLE && OpMode!=DT_FUNC_OPMODE_RUN)
    {
        DtDbgOutDf(ERR, SDIRX, pDf, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    DF_SDIRX_MUST_BE_ENABLED(pDf);

    // No change?
    if (pDf->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Make sure only the SDIRXP 
    DtSpinLockAcquire(&pDf->m_SpinLock);

    if (OpMode==DT_FUNC_OPMODE_IDLE && pDf->m_RxMode==DT_SDIRX_RXMODE_SDI)
    {
        // To IDLE
        // Set operational mode of SDIRXP to STANDBY
        Status = DtBcSDIRXP_SetOperationalMode(pDf->m_pBcSdiRxProt,
                                                                 DT_BLOCK_OPMODE_STANDBY);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, SDIRX, pDf, "ERROR: DtBcSDIRXP_SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }
    else if (OpMode==DT_FUNC_OPMODE_RUN && pDf->m_RxMode==DT_SDIRX_RXMODE_SDI)
    {   
        // To RUN
        // Set operational mode of SDIRXP to RUN
        Status = DtBcSDIRXP_SetOperationalMode(pDf->m_pBcSdiRxProt, DT_BLOCK_OPMODE_RUN);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, SDIRX, pDf, "ERROR: DtBcSDIRXP_SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }
    // Save new operational mode
    if (DT_SUCCESS(Status))
        pDf->m_OperationalMode = OpMode;

    DtSpinLockRelease(&pDf->m_SpinLock);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_SetRxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiRx_SetRxMode(DtDfSdiRx* pDf, Int RxMode)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_SDIRX_MUST_BE_ENABLED(pDf);

    // Check parameters
    if (RxMode!=DT_SDIRX_RXMODE_ASI && RxMode!=DT_SDIRX_RXMODE_SDI)
    {
        DtDbgOutDf(ERR, SDIRX, pDf, "Invalid RX-mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be in IDLE
    if (pDf->m_OperationalMode != DT_FUNC_OPMODE_IDLE)
    {
        DtDbgOutDf(ERR, SDIRX, pDf, "Function not in IDLE");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // No change?
    if (pDf->m_RxMode == RxMode)
        return DT_STATUS_OK;

    // SDI is not supported in a ASI only configuration
    if ( RxMode==DT_SDIRX_RXMODE_SDI && pDf->m_pBcSdiRxProt==NULL)
        return DT_STATUS_NOT_SUPPORTED;

    DtSpinLockAcquire(&pDf->m_SpinLock);

    if (RxMode==DT_SDIRX_RXMODE_SDI)
    {
        // SDI receive  mode
        // Enable SDIRXP
        Status = DF_SDIRX->m_pBcSdiRxProt->m_EnableFunc(
                                                      (DtBc*)(pDf->m_pBcSdiRxProt), TRUE);
        // Set operational mode of SDIRXP to STANDBY
        if (DT_SUCCESS(Status))
            Status = DtBcSDIRXP_SetOperationalMode(pDf->m_pBcSdiRxProt,
                                                                 DT_BLOCK_OPMODE_STANDBY);
    }
    else
    {
        // ASI receive mode
        // Disable SDIRXP (if present)
        if (pDf->m_pBcSdiRxProt != NULL)
            Status = DF_SDIRX->m_pBcSdiRxProt->m_EnableFunc(
                                                     (DtBc*)(pDf->m_pBcSdiRxProt), FALSE);
    }

    if (DT_SUCCESS(Status))
    {
        Int  TimeDiffMs = 0;
        DtTodTime  Time;
        // Restart locking 
        pDf->m_LockState = SDIRX_STATE_INIT_XCVR;
        pDf->m_CurrentSdiRate = pDf->m_ConfigSdiRate;
        pDf->m_RxMode = RxMode;
        // Logging
        DtCore_TOD_GetTime(pDf->m_pCore, &Time);
        TimeDiffMs = (Int)DtDivideS64(DtCore_TOD_TimeDiff(Time, pDf->m_StateTime),
                                                                               1000*1000);
        pDf->m_StateTime = Time;
        DtDbgOutDf(MIN, SDIRX, pDf, "Entered: STATE_INIT_XCVR; Rate: %d RxMode: %d"
                              " Duration: %d", pDf->m_CurrentSdiRate, RxMode, TimeDiffMs);
    }
    DtSpinLockRelease(&pDf->m_SpinLock);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_SetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSdiRx_SetSdiRate(DtDfSdiRx* pDf, Int SdiRate)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity checks
    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_SDIRX_MUST_BE_ENABLED(pDf);

    // Check parameters
    DT_RETURN_ON_ERROR(DtDfSdiRx_CheckSdiRate(pDf, SdiRate));

    // Must be in IDLE
    if (pDf->m_OperationalMode != DT_FUNC_OPMODE_IDLE)
    {
        DtDbgOutDf(ERR, SDIRX, pDf, "Function not in IDLE");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // No change?
    if (pDf->m_ConfigSdiRate == SdiRate)
        return DT_STATUS_OK;
    // Cannot change SDI-rate if RX-mode is ASI
    if (pDf->m_RxMode == DT_SDIRX_RXMODE_ASI)
        return DT_STATUS_NOT_SUPPORTED;

    DtSpinLockAcquire(&pDf->m_SpinLock);

    // If not locked restart locking with the new configured SDI-rate
    if (pDf->m_LockState != SDIRX_STATE_SDI_LOCKED)
    { 
        Int  TimeDiffMs = 0;
        DtTodTime  Time;
        pDf->m_LockState = SDIRX_STATE_INIT_XCVR;
        pDf->m_CurrentSdiRate = SdiRate;

        // Logging
        DtCore_TOD_GetTime(pDf->m_pCore, &Time);
        TimeDiffMs = (Int)DtDivideS64(DtCore_TOD_TimeDiff(Time, pDf->m_StateTime),
                                                                               1000*1000);
        pDf->m_StateTime = Time;
        DtDbgOutDf(MIN, SDIRX, pDf, "Entered: STATE_INIT_XCVR; SetSdiRate: %d"
                              " Duration: %d", pDf->m_CurrentSdiRate, TimeDiffMs);
    }
    pDf->m_ConfigSdiRate = SdiRate;

    DtSpinLockRelease(&pDf->m_SpinLock);

    return Status;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSdiRx - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_StartConfigureRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Starts the configuration of the block controllers for the new SDI-rate
//
DtStatus DtDfSdiRx_StartConfigureRate(DtDfSdiRx* pDf, Int* pSdiRate, Bool Next)
{
    // Sanity checks
    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (pSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

#ifdef  DISABLE_AUTO_SDIRATE_DETECTION
    if (pDf->m_RxMode == DT_SDIRX_RXMODE_SDI)
    { 
        // Force to use configured SDI-rate
        *pSdiRate = pDf->m_ConfigSdiRate;
        Next = FALSE;
    }
#endif

    DT_RETURN_ON_ERROR(DtDfSdiRx_CheckSdiRate(pDf, *pSdiRate));

    // Select next SDI-rate?
    if (Next)
    {
        if (*pSdiRate == pDf->m_PhyMaxSdiRate)
            *pSdiRate = DT_DRV_SDIRATE_SD;
        else
            switch(*pSdiRate)
            {
                case DT_DRV_SDIRATE_SD:  *pSdiRate = DT_DRV_SDIRATE_HD; break;
                case DT_DRV_SDIRATE_HD:  *pSdiRate = DT_DRV_SDIRATE_3G; break;
                case DT_DRV_SDIRATE_3G:  *pSdiRate = DT_DRV_SDIRATE_6G; break;
                case DT_DRV_SDIRATE_6G:  *pSdiRate = DT_DRV_SDIRATE_12G; break;
                default:
                case DT_DRV_SDIRATE_12G: *pSdiRate = DT_DRV_SDIRATE_SD; break;
            }
    }

    // Call device family specific configure function
    switch (pDf->m_PhyDeviceFamily)
    {
    case DT_BC_SDIRXPHY_FAMILY_A10:
    case DT_BC_SDIRXPHY_FAMILY_C10:
        return DtDfSdiRx_StartConfigureRateC10A10(pDf, *pSdiRate);
    case DT_BC_SDIRXPHY_FAMILY_CV:
        return DtDfSdiRx_StartConfigureRateCV(pDf, *pSdiRate);
    default:
        DT_ASSERT(FALSE);
        DtDbgOutDf(ERR, SDIRX, pDf, "Unsupported device family");
        return DT_STATUS_NOT_SUPPORTED;
    }
    return DT_STATUS_OK;
}
// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_StartConfigureRateC10A10 -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiRx_StartConfigureRateC10A10(DtDfSdiRx* pDf, Int SdiRate)
{
    DtStatus  Status=DT_STATUS_OK;
    // Change the operational mode such that PHY and transceiver can be configured
    if (DT_SUCCESS(Status))
        Status =  DtBcSDIRXPHY_SetOperationalMode(pDf->m_pBcSdiRxPhy,
                                                                 DT_BLOCK_OPMODE_STANDBY);
    // Reset clock
    if (DT_SUCCESS(Status))
        Status = DtBcSDIRXPHY_SetClockReset(pDf->m_pBcSdiRxPhy, TRUE);

    // Select new rate
    if (DT_SUCCESS(Status))
        Status = DtBcSDIRXPHY_StartSetSdiRate(pDf->m_pBcSdiRxPhy, SdiRate);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_ConfigureRateCV -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiRx_StartConfigureRateCV(DtDfSdiRx* pDf, Int SdiRate)
{
    DtStatus  Status=DT_STATUS_OK;
    Int  BcReconfig=DT_SDIXCFG_MODE_3G;

    // Convert parameter for the block controllers
    switch (SdiRate)
    {
    case DT_DRV_SDIRATE_SD:
        BcReconfig = DT_SDIXCFG_MODE_3G;
        break;
    case DT_DRV_SDIRATE_HD:
        BcReconfig = DT_SDIXCFG_MODE_1G5;
        break;
    case DT_DRV_SDIRATE_3G:
        BcReconfig = DT_SDIXCFG_MODE_3G;
        break;
    default:
        DT_ASSERT(FALSE);
        break;
    }   

    // Change the operational mode such that PHY and transceiver can be configured
    if (DT_SUCCESS(Status))
        Status =  DtBcSDIRXPHY_SetOperationalMode(pDf->m_pBcSdiRxPhy,
                                                                 DT_BLOCK_OPMODE_STANDBY);
    // Reset clock
    if (DT_SUCCESS(Status))
        Status = DtBcSDIRXPHY_SetClockReset(pDf->m_pBcSdiRxPhy, TRUE);

    // Change reconfig controller for new rate
    if (DT_SUCCESS(Status))
        Status = DtDfSdiXCfgMgr_SetXcvrMode(pDf->m_pDfSdiXCfgMgr, 
                                                 DtCore_PT_GetPortIndex(pDf->m_pPt),
                                                 DT_SDIXCFG_CHANTYPE_RX_CHAN, BcReconfig);
    // Select new rate
    if (DT_SUCCESS(Status))
        Status = DtBcSDIRXPHY_StartSetSdiRate(pDf->m_pBcSdiRxPhy, SdiRate);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_FinishConfigureRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Finishes the configuration of the block controllers for the new SDI-rate
//
DtStatus DtDfSdiRx_FinishConfigureRate(DtDfSdiRx* pDf, Int SdiRate)
{
    DtStatus  Status=DT_STATUS_OK;
    Bool DownsamplerEnable = FALSE;

    // Sanity checks
    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(DtDfSdiRx_CheckSdiRate(pDf, SdiRate) == DT_STATUS_OK);

    // Convert parameter for the block controllers
    switch (SdiRate)
    {
    case DT_DRV_SDIRATE_SD:
        DownsamplerEnable = TRUE;
        break;
    case DT_DRV_SDIRATE_HD:
        DownsamplerEnable = FALSE;
        break;
    case DT_DRV_SDIRATE_3G:
        DownsamplerEnable = FALSE;
        break;
    case DT_DRV_SDIRATE_6G:
        DownsamplerEnable = FALSE;
        break;
    case DT_DRV_SDIRATE_12G:
        DownsamplerEnable = FALSE;
        break;
    }   
    // Finish SetSdiRate
    Status = DtBcSDIRXPHY_FinishSetSdiRate(pDf->m_pBcSdiRxPhy);

    // Restart locking
    // Lock to reference
    if (DT_SUCCESS(Status))
        Status = DtBcSDIRXPHY_SetLockMode(pDf->m_pBcSdiRxPhy,
                                                     DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_REF);
    // Set downsampling
    if (DT_SUCCESS(Status))
        Status = DtBcSDIRXPHY_SetDownsamplerEnable(pDf->m_pBcSdiRxPhy, DownsamplerEnable);
    // SDIRXP is only configured when in SDI-mode
    if (pDf->m_RxMode == DT_SDIRX_RXMODE_SDI)
    { 
        DT_ASSERT(pDf->m_pBcSdiRxProt != NULL);
        // Change the operational mode of SDIRXP in idle such that it can be configured
        if (DT_SUCCESS(Status))
            Status = DtBcSDIRXP_SetOperationalMode(pDf->m_pBcSdiRxProt,
                                                                    DT_BLOCK_OPMODE_IDLE);
        // Set SDI-rate in SDIRXP
        if (DT_SUCCESS(Status))
            Status = DtBcSDIRXP_SetSdiRate(pDf->m_pBcSdiRxProt, SdiRate);

        // Set the SDIRXP in standby
        if (DT_SUCCESS(Status))
            Status = DtBcSDIRXP_SetOperationalMode(pDf->m_pBcSdiRxProt,
                                                                 DT_BLOCK_OPMODE_STANDBY);
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSdiRx_Init(DtDf*  pDfBase)
{
    DtDfSdiRx*  pDf = (DtDfSdiRx*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
    DtOnPeriodicIntervalRegData  RegData;

    // Sanity checks
    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);

    // Initialize the spin lock
    DtSpinLockInit(&pDf->m_SpinLock);

    // Set defaults
    pDf->m_LockState = SDIRX_STATE_INIT_XCVR;
    pDf->m_StateTime.m_Seconds = 0;
    pDf->m_StateTime.m_Nanoseconds = 0;
    pDf->m_OperationalMode = DT_FUNC_OPMODE_IDLE;
    pDf->m_CurrentSdiRate = DT_DRV_SDIRATE_HD;
    pDf->m_ConfigSdiRate = DT_DRV_SDIRATE_HD;
    pDf->m_LastLockedSdiRate = DT_DRV_SDIRATE_HD;
    // Set default PHY configuration
    pDf->m_PhyMaxSdiRate = DT_DRV_SDIRATE_HD;
    pDf->m_PhyDeviceFamily = DT_BC_SDIRXPHY_FAMILY_UNKNOWN;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfSdiRx_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SDIRX, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    // Determine default for RX-mode
    if (pDf->m_pBcSdiRxProt == NULL)
        pDf->m_RxMode = DT_SDIRX_RXMODE_ASI;
    else
        pDf->m_RxMode = DT_SDIRX_RXMODE_SDI;
    
    // Init periodic interval handler enable flag and spinlock
    DtSpinLockInit(&pDf->m_PerItvSpinLock);
    pDf->m_PerItvEnable = FALSE;

    // Register periodic interval handler
    RegData.m_OnPeriodicFunc = DtDfSdiRx_PeriodicIntervalHandler;
    RegData.m_pObject = (DtObject*)pDf;
    Status = DtCore_TOD_PeriodicItvRegister(pDf->m_pCore, &RegData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SDIRX, pDf, "ERROR: failed to register period itv handler");
        return Status;
    }

    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtDfSdiRx_OnCloseFile(DtDf* pDf, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    // Sanity checks
    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);

    // Check if the owner closed the file handle
    Status = DtDf_ExclAccessCheck(pDf, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutDf(AVG, SDIRX, pDf, "Go to IDLE");

        // Go to IDLE
        Status = DtDfSdiRx_SetOperationalMode(DF_SDIRX, DT_FUNC_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, SDIRX, pDf, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtDf_OnCloseFile(pDf, pFile);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSdiRx_OnEnablePostChildren(DtDf*  pDfBase, Bool  Enable)
{
    Int  MaxSdiRate1=0, MaxSdiRate2=0;
    DtDfSdiRx*  pDf = (DtDfSdiRx*)pDfBase;
    // Sanity checks
    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);
    
    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutDf(AVG, SDIRX, pDf, "Function enable from disable -> enable");

        // Get RXPHY configuration
        // Phy's device family
        DT_RETURN_ON_ERROR(DtBcSDIRXPHY_GetDeviceFamily(pDf->m_pBcSdiRxPhy, 
                                                                &pDf->m_PhyDeviceFamily));
        // Maximum SDI-rate is minimum of SdiTxP and SdiTxPhy
        DT_RETURN_ON_ERROR(DtBcSDIRXPHY_GetMaxSdiRate(pDf->m_pBcSdiRxPhy, &MaxSdiRate1)); 
        DT_RETURN_ON_ERROR(DtBcSDIRXP_GetMaxSdiRate(pDf->m_pBcSdiRxProt, &MaxSdiRate2)); 
        pDf->m_PhyMaxSdiRate = (MaxSdiRate1 < MaxSdiRate2) ? MaxSdiRate1 : MaxSdiRate2;

        // Operational mode must be IDLE
        DT_ASSERT(pDf->m_OperationalMode == DT_FUNC_OPMODE_IDLE);
        // Disable SDIRXP if RX-Mode is ASI
        if (pDf->m_RxMode == DT_SDIRX_RXMODE_ASI)
            DT_RETURN_ON_ERROR(pDf->m_pBcSdiRxProt->m_EnableFunc(
                                                    (DtBc*)(pDf->m_pBcSdiRxProt), FALSE));
        //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Start in IDLE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
        // Set operational mode of SDIRXP to IDLE if RX-Mode is SDI
        if (pDf->m_RxMode == DT_SDIRX_RXMODE_SDI)
            DT_RETURN_ON_ERROR(DtBcSDIRXP_SetOperationalMode(pDf->m_pBcSdiRxProt,
                                                                   DT_BLOCK_OPMODE_IDLE));
        // Set operational mode of SDIRXPHY to IDLE
        DT_RETURN_ON_ERROR(DtBcSDIRXPHY_SetOperationalMode(pDf->m_pBcSdiRxPhy,
                                                                   DT_BLOCK_OPMODE_IDLE));

        // Configure SDI-rate
        pDf->m_LockState = SDIRX_STATE_INIT_XCVR;
        DtCore_TOD_GetTime(pDf->m_pCore, &pDf->m_StateTime);
        pDf->m_CurrentSdiRate = pDf->m_ConfigSdiRate;

        // Enable periodic interval handler
        DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
        pDf->m_PerItvEnable = TRUE;
        DtSpinLockRelease(&pDf->m_PerItvSpinLock);
    }
    return DT_STATUS_OK;
}
//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_OnEnablePreChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSdiRx_OnEnablePreChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtDfSdiRx*  pDf = (DtDfSdiRx*)pDfBase;
    // Sanity checks
    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);
    if (!Enable)
    {
        // ENABLE -> DISABLE
        DtDbgOutDf(AVG, SDIRX, pDf, "Function enable from enable -> disable");

        // Disable periodic interval handler
        DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
        pDf->m_PerItvEnable = FALSE;
        DtSpinLockRelease(&pDf->m_PerItvSpinLock);

        // Return to IDLE
        DT_RETURN_ON_ERROR(DtDfSdiRx_SetOperationalMode(pDf, DT_FUNC_OPMODE_IDLE));
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSdiRx_LoadParameters(DtDf*  pDf)
{
    return DT_STATUS_OK;
}
 
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSdiRx_OpenChildren(DtDfSdiRx*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

     // List of children supported by the the SDIRX function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_SDIRXP, DT_BC_SDIRXP_NAME,
                        SDIRXP_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcSdiRxProt), TRUE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_SDIRXPHY, DT_BC_SDIRXPHY_NAME,
                       SDIRXPHY_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcSdiRxPhy), TRUE},
    };

    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    // Find the shared device level SdiXcvrReconfig manager function (optional)
    pDf->m_pDfSdiXCfgMgr = (DtDfSdiXCfgMgr*)DtCore_DF_Find(pDf->m_pCore, NULL,
                                           DT_FUNC_TYPE_SDIXCFGMGR, SDIXCFGMGR_ROLE_NONE);

    // Check mandatory children have been loaded (i.e. shortcut is valid)
    DT_ASSERT(pDf->m_pBcSdiRxProt != NULL);
    DT_ASSERT(pDf->m_pBcSdiRxPhy != NULL);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_CheckSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiRx_CheckSdiRate(DtDfSdiRx* pDf, Int SdiRate)
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

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_PeriodicIntervalHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfSdiRx_PeriodicIntervalHandler(DtObject* pObj, DtTodTime  Time)
{
    DtDfSdiRx*  pDf = (DtDfSdiRx*)pObj;
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfSdiRx));

    DtSpinLockAcquireAtDpc(&pDf->m_PerItvSpinLock);
    // Only when enabled
    if (pDf->m_PerItvEnable)
        DtDfSdiRx_SdiLockStateUpdate(pDf, Time);  // Update lock
    DtSpinLockReleaseFromDpc(&pDf->m_PerItvSpinLock);
}



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_SdiLockStateUpdate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Called from DPC
//
void DtDfSdiRx_SdiLockStateUpdate(DtDfSdiRx*  pDf, DtTodTime  Time)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool  Done=FALSE, LockedToRef=FALSE,LineLock = FALSE, CarrierDetected = FALSE;
    DT_UNUSED Int  TimeDiffMs = (Int)DtDivideS64(
                                DtCore_TOD_TimeDiff(Time, pDf->m_StateTime), (1000*1000));
#ifdef CONFIGURE_SDIRATE_DISABLED
    return;
#endif

    DtSpinLockAcquireAtDpc(&pDf->m_SpinLock);

    switch (pDf->m_LockState)
    {
    default:
    case SDIRX_STATE_INIT_XCVR:
        if (pDf->m_RxMode == DT_SDIRX_RXMODE_SDI)
        {
             // Configure current SDI-rate
            Status = DtDfSdiRx_StartConfigureRate(pDf, &pDf->m_CurrentSdiRate, FALSE);
        }
        else 
        {
            // Configure ASI rate
            Int AsiRate = DT_DRV_SDIRATE_SD;
            Status = DtDfSdiRx_StartConfigureRate(pDf, &AsiRate, FALSE);
        }
        if (!DT_SUCCESS(Status))
        {
            // Configuration failed; retry later again
            DtDbgOutDf(ERR, SDIRX, pDf, 
                    "ERROR: DtDfSdiRx_StartConfigureRate failed (Status=0x%08X)", Status);
            break;
        }
        // Rate configuration started
        // State = WAIT_CONFIGURE_RATE_DONE_1
        pDf->m_LockState = SDIRX_STATE_WAIT_CONFIGURE_RATE_DONE_1;
        pDf->m_StateTime = Time;
        DtDbgOutDf(MIN, SDIRX, pDf, "Entered: WAIT_CONFIGURE_RATE_DONE"
                                                             " Duration: %d", TimeDiffMs);
        // Fall through and check done

    case SDIRX_STATE_WAIT_CONFIGURE_RATE_DONE_1:
        Status = DtBcSDIRXPHY_GetSetSdiRateDone(pDf->m_pBcSdiRxPhy, &Done);
        DT_ASSERT(DT_SUCCESS(Status));
        if (DT_SUCCESS(Status) && Done)
        {
            if (pDf->m_RxMode == DT_SDIRX_RXMODE_SDI)
            {
               // Configuration completed
                Status = DtDfSdiRx_FinishConfigureRate(pDf, pDf->m_CurrentSdiRate);
                DT_ASSERT(DT_SUCCESS(Status));
                // State = WAIT_SDI_LOCKED_TO_REF1
                pDf->m_LockState = SDIRX_STATE_WAIT_SDI_LOCKED_TO_REF1;
                pDf->m_StateTime = Time;
                DtDbgOutDf(MIN, SDIRX, pDf, "Entered: WAIT_SDI_LOCKED_TO_REF1; Rate: %d"
                                      " Duration: %d", pDf->m_CurrentSdiRate, TimeDiffMs);
            }
            else
            {
                // Configuration completed
                Status = DtDfSdiRx_FinishConfigureRate(pDf, DT_DRV_SDIRATE_SD);
                DT_ASSERT(DT_SUCCESS(Status));
                // State = WAIT_ASI_LOCKED_TO_REF
                pDf->m_LockState = SDIRX_STATE_WAIT_ASI_LOCKED_TO_REF;
                pDf->m_StateTime = Time;
                DtDbgOutDf(MIN, SDIRX, pDf, "Entered: WAIT_ASI_LOCKED_TO_REF; Rate: %d"
                                          " Duration: %d", DT_DRV_SDIRATE_SD, TimeDiffMs);
            }
        }
        break;

        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Fast SDI-rate search -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    case SDIRX_STATE_WAIT_SDI_LOCKED_TO_REF1:
        // Is PHY.LockedToRef?
        Status = DtBcSDIRXPHY_IsLockedToRef(pDf->m_pBcSdiRxPhy, &LockedToRef);
        DT_ASSERT(DT_SUCCESS(Status));
        if (DT_SUCCESS(Status) && LockedToRef)
        {
            // Current rate requires lock to data?
            if (DtDfSdiRx_UsesLockToData(pDf->m_CurrentSdiRate))
            {
                // PHY.LockMode = LOCK_TO_DATA
                Status = DtBcSDIRXPHY_SetLockMode(pDf->m_pBcSdiRxPhy,
                                                    DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_DATA);
                DT_ASSERT(DT_SUCCESS(Status));
            }
            // PHY.RxClkReset = '0'
            Status = DtBcSDIRXPHY_SetClockReset(pDf->m_pBcSdiRxPhy, FALSE);
            DT_ASSERT(DT_SUCCESS(Status));


            // PHY.OperationalMode = RUN
            Status = DtBcSDIRXPHY_SetOperationalMode(pDf->m_pBcSdiRxPhy,
                                                                     DT_BLOCK_OPMODE_RUN);
            DT_ASSERT(DT_SUCCESS(Status));

            // State = CHECK_SDI_LOCK1
            pDf->m_LockState = SDIRX_STATE_CHECK_SDI_LOCK1;
            pDf->m_StateTime = Time;
            DtDbgOutDf(MIN, SDIRX, pDf, "Entered: CHECK_SDI_LOCK1; Rate: %d"
                                      " Duration: %d", pDf->m_CurrentSdiRate, TimeDiffMs);
        }
        break;

    case SDIRX_STATE_CHECK_SDI_LOCK1:
        // Check SDI-line-lock
        Status = DtBcSDIRXP_GetSdiLineLock(pDf->m_pBcSdiRxProt, &LineLock);
        DT_ASSERT(DT_SUCCESS(Status));
        if (DT_SUCCESS(Status) && LineLock)
        {
            // In running mode?
            if (pDf->m_OperationalMode == DT_FUNC_OPMODE_RUN)
            {
                // SDIRXP.OperationalMode = RUN
                Status = DtBcSDIRXP_SetOperationalMode(pDf->m_pBcSdiRxProt,
                                                                     DT_BLOCK_OPMODE_RUN);
                if (!DT_SUCCESS(Status))
                    DtDbgOutDf(ERR, SDIRX, pDf, 
                                             "ERROR: DtBcSDIRXP_SetOperationalMode failed"
                                             " (Status=0x%08X)", Status);
            }
            if (DT_SUCCESS(Status))
            {
                pDf->m_LastLockedSdiRate = pDf->m_CurrentSdiRate;
                // State = LOCKED
                pDf->m_LockState = SDIRX_STATE_SDI_LOCKED;
                pDf->m_StateTime = Time;
                DtDbgOutDf(MIN, SDIRX, pDf, "Entered: SDI_LOCKED; Rate: %d"
                                                       " Duration: %d           (Locked)",
                                                       pDf->m_CurrentSdiRate, TimeDiffMs);
            }
        }
        else
        {
#ifdef CONFIGURE_SDIRATE_ONCE
             break;
#endif
            // Check carrier detect
            Status = DtBcSDIRXPHY_IsCarrierDetect(pDf->m_pBcSdiRxPhy, &CarrierDetected);
            if (!DT_SUCCESS(Status) || !CarrierDetected)
            {
                // No carrier detected
                // State = DELAY
                pDf->m_DelayCount = 20;
                pDf->m_LockState = SDIRX_STATE_SDI_DELAY;
                pDf->m_StateTime = Time;
                DtDbgOutDf(MIN, SDIRX, pDf, "Entered: SDI_DELAY; Rate: %d"
                                      " Duration: %d", pDf->m_CurrentSdiRate, TimeDiffMs);
            }
            else
            {
                // Carrier detected but no line lock; try next SDI-rate
                Status = DtDfSdiRx_StartConfigureRate(pDf, &pDf->m_CurrentSdiRate, TRUE);
                if (DT_SUCCESS(Status))
                {
                    // State = WAIT_CONFIGURE_RATE_DONE
                    pDf->m_LockState = SDIRX_STATE_WAIT_CONFIGURE_RATE_DONE_1;
                    pDf->m_StateTime = Time;
                    DtDbgOutDf(MIN, SDIRX, pDf, "Entered: WAIT_CONFIGURE_RATE_DONE;"
                         " Rate: %d Duration: %d", pDf->m_CurrentSdiRate, TimeDiffMs);

                }
                else
                    DtDbgOutDf(ERR, SDIRX, pDf, "ERROR: DtDfSdiRx_StartConfigureRate"
                                                       " failed (Status=0x%08X)", Status);
            }
        }
        break;

    case SDIRX_STATE_SDI_LOCKED:
    {
        Int SdiLock, Valid, SdiRate, LineLockInt, NumSymsHanc, NumSymsVidVanc, NumLinesF1,
                                                        NumLinesF2, IsLevelB, FramePeriod;
        UInt32 PayloadId;
        // Get SDI status
        Status = DtBcSDIRXP_GetSdiStatus(pDf->m_pBcSdiRxProt, &SdiLock, &LineLockInt,
                                                &Valid, &SdiRate, &NumSymsHanc, 
                                                &NumSymsVidVanc, &NumLinesF1, &NumLinesF2,
                                                &IsLevelB, &PayloadId, &FramePeriod);
        DT_ASSERT(DT_SUCCESS(Status));
        if (!DT_SUCCESS(Status) || LineLockInt==0)
        {
            // No line-lock, search for rate
            // State = CHECK_SDI_LOCK1
            pDf->m_LockState = SDIRX_STATE_CHECK_SDI_LOCK1;
            pDf->m_StateTime = Time;
            DtDbgOutDf(MIN, SDIRX, pDf, "Entered: CHECK_SDI_LOCK1; Rate: %d"
                                                       " Duration: %d        (Lock Lost)",
                                                       pDf->m_CurrentSdiRate, TimeDiffMs);
        }
        else if (Valid == 1)
        {
            // Valid SDI detected
            // State = SDIRX_STATE_SDI_VALID
            pDf->m_LockState = SDIRX_STATE_SDI_VALID;
            pDf->m_StateTime = Time;
            DtDbgOutDf(MIN, SDIRX, pDf, "Entered: SDI_VALID; Rate: %d"
                                                       " Duration: %d        (SDI Valid)",
                                                       pDf->m_CurrentSdiRate, TimeDiffMs);
            DtDbgOutDf(MIN, SDIRX, pDf, "SDILOCK:%d; LineLock:%d; Valid:%d; NumH:%d; "
                                        " NumV:%d;F1:%d; F2:%d; LvlB:%d; PID:0x%x; Fp:%d",
                                        SdiLock, LineLockInt, Valid,
                                        NumSymsHanc, NumSymsVidVanc, NumLinesF1,
                                        NumLinesF2, IsLevelB, PayloadId, FramePeriod);
        }
    }
    break;

    case SDIRX_STATE_SDI_VALID:
        // Check linelock
        Status = DtBcSDIRXP_GetSdiLineLock(pDf->m_pBcSdiRxProt, &LineLock);
        DT_ASSERT(DT_SUCCESS(Status));

        if (!DT_SUCCESS(Status) || !LineLock)
        {
            // No line-lock, search for rate
            // State = CHECK_SDI_LOCK1
            pDf->m_LockState = SDIRX_STATE_CHECK_SDI_LOCK1;
            pDf->m_StateTime = Time;
            DtDbgOutDf(MIN, SDIRX, pDf, "Entered: CHECK_SDI_LOCK1; Rate: %d"
                                                       " Duration: %d        (Lock Lost)",
                                                       pDf->m_CurrentSdiRate, TimeDiffMs);
        }
        break;

        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Slow SDI-rate search -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    case SDIRX_STATE_SDI_DELAY:
        // Check carrier detect
        Status = DtBcSDIRXPHY_IsCarrierDetect(pDf->m_pBcSdiRxPhy, &CarrierDetected);
        if (DT_SUCCESS(Status) && CarrierDetected)
        {
            // Carrier detected
            // Configure last locked SDI-rate
            pDf->m_CurrentSdiRate = pDf->m_LastLockedSdiRate;
            Status = DtDfSdiRx_StartConfigureRate(pDf, &pDf->m_CurrentSdiRate, FALSE);
            if (DT_SUCCESS(Status))
            {
                // State = WAIT_CONFIGURE_RATE_DONE
                pDf->m_LockState = SDIRX_STATE_WAIT_CONFIGURE_RATE_DONE_1;
                pDf->m_StateTime = Time;
                DtDbgOutDf(MIN, SDIRX, pDf, "Entered: WAIT_CONFIGURE_RATE_DONE; Rate: %d"
                                      " Duration: %d", pDf->m_CurrentSdiRate, TimeDiffMs);
            }
            else
                DtDbgOutDf(ERR, SDIRX, pDf, "ERROR: DtDfSdiRx_StartConfigureRate"
                                                       " failed (Status=0x%08X)", Status);
        }
        else if (pDf->m_DelayCount <= 0)
        {
            // No carrier detected
            // Configure next SDI-rate
            Status = DtDfSdiRx_StartConfigureRate(pDf, &pDf->m_CurrentSdiRate, TRUE);
            if (DT_SUCCESS(Status))
            {
                // State = WAIT_CONFIGURE_RATE_DONE_2
                pDf->m_LockState = SDIRX_STATE_WAIT_CONFIGURE_RATE_DONE_2;
                pDf->m_StateTime = Time;
                DtDbgOutDf(MIN, SDIRX, pDf, "Entered: WAIT_CONFIGURE_RATE_DONE_2;"
                             " Rate: %d Duration: %d", pDf->m_CurrentSdiRate, TimeDiffMs);
            }
            else
                DtDbgOutDf(ERR, SDIRX, pDf, "ERROR: DtDfSdiRx_StartConfigureRate"
                                                       " failed (Status=0x%08X)", Status);
        }
        else
            pDf->m_DelayCount--;
        break;

    case SDIRX_STATE_WAIT_CONFIGURE_RATE_DONE_2:
        Status = DtBcSDIRXPHY_GetSetSdiRateDone(pDf->m_pBcSdiRxPhy, &Done);
        DT_ASSERT(DT_SUCCESS(Status));
        if (DT_SUCCESS(Status) && Done)
        {
            // Configuration completed
            Status = DtDfSdiRx_FinishConfigureRate(pDf, pDf->m_CurrentSdiRate);
            DT_ASSERT(DT_SUCCESS(Status));
            DT_ASSERT(pDf->m_RxMode == DT_SDIRX_RXMODE_SDI);

            // State = WAIT_SDI_LOCKED_TO_REF2
            pDf->m_LockState = SDIRX_STATE_WAIT_SDI_LOCKED_TO_REF2;
            pDf->m_StateTime = Time;
            DtDbgOutDf(MIN, SDIRX, pDf, "Entered: WAIT_SDI_LOCKED_TO_REF2; Rate: %d"
                       " Duration: %d", pDf->m_CurrentSdiRate, TimeDiffMs);
        }
        break;

   case SDIRX_STATE_WAIT_SDI_LOCKED_TO_REF2:
        // Is PHY.LockedToRef?
        Status = DtBcSDIRXPHY_IsLockedToRef(pDf->m_pBcSdiRxPhy, &LockedToRef);
        DT_ASSERT(DT_SUCCESS(Status));
        if (DT_SUCCESS(Status) && LockedToRef)
        {
            // Current rate requires lock to data?
            if ( DtDfSdiRx_UsesLockToData(pDf->m_CurrentSdiRate))
            {
                // PHY.LockMode = LOCK_TO_DATA
                Status = DtBcSDIRXPHY_SetLockMode(pDf->m_pBcSdiRxPhy,
                                                    DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_DATA);
                DT_ASSERT(DT_SUCCESS(Status));
            }
            // PHY.RxClkReset = '0'
            Status = DtBcSDIRXPHY_SetClockReset(pDf->m_pBcSdiRxPhy, FALSE);
            DT_ASSERT(DT_SUCCESS(Status));

           // PHY.OperationalMode = RUN
            Status = DtBcSDIRXPHY_SetOperationalMode(pDf->m_pBcSdiRxPhy,
                                                                     DT_BLOCK_OPMODE_RUN);
            DT_ASSERT(DT_SUCCESS(Status));

            // State = CHECK_SDI_LOCK2
            pDf->m_LockState = SDIRX_STATE_CHECK_SDI_LOCK2;
            pDf->m_StateTime = Time;
            DtDbgOutDf(MIN, SDIRX, pDf, "Entered: CHECK_SDI_LOCK2; Rate: %d"
                                      " Duration: %d", pDf->m_CurrentSdiRate, TimeDiffMs);
        }
        break;

    case SDIRX_STATE_CHECK_SDI_LOCK2:
        // Check line-lock
        Status = DtBcSDIRXP_GetSdiLineLock(pDf->m_pBcSdiRxProt, &LineLock);
        DT_ASSERT(DT_SUCCESS(Status));
        if (DT_SUCCESS(Status) && LineLock)
        {
            // In running mode?
            if (pDf->m_OperationalMode == DT_FUNC_OPMODE_RUN)
            {
                // SDIRXP.OperationalMode = RUN
                Status = DtBcSDIRXP_SetOperationalMode(pDf->m_pBcSdiRxProt,
                                                                     DT_BLOCK_OPMODE_RUN);
                if (!DT_SUCCESS(Status))
                    DtDbgOutDf(ERR, SDIRX, pDf, 
                                             "ERROR: DtBcSDIRXP_SetOperationalMode failed"
                                             " (Status=0x%08X)", Status);
            }
            if (DT_SUCCESS(Status))
            {
                pDf->m_LastLockedSdiRate = pDf->m_CurrentSdiRate;
                // State = LOCKED
                pDf->m_LockState = SDIRX_STATE_SDI_LOCKED;
                pDf->m_StateTime = Time;
                DtDbgOutDf(MIN, SDIRX, pDf, "Entered: SDI_LOCKED; Rate: %d"
                                                       " Duration: %d           (Locked)",
                                                       pDf->m_CurrentSdiRate, TimeDiffMs);
            }
        }
        else
        {
            // State = DELAY
            pDf->m_DelayCount = 20;
            pDf->m_LockState = SDIRX_STATE_SDI_DELAY;
            pDf->m_StateTime = Time;
            DtDbgOutDf(MIN, SDIRX, pDf, "Entered: SDI_DELAY; Rate: %d"
                                      " Duration: %d", pDf->m_CurrentSdiRate, TimeDiffMs);
        }
        break;

        //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ASI detection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    case SDIRX_STATE_WAIT_ASI_LOCKED_TO_REF:
        // Is PHY.LockedToRef?
        Status = DtBcSDIRXPHY_IsLockedToRef(pDf->m_pBcSdiRxPhy, &LockedToRef);
        DT_ASSERT(DT_SUCCESS(Status));
        if (DT_SUCCESS(Status) && LockedToRef)
        {
            // PHY.RxClkReset = '0'
            Status = DtBcSDIRXPHY_SetClockReset(pDf->m_pBcSdiRxPhy, FALSE);
            DT_ASSERT(DT_SUCCESS(Status));

            // PHY.OperationalMode = RUN
            Status = DtBcSDIRXPHY_SetOperationalMode(pDf->m_pBcSdiRxPhy,
                                                                     DT_BLOCK_OPMODE_RUN);
            DT_ASSERT(DT_SUCCESS(Status));

            // State = WAIT_ASI_CARRIER_DETECT
            pDf->m_LockState = SDIRX_STATE_WAIT_ASI_CARRIER_DETECT;
            pDf->m_StateTime = Time;
            DtDbgOutDf(MIN, SDIRX, pDf, "Entered: WAIT_ASI_CARRIER_DETECT; Rate: %d"
                                      " Duration: %d", pDf->m_CurrentSdiRate, TimeDiffMs);
        }
        break;

    case SDIRX_STATE_WAIT_ASI_CARRIER_DETECT:
        // Check carrier detection
        Status = DtBcSDIRXPHY_IsCarrierDetect(pDf->m_pBcSdiRxPhy, &CarrierDetected);
        DT_ASSERT(DT_SUCCESS(Status));
        if (DT_SUCCESS(Status) && CarrierDetected)
        {

            // State = ASI_CARRIER_DETECTED
            pDf->m_LockState = SDIRX_STATE_ASI_CARRIER_DETECTED;
            pDf->m_StateTime = Time;
            DtDbgOutDf(MIN, SDIRX, pDf, "Entered: ASI_CARRIER_DETECTED; Rate: %d"
                                      " Duration: %d", pDf->m_CurrentSdiRate, TimeDiffMs);
        }
        break;

    case SDIRX_STATE_ASI_CARRIER_DETECTED:
        // Check carrier detection
        Status = DtBcSDIRXPHY_IsCarrierDetect(pDf->m_pBcSdiRxPhy, &CarrierDetected);
        DT_ASSERT(DT_SUCCESS(Status));
        if (!DT_SUCCESS(Status) || !CarrierDetected)
        {
            // No carrier, wait for carrier
            // State = WAIT_ASI_CARRIER_DETECT
            pDf->m_LockState = SDIRX_STATE_WAIT_ASI_CARRIER_DETECT;
            DtDbgOutDf(MIN, SDIRX, pDf, "Entered: WAIT_ASI_CARRIER_DETECT; Rate: %d"
                                      " Duration: %d", pDf->m_CurrentSdiRate, TimeDiffMs);
        }
        break;
    }
    DtSpinLockReleaseFromDpc(&pDf->m_SpinLock);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx_UsesLockToData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool DtDfSdiRx_UsesLockToData(Int Rate)
{
    // Only ASI/SD uses lock-to-ref others use lock-to-data
    return Rate!=DT_DRV_SDIRATE_SD;                                        
}
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfAsiRx implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtDf member to DtDfSdiRx
#define STUB_SDIRX   ((DtIoStubDfSdiRx*)pStub)
#define STUB_DF  ((DtDfSdiRx*)STUB_SDIRX->m_pDf)


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtIoStubDfSdiRx_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int* pOutSize);
static DtStatus  DtIoStubDfSdiRx_OnCmdGetMaxSdiRate(const DtIoStubDfSdiRx*,
                                                     DtIoctlSdiRxCmdGetMaxSdiRateOutput*);
static DtStatus  DtIoStubDfSdiRx_OnCmdGetOperationalMode(const DtIoStubDfSdiRx*,
                                                         DtIoctlSdiRxCmdGetOpModeOutput*);
static DtStatus  DtIoStubDfSdiRx_OnCmdGetRxMode(const DtIoStubDfSdiRx*, 
                                                         DtIoctlSdiRxCmdGetRxModeOutput*);
static DtStatus  DtIoStubDfSdiRx_OnCmdGetSdiRate(const DtIoStubDfSdiRx*, 
                                                        DtIoctlSdiRxCmdGetSdiRateOutput*);
static DtStatus  DtIoStubDfSdiRx_OnCmdGetSdiStatus(const DtIoStubDfSdiRx*, 
                                                      DtIoctlSdiRxCmdGetSdiStatusOutput*);
static DtStatus  DtIoStubDfSdiRx_OnCmdGetSdiStatus2(const DtIoStubDfSdiRx*, 
                                                     DtIoctlSdiRxCmdGetSdiStatusOutput2*);
static DtStatus  DtIoStubDfSdiRx_OnCmdSetOperationalMode(const DtIoStubDfSdiRx*,
                                                    const DtIoctlSdiRxCmdSetOpModeInput*);
static DtStatus DtIoStubDfSdiRx_OnCmdSetRxMode(const DtIoStubDfSdiRx*, 
                                                    const DtIoctlSdiRxCmdSetRxModeInput*);
static DtStatus DtIoStubDfSdiRx_OnCmdSetSdiRate(const DtIoStubDfSdiRx*, 
                                                   const DtIoctlSdiRxCmdSetSdiRateInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_SDIRX;

static const DtIoctlProperties  IOSTUB_DF_SDIRX_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubDf_OnExclAccessCmd,
        NULL, NULL),
    DT_IOCTL_PROPS_SDIRX_CMD(
        DtIoStubDfSdiRx_OnCmd, 
        NULL, NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfAsiRx - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiRx_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubDfSdiRx_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiRx));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiRx_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubDfSdiRx*  DtIoStubDfSdiRx_Open(DtDf*  pDf)
{
    DtIoStubDfSdiRx*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    DF_SDIRX_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfSdiRx, pDf, NULL, 
                                                              DtIoStubDfSdiRx_Close,
                                                              NULL,  // Use default IOCTL
                                                              IOSTUB_DF_SDIRX_IOCTLS);
    pStub = (DtIoStubDfSdiRx*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfSdiRx - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiRx_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfSdiRx_OnCmd(
    const DtIoStub*  pStub, 
    DtIoStubIoParams*  pIoParams, 
    Int* pOutSize)
{
    DtStatus Status= DT_STATUS_OK;
    const DtIoctlSdiRxCmdInput*  pInData = NULL;
    DtIoctlSdiRxCmdOutput*  pOutData = NULL;
    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiRx));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_SDIRX_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, SDIRX, pStub, "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SdiRxCmd;
    if (pIoParams->m_OutReqSize > 0)
    { 
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_SdiRxCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_SDIRX_CMD_GET_MAX_SDIRATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSdiRx_OnCmdGetMaxSdiRate(STUB_SDIRX, 
                                                              &pOutData->m_GetMaxSdiRate);
        break;
    case DT_SDIRX_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSdiRx_OnCmdGetOperationalMode(STUB_SDIRX, 
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_SDIRX_CMD_GET_RXMODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSdiRx_OnCmdGetRxMode(STUB_SDIRX, &pOutData->m_GetRxMode);
        break;
    case DT_SDIRX_CMD_GET_SDIRATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSdiRx_OnCmdGetSdiRate(STUB_SDIRX, &pOutData->m_GetSdiRate);
        break;
    case DT_SDIRX_CMD_GET_SDI_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSdiRx_OnCmdGetSdiStatus(STUB_SDIRX, &pOutData->m_GetSdiStatus);
        break;
    case DT_SDIRX_CMD_GET_SDI_STATUS2:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSdiRx_OnCmdGetSdiStatus2(STUB_SDIRX, 
                                                              &pOutData->m_GetSdiStatus2);
        break;
    case DT_SDIRX_CMD_SET_OPERATIONAL_MODE:
        Status = DtIoStubDfSdiRx_OnCmdSetOperationalMode(STUB_SDIRX, 
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_SDIRX_CMD_SET_RXMODE:
        Status = DtIoStubDfSdiRx_OnCmdSetRxMode(STUB_SDIRX, &pInData->m_SetRxMode);
        break;
    case DT_SDIRX_CMD_SET_SDIRATE:
        Status = DtIoStubDfSdiRx_OnCmdSetSdiRate(STUB_SDIRX, &pInData->m_SetSdiRate);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiRx_OnCmdGetMaxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSdiRx_OnCmdGetMaxSdiRate(
    const DtIoStubDfSdiRx*  pStub, 
    DtIoctlSdiRxCmdGetMaxSdiRateOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiRx));
    DT_ASSERT(pOutData != NULL);

    return DtDfSdiRx_GetMaxSdiRate(STUB_DF, &pOutData->m_MaxSdiRate);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiRx_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSdiRx_OnCmdGetOperationalMode(
    const DtIoStubDfSdiRx*  pStub, 
    DtIoctlSdiRxCmdGetOpModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiRx));
    DT_ASSERT(pOutData != NULL);

    return DtDfSdiRx_GetOperationalMode(STUB_DF, &pOutData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiRx_OnCmdGetRxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfSdiRx_OnCmdGetRxMode(
    const DtIoStubDfSdiRx* pStub,
    DtIoctlSdiRxCmdGetRxModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiRx));
    DT_ASSERT(pOutData != NULL);

    return DtDfSdiRx_GetRxMode(STUB_DF, &pOutData->m_RxMode);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiRx_OnCmdGetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSdiRx_OnCmdGetSdiRate(
    const DtIoStubDfSdiRx*  pStub, 
    DtIoctlSdiRxCmdGetSdiRateOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiRx));
    DT_ASSERT(pOutData != NULL);

    return DtDfSdiRx_GetSdiRate(STUB_DF, &pOutData->m_SdiRate);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiRx_OnCmdGetSdiStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfSdiRx_OnCmdGetSdiStatus(
    const DtIoStubDfSdiRx*  pStub, 
    DtIoctlSdiRxCmdGetSdiStatusOutput* pOutData)
{
    Int SdiRate = 0;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiRx));
    DT_ASSERT(pOutData != NULL);

    return DtDfSdiRx_GetSdiStatus(STUB_DF, &pOutData->m_CarrierDetect, 
                          &pOutData->m_SdiLock, &pOutData->m_LineLock, &pOutData->m_Valid,
                          &SdiRate, &pOutData->m_NumSymsHanc, &pOutData->m_NumSymsVidVanc,
                          &pOutData->m_NumLinesF1, &pOutData->m_NumLinesF2, 
                          &pOutData->m_IsLevelB, &pOutData->m_PayloadId,
                          &pOutData->m_FramePeriod);
}
// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiRx_OnCmdGetSdiStatus2 -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSdiRx_OnCmdGetSdiStatus2(
    const DtIoStubDfSdiRx*  pStub, 
    DtIoctlSdiRxCmdGetSdiStatusOutput2* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiRx));
    DT_ASSERT(pOutData != NULL);
    return DtDfSdiRx_GetSdiStatus(STUB_DF, &pOutData->m_CarrierDetect, 
              &pOutData->m_SdiLock, &pOutData->m_LineLock, &pOutData->m_Valid,
              &pOutData->m_SdiRate, &pOutData->m_NumSymsHanc, &pOutData->m_NumSymsVidVanc,
              &pOutData->m_NumLinesF1, &pOutData->m_NumLinesF2, 
              &pOutData->m_IsLevelB, &pOutData->m_PayloadId,
              &pOutData->m_FramePeriod);
}
//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiRx_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSdiRx_OnCmdSetOperationalMode(
    const DtIoStubDfSdiRx*  pStub, 
    const DtIoctlSdiRxCmdSetOpModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiRx));
    DT_ASSERT(pInData != NULL);

    return DtDfSdiRx_SetOperationalMode(STUB_DF, pInData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiRx_OnCmdSetRxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfSdiRx_OnCmdSetRxMode(
    const DtIoStubDfSdiRx*  pStub,
    const DtIoctlSdiRxCmdSetRxModeInput*  pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiRx));
    DT_ASSERT(pInData != NULL);

    return DtDfSdiRx_SetRxMode(STUB_DF, pInData->m_RxMode);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiRx_OnCmdSetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSdiRx_OnCmdSetSdiRate(
    const DtIoStubDfSdiRx*  pStub, 
    const DtIoctlSdiRxCmdSetSdiRateInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiRx));
    DT_ASSERT(pInData != NULL);

    return DtDfSdiRx_SetSdiRate(STUB_DF, pInData->m_SdiRate);
}
