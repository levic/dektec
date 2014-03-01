//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dtu2xxCommon.h *#*#*#*#*#*#*#* (C) 2000-2007 DekTec
//
// Header file with common data structures between the Dtu2xx driver and application
// programs, mainly DTAPI.
//
// ASSUMPTIONS:
//	- In the driver:
//			* "DriverTypes.h" has been included.
//			* DTU2XX_DRIVER is defined
//	- In a user-mode application:
//			* "StandardTypes.h" has been included.

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	SD	2002.03.29	Add GET_RX_INFO ioctl
//	SD	2002.03.05	Add CLR_OUTPUT_FIFO ioctl
//	SD	2001.02.09	Re-arranged function codes
//	SD	2000.10.29	Created

//#pragma once
#ifndef __DTU2X_COMMON_H
#define __DTU2X_COMMON_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#if defined(WIN32) || defined(WIN64)
	// Windows specific includes
	#if defined(DTU2XX_DRIVER)
		#include <ntddk.h>			// Cannot be used in DTAPI
	#else
		#include <winioctl.h>		// Cannot be used in driver
	#endif
#else
	// Linux specific includes
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+ Ioctl Input / Output Data Structures +=+=+=+=+=+=+=+=+=+=+=+=+

// Constants
#define DTU2XX_LOC_BUF_SIZE		128		// Size of location-info buffer in #WCHARs
#define	DTU2XX_VPD_BUF_SIZE		512		// Size of VPD buffer
#define DTU2XX_I2C_BUF_SIZE		512		// Size of I2C transfer buffer
#define DTU2XX_VPD_ITEM_MAX_LENGTH	63		// 64 if trailing '\0' is factored in

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xx Structs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU2XX_VENDOR_REQUEST_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Vendor command request setup structure
//
typedef struct _DTU2XX_VENDOR_REQUEST_INFO
{
	UInt8		m_Request;		// Request code
	UInt16		m_ReqValue;		// Request value (for simple requests)
    UInt8		m_Direction;	// Transfer direction (DTU2XX_DIRECTION_WRITE / *_READ)
	UInt		m_BufSize;		// Size of data buffer
	UInt8*		m_pBuf;			// Data buffer
	UInt		m_BytesTranf;	// Number of bytes transferred from or to buffer
} DTU2XX_VENDOR_REQUEST_INFO, *PDTU2XX_VENDOR_REQUEST_INFO;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Device-Driver Versioning Info. -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Driver version information
//
typedef struct _DTU2XX_DRIVER_VERSION {
	Int		m_VersionMajor;
	Int		m_VersionMinor;
	Int		m_VersionMicro;
	Int		m_VersionBuild;
} DTU2XX_DRIVER_VERSION;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Flags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTU2XX_FLAGS {
	Int		m_Status;						// Flags
	Int		m_Latched;						// Latched flags (to be cleared)
	Int		m_ChannelType;					// Channel type: DTU2XX_TS_TX_CHANNEL/...
	Int		m_PortIndex;					// Zero-based port index (0=first port, ...)
} DTU2XX_FLAGS;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Exclusive-Access Info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTU2XX_EXCL_ACCESS_INFO {
	Int		m_Request;						// 0 = Request exclusive access
											// 1 = Release exclusive access
	Int		m_Granted;						// 0 = In use
											// 1 = Exclusive access granted
	Int		m_ChannelType;					// Channel type: DTU2XX_TS_TX_CHANNEL/...
	Int		m_PortIndex;					// Zero-based port index (0=first port, ...)
} DTU2XX_EXCL_ACCESS_INFO, *PDTU2XX_EXCL_ACCESS_INFO;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- I2C Exclusive-Access Info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTU2XX_I2C_EXCL_ACCESS_INFO {
	Int		m_Request;						// 0 = Request exclusive access
											// 1 = Release exclusive access
	Int		m_Granted;						// 0 = In use
											// 1 = Exclusive access granted
} DTU2XX_I2C_EXCL_ACCESS_INFO, *PDTU2XX_I2C_EXCL_ACCESS_INFO;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Location Info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTU2XX_GET_LOC_INFO {
	Int		m_DeviceId;						// Device ID
	Int		m_VendorId;						// Vendor ID
	Int		m_SubSystemId;					// Subsystem ID
	Int		m_SubVendorId;					// Subsystem Vendor ID
	Int		m_UsbAddress;					// USB address
#if defined(WIN32) || defined(WIN64)
	wchar_t m_LocBuf[DTU2XX_LOC_BUF_SIZE];	// Location information in string format.
#endif
} DTU2XX_LOC_INFO;

//-.-.-.-.-.-.-.-.-.-.-.- Vital Product Data for VPD read or write -.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTU2XX_VPD_TRANSFER {
	UInt  m_Start;					// Start BYTE address
	UInt  m_Length;					// Number of BYTES
	UInt8  m_VpdBuf[DTU2XX_VPD_BUF_SIZE];
									// The Vital product data itself
} DTU2XX_VPD_TRANSFER;

typedef struct _DTU2XX_VPD_ITEM {
	UInt  m_VpdSectionType;			// VPDSECT_ID, VPDSECT_RO, VPDSECT_RW
	char  m_VpdKeyword[2];			// Not used for VPD_ID
	UInt  m_Length;					// Length of buffer / Number of bytes in buffer
	UInt  m_Flag;					// Extra flags:
	UInt8  m_VpdBuf[DTU2XX_VPD_ITEM_MAX_LENGTH+1];
									// The Vital product data itself
} DTU2XX_VPD_ITEM;

typedef struct _DTU2XX_VPD_EEPROM {
	UInt  m_Start;					// Start BYTE address. 0 = start of VPDID
	UInt  m_Length;					// Number of BYTES
	UInt8  m_VpdBuf[DTU2XX_VPD_BUF_SIZE];
									// The Vital product data itself
} DTU2XX_VPD_EEPROM;

