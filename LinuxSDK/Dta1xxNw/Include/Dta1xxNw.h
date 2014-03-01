//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dta1Nw.h *#*#*#*#*#*#*#*#*# (C) 2000-2005 DEKTEC

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	RD	2007-08-21	Created

#ifndef __DTA1XXNW_H
#define __DTA1XXNW_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#ifdef __KERNEL__

#include <linux/version.h>
#include <linux/slab.h>			// kmalloc ()
#include <linux/fs.h>			// inode, file
#include <linux/errno.h>		// error codes
#include <linux/wait.h>			// wait queue
#include <linux/sched.h>		// wait queue
#include <linux/ioctl.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
#include <linux/semaphore.h>		// semaphore
#else
#include <asm/semaphore.h>			// semaphore
#endif

#endif

#include "DriverTypes.h"
#include "DriverVersion.h"
#include "Dta1xxRegs.h"
#include "CrossPlatform.h"
#include "Dta1xxCommon.h"


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- LOG LEVEL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// LOG LEVEL with KdPrint
//  0	Off
//	1	Minimal (setting-up / breaking-down of channels)
//	2	Average (up to 10 per second)
//	3	All

// DMA related

// Other logs (0=off not supported)
#define LOG_LEVEL				0

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions and constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Module ID information
#define DRIVER_NAME				"Dta1xxNw"		// Name of the driver

											// supported
// DTA-1XX (sub-)vendor and (sub-)device ID's
#define DTA1XX_VENDORID			0x14B4		// Old DEKTEC vendor ID
#define DTA1XX_VENDORID_DEKTEC	0x1A0E		// DEKTEC vendor ID

#define DTA160_DEVICEID		0x00A0			// DTA-160 device ID
#define DTA2160_DEVICEID	0x0870			// DTA-2160 device ID

// Tx/Rx buffers
#define	DTA1XXNW_TXBUFSIZE		((128*1024) - (3 + sizeof(void*)))
#define	DTA1XXNW_RXBUFSIZE		((128*1024) - (3 + sizeof(void*)))

// Maximum supported number of PCI and USB devices
#define  MaxPciDevices	50
#define	 TX_TIMEOUT		(400*HZ/1000)	 	// Time in jiffies

// Driver log message helpers
#define DTA1XXNW_LOG(l,m,args...)			printk(l DRIVER_NAME ": " m "\n", ## args);

typedef struct _Dta1xxNw_Private {
	int  m_Index;							// Network driver index
	int	 m_Dta1xxIndex;						// Dta1xx connection index
	int  m_PortIndex;						// Network port index
	int  m_Connected;						// Connection status
	int  m_TypeNumber;						// Type number of card
	struct net_device*  m_pNwDevice;		// Network device
	struct net_device_stats  m_NetStats;	// Network status
	spinlock_t m_Lock;						// Lock
	struct task_struct*  m_pRxThread;		// Rx thread
	struct task_struct*  m_pTxThread;		// Tx thread
	Dta1xxNwBuf	 m_NwRxBuf;					// Rx buffer
	Dta1xxNwBuf	 m_NwTxBuf;					// Tx buffer
	DTA1XX_NWI_STATUS  m_Status;			// Link status
} Dta1xxNw_Private;

#define ETH_LENGTH_OF_ADDRESS	ETH_ALEN  

#endif //__DTA1XXNW_H
