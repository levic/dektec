//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaCommon.h *#*#*#*#*#*#*#*#* (C) 2010-2016 DekTec
//
// Dta driver - Common file shared between Dta driver and DTAPI
//
// This file describes the Dta driver interface, which is used by the DTAPI.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2016 DekTec Digital Video B.V.
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


#ifndef __DTA_COMMON_H
#define __DTA_COMMON_H

// Define Int64/UInt64 here to a non-existing identifier. Since all types in this header
// are used in a binary interface we have to make sure they are compatible between
// 32-bit and 64-bit builds. For this we have to align every 64-bit value to an 8-byte
// boundary. The Int64A / UInt64A types do this. Make sure Int64/UInt64 are not used.
#define Int64 ERROR_DO_NOT_USE_UNALIGNED_INT64
#define UInt64 ERROR_DO_NOT_USE_UNALIGNED_UINT64

#ifdef WINBUILD

// Driver / DTAPI include files
#ifdef DTAPI
    // Disable warning about the zero sized array extension
    #pragma warning(disable : 4200)
    
    #include <winioctl.h>
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Driver Device interface GUID -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// {9051D683-157F-44b7-9A22-6B013CCA676A}
static const GUID GUID_DEVINTERFACE_DTA = 
{ 0x9051d683, 0x157f, 0x44b7, { 0x9a, 0x22, 0x6b, 0x1, 0x3c, 0xca, 0x67, 0x6a } };

#else // WINBUILD

#ifdef DTAPI
    #include <sys/ioctl.h>
#endif

#endif

#pragma pack (push, 8)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IOCTL codes +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// On Windows:
// METHOD_OUT_DIRECT is used for 'normal' IOCTLS, METHOD_IN_DIRECT is not needed for
//                          'normal' IOCTLS because output buffers are never used as input
// METHOD_IN_DIRECT and METHOD_OUT_DIRECT are used for DMA's
// METHOD_IN_DIRECT is used for shared buffers. The output buffer can then be used as 
//                                                                        input and output
// METHOD_NEITHER is not used at all
//

// Constants
#ifdef WINBUILD
    // Windows specific defines
    #define DTA_DEVICE_TYPE  45100  // DeviceType for DeviceIoControl operations
    #define DTA_IOCTL_BASE   3100   // Offset for Function
#else
    // Linux specific defines
    #define DTA_IOCTL_MAGIC         0xBB   // Magic number
    #define DTA_IOCTL_MAGIC_SIZE    0xBC   // Magic number for Ioctls with a dynamic size
    #define DTA_IOCTL_BASE          0x50   // Offset for Function
#endif

// DTA functions
// DON'T INSERT NEW VALUES BETWEEN EXISTING ONES, SO THAT NEW DRIVER VERSIONS REMAIN
// COMPATIBLE WITH USER APPLICATIONS THAT WERE COMPILED AGAINST AN OLD DRIVER!!
enum {
    FUNC_DTA_GET_PROPERTY = DTA_IOCTL_BASE,
    FUNC_DTA_GET_DEV_INFO,
    FUNC_DTA_VPD_CMD,
    FUNC_DTA_GET_DRIVER_VERSION,
    FUNC_DTA_I2C_CMD,
    FUNC_DTA_GET_ADDRESS_REGS,
    FUNC_DTA_RELEASE_ADDRESS_REGS,
    FUNC_DTA_GET_EVENT,
    FUNC_DTA_REGISTER_EVENTS,
    FUNC_DTA_UNREGISTER_EVENTS,
    FUNC_DTA_DMA_READ,
    FUNC_DTA_DMA_WRITE,
    FUNC_DTA_SH_BUF_CMD,
    FUNC_DTA_GET_IOCONFIG,
    FUNC_DTA_SET_IOCONFIG,
    FUNC_DTA_NONIP_CMD,
    FUNC_DTA_NONIP_TX_CMD,
    FUNC_DTA_NONIP_RX_CMD,
    FUNC_DTA_IP_CMD,
    FUNC_DTA_IP_TX_CMD,
    FUNC_DTA_IP_RX_CMD,
    FUNC_DTA_PHYMAC_CMD,
    FUNC_DTA_IP_NW_CMD,
    FUNC_DTA_GET_TABLE,
    FUNC_DTA_REBOOT_FIRMWARE,
    FUNC_DTA_GET_NWDRIVER_VERSION,
    FUNC_DTA_GET_STR_PROPERTY,
    FUNC_DTA_MATRIX_CMD,
    FUNC_DTA_GET_DEV_SUBTYPE,
    FUNC_DTA_SET_MEMORY_TESTMODE,
    FUNC_DTA_GET_PROPERTY2,
    FUNC_DTA_GET_DEV_GENLOCKSTATE,
    FUNC_DTA_GET_DEV_INFO2,
    FUNC_DTA_GET_PROPERTY3,
    FUNC_DTA_GET_STR_PROPERTY2,
    FUNC_DTA_GET_TABLE2,
    FUNC_DTA_RS422_CMD,
    FUNC_DTA_GET_DEV_INFO3,
    FUNC_DTA_SET_VCXO,
    FUNC_DTA_SDIAVRX_CMD,
    FUNC_DTA_ENDEC_CMD,
    FUNC_DTA_D7PRO_CMD,
    FUNC_DTA_SPIMF_CMD,
    FUNC_DTA_GET_DEV_INFO4,
    FUNC_DTA_PROGITF_CMD,
    FUNC_DTA_MULTIMOD_CMD,
    FUNC_DTA_GET_PROPERTY4,
    FUNC_DTA_GET_STR_PROPERTY3,
    FUNC_DTA_GET_TABLE3,
    FUNC_DTA_HDMI_TX_CMD
};

// Ioctl input data type
typedef struct _DtaIoctlGetPropertyInput {
    Int  m_TypeNumber;                      // Type number (-1 = get for current device)
    Int  m_HardwareRevision;                // Hardware revision
    Int  m_FirmwareVersion;                 // Firmware version
    Int  m_PortIndex;                       // Port index
    char  m_Name[PROPERTY_NAME_MAX_SIZE];   // Name of property
} DtaIoctlGetPropertyInput;
ASSERT_SIZE(DtaIoctlGetPropertyInput, 68)

typedef struct _DtaIoctlGetProperty2Input {
    Int  m_TypeNumber;                      // Type number (-1 = get for current device)
    Int  m_HardwareRevision;                // Hardware revision
    Int  m_FirmwareVersion;                 // Firmware version
    Int  m_PortIndex;                       // Port index
    char  m_Name[PROPERTY_NAME_MAX_SIZE];   // Name of property
    Int  m_DtapiMaj;                        // DTAPI major version
    Int  m_DtapiMin;                        // DTAPI minor version
    Int  m_DtapiBugfix;                     // DTAPI bug fix version
} DtaIoctlGetProperty2Input;
ASSERT_SIZE(DtaIoctlGetProperty2Input, 80)

typedef struct _DtaIoctlGetProperty3Input {
    Int  m_TypeNumber;                      // Type number (-1 = get for current device)
    Int  m_SubDvc;                          // Sub-device (-1 = get for current device)
    Int  m_HardwareRevision;                // Hardware revision
    Int  m_FirmwareVersion;                 // Firmware version
    Int  m_PortIndex;                       // Port index
    char  m_Name[PROPERTY_NAME_MAX_SIZE];   // Name of property
    Int  m_DtapiMaj;                        // DTAPI major version
    Int  m_DtapiMin;                        // DTAPI minor version
    Int  m_DtapiBugfix;                     // DTAPI bug fix version
} DtaIoctlGetProperty3Input;
ASSERT_SIZE(DtaIoctlGetProperty3Input, 84)

typedef struct _DtaIoctlGetProperty4Input {
    Int  m_TypeNumber;                      // Type number (-1 = get for current device)
    Int  m_SubDvc;                          // Sub-device (-1 = get for current device)
    Int  m_HardwareRevision;                // Hardware revision
    Int  m_FirmwareVersion;                 // Firmware version
    Int  m_FirmwareVariant;                 // Firmware variant
    Int  m_PortIndex;                       // Port index
    char  m_Name[PROPERTY_NAME_MAX_SIZE];   // Name of property
    Int  m_DtapiMaj;                        // DTAPI major version
    Int  m_DtapiMin;                        // DTAPI minor version
    Int  m_DtapiBugfix;                     // DTAPI bug fix version
} DtaIoctlGetProperty4Input;
ASSERT_SIZE(DtaIoctlGetProperty4Input, 88)


// Ioctl output data type
// Don't use ENUMS in Ioctl's. Size is unknown
typedef struct _DtaIoctlGetPropertyOutput {
    Int  m_Scope;                           // Scope of property
    Int  m_Type;                            // Type of property
    UInt64A  m_Value;                       // Property value
} DtaIoctlGetPropertyOutput;
ASSERT_SIZE(DtaIoctlGetPropertyOutput, 16)

#ifdef WINBUILD
    #define DTA_IOCTL_GET_PROPERTY  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_GET_PROPERTY, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetPropertyData {
        DtaIoctlGetPropertyInput  m_Input;
        DtaIoctlGetPropertyOutput  m_Output;
    } DtaIoctlGetPropertyData;

    #define DTA_IOCTL_GET_PROPERTY  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_GET_PROPERTY, \
                                                                  DtaIoctlGetPropertyData)
#endif

#ifdef WINBUILD
    #define DTA_IOCTL_GET_PROPERTY2  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_GET_PROPERTY2, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetProperty2Data {
        DtaIoctlGetProperty2Input  m_Input;
        DtaIoctlGetPropertyOutput  m_Output;
    } DtaIoctlGetProperty2Data;

    #define DTA_IOCTL_GET_PROPERTY2  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_GET_PROPERTY2, \
                                                                 DtaIoctlGetProperty2Data)
#endif

#ifdef WINBUILD
    #define DTA_IOCTL_GET_PROPERTY3  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_GET_PROPERTY3, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetProperty3Data {
        DtaIoctlGetProperty3Input  m_Input;
        DtaIoctlGetPropertyOutput  m_Output;
    } DtaIoctlGetProperty3Data;

    #define DTA_IOCTL_GET_PROPERTY3  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_GET_PROPERTY3, \
                                                                 DtaIoctlGetProperty3Data)
#endif

#ifdef WINBUILD
    #define DTA_IOCTL_GET_PROPERTY4  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_GET_PROPERTY4, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetProperty4Data {
        DtaIoctlGetProperty4Input  m_Input;
        DtaIoctlGetPropertyOutput  m_Output;
    } DtaIoctlGetProperty4Data;

    #define DTA_IOCTL_GET_PROPERTY4  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_GET_PROPERTY4, \
                                                                 DtaIoctlGetProperty4Data)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_GET_DEV_INFO +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Gets the device information
//
typedef struct _DtaIoctlGetDevInfoOutput {
    UInt16  m_DeviceId;         // Device ID
    UInt16  m_VendorId;         // Vendor ID
    UInt16  m_SubVendorId;      // Subsystem Vendor ID
    UInt16  m_SubSystemId;      // Subsystem ID
    Int  m_BusNumber;           // PCI-bus number
    Int  m_SlotNumber;          // PCI-slot number
    Int  m_TypeNumber;          // Type number in decimal, e.g. 2160 for DTA-2160
    Int  m_HardwareRevision;    // Hardware Revision
    Int  m_FirmwareVersion;     // Firmware Version (= Altera revision), e.g. 3 for
                                // "Firmware Version 3"
    Int  m_FirmwareVariant;     // Firmware Variant, e.g. to distinguish between
                                // firmware with different #inputs/#outputs
    UInt64A  m_Serial;          // Serial number
} DtaIoctlGetDevInfoOutput;
ASSERT_SIZE(DtaIoctlGetDevInfoOutput, 40)

#ifdef WINBUILD
    #define DTA_IOCTL_GET_DEV_INFO  CTL_CODE(DTA_DEVICE_TYPE, \
                          FUNC_DTA_GET_DEV_INFO, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetDevInfoData {
        DtaIoctlGetDevInfoOutput  m_Output;
    } DtaIoctlGetDevInfoData;

    #define DTA_IOCTL_GET_DEV_INFO  _IOR(DTA_IOCTL_MAGIC, FUNC_DTA_GET_DEV_INFO, \
                                                             DtaIoctlGetDevInfoData)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_GET_DEV_INFO2 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Gets the device information
//
typedef struct _DtaIoctlGetDevInfoOutput2 {
    UInt16  m_DeviceId;         // Device ID
    UInt16  m_VendorId;         // Vendor ID
    UInt16  m_SubVendorId;      // Subsystem Vendor ID
    UInt16  m_SubSystemId;      // Subsystem ID
    Int  m_BusNumber;           // PCI-bus number
    Int  m_SlotNumber;          // PCI-slot number
    Int  m_TypeNumber;          // Type number in decimal, e.g. 2160 for DTA-2160
    Int  m_SubDvc;              // Sub-device: 0=master, 1=slave1, 2=slave2, etc
    Int  m_HardwareRevision;    // Hardware Revision
    Int  m_FirmwareVersion;     // Firmware Version (= Altera revision), e.g. 3 for
                                // "Firmware Version 3"
    Int  m_FirmwareVariant;     // Firmware Variant, e.g. to distinguish between
                                // firmware with different #inputs/#outputs
    UInt64A  m_Serial;          // Serial number
} DtaIoctlGetDevInfoOutput2;
ASSERT_SIZE(DtaIoctlGetDevInfoOutput2, 48)

#ifdef WINBUILD
    #define DTA_IOCTL_GET_DEV_INFO2  CTL_CODE(DTA_DEVICE_TYPE, \
                          FUNC_DTA_GET_DEV_INFO2, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetDevInfoData2 {
        DtaIoctlGetDevInfoOutput2  m_Output;
    } DtaIoctlGetDevInfoData2;

    #define DTA_IOCTL_GET_DEV_INFO2  _IOR(DTA_IOCTL_MAGIC, FUNC_DTA_GET_DEV_INFO2, \
                                                             DtaIoctlGetDevInfoData2)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_GET_DEV_INFO3 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Gets the device information
//
typedef struct _DtaIoctlGetDevInfoOutput3 {
    UInt16  m_DeviceId;         // Device ID
    UInt16  m_VendorId;         // Vendor ID
    UInt16  m_SubVendorId;      // Subsystem Vendor ID
    UInt16  m_SubSystemId;      // Subsystem ID
    Int  m_BusNumber;           // PCI-bus number
    Int  m_SlotNumber;          // PCI-slot number
    Int  m_TypeNumber;          // Type number in decimal, e.g. 2160 for DTA-2160
    Int  m_SubDvc;              // Sub-device: 0=master, 1=slave1, 2=slave2, etc
    Int  m_HardwareRevision;    // Hardware Revision
    Int  m_FirmwareVersion;     // Firmware Version (= Altera revision), e.g. 3 for
                                // "Firmware Version 3"
    Int  m_FirmwareVariant;     // Firmware Variant, e.g. to distinguish between
                                // firmware with different #inputs/#outputs
    UInt64A  m_Serial;          // Serial number
    Int  m_PcieNumLanes;        // Number of allocated PCIe lanes
    Int  m_PcieMaxLanes;        // Maximum number of PCIe lanes
    Int  m_PcieLinkSpeed;       // Current PCIe link speed
    Int  m_PcieMaxSpeed;        // Maximum PCIe link speed
} DtaIoctlGetDevInfoOutput3;
ASSERT_SIZE(DtaIoctlGetDevInfoOutput3, 64)

#ifdef WINBUILD
    #define DTA_IOCTL_GET_DEV_INFO3  CTL_CODE(DTA_DEVICE_TYPE, \
                          FUNC_DTA_GET_DEV_INFO3, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetDevInfoData3 {
        DtaIoctlGetDevInfoOutput3  m_Output;
    } DtaIoctlGetDevInfoData3;

    #define DTA_IOCTL_GET_DEV_INFO3  _IOR(DTA_IOCTL_MAGIC, FUNC_DTA_GET_DEV_INFO3, \
                                                             DtaIoctlGetDevInfoData3)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_GET_DEV_INFO4 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Gets the device information
//
typedef struct _DtaIoctlGetDevInfoOutput4 {
    UInt16  m_DeviceId;         // Device ID
    UInt16  m_VendorId;         // Vendor ID
    UInt16  m_SubVendorId;      // Subsystem Vendor ID
    UInt16  m_SubSystemId;      // Subsystem ID
    Int  m_BusNumber;           // PCI-bus number
    Int  m_SlotNumber;          // PCI-slot number
    Int  m_TypeNumber;          // Type number in decimal, e.g. 2160 for DTA-2160
    Int  m_SubDvc;              // Sub-device: 0=master, 1=slave1, 2=slave2, etc
    Int  m_HardwareRevision;    // Hardware Revision
    Int  m_FirmwareVersion;     // Firmware Version (= Altera revision), e.g. 3 for
                                // "Firmware Version 3"
    Int  m_FirmwareVariant;     // Firmware Variant, e.g. to distinguish between
                                // firmware with different #inputs/#outputs
    Int  m_FwPackageVersion;    // Firmware Package version, version number given
                                // to the set of firmware variants
    UInt64A  m_Serial;          // Serial number
    Int  m_PcieNumLanes;        // Number of allocated PCIe lanes
    Int  m_PcieMaxLanes;        // Maximum number of PCIe lanes
    Int  m_PcieLinkSpeed;       // Current PCIe link speed
    Int  m_PcieMaxSpeed;        // Maximum PCIe link speed
} DtaIoctlGetDevInfoOutput4;
ASSERT_SIZE(DtaIoctlGetDevInfoOutput4, 64)
    

#ifdef WINBUILD
    #define DTA_IOCTL_GET_DEV_INFO4  CTL_CODE(DTA_DEVICE_TYPE, \
                          FUNC_DTA_GET_DEV_INFO4, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetDevInfoData4 {
        DtaIoctlGetDevInfoOutput4  m_Output;
    } DtaIoctlGetDevInfoData4;

    #define DTA_IOCTL_GET_DEV_INFO4  _IOR(DTA_IOCTL_MAGIC, FUNC_DTA_GET_DEV_INFO4, \
                                                             DtaIoctlGetDevInfoData4)
#endif


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_VPD_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Wrapper command to performs a VPD read, delete or write command.
//

// DTA VPD commands
#define DTA_VPD_CMD_ITEM_READ          1
#define DTA_VPD_CMD_ITEM_WRITE         2
#define DTA_VPD_CMD_ITEM_DELETE        3
#define DTA_VPD_CMD_RAW_READ           4
#define DTA_VPD_CMD_RAW_WRITE          5

// DTA VPD section ID's
#define DTA_VPD_SECT_ID                1
#define DTA_VPD_SECT_RO                2
#define DTA_VPD_SECT_RW                4

// DTA VPD flags
#define DTA_VPD_FLAG_RO_WRITE_ALLOWED  1
#define DTA_VPD_FLAG_UPDATE_EEPROM     2

// DTA VPD size constants
#define DTA_VPD_MAX_KEY_LENGTH         15

// DTA VPD read item command input data type
typedef struct _DtaIoctlVpdCmdReadItemInput {
    UInt  m_SectionType;
    UInt  m_KeywordLen;
    char  m_Keyword[DTA_VPD_MAX_KEY_LENGTH];
    UInt  m_Flags;                              // Not used for now
    UInt  m_OutBufLen;
} DtaIoctlVpdCmdReadItemInput;
ASSERT_SIZE(DtaIoctlVpdCmdReadItemInput, 32)

// DTA VPD write item command input data type
typedef struct _DtaIoctlVpdCmdWriteItemInput {
    UInt  m_SectionType;
    UInt  m_KeywordLen;
    char  m_Keyword[DTA_VPD_MAX_KEY_LENGTH];
    UInt  m_Flags;
    UInt  m_BufLen;
    UInt8  m_Buf[0];                // Dynamic sized buffer
} DtaIoctlVpdCmdWriteItemInput;
ASSERT_SIZE(DtaIoctlVpdCmdWriteItemInput, 32)

// DTA VPD delete item command input data type
typedef struct _DtaIoctlVpdCmdDeleteItemInput {
    UInt  m_SectionType;
    UInt  m_KeywordLen;
    char  m_Keyword[DTA_VPD_MAX_KEY_LENGTH];
    UInt  m_Flags;
} DtaIoctlVpdCmdDeleteItemInput;
ASSERT_SIZE(DtaIoctlVpdCmdDeleteItemInput, 28)

// DTA VPD read raw command input data type
typedef struct _DtaIoctlVpdCmdReadRawInput {
    UInt  m_StartOffset;
    UInt  m_OutBufLen;
} DtaIoctlVpdCmdReadRawInput;
ASSERT_SIZE(DtaIoctlVpdCmdReadRawInput, 8)

// DTA VPD write raw command input data type
typedef struct _DtaIoctlVpdCmdWriteRawInput {
    UInt  m_StartOffset;
    UInt  m_BufLen;
    UInt8  m_Buf[0];                // Dynamic sized buffer
} DtaIoctlVpdCmdWriteRawInput;
ASSERT_SIZE(DtaIoctlVpdCmdWriteRawInput, 8)

// Ioctl input data type
typedef struct _DtaIoctlVpdCmdInput {
    UInt  m_Cmd;
    union {
        DtaIoctlVpdCmdReadItemInput  m_ReadItem;
        DtaIoctlVpdCmdWriteItemInput  m_WriteItem;
        DtaIoctlVpdCmdDeleteItemInput  m_DeleteItem;
        DtaIoctlVpdCmdReadRawInput  m_ReadRaw;
        DtaIoctlVpdCmdWriteRawInput  m_WriteRaw;
    } m_Data;
} DtaIoctlVpdCmdInput;
ASSERT_SIZE(DtaIoctlVpdCmdInput, 36)