typedef struct _DTU2XX_VPD_DATA {
	UInt  m_Command;				// VPD_CMD_...
	union {
		DTU2XX_VPD_ITEM  m_VpdItem;
		DTU2XX_VPD_EEPROM  m_VpdEeprom;
	} m_Data;
} DTU2XX_VPD_DATA;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Transmit Mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// NOTE: only used with Transport-Stream transmit channel => no ChannelType
//
typedef struct _DTU2XX_TX_MODE {
	Int		m_TxPacketMode;					// Packet transmit mode
	Int		m_TxStuffMode;					// Null-packet stuffing on/off
} DTU2XX_TX_MODE;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU2XX_RX_STATUS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTU2XX_RX_STATUS {
	Int  m_PacketSize;		// Size of incoming transport packets: DTAPI_PCKSIZE_XXX
	Int  m_NumInv;			// Number of invalid bytes per packet: DTAPI_NUMINV_XXX
	Int  m_ClkDet;			// Receive-clock signal detected: DTAPI_CLKDET_XXX
	Int  m_AsiLock;			// DVB/ASI PLL locked: DTAPI_ASILOCK_XXX
	Int  m_RateOk;			// Input rate above minimum
	Int  m_AsiInv;			// DVB-ASI inversion status
} DTU2XX_RX_STATUS;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU2XX_RST_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTU2XX_RST_INFO {
	UInt	m_RstMode;						// ResetMode
} DTU2XX_RST_INFO;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU2XX_REG_CTRL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTU2XX_REG_INFO
{
	UInt	m_Address;						// Address of register
	UInt	m_Value;						// Value of register
} DTU2XX_REG_INFO;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU2XX_DVC_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTU2XX_DVC_INFO
{
	Int		m_TypeNumber;					// Device type number
	Int		m_FwVersion;					// Device firmware version
} DTU2XX_DVC_INFO;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU2XX_EEPROM_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTU2XX_EEPROM_CONTROL {
	Int		m_Code;							// EEPROM control code
} DTU2XX_EEPROM_CONTROL;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU2XX_I2C_TRANSFER -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTU2XX_I2C_TRANSFER
{
	UInt	m_DeviceAddr;					// I2C device address (7+1 bit, LSB is
											// ignored)
	UInt	m_Length;						// Number of BYTES
	UInt8	m_DataBuf[DTU2XX_I2C_BUF_SIZE];	// The Vital product data itself
} DTU2XX_I2C_TRANSFER;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- I/O Configuration -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DTU2XX_IOCONFIG {
	Int  m_ParXtra;					// Extra IO config option
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	Int  m_IoConfig;				// DTU2XX_IOCONFIG_XXX constants
} DTU2XX_IOCONFIG;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Modulation Control -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// NOTE: only used with Transport-Stream transmit channel => no ChannelType
//
typedef struct _DTU2XX_MOD_CONTROL {
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	Int  m_ModType;					// Modulation type
	Int  m_ParXtra0;				// Extra parameter #0
	Int  m_ParXtra1;				// Extra parameter #1
	Int  m_ParXtra2;				// Extra parameter #2
} DTU2XX_MOD_CONTROL;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- uCode state -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTU2XX_UCODE_UPLOAD_STATE_INFO {
	Int m_RequestUpload;
	Int m_Uploaded;
	Int m_Granted;
	Int m_State;
} DTU2XX_UCODE_UPLOAD_STATE_INFO;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Read/Write Info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DTU2XX_RW_INFO {
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
#if !defined(WIN32) && !defined(WIN64)
	// Linux specific members
	UInt8*  m_pBuffer;				// Pointer to user buffer
	Int  m_NumBytes2Transfer;		// Number of bytes to transfer
#endif
} DTU2XX_RW_INFO;

#if defined(CONFIG_X86_64)
//-.-.-.-.-.-.-.- Read/Write Info (for 32-bit application on 64-bit Linux) -.-.-.-.-.-.-.-
typedef struct _DTU2XX_RW_INFO_32 {
	Int  m_PortIndex;				// Zero-based port index (0=first port, ...)
	UInt32  m_pBuffer;				// Pointer to user buffer
	Int  m_NumBytes2Transfer;		// Number of bytes to transfer
} DTU2XX_RW_INFO_32;
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Union of all data structures -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef union _DTU2XX_IOCTL_DATA {
	DTU2XX_DRIVER_VERSION			m_DriverVersion;	// Versioning info
	DTU2XX_EXCL_ACCESS_INFO			m_ExclAccess;		// Exclusive-access info
	DTU2XX_I2C_EXCL_ACCESS_INFO		m_I2cExclAccess;	// I2C Exclusive-access info
	DTU2XX_UCODE_UPLOAD_STATE_INFO	m_UCodeUploadState; // uCode upload state info
	DTU2XX_IOCONFIG					m_IoConfig;			// I/O-Configuration info
	DTU2XX_LOC_INFO					m_LocInfo;			// Location info
	DTU2XX_RST_INFO					m_RstInfo;			// Reset flags
	DTU2XX_EEPROM_CONTROL			m_EepromControl;	// Special EEPROM-control operation
	DTU2XX_FLAGS					m_Flags;			// Status/lached flags
	DTU2XX_TX_MODE					m_TxMode;			// Transmit-Mode info
	DTU2XX_VPD_TRANSFER				m_Vpd;				// VPD transfer data
	DTU2XX_VPD_DATA					m_VpdData;			// VPD read/write/delete item / raw data
	DTU2XX_I2C_TRANSFER				m_I2CData;			// I2C transfer data
	DTU2XX_REG_INFO					m_RegInfo;			// For read or write register
	DTU2XX_DVC_INFO					m_DevInfo;			// Device information
	DTU2XX_RX_STATUS				m_RxStatusInfo;		// Receive status information
	DTU2XX_RW_INFO					m_RwInfo;			// Read/write info
	DTU2XX_MOD_CONTROL				m_ModCtrl;			// Modulation control
#if defined(CONFIG_X86_64)
	DTU2XX_RW_INFO_32				m_RwInfo_32;		// Read/write info (for 32-bit apps)
#endif
	UInt							m_Value;			// Simple type value
	UInt64							m_Value64b;			// 64-bit simple type value
	void*							m_pUserVirtual;		// Pointer to virtual address of
														// user buffer
} DTU2XX_IOCTL_DATA, *PDTU2XX_IOCTL_DATA;

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IOCTL +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Constants
#if defined(WIN32) || defined(WIN64)
	// Windows specific defines
	#define	DTU2XX_DEVICE_TYPE	45200	// DeviceType for DeviceIoControl operations
	#define	DTU2XX_IOCTL_BASE	3500	// Offset for Function
