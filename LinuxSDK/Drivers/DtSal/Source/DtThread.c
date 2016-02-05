//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtThread.c *#*#*#*#*#*#*#*#* (C) 2011-2016 DekTec
//
// DtSal - Thread Function Calls - Windows/Linux imlementation of kernel Threads
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtSal.h>

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- NO_KTHREAD_SUPPORT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Support for kthread functions if they are not available in the Linux kernel
//
// DtLinuxXXXX defines and functions are used to prevent collisions with Linux
// distributions who back-ported  the kthread functions to previous Linux kernel versions
//

#ifdef LINBUILD
// Function defines
#ifdef NO_KTHREAD_SUPPORT

struct DtLinuxKThreadCreateInfo
{
    // Information passed to DtLinuxKThread from keventd
    int  (*m_pThreadfn)(void *pData);
    void  *m_pData;
    struct completion  m_Started;

    // Result passed back to dta1xx_kthread_create() from keventd
    struct task_struct*  m_pResult;
    struct completion  m_Done;
};

struct DtLinuxKThreadStopInfo
{
    struct task_struct*  m_pTask;
    int  m_Err;
    struct completion  m_Done;
    volatile int  m_Wakeupdone;
};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)
static DEFINE_SEMAPHORE(g_KThreadStopLock);
#else
static DECLARE_MUTEX(g_KThreadStopLock);
#endif