// DTA VPD read item command output data type
typedef struct _DtaIoctlVpdCmdReadItemOutput {
    UInt  m_Length;
    UInt8  m_Buf[0];                // Dynamic sized buffer
} DtaIoctlVpdCmdReadItemOutput;
ASSERT_SIZE(DtaIoctlVpdCmdReadItemOutput, 4)

// DTA VPD read raw command output data type
typedef struct _DtaIoctlVpdCmdReadRawOutput {
    UInt  m_Length;
    UInt8  m_Buf[0];                // Dynamic sized buffer
} DtaIoctlVpdCmdReadRawOutput;
ASSERT_SIZE(DtaIoctlVpdCmdReadRawOutput, 4)

// Ioctl output data type
typedef struct _DtaIoctlVpdCmdOutput {
    union {
        DtaIoctlVpdCmdReadItemOutput  m_ReadItem;
        DtaIoctlVpdCmdReadRawOutput  m_ReadRaw;
    } m_Data;
} DtaIoctlVpdCmdOutput;
ASSERT_SIZE(DtaIoctlVpdCmdOutput, 4)

#ifdef WINBUILD
    #define DTA_IOCTL_VPD_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_VPD_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlVpdCmdData {
        DtaIoctlVpdCmdInput  m_Input;
        DtaIoctlVpdCmdOutput  m_Output;
    } DtaIoctlVpdCmdData;

    #define DTA_IOCTL_VPD_CMD  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_VPD_CMD, \
                                                                       DtaIoctlVpdCmdData)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_GET_DRIVER_VERSION +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Gets the DTA driver version number
//

typedef struct _DtaIoctlGetDriverVersionOutput {
    Int  m_Major;
    Int  m_Minor;
    Int  m_Micro;
    Int  m_Build;
} DtaIoctlGetDriverVersionOutput;
ASSERT_SIZE(DtaIoctlGetDriverVersionOutput, 16)

#ifdef WINBUILD
    #define DTA_IOCTL_GET_DRIVER_VERSION  CTL_CODE(DTA_DEVICE_TYPE, \
                           FUNC_DTA_GET_DRIVER_VERSION, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetDriverVersionData {
        DtaIoctlGetDriverVersionOutput  m_Output;
    } DtaIoctlGetDriverVersionData;

    #define DTA_IOCTL_GET_DRIVER_VERSION  _IOR(DTA_IOCTL_MAGIC, \
                                FUNC_DTA_GET_DRIVER_VERSION, DtaIoctlGetDriverVersionData)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_I2C_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Wrapper command to perform a I2C read or write command.
//

// Constants
#define DTA_I2C_MAX_BUF_SIZE  512 // Size of I2C buffer

// DTA I2C commands
#define DTA_I2C_CMD_LOCK      1
#define DTA_I2C_CMD_UNLOCK    2
#define DTA_I2C_CMD_READ      3
#define DTA_I2C_CMD_WRITE     4
#define DTA_I2C_CMD_WRITEREAD 5

// DTA I2C lock command input data type
typedef struct _DtaIoctlI2cCmdLockInput {
    Int  m_TimeoutMS;
} DtaIoctlI2cCmdLockInput;

// DTA I2C read command input data type
typedef struct _DtaIoctlI2cCmdReadInput {
    UInt  m_DevAddr;
    UInt  m_OutBufLen;
} DtaIoctlI2cCmdReadInput;
ASSERT_SIZE(DtaIoctlI2cCmdReadInput, 8)

// DTA I2C write command input data type
typedef struct _DtaIoctlI2cCmdWriteInput {
    UInt  m_DevAddr;
    UInt  m_BufLen;
    UInt8  m_Buf[DTA_I2C_MAX_BUF_SIZE];
} DtaIoctlI2cCmdWriteInput;
ASSERT_SIZE(DtaIoctlI2cCmdWriteInput, 520)

// DTA I2C write-read command input data type
typedef struct _DtaIoctlI2cCmdWriteReadInput {
    UInt  m_DevWriteAddr;
    UInt  m_DevReadAddr;
    UInt  m_BufLen;
    UInt  m_OutBufLen;
    UInt8  m_Buf[DTA_I2C_MAX_BUF_SIZE];
} DtaIoctlI2cCmdWriteReadInput;
ASSERT_SIZE(DtaIoctlI2cCmdWriteReadInput, 528)

// Ioctl input data type
typedef struct _DtaIoctlI2cCmdInput {
    UInt  m_Cmd;
    Int  m_PortIndex;                       // Port index for future use
    union {
        DtaIoctlI2cCmdLockInput  m_Lock;
        DtaIoctlI2cCmdReadInput  m_Read;
        DtaIoctlI2cCmdWriteInput  m_Write;
        DtaIoctlI2cCmdWriteReadInput  m_WriteRead;
    } m_Data;
} DtaIoctlI2cCmdInput;
ASSERT_SIZE(DtaIoctlI2cCmdInput, 536)

// DTA I2C read command output data type
typedef struct _DtaIoctlI2cCmdReadOutput {
    UInt  m_Length;
    UInt8  m_Buf[DTA_I2C_MAX_BUF_SIZE];
} DtaIoctlI2cCmdReadOutput;
ASSERT_SIZE(DtaIoctlI2cCmdReadOutput, 516)

// Ioctl output data type
typedef struct _DtaIoctlI2cCmdOutput {
    union {
        DtaIoctlI2cCmdReadOutput  m_Read;
    } m_Data;
} DtaIoctlI2cCmdOutput;
ASSERT_SIZE(DtaIoctlI2cCmdReadOutput, 516)

#ifdef WINBUILD
    #define DTA_IOCTL_I2C_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_I2C_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlI2cCmdData {
        DtaIoctlI2cCmdInput  m_Input;
        DtaIoctlI2cCmdOutput  m_Output;
    } DtaIoctlI2cCmdData;

    #define DTA_IOCTL_I2C_CMD  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_I2C_CMD, \
                                                                       DtaIoctlI2cCmdData)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Address Regs Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#define DTA_REGS_TYPE_GEN           1
#define DTA_REGS_TYPE_TS_TX         2
#define DTA_REGS_TYPE_TS_RX         3

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_GET_ADDRESS_REGS +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Gets a user-space virtual address of a register block.
// To prevent different Ioctl's/data between 32-bit and 64-bit applications<->driver,
// we use a UInt64 to store a 32-bit or 64-bit pointer
//
typedef struct _DtaIoctlGetAddressRegsInput {
    Int  m_PortIndex;             // Port index
    Int  m_RegsType;            // Type of register block
#ifndef WINBUILD
    UInt64A  m_pPaMmap;          // User-space address returned by mmap
#endif
} DtaIoctlGetAddressRegsInput;
#ifndef WINBUILD
ASSERT_SIZE(DtaIoctlGetAddressRegsInput, 16)
#else
ASSERT_SIZE(DtaIoctlGetAddressRegsInput, 8)
#endif

typedef struct _DtaIoctlGetAddressRegsOutput {
#ifndef WINBUILD
    Int  m_NotMapped;           // Indicats mmap must be done first
#endif
    Int  m_Size;                 // Size of register block returned, 0 = not filled
    UInt64A  m_pUserVirtual;     // User-space virtual address to register block
} DtaIoctlGetAddressRegsOutput;
ASSERT_SIZE(DtaIoctlGetAddressRegsOutput, 16)

#ifdef WINBUILD
    #define DTA_IOCTL_GET_ADDRESS_REGS  CTL_CODE(DTA_DEVICE_TYPE, \
                             FUNC_DTA_GET_ADDRESS_REGS, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetAddressRegs {
        DtaIoctlGetAddressRegsInput  m_Input;
        DtaIoctlGetAddressRegsOutput  m_Output;
    } DtaIoctlGetAddressRegs;

    #define DTA_IOCTL_GET_ADDRESS_REGS  _IOWR(DTA_IOCTL_MAGIC, \
                                FUNC_DTA_GET_ADDRESS_REGS, DtaIoctlGetAddressRegs)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_RELEASE_ADDRESS_REGS +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Releases a user-space virtual address of a register block.
//
typedef struct _DtaIoctlReleaseAddressRegsInput {
    Int  m_PortIndex;           // Port index
    Int  m_RegsType;            // Type of register block
} DtaIoctlReleaseAddressRegsInput;
ASSERT_SIZE(DtaIoctlReleaseAddressRegsInput, 8)

#ifndef WINBUILD
typedef struct _DtaIoctlReleaseAddressRegsOutput {
    Int  m_NotInUse;            // Indicates ref count was not 0
    Int  m_Size;                // Size of register block returned, 0 = not filled
    UInt64A  m_pPaMmap;         // User-space address returned by mmap
} DtaIoctlReleaseAddressRegsOutput;
ASSERT_SIZE(DtaIoctlReleaseAddressRegsOutput, 16)
#endif

#ifdef WINBUILD
    #define DTA_IOCTL_RELEASE_ADDRESS_REGS  CTL_CODE(DTA_DEVICE_TYPE, \
                         FUNC_DTA_RELEASE_ADDRESS_REGS, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlReleaseAddressRegs {
        DtaIoctlReleaseAddressRegsInput  m_Input;
        DtaIoctlReleaseAddressRegsOutput  m_Output;
    } DtaIoctlReleaseAddressRegs;

    #define DTA_IOCTL_RELEASE_ADDRESS_REGS  _IOWR(DTA_IOCTL_MAGIC, \
                                FUNC_DTA_RELEASE_ADDRESS_REGS, DtaIoctlReleaseAddressRegs)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_GET_EVENT +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Get the pending events from the driver
//
// Event types
#define DTA_EVENT_TYPE_POWER         0x00000001
#define DTA_EVENT_TYPE_GENLOCK       0x00000002
#define DTA_EVENT_TYPE_IOCONFIG      0x00000004
#define DTA_EVENT_TYPE_ALL           0xFFFFFFFF
// Power event values
#define DTA_EVENT_VALUE1_POWER_DOWN  0x00000001
#define DTA_EVENT_VALUE1_POWER_UP    0x00000002
// Genlock event values
#define DTA_EVENT_VALUE1_NO_LOCK     0x00000001
#define DTA_EVENT_VALUE1_LOCKED      0x00000002

typedef struct _DtaIoctlGetEventOutput {
    UInt  m_EventType;
    UInt  m_Value1;
    UInt  m_Value2;
} DtaIoctlGetEventOutput;
ASSERT_SIZE(DtaIoctlGetEventOutput, 12)

#ifdef WINBUILD
    #define DTA_IOCTL_GET_EVENT  CTL_CODE(DTA_DEVICE_TYPE, \
                                   FUNC_DTA_GET_EVENT, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetEvent {
        DtaIoctlGetEventOutput  m_Output;
    } DtaIoctlGetEvent;

    #define DTA_IOCTL_GET_EVENT  _IOR(DTA_IOCTL_MAGIC, \
                                                   FUNC_DTA_GET_EVENT, DtaIoctlGetEvent)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_REGISTER_EVENTS +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Registers the events to be interested in 
//

typedef struct _DtaIoctlRegisterEventsInput {
    UInt  m_EventTypeMask;            // Holds the event types to be interested in
} DtaIoctlRegisterEventsInput;
ASSERT_SIZE(DtaIoctlRegisterEventsInput, 4)

#ifdef WINBUILD
    #define DTA_IOCTL_REGISTER_EVENTS  CTL_CODE(DTA_DEVICE_TYPE, \
                              FUNC_DTA_REGISTER_EVENTS, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlRegisterEvents {
        DtaIoctlRegisterEventsInput  m_Input;
    } DtaIoctlRegisterEvents;

    #define DTA_IOCTL_REGISTER_EVENTS  _IOW(DTA_IOCTL_MAGIC, \
                                         FUNC_DTA_REGISTER_EVENTS, DtaIoctlRegisterEvents)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_UNREGISTER_EVENTS +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// UnRegisters the events
//

#ifdef WINBUILD
    #define DTA_IOCTL_UNREGISTER_EVENTS  CTL_CODE(DTA_DEVICE_TYPE, \
                            FUNC_DTA_UNREGISTER_EVENTS, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    

    #define DTA_IOCTL_UNREGISTER_EVENTS  _IO(DTA_IOCTL_MAGIC, FUNC_DTA_REGISTER_EVENTS)
#endif


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_DMA_READ +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//

typedef struct _DtaIoctlDmaReadInput {
    Int  m_PortIndex;           // Port index
#ifdef LINBUILD
    UInt64A  m_BufferAddr;
    Int  m_NumBytesToRead;
#endif
} DtaIoctlDmaReadInput;
#ifdef LINBUILD
ASSERT_SIZE(DtaIoctlDmaReadInput, 24)
#else
ASSERT_SIZE(DtaIoctlDmaReadInput, 4)
#endif

#ifdef LINBUILD
typedef struct _DtaIoctlDmaReadOutput {
    Int  m_NumBytesRead;
} DtaIoctlDmaReadOutput;
#endif

#ifdef WINBUILD
    #define DTA_IOCTL_DMA_READ  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_DMA_READ, \
                                                        METHOD_IN_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlDmaReadData {
        DtaIoctlDmaReadInput  m_Input;
        DtaIoctlDmaReadOutput  m_Output;
    } DtaIoctlDmaReadData;

    #define DTA_IOCTL_DMA_READ  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_DMA_READ, \
                                                                      DtaIoctlDmaReadData)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_DMA_WRITE +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// For windows, we don't use the m_BufferAddr but the output buffer to store the buffer
// pointer so we don't need the extra EvtIoInCallerContext callback routine
//
typedef struct _DtaIoctlDmaWriteInput {
    Int  m_PortIndex;           // Port index
#ifdef LINBUILD
    UInt64A  m_BufferAddr;
    Int  m_NumBytesToWrite;
#endif
} DtaIoctlDmaWriteInput;
#ifdef LINBUILD
ASSERT_SIZE(DtaIoctlDmaWriteInput, 24)
#else
ASSERT_SIZE(DtaIoctlDmaWriteInput, 4)
#endif

#ifdef WINBUILD
    #define DTA_IOCTL_DMA_WRITE  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_DMA_WRITE, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlDmaWriteData {
        DtaIoctlDmaWriteInput  m_Input;
    } DtaIoctlDmaWriteData;

    #define DTA_IOCTL_DMA_WRITE  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_DMA_WRITE, \
                                                                     DtaIoctlDmaWriteData)
#endif



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_SH_BUF_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Wrapper command to perform operation on a shared buffer user-driver
//
// DTA shared buf commands
#define DTA_SH_BUF_CMD_INIT            1
#define DTA_SH_BUF_CMD_READ            2
#define DTA_SH_BUF_CMD_WRITE           3
#define DTA_SH_BUF_CMD_CLOSE           4

// DTA shared buf purpose
#define DTA_SH_BUF_PURPOSE_DMA         DT_SH_BUF_PURPOSE_DMA
#define DTA_SH_BUF_PURPOSE_GEN         DT_SH_BUF_PURPOSE_GEN

// DTA shared buf channel types
#define DTA_SH_CHANTYPE_UNDEFINED      DT_SH_CHANTYPE_UNDEFINED
#define DTA_SH_CHANTYPE_IPTX           DT_SH_CHANTYPE_IPTX
#define DTA_SH_CHANTYPE_IPRX           DT_SH_CHANTYPE_IPRX
#define DTA_SH_CHANTYPE_IPRX_V2        DT_SH_CHANTYPE_IPRX_V2   // Dynamic buf size layout

// DTA HP Buf init command input data type
typedef struct _DtaIoctlShBufCmdInitInput {
    UInt  m_Purpose;            //  Shared buffer user/driver or DMA'eble buffer
#ifdef LINBUILD
    UInt64A  m_BufferAddr;
    Int  m_BufferSize;
#endif
} DtaIoctlShBufCmdInitInput;
#ifdef LINBUILD
ASSERT_SIZE(DtaIoctlShBufCmdInitInput, 24)
#else
ASSERT_SIZE(DtaIoctlShBufCmdInitInput, 4)
#endif

// DTA HP Buf read command input data type
typedef struct _DtaIoctlShBufCmdReadInput {
    Int  m_Offset;
    Int  m_NumBytesToRead;
} DtaIoctlShBufCmdReadInput;
ASSERT_SIZE(DtaIoctlShBufCmdReadInput, 8)

// DTA HP Buf write command input data type
typedef struct _DtaIoctlShBufCmdWriteInput {
    Int  m_Offset;
    Int  m_NumBytesToWrite;
} DtaIoctlShBufCmdWriteInput;
ASSERT_SIZE(DtaIoctlShBufCmdWriteInput, 8)

// Ioctl input data type
typedef struct _DtaIoctlShBufCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;           // Port index
    Int  m_ChannelIndex;        // Channel index (for IP channels only)
    Int  m_ChannelType;         // Channel type (for IP channels only)
    Int  m_BufferIndex;         // Only 1 buffer supported for now. Must be 0
    union {
        DtaIoctlShBufCmdInitInput  m_Init;
        DtaIoctlShBufCmdReadInput  m_Read;
        DtaIoctlShBufCmdWriteInput  m_Write;
    } m_Data;
} DtaIoctlShBufCmdInput;
#ifdef LINBUILD
ASSERT_SIZE(DtaIoctlShBufCmdInput, 48)
#else
ASSERT_SIZE(DtaIoctlShBufCmdInput, 28)
#endif

#ifdef WINBUILD
    #define DTA_IOCTL_SH_BUF_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_SH_BUF_CMD, \
                                                         METHOD_IN_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlShBufCmdData {
        DtaIoctlShBufCmdInput  m_Input;
    } DtaIoctlShBufCmdData;

    #define DTA_IOCTL_SH_BUF_CMD  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_SH_BUF_CMD, \
                                                                       DtaIoctlShBufCmdData)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_GET_IO_CONFIG +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Get port IO configurations
//

// DtaIoctlGetIoConfigInput
typedef struct _DtaIoctlGetIoConfigInput
{
    Int  m_IoConfigCount;
    DtIoConfigId m_IoCfgId[0];

} DtaIoctlGetIoConfigInput;
ASSERT_SIZE(DtaIoctlGetIoConfigInput, 4)

// DtaIoctlGetIoConfigOutput
typedef struct _DtaIoctlGetIoConfigOutput
{
    DtIoConfigValue m_IoCfgValue[0];
} DtaIoctlGetIoConfigOutput;

#ifdef WINBUILD
    #define DTA_IOCTL_GET_IOCONFIG  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_GET_IOCONFIG, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetIoConfig {
        DtaIoctlGetIoConfigInput  m_Input;
        DtaIoctlGetIoConfigOutput  m_Output;
    } DtaIoctlGetIoConfig;

    #define DTA_IOCTL_GET_IOCONFIG  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_GET_IOCONFIG, \
                                                                      DtaIoctlGetIoConfig)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_SET_IOCONFIG +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Set port IO configurations
//

typedef struct _DtaIoctlSetIoConfigInput {
    Int  m_IoConfigCount;
    DtIoConfigPars  m_IoConfig[0];
} DtaIoctlSetIoConfigInput;
ASSERT_SIZE(DtaIoctlSetIoConfigInput, 8)

#ifdef WINBUILD
    #define DTA_IOCTL_SET_IOCONFIG  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_SET_IOCONFIG, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlSetIoConfig {
        DtaIoctlSetIoConfigInput  m_Input;
    } DtaIoctlSetIoConfig;

    #define DTA_IOCTL_SET_IOCONFIG  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_SET_IOCONFIG, \
                                                                      DtaIoctlSetIoConfig)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_NONIP_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Wrapper IOCTL for all non IP commands.
//

// Command definitions
#define DTA_NONIP_CMD_EXCLUSIVE_ACCESS  1
#define DTA_NONIP_CMD_GET_TARGET_ID     2
#define DTA_NONIP_CMD_DETECT_VIDSTD     3   // DO NOT USE; REPLACED BY MATRIX COMMAND
                                            // DTA_MATRIX_CMD_DETECT_VIDSTD
#define DTA_NONIP_CMD_GET_GENREF_PROPS  4   // Get the properties for a GENREF port
#define DTA_NONIP_CMD_NOTIFY_GENREF_PROPS  5  // Notification message with update of 
                                              // GENREF properties
#define DTA_NONIP_CMD_DETECT_VIDSTD2    6   // Get information about video standard
                                            // and VPID information
