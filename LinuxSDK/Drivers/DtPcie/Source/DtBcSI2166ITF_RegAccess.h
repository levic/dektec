//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSI2166ITF_RegAccess.h *#*#*#*#*#*#*# (C) 2020 DekTec
//
// Device driver - Si2166Itf v0 (SI2166ITF) - Register access functions
//
// The register access functions defined here are elementary inline functions to read
// from and write to registers of BB2.0 building block Si2166Itf.
// These functions should solely be used by the block controller for Si2166Itf.
//
// 2020.09.09 9:49 - AUTOGENERATED BY DtBlock - DO NOT EDIT THIS FILE BY HAND

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Copyright (C) 2020 DekTec Digital Video B.V.
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
// 1. Getting fields 'Overflow' and 'Load' from register 'BufferFifoStatus':
//      UInt32  Reg_BufferFifoStatus, Overflow, Load;
//      Reg_BufferFifoStatus = SI2166ITF_BufferFifoStatus_READ(pBc);
//      Overflow             =
// SI2166ITF_BufferFifoStatus_GET_Overflow(Reg_BufferFifoStatus);
//      Load                 = SI2166ITF_BufferFifoStatus_GET_Load(Reg_BufferFifoStatus);
//
// 2. Single-function shorthand for reading field 'Load' from register 'BufferFifoStatus':
//      UInt32  Load = SI2166ITF_BufferFifoStatus_READ_Load(pBc);
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

// SI2166ITF_BufferFifoStatus_CLEAR_Overflow
// SI2166ITF_BufferFifoStatus_GET_Load
// SI2166ITF_BufferFifoStatus_GET_Overflow
// SI2166ITF_BufferFifoStatus_READ
// SI2166ITF_BufferFifoStatus_READ_Load
// SI2166ITF_BufferFifoStatus_READ_Overflow
// SI2166ITF_Control_READ
// SI2166ITF_Control_SET_BlockEnable
// SI2166ITF_Control_SET_DataType
// SI2166ITF_Control_SET_OperationalMode
// SI2166ITF_Control_SET_Si2166Mode
// SI2166ITF_Control_WRITE

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Include Files +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#include "DtBc.h"                     // Block controller structs and functions

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Constant Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Value map 'BlockEnable'
#define SI2166ITF_BLKENA_DISABLED  0x0  // Block is entirely disabled
#define SI2166ITF_BLKENA_ENABLED   0x1  // Block is enabled and processes data

// Value map 'OperationalMode'
#define SI2166ITF_OPMODE_IDLE      0x0  // Block is inactive
#define SI2166ITF_OPMODE_STANDBY   0x1  // Block is partially operational
#define SI2166ITF_OPMODE_RUN       0x2  // Block performs its normal function

// Value map 'Si2166Mode'
#define SI2166ITF_SI2166MODE_TS    0x0  // Transport Stream
#define SI2166ITF_SI2166MODE_GSE   0x1  // GSE-HEM/GP/GC

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+ REGISTER-ACCESS FUNCTIONS - IMPLEMENTATION +=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Control' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32 SI2166ITF_Control_READ(DtBcSI2166ITF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x4);
}

// Field SET functions
static __inline UInt32 SI2166ITF_Control_SET_BlockEnable(UInt32 Control, UInt32 BlockEnable)
{
    DT_ASSERT((BlockEnable & ~0x1) == 0);
    return (Control & ~0x1) | BlockEnable;
}
static __inline UInt32 SI2166ITF_Control_SET_OperationalMode(UInt32 Control, UInt32 OperationalMode)
{
    DT_ASSERT((OperationalMode & ~0x3) == 0);
    return (Control & ~0x6) | (OperationalMode << 1);
}
static __inline UInt32 SI2166ITF_Control_SET_Si2166Mode(UInt32 Control, UInt32 Si2166Mode)
{
    DT_ASSERT((Si2166Mode & ~0x1) == 0);
    return (Control & ~0x8) | (Si2166Mode << 3);
}
static __inline UInt32 SI2166ITF_Control_SET_DataType(UInt32 Control, UInt32 DataType)
{
    DT_ASSERT((DataType & ~0xFF) == 0);
    return (Control & ~0xFF00) | (DataType << 8);
}

// Register WRITE function
static __inline void SI2166ITF_Control_WRITE(DtBcSI2166ITF* pBc, UInt32 Control)
{
    DtBc_RegWrite32((DtBc*)pBc, 0x4, Control);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'BufferFifoStatus' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32 SI2166ITF_BufferFifoStatus_READ(DtBcSI2166ITF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8);
}

// Field GET functions
static __inline UInt32 SI2166ITF_BufferFifoStatus_GET_Overflow(UInt32 BufferFifoStatus)
{
    return BufferFifoStatus & 0x1;
}
static __inline UInt32 SI2166ITF_BufferFifoStatus_GET_Load(UInt32 BufferFifoStatus)
{
    return (BufferFifoStatus & 0x7FF00) >> 8;
}

// Field READ functions
static __inline UInt32 SI2166ITF_BufferFifoStatus_READ_Overflow(DtBcSI2166ITF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8) & 0x1;
}
static __inline UInt32 SI2166ITF_BufferFifoStatus_READ_Load(DtBcSI2166ITF* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x8) & 0x7FF00) >> 8;
}

// Field CLEAR function
static __inline void SI2166ITF_BufferFifoStatus_CLEAR_Overflow(DtBcSI2166ITF* pBc)
{
    UInt32  BufferFifoStatus = DtBc_RegRead32((DtBc*)pBc, 0x8);
    DtBc_RegWrite32((DtBc*)pBc, 0x8, BufferFifoStatus | 0x1);
}

