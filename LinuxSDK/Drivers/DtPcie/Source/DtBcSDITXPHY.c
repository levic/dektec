//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDITXPHY.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
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
#include "DtBcSDITXPHY.h"
#include "DtBcSDITXPHY_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXPHY implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDITXPHY))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_SDITXPHY_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(SDITXPHY, pBc)

// Helper macro to cast a DtBc* to a DtBcSDITXPHY*
#define BC_SDITXPHY         ((DtBcSDITXPHY*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcSDITXPHY_Init(DtBc*);
static DtStatus  DtBcSDITXPHY_OnEnable(DtBc*, Bool);
static DtStatus  DtBcSDITXPHY_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcSDITXPHY_CheckSdiRate(DtBcSDITXPHY*, Int SdiRate);
static void  DtBcSDITXPHY_SetControlRegs(DtBcSDITXPHY*, Bool BlkEnable, Int OpMode,
                                  Bool TxClkReset, Bool Arm, Int SofDelay, Int SrcFactor);
static DtStatus DtBcSDITXPHY_C10A10_SetPllSelect(DtBcSDITXPHY*, Int ClockIdx);
static void  DtBcSDITXPHY_SetSlewRateControl(DtBcSDITXPHY*, Int SdiRate);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXPHY - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSDITXPHY_Close(DtBc*  pBc)
{
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcSDITXPHY*  DtBcSDITXPHY_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_SDITXPHY_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcSDITXPHY, Id, DT_BLOCK_TYPE_SDITXPHY,
                                                              Address, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcSDITXPHY_Close;
    OpenParams.m_InitFunc = DtBcSDITXPHY_Init;
    OpenParams.m_OnEnableFunc = DtBcSDITXPHY_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcSDITXPHY_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcSDITXPHY*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_ArmForSof -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXPHY_ArmForSof(DtBcSDITXPHY * pBc)
{
     // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);
    
    // Must be supported
    if (!pBc->m_SupportsStartOnSof)
    { 
        DtDbgOutBc(ERR, SDITXPHY, pBc, "Arm for Sof not supported");
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Operational mode must be STANDBY
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_STANDBY)
    { 
        DtDbgOutBc(ERR, SDITXPHY, pBc, "Operational mode not in standby");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // Make settings in register
    DtBcSDITXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                  pBc->m_ClockReset, TRUE, 
                                                  pBc->m_SofDelay, pBc->m_UpsampleFactor);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_ClearUnderflowFlag -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXPHY_ClearUnderflowFlag(DtBcSDITXPHY*  pBc)
{
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);

    // Clear the overflow flag
    SDITXPHY_CdcFifoStatus_CLEAR_Underflow(pBc);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_GetArmStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXPHY_GetArmStatus(DtBcSDITXPHY* pBc, Int* pArmStatus)
{
    UInt32  FwArmStatus;

    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pArmStatus == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);

    // Read ARM status
    FwArmStatus = SDITXPHY_Status_READ_ArmStatus(pBc);
    switch (FwArmStatus)
    {
    case SDITXPHY_ARMSTATUS_IDLE:
        *pArmStatus = DT_BC_SDITXPHY_ARMSTATUS_IDLE;
        break;
    case SDITXPHY_ARMSTATUS_PREARMED:
        *pArmStatus = DT_BC_SDITXPHY_ARMSTATUS_PREARMED;
        break;
    case SDITXPHY_ARMSTATUS_ARMED:
        *pArmStatus = DT_BC_SDITXPHY_ARMSTATUS_ARMED;
        break;
    default: DT_ASSERT(FALSE); return DT_STATUS_FAIL;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_GetClockReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXPHY_GetClockReset(DtBcSDITXPHY * pBc, Bool* pClkReset)
{
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pClkReset == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);

    // Return last cached clock reset
    *pClkReset = pBc->m_ClockReset;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_GetDeviceFamily -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXPHY_GetDeviceFamily(DtBcSDITXPHY * pBc, Int* pDeviceFamily)
{
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pDeviceFamily == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);

    // Return cached device family
    *pDeviceFamily = pBc->m_DeviceFamily;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_GetMaxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXPHY_GetMaxSdiRate(DtBcSDITXPHY * pBc, Int * pMaxSdiRate)
{
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pMaxSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);

    // Return cached SDI-rate
    *pMaxSdiRate = pBc->m_MaxSdiRate;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_GetNumClocks -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXPHY_GetNumClocks(DtBcSDITXPHY* pBc, Int* pNumClocks)
{
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pNumClocks == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);

    // Return last cached number of clocks
    *pNumClocks = pBc->m_NumClocks;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_GetOperationalStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXPHY_GetOperationalStatus(DtBcSDITXPHY* pBc, Int* pOpStatus)
{
    UInt32  FwOpStatus;

    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpStatus == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);

    // Read operational status
    FwOpStatus = SDITXPHY_Status_READ_OperationalStatus(pBc);
    switch (FwOpStatus)
    {
    case SDITXPHY_OPSTATUS_IDLE:    *pOpStatus = DT_BLOCK_OPSTATUS_IDLE; break;
    case SDITXPHY_OPSTATUS_STANDBY: *pOpStatus = DT_BLOCK_OPSTATUS_STANDBY; break;
    case SDITXPHY_OPSTATUS_RUN:     *pOpStatus = DT_BLOCK_OPSTATUS_RUN; break;
    case SDITXPHY_OPSTATUS_ERROR:   *pOpStatus = DT_BLOCK_OPSTATUS_ERROR; break;
    default: DT_ASSERT(FALSE); return DT_STATUS_FAIL;
    }

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_GetTxPllId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXPHY_GetTxPllId(DtBcSDITXPHY* pBc, Int* pTxPllId)
{
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pTxPllId == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);

    // Return the ID of the TXPLL that is currently used
    if (pBc->m_NumClocks == 1)
        *pTxPllId = pBc->m_PllIdClk1;
    else
    { 
        DT_ASSERT(pBc->m_DeviceFamily==DT_BC_SDITXPHY_FAMILY_A10 
                                       || pBc->m_DeviceFamily==DT_BC_SDITXPHY_FAMILY_C10);
        *pTxPllId = pBc->m_C10A10FractClock ? pBc->m_PllIdClk2 : pBc->m_PllIdClk1;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_GetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXPHY_GetSdiRate(DtBcSDITXPHY* pBc, Int* pSdiRate)
{
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);
    // Check parameters
    if (pSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);
    // Return cached SDI-rate
    *pSdiRate = pBc->m_SdiRate;
    return DT_STATUS_OK;
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_GetSofDelay -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXPHY_GetSofDelay(DtBcSDITXPHY* pBc, Int* pSofDelay)
{
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pSofDelay == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);

    // Return cached SoF delay
    *pSofDelay = pBc->m_SofDelay;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_GetUnderflowFlag -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXPHY_GetUnderflowFlag(DtBcSDITXPHY*  pBc, Bool*  pUnderflow)
{
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pUnderflow == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);

    // Return the underflow flag
    *pUnderflow =  (SDITXPHY_CdcFifoStatus_READ_Underflow(pBc) != 0);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_IsResetInProgress -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXPHY_IsResetInProgress(DtBcSDITXPHY*  pBc, Bool*  pInProgress)
{
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pInProgress == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);

    // Return the reset in progress flag
    *pInProgress =  (SDITXPHY_Status_READ_ResetInProgress(pBc) != 0);

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_SetClockReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXPHY_SetClockReset(DtBcSDITXPHY* pBc, Bool ClkReset)
{
     // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);
    
    // Operational mode must be IDLE or STANDBY
    if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDITXPHY, pBc, "Operational mode not in idle/standby");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // No change?
    if (pBc->m_ClockReset == ClkReset)
        return DT_STATUS_OK;

    // Update cache
    pBc->m_ClockReset = ClkReset;

    // Make settings in register
    DtBcSDITXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                  pBc->m_ClockReset, FALSE, 
                                                  pBc->m_SofDelay, pBc->m_UpsampleFactor);

    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_C10A10_GetCalibrationDone -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXPHY_C10A10_GetCalibrationDone(DtBcSDITXPHY* pBc, Bool* pDone)
{
    UInt32  RegCalBusy=0;

    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);
    
    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);
    
    // Check device family
    if (pBc->m_DeviceFamily!=DT_BC_SDITXPHY_FAMILY_A10 
                                        && pBc->m_DeviceFamily!=DT_BC_SDITXPHY_FAMILY_C10)
    { 
        DtDbgOutBc(ERR, SDITXPHY, pBc, "ERROR: Not supported for this device family");
        return DT_STATUS_NOT_SUPPORTED;
    }

    // No calibration needed before PHY version 2
    if (pBc->m_Version < 2)
    { 
        *pDone = TRUE;
        return DT_STATUS_OK;
    }

    // Wait until CalDone is asserted
    RegCalBusy = SDITXPHY_Status_READ_CalBusy(pBc);
    *pDone = (RegCalBusy == 0);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_C10A10_GetSdiFractionalClock -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXPHY_C10A10_GetSdiFractionalClock(DtBcSDITXPHY* pBc, Bool* pFracClk)
{
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pFracClk == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);
    
    // Check device family
    if (pBc->m_DeviceFamily!=DT_BC_SDITXPHY_FAMILY_A10 
                                        && pBc->m_DeviceFamily!=DT_BC_SDITXPHY_FAMILY_C10)
    { 
        DtDbgOutBc(ERR, SDITXPHY, pBc, "ERROR: Not supported for this device family");
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Return cached value
    *pFracClk = pBc->m_C10A10FractClock;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_C10A10_SetSdiFractionalClock -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXPHY_C10A10_SetSdiFractionalClock(DtBcSDITXPHY* pBc, Bool FracClk)
{
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);
    
    // Check device family
    if (pBc->m_DeviceFamily!=DT_BC_SDITXPHY_FAMILY_A10 
                                        && pBc->m_DeviceFamily!=DT_BC_SDITXPHY_FAMILY_C10)
    { 
        DtDbgOutBc(ERR, SDITXPHY, pBc, "ERROR: Not supported for this device family");
        return DT_STATUS_NOT_SUPPORTED;
    }

    // No change?
    if (pBc->m_C10A10FractClock == FracClk)
        return DT_STATUS_OK;

    // Make PLL selection
    DT_RETURN_ON_ERROR(DtBcSDITXPHY_C10A10_SetPllSelect(pBc, FracClk ? 1 : 0));

    // Update cached value
    pBc->m_C10A10FractClock = FracClk;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_C10A10_StartCalibration -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXPHY_C10A10_StartCalibration(DtBcSDITXPHY* pBc)
{
    UInt32  RegData=0,  WaitRequest=0, TimeoutCount=0;
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);
    
    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);
    
    // Check device family
    if (pBc->m_DeviceFamily!=DT_BC_SDITXPHY_FAMILY_A10 
                                        && pBc->m_DeviceFamily!=DT_BC_SDITXPHY_FAMILY_C10)
    { 
        DtDbgOutBc(ERR, SDITXPHY, pBc, "ERROR: Not supported for this device family");
        return DT_STATUS_NOT_SUPPORTED;
    }

    // No calibration needed before PHY version 2
    if (pBc->m_Version < 2)
        return DT_STATUS_OK;

    // Clock reset should be asserted
    DT_ASSERT(pBc->m_ClockReset == TRUE);

    // Request access to calibration registers.
    RegData = SDITXPHY_C10A10_ArbitrationCtrl_READ(pBc);
    RegData = SDITXPHY_C10A10_ArbitrationCtrl_SET_ArbiterCtrlPma(RegData, 
                                                                  SDITXPHY_ARBOWNER_User);
    RegData = SDITXPHY_C10A10_ArbitrationCtrl_SET_CalDonePma(RegData, 
                                                                  SDITXPHY_CALDONE_Done);
    SDITXPHY_C10A10_ArbitrationCtrl_WRITE(pBc, RegData);

    // Wait for access to calibration registers.
    WaitRequest = SDITXPHY_Status_READ_WaitRequest(pBc);
    TimeoutCount = 100;
    while (WaitRequest!=0 && TimeoutCount>0)
    {
        DtWaitBlock(1);
        TimeoutCount--;
        WaitRequest = SDITXPHY_Status_READ_WaitRequest(pBc);
    }
    DT_ASSERT(WaitRequest == 0);
    if (WaitRequest != 0)
        DtDbgOutBc(ERR, SDITXPHY, pBc, "Wait request timeout");


    // CalEnable.TxCalEn = 1; CalEnable.RxCalEn = 0; CalEnable.AdaptEn = 0;
    RegData = SDITXPHY_C10A10_CalEnable_READ(pBc);
    RegData = SDITXPHY_C10A10_CalEnable_SET_RxCalEn(RegData, SDITXPHY_CALEN_Disable);
    RegData = SDITXPHY_C10A10_CalEnable_SET_TxCalEn(RegData, SDITXPHY_CALEN_Enable);
    RegData = SDITXPHY_C10A10_CalEnable_SET_AdaptEn(RegData, SDITXPHY_CALEN_Disable);
    SDITXPHY_C10A10_CalEnable_WRITE(pBc, RegData);

    // Release configuration bus to PreSICE to perform calibration
    RegData = SDITXPHY_C10A10_ArbitrationCtrl_READ(pBc);
    RegData = SDITXPHY_C10A10_ArbitrationCtrl_SET_ArbiterCtrlPma(RegData, 
                                                               SDITXPHY_ARBOWNER_PreSICE);
    RegData = SDITXPHY_C10A10_ArbitrationCtrl_SET_CalDonePma(RegData, 
                                                                   SDITXPHY_CALDONE_Busy);
    SDITXPHY_C10A10_ArbitrationCtrl_WRITE(pBc, RegData);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXPHY_SetOperationalMode(DtBcSDITXPHY * pBc, Int OpMode)
{
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY 
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDITXPHY, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Clear overflow before going to run
    if (OpMode == DT_BLOCK_OPMODE_RUN)
        SDITXPHY_CdcFifoStatus_CLEAR_Underflow(pBc);

    // Save operational mode
    pBc->m_OperationalMode = OpMode;

    // Make setting in control register
    DtBcSDITXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                  pBc->m_ClockReset, FALSE, 
                                                  pBc->m_SofDelay, pBc->m_UpsampleFactor);
    return DT_STATUS_OK;

}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_SetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXPHY_SetSdiRate(DtBcSDITXPHY* pBc, Int SdiRate)
{
    UInt32  SrcFactor = 0;
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);
    // Check parameters
    DT_RETURN_ON_ERROR(DtBcSDITXPHY_CheckSdiRate(pBc, SdiRate));
    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);

    // Determine upsampling factor
    if (pBc->m_MaxSdiRate <= DT_DRV_SDIRATE_3G)
    {
        switch (SdiRate)
        {
        case  DT_DRV_SDIRATE_SD: SrcFactor = 11; break;
        case  DT_DRV_SDIRATE_HD: SrcFactor = 2; break;
        case  DT_DRV_SDIRATE_3G: SrcFactor = 1; break;
        default: DT_ASSERT(FALSE);
        }
    } else {
        switch (SdiRate)
        {
        case  DT_DRV_SDIRATE_SD:  SrcFactor = 44; break;
        case  DT_DRV_SDIRATE_HD:  SrcFactor = 8; break;
        case  DT_DRV_SDIRATE_3G:  SrcFactor = 4; break;
        case  DT_DRV_SDIRATE_6G:  SrcFactor = 2; break;
        case  DT_DRV_SDIRATE_12G: SrcFactor = 1; break;
        default: DT_ASSERT(FALSE);
        }
    }

    // No change?
    if (pBc->m_SdiRate==SdiRate && pBc->m_UpsampleFactor==SrcFactor)
        return DT_STATUS_OK;

    // Update cache
    pBc->m_UpsampleFactor = SrcFactor;
    pBc->m_SdiRate = SdiRate;

    // Make settings in register
    DtBcSDITXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                  pBc->m_ClockReset, FALSE, 
                                                  pBc->m_SofDelay, pBc->m_UpsampleFactor);
    // Update slew rate
    DtBcSDITXPHY_SetSlewRateControl(pBc, SdiRate);

    return DT_STATUS_OK;
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_SetSofDelay -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXPHY_SetSofDelay(DtBcSDITXPHY* pBc, Int SofDelay)
{
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (SofDelay<0 || SofDelay>=(1<<16))
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXPHY_MUST_BE_ENABLED(pBc);
    
    // Operational mode must be IDLE/STANDBY
    if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDITXPHY, pBc, "Operational mode not in idle/standby");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // No change?
    if (pBc->m_SofDelay == SofDelay)
        return DT_STATUS_OK;

    // Update cache
    pBc->m_SofDelay = SofDelay;

    // Make settings in register
    DtBcSDITXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                  pBc->m_ClockReset, FALSE, 
                                                  pBc->m_SofDelay, pBc->m_UpsampleFactor);
    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXPHY - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSDITXPHY_Init(DtBc*  pBcBase)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcSDITXPHY* pBc = (DtBcSDITXPHY*)pBcBase;
    UInt32  FwMaxSdiRate, FwDeviceFamily;

    // Sanity checks
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Clear underflow
    SDITXPHY_CdcFifoStatus_CLEAR_Underflow(pBc);

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_ClockReset = TRUE;
    pBc->m_SofDelay = 0;
    pBc->m_C10A10FractClock = FALSE;

    // Start-on-SoF is supported from version 1
    pBc->m_SupportsStartOnSof = pBc->m_Version>=1;

    // Get the number of clocks from the config register
    pBc->m_NumClocks = SDITXPHY_Config_READ_NumClocks(pBc);

    // Get PLL-IDs
    pBc->m_PllIdClk1 = SDITXPHY_Config_READ_PllIdClk1(pBc);
    pBc->m_PllIdClk2 = SDITXPHY_Config_READ_PllIdClk2(pBc);

    // Get device family
    FwDeviceFamily = SDITXPHY_Config_READ_DeviceFamily(pBc);
    switch (FwDeviceFamily)
    {
    case SDITXPHY_FAMILY_A10:  pBc->m_DeviceFamily = DT_BC_SDITXPHY_FAMILY_A10; break;
    case SDITXPHY_FAMILY_C10:  pBc->m_DeviceFamily = DT_BC_SDITXPHY_FAMILY_C10; break;
    case SDITXPHY_FAMILY_CV:   pBc->m_DeviceFamily = DT_BC_SDITXPHY_FAMILY_CV;  break;
    default: DT_ASSERT(FALSE);  return DT_STATUS_FAIL;
    }

    // Device family specific initialization
    if (pBc->m_DeviceFamily==DT_BC_SDITXPHY_FAMILY_A10 
                                        || pBc->m_DeviceFamily==DT_BC_SDITXPHY_FAMILY_C10)
        // Select clock
        DT_RETURN_ON_ERROR(DtBcSDITXPHY_C10A10_SetPllSelect(pBc, 
                                                        pBc->m_C10A10FractClock ? 1 : 0));

    // Get maximum supported rate
    FwMaxSdiRate = SDITXPHY_Config_READ_MaxSdiRate(pBc);
    switch (FwMaxSdiRate)
    {
    case SDITXPHY_SDIMODE_SD:  pBc->m_MaxSdiRate = DT_DRV_SDIRATE_SD; break;
    case SDITXPHY_SDIMODE_HD:  pBc->m_MaxSdiRate = DT_DRV_SDIRATE_HD; break;
    case SDITXPHY_SDIMODE_3G:  pBc->m_MaxSdiRate = DT_DRV_SDIRATE_3G; break;
    case SDITXPHY_SDIMODE_6G:  pBc->m_MaxSdiRate = DT_DRV_SDIRATE_6G; break;
    case SDITXPHY_SDIMODE_12G: pBc->m_MaxSdiRate = DT_DRV_SDIRATE_12G; break;
    default: DT_ASSERT(FALSE);  return DT_STATUS_FAIL;
    }

    // Select default up-sample factor for HD
    pBc->m_SdiRate = DT_DRV_SDIRATE_HD;
    pBc->m_UpsampleFactor = (pBc->m_MaxSdiRate <= DT_DRV_SDIRATE_3G) ? 2 : 8;

    // Make settings in register
    DtBcSDITXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                  pBc->m_ClockReset, FALSE, 
                                                  pBc->m_SofDelay, pBc->m_UpsampleFactor);
    // Set slew rate
    DtBcSDITXPHY_SetSlewRateControl(pBc, pBc->m_SdiRate);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXPHY_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcSDITXPHY* pBc = (DtBcSDITXPHY*) pBcBase;
 
    // Sanity check
    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE-> ENABLE
        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
        pBc->m_ClockReset = TRUE;
        pBc->m_SofDelay = 0;
    }
    else
    {
        // ENABLED -> DISABLED?
        // Operational mode to IDLE
        Status = DtBcSDITXPHY_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SDITXPHY, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }
    // Save block enable
    pBc->m_BlockEnabled = Enable;
    // Make setting in control register
    DtBcSDITXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                  pBc->m_ClockReset, FALSE, 
                                                  pBc->m_SofDelay, pBc->m_UpsampleFactor);
    // Device family specific initialization
    if (pBc->m_DeviceFamily==DT_BC_SDITXPHY_FAMILY_A10 
                                        || pBc->m_DeviceFamily==DT_BC_SDITXPHY_FAMILY_C10)
        // Select clock
        DT_RETURN_ON_ERROR(DtBcSDITXPHY_C10A10_SetPllSelect(pBc, 
                                                        pBc->m_C10A10FractClock ? 1 : 0));
    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSDITXPHY_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_SDITXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled(pBc))
    {
        DtDbgOutBc(AVG, SDITXPHY, pBc, "Set operational mode to IDLE");

        // Set operational mode to IDLE
        Status = DtBcSDITXPHY_SetOperationalMode(BC_SDITXPHY, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SDITXPHY, pBc, "ERROR: failed to set opmode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_CheckSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXPHY_CheckSdiRate(DtBcSDITXPHY* pBc, Int SdiRate)
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSDITXPHY_SetControlRegs(DtBcSDITXPHY* pBc, Bool BlkEnable, Int OpMode,
                                  Bool TxClkReset,  Bool Arm, Int SofDelay, Int SrcFactor)
{
    UInt RegData=0, FldOpMode=0, FldBlkEnable=0, FldSrcFactor=0;

    // Convert block enable to BB-type
    FldBlkEnable = BlkEnable ? SDITXPHY_BLKENA_ENABLED : SDITXPHY_BLKENA_DISABLED;

    // Fix for DTA-2274B 3G-outputs (MediaKind) where SofDelay is specified in 
    // 74.25MHz ticks instead of 148.5MHz ticks TT3292
    if (pBc->m_Version==2 && pBc->m_MaxSdiRate==DT_DRV_SDIRATE_3G)
        SofDelay = SofDelay/2;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FldOpMode = SDITXPHY_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FldOpMode = SDITXPHY_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FldOpMode = SDITXPHY_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Convert upsample factor to BB-type
    switch (SrcFactor)
    {
    case 1:   FldSrcFactor = SDITXPHY_SDIMODE_BYPASS; break;
    case 2:   FldSrcFactor = SDITXPHY_SDIMODE_x2; break;
    case 4:   FldSrcFactor = SDITXPHY_SDIMODE_x4; break;
    case 8:   FldSrcFactor = SDITXPHY_SDIMODE_x8; break;
    case 11:  FldSrcFactor = SDITXPHY_SDIMODE_x11; break;
    case 44:  FldSrcFactor = SDITXPHY_SDIMODE_x44; break;
    default: DT_ASSERT(FALSE);
    }

    // Invalid combinations?
    DT_ASSERT(BlkEnable || OpMode==DT_BLOCK_OPMODE_IDLE);
    DT_ASSERT(!TxClkReset || OpMode==DT_BLOCK_OPMODE_IDLE);
    DT_ASSERT(!Arm || OpMode==DT_BLOCK_OPMODE_STANDBY);

    // Set FIFO inhibit first?
    if (!BlkEnable || OpMode==DT_BLOCK_OPMODE_IDLE || TxClkReset)
    { 
        RegData = SDITXPHY_CdcFifoControl_READ(pBc);
        RegData = SDITXPHY_CdcFifoControl_SET_InhibitFifo(RegData, 1);
        SDITXPHY_CdcFifoControl_WRITE(pBc, RegData);
    }

    // Update SDITXPHY control register
    RegData = SDITXPHY_Control_READ(pBc);
    RegData = SDITXPHY_Control_SET_BlockEnable(RegData, FldBlkEnable);
    RegData = SDITXPHY_Control_SET_OperationalMode(RegData, FldOpMode);
    RegData = SDITXPHY_Control_SET_TxClkReset(RegData, TxClkReset ?  1 : 0);
    RegData = SDITXPHY_Control_SET_Arm(RegData, Arm ?  1 : 0);
    RegData = SDITXPHY_Control_SET_SrcFactor(RegData, FldSrcFactor);
    RegData = SDITXPHY_Control_SET_GenlockSofDelay(RegData, (UInt32)SofDelay);
    SDITXPHY_Control_WRITE(BC_SDITXPHY, RegData);

    // Can FIFO inhibit be cleared?
    if (BlkEnable && OpMode!=DT_BLOCK_OPMODE_IDLE && !TxClkReset)
    { 
        RegData = SDITXPHY_CdcFifoControl_READ(pBc);
        RegData = SDITXPHY_CdcFifoControl_SET_InhibitFifo(RegData, 0);
        SDITXPHY_CdcFifoControl_WRITE(pBc, RegData);
    }
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_C10A10_SetPllSelect -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXPHY_C10A10_SetPllSelect(DtBcSDITXPHY* pBc, Int ClockIdx)
{
    UInt32 PllId, PllSelect;

    // Parameter check
    if (ClockIdx!=0 && ClockIdx!=1)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Determine the PllId
    switch (ClockIdx)
    {
        case 0: PllId = SDITXPHY_C10A10_PllLookup_READ_Pll0(pBc); break;
        case 1: PllId = SDITXPHY_C10A10_PllLookup_READ_Pll1(pBc); break;
        case 2: PllId = SDITXPHY_C10A10_PllLookup_READ_Pll2(pBc); break;
        case 3: PllId = SDITXPHY_C10A10_PllLookup_READ_Pll3(pBc); break;
        default:  return DT_STATUS_FAIL;
    }
    PllSelect =  (((PllId&8)^8)<<4) | ((PllId&3)<<5) | ((PllId&8)<<1) | (PllId&0xF);

    SDITXPHY_C10A10_PllSelectionMux_WRITE(pBc, PllSelect);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPHY_SetSlewRateControl -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcSDITXPHY_SetSlewRateControl(DtBcSDITXPHY* pBc, Int SdiRate)
{
    UInt32  FwSlewRate = SDITXPHY_SLEWRATE_SLEW_R3;
    UInt32  RegTxCtrl = 0;

    // Supported by C10A10 PHY version 2 and later
    if (pBc->m_DeviceFamily==DT_BC_SDITXPHY_FAMILY_CV || pBc->m_Version < 2)
        return;

    switch (SdiRate)
    {
    case DT_DRV_SDIRATE_SD:   FwSlewRate = SDITXPHY_SLEWRATE_SLEW_R1; break;
    case DT_DRV_SDIRATE_HD:   FwSlewRate = SDITXPHY_SLEWRATE_SLEW_R3; break;
    case DT_DRV_SDIRATE_3G:   FwSlewRate = SDITXPHY_SLEWRATE_SLEW_R4; break;
    case DT_DRV_SDIRATE_6G:   FwSlewRate = SDITXPHY_SLEWRATE_SLEW_R5; break;
    case DT_DRV_SDIRATE_12G:  FwSlewRate = SDITXPHY_SLEWRATE_SLEW_R5; break;
    }
    RegTxCtrl = SDITXPHY_C10A10_TxCtrl_READ(pBc);
    RegTxCtrl = SDITXPHY_C10A10_TxCtrl_SET_SlewRateCtrl(RegTxCtrl,  FwSlewRate);
    SDITXPHY_C10A10_TxCtrl_WRITE(pBc, RegTxCtrl);
}
