// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfGenLockCtrl.h *#*#*#*#*#*#*#*#*# (C) 2019 DekTec
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

#ifndef __DT_DF_GENLOCKCTRL_H
#define __DT_DF_GENLOCKCTRL_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcGENL.h"
#include "DtDfSi534X.h"
#include "DtDfGenLockCtrl_CallbackFunction.h"

// Name + short-name for the GENLOCKCTRL function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_GENLOCKCTRL_NAME          "SdiGenLockCtrl"
#define DT_DF_GENLOCKCTRL_SHORTNAME     "GENLOCKCTRL"

// MACRO: to init an driver-function-ID for the GENLOCKCTRL-DF
#define DT_DF_GENLOCKCTRL_INIT_ID(ID, ROLE, INSTANCE, UUID)                              \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_GENLOCKCTRL_NAME, DT_DF_GENLOCKCTRL_SHORTNAME, ROLE,         \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define  DT_DF_GENLOCKCTRL_START_OF_FRAME_OFFSET (-50000) // Start-of-Frame time offset
                                                          // in nano seconds

#define  DT_DF_GENLOCKCTRL_MAX_NUM_FRAME_PERIODS  60 // Maximum 1 second of samples
#define  DT_DF_GENLOCKCTRL_SOFTOD_SIZE      3       // Number of start-of-frame timestamps
                                                    // saved

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrlDcoState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef enum _DtDfGenLockCtrlDcoState
{
    DT_DF_GENLOCKCTRL_STATE_INITIAL,
    DT_DF_GENLOCKCTRL_STATE_FREE_RUNNING,
    DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_FREQ,
    DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_FREQ_CHECK,
    DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_PHASE,
    DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_PHASE_WAIT_FOR_DONE,
    DT_DF_GENLOCKCTRL_STATE_LOCKED,
} DtDfGenLockCtrlDcoState;

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrlGenRefStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef enum DtDfGenLockCtrlGenRefStatus
{
    DT_DF_GENLOCKCTRL_GENREFSTATUS_OK,
    DT_DF_GENLOCKCTRL_GENREFSTATUS_NO_SIGNAL,
    DT_DF_GENLOCKCTRL_GENREFSTATUS_INVALID_SIGNAL,
} DtDfGenLockCtrlGenRefStatus;

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrlSofTods -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtDfGenLockCtrlSofTods
{
    DtTodTime  m_SofTods[DT_DF_GENLOCKCTRL_SOFTOD_SIZE]; // Last start-of-frame timestamps
    Int64  m_TotalSofTods;      // Total number of start-of-frames
    Int  m_SofTodIdx;           // Next index to write start-of-frame timestamp
    Int  m_LastFramePeriod;     // Last frame period
}DtDfGenLockCtrlSofTods;

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrlFramePeriodMeasure -.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtDfGenLockCtrlFramePeriodMeasure
{
    Int m_FramePeriods[DT_DF_GENLOCKCTRL_MAX_NUM_FRAME_PERIODS]; // Frame period samples
    Int  m_BufSize;                 // Sample buffer size
    Int  m_Index;                   // Index of next sample
    Int  m_NumValid;                // Number of valid samples
    Int64 m_TotalFramePeriods;      // Sum of all frame periods
} DtDfGenLockCtrlFramePeriodMeasure;

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrlDcoControlPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtDfGenLockCtrlDcoControlPars
{
    Int  m_FreqMult;            // Frequency multiplier
    Int  m_PhaseMult;           // Phase multiplier
    Int  m_MaxStepPpt;          // Maximum control step
    Int  m_MaxPhaseDiffNs;      // Maximum phase difference in nanoseconds
}DtDfGenLockCtrlDcoControlPars;

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrlStatusChangedFunc -.-.-.-.-.-.-.-.-.-.-.-.-.-
// Signature of the GenLock lock changed callback function
typedef void  (*DtDfGenLockCtrlLockChangedFunc)(DtObject*, Bool Locked);

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrlStatusChangedRegData -.-.-.-.-.-.-.-.-.-.-.-.-
// Registration data for GenLock lock changed callback
typedef struct  _DtDfGenLockCtrlLockChangedRegData
{
    DtObject*  m_pObject;       // DtBc or DtDf object registering for the event
    DtDfGenLockCtrlLockChangedFunc  m_OnLockChangedFunc;  // Callback function
}  DtDfGenLockCtrlLockChangedRegData;

