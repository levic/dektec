//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* IoCtl.c *#*#*#*#*#*#*#*#*#* (C) 2000-2007 DekTec

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
#include <asm/uaccess.h>

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#ifdef CONFIG_COMPAT
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCompatIoCtl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Compatibility for 32-bit applications calling IOCTL in a 64-bit driver
//
long Dta1xxCompatIoCtl(struct file *file, unsigned int cmd, unsigned long arg)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,20)
	struct inode *inode = file->f_dentry->d_inode;
#else
	struct inode *inode = file->f_path.dentry->d_inode;
#endif

	// Just call the regular IOCTL handler; it should be 32-/64-bit clean
	return Dta1xxIoCtl(inode, file, cmd, (unsigned long)compat_ptr(arg));
}
#endif	// #ifdef CONFIG_COMPAT

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxIoCtl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// IO control handler function. Dispatches an IO control command to the appropriate
// handler
//
int Dta1xxIoCtl(
	struct inode *inode,
	struct file *filp,		// Represents file object making the function call
    unsigned int cmd,		// Command
	unsigned long arg)		// Pointer to the IoCtl parameter
{
	char*  IoctlStr;				// Mnemonic string for IOCTL code
	Int  ChannelType, PortIndex;
	Int  IoReqSize;					// Required length of IO buffer
	Channel*  pCh;					// Channel pointer
	Int ReturnStatus = 0;			// Assume success
	DTA1XX_IOCTL_DATA  IoData;		// IO-data
	DTA1XX_FDO* pFdo = (DTA1XX_FDO*)filp->private_data;	// Our functional-device-object
	UInt  InBufSize = 0;
	UInt  OutBufSize = 0;
#if defined(CONFIG_X86_64)
	// 64-bit pointer for use in 32-bit IOCTLs in a 64-bit Linux environment
	void* pTmpPointer64 = NULL;
	DTA1XX_SHAREDBUF_CTRL ShBufCtrl_64;
#endif

	// Check command that this is one of our commands
	if ( DTA1XX_IOCTL_MAGIC!=_IOC_TYPE(cmd) )
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

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Copy IoCtl data from user -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	if ( copy_from_user(&IoData, (DTA1XX_IOCTL_DATA*)arg, _IOC_SIZE(cmd)) )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxIoCtl: could not copy data from user" );
#endif
		return -EFAULT;
	}

	//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ FIRST BIG SWITCH +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	//
	// For debug output: set IoctlStr to mnemonic representation of IOCTL

	IoReqSize = 0;
	switch (cmd)
    {

	case IOCTL_DTA1XX_CDMA_CTRL:
		IoctlStr = "IOCTL_DTA1XX_CDMA_CTRL";
		IoReqSize = sizeof(DTA1XX_CDMA_CONTROL);
		break;

	case IOCTL_DTA1XX_CLEAR_FLAGS:
		IoctlStr = "IOCTL_DTA1XX_CLEAR_FLAGS";
		IoReqSize = sizeof(DTA1XX_FLAGS);
		break;

//	case IOCTL_DTA1XX_CLR_OUTPUT_FIFO:
//		IoctlStr = "IOCTL_DTA1XX_CLR_OUTPUT_FIFO";
//		break;

	case IOCTL_DTA1XX_DMA_USERMEM_READ:
		IoctlStr = "IOCTL_DTA1XX_DMA_USERMEM_READ";
		IoReqSize = sizeof(DTA1XX_RW_INFO);
		break;

#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTA1XX_DMA_USERMEM_READ_32:
		IoctlStr = "IOCTL_DTA1XX_DMA_USERMEM_READ_32";
		IoReqSize = sizeof(DTA1XX_RW_INFO_32);
		break;
#endif	/* CONFIG_X86_64 */

	case IOCTL_DTA1XX_DMA_USERMEM_WRITE:
		IoctlStr = "IOCTL_DTA1XX_DMA_USERMEM_WRITE";
		IoReqSize = sizeof(DTA1XX_RW_INFO);
		break;

#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTA1XX_DMA_USERMEM_WRITE_32:
		IoctlStr = "IOCTL_DTA1XX_DMA_USERMEM_WRITE_32";
		IoReqSize = sizeof(DTA1XX_RW_INFO_32);
		break;
#endif	/* CONFIG_X86_64 */

	case IOCTL_DTA1XX_EEPROM_CONTROL:
		IoctlStr = "IOCTL_DTA1XX_EEPROM_CONTROL";
		IoReqSize = sizeof(DTA1XX_EEPROM_CONTROL);
		break;

	case IOCTL_DTA1XX_FIRMWARE_REBOOT:
		IoctlStr = "IOCTL_DTA1XX_FIRMWARE_REBOOT";
		IoReqSize = sizeof(DTA1XX_FIRMWARE_REBOOT_CTRL);
		break;

	case IOCTL_DTA1XX_GET_ADDR_OPER_REGS:
		IoctlStr = "IOCTL_DTA1XX_GET_ADDR_OPER_REGS";
		IoReqSize = sizeof(DTA1XX_ADDR_INFO);
		break;

#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTA1XX_GET_ADDR_OPER_REGS_32:
		IoctlStr = "IOCTL_DTA1XX_GET_ADDR_OPER_REGS_32";
		IoReqSize = sizeof(DTA1XX_ADDR_INFO_32);
		break;
#endif	/* CONFIG_X86_64 */

	case IOCTL_DTA1XX_GET_CLOCK_ERR:
		IoctlStr = "IOCTL_DTA1XX_GET_CLOCK_ERR";
		IoReqSize = sizeof(DTA1XX_CLOCK_ERR);
		break;

	case IOCTL_DTA1XX_GET_DRIVER_VERSION:
		IoctlStr = "IOCTL_DTA1XX_GET_DRIVER_VERSION";
		IoReqSize = sizeof(DTA1XX_DRIVER_VERSION);
		break;

	case IOCTL_DTA1XX_GET_EXTCLK_FREQ:
		IoctlStr = "IOCTL_DTA1XX_GET_EXTCLK_FREQ";
		IoReqSize = sizeof(DTA1XX_RATE_INFO);
		break;

	case IOCTL_DTA1XX_GET_FAILSAFE:
		IoctlStr = "IOCTL_DTA1XX_GET_FAILSAFE";
		IoReqSize = sizeof(DTA1XX_FAILSAFE);
		break;

	case IOCTL_DTA1XX_GET_FLAGS:
		IoctlStr = "IOCTL_DTA1XX_GET_FLAGS";
		IoReqSize = sizeof(DTA1XX_FLAGS);
		break;

	case IOCTL_DTA1XX_GET_GENLOCK:
		IoctlStr = "IOCTL_DTA1XX_GET_GENLOCK";
		IoReqSize = sizeof(DTA1XX_GENLOCK);
		break;

	case IOCTL_DTA1XX_GET_IOCONFIG:
		IoctlStr = "IOCTL_DTA1XX_GET_IOCONFIG";
		IoReqSize = sizeof(DTA1XX_IOCONFIG);
		break;

	case IOCTL_DTA1XX_GET_IOCONFIG2:
		IoctlStr = "IOCTL_DTA1XX_GET_IOCONFIG2";
		IoReqSize = sizeof(DTA1XX_IOCONFIG2);
		break;

	case IOCTL_DTA1XX_GET_LOC_INFO:
		IoctlStr = "IOCTL_DTA1XX_GET_LOC_INFO";
		IoReqSize = sizeof(DTA1XX_LOC_INFO);
		break;

	case IOCTL_DTA1XX_GET_MACADDRESS:
		IoctlStr = "IOCTL_DTA1XX_GET_MACADDRESS";
		IoReqSize = sizeof(DTA1XX_MACADDRESS);
		break;

	case IOCTL_DTA1XX_GET_MOD_CONTROL:
		IoctlStr = "IOCTL_DTA1XX_GET_MOD_CONTROL";
		IoReqSize = sizeof(DTA1XX_MOD_CONTROL);
		break;

	case IOCTL_DTA1XX_GET_NW_VERSION:
		IoctlStr = "IOCTL_DTA1XX_GET_NW_VERSION";
		IoReqSize = sizeof(DTA1XX_DRIVER_VERSION);
		break;

	case IOCTL_DTA1XX_GET_NWSPEED:
		IoctlStr = "IOCTL_DTA1XX_GET_NWSPEED";
		IoReqSize = sizeof(DTA1XX_NWSPEED);
		break;

	case IOCTL_DTA1XX_GET_NWSTATCNT:
		IoctlStr = "IOCTL_DTA1XX_GET_NWSTATCNT";
		IoReqSize = sizeof(DTA1XX_NWSTATCNT);
		break;

	case IOCTL_DTA1XX_GET_OUTPUT_LEVEL:
		IoctlStr = "IOCTL_DTA1XX_GET_OUTPUT_LEVEL";
		IoReqSize = sizeof(DTA1XX_OUTPUT_LEVEL);
		break;

	case IOCTL_DTA1XX_GET_RATE2:		// Successor of IOCTL_DTA1XX_GET_TS_RATE
		IoctlStr = "IOCTL_DTA1XX_GET_RATE2";
		IoReqSize = sizeof(DTA1XX_RATE_INFO2);
		break;
	
	case IOCTL_DTA1XX_GET_RF_CONTROL:
		IoctlStr = "IOCTL_DTA1XX_GET_RF_CONTROL";
		IoReqSize = sizeof(DTA1XX_RF_CONTROL);
		break;

	case IOCTL_DTA1XX_GET_RX_INFO:
		IoctlStr = "IOCTL_DTA1XX_GET_RX_INFO";
		IoReqSize = sizeof(DTA1XX_RX_INFO);
		break;

	case IOCTL_DTA1XX_GET_RXCLKFREQ:
		IoctlStr = "IOCTL_DTA1XX_GET_RXCLKFREQ";
		IoReqSize = sizeof(DTA1XX_PORT_INTPAR);
		break;

	case IOCTL_DTA1XX_GET_TARGET_ID:
		IoctlStr = "IOCTL_DTA1XX_GET_TARGET_ID";
		IoReqSize = sizeof(DTA1XX_TARGET_INFO);
		break;

	case IOCTL_DTA1XX_GET_TS_RATE:		// LEGACY, replaced by IOCTL_DTA1XX_GET_RATE2
		IoctlStr = "IOCTL_DTA1XX_GET_TS_RATE";
		IoReqSize = sizeof(DTA1XX_RATE_INFO);
		break;

	case IOCTL_DTA1XX_GET_TX_CONTROL:
		IoctlStr = "IOCTL_DTA1XX_GET_TX_CONTROL";
		IoReqSize = sizeof(DTA1XX_TX_CONTROL);
		break;

	case IOCTL_DTA1XX_GET_TXPHASEINCR:
		IoctlStr = "IOCTL_DTA1XX_GET_TXPHASEINCR";
		IoReqSize = sizeof(DTA1XX_PORT_2INTPAR);
		break;

	case IOCTL_DTA1XX_GET_TX_MODE:
		IoctlStr = "IOCTL_DTA1XX_GET_TX_MODE";
		IoReqSize = sizeof(DTA1XX_TX_MODE);
		break;

	case IOCTL_DTA1XX_I2C_TRANSFER:
		IoctlStr = "IOCTL_DTA1XX_I2C_TRANSFER";
		// IoReqSize is checked in Dta1xxI2cTransferIoctl
		// but we need it here for copy_to_user function
		IoReqSize = _IOC_SIZE(cmd);
		break;

	case IOCTL_DTA1XX_IPRX_CTRL:
		IoctlStr = "IOCTL_DTA1XX_IPRX_CTRL";
		// IoReqSize is checked in NwIpRx.c
		// but we need it here for copy_to_user function
		IoReqSize = _IOC_SIZE(cmd);
		break;

	case IOCTL_DTA1XX_IPTX_CTRL:
		IoctlStr = "IOCTL_DTA1XX_IPTX_CTRL";
		// IoReqSize is checked in NwIpTx.c
		// but we need it here for copy_to_user function
		IoReqSize = _IOC_SIZE(cmd);
		break;

	case IOCTL_DTA1XX_READ_VPD:
		IoctlStr = "IOCTL_DTA1XX_READ_VPD";
		IoReqSize = sizeof(DTA1XX_VPD_TRANSFER);
		break;

	case IOCTL_DTA1XX_RELEASE_ADDR_OPER_REGS:
		IoctlStr = "IOCTL_DTA1XX_RELEASE_ADDR_OPER_REGS";
		IoReqSize = sizeof(DTA1XX_ADDR_INFO);
		break;

#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTA1XX_RELEASE_ADDR_OPER_REGS_32:
		IoctlStr = "IOCTL_DTA1XX_RELEASE_ADDR_OPER_REGS_32";
		IoReqSize = sizeof(DTA1XX_ADDR_INFO_32);
		break;
#endif	/* CONFIG_X86_64 */

	case IOCTL_DTA1XX_REQUEST_EXCL_ACCESS:
		IoctlStr = "IOCTL_DTA1XX_REQUEST_EXCL_ACCESS";
		IoReqSize = sizeof(DTA1XX_EXCL_ACCESS_INFO);
		break;

	case IOCTL_DTA1XX_RESET_SOFT:
		IoctlStr = "IOCTL_DTA1XX_RESET_SOFT";
		IoReqSize = sizeof(DTA1XX_RST_INFO);
		break;

	case IOCTL_DTA1XX_SET_FAILSAFE:
		IoctlStr = "IOCTL_DTA1XX_SET_FAILSAFE";
		IoReqSize = sizeof(DTA1XX_FAILSAFE);
		break;

	case IOCTL_DTA1XX_SET_IOCONFIG:
		IoctlStr = "IOCTL_DTA1XX_SET_IOCONFIG";
		IoReqSize = sizeof(DTA1XX_IOCONFIG);
		break;

	case IOCTL_DTA1XX_SET_IOCONFIG2:
		IoctlStr = "IOCTL_DTA1XX_SET_IOCONFIG2";
		IoReqSize = sizeof(DTA1XX_IOCONFIG2);
		break;

	case IOCTL_DTA1XX_SET_MOD_CONTROL:
		IoctlStr = "IOCTL_DTA1XX_SET_MOD_CONTROL";
		IoReqSize = sizeof(DTA1XX_MOD_CONTROL);
		break;

	case IOCTL_DTA1XX_SET_MOD_CONTROL2:
		IoctlStr = "IOCTL_DTA1XX_SET_MOD_CONTROL2";
		IoReqSize = sizeof(DTA1XX_MOD_CONTROL);
		break;

	case IOCTL_DTA1XX_SET_NWLOOPBACK:
		IoctlStr = "IOCTL_DTA1XX_SET_NWLOOPBACK";
		IoReqSize = sizeof(DTA1XX_NWLOOPBACK);
		break;

	case IOCTL_DTA1XX_SET_NWSPEED:
		IoctlStr = "IOCTL_DTA1XX_SET_NWSPEED";
		IoReqSize = sizeof(DTA1XX_NWSPEED);
		break;

	case IOCTL_DTA1XX_SET_NWSTATCNT:
		IoctlStr = "IOCTL_DTA1XX_SET_NWSTATCNT";
		IoReqSize = sizeof(DTA1XX_NWSTATCNT);
		break;
	
	case IOCTL_DTA1XX_SET_OUTPUT_LEVEL:
		IoctlStr = "IOCTL_DTA1XX_SET_OUTPUT_LEVEL";
		IoReqSize = sizeof(DTA1XX_OUTPUT_LEVEL);
		break;

	case IOCTL_DTA1XX_SET_RATE2:		// Successor of IOCTL_DTA1XX_SET_TS_RATE
		IoctlStr = "IOCTL_DTA1XX_SET_RATE2";
		IoReqSize = sizeof(DTA1XX_RATE_INFO2);
		break;
	
	case IOCTL_DTA1XX_SET_RF_CONTROL:
		IoctlStr = "IOCTL_DTA1XX_SET_RF_CONTROL";
		IoReqSize = sizeof(DTA1XX_RF_CONTROL);
		break;

	case IOCTL_DTA1XX_SET_RF_MODE:
		IoctlStr = "IOCTL_DTA1XX_SET_RF_MODE";
		IoReqSize = sizeof(DTA1XX_RF_MODE);
		break;

	case IOCTL_DTA1XX_SET_RX_MODE:
		IoctlStr = "IOCTL_DTA1XX_SET_RX_MODE";
		IoReqSize = sizeof(DTA1XX_RX_MODE);
		break;

	case IOCTL_DTA1XX_SET_TS_RATE:		// LEGACY, replaced by IOCTL_DTA1XX_SET_RATE2
		IoctlStr = "IOCTL_DTA1XX_SET_TS_RATE";
		IoReqSize = sizeof(DTA1XX_RATE_INFO);
		break;

	case IOCTL_DTA1XX_SET_TX_CONTROL:
		IoctlStr = "IOCTL_DTA1XX_SET_TX_CONTROL";
		IoReqSize = sizeof(DTA1XX_TX_CONTROL);
		break;

	case IOCTL_DTA1XX_SET_TX_MODE:
		IoctlStr = "IOCTL_DTA1XX_SET_TX_MODE";
		IoReqSize = sizeof(DTA1XX_TX_MODE);
		break;

	case IOCTL_DTA1XX_SET_TXPHASEINCR:
		IoctlStr = "IOCTL_DTA1XX_SET_TXPHASEINCR";
		IoReqSize = sizeof(DTA1XX_PORT_2INTPAR);
		break;

	case IOCTL_DTA1XX_SHAREDBUF_ALLOC:
		IoctlStr = "IOCTL_DTA1XX_SHAREDBUF_ALLOC";
		IoReqSize = sizeof(DTA1XX_SHAREDBUF_CTRL);
		break;

#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTA1XX_SHAREDBUF_ALLOC_32:
		IoctlStr = "IOCTL_DTA1XX_SHAREDBUF_ALLOC_32";
		IoReqSize = sizeof(DTA1XX_SHAREDBUF_CTRL_32);
		break;
#endif	/* CONFIG_X86_64 */

	case IOCTL_DTA1XX_SHAREDBUF_FREE:
		IoctlStr = "IOCTL_DTA1XX_SHAREDBUF_FREE";
		IoReqSize = sizeof(DTA1XX_SHAREDBUF_CTRL);
		break;

#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTA1XX_SHAREDBUF_FREE_32:
		IoctlStr = "IOCTL_DTA1XX_SHAREDBUF_FREE_32";
		IoReqSize = sizeof(DTA1XX_SHAREDBUF_CTRL_32);
		break;
#endif	/* CONFIG_X86_64 */

	case IOCTL_DTA1XX_VPD_DATA:
		IoctlStr = "IOCTL_DTA1XX_VPD_DATA";
		// IoReqSize is checked in Vpd.c
		// but we need it here for copy_to_user function
		IoReqSize = _IOC_SIZE(cmd);
		break;

	case IOCTL_DTA1XX_WRITE_VPD:
		IoctlStr = "IOCTL_DTA1XX_WRITE_VPD";
		IoReqSize = sizeof(DTA1XX_VPD_TRANSFER);
		break;

	default:
		IoctlStr = "??UNKNOWN IOCTL CODE??";
		break;
	}

