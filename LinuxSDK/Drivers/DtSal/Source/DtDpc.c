//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDpc.c *#*#*#*#*#*#*#*#*#* (C) 2010-2012 DekTec
//
// DtSal - Deferred Procedure Calls - Windows/Linux imlementation of DPC's
//
// This implementation is created to achieve the same results on Windows and Linux and to
// support an extended struct as a parameter for the schedule function.
// A DPC worker can never run twice at the same time, and can be queued only ones if 
// already running.
//
//
// Another solution is to use a run counter in the DtDpcWorker. This counter should be
// atomic incremented at the beginning of the function. If the counter was 0, the function
// should execute a loop or else exit. At the end of the loop the counter must be atomic
// decremented and the loop should exit if the counter was 1 (last run).
// This is a very easy implementation, but it doesn't support specific arguments for each
// queued worker.
//
// Example:
//
// DoDpcWorker
//     if (DtAtomicInc(&RunCount) > 0)
//         return;
//     do
//         m_pWorker()
//     while ((DtAtomicDec(&RunCount) > 1)
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtSal.h>

#define  DPC_STATE_BIT_RUNNING  1
#define  DPC_STATE_BIT_QUEUING  2
#define  DPC_STATE_BIT_QUEUED   4


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDpc +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDpcWorker -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#ifdef WINBUILD
KDEFERRED_ROUTINE  DtDpcWorker;
void  DtDpcWorker(KDPC* pKdpc, void* pContext, void* pArg1, void* pArg2)
#else
// We've had problems in the past with using "unsigned long" instead of "void*" were
// an unsigned long was smaller and as a result some bits were cut off. This assert
// is here to make sure that if this happens again we notice it immediately. If this
// assert triggers, there are problems with free_pages in DtUtility.c too.
extern const int g_StaticAssert[sizeof(unsigned long) >= sizeof(void*)];

