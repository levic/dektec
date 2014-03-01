//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dta1xxCommon.h *#*#*#*#*#*#*#* (C) 2000-2007 DekTec
//
// Header file with common data structures between the Dta1xx driver and application
// programs, mainly DTAPI.
//
// ASSUMPTIONS:
//	- In the driver:
//			* "DriverTypes.h" has been included
//			* DTA1XX_DRIVER is defined
//	- In a user-mode application:
//			* "StandardTypes.h" has been included
//
// INDEXING:
// As of 2006.03.30, the following indices are used to identify channels:
//	- Port Index		Zero-based index identifying a port. Each physical port has a
//						unique port index
//	- Channel Index		Zero-based index for identifying IP Rx or Tx channels

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  RD  2008.04.01  Add IOCTL_DTA1XX_RELEASE_ADDR_OPER_REGS
//	RD	2007.09.18	Renamed all *NDIS* to generic *NW*
//	SD	2007.03.01	Add IOCTL_DTA1XX_GET_RATE2/SET_RATE2, IOCTL_DTA1XX_SET_MOD_CONTROL
//	RD	2006.09.13	Add VPD read/write functions
//	RD	2006.07.14	Add UserIpTxCommon struct for shared memory between driver and DTAPI
//	RD	2006.06.07	Add DTA1XX_FUNCTION_SET_NWSTATCNT, DTA1XX_FUNCTION_GET_NWSTATCNT
//	RD	2006.05.05	Add DTA1XX_FUNCTION_NDIS_RESET
//	SD	2006.03.30	Major low-level change: Use of port index instead of channel index
//	RD	2006.03.21	Add DTA1XX_FUNCTION_NDIS_SET_VERSION, DTA1XX_FUNCTION_GET_NDIS_VERSION
//	RD	2006.03.10	Add DTA1XX_FUNCTION_GET_TS_IOCONFIG, DTA1XX_FUNCTION_SET_TS_IOCONFIG
//	SD	2005.08.23	Add DTA1XX_FUNCTION_IPRX_CTRL and DTA1XX_FUNCTION_IPTX_CTRL
//	RD	2005.08.19	Add DTA1XX_FUNCTION_NDIS_GET_STATUS
//	SD	2005.08.11	Add DTA1XX_FUNCTION_IPRX_CTRL and DTA1XX_FUNCTION_IPTX_CTRL
//	RD	2005.08.07	Add DTA1XX_FUCNTION_NDIS_QUERY_INFO and DTA1XX_FUNCTION_NDIS_SET_INFO
//	SD	2004.11.28	Add DTA1XX_RW_INFO to pass ChannelIndex for DMA reads / writes
//	SD	2004.08.09	Add GET and SET MOD-CONTROL ioctl
//	SD	2004.07.06	Add FIRMWARE_REBOOT ioctl
//	SD	2002.03.29	Add GET_RX_INFO ioctl
//	SD	2002.03.05	Add CLR_OUTPUT_FIFO ioctl
//	SD	2001.02.09	Re-arranged function codes
//	SD	2000.10.29	Created

//#pragma once
#ifndef __DTA1XX_COMMON_H
#define __DTA1XX_COMMON_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#if defined(WIN32) || defined(WIN64)
	// Windows specific includes
	#if defined(DTA1XX_DRIVER)
		#include <ntddk.h>			// Cannot be used in DTAPI
	#elif defined (NDIS_WDM)
		#include <ndis.h>			// Used for the Ndis driver
	#else
		#include <winioctl.h>		// Cannot be used in driver
	#endif
#else
	// Linux specific includes
	// To prevent compile errors
	#ifndef MODULE 
		typedef UInt32	KEVENT, *PRKEVENT;	
	#endif
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+ Ioctl Input / Output Data Structures +=+=+=+=+=+=+=+=+=+=+=+=+


// NOTE-1 Starting at Dta1xx driver version 2.0.0, channels are identified by port index
//        instead of channel index. The port index is unique for each physical port.
//        For IP channels, a channel index is still used as sub-index of an IP port.
//        With this change, channel type has become redundant.
//
// NOTE-2 IoCtl structures are not changed, and channel type is still passed to the
//        driver, so that OLD applications will still operate with the NEW driver
//
// NOTE-3 ChannelType constants in the driver (DTA1XX_TS_TX_CHANNEL/...) are different
//        from ChannelType constants used in the DTAPI (DTAPI_CHAN_OUTPUT/...)

// Constants
#define DTA1XX_LOC_BUF_SIZE			128		// Size of location-info buffer in #WCHARs
#define DTA1XX_I2C_BUF_SIZE			512		// Size of I2C buffer
#define	DTA1XX_VPD_BUF_SIZE			512		// Size of VPD buffer
#define DTA1XX_VPD_ITEM_MAX_LENGTH	63		// 64 if trailing '\0' is factored in

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Address Info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Used by IOCTL_DTA1XX_GET_ADDR_OPER_REGS and IOCTL_DTA1XX_RELEASE_ADDR_OPER_REGS
// to communicate address pointers.
//

#if defined(WIN32) || defined(WIN64)
typedef struct _DTA1XX_ADDR_INFO {
	void*  m_pUserVirtual;			// User-space address pointer
	Int  m_ChannelType;				// Channel type: DTA1XX_TS_TX_CHANNEL/...
									// Special value: DTA1XX_GEN_REGS
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
} DTA1XX_ADDR_INFO;
#else
typedef struct _DTA1XX_ADDR_INFO {
	void*  m_pUserVirtual;		// User-space address pointer
	void*  m_pPaMmap;				// User-space address pointer returned by mmap
	Int  m_ChannelType;				// Channel type: DTA1XX_TS_TX_CHANNEL/...
									// Special value: DTA1XX_GEN_REGS
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	UInt  m_Size;					// Size of memory block returned. (0 = not filled)
} DTA1XX_ADDR_INFO;
#endif

#ifdef WIN32
// For 32-bit application running on 64-bit OS.
typedef struct _DTA1XX_ADDR_INFO_32 {
	void*POINTER_32  m_pUserVirtual;// User-space 32-bit address pointer
	Int  m_ChannelType;				// Channel type: DTA1XX_TS_TX_CHANNEL/...
									// Special value: DTA1XX_GEN_REGS
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
} DTA1XX_ADDR_INFO_32;
#endif

#if defined(CONFIG_X86_64) && !defined(WIN32) && !defined(WIN64)
// Address data for 32-bit applications running on a 64-bit Linux kernel
typedef struct _DTA1XX_ADDR_INFO_32 {
	UInt32 m_pUserVirtual;			// 32-bit user-space address pointer
	UInt32 m_pPaMmap;				// 32-bit user-space address pointer returned by mmap
	Int  m_ChannelType;				// Channel type: DTA1XX_TS_TX_CHANNEL/...
									// Special value: DTA1XX_GEN_REGS
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	UInt  m_Size;					// Size of memory block returned. (0 = not filled)
} DTA1XX_ADDR_INFO_32;
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Continuous DMA (CDMA) -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_CDMA_CONTROL {
	Int  m_ChannelType;				// Channel type: DTA1XX_TS_TX_CHANNEL/...
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	Int  m_Control;					// Control code
	Int  m_Segment;					// CDMA buffer segment
	UInt32  m_RefClkSamp;			// Reference-clock sample at last segment boundary
	Int  m_BufPos;					// Estimate of current CDMA buffer position

} DTA1XX_CDMA_CONTROL;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Clock Error -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_CLOCK_ERR {
	Int  m_ClockErrPpb;				// Clock error in ppb
} DTA1XX_CLOCK_ERR;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Device-Driver Versioning Info. -.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_DRIVER_VERSION {
	Int  m_VersionMajor;
	Int  m_VersionMinor;
	Int  m_VersionMicro;
	Int  m_VersionBuild;
} DTA1XX_DRIVER_VERSION;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EEPROM Control -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_EEPROM_CONTROL {
	Int  m_Code;					// DTA1XX_EEPROM_PROTECT or
									// DTA1XX_EEPROM_PROTECT_CLR
} DTA1XX_EEPROM_CONTROL;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Exclusive-Access Info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_EXCL_ACCESS_INFO {
	Int  m_Request;					// 0 = Request exclusive access
									// 1 = Release exclusive access
	Int  m_Granted;					// 0 = In use
									// 1 = Exclusive access granted
	Int  m_ChannelType;				// Channel type: DTA1XX_TS_TX_CHANNEL/...
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
} DTA1XX_EXCL_ACCESS_INFO;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FailSafe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DTA1XX_FAILSAFE {
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	UInt  m_Mode;					// 1 = Enable, 2 = Timeout, 3 = Enable+Timeout, 4 = Alive
									// On a Get, Alive = on if application has responded on time
	UInt  m_Enable;					// TRUE: Failsafe enabled, check timeout value
	UInt32  m_Timeout;				// Timeout value. (resolution 20 ms)
	UInt32  m_Reserved;				// Reserved
} DTA1XX_FAILSAFE;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Genlock state info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DTA1XX_GENLOCK {
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	Int  m_ChannelType;				// Channel type: DTA1XX_TS_TX_CHANNEL/...
	Int  m_Mode;					// -1 = 27Mhz VCXO, 0 = Reserved, 1=SDI Frame
	UInt m_Enable;					// TRUE: Genlock enabled, check mode and offset value
	Int  m_OffsetSamples;			// Current Offset in Samples
} DTA1XX_GENLOCK;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Firmware Reboot Control -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_FIRMWARE_REBOOT_CTRL {
	Int  m_Reserved1;
	Int  m_Reserved2;
} DTA1XX_FIRMWARE_REBOOT_CTRL;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Flags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_FLAGS {
	Int  m_Status;					// Flags
	Int  m_Latched;					// Latched flags (to be cleared)
	// NEW:
	Int  m_ChannelType;				// Channel type: DTA1XX_TS_TX_CHANNEL/...
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
} DTA1XX_FLAGS;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Location Info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_LOC_INFO {
	Int  m_DeviceId;				// Device ID
	Int  m_VendorId;				// Vendor ID
	Int  m_SubSystemId;				// Subsystem ID
	Int  m_SubVendorId;				// Subsystem Vendor ID
	Int  m_BusNumber;				// PCI-bus number
	Int  m_SlotNumber;				// PCI-slot number
#if defined(WIN32) || defined(WIN64)
	wchar_t m_LocBuf[DTA1XX_LOC_BUF_SIZE];
									// Location information in string format.
#endif
} DTA1XX_LOC_INFO;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- I2C Transfer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_I2C_TRANSFER {
	UInt  m_Command;				// I2C transfer command (e.g. DTA1XX_I2CCMD_READ, etc)
	UInt  m_DeviceAddr;				// I2C device address (7+1 bit, LSB is ignored)
	UInt  m_Length;					// Number of BYTES
	UInt8 m_Buf[DTA1XX_I2C_BUF_SIZE]; // Buffer with the I2C data to be read/written 
} DTA1XX_I2C_TRANSFER;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- I/O Configuration -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_IOCONFIG {
	Int  m_ParXtra;					// Extra IO config option
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	Int  m_IoConfig;				// DTA1XX_IOCONFIG_XXX constants
} DTA1XX_IOCONFIG;
typedef struct _DTA1XX_IOCONFIG2 {
	Int  m_ParXtra;					// Extra IO config option
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	Int  m_IoConfig;				// DTA1XX_IOXXX_XXX constants
	Int  m_ConfigCode;				// DTA1XX_IOXXX configuration code
} DTA1XX_IOCONFIG2;


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Modulation Control -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// NOTE: only used with Transport-Stream transmit channel => no ChannelType
//
typedef struct _DTA1XX_MOD_CONTROL {
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	Int  m_ModType;					// Modulation type
	Int  m_ParXtra0;				// Extra parameter #0
	Int  m_ParXtra1;				// Extra parameter #1
	Int  m_ParXtra2;				// Extra parameter #2
} DTA1XX_MOD_CONTROL;

typedef struct _DTA1XX_OUTPUT_LEVEL {
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	Int  m_OutputLevel;				// Output level
} DTA1XX_OUTPUT_LEVEL;

typedef struct _DTA1XX_RF_CONTROL {
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	Int64  m_RfRate;				// RF rate
	Int  m_LockStatus;				// RF PLL lock status
} DTA1XX_RF_CONTROL;

