//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPcieCommon.h *#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
//
// DtPcie driver - Common file shared between DtPcie driver and DTAPI
//
// This file describes the DtPcie driver interface, which is used by the DTAPI.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2017 DekTec Digital Video B.V.
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

#ifndef __DTPCIE_COMMON_H
#define __DTPCIE_COMMON_H

// Define Int64/UInt64 here to a non-existing identifier. Since all types in this header
// are used in a binary interface we have to make sure they are compatible between 32-bit 
// and 64-bit builds. For this we have to align every 64-bit value to an 8-byte boundary.
// The Int64A / UInt64A types do this. Make sure Int64/UInt64 are not used.
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
// {18AB1E49-C874-4619-B8AC-62232B4EC41B}
static const GUID  GUID_DEVINTERFACE_DTPCIE = 
{ 0x18ab1e49, 0xc874, 0x4619, { 0xb8, 0xac, 0x62, 0x23, 0x2b, 0x4e, 0xc4, 0x1b } };

#else // WINBUILD

#ifdef DTAPI
    #include <sys/ioctl.h>
#endif

#endif

#pragma pack (push, 8)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtPcie IOCTL codes +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
//
// On Windows:
// METHOD_OUT_DIRECT is used for 'normal' IOCTLS, METHOD_IN_DIRECT is not needed for
//                   'normal' IOCTLS because output buffers are never used as input
// METHOD_IN_DIRECT and METHOD_OUT_DIRECT are used for DMA's
// METHOD_IN_DIRECT is used for shared buffers. The output buffer can then be used as 
//                  input and output
// METHOD_NEITHER   is not used at all
//

// Constants
#ifdef WINBUILD
    // Windows specific defines
    #define DTPCIE_DEVICE_TYPE  45101  // DeviceType for DeviceIoControl operations
    #define DTPCIE_IOCTL_BASE   3300   // Offset for Function
#else
    // Linux specific defines
    #define DTPCIE_IOCTL_MAGIC         0xBC   // Magic number
    #define DTPCIE_IOCTL_MAGIC_SIZE    0xBD   // Magic number for Ioctls with a dynamic size
    #define DTPCIE_IOCTL_BASE          0x50   // Offset for Function
#endif

// DTPCIE functions
// DON'T INSERT NEW VALUES BETWEEN EXISTING ONES, SO THAT NEW DRIVER VERSIONS REMAIN 
// COMPATIBLE WITH USER APPLICATIONS THAT WERE COMPILED AGAINST AN OLDER DRIVER!!
//enum {
//    // = DTPCIE_IOCTL_BASE,
//};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieIoctlInputData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef union _DtPcieIoctlInputData
{
    // Add common command data members first
    DT_IOCTL_INPUT_DATA_COMMON_DATA;
} DtPcieIoctlInputData;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieIoctlOutputData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef union _DtPcieIoctlOutputData 
{
    // Add common command data members first
    DT_IOCTL_OUTPUT_DATA_COMMON_DATA;
} DtPcieIoctlOutputData;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPcieIoctlData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef union _DtPcieIoctlData 
{
    DtPcieIoctlInputData  m_Input;
    DtPcieIoctlOutputData  m_Output;
} DtPcieIoctlData;


// Status flag definitions
#define DTPCIE_TX_FIFO_UFL      2
#define DTPCIE_TX_SYNC_ERR      4
#define DTPCIE_TX_READBACK_ERR  8
#define DTPCIE_TX_TARGET_ERR    16
#define DTPCIE_TX_FIFO_OVF      32

// Status flag definitions
#define DTPCIE_RX_FIFO_OVF         2
#define DTPCIE_RX_SYNC_ERR         4
#define DTPCIE_RX_RATE_OVF         8
#define DTPCIE_RX_TARGET_ERR       16

#define DTPCIE_AR_UNKNOWN                0  // Unknown aspect ratio
#define DTPCIE_AR_4_3                    1  // 4x3
#define DTPCIE_AR_16_9                   2  // 16x9
#define DTPCIE_AR_14_9                   3  // 14x9

#pragma pack (pop)

#undef Int64
#undef UInt64

#endif // __DTPCIE_COMMON_H
