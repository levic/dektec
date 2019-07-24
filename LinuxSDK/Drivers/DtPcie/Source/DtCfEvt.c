// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCfEvt.c *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtCfEvt.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfEvt implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtCfEvt function
#define CF_EVT_DEFAULT_PRECONDITIONS(pCf)      \
    DT_ASSERT(pCf!=NULL && pCf->m_Size==sizeof(DtCfEvt))

// Helper macro to cast a DtDf* to a DtCfEvt*
#define CF_EVT      ((DtCfEvt*)pCf)

// Structure for storing driver events
typedef  struct _DtCfEvtData 
{
    DtSpinLock  m_EventsSpinlock;
    DtDriverEvents*  m_pEvents;
} DtCfEvtData;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtCfEvt_Init(DtDf*);
static DtDriverEvents*  DtCfEvt_AllocEventsObject(DtCfEvt*, DtFileObject*, UInt);
DT_UNUSED  static DtStatus  DtCfEvt_Dequeue(DtCfEvt*, DtDriverEvents*);
static DtStatus  DtCfEvt_Get(DtCfEvt*, DtFileObject*, DtDriverEvents*, DtDriverEvent*);
static DtStatus  DtCfEvt_GetCancel(DtCfEvt*, DtDriverEvents*  pDrvEvents);
static DtDriverEvents*  DtCfEvt_GetEventsObject(DtCfEvt*, DtFileObject*);
static DtStatus  DtCfEvt_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtCfEvt_Register(DtCfEvt*, DtFileObject*, UInt EventType);
static DtStatus  DtCfEvt_SetEvent(DtCfEvt*, DtDriverEvents*, DtDriverEvent, Bool);
static DtStatus  DtCfEvt_Unregister(DtCfEvt*, const DtFileObject*);
static DtStatus  DtCfEvt_UnrefEventsObject(DtCfEvt*, DtDriverEvents*);

// DtIoStubCfEvt
static DtStatus  DtIoStubCfEvt_Dequeue(DtIoStubCfEvt*, DtDriverEvents*);

