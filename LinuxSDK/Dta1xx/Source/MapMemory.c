//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* MapMemory.c *#*#*#*#*#*#*#*#* (C) 2006-2008 DekTec

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dta1xx driver, please refer to the Dta1xx readme

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "../Include/Dta1xx.h"
#include "../Include/Dta1xxRegs.h"
#include <linux/version.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,13)
#define io_remap_pfn_range remap_pfn_range
#endif

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,10)
#define remap_pfn_range(vma,from,to,size,prot) remap_page_range(vma,from,(to<<PAGE_SHIFT),size,prot)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxReleaseAddrOperRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Called by DeviceControl to release a user-space pointer to a register block.
//
Int  Dta1xxReleaseAddrOperRegs(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	struct file* filp,		// Current file object
	Int ChannelType,		// DTA1XX_TS_TX_CHANNEL / DTA1XX_TS_RX_CHANNEL
							// Special value: DTA1XX_GEN_REGS
	Int PortIndex,			// Zero-based port index
	void** ppPaMmap,		// User-space pointer from Mmap
	UInt* pSize)			// Size of memory block returned
{
	Int  i, Status;

	if ((ChannelType != DTA1XX_TS_TX_CHANNEL) && 
	    (ChannelType != DTA1XX_TS_RX_CHANNEL) &&
		(ChannelType != DTA1XX_GEN_REGS)) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxReleaseAddrOperRegs: ChannelType=%d ILLEGAL",
				     PortIndex, ChannelType );
#endif
		return -EFAULT;
	}

	// Base address should already be mapped by ussing mmap by user process
	// Search for the mapping
	// Acquire memory map mutex
	Status = down_interruptible( &pFdo->m_UserMapMutex );
	if ( Status!=0 && Status!=-EINTR )
		return -EFAULT;
	
	for (i=0; i<MAX_NUM_USER_MAPPINGS; i++) {
		if (pFdo->m_UserMapping[i].m_FileObject == filp) {
			break;
		}
	}
	if (i == MAX_NUM_USER_MAPPINGS) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxReleaseAddrOperRegs: No mapping found for this fileobject.");
#endif
		up( &pFdo->m_UserMapMutex );
		return -EFAULT;
	}

	if (pFdo->m_UserMapping[i].m_RefCount == 0) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxReleaseAddrOperRegs: Reference count already 0!",
				     PortIndex);
#endif
		up( &pFdo->m_UserMapMutex );
		return -EFAULT;
	}

	pFdo->m_UserMapping[i].m_RefCount--;
	if (pFdo->m_UserMapping[i].m_RefCount != 0) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxReleaseAddrOperRegs: Reference count = %u. [EBUSY]",
				     PortIndex, pFdo->m_UserMapping[i].m_RefCount);
#endif
		up( &pFdo->m_UserMapMutex );
		return -EBUSY;
	}
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxReleaseAddrOperRegs: Reference count = 0. Release",
				    PortIndex );
#endif
	*ppPaMmap = pFdo->m_UserMapping[i].m_pPaMmap;
	*pSize = pFdo->m_Dta1xxRegsBase.m_Length;
	pFdo->m_UserMapping[i].m_pUserVirtual = NULL;
	pFdo->m_UserMapping[i].m_FileObject = NULL;
	pFdo->m_UserMapping[i].m_EntryUsed = FALSE;
	pFdo->m_UserMapping[i].m_pPaMmap = NULL;
	up( &pFdo->m_UserMapMutex );
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxGetAddrOperRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Called by DeviceControl to get a user-space pointer to a register block.
//
Int  Dta1xxGetAddrOperRegs(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	struct file* filp,		// Current file object
	Int ChannelType,		// DTA1XX_TS_TX_CHANNEL / DTA1XX_TS_RX_CHANNEL
							// Special value: DTA1XX_GEN_REGS
	Int PortIndex,			// Zero-based port index
	void** ppUserVirtual,	// User-space pointer to register block
	UInt* pSize,			// Size of register map.
	void* pPaMmap)			// User-space pointer returned by mmap or NULL.
	
{
	UInt8*  pUserVirtual;
	Int  i;

	*ppUserVirtual = NULL;
	*pSize = 0;

	// Do not crash on erroneous channel index
	if (ChannelType == DTA1XX_TS_TX_CHANNEL) {
		if (PortIndex<0 || PortIndex>=pFdo->m_NumNonIpChannels) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[?] Dta1xxGetAddrOperRegs: Tx PortIndex=%d INVALID",
						 PortIndex );
#endif
			return -EFAULT;
		}
	}
	else if (ChannelType == DTA1XX_TS_RX_CHANNEL) {
		if (PortIndex<0 || PortIndex>=pFdo->m_NumNonIpChannels) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[?] Dta1xxGetAddrOperRegs: Rx PortIndex=%d INVALID",
					    PortIndex );
#endif
			return -EFAULT;
		}
	}
	else if (ChannelType != DTA1XX_GEN_REGS) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxGetAddrOperRegs: ChannelType=%d ILLEGAL",
				     PortIndex, ChannelType );