typedef struct _DTA1XX_RF_MODE {
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	Int  m_RfMode;					// RF mode
} DTA1XX_RF_MODE;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- NWI Info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_NWI_INFO {
	Int  m_IpPortIndex;				// Identifies IP physical port
	UInt32  m_Oid;					// NDIS_OID
	UInt  m_BytesWritten;			// Bytes written into m_Data
	UInt8  m_Data[sizeof(UInt32)];	// Variable-length data buffer
	// InformationBufferLength = BytesNeeded
} DTA1XX_NWI_INFO, *PDTA1XX_NWI_INFO;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- NWI Reset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_NWI_RESET {
	Int  m_IpPortIndex;				// Identifies IP physical port
} DTA1XX_NWI_RESET, *PDTA1XX_NWI_RESET;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- NWI Status -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_NWI_STATUS {
	Int  m_IpPortIndex;				// Identifies IP physical port
	volatile Int  m_GeneralStatus;	// NDIS_STATUS information
	Int  m_StatusBufferLength;		// Length of StatusBuffer in bytes
	union{
		UInt8  m_StatusBuffer[sizeof(UInt32)*4];
#if defined(WIN32) || defined(WIN64)
#else
		struct {
			volatile PRKEVENT  m_StatusAvail;
			volatile UInt  m_Avail;
		}m_LinkStatus;
#endif
	};
} DTA1XX_NWI_STATUS, *PDTA1XX_NWI_STATUS;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- NWI Speed / Duplex -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_NWI_SPEED {
	Int  m_IpPortIndex;				// Identifies IP physical port
	UInt  m_Speed;					// 0 = Auto Detect, 1 = 10MB-half,  2 = 10MB-full,
									// 3 = 100MB-half,  4 = 100MB-full, 5 = 1GB
} DTA1XX_NWI_SPEED, *PDTA1XX_NWI_SPEED;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Mac Address -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_MACADDRESS {
	UInt8  m_IpPortIndex;			// Identifies IP physical port
	UInt8  m_MacAddress[6];
} DTA1XX_MACADDRESS;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Network loopback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_NWLOOPBACK {
	UInt8  m_IpPortIndex;			// Identifies IP physical port
	UInt8  m_LoopbackEnable;		// TRUE: Phy is set to loopback, and prom. enable
} DTA1XX_NWLOOPBACK;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Network speed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_NWSPEED {
	UInt8  m_IpPortIndex;			// Identifies IP physical port
	UInt8  m_ForceSpeedEnable;		// TRUE: Speed/Duplex is forced to m_Speed setting
	UInt  m_Speed;					// 0 = Auto Detect, 1 = 10MB-half,  2 = 10MB-full,
									// 3 = 100MB-half,  4 = 100MB-full, 5 = 1GB
} DTA1XX_NWSPEED;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Network status/counters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_NWSTATCNT {
	UInt8  m_IpPortIndex;		// Identifies IP physical port
	UInt16  m_Offset;			// Identifies the status/counter to get/set
	UInt32  m_Value;			// Value of the status/counter
	UInt32  m_ValueH;			// Value of the status/counter (high part)
} DTA1XX_NWSTATCNT;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Generic integer port parameter -.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_PORT_INTPAR {
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	Int  m_Parameter;				// Parameter value
} DTA1XX_PORT_INTPAR;

//.-.-.-.-.-.-.-.-.-.-.-.-.- Generic 2 integer port parameters .-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_PORT_2INTPAR {
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	Int  m_Parameter[2];			// Parameter value
} DTA1XX_PORT_2INTPAR;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Rate Info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// NOTE: only used with Transport-Stream transmit channel => no ChannelType
//
typedef struct _DTA1XX_RATE_INFO {
	Int  m_ClockGenMode;			// Internal / external mode
	Int  m_TsRate;					// Transport-stream rate
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
} DTA1XX_RATE_INFO;
//
// New Style
//
typedef struct _DTA1XX_RATE_INFO2 {
	Int  m_ClockGenMode;			// Internal / external mode
	Int  m_TsSymOrSampRate;			// TS-, symbol- or sample rate
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
} DTA1XX_RATE_INFO2;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Receive Status Info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// NOTE: only used with Transport-Stream receive channel => no ChannelType
//
typedef struct _DTA1XX_RX_INFO {
	Int  m_TsRate;					// Transport-Stream rate in bps, not yet
									// compensated for 188/204 bytes
	Int  m_ViolCount;				// Code-violation counter
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
} DTA1XX_RX_INFO;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Reset Info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_RST_INFO {
	Int  m_ChannelType;				// Channel type: DTA1XX_TS_TX_CHANNEL/...
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	Int  m_RstMode;					// Reset mode: DTA1XX_RST_CHANNEL/...
} DTA1XX_RST_INFO;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Read/Write Info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_RW_INFO {
	Int  m_ChannelType;				// Channel type: may be useful later
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
#if !defined(WIN32) && !defined(WIN64)
	// Linux specific members
	UInt8*  m_pBuffer;				// Pointer to user buffer
	Int  m_NumBytes2Transfer;		// Number of bytes to transfer
#endif
} DTA1XX_RW_INFO;


#if defined(CONFIG_X86_64) && !defined(WIN32) && !defined(WIN64)
//-.-.-.-.-.-.-.- Read/Write Info (for 32-bit application on 64-bit Linux) -.-.-.-.-.-.-.-
typedef struct _DTA1XX_RW_INFO_32 {
	Int  m_ChannelType;				// Channel type: may be useful later
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	UInt32  m_pBuffer;				// Pointer to user buffer
	Int  m_NumBytes2Transfer;		// Number of bytes to transfer
} DTA1XX_RW_INFO_32;
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.- Shared Buffer Allocation / Freeing -.-.-.-.-.-.-.-.-.-.-.-.-.
// Control
typedef struct _DTA1XX_SHAREDBUF_CTRL {
	Int  m_Purpose;					// Purpose: DTA1XX_SHBUF_CDMA, ...
	Int  m_ChannelType;				// Channel type: DTA1XX_TS_TX_CHANNEL/...
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	Int  m_ChannelIndex;			// Channel index for IP channels

#if !defined(WIN32) && !defined(WIN64)
	// User buffer decsription -> for allocation
    UInt8* m_pSharedBuf;			// Pointer to the shared buffer
    Int m_SharedBufSize;			// Shared buffer size
#endif

} DTA1XX_SHAREDBUF_CTRL;
// Return Values
typedef struct _DTA1XX_SHAREDBUF_RTRN {
	Int  m_XXX;
} DTA1XX_SHAREDBUF_RTRN;

#if defined(CONFIG_X86_64) && !defined(WIN32) && !defined(WIN64)
//-.-.- Shared Buffer Allocation / Freeing (for 32-bit applications on 64-bit Linux) -.-.-
typedef struct _DTA1XX_SHAREDBUF_CTRL_32 {
	Int  m_Purpose;					// Purpose: DTA1XX_SHBUF_CDMA, ...
	Int  m_ChannelType;				// Channel type: DTA1XX_TS_TX_CHANNEL/...
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	Int  m_ChannelIndex;			// Channel index for IP channels

	// User buffer decsription -> for allocation
    UInt32 m_pSharedBuf;			// Pointer to the shared buffer
    Int m_SharedBufSize;			// Shared buffer size
} DTA1XX_SHAREDBUF_CTRL_32;
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Target Info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_TARGET_INFO {
	Int  m_Present;					// 0=Nothing connected; 1=DVB-SPI sink;
									// 2=Target adapter connected
	Int  m_TargetId;				// Target-adapter identifier
	Int  m_ChannelType;				// Channel type: DTA1XX_TS_TX_CHANNEL/...
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
} DTA1XX_TARGET_INFO;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Transmit Control -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// NOTE: only used with Transport-Stream transmit channel => no ChannelType
//
typedef struct _DTA1XX_TX_CONTROL {
	Int  m_TxControl;				// Transmit-control state
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
} DTA1XX_TX_CONTROL;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Transmit Mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// NOTE: only used with Transport-Stream transmit channel => no ChannelType
//
typedef struct _DTA1XX_TX_MODE {
	Int  m_TxPacketMode;			// Packet transmit mode
	Int  m_TxStuffMode;				// Null-packet stuffing on/off
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
} DTA1XX_TX_MODE;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Receive Mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// NOTE: only used with Transport-Stream receive channel => no ChannelType
//
typedef struct _DTA1XX_RX_MODE {
	Int  m_RxMode;					// Packet receive mode
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
} DTA1XX_RX_MODE;

//-.-.-.-.-.-.-.-.-.-.-.- Vital Product Data for VPD read or write -.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_VPD_TRANSFER {
	UInt  m_Start;					// Start BYTE address
	UInt  m_Length;					// Number of BYTES
	UInt8  m_VpdBuf[DTA1XX_VPD_BUF_SIZE];
									// The Vital product data itself
} DTA1XX_VPD_TRANSFER;

typedef struct _DTA1XX_VPD_ITEM {
	UInt  m_VpdSectionType;			// VPDSECT_ID, VPDSECT_RO, VPDSECT_RW
	char  m_VpdKeyword[2];			// Not used for VPD_ID
	UInt  m_Length;					// Length of buffer / Number of bytes in buffer
	UInt  m_Flag;					// Extra flags:
	UInt8  m_VpdBuf[DTA1XX_VPD_ITEM_MAX_LENGTH+1];
									// The Vital product data itself
} DTA1XX_VPD_ITEM;

typedef struct _DTA1XX_VPD_EEPROM {
	UInt  m_Start;					// Start BYTE address. 0 = start of VPDID
	UInt  m_Length;					// Number of BYTES
	UInt8  m_VpdBuf[DTA1XX_VPD_BUF_SIZE];
									// The Vital product data itself
} DTA1XX_VPD_EEPROM;

typedef struct _DTA1XX_VPD_DATA {
	UInt  m_Command;				// VPD_CMD_...
	union {
		DTA1XX_VPD_ITEM  m_VpdItem;
		DTA1XX_VPD_EEPROM  m_VpdEeprom;
	} m_Data;
} DTA1XX_VPD_DATA;

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IP Rx IOCTL +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// This IOCTL is used to control TS-Over-IP reception.
// A sub-structure (similar to the structure for the main Dta1xx IOCTLs) is used
// to encapsulate control data and return values.

