//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtuCommon.h *#*#*#*#*#*#*#*#* (C) 2011-2013 DekTec
//
// Dtu driver - Common file shared between Dtu driver and DTAPI
//
// This file describes the Dtu driver interface, which is used by the DTAPI.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2013 DekTec Digital Video B.V.
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

#ifndef __DTU_COMMON_H
#define __DTU_COMMON_H

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
        #pragma warning( disable : 4200 )
        
        #include <winioctl.h>
    #endif

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Driver Device interface GUID -.-.-.-.-.-.-.-.-.-.-.-.-.-
    //
    // {96A04ED6-FAA1-47fb-A209-E7BEEC779E28}
    static const GUID GUID_DEVINTERFACE_DTU = 
    { 0x96a04ed6, 0xfaa1, 0x47fb, { 0xa2, 0x9, 0xe7, 0xbe, 0xec, 0x77, 0x9e, 0x28 } };

#else // WINBUILD
    #ifdef DTAPI
        #include <sys/ioctl.h>
    #endif
#endif

// Align members of structures and unions always to 4 bytes to prevent 32/64-bit
// size difference and misalignment
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
    #define DTU_DEVICE_TYPE  45500  // DeviceType for DeviceIoControl operations
    #define DTU_IOCTL_BASE   3500   // Offset for Function
#else
    // Linux specific defines
    #define DTU_IOCTL_MAGIC       0xAE   // Magic number
    #define DTU_IOCTL_MAGIC_SIZE  0xAF   // Magic number for Ioctls with a dynamic size
    #define DTU_IOCTL_BASE        0x50   // Offset for Function
#endif

// DTU functions
// DON'T INSERT NEW VALUES BETWEEN EXISTING ONES, SO THAT NEW DRIVER VERSIONS REMAIN
// COMPATIBLE WITH USER APPLICATIONS THAT WERE COMPILED AGAINST AN OLD DRIVER!!
enum {
    FUNC_DTU_GET_PROPERTY = DTU_IOCTL_BASE,
    FUNC_DTU_GET_DEV_INFO,
    FUNC_DTU_VPD_CMD,
    FUNC_DTU_GET_DRIVER_VERSION,
    FUNC_DTU_I2C_CMD,
    FUNC_DTU_GET_EVENT,
    FUNC_DTU_REGISTER_EVENTS,
    FUNC_DTU_UNREGISTER_EVENTS,
    FUNC_DTU_REG_READ,
    FUNC_DTU_REG_WRITE_MASKED,
    FUNC_DTU_READ_DATA,
    FUNC_DTU_WRITE_DATA,
    FUNC_DTU_GET_IOCONFIG,
    FUNC_DTU_SET_IOCONFIG,
    FUNC_DTU_NONIP_CMD,
    FUNC_DTU_NONIP_RX_CMD,
    FUNC_DTU_NONIP_TX_CMD,
    FUNC_DTU_RESET_SOFT,
    FUNC_DTU_GET_TABLE,
    FUNC_DTU_GET_STR_PROPERTY,
    FUNC_DTU_GET_DEV_SUBTYPE,
    FUNC_DTU_GET_PROPERTY2,
    FUNC_DTU_SH_BUF_CMD,
    FUNC_DTU_REENUMERATE,
    FUNC_DTU_VENDOR_REQUEST,
    FUNC_DTU_GET_STATE_FLAGS,
    FUNC_DTU_TRIGGER_WATCHDOG,
    FUNC_DTU_SET_RX_MODE,
    FUNC_DTU_UPLOAD_FPGA_FW,
    FUNC_DTU_GET_DATA_BUF_SIZE,
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_GET_PROPERTY -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Gets a device or port property value and type from the DtuProperyStore.
//

// Ioctl input data type
typedef struct _DtuIoctlGetPropertyInput {
    Int  m_TypeNumber;                      // Type number (-1 = get for current device)
    Int  m_HardwareRevision;                // Hardware revision
    Int  m_FirmwareVersion;                 // Firmware version
    Int  m_PortIndex;                       // Port index
    char  m_Name[PROPERTY_NAME_MAX_SIZE];   // Name of property
} DtuIoctlGetPropertyInput;
ASSERT_SIZE(DtuIoctlGetPropertyInput, 68)

typedef struct _DtuIoctlGetProperty2Input {
    Int  m_TypeNumber;                      // Type number (-1 = get for current device)
    Int  m_HardwareRevision;                // Hardware revision
    Int  m_FirmwareVersion;                 // Firmware version
    Int  m_PortIndex;                       // Port index
    char  m_Name[PROPERTY_NAME_MAX_SIZE];   // Name of property
    Int  m_DtapiMaj;                        // DTAPI major version
    Int  m_DtapiMin;                        // DTAPI minor version
    Int  m_DtapiBugfix;                     // DTAPI bug fix version
} DtuIoctlGetProperty2Input;
ASSERT_SIZE(DtuIoctlGetProperty2Input, 80)

// Ioctl output data type
// Don't use ENUMS in Ioctl's. Size is unknown
typedef struct _DtuIoctlGetPropertyOutput {
    Int  m_Scope;                           // Scope of property
    Int  m_Type;                            // Type of property
    UInt64A  m_Value;                       // Property value
} DtuIoctlGetPropertyOutput;
ASSERT_SIZE(DtuIoctlGetPropertyOutput, 16)

#ifdef WINBUILD
    #define DTU_IOCTL_GET_PROPERTY  CTL_CODE(DTU_DEVICE_TYPE, FUNC_DTU_GET_PROPERTY, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlGetPropertyData {
        DtuIoctlGetPropertyInput  m_Input;
        DtuIoctlGetPropertyOutput  m_Output;
    } DtuIoctlGetPropertyData;

    #define DTU_IOCTL_GET_PROPERTY  _IOWR(DTU_IOCTL_MAGIC, FUNC_DTU_GET_PROPERTY, \
                                                                  DtuIoctlGetPropertyData)
#endif

