//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPex811xCommon.h *#*#*#*#*#*#*# (C) 2012-2015 DekTec
//
// DekTec SDK - PEX-811x common constants
//
// The PEX-8111 and PEX-8112 are PCI Express brdige chips used on some DekTec cards to
// interface to the PCI Express bus.
//
// This file contains PEX-related constants that are shared between driver and DTAPI
//

#if defined(WIN32) || defined(WIN64)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Constants used in defining device IO control codes to the PEX-811x driver
#define DTPEX811X_DEVICE_TYPE       45100        // Device type for PEX-811x
#define DTPEX811X_IOCTL_BASE        3100         // Offset for PEX-811x functions

// PEX-811x device related constants
#define DTPEX811X_EEPROM_BUF_SIZE   512          // Size of EEPROM buffer
#define DTPEX811X_LOC_BUF_SIZE      128          // Size of location-info buffer in
                                                 // number of wchars

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Function Codes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// DON'T INSERT NEW VALUES BETWEEN EXISTING ONES, SO THAT NEW DRIVER VERSIONS REMAIN
// COMPATIBLE WITH USER APPLICATIONS THAT WERE COMPILED AGAINST AN OLD DRIVER!!

enum {
    DTPEX811X_FUNCTION_GET_DRIVER_VERSION = DTPEX811X_IOCTL_BASE,
    DTPEX811X_FUNCTION_GET_LOC_INFO,
    DTPEX811X_FUNCTION_READ_EEPROM,
    DTPEX811X_FUNCTION_WRITE_EEPROM
};

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IO Control Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTPEX811X_GET_DRIVER_VERSION -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : -
// OutBuffer: DTPEX811X_DRIVER_VERSION
// Result   : BOOL
//
// Get device-driver version
//
#define IOCTL_DTPEX811X_GET_DRIVER_VERSION                                              \
            CTL_CODE(DTPEX811X_DEVICE_TYPE, DTPEX811X_FUNCTION_GET_DRIVER_VERSION,      \
                     METHOD_BUFFERED, FILE_READ_DATA)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTPEX811X_GET_LOC_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : -
// OutBuffer: DTPEX811X_GET_LOC_INFO
// Result   : BOOL
//
// Get location info (PCI-bus number, slot number, PnP location string) and (sub-system)
// device ID and vendor ID
//
#define IOCTL_DTPEX811X_GET_LOC_INFO                                                    \
            CTL_CODE(DTPEX811X_DEVICE_TYPE, DTPEX811X_FUNCTION_GET_LOC_INFO,            \
                     METHOD_BUFFERED, FILE_READ_DATA)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTPEX811X_READ_EEPROM -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// InBuffer : UInt m_Start, UInt m_Length; size = 2*sizeof(UInt)
// OutBuffer: DTA1XX_EEPROM_TRANSFER
// Result   : BOOL
//
// Read specified range of EEPROM data BYTES.
// DTA1XX_EEPROM_TRANSFER.m_Eeprom[0] will contain the byte read from EEPROM
// address m_Start
//
#define IOCTL_DTPEX811X_READ_EEPROM                                                     \
            CTL_CODE(DTPEX811X_DEVICE_TYPE, DTPEX811X_FUNCTION_READ_EEPROM,             \
                     METHOD_BUFFERED, FILE_READ_DATA)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL_DTPEX811X_WRITE_EEPROM -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// InBuffer : UInt m_Start, UInt m_Length; size = 2*sizeof(UInt) + #bytes_to_write
// OutBuffer: -
// Result   : BOOL
//
// Write specified range of EEPROM data BYTES
// DTA1XX_TRANSFER.m_Eeprom[0] shall contain the byte to be written to EEPROM address
// m_Start
//
#define IOCTL_DTPEX811X_WRITE_EEPROM                                                    \
            CTL_CODE(DTPEX811X_DEVICE_TYPE, DTPEX811X_FUNCTION_WRITE_EEPROM,            \
                     METHOD_BUFFERED, FILE_READ_DATA)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTPEX811X_DRIVER_VERSION -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Device-driver versioning info
//
typedef struct _DTPEX811X_DRIVER_VERSION 
{
    Int  m_VersionMajor;
    Int  m_VersionMinor;
    Int  m_VersionMicro;
    Int  m_VersionBuild;
} DTPEX811X_DRIVER_VERSION;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTPEX811X_LOC_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Location info
//
typedef struct _DTPEX811X_GET_LOC_INFO 
{
    Int  m_DeviceIdBridge;          // Device ID
    Int  m_VendorIdBridge;          // Vendor ID
    Int  m_SubSystemIdBridge;       // Subsystem ID
    Int  m_SubVendorIdBridge;       // Subsystem Vendor ID
    Int  m_BusNumberBridge;         // PCI-bus number
    Int  m_SlotNumber;              // PCI-slot number
    Int  m_DeviceIdDevice;          // Device ID Bridge
    Int  m_VendorIdDevice;          // Vendor ID Bridge
    Int  m_SubSystemIdDevice;       // Subsystem ID
    Int  m_SubVendorIdDevice;       // Subsystem Vendor ID
    Int  m_BusNumberDevice;         // PCI-bus number Bridge

    // Location information in string format
    WCHAR  m_LocBuf[DTPEX811X_LOC_BUF_SIZE];
} DTPEX811X_LOC_INFO;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTPEX811X_EEPROM_TRANSFER -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DTPEX811X_EEPROM_TRANSFER 
{
    UInt  m_Start;                  // Start BYTE address
    UInt  m_Length;                 // Number of BYTES

    // The EEPROM data itself
    UInt8  m_EepromBuf[DTPEX811X_EEPROM_BUF_SIZE];
} DTPEX811X_EEPROM_TRANSFER;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTPEX811X_IOCTL_DATA -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Union of main all IOCTL data structures
//
typedef union _DTPEX811X_IOCTL_DATA 
{
    DTPEX811X_DRIVER_VERSION  m_DriverVersion;   // Driver version
    DTPEX811X_LOC_INFO  m_LocInfo;               // Location info
    DTPEX811X_EEPROM_TRANSFER  m_EepromTransfer; // Eeprom transfer
} DTPEX811X_IOCTL_DATA;

#endif // #if defined(WIN32) || defined(WIN64)
