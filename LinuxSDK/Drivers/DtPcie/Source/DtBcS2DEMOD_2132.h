//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcS2DEMOD_2132.h *#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

#ifndef __DT_BC_S2DEMOD_2132_H
#define __DT_BC_S2DEMOD_2132_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the DVB-S2X Demodulator From Creonix block (must match block ID)
#define DT_BC_S2DEMOD_2132_NAME        "S2Demod"
#define DT_BC_S2DEMOD_2132_SHORTNAME   "S2DEMOD"

// MACRO: to init an block-controller-ID for the S2DEMOD_2132-BC
#define DT_BC_S2DEMOD_2132_INIT_ID(ID, ROLE, INSTANCE, UUID)                             \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_S2DEMOD_2132_NAME, DT_BC_S2DEMOD_2132_SHORTNAME,  ROLE,      \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS2DEMOD_2132 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Roll-off factor
#define DT_BC_S2DEMOD_2132_ROLL_OFF_0_35    0x00    // 0.35
#define DT_BC_S2DEMOD_2132_ROLL_OFF_0_25    0x01    // 0.25
#define DT_BC_S2DEMOD_2132_ROLL_OFF_0_20    0x02    // 0.20
#define DT_BC_S2DEMOD_2132_ROLL_OFF_0_15    0x03    // 0.15
#define DT_BC_S2DEMOD_2132_ROLL_OFF_0_10    0x04    // 0.10
#define DT_BC_S2DEMOD_2132_ROLL_OFF_0_05    0x05    // 0.05

// Spectrum Inversion
#define DT_BC_S2DEMOD_2132_SPECINV_AUTO    0        // Auto spectrum inversion detection
#define DT_BC_S2DEMOD_2132_SPECINV_NORMAL  1        // Normal spectrum
#define DT_BC_S2DEMOD_2132_SPECINV_INVERT  2        // Inverted spectrum

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcS2DEMOD_2132
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    DtDpc  m_IntDpc;
    DtSpinLock  m_Lock;         // Lock to protect against mutual access of 
                                // m_OperationalMode
    // Cached settings
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_OperationalMode;     // Current operational mode
    Int  m_RollOffFactor;       // Roll-off factor
    Int  m_SpectrumInvCtrl;     // Spectum inversion control
    Int  m_DecimationRate;      // Decimation rate for symbolrate setting
    Int  m_SampleToSymbolRateRatio; // Sample to symbol-rate ratio
    Int  m_SymbolToSampleRateRatio; // Symbol to sample-rate ratio
    UInt  m_PlHdrResyncCount;   // PL-header re-sync count
    Bool  m_PlHeaderSync;       // PL-header sync
} DtBcS2DEMOD_2132;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEMOD_2132 public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcS2DEMOD_2132_Close(DtBc*);
DtBcS2DEMOD_2132*  DtBcS2DEMOD_2132_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                            const char*  pRole, Int  Instance, Int  Uuid);
DtStatus  DtBcS2DEMOD_2132_GetOperationalMode(DtBcS2DEMOD_2132* pBc,  Int* pOpMode);
DtStatus  DtBcS2DEMOD_2132_GetFreqOffset(DtBcS2DEMOD_2132* pBc, Int* pCoarseFreq,
                                                   Int* pFineFreq, Int* pDecimationRate);
DtStatus  DtBcS2DEMOD_2132_GetRollOff(DtBcS2DEMOD_2132* pBc, Int* pRollOff);
DtStatus  DtBcS2DEMOD_2132_GetSnrInfo(DtBcS2DEMOD_2132* pBc, UInt* pSigPower, 
                                                                         UInt* pErrPower);
DtStatus  DtBcS2DEMOD_2132_GetSpectrumInv(DtBcS2DEMOD_2132* pBc, Int* pSpecInvCtrl);
DtStatus  DtBcS2DEMOD_2132_GetStatus(DtBcS2DEMOD_2132* pBc,  Bool* pSignalDetect, 
                              Bool* pSpecInv,  Bool* pPlHdrSync, UInt* pPlHdrResyncCount);
DtStatus  DtBcS2DEMOD_2132_GetSymbolRate(DtBcS2DEMOD_2132* pBc, Int* pDecRate, 
                            Int* pSamSymRatio,  Int* pSymSamRatio, Int* pMeasSamSymRatio);
DtStatus  DtBcS2DEMOD_2132_SetOperationalMode(DtBcS2DEMOD_2132* pBc,  Int OpMode);
DtStatus  DtBcS2DEMOD_2132_SetRollOff(DtBcS2DEMOD_2132* pBc, Int RollOff);
DtStatus  DtBcS2DEMOD_2132_SetSpectrumInv(DtBcS2DEMOD_2132* pBc, Int SpecInvCtrl);
DtStatus  DtBcS2DEMOD_2132_SetSymbolRate(DtBcS2DEMOD_2132* pBc, Int DecRate, 
                                                       Int SamSymRatio,  Int SymSamRatio);
DtStatus  DtBcS2DEMOD_2132_SoftReset(DtBcS2DEMOD_2132* pBc);


#endif  // #ifndef __DT_BC_S2DEMOD_2132_H