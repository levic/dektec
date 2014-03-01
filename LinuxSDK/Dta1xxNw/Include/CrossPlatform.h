//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* CrossPlatform.h *#*#*#*#*#*#*#*#*#*# (C) 2007 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dta1xx driver, please refer to the Dta1xx readme
// Be sure to only define crossplatform inline functions in this file


// TODO:
// Check Windows/Linux types and possibly use types from DriverTypes.h
// Rename some windows specific names into more general names
//
#ifndef __CROSSPLATFORM_H
#define __CROSSPLATFORM_H

#ifdef DDK_TARGET_OS
	#define WINBUILD
#else
	#define LINBUILD
#endif

#ifdef WINBUILD
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Windows specific +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Packet types
#define NW_PACKET_TYPE_PROMISCUOUS	NDIS_PACKET_TYPE_PROMISCUOUS
#define NW_PACKET_TYPE_ALL_MULTICAST	NDIS_PACKET_TYPE_ALL_MULTICAST
#define NW_PACKET_TYPE_BROADCAST	NDIS_PACKET_TYPE_BROADCAST
#define NW_PACKET_TYPE_MULTICAST	NDIS_PACKET_TYPE_MULTICAST

// Link status
#define NW_STATUS_MEDIA_DISCONNECT	NDIS_STATUS_MEDIA_DISCONNECT
#define NW_STATUS_MEDIA_CONNECT		NDIS_STATUS_MEDIA_CONNECT

// Types
#define PFILE_OBJECT				PFILEOBJECT

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DPC's -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Function defines
#define InitializeDpc				KeInitializeDpc
#define InsertQueueDpc(pDpc)		KeInsertQueueDpc(pDpc, NULL, NULL)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Mutexes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Function defines
#define ReleaseMutex(pMutex)		KeReleaseMutex(pMutex, FALSE)
#define InitializeMutex(pMutex)		KeInitializeMutex(pMutex, 0)
#define WaitForMutexObject(pMutex)	KeWaitForMutexObject(pMutex, Executive, KernelMode, \
														 FALSE, NULL)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Atomic operations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Type defines
typedef LONG						ATOMIC, *PATOMIC;

// Function defines
#define atomic_read(a)				*(a)
#define atomic_set(Addend, Value)	(*Addend = Value)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Threads -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Function defines
NTSTATUS CreateSystemThread(PKTHREAD* ppKThread, PKSTART_ROUTINE pStartRoutine,
							void* pData);


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Lists -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Function defines
#define ListGetNext(Head)			(head->Flink)
#define ListGetPrev(Head)			(head->Blink)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Other -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Function defines
#define cpu_to_be16					RtlUshortByteSwap
#define be16_to_cpu					RtlUshortByteSwap

#else

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Linux specific +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Include files
#include <asm/uaccess.h>
#include <linux/version.h>
#include <linux/interrupt.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
#include <linux/semaphore.h>		// semaphore
#else
#include <asm/semaphore.h>			// semaphore
#endif
#include <linux/delay.h>			// udelay
#include "DriverTypes.h"

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,10)
	#define NO_ATOMIC_RETURN
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,16)
	#define NO_MUTEX_SUPPORT
#else
	#include <linux/mutex.h>
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,4)
	#define NO_KTHREAD_SUPPORT
#else
	#include <linux/kthread.h>
#endif
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,41)
	#define NO_WORKQUEUE_SUPPORT
	#include <linux/tqueue.h>
#else
	#include <linux/workqueue.h>
#endif

// Packet types
#define NW_PACKET_TYPE_PROMISCUOUS		0x01
#define NW_PACKET_TYPE_ALL_MULTICAST	0x02
#define NW_PACKET_TYPE_BROADCAST		0x04
#define NW_PACKET_TYPE_MULTICAST		0x08
#define NW_PACKET_TYPE_DIRECTED			0x10

