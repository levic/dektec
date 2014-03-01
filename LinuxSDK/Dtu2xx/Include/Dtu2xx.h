//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dtu2xx.h *#*#*#*#*#*#*#*#*# (C) 2000-2010 DekTec
//
// Main header file for Dtu2xx driver.

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dtu2xx driver, please refer to the Dta1xx readme

#ifndef __DTU2XX_H
#define __DTU2XX_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

#ifdef __KERNEL__

#include <linux/version.h>
#include <linux/slab.h>		// kmalloc ()
#include <linux/fs.h>		// inode, file
#include <linux/errno.h>	// error codes
#include <linux/wait.h>		// wait queue
#include <linux/sched.h>	// wait queue
#include <linux/time.h>		// CURRENT_TIME macro
#include <linux/ioctl.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,31)
	#include <linux/smp_lock.h>
#else
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
	#include <linux/semaphore.h>		// semaphore
	#else
	#include <asm/semaphore.h>			// semaphore
	#endif
#endif
#include <asm/types.h>
#include <asm/atomic.h>
#include <asm/uaccess.h>
#include <linux/usb.h>
#include <linux/kref.h>

#ifdef CONFIG_COMPAT
	#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,28)
	#include <asm/compat.h>				// compat_ptr
	#else 
		#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24)
		#include <asm-x86/compat.h>		// compat_ptr
		#else
		#include <asm-x86_64/compat.h>	// compat_ptr
		#endif
	#endif
#endif // #ifdef CONFIG_COMPAT


#endif // #ifdef __KERNEL__

#include "DriverTypes.h"
#include "DriverVersion.h"
#include "Dtu2xxCodes.h"
#include "Dtu2xxCommon.h"
#include "Dtu2xxRegs.h"
#include "EzUsb.h"

// Module ID information
#define DRIVER_NAME			"Dtu2xx"		// Name of the driver
#define DTU2XX_MAJOR		0               // Default major number
#define DTU2XX_MINOR		176             // Driver minor number

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- LOG LEVEL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// LOG LEVEL with KdPrint
//  0	Off
//	1	Minimal (setting-up / breaking-down of channels)
//	2	Average (up to 10 per second)
//	3	All

// Other logs (0=off not supported)
#ifdef DEBUG
#define LOG_LEVEL					1
#define TRACE               		DTU2XX_LOG(KERN_INFO,"trace: %s, %d",__FILE__,__LINE__)
#else
#define LOG_LEVEL					0
// do not define TRACE in order to get compile errors on every remaining TRACE in the code
#endif

// DMA related
#define LOG_LEVEL_DMA				0
// Modulation related
#define LOG_LEVEL_MODULATION        0

// Driver log message helpers
#define DTU2XX_LOG(l,m,args...)		printk(l DRIVER_NAME ": " m "\n", ## args)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Our vendor ID
#define DTU2XX_VENDORID                 0x1297

// DTU-2XX product IDs
#define PID_DTU205_NOFIRMWARE		0x2050		// Uninitialised DTU-205 PID
#define PID_DTU205					0x0205		// Initialised DTU-205 PID
#define PID_DTU225_NOFIRMWARE_OLD	0x0100		// Uninitialised DTU-225 PID (for
												// backward compatibility)
#define PID_DTU225_NOFIRMWARE		0x2190		// Uninitialised DTU-225 PID
#define PID_DTU225_OLD				0x0200		// Initialised DTU-225 PID  (for
												// backward compatibility)
#define PID_DTU215					0x020F		// Initialised DTU-225 PID
#define PID_DTU225					0x0219		// Initialised DTU-225 PID
#define PID_DTU234_NOFIRMWARE		0x2220		// Uninitialised DTU-234 PID
#define PID_DTU234					0x0222		// Initialised DTU-234 PID
#define PID_DTU235_NOFIRMWARE		0x2230		// Uninitialised DTU-235 PID
#define PID_DTU235					0x0223		// Initialised DTU-235 PID
#define PID_DTU236					0x0224		// DTU-236 PID
#define PID_DTU245					0x022D		// DTU-245 PID
#define PID_UNIT_EEPROM				0x8613		// PID of an uninitialised DTU-2xx
												// device (=Default PID of FX2)