#ifdef WINBUILD
    #define DTU_IOCTL_GET_PROPERTY2  CTL_CODE(DTU_DEVICE_TYPE, FUNC_DTU_GET_PROPERTY2, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlGetProperty2Data {
        DtuIoctlGetProperty2Input  m_Input;
        DtuIoctlGetPropertyOutput  m_Output;
    } DtuIoctlGetProperty2Data;

    #define DTU_IOCTL_GET_PROPERTY2  _IOWR(DTU_IOCTL_MAGIC, FUNC_DTU_GET_PROPERTY2, \
                                                                 DtuIoctlGetProperty2Data)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_GET_DEV_INFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Gets the device information
//
typedef struct _DtuIoctlGetDevInfoOutput {
    UInt16  m_ProductId;        // Product ID
    UInt16  m_VendorId;         // Vendor ID
    UInt  m_UsbAddress;         // USB address assigned to device
    Int  m_UsbSpeed;            // USB Speed
    Int  m_TypeNumber;          // Type number in decimal, e.g. 205 for DTU-205
    Int  m_HardwareRevision;    // Hardware Revision
    Int  m_FirmwareVersion;     // Firmware Version (= Altera revision), e.g. 3 for
                                // "Firmware Version 3"
    Int  m_FirmwareVariant;     // Firmware Variant, e.g. to distinguish between
                                // firmware with different #inputs/#outputs
    UInt64A  m_Serial;          // Serial number
} DtuIoctlGetDevInfoOutput;
ASSERT_SIZE(DtuIoctlGetDevInfoOutput, 40)

#ifdef WINBUILD
    #define DTU_IOCTL_GET_DEV_INFO  CTL_CODE(DTU_DEVICE_TYPE, \
                          FUNC_DTU_GET_DEV_INFO, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlGetDevInfoData {
        DtuIoctlGetDevInfoOutput  m_Output;
    } DtuIoctlGetDevInfoData;

    #define DTU_IOCTL_GET_DEV_INFO  _IOR(DTU_IOCTL_MAGIC, FUNC_DTU_GET_DEV_INFO, \
                                                             DtuIoctlGetDevInfoData)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_VPD_CMD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Wrapper command to performs a VPD read, delete or write command.
//

// DTU VPD commands
#define DTU_VPD_CMD_ITEM_READ          1
#define DTU_VPD_CMD_ITEM_WRITE         2
#define DTU_VPD_CMD_ITEM_DELETE        3
#define DTU_VPD_CMD_RAW_READ           4
#define DTU_VPD_CMD_RAW_WRITE          5

// DTU VPD section ID's
#define DTU_VPD_SECT_ID                1
#define DTU_VPD_SECT_RO                2
#define DTU_VPD_SECT_RW                4

// DTU VPD flags
#define DTU_VPD_FLAG_RO_WRITE_ALLOWED  1
#define DTU_VPD_FLAG_UPDATE_EEPROM     2

// DTU VPD size constants
#define DTU_VPD_MAX_KEY_LENGTH         15

// DTU VPD read item command input data type
typedef struct _DtuIoctlVpdCmdReadItemInput {
    UInt  m_SectionType;
    UInt  m_KeywordLen;
    char  m_Keyword[DTU_VPD_MAX_KEY_LENGTH];
    UInt  m_Flags;                              // Not used for now
    UInt  m_OutBufLen;
} DtuIoctlVpdCmdReadItemInput;
ASSERT_SIZE(DtuIoctlVpdCmdReadItemInput, 32)

// DTU VPD write item command input data type
typedef struct _DtuIoctlVpdCmdWriteItemInput {
    UInt  m_SectionType;
    UInt  m_KeywordLen;
    char  m_Keyword[DTU_VPD_MAX_KEY_LENGTH];
    UInt  m_Flags;
    UInt  m_BufLen;
    UInt8  m_Buf[0];                // Dynamic sized buffer
} DtuIoctlVpdCmdWriteItemInput;
ASSERT_SIZE(DtuIoctlVpdCmdWriteItemInput, 32)

// DTU VPD delete item command input data type
typedef struct _DtuIoctlVpdCmdDeleteItemInput {
    UInt  m_SectionType;
    UInt  m_KeywordLen;
    char  m_Keyword[DTU_VPD_MAX_KEY_LENGTH];
    UInt  m_Flags;
} DtuIoctlVpdCmdDeleteItemInput;
ASSERT_SIZE(DtuIoctlVpdCmdDeleteItemInput, 28)

// DTU VPD read raw command input data type
typedef struct _DtuIoctlVpdCmdReadRawInput {
    UInt  m_StartOffset;
    UInt  m_OutBufLen;
} DtuIoctlVpdCmdReadRawInput;
ASSERT_SIZE(DtuIoctlVpdCmdReadRawInput, 8)

// DTU VPD write raw command input data type
typedef struct _DtuIoctlVpdCmdWriteRawInput {
    UInt  m_StartOffset;
    UInt  m_BufLen;
    UInt8  m_Buf[0];                // Dynamic sized buffer
} DtuIoctlVpdCmdWriteRawInput;
ASSERT_SIZE(DtuIoctlVpdCmdWriteRawInput, 8)

// Ioctl input data type
typedef struct _DtuIoctlVpdCmdInput {
    UInt  m_Cmd;
    union {
        DtuIoctlVpdCmdReadItemInput  m_ReadItem;
        DtuIoctlVpdCmdWriteItemInput  m_WriteItem;
        DtuIoctlVpdCmdDeleteItemInput  m_DeleteItem;
        DtuIoctlVpdCmdReadRawInput  m_ReadRaw;
        DtuIoctlVpdCmdWriteRawInput  m_WriteRaw;
    } m_Data;
} DtuIoctlVpdCmdInput;
ASSERT_SIZE(DtuIoctlVpdCmdInput, 36)

// DTU VPD read item command output data type
typedef struct _DtuIoctlVpdCmdReadItemOutput {
    UInt  m_Length;
    UInt8  m_Buf[0];                // Dynamic sized buffer
} DtuIoctlVpdCmdReadItemOutput;
ASSERT_SIZE(DtuIoctlVpdCmdReadItemOutput, 4)

// DTU VPD read raw command output data type
typedef struct _DtuIoctlVpdCmdReadRawOutput {
    UInt  m_Length;
    UInt8  m_Buf[0];                // Dynamic sized buffer
} DtuIoctlVpdCmdReadRawOutput;
ASSERT_SIZE(DtuIoctlVpdCmdReadRawOutput, 4)

