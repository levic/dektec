//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtEvent.c *#*#*#*#*#*#*#*#*# (C) 2010-2012 DekTec
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

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtEvent +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtEventInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtEventInit(DtEvent* pDtEvent, Bool AutoReset)
{
#ifdef WINBUILD
    EVENT_TYPE  Type;
    if (AutoReset)
        Type = SynchronizationEvent;
    else
        Type = NotificationEvent;
    KeInitializeEvent(&pDtEvent->m_Event, Type, FALSE);
#else
    pDtEvent->m_AutoReset = AutoReset;
    pDtEvent->m_EventSet = FALSE;
    init_waitqueue_head(&pDtEvent->m_WaitQueueHead);
    DtFastMutexInit(&pDtEvent->m_Mutex);
#endif

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtEventSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtEventSet(DtEvent* pDtEvent)
{
#ifdef WINBUILD
    DT_ASSERT(KeGetCurrentIrql() <= DISPATCH_LEVEL);
    KeSetEvent(&pDtEvent->m_Event, 0, FALSE);
#else
    pDtEvent->m_EventSet = TRUE;
    wake_up_interruptible(&pDtEvent->m_WaitQueueHead);
#endif
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtEventReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtEventReset(DtEvent* pDtEvent)
{
#ifdef WINBUILD
    DT_ASSERT(KeGetCurrentIrql() <= DISPATCH_LEVEL);
    KeClearEvent(&pDtEvent->m_Event);
#else
    DtFastMutexAcquire(&pDtEvent->m_Mutex);
    pDtEvent->m_EventSet = FALSE;
    DtFastMutexRelease(&pDtEvent->m_Mutex);
#endif
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtEventWait -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtEventWait(DtEvent* pDtEvent, Int TimeoutMS)
{
    DtStatus  Status = DT_STATUS_FAIL;
#ifdef WINBUILD
    NTSTATUS  Result;
    LARGE_INTEGER  Timeout;
    LARGE_INTEGER*  pTimeout;
    DT_ASSERT((KeGetCurrentIrql()<=APC_LEVEL && (TimeoutMS>0 || TimeoutMS==-1)) ||
                                    (KeGetCurrentIrql()<=DISPATCH_LEVEL && TimeoutMS==0));

    if (TimeoutMS >= 0)
    {
        Timeout.QuadPart = (Int64)TimeoutMS * -10000;
        pTimeout = &Timeout;
    } else
        pTimeout = NULL;

    Result = KeWaitForSingleObject(&pDtEvent->m_Event, Executive, KernelMode, FALSE,
                                                                                pTimeout);
    if (Result == STATUS_SUCCESS)
        Status = DT_STATUS_OK;
    else if (Result == STATUS_TIMEOUT)
        Status = DT_STATUS_TIMEOUT;
#else
    long  Jiffies;

    DtFastMutexAcquire(&pDtEvent->m_Mutex);
    if (pDtEvent->m_EventSet)
    {
        if (pDtEvent->m_AutoReset)
            pDtEvent->m_EventSet = FALSE;
        Status = DT_STATUS_OK;
    } else
        Status = DT_STATUS_TIMEOUT;
    DtFastMutexRelease(&pDtEvent->m_Mutex);

    // Should we sleep?
    if ((Status!=DT_STATUS_OK) && (TimeoutMS!=0))
    {
        if (TimeoutMS < 0)
            Jiffies = MAX_JIFFY_OFFSET;
        else
            Jiffies = msecs_to_jiffies(TimeoutMS);

        while (TRUE)
        {
            Jiffies = wait_event_interruptible_timeout(pDtEvent->m_WaitQueueHead,
                                                   (pDtEvent->m_EventSet==TRUE), Jiffies);

            if (Jiffies < 0)
                break;

            if (Jiffies == 0)
            {
                Status = DT_STATUS_TIMEOUT;
                break;
            }

            if (Jiffies > 0)
            {
                DtFastMutexAcquire(&pDtEvent->m_Mutex);
                if (pDtEvent->m_EventSet)
                {
                    if (pDtEvent->m_AutoReset)
                        pDtEvent->m_EventSet = FALSE;
                    Status = DT_STATUS_OK;
                }
                DtFastMutexRelease(&pDtEvent->m_Mutex);

                if (Status == DT_STATUS_OK)
                    break;
            }

            if (TimeoutMS < 0)
                Jiffies = MAX_JIFFY_OFFSET;
        }
    }

    
#endif
    return Status;
}