#define PID_DTU2X5_MANUF			0x0237		// DTU-2x5 in manuf test mode
#define PID_DTU2XX_MANUF			0x0263		// DTU-2XX device with manuf code

// BULK-transfer direction
#define  DTU2XX_BULK_READ			0
#define  DTU2XX_BULK_WRITE			1

// Maximum total number of channels
#define DTU2XX_MAX_CHANNELS			4

// Align bulk transfer to:
#define DTU2XX_BULK_TRANSFER_ALIGN	1024
// Size of Rx channel ring buffers (to store none alligned data)
#define DTU2XX_RX_RING_BUF_SIZE		(2 * DTU2XX_BULK_TRANSFER_ALIGN)
// Size of Tx channel ring buffers (to store none alligned data)
#define DTU2XX_TX_RING_BUF_SIZE		(2 * DTU2XX_BULK_TRANSFER_ALIGN)

// Channel capabitilies
#define DTU2XX_CHCAP_IN_PORT		0x0001
#define DTU2XX_CHCAP_OUT_PORT		0x0002
#define DTU2XX_CHCAP_BIDIR_PORT		0x0003
#define DTU2XX_CHCAP_DBLBUF			0x0010	// Must be or-ed with one of the cap_x_port
#define DTU2XX_CHCAP_FAILSAFE		0x0020	// Must be or-ed with one of the cap_x_port
#define DTU2XX_CHCAP_LOOPTHR		0x0040	// Must be or-ed with one of the cap_x_port
#define	DTU2XX_CHCAP_DIVERSITY		0x0080	// Must be or-ed with one of the cap_x_port
#define DTU2XX_CHCAP_SHARED			0x0100	// Must be or-ed with one of the cap_x_port

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Macros -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a)				(sizeof(a)/sizeof((a)[0]))
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- VPD Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Valid VPD characters
#define  VALID_VPD_KEYW_CHAR(c) 	(((c)>='0' && (c)<='9') || ((c)>='A' && (c)<='Z'))

// *** PLX-attached EEPROM

// Start and end of VPD sections
#define  DTU2XX_VPD_ID_START			0x58	// Read-only section directly follows VPD-ID
//#define  DTU2XX_VPD_ID_START_NEW		0x63	// Read-only section directly follows VPD-ID

#define  DTU2XX_VPD_RW_START			0x100
#define  DTU2XX_VPD_END					(DTU2XX_VPD_BUF_SIZE - 1) // See Dtu2xxCommon.h

// Section lengths
#define  DTU2XX_VPD_RW_LENGTH			(DTU2XX_VPD_END - DTU2XX_VPD_RW_START)

