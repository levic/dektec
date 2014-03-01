//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DeviceIoControl.c *#*#*#*#*#*#*# (C) 2000-2008 DekTec
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
#include "Dtu2xxRegs.h"
#include "EzUsb.h"

#ifdef CONFIG_COMPAT
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxCompatDeviceControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Compatibility for 32-bit applications calling IOCTL in a 64-bit driver
//
long Dtu2xxCompatDeviceControl(struct file *file, unsigned int cmd, unsigned long arg)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
	struct inode *inode = file->f_dentry->d_inode;
#else
	struct inode *inode = file->f_path.dentry->d_inode;
#endif

	// Just call the regular IOCTL handler; it should be 32-/64-bit clean
	return Dtu2xxDeviceControl(inode, file, cmd, (unsigned long)compat_ptr(arg));
}
#endif	// #ifdef CONFIG_COMPAT

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxDeviceControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Process IRP_MJ_DEVICE_CONTROL.
//
Int  Dtu2xxDeviceControl(
	struct inode *inode,
	struct file *filp,		// Represents file object making the function call
	unsigned int cmd,		// Command
	unsigned long arg)		// Pointer to the IoCtl parameter

{
	Int  PortIndex, ChannelType, Dummy;
	char*  IoctlStr;				// Mnemonic string for IOCTL code
    UInt  InBufSize=0;				// Amount of data avail. from caller.
	UInt  OutBufSize=0;			// Max data that caller can accept.
	UInt  IoReqSize;				// Required length of IO buffer
	Int  ReturnStatus=0;
	DTU2XX_IOCTL_DATA  IoData;
	PDTU2XX_FDO  pFdo = (PDTU2XX_FDO)filp->private_data;	// Our functional-device-object

    // Check command that this is one of our commands
	if ( DTU2XX_IOCTL_MAGIC!=_IOC_TYPE(cmd) )
		return -ENOTTY;

	// Check memory
	if (  (_IOC_DIR(cmd)&_IOC_READ) != 0 ) {
		OutBufSize = _IOC_SIZE(cmd);
		if ( 0==access_ok(VERIFY_WRITE, (void*)arg, _IOC_SIZE(cmd)) )
            return -EFAULT;
	}
	if ( (_IOC_DIR(cmd)&_IOC_WRITE) != 0 ) {
		InBufSize = _IOC_SIZE(cmd);
		if ( 0==access_ok(VERIFY_READ, (void*)arg, _IOC_SIZE(cmd)) )
            return -EFAULT;
	}

	// Copy IoCtl data from user
	if ( copy_from_user(&IoData, (DTU2XX_IOCTL_DATA*)arg, _IOC_SIZE(cmd)) )
	{
		DTU2XX_LOG( KERN_INFO, "Dtu2xxDeviceControl: could not copy data from user" );
		return -EFAULT;
	}


	//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ FIRST BIG SWITCH +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

	// Determine expected length of data input and output buffer
	// For debug output: set IoctlStr to mnemonic representation of IOCTL

	// Determine expected length of data buffer
	IoReqSize = 0;
	switch (cmd)
	{
	case IOCTL_DTU2XX_RESET_SOFT:
		IoctlStr = "IOCTL_DTU2XX_RESET_SOFT";
		break;
	case IOCTL_DTU2XX_GET_DRIVER_VERSION:
		IoctlStr = "IOCTL_DTU2XX_GET_DRIVER_VERSION";
		IoReqSize = sizeof(DTU2XX_DRIVER_VERSION);
		break;
	case IOCTL_DTU2XX_GET_LOC_INFO:
		IoctlStr = "IOCTL_DTU2XX_GET_LOC_INFO";
		IoReqSize = sizeof(DTU2XX_LOC_INFO);
		break;
	case IOCTL_DTU2XX_LED_CONTROL:
		IoctlStr = "IOCTL_DTU2XX_LED_CONTROL";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_READ_REGISTER:
		IoctlStr = "IOCTL_DTU2XX_READ_REGISTER";
		IoReqSize = sizeof(DTU2XX_REG_INFO);
		break;
	case IOCTL_DTU2XX_WRITE_REGISTER:
		IoctlStr = "IOCTL_DTU2XX_WRITE_REGISTER";
		IoReqSize  = sizeof(DTU2XX_REG_INFO);
		break;
	case IOCTL_DTU2XX_GET_DEVICE_INFO:
		IoctlStr = "IOCTL_DTU2XX_GET_DEVICE_INFO";
		IoReqSize  = sizeof(DTU2XX_DVC_INFO);
		break;
	case IOCTL_DTU2XX_REQUEST_EXCL_ACCESS:
		IoctlStr = "IOCTL_DTU2XX_REQUEST_EXCL_ACCESS";
		IoReqSize = sizeof(DTU2XX_EXCL_ACCESS_INFO);
		break;
	case IOCTL_DTU2XX_READ_VPD:
		IoctlStr = "IOCTL_DTU2XX_READ_VPD";
		IoReqSize = sizeof(DTU2XX_VPD_TRANSFER);
		break;
	case IOCTL_DTU2XX_WRITE_VPD:
		IoctlStr = "IOCTL_DTU2XX_WRITE_VPD";
		IoReqSize = sizeof(DTU2XX_VPD_TRANSFER);
		break;
	case IOCTL_DTU2XX_VPD_DATA:
		IoctlStr = "IOCTL_DTU2XX_VPD_DATA";
		IoReqSize = sizeof(DTU2XX_VPD_DATA);
		break;
	case IOCTL_DTU2XX_I2C_WRITE:
		IoctlStr = "IOCTL_DTU2XX_I2C_WRITE";
		IoReqSize = sizeof(DTU2XX_I2C_TRANSFER);
		break;
	case IOCTL_DTU2XX_I2C_READ:
		IoctlStr = "IOCTL_DTU2XX_I2C_READ";
		IoReqSize = sizeof(DTU2XX_I2C_TRANSFER);
		break;
	case IOCTL_DTU2XX_GET_USB_SPEED:
		IoctlStr = "IOCTL_DTU2XX_GET_USB_SPEED";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_EEPROM_CONTROL:
		IoctlStr = "IOCTL_DTU2XX_EEPROM_CONTROL";
		IoReqSize = sizeof(DTU2XX_EEPROM_CONTROL);
		break;
	case IOCTL_DTU2XX_UPLOAD_FIRMWARE:
		IoctlStr = "IOCTL_DTU2XX_UPLOAD_FIRMWARE";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_GET_IOCONFIG:
		IoctlStr = "IOCTL_DTU2XX_GET_IOCONFIG";
		IoReqSize = sizeof(DTU2XX_IOCONFIG);
		break;
	case IOCTL_DTU2XX_SET_IOCONFIG:
		IoctlStr = "IOCTL_DTU2XX_SET_IOCONFIG";
		IoReqSize = sizeof(DTU2XX_IOCONFIG);
		break;
	case IOCTL_DTU2XX_RX_LED_CONTROL:
		IoctlStr = "IOCTL_DTU2XX_RX_LED_CONTROL";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_RX_EQUALISE:
		IoctlStr = "IOCTL_DTU2XX_RX_EQUALISE";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_RX_GET_FIFO_LOAD:
		IoctlStr = "IOCTL_DTU2XX_RX_GET_FIFO_LOAD";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_RX_GET_FLAGS:
		IoctlStr = "IOCTL_DTU2XX_RX_GET_FLAGS";
		IoReqSize = sizeof(DTU2XX_FLAGS);
		break;
	case IOCTL_DTU2XX_RX_CLEAR_FLAGS:
		IoctlStr = "IOCTL_DTU2XX_RX_CLEAR_FLAGS";
		IoReqSize = sizeof(DTU2XX_FLAGS);
		break;
	case IOCTL_DTU2XX_RX_GET_STATUS:
		IoctlStr = "IOCTL_DTU2XX_RX_GET_FIFO_LOAD";
		IoReqSize = sizeof(DTU2XX_RX_STATUS);
		break;
	case IOCTL_DTU2XX_RX_GET_TS_RATE_BPS:
		IoctlStr = "IOCTL_DTU2XX_RX_GET_TS_RATE_BPS";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_RX_RESET:
		IoctlStr = "IOCTL_DTU2XX_RX_RESET";
		IoReqSize = sizeof(DTU2XX_RST_INFO);
		break;
	case IOCTL_DTU2XX_RX_SET_RX_POLARITY:
		IoctlStr = "IOCTL_DTU2XX_RX_SET_RX_POLARITY";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_RX_GET_MAX_FIFO_SIZE:
		IoctlStr = "IOCTL_DTU2XX_RX_GET_MAX_FIFO_SIZE";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_RX_SET_LOOPBACK_MODE:
		IoctlStr = "IOCTL_DTU2XX_RX_SET_LOOPBACK_MODE";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_RX_SET_RX_CONTROL:
		IoctlStr = "IOCTL_DTU2XX_RX_SET_RX_CONTROL";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_RX_GET_RX_CONTROL:
		IoctlStr = "IOCTL_DTU2XX_RX_GET_RX_CONTROL";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_RX_SET_RX_MODE:
		IoctlStr = "IOCTL_DTU2XX_RX_SET_RX_MODE";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_RX_GET_RX_MODE:
		IoctlStr = "IOCTL_DTU2XX_RX_GET_RX_MODE";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_RX_GET_VIOL_COUNT:
		IoctlStr = "IOCTL_DTU2XX_RX_GET_VIOL_COUNT";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_RX_BULK_USERMEM_READ:
		IoctlStr = "IOCTL_DTU2XX_RX_BULK_USERMEM_READ";
		IoReqSize = sizeof(DTU2XX_RW_INFO);
		break;
#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTU2XX_RX_BULK_USERMEM_READ_32:
		IoctlStr = "IOCTL_DTU2XX_RX_BULK_USERMEM_READ_32";
		IoReqSize = sizeof(DTU2XX_RW_INFO_32);
		break;
#endif	// #if defined(CONFIG_X86_64)
	case IOCTL_DTU2XX_TX_CLEAR_FLAGS:
		IoctlStr = "IOCTL_DTU2XX_TX_CLEAR_FLAGS";
		IoReqSize = sizeof(DTU2XX_FLAGS);
		break;
	case IOCTL_DTU2XX_TX_GET_FIFO_LOAD:
		IoctlStr = "IOCTL_DTU2XX_TX_GET_FIFO_LOAD";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_TX_GET_FLAGS:
        IoctlStr = "IOCTL_DTU2XX_TX_GET_FLAGS";
		IoReqSize = sizeof(DTU2XX_FLAGS);
		break;
	case IOCTL_DTU2XX_TX_GET_TS_RATE_BPS:
		IoctlStr = "IOCTL_DTU2XX_TX_GET_TS_RATE_BPS";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_TX_GET_TX_CONTROL:
		IoctlStr = "IOCTL_DTU2XX_TX_GET_TX_CONTROL";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_TX_GET_TX_MODE:
		IoctlStr = "IOCTL_DTU2XX_TX_GET_TX_MODE";
		IoReqSize = sizeof(DTU2XX_TX_MODE);
		break;
	case IOCTL_DTU2XX_TX_GET_MAX_FIFO_SIZE:
		IoctlStr = "IOCTL_DTU2XX_TX_GET_MAX_FIFO_SIZE";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_TX_GET_FIFO_SIZE:
		IoctlStr = "IOCTL_DTU2XX_TX_GET_FIFO_SIZE";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_TX_RD_LOOPBACK_DATA:
		IoctlStr = "IOCTL_DTU2XX_TX_RD_LOOPBACK_DATA";
		//IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_TX_RESET:
		IoctlStr = "IOCTL_DTU2XX_TX_RESET";
		IoReqSize = sizeof(DTU2XX_RST_INFO);
		break;
	case IOCTL_DTU2XX_TX_SET_TX_POLARITY:
		IoctlStr = "IOCTL_DTU2XX_TX_SET_TX_POLARITY";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_TX_SET_FIFO_SIZE:
		IoctlStr = "IOCTL_DTU2XX_TX_SET_FIFO_SIZE";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_TX_SET_LOOPBACK_MODE:
		IoctlStr = "IOCTL_DTU2XX_TX_SET_LOOPBACK_MODE";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_TX_SET_TS_RATE_BPS:
		IoctlStr = "IOCTL_DTU2XX_TX_SET_TS_RATE_BPS";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_TX_SET_TX_CONTROL:
		IoctlStr = "IOCTL_DTU2XX_TX_SET_TX_CONTROL";
		IoReqSize = sizeof(UInt);
		break;
	case IOCTL_DTU2XX_TX_SET_TX_MODE:
		IoctlStr = "IOCTL_DTU2XX_TX_SET_TX_MODE";
		IoReqSize = sizeof(DTU2XX_TX_MODE);
		break;
	case IOCTL_DTU2XX_TX_BULK_USERMEM_WRITE:
		IoctlStr = "IOCTL_DTU2XX_TX_BULK_USERMEM_WRITE";
		IoReqSize = sizeof(DTU2XX_RW_INFO);
		break;
#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTU2XX_TX_BULK_USERMEM_WRITE_32:
		IoctlStr = "IOCTL_DTU2XX_TX_BULK_USERMEM_WRITE_32";
		IoReqSize = sizeof(DTU2XX_RW_INFO_32);
		break;
#endif	// #if defined(CONFIG_X86_64)
	default:
		IoctlStr = "??UNKNOWN IOCTL CODE??";
		break;
	}

#if LOG_LEVEL > 2
	DTU2XX_LOG( KERN_INFO, "Dtu2xxDeviceControl: IoCtl call received: %s (0x%08X)",
				IoctlStr, cmd );
#endif

	// Check buffer size
	if ( _IOC_SIZE(cmd) < IoReqSize )
	{
#if LOG_LEVEL > 0
		DTU2XX_LOG( KERN_INFO, "Dtu2xxDeviceControl: BufSize=%d (Rq=%d) IO BUFFER IS TOO "
					"SMALL!!", _IOC_SIZE(cmd), IoReqSize );
#endif
		return -EINVAL;
	}

	//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ SECOND BIG SWITCH +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

	// Determine ChannelType and PortIndex

	// Default value is 'device-level' (instead of port-level)
	ChannelType = PortIndex = -1;

	switch (cmd)
	{

	//.-.-.-.-.-.-.-.- General commands not related to a specific channel -.-.-.-.-.-.-.-.

	case IOCTL_DTU2XX_RESET_SOFT:
	case IOCTL_DTU2XX_GET_DRIVER_VERSION:
	case IOCTL_DTU2XX_GET_LOC_INFO:
	case IOCTL_DTU2XX_LED_CONTROL:
	case IOCTL_DTU2XX_READ_REGISTER:
	case IOCTL_DTU2XX_WRITE_REGISTER:
	case IOCTL_DTU2XX_GET_DEVICE_INFO:
	case IOCTL_DTU2XX_READ_VPD:
	case IOCTL_DTU2XX_WRITE_VPD:
	case IOCTL_DTU2XX_VPD_DATA:
	case IOCTL_DTU2XX_I2C_WRITE:
	case IOCTL_DTU2XX_I2C_READ:
	case IOCTL_DTU2XX_GET_USB_SPEED:
	case IOCTL_DTU2XX_EEPROM_CONTROL:
	case IOCTL_DTU2XX_UPLOAD_FIRMWARE:
		PortIndex = -1; ChannelType = -1;
		break;

	//.-.-.-.-.-.-.-.- General commands not related to a specific channel -.-.-.-.-.-.-.-.

	case IOCTL_DTU2XX_GET_IOCONFIG:
		PortIndex = IoData.m_IoConfig.m_PortIndex;
		ChannelType = -1; // Not relevant
		break;
	case IOCTL_DTU2XX_SET_IOCONFIG:
		PortIndex = IoData.m_IoConfig.m_PortIndex;
		ChannelType = -1; // Not relevant
		break;
	case IOCTL_DTU2XX_REQUEST_EXCL_ACCESS:
		PortIndex = IoData.m_ExclAccess.m_PortIndex;
		ChannelType = IoData.m_ExclAccess.m_ChannelType;
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Receive channel commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	case IOCTL_DTU2XX_RX_LED_CONTROL:
	case IOCTL_DTU2XX_RX_EQUALISE:
	case IOCTL_DTU2XX_RX_GET_FIFO_LOAD:
	case IOCTL_DTU2XX_RX_GET_FLAGS:
	case IOCTL_DTU2XX_RX_CLEAR_FLAGS:
	case IOCTL_DTU2XX_RX_GET_STATUS:
	case IOCTL_DTU2XX_RX_GET_TS_RATE_BPS:
	case IOCTL_DTU2XX_RX_RESET:
	case IOCTL_DTU2XX_RX_SET_RX_POLARITY:
	case IOCTL_DTU2XX_RX_GET_MAX_FIFO_SIZE:
	case IOCTL_DTU2XX_RX_SET_LOOPBACK_MODE:
	case IOCTL_DTU2XX_RX_SET_RX_CONTROL:
	case IOCTL_DTU2XX_RX_GET_RX_CONTROL:
	case IOCTL_DTU2XX_RX_SET_RX_MODE:
	case IOCTL_DTU2XX_RX_GET_RX_MODE:
	case IOCTL_DTU2XX_RX_GET_VIOL_COUNT:
	case IOCTL_DTU2XX_RX_BULK_USERMEM_READ:
#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTU2XX_RX_BULK_USERMEM_READ_32:
#endif	// #if defined(CONFIG_X86_64)

		// All devices use port 1 (i.e. index=0) as input
		PortIndex = 0;
		ChannelType = DTU2XX_TS_RX_CHANNEL;
		break;

	case IOCTL_DTU2XX_TX_CLEAR_FLAGS:
	case IOCTL_DTU2XX_TX_GET_FIFO_LOAD:
	case IOCTL_DTU2XX_TX_GET_FLAGS:
	case IOCTL_DTU2XX_TX_GET_TS_RATE_BPS:
	case IOCTL_DTU2XX_TX_GET_TX_CONTROL:
	case IOCTL_DTU2XX_TX_GET_TX_MODE:
	case IOCTL_DTU2XX_TX_GET_MAX_FIFO_SIZE:
	case IOCTL_DTU2XX_TX_GET_FIFO_SIZE:
	case IOCTL_DTU2XX_TX_RD_LOOPBACK_DATA:
	case IOCTL_DTU2XX_TX_RESET:
	case IOCTL_DTU2XX_TX_SET_TX_POLARITY:
	case IOCTL_DTU2XX_TX_SET_FIFO_SIZE:
	case IOCTL_DTU2XX_TX_SET_LOOPBACK_MODE:
	case IOCTL_DTU2XX_TX_SET_TS_RATE_BPS:
	case IOCTL_DTU2XX_TX_SET_TX_CONTROL:
	case IOCTL_DTU2XX_TX_SET_TX_MODE:
	case IOCTL_DTU2XX_TX_BULK_USERMEM_WRITE:
#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTU2XX_TX_BULK_USERMEM_WRITE_32:
#endif	// #if defined(CONFIG_X86_64)

		// All devices, except DTU-245, use port 1 (i.e. index=0) as output
		if ( pFdo->m_TypeNumber==245 || pFdo->m_TypeNumber==255 )
			PortIndex = 1;
		else
			PortIndex = 0;
		ChannelType = DTU2XX_TS_TX_CHANNEL;
		break;

	default:
		PortIndex = ChannelType = -1;
		break;
	}

	//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ THIRD BIG SWITCH +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

	ReturnStatus = 0; // Assume success

	// Invoke handler IO-control command
	switch (cmd)
	{
	case IOCTL_DTU2XX_RESET_SOFT:
		ReturnStatus = Dtu2xxIoCtlReset(pFdo);
		// No need to set pIrp->IoStatus.Information (returns 0 bytes)
		break;

	case IOCTL_DTU2XX_GET_DRIVER_VERSION:

		IoData.m_DriverVersion.m_VersionMajor = DTU2XX_VERSION_MAJOR;
		IoData.m_DriverVersion.m_VersionMinor = DTU2XX_VERSION_MINOR;
		IoData.m_DriverVersion.m_VersionMicro = DTU2XX_VERSION_MICRO;
		IoData.m_DriverVersion.m_VersionBuild = DTU2XX_VERSION_BUILD;
		break;

	case IOCTL_DTU2XX_GET_LOC_INFO:
		IoData.m_LocInfo.m_VendorId	= pFdo->m_VendorId;
		// This actually is the USB PID
		IoData.m_LocInfo.m_DeviceId	= pFdo->m_DeviceId;

		// Sub_vendorid and sub_systemid have no real meaning for USB devices
		// Encode sub_systemid to match fromat for DTA-1xx cards
		IoData.m_LocInfo.m_SubSystemId	= 0xD100 + pFdo->m_TypeNumber - 100;
		IoData.m_LocInfo.m_SubVendorId	= 0;
		IoData.m_LocInfo.m_UsbAddress	= pFdo->m_UsbAddress;
		break;

	case IOCTL_DTU2XX_LED_CONTROL:
		ReturnStatus = Dtu2xxIoCtlDeviceLedControl(pFdo, IoData.m_Value);
        break;

	case IOCTL_DTU2XX_READ_REGISTER:
		ReturnStatus = Dtu2xxIoCtlReadRegister(pFdo, IoData.m_RegInfo.m_Address,
											   (UInt32*)(&IoData.m_RegInfo.m_Value));
		break;

	case IOCTL_DTU2XX_WRITE_REGISTER:
		ReturnStatus = Dtu2xxIoCtlWriteRegister(pFdo, IoData.m_RegInfo.m_Address,
											   (UInt32)IoData.m_RegInfo.m_Value);
		break;

	case IOCTL_DTU2XX_GET_DEVICE_INFO:
		ReturnStatus = Dtu2xxIoCtlGetDeviceInfo(pFdo, &IoData.m_DevInfo.m_FwVersion,
											    &IoData.m_DevInfo.m_TypeNumber);
		break;

	case IOCTL_DTU2XX_REQUEST_EXCL_ACCESS:

		ReturnStatus = Dtu2xxRequestExclusiveAccess(pFdo, filp, PortIndex,
                                                            IoData.m_ExclAccess.m_Request,
                                                            &IoData.m_ExclAccess.m_Granted);
		break;

	case IOCTL_DTU2XX_READ_VPD:
		ReturnStatus = -EFAULT;	// NOT SUPPORTED
		break;

	case IOCTL_DTU2XX_WRITE_VPD:
		ReturnStatus = -EFAULT;	// NOT SUPPORTED
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_VPD_DATA -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTU2XX_VPD_DATA:
		// Acquire mutex for VPD access
		if ( 0==down_interruptible(&pFdo->m_VpdMutex) )
		{
			ReturnStatus = Dtu2xxVpdIoctl(pFdo, InBufSize, OutBufSize,
										  &IoData.m_VpdData);
			up(&pFdo->m_VpdMutex);
		}
		else
			ReturnStatus = -EFAULT;
		break;

	case IOCTL_DTU2XX_I2C_WRITE:
		ReturnStatus = Dtu2xxIoCtlI2CWrite(pFdo, IoData.m_I2CData.m_DataBuf,
										   IoData.m_I2CData.m_DeviceAddr,
										   IoData.m_I2CData.m_Length);
		break;

	case IOCTL_DTU2XX_I2C_READ:

		ReturnStatus = Dtu2xxIoCtlI2CRead(pFdo, IoData.m_I2CData.m_DataBuf,
										  IoData.m_I2CData.m_DeviceAddr,
										  IoData.m_I2CData.m_Length);
		break;

	case IOCTL_DTU2XX_GET_USB_SPEED:
		ReturnStatus = Dtu2xxIoCtlGetUsbSpeed(pFdo, &IoData.m_Value);
		break;

	case IOCTL_DTU2XX_EEPROM_CONTROL:
		ReturnStatus = Dtu2xxIoCtlEepromControl(pFdo, IoData.m_EepromControl.m_Code);
		break;

	case IOCTL_DTU2XX_UPLOAD_FIRMWARE:
		ReturnStatus = Dtu2xxIoCtlUploadFirmware(pFdo, IoData.m_Value);
		break;

	case IOCTL_DTU2XX_GET_IOCONFIG:
		ReturnStatus = Dtu2xxGetIoConfig(pFdo, &IoData.m_IoConfig);
		break;

	case IOCTL_DTU2XX_SET_IOCONFIG:
		ReturnStatus = Dtu2xxSetIoConfig(pFdo, &IoData.m_IoConfig, FALSE);
		break;

	case IOCTL_DTU2XX_RX_LED_CONTROL:
		ReturnStatus = Dtu2xxRxIoCtlLedControl(pFdo, PortIndex, IoData.m_Value);
		break;

	case IOCTL_DTU2XX_RX_EQUALISE:
		ReturnStatus = Dtu2xxRxIoCtlEqualise(pFdo, PortIndex, IoData.m_Value);
		break;

	case IOCTL_DTU2XX_RX_GET_FIFO_LOAD:
		ReturnStatus = Dtu2xxRxIoCtlGetFifoLoad(pFdo, PortIndex, &IoData.m_Value);
		break;

	case IOCTL_DTU2XX_RX_GET_FLAGS:
		ReturnStatus = Dtu2xxRxIoCtlGetFlags(pFdo, PortIndex,
											  &(IoData.m_Flags.m_Status),
											  &(IoData.m_Flags.m_Latched));
		break;

	case IOCTL_DTU2XX_RX_CLEAR_FLAGS:
		ReturnStatus = Dtu2xxRxIoCtlClearFlags(pFdo, PortIndex, IoData.m_Flags.m_Latched);
		break;

	case IOCTL_DTU2XX_RX_GET_STATUS:
		ReturnStatus = Dtu2xxRxIoCtlGetStatus(pFdo, PortIndex,
										&(IoData.m_RxStatusInfo.m_PacketSize),
										&(IoData.m_RxStatusInfo.m_ClkDet),
										&(IoData.m_RxStatusInfo.m_AsiLock),
										&(IoData.m_RxStatusInfo.m_RateOk),
										&(IoData.m_RxStatusInfo.m_AsiInv));
		break;

	case IOCTL_DTU2XX_RX_GET_TS_RATE_BPS:
		ReturnStatus = Dtu2xxRxIoCtlGetTsRateBps(pFdo, PortIndex, &IoData.m_Value);
		break;

	case IOCTL_DTU2XX_RX_RESET:
		ReturnStatus = Dtu2xxRxIoCtlReset(pFdo, PortIndex, IoData.m_RstInfo.m_RstMode);
		break;

	case IOCTL_DTU2XX_RX_SET_RX_POLARITY:
		ReturnStatus = Dtu2xxRxIoCtlRxPolarity(pFdo, PortIndex, IoData.m_Value);
		break;

	case IOCTL_DTU2XX_RX_SET_LOOPBACK_MODE:
		ReturnStatus = Dtu2xxRxIoCtlSetLoopBackMode(pFdo, PortIndex, IoData.m_Value);
		break;

	case IOCTL_DTU2XX_RX_SET_RX_CONTROL:
		ReturnStatus = Dtu2xxRxIoCtlSetRxControl(pFdo, PortIndex, IoData.m_Value);
		break;

	case IOCTL_DTU2XX_RX_GET_RX_CONTROL:
		ReturnStatus = Dtu2xxRxIoCtlGetRxControl(pFdo, PortIndex, &IoData.m_Value);
		break;

	case IOCTL_DTU2XX_RX_SET_RX_MODE:
		ReturnStatus = Dtu2xxRxIoCtlSetRxMode(pFdo, PortIndex, IoData.m_Value);
		break;

	case IOCTL_DTU2XX_RX_GET_MAX_FIFO_SIZE:
		ReturnStatus = Dtu2xxRxIoCtlGetMaxFifoSize(pFdo, PortIndex, &IoData.m_Value);
		break;

	case IOCTL_DTU2XX_RX_GET_RX_MODE:
		ReturnStatus = Dtu2xxRxIoCtlGetRxMode(pFdo, PortIndex, &IoData.m_Value);
		break;

	case IOCTL_DTU2XX_RX_GET_VIOL_COUNT:
		ReturnStatus = Dtu2xxRxIoCtlGetViolationCount(pFdo, PortIndex, &IoData.m_Value);
		break;

	case IOCTL_DTU2XX_RX_BULK_USERMEM_READ:
		ReturnStatus = Dtu2xxRxIoCtlBulkRead(pFdo, PortIndex,
                                             IoData.m_RwInfo.m_NumBytes2Transfer,
                                             &Dummy, IoData.m_RwInfo.m_pBuffer);
		break;

#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTU2XX_RX_BULK_USERMEM_READ_32:
		ReturnStatus = Dtu2xxRxIoCtlBulkRead(
										 pFdo, PortIndex,
                                         IoData.m_RwInfo_32.m_NumBytes2Transfer, &Dummy,
                                         (UInt8*)((UInt64)IoData.m_RwInfo_32.m_pBuffer));
		break;
#endif	// #if defined(CONFIG_X86_64)

	case IOCTL_DTU2XX_TX_CLEAR_FLAGS:
		ReturnStatus = Dtu2xxTxIoCtlClearFlags(pFdo, PortIndex, IoData.m_Flags.m_Latched);
		break;

	case IOCTL_DTU2XX_TX_GET_FIFO_LOAD:
		ReturnStatus = Dtu2xxTxIoCtlGetFifoLoad(pFdo, PortIndex, &IoData.m_Value);
		break;

	case IOCTL_DTU2XX_TX_GET_FLAGS:
		ReturnStatus = Dtu2xxTxIoCtlGetFlags(
								pFdo, PortIndex,
								&IoData.m_Flags.m_Status,
								&IoData.m_Flags.m_Latched);
		break;

	case IOCTL_DTU2XX_TX_GET_TS_RATE_BPS:
		ReturnStatus = Dtu2xxTxIoCtlGetTsRateBps(pFdo, PortIndex, &IoData.m_Value);
		break;

	case IOCTL_DTU2XX_TX_GET_TX_CONTROL:
		ReturnStatus = Dtu2xxTxIoCtlGetTxControl(pFdo, PortIndex, &IoData.m_Value);
		break;

	case IOCTL_DTU2XX_TX_GET_TX_MODE:
		ReturnStatus = Dtu2xxTxIoCtlGetTxMode(
							pFdo, PortIndex,
							&IoData.m_TxMode.m_TxPacketMode,
							&IoData.m_TxMode.m_TxStuffMode);
		break;

	case IOCTL_DTU2XX_TX_RD_LOOPBACK_DATA:
//		ReturnStatus = Dtu2xxTxIoCtlReadLoopBackData(pFdo, PortIndex, IoData.m_Value);
		break;

	case IOCTL_DTU2XX_TX_RESET:
		ReturnStatus = Dtu2xxTxIoCtlReset(pFdo, PortIndex, IoData.m_RstInfo.m_RstMode);
		break;

	case IOCTL_DTU2XX_TX_SET_TX_POLARITY:
		ReturnStatus = Dtu2xxTxIoCtlTxPolarity(pFdo, PortIndex, IoData.m_Value);
		break;

	case IOCTL_DTU2XX_TX_SET_FIFO_SIZE:
		ReturnStatus = Dtu2xxTxIoCtlSetFifoSize(pFdo, PortIndex, IoData.m_Value);
		break;

	case IOCTL_DTU2XX_TX_GET_MAX_FIFO_SIZE:
		ReturnStatus = Dtu2xxTxIoCtlGetMaxFifoSize(pFdo, PortIndex, &IoData.m_Value);
		break;

	case IOCTL_DTU2XX_TX_GET_FIFO_SIZE:
		ReturnStatus = Dtu2xxTxIoCtlGetFifoSize(pFdo, PortIndex, &IoData.m_Value);
		break;

	case IOCTL_DTU2XX_TX_SET_LOOPBACK_MODE:
		ReturnStatus = Dtu2xxTxIoCtlSetLoopBackMode(pFdo, PortIndex, IoData.m_Value);
		break;

	case IOCTL_DTU2XX_TX_SET_TS_RATE_BPS:
		ReturnStatus = Dtu2xxTxIoCtlSetTsRateBps(pFdo, PortIndex, IoData.m_Value);
		break;

	case IOCTL_DTU2XX_TX_SET_TX_CONTROL:
		ReturnStatus = Dtu2xxTxIoCtlSetTxControl(pFdo, PortIndex, IoData.m_Value);
		break;

	case IOCTL_DTU2XX_TX_SET_TX_MODE:
		ReturnStatus = Dtu2xxTxIoCtlSetTxMode(
							pFdo, PortIndex,
							IoData.m_TxMode.m_TxPacketMode,
							IoData.m_TxMode.m_TxStuffMode);
		break;

	case IOCTL_DTU2XX_TX_BULK_USERMEM_WRITE:

		ReturnStatus = Dtu2xxTxIoCtlBulkWrite(pFdo, PortIndex,
                                              IoData.m_RwInfo.m_NumBytes2Transfer,
                                              IoData.m_RwInfo.m_pBuffer);
		break;

#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTU2XX_TX_BULK_USERMEM_WRITE_32:
		ReturnStatus = Dtu2xxTxIoCtlBulkWrite(pFdo, PortIndex,
                                              IoData.m_RwInfo_32.m_NumBytes2Transfer,
                                              (UInt8*)((UInt64)IoData.m_RwInfo_32.m_pBuffer));
		break;
#endif	// #if defined(CONFIG_X86_64)

	default:
		return -ENOTTY;
		break;
	}

	//-.-.-.-.-.-.-.-.-.-.-.-.- Finally copy result back to user -.-.-.-.-.-.-.-.-.-.-.-.-

	if ( ReturnStatus==0 && ((_IOC_DIR(cmd)&_IOC_READ)!= 0) )
	{
		if ( copy_to_user((DTU2XX_IOCTL_DATA*)arg, &IoData, IoReqSize) )
		{
#if LOG_LEVEL > 0
			DTU2XX_LOG( KERN_INFO, "Dtu2xxDeviceControl: could not copy result to user" );
#endif
			ReturnStatus = -EFAULT;
		}
	}
	return ReturnStatus;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxIoCtlReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for reset device command
//
Int  Dtu2xxIoCtlReset(
	IN PDTU2XX_FDO pFdo) // Our device object
{
	Int Status = 0;
	Int  i=0;
	UInt  BytesTrans;
	Channel*  pCh=NULL;

	// Reset bulk buffers for all channels
	for ( i=0; i<pFdo->m_NumChannels; i++ )
	{
		pCh = &(pFdo->m_Channel[i]);

		// Clear internal buffer
		pCh->m_BulkTransf.m_Buffer.m_pRdPtr
				= pCh->m_BulkTransf.m_Buffer.m_pStart;
		pCh->m_BulkTransf.m_Buffer.m_pWrPtr
				= pCh->m_BulkTransf.m_Buffer.m_pStart;
		pCh->m_BulkTransf.m_Buffer.m_Load = 0;
	}

	// Simply send the vendor command for resetting the device
	Status = Dtu2xxVendorRequest(pFdo, DTU2XX_USB_RESET_DEVICE, 0, 0,
		                         DTU2XX_DIRECTION_WRITE, 0, NULL, &BytesTrans);
	if ( Status != 0 )
		return Status;

	// Update cache after a reset
	return Dtu2xxCacheOpRegs(pFdo);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxIoCtlDeviceLedControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for controlling the device LED
//
Int  Dtu2xxIoCtlDeviceLedControl(
	IN PDTU2XX_FDO pFdo, // Our device object
	IN Int LedControl)			// LED control value
{
	// Get register cache
	Dtu2xxGen* pGenRegs = &(pFdo->m_GenRegs);

	// Check for valid value
	if (   LedControl!=DTU2XX_LED_HARDWARE && LedControl!=DTU2XX_LED_OFF
		&& LedControl!=DTU2XX_LED_GREEN && LedControl!=DTU2XX_LED_RED
		&& LedControl!=DTU2XX_LED_YELLOW )
	{
		return -EINVAL;
	}

	// Modify LED bits
	switch ( LedControl )
	{
	case DTU2XX_LED_HARDWARE:	pGenRegs->m_GenControl.Fields.m_LedControl	= 0;
								break;

	case DTU2XX_LED_OFF:		pGenRegs->m_GenControl.Fields.m_LedControl	= 1;
								pGenRegs->m_GenControl.Fields.m_LedGreen	= 0;
								pGenRegs->m_GenControl.Fields.m_LedRed		= 0;
                                break;

	case DTU2XX_LED_GREEN:		pGenRegs->m_GenControl.Fields.m_LedControl	= 1;
								pGenRegs->m_GenControl.Fields.m_LedGreen	= 1;
								pGenRegs->m_GenControl.Fields.m_LedRed		= 0;
                                break;

	case DTU2XX_LED_RED:		pGenRegs->m_GenControl.Fields.m_LedControl	= 1;
								pGenRegs->m_GenControl.Fields.m_LedGreen	= 0;
								pGenRegs->m_GenControl.Fields.m_LedRed		= 1;
                                break;

	case DTU2XX_LED_YELLOW:		pGenRegs->m_GenControl.Fields.m_LedControl	= 1;
								pGenRegs->m_GenControl.Fields.m_LedGreen	= 1;
								pGenRegs->m_GenControl.Fields.m_LedRed		= 1;
                                break;
	}

	// Write the new value
	return Dtu2xxIoCtlWriteRegister( pFdo, DTU2XX_GEN_BASE_ADDR+DTU2XX_GEN_REG_GENCTRL,
									(Int)(pGenRegs->m_GenControl.All) );
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxIoCtlGetDeviceInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for getting the device information
//
Int  Dtu2xxIoCtlGetDeviceInfo(
	IN PDTU2XX_FDO pFdo, // Our device object
	OUT Int* pFwVersion,		// Firmware version
	OUT Int* pTypeNumber)		// Type number
{
	Int Status;
	UInt BytesTransf=0;
	UInt8 TempBuf[3];

	// Send vendor request
	Status = Dtu2xxVendorRequest(
					pFdo,
					DTU2XX_USB_GET_DEVICE_INF0,
					0, 0, DTU2XX_DIRECTION_READ, 3, TempBuf, &BytesTransf);

	if ( BytesTransf >= 2 )
	{
		*pFwVersion  = TempBuf[0];
		*pTypeNumber = TempBuf[1];
	}
	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxIoCtlReadRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for reading an ALTERA register
//
Int  Dtu2xxIoCtlReadRegister(
	IN PDTU2XX_FDO pFdo, // Our device object
	IN Int Address,				// Absolute address of the register
	OUT UInt32* pRegValue)			// Register value
{
	Int Status;
	UInt BytesTransf=0;
	UInt8 TempBuf[4];

	// Init to zero
	*pRegValue = 0;

	// Send vendor request
	Status = Dtu2xxVendorRequest(
					pFdo,
					DTU2XX_USB_READ_REG, (UInt16)(Address),
					0, DTU2XX_DIRECTION_READ, 4, TempBuf, &BytesTransf);

	if ( BytesTransf >= 4 )
	{
		*pRegValue = (UInt32)(TempBuf[0]);
		*pRegValue += (UInt32)(TempBuf[1]<<8);
		*pRegValue += (UInt32)(TempBuf[2]<<16);
		*pRegValue += (UInt32)(TempBuf[3]<<24);
	}
	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxIoCtlWriteRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for writing to an ALTERA register
//
Int  Dtu2xxIoCtlWriteRegister(
	IN PDTU2XX_FDO pFdo, // Our device object
	IN Int Address,				// Absolute address of the register
	IN UInt32 RegValue)			// Register value
{
	Int Status;
	UInt BytesTransf=0;
	UInt8 TempBuf[4];

	TempBuf[0] = (UInt8)(RegValue & 0xFF);
	TempBuf[1] = (UInt8)((RegValue>>8) & 0xFF);
	TempBuf[2] = (UInt8)((RegValue>>16) & 0xFF);
	TempBuf[3] = (UInt8)((RegValue>>24) & 0xFF);

	// Send vendor request
	Status = Dtu2xxVendorRequest(
					pFdo,
					DTU2XX_USB_WRITE_REG, (UInt16)(Address),
					0, DTU2XX_DIRECTION_WRITE, 4, TempBuf, &BytesTransf);

	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxIoCtlGetUsbSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for getting the USB bus speed
//
Int  Dtu2xxIoCtlGetUsbSpeed(
	IN PDTU2XX_FDO pFdo, // Our device object
	OUT UInt* pUsbSpeed)		// The USB bus speed
{
	Int Status;
	UInt BytesTransf=0;
	UInt8 TempBuf[2];

	// Send vendor request
	Status = Dtu2xxVendorRequest(
					pFdo,
					DTU2XX_USB_GET_USB_SPEED, 0,
					0, DTU2XX_DIRECTION_READ, 2, TempBuf, &BytesTransf);

	if ( BytesTransf >= 2 ) {
		*pUsbSpeed = TempBuf[0];
		*pUsbSpeed |= (TempBuf[1]<<8);
	}
	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxIoCtlEepromControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for special EEPROM control command
//
Int  Dtu2xxIoCtlEepromControl(
	IN PDTU2XX_FDO pFdo, // Our device object
	IN UInt Code)				// Control code
{
	Int Status;
	UInt BytesTransf=0;

	// Send to device
	Status = Dtu2xxVendorRequest(
						pFdo, DTU2XX_USB_EEPROM_CONTROL,
						(UInt16)(Code&0xFFFF),		// The code
						0,
						DTU2XX_DIRECTION_WRITE,
						0, NULL, &BytesTransf);
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxIoCtlUploadFirmware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for upload firmware control command
//
Int  Dtu2xxIoCtlUploadFirmware(
	IN PDTU2XX_FDO pFdo, // Our device object
	IN UInt FirmwareId)			// Firmware ID
{
	//Int Status = 0;

	// Let the upload firmware function do te real work
	return Dtu2xxUploadFirmware(pFdo, FirmwareId);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxIoCtlI2CRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for reading from on-board I2C bus
//
Int  Dtu2xxIoCtlI2CRead(
	IN PDTU2XX_FDO pFdo, // Our device object
	OUT UInt8* pBuf,			// Buffer for data read from I2C bus
	IN UInt DvcAddr,			// I2C Device address
	IN UInt  Length)			// Number of BYTES to read
{
    Int Status = 0;
	UInt TransferSize, ActualTransferred, BufIndex;
	UInt BytesLeft = Length;	// Bytes left to transfer

	// Nothing to transfer
	if ( BytesLeft == 0 )
		return 0;

	if ( Length > DTU2XX_I2C_BUF_SIZE )
		return -EFAULT;

	// Divide transfer is chucks suitable for EP0
	BufIndex = 0;
    while ( BytesLeft > 0 )
	{
		// Do not transfer more than maximum allowed
		TransferSize = ( BytesLeft > EZUSB_EP0_MAX_TR_SIZE ) ?
									EZUSB_EP0_MAX_TR_SIZE : BytesLeft;

		Status = Dtu2xxVendorRequest(
								pFdo, DTU2XX_USB_I2C_READ,
								(UInt16)(DvcAddr),				// Set the device address
								0, DTU2XX_DIRECTION_READ,
								TransferSize, &(pBuf[BufIndex]),
								&ActualTransferred);

		// Transfer must be succesful
		if ( Status!=0 ) {
			break;
		}

		// Must have transferred all
		if ( ActualTransferred != TransferSize ) {
			Status = -EFAULT;
			break;
		}
		// update bytes left and index with buffer
		BytesLeft -= ActualTransferred;
		BufIndex += ActualTransferred;
	}
	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxIoCtlI2CWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for writing to on-board I2C bus
//
Int  Dtu2xxIoCtlI2CWrite(
	IN PDTU2XX_FDO pFdo, // Our device object
	IN UInt8* pBuf,				// Buffer for data read from I2C bus
	IN UInt DvcAddr,			// I2C Device address
	IN UInt  Length)			// Number of BYTES to read
{
	Int Status = 0;
	UInt TransferSize, ActualTransferred, BufIndex;
	UInt BytesLeft = Length;	// Bytes left to transfer

	if ( Length > DTU2XX_I2C_BUF_SIZE )
		return -EFAULT;

	// Nothing to transfer
	if ( BytesLeft == 0 )
		return 0;

	// Divide transfer is chucks suitable for EP0
	BufIndex = 0;
    while ( BytesLeft > 0 )
	{
		// Do not transfer more than maximum allowed
		TransferSize = ( BytesLeft > EZUSB_EP0_MAX_TR_SIZE ) ?
									EZUSB_EP0_MAX_TR_SIZE : BytesLeft;

		Status = Dtu2xxVendorRequest(
								pFdo, DTU2XX_USB_I2C_WRITE,
								(UInt16)(DvcAddr),				// Set the device address
								0, DTU2XX_DIRECTION_WRITE,
								TransferSize, &(pBuf[BufIndex]),
								&ActualTransferred);

		// Transfer must be succesful
		if ( Status!=0 ) {
			break;
		}

		// Must have transferred all
		if ( ActualTransferred != TransferSize ) {
			Status = -EFAULT;
			break;
		}
		// update bytes left and index with buffer
		BytesLeft -= ActualTransferred;
		BufIndex += ActualTransferred;
	}
	return Status;
}