static struct DtLinuxKThreadStopInfo  g_KThreadStopInfo;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declaration -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static struct task_struct*  DtLinuxKThreadCreate(int (*pThreadfn)(void* pData), 
                                                 void* pData, const char  Namefmt[], ...);


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtLinuxKThreadRun -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#define  DtLinuxKThreadRun(threadfn, data, namefmt, ...)                     \
    ({                                                                       \
        struct task_struct* __k   \
            = DtLinuxKThreadCreate(threadfn, data, namefmt, ## __VA_ARGS__); \
        if (!IS_ERR(__k)) wake_up_process(__k);                              \
        __k;                                                                 \
    })

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtLinuxKThreadShouldStop -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
int  DtLinuxKThreadShouldStop(void)
{
    Bool  IsFromCurTask = (g_KThreadStopInfo.m_pTask == current);
    if (IsFromCurTask)
    {
        // Wait until wakeup schedule is done
        while(g_KThreadStopInfo.m_Wakeupdone == 0) 
            DtSleep(1000);  // 1 ms
    }
    return (IsFromCurTask ? 1 : 0);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtLinuxKThreadExitFiles -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static void  DtLinuxKThreadExitFiles(void)
{
    struct fs_struct*  pFs;
    struct task_struct*  pTask = current;

    exit_fs(pTask);       // current->fs->count--;
    pFs = init_task.fs;
    pTask->fs = pFs;
    atomic_inc(&pFs->count);
    exit_files(pTask);
    current->files = init_task.files;
    atomic_inc(&pTask->files->count);
}

#ifdef CONFIG_SMP
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,1)
    #define CPU_MASK_ALL cpu_online_map
#endif
#endif // CONFIG_SMP

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtLinuxKThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static int  DtLinuxKThread(void* pContext)
{
    struct DtLinuxKThreadCreateInfo*  pCreate = pContext;
    int (*pThreadfn)(void *pData);
    void*  pData;

    //sigset_t blocked;
    int  Ret = -EINTR;

    DtLinuxKThreadExitFiles();
    
    // Copy data: it's on keventd's stack
    pThreadfn = pCreate->m_pThreadfn;
    pData = pCreate->m_pData;

    // Flush all signals (in case we're not from keventd)
    flush_signals(current);

    // By default we can run anywhere, unlike keventd
    set_cpus_allowed(current, CPU_MASK_ALL);

    // OK, tell user we're spawned, wait for stop or wakeup
    __set_current_state(TASK_INTERRUPTIBLE);
    complete(&pCreate->m_Started);
    schedule();

    if (DtLinuxKThreadShouldStop() == 0)
        Ret = pThreadfn(pData);

    // It might have exited on its own, w/o kthread_stop
    if (DtLinuxKThreadShouldStop()) 
    {
        g_KThreadStopInfo.m_Err = Ret;
        complete(&g_KThreadStopInfo.m_Done);
    }
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtLinuxCreateKThreadWorkItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void DtLinuxCreateKThreadWorkItem(void* pContext)
{
    struct DtLinuxKThreadCreateInfo*  pCreate = pContext;
    int pid;

    // We want our own signal handler (we take no signals by default)
    pid = kernel_thread(DtLinuxKThread, pCreate, CLONE_FS | CLONE_FILES | SIGCHLD);
    if (pid < 0) 
        pCreate->m_pResult = ERR_PTR(pid);
    else 
    {
        wait_for_completion(&pCreate->m_Started);
#ifdef CONFIG_SMP
        read_lock(&tasklist_lock);
#endif
        pCreate->m_pResult = find_task_by_pid(pid);
#ifdef CONFIG_SMP
        read_unlock(&tasklist_lock);
#endif
    }
    complete(&pCreate->m_Done);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- *DtaKThreadCreate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
struct task_struct*  DtLinuxKThreadCreate(
    int (*pThreadfn)(void *pData),
    void*  pData, 
    const char  Namefmt[],
    ...)
{
    struct DtLinuxKThreadCreateInfo  Create;
    DECLARE_WORK(WorkItem, DtLinuxCreateKThreadWorkItem, &Create);

    Create.m_pThreadfn = pThreadfn;
    Create.m_pData = pData;
    init_completion(&Create.m_Started);
    init_completion(&Create.m_Done);

    
    schedule_work(&WorkItem);
    wait_for_completion(&Create.m_Done);
    
    if (!IS_ERR(Create.m_pResult))
    {
        va_list args;
        va_start(args, Namefmt);
        vsnprintf(Create.m_pResult->comm, sizeof(Create.m_pResult->comm),
              Namefmt, args);
        va_end(args);
    }

    return Create.m_pResult;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- kthread_stop -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
int  DtLinuxKThreadStop(struct task_struct* pTask)
{
    Int Ret;
    down(&g_KThreadStopLock);

    // Must init completion *before* thread sees g_dta1xx_kthread_stop_info.k
    init_completion(&g_KThreadStopInfo.m_Done);
    // Init synchronise wakeup
    g_KThreadStopInfo.m_Wakeupdone = 0;
    wmb();

    // Now set DtaKThreadShouldStop() to true, and wake it up
    g_KThreadStopInfo.m_pTask = pTask;
    
    wake_up_process(pTask);
    // Signal wakeup done
    g_KThreadStopInfo.m_Wakeupdone = 1;
    
    // Once it dies, reset stop ptr, gather result and we're done
    wait_for_completion(&g_KThreadStopInfo.m_Done);
    g_KThreadStopInfo.m_pTask = NULL;
    Ret = g_KThreadStopInfo.m_Err;
    up(&g_KThreadStopLock);
    return Ret;
}

#else
    #define DtLinuxKThreadCreate        kthread_create
    #define DtLinuxKThreadRun           kthread_run
    #define DtLinuxKThreadStop          kthread_stop
#endif // NO_KTHREAD_SUPPORT

#endif // LINBUILD

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtThreadWorker -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#ifdef WINBUILD
KSTART_ROUTINE  DtThreadWorker;
void  DtThreadWorker(void* pContext)
#else
Int  DtThreadWorker(void* pContext)
#endif
{
    DtThread*  pThread = (DtThread*)pContext;
    
    DtDbgOut(MAX, SAL_THREAD, "Start");
    DT_ASSERT(pThread->m_pWorker != NULL);
    
    pThread->m_pWorker(pThread, pThread->m_pContext);


    // The thread must not be stopped without waiting for
    // the DtStatus  DtThreadStop(DtThread* pThread) function.
    // because Linux can not handle that
    DT_ASSERT(pThread->m_StopThread);
    
    DtDbgOut(MAX, SAL_THREAD, "Exit");
#ifdef  WINBUILD
    PsTerminateSystemThread(STATUS_SUCCESS);
#else
    return 0;
#endif
    
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtThreadInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtThreadInit(DtThread* pThread, pDtThreadWorker pWorker, void* pContext)
{
    DtDbgOut(MAX, SAL_THREAD, "Start");
    pThread->m_Started = FALSE;
    pThread->m_pWorker = pWorker;
    pThread->m_pContext = pContext;
    DtEventInit(&pThread->m_StopEvent, FALSE);
    DtDbgOut(MAX, SAL_THREAD, "Exit");
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtThreadStart -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtThreadStart(DtThread* pThread)
{
#ifdef WINBUILD
    OBJECT_ATTRIBUTES  ObjectAttrib;
    NTSTATUS  NtStatus;
    HANDLE  hThread;
    DT_ASSERT(KeGetCurrentIrql() == PASSIVE_LEVEL);
#endif
    DT_ASSERT(pThread->m_Started == FALSE);
    DtDbgOut(MAX, SAL_THREAD, "Start");

    pThread->m_StopThread = FALSE;
    DtEventReset(&pThread->m_StopEvent);

#ifdef WINBUILD
    InitializeObjectAttributes(&ObjectAttrib, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
    NtStatus = PsCreateSystemThread(&hThread, THREAD_ALL_ACCESS, &ObjectAttrib, NULL,
                                                           NULL, DtThreadWorker, pThread);
    if (!NT_SUCCESS(NtStatus))
        return DT_STATUS_OUT_OF_RESOURCES;
    ObReferenceObjectByHandle(hThread, THREAD_ALL_ACCESS, NULL, KernelMode,
                                                      (PVOID*)&pThread->m_pKThread, NULL);
    // We will not use the handle, so we can close it.
    ZwClose(hThread);
#else
    pThread->m_pTask = DtLinuxKThreadRun(DtThreadWorker, pThread, "DTSAL");
    if (IS_ERR(pThread->m_pTask))
    {
        pThread->m_pTask = NULL;
        return DT_STATUS_OUT_OF_RESOURCES;
    }
#endif
    pThread->m_Started = TRUE;
    DtDbgOut(MAX, SAL_THREAD, "Exit");

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtThreadStop -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Stop the thread. Executed by another process. This function blocks until the
// thread is stopped.
//
DtStatus  DtThreadStop(DtThread* pThread)
{
    DtDbgOut(MAX, SAL_THREAD, "Start");

    if (!pThread->m_Started)
        return DT_STATUS_OK;

    // Stop and cleanup
    pThread->m_StopThread = TRUE;
    DtEventSet(&pThread->m_StopEvent);

#ifdef WINBUILD
    KeWaitForSingleObject(pThread->m_pKThread, Executive, KernelMode, FALSE, NULL);
    ObDereferenceObject(pThread->m_pKThread);
#else
    DtLinuxKThreadStop(pThread->m_pTask);
#endif
    pThread->m_Started = FALSE;
    
    DtDbgOut(MAX, SAL_THREAD, "Exit");
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtThreadShouldStop -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns: True if the thread should stop. 
//
Bool  DtThreadShouldStop(DtThread* pThread)
{
    return pThread->m_StopThread;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtThreadWaitForStop -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Waits until the thread should stop
//
DtStatus  DtThreadWaitForStop(DtThread* pThread)
{
#ifdef WINBUILD
    return DtEventWait(&pThread->m_StopEvent, -1);
#else
    DtStatus  Status = DtEventWait(&pThread->m_StopEvent, -1);
    if (Status == DT_STATUS_OK)
    {
        while (!DtLinuxKThreadShouldStop())
            DtSleep(100);
    }
    return Status;
#endif
}

#ifdef WINBUILD
//.-.-.-.-.-.-.-.-.-.-.-.-.- DtThreadWaitForStopOrEventInternal -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Windows does not support macro's with return values
//
DtStatus  DtThreadWaitForStopOrEventInternal(DtThread* pThread, DtEvent* pEvent)
{  DtStatus  Status = DT_STATUS_CANCELLED;
   NTSTATUS  NtStatus;
   void*  pEvents[] = {&pThread->m_StopEvent.m_Event, &pEvent->m_Event};
    
   NtStatus = KeWaitForMultipleObjects(2, pEvents, WaitAny, Executive, KernelMode, FALSE,
                                                                              NULL, NULL);
    if (NtStatus == STATUS_WAIT_0)
        return DT_STATUS_CANCELLED;
    else if (NtStatus == STATUS_WAIT_0+1)
        return DT_STATUS_OK;
    else
        DT_ASSERT(FALSE);
    return Status;
}
#endif
