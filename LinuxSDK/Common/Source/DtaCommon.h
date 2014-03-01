//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaCommon.h *#*#*#*#*#*#*#*#* (C) 2010-2012 DekTec
//
// Dta driver - Common file shared between Dta driver and DTAPI
//
// This file describes the Dta driver interface, which is used by the DTAPI.

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2012 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//  3. The source code may not be modified for the express purpose of enabling hardware
//     features for which no genuine license has been obtained.
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
    FUNC_DTA_GET_DEV_GENLOCKSTATE
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
#define DTA_NONIP_CMD_DETECT_VIDSTD     3

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_NONIP_CMD_EXCLUSIVE_ACCESS -.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Exclusive access cmd's
#define DTA_EXCLUSIVE_ACCESS_CMD_ACQUIRE  1
#define DTA_EXCLUSIVE_ACCESS_CMD_RELEASE  2
#define DTA_EXCLUSIVE_ACCESS_CMD_PROBE    3
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

// Ioctl input data type
typedef struct _DtaIoctlNonIpCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;
    union {
        DtaIoctlNonIpCmdExclusiveAccessInput  m_ExclusiveAccess;
    } m_Data;
} DtaIoctlNonIpCmdInput;
ASSERT_SIZE(DtaIoctlNonIpCmdInput, 12)

// Ioctl Output data type
typedef struct _DtaIoctlNonIpCmdOutput {
    union {
        DtaIoctlNonIpCmdGetTargetIdOutput  m_GetTargetId;
        DtaIoctlNonIpCmdDetectVidStdOutput  m_DetVidStd;
    } m_Data;
} DtaIoctlNonIpCmdOutput;
ASSERT_SIZE(DtaIoctlNonIpCmdOutput, 8)

#ifdef WINBUILD
    #define DTA_IOCTL_NONIP_CMD  CTL_CODE(DTA_DEVICE_TYPE, FUNC_DTA_NONIP_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtaIoctlNonIpCmd {
        DtaIoctlNonIpCmdInput  m_Input;
        DtaIoctlNonIpCmdOutput  m_Output;
    } DtaIoctlNonIpCmd;

    #define DTA_IOCTL_NONIP_CMD  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_NONIP_CMD, \
                                                                         DtaIoctlNonIpCmd)
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
    UInt8  m_Buf[0];                // Dynamic sized buffer    
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
#define DTA_FEC_2D               1       // FEC reconstruction
#define DTA_FEC_2D_M1            1       // Mode1: FECdT = DVBdT + .5 * DVBdT
#define DTA_FEC_2D_M2            2       // Mode2: FECdT = DVBdT

// Mode
#define DTA_IP_NORMAL            0
#define DTA_IP_TX_DBLBUF         1
#define DTA_IP_RX_DBLBUF         2
#define DTA_IP_RX_FAILOVER       3

// Flags
#define DTA_IP_V4                1
#define DTA_IP_V6                2

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
    UInt8  m_DstIp[16];              // Destination: IP address
    UInt16  m_DstPort;               // Destination: port number
    UInt8  m_SrcIp[16];              // Source: IP address
    UInt16  m_SrcPort;               // Source: port number
    Int  m_VlanId;                   // VLAN ID
    // Redundant link
    UInt8  m_DstIp2[16];             // Destination: IP address
    UInt16  m_DstPort2;              // Destination: port number
    UInt8  m_SrcIp2[16];             // Source: IP address
    UInt16  m_SrcPort2;              // Source: port number
    Int  m_VlanId2;                  // VLAN ID
    // Options
    Int  m_Mode;
    Int  m_Flags;
    Int  m_Protocol;                 // Protocol: UDP/RTP
    Int  m_FecMode;                  // Error correction mode
} DtaIoctlIpRxCmdSetIpPars2Input;
ASSERT_SIZE(DtaIoctlIpRxCmdSetIpPars2Input, 100)

// DTA_IP_RX_CMD_GETIPPARS
typedef struct _DtaIoctlIpRxCmdGetIpParsInput {
    Int  m_Channel;
} DtaIoctlIpRxCmdGetIpParsInput;
ASSERT_SIZE(DtaIoctlIpRxCmdGetIpParsInput, 4)

typedef struct _DtaIoctlIpRxCmdGetIpParsOutput {
    Int  m_Protocol;                // Protocol: UDP/RTP/Unknown
    Int  m_FecNumRows;              // @D@ = #rows in FEC matrix
    Int  m_FecNumCols;              // @L@ = #columns in FEC matrix
    Int  m_NumTpPerIp;              // 0: Not detected
} DtaIoctlIpRxCmdGetIpParsOutput;
ASSERT_SIZE(DtaIoctlIpRxCmdGetIpParsOutput, 16)