DtStatus DtDfGenLockCtrl_LockChangedRegister(DtDfGenLockCtrl*, 
                                                const DtDfGenLockCtrlLockChangedRegData*);
DtStatus DtDfGenLockCtrl_LockChangedUnregister(DtDfGenLockCtrl*, const DtObject*);


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtDfGenLockCtrl
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;
    
    // Genref start-of-frame event information protected by m_SofSpinLock
    DtDfGenLockCtrlSofTods  m_GenRefSofTods; // Reference start-of-frame timestamp 
    DtDfGenLockCtrlFramePeriodMeasure  m_GenRefFpMeasure;  // Frame period measurement
    Int  m_GenRefPortIndex;                 // Port index
    Int  m_GenRefVidStd;                    // Video standard
    Int  m_GenRefFramePeriod;               // Video standard frame period in ns
    DtDfGenLockCtrlGenRefType m_GenRefType; // Genref type
    Bool  m_GenRefParChanged;               // Change in genref parameters
    Int  m_GenRefDetectVidStd;              // Detected video standard


    DtBcGENL*  m_pBcGenLock;                  // GenLock block-controller
    DtTodTime  m_GenLockSofTod;               // GENL start-of-frame timestamp 
                                              // protected by m_SofSpinLock

    // Driver function
    DtDfSi534X* m_pDfSi534X;                  // Si534X Clock Oscillator
    const DtDfSi534XClockProps*  m_pSi534XClockProps;   // Clock properties
    Int  m_NumSi534XClockProps;

    // Start-of-frame handling
    DtSpinLock  m_SofSpinLock;      // Spinlock to protect Operational Mode and SofTods

    // DCO-control thread for synchronisation
    DtThread  m_DcoControlThread;   // DCO-control thread
    Bool  m_StopDcoControlThread;   // DCO-control stop flag
    DtEvent  m_DcoControlSofEvent;  // DCO-control new start-of-frame event
    DtDfGenLockCtrlDcoState  m_DcoControlState;  // DCO-control state
    DtDfGenLockCtrlGenRefStatus  m_GenRefStatus;  // GenRef signal status
    DtDfGenLockCtrlDcoControlPars  m_DcoControlPars; // Dco control parameter
    Int  m_Count;                   // For debugging
    Int  m_DcoFreqOffsetPpt;        // DcoFrequency offset in parts per trilion

    DtVector*  m_pOnLockChangedHandlers; // List of registered on genlock lock changed
                                         // handlers
    // Cached values
    Bool m_GenLockControlEnabled;   // Genlocking is enabled
    Int  m_VideoStandard;           // Current used video standard
    Int  m_FrameLength;             // Frame length in clock ticks
    Int  m_FramePeriod;             // Frame period in nanoseconds
    Bool  m_FractionalClock;        // Fractional clock is used
}  DtDfGenLockCtrl;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfGenLockCtrl_Close(DtDf*);
DtDfGenLockCtrl*  DtDfGenLockCtrl_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                             Int  Instance,  Int  Uuid, Bool  CreateStub);
DtStatus  DtDfGenLockCtrl_GetGenLockStatus(DtDfGenLockCtrl*, Int* pGenLockState,
                                                        Int* pRefVidStd, Int* pDetVidStd);
DtStatus  DtDfGenLockCtrl_IsLocked(DtDfGenLockCtrl*, Bool* pLocked);
DtStatus  DtDfGenLockCtrl_ReLock(DtDfGenLockCtrl*);
DtStatus  DtDfGenLockCtrl_GetDcoClockProperties(DtDfGenLockCtrl*, Int, Int*,
                                                           DtIoctlGenLockCtrlClockProps*);
DtStatus  DtDfGenLockCtrl_GetDcoFreqOffset(DtDfGenLockCtrl*, Int, Int*, Int64*);
DtStatus  DtDfGenLockCtrl_SetDcoFreqOffset(DtDfGenLockCtrl*, Int, Int);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfGenLockCtrl definitions +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfGenLockCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an GENLOCKCTRL Driver-Function
typedef struct _DtIoStubDfGenLockCtrl
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfGenLockCtrl;

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfGenLockCtrl public functions -.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubDfGenLockCtrl_Close(DtIoStub*);
DtIoStubDfGenLockCtrl*  DtIoStubDfGenLockCtrl_Open(DtDf*);

#endif  // #ifndef __DT_DF_GENLOCKCTRL_H

