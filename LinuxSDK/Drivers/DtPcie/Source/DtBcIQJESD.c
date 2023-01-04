//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQJESD.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec

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
#include "DtBcIQJESD.h"
#include "DtBcIQJESD_RegAccess.h"


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQJESD implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQJESD_DEFAULT_PRECONDITIONS(pBc)      \
                                   DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQJESD))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQJESD_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQJESD, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQJESD_ResetAndCalibratePll(DtBcIQJESD* pBc);
static DtStatus  DtBcIQJESD_ResetAndCalibratePhy(DtBcIQJESD* pBc);
static DtStatus  DtBcIQJESD_CalibrateAtxPll(DtBcIQJESD* pBc);
static DtStatus  DtBcIQJESD_InitTx(DtBcIQJESD* pBc);
static DtStatus  DtBcIQJESD_InitJesdInterface(DtBcIQJESD* pBc);
static DtStatus  DtBcIQJESD_GetOperationalMode(DtBcIQJESD*, Int*);
static DtStatus  DtBcIQJESD_GetSysrefAlwaysOn(DtBcIQJESD* ,Bool* State);
static DtStatus  DtBcIQJESD_Init(DtBc*);
static DtStatus  DtBcIQJESD_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIQJESD_OnEnable(DtBc*, Bool Enable);
static DtStatus  DtBcIQJESD_ReInitializeCsrLink(DtBcIQJESD* pBc);
static DtStatus  DtBcIQJESD_SetSysrefAlwaysOn(DtBcIQJESD*pBc, Bool State);
static DtStatus  DtBcIQJESD_SetOperationalMode(DtBcIQJESD* pBc, Int OpMode);
static void  DtBcIQJESD_SetControlRegs(DtBcIQJESD* pBc, Bool BlkEna, Int OpMode, 
                                                                       Bool PhyLinkReset);

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQJESD - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcIQJESD_Close(DtBc*  pBc)
{
    BC_IQJESD_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIQJESD*  DtBcIQJESD_Open(Int  Address, DtCore* pCore, DtPt* pPt,
    const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_IQJESD_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQJESD, Id, DT_BLOCK_TYPE_IQJESD,
        Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQJESD_Close;
    OpenParams.m_InitFunc = DtBcIQJESD_Init;
    OpenParams.m_OnEnableFunc = DtBcIQJESD_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIQJESD_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQJESD*)DtBc_Open(&OpenParams);
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQJESD - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQJESD_GetOperationalMode(DtBcIQJESD* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IQJESD_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQJESD_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_GetSysrefAlwaysOn -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQJESD_GetSysrefAlwaysOn(DtBcIQJESD* pBc, Bool* State)
{
    BC_IQJESD_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_IQJESD_MUST_BE_ENABLED(pBc);

    *State = pBc->m_CsrSysrefAlwayson;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQJESD_Init(DtBc* pBcBase)
{
    DtBcIQJESD*  pBc = (DtBcIQJESD*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    // Set control register
    DtBcIQJESD_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                     pBc->m_PhyLinkReset);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQJESD_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQJESD* pBc = (DtBcIQJESD*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IQJESD_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IQJESD, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIQJESD_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQJESD, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQJESD_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtBcIQJESD*  pBc = (DtBcIQJESD*)pBcBase;
    
    // Sanity check
    BC_IQJESD_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IQJESD, pBc, "OnEnable from disable -> enable");

        // .-.-.-.-.-.-.-.-.-.-.-.-.- Initialize JESD interface -.-.-.-.-.-.-.-.-.-.-.-.-.
        DT_RETURN_ON_ERROR(DtBcIQJESD_InitJesdInterface(pBc));
        
        // Block can now be enabled: PHY and PLL remain in reset by the other flags.
        pBc->m_BlockEnabled = TRUE;
        DtBcIQJESD_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                     pBc->m_PhyLinkReset);

        DT_RETURN_ON_ERROR(DtBcIQJESD_ResetAndCalibratePll(pBc));
        DT_RETURN_ON_ERROR(DtBcIQJESD_ResetAndCalibratePhy(pBc));

        // IqJesdDacItf is ready to receive I/Q samples.
        // Other I/Q transmit blocks can be switched to ‘RUN’.
        pBc->m_PhyLinkReset = FALSE;
        DtBcIQJESD_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                     pBc->m_PhyLinkReset);

        // Init TX values
        DT_RETURN_ON_ERROR(DtBcIQJESD_InitTx(pBc));
        return DT_STATUS_OK;
    }

    // ENABLE -> DISABLE
    DtDbgOutBc(AVG, IQJESD, pBc, "OnEnable from enable -> disable");
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_BlockEnabled = FALSE;
    DtBcIQJESD_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                     pBc->m_PhyLinkReset);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- WAIT_UNTIL_READ -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Macro to improve readability