#endif
		return -EFAULT;
	}

	// Base address should already be mapped by ussing mmap by user process
	// Search for the mapping
	// Acquire memory map mutex
	if ( 0!=down_interruptible( &pFdo->m_UserMapMutex ) )
		return -EFAULT;
	
	for (i=0; i<MAX_NUM_USER_MAPPINGS; i++) {
		if (pFdo->m_UserMapping[i].m_FileObject == filp) {
			break;
		}
	}
	if (i == MAX_NUM_USER_MAPPINGS) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxMapOperRegsToUserSpace: No mapping found for this process." );
#endif
		// We will return the size of the registermap for mmap
		*pSize = pFdo->m_Dta1xxRegsBase.m_Length;

		// Release memory map mutex
		up( &pFdo->m_UserMapMutex );
		return -EAGAIN;
	}
	pUserVirtual = pFdo->m_UserMapping[i].m_pUserVirtual;

	if (pPaMmap != NULL)
		pFdo->m_UserMapping[i].m_pPaMmap = pPaMmap;

	// Increment reference count
	pFdo->m_UserMapping[i].m_RefCount++;

	// Release memory map mutex
	up( &pFdo->m_UserMapMutex );

	// Adapt address dependent on channel type
	if (ChannelType == DTA1XX_TS_TX_CHANNEL) {
		pUserVirtual += (UInt8*)pFdo->m_Channel[PortIndex].m_pRegBase -
						(UInt8*)pFdo->m_pGenRegs;
	}
	else if (ChannelType == DTA1XX_TS_RX_CHANNEL) {
		pUserVirtual += (UInt8*)pFdo->m_Channel[PortIndex].m_pRegBase -
						(UInt8*)pFdo->m_pGenRegs;
	}
	// else: no adaptation of user-space pointer required

	// Copy OUT parameter
	*ppUserVirtual = pUserVirtual;

#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxGetAddrOperRegs: ChannelType=%d pUserVirtual=0x%p",
				PortIndex, ChannelType, pUserVirtual);
#endif

	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxMapOperRegsToUserSpace -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Maps the BASE address of the operational registers to user space.
//
Int  Dta1xxMapOperRegsToUserSpace(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	struct file* filp,				// Current file object
	struct vm_area_struct* vma)
{
	Int  i, r;
	UInt Size, RegionStart;
	
	// Acquire memory map mutex
	if ( 0!=down_interruptible( &pFdo->m_UserMapMutex ) )
		return -EFAULT;
	
	// Check whether a mapping is available for this file object. If so, return
	for (i=0; i<MAX_NUM_USER_MAPPINGS; i++) {
		if (pFdo->m_UserMapping[i].m_FileObject == filp) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, 
				"Dta1xxMapOperRegsToUserSpace: Error BAR already mapped to user space" );
#endif
			// Release memory map mutex
			up( &pFdo->m_UserMapMutex );
			return -EFAULT;
		}
	}
	// No => find first free entry
	for (i=0; i<MAX_NUM_USER_MAPPINGS; i++)
		if (pFdo->m_UserMapping[i].m_EntryUsed == FALSE)
			break;
	if (i == MAX_NUM_USER_MAPPINGS) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxMapOperRegsToUserSpace: OUT OF USER MAPPINGS" );
#endif
		// Release memory map mutex
		up( &pFdo->m_UserMapMutex );
		return -EFAULT;
	}

	// Be sure the memory is mapped non-cacheable
	vma->vm_flags |= VM_IO;
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	RegionStart = pFdo->m_Dta1xxRegsBase.m_PciAddr;
	Size = vma->vm_end - vma->vm_start;
	
	if ((Size % PAGE_SIZE) != 0){ 
		Size = (Size / PAGE_SIZE) * PAGE_SIZE + PAGE_SIZE; 
	} 

	r = io_remap_pfn_range(
						vma,
						vma->vm_start,
						RegionStart >> PAGE_SHIFT,
						Size,
						vma->vm_page_prot);
	if (r!=0) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxMapOperRegsToUserSpace: Error mapping BAR to user space" );
#endif
		up( &pFdo->m_UserMapMutex );
		return -EFAULT;
	}
	// Store result in free element
	// Take the region startoffset into account
	pFdo->m_UserMapping[i].m_pUserVirtual = (void*)(vma->vm_start+(RegionStart%PAGE_SIZE));
	pFdo->m_UserMapping[i].m_FileObject = filp;
	pFdo->m_UserMapping[i].m_EntryUsed = TRUE;
	pFdo->m_UserMapping[i].m_RefCount = 0;
	pFdo->m_UserMapping[i].m_pPaMmap = NULL;


#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "Dta1xxMapOperRegsToUserSpace: pUserVirtual=0x%p",
			    pFdo->m_UserMapping[i].m_pUserVirtual);