// General helpers
#define IN
#define OUT
#define INOUT

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Helper Structs +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTU2XX_INTEL_HEX_RECORD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// INTEL hex record structure used for EzUsb firmware
//
#define MAX_INTEL_HEX_RECORD_LENGTH 16
typedef struct _INTEL_HEX_RECORD
{
   UInt8  Length;
   UInt16 Address;
   UInt8  Type;
   UInt8  Data[MAX_INTEL_HEX_RECORD_LENGTH];
} DTU2XX_INTEL_HEX_RECORD, *PDTU2XX_INTEL_HEX_RECORD;

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTU2XX_RING_BUFFER +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Ring buffer used for bulk transfers
//
typedef struct _DTU2XX_RING_BUFFER
{
	UInt8*  m_pStart;			// Buffer start
	UInt8*  m_pWrPtr;			// Buffer write pointer
	UInt8*  m_pRdPtr;			// Buffer read pointer
	Int  m_Load;				// Buffer load (# bytes)
	Int  m_TotalSize;			// Size of the buffer
	spinlock_t  m_SpinLock;		// Buffer lock to protect pointers and or load

} DTU2XX_RING_BUFFER, *PDTU2XX_RING_BUFFER;

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ BulkTransferCh +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Variables to describe and control a single bulk transfer channel.
//
typedef struct _BulkTransferCh {

	Int  m_TransfDir;			// Direction: DTU2XX_DIRECTION_READ or
								// DTU2XX_DIRECTION_WRITE
	DTU2XX_RING_BUFFER  m_Buffer;	// The channels ring buffer

} BulkTransferCh;

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Channel +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Describes a single channel, which is either:
//	- Transport-Stream Receive or Transmit Channel (User <-> PCI Card)
//
typedef struct _Channel {

	// Identity information: Reference back to device object and index
	struct _DTU2XX_FDO*  m_pFdo;
	Int  m_ChannelType;				// DTU2XX_CHANNEL_UNUSED/DTU2XX_TS_TX_CHANNEL/...
	Int  m_PortIndex;				// Index in Device Extension array
	Int  m_Capability;				// Channel capability
	Int  m_IoConfig;				// Current IO config
	Int  m_IoConfigPar;				// Extra paremeter for IO config

	// Exclusive Access
	struct semaphore  m_ExclAccLock; // Protect updates to channel members
	Int	 m_ExclAccess;				// Does someone have exclusive access?
	struct file*  m_pExclAccFileObject; // File object used to request exclusive access

	// Status Flags
	Int  m_Status;					// Actual value of status flags
	Int  m_Latched;					// Latched value of status flags
	Int  m_FlagsToClear;			// Flags to be cleared

	// BULK transfer members
	BulkTransferCh  m_BulkTransf;	// Bulk transfer channel for reading from channel
	UInt8*  m_pTempBuf;				// Temporary buffer used internally to allign
									// data	during Bulk transfers
	Int  m_TempBufSize;				// Size of tempory buffer

	// Rx-channel specific
	Dtu2xxRx  m_RxRegs;				// Cached receive registers

	// Tx-channel specific
	Dtu2xxTx  m_TxRegs;				// Cached transmit registers

	Ad9789Registers  m_Ad9789;		// Cached AD9789 SPI registers (only used on
									// DTU-215)

	// Modulation
	// Current modulation parameters for DTU-215
	// This is for remembering the last modulation settings between applications
	// In device-extension initialisation, an attempt is made to initialise these
	// variables in a meaningful way. If this is not possible => initialise to -1
	Int  m_ModType;					// Modulation type (to know whether this is OFDM)
	Int  m_ParXtra[3];				// Extra parameters
	Int  m_TsSymOrSampRate;			// TS, Symbol- or Sample rate
									// This is a shadow variable: value may be derived
									// from hardware too, but with less precision
	Int64  m_RfFreq;				// RF-frequency (in Hz)
	Int  m_OutpLeveldBm;			// Output level in dBm (expressed in 0.1 dB units)

	// Tx FIFO load extrapolation
	// Used in I/Q sample mode only
	BOOLEAN  m_EnaFifoLoadExtrap;	// Enable Tx FIFO load extrapolation
	Int  m_RpFifoLoad;				// Reference point: FIFO load
	UInt64  m_RpTimestamp;			// Reference point: time stamp
	Int  m_WrittenAfterRp;			// #bytes written after reference point is taken
	Int  m_NumTxBytesPerSec;		// #bytes transmitted per second
	struct semaphore  m_LoadExtrapLock;	// Protect updates to load-extrapolation members

} Channel;

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTU2XX_FDO +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Dtu2xx Functional Device Object Extension.
//
typedef struct _DTU2XX_FDO
{
	// Cross-reference pointers
	struct usb_device*  m_pUDev;	// Corresponding Physical Device Object (USB device)

	// USB interfaces/status info
	struct usb_interface*  m_pUsbItf; // USB interface
    struct kref  m_KRef;			// Keep track of reference counts
    struct usb_class_driver*  m_pUsbClass; // USB class

	// State Info
    BOOLEAN  m_Removed;				// True if device has been removed
    BOOLEAN  m_Started;				// True if device has been started
	BOOLEAN  m_EzUsbFirmware;		// True if the EzUsb firmware has been downloaded
	BOOLEAN  m_IsDtu2x5Hw;			// True if the device uses DTU-2x5 hardware

	// Location info
	UInt32  m_UsbAddress;			// The USB address of the device
	UInt16  m_DeviceId;				// Device ID
	UInt16  m_VendorId;				// Vendor ID
	UInt16  m_SubSystemId;			// Subsystem ID (not relevant for USB)
	UInt16  m_SubVendorId;			// Subsystem Vendor ID (not relevant for USB)
	Int  m_TypeNumber;				// Type number in decimal, e.g. 225
	Int  m_FirmwareVersion;			// Firmware Version (= Altera revision),
									// e.g. 3 for "Firmware Version 3"
	UInt8  m_EepromCache[DTU2XX_VPD_BUF_SIZE];	// Buffer (cache) for EEPROM contents
	UInt8*  m_pVpdEeprom;			// Pointer to start VPD data
	Int  m_VpdIdOffset;				// Offset of start VPD ID
//	Int  m_VpdEepromSize;			// Size of VPD area
	struct semaphore  m_VpdMutex;	// Mutex to protect simultaneous read/write from/to VPD

	// Register cache
	Dtu2xxGen m_GenRegs;			// Cached general registers

	// I2C mutex and lock info
	struct semaphore  m_I2cMutex;				// Mutex to protect against simultaneous access 
										// to I2C interface
	struct file*  m_pI2cExclAccFileObj;	// File object that owns the I2C lock
	spinlock_t  m_I2cExclAccSpinLock;	// Protect updates to m_pI2cExclAccFileObj
	Int  m_I2cExclAccRecursiveCount;	// Counts number of recursive exclusive access

	// uCode upload state
	spinlock_t m_UCodeUploadStateLock;
	struct file*  m_UCodeUploadStateLockFileObj;
	Int m_UCodeUploadState;

	// Channel-specific data
	Int  m_NumChannels;				// Total #channels
	Channel  m_Channel[DTU2XX_MAX_CHANNELS];	// Actual channel data

} DTU2XX_FDO, *PDTU2XX_FDO;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DeviceIoControl.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
Int  Dtu2xxIoCtlReset(IN PDTU2XX_FDO);
int  Dtu2xxDeviceControl(struct inode *inode, struct file *filp,unsigned int cmd,
                         unsigned long arg);