#define DTA_NONIP_CMD_GET_AUDIO_STATUS  7   // Get audio channel status
#define DTA_NONIP_CMD_GET_DMA_STATS     8   // Get stats of last DMA transfer
#define DTA_NONIP_CMD_GET_AUDIO_STATUS2  9  // Get audio channel status and PCM/Data

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_NONIP_CMD_EXCLUSIVE_ACCESS -.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Exclusive access cmd's
#define DTA_EXCLUSIVE_ACCESS_CMD_ACQUIRE  1
#define DTA_EXCLUSIVE_ACCESS_CMD_RELEASE  2
#define DTA_EXCLUSIVE_ACCESS_CMD_PROBE    3
#define DTA_EXCLUSIVE_ACCESS_CMD_CHECK    4
// Exlusive Access input data
typedef struct _DtaIoctlNonIpCmdExclusiveAccessInput {
    Int  m_Cmd;                 // Acquire/release/probe command
} DtaIoctlNonIpCmdExclusiveAccessInput;
ASSERT_SIZE(DtaIoctlNonIpCmdExclusiveAccessInput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_NONIP_CMD_GET_TARGET_ID -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Target Id present states
#define DTA_NO_CONNECTION   0
#define DTA_DVB_SPI_SINK    1
#define DTA_TARGET_PRESENT  2
#define DTA_TARGET_UNKNOWN  3
// Get Target Id output data
typedef struct _DtaIoctlNonIpCmdGetTargetIdOutput {
    Int  m_Present;             // Target ID present state
    Int  m_Id;                  // Target ID if present == DTA_TARGET_PRESENT
} DtaIoctlNonIpCmdGetTargetIdOutput;
ASSERT_SIZE(DtaIoctlNonIpCmdGetTargetIdOutput, 8)


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_NONIP_CMD_DETECT_VIDSTD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Ioctl output data type
typedef struct _DtaIoctlNonIpCmdDetectVidStdOutput {
    Int  m_VidStd;              // Detected video standard
} DtaIoctlNonIpCmdDetectVidStdOutput;
ASSERT_SIZE(DtaIoctlNonIpCmdDetectVidStdOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_NONIP_CMD_GET_GENREF_PROPS -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Get the properties for a GENREF port

// GENREF port types
#define DTA_GENREF_PORTTYPE_INTERNAL    0
#define DTA_GENREF_PORTTYPE_ANALOG      1
#define DTA_GENREF_PORTTYPE_DIGITAL     2

// NOTE: DtaIoctlGetGenrefProps is used by DTA_IOCTL_GET_GENREF_PROPS and 
// DTA_IOCTL_NOTIFY_GENREF_PROPS
typedef struct _DtaIoctlNonIpGenRefProps {
    // GENREF port
    Int  m_RefPortIndex;        // Port index of the GENREF port
    Int  m_RefPortType;         // Type of GENREF port (Internal, analog or Digital)
    // GENREF settings
    Int  m_RefVidStd;           // Reference signal applied to GENREF input
    Int  m_OutVidStd;           // Output video standard the GENREF logic generates
    Int  m_TofAlignOffsetNs;    // Alignment offset, in ns, TOF signal 
    // Group of genlock ports, under control of this GENREF port. 
    // NOTE: Bit0=Port1, Bit1=Port2, ... , Bit31=Port32
    UInt32  m_Group;            
} DtaIoctlNonIpGenRefProps;
ASSERT_SIZE(DtaIoctlNonIpGenRefProps, 24)

typedef DtaIoctlNonIpGenRefProps DtaIoctlNonIpGetGenRefPropsOutput;
ASSERT_SIZE(DtaIoctlNonIpGetGenRefPropsOutput, 24)

//.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_NONIP_CMD_NOTIFY_GENREF_PROPS -.-.-.-.-.-.-.-.-.-.-.-.-.-
// Notification message with update of GENREF properties

typedef DtaIoctlNonIpGenRefProps DtaIoctlNonIpNotifyGenRefPropsInput;
ASSERT_SIZE(DtaIoctlNonIpNotifyGenRefPropsInput, 24)


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_NONIP_CMD_DETECT_VIDSTD2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Ioctl output data type
typedef struct _DtaIoctlNonIpCmdDetectVidStd2Output {
    Int  m_VidStd;              // Detected video standard
    UInt  m_Vpid;               // Detected VPID
    UInt  m_Vpid2;              // VPID in link 2 for 3g level b signals
    Int  m_AspectRatio;         // Detected aspect ratio
} DtaIoctlNonIpCmdDetectVidStd2Output;
ASSERT_SIZE(DtaIoctlNonIpCmdDetectVidStd2Output, 16)

#define DTA_AR_UNKNOWN                0  // Unknown aspect ratio
#define DTA_AR_4_3                    1  // 4x3
#define DTA_AR_16_9                   2  // 16x9
#define DTA_AR_14_9                   3  // 14x9

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_NONIP_CMD_GET_AUDIO_STATUS -.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Ioctl output data type
typedef struct _DtaIoctlNonIpCmdGetAudioStatusOutput {
    Int  m_NumAudioChannels;
    struct 
    {
        Int  m_ChanIdx;
        Int  m_IsAsynchronous;          // Is channel pair asynchronous?
        Int  m_Rate;                    // Audio sample rate
        Int  m_ChanStatusNumValid;      // Number of valid bytes in channel status data
        UInt8  m_ChanStatusData[24];    // Channel status data
    } m_AudioChanStatus[32];
} DtaIoctlNonIpCmdGetAudioStatusOutput;
ASSERT_SIZE(DtaIoctlNonIpCmdGetAudioStatusOutput, 1284)

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_NONIP_CMD_GET_AUDIO_STATUS2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Ioctl output data type
typedef struct _DtaIoctlNonIpCmdGetAudioStatus2Output {
    Int  m_NumAudioChannels;
    struct 
    {
        Int  m_ChanIdx;
        Int  m_IsAsynchronous;          // Is channel pair asynchronous?
        Int  m_Rate;                    // Audio sample rate
        Int  m_Content;                 // Audio channel contents
        Int  m_ChanStatusNumValid;      // Number of valid bytes in channel status data
        UInt8  m_ChanStatusData[24];    // Channel status data
    } m_AudioChanStatus[32];
} DtaIoctlNonIpCmdGetAudioStatus2Output;
ASSERT_SIZE(DtaIoctlNonIpCmdGetAudioStatus2Output, 1412)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_NONIP_CMD_GET_DMA_STATS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Ioctl output data type
typedef struct _DtaIoctlNonIpCmdGetDmaStatsOutput {
    UInt64A  m_Time;            // Total DMA time in us
    UInt  m_Direction;          // 1 = From device, 2 = To device
    UInt64A  m_NumBytesTransferred; // Total number of bytes send or received
} DtaIoctlNonIpCmdGetDmaStatsOutput;
ASSERT_SIZE(DtaIoctlNonIpCmdGetDmaStatsOutput, 24)


// Ioctl input data type
typedef struct _DtaIoctlNonIpCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;
    union {
        DtaIoctlNonIpCmdExclusiveAccessInput  m_ExclusiveAccess;
        DtaIoctlNonIpNotifyGenRefPropsInput  m_NotifyGenRefProps;
    } m_Data;
} DtaIoctlNonIpCmdInput;
ASSERT_SIZE(DtaIoctlNonIpCmdInput, 32)

// Ioctl Output data type
typedef struct _DtaIoctlNonIpCmdOutput {
    union {
        DtaIoctlNonIpCmdGetTargetIdOutput  m_GetTargetId;
        DtaIoctlNonIpCmdDetectVidStdOutput  m_DetVidStd;
        DtaIoctlNonIpGetGenRefPropsOutput  m_GetGenRefProps;
        DtaIoctlNonIpCmdDetectVidStd2Output  m_DetVidStd2;
        DtaIoctlNonIpCmdGetAudioStatusOutput  m_GetAudioStatus;
        DtaIoctlNonIpCmdGetAudioStatus2Output  m_GetAudioStatus2;
        DtaIoctlNonIpCmdGetDmaStatsOutput  m_GetDmaStats;
    } m_Data;
} DtaIoctlNonIpCmdOutput;
ASSERT_SIZE(DtaIoctlNonIpCmdOutput, 1416)

#ifdef WINBUILD
    #define DTA_IOCTL_NONIP_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_NONIP_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlNonIpCmd {
        DtaIoctlNonIpCmdInput  m_Input;
        DtaIoctlNonIpCmdOutput  m_Output;
    } DtaIoctlNonIpCmd;
    ASSERT_SIZE(DtaIoctlNonIpCmd, 1416)

    #define DTA_IOCTL_NONIP_CMD  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_NONIP_CMD, \
                                                                         DtaIoctlNonIpCmd)


    // Ioctl input data type
    typedef struct _DtaIoctlNonIpCmdInputLegacy {
        Int  m_Cmd;
        Int  m_PortIndex;
        union {
            DtaIoctlNonIpCmdExclusiveAccessInput  m_ExclusiveAccess;
        } m_Data;
    } DtaIoctlNonIpCmdInputLegacy;
    ASSERT_SIZE(DtaIoctlNonIpCmdInputLegacy, 12)

    // Ioctl Output data type
    typedef struct _DtaIoctlNonIpCmdOutputLegacy {
        union {
            DtaIoctlNonIpCmdGetTargetIdOutput  m_GetTargetId;
            DtaIoctlNonIpCmdDetectVidStdOutput  m_DetVidStd;
        } m_Data;
    } DtaIoctlNonIpCmdOutputLegacy;
    ASSERT_SIZE(DtaIoctlNonIpCmdOutputLegacy, 8)

    typedef union _DtaIoctlNonIpCmdLegacy {
        DtaIoctlNonIpCmdInputLegacy  m_Input;
        DtaIoctlNonIpCmdOutputLegacy  m_Output;
    } DtaIoctlNonIpCmdLegacy;

    #define DTA_IOCTL_NONIP_CMD_LEGACY  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_NONIP_CMD, \
                                                                   DtaIoctlNonIpCmdLegacy)

    // Ioctl input data type
    typedef struct _DtaIoctlNonIpCmdInputLegacy2 {
        Int  m_Cmd;
        Int  m_PortIndex;
        union {
            DtaIoctlNonIpCmdExclusiveAccessInput  m_ExclusiveAccess;
            DtaIoctlNonIpNotifyGenRefPropsInput  m_NotifyGenRefProps;
        } m_Data;
    } DtaIoctlNonIpCmdInputLegacy2;
    ASSERT_SIZE(DtaIoctlNonIpCmdInputLegacy2, 32)

    // Ioctl Output data type
    typedef struct _DtaIoctlNonIpCmdOutputLegacy2 {
        union {
            DtaIoctlNonIpCmdGetTargetIdOutput  m_GetTargetId;
            DtaIoctlNonIpCmdDetectVidStdOutput  m_DetVidStd;
            DtaIoctlNonIpGetGenRefPropsOutput  m_GetGenRefProps;
        } m_Data;
    } DtaIoctlNonIpCmdOutputLegacy2;
    ASSERT_SIZE(DtaIoctlNonIpCmdOutputLegacy2, 24)

    typedef union _DtaIoctlNonIpCmdLegacy2 {
        DtaIoctlNonIpCmdInputLegacy2  m_Input;
        DtaIoctlNonIpCmdOutputLegacy2  m_Output;
    } DtaIoctlNonIpCmdLegacy2;
    ASSERT_SIZE(DtaIoctlNonIpCmdLegacy2, 32)

    #define DTA_IOCTL_NONIP_CMD_LEGACY2  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_NONIP_CMD, \
                                                                  DtaIoctlNonIpCmdLegacy2)

    // Ioctl Output data type
    typedef struct _DtaIoctlNonIpCmdOutputLegacy3 {
        union {
            DtaIoctlNonIpCmdGetTargetIdOutput  m_GetTargetId;
            DtaIoctlNonIpCmdDetectVidStdOutput  m_DetVidStd;
            DtaIoctlNonIpGetGenRefPropsOutput  m_GetGenRefProps;
            DtaIoctlNonIpCmdDetectVidStd2Output  m_DetVidStd2;
            DtaIoctlNonIpCmdGetAudioStatusOutput  m_GetAudioStatus;
            DtaIoctlNonIpCmdGetDmaStatsOutput  m_GetDmaStats;
        } m_Data;
    } DtaIoctlNonIpCmdOutputLegacy3;

    typedef union _DtaIoctlNonIpCmdLegacy3 {
        DtaIoctlNonIpCmdInputLegacy2  m_Input;
        DtaIoctlNonIpCmdOutputLegacy3  m_Output;
    } DtaIoctlNonIpCmdLegacy3;
    ASSERT_SIZE(DtaIoctlNonIpCmdLegacy3, 1288)

    #define DTA_IOCTL_NONIP_CMD_LEGACY3  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_NONIP_CMD, \
                                                                  DtaIoctlNonIpCmdLegacy3)

#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_NONIP_TX_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Wrapper IOCTL for all non IP TX commands.
//

// Command definitions
#define DTA_NONIP_TX_CMD_GET_FLAGS           1
#define DTA_NONIP_TX_CMD_CLEAR_FLAGS         2
#define DTA_NONIP_TX_CMD_SET_FAILSAFE_CFG    3
#define DTA_NONIP_TX_CMD_SET_FAILSAFE_ALIVE  4
#define DTA_NONIP_TX_CMD_GET_FAILSAFE_INFO   5

// Status flag definitions
#define DTA_TX_FIFO_UFL      2
#define DTA_TX_SYNC_ERR      4
#define DTA_TX_READBACK_ERR  8
#define DTA_TX_TARGET_ERR    16
#define DTA_TX_FIFO_OVF      32

// DTA_NONIP_TX_CMD_GET_FLAGS
typedef struct _DtaIoctlNonIpTxCmdGetFlagsOutput {
    Int  m_Status;
    Int  m_Latched;
} DtaIoctlNonIpTxCmdGetFlagsOutput;
ASSERT_SIZE(DtaIoctlNonIpTxCmdGetFlagsOutput, 8)
// DTA_NONIP_TX_CMD_CLEAR_FLAGS
typedef struct _DtaIoctlNonIpTxCmdClearFlagsInput {
    Int  m_FlagsToClear;
} DtaIoctlNonIpTxCmdClearFlagsInput;
ASSERT_SIZE(DtaIoctlNonIpTxCmdClearFlagsInput, 4)

// DTA_NONIP_TX_CMD_SET_FAILSAVE_CFG
typedef struct _DtaIoctlNonIpTxCmdSetFailsafeCfgInput {
    Int  m_Enable;   // Do NOT use Bool (defined different in DTAPI and Driver)
    Int  m_Timeout;
} DtaIoctlNonIpTxCmdSetFailsafeCfgInput;
ASSERT_SIZE(DtaIoctlNonIpTxCmdSetFailsafeCfgInput, 8)
// DTA_NONIP_TX_CMD_SET_FAILSAVE_ALIVE
// DTA_NONIP_TX_CMD_GET_FAILSAVE_INFO
typedef struct _DtaIoctlNonIpTxCmdGetFailsafeInfoOutput {
    Int  m_Enable;   // Do NOT use Bool (defined different in DTAPI and Driver)
    Int  m_Timeout;
    Int  m_Alive;
} DtaIoctlNonIpTxCmdGetFailsafeInfoOutput;
ASSERT_SIZE(DtaIoctlNonIpTxCmdGetFailsafeInfoOutput, 12)

// Ioctl input data type
typedef struct _DtaIoctlNonIpTxCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;
    union {
        DtaIoctlNonIpTxCmdClearFlagsInput  m_ClearFlags;
        DtaIoctlNonIpTxCmdSetFailsafeCfgInput  m_SetFailsafeCfg;
    } m_Data;
} DtaIoctlNonIpTxCmdInput;
ASSERT_SIZE(DtaIoctlNonIpTxCmdInput, 16)

// Ioctl output data type
typedef struct _DtaIoctlNonIpTxCmdOutput {
    union {
        DtaIoctlNonIpTxCmdGetFlagsOutput  m_GetFlags;
        DtaIoctlNonIpTxCmdGetFailsafeInfoOutput  m_GetFailsafeInfo;
    } m_Data;
} DtaIoctlNonIpTxCmdOutput;
ASSERT_SIZE(DtaIoctlNonIpTxCmdOutput, 12)

#ifdef WINBUILD
    #define DTA_IOCTL_NONIP_TX_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_NONIP_TX_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlNonIpTxCmd {
        DtaIoctlNonIpTxCmdInput  m_Input;
        DtaIoctlNonIpTxCmdOutput  m_Output;
    } DtaIoctlNonIpTxCmd;

    #define DTA_IOCTL_NONIP_TX_CMD  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_NONIP_TX_CMD, \
                                                                       DtaIoctlNonIpTxCmd)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_NONIP_RX_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Wrapper IOCTL for all non IP RX commands.
//

// Command definitions
#define DTA_NONIP_RX_CMD_GET_FLAGS           1
#define DTA_NONIP_RX_CMD_CLEAR_FLAGS         2
#define DTA_NONIP_RX_CMD_GET_FREQ_ESTIMATE   3
#define DTA_NONIP_RX_CMD_GET_GENREF_STATE    4

// Status flag definitions
#define DTA_RX_FIFO_OVF         2
#define DTA_RX_SYNC_ERR         4
#define DTA_RX_RATE_OVF         8
#define DTA_RX_TARGET_ERR       16

// DTA_NONIP_RX_CMD_GET_FLAGS
typedef struct _DtaIoctlNonIpRxCmdGetFlagsOutput {
    Int  m_Status;
    Int  m_Latched;
} DtaIoctlNonIpRxCmdGetFlagsOutput;
ASSERT_SIZE(DtaIoctlNonIpRxCmdGetFlagsOutput, 8)

// DTA_NONIP_RX_CMD_CLEAR_FLAGS
typedef struct _DtaIoctlNonIpRxCmdClearFlagsInput {
    Int  m_FlagsToClear;
} DtaIoctlNonIpRxCmdClearFlagsInput;
ASSERT_SIZE(DtaIoctlNonIpRxCmdClearFlagsInput, 4)

// DTA_NONIP_RX_CMD_GET_FREQ_ESTIMATE
typedef struct _DtaIoctlNonIpRxCmdGetFreqEstimateOutput {
    Int  m_FreqEstimation;
} DtaIoctlNonIpRxCmdGetFreqEstimateOutput;
ASSERT_SIZE(DtaIoctlNonIpRxCmdGetFreqEstimateOutput, 4)

// DTA_NONIP_RX_CMD_GET_GENREF_STATE
typedef struct _DtaIoctlNonIpRxCmdGetGenrefStateOutput {
    Int  m_InLock;  // Do NOT use Bool (defined different in DTAPI and Driver)
    Int  m_Enabled; // Do NOT use Bool (defined different in DTAPI and Driver)
} DtaIoctlNonIpRxCmdGetGenrefStateOutput;
ASSERT_SIZE(DtaIoctlNonIpRxCmdGetGenrefStateOutput, 8)

// Ioctl input data type
typedef struct _DtaIoctlNonIpRxCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;
    union {
        DtaIoctlNonIpRxCmdClearFlagsInput  m_ClearFlags;
    } m_Data;
} DtaIoctlNonIpRxCmdInput;
ASSERT_SIZE(DtaIoctlNonIpRxCmdInput, 12)

// Ioctl input data type
typedef struct _DtaIoctlNonIpRxCmdOutput {
    union {
        DtaIoctlNonIpRxCmdGetFlagsOutput  m_GetFlags;
        DtaIoctlNonIpRxCmdGetFreqEstimateOutput  m_GetFreqEstimate;
        DtaIoctlNonIpRxCmdGetGenrefStateOutput  m_GetGenRefState;
    } m_Data;
} DtaIoctlNonIpRxCmdOutput;
ASSERT_SIZE(DtaIoctlNonIpRxCmdOutput, 8)

#ifdef WINBUILD
    #define DTA_IOCTL_NONIP_RX_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_NONIP_RX_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlNonIpRxCmd {
        DtaIoctlNonIpRxCmdInput  m_Input;
        DtaIoctlNonIpRxCmdOutput  m_Output;
    } DtaIoctlNonIpRxCmd;

    #define DTA_IOCTL_NONIP_RX_CMD  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_NONIP_RX_CMD, \
                                                                       DtaIoctlNonIpRxCmd)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_IP_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Wrapper IOCTL for all IP device specific commands.
//

// Command definitions
#define DTA_IP_CMD_GETMACADDRESS   1
#define DTA_IP_CMD_GETSPEED        2
#define DTA_IP_CMD_SETSPEED        3
#define DTA_IP_CMD_GETADMINSTATUS  4
#define DTA_IP_CMD_SETADMINSTATUS  5
#define DTA_IP_CMD_GETPHYREGISTER  6
#define DTA_IP_CMD_SETPHYREGISTER  7
#define DTA_IP_CMD_GETMACCOUNTER   8
#define DTA_IP_CMD_ENABLELOOPBACK  9

// DTA_IP_CMD_GETMACADDRESS
typedef struct _DtaIoctlIpCmdGetMacAddressOutput {
    UInt8  m_Address[6];
} DtaIoctlIpCmdGetMacAddressOutput;
ASSERT_SIZE(DtaIoctlIpCmdGetMacAddressOutput, 6)

// Speed values
#define DTA_NWSPEED_10MB_HALF       DTA_PHY_SPEED_10_HALF
#define DTA_NWSPEED_10MB_FULL       DTA_PHY_SPEED_10_FULL
#define DTA_NWSPEED_100MB_HALF      DTA_PHY_SPEED_100_HALF
#define DTA_NWSPEED_100MB_FULL      DTA_PHY_SPEED_100_FULL
#define DTA_NWSPEED_1GB_MASTER      DTA_PHY_SPEED_1000_MASTER
#define DTA_NWSPEED_1GB_SLAVE       DTA_PHY_SPEED_1000_SLAVE
#define DTA_NWSPEED_AUTO            DTA_PHY_SPEED_AUTO_DETECT  // Set
#define DTA_NWSPEED_NOLINK          DTA_PHY_SPEED_NO_LINK      // Get

// DTA_IP_CMD_GETSPEED
typedef struct _DtaIoctlIpCmdGetSpeedOutput {
    Int  m_Speed;
    Int  m_EnableForceSpeed; // Do NOT use Bool (defined different in DTAPI and Driver)
} DtaIoctlIpCmdGetSpeedOutput;
ASSERT_SIZE(DtaIoctlIpCmdGetSpeedOutput, 8)