#if LOG_LEVEL > 2
	DTA1XX_LOG( KERN_INFO, "Dta1xxIoCtl: IoCtl call received: %s (0x%08X)",
				IoctlStr, cmd );
#endif

	// Check buffer size
	if ( _IOC_SIZE(cmd) < IoReqSize )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_CRIT, "Dta1xxIoCtl: BufSize=%d (Rq=%d) IO BUFFER IS TOO "
					"SMALL!!", _IOC_SIZE(cmd), IoReqSize );
#endif
		return -EINVAL;
	}

	//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ SECOND BIG SWITCH +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	//
	// Determine ChannelType and PortIndex

	// Default value is 'device-level' (instead of port-level)
	ChannelType = PortIndex = -1;

	switch (cmd)
    {

	case IOCTL_DTA1XX_CDMA_CTRL:
		ChannelType = IoData.m_CdmaControl.m_ChannelType;
		PortIndex = IoData.m_CdmaControl.m_PortIndex;
		break;

	case IOCTL_DTA1XX_CLEAR_FLAGS:

		if (   pFdo->m_TypeNumber == 140
			&& IoData.m_Flags.m_ChannelType == DTA1XX_TS_TX_CHANNEL
			&& IoData.m_Flags.m_PortIndex == 0)
		{
			ChannelType = DTA1XX_TS_TX_CHANNEL;
			PortIndex   = 1;
		}
		else
		{
			ChannelType = IoData.m_Flags.m_ChannelType;
			PortIndex   = IoData.m_Flags.m_PortIndex;
		}
		break;

//	case IOCTL_DTA1XX_CLR_OUTPUT_FIFO:
//		break;

	case IOCTL_DTA1XX_DMA_USERMEM_READ:
		ChannelType = IoData.m_RwInfo.m_ChannelType;
		PortIndex = IoData.m_RwInfo.m_PortIndex;
		break;

#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTA1XX_DMA_USERMEM_READ_32:
		ChannelType = IoData.m_RwInfo_32.m_ChannelType;
		PortIndex = IoData.m_RwInfo_32.m_PortIndex;
		break;
#endif	/* CONFIG_X86_64 */

	case IOCTL_DTA1XX_DMA_USERMEM_WRITE:
		ChannelType = IoData.m_RwInfo.m_ChannelType;
		PortIndex = IoData.m_RwInfo.m_PortIndex;
		break;

#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTA1XX_DMA_USERMEM_WRITE_32:
		ChannelType = IoData.m_RwInfo_32.m_ChannelType;
		PortIndex = IoData.m_RwInfo_32.m_PortIndex;
		break;
#endif	/* CONFIG_X86_64 */

	case IOCTL_DTA1XX_EEPROM_CONTROL:
		break;

	case IOCTL_DTA1XX_FIRMWARE_REBOOT:
		break;

	case IOCTL_DTA1XX_GET_ADDR_OPER_REGS:

		if (   pFdo->m_TypeNumber == 140
			&& IoData.m_AddrInfo.m_ChannelType == DTA1XX_TS_TX_CHANNEL
			&& IoData.m_AddrInfo.m_PortIndex == 0)
		{
				ChannelType = DTA1XX_TS_TX_CHANNEL;
				PortIndex   = 1;
		}
		else
		{
			ChannelType = IoData.m_AddrInfo.m_ChannelType;
			PortIndex   = IoData.m_AddrInfo.m_PortIndex;
		}
		break;

#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTA1XX_GET_ADDR_OPER_REGS_32:

		if (   pFdo->m_TypeNumber == 140
			&& IoData.m_AddrInfo_32.m_ChannelType == DTA1XX_TS_TX_CHANNEL
			&& IoData.m_AddrInfo_32.m_PortIndex == 0)
		{
				ChannelType = DTA1XX_TS_TX_CHANNEL;
				PortIndex   = 1;
		}
		else
		{
			ChannelType = IoData.m_AddrInfo_32.m_ChannelType;
			PortIndex   = IoData.m_AddrInfo_32.m_PortIndex;
		}
		break;
#endif	/* CONFIG_X86_64 */

	case IOCTL_DTA1XX_GET_CLOCK_ERR:
		break;

	case IOCTL_DTA1XX_GET_DRIVER_VERSION:
		break;

	case IOCTL_DTA1XX_GET_EXTCLK_FREQ:
		ChannelType = DTA1XX_TS_TX_CHANNEL;
		PortIndex = IoData.m_RateInfo.m_PortIndex;
		break;

	case IOCTL_DTA1XX_GET_FAILSAFE:
		break;

	case IOCTL_DTA1XX_GET_FLAGS:

		if (   pFdo->m_TypeNumber == 140
			&& IoData.m_Flags.m_ChannelType == DTA1XX_TS_TX_CHANNEL
			&& IoData.m_Flags.m_PortIndex == 0)
		{
			ChannelType = DTA1XX_TS_TX_CHANNEL;
			PortIndex   = 1;
		}
		else
		{
			ChannelType = IoData.m_Flags.m_ChannelType;
			PortIndex   = IoData.m_Flags.m_PortIndex;
		}
		break;

	case IOCTL_DTA1XX_GET_GENLOCK:
		ChannelType = IoData.m_Genlock.m_ChannelType;
		PortIndex   = IoData.m_Genlock.m_PortIndex;
		break;

	case IOCTL_DTA1XX_GET_IOCONFIG:
		// Channel type is not relavent in this case
		PortIndex = IoData.m_IoConfig.m_PortIndex;
		break;

	case IOCTL_DTA1XX_GET_IOCONFIG2:
		// Channel type is not relavent in this case
		PortIndex = IoData.m_IoConfig2.m_PortIndex;
		break;

	case IOCTL_DTA1XX_GET_LOC_INFO:
		break;

	case IOCTL_DTA1XX_GET_MACADDRESS:
		break;
	
	case IOCTL_DTA1XX_GET_MOD_CONTROL:
		ChannelType = DTA1XX_TS_TX_CHANNEL;
		PortIndex   = IoData.m_ModControl.m_PortIndex;
		break;

	case IOCTL_DTA1XX_GET_NW_VERSION:
		break;

	case IOCTL_DTA1XX_GET_NWSPEED:
		break;

	case IOCTL_DTA1XX_GET_NWSTATCNT:
		break;

	case IOCTL_DTA1XX_GET_RATE2:		// Successor of IOCTL_DTA1XX_GET_TS_RATE
		ChannelType = DTA1XX_TS_TX_CHANNEL;
		if ( pFdo->m_TypeNumber==140 && IoData.m_RateInfo2.m_PortIndex==0 )
			PortIndex = 1;
		else
			PortIndex = IoData.m_RateInfo2.m_PortIndex;
		break;

	case IOCTL_DTA1XX_GET_RX_INFO:
		ChannelType = DTA1XX_TS_RX_CHANNEL;
		PortIndex = IoData.m_RxInfo.m_PortIndex;
		break;

	case IOCTL_DTA1XX_GET_TARGET_ID:
		ChannelType = DTA1XX_TS_TX_CHANNEL;
		PortIndex   = IoData.m_TargetInfo.m_PortIndex;
		break;

	case IOCTL_DTA1XX_GET_TS_RATE:		// LEGACY, replaced by IOCTL_DTA1XX_GET_RATE2
		ChannelType = DTA1XX_TS_TX_CHANNEL;
		if ( pFdo->m_TypeNumber == 140 && IoData.m_RateInfo.m_PortIndex == 0 )
			PortIndex = 1;
		else
			PortIndex = IoData.m_RateInfo.m_PortIndex;
		break;

	case IOCTL_DTA1XX_GET_TX_CONTROL:
		ChannelType = DTA1XX_TS_TX_CHANNEL;
		if ( pFdo->m_TypeNumber == 140 && IoData.m_TxControl.m_PortIndex == 0 )
			PortIndex = 1;
		else
			PortIndex = IoData.m_TxControl.m_PortIndex;
		break;

	case IOCTL_DTA1XX_GET_TX_MODE:
		ChannelType = DTA1XX_TS_TX_CHANNEL;
		if ( pFdo->m_TypeNumber == 140 && IoData.m_TxControl.m_PortIndex == 0 )
			PortIndex = 1;
		else
			PortIndex = IoData.m_TxControl.m_PortIndex;
		break;

	case IOCTL_DTA1XX_I2C_TRANSFER:
		break;

	case IOCTL_DTA1XX_IPRX_CTRL:
		break;

	case IOCTL_DTA1XX_IPTX_CTRL:
		break;

	case IOCTL_DTA1XX_READ_VPD:
		break;

	case IOCTL_DTA1XX_RELEASE_ADDR_OPER_REGS:
		if (   pFdo->m_TypeNumber == 140
			&& IoData.m_AddrInfo.m_ChannelType == DTA1XX_TS_TX_CHANNEL
			&& IoData.m_AddrInfo.m_PortIndex == 0)
		{
				ChannelType = DTA1XX_TS_TX_CHANNEL;
				PortIndex   = 1;
		}
		else
		{
			ChannelType = IoData.m_AddrInfo.m_ChannelType;
			PortIndex   = IoData.m_AddrInfo.m_PortIndex;
		}
		break;

#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTA1XX_RELEASE_ADDR_OPER_REGS_32:
		if (   pFdo->m_TypeNumber == 140
			&& IoData.m_AddrInfo_32.m_ChannelType == DTA1XX_TS_TX_CHANNEL
			&& IoData.m_AddrInfo_32.m_PortIndex == 0)
		{
				ChannelType = DTA1XX_TS_TX_CHANNEL;
				PortIndex   = 1;
		}
		else
		{
			ChannelType = IoData.m_AddrInfo_32.m_ChannelType;
			PortIndex   = IoData.m_AddrInfo_32.m_PortIndex;
		}
		break;
#endif	/* CONFIG_X86_64 */

	case IOCTL_DTA1XX_REQUEST_EXCL_ACCESS:

		if (    pFdo->m_TypeNumber == 140
			&& IoData.m_ExclAccess.m_ChannelType == DTA1XX_TS_TX_CHANNEL
			&& IoData.m_ExclAccess.m_PortIndex == 0)
		{
			ChannelType = DTA1XX_TS_TX_CHANNEL;
			PortIndex   = 1;
		}
		else
		{
			ChannelType = IoData.m_ExclAccess.m_ChannelType;
			PortIndex   = IoData.m_ExclAccess.m_PortIndex;
		}
		break;

	case IOCTL_DTA1XX_RESET_SOFT:

		if (   pFdo->m_TypeNumber == 140
			&& IoData.m_RstInfo.m_ChannelType == DTA1XX_TS_TX_CHANNEL
			&& IoData.m_RstInfo.m_PortIndex == 0)
		{
			ChannelType = DTA1XX_TS_TX_CHANNEL;
			PortIndex   = 1;
		}
		else
		{
			ChannelType = IoData.m_RstInfo.m_ChannelType;
			PortIndex   = IoData.m_RstInfo.m_PortIndex;
		}
		break;

	case IOCTL_DTA1XX_SET_FAILSAFE:
		break;

	case IOCTL_DTA1XX_SET_IOCONFIG:
		// Channel type is not relavent in this case
		PortIndex = IoData.m_IoConfig.m_PortIndex;
		break;

	case IOCTL_DTA1XX_SET_IOCONFIG2:
		PortIndex = IoData.m_IoConfig2.m_PortIndex;
		break;

	case IOCTL_DTA1XX_SET_MOD_CONTROL:
	case IOCTL_DTA1XX_SET_MOD_CONTROL2:
		ChannelType = DTA1XX_TS_TX_CHANNEL;
		PortIndex   = IoData.m_ModControl.m_PortIndex;
		break;

	case IOCTL_DTA1XX_SET_NWLOOPBACK:
		break;

	case IOCTL_DTA1XX_SET_NWSPEED:
		break;

	case IOCTL_DTA1XX_SET_NWSTATCNT:
		break;

	case IOCTL_DTA1XX_SET_RATE2:		// Successor of IOCTL_DTA1XX_SET_TS_RATE
		ChannelType = DTA1XX_TS_TX_CHANNEL;
		if ( pFdo->m_TypeNumber==140 && IoData.m_RateInfo2.m_PortIndex==0 )
			PortIndex = 1;
		else
			PortIndex = IoData.m_RateInfo2.m_PortIndex;
		break;

	case IOCTL_DTA1XX_SET_RX_MODE:
		PortIndex = IoData.m_RxMode.m_PortIndex;
		break;

	case IOCTL_DTA1XX_SET_TS_RATE:		// LEGACY, replaced by IOCTL_DTA1XX_SET_RATE2
		ChannelType = DTA1XX_TS_TX_CHANNEL;
		if ( pFdo->m_TypeNumber==140 && IoData.m_RateInfo.m_PortIndex==0 )
			PortIndex = 1;
		else
			PortIndex = IoData.m_RateInfo.m_PortIndex;
		break;

	case IOCTL_DTA1XX_SET_TX_CONTROL:
		ChannelType = DTA1XX_TS_TX_CHANNEL;
		if ( pFdo->m_TypeNumber==140 && IoData.m_TxControl.m_PortIndex==0 )
			PortIndex = 1;
		else
			PortIndex = IoData.m_TxControl.m_PortIndex;
		break;

	case IOCTL_DTA1XX_SET_TX_MODE:
		ChannelType = DTA1XX_TS_TX_CHANNEL;
		if ( pFdo->m_TypeNumber == 140 && IoData.m_TxMode.m_PortIndex == 0)
			PortIndex = 1;
		else
			PortIndex = IoData.m_TxMode.m_PortIndex;
		break;

	case IOCTL_DTA1XX_SHAREDBUF_ALLOC:
		// Cannot read PortIndex here!!
		break;

#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTA1XX_SHAREDBUF_ALLOC_32:
		// Cannot read PortIndex here!!
		break;
#endif	/* CONFIG_X86_64 */

	case IOCTL_DTA1XX_SHAREDBUF_FREE:
		PortIndex = IoData.m_ShBufCtrl.m_PortIndex;
		break;

#if defined(CONFIG_X86_64)
	// Only for 64-bit Linux, in case the application is 32-bit
	case IOCTL_DTA1XX_SHAREDBUF_FREE_32:
		PortIndex = IoData.m_ShBufCtrl_32.m_PortIndex;
		break;
#endif	/* CONFIG_X86_64 */

	case IOCTL_DTA1XX_VPD_DATA:
		break;

	case IOCTL_DTA1XX_WRITE_VPD:
		break;

		// input buffer DTA1XX_PORT_INTPAR
	case IOCTL_DTA1XX_GET_RXCLKFREQ:
		PortIndex   = IoData.m_PortIntpar.m_PortIndex;
		break;

	// input buffer DTA1XX_PORT_2INTPAR
	case IOCTL_DTA1XX_GET_TXPHASEINCR:
	case IOCTL_DTA1XX_SET_TXPHASEINCR:
		PortIndex   = IoData.m_Port2Intpar.m_PortIndex;
		break;

	// input buffer IOCTL_DTA1XX_GET_OUTPUT_LEVEL
	case IOCTL_DTA1XX_GET_OUTPUT_LEVEL:
	case IOCTL_DTA1XX_SET_OUTPUT_LEVEL:
		PortIndex   = IoData.m_OutputLevel.m_PortIndex;
		break;

	// input buffer DTA1XX_RF_CONTROL
	case IOCTL_DTA1XX_GET_RF_CONTROL:
	case IOCTL_DTA1XX_SET_RF_CONTROL:
		PortIndex   = IoData.m_RfControl.m_PortIndex;
		break;

	// input buffer DTA1XX_RF_MODE
	case IOCTL_DTA1XX_SET_RF_MODE:
		PortIndex   = IoData.m_RfMode.m_PortIndex;
		break;

	default:
		break;
	}


	//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ THIRD BIG SWITCH +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
	//
	// Call appropriate handler for IO-control command

	switch (cmd)
    {

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_CDMA_CTRL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	// Prepare a buffer for Continuous DMA.
	//
	case IOCTL_DTA1XX_CDMA_CTRL:
		ReturnStatus = Dta1xxCdmaCtrl(pFdo, &IoData.m_CdmaControl);
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_CLEAR_FLAGS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_CLEAR_FLAGS:
		ReturnStatus = Dta1xxClearFlags(pFdo, ChannelType, PortIndex,
										IoData.m_Flags.m_Latched);
		break;


	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_CLR_OUTPUT_FIFO -.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// LEGACY: only used by old programs. New DTAPI uses IOCTL_DTA1XX_RESET_SOFT
	// exclusively.
	//
//	case IOCTL_DTA1XX_CLR_OUTPUT_FIFO:
//		ReturnStatus = -EFAULT;	// NOT SUPPORTED
//		break;

	// IOCTL_DTA1XX_DMA_USERMEM_READ
	//
	// Transfer data from card to user buffer using DMA
	//
	case IOCTL_DTA1XX_DMA_USERMEM_READ:
		ReturnStatus = Dta1xxDmaUserMemRead(pFdo, ChannelType, PortIndex,
											IoData.m_RwInfo.m_pBuffer,
											IoData.m_RwInfo.m_NumBytes2Transfer);
		break;

#if defined(CONFIG_X86_64)
	// IOCTL_DTA1XX_DMA_USERMEM_READ_32
	//
	// Transfer data from card to user buffer using DMA
	//
	// This is the 32-bit version of the IOCTL, in case a 32-bit application calls
	//	in a 64-bit Linux environment.
	//
	case IOCTL_DTA1XX_DMA_USERMEM_READ_32:
		ReturnStatus = Dta1xxDmaUserMemRead(pFdo, ChannelType, PortIndex,
											(UInt8*)((UInt64)IoData.m_RwInfo_32.m_pBuffer),
											IoData.m_RwInfo_32.m_NumBytes2Transfer);
		break;
#endif	/* CONFIG_X86_64 */

	// IOCTL_DTA1XX_DMA_USERMEM_WRITE
	//
	// Transfer data from user buffer to card using DMA
	//
	case IOCTL_DTA1XX_DMA_USERMEM_WRITE:
		ReturnStatus = Dta1xxDmaUserMemWrite(pFdo, ChannelType, PortIndex,
											 IoData.m_RwInfo.m_pBuffer,
											 IoData.m_RwInfo.m_NumBytes2Transfer);
		break;

#if defined(CONFIG_X86_64)
	// IOCTL_DTA1XX_DMA_USERMEM_WRITE_32
	//
	// Transfer data from user buffer to card using DMA
	//
	// This is the 32-bit version of the IOCTL, in case a 32-bit application calls
	//	in a 64-bit Linux environment.
	//
	case IOCTL_DTA1XX_DMA_USERMEM_WRITE_32:
		ReturnStatus = Dta1xxDmaUserMemWrite(pFdo, ChannelType, PortIndex,
											 (UInt8*)((UInt64)IoData.m_RwInfo_32.m_pBuffer),
											 IoData.m_RwInfo_32.m_NumBytes2Transfer);
		break;
#endif	/* CONFIG_X86_64 */

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_EEPROM_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	// Special EEPROM operations: set protect register and clear protect-register
	// Code must match exactly, otherwise operation is a NOP.
	//
	case IOCTL_DTA1XX_EEPROM_CONTROL:
		if ( IoData.m_EepromControl.m_Code == DTA1XX_EEPROM_PROTECT )
			Dta1xxSetProtectRegister(pFdo);
		else if ( IoData.m_EepromControl.m_Code == DTA1XX_EEPROM_PROTECT_CLR )
			Dta1xxSetProtectRegisterClear(pFdo);
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_FIRMWARE_REBOOT -.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// Reboot firmware on PCI cards with PLD
	//
	case IOCTL_DTA1XX_FIRMWARE_REBOOT:
		ReturnStatus = Dta1xxFirmwareReboot(pFdo);
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_ADDR_OPER_REGS -.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	// Get virtual address of operational registers. This address can be used in
	// user-mode to directly access the operational registers.
	// EAGAIN is returned if mmap is not yet executed.
	//
	case IOCTL_DTA1XX_GET_ADDR_OPER_REGS:
		ReturnStatus = Dta1xxGetAddrOperRegs(pFdo, filp, ChannelType, PortIndex, 
											&IoData.m_AddrInfo.m_pUserVirtual,
											&IoData.m_AddrInfo.m_Size,
											IoData.m_AddrInfo.m_pPaMmap);
		if (ReturnStatus == -EAGAIN) {
			// Special case
			if ( copy_to_user((DTA1XX_IOCTL_DATA*)arg, &IoData, IoReqSize) )
			{
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "Dta1xxIoCtl: could not copy result to user" );
#endif
				ReturnStatus = -EFAULT;
			}
		}
		break;
		