#endif

	// Release memory map mutex
	up( &pFdo->m_UserMapMutex );
	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxUnmapOperRegsForFileObject -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dta1xxUnmapOperRegsForFileObject(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	struct file* filp)		// Current file object
{
	Int  i, Status, Found;
	
	// Acquire memory map mutex
	Status = down_interruptible( &pFdo->m_UserMapMutex );
	if ( Status!=0 && Status!=-EINTR )
		return -EFAULT;
	
	// Check whether a mapping has been issued for this file object
	Found = FALSE;
	for (i=0; i<MAX_NUM_USER_MAPPINGS; i++) {
		if (pFdo->m_UserMapping[i].m_FileObject == filp) {
			Found = TRUE;
			break;
		}
	}

	// No => return without further action
	if (Found == FALSE) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxUnmapOperRegsForFileObject: NO MAPPING FOUND");
#endif
		// Release memory map mutex
		up( &pFdo->m_UserMapMutex );
		return 0;
	}

	// Yes => unmap memory the virtual address stored in the mapping entry
#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "Dta1xxUnmapOperRegsForFileObject: Unmap pUserVirtual=0x%p",
			    pFdo->m_UserMapping[i].m_pUserVirtual);
#endif

	// Clear entry
	pFdo->m_UserMapping[i].m_pUserVirtual = NULL;
	pFdo->m_UserMapping[i].m_FileObject = NULL;
	pFdo->m_UserMapping[i].m_EntryUsed = FALSE;
	pFdo->m_UserMapping[i].m_RefCount = 0;
	pFdo->m_UserMapping[i].m_pPaMmap = NULL;
	
	// Release memory map mutex
	up( &pFdo->m_UserMapMutex );
	return 0;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ MEMORY MAPPING ROUTINES FOR IP RX/TX +=+=+=+=+=+=+=+=+=+=+=+=+

typedef struct __UNMAPWI
{
	IO_WORKITEM m_WorkItem;
	UInt8*		 m_pBuffer;
} UNMAPWI, *PUNMAPWI;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- UnmapWorkItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
static void UnmapWorkItem(PIO_WORKITEM pWork)
#else
static void UnmapWorkItem(PVOID pContext)
#endif
{	
	PUNMAPWI pUnmapWI;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,20)
	pUnmapWI = container_of(pWork, UNMAPWI, m_WorkItem);
#else
	pUnmapWI = (PUNMAPWI)pContext;
#endif
	vunmap(pUnmapWI->m_pBuffer);
	kfree(pUnmapWI);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpRxReleaseBuf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Release Rx buffer
//
NTSTATUS  Dta1xxNwIpRxReleaseBuf(
	IN PDTA1XX_FDO  pFdo,					// Device extension
	IN UserIpRxChannel*  pIpRx)				// IP Rx channel
{
	Int i;

	// Buffer shouldn't have been released already
	if (!pIpRx->m_SharedBuf.m_Allocated) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[IpRx%d] Dta1xxNwIpRxReleaseBuf: ERROR, BUFFER "
				    "HAS BEEN RELEASED ALREADY", pIpRx->m_ChannelIndex );
#endif
		return STATUS_UNSUCCESSFUL;
	}

	// Free the RTP buffer
	Dta1xxNwIpRxFreeRtpBuffers(pIpRx);

	// Unmap the buffer from virtual address space
	// Check if we are running in user process context
	if (in_interrupt()) {
		// We can not unmap the memory area at this moment. Do it in a work queue
		// Allocate memory for the work_struct and data pointer
		UNMAPWI* pUnmapWI;
		pUnmapWI = kmalloc(sizeof(UNMAPWI), GFP_ATOMIC);
		pUnmapWI->m_pBuffer = pIpRx->m_SharedBuf.m_pBuf;
		IoQueueWorkItem(&(pUnmapWI->m_WorkItem), UnmapWorkItem, DelayedWorkQueue, pUnmapWI);

#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[IpRx%d] Dta1xxNwIpRxReleaseBuf: Placed unmap on delayed"
					"work queue", pIpRx->m_ChannelIndex );
#endif
	} else
		vunmap(pIpRx->m_SharedBuf.m_pBuf);

	// Unlock pages
	for ( i=0; i<pIpRx->m_SharedBuf.m_NumPages; i++ )
          page_cache_release(pIpRx->m_SharedBuf.m_PageList[i]);

	// Free buffer for page list
	kfree(pIpRx->m_SharedBuf.m_PageList);

	pIpRx->m_SharedBuf.m_pBuf = NULL;
	pIpRx->m_SharedBuf.m_PageList = NULL;
	pIpRx->m_SharedBuf.m_NumPages = 0;
	pIpRx->m_SharedBuf.m_Allocated = FALSE;

#if LOG_LEVEL_IPRX > 0
	DTA1XX_LOG( KERN_INFO, "[IpRx%d] Dta1xxNwIpRxReleaseBuf: Buffer has been released",
			    pIpRx->m_ChannelIndex );
#endif

	return STATUS_SUCCESS;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNwIpTxReleaseBuf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Release a Tx buffer