// Ioctl output data type
typedef struct _DtuIoctlVpdCmdOutput {
    union {
        DtuIoctlVpdCmdReadItemOutput  m_ReadItem;
        DtuIoctlVpdCmdReadRawOutput  m_ReadRaw;
    } m_Data;
} DtuIoctlVpdCmdOutput;
ASSERT_SIZE(DtuIoctlVpdCmdOutput, 4)

#ifdef WINBUILD
    #define DTU_IOCTL_VPD_CMD  CTL_CODE(DTU_DEVICE_TYPE, FUNC_DTU_VPD_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlVpdCmdData {
        DtuIoctlVpdCmdInput  m_Input;
        DtuIoctlVpdCmdOutput  m_Output;
    } DtuIoctlVpdCmdData;

    #define DTU_IOCTL_VPD_CMD  _IOWR(DTU_IOCTL_MAGIC_SIZE, FUNC_DTU_VPD_CMD, \
                                                                       DtuIoctlVpdCmdData)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_GET_DRIVER_VERSION -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Gets a port info item from the drivers device description
//

typedef struct _DtuIoctlGetDriverVersionOutput {
    Int  m_Major;
    Int  m_Minor;
    Int  m_Micro;
    Int  m_Build;
} DtuIoctlGetDriverVersionOutput;
ASSERT_SIZE(DtuIoctlGetDriverVersionOutput, 16)

#ifdef WINBUILD
    #define DTU_IOCTL_GET_DRIVER_VERSION  CTL_CODE(DTU_DEVICE_TYPE, \
                           FUNC_DTU_GET_DRIVER_VERSION, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlGetDriverVersionData {
        DtuIoctlGetDriverVersionOutput  m_Output;
    } DtuIoctlGetDriverVersionData;

    #define DTU_IOCTL_GET_DRIVER_VERSION  _IOR(DTU_IOCTL_MAGIC, \
                                FUNC_DTU_GET_DRIVER_VERSION, DtuIoctlGetDriverVersionData)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_I2C_CMD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Wrapper command to perform a I2C read or write command.
//

// Constants
#define DTU_I2C_MAX_BUF_SIZE  512 // Size of I2C buffer

// DTU I2C commands
#define DTU_I2C_CMD_LOCK      1
#define DTU_I2C_CMD_UNLOCK    2
#define DTU_I2C_CMD_READ      3
#define DTU_I2C_CMD_WRITE     4

// DTU I2C lock command input data type
typedef struct _DtuIoctlI2cCmdLockInput {
    Int  m_TimeoutMS;
} DtuIoctlI2cCmdLockInput;
ASSERT_SIZE(DtuIoctlI2cCmdLockInput, 4)

// DTU I2C read command input data type
typedef struct _DtuIoctlI2cCmdReadInput {
    UInt  m_DevAddr;
    UInt  m_OutBufLen;
} DtuIoctlI2cCmdReadInput;
ASSERT_SIZE(DtuIoctlI2cCmdReadInput, 8)

// DTU I2C write command input data type
typedef struct _DtuIoctlI2cCmdWriteInput {
    UInt  m_DevAddr;
    UInt  m_BufLen;
    UInt8  m_Buf[DTU_I2C_MAX_BUF_SIZE];
} DtuIoctlI2cCmdWriteInput;
ASSERT_SIZE(DtuIoctlI2cCmdWriteInput, 520)

// Ioctl input data type
typedef struct _DtuIoctlI2cCmdInput {
    UInt  m_Cmd;
    Int  m_PortIndex;                       // Port index for future use
    union {
        DtuIoctlI2cCmdLockInput  m_Lock;
        DtuIoctlI2cCmdReadInput  m_Read;
        DtuIoctlI2cCmdWriteInput  m_Write;
    } m_Data;
} DtuIoctlI2cCmdInput;
ASSERT_SIZE(DtuIoctlI2cCmdInput, 528)

// DTU I2C read command output data type
typedef struct _DtuIoctlI2cCmdReadOutput {
    UInt  m_Length;
    UInt8  m_Buf[DTU_I2C_MAX_BUF_SIZE];
} DtuIoctlI2cCmdReadOutput;
ASSERT_SIZE(DtuIoctlI2cCmdReadOutput, 516)

// Ioctl output data type
typedef struct _DtuIoctlI2cCmdOutput {
    union {
        DtuIoctlI2cCmdReadOutput  m_Read;
    } m_Data;
} DtuIoctlI2cCmdOutput;
ASSERT_SIZE(DtuIoctlI2cCmdOutput, 516)

#ifdef WINBUILD
    #define DTU_IOCTL_I2C_CMD  CTL_CODE(DTU_DEVICE_TYPE, FUNC_DTU_I2C_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlI2cCmdData {
        DtuIoctlI2cCmdInput  m_Input;
        DtuIoctlI2cCmdOutput  m_Output;
    } DtuIoctlI2cCmdData;

    #define DTU_IOCTL_I2C_CMD  _IOWR(DTU_IOCTL_MAGIC, FUNC_DTU_I2C_CMD, \
                                                                       DtuIoctlI2cCmdData)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_GET_EVENT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Get the pending events from the driver
//
// Event types
#define DTU_EVENT_TYPE_POWER         0x00000001
#define DTU_EVENT_TYPE_TEST          0x80000000
#define DTU_EVENT_TYPE_ALL           0xFFFFFFFF
// Power event values
#define DTU_EVENT_VALUE1_POWER_DOWN  0x00000001
#define DTU_EVENT_VALUE1_POWER_UP    0x00000002

typedef struct _DtuIoctlGetEventOutput {
    UInt  m_EventType;
    UInt  m_Value1;
    UInt  m_Value2;
} DtuIoctlGetEventOutput;
ASSERT_SIZE(DtuIoctlGetEventOutput, 12)

#ifdef WINBUILD
    #define DTU_IOCTL_GET_EVENT  CTL_CODE(DTU_DEVICE_TYPE, \
                                   FUNC_DTU_GET_EVENT, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlGetEvent {
        DtuIoctlGetEventOutput  m_Output;
    } DtuIoctlGetEvent;

    #define DTU_IOCTL_GET_EVENT  _IOR(DTU_IOCTL_MAGIC, \
                                                   FUNC_DTU_GET_EVENT, DtuIoctlGetEvent)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_REGISTER_EVENTS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Registers the events to be interested in 
