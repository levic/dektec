//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDIRXPHY.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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
#include "DtBcSDIRXPHY.h"
#include "DtBcSDIRXPHY_RegAccess.h"
#include "Messages.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIRXPHY implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDIRXPHY))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_SDIRXPHY_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(SDIRXPHY, pBc)

// Helper macro to cast a DtBc* to a DtBcSDIRXPHY*
#define BC_SDIRXPHY         ((DtBcSDIRXPHY*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcSDIRXPHY_Init(DtBc*);
static DtStatus  DtBcSDIRXPHY_OnEnable(DtBc*, Bool);
static DtStatus  DtBcSDIRXPHY_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcSDIRXPHY_CheckSdiRate(DtBcSDIRXPHY*, Int SdiRate);
static DtStatus  DtBcSDIRXPHY_WaitPhyReady(DtBcSDIRXPHY*);
static DtStatus DtBcSDIRXPHY_C10A10_StartSetSdiRate(DtBcSDIRXPHY*, Int SdiRate);
static DtStatus DtBcSDIRXPHY_C10A10_GetSetSdiRateDone(DtBcSDIRXPHY*, Bool* pDone);
static DtStatus DtBcSDIRXPHY_C10A10_FinishSetSdiRate(DtBcSDIRXPHY*);
static DtStatus DtBcSDIRXPHY_CV_StartSetSdiRate(DtBcSDIRXPHY*, Int SdiRate);
static DtStatus DtBcSDIRXPHY_CV_GetSetSdiRateDone(DtBcSDIRXPHY*, Bool* pDone);
static DtStatus DtBcSDIRXPHY_CV_FinishSetSdiRate(DtBcSDIRXPHY*);
static Bool DtBcSDIRXPHY_C10A10_IsAccessAllowed(DtBcSDIRXPHY*);
static DtStatus DtBcSDIRXPHY_V1_C10A10_StartSetSdiRate(DtBcSDIRXPHY*, Int SdiRate);
static DtStatus DtBcSDIRXPHY_V1_C10A10_GetSetSdiRateDone(DtBcSDIRXPHY*, Bool* pDone);
static DtStatus DtBcSDIRXPHY_V1_C10A10_FinishSetSdiRate(DtBcSDIRXPHY*);
static DtStatus DtBcSDIRXPHY_V2_C10A10_StartSetSdiRate(DtBcSDIRXPHY*, Int SdiRate);
static DtStatus DtBcSDIRXPHY_V2_C10A10_ConfigurePllRate(DtBcSDIRXPHY*, Int SdiRate);
static DtStatus DtBcSDIRXPHY_V2_C10A10_GetSetSdiRateDone(DtBcSDIRXPHY*, Bool* pDone);
static DtStatus DtBcSDIRXPHY_V2_C10A10_FinishSetSdiRate(DtBcSDIRXPHY*);
static DtStatus DtBcSDIRXPHY_V2_C10A10_ConfigureUserCalibration(DtBcSDIRXPHY*);
static void  DtBcSDIRXPHY_SetControlRegs(DtBcSDIRXPHY*, Bool BlkEnable,  Int OpMode,
                                      Bool RxClkReset,  Bool PllReset,  Bool XcvrReset,
                                      Int LockMode, Bool  DownsamplerEnable, Int SdiRate);
