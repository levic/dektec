//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcLMH1981_RegAccess.h *#*#*#*#*#*#*#* (C) 2019 DekTec
//
// Device driver - Lmh1981Ctrl v0 (LMH1981) - Register access functions
//
// The register access functions defined here are elementary inline functions to read
// from and write to registers of BB2.0 building block Lmh1981Ctrl.
// These functions should solely be used by the block controller for Lmh1981Ctrl.
//
// 2019.03.06 11:21 - AUTOGENERATED BY DtBlock - DO NOT EDIT THIS FILE BY HAND

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Copyright (C) 2019 DekTec Digital Video B.V.
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

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ EXAMPLES +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// 1. Getting fields 'HsRate' and 'VsRate' from register 'SyncRate':
//      UInt32  Reg_SyncRate, HsRate, VsRate;
//      Reg_SyncRate = LMH1981_SyncRate_READ(pBc);
//      HsRate       = LMH1981_SyncRate_GET_HsRate(Reg_SyncRate);
//      VsRate       = LMH1981_SyncRate_GET_VsRate(Reg_SyncRate);
//
// 2. Single-function shorthand for reading field 'Field2' from register 'NumLines':
//      UInt32  Field2 = LMH1981_NumLines_READ_Field2(pBc);
//
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ NOTES +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// A) No automatic caching is provided, the user has to manage a 'cache' of 32-bit
// register values himself. The number of register reads and writes shall be minimized,
// as these are extremely slow (hundreds of CPU cycles for PCIe bus and orders of
// magnitude longer for USB).
//
// B) To read a field, the register shall first be read into an UInt32 using a READ
// function, followed by getting the field from the UInt32 value with a GET function.
// Field READ functions (a combination of the register READ- and field GET function) are
// provided for convenience, but these should only be used if the register value is just
// needed once for getting a single field.
//
// C) No register-access functions are generated for the BlockId register at address 0,
// nor for interrupt-status registers. The driver has generic (BB-independent) functions
// available for accessing these registers.

//=+=+=+=+=+=+=+=+=+=+=+ List of Generated Register-Access Functions +=+=+=+=+=+=+=+=+=+=+

// LMH1981_NumLines_GET_Field1
// LMH1981_NumLines_GET_Field2
// LMH1981_NumLines_READ
// LMH1981_NumLines_READ_Field1
// LMH1981_NumLines_READ_Field2
// LMH1981_SofTimestampNanosec_READ
// LMH1981_SofTimestampSec_READ
// LMH1981_SyncRate_GET_HsRate
// LMH1981_SyncRate_GET_VsRate
// LMH1981_SyncRate_READ
// LMH1981_SyncRate_READ_HsRate
// LMH1981_SyncRate_READ_VsRate
// LMH1981_VideoFormat_READ

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Include Files +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#include "DtBc.h"                     // Block controller structs and functions

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Constant Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Value map 'VideoFormat'
#define LMH1981_FORMAT_NtscF1   0x104  // NTSC Format Field 1
#define LMH1981_FORMAT_NtscF2   0x103  // NTSC Format Field 2
#define LMH1981_FORMAT_PalF1    0x136  // PAL Format Field 1
#define LMH1981_FORMAT_PalF2    0x135  // PAL Format Field 2
#define LMH1981_FORMAT_480p     0x20A  // 480p Format
#define LMH1981_FORMAT_576p     0x26E  // 576p Format
#define LMH1981_FORMAT_720p     0x2EB  // 720p Format
#define LMH1981_FORMAT_1080iF1  0x230  // 1080i Format Field 1
#define LMH1981_FORMAT_1080iF2  0x22F  // 1080i Format Field 2
#define LMH1981_FORMAT_1080p    0x462  // 1080p Format
#define LMH1981_FORMAT_Unknown  0x0    // Unknown Input

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+ REGISTER-ACCESS FUNCTIONS - IMPLEMENTATION +=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'SyncRate' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32  LMH1981_SyncRate_READ(DtBcLMH1981* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8);
}

// Field GET functions
static __inline UInt32  LMH1981_SyncRate_GET_HsRate(UInt32 SyncRate)
{
    return SyncRate & 0xFFFF;
}
static __inline UInt32  LMH1981_SyncRate_GET_VsRate(UInt32 SyncRate)
{
    return (SyncRate & 0xFFFF0000) >> 16;
}

// Field READ functions
static __inline UInt32  LMH1981_SyncRate_READ_HsRate(DtBcLMH1981* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8) & 0xFFFF;
}
static __inline UInt32  LMH1981_SyncRate_READ_VsRate(DtBcLMH1981* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x8) & 0xFFFF0000) >> 16;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'NumLines' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32  LMH1981_NumLines_READ(DtBcLMH1981* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0xC);
}

// Field GET functions
static __inline UInt32  LMH1981_NumLines_GET_Field1(UInt32 NumLines)
{
    return NumLines & 0xFFFF;
}
static __inline UInt32  LMH1981_NumLines_GET_Field2(UInt32 NumLines)
{
    return (NumLines & 0xFFFF0000) >> 16;
}

// Field READ functions
static __inline UInt32  LMH1981_NumLines_READ_Field1(DtBcLMH1981* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0xC) & 0xFFFF;
}
static __inline UInt32  LMH1981_NumLines_READ_Field2(DtBcLMH1981* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0xC) & 0xFFFF0000) >> 16;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'VideoFormat' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Field READ function
static __inline UInt32  LMH1981_VideoFormat_READ(DtBcLMH1981* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x10) & 0x7FF;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'SofTimestampSec' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Field READ function
static __inline UInt32  LMH1981_SofTimestampSec_READ(DtBcLMH1981* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x14);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'SofTimestampNanosec' -.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Field READ function
static __inline UInt32  LMH1981_SofTimestampNanosec_READ(DtBcLMH1981* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x18) & 0x3FFFFFFF;
}