// DTA_IP_RX_CMD_GETIPSTAT
typedef struct _DtaIoctlIpRxCmdGetIpStatInput {
    Int  m_Channel;
} DtaIoctlIpRxCmdGetIpStatInput;
ASSERT_SIZE(DtaIoctlIpRxCmdGetIpStatInput, 4)

typedef struct _DtaIoctlIpRxCmdGetIpStatOutput {
    UInt  m_TotNumIPPackets;
    UInt  m_LostIPPacketsBeforeFec;     // (BER before FEC)
    UInt  m_LostIPPacketsAfterFec;      // (BER after FEC)
    UInt  m_Reserved1;
    UInt  m_Reserved2;
} DtaIoctlIpRxCmdGetIpStatOutput;
ASSERT_SIZE(DtaIoctlIpRxCmdGetIpStatOutput, 20)

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
        DtaIoctlIpRxCmdSetIpParsInput  m_SetIpPars;
        DtaIoctlIpRxCmdGetIpStatInput  m_GetIpStat;
        DtaIoctlIpRxCmdSetIpPars2Input  m_SetIpPars2;
    } m_Data;
} DtaIoctlIpRxCmdInput;
ASSERT_SIZE(DtaIoctlIpRxCmdInput, 108)

// Ioctl output data type
typedef struct _DtaIoctlIpRxCmdOutput {
    union {
        DtaIoctlIpXxCmdAttachChannelOutput m_AttachChannel;
        DtaIoctlIpXxCmdGetFlagsOutput m_GetFlags;
        DtaIoctlIpRxCmdGetStatusOutput  m_GetStatus;
        DtaIoctlIpRxCmdGetTsRateOutput  m_GetTsRate;
        DtaIoctlIpRxCmdGetIpParsOutput  m_GetIpPars;
        DtaIoctlIpRxCmdGetIpStatOutput  m_GetIpStat;
    } m_Data;
} DtaIoctlIpRxCmdOutput;
ASSERT_SIZE(DtaIoctlIpRxCmdOutput, 20)

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
    Int  m_PortIndex;                           // Port index
    char  m_Name[DTA_TABLE_NAME_MAX_SIZE];      // Name of table
    UInt  m_MaxNumEntries;                      // Max. number of entry's to store
} DtaIoctlGetTableInput;
ASSERT_SIZE(DtaIoctlGetTableInput, 60)

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
    typedef union _DtaIoctlGetTable {
        DtaIoctlGetTableInput  m_Input;
        DtaIoctlGetTableOutput  m_Output;
    } DtaIoctlGetTable;

    #define DTA_IOCTL_GET_TABLE  _IOWR(DTA_IOCTL_MAGIC_SIZE, FUNC_DTA_GET_TABLE, \
                                                                         DtaIoctlGetTable)
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
    Int  m_Reserved1;
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

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA_IOCTL_MATRIX_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Wrapper IOCTL for all Matrix-API (HD-SDI) related commands
//

// Command definitions
#define  DTA_MATRIX_CMD_WAIT_FRAME        1     // Waits for a specific or simple the
                                                // next frame to be received/transmitted
#define  DTA_MATRIX_CMD_GET_SYNC_INFO     2     // Get SOF sync info

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA_MATRIX_CMD_WAIT_FRAME -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

typedef struct _DtaIoctlMatrixCmdWaitFrameInput {
    Int64A  m_Frame;            // Frame to wait for (-1 signals wait for next frame)
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

// Ioctl input data type
typedef struct _DtaIoctlMatrixCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;           // -1 signals a device level command
    union {
        DtaIoctlMatrixCmdWaitFrameInput  m_WaitFrame;
    } m_Data;
} DtaIoctlMatrixCmdInput;
ASSERT_SIZE(DtaIoctlMatrixCmdInput, 24)

// Ioctl output data type
typedef struct _DtaIoctlMatrixCmdOutput {
    union {
        DtaIoctlMatrixCmdWaitFrameOutput  m_WaitFrame;
        DtaIoctlMatrixCmdGetSyncInfoOutput  m_SyncInfo;
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

    #define DTA_IOCTL_MATRIX_CMD  _IOWR(DTA_IOCTL_MAGIC, FUNC_DTA_MATRIX_CMD, \
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoctlInputData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef union _DtaIoctlInputData {
    DtaIoctlGetPropertyInput  m_GetProperty;
    DtaIoctlGetProperty2Input  m_GetProperty2;
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
    DtaIoctlRebootFirmwareInput  m_RebootFirmware;
    DtaIoctlGetStrPropertyInput  m_GetStrProperty;
    DtaIoctlMatrixCmdInput  m_NonIpHdCmd;
    DtaIoctlSetMemoryTestModeInput  m_SetMemoryTestMode;
} DtaIoctlInputData;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoctlOutputData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef union _DtaIoctlOutputData {
    DtaIoctlGetPropertyOutput  m_GetProperty;
    DtaIoctlGetDevInfoOutput  m_GetDevInfo;
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
