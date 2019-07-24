// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCorePcie_IAL.c *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018 DekTec Digital Video B.V.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtPcieIncludes.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Compile time defaults
#ifndef DTPCIE_MAX_DEVICES
#define  DTPCIE_MAX_DEVICES 10
#endif
#ifndef DTPCIE_MAJOR
#define  DTPCIE_MAJOR 0   /* 0 --> Dynamic major by default */
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Global variables
Int  g_DtPcieMaxDevices = DTPCIE_MAX_DEVICES;
Int  g_DtPcieMajor = DTPCIE_MAJOR;
Int  g_DtPcieMinor = 0;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Global variables -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Load time parameters
module_param(g_DtPcieMaxDevices, int, S_IRUGO);  // Do not change int to Int!
module_param(g_DtPcieMajor, int, S_IRUGO);       // Do not change int to Int!

// Supported devices table
extern DEFINE_PCI_DEVICE_TABLE(DtaDeviceTable);

MODULE_AUTHOR("DekTec Digital Video B.V.");
MODULE_DESCRIPTION("DtCorePcie_IAL_ Series Driver");

// ADDED TO PREVENT TAINTED MESSAGE. THIS MODULE CAN BE USED FREELY (SEE LICENSE
// STATEMENT ABOVE).
MODULE_LICENSE("GPL";)

// Global variables
static DtCorePcie**  g_pDtCorePcie = NULL;
static struct class*  g_pDtPcieClass = NULL;
static Int  g_DtPcieDeviceCount = 0;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Forward declarations +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Private helper functions
static Int  DtCorePcie_IAL_DevicePowerUpSeq(DtCorePcie* pCore);
static Int  DtCorePcie_IAL_DevicePowerDownSeq(DtCorePcie* pCore, Bool IgnoreErrors);
static Int  DtCorePcie_IAL_DeviceStartSeq(DtCorePcie* pCore);
static void  DtCorePcie_IAL_DeviceStopSeq(DtCorePcie* pCore);

static void  DtCorePcie_IAL_InitDeviceData(DtCorePcie* pCore);
static Int  DtCorePcie_IAL_InitDeviceResources(DtCorePcie* pCore);
static Int  DtCorePcie_IAL_FreeDeviceResources(DtCorePcie* pCore);
static Int  DtCorePcie_IAL_MapResources(DtCorePcie* pCore);
static void  DtCorePcie_IAL_UnMapResources(DtCorePcie* pCore);

// Interrupt handler
static irqreturn_t  DtCorePcie_IAL_Interrupt(Int Irq, void* pContext);

// Character device interface
static int  DtCorePcie_IAL_Ioctl(struct inode* pInode, struct file* pFile, unsigned int Cmd, 
                                                                       unsigned long Arg);
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
static long  DtCorePcie_IAL_UnlockedIoctl(struct file* pFile, unsigned int Cmd, unsigned long Arg);
#endif
#ifdef CONFIG_COMPAT
// 32-bit applications using 64-bit driver
static long  DtCorePcie_IAL_IoctlCompat(struct file *filp, unsigned int cmd, unsigned long arg);
#endif
static Int  DtCorePcie_IAL_Open(struct inode* pInode, struct file* pFile);
static Int  DtCorePcie_IAL_Close(struct inode* pInode, struct file* pFile);
static UInt DtCorePcie_IAL_Poll(struct file* pFile, poll_table* pPollTable);
static ssize_t DtCorePcie_IAL_Read(struct file* pFile, char __user* pBuf, size_t Count, 
                                                                           loff_t* pOffp);

// PCI system interface
static Int  DtCorePcie_IAL_Probe(struct pci_dev* pPciDev, const struct pci_device_id* pId);
static void  DtCorePcie_IAL_Remove(struct pci_dev* pPciDev);
Int  DtCorePcie_IAL_Suspend(struct pci_dev* pPciDev, pm_message_t State);
Int  DtCorePcie_IAL_Resume(struct pci_dev* pPciDev);

// Child devices interface
DtStatus  DtCorePcie_IAL_IoctlChild(UInt32 IoctlCode, UInt InputBufferSize, void* pInputBuffer,
                                         UInt OutputBufferSize, void* pOutputBuffer, 
                                         UInt* pOutputBufferBytesWritten, void* pContext);

// Kernel module interface
static Int  DtCorePcie_IAL_ModuleInit(void);
static void  DtCorePcie_IAL_ModuleExit(void);

