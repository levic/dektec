// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBb2Common.h *#*#*#*#*#*#*#* (C) 2021-2022 DekTec
//
// DtBb2 - Interface between DtBb2 driver (C) and DTAPI (C++), for Windows and Linux
//
// This file defines the I/O controls (IOCTLs) used by DTAPI to communicate with the 
// DtBb2 driver:
//  - IOCTL codes, e.g. DTBB2_IOCTL_DEVICE;
//  - Command codes per IOCTL, e.g. DtBb2Device_GetDeviceInfo;
//  - In and output data structs per IOCTL.
//

// +=+=+=+=+=+=+=+=+=+=+=+=+ MAINTAINING BACKWARDS COMPATIBILITY +=+=+=+=+=+=+=+=+=+=+=+=+
//                                                                                      // 
// This header file contains enumerations for IOCTL function- and command codes.        //       
//                                                                                      //
// ALWAYS ADD NEW FUNCTION- AND COMMAND CODES AT THE END OF THE ENUMERATION.            //
//                                                                                      //
// This will ensure that the value of existing codes remains the same, so that new      //
// driver versions remain compatible with user applications that were compiled against  //
// an older driver version.                                                             //
//                                                                                      //
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// =+=+=+=+=+=+=+=+=+=+=+ I/O CONTROLS DEFINED IN THIS HEADER FILE +=+=+=+=+=+=+=+=+=+=+=+
//                                                                                      //
// DTBB2_IOCTL_BRP          Execute a BRP (BB2.0 Request Package), for controlling      //
//                          building blocks and peripherals.                            //
// DTBB2_IOCTL_DATAIN       Data input and output.                                      //
// DTBB2_IOCTL_DEVICE       Device-level I/O controls, e.g. switch firmware variant.    //
// DTBB2_IOCTL_DRIVER       Driver-level I/O controls, e.g. get driver version.         //
// DTBB2_IOCTL_EXCL_ACCESS  Requesting and releasing exclusive access.                  //
// DTBB2_IOCTL_EVENT        Event interface.                                            //
// DTBB2_IOCTL_VPD          Read and write VPD EEPROM.                                  //
//                                                                                      //
// To be elaborated later:                                                              //
// DTBB2_IOCTL_IOCONFIG     I/O configuration.                                          //
//                                                                                      //
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2021 DekTec Digital Video B.V.
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

#pragma once

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#ifdef WINBUILD
#include "GuidDef.h"
#endif

// Use C++ 11 fixed-width integer types both for DTAPI (C++ 11) and driver (C).
#ifdef DTAPI
    #include <cstdint>
#else
    typedef int int32_t;
    typedef long long int64_t;
    typedef unsigned char uint8_t;
    typedef unsigned short uint16_t;
    typedef unsigned int uint32_t;
#endif

#ifdef WINBUILD
    // .-.-.-.-.-.-.-.-.-.-.-.-.- Windows-Specific Definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-

    // Standard Windows IOCTL for function code 'f'.
    #define WIN_IOCTL(f) CTL_CODE(DTBB2_DEVICE_TYPE, f, METHOD_OUT_DIRECT, FILE_READ_DATA)

    // Driver GUID: {F9882BF6-B32D-4098-801B-EFB0AD1135F9}.
    // Identifies the DTBB2 driver interface on Windows.
    static const GUID GUID_DEVINTERFACE_DTBB2 = 
           {0xf9882bf6, 0xb32d, 0x4098, {0x80, 0x1b, 0xef, 0xb0, 0xad, 0x11, 0x35, 0xf9}};

    // GUID from custom device ready event {76B74A03-CBB2-41FA-A6CC-5955BD7CD198}.
    DEFINE_GUID(DeviceReadyEventGuid, 
              0x76b74a03, 0xcbb2, 0x41fa, 0xa6, 0xcc, 0x59, 0x55, 0xbd, 0x7c, 0xd1, 0x98);

    // Disable warning about zero-size array extension.
    #pragma warning(disable : 4200)

    #ifdef DTAPI
        #include <winioctl.h>
    #endif // DTAPI

#endif // WINBUILD

#ifndef WINBUILD
    // -.-.-.-.-.-.-.-.-.-.-.-.-.- Linux Specific Definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // Standard Linux IOCTL for functions code 'f'.
    // Since we're using MAGIC_SIZE , the size is passed in two additional fields in the
    // IO-control. A dummy type is passed in the _IO macros 
    #define LIN_IOCTL_R(f) _IOR(DTBB2_IOCTL_MAGIC_SIZE, f, int)
    #define LIN_IOCTL_W(f) _IOW(DTBB2_IOCTL_MAGIC_SIZE, f, int)
    #define LIN_IOCTL_WR(f) _IOWR(DTBB2_IOCTL_MAGIC_SIZE, f, int)

    #ifdef DTAPI
        #include <sys/ioctl.h>
    #endif // DTAPI

#endif // !WINBUILD

