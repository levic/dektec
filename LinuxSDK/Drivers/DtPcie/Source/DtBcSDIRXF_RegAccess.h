//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDIRXF_RegAccess.h *#*#*#*#*#*#*#*# (C) 2019 DekTec
//
// Device driver - SdiRxFmtSimple v1 (SDIRXF) - Register access functions
//
// The register access functions defined here are elementary inline functions to read
// from and write to registers of BB2.0 building block SdiRxFmtSimple.
// These functions should solely be used by the block controller for SdiRxFmtSimple.
//
// 2019.11.06 10:03 - AUTOGENERATED BY DtBlock - DO NOT EDIT THIS FILE BY HAND

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
// 1. Getting fields 'MaxSdiRate' and 'Alignment' from register 'Config':
//      UInt32  Reg_Config, MaxSdiRate, Alignment;
//      Reg_Config = SDIRXF_Config_READ(pBc);
//      MaxSdiRate = SDIRXF_Config_GET_MaxSdiRate(Reg_Config);
//      Alignment  = SDIRXF_Config_GET_Alignment(Reg_Config);
//
// 2. Single-function shorthand for reading field 'FmtSyncLostLockReason' from register
//    'Debug':
//      UInt32  FmtSyncLostLockReason = SDIRXF_Debug_READ_FmtSyncLostLockReason(pBc);
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

// SDIRXF_Config_GET_Alignment
// SDIRXF_Config_GET_MaxSdiRate
// SDIRXF_Config_READ
// SDIRXF_Config_READ_Alignment
// SDIRXF_Config_READ_MaxSdiRate
// SDIRXF_Control_READ
// SDIRXF_Control_SET_BlockEnable
// SDIRXF_Control_SET_OperationalMode
// SDIRXF_Control_WRITE
// SDIRXF_Debug_GET_FmtChkLostLockReason
// SDIRXF_Debug_GET_FmtChkStatus
// SDIRXF_Debug_GET_FmtSyncLostLockReason
// SDIRXF_Debug_GET_LineLock
// SDIRXF_Debug_READ
// SDIRXF_Debug_READ_FmtChkLostLockReason
// SDIRXF_Debug_READ_FmtChkStatus
// SDIRXF_Debug_READ_FmtSyncLostLockReason
// SDIRXF_Debug_READ_LineLock
// SDIRXF_FmtEvIntTiming_READ
// SDIRXF_FmtEvIntTiming_SET_Delay
// SDIRXF_FmtEvIntTiming_SET_Interval
// SDIRXF_FmtEvIntTiming_SET_N
// SDIRXF_FmtEvIntTiming_WRITE
// SDIRXF_FmtEvStatus_GET_FmtSync
// SDIRXF_FmtEvStatus_GET_FrameId
// SDIRXF_FmtEvStatus_GET_SeqNum
// SDIRXF_FmtEvStatus_READ
// SDIRXF_FmtEvStatus_READ_FmtSync
// SDIRXF_FmtEvStatus_READ_FrameId
// SDIRXF_FmtEvStatus_READ_SeqNum
// SDIRXF_FrameProperties_READ
// SDIRXF_FrameProperties_SET_IsInterlaced
// SDIRXF_FrameProperties_SET_LinesPerFrame
// SDIRXF_FrameProperties_SET_SdiRate
// SDIRXF_FrameProperties_WRITE
// SDIRXF_LineProperties_READ
// SDIRXF_LineProperties_SET_NumSymH
// SDIRXF_LineProperties_SET_NumSymVV
// SDIRXF_LineProperties_WRITE

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Include Files +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#include "DtBc.h"                     // Block controller structs and functions

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Constant Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Value map 'BlockEnable'
#define SDIRXF_BLKENA_DISABLED            0x0  // Block is entirely disabled
#define SDIRXF_BLKENA_ENABLED             0x1  // Block is enabled and processes data

// Value map 'FmtChk'
#define SDIRXF_FMTCHK_FMT_ERR             0x0  // SDI format is not the expected format
#define SDIRXF_FMTCHK_FMT_OK              0x1  // SDI format is expected format

// Value map 'FmtChkLostLockReason'
#define SDIRXF_FMTCHKLLR_NO_FMTOK_YET     0x0  // Haven't reached FMT_OK yet
#define SDIRXF_FMTCHKLLR_NO_LINE_LOCK     0x1  // Line lock was lost
#define SDIRXF_FMTCHKLLR_SOF_NO_NUMLINES  0x2  // SOF detected but #lines in the frame !=
                                               // LinesPerFrame
#define SDIRXF_FMTCHKLLR_NUMLINES_NO_SOF  0x3  // #lines in the frame = LinesPerFrame but
                                               // no SOF detected

// Value map 'FmtSync'
#define SDIRXF_FMTSYNC_OUT_OF_SYNC        0x0  // Formatting process is out of sync.
#define SDIRXF_FMTSYNC_IN_SYNC            0x1  // Formatting process is fully in sync.

