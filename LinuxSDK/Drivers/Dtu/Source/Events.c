//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Events.c *#*#*#*#*#*#*#*#*# (C) 2010-2016 DekTec
//
// Dtu1xx driver - Events subsystem - Implements a mechanism to queue events for the user
//
// This module queues all evenst like power events. The events can be requested with the
// DtuEventsWait function. To cancel a pending wait DtuEventsWaitCancel must be called.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2016 DekTec Digital Video B.V.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtuIncludes.h>


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEventsGetEventsObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtuEvents* DtuEventsGetEventsObject(
    DtuDeviceData*  pDvcData,
    DtFileObject*  pFile)
{
    Int  i;
    DtuEvents*  pDtuEvents = NULL;

    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
    {
        DtSpinLockAcquire(&pDvcData->m_Events[i].m_Lock);
        if (pDvcData->m_Events[i].m_RefCount > 0)
        {
            if (DtFileCompare(&pDvcData->m_Events[i].m_File, pFile))
            {
                pDtuEvents = &pDvcData->m_Events[i];

                // Increment refcount
                pDtuEvents->m_RefCount++;
            }
        }
        DtSpinLockRelease(&pDvcData->m_Events[i].m_Lock);

        if (pDtuEvents != NULL)
            break;
    }

    return pDtuEvents;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEventsAllocEventsObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtuEvents* DtuEventsAllocEventsObject(
    DtuDeviceData*  pDvcData,
    DtFileObject*  pFileHandle,
    UInt  EventTypeMask)
{
    Int  i;
    DtuEvents*  pDtuEvents = NULL;
    
    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
    {
        DtSpinLockAcquire(&pDvcData->m_Events[i].m_Lock);
        if (pDvcData->m_Events[i].m_RefCount == 0)
        {
            pDtuEvents = &pDvcData->m_Events[i];

            pDtuEvents->m_RefCount = 2;
                                // Start with 2 to prevent removal after first unref
            pDtuEvents->m_File = *pFileHandle;
            pDtuEvents->m_EventTypeMask = EventTypeMask;
            pDtuEvents->m_CancelInProgress = FALSE;
            pDtuEvents->m_NumPendingEvents = 0;
            memset(&pDtuEvents->m_PendingEvents, 0, sizeof(pDtuEvents->m_PendingEvents));
        }
        DtSpinLockRelease(&pDvcData->m_Events[i].m_Lock);

        if (pDtuEvents != NULL)
            break;
    }

    return pDtuEvents;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEventsUnrefEventsObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static void  DtuEventsUnrefEventsObject(DtuDeviceData* pDvcData, DtuEvents* pDtuEvents)
{
    DtSpinLockAcquire(&pDtuEvents->m_Lock);
    if (pDtuEvents->m_RefCount > 0)
        pDtuEvents->m_RefCount--;
    DtSpinLockRelease(&pDtuEvents->m_Lock);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEventsSetEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtuEventsSetEvent(
    DtuEvents*  pDtuEvents,
    UInt  EventType,
    UInt  Value1,
    UInt  Value2)
{
    DtStatus  Result = DT_STATUS_OK;
    DtuEvent*  pDtuEvent;

    // Skip unregistered events
    if ((EventType & pDtuEvents->m_EventTypeMask) != 0)
    {
        DtSpinLockAcquire(&pDtuEvents->m_Lock);

        // Add new events
        if (pDtuEvents->m_NumPendingEvents == MAX_PENDING_EVENTS)
        {
            // Remove oldest event
            DtDbgOut(AVG, EVENTS, "Max. number of events. Remove old event");
            DtMemMove(&pDtuEvents->m_PendingEvents[0],
                                         &pDtuEvents->m_PendingEvents[1],
                                         sizeof(DtuEvent) * (MAX_PENDING_EVENTS - 1));
            pDtuEvent = &pDtuEvents->m_PendingEvents[MAX_PENDING_EVENTS - 1];
        } else {
            pDtuEvent = &pDtuEvents->m_PendingEvents[pDtuEvents->m_NumPendingEvents];
            pDtuEvents->m_NumPendingEvents++;
        }

        DtDbgOut(MAX, EVENTS, "New event #%d. Type: %d, Value1: %d, Value2: %d", 
                           pDtuEvents->m_NumPendingEvents, EventType, Value1, Value2);
        
        pDtuEvent->m_EventType = EventType;
        pDtuEvent->m_EventValue1 = Value1;
        pDtuEvent->m_EventValue2 = Value2;

        // Trigger wait function if it was waiting
        DtEventSet(&pDtuEvents->m_PendingEvent);
        DtSpinLockRelease(&pDtuEvents->m_Lock);
    }
    return Result;
}

#ifdef WINBUILD
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEventsDequeue -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuEventsDequeue(
    DtuDeviceData*  pDvcData,
    DtFileObject*  pFile)
{
    WDF_REQUEST_PARAMETERS  Params;
    WDFREQUEST  Request;
    WDFREQUEST  Request2;
    NTSTATUS  NtStatus = STATUS_SUCCESS;
    size_t  BufSize;
    DtuIoctlOutputData*  pOutBuf;
    
    
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
            BufSize = sizeof(DtuIoctlGetEventOutput);
            Status = DtuEventsGet(pDvcData, pFile, &pOutBuf->m_GetEvent.m_EventType,
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEventsInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialised the events structure and mark the event as Initialised/InUse.
//
DtStatus  DtuEventsInit(DtuDeviceData* pDvcData)
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEventsCleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
//
DtStatus  DtuEventsCleanup(DtuDeviceData* pDvcData)
{
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEventsGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Return a pending event. This functions blocks if no events are pending.
//
DtStatus  DtuEventsGet(
    DtuDeviceData*  pDvcData,
    DtFileObject*  pFile, 
    UInt*  pEventType, 
    UInt*  pValue1,
    UInt*  pValue2,
    Bool  Wait)
{
    DtStatus  Result = DT_STATUS_OK;
    DtuEvents*  pDtuEvents;

    if (pFile==NULL || pEventType==NULL || pValue1==NULL || pValue2==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Get corresponding events object
    pDtuEvents = DtuEventsGetEventsObject(pDvcData, pFile);
    if (pDtuEvents == NULL)
        Result = DT_STATUS_NOT_FOUND;

    if (DT_SUCCESS(Result))
    {
        DtEventReset(&pDtuEvents->m_PendingEvent);

        if (Wait && pDtuEvents->m_NumPendingEvents==0 && !pDtuEvents->m_CancelInProgress) 
        {
            DtDbgOut(MAX, EVENTS, "Waiting for event");
        
            // Wait for event to be triggered
            DtEventWait(&pDtuEvents->m_PendingEvent, -1);
        }

        // The next request will be rejected by the IoCtl function, so we can reset
        // the Cancel state here.
        if (pDtuEvents->m_CancelInProgress)
            Result = DT_STATUS_CANCELLED;
        pDtuEvents->m_CancelInProgress = FALSE;

        DtSpinLockAcquire(&pDtuEvents->m_Lock);
        
        // Return pending events
        if (pDtuEvents->m_NumPendingEvents != 0)
        {  
            *pEventType = pDtuEvents->m_PendingEvents[0].m_EventType;
            *pValue1 = pDtuEvents->m_PendingEvents[0].m_EventValue1;
            *pValue2 =  pDtuEvents->m_PendingEvents[0].m_EventValue2;
            DtDbgOut(MAX, EVENTS, "Event #%d. Type: %d, Value1: %d, Value2: %d", 
                         pDtuEvents->m_NumPendingEvents, *pEventType, *pValue1, *pValue2);
            pDtuEvents->m_NumPendingEvents--;

            if (pDtuEvents->m_NumPendingEvents != 0)
            {   
                // Remove the old event
                DtMemMove(&pDtuEvents->m_PendingEvents[0],
                                       &pDtuEvents->m_PendingEvents[1],
                                       sizeof(DtuEvent) * pDtuEvents->m_NumPendingEvents);
            }
        } else {
            *pEventType = 0;
            *pValue1 = 0;
            *pValue2 = 0;
            if (Result == DT_STATUS_OK)
                Result = DT_STATUS_REQUEUE; // No pending events
            DtDbgOut(MAX, EVENTS, "Event #%d. No event", pDtuEvents->m_NumPendingEvents);
        }
    
        DtSpinLockRelease(&pDtuEvents->m_Lock);

        // Decrement refcount
        DtuEventsUnrefEventsObject(pDvcData, pDtuEvents);
    }
    
    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEventsGetCancel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This functions cancels the DtuEventGetWait function if that function was waiting
// for an event to happen
//
DtStatus  DtuEventsGetCancel(DtuDeviceData* pDvcData, DtFileObject* pFile)
{
    DtuEvents*  pDtuEvents;

    if (pFile == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    DtDbgOut(MAX, EVENTS, "Start");

    // We force the Pending event to be signaled.
    pDtuEvents = DtuEventsGetEventsObject(pDvcData, pFile);
    if (pDtuEvents == NULL)
    {
        DtDbgOut(MAX, EVENTS, "Exit");
        return DT_STATUS_NOT_FOUND;
    }

    pDtuEvents->m_CancelInProgress = TRUE;

#ifdef WINBUILD
    // Dequeue pending events
    DtuEventsDequeue(pDvcData, pFile);
#endif

    // Trigger event
    DtEventSet(&pDtuEvents->m_PendingEvent);

    // Decrement refcount
    DtuEventsUnrefEventsObject(pDvcData, pDtuEvents);

    DtDbgOut(MAX, EVENTS, "Exit");

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEventsNumPending -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtuEventsNumPending(DtuDeviceData* pDvcData, DtFileObject* pFile)
{
    DtuEvents*  pDtuEvents;
    Int  NumPending = 0;

    DtDbgOut(MAX, EVENTS, "Start");

    // We force the Pending event to be signaled.
    pDtuEvents = DtuEventsGetEventsObject(pDvcData, pFile);
    if (pDtuEvents != NULL)
    {
        NumPending = pDtuEvents->m_NumPendingEvents;
        
        // Decrement refcount
        DtuEventsUnrefEventsObject(pDvcData, pDtuEvents);
    }

    DtDbgOut(MAX, EVENTS, "Exit (%d pending events)", NumPending);

    return NumPending;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEventsRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtuEventsRegister(
    DtuDeviceData*  pDvcData,
    DtFileObject*  pFile,
    UInt  EventTypeMask)
{
    DtuEvents*  pDtuEvents;

    if (pFile == NULL)
        return DT_STATUS_INVALID_PARAMETER;
        
    DtDbgOut(MAX, EVENTS, "Register event mask 0x%x", EventTypeMask);

    pDtuEvents = DtuEventsAllocEventsObject(pDvcData, pFile, EventTypeMask);
    if (pDtuEvents == NULL)
        return DT_STATUS_OUT_OF_RESOURCES;

    // Decrement refcount
    DtuEventsUnrefEventsObject(pDvcData, pDtuEvents);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEventsSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function adds new events to the pending events. If the Events argument is 0, 
// this function does nothing.
//
DtStatus  DtuEventsSet(
    DtuDeviceData*  pDvcData,
    DtFileObject*  pFile,
    UInt  EventType,
    UInt  Value1,
    UInt  Value2)
{
    DtStatus  Result = DT_STATUS_OK;
    Int  i;
    DtuEvents*  pDtuEvents = NULL;
    Bool  InUse;
    
    DtDbgOut(MAX, EVENTS, "Start");
    DtDbgOut(AVG, EVENTS, "EventType: %i, Value1: %i, Value2: %i", EventType, Value1, 
                                                                                  Value2);
    if (pFile != NULL)
    {
        // Get corresponding events object
        pDtuEvents = DtuEventsGetEventsObject(pDvcData, pFile);
        if (pDtuEvents == NULL)
            Result = DT_STATUS_NOT_FOUND;
        else {
            Result = DtuEventsSetEvent(pDtuEvents, EventType, Value1, Value2);
#ifdef WINBUILD
            if (pDtuEvents->m_NumPendingEvents != 0)
            {
                // Dequeue pending events
                DtuEventsDequeue(pDvcData, pFile);
            }
#endif
            DtuEventsUnrefEventsObject(pDvcData, pDtuEvents);
        }
    } else {
        // Set event for all file handles
        for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
        {
            pDtuEvents = &pDvcData->m_Events[i];

            // Increment refcount
            InUse = FALSE;
            DtSpinLockAcquire(&pDtuEvents->m_Lock);
            if (pDtuEvents->m_RefCount > 0)
            {
                pDtuEvents->m_RefCount++;
                InUse = TRUE;
            }
            DtSpinLockRelease(&pDtuEvents->m_Lock);
            
            if (InUse)
            {
                // Set event
                Result = DtuEventsSetEvent(pDtuEvents, EventType, Value1, Value2);
#ifdef WINBUILD
                if (pDtuEvents->m_NumPendingEvents != 0)
                {
                    // Dequeue pending events
                    pFile = &pDtuEvents->m_File;
                    DtuEventsDequeue(pDvcData, pFile);
                }
#endif
                // Decrement refcount
                DtuEventsUnrefEventsObject(pDvcData, pDtuEvents);
            }
        }
    }
    
    DtDbgOut(MAX, EVENTS, "Exit");

    return Result;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEventsUnregister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtuEventsUnregister(DtuDeviceData* pDvcData, DtFileObject* pFile)
{
    DtuEvents*  pDtuEvents;

    // We force the Pending event to be signaled.
    pDtuEvents = DtuEventsGetEventsObject(pDvcData, pFile);
    if (pDtuEvents == NULL)
        return;

    // Decrement initial refcount to mark unused after last unreference
    DtSpinLockAcquire(&pDtuEvents->m_Lock);
    if (pDtuEvents->m_RefCount > 0)
        pDtuEvents->m_RefCount--;
    DtSpinLockRelease(&pDtuEvents->m_Lock);

    // Decrement refcount
    DtuEventsUnrefEventsObject(pDvcData, pDtuEvents);

    // Signal pending event (will remove object if event was pending)
    DtuEventsGetCancel(pDvcData, pFile);
}
