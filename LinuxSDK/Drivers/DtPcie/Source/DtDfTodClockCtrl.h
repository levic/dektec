// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfTodClockCtrl.h *#*#*#*#*#*#*#*#*# (C) 2021 DekTec
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

#ifndef __DT_DF_TODCLOCKCTRL_H
#define __DT_DF_TODCLOCKCTRL_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcTOD.h"


// Name + short-name for the TODCLOCKCTRL function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_TODCLOCKCTRL_NAME          "TodClockCtrlName"
#define DT_DF_TODCLOCKCTRL_SHORTNAME     "TODCLKCTRL"

// MACRO: to init an driver-function-ID for the TODCLOCKCTRL-DF
#define DT_DF_TODCLOCKCTRL_INIT_ID(ID, ROLE, INSTANCE, UUID)                             \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_TODCLOCKCTRL_NAME, DT_DF_TODCLOCKCTRL_SHORTNAME, ROLE,       \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfTodClockCtrl definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


#define  DT_DF_TODCLOCKCTRL_MAX_NUM_TIME_SAMPLES  1000 // At least 1 second of samples

// -.-.-.-.-.-.-.-.-.-.-.-.- typedef enum _DtDfTodClockCtrlState -.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef enum _DtDfTodClockCtrlState
{
    DT_DF_TODCLOCKCTRL_STATE_INITIAL,
    DT_DF_TODCLOCKCTRL_STATE_CRASH_LOCK,
    DT_DF_TODCLOCKCTRL_STATE_LOCKED,
    DT_DF_TODCLOCKCTRL_STATE_FREE_RUNNING,
    DT_DF_TODCLOCKCTRL_STATE_INVALID_REF
} DtDfTodClockCtrlState;

typedef struct  _DtDfTodClockCtrlTimeSamps
{
    Int64 m_Tod[DT_DF_TODCLOCKCTRL_MAX_NUM_TIME_SAMPLES]; // TOD time periods
    Int64 m_Ref[DT_DF_TODCLOCKCTRL_MAX_NUM_TIME_SAMPLES]; // Reference time periods
    Int  m_BufSize;                 // Sample buffer size
    Int  m_Index;                   // Index of next sample
    Int  m_NumValid;                // Number of valid samples
    Int64 m_PrevTod;                // Previous TOD timestamp
    Int64 m_PrevRef;                // Previous Reference timestamp
    Int64 m_TotalTod;               // Sum of all TOD periods
    Int64 m_TotalRef;               // Sum of all TOD periods
} DtDfTodClockCtrlTimeSamps;

typedef struct _DtDfTodClockCtrlClkControlPars
{
    Int  m_FreqMult;            // Frequency multiplier
    Int  m_PhaseMult;           // Phase multiplier
    Int  m_MaxStepPpt;          // Maximum control step
    Int  m_MaxPhaseDiffNs;      // Maximum phase difference in nanoseconds
    Int  m_MaxDeviationPpm;     // Maximum deviation in ppm
}DtDfTodClockCtrlClkControlPars;

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTodClockCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtDfTodClockCtrl
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // Periodic interval 
    DtSpinLock  m_PerItvSpinLock;   // Spinlock to protect m_PerItvEnable changes
    Bool  m_PerItvEnable;           // Periodic interval hander enabled

    // Control thread
    DtThread  m_ControlThread;      // TOD frequency control thread
    Bool  m_StopControlThread;      // Control thread stop flag
    DtEvent   m_NewTimeSampEvent;   // New timestamp available 
    DtDfTodClockCtrlTimeSamps   m_TimeSamples;
    Int  m_RefDeviationPpm;         // Deviation of reference clock in ppm
    DtDfTodClockCtrlState  m_State; // Control state
    DtDfTodClockCtrlClkControlPars  m_ControlPars;

    // Cached values
    DtCfTod* m_pCfTod;              // The TOD core function 
    Int  m_TodReference;            // TOD reference
    Int64  m_SysClockFreqMilliHz;   // System clock frequency in milli Hz
    Int64  m_TodClockFreqMilliHz;   // Current TOD clock settings in milli Hertz 
}  DtDfTodClockCtrl;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfTodClockCtrl_Close(DtDf*);
DtDfTodClockCtrl*  DtDfTodClockCtrl_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                             Int  Instance,  Int  Uuid, Bool  CreateStub);
DtStatus DtDfTodClockCtrl_GetTodReference(DtDfTodClockCtrl*, Int*);
DtStatus DtDfTodClockCtrl_GetState(DtDfTodClockCtrl*, Int*, Int* , Int*, Int64*, Int64*);
DtStatus DtDfTodClockCtrl_SetTodReference(DtDfTodClockCtrl*, Int);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfTodClockCtrl definitions +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTodClockCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an TODCLOCKCTRL Driver-Function
typedef struct _DtIoStubDfTodClockCtrl
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfTodClockCtrl;

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTodClockCtrl public functions -.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubDfTodClockCtrl_Close(DtIoStub*);
DtIoStubDfTodClockCtrl*  DtIoStubDfTodClockCtrl_Open(DtDf*);


#endif  // #ifndef __DT_DF_TODCLOCKCTRL_H