// Value map 'FmtSyncLostLockReason'
#define SDIRXF_FMTSYNCLLR_NO_SYNC_YET     0x0  // Haven't reached Formatting Sync yet
#define SDIRXF_FMTSYNCLLR_FMT_ERR         0x1  // Format-checking indicated FmtErr
#define SDIRXF_FMTSYNCLLR_SOF_TIMEOUT     0x2  // SOF time out
#define SDIRXF_FMTSYNCLLR_EARLY_SOF       0x3  // Early SOF

// Value map 'OperationalMode'
#define SDIRXF_OPMODE_IDLE                0x0  // Block is inactive
#define SDIRXF_OPMODE_STANDBY             0x1  // Block is partially operational
#define SDIRXF_OPMODE_RUN                 0x2  // Block performs its normal function

// Value map 'SdiRate'
#define SDIRXF_SDIMODE_SD                 0x0  // SDI mode is SD-SDI
#define SDIRXF_SDIMODE_HD                 0x1  // SDI mode is HD-SDI
#define SDIRXF_SDIMODE_3G                 0x2  // SDI mode is 3G-SDI
#define SDIRXF_SDIMODE_6G                 0x3  // SDI mode is 6G-SDI
#define SDIRXF_SDIMODE_12G                0x4  // SDI mode is 12G-SDI

// Value map 'StreamAlignment'
#define SDIRXF_STREAMALIGNMENT_32b        0x0  // Aligned on 32-bit boundaries
#define SDIRXF_STREAMALIGNMENT_64b        0x1  // Aligned on 64-bit boundaries
#define SDIRXF_STREAMALIGNMENT_128b       0x2  // Aligned on 128-bit boundaries
#define SDIRXF_STREAMALIGNMENT_256b       0x3  // Aligned on 256-bit boundaries
#define SDIRXF_STREAMALIGNMENT_512b       0x4  // Aligned on 512-bit boundaries

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+ REGISTER-ACCESS FUNCTIONS - IMPLEMENTATION +=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Config' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32  SDIRXF_Config_READ(DtBcSDIRXF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8);
}

// Field GET functions
static __inline UInt32  SDIRXF_Config_GET_MaxSdiRate(UInt32 Config)
{
    return Config & 0x7;
}
static __inline UInt32  SDIRXF_Config_GET_Alignment(UInt32 Config)
{
    return (Config & 0x70) >> 4;
}

// Field READ functions
static __inline UInt32  SDIRXF_Config_READ_MaxSdiRate(DtBcSDIRXF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8) & 0x7;
}
static __inline UInt32  SDIRXF_Config_READ_Alignment(DtBcSDIRXF* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x8) & 0x70) >> 4;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Control' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32  SDIRXF_Control_READ(DtBcSDIRXF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0xC);
}

// Field SET functions
static __inline UInt32  SDIRXF_Control_SET_BlockEnable(UInt32 Control, UInt32 BlockEnable)
{
    DT_ASSERT((BlockEnable & ~0x1) == 0);
    return (Control & ~0x1) | BlockEnable;
}
static __inline UInt32  SDIRXF_Control_SET_OperationalMode(UInt32 Control, UInt32 OperationalMode)
{
    DT_ASSERT((OperationalMode & ~0x3) == 0);
    return (Control & ~0x6) | (OperationalMode << 1);
}

// Register WRITE function
static __inline void  SDIRXF_Control_WRITE(DtBcSDIRXF* pBc, UInt32 Control)
{
    DtBc_RegWrite32((DtBc*)pBc, 0xC, Control);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'FmtEvStatus' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32  SDIRXF_FmtEvStatus_READ(DtBcSDIRXF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x10);
}

// Field GET functions
static __inline UInt32  SDIRXF_FmtEvStatus_GET_FrameId(UInt32 FmtEvStatus)
{
    return FmtEvStatus & 0xFFFF;
}
static __inline UInt32  SDIRXF_FmtEvStatus_GET_SeqNum(UInt32 FmtEvStatus)
{
    return (FmtEvStatus & 0xF0000) >> 16;
}
static __inline UInt32  SDIRXF_FmtEvStatus_GET_FmtSync(UInt32 FmtEvStatus)
{
    return (FmtEvStatus & 0x80000000) >> 31;
}

// Field READ functions
static __inline UInt32  SDIRXF_FmtEvStatus_READ_FrameId(DtBcSDIRXF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x10) & 0xFFFF;
}
static __inline UInt32  SDIRXF_FmtEvStatus_READ_SeqNum(DtBcSDIRXF* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x10) & 0xF0000) >> 16;
}
static __inline UInt32  SDIRXF_FmtEvStatus_READ_FmtSync(DtBcSDIRXF* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x10) & 0x80000000) >> 31;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'LineProperties' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32  SDIRXF_LineProperties_READ(DtBcSDIRXF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x14);
}

