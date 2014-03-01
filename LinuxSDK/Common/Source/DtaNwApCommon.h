//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaNwApCommon.h *#*#*#*#*#*#*#*#*#*# (C) 2012 DekTec
//
// DtaNwAp driver - Common file shared between DtaNwAp driver and DTAPI
//
// This file describes the DtaNwAp driver interface, which is used by the DTAPI.

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


#ifndef __DTANWAP_COMMON_H
#define __DTANWAP_COMMON_H

#define DTANWAP_LINKNAME_STRING     L"\\DosDevices\\DtaNwAp"
#define DTANWAP_NTDEVICE_STRING     L"\\Device\\DtaNwAp"


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
// {125582CF-DB33-42F3-B6C1-2CD1D90C830B}
//static const GUID GUID_DEVINTERFACE_DTANWAP = 
//{ 0x125582cf, 0xdb33, 0x42f3, { 0xb6, 0xc1, 0x2c, 0xd1, 0xd9, 0xc, 0x83, 0xb } };

#else // WINBUILD

#ifdef DTAPI
    #include <sys/ioctl.h>
#endif

#endif

#pragma pack (push, 8)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IOCTL codes +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Constants
#ifdef WINBUILD
    // Windows specific defines
    #define DTANWAP_DEVICE_TYPE  45100  // DeviceType for DeviceIoControl operations
    #define DTANWAP_IOCTL_BASE   3100   // Offset for Function
#else
    // Linux specific defines
    // This driver is not used for Linux
#endif

// DTA functions
// DON'T INSERT NEW VALUES BETWEEN EXISTING ONES, SO THAT NEW DRIVER VERSIONS REMAIN
// COMPATIBLE WITH USER APPLICATIONS THAT WERE COMPILED AGAINST AN OLD DRIVER!!
enum {
    FUNC_DTANWAP_GET_GUIDVLAN = DTANWAP_IOCTL_BASE,
    FUNC_DTANWAP_GET_DRIVER_VERSION,
    FUNC_DTANWAP_RECONFIGURE,
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTANWAP_IOCTL_GET_GUIDVLAN -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Ioctl input data type
typedef struct _DtaNwApIoctlGetGuidVlanInput {
    UInt8  m_MacAddress[6];                 // MAC address main network device
    UInt8  m_Reserved[2];                   // Alignment
    Int  m_VlanId;                          // Vlan ID
} DtaNwApIoctlGetGuidVlanInput;
//ASSERT_SIZE(DtaNwApIoctlGetGuidVlanInput, 12)

// Ioctl output data type
typedef struct _DtaNwApIoctlGetGuidVlanOutput {
    GUID  m_Guid;                           // Guid of Vlan
} DtaNwApIoctlGetGuidVlanOutput;
//ASSERT_SIZE(DtaNwApIoctlGetGuidVlanOutput, 16)

#define DTANWAP_IOCTL_GET_GUIDVLAN  CTL_CODE(DTANWAP_DEVICE_TYPE,                        \
                                                          FUNC_DTANWAP_GET_GUIDVLAN,     \
                                                          METHOD_BUFFERED, FILE_READ_DATA)

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DTANWAP_IOCTL_GET_DRIVER_VERSION -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtaNwApIoctlGetDriverVersionOutput {
    Int  m_Major;
    Int  m_Minor;
    Int  m_Micro;
    Int  m_Build;
} DtaNwApIoctlGetDriverVersionOutput;
//ASSERT_SIZE(DtaNwApIoctlGetDriverVersionOutput, 16)

#define DTANWAP_IOCTL_GET_DRIVER_VERSION  CTL_CODE(DTANWAP_DEVICE_TYPE, \
                         FUNC_DTANWAP_GET_DRIVER_VERSION, METHOD_BUFFERED, FILE_READ_DATA)

    // Ioctl output data type
typedef struct _DtaNwApIoctlReconfigureInput {
    GUID  m_AdapterGuid;                           // Guid of Adapter
} DtaNwApIoctlReconfigureInput;
//ASSERT_SIZE(DtaNwApIoctlReconfigureInput, 16)

#define DTANWAP_IOCTL_RECONFIGURE  CTL_CODE(DTANWAP_DEVICE_TYPE,                        \
                                                          FUNC_DTANWAP_RECONFIGURE,     \
                                                          METHOD_BUFFERED, FILE_READ_DATA)


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwApIoctlInputData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef union _DtaNwApIoctlInputData {
    DtaNwApIoctlGetGuidVlanInput  m_GetGuidVlan;
    DtaNwApIoctlReconfigureInput  m_Reconfigure;
} DtaNwApIoctlInputData;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwApIoctlOutputData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef union _DtaNwApIoctlOutputData {
    DtaNwApIoctlGetGuidVlanOutput  m_GetGuidVlan;
    DtaNwApIoctlGetDriverVersionOutput  m_GetDriverVersion;
} DtaNwApIoctlOutputData;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNwApIoctlData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef union _DtaNwApIoctlData {
    DtaNwApIoctlInputData  m_Input;
    DtaNwApIoctlOutputData  m_Output;
} DtaNwApIoctlData;

#pragma pack (pop)

#undef Int64
#undef UInt64

#endif // __DTANWAP_COMMON_H
