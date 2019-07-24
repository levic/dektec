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
static DtStatus  DtDfSdiTxPhy_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfSdiTxPhy_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfSdiTxPhy_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfSdiTxPhy_OpenChildren(DtDfSdiTxPhy*);
static void  DtDfSdiTxPhy_PeriodicIntervalHandler(DtObject*, DtTodTime  Time);
static DtStatus  DtDfSdiTxPhy_ConfigureRate(DtDfSdiTxPhy*, Int SdiRate, Bool Fractional);
static void  DtDfSdiTxPhy_SdiLockStateUpdate(DtDfSdiTxPhy*  pDf);


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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_GetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiTxPhy_GetSdiRate(DtDfSdiTxPhy* pDf, Int* pSdiRate, Bool* pFractional)
{
    // Sanity check
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);
    
    // Cannot get SDI-rate if TX-mode is ASI
    if (pDf->m_TxMode == DT_SDITXPHY_TXMODE_ASI)
        return DT_STATUS_NOT_SUPPORTED;

    // Return cached configured SDI-rate
    *pSdiRate = pDf->m_SdiRate;
    *pFractional = pDf->m_FractionalClock;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_GetSdiStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSdiTxPhy_GetSdiStatus(DtDfSdiTxPhy*  pDf, Int*  pIsTxLocked)
{
     DtStatus  Status=DT_STATUS_OK;
     Bool  IsPllLocked = FALSE;

    // Sanity check
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pIsTxLocked == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);

    // Get locked status
    DtSpinLockAcquire(&pDf->m_SpinLock);
    if (pDf->m_pBcSdiTxPll != NULL)
        Status = DtBcSDITXPLL_IsPllLocked(pDf->m_pBcSdiTxPll, &IsPllLocked);
    else
        IsPllLocked = TRUE; // There is no PLL in this channel; assume always lock
    *pIsTxLocked = (Int)(IsPllLocked && (pDf->m_LockState==SDITXPHY_STATE_TX_LOCKED));
    DtSpinLockRelease(&pDf->m_SpinLock);

    return Status;
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


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSdiTxPhy_SetOperationalMode(DtDfSdiTxPhy*  pDf, Int  OpMode)
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
    // Must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);

    // No change?
    if (pDf->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Lock the LockState
    DtSpinLockAcquire(&pDf->m_SpinLock);

    // When going to standby/run, we need to wait for TX-lock
    if ((OpMode==DT_FUNC_OPMODE_STANDBY || OpMode==DT_FUNC_OPMODE_RUN)
                                            && pDf->m_LockState!=SDITXPHY_STATE_TX_LOCKED)
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
        Status = DtBcSDITXPHY_SetOperationalMode(pDf->m_pBcSdiTxPhy, PhyOpMode);

        // Save new operational mode
        if (DT_SUCCESS(Status))
            pDf->m_OperationalMode = OpMode;
    }

    DtSpinLockRelease(&pDf->m_SpinLock);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_SetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiTxPhy_SetSdiRate(DtDfSdiTxPhy* pDf, Int SdiRate, Bool Fractional)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity checks
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Operational state must be enabled
    DF_SDITXPHY_MUST_BE_ENABLED(pDf);

    // Check parameters
    if (SdiRate!=DT_DRV_SDIRATE_SD && SdiRate!=DT_DRV_SDIRATE_HD
                                                            && SdiRate!=DT_DRV_SDIRATE_3G)
    {
        DtDbgOutDf(ERR, SDITXPHY, pDf, "Invalid SDI-rate");
        return DT_STATUS_INVALID_PARAMETER;
    }

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
    if (pDf->m_SdiRate==SdiRate && pDf->m_FractionalClock==Fractional)
        return DT_STATUS_OK;

    // Start clock reconfiguration
    DtSpinLockAcquire(&pDf->m_SpinLock);
    pDf->m_LockState = SDITXPHY_STATE_RECONFIG;
    DtSpinLockRelease(&pDf->m_SpinLock);

    // Configure new rate
    pDf->m_SdiRate = SdiRate;
    pDf->m_FractionalClock = Fractional;
    Status = DtDfSdiTxPhy_ConfigureRate(pDf, pDf->m_SdiRate, pDf->m_FractionalClock);

    // Restart locking
    DtSpinLockAcquire(&pDf->m_SpinLock);
    pDf->m_LockState = SDITXPHY_STATE_WAIT_FOR_PLL_LOCK;
    DtSpinLockRelease(&pDf->m_SpinLock);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_SetTxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiTxPhy_SetTxMode(DtDfSdiTxPhy* pDf, Int TxMode)
{
    DtStatus  Status = DT_STATUS_OK;

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

    // Start reconfiguration
    DtSpinLockAcquire(&pDf->m_SpinLock);
    pDf->m_LockState = SDITXPHY_STATE_RECONFIG;
    DtSpinLockRelease(&pDf->m_SpinLock);

    // Configure new TxMode
    pDf->m_TxMode = TxMode;
    if (pDf->m_TxMode == DT_SDITXPHY_TXMODE_SDI)
        Status = DtDfSdiTxPhy_ConfigureRate(pDf, pDf->m_SdiRate, pDf->m_FractionalClock);
    else
        // ASI uses SD-SDI-rate
        Status = DtDfSdiTxPhy_ConfigureRate(pDf, DT_DRV_SDIRATE_SD, FALSE);

    // Restart locking
    DtSpinLockAcquire(&pDf->m_SpinLock);
    pDf->m_LockState = SDITXPHY_STATE_WAIT_FOR_PLL_LOCK;
    DtSpinLockRelease(&pDf->m_SpinLock);

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

    // Sanity checks
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Initialize the spin lock
    DtSpinLockInit(&pDf->m_SpinLock);

    // Set defaults
    pDf->m_LockState = SDITXPHY_STATE_RECONFIG;
    pDf->m_OperationalMode = DT_FUNC_OPMODE_IDLE;
    pDf->m_TxMode = DT_SDITXPHY_TXMODE_SDI;
    pDf->m_SdiRate = DT_DRV_SDIRATE_HD;
    pDf->m_FractionalClock = FALSE;

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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSdiTxPhy_OnEnablePostChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtDfSdiTxPhy* pDf = (DtDfSdiTxPhy*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
     
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

        // Start clock reconfiguration
        DtSpinLockAcquire(&pDf->m_SpinLock);
        pDf->m_LockState = SDITXPHY_STATE_RECONFIG;
        DtSpinLockRelease(&pDf->m_SpinLock);

        // Configure defaults
        pDf->m_TxMode = DT_SDITXPHY_TXMODE_SDI;
        pDf->m_SdiRate = DT_DRV_SDIRATE_HD;
        pDf->m_FractionalClock = FALSE;
        Status = DtDfSdiTxPhy_ConfigureRate(pDf, pDf->m_SdiRate,  pDf->m_FractionalClock);

        // Start relocking
        DtSpinLockAcquire(&pDf->m_SpinLock);
        pDf->m_LockState = SDITXPHY_STATE_WAIT_FOR_PLL_LOCK;
        DtSpinLockRelease(&pDf->m_SpinLock);

        // Enable periodic interval handler
        DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
        pDf->m_PerItvEnable = TRUE;
        DtSpinLockRelease(&pDf->m_PerItvSpinLock);
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

    // Find the shared device level SdiXcvrReconfig manager function which is needed
    // for the SDITXPHY
    pDf->m_pDfSdiXCfgMgr = (DtDfSdiXCfgMgr*)DtCore_DF_Find(pDf->m_pCore, NULL,
                                           DT_FUNC_TYPE_SDIXCFGMGR, SDIXCFGMGR_ROLE_NONE);
    if (pDf->m_pDfSdiXCfgMgr == NULL)
    {
        DtDbgOutDf(ERR, SDITXPHY, pDf, "ERROR : SDIXCFGMGR DF not found");
        return DT_STATUS_CONFIG_ERROR;
    }

    // Find device level clock generator (optional)
    pDf->m_pDfSi534X  = (DtDfSi534X*)DtCore_DF_Find(pDf->m_pCore, NULL,
                                                               DT_FUNC_TYPE_SI534X, NULL);

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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_ConfigureRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Configures the block controllers for the new SDI-rate
//
DtStatus DtDfSdiTxPhy_ConfigureRate(DtDfSdiTxPhy*  pDf, Int  SdiRate, Bool  FracClk)
{
    Int  SrcFactor = 1;
    Int  NumClocks;

    // Sanity checks
    DF_SDITXPHY_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (SdiRate!=DT_DRV_SDIRATE_SD && SdiRate!=DT_DRV_SDIRATE_HD
                                                            && SdiRate!=DT_DRV_SDIRATE_3G)
    {
        DtDbgOutDf(ERR, SDITXPHY, pDf, "Invalid SDI-rate");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Determine number of clocks available
    DT_RETURN_ON_ERROR(DtBcSDITXPHY_GetNumClocks(pDf->m_pBcSdiTxPhy, &NumClocks));
    DT_ASSERT(NumClocks==1 || NumClocks==2);

    // Reset  PHY clock
     DT_RETURN_ON_ERROR(DtBcSDITXPHY_SetClockReset(pDf->m_pBcSdiTxPhy, TRUE));
    
     // Set Upsampling factor
     switch (SdiRate)
     {
     case  DT_DRV_SDIRATE_SD: SrcFactor = 11; break;
     case  DT_DRV_SDIRATE_HD: SrcFactor = 2; break;
     case  DT_DRV_SDIRATE_3G: SrcFactor = 1; break;
     }
     DT_RETURN_ON_ERROR(DtBcSDITXPHY_SetUpsampleFactor(pDf->m_pBcSdiTxPhy, SrcFactor));

     // Set SDI-rate for genlocking
     DT_RETURN_ON_ERROR(DtBcSDITXPHY_SetSdiRate(pDf->m_pBcSdiTxPhy, SdiRate));
     
    // Assumption there is one clock, we need to change the clock frequency.
    DT_ASSERT(NumClocks == 1);
    DT_ASSERT(pDf->m_pBcSdiTxPll!=NULL && pDf->m_pDfSi534X!=NULL);

    // Change clock frequency
    DT_RETURN_ON_ERROR(DtDfSi534X_SetConfig(pDf->m_pDfSi534X,
                                    FracClk ? DT_DF_SI534X_CFG_FREE_RUN_FRAC_CLOCK
                                            : DT_DF_SI534X_CFG_FREE_RUN_NONFRAC_CLOCK));
    // Reset the PLL-clock
    DT_RETURN_ON_ERROR(DtBcSDITXPLL_ResetClock(pDf->m_pBcSdiTxPll));

    // Change clock rate through reconfig controller; always 3G
    DT_ASSERT(pDf->m_pDfSdiXCfgMgr != NULL);
    DT_RETURN_ON_ERROR(DtDfSdiXCfgMgr_SetXcvrMode(pDf->m_pDfSdiXCfgMgr,
                                        DtCore_PT_GetPortIndex(pDf->m_pPt),
                                        DT_SDIXCFG_CHANTYPE_TX_CHAN, DT_SDIXCFG_MODE_3G));

    // Change fractional mode through reconfig controller; always non-fractional
    DT_RETURN_ON_ERROR(DtDfSdiXCfgMgr_SetXcvrMode(pDf->m_pDfSdiXCfgMgr,
                                   DtCore_PT_GetPortIndex(pDf->m_pPt),
                                   DT_SDIXCFG_CHANTYPE_TX_PLL, DT_SDIXCFG_MODE_NON_FRAC));
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy_SdiLockStateUpdate -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Called from DPC
//
void DtDfSdiTxPhy_SdiLockStateUpdate(DtDfSdiTxPhy*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool  IsPllLocked=FALSE, IsResetInProgress=TRUE;

    DtSpinLockAcquireAtDpc(&pDf->m_SpinLock);

    switch (pDf->m_LockState)
    {
    case SDITXPHY_STATE_RECONFIG:
        // Reconfiguration busy; nothing to do
        break;

    case SDITXPHY_STATE_WAIT_FOR_PLL_LOCK:
        // Check PLL-lock
        if (pDf->m_pBcSdiTxPll != NULL)
        { 
            Status = DtBcSDITXPLL_IsPllLocked(pDf->m_pBcSdiTxPll, &IsPllLocked);
            DT_ASSERT(DT_SUCCESS(Status));
        }
        else
            IsPllLocked = TRUE; // There is no PLL in this channel; assume always lock
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
        if (pDf->m_pBcSdiTxPll != NULL)
        { 
            Status = DtBcSDITXPLL_IsPllLocked(pDf->m_pBcSdiTxPll, &IsPllLocked);
            DT_ASSERT(DT_SUCCESS(Status));
        }
        else
            IsPllLocked = TRUE; // There is no PLL in this channel; assume always lock

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
static DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetOperationalMode(const DtIoStubDfSdiTxPhy*,
                                                      DtIoctlSdiTxPhyCmdGetOpModeOutput*);
static DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetSdiRate(const DtIoStubDfSdiTxPhy*, 
                                                     DtIoctlSdiTxPhyCmdGetSdiRateOutput*);
static DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetSdiStatus(const DtIoStubDfSdiTxPhy*, 
                                                   DtIoctlSdiTxPhyCmdGetSdiStatusOutput*);
static DtStatus  DtIoStubDfSdiTxPhy_OnCmdGetUnderflowFlag(const DtIoStubDfSdiTxPhy*,
                                               DtIoctlSdiTxPhyCmdGetUnderflowFlagOutput*);
static DtStatus  DtIoStubDfSdiTxPhy_OnCmdSetOperationalMode(const DtIoStubDfSdiTxPhy*,
                                                 const DtIoctlSdiTxPhyCmdSetOpModeInput*);
static DtStatus DtIoStubDfSdiTxPhy_OnCmdSetSdiRate(const DtIoStubDfSdiTxPhy*, 
                                                const DtIoctlSdiTxPhyCmdSetSdiRateInput*);

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
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_SDITXPHY_CMD);
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

    case DT_SDITXPHY_CMD_GET_UNDERFLOW_FLAG:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSdiTxPhy_OnCmdGetUnderflowFlag(STUB_SDITXPHY, 
                                                                 &pOutData->m_GetUflFlag);
        break;

    case DT_SDITXPHY_CMD_SET_OPERATIONAL_MODE:
        Status = DtIoStubDfSdiTxPhy_OnCmdSetOperationalMode(STUB_SDITXPHY, 
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_SDITXPHY_CMD_SET_SDIRATE:
        Status = DtIoStubDfSdiTxPhy_OnCmdSetSdiRate(STUB_SDITXPHY, 
                                                                  &pInData->m_SetSdiRate);
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
    DtStatus  Status = DT_STATUS_OK;
    Bool  Fractional = FALSE;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiTxPhy));
    DT_ASSERT(pOutData != NULL);

    Status =  DtDfSdiTxPhy_GetSdiRate(STUB_DF, &pOutData->m_SdiRate, &Fractional);
    if (DT_SUCCESS(Status))
        pOutData->m_Fractional = (Int)Fractional;

    return Status;

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

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy_OnCmdSetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSdiTxPhy_OnCmdSetSdiRate(
    const DtIoStubDfSdiTxPhy*  pStub, 
    const DtIoctlSdiTxPhyCmdSetSdiRateInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSdiTxPhy));
    DT_ASSERT(pInData != NULL);

    return DtDfSdiTxPhy_SetSdiRate(STUB_DF, pInData->m_SdiRate, 
                                                             (Bool)pInData->m_Fractional);
}
