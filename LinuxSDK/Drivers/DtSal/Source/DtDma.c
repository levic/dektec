//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDma.c *#*#*#*#*#*#*#*#*#* (C) 2010-2016 DekTec
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
#include "DtDma.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Helper functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
#ifndef SKIP_DMA
#ifdef WINBUILD
EVT_WDF_PROGRAM_DMA  DtDmaSgListCreated;


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDmaSgListCreated -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the KMDF framework when the SG list is created
// It triggers an event the DtDmaCreateSgList function is waiting for
//
BOOLEAN  DtDmaSgListCreated(
    WDFDMATRANSACTION  Transaction,
    WDFDEVICE  Device,
    PVOID  pContext,
    WDF_DMA_DIRECTION  Direction,
    PSCATTER_GATHER_LIST  pSgList)
{
    DmaOsSgl*  pOsSgl = (DmaOsSgl*)pContext;
    pOsSgl->m_pSgList = pSgList;

    // Trigger event
    KeSetEvent(&pOsSgl->m_DmaPrepareDoneEvent, 0, FALSE);
    return TRUE; // We return that the DMA is started with success. 
                 // This parameter is ignored by the KMDF framework in current release.
}

#else
//=+=+=+=+=+=+=+=+=+=+=+=+=+ Linux Kernel version dependancies +=+=+=+=+=+=+=+=+=+=+=+=+=+

// The dma functions are renamed from v4.5.0
// The compatibility functions are removed from kernel version v5.18
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,5,0)
static inline int pci_set_dma_mask_dt(struct pci_dev* dev, u64 mask)
{
    return dma_set_mask(&dev->dev, mask);
}
static inline int pci_set_consistent_dma_mask_dt(struct pci_dev* dev, u64 mask)
{
    return dma_set_coherent_mask(&dev->dev, mask);
}
static inline void* pci_alloc_consistent_dt(struct pci_dev* hwdev, size_t size,
                                                                   dma_addr_t* dma_handle)
{
    return dma_alloc_coherent(&hwdev->dev, size, dma_handle, GFP_ATOMIC);
}
static inline void pci_free_consistent_dt(struct pci_dev* hwdev, size_t size,
                                                       void* vaddr, dma_addr_t dma_handle)
{
    dma_free_coherent(&hwdev->dev, size, vaddr, dma_handle);
}
static inline int pci_map_sg_dt(struct pci_dev* hwdev, struct scatterlist* sg,
                                                                 int nents, int direction)
{
    return dma_map_sg(&hwdev->dev, sg, nents, (enum dma_data_direction)direction);
}
static inline void pci_unmap_sg_dt(struct pci_dev* hwdev, struct scatterlist* sg,
                                                                 int nents, int direction)
{
    dma_unmap_sg(&hwdev->dev, sg, nents, (enum dma_data_direction)direction);
}
static inline void pci_dma_sync_sg_for_device_dt(struct pci_dev* hwdev, 
                                        struct scatterlist* sg, int nelems, int direction)
{
    dma_sync_sg_for_device(&hwdev->dev, sg, nelems, (enum dma_data_direction)direction);
}
#else
#define pci_set_dma_mask_dt pci_set_dma_mask
#define pci_set_consistent_dma_mask_dt pci_set_consistent_dma_mask
#define pci_alloc_consistent_dt pci_alloc_consistent
#define pci_free_consistent_dt pci_free_consistent
#define pci_map_sg_dt pci_map_sg
#define pci_unmap_sg_dt pci_unmap_sg

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,5)
#define  pci_dma_sync_sg_for_device_dt  pci_dma_sync_sg
#else
#define pci_dma_sync_sg_for_device_dt pci_dma_sync_sg_for_device
#endif

#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Scatter/Gather helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Kernel 2.6.24 provides an updated struct scatterlist API. We use __inline functions as
// wrapper for kernel dependant scatter-gather code

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtSgInitTable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline void  DtDmaSgInitTable(
    struct scatterlist*  pSGL,
    Int  NumEntries)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24)
    // Kernel 2.6.24 or later
    sg_init_table(pSGL, NumEntries);