//

typedef struct _DtuIoctlRegisterEventsInput {
    UInt  m_EventTypeMask;            // Holds the event types to be interested in
} DtuIoctlRegisterEventsInput;
ASSERT_SIZE(DtuIoctlRegisterEventsInput, 4)

#ifdef WINBUILD
    #define DTU_IOCTL_REGISTER_EVENTS  CTL_CODE(DTU_DEVICE_TYPE, \
                              FUNC_DTU_REGISTER_EVENTS, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlRegisterEvents {
        DtuIoctlRegisterEventsInput  m_Input;
    } DtuIoctlRegisterEvents;

    #define DTU_IOCTL_REGISTER_EVENTS  _IOW(DTU_IOCTL_MAGIC, \
                                         FUNC_DTU_REGISTER_EVENTS, DtuIoctlRegisterEvents)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_UNREGISTER_EVENTS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// UnRegisters the events
//

#ifdef WINBUILD
    #define DTU_IOCTL_UNREGISTER_EVENTS  CTL_CODE(DTU_DEVICE_TYPE, \
                            FUNC_DTU_UNREGISTER_EVENTS, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    #define DTU_IOCTL_UNREGISTER_EVENTS  _IO(DTU_IOCTL_MAGIC, FUNC_DTU_REGISTER_EVENTS)
#endif


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_REG_READ -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read a register

typedef struct _DtuIoctlRegReadInput {
    UInt32  m_RegAddr;      // Register address offset
} DtuIoctlRegReadInput;
ASSERT_SIZE(DtuIoctlRegReadInput, 4)

typedef struct _DtuIoctlRegReadOutput {
    UInt32  m_Value;
} DtuIoctlRegReadOutput;
ASSERT_SIZE(DtuIoctlRegReadOutput, 4)

#ifdef WINBUILD
    #define DTU_IOCTL_REG_READ  CTL_CODE(DTU_DEVICE_TYPE, \
                                     FUNC_DTU_REG_READ, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlRegRead {
        DtuIoctlRegReadInput  m_Input;
        DtuIoctlRegReadOutput  m_Output;
    } DtuIoctlRegRead;

    #define DTU_IOCTL_REG_READ  _IOWR(DTU_IOCTL_MAGIC, \
                                                       FUNC_DTU_REG_READ, DtuIoctlRegRead)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_REG_WRITE_MASKED -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write masked value to register

typedef struct _DtuIoctlRegWriteMaskedInput {
    UInt32  m_RegAddr;      // Register address offset
    UInt32  m_FieldMask;    // AND-mask to get field value in register
    UInt32  m_FieldShift;   // Position of field in register (#times to shift left)
    UInt32  m_FieldValue;   // Value to write in field
} DtuIoctlRegWriteMaskedInput;
ASSERT_SIZE(DtuIoctlRegWriteMaskedInput, 16)

#ifdef WINBUILD
    #define DTU_IOCTL_REG_WRITE_MASKED  CTL_CODE(DTU_DEVICE_TYPE, \
                             FUNC_DTU_REG_WRITE_MASKED, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlRegWriteMasked {
        DtuIoctlRegWriteMaskedInput  m_Input;
    } DtuIoctlRegWriteMasked;

    #define DTU_IOCTL_REG_WRITE_MASKED  _IOW(DTU_IOCTL_MAGIC, \
                                        FUNC_DTU_REG_WRITE_MASKED, DtuIoctlRegWriteMasked)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_READ_DATA -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read data from device to user buffer

typedef struct _DtuIoctlReadDataInput {
    Int  m_PortIndex;           // Port index
#ifdef LINBUILD
    UInt64A  m_BufferAddr;
    Int  m_NumBytesToRead;
#endif
} DtuIoctlReadDataInput;
#ifdef LINBUILD
ASSERT_SIZE(DtuIoctlReadDataInput, 24)
#else
ASSERT_SIZE(DtuIoctlReadDataInput, 4)
#endif

typedef struct _DtuIoctlReadDataOutput {
    Int  m_NumBytesRead;
} DtuIoctlReadDataOutput;
ASSERT_SIZE(DtuIoctlReadDataOutput, 4)

#ifdef WINBUILD
    #define DTU_IOCTL_READ_DATA  CTL_CODE(DTU_DEVICE_TYPE, \
                                    FUNC_DTU_READ_DATA, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlReadData {
        DtuIoctlReadDataInput  m_Input;
        DtuIoctlReadDataOutput  m_Output;
    } DtuIoctlReadData;

    #define DTU_IOCTL_READ_DATA  _IOWR(DTU_IOCTL_MAGIC, FUNC_DTU_READ_DATA, \
                                                                         DtuIoctlReadData)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_WRITE_DATA -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write data from user buffer to device

typedef struct _DtuIoctlWriteDataInput {
    Int  m_PortIndex;           // Port index
#ifdef LINBUILD
    UInt64A  m_BufferAddr;
    Int  m_NumBytesToWrite;
#endif
} DtuIoctlWriteDataInput;
#ifdef LINBUILD
ASSERT_SIZE(DtuIoctlWriteDataInput, 24)
#else
ASSERT_SIZE(DtuIoctlWriteDataInput, 4)
#endif

#ifdef WINBUILD
    #define DTU_IOCTL_WRITE_DATA  CTL_CODE(DTU_DEVICE_TYPE, \
                                    FUNC_DTU_WRITE_DATA, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlWriteData {
        DtuIoctlWriteDataInput  m_Input;
    } DtuIoctlWriteData;

    #define DTU_IOCTL_WRITE_DATA  _IOW(DTU_IOCTL_MAGIC, FUNC_DTU_WRITE_DATA, \
                                                                        DtuIoctlWriteData)
#endif 


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTU_IOCTL_SH_BUF_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Wrapper command to perform operation on a shared buffer user-driver
//
// DTU shared buf commands
#define DTU_SH_BUF_CMD_INIT            1
#define DTU_SH_BUF_CMD_CLOSE           2