#if defined(CONFIG_X86_64)
	//-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_ADDR_OPER_REGS_32 .-.-.-.-.-.-.-.-.-.-.-.
	//
	// Get virtual address of operational registers. This address can be used in
	// user-mode to directly access the operational registers.
	// EAGAIN is returned if mmap is not yet executed.
	//
	// This is the 32-bit version of the IOCTL, in case a 32-bit application calls
	//	in a 64-bit Linux environment.
	//
	case IOCTL_DTA1XX_GET_ADDR_OPER_REGS_32:
		ReturnStatus = Dta1xxGetAddrOperRegs(pFdo, filp, ChannelType, PortIndex, 
											&pTmpPointer64,
											&IoData.m_AddrInfo_32.m_Size,
											(void*)((UInt64)IoData.m_AddrInfo_32.m_pPaMmap));

		IoData.m_AddrInfo_32.m_pUserVirtual = (UInt64)pTmpPointer64;

		if (ReturnStatus == -EAGAIN) {
			// Special case
			if ( copy_to_user((DTA1XX_IOCTL_DATA*)arg, &IoData, IoReqSize) )
			{
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "Dta1xxIoCtl: could not copy result to user" );
#endif
				ReturnStatus = -EFAULT;
			}
		}
		break;
#endif	/* CONFIG_X86_64 */

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_CLOCK_ERR -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	case IOCTL_DTA1XX_GET_CLOCK_ERR:
		ReturnStatus = -EFAULT;	// NOT SUPPORTED
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_DRIVER_VERSION -.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	case IOCTL_DTA1XX_GET_DRIVER_VERSION:
		IoData.m_DriverVersion.m_VersionMajor = DTA1XX_VERSION_MAJOR;
		IoData.m_DriverVersion.m_VersionMinor = DTA1XX_VERSION_MINOR;
		IoData.m_DriverVersion.m_VersionMicro = DTA1XX_VERSION_MICRO;
		IoData.m_DriverVersion.m_VersionBuild = DTA1XX_VERSION_BUILD;
  		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_EXTCLK_FREQ -.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// Get external clock frequency.
	// Only used for Transport-Stream transmit channels
	//
	case IOCTL_DTA1XX_GET_EXTCLK_FREQ:
		// NOTE: Dta1xxGetFreqEst checks for validity of PortIndex
		if (pFdo->m_TypeNumber == 2142) {
			UInt ExtClkFreq;
			ExtClkFreq = Dta1xxGenGetExtClkFreqReg((UInt8*)pFdo->m_pGenRegs);
			if (ExtClkFreq) ExtClkFreq+=3;
			IoData.m_RateInfo.m_TsRate = ExtClkFreq;
			ReturnStatus = 0;
		} else {
			ReturnStatus = Dta1xxGetFreqEst(pFdo, ChannelType, PortIndex,
											&IoData.m_RateInfo.m_TsRate);
			IoData.m_RateInfo.m_TsRate /= 8;
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_FAILSAFE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// Returns the current failsafe status
	//
	case IOCTL_DTA1XX_GET_FAILSAFE:
		ReturnStatus = Dta1xxGetFailsafeState(pFdo, &IoData.m_Failsafe);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_FLAGS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	// Read status flags from hardware.
	//
	case IOCTL_DTA1XX_GET_FLAGS:
		ReturnStatus = Dta1xxGetFlags(pFdo, ChannelType, PortIndex,
									  &IoData.m_Flags.m_Status,
									  &IoData.m_Flags.m_Latched);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_GENLOCK -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// Returns the current SDI reference genlock status of the device
	//
	case IOCTL_DTA1XX_GET_GENLOCK:
		ReturnStatus = Dta1xxGetGenRefState(pFdo, &IoData.m_Genlock);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_IOCONFIG -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_GET_IOCONFIG:
		ReturnStatus = Dta1xxGetIoConfig(pFdo, &IoData.m_IoConfig);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_IOCONFIG2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_GET_IOCONFIG2:
		ReturnStatus = Dta1xxGetIoConfig2(pFdo, &IoData.m_IoConfig2);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_LOC_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// Get location info: PCI bus#, Slot#, Device ID, ...
	//
	case IOCTL_DTA1XX_GET_LOC_INFO:
		IoData.m_LocInfo.m_DeviceId		= pFdo->m_DeviceId;
		IoData.m_LocInfo.m_VendorId		= pFdo->m_VendorId;
		IoData.m_LocInfo.m_SubSystemId	= pFdo->m_SubSystemId;
		IoData.m_LocInfo.m_SubVendorId	= pFdo->m_SubVendorId;
		IoData.m_LocInfo.m_BusNumber	= pFdo->m_BusNumber;
		IoData.m_LocInfo.m_SlotNumber	= pFdo->m_SlotNumber;
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_MACADDRESS -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	case IOCTL_DTA1XX_GET_MACADDRESS:
		if (IoData.m_MacAddress.m_IpPortIndex >= pFdo->m_NumIpPorts) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_GET_MACADDRESS: IpPortIndex=%d "
					    "DOES NOT EXIST", IoData.m_MacAddress.m_IpPortIndex );
