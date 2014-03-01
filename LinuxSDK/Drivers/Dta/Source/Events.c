//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Events.c *#*#*#*#*#*#*#*#*# (C) 2010-2012 DekTec
//
// Dta driver - Events subsystem - Implements a mechanism to queue events for the user
//
// This module queues all evenst like power events. The events can be requested with the
// DtaEventsWait function. To cancel a pending wait DtaEventsWaitCancel must be called.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2012 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//  3. The source code may not be modified for the express purpose of enabling hardware
//     features for which no genuine license has been obtained.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtaIncludes.h>


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEventsGetEventsObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtaEvents* DtaEventsGetEventsObject(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile)
{
    Int  i;
    DtaEvents*  pDtaEvents = NULL;

    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
    {
        DtSpinLockAcquire(&pDvcData->m_Events[i].m_Lock);
        if (pDvcData->m_Events[i].m_RefCount > 0)
        {
            if (DtFileCompare(&pDvcData->m_Events[i].m_File, pFile))
            {
                pDtaEvents = &pDvcData->m_Events[i];

                // Increment refcount
                pDtaEvents->m_RefCount++;
            }
        }
        DtSpinLockRelease(&pDvcData->m_Events[i].m_Lock);

        if (pDtaEvents != NULL)
            break;
    }

    return pDtaEvents;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEventsAllocEventsObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtaEvents* DtaEventsAllocEventsObject(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFileHandle,
    UInt  EventTypeMask)
{
    Int  i;
    DtaEvents*  pDtaEvents = NULL;
    
    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
    {
        DtSpinLockAcquire(&pDvcData->m_Events[i].m_Lock);
        if (pDvcData->m_Events[i].m_RefCount == 0)
        {
            pDtaEvents = &pDvcData->m_Events[i];

            pDtaEvents->m_RefCount = 2;
                                // Start with 2 to prevent removal after first unref
            pDtaEvents->m_File = *pFileHandle;
            pDtaEvents->m_EventTypeMask = EventTypeMask;
            pDtaEvents->m_CancelInProgress = FALSE;
            pDtaEvents->m_NumPendingEvents = 0;
            memset(&pDtaEvents->m_PendingEvents, 0, sizeof(pDtaEvents->m_PendingEvents));
        }
        DtSpinLockRelease(&pDvcData->m_Events[i].m_Lock);

        if (pDtaEvents != NULL)
            break;
    }

    return pDtaEvents;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEventsUnrefEventsObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static void  DtaEventsUnrefEventsObject(DtaDeviceData* pDvcData, DtaEvents* pDtaEvents)
{
    DtSpinLockAcquire(&pDtaEvents->m_Lock);
    if (pDtaEvents->m_RefCount > 0)
        pDtaEvents->m_RefCount--;
    DtSpinLockRelease(&pDtaEvents->m_Lock);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEventsSetEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaEventsSetEvent(
    DtaEvents*  pDtaEvents,
    UInt  EventType,
    UInt  Value1,
    UInt  Value2)
{
    DtStatus  Result = DT_STATUS_OK;
    DtaEvent*  pDtaEvent;

    // Skip unregistered events
    if ((EventType & pDtaEvents->m_EventTypeMask) != 0)
    {
        DtSpinLockAcquire(&pDtaEvents->m_Lock);

        // Add new events
        if (pDtaEvents->m_NumPendingEvents == MAX_PENDING_EVENTS)
        {
            // Remove oldest event
            DtDbgOut(AVG, EVENTS, "Max. number of events. Remove old event");
            DtMemCopy(&pDtaEvents->m_PendingEvents[0],
                                         &pDtaEvents->m_PendingEvents[1],
                                         sizeof(DtaEvent) * (MAX_PENDING_EVENTS - 1));
            pDtaEvent = &pDtaEvents->m_PendingEvents[MAX_PENDING_EVENTS - 1];
        } else {
            pDtaEvent = &pDtaEvents->m_PendingEvents[pDtaEvents->m_NumPendingEvents];
            pDtaEvents->m_NumPendingEvents++;
        }

        DtDbgOut(MAX, EVENTS, "New event #%d. Type: %d, Value1: %d, Value2: %d", 
                           pDtaEvents->m_NumPendingEvents, EventType, Value1, Value2);
        
        pDtaEvent->m_EventType = EventType;
        pDtaEvent->m_EventValue1 = Value1;
        pDtaEvent->m_EventValue2 = Value2;

        // Trigger wait function if it was waiting
        DtEventSet(&pDtaEvents->m_PendingEvent);
        DtSpinLockRelease(&pDtaEvents->m_Lock);
    }
    return Result;
}

#ifdef WINBUILD
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEventsDequeue -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaEventsDequeue(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile)
{
    WDF_REQUEST_PARAMETERS  Params;
    WDFREQUEST  Request;
    WDFREQUEST  Request2;
    NTSTATUS  NtStatus = STATUS_SUCCESS;
    size_t  BufSize;
    DtaIoctlOutputData*  pOutBuf;
    
    
    // Get all pending requests from queue for this file object
    while (NtStatus == STATUS_SUCCESS)
    {
        WDF_REQUEST_PARAMETERS_INIT(&Params);
        
        NtStatus = WdfIoQueueFindRequest(pDvcData->m_IalData.m_EventQueue, NULL, 
                                               DtFileGetHandle(pFile), &Params, &Request);
        if (NtStatus != STATUS_SUCCESS) // Don't use the NT_SUCCESS macro here
            break;
        
        NtStatus = WdfIoQueueRetrieveFoundRequest(pDvcData->m_IalData.m_EventQueue, 
                                                                      Request, &Request2);
        
        WdfObjectDereference(Request);
        if (!NT_SUCCESS(NtStatus))
            continue;
        
        if (NT_SUCCESS(NtStatus))
        {
            NtStatus = WdfRequestRetrieveOutputBuffer(Request2, 
                                     Params.Parameters.DeviceIoControl.OutputBufferLength,
                                     &pOutBuf, &BufSize);
        }

        if (NT_SUCCESS(NtStatus))
        {   DtStatus  Status;
            BufSize = sizeof(DtaIoctlGetEventOutput);
            Status = DtaEventsGet(pDvcData, pFile, &pOutBuf->m_GetEvent.m_EventType,
                                                 &pOutBuf->m_GetEvent.m_Value1,
                                                 &pOutBuf->m_GetEvent.m_Value2, FALSE);
        }

        if (!NT_SUCCESS(NtStatus))
            BufSize = 0;
        
        // Complete request, use DtStatus in the driver-defined information field
        WdfRequestCompleteWithInformation(Request2, NtStatus, (ULONG_PTR)BufSize);
    }
    return DT_STATUS_OK;
}
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEventsInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialised the events structure and mark the event as Initialised/InUse.
//
DtStatus  DtaEventsInit(DtaDeviceData* pDvcData)
{
    Int  i;

    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
    {
        pDvcData->m_Events[i].m_RefCount = 0;
            
        // Initialize spinlock
        DtSpinLockInit(&pDvcData->m_Events[i].m_Lock);
        DtEventInit(&pDvcData->m_Events[i].m_PendingEvent, FALSE);
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEventsCleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
//
DtStatus  DtaEventsCleanup(DtaDeviceData* pDvcData)
{
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEventsGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Return a pending event. This functions blocks if no events are pending.
//
DtStatus  DtaEventsGet(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile, 
    UInt*  pEventType, 
    UInt*  pValue1,
    UInt*  pValue2,
    Bool  Wait)
{
    DtStatus  Result = DT_STATUS_OK;
    DtaEvents*  pDtaEvents;

    if (pFile==NULL || pEventType==NULL || pValue1==NULL || pValue2==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Get corresponding events object
    pDtaEvents = DtaEventsGetEventsObject(pDvcData, pFile);
    if (pDtaEvents == NULL)
        Result = DT_STATUS_NOT_FOUND;

    if (DT_SUCCESS(Result))
    {
        DtEventReset(&pDtaEvents->m_PendingEvent);

        if (Wait && pDtaEvents->m_NumPendingEvents==0 && !pDtaEvents->m_CancelInProgress) 
        {
            DtDbgOut(MAX, EVENTS, "Waiting for event");
        
            // Wait for event to be triggered
            DtEventWait(&pDtaEvents->m_PendingEvent, -1);
        }

        // The next request will be rejected by the IoCtl function, so we can reset
        // the Cancel state here.
        if (pDtaEvents->m_CancelInProgress)
            Result = DT_STATUS_CANCELLED;
        pDtaEvents->m_CancelInProgress = FALSE;

        DtSpinLockAcquire(&pDtaEvents->m_Lock);
        
        // Return pending events
        if (pDtaEvents->m_NumPendingEvents != 0)
        {  
            *pEventType = pDtaEvents->m_PendingEvents[0].m_EventType;
            *pValue1 = pDtaEvents->m_PendingEvents[0].m_EventValue1;
            *pValue2 =  pDtaEvents->m_PendingEvents[0].m_EventValue2;
            DtDbgOut(MAX, EVENTS, "Event #%d. Type: %d, Value1: %d, Value2: %d", 
                         pDtaEvents->m_NumPendingEvents, *pEventType, *pValue1, *pValue2);
            pDtaEvents->m_NumPendingEvents--;

            if (pDtaEvents->m_NumPendingEvents != 0)
            {   
                // Remove the old event
                DtMemCopy(&pDtaEvents->m_PendingEvents[0],
                                       &pDtaEvents->m_PendingEvents[1],
                                       sizeof(DtaEvent) * pDtaEvents->m_NumPendingEvents);
            }
        } else {
            *pEventType = 0;
            *pValue1 = 0;
            *pValue2 = 0;
            if (Result == DT_STATUS_OK)
                Result = DT_STATUS_REQUEUE; // No pending events
            DtDbgOut(MAX, EVENTS, "Event #%d. No event", pDtaEvents->m_NumPendingEvents);
        }
    
        DtSpinLockRelease(&pDtaEvents->m_Lock);

        // Decrement refcount
        DtaEventsUnrefEventsObject(pDvcData, pDtaEvents);
    }
    
    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEventsGetCancel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This functions cancels the DtaEventGetWait function if that function was waiting
// for an event to happen
//
DtStatus  DtaEventsGetCancel(DtaDeviceData* pDvcData, DtFileObject* pFile)
{
    DtaEvents*  pDtaEvents;

    if (pFile == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    DtDbgOut(MAX, EVENTS, "Start");

    // We force the Pending event to be signaled.
    pDtaEvents = DtaEventsGetEventsObject(pDvcData, pFile);
    if (pDtaEvents == NULL)
    {
        DtDbgOut(MAX, EVENTS, "Exit");
        return DT_STATUS_NOT_FOUND;
    }

    pDtaEvents->m_CancelInProgress = TRUE;

#ifdef WINBUILD
    // Dequeue pending events
    DtaEventsDequeue(pDvcData, pFile);
#endif

    // Trigger event
    DtEventSet(&pDtaEvents->m_PendingEvent);

    // Decrement refcount
    DtaEventsUnrefEventsObject(pDvcData, pDtaEvents);

    DtDbgOut(MAX, EVENTS, "Exit");

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEventsNumPending -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtaEventsNumPending(DtaDeviceData* pDvcData, DtFileObject* pFile)
{
    DtaEvents*  pDtaEvents;
    Int  NumPending = 0;

    DtDbgOut(MAX, EVENTS, "Start");

    // We force the Pending event to be signaled.
    pDtaEvents = DtaEventsGetEventsObject(pDvcData, pFile);
    if (pDtaEvents != NULL)
    {
        NumPending = pDtaEvents->m_NumPendingEvents;
        
        // Decrement refcount
        DtaEventsUnrefEventsObject(pDvcData, pDtaEvents);
    }

    DtDbgOut(MAX, EVENTS, "Exit (%d pending events)", NumPending);

    return NumPending;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEventsRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtaEventsRegister(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    UInt  EventTypeMask)
{
    DtaEvents*  pDtaEvents;

    if (pFile == NULL)
        return DT_STATUS_INVALID_PARAMETER;
        
    DtDbgOut(MAX, EVENTS, "Register event mask 0x%x", EventTypeMask);

    pDtaEvents = DtaEventsAllocEventsObject(pDvcData, pFile, EventTypeMask);
    if (pDtaEvents == NULL)
        return DT_STATUS_OUT_OF_RESOURCES;

    // Decrement refcount
    DtaEventsUnrefEventsObject(pDvcData, pDtaEvents);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEventsSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function adds new events to the pending events. If the Events argument is 0, 
// this function does nothing.
//
DtStatus  DtaEventsSet(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    UInt  EventType,
    UInt  Value1,
    UInt  Value2)
{
    DtStatus  Result = DT_STATUS_OK;
    Int  i;
    DtaEvents*  pDtaEvents = NULL;
    Bool  InUse;
    
    DtDbgOut(MAX, EVENTS, "Start");
    DtDbgOut(AVG, EVENTS, "EventType: %i, Value1: %i, Value2: %i", EventType, Value1, 
                                                                                  Value2);
    if (pFile != NULL)
    {
        // Get corresponding events object
        pDtaEvents = DtaEventsGetEventsObject(pDvcData, pFile);
        if (pDtaEvents == NULL)
            Result = DT_STATUS_NOT_FOUND;
        else {
            Result = DtaEventsSetEvent(pDtaEvents, EventType, Value1, Value2);
#ifdef WINBUILD
            if (pDtaEvents->m_NumPendingEvents != 0)
            {
                // Dequeue pending events
                DtaEventsDequeue(pDvcData, pFile);
            }
#endif
            DtaEventsUnrefEventsObject(pDvcData, pDtaEvents);
        }
    } else {
        // Set event for all file handles
        for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
        {
            pDtaEvents = &pDvcData->m_Events[i];

            // Increment refcount
            InUse = FALSE;
            DtSpinLockAcquire(&pDtaEvents->m_Lock);
            if (pDtaEvents->m_RefCount > 0)
            {
                pDtaEvents->m_RefCount++;
                InUse = TRUE;
            }
            DtSpinLockRelease(&pDtaEvents->m_Lock);
            
            if (InUse)
            {
                // Set event
                Result = DtaEventsSetEvent(pDtaEvents, EventType, Value1, Value2);
#ifdef WINBUILD
                if (pDtaEvents->m_NumPendingEvents != 0)
                {
                    // Dequeue pending events
                    pFile = &pDtaEvents->m_File;
                    DtaEventsDequeue(pDvcData, pFile);
                }
#endif
                // Decrement refcount
                DtaEventsUnrefEventsObject(pDvcData, pDtaEvents);
            }
        }
    }
    
    DtDbgOut(MAX, EVENTS, "Exit");

    return Result;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEventsUnregister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaEventsUnregister(DtaDeviceData* pDvcData, DtFileObject* pFile)
{
    DtaEvents*  pDtaEvents;

    // We force the Pending event to be signaled.
    pDtaEvents = DtaEventsGetEventsObject(pDvcData, pFile);
    if (pDtaEvents == NULL)
        return;

    // Decrement initial refcount to mark unused after last unreference
    DtSpinLockAcquire(&pDtaEvents->m_Lock);
    if (pDtaEvents->m_RefCount > 0)
        pDtaEvents->m_RefCount--;
    DtSpinLockRelease(&pDtaEvents->m_Lock);

    // Decrement refcount
    DtaEventsUnrefEventsObject(pDvcData, pDtaEvents);

    // Signal pending event (will remove object if event was pending)
    DtaEventsGetCancel(pDvcData, pFile);
}