//
NTSTATUS  Dta1xxNwIpTxReleaseBuf(
	IN PDTA1XX_FDO  pFdo,					// Device extension
	IN UserIpTxChannel*  pIpTx)				// IP Tx channel
{
	//KIRQL  OldIrqL;							// OLD IRQ level
	Int i;
	
	// Buffer shouldn't have been released already
	if (!pIpTx->m_SharedBuf.m_Allocated) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[IpTx%d] Dta1xxNwIpTxReleaseBuf: ERROR, BUFFER "
				    "HAS BEEN RELEASED ALREADY", pIpTx->m_ChannelIndex );
#endif
		return STATUS_UNSUCCESSFUL;
	}

	if (pIpTx->m_TxControl != DTA1XX_TXCTRL_IDLE) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[IpTx%d] Dta1xxNwIpTxReleaseBuf: "
				    "m_TxControl=%d not in IDLE state",
				    pIpTx->m_ChannelIndex, pIpTx->m_TxControl );
#endif
		return STATUS_UNSUCCESSFUL;
	}

	pIpTx->m_pCommon = NULL;

	// Unmap the buffer from virtual address space
	// Check if we are running in user process context
	if ( in_interrupt() )
	{
		// We can not unmap the memory area at this moment. Do it in a work queue
		// Allocate memory for the work_struct and data pointer
		UNMAPWI* pUnmapWI;
		pUnmapWI = kmalloc(sizeof(UNMAPWI), GFP_ATOMIC);
		pUnmapWI->m_pBuffer = pIpTx->m_SharedBuf.m_pBuf;
		IoQueueWorkItem(&(pUnmapWI->m_WorkItem), UnmapWorkItem, DelayedWorkQueue,
						pUnmapWI);

#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[IpTx%d] Dta1xxNwIpTxReleaseBuf: Placed unmap on delayed"
					"work queue", pIpTx->m_ChannelIndex );
#endif
	} else
		vunmap(pIpTx->m_SharedBuf.m_pBuf);
	
	// Unlock pages
	for ( i=0; i<pIpTx->m_SharedBuf.m_NumPages; i++ )
		page_cache_release(pIpTx->m_SharedBuf.m_PageList[i]);

	// Free buffer for page list
	kfree(pIpTx->m_SharedBuf.m_PageList);

	pIpTx->m_SharedBuf.m_pBuf = NULL;
	pIpTx->m_SharedBuf.m_PageList = NULL;
	pIpTx->m_SharedBuf.m_NumPages = 0;
	pIpTx->m_SharedBuf.m_Allocated = FALSE;
	

#if LOG_LEVEL_IPTX > 0
	DTA1XX_LOG( KERN_INFO, "[IpTx%d] Dta1xxNwIpTxReleaseBuf: buffer has been released",
			    pIpTx->m_ChannelIndex );
#endif
	return STATUS_SUCCESS;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ SHARED BUFFERS +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Allocation and freeing of (large) buffers that are shared between user-space
// applications and driver. Such a buffer is allocated in user space and then shared
// with the driver. This avoids quick exhaustion of the non-paged pool.

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSharedBufAlloc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dta1xxSharedBufAlloc(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	DTA1XX_SHAREDBUF_CTRL* pShBufCtrl)	// Shared buffer control
{
	Int rc, TotalNumPages=0, Purpose, ChannelType, PortIndex, ChannelIndex = 0, i;
	Channel* pCh=NULL;
	ShBufDesc* pShBufDesc=NULL;
	struct vm_area_struct** pVmArea = NULL;
	UserIpRxChannel*  pIpRx = NULL;
	UserIpTxChannel*  pIpTx = NULL;
	Int TypeNumber = pFdo->m_TypeNumber;
	BOOLEAN  Is11X = (   TypeNumber==110 || TypeNumber==111 || TypeNumber==112
					  || TypeNumber==115 || TypeNumber==116 || TypeNumber==117);
	Int  PrefixIndex = 0;			// Index used in debug prints
	char*  PrefixStr = "";			// Used in debug prints: "", "IpRx", "IpTx"

	// LATER: put accesses through pShBufCtrl in try/catch
	Purpose      = pShBufCtrl->m_Purpose;
	ChannelType  = pShBufCtrl->m_ChannelType;
	PortIndex    = pShBufCtrl->m_PortIndex;
	// With test for backward compatibility
	if (Purpose == DTA1XX_SHBUF_IP_RWBUF)
		ChannelIndex = pShBufCtrl->m_ChannelIndex;

	// Set DTA1XX_LOG related variables
	switch (Purpose) {
		case DTA1XX_SHBUF_DIRIQ_CDMA:
			PrefixStr = "";
			PrefixIndex = PortIndex;
			break;
		case DTA1XX_SHBUF_IP_RWBUF:
			PrefixStr = (ChannelType == DTA1XX_IP_RX_CHANNEL) ? "IpRx" : "IpTx";
			PrefixIndex = ChannelIndex;
			break;
	}

#if LOG_LEVEL > 1
		DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufAlloc: Allocating shared buffer",
					PrefixStr, PrefixIndex);