// DTA_IP_CMD_SETSPEED
typedef struct _DtaIoctlIpCmdSetSpeedInput {
    Int  m_Speed;
    Int  m_EnableForceSpeed;
} DtaIoctlIpCmdSetSpeedInput;
ASSERT_SIZE(DtaIoctlIpCmdSetSpeedInput, 8)

// DTA_IP_CMD_GETADMINSTATUS
typedef struct _DtaIoctlIpCmdGetAdminStatusOutput {
    Int  m_AdminStatusUp; // Do NOT use Bool (defined different in DTAPI and Driver)
} DtaIoctlIpCmdGetAdminStatusOutput;
ASSERT_SIZE(DtaIoctlIpCmdGetAdminStatusOutput, 4)

// DTA_IP_CMD_SETADMINSTATUS
typedef struct _DtaIoctlIpCmdSetAdminStatusInput {
    Int  m_AdminStatusUp;    
} DtaIoctlIpCmdSetAdminStatusInput;
ASSERT_SIZE(DtaIoctlIpCmdSetAdminStatusInput, 4)

// DTA_IP_CMD_GETPHYREGISTER
typedef struct _DtaIoctlIpCmdGetPhyRegisterInput {
    UInt  m_Offset;
} DtaIoctlIpCmdGetPhyRegisterInput;
ASSERT_SIZE(DtaIoctlIpCmdGetPhyRegisterInput, 4)

typedef struct _DtaIoctlIpCmdGetPhyRegisterOutput {
    UInt  m_Value;
} DtaIoctlIpCmdGetPhyRegisterOutput;
ASSERT_SIZE(DtaIoctlIpCmdGetPhyRegisterOutput, 4)

// DTA_IP_CMD_SETPHYREGISTER
typedef struct _DtaIoctlIpCmdSetPhyRegisterInput {
    UInt  m_Offset;
    UInt32  m_Value;
} DtaIoctlIpCmdSetPhyRegisterInput;
ASSERT_SIZE(DtaIoctlIpCmdSetPhyRegisterInput, 8)

    // DTA_IP_CMD_GETMACCOUNTER
typedef struct _DtaIoctlIpCmdGetMacCounterInput {
    UInt  m_CounterId;
} DtaIoctlIpCmdGetMacCounterInput;
ASSERT_SIZE(DtaIoctlIpCmdGetMacCounterInput, 4)

typedef struct _DtaIoctlIpCmdGetMacCounterOutput {
    UInt64A  m_Value;
} DtaIoctlIpCmdGetMacCounterOutput;
ASSERT_SIZE(DtaIoctlIpCmdGetMacCounterOutput, 8)

// DTA_IP_CMD_ENABLELOOPBACK
typedef struct _DtaIoctlIpCmdEnableLoopbackInput {
    Int  m_Enable;    
} DtaIoctlIpCmdEnableLoopbackInput;
ASSERT_SIZE(DtaIoctlIpCmdEnableLoopbackInput, 4)

// Ioctl input data type
typedef struct _DtaIoctlIpCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;
    union {
        DtaIoctlIpCmdSetSpeedInput  m_SetSpeed;
        DtaIoctlIpCmdSetAdminStatusInput  m_SetAdminStatus;
        DtaIoctlIpCmdGetMacCounterInput  m_GetMacCounter;
        DtaIoctlIpCmdGetPhyRegisterInput  m_GetPhyRegister;
        DtaIoctlIpCmdSetPhyRegisterInput  m_SetPhyRegister;
        DtaIoctlIpCmdEnableLoopbackInput  m_EnableLoopback;
    } m_Data;
} DtaIoctlIpCmdInput;
ASSERT_SIZE(DtaIoctlIpCmdInput, 16)

// Ioctl Output data type
typedef struct _DtaIoctlIpCmdOutput {
    union {
        DtaIoctlIpCmdGetMacAddressOutput  m_MacAddress;
        DtaIoctlIpCmdGetSpeedOutput m_GetSpeed;
        DtaIoctlIpCmdGetAdminStatusOutput  m_GetAdminStatus;
        DtaIoctlIpCmdGetMacCounterOutput  m_GetMacCounter;
        DtaIoctlIpCmdGetPhyRegisterOutput  m_GetPhyRegister;
    } m_Data;
} DtaIoctlIpCmdOutput;
ASSERT_SIZE(DtaIoctlIpCmdOutput, 8)

#ifdef WINBUILD
    #define DTA_IOCTL_IP_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_IP_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlIpCmd {
        DtaIoctlIpCmdInput  m_Input;
        DtaIoctlIpCmdOutput  m_Output;
    } DtaIoctlIpCmd;

    #define DTA_IOCTL_IP_CMD  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_IP_CMD, DtaIoctlIpCmd)
#endif

    
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_IP_RX/TX_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Wrapper IOCTL for all IP RX and TX shared commands.
//
// DTA_IP_XX_CMD_ATTACH_CHANNEL
typedef struct _DtaIoctlIpXxCmdAttachChannelOutput {
    Int  m_Channel;
} DtaIoctlIpXxCmdAttachChannelOutput;
ASSERT_SIZE(DtaIoctlIpXxCmdAttachChannelOutput, 4)

// DTA_IP_XX_CMD_CHANNEL_DETACH
typedef struct _DtaIoctlIpXxCmdDetachChannelInput {
    Int  m_Channel;    
} DtaIoctlIpXxCmdDetachChannelInput;
ASSERT_SIZE(DtaIoctlIpXxCmdDetachChannelInput, 4)

// For status flag definitions: See NonIp Status flags
// DTA_IP_XX_CMD_CLEARFLAGS
typedef struct _DtaIoctlIpXxCmdClearFlagsInput {
    Int  m_Channel;
    Int  m_Flags;
} DtaIoctlIpXxCmdClearFlagsInput;
ASSERT_SIZE(DtaIoctlIpXxCmdClearFlagsInput, 8)

// DTA_IP_XX_CMD_GETFLAGS
typedef struct _DtaIoctlIpXxCmdGetFlagsInput {
    Int  m_Channel;   
} DtaIoctlIpXxCmdGetFlagsInput;
ASSERT_SIZE(DtaIoctlIpXxCmdGetFlagsInput, 4)

typedef struct _DtaIoctlIpXxCmdGetFlagsOutput {
    Int m_Flags;   
    Int m_LatchedFlags;
} DtaIoctlIpXxCmdGetFlagsOutput;
ASSERT_SIZE(DtaIoctlIpXxCmdGetFlagsOutput, 8)

// DTA_IP_XX_CMD_RESET
typedef struct _DtaIoctlIpXxCmdResetInput {
    Int  m_Channel;   
} DtaIoctlIpXxCmdResetInput;
ASSERT_SIZE(DtaIoctlIpXxCmdResetInput, 4)

// For control flag definitions: See NonIp Tx control flags
// DTA_IP_XX_CMD_SETTXCONTROL
typedef struct _DtaIoctlIpXxCmdSetControlInput {
    Int  m_Channel;
    Int  m_Control;
} DtaIoctlIpXxCmdSetControlInput;
ASSERT_SIZE(DtaIoctlIpXxCmdSetControlInput, 8)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_IP_TX_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Wrapper IOCTL for all IP TX specific commands.
//

// Command definitions
#define DTA_IP_TX_CMD_CHANNEL_ATTACH   1
#define DTA_IP_TX_CMD_CLEARFLAGS       2
#define DTA_IP_TX_CMD_CHANNEL_DETACH   3
#define DTA_IP_TX_CMD_GETFLAGS         4
#define DTA_IP_TX_CMD_RESET            5
#define DTA_IP_TX_CMD_SETCONTROL       6
#define DTA_IP_TX_CMD_SETIPPARS        7
#define DTA_IP_TX_CMD_WRITENDISPCKT    8

// DTA_IP_TX_CMD_WRITENDISPCKT
typedef struct _DtaIoctlIpTxCmdWriteNdisPcktInput {
    UInt  m_BufLen;
    UInt8  m_Buf[0];                 // Dynamic sized buffer
} DtaIoctlIpTxCmdWriteNdisPcktInput;
ASSERT_SIZE(DtaIoctlIpTxCmdWriteNdisPcktInput, 4)

// DTA_IP_TX_CMD_SETIPPARS
typedef struct _DtaIoctlIpTxCmdSetIpParsInput {
    Int  m_Channel;
    Int  m_VlanId;                   // VLAN ID of 2nd port
    Int  m_VlanPriority;             // VLAN Priority of 2nd port
    UInt16  m_DstPort;               // Destination port number 2nd port
    UInt16  m_SrcPort;               // Source port number os 2nd port
    UInt16  m_DstPort2;              // Destination port number of 1st port
    UInt16  m_DummyAlign1;
    Int  m_Mode;                     // Mode: DBLBUF/NORMAL
    Int  m_Flags;                    // Flag: IpV4/V6
    Int  m_Protocol;                 // Protocol: UDP/RTP
    Int  m_FecMode;                  // Error correction mode
    Int  m_DummyAlign2;
    UInt8  m_DstMac[6];              // Destination MAC address 2nd port
    UInt8  m_DstIp[16];              // Destination IP address 2nd port
    UInt8  m_SrcMac[6];              // Source MAC address of 2nd port
    UInt8  m_SrcIp[16];              // Source IP address of 2nd port
    UInt8  m_DummyAlign3[4];
} DtaIoctlIpTxCmdSetIpParsInput;
ASSERT_SIZE(DtaIoctlIpTxCmdSetIpParsInput, 88)

// Ioctl input data type
typedef struct _DtaIoctlIpTxCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;
    union {
        DtaIoctlIpXxCmdClearFlagsInput  m_ClearFlags;
        DtaIoctlIpXxCmdDetachChannelInput m_DetachChannel;
        DtaIoctlIpXxCmdGetFlagsInput m_GetFlags;
        DtaIoctlIpXxCmdResetInput m_Reset;
        DtaIoctlIpXxCmdSetControlInput m_SetTxControl;
        DtaIoctlIpTxCmdSetIpParsInput m_SetIpPars;
        DtaIoctlIpTxCmdWriteNdisPcktInput m_WriteNdisPckt;
    } m_Data;
} DtaIoctlIpTxCmdInput;
ASSERT_SIZE(DtaIoctlIpTxCmdInput, 96)

// Ioctl output data type
typedef struct _DtaIoctlIpTxCmdOutput {
    union {
        DtaIoctlIpXxCmdAttachChannelOutput m_AttachChannel;
        DtaIoctlIpXxCmdGetFlagsOutput m_GetFlags;
    } m_Data;
} DtaIoctlIpTxCmdOutput;
ASSERT_SIZE(DtaIoctlIpTxCmdOutput, 8)

#ifdef WINBUILD
    #define DTA_IOCTL_IP_TX_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_IP_TX_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlIpTxCmd {
        DtaIoctlIpTxCmdInput  m_Input;
        DtaIoctlIpTxCmdOutput  m_Output;
    } DtaIoctlIpTxCmd;

    #define DTA_IOCTL_IP_TX_CMD  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_IP_TX_CMD, \
                                                                       DtaIoctlIpTxCmd)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_IP_RX_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Wrapper IOCTL for all IP RX specific commands.
//

// Command definitions
#define DTA_IP_RX_CMD_CHANNEL_ATTACH   1
#define DTA_IP_RX_CMD_CLEARFLAGS       2
#define DTA_IP_RX_CMD_CHANNEL_DETACH   3
#define DTA_IP_RX_CMD_GETFLAGS         4
#define DTA_IP_RX_CMD_RESET            5
#define DTA_IP_RX_CMD_SETCONTROL       6
#define DTA_IP_RX_CMD_GETSTATUS        7
#define DTA_IP_RX_CMD_GETTSRATE        8
#define DTA_IP_RX_CMD_SETMODE          9
#define DTA_IP_RX_CMD_SETIPPARS        10
#define DTA_IP_RX_CMD_GETIPPARS        11
#define DTA_IP_RX_CMD_GETIPSTAT        12
#define DTA_IP_RX_CMD_SETIPPARS2       13
#define DTA_IP_RX_CMD_GETIPSTAT2       14
#define DTA_IP_RX_CMD_SETUPBUFFER      15
#define DTA_IP_RX_CMD_SETIPPARS3       16
#define DTA_IP_RX_CMD_GETIPPARS2       17
#define DTA_IP_RX_CMD_SETSRCFLT        18

// DTA_IP_RX_CMD_GETSTATUS
typedef struct _DtaIoctlIpRxCmdGetStatusInput {
    Int  m_Channel;
} DtaIoctlIpRxCmdGetStatusInput;
ASSERT_SIZE(DtaIoctlIpRxCmdGetStatusInput, 4)

typedef struct _DtaIoctlIpRxCmdGetStatusOutput {
    Int m_PacketSize;
    Int m_ClkDet;
} DtaIoctlIpRxCmdGetStatusOutput;
ASSERT_SIZE(DtaIoctlIpRxCmdGetStatusOutput, 8)

// DTA_IP_RX_CMD_GETTSRATE
typedef struct _DtaIoctlIpRxCmdGetTsRateInput {
    Int  m_Channel;
} DtaIoctlIpRxCmdGetTsRateInput;
ASSERT_SIZE(DtaIoctlIpRxCmdGetTsRateInput, 4)

typedef struct _DtaIoctlIpRxCmdGetTsRateOutput {
    Int m_TsRate;    
} DtaIoctlIpRxCmdGetTsRateOutput;
ASSERT_SIZE(DtaIoctlIpRxCmdGetTsRateOutput, 4)

// DTA_IP_RX_CMD_SETMODE
typedef struct _DtaIoctlIpRxCmdSetModeInput {
    Int  m_Channel;
    Int  m_RxMode;
} DtaIoctlIpRxCmdSetModeInput;
ASSERT_SIZE(DtaIoctlIpRxCmdSetModeInput, 8)

// Protocol flags
#define DTA_PROTO_UDP            0
#define DTA_PROTO_RTP            1
#define DTA_PROTO_AUTO           2
#define DTA_PROTO_UNKN           2

// Fec mode flags
#define DTA_FEC_DISABLE          0
#define DTA_FEC_2D               1      // FEC reconstruction
#define DTA_FEC_2D_M1            1      // Mode1: FECdT = DVBdT + .5 * DVBdT
#define DTA_FEC_2D_M2            2      // Mode2: FECdT = DVBdT

// Mode
#define DTA_IP_NORMAL            0
#define DTA_IP_TX_2022_7         1
#define DTA_IP_RX_2022_7         2

// Flags
#define DTA_IP_V4                1
#define DTA_IP_V6                2
#define DTA_IP_TX_MANSRCPORT     4
#define DTA_IP_RX_DIFFSRCPORTFEC 8

// DTA_IP_RX_CMD_SETIPPARS
typedef struct _DtaIoctlIpRxCmdSetIpParsInput {
    Int  m_Channel;
    UInt8  m_DstIp[4];              // Destination: IP address
    UInt16  m_DstPort;              // Destination: port number
    UInt8  m_SrcIp[4];              // Source: IP address
    UInt16  m_SrcPort;              // Source: port number
    Int  m_Protocol;                // Protocol: UDP/RTP
    Int  m_FecMode;                 // Error correction mode
} DtaIoctlIpRxCmdSetIpParsInput;
ASSERT_SIZE(DtaIoctlIpRxCmdSetIpParsInput, 24)

// DTA_IP_RX_CMD_SETIPPARS2
typedef struct _DtaIoctlIpRxCmdSetIpPars2Input {
    Int  m_Channel;
    // Primary link
    UInt8  m_DstIp[16];             // Destination: IP address
    UInt16  m_DstPort;              // Destination: port number
    UInt8  m_SrcIp[16];             // Source: IP address
    UInt16  m_SrcPort;              // Source: port number
    Int  m_VlanId;                  // VLAN ID
    // Redundant link
    UInt8  m_DstIp2[16];            // Destination: IP address
    UInt16  m_DstPort2;             // Destination: port number
    UInt8  m_SrcIp2[16];            // Source: IP address
    UInt16  m_SrcPort2;             // Source: port number
    Int  m_VlanId2;                 // VLAN ID
    // Options
    Int  m_Mode;                    // Normal/SMPTE_2022_7
    Int  m_Flags;                   // Control flags: IPv4/IPv6
    Int  m_Protocol;                // Protocol: UDP/RTP
    Int  m_FecMode;                 // Error correction mode
} DtaIoctlIpRxCmdSetIpPars2Input;
ASSERT_SIZE(DtaIoctlIpRxCmdSetIpPars2Input, 100)

// DTA_IP_RX_CMD_SETIPPARS3
typedef struct _DtaIoctlIpRxCmdSetIpPars3Input {
    Int  m_Channel;
    // Primary link
    UInt8  m_DstIp[16];             // Destination: IP address
    UInt16  m_DstPort;              // Destination: port number
    UInt8  m_SrcIp[16];             // Source: IP address
    UInt16  m_SrcPort;              // Source: port number
    Int  m_VlanId;                  // VLAN ID
    // Redundant link
    UInt8  m_DstIp2[16];            // Destination: IP address
    UInt16  m_DstPort2;             // Destination: port number
    UInt8  m_SrcIp2[16];            // Source: IP address
    UInt16  m_SrcPort2;             // Source: port number
    Int  m_VlanId2;                 // VLAN ID
    // Options
    Int  m_Mode;                    // Normal/SMPTE_2022_7
    Int  m_Flags;                   // Control flags: IPv4/IPv6
    Int  m_Protocol;                // Protocol: UDP/RTP
    Int  m_FecMode;                 // Error correction mode
    UInt16  m_NumSrcFlt;            // Number of source IP-filters. If <> 0, use new style
    UInt16  m_NumSrcFlt2;           // Number of source IP-filters. If <> 0, use new style
    // Profile
    Int  m_VidStd;                  // Video standard to receive. -1 = undefined
    Int  m_MaxBitrate;              // Maximal expected bitrate
    Int  m_MaxSkew;                 // Max. skew in SMPTE_2022-7
} DtaIoctlIpRxCmdSetIpPars3Input;
ASSERT_SIZE(DtaIoctlIpRxCmdSetIpPars3Input, 116)

// DTA_IP_RX_CMD_GETIPPARS
typedef struct _DtaIoctlIpRxCmdGetIpParsInput {
    Int  m_Channel;
} DtaIoctlIpRxCmdGetIpParsInput;
ASSERT_SIZE(DtaIoctlIpRxCmdGetIpParsInput, 4)
typedef struct _DtaIoctlIpRxCmdGetIpPars2Input {
    Int  m_Channel;
} DtaIoctlIpRxCmdGetIpPars2Input;
ASSERT_SIZE(DtaIoctlIpRxCmdGetIpPars2Input, 4)

typedef struct _DtaIoctlIpRxCmdGetIpParsOutput {
    Int  m_Protocol;                // Protocol: UDP/RTP/Unknown
    Int  m_FecNumRows;              // 'D' = #rows in FEC matrix
    Int  m_FecNumCols;              // 'L' = #columns in FEC matrix
    Int  m_NumTpPerIp;              // 0: Not detected
} DtaIoctlIpRxCmdGetIpParsOutput;
ASSERT_SIZE(DtaIoctlIpRxCmdGetIpParsOutput, 16)

typedef struct _DtaIoctlIpRxCmdGetIpPars2Output{
    Int  m_Protocol;                // Protocol: UDP/RTP/Unknown
    Int  m_FecNumRows;              // 'D' = #rows in FEC matrix
    Int  m_FecNumCols;              // 'L' = #columns in FEC matrix
    Int  m_NumTpPerIp;              // 0: Not detected
    Int  m_VidStd;                  // Detected video standard: DT_VIDSTD_xxx
} DtaIoctlIpRxCmdGetIpPars2Output;
ASSERT_SIZE(DtaIoctlIpRxCmdGetIpPars2Output, 20)

// DTA_IP_RX_CMD_GETIPSTAT
typedef struct _DtaIoctlIpRxCmdGetIpStatInput {
    Int  m_Channel;
} DtaIoctlIpRxCmdGetIpStatInput;
ASSERT_SIZE(DtaIoctlIpRxCmdGetIpStatInput, 4)

typedef struct _DtaIoctlIpRxCmdGetIpStatOutput {
    UInt  m_TotNumIPPackets;
    UInt  m_LostIPPacketsBeforeFec; // (BER before FEC)
    UInt  m_LostIPPacketsAfterFec;  // (BER after FEC)
    UInt  m_Reserved1;
    UInt  m_Reserved2;
} DtaIoctlIpRxCmdGetIpStatOutput;
ASSERT_SIZE(DtaIoctlIpRxCmdGetIpStatOutput, 20)

// DTA_IP_RX_CMD_GETIPSTAT2
typedef struct _DtaIoctlIpRxCmdGetIpStat2Input {
    Int  m_Channel;
} DtaIoctlIpRxCmdGetIpStat2Input;
ASSERT_SIZE(DtaIoctlIpRxCmdGetIpStat2Input, 4)