#endif
			ReturnStatus = -EFAULT;
		} else {
			ReturnStatus = Dta1xxNwMacGetMacAddress(
				&pFdo->m_NwStore[IoData.m_MacAddress.m_IpPortIndex],
				&IoData.m_MacAddress);
		}
		break;
		
	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_MOD_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_GET_MOD_CONTROL:
		ReturnStatus = Dta1xxTxGetModControl(pFdo, PortIndex,
											 &IoData.m_ModControl.m_ModType,
											 &IoData.m_ModControl.m_ParXtra0,
											 &IoData.m_ModControl.m_ParXtra1,
											 &IoData.m_ModControl.m_ParXtra2);
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_NW_VERSION -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	case IOCTL_DTA1XX_GET_NW_VERSION:
		ReturnStatus = -EFAULT;	// NOT IMPLEMENTED
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_NWSPEED -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_GET_NWSPEED:
		if (IoData.m_NwSpeed.m_IpPortIndex >= pFdo->m_NumIpPorts) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_GET_NWSPEED: IpPortIndex=%d "
					    "DOES NOT EXIST", IoData.m_NwSpeed.m_IpPortIndex );
#endif
			ReturnStatus = -EFAULT;
		} else {
			ReturnStatus = Dta1xxNwGetSpeed(
				&pFdo->m_NwStore[IoData.m_NwSpeed.m_IpPortIndex],
				&IoData.m_NwSpeed);
		}
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_NWSTATCNT -.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_GET_NWSTATCNT:
		if (IoData.m_NwStatCnt.m_IpPortIndex >= pFdo->m_NumIpPorts) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_GET_NWSTATCNT: IpPortIndex=%d "
					 "DOES NOT EXIST", IoData.m_NwStatCnt.m_IpPortIndex );
