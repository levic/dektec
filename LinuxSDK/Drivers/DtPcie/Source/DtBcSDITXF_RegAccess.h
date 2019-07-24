//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDITXF_RegAccess.h *#*#*#*#*#*#*#*# (C) 2019 DekTec
//
// Device driver - SdiTxFmtSimple v0 (SDITXF) - Register access functions
//
// The register access functions defined here are elementary inline functions to read
// from and write to registers of BB2.0 building block SdiTxFmtSimple.
// These functions should solely be used by the block controller for SdiTxFmtSimple.
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
// 1. Getting fields 'FrameId' and 'SeqNum' from register 'FmtEvStatus':
//      UInt32  Reg_FmtEvStatus, FrameId, SeqNum;
//      Reg_FmtEvStatus = SDITXF_FmtEvStatus_READ(pBc);
//      FrameId         = SDITXF_FmtEvStatus_GET_FrameId(Reg_FmtEvStatus);
//      SeqNum          = SDITXF_FmtEvStatus_GET_SeqNum(Reg_FmtEvStatus);
//
// 2. Single-function shorthand for reading field 'Underflow' from register 'FmtEvStatus':
//      UInt32  Underflow = SDITXF_FmtEvStatus_READ_Underflow(pBc);
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

// SDITXF_Control_READ
// SDITXF_Control_SET_BlockEnable
// SDITXF_Control_SET_OperationalMode
// SDITXF_Control_SET_UflEnable
// SDITXF_Control_WRITE
// SDITXF_FmtEvStatus_GET_FrameId
// SDITXF_FmtEvStatus_GET_SeqNum
// SDITXF_FmtEvStatus_GET_Underflow
// SDITXF_FmtEvStatus_READ
// SDITXF_FmtEvStatus_READ_FrameId
// SDITXF_FmtEvStatus_READ_SeqNum
// SDITXF_FmtEvStatus_READ_Underflow
// SDITXF_FmtEvent_READ
// SDITXF_FmtEvent_SET_NumLines
// SDITXF_FmtEvent_WRITE

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Include Files +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#include "DtBc.h"                     // Block controller structs and functions

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Constant Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Value map 'BlockEnable'
#define SDITXF_BLKENA_DISABLED  0x0  // Block is entirely disabled
#define SDITXF_BLKENA_ENABLED   0x1  // Block is enabled and processes data

// Value map 'OperationalMode'
#define SDITXF_OPMODE_IDLE      0x0  // Block is inactive
#define SDITXF_OPMODE_STANDBY   0x1  // Block is partially operational
#define SDITXF_OPMODE_RUN       0x2  // Block performs its normal function

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+ REGISTER-ACCESS FUNCTIONS - IMPLEMENTATION +=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Control' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32  SDITXF_Control_READ(DtBcSDITXF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8);
}

// Field SET functions
static __inline UInt32  SDITXF_Control_SET_BlockEnable(UInt32 Control, UInt32 BlockEnable)
{
    DT_ASSERT((BlockEnable & ~0x1) == 0);
    return (Control & ~0x1) | BlockEnable;
}
static __inline UInt32  SDITXF_Control_SET_OperationalMode(UInt32 Control, UInt32 OperationalMode)
{
    DT_ASSERT((OperationalMode & ~0x3) == 0);
    return (Control & ~0x6) | (OperationalMode << 1);
}
static __inline UInt32  SDITXF_Control_SET_UflEnable(UInt32 Control, UInt32 UflEnable)
{
    DT_ASSERT((UflEnable & ~0x1) == 0);
    return (Control & ~0x8) | (UflEnable << 3);
}

// Register WRITE function
static __inline void  SDITXF_Control_WRITE(DtBcSDITXF* pBc, UInt32 Control)
{
    DtBc_RegWrite32((DtBc*)pBc, 0x8, Control);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'FmtEvStatus' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32  SDITXF_FmtEvStatus_READ(DtBcSDITXF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0xC);
}

// Field GET functions
static __inline UInt32  SDITXF_FmtEvStatus_GET_FrameId(UInt32 FmtEvStatus)
{
    return FmtEvStatus & 0xFFFF;
}
static __inline UInt32  SDITXF_FmtEvStatus_GET_SeqNum(UInt32 FmtEvStatus)
{
    return (FmtEvStatus & 0xF0000) >> 16;
}
static __inline UInt32  SDITXF_FmtEvStatus_GET_Underflow(UInt32 FmtEvStatus)
{
    return (FmtEvStatus & 0x100000) >> 20;
}

// Field READ functions
static __inline UInt32  SDITXF_FmtEvStatus_READ_FrameId(DtBcSDITXF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0xC) & 0xFFFF;
}
static __inline UInt32  SDITXF_FmtEvStatus_READ_SeqNum(DtBcSDITXF* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0xC) & 0xF0000) >> 16;
}
static __inline UInt32  SDITXF_FmtEvStatus_READ_Underflow(DtBcSDITXF* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0xC) & 0x100000) >> 20;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'FmtEvent' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32  SDITXF_FmtEvent_READ(DtBcSDITXF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x10);
}

// Field SET function
static __inline UInt32  SDITXF_FmtEvent_SET_NumLines(UInt32 FmtEvent, UInt32 NumLines)
{
    DT_ASSERT((NumLines & ~0x3FF) == 0);
    return (FmtEvent & ~0x3FF) | NumLines;
}

// Register WRITE function
static __inline void  SDITXF_FmtEvent_WRITE(DtBcSDITXF* pBc, UInt32 FmtEvent)
{
    DtBc_RegWrite32((DtBc*)pBc, 0x10, FmtEvent);
}