#endif


	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Pre-Allocation Checks -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	// Purpose-dependent code
	if (Purpose == DTA1XX_SHBUF_DIRIQ_CDMA) {

		// FOR NOW: we only support the DTA-110(T)/112/115/116/117
		if ( !Is11X )
		{
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufAlloc: ONLY SUPPORTED FOR "
						"DTA-110/111/112/115/116/117", PrefixStr, PrefixIndex );
#endif
			return -EFAULT;
		}
		// Check channel type and index
		if (   ChannelType!=DTA1XX_TS_TX_CHANNEL
			|| (PortIndex!=0 && TypeNumber==110)
			|| (PortIndex!=0 && TypeNumber==111)
			|| (PortIndex!=1 && TypeNumber==112)
			|| (PortIndex!=1 && TypeNumber==115)
			|| (PortIndex!=1 && TypeNumber==116)
			|| (PortIndex!=1 && TypeNumber==117) )
		{
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufAlloc: ChannelType=%d INVALID",
						PrefixStr, PrefixIndex, ChannelType );
#endif
			return -EFAULT;
		}

		// Get channel & shared buffer descriptor
		pCh = &pFdo->m_Channel[PortIndex];
		pShBufDesc = &(pCh->m_CdmaShBuf);

		// Sanity check: no CDMA may be running
		if ( pCh->m_CdmaDmaRunning )
		{
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufAlloc: CDMA STILL RUNNING?!?",
						PrefixStr, PrefixIndex );
#endif
			return -EFAULT;
		}
	} 
	else if (Purpose == DTA1XX_SHBUF_IP_RWBUF) {

		if (ChannelType == DTA1XX_IP_RX_CHANNEL) {

			if (ChannelIndex<0) {
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "[%s?] Dta1xxSharedBufAlloc: ChannelIndex=%d IS "
						    "OUT OF RANGE!!", PrefixStr, ChannelIndex);
#endif
				return STATUS_INVALID_PARAMETER;
			}
			// Get channel pointer
			pIpRx = Dta1xxNwIpRxGetChannel(pFdo, ChannelIndex);
			if (pIpRx == NULL) {
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufAlloc: ChannelIndex=%d IS "
						    "NOT AVAILABLE!!", PrefixStr, PrefixIndex, ChannelIndex);
#endif
				return STATUS_UNSUCCESSFUL;
			}
			pShBufDesc = &(pIpRx->m_SharedBuf);
		}
		else if (ChannelType == DTA1XX_IP_TX_CHANNEL) {

			if (ChannelIndex<0) {
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "[%s?] Dta1xxSharedBufAlloc: ChannelIndex=%d IS "
						    "OUT OF RANGE!!", PrefixStr, ChannelIndex);
#endif
				return STATUS_INVALID_PARAMETER;
			}
			// Get channel pointer
			pIpTx = Dta1xxNwIpTxGetChannel(pFdo, ChannelIndex);
			if (pIpTx == NULL) {
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufAlloc: ChannelIndex=%d IS "
						    "NOT AVAILABLE!!", PrefixStr, PrefixIndex, ChannelIndex);
#endif
				return STATUS_UNSUCCESSFUL;
			}
			pShBufDesc = &(pIpTx->m_SharedBuf);
		}
		else {

#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[?] Dta1xxSharedBufAlloc: ChannelType=%d NOT SUPPORTED",
					    ChannelType);
#endif
			return STATUS_UNSUCCESSFUL;
		}
	} else
		return STATUS_UNSUCCESSFUL;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Main Allocation Code -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufAlloc: ShBufDesc=0x%p",
			    PrefixStr, PrefixIndex, pShBufDesc);
#endif
		// Shared buffer may not have been allocated already
	if ( pShBufDesc->m_Allocated )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufAlloc: BUFFER ALREADY ALLOCATED",
				    PrefixStr, PrefixIndex );
#endif
		return -EFAULT;
	}

	// Init shared-buffer descriptor
	pShBufDesc->m_pBuf = NULL;
	pShBufDesc->m_PageList = NULL;
	pShBufDesc->m_NumPages = 0;
	pShBufDesc->m_SglAllocated = FALSE;
    pShBufDesc->m_pUserBuf = pShBufCtrl->m_pSharedBuf;
	if (pCh != NULL)
		pCh->m_CdmaBufSize = pShBufCtrl->m_SharedBufSize;

    // Must be page alligned
	if ( ((UIntPtr)pShBufCtrl->m_pSharedBuf % PAGE_SIZE)!=0 )
	{
#if LOG_LEVEL > 0
        DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufAlloc: Buffer %p is not starting at "
                    "a page alligned address. (page size: %d)", PrefixStr, PrefixIndex, 
					pShBufCtrl->m_pSharedBuf, (Int)(PAGE_SIZE) );
