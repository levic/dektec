//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dtu2xxMod.c *#*#*#*#*#*#*#*#* (C) 2000-2010 DekTec
//
// Implementation of driver entry point routine for Dtu2xx devices (DriverEntry)
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dtu2xx driver, please refer to the Dta1xx readme

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "Dtu2xx.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ USB device ID structure +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// This structure makes hotplug associate the driver with the right device ID's.
//
// NOTE: some devices have two ID: 1) for no firmware loaded 2) for firmware loaded
//
static struct usb_device_id Dtu2xxUsbIds[] = {
    { USB_DEVICE(DTU2XX_VENDORID, PID_DTU205_NOFIRMWARE) },
    { USB_DEVICE(DTU2XX_VENDORID, PID_DTU225_NOFIRMWARE) },
	{ USB_DEVICE(DTU2XX_VENDORID, PID_DTU234_NOFIRMWARE) },
	{ USB_DEVICE(DTU2XX_VENDORID, PID_DTU235_NOFIRMWARE) },
    { USB_DEVICE(DTU2XX_VENDORID, PID_DTU205) },
	{ USB_DEVICE(DTU2XX_VENDORID, PID_DTU215) },
    { USB_DEVICE(DTU2XX_VENDORID, PID_DTU225) },
	{ USB_DEVICE(DTU2XX_VENDORID, PID_DTU234) },
	{ USB_DEVICE(DTU2XX_VENDORID, PID_DTU235) },
	{ USB_DEVICE(DTU2XX_VENDORID, PID_DTU236) },
    { USB_DEVICE(DTU2XX_VENDORID, PID_DTU245) },
    {}
};
MODULE_DEVICE_TABLE(usb, Dtu2xxUsbIds);

static struct usb_driver Dtu2xxDriver =
{
	/* Only for pre-2.6.16 kernels. The '.owner' field has been removed. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 16)
    .owner = THIS_MODULE,
#endif
	.name = "Dtu2xx",
	.probe = Dtu2xxProbe,
	.disconnect = Dtu2xxDisconnect,
	.id_table = Dtu2xxUsbIds,
};

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ File operation structur +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

struct file_operations Dtu2xxFops =
{
    .owner = THIS_MODULE,
    .llseek = no_llseek,
    .read = Dtu2xxRead,
    .write = Dtu2xxWrite,
    .ioctl = Dtu2xxDeviceControl,
#ifdef CONFIG_COMPAT
	.compat_ioctl =	Dtu2xxCompatDeviceControl,
#endif
    .open = Dtu2xxOpen,
    .release = Dtu2xxRelease,
};

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ USB classes +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define _DTU2XX_DRIVER_PATH "usb/DekTec/Dtu2xx-%d"

// DTU-205
static struct usb_class_driver Dtu205Class =
{
    .name = _DTU2XX_DRIVER_PATH,
    .fops = &Dtu2xxFops,
	/* Only for pre-2.6.12 kernels. The '.mode' field has been removed. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 12)
    .mode = S_IFCHR | S_IWUSR | S_IWGRP | S_IWOTH,
#endif
    .minor_base = DTU2XX_MINOR,
};

// DTU-215
static struct usb_class_driver Dtu215Class =
{
    .name = _DTU2XX_DRIVER_PATH,
    .fops = &Dtu2xxFops,
	/* Only for pre-2.6.12 kernels. The '.mode' field has been removed. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 12)
    .mode = S_IFCHR | S_IWUSR | S_IWGRP | S_IWOTH,
#endif
    .minor_base = DTU2XX_MINOR,
};

// DTU-225
static struct usb_class_driver Dtu225Class =
{
    .name = _DTU2XX_DRIVER_PATH,
    .fops = &Dtu2xxFops,
	/* Only for pre-2.6.12 kernels. The '.mode' field has been removed. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 12)
    .mode = S_IFCHR | S_IRUSR | S_IRGRP | S_IROTH,
#endif
    .minor_base = DTU2XX_MINOR,
};

// DTU-234
static struct usb_class_driver Dtu234Class =
{
    .name = _DTU2XX_DRIVER_PATH,
    .fops = &Dtu2xxFops,
	/* Only for pre-2.6.12 kernels. The '.mode' field has been removed. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 12)
    .mode = S_IFCHR | S_IRUSR | S_IRGRP | S_IROTH,
#endif
    .minor_base = DTU2XX_MINOR,
};

// DTU-235
static struct usb_class_driver Dtu235Class =
{
    .name = _DTU2XX_DRIVER_PATH,
    .fops = &Dtu2xxFops,
	/* Only for pre-2.6.12 kernels. The '.mode' field has been removed. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 12)
    .mode = S_IFCHR | S_IRUSR | S_IRGRP | S_IROTH,
#endif
    .minor_base = DTU2XX_MINOR,
};

// DTU-236
static struct usb_class_driver Dtu236Class =
{
    .name = _DTU2XX_DRIVER_PATH,
    .fops = &Dtu2xxFops,
	/* Only for pre-2.6.12 kernels. The '.mode' field has been removed. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 12)
    .mode = S_IFCHR | S_IRUSR | S_IRGRP | S_IROTH,
#endif
    .minor_base = DTU2XX_MINOR,
};

// DTU-245
static struct usb_class_driver Dtu245Class =
{
    .name = _DTU2XX_DRIVER_PATH,
    .fops = &Dtu2xxFops,
	/* Only for pre-2.6.12 kernels. The '.mode' field has been removed. */
#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 12)
    .mode = S_IFCHR | S_IRUSR | S_IRGRP | S_IROTH | S_IWOTH,
#endif
    .minor_base = DTU2XX_MINOR,
};

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ File operation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxOpen -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Dtu2xxOpen - open a device; called upon a open systemcall
//
// registered as file operation
// a device can only be opened by 1 process
//
int Dtu2xxOpen(
    struct inode* inode,
    struct file *file)
{
    Int minor;
    struct usb_interface *pUsbItf;
    DTU2XX_FDO *pFdo;

#if LOG_LEVEL > 0
	DTU2XX_LOG( KERN_INFO, "Dtu2xxOpen: ENTER" );
#endif

