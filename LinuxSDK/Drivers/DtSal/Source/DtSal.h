//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtSal.h *#*#*#*#*#*#*#*#*#* (C) 2010-2016 DekTec
//
// This file contains the driver SAL interface. A driver only needs to include this file
// to use all the SAL support.
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

#ifndef __DT_SAL_H
#define __DT_SAL_H

#ifdef WINBUILD
// When building under linux we get warnings for unused variables. Enable this specific
// warning as well when building for linux so it gets spotted earlier.
#pragma warning(3 : 4101)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- OS specific includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#ifdef WINBUILD
#ifdef USES_NDIS
#include <ndis.h>
#else
#include <ntddk.h>
#endif // USES_NDIS
#ifdef USES_KMDF
#include <wdf.h>
#ifdef USB_DRIVER
#include <usbdi.h>
#include <wdfusb.h>
#endif // USB_DRIVER
#endif // USES_KMDF
#else // WINBUILD
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/pci.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31) && LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,38)
    #include <linux/smp_lock.h>
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
#include <linux/semaphore.h>        // semaphore
#else
#include <asm/semaphore.h>          // semaphore
#endif
#include <asm/uaccess.h>
#include <linux/delay.h>
#include <linux/swap.h>
#include <linux/pagemap.h>
#include <linux/vmalloc.h>
#include <linux/workqueue.h>
#include <asm/segment.h>
#include <asm/uaccess.h>
#include <linux/buffer_head.h>
#include <linux/time.h>
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,4)
    #define NO_KTHREAD_SUPPORT
    #include <linux/sched.h>        // task_struct
#else
    #include <linux/kthread.h>
#endif
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,11,0)
#include <linux/sched/signal.h>
#endif
#ifdef NW_DRIVER
#include <linux/netdevice.h>
#endif // NW_DRIVER
#ifdef USB_DRIVER
#include <linux/usb.h>
#endif // USB_DRIVER
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DekTec basic types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#include <DtTypes.h>
#include <DtCommon.h>


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#ifndef __section
#define  __section(S)  __attribute__ ((__section__(S)))
#endif
#ifndef __devinitconst
#define  __devinitconst  __section(".devinit.rodata")
#endif
#ifndef DEFINE_PCI_DEVICE_TABLE
#define  DEFINE_PCI_DEVICE_TABLE(_table)  const struct pci_device_id _table[] __devinitconst
#endif

#define _QUOTE_ME(Name) #Name
#define QUOTE_ME(Name)  _QUOTE_ME(Name)

#ifndef _DRIVER_NAME
#define _DRIVER_NAME    DtSal
#endif
#define DRIVER_NAME     QUOTE_ME(_DRIVER_NAME)

// Sal Tag --> 'SalT'
#define SAL_TAG         0x546C6153  // 'TlaS'

