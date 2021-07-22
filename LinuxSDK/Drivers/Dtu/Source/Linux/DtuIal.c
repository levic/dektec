//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtuIal.c *#*#*#*#*#*#*#*#*# (C) 2011-2016 DekTec
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtuIncludes.h>
#include <linux/poll.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Compile time defaults
#ifndef DTU_MAX_DEVICES
#define  DTU_MAX_DEVICES 10
#endif
#define  DTU_DEV_DRIVER_PATH    "usb/DekTec/Dtu%d"
#define  DTU_DEV_MINOR          176             // Dev. driver minor number

#define  DTU_VENDOR_ID          0x1297

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Global variables
Int  g_DtuMaxDevices = DTU_MAX_DEVICES;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Global variables -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Load time parameters

// Supported devices table
extern struct usb_device_id  DtuDeviceTable[];

MODULE_AUTHOR("DekTec Digital Video B.V.");
MODULE_DESCRIPTION("Dtu Series Driver");

// ADDED TO PREVENT TAINTED MESSAGE. THIS MODULE CAN BE USED FREELY (SEE LICENSE
// STATEMENT ABOVE).
MODULE_LICENSE("GPL";)

// Global variables
static DtuDeviceData**  g_pDtuDeviceData = NULL;
static Int  g_DtuDeviceCount = 0;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Forward declarations +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Private helper functions
static Int  DtuDevicePowerUpSeq(DtuDeviceData* pDvcData);
static Int  DtuDevicePowerDownSeq(DtuDeviceData* pDvcData, Bool IgnoreErrors);
static Int  DtuDeviceStartSeq(DtuDeviceData* pDvcData);
static void  DtuDeviceStopSeq(DtuDeviceData* pDvcData, Bool Initializing);

static void  DtuInitDeviceData(DtuDeviceData* pDvcData);
static Int  DtuInitDeviceResources(DtuDeviceData* pDvcData);
static Int  DtuFreeDeviceResources(DtuDeviceData* pDvcData);
void  DtuDevDelete(struct kref* pKRef);
void  DtuDecrUsageCount(DtuDeviceData* pDvcData);
void  DtuIncrUsageCount(DtuDeviceData* pDvcData);

// Character device interface
static long  DtuIoctl(struct inode* pInode, struct file* pFile, unsigned int Cmd, 
                                                                       unsigned long Arg);
static long  DtuUnlockedIoctl(struct file* pFile, unsigned int Cmd, unsigned long Arg);
#ifdef CONFIG_COMPAT
// 32-bit applications using 64-bit driver
static long  DtuIoctlCompat(struct file *filp, unsigned int cmd, unsigned long arg);
#endif
static Int  DtuOpen(struct inode* pInode, struct file* pFile);
static Int  DtuClose(struct inode* pInode, struct file* pFile);
static UInt  DtuPoll(struct file* pFile, poll_table* pPollTable);
static ssize_t  DtuIalRead(struct file* pFile, char __user* pBuf, size_t Count, 
                                                                           loff_t* pOffp);
static ssize_t  DtuIalWrite(struct file* pFile, const char __user* pBuf, 
                                                       size_t TransferSize, loff_t* pPos);

// USB system interface
static Int  DtuProbe(struct usb_interface* pUsbItf, 
                                                   const struct usb_device_id* pUsbDevId);
static void  DtuDisconnect(struct usb_interface* pUsbItf);
Int  DtuSuspend(struct usb_interface* pUsbItf, pm_message_t State);
Int  DtuResume(struct usb_interface* pUsbItf);


// Kernel module interface
static Int  DtuModuleInit(void);
static void  DtuModuleExit(void);

// Attributes
static ssize_t  DtuShowSerial(
    struct device*  pDevice, 
    struct device_attribute*  pAttr, 
    char*  pBuf)
{
    DtuDeviceData*  pDvcData = (DtuDeviceData*)dev_get_drvdata(pDevice);
    
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

static DEVICE_ATTR(serial, S_IRUGO, DtuShowSerial, NULL);

// Registering the sysfs attributes is done with the following two functions:
// device_create_file / device_remove_file
// The attribute itself is declared with the DEVICE_ATTR macro.

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,4,4)
#define HAVE_COMPAT_PTR_IOCTL
#endif


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Interface declarations +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Character interface definition -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
struct file_operations  DtuFileOps =
{
    .owner = THIS_MODULE,
    .llseek = no_llseek,
    .read = DtuIalRead,
    .write = DtuIalWrite,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,19)
    .ioctl = DtuIoctl,
