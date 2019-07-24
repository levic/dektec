//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcTOD.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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

#ifndef __DT_BC_TOD_H
#define __DT_BC_TOD_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the FpgaReboot block (must match block ID)
#define DT_BC_TOD_NAME        "TimeOfDay"
#define DT_BC_TOD_SHORTNAME   "TOD"

// MACRO: to init an block-controller-ID for the TOD-BC
#define DT_BC_TOD_INIT_ID(ID, ROLE, INSTANCE, UUID)                                      \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_TOD_NAME, DT_BC_TOD_SHORTNAME, ROLE, INSTANCE, UUID);        \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcTOD definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
struct _DtBcTOD
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    DtSpinLock  m_TodLock;      // To prevent time adjust and time get race conditions

    DtDpc  m_IntDpc;            // DPC for periodic interval interrupt
    DtOnPeriodicIntervalFunc  m_OnPeriodicFunc;
    DtObject*  m_pOnPeriodicObject;
    
    // Cached properties and settings
    UInt  m_SysClockFreqHz;     // System clock frequency in Hz
    UInt  m_SysClockAccuracy;   // System clock accuracy in units of 0.1ppm
    UInt32  m_ClockPhaseIncr;   // The clock phase increment
    Int  m_PeriodicInterval;    // Periodic timer interval in milliseconds

};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTOD public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcTOD_Close(DtBc*);
DtBcTOD*  DtBcTOD_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);
DtStatus  DtBcTOD_Adjust(DtBcTOD*, Int64  DeltaNs);
DtStatus  DtBcTOD_GetPeriodicItv(DtBcTOD*, Int* pIntervalMs);
DtStatus  DtBcTOD_GetPhaseIncr(DtBcTOD*, UInt32* pClockPhaseIncr);
DtStatus  DtBcTOD_GetProperties(DtBcTOD*, UInt* pSysClockFreqHz, UInt* pAccuracy);
DtStatus  DtBcTOD_GetTime(DtBcTOD*, DtTodTime*  pTime, UInt* pAdjustCount);
DtStatus  DtBcTOD_SetPhaseIncr(DtBcTOD*, UInt32 ClockPhaseIncr);
DtStatus  DtBcTOD_SetTime(DtBcTOD*, DtTodTime  Time);
DtStatus  DtBcTOD_PeriodicIntervalRegister(DtBcTOD*, DtOnPeriodicIntervalFunc, DtObject*);
DtStatus  DtBcTOD_PeriodicIntervalUnregister(DtBcTOD*);



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcTOD definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTOD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an Time-of-Day Block
typedef struct _DtIoStubBcTOD
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcTOD;


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTOD public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcTOD_Close(DtIoStub*);
DtIoStubBcTOD*  DtIoStubBcTOD_Open(DtBc*);


#endif  // #ifndef __DT_BC_TOD_H
