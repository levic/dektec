//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcS2STATS_2132_RegAccess.h *#*#*#*#*#*#*#* (C) 2019 DekTec
//
// Device driver - DvbS2Statistics v-1 (S2STATS) - Register access functions
//
// The register access functions defined here are elementary inline functions to read
// from and write to registers of BB2.0 building block DvbS2Statistics.
// These functions should solely be used by the block controller for DvbS2Statistics.
//
// 2019.03.28 13:58 - AUTOGENERATED BY DtBlock - DO NOT EDIT THIS FILE BY HAND

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
// 1. Getting fields 'PLS' and 'InputStreamId' from register 'FrameStatus':
//      UInt32  Reg_FrameStatus, PLS, InputStreamId;
//      Reg_FrameStatus = S2STATS_FrameStatus_READ(pBc);
//      PLS             = S2STATS_FrameStatus_GET_PLS(Reg_FrameStatus);
//      InputStreamId   = S2STATS_FrameStatus_GET_InputStreamId(Reg_FrameStatus);
//
// 2. Single-function shorthand for reading field 'FifoLoad' from register 'Status':
//      UInt32  FifoLoad = S2STATS_Status_READ_FifoLoad(pBc);
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

// S2STATS_Configuration_GET_FifoSize
// S2STATS_Configuration_READ
// S2STATS_Configuration_READ_FifoSize
// S2STATS_Control_READ
// S2STATS_Control_SET_ClearFifo
// S2STATS_Control_WRITE
// S2STATS_FrameStatus_GET_CcmAcm
// S2STATS_FrameStatus_GET_InputStreamId
// S2STATS_FrameStatus_GET_IsBchOk
// S2STATS_FrameStatus_GET_IsLdpcOk
// S2STATS_FrameStatus_GET_MER
// S2STATS_FrameStatus_GET_PLS
// S2STATS_FrameStatus_READ
// S2STATS_Status_GET_FifoLoad
// S2STATS_Status_READ
// S2STATS_Status_READ_FifoLoad

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Include Files +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#include "DtBc.h"                     // Block controller structs and functions

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Constant Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Value map 'IsOk'
#define S2STATS_CMD_NOT_OK  0x0  // LDPC decoding is not ok
#define S2STATS_CMD_OK      0x1  // LDPC decoding is ok

// Value map 'AcmCcm'
#define S2STATS_CMD_ACM     0x0  // Adaptive Coding and Modulation
#define S2STATS_CMD_CCM     0x1  // Constant Coding and Modulation

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+ REGISTER-ACCESS FUNCTIONS - IMPLEMENTATION +=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Configuration' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32  S2STATS_Configuration_READ(DtBcS2STATS_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8);
}

// Field GET function
static __inline UInt32  S2STATS_Configuration_GET_FifoSize(UInt32 Configuration)
{
    return Configuration & 0xFFFF;
}

// Field READ function
static __inline UInt32  S2STATS_Configuration_READ_FifoSize(DtBcS2STATS_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8) & 0xFFFF;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Control' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32  S2STATS_Control_READ(DtBcS2STATS_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0xC);
}

// Field SET function
static __inline UInt32  S2STATS_Control_SET_ClearFifo(UInt32 Control, UInt32 ClearFifo)
{
    DT_ASSERT((ClearFifo & ~0x1) == 0);
    return (Control & ~0x1) | ClearFifo;
}

// Register WRITE function
static __inline void  S2STATS_Control_WRITE(DtBcS2STATS_2132* pBc, UInt32 Control)
{
    DtBc_RegWrite32((DtBc*)pBc, 0xC, Control);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Status' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32  S2STATS_Status_READ(DtBcS2STATS_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x10);
}

// Field GET function
static __inline UInt32  S2STATS_Status_GET_FifoLoad(UInt32 Status)
{
    return Status & 0xFFFF;
}

// Field READ function
static __inline UInt32  S2STATS_Status_READ_FifoLoad(DtBcS2STATS_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x10) & 0xFFFF;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'FrameStatus' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32  S2STATS_FrameStatus_READ(DtBcS2STATS_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x14);
}

// Field GET functions
static __inline UInt32  S2STATS_FrameStatus_GET_PLS(UInt32 FrameStatus)
{
    return FrameStatus & 0xFF;
}
static __inline UInt32  S2STATS_FrameStatus_GET_InputStreamId(UInt32 FrameStatus)
{
    return (FrameStatus & 0xFF00) >> 8;
}
static __inline UInt32  S2STATS_FrameStatus_GET_MER(UInt32 FrameStatus)
{
    return (FrameStatus & 0x1FF0000) >> 16;
}
static __inline UInt32  S2STATS_FrameStatus_GET_IsBchOk(UInt32 FrameStatus)
{
    return (FrameStatus & 0x2000000) >> 25;
}
static __inline UInt32  S2STATS_FrameStatus_GET_IsLdpcOk(UInt32 FrameStatus)
{
    return (FrameStatus & 0x4000000) >> 26;
}
static __inline UInt32  S2STATS_FrameStatus_GET_CcmAcm(UInt32 FrameStatus)
{
    return (FrameStatus & 0x8000000) >> 27;
}