// SAL related logging
#define LOG_LEVEL_SAL         LOG_MIN
#define LOG_LEVEL_SAL_DMA     LOG_AVG
#define LOG_LEVEL_SAL_DPC     LOG_AVG
#define LOG_LEVEL_SAL_THREAD  LOG_AVG
#define LOG_LEVEL_SAL_USB     LOG_AVG


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Generic type definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
typedef struct _DtDvcObject  DtDvcObject;
typedef struct _DtPageList  DtPageList;
typedef struct _DtIoctlObject  DtIoctlObject;
typedef struct _DtVma  DtVma;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DekTec basic SAL Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#include <DtStatusCodes.h>
#include <DtString.h>
#include <DtUtility.h>


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDrvObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Wrapper struct for OS specific driver reference.
//
typedef struct _DtDrvObject
{
    DtEvtLog  m_EvtObject;
#ifdef WINBUILD
#ifdef USES_KMDF
    WDFDRIVER  m_WdfDriver;
#else
    DRIVER_OBJECT*  m_pWdmDriver;
#endif
#else
    struct module*  m_pModule;
#endif
} DtDrvObject;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDvcObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Wrapper struct for OS specific device reference.
//
struct _DtDvcObject
{
    DtEvtLog  m_EvtObject;
#ifdef WINBUILD
#ifdef USES_KMDF
    WDFDEVICE  m_WdfDevice;
#ifdef USB_DRIVER
    WDFUSBDEVICE  m_UsbDevice;
    USB_DEVICE_DESCRIPTOR  m_UsbDevDesc;
    WDFUSBINTERFACE  m_UsbInterface;
#endif
#else
    DEVICE_OBJECT*  m_pWdmDevice;
#endif
    BUS_INTERFACE_STANDARD  m_BusInterface;
#else
    struct device*  m_pDevice;
#ifdef USB_DRIVER
    struct usb_interface*  m_pUsbItf;
    struct usb_device*  m_pUsbDev;
#else
    struct pci_dev*  m_pPciDev;
    struct cdev  m_CDev;
#endif
#endif
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPhysicalAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#ifdef WINBUILD
#define  DtPhysicalAddress  PHYSICAL_ADDRESS
#else
typedef struct _DtPhysicalInt{
    union {
        UInt64  QuadPart;
        struct {
            UInt32  LowPart;
            UInt32  HighPart;
        };
    };
} DtPhysicalInt;
#define  DtPhysicalAddress  DtPhysicalInt
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtMemDesc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Memory-space descriptor.
//
typedef struct _DtMemDesc
{
    UInt  m_Length;                 // Length of memory block
    UInt8*  m_pKernel;              // Address when mapped in kernel space or NULL if not
                                    // mapped
    DtPhysicalAddress  m_PciAddr;
#ifdef WINBUILD
    PMDL  m_pMdl;                   // Allocated MDL for kernel space buffer or NULL if 
                                    // not allocated.
#endif
} DtMemDesc;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIrqObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Wrapper struct for OS specific interrupt object.
//
typedef struct _DtIrqObject
{
#ifdef WINBUILD
#ifdef USES_KMDF
    WDFINTERRUPT  m_WdfInterrupt;
#else
    PKINTERRUPT  m_pWdmInterrupt;
#endif
#else

#endif
} DtIrqObject;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtQueueObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Wrapper struct for OS specific queue object.
//
typedef struct _DtQueueObject
{
#ifdef WINBUILD
#ifdef USES_KMDF
    WDFQUEUE  m_WdfQueue;
#else
    LIST_ENTRY  m_WmdQueue;
#endif
#else
    
#endif
} DtQueueObject;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoctlObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Wrapper struct for generic and OS specific IO control params
//
struct _DtIoctlObject
{
    UInt32  m_IoctlCode;
    UInt32  m_FunctionCode;
    UInt  m_InputBufferSize;
    void*  m_pInputBuffer;
    UInt  m_OutputBufferSize;
    void*  m_pOutputBuffer;
    UInt  m_OutputBufferBytesWritten;
#ifdef WINBUILD
#ifdef  USES_KMDF    
    WDFREQUEST  m_WdfRequest;
#else
    IRP*  m_pIrp;
#endif
#else
    void*  m_pContext; // Can be used to store application specific data
#endif
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtFileObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Wrapper struct for OS specific File object
//
typedef struct _DtFileObject
{
#ifdef WINBUILD
#ifdef USES_KMDF
    WDFFILEOBJECT  m_WdfFileObject;
#else
    PFILE_OBJECT  m_pWdmFileObject;
#endif
#else
    struct file*  m_pFile;
#endif
} DtFileObject;
#ifndef DT_INVALID_POINTER
#define DT_INVALID_POINTER ((void*)-1)
#endif
#ifndef DT_INVALID_FILE_OBJECT_PTR
#define DT_INVALID_FILE_OBJECT_PTR ((DtFileObject*)DT_INVALID_POINTER)
#endif

static __inline void*  DtFileGetHandle(DtFileObject* pFileObj)
{
    if (pFileObj == DT_INVALID_FILE_OBJECT_PTR)
        return DT_INVALID_POINTER;
#ifdef WINBUILD
#ifdef USES_KMDF
    return (void*)pFileObj->m_WdfFileObject;
#else
    return (void*)pFileObj->m_pWdmFileObject;
#endif
#else
    return (void*)pFileObj->m_pFile;
#endif
}

static __inline Bool DtFileCompare(const DtFileObject* pFileObj1,
                                                            const DtFileObject* pFileObj2)
{
#ifdef WINBUILD
#ifdef USES_KMDF
    return (pFileObj1->m_WdfFileObject==pFileObj2->m_WdfFileObject);
#else
    return (pFileObj1->m_pWdmFileObject==pFileObj2->m_pWdmFileObject);
#endif
#else
    return (pFileObj1->m_pFile==pFileObj2->m_pFile);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtUserMapping -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Wrapper struct for OS specific user mapping info
//
typedef struct _DtUserMapping  DtUserMapping;
struct _DtUserMapping
{
    DtUserMapping*  m_pNext;
    DtUserMapping*  m_pPrev;
    UInt  m_RefCount;               // Reference count of user mapping
    void*  m_pUserVirtual;          // Virtual address in user mode
    DtFileObject  m_FileObject;     // Corresponding file object
#ifdef WINBUILD
#else
    void*  m_pPaMmap;               // Virtual address returned by mmap 
                                    // used to cleanup in userspace for munmap
#endif
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPageList -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
struct _DtPageList
{
    UInt  m_BufType;                    // Kernel or user buffer page list
    Bool  m_OwnedByUs;                  // TRUE, if pagelist is ours and we are 
                                        // responsible for deleting it
    UInt8*  m_pVirtualKernel;
#ifdef WINBUILD
    PMDL  m_pMdl;
#else
    struct page**  m_pPages;            // Pointer to list of pages describing buffer
    UInt  m_NumPages;                   // Number of pages in page list
#endif
};

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVma -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Virtual memory area struct describing a virtual memory area. 
//
// Note: the Windows version of this struct is "empty".
//
struct _DtVma
{
#ifdef WINBUILD
    void* m_pUnused;                // Dummy member to satisfy compiler (a C struct needs 
                                    // member)
#else
    struct vm_area_struct* m_pVma;
#endif // #ifdef LINBUILD
};

#include <DtNonVolatileSettings.h>
#include <DtMutex.h>
#include <DtSpinLock.h>
#include <DtFile.h>
#include <DtEvent.h>
#include <DtDpc.h>
#include <DtWorkItem.h>
#include <DtThread.h>
#include <DtDma.h>
#include <DtList.h>
#include <DtTrigEvt.h>
#ifdef USB_DRIVER
#include <DtUsb.h>
#endif

#endif // __DT_SAL_H