// Sub-function Codes for IP Rx IOCTLs
enum {
	IOCTL_DTA1XX_IPRXCTRL_ATTACH,
	IOCTL_DTA1XX_IPRXCTRL_CLEARFLAGS,
	IOCTL_DTA1XX_IPRXCTRL_DETACH,
	IOCTL_DTA1XX_IPRXCTRL_GETFIFOLOAD,
	IOCTL_DTA1XX_IPRXCTRL_GETFLAGS,
	IOCTL_DTA1XX_IPRXCTRL_GETIPPARS,
	IOCTL_DTA1XX_IPRXCTRL_GETIPSTAT,
	IOCTL_DTA1XX_IPRXCTRL_GETSTATUS,
	IOCTL_DTA1XX_IPRXCTRL_GETTSRATE,
	IOCTL_DTA1XX_IPRXCTRL_READ,
	IOCTL_DTA1XX_IPRXCTRL_RESET,
	IOCTL_DTA1XX_IPRXCTRL_SETIPPARS,
	IOCTL_DTA1XX_IPRXCTRL_SETRXCONTROL,
	IOCTL_DTA1XX_IPRXCTRL_SETRXMODE,
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Attach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_IPRX_ATTACH_CTRL {
	Int  m_Dummy;
} DTA1XX_IPRX_ATTACH_CTRL;

typedef struct _DTA1XX_IPRX_ATTACH_RTRN {
	Int  m_ChannelIndex;			// Channel index for newly openened channel
} DTA1XX_IPRX_ATTACH_RTRN;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Detach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_IPRX_DETACH_CTRL {
	Int  m_Dummy;
} DTA1XX_IPRX_DETACH_CTRL;

typedef struct _DTA1XX_IPRX_DETACH_RTRN {
	Int  m_Dummy;
} DTA1XX_IPRX_DETACH_RTRN;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Flags for ClearFlags, GetFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_IPRX_FLAGS_CTRL {
	Int  m_Flags;					// (Latched) flags to be cleared
} DTA1XX_IPRX_FLAGS_CTRL;

typedef struct _DTA1XX_IPRX_FLAGS_RTRN {
	Int  m_Flags;					// Flags
	Int  m_LatchedFlags;			// Latched status flags
} DTA1XX_IPRX_FLAGS_RTRN;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- GetFifoLoad -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_IPRX_GETFIFOLOAD_CTRL {
	Int  m_Dummy;
} DTA1XX_IPRX_GETFIFOLOAD_CTRL;

typedef struct _DTA1XX_IPRX_GETFIFOLOAD_RTRN {
	Int  m_FifoLoad;				// FIFO load returned
} DTA1XX_IPRX_GETFIFOLOAD_RTRN;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- GetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_IPRX_GETSTATUS_CTRL {
	Int  m_Dummy;
} DTA1XX_IPRX_GETSTATUS_CTRL;

typedef struct _DTA1XX_IPRX_GETSTATUS_RTRN {
	Int  m_PacketSize;				// Size of incoming transport packets:
	Int  m_ClkDet;					// For IP Rx: IP traffic detected
} DTA1XX_IPRX_GETSTATUS_RTRN;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- GetTsRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_IPRX_GETTSRATE_CTRL {
	Int  m_Dummy;
} DTA1XX_IPRX_GETTSRATE_CTRL;

typedef struct _DTA1XX_IPRX_GETTSRATE_RTRN {
	Int  m_TsRate;					// Transport-stream rate in bps
} DTA1XX_IPRX_GETTSRATE_RTRN;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IpPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// NOTE: fields in DTAPI struct DtTsIpPars that are not used for Rx (this is: Tx-only
// fields) are not stored in this structure.
//
typedef struct _DTA1XX_IPRX_IPPARS_CTRL {
	UInt8  m_DstIp[4];				// Destination: IP address
	UInt16  m_DstPort;				// Destination: port number
	UInt8  m_SrcIp[4];				// Source: IP address
	UInt16  m_SrcPort;				// Source: port number
	Int  m_Protocol;				// Protocol: UDP/RTP
	Int  m_FecMode;					// Error correction mode
} DTA1XX_IPRX_IPPARS_CTRL;

// Structure used for retrieving the IpPars
typedef struct _DTA1XX_IPRX_IPPARS_RTRN {
	Int  m_Protocol;				// Protocol: UDP/RTP/Unknown
	Int  m_FecNumRows;				// ‘D’ = #rows in FEC matrix
	Int  m_FecNumCols;				// ‘L’ = #columns in FEC matrix
	Int  m_NumTpPerIp;				// 0: Not detected
} DTA1XX_IPRX_IPPARS_RTRN;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IpStat -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_IPRX_IPSTAT_CTRL {
	Int  m_Dummy;
} DTA1XX_IPRX_IPSTAT_CTRL;

typedef struct _DTA1XX_IPRX_IPSTAT_RTRN {
	UInt  m_TotNumIPPackets;
	UInt  m_LostIPPacketsBeforeFec;		// (BER before FEC)
	UInt  m_LostIPPacketsAfterFec;		// (BER after FEC)
	UInt  m_LostIPPacketsBeforeSort;	// #Lost Packets before RTP sorting
	UInt  m_LostIPPacketsAfterSort;		// #Lost Packets after RTP sorting
} DTA1XX_IPRX_IPSTAT_RTRN;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Read -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_IPRX_READ_CTRL {
	Int  m_ReadCmd;					// Read command: 1=Get read index; 2=Update read ptr
	Int  m_NumBytesToRead;			// Number of bytes to read
} DTA1XX_IPRX_READ_CTRL;

typedef struct _DTA1XX_IPRX_READ_RTRN {
	Int  m_ReadIdx;					// Index in buffer
} DTA1XX_IPRX_READ_RTRN;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Reset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_IPRX_RESET_CTRL {
	Int  m_ResetMode;				// Reset Mode
} DTA1XX_IPRX_RESET_CTRL;

typedef struct _DTA1XX_IPRX_RESET_RTRN {
	Int  m_Dummy;
} DTA1XX_IPRX_RESET_RTRN;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SetRxControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_IPRX_SETRXCONTROL_CTRL {
	Int  m_RxControl;				// Receive-Control state
} DTA1XX_IPRX_SETRXCONTROL_CTRL;

typedef struct _DTA1XX_IPRX_SETRXCONTROL_RTRN {
	Int  m_Dummy;
} DTA1XX_IPRX_SETRXCONTROL_RTRN;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SetRxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_IPRX_SETRXMODE_CTRL {
	Int  m_RxMode;					// Receive Mode
} DTA1XX_IPRX_SETRXMODE_CTRL;

typedef struct _DTA1XX_IPRX_SETRXMODE_RTRN {
	Int  m_Dummy;
} DTA1XX_IPRX_SETRXMODE_RTRN;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IP Tx IOCTL +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// This IOCTL is used to control TS-Over-IP transmission.
// A sub-structure (similar to the structure for the main Dta1xx IOCTLs) is used
// to encapsulate control data and return values.

// Sub-function Codes for IP Tx IOCTLs
enum {
	IOCTL_DTA1XX_IPTXCTRL_ATTACH,
	IOCTL_DTA1XX_IPTXCTRL_CLEARFLAGS,
	IOCTL_DTA1XX_IPTXCTRL_DETACH,
	IOCTL_DTA1XX_IPTXCTRL_GETFIFOLOAD,
	IOCTL_DTA1XX_IPTXCTRL_GETMACADDRESS,
	IOCTL_DTA1XX_IPTXCTRL_GETFLAGS,
	IOCTL_DTA1XX_IPTXCTRL_RESET,
	IOCTL_DTA1XX_IPTXCTRL_SETIPPARS,
	IOCTL_DTA1XX_IPTXCTRL_SETTXCONTROL,
	IOCTL_DTA1XX_IPTXCTRL_SETTXMODE,
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Attach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_IPTX_ATTACH_CTRL {
	Int  m_Dummy;
} DTA1XX_IPTX_ATTACH_CTRL;

typedef struct _DTA1XX_IPTX_ATTACH_RTRN {
	Int  m_ChannelIndex;			// Channel index for newly openened channel
} DTA1XX_IPTX_ATTACH_RTRN;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Flags for ClearFlags, GetFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_IPTX_FLAGS_CTRL {
	Int  m_Flags;					// (Latched) flags to be cleared
} DTA1XX_IPTX_FLAGS_CTRL;

typedef struct _DTA1XX_IPTX_FLAGS_RTRN {
	Int  m_Flags;					// Flags
	Int  m_LatchedFlags;			// Latched status flags
} DTA1XX_IPTX_FLAGS_RTRN;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Detach -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_IPTX_DETACH_CTRL {
	Int  m_Dummy;
} DTA1XX_IPTX_DETACH_CTRL;

typedef struct _DTA1XX_IPTX_DETACH_RTRN {
	Int  m_Dummy;
} DTA1XX_IPTX_DETACH_RTRN;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- GetFifoLoad -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTA1XX_IPTX_GETFIFOLOAD_CTRL {
	Int  m_Dummy;
} DTA1XX_IPTX_GETFIFOLOAD_CTRL;

typedef struct _DTA1XX_IPTX_GETFIFOLOAD_RTRN {
	Int  m_FifoLoad;				// FIFO load returned
} DTA1XX_IPTX_GETFIFOLOAD_RTRN;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IpPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// NOTE: fields in DTAPI struct DtTsIpPars that are not used for Tx (this is: Rx-only
// fields) are not stored in this structure.
//
typedef struct _DTA1XX_IPTX_IPPARS_CTRL {
	UInt8  m_DstIp[4];				// Destination: IP address
	UInt16  m_DstPort;				// Destination: port number
	UInt8  m_SrcIp[4];				// Source: Own IP address
	UInt16  m_SrcPort;				// Source: port number
	Int  m_Protocol;				// Protocol: UDP/RTP
	Int  m_FecMode;					// Error correction mode
	Int  m_FecNumRows;				// ‘D’ = #rows in FEC matrix
	Int  m_FecNumCols;				// ‘L’ = #columns in FEC matrix
} DTA1XX_IPTX_IPPARS_CTRL;

typedef struct _DTA1XX_IPTX_IPPARS_RTRN {
	Int  m_Dummy;
} DTA1XX_IPTX_IPPARS_RTRN;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Reset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_IPTX_RESET_CTRL {
	Int  m_ResetMode;				// Reset Mode
} DTA1XX_IPTX_RESET_CTRL;

typedef struct _DTA1XX_IPTX_RESET_RTRN {
	Int  m_Dummy;
} DTA1XX_IPTX_RESET_RTRN;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SetTxControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_IPTX_SETTXCONTROL_CTRL {
	Int  m_TxControl;				// Transmit-Control state
} DTA1XX_IPTX_SETTXCONTROL_CTRL;

typedef struct _DTA1XX_IPTX_SETTXCONTROL_RTRN {
	Int  m_Dummy;
} DTA1XX_IPTX_SETTXCONTROL_RTRN;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SetTxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTA1XX_IPTX_SETTXMODE_CTRL {
	Int  m_TxMode;					// Transmit Mode
} DTA1XX_IPTX_SETTXMODE_CTRL;

typedef struct _DTA1XX_IPTX_SETTXMODE_RTRN {
	Int  m_Dummy;
} DTA1XX_IPTX_SETTXMODE_RTRN;


//=+=+=+=+=+=+=+=+=+=+=+ IP Rx/Tx Control - Control and Return Data +=+=+=+=+=+=+=+=+=+=+=

// Control data
typedef struct _DTA1XX_IP_RXTX_CTRL {
	Int  m_IpRxTxIoctl;				// IP Rx/Tx IO-Control (sub-)command
	Int  m_ChannelType;				// Channel type: DTA1XX_IP_TX_CHANNEL/...
	Int  m_IpPortIndex;				// Identifies IP physical port
	Int  m_ChannelIndex;			// Identifies IP Rx/Tx channel

	// Big union with specific control data for IP Rx/Tx IOCTLs
	union {
		DTA1XX_IPRX_ATTACH_CTRL  m_AttachCtrl;
		DTA1XX_IPRX_DETACH_CTRL  m_DetachCtrl;
		DTA1XX_IPRX_FLAGS_CTRL  m_FlagsCtrl;
		DTA1XX_IPRX_GETFIFOLOAD_CTRL  m_GetFifoLoadCtrl;
		DTA1XX_IPRX_GETSTATUS_CTRL  m_GetStatusCtrl;
		DTA1XX_IPRX_GETTSRATE_CTRL  m_GetTsRateCtrl;
		DTA1XX_IPRX_IPPARS_CTRL  m_IpParsCtrl;
		DTA1XX_IPRX_IPSTAT_CTRL  m_IpStatCtrl;
		DTA1XX_IPRX_READ_CTRL  m_ReadCtrl;
		DTA1XX_IPRX_RESET_CTRL  m_ResetCtrl;
		DTA1XX_IPRX_SETRXCONTROL_CTRL  m_SetRxControlCtrl;
		DTA1XX_IPRX_SETRXMODE_CTRL  m_SetRxModeCtrl;

		DTA1XX_IPTX_ATTACH_CTRL  m_TxAttachCtrl;
		DTA1XX_IPTX_DETACH_CTRL  m_TxDetachCtrl;
		DTA1XX_IPTX_FLAGS_CTRL  m_TxFlagsCtrl;
		DTA1XX_IPTX_GETFIFOLOAD_CTRL  m_TxGetFifoLoadCtrl;
		DTA1XX_IPTX_IPPARS_CTRL  m_TxIpParsCtrl;
		DTA1XX_IPTX_RESET_CTRL  m_TxResetCtrl;
		DTA1XX_IPTX_SETTXCONTROL_CTRL  m_TxSetTxControlCtrl;
		DTA1XX_IPTX_SETTXMODE_CTRL  m_TxSetTxModeCtrl;
	}  m_Data;
} DTA1XX_IP_RXTX_CTRL, *PDTA1XX_IP_RXTX_CTRL;

// Return data
typedef struct _DTA1XX_IP_RXTX_RTRN {

	// Big union with specific return data for IP Rx/Tx IOCTLs
	union {
		DTA1XX_IPRX_ATTACH_RTRN  m_AttachRtrn;
		DTA1XX_IPRX_DETACH_RTRN  m_DetachRtrn;
		DTA1XX_IPRX_FLAGS_RTRN  m_FlagsRtrn;
		DTA1XX_IPRX_GETFIFOLOAD_RTRN  m_GetFifoLoadRtrn;
		DTA1XX_IPRX_GETSTATUS_RTRN  m_GetStatusRtrn;
		DTA1XX_IPRX_GETTSRATE_RTRN  m_GetTsRateRtrn;
		DTA1XX_IPRX_IPPARS_RTRN  m_IpParsRtrn;
		DTA1XX_IPRX_IPSTAT_RTRN  m_IpStatRtrn;
		DTA1XX_IPRX_READ_RTRN  m_ReadRtrn;
		DTA1XX_IPRX_RESET_RTRN  m_ResetRtrn;
		DTA1XX_IPRX_SETRXCONTROL_RTRN  m_SetRxControlRtrn;
		DTA1XX_IPRX_SETRXMODE_RTRN  m_SetRxModeRtrn;

		DTA1XX_IPTX_ATTACH_RTRN  m_TxAttachRtrn;
		DTA1XX_IPTX_DETACH_RTRN  m_TxDetachRtrn;
		DTA1XX_IPTX_FLAGS_RTRN  m_TxFlagsRtrn;
		DTA1XX_IPTX_GETFIFOLOAD_RTRN  m_TxGetFifoLoadRtrn;
		DTA1XX_IPTX_IPPARS_RTRN  m_TxIpParsRtrn;
		DTA1XX_IPTX_RESET_RTRN  m_TxResetRtrn;
		DTA1XX_IPTX_SETTXCONTROL_RTRN  m_TxSetTxControlRtrn;
		DTA1XX_IPTX_SETTXMODE_RTRN  m_TxSetTxModeRtrn;
	}  m_Data;
} DTA1XX_IP_RXTX_RTRN, *PDTA1XX_IP_RXTX_RTRN;

//-.-.-.-.-.-.-.-.-.-.-.- Union of main all IOCTL data structures -.-.-.-.-.-.-.-.-.-.-.-.
typedef union _DTA1XX_IOCTL_DATA {
	DTA1XX_ADDR_INFO  m_AddrInfo;			// Address info get
#if defined(WIN32) || defined(CONFIG_X86_64)
	DTA1XX_ADDR_INFO_32  m_AddrInfo_32;		// Address info for 32-bit applications get
#endif
	DTA1XX_CDMA_CONTROL  m_CdmaControl;		// Continuous DMA control
	DTA1XX_CLOCK_ERR  m_ClockErr;			// Clock error
	DTA1XX_DRIVER_VERSION  m_DriverVersion;	// Versioning info
	DTA1XX_EEPROM_CONTROL  m_EepromControl;	// Special EEPROM-control operation
	DTA1XX_EXCL_ACCESS_INFO  m_ExclAccess;	// Exclusive-access info
	DTA1XX_FLAGS  m_Flags;					// Flags
	DTA1XX_IOCONFIG  m_IoConfig;			// I/O-Configuration info (legacy)
	DTA1XX_IOCONFIG2  m_IoConfig2;			// I/O-Configuration info
	DTA1XX_IP_RXTX_CTRL  m_IpRxTxCtrl;		// IP Rx/Tx control (with sub-union)
	DTA1XX_IP_RXTX_RTRN  m_IpRxTxRtrn;		// IP Rx/Tx return data (with sub-union)
	DTA1XX_LOC_INFO  m_LocInfo;				// Location info
	DTA1XX_MACADDRESS  m_MacAddress;		// Mac Address of IP port
	DTA1XX_MOD_CONTROL  m_ModControl;		// Modulation control
	DTA1XX_NWI_INFO  m_NwiInfo;				// NW internal Information
	DTA1XX_NWI_RESET  m_NwiReset;			// NW internal Reset
	DTA1XX_NWI_SPEED  m_NwiSpeed;			// NW internal Speed / Duplex
	DTA1XX_NWI_STATUS  m_NwiStatus;			// NW internal Status
	DTA1XX_NWLOOPBACK  m_NwLoopback;		// Loopback for ethernet
	DTA1XX_NWSPEED  m_NwSpeed;				// Speed settings for ethernet
	DTA1XX_NWSTATCNT m_NwStatCnt;			// Network status/counter info
	DTA1XX_OUTPUT_LEVEL  m_OutputLevel;		// Output level
	DTA1XX_PORT_INTPAR  m_PortIntpar;		// Generic integer port parameter
	DTA1XX_PORT_2INTPAR  m_Port2Intpar;		// Generic 2 integer port parameters
	DTA1XX_RATE_INFO  m_RateInfo;			// Transport-stream rate info
	DTA1XX_RATE_INFO2  m_RateInfo2;			// Transport-stream rate info - new style
	DTA1XX_RF_CONTROL  m_RfControl;			// RF control
	DTA1XX_RF_MODE  m_RfMode;				// RF mode
	DTA1XX_RST_INFO  m_RstInfo;				// Reset info
	DTA1XX_RX_INFO  m_RxInfo;				// Receive-status info
	DTA1XX_RW_INFO  m_RwInfo;				// Read/write info
#if defined(CONFIG_X86_64)
	DTA1XX_RW_INFO_32  m_RwInfo_32;			// Read/write info for 32-bit applications
#endif
	DTA1XX_SHAREDBUF_CTRL  m_ShBufCtrl;		// Shared buffer allocation/freeing control
#if defined(CONFIG_X86_64)
	DTA1XX_SHAREDBUF_CTRL_32  m_ShBufCtrl_32; // Shared buffer allocation/freeing control
											  // for 32-bit applications
#endif
	DTA1XX_SHAREDBUF_RTRN  m_ShBufRtrn;		// Shared buffer allocation/freeing return data
	DTA1XX_TARGET_INFO  m_TargetInfo;		// Target-adapter info
	DTA1XX_TX_CONTROL  m_TxControl;			// Transmit control
	DTA1XX_TX_MODE  m_TxMode;				// Transmit mode
	DTA1XX_RX_MODE  m_RxMode;				// Receive mode
	DTA1XX_I2C_TRANSFER m_I2cTransfer;		// I2C transfer read/write
	DTA1XX_VPD_TRANSFER  m_Vpd;				// VPD transfer data
	DTA1XX_VPD_DATA  m_VpdData;				// VPD read/write/delete item / raw data
	void*  m_pUserVirtual;					// For legacy drivers (see m_AddrInfo)
#ifdef WIN32
	void*POINTER_32  m_pUserVirtual_32;		// For legacy drivers (see m_AddrInfo_32)
#endif
	DTA1XX_FAILSAFE  m_Failsafe;			// Fail safe data
	DTA1XX_GENLOCK  m_Genlock;				// Genlock data
} DTA1XX_IOCTL_DATA;


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IOCTL +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Constants
#if defined(WIN32) || defined(WIN64)
	// Windows specific defines
	#define	DTA1XX_DEVICE_TYPE		45100	// DeviceType for DeviceIoControl operations
	#define	DTA1XX_IOCTL_BASE		3100	// Offset for Function
#else
	// Linux specific defines
	#define	DTA1XX_IOCTL_MAGIC		0xBB	// Magic number
	#define	DTA1XX_IOCTL_BASE		0x50	// Offset for Function
#endif


// Function Codes
// DON'T INSERT NEW VALUES BETWEEN EXISTING ONES, SO THAT NEW DRIVER VERSIONS REMAIN
// COMPATIBLE WITH USER APPLICATIONS THAT WERE COMPILED AGAINST AN OLD DRIVER!!
enum {
	DTA1XX_FUNCTION_CLEAR_FLAGS			= DTA1XX_IOCTL_BASE,
	DTA1XX_FUNCTION_DMA_USERMEM,
	DTA1XX_FUNCTION_GET_ADDR_OPER_REGS,
	DTA1XX_FUNCTION_GET_DRIVER_VERSION,
	DTA1XX_FUNCTION_GET_EXTCLK_FREQ,
	DTA1XX_FUNCTION_GET_FLAGS,
	DTA1XX_FUNCTION_GET_LOC_INFO,
	DTA1XX_FUNCTION_GET_TARGET_ID,
	DTA1XX_FUNCTION_GET_TS_RATE,
	DTA1XX_FUNCTION_GET_TX_CONTROL,
	DTA1XX_FUNCTION_GET_TX_MODE,
	DTA1XX_FUNCTION_READ_VPD,
	DTA1XX_FUNCTION_REQUEST_EXCL_ACCESS,
	DTA1XX_FUNCTION_RESET_SOFT,
	DTA1XX_FUNCTION_SET_TS_RATE,
	DTA1XX_FUNCTION_SET_TX_CONTROL,
	DTA1XX_FUNCTION_SET_TX_MODE,
	DTA1XX_FUNCTION_WRITE_VPD,
	DTA1XX_FUNCTION_EEPROM_CONTROL,
	DTA1XX_FUNCTION_CLR_OUTPUT_FIFO,
	DTA1XX_FUNCTION_GET_RX_INFO,
	DTA1XX_FUNCTION_FIRMWARE_REBOOT,
	DTA1XX_FUNCTION_GET_MOD_CONTROL,
	DTA1XX_FUNCTION_SET_MOD_CONTROL,
	DTA1XX_FUNCTION_NWI_GET_STATUS,
	DTA1XX_FUNCTION_NWI_QUERY_INFO,
	DTA1XX_FUNCTION_NWI_SET_INFO,
	DTA1XX_FUNCTION_NWI_SET_SPEED,
	DTA1XX_FUNCTION_IPRX_CTRL,
	DTA1XX_FUNCTION_IPTX_CTRL,
	DTA1XX_FUNCTION_CDMA_CTRL,
	DTA1XX_FUNCTION_SHAREDBUF_ALLOC,
	DTA1XX_FUNCTION_SHAREDBUF_FREE,
	DTA1XX_FUNCTION_GET_CLOCK_ERR,
	DTA1XX_FUNCTION_GET_IOCONFIG,
	DTA1XX_FUNCTION_SET_IOCONFIG,
	DTA1XX_FUNCTION_NWI_SET_VERSION,
	DTA1XX_FUNCTION_GET_NW_VERSION,
	DTA1XX_FUNCTION_GET_MACADDRESS,
	DTA1XX_FUNCTION_SET_NWLOOPBACK,
	DTA1XX_FUNCTION_SET_NWSPEED,
	DTA1XX_FUNCTION_GET_NWSPEED,
	DTA1XX_FUNCTION_NWI_RESET,
	DTA1XX_FUNCTION_GET_NWSTATCNT,
	DTA1XX_FUNCTION_SET_NWSTATCNT,
	DTA1XX_FUNCTION_VPD_DATA,
	DTA1XX_FUNCTION_GET_FAILSAFE,
	DTA1XX_FUNCTION_SET_FAILSAFE,
	DTA1XX_FUNCTION_I2C_TRANSFER,
	DTA1XX_FUNCTION_SET_MOD_CONTROL2,
	DTA1XX_FUNCTION_GET_RATE2,
	DTA1XX_FUNCTION_SET_RATE2,
	DTA1XX_FUNCTION_DMA_USERMEM_READ,	// Only used for Linux
	DTA1XX_FUNCTION_DMA_USERMEM_WRITE,	// Only used for Linux
	DTA1XX_FUNCTION_RELEASE_ADDR_OPER_REGS,  // Only used for Linux
	DTA1XX_FUNCTION_GET_GENLOCK,
	DTA1XX_FUNCTION_GET_IOCONFIG2,
	DTA1XX_FUNCTION_SET_IOCONFIG2,
	DTA1XX_FUNCTION_GET_RXCLKFREQ,
	DTA1XX_FUNCTION_GET_TXPHASEINCR,
	DTA1XX_FUNCTION_SET_TXPHASEINCR,
	DTA1XX_FUNCTION_SET_RX_MODE,
	DTA1XX_FUNCTION_GET_OUTPUT_LEVEL,
	DTA1XX_FUNCTION_SET_OUTPUT_LEVEL,
	DTA1XX_FUNCTION_GET_RF_CONTROL,
	DTA1XX_FUNCTION_SET_RF_CONTROL,
	DTA1XX_FUNCTION_SET_RF_MODE
};


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_CDMA_CTRL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_CDMA_CONTROL
// OutBuffer: DTA1XX_CDMA_CONTROL
// Result   : BOOL
//
// Continuous DMA (CDMA) - Control.
//

#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_CDMA_CTRL												\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_CDMA_CTRL,				\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTA1XX_CDMA_CTRL												\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_CDMA_CTRL,				\
				  DTA1XX_CDMA_CONTROL)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_CLEAR_FLAGS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_FLAGS  (contains flags to be cleared)
// OutBuffer: -
// Result   : BOOL
//
// Clear selected flags.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_CLEAR_FLAGS											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_CLEAR_FLAGS,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_CLEAR_FLAGS											\
			_IOW(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_CDMA_CTRL, DTA1XX_FLAGS)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_CLR_OUTPUT_FIFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : -
// OutBuffer: -
// Result   : BOOL
//
// Soft reset of PCI card.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_CLR_OUTPUT_FIFO										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_CLR_OUTPUT_FIFO,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_DMA_USERMEM_READ -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_RW_INFO or NULL (legacy)
// OutBuffer: Data buffer with data to be DMA-ed
// Result   : BOOL
//
// Read data (using DMA) from a PCI card into an application-supplied buffer in
// User-Space memory.
//
// NOTE: According to the DDK documentation, METHOD_IN_DIRECT should be replaced by
//       METHOD_OUT_DIRECT. However, in winioctl.h, METHOD_IN_DIRECT is defined to be
//       equivalent to METHOD_DIRECT_FROM_HARDWARE, which is exactly what we are doing
//       here. Dekker and Newcomer, in their book "Developing Windows NT Device Drivers"
//       support the view that METHOD_IN_DIRECT should be used here.
//       It appears that the I/O manager treats METHOD_IN_DIRECT and METHOD_OUT_DIRECT
//       in exactly the same way, so that in the end it does not matter!
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_DMA_USERMEM_READ										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_DMA_USERMEM,			\
					 METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTA1XX_DMA_USERMEM_READ										\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_DMA_USERMEM_READ,			\
				  DTA1XX_RW_INFO)
	
	#if defined(CONFIG_X86_64)
		// For 32-bit application on 64-bit Linux
		#define IOCTL_DTA1XX_DMA_USERMEM_READ_32								\
				_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_DMA_USERMEM_READ,		\
					  DTA1XX_RW_INFO_32)
	#endif
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_DMA_USERMEM_WRITE -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_RW_INFO or NULL (legacy)
// OutBuffer: Data buffer with data to be DMA-ed
// Result   : BOOL
//
// Write data (using DMA) to the PCI card from an application-supplied buffer in
// User-Space memory.
//
// NOTE: See DMA_USERMEM_READ, with IN and OUT swapped.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_DMA_USERMEM_WRITE										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_DMA_USERMEM,			\
					 METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTA1XX_DMA_USERMEM_WRITE										\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_DMA_USERMEM_WRITE,		\
				  DTA1XX_RW_INFO)

	#if defined(CONFIG_X86_64)
		// For 32-bit application on 64-bit Linux
		#define IOCTL_DTA1XX_DMA_USERMEM_WRITE_32								\
				_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_DMA_USERMEM_WRITE,	\
					  DTA1XX_RW_INFO_32)
	#endif
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_EEPROM_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_EEPROM_CONTROL
// OutBuffer: -
// Result   : BOOL
//
// Set the channel's transport-stream rate.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_EEPROM_CONTROL											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_EEPROM_CONTROL,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_EEPROM_CONTROL											\
			_IOW(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_EEPROM_CONTROL,			\
				 DTA1XX_EEPROM_CONTROL)
