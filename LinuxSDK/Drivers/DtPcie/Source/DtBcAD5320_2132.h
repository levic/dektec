//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcAD5320_2132.h *#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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

#ifndef __DT_BC_AD5320_2132_H
#define __DT_BC_AD5320_2132_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the DacAd5320Ctrl block (must match block ID)
#define DT_BC_AD5320_2132_NAME        "DacAd5320Ctrl"
#define DT_BC_AD5320_2132_SHORTNAME   "AD5320"

// MACRO: to init an block-controller-ID for the AD5320_2132-BC
#define DT_BC_AD5320_2132_INIT_ID(ID, ROLE, INSTANCE, UUID)                              \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_AD5320_2132_NAME, DT_BC_AD5320_2132_SHORTNAME,  ROLE,        \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcAD5320_2132 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD5320_2132 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcAD5320_2132
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    // Cached settings
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_OperationalMode;     // Current operational mode
    Int  m_GainControl;         // Gain control: AUTO/MANUAL
    Int  m_StepSizeUnlocked;    // Gain control step size when not locked
    Int  m_StepSizeLocked;      // Gain control step size when  locked
    Int  m_OverflowThreshold;   // Signed threshold value for detecting overflows
    Int  m_WorkingThreshold;    // Signed threshold value for detecting working level
    UInt  m_TimeWindow;         // Time window for the gain measurement
    Int  m_ManualGain;          // Value to manually overrule the gain
} DtBcAD5320_2132;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD5320_2132 public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcAD5320_2132_Close(DtBc*);
DtBcAD5320_2132*  DtBcAD5320_2132_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcAD5320_2132_GetGainControl(DtBcAD5320_2132* pBc, Int* pGainControl,
                                                                        Int* pManualGain);
DtStatus  DtBcAD5320_2132_GetGainCtrlPars(DtBcAD5320_2132* pBc, Int* pStepSizeUnlocked, 
                                            Int* pStepSizeLocked, Int* pOverflowThreshold,
                                            Int* pWorkingThreshold, UInt* pTimeWindow);
DtStatus  DtBcAD5320_2132_GetGainStatus(DtBcAD5320_2132* pBc, Int* pStatus, Int* pGain);
DtStatus  DtBcAD5320_2132_GetOperationalMode(DtBcAD5320_2132* pBc,  Int* pOpMode);
DtStatus  DtBcAD5320_2132_SetGainControl(DtBcAD5320_2132* pBc, Int GainControl, 
                                                                          Int ManualGain);
DtStatus  DtBcAD5320_2132_SetGainCtrlPars(DtBcAD5320_2132* pBc, Int StepSizeUnlocked, 
                                                Int StepSizeLocked, Int OverflowThreshold,
                                                Int WorkingThreshold, UInt TimeWindow);
DtStatus  DtBcAD5320_2132_SetOperationalMode(DtBcAD5320_2132* pBc,  Int OpMode);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcAD5320_2132 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD5320_2132 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an AD5320_2132 Block
typedef struct _DtIoStubBcAD5320_2132
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcAD5320_2132;


//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD5320_2132 public functions -.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcAD5320_2132_Close(DtIoStub*);
DtIoStubBcAD5320_2132*  DtIoStubBcAD5320_2132_Open(DtBc*);


#endif  // #ifndef __DT_BC_AD5320_2132_H