	// Store minor number
    minor = iminor(inode);

	// Get USB interface
    pUsbItf = usb_find_interface(&Dtu2xxDriver, minor);
    if (!pUsbItf) {
		DTU2XX_LOG( KERN_INFO, "Dtu2xxOpen: usb_find_interface failed" );
		return -ENODEV;
	}

	// Get functional device object
    pFdo = usb_get_intfdata(pUsbItf);
    if (!pFdo) {
		DTU2XX_LOG( KERN_INFO, "Dtu2xxOpen: usb_get_intfdata failed" );
		return -ENODEV;
	}

	// Increment usage count
    Dtu2xxIncrUsageCount(pFdo);
	// Store FDO with file-object
    file->private_data = pFdo;
    return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRelease -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Dtu2xxRelease - close device; called upon close systemcall
//
int Dtu2xxRelease(
    struct inode* inode,
    struct file *file)
{
	Int Status=0;
    DTU2XX_FDO *pFdo;

#if LOG_LEVEL > 0
	DTU2XX_LOG( KERN_INFO, "Dtu2xxRelease: ENTER" );
#endif

	// Get our functional-device-object
    pFdo = (DTU2XX_FDO*)(file->private_data);
    if (!pFdo)
		return -ENODEV;

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Release exclusive access -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	// NOTE: we donot care about the return value in this case
	Status = Dtu2xxCheckExclusiveAccess(pFdo, file);