typedef struct _DtaIoctlIpRxCmdGetIpStat2Output {
    UInt  m_TotNumIPPackets;
    UInt  m_LostIPPacketsBeforeFec; // (BER before FEC)
    UInt  m_LostIPPacketsAfterFec;  // (BER after FEC)
    UInt  m_Spare;                  // Align;

    UInt  m_NumIpPacketsReceived[2];
    UInt  m_NumIpPacketsLost[2];

    UInt  m_BerNumIpPacketsMainSec;
    UInt  m_BerNumIpPacketsLostMainSec;
    UInt  m_BerNumIpPacketsMainMin;
    UInt  m_BerNumIpPacketsLostMainMin;
    
    UInt  m_BerNumIpPacketsSec[2];
    UInt  m_BerNumIpPacketsLostSec[2];
    UInt  m_BerNumIpPacketsMin[2];
    UInt  m_BerNumIpPacketsLostMin[2];
    
    UInt64A  m_DelayFactorSec[2];
    UInt64A  m_DelayFactorMin[2];
    UInt64A  m_MinIpatSec[2];
    UInt64A  m_MaxIpatSec[2];
    UInt64A  m_MinIpatMin[2];
    UInt64A  m_MaxIpatMin[2];

    Int64A  m_MinSkewSec;
    Int64A  m_MaxSkewSec;
    Int64A  m_MinSkewMin;
    Int64A  m_MaxSkewMin;
} DtaIoctlIpRxCmdGetIpStat2Output;
ASSERT_SIZE(DtaIoctlIpRxCmdGetIpStat2Output, 208)


// DTA_IP_RX_CMD_SETUPBUFFER
typedef struct _DtaIoctlIpRxCmdSetupBufferInput {
    Int  m_Channel;
    UInt  m_TsBufSize;              // Size of Ts buffer shared between DTAPI/DRIVER
                                    // (including wrap area, excluding header)
    UInt  m_IpBufSize;              // Size of IP buffer used only by driver for storing
                                    // RTP/FEC packets
    UInt  m_JumboPktSize;           // Max. size of jumbo packets
    UInt  m_MinPktDelay;            // Min. # packets to wait before copy to user
    UInt  m_MaxPktOutOfSync;        // Max. # packets out of sync
} DtaIoctlIpRxCmdSetupBufferInput;
ASSERT_SIZE(DtaIoctlIpRxCmdSetupBufferInput, 24)

// DTA_IP_RX_CMD_SETSRCFLT
typedef struct _DtaIoctlIpRxCmdSetSrcFltInput {
    Int  m_Channel;
    UInt  m_NumElements;
    UInt  m_FilterOffset;
    UInt  m_LastElement;
    UInt16  m_SrcPort[4];
    UInt8  m_SrcIp[4][16];
} DtaIoctlIpRxCmdSetSrcFltInput;
ASSERT_SIZE(DtaIoctlIpRxCmdSetSrcFltInput, 88)

// Ioctl input data type
typedef struct _DtaIoctlIpRxCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;
    union {
        DtaIoctlIpXxCmdClearFlagsInput  m_ClearFlags;
        DtaIoctlIpXxCmdDetachChannelInput  m_DetachChannel;
        DtaIoctlIpXxCmdGetFlagsInput  m_GetFlags;
        DtaIoctlIpXxCmdResetInput  m_Reset;
        DtaIoctlIpXxCmdSetControlInput  m_SetRxControl;
        DtaIoctlIpRxCmdGetStatusInput  m_GetStatus;
        DtaIoctlIpRxCmdGetTsRateInput  m_GetTsRate;
        DtaIoctlIpRxCmdSetModeInput   m_SetRxMode;
        DtaIoctlIpRxCmdGetIpParsInput  m_GetIpPars;
        DtaIoctlIpRxCmdGetIpPars2Input  m_GetIpPars2;
        DtaIoctlIpRxCmdSetIpParsInput  m_SetIpPars;
        DtaIoctlIpRxCmdGetIpStatInput  m_GetIpStat;
        DtaIoctlIpRxCmdGetIpStat2Input  m_GetIpStat2;
        DtaIoctlIpRxCmdSetIpPars2Input  m_SetIpPars2;
        DtaIoctlIpRxCmdSetIpPars3Input  m_SetIpPars3;
        DtaIoctlIpRxCmdSetupBufferInput  m_SetupBuffer;
        DtaIoctlIpRxCmdSetSrcFltInput  m_SetSrcFlt;
    } m_Data;
} DtaIoctlIpRxCmdInput;
ASSERT_SIZE(DtaIoctlIpRxCmdInput, 124)

// Ioctl output data type
typedef struct _DtaIoctlIpRxCmdOutput {
    union {
        DtaIoctlIpXxCmdAttachChannelOutput m_AttachChannel;
        DtaIoctlIpXxCmdGetFlagsOutput m_GetFlags;
        DtaIoctlIpRxCmdGetStatusOutput  m_GetStatus;
        DtaIoctlIpRxCmdGetTsRateOutput  m_GetTsRate;
        DtaIoctlIpRxCmdGetIpParsOutput  m_GetIpPars;
        DtaIoctlIpRxCmdGetIpPars2Output  m_GetIpPars2;
        DtaIoctlIpRxCmdGetIpStatOutput  m_GetIpStat;
        DtaIoctlIpRxCmdGetIpStat2Output  m_GetIpStat2;
    } m_Data;
} DtaIoctlIpRxCmdOutput;
ASSERT_SIZE(DtaIoctlIpRxCmdOutput, 208)

#ifdef WINBUILD
    #define DTA_IOCTL_IP_RX_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_IP_RX_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlIpRxCmd {
        DtaIoctlIpRxCmdInput  m_Input;
        DtaIoctlIpRxCmdOutput  m_Output;
    } DtaIoctlIpRxCmd;

    #define DTA_IOCTL_IP_RX_CMD  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_IP_RX_CMD, \
                                                                       DtaIoctlIpRxCmd)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_PHYMAC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Wrapper IOCTL for all PHY/MAC commands.
//

// Command definitions
#define DTA_PHYMAC_CMD_GET_MACADDRESS_PERM     1
#define DTA_PHYMAC_CMD_SET_MACADDRESS          2
#define DTA_PHYMAC_CMD_GET_MAX_MULTICAST_LIST  3
#define DTA_PHYMAC_CMD_SET_MULTICAST_LIST      4
#define DTA_PHYMAC_CMD_SET_PACKET_FILTER       5
#define DTA_PHYMAC_CMD_GET_PACKET_FILTER       6
#define DTA_PHYMAC_CMD_GET_COUNTER             7
#define DTA_PHYMAC_CMD_GET_LINKSTATUS          8
#define DTA_PHYMAC_CMD_GET_PHY_SPEED           9
#define DTA_PHYMAC_CMD_SET_PHY_SPEED           10
#define DTA_PHYMAC_CMD_REGISTER_CALLBACK       11
#define DTA_PHYMAC_CMD_UNREGISTER_CALLBACK     12
#define DTA_PHYMAC_CMD_RESET_NW_STATUS         13

// Generic phy defines
// Phy speed
#define DTA_PHY_SPEED_10_HALF          0x0
#define DTA_PHY_SPEED_10_FULL          0x1
#define DTA_PHY_SPEED_100_HALF         0x2
#define DTA_PHY_SPEED_100_FULL         0x3
#define DTA_PHY_SPEED_1000_MASTER      0x4
#define DTA_PHY_SPEED_1000_SLAVE       0x5
#define DTA_PHY_SPEED_NOT_SET          0x6     // Init
#define DTA_PHY_SPEED_AUTO_DETECT      0x6     // Set speed
#define DTA_PHY_SPEED_NO_LINK          0x6     // Get speed

// Phy link status
#define DTA_PHY_LINK_DOWN              0x0
#define DTA_PHY_LINK_UP                0x1
#define DTA_PHY_LINK_NOT_SET           0x2


// DTA_PHYMAC_CMD_GET_MACADDRESS_PERM
typedef struct _DtaIoctlPhyMacGetMacAddressPermOutput {
    UInt8  m_Address[6];
} DtaIoctlPhyMacGetMacAddressPermOutput;
ASSERT_SIZE(DtaIoctlPhyMacGetMacAddressPermOutput, 6)

// DTA_PHYMAC_CMD_SET_MACADDRESS
typedef struct _DtaIoctlPhyMacSetMacAddressInput {
    UInt8  m_Address[6];
} DtaIoctlPhyMacSetMacAddressInput;
ASSERT_SIZE(DtaIoctlPhyMacSetMacAddressInput, 6)

// DTA_PHYMAC_CMD_GET_MAX_MULTICAST_LIST
typedef struct _DtaIoctlPhyMacGetMaxMulticastListOutput {
    UInt  m_MaxListSize;
} DtaIoctlPhyMacGetMaxMulticastListOutput;
ASSERT_SIZE(DtaIoctlPhyMacGetMaxMulticastListOutput, 4)

// DTA_PHYMAC_CMD_SET_MULTICAST_LIST
typedef struct _DtaIoctlPhyMacSetMulticastListInput {
    UInt  m_NumItems;
    UInt8  m_Items[0][8];
} DtaIoctlPhyMacSetMulticastListInput;
ASSERT_SIZE(DtaIoctlPhyMacSetMulticastListInput, 4)

// DTA_PHYMAC_CMD_SET_PACKET_FILTER
// DTA_PHYMAC_CMD_GET_PACKET_FILTER
// Packet filter
#define DTA_MAC_FLT_NOTSET             0x00
#define DTA_MAC_FLT_DIRECTED           0x01
#define DTA_MAC_FLT_MULTICAST          0x02
#define DTA_MAC_FLT_BROADCAST          0x04
#define DTA_MAC_FLT_ALL_MULTICAST      0x08
#define DTA_MAC_FLT_PROMISCUOUS        0x10

typedef struct _DtaIoctlPhyMacSetPacketFilterInput {
    UInt  m_PacketFilter;
} DtaIoctlPhyMacSetPacketFilterInput;

typedef struct _DtaIoctlPhyMacGetPacketFilterOutput {
    UInt  m_PacketFilter;
} DtaIoctlPhyMacGetPacketFilterOutput;

// DTA_PHYMAC_CMD_GET_COUNTER
#define DTA_MAC_CNT_GEN_BYTES_RCV                  1
#define DTA_MAC_CNT_GEN_BYTES_XMIT                 2
#define DTA_MAC_CNT_GEN_XMIT_OK                    3
#define DTA_MAC_CNT_GEN_RCV_OK                     4
#define DTA_MAC_CNT_GEN_XMIT_ERROR                 5
#define DTA_MAC_CNT_GEN_RCV_ERROR                  6
#define DTA_MAC_CNT_GEN_RCV_NO_BUFFER              7
#define DTA_MAC_CNT_GEN_RCV_CRC_ERROR              8
#define DTA_MAC_CNT_GEN_TRANSMIT_QUEUE_LENGTH      9
#define DTA_MAC_CNT_802_3_RCV_ERROR_ALIGNMENT      10
#define DTA_MAC_CNT_802_3_XMIT_ONE_COLLISION       11
#define DTA_MAC_CNT_802_3_XMIT_MORE_COLLISIONS     12
#define DTA_MAC_CNT_802_3_XMIT_DEFERRED            13
#define DTA_MAC_CNT_802_3_XMIT_MAX_COLLISIONS      14
#define DTA_MAC_CNT_802_3_RCV_OVERRUN              15
#define DTA_MAC_CNT_802_3_XMIT_UNDERRUN            16
#define DTA_MAC_CNT_802_3_XMIT_HEARTBEAT_FAILURE   17
#define DTA_MAC_CNT_802_3_XMIT_TIMES_CRS_LOST      18
#define DTA_MAC_CNT_802_3_XMIT_LATE_COLLISIONS     19
#define DTA_MAC_CNT_GEN_RCV_HDR_ERROR              20

typedef struct _DtaIoctlPhyMacGetCounterInput {
    UInt  m_CounterId;
} DtaIoctlPhyMacGetCounterInput;

typedef struct _DtaIoctlPhyMacGetCounterOutput {
    UInt64A  m_Value;
} DtaIoctlPhyMacGetCounterOutput;

// DTA_PHYMAC_CMD_GET_PHY_SPEED
typedef struct _DtaIoctlPhyMacGetPhySpeedOutput {
    UInt  m_Speed;              // DTA_PHY_SPEED_ constants
} DtaIoctlPhyMacGetPhySpeedOutput;

// DTA_PHYMAC_CMD_SET_PHY_SPEED
typedef struct _DtaIoctlPhyMacSetPhySpeedInput {
    UInt  m_Speed;              // DTA_PHY_SPEED_ constants
} DtaIoctlPhyMacSetPhySpeedInput;

// DTA_PHYMAC_CMD_REGISTER_CALLBACK
typedef void  (*LinkStatFunc)(UInt PhyLinkStatus, void* pContext);
typedef struct _DtaIoctlPhyMacRegisterCallbackInput {
    void*  m_pContext;
    LinkStatFunc  m_LinkStatusCallback;
} DtaIoctlPhyMacRegisterCallbackInput;

// DTA_PHYMAC_CMD_UNREGISTER_CALLBACK
typedef struct _DtaIoctlPhyMacUnregisterCallbackInput {
    void*  m_pContext;
} DtaIoctlPhyMacUnregisterCallbackInput;


// Ioctl input data type
typedef struct _DtaIoctlPhyMacCmdInput {
    Int  m_Cmd;
    Int  m_IpPortIndex;
    union {
        DtaIoctlPhyMacSetMacAddressInput  m_SetMacAddress;
        DtaIoctlPhyMacSetMulticastListInput  m_SetMulticastList;
        DtaIoctlPhyMacSetPacketFilterInput  m_SetPacketFilter;
        DtaIoctlPhyMacGetCounterInput  m_GetCounter;
        DtaIoctlPhyMacSetPhySpeedInput  m_SetPhySpeed;
        DtaIoctlPhyMacRegisterCallbackInput  m_RegisterCallback;
        DtaIoctlPhyMacUnregisterCallbackInput  m_UnregisterCallback;
    } m_Data;
} DtaIoctlPhyMacCmdInput;

// Ioctl Output data type
typedef struct _DtaIoctlPhyMacCmdOutput {
    union {
        DtaIoctlPhyMacGetMacAddressPermOutput  m_GetMacAddressPerm;
        DtaIoctlPhyMacGetMaxMulticastListOutput  m_GetMaxMulticastList;
        DtaIoctlPhyMacGetPhySpeedOutput  m_GetPhySpeed;
        DtaIoctlPhyMacGetPacketFilterOutput  m_GetPacketFilter;
        DtaIoctlPhyMacGetCounterOutput  m_GetCounter;
    } m_Data;
} DtaIoctlPhyMacCmdOutput;

#ifdef WINBUILD
    #define DTA_IOCTL_PHYMAC_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_PHYMAC_CMD,\
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlPhyMacCmd {
        DtaIoctlPhyMacCmdInput  m_Input;
        DtaIoctlPhyMacCmdOutput  m_Output;
    } DtaIoctlPhyMacCmd;

    #define DTA_IOCTL_PHYMAC_CMD  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_PHYMAC_CMD, \
                                                                        DtaIoctlPhyMacCmd)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_IP_NW_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Wrapper IOCTL for all IP Network Driver commands.
//

// Command definitions
#define DTA_IP_NW_CMD_SET_DRIVER_VERSION       1
#define DTA_IP_NW_CMD_GET_SHARED_BUFFERS       2

// DTA_IP_NW_CMD_SET_DRIVER_VERSION
typedef struct _DtaIoctlIpNwSetDriverVersionInput{
    Int  m_Major;
    Int  m_Minor;
    Int  m_Micro;
    Int  m_Build;
} DtaIoctlIpNwSetDriverVersionInput;

#ifndef DTAPI   // DtEvent type not defined for DTAPI and struct only used in Driver
// DTA_IP_NW_CMD_GET_SHARED_BUFFERS
typedef struct _DtaIpNwSharedBufInfo {
    volatile UInt  m_ReadOffset;
    volatile UInt  m_WriteOffset;
    DtEvent  m_DataAvailableEvent;
    Int  m_DataAvailableCnt;
} DtaIpNwSharedBufInfo;
#endif //DTAPI

typedef struct _DtaIoctlIpNwGetSharedBuffersOutput {
    void*  m_pRxBuffer;
    void*  m_pTxBuffer;
    UInt  m_RxBufSize;
    UInt  m_TxBufSize;
    Bool  m_AlignedPayload;            // TRUE: 2 extra dummy bytes are before payload
} DtaIoctlIpNwGetSharedBuffersOutput;

// Ioctl input data type
typedef struct _DtaIoctlIpNwCmdInput {
    Int  m_Cmd;
    Int  m_IpPortIndex;
    union {
        DtaIoctlIpNwSetDriverVersionInput  m_SetDriverVersion;
    } m_Data;
} DtaIoctlIpNwCmdInput;

// Ioctl Output data type
typedef struct _DtaIoctlIpNwCmdOutput {
    union {
        DtaIoctlIpNwGetSharedBuffersOutput  m_GetSharedBuffers;
    } m_Data;
} DtaIoctlIpNwCmdOutput;

#ifdef WINBUILD
    #define DTA_IOCTL_IP_NW_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_IP_NW_CMD,\
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlIpNwCmd {
        DtaIoctlIpNwCmdInput  m_Input;
        DtaIoctlIpNwCmdOutput  m_Output;
    } DtaIoctlIpNwCmd;

    #define DTA_IOCTL_IP_NW_CMD  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_IP_NW_CMD, \
                                                                          DtaIoctlIpNwCmd)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_GET_TABLE +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Gets the table entries for given port and table name from the TableStore.
//
#define DTA_TABLE_NAME_MAX_SIZE  50


// Ioctl input data type
typedef struct _DtaIoctlGetTableInput {
    Int  m_PortIndex;                       // Port index
    char  m_Name[DTA_TABLE_NAME_MAX_SIZE];  // Name of table
    UInt  m_MaxNumEntries;                  // Max. number of entry's to store
} DtaIoctlGetTableInput;
ASSERT_SIZE(DtaIoctlGetTableInput, 60)

typedef struct _DtaIoctlGetTable2Input {
    Int  m_TypeNumber;                      // Type number (-1 = get for current device)
    Int  m_SubDvc;                          // Sub-device (-1 = get for current device)
    Int  m_HardwareRevision;                // Hardware revision
    Int  m_FirmwareVersion;                 // Firmware version
    Int  m_PortIndex;                       // Port index
    char  m_Name[DTA_TABLE_NAME_MAX_SIZE];  // Name of table
    UInt  m_MaxNumEntries;                  // Max. number of entry's to store
    Int  m_DtapiMaj;                        // DTAPI major version
    Int  m_DtapiMin;                        // DTAPI minor version
    Int  m_DtapiBugfix;                     // DTAPI bug fix version
} DtaIoctlGetTable2Input;
ASSERT_SIZE(DtaIoctlGetTable2Input, 88)

typedef struct _DtaIoctlGetTable3Input {
    Int  m_TypeNumber;                      // Type number (-1 = get for current device)
    Int  m_SubDvc;                          // Sub-device (-1 = get for current device)
    Int  m_HardwareRevision;                // Hardware revision
    Int  m_FirmwareVersion;                 // Firmware version
    Int  m_FirmwareVariant;                 // Firmware variant
    Int  m_PortIndex;                       // Port index
    char  m_Name[DTA_TABLE_NAME_MAX_SIZE];  // Name of table
    UInt  m_MaxNumEntries;                  // Max. number of entry's to store
    Int  m_DtapiMaj;                        // DTAPI major version
    Int  m_DtapiMin;                        // DTAPI minor version
    Int  m_DtapiBugfix;                     // DTAPI bug fix version
} DtaIoctlGetTable3Input;
ASSERT_SIZE(DtaIoctlGetTable3Input, 92)

// Ioctl output data type
typedef struct _DtaIoctlGetTableOutput {
    UInt  m_NumEntries;
    DtTableEntry  m_TableEntry[0];
} DtaIoctlGetTableOutput;
ASSERT_SIZE(DtaIoctlGetTableOutput, 8)

#ifdef WINBUILD
    #define DTA_IOCTL_GET_TABLE  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_GET_TABLE, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetTableData {
        DtaIoctlGetTableInput  m_Input;
        DtaIoctlGetTableOutput  m_Output;
    } DtaIoctlGetTableData;

    #define DTA_IOCTL_GET_TABLE  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_GET_TABLE, \
                                                                     DtaIoctlGetTableData)
#endif

#ifdef WINBUILD
    #define DTA_IOCTL_GET_TABLE2  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_GET_TABLE2, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetTable2Data {
        DtaIoctlGetTable2Input  m_Input;
        DtaIoctlGetTableOutput  m_Output;
    } DtaIoctlGetTable2Data;

    #define DTA_IOCTL_GET_TABLE2  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_GET_TABLE2, \
                                                                    DtaIoctlGetTable2Data)
#endif

#ifdef WINBUILD
    #define DTA_IOCTL_GET_TABLE3  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_GET_TABLE3, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetTable3Data {
        DtaIoctlGetTable3Input  m_Input;
        DtaIoctlGetTableOutput  m_Output;
    } DtaIoctlGetTable3Data;

    #define DTA_IOCTL_GET_TABLE3  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_GET_TABLE3, \
                                                                    DtaIoctlGetTable3Data)
#endif
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_SET_MEMORY_TESTMODE +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Enter testmode for a memory test.
//
typedef struct _DtaIoctlSetMemoryTestModeInput {
    Int  m_On;    
} DtaIoctlSetMemoryTestModeInput;
ASSERT_SIZE(DtaIoctlSetMemoryTestModeInput, 4)

