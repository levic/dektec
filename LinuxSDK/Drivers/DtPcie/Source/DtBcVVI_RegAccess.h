//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcVVI_RegAccess.h *#*#*#*#*#*#*#*#* (C) 2019 DekTec
//
// Device driver - VitalVersionInfo v0 (VVI) - Register access functions
//
// The register access functions defined here are elementary inline functions to read
// from and write to registers of BB2.0 building block VitalVersionInfo.
// These functions should solely be used by the block controller for VitalVersionInfo.
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
// 1. Getting fields 'Version' and 'Variant' from register 'FirmwareBuild':
//      UInt32  Reg_FirmwareBuild, Version, Variant;
//      Reg_FirmwareBuild = VVI_FirmwareBuild_READ(pBc);
//      Version           = VVI_FirmwareBuild_GET_Version(Reg_FirmwareBuild);
//      Variant           = VVI_FirmwareBuild_GET_Variant(Reg_FirmwareBuild);
//
// 2. Single-function shorthand for reading field 'Minute' from register
//    'FirmwareBuildDate':
//      UInt32  Minute = VVI_FirmwareBuildDate_READ_Minute(pBc);
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

// VVI_FirmwareBuildDate_GET_Day
// VVI_FirmwareBuildDate_GET_Hour
// VVI_FirmwareBuildDate_GET_Minute
// VVI_FirmwareBuildDate_GET_Month
// VVI_FirmwareBuildDate_GET_Year
// VVI_FirmwareBuildDate_READ
// VVI_FirmwareBuildDate_READ_Day
// VVI_FirmwareBuildDate_READ_Hour
// VVI_FirmwareBuildDate_READ_Minute
// VVI_FirmwareBuildDate_READ_Month
// VVI_FirmwareBuildDate_READ_Year
// VVI_FirmwareBuild_GET_Variant
// VVI_FirmwareBuild_GET_Version
// VVI_FirmwareBuild_READ
// VVI_FirmwareBuild_READ_Variant
// VVI_FirmwareBuild_READ_Version
// VVI_TypeNumber_READ

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Include Files +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#include "DtBc.h"                     // Block controller structs and functions

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Constant Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+ REGISTER-ACCESS FUNCTIONS - IMPLEMENTATION +=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'TypeNumber' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Field READ function
static __inline UInt32  VVI_TypeNumber_READ(DtBcVVI* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x4) & 0xFFFF;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'FirmwareBuild' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32  VVI_FirmwareBuild_READ(DtBcVVI* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8);
}

// Field GET functions
static __inline UInt32  VVI_FirmwareBuild_GET_Version(UInt32 FirmwareBuild)
{
    return FirmwareBuild & 0xFF;
}
static __inline UInt32  VVI_FirmwareBuild_GET_Variant(UInt32 FirmwareBuild)
{
    return (FirmwareBuild & 0xFF00) >> 8;
}

// Field READ functions
static __inline UInt32  VVI_FirmwareBuild_READ_Version(DtBcVVI* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8) & 0xFF;
}
static __inline UInt32  VVI_FirmwareBuild_READ_Variant(DtBcVVI* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x8) & 0xFF00) >> 8;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'FirmwareBuildDate' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32  VVI_FirmwareBuildDate_READ(DtBcVVI* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0xC);
}

// Field GET functions
static __inline UInt32  VVI_FirmwareBuildDate_GET_Year(UInt32 FirmwareBuildDate)
{
    return FirmwareBuildDate & 0xFFF;
}
static __inline UInt32  VVI_FirmwareBuildDate_GET_Month(UInt32 FirmwareBuildDate)
{
    return (FirmwareBuildDate & 0xF000) >> 12;
}
static __inline UInt32  VVI_FirmwareBuildDate_GET_Day(UInt32 FirmwareBuildDate)
{
    return (FirmwareBuildDate & 0x1F0000) >> 16;
}
static __inline UInt32  VVI_FirmwareBuildDate_GET_Hour(UInt32 FirmwareBuildDate)
{
    return (FirmwareBuildDate & 0x3E00000) >> 21;
}
static __inline UInt32  VVI_FirmwareBuildDate_GET_Minute(UInt32 FirmwareBuildDate)
{
    return (FirmwareBuildDate & 0xFC000000) >> 26;
}

// Field READ functions
static __inline UInt32  VVI_FirmwareBuildDate_READ_Year(DtBcVVI* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0xC) & 0xFFF;
}
static __inline UInt32  VVI_FirmwareBuildDate_READ_Month(DtBcVVI* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0xC) & 0xF000) >> 12;
}
static __inline UInt32  VVI_FirmwareBuildDate_READ_Day(DtBcVVI* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0xC) & 0x1F0000) >> 16;
}
static __inline UInt32  VVI_FirmwareBuildDate_READ_Hour(DtBcVVI* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0xC) & 0x3E00000) >> 21;
}
static __inline UInt32  VVI_FirmwareBuildDate_READ_Minute(DtBcVVI* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0xC) & 0xFC000000) >> 26;
}