    Dtu2xxDecrUsageCount(pFdo);
    //atomic_inc(&pFdo->m_Count);
    return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Dtu2xxRead - bulk read from device; called upon a read systemcall
//
ssize_t Dtu2xxRead(
    struct file *file,
    char __user *pBuf,
    size_t TransferSize,
    loff_t *ppos)
{
	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Dtu2xxWrite - bulk write to device; called upon a write systemcall
//
ssize_t Dtu2xxWrite(
    struct file *file,
    const char __user *pBuf,
    size_t TransferSize,
    loff_t *ppos)
{
	return -EFAULT;	// Not supported for now
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Device operations +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxProbe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Dtu2xxProbe - probe for a new device; called when a usb-device is inserted
//
int Dtu2xxProbe(
    struct usb_interface *pUsbItf,
    const struct usb_device_id *pUsbDevId)
{
	Int Status=0;
    DTU2XX_FDO* pFdo=NULL;
	struct usb_device_descriptor* pDvcDesc=NULL;

#if LOG_LEVEL > 0
	DTU2XX_LOG( KERN_INFO, "Dtu2xxProbe: ENTER" );
#endif

	// We must have a DekTec device
	if (pUsbDevId->idVendor != DTU2XX_VENDORID)
		return -EBADR;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Create FDO object -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	// Allocate memory for FDO object
	pFdo = kmalloc(sizeof(DTU2XX_FDO), GFP_KERNEL);
    if ( pFdo == NULL )
		return -ENOMEM;
	// Init structure to '0'
	memset(pFdo, 0, sizeof(DTU2XX_FDO));

	// The Dtu2xx device is not started or removed
	pFdo->m_Started = FALSE;
	pFdo->m_Removed = FALSE;
	// Assume no EzUsb firmware and no DTU-2x5 hardware
	pFdo->m_EzUsbFirmware = FALSE;
	pFdo->m_IsDtu2x5Hw = FALSE;

	// Init other members to initial 'safe' values
	pFdo->m_pUsbItf = NULL;
    pFdo->m_pUsbClass = NULL;
	pFdo->m_pVpdEeprom = NULL;
	pFdo->m_NumChannels = 0;

#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,9)
    kref_init(&pFdo->m_KRef, Dtu2xxDevDelete);
#else
    kref_init(&pFdo->m_KRef);	// 2.6.9
#endif

	// Set device and interface members
	pFdo->m_pUDev		= usb_get_dev(interface_to_usbdev(pUsbItf));
    pFdo->m_pUsbItf		= pUsbItf;
	usb_set_intfdata(pUsbItf, pFdo);

	// Get pointer to the device descriptor
	pDvcDesc = &(pFdo->m_pUDev->descriptor);

	//.-.-.-.-.-.-.-.-.-.-.- Check which device we are dealing with -.-.-.-.-.-.-.-.-.-.-.

    // Check for new 2x5 hardware ( product version >=5 => DTU-2x5)
    if ( pDvcDesc->bcdDevice >= 5 )
		pFdo->m_IsDtu2x5Hw = TRUE;
    else
		pFdo->m_IsDtu2x5Hw = FALSE;

	switch ( pDvcDesc->idProduct )
	{
	case PID_DTU205_NOFIRMWARE:
	case PID_DTU205:
        // Found a DTU-205
		pFdo->m_TypeNumber = 205;
		pFdo->m_pUsbClass = &Dtu205Class;
		// Check if EzUSb (FX2) firmware has been loaded
		pFdo->m_EzUsbFirmware = Dtu2xxIsFwLoaded(pFdo);
		//if ( pDvcDesc->idProduct == PID_DTU205 )
		//	pFdo->m_EzUsbFirmware = TRUE;
		break;

	case PID_DTU215:
		// // Found a DTU-215
		pFdo->m_TypeNumber = 215;
		pFdo->m_pUsbClass = &Dtu215Class;
		// Check if EzUSb (FX2) firmware has been loaded
		pFdo->m_EzUsbFirmware = Dtu2xxIsFwLoaded(pFdo);
		break;

	case PID_DTU225_NOFIRMWARE_OLD:
	case PID_DTU225_NOFIRMWARE:
	case PID_DTU225_OLD:
	case PID_DTU225:
		// Found a DTU-225
		pFdo->m_TypeNumber = 225;
		pFdo->m_pUsbClass = &Dtu225Class;
		// Check if EzUSb (FX2) firmware has been loaded
		pFdo->m_EzUsbFirmware = Dtu2xxIsFwLoaded(pFdo);
		//if ( pDvcDesc->idProduct == PID_DTU225 )
		//	pFdo->m_EzUsbFirmware = TRUE;
		break;

	case PID_DTU234_NOFIRMWARE:
	case PID_DTU234:
		// Found a DTU-234
		pFdo->m_TypeNumber = 234;
		pFdo->m_pUsbClass = &Dtu234Class;
		// Check if EzUSb (FX2) firmware has been loaded
		pFdo->m_EzUsbFirmware = Dtu2xxIsFwLoaded(pFdo);
		//if ( pDvcDesc->idProduct == PID_DTU234 )
		//	pFdo->m_EzUsbFirmware = TRUE;
		break;

	case PID_DTU235_NOFIRMWARE:
	case PID_DTU235:
		// Found a DTU-235
		pFdo->m_TypeNumber = 235;
		pFdo->m_pUsbClass = &Dtu235Class;
		// Check if EzUSb (FX2) firmware has been loaded
		pFdo->m_EzUsbFirmware = Dtu2xxIsFwLoaded(pFdo);
		//if ( pDvcDesc->idProduct == PID_DTU235 )
		//	pFdo->m_EzUsbFirmware = TRUE;
		break;

	case PID_DTU236:
		// Found a DTU-236
		pFdo->m_TypeNumber = 236;
		pFdo->m_pUsbClass = &Dtu236Class;
		// Check if EzUSb (FX2) firmware has been loaded
		pFdo->m_EzUsbFirmware = Dtu2xxIsFwLoaded(pFdo);
		break;

	case PID_DTU245:
		// Found a DTU-245
		pFdo->m_TypeNumber = 245;
		pFdo->m_pUsbClass = &Dtu245Class;
		// Check if EzUSb (FX2) firmware has been loaded
		pFdo->m_EzUsbFirmware = Dtu2xxIsFwLoaded(pFdo);
		break;

	default:
		// Unknown device
		pFdo->m_TypeNumber = 0;
		break;
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Init USB specific -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	Status = usb_register_dev(pUsbItf, pFdo->m_pUsbClass);
	if ( Status != 0 ) {
		DTU2XX_LOG( KERN_INFO, "Dtu2xxProbe: Failed to register USB device (DTU-%d)",
					pFdo->m_TypeNumber );
		Dtu2xxDecrUsageCount(pFdo);
		return Status;
	}

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- Download the EzUsb Firmware -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	// Only download when a uninitilised device was are encountered
	//
	if ( !pFdo->m_EzUsbFirmware )
	{
#if LOG_LEVEL > 0
		DTU2XX_LOG( KERN_INFO, "Dtu2xxProbe: Program FX2 firmware (DTU-%d, Is2x5Hw=%d)",
					pFdo->m_TypeNumber, pFdo->m_IsDtu2x5Hw);
#endif

		switch ( pFdo->m_TypeNumber )
		{
		case 205:
			Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_USBITF | DTU2XX_FWID_DTU205);
			break;
		case 215:
			Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_USBITF | DTU2XX_FWID_DTU215);
			break;
		case 225:
			Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_USBITF | DTU2XX_FWID_DTU225);
			break;
		case 234:
			Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_USBITF | DTU2XX_FWID_DTU234);
			break;
		case 235:
			Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_USBITF | DTU2XX_FWID_DTU235);
			break;
		case 236:
			Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_USBITF | DTU2XX_FWID_DTU236);
			break;
		case 245:
			Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_USBITF | DTU2XX_FWID_DTU245);
			break;
		default:
			break;
		}

