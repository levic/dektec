// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQAGC.c *#*#*#*#*#*#*#*#*#*#* (C) 2022 DekTec
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
#include "DtBc.h"
#include "DtBcIQAGC.h"
#include "DtBcIQAGC_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQAGC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQAGC_DEFAULT_PRECONDITIONS(pBc)                                              \
                                    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQAGC))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQAGC_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQAGC, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQAGC_Init(DtBc*);
static DtStatus  DtBcIQAGC_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIQAGC_OnEnable(DtBc*, Bool);
static void  DtBcIQAGC_SetControlRegs(DtBcIQAGC*, Bool, Int , Int, Int, Bool, Int);

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQAGC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcIQAGC_Close(DtBc* pBc)
{
    BC_IQAGC_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcIQAGC*  DtBcIQAGC_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_IQAGC_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQAGC, Id, DT_BLOCK_TYPE_IQAGC,
        Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQAGC_Close;
    OpenParams.m_InitFunc = DtBcIQAGC_Init;
    OpenParams.m_OnEnableFunc = DtBcIQAGC_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIQAGC_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQAGC*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC_GetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQAGC_GetConfig(DtBcIQAGC* pBc, Int* pGainCorr1, Int* pGainCorr2,
                                                Int* pMaxDacValue1, Int* pMaxDacValue2,
                                                Int* pMinDacValue1, Int* pMinDacValue2,
                                                Int* pDacNumBits, Int* pIqBitDepth)
{
    UInt32 RegValue;

    // Sanity check
    BC_IQAGC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pGainCorr1==NULL || pGainCorr2==NULL || pMaxDacValue1==NULL || pMaxDacValue2==NULL
                                             || pMinDacValue1==NULL || pMinDacValue2==NULL
                                             || pDacNumBits==NULL || pIqBitDepth==NULL)
        return DT_STATUS_INVALID_PARAMETER;
    // Must be enabled
    BC_IQAGC_MUST_BE_ENABLED(pBc);

    // Read configuration data
    RegValue = IQAGC_Config1_READ(pBc);
    if (IQAGC_Config1_GET_GainCorrelation1(RegValue) == IQAGC_CORRELATION_POSITIVE)
        *pGainCorr1 = DT_IQAGC_CORRELATION_POSITIVE;
    else
        *pGainCorr1 = DT_IQAGC_CORRELATION_NEGATIVE;
    if (IQAGC_Config1_GET_GainCorrelation2(RegValue) == IQAGC_CORRELATION_POSITIVE)
        *pGainCorr2 = DT_IQAGC_CORRELATION_POSITIVE;
    else
        *pGainCorr2 = DT_IQAGC_CORRELATION_NEGATIVE;
    *pDacNumBits = IQAGC_Config1_GET_DacNumBits(RegValue);
    *pIqBitDepth = IQAGC_Config1_GET_IqBitDepth(RegValue);
    RegValue = IQAGC_Config2_READ(pBc);
    *pMinDacValue1 = IQAGC_Config2_GET_MinDacValue1(RegValue);
    *pMaxDacValue1 = IQAGC_Config2_GET_MaxDacValue1(RegValue);
    RegValue = IQAGC_Config3_READ(pBc);
    *pMinDacValue2 = IQAGC_Config3_GET_MinDacValue2(RegValue);
    *pMaxDacValue2 = IQAGC_Config3_GET_MaxDacValue2(RegValue);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC_GetGainControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQAGC_GetGainControl(DtBcIQAGC* pBc, Int* pUseManualGain, Int* pManualGain)
{
    // Sanity check
    BC_IQAGC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pUseManualGain==NULL || pManualGain==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQAGC_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pUseManualGain = (Int)pBc->m_UseManualGain;
    *pManualGain = pBc->m_ManualGain;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC_GetGainCtrlPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQAGC_GetGainCtrlPars(DtBcIQAGC* pBc, Int* pSignalType, 
                                Int* pGainCorrelation,
                                Int* pMaxDacValue, Int* pMinDacValue,
                                Int* pStepSizeUnlocked, Int* pStepSizeLocked,
                                Int* pThresholdHigh, Int* pThresholdLow, Int* pMeasPeriod)
{
    // Sanity check
    BC_IQAGC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pSignalType==NULL || pGainCorrelation==NULL || pMaxDacValue==NULL 
                 || pMinDacValue==NULL || pStepSizeUnlocked==NULL || pStepSizeLocked==NULL
                 || pThresholdHigh==NULL || pThresholdLow==NULL || pMeasPeriod==NULL)
        return DT_STATUS_INVALID_PARAMETER;
    // Must be enabled
    BC_IQAGC_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pSignalType = pBc->m_SignalType;
    *pGainCorrelation = pBc->m_GainCorrelation;
    *pMaxDacValue = pBc->m_MaxDacValue;
    *pMinDacValue = pBc->m_MinDacValue;
    *pStepSizeUnlocked = pBc->m_StepSizeUnlocked;
    *pStepSizeLocked = pBc->m_StepSizeLocked;
    *pThresholdHigh = pBc->m_ThresholdHigh;
    *pThresholdLow = pBc->m_ThresholdLow;
    *pMeasPeriod = pBc->m_MeasurementPeriod;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC_GetGainStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQAGC_GetGainStatus(DtBcIQAGC* pBc, Int* pLocked, Int* pGain)
{
    UInt32 RegValue;

    // Sanity check
    BC_IQAGC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pLocked==NULL || pGain==NULL)
        return DT_STATUS_INVALID_PARAMETER;
    // Must be enabled
    BC_IQAGC_MUST_BE_ENABLED(pBc);

    // Read status
    RegValue = IQAGC_Status_READ(pBc);
    *pLocked = (IQAGC_Status_GET_Lock(RegValue)!=0) ? TRUE : FALSE;
    *pGain = IQAGC_Status_GET_GainReadback(RegValue);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQAGC_GetOperationalMode(DtBcIQAGC* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IQAGC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQAGC_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC_SetGainControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQAGC_SetGainControl(DtBcIQAGC* pBc, Int UseManualGain, Int ManualGain)
{
    // Sanity check
    BC_IQAGC_DEFAULT_PRECONDITIONS(pBc);

     // Must be enabled
    BC_IQAGC_MUST_BE_ENABLED(pBc);

    // Parameter check
    if (UseManualGain!=FALSE && UseManualGain!=TRUE)
        return DT_STATUS_INVALID_PARAMETER;
    if (ManualGain<0 || ManualGain>0xFFFF)
        return DT_STATUS_INVALID_PARAMETER;

    // Cache values
    pBc->m_UseManualGain = (Bool)UseManualGain;
    pBc->m_ManualGain = ManualGain;

    // Set control register
    DtBcIQAGC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                          pBc->m_SignalType, pBc->m_GainCorrelation, pBc->m_UseManualGain,
                          pBc->m_ManualGain);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC_SetGainCtrlPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQAGC_SetGainCtrlPars(DtBcIQAGC* pBc, Int SignalType, Int GainCorrelation,
                                      Int MaxDacValue, Int MinDacValue,
                                      Int StepSizeUnlocked, Int StepSizeLocked,
                                      Int ThresholdHigh, Int ThresholdLow, Int MeasPeriod)
{
    UInt32 RegValue;

    // Sanity check
    BC_IQAGC_DEFAULT_PRECONDITIONS(pBc);

     // Must be enabled
    BC_IQAGC_MUST_BE_ENABLED(pBc);

    // Parameter check
    if (SignalType!=DT_IQAGC_SIGNALTYPE_REAL && SignalType!=DT_IQAGC_SIGNALTYPE_COMPLEX)
        return DT_STATUS_INVALID_PARAMETER;
    if (GainCorrelation!=DT_IQAGC_CORRELATION_POSITIVE
                                        && GainCorrelation!=DT_IQAGC_CORRELATION_NEGATIVE)
        return DT_STATUS_INVALID_PARAMETER;
    if (MaxDacValue<0 || MaxDacValue>0xFFFF)
        return DT_STATUS_INVALID_PARAMETER;
    if (MinDacValue<0 || MinDacValue>0xFFFF)
        return DT_STATUS_INVALID_PARAMETER;
    if (StepSizeUnlocked<0 || StepSizeUnlocked>0xFF)
        return DT_STATUS_INVALID_PARAMETER;
    if (StepSizeLocked<0 || StepSizeLocked>0xFF)
        return DT_STATUS_INVALID_PARAMETER;
    if (ThresholdHigh<0 || ThresholdHigh>0x3FFFF)
        return DT_STATUS_INVALID_PARAMETER;
    if (ThresholdLow<0 || ThresholdLow>0x3FFFF)
        return DT_STATUS_INVALID_PARAMETER;
    if (MeasPeriod<0 || MeasPeriod>0xFFFF)
        return DT_STATUS_INVALID_PARAMETER;

    // Cache values
    pBc->m_SignalType = SignalType;
    pBc->m_GainCorrelation = GainCorrelation;
    pBc->m_MaxDacValue = MaxDacValue;
    pBc->m_MinDacValue = MinDacValue;
    pBc->m_StepSizeUnlocked = StepSizeUnlocked;
    pBc->m_StepSizeLocked = StepSizeLocked;
    pBc->m_ThresholdHigh = ThresholdHigh;
    pBc->m_ThresholdLow = ThresholdLow;
    pBc->m_MeasurementPeriod = MeasPeriod;

    // Set control register
    DtBcIQAGC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                          pBc->m_SignalType, pBc->m_GainCorrelation, pBc->m_UseManualGain,
                          pBc->m_ManualGain);
    // DacControl
    RegValue = IQAGC_DacControl_READ(pBc);
    RegValue = IQAGC_DacControl_SET_MaxDacValue(RegValue, pBc->m_MaxDacValue);
    RegValue = IQAGC_DacControl_SET_MinDacValue(RegValue , pBc->m_MinDacValue);
    IQAGC_DacControl_WRITE(pBc, RegValue);

    // GainStepSize
    RegValue = IQAGC_GainStepSize_READ(pBc);
    RegValue = IQAGC_GainStepSize_SET_Unlocked(RegValue, pBc->m_StepSizeUnlocked);
    RegValue = IQAGC_GainStepSize_SET_Locked(RegValue , pBc->m_StepSizeLocked);
    IQAGC_GainStepSize_WRITE(pBc, RegValue);

    // ThresholdHigh
    IQAGC_ThresholdHigh_WRITE(pBc, pBc->m_ThresholdHigh);
    
    // ThresholdLow
    IQAGC_ThresholdLow_WRITE(pBc, pBc->m_ThresholdLow);

    // MeasurementPeriod
    IQAGC_MeasurementPeriod_WRITE(pBc, pBc->m_MeasurementPeriod);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQAGC_SetOperationalMode(DtBcIQAGC* pBc, Int OpMode)
{
    // Sanity check
    BC_IQAGC_DEFAULT_PRECONDITIONS(pBc);


    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
        && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQAGC_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;

    // Set control register
    DtBcIQAGC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                          pBc->m_SignalType, pBc->m_GainCorrelation, pBc->m_UseManualGain,
                          pBc->m_ManualGain);
    return DT_STATUS_OK;
}

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQAGC - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQAGC_Init(DtBc* pBcBase)
{
    UInt32 RegValue;
    DtBcIQAGC* pBc = (DtBcIQAGC*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_SignalType = DT_IQAGC_SIGNALTYPE_REAL;
    pBc->m_GainCorrelation = DT_IQAGC_CORRELATION_POSITIVE;
    pBc->m_UseManualGain = FALSE;
    pBc->m_ManualGain = 0;
    pBc->m_MaxDacValue = 0;
    pBc->m_MinDacValue = 0;
    pBc->m_StepSizeUnlocked = 0;
    pBc->m_StepSizeLocked = 0;
    pBc->m_ThresholdHigh = 0;
    pBc->m_ThresholdLow = 0;
    pBc->m_MeasurementPeriod = 0;

    // Set control register
    DtBcIQAGC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                          pBc->m_SignalType, pBc->m_GainCorrelation, pBc->m_UseManualGain,
                          pBc->m_ManualGain);
    // DacControl
    RegValue = IQAGC_DacControl_READ(pBc);
    RegValue = IQAGC_DacControl_SET_MaxDacValue(RegValue, pBc->m_MaxDacValue);
    RegValue = IQAGC_DacControl_SET_MinDacValue(RegValue , pBc->m_MinDacValue);
    IQAGC_DacControl_WRITE(pBc, RegValue);

    // GainStepSize
    RegValue = IQAGC_GainStepSize_READ(pBc);
    RegValue = IQAGC_GainStepSize_SET_Unlocked(RegValue, pBc->m_StepSizeUnlocked);
    RegValue = IQAGC_GainStepSize_SET_Locked(RegValue , pBc->m_StepSizeLocked);
    IQAGC_GainStepSize_WRITE(pBc, RegValue);

    // ThresholdHigh
    IQAGC_ThresholdHigh_WRITE(pBc, pBc->m_ThresholdHigh);

    // ThresholdLow
    IQAGC_ThresholdLow_WRITE(pBc, pBc->m_ThresholdLow);

    // MeasurementPeriod
    IQAGC_MeasurementPeriod_WRITE(pBc, pBc->m_MeasurementPeriod);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQAGC_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQAGC* pBc = (DtBcIQAGC*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IQAGC_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IQAGC, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIQAGC_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQAGC, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQAGC_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQAGC* pBc = (DtBcIQAGC*)pBcBase;

    // Sanity check
    BC_IQAGC_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IQAGC, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IQAGC, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcIQAGC_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQAGC, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIQAGC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                          pBc->m_SignalType, pBc->m_GainCorrelation, pBc->m_UseManualGain,
                          pBc->m_ManualGain);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQAGC_SetControlRegs(DtBcIQAGC* pBc, Bool BlkEna, Int OpMode, Int SignalType,
                                         Int GainCorr, Bool UseManualGain, Int ManualGain)
{
    UInt32  RegData = 0, FwBlkEna = 0, FwOpMode = 0, FwGainCorr = 0, FwSignalType = 0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IQAGC_BLKENA_ENABLED : IQAGC_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = IQAGC_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = IQAGC_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = IQAGC_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Convert signal type to BB-type
    switch (SignalType)
    {
    case DT_IQAGC_SIGNALTYPE_REAL:    FwSignalType = IQAGC_SIGTYPE_REAL; break;
    case DT_IQAGC_SIGNALTYPE_COMPLEX: FwSignalType = IQAGC_SIGTYPE_COMPLEX; break;
    default: DT_ASSERT(FALSE);
    }

    // Convert gain correlation to BB-type
    switch (GainCorr)
    {
    case DT_IQAGC_CORRELATION_POSITIVE: FwGainCorr = IQAGC_CORRELATION_POSITIVE; break;
    case DT_IQAGC_CORRELATION_NEGATIVE: FwGainCorr = IQAGC_CORRELATION_NEGATIVE; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = IQAGC_Control_READ(pBc);
    RegData = IQAGC_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IQAGC_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = IQAGC_Control_SET_SignalType(RegData, FwSignalType);
    RegData = IQAGC_Control_SET_GainCorrelation(RegData, FwGainCorr);
    RegData = IQAGC_Control_SET_UseManualGain(RegData, UseManualGain ? 1 : 0);
    RegData = IQAGC_Control_SET_ManualGain(RegData, (UInt32)ManualGain);
    IQAGC_Control_WRITE(pBc, RegData);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQAGC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IQAGC_STUB_DEFAULT_PRECONDITIONS(pStub)                                          \
                          DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQAGC))
// Helper macro to cast stub's DtBc member to DtBcIQAGC
#define IQAGC_STUB   ((DtIoStubBcIQAGC*)pStub)
#define IQAGC_BC  ((DtBcIQAGC*)IQAGC_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQAGC_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);

static DtStatus  DtIoStubBcIQAGC_OnCmdGetConfig(const DtIoStubBcIQAGC*,
                                                         DtIoctlIqAgcCmdGetConfigOutput*);
static DtStatus  DtIoStubBcIQAGC_OnCmdGetGainControl(const DtIoStubBcIQAGC*,
                                                    DtIoctlIqAgcCmdGetGainControlOutput*);
static DtStatus  DtIoStubBcIQAGC_OnCmdGetGainCtrlPars(const DtIoStubBcIQAGC*,
                                                   DtIoctlIqAgcCmdGetGainCtrlParsOutput*);
static DtStatus  DtIoStubBcIQAGC_OnCmdGetGainStatus(const DtIoStubBcIQAGC*,
                                                   DtIoctlIqAgcCmdGetGainStatusOutput*);
static DtStatus  DtIoStubBcIQAGC_OnCmdGetOperationalMode(const DtIoStubBcIQAGC*,
                                                         DtIoctlIqAgcCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcIQAGC_OnCmdSetGainControl(const DtIoStubBcIQAGC*,
                                               const DtIoctlIqAgcCmdSetGainControlInput*);
static DtStatus  DtIoStubBcIQAGC_OnCmdSetGainCtrlPars(const DtIoStubBcIQAGC*,
                                              const DtIoctlIqAgcCmdSetGainCtrlParsInput*);
static DtStatus  DtIoStubBcIQAGC_OnCmdSetOperationalMode(const DtIoStubBcIQAGC*,
                                                    const DtIoctlIqAgcCmdSetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQAGC;

static const DtIoctlProperties  IOSTUB_BC_IQAGC_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQAGC_CMD(
        DtIoStubBcIQAGC_OnCmd,
        NULL, NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQAGC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQAGC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcIQAGC_Close(DtIoStub* pStub)
{
    IQAGC_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQAGC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcIQAGC*  DtIoStubBcIQAGC_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IQAGC_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQAGC, pBc, NULL,
        DtIoStubBcIQAGC_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_IQAGC_IOCTLS);
    return (DtIoStubBcIQAGC*)DtIoStubBc_Open(&OpenParams);
}

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQAGC - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQAGC_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQAGC_OnCmd(const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqAgcCmdInput*  pInData = NULL;
    DtIoctlIqAgcCmdOutput*  pOutData = NULL;

    IQAGC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQAGC_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQAGC, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqAgcCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqAgcCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IQAGC_CMD_GET_CONFIG:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQAGC_OnCmdGetConfig(IQAGC_STUB, &pOutData->m_GetConfig);
        break;
    case DT_IQAGC_CMD_GET_GAIN_CONTROL:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQAGC_OnCmdGetGainControl(IQAGC_STUB,
                                                             &pOutData->m_GetGainControl);
        break;
    case DT_IQAGC_CMD_GET_GAIN_CTRL_PARS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQAGC_OnCmdGetGainCtrlPars(IQAGC_STUB, 
                                                                &pOutData->m_GetCtrlPars);
        break;
    case DT_IQAGC_CMD_GET_GAIN_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQAGC_OnCmdGetGainStatus(IQAGC_STUB, &pOutData->m_GetStatus);
        break;
    case DT_IQAGC_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQAGC_OnCmdGetOperationalMode(IQAGC_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_IQAGC_CMD_SET_GAIN_CONTROL:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQAGC_OnCmdSetGainControl(IQAGC_STUB, 
                                                              &pInData->m_SetGainControl);
        break;
    case DT_IQAGC_CMD_SET_GAIN_CTRL_PARS:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQAGC_OnCmdSetGainCtrlPars(IQAGC_STUB, 
                                                                 &pInData->m_SetCtrlPars);
        break;
    case DT_IQAGC_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQAGC_OnCmdSetOperationalMode(IQAGC_STUB,
                                                                   &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQAGC_OnCmdGetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQAGC_OnCmdGetConfig(const DtIoStubBcIQAGC* pStub,
                                             DtIoctlIqAgcCmdGetConfigOutput* pOutData)
{
    IQAGC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQAGC_GetConfig(IQAGC_BC, &pOutData->m_GainCorrelation1,
                                                            &pOutData->m_GainCorrelation2,
                                                            &pOutData->m_MaxDacValue1,
                                                            &pOutData->m_MaxDacValue2,
                                                            &pOutData->m_MinDacValue1,
                                                            &pOutData->m_MinDacValue2,
                                                            &pOutData->m_DacNumBits,
                                                            &pOutData->m_IqBitDepth);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQAGC_OnCmdGetGainControl -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQAGC_OnCmdGetGainControl(const DtIoStubBcIQAGC* pStub,
                                             DtIoctlIqAgcCmdGetGainControlOutput* pOutData)
{
    IQAGC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQAGC_GetGainControl(IQAGC_BC, &pOutData->m_UseManualGain,
                                                                 &pOutData->m_ManualGain);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQAGC_OnCmdGetGainCtrlPars -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQAGC_OnCmdGetGainCtrlPars(const DtIoStubBcIQAGC* pStub,
                                             DtIoctlIqAgcCmdGetGainCtrlParsOutput* pOutData)
{
    IQAGC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQAGC_GetGainCtrlPars(IQAGC_BC,  &pOutData->m_SignalType,
                                                          &pOutData->m_GainCorrelation,
                                                          &pOutData->m_MaxDacValue,
                                                          &pOutData->m_MinDacValue,
                                                          &pOutData->m_StepSizeUnlocked,
                                                          &pOutData->m_StepSizeLocked,
                                                          &pOutData->m_ThresholdHigh,
                                                          &pOutData->m_ThresholdLow,
                                                          &pOutData->m_MeasurementPeriod);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQAGC_OnCmdGetGainStatus -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQAGC_OnCmdGetGainStatus(const DtIoStubBcIQAGC* pStub,
                                             DtIoctlIqAgcCmdGetGainStatusOutput* pOutData)
{
    IQAGC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQAGC_GetGainStatus(IQAGC_BC, &pOutData->m_Locked, 
                                                               &pOutData->m_GainReadback);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQAGC_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQAGC_OnCmdGetOperationalMode(
    const DtIoStubBcIQAGC* pStub,
    DtIoctlIqAgcCmdGetOpModeOutput* pOutData)
{
    IQAGC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQAGC_GetOperationalMode(IQAGC_BC, &pOutData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQAGC_OnCmdSetGainControl -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQAGC_OnCmdSetGainControl(const DtIoStubBcIQAGC* pStub,
                                        const DtIoctlIqAgcCmdSetGainControlInput* pInData)
{
    IQAGC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQAGC_SetGainControl(IQAGC_BC, pInData->m_UseManualGain,
                                                                  pInData->m_ManualGain);


}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQAGC_OnCmdSetGainCtrlPars -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQAGC_OnCmdSetGainCtrlPars(const DtIoStubBcIQAGC* pStub,
                                       const DtIoctlIqAgcCmdSetGainCtrlParsInput* pInData)
{
    IQAGC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQAGC_SetGainCtrlPars(IQAGC_BC,  pInData->m_SignalType,
                                                         pInData->m_GainCorrelation,
                                                         pInData->m_MaxDacValue,
                                                         pInData->m_MinDacValue,
                                                         pInData->m_StepSizeUnlocked,
                                                         pInData->m_StepSizeLocked,
                                                         pInData->m_ThresholdHigh,
                                                         pInData->m_ThresholdLow,
                                                         pInData->m_MeasurementPeriod);

}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQAGC_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQAGC_OnCmdSetOperationalMode(
    const DtIoStubBcIQAGC* pStub,
    const DtIoctlIqAgcCmdSetOpModeInput* pInData)
{
    IQAGC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQAGC_SetOperationalMode(IQAGC_BC, pInData->m_OpMode);
}