// DTU shared Buf init command input data type
typedef struct _DtUIoctlShBufCmdInitInput {
#ifdef LINBUILD
    UInt64A  m_BufferAddr;
    Int  m_BufferSize;
#else
    Int  m_Dummy;
#endif
} DtuIoctlShBufCmdInitInput;
#ifdef LINBUILD
ASSERT_SIZE(DtuIoctlShBufCmdInitInput, 16)
#else
ASSERT_SIZE(DtuIoctlShBufCmdInitInput, 4)
#endif

// Ioctl input data type
typedef struct _DtuIoctlShBufCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;           // Port index
    Int  m_BufferIndex;         // Only 1 buffer supported for now. Must be 0
    union {
        DtuIoctlShBufCmdInitInput  m_Init;
    } m_Data;
} DtuIoctlShBufCmdInput;
#ifdef LINBUILD
ASSERT_SIZE(DtuIoctlShBufCmdInput, 32)
#else
ASSERT_SIZE(DtuIoctlShBufCmdInput, 16)
#endif

#ifdef WINBUILD
    #define DTU_IOCTL_SH_BUF_CMD  CTL_CODE(DTU_DEVICE_TYPE, FUNC_DTU_SH_BUF_CMD, \
                                                         METHOD_IN_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlShBufCmdData {
        DtuIoctlShBufCmdInput  m_Input;
    } DtuIoctlShBufCmdData;

    #define DTU_IOCTL_SH_BUF_CMD  _IOWR(DTU_IOCTL_MAGIC, FUNC_DTU_SH_BUF_CMD, \
                                                                     DtuIoctlShBufCmdData)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IO configuration -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_GET_IO_CONFIG -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Get port IO configurations
//

// DtuIoctlGetIoConfigInput
typedef struct _DtuIoctlGetIoConfigInput {
    int  m_IoConfigCount;
    DtIoConfigId m_IoCfgId[0];
} DtuIoctlGetIoConfigInput;
ASSERT_SIZE(DtuIoctlGetIoConfigInput, 4)

// DtuIoctlGetIoConfigOutput
typedef struct _DtuIoctlGetIoConfigOutput {
    DtIoConfigValue m_IoCfgValue[0];
} DtuIoctlGetIoConfigOutput;

#ifdef WINBUILD
    #define DTU_IOCTL_GET_IOCONFIG  CTL_CODE(DTU_DEVICE_TYPE, FUNC_DTU_GET_IOCONFIG, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlGetIoConfig {
        DtuIoctlGetIoConfigInput  m_Input;
        DtuIoctlGetIoConfigOutput  m_Output;
    } DtuIoctlGetIoConfig;

    #define DTU_IOCTL_GET_IOCONFIG  _IOWR(DTU_IOCTL_MAGIC_SIZE, FUNC_DTU_GET_IOCONFIG, \
                                                                      DtuIoctlGetIoConfig)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_SET_IOCONFIG -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Set port IO configurations
//

typedef struct _DtuIoctlSetIoConfigInput {
    Int  m_IoConfigCount;
    DtIoConfigPars  m_IoConfig[0];
} DtuIoctlSetIoConfigInput;
ASSERT_SIZE(DtuIoctlSetIoConfigInput, 8)

#ifdef WINBUILD
    #define DTU_IOCTL_SET_IOCONFIG  CTL_CODE(DTU_DEVICE_TYPE, FUNC_DTU_SET_IOCONFIG, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlSetIoConfig {
        DtuIoctlSetIoConfigInput  m_Input;
    } DtuIoctlSetIoConfig;

    #define DTU_IOCTL_SET_IOCONFIG  _IOWR(DTU_IOCTL_MAGIC_SIZE, FUNC_DTU_SET_IOCONFIG, \
                                                                      DtuIoctlSetIoConfig)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_NONIP_CMD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Wrapper IOCTL for all non IP commands.
//

// Command definitions
#define DTU_NONIP_CMD_EXCLUSIVE_ACCESS  1
#define DTU_NONIP_CMD_GET_TARGET_ID     2
#define DTU_NONIP_CMD_DETECT_VIDSTD     3


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_NONIP_CMD_EXCLUSIVE_ACCESS -.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Exclusive access cmd's
#define DTU_EXCLUSIVE_ACCESS_CMD_ACQUIRE  1
#define DTU_EXCLUSIVE_ACCESS_CMD_RELEASE  2
#define DTU_EXCLUSIVE_ACCESS_CMD_PROBE    3

// Exlusive Access input data
typedef struct _DtuIoctlNonIpCmdExclusiveAccessInput {
    Int  m_Cmd;                 // Acquire/release/probe command
} DtuIoctlNonIpCmdExclusiveAccessInput;
ASSERT_SIZE(DtuIoctlNonIpCmdExclusiveAccessInput, 4)
    
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_NONIP_CMD_DETECT_VIDSTD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Ioctl output data type
typedef struct _DtuIoctlNonIpCmdDetectVidStdOutput {
    Int  m_VidStd;              // Detected video standard
} DtuIoctlNonIpCmdDetectVidStdOutput;
ASSERT_SIZE(DtuIoctlNonIpCmdDetectVidStdOutput, 4)

// Ioctl input data type
typedef struct _DtuIoctlNonIpCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;
    union {
        DtuIoctlNonIpCmdExclusiveAccessInput  m_ExclusiveAccess;
    } m_Data;
} DtuIoctlNonIpCmdInput;
ASSERT_SIZE(DtuIoctlNonIpCmdInput, 12)

// Ioctl Output data type
typedef struct _DtuIoctlNonIpCmdOutput {
    union {
        DtuIoctlNonIpCmdDetectVidStdOutput  m_DetVidStd;
    } m_Data;
} DtuIoctlNonIpCmdOutput;
ASSERT_SIZE(DtuIoctlNonIpCmdOutput, 4)


#ifdef WINBUILD
    #define DTU_IOCTL_NONIP_CMD  CTL_CODE(DTU_DEVICE_TYPE, FUNC_DTU_NONIP_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlNonIpCmd {
        DtuIoctlNonIpCmdInput  m_Input;
        DtuIoctlNonIpCmdOutput  m_Output;
    } DtuIoctlNonIpCmd;

    #define DTU_IOCTL_NONIP_CMD  _IOWR(DTU_IOCTL_MAGIC, FUNC_DTU_NONIP_CMD, \
                                                                         DtuIoctlNonIpCmd)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_NONIP_XX_CMD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Exclusive access cmd's