// DtVectorEvent
static DtVectorEvent*  DtVectorEvent_Create(Int  Capacity, Int  GrowSize);
static void  DtVectorEvent_Cleanup(DtVectorEvent*);
static DtDriverEvent*  DtVectorEvent_Data(DtVectorEvent*);
static DtStatus  DtVectorEvent_PushBack(DtVectorEvent* , DtDriverEvent*);
static DtStatus  DtVectorEvent_PopFront(DtVectorEvent*  , DtDriverEvent*);
static Int  DtVectorEvent_Size(const DtVectorEvent*);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfEvt - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtCfEvt_Close(DtDf*  pCf)
{

    // Sanity checks
    CF_EVT_DEFAULT_PRECONDITIONS(pCf);

    // Let base function perform final clean-up
    DtDf_Close(pCf);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtCfEvt*  DtCfEvt_Open(DtCore*  pCore, const char*  pRole, Int  Instance, 
                                                              Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    
    // Init open parameters
    DT_CF_EVT_INIT_ID(Id, pRole, Instance, Uuid);
    DT_CF_INIT_OPEN_PARAMS(OpenParams, DtCfEvt, Id, DT_FUNC_TYPE_CF_EVT,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtCfEvt_Close;
    OpenParams.m_InitFunc = DtCfEvt_Init;
    OpenParams.m_OnCloseFileFunc = DtCfEvt_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtCfEvt*)DtDf_Open(&OpenParams);
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_CleanupEventsData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtCfEvt_CleanupEventsData(DtCfEvtData* pEvtData)
{
    DtDriverEvents*  pDrvEvents = NULL;
    DtDriverEvents*  pTmpEvents = NULL;

    if (pEvtData == NULL)
        return;

    pDrvEvents = pEvtData->m_pEvents;
    while (pDrvEvents != NULL)
    {
        pTmpEvents = pDrvEvents;
        pDrvEvents = pDrvEvents->m_pNext;

        // Free the vector
        DtVectorEvent_Cleanup(pTmpEvents->m_PendingEvents);

        DtMemFreePool(pTmpEvents, DF_TAG);
    }

    // Free the event data structure
    DtMemFreePool(pEvtData, DF_TAG);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_CreateEventsData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtCfEvtData*  DtCfEvt_CreateEventsData()
{
    DtCfEvtData*  pEvtData = (DtCfEvtData*)DtMemAllocPool(
                                          DtPoolNonPaged, sizeof(DtCfEvtData), DF_TAG);
    if (pEvtData == NULL)
        return NULL;

    DtSpinLockInit(&pEvtData->m_EventsSpinlock);
    pEvtData->m_pEvents = NULL;

    return pEvtData;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_Set -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfEvt_Set(DtCfEvt*  pCf, DtDriverEvent  Event, Bool AddIfExists)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDriverEvents*  pDrvEvents = NULL;
    DtCfEvtData* pEvtData = pCf->m_pEvtData;

    // Sanity check
    CF_EVT_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pEvtData != NULL);
    
    DtDbgOutCf(MAX, EVT, pCf, "Start");
    DtDbgOutCf(MAX, EVT, pCf, "EventType: %i, Value1: %i, Value2: %i", 
                              Event.m_EventType, Event.m_EventValue1,Event.m_EventValue2);

    // Set event for all file handles
    DtSpinLockAcquire(&pEvtData->m_EventsSpinlock);

    pDrvEvents = pEvtData->m_pEvents;
    while (pDrvEvents != NULL)
    {
        // Set event
        Status = DtCfEvt_SetEvent(pCf, pDrvEvents, Event, AddIfExists);
#ifdef WINBUILD
        if (DtVectorEvent_Size(pDrvEvents->m_PendingEvents) != 0)
        {
            // Dequeue pending events
            DtCfEvt_Dequeue(pCf, pDrvEvents);
        }
#endif
        pDrvEvents = pDrvEvents->m_pNext;
    }
    DtSpinLockRelease(&pEvtData->m_EventsSpinlock);
    
    DtDbgOutCf(MAX, EVT, pCf, "Exit");

    return Status;
}

#ifdef LINBUILD
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_Poll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt DtCfEvt_Poll(DtCfEvt* pCf, DtFileObject* pFile, poll_table* pPollTable)
{
    UInt  Mask = 0;
    DtDriverEvents*  pDrvEvents = NULL;
    DtCfEvtData* pEvtData = pCf->m_pEvtData;
    Int  NumPending = 0;

    // Sanity check
    CF_EVT_DEFAULT_PRECONDITIONS(pCf);

    // No event data?
    if (pCf->m_pEvtData == NULL)
        return Mask;

    DtSpinLockAcquire(&pEvtData->m_EventsSpinlock);
        
    // Add events wait_queue to poll table and get number of pending events
    pDrvEvents = pEvtData->m_pEvents;
    while (pDrvEvents != NULL)
    {
        if (DtFileCompare(&pDrvEvents->m_File, pFile))
        {
            poll_wait(pFile->m_pFile, &pDrvEvents->m_PendingEvent.m_WaitQueueHead,
                                                                              pPollTable);
            NumPending = DtVectorEvent_Size(pDrvEvents->m_PendingEvents);
            break;
        }
        pDrvEvents = pDrvEvents->m_pNext;
    }
    
    // Check if events are pending
    if (NumPending > 0)
    {
        Mask |= POLLIN | POLLRDNORM;
    }    
    DtSpinLockRelease(&pEvtData->m_EventsSpinlock);

    return Mask;
}

#endif

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfEvt - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_AllocEventsObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDriverEvents*  DtCfEvt_AllocEventsObject(
    DtCfEvt*  pCf,
    DtFileObject*  pFileHandle,
    UInt  EventTypeMask)
{
    DtDriverEvents*  pDrvEvents;
    DtCfEvtData* pEvtData = pCf->m_pEvtData;

    // Sanity check
    CF_EVT_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pEvtData != NULL);

    pDrvEvents = (DtDriverEvents*)DtMemAllocPool(
                                          DtPoolNonPaged, sizeof(DtDriverEvents), DF_TAG);
    if (pDrvEvents == NULL)
    {
        DtDbgOutCf(ERR, EVT, pCf, "ERROR: Allocating DtDriverEvents struct");
        return NULL;
    }

    DtAtomicSet(&pDrvEvents->m_RefCount, 1);
    pDrvEvents->m_File = *pFileHandle;
    pDrvEvents->m_EventTypeMask = EventTypeMask;
    pDrvEvents->m_CancelInProgress = FALSE;
    DtEventInit(&pDrvEvents->m_PendingEvent, FALSE);
    DtSpinLockInit(&pDrvEvents->m_Lock);

    // Create vector for pending events
    pDrvEvents->m_PendingEvents = DtVectorEvent_Create(DT_CF_EVT_MAX_PENDING_EVENTS, 0);
    if (pDrvEvents->m_PendingEvents == NULL)
    {
        DtDbgOutCf(ERR, EVT, pCf, "ERROR: Allocating PendingEvents vector");
        DtMemFreePool(pDrvEvents, DF_TAG);
        return NULL;
    }

    // Insert at start of list
    DtSpinLockAcquire(&pEvtData->m_EventsSpinlock);

    pDrvEvents->m_pPrev = NULL;
    pDrvEvents->m_pNext = NULL;
    if (pEvtData->m_pEvents != NULL)
    {
        pDrvEvents->m_pNext = pEvtData->m_pEvents;
        pDrvEvents->m_pNext->m_pPrev = pDrvEvents;
    }
    pEvtData->m_pEvents = pDrvEvents;
    DtSpinLockRelease(&pEvtData->m_EventsSpinlock);

    return pDrvEvents;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_Dequeue -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfEvt_Dequeue(DtCfEvt*  pCf, DtDriverEvents*  pDrvEvents)
{
    // Sanity check
    CF_EVT_DEFAULT_PRECONDITIONS(pCf);

    // Let the stub do the work
    if (pCf->m_pIoStub == NULL)
        return DT_STATUS_OK;

    return DtIoStubCfEvt_Dequeue((DtIoStubCfEvt*)pCf->m_pIoStub, pDrvEvents);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_Get -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Return a pending event if one is available.
//
DtStatus  DtCfEvt_Get(
    DtCfEvt*  pCf, 
    DtFileObject*  pFile,
    DtDriverEvents*  pDrvEvents,    // If not NULL, we must not use file object
    DtDriverEvent*   pEvent)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool NeedUnref = FALSE;

    // Sanity check
    CF_EVT_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pFile!=NULL || pDrvEvents!=NULL);
    DT_ASSERT(pEvent!=NULL);

    // Get corresponding events object
    // if pDrvEvents is passed, we shall not call the DtCfEvt_GetEventsObject.
    // Since the caller (= DtCfEvt_Set()) will have already a lock on pCore; 
    // it will cause a deadlock
    if (pDrvEvents == NULL)
    {
        pDrvEvents = DtCfEvt_GetEventsObject(pCf, pFile);
        if (pDrvEvents == NULL)
            return DT_STATUS_NOT_FOUND;
        else
            NeedUnref = TRUE;
    }

    DtEventReset(&pDrvEvents->m_PendingEvent);

    // The next request will be rejected by the IoCtl function, so we can reset
    // the Cancel state here.
    if (pDrvEvents->m_CancelInProgress)
        Status = DT_STATUS_CANCELLED;

    pDrvEvents->m_CancelInProgress = FALSE;

    DtSpinLockAcquire(&pDrvEvents->m_Lock);
        
    // Return pending events
    if (DtVectorEvent_PopFront(pDrvEvents->m_PendingEvents, pEvent) == DT_STATUS_OK)
    {
        DtDbgOutCf(MAX, EVT, pCf, 
                       "Event #%d. Type: %d, Value1: %d, Value2: %d", 
                       DtVectorEvent_Size(pDrvEvents->m_PendingEvents),
                       pEvent->m_EventType, pEvent->m_EventValue1, pEvent->m_EventValue2);

    } 
    else 
    {
        pEvent->m_EventType = 0;
        pEvent->m_EventValue1 = 0;
        pEvent->m_EventValue2 = 0;
        if (Status == DT_STATUS_OK)
            Status = DT_STATUS_REQUEUE; // No pending events
        DtDbgOutCf(MAX, EVT, pCf, "Event #%d. No event",
                                         DtVectorEvent_Size(pDrvEvents->m_PendingEvents));
    }
    
    DtSpinLockRelease(&pDrvEvents->m_Lock);

    // Decrement refcount
    if (NeedUnref)
        DtCfEvt_UnrefEventsObject(pCf, pDrvEvents);
  
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_GetCancel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This functions cancels the DtCfEvt_Get function if that function was waiting
// for an event to happen
//
DtStatus  DtCfEvt_GetCancel(DtCfEvt*  pCf, DtDriverEvents*  pDrvEvents)
{
    // Sanity check
    CF_EVT_DEFAULT_PRECONDITIONS(pCf);

    // We force the Pending event to be signaled.
    pDrvEvents->m_CancelInProgress = TRUE;

#ifdef WINBUILD
    // Dequeue pending events
    DtCfEvt_Dequeue(pCf, pDrvEvents);
#endif

    // Trigger event
    DtEventSet(&pDrvEvents->m_PendingEvent);

    DtDbgOutCf(MAX, EVT, pCf, "Exit");

    return DT_STATUS_OK;

}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_GetEventsObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDriverEvents*  DtCfEvt_GetEventsObject(DtCfEvt*  pCf, DtFileObject*  pFile)
{
    DtDriverEvents*  pDrvEvents = NULL;
    DtCfEvtData* pEvtData = pCf->m_pEvtData;

    // Sanity check
    CF_EVT_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pEvtData != NULL);

    DtSpinLockAcquire(&pEvtData->m_EventsSpinlock);
    pDrvEvents = pEvtData->m_pEvents;

    while (pDrvEvents != NULL)
    {
        if (DtFileCompare(&pDrvEvents->m_File, pFile))
        {
            // Increment refcount
            DtAtomicIncrement(&pDrvEvents->m_RefCount);
            break;
        }
        pDrvEvents = pDrvEvents->m_pNext;
    }
    DtSpinLockRelease(&pEvtData->m_EventsSpinlock);
    return pDrvEvents;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_Register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfEvt_Register(DtCfEvt*  pCf, DtFileObject*  pFile, UInt EventTypeMask)
{
    DtDriverEvents*  pDrvEvents;

    // Sanity check
    CF_EVT_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pFile!=NULL);

    DtDbgOutCf(MAX, EVT, pCf, "Register event mask 0x%x", EventTypeMask);
   
    // Already registered?
    pDrvEvents = DtCfEvt_GetEventsObject(pCf, pFile);
    if (pDrvEvents != NULL)
    {
        DtDbgOutCf(ERR, EVT, pCf, "ERROR: Already registered");
        DtCfEvt_UnrefEventsObject(pCf, pDrvEvents);
        return DT_STATUS_IN_USE;
    }

    // Allocate new event object
    pDrvEvents = DtCfEvt_AllocEventsObject(pCf, pFile, EventTypeMask);
    if (pDrvEvents == NULL)
        return DT_STATUS_OUT_OF_RESOURCES;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfEvt_Init(DtDf*  pCf)
{
    // Sanity checks
    CF_EVT_DEFAULT_PRECONDITIONS(pCf);
    
    DT_ASSERT(pCf->m_pCore->m_pCfEvtData != NULL);
    if (pCf->m_pCore->m_pCfEvtData == NULL)
        return DT_STATUS_FAIL;

    // Copy reference to the event data
    CF_EVT->m_pEvtData = pCf->m_pCore->m_pCfEvtData;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtCfEvt_OnCloseFile(DtDf* pCf, const DtFileObject* pFile)
{
    if (CF_EVT->m_pEvtData != NULL)
        DtCfEvt_Unregister(CF_EVT, pFile);

    // Use base function to release exclusive access
    return DtDf_OnCloseFile(pCf, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_SetEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfEvt_SetEvent(DtCfEvt*  pCf, DtDriverEvents* pDrvEvents, 
                                                    DtDriverEvent Event, Bool AddIfExists)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    CF_EVT_DEFAULT_PRECONDITIONS(pCf);

    // Skip unregistered events
    if ((Event.m_EventType & pDrvEvents->m_EventTypeMask) != 0)
    {
        DtSpinLockAcquire(&pDrvEvents->m_Lock);

        // Add duplicates?
        if (!AddIfExists)
        {
            Int i;
            Int NumPending = DtVectorEvent_Size(pDrvEvents->m_PendingEvents);
            DtDriverEvent *pPendingEvents = 
                                          DtVectorEvent_Data(pDrvEvents->m_PendingEvents);
            for (i=0; i<NumPending; i++)
            {
                if (   pPendingEvents[i].m_EventType==Event.m_EventType
                    && pPendingEvents[i].m_EventValue1==Event.m_EventValue1
                    && pPendingEvents[i].m_EventValue2==Event.m_EventValue2)
                {
                    // Event is already pending, don't create a duplicate
                    DtSpinLockRelease(&pDrvEvents->m_Lock);
                    return DT_STATUS_OK;
                }
            }
        }
        if (DtVectorEvent_Size(pDrvEvents->m_PendingEvents) 
                                                          >= DT_CF_EVT_MAX_PENDING_EVENTS)
        {
            // Remove oldest event
            DtDriverEvent Dummy;
            DtVectorEvent_PopFront(pDrvEvents->m_PendingEvents, &Dummy);
        }

        DtDbgOutCf(MAX, EVT, pCf, 
                             "New event #%d. Type: %d, Value1: %d, Value2: %d", 
                             DtVectorEvent_Size(pDrvEvents->m_PendingEvents),
                             Event.m_EventType, Event.m_EventValue1, Event.m_EventValue2);
        
        // Add new event
        Status =  DtVectorEvent_PushBack(pDrvEvents->m_PendingEvents, &Event);

        // Trigger wait function if it was waiting
        DtEventSet(&pDrvEvents->m_PendingEvent);
        DtSpinLockRelease(&pDrvEvents->m_Lock);
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_Unregister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfEvt_Unregister(DtCfEvt*  pCf, const DtFileObject* pFile)
{
    DtDriverEvents*  pDrvEvents = NULL;
    DtCfEvtData* pEvtData = pCf->m_pEvtData;

    // Sanity check
    CF_EVT_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pEvtData != NULL);
    DT_ASSERT(pFile!=NULL);
    
    DtSpinLockAcquire(&pEvtData->m_EventsSpinlock);
    pDrvEvents = pEvtData->m_pEvents;

    while (pDrvEvents != NULL)
    {
        if (DtFileCompare(&pDrvEvents->m_File, pFile))
            break;
        pDrvEvents = pDrvEvents->m_pNext;
    }
    if (pDrvEvents == NULL)
    {
        DtSpinLockRelease(&pEvtData->m_EventsSpinlock);
        return DT_STATUS_NOT_FOUND;
    }

    // Even if two threads call Unregister at the same time we'll only come here
    // once. We remove the DtaEvents object from the linked-list while holding the
    // spinlock, so the second thread won't find the object and return immediately.

    // Signal pending event (will remove object if event was pending)
    DtCfEvt_GetCancel(pCf, pDrvEvents);


    // Remove events struct from linked list while holding the spinlock
    if (pDrvEvents->m_pPrev != NULL)
        pDrvEvents->m_pPrev->m_pNext = pDrvEvents->m_pNext;
    else
        pEvtData->m_pEvents = pDrvEvents->m_pNext;
    if (pDrvEvents->m_pNext != NULL)
        pDrvEvents->m_pNext->m_pPrev = pDrvEvents->m_pPrev;

    pDrvEvents->m_pPrev = NULL;
    pDrvEvents->m_pNext = NULL;

    DtSpinLockRelease(&pEvtData->m_EventsSpinlock);
        
    // Decrement refcount to free object as soon as it's no longer in use
    DtCfEvt_UnrefEventsObject(pCf, pDrvEvents);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfEvt_UnrefEventsObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfEvt_UnrefEventsObject(DtCfEvt*  pCf, DtDriverEvents* pDrvEvents)
{
    // Sanity check
    CF_EVT_DEFAULT_PRECONDITIONS(pCf);

    if (DtAtomicDecrementReturn(&pDrvEvents->m_RefCount) == 0)
    {
        DT_ASSERT(pDrvEvents->m_pPrev == NULL);
        DT_ASSERT(pDrvEvents->m_pNext == NULL);      
        DtVectorEvent_Cleanup(pDrvEvents->m_PendingEvents);
        DtMemFreePool(pDrvEvents, CORE_TAG);
    }
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCfEvt implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the CfEvt stub
#define STUB_CFEVT_DEFAULT_PRECONDITIONS(pStub)      \
    DT_ASSERT(pStub!=NULL && pStub->m_Size>=sizeof(DtIoStubCfEvt))

// MACRO to cast a stub object to a DtIoStubCfEvt object
#define STUB_EVT        ((DtIoStubCfEvt*)pStub)
#define STUB_CF         ((DtCfEvt*)STUB_EVT->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtIoStubCfEvt_CreateQueue(DtIoStubCfEvt*);
static DtStatus  DtIoStubCfEvt_Init(DtIoStubCfEvt*);
static DtStatus  DtIoStubCfEvt_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int* pOutSize);
static DtStatus  DtIoStubCfEvt_OnCmdGetEvent(const DtIoStubCfEvt*, DtFileObject*, 
                                                          DtIoctlEventCmdGetEventOutput*);
static DtStatus  DtIoStubCfEvt_OnCmdRegister(const DtIoStubCfEvt*, DtFileObject*, 
                                                const DtIoctlEventCmdRegisterEventInput*);
static DtStatus  DtIoStubCfEvt_OnCmdSetEvent(const DtIoStubCfEvt*, DtFileObject*, 
                                                     const DtIoctlEventCmdSetEventInput*);
static DtStatus  DtIoStubCfEvt_OnCmdUnregister(const DtIoStubCfEvt*, DtFileObject*);
DT_UNUSED  static DtStatus  DtIoStubCfEvt_Requeue(DtIoStubCfEvt*, DtIoStubIoParams*);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EVENT;

static const DtIoctlProperties  IOSTUB_CF_EVT_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EVENT_CMD(
        DtIoStubCfEvt_OnCmd, 
        NULL, NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCfEvt - Public fuctions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfEvt_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubCfEvt_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubCfEvt));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfEvt_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubCfEvt*  DtIoStubCfEvt_Open(DtDf*  pCf)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoStubCfEvt*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    // Sanity checks
    CF_EVT_DEFAULT_PRECONDITIONS(pCf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubCfEvt, pCf, NULL, 
                                                              DtIoStubCfEvt_Close,
                                                              NULL,  // Use default IOCTL
                                                              IOSTUB_CF_EVT_IOCTLS);
    pStub = (DtIoStubCfEvt*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;

    // Init stub
    Status = DtIoStubCfEvt_Init(pStub);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutIoStub(ERR, CORE, pStub, "ERROR: failed to initialise stub");
        DtIoStubCfEvt_Close((DtIoStub*)pStub);
        return NULL;
    }
    return pStub;
}

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCfEvt - Private fuctions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfEvt_CreateQueue -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubCfEvt_CreateQueue(DtIoStubCfEvt*  pStub)
{
    DtStatus Status = DT_STATUS_OK;

#ifdef WINBUILD

    WDF_IO_QUEUE_CONFIG  QueueConfig;
    WDF_OBJECT_ATTRIBUTES  Attributes;
    NTSTATUS  NtStatus;

    // Sanity checks
    STUB_CFEVT_DEFAULT_PRECONDITIONS(pStub);
    
    // Initialize fast mutex
    DtFastMutexInit(&STUB_EVT->m_EventQueueMutex);

    // Prevent mutual access to event queue
    DtFastMutexAcquire(&STUB_EVT->m_EventQueueMutex);


    // Set SynchronizationScope to NONE to prevent the I/O request to be synchronized for
    // each device.
    WDF_OBJECT_ATTRIBUTES_INIT(&Attributes);
    Attributes.SynchronizationScope = WdfSynchronizationScopeNone;


    // Create EventQueue
    WDF_IO_QUEUE_CONFIG_INIT(&QueueConfig, WdfIoQueueDispatchManual);

    // Disable power managed queue. The events can/must be processed
    QueueConfig.PowerManaged = WdfFalse;

    NtStatus = WdfIoQueueCreate(pStub->m_pCore->m_Device.m_WdfDevice, &QueueConfig,
                                                    &Attributes, &STUB_EVT->m_EventQueue);

    if (!NT_SUCCESS(NtStatus))
    {
        DtDbgOutIoStubCf(ERR, EVT, pStub, "ERROR: WdfIoQueueCreate failed. Error: 0x%x", 
                                                                                NtStatus);
        Status = DT_STATUS_FAIL;
    }

    DtFastMutexRelease(&STUB_EVT->m_EventQueueMutex);

#endif
   return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfEvt_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubCfEvt_Init(DtIoStubCfEvt*  pStub)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    STUB_CFEVT_DEFAULT_PRECONDITIONS(pStub);

    // Init the event queue
    Status = DtIoStubCfEvt_CreateQueue(pStub);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutIoStub(ERR, CORE, pStub, "ERROR: failed to create event-queue");
        return Status;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfEvt_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCfEvt_OnCmd(
    const DtIoStub*  pStub, 
    DtIoStubIoParams*  pIoParams, 
    Int* pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    DtFileObject*  pFile = NULL;
    const DtIoctlEventCmdInput*  pInData = NULL;
    DtIoctlEventCmdOutput*  pOutData = NULL;

    // Sanity checks
    STUB_CFEVT_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_EVENT_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_Event;
    if (pIoParams->m_OutReqSize > 0)
    { 
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_Event;
    }
    pFile = (DtFileObject*)pIoParams->m_pFile;

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_EVENT_CMD_GET_EVENT:
        Status = DtIoStubCfEvt_OnCmdGetEvent(STUB_EVT, pFile, &pOutData->m_GetEvent);
        break;
    case DT_EVENT_CMD_REGISTER:
        Status = DtIoStubCfEvt_OnCmdRegister(STUB_EVT, pFile, &pInData->m_RegEvent);
        break;
    case DT_EVENT_CMD_SET_EVENT:
        Status = DtIoStubCfEvt_OnCmdSetEvent(STUB_EVT, pFile, &pInData->m_SetEvent);
        break;
    case DT_EVENT_CMD_UNREGISTER:
        Status = DtIoStubCfEvt_OnCmdUnregister(STUB_EVT, pFile);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }

    // For Windows requeue if indicated
#ifdef WINBUILD
    if (Status == DT_STATUS_REQUEUE)
        DtIoStubCfEvt_Requeue(STUB_EVT, pIoParams);
#endif
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfEvt_OnCmdGetEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCfEvt_OnCmdGetEvent(
    const DtIoStubCfEvt*  pStub,
    DtFileObject*  pFile,
    DtIoctlEventCmdGetEventOutput*  pOutData)
{
    DtStatus Status = DT_STATUS_OK;
    DtDriverEvent Event;

    // Sanity checks
    STUB_CFEVT_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData!=NULL);
    
    Status = DtCfEvt_Get(STUB_CF, pFile, NULL, &Event);

    pOutData->m_EventType = Event.m_EventType;
    pOutData->m_EventValue1 = Event.m_EventValue1;
    pOutData->m_EventValue2 = Event.m_EventValue2;

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfEvt_OnCmdRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCfEvt_OnCmdRegister(
    const DtIoStubCfEvt*  pStub,
    DtFileObject*  pFile,
    const DtIoctlEventCmdRegisterEventInput*  pInData)
{
    // Sanity checks
    STUB_CFEVT_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData!=NULL);

    return DtCfEvt_Register(STUB_CF, pFile,  pInData->m_EventType);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfEvt_OnCmdSetEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubCfEvt_OnCmdSetEvent(
    const DtIoStubCfEvt*  pStub, 
    DtFileObject* pFile, 
    const DtIoctlEventCmdSetEventInput* pInData)
{
    DtDriverEvent Event;
    Bool AddIfExists;

    // Sanity checks
    STUB_CFEVT_DEFAULT_PRECONDITIONS(pStub);

    Event.m_EventType = pInData->m_EventType;
    Event.m_EventValue1 = pInData->m_EventValue1;
    Event.m_EventValue2 = pInData->m_EventValue2;
    AddIfExists = (pInData->m_AddIfExists != 0);

    return DtCfEvt_Set(STUB_CF, Event, AddIfExists);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfEvt_OnCmdUnregister -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubCfEvt_OnCmdUnregister(
    const DtIoStubCfEvt*  pStub,
    DtFileObject*  pFile)
{
    // Sanity checks
    STUB_CFEVT_DEFAULT_PRECONDITIONS(pStub);

    return DtCfEvt_Unregister(STUB_CF, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfEvt_Dequeue -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubCfEvt_Dequeue(DtIoStubCfEvt*  pStub, DtDriverEvents*  pDrvEvents)
{
#ifdef WINBUILD
    WDF_REQUEST_PARAMETERS  Params;
    WDFREQUEST  Request;
    WDFREQUEST  Request2;
    NTSTATUS  NtStatus = STATUS_SUCCESS;
    size_t  BufSize;
    DtIoctlEventCmdOutput*  pOutBuf;
    DtFileObject*  pFile;

    // Sanity checks
    STUB_CFEVT_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pDrvEvents != NULL);
    pFile = &pDrvEvents->m_File;
    DT_ASSERT(pFile != NULL);

    // Prevent mutual access to event queue
    DtFastMutexAcquire(&pStub->m_EventQueueMutex);
    
    // Get all pending requests from queue for this file object
    for(;;)
    {
        WDF_REQUEST_PARAMETERS_INIT(&Params);
        
        NtStatus = WdfIoQueueFindRequest(pStub->m_EventQueue, NULL, 
                                               DtFileGetHandle(pFile), &Params, &Request);
        if (NtStatus != STATUS_SUCCESS) // Don't use the NT_SUCCESS macro here
            break;
        
        NtStatus = WdfIoQueueRetrieveFoundRequest(pStub->m_EventQueue, 
                                                                      Request, &Request2);
        
        WdfObjectDereference(Request);
        if (!NT_SUCCESS(NtStatus))
            break;
        
        if (NT_SUCCESS(NtStatus))
        {
            NtStatus = WdfRequestRetrieveOutputBuffer(Request2, 
                                     Params.Parameters.DeviceIoControl.OutputBufferLength,
                                     &pOutBuf, &BufSize);
        }

        if (NT_SUCCESS(NtStatus))
        {
            DtStatus  Status;
            DtDriverEvent Event;
            Status = DtCfEvt_Get(STUB_CF, NULL, pDrvEvents, &Event);
            pOutBuf->m_GetEvent.m_EventType = Event.m_EventType;
            pOutBuf->m_GetEvent.m_EventValue1 = Event.m_EventValue1;
            pOutBuf->m_GetEvent.m_EventValue2 = Event.m_EventValue2;      
            BufSize = sizeof(DtIoctlEventCmdGetEventOutput);

            // Use DtStatus in the driver-defined information field
            if (!DT_SUCCESS(Status))
               NtStatus = (NTSTATUS)Status;
        }

        if (!NT_SUCCESS(NtStatus))
            BufSize = 0;
        
        // Complete request
        WdfRequestCompleteWithInformation(Request2, NtStatus, (ULONG_PTR)BufSize);
    }

    DtFastMutexRelease(&pStub->m_EventQueueMutex);
#endif
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfEvt_Requeue -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubCfEvt_Requeue(DtIoStubCfEvt*  pStub, DtIoStubIoParams*  pIoParams)
{
#ifdef WINBUILD  
    NTSTATUS  NtStatus;

    // Sanity checks
    STUB_CFEVT_DEFAULT_PRECONDITIONS(pStub);

    // Prevent mutual access to event queue
    DtFastMutexAcquire(&pStub->m_EventQueueMutex);

    // Only get events commands can be requeued. Put this request into the event queue
    DT_ASSERT(pIoParams->m_Cmd == DT_EVENT_CMD_GET_EVENT); 
    DtDbgOutIoStubCf(MAX, EVT, pStub, "Request put in event queue");
    NtStatus = WdfRequestForwardToIoQueue(pIoParams->m_pIoctl->m_WdfRequest,
                                                                     pStub->m_EventQueue);
    if (NtStatus != STATUS_SUCCESS)
        WdfRequestCompleteWithInformation(pIoParams->m_pIoctl->m_WdfRequest,
                                                        (NTSTATUS)NtStatus, (ULONG_PTR)0);

    DtFastMutexRelease(&pStub->m_EventQueueMutex);
    
#endif
    return DT_STATUS_OK;
}



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtVectorEvent implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorEvent_Create -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtVectorEvent*  DtVectorEvent_Create(Int  Capacity, Int  GrowSize)
{
    // Vector will hold driver events
    return (DtVectorEvent*)DtVector_Create(Capacity, sizeof(DtDriverEvent), GrowSize);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorEvent_Cleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtVectorEvent_Cleanup(DtVectorEvent*  pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    DtVector_Cleanup((DtVector*)pTheVector);
}
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorEvent_Data -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDriverEvent*  DtVectorEvent_Data(DtVectorEvent* pTheVector)
{
    DT_ASSERT(pTheVector !=NULL);
    return (DtDriverEvent*)DtVector_Data((DtVector*)pTheVector);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorEvent_PushBack -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorEvent_PushBack(DtVectorEvent*  pTheVector, DtDriverEvent*  pEvent)
{
    DT_ASSERT(pTheVector!=NULL && pEvent!=NULL);
    return DtVector_PushBack((DtVector*)pTheVector, pEvent);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorEvent_PopFront -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorEvent_PopFront(DtVectorEvent*  pTheVector, DtDriverEvent* pEvent)
{
    DT_ASSERT(pTheVector!=NULL && pEvent!=NULL);
    return DtVector_PopFront((DtVector*)pTheVector, pEvent);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorEvent_Size -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtVectorEvent_Size(const DtVectorEvent*  pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Size((DtVector*)pTheVector);
}