#if LOG_LEVEL > 0
		if ( Status!=0 )
			DTU2XX_LOG(KERN_INFO, "Dtu2xxProbe: Failed to program the EzUsb firmware");
		else
			DTU2XX_LOG(KERN_INFO, "Dtu2xxProbe: Completed programming the EzUsb firmware");
#endif
		// Clear CPU reset (not really needed)
		Dtu2xx8051Reset(pFdo, 0);

		// We will now re-enumerate (i.e. driver will be unloaded a re-loaded for new PID)
		return 0;
	}

	// Store ID information
	pFdo->m_UsbAddress = Dtu2xxGetDeviceAddress(pFdo);
	pFdo->m_VendorId = pDvcDesc->idVendor;
	pFdo->m_DeviceId = pDvcDesc->idProduct;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Init Device -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	Status = Dtu2xxInitDevice(pFdo);
	if ( Status != 0 ) {
		Dtu2xxDecrUsageCount(pFdo);
		return Status;
	}

	DTU2XX_LOG( KERN_INFO, "Found DTU-%d device (FW version=%d), minor nb=%d",
                pFdo->m_TypeNumber, pFdo->m_FirmwareVersion, pFdo->m_pUsbClass->minor_base);
	DTU2XX_LOG( KERN_INFO, "Dtu2xx driver (v%d.%d.%d.%d) started sucessfully",
				DTU2XX_VERSION_MAJOR, DTU2XX_VERSION_MINOR, DTU2XX_VERSION_MICRO,
				DTU2XX_VERSION_BUILD );

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxDisconnect -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Dtu2xxDisconnect - disconnect device, called when the usb-device is disconnected.
//
void Dtu2xxDisconnect(
    struct usb_interface *pUsbItf)
{
    DTU2XX_FDO* pFdo=NULL;

#if LOG_LEVEL > 0
	DTU2XX_LOG( KERN_INFO, "Dtu2xxDisconnect: ENTER" );
#endif