#else
	// Linux specific defines
	#define	DTU2XX_IOCTL_MAGIC	0xBD	// Magic number
	#define	DTU2XX_IOCTL_BASE	0x40	// Offset for Function
#endif

// Function Codes
enum {
	DTU2XX_FUNCTION_GET_DRIVER_VERSION = DTU2XX_IOCTL_BASE,
	DTU2XX_FUNCTION_RESET_SOFT,
	DTU2XX_FUNCTION_READ_VPD,
	DTU2XX_FUNCTION_WRITE_VPD,
	DTU2XX_FUNCTION_REQUEST_EXCL_ACCESS,
	DTU2XX_FUNCTION_GET_LOC_INFO,
	DTU2XX_FUNCTION_LED_CONTROL,
	DTU2XX_FUNCTION_READ_REGISTER,
	DTU2XX_FUNCTION_WRITE_REGISTER,
	DTU2XX_FUNCTION_GET_DEVICE_INFO,

	DTU2XX_RX_FUNCTION_LED_CONTROL,
	DTU2XX_RX_FUNCTION_EQUALISE,
    DTU2XX_RX_FUNCTION_GET_FIFO_LOAD,
	DTU2XX_RX_FUNCTION_GET_FLAGS,
	DTU2XX_RX_FUNCTION_CLEAR_FLAGS,
	DTU2XX_RX_FUNCTION_GET_STATUS,
	DTU2XX_RX_FUNCTION_GET_TS_RATE_BPS,
	DTU2XX_RX_FUNCTION_RESET,
	DTU2XX_RX_FUNCTION_SET_LOOPBACK_MODE,
	DTU2XX_RX_FUNCTION_SET_RX_CONTROL,
	DTU2XX_RX_FUNCTION_GET_RX_CONTROL,
	DTU2XX_RX_FUNCTION_SET_RX_MODE,
	DTU2XX_RX_FUNCTION_GET_RX_MODE,
	DTU2XX_RX_FUNCTION_GET_VIOL_COUNT,
	DTU2XX_RX_FUNCTION_BULK_USER_READ,

	DTU2XX_TX_FUNCTION_CLEAR_FLAGS,
	DTU2XX_TX_FUNCTION_GET_FIFO_LOAD,
	DTU2XX_TX_FUNCTION_GET_FLAGS,
	DTU2XX_TX_FUNCTION_GET_TS_RATE_BPS,
	DTU2XX_TX_FUNCTION_GET_TX_CONTROL,
	DTU2XX_TX_FUNCTION_GET_TX_MODE,
    DTU2XX_TX_FUNCTION_RD_LOOPBACK_DATA,
	DTU2XX_TX_FUNCTION_RESET,
	DTU2XX_TX_FUNCTION_SET_TX_POLARITY,
	DTU2XX_TX_FUNCTION_SET_FIFO_SIZE,
	DTU2XX_TX_FUNCTION_SET_LOOPBACK_MODE,
	DTU2XX_TX_FUNCTION_SET_TS_RATE_BPS,
	DTU2XX_TX_FUNCTION_SET_TX_CONTROL,
	DTU2XX_TX_FUNCTION_SET_TX_MODE,
	DTU2XX_TX_FUNCTION_BULK_USER_WRITE,

	//.-.-.-.-.-.- New functions add to end for backward compatibilty reasons -.-.-.-.-.-.
	DTU2XX_FUNCTION_I2C_WRITE,
	DTU2XX_FUNCTION_I2C_READ,
	DTU2XX_FUNCTION_GET_USB_SPEED,
	DTU2XX_FUNCTION_EEPROM_CONTROL,
	DTU2XX_RX_FUNCTION_SET_RX_POLARITY,
	DTU2XX_RX_FUNCTION_GET_MAX_FIFO_SIZE,
	DTU2XX_TX_FUNCTION_GET_MAX_FIFO_SIZE,
	DTU2XX_TX_FUNCTION_GET_FIFO_SIZE,
	DTU2XX_FUNCTION_UPLOAD_FIRMWARE,
	DTU2XX_FUNCTION_GET_IOCONFIG,
	DTU2XX_FUNCTION_SET_IOCONFIG,
	DTU2XX_FUNCTION_VPD_DATA,
	DTU2XX_TX_FUNCTION_SET_RF_MODE,
	DTU2XX_TX_FUNCTION_GET_MOD_CONTROL,
	DTU2XX_TX_FUNCTION_SET_MOD_CONTROL,
	DTU2XX_TX_FUNCTION_GET_SYMSAMP_RATE,
	DTU2XX_TX_FUNCTION_SET_SYMSAMP_RATE,
	DTU2XX_TX_FUNCTION_GET_RF_CONTROL,
	DTU2XX_TX_FUNCTION_SET_RF_CONTROL,
	DTU2XX_TX_FUNCTION_SET_OUTPUT_LEVEL,
	DTU2XX_FUNCTION_I2C_REQ_EXCL_ACCESS,
	DTU2XX_FUNCTION_UCODE_UPLOAD_STATE
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_GET_DRIVER_VERSION -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : Unused
// OutBuffer: DTU2XX_DRIVER_VERSION
// Result   : BOOL
//
// Get device-driver version.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_GET_DRIVER_VERSION										\
			CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_FUNCTION_GET_DRIVER_VERSION,	\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_GET_DRIVER_VERSION										\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_GET_DRIVER_VERSION,		\
				 DTU2XX_DRIVER_VERSION)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_GET_LOC_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : Unused