// Link status
#define NW_STATUS_MEDIA_DISCONNECT		0
#define NW_STATUS_MEDIA_CONNECT			1

// OID's
#define OID_802_3_PERMANENT_ADDRESS		1
#define OID_802_3_CURRENT_ADDRESS		2
#define OID_802_3_MAXIMUM_LIST_SIZE		3
#define OID_802_3_MULTICAST_LIST		4
#define OID_GEN_LINK_SPEED				5
#define OID_GEN_XMIT_OK					6
#define OID_GEN_RCV_OK					7
#define OID_GEN_XMIT_ERROR				8
#define OID_GEN_RCV_ERROR				9
#define OID_GEN_RCV_CRC_ERROR			10
#define OID_802_3_RCV_ERROR_ALIGNMENT	11
#define OID_802_3_XMIT_ONE_COLLISION	12
#define OID_802_3_XMIT_MORE_COLLISIONS	13
#define OID_802_3_XMIT_DEFERRED			14
#define OID_802_3_XMIT_MAX_COLLISIONS	15
#define OID_802_3_XMIT_UNDERRUN			16
#define OID_802_3_XMIT_TIMES_CRS_LOST	17
#define OID_802_3_XMIT_LATE_COLLISIONS	18
#define OID_GEN_RCV_NO_BUFFER			19
#define OID_GEN_CURRENT_PACKET_FILTER	20
#define OID_GEN_BYTES_RCV				21
#define OID_GEN_BYTES_XMIT				22

// Types
typedef UInt32							ULONG;
typedef Int32							LONG;
typedef LONG							NTSTATUS;
typedef	Int64							LONG64;
typedef UInt64							ULONG64;
typedef void							VOID;
typedef void*							PVOID;
typedef unsigned long					ULONG_PTR, *PULONG_PTR;
typedef struct file*					PFILEOBJECT;
typedef size_t							SIZE_T;
typedef UInt16							USHORT;
typedef Int								NDIS_STATUS;
typedef Int								KIRQL;

// General
#define IN
#define OUT
#define INOUT
#define ASSERT(ignore)((void) 0)
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

// Error codes
#define STATUS_SUCCESS					0
#define STATUS_NOT_IMPLEMENTED			-EFAULT
#define	STATUS_UNSUCCESSFUL				-EFAULT
#define STATUS_PENDING					-EBUSY
#define STATUS_INSUFFICIENT_RESOURCES	-ENOMEM
#define STATUS_INVALID_PARAMETER		-EINVAL
#define NDIS_STATUS_INVALID_LENGTH		-EINVAL
#define NDIS_STATUS_MULTICAST_FULL		-E2BIG
#define STATUS_NOT_SUPPORTED			-EFAULT			// -ENOTSUP
#define STATUS_INVALID_DEVICE_STATE		-EFAULT

//****************************************************************************************
//*** The following must be removed after full linux integrations

typedef void*							PDEVICE_OBJECT;

typedef struct _IO_STATUS_BLOCK 
{
    union
	{
        NTSTATUS Status;
        PVOID Pointer;
    };
    ULONG_PTR Information;

} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;

typedef struct _IRP
{
    union
	{
        PVOID SystemBuffer;
    } AssociatedIrp;
    IO_STATUS_BLOCK IoStatus;
    PVOID UserBuffer;

} IRP, *PIRP;

//*** End remove block
//****************************************************************************************

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Events -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Type defines
typedef wait_queue_head_t				KEVENT, *PRKEVENT;

// Function defines
#define KeSetEvent(pEvent, pIncrement, Wait)	wake_up_interruptible(pEvent)
#define KeInitializeEvent(pEvent, Type, State)	init_waitqueue_head(pEvent)



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Threads -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Type defines
typedef struct task_struct				*PKTHREAD;
typedef int (*PKSTART_ROUTINE)			(IN PVOID pStartContext);