#define DTU_NONIP_XX_CMD_RESET     1
// Reset modes
#define DTU_RST_CLEARFIFO          0   // Clear FIFO without disrupting channel
#define DTU_RST_CHANNEL            1    // Channel reset

// Reset channel input data
typedef struct _DtuIoctlNonIpXxCmdResetInput {
    Int  m_ResetMode;                 // Reset mode
} DtuIoctlNonIpXxCmdResetInput;
ASSERT_SIZE(DtuIoctlNonIpXxCmdResetInput, 4)


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_NONIP_RX_CMD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Wrapper IOCTL for all non IP RX commands.
//
    
// Ioctl input data type
typedef struct _DtuIoctlNonIpRxCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;
    union {
        DtuIoctlNonIpXxCmdResetInput m_Reset;
    } m_Data;
} DtuIoctlNonIpRxCmdInput;
ASSERT_SIZE(DtuIoctlNonIpRxCmdInput, 12)

#ifdef WINBUILD
    #define DTU_IOCTL_NONIP_RX_CMD  CTL_CODE(DTU_DEVICE_TYPE, FUNC_DTU_NONIP_RX_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlNonIpRxCmd {
        DtuIoctlNonIpRxCmdInput  m_Input;
    } DtuIoctlNonIpRxCmd;

    #define DTU_IOCTL_NONIP_RX_CMD  _IOWR(DTU_IOCTL_MAGIC, FUNC_DTU_NONIP_RX_CMD, \
                                                                       DtuIoctlNonIpRxCmd)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_NONIP_TX_CMD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Wrapper IOCTL for all non IP TX commands.
//
    
// Ioctl input data type
typedef struct _DtuIoctlNonIpTxCmdInput {
    Int  m_Cmd;
    Int  m_PortIndex;
    union {
        DtuIoctlNonIpXxCmdResetInput m_Reset;
    } m_Data;
} DtuIoctlNonIpTxCmdInput;
ASSERT_SIZE(DtuIoctlNonIpTxCmdInput, 12)

#ifdef WINBUILD
    #define DTU_IOCTL_NONIP_TX_CMD  CTL_CODE(DTU_DEVICE_TYPE, FUNC_DTU_NONIP_TX_CMD, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlNonIpTxCmd {
        DtuIoctlNonIpTxCmdInput  m_Input;
    } DtuIoctlNonIpTxCmd;

    #define DTU_IOCTL_NONIP_TX_CMD  _IOWR(DTU_IOCTL_MAGIC, FUNC_DTU_NONIP_TX_CMD, \
                                                                       DtuIoctlNonIpTxCmd)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_RESET_SOFT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Reset DTU
//
  
#ifdef WINBUILD
    #define DTU_IOCTL_RESET_SOFT  CTL_CODE(DTU_DEVICE_TYPE, FUNC_DTU_RESET_SOFT, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    #define DTU_IOCTL_RESET_SOFT  _IO(DTU_IOCTL_MAGIC, FUNC_DTU_RESET_SOFT)
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_GET_TABLE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Gets the table entries for given port and table name from the TableStore.
//
#define DTU_TABLE_NAME_MAX_SIZE  50


// Ioctl input data type
typedef struct _DtuIoctlGetTableInput {
    Int  m_PortIndex;                           // Port index
    char  m_Name[DTU_TABLE_NAME_MAX_SIZE];      // Name of table
    UInt  m_MaxNumEntries;                      // Max. number of entry's to store
} DtuIoctlGetTableInput;
ASSERT_SIZE(DtuIoctlGetTableInput, 60)

// Ioctl output data type
typedef struct _DtuIoctlGetTableOutput {
    UInt  m_NumEntries;
    DtTableEntry  m_TableEntry[0];
} DtuIoctlGetTableOutput;
ASSERT_SIZE(DtuIoctlGetTableOutput, 8)

#ifdef WINBUILD
    #define DTU_IOCTL_GET_TABLE  CTL_CODE(DTU_DEVICE_TYPE, FUNC_DTU_GET_TABLE, \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlGetTable {
        DtuIoctlGetTableInput  m_Input;
        DtuIoctlGetTableOutput  m_Output;
    } DtuIoctlGetTable;

    #define DTU_IOCTL_GET_TABLE  _IOWR(DTU_IOCTL_MAGIC_SIZE, FUNC_DTU_GET_TABLE, \
                                                                         DtuIoctlGetTable)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_GET_STR_PROPERTY -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Gets a device or port string property value and type from the DtuProperyStore.
//

// Ioctl input data type
typedef struct _DtuIoctlGetStrPropertyInput {
    Int  m_TypeNumber;                      // Type number (-1 = get for current device)
    Int  m_HardwareRevision;                // Hardware revision
    Int  m_FirmwareVersion;                 // Firmware version
    Int  m_PortIndex;                       // Port index
    char  m_Name[PROPERTY_NAME_MAX_SIZE];   // Name of property
} DtuIoctlGetStrPropertyInput;
ASSERT_SIZE(DtuIoctlGetStrPropertyInput, 68)

// Ioctl output data type
typedef struct _DtuIoctlGetStrPropertyOutput {
    DtPropertyScope  m_Scope;               // Scope of property
    char  m_Str[PROPERTY_STR_MAX_SIZE];     // The requested string
} DtuIoctlGetStrPropertyOutput;
ASSERT_SIZE(DtuIoctlGetStrPropertyOutput, 100)

#ifdef WINBUILD
    #define DTU_IOCTL_GET_STR_PROPERTY  CTL_CODE(DTU_DEVICE_TYPE,                        \
                                                        FUNC_DTU_GET_STR_PROPERTY,       \
                                                        METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlGetStrPropertyData {
        DtuIoctlGetStrPropertyInput  m_Input;
        DtuIoctlGetStrPropertyOutput  m_Output;
    } DtuIoctlGetStrPropertyData;

    #define DTU_IOCTL_GET_STR_PROPERTY  _IOWR(DTU_IOCTL_MAGIC, FUNC_DTU_GET_STR_PROPERTY,\
                                                               DtuIoctlGetStrPropertyData)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_GET_DEV_SUBTYPE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Gets the device subtype