static DtStatus DtBcSDIRXPHY_C10A10_RequestAccess(DtBcSDIRXPHY*, Bool AllowLongSleep);
static void DtBcSDIRXPHY_C10A10_ReleaseAccess(DtBcSDIRXPHY*, Bool TriggerCalibration);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIRXPHY - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSDIRXPHY_Close(DtBc*  pBc)
{
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));
       
    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcSDIRXPHY*  DtBcSDIRXPHY_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_SDIRXPHY_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcSDIRXPHY, Id, DT_BLOCK_TYPE_SDIRXPHY,
                                                              Address, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcSDIRXPHY_Close;
    OpenParams.m_InitFunc = DtBcSDIRXPHY_Init;
    OpenParams.m_OnEnableFunc = DtBcSDIRXPHY_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcSDIRXPHY_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcSDIRXPHY*)DtBc_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_ClearCdcFifoOverflow -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_ClearCdcFifoOverflow(DtBcSDIRXPHY* pBc)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);

    // Clear overflow flag
    SDIRXPHY_CdcFifoStatus_CLEAR_Overflow(pBc);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_GetCdcFifoStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_GetCdcFifoStatus(DtBcSDIRXPHY* pBc, Int* pFifoLoad, Bool* pOverflow)
{
    UInt32  RegStatus = 0;
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pFifoLoad==NULL || pOverflow==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);

    // Return carrier detect status
    RegStatus = SDIRXPHY_CdcFifoStatus_READ(pBc);
    *pFifoLoad =  (Int)SDIRXPHY_CdcFifoStatus_GET_Load(RegStatus);
    *pOverflow = (SDIRXPHY_CdcFifoStatus_GET_Overflow(RegStatus) != 0);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_GetClockReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_GetClockReset(DtBcSDIRXPHY * pBc, Bool* pClkReset)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pClkReset == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);

    // Return last cached clock reset
    *pClkReset = pBc->m_ClockReset;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_GetDeviceFamily -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_GetDeviceFamily(DtBcSDIRXPHY * pBc, Int * pDeviceFamily)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pDeviceFamily == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);

    // Return cached device family
    *pDeviceFamily = pBc->m_DeviceFamily;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_GetMaxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_GetMaxSdiRate(DtBcSDIRXPHY * pBc, Int* pMaxSdiRate)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pMaxSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);

    // Return cached maximum SDI-rate
    *pMaxSdiRate = pBc->m_MaxSdiRate;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_GetOperationalMode(DtBcSDIRXPHY* pBc, Int * pOpMode)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);

    // Return last cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_GetLockMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_GetLockMode(DtBcSDIRXPHY* pBc, Int* pLockMode)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pLockMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
    
    // Return cached value
    *pLockMode = pBc->m_LockMode;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_GetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_GetSdiRate(DtBcSDIRXPHY* pBc, Int* pSdiRate)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
    
    // Return cached value
    *pSdiRate = pBc->m_SdiRate;
    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_IsCarrierDetect -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_IsCarrierDetect(DtBcSDIRXPHY* pBc, Bool* pCarrier)
{
    UInt32  RegStatus;
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pCarrier == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);

    // Return carrier detect status
    RegStatus = SDIRXPHY_Status_READ(pBc);
    *pCarrier =  (SDIRXPHY_Status_GET_CarrierDetect(RegStatus)!=0)
                                   && (SDIRXPHY_Status_GET_ResetInProgress(RegStatus)==0);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_IsLockedToData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_IsLockedToData(DtBcSDIRXPHY* pBc, Bool* pLocked)
{
    UInt32  RegStatus;

    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pLocked == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
    
    // Check lock mode
    if (pBc->m_LockMode != DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_DATA)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "ERROR: Not supported for this lockmode");
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Return locked to data
    RegStatus = SDIRXPHY_Status_READ(pBc);
    *pLocked =  (SDIRXPHY_Status_GET_LockedToData(RegStatus)!=0)
                                   && (SDIRXPHY_Status_GET_ResetInProgress(RegStatus)==0);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_IsLockedToRef -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_IsLockedToRef(DtBcSDIRXPHY* pBc, Bool* pLocked)
{
    UInt32  RegStatus;

    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pLocked == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
    
    // Check lock mode
    if (pBc->m_LockMode != DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_REF)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "ERROR: Not supported for this lockmode");
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Return locked to reference
    RegStatus = SDIRXPHY_Status_READ(pBc);
    *pLocked =  (SDIRXPHY_Status_GET_LockedToRef(RegStatus)!=0)
                                   && (SDIRXPHY_Status_GET_ResetInProgress(RegStatus)==0);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_SetClockReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_SetClockReset(DtBcSDIRXPHY* pBc, Bool ClkReset)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
    
    // Operational mode must be IDLE or STANDBY
    if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "Operational mode not in idle/standby");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // No change?
    if (pBc->m_ClockReset == ClkReset)
        return DT_STATUS_OK;

    // Update cache
    pBc->m_ClockReset = ClkReset;
    // Make settings in register
    DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                               pBc->m_OperationalMode, pBc->m_ClockReset, FALSE, FALSE,
                               pBc->m_LockMode, pBc->m_DownsamplerEnable, pBc->m_SdiRate);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_SetDownsamplerEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSDIRXPHY_SetDownsamplerEnable(DtBcSDIRXPHY* pBc, Bool Enable)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
    
    // Operational mode must be IDLE/STANDBY
    if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "Operational mode not in idle/standby");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // No change?
    if (pBc->m_DownsamplerEnable == Enable)
        return DT_STATUS_OK;

    // Update cache
    pBc->m_DownsamplerEnable = Enable;
    // Make settings in register
    DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                               pBc->m_OperationalMode, pBc->m_ClockReset, FALSE, FALSE,
                               pBc->m_LockMode, pBc->m_DownsamplerEnable, pBc->m_SdiRate);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_SetOperationalMode(DtBcSDIRXPHY * pBc, Int OpMode)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY 
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
   
    // Clock reset should be de-asserted when going to run
    if (pBc->m_ClockReset && OpMode==DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "Clock reset busy");
        return DT_STATUS_BUSY;
    }
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Clear overflow
    if (OpMode==DT_BLOCK_OPMODE_STANDBY || OpMode==DT_BLOCK_OPMODE_RUN)
        SDIRXPHY_CdcFifoStatus_CLEAR_Overflow(pBc);

    // Save operational mode
    pBc->m_OperationalMode = OpMode;
    // Make settings in register
    DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                               pBc->m_OperationalMode, pBc->m_ClockReset, FALSE, FALSE,
                               pBc->m_LockMode, pBc->m_DownsamplerEnable, pBc->m_SdiRate);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_SetLockMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_SetLockMode(DtBcSDIRXPHY* pBc, Int LockMode)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (LockMode!=DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_DATA && 
                                        LockMode!=DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_REF)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "Invalid lock mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
    
    // Operational mode must be IDLE or STANDBY
    if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "Operational mode not in idle");
        return DT_STATUS_INVALID_IN_OPMODE;
    }
    // No change?
    if (pBc->m_LockMode == LockMode)
        return DT_STATUS_OK;

    // Update cache
    pBc->m_LockMode = LockMode;
    // Make settings in register
    DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                               pBc->m_OperationalMode, pBc->m_ClockReset, FALSE, FALSE,
                               pBc->m_LockMode, pBc->m_DownsamplerEnable, pBc->m_SdiRate);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_SetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_StartSetSdiRate(DtBcSDIRXPHY* pBc, Int SdiRate)
{
    DtStatus  Status=DT_STATUS_OK;
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    DT_RETURN_ON_ERROR(DtBcSDIRXPHY_CheckSdiRate(pBc, SdiRate));

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
    
    // Operational mode must be IDLE or STANDBY
    if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "Operational mode not in idle");
        return DT_STATUS_INVALID_IN_OPMODE;
    }
    // No change?
    if (pBc->m_SdiRate == SdiRate)
        return DT_STATUS_OK;

    // Update cache
    pBc->m_SdiRate = SdiRate;
    // Make settings in register
    DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                               pBc->m_OperationalMode, pBc->m_ClockReset, FALSE, FALSE,
                               pBc->m_LockMode, pBc->m_DownsamplerEnable, pBc->m_SdiRate);

    // Device family specific function
    switch (pBc->m_DeviceFamily)
    {
    case DT_BC_SDIRXPHY_FAMILY_A10:
        Status = DtBcSDIRXPHY_C10A10_StartSetSdiRate(pBc, SdiRate);
        break;
    case DT_BC_SDIRXPHY_FAMILY_C10:
        Status = DtBcSDIRXPHY_C10A10_StartSetSdiRate(pBc, SdiRate);
        break;
    case DT_BC_SDIRXPHY_FAMILY_CV:
        Status = DtBcSDIRXPHY_CV_StartSetSdiRate(pBc, SdiRate);
        break;
    default: DT_ASSERT(FALSE);
        Status = DT_STATUS_FAIL;
        break;
    }
    if (DT_SUCCESS(Status))
        pBc->m_SetSdiRateBusy = TRUE;

    // Check error status indicator
    if (pBc->m_Version >= 5)
        DT_ASSERT(SDIRXPHY_Status_READ_AccessTimedout(pBc) == 0);
    return Status;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_GetSetSdiRateDone -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_GetSetSdiRateDone(DtBcSDIRXPHY* pBc, Bool* pDone)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pDone == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);

    // Device family specific function
    switch (pBc->m_DeviceFamily)
    {
    case DT_BC_SDIRXPHY_FAMILY_A10:
        Status = DtBcSDIRXPHY_C10A10_GetSetSdiRateDone(pBc, pDone);
        break;
    case DT_BC_SDIRXPHY_FAMILY_C10:
        Status = DtBcSDIRXPHY_C10A10_GetSetSdiRateDone(pBc, pDone);
        break;
    case DT_BC_SDIRXPHY_FAMILY_CV:
        Status = DtBcSDIRXPHY_CV_GetSetSdiRateDone(pBc, pDone);
        break;
    default: DT_ASSERT(FALSE);  Status =  DT_STATUS_FAIL;
        break;
    }

    // Check error status indicator
    if (pBc->m_Version >= 5)
        DT_ASSERT(SDIRXPHY_Status_READ_AccessTimedout(pBc) == 0);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_FinishSetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_FinishSetSdiRate(DtBcSDIRXPHY* pBc)
{
    DtStatus  Status=DT_STATUS_OK;
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);

    // Device family specific function
    switch (pBc->m_DeviceFamily)
    {
    case DT_BC_SDIRXPHY_FAMILY_A10:
        Status = DtBcSDIRXPHY_C10A10_FinishSetSdiRate(pBc);
        break;
    case DT_BC_SDIRXPHY_FAMILY_C10:
        Status = DtBcSDIRXPHY_C10A10_FinishSetSdiRate(pBc);
        break;
    case DT_BC_SDIRXPHY_FAMILY_CV:
        Status = DtBcSDIRXPHY_CV_FinishSetSdiRate(pBc);
        break;
    default: DT_ASSERT(FALSE);
        Status = DT_STATUS_FAIL;
        break;
    }
    pBc->m_SetSdiRateBusy = FALSE;

    // Check error status indicator
    if (pBc->m_Version >= 5)
        DT_ASSERT(SDIRXPHY_Status_READ_AccessTimedout(pBc) == 0);
    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIRXPHY - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSDIRXPHY_Init(DtBc*  pBcBase)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcSDIRXPHY* pBc = (DtBcSDIRXPHY*) pBcBase;
    UInt32  FwMaxSdiRate, FwDeviceFamily;

    // Sanity checks
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Get device family
    FwDeviceFamily = SDIRXPHY_Config_READ_DeviceFamily(pBc);
    switch (FwDeviceFamily)
    {
    case SDIRXPHY_FAMILY_A10:  pBc->m_DeviceFamily = DT_BC_SDIRXPHY_FAMILY_A10; break;
    case SDIRXPHY_FAMILY_C10:  pBc->m_DeviceFamily = DT_BC_SDIRXPHY_FAMILY_C10; break;
    case SDIRXPHY_FAMILY_CV:   pBc->m_DeviceFamily = DT_BC_SDIRXPHY_FAMILY_CV;  break;
    default: DT_ASSERT(FALSE);  return DT_STATUS_FAIL;
    }

    // Get maximum supported rate
    FwMaxSdiRate = SDIRXPHY_Config_READ_MaxSdiRate(pBc);
    switch (FwMaxSdiRate)
    {
    case SDIRXPHY_SDIMODE_SD:  pBc->m_MaxSdiRate = DT_DRV_SDIRATE_SD; break;
    case SDIRXPHY_SDIMODE_HD:  pBc->m_MaxSdiRate = DT_DRV_SDIRATE_HD; break;
    case SDIRXPHY_SDIMODE_3G:  pBc->m_MaxSdiRate = DT_DRV_SDIRATE_3G; break;
    case SDIRXPHY_SDIMODE_6G:  pBc->m_MaxSdiRate = DT_DRV_SDIRATE_6G; break;
    case SDIRXPHY_SDIMODE_12G: pBc->m_MaxSdiRate = DT_DRV_SDIRATE_12G; break;
    default: DT_ASSERT(FALSE);  return DT_STATUS_FAIL;
    }

    // Clear overflow
    SDIRXPHY_CdcFifoStatus_CLEAR_Overflow(pBc);

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_ClockReset = TRUE;
    pBc->m_LockMode = DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_REF;
    pBc->m_DownsamplerEnable = FALSE;
    pBc->m_SetSdiRateBusy = FALSE;
    pBc->m_SdiRate = DT_DRV_SDIRATE_HD;
    DT_ASSERT(DT_SUCCESS(DtBcSDIRXPHY_CheckSdiRate(pBc, pBc->m_SdiRate)));

    // Make settings in register
    DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                               pBc->m_OperationalMode, pBc->m_ClockReset, FALSE, FALSE,
                               pBc->m_LockMode, pBc->m_DownsamplerEnable, pBc->m_SdiRate);

    // The device specific initialization cannot be performed when the block is in IDLE
    // It will be performed in OnEnable()


    // Clear error status indicator
    if (pBc->m_Version >= 5)
        SDIRXPHY_Status_CLEAR_AccessTimedout(pBc);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcSDIRXPHY* pBc = (DtBcSDIRXPHY*) pBcBase;
    UInt32  RegResetBusy=0, RegCalBusy=0;
    Int  TimeoutCount=0;
 
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE-> ENABLE
        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
        pBc->m_ClockReset = TRUE;
        pBc->m_LockMode = DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_REF;
        pBc->m_DownsamplerEnable = FALSE;
        pBc->m_SetSdiRateBusy = FALSE;

        // Save block enable
        pBc->m_BlockEnabled = Enable;
        // Make settings in register
        DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                               pBc->m_OperationalMode, pBc->m_ClockReset, FALSE, FALSE,
                               pBc->m_LockMode, pBc->m_DownsamplerEnable, pBc->m_SdiRate);

        // Wait a while such that the reset is completed
        DtWaitBlock(1);
        RegResetBusy = SDIRXPHY_Status_READ_ResetInProgress(pBc);
        TimeoutCount = 100;
        while (RegResetBusy!=0 && TimeoutCount>0)
        {
            DtWaitBlock(1);
            TimeoutCount--;
            RegResetBusy = SDIRXPHY_Status_READ_ResetInProgress(pBc);
        }
        DT_ASSERT(RegResetBusy == 0);
        if (RegResetBusy != 0)
            DtDbgOutBc(ERR, SDIRXPHY, pBc, "Reset in progress timeout");

        // Wait till power-up calibration is completed
        RegCalBusy = SDIRXPHY_Status_READ_CalBusy(pBc);
        TimeoutCount = 1000;
        while (RegCalBusy!=0 && TimeoutCount>0)
        {
            DtSleep(1);
            TimeoutCount--;
            RegCalBusy = SDIRXPHY_Status_READ_CalBusy(pBc);
        }
        DT_ASSERT(RegCalBusy == 0);
        if (RegCalBusy != 0)
            DtDbgOutBc(ERR, SDIRXPHY, pBc, "Cal busy timeout");

        // Wait till RX PHY is ready
        DT_RETURN_ON_ERROR(DtBcSDIRXPHY_WaitPhyReady(pBc));

        // Device family specific initialisation
        if (pBc->m_DeviceFamily==DT_BC_SDIRXPHY_FAMILY_A10
                                        || pBc->m_DeviceFamily==DT_BC_SDIRXPHY_FAMILY_C10)
        {
            Bool Done = FALSE;
            // Start setting SDI-rate
            Status = DtBcSDIRXPHY_C10A10_StartSetSdiRate(pBc, pBc->m_SdiRate);
            DT_ASSERT(DT_SUCCESS(Status));
            if (!DT_SUCCESS(Status))
                return Status;

            // Wait till completed
            Status = DtBcSDIRXPHY_C10A10_GetSetSdiRateDone(pBc, &Done);
            TimeoutCount = 200;
            while (!Done && DT_SUCCESS(Status) && TimeoutCount>0)
            {
                DtSleep(1);
                TimeoutCount--;
                Status = DtBcSDIRXPHY_C10A10_GetSetSdiRateDone(pBc, &Done);
            }
            if (!Done)
            { 
                DtDbgOutBc(ERR, SDIRXPHY, pBc, "Calibration timeout");
                Status = DT_STATUS_TIMEOUT;
            }
            // Finish setting SDI-rate
            DtBcSDIRXPHY_C10A10_FinishSetSdiRate(pBc);
        }
    }
    else
    {
        // ENABLED -> DISABLED?
        // Make sure set SDI-rate is completed
        Bool Done = FALSE;
        if (pBc->m_SetSdiRateBusy)
        {
            // Wait till set SDI-rate is completed
            Status = DtBcSDIRXPHY_GetSetSdiRateDone(pBc, &Done);
            TimeoutCount = 200;
            while (!Done && DT_SUCCESS(Status) && TimeoutCount>0)
            {
                DtSleep(1);
                TimeoutCount--;
                Status = DtBcSDIRXPHY_GetSetSdiRateDone(pBc, &Done);
            }
            if (!Done)
                DtDbgOutBc(ERR, SDIRXPHY, pBc, "Calibration timeout");

            // Finish setting SDI-rate
            DtBcSDIRXPHY_FinishSetSdiRate(pBc);
        }

        // Operational mode to IDLE
        Status = DtBcSDIRXPHY_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SDIRXPHY, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }

        // Save block enable
        pBc->m_BlockEnabled = Enable;

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
        pBc->m_ClockReset = TRUE;
        pBc->m_LockMode = DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_REF;
        pBc->m_DownsamplerEnable = FALSE;
        pBc->m_SetSdiRateBusy = FALSE;

        // Make settings in register
        DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                               pBc->m_OperationalMode, pBc->m_ClockReset, FALSE, FALSE,
                               pBc->m_LockMode, pBc->m_DownsamplerEnable, pBc->m_SdiRate);
    }
   return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSDIRXPHY_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled(pBc))
    {
        DtDbgOutBc(AVG, SDIRXPHY, pBc, "Set operational mode to IDLE");

        // Set operational mode to IDLE
        Status = DtBcSDIRXPHY_SetOperationalMode(BC_SDIRXPHY, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SDIRXPHY, pBc, "ERROR: failed to set opmode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_CheckSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_CheckSdiRate(DtBcSDIRXPHY* pBc, Int SdiRate)
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
    if (SdiRate > pBc->m_MaxSdiRate)
        return DT_STATUS_NOT_SUPPORTED;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_WaitPhyReady -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_WaitPhyReady(DtBcSDIRXPHY* pBc)
{
    switch (pBc->m_DeviceFamily)
    {
    case SDIRXPHY_FAMILY_A10:
    case SDIRXPHY_FAMILY_C10:
        DT_RETURN_ON_ERROR(DtBcSDIRXPHY_C10A10_RequestAccess(pBc, TRUE));
        DtBcSDIRXPHY_C10A10_ReleaseAccess(pBc, FALSE);
        return DT_STATUS_OK;
    case SDIRXPHY_FAMILY_CV:
        return DT_STATUS_OK;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_FAIL;
    }
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_C10A10_StartSetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_C10A10_StartSetSdiRate(DtBcSDIRXPHY* pBc, Int SdiRate)
{
    DtStatus  Status = DT_STATUS_OK;

    // Version specific StartSetSdiRate
    if (pBc->m_Version == 1)
        Status = DtBcSDIRXPHY_V1_C10A10_StartSetSdiRate(pBc, SdiRate);
    else if (pBc->m_Version >= 2)
        Status = DtBcSDIRXPHY_V2_C10A10_StartSetSdiRate(pBc, SdiRate);
   else
        Status = DT_STATUS_FAIL;
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_C10A10_GetSetSdiRateDone -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_C10A10_GetSetSdiRateDone(DtBcSDIRXPHY* pBc, Bool* pDone)
{
    DtStatus  Status = DT_STATUS_OK;

    // Version specific StartSetSdiRate
    if (pBc->m_Version == 1)
        Status = DtBcSDIRXPHY_V1_C10A10_GetSetSdiRateDone(pBc, pDone);
    else if (pBc->m_Version >= 2)
        Status = DtBcSDIRXPHY_V2_C10A10_GetSetSdiRateDone(pBc, pDone);
   else
        Status = DT_STATUS_FAIL;
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_C10A10_FinishSetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_C10A10_FinishSetSdiRate(DtBcSDIRXPHY* pBc)
{
    DtStatus  Status = DT_STATUS_OK;

    // Version specific StartSetSdiRate
    if (pBc->m_Version == 1)
        Status = DtBcSDIRXPHY_V1_C10A10_FinishSetSdiRate(pBc);
    else if (pBc->m_Version >= 2)
        Status = DtBcSDIRXPHY_V2_C10A10_FinishSetSdiRate(pBc);
   else
        Status = DT_STATUS_FAIL;
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_CV_StartSetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_CV_StartSetSdiRate(DtBcSDIRXPHY* pBc, Int SdiRate)
{
    // Nothing to do  for CV
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_CV_GetSetSdiRateDone -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_CV_GetSetSdiRateDone(DtBcSDIRXPHY* pBc, Bool* pDone)
{
    // Nothing to do  for CV
    *pDone = TRUE;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_CV_FinishSetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_CV_FinishSetSdiRate(DtBcSDIRXPHY* pBc)
{
    // Reset the PLL
    DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                               pBc->m_OperationalMode, pBc->m_ClockReset, TRUE, FALSE,
                               pBc->m_LockMode, pBc->m_DownsamplerEnable, pBc->m_SdiRate);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_C10A10_IsAccessAllowed -.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool DtBcSDIRXPHY_C10A10_IsAccessAllowed(DtBcSDIRXPHY* pBc)
{
    if (pBc->m_Version <= 1)
        return TRUE;  // There is no waitrequest
    else if (pBc->m_Version <= 4)
        return SDIRXPHY_Status_READ_WaitRequest(pBc) == 0;
    else
        return SDIRXPHY_C10A10_CalCapability_READ_AvMmBusy(pBc) == SDIRXPHY_ARBOWNER_User;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_V1_C10A10_SetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_V1_C10A10_StartSetSdiRate(DtBcSDIRXPHY* pBc, Int SdiRate)
{
    UInt32  RegCdrPllSettings4, RegCdrPllSettings9;

    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);
    
    // No wait request field available in v1 !!!!

    // Read registers
    RegCdrPllSettings4 = SDIRXPHY_C10A10_CdrPllSettings4_READ(pBc);
    RegCdrPllSettings9 = SDIRXPHY_C10A10_CdrPllSettings9_READ(pBc);
    if (SdiRate==DT_DRV_SDIRATE_SD || SdiRate==DT_DRV_SDIRATE_3G)
    { 
        // Set loop filter and dividers for 3Gbps operation
        RegCdrPllSettings4 = SDIRXPHY_C10A10_CdrPllSettings4_SET_LfResistorPd(
                                             RegCdrPllSettings4, SDIRXPHY_LFPD_SETTING_3);
        RegCdrPllSettings9 = SDIRXPHY_C10A10_CdrPllSettings9_SET_PdLCounter(
                                          RegCdrPllSettings9, SDIRXPHY_LCOUNTER_DIV_BY_4);
    }
    else
    { 
        // Set loop filter and dividers for HD operation
        RegCdrPllSettings4 = SDIRXPHY_C10A10_CdrPllSettings4_SET_LfResistorPd(
                                             RegCdrPllSettings4, SDIRXPHY_LFPD_SETTING_0);
        RegCdrPllSettings9 = SDIRXPHY_C10A10_CdrPllSettings9_SET_PdLCounter(
                                          RegCdrPllSettings9, SDIRXPHY_LCOUNTER_DIV_BY_8);
    }
    // Write registers
    SDIRXPHY_C10A10_CdrPllSettings4_WRITE(pBc, RegCdrPllSettings4);
    SDIRXPHY_C10A10_CdrPllSettings9_WRITE(pBc, RegCdrPllSettings9);


    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_V1_C10A10_GetSetSdiRateDone -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_V1_C10A10_GetSetSdiRateDone(DtBcSDIRXPHY* pBc, Bool* pDone)
{
    // All done  for C10/A10 Version 1
    *pDone = TRUE;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_V1_C10A10_FinishSetSdiRate -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_V1_C10A10_FinishSetSdiRate(DtBcSDIRXPHY* pBc)
{
    // Reset the PLL
    DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                               pBc->m_OperationalMode, pBc->m_ClockReset, TRUE, FALSE,
                               pBc->m_LockMode, pBc->m_DownsamplerEnable, pBc->m_SdiRate);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_V2_C10A10_SetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.
//
//  Settings include 6/12G register
//
DtStatus DtBcSDIRXPHY_V2_C10A10_StartSetSdiRate(DtBcSDIRXPHY* pBc, Int SdiRate)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32  RegCalBusy;

    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // CalBusy should be de-asserted
    RegCalBusy = SDIRXPHY_Status_READ_CalBusy(pBc);
    DT_ASSERT(RegCalBusy ==0);

    // 1. Place the RX simplex channel in DIGITAL reset (rx_digital_reset) 
    // then Analog reset (rx_analog_reset)
    DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                       pBc->m_OperationalMode, pBc->m_ClockReset, FALSE, TRUE,
                       pBc->m_LockMode, pBc->m_DownsamplerEnable, pBc->m_SdiRate);

    // 2. Check for internal configuration bus arbitration. Check that 
    // “reconfig_waitrequest” is not high before writing to the reconfiguration interface.
    DT_RETURN_ON_ERROR(DtBcSDIRXPHY_C10A10_RequestAccess(pBc, FALSE));

    // 3. Reconfigure PLL
    Status = DtBcSDIRXPHY_V2_C10A10_ConfigurePllRate(pBc, SdiRate);
    if (Status != DT_STATUS_OK)
    {
        DtBcSDIRXPHY_C10A10_ReleaseAccess(pBc, FALSE);
        return Status;
    }
    // 4. Reconfigure the PMA analog settings. Currently not required
    
    // 5. Configure user calibration
    Status = DtBcSDIRXPHY_V2_C10A10_ConfigureUserCalibration(pBc);

    // 5.5 Release configuration bus to PreSICE and trigger calibration
    DtBcSDIRXPHY_C10A10_ReleaseAccess(pBc, TRUE);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_V2_C10A10_GetSetSdiRateDone -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_V2_C10A10_GetSetSdiRateDone(DtBcSDIRXPHY* pBc, Bool* pDone)
{
    // 5.6 Wait until CalDone is asserted
    UInt32  RegCalBusy = SDIRXPHY_Status_READ_CalBusy(pBc);
    *pDone = (RegCalBusy == 0);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_V2_C10A10_FinishSetSdiRate -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_V2_C10A10_FinishSetSdiRate(DtBcSDIRXPHY* pBc)
{
    // 7. Release the ANALOG reset (rx_analog_reset) then 
    // the DIGITAL reset (rx_digital_reset) 
    DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                       pBc->m_OperationalMode, pBc->m_ClockReset, FALSE, FALSE,
                       pBc->m_LockMode, pBc->m_DownsamplerEnable, pBc->m_SdiRate);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_V2_C10A10_StartUserCalibration -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSDIRXPHY_V2_C10A10_ConfigureUserCalibration(DtBcSDIRXPHY* pBc)
{
    UInt32  RegData=0;

    // 5.1 Request access to calibration registers. Access already allowed

    // 5.3 CalEnable.RxCalEn =1  CalEnable.AdaptEn = 0 
    RegData = SDIRXPHY_C10A10_CalEnable_READ(pBc);
    RegData = SDIRXPHY_C10A10_CalEnable_SET_RxCalEn(RegData, SDIRXPHY_CALEN_Enable);
    RegData = SDIRXPHY_C10A10_CalEnable_SET_TxCalEn(RegData, SDIRXPHY_CALEN_Disable);
    RegData = SDIRXPHY_C10A10_CalEnable_SET_AdaptEn(RegData, SDIRXPHY_CALEN_Disable);
    SDIRXPHY_C10A10_CalEnable_WRITE(pBc, RegData);

    // 5.4 PmaSettings.RateSwitchFlag=0 for a CDR rate switch
    // write a 1 if no CDR rate switch with different CDR bandwidth setting 
    RegData = SDIRXPHY_C10A10_PmaSettings_READ(pBc);
    RegData = SDIRXPHY_C10A10_PmaSettings_SET_RateSwitchFlag(RegData,
                                                            SDIRXPHY_RATESWITCH_Switched);
    SDIRXPHY_C10A10_PmaSettings_WRITE(pBc, RegData);

    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_V2_C10A10_ConfigurePllRate -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_V2_C10A10_ConfigurePllRate(DtBcSDIRXPHY* pBc, Int SdiRate)
{
    UInt32  RegCdrPllSettings4, RegCdrPllSettings9, RegPcsPmaSel, RegPcsRxBlockSel,
        RegRxPcs8G0, RegRxPcs8G1, RegRxPcs8G2, RegRxPcs8G3, RegRxPcs8G4,
        RegRxPcs8G5, RegRxPcs8G6, RegRxPcs8G7, RegRxPcs10G0, RegRxPcs10G1,
        RegRxPcs10G2, RegRxPcs10G3, RegRxPcs10G4, RegRxPcs10G5, RegRxPcsFifo,
        RegCdrPllSettings8, RegCdrPllSettings10, RegPmaRxDeser, RegPmaEqBw;

    // Read registers
    RegCdrPllSettings4 = SDIRXPHY_C10A10_CdrPllSettings4_READ(pBc);
    RegCdrPllSettings9 = SDIRXPHY_C10A10_CdrPllSettings9_READ(pBc);
    RegPcsPmaSel = SDIRXPHY_C10A10_PcsPmaSel_READ(pBc);
    RegPcsRxBlockSel = SDIRXPHY_C10A10_PcsRxBlockSel_READ(pBc);
    RegRxPcs8G0 = SDIRXPHY_C10A10_RxPcs8G0_READ(pBc);
    RegRxPcs8G1 = SDIRXPHY_C10A10_RxPcs8G1_READ(pBc);
    RegRxPcs8G2 = SDIRXPHY_C10A10_RxPcs8G2_READ(pBc);
    RegRxPcs8G3 = SDIRXPHY_C10A10_RxPcs8G3_READ(pBc);
    RegRxPcs8G4 = SDIRXPHY_C10A10_RxPcs8G4_READ(pBc);
    RegRxPcs8G5 = SDIRXPHY_C10A10_RxPcs8G5_READ(pBc);
    RegRxPcs8G6 = SDIRXPHY_C10A10_RxPcs8G6_READ(pBc);
    RegRxPcs8G7 = SDIRXPHY_C10A10_RxPcs8G7_READ(pBc);
    RegRxPcs10G0 = SDIRXPHY_C10A10_RxPcs10G0_READ(pBc);
    RegRxPcs10G1 = SDIRXPHY_C10A10_RxPcs10G1_READ(pBc);
    RegRxPcs10G2 = SDIRXPHY_C10A10_RxPcs10G2_READ(pBc);
    RegRxPcs10G3 = SDIRXPHY_C10A10_RxPcs10G3_READ(pBc);
    RegRxPcs10G4 = SDIRXPHY_C10A10_RxPcs10G4_READ(pBc);
    RegRxPcs10G5 = SDIRXPHY_C10A10_RxPcs10G5_READ(pBc);
    RegRxPcsFifo = SDIRXPHY_C10A10_RxPcsFifo_READ(pBc);
    RegCdrPllSettings8 = SDIRXPHY_C10A10_CdrPllSettings8_READ(pBc);
    RegCdrPllSettings10 = SDIRXPHY_C10A10_CdrPllSettings10_READ(pBc);
    RegPmaRxDeser = SDIRXPHY_C10A10_PmaRxDeser_READ(pBc);
    RegPmaEqBw = SDIRXPHY_C10A10_PmaEqSettings_READ(pBc);

    // Modify registers
    if (SdiRate==DT_DRV_SDIRATE_SD || SdiRate==DT_DRV_SDIRATE_3G)
    { 
        // Set loop filter and dividers for 3Gbps operation
        RegCdrPllSettings4 = SDIRXPHY_C10A10_CdrPllSettings4_SET_LfResistorPd(
                                             RegCdrPllSettings4, SDIRXPHY_LFPD_SETTING_3);
        RegCdrPllSettings9 = SDIRXPHY_C10A10_CdrPllSettings9_SET_PdLCounter(
                                          RegCdrPllSettings9, SDIRXPHY_LCOUNTER_DIV_BY_4);
    }
    else if (SdiRate==DT_DRV_SDIRATE_HD)
    { 
        // Set loop filter and dividers for HD operation
        RegCdrPllSettings4 = SDIRXPHY_C10A10_CdrPllSettings4_SET_LfResistorPd(
                                             RegCdrPllSettings4, SDIRXPHY_LFPD_SETTING_0);
        RegCdrPllSettings9 = SDIRXPHY_C10A10_CdrPllSettings9_SET_PdLCounter(
                                          RegCdrPllSettings9, SDIRXPHY_LCOUNTER_DIV_BY_8);
    }
    else if (SdiRate==DT_DRV_SDIRATE_6G)
    { 
        // Set loop filter and dividers for 12G operation
        RegCdrPllSettings4 = SDIRXPHY_C10A10_CdrPllSettings4_SET_LfResistorPd(
                                             RegCdrPllSettings4, SDIRXPHY_LFPD_SETTING_3);
        RegCdrPllSettings9 = SDIRXPHY_C10A10_CdrPllSettings9_SET_PdLCounter(
                                          RegCdrPllSettings9, SDIRXPHY_LCOUNTER_DIV_BY_2);
    }
    else if (SdiRate==DT_DRV_SDIRATE_12G)
    { 
        // Set loop filter and dividers for 12G operation
        RegCdrPllSettings4 = SDIRXPHY_C10A10_CdrPllSettings4_SET_LfResistorPd(
                                             RegCdrPllSettings4, SDIRXPHY_LFPD_SETTING_2);
        RegCdrPllSettings9 = SDIRXPHY_C10A10_CdrPllSettings9_SET_PdLCounter(
                                          RegCdrPllSettings9, SDIRXPHY_LCOUNTER_DIV_BY_1);
    }
    else
    {
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }

    if (SdiRate==DT_DRV_SDIRATE_SD || SdiRate==DT_DRV_SDIRATE_HD
                                                            || SdiRate==DT_DRV_SDIRATE_3G)
    { 
        // PCS PMA select : 8G PCS
        RegPcsPmaSel = SDIRXPHY_C10A10_PcsPmaSel_SET_PcsPmaBlockSel(RegPcsPmaSel,
                                                                SDIRXPHY_PCS_EIGHT_G_PCS);
        // PCS RX BlockSelect  8G and 8G clock out
        RegPcsRxBlockSel = SDIRXPHY_C10A10_PcsRxBlockSel_SET_RxBlockSel(RegPcsRxBlockSel,
                                                                SDIRXPHY_RXBLKSEL_EIGHTG);
        RegPcsRxBlockSel = SDIRXPHY_C10A10_PcsRxBlockSel_SET_RxClkOut(RegPcsRxBlockSel,
                                                        SDIRXPHY_RXCLKOUT_EIGHTG_CLK_OUT);
        // 8G Rx PCS 0: PMA double width: 10
        RegRxPcs8G0 = SDIRXPHY_C10A10_RxPcs8G0_SET_PmaDw(RegRxPcs8G0,  SDIRXPHY_PMADW_10);
        // 8G Rx PCS 1: Word aligner pattern length: 7 WaPldControlled: SW
        RegRxPcs8G1 = SDIRXPHY_C10A10_RxPcs8G1_SET_WaPd(RegRxPcs8G1, 
                                                                   SDIRXPHY_WAPD_WA_PD_7);
        RegRxPcs8G1 = SDIRXPHY_C10A10_RxPcs8G1_SET_WaPldControlled(RegRxPcs8G1, 
                                                            SDIRXPHY_PLDCTRL_PLD_CTRL_SW);
        // 8G Rx PCS 2: 8B 10B decoder: Disable
        RegRxPcs8G2 = SDIRXPHY_C10A10_RxPcs8G2_SET_Decoder8B10B(RegRxPcs8G2, 
                                                                  SDIRXPHY_8B10B_DISABLE);
        // 8G Rx PCS 3: Byte deserializer: 2
        RegRxPcs8G3 = SDIRXPHY_C10A10_RxPcs8G3_SET_ByteDeserializer(RegRxPcs8G3, 
                                                                    SDIRXPHY_BDS_EN_BY_2);
        // 8G Rx PCS 4: Phase compensator read Pointer: Enable; 
        //              WA disparity error flag: Disable 
        RegRxPcs8G4 = SDIRXPHY_C10A10_RxPcs8G4_SET_PhaseCompRdptr(RegRxPcs8G4, 
                                                                 SDIRXPHY_ENRDPTR_ENABLE);
        RegRxPcs8G4 = SDIRXPHY_C10A10_RxPcs8G4_SET_WaDispErrFlag(RegRxPcs8G4, 
                                                            SDIRXPHY_DISPERRFLAG_DISABLE);
        // 8G Rx PCS 5: Word aligneer mode: Bit slip
        RegRxPcs8G5 = SDIRXPHY_C10A10_RxPcs8G5_SET_WaBoundaryLockCtrl(RegRxPcs8G5, 
                                                             SDIRXPHY_BOUNDLOCK_BIT_SLIP);
        // 8G Rx PCS 6: Rdclk gate: Disable; DW WrClk gate: Enable; SW WrClk gate: Disable
        RegRxPcs8G6 = SDIRXPHY_C10A10_RxPcs8G6_SET_ClockGatePcRdclk(RegRxPcs8G6, 
                                                            SDIRXPHY_PCRDCLKGATE_DISABLE);
        RegRxPcs8G6 = SDIRXPHY_C10A10_RxPcs8G6_SET_ClockGateDwPcWrclk(RegRxPcs8G6, 
                                                             SDIRXPHY_DWWRCLKGATE_ENABLE);
        RegRxPcs8G6 = SDIRXPHY_C10A10_RxPcs8G6_SET_ClockGateSwPcWrclk(RegRxPcs8G6, 
                                                            SDIRXPHY_SWWRCLKGATE_DISABLE);
        // 8G Rx PCS 7: DW WaClk gate: Enable; SW WaClk gate: Disable; BdsClk gate:Disable
        RegRxPcs8G7 = SDIRXPHY_C10A10_RxPcs8G7_SET_ClockGateDwWa(RegRxPcs8G7, 
                                                             SDIRXPHY_CLKGATEDWWA_ENABLE);
        RegRxPcs8G7 = SDIRXPHY_C10A10_RxPcs8G7_SET_ClockGateSwWa(RegRxPcs8G7, 
                                                            SDIRXPHY_CLKGATESWWA_DISABLE);
        RegRxPcs8G7 = SDIRXPHY_C10A10_RxPcs8G7_SET_ClockGateBdsDec(RegRxPcs8G7, 
                                                             SDIRXPHY_BDSCLKGATE_DISABLE);
        // 10G Rx PCS 0:GB Rx OD Width: 64; GB Rx ID Width: 64
        RegRxPcs10G0 = SDIRXPHY_C10A10_RxPcs10G0_SET_GbRxOdWidth(RegRxPcs10G0, 
                                                                 SDIRXPHY_GBRXODWIDTH_64);
        RegRxPcs10G0 = SDIRXPHY_C10A10_RxPcs10G0_SET_GbRxIdWidth(RegRxPcs10G0, 
                                                                 SDIRXPHY_GBRXIDWIDTH_64);
        // 10G Rx PCS 1:Read fifo clk enable: Disable; Write fifo clk enable: Dusable
        RegRxPcs10G1 = SDIRXPHY_C10A10_RxPcs10G1_SET_RdfifoClken(RegRxPcs10G1, 
                                                            SDIRXPHY_RDFIFOCLKEN_DISABLE);
        RegRxPcs10G1 = SDIRXPHY_C10A10_RxPcs10G1_SET_WrfifoClken(RegRxPcs10G1, 
                                                            SDIRXPHY_WRFIFOCLKEN_DISABLE);

        // 10G Rx PCS 2: GB exp clock enable: Disable
        RegRxPcs10G2 = SDIRXPHY_C10A10_RxPcs10G2_SET_GbExpClken(RegRxPcs10G2, 
                                                             SDIRXPHY_GBEXPCLKEN_DISABLE);
        // 10G Rx PCS 3: Stretch num stages: 0
        RegRxPcs10G3 = SDIRXPHY_C10A10_RxPcs10G3_SET_StretchNumStages(RegRxPcs10G3, 
                                                                      SDIRXPHY_STRETCH_0);
        // 10G Rx PCS 4: FIFO stop read: Not empty; FIFO double read: Disable
        RegRxPcs10G4 = SDIRXPHY_C10A10_RxPcs10G4_SET_FifoStopRd(RegRxPcs10G4, 
                                                             SDIRXPHY_FIFOSTOPRD_N_EMPTY);
        RegRxPcs10G4 = SDIRXPHY_C10A10_RxPcs10G4_SET_FifoDoubleRead(RegRxPcs10G4, 
                                                         SDIRXPHY_FIFODOUBLEREAD_DISABLE);
        // 10G Rx PCS 5:Phase compensator rd del: Del2
        RegRxPcs10G5 = SDIRXPHY_C10A10_RxPcs10G5_SET_PhcompRdDel(RegRxPcs10G5, 
                                                     SDIRXPHY_PHCOMPRDDEL_PHCOMP_RD_DEL2);
        // Rx PCS FIFO: Prot mode: Non 10G; Double read mode: Disable
        RegRxPcsFifo = SDIRXPHY_C10A10_RxPcsFifo_SET_ProtMode(RegRxPcsFifo, 
                                                              SDIRXPHY_PROTMODE_NON_TENG);
        RegRxPcsFifo = SDIRXPHY_C10A10_RxPcsFifo_SET_DoubleReadMode(RegRxPcsFifo, 
                                                         SDIRXPHY_DOUBLEREADMODE_DISABLE);
        // CDR PLL Settings 8: Chargepump current up: Setting4
        RegCdrPllSettings8 = SDIRXPHY_C10A10_CdrPllSettings8_SET_ChgpmpCurrentUpPd(
                                      RegCdrPllSettings8, SDIRXPHY_CPCURRENTUP_SETTING_4);
        // CDR PLL Settings 10: Chargepump current down: Setting4
        RegCdrPllSettings10 = SDIRXPHY_C10A10_CdrPllSettings10_SET_ChgpmpCurrentDnPd(
                                     RegCdrPllSettings10, SDIRXPHY_CPCURRENTDN_SETTING_4);
        // PMS Rx Deserializer: Deserialization Factor: 10
        RegPmaRxDeser = SDIRXPHY_C10A10_PmaRxDeser_SET_DeserFactor(RegPmaRxDeser, 
                                                                 SDIRXPHY_DESERFACTOR_10);
        // Equalizer bandwidth select for datarate <= 5G
        RegPmaEqBw = SDIRXPHY_C10A10_PmaEqSettings_SET_EqualizerBw(RegPmaEqBw, 
                                                                   SDIRXPHY_EQBW_EQ_BW_1);
    }
    else if (SdiRate==DT_DRV_SDIRATE_6G)
    { 
        // PCS PMA select : 8G PCS
        RegPcsPmaSel = SDIRXPHY_C10A10_PcsPmaSel_SET_PcsPmaBlockSel(RegPcsPmaSel,
                                                                SDIRXPHY_PCS_EIGHT_G_PCS);
        // PCS RX BlockSelect  8G and 8G clock out
        RegPcsRxBlockSel = SDIRXPHY_C10A10_PcsRxBlockSel_SET_RxBlockSel(RegPcsRxBlockSel,
                                                                SDIRXPHY_RXBLKSEL_EIGHTG);
        RegPcsRxBlockSel = SDIRXPHY_C10A10_PcsRxBlockSel_SET_RxClkOut(RegPcsRxBlockSel,
                                                        SDIRXPHY_RXCLKOUT_EIGHTG_CLK_OUT);
        // 8G Rx PCS 0: PMA double width: 20
        RegRxPcs8G0 = SDIRXPHY_C10A10_RxPcs8G0_SET_PmaDw(RegRxPcs8G0,  SDIRXPHY_PMADW_20);
        // 8G Rx PCS 1: Word aligner pattern length: 7 WaPldControlled: Rising edge
        RegRxPcs8G1 = SDIRXPHY_C10A10_RxPcs8G1_SET_WaPd(RegRxPcs8G1, 
                                                                   SDIRXPHY_WAPD_WA_PD_7);
        RegRxPcs8G1 = SDIRXPHY_C10A10_RxPcs8G1_SET_WaPldControlled(RegRxPcs8G1, 
                                                         SDIRXPHY_PLDCTRL_RISING_EDGE_DW);
        // 8G Rx PCS 2: 8B 10B decoder: Disable
        RegRxPcs8G2 = SDIRXPHY_C10A10_RxPcs8G2_SET_Decoder8B10B(RegRxPcs8G2, 
                                                                  SDIRXPHY_8B10B_DISABLE);
        // 8G Rx PCS 3: Byte deserializer: 2
        RegRxPcs8G3 = SDIRXPHY_C10A10_RxPcs8G3_SET_ByteDeserializer(RegRxPcs8G3, 
                                                                    SDIRXPHY_BDS_EN_BY_2);
        // 8G Rx PCS 4: Phase compensator read Pointer: Enable; 
        //              WA disparity error flag: Disable 
        RegRxPcs8G4 = SDIRXPHY_C10A10_RxPcs8G4_SET_PhaseCompRdptr(RegRxPcs8G4, 
                                                                 SDIRXPHY_ENRDPTR_ENABLE);
        RegRxPcs8G4 = SDIRXPHY_C10A10_RxPcs8G4_SET_WaDispErrFlag(RegRxPcs8G4, 
                                                             SDIRXPHY_DISPERRFLAG_DISABLE);
        // 8G Rx PCS 5: Word aligneer mode: Bit slip
        RegRxPcs8G5 = SDIRXPHY_C10A10_RxPcs8G5_SET_WaBoundaryLockCtrl(RegRxPcs8G5, 
                                                             SDIRXPHY_BOUNDLOCK_BIT_SLIP);
        // 8G Rx PCS 6: Rdclk gate: Disable; DW WrClk gate:Disable; SW WrClk gate:Disable
        RegRxPcs8G6 = SDIRXPHY_C10A10_RxPcs8G6_SET_ClockGatePcRdclk(RegRxPcs8G6, 
                                                            SDIRXPHY_PCRDCLKGATE_DISABLE);
        RegRxPcs8G6 = SDIRXPHY_C10A10_RxPcs8G6_SET_ClockGateDwPcWrclk(RegRxPcs8G6, 
                                                            SDIRXPHY_DWWRCLKGATE_DISABLE);
        RegRxPcs8G6 = SDIRXPHY_C10A10_RxPcs8G6_SET_ClockGateSwPcWrclk(RegRxPcs8G6, 
                                                            SDIRXPHY_SWWRCLKGATE_DISABLE);
        // 8G Rx PCS 7: DW WaClk gate:Disable; SW WaClk gate:Disable; BdsClk gate:Disable
        RegRxPcs8G7 = SDIRXPHY_C10A10_RxPcs8G7_SET_ClockGateDwWa(RegRxPcs8G7, 
                                                            SDIRXPHY_CLKGATEDWWA_DISABLE);
        RegRxPcs8G7 = SDIRXPHY_C10A10_RxPcs8G7_SET_ClockGateSwWa(RegRxPcs8G7, 
                                                            SDIRXPHY_CLKGATESWWA_DISABLE);
        RegRxPcs8G7 = SDIRXPHY_C10A10_RxPcs8G7_SET_ClockGateBdsDec(RegRxPcs8G7, 
                                                             SDIRXPHY_BDSCLKGATE_DISABLE);
        // 10G Rx PCS 0:GB Rx OD Width: 64; GB Rx ID Width: 64
        RegRxPcs10G0 = SDIRXPHY_C10A10_RxPcs10G0_SET_GbRxOdWidth(RegRxPcs10G0, 
                                                                 SDIRXPHY_GBRXODWIDTH_64);
        RegRxPcs10G0 = SDIRXPHY_C10A10_RxPcs10G0_SET_GbRxIdWidth(RegRxPcs10G0, 
                                                                 SDIRXPHY_GBRXIDWIDTH_64);
        // 10G Rx PCS 1:Read fifo clk enable: Disable; Write fifo clk enable: Dusable
        RegRxPcs10G1 = SDIRXPHY_C10A10_RxPcs10G1_SET_RdfifoClken(RegRxPcs10G1, 
                                                            SDIRXPHY_RDFIFOCLKEN_DISABLE);
        RegRxPcs10G1 = SDIRXPHY_C10A10_RxPcs10G1_SET_WrfifoClken(RegRxPcs10G1, 
                                                            SDIRXPHY_WRFIFOCLKEN_DISABLE);

        // 10G Rx PCS 2: GB exp clock enable: Disable
        RegRxPcs10G2 = SDIRXPHY_C10A10_RxPcs10G2_SET_GbExpClken(RegRxPcs10G2, 
                                                             SDIRXPHY_GBEXPCLKEN_DISABLE);
        // 10G Rx PCS 3: Stretch num stages: 0
        RegRxPcs10G3 = SDIRXPHY_C10A10_RxPcs10G3_SET_StretchNumStages(RegRxPcs10G3, 
                                                                      SDIRXPHY_STRETCH_0);
        // 10G Rx PCS 4: FIFO stop read: Not empty; FIFO double read: Disable
        RegRxPcs10G4 = SDIRXPHY_C10A10_RxPcs10G4_SET_FifoStopRd(RegRxPcs10G4, 
                                                             SDIRXPHY_FIFOSTOPRD_N_EMPTY);
        RegRxPcs10G4 = SDIRXPHY_C10A10_RxPcs10G4_SET_FifoDoubleRead(RegRxPcs10G4, 
                                                         SDIRXPHY_FIFODOUBLEREAD_DISABLE);
        // 10G Rx PCS 5:Phase compensator rd del: Del2
        RegRxPcs10G5 = SDIRXPHY_C10A10_RxPcs10G5_SET_PhcompRdDel(RegRxPcs10G5, 
                                                     SDIRXPHY_PHCOMPRDDEL_PHCOMP_RD_DEL2);
        // Rx PCS FIFO: Prot mode: Non 10G; Double read mode: Disable
        RegRxPcsFifo = SDIRXPHY_C10A10_RxPcsFifo_SET_ProtMode(RegRxPcsFifo, 
                                                              SDIRXPHY_PROTMODE_NON_TENG);
        RegRxPcsFifo = SDIRXPHY_C10A10_RxPcsFifo_SET_DoubleReadMode(RegRxPcsFifo, 
                                                         SDIRXPHY_DOUBLEREADMODE_DISABLE);
        // CDR PLL Settings 8: Chargepump current up: Setting4
        RegCdrPllSettings8 = SDIRXPHY_C10A10_CdrPllSettings8_SET_ChgpmpCurrentUpPd(
                                      RegCdrPllSettings8, SDIRXPHY_CPCURRENTUP_SETTING_4);
        // CDR PLL Settings 10: Chargepump current down: Setting4
        RegCdrPllSettings10 = SDIRXPHY_C10A10_CdrPllSettings10_SET_ChgpmpCurrentDnPd(
                                     RegCdrPllSettings10, SDIRXPHY_CPCURRENTDN_SETTING_4);
        // PMS Rx Deserializer: Deserialization Factor: 20
        RegPmaRxDeser = SDIRXPHY_C10A10_PmaRxDeser_SET_DeserFactor(RegPmaRxDeser, 
                                                                 SDIRXPHY_DESERFACTOR_20);
        // Equalizer bandwidth select for datarate <= 10G
        RegPmaEqBw = SDIRXPHY_C10A10_PmaEqSettings_SET_EqualizerBw(RegPmaEqBw, 
                                                                   SDIRXPHY_EQBW_EQ_BW_2);
    }
    else if (SdiRate==DT_DRV_SDIRATE_12G) 
    { 
        // PCS PMA select : 10G PCS
        RegPcsPmaSel = SDIRXPHY_C10A10_PcsPmaSel_SET_PcsPmaBlockSel(RegPcsPmaSel,
                                                                  SDIRXPHY_PCS_TEN_G_PCS);
        // PCS RX BlockSelect  10G and 10G clock out
        RegPcsRxBlockSel = SDIRXPHY_C10A10_PcsRxBlockSel_SET_RxBlockSel(RegPcsRxBlockSel,
                                                                  SDIRXPHY_RXBLKSEL_TENG);
        RegPcsRxBlockSel = SDIRXPHY_C10A10_PcsRxBlockSel_SET_RxClkOut(RegPcsRxBlockSel,
                                                          SDIRXPHY_RXCLKOUT_TENG_CLK_OUT);
        // 8G Rx PCS 0: PMA double width: 10
        RegRxPcs8G0 = SDIRXPHY_C10A10_RxPcs8G0_SET_PmaDw(RegRxPcs8G0,  SDIRXPHY_PMADW_10);
        // 8G Rx PCS 1: Word aligner pat. length: 10; WaPldControlled: Disabled(=Rising)
        RegRxPcs8G1 = SDIRXPHY_C10A10_RxPcs8G1_SET_WaPd(RegRxPcs8G1, 
                                                                  SDIRXPHY_WAPD_WA_PD_10);
        RegRxPcs8G1 = SDIRXPHY_C10A10_RxPcs8G1_SET_WaPldControlled(RegRxPcs8G1, 
                                                         SDIRXPHY_PLDCTRL_RISING_EDGE_DW);
        // 8G Rx PCS 2: 8B 10B decoder: Enable IBM
        RegRxPcs8G2 = SDIRXPHY_C10A10_RxPcs8G2_SET_Decoder8B10B(RegRxPcs8G2, 
                                                               SDIRXPHY_8B10B_ENABLE_IBM);
        // 8G Rx PCS 3: Byte deserializer: Disabled
        RegRxPcs8G3 = SDIRXPHY_C10A10_RxPcs8G3_SET_ByteDeserializer(RegRxPcs8G3, 
                                                                    SDIRXPHY_BDS_DISABLE);
        // 8G Rx PCS 4: Phase compensator read Pointer: Disabled; 
        //              WA disparity error flag: Enabled 
        RegRxPcs8G4 = SDIRXPHY_C10A10_RxPcs8G4_SET_PhaseCompRdptr(RegRxPcs8G4, 
                                                                SDIRXPHY_ENRDPTR_DISABLE);
        RegRxPcs8G4 = SDIRXPHY_C10A10_RxPcs8G4_SET_WaDispErrFlag(RegRxPcs8G4, 
                                                             SDIRXPHY_DISPERRFLAG_ENABLE);
        // 8G Rx PCS 5: Word aligneer mode: Sync SM
        RegRxPcs8G5 = SDIRXPHY_C10A10_RxPcs8G5_SET_WaBoundaryLockCtrl(RegRxPcs8G5, 
                                                              SDIRXPHY_BOUNDLOCK_SYNC_SM);
        // 8G Rx PCS 6: Rdclk gate: Enable; DW WrClk gate: Enable; SW WrClk gate: Enable
        RegRxPcs8G6 = SDIRXPHY_C10A10_RxPcs8G6_SET_ClockGatePcRdclk(RegRxPcs8G6, 
                                                             SDIRXPHY_PCRDCLKGATE_ENABLE);
        RegRxPcs8G6 = SDIRXPHY_C10A10_RxPcs8G6_SET_ClockGateDwPcWrclk(RegRxPcs8G6, 
                                                             SDIRXPHY_DWWRCLKGATE_ENABLE);
        RegRxPcs8G6 = SDIRXPHY_C10A10_RxPcs8G6_SET_ClockGateSwPcWrclk(RegRxPcs8G6, 
                                                             SDIRXPHY_SWWRCLKGATE_ENABLE);
        // 8G Rx PCS 7: DW WaClk gate: Enable; SW WaClk gate: Enable; BdsClk gate: Enable
        RegRxPcs8G7 = SDIRXPHY_C10A10_RxPcs8G7_SET_ClockGateDwWa(RegRxPcs8G7, 
                                                             SDIRXPHY_CLKGATEDWWA_ENABLE);
        RegRxPcs8G7 = SDIRXPHY_C10A10_RxPcs8G7_SET_ClockGateSwWa(RegRxPcs8G7, 
                                                             SDIRXPHY_CLKGATESWWA_ENABLE);
        RegRxPcs8G7 = SDIRXPHY_C10A10_RxPcs8G7_SET_ClockGateBdsDec(RegRxPcs8G7, 
                                                              SDIRXPHY_BDSCLKGATE_ENABLE);
        // 10G Rx PCS 0:GB Rx OD Width: 40; GB Rx ID Width: 40
        RegRxPcs10G0 = SDIRXPHY_C10A10_RxPcs10G0_SET_GbRxOdWidth(RegRxPcs10G0, 
                                                                 SDIRXPHY_GBRXODWIDTH_40);
        RegRxPcs10G0 = SDIRXPHY_C10A10_RxPcs10G0_SET_GbRxIdWidth(RegRxPcs10G0, 
                                                                 SDIRXPHY_GBRXIDWIDTH_40);
        // 10G Rx PCS 1:Read fifo clk enable: Enable; Write fifo clk enable: Enable
        RegRxPcs10G1 = SDIRXPHY_C10A10_RxPcs10G1_SET_RdfifoClken(RegRxPcs10G1, 
                                                             SDIRXPHY_RDFIFOCLKEN_ENABLE);
        RegRxPcs10G1 = SDIRXPHY_C10A10_RxPcs10G1_SET_WrfifoClken(RegRxPcs10G1, 
                                                             SDIRXPHY_WRFIFOCLKEN_ENABLE);

        // 10G Rx PCS 2: GB exp clock enable: Enable
        RegRxPcs10G2 = SDIRXPHY_C10A10_RxPcs10G2_SET_GbExpClken(RegRxPcs10G2, 
                                                              SDIRXPHY_GBEXPCLKEN_ENABLE);
        // 10G Rx PCS 3: Stretch num stages: 2
        RegRxPcs10G3 = SDIRXPHY_C10A10_RxPcs10G3_SET_StretchNumStages(RegRxPcs10G3, 
                                                                      SDIRXPHY_STRETCH_2);
        // 10G Rx PCS 4: FIFO stop read: Empty; FIFO double read: Enable
        RegRxPcs10G4 = SDIRXPHY_C10A10_RxPcs10G4_SET_FifoStopRd(RegRxPcs10G4, 
                                                               SDIRXPHY_FIFOSTOPRD_EMPTY);
        RegRxPcs10G4 = SDIRXPHY_C10A10_RxPcs10G4_SET_FifoDoubleRead(RegRxPcs10G4, 
                                                          SDIRXPHY_FIFODOUBLEREAD_ENABLE);
        // 10G Rx PCS 5:Phase compensator rd del: Del3
        RegRxPcs10G5 = SDIRXPHY_C10A10_RxPcs10G5_SET_PhcompRdDel(RegRxPcs10G5, 
                                                     SDIRXPHY_PHCOMPRDDEL_PHCOMP_RD_DEL3);
        // Rx PCS FIFO: Prot mode: 10G; Double read mode: Enable
        RegRxPcsFifo = SDIRXPHY_C10A10_RxPcsFifo_SET_ProtMode(RegRxPcsFifo, 
                                                                  SDIRXPHY_PROTMODE_TENG);
        RegRxPcsFifo = SDIRXPHY_C10A10_RxPcsFifo_SET_DoubleReadMode(RegRxPcsFifo, 
                                                          SDIRXPHY_DOUBLEREADMODE_ENABLE);
        // CDR PLL Settings 8: Chargepump current up: Setting3
        RegCdrPllSettings8 = SDIRXPHY_C10A10_CdrPllSettings8_SET_ChgpmpCurrentUpPd(
                                      RegCdrPllSettings8, SDIRXPHY_CPCURRENTUP_SETTING_3);
        // CDR PLL Settings 10: Chargepump current down: Setting3
        RegCdrPllSettings10 = SDIRXPHY_C10A10_CdrPllSettings10_SET_ChgpmpCurrentDnPd(
                                     RegCdrPllSettings10, SDIRXPHY_CPCURRENTDN_SETTING_3);
        // PMS Rx Deserializer: Deserialization Factor: 40
        RegPmaRxDeser = SDIRXPHY_C10A10_PmaRxDeser_SET_DeserFactor(RegPmaRxDeser, 
                                                                 SDIRXPHY_DESERFACTOR_40);
        // Equalizer bandwidth select for datarate <= 20G
        RegPmaEqBw = SDIRXPHY_C10A10_PmaEqSettings_SET_EqualizerBw(RegPmaEqBw, 
                                                                   SDIRXPHY_EQBW_EQ_BW_3);
    }
    else
    {
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Write registres
    SDIRXPHY_C10A10_CdrPllSettings4_WRITE(pBc, RegCdrPllSettings4);
    SDIRXPHY_C10A10_CdrPllSettings9_WRITE(pBc, RegCdrPllSettings9);
    SDIRXPHY_C10A10_PcsPmaSel_WRITE(pBc, RegPcsPmaSel);
    SDIRXPHY_C10A10_PcsRxBlockSel_WRITE(pBc, RegPcsRxBlockSel);
    SDIRXPHY_C10A10_RxPcs8G0_WRITE(pBc, RegRxPcs8G0);
    SDIRXPHY_C10A10_RxPcs8G1_WRITE(pBc, RegRxPcs8G1);
    SDIRXPHY_C10A10_RxPcs8G2_WRITE(pBc, RegRxPcs8G2);
    SDIRXPHY_C10A10_RxPcs8G3_WRITE(pBc, RegRxPcs8G3);
    SDIRXPHY_C10A10_RxPcs8G4_WRITE(pBc, RegRxPcs8G4);
    SDIRXPHY_C10A10_RxPcs8G5_WRITE(pBc, RegRxPcs8G5);
    SDIRXPHY_C10A10_RxPcs8G6_WRITE(pBc, RegRxPcs8G6);
    SDIRXPHY_C10A10_RxPcs8G7_WRITE(pBc, RegRxPcs8G7);
    SDIRXPHY_C10A10_RxPcs10G0_WRITE(pBc, RegRxPcs10G0);
    SDIRXPHY_C10A10_RxPcs10G1_WRITE(pBc, RegRxPcs10G1);
    SDIRXPHY_C10A10_RxPcs10G2_WRITE(pBc, RegRxPcs10G2);
    SDIRXPHY_C10A10_RxPcs10G3_WRITE(pBc, RegRxPcs10G3);
    SDIRXPHY_C10A10_RxPcs10G4_WRITE(pBc, RegRxPcs10G4);
    SDIRXPHY_C10A10_RxPcs10G5_WRITE(pBc, RegRxPcs10G5);
    SDIRXPHY_C10A10_RxPcsFifo_WRITE(pBc, RegRxPcsFifo);
    SDIRXPHY_C10A10_CdrPllSettings8_WRITE(pBc, RegCdrPllSettings8);
    SDIRXPHY_C10A10_CdrPllSettings10_WRITE(pBc, RegCdrPllSettings10);
    SDIRXPHY_C10A10_PmaRxDeser_WRITE(pBc, RegPmaRxDeser);
    SDIRXPHY_C10A10_PmaEqSettings_WRITE(pBc, RegPmaEqBw);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSDIRXPHY_SetControlRegs(DtBcSDIRXPHY* pBc, Bool BlkEnable, Int OpMode,
                                        Bool RxClkReset, Bool PllReset, Bool XcvrReset,
                                        Int LockMode, Bool DownsamplerEnable, Int SdiRate)
{
    UInt RegData=0, FldOpMode=0, FldBlkEnable=0, FldLockMode=0, FldRxClkReset=0,
         FldXcvrReset=0, FldSdiRate=0;
    // Convert block enable to BB-type
    FldBlkEnable = BlkEnable ? SDIRXPHY_BLKENA_ENABLED : SDIRXPHY_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FldOpMode = SDIRXPHY_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FldOpMode = SDIRXPHY_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FldOpMode = SDIRXPHY_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }
    // Convert lockmode to BB-type
    switch (LockMode)
    {
    case DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_DATA:
        FldLockMode= SDIRXPHY_LOCKMODE_LOCK_TO_DATA;
        break;
    case DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_REF:
        FldLockMode= SDIRXPHY_LOCKMODE_LOCK_TO_REF;
        break;
    default: DT_ASSERT(FALSE);
    }

    // Convert clock reset
    FldRxClkReset = RxClkReset ?  1 : 0;

    // Convert transceiver reset
    FldXcvrReset = XcvrReset ?  1 : 0;

    // Convert SDI rate
    switch (SdiRate)
    {
    case DT_DRV_SDIRATE_SD:   FldSdiRate = SDIRXPHY_SDIMODE_SD; break;
    case DT_DRV_SDIRATE_HD:   FldSdiRate = SDIRXPHY_SDIMODE_HD; break;
    case DT_DRV_SDIRATE_3G:   FldSdiRate = SDIRXPHY_SDIMODE_3G; break;
    case DT_DRV_SDIRATE_6G:   FldSdiRate = SDIRXPHY_SDIMODE_6G; break;
    case DT_DRV_SDIRATE_12G:  FldSdiRate = SDIRXPHY_SDIMODE_12G; break;
    default: DT_ASSERT(FALSE);
    }

    // Invalid combination?
    DT_ASSERT(BlkEnable || OpMode==DT_BLOCK_OPMODE_IDLE);

    // Update SDIRXPHY control register
    RegData = SDIRXPHY_Control_READ(pBc);
    RegData = SDIRXPHY_Control_SET_BlockEnable(RegData, FldBlkEnable);
    RegData = SDIRXPHY_Control_SET_OperationalMode(RegData, FldOpMode);
    RegData = SDIRXPHY_Control_SET_RxClkReset(RegData, FldRxClkReset);
    RegData = SDIRXPHY_Control_SET_LockMode(RegData, FldLockMode);
    RegData = SDIRXPHY_Control_SET_PllReset(RegData, PllReset ? 1 : 0);
    RegData = SDIRXPHY_Control_SET_SrcFactor(RegData, DownsamplerEnable ? 
                                          SDIRXPHY_SDIMODE_x11 : SDIRXPHY_SDIMODE_BYPASS);
    RegData = SDIRXPHY_Control_SET_SdiRate(RegData, FldSdiRate);
    RegData = SDIRXPHY_Control_SET_XcvrReset(RegData, FldXcvrReset);
    SDIRXPHY_Control_WRITE(BC_SDIRXPHY, RegData);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_C10A10_RequestAccess -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_C10A10_RequestAccess(DtBcSDIRXPHY* pBc, Bool AllowLongSleep)
{
    UInt32  RegData=0, TimeoutCount=0;
    Bool AccessAllowed;
    // Request access to PHY registers.
    RegData = SDIRXPHY_C10A10_ArbitrationCtrl_SET_ArbiterCtrlPma(0, 
                                                                  SDIRXPHY_ARBOWNER_User);
    RegData = SDIRXPHY_C10A10_ArbitrationCtrl_SET_CalDonePma(RegData, 
                                                                  SDIRXPHY_CALDONE_Done);
    SDIRXPHY_C10A10_ArbitrationCtrl_WRITE(pBc, RegData);

    // Wait for 20ms to get access to PHY registers.
    AccessAllowed = DtBcSDIRXPHY_C10A10_IsAccessAllowed(pBc);
    TimeoutCount = 20000;
    while (!AccessAllowed && TimeoutCount>0)
    {
        DtWaitBlock(1);
        TimeoutCount--;
        AccessAllowed = DtBcSDIRXPHY_C10A10_IsAccessAllowed(pBc);
    }
    if (AccessAllowed)
        return DT_STATUS_OK;
    if (!AllowLongSleep)
        return DT_STATUS_TIMEOUT;

    // It takes a bit longer. Use a sleep if allowed
    TimeoutCount = 1000;    // Maximum 10 seconds
    while (!AccessAllowed && TimeoutCount>0)
    {
        DtSleep(10);
        TimeoutCount--;
        AccessAllowed = DtBcSDIRXPHY_C10A10_IsAccessAllowed(pBc);
    }
    if (!AccessAllowed)
    {
        // Timeout
        DtString  Str;
        DtStringAlloc(&Str, 256);

        DtStringAppendChars(&Str, "[SN=");
        DtStringUInt64ToDtStringAppend(&Str, 10, pBc->m_pCore->m_pDevInfo->m_Serial);
        DtStringAppendChars(&Str, "] ");
        DtStringAppendChars(&Str, "SDI RX PHY RequestAccess has timed out");
        DtEvtLogReport(&pBc->m_pCore->m_Device.m_EvtObject, 
                                              DTPCIE_LOG_ERROR_GENERIC, &Str, NULL, NULL);

        DtStringFree(&Str);
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "Wait request timeout");
        return DT_STATUS_TIMEOUT;
    } else {
        DtString  Str;
        DtStringAlloc(&Str, 256);

        DtStringAppendChars(&Str, "[SN=");
        DtStringUInt64ToDtStringAppend(&Str, 10, pBc->m_pCore->m_pDevInfo->m_Serial);
        DtStringAppendChars(&Str, "] ");
        DtStringAppendChars(&Str, "SDI RX PHY RequestAcces granted in: ");
        DtStringUInt64ToDtStringAppend(&Str, 10, (UInt32)(1000 - TimeoutCount)*10);
        DtStringAppendChars(&Str, "ms");

        DtEvtLogReport(&pBc->m_pCore->m_Device.m_EvtObject, 
                                              DTPCIE_LOG_INFO_GENERIC, &Str, NULL, NULL);
        DtStringFree(&Str);
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_C10A10_ReleaseAccess -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtBcSDIRXPHY_C10A10_ReleaseAccess(DtBcSDIRXPHY* pBc, Bool TriggerCalibration)
{
    // Write SDIRXPHY_CALDONE_Done to avoid triggering calibration.
    // Write SDIRXPHY_CALDONE_Busy to trigger calibration.
    UInt32 CalDone = TriggerCalibration ? SDIRXPHY_CALDONE_Busy : SDIRXPHY_CALDONE_Done;

    // Release configuration bus to PreSICE to perform calibration
    UInt32 RegData = SDIRXPHY_C10A10_ArbitrationCtrl_SET_ArbiterCtrlPma(0, 
                                                               SDIRXPHY_ARBOWNER_PreSICE);
    RegData = SDIRXPHY_C10A10_ArbitrationCtrl_SET_CalDonePma(RegData, CalDone);
    SDIRXPHY_C10A10_ArbitrationCtrl_WRITE(pBc, RegData);
}