#endif
			ReturnStatus = -EFAULT;
		} else {
			// FIXME: Dummy IRP variable required to comply function definition.
			IRP IrpDummy;

			ReturnStatus = Dta1xxNwGetSetStatCnt(
							&pFdo->m_NwStore[IoData.m_NwStatCnt.m_IpPortIndex],
							&IrpDummy,
							&IoData.m_NwStatCnt,
							0);
			if (ReturnStatus == STATUS_PENDING) {
				return ReturnStatus;
			}
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_OUTPUT_LEVEL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_GET_OUTPUT_LEVEL:
		ReturnStatus = Dta1xxTxGetOutputLevel( pFdo, PortIndex,
				&IoData.m_OutputLevel.m_OutputLevel);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_RATE2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	// Sucessor of IOCTL_DTA1XX_GET_TS_RATE
	//
	case IOCTL_DTA1XX_GET_RATE2:
		ReturnStatus = Dta1xxTxGetRate2(pFdo, PortIndex,
									   &IoData.m_RateInfo2.m_ClockGenMode,
									   &IoData.m_RateInfo2.m_TsSymOrSampRate);
		break;
	
	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_RF_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_GET_RF_CONTROL:
		ReturnStatus = Dta1xxTxGetRfControl( pFdo, PortIndex,
			&IoData.m_RfControl.m_RfRate,
			&IoData.m_RfControl.m_LockStatus );
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_RX_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// Get receive-channel info.
	// Only used for Transport-Stream receive channels
	//
	case IOCTL_DTA1XX_GET_RX_INFO:
		ReturnStatus = Dta1xxGetFreqEst(pFdo, ChannelType, PortIndex,
										&IoData.m_RxInfo.m_TsRate);
		if ( ReturnStatus == 0 )
			IoData.m_RxInfo.m_ViolCount = pFdo->m_Channel[PortIndex].m_ViolCountSamp;
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_RXCLKFREQ -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_GET_RXCLKFREQ:
		ReturnStatus = Dta1xxGetRxClkFreq(pFdo,&IoData.m_PortIntpar);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_TARGET_ID -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	case IOCTL_DTA1XX_GET_TARGET_ID:
		ReturnStatus = Dta1xxGetTargetId(pFdo, ChannelType, PortIndex,
										 &IoData.m_TargetInfo.m_Present,
										 &IoData.m_TargetInfo.m_TargetId);
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_TS_RATE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// LEGACY, replaced by IOCTL_DTA1XX_GET_RATE2
	//
	case IOCTL_DTA1XX_GET_TS_RATE:
		ReturnStatus = -EFAULT;	// NOT SUPPORTED
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_TX_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	case IOCTL_DTA1XX_GET_TX_CONTROL:
		ReturnStatus = Dta1xxTxGetTxControl(pFdo, PortIndex,
											&IoData.m_TxControl.m_TxControl);
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_TX_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_GET_TX_MODE:
		ReturnStatus = Dta1xxTxGetTxMode(pFdo, PortIndex,
										 &IoData.m_TxMode.m_TxPacketMode,
										 &IoData.m_TxMode.m_TxStuffMode);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_TXPHASEINCR -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_GET_TXPHASEINCR:
		ReturnStatus = Dta1xxGetTxPhaseIncr(pFdo, &IoData.m_Port2Intpar);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_I2C_TRANSFER -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_I2C_TRANSFER:
		ReturnStatus = Dta1xxI2cTransferIoctl(pFdo, &IoData.m_I2cTransfer);
		break;


	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_IPRX_CTRL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	case IOCTL_DTA1XX_IPRX_CTRL:
		ReturnStatus = Dta1xxNwIpRxIoctl(pFdo, filp, InBufSize, OutBufSize,
										 &(IoData.m_IpRxTxCtrl),
										 &(IoData.m_IpRxTxRtrn));
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_IPTX_CTRL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	case IOCTL_DTA1XX_IPTX_CTRL:
		ReturnStatus = Dta1xxNwIpTxIoctl(pFdo, filp, InBufSize, OutBufSize,
										 &(IoData.m_IpRxTxCtrl),
										 &(IoData.m_IpRxTxRtrn));
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_READ_VPD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// This IOCTL is obsolete. Use IOCTL_DTA1XX_VPD_DATA instead.
	//
	case IOCTL_DTA1XX_READ_VPD:
		ReturnStatus = -EFAULT;	// NOT SUPPORTED
		break;

	//-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_RELEASE_ADDR_OPER_REGS -.-.-.-.-.-.-.-.-.-.-.-.
	//
	// Decreases reference count of operational registers usage previously
	// getted by IOCTL_DTA1XX_GET_ADDR_OPER_REGS.
	// EBUSY is returned if reference count != 0
	//
	case IOCTL_DTA1XX_RELEASE_ADDR_OPER_REGS:
		IoData.m_pUserVirtual = NULL;
		ReturnStatus = Dta1xxReleaseAddrOperRegs(pFdo, filp, ChannelType, PortIndex, 
												&IoData.m_AddrInfo.m_pPaMmap,
												&IoData.m_AddrInfo.m_Size);
		break;

#if defined(CONFIG_X86_64)
	//-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_RELEASE_ADDR_OPER_REGS_32 .-.-.-.-.-.-.-.-.-.-.
	//
	// Decreases reference count of operational registers usage previously
	// getted by IOCTL_DTA1XX_GET_ADDR_OPER_REGS.
	// EBUSY is returned if reference count != 0
	//
	// This is the 32-bit version of the IOCTL, in case a 32-bit application calls
	//	in a 64-bit Linux environment.
	//
	case IOCTL_DTA1XX_RELEASE_ADDR_OPER_REGS_32:
		IoData.m_pUserVirtual = NULL;
		ReturnStatus = Dta1xxReleaseAddrOperRegs(pFdo, filp, ChannelType, PortIndex, 
												&pTmpPointer64,
												&IoData.m_AddrInfo_32.m_Size);

		IoData.m_AddrInfo_32.m_pPaMmap = (UInt64)pTmpPointer64;
		break;
#endif	/* CONFIG_X86_64 */

	//-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_REQUEST_EXCL_ACCESS -.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_REQUEST_EXCL_ACCESS:
		// Exclusive access is supported for TS_RX/TS_TX (standard HW channels like ASI)
		// For IP_RX/IP_TX, exclusive access is managed in a different way
		if (   IoData.m_ExclAccess.m_ChannelType == DTA1XX_TS_RX_CHANNEL
			|| IoData.m_ExclAccess.m_ChannelType == DTA1XX_TS_TX_CHANNEL)
		{
			ReturnStatus = Dta1xxRequestExclusiveAccess(
											pFdo, PortIndex, filp,
											IoData.m_ExclAccess.m_Request,
											&IoData.m_ExclAccess.m_Granted);
		}
		else
		{
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[-] IOCTL_DTA1XX_REQUEST_EXCL_ACCESS ChannelType=%d "
					    "NOT SUPPORTED", ChannelType );
#endif
			ReturnStatus = -EFAULT;	// NOT SUPPORTED
		}
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_RESET_SOFT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	case IOCTL_DTA1XX_RESET_SOFT:
		// Check port index
		if (PortIndex >= pFdo->m_NumNonIpChannels) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_RESET_SOFT: PortIndex=%d INVALID",
						PortIndex );