#else
    .unlocked_ioctl = DtuUnlockedIoctl,
#endif
#ifdef CONFIG_COMPAT
#ifdef HAVE_COMPAT_PTR_IOCTL
    .compat_ioctl = compat_ptr_ioctl,
#else
    .compat_ioctl = DtuIoctlCompat,
#endif
#endif
    .open = DtuOpen,
    .release = DtuClose,
    .poll = DtuPoll,
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- System interface definition -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static struct usb_driver  DtuUsbOps = 
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16)
    .owner = THIS_MODULE,
#endif
    .name = DRIVER_NAME,
    .probe = DtuProbe,
    .disconnect = DtuDisconnect,
    .id_table = DtuDeviceTable,
    .suspend = DtuSuspend,
    .resume = DtuResume,
};

static struct usb_class_driver  DtuClass =
{
    .name = DTU_DEV_DRIVER_PATH,
    .fops = &DtuFileOps,
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 12)
    .mode = S_IFCHR | S_IWUSR | S_IWGRP | S_IWOTH | S_IRUSR | S_IRGRP | S_IROTH,
#endif
    .minor_base = DTU_DEV_MINOR,
};

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private helper functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuInitDeviceData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtuInitDeviceData(DtuDeviceData* pDvcData)
{
    // Default all 0
    memset(pDvcData, 0, sizeof(DtuDeviceData));

    // IAL state
    pDvcData->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_IDLE;
    pDvcData->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_DOWN;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuInitDeviceResources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function must initialise Linux specific device resources and get the USB
// configuration information
//
static Int  DtuInitDeviceResources(DtuDeviceData* pDvcData)
{
    // Save ID's from config space
    pDvcData->m_DevInfo.m_ProductId = pDvcData->m_Device.m_pUsbDev->descriptor.idProduct;
    pDvcData->m_DevInfo.m_VendorId = pDvcData->m_Device.m_pUsbDev->descriptor.idVendor;
    pDvcData->m_DevInfo.m_HardwareRevision = 
                                       pDvcData->m_Device.m_pUsbDev->descriptor.bcdDevice;
    return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuFreeDeviceResources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtuFreeDeviceResources(DtuDeviceData* pDvcData)
{
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDevicePowerUpSeq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtuDevicePowerUpSeq(DtuDeviceData* pDvcData)
{
    DtStatus  Status;
    
    switch (pDvcData->m_IalData.m_PowerSeqState)
    {
    case DEVICE_POWERSEQ_STATE_DOWN:
        // Call common powerup
        Status = DtuDevicePowerUp(pDvcData);
        if (!DT_SUCCESS(Status))
        {
            DtuDevicePowerDownSeq(pDvcData, TRUE);
            return -EPERM;
        }
        // Next state in power up sequence
        pDvcData->m_IalData.m_PowerSeqState = DEVICE_POWERSEQ_STATE_UP;
        
    case DEVICE_POWERSEQ_STATE_UP:
    default :
        // Nothing to do, fully powered up
    break;
    }

    return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDevicePowerDownSeq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtuDevicePowerDownSeq(DtuDeviceData* pDvcData, Bool IgnoreErrors)
{
    DtStatus  Status;
    
    switch (pDvcData->m_IalData.m_PowerSeqState)
    {
    case DEVICE_POWERSEQ_STATE_UP:
        // Call common powerdown
        Status = DtuDevicePowerDown(pDvcData, DT_STATE_D3);
        if (!IgnoreErrors)
        {
            if (!DT_SUCCESS(Status))
            {
                DtuDevicePowerUpSeq(pDvcData);
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDeviceStartSeq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtuDeviceStartSeq(DtuDeviceData* pDvcData)
{
    Int  Result;
    DtStatus  Status;
    
    switch (pDvcData->m_IalData.m_StartSeqState)
    {
    case DEVICE_STARTSEQ_STATE_IDLE:
        // Init Linux specific device resources
        Result = DtuInitDeviceResources(pDvcData);
        if (Result < 0)
        {
            DtuDeviceStopSeq(pDvcData, true);
            return Result;
        }
        
        // Next state in start sequence
        pDvcData->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_RES_INITIALIZED;

    case DEVICE_STARTSEQ_STATE_RES_INITIALIZED:
        // Call common init
        Status = DtuDeviceInit(pDvcData);
        if (!DT_SUCCESS(Status))
        {
            DtuDeviceStopSeq(pDvcData, true);
            return -EPERM;
        }
                
        // Next state in start sequence
        pDvcData->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_COMMON_INITIALIZED;

    case DEVICE_STARTSEQ_STATE_COMMON_INITIALIZED:
        // Execute power sequence in common driver
        Result = DtuDevicePowerUpSeq(pDvcData);
        if (Result < 0)
        {
            DtuDeviceStopSeq(pDvcData, true);
            return Result;
        }
        
        // Next state in start sequence
        pDvcData->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_POWERED_UP;

    case DEVICE_STARTSEQ_STATE_POWERED_UP:
        // Register USB device
        Result = usb_register_dev(pDvcData->m_Device.m_pUsbItf, &DtuClass);

        if (Result < 0)
        {
            DtuDeviceStopSeq(pDvcData, true);
            DtuDecrUsageCount(pDvcData);
            return Result;
        }
        
        // Next state in start sequence
        pDvcData->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_USBDEV_REGISTERED;
    
    case DEVICE_STARTSEQ_STATE_USBDEV_REGISTERED:
    default :
        // Start sequence is done
        break;
    }
    
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDeviceStopSeq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtuDeviceStopSeq(DtuDeviceData* pDvcData, Bool Initializing)
{
    Bool  Registered = false;
    switch (pDvcData->m_IalData.m_StartSeqState)
    {
        case DEVICE_STARTSEQ_STATE_USBDEV_REGISTERED:
            Registered = true;
            
        case DEVICE_STARTSEQ_STATE_POWERED_UP:
            // Execute powerdown sequence in common driver
            DtuDevicePowerDownSeq(pDvcData, TRUE);
            
        case DEVICE_STARTSEQ_STATE_COMMON_INITIALIZED:
            // Execute DeviceExit in common driver
            DtuDeviceExit(pDvcData);
            
        case DEVICE_STARTSEQ_STATE_RES_INITIALIZED:
            // Free all Linux specific device resources
            DtuFreeDeviceResources(pDvcData);
            
        case DEVICE_STARTSEQ_STATE_IDLE:
        default:
            // Stop sequence is done
           break;
    }

     if (Registered && Initializing)
        DtuDecrUsageCount(pDvcData);
            
    // After DeviceStopSeq all resources are free
    pDvcData->m_IalData.m_StartSeqState = DEVICE_STARTSEQ_STATE_IDLE;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Character interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#if defined(CONFIG_COMPAT) && !defined(HAVE_COMPAT_PTR_IOCTL)
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuIoctlCompat -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Compatibility function for 32-bit applications using a 64-bit driver
//
long DtuIoctlCompat(
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
    return DtuIoctl(pInode, pFile, Cmd, (unsigned long)Arg);
}
#endif  // #ifdef CONFIG_COMPAT

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuUnlockedIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
long  DtuUnlockedIoctl(struct file* pFile, unsigned int Cmd, unsigned long Arg)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
    struct inode*  pInode = pFile->f_dentry->d_inode;
#else
    struct inode*  pInode = pFile->f_path.dentry->d_inode;
#endif
    return DtuIoctl(pInode, pFile, Cmd, Arg);
}

#if defined(RHEL_RELEASE_CODE)
#if RHEL_RELEASE_CODE>=RHEL_RELEASE_VERSION(8,1)
#define NEW_ACCESS_OK_MACRO_RHEL
#endif
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static long  DtuIoctl(
    struct inode*  pInode,
    struct file*  pFile,
    unsigned int  Cmd,
    unsigned long  Arg)
{
    DtStatus  Status;
    Int  Result = 0;
    DtFileObject  File;
    DtIoctlObject  Ioctl;
    DtuIoctlInputData  IoctlInputData; 
    DtuIoctlOutputData  IoctlOutputData; 

    char*  pIoctlInputData = NULL;
    char*  pIoctlOutputData = NULL;
    
    UInt  InputSizeParam = 0;
    UInt  OutputSizeParam = 0;
    UInt  ReservedForSizeParam = 0;
    DtPageList*  pPageListIn = NULL;
    DtPageList*  pPageListOut = NULL;

    DtDbgOut(MAX, IAL, "Start");

    // Check command that this is one of our commands
    if (DTU_IOCTL_MAGIC!=_IOC_TYPE(Cmd) && DTU_IOCTL_MAGIC_SIZE!=_IOC_TYPE(Cmd))
        Result = -ENOTTY;

    if (Result >= 0)
    {
        // Check device context
        if (pFile->private_data == NULL)
            Result = -EFAULT;
    }

    if (Result >= 0)
    {
        if(_IOC_TYPE(Cmd) == DTU_IOCTL_MAGIC_SIZE)
        {
            UInt*  pBufSizeLoc = (UInt*)Arg;

            ReservedForSizeParam = 2*sizeof(UInt);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0) && !defined(NEW_ACCESS_OK_MACRO_RHEL))
            if (access_ok(VERIFY_READ, (void*)Arg, ReservedForSizeParam) == 0)
#else
            if (access_ok((void*)Arg, ReservedForSizeParam) == 0)
#endif
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
            if (_IOC_TYPE(Cmd)==DTU_IOCTL_MAGIC && 
                                         Ioctl.m_OutputBufferSize>sizeof(IoctlOutputData))
                Ioctl.m_OutputBufferSize = sizeof(IoctlOutputData);

            DtDbgOut(MAX, IAL, "Output buffer size %d", Ioctl.m_OutputBufferSize);

            // Ioctl reads --> driver needs write access for user memory
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0) && !defined(NEW_ACCESS_OK_MACRO_RHEL))
            if (access_ok(VERIFY_WRITE, (void*)Arg, Ioctl.m_OutputBufferSize) == 0)
#else
            if (access_ok((void*)Arg, Ioctl.m_OutputBufferSize) == 0)
#endif
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
                }
                else
                    Ioctl.m_pOutputBuffer = NULL;
            } else
                Ioctl.m_pOutputBuffer = &IoctlOutputData;
        }

        if ((_IOC_DIR(Cmd)&_IOC_WRITE) != 0)
        {
            // Set input buffer size
            Ioctl.m_InputBufferSize = InputSizeParam;

            if (_IOC_TYPE(Cmd)==DTU_IOCTL_MAGIC && 
                                        Ioctl.m_InputBufferSize>sizeof(DtuIoctlInputData))
                Ioctl.m_InputBufferSize = sizeof(DtuIoctlInputData);
            
            DtDbgOut(MAX, IAL, "Input buffer size %d", Ioctl.m_InputBufferSize);
            
            // Ioctl writes --> driver needs read access for user memory
#if (LINUX_VERSION_CODE < KERNEL_VERSION(5, 0, 0) && !defined(NEW_ACCESS_OK_MACRO_RHEL))
            if (access_ok(VERIFY_READ, (void*)Arg,
                                       Ioctl.m_InputBufferSize+ReservedForSizeParam) == 0)
#else
            if (access_ok((void*)Arg,
                                     Ioctl.m_InputBufferSize + ReservedForSizeParam) == 0)
#endif
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
                }
                else
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
        Status = DtuDeviceIoctl(pFile->private_data, &File, &Ioctl);
        
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuOpen -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static Int  DtuOpen(struct inode* pInode, struct file* pFile)
{
    DtStatus  Status;
    DtFileObject  File;
    Int  Result = 0;
    Int  Minor;
    struct usb_interface*  pUsbItf;
    DtuDeviceData*  pDvcData;
    
    DtDbgOut(MAX, IAL, "Start");

    // Get the device index that is opened (Use minor number without offset)

    Minor = MINOR(pInode->i_rdev);
    
    pUsbItf = usb_find_interface(&DtuUsbOps, Minor);
    if (pUsbItf == NULL) 
    {
        DtDbgOut(ERR, IAL, "usb_find_interface failed");
        return -ENODEV;
    }

    pDvcData = usb_get_intfdata(pUsbItf);
    if (pDvcData == NULL)
    {
        DtDbgOut(ERR, IAL, "usb_get_infdata failed");
        return -ENODEV;
    }

    
    // Init File object
    File.m_pFile = pFile;

    // Store device data device data
    pFile->private_data = pDvcData;

    // Call common open
    Status = DtuDeviceOpen(pDvcData, &File);

    // Convert result
    if (!DT_SUCCESS(Status))
        Result = -EFAULT;
    
    if (Result >= 0)
        DtuIncrUsageCount(pFile->private_data);
    
    DtDbgOut(MAX, IAL, "Exit");
    return Result;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtuClose(struct inode* pInode, struct file* pFile)
{
    DtFileObject  File;

    DtDbgOut(MAX, IAL, "Start");

    // Check device context
    if (pFile->private_data == NULL)
        return -EFAULT;

    // Init File object
    File.m_pFile = pFile;

    // Release Non IP channel specific resources
    DtuNonIpReleaseResourceFromFileObject(pFile->private_data, &File);

    // Release Ip channel specific resources
    //DtuIpReleaseResourceFromFileObject(pFile->private_data, &File);

    // Call common close
    DtuDeviceClose(pFile->private_data, &File);

    DtuDecrUsageCount(pFile->private_data);
    
    // Clear private_data
    pFile->private_data = NULL;
    
    DtDbgOut(MAX, IAL, "Exit");
    
    return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuPoll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static UInt  DtuPoll(struct file* pFile, poll_table* pPollTable)
{
    Int  i;
    DtFileObject  File;
    UInt  Mask = 0;
    DtuEvents*  pDtuEvents = NULL;
    Int  NumPending = 0;
    DtuDeviceData*  pDvcData = (DtuDeviceData*)pFile->private_data;

    DtDbgOut(MAX, IAL, "Start");

    // Init File object
    File.m_pFile = pFile;

    // Add events wait_queue to poll table and get number of pending events
    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
    {
        DtSpinLockAcquire(&pDvcData->m_Events[i].m_Lock);
        if (pDvcData->m_Events[i].m_RefCount > 0)
        {
            if (DtFileCompare(&pDvcData->m_Events[i].m_File, &File))
            {
                pDtuEvents = &pDvcData->m_Events[i];
                poll_wait(pFile, &pDtuEvents->m_PendingEvent.m_WaitQueueHead, pPollTable);
                NumPending = pDtuEvents->m_NumPendingEvents;
            }
        }
        DtSpinLockRelease(&pDvcData->m_Events[i].m_Lock);
        
        if (pDtuEvents != NULL)
            break;
    }
    

    // Check if events are pending
    if (NumPending > 0)
    {
        DtDbgOut(MAX, IAL, "Pending Events found.");
        Mask |= POLLIN | POLLRDNORM;
    } else
        DtDbgOut(MAX, IAL, "No pending Events found.");

    DtDbgOut(MAX, IAL, "Exit");

    return Mask;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuIalRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static ssize_t  DtuIalRead(
    struct file*  pFile, 
    char __user*  pBuf, 
    size_t  Count, 
    loff_t*  pOffp)
{
    return -EFAULT; // Not supported for now
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuIalWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
//
ssize_t DtuIalWrite(
    struct file*  pFile,
    const char __user*  pBuf,
    size_t  TransferSize,
    loff_t*  pPos)
{
    return -EFAULT; // Not supported for now
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ System interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuProbe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtuProbe(struct usb_interface* pUsbItf, const struct usb_device_id* pUsbDevId)
{
    Int  Result = 0;
    UInt  DvcIndex;
    DtuDeviceData*  pDvcData = NULL;
    Bool  AttrCreated = false;
    
    DtDbgOut(MAX, IAL, "Start");

    // Check is it's a Dektec Device
    if (pUsbDevId->idVendor != DTU_VENDOR_ID)
        return -EBADR;
    
    // Quick check if maximum device count is reached
    if (g_DtuDeviceCount < g_DtuMaxDevices)
    {
        // Find free DvcIndex
        for (DvcIndex=0; DvcIndex<g_DtuMaxDevices; DvcIndex++)
        {
            if (g_pDtuDeviceData[DvcIndex] == NULL)
                break;
        }
        if (DvcIndex == g_DtuMaxDevices)
            Result = -ENOMEM;
    } else
        Result = -ENOMEM;
    
    // Only proceed if no errors
    if (Result >= 0)
    {
        // Allocate memory for device context
        pDvcData = kmalloc(sizeof(DtuDeviceData), GFP_KERNEL);
        if (pDvcData != NULL)
        {
            // Initialize device data to default values
            DtuInitDeviceData(pDvcData);
            
            // Store handle to pDvcData in array
            g_pDtuDeviceData[DvcIndex] = pDvcData;

            // Store DvcIndex in device data IAL data
            pDvcData->m_IalData.m_DvcIndex = DvcIndex;
        } else
            Result = -ENOMEM;
    }
    
    // Only proceed if no errors
    if (Result >= 0)
    {
        // Initialise reference count to 1.
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,9)
        kref_init(&pDvcData->m_IalData.m_KRef, DtuIalDevDelete);
#else
        kref_init(&pDvcData->m_IalData.m_KRef); // 2.6.9
#endif
        // Store pointer to this module and usb_dev in device data
        pDvcData->m_Driver.m_pModule = THIS_MODULE;
        DtEvtLogInit(&pDvcData->m_Driver.m_EvtObject, &EventGetMessage);
        pDvcData->m_Device.m_pUsbDev = usb_get_dev(interface_to_usbdev(pUsbItf));
        pDvcData->m_Device.m_pUsbItf = pUsbItf;
        DtEvtLogInit(&pDvcData->m_Device.m_EvtObject, &EventGetMessage);

        // Add serial attribute to device
        Result = device_create_file(&pUsbItf->dev, &dev_attr_serial);
        if (Result < 0)
            DtDbgOut(ERR, IAL, "device_create_file failed. Error: %i", Result);
        else
            AttrCreated = true;

    }
        
    // Only proceed if no errors
    if (Result >= 0)
    {
       Result = usb_set_interface(pDvcData->m_Device.m_pUsbDev, 0, 0);
       if (Result < 0)
            DtDbgOut(ERR, IAL, "usb_set_interface failed. Error: %i", Result);
    }
    
    // Only proceed if no errors
    if (Result >= 0)
        // Start DTU card
        Result = DtuDeviceStartSeq(pDvcData);
    
    // Only proceed if no errors
    if (Result >= 0)
    {
        // Set device driver data
        usb_set_intfdata(pUsbItf, pDvcData);

        // Finnaly increment device count
        g_DtuDeviceCount++;
        
        DtDbgOut(AVG, IAL, "New device count: %u", g_DtuDeviceCount);
    }

    // Cleanup if errors
    if (Result < 0)
    {
        if (AttrCreated)
            device_remove_file(&pUsbItf->dev, &dev_attr_serial);
        if (pDvcData != NULL)
        {
            // Free minor by clearing array entry
            g_pDtuDeviceData[pDvcData->m_IalData.m_DvcIndex] = NULL;
            
            // Free device data
            kfree(pDvcData);
        }
    }
    
    DtDbgOut(MAX, IAL, "Exit");
    
    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuRemove -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtuDisconnect(struct usb_interface* pUsbItf)
{
    DtuDeviceData*  pDvcData;
    
    DtDbgOut(MAX, IAL, "Start");

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,38)
    // Prevent DtuOpen from racing DtuDisconnect by a big kernel lock
    lock_kernel();
#endif

    // Get device data
    pDvcData = usb_get_intfdata(pUsbItf);
    if (pDvcData == NULL)
        return;
    
    // Clear device driver data
    usb_set_intfdata(pUsbItf, NULL);

    // Stop DTU card, do not decrement usage count. We do it after
    // deregistering device
    DtuDeviceStopSeq(pDvcData, false);

    // Deregister our device
    usb_deregister_dev(pUsbItf, &DtuClass);

    // Remove attributes from device
    device_remove_file(&pDvcData->m_Device.m_pUsbItf->dev, &dev_attr_serial);

#if LINUX_VERSION_CODE <= KERNEL_VERSION(2,6,38)
    // Unlock kerenel
    unlock_kernel();
#endif

    DtuDecrUsageCount(pDvcData);
    
    DtDbgOut(MAX, IAL, "Exit");
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuSuspend -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtuSuspend(struct usb_interface* pUsbItf, pm_message_t State)
{
    Int  Result = 0;
    DtuDeviceData*  pDvcData;
    
    DtDbgOut(MAX, IAL, "Start");

    // Get device data
    pDvcData = usb_get_intfdata(pUsbItf);
    if (pDvcData == NULL)
        return -ENODEV;

    // Execute powerdown sequence in common driver
    Result = DtuDevicePowerDownSeq(pDvcData, false);
    
    DtDbgOut(MAX, IAL, "Exit");

    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuResume -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtuResume(struct usb_interface* pUsbItf)
{
    Int  Result = 0;
    DtuDeviceData* pDvcData;
    
    DtDbgOut(MAX, IAL, "Start");

    // Get device data
    pDvcData = usb_get_intfdata(pUsbItf);
    if (pDvcData == NULL)
        return -ENODEV;

    // Execute powerup sequence in common driver
    Result = DtuDevicePowerUpSeq(pDvcData);
        
    DtDbgOut(MAX, IAL, "Exit");
    
    return Result;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Module init / exit +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuModuleInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Int  DtuModuleInit(void)
{
    Int  Result = 0;
    DtStatus  Status;
    DtEvtLog  EvtObject;
    
    DtDbgOut(MAX, IAL, "Start");

    // Only proceed if no errors
    if (Result >= 0)
    {
        // Allocate memory for device context pointers
        g_pDtuDeviceData = kmalloc(g_DtuMaxDevices * sizeof(DtuDeviceData*), GFP_KERNEL);
        if (g_pDtuDeviceData != NULL)
            memset(g_pDtuDeviceData, 0, g_DtuMaxDevices * sizeof(DtuDeviceData*));
        else
            Result = -EBUSY;
    }

    // Call init in common driver
    DtEvtLogInit(&EvtObject, &EventGetMessage);
    
    Status = DtuDriverInit(&EvtObject);
    if (!DT_SUCCESS(Status))
        Result = -EBUSY;
    
    if (Result >= 0)
        Result = usb_register(&DtuUsbOps);

    // Cleanup if errors
    if (Result < 0)
        DtuModuleExit();

    DtDbgOut(MAX, IAL, "Exit");
    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuModuleExit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtuModuleExit(void)
{
    DtDbgOut(MAX, IAL, "Start");

    // Unregister our system interface in the USB driver
    usb_deregister(&DtuUsbOps);
    
    if (g_DtuDeviceCount != 0)
        DtDbgOut(ERR, IAL, "DtuModuleExit called, but DtuDeviceCount is not 0 after"
                                                " usb_deregister (%u)", g_DtuDeviceCount);

    // Call exit in common driver
    DtuDriverExit();

    // Destroy device class
    
    // Free memory for device context pointers
    if (g_pDtuDeviceData != NULL)
        kfree(g_pDtuDeviceData);
    
    DtDbgOut(MAX, IAL, "Exit");
}

module_init(DtuModuleInit);
module_exit(DtuModuleExit);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuIncrUsageCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
//
// DtuIncrUsageCount -- increment use m_UsafeCount of this device
//
// Must be called when this devices is opened.
// Do not call this after creation.
//
void  DtuIncrUsageCount(DtuDeviceData* pDvcData)
{
    kref_get(&pDvcData->m_IalData.m_KRef);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDecrUsageCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DtuDecrUsageCount -- decrease use m_Count of this device
//
// Must be called when this device is closed or cleaned up (disconnect).
// When the reference m_Count = 0, all structures used will be freed.
//
void  DtuDecrUsageCount(DtuDeviceData* pDvcData)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,9)
    kref_put(&pDvcData->m_IalData.m_KRef);
#else
    kref_put(&pDvcData->m_IalData.m_KRef, DtuDevDelete); // >2.6.9
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDevDelete -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DtuDevDelete -- delete device structure
//
// Will be called when reference count = 0
//
void  DtuDevDelete(
    struct kref*  pKRef)
{
    DtuDeviceData*  pDvcData;
    DtuIalData*  pIalData;

    
    // Get pointer to DeviceData
    pIalData = container_of(pKRef, DtuIalData, m_KRef);
    if (pIalData == NULL)
        return;
    
    pDvcData = container_of(pIalData, DtuDeviceData, m_IalData);
    if (pDvcData == NULL)
        return;
    
    // Release link to USB device
    usb_put_dev(pDvcData->m_Device.m_pUsbDev);

    // Free minor by clearing array entry
    g_pDtuDeviceData[pDvcData->m_IalData.m_DvcIndex] = NULL;
    
    // Decrement device count
    g_DtuDeviceCount--;
    
    // Finally free the device data object
    kfree(pDvcData);
}