// OutBuffer: DTU2XX_LOC_INFO
// Result   : BOOL
//
// Get location info (Usb Address, PnP location string) and (sub-system)
// device ID and vendor ID.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_GET_LOC_INFO											\
			CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_FUNCTION_GET_LOC_INFO,			\
					 METHOD_BUFFERED,  FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_GET_LOC_INFO											\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_GET_LOC_INFO,				\
				 DTU2XX_LOC_INFO)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_LED_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : UInt (led control value)
// OutBuffer: -
// Result   : BOOL
//
// Control the status of device LED
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_LED_CONTROL											\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_FUNCTION_LED_CONTROL,				\
					METHOD_BUFFERED,  FILE_WRITE_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_LED_CONTROL											\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_LED_CONTROL, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_READ_REGISTER -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTU2XX_REG_INFO
// OutBuffer: DTU2XX_REG_INFO
// Result   : BOOL
//
// Read a regsiter in the onboard ALTERA
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_READ_REGISTER											\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_FUNCTION_READ_REGISTER,				\
					 METHOD_BUFFERED,  FILE_READ_DATA | FILE_WRITE_DATA)
#else
	// Linux
	#define IOCTL_DTU2XX_READ_REGISTER											\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_READ_REGISTER,			\
				  DTU2XX_REG_INFO)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_WRITE_REGISTER -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTU2XX_REG_INFO
// OutBuffer: Unsued
// Result   : BOOL
//
// Write to register in the onboard ALTERA
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_WRITE_REGISTER										\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_FUNCTION_WRITE_REGISTER,		\
					 METHOD_BUFFERED,  FILE_WRITE_DATA )
#else
	// Linux
	#define IOCTL_DTU2XX_WRITE_REGISTER										\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_WRITE_REGISTER,		\
				 DTU2XX_REG_INFO)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_WRITE_REGISTER -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : -
// OutBuffer: DTU2XX_DVC_INFO
// Result   : BOOL
//
// Get the device information (e.g. type number and firmware version)
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_GET_DEVICE_INFO										\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_FUNCTION_GET_DEVICE_INFO,			\
					 METHOD_BUFFERED, FILE_READ_DATA )
#else
	// Linux
	#define IOCTL_DTU2XX_GET_DEVICE_INFO										\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_GET_DEVICE_INFO,			\
				 DTU2XX_DVC_INFO)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_REQUEST_EXCL_ACCESS -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTU2XX_EXCL_ACCESS_INFO
// OutBuffer: DTU2XX_EXCL_ACCESS_INFO
// Result   : BOOL
//
// Requests exclusive access to a transmit or receive channel
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_REQUEST_EXCL_ACCESS									\
			CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_FUNCTION_REQUEST_EXCL_ACCESS,	\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTU2XX_REQUEST_EXCL_ACCESS									\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_REQUEST_EXCL_ACCESS,		\
				  DTU2XX_EXCL_ACCESS_INFO)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_READ_VPD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTU2XX_VPD_TRANSFER
// OutBuffer: DTU2XX_VPD_TRANSFER
// Result   : BOOL
//
// Read specified range of VPD data BYTES.
// DTU2XX_VPD_TRANSFER.m_Vpd[0] will contain the byte read from EEPROM address m_Start.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_READ_VPD												\
			CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_FUNCTION_READ_VPD,				\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTU2XX_READ_VPD												\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_READ_VPD,					\
				  DTU2XX_VPD_TRANSFER)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_WRITE_VPD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTU2XX_VPD_TRANSFER
// OutBuffer: -
// Result   : BOOL
//
// Write specified range of VPD data BYTES
// DTU2XX_VPD_TRANSFER.m_Vpd[0] shall contain the byte to be written to EEPROM address
// m_Start.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_WRITE_VPD												\
			CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_FUNCTION_WRITE_VPD,				\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTU2XX_WRITE_VPD												\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_WRITE_VPD,				\
				  DTU2XX_VPD_TRANSFER)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_VPD_DATA -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTU2XX_VPD_DATA
// OutBuffer: DTU2XX_VPD_DATA
// Result   : BOOL
//
// Read/Write/Delete items or RAW data from/to VPD
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_VPD_DATA												\
			CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_FUNCTION_VPD_DATA,				\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTU2XX_VPD_DATA												\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_VPD_DATA,					\
				  DTU2XX_VPD_DATA)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RESET_SOFT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : Unused
// OutBuffer: Unused
// Result   : BOOL
//
// Issue a software reset of the device
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RESET_SOFT												\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_FUNCTION_RESET_SOFT,			\
					METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RESET_SOFT												\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_RESET_SOFT, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_I2C_WRITE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTU2XX_I2C_TRANSFER