#ifdef WINBUILD
    #define DTA_IOCTL_SET_MEMORY_TESTMODE CTL_CODE(DTA_DEVICE_TYPE, \
                                                        FUNC_DTA_SET_MEMORY_TESTMODE, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlSetMemoryTestMode {
        DtaIoctlSetMemoryTestModeInput  m_On;
    } DtaIoctlSetMemoryTestMode;

    #define DTA_IOCTL_SET_MEMORY_TESTMODE  _IOWR(DTA_IOCTL_MAGIC, \
                              FUNC_DTA_SET_MEMORY_TESTMODE, DtaIoctlSetMemoryTestMode)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_REBOOT_FIRMWARE +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Reboots the firmware
//
typedef struct _DtaIoctlRebootFirmwareInput {
    Int  m_Delay;
    Int  m_Reserved2;
} DtaIoctlRebootFirmwareInput;
ASSERT_SIZE(DtaIoctlRebootFirmwareInput, 8)

#ifdef WINBUILD
    #define DTA_IOCTL_REBOOT_FIRMWARE  CTL_CODE(DTA_DEVICE_TYPE, \
                                                        FUNC_DTA_REBOOT_FIRMWARE, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlRemootFirmware {
        DtaIoctlRebootFirmwareInput  m_Input;
    } DtaIoctlRebootFirmware;

    #define DTA_IOCTL_REBOOT_FIRMWARE  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_REBOOT_FIRMWARE, \
                                                                   DtaIoctlRebootFirmware)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_GET_NWDRIVER_VERSION +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Gets the DTA network driver version number
//

typedef struct _DtaIoctlGetNwDriverVersionOutput {
    Int  m_Major;
    Int  m_Minor;
    Int  m_Micro;
    Int  m_Build;
} DtaIoctlGetNwDriverVersionOutput;
ASSERT_SIZE(DtaIoctlGetNwDriverVersionOutput, 16)

#ifdef WINBUILD
    #define DTA_IOCTL_GET_NWDRIVER_VERSION  CTL_CODE(DTA_DEVICE_TYPE, \
                         FUNC_DTA_GET_NWDRIVER_VERSION, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetNwDriverVersionData {
        DtaIoctlGetNwDriverVersionOutput  m_Output;
    } DtaIoctlGetNwDriverVersionData;

    #define DTA_IOCTL_GET_NWDRIVER_VERSION  _IOR(DTA_IOCTL_MAGIC, \
                            FUNC_DTA_GET_NWDRIVER_VERSION, DtaIoctlGetNwDriverVersionData)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_GET_STR_PROPERTY +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Gets a device or port string property value and type from the DtaProperyStore.
//

// Ioctl input data type
typedef struct _DtaIoctlGetStrPropertyInput {
    Int  m_TypeNumber;                      // Type number (-1 = get for current device)
    Int  m_HardwareRevision;                // Hardware revision
    Int  m_FirmwareVersion;                 // Firmware version
    Int  m_PortIndex;                       // Port index
    char  m_Name[PROPERTY_NAME_MAX_SIZE];   // Name of property
} DtaIoctlGetStrPropertyInput;
ASSERT_SIZE(DtaIoctlGetStrPropertyInput, 68)

typedef struct _DtaIoctlGetStrProperty2Input {
    Int  m_TypeNumber;                      // Type number (-1 = get for current device)
    Int  m_SubDvc;                          // Sub-device (-1 = get for current device)
    Int  m_HardwareRevision;                // Hardware revision
    Int  m_FirmwareVersion;                 // Firmware version
    Int  m_PortIndex;                       // Port index
    char  m_Name[PROPERTY_NAME_MAX_SIZE];   // Name of property
    Int  m_DtapiMaj;                        // DTAPI major version
    Int  m_DtapiMin;                        // DTAPI minor version
    Int  m_DtapiBugfix;                     // DTAPI bug fix version
} DtaIoctlGetStrProperty2Input;
ASSERT_SIZE(DtaIoctlGetStrProperty2Input, 84)

typedef struct _DtaIoctlGetStrProperty3Input {
    Int  m_TypeNumber;                      // Type number (-1 = get for current device)
    Int  m_SubDvc;                          // Sub-device (-1 = get for current device)
    Int  m_HardwareRevision;                // Hardware revision
    Int  m_FirmwareVersion;                 // Firmware version
    Int  m_FirmwareVariant;                 // Firmware variant
    Int  m_PortIndex;                       // Port index
    char  m_Name[PROPERTY_NAME_MAX_SIZE];   // Name of property
    Int  m_DtapiMaj;                        // DTAPI major version
    Int  m_DtapiMin;                        // DTAPI minor version
    Int  m_DtapiBugfix;                     // DTAPI bug fix version
} DtaIoctlGetStrProperty3Input;
ASSERT_SIZE(DtaIoctlGetStrProperty3Input, 88)

// Ioctl output data type
typedef struct _DtaIoctlGetStrPropertyOutput {
    DtPropertyScope  m_Scope;               // Scope of property
    char  m_Str[PROPERTY_STR_MAX_SIZE];     // The requested string
} DtaIoctlGetStrPropertyOutput;
ASSERT_SIZE(DtaIoctlGetStrPropertyOutput, 100)

#ifdef WINBUILD
    #define DTA_IOCTL_GET_STR_PROPERTY  CTL_CODE(DTA_DEVICE_TYPE,                        \
                                                        FUNC_DTA_GET_STR_PROPERTY,       \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetStrPropertyData {
        DtaIoctlGetStrPropertyInput  m_Input;
        DtaIoctlGetStrPropertyOutput  m_Output;
    } DtaIoctlGetStrPropertyData;

    #define DTA_IOCTL_GET_STR_PROPERTY  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_GET_STR_PROPERTY,\
                                                               DtaIoctlGetStrPropertyData)
#endif

#ifdef WINBUILD
    #define DTA_IOCTL_GET_STR_PROPERTY2  CTL_CODE(DTA_DEVICE_TYPE,                       \
                                                        FUNC_DTA_GET_STR_PROPERTY2,      \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetStrProperty2Data {
        DtaIoctlGetStrProperty2Input  m_Input;
        DtaIoctlGetStrPropertyOutput  m_Output;
    } DtaIoctlGetStrProperty2Data;

    #define DTA_IOCTL_GET_STR_PROPERTY2  _IOWR(DTA_IOCTL_MAGIC,                          \
                                  FUNC_DTA_GET_STR_PROPERTY2, DtaIoctlGetStrProperty2Data)
#endif

#ifdef WINBUILD
    #define DTA_IOCTL_GET_STR_PROPERTY3  CTL_CODE(DTA_DEVICE_TYPE,                       \
                                                        FUNC_DTA_GET_STR_PROPERTY3,      \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetStrProperty3Data {
        DtaIoctlGetStrProperty3Input  m_Input;
        DtaIoctlGetStrPropertyOutput  m_Output;
    } DtaIoctlGetStrProperty3Data;

    #define DTA_IOCTL_GET_STR_PROPERTY3  _IOWR(DTA_IOCTL_MAGIC,                          \
                                  FUNC_DTA_GET_STR_PROPERTY3, DtaIoctlGetStrProperty3Data)
#endif

    //+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_MATRIX_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Wrapper IOCTL for all Matrix-API (HD-SDI) related commands
//

// Command definitions
#define  DTA_MATRIX_CMD_WAIT_FRAME        1     // Waits for a specific or simple the
                                                // next frame to be received/transmitted
#define  DTA_MATRIX_CMD_GET_SYNC_INFO     2     // Get SOF sync info
#define  DTA_MATRIX_CMD_START             3     // Start reception/transmission
#define  DTA_MATRIX_CMD_STOP              4     // Stop reception/transmission
#define  DTA_MATRIX_CMD_GET_CURR_FRAME    5     // Get the frame currently being
                                                // received/transmitted
#define  DTA_MATRIX_CMD_DMA_WRITE         6     // Write DMA
#define  DTA_MATRIX_CMD_DMA_READ          7     // Read DMA
#define  DTA_MATRIX_CMD_ATTACH_TO_ROW     8     // Attach the port to the specified row
#define  DTA_MATRIX_CMD_DETECT_VIDSTD     9     // Determine video standard at input
#define  DTA_MATRIX_CMD_GET_FIFOLOAD      10
#define  DTA_MATRIX_CMD_GET_FIFOSIZE      11
#define  DTA_MATRIX_CMD_GET_FIFOSIZE_MAX  12
#define  DTA_MATRIX_CMD_SET_FIFOSIZE      13
#define  DTA_MATRIX_CMD_SET_ASI_CTRL      14
#define  DTA_MATRIX_CMD_GET_BUF_CONFIG    15    // Returns the buffer configuration
#define  DTA_MATRIX_CMD_GET_REQ_DMA_SIZE  16    // Returns the minimum req DMA buffer size
#define  DTA_MATRIX_CMD_GET_FRM_INFO      17    // Get the timing on a frame in the buffer
#define  DTA_MATRIX_CMD_GET_FRM_PROPS     18    // Get the frame properties
#define  DTA_MATRIX_CMD_START_MAN         19    // Start reception/transmission with
                                                // control over which buffers to use
#define  DTA_MATRIX_CMD_SET_NEXT_FRAME    20    // Set the next frame that will be
                                                // received/transmitted
#define  DTA_MATRIX_CMD_GET_FRM_INFO2     21    // Get the timing on a frame in the buffer
#define  DTA_MATRIX_CMD_WAIT_FRAME2       22    // Waits for a specific or simple the
                                                // next frame to be received/transmitted
#define  DTA_MATRIX_CMD_START2            23    // Start reception/transmission
#define  DTA_MATRIX_CMD_START3            24    // Start reception/transmission
#define  DTA_MATRIX_CMD_WAIT_FRAME3       25    // Waits for a specific or simple the
                                                // next frame to be received/transmitted
#define  DTA_MATRIX_CMD_GET_VPID          26    // Get detected VPID

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_WAIT_FRAME -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtaIoctlMatrixCmdWaitFrameInput {
    Int64A  m_FrmIntCnt;        // Frame interrupt counter to wait for (-1 signals wait
                                // for next frame interrupt)
    Int  m_Timeout;             // Max wait time (in ms)
} DtaIoctlMatrixCmdWaitFrameInput;
ASSERT_SIZE(DtaIoctlMatrixCmdWaitFrameInput, 16)

typedef struct _DtaIoctlMatrixCmdWaitFrameOutput {
    Int64A  m_Frame;            // The actual frame received/transmitted
} DtaIoctlMatrixCmdWaitFrameOutput;
ASSERT_SIZE(DtaIoctlMatrixCmdWaitFrameOutput, 8)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_GET_SYNC_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtaIoctlMatrixCmdGetSyncInfoOutput {
    Int  m_IsValid;             // Synchronisation info is valid
                                // Do NOT use Bool (defined different in DTAPI and Driver)
    Int64A  m_SofFrame;         // The master SOF frame
    Int  m_NumChannels;         // Number of channels which are valid
    struct  _SOF_CHANNEL_INFO
    {
        Int  m_PortIndex;       // Port index
        Int64A  m_SofFrame;     // Latched channel SOF frame counter
        Int  m_SofLine;         // Latched channel line number counter
    }  m_Channels[16];
} DtaIoctlMatrixCmdGetSyncInfoOutput;
ASSERT_SIZE(DtaIoctlMatrixCmdGetSyncInfoOutput, 24+16*24)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_START -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtaIoctlMatrixCmdStartInput {
    Int64A  m_StartFrame;       // First frame to transmit/receive
} DtaIoctlMatrixCmdStartInput;
ASSERT_SIZE(DtaIoctlMatrixCmdStartInput, 8)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_STOP -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// THIS COMMAND HAS NO EXTRA PARAMETERS 

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_GET_CURR_FRAME -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtaIoctlMatrixCmdGetCurrFrameOutput {
    Int64A  m_CurrFrame;        // current frame
} DtaIoctlMatrixCmdGetCurrFrameOutput;
ASSERT_SIZE(DtaIoctlMatrixCmdGetCurrFrameOutput, 8)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_DMA_WRITE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtaIoctlMatrixCmdDmaCommonPars
{
    Int64A  m_Frame;            // Frame to transfer
    Int  m_Line;                // First line to transfer
    Int  m_NumLines;            // Number of lines to transfer
    Int  m_TrCmd;               // Transfer type
    Int  m_DataFormat;          // Format of data (8-, 10-, 16-bit)
    Int  m_RgbMode;             // RGB mode
    Int  m_SymFlt;              // Symbol filter mode
    Int  m_Scaling;             // Scaling mode
    Int  m_AncFlt;              // Anc filter mode
    Int  m_Stride;              // -1 means no stride
} DtaIoctlMatrixCmdDmaCommonPars;

typedef struct _DtaIoctlMatrixCmdWriteDmaInput {
    DtaIoctlMatrixCmdDmaCommonPars  m_Common;
#ifdef LINBUILD
    UInt64A  m_BufferAddr;
    Int  m_NumBytesToWrite;
#endif
} DtaIoctlMatrixCmdWriteDmaInput;
#ifdef LINBUILD
ASSERT_SIZE(DtaIoctlMatrixCmdWriteDmaInput, 64)
#else
ASSERT_SIZE(DtaIoctlMatrixCmdWriteDmaInput, 48)
#endif

#ifdef LINBUILD
typedef struct _DtaIoctlMatrixCmdWriteDmaOutput {
    Int  m_NumBytesWritten;
} DtaIoctlMatrixCmdWriteDmaOutput;
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_DMA_READ -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtaIoctlMatrixCmdReadDmaInput {
    DtaIoctlMatrixCmdDmaCommonPars  m_Common;
#ifdef LINBUILD
    UInt64A  m_BufferAddr;
    Int  m_NumBytesToRead;
#endif
} DtaIoctlMatrixCmdReadDmaInput;
#ifdef LINBUILD
ASSERT_SIZE(DtaIoctlMatrixCmdReadDmaInput, 64)
#else
ASSERT_SIZE(DtaIoctlMatrixCmdReadDmaInput, 48)
#endif