#endif
		return -EFAULT;
	}

	// Compute number of pages
    TotalNumPages = pShBufCtrl->m_SharedBufSize/PAGE_SIZE;
	// Check for partial page
	if ( (pShBufCtrl->m_SharedBufSize%PAGE_SIZE)!=0 )
		TotalNumPages++;

	// Lock user buffer, but first allocate memory for page list
	pShBufDesc->m_PageList = kmalloc( sizeof(struct page*) * TotalNumPages, GFP_KERNEL );
	if ( pShBufDesc->m_PageList == NULL )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufAlloc: Failed to allocate page-list",
				    PrefixStr, PrefixIndex );
#endif
		return -EFAULT;
	}

	// Allocate memory for temporarily storing the vm_area list
	pVmArea = kmalloc( sizeof(struct vm_area_struct*) * TotalNumPages, GFP_KERNEL );
	if ( pVmArea == NULL )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufAlloc: Failed to allocate vm_area-list",
				    PrefixStr, PrefixIndex );
#endif
		// Free already allocated resources
		kfree(pShBufDesc->m_PageList);
		pShBufDesc->m_PageList = NULL;
		return -EFAULT;
	}

	pShBufDesc->m_NumPages = TotalNumPages;

	// Get mmap reader/writer semaphore
	// We have to get the write semaphore because we are going to change the flags
	down_write(&current->mm->mmap_sem);

	// Lock pages
	rc = get_user_pages(
			current,        // Task performing I/O
			current->mm,    // The tasks memory-management structure
			(UIntPtr)(pShBufDesc->m_pUserBuf),   // Page-aligned starting address of user buffer
			TotalNumPages,  // Length of the buffer in pages
			1,              // Map for read access (i.e. user app performing a write)
			0,              // Do not force an override of page protections
			pShBufDesc->m_PageList, // Will contain list of page pointers describing buffer
			pVmArea);       // Will contain list of associated VMAs

	// Make sure the pages will be permanently locked into physical memory
	if (rc == TotalNumPages) {
		for (i = 0; i < TotalNumPages; i++)
			pVmArea[i]->vm_flags |= VM_LOCKED;
	}
	// Release mmap semaphore
	up_write(&current->mm->mmap_sem);

	kfree(pVmArea);
	pVmArea = NULL;

	if ( rc != TotalNumPages )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufAlloc: Failed to lock user buffer",
				    PrefixStr, PrefixIndex );
#endif
		kfree( pShBufDesc->m_PageList );
		pShBufDesc->m_PageList = NULL;
		pShBufDesc->m_NumPages = 0;
		pShBufDesc->m_Allocated = FALSE;
		pShBufDesc->m_SglAllocated = FALSE;

		return -EFAULT;
	}

	// Just to be safe, we flush the cache for the complete buffer
	for (i = 0; i < TotalNumPages; i++)
		flush_dcache_page(pShBufDesc->m_PageList[i]);

	pShBufDesc->m_Allocated = TRUE;

#if LOG_LEVEL > 1
	DTA1XX_LOG( KERN_INFO,
				"[%s%d] Dta1xxSharedBufAlloc: Locked user-buffer (num pages:%d)",
				PrefixStr, PrefixIndex, TotalNumPages );
#endif

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Post-Allocation Processing -.-.-.-.-.-.-.-.-.-.-.-.-.-.

	// Allocate buffer for scatter-gather descriptors
	if ( pShBufCtrl->m_Purpose==DTA1XX_SHBUF_DIRIQ_CDMA )
	{
		if ( 0 == Dta1xxCdmaSglCreate(pCh) )
			pShBufDesc->m_SglAllocated = TRUE;
	} 
	else if ( pShBufCtrl->m_Purpose==DTA1XX_SHBUF_IP_RWBUF ) {
		// Map buffer in virtual continuous kernel space
		// flags = vm_area flags!
		pShBufDesc->m_pBuf = vmap(pShBufDesc->m_PageList, pShBufDesc->m_NumPages, 
								  VM_READ | VM_WRITE, PAGE_SHARED);
		if (pShBufDesc->m_pBuf == NULL) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufAlloc: Failed to map user buffer",
						PrefixStr, PrefixIndex );