#endif
			ReturnStatus = -EFAULT;
			break;
		}

		// Get channel pointer
		pCh = &pFdo->m_Channel[PortIndex];

		// Process reset request.
		// Clear the latched status flags as well
		if ( ChannelType == DTA1XX_TS_TX_CHANNEL ) 	{
			ReturnStatus = Dta1xxTxReset(pCh, IoData.m_RstInfo.m_RstMode);
			// Clear all latached flags after a reset (donot care about return value)
			Dta1xxTxClearLatchedStatusFlags(pCh, 0xFFFFFFFF);
		}
		else if ( ChannelType == DTA1XX_TS_RX_CHANNEL ) {
			ReturnStatus = Dta1xxRxReset(pCh, IoData.m_RstInfo.m_RstMode);
			// Clear all latached flags after a reset (donot care about return value)
			Dta1xxRxClearLatchedStatusFlags(pCh, 0xFFFFFFFF);
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_FAILSAFE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// Changed failsafe state
	//
	case IOCTL_DTA1XX_SET_FAILSAFE:
		ReturnStatus = Dta1xxSetFailsafeState(pFdo, &IoData.m_Failsafe);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_IOCONFIG -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_SET_IOCONFIG:
		ReturnStatus = Dta1xxSetIoConfig(pFdo, &IoData.m_IoConfig, FALSE);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_IOCONFIG2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_SET_IOCONFIG2:
		ReturnStatus = Dta1xxSetIoConfig2(pFdo, &IoData.m_IoConfig2, FALSE);
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_MOD_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// LEGACY
	//
	case IOCTL_DTA1XX_SET_MOD_CONTROL:
		ReturnStatus = -EFAULT;	// NOT SUPPORTED
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_MOD_CONTROL2 -.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// Successor of IOCTL_DTA1XX_SET_MOD_CONTROL
	//
	case IOCTL_DTA1XX_SET_MOD_CONTROL2:
		ReturnStatus = Dta1xxTxSetModControl2(pFdo, PortIndex,
											  IoData.m_ModControl.m_ModType,
											  IoData.m_ModControl.m_ParXtra0,
											  IoData.m_ModControl.m_ParXtra1,
											  IoData.m_ModControl.m_ParXtra2);
		break;


	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_NWLOOPBACK -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	case IOCTL_DTA1XX_SET_NWLOOPBACK:
		if (IoData.m_NwLoopback.m_IpPortIndex >= pFdo->m_NumIpPorts) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_SET_NWLOOPBACK: IpPortIndex=%d "
					    "DOES NOT EXIST", IoData.m_NwLoopback.m_IpPortIndex);
#endif
			ReturnStatus = -EFAULT;
		} else {
			ReturnStatus = Dta1xxNwSetLoopback(
				&pFdo->m_NwStore[IoData.m_NwLoopback.m_IpPortIndex],
				&IoData.m_NwLoopback);
		}
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_NWSPEED -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_SET_NWSPEED:
		if (IoData.m_NwSpeed.m_IpPortIndex >= pFdo->m_NumIpPorts) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_SET_NWSPEED: IpPortIndex=%d "
					    "DOES NOT EXIST", IoData.m_NwLoopback.m_IpPortIndex );
#endif
			ReturnStatus = -EFAULT;
		} else {
			ReturnStatus = Dta1xxNwSetSpeed(
				&pFdo->m_NwStore[IoData.m_NwLoopback.m_IpPortIndex],
				&IoData.m_NwSpeed);
		}
		break;


	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_NWSTATCNT -.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_SET_NWSTATCNT:
		if (IoData.m_NwStatCnt.m_IpPortIndex >= pFdo->m_NumIpPorts) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_SET_NWSTATCNT: IpPortIndex=%d "
					 "DOES NOT EXIST", IoData.m_NwStatCnt.m_IpPortIndex );
#endif
			ReturnStatus = -EFAULT;
		} else {
			// FIXME: Dummy IRP variable required to comply function definition.
			IRP IrpDummy;

			ReturnStatus = Dta1xxNwGetSetStatCnt(
								&pFdo->m_NwStore[IoData.m_NwStatCnt.m_IpPortIndex],
								&IrpDummy,
								&IoData.m_NwStatCnt,
								1);
			if (ReturnStatus == STATUS_PENDING) {
				return ReturnStatus;
			}
		}
		break;
	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_OUTPUT_LEVEL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_SET_OUTPUT_LEVEL:
		ReturnStatus = Dta1xxTxSetOutputLevel(pFdo, PortIndex,
			IoData.m_OutputLevel.m_OutputLevel );
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_RF_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_SET_RF_CONTROL:
		ReturnStatus = Dta1xxTxSetRfControl( pFdo, PortIndex,
			IoData.m_RfControl.m_RfRate );
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_RF_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_SET_RF_MODE:
		ReturnStatus = Dta1xxTxSetRfMode( pFdo, PortIndex,
			IoData.m_RfMode.m_RfMode );
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_RATE2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	// Succesor of IOCTL_DTA1XX_SET_TS_RATE
	//
	case IOCTL_DTA1XX_SET_RATE2:
		ReturnStatus = Dta1xxTxSetRate2(pFdo, PortIndex,
										IoData.m_RateInfo2.m_ClockGenMode,
										IoData.m_RateInfo2.m_TsSymOrSampRate);
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_RX_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_SET_RX_MODE:
		ReturnStatus = Dta1xxRxSetRxMode(pFdo, PortIndex, IoData.m_RxMode.m_RxMode);
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_TS_RATE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// LEGACY, replaced by IOCTL_DTA1XX_SET_RATE2
	//
	case IOCTL_DTA1XX_SET_TS_RATE:
		ReturnStatus = -EFAULT;	// NOT SUPPORTED
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_TX_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	case IOCTL_DTA1XX_SET_TX_CONTROL:
		ReturnStatus = Dta1xxTxSetTxControl(pFdo, PortIndex,
											IoData.m_TxControl.m_TxControl);
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_TX_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_SET_TX_MODE:
		ReturnStatus = Dta1xxTxSetTxMode(pFdo, PortIndex,
										 IoData.m_TxMode.m_TxPacketMode,
										 IoData.m_TxMode.m_TxStuffMode);
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_TXPHASEINCR -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_SET_TXPHASEINCR:
		ReturnStatus = Dta1xxSetTxPhaseIncr(pFdo, &IoData.m_Port2Intpar);
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SHAREDBUF_ALLOC -.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_SHAREDBUF_ALLOC:
		ReturnStatus = Dta1xxSharedBufAlloc(pFdo, &IoData.m_ShBufCtrl);
		break;

#if defined(CONFIG_X86_64)
	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SHAREDBUF_ALLOC_32 .-.-.-.-.-.-.-.-.-.-.-.-
	//
	// This is the 32-bit version of the IOCTL, in case a 32-bit application calls
	//	in a 64-bit Linux environment.
	//
	case IOCTL_DTA1XX_SHAREDBUF_ALLOC_32:

		// Copy struct data in 64-bit compatible version
		ShBufCtrl_64.m_Purpose = IoData.m_ShBufCtrl_32.m_Purpose;
		ShBufCtrl_64.m_ChannelType = IoData.m_ShBufCtrl_32.m_ChannelType;
		ShBufCtrl_64.m_PortIndex = IoData.m_ShBufCtrl_32.m_PortIndex;
		ShBufCtrl_64.m_ChannelIndex = IoData.m_ShBufCtrl_32.m_ChannelIndex;
		ShBufCtrl_64.m_pSharedBuf = (UInt8*)((UInt64)IoData.m_ShBufCtrl_32.m_pSharedBuf);
		ShBufCtrl_64.m_SharedBufSize = IoData.m_ShBufCtrl_32.m_SharedBufSize;

		ReturnStatus = Dta1xxSharedBufAlloc(pFdo, &ShBufCtrl_64);
		break;
