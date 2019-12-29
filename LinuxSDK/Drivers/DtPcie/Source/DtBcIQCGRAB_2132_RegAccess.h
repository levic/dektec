//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQCGRAB_RegAccess.h *#*#*#*#*#*#*#* (C) 2019 DekTec
//
// Device driver - IqConstGrabber v-1 (IQCGRAB) - Register access functions
//
// The register access functions defined here are elementary inline functions to read
// from and write to registers of BB2.0 building block IqConstGrabber.
// These functions should solely be used by the block controller for IqConstGrabber.
//
// 2019.03.26 8:50 - AUTOGENERATED BY DtBlock - DO NOT EDIT THIS FILE BY HAND

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
// 1. Getting fields 'I' and 'Q' from register 'ConstPointFifo':
//      UInt32  Reg_ConstPointFifo, I, Q;
//      Reg_ConstPointFifo = IQCGRAB_ConstPointFifo_READ(pBc);
//      I                  = IQCGRAB_ConstPointFifo_GET_I(Reg_ConstPointFifo);
//      Q                  = IQCGRAB_ConstPointFifo_GET_Q(Reg_ConstPointFifo);
//
// 2. Single-function shorthand for reading field 'FifoLoad' from register
//    'CaptureStatus':
//      UInt32  FifoLoad = IQCGRAB_CaptureStatus_READ_FifoLoad(pBc);
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

// IQCGRAB_CaptureControl_READ
// IQCGRAB_CaptureControl_SET_Start
// IQCGRAB_CaptureControl_WRITE
// IQCGRAB_CaptureSettings_READ
// IQCGRAB_CaptureSettings_SET_NumSamples
// IQCGRAB_CaptureSettings_SET_PLS
// IQCGRAB_CaptureSettings_WRITE
// IQCGRAB_CaptureStatus_GET_FifoLoad
// IQCGRAB_CaptureStatus_READ
// IQCGRAB_CaptureStatus_READ_FifoLoad
// IQCGRAB_Configuration_GET_FifoSize
// IQCGRAB_Configuration_READ
// IQCGRAB_Configuration_READ_FifoSize
// IQCGRAB_ConstPointFifo_GET_I
// IQCGRAB_ConstPointFifo_GET_Q
// IQCGRAB_ConstPointFifo_READ

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Include Files +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#include "DtBc.h"                     // Block controller structs and functions

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Constant Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+ REGISTER-ACCESS FUNCTIONS - IMPLEMENTATION +=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Configuration' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32  IQCGRAB_Configuration_READ(DtBcIQCGRAB_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8);
}

// Field GET function
static __inline UInt32  IQCGRAB_Configuration_GET_FifoSize(UInt32 Configuration)
{
    return Configuration & 0xFFFF;
}

// Field READ function
static __inline UInt32  IQCGRAB_Configuration_READ_FifoSize(DtBcIQCGRAB_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8) & 0xFFFF;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'CaptureControl' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32  IQCGRAB_CaptureControl_READ(DtBcIQCGRAB_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0xC);
}

// Field SET function
static __inline UInt32  IQCGRAB_CaptureControl_SET_Start(UInt32 CaptureControl, UInt32 Start)
{
    DT_ASSERT((Start & ~0x1) == 0);
    return (CaptureControl & ~0x1) | Start;
}

// Register WRITE function
static __inline void  IQCGRAB_CaptureControl_WRITE(DtBcIQCGRAB_2132* pBc, UInt32 CaptureControl)
{
    DtBc_RegWrite32((DtBc*)pBc, 0xC, CaptureControl);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'CaptureSettings' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32  IQCGRAB_CaptureSettings_READ(DtBcIQCGRAB_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x10);
}

// Field SET functions
static __inline UInt32  IQCGRAB_CaptureSettings_SET_PLS(UInt32 CaptureSettings, UInt32 PLS)
{
    DT_ASSERT((PLS & ~0xFF) == 0);
    return (CaptureSettings & ~0xFF) | PLS;
}
static __inline UInt32  IQCGRAB_CaptureSettings_SET_NumSamples(UInt32 CaptureSettings, UInt32 NumSamples)
{
    DT_ASSERT((NumSamples & ~0xFFFF) == 0);
    return (CaptureSettings & ~0xFFFF0000) | (NumSamples << 16);
}

// Register WRITE function
static __inline void  IQCGRAB_CaptureSettings_WRITE(DtBcIQCGRAB_2132* pBc, UInt32 CaptureSettings)
{
    DtBc_RegWrite32((DtBc*)pBc, 0x10, CaptureSettings);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'CaptureStatus' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32  IQCGRAB_CaptureStatus_READ(DtBcIQCGRAB_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x14);
}

// Field GET function
static __inline UInt32  IQCGRAB_CaptureStatus_GET_FifoLoad(UInt32 CaptureStatus)
{
    return CaptureStatus & 0xFFFF;
}

// Field READ function
static __inline UInt32  IQCGRAB_CaptureStatus_READ_FifoLoad(DtBcIQCGRAB_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x14) & 0xFFFF;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'ConstPointFifo' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32  IQCGRAB_ConstPointFifo_READ(DtBcIQCGRAB_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x18);
}

// Field GET functions
static __inline UInt32  IQCGRAB_ConstPointFifo_GET_I(UInt32 ConstPointFifo)
{
    return ConstPointFifo & 0xFFF;
}
static __inline UInt32  IQCGRAB_ConstPointFifo_GET_Q(UInt32 ConstPointFifo)
{
    return (ConstPointFifo & 0xFFF0000) >> 16;
}