#endif
			// Unlock pages
			for ( i=0; i<pShBufDesc->m_NumPages; i++ )
				  page_cache_release(pShBufDesc->m_PageList[i]);
			kfree( pShBufDesc->m_PageList );
			pShBufDesc->m_PageList = NULL;
			pShBufDesc->m_NumPages = 0;
			pShBufDesc->m_Allocated = FALSE;
			pShBufDesc->m_SglAllocated = FALSE;
			return -EFAULT;
		}
	
		if (ChannelType == DTA1XX_IP_RX_CHANNEL) {
			pIpRx->m_BufSize = pShBufCtrl->m_SharedBufSize - DTA1XX_IPRX_BUFWRAPSIZE - 
								DTA1XX_IPRX_BUFRTPSIZE;
			pIpRx->m_pEndOfBuffer = pShBufDesc->m_pBuf + pIpRx->m_BufSize;
			pIpRx->m_pRtpBuffer = pIpRx->m_pEndOfBuffer + DTA1XX_IPRX_BUFWRAPSIZE;

			// Initialise read/write pointers
			pIpRx->m_pRead = pShBufDesc->m_pBuf;
			pIpRx->m_pWrite = pShBufDesc->m_pBuf;

	#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufAlloc: Address=0x%p Used-Size=%d "
					    "Full-Size=%d", PrefixStr, PrefixIndex,
					    pShBufDesc->m_pUserBuf, pIpRx->m_BufSize,
						pShBufCtrl->m_SharedBufSize );
	#endif

			// Initialize RTP Buffers
			return Dta1xxNwIpRxInitRtpBuffers(pIpRx);
		}
		else {	// ChannelType == DTA1XX_IP_TX_CHANNEL
			pIpTx->m_BufSize = pShBufCtrl->m_SharedBufSize - DTA1XX_IPTX_BUFCOMMSIZE;
			pIpTx->m_pCommon = (UserIpTxCommon*)(pShBufDesc->m_pBuf + pIpTx->m_BufSize);

			// Initialise read/write offset
			pIpTx->m_pCommon->m_ReadOffset = 0;
			pIpTx->m_pCommon->m_WriteOffset = 0;

	#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufAlloc: Address=0x%p Size=%d",
					    PrefixStr, PrefixIndex, pShBufDesc->m_pUserBuf, pIpTx->m_BufSize );
	#endif
			// OTHER INITIALISATION
		}
	}
	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSharedBufFree -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dta1xxSharedBufFree(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	DTA1XX_SHAREDBUF_CTRL* pShBufCtrl)	// Shared buffer control
{
    Channel*  pCh=NULL;
	Int  Purpose = pShBufCtrl->m_Purpose;
	Int  PortIndex = pShBufCtrl->m_PortIndex;
	Int  ChannelType = pShBufCtrl->m_ChannelType;
	Int  ChannelIndex = pShBufCtrl->m_ChannelIndex;
	Int  TypeNumber = pFdo->m_TypeNumber;
	BOOLEAN  Is11X = (   TypeNumber==110 || TypeNumber==111 || TypeNumber==112
					  || TypeNumber==115 || TypeNumber==116 || TypeNumber==117);
	
	ShBufDesc* pShBufDesc=NULL;
	UserIpRxChannel*  pIpRx = NULL;
	UserIpTxChannel*  pIpTx = NULL;
	
	Int  PrefixIndex = 0;			// Index used in debug prints
	char*  PrefixStr = "";			// Used in debug prints: "", "IpRx", "IpTx"

	// Set KdPrint related variables
	switch (Purpose) {
		case DTA1XX_SHBUF_DIRIQ_CDMA:
			PrefixStr = "";
			PrefixIndex = PortIndex;
			break;
		case DTA1XX_SHBUF_IP_RWBUF:
			PrefixStr = (ChannelType == DTA1XX_IP_RX_CHANNEL) ? "IpRx" : "IpTx";
			PrefixIndex = ChannelIndex;
			break;
	}

#if LOG_LEVEL > 1
		DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufRelease: free shared buffer",
					PrefixStr, PrefixIndex );
#endif

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Pre-Release Checks -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

		// Purpose-dependent code
	if (Purpose == DTA1XX_SHBUF_DIRIQ_CDMA) {

		// FOR NOW: we only support the DTA-110(T)/112/115/116/117
		if ( !Is11X )
		{
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufFree: ONLY SUPPORTED FOR "
						 "DTA-110/111/112/115/116/117", PrefixStr, PrefixIndex );
#endif
			return -EFAULT;
		}
		// Check channel type and index
		if (   ChannelType!=DTA1XX_TS_TX_CHANNEL
			|| (PortIndex!=0 && TypeNumber==110)
			|| (PortIndex!=0 && TypeNumber==111)
			|| (PortIndex!=1 && TypeNumber==112)
			|| (PortIndex!=1 && TypeNumber==115)
			|| (PortIndex!=1 && TypeNumber==116)
			|| (PortIndex!=1 && TypeNumber==117) )
		{
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufFree: ChannelType=%d INVALID",
						PrefixStr, PrefixIndex, ChannelType );
#endif
			return -EFAULT;
		}

		// FOR NOW: we only support shared buffers for CDMA purposes
		if ( pShBufCtrl->m_Purpose != DTA1XX_SHBUF_DIRIQ_CDMA )
		{
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufFree: Purpose=%d INVALID",
						PrefixStr, PrefixIndex, pShBufCtrl->m_Purpose );