// OutBuffer: DTU2XX_I2C_TRANSFER
// Result   : BOOL
//
// Write data to the onboard I2C bus
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_I2C_WRITE												\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_FUNCTION_I2C_WRITE,			\
					METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTU2XX_I2C_WRITE												\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_I2C_WRITE,				\
				  DTU2XX_I2C_TRANSFER)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_I2C_READ -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTU2XX_I2C_TRANSFER
// OutBuffer: -
// Result   : BOOL
//
// Read data from the onboard I2C bus
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_I2C_READ												\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_FUNCTION_I2C_READ,			\
					METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTU2XX_I2C_READ												\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_I2C_READ,					\
				  DTU2XX_I2C_TRANSFER)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_GET_USB_SPEED -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : Unused
// OutBuffer: UInt (USB speed e.g. DTU2XX_USB_FULL_SPEED or DTU2XX_USB_HIGH_SPEED)
// Result   : BOOL
//
// Get the speed of the USB bus
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_GET_USB_SPEED												\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_FUNCTION_GET_USB_SPEED,			\
					METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTU2XX_GET_USB_SPEED												\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_GET_USB_SPEED, UInt)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_UPLOAD_FIRMWARE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
//
// InBuffer : Unused
// OutBuffer: UInt (ID of firmware to upload)
// Result   : BOOL
//
// Command to initiate upload of firmware
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_UPLOAD_FIRMWARE											\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_FUNCTION_UPLOAD_FIRMWARE,		\
					METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_UPLOAD_FIRMWARE											\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_UPLOAD_FIRMWARE, UInt)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_EEPROM_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTU2XX_EEPROM_CONTROL
// OutBuffer: -
// Result   : BOOL
//
// Special EEPROM control function
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_EEPROM_CONTROL											\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_FUNCTION_EEPROM_CONTROL,		\
					METHOD_BUFFERED, FILE_WRITE_DATA)
#else
	// Linux
	#define IOCTL_DTU2XX_EEPROM_CONTROL											\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_EEPROM_CONTROL,			\
				  DTU2XX_EEPROM_CONTROL)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_GET_IOCONFIG -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTU2XX_IOCONFIG
// OutBuffer: DTU2XX_IOCONFIG
// Result   : BOOL
//
// Get the configuration of an I/O port
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_GET_IOCONFIG											\
			CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_FUNCTION_GET_IOCONFIG,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTU2XX_GET_IOCONFIG											\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_GET_IOCONFIG,				\
				  DTU2XX_IOCONFIG)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_SET_IOCONFIG -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTU2XX_IOCONFIG
// OutBuffer: DTU2XX_IOCONFIG
// Result   : BOOL
//
// Configures an I/O port
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_SET_IOCONFIG											\
			CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_FUNCTION_SET_IOCONFIG,			\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTU2XX_SET_IOCONFIG											\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_SET_IOCONFIG,				\
				  DTU2XX_IOCONFIG)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_LED_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : UInt (led control value e.g. DTU2XX_LED_GREEN, DTU2XX_LED_RED, ...)
// OutBuffer: -
// Result   : BOOL
//
// Take control of the receive-status led
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_LED_CONTROL											\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_RX_FUNCTION_LED_CONTROL,			\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_LED_CONTROL											\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_LED_CONTROL, UInt)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_EQUALISE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : UInt (Enable value)
// OutBuffer: -
// Result   : BOOL
//
// Enable disbale the on-board equaliser
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_EQUALISE												\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_RX_FUNCTION_EQUALISE,					\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_EQUALISE											\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_EQUALISE, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_GET_FIFO_LOAD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : -
// OutBuffer: UInt (the load)
// Result   : BOOL
//
// Get the load of the SDRAM fifo
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_GET_FIFO_LOAD											\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_RX_FUNCTION_GET_FIFO_LOAD,				\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_GET_FIFO_LOAD											\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_GET_FIFO_LOAD, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_GET_FLAGS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTU2XX_FLAGS
// OutBuffer: DTU2XX_FLAGS
// Result   : BOOL
//
// Get current value of status flags and latched status flags.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_GET_FLAGS											\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_RX_FUNCTION_GET_FLAGS,				\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_GET_FLAGS											\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_GET_FLAGS,				\
				  DTU2XX_FLAGS)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_CLEAR_FLAGS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTU2XX_FLAGS
// OutBuffer: Unused
// Result   : BOOL
//
// Clear the specified latched status flags
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_CLEAR_FLAGS											\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_RX_FUNCTION_CLEAR_FLAGS,			\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_CLEAR_FLAGS											\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_CLEAR_FLAGS,			\
				  DTU2XX_FLAGS)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_GET_STATUS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : -
// OutBuffer: DTU2XX_RX_STATUS
// Result   : BOOL
//
// Get the input status information
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_GET_STATUS											\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_RX_FUNCTION_GET_STATUS,				\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_GET_STATUS											\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_GET_STATUS,			\
				  DTU2XX_RX_STATUS)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_GET_TS_RATE_BPS -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : -
// OutBuffer: UInt (the bit-rate)
// Result   : BOOL
//
// Get the measured TS bit-rate
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_GET_TS_RATE_BPS											\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_RX_FUNCTION_GET_TS_RATE_BPS,			\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_GET_TS_RATE_BPS											\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_GET_TS_RATE_BPS, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_RESET -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTU2XX_RST_INFO
// OutBuffer: Unused
// Result   : BOOL
//
// Resets the Rx channel
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_RESET												\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_RX_FUNCTION_RESET,					\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_RESET												\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_RESET,					\
				  DTU2XX_RST_INFO)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_SET_RX_POLARITY -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : UInt (Polarity mode)
// OutBuffer: Unused
// Result   : BOOL
//
// Sets the receive-polarity
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_SET_RX_POLARITY										\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_RX_FUNCTION_SET_RX_POLARITY,	\
						 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_SET_RX_POLARITY										\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_SET_RX_POLARITY, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_SET_LOOPBACK_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : UInt (mode)