#endif



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_FIRMWARE_REBOOT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_FIRMWARE_REBOOT_CTRL
// OutBuffer: -
// Result   : BOOL
//
// Reboot firmware, e.g. for reloading of FPGA code after EEPROM update
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_FIRMWARE_REBOOT										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_FIRMWARE_REBOOT,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_FIRMWARE_REBOOT										\
			_IOW(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_FIRMWARE_REBOOT,			\
				 DTA1XX_FIRMWARE_REBOOT_CTRL)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_ADDR_OPER_REGS -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_ADDR_INFO		(LEGACY: Unused)
// OutBuffer: DTA1XX_ADDR_INFO		(LEGACY: void* m_pUserVirtual)
// Result   : BOOL
//
// Get virtual address of Dta1xx operational registers.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_ADDR_OPER_REGS										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_ADDR_OPER_REGS,	\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_ADDR_OPER_REGS										\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_ADDR_OPER_REGS,		\
				  DTA1XX_ADDR_INFO)

	#if defined(CONFIG_X86_64)
		// For 32-bit application on 64-bit Linux
		#define IOCTL_DTA1XX_GET_ADDR_OPER_REGS_32								\
				_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_ADDR_OPER_REGS,	\
					  DTA1XX_ADDR_INFO_32)
	#endif
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_CLOCK_ERR -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : -
// OutBuffer: DTA1XX_CLOCK_ERR
// Result   : BOOL
//
// Get clock error.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_CLOCK_ERR											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_CLOCK_ERR,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_CLOCK_ERR											\
			_IOR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_CLOCK_ERR,				\
				 DTA1XX_CLOCK_ERR)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_DRIVER_VERSION -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : -
// OutBuffer: DTA1XX_DRIVER_VERSION
// Result   : BOOL
//
// Get device-driver version.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_DRIVER_VERSION										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_DRIVER_VERSION,	\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_DRIVER_VERSION										\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_DRIVER_VERSION,		\
				  DTA1XX_DRIVER_VERSION)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_EXTCLK_FREQ -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : -
// OutBuffer: DTA1XX_RATE_INFO
// Result   : BOOL
//
// Get a measurement of the external-clock frequency.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_EXTCLK_FREQ										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_EXTCLK_FREQ,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_EXTCLK_FREQ										\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_EXTCLK_FREQ,			\
				  DTA1XX_RATE_INFO)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_FAILSAFE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : -
// OutBuffer: DTA1XX_FAILSAFE
// Result   : BOOL
//
// Get current value of failsafe state.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_FAILSAFE											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_FAILSAFE,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_FAILSAFE											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_FAILSAFE,				\
				  DTA1XX_FAILSAFE)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_GENLOCK -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : -
// OutBuffer: DTA1XX_GENLOCK
// Result   : BOOL
//
// Get current value of Genlock state.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_GENLOCK											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_GENLOCK,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_GENLOCK											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_GENLOCK,				\
				  DTA1XX_GENLOCK)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_FLAGS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : -
// OutBuffer: DTA1XX_FLAGS
// Result   : BOOL
//
// Get current value of status flags and latched status flags.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_FLAGS												\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_FLAGS,				\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_FLAGS												\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_FLAGS,				\
				  DTA1XX_FLAGS)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_IOCONFIG -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_IOCONFIG
// OutBuffer: DTA1XX_IOCONFIG
// Result   : BOOL
//
// Get the configuration of an I/O port
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_IOCONFIG											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_IOCONFIG,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_IOCONFIG											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_IOCONFIG,				\
				  DTA1XX_IOCONFIG)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_IOCONFIG2 .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_IOCONFIG2
// OutBuffer: DTA1XX_IOCONFIG2
// Result   : BOOL
//
// Get the configuration of an I/O port
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_IOCONFIG2											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_IOCONFIG2,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_IOCONFIG2											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_IOCONFIG2,			\
				  DTA1XX_IOCONFIG2)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_LOC_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : -
// OutBuffer: DTA1XX_GET_LOC_INFO
// Result   : BOOL
//
// Get location info (PCI-bus number, slot number, PnP location string) and (sub-system)
// device ID and vendor ID.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_LOC_INFO											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_LOC_INFO,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_LOC_INFO											\
			_IOR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_LOC_INFO,				\
				 DTA1XX_LOC_INFO)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_MACADDRESS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_MACADDRESS
// OutBuffer: DTA1XX_MACADDRESS
// Result   : BOOL
//
// Get Mac Address of specified Ip port
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_MACADDRESS											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_MACADDRESS,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_MACADDRESS											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_MACADDRESS,			\
				  DTA1XX_MACADDRESS)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_MOD_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : -
// OutBuffer: DTA1XX_MOD_CONTROL
// Result   : BOOL
//
// Get modulation parameters.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_MOD_CONTROL										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_MOD_CONTROL,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_MOD_CONTROL										\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_MOD_CONTROL,			\
				  DTA1XX_MOD_CONTROL)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_NW_VERSION -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : -
// OutBuffer: DTA1XX_DRIVER_VERSION
// Result   : BOOL
//
// Get Network driver version.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_NW_VERSION										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_NW_VERSION,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_NW_VERSION										\
			_IOR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_NW_VERSION,			\
				 DTA1XX_DRIVER_VERSION)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_NWSTATCNT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_NWSTATCNT
// OutBuffer: DTA1XX_NWSTATCNT
// Result   : BOOL
//
// Force speed for ethernet
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_NWSTATCNT											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_NWSTATCNT,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_NWSTATCNT											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_NWSTATCNT,			\
				  DTA1XX_NWSTATCNT)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_NWSPEED -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : -
// OutBuffer: DTA1XX_NWSPEED
// Result   : BOOL
//
// Force speed for ethernet
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_NWSPEED											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_NWSPEED,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_NWSPEED											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_NWSPEED,				\
				 DTA1XX_NWSPEED)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_OUTPUT_LEVEL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_OUTPUT_LEVEL
// OutBuffer: DTA1XX_OUTPUT_LEVEL
// Result   : BOOL
//
// Get output level parameter.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_OUTPUT_LEVEL										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_OUTPUT_LEVEL,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_OUTPUT_LEVEL										\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_OUTPUT_LEVEL,			\
				  DTA1XX_OUTPUT_LEVEL)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA1XX_FUNCTION_GET_TXPHASEINCR -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_PORT_INTPAR
// OutBuffer: DTA1XX_PORT_INTPAR
// Result   : BOOL
//
// Get transmitter's I/O clock to TS byte clock NCO phase increment
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_TXPHASEINCR										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_TXPHASEINCR,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_TXPHASEINCR										\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_TXPHASEINCR,			\
				 DTA1XX_PORT_2INTPAR)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_RATE2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Successor of IOCTL_DTA1XX_GET_TS_RATE
//
// InBuffer : -
// OutBuffer: DTA1XX_RATE_INFO2
// Result   : BOOL
//
// Get the channel's transport-stream rate.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_RATE2												\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_RATE2,				\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_RATE2												\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_RATE2,				\
				  DTA1XX_RATE_INFO2)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_RF_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_RF_CONTROL
// OutBuffer: DTA1XX_RF_CONTROL
// Result   : BOOL
//
// Get output level parameter.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_RF_CONTROL											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_RF_CONTROL,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_RF_CONTROL											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_RF_CONTROL,			\
				  DTA1XX_RF_CONTROL)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_RF_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_RF_MODE
// OutBuffer: DTA1XX_RF_MODE
// Result   : BOOL
//
// Get RF mode parameter.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_RF_MODE											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_RF_MODE,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_RF_MODE											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_RF_MODE,				\
				  DTA1XX_RF_MODE)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_RX_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : -
// OutBuffer: DTA1XX_RX_INFO
// Result   : BOOL
//
// Get the channel's transport-stream rate.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_RX_INFO											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_RX_INFO,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_RX_INFO											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_RX_INFO,				\
				  DTA1XX_RX_INFO)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA1XX_FUNCTION_GET_RXCLKFREQ -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_PORT_INTPAR
// OutBuffer: DTA1XX_PORT_INTPAR
// Result   : BOOL
//
// Get receiver's clock frequency
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_RXCLKFREQ											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_RXCLKFREQ,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_RXCLKFREQ											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_RXCLKFREQ,			\
				 DTA1XX_PORT_INTPAR)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_TARGET_ID -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : -
// OutBuffer: DTA1XX_TARGET_INFO
// Result   : BOOL
//
// Get current value of status flags and latched status flags.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_TARGET_ID											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_TARGET_ID,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_TARGET_ID											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_TARGET_ID,			\
				  DTA1XX_TARGET_INFO)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_TS_RATE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : -
// OutBuffer: DTA1XX_RATE_INFO
// Result   : BOOL
//
// Get the channel's transport-stream rate.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_TS_RATE											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_TS_RATE,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_TS_RATE											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_TS_RATE,				\
				  DTA1XX_RATE_INFO)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_TX_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : -
// OutBuffer: DTA1XX_TX_CONTROL
// Result   : BOOL
//
// Get the channel's transmit-control state.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_TX_CONTROL											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_TX_CONTROL,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_TX_CONTROL											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_TX_CONTROL,			\
				  DTA1XX_TX_CONTROL)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_GET_TX_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : -
// OutBuffer: DTA1XX_TX_MODE
// Result   : BOOL
//
// Get the channel's transmit-control state.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_GET_TX_MODE											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_GET_TX_MODE,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_GET_TX_MODE											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_GET_TX_MODE,				\
				  DTA1XX_TX_MODE)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_I2C_TRANSFER -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_I2C_TRANSFER