void  DtDpcWorker(unsigned long pContext)
#endif
{
    Bool  DeQueue;
    Int  OldState;
    DtDpc*  pDpc = (DtDpc*)pContext;

    // Assume running...
    pDpc->m_pWorker(&pDpc->m_Args);

    do
    {
        DeQueue = FALSE;

        // Done, set to idle
        OldState = DtAtomicCompareExchange((Int*)&pDpc->m_State, DPC_STATE_BIT_RUNNING,
                                                                                       0);
        if (OldState != DPC_STATE_BIT_RUNNING)
        {
            // Failed, maybe queuing pending?
            if (OldState == (DPC_STATE_BIT_RUNNING|DPC_STATE_BIT_QUEUING))
            {
                // Just set running flag to zero, but hold queuing
                OldState = DtAtomicCompareExchange((Int*)&pDpc->m_State,
                                              DPC_STATE_BIT_RUNNING|DPC_STATE_BIT_QUEUING,
                                              DPC_STATE_BIT_QUEUING);
                if (OldState != (DPC_STATE_BIT_RUNNING|DPC_STATE_BIT_QUEUING))
                {
                    // Whoops failed, queuing is now done so we can dequeue...
                    DT_ASSERT(OldState == (DPC_STATE_BIT_RUNNING|DPC_STATE_BIT_QUEUING
                                                                  |DPC_STATE_BIT_QUEUED));
                    DeQueue = TRUE;
                }
            } else {
                DT_ASSERT(OldState ==
                      (DPC_STATE_BIT_RUNNING|DPC_STATE_BIT_QUEUING|DPC_STATE_BIT_QUEUED));
                DeQueue = TRUE;
            }
        }

        if (DeQueue)
        {
            // Copy args
            pDpc->m_Args = pDpc->m_QueuedArgs;

            // Release queue, but keep running flag
            DtAtomicCompareExchange((Int*)&pDpc->m_State,
                         DPC_STATE_BIT_RUNNING|DPC_STATE_BIT_QUEUING|DPC_STATE_BIT_QUEUED,
                         DPC_STATE_BIT_RUNNING);

            // Run worker again with new args
            pDpc->m_pWorker(&pDpc->m_Args);
        }
    } while (DeQueue);
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public API +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDpcInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDpcInit(DtDpc* pDpc, pDtDpcWorker pWorker, Bool QueueIfRunning)
{
    DtStatus  Result = DT_STATUS_OK;
      
    // Store settings
    pDpc->m_pWorker = pWorker;
    pDpc->m_QueueIfRunning = QueueIfRunning;

    pDpc->m_State = 0;
#ifdef _DEBUG
    pDpc->m_SchedulingEnabled = TRUE;
#endif

    // Initialize kernel DPC/tasklet
#ifdef WINBUILD
    KeInitializeDpc(&pDpc->m_Kdpc, DtDpcWorker, pDpc);
#else
    tasklet_init(&pDpc->m_Tasklet, DtDpcWorker, (unsigned long)pDpc);
#endif
    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDpcSchedule -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDpcSchedule(DtDpc* pDpc, DtDpcArgs* pArgs)
{
    DtStatus  Result = DT_STATUS_OK;
    Int  OldState;
    Bool  DoRun = FALSE;
    Bool  DoQueue = FALSE;

    DT_ASSERT(pDpc->m_SchedulingEnabled);

    // Try to set running from idle state
    OldState = DtAtomicCompareExchange((Int*)&pDpc->m_State, 0, DPC_STATE_BIT_RUNNING);
    if (OldState == 0)
        // Successfully running
        DoRun = TRUE;
    else if (pDpc->m_QueueIfRunning)
    {
        // Try to set Queuing
        OldState = DtAtomicCompareExchange((Int*)&pDpc->m_State, DPC_STATE_BIT_RUNNING,
                                             DPC_STATE_BIT_RUNNING|DPC_STATE_BIT_QUEUING);
        if (OldState == DPC_STATE_BIT_RUNNING)
            // Successfully set to queuing
            DoQueue = TRUE;
        // Not running anymore?
        // Try to set running again to be sure a full execution of the worker is pending
        // after the call the DtDpcSchedule... 
        else if ((OldState&DPC_STATE_BIT_RUNNING) == 0)
        {
            // Retry to set running from idle state
            OldState = DtAtomicCompareExchange((Int*)&pDpc->m_State, 0, 
                                                                   DPC_STATE_BIT_RUNNING);
            if (OldState == 0)
                // Successfully set to running
                DoRun = TRUE;
            else
                Result = DT_STATUS_IN_USE;
        } else
            Result = DT_STATUS_IN_USE;
    }

    if (!DT_SUCCESS(Result))
        return Result;

    // Queue DPC?
    if (DoQueue)
    {
        // Copy arguments
        pDpc->m_QueuedArgs = *pArgs;

        // Set to queued (running|queuing|queued)
        OldState = DtAtomicCompareExchange((Int*)&pDpc->m_State,
                        DPC_STATE_BIT_RUNNING|DPC_STATE_BIT_QUEUING,
                        DPC_STATE_BIT_RUNNING|DPC_STATE_BIT_QUEUING|DPC_STATE_BIT_QUEUED);
        // Check if we failed because we were not running anymore
        if (OldState == DPC_STATE_BIT_QUEUING)
        {
            // Choose running slot --> try to set from queuing to running instead of
            // queued
            OldState = DtAtomicCompareExchange((Int*)&pDpc->m_State,
                                            DPC_STATE_BIT_QUEUING, DPC_STATE_BIT_RUNNING);
            if (OldState == DPC_STATE_BIT_QUEUING)
                DoRun = TRUE;
            else {
                Result = DT_STATUS_IN_USE;
                // Can not happen?
                DT_ASSERT(FALSE);
            }
        }
    }

    // Start initial DPC?
    if (DoRun)
    {
        pDpc->m_Args = *pArgs;

#ifdef WINBUILD
        KeInsertQueueDpc(&pDpc->m_Kdpc, NULL, NULL);
#else
        tasklet_schedule(&pDpc->m_Tasklet);
#endif

        // Running flag is already set...
    }

    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDpcWaitForCompletion -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function can be used to wait for pending DPC's during powerdown.
//
// Precondition: The DPC should not be scheduled anymore when this routine is called.
//               Else, this routine could never end!!!
void DtDpcWaitForCompletion(DtDpc* pDpc)
{
#ifdef _DEBUG
    pDpc->m_SchedulingEnabled = FALSE;
#endif

    while (pDpc->m_State != 0)
    {
        // Delete queued item (only if fully queued)
        DtAtomicCompareExchange((Int*)&pDpc->m_State,
                         DPC_STATE_BIT_RUNNING|DPC_STATE_BIT_QUEUING|DPC_STATE_BIT_QUEUED,
                         DPC_STATE_BIT_RUNNING);

        DtSleep(10);
    }

#ifdef _DEBUG
    pDpc->m_SchedulingEnabled = TRUE;
#endif
}
