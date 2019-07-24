//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfS2CrDemod_2132.h *#*#*#*#*#*#*#*#* (C) 2017 DekTec
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

#ifndef __DT_DF_S2CRDEMOD_2132_H
#define __DT_DF_S2CRDEMOD_2132_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcS2DEC_2132.h"
#include "DtBcS2DEMOD_2132.h"

// Name + short-name for the S2CRDEMOD function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_S2CRDEMOD_2132_NAME          "S2CrDemod"
#define DT_DF_S2CRDEMOD_2132_SHORTNAME     "S2CRDEMOD"

// MACRO: to init an driver-function-ID for the S2CRDEMOD-DF
#define DT_DF_S2CRDEMOD_2132_INIT_ID(ID, ROLE, INSTANCE, UUID)                           \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_S2CRDEMOD_2132_NAME, DT_DF_S2CRDEMOD_2132_SHORTNAME, ROLE,   \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfS2CrDemod_2132 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfS2CrDemod_2132 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtDfS2CrDemod_2132
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // Block controllers
    DtBcS2DEC_2132*  m_pBcS2Dec;        // S2 Decoder
    DtBcS2DEMOD_2132* m_pBcS2Demod;     // S2 Demodulator
    DtSpinLock  m_OpModeSpinLock;       // Lock to protect operational mode
    // Cached values
    Int  m_OperationalMode;     // Operational mode
}  DtDfS2CrDemod_2132;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfS2CrDemod_2132_Close(DtDf*);
DtDfS2CrDemod_2132*  DtDfS2CrDemod_2132_Open(DtCore*, DtPt*  pPt, const char*  pRole, 
                                              Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtDfS2CrDemod_2132_GetCounters(DtDfS2CrDemod_2132* pBc, UInt* pFrameCount, 
               UInt* pFrameSkipCount, UInt* pLdcpSuccessCount, UInt* pLdcpIterationsCount,
               UInt* pBchSuccessCount);
DtStatus  DtDfS2CrDemod_2132_GetFreqOffset(DtDfS2CrDemod_2132* pDf, Int* pCoarseFreq,
                                                    Int* pFineFreq, Int* pDecimationRate);
DtStatus  DtDfS2CrDemod_2132_GetOperationalMode(DtDfS2CrDemod_2132* pBc,  Int* pOpMode);
DtStatus  DtDfS2CrDemod_2132_GetRollOff(DtDfS2CrDemod_2132* pBc, Int* pRollOff);
DtStatus  DtDfS2CrDemod_2132_GetSnrInfo(DtDfS2CrDemod_2132* pBc, UInt* pSigPower, 
                                                                         UInt* pErrPower);
DtStatus  DtDfS2CrDemod_2132_GetSpectrumInv(DtDfS2CrDemod_2132* pBc, Int* pSpecInvCtrl);
DtStatus  DtDfS2CrDemod_2132_GetStatus(DtDfS2CrDemod_2132* pBc,  Bool* pSignalDetect, 
                              Bool* pSpecInv,  Bool* pPlHdrSync, UInt* pPlHdrResyncCount);
DtStatus  DtDfS2CrDemod_2132_GetSymbolRate(DtDfS2CrDemod_2132* pBc, Int* pDecRate, 
                            Int* pSamSymRatio,  Int* pSymSamRatio, Int* pMeasSamSymRatio);
DtStatus  DtDfS2CrDemod_2132_SetOperationalMode(DtDfS2CrDemod_2132* pBc,  Int OpMode);
DtStatus  DtDfS2CrDemod_2132_SetSpectrumInv(DtDfS2CrDemod_2132* pBc, Int SpecInvCtrl);
DtStatus  DtDfS2CrDemod_2132_SetSymbolRate(DtDfS2CrDemod_2132* pBc, Int DecRate, 
                                                       Int SamSymRatio,  Int SymSamRatio);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfS2CrDemod_2132 definitions +=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_2132 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an S2CRDEMOD Driver-Function
typedef struct _DtIoStubDfS2CrDemod_2132
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfS2CrDemod_2132;

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfS2CrDemod_2132 public functions -.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubDfS2CrDemod_2132_Close(DtIoStub*);
DtIoStubDfS2CrDemod_2132*  DtIoStubDfS2CrDemod_2132_Open(DtDf*);

#endif  // #ifndef __DT_DF_S2CRDEMOD_2132_H