#ifdef LINBUILD
typedef struct _DtaIoctlMatrixCmdReadDmaOutput {
    Int  m_NumBytesRead;
} DtaIoctlMatrixCmdReadDmaOutput;
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_ATTACH_TO_ROW -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtaIoctlMatrixCmdAttachToRowInput {
    Int  m_RowIdx;              // Matrix row the port should be associated with
} DtaIoctlMatrixCmdAttachToRowInput;
ASSERT_SIZE(DtaIoctlMatrixCmdAttachToRowInput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_DETECT_VIDSTD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// NO ADDITIONAL INPUT PARAMETERS

typedef struct _DtaIoctlMatrixCmdDetectVidStdOutput {
    Int  m_VidStd;              // Detected video standard
} DtaIoctlMatrixCmdDetectVidStdOutput;
ASSERT_SIZE(DtaIoctlMatrixCmdDetectVidStdOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_GET_FIFOLOAD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// NO ADDITIONAL INPUT PARAMETERS

typedef struct _DtaIoctlMatrixCmdGetFifoLoadOutput {
    Int  m_FifoLoad;
} DtaIoctlMatrixCmdGetFifoLoadOutput;
ASSERT_SIZE(DtaIoctlMatrixCmdGetFifoLoadOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_GET_FIFOSIZE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// NO ADDITIONAL INPUT PARAMETERS

typedef struct _DtaIoctlMatrixCmdGetFifoSizeOutput {
    Int  m_FifoSize;
} DtaIoctlMatrixCmdGetFifoSizeOutput;
ASSERT_SIZE(DtaIoctlMatrixCmdGetFifoSizeOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_GET_FIFOSIZE_MAX -.-.-.-.-.-.-.-.-.-.-.-.-.-.

// NO ADDITIONAL INPUT PARAMETERS

typedef struct _DtaIoctlMatrixCmdGetFifoSizeMaxOutput {
    Int  m_FifoSize;
} DtaIoctlMatrixCmdGetFifoSizeMaxOutput;
ASSERT_SIZE(DtaIoctlMatrixCmdGetFifoSizeMaxOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_SET_FIFOSIZE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtaIoctlMatrixCmdSetFifoSizeInput {
    Int  m_FifoSize;
} DtaIoctlMatrixCmdSetFifoSizeInput;
ASSERT_SIZE(DtaIoctlMatrixCmdSetFifoSizeInput, 4)

// NO ADDITIONAL OUTPUT PARAMETERS


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_SET_ASI_CTRL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtaIoctlMatrixCmdSetAsiCtrlInput {
    Int  m_AsiCtrl;
} DtaIoctlMatrixCmdSetAsiCtrlInput;
ASSERT_SIZE(DtaIoctlMatrixCmdSetAsiCtrlInput, 4)

// NO ADDITIONAL OUTPUT PARAMETERS

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_GET_BUF_CONFIG -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// NO ADDITIONAL INPUT PARAMETERS

typedef struct _DtaIoctlMatrixCmdGetBufConfigOutput {
    Int  m_VidStd;              // Video standard the buffer is configured for
    Int  m_NumFrames;           // # of frames that can be stored in buffer    
} DtaIoctlMatrixCmdGetBufConfigOutput;
ASSERT_SIZE(DtaIoctlMatrixCmdGetBufConfigOutput, 8)
    
//-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_GET_REQ_DMA_SIZE -.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef DtaIoctlMatrixCmdDmaCommonPars  DtaIoctlMatrixCmdGetReqDmaSizeInput;
ASSERT_SIZE(DtaIoctlMatrixCmdGetReqDmaSizeInput, 48)

typedef struct _DtaIoctlMatrixCmdGetReqDmaSizeOutput {
    Int  m_ReqSize;     // Minimum size of the DMA buffer
} DtaIoctlMatrixCmdGetReqDmaSizeOutput;
ASSERT_SIZE(DtaIoctlMatrixCmdGetReqDmaSizeOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_GET_FRM_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtaIoctlMatrixCmdGetFrmInfoInput {
    Int64A  m_Frame;            // Frame number to get timestamp from
} DtaIoctlMatrixCmdGetFrmInfoInput;
ASSERT_SIZE(DtaIoctlMatrixCmdGetFrmInfoInput, 8)

typedef struct _DtaIoctlMatrixCmdGetFrmInfoOutput {
    Int64A  m_RfClkLatched;     // Latched version of ref clock at start of frame
} DtaIoctlMatrixCmdGetFrmInfoOutput;
ASSERT_SIZE(DtaIoctlMatrixCmdGetFrmInfoOutput, 8)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_GET_FRM_PROPS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtaIoctlMatrixCmdGetFrmPropsInput {
    Int  m_VidStd;              // Video standard for which to get properties
} DtaIoctlMatrixCmdGetFrmPropsInput;
ASSERT_SIZE(DtaIoctlMatrixCmdGetFrmPropsInput, 4)

typedef DtAvFrameProps DtaIoctlMatrixCmdGetFrmPropsOutput;
ASSERT_SIZE(DtaIoctlMatrixCmdGetFrmPropsOutput, 80)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_SET_NEXT_FRAME -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtaIoctlMatrixCmdSetNextFrmInput {
    Int64A  m_NextFrame;        // Next frame to transmit/receive
} DtaIoctlMatrixCmdSetNextFrmInput;
ASSERT_SIZE(DtaIoctlMatrixCmdSetNextFrmInput, 8)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_GET_FRM_INFO2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtaIoctlMatrixCmdGetFrmInfo2Output {
    Int64A  m_RfClkLatchedStart; // Latched version of ref clock at start of frame
    Int64A  m_RfClkLatchedEnd;   // Latched version of ref clock at end of frame
} DtaIoctlMatrixCmdGetFrmInfo2Output;
ASSERT_SIZE(DtaIoctlMatrixCmdGetFrmInfo2Output, 16)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_WAIT_FRAME2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtaIoctlMatrixCmdWaitFrame2Output {
    Int64A  m_Frame;             // The actual frame received/transmitted
    Int64A  m_RfClkLatchedStart; // Latched version of ref clock at start of frame
    Int64A  m_RfClkLatchedEnd;   // Latched version of ref clock at end of frame
    Int64A  m_FrmIntCnt;         // Number of frame interrupts since driver start
} DtaIoctlMatrixCmdWaitFrame2Output;
ASSERT_SIZE(DtaIoctlMatrixCmdWaitFrame2Output, 32)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_START2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#define DTA_MATRIX_STARTFLAGS_MANUAL            1
#define DTA_MATRIX_STARTFLAGS_FORCE_RESTART     2

typedef struct _DtaIoctlMatrixCmdStart2Input {
    Int64A  m_StartFrame;       // First frame to transmit/receive
    Int  m_StartFlags;          // DTA_MATRIX_STARTFLAGS_*
    UInt32  m_Vpid;             // Vpid to insert (output only), 0=driver default
} DtaIoctlMatrixCmdStart2Input;
ASSERT_SIZE(DtaIoctlMatrixCmdStart2Input, 16)

typedef struct _DtaIoctlMatrixCmdStart3Input {
    Int64A  m_StartFrame;       // First frame to transmit/receive
    Int  m_StartFlags;          // DTA_MATRIX_STARTFLAGS_*
    UInt32  m_Vpid;             // Vpid to insert in 2nd link (3G lvlB), 0=driver default
    UInt32  m_Vpid2;            // Vpid to insert, 0=driver default
    Int  m_ExtraPixelDelay;     // Extra delay for output, in #pixels, relative to 
                                // the GENREF TOF. NOTE: must be between -32000..32000
} DtaIoctlMatrixCmdStart3Input;
ASSERT_SIZE(DtaIoctlMatrixCmdStart3Input, 24)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_WAIT_FRAME3 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtaIoctlMatrixCmdWaitFrame3Output {
    Int64A  m_Frame;             // The actual frame received/transmitted
    Int64A  m_RfClkLatchedStart; // Latched version of ref clock at start of frame
    Int64A  m_RfClkLatchedEnd;   // Latched version of ref clock at end of frame
    Int64A  m_FrmIntCnt;         // Number of frame interrupts since driver start
    Int  m_TopHalf;              // 1 for first part of 3g level B frame, 0 otherwise
} DtaIoctlMatrixCmdWaitFrame3Output;
ASSERT_SIZE(DtaIoctlMatrixCmdWaitFrame3Output, 40)

#define DTA_3GB_BOTTOM  0        // Bottom half of 3G level B frame
#define DTA_3GB_TOP     1        // Top half of 3G level B frame
#define DTA_3GB_UNKNOWN 2        // Either top or bottom part of 3G level B frame

    
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_GET_VPID -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtaIoctlMatrixCmdGetVpidOutput {
    UInt  m_Vpid;               // Detected VPID
    UInt  m_Vpid2;              // VPID in link 2 for 3g level b signals
} DtaIoctlMatrixCmdGetVpidOutput;
ASSERT_SIZE(DtaIoctlMatrixCmdGetVpidOutput, 8)

// Ioctl input data type
typedef struct _DtaIoctlMatrixCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;           // -1 signals a device level command
    union {
        DtaIoctlMatrixCmdWaitFrameInput  m_WaitFrame;
        DtaIoctlMatrixCmdStartInput  m_Start;
        DtaIoctlMatrixCmdWriteDmaInput  m_DmaWrite;
        DtaIoctlMatrixCmdReadDmaInput  m_DmaRead;
        DtaIoctlMatrixCmdAttachToRowInput  m_AttachToRow;
        DtaIoctlMatrixCmdSetFifoSizeInput  m_SetFifoSize;
        DtaIoctlMatrixCmdSetAsiCtrlInput  m_SetAsiCtrl;
        DtaIoctlMatrixCmdGetReqDmaSizeInput  m_GetReqDmaSize;
        DtaIoctlMatrixCmdGetFrmInfoInput  m_GetFrmInfo;
        DtaIoctlMatrixCmdGetFrmPropsInput  m_GetFrmProps;
        DtaIoctlMatrixCmdSetNextFrmInput  m_SetNextFrm;
        DtaIoctlMatrixCmdStart2Input  m_Start2;
        DtaIoctlMatrixCmdStart3Input  m_Start3;
    } m_Data;
} DtaIoctlMatrixCmdInput;
#ifdef LINBUILD
ASSERT_SIZE(DtaIoctlMatrixCmdInput, 72)
#else
ASSERT_SIZE(DtaIoctlMatrixCmdInput, 56)
#endif

// Ioctl output data type
typedef struct _DtaIoctlMatrixCmdOutput {
    union {
        DtaIoctlMatrixCmdWaitFrameOutput  m_WaitFrame;
        DtaIoctlMatrixCmdGetSyncInfoOutput  m_SyncInfo;
        DtaIoctlMatrixCmdGetCurrFrameOutput  m_GetCurrFrame;
        DtaIoctlMatrixCmdDetectVidStdOutput  m_DetVidStd;
#ifdef LINBUILD
        DtaIoctlMatrixCmdWriteDmaOutput  m_DmaWrite;
        DtaIoctlMatrixCmdReadDmaOutput  m_DmaRead;
#endif
        DtaIoctlMatrixCmdGetFifoLoadOutput  m_GetFifoLoad;
        DtaIoctlMatrixCmdGetFifoSizeOutput  m_GetFifoSize;
        DtaIoctlMatrixCmdGetFifoSizeMaxOutput  m_GetFifoSizeMax;
        DtaIoctlMatrixCmdGetReqDmaSizeOutput  m_GetReqDmaSize;
        DtaIoctlMatrixCmdGetBufConfigOutput  m_GetBufConfig;
        DtaIoctlMatrixCmdGetFrmInfoOutput  m_GetFrmInfo;
        DtaIoctlMatrixCmdGetFrmPropsOutput  m_GetFrmProps;
        DtaIoctlMatrixCmdGetFrmInfo2Output  m_GetFrmInfo2;
        DtaIoctlMatrixCmdWaitFrame2Output  m_WaitFrame2;
        DtaIoctlMatrixCmdWaitFrame3Output  m_WaitFrame3;
        DtaIoctlMatrixCmdGetVpidOutput  m_GetVpid;
    } m_Data;
} DtaIoctlMatrixCmdOutput;
ASSERT_SIZE(DtaIoctlMatrixCmdOutput, 408)

#ifdef WINBUILD
    #define DTA_IOCTL_MATRIX_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_MATRIX_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlMatrixCmd {
        DtaIoctlMatrixCmdInput  m_Input;
        DtaIoctlMatrixCmdOutput  m_Output;
    } DtaIoctlMatrixCmd;

    #define DTA_IOCTL_MATRIX_CMD  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_MATRIX_CMD, \
                                                                        DtaIoctlMatrixCmd)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_GET_DEV_SUBTYPE +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Gets the device subtype
//
typedef struct _DtaIoctlGetDevSubTypeOutput {
    Int  m_SubType;             // Device subtype (-1=undefined, 0=none, 1=A, ...)
} DtaIoctlGetDevSubTypeOutput;
ASSERT_SIZE(DtaIoctlGetDevSubTypeOutput, 4)

#ifdef WINBUILD
    #define DTA_IOCTL_GET_DEV_SUBTYPE  CTL_CODE(DTA_DEVICE_TYPE, \
                              FUNC_DTA_GET_DEV_SUBTYPE, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetDevSubTypeData {
        DtaIoctlGetDevSubTypeOutput  m_Output;
    } DtaIoctlGetDevSubTypeData;

    #define DTA_IOCTL_GET_DEV_SUBTYPE  _IOR(DTA_IOCTL_MAGIC, FUNC_DTA_GET_DEV_SUBTYPE, \
                                                                DtaIoctlGetDevSubTypeData)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_GET_DEV_GENLOCKSTATE +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Gets the device genlock state
//

// DTA genlock state
#define DTA_GENLOCKSTATE_NO_REF        1
#define DTA_GENLOCKSTATE_LOCKING       2
#define DTA_GENLOCKSTATE_LOCKED        3

typedef struct _DtaIoctlGetGenlockStateOutput {
    Int  m_State;               // Current state
    Int  m_RefVidStd;           // Reference video standard
} DtaIoctlGetGenlockStateOutput;
ASSERT_SIZE(DtaIoctlGetGenlockStateOutput, 8)

#ifdef WINBUILD
    #define DTA_IOCTL_GET_DEV_GENLOCKSTATE  CTL_CODE(DTA_DEVICE_TYPE, \
                         FUNC_DTA_GET_DEV_GENLOCKSTATE, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlGetGenlockStateData {
        DtaIoctlGetGenlockStateOutput  m_Output;
    } DtaIoctlGetGenlockStateData;

    #define DTA_IOCTL_GET_DEV_GENLOCKSTATE  _IOR(DTA_IOCTL_MAGIC, \
                               FUNC_DTA_GET_DEV_GENLOCKSTATE, DtaIoctlGetGenlockStateData)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_RS422_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Wrapper IOCTL for all RS-422 related commands
//

// Command definitions
#define  DTA_RS422_CMD_WRITE              1     // Initiate a write to the RS-422 port
#define  DTA_RS422_CMD_READ               2     // Read buffered data
#define  DTA_RS422_CMD_FLUSH              3     // Flush input buffer

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_RS422_CMD_WRITE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtaIoctlRs422CmdWriteInput {
    UInt8  m_Buf[256];          // Data to be transmitted
    Int  m_NumBytes;            // Number of bytes in buffer
    // Do NOT use Bool (defined different in DTAPI and Driver)
    Int  m_Blocking;            // Wait for transmit to be finished?
} DtaIoctlRs422CmdWriteInput;
ASSERT_SIZE(DtaIoctlRs422CmdWriteInput, 264)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_RS422_CMD_READ -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtaIoctlRs422CmdReadInput {
    Int  m_BytesToRead;         // Number of bytes to read
    Int  m_Timeout;             // Timeout in ms to wait for the new data to arrive.
                                // 0 means non-blocking, <0 is invalid.
} DtaIoctlRs422CmdReadInput;
ASSERT_SIZE(DtaIoctlRs422CmdReadInput, 8)

typedef struct _DtaIoctlRs422CmdReadOutput {
    UInt8  m_Buf[256];          // Data that has been received
    Int  m_NumBytes;            // Number of bytes in buffer
} DtaIoctlRs422CmdReadOutput;
ASSERT_SIZE(DtaIoctlRs422CmdReadOutput, 260)

// Ioctl input data type
typedef struct _DtaIoctlRs422CmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;
    union {
        DtaIoctlRs422CmdWriteInput  m_Write;
        DtaIoctlRs422CmdReadInput  m_Read;
    } m_Data;
} DtaIoctlRs422CmdInput;
ASSERT_SIZE(DtaIoctlRs422CmdInput, 272)
    
// Ioctl output data type
typedef struct _DtaIoctlRs422CmdOutput {
    union {
        DtaIoctlRs422CmdReadOutput  m_Read;
    } m_Data;
} DtaIoctlRs422CmdOutput;
ASSERT_SIZE(DtaIoctlRs422CmdOutput, 260)

#ifdef WINBUILD
    #define DTA_IOCTL_RS422_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_RS422_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlRs422Cmd {
        DtaIoctlRs422CmdInput  m_Input;
        DtaIoctlRs422CmdOutput  m_Output;
    } DtaIoctlRs422Cmd;

    #define DTA_IOCTL_RS422_CMD  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_RS422_CMD, \
                                                                         DtaIoctlRs422Cmd)
#endif


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_SET_VCXO +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//

typedef struct _DtaIoctlSetVcxoInput {
    Int  m_VcxoIdx;
    Int  m_VcxoVal;
} DtaIoctlSetVcxoInput;
ASSERT_SIZE(DtaIoctlSetVcxoInput, 8)

#ifdef WINBUILD
    #define DTA_IOCTL_SET_VCXO  CTL_CODE(DTA_DEVICE_TYPE, \
                                                        FUNC_DTA_SET_VCXO, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlSetVcxo {
        DtaIoctlSetVcxoInput  m_Input;
    } DtaIoctlSetVcxo;

    #define DTA_IOCTL_SET_VCXO  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_SET_VCXO, DtaIoctlSetVcxo)
#endif


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_SDIAVRX_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Wrapper IOCTL for all SDIAVRX-API related commands
//
#define DTA_SDIAVRX_MAX_NUM_AUDPAIRS   8

// Command definitions
#define  DTA_SDIAVRX_CMD_GET_AUDIOSELECT    0     // Get audio selection
#define  DTA_SDIAVRX_CMD_SET_AUDIOSELECT    1     // Set audio selection
#define  DTA_SDIAVRX_CMD_READ_SMPTE2020     2     // Read SMPTE-2020 ANC packets
#define  DTA_SDIAVRX_CMD_GET_AUDIOSELECT2   3     // Get audio selection and extract mode
#define  DTA_SDIAVRX_CMD_SET_AUDIOSELECT2   4     // Set audio selection and extract mode

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_SDIAVRX_CMD_GET_AUDSELECT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtaIoctSdiAvRxCmdGetAudSelectOutput
{
    Int  m_NumChanPairs;                // Number of channel pairs
    UInt32  m_ChanPairSelect[DTA_SDIAVRX_MAX_NUM_AUDPAIRS];
} DtaIoctSdiAvRxCmdGetAudSelectOutput;
ASSERT_SIZE(DtaIoctSdiAvRxCmdGetAudSelectOutput, 4 + 4*DTA_SDIAVRX_MAX_NUM_AUDPAIRS)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_SDIAVRX_CMD_GET_AUDSELECT2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtaIoctSdiAvRxCmdGetAudSelect2Output
{
    Int  m_NumChanPairs;                // Number of channel pairs
    UInt32  m_ChanPairSelect[DTA_SDIAVRX_MAX_NUM_AUDPAIRS];
    UInt32  m_ChanExtractMode[DTA_SDIAVRX_MAX_NUM_AUDPAIRS];
} DtaIoctSdiAvRxCmdGetAudSelect2Output;
ASSERT_SIZE(DtaIoctSdiAvRxCmdGetAudSelect2Output, 4 + 8*DTA_SDIAVRX_MAX_NUM_AUDPAIRS)



//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_SDIAVRX_CMD_SET_AUDSELECT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtaIoctSdiAvRxCmdSetAudSelectInput
{
    Int  m_NumChanPairs;                // Number of channel pairs
    UInt32  m_ChanPairSelect[DTA_SDIAVRX_MAX_NUM_AUDPAIRS];
} DtaIoctSdiAvRxCmdSetAudSelectInput;
ASSERT_SIZE(DtaIoctSdiAvRxCmdSetAudSelectInput, 4 + 4*DTA_SDIAVRX_MAX_NUM_AUDPAIRS)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_SDIAVRX_CMD_SET_AUDSELECT2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtaIoctSdiAvRxCmdSetAudSelect2Input
{
    Int  m_NumChanPairs;                // Number of channel pairs
    UInt32  m_ChanPairSelect[DTA_SDIAVRX_MAX_NUM_AUDPAIRS];
    UInt32  m_ChanExtractMode[DTA_SDIAVRX_MAX_NUM_AUDPAIRS];
} DtaIoctSdiAvRxCmdSetAudSelect2Input;
ASSERT_SIZE(DtaIoctSdiAvRxCmdSetAudSelect2Input, 4 + 8*DTA_SDIAVRX_MAX_NUM_AUDPAIRS)


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_SDIAVRX_CMD_READ_SMPTE2020 -.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtaIoctSdiAvRxCmdReadSmpte2020Output
{
    Int  m_BufSize;                     // Number of valid bytes in m_Buf
    UInt8  m_Buf[4096];
} DtaIoctSdiAvRxCmdReadSmpte2020Output;
ASSERT_SIZE(DtaIoctSdiAvRxCmdReadSmpte2020Output, 4100)


// Ioctl input data type
typedef struct _DtaIoctlSdiAvRxCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;           // -1 signals a device level command
    union {
        DtaIoctSdiAvRxCmdSetAudSelectInput  m_AudSelect;
        DtaIoctSdiAvRxCmdSetAudSelect2Input  m_AudSelect2;
    } m_Data;
} DtaIoctlSdiAvRxCmdInput;
ASSERT_SIZE(DtaIoctlSdiAvRxCmdInput, 76)

// Ioctl output data type
typedef struct _DtaIoctlSdiAvRxCmdOutput {
    union {
        DtaIoctSdiAvRxCmdGetAudSelectOutput  m_AudSelect;
        DtaIoctSdiAvRxCmdGetAudSelect2Output  m_AudSelect2;
        DtaIoctSdiAvRxCmdReadSmpte2020Output  m_ReadSmpte2020;
    } m_Data;
} DtaIoctlSdiAvRxCmdOutput;

#ifdef WINBUILD
    #define DTA_IOCTL_SDIAVRX_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_SDIAVRX_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlSdiAvRxCmd {
        DtaIoctlSdiAvRxCmdInput  m_Input;
        DtaIoctlSdiAvRxCmdOutput  m_Output;
    } DtaIoctlSdiAvRxCmd;

    #define DTA_IOCTL_SDIAVRX_CMD  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_SDIAVRX_CMD, \
                                                                       DtaIoctlSdiAvRxCmd)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ FUNC_DTA_ENDEC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Wrapper for common encoder/decoder commands
//

// Command definitions
#define  DTA_ENDEC_CMD_EXCLUSIVE_ACCESS  0 // Acquire/release/probe exclusive access
#define  DTA_ENDEC_CMD_GET_SOURCE_PORT   1 // Get source port selection
#define  DTA_ENDEC_CMD_SET_SOURCE_PORT   2 // Set source port selection
#define  DTA_ENDEC_CMD_GET_VIDSTD        3 // Get video standard selection
#define  DTA_ENDEC_CMD_SET_VIDSTD        4 // Set video standard selection

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_ENDEC_CMD_EXCLUSIVE_ACCESS -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Reuse DtaIoctlNonIpCmdExclusiveAccessInput

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_ENDEC_CMD_GET_SOURCE_PORT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtaIoctEnDecCmdGetSourcePortOutput
{
    Int  m_PortIndex;                // Source port
} DtaIoctEnDecCmdGetSourcePortOutput;
ASSERT_SIZE(DtaIoctEnDecCmdGetSourcePortOutput, 4)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_ENDEC_CMD_SET_SOURCE_PORT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtaIoctEnDecCmdSetSourcePortInput
{
    Int  m_PortIndex;                // Source port
} DtaIoctEnDecCmdSetSourcePortInput;
ASSERT_SIZE(DtaIoctEnDecCmdSetSourcePortInput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_ENDEC_CMD_GET_VIDSTD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtaIoctEnDecCmdGetVidStdOutput
{
    Int  m_VidStd;                  // Video standard
} DtaIoctEnDecCmdGetVidStdOutput;
ASSERT_SIZE(DtaIoctEnDecCmdGetVidStdOutput, 4)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_ENDEC_CMD_SET_VIDSTD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtaIoctEnDecCmdSetVidStdInput
{
    Int  m_VidStd;                  // Video standard
} DtaIoctEnDecCmdSetVidStdInput;
ASSERT_SIZE(DtaIoctEnDecCmdSetVidStdInput, 4)


// Ioctl input data type
typedef struct _DtaIoctlEnDecCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;
    union {
        DtaIoctlNonIpCmdExclusiveAccessInput  m_ExclusiveAccess;
        DtaIoctEnDecCmdSetSourcePortInput  m_SetSourcePort;
        DtaIoctEnDecCmdSetVidStdInput  m_SetVidStd;
    } m_Data;
} DtaIoctlEnDecCmdInput;

// Ioctl output data type
typedef struct _DtaIoctlEnDecCmdOutput {
    union {
        DtaIoctEnDecCmdGetSourcePortOutput  m_GetSourcePort;
        DtaIoctEnDecCmdGetVidStdOutput  m_GetVidStd;
    } m_Data;
} DtaIoctlEnDecCmdOutput;

#ifdef WINBUILD
    #define DTA_IOCTL_ENDEC_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_ENDEC_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlEnDecCmd {
        DtaIoctlEnDecCmdInput  m_Input;
        DtaIoctlEnDecCmdOutput  m_Output;
    } DtaIoctlEnDecCmd;

    #define DTA_IOCTL_ENDEC_CMD  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_ENDEC_CMD, \
                                                                         DtaIoctlEnDecCmd)
#endif


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ FUNC_DTA_D7PRO_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Wrapper for all D7Pro related commands
//

// Command definitions
#define  DTA_D7PRO_CMD_SEND_COMMAND      0 // Send command to the control interface
#define  DTA_D7PRO_CMD_DEBUG_READ        1 // Read text from debug port
#define  DTA_D7PRO_CMD_STATUS_GET        2 // Get current status of encoder
#define  DTA_D7PRO_CMD_REBOOT            3 // Force a reboot of the D7Pro

// Status codes for D7Pro device
#define DTA_D7PRO_STATE_INIT          0 // Initial state, power not checked nor enabled
#define DTA_D7PRO_STATE_FLASH_PROG    1 // Flash programming in progress
#define DTA_D7PRO_STATE_NO_12V        2 // External 12V is not present
#define DTA_D7PRO_STATE_FAN_FAIL      3 // Power has been disabled due to a fan error
#define DTA_D7PRO_STATE_BOOTING       4 // D7Pro is booting, handshake not done yet
#define DTA_D7PRO_STATE_OK            5 // Boot complete, device ready for use

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_D7PRO_CMD_SEND_COMMAND -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtaIoctlD7ProSendCommandInput {
    Int  m_NumBytesToWrite; // Number of bytes in buffer
    Int  m_NumBytesToRead;  // Number of bytes to read
    Int  m_Timeout;         // Maximum time in ms to wait for data
    UInt8  m_Buf[0];        // Data to be transmitted, dynamic sized buffer
} DtaIoctlD7ProSendCommandInput;

typedef struct _DtaIoctlD7ProSendCommandOutput {
    Int  m_NumBytes;        // Actual number of bytes read
    UInt8  m_Buf[0];        // Data read back, dynamic sized buffer
} DtaIoctlD7ProSendCommandOutput;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_D7PRO_CMD_DEBUG_READ -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtaIoctlD7ProDebugReadInput {
    Int  m_NumBytesToRead;  // Maximum number of bytes to read
    Int  m_Timeout;         // Maximum time in ms to wait for data
} DtaIoctlD7ProDebugReadInput;
ASSERT_SIZE(DtaIoctlD7ProDebugReadInput, 8)

typedef struct _DtaIoctlD7ProDebugReadOutput {
    Int  m_NumBytes;        // Actual number of bytes read
    UInt8  m_Buf[0];        // Data read back, dynamic sized buffer
} DtaIoctlD7ProDebugReadOutput;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_D7PRO_CMD_STATUS_GET -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtaIoctlD7ProStatusGetOutput {
    Int  m_State;
    Int  m_Ext12FailCnt;
    Int  m_FanFailCnt;
    Int  m_PowerFailCnt;
} DtaIoctlD7ProStatusGetOutput;
ASSERT_SIZE(DtaIoctlD7ProStatusGetOutput, 16)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_D7PRO_CMD_EXCLUSIVE_ACCESS -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Reuse DtaIoctlNonIpCmdExclusiveAccessInput

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_D7PRO_CMD_ACK_BOOT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Reuse DtaIoctlD7ProStatusGetOutput