// Attributes
static ssize_t DtCorePcie_IAL_ShowSerial(
    struct device*  pDevice, 
    struct device_attribute*  pAttr, 
    char*  pBuf)
{
    DtCorePcie* pCore = (DtCorePcie*)dev_get_drvdata(pDevice);
    
    DtDbgOut(MAX, IAL, "Start");
    
    if (pCore != NULL)
    {
        DtDbgOut(AVG, IAL, "Serial: %llu", pCore->m_DevInfo.m_Serial);
        DtDbgOut(MAX, IAL, "Exit");
        return snprintf(pBuf, PAGE_SIZE, "%llu\n", pCore->m_DevInfo.m_Serial);
    }

    DtDbgOut(MAX, IAL, "Exit");
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Sysfs attributes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
static DEVICE_ATTR(serial, S_IRUGO, DtCorePcie_IAL_ShowSerial, NULL);
// Registering the sysfs attributes is done with the following two functions:
// device_create_file / device_remove_file
// The attribute itself is declared with the DEVICE_ATTR macro.
#else
// In newer kernels the DtCorePcie_IAL_Attributes array is directly
// registered with the DTA device class during the module init.
static struct device_attribute DtPcieAttributes[] = {
    __ATTR(serial, S_IRUGO, DtCorePcie_IAL_ShowSerial, NULL),
    __ATTR_NULL
};
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
static struct attribute *DtPcieAttributesArray[] = {
    &DtPcieAttributes[0].attr,
    NULL,
};
static struct attribute_group DtPcieAttributesGroup = {
    .attrs = DtPcieAttributesArray,
};
static const struct attribute_group *DtPcieAttributesGroups[] = {
    &DtPcieAttributesGroup,
    NULL,
}; 
#endif
#endif


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Interface declarations +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Character interface definition -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static struct file_operations  DtPcieFileOps = {
    .owner          = THIS_MODULE,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
    .ioctl          = DtCorePcie_IAL_Ioctl,
#else
    .unlocked_ioctl = DtCorePcie_IAL_UnlockedIoctl,
#endif
#ifdef CONFIG_COMPAT
    .compat_ioctl   = DtCorePcie_IAL_IoctlCompat,
#endif
    .open           = DtCorePcie_IAL_Open,
    .release        = DtCorePcie_IAL_Close,
    .mmap           = NULL,
    .poll           = DtCorePcie_IAL_Poll,
    .read           = DtCorePcie_IAL_Read,
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- System interface definition -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// The pci_driver variable name must end with _ops to prevent a compiler
// warning because it contains a reference to the DtaDeviceTable wich is of type 
// '__deviceinitconst'
//
static struct pci_driver  DtPciePciOps_ops = 
{
    .name = DRIVER_NAME,
    .id_table = DtaDeviceTable,
    .probe = DtCorePcie_IAL_Probe,
    .remove = DtCorePcie_IAL_Remove,
    .suspend = DtCorePcie_IAL_Suspend,
    .resume = DtCorePcie_IAL_Resume,
};

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private helper functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_InitDeviceData -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static void  DtCorePcie_IAL_InitDeviceData(DtCorePcie* pCore)
{
    // Default all 0
    memset(pCore, 0, sizeof(DtCorePcie));

    // Set type name
    pCore->m_TypeName = "DTA";

    // IAL state
    pCore->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_IDLE;
    pCore->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_DOWN;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_InitDeviceResources -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function must initialise Linux specific device resources and get the PCI
// configuration information
//
static Int  DtCorePcie_IAL_InitDeviceResources(DtCorePcie* pCore)
{
    // Save ID's from config space
    pCore->m_DevInfo.m_VendorId = pCore->m_Device.m_pPciDev->vendor;
    pCore->m_DevInfo.m_DeviceId = pCore->m_Device.m_pPciDev->device;
    pCore->m_DevInfo.m_SubVendorId = pCore->m_Device.m_pPciDev->subsystem_vendor;
    pCore->m_DevInfo.m_SubSystemId = pCore->m_Device.m_pPciDev->subsystem_device;
    pCore->m_DevInfo.m_BusNumber = pCore->m_Device.m_pPciDev->bus->number;
    pCore->m_DevInfo.m_SlotNumber = PCI_SLOT(pCore->m_Device.m_pPciDev->devfn);
    return 0;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_FreeDeviceResources -.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtCorePcie_IAL_FreeDeviceResources(DtCorePcie* pCore)
{
    return 0;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_MapResources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtCorePcie_IAL_MapResources(DtCorePcie* pCore)
{
    Int  Result;
    
    DtDbgOut(MIN, IAL, "Start");

    // Enable PCI device
    Result = pci_enable_device(pCore->m_Device.m_pPciDev);
    
    // Only proceed if no errors
    if (Result == 0)
    {
        // Enable DMA
        pci_set_master(pCore->m_Device.m_pPciDev);
        
        // Aquire memory resources
        Result = pci_request_regions(pCore->m_Device.m_pPciDev, DRIVER_NAME);
    }
    
    // Memory mapping. Only proceed if no errors
    if (Result == 0)
    {
        // Get DTA1xx memory address range (BAR0)
        pCore->m_DtPcieRegs.m_PciAddr.QuadPart =
            pci_resource_start(pCore->m_Device.m_pPciDev, 0);
        pCore->m_DtPcieRegs.m_Length =
            pci_resource_len(pCore->m_Device.m_pPciDev, 0);
        // Map into kernel space
        pCore->m_DtPcieRegs.m_pKernel =
                                   ioremap_nocache(pCore->m_DtPcieRegs.m_PciAddr.QuadPart,
                                                   pCore->m_DtPcieRegs.m_Length);

        if (pCore->m_DtPcieRegs.m_pKernel == NULL)
            Result = -EFAULT;
    }
    
    // Cleanup if any errors
    if (Result < 0)
    {
        DtCorePcie_IAL_UnMapResources(pCore);
        DtDbgOut(ERR, IAL, "Error mapping resources");
    }

    DtDbgOut(MIN, IAL, "Exit");

    return Result;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_IalRegisterISR -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtCorePcie_IAL_IalRegisterISR(DtCorePcie* pCore)
{
    Int  i=0, Result=0;
    const Int  MinNumVectors = 1;
    const Int  MaxNumVectors = 8;
    if (pCore->m_IalData.m_ISRRegistered)
        return 0;
    
    DtDbgOut(MIN, IAL, "Start");
    
    // Request the MSI-X interrupt vectors
    pCore->m_IalData.m_NumIrqVectors = 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
    Result = pci_alloc_irq_vectors(pCore->m_Device.m_pPciDev, MinNumVectors,
                                                             MaxNumVectors, PCI_IRQ_MSIX);
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(3,14,0)
    Result = pci_enable_msix_range(pCore->m_Device.m_pPciDev, pCore->m_IalData.MsixEntry, 
                                                            MinNumVectors, MaxNumVectors);
#else
    Result = MaxNumVectors;
    while (Result >= MinNumVectors && pci_enable_msix(pCore->m_Device.m_pPciDev, 
                                                 pCore->m_IalData.MsixEntry, Result) != 0)
        Result--;
#endif
    if (Result < MinNumVectors)
    {
        DtDbgOut(ERR, IAL, "Failed to alloc minumum amount (%d) of MSI-X vectors", 
                                                                           MinNumVectors);
    }
    else
        DtDbgOut(MIN, IAL, "Allocated %d of MSI-X vectors", Result);
    // Register handler for each vector
    for (i=0; i<Result; i++)
    {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
        Int  Irq = pci_irq_vector(pCore->m_Device.m_pPciDev, i);
#else
        Int  Irq = pCore->m_IalData.MsixEntry[i].vector;
#endif
        if (Irq < 0)
        {
            DtDbgOut(ERR, IAL, "Failed to get IRQ for MSI-X vector %d", i);
            return Irq;
        }
        Result = request_irq(Irq, 
                             (void*)DtCorePcie_IAL_Interrupt,
    #if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24)
                             SA_SHIRQ,            // Shared interrupt
    #else
                             IRQF_SHARED,         // Shared interrupt
    #endif
                             DRIVER_NAME,         // Driver name
                             pCore);           // is parameter
        if (Result < 0)
        {
            DtDbgOut(ERR, IAL, "Error registering ISR for MSI-X vector %d (irq=%d)",
                                                                                  i, Irq);
        } 
        pCore->m_IalData.m_NumIrqVectors++;
    }

    if (Result < 0)
        DtDbgOut(ERR, IAL, "Error registering ISR");
    else
        pCore->m_IalData.m_ISRRegistered = TRUE;
    
    DtDbgOut(MIN, IAL, "Exit");

    return Result;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_IalUnRegisterISR -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtCorePcie_IAL_IalUnRegisterISR(DtCorePcie* pCore)
{
    Int  i=0;

    if (pCore->m_IalData.m_NumIrqVectors <= 0)
        return;

    DtDbgOut(MIN, IAL, "Start");

    // Free interrupt resources
    for (i=0; i<pCore->m_IalData.m_NumIrqVectors; i++)
    {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
        Int  Irq = pci_irq_vector(pCore->m_Device.m_pPciDev, i);
#else
        Int  Irq = pCore->m_IalData.MsixEntry[i].vector;
#endif
        if (Irq < 0)
        {
            DtDbgOut(ERR, IAL, "Failed to get IRQ for MSI-X vector %d", i);
            continue;
        }
        free_irq(Irq, pCore);
    }
    pCore->m_IalData.m_ISRRegistered = FALSE;
    pCore->m_IalData.m_NumIrqVectors = 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,8,0)
    pci_free_irq_vectors(pCore->m_Device.m_pPciDev);
#else
    pci_disable_msix(pCore->m_Device.m_pPciDev);
#endif

    DtDbgOut(MIN, IAL, "Exit");
    return;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_UnMapResources -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static void  DtCorePcie_IAL_UnMapResources(DtCorePcie* pCore)
{
    DtDbgOut(MIN, IAL, "Start");

    // Unmap PCI IO memory
    if (pCore->m_DtPcieRegs.m_pKernel != NULL)
    {
        iounmap((char*)pCore->m_DtPcieRegs.m_pKernel);
        pCore->m_DtPcieRegs.m_pKernel = NULL;
    }

    // Disable DMA
    //pci_clear_master(pCore->m_Device.m_pPciDev);

    // Disable PCI device
    pci_disable_device(pCore->m_Device.m_pPciDev);

    // Finnaly...
    // Call pci_release_region AFTER pci_disable_device to prevent to devices
    // colliding on the same address range.
    pci_release_regions(pCore->m_Device.m_pPciDev);

    DtDbgOut(MIN, IAL, "Exit");
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_DevicePowerUpSeq -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtCorePcie_IAL_DevicePowerUpSeq(DtCorePcie* pCore)
{
    DtStatus  Status;
    
    switch (pCore->m_IalData.m_PowerSeqState)
    {
    case DEVICE_POWERSEQ_STATE_DOWN:
        // Call common powerup
        Status = DtCorePcie_DEVICE_PowerUp(pCore);
        if (!DT_SUCCESS(Status))
        {
            DtCorePcie_IAL_DevicePowerDownSeq(pCore, TRUE);
            return -EPERM;
        }
        // Next state in power up sequence
        pCore->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_UP;
        
    case DEVICE_POWERSEQ_STATE_UP:
        // All channels are initialised. We can now register the ISR
        if (DtCorePcie_IAL_IalRegisterISR(pCore) < 0)
        {
            DtCorePcie_IAL_DevicePowerDownSeq(pCore, TRUE);
            return -EPERM;
        }

        // Call common interrupts enable
        Status = DtCorePcie_DEVICE_InterruptEnable(pCore);
        if (!DT_SUCCESS(Status))
        {
            DtCorePcie_IAL_DevicePowerDownSeq(pCore, TRUE);
            return -EPERM;
        }
        // Next state in power up sequence
        pCore->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_INT_ENABLED;
        
    case DEVICE_POWERSEQ_STATE_INT_ENABLED:
        // Call common powerup post
        Status = DtCorePcie_DEVICE_PowerUpPost(pCore);
        if (!DT_SUCCESS(Status))
        {
            DtCorePcie_IAL_DevicePowerDownSeq(pCore, TRUE);
            return -EPERM;
        }
        // Next state in power up sequence
        pCore->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_UP_POST;
        
    case DEVICE_POWERSEQ_STATE_UP_POST:
    default :
        // Nothing to do, fully powered up
    break;
    }

    return 0;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_DevicePowerDownSeq -.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtCorePcie_IAL_DevicePowerDownSeq(DtCorePcie* pCore, Bool IgnoreErrors)
{
    DtStatus  Status;
    
    switch (pCore->m_IalData.m_PowerSeqState)
    {
    case DEVICE_POWERSEQ_STATE_UP_POST:
        // Call common powerup pre
        Status = DtCorePcie_DEVICE_PowerDownPre(pCore);
        if (!IgnoreErrors)
        {
            if (!DT_SUCCESS(Status))
            {
                DtCorePcie_IAL_DevicePowerUpSeq(pCore);
                return -EPERM;
            }
            // Next state in power down sequence
            pCore->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_UP;
        }
        
    case DEVICE_POWERSEQ_STATE_UP:
        // Call common interrupts disable
        Status = DtCorePcie_DEVICE_InterruptDisable(pCore);
        if (!IgnoreErrors)
        {
            if (!DT_SUCCESS(Status))
            {
                DtCorePcie_IAL_DevicePowerUpSeq(pCore);
                return -EPERM;
            }
            // Next state in power down sequence
            pCore->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_INT_ENABLED;
        }
        
    case DEVICE_POWERSEQ_STATE_INT_ENABLED:
        // Call common powerdown
        Status = DtCorePcie_DEVICE_PowerDown(pCore);
        if (!IgnoreErrors)
        {
            if (!DT_SUCCESS(Status))
            {
                DtCorePcie_IAL_DevicePowerUpSeq(pCore);
                return -EPERM;
            }
            // Next state in power down sequence
            pCore->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_DOWN;
        }
        
    case DEVICE_POWERSEQ_STATE_DOWN:
    default:
        // Nothing to do, fully powered down
    break;
    }

    if (IgnoreErrors)
    {
        // If errors are ignored, final state is allways down
        pCore->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_DOWN;
    }

    return 0;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_DeviceStartSeq -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtCorePcie_IAL_DeviceStartSeq(DtCorePcie* pCore)
{
    Int  Result;
    DtStatus  Status;
    dev_t  DevNum;
    
    switch (pCore->m_IalData.m_StartSeqState)
    {
    case DEVICE_STARTSEQ_STATE_IDLE:
        // Init Linux specific device resources
        Result = DtCorePcie_IAL_InitDeviceResources(pCore);
        if (Result < 0)
        {
            DtCorePcie_IAL_DeviceStopSeq(pCore);
            return Result;
        }
        
        // Next state in start sequence
        pCore->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_RES_INITIALIZED;
    
    case DEVICE_STARTSEQ_STATE_RES_INITIALIZED:
        // Call common init
        Status = DtCorePcie_DEVICE_Init(pCore);
        if (!DT_SUCCESS(Status))
        {
            DtCorePcie_IAL_DeviceStopSeq(pCore);
            return -EPERM;
        }
                
        // Next state in start sequence
        pCore->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_COMMON_INITIALIZED;
    
    case DEVICE_STARTSEQ_STATE_COMMON_INITIALIZED:
        // Create memory mappings and request IRQ
        Result = DtCorePcie_IAL_MapResources(pCore);
        if (Result < 0)
        {
            DtCorePcie_IAL_DeviceStopSeq(pCore);
            return Result;
        }
        
        // Next state in start sequence
        pCore->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_RES_MAPPED ;
    
    case DEVICE_STARTSEQ_STATE_RES_MAPPED:
        // Execute power sequence in common driver
        Result = DtCorePcie_IAL_DevicePowerUpSeq(pCore);
        if (Result < 0)
        {
            DtCorePcie_IAL_DeviceStopSeq(pCore);
            return Result;
        }
        
        // Next state in start sequence
        pCore->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_POWERED_UP;
    
    case DEVICE_STARTSEQ_STATE_POWERED_UP:
        // Merge Major / Minor to dev_t
        DevNum = MKDEV(g_DtPcieMajor, g_DtPcieMinor + pCore->m_IalData.m_DvcIndex);
        // Initialize cdev
        cdev_init(&pCore->m_Device.m_CDev, &DtPcieFileOps);
        // Add character device interface
        Result = cdev_add(&pCore->m_Device.m_CDev, DevNum, 1);
        if (Result < 0)
        {
            DtCorePcie_IAL_DeviceStopSeq(pCore);
            return Result;
        }
        
        // Next state in start sequence
        pCore->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_CDEV_ADDED;
    
    case DEVICE_STARTSEQ_STATE_CDEV_ADDED:
        // Merge Major / Minor to dev_t
        DevNum = MKDEV(g_DtPcieMajor, g_DtPcieMinor + pCore->m_IalData.m_DvcIndex);
        // Create a device withing the DtCorePcie_IAL_ class
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,27)
        pCore->m_Device.m_pDevice = device_create(g_pDtPcieClass, NULL, DevNum,
                                         DRIVER_NAME"%u", pCore->m_IalData.m_DvcIndex);
#else
        pCore->m_Device.m_pDevice = device_create(g_pDtPcieClass, NULL, DevNum, pCore,
                                         DRIVER_NAME"%u", pCore->m_IalData.m_DvcIndex);
#endif

        dev_set_drvdata(pCore->m_Device.m_pDevice, pCore);
        if (IS_ERR(pCore->m_Device.m_pDevice))
        {
            DtCorePcie_IAL_DeviceStopSeq(pCore);
            return PTR_ERR(pCore->m_Device.m_pDevice);
        }
        
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
        // Create serial sysfs file
        device_create_file(pCore->m_Device.m_pDevice, &dev_attr_serial);
#endif
        
        // Next state in start sequence
        pCore->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_DEV_CREATED;
    
    case DEVICE_STARTSEQ_STATE_DEV_CREATED:
    default :
        // Start sequence is done
        break;
    }
    
    return 0;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_DeviceStopSeq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void DtCorePcie_IAL_DeviceStopSeq(DtCorePcie* pCore)
{
    switch (pCore->m_IalData.m_StartSeqState)
    {
        case DEVICE_STARTSEQ_STATE_DEV_CREATED:
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
            // Remove serial sysfs file
            device_remove_file(pCore->m_Device.m_pDevice, &dev_attr_serial);
#endif
            // Destroy class device
            device_destroy(g_pDtPcieClass, pCore->m_Device.m_CDev.dev);
            
        case DEVICE_STARTSEQ_STATE_CDEV_ADDED:
            // Remove cdev object
            cdev_del(&pCore->m_Device.m_CDev);
            
        case DEVICE_STARTSEQ_STATE_POWERED_UP:
            // Execute powerdown sequence in common driver
            DtCorePcie_IAL_DevicePowerDownSeq(pCore, TRUE);
            
        case DEVICE_STARTSEQ_STATE_RES_MAPPED:
            // Unmap resources
            DtCorePcie_IAL_IalUnRegisterISR(pCore);
            DtCorePcie_IAL_UnMapResources(pCore);
            
        case DEVICE_STARTSEQ_STATE_COMMON_INITIALIZED:
            // Execute DeviceExitPre in common driver
            DtCorePcie_DEVICE_ExitPre(pCore);
            // Execute DeviceExit in common driver
            DtCorePcie_DEVICE_Exit(pCore);
            
        case DEVICE_STARTSEQ_STATE_RES_INITIALIZED:
            // Free all Linux specific device resources
            DtCorePcie_IAL_FreeDeviceResources(pCore);
            
        case DEVICE_STARTSEQ_STATE_IDLE:
        default:
            // Stop sequence is done
            break;
    }
    
    // After DeviceStopSeq all resources are free
    pCore->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_IDLE;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Interrupt handler +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_Interrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static irqreturn_t  DtCorePcie_IAL_Interrupt(Int Irq, void* pContext)
{
    DtCorePcie*  pCore = (DtCorePcie*)pContext;
    if (pCore == NULL)
        return IRQ_NONE;
    
    if (DtCorePcie_DEVICE_Interrupt(pCore, -1))
        return IRQ_HANDLED;
    else
        return IRQ_NONE;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Character interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#ifdef CONFIG_COMPAT
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_IoctlCompat -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Compatibility function for 32-bit applications using a 64-bit driver
//
long DtCorePcie_IAL_IoctlCompat(
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
    return DtCorePcie_IAL_Ioctl(pInode, pFile, Cmd, (unsigned long)compat_ptr(Arg));
}
#endif  // #ifdef CONFIG_COMPAT

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_UnlockedIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static long  DtCorePcie_IAL_UnlockedIoctl(struct file* pFile, unsigned int Cmd, 
                                                                        unsigned long Arg)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
    struct inode*  pInode = pFile->f_dentry->d_inode;
#else
    struct inode*  pInode = pFile->f_path.dentry->d_inode;
#endif
    return DtCorePcie_IAL_Ioctl(pInode, pFile, Cmd, Arg);
}
#endif

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_Ioctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static int  DtCorePcie_IAL_Ioctl(
    struct inode*  pInode,
    struct file*  pFile,
    unsigned int  Cmd,
    unsigned long  Arg)
{
    DtStatus  Status;
    Int  Result = 0;
    DtFileObject  File;
    DtIoctlObject  Ioctl;
    DtPcieIoctlInputData  IoctlInputData; 
    DtPcieIoctlOutputData  IoctlOutputData; 

    char*  pIoctlInputData = NULL;
    char*  pIoctlOutputData = NULL;
    
    UInt  InputSizeParam = 0;
    UInt  OutputSizeParam = 0;
    UInt  ReservedForSizeParam = 0;
    DtPageList*  pPageListIn = NULL;
    DtPageList*  pPageListOut = NULL;

    DtDbgOut(MAX, IAL, "Start");

    // Check command that this is one of our commands
    if (DT_IOCTL_MAGIC!=_IOC_TYPE(Cmd) && DT_IOCTL_MAGIC_SIZE!=_IOC_TYPE(Cmd)
         && DTPCIE_IOCTL_MAGIC!=_IOC_TYPE(Cmd) && DTPCIE_IOCTL_MAGIC_SIZE!=_IOC_TYPE(Cmd))
        Result = -ENOTTY;

    if (Result >= 0)
    {
        // Check device context
        if (pFile->private_data == NULL)
            Result = -EFAULT;
    }

    if (Result >= 0)
    {
        if(_IOC_TYPE(Cmd)==DTPCIE_IOCTL_MAGIC_SIZE || _IOC_TYPE(Cmd)==DT_IOCTL_MAGIC_SIZE)
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
            if ((_IOC_TYPE(Cmd)==DTPCIE_IOCTL_MAGIC || _IOC_TYPE(Cmd)==DT_IOCTL_MAGIC) && 
                                         Ioctl.m_OutputBufferSize>sizeof(IoctlOutputData))
                Ioctl.m_OutputBufferSize = sizeof(IoctlOutputData);

            DtDbgOut(MAX, IAL, "Output buffer size %d", Ioctl.m_OutputBufferSize);

            // Ioctl reads --> driver needs write access for user memory
            if (access_ok(VERIFY_WRITE, (void*)Arg, Ioctl.m_OutputBufferSize) == 0)
                Result = -EFAULT;

            // Allocate memory for the output data
            if (ReservedForSizeParam != 0)
            {
                if (Ioctl.m_OutputBufferSize != 0)
                {
                    pIoctlOutputData = DtMemAllocPoolLarge(DtPoolNonPaged, 
                                              Ioctl.m_OutputBufferSize, 0, &pPageListOut);
                    Ioctl.m_pOutputBuffer = pIoctlOutputData;
                    if (pIoctlOutputData == NULL)
                            Result = -EFAULT;
                } else
                    Ioctl.m_pOutputBuffer = NULL;
            } else
                Ioctl.m_pOutputBuffer = &IoctlOutputData;
        }

        if ((_IOC_DIR(Cmd)&_IOC_WRITE) != 0)
        {
            // Set input buffer size
            Ioctl.m_InputBufferSize = InputSizeParam;

            if ((_IOC_TYPE(Cmd)==DTPCIE_IOCTL_MAGIC || _IOC_TYPE(Cmd)==DT_IOCTL_MAGIC) && 
                                           Ioctl.m_InputBufferSize>sizeof(IoctlInputData))
                Ioctl.m_InputBufferSize = sizeof(IoctlInputData);
            
            DtDbgOut(MAX, IAL, "Input buffer size %d", Ioctl.m_InputBufferSize);
            
            // Ioctl writes --> driver needs read access for user memory
            if (access_ok(VERIFY_READ, (void*)Arg, 
                                       Ioctl.m_InputBufferSize+ReservedForSizeParam) == 0)
                Result = -EFAULT;
            
            // Allocate memory for the input data
            if (ReservedForSizeParam != 0)
            {
                if (Ioctl.m_InputBufferSize != 0)
                {
                    pIoctlInputData = DtMemAllocPoolLarge(DtPoolNonPaged,
                                                Ioctl.m_InputBufferSize, 0, &pPageListIn);
                    Ioctl.m_pInputBuffer = pIoctlInputData;
                    if (pIoctlInputData == NULL)
                        Result = -EFAULT;
                } else
                    Ioctl.m_pInputBuffer = NULL;
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

                if (copy_from_user(Ioctl.m_pInputBuffer, (void*)pArg, 
                                                            Ioctl.m_InputBufferSize) != 0)
                    Result = -EFAULT;
            }
        }
    }

    if (Result >= 0)
    {
        // Call common IOCTRL
        Status = DtCorePcie_Ioctl(pFile->private_data, &File, &Ioctl);
        
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
        DtMemFreePoolLarge(pIoctlInputData, 0, pPageListIn);
    if (pIoctlOutputData != NULL)
        DtMemFreePoolLarge(pIoctlOutputData, 0, pPageListOut);
    
    DtDbgOut(MAX, IAL, "Exit");

    return Result;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtCorePcie_IAL_Open(struct inode* pInode, struct file* pFile)
{
    DtStatus  Status;
    DtFileObject  File;
    Int  Result = 0;
    Int  Index;

    DtDbgOut(MAX, IAL, "Start");

    // Get the device index that is opened (Use minor number without offset)
    Index = MINOR(pInode->i_rdev) - g_DtPcieMinor;

    // Init File object
    File.m_pFile = pFile;

    // Store device data device data
    pFile->private_data = g_pDtCorePcie[Index];

    // Call common open
    Status = DtCorePcie_Open(pFile->private_data, &File);

    // Convert result
    if (!DT_SUCCESS(Status))
        Result = -EFAULT;
    
    DtDbgOut(MAX, IAL, "Exit");
    
    return Result;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtCorePcie_IAL_Close(struct inode* pInode, struct file* pFile)
{
    DtFileObject  File;

    DtDbgOut(MAX, IAL, "Start");

    // Check device context
    if (pFile->private_data == NULL)
        return -EFAULT;

    // Init File object
    File.m_pFile = pFile;

    // Call common close
    DtCorePcie_Close(pFile->private_data, &File);

    // clear private_data
    pFile->private_data = NULL;
    
    DtDbgOut(MAX, IAL, "Exit");
    
    return 0;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_Poll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static UInt  DtCorePcie_IAL_Poll(struct file* pFile, poll_table* pPollTable)
{
    DtFileObject  File;
    UInt  Mask = 0;
    DtCorePcie*  pCore = (DtCorePcie*)pFile->private_data;

    // Init File object
    File.m_pFile = pFile;

    DtDbgOut(MAX, IAL, "Start");
    
    if (pCore != NULL)
    {
        Mask = DtCore_EVENTS_Poll((DtCore*)pCore, &File, pPollTable);
    }
    DtDbgOut(MAX, IAL, "Exit");

    return Mask;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_Read -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static ssize_t  DtCorePcie_IAL_Read(struct file* pFile, char __user* pBuf, size_t Count, loff_t* pOffp)
{
    // DtCorePcie_IAL_EventGet
    return 0;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ System interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_Probe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtCorePcie_IAL_Probe(struct pci_dev *pPciDev, const struct pci_device_id *pId)
{
    Int  Result = 0;
    UInt  DvcIndex=0;
    DtCorePcie*  pCore = NULL;
    
    DtDbgOut(MAX, IAL, "Start");
    
    // Quick check if maximum device count is reached
    if (g_DtPcieDeviceCount < g_DtPcieMaxDevices)
    {
        // Find free DvcIndex
        for (DvcIndex=0; DvcIndex<g_DtPcieMaxDevices; DvcIndex++)
        {
            if (g_pDtCorePcie[DvcIndex] == NULL)
                break;
        }
        if (DvcIndex == g_DtPcieMaxDevices)
            Result = -ENOMEM;
    } else
        Result = -ENOMEM;
    
    // Only proceed if no errors
    if (Result >= 0)
    {
        // Allocate memory for device context
        pCore = kmalloc(sizeof(DtCorePcie), GFP_KERNEL);
        if (pCore != NULL)
        {
            // Initialize device data to default values
            DtCorePcie_IAL_InitDeviceData(pCore);
            
            // Store handle to pCore in array
            g_pDtCorePcie[DvcIndex] = pCore;

            // Set common device info pointer and init size
            pCore->m_pDevInfo = (DtDeviceInfo*)&pCore->m_DevInfo;
            pCore->m_DevInfo.m_Size = sizeof(DtPcieDeviceInfo);

            // Store DvcIndex in device data IAL data
            pCore->m_IalData.m_DvcIndex = DvcIndex;
            pCore->m_IalData.m_ISRRegistered = FALSE;
        } else
            Result = -ENOMEM;
    }
    
    // Only proceed if no errors
    if (Result >= 0)
    {
        // Store pointer to this module and pci_dev in device data
        pCore->m_Driver.m_pModule = THIS_MODULE;
        DtEvtLogInit(&pCore->m_Driver.m_EvtObject, &EventGetMessage);
        pCore->m_Device.m_pPciDev = pPciDev;
        DtEvtLogInit(&pCore->m_Device.m_EvtObject, &EventGetMessage);
    }
    
    // Only proceed if no errors
    if (Result >= 0)
        // Start DTA card
        Result = DtCorePcie_IAL_DeviceStartSeq(pCore);
    
    // Only proceed if no errors
    if (Result >= 0)
    {
        // Set device driver data
        pci_set_drvdata(pCore->m_Device.m_pPciDev, pCore);
    
        // Finnaly increment device count
        g_DtPcieDeviceCount++;
        
        DtDbgOut(AVG, IAL, "New device count: %u", g_DtPcieDeviceCount);
    }

    // Cleanup if errors
    if (Result < 0)
    {
        if (pCore != NULL)
        {
            // Free minor by clearing array entry
            g_pDtCorePcie[pCore->m_IalData.m_DvcIndex] = NULL;
            
            // Free device data
            kfree(pCore);
        }
    }
    
    DtDbgOut(MAX, IAL, "Exit");
    
    return Result;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_Remove -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static void  DtCorePcie_IAL_Remove(struct pci_dev *pPciDev)
{
    DtCorePcie*  pCore;
    
    DtDbgOut(MAX, IAL, "Start");

    // Get device data
    pCore = pci_get_drvdata(pPciDev);
    if (pCore == NULL)
        return;
    
    // Clear device driver data
    pci_set_drvdata(pCore->m_Device.m_pPciDev, NULL);

    // Stop DTA card
    DtCorePcie_IAL_DeviceStopSeq(pCore);
    
    // Free minor by clearing array entry
    g_pDtCorePcie[pCore->m_IalData.m_DvcIndex] = NULL;
    
    // Free device data
    kfree(pCore);
    
    // Decrement device count
    g_DtPcieDeviceCount--;
    
    DtDbgOut(AVG, IAL, "New device count: %u", g_DtPcieDeviceCount);
    
    DtDbgOut(MAX, IAL, "Exit");
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_Suspend -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtCorePcie_IAL_Suspend(struct pci_dev *pPciDev, pm_message_t State)
{
    Int  Result = 0;
    DtCorePcie*  pCore;
    
    DtDbgOut(MAX, IAL, "Start");

    // Get device data
    pCore = pci_get_drvdata(pPciDev);
    if (pCore == NULL)
        return -ENODEV;

    // Save the PCI configuration space
    pci_save_state(pPciDev);

    // Execute powerdown sequence in common driver
    Result = DtCorePcie_IAL_DevicePowerDownSeq(pCore, FALSE);
    if (Result == 0)
    {
        // Unmap the device resources
        DtCorePcie_IAL_IalUnRegisterISR(pCore);
        DtCorePcie_IAL_UnMapResources(pCore);
    }
    
    DtDbgOut(MAX, IAL, "Exit");

    return Result;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_Resume -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtCorePcie_IAL_Resume(struct pci_dev *pPciDev)
{
    Int  Result = 0;
    DtCorePcie* pCore;
    
    DtDbgOut(MAX, IAL, "Start");

    // Get device data
    pCore = pci_get_drvdata(pPciDev);
    if (pCore == NULL)
        return -ENODEV;

    // Make sure property not found counter is reset after a power resume
    //DtResetPropertiesNotFoundCounter(&pCore->m_PropData);

    // Restore the PCI configuration space
    pci_restore_state(pPciDev);
    
    // Map the device resources
    Result = DtCorePcie_IAL_MapResources(pCore);
    if (Result == 0)
    {
        // Execute powerup sequence in common driver
        Result = DtCorePcie_IAL_DevicePowerUpSeq(pCore);
        if (Result < 0)
        {
            // Unmap the device resources
            DtCorePcie_IAL_IalUnRegisterISR(pCore);
            DtCorePcie_IAL_UnMapResources(pCore);
        }
    }
    
    DtDbgOut(MAX, IAL, "Exit");
    
    return Result;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Module init / exit +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_ModuleInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtCorePcie_IAL_ModuleInit(void)
{
    Int  Result = 0;
    dev_t  Dev = 0;
    DtStatus  Status;
    DtEvtLog  EvtObject;
    
    DtDbgOut(MAX, IAL, "Start");

    // Get major device number
    if (g_DtPcieMajor != 0) 
    {
        Dev = MKDEV(g_DtPcieMajor, g_DtPcieMinor);
        Result = register_chrdev_region(Dev, g_DtPcieMaxDevices, DRIVER_NAME);
    } else {
        Result = alloc_chrdev_region(&Dev, g_DtPcieMinor, g_DtPcieMaxDevices, DRIVER_NAME);
        g_DtPcieMajor = MAJOR(Dev);
    }
    if (Result < 0)
        DtDbgOut(ERR, IAL, "Can't get major %d", g_DtPcieMajor);

    // Only proceed if no errors
    if (Result >= 0)
    {
        // Allocate memory for device context pointers
        g_pDtCorePcie = kmalloc(g_DtPcieMaxDevices * sizeof(DtCorePcie*), GFP_KERNEL);
        if (g_pDtCorePcie != NULL)
            memset(g_pDtCorePcie, 0, g_DtPcieMaxDevices * sizeof(DtCorePcie*));
        else
            Result = -EBUSY;
    }

    // Only proceed if no errors
    if (Result >= 0)
    {
        // Create Device class
        g_pDtPcieClass = class_create(THIS_MODULE, DRIVER_NAME);
        if (IS_ERR(g_pDtPcieClass))
            Result = PTR_ERR(g_pDtPcieClass);
        else {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,13,0)
            // Add device class attributes
            g_pDtPcieClass->dev_groups = DtPcieAttributesGroups;
#elif LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,19)
            // Add device class attributes
            g_pDtPcieClass->dev_attrs = DtPcieAttributes;
#endif
        }
    }
    
    // Only proceed if no errors
    if (Result >= 0)
    {
        // Call init in common driver
        DtEvtLogInit(&EvtObject, &EventGetMessage);
        Status = DtCorePcie_DRIVER_Init(&EvtObject);
        if (!DT_SUCCESS(Status))
            Result = -EBUSY;
    }
    
    // Only proceed if no errors
    if (Result >= 0)
        // Register our system interface in the PCI driver
        // DtCorePcie_IAL_Probe is called from this function for all devices in the system
        Result = pci_register_driver(&DtPciePciOps_ops);
    
    // Cleanup if errors
    if (Result < 0)
        DtCorePcie_IAL_ModuleExit();

    // Initialise the child driver interface
    //DtSpinLockInit(&g_DtPcieChildDriver.m_SpinLock);
    //g_DtPcieChildDriver.m_pDriverItf = NULL;
    
    DtDbgOut(MAX, IAL, "Exit");
    return Result;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IAL_ModuleExit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static void  DtCorePcie_IAL_ModuleExit(void)
{
    dev_t  DevNum = 0;
        
    DtDbgOut(MAX, IAL, "Start");

    // Unregister our system interface in the PCI driver
    pci_unregister_driver(&DtPciePciOps_ops);
    
    if (g_DtPcieDeviceCount != 0)
        DtDbgOut(ERR, IAL, "DtCorePcie_IAL_ModuleExit called, but DtCorePcie_IAL_DeviceCount is not 0 after"
                                           " pci_unregister_driver (%u)", g_DtPcieDeviceCount);

    // Call exit in common driver
    DtCorePcie_DRIVER_Exit();

    // Destroy device class
    if (!IS_ERR(g_pDtPcieClass))
        class_destroy(g_pDtPcieClass);

    // Free memory for device context pointers
    if (g_pDtCorePcie != NULL)
        kfree(g_pDtCorePcie);

    // Unregister character device region
    DevNum = MKDEV(g_DtPcieMajor, g_DtPcieMinor);
    unregister_chrdev_region(DevNum, g_DtPcieMaxDevices);
    
    DtDbgOut(MAX, IAL, "Exit");
}

module_init(DtCorePcie_IAL_ModuleInit);
module_exit(DtCorePcie_IAL_ModuleExit);




