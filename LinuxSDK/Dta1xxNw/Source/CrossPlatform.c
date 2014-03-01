//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* CrossPlatform.c *#*#*#*#*#*#*#*#*#*# (C) 2007 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dta1xx driver, please refer to the Dta1xx readme

// Include files
#include "../Include/CrossPlatform.h"

#ifdef WINBUILD

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TryLockMutexObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
__inline BOOLEAN TryLockMutexObject(PKMUTEX pMutex)
{
	LARGE_INTEGER WaitTimeout;
	WaitTimeout.QuadPart = 0;
	return KeWaitForMutexObject(pMutex, Executive, KernelMode,
								FALSE, &WaitTimeout) != STATUS_TIMEOUT;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CreateSystemThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS CreateSystemThread(
	PKTHREAD* ppKThread,
	PKSTART_ROUTINE pStartRoutine,
	void* pData)
{	
	NTSTATUS Status;
	HANDLE hThread;
	OBJECT_ATTRIBUTES  ObjectAttrib;
	InitializeObjectAttributes(&ObjectAttrib, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
	Status = PsCreateSystemThread(&hThread, THREAD_ALL_ACCESS, &ObjectAttrib, NULL, NULL,
								pStartRoutine, pData);
	if (!NT_SUCCESS(Status)) {
		return Status;
	}

	ObReferenceObjectByHandle(hThread, THREAD_ALL_ACCESS, NULL, KernelMode,
							  (PVOID*) ppKThread, NULL);

	// We will not use the handle. We have now an address to the thread
	ZwClose(hThread);
	return STATUS_SUCCESS;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- AllocateCdmaBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS AllocateCdmaBuffer(
	Channel* pCh,
	UInt BufSize)
{
	// Allocate DMA buffer
	Int PortIndex = pCh->m_PortIndex;
	ShBufDesc* pCdmaBuf = &pCh->m_CdmaShBuf;
	pCdmaBuf->m_pBuf = ExAllocatePoolWithTag(NonPagedPool, BufSize, '1ATD');

	if (pCdmaBuf->m_pBuf == NULL) {
		// Error allocating memory
#if LOG_LEVEL_CDMA > 0
		DTA1XX_LOG( KERN_INFO, "[NwXx%d] AllocateCdmaBuffer: Allocating CDMA buffer "
				    "FAILS", PortIndex );
#endif

		return STATUS_INSUFFICIENT_RESOURCES;
	}

	pCdmaBuf->m_pMdl = IoAllocateMdl(pCdmaBuf->m_pBuf, BufSize, FALSE, TRUE, NULL);

	if (pCdmaBuf->m_pMdl == NULL) {
		// Error allocating mdl 
#if LOG_LEVEL_CDMA > 0
		DTA1XX_LOG( KERN_INFO, "[NwXx%d] AllocateCdmaBuffer: Allocating mdl CDMA buffer "
					"FAILS", PortIndex);
#endif
		ExFreePool(pCdmaBuf->m_pBuf);
		return STATUS_INSUFFICIENT_RESOURCES;
	}

	MmBuildMdlForNonPagedPool(pCdmaBuf->m_pMdl);
}

void FreeCdmaBuffer(Channel* pCh)
{
	ShBufDesc* pCdmaBuf = &pCh->m_CdmaShBuf;
	IoFreeMdl(pCdmaBuf->m_pMdl);
	ExFreePool(pCdmaBuf->m_pBuf);
	pCdmaBuf->m_pMdl = NULL;
	pCdmaBuf->m_pBuf = NULL;
}
#else

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TryLockMutexObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#ifdef NO_MUTEX_SUPPORT 

__inline BOOLEAN TryLockMutexObject(PKMUTEX pMutex)
{
	return Dta1xxMutexTryLock1(pMutex) == 0;
}

#else

__inline BOOLEAN TryLockMutexObject(PKMUTEX pMutex)
{	
	return Dta1xxMutexTryLock2(pMutex) == 1;
}

#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- InsertQueueDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
__inline BOOLEAN InsertQueueDpc(PKDPC pDpc)
{
	tasklet_schedule(pDpc);
	return TRUE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IoQueueWorkItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
__inline VOID IoQueueWorkItem(
	IN PIO_WORKITEM pIoWorkItem,
	IN PIO_WORKITEM_ROUTINE pWorkerRoutine,
	IN WORK_QUEUE_TYPE QueueType,
	IN PVOID pContext)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
	// INIT_WORK has been redefined in kernel version 2.6.20 (no 3rd context parameter)
	Dta1xxInitWork(pIoWorkItem, pWorkerRoutine);
#else
	Dta1xxInitWork(pIoWorkItem, pWorkerRoutine, pContext);
#endif
	Dta1xxScheduleWork(pIoWorkItem);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Atomic -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//

#ifdef NO_ATOMIC_RETURN
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- atomic_add_return -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
int Dta1xxAtomicAddReturn(int i, atomic_t *v)
{
	atomic_add(i,v);
	return atomic_read(v);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- atomic_inc_return -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
int Dta1xxAtomicIncReturn(atomic_t *v)
{
	atomic_inc(v);
	return atomic_read(v);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- atomic_dec_return -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
int Dta1xxAtomicDecReturn(atomic_t *v)
{
	atomic_dec(v);
	return atomic_read(v);
}
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- InterlockedExchangeAdd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
__inline int InterlockedExchangeAdd(PATOMIC Addend,  int Value)
{
	int NewValue = Dta1xxAtomicAddReturn(Value, Addend);
	return NewValue - Value;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Linked lists -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
__inline PLIST_ENTRY RemoveHeadList(IN PLIST_ENTRY  pListHead)
{
	PLIST_ENTRY pListEntry = pListHead->next;
	list_del(pListEntry);
	return pListEntry;
}

__inline unsigned long PtrToUlong(const void  *p)
{
    return((unsigned long) (ULONG_PTR) p );
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CreateSystemThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS CreateSystemThread(
	PKTHREAD* ppKThread,
	PKSTART_ROUTINE pStartRoutine,
	void* pData)
{
	*ppKThread = Dta1xxKThreadRun(pStartRoutine, pData, "DTA1XX");
	if (IS_ERR(*ppKThread)) {
		*ppKThread = NULL;
		return STATUS_INSUFFICIENT_RESOURCES;		
	}
	return STATUS_SUCCESS;
}

#ifdef NO_KTHREAD_SUPPORT

struct dta1xx_kthread_create_info
{
	// Information passed to dta1xx_kthread() from keventd
	int (*threadfn)(void *data);
	void *data;
	struct completion started;

	// Result passed back to dta1xx_kthread_create() from keventd
	struct task_struct *result;
	struct completion done;
};

struct dta1xx_kthread_stop_info
{
	struct task_struct *k;
	int err;
	struct completion done;
	volatile int wakeupdone;
};
static DECLARE_MUTEX(g_dta1xx_kthread_stop_lock);
static struct dta1xx_kthread_stop_info g_dta1xx_kthread_stop_info;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxKThreadShouldStop -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
int Dta1xxKThreadShouldStop(void)
{	int Ret = (g_dta1xx_kthread_stop_info.k == current);
	if (Ret != 0) {	
		// Wait until wakeup schedule is done
		while(g_dta1xx_kthread_stop_info.wakeupdone == 0) 
			__udelay(1000);  // 1 ms
	}
	return Ret;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- dta1xx_kthread_exit_files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void dta1xx_kthread_exit_files(void)
{
	struct fs_struct *fs;
	struct task_struct *tsk = current;

	exit_fs(tsk);		// current->fs->count--;
	fs = init_task.fs;
	tsk->fs = fs;
	atomic_inc(&fs->count);
 	exit_files(tsk);
	current->files = init_task.files;
	atomic_inc(&tsk->files->count);
}

#ifdef CONFIG_SMP
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,1)
	#define CPU_MASK_ALL cpu_online_map	
#endif
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- dta1xx_kthread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static int dta1xx_kthread(void *_create)
{
	struct dta1xx_kthread_create_info *create = _create;
	int (*threadfn)(void *data);
	void *data;
	//sigset_t blocked;
	int ret = -EINTR;

	dta1xx_kthread_exit_files();

	// Copy data: it's on keventd's stack
	threadfn = create->threadfn;
	data = create->data;

	// Flush all signals (in case we're not from keventd)
	flush_signals(current);

	// By default we can run anywhere, unlike keventd
	set_cpus_allowed(current, CPU_MASK_ALL);

	// OK, tell user we're spawned, wait for stop or wakeup
	__set_current_state(TASK_INTERRUPTIBLE);
	complete(&create->started);
	schedule();

	if (!Dta1xxKThreadShouldStop())
		ret = threadfn(data);

	// It might have exited on its own, w/o kthread_stop
	if (Dta1xxKThreadShouldStop()) {
		g_dta1xx_kthread_stop_info.err = ret;
		complete(&g_dta1xx_kthread_stop_info.done);
	}
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- dta1xx_keventd_create_kthread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// We are keventd: create a thread
//
static void dta1xx_keventd_create_kthread(void *_create)
{
	struct dta1xx_kthread_create_info *create = _create;
	int pid;

	// We want our own signal handler (we take no signals by default)
	pid = kernel_thread(dta1xx_kthread, create, CLONE_FS | CLONE_FILES | SIGCHLD);
	if (pid < 0) {
		create->result = ERR_PTR(pid);
	} else {
		wait_for_completion(&create->started);
#ifdef CONFIG_SMP
		read_lock(&tasklist_lock);
#endif
		create->result = find_task_by_pid(pid);
#ifdef CONFIG_SMP
		read_unlock(&tasklist_lock);
#endif
	}
	complete(&create->done);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- *Dta1xxKThreadCreate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
struct task_struct *Dta1xxKThreadCreate(int (*threadfn)(void *data),
				   void *data,
				   const char namefmt[],
				   ...)
{
	struct dta1xx_kthread_create_info create;
	Dta1xxDeclareWork(work, dta1xx_keventd_create_kthread, &create);

	create.threadfn = threadfn;
	create.data = data;
	init_completion(&create.started);
	init_completion(&create.done);

	
	Dta1xxScheduleWork(&work);
	wait_for_completion(&create.done);
	
	if (!IS_ERR(create.result)) {
		va_list args;
		va_start(args, namefmt);
		vsnprintf(create.result->comm, sizeof(create.result->comm),
			  namefmt, args);
		va_end(args);
	}

	return create.result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxKThreadStop -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
int Dta1xxKThreadStop(struct task_struct *k)
{
	int ret;

	down(&g_dta1xx_kthread_stop_lock);

	// Must init completion *before* thread sees g_dta1xx_kthread_stop_info.k
	init_completion(&g_dta1xx_kthread_stop_info.done);
	// Init synchronise wakeup
	g_dta1xx_kthread_stop_info.wakeupdone = 0;
	wmb();

	// Now set Dta1xxKThreadShouldStop() to true, and wake it up
	g_dta1xx_kthread_stop_info.k = k;
	wake_up_process(k);
	// Signal wakeup done
	g_dta1xx_kthread_stop_info.wakeupdone = 1;
	
	// Once it dies, reset stop ptr, gather result and we're done
	wait_for_completion(&g_dta1xx_kthread_stop_info.done);
	g_dta1xx_kthread_stop_info.k = NULL;
	ret = g_dta1xx_kthread_stop_info.err;
	up(&g_dta1xx_kthread_stop_lock);
	return ret;
}

#endif

#ifndef DTA1XXNWDRIVER
#include "../Include/Dta1xx.h"

// Older kernels do not support the GFP_DMA32 flag, so define it with a 'safe' value
#ifndef GFP_DMA32
#define GFP_DMA32	(0)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- AllocateCdmaBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Remark: BufferSize will be alligned to a PAGE_SIZE boundary!
//
NTSTATUS AllocateCdmaBuffer(Channel* pCh, UInt* BufferSize)
{
	// Allocate DMA buffer
	ShBufDesc* pCdmaBuf = &pCh->m_CdmaShBuf;
	Int TotalNumPages;
	Int i,j;
#if LOG_LEVEL > 0
	Int PortIndex = pCh->m_PortIndex;
#endif
	UInt BufSize = *BufferSize;

	// Round up to PAGE_SIZE
	if (BufSize % PAGE_SIZE)
		BufSize = ((BufSize / PAGE_SIZE) + 1) * PAGE_SIZE;

	*BufferSize = BufSize;
	
	TotalNumPages = BufSize / PAGE_SIZE;

	// Allocate memory for storing the page list
   	pCdmaBuf->m_PageList = kmalloc( sizeof(struct page*) * TotalNumPages, GFP_KERNEL );
	if ( pCdmaBuf->m_PageList == NULL )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%s%i] AllocateCdmaBuffer: Failed to allocate page-list",
				    Dta1xxChType2Str(pCh->m_ChannelType), PortIndex );
#endif
		return -EFAULT;
	}

	// Get free pages
	pCdmaBuf->m_NumPages = TotalNumPages;
	for (i = 0; i < TotalNumPages; i++)
	{
		pCdmaBuf->m_PageList[i] = virt_to_page(__get_free_page(GFP_KERNEL | GFP_DMA32));

#if LOG_LEVEL > 2
		DTA1XX_LOG( KERN_INFO, "[%s%i] AllocateCdmaBuffer: Page %i Address: %p", 
					Dta1xxChType2Str(pCh->m_ChannelType),
					PortIndex, i,  pCdmaBuf->m_PageList[i]);
#endif

		if (pCdmaBuf->m_PageList[i] == NULL) {
			// Out of memory
			// Clean up
			for (j = 0; j < i; j++) {
				free_page((unsigned long)page_address(pCdmaBuf->m_PageList[j]));
			}
			kfree(pCdmaBuf->m_PageList);
			pCdmaBuf->m_PageList = NULL;

#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%s%i] AllocateCdmaBuffer: Failed to allocate pages",
						Dta1xxChType2Str(pCh->m_ChannelType), PortIndex );
#endif
			return -EFAULT;		
		}
	}

	// Map pages to virtual kernel memory
	pCh->m_Dma.m_DmaBuffer.m_pVirtStartAddr = vmap(pCdmaBuf->m_PageList,
												   pCdmaBuf->m_NumPages,
												   VM_READ | VM_WRITE,
												   PAGE_KERNEL);

	// Allocation succeeded?
	if ( pCh->m_Dma.m_DmaBuffer.m_pVirtStartAddr==NULL )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "AllocateCdmaBuffer: failed to get virtual buffer" );
#endif

		// Release pages
		for (i = 0; i < pCdmaBuf->m_NumPages; i++) {
			free_page((unsigned long)page_address(pCdmaBuf->m_PageList[i]));
		}
			
		kfree(pCdmaBuf->m_PageList);
		pCdmaBuf->m_PageList = NULL;
		return -EFAULT;
	}
	pCh->m_CdmaBufSize = BufSize;
	//pCdmaBuf->m_SglBufSize = BufSize;
	pCdmaBuf->m_pBuf = pCh->m_Dma.m_DmaBuffer.m_pVirtStartAddr;
	pCdmaBuf->m_Allocated = TRUE;

	return STATUS_SUCCESS;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FreeCdmaBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void FreeCdmaBuffer(Channel* pCh)
{	ShBufDesc* pCdmaBuf = &pCh->m_CdmaShBuf;
	Int i;

	vunmap(pCh->m_Dma.m_DmaBuffer.m_pVirtStartAddr);
	pCh->m_Dma.m_DmaBuffer.m_pVirtStartAddr = NULL;

	// Release pages
	for (i = 0; i < pCdmaBuf->m_NumPages; i++) {
		free_page((unsigned long)page_address(pCdmaBuf->m_PageList[i]));
	}
		
	kfree(pCdmaBuf->m_PageList);
	pCdmaBuf->m_PageList = NULL;
	pCdmaBuf->m_pBuf = NULL;
}
#endif	//DTA1XXNWDRIVER

#endif