// OutBuffer: DTA1XX_I2C_TRANSFER
// Result   : BOOL
//
// Read/Write data to onboard I2C-bus
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_I2C_TRANSFER											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_I2C_TRANSFER,			\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTA1XX_I2C_TRANSFER											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_I2C_TRANSFER,				\
				  DTA1XX_I2C_TRANSFER)

#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_IPRX_CTRL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_IP_RXTX_CTRL
// OutBuffer: DTA1XX_IP_RXTX_RTRN
// Result   : BOOL
//
// This IOCTL is a container for control of TS-Over-IP receive functions.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_IPRX_CTRL												\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_IPRX_CTRL,				\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_IPRX_CTRL											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_IPRX_CTRL,			\
				  DTA1XX_IP_RXTX_CTRL)

#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_IPTX_CTRL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_IP_RXTX_CTRL
// OutBuffer: DTA1XX_IP_RXTX_RTRN
// Result   : BOOL
//
// This IOCTL is a container for control of TS-Over-IP transmit functions.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_IPTX_CTRL												\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_IPTX_CTRL,				\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_IPTX_CTRL											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_IPTX_CTRL,			\
				  DTA1XX_IP_RXTX_CTRL)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_NWI_GET_STATUS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_NWI_STATUS
// OutBuffer: DTA1XX_NWI_STATUS
// Result   : BOOL
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_NWI_GET_STATUS										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_NWI_GET_STATUS,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_NWI_GET_STATUS											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_NWI_GET_STATUS,			\
				  DTA1XX_NWI_STATUS)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_NWI_QUERY_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_NWI_INFO
// OutBuffer: DTA1XX_NWI_INFO
// Result   : BOOL
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_NWI_QUERY_INFO										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_NWI_QUERY_INFO,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_NWI_QUERY_INFO											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_NWI_QUERY_INFO,			\
				  DTA1XX_NWI_INFO)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_NWI_RESET -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_NWI_RESET
// OutBuffer: -
// Result   : BOOL
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_NWI_RESET												\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_NWI_RESET,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_NWI_RESET											\
			_IOW(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_NWI_RESET,			\
				  DTA1XX_NWI_RESET)
#endif



//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_NWI_SET_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_NWI_INFO
// OutBuffer: DTA1XX_NWI_INFO
// Result   : BOOL
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_NWI_SET_INFO											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_NWI_SET_INFO,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_NWI_SET_INFO											\
			_IOW(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_NWI_SET_INFO,			\
				  DTA1XX_NWI_INFO)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_NWI_SET_SPEED -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_NWI_SPEED
// OutBuffer: DTA1XX_NWI_SPEED
// Result   : BOOL
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_NWI_SET_SPEED											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_NWI_SET_SPEED,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_NWI_SET_SPEED											\
			_IOW(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_NWI_SET_SPEED,			\
				  DTA1XX_NWI_SPEED)
#endif



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_NWI_SET_VERSION -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_DRIVER_VERSION
// OutBuffer: -
// Result   : BOOL
//
// Sets the number of in and outputs of the given channel type
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_NWI_SET_VERSION										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_NWI_SET_VERSION,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_NWI_SET_VERSION											\
			_IOW(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_NWI_SET_VERSION,			\
				  DTA1XX_DRIVER_VERSION)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_READ_EEPROM -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : UInt m_Start: 0 = Start of VPDID, UInt m_Length; size = 2*sizeof(UInt)
// OutBuffer: DTA1XX_VPD_TRANSFER
// Result   : BOOL
//
// Read specified range of VPD data BYTES. m_Start: 0 = Start of VPDID
// DTA1XX_VPD_TRANSFER.m_Vpd[0] will contain the byte read from EEPROM address
// m_Start + VpdId offset.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_READ_EEPROM											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUCNTION_READ_EEPROM,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_READ_EEPROM											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUCNTION_READ_EEPROM,				\
				  DTA1XX_VPD_TRANSFER)
#endif



//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_READ_VPD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : UInt m_Start, UInt m_Length; size = 2*sizeof(UInt)
// OutBuffer: DTA1XX_VPD_TRANSFER
// Result   : BOOL
//
// Read specified range of VPD data BYTES.
// DTA1XX_VPD_TRANSFER.m_Vpd[0] will contain the byte read from EEPROM address m_Start.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_READ_VPD												\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_READ_VPD,				\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_READ_VPD												\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_READ_VPD,					\
				  DTA1XX_VPD_TRANSFER)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_RELEASE_ADDR_OPER_REGS -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_ADDR_INFO
// OutBuffer: DTA1XX_ADDR_INFO
// Result   : BOOL
//
// Decreases reference count and returns E_BUSY in case reference count > 0
//
#if defined(WIN32) || defined(WIN64)
	// Windows
#else
	// Linux
	#define IOCTL_DTA1XX_RELEASE_ADDR_OPER_REGS									\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_RELEASE_ADDR_OPER_REGS,	\
				  DTA1XX_ADDR_INFO)

	#if defined(CONFIG_X86_64)
		// For 32-bit application on 64-bit Linux
		#define IOCTL_DTA1XX_RELEASE_ADDR_OPER_REGS_32							\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_RELEASE_ADDR_OPER_REGS,	\
				  DTA1XX_ADDR_INFO_32)
	#endif
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_REQUEST_EXCL_ACCESS -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_EXCL_ACCESS_INFO
// OutBuffer: DTA1XX_EXCL_ACCESS_INFO
// Result   : BOOL
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_REQUEST_EXCL_ACCESS									\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_REQUEST_EXCL_ACCESS,	\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_REQUEST_EXCL_ACCESS									\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_REQUEST_EXCL_ACCESS,		\
				  DTA1XX_EXCL_ACCESS_INFO)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_RESET_SOFT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_RST_INFO
// OutBuffer: -
// Result   : BOOL
//
// Soft reset of PCI card.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_RESET_SOFT												\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_RESET_SOFT,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_RESET_SOFT												\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_RESET_SOFT,				\
				  DTA1XX_RST_INFO)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_FAILSAFE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_FAILSAFE
// OutBuffer: -
// Result   : BOOL
//
// Set failsafe state.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_FAILSAFE											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_FAILSAFE,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_FAILSAFE											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_FAILSAFE,				\
				  DTA1XX_FAILSAFE)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_IOCONFIG -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_IOCONFIG
// OutBuffer: DTA1XX_IOCONFIG
// Result   : BOOL
//
// Configures an I/O port
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_IOCONFIG											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_IOCONFIG,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_IOCONFIG											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_IOCONFIG,				\
				  DTA1XX_IOCONFIG)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_IOCONFIG2 .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_IOCONFIG2
// OutBuffer: DTA1XX_IOCONFIG2
// Result   : BOOL
//
// Configures an I/O port
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_IOCONFIG2											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_IOCONFIG2,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_IOCONFIG2											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_IOCONFIG2,			\
				  DTA1XX_IOCONFIG2)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_MOD_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_MOD_CONTROL
// OutBuffer: -
// Result   : BOOL
//
// Set modulation-control parameters
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_MOD_CONTROL										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_MOD_CONTROL,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_MOD_CONTROL										\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_MOD_CONTROL,			\
				  DTA1XX_MOD_CONTROL)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_MOD_CONTROL2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Successor of IOCTL_DTA1XX_SET_MOD_CONTROL
//
// InBuffer : DTA1XX_MOD_CONTROL
// OutBuffer: -
// Result   : BOOL
//
// Set modulation-control parameters
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_MOD_CONTROL2										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_MOD_CONTROL2,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_MOD_CONTROL2										\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_MOD_CONTROL2,			\
				  DTA1XX_MOD_CONTROL)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_NWLOOPBACK -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_NWLOOPBACK
// OutBuffer: -
// Result   : BOOL
//
// Set loopback mode for ethernet
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_NWLOOPBACK											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_NWLOOPBACK,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_NWLOOPBACK												\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_NWLOOPBACK,				\
				  DTA1XX_NWLOOPBACK)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_NWSPEED -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_NWSPEED
// OutBuffer: -
// Result   : BOOL
//
// Force speed for ethernet
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_NWSPEED											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_NWSPEED,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_NWSPEED											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_NWSPEED,				\
				  DTA1XX_NWSPEED)
#endif



//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_NWSTATCNT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_NWSTATCNT
// OutBuffer: -
// Result   : BOOL
//
// Force speed for ethernet
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_NWSTATCNT											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_NWSTATCNT,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_NWSTATCNT											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_NWSTATCNT,			\
				  DTA1XX_NWSTATCNT)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA1XX_FUNCTION_SET_OUTPUT_LEVEL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_OUTPUT_LEVEL
// OutBuffer: -
// Result   : BOOL
//
// Set RF output level
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_OUTPUT_LEVEL									\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_OUTPUT_LEVEL,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_OUTPUT_LEVEL									\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_OUTPUT_LEVEL,			\
				 DTA1XX_OUTPUT_LEVEL)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA1XX_FUNCTION_SET_RF_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_RF_CONTROL
// OutBuffer: -
// Result   : BOOL
//
// Set RF control parameter
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_RF_CONTROL											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_RF_CONTROL,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_RF_CONTROL											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_RF_CONTROL,			\
				 DTA1XX_RF_CONTROL)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA1XX_FUNCTION_SET_RF_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_RF_MODE
// OutBuffer: -
// Result   : BOOL
//
// Set RF mode parameter
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_RF_MODE											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_RF_MODE,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_RF_MODE											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_RF_MODE,				\
				 DTA1XX_RF_MODE)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA1XX_FUNCTION_SET_TXPHASEINCR -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_PORT_INTPAR
// OutBuffer: DTA1XX_PORT_INTPAR
// Result   : BOOL
//
// Set transmitter's I/O clock to TS byte clock NCO phase increment
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_TXPHASEINCR										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_TXPHASEINCR,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_TXPHASEINCR										\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_TXPHASEINCR,			\
				 DTA1XX_PORT_2INTPAR)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_RATE2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Successor of IOCTL_DTA1XX_SET_TS_RATE
//
// InBuffer : DTA1XX_RATE_INFO2
// OutBuffer: -
// Result   : BOOL
//
// Set the channel's transport-stream rate.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_RATE2												\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_RATE2,				\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_RATE2												\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_RATE2,				\
				  DTA1XX_RATE_INFO2)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_TS_RATE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_RATE_INFO
// OutBuffer: -
// Result   : BOOL
//
// Set the channel's transport-stream rate.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_TS_RATE											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_TS_RATE,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_TS_RATE											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_TS_RATE,				\
				  DTA1XX_RATE_INFO)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_TX_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_TX_CONTROL
// OutBuffer: -
// Result   : BOOL
//
// Set the channel's transmit CONTROL.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_TX_CONTROL											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_TX_CONTROL,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_TX_CONTROL											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_TX_CONTROL,			\
				  DTA1XX_TX_CONTROL)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_TX_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_TX_MODE
// OutBuffer: -
// Result   : BOOL
//
// Set the channel's transmit mode.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_TX_MODE											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_TX_MODE,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_TX_MODE											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_TX_MODE,				\
				  DTA1XX_TX_MODE)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SET_RX_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_RX_MODE
// OutBuffer: -
// Result   : BOOL
//
// Set the channel's receive mode.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SET_RX_MODE											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SET_RX_MODE,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SET_RX_MODE											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SET_RX_MODE,				\
				  DTA1XX_RX_MODE)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SHAREDBUF_ALLOC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_SHAREDBUF_CTRL
// OutBuffer: Data buffer to be shared between driver and user space
//			  In this buffer: DTA1XX_SHAREDBUF_RTRN
// Result   : BOOL
//
// Supply a data buffer that can be shared between driver and user space application.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SHAREDBUF_ALLOC										\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SHAREDBUF_ALLOC,		\
					 METHOD_NEITHER, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTA1XX_SHAREDBUF_ALLOC										\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SHAREDBUF_ALLOC,			\
				  DTA1XX_SHAREDBUF_CTRL)

	
	#if defined(CONFIG_X86_64)
		// For 32-bit application on 64-bit Linux
		#define IOCTL_DTA1XX_SHAREDBUF_ALLOC_32									\
				_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SHAREDBUF_ALLOC,		\
					  DTA1XX_SHAREDBUF_CTRL_32)
	#endif
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_SHAREDBUF_FREE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTA1XX_SHAREDBUF_CTRL
// OutBuffer: DTA1XX_SHAREDBUF_RTRN
// Result   : BOOL
//
// Free a shared data buffer.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_SHAREDBUF_FREE											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_SHAREDBUF_FREE,		\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_SHAREDBUF_FREE											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SHAREDBUF_FREE,			\
				  DTA1XX_SHAREDBUF_CTRL)

	#if defined(CONFIG_X86_64)
		// For 32-bit application on 64-bit Linux
		#define IOCTL_DTA1XX_SHAREDBUF_FREE_32									\
				_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_SHAREDBUF_FREE,		\
					  DTA1XX_SHAREDBUF_CTRL_32)
	#endif
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_VPD_DATA -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTA1XX_VPD_DATA
// OutBuffer: DTA1XX_VPD_DATA
// Result   : BOOL
//
// Read/Write/Delete items or RAW data from/to VPD
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_VPD_DATA												\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_VPD_DATA,				\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_VPD_DATA												\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_VPD_DATA,					\
				  DTA1XX_VPD_DATA)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_WRITE_EEPROM -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : UInt m_Start: 0 = Start of VPDID, UInt m_Length; size = 2*sizeof(UInt) +