#ifdef CONFIG_COMPAT
long  Dtu2xxCompatDeviceControl(struct file *filp,unsigned int cmd, unsigned long arg);
#endif
Int  Dtu2xxIoCtlDeviceLedControl(IN PDTU2XX_FDO, IN Int);
Int  Dtu2xxIoCtlGetDeviceInfo(IN PDTU2XX_FDO, OUT Int*, OUT Int*);
Int  Dtu2xxIoCtlReadRegister(IN PDTU2XX_FDO, IN Int, OUT UInt32*);
Int  Dtu2xxIoCtlWriteRegister(IN PDTU2XX_FDO, IN Int, IN UInt32);
Int  Dtu2xxIoCtlGetUsbSpeed(IN PDTU2XX_FDO, OUT UInt*);
Int  Dtu2xxIoCtlEepromControl(IN PDTU2XX_FDO, IN UInt);
Int  Dtu2xxIoCtlUploadFirmware(IN PDTU2XX_FDO, IN UInt);
Int  Dtu2xxIoCtlI2CRead(IN PDTU2XX_FDO, IN struct file*, OUT UInt8*, IN UInt, IN UInt);
Int  Dtu2xxIoCtlI2CWrite(IN PDTU2XX_FDO, IN struct file*, IN UInt8*, IN UInt, IN UInt);
Int  Dtu2xxIoCtlUCodeUploadState(IN PDTU2XX_FDO, IN struct file*, IN Int, IN Int, OUT Int*, OUT Int*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xx.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void __exit Dtu2xxCleanupModule(void);
void Dtu2xxDecrUsageCount(DTU2XX_FDO*);
void Dtu2xxDevDelete(struct kref*);
void Dtu2xxDisconnect(struct usb_interface*);
int  __init Dtu2xxInitModule(void);
void Dtu2xxIncrUsageCount(DTU2XX_FDO*);
int  Dtu2xxOpen(struct inode*, struct file*);
int  Dtu2xxProbe(struct usb_interface*, const struct usb_device_id*);
ssize_t Dtu2xxRead(struct file*, char __user*, size_t, loff_t*);
int  Dtu2xxRelease(struct inode*, struct file*);
ssize_t Dtu2xxWrite(struct file*, const char __user*, size_t, loff_t*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu215.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
Int  Dtu2xxInitDtu215Hardware(IN PDTU2XX_FDO);
Int  Dtu2xxInitDtu215MainPowerSupply(IN PDTU2XX_FDO);
void  Dtu215GetMinMaxOutpLevel(IN Channel*, OUT Int*, OUT Int*);
Int  Dtu215FrequencyResponseCompensation(IN Channel*);
Int  Dtu215SetModControl(IN Channel*, IN Int, IN Int, IN Int, IN Int);
Int  Dtu215SetRfControl(IN Channel*, IN Int64);
Int  Dtu215SetRfMode(IN Channel*, IN Int);
Int  Dtu215SetSymSampleRate(IN Channel*, IN Int);
Int  Dtu215SpiRegisterRead(IN Channel*, IN UInt16, IN UInt, OUT UInt8*);
Int  Dtu215SpiRegisterWrite(IN Channel*, IN UInt16, IN UInt, IN UInt8*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu234.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
Int  Dtu2xxInitDtu234Hardware(IN PDTU2XX_FDO);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Exclusive.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int  Dtu2xxCheckExclusiveAccess(IN PDTU2XX_FDO, IN struct file*);
Int  Dtu2xxRequestExclusiveAccess(IN PDTU2XX_FDO, IN struct file*, IN Int,
									   IN Int, OUT Int*);
Int  Dtu2xxI2cReqExclAccess(IN PDTU2XX_FDO, IN struct file*, IN Int, OUT Int*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Flags.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Init.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int  Dtu2xxInitDevice(IN PDTU2XX_FDO);
Int  Dtu2xxProgramEzUsb(IN PDTU2XX_FDO, IN const DTU2XX_INTEL_HEX_RECORD*);
Int  Dtu2xxProgramAltera(IN PDTU2XX_FDO, IN UInt, IN const UInt8*);
Int  Dtu2xxAllocRingBuffer(IN PDTU2XX_RING_BUFFER, IN Int);
//Int  Dtu2xxFreeRingBuffer(IN PDTU2XX_RING_BUFFER, IN Int);
Int  Dtu2xxGetDeviceAddress(IN PDTU2XX_FDO);
Int  Dtu2xxGetFirmwareVersion(IN PDTU2XX_FDO);
Int  Dtu2xxCacheOpRegs(IN PDTU2XX_FDO);
BOOLEAN  Dtu2xxIsFwLoaded(IN PDTU2XX_FDO);
Int  Dtu2xxInitModChannel(IN Channel*);
Int  Dtu2xxUploadFirmware(IN PDTU2XX_FDO, IN UInt);
Int  Dtu2xxGetIoConfig(IN PDTU2XX_FDO, DTU2XX_IOCONFIG*);
Int  Dtu2xxSetIoConfig(IN PDTU2XX_FDO, DTU2XX_IOCONFIG*, BOOLEAN);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- RxIoCtl.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
Int  Dtu2xxRxIoCtlLedControl(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxRxIoCtlEqualise(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxRxIoCtlGetFifoLoad(IN PDTU2XX_FDO, IN Int, OUT Int*);
Int  Dtu2xxRxIoCtlReset(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxRxIoCtlRxPolarity(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxRxIoCtlGetFlags(IN PDTU2XX_FDO, IN Int, OUT Int*, OUT Int*);
Int  Dtu2xxRxIoCtlClearFlags(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxRxIoCtlGetStatus(IN PDTU2XX_FDO, IN Int, OUT Int*, OUT Int*, OUT Int*,
								 OUT Int*, OUT Int*);
Int  Dtu2xxRxIoCtlSetRxControl(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxRxIoCtlGetRxControl(IN PDTU2XX_FDO, IN Int, OUT Int*);
Int  Dtu2xxRxIoCtlSetRxMode(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxRxIoCtlGetRxMode(IN PDTU2XX_FDO, IN Int, OUT Int*);
Int  Dtu2xxRxIoCtlGetTsRateBps(IN PDTU2XX_FDO, IN Int, OUT Int*);
Int  Dtu2xxRxIoCtlSetLoopBackMode(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxRxIoCtlBulkRead(IN PDTU2XX_FDO, IN Int, IN Int, OUT Int*, OUT UInt8*);
Int  Dtu2xxRxIoCtlGetMaxFifoSize(IN PDTU2XX_FDO, IN Int, OUT Int*);
Int  Dtu2xxRxIoCtlGetViolationCount(IN PDTU2XX_FDO, IN Int, OUT Int*);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TxIoCtl.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
Int  Dtu2xxTxIoctlAd9789Write(IN PDTU2XX_FDO, IN Int, IN Int, IN Int, IN UInt8*);
Int  Dtu2xxTxIoCtlReset(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxTxIoCtlGetFlags(IN PDTU2XX_FDO, IN Int, OUT Int*, OUT Int*);
Int  Dtu2xxTxIoCtlClearFlags(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxTxIoCtlSetTsRateBps(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxTxIoCtlSetTxControl(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxTxIoCtlSetTxMode(IN PDTU2XX_FDO, IN Int, IN Int, IN Int);
Int  Dtu2xxTxIoCtlSetSourceSelect(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxTxIoCtlTxPolarity(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxTxIoCtlSetFifoSize(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxTxIoCtlGetFifoLoad(IN PDTU2XX_FDO, IN Int, OUT Int*);
Int  Dtu2xxTxIoCtlGetTsRateBps(IN PDTU2XX_FDO, IN Int, OUT Int*);
Int  Dtu2xxTxIoCtlGetTxControl(IN PDTU2XX_FDO, IN Int, OUT Int*);
Int  Dtu2xxTxIoCtlGetTxMode(IN PDTU2XX_FDO, IN Int, OUT Int*, OUT Int*);
Int  Dtu2xxTxIoCtlReadLoopBackData(IN PDTU2XX_FDO, IN Int, OUT UInt8*, IN Int);
Int  Dtu2xxTxIoCtlRegWriteMasked(IN PDTU2XX_FDO, IN Int, IN Int, IN Int, IN Int, IN Int);
Int  Dtu2xxTxIoCtlSetLoopBackMode(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxTxIoCtlSetFifoExtrap(IN PDTU2XX_FDO, IN Int, IN Int, IN Int);
Int  Dtu2xxTxIoCtlBulkWrite(IN PDTU2XX_FDO, IN Int, IN Int, IN UInt8*);
Int  Dtu2xxTxIoCtlGetMaxFifoSize(IN PDTU2XX_FDO, IN Int, OUT Int*);
Int  Dtu2xxTxIoCtlGetFifoSize(IN PDTU2XX_FDO, IN Int, OUT Int*);
Int  Dtu2xxTxIoCtlGetModControl(IN PDTU2XX_FDO, IN Int, OUT Int*, OUT Int*,
								OUT Int*, OUT Int*);
Int  Dtu2xxTxIoCtlSetModControl(IN PDTU2XX_FDO, IN Int, IN Int, IN Int,
								IN Int, IN Int);
Int  Dtu2xxTxIoCtlSetRfMode(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxTxIoCtlGetSymSampleRate(IN PDTU2XX_FDO, IN Int, OUT Int*);
Int  Dtu2xxTxIoCtlSetSymSampleRate(IN PDTU2XX_FDO, IN Int, IN Int);
Int  Dtu2xxTxIoCtlGetRfControl(IN PDTU2XX_FDO, IN Int, OUT Int64*);
Int  Dtu2xxTxIoCtlSetRfControl(IN PDTU2XX_FDO, IN Int, IN Int64);
Int  Dtu2xxTxIoCtlSetOutputLevel(IN PDTU2XX_FDO, IN Int, IN Int);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Usb.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
Int  Dtu2xxVendorRequest(DTU2XX_FDO*, UInt8, UInt16, UInt16, Int, Int, UInt8*, UInt*);
Int  Dtu2xx8051Reset(IN PDTU2XX_FDO, IN UInt8);
Int  Dtu2xxSimpleBulkTransfer(DTU2XX_FDO*, Int, Int, Int, UInt8*);
Int  Dtu2xxReadEeProm(IN PDTU2XX_FDO, IN UInt, IN UInt, OUT UInt8*);
Int  Dtu2xxWriteEeProm(IN PDTU2XX_FDO, IN UInt, IN UInt, IN UInt8*);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Utility.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
UInt64  Dtu2xxGetTickCount(void);
Int  Dtu2xxTxMode2PacketSize(Int);
Int  Dtu2xxBuf2RingBuffer(IN UInt8*, IN Int, IN PDTU2XX_RING_BUFFER);
Int  Dtu2xxRingBuffer2Buf(IN PDTU2XX_RING_BUFFER, IN Int, OUT UInt8*);
Int64   Dtu2xxBinDiv(Int64 num, Int64 denom, Int64* pRest);
UInt64  Dtu2xxBinUDiv(UInt64 num, UInt64 denom, UInt64* pRest);
Int  Dtu2xxWaitMs(IN Int TimeInMs);
Int  Dtu2xxComputeRollOff(IN Int, IN Int);
BOOLEAN  Dtu2xxIsHardQamAorC(IN Int, IN Int);
BOOLEAN  Dtu2xxIsHardQamA(IN Int, IN Int);
BOOLEAN  Dtu2xxIsHardQamC(IN Int, IN Int);
BOOLEAN  Dtu2xxIsIqMode(Int ModType);
BOOLEAN  Dtu2xxIsQamB(IN Int, IN Int);
BOOLEAN  Dtu2xxIsQamMode(Int ModType);
BOOLEAN  Dtu2xxIsSoftQam(IN Int, IN Int);
Int  Dtu2xxNumBitsPerSymbol(IN Int);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Vpd.c -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
Int  Dtu2xxVpdIoctl(IN PDTU2XX_FDO, UInt, UInt, DTU2XX_VPD_DATA*);
void Dtu2xxVpdInitOffset(IN PDTU2XX_FDO);
Int  Dtu2xxVpdRead(IN PDTU2XX_FDO, OUT UInt8*, IN UInt, IN UInt);
Int  Dtu2xxVpdReadFromCache(IN PDTU2XX_FDO, OUT UInt8*, IN UInt, IN UInt);
Int  Dtu2xxVpdReadItemRoRw(IN PDTU2XX_FDO, IN char*, OUT char*);
Int  Dtu2xxVpdWrite(IN PDTU2XX_FDO, IN UInt8*, IN  UInt, IN  UInt);

#endif // #ifndef __DTU2XX_H
