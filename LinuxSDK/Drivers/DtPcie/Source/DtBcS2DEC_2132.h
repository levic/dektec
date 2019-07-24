//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcS2DEC_2132.h *#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

#ifndef __DT_BC_S2DEC_2132_H
#define __DT_BC_S2DEC_2132_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the DVB-S2X Demodulator From Creonix block (must match block ID)
#define DT_BC_S2DEC_2132_NAME        "S2Decod"
#define DT_BC_S2DEC_2132_SHORTNAME   "S2DEC"

// MACRO: to init an block-controller-ID for the S2DEC_2132-BC
#define DT_BC_S2DEC_2132_INIT_ID(ID, ROLE, INSTANCE, UUID)                             \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_S2DEC_2132_NAME, DT_BC_S2DEC_2132_SHORTNAME,  ROLE,      \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS2DEC_2132 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Roll-off
#define DT_BC_S2DEC_2132_ROLL_OFF_UNKNOWN   0xFF    // Unknown
#define DT_BC_S2DEC_2132_ROLL_OFF_0_35      0x00    // 0.35
#define DT_BC_S2DEC_2132_ROLL_OFF_0_25      0x01    // 0.25
#define DT_BC_S2DEC_2132_ROLL_OFF_0_20      0x02    // 0.20
#define DT_BC_S2DEC_2132_ROLL_OFF_0_15      0x03    // 0.15
#define DT_BC_S2DEC_2132_ROLL_OFF_0_10      0x04    // 0.10
#define DT_BC_S2DEC_2132_ROLL_OFF_0_05      0x05    // 0.05

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEC_2132 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcS2DEC_2132
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    // Cached settings
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_OperationalMode;     // Current operational mode
} DtBcS2DEC_2132;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEC_2132 public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcS2DEC_2132_Close(DtBc*);
DtBcS2DEC_2132*  DtBcS2DEC_2132_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                            const char*  pRole, Int  Instance, Int  Uuid);
DtStatus  DtBcS2DEC_2132_GetRollOff(DtBcS2DEC_2132* pBc, Int* pRollOff);
DtStatus  DtBcS2DEC_2132_GetOperationalMode(DtBcS2DEC_2132* pBc,  Int* pOpMode);
DtStatus  DtBcS2DEC_2132_SetOperationalMode(DtBcS2DEC_2132* pBc,  Int OpMode);
DtStatus  DtBcS2DEC_2132_GetCounters(DtBcS2DEC_2132* pBc, UInt* pFrameCount, 
               UInt* pFrameSkipCount, UInt* pLdcpSuccessCount, UInt* pLdcpIterationsCount,
               UInt* pBchSuccessCount);


#endif  // #ifndef __DT_BC_S2DEC_2132_H