#endif	/* CONFIG_X86_64 */

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SHAREDBUF_FREE -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	case IOCTL_DTA1XX_SHAREDBUF_FREE:
		ReturnStatus = Dta1xxSharedBufFree(pFdo, &IoData.m_ShBufCtrl);
		break;

#if defined(CONFIG_X86_64)
	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SHAREDBUF_FREE_32 .-.-.-.-.-.-.-.-.-.-.-.-.
	//
	// This is the 32-bit version of the IOCTL, in case a 32-bit application calls
	//	in a 64-bit Linux environment.
	//
	case IOCTL_DTA1XX_SHAREDBUF_FREE_32:

		// Copy struct data in 64-bit compatible version
		ShBufCtrl_64.m_Purpose = IoData.m_ShBufCtrl_32.m_Purpose;
		ShBufCtrl_64.m_ChannelType = IoData.m_ShBufCtrl_32.m_ChannelType;
		ShBufCtrl_64.m_PortIndex = IoData.m_ShBufCtrl_32.m_PortIndex;
		ShBufCtrl_64.m_ChannelIndex = IoData.m_ShBufCtrl_32.m_ChannelIndex;
		ShBufCtrl_64.m_pSharedBuf = (UInt8*)((UInt64)IoData.m_ShBufCtrl_32.m_pSharedBuf);
		ShBufCtrl_64.m_SharedBufSize = IoData.m_ShBufCtrl_32.m_SharedBufSize;

		ReturnStatus = Dta1xxSharedBufFree(pFdo, &ShBufCtrl_64);
		break;
#endif	/* CONFIG_X86_64 */

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_VPD_DATA -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_VPD_DATA:
		// Acquire mutex for VPD access
		if ( 0==down_interruptible(&pFdo->m_VpdMutex) )
		{
			ReturnStatus = Dta1xxVpdIoctl(pFdo, &IoData.m_VpdData);
			up(&pFdo->m_VpdMutex);
		}
		else
			ReturnStatus = -EFAULT;
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_WRITE_VPD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	// This IOCTL is obsolete. Use IOCTL_DTA1XX_VPD_DATA instead.
	//
	case IOCTL_DTA1XX_WRITE_VPD:
		ReturnStatus = -EFAULT;	// NOT SUPPORTED
		break;

	default:
		return -ENOTTY;
		break;
	}

	//-.-.-.-.-.-.-.-.-.-.-.-.- Finally copy result back to user -.-.-.-.-.-.-.-.-.-.-.-.-

	if ( ReturnStatus==0 && ((_IOC_DIR(cmd)&_IOC_READ)!= 0) )
	{
		if ( copy_to_user((DTA1XX_IOCTL_DATA*)arg, &IoData, IoReqSize) )
		{
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "Dta1xxIoCtl: could not copy result to user" );
#endif
			ReturnStatus = -EFAULT;
		}
	}
	return ReturnStatus;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxIoctlI -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int Dta1xxIoCtlI(
	Int CardIndex,					// Index of card to use from g_DtaqxxCards
	UInt32 ChildId,					// ID of driver where the request is from 
	UInt IoControlCode,				// Ioctl command
	DTA1XX_IOCTL_DATA* pIoctlData)	// Ioctl data
{
	PDTA1XX_FDO  pFdoExt = (PDTA1XX_FDO) &g_Dta1xxCards[CardIndex];
	char*  IoctlStr;				// Mnemonic string for IOCTL code
	Int  ChannelType, PortIndex;
	Int  NwPortIndex = 0;
	Int  InReqSize = 0;				// Required length of IO buffer
	Int  OutReqSize = 0;			// Required length of IO buffer
	Channel*  pCh;					// Channel pointer
	Int ReturnStatus = STATUS_SUCCESS;	// Assume success
	UInt  InBufSize = 0;
	UInt  OutBufSize = 0;
	PNwStore  pNwStore;
	
	// Check command that this is one of our commands
	if ( DTA1XX_IOCTL_MAGIC!=_IOC_TYPE(IoControlCode) )
		return -ENOTTY;

	// Check memory
	if (  (_IOC_DIR(IoControlCode)&_IOC_READ) != 0 ) {
		OutBufSize = _IOC_SIZE(IoControlCode);
	}
	if ( (_IOC_DIR(IoControlCode)&_IOC_WRITE) != 0 ) {
		InBufSize = _IOC_SIZE(IoControlCode);
	}

	//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ FIRST BIG SWITCH +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

	// Determine expected length of data input and output buffer
	// For debug output: set IoctlStr to mnemonic representation of IOCTL
	// Some IOCTLs are checked in other files:
	//	IOCTL_DTA1XX_NWI_QUERY_INFO, DTA1XX_FUNCTION_NWI_SET_INFO,

	InReqSize = OutReqSize = 0;		// Default size is 0
	switch (IoControlCode) {

	case IOCTL_DTA1XX_DMA_USERMEM_READ:
		IoctlStr = "IOCTL_DTA1XX_DMA_USERMEM_READ";
		InReqSize = sizeof(DTA1XX_RW_INFO);
		OutReqSize = OutBufSize;
		break;

	case IOCTL_DTA1XX_DMA_USERMEM_WRITE:
		IoctlStr = "IOCTL_DTA1XX_DMA_USERMEM_WRITE";
		InReqSize = sizeof(DTA1XX_RW_INFO);
		OutReqSize = 0;
		break;

	case IOCTL_DTA1XX_GET_DRIVER_VERSION:
		IoctlStr = "IOCTL_DTA1XX_GET_DRIVER_VERSION";
		OutReqSize = sizeof(DTA1XX_DRIVER_VERSION);
		break;

	case IOCTL_DTA1XX_GET_LOC_INFO:
		IoctlStr = "IOCTL_DTA1XX_GET_LOC_INFO";
		OutReqSize = sizeof(DTA1XX_LOC_INFO);
		break;

	case IOCTL_DTA1XX_GET_NWSPEED:
		IoctlStr = "IOCTL_DTA1XX_GET_NWSPEED";
		InReqSize = sizeof(DTA1XX_NWSPEED);
		break;


	case IOCTL_DTA1XX_NWI_SET_SPEED:
		IoctlStr = "IOCTL_DTA1XX_NWI_SET_SPEED";
		InReqSize = sizeof(DTA1XX_NWI_SPEED);
		break;

	case IOCTL_DTA1XX_NWI_SET_VERSION:
		IoctlStr = "IOCTL_DTA1XX_NWI_SET_VERSION";
		InReqSize = sizeof(DTA1XX_DRIVER_VERSION);
		break;

	case IOCTL_DTA1XX_NWI_GET_STATUS:
		IoctlStr = "IOCTL_DTA1XX_NWI_GET_STATUS";
		InReqSize = sizeof(DTA1XX_NWI_STATUS) - (sizeof(ULONG)*4);
		break;

	case IOCTL_DTA1XX_NWI_QUERY_INFO:
		IoctlStr = "IOCTL_DTA1XX_NWI_QUERY_INFO";
		// InReqSize and OutReqSize are checked in NwNdis.c
		break;

	case IOCTL_DTA1XX_NWI_RESET:
		IoctlStr = "IOCTL_DTA1XX_NWI_RESET";
		InReqSize = sizeof(DTA1XX_NWI_RESET);
		break;
	
	case IOCTL_DTA1XX_NWI_SET_INFO:
		IoctlStr = "IOCTL_DTA1XX_NWI_SET_INFO";
		// InReqSize and OutReqSize are checked in NwNdis.c
		break;

	default:
		IoctlStr = "??UNKNOWN IOCTL CODE??";
		break;
	}

	// Some debug logging
#if LOG_LEVEL>1
	if (   IoControlCode != IOCTL_DTA1XX_NWI_QUERY_INFO
		&& IoControlCode != IOCTL_DTA1XX_NWI_SET_INFO
		&& IoControlCode != IOCTL_DTA1XX_DMA_USERMEM_WRITE		// TEMP
		&& IoControlCode != IOCTL_DTA1XX_DMA_USERMEM_READ){		// TEMP
			DTA1XX_LOG( KERN_INFO, "Dta1xxIoCtlI: %s In=%d (Rq=%d) Out=%d (Rq=%d)",
				        IoctlStr, InBufSize, InReqSize, OutBufSize, OutReqSize);
	}
#endif

	// Check buffer sizes
	if (InBufSize < InReqSize) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxIoCtlI: INPUT BUFFER TOO SMALL!" );
#endif
		return STATUS_INVALID_PARAMETER;
	}
	if (OutBufSize < OutReqSize) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxIoCtlI: OUTPUT BUFFER TOO SMALL!" );
#endif
		return STATUS_INVALID_PARAMETER;
	}
	
	//.-.-.-.-.- IOCTL_DTA1XX_DMA_USERMEM_READ, IOCTL_DTA1XX_DMA_USERMEM_WRITE -.-.-.-.-.-

	// Queue DMA requests
	if (IoControlCode == IOCTL_DTA1XX_DMA_USERMEM_WRITE) {

		ChannelType = (InReqSize==0) ? DTA1XX_TS_TX_CHANNEL
									 : pIoctlData->m_RwInfo.m_ChannelType;
		PortIndex     = (InReqSize==0) ? 0 : pIoctlData->m_RwInfo.m_PortIndex;
		
		// DMA Write
		if (ChannelType == DTA1XX_NW_TX_CHANNEL) {
			// NDIS Transmit
			if (PortIndex >= pFdoExt->m_NumIpPorts) {
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_DMA_USERMEM_WRITE: PortIndex=%d INVALID",
						    PortIndex );
#endif
				return STATUS_INVALID_PARAMETER;
			}
			pCh = pFdoExt->m_NwTx[PortIndex];

			if (pFdoExt->m_NwStore[PortIndex].m_LoopbackEnable) {
				// Skip all NDIS Tx packets in loopback mode.
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, 
					"IOCTL_DTA1XX_DMA_USERMEM_WRITE: Loopback mode enabled. Skip Ndis packets");
#endif
				return STATUS_SUCCESS;
			}
		} else {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO,
						"IOCTL_DTA1XX_DMA_USERMEM_WRITE: ChannelType=%d INVALID",
						ChannelType);
