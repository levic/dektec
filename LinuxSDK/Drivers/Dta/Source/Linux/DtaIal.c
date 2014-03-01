//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaIal.c *#*#*#*#*#*#*#*#*# (C) 2010-2012 DekTec
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtaIncludes.h>
#include <linux/poll.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Compile time defaults
#ifndef DTA_MAX_DEVICES
#define  DTA_MAX_DEVICES 10
#endif
#ifndef DTA_MAJOR
#define  DTA_MAJOR 0   /* 0 --> Dynamic major by default */
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Global variables
Int  g_DtaMaxDevices = DTA_MAX_DEVICES;
Int  g_DtaMajor = DTA_MAJOR;
Int  g_DtaMinor = 0;
DtaChildDriver  g_DtaChildDriver;   // Interface to Child (Network) driver

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Global variables -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Load time parameters
module_param(g_DtaMaxDevices, int, S_IRUGO);  // Do not change int to Int!
module_param(g_DtaMajor, int, S_IRUGO);       // Do not change int to Int!

// Supported devices table
extern DEFINE_PCI_DEVICE_TABLE(DtaDeviceTable);

MODULE_AUTHOR("DekTec Digital Video B.V.");
MODULE_DESCRIPTION("Dta Series Driver");

// ADDED TO PREVENT TAINTED MESSAGE. THIS MODULE CAN BE USED FREELY (SEE LICENSE
// STATEMENT ABOVE).
MODULE_LICENSE("GPL";)

// Global variables
static DtaDeviceData**  g_pDtaDeviceData = NULL;
static struct class*  g_pDtaClass = NULL;
static Int  g_DtaDeviceCount = 0;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Forward declarations +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Private helper functions
static Int  DtaDevicePowerUpSeq(DtaDeviceData* pDvcData);
static Int  DtaDevicePowerDownSeq(DtaDeviceData* pDvcData, Bool IgnoreErrors);
static Int  DtaDeviceStartSeq(DtaDeviceData* pDvcData);
static void  DtaDeviceStopSeq(DtaDeviceData* pDvcData);

static void  DtaInitDeviceData(DtaDeviceData* pDvcData);
static Int  DtaInitDeviceResources(DtaDeviceData* pDvcData);
static Int  DtaFreeDeviceResources(DtaDeviceData* pDvcData);
static Int  DtaMapResources(DtaDeviceData* pDvcData);
static void  DtaUnMapResources(DtaDeviceData* pDvcData);
static Int  DtaMapRegsToUserSpace(DtaDeviceData* pDvcData, struct file* pFile,
                                                             struct vm_area_struct* pVma);
static DtStatus  DtaReleaseAddressRegsForFileObject(DtaDeviceData* pDvcData, 
                                                                     DtFileObject* pFile);

// Interrupt handler
static irqreturn_t  DtaInterrupt(Int Irq, void* pContext);

// Character device interface
static long  DtaIoctl(struct inode* pInode, struct file* pFile, unsigned int Cmd, 
                                                                       unsigned long Arg);
static long  DtaUnlockedIoctl(struct file* pFile, unsigned int Cmd, unsigned long Arg);
#ifdef CONFIG_COMPAT
// 32-bit applications using 64-bit driver
static long  DtaIoctlCompat(struct file *filp, unsigned int cmd, unsigned long arg);
#endif
static Int  DtaOpen(struct inode* pInode, struct file* pFile);
static Int  DtaClose(struct inode* pInode, struct file* pFile);
static Int  DtaMmap(struct file* pFile, struct vm_area_struct* pVma);
static UInt DtaPoll(struct file* pFile, poll_table* pPollTable);
static ssize_t DtaRead(struct file* pFile, char __user* pBuf, size_t Count, 
                                                                           loff_t* pOffp);

// PCI system interface
static Int  DtaProbe(struct pci_dev* pPciDev, const struct pci_device_id* pId);
static void  DtaRemove(struct pci_dev* pPciDev);
Int  DtaSuspend(struct pci_dev* pPciDev, pm_message_t State);
Int  DtaResume(struct pci_dev* pPciDev);

// Child devices interface
DtStatus  DtaIoctlChild(UInt32 IoctlCode, UInt InputBufferSize, void* pInputBuffer,
                                         UInt OutputBufferSize, void* pOutputBuffer, 
                                         UInt* pOutputBufferBytesWritten, void* pContext);

// Kernel module interface
static Int  DtaModuleInit(void);
static void  DtaModuleExit(void);