// Ioctl input data type
typedef struct _DtaIoctlD7ProCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;
    union {
        DtaIoctlD7ProSendCommandInput  m_SendCmd;
        DtaIoctlD7ProDebugReadInput  m_DebugRead;
        DtaIoctlNonIpCmdExclusiveAccessInput  m_ExclusiveAccess;
    } m_Data;
} DtaIoctlD7ProCmdInput;

// Ioctl output data type
typedef struct _DtaIoctlD7ProCmdOutput {
    union {
        DtaIoctlD7ProSendCommandOutput  m_SendCmd;
        DtaIoctlD7ProDebugReadOutput  m_DebugRead;
        DtaIoctlD7ProStatusGetOutput  m_StatusGet;
    } m_Data;
} DtaIoctlD7ProCmdOutput;

#ifdef WINBUILD
    #define DTA_IOCTL_D7PRO_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_D7PRO_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlD7ProCmd {
        DtaIoctlD7ProCmdInput  m_Input;
        DtaIoctlD7ProCmdOutput  m_Output;
    } DtaIoctlD7ProCmd;

    #define DTA_IOCTL_D7PRO_CMD  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_D7PRO_CMD, \
                                                                         DtaIoctlD7ProCmd)
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ FUNC_DTA_SPIMF_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Wrapper for common SPI master falsh commands
//

// Command definitions
#define  DTA_SPIMF_CMD_EXCLUSIVE_ACCESS  0 // Acquire/release/probe exclusive access
#define  DTA_SPIMF_CMD_DEVICE_LOCK       1 // Lock/unlock device for write and erase ops
#define  DTA_SPIMF_CMD_GET_PROPERTIES    3 // Get device properties
#define  DTA_SPIMF_CMD_ERASE             4 // Erase
#define  DTA_SPIMF_CMD_READ              5 // Read data
#define  DTA_SPIMF_CMD_WRITE             6 // Write datea

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_SPIMF_CMD_EXCLUSIVE_ACCESS -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Reuse DtaIoctlNonIpCmdExclusiveAccessInput

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_SPIMF_CMD_DEVICE_LOCK -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtaIoctlSpiMfCmdDeviceLockInput {
    Int  m_Lock;                // Lock / Unlock
} DtaIoctlSpiMfCmdDeviceLockInput;
ASSERT_SIZE(DtaIoctlSpiMfCmdDeviceLockInput, 4)


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_SPIMF_CMD_READ -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtaIoctlSpiMfCmdReadInput {
    Int  m_StartAddress;        // Start address
    Int  m_NumBytesToRead;      // Number of bytes to read
} DtaIoctlSpiMfCmdReadInput;
ASSERT_SIZE(DtaIoctlSpiMfCmdReadInput, 8)

typedef struct _DtaIoctlSpiMfCmdReadOutput {
    Int  m_NumBytes;           // Number of bytes in buffer
    UInt8  m_Buf[0];            // Dynamic sized buffer
} DtaIoctlSpiMfCmdReadOutput;
ASSERT_SIZE(DtaIoctlSpiMfCmdReadOutput, 4)




//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_SPIMF_CMD_ERASE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtaIoctlSpiMfCmdEraseInput {
    Int  m_StartAddress;        // Start address
    Int  m_NumBytes;            // Number of bytes to be erased
} DtaIoctlSpiMfCmdEraseInput;
ASSERT_SIZE(DtaIoctlSpiMfCmdEraseInput, 8)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_SPIMF_CMD_GET_PROPERTIES -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtaIoctlSpiMfProperties {    
    Int  m_SpiClockRate;  //  SPI Clock Rate in units of 100kHz
    Int  m_MemoryId;      // Identifier for SPI Flash/PROM device.
    Int  m_PageSize;      // Device page size. 
    Int  m_SectorSize;    // Device sector size (1 = device hase no sectors).
    Int  m_MemorySize;    // Device memory size

} DtaIoctlSpiMfCmdGetPropertiesOutput;
ASSERT_SIZE(DtaIoctlSpiMfCmdGetPropertiesOutput, 20)


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_SPIMF_CMD_WRITE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _DtaIoctlSpiMfCmdWriteInput {
    Int  m_StartAddress;        // Start address
    Int  m_NumBytesToWrite;     // Number of bytes in buffer to write
    UInt8  m_Buf[0];            // Dynamic sized buffer
} DtaIoctlSpiMfCmdWriteInput;
ASSERT_SIZE(DtaIoctlSpiMfCmdWriteInput, 8)

// Ioctl input data type
typedef struct _DtaIoctlSpiMfCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;
    union {
        DtaIoctlNonIpCmdExclusiveAccessInput  m_ExclusiveAccess;
        DtaIoctlSpiMfCmdEraseInput  m_Erase;
        DtaIoctlSpiMfCmdDeviceLockInput  m_Lock;
        DtaIoctlSpiMfCmdReadInput  m_Read;
        DtaIoctlSpiMfCmdWriteInput  m_Write;

    } m_Data;
} DtaIoctlSpiMfCmdInput;
ASSERT_SIZE(DtaIoctlSpiMfCmdInput, 16)

// Ioctl output data type
typedef struct _DtaIoctlSpiMfCmdOutput {
    union {
        DtaIoctlSpiMfCmdReadOutput  m_Read;
        DtaIoctlSpiMfCmdGetPropertiesOutput  m_GetProperties;
    } m_Data;
} DtaIoctlSpiMfCmdOutput;
ASSERT_SIZE(DtaIoctlSpiMfCmdOutput, 20)

#ifdef WINBUILD
#define DTA_IOCTL_SPIMF_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_SPIMF_CMD, \
                                                    METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
typedef union _DtaIoctlSpiMfCmd {
    DtaIoctlSpiMfCmdInput  m_Input;
    DtaIoctlSpiMfCmdOutput  m_Output;
} DtaIoctlSpiMfCmd;

#define DTA_IOCTL_SPIMF_CMD  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_SPIMF_CMD, \
                                                                        DtaIoctlSpiMfCmd)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ or +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Wrapper for programming interface commands
//

// Command definitions
#define  DTA_PROGITF_CMD_EXCLUSIVE_ACCESS  0 // Acquire/release/probe exclusive access

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_PROGITF_CMD_EXCLUSIVE_ACCESS -.-.-.-.-.-.-.-.-.-.-.-.-.-
// Reuse DtaIoctlNonIpCmdExclusiveAccessInput

// Ioctl input data type
typedef struct _DtaIoctlProgItfCmdInput {
    Int  m_Cmd;
    Int  m_ItfIndex;
    union {
        DtaIoctlNonIpCmdExclusiveAccessInput  m_ExclusiveAccess;
    } m_Data;
} DtaIoctlProgItfCmdInput;

#ifdef WINBUILD
    #define DTA_IOCTL_PROGITF_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_PROGITF_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlProgItfCmd {
        DtaIoctlProgItfCmdInput  m_Input;
    } DtaIoctlProgItfCmd;

    #define DTA_IOCTL_PROGITF_CMD  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_PROGITF_CMD, \
                                                                       DtaIoctlProgItfCmd)
#endif


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ FUNC_DTA_MULTIMOD_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Wrapper for multi-modulator commands
//

// Command definitions
#define  DTA_MULTIMOD_CMD_SET_LEVEL      0 // Set RF-level

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MULTIMOD_CMD_SET_LEVEL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtaIoctlMultiModCmdSetLevelInput {
    Int  m_ChanTotalGainFactor; // Total gain for this channel (times 1<<16)
    Int  m_CommonAttenFactor;   // The common attenuation part (times 1<<16)
    Int  m_Atten1;              // Attenuator 1 setting
    Int  m_Atten2;              // Attenuator 2 setting
    Int  m_Atten3;              // Attenuator 3 setting
    Int  m_FreqMHz;             // Modulator frequency in MHz
} DtaIoctlMultiModCmdSetLevelInput;
ASSERT_SIZE(DtaIoctlMultiModCmdSetLevelInput, 24)

// Ioctl input data type
typedef struct _DtaIoctlMultiModCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;
    union {
        DtaIoctlMultiModCmdSetLevelInput  m_SetLevel;
    } m_Data;
} DtaIoctlMultiModCmdInput;
ASSERT_SIZE(DtaIoctlMultiModCmdInput, 32)


#ifdef WINBUILD
#define DTA_IOCTL_MULTIMOD_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_MULTIMOD_CMD, \
                                                    METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
typedef union _DtaIoctlMultiModCmd {
    DtaIoctlMultiModCmdInput  m_Input;
} DtaIoctlMultiModCmd;

#define DTA_IOCTL_MULTIMOD_CMD  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_MULTIMOD_CMD, \
                                                                      DtaIoctlMultiModCmd)
#endif


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_HDMI_TX_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Wrapper command to perform a HDMI command.
//

// Constants

// DTA HDMI commands
#define DTA_HDMI_TX_CMD_GET_HDMI_STATUS        1
#define DTA_HDMI_TX_CMD_GET_VIDSTD             2
#define DTA_HDMI_TX_CMD_SET_VIDMOD             3
#define DTA_HDMI_TX_CMD_FORCE_TEST_PICTURE     4
#define DTA_HDMI_TX_CMD_FORCE_MONITOR_DETECTED 5
#define DTA_HDMI_TX_CMD_DISABLE_EDID_CHECK     6
#define DTA_HDMI_TX_CMD_DISABLE_OUTPUT         7
#define DTA_HDMI_TX_CMD_SET_COLORIMETRY        8
#define DTA_HDMI_TX_CMD_GET_COLORIMETRY        9


// DTA HDMI get vidstd from index
typedef struct _DtaIoctlHdmiTxCmdGetVidStdInput {
    Int  m_Index;
} DtaIoctlHdmiTxCmdGetVidStdInput;
ASSERT_SIZE(DtaIoctlHdmiTxCmdGetVidStdInput, 4)

// DTA HDMI set vidmod
typedef struct _DtaIoctlHdmiTxCmdSetVidModInput {
    Int  m_VidMod;
} DtaIoctlHdmiTxCmdSetVidModInput;
ASSERT_SIZE(DtaIoctlHdmiTxCmdSetVidModInput, 4)

// DTA HDMI force test picture
typedef struct _DtaIoctlHdmiTxCmdForceTestPictureInput {
    Int  m_Enable;
} DtaIoctlHdmiTxCmdForceTestPictureInput;
ASSERT_SIZE(DtaIoctlHdmiTxCmdForceTestPictureInput, 4)

// DTA HDMI force monitor detected
typedef struct _DtaIoctlHdmiTxCmdForceMonitorDetectedInput {
    Int  m_Enable;
} DtaIoctlHdmiTxCmdForceMonitorDetectedInput;
ASSERT_SIZE(DtaIoctlHdmiTxCmdForceMonitorDetectedInput, 4)

// DTA HDMI disable edid check
typedef struct _DtaIoctlHdmiTxCmdDisableEdidCheckInput {
    Int  m_Disable;
} DtaIoctlHdmiTxCmdDisableEdidCheckInput;
ASSERT_SIZE(DtaIoctlHdmiTxCmdDisableEdidCheckInput, 4)

// DTA HDMI disable edid check
typedef struct _DtaIoctlHdmiTxCmdDisableOutputInput {
    Int  m_Disable;
} DtaIoctlHdmiTxCmdDisableOutputInput;
ASSERT_SIZE(DtaIoctlHdmiTxCmdDisableOutputInput, 4)

// DTA HDMI set colorimetry
typedef struct _DtaIoctlHdmiTxCmdSetColorimetryInput {
    Int  m_Colorimetry;
    Int  m_ExtendedColorimetry;
} DtaIoctlHdmiTxCmdSetColorimetryInput;
ASSERT_SIZE(DtaIoctlHdmiTxCmdSetColorimetryInput, 8)

// Ioctl input data type
typedef struct _DtaIoctlHdmiTxCmdInput {
    UInt  m_Cmd;
    Int  m_PortIndex;
    union {
        DtaIoctlHdmiTxCmdGetVidStdInput  m_GetVidStd;
        DtaIoctlHdmiTxCmdSetVidModInput  m_SetVidMod;
        DtaIoctlHdmiTxCmdForceTestPictureInput  m_ForceTestPicture;
        DtaIoctlHdmiTxCmdForceMonitorDetectedInput  m_ForceMonitorDetected;
        DtaIoctlHdmiTxCmdDisableEdidCheckInput  m_DisableEdidCheck;
        DtaIoctlHdmiTxCmdDisableOutputInput  m_DisableHdmiOutput;
        DtaIoctlHdmiTxCmdSetColorimetryInput  m_SetColorimetry;
    } m_Data;
} DtaIoctlHdmiTxCmdInput;
ASSERT_SIZE(DtaIoctlHdmiTxCmdInput, 16)

// DTA Get HDMI status command output data type
typedef struct _DtaIoctlHdmiTxCmdGetHdmiStatusOutput {
    Int  m_MonDetected;
    Int  m_EdidError;
    Int  m_SupportHdmi;
    Int  m_SupportYCbCr444;
    Int  m_SupportYCbCr422;
    Int  m_SupportBasicAudio;
    Int  m_SupportScDc;
    Int  m_SupportMonitorRangeLimits;
    Int  m_SupportHdr;
    Int  m_ColorimetryForced;
    Int  m_ForceTestPicture;
    Int  m_UsingTestPicture;
    Int  m_ForceMonitorDetected;
    Int  m_DisableEdidCheck;
    Int  m_DisableHdmiOutput;
    Int  m_UsedVidStd;
    Int  m_UsedAspectRatio;
    Int  m_SelectedVidStd;
    Int  m_SelectedAspectRatio;
    Int  m_UsedVidMod;
    Int  m_SelectedVidMod;
    Int  m_Reserved[4];
    UInt  m_MaxPixelClk;
    UInt  m_MinVRate;
    UInt  m_MaxVRate;
    UInt  m_MinHRate;
    UInt  m_MaxHRate;
    UInt64A  m_MonSupportedFormats;
    UInt64A  m_MonSupportedAudio;
} DtaIoctlHdmiTxCmdGetHdmiStatusOutput;
ASSERT_SIZE(DtaIoctlHdmiTxCmdGetHdmiStatusOutput, 136)

// DTA Get vidstd command output data type
typedef struct _DtaIoctlHdmiTxCmdGetVidStdOutput {
    Int  m_VidStd;
    Int  m_AspectRatio;
} DtaIoctlHdmiTxCmdGetVidStdOutput;
ASSERT_SIZE(DtaIoctlHdmiTxCmdGetVidStdOutput, 8)

// DTA HDMI set colorimetry
typedef struct _DtaIoctlHdmiTxCmdGetColorimetryOutput {
    Int  m_Colorimetry;
    Int  m_ExtendedColorimetry;
} DtaIoctlHdmiTxCmdGetColorimetryOutput;
ASSERT_SIZE(DtaIoctlHdmiTxCmdGetColorimetryOutput, 8)

// Ioctl output data type
typedef struct _DtaIoctlHdmiTxCmdOutput {
    union {
        DtaIoctlHdmiTxCmdGetHdmiStatusOutput  m_GetHdmiStatus;
        DtaIoctlHdmiTxCmdGetVidStdOutput  m_GetVidStd;
        DtaIoctlHdmiTxCmdGetColorimetryOutput  m_GetColorimetry;
    } m_Data;
} DtaIoctlHdmiTxCmdOutput;
ASSERT_SIZE(DtaIoctlHdmiTxCmdOutput, 136)


#ifdef WINBUILD
    #define DTA_IOCTL_HDMI_TX_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_HDMI_TX_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlHdmiTxCmdData {
        DtaIoctlHdmiTxCmdInput  m_Input;
        DtaIoctlHdmiTxCmdOutput  m_Output;
    } DtaIoctlHdmiTxCmdData;

    #define DTA_IOCTL_HDMI_TX_CMD  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_HDMI_TX_CMD, \
                                                                       DtaIoctlHdmiTxCmdData)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoctlInputData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef union _DtaIoctlInputData {
    DtaIoctlGetPropertyInput  m_GetProperty;
    DtaIoctlGetProperty2Input  m_GetProperty2;
    DtaIoctlGetProperty3Input  m_GetProperty3;
    DtaIoctlGetProperty4Input  m_GetProperty4;
    DtaIoctlVpdCmdInput  m_VpdCmd;
    DtaIoctlI2cCmdInput  m_I2cCmd;
    DtaIoctlGetAddressRegsInput  m_GetAddrRegs;
    DtaIoctlReleaseAddressRegsInput  m_ReleaseAddrRegs;
    DtaIoctlRegisterEventsInput  m_RegisterEvents;
    DtaIoctlDmaReadInput  m_DmaRead;
    DtaIoctlDmaWriteInput  m_DmaWrite;
    DtaIoctlShBufCmdInput  m_ShBufCmd;
    DtaIoctlGetIoConfigInput  m_GetIoConfig;
    DtaIoctlSetIoConfigInput  m_SetIoConfig;
    DtaIoctlNonIpCmdInput  m_NonIpCmd;
    DtaIoctlNonIpTxCmdInput  m_NonIpTxCmd;
    DtaIoctlNonIpRxCmdInput  m_NonIpRxCmd;
    DtaIoctlIpCmdInput  m_IpCmd;
    DtaIoctlNonIpTxCmdInput  m_IpTxCmd;
    DtaIoctlPhyMacCmdInput  m_PhyMacCmd;
    DtaIoctlIpNwCmdInput  m_IpNwCmd;
    DtaIoctlGetTableInput  m_GetTable;
    DtaIoctlGetTable2Input  m_GetTable2;
    DtaIoctlGetTable3Input  m_GetTable3;
    DtaIoctlRebootFirmwareInput  m_RebootFirmware;
    DtaIoctlGetStrPropertyInput  m_GetStrProperty;
    DtaIoctlGetStrProperty2Input m_GetStrProperty2;
    DtaIoctlGetStrProperty3Input m_GetStrProperty3;
    DtaIoctlMatrixCmdInput  m_NonIpHdCmd;
    DtaIoctlSetMemoryTestModeInput  m_SetMemoryTestMode;
    DtaIoctlSetVcxoInput  m_SetVcxo;
    DtaIoctlSdiAvRxCmdInput  m_NonIpSdiAvRxCmd;
    DtaIoctlEnDecCmdInput  m_EnDecCmd;
    DtaIoctlD7ProCmdInput  m_D7ProCmd;
    DtaIoctlSpiMfCmdInput  m_SpiMfCmd;
    DtaIoctlProgItfCmdInput  m_ProgItfCmd;
    DtaIoctlMultiModCmdInput  m_MultiModCmd;
    DtaIoctlHdmiTxCmdInput  m_HdmiTxCmd;
} DtaIoctlInputData;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoctlOutputData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef union _DtaIoctlOutputData {
    DtaIoctlGetPropertyOutput  m_GetProperty;
    DtaIoctlGetDevInfoOutput  m_GetDevInfo;
    DtaIoctlGetDevInfoOutput2  m_GetDevInfo2;
    DtaIoctlGetDevInfoOutput3  m_GetDevInfo3;
    DtaIoctlGetDevInfoOutput4  m_GetDevInfo4;
    DtaIoctlVpdCmdOutput  m_VpdCmd;
    DtaIoctlGetDriverVersionOutput  m_GetDriverVersion;
    DtaIoctlI2cCmdOutput  m_I2cCmd;
    DtaIoctlGetAddressRegsOutput  m_GetAddrRegs;
    DtaIoctlGetEventOutput  m_GetEvent;
#ifdef LINBUILD
    DtaIoctlDmaReadOutput  m_DmaRead;
#endif
    DtaIoctlGetIoConfigOutput  m_GetIoConfig;
    DtaIoctlNonIpCmdOutput  m_NonIpCmd;
    DtaIoctlNonIpTxCmdOutput  m_NonIpTxCmd;
    DtaIoctlNonIpRxCmdOutput  m_NonIpRxCmd;
    DtaIoctlIpCmdOutput  m_IpCmd;
    DtaIoctlNonIpTxCmdOutput  m_IpTxCmd;
#ifndef WINBUILD
    DtaIoctlReleaseAddressRegsOutput  m_ReleaseAddrRegs;
#endif
    DtaIoctlPhyMacCmdOutput  m_PhyMacCmd;
    DtaIoctlIpNwCmdOutput  m_IpNwCmd;
    DtaIoctlGetTableOutput  m_GetTable;
    DtaIoctlGetNwDriverVersionOutput  m_GetNwDriverVersion;
    DtaIoctlGetStrPropertyOutput  m_GetStrProperty;
    DtaIoctlMatrixCmdOutput  m_NonIpHdCmd;
    DtaIoctlGetDevSubTypeOutput  m_GetDevSubType;
    DtaIoctlGetGenlockStateOutput  m_GetGenlockstate;
    DtaIoctlSdiAvRxCmdOutput  m_NonIpSdiAvRxCmd;
    DtaIoctlEnDecCmdOutput  m_EnDecCmd;
    DtaIoctlD7ProCmdOutput  m_D7ProCmd;
    DtaIoctlSpiMfCmdOutput  m_SpiMfCmd;
    DtaIoctlHdmiTxCmdOutput  m_HdmiTxCmd;
} DtaIoctlOutputData;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoctlData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef union _DtaIoctlData {
    DtaIoctlInputData  m_Input;
    DtaIoctlOutputData  m_Output;
} DtaIoctlData;

#pragma pack (pop)

#undef Int64
#undef UInt64

#endif // __DTA_COMMON_H