    // Prevent Dtu2xxOpen from racing Dtu2xxDisconnect by a big kernel lock
    lock_kernel();

	// Get our functional-device-object
	pFdo = usb_get_intfdata(pUsbItf);
	// Release USB interface
    usb_set_intfdata(pUsbItf, NULL);
	// Deregister our device
    usb_deregister_dev(pUsbItf, pFdo->m_pUsbClass);

	// Unlock kerenel
    unlock_kernel();

    Dtu2xxDecrUsageCount(pFdo);
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Module operations +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxInitModule -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// module init; called when the driver is loaded
//
int __init Dtu2xxInitModule(void)
{
#if LOG_LEVEL > 0
	DTU2XX_LOG( KERN_INFO, "Dtu2xxInitModule: ENTER" );
#endif
    return usb_register(&Dtu2xxDriver);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxCleanupModule -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// module cleanup; called when the driver is unloaded
//
void __exit Dtu2xxCleanupModule(void)
{
#if LOG_LEVEL > 0
	DTU2XX_LOG( KERN_INFO, "Dtu2xxCleanupModule: ENTER" );
#endif
    usb_deregister(&Dtu2xxDriver);
}

module_init(Dtu2xxInitModule);
module_exit(Dtu2xxCleanupModule);

MODULE_AUTHOR("DekTec Digital Video B.V.");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Hardware driver for Dtu2xx usb-devices");

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Internal helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxIncrUsageCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
//
// Dtu2xxIncrUsageCount -- increment use m_UsafeCount of this device
//
// Must be called when this devices is opened.
// Do not call this after creation.
//
void Dtu2xxIncrUsageCount(
    DTU2XX_FDO* pFdo)
{
    kref_get(&pFdo->m_KRef);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxDecrUsageCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Dtu2xxDecrUsageCount -- decrease use m_Count of this device
//
// Must be called when this device is closed or cleaned up (disconnect).
// When the reference m_Count = 0, all structures used will be freed.
//
void Dtu2xxDecrUsageCount(
    DTU2XX_FDO* pFdo)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,9)
    kref_put(&pFdo->m_KRef);
#else
    kref_put(&pFdo->m_KRef, Dtu2xxDevDelete); // >2.6.9
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxDevDelete -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Dtu2xxDevDelete -- delete device structure
//
// Will be called when reference m_UsageCount = 0
//
void Dtu2xxDevDelete(
    struct kref* kref)
{
	Int i;
    DTU2XX_FDO* pFdo=NULL;
	Channel* pCh=NULL;

	// Get FDO pointer
    pFdo = container_of(kref, DTU2XX_FDO, m_KRef);
	if ( pFdo == NULL )
		return;

#if LOG_LEVEL > 0
	DTU2XX_LOG( KERN_INFO,
			    "Dtu2xxDevDelete: Performing final USB device clean-up (DTU-%d)",
				pFdo->m_TypeNumber );
#endif

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Free resources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	// Free bulk-transfer resources
	for ( i=0; i<pFdo->m_NumChannels; i++ )
	{
		pCh = &pFdo->m_Channel[i];

		if ( pCh->m_BulkTransf.m_Buffer.m_pStart != NULL )
			kfree(pCh->m_BulkTransf.m_Buffer.m_pStart);

		pCh->m_BulkTransf.m_Buffer.m_pStart	= NULL;
		pCh->m_BulkTransf.m_Buffer.m_pRdPtr	= NULL;
		pCh->m_BulkTransf.m_Buffer.m_pWrPtr	= NULL;
		pCh->m_BulkTransf.m_Buffer.m_Load = 0;
		pCh->m_BulkTransf.m_Buffer.m_TotalSize = 0;

		if ( pCh->m_pTempBuf != NULL )
			kfree(pCh->m_pTempBuf);
		pCh->m_pTempBuf = NULL;
		pCh->m_TempBufSize = 0;
	}

	// Release link to USB device
	usb_put_dev(pFdo->m_pUDev);

	// Finally free the FDO object
    kfree(pFdo);
}
