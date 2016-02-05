//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtWorkItem.c *#*#*#*#*#*#*#*# (C) 2011-2016 DekTec
//
// DtSal - WorkItem Calls - Windows/Linux imlementation of WorkItems
// (Identical design as DtDpc)
//
// This implementation is created to achieve the same results on Windows and Linux and to
// support an extended struct as a parameter for the schedule function.
// A WorkItem worker can never run twice at the same time, and can be queued only ones if
// already running.
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtSal.h>

#define  WI_STATE_BIT_RUNNING  1
#define  WI_STATE_BIT_QUEUING  2
#define  WI_STATE_BIT_QUEUED   4

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtWorkItemStructDelete -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtWorkItemStructDelete(DtWorkItemStruct* pWorkItemStruct)
{
#ifdef WINBUILD
#ifdef USES_NDIS
    // Nothing to do
#else
    IoFreeWorkItem(pWorkItemStruct->m_pIoWorkItem);
#endif // USES_NDIS
#else
    //kfree(pWorkItemStruct->m_pWork);
#endif
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtWorkItemStructCreate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtWorkItemStructCreate(
    DtWorkItemStruct*  pWorkItemStruct, 
    DtDvcObject*  pDevice)
{
#ifdef WINBUILD
#ifdef USES_KMDF
    pWorkItemStruct->m_pIoWorkItem = IoAllocateWorkItem(
                                       WdfDeviceWdmGetDeviceObject(pDevice->m_WdfDevice));
    if (pWorkItemStruct->m_pIoWorkItem == NULL)
        return DT_STATUS_OUT_OF_RESOURCES;
#else
#ifdef USES_NDIS
    // Nothing to do
#else
    pWorkItemStruct->m_pIoWorkItem = IoAllocateWorkItem(pDevice->m_pWdmDevice);
    if (pWorkItemStruct->m_pIoWorkItem == NULL)
        return DT_STATUS_OUT_OF_RESOURCES;
#endif // USES_NDIS
#endif // USES_KMDF
#else
    //pWorkItemStruct->m_pWork = kmalloc(sizeof(struct work_struct), GFP_ATOMIC);
    //if (pWorkItemStruct->m_pWork == NULL)
    //    return DT_STATUS_OUT_OF_MEMORY;
#endif
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtWorkItemCopy -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtWorkItemStructCopy(
    DtWorkItemStruct*  pWorkItemStructDst, 
    DtWorkItemStruct*  pWorkItemStructSrc)
{
    DtMemCopy(pWorkItemStructDst, pWorkItemStructSrc, sizeof(DtWorkItemStruct));
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtWorkItemFinish -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtWorkItemFinish(DtWorkItem* pWorkItem)
{
    return DtWorkItemStructDelete(&pWorkItem->m_WorkStruct);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtWorkItemWorker -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#ifdef WINBUILD
#ifdef USES_NDIS
void  DtWorkItemWorker(PNDIS_WORK_ITEM pNdisWorkItem, void* pContext)
#else
IO_WORKITEM_ROUTINE  DtWorkItemWorker;
void  DtWorkItemWorker(PDEVICE_OBJECT pDeviceObject, void* pContext)
#endif // USES_NDIS
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
void  DtWorkItemWorker(struct work_struct* pWorkStruct)
#else
void  DtWorkItemWorker(void* pContext)
#endif
#endif
{
    Bool  DeQueue;
    Int  OldState;
    DtWorkItem*  pWorkItem;
#ifdef LINBUILD
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
    void*  pContext;
    pContext = container_of(pWorkStruct, struct _DtWorkItem, m_WorkStruct.m_Work);
#endif
#endif

    pWorkItem = pContext;
    // Assume running...
    pWorkItem->m_pWorker(&pWorkItem->m_Args);

    // DtWorkItemFinish must be done before running bit is cleared.
    DtWorkItemFinish(pWorkItem);
    
    do
    {
        DeQueue = FALSE;

        // Done, set to idle
        OldState = DtAtomicCompareExchange((Int*)&pWorkItem->m_State, 
                                                                 WI_STATE_BIT_RUNNING, 0);
        if (OldState != WI_STATE_BIT_RUNNING)
        {
            // Failed, maybe queuing pending?
            if (OldState == (WI_STATE_BIT_RUNNING|WI_STATE_BIT_QUEUING))
            {
                // Just set running flag to zero, but hold queuing
                OldState = DtAtomicCompareExchange((Int*)&pWorkItem->m_State,
                                                WI_STATE_BIT_RUNNING|WI_STATE_BIT_QUEUING,
                                                WI_STATE_BIT_QUEUING);
                if (OldState != (WI_STATE_BIT_RUNNING|WI_STATE_BIT_QUEUING))
                {
                    // Whoops failed, queuing is now done so we can dequeue...
                    DT_ASSERT(OldState == (WI_STATE_BIT_RUNNING|WI_STATE_BIT_QUEUING
                                                                   |WI_STATE_BIT_QUEUED));
                    DeQueue = TRUE;
                }
            } else {
                DT_ASSERT(OldState ==
                         (WI_STATE_BIT_RUNNING|WI_STATE_BIT_QUEUING|WI_STATE_BIT_QUEUED));
                DeQueue = TRUE;
            }
        }

        if (DeQueue)
        {
            // Copy args
            pWorkItem->m_Args = pWorkItem->m_QueuedArgs;

            // Release queue, but keep running flag
            DtAtomicCompareExchange((Int*)&pWorkItem->m_State,
                            WI_STATE_BIT_RUNNING|WI_STATE_BIT_QUEUING|WI_STATE_BIT_QUEUED,
                            WI_STATE_BIT_RUNNING);

            // Run worker again with new args
            pWorkItem->m_pWorker(&pWorkItem->m_Args);
        }
    } while (DeQueue);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtWorkItemInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtWorkItemInit(
    DtWorkItem*  pWorkItem, 
    pDtWorkItemWorker  pWorker,
    Bool  QueueIfRunning, 
    DtDvcObject*  pDevice)
{
    // Store settings
    pWorkItem->m_pWorker = pWorker;
    pWorkItem->m_QueueIfRunning = QueueIfRunning;
    pWorkItem->m_pDevice = pDevice;

    pWorkItem->m_State = 0;
#ifdef _DEBUG
    pWorkItem->m_SchedulingEnabled = TRUE;
#endif
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtWorkItemSchedule -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtWorkItemSchedule(DtWorkItem* pWorkItem, DtWorkItemArgs* pArgs)
{
    DtStatus  Result = DT_STATUS_OK;
    Int  OldState;
    Bool  DoRun = FALSE;
    Bool  DoQueue = FALSE;
    DtWorkItemStruct  WorkItemStruct;

    DT_ASSERT(pWorkItem->m_SchedulingEnabled);

    // Allocate workitem
    Result = DtWorkItemStructCreate(&WorkItemStruct, pWorkItem->m_pDevice);
    if (!DT_SUCCESS(Result))
        return Result;
    
    // Try to set running from idle state
    OldState = DtAtomicCompareExchange((Int*)&pWorkItem->m_State, 0, 
                                                                    WI_STATE_BIT_RUNNING);
    if (OldState == 0)
        // Successfully running
        DoRun = TRUE;
    else if (pWorkItem->m_QueueIfRunning)
    {
        // Try to set Queuing
        OldState = DtAtomicCompareExchange((Int*)&pWorkItem->m_State, 
                                               WI_STATE_BIT_RUNNING,
                                               WI_STATE_BIT_RUNNING|WI_STATE_BIT_QUEUING);
        if (OldState == WI_STATE_BIT_RUNNING)
            // Successfully set to queuing
            DoQueue = TRUE;
        // Not running anymore?
        // Try to set running again to be sure a full execution of the worker is pending
        // after the call the DtDpcSchedule... 
        else if ((OldState&WI_STATE_BIT_RUNNING) == 0)
        {
            // Retry to set running from idle state
            OldState = DtAtomicCompareExchange((Int*)&pWorkItem->m_State, 0, 
                                                                    WI_STATE_BIT_RUNNING);
            if (OldState == 0)
                // Successfully set to running
                DoRun = TRUE;
            else
                Result = DT_STATUS_IN_USE;
        } else
            Result = DT_STATUS_IN_USE;
    }

    if (!DT_SUCCESS(Result))
    {
        // Delete workitem
        DtWorkItemStructDelete(&WorkItemStruct);
        return Result;
    }

    // Queue WorkItem?
    if (DoQueue)
    {
        // Copy arguments
        pWorkItem->m_QueuedArgs = *pArgs;
        
        // Set to queued (running|queuing|queued)
        OldState = DtAtomicCompareExchange((Int*)&pWorkItem->m_State,
                           WI_STATE_BIT_RUNNING|WI_STATE_BIT_QUEUING,
                           WI_STATE_BIT_RUNNING|WI_STATE_BIT_QUEUING|WI_STATE_BIT_QUEUED);
        // Check if we failed because we were not running anymore
        if (OldState == WI_STATE_BIT_QUEUING)
        {
            // Choose running slot --> try to set from queuing to running instead of
            // queued
            OldState = DtAtomicCompareExchange((Int*)&pWorkItem->m_State, 
                                              WI_STATE_BIT_QUEUING, WI_STATE_BIT_RUNNING);
            if (OldState == WI_STATE_BIT_QUEUING)
                DoRun = TRUE;
            else {
                Result = DT_STATUS_IN_USE;
                // Can not happen?
                DT_ASSERT(FALSE);
            }
        }
    } 

    // Start initial WorkItem?
    if (DoRun)
    {
        pWorkItem->m_Args = *pArgs;
        DtWorkItemStructCopy(&pWorkItem->m_WorkStruct, &WorkItemStruct);

#ifdef WINBUILD
#ifdef USES_NDIS
        NdisInitializeWorkItem(&pWorkItem->m_WorkStruct.m_NdisWorkItem,
                                                             DtWorkItemWorker, pWorkItem);
        NdisScheduleWorkItem(&pWorkItem->m_WorkStruct.m_NdisWorkItem);
#else
        IoQueueWorkItem(pWorkItem->m_WorkStruct.m_pIoWorkItem, DtWorkItemWorker,
                                                             DelayedWorkQueue, pWorkItem);
#endif // USES_NDIS
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
        // INIT_WORK has been redefined in kernel version 2.6.20 (no 3rd context parameter)
        INIT_WORK(&pWorkItem->m_WorkStruct.m_Work, DtWorkItemWorker);
#else
        INIT_WORK(&pWorkItem->m_WorkStruct.m_Work, DtWorkItemWorker, pWorkItem);
#endif
        schedule_work(&pWorkItem->m_WorkStruct.m_Work);
#endif
        // Running flag is already set...
    } else // We do not need the WorkItemStruct.
        DtWorkItemStructDelete(&WorkItemStruct);

    return Result;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtWorkItemWaitForCompletion -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function can be used to wait for pending WorkItems during powerdown.
//
// Precondition: The WorkItems should not be scheduled anymore when this routine is called.
//               Else, this routine could never end!!!
void  DtWorkItemWaitForCompletion(DtWorkItem* pWorkItem)
{
#ifdef _DEBUG
    pWorkItem->m_SchedulingEnabled = FALSE;
#endif

    while (pWorkItem->m_State != 0)
    {
        // Delete queued item (only if fully queued)
        DtAtomicCompareExchange((Int*)&pWorkItem->m_State,
                            WI_STATE_BIT_RUNNING|WI_STATE_BIT_QUEUING|WI_STATE_BIT_QUEUED,
                            WI_STATE_BIT_RUNNING);

        DtSleep(10);
    }

#ifdef _DEBUG
    pWorkItem->m_SchedulingEnabled = TRUE;
#endif
}