#endif
			return -EFAULT;
		}

		// Get channel & shared buffer descriptor
		pCh = &pFdo->m_Channel[PortIndex];
		pShBufDesc = &(pCh->m_CdmaShBuf);

		// Sanity check: no CDMA may be running
		if ( pCh->m_CdmaDmaRunning != FALSE )
		{
	#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufFree: CDMA STILL RUNNING?!?",
						PrefixStr, PrefixIndex);
	#endif
			return -EFAULT;
		}
	} if (Purpose == DTA1XX_SHBUF_IP_RWBUF) {
		if (ChannelType == DTA1XX_IP_RX_CHANNEL) {
			if (ChannelIndex<0) {
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "[%s?] Dta1xxSharedBufFree: ChannelIndex=%d IS "
						    "OUT OF RANGE!!", PrefixStr, ChannelIndex);
#endif
				return STATUS_INVALID_PARAMETER;
			}
			// Get channel pointer
			pIpRx = Dta1xxNwIpRxGetChannel(pFdo, ChannelIndex);
			if (pIpRx == NULL) {
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufFree: ChannelIndex=%d IS "
						    "NOT AVAILABLE!!", PrefixStr, PrefixIndex, ChannelIndex );
#endif
				return STATUS_UNSUCCESSFUL;
			}
			if (pIpRx->m_RxControl != DTA1XX_RXCTRL_IDLE) {
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufFree: RxControl=%d not in "
						    "IDLE state", PrefixStr, PrefixIndex, pIpRx->m_RxControl );
#endif
				return STATUS_UNSUCCESSFUL;
			}
			pShBufDesc = &(pIpRx->m_SharedBuf);
		}
		else if (ChannelType == DTA1XX_IP_TX_CHANNEL) {
			if (ChannelIndex<0) {
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "[%s?] Dta1xxSharedBufFree: ChannelIndex=%d IS "
						    "OUT OF RANGE!!", PrefixStr, ChannelIndex );
#endif
				return STATUS_INVALID_PARAMETER;
			}
			// Get channel pointer
			pIpTx = Dta1xxNwIpTxGetChannel(pFdo, ChannelIndex);
			if (pIpTx == NULL) {
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufFree: ChannelIndex=%d IS "
						    "NOT AVAILABLE!!",  PrefixStr, PrefixIndex, ChannelIndex );
#endif
				return STATUS_UNSUCCESSFUL;
			}
			if (pIpTx->m_TxControl != DTA1XX_TXCTRL_IDLE) {
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "[%s%d] Dta1xxSharedBufFree: m_TxControl not in "
						    "IDLE state", PrefixStr, PrefixIndex );
#endif
				return STATUS_UNSUCCESSFUL;
			}
			pShBufDesc = &(pIpTx->m_SharedBuf);
		} else {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[?] Dta1xxSharedBufFree: ChannelType=%d NOT SUPPORTED",
					    ChannelType);
#endif
			return STATUS_UNSUCCESSFUL;
		}
	}
	else
		return STATUS_UNSUCCESSFUL;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Buffer Release -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	// In a separate routine, because clean-up also has to release buffers
	if ( pShBufCtrl->m_Purpose == DTA1XX_SHBUF_DIRIQ_CDMA )
		return Dta1xxSharedBufRelease(pCh, PortIndex, pShBufDesc, pShBufCtrl->m_Purpose);
	else if (ChannelType == DTA1XX_IP_RX_CHANNEL) {
		return Dta1xxNwIpRxReleaseBuf(pFdo, pIpRx);
	}
	else { //(ChannelType == DTA1XX_IP_TX_CHANNEL) 
		return Dta1xxNwIpTxReleaseBuf(pFdo, pIpTx);
	}
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSharedBufRelease -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dta1xxSharedBufRelease(
	Channel* pCh,			// Channel pointer
	Int PortIndex,			// Zero-based port index
	ShBufDesc* pShBufDesc,	// Shared buffer descriptor
	Int Purpose)			// Purpose of shread buffer
{
	Int i;

#if LOG_LEVEL > 1
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSharedBufRelease: releasing shared buffer",
					 PortIndex );
#endif

	// Buffer shouldn't have been released already
	if ( !pShBufDesc->m_Allocated )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSharedBufRelease: WARNING, BUFFER HAS BEEN "
								 "RELEASED ALREADY", PortIndex );
#endif
		return 0;	// Nothing to release
	}

	// Special processing for CDMA
	if ( Purpose == DTA1XX_SHBUF_DIRIQ_CDMA )
	{
		// If CDMA is still running => stop CDMA first
		if ( pCh->m_CdmaDmaRunning )
			Dta1xxCdmaStop(pCh);

		// Release CDMA Scatter-Gather List resources
		Dta1xxCdmaSglCleanUp(pCh);
	}

	// Sanity check: verify pointer before using it
	if ( pShBufDesc->m_PageList != NULL )
	{
		// Unlock pages
		for ( i=0; i<pShBufDesc->m_NumPages; i++ )
			  page_cache_release(pShBufDesc->m_PageList[i]);

		// Free buffer for page list
		kfree( pShBufDesc->m_PageList );
	}

	// Reset status members
	pShBufDesc->m_PageList = NULL;
	pShBufDesc->m_NumPages = 0;
	pShBufDesc->m_Allocated = FALSE;
	pShBufDesc->m_pUserBuf = NULL;
	return 0;
}