#define WAIT_UNTIL_READ(read_reg, state, err_msg) \
Count = 0;                                        \
do                                                \
{                                                 \
    WaitVal = read_reg(pBc);                      \
    if (WaitVal == state) break;                  \
    DtSleep(10);                                  \
} while (Count++ < WaitTimeout);                  \
if (Count >= WaitTimeout)                         \
{                                                 \
    DtDbgOutBc(ERR, IQJESD, pBc, err_msg);        \
    return DT_STATUS_TIMEOUT;                     \
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_CalibrateAtxPll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Runtime calibration
static DtStatus  DtBcIQJESD_CalibrateAtxPll(DtBcIQJESD* pBc)
{
    UInt32  RegData = 0;

    BC_IQJESD_DEFAULT_PRECONDITIONS(pBc);

    // Must be IDLE
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    pBc->m_PhyLinkReset = TRUE;
    DtBcIQJESD_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                     pBc->m_PhyLinkReset);

    RegData = IQJESD_JesdPhy_Control_READ(pBc);
    RegData = IQJESD_JesdPhy_Control_SET_TxAnalogReset(RegData, 1);
    RegData = IQJESD_JesdPhy_Control_SET_TxDigitalReset(RegData, 1);
    IQJESD_JesdPhy_Control_WRITE(pBc, RegData);

    DT_RETURN_ON_ERROR(DtBcIQJESD_ResetAndCalibratePll(pBc));
    DT_RETURN_ON_ERROR(DtBcIQJESD_ResetAndCalibratePhy(pBc));
    
    // IqJesdDacItf is ready to receive I/Q samples.
    pBc->m_PhyLinkReset = FALSE;
    DtBcIQJESD_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                     pBc->m_PhyLinkReset);

    // Init TX values
    DT_RETURN_ON_ERROR(DtBcIQJESD_InitTx(pBc));
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_ReInitializeCsrLink -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQJESD_ReInitializeCsrLink(DtBcIQJESD* pBc)
{
    UInt32  RegData=0;

    BC_IQJESD_DEFAULT_PRECONDITIONS(pBc);
    
    // Must be enabled
    BC_IQJESD_MUST_BE_ENABLED(pBc);

    RegData = IQJESD_JesdTx_SyncnSysrefCtrl_READ(pBc);
    RegData = IQJESD_JesdTx_SyncnSysrefCtrl_SET_CsrLinkReinit(RegData, 1);
    IQJESD_JesdTx_SyncnSysrefCtrl_WRITE(pBc, RegData);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_Enable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQJESD_InitJesdInterface(DtBcIQJESD* pBc)
{
    UInt32  RegData = 0;

    BC_IQJESD_DEFAULT_PRECONDITIONS(pBc);

    // Clear timed-out status flags.
    IQJESD_Status_CLEAR_PllAccessTimedout(pBc);
    IQJESD_Status_CLEAR_XcvrAccessTimedout(pBc);

    pBc->m_PhyLinkReset = TRUE;
    DtBcIQJESD_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                     pBc->m_PhyLinkReset);
    RegData = 0;
    RegData = IQJESD_JesdPhy_Control_SET_TxAnalogReset(RegData, 1);
    RegData = IQJESD_JesdPhy_Control_SET_OverrideTxAnalogReset(RegData, 1);
    RegData = IQJESD_JesdPhy_Control_SET_TxDigitalReset(RegData, 1);
    RegData = IQJESD_JesdPhy_Control_SET_OverrideTxDigitalReset(RegData, 1);
    IQJESD_JesdPhy_Control_WRITE(pBc, RegData);

    RegData = 0;
    RegData = IQJESD_Pll_Control_SET_PllPowerdown(RegData, 1);
    RegData = IQJESD_Pll_Control_SET_OverridePllPowerdown(RegData, 1);
    IQJESD_Pll_Control_WRITE(pBc, RegData);
    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_InitTx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Set CsrLinkReinit + CsrSysrefAlwayson
static DtStatus  DtBcIQJESD_InitTx(DtBcIQJESD* pBc)
{
    UInt32  RegData = 0;
    
    // Sanity check
    BC_IQJESD_DEFAULT_PRECONDITIONS(pBc);

    RegData = IQJESD_JesdTx_SyncnSysrefCtrl_READ(pBc);
    RegData = IQJESD_JesdTx_SyncnSysrefCtrl_SET_CsrLinkReinit(RegData, 1);
    RegData = IQJESD_JesdTx_SyncnSysrefCtrl_SET_CsrSysrefAlwayson(RegData, 1);
    IQJESD_JesdTx_SyncnSysrefCtrl_WRITE(pBc, RegData);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_ResetAndCalibratePll -.-.-.-.-.-.-.-.-.-.-.-.-.-
// Resets and calibrates the PLL.
// Pre-condition: PHY reset lines are asserted.
//                Reference clock is stable.
DtStatus  DtBcIQJESD_ResetAndCalibratePll_V0(DtBcIQJESD* pBc)
{
    UInt32  RegData=0;
    
    // Waiting params
    Int  Count = 0;
    UInt32  WaitVal=0;
    const int WaitTimeout = 1000;

    // Sanity check
    BC_IQJESD_DEFAULT_PRECONDITIONS(pBc);

   
    // Power-up calibration may still be running, so wait until it is completed
    WAIT_UNTIL_READ(IQJESD_Pll_Status_READ_CalBusy, 0,
                            "ERROR: Waiting for PLL-CalBusy flag to become 0 timed out.");
    WAIT_UNTIL_READ(IQJESD_JesdPhy_Status2_READ_TxCalBusy, 0,
                          "ERROR: Waiting for Phy-TxCalBusy flag to become 0 timed out.");

    // Reset PLL
    RegData = IQJESD_Pll_Control_READ(pBc);
    RegData = IQJESD_Pll_Control_SET_PllPowerdown(RegData, 1);
    IQJESD_Pll_Control_WRITE(pBc, RegData);
    DtWaitBlock(1);
    RegData = IQJESD_Pll_Control_SET_PllPowerdown(RegData, 0);
    IQJESD_Pll_Control_WRITE(pBc, RegData);

    // Request access to the PLL’s configuration bus so that we can start calibration.
    // IMPORTANT NOTE: Register AvMmArbitration may NOT be read to avoid a bus time out.
    // The statement below must be implemented with a write, not with a read-modify-write.
    RegData = 0;
    RegData = IQJESD_Pll_AvmmArbitration_SET_ArbiterCtrl(RegData, IQJESD_ARBREQ_User);
    RegData = IQJESD_Pll_AvmmArbitration_SET_CalDone(RegData, 1);
    IQJESD_Pll_AvmmArbitration_WRITE(pBc, RegData);
    WAIT_UNTIL_READ(IQJESD_Pll_Status_READ_AvMmBusy, IQJESD_BUSOWNER_User,
                                             "ERROR: Waiting for Pll-AvmBusy timed out.");

    // Request FPGA calibration engine (PreSICE) to calibrate the ATX PLL.
    RegData = 0;
    RegData = IQJESD_Pll_Calibration_SET_Enable(RegData, 1);
    IQJESD_Pll_Calibration_WRITE(pBc, RegData);

    // Release the PLL’s configuration bus so that PreSICE can start.
    RegData = 0;
    RegData = IQJESD_Pll_AvmmArbitration_SET_ArbiterCtrl(RegData, IQJESD_ARBREQ_PreSICE);
    RegData = IQJESD_Pll_AvmmArbitration_SET_CalDone(RegData, 0);
    IQJESD_Pll_AvmmArbitration_WRITE(pBc, RegData);

    // Safety: Allow PreSICE to start up before checking busy flag.
    DtWaitBlock(1);

    // Wait until calibration is finished.
    WAIT_UNTIL_READ(IQJESD_Pll_Status_READ_CalBusy, 0,
                                             "ERROR: Waiting for PLL-CalBusy timed out.");

    // Wait until PLL is locked.
    WAIT_UNTIL_READ(IQJESD_Pll_Status_READ_Locked, 1,
                                              "ERROR: Waiting for Pll-Locked timed out.");

    // Clear loss-of-lock status to allow for periodic loss-of-lock check.
    IQJESD_Status_CLEAR_PllLockLossLatched(pBc);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_ResetAndCalibratePll -.-.-.-.-.-.-.-.-.-.-.-.-.-
// Resets and calibrates the PLL.
// Pre-condition: PHY reset lines are asserted.
//                Reference clock is stable.
DtStatus  DtBcIQJESD_ResetAndCalibratePll(DtBcIQJESD* pBc)
{
    UInt32  RegData = 0;
    
    // Waiting params
    Int  Count = 0;
    UInt32  WaitVal = 0;
    const int WaitTimeout = 1000;

    // Sanity check
    BC_IQJESD_DEFAULT_PRECONDITIONS(pBc);


    // Power-up calibration may still be running, so wait until it is completed
    WAIT_UNTIL_READ(IQJESD_Pll_Status_READ_CalBusy, 0,
        "ERROR: Waiting for PLL-CalBusy flag to become 0 timed out.");
    WAIT_UNTIL_READ(IQJESD_JesdPhy_Status2_READ_TxCalBusy, 0,
        "ERROR: Waiting for Phy-TxCalBusy flag to become 0 timed out.");

    // Reset PLL
    RegData = IQJESD_Pll_Control_READ(pBc);
    RegData = IQJESD_Pll_Control_SET_PllPowerdown(RegData, 1);
    IQJESD_Pll_Control_WRITE(pBc, RegData);
    
    // Request access to the PLL’s configuration bus so that we can start calibration.
    // IMPORTANT NOTE: Register AvMmArbitration may NOT be read to avoid a bus time out.
    // The statement below must be implemented with a write, not with a read-modify-write.
    RegData = 0;
    RegData = IQJESD_Pll_AvmmArbitration_SET_ArbiterCtrl(RegData, IQJESD_ARBREQ_User);
    RegData = IQJESD_Pll_AvmmArbitration_SET_CalDone(RegData, 1);
    IQJESD_Pll_AvmmArbitration_WRITE(pBc, RegData);
    WAIT_UNTIL_READ(IQJESD_Pll_Status_READ_AvMmBusy, IQJESD_BUSOWNER_User,
        "ERROR: Waiting for Pll-AvmBusy timed out.");

    // Request FPGA calibration engine (PreSICE) to calibrate the ATX PLL.
    RegData = 0;
    RegData = IQJESD_Pll_Calibration_SET_Enable(RegData, 1);
    IQJESD_Pll_Calibration_WRITE(pBc, RegData);

    // Release the PLL’s configuration bus so that PreSICE can start.
    RegData = 0;
    RegData = IQJESD_Pll_AvmmArbitration_SET_ArbiterCtrl(RegData, IQJESD_ARBREQ_PreSICE);
    RegData = IQJESD_Pll_AvmmArbitration_SET_CalDone(RegData, 0);
    IQJESD_Pll_AvmmArbitration_WRITE(pBc, RegData);

    DtWaitBlock(1); // Safety: Allow PreSICE to start up before checking busy flag.

    // Wait until calibration is finished.
    WAIT_UNTIL_READ(IQJESD_Pll_Status_READ_CalBusy, 0,
        "ERROR: Waiting for PLL-CalBusy timed out.");

    RegData = IQJESD_Pll_Control_READ(pBc);
    RegData = IQJESD_Pll_Control_SET_PllPowerdown(RegData, 0);
    IQJESD_Pll_Control_WRITE(pBc, RegData);

    // Wait until PLL is locked.
    WAIT_UNTIL_READ(IQJESD_Pll_Status_READ_Locked, 1,
        "ERROR: Waiting for Pll-Locked timed out.");


    // Clear loss-of-lock status to allow for periodic loss-of-lock check.
    IQJESD_Status_CLEAR_PllLockLossLatched(pBc);
    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_SetSysrefAlwaysOn -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQJESD_SetSysrefAlwaysOn(DtBcIQJESD* pBc, Bool State)
{
    UInt32  RegData=0;

    BC_IQJESD_DEFAULT_PRECONDITIONS(pBc);
    
    // Must be enabled
    BC_IQJESD_MUST_BE_ENABLED(pBc);

    pBc->m_CsrSysrefAlwayson = State;
    RegData = IQJESD_JesdTx_SyncnSysrefCtrl_READ(pBc);
    RegData = IQJESD_JesdTx_SyncnSysrefCtrl_SET_CsrSysrefAlwayson(RegData, State ? 1:0);
    IQJESD_JesdTx_SyncnSysrefCtrl_WRITE(pBc, RegData);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_ResetAndCalibratePhy -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQJESD_ResetAndCalibratePhy(DtBcIQJESD* pBc)
{
    UInt32  RegData = 0;

    // Waiting params
    Int  Count = 0;
    UInt32  WaitVal = 0;
    const int WaitTimeout = 1000;

    // Request access to the transceiver’s configuration bus to start calibration.
    // IMPORTANT NOTE: See StartUpPll() for notes on bus arbitration.
    RegData = 0;
    RegData = IQJESD_JesdPhy_AvmmArbitration_SET_ArbiterCtrl(RegData, IQJESD_ARBREQ_User);
    RegData = IQJESD_JesdPhy_AvmmArbitration_SET_CalDone(RegData, 1);
    IQJESD_JesdPhy_AvmmArbitration_WRITE(pBc, RegData);
    WAIT_UNTIL_READ(IQJESD_JesdPhy_Status2_READ_AvMmBusy, IQJESD_BUSOWNER_User,
                                             "ERROR: Waiting for PHY-AvmBusy timed out.");

    // Request calibration engine (PreSICE) to calibrate the Tx transceiver
     RegData = IQJESD_JesdPhy_Calibration_READ(pBc);
    RegData = IQJESD_JesdPhy_Calibration_SET_TxCalEnable(RegData, 1);
    IQJESD_JesdPhy_Calibration_WRITE(pBc, RegData);

    // Release the PLL’s configuration bus so that PreSICE can start.
     RegData = 0;
    RegData = IQJESD_JesdPhy_AvmmArbitration_SET_ArbiterCtrl(RegData, 
                                                                   IQJESD_ARBREQ_PreSICE);
    RegData = IQJESD_JesdPhy_AvmmArbitration_SET_CalDone(RegData, 0);
    IQJESD_JesdPhy_AvmmArbitration_WRITE(pBc, RegData);

    // Safety: Allow PreSICE to start up before checking busy flag.
    DtWaitBlock(1);

    // Wait until transceiver calibration is completed.
    WAIT_UNTIL_READ(IQJESD_JesdPhy_Status2_READ_TxCalBusy, 0,
                                           "ERROR: Waiting for PHY-TxCalBusy timed out.");

    // Resets may be removed after PHY calibration is done and a minimum time has passed
    DtWaitBlock(70);
    RegData = IQJESD_JesdPhy_Control_READ(pBc);
    RegData = IQJESD_JesdPhy_Control_SET_TxAnalogReset(RegData, 0);
    IQJESD_JesdPhy_Control_WRITE(pBc, RegData);
    DtWaitBlock(10);
    RegData = IQJESD_JesdPhy_Control_SET_TxDigitalReset(RegData, 0);
    IQJESD_JesdPhy_Control_WRITE(pBc, RegData);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQJESD_SetOperationalMode(DtBcIQJESD* pBc, Int OpMode)
{

    // Sanity check
    BC_IQJESD_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
        && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQJESD_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcIQJESD_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                     pBc->m_PhyLinkReset);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQJESD_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQJESD_SetControlRegs(DtBcIQJESD* pBc, Bool BlkEna, Int OpMode,
                                                                        Bool PhyLinkReset)
{
    UInt32  RegData = 0, FwBlkEna = 0, FwOpMode = 0, PhyLinkRes = 0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IQJESD_BLKENA_ENABLED : IQJESD_BLKENA_DISABLED;

    // Convert Link reset boolean to 0 or 1
    PhyLinkRes = PhyLinkReset ? 1 : 0;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = IQJESD_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = IQJESD_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = IQJESD_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = IQJESD_Control_SET_PhyLinkReset(RegData, PhyLinkReset);
    RegData = IQJESD_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IQJESD_Control_SET_OperationalMode(RegData, FwOpMode);
    IQJESD_Control_WRITE(pBc, RegData);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQJESD implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IQJESD_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQJESD))
// Helper macro to cast stub's DtBc member to DtBcIQJESD
#define IQJESD_STUB   ((DtIoStubBcIQJESD*)pStub)
#define IQJESD_BC  ((DtBcIQJESD*)IQJESD_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQJESD_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcIQJESD_OnCmdCalibrateAtxPll(const DtIoStubBcIQJESD*);
static DtStatus  DtIoStubBcIQJESD_OnCmdGetOperationalMode(const DtIoStubBcIQJESD*,
                                                        DtIoctlIqJesdCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcIQJESD_OnCmdGetSysrefAlwaysOn(const DtIoStubBcIQJESD*, 
                                                DtIoctlIqJesdCmdGetSysrefAlwaysOnOutput*);
static DtStatus  DtIoStubBcIQJESD_OnCmdReInitializeCsrLink(const DtIoStubBcIQJESD*);
static DtStatus  DtIoStubBcIQJESD_OnCmdSetOperationalMode(const DtIoStubBcIQJESD*,
                                                   const DtIoctlIqJesdCmdSetOpModeInput*);
static DtStatus  DtIoStubBcIQJESD_OnCmdSetSysrefAlwaysOn(const DtIoStubBcIQJESD*, 
                                           const DtIoctlIqJesdCmdSetSysrefAlwaysOnInput*);



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQJESD;

static const DtIoctlProperties  IOSTUB_BC_IQJESD_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQJESD_CMD(
        DtIoStubBcIQJESD_OnCmd,
        NULL, NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQJESD - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQJESD_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcIQJESD_Close(DtIoStub* pStub)
{
    IQJESD_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQJESD_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcIQJESD*  DtIoStubBcIQJESD_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IQJESD_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQJESD, pBc, NULL,
        DtIoStubBcIQJESD_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_IQJESD_IOCTLS);
    return (DtIoStubBcIQJESD*)DtIoStubBc_Open(&OpenParams);
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQJESD - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQJESD_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQJESD_OnCmd(const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqJesdCmdInput*  pInData = NULL;
    DtIoctlIqJesdCmdOutput*  pOutData = NULL;

    IQJESD_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQJESD_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQJESD, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqJesdCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqJesdCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {

    case DT_IQJESD_CMD_CALIBRATE_ATX_PLL:
        Status = DtIoStubBcIQJESD_OnCmdCalibrateAtxPll(IQJESD_STUB);
        break;
    case DT_IQJESD_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQJESD_OnCmdGetOperationalMode(IQJESD_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;    
    case DT_IQJESD_CMD_GET_SYSREF_ALWAYS_ON:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQJESD_OnCmdGetSysrefAlwaysOn(IQJESD_STUB, 
                                                          &pOutData->m_GetSysrefAlwaysOn);
        break;
    case DT_IQJESD_CMD_RE_INITIALIZE_CSR_LINK:
        Status = DtIoStubBcIQJESD_OnCmdReInitializeCsrLink(IQJESD_STUB);
        break;
    case DT_IQJESD_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQJESD_OnCmdSetOperationalMode(IQJESD_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_IQJESD_CMD_SET_SYSREF_ALWAYS_ON:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQJESD_OnCmdSetSysrefAlwaysOn(IQJESD_STUB, 
                                                           &pInData->m_SetSysRefAlwaysOn);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQJESD_OnCmdCalibrateAtxPll -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQJESD_OnCmdCalibrateAtxPll(const DtIoStubBcIQJESD* pStub)
{
    IQJESD_STUB_DEFAULT_PRECONDITIONS(pStub);
    return DtBcIQJESD_CalibrateAtxPll(IQJESD_BC);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQJESD_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQJESD_OnCmdGetOperationalMode(const DtIoStubBcIQJESD* pStub,
                                                DtIoctlIqJesdCmdGetOpModeOutput* pOutData)
{
    IQJESD_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQJESD_GetOperationalMode(IQJESD_BC, &pOutData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQJESD_OnCmdGetSysrefAlwaysOn -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQJESD_OnCmdGetSysrefAlwaysOn(const DtIoStubBcIQJESD* pStub,
                                        DtIoctlIqJesdCmdGetSysrefAlwaysOnOutput* pOutData)
{
    DtStatus Status = DT_STATUS_OK;
    Bool  AlwaysOn = FALSE;

    IQJESD_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    Status = DtBcIQJESD_GetSysrefAlwaysOn(IQJESD_BC, &AlwaysOn);
    pOutData->m_AlwaysOn = AlwaysOn ?1:0;
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQJESD_OnCmdReInitializeCsrLink -.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtIoStubBcIQJESD_OnCmdReInitializeCsrLink(const DtIoStubBcIQJESD* pStub)
{
    IQJESD_STUB_DEFAULT_PRECONDITIONS(pStub);
    return DtBcIQJESD_ReInitializeCsrLink(IQJESD_BC);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQJESD_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQJESD_OnCmdSetOperationalMode(const DtIoStubBcIQJESD* pStub,
                                            const DtIoctlIqJesdCmdSetOpModeInput* pInData)
{
    IQJESD_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQJESD_SetOperationalMode(IQJESD_BC, pInData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQJESD_OnCmdSetSysrefAlwaysOn -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQJESD_OnCmdSetSysrefAlwaysOn(const DtIoStubBcIQJESD* pStub,
                                    const DtIoctlIqJesdCmdSetSysrefAlwaysOnInput* pInData)
{
    IQJESD_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQJESD_SetSysrefAlwaysOn(IQJESD_BC, (pInData->m_AlwaysOn==1)?TRUE:FALSE);
}

