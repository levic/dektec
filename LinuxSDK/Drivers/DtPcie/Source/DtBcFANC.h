//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcFANC.h *#*#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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

#ifndef __DT_BC_FANC_H
#define __DT_BC_FANC_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the Fan controller block (must match block ID)
#define DT_BC_FANC_NAME        "FanCtrl"
#define DT_BC_FANC_SHORTNAME   "FANC"

// MACRO: to init an block-controller-ID for the FANC-BC
#define DT_BC_FANC_INIT_ID(ID, ROLE, INSTANCE, UUID)                                     \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_FANC_NAME, DT_BC_FANC_SHORTNAME, ROLE, INSTANCE, UUID);      \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcFANC definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_BC_FANC_MIN_SPEED     0
#define DT_BC_FANC_DEFAULT_SPEED 100
#define DT_BC_FANC_MAX_SPEED     127


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFANC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcFANC
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    // Cached settings
    Bool  m_HasNoFan;           // Fanless variant
    Int  m_MeasurementPeriod;   // Measurement period in milliseconds
    Int  m_WatchdogTimeout;     // Watchdog timeout in milliseconds
    Int  m_InitialFanSpeed;     // Default/initial fanspeed
    Int  m_FanSpeed;            // Fan speed "percentage" (100% = 127)
}  DtBcFANC;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFANC public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcFANC_Close(DtBc*);
DtBcFANC*  DtBcFANC_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                            const char*  pRole, Int  Instance, Int  Uuid);
DtStatus DtBcFANC_GetConfig(DtBcFANC* pBc, Bool* pHasNoFan, Int* pMeasPeriod,
                                                        Int* pWdTimeout, Int* pInitSpeed);
DtStatus  DtBcFANC_GetFanSpeed(DtBcFANC* pBc,  Int* pFanSpeed);
DtStatus  DtBcFANC_GetStatus(DtBcFANC* pBc,  Int*  pFanSpeedRpm, Bool*  pIsWdTimeout, 
                                                                      Bool*  pIsFanStuck);
DtStatus  DtBcFANC_SetFanSpeed(DtBcFANC* pBc,  Int  FanSpeed);
#endif  // #ifndef __DT_BC_FANC_H
