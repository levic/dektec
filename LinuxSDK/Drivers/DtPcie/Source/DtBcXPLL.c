// =+=+=+=+=+=+=+=+=+=+ DtBcXPLL.c *#*#*#*#*#*#*#*#* (C) 2022 DekTec +=+=+=+=+=+=+=+=+=+=+

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
#include "DtBcXPLL.h"
#include "DtBcXPLL_RegAccess.h"


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcXPLL implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_XPLL_DEFAULT_PRECONDITIONS(pBc)      \
                                     DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcXPLL))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_XPLL_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(XPLL, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcXPLL_Init(DtBc*);
static DtStatus  DtBcXPLL_OnEnable(DtBc*, Bool Enable);
static DtStatus  DtBcXPLL_ResetAndCalibrateFPLLPll(DtBcXPLL* pBc);
static DtStatus  DtBcXPLL_ResetAndCalibrateAtxPll(DtBcXPLL* pBc);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcXPLL - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcXPLL_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcXPLL_Close(DtBc*  pBc)
{
    BC_XPLL_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcXPLL_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcXPLL*  DtBcXPLL_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                                                const char* pRole, Int Instance, Int Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_XPLL_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcXPLL, Id, DT_BLOCK_TYPE_XPLL,
                                                              Address, pPt, FALSE, pCore);
    
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcXPLL_Close;
    OpenParams.m_InitFunc = DtBcXPLL_Init;
    OpenParams.m_OnEnableFunc = DtBcXPLL_OnEnable;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcXPLL*)DtBc_Open(&OpenParams);
}

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcXPLL - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcXPLL_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcXPLL_Init(DtBc* pBcBase)
{
    DtBcXPLL*  pBc = (DtBcXPLL*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcXPLL_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcXPLL_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtBcXPLL*  pBc = (DtBcXPLL*)pBcBase;

    // Sanity check
    BC_XPLL_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, XPLL, pBc, "OnEnable from disable -> enable");

        // Block can now be enabled: PLL remain in reset by the other flags.
        XPLL_Control_WRITE(pBc, XPLL_Control_SET_BlockEnable(0, 1));

       
        pBc->m_BlockEnabled = TRUE;
        return DT_STATUS_OK;
    }

    // ENABLE -> DISABLE
    DtDbgOutBc(AVG, XPLL, pBc, "OnEnable from enable -> disable");
    XPLL_Control_WRITE(pBc, XPLL_Control_SET_BlockEnable(0, 0));

    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_BlockEnabled = FALSE;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- WAIT_UNTIL_READ -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Macro to improve readability