//
typedef struct _DtuIoctlGetDevSubTypeOutput {
    Int  m_SubType;             // Device subtype (-1=undefined, 0=none, 1=A, ...)
} DtuIoctlGetDevSubTypeOutput;
ASSERT_SIZE(DtuIoctlGetDevSubTypeOutput, 4)

#ifdef WINBUILD
    #define DTU_IOCTL_GET_DEV_SUBTYPE  CTL_CODE(DTU_DEVICE_TYPE, \
                          FUNC_DTU_GET_DEV_SUBTYPE, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlGetDevSubTypeData {
        DtuIoctlGetDevSubTypeOutput  m_Output;
    } DtuIoctlGetDevSubTypeData;

    #define DTU_IOCTL_GET_DEV_SUBTYPE  _IOR(DTU_IOCTL_MAGIC, FUNC_DTU_GET_DEV_SUBTYPE, \
                                                             DtuIoctlGetDevSubTypeData)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_REENUMERATE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Force the device to re-enumerate
//

#ifdef WINBUILD
    #define DTU_IOCTL_REENUMERATE  CTL_CODE(DTU_DEVICE_TYPE, \
                                  FUNC_DTU_REENUMERATE, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    #define DTU_IOCTL_REENUMERATE  _IO(DTU_IOCTL_MAGIC, FUNC_DTU_REENUMERATE)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_VENDOR_REQUEST -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
//
//

typedef struct _DtuIoctlVendorRequestInput {
    UInt8  m_Code;
    UInt16  m_Value;
    UInt16  m_Index;
    Int  m_Dir;
    UInt  m_BufLen;
    UInt8  m_Buf[0];                // Dynamic sized buffer
} DtuIoctlVendorRequestInput;
ASSERT_SIZE(DtuIoctlVendorRequestInput, 16)

typedef struct _DtuIoctlVendorRequestOutput {
    Int  m_NumBytesTransferred;
    UInt8  m_Buf[0];                // Dynamic sized buffer
} DtuIoctlVendorRequestOutput;
ASSERT_SIZE(DtuIoctlVendorRequestOutput, 4)

#ifdef WINBUILD
    #define DTU_IOCTL_VENDOR_REQUEST  CTL_CODE(DTU_DEVICE_TYPE, \
                               FUNC_DTU_VENDOR_REQUEST, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlVendorRequest {
        DtuIoctlVendorRequestInput  m_Input;
        DtuIoctlVendorRequestOutput  m_Output;
    } DtuIoctlVendorRequest;

    #define DTU_IOCTL_VENDOR_REQUEST  _IOWR(DTU_IOCTL_MAGIC_SIZE, \
                                           FUNC_DTU_VENDOR_REQUEST, DtuIoctlVendorRequest)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_GET_STATE_FLAGS -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get the state flags for a specific port or for the device (m_PortIndex=-1).
//

typedef struct _DtuIoctlGetStateFlagsInput {
    Int  m_PortIndex;                       // Port index
} DtuIoctlGetStateFlagsInput;
ASSERT_SIZE(DtuIoctlGetStateFlagsInput, 4)

typedef struct _DtuIoctlGetStateFlagsOutput {
    Int  m_Flags;
} DtuIoctlGetStateFlagsOutput;
ASSERT_SIZE(DtuIoctlGetStateFlagsOutput, 4)

#ifdef WINBUILD
    #define DTU_IOCTL_GET_STATE_FLAGS  CTL_CODE(DTU_DEVICE_TYPE, \
                              FUNC_DTU_GET_STATE_FLAGS, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlGetStateFlags {
        DtuIoctlGetStateFlagsInput  m_Input;
        DtuIoctlGetStateFlagsOutput  m_Output;
    } DtuIoctlGetStateFlags;

    #define DTU_IOCTL_GET_STATE_FLAGS  _IOWR(DTU_IOCTL_MAGIC, FUNC_DTU_GET_STATE_FLAGS, \
                                                                    DtuIoctlGetStateFlags)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_TRIGGER_WATCHDOG -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Simulate a watchdog timeout, causing a device reset.
//

#ifdef WINBUILD
    #define DTU_IOCTL_TRIGGER_WATCHDOG  CTL_CODE(DTU_DEVICE_TYPE, \
                             FUNC_DTU_TRIGGER_WATCHDOG, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    #define DTU_IOCTL_TRIGGER_WATCHDOG  _IO(DTU_IOCTL_MAGIC, FUNC_DTU_TRIGGER_WATCHDOG)
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_SET_RX_MODE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//

typedef struct _DtuIoctlSetRxModeInput {
    Int  m_PortIndex;
    Int  m_RxMode;
} DtuIoctlSetRxModeInput;
ASSERT_SIZE(DtuIoctlSetRxModeInput, 8)

typedef struct _DtuIoctlSetRxModeOutput {
    Int  m_FrameIdNewRxMode;
} DtuIoctlSetRxModeOutput;
ASSERT_SIZE(DtuIoctlSetRxModeOutput, 4)

#ifdef WINBUILD
    #define DTU_IOCTL_SET_RX_MODE  CTL_CODE(DTU_DEVICE_TYPE, \
                              FUNC_DTU_SET_RX_MODE, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlSetRxMode {
        DtuIoctlSetRxModeInput  m_Input;
        DtuIoctlSetRxModeOutput  m_Output;
    } DtuIoctlSetRxMode;

    #define DTU_IOCTL_SET_RX_MODE  _IOWR(DTU_IOCTL_MAGIC, FUNC_DTU_SET_RX_MODE, \
                                                                    DtuIoctlVendorRequest)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_UPLOAD_FPGA_FW -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//

typedef struct _DtuIoctlUploadFpgaFwInput {
#ifdef LINBUILD
    UInt64A  m_BufferAddr;
    Int  m_NumBytesToWrite;
#else
    Int  m_Dummy;
#endif
} DtuIoctlUploadFpgaFwInput;
#ifdef LINBUILD
ASSERT_SIZE(DtuIoctlUploadFpgaFwInput, 16)
#else
ASSERT_SIZE(DtuIoctlUploadFpgaFwInput, 4)
#endif

