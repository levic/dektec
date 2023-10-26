// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCfTod.h *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

#ifndef __DT_CF_TOD_H
#define __DT_CF_TOD_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcTOD.h"

// Name + short-name for the TIME core-function
#define DT_CF_TOD_NAME          "TimeOfDay"
#define DT_CF_TOD_SHORTNAME     "TOD"

// MACRO: to init an driver-function-ID for the TIME-CF
#define DT_CF_TOD_INIT_ID(ID, ROLE, INSTANCE, UUID)                                      \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_CF_TOD_NAME, DT_CF_TOD_SHORTNAME, ROLE, INSTANCE, UUID);        \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfTod definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
struct _DtCfTod
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    DtVector*  m_pOnPeriodicHandlers;  
                                // List of registered on-periodic-interval-event handlers

    DtBcTOD*  m_pBcTod;         // Shortcut to the ToD-BC

};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtCfTod_Close(DtDf*);
DtCfTod*  DtCfTod_Open(DtCore*, const char*  pRole, Int  Instance, 
                                                             Int  Uuid, Bool  CreateStub);
DtStatus  DtCfTod_AdjustTime(DtCfTod*,  Int64 DeltaNanoseconds);
DtStatus  DtCfTod_GetAdjustPpb(DtCfTod*, Int* pAdjustPpb);
DtStatus  DtCfTod_GetPeriodicItv(DtCfTod*, Int* pIntervalMs);
DtStatus  DtCfTod_GetPhaseIncr(DtCfTod*, UInt* pClockPhaseIncr);
DtStatus  DtCfTod_GetProperties(DtCfTod*, UInt* pSysClockFreqHz, UInt* pAccuracy);
DtStatus  DtCfTod_GetTime(DtCfTod*, DtTodTime* pTime, UInt* pAdjustCount);
DtStatus  DtCfTod_SetPhaseIncr(DtCfTod*, UInt  ClockPhaseIncr);
DtStatus  DtCfTod_SetTime(DtCfTod*, DtTodTime  Time);
DtStatus  DtCfTod_PeriodicIntervalRegister(DtCfTod*, const DtOnPeriodicIntervalRegData*);
DtStatus  DtCfTod_PeriodicIntervalUnregister(DtCfTod*, const DtObject*);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCfTod definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfTod -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for a ToD Core-Function
typedef struct _DtIoStubCfTod
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubCfTod;

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfTod - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubCfTod_Close(DtIoStub*);
DtIoStubCfTod*  DtIoStubCfTod_Open(DtDf*);

#endif  // #ifndef __DT_CF_TOD_H
