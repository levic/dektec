// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCfEvt.h *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

#ifndef __DT_CF_EVT_H
#define __DT_CF_EVT_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"

// Name + short-name for the EVENTS core-function
#define DT_CF_EVT_NAME          "Events"
#define DT_CF_EVT_SHORTNAME     "EVT"

// MACRO: to init an driver-function-ID for the EVENTS-CF
#define DT_CF_EVT_INIT_ID(ID, ROLE, INSTANCE, UUID)                                      \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_CF_EVT_NAME, DT_CF_EVT_SHORTNAME, ROLE, INSTANCE, UUID);        \
}                                                                                        \
while (0)

// Max. number of pending events
#define DT_CF_EVT_MAX_PENDING_EVENTS   20

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDriverEvents -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  DtVector  DtVectorEvent;
typedef struct  _DtDriverEvents  DtDriverEvents;
struct  _DtDriverEvents
{
    DtDriverEvents*  m_pNext;
    DtDriverEvents*  m_pPrev;
    Int  m_RefCount;            // >1 == InUse by someone
                                // 1 == Not inuse by someone
                                // 0 == Not in use (can be freed)
    DtSpinLock  m_Lock;         // Used to protect the m_PendingEvents
    DtFileObject  m_File;       // DtFileObject to identify user
    UInt  m_EventTypeMask;      // Mask of event types to be notified of

    DtVectorEvent*  m_PendingEvents; // Contains the pending events

    DtEvent  m_PendingEvent;    // Used to trigger wait function when events become
                                // available during wait.
    Bool  m_CancelInProgress;   // TRUE if the DtaEventsGetWait() must be cancelled

};


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfEvt definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
struct _DtCfEvt
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;
    DtCfEvtData*  m_pEvtData;    // Reference to the event data; Do not free!!
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtCfEvt_Close(DtDf*);
DtCfEvt*  DtCfEvt_Open(DtCore*, const char*  pRole, Int  Instance, 
                                                             Int  Uuid, Bool  CreateStub);
void  DtCfEvt_CleanupEventsData(DtCfEvtData* pEvtData);
DtCfEvtData*  DtCfEvt_CreateEventsData(void);

DtStatus  DtCfEvt_Set(DtCfEvt*, DtDriverEvent, Bool AddIfExists);
#ifdef LINBUILD
UInt      DtCfEvt_Poll(DtCfEvt*, DtFileObject*, poll_table*);
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCfEvt definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfEvt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for a EVENTS Core-Function
typedef struct _DtIoStubCfEvt
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
    
    // Windows specific
#ifdef WINBUILD
    DtMutex m_EventQueueMutex;      // Access protection for event queue
    WDFQUEUE m_EventQueue;          // Event queue
#endif
}  DtIoStubCfEvt;

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfEvt - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubCfEvt_Close(DtIoStub*);
DtIoStubCfEvt*  DtIoStubCfEvt_Open(DtDf*);

#endif  // #ifndef __DT_CF_EVT_H