// OutBuffer: -
// Result   : BOOL
//
// Set the loopback mode
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_SET_LOOPBACK_MODE									\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_RX_FUNCTION_SET_LOOPBACK_MODE,		\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_SET_LOOPBACK_MODE									\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_SET_LOOPBACK_MODE,		\
				 UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_SET_RX_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : UInt (Receive-control value DTU2XX_RXCTRL_IDLE, DTU2XX_RXCTRL_RCV)
// OutBuffer: -
// Result   : BOOL
//
// Set the receive control value
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_SET_RX_CONTROL										\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_RX_FUNCTION_SET_RX_CONTROL,			\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_SET_RX_CONTROL										\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_SET_RX_CONTROL,			\
				 UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_GET_RX_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : -
// OutBuffer: UInt (Receive-control value DTU2XX_RXCTRL_IDLE, DTU2XX_RXCTRL_RCV)
// Result   : BOOL
//
// Get the receive control value
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_GET_RX_CONTROL										\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_RX_FUNCTION_GET_RX_CONTROL,			\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_GET_RX_CONTROL										\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_GET_RX_CONTROL,			\
				 UInt)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_SET_RX_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : UInt (receive-mode DTU2XX_RXMODE_ST188, DTU2XX_RXMODE_ST204, ... )
// OutBuffer: Unused
// Result   : BOOL
//
// Set the channel's receive-mode state.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_SET_RX_MODE											\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_RX_FUNCTION_SET_RX_MODE,			\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_SET_RX_MODE											\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_SET_RX_MODE, UInt)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_GET_RX_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : Unused
// OutBuffer: UInt (receive-mode DTU2XX_RXMODE_ST188, DTU2XX_RXMODE_ST204, ... )
// Result   : BOOL
//
// Get the channel's receive-mode state.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_GET_RX_MODE											\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_RX_FUNCTION_GET_RX_MODE,			\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_GET_RX_MODE											\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_GET_RX_MODE, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_GET_VIOL_COUNT -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : Unused
// OutBuffer: UInt (violation count)
// Result   : BOOL
//
// Get the channel's violation count value
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_GET_VIOL_COUNT										\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_RX_FUNCTION_GET_VIOL_COUNT,			\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_GET_VIOL_COUNT										\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_GET_VIOL_COUNT, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_GET_MAX_FIFO_SIZE -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : Unused
// OutBuffer: UInt (SDRAM Size)
// Result   : BOOL
//
// Get the channel's FIFO size
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_GET_MAX_FIFO_SIZE									\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_RX_FUNCTION_GET_MAX_FIFO_SIZE,\
						 METHOD_BUFFERED, FILE_WRITE_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_GET_MAX_FIFO_SIZE									\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_GET_MAX_FIFO_SIZE, UInt)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_RX_BULK_USERMEM_READ -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : Data buffer with data to be DMA-ed.
// OutBuffer: -
// Result   : BOOL
//
// Read data (using DMA) in an application-supplied buffer in User-Space memory.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_RX_BULK_USERMEM_READ									\
		CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_RX_FUNCTION_BULK_USER_READ,			\
					 METHOD_IN_DIRECT, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_RX_BULK_USERMEM_READ									\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_BULK_USER_READ,		\
				  DTU2XX_RW_INFO)

	#if defined(CONFIG_X86_64)
	
	// For 32-bit application on 64-bit Linux
	#define IOCTL_DTU2XX_RX_BULK_USERMEM_READ_32								\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_RX_FUNCTION_BULK_USER_READ,		\
				  DTU2XX_RW_INFO_32)
	#endif

#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_CLEAR_FLAGS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTU2XX_FLAGS
// OutBuffer: Unused
// Result   : BOOL
//
// Clear the specified latched status flags
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_CLEAR_FLAGS											\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_CLEAR_FLAGS,		\
					METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_CLEAR_FLAGS											\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_CLEAR_FLAGS,			\
				  DTU2XX_FLAGS)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_GET_FIFO_LOAD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : Unused
// OutBuffer: UInt
// Result   : BOOL
//
// Get the HW fifo load
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_GET_FIFO_LOAD										\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_GET_FIFO_LOAD,	\
						 METHOD_BUFFERED, FILE_WRITE_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_GET_FIFO_LOAD										\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_GET_FIFO_LOAD, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_GET_FLAGS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTU2XX_FLAGS
// OutBuffer: DTU2XX_FLAGS
// Result   : BOOL
//
// Get current value of status flags and latched status flags.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_GET_FLAGS											\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_GET_FLAGS,		\
						 METHOD_BUFFERED, FILE_WRITE_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_GET_FLAGS											\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_GET_FLAGS,				\
				  DTU2XX_FLAGS)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_GET_MOD_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : -
// OutBuffer: DTU2XX_MOD_CONTROL
// Result   : BOOL
//
// Get modulation parameters.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_GET_MOD_CONTROL										\
			CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_TX_FUNCTION_GET_MOD_CONTROL,	\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_GET_MOD_CONTROL										\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_GET_MOD_CONTROL,		\
				  DTU2XX_MOD_CONTROL)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_GET_RF_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : UInt64
// OutBuffer: Unused
// Result   : BOOL
//
// Get the rf-control (carrier freq) setting
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_GET_RF_CONTROL										\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_GET_RF_CONTROL,	\
						 METHOD_BUFFERED, FILE_WRITE_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_GET_RF_CONTROL										\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_GET_RF_CONTROL, UInt64)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_GET_SYMSAMPL_RATE -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : UInt
