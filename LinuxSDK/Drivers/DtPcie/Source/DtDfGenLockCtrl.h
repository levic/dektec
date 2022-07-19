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

// Start-of-Frame time offset in nano seconds; this is about half the range of the
// GenlockSofDelay in the TXPHY
#define  DT_DF_GENLOCKCTRL_START_OF_FRAME_OFFSET (220000)
#define  DT_DF_GENLOCKCTRL_MAX_NUM_FRAME_PERIODS  60 // Maximum 1 second of samples
#define  DT_DF_GENLOCKCTRL_SOFTOD_SIZE      3       // Number of start-of-frame timestamps
                                                    // saved

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_DcoState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DCO clock control state
//
typedef enum _DtDfGenLockCtrl_DcoState
{
    DT_DF_GENLOCKCTRL_STATE_INITIAL,
    DT_DF_GENLOCKCTRL_STATE_FREE_RUNNING,
    DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_FREQ,
    DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_FREQ_CHECK,
    DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_PHASE,
    DT_DF_GENLOCKCTRL_STATE_CRASH_LOCK_PHASE_WAIT_FOR_DONE,
    DT_DF_GENLOCKCTRL_STATE_LOCKED,
} DtDfGenLockCtrl_DcoState;

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_GenRefStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef enum DtDfGenLockCtrl_GenRefStatus
{
    DT_DF_GENLOCKCTRL_GENREFSTATUS_OK,
    DT_DF_GENLOCKCTRL_GENREFSTATUS_NO_SIGNAL,
    DT_DF_GENLOCKCTRL_GENREFSTATUS_INVALID_SIGNAL,
} DtDfGenLockCtrl_GenRefStatus;

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_GenRefSofTods -.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtDfGenLockCtrl_GenRefSofTods
{
    DtTodTime  m_SofTods[DT_DF_GENLOCKCTRL_SOFTOD_SIZE]; // Last start-of-frame timestamps
    Int  m_NumSofTods;          // Number of start-of-frames stored
    Int  m_SofTodIdx;           // Next index to write start-of-frame timestamp
    Int  m_LastFramePeriod;     // Last frame period
    Int64  m_TotalNumSofTods;   // Total number of start-of-frames
}DtDfGenLockCtrl_GenRefSofTods;

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_FramePeriodMeasure -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Frame period measurement data
//
typedef struct  _DtDfGenLockCtrl_FramePeriodMeasure
{
    Int m_FramePeriods[DT_DF_GENLOCKCTRL_MAX_NUM_FRAME_PERIODS]; // Frame period samples
    Int  m_BufSize;                 // Sample buffer size
    Int  m_Index;                   // Index of next sample
    Int  m_NumValid;                // Number of valid samples
    Int64 m_TotalFramePeriods;      // Sum of all frame periods
} DtDfGenLockCtrl_FramePeriodMeasure;

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_DcoControlPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtDfGenLockCtrl_DcoControlPars
{
    Int  m_FreqMult;            // Frequency multiplier
    Int  m_PhaseMult;           // Phase multiplier
    Int  m_MaxStepPpt;          // Maximum control step
    Int  m_MaxPhaseDiffNs;      // Maximum phase difference in nanoseconds
}DtDfGenLockCtrl_DcoControlPars;

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_LockChangedFunc -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Signature of the GenLock lock changed callback function
typedef void  (*DtDfGenLockCtrl_LockChangedFunc)(DtObject*, Bool Locked);

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_LockChangedRegData -.-.-.-.-.-.-.-.-.-.-.-.-.
// Registration data for GenLock lock changed callback
typedef struct  _DtDfGenLockCtrlLockChangedRegData
{
    DtObject*  m_pObject;       // DtBc or DtDf object registering for the event
    DtDfGenLockCtrl_LockChangedFunc  m_OnLockChangedFunc;  // Callback function
}  DtDfGenLockCtrl_LockChangedRegData;

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_ControlData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtDfGenLockCtrl_ControlData
{
    DtBcGENL*  m_pBcGenLock;        // GenLock block-controller

    // 2 GENL start-of-frame timestamps protected by m_SofSpinLock
    // For genref there are 3 start-of-frame timestamps stored. This means that for the
    // oldest GENL SOF we can find a nearest genref SOF that can be used for controlling.
    Bool m_NewSofTodStored;
    DtTodTime  m_GenLockSofTod[2];
    DtDfGenLockCtrl_DcoState  m_DcoControlState;  // DCO-control state
    Int  m_DcoFreqOffsetPpt;        // DcoFrequency offset in parts per trillion
    Int  m_LockCount;               // For lock checking and debugging
    Int  m_PhaseDiffNs;             // Phase difference in nano seconds when
                                    // m_DcoControlState == DT_DF_GENLOCKCTRL_STATE_LOCKED

    // Cached values set on DtDfGenLockCtrl_SetVideoStandard() 
    DtDfGenLockCtrl_DcoControlPars  m_DcoControlPars; // Dco control parameter
    Bool m_GenLockControlEnabled;   // Genlocking is enabled
    Int  m_VideoStandard;           // Current used video standard
    Int  m_FrameLength;             // Frame length in clock ticks
    Int  m_FramePeriod;             // Frame period in nanoseconds
    Bool  m_FractionalClock;        // Fractional clock is used
}DtDfGenLockCtrl_ControlData;

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl_GenRefData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtDfGenLockCtrl_GenRefData
{
    // Genref start-of-frame event information protected by m_SofSpinLock
    DtDfGenLockCtrl_GenRefSofTods  m_GenRefSofTods; // Reference start-of-frame timestamp 
    DtDfGenLockCtrl_FramePeriodMeasure  m_GenRefFpMeasure;  // Frame period measurement
    Int  m_GenRefPortIndex;                 // Port index
    Int  m_GenRefVidStd;                    // Video standard
    Int  m_GenRefFramePeriod;               // Video standard frame period in ns
    Int  m_GenRefStartOfFrameOffset;        // Global start-of-frame offset in nanoseconds
                                            // set through the genref port and IoCfg
    DtDfGenLockCtrl_GenRefType m_GenRefType; // Genref type
    Bool  m_GenRefParChanged;               // Change in genref parameters
    Int  m_GenRefDetectVidStd;              // Detected video standard
    DtDfGenLockCtrl_GenRefStatus  m_GenRefStatus;  // GenRef signal status
}DtDfGenLockCtrl_GenRefData;

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfGenLockCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
struct  _DtDfGenLockCtrl
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // Driver function
    DtDfSi534X* m_pDfSi534X;                // Si534X Clock Oscillator
    const DtDfSi534XClockProps*  m_pSi534XClockProps;   // Clock properties
    Int  m_NumSi534XClockProps;

    // Start-of-frame handling
    DtSpinLock  m_SofSpinLock;          // Spinlock to protect OpMode and SofTods

    // DCO-control thread for synchronization
    DtThread  m_DcoControlThread;       // DCO-control thread
    Bool  m_StopDcoControlThread;       // DCO-control stop flag
    DtEvent  m_DcoControlSofEvent;      // DCO-control new start-of-frame event

    DtVector*  m_pOnLockChangedHandlers; // List of registered on genlock lock changed
                                         // handlers

    DtDfGenLockCtrl_GenRefData   m_GenRefData;  // GenRef data
    DtDfGenLockCtrl_ControlData  m_Genl[2];     // GenLock control data;
                                                // primary and secondary
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfGenLockCtrl_Close(DtDf*);
DtDfGenLockCtrl*  DtDfGenLockCtrl_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                             Int  Instance,  Int  Uuid, Bool  CreateStub);
DtStatus  DtDfGenLockCtrl_GetGenLockState(DtDfGenLockCtrl*, Int* pGenLockState,
                    Int* pRefVidStd, Int* pDetVidStd, Int64* pSofCount,
                    Int* pIsSofTodValid, DtTodTime* pLastSofTod, Int* pTimeSinceLastSof);
DtStatus  DtDfGenLockCtrl_IsLocked(DtDfGenLockCtrl*, Bool* pLocked);
DtStatus  DtDfGenLockCtrl_ReLock(DtDfGenLockCtrl*);
DtStatus  DtDfGenLockCtrl_GetDcoClockProperties(DtDfGenLockCtrl*, Int, Int*,
                                                           DtIoctlGenLockCtrlClockProps*);
DtStatus  DtDfGenLockCtrl_GetDcoFreqOffset(DtDfGenLockCtrl*, Int, Int*, Int64*);
DtStatus  DtDfGenLockCtrl_GetStartOfFrameOffset(DtDfGenLockCtrl*, Int*);
DtStatus  DtDfGenLockCtrl_SetDcoFreqOffset(DtDfGenLockCtrl*, Int, Int);
DtStatus  DtDfGenLockCtrl_SetStartOfFrameOffset(DtDfGenLockCtrl*, Int);
DtStatus  DtDfGenLockCtrl_LockChangedRegister(DtDfGenLockCtrl*, 
                                               const DtDfGenLockCtrl_LockChangedRegData*);
DtStatus  DtDfGenLockCtrl_LockChangedUnregister(DtDfGenLockCtrl*, const DtObject*);

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