// Start of DtBb2 namespace in C++.
#ifdef __cplusplus
namespace DtBb2 { 
#endif

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBb2 IOCTL Codes +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

#ifdef WINBUILD
    // .-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL Definitions for Windows -.-.-.-.-.-.-.-.-.-.-.-.-.

    //                  METHOD_   METHOD_
    //                 IN_DIRECT OUT_DIRECT
    // 'Normal' IOCTLs    No        Yes     METHOD_IN_DIRECT is not needed because output 
    //                                      buffers are never used as input.
    // DMA                Yes       Yes
    // Shared buffers     Yes       No      Out buffer can be used as input and output.
    // 
    // Note: METHOD_NEITHER is not used at all.

    #define DTBB2_DEVICE_TYPE       45201
    #define DTBB2_IOCTL_BASE        3400         // First function code used for DtBb2.

#else
    // -.-.-.-.-.-.-.-.-.-.-.-.-.- IOCTL Definitions for Linux -.-.-.-.-.-.-.-.-.-.-.-.-.-

    #define DTBB2_IOCTL_MAGIC       0xDC         // Magic number.
    #define DTBB2_IOCTL_MAGIC_SIZE  0xDD         // Magic for IOCTLs with dynamic size.
    #define DTBB2_IOCTL_BASE        0x60         // First function code used for DtBb2.

#endif // WINBUILD

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBb2 IOCTL Function Codes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

enum {
    FuncCode_Access = DTBB2_IOCTL_BASE,
    FuncCode_Device,
    FuncCode_Driver, 
    FuncCode_IoConfig,
    FuncCode_SendBrp,
    FuncCode_Stream,
    FuncCode_Vpd
    // ADD NEW FUNCTION CODES HERE TO MAINTAIN BACKWARDS COMPATIBILITY.
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.- Macro DTBB2_IOCTL_TO_FUNC_CODE -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// 
// Extracts the function code from an IOCTL code (macro).
// 
// Note:
//   An IOCTL is an OR of several fields (e.g. DeviceType, RequiredAccess, FunctionCode,
//   TransferType and on Linux also the size of the IOCTL structure is part of the code).
//   When this structure grows/shrinks the IOCTL code changes, breaking backward 
//   compatibility. Therefore one should use only the function code to determine
//   which IOCTL is received.
//
#ifdef WINBUILD 
    #define DTBB2_IOCTL_TO_FUNC_CODE(IoctlCode)  ((uint32_t)((IoctlCode>>2)&0xFFF))
#else
    #define DTBB2_IOCTL_TO_FUNC_CODE(IoctlCode)  ((uint32_t)_IOC_NR(IoctlCode))
#endif

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- Macro DTBB2_SIZEOF_DYN_INOUT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Computes the size of an input/output structure containing a dynamically sized buffer.
// The input/output structure must contain exactly 1 buffer element.
// 
// Usage example:
//   DTBB2_SIZEOF_DYN_INOUT(IoctlBrpExecuteOut, uint8_t, 1000)
//
#define DTBB2_SIZEOF_DYN_INOUT(STRUCT_TYPE, ELEM_TYPE, NUM_ELEMS)                        \
                          ((int)(sizeof(STRUCT_TYPE) + sizeof(ELEM_TYPE)*((NUM_ELEMS)-1)))

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct IoctlInHdr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Common header for input data of IOCTL commands.
//
typedef struct {
    int32_t Cmd;                    // (Sub)command to be executed.
    int32_t Id;                     // Identification of the driver (sub)function, e.g. 
                                    // interface ID, or in case of multiple BRPs).
                                    // Set to 0 if no ID is applicable.
} IoctlInHdr;

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IoctlSendBrp +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Request type.
typedef enum {
    BrpReq_Peripheral,
    BrpReq_ReadField,
    BrpReq_ReadInput,
    BrpReq_WriteField,
    BrpReq_WriteOutput
} BrpRequestType;

// Peripheral commands.
typedef enum {
    PeriphCmd_Read,
    PeriphCmd_Write,
    PeriphCmd_WriteRead
} PeriphCmdType;

// Peripheral request - Result code.
typedef enum {
    PeriphResult_Ok,
    PeriphResult_I2cAddrNack,
    PeriphResult_I2cDataNack,
    PeriphResult_I2cTimeout,
    PeriphResult_SpiTimeout
} PeriphResultCode;

#ifdef WINBUILD
    #define DTBB2_IOCTL_SENDBRP WIN_IOCTL(FuncCode_SendBrp)
#else
    #define DTBB2_IOCTL_SENDBRP LIN_IOCTL_WR(FuncCode_SendBrp)
#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IoctlStream +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- I/O control Stream -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Identifies the kind of Stream I/O control.
//
typedef enum {
    IoctlStream_TagGetStatus = 0, 
    IoctlStream_TagSetOffset, IoctlStream_TagStart, IoctlStream_TagStop,
} IoctlStreamCmd;

#ifdef WINBUILD
#define DTBB2_IOCTL_STREAM WIN_IOCTL(FuncCode_Stream)
#else
#define DTBB2_IOCTL_STREAM LIN_IOCTL_WR(FuncCode_Stream)
#endif

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ I/O Control 'Device' +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// I/O control 'Device' - Command codes.
typedef enum {
    IoctlDeviceCmd_CloseDriverInterface,
    IoctlDeviceCmd_CloseStream,
    IoctlDeviceCmd_GetInfo,
    IoctlDeviceCmd_GetSection,      // Get raw data for a .dat metadata section.
    IoctlDeviceCmd_GetSectionLen,   // Get length of a .dat metadata section.
    IoctlDeviceCmd_GetStatus,
    IoctlDeviceCmd_OpenDriverInterface,
    IoctlDeviceCmd_OpenStream,
    IoctlDeviceCmd_OverruleLedPattern,
    IoctlDeviceCmd_ManageEvent,
    IoctlDevice_SendBrp,
    IoctlDeviceCmd_SwitchVariant
    // ADD NEW CMDs HERE TO MAINTAIN BACKWARDS COMPATIBILITY.
} IoctlDeviceCmd;

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Device_ManageEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// EventCommand codes.
typedef enum {
    ManageEventCmd_Subscribe,
    ManageEventCmd_Unsubscribe,
    ManageEventCmd_Wait,
    ManageEventCmd_SimulateEvent                 // Only used for testing.
} ManageEventCmd;

// Device_ManageEvent codes.
// DO NOT CHANGE EXISTING VALUES FOR BACKWARD COMPATIBILITY.

// Event types used for  Device_ManageEvent.
typedef enum {
    ManageEventType_Power,
} ManageEventType;

// Power event values flags
#define DtBb2EventValue1PowerUp        0x00000001
#define DtBb2EventValue1PowerDown      0x00000002

#ifdef WINBUILD
    #define DTBB2_IOCTL_DEVICE WIN_IOCTL(FuncCode_Device)
#else
    #define DTBB2_IOCTL_DEVICE LIN_IOCTL_WR(FuncCode_Device)
#endif

// Device category.
typedef enum {
    DeviceCat_Pcie, 
    DeviceCat_Usb
} DeviceCat;

// USB speed.
typedef enum {
    UsbSpeed_FullSpeed,
    UsbSpeed_HighSpeed,
    UsbSpeed_SuperSpeed,
    UsbSpeed_SuperSpeedPlus,
    UsbSpeed_Unknown
} UsbDeviceSpeed;

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ I/O control 'Driver' +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Driver command codes.
typedef enum {
    IoctlDriverCmd_GetVersion,
    IoctlDriverCmd_GetCatalogSize,
    IoctlDriverCmd_GetCatalog,
    IoctlDriverCmd_ReadDotDatFile
    // ADD NEW CMDs HERE TO MAINTAIN BACKWARDS COMPATIBILITY.
} IoctlDriverCmd;

#ifdef WINBUILD
    #define DTBB2_IOCTL_DRIVER WIN_IOCTL(FuncCode_Driver)
#else
    #define DTBB2_IOCTL_DRIVER LIN_IOCTL_WR(FuncCode_Driver)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ I/O control Exclusive Access +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Exclusive access command codes.
typedef enum {
    IoctlExclAccessCmd_Acquire,
    IoctlExclAccessCmd_Probe,
    IoctlExclAccessCmd_Release
    // ADD NEW CMDs HERE TO MAINTAIN BACKWARDS COMPATIBILITY.
} IoctlExclAccessCmd;

#ifdef WINBUILD
    #define DTBB2_IOCTL_EXCL_ACCESS  WIN_IOCTL(FuncCode_Access)
#else
    #define DTBB2_IOCTL_EXCL_ACCESS LIN_IOCTL_WR(FuncCode_Access)
#endif

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ I/O control VPD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Identifies the kind of VPD I/O control.
typedef enum {
    IoctlVpd_TagDeleteItem=0, IoctlVpd_TagReadItem=1, IoctlVpd_TagReadRaw=2,
    IoctlVpd_TagWriteItem=3, IoctlVpd_TagWriteRaw=4
} IoctlVpdCmd;

// VPD section identifiers. Can be OR-ed together.
enum {Vpd_SectionBoot=1, Vpd_SectionRo=2, Vpd_SectionRw=4};

// VPD size constants.
#define Vpd_MaxItemLength           63           // Maximum VPD item length.
#define Vpd_MaxKeywordLength        15           // Maximum VPD keyword length.

#ifdef WINBUILD
    #define DTBB2_IOCTL_VPD WIN_IOCTL(FuncCode_Vpd)
#else
    #define DTBB2_IOCTL_VPD LIN_IOCTL_WR(FuncCode_Vpd)
#endif

// End of DtBb2 namespace
#ifdef __cplusplus
} 
#endif