// Function defines
NTSTATUS CreateSystemThread(PKTHREAD* ppKThread, PKSTART_ROUTINE pStartRoutine,
							void* pData);
#define PsTerminateSystemThread(ignore)	((void) 0)	//	do_exit

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- KThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Function defines
#ifdef NO_KTHREAD_SUPPORT
	// No thread helper functions available

	struct task_struct *Dta1xxKThreadCreate(int (*threadfn)(void *data),
									   void *data, const char namefmt[], ...);
	#define Dta1xxKThreadRun(threadfn, data, namefmt, ...)			   \
	({									   \
		struct task_struct *__k						   \
			= Dta1xxKThreadCreate(threadfn, data, namefmt, ## __VA_ARGS__); \
		if (!IS_ERR(__k)) wake_up_process(__k);					   \
		__k;								   \
	})
	int Dta1xxKThreadShouldStop(void);
	int Dta1xxKThreadStop(struct task_struct *k);
	
#else
	#define Dta1xxKThreadCreate			kthread_create
	#define Dta1xxKThreadRun			kthread_run
	#define Dta1xxKThreadShouldStop		kthread_should_stop
	#define Dta1xxKThreadStop			kthread_stop
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Linked lists -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Type defines
typedef struct list_head				LIST_ENTRY, *PLIST_ENTRY;

// Function defines
#define InitializeListHead				INIT_LIST_HEAD
#define InsertTailList(Head, Entry)		list_add_tail(Entry, Head)
#define IsListEmpty						list_empty
#define ListGetNext(Head)				((Head)->next)
#define ListGetPrev(Head)				((Head)->prev)
#define RemoveEntryList					list_del
PLIST_ENTRY RemoveHeadList(IN PLIST_ENTRY  pListHead);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Mutexes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

#ifdef NO_MUTEX_SUPPORT 
	#define dta1xx_mutexstruct			semaphore
	#define Dta1xxMutexLock				down
	#define Dta1xxMutexUnlock			up
	#define Dta1xxMutexInit				init_MUTEX
	#define Dta1xxMutexTryLock1			down_trylock
#else
	#define dta1xx_mutexstruct			mutex
	#define Dta1xxMutexLock				mutex_lock
	#define Dta1xxMutexUnlock			mutex_unlock
	#define Dta1xxMutexInit				mutex_init
	#define Dta1xxMutexTryLock2			mutex_trylock
#endif

// Type defines
typedef	struct dta1xx_mutexstruct		KMUTEX, *PKMUTEX;
typedef struct dta1xx_mutexstruct		FAST_MUTEX, *PFAST_MUTEX;

// Function defines
#define ExAcquireFastMutex				Dta1xxMutexLock
#define ExReleaseFastMutex				Dta1xxMutexUnlock
#define ExInitializeFastMutex			Dta1xxMutexInit
#define InitializeMutex					Dta1xxMutexInit
#define ReleaseMutex					Dta1xxMutexUnlock
#define WaitForMutexObject				Dta1xxMutexLock			// IRQL < DPC

static __inline KIRQL KeGetCurrentIrql(void) { return 0; }


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Spinlocks -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Type defines
typedef spinlock_t						KSPIN_LOCK, *PKSPIN_LOCK;

// Function defines
#define KeInitializeSpinLock			spin_lock_init
#define KeAcquireSpinLockAtDpcLevel		spin_lock_bh

static __inline void KeAcquireSpinLock(KSPIN_LOCK* pSpinLock, KIRQL* pOldIrql)
{ 
	spin_lock_bh(pSpinLock);
}

static __inline void KeReleaseSpinLock(KSPIN_LOCK* pSpinLock, KIRQL OldIrql)
{ 
	spin_unlock_bh(pSpinLock);
}

#define KeReleaseSpinLockFromDpcLevel	spin_unlock_bh


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Work items -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Type defines
#ifdef NO_WORKQUEUE_SUPPORT
	typedef struct tq_struct			IO_WORKITEM, *PIO_WORKITEM;
	#define Dta1xxScheduleWork			schedule_task
	#define Dta1xxInitWork				INIT_TQUEUE
	#define Dta1xxDeclareWork(n, f, d)					\
		IO_WORKITEM n; INIT_TQUEUE((&n), f, d);
	
#else
	typedef struct work_struct			IO_WORKITEM, *PIO_WORKITEM;
	#define Dta1xxDeclareWork			DECLARE_WORK
	#define Dta1xxInitWork				INIT_WORK	
	#define Dta1xxScheduleWork			schedule_work
	
#endif

typedef enum _WORK_QUEUE_TYPE
{
    DelayedWorkQueue = 1
} WORK_QUEUE_TYPE;

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
	typedef VOID (*PIO_WORKITEM_ROUTINE) (IN PIO_WORKITEM pWork);
#else
	typedef VOID (*PIO_WORKITEM_ROUTINE) (IN PVOID pContext);
#endif

// Function defines
#define IoAllocateWorkItem(ignore)		kmalloc(sizeof(IOWORKITEM), GFP_ATOMIC)
VOID IoQueueWorkItem(IN PIO_WORKITEM pIoWorkItem, IN PIO_WORKITEM_ROUTINE pWorkerRoutine,
		     IN WORK_QUEUE_TYPE QueueType, IN PVOID pContext);
#define IoFreeWorkItem					kfree

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DPC's -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Type defines
typedef struct tasklet_struct			KDPC, *PKDPC;
typedef void (*PKDEFERRED_ROUTINE) (unsigned long);

// Function defines
BOOLEAN InsertQueueDpc(PKDPC pDpc);
#define InitializeDpc(pDpc, pFunc, pCont)	tasklet_init(pDpc, pFunc, (unsigned long)pCont)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Memory allocation -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Type defines
typedef enum _POOL_TYPE 
{
  NonPagedPool = 0,
  //PagedPool = 1,
  //NonPagedPoolMustSucceed = 2,
  //DontUseThisType = 3,
  //NonPagedPoolCacheAligned = 4,
  //PagedPoolCacheAligned = 5,
  //NonPagedPoolCacheAlignedMustS = 6
} POOL_TYPE;

#define ExAllocatePoolWithTag(PoolType, NumberOfBytes, Tag) kmalloc(NumberOfBytes, GFP_KERNEL)
#define ExAllocatePool(PoolType, NumberOfBytes) kmalloc(NumberOfBytes, GFP_KERNEL)
#define ExFreePool						kfree

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Atomic operations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Type defines
typedef atomic_t						ATOMIC,*PATOMIC;

#ifdef NO_ATOMIC_RETURN
int Dta1xxAtomicAddReturn(int i, atomic_t *v);
int Dta1xxAtomicIncReturn(atomic_t *v);
int Dta1xxAtomicDecReturn(atomic_t *v);
#else
#define Dta1xxAtomicAddReturn				atomic_add_return
#define Dta1xxAtomicIncReturn				atomic_inc_return
#define Dta1xxAtomicDecReturn				atomic_dec_return
#endif

// Function defines
#define InterlockedIncrement				Dta1xxAtomicIncReturn
#define InterlockedDecrement				Dta1xxAtomicDecReturn
int InterlockedExchangeAdd(PATOMIC Addend, int Value);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Other -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Function defines
#define KeStallExecutionProcessor			__udelay
#define RtlZeroMemory(pDest,Length)			memset(pDest, 0, Length)
#define RtlCopyMemory						memcpy
#define CONTAINING_RECORD					container_of
#define RtlUshortByteSwap					swab16
unsigned long PtrToUlong(const void* p);

#endif

BOOLEAN TryLockMutexObject(PKMUTEX pMutex);					// IRQL <=DPC

#endif //__CROSSPLATFORM_H