#define WAIT_UNTIL_READ(read_reg, state, timeout_ms, err_msg)       \
{                                                                   \
    Int Count = 0;                                                  \
    do                                                              \
    {                                                               \
        Int Val = read_reg(pBc);                                    \
        if (Val == state) break;                                    \
        DtSleep(1);                                                 \
    } while (Count++ < timeout_ms);                                 \
    if (Count >= timeout_ms)                                        \
    {                                                               \
        DtDbgOutBc(ERR, XPLL, pBc, err_msg);                        \
        return DT_STATUS_TIMEOUT;                                   \
    }                                                               \
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcXPLL_ResetAndCalibratePll -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Resets and calibrates the PLL.
// Pre-condition: Reference clock is stable.
DtStatus  DtBcXPLL_ResetAndCalibratePll(DtBcXPLL* pBc)
{
    UInt32  RegData = 0;
    
    // Sanity check
    BC_XPLL_DEFAULT_PRECONDITIONS(pBc);
    RegData = XPLL_Config_READ(pBc);
    switch (XPLL_Config_GET_PllType(RegData))
    {
    case XPLL_PLLTYPE_FPLL: return DtBcXPLL_ResetAndCalibrateFPLLPll(pBc);
    case XPLL_PLLTYPE_ATX: return DtBcXPLL_ResetAndCalibrateAtxPll(pBc);
    default:
        DtDbgOutBc(ERR, XPLL, pBc, "Unknown PLL type.");
        return DT_STATUS_NOT_IMPLEMENTED;
    }
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcXPLL_ResetAndCalibrateFPLLPll -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcXPLL_ResetAndCalibrateFPLLPll(DtBcXPLL* pBc)
{
    UInt32  RegData = 0;
    Int WaitTimeoutMs = 100;  // Default 100ms

    // Sanity check
    BC_XPLL_DEFAULT_PRECONDITIONS(pBc);

    DtDbgOutBc(MAX, XPLL, pBc, "Init");

    // Power-up calibration may still be running, so wait (500ms) until it is completed
    WaitTimeoutMs = 500;
    WAIT_UNTIL_READ(XPLL_Status_READ_PllCalBusy, 0, WaitTimeoutMs,
        "ERROR: Waiting for PLL-CalBusy flag to become 0 timed out.");

    // Reset PLL
    RegData = XPLL_C10A10_PllCore_Control_READ(pBc);
    RegData = XPLL_C10A10_PllCore_Control_SET_PllPowerdown(RegData, 1);
    XPLL_C10A10_PllCore_Control_WRITE(pBc, RegData);

    // Request access to the PLL’s configuration bus so that we can start calibration.
    // IMPORTANT NOTE: Register AvMmArbitration may NOT be read to avoid a bus time out.
    // The statement below must be implemented with a write, not with a read-modify-write.
    RegData = 0;
    RegData = XPLL_C10A10_PllCore_AvmmArbitration_SET_ArbiterCtrl(
                                                               RegData, XPLL_ARBREQ_User);
    RegData = XPLL_C10A10_PllCore_AvmmArbitration_SET_CalDone(RegData, 1);
    XPLL_C10A10_PllCore_AvmmArbitration_WRITE(pBc, RegData);

    // Wait max. 5ms
    WaitTimeoutMs = 5;
    WAIT_UNTIL_READ(XPLL_C10A10_PllCore_Status_READ_AvMmBusy, 
                                             XPLL_ARBREQ_User, WaitTimeoutMs,
                                             "ERROR: Waiting for Pll-AvmBusy timed out.");

    // Request FPGA calibration engine (PreSICE) to calibrate the PLL.
    RegData = 0;
    RegData = XPLL_C10A10_PllCore_Calibration_SET_FpllEnable(RegData, 1);
    XPLL_C10A10_PllCore_Calibration_WRITE(pBc, RegData);

    // Release the PLL’s configuration bus so that PreSICE can start.
    RegData = 0;
    RegData = XPLL_C10A10_PllCore_AvmmArbitration_SET_ArbiterCtrl(
                                                            RegData, XPLL_ARBREQ_PreSICE);
    RegData = XPLL_C10A10_PllCore_AvmmArbitration_SET_CalDone(RegData, 0);
    XPLL_C10A10_PllCore_AvmmArbitration_WRITE(pBc, RegData);

    DtWaitBlock(1); // Safety: Allow PreSICE to start up before checking busy flag.

    // Wait a maximum of 100ms. 
    // NOTE: typical calibration times are: fPLL=1.3ms and ATX=25.5ms
    WaitTimeoutMs = 100;
    WAIT_UNTIL_READ(XPLL_Status_READ_PllCalBusy, 0, WaitTimeoutMs,
                                             "ERROR: Waiting for PLL-CalBusy timed out.");

    RegData = XPLL_C10A10_PllCore_Control_READ(pBc);
    RegData = XPLL_C10A10_PllCore_Control_SET_PllPowerdown(RegData, 0);
    XPLL_C10A10_PllCore_Control_WRITE(pBc, RegData);

    // Wait (100ms) until PLL is locked.
    WaitTimeoutMs = 100;
    WAIT_UNTIL_READ(XPLL_Status_READ_PllLocked, 1, WaitTimeoutMs,
                                              "ERROR: Waiting for Pll-Locked timed out.");
    DtDbgOutBc(MAX, XPLL, pBc, "Exit");

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcXPLL_ResetAndCalibrateAtxPll -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcXPLL_ResetAndCalibrateAtxPll(DtBcXPLL* pBc)
{
    UInt32  RegData = 0;
    Int WaitTimeoutMs = 100;  // Default 100ms

    // Sanity check
    BC_XPLL_DEFAULT_PRECONDITIONS(pBc);

    DtDbgOutBc(MAX, XPLL, pBc, "Init");

    // Power-up calibration may still be running, so wait (500ms) until it is completed
    WaitTimeoutMs = 500;
    WAIT_UNTIL_READ(XPLL_Status_READ_PllCalBusy, 0, WaitTimeoutMs,
                            "ERROR: Waiting for PLL-CalBusy flag to become 0 timed out.");
    
    // Reset PLL
    RegData = XPLL_C10A10_PllCore_Control_READ(pBc);
    RegData = XPLL_C10A10_PllCore_Control_SET_PllPowerdown(RegData, 1);
    XPLL_C10A10_PllCore_Control_WRITE(pBc, RegData);
    
    // Request access to the PLL’s configuration bus so that we can start calibration.
    // IMPORTANT NOTE: Register AvMmArbitration may NOT be read to avoid a bus time out.
    // The statement below must be implemented with a write, not with a read-modify-write.
    RegData = 0;
    RegData = XPLL_C10A10_PllCore_AvmmArbitration_SET_ArbiterCtrl(RegData,
                                                                        XPLL_ARBREQ_User);
    RegData = XPLL_C10A10_PllCore_AvmmArbitration_SET_CalDone(RegData, 1);
    XPLL_C10A10_PllCore_AvmmArbitration_WRITE(pBc, RegData);

    // Wait max. 5ms
    WaitTimeoutMs = 5;
    WAIT_UNTIL_READ(XPLL_C10A10_PllCore_Status_READ_AvMmBusy,
                                             XPLL_ARBREQ_User, WaitTimeoutMs,
                                             "ERROR: Waiting for Pll-AvmBusy timed out.");

    // Request FPGA calibration engine (PreSICE) to calibrate the PLL.
    RegData = 0;
    RegData = XPLL_C10A10_PllCore_Calibration_SET_AtxEnable(RegData, 1);
    XPLL_C10A10_PllCore_Calibration_WRITE(pBc, RegData);

    // Release the PLL’s configuration bus so that PreSICE can start.
    RegData = 0;
    RegData = XPLL_C10A10_PllCore_AvmmArbitration_SET_ArbiterCtrl(RegData,
                                                                     XPLL_ARBREQ_PreSICE);
    RegData = XPLL_C10A10_PllCore_AvmmArbitration_SET_CalDone(RegData, 0);
    XPLL_C10A10_PllCore_AvmmArbitration_WRITE(pBc, RegData);

    DtWaitBlock(1); // Safety: Allow PreSICE to start up before checking busy flag.

    // Wait a maximum of 100ms. 
    // NOTE: typical calibration times are: fPLL=1.3ms and ATX=25.5ms
    WaitTimeoutMs = 100;
    WAIT_UNTIL_READ(XPLL_Status_READ_PllCalBusy, 0, WaitTimeoutMs,
                                             "ERROR: Waiting for PLL-CalBusy timed out.");

    RegData = XPLL_C10A10_PllCore_Control_READ(pBc);
    RegData = XPLL_C10A10_PllCore_Control_SET_PllPowerdown(RegData, 0);
    XPLL_C10A10_PllCore_Control_WRITE(pBc, RegData);

    // Wait (100ms) until PLL is locked.
    WaitTimeoutMs = 100;
    WAIT_UNTIL_READ(XPLL_Status_READ_PllLocked, 1, WaitTimeoutMs,
                                              "ERROR: Waiting for Pll-Locked timed out.");
    DtDbgOutBc(MAX, XPLL, pBc, "Exit");
    return DT_STATUS_OK;
}

#undef WAIT_UNTIL_READ
