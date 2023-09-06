// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQAGC.h *#*#*#*#*#*#*#*#*#*#* (C) 2022 DekTec
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

#ifndef __DT_BC_IQAGC_H
#define __DT_BC_IQAGC_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the IqAgc block (must match block ID)
#define DT_BC_IQAGC_NAME        "IqAgc"
#define DT_BC_IQAGC_SHORTNAME   "IQAGC"

// MACRO: to init an block-controller-ID for the IQAGC-BC
#define DT_BC_IQAGC_INIT_ID(ID, ROLE, INSTANCE, UUID)                                    \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_IQAGC_NAME, DT_BC_IQAGC_SHORTNAME, ROLE, INSTANCE, UUID);    \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQAGC definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcIQAGC
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached settings
    Bool  m_BlockEnabled;
    Int  m_OperationalMode;

    // Control parameters
    Bool m_UseManualGain;       // Gain control: AUTO/MANUAL
    Int  m_ManualGain ;         // Value to manually overrule the gain
    Int  m_SignalType;          // Input signal type Real/Complex
    Int  m_GainCorrelation;     // Correlation between voltage and gain
    Int  m_MaxDacValue;         // Maximum DAC value
    Int  m_MinDacValue;         // Minimum DAC value
    Int  m_StepSizeUnlocked;    // Gain control step size when not locked
    Int  m_StepSizeLocked;      // Gain control step size when  locked
    Int  m_ThresholdHigh;       // Threshold value for the upper side of the working area
    Int  m_ThresholdLow;        // Threshold value for the lower side of the working area
    Int  m_MeasurementPeriod;   // Time period for the gain measurement in us
}  DtBcIQAGC;

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQAGC public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

void  DtBcIQAGC_Close(DtBc*);
DtBcIQAGC*  DtBcIQAGC_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);
DtStatus  DtBcIQAGC_GetConfig(DtBcIQAGC* pBc, Int* pGainCorr1, Int* pGainCorr2,
                                            Int* pMaxDacValue1, Int* pMaxDacValue2,
                                            Int* pMinDacValue1, Int* pMinDacValue2,
                                            Int* pDacNumBits, Int* pIqBitDepth);
DtStatus  DtBcIQAGC_GetGainControl(DtBcIQAGC* pBc, Int* pUseManualGain, Int* pManualGain);
DtStatus  DtBcIQAGC_GetGainCtrlPars(DtBcIQAGC* pBc, Int* pSignalType, 
                                            Int* pGainCorrelation,
                                            Int* pMaxDacValue, Int* pMinDacValue,
                                            Int* pStepSizeUnlocked, Int* pStepSizeLocked,
                                            Int* pThresholdHigh, Int* pThresholdLow,
                                            Int* pMeasPeriod);
DtStatus  DtBcIQAGC_GetGainStatus(DtBcIQAGC* pBc, Int* pLocked, Int* pGain);
DtStatus  DtBcIQAGC_GetOperationalMode(DtBcIQAGC* pBc,  Int* pOpMode);
DtStatus  DtBcIQAGC_SetGainControl(DtBcIQAGC* pBc, Int UseManualGain, Int ManualGain);
DtStatus  DtBcIQAGC_SetGainCtrlPars(DtBcIQAGC* pBc, Int SignalType, Int GainCorrelation,
                                                 Int MaxDacValue, Int MinDacValue,
                                                 Int StepSizeUnlocked, Int StepSizeLocked,
                                                 Int ThresholdHigh, Int ThresholdLow,
                                                 Int MeasPeriod);
DtStatus  DtBcIQAGC_SetOperationalMode(DtBcIQAGC* pBc,  Int OpMode);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQAGC definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQAGC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an IQAGC Block
typedef struct _DtIoStubBcIQAGC
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcIQAGC;

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQAGC public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcIQAGC_Close(DtIoStub*);
DtIoStubBcIQAGC*  DtIoStubBcIQAGC_Open(DtBc*);

#endif  // #ifndef __DT_BC_IQAGC_H