// Attributes
static ssize_t DtaShowSerial(
    struct device*  pDevice, 
    struct device_attribute*  pAttr, 
    char*  pBuf)
{
    DtaDeviceData* pDvcData = (DtaDeviceData*)dev_get_drvdata(pDevice);
    
    DtDbgOut(MAX, IAL, "Start");
    
    if (pDvcData != NULL)
    {
        DtDbgOut(AVG, IAL, "Serial: %llu", pDvcData->m_DevInfo.m_Serial);
        DtDbgOut(MAX, IAL, "Exit");
        return snprintf(pBuf, PAGE_SIZE, "%llu\n", pDvcData->m_DevInfo.m_Serial);
    }

    DtDbgOut(MAX, IAL, "Exit");
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Sysfs attributes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
static DEVICE_ATTR(serial, S_IRUGO, DtaShowSerial, NULL);
// Registering the sysfs attributes is done with the following two functions:
// device_create_file / device_remove_file
// The attribute itself is declared with the DEVICE_ATTR macro.
#else
// In newer kernels the DtaAttributes array is directly
// registered with the DTA device class during the module init.
static struct device_attribute DtaAttributes[] = {
    __ATTR(serial, S_IRUGO, DtaShowSerial, NULL),
    __ATTR_NULL
};
#endif


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Interface declarations +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Character interface definition -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static struct file_operations  DtaFileOps = {
    .owner          = THIS_MODULE,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
    .ioctl          = DtaIoctl,
#else
    .unlocked_ioctl = DtaUnlockedIoctl,
#endif
#ifdef CONFIG_COMPAT
    .compat_ioctl   = DtaIoctlCompat,
#endif
    .open           = DtaOpen,
    .release        = DtaClose,
    .mmap           = DtaMmap,
    .poll           = DtaPoll,
    .read           = DtaRead,
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- System interface definition -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// The pci_driver variable name must end with _ops to prevent a compiler
// warning because it contains a reference to the DtaDeviceTable wich is of type 
// '__deviceinitconst'
//
static struct pci_driver  DtaPciOps_ops = 
{
    .name = DRIVER_NAME,
    .id_table = DtaDeviceTable,
    .probe = DtaProbe,
    .remove = DtaRemove,
    .suspend = DtaSuspend,
    .resume = DtaResume,
};

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private helper functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaInitDeviceData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtaInitDeviceData(DtaDeviceData* pDvcData)
{
    // Default all 0
    memset(pDvcData, 0, sizeof(DtaDeviceData));

    // IAL state
    pDvcData->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_IDLE;
    pDvcData->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_DOWN;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaInitDeviceResources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function must initialise Linux specific device resources and get the PCI
// configuration information
//
static Int  DtaInitDeviceResources(DtaDeviceData* pDvcData)
{
    // Save ID's from config space
    pDvcData->m_DevInfo.m_VendorId = pDvcData->m_Device.m_pPciDev->vendor;
    pDvcData->m_DevInfo.m_DeviceId = pDvcData->m_Device.m_pPciDev->device;
    pDvcData->m_DevInfo.m_SubVendorId = pDvcData->m_Device.m_pPciDev->subsystem_vendor;
    pDvcData->m_DevInfo.m_SubSystemId = pDvcData->m_Device.m_pPciDev->subsystem_device;
    pDvcData->m_DevInfo.m_BusNumber = pDvcData->m_Device.m_pPciDev->bus->number;
    pDvcData->m_DevInfo.m_SlotNumber = PCI_SLOT(pDvcData->m_Device.m_pPciDev->devfn);
    return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaFreeDeviceResources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtaFreeDeviceResources(DtaDeviceData* pDvcData)
{
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMapResources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtaMapResources(DtaDeviceData* pDvcData)
{
    Int  Result;
    DtStatus  Status;

    DtDbgOut(MIN, IAL, "Start");

    // Enable PCI device
    Result = pci_enable_device(pDvcData->m_Device.m_pPciDev);
    
    // Only proceed if no errors
    if (Result == 0)
    {
        // Enable DMA
        pci_set_master(pDvcData->m_Device.m_pPciDev);
        
        // Aquire memory resources
        Result = pci_request_regions(pDvcData->m_Device.m_pPciDev, DRIVER_NAME);
    }

    if (pDvcData->m_DevInfo.m_UsesPlxChip)
    {
        // Memory mapping for devices with PLX chip (BAR0 and BAR2)
        // Only proceed if no errors
        if (Result == 0)
        {
            // Get PLX address range (BAR0)
            pDvcData->m_Pci905XConfRegs.m_PciAddr.QuadPart =
                pci_resource_start(pDvcData->m_Device.m_pPciDev, 0);
            pDvcData->m_Pci905XConfRegs.m_Length =
                pci_resource_len(pDvcData->m_Device.m_pPciDev, 0);
            // Map into kernel space
            pDvcData->m_Pci905XConfRegs.m_pKernel = 
                ioremap_nocache(pDvcData->m_Pci905XConfRegs.m_PciAddr.QuadPart,
                                pDvcData->m_Pci905XConfRegs.m_Length);

            if (pDvcData->m_Pci905XConfRegs.m_pKernel == NULL)
                Result = -EFAULT;

            DtDbgOut(AVG, IAL, "PlxRegs: PciAddr=0x%08x%08x, pKernel=0x%p", 
                                           pDvcData->m_Pci905XConfRegs.m_PciAddr.HighPart,
                                           pDvcData->m_Pci905XConfRegs.m_PciAddr.LowPart,
                                           pDvcData->m_Pci905XConfRegs.m_pKernel);
        }

        // Only proceed if no errors
        if (Result == 0)
        {
            Status = DtaDeviceInitPci905X(pDvcData);
            if (!DT_SUCCESS(Status))
                Result = -EFAULT;
        }

        // Only proceed if no errors
        if (Result == 0)
        {
            // Get Dta operational address range (BAR2)
            pDvcData->m_DtaRegs.m_PciAddr.QuadPart =
                pci_resource_start(pDvcData->m_Device.m_pPciDev, 2);
            pDvcData->m_DtaRegs.m_Length =
                pci_resource_len(pDvcData->m_Device.m_pPciDev, 2);
            // Map into kernel space
            pDvcData->m_DtaRegs.m_pKernel =
                ioremap_nocache(pDvcData->m_DtaRegs.m_PciAddr.QuadPart,
                                pDvcData->m_DtaRegs.m_Length);

            if (pDvcData->m_DtaRegs.m_pKernel == NULL)
                Result = -EFAULT;
        
            DtDbgOut(AVG, IAL, "DtaRegs: PciAddr=0x%llx, pKernel=0x%p", 
                                                   pDvcData->m_DtaRegs.m_PciAddr.QuadPart,
                                                   pDvcData->m_DtaRegs.m_pKernel);
        }
    } else { 
        // Memory mapping for devices without PLX chip (only BAR0)
        // Only proceed if no errors
        if (Result == 0)
        {
            // Get DTA1xx memory address range (BAR0)
            pDvcData->m_DtaRegs.m_PciAddr.QuadPart =
                pci_resource_start(pDvcData->m_Device.m_pPciDev, 0);
            pDvcData->m_DtaRegs.m_Length =
                pci_resource_len(pDvcData->m_Device.m_pPciDev, 0);
            // Map into kernel space
            pDvcData->m_DtaRegs.m_pKernel =
                                   ioremap_nocache(pDvcData->m_DtaRegs.m_PciAddr.QuadPart,
                                   pDvcData->m_DtaRegs.m_Length);

            if (pDvcData->m_DtaRegs.m_pKernel == NULL)
                Result = -EFAULT;
        }
    }
    
    // Only proceed if no errors
    
    
    // Cleanup if any errors
    if (Result < 0)
    {
        DtaUnMapResources(pDvcData);
        DtDbgOut(ERR, IAL, "Error mapping resources");
    }

    DtDbgOut(MIN, IAL, "Exit");

    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIalRegisterISR -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtaIalRegisterISR(DtaDeviceData* pDvcData)
{
    Int  Result;

    if (pDvcData->m_IalData.m_ISRRegistered)
        return 0;
    
    DtDbgOut(MIN, IAL, "Start");
    
    // Request the interrupt
    Result = request_irq(pDvcData->m_Device.m_pPciDev->irq,
                            (void*)DtaInterrupt,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
                            SA_SHIRQ,            // Shared interrupt
#else
                            IRQF_SHARED,         // Shared interrupt
#endif
                            DRIVER_NAME,         // Driver name
                            pDvcData);           // is parameter
    if (Result < 0)
    {
        DtDbgOut(ERR, IAL, "Error registering ISR");
    } else 
        pDvcData->m_IalData.m_ISRRegistered = TRUE;
    
    DtDbgOut(MIN, IAL, "Exit");

    return Result;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIalUnRegisterISR -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static void  DtaIalUnRegisterISR(DtaDeviceData* pDvcData)
{
    if (!pDvcData->m_IalData.m_ISRRegistered)
        return;

    DtDbgOut(MIN, IAL, "Start");

    // Free interrupt resource
    free_irq(pDvcData->m_Device.m_pPciDev->irq, pDvcData);
    pDvcData->m_IalData.m_ISRRegistered = FALSE;

    DtDbgOut(MIN, IAL, "Exit");
    return;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaUnMapResources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtaUnMapResources(DtaDeviceData* pDvcData)
{
    DtDbgOut(MIN, IAL, "Start");

    // Unmap PCI IO memory
    if (pDvcData->m_DevInfo.m_UsesPlxChip)
    {
        if (pDvcData->m_Pci905XConfRegs.m_pKernel != NULL)
        {
            iounmap((char*)(pDvcData->m_Pci905XConfRegs.m_pKernel));
            pDvcData->m_Pci905XConfRegs.m_pKernel = NULL;
        }
    }
    if (pDvcData->m_DtaRegs.m_pKernel != NULL)
    {
        iounmap((char*)pDvcData->m_DtaRegs.m_pKernel);
        pDvcData->m_DtaRegs.m_pKernel = NULL;
    }

    // Disable DMA
    //pci_clear_master(pDvcData->m_Device.m_pPciDev);

    // Disable PCI device
    pci_disable_device(pDvcData->m_Device.m_pPciDev);

    // Finnaly...
    // Call pci_release_region AFTER pci_disable_device to prevent to devices
    // colliding on the same address range.
    pci_release_regions(pDvcData->m_Device.m_pPciDev);

    DtDbgOut(MIN, IAL, "Exit");
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDevicePowerUpSeq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtaDevicePowerUpSeq(DtaDeviceData* pDvcData)
{
    DtStatus  Status;
    
    switch (pDvcData->m_IalData.m_PowerSeqState)
    {
    case DEVICE_POWERSEQ_STATE_DOWN:
        // Call common powerup
        Status = DtaDevicePowerUp(pDvcData);
        if (!DT_SUCCESS(Status))
        {
            DtaDevicePowerDownSeq(pDvcData, TRUE);
            return -EPERM;
        }
        // Next state in power up sequence
        pDvcData->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_UP;
        
    case DEVICE_POWERSEQ_STATE_UP:
        // All channels are initialised. We can now register the ISR
        if (DtaIalRegisterISR(pDvcData) < 0)
        {
            DtaDevicePowerDownSeq(pDvcData, TRUE);
            return -EPERM;
        }

        // Call common interrupts enable
        Status = DtaDeviceInterruptEnable(pDvcData);
        if (!DT_SUCCESS(Status))
        {
            DtaDevicePowerDownSeq(pDvcData, TRUE);
            return -EPERM;
        }
        // Next state in power up sequence
        pDvcData->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_INT_ENABLED;
        
    case DEVICE_POWERSEQ_STATE_INT_ENABLED:
        // Call common powerup post
        Status = DtaDevicePowerUpPost(pDvcData);
        if (!DT_SUCCESS(Status))
        {
            DtaDevicePowerDownSeq(pDvcData, TRUE);
            return -EPERM;
        }
        // Next state in power up sequence
        pDvcData->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_UP_POST;
        
    case DEVICE_POWERSEQ_STATE_UP_POST:
    default :
        // Nothing to do, fully powered up
    break;
    }

    return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDevicePowerDownSeq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtaDevicePowerDownSeq(DtaDeviceData* pDvcData, Bool IgnoreErrors)
{
    DtStatus  Status;
    
    switch (pDvcData->m_IalData.m_PowerSeqState)
    {
    case DEVICE_POWERSEQ_STATE_UP_POST:
        // Call common powerup pre
        Status = DtaDevicePowerDownPre(pDvcData);
        if (!IgnoreErrors)
        {
            if (!DT_SUCCESS(Status))
            {
                DtaDevicePowerUpSeq(pDvcData);
                return -EPERM;
            }
            // Next state in power down sequence
            pDvcData->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_UP;
        }
        
    case DEVICE_POWERSEQ_STATE_UP:
        // Call common interrupts disable
        Status = DtaDeviceInterruptDisable(pDvcData);
        if (!IgnoreErrors)
        {
            if (!DT_SUCCESS(Status))
            {
                DtaDevicePowerUpSeq(pDvcData);
                return -EPERM;
            }
            // Next state in power down sequence
            pDvcData->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_INT_ENABLED;
        }
        
    case DEVICE_POWERSEQ_STATE_INT_ENABLED:
        // Call common powerdown
        Status = DtaDevicePowerDown(pDvcData);
        if (!IgnoreErrors)
        {
            if (!DT_SUCCESS(Status))
            {
                DtaDevicePowerUpSeq(pDvcData);
                return -EPERM;
            }
            // Next state in power down sequence
            pDvcData->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_DOWN;
        }
        
    case DEVICE_POWERSEQ_STATE_DOWN:
    default:
        // Nothing to do, fully powered down
    break;
    }

    if (IgnoreErrors)
    {
        // If errors are ignored, final state is allways down
        pDvcData->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_DOWN;
    }

    return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceStartSeq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtaDeviceStartSeq(DtaDeviceData* pDvcData)
{
    Int  Result;
    DtStatus  Status;
    dev_t  DevNum;
    
    switch (pDvcData->m_IalData.m_StartSeqState)
    {
    case DEVICE_STARTSEQ_STATE_IDLE:
        // Init Linux specific device resources
        Result = DtaInitDeviceResources(pDvcData);
        if (Result < 0)
        {
            DtaDeviceStopSeq(pDvcData);
            return Result;
        }
        
        // Next state in start sequence
        pDvcData->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_RES_INITIALIZED;
    
    case DEVICE_STARTSEQ_STATE_RES_INITIALIZED:
        // Call common init
        Status = DtaDeviceInit(pDvcData);
        if (!DT_SUCCESS(Status))
        {
            DtaDeviceStopSeq(pDvcData);
            return -EPERM;
        }
                
        // Next state in start sequence
        pDvcData->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_COMMON_INITIALIZED;
    
    case DEVICE_STARTSEQ_STATE_COMMON_INITIALIZED:
        // Create memory mappings and request IRQ
        Result = DtaMapResources(pDvcData);
        if (Result < 0)
        {
            DtaDeviceStopSeq(pDvcData);
            return Result;
        }
        
        // Next state in start sequence
        pDvcData->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_RES_MAPPED ;
    
    case DEVICE_STARTSEQ_STATE_RES_MAPPED:
        // Execute power sequence in common driver
        Result = DtaDevicePowerUpSeq(pDvcData);
        if (Result < 0)
        {
            DtaDeviceStopSeq(pDvcData);
            return Result;
        }
        
        // Next state in start sequence
        pDvcData->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_POWERED_UP;
    
    case DEVICE_STARTSEQ_STATE_POWERED_UP:
        // Merge Major / Minor to dev_t
        DevNum = MKDEV(g_DtaMajor, g_DtaMinor + pDvcData->m_IalData.m_DvcIndex);
        // Initialize cdev
        cdev_init(&pDvcData->m_Device.m_CDev, &DtaFileOps);
        // Add character device interface
        Result = cdev_add(&pDvcData->m_Device.m_CDev, DevNum, 1);
        if (Result < 0)
        {
            DtaDeviceStopSeq(pDvcData);
            return Result;
        }
        
        // Next state in start sequence
        pDvcData->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_CDEV_ADDED;
    
    case DEVICE_STARTSEQ_STATE_CDEV_ADDED:
        // Merge Major / Minor to dev_t
        DevNum = MKDEV(g_DtaMajor, g_DtaMinor + pDvcData->m_IalData.m_DvcIndex);
        // Create a device withing the Dta class
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
        pDvcData->m_Device.m_pDevice = device_create(g_pDtaClass, NULL, DevNum,
                                         DRIVER_NAME"%u", pDvcData->m_IalData.m_DvcIndex);
#else
        pDvcData->m_Device.m_pDevice = device_create(g_pDtaClass, NULL, DevNum, pDvcData,
                                         DRIVER_NAME"%u", pDvcData->m_IalData.m_DvcIndex);
#endif

        dev_set_drvdata(pDvcData->m_Device.m_pDevice, pDvcData);
        if (IS_ERR(pDvcData->m_Device.m_pDevice))
        {
            DtaDeviceStopSeq(pDvcData);
            return PTR_ERR(pDvcData->m_Device.m_pDevice);
        }
        
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
        // Create serial sysfs file
        device_create_file(pDvcData->m_Device.m_pDevice, &dev_attr_serial);
#endif
        
        // Next state in start sequence
        pDvcData->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_DEV_CREATED;
    
    case DEVICE_STARTSEQ_STATE_DEV_CREATED:
    default :
        // Start sequence is done
        break;
    }
    
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceStopSeq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void DtaDeviceStopSeq(DtaDeviceData* pDvcData)
{
    switch (pDvcData->m_IalData.m_StartSeqState)
    {
        case DEVICE_STARTSEQ_STATE_DEV_CREATED:
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
            // Remove serial sysfs file
            device_remove_file(pDvcData->m_Device.m_pDevice, &dev_attr_serial);
#endif
            // Destroy class device
            device_destroy(g_pDtaClass, pDvcData->m_Device.m_CDev.dev);
            
        case DEVICE_STARTSEQ_STATE_CDEV_ADDED:
            // Remove cdev object
            cdev_del(&pDvcData->m_Device.m_CDev);
            
        case DEVICE_STARTSEQ_STATE_POWERED_UP:
            // Execute powerdown sequence in common driver
            DtaDevicePowerDownSeq(pDvcData, TRUE);
            
        case DEVICE_STARTSEQ_STATE_RES_MAPPED:
            // Unmap resources
            DtaIalUnRegisterISR(pDvcData);
            DtaUnMapResources(pDvcData);
            
        case DEVICE_STARTSEQ_STATE_COMMON_INITIALIZED:
            // Execute DeviceExitPre in common driver
            DtaDeviceExitPre(pDvcData);
            // Execute DeviceExit in common driver
            DtaDeviceExit(pDvcData);
            
        case DEVICE_STARTSEQ_STATE_RES_INITIALIZED:
            // Free all Linux specific device resources
            DtaFreeDeviceResources(pDvcData);
            
        case DEVICE_STARTSEQ_STATE_IDLE:
        default:
            // Stop sequence is done
            break;
    }
    
    // After DeviceStopSeq all resources are free
    pDvcData->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_IDLE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMapRegsToUserspace -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtaMapRegsToUserSpace(
    DtaDeviceData*  pDvcData, 
    struct file*  pFile, 
    struct vm_area_struct*  pVma)
{
    UInt64  RegionStart;
    UInt  Size;
    DtUserMapping*  pUserMapping;
    Int  r;

    // Acquire mutex
    DtFastMutexAcquire(&pDvcData->m_UserMapMutex);

    // Check whether a mapping is available for this file object. If so, return an error
    pUserMapping = pDvcData->m_pUserMapping;
    while (pUserMapping != NULL)
    {
        if (pUserMapping->m_FileObject.m_pFile == pFile) 
        {
            DtDbgOut(ERR, IAL, "[%d] Error register already mapped into userspace");

            // Release memory map mutex
            DtFastMutexRelease(&pDvcData->m_UserMapMutex);
            return -EFAULT;
        }
        pUserMapping = pUserMapping->m_pNext;
    }
    
    pUserMapping = (DtUserMapping*)DtMemAllocPool(DtPoolPaged, sizeof(DtUserMapping), 
                                                                                 DTA_TAG);

    if (pUserMapping == NULL)
    {
        DtDbgOut(ERR, IAL, "Out of memory creating DtUserMapping struct");
        DtFastMutexRelease(&pDvcData->m_UserMapMutex);
        return -EFAULT;
    }

    // Insert at start of list
    pUserMapping->m_pPrev = NULL;
    pUserMapping->m_pNext = NULL;
    if (pDvcData->m_pUserMapping != NULL) 
    {
        pUserMapping->m_pNext = pDvcData->m_pUserMapping;
        pUserMapping->m_pNext->m_pPrev = pUserMapping;
    }
    pDvcData->m_pUserMapping = pUserMapping;

    // Be sure the memory is mapped non-cacheable
    pVma->vm_flags |= VM_IO;
    pVma->vm_page_prot = pgprot_noncached(pVma->vm_page_prot);
    
    RegionStart = pDvcData->m_DtaRegs.m_PciAddr.QuadPart;
    Size = pVma->vm_end - pVma->vm_start;
    
    if ((Size % PAGE_SIZE) != 0)
    { 
        Size = (Size / PAGE_SIZE) * PAGE_SIZE + PAGE_SIZE; 
    } 

    r = io_remap_pfn_range(pVma, pVma->vm_start, RegionStart >> PAGE_SHIFT, Size,
                                                                      pVma->vm_page_prot);
    if (r!=0) 
    {
        DtDbgOut(ERR, IAL, "io_remap_pfn_range FAILED");
        DtFastMutexRelease(&pDvcData->m_UserMapMutex);
        return -EFAULT;
    }

    // Store result in free element
    // Take the region startoffset into account
    pUserMapping->m_pUserVirtual = (void*)(pVma->vm_start + (RegionStart%PAGE_SIZE));
    pUserMapping->m_FileObject.m_pFile = pFile;
    pUserMapping->m_RefCount = 0;
    pUserMapping->m_pPaMmap = NULL;

    DtDbgOut(MIN, IAL, "pUserVirtual=0x%p, RefCount=%d", pUserMapping->m_pUserVirtual,
                                                                 pUserMapping->m_RefCount);

    // Release memory map mutex
    DtFastMutexRelease(&pDvcData->m_UserMapMutex);
    return 0;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaReleaseAddressRegsForFileObject -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaReleaseAddressRegsForFileObject(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile)
{
    DtUserMapping*  pUserMapping;
    
    // Memory should have been mapped in kernel space
    if (pDvcData->m_DtaRegs.m_pKernel == NULL) 
    {   DtDbgOut(ERR, IAL, "Registers not mapped");
        return DT_STATUS_NOT_INITIALISED;
    }
    // Acquire mutex
    DtFastMutexAcquire(&pDvcData->m_UserMapMutex);

    // Check whether a mapping is available for this file object
    pUserMapping = pDvcData->m_pUserMapping;
    while (pUserMapping != NULL)
    {
        if (pUserMapping->m_FileObject.m_pFile == pFile->m_pFile)
            break;
        pUserMapping = pUserMapping->m_pNext;
    }
    
    if (pUserMapping == NULL)
    {   DtFastMutexRelease(&pDvcData->m_UserMapMutex);
        return DT_STATUS_OK;
    }
    
    if (pUserMapping->m_RefCount == 0) 
    { 
        DtDbgOut(ERR, IAL, "Reference count already 0");
    }
    
    DtDbgOut(ERR, IAL, "Reference count=%i", pUserMapping->m_RefCount);
    
    // The application must Unmap the memory, but we can already remove the entry
    if (pUserMapping->m_pPrev != NULL)
        pUserMapping->m_pPrev->m_pNext = pUserMapping->m_pNext;
    else
        pDvcData->m_pUserMapping = pUserMapping->m_pNext;
    if (pUserMapping->m_pNext != NULL)
        pUserMapping->m_pNext->m_pPrev = pUserMapping->m_pPrev;
    DtMemFreePool(pUserMapping, DTA_TAG);
    // Release memory map mutex
    DtFastMutexRelease(&pDvcData->m_UserMapMutex);
    return DT_STATUS_OK;
}

// TODO: Move following 2 funcs to SAL???

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaGetAddressRegsForUserspace -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaGetAddressRegsForUserspace(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    Int  PortIndex,
    void*  pPaMmap,
    Int*  pSize,                 // Size of register block
    void**  ppUserVirtual)
{
    DtUserMapping*  pUserMapping;
    
    // Memory should have been mapped in kernel space
    if (pDvcData->m_DtaRegs.m_pKernel == NULL) 
    {
        DtDbgOut(ERR, IAL, "[%d] Registers not mapped", PortIndex);
        return DT_STATUS_NOT_INITIALISED;
    }
    // Acquire mutex
    DtFastMutexAcquire(&pDvcData->m_UserMapMutex);

    // Check whether a mapping is available for this file object
    pUserMapping = pDvcData->m_pUserMapping;
    while (pUserMapping != NULL)
    {
        if (pUserMapping->m_FileObject.m_pFile == pFile->m_pFile)
            break;
        pUserMapping = pUserMapping->m_pNext;
    }
    if (pUserMapping != NULL)
    {
        pUserMapping->m_RefCount++;
        *ppUserVirtual = pUserMapping->m_pUserVirtual;
        *pSize = pDvcData->m_DtaRegs.m_Length;
        if (pPaMmap != NULL) 
            pUserMapping->m_pPaMmap = pPaMmap;
        
        DtDbgOut(MIN, IAL, "[%d] pUserVirtual=0x%p, Count=%d", PortIndex, *ppUserVirtual,
                                                                pUserMapping->m_RefCount);
        DtFastMutexRelease(&pDvcData->m_UserMapMutex);
        return DT_STATUS_OK;
    }
    
    // Application must first excecute Mmap.
    
    // Set the size of the register block for Mmap.
    *pSize = pDvcData->m_DtaRegs.m_Length;

    // Release the mutex
    DtFastMutexRelease(&pDvcData->m_UserMapMutex);

    return DT_STATUS_NOT_INITIALISED;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaReleaseAddressRegsForUserspace -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaReleaseAddressRegsForUserspace(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    Int  PortIndex,
    void**  ppPaMap,
    Int*  pSize)
{
    DtUserMapping*  pUserMapping;
    
    // Memory should have been mapped in kernel space
    if (pDvcData->m_DtaRegs.m_pKernel == NULL) 
    {   DtDbgOut(ERR, IAL, "[%d] Registers not mapped", PortIndex);
        return DT_STATUS_NOT_INITIALISED;
    }
    // Acquire mutex
    DtFastMutexAcquire(&pDvcData->m_UserMapMutex);

    // Check whether a mapping is available for this file object
    pUserMapping = pDvcData->m_pUserMapping;
    while (pUserMapping != NULL)
    {
        if (pUserMapping->m_FileObject.m_pFile == pFile->m_pFile)
            break;
        pUserMapping = pUserMapping->m_pNext;
    }
    
    if (pUserMapping == NULL)
    {   DtDbgOut(MIN, IAL, "[%d] NO MAPPING FOUND", PortIndex);
        DtFastMutexRelease(&pDvcData->m_UserMapMutex);
        return DT_STATUS_NOT_FOUND;
    }
    
    if (pUserMapping->m_RefCount > 1) 
    { 
        pUserMapping->m_RefCount--;
        DtDbgOut(MIN, IAL, "[%d] Reference count=%i", PortIndex, 
                                                                pUserMapping->m_RefCount);
        DtFastMutexRelease(&pDvcData->m_UserMapMutex);
        return DT_STATUS_IN_USE;
    }

    if (pUserMapping->m_RefCount == 0)
    {
        DT_ASSERT(FALSE);
        DtDbgOut(ERR, IAL, "[%d] Reference count already 0", PortIndex);
    }

    DtDbgOut(MIN, IAL, "[%d] Reference count=0", PortIndex);
    
    *ppPaMap = pUserMapping->m_pPaMmap;
    *pSize = pDvcData->m_DtaRegs.m_Length;

    // The application must Unmap the memory, but we can already remove the entry
    if (pUserMapping->m_pPrev != NULL)
        pUserMapping->m_pPrev->m_pNext = pUserMapping->m_pNext;
    else
        pDvcData->m_pUserMapping = pUserMapping->m_pNext;
    if (pUserMapping->m_pNext != NULL)
        pUserMapping->m_pNext->m_pPrev = pUserMapping->m_pPrev;
    DtMemFreePool(pUserMapping, DTA_TAG);

    // Release memory map mutex
    DtFastMutexRelease(&pDvcData->m_UserMapMutex);
    return DT_STATUS_OK;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Interrupt handler +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaInterrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static irqreturn_t  DtaInterrupt(Int Irq, void* pContext)
{
    DtaDeviceData*  pDvcData = (DtaDeviceData*)pContext;
    if (pDvcData == NULL)
        return IRQ_NONE;
    
    if (DtaDeviceInterrupt(pDvcData))
        return IRQ_HANDLED;
    else
        return IRQ_NONE;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Character interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#ifdef CONFIG_COMPAT
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoctlCompat -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Compatibility function for 32-bit applications using a 64-bit driver
//
long DtaIoctlCompat(
    struct file*  pFile, 
    unsigned int  Cmd, 
    unsigned long  Arg)
{

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
    struct inode*  pInode = pFile->f_dentry->d_inode;
#else
    struct inode*  pInode = pFile->f_path.dentry->d_inode;
#endif
    DtDbgOut(MAX, IAL, "Start");
    
    // Just call the regular IOCTL handler; it should be 32-/64-bit clean
    DtDbgOut(MAX, IAL, "Exit");
    return DtaIoctl(pInode, pFile, Cmd, (unsigned long)compat_ptr(Arg));
}
#endif  // #ifdef CONFIG_COMPAT

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaUnlockedIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static long  DtaUnlockedIoctl(struct file* pFile, unsigned int Cmd, unsigned long Arg)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
    struct inode*  pInode = pFile->f_dentry->d_inode;
#else
    struct inode*  pInode = pFile->f_path.dentry->d_inode;
#endif
    return DtaIoctl(pInode, pFile, Cmd, Arg);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static long  DtaIoctl(
    struct inode*  pInode,
    struct file*  pFile,
    unsigned int  Cmd,
    unsigned long  Arg)
{
    DtStatus  Status;
    Int  Result = 0;
    DtFileObject  File;
    DtIoctlObject  Ioctl;
    DtaIoctlInputData  IoctlInputData; 
    DtaIoctlOutputData  IoctlOutputData; 

    char*  pIoctlInputData = NULL;
    char*  pIoctlOutputData = NULL;
    
    UInt  InputSizeParam = 0;
    UInt  OutputSizeParam = 0;
    UInt  ReservedForSizeParam = 0;

    DtDbgOut(MAX, IAL, "Start");

    // Check command that this is one of our commands
    if (DTA_IOCTL_MAGIC!=_IOC_TYPE(Cmd) && DTA_IOCTL_MAGIC_SIZE!=_IOC_TYPE(Cmd))
        Result = -ENOTTY;

    if (Result >= 0)
    {
        // Check device context
        if (pFile->private_data == NULL)
            Result = -EFAULT;
    }

    if (Result >= 0)
    {
        if(_IOC_TYPE(Cmd) == DTA_IOCTL_MAGIC_SIZE)
        {
            UInt*  pBufSizeLoc = (UInt*)Arg;

            ReservedForSizeParam = 2*sizeof(UInt);

            if (access_ok(VERIFY_READ, (void*)Arg, ReservedForSizeParam) == 0)
                Result = -EFAULT;

            if (Result >= 0)
            {   
                Result = get_user(InputSizeParam, pBufSizeLoc);
                pBufSizeLoc++;
                if (Result >=0)
                    Result = get_user(OutputSizeParam, pBufSizeLoc);
                
                DtDbgOut(MAX, IAL, "I/O buffer has size parameter"
                                                        "(Input size:%d, Output size:%d)",
                                                        InputSizeParam, OutputSizeParam);
            }
        } else {
            InputSizeParam = _IOC_SIZE(Cmd);
            OutputSizeParam = _IOC_SIZE(Cmd);
        }
    }

    if (Result >= 0)
    {
        // Init file object
        File.m_pFile = pFile;

        // Init Ioctl object
        Ioctl.m_IoctlCode = Cmd;
        Ioctl.m_pOutputBuffer = NULL;
        Ioctl.m_OutputBufferSize = 0;
        Ioctl.m_pInputBuffer = NULL;
        Ioctl.m_InputBufferSize = 0;

        // Check memory and get buffer sizes
        if ((_IOC_DIR(Cmd)&_IOC_READ) != 0)
        {
            // Set output buffer size
            Ioctl.m_OutputBufferSize = OutputSizeParam;
            if (_IOC_TYPE(Cmd)==DTA_IOCTL_MAGIC && Ioctl.m_OutputBufferSize>sizeof(IoctlOutputData))
                Ioctl.m_OutputBufferSize = sizeof(IoctlOutputData);

            DtDbgOut(MAX, IAL, "Output buffer size %d", Ioctl.m_OutputBufferSize);

            // Ioctl reads --> driver needs write access for user memory
            if (access_ok(VERIFY_WRITE, (void*)Arg, Ioctl.m_OutputBufferSize) == 0)
                Result = -EFAULT;

            // Allocate memory for the output data
            if (ReservedForSizeParam != 0)
            {
                pIoctlOutputData = kmalloc(Ioctl.m_OutputBufferSize, GFP_KERNEL);
                Ioctl.m_pOutputBuffer = pIoctlOutputData;
                if (pIoctlOutputData == NULL)
                    Result = -EFAULT;
            } else
                Ioctl.m_pOutputBuffer = &IoctlOutputData;
        }

        if ((_IOC_DIR(Cmd)&_IOC_WRITE) != 0)
        {
            // Set input buffer size
            Ioctl.m_InputBufferSize = InputSizeParam;

            if (_IOC_TYPE(Cmd)==DTA_IOCTL_MAGIC && Ioctl.m_InputBufferSize>sizeof(DtaIoctlInputData))
                Ioctl.m_InputBufferSize = sizeof(DtaIoctlInputData);
            
            DtDbgOut(MAX, IAL, "Input buffer size %d", Ioctl.m_InputBufferSize);
            
            // Ioctl writes --> driver needs read access for user memory
            if (access_ok(VERIFY_READ, (void*)Arg, Ioctl.m_InputBufferSize+ReservedForSizeParam) == 0)
                Result = -EFAULT;
            
            // Allocate memory for the input data
            if (ReservedForSizeParam != 0)
            {
                pIoctlInputData = kmalloc(Ioctl.m_InputBufferSize, GFP_KERNEL);
                Ioctl.m_pInputBuffer = pIoctlInputData;
                if (pIoctlInputData == NULL)
                    Result = -EFAULT;
            } else
                Ioctl.m_pInputBuffer = &IoctlInputData;
        }
    }

    if (Result >= 0)
    {
        if ((_IOC_DIR(Cmd)&_IOC_WRITE) != 0)
        {
            // Get user space data
            DtDbgOut(MAX, IAL, "Copy from user (size %d)", Ioctl.m_InputBufferSize);
            
            if (Ioctl.m_InputBufferSize>0 && Ioctl.m_pInputBuffer!=NULL)
            {
                UInt8*  pArg = (UInt8*)Arg;
                // Skip the size counters, we already have them
                pArg += ReservedForSizeParam;

                if (copy_from_user(Ioctl.m_pInputBuffer, (void*)pArg, Ioctl.m_InputBufferSize) != 0)
                    Result = -EFAULT;
            }
        }
    }

    if (Result >= 0)
    {
        // Call common IOCTRL
        Status = DtaDeviceIoctl(pFile->private_data, &File, &Ioctl);
        
        if (DT_SUCCESS(Status))
        {
            // Copy data back to user space if there was any data.
            // Common driver may have written a smaller value to m_OutputBufferSize to
            // speed up the copy_to_user, or 0 in case of an error/no data
            if (Ioctl.m_OutputBufferBytesWritten > 0)
            {
                if ((_IOC_DIR(Cmd)&_IOC_READ) != 0)
                {
                    DtDbgOut(MAX, IAL, "Copy to user (size %d)",
                                                        Ioctl.m_OutputBufferBytesWritten);
                    if (copy_to_user((void*)Arg, Ioctl.m_pOutputBuffer,
                                                   Ioctl.m_OutputBufferBytesWritten) != 0)
                        Status = DT_STATUS_FAIL;
                }
            }
        } else
            Result = -Status;
    }

    if (pIoctlInputData != NULL)
        kfree(pIoctlInputData);
    if (pIoctlOutputData != NULL)
        kfree(pIoctlOutputData);


    DtDbgOut(MAX, IAL, "Exit");

    return Result;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaOpen -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtaOpen(struct inode* pInode, struct file* pFile)
{
    DtStatus  Status;
    DtFileObject  File;
    Int  Result = 0;
    Int  Index;

    DtDbgOut(MAX, IAL, "Start");

    // Get the device index that is opened (Use minor number without offset)
    Index = MINOR(pInode->i_rdev) - g_DtaMinor;

    // Init File object
    File.m_pFile = pFile;

    // Store device data device data
    pFile->private_data = g_pDtaDeviceData[Index];

    // Call common open
    Status = DtaDeviceOpen(pFile->private_data, &File);

    // Convert result
    if (!DT_SUCCESS(Status))
        Result = -EFAULT;
    
    DtDbgOut(MAX, IAL, "Exit");
    
    return Result;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtaClose(struct inode* pInode, struct file* pFile)
{
    DtFileObject  File;

    DtDbgOut(MAX, IAL, "Start");

    // Check device context
    if (pFile->private_data == NULL)
        return -EFAULT;

    // Init File object
    File.m_pFile = pFile;

    // Release mapping to userspace
    DtaReleaseAddressRegsForFileObject(pFile->private_data, &File);

    // Release Non IP channel specific resources
    DtaNonIpReleaseResourceFromFileObject(pFile->private_data, &File);

    // Release Ip channel specific resources
    DtaIpReleaseResourceFromFileObject(pFile->private_data, &File);

    // Call common close
    DtaDeviceClose(pFile->private_data, &File);

    // Check if AdminStatus must be set up
    DtaIpCheckReleaseAdminStatus(pFile->private_data);

    // clear private_data
    pFile->private_data = NULL;
    
    DtDbgOut(MAX, IAL, "Exit");
    
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMmap -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Map PCI device memory (register map) to userspace
//
// NOTE: PCI device memory might not be allocated on a page-aligned address, the BIOS does
//       not look into that. 'io_remap_pfn_range' can only map page-aligned addresses
//       We use DTA_IOCTL_GET_ADDR_OPER_REGS to get the actual register address 
//       base pointers
//
static Int  DtaMmap(struct file* pFile, struct vm_area_struct* pVma)
{
    UInt64  RegionStart = 0;
    UInt  RegionSize = 0;
    Int  Result = 0;
    DtaDeviceData*  pDvcData = (DtaDeviceData*)pFile->private_data;

    DtDbgOut(MAX, IAL, "Start");

    // Get the start and size of the operation register region
    RegionStart = pDvcData->m_DtaRegs.m_PciAddr.QuadPart;
    RegionSize =  pDvcData->m_DtaRegs.m_Length;

    // We only permit at least the same register block size, just to be safe
    if (pVma->vm_end-pVma->vm_start < RegionSize) 
    {
        DtDbgOut(MIN, IAL, "Incorrect mapping length %u. Should be at least %u", 
                                         (UInt)(pVma->vm_end-pVma->vm_start), RegionSize);
        DtDbgOut(MAX, IAL, "Exit");
        return -EFAULT;
    }
    // 
    Result = DtaMapRegsToUserSpace(pDvcData, pFile, pVma);
    DtDbgOut(MAX, IAL, "Exit");

    return Result;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaPoll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static UInt  DtaPoll(struct file* pFile, poll_table* pPollTable)
{
    DtFileObject  File;
    UInt  Mask = 0;
    DtaEvents*  pDtaEvents = NULL;
    Int  NumPending = 0;
    DtaDeviceData*  pDvcData = (DtaDeviceData*)pFile->private_data;

    DtDbgOut(MAX, IAL, "Start");

    // Init File object
    File.m_pFile = pFile;

    DtSpinLockAcquire(&pDvcData->m_EventsSpinlock);
        
    // Add events wait_queue to poll table and get number of pending events
    pDtaEvents = pDvcData->m_pEvents;
    while (pDtaEvents != NULL)
    {
        if (DtFileCompare(&pDtaEvents->m_File, &File))
        {
            poll_wait(pFile, &pDtaEvents->m_PendingEvent.m_WaitQueueHead, pPollTable);
            NumPending = pDtaEvents->m_NumPendingEvents;
            break;
        }
        pDtaEvents = pDtaEvents->m_pNext;
    }
    
    // Check if events are pending
    if (NumPending > 0)
    {
        DtDbgOut(MAX, IAL, "Pending Events found.");
        Mask |= POLLIN | POLLRDNORM;
    } else
        DtDbgOut(MAX, IAL, "No pending Events found.");

    DtSpinLockRelease(&pDvcData->m_EventsSpinlock);
    DtDbgOut(MAX, IAL, "Exit");

    return Mask;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static ssize_t  DtaRead(struct file* pFile, char __user* pBuf, size_t Count, loff_t* pOffp)
{
    // DtaEventGet
    return 0;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ System interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaProbe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtaProbe(struct pci_dev *pPciDev, const struct pci_device_id *pId)
{
    Int  Result = 0;
    UInt  DvcIndex;
    DtaDeviceData*  pDvcData = NULL;
    
    DtDbgOut(MAX, IAL, "Start");
    
    // Quick check if maximum device count is reached
    if (g_DtaDeviceCount < g_DtaMaxDevices)
    {
        // Find free DvcIndex
        for (DvcIndex=0; DvcIndex<g_DtaMaxDevices; DvcIndex++)
        {
            if (g_pDtaDeviceData[DvcIndex] == NULL)
                break;
        }
        if (DvcIndex == g_DtaMaxDevices)
            Result = -ENOMEM;
    } else
        Result = -ENOMEM;
    
    // Only proceed if no errors
    if (Result >= 0)
    {
        // Allocate memory for device context
        pDvcData = kmalloc(sizeof(DtaDeviceData), GFP_KERNEL);
        if (pDvcData != NULL)
        {
            // Initialize device data to default values
            DtaInitDeviceData(pDvcData);
            
            // Store handle to pDvcData in array
            g_pDtaDeviceData[DvcIndex] = pDvcData;

            // Store DvcIndex in device data IAL data
            pDvcData->m_IalData.m_DvcIndex = DvcIndex;
            pDvcData->m_IalData.m_ISRRegistered = FALSE;
        } else
            Result = -ENOMEM;
    }
    
    // Only proceed if no errors
    if (Result >= 0)
    {
        // Store pointer to this module and pci_dev in device data
        pDvcData->m_Driver.m_pModule = THIS_MODULE;
        DtEvtLogInit(&pDvcData->m_Driver.m_EvtObject, &EventGetMessage);
        pDvcData->m_Device.m_pPciDev = pPciDev;
        DtEvtLogInit(&pDvcData->m_Device.m_EvtObject, &EventGetMessage);
    }
    
    // Only proceed if no errors
    if (Result >= 0)
        // Start DTA card
        Result = DtaDeviceStartSeq(pDvcData);
    
    // Only proceed if no errors
    if (Result >= 0)
    {
        // Set device driver data
        pci_set_drvdata(pDvcData->m_Device.m_pPciDev, pDvcData);
    
        // Finnaly increment device count
        g_DtaDeviceCount++;
        
        DtDbgOut(AVG, IAL, "New device count: %u", g_DtaDeviceCount);
    }

    // Cleanup if errors
    if (Result < 0)
    {
        if (pDvcData != NULL)
        {
            // Free minor by clearing array entry
            g_pDtaDeviceData[pDvcData->m_IalData.m_DvcIndex] = NULL;
            
            // Free device data
            kfree(pDvcData);
        }
    }
    
    DtDbgOut(MAX, IAL, "Exit");
    
    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaRemove -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtaRemove(struct pci_dev *pPciDev)
{
    DtaDeviceData*  pDvcData;
    
    DtDbgOut(MAX, IAL, "Start");

    // Get device data
    pDvcData = pci_get_drvdata(pPciDev);
    if (pDvcData == NULL)
        return;
    
    // Clear device driver data
    pci_set_drvdata(pDvcData->m_Device.m_pPciDev, NULL);

    // Stop DTA card
    DtaDeviceStopSeq(pDvcData);
    
    // Free minor by clearing array entry
    g_pDtaDeviceData[pDvcData->m_IalData.m_DvcIndex] = NULL;
    
    // Free device data
    kfree(pDvcData);
    
    // Decrement device count
    g_DtaDeviceCount--;
    
    DtDbgOut(AVG, IAL, "New device count: %u", g_DtaDeviceCount);
    
    DtDbgOut(MAX, IAL, "Exit");
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSuspend -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtaSuspend(struct pci_dev *pPciDev, pm_message_t State)
{
    Int  Result = 0;
    DtaDeviceData*  pDvcData;
    
    DtDbgOut(MAX, IAL, "Start");

    // Get device data
    pDvcData = pci_get_drvdata(pPciDev);
    if (pDvcData == NULL)
        return -ENODEV;

    // Save the PCI configuration space
    pci_save_state(pPciDev);

    // Execute powerdown sequence in common driver
    Result = DtaDevicePowerDownSeq(pDvcData, FALSE);
    if (Result == 0)
    {
        // Unmap the device resources
        DtaIalUnRegisterISR(pDvcData);
        DtaUnMapResources(pDvcData);
    }
    
    DtDbgOut(MAX, IAL, "Exit");

    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaResume -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaResume(struct pci_dev *pPciDev)
{
    Int  Result = 0;
    DtaDeviceData* pDvcData;
    
    DtDbgOut(MAX, IAL, "Start");

    // Get device data
    pDvcData = pci_get_drvdata(pPciDev);
    if (pDvcData == NULL)
        return -ENODEV;

    // Make sure property not found counter is reset after a power resume
    DtResetPropertiesNotFoundCounter(&pDvcData->m_PropData);

    // Restore the PCI configuration space
    pci_restore_state(pPciDev);
    
    // Map the device resources
    Result = DtaMapResources(pDvcData);
    if (Result == 0)
    {
        // Execute powerup sequence in common driver
        Result = DtaDevicePowerUpSeq(pDvcData);
        if (Result < 0)
        {
            // Unmap the device resources
            DtaIalUnRegisterISR(pDvcData);
            DtaUnMapResources(pDvcData);
        }
    }
    
    DtDbgOut(MAX, IAL, "Exit");
    
    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDoProbeChildDevices -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Helper function to initialize and execute the Probe function on the child device
//
void  DtDoProbeChildDevices(DtaDeviceData* pDvcData)
{
    Int  i;
    for (i=0; i<pDvcData->m_NumIpPorts; i++)
    {
        DtaChildDeviceData*  pChildDvcData = 
                                       pDvcData->m_IpDevice.m_pIpPorts[i].m_pChildDvcData;
        if (pChildDvcData->m_IalData.m_DeviceItf.m_pDriverItf == NULL)
        {
            pChildDvcData->m_IalData.m_DeviceItf.m_pParentDevice = 
                                                             pDvcData->m_Device.m_pDevice;
            pChildDvcData->m_IalData.m_DeviceItf.m_pDriverItf = 
                                                            g_DtaChildDriver.m_pDriverItf;
            pChildDvcData->m_IalData.m_DeviceItf.m_pDriverItf->m_pContext = pChildDvcData;
            pChildDvcData->m_IalData.m_DeviceItf.m_pDriverItf->IoCtrl = DtaIoctlChild;
            g_DtaChildDriver.m_pDriverItf->Probe(&pChildDvcData->m_IalData.m_DeviceItf, 
                                                                                      i+1);
        }
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaInitChildDevices -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function will report a new bus to the OS and adds new child devices to the 
// OS/Network driver
//
DtStatus  DtaInitChildDevices(
    DtaDeviceData*  pDvcData)
{
    Int  i;
    
    DtDbgOut(MAX, IAL, "Start");

    //DtSpinLockAcquire(&g_DtaChildDriver.m_SpinLock);

    // Allocate storage for ChildDevice data
    for (i=0; i<pDvcData->m_NumIpPorts; i++)
    {
        DtaChildDeviceData*  pChildDvcData;
        pChildDvcData = kmalloc(sizeof(DtaChildDeviceData), GFP_KERNEL);
        if (pChildDvcData == NULL)
            return DT_STATUS_OUT_OF_MEMORY;

        // Initialise child device data structure
        memset(pChildDvcData, 0, sizeof(DtaChildDeviceData));
        pDvcData->m_IpDevice.m_pIpPorts[i].m_pChildDvcData = pChildDvcData;
        pChildDvcData->m_pParentDeviceData = pDvcData;
        pChildDvcData->m_Index = i;
    }
   
    // Check if the network driver is registered by the bus driver. If not, skip the probe
    // function
    if (g_DtaChildDriver.m_pDriverItf == NULL)
    {
        DtDbgOut(AVG, IAL, "No network driver registered."
                                                 " We can not initiate the enumeration.");
        //DtSpinLockRelease(&g_DtaChildDriver.m_SpinLock);
    
        return DT_STATUS_OK;
    }
        
    DtDoProbeChildDevices(pDvcData);
    
    //DtSpinLockRelease(&g_DtaChildDriver.m_SpinLock);
    DtDbgOut(MAX, IAL, "Exit");
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaRemoveChildDevices -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaRemoveChildDevices(
    DtaDeviceData*  pDvcData)
{
    Int  i;

    DtDbgOut(MAX, IAL, "Start");
    
    //DtSpinLockAcquire(&g_DtaChildDriver.m_SpinLock);
    if (g_DtaChildDriver.m_pDriverItf == NULL)
        DtDbgOut(AVG, IAL, "No network driver registered. Remove not needed");
        
    for (i=0; i<pDvcData->m_NumIpPorts; i++)
    {
        DtaChildDeviceData*  pChildDvcData = 
                                       pDvcData->m_IpDevice.m_pIpPorts[i].m_pChildDvcData;
        if (g_DtaChildDriver.m_pDriverItf != NULL)
            // Report the remove to the Network driver
            g_DtaChildDriver.m_pDriverItf->Remove(&pChildDvcData->m_IalData.m_DeviceItf,
                                                                                     i+1);
        
        kfree(pChildDvcData);
        pDvcData->m_IpDevice.m_pIpPorts[i].m_pChildDvcData = NULL;
    }
    //DtSpinLockRelease(&g_DtaChildDriver.m_SpinLock);

    DtDbgOut(MAX, IAL, "Exit");
    
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoctlChild -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaIoctlChild(
    UInt32  IoctlCode, 
    UInt  InputBufferSize, 
    void*  pInputBuffer,
    UInt  OutputBufferSize, 
    void*  pOutputBuffer,
    UInt*  pOutputBufferBytesWritten,
    void*  pContext)
{
    DtStatus  Status = DT_STATUS_OK;
    DtFileObject  File;
    DtIoctlObject  Ioctl;

    DtDbgOut(MAX, IAL, "Start");

    // Check command that this is one of our commands
    if (DTA_IOCTL_MAGIC != _IOC_TYPE(IoctlCode))
        return DT_STATUS_INVALID_PARAMETER;

   // Check device context
    if (pContext == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Init file object
    File.m_pFile = NULL;

    // Init Ioctl object
    Ioctl.m_IoctlCode = IoctlCode;
    Ioctl.m_pOutputBuffer = pOutputBuffer;
    Ioctl.m_OutputBufferSize = OutputBufferSize;
    Ioctl.m_pInputBuffer = pInputBuffer;
    Ioctl.m_InputBufferSize = InputBufferSize;

    // Call common IOCTL
    Status = DtaDeviceIoctlChild(pContext, &File, &Ioctl);
    
    if (pOutputBufferBytesWritten != NULL)
        *pOutputBufferBytesWritten = Ioctl.m_OutputBufferBytesWritten;
    
    DtDbgOut(MAX, IAL, "Exit");
    
    return Status;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+ Child devices register/unregister +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DektecRegisterChildDriver -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DektecRegisterChildDriver(DtaDriverItf* pDriverItf)
{
    Int  DvcIndex;
    
    DtDbgOut(MAX, IAL, "Start");
    if (g_DtaChildDriver.m_pDriverItf != NULL)
        return;

    //DtSpinLockAcquire(&g_DtaChildDriver.m_SpinLock);
    
    g_DtaChildDriver.m_pDriverItf = pDriverItf;

    for (DvcIndex=0; DvcIndex<g_DtaMaxDevices; DvcIndex++)
    {
        if (g_pDtaDeviceData[DvcIndex] != NULL)
            DtDoProbeChildDevices(g_pDtaDeviceData[DvcIndex]);
    }
    
    //DtSpinLockRelease(&g_DtaChildDriver.m_SpinLock);
    
    DtDbgOut(MAX, IAL, "Exit");
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DektecUnRegisterChildDriver -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DektecUnRegisterChildDriver(DtaDriverItf* pDriverItf)
{

    Int  DvcIndex;
    Int  i;
    DtaDeviceData*  pDvcData = NULL;
    
    DtDbgOut(MAX, IAL, "Start");
    
    //DtSpinLockAcquire(&g_DtaChildDriver.m_SpinLock);

    g_DtaChildDriver.m_pDriverItf = NULL;

    for (DvcIndex=0; DvcIndex<g_DtaMaxDevices; DvcIndex++)
    {
        if (g_pDtaDeviceData[DvcIndex] != NULL)
        {
            pDvcData = g_pDtaDeviceData[DvcIndex];
            for (i=0; i<pDvcData->m_NumIpPorts; i++)
            {
                DtaChildDeviceData*  pChildDvcData = 
                                       pDvcData->m_IpDevice.m_pIpPorts[i].m_pChildDvcData;
                if (pChildDvcData->m_IalData.m_DeviceItf.m_pDriverItf == pDriverItf)
                    pChildDvcData->m_IalData.m_DeviceItf.m_pDriverItf = NULL;
            }
        }
    }

    //DtSpinLockRelease(&g_DtaChildDriver.m_SpinLock);

    DtDbgOut(MAX, IAL, "Exit");
}

EXPORT_SYMBOL(DektecRegisterChildDriver);
EXPORT_SYMBOL(DektecUnRegisterChildDriver);


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Module init / exit +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaModuleInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtaModuleInit(void)
{
    Int  Result = 0;
    dev_t  Dev = 0;
    DtStatus  Status;
    DtEvtLog  EvtObject;
    
    DtDbgOut(MAX, IAL, "Start");

    // Get major device number
    if (g_DtaMajor != 0) 
    {
        Dev = MKDEV(g_DtaMajor, g_DtaMinor);
        Result = register_chrdev_region(Dev, g_DtaMaxDevices, DRIVER_NAME);
    } else {
        Result = alloc_chrdev_region(&Dev, g_DtaMinor, g_DtaMaxDevices, DRIVER_NAME);
        g_DtaMajor = MAJOR(Dev);
    }
    if (Result < 0)
        DtDbgOut(ERR, IAL, "Can't get major %d", g_DtaMajor);

    // Only proceed if no errors
    if (Result >= 0)
    {
        // Allocate memory for device context pointers
        g_pDtaDeviceData = kmalloc(g_DtaMaxDevices * sizeof(DtaDeviceData*), GFP_KERNEL);
        if (g_pDtaDeviceData != NULL)
            memset(g_pDtaDeviceData, 0, g_DtaMaxDevices * sizeof(DtaDeviceData*));
        else
            Result = -EBUSY;
    }

    // Only proceed if no errors
    if (Result >= 0)
    {
        // Create Device class
        g_pDtaClass = class_create(THIS_MODULE, DRIVER_NAME);
        if (IS_ERR(g_pDtaClass))
            Result = PTR_ERR(g_pDtaClass);
        else {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
            // Add device class attributes
            g_pDtaClass->dev_attrs = DtaAttributes;
#endif
        }
    }
    
    // Only proceed if no errors
    if (Result >= 0)
    {
        // Call init in common driver
        DtEvtLogInit(&EvtObject, &EventGetMessage);
        Status = DtaDriverInit(&EvtObject);
        if (!DT_SUCCESS(Status))
            Result = -EBUSY;
    }
    
    // Only proceed if no errors
    if (Result >= 0)
        // Register our system interface in the PCI driver
        // DtaProbe is called from this function for all devices in the system
        Result = pci_register_driver(&DtaPciOps_ops);
    
    // Cleanup if errors
    if (Result < 0)
        DtaModuleExit();

    // Initialise the child driver interface
    //DtSpinLockInit(&g_DtaChildDriver.m_SpinLock);
    g_DtaChildDriver.m_pDriverItf = NULL;
    
    DtDbgOut(MAX, IAL, "Exit");
    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaModuleExit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtaModuleExit(void)
{
    dev_t  DevNum = 0;
        
    DtDbgOut(MAX, IAL, "Start");

    // Unregister our system interface in the PCI driver
    pci_unregister_driver(&DtaPciOps_ops);
    
    if (g_DtaDeviceCount != 0)
        DtDbgOut(ERR, IAL, "DtaModuleExit called, but DtaDeviceCount is not 0 after"
                                           " pci_unregister_driver (%u)", g_DtaDeviceCount);

    // Call exit in common driver
    DtaDriverExit();

    // Destroy device class
    if (!IS_ERR(g_pDtaClass))
        class_destroy(g_pDtaClass);

    // Free memory for device context pointers
    if (g_pDtaDeviceData != NULL)
        kfree(g_pDtaDeviceData);

    // Unregister character device region
    DevNum = MKDEV(g_DtaMajor, g_DtaMinor);
    unregister_chrdev_region(DevNum, g_DtaMaxDevices);
    
    DtDbgOut(MAX, IAL, "Exit");
}

module_init(DtaModuleInit);
module_exit(DtaModuleExit);