// OutBuffer: Unused
// Result   : BOOL
//
// Gets the channels symbol or sample rate
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_GET_SYMSAMPL_RATE										\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_GET_SYMSAMP_RATE,	\
						 METHOD_BUFFERED, FILE_WRITE_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_GET_SYMSAMPL_RATE										\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_GET_SYMSAMP_RATE, UInt)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_GET_TS_RATE_BPS -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : Unused
// OutBuffer: UInt
// Result   : BOOL
//
// Gets the set transport-stream bit-rate
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_GET_TS_RATE_BPS										\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_GET_TS_RATE_BPS,	\
						 METHOD_BUFFERED, FILE_WRITE_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_GET_TS_RATE_BPS										\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_GET_TS_RATE_BPS, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_GET_TX_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : Unused
// OutBuffer: UInt
// Result   : BOOL
//
// Get the channel's transmit-control state.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_GET_TX_CONTROL										\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_GET_TX_CONTROL,	\
						 METHOD_BUFFERED, FILE_WRITE_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_GET_TX_CONTROL										\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_GET_TX_CONTROL, UInt)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_GET_TX_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : Unused
// OutBuffer: DTU2XX_TX_MODE
// Result   : BOOL
//
// Get the channel's transmit-mode state.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_GET_TX_MODE											\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_GET_TX_MODE,		\
						 METHOD_BUFFERED, FILE_WRITE_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_GET_TX_MODE											\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_GET_TX_MODE,			\
				 DTU2XX_TX_MODE)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_GET_MAX_FIFO_SIZE -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : Unused
// OutBuffer: UInt (SDRAM Size)
// Result   : BOOL
//
// Get the channel's max. FIFO size
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_GET_MAX_FIFO_SIZE									\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_GET_MAX_FIFO_SIZE,\
						 METHOD_BUFFERED, FILE_WRITE_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_GET_MAX_FIFO_SIZE									\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_GET_MAX_FIFO_SIZE, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_GET_FIFO_SIZE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : Unused
// OutBuffer: UInt (FIFO Size)
// Result   : BOOL
//
// Get the channel's current FIFO size
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_GET_FIFO_SIZE										\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_GET_FIFO_SIZE,	\
						 METHOD_BUFFERED, FILE_WRITE_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_GET_FIFO_SIZE										\
			_IOR(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_GET_FIFO_SIZE, UInt)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_RD_LOOPBACK_DATA -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Reads data from loop back register
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_RD_LOOPBACK_DATA									\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_RD_LOOPBACK_DATA,\
						 METHOD_BUFFERED, FILE_WRITE_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_RD_LOOPBACK_DATA									\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_RD_LOOPBACK_DATA, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_RESET -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTU2XX_RST_INFO
// OutBuffer: Unused
// Result   : BOOL
//
// Resets the Tx channel
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_RESET												\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_RESET,			\
						 METHOD_BUFFERED, FILE_READ_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_RESET												\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_RESET, DTU2XX_RST_INFO)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_SET_TX_POLARITY -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : UInt (Polarity mode)
// OutBuffer: Unused
// Result   : BOOL
//
// Sets the transmission polarity
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_SET_TX_POLARITY										\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_SET_TX_POLARITY,	\
						 METHOD_BUFFERED, FILE_READ_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_SET_TX_POLARITY										\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_SET_TX_POLARITY, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_SET_FIFO_SIZE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : UInt
// OutBuffer: Unused
// Result   : BOOL
//
// Sets the fifo size
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_SET_FIFO_SIZE										\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_SET_FIFO_SIZE,	\
						 METHOD_BUFFERED, FILE_READ_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_SET_FIFO_SIZE										\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_SET_FIFO_SIZE, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_SET_LOOPBACK_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : UInt
// OutBuffer: Unused
// Result   : BOOL
//
// Set the loopback mode
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_SET_LOOPBACK_MODE										\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_SET_LOOPBACK_MODE,	\
						 METHOD_BUFFERED, FILE_READ_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_SET_LOOPBACK_MODE										\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_SET_LOOPBACK_MODE, UInt)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_SET_MOD_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTU2XX_MOD_CONTROL
// OutBuffer: -
// Result   : BOOL
//
// Set modulation-control parameters
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_SET_MOD_CONTROL										\
			CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_TX_FUNCTION_SET_MOD_CONTROL,	\
					 METHOD_BUFFERED, FILE_READ_DATA)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_SET_MOD_CONTROL										\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_SET_MOD_CONTROL,		\
				 DTU2XX_MOD_CONTROL)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_SET_OUTPUT_LEVEL -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : UInt
// OutBuffer: Unused
// Result   : BOOL
//
// Set output level (in .1dBm steps)
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_SET_OUTPUT_LEVEL									\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_SET_OUTPUT_LEVEL,\
						 METHOD_BUFFERED, FILE_READ_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_SET_OUTPUT_LEVEL									\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_SET_OUTPUT_LEVEL, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_SET_RF_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : UInt64
// OutBuffer: Unused
// Result   : BOOL
//
// Set rf-control (carrier freq) setting
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_SET_RF_CONTROL										\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_SET_RF_CONTROL,	\
						 METHOD_BUFFERED, FILE_READ_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_SET_RF_CONTROL										\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_SET_RF_CONTROL, UInt64)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_SET_RF_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : UInt
// OutBuffer: Unused
// Result   : BOOL
//
// Set the loopback mode
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_SET_RF_MODE										\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_SET_RF_MODE,	\
						 METHOD_BUFFERED, FILE_READ_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_SET_RF_MODE										\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_SET_RF_MODE, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_SET_SYMSAMPL_RATE -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : UInt