#else
    // Pre kernel 2.6.24
    return; // NOTHING TODO
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtSgSetPage -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static __inline void  DtDmaSgSetPage(
    struct scatterlist*  pSGL,
    struct page*  pPage,
    Int  Size,
    Int  Offset)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24)
    // Kernel 2.6.24 or later
    sg_set_page(pSGL, pPage, Size, Offset);
#else
    // Pre kernel 2.6.24
    pSGL->page = pPage;
    pSGL->length = Size;
    pSGL->offset = Offset;
#endif
}

// DMA_32BIT_MASK/DMA_64BIT_MASK are deprecated at least since 2.6.18 and later removed
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,18)
    #define DT_DMA_32BIT_MASK  DMA_32BIT_MASK
    #define DT_DMA_64BIT_MASK  DMA_64BIT_MASK
#else  
    #define DT_DMA_32BIT_MASK  DMA_BIT_MASK(32)
    #define DT_DMA_64BIT_MASK  DMA_BIT_MASK(64)
#endif
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDmaInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialize the DMA device
//
DtStatus  DtDmaInit(
    DtDvcObject*  pDevice, 
    DmaDevice*  pDmaDevice,
    UInt  MaxDmaTransfer,
    Bool  Supports64Bit)
{
#ifdef WINBUILD
    WDF_DMA_ENABLER_CONFIG  DmaEnablerConfig;
    WDF_DMA_PROFILE  DmaProfile;
    NTSTATUS  Status;
    UInt  MaxDmaLengthSupported;
#else
#endif
    pDmaDevice->m_NotEnoughMapRegisters = FALSE;

#ifdef WINBUILD
    WdfDeviceSetAlignmentRequirement(pDevice->m_WdfDevice, FILE_LONG_ALIGNMENT);

    if (Supports64Bit)
        DmaProfile = WdfDmaProfileScatterGather64Duplex;  // 64-bit SG List
    else
        DmaProfile = WdfDmaProfileScatterGatherDuplex;  // 32-bit SG List
    
    WDF_DMA_ENABLER_CONFIG_INIT(&DmaEnablerConfig, DmaProfile, MaxDmaTransfer);
    Status = WdfDmaEnablerCreate(pDevice->m_WdfDevice, &DmaEnablerConfig,
                                      WDF_NO_OBJECT_ATTRIBUTES, &pDmaDevice->m_DmaObject);
    if (!NT_SUCCESS(Status))
    {
        DtDbgOut(ERR, SAL_DMA, "Error initializing DMA enabler object.");
        return DT_STATUS_OUT_OF_RESOURCES;
    }

    // Check if we have enough MAP-registers to create a SG list for the max. buffer size. 
    MaxDmaLengthSupported = WdfDmaEnablerGetFragmentLength(pDmaDevice->m_DmaObject,
                                                           WdfDmaDirectionReadFromDevice);
    if (MaxDmaLengthSupported < MaxDmaTransfer)
        pDmaDevice->m_NotEnoughMapRegisters = TRUE;

    MaxDmaLengthSupported = WdfDmaEnablerGetFragmentLength(pDmaDevice->m_DmaObject,
                                                           WdfDmaDirectionWriteToDevice);
    if (MaxDmaLengthSupported < MaxDmaTransfer)
        pDmaDevice->m_NotEnoughMapRegisters = TRUE;   

#else
    if (Supports64Bit)
    {
        if (pci_set_dma_mask_dt(pDevice->m_pPciDev, DT_DMA_64BIT_MASK)<0 ||
                  pci_set_consistent_dma_mask_dt(pDevice->m_pPciDev, DT_DMA_64BIT_MASK)<0)
        {
            DtDbgOut(ERR, SAL_DMA, "Unable to set DMA 64-bit mask.");
            return DT_STATUS_FAIL;
        }
    } else {
        if (pci_set_dma_mask_dt(pDevice->m_pPciDev, DT_DMA_32BIT_MASK)<0 ||
                  pci_set_consistent_dma_mask_dt(pDevice->m_pPciDev, DT_DMA_32BIT_MASK)<0)
        {
            DtDbgOut(ERR, SAL_DMA, "Unable to set DMA 32-bit mask.");
            return DT_STATUS_FAIL;
        }
        // Check if the memory in the system can be larger then 4GB, if so we don't use
        // SGL transfer because the buffer to transfer could be mapped above the 4GB.
#if defined(LIN64) || defined(CONFIG_X86_PAE)
        pDmaDevice->m_NotEnoughMapRegisters = TRUE;
#endif
    }
#endif
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDmaCreateDirectBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This functions creates a continuous DMA buffer.
//
// Align: Alignment of buffer. If 0, no alignment required
//
DtStatus  DtDmaCreateDirectBuffer(
    DtDvcObject*  pDevice,
    DmaDevice*  pDmaDevice, 
    DmaDirectBuffer*  pDmaBuffer,
    UInt  BufSize,
    UInt  Align)
{
#ifdef WINBUILD
    NTSTATUS  Status;
#else
    dma_addr_t DmaAddress;
#endif
    UInt BufSizeAligned;
    UInt  Offset = 0;
        
    if (pDmaBuffer->m_Allocated)
        return DT_STATUS_IN_USE;

    BufSizeAligned = BufSize + Align + sizeof(void*);

#ifdef WINBUILD
    // Create common buffer.
    Status = WdfCommonBufferCreate(pDmaDevice->m_DmaObject, BufSizeAligned, 
                                   WDF_NO_OBJECT_ATTRIBUTES, &pDmaBuffer->m_CommonBuffer);
    if (!NT_SUCCESS(Status))
    {
        DtDbgOut(ERR, SAL_DMA, "Error creating common buffer."
                              "Status:0x%xh, Requested size: %d", Status, BufSizeAligned);
        return DT_STATUS_OUT_OF_MEMORY;
    }

    // Store virtual and physical address
    pDmaBuffer->m_pVirtualAddress = WdfCommonBufferGetAlignedVirtualAddress(
                                                              pDmaBuffer->m_CommonBuffer);
    pDmaBuffer->m_PhysicalAddress = WdfCommonBufferGetAlignedLogicalAddress(
                                                              pDmaBuffer->m_CommonBuffer);
    // Align returned address
    if (Align != 0)
    {   if ((DtPtrToUInt(pDmaBuffer->m_pVirtualAddress)%Align) != 0)
            Offset = (UInt8)(Align - DtPtrToUInt(pDmaBuffer->m_pVirtualAddress) % Align);

        pDmaBuffer->m_pVirtualAddress = pDmaBuffer->m_pVirtualAddress + Offset;
        pDmaBuffer->m_PhysicalAddress.LowPart = pDmaBuffer->m_PhysicalAddress.LowPart + 
                                                                                   Offset;
    }
#else
    // Create common buffer and store virtual and physical address.
    pDmaBuffer->m_pVirtualAddressUnaligned = pci_alloc_consistent_dt(pDevice->m_pPciDev,
                                                             BufSizeAligned, &DmaAddress);
    pDmaBuffer->m_PhysicalAddressUnaligned.QuadPart = DmaAddress;
    if (Align != 0)
    {   
        if ((DtPtrToUInt(pDmaBuffer->m_pVirtualAddressUnaligned)%Align) != 0)
            Offset = (UInt8)(Align-(DtPtrToUInt(pDmaBuffer->m_pVirtualAddress)) % Align);

        pDmaBuffer->m_pVirtualAddress = pDmaBuffer->m_pVirtualAddressUnaligned + Offset;
        pDmaBuffer->m_PhysicalAddress.QuadPart = 
                                 pDmaBuffer->m_PhysicalAddressUnaligned.QuadPart + Offset;
    }
#endif
    pDmaBuffer->m_BufferLength = BufSize;
    pDmaBuffer->m_Allocated = TRUE;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDmaFreeDirectBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDmaFreeDirectBuffer(
    DtDvcObject*  pDevice,
    DmaDirectBuffer*  pDmaBuffer)
{
    if (!pDmaBuffer->m_Allocated)
        return DT_STATUS_OK;
#ifdef WINBUILD
    WdfObjectDelete(pDmaBuffer->m_CommonBuffer);
#else
    pci_free_consistent_dt(pDevice->m_pPciDev, pDmaBuffer->m_BufferLength,
                                         pDmaBuffer->m_pVirtualAddressUnaligned,
                                         pDmaBuffer->m_PhysicalAddressUnaligned.QuadPart);
#endif
     return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDmaCreateSgList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Pre: pOsSgl->m_PageList contains the pages of the pBuffer
//
DtStatus  DtDmaCreateSgList(
    DtDvcObject*  pDevice,
    DmaDevice*  pDmaDevice, 
    void*  pBuffer, 
    UInt  BufSize,
    UInt  Direction,
    DmaOsSgl*  pOsSgl)
{
    DtPageList*  pPageList = &pOsSgl->m_PageList;
#ifdef WINBUILD
    NTSTATUS  Status;
    WDF_DMA_DIRECTION  WdfDmaDirection;
#else
    Int  i;
    UInt  SgSizeReq;
#endif

    if (pOsSgl->m_Allocated)
        return DT_STATUS_IN_USE;
#ifdef WINBUILD
    Status = WdfDmaTransactionCreate(pDmaDevice->m_DmaObject, WDF_NO_OBJECT_ATTRIBUTES, 
                                                               &pOsSgl->m_WdfTransaction);
    if (Status != STATUS_SUCCESS)
        return DT_STATUS_OUT_OF_RESOURCES;
    
    if (Direction == DT_DMA_DIRECTION_FROM_DEVICE)
        WdfDmaDirection = WdfDmaDirectionReadFromDevice;
    else
        WdfDmaDirection = WdfDmaDirectionWriteToDevice;
    
    Status = WdfDmaTransactionInitialize(pOsSgl->m_WdfTransaction,  DtDmaSgListCreated,
                                    WdfDmaDirection, pPageList->m_pMdl, pBuffer, BufSize);
    if (Status == STATUS_SUCCESS) 
    {   
        KeInitializeEvent(&pOsSgl->m_DmaPrepareDoneEvent, NotificationEvent, FALSE);
        Status = WdfDmaTransactionExecute(pOsSgl->m_WdfTransaction, pOsSgl);
    }
    if (Status != STATUS_SUCCESS)
    {   
        WdfObjectDelete(pOsSgl->m_WdfTransaction);
        return DT_STATUS_OUT_OF_RESOURCES;
    }
    KeWaitForSingleObject(&pOsSgl->m_DmaPrepareDoneEvent, Executive, KernelMode, FALSE, 
                                                                                    NULL);
#else
    // Allocate a buffer to store the OS SG list
    SgSizeReq = sizeof(struct scatterlist) * pPageList->m_NumPages;
    if (SgSizeReq>pOsSgl->m_SgListSize || pOsSgl->m_pSgList==NULL)
    {
        kfree(pOsSgl->m_pSgList);
        pOsSgl->m_pSgList = kmalloc(SgSizeReq, GFP_KERNEL);
        pOsSgl->m_SgListSize = SgSizeReq;
    }
    if (pOsSgl->m_pSgList == NULL)
    {
        DtDbgOut(ERR, SAL_DMA, 
                         "Failed to allocate buffer (%llu-bytes) for scatter-gather-list",
                         (UInt64)(sizeof(struct scatterlist) * pPageList->m_NumPages));
        return DT_STATUS_OUT_OF_MEMORY;
    }

    DtDmaSgInitTable(pOsSgl->m_pSgList, pPageList->m_NumPages);
   
    // Fill the SGL descriptors with the pages and size
    for (i=0; i<pPageList->m_NumPages; i++) 
    {
        Int Offset = DtPtrToUInt(pBuffer) & (PAGE_SIZE-1);
        if (i==0 && Offset!=0)
            // If the pBuffer is not page aligned, we have to calculate the page offset
            DtDmaSgSetPage(&pOsSgl->m_pSgList[i], pPageList->m_pPages[i], 
                         (PAGE_SIZE-Offset > BufSize ? BufSize:PAGE_SIZE-Offset), Offset);
        else if (i+1==pPageList->m_NumPages && (BufSize+Offset)%PAGE_SIZE!=0)
            // If the BufSize+Offset is not page aligned, we have to calculate the last
            // bytes of the last page
            DtDmaSgSetPage(&pOsSgl->m_pSgList[i], pPageList->m_pPages[i], 
                                                         (BufSize+Offset) % PAGE_SIZE, 0);
        else
            DtDmaSgSetPage(&pOsSgl->m_pSgList[i], pPageList->m_pPages[i], PAGE_SIZE, 0);
    }
    
    // Map scatter-gather-list
    pOsSgl->m_NumSglEntries = pci_map_sg_dt(
           pDevice->m_pPciDev, pOsSgl->m_pSgList, pPageList->m_NumPages,
           (Direction == DT_DMA_DIRECTION_FROM_DEVICE ? DMA_FROM_DEVICE : DMA_TO_DEVICE));
#endif
    pOsSgl->m_BufferLength = BufSize;
    pOsSgl->m_Allocated = TRUE;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDmaFreeSgList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDmaFreeSgList(DtDvcObject* pDevice, DmaOsSgl* pOsSgl, UInt Direction)
{
    if (!pOsSgl->m_Allocated)
        return DT_STATUS_OK;
#ifdef WINBUILD
    WdfObjectDelete(pOsSgl->m_WdfTransaction);
    // Cleanup SG List
    pOsSgl->m_pSgList = NULL;  // ???Do we need to do something...???  or is KMDF deleting
                               // it for us when deleting the transaction??
#else
    pci_unmap_sg_dt(pDevice->m_pPciDev, pOsSgl->m_pSgList, pOsSgl->m_PageList.m_NumPages,
           (Direction == DT_DMA_DIRECTION_FROM_DEVICE ? DMA_FROM_DEVICE : DMA_TO_DEVICE));
#endif
    pOsSgl->m_Allocated = FALSE;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDmaFlushCacheSglPreDma -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDmaFlushCacheSglPreDma(
    DtDvcObject*  pDevice, 
    DmaOsSgl*  pOsSgl, 
    UInt  Direction)
{
#ifdef WINBUILD
    KeFlushIoBuffers(pOsSgl->m_PageList.m_pMdl, Direction == DT_DMA_DIRECTION_FROM_DEVICE,
                                                                                    TRUE);
#else
    if (Direction == DT_DMA_DIRECTION_TO_DEVICE)
        pci_dma_sync_sg_for_device_dt(pDevice->m_pPciDev, pOsSgl->m_pSgList, 
                                                  pOsSgl->m_NumSglEntries, DMA_TO_DEVICE);
    else
        pci_dma_sync_sg_for_device_dt(pDevice->m_pPciDev, pOsSgl->m_pSgList, 
                                                pOsSgl->m_NumSglEntries, DMA_FROM_DEVICE);
#endif
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDmaFinalSglTransfer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDmaFinalSglTransfer(
    DmaOsSgl*  pOsSgl)
{
#ifdef WINBUILD
    NTSTATUS  NtStatus;
    
    if (!WdfDmaTransactionDmaCompleted(pOsSgl->m_WdfTransaction, &NtStatus))
        DT_ASSERT(FALSE);

    if (!NT_SUCCESS(NtStatus))
        return DT_STATUS_FAIL;
#else
#endif
    return DT_STATUS_OK;
}
#endif // SKIP_DMA