#endif
			return STATUS_INVALID_PARAMETER;
		}
		return Dta1xxDmaKernelMemWrite(pCh, pIoctlData->m_RwInfo.m_pBuffer, 
											pIoctlData->m_RwInfo.m_NumBytes2Transfer);
	}

	if (IoControlCode == IOCTL_DTA1XX_DMA_USERMEM_READ) {

		ChannelType = (InReqSize==0) ? DTA1XX_TS_RX_CHANNEL
									 : pIoctlData->m_RwInfo.m_ChannelType;
		PortIndex     = (InReqSize==0) ? 0 : pIoctlData->m_RwInfo.m_PortIndex;
	
		// DMA Read
		if (ChannelType == DTA1XX_NW_RX_CHANNEL) {
			// NDIS Receive
			if (PortIndex >= pFdoExt->m_NumIpPorts) {
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_DMA_USERMEM_READ: PortIndex=%d INVALID",
						    PortIndex );
#endif
				return STATUS_INVALID_PARAMETER;
			}
		}
		else
		{
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_DMA_USERMEM_READ: ChannelType=%d INVALID",
					    ChannelType);
#endif
			return STATUS_INVALID_PARAMETER;
		}
		pFdoExt->m_NwStore[PortIndex].m_pNwRxQueue = (Dta1xxNwBuf*)pIoctlData->m_RwInfo.m_pBuffer;
			/*Dta1xxInternalQueueInsertIrp(&pFdoExt->m_NwStore[PortIndex].m_NwRxQueue,
											pIrp);*/

		return STATUS_SUCCESS;
	}

	//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ SECOND BIG SWITCH +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

	switch (IoControlCode) {

	//-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_DRIVER_VERSION -.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	case IOCTL_DTA1XX_GET_DRIVER_VERSION:	// Get driver versioning info
		pIoctlData->m_DriverVersion.m_VersionMajor = DTA1XX_VERSION_MAJOR;
		pIoctlData->m_DriverVersion.m_VersionMinor = DTA1XX_VERSION_MINOR;
		pIoctlData->m_DriverVersion.m_VersionMicro = DTA1XX_VERSION_MICRO;
		pIoctlData->m_DriverVersion.m_VersionBuild = DTA1XX_VERSION_BUILD;
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_LOC_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// Get location info: PCI bus#, Slot#, Device ID, ...
	//
	case IOCTL_DTA1XX_GET_LOC_INFO:
		pIoctlData->m_LocInfo.m_DeviceId	= pFdoExt->m_DeviceId;
		pIoctlData->m_LocInfo.m_VendorId	= pFdoExt->m_VendorId;
		pIoctlData->m_LocInfo.m_SubSystemId	= pFdoExt->m_SubSystemId;
		pIoctlData->m_LocInfo.m_SubVendorId	= pFdoExt->m_SubVendorId;
		pIoctlData->m_LocInfo.m_BusNumber	= pFdoExt->m_BusNumber;
		pIoctlData->m_LocInfo.m_SlotNumber	= pFdoExt->m_SlotNumber;
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_NWSPEED -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_GET_NWSPEED:
		if (pIoctlData->m_NwSpeed.m_IpPortIndex >= pFdoExt->m_NumIpPorts) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_GET_NWSPEED: IpPortIndex=%d "
					    "DOES NOT EXIST", pIoctlData->m_NwSpeed.m_IpPortIndex );
#endif
			ReturnStatus = -EFAULT;
		} else {
			ReturnStatus = Dta1xxNwGetSpeed(
				&pFdoExt->m_NwStore[pIoctlData->m_NwSpeed.m_IpPortIndex],
				&pIoctlData->m_NwSpeed);
		}
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_NDIS_RESET -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	case IOCTL_DTA1XX_NWI_RESET:
		NwPortIndex = pIoctlData->m_NwiReset.m_IpPortIndex;
		if (NwPortIndex>=0 && NwPortIndex<pFdoExt->m_NumIpPorts) {
			// Reset Ndis data structure.
			pNwStore = &pFdoExt->m_NwStore[NwPortIndex];

			// Reset link state to unknown, used for 
			// first status indication IRP asked by NDIS
			pNwStore->m_NwiLink = 2;

			// Reset packet filter
			pNwStore->m_NwiPacketFilter = 0; 

			// Reset multicast list
			pNwStore->m_NumMulticast = 0;
			
			// Disable Ndis receive
			pNwStore->m_EnableNwiRx = FALSE;

			// Set admin status to up
			pNwStore->m_AdminStatus = DTA1XX_NW_ADMINSTATUS_UP;

		} else {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_NWI_RESET: NwPortIndex=%d INVALID",
					    NwPortIndex);
#endif
			ReturnStatus = STATUS_INVALID_PARAMETER;
		}
		break;

	
		//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_NWI_SET_SPEED -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	case IOCTL_DTA1XX_NWI_SET_SPEED:
		NwPortIndex = pIoctlData->m_NwiSpeed.m_IpPortIndex;
		if (NwPortIndex>=0 && NwPortIndex<pFdoExt->m_NumIpPorts) {
			// Reset ForceSpeedEnable if NDIS is forcing the speed.
			pNwStore = &pFdoExt->m_NwStore[NwPortIndex];

			pNwStore->m_ForceSpeedEnable = FALSE;
			pNwStore->m_LoopbackEnable = FALSE;

			// Update speed status
			pNwStore->m_NwiSpeed = pIoctlData->m_NwiSpeed.m_Speed;
			pNwStore->m_SpeedPending = TRUE;
		} else {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_NWI_SET_SPEED: NwPortIndex=%d INVALID",
					    NwPortIndex);
#endif
			ReturnStatus = STATUS_INVALID_PARAMETER;
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_NWI_SET_VERSION -.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// Set the NW driver version
	//
	case IOCTL_DTA1XX_NWI_SET_VERSION:
		g_Globals.m_NwVersionMajor = pIoctlData->m_DriverVersion.m_VersionMajor;
		g_Globals.m_NwVersionMinor = pIoctlData->m_DriverVersion.m_VersionMinor;
		g_Globals.m_NwVersionMicro = pIoctlData->m_DriverVersion.m_VersionMicro;
		g_Globals.m_NwVersionBuild = pIoctlData->m_DriverVersion.m_VersionBuild;
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_NWI_GET_STATUS -.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_NWI_GET_STATUS:
		NwPortIndex = pIoctlData->m_NwiStatus.m_IpPortIndex;
		if (NwPortIndex>=0 && NwPortIndex<pFdoExt->m_NumIpPorts) {
			pNwStore = &pFdoExt->m_NwStore[NwPortIndex];

			pNwStore->m_pStatus = &pIoctlData->m_NwiStatus;
			
			// Trigger first link status
			if (pNwStore->m_NwiLink == 2) {
				if (pNwStore->m_PhyLink == 2) {
					pNwStore->m_NwiLink = DTA1XX_ETH_NW_LINK_DOWN;
				} else {
					pNwStore->m_NwiLink = pNwStore->m_PhyLink;
				}
				pNwStore->m_LinkPending = TRUE;
			}
			break;
		} else {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "IOCTL_DTA1XX_NWI_GET_STATUS: NwPortIndex=%d INVALID",
					    NwPortIndex);
#endif
			ReturnStatus = STATUS_INVALID_PARAMETER;
		}
		break;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_NWI_QUERY_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	case IOCTL_DTA1XX_NWI_QUERY_INFO:
		if (InBufSize != 0)
			InBufSize+= pIoctlData->m_NwiInfo.m_BytesWritten - sizeof(ULONG);
		if (OutBufSize != 0)
			OutBufSize+= pIoctlData->m_NwiInfo.m_BytesWritten - sizeof(ULONG);

		if (TRUE) {
			ULONG ulOutBufSize = OutBufSize;
			ReturnStatus = Dta1xxNwNdisIoctlQueryInfo(InBufSize, &ulOutBufSize,
												  &pIoctlData->m_NwiInfo, pFdoExt);
			OutBufSize = (UInt)ulOutBufSize;
		}
		break;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_NWI_SET_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	case IOCTL_DTA1XX_NWI_SET_INFO:
		if (InBufSize != 0)
			InBufSize+= pIoctlData->m_NwiInfo.m_BytesWritten - sizeof(ULONG);
		if (OutBufSize != 0)
			OutBufSize+= pIoctlData->m_NwiInfo.m_BytesWritten - sizeof(ULONG);
		ReturnStatus = Dta1xxNwNdisIoctlSetInfo(InBufSize, OutBufSize,
												  &pIoctlData->m_NwiInfo, pFdoExt);
		break;

	default:
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxIoCtlI: IoControlCode=0x%x is not implemented",
				    IoControlCode);
#endif
		ReturnStatus = STATUS_NOT_IMPLEMENTED;
		break;
	}

	return ReturnStatus;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxFirmwareReboot -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Reboot firmware on PCI cards with FPGA
//
Int  Dta1xxFirmwareReboot(
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	return -EFAULT;	// NOT IMPLEMENTED
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxI2cTransferIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Dispatch routine for I2C transfer IOCTL commands
//
Int  Dta1xxI2cTransferIoctl(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	DTA1XX_I2C_TRANSFER*  pI2cTransf)
{
	char*  CmdStr;						// Mnemonic string for Command
	Int  ReturnStatus = 0;

	switch (pI2cTransf->m_Command) {

	case DTA1XX_I2CCMD_READ:
		CmdStr = "DTA1XX_I2CCMD_READ";
		break;

	case DTA1XX_I2CCMD_WRITE:
		CmdStr = "DTA1XX_I2CCMD_WRITE";
		break;

	default:
		CmdStr = "??UNKNOWN I2CCMD CODE??";
		break;
	}


#if LOG_LEVEL > 1
	DTA1XX_LOG( KERN_INFO, "Dta1xxI2cTransferIoctl: command=%s", CmdStr );
#endif

	switch (pI2cTransf->m_Command)
	{
	case DTA1XX_I2CCMD_READ:
		ReturnStatus = Dta1xxI2cRead(pFdo, pI2cTransf->m_DeviceAddr,
									 pI2cTransf->m_Length, pI2cTransf->m_Buf);
		break;

	case DTA1XX_I2CCMD_WRITE:
		ReturnStatus = Dta1xxI2cWrite(pFdo, pI2cTransf->m_DeviceAddr,
									  pI2cTransf->m_Length, pI2cTransf->m_Buf);
		break;

	default:
		ReturnStatus = -ENOTTY;
	}
	return ReturnStatus;
}