//																#bytes_to_write
// OutBuffer: -
// Result   : BOOL
//
// Write specified range of VPD data BYTES. m_Start: 0 = Start of VPDID
// DTA1XX_TRANSFER.m_Vpd[0] shall contain the byte to be written to EEPROM address
// m_Start+VpdId offset.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_WRITE_EEPROM											\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_WRITE_EEPROM,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_WRITE_EEPROM											\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_WRITE_EEPROM,				\
				  DTA1XX_VPD_TRANSFER)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTA1XX_WRITE_VPD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : UInt m_Start, UInt m_Length; size = 2*sizeof(UInt) + #bytes_to_write
// OutBuffer: -
// Result   : BOOL
//
// Write specified range of VPD data BYTES
// DTA1XX_TRANSFER.m_Vpd[0] shall contain the byte to be written to EEPROM address
// m_Start.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTA1XX_WRITE_VPD												\
			CTL_CODE(DTA1XX_DEVICE_TYPE, DTA1XX_FUNCTION_WRITE_VPD,				\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTA1XX_WRITE_VPD												\
			_IOWR(DTA1XX_IOCTL_MAGIC, DTA1XX_FUNCTION_WRITE_VPD,				\
				  DTA1XX_VPD_TRANSFER)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Helper Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// External Driver IoCtl functions
#if defined(WIN32) || defined(WIN64)
	// Windows

#else
	// Linux
Int Dta1xxCloseI(Int CardIndex, UInt32 ChildId);
Int Dta1xxIoCtlI(Int CardIndex, UInt32 ChildId, UInt IoControlCode, 
				 DTA1XX_IOCTL_DATA* pIoctlData);
Int Dta1xxOpenI(Int CardIndex, UInt32 ChildId);

#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Channel Definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTA1XX_GEN_REGS			-1		// Special value for GENERIC registers
#define  DTA1XX_TS_TX_CHANNEL		0		// Transport-Stream Transmit Channel
#define  DTA1XX_TS_RX_CHANNEL		1		// Transport-Stream Receive Channel
#define  DTA1XX_IP_TX_CHANNEL		10		// IP Transmit Channel
#define  DTA1XX_IP_RX_CHANNEL		11		// IP Receive Channel
#define  DTA1XX_NW_TX_CHANNEL		20		// NW Transmit Channel
#define  DTA1XX_NW_RX_CHANNEL		21		// NW Receive Channel
#define  DTA1XX_CHANNEL_UNUSED		99		// Channel not used

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CDMA Control -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTA1XX_CDMA_SETBUF			1
#define  DTA1XX_CDMA_START			2
#define  DTA1XX_CDMA_STOP			3
#define  DTA1XX_CDMA_WAITSEGM		4
#define  DTA1XX_CDMA_GETBUFPOS		5

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Reset Modes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTA1XX_RST_CLEARFIFO		0		// Clear FIFO without disrupting channel
#define  DTA1XX_RST_CHANNEL			1		// Channel reset

//.-.-.-.-.-.-.-.-.-.-.-.-.- Shared Buffer Allocation / Freeing -.-.-.-.-.-.-.-.-.-.-.-.-.
// Purpose
#define  DTA1XX_SHBUF_DIRIQ_CDMA	0		// DIRECT-IQ: CDMA buffer
#define  DTA1XX_SHBUF_IP_RWBUF		1		// IP: Read/Write buffer

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- VPD Read/Write -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Write flags
#define DTA1XX_VPDFLAG_RO_WRITE_ALLOWED	1
#define DTA1XX_VPDFLAG_UPDATE_EEPROM	2

// Section types
#define DTA1XX_VPDSECT_ID				1
#define DTA1XX_VPDSECT_RO				2
#define	DTA1XX_VPDSECT_RW				4

// Commands for VPD read/write/delete
#define DTA1XX_VPDCMD_ITEM_READ			1
#define DTA1XX_VPDCMD_ITEM_WRITE		2
#define DTA1XX_VPDCMD_ITEM_DELETE		3
#define DTA1XX_VPDCMD_RAW_READ			4
#define DTA1XX_VPDCMD_RAW_WRITE			5

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- I2C Read/Write -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Commands for I2C read/write
#define DTA1XX_I2CCMD_READ				1
#define DTA1XX_I2CCMD_WRITE				2

//+=+=+=+=+=+=+=+=+ Definitions below must match definitions in DTAPI.h! +=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EEPROM Control -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTA1XX_EEPROM_PROTECT		0x1FEE32CC
#define  DTA1XX_EEPROM_PROTECT_CLR	0x28392A3B

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- I/O Configuration -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// I/O ConfigCode
#define  DTA1XX_IOCONFIG_IOCONFIG	1
#define  DTA1XX_IOCONFIG_IOSTD		2
#define  DTA1XX_IOCONFIG_IOCLKSEL	3
#define  DTA1XX_IOCONFIG_IORATESEL	4
#define  DTA1XX_IOCONFIG_IOMODE		5
// ConfigCode DTA1XX_IOCONFIG_CONFIG
#define  DTA1XX_IOCONFIG_NOTSUP		0
#define  DTA1XX_IOCONFIG_INPUT		1
#define  DTA1XX_IOCONFIG_INPUT_APSK	11
#define  DTA1XX_IOCONFIG_OUTPUT		2
#define  DTA1XX_IOCONFIG_DBLBUF		3
#define  DTA1XX_IOCONFIG_IP			4
#define  DTA1XX_IOCONFIG_FAILSAFE	5
#define  DTA1XX_IOCONFIG_DIVERSITY	6
#define  DTA1XX_IOCONFIG_LOOPTHR	7
#define	 DTA1XX_IOCONFIG_SHARED		8
#define	 DTA1XX_IOCONFIG_GENREF		9
#define  DTA1XX_IOCONFIG_GENLOCKED	10
#define  DTA1XX_IOCONFIG_DISABLED	255

// ConfigCode DTA1XX_IOCONFIG_IOSTD
#define  DTA1XX_IOSTD_DISABLED		1
#define  DTA1XX_IOSTD_LVDS1			2
#define  DTA1XX_IOSTD_LVDS2			3
#define  DTA1XX_IOSTD_LVTTL			4
// ConfigCode DTA1XX_IOCONFIG_IOMODE
#define  DTA1XX_IOMODE_STANDARD		1 // DVB-SPI (DVB-ASI), BT.656 27 MHz
#define  DTA1XX_IOMODE_FIXEDCLK		2 // DVB fixed-clock mode, BT.656 variable clock
#define  DTA1XX_IOMODE_SERIAL8B		3 // DVB 8-bit serial on parallel port
#define  DTA1XX_IOMODE_SERIAL10B	4 // DVB 10-bit serial on parallel port
// ConfigCode DTA1XX_IOCONFIG_IOCLKSEL
#define  DTA1XX_IOCLKSEL_INTCLK		1 // Default clock for this port
#define  DTA1XX_IOCLKSEL_EXTCLK		2 // Clock from external clock input
#define  DTA1XX_IOCLKSEL_PORT		3 // Clock from port <parXtra>
// ConfigCode DTA1XX_IOCONFIG_IORATESEL
#define  DTA1XX_IORATESEL_INTCLK	1
#define  DTA1XX_IORATESEL_EXTCLK	2

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Genlock Parameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA1XX_GENLOCK_SDI625		0		// "PAL"
#define DTA1XX_GENLOCK_SDI525		1		// "NTSC"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Upconverter RF modes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA1XX_UPCONV_NORMAL		0
#define DTA1XX_UPCONV_MUTE			1
#define DTA1XX_UPCONV_CW			2
#define DTA1XX_UPCONV_CWI			3
#define DTA1XX_UPCONV_CWQ			4

#define DTA1XX_UPCONV_SPECINV		0x100	// Can be OR-ed with other RF modes


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Modulation Parameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE:
// DTA1XX_MOD constants below do NOT correspond 1-to-1 with modulation-type register
// For definition of modulation-type register values, see DTA1XX_TXMODC_ constants
// defined in Dta1xxRegs.h
#define  DTA1XX_MOD_DVBS_QPSK		0		// Native DVB-S on DTA-107
#define  DTA1XX_MOD_DVBS_BPSK		1		// Not used yet
#define  DTA1XX_MOD_QAM4			3
#define  DTA1XX_MOD_QAM16			4
#define  DTA1XX_MOD_QAM32			5
#define  DTA1XX_MOD_QAM64			6
#define  DTA1XX_MOD_QAM128			7
#define  DTA1XX_MOD_QAM256			8
#define  DTA1XX_MOD_DVBT			9
#define  DTA1XX_MOD_ATSC			10
#define  DTA1XX_MOD_DVBT2			11
#define  DTA1XX_MOD_ISDBT			12
#define  DTA1XX_MOD_IQDIRECT		15
#define  DTA1XX_MOD_IQDIRECT_NOLIC	78126315
#define  DTA1XX_MOD_DVBS_QPSK_SOFT	16		// Soft DVB-S for DTA-107S2
#define  DTA1XX_MOD_DVBS2_QPSK		32
#define  DTA1XX_MOD_DVBS2_8PSK		33
#define  DTA1XX_MOD_DMBTH			48
#define  DTA1XX_MOD_ADTBT			49
#define  DTA1XX_MOD_CMMB			50
#define  DTA1XX_MOD_T2MI			51

#define  DTA1XX_MOD_ATSC_VSB8		0
#define  DTA1XX_MOD_ATSC_VSB16		1

#define  DTA1XX_MOD_1_2				0		// Code rate 1/2
#define  DTA1XX_MOD_2_3				1		// Code rate 2/3
#define  DTA1XX_MOD_3_4				2		// Code rate 3/4
#define  DTA1XX_MOD_4_5				3		// Code rate 4/5
#define  DTA1XX_MOD_5_6				4		// Code rate 5/6
#define  DTA1XX_MOD_6_7				5		// Code rate 6/7
#define  DTA1XX_MOD_7_8				6		// Code rate 7/8
#define  DTA1XX_MOD_1_4				7		// Code rate 1/4
#define  DTA1XX_MOD_1_3				8		// Code rate 1/3
#define  DTA1XX_MOD_2_5				9		// Code rate 2/5
#define  DTA1XX_MOD_3_5				10		// Code rate 3/5
#define  DTA1XX_MOD_8_9				11		// Code rate 8/9
#define  DTA1XX_MOD_9_10			12		// Code rate 9/10

#define  DTA1XX_MOD_J83_MSK			0x0003
#define  DTA1XX_MOD_J83_A			2		// J.83 annex A (DVB-C)
#define  DTA1XX_MOD_J83_B			3		// J.83 annex B (“American QAM”)
#define  DTA1XX_MOD_J83_C			1		// J.83 annex C (“Japanese QAM”)
#define  DTA1XX_MOD_DVBC_SOFT		0x0100	// Soft DVB-C flag

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Modulation Parameters (DVB-T) -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTA1XX_MOD_DVBT_BW_MSK		0x0000000F
#define  DTA1XX_MOD_DVBT_5MHZ		0x00000001
#define  DTA1XX_MOD_DVBT_6MHZ		0x00000002
#define  DTA1XX_MOD_DVBT_7MHZ		0x00000003
#define  DTA1XX_MOD_DVBT_8MHZ		0x00000004
#define  DTA1XX_MOD_DVBT_CO_MSK		0x000000F0
#define  DTA1XX_MOD_DVBT_QPSK		0x00000010
#define  DTA1XX_MOD_DVBT_QAM16		0x00000020
#define  DTA1XX_MOD_DVBT_QAM64		0x00000030
#define  DTA1XX_MOD_DVBT_GU_MSK		0x00000F00
#define  DTA1XX_MOD_DVBT_G_1_32		0x00000100
#define  DTA1XX_MOD_DVBT_G_1_16		0x00000200
#define  DTA1XX_MOD_DVBT_G_1_8		0x00000300
#define  DTA1XX_MOD_DVBT_G_1_4		0x00000400
#define  DTA1XX_MOD_DVBT_IL_MSK		0x0000F000
#define  DTA1XX_MOD_DVBT_INDEPTH	0x00001000
#define  DTA1XX_MOD_DVBT_NATIVE		0x00002000
#define  DTA1XX_MOD_DVBT_MD_MSK		0x000F0000
#define  DTA1XX_MOD_DVBT_2K			0x00010000
#define  DTA1XX_MOD_DVBT_4K			0x00020000
#define  DTA1XX_MOD_DVBT_8K			0x00030000

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Packet Size -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTA1XX_PCKSIZE_INV			0
#define  DTA1XX_PCKSIZE_188			2
#define  DTA1XX_PCKSIZE_204			3

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Receive Control -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTA1XX_RXCTRL_IDLE			0
#define  DTA1XX_RXCTRL_RCV			1

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Receive Mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTA1XX_RXMODE_ST188		0
#define  DTA1XX_RXMODE_ST204		1
#define  DTA1XX_RXMODE_STMP2		2
#define  DTA1XX_RXMODE_STRAW		3
#define  DTA1XX_RXMODE_IPRAW		5
#define  DTA1XX_RXMODE_STTRP		7
#define  DTA1XX_RXMODE_MASK			7
#define  DTA1XX_RX_TIMESTAMP		8

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Status Flags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTA1XX_RX_DMA_PENDING		1
#define  DTA1XX_RX_FIFO_OVF			2
#define  DTA1XX_RX_SYNC_ERR			4
#define  DTA1XX_RX_RATE_OVF			8
#define  DTA1XX_RX_TARGET_ERR		16

#define  DTA1XX_TX_DMA_PENDING		1
#define  DTA1XX_TX_FIFO_UFL			2
#define  DTA1XX_TX_SYNC_ERR			4
#define  DTA1XX_TX_READBACK_ERR		8
#define  DTA1XX_TX_TARGET_ERR		16

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Transmit Control -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTA1XX_TXCTRL_IDLE			0
#define  DTA1XX_TXCTRL_HOLD			1
#define  DTA1XX_TXCTRL_SEND			3

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Packet Transmit Mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTA1XX_TXMODE_188			0
#define  DTA1XX_TXMODE_204			1
#define  DTA1XX_TXMODE_ADD16		2
#define  DTA1XX_TXMODE_RAW			3
#define  DTA1XX_TXMODE_192			4		// Supported on DTA-102 Fimrware Rev >= 2
#define  DTA1XX_TXMODE_130			5		// Supported on DTA-102 Fimrware Rev >= 7
#define  DTA1XX_TXMODE_MIN16		6		// Supported on DTA-100 Firmware Rev >= 5
											// and          DTA-102 Firmware Rev >= 8
#define	 DTA1XX_TXMODE_BURST		0x010	// Can be OR-ed with one of 188/192/...
#define	 DTA1XX_TXMODE_SDI			0x040
#define	 DTA1XX_TXMODE_SDI_10BIT	0x080
#define	 DTA1XX_TXMODE_SDI_625		0x100
#define  DTA1XX_TXMODE_SDI_HUFFMAN	0x200	// SDI Line compression on


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Target Adapter Flags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTA1XX_NO_CONNECTION		0
#define  DTA1XX_DVB_SPI_SINK		1
#define  DTA1XX_TARGET_PRESENT		2
#define  DTA1XX_TARGET_UNKNOWN		3

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Protocol -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTA1XX_IP_PROTO_UDP		0
#define  DTA1XX_IP_PROTO_RTP		1
#define  DTA1XX_IP_PROTO_AUTO		2
#define  DTA1XX_IP_PROTO_UNKNOWN	2

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- FEC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTA1XX_IP_FEC_DISABLE		0
#define  DTA1XX_IP_FEC_2D			1

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IP Rx Buffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTA1XX_IPRX_BUFWRAPSIZE	256
#define  MAX_FEC_RECONSTR_ELEMENTS	20		// (L,D <= 20, L*D<=100, L+D max 20) for FEC
#define  MAX_NUM_RTP_DVB_PACKETS	100		// (L*D = 100)
#define  MAX_NUM_FEC_MATRIX			5		// (Last FEC max L*D after last packet -->
											// (L*D)+(L*D)+extra store for incoming pckts
#define  DTA1XX_IPRX_MAX_RTP_PACKETS	((MAX_FEC_RECONSTR_ELEMENTS +					\
										  MAX_NUM_RTP_DVB_PACKETS) * MAX_NUM_FEC_MATRIX)
											// ((L+D)+(L*D)) * NumMatrix