// Field SET functions
static __inline UInt32  SDIRXF_LineProperties_SET_NumSymH(UInt32 LineProperties, UInt32 NumSymH)
{
    DT_ASSERT((NumSymH & ~0xFFFF) == 0);
    return (LineProperties & ~0xFFFF) | NumSymH;
}
static __inline UInt32  SDIRXF_LineProperties_SET_NumSymVV(UInt32 LineProperties, UInt32 NumSymVV)
{
    DT_ASSERT((NumSymVV & ~0xFFFF) == 0);
    return (LineProperties & ~0xFFFF0000) | (NumSymVV << 16);
}

// Register WRITE function
static __inline void  SDIRXF_LineProperties_WRITE(DtBcSDIRXF* pBc, UInt32 LineProperties)
{
    DtBc_RegWrite32((DtBc*)pBc, 0x14, LineProperties);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'FrameProperties' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32  SDIRXF_FrameProperties_READ(DtBcSDIRXF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x18);
}

// Field SET functions
static __inline UInt32  SDIRXF_FrameProperties_SET_LinesPerFrame(UInt32 FrameProperties, UInt32 LinesPerFrame)
{
    DT_ASSERT((LinesPerFrame & ~0x3FFF) == 0);
    return (FrameProperties & ~0x3FFF) | LinesPerFrame;
}
static __inline UInt32  SDIRXF_FrameProperties_SET_SdiRate(UInt32 FrameProperties, UInt32 SdiRate)
{
    DT_ASSERT((SdiRate & ~0x7) == 0);
    return (FrameProperties & ~0x1C000) | (SdiRate << 14);
}
static __inline UInt32  SDIRXF_FrameProperties_SET_IsInterlaced(UInt32 FrameProperties, UInt32 IsInterlaced)
{
    DT_ASSERT((IsInterlaced & ~0x1) == 0);
    return (FrameProperties & ~0x20000) | (IsInterlaced << 17);
}

// Register WRITE function
static __inline void  SDIRXF_FrameProperties_WRITE(DtBcSDIRXF* pBc, UInt32 FrameProperties)
{
    DtBc_RegWrite32((DtBc*)pBc, 0x18, FrameProperties);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'FmtEvIntTiming' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32  SDIRXF_FmtEvIntTiming_READ(DtBcSDIRXF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x1C);
}

// Field SET functions
static __inline UInt32  SDIRXF_FmtEvIntTiming_SET_Interval(UInt32 FmtEvIntTiming, UInt32 Interval)
{
    DT_ASSERT((Interval & ~0xFFFF) == 0);
    return (FmtEvIntTiming & ~0xFFFF) | Interval;
}
static __inline UInt32  SDIRXF_FmtEvIntTiming_SET_Delay(UInt32 FmtEvIntTiming, UInt32 Delay)
{
    DT_ASSERT((Delay & ~0xFFF) == 0);
    return (FmtEvIntTiming & ~0xFFF0000) | (Delay << 16);
}
static __inline UInt32  SDIRXF_FmtEvIntTiming_SET_N(UInt32 FmtEvIntTiming, UInt32 N)
{
    DT_ASSERT((N & ~0xF) == 0);
    return (FmtEvIntTiming & ~0xF0000000) | (N << 28);
}

// Register WRITE function
static __inline void  SDIRXF_FmtEvIntTiming_WRITE(DtBcSDIRXF* pBc, UInt32 FmtEvIntTiming)
{
    DtBc_RegWrite32((DtBc*)pBc, 0x1C, FmtEvIntTiming);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Debug' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32  SDIRXF_Debug_READ(DtBcSDIRXF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x20);
}

// Field GET functions
static __inline UInt32  SDIRXF_Debug_GET_LineLock(UInt32 Debug)
{
    return Debug & 0x1;
}
static __inline UInt32  SDIRXF_Debug_GET_FmtChkStatus(UInt32 Debug)
{
    return (Debug & 0x2) >> 1;
}
static __inline UInt32  SDIRXF_Debug_GET_FmtChkLostLockReason(UInt32 Debug)
{
    return (Debug & 0xC) >> 2;
}
static __inline UInt32  SDIRXF_Debug_GET_FmtSyncLostLockReason(UInt32 Debug)
{
    return (Debug & 0x30) >> 4;
}

// Field READ functions
static __inline UInt32  SDIRXF_Debug_READ_LineLock(DtBcSDIRXF* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x20) & 0x1;
}
static __inline UInt32  SDIRXF_Debug_READ_FmtChkStatus(DtBcSDIRXF* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x20) & 0x2) >> 1;
}
static __inline UInt32  SDIRXF_Debug_READ_FmtChkLostLockReason(DtBcSDIRXF* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x20) & 0xC) >> 2;
}
static __inline UInt32  SDIRXF_Debug_READ_FmtSyncLostLockReason(DtBcSDIRXF* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x20) & 0x30) >> 4;
}

