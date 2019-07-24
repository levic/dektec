//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcS2DEMOD_2132.c *#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
#include "DtBcS2DEMOD_2132.h"
#include "DtBcS2DEMOD_2132_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS2DEMOD_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcS2DEMOD_2132))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(S2DEMOD_2132, pBc)

// Helper macro to cast a DtBc* to a DtBcS2DEMOD_2132*
#define BC_S2DEMOD_2132         ((DtBcS2DEMOD_2132*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcS2DEMOD_2132_Init(DtBc*);
static DtStatus  DtBcS2DEMOD_2132_InterruptHandler(DtBc*, Int Id, Int Index, void*);
static void  DtBcS2DEMOD_2132_InterruptDpc(DtDpcArgs*);
static DtStatus  DtBcS2DEMOD_2132_OnEnable(DtBc*, Bool);
static DtStatus  DtBcS2DEMOD_2132_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcS2DEMOD_2132_RegisterIntHandlers(DtBcS2DEMOD_2132* pBc);
static void  DtBcS2DEMOD_2132_SetConstants(DtBcS2DEMOD_2132*);

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS2DEMOD_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcS2DEMOD_2132_Close(DtBc*  pBc)
{
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));
       
    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_DemodReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS2DEMOD_2132_DemodReset(DtBcS2DEMOD_2132* pBc)
{
    UInt32 RegData;

    // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc);

    // Get spinlock
    DtSpinLockAcquire(&pBc->m_Lock);

    // Disable the Creonix interrupt
    DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_S2DEMOD_2132_CREONIX);

    // Issue a demodulator hard reset
    S2DEMOD_DemodRst_PULSE_DemodRst(pBc);

    // Wait for a proper reset (takes 20 clock cycles)
    DtWaitBlock(1);

    // Perform other initialization of the DEMOD of registers that stay constant.
    DtBcS2DEMOD_2132_SetConstants(pBc);

    // re-establish previous interrupt handling
    if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    {
        // Clear interrupt status
        RegData = S2DEMOD_InterruptStatus_SET_Demodulator(0, 1);
        RegData = S2DEMOD_InterruptStatus_SET_FrequencyLoop(RegData, 1);
        RegData = S2DEMOD_InterruptStatus_SET_PlHeaderSync(RegData, 1);
        RegData = S2DEMOD_InterruptStatus_SET_TimingLoop(RegData, 1);
        S2DEMOD_InterruptStatus_WRITE(BC_S2DEMOD_2132, RegData);

        // Enable Creonix interrupts
        RegData = S2DEMOD_InterruptEnable_SET_Demodulator(0, 0);
        RegData = S2DEMOD_InterruptEnable_SET_FrequencyLoop(RegData, 0);
        RegData = S2DEMOD_InterruptEnable_SET_PlHeaderSync(RegData, 1);
        RegData = S2DEMOD_InterruptEnable_SET_TimingLoop(RegData, 0);
        S2DEMOD_InterruptEnable_WRITE(BC_S2DEMOD_2132, RegData);

        // Enable the Creonix interrupt
        DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_S2DEMOD_2132_CREONIX);
    }

    // Use MPI register values by doing a softreset to use these settings
    RegData = S2DEMOD_GlobalControl_SET_SoftReset(0, 1);
    S2DEMOD_GlobalControl_WRITE(BC_S2DEMOD_2132, RegData);

    // Release spinlock
    DtSpinLockRelease(&pBc->m_Lock);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcS2DEMOD_2132*  DtBcS2DEMOD_2132_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_S2DEMOD_2132_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcS2DEMOD_2132, Id, DT_BLOCK_TYPE_S2DEMOD_2132, 
                                                              Address, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcS2DEMOD_2132_Close;
    OpenParams.m_InitFunc = DtBcS2DEMOD_2132_Init;
    OpenParams.m_OnEnableFunc = DtBcS2DEMOD_2132_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcS2DEMOD_2132_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcS2DEMOD_2132*)DtBc_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS2DEMOD_2132_GetOperationalMode(DtBcS2DEMOD_2132* pBc, Int* pOpMode)
{
    // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc);

    // Return last cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_GetPlInformation -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcS2DEMOD_2132_GetPlInformation(DtBcS2DEMOD_2132* pBc, Int* pPlMode, 
                                                                           Int* pPlsValue)
{
     // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pPlMode == NULL || pPlsValue == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc);

    // Get values from cache
    *pPlsValue = pBc->m_PlsValue;
    *pPlMode = pBc->m_PlMode;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_GetFreqOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcS2DEMOD_2132_GetFreqOffset(DtBcS2DEMOD_2132* pBc, Int* pCoarseFreq,
                                                     Int* pFineFreq, Int* pDecimationRate)
{
    // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pCoarseFreq == NULL || pFineFreq == NULL || pDecimationRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc);

    // Get values
    *pCoarseFreq = (Int)S2DEMOD_FreqOffset_READ(pBc);
    *pFineFreq = (Int)S2DEMOD_FineFreqOffset_READ_FreqOffset(pBc);
    *pDecimationRate = (Int)S2DEMOD_DecimationRate_READ(pBc);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_GetRollOff -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS2DEMOD_2132_GetRollOff(DtBcS2DEMOD_2132* pBc, Int* pRollOff)
{
    // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pRollOff == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc);

    // Return last cached roll-off
    *pRollOff = pBc->m_RollOffFactor;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_GetSnrInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS2DEMOD_2132_GetSnrInfo(DtBcS2DEMOD_2132* pBc, UInt* pSigPower,
                                                                          UInt* pErrPower)
{
    // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pSigPower==NULL || pErrPower==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc);

    // Read signal power and error power
    *pSigPower = S2DEMOD_SnrEstimSignalPower_READ_SignalPower(pBc);
    *pErrPower = S2DEMOD_SnrEstimErrorPower_READ_ErrorPower(pBc);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_GetSpectrumInv -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS2DEMOD_2132_GetSpectrumInv(DtBcS2DEMOD_2132* pBc,  Int* pSpecInvCtrl)
{
    // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pSpecInvCtrl == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pSpecInvCtrl = pBc->m_SpectrumInvCtrl;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_GetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS2DEMOD_2132_GetStatus(DtBcS2DEMOD_2132* pBc, Bool* pSignalDetect, 
                               Bool* pSpecInv, Bool* pPlHdrSync, UInt * pPlHdrResyncCount)
{
    UInt32 RegData;

   // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pSignalDetect==NULL || pSpecInv==NULL || pPlHdrSync==NULL
                                                               || pPlHdrResyncCount==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc);

    // Operational mode must be run
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, S2DEMOD_2132, pBc, "Block not in run-mode");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // Read status
    RegData = S2DEMOD_GlobalStatus_READ(pBc);
    *pSignalDetect = (S2DEMOD_GlobalStatus_GET_NoSignal(RegData) == 0);
    *pPlHdrSync = (S2DEMOD_GlobalStatus_GET_PlHeaderSync(RegData) != 0);

    // Get PL-header resync count
    *pPlHdrResyncCount = pBc->m_PlHdrResyncCount;

    // Read spectrum inverted
    *pSpecInv = (S2DEMOD_DetSpectrumInversion_READ_SpectrumInversion(pBc) != 0);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_GetSymbolRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS2DEMOD_2132_GetSymbolRate(DtBcS2DEMOD_2132* pBc, Int* pDecRate, 
                              Int* pSamSymRatio, Int* pSymSamRatio, Int* pMeasSamSymRatio)
{
    UInt32  RegData;
    // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pDecRate==NULL || pSamSymRatio==NULL || pSymSamRatio==NULL 
                                                                || pMeasSamSymRatio==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pDecRate = pBc->m_DecimationRate;
    *pSamSymRatio = pBc->m_SampleToSymbolRateRatio;
    *pSymSamRatio = pBc->m_SymbolToSampleRateRatio;

    // Return measured sample to symbol rate ratio
    RegData = S2DEMOD_MeasSymbRateRatio_READ(pBc);
    *pMeasSamSymRatio = (Int)S2DEMOD_MeasSymbRateRatio_GET_MeasSymbolRateRatio(RegData);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_SetRollOff -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS2DEMOD_2132_SetRollOff(DtBcS2DEMOD_2132* pBc, Int RollOff)
{
    UInt32  RegData=0, FldRollOff=0;
    // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (   RollOff!=DT_BC_S2DEMOD_2132_ROLL_OFF_0_35
        && RollOff!=DT_BC_S2DEMOD_2132_ROLL_OFF_0_25
        && RollOff!=DT_BC_S2DEMOD_2132_ROLL_OFF_0_20
        && RollOff!=DT_BC_S2DEMOD_2132_ROLL_OFF_0_15
        && RollOff!=DT_BC_S2DEMOD_2132_ROLL_OFF_0_10
        && RollOff!=DT_BC_S2DEMOD_2132_ROLL_OFF_0_05)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc);
    
    // No change?
    if (pBc->m_RollOffFactor == RollOff)
        return DT_STATUS_OK;

    // Convert 
    switch (RollOff)
    {
    case DT_BC_S2DEMOD_2132_ROLL_OFF_0_35: FldRollOff = 0; break;
    case DT_BC_S2DEMOD_2132_ROLL_OFF_0_25: FldRollOff = 1; break;
    case DT_BC_S2DEMOD_2132_ROLL_OFF_0_20: FldRollOff = 2; break;
    case DT_BC_S2DEMOD_2132_ROLL_OFF_0_15: FldRollOff = 3; break;
    case DT_BC_S2DEMOD_2132_ROLL_OFF_0_10: FldRollOff = 4; break;
    case DT_BC_S2DEMOD_2132_ROLL_OFF_0_05: FldRollOff = 5; break;
    }

    // Set in register
    RegData = S2DEMOD_RollOff_SET_Direct(0, 1);
    RegData = S2DEMOD_RollOff_SET_Factor(RegData, FldRollOff);
    S2DEMOD_RollOff_WRITE(pBc, RegData);

    // Update cached roll-off
    pBc->m_RollOffFactor = RollOff;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_SetSpectrumInv -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS2DEMOD_2132_SetSpectrumInv(DtBcS2DEMOD_2132* pBc, Int SpecInvCtrl)
{
    UInt32  RegData, FldAuto, FldSpecInv;
    // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (   SpecInvCtrl!=DT_BC_S2DEMOD_2132_SPECINV_AUTO
        && SpecInvCtrl!=DT_BC_S2DEMOD_2132_SPECINV_INVERT
        && SpecInvCtrl!=DT_BC_S2DEMOD_2132_SPECINV_NORMAL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc);
    
    // Convert 
    switch (SpecInvCtrl)
    {
    case DT_BC_S2DEMOD_2132_SPECINV_AUTO:   FldAuto = 1; FldSpecInv=0; break;
    case DT_BC_S2DEMOD_2132_SPECINV_INVERT: FldAuto = 0; FldSpecInv=1; break;
    case DT_BC_S2DEMOD_2132_SPECINV_NORMAL: FldAuto = 0; FldSpecInv=0; break;
    }

    // Set in register
    RegData = S2DEMOD_SpectrumInversion_SET_AutoDetect(0, FldAuto);
    RegData = S2DEMOD_SpectrumInversion_SET_InitialValue(RegData, FldSpecInv);
    S2DEMOD_SpectrumInversion_WRITE(pBc, RegData);

    // Update cached spectrum inversion
    pBc->m_SpectrumInvCtrl = SpecInvCtrl;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_SetSymbolRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS2DEMOD_2132_SetSymbolRate(DtBcS2DEMOD_2132* pBc, Int DecRate, 
                                                         Int SamSymRatio, Int SymSamRatio)
{
    UInt32  FilterSelection = 0;
    UInt32  RegData;

    // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (DecRate!=1 && DecRate!=2 && DecRate!=4 && DecRate!=8 && DecRate!=16 && DecRate!=32
        && DecRate!=64 && DecRate!=128 && DecRate!=256)
        return DT_STATUS_INVALID_PARAMETER;
    if (SamSymRatio<0 || SamSymRatio>=(1<<19))
        return DT_STATUS_INVALID_PARAMETER;
    if (SymSamRatio<0 || SymSamRatio>=(1<<17))
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc);

    // Anti alias Filter carrier selection depends on SampleToSymble ratio
    // See datasheet Creonic.
    // 2..3.5    scaled 131072..229376: filter = 0
    // 3.5..5.0  scaled 229376..327680: filter = 1
    // 3.5..6.5  scaled 327680..425984: filter = 2
    // 6.5..8.0  scaled 425984..524288: filter = 3
    FilterSelection = 0;
    if (SamSymRatio <= 229376)
        FilterSelection = 0;
    else if (SamSymRatio <= 327680)
        FilterSelection = 1;
    else if (SamSymRatio <= 425984)
        FilterSelection = 2;
    else
        FilterSelection = 3;
    
    // Set in register
    S2DEMOD_DecimationRate_WRITE(pBc, (UInt32)DecRate);
    S2DEMOD_InitSampleRateRatio_WRITE(pBc, (UInt32)SymSamRatio);
    RegData = S2DEMOD_InitSymbRateRatio_SET_InitSymbolRateRatio(0, (UInt32)SamSymRatio);
    S2DEMOD_InitSymbRateRatio_WRITE(pBc, RegData);
    RegData = S2DEMOD_AntiAlisasingFilter_SET_FilterSelect(0, FilterSelection);
    S2DEMOD_AntiAlisasingFilter_WRITE(pBc, RegData);

    // Update cached symbol rate
    pBc->m_DecimationRate = DecRate;
    pBc->m_SampleToSymbolRateRatio = SamSymRatio;
    pBc->m_SymbolToSampleRateRatio = SymSamRatio;

    return DT_STATUS_OK;
}
//-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS2DEMOD_2132_SetOperationalMode(DtBcS2DEMOD_2132* pBc, Int OpMode)
{
    UInt32 RegData;

    // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, S2DEMOD_2132, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // IDLE -> RUN
    if (OpMode == DT_BLOCK_OPMODE_RUN)
    {
        DtDbgOutBc(AVG, S2DEMOD_2132, pBc, "OpMode from IDLE -> RUN");

        // Clear resync count
        pBc->m_PlHdrResyncCount = 0;
        // Read PL-header sync
        pBc->m_PlHeaderSync = (S2DEMOD_GlobalStatus_READ_PlHeaderSync(pBc) != 0);
    }
    else
        // RUN->IDLE
        DtDbgOutBc(AVG, S2DEMOD_2132, pBc, "OpMode from RUN -> IDLE");


    // Get spinlock
    DtSpinLockAcquire(&pBc->m_Lock);

    if (OpMode == DT_BLOCK_OPMODE_RUN)
    {
        // Clear interrupt status
        RegData = S2DEMOD_InterruptStatus_SET_Demodulator(0, 1);
        RegData = S2DEMOD_InterruptStatus_SET_FrequencyLoop(RegData, 1);
        RegData = S2DEMOD_InterruptStatus_SET_PlHeaderSync(RegData, 1);
        RegData = S2DEMOD_InterruptStatus_SET_TimingLoop(RegData, 1);
        S2DEMOD_InterruptStatus_WRITE(BC_S2DEMOD_2132, RegData);

        // Enable Creonix interrupts
        RegData = S2DEMOD_InterruptEnable_SET_Demodulator(0, 0);
        RegData = S2DEMOD_InterruptEnable_SET_FrequencyLoop(RegData, 0);
        RegData = S2DEMOD_InterruptEnable_SET_PlHeaderSync(RegData, 1);
        RegData = S2DEMOD_InterruptEnable_SET_TimingLoop(RegData, 0);
        S2DEMOD_InterruptEnable_WRITE(BC_S2DEMOD_2132, RegData);

        // Enable the Creonix interrupt
        DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_S2DEMOD_2132_CREONIX);
    }
    else
    {
        // Disable the Creonix interrupt
        DtBc_InterruptDisable((DtBc*)pBc, DT_INTERRUPT_S2DEMOD_2132_CREONIX);
    }

    // Save new setting
    pBc->m_OperationalMode = OpMode;

    // Release spinlock
    DtSpinLockRelease(&pBc->m_Lock);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_SetPlInformation -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcS2DEMOD_2132_SetPlInformation(DtBcS2DEMOD_2132 * pBc, Int PlMode, 
                                                                           Int PlsValue)
{
     UInt32 RegData, FldPlMode;

    // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (PlMode!=DT_BC_S2DEMOD_2132_PL_MODE_ACM && PlMode!=DT_BC_S2DEMOD_2132_PL_MODE_CCM 
                                                && PlMode!=DT_BC_S2DEMOD_2132_PL_MODE_VCM)
    { 
        DtDbgOutBc(ERR, S2DEMOD_2132, pBc, "Invalid PL information mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    if (PlsValue < 4 || PlsValue>249)
    {
        DtDbgOutBc(ERR, S2DEMOD_2132, pBc, "Invalid PL information PLS value");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc);

    // Convert PlMode
    switch (PlMode)
    {
    case DT_BC_S2DEMOD_2132_PL_MODE_ACM: FldPlMode = 0; break;
    case DT_BC_S2DEMOD_2132_PL_MODE_CCM: FldPlMode = 1; break;
    case DT_BC_S2DEMOD_2132_PL_MODE_VCM: FldPlMode = 2; break;
    }

    // Set in register
    RegData = S2DEMOD_PlInformation_SET_Mode(0, FldPlMode);
    RegData = S2DEMOD_PlInformation_SET_PlsValue(RegData, PlsValue);
    S2DEMOD_PlInformation_WRITE(pBc, RegData);
    
    // Update cache values
    pBc->m_PlMode = PlMode;
    pBc->m_PlsValue = PlsValue;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_SoftReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcS2DEMOD_2132_SoftReset(DtBcS2DEMOD_2132* pBc)
{
    UInt32  RegData;
    // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_S2DEMOD_2132_MUST_BE_ENABLED(pBc);

    // Use MPI register values by doing a softreset
    RegData = S2DEMOD_GlobalControl_SET_SoftReset(0, 1);
    S2DEMOD_GlobalControl_WRITE(pBc, RegData);

    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS2DEMOD_2132 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcS2DEMOD_2132_Init(DtBc*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;
    UInt32 RegData;

    // Sanity checks
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Initialize the spin lock
    DtSpinLockInit(&BC_S2DEMOD_2132->m_Lock);

    // Set defaults
    BC_S2DEMOD_2132->m_BlockEnabled = FALSE;
    BC_S2DEMOD_2132->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    BC_S2DEMOD_2132->m_PlHeaderSync = FALSE;
    BC_S2DEMOD_2132->m_PlHdrResyncCount= 0;

    // Set default PL information register
    // DVB-S2: Rate 1/2, QPSK, pilots, short frame (==PLS value 19), ACM
    BC_S2DEMOD_2132->m_PlMode = 0x00;   // ACM
    BC_S2DEMOD_2132->m_PlsValue = 0x13; // PLS
    RegData = S2DEMOD_PlInformation_SET_PlsValue(0, 0x13);
    RegData = S2DEMOD_PlInformation_SET_Mode(RegData, 0x00);
    S2DEMOD_PlInformation_WRITE(BC_S2DEMOD_2132, RegData);

    // Set default roll-off
    BC_S2DEMOD_2132->m_RollOffFactor = DT_BC_S2DEMOD_2132_ROLL_OFF_0_35;
    // Set in register
    RegData = S2DEMOD_RollOff_SET_Direct(0, 1);
    RegData = S2DEMOD_RollOff_SET_Factor(RegData, 0);
    S2DEMOD_RollOff_WRITE(BC_S2DEMOD_2132, RegData);

    // Set default spectrum inversion
    BC_S2DEMOD_2132->m_SpectrumInvCtrl= DT_BC_S2DEMOD_2132_SPECINV_AUTO;
    // Set in registers
    RegData = S2DEMOD_SpectrumInversion_SET_AutoDetect(0, 1);
    RegData = S2DEMOD_SpectrumInversion_SET_InitialValue(RegData, 0);
    S2DEMOD_SpectrumInversion_WRITE(BC_S2DEMOD_2132, RegData);

    // Set default to 8Msps (sample rate is 80Msps)
    BC_S2DEMOD_2132->m_DecimationRate = 4;   // Gives 20Msps
    BC_S2DEMOD_2132->m_SampleToSymbolRateRatio = 0x28000; // Ratio 2.5 << 16
    BC_S2DEMOD_2132->m_SymbolToSampleRateRatio = 0x0CCCD; // Ratio 0.4 << 17

    // Set in registers
    S2DEMOD_DecimationRate_WRITE(BC_S2DEMOD_2132, 
                                      (UInt32)BC_S2DEMOD_2132->m_DecimationRate);
    S2DEMOD_InitSampleRateRatio_WRITE(BC_S2DEMOD_2132, 
                                      (UInt32)BC_S2DEMOD_2132->m_SymbolToSampleRateRatio);
    RegData = S2DEMOD_InitSymbRateRatio_SET_InitSymbolRateRatio(0, 
                                      (UInt32)BC_S2DEMOD_2132->m_SampleToSymbolRateRatio);
    S2DEMOD_InitSymbRateRatio_WRITE(BC_S2DEMOD_2132, RegData);

    // Perform other initialization of the DEMOD of registers that stay constant.
    DtBcS2DEMOD_2132_SetConstants(BC_S2DEMOD_2132);

    // Disable Creonix interrupts
    RegData = S2DEMOD_InterruptEnable_SET_Demodulator(0, 0);
    RegData = S2DEMOD_InterruptEnable_SET_FrequencyLoop(RegData, 0);
    RegData = S2DEMOD_InterruptEnable_SET_PlHeaderSync(RegData, 0);
    RegData = S2DEMOD_InterruptEnable_SET_TimingLoop(RegData, 0);
    S2DEMOD_InterruptEnable_WRITE(BC_S2DEMOD_2132, RegData);

    // Clear interrupt status
    RegData = S2DEMOD_InterruptStatus_SET_Demodulator(0, 1);
    RegData = S2DEMOD_InterruptStatus_SET_FrequencyLoop(RegData, 1);
    RegData = S2DEMOD_InterruptStatus_SET_PlHeaderSync(RegData, 1);
    RegData = S2DEMOD_InterruptStatus_SET_TimingLoop(RegData, 1);
    S2DEMOD_InterruptStatus_WRITE(BC_S2DEMOD_2132, RegData);

    // Finalize by doing a softreset
    RegData = S2DEMOD_GlobalControl_SET_SoftReset(0, 1);
    S2DEMOD_GlobalControl_WRITE(BC_S2DEMOD_2132, RegData);

    // Init interrupt DPC
    Status = DtDpcInit(&BC_S2DEMOD_2132->m_IntDpc, DtBcS2DEMOD_2132_InterruptDpc, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, S2DEMOD_2132, pBc, "ERROR: failed to init DPC (Status=0x%08X)", 
                                                                                  Status);
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-. 
    Status = DtBcS2DEMOD_2132_RegisterIntHandlers(BC_S2DEMOD_2132);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, S2DEMOD_2132, pBc, "Failed to register interrupt handlers");
        return Status;
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcS2DEMOD_2132_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcS2DEMOD_2132* pBc = (DtBcS2DEMOD_2132*) pBcBase;

    // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    { 
        // DISABLE -> ENABLE' set cached values and enable
        DtDbgOutBc(AVG, S2DEMOD_2132, pBc, "Block Enable from disable -> enable");

    }
    else
    { 
        // ENABLE -> DISABLE' disable and set defaults
        DtDbgOutBc(AVG, S2DEMOD_2132, pBc, "Block Enable from enable -> disable");
        // Operational mode to IDLE
        Status = DtBcS2DEMOD_2132_SetOperationalMode(BC_S2DEMOD_2132,
                                                                    DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DT_ASSERT(FALSE);
            DtDbgOutBc(ERR, S2DEMOD_2132, pBc, "Failed to set opmode to idle");
        }  
    }

    // Save new setting
    pBc->m_BlockEnabled = Enable;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcS2DEMOD_2132_InterruptHandler(DtBc* pBc, Int Id, Int Index, void* pContext)
{
    DtStatus  Status=DT_STATUS_OK;
    DtDpcArgs  DpcArgs;

    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check ID is one of ours
    if (Id != DT_INTERRUPT_S2DEMOD_2132_CREONIX)
    {
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Disable the interrupt that was fired. 
    // NOTE: is re-enabled afte all data is handled by the DPC
    DtBc_InterruptDisable(pBc, Id);

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    DpcArgs.m_Data1.m_UInt32_2 = 0;
    
    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&((DtBcS2DEMOD_2132*)pBc)->m_IntDpc, &DpcArgs);
    DT_ASSERT(DT_SUCCESS(Status));

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_InterruptDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcS2DEMOD_2132_InterruptDpc(DtDpcArgs* pArgs)
{
    UInt32 RegData, RegClearInt;
    DtBcS2DEMOD_2132*  pBc = (DtBcS2DEMOD_2132*)pArgs->m_pContext;
    DtDbgOutBc(MAX, S2DEMOD_2132, pBc, "Creonix demod interrupt");

    // Get spinlock
    DtSpinLockAcquire(&pBc->m_Lock);
    RegClearInt = 0;
    RegData = S2DEMOD_InterruptStatus_READ(pBc);

    // PL-header sync interrupt?
    if (S2DEMOD_InterruptStatus_GET_PlHeaderSync(RegData))
    {
        Bool PlHeaderSync = (S2DEMOD_GlobalStatus_READ_PlHeaderSync(pBc) != 0);
        // Resync?
        if (PlHeaderSync && !pBc->m_PlHeaderSync)
        {
            // Increment resync count
            pBc->m_PlHdrResyncCount++;
        }
        pBc->m_PlHeaderSync = PlHeaderSync;
        // Clear interrupt
        RegClearInt = S2DEMOD_InterruptStatus_SET_PlHeaderSync(RegClearInt, 1);
    }

    // Clear handled interrupts
    S2DEMOD_InterruptStatus_WRITE(pBc, RegClearInt);

    // Check if we need to re-enable the interrupts
    if (pBc->m_OperationalMode==DT_BLOCK_OPMODE_RUN && DtBc_IsEnabled((DtBc*)pBc))
        DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_S2DEMOD_2132_CREONIX);

    // Release spinlock
    DtSpinLockRelease(&pBc->m_Lock);
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcS2DEMOD_2132_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutBc(AVG, S2DEMOD_2132, pBc, "Set operational mode to IDLE");

        // Set operational mode to IDLE
        Status = DtBcS2DEMOD_2132_SetOperationalMode(BC_S2DEMOD_2132,
                                                                    DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, S2DEMOD_2132, pBc, "ERROR: failed to set opmode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcS2DEMOD_2132_RegisterIntHandlers(DtBcS2DEMOD_2132* pBc)
{
    Int  i=0;
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_S2DEMOD_2132_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch(Id)
        {
        case DT_INTERRUPT_S2DEMOD_2132_CREONIX:
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id,
                                                 DtBcS2DEMOD_2132_InterruptHandler, NULL);
            if(!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, S2DEMOD_2132, pBc,
                               "ERROR: failed to register interrupt handler (ID=%d)", Id);
            }
            break;

        default:
            DT_ASSERT(FALSE);      // Unknown ID. New version of the block??
            break;
        }
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132_SetConstants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcS2DEMOD_2132_SetConstants(DtBcS2DEMOD_2132* pBc)
{
    UInt32 RegData;
    RegData = S2DEMOD_DcOffsetWeightFactor_SET_WeightFactor(0, 0x14); // a=2.2
    S2DEMOD_DcOffsetWeightFactor_WRITE(pBc, RegData);

    // Scrambling for broadcasting mode
    RegData = S2DEMOD_ScramblerStartSeq_SET_ScramblerSequence(0, 0x01);
    S2DEMOD_ScramblerStartSeq_WRITE(pBc, RegData);
    // Timing ACQ, normalized bandwith of 10-4
    RegData = S2DEMOD_TimingLoopFiltCoef0_SET_ACQCoefficientG0(0, 0x01EA1D0E); // 25-bits!
    S2DEMOD_TimingLoopFiltCoef0_WRITE(pBc, RegData);
    RegData = S2DEMOD_TimingLoopFiltCoef1_SET_ACQCoefficientG1(0, 0x0015DB7C);
    S2DEMOD_TimingLoopFiltCoef1_WRITE(pBc, RegData);
    // Timing TRK, normalized bandwith of 5  10.5
    RegData = S2DEMOD_TimingLoopFiltCoef2_SET_TRKCoefficientG0(0, 0x01FEE833); // 25-bits!
    S2DEMOD_TimingLoopFiltCoef2_WRITE(pBc, RegData);
    RegData = S2DEMOD_TimingLoopFiltCoef3_SET_TRKCoefficientG1(0, 0x000117C8);
    S2DEMOD_TimingLoopFiltCoef3_WRITE(pBc, RegData);
    // Frequency ACQ, normalized bandwith of 2:3  10-7
    RegData = S2DEMOD_FreqLoopFilterCoef0_SET_ACQCoefficientK1(0, 0x00006B5F);
    S2DEMOD_FreqLoopFilterCoef0_WRITE(pBc, RegData);
    S2DEMOD_FreqLoopFilterCoef1_WRITE_ACQCoefficientK2(pBc, 0x00002D09);
    // Frequency TRK, normalized bandwith of 3  10-8
    RegData = S2DEMOD_FreqLoopFilterCoef2_SET_TRKCoefficientK1(0, 0x00000ABC);
    S2DEMOD_FreqLoopFilterCoef2_WRITE(pBc, RegData);
    S2DEMOD_FreqLoopFilterCoef3_WRITE_TRKCoefficientK2(pBc, 0x00000073);
    // Fine frequency window length = 1500
    S2DEMOD_FineFreqWindowLength_WRITE(pBc, 0x00003fff);
    // Equalizer Step Size = 2-10
    RegData = S2DEMOD_Equalizer_SET_StepSize(0, 3);
    RegData = S2DEMOD_Equalizer_SET_Mode(RegData, 0);
    S2DEMOD_Equalizer_WRITE(pBc, RegData);
    // NDA AGC window length = 8191
    RegData = S2DEMOD_NdaAgcWindowLength_SET_WindowLength(0, 0x00001FFF);
    S2DEMOD_NdaAgcWindowLength_WRITE(pBc, RegData);
    // Filter
    RegData = S2DEMOD_AntiAlisasingFilter_SET_FilterSelect(0, 0);
    S2DEMOD_AntiAlisasingFilter_WRITE(pBc, RegData);
    // SNR 1 frame
    RegData = S2DEMOD_SnrEstimatorFrames_SET_SnrFrames(0, 0);
    S2DEMOD_SnrEstimatorFrames_WRITE(pBc, RegData);
 
    // Creonic non-specified registers D0--94
    S2DEMOD_UndocumentedD4_WRITE(pBc, 0xFFFFFFFF);
    S2DEMOD_UndocumentedD8_WRITE(pBc, 0x0000157C);
    S2DEMOD_UndocumentedDC_WRITE(pBc, 0x000036B0);
    S2DEMOD_UndocumentedE0_WRITE(pBc, 0x3C2707CF);
    S2DEMOD_UndocumentedE4_WRITE(pBc, 0x000F4240);
    S2DEMOD_UndocumentedE8_WRITE(pBc, 0x000A0A03);
    S2DEMOD_UndocumentedEC_WRITE(pBc, 0x00004000);
    S2DEMOD_UndocumentedF0_WRITE(pBc, 0x0000000D);
    S2DEMOD_Undocumented94_WRITE(pBc, 0x00000006);
    S2DEMOD_UndocumentedD0_WRITE(pBc, 0x00000080);
}