#ifdef WINBUILD
    #define DTU_IOCTL_UPLOAD_FPGA_FW  CTL_CODE(DTU_DEVICE_TYPE, \
                               FUNC_DTU_UPLOAD_FPGA_FW, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlUploadFpgaFw {
        DtuIoctlUploadFpgaFwInput  m_Input;
    } DtuIoctlUploadFpgaFw;

    #define DTU_IOCTL_UPLOAD_FPGA_FW  _IOW(DTU_IOCTL_MAGIC, FUNC_DTU_UPLOAD_FPGA_FW, \
                                                                     DtuIoctlUploadFpgaFw)
#endif 

//.-.-.-.-.-.-.-.-.-.-.-.-.- DTU_IOCTL_GET_DATA_BUF_SIZE_FLAGS -.-.-.-.-.-.-.-.-.-.-.-.-.-
//

typedef struct _DtuIoctlGetDataBufSizeInput {
    Int  m_PortIndex;
} DtuIoctlGetDataBufSizeInput;
ASSERT_SIZE(DtuIoctlGetDataBufSizeInput, 4)

typedef struct _DtuIoctlGetDataBufSizeOutput {
    Int  m_BufSize;
} DtuIoctlGetDataBufSizeOutput;
ASSERT_SIZE(DtuIoctlGetDataBufSizeOutput, 4)

#ifdef WINBUILD
    #define DTU_IOCTL_GET_DATA_BUF_SIZE  CTL_CODE(DTU_DEVICE_TYPE, \
                            FUNC_DTU_GET_DATA_BUF_SIZE, METHOD_OUT_DIRECT, FILE_READ_DATA)
#else
    typedef union _DtuIoctlGetDataBufSize {
        DtuIoctlGetDataBufSizeInput  m_Input;
        DtuIoctlGetDataBufSizeOutput  m_Output;
    } DtuIoctlGetDataBufSize;

    #define DTU_IOCTL_GET_DATA_BUF_SIZE  _IOWR(DTU_IOCTL_MAGIC, \
                                       FUNC_DTU_GET_DATA_BUF_SIZE, DtuIoctlGetDataBufSize)
#endif 

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuIoctlInputData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef union _DtuIoctlInputData {
    DtuIoctlGetPropertyInput  m_GetProperty;
    DtuIoctlGetProperty2Input  m_GetProperty2;
    DtuIoctlVpdCmdInput  m_VpdCmd;
    DtuIoctlI2cCmdInput  m_I2cCmd;
    DtuIoctlRegReadInput  m_RegRead;
    DtuIoctlRegWriteMaskedInput m_RegWriteMasked;
    DtuIoctlRegisterEventsInput  m_RegisterEvents;
    DtuIoctlReadDataInput  m_ReadData;
    DtuIoctlWriteDataInput  m_WriteData;
    DtuIoctlGetIoConfigInput  m_GetIoConfig;
    DtuIoctlSetIoConfigInput  m_SetIoConfig;
    DtuIoctlGetTableInput  m_GetTable;
    DtuIoctlGetStrPropertyInput  m_GetStrProperty;
    DtuIoctlVendorRequestInput  m_VendorRequest;
    DtuIoctlGetStateFlagsInput  m_GetStateFlags;
    DtuIoctlSetRxModeInput  m_RxMode;
    DtuIoctlUploadFpgaFwInput  m_UploadFpgaFw;
    DtuIoctlGetDataBufSizeInput  m_DataBufSize;
} DtuIoctlInputData;
ASSERT_SIZE(DtuIoctlInputData, 528)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuIoctlOutputData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef union _DtuIoctlOutputData {
    DtuIoctlGetPropertyOutput  m_GetProperty;
    DtuIoctlGetDevInfoOutput  m_GetDevInfo;
    DtuIoctlVpdCmdOutput  m_VpdCmd;
    DtuIoctlGetDriverVersionOutput  m_GetDriverVersion;
    DtuIoctlI2cCmdOutput  m_I2cCmd;
    DtuIoctlRegReadOutput  m_RegRead;
    DtuIoctlReadDataOutput  m_ReadData;
    DtuIoctlGetEventOutput  m_GetEvent;
    DtuIoctlGetIoConfigOutput  m_GetIoConfig;
    DtuIoctlGetTableOutput  m_GetTable;
    DtuIoctlGetStrPropertyOutput  m_GetStrProperty;
    DtuIoctlGetDevSubTypeOutput  m_GetDevSubType;
    DtuIoctlVendorRequestOutput  m_VendorRequest;
    DtuIoctlGetStateFlagsOutput  m_GetStateFlags;
    DtuIoctlSetRxModeOutput  m_RxMode;
    DtuIoctlGetDataBufSizeOutput  m_DataBufSize;
} DtuIoctlOutputData;
ASSERT_SIZE(DtuIoctlOutputData, 520)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuIoctlData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef union _DtuIoctlData {
    DtuIoctlInputData  m_Input;
    DtuIoctlOutputData  m_Output;
} DtuIoctlData;
ASSERT_SIZE(DtuIoctlData, 528)

//+=+=+=+=+=+=+=+=+=+=+=+ DTU-351 shared buffer header definitions +=+=+=+=+=+=+=+=+=+=+=+

typedef struct _Dtu351DataHdr
{
    volatile UInt32  m_Flags;              // 0: not valid yet, 1: contains data
    volatile UInt32  m_NumValid;           // Number of bytes in this buffer
} Dtu351DataHdr;

typedef struct  _Dtu351BufHdr
{
    volatile UInt32  m_TotalBufSize;        // Total size of the buffer, including header
    volatile UInt32  m_Overflow;
    volatile UInt32  m_NumAsyncReads;       // Number of transfers to queue at same time
    volatile UInt32  m_NumBuffers;          // Number of data buffers
    volatile UInt32  m_FirstBuf;
} Dtu351BufHdr;

#pragma pack (pop)

#undef Int64
#undef UInt64

#endif // __DTU_COMMON_H
