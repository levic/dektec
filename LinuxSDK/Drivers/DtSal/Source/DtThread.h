//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtThread.h *#*#*#*#*#*#*#*#* (C) 2011-2016 DekTec
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

#ifndef DT_THREAD_H
#define DT_THREAD_H

typedef struct  _DtThread  DtThread;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- pDtThreadWorker -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef void (*pDtThreadWorker)(DtThread* pThread, void* pContext);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
struct _DtThread
{
    pDtThreadWorker  m_pWorker;
    void*  m_pContext;
    DtEvent  m_StopEvent;
    Bool  m_StopThread;
    Bool  m_Started;
#ifdef WINBUILD
    PKTHREAD  m_pKThread;
#else
    struct task_struct*  m_pTask;
#endif
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Private API -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#ifdef  WINBUILD
DtStatus  DtThreadWaitForStopOrEventInternal(DtThread* pThread, DtEvent* pEvent);
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtThreadWaitForStopOrEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Condition: a C expression for the event to wait for
// Returns: DT_STATUS_CANCELLED: Thread must be stopped
//          DT_STATUS_OK: Event is signalled/Condition is true
//          
#ifdef WINBUILD
#define DtThreadWaitForStopOrEvent(pThread, pEvent)                             \
   DtThreadWaitForStopOrEventInternal(pThread, pEvent);
#else
#ifdef NO_KTHREAD_SUPPORT
int  DtLinuxKThreadShouldStop(void);
#else
#define DtLinuxKThreadShouldStop  kthread_should_stop
#endif
#define  DtThreadWaitForStopOrEvent(pThread, pEvent)                            \
({  DtStatus  __DtStatus = DT_STATUS_CANCELLED;                                 \
    Int  Ret = 0;                                                               \
    if (!(pEvent)->m_EventSet)                                                  \
        Ret = wait_event_interruptible((pEvent)->m_WaitQueueHead,               \
                                                   (pEvent)->m_EventSet ||      \
                                                   DtLinuxKThreadShouldStop()); \
    if (DtLinuxKThreadShouldStop() || Ret!=0)                                   \
        __DtStatus = DT_STATUS_CANCELLED;                                       \
    else {                                                                      \
        __DtStatus = DT_STATUS_OK;                                              \
        if ((pEvent)->m_AutoReset)                                              \
            (pEvent)->m_EventSet = FALSE;                                       \
    }                                                                           \
    __DtStatus;                                                                 \
})
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public API -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtThreadInit(DtThread* pThread, pDtThreadWorker pWorker, void* pContext);
DtStatus  DtThreadStart(DtThread* pThread);
DtStatus  DtThreadStop(DtThread* pThread);
DtStatus  DtThreadWaitForStop(DtThread* pThread);
Bool  DtThreadShouldStop(DtThread* pThread);

// The following macro function is defined somewhere else but is also public:
//#define   DtThreadWaitForStopOrEvent(DtThread* pThread, DtEvent* pEvent)

#endif
