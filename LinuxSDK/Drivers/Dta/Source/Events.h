//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Events.h *#*#*#*#*#*#*#*#*# (C) 2010-2016 DekTec
//
// Dta driver - Events subsystem - Implements a mechanism to queue events for the user
//
// This module queues all evenst like power events. The events can be requested with the
// DtaEventsWait function. To cancel a pending wait DtaEventsWaitCancel must be called.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2016 DekTec Digital Video B.V.
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

#ifndef __EVENTS_H
#define __EVENTS_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#define  MAX_PENDING_EVENTS 20

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DtaEvent
typedef struct _DtaEvent
{
    UInt  m_EventType;
    UInt  m_EventValue1;
    UInt  m_EventValue2;
} DtaEvent;

// DtaEvents
typedef struct _DtaEvents  DtaEvents;
struct _DtaEvents
{
    DtaEvents*  m_pNext;
    DtaEvents*  m_pPrev;
    Int  m_RefCount;            // >1 == InUse by someone
                                // 1 == Not inuse by someone
                                // 0 == Not in use
    DtSpinLock  m_Lock;         // Used to protect the m_PendingEvents
    DtFileObject  m_File;       // DtFileObject to identify user
    UInt  m_EventTypeMask;      // Mask of event types to be notified of
    volatile UInt  m_NumPendingEvents;
                                // Number of pending events in the pending events struct
    DtaEvent  m_PendingEvents[MAX_PENDING_EVENTS];
                                // Contains the pending events

    DtEvent  m_PendingEvent;    // Used to trigger wait function when events become
                                // available during wait.
    Bool  m_CancelInProgress;   // TRUE if the DtaEventsGetWait() must be cancelled
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaEventsInit(DtaDeviceData* pDvcData);
DtStatus  DtaEventsCleanup(DtaDeviceData* pDvcData);
DtStatus  DtaEventsGet(DtaDeviceData* pDvcData, DtFileObject* pFile, DtaEvents* pEvents,
                                          UInt* pEventType, UInt* pValue1, UInt* pValue2);
DtStatus  DtaEventsGetCancel(DtaDeviceData* pDvcData, DtFileObject* pFile);
DtStatus  DtaEventsRegister(DtaDeviceData* pDvcData, DtFileObject* pFile,
                                                                      UInt EventTypeMask);
DtStatus  DtaEventsSet(DtaDeviceData* pDvcData, DtFileObject* pFile, UInt EventType,
                                                                UInt Value1, UInt Value2);
void  DtaEventsUnregister(DtaDeviceData* pDvcData, DtFileObject* pFile);

#endif // __EVENTS_H