#define  DTA1XX_IPRX_MAX_PACKET_LENGTH	(1500+32)
#define  DTA1XX_IPRX_BUFRTPSIZE			(DTA1XX_IPRX_MAX_RTP_PACKETS *	\
										 DTA1XX_IPRX_MAX_PACKET_LENGTH)
#if defined(WIN32) || defined(WIN64)
#else
typedef struct _Dta1xxNwBuf{
	KEVENT				m_WaitQueue;
	volatile UInt		m_PktAvail;
	volatile UInt		m_ReadOffset;
	volatile UInt		m_WriteOffset;
	UInt8*				m_pBuffer;
	UInt				m_BufSize;
}Dta1xxNwBuf;
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IP Tx Buffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// This structure holds common state data used for interaction between DTAPI and driver

// 20x 32 bits = 80 bytes. If length is changed, change DTA1XX_IPTX_BUFCOMMSIZE
typedef struct _UserIpTxCommon {
			 UInt32  m_Reserved1[4];
			 UInt32  m_Reserved2;
	volatile UInt32  m_ReadOffset;		// Read offset; updated by DPC
			 UInt32  m_Reserved3;
	volatile UInt32  m_WriteOffset;		// Write offset; updated by DTAPI
	volatile UInt64  m_StartTimestamp;	// Initial timestamp after starting Tx channel
	UInt32	m_Reserved5[10];
} UserIpTxCommon;

#define  DTA1XX_IPTX_MAX_PACKET_LENGTH	(1500+32)	// Header + Max packet length
#define  DTA1XX_IPTX_BUFCOMMSIZE		(sizeof(UserIpTxCommon))

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Network speed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA1XX_NWSPEED_AUTO			0		// Set
#define DTA1XX_NWSPEED_NOLINK		0		// Get
#define DTA1XX_NWSPEED_10MB_HALF	1
#define DTA1XX_NWSPEED_10MB_FULL	2
#define DTA1XX_NWSPEED_100MB_HALF	3
#define DTA1XX_NWSPEED_100MB_FULL	4
#define DTA1XX_NWSPEED_1GB_MASTER	5
#define DTA1XX_NWSPEED_1GB_SLAVE	6

//.-.-.-.-.-.-.-.-.-.-.-.-.- Network status / counter registers -.-.-.-.-.-.-.-.-.-.-.-.-.
#define DTA1XX_NWSTATCNT_BMCR				0x00	// Basic Mode Control Register
#define DTA1XX_NWSTATCNT_BMSR				0x01	// Basic Mode Status Register
#define DTA1XX_NWSTATCNT_PHYIDR1			0x02	// PHY Identifier Register #1
#define DTA1XX_NWSTATCNT_PHYIDR2			0x03	// PHY Identifier Register #2
#define DTA1XX_NWSTATCNT_ANAR				0x04	// Auto-Negotioation Advertisement Register
#define DTA1XX_NWSTATCNT_ANLPAR				0x05	// Auto-Negotiation Link Partner Ability Register
#define DTA1XX_NWSTATCNT_ANER				0x06	// Auto-Negotiation Expansion Register
#define DTA1XX_NWSTATCNT_ANNPTR				0x07	// Auto-Negotiation Next Page Tx
#define DTA1XX_NWSTATCNT_ANNPRR				0x08	// Auto-Negotiation Next Page Rx
#define DTA1XX_NWSTATCNT_1KTCR				0x09	// 1000BASE-T Control Register
#define DTA1XX_NWSTATCNT_1KSTSR				0x0A	// 1000BASE-T Status Register
// 0x0B..0x0E: Reserved
#define DTA1XX_NWSTATCNT_1KSCR				0x0F	// 1000BASE-T Extended Status Register

// Extra National registers
#define DTA1XX_NWSTATCNT_NAT_STRAP_REG		0x10	// Strap Options Register
#define DTA1XX_NWSTATCNT_NAT_LINK_AN		0x11	// Link and Auto-Negotiation Status Register
#define DTA1XX_NWSTATCNT_NAT_AUC_CTRL		0x12	// Auxilliary control Register
#define DTA1XX_NWSTATCNT_NAT_LED_CTRL		0x13	// LED Control Register
#define DTA1XX_NWSTATCNT_NAT_INT_STATUS		0x14	// Interrupt Status Register
#define DTA1XX_NWSTATCNT_NAT_INT_MASK		0x15	// Interrupt Mask Register
#define DTA1XX_NWSTATCNT_NAT_EXP_MEM_CTRL	0x16	// Expanded Memory Access Control
#define DTA1XX_NWSTATCNT_NAT_INT_CLEAR		0x17	// Interrupt Clear Register
#define DTA1XX_NWSTATCNT_NAT_BIST_CNT		0x18	// BIST Counter Register
#define DTA1XX_NWSTATCNT_NAT_BIST_CFG1		0x19	// BIST Configuration Register #1
#define DTA1XX_NWSTATCNT_NAT_BIST_CFG2		0x1A	// BIST Configuration Register #2
// 0x1B..0x1C: Reserved
#define DTA1XX_NWSTATCNT_NAT_EXP_MEM		0x1D	// Special handling
// 0x1E: see DTA1XX_NWSTATCNT_NAT_EXP_MEM
#define DTA1XX_NWSTATCNT_NAT_PHYSUP			0x1F	// PHY Support Register

// Extra Marvell register
#define DTA1XX_NWSTATCNT_MAR_PHY_CTRL		0x10	// Phy Specific Control Register
#define DTA1XX_NWSTATCNT_MAR_LINK_AN		0x11	// Link and Auto-Negotiation Status Register
#define DTA1XX_NWSTATCNT_MAR_INT_MASK		0x12	// Interrupt Mask Register
#define DTA1XX_NWSTATCNT_MAR_INT_STATUS		0x13	// Interrupt Status Register
#define DTA1XX_NWSTATCNT_MAR_EXT_PHY_CTRL	0x14	// Extended Phy Specific Control Register
#define DTA1XX_NWSTATCNT_MAR_RX_ERR_CNT		0x15	// Receive Error Counter Register
#define DTA1XX_NWSTATCNT_MAR_PAGE_SEL		0x16	// Page Select Register!!!!
#define DTA1XX_NWSTATCNT_MAR_GLOB_STAT		0x17	// Global Status Register
#define DTA1XX_NWSTATCNT_MAR_LED_CTRL		0x18	// LED Control Register
#define DTA1XX_NWSTATCNT_MAR_MAN_LED_CTRL	0x19	// Manual LED Override Register
#define DTA1XX_NWSTATCNT_MAR_EXT_PHY_CTRL2	0x1A	// Extended Phy Specific Control2 Register
#define DTA1XX_NWSTATCNT_MAR_EXT_PHY_STAT	0x1B	// Extended Phy Specific Status Register
#define DTA1XX_NWSTATCNT_MAR_VIRT_CABLE_TST	0x1C	// Virtual Cable Tester
#define DTA1XX_NWSTATCNT_MAR_EXT_ADDR		0x1D	// Extended Address
#define DTA1XX_NWSTATCNT_MAR_EXT_DATA		0x1E	// Extended Data
// 0x1F: Reserved

// Extra common registers
#define DTA1XX_NWSTATCNT_RXFIFOOVERFL		0x40	// Rx Fifo Overflow
#define DTA1XX_NWSTATCNT_ADMINSTATUS		0x41	// Network Administrative status


// DTA1XX_NWSTATCNT_ADMINSTATUS bit defines
#define DTA1XX_NW_ADMINSTATUS_DOWN			0x2
#define DTA1XX_NW_ADMINSTATUS_UP			0x1


#endif // #ifndef __DTA1XX_COMMON_H