// OutBuffer: Unused
// Result   : BOOL
//
// Sets the channels symbol or sample rate
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_SET_SYMSAMPL_RATE										\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_SET_SYMSAMP_RATE,	\
						 METHOD_BUFFERED, FILE_READ_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_SET_SYMSAMPL_RATE										\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_SET_SYMSAMP_RATE, UInt)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_SET_TS_RATE_BPS -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : UInt
// OutBuffer: Unused
// Result   : BOOL
//
// Sets the channels transport-stream bit-rate
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_SET_TS_RATE_BPS										\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_SET_TS_RATE_BPS,	\
						 METHOD_BUFFERED, FILE_READ_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_SET_TS_RATE_BPS										\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_SET_TS_RATE_BPS, UInt)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_SET_TX_CONTROL -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : UInt
// OutBuffer: Unused
// Result   : BOOL
//
// Sets the channels transmit-control state
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_SET_TX_CONTROL										\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_SET_TX_CONTROL,	\
						 METHOD_BUFFERED, FILE_READ_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_SET_TX_CONTROL										\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_SET_TX_CONTROL, UInt)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_SET_TX_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : DTU2XX_TX_MODE
// OutBuffer: Unused
// Result   : BOOL
//
// Sets the channels transmit-mode state
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_SET_TX_MODE											\
				CTL_CODE(DTU2XX_DEVICE_TYPE,DTU2XX_TX_FUNCTION_SET_TX_MODE,		\
						 METHOD_BUFFERED, FILE_READ_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_SET_TX_MODE											\
			_IOW(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_SET_TX_MODE,			\
				 DTU2XX_TX_MODE)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_TX_BULK_USERMEM_WRITE -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : -
// OutBuffer: Data buffer with data to be DMA-ed.
// Result   : BOOL
//
// Write data (using DMA) from an application-supplied buffer in User-Space memory.
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_TX_BULK_USERMEM_WRITE									\
			CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_TX_FUNCTION_BULK_USER_WRITE,	\
					 METHOD_OUT_DIRECT, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_TX_BULK_USERMEM_WRITE									\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_BULK_USER_WRITE,		\
				  DTU2XX_RW_INFO)

	#if defined(CONFIG_X86_64)
	// For 32-bit application on 64-bit Linux
	#define IOCTL_DTU2XX_TX_BULK_USERMEM_WRITE_32								\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_TX_FUNCTION_BULK_USER_WRITE,		\
				  DTU2XX_RW_INFO_32)
	#endif
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_I2C_REQUEST_EXCL_ACCESS -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : DTU2XX_I2C_EXCL_ACCESS_INFO
// OutBuffer: DTU2XX_I2C_EXCL_ACCESS_INFO
// Result   : BOOL
//
// Requests / releases exclusive access to a I2C port
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_I2C_REQUEST_EXCL_ACCESS								\
			CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_FUNCTION_I2C_REQ_EXCL_ACCESS,	\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_I2C_REQUEST_EXCL_ACCESS								\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_I2C_REQ_EXCL_ACCESS,		\
				  DTU2XX_EXCL_ACCESS_INFO)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTU2XX_UCODE_UPLOAD_STATE -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : IOCTL_DTU2XX_UCODE_UPLOAD_STATE_INFO
// OutBuffer: IOCTL_DTU2XX_UCODE_UPLOAD_STATE_INFO
// Result   : BOOL
//
// Requests / releases exclusive access to a I2C port
//
#if defined(WIN32) || defined(WIN64)
	// Windows
	#define IOCTL_DTU2XX_UCODE_UPLOAD_STATE								\
			CTL_CODE(DTU2XX_DEVICE_TYPE, DTU2XX_FUNCTION_UCODE_UPLOAD_STATE,	\
					 METHOD_BUFFERED, FILE_ANY_ACCESS)
#else
	// Linux
	#define IOCTL_DTU2XX_UCODE_UPLOAD_STATE								\
			_IOWR(DTU2XX_IOCTL_MAGIC, DTU2XX_FUNCTION_UCODE_UPLOAD_STATE,		\
				  DTU2XX_EXCL_ACCESS_INFO)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Helper Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Channel Definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTU2XX_GEN_REGS			-1		// Special value for GENERIC registers
#define  DTU2XX_TS_TX_CHANNEL		0		// Transport-Stream Transmit Channel
#define  DTU2XX_TS_RX_CHANNEL		1		// Transport-Stream Receive Channel
#define  DTU2XX_CHANNEL_UNUSED		99		// Channel not used

// Transfer direction
#define DTU2XX_DIRECTION_READ		0		// Read transfer (DTU-2XX=>driver)
#define DTU2XX_DIRECTION_WRITE		1		// Write transfer (DTU-2XX<=driver)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Reset Modes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTU2XX_RST_CLEARFIFO		0		// Clear FIFO without disrupting channel
#define  DTU2XX_RST_CHANNEL			1		// Channel reset

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- VPD Read/Write -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Write flags
#define DTU2XX_VPDFLAG_RO_WRITE_ALLOWED	1
#define DTU2XX_VPDFLAG_UPDATE_EEPROM	2

// Section types
#define DTU2XX_VPDSECT_ID				1
#define DTU2XX_VPDSECT_RO				2
#define	DTU2XX_VPDSECT_RW				4

// Commands for VPD read/write/delete
#define DTU2XX_VPDCMD_ITEM_READ			1
#define DTU2XX_VPDCMD_ITEM_WRITE		2
#define DTU2XX_VPDCMD_ITEM_DELETE		3
#define DTU2XX_VPDCMD_RAW_READ			4
#define DTU2XX_VPDCMD_RAW_WRITE			5

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- uCode upload states -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTU2XX_UCODE_NOT_LOADED	0
#define  DTU2XX_UCODE_LOADING		1
#define  DTU2XX_UCODE_LOADED		2

#endif	// #ifndef __DTU2X_COMMON_H
