// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQFIR_RegAccess.h *#*#*#*#*#*#*#*# (C) 2022 DekTec
//
// Device driver - IqFirFilter v0 (IQFIR) - Register access functions
//
// The register access functions defined here are elementary inline functions to read
// from and write to registers of BB2.0 building block IqFirFilter.
// These functions should solely be used by the block controller for IqFirFilter.
//
// 2022.10.25 13:33 - AUTOGENERATED BY DtBlock - DO NOT EDIT THIS FILE BY HAND.

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Copyright (C) 2022 DekTec Digital Video B.V.
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
// IN No EVENT SHALL DEKTEC DIGITAL VIDEO BV, ITS AGENTS OR ITS EMPLOYEES BE LIABLE FOR
// ANY DIRECT, INDIRECT, CONSEQUENTIAL, INCIDENTAL, OR OTHER DAMAGES (INCLUDING DAMAGES
// FOR THE LOSS OF USE, INFORMATION, GOODWILL, PROFIT, WORK STOPPAGE, DATA, BUSINESS OR
// REVENUE) UNDER ANY CIRCUMSTANCES, OR UNDER ANY LEGAL THEORY, WHETHER IN CONTRACT, IN
// TORT, IN NEGLIGENCE, OR OTHERWISE, ARISING FROM THE USE OF, OR INABILITY TO USE THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ EXAMPLES +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// 1. Getting fields 'NumCoefs' and 'CoefBitDepth' from register 'Config':
//      UInt32 Reg_Config, NumCoefs, CoefBitDepth;
//      Reg_Config   = IQFIR_Config_READ(pBc);
//      NumCoefs     = IQFIR_Config_GET_NumCoefs(Reg_Config);
//      CoefBitDepth = IQFIR_Config_GET_CoefBitDepth(Reg_Config);
//
// 2. Single-function shorthand for reading field 'Ready' from register 'CoefControl':
//      UInt32  Ready = IQFIR_CoefControl_READ_Ready(pBc);
//
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ NOTES +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
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

// +=+=+=+=+=+=+=+=+=+=+ List of Generated Register-Access Functions +=+=+=+=+=+=+=+=+=+=+

// IQFIR_CoefControl_GET_Ready
// IQFIR_CoefControl_READ
// IQFIR_CoefControl_READ_Ready
// IQFIR_CoefControl_SET_ClearFifo
// IQFIR_CoefControl_SET_Direction
// IQFIR_CoefControl_SET_Start
// IQFIR_CoefControl_WRITE
// IQFIR_CoefReadFifo_READ
// IQFIR_CoefWriteFifo_WRITE
// IQFIR_Config_GET_CoefBitDepth
// IQFIR_Config_GET_NumCoefs
// IQFIR_Config_READ
// IQFIR_Config_READ_CoefBitDepth
// IQFIR_Config_READ_NumCoefs
// IQFIR_Control_READ
// IQFIR_Control_SET_BlockEnable
// IQFIR_Control_SET_OperationalMode
// IQFIR_Control_WRITE
// IQFIR_Prescale_WRITE

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Include Files +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#include "DtBc.h"                     // Block controller structs and functions

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Constant Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Value map 'BlockEnable'
#define IQFIR_BLKENA_DISABLED  0x0  // Block is entirely disabled
#define IQFIR_BLKENA_ENABLED   0x1  // Block is enabled and processes data

// Value map 'Direction'
#define IQFIR_CMD_READ         0x1  // Read filter coefficients from the filter core
#define IQFIR_CMD_WRITE        0x0  // Write filter coefficients to the filter core

// Value map 'OperationalMode'
#define IQFIR_OPMODE_IDLE      0x0  // Block is inactive
#define IQFIR_OPMODE_STANDBY   0x1  // Block is partially operational
#define IQFIR_OPMODE_RUN       0x2  // Block performs its normal function

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+ REGISTER-ACCESS FUNCTIONS - IMPLEMENTATION +=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Config' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32 IQFIR_Config_READ(DtBcIQFIR* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x4);
}

// Field GET functions
static __inline UInt32 IQFIR_Config_GET_NumCoefs(UInt32 Config)
{
    return Config & 0xFFFF;
}
static __inline UInt32 IQFIR_Config_GET_CoefBitDepth(UInt32 Config)
{
    return (Config & 0xFFFF0000) >> 16;
}

// Field READ functions
static __inline UInt32 IQFIR_Config_READ_NumCoefs(DtBcIQFIR* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x4) & 0xFFFF;
}
static __inline UInt32 IQFIR_Config_READ_CoefBitDepth(DtBcIQFIR* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x4) & 0xFFFF0000) >> 16;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Control' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32 IQFIR_Control_READ(DtBcIQFIR* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8);
}

// Field SET functions
static __inline UInt32 IQFIR_Control_SET_BlockEnable(UInt32 Control, UInt32 BlockEnable)
{
    DT_ASSERT((BlockEnable & ~0x1) == 0);
    return (Control & ~0x1) | BlockEnable;
}
static __inline UInt32 IQFIR_Control_SET_OperationalMode(UInt32 Control, UInt32 OperationalMode)
{
    DT_ASSERT((OperationalMode & ~0x3) == 0);
    return (Control & ~0x6) | (OperationalMode << 1);
}

// Register WRITE function
static __inline void IQFIR_Control_WRITE(DtBcIQFIR* pBc, UInt32 Control)
{
    DtBc_RegWrite32((DtBc*)pBc, 0x8, Control);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'CoefControl' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32 IQFIR_CoefControl_READ(DtBcIQFIR* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0xC);
}

// Field GET function
static __inline UInt32 IQFIR_CoefControl_GET_Ready(UInt32 CoefControl)
{
    return CoefControl & 0x1;
}

// Field READ function
static __inline UInt32 IQFIR_CoefControl_READ_Ready(DtBcIQFIR* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0xC) & 0x1;
}

// Field SET functions
static __inline UInt32 IQFIR_CoefControl_SET_Start(UInt32 CoefControl, UInt32 Start)
{
    DT_ASSERT((Start & ~0x1) == 0);
    return (CoefControl & ~0x2) | (Start << 1);
}
static __inline UInt32 IQFIR_CoefControl_SET_ClearFifo(UInt32 CoefControl, UInt32 ClearFifo)
{
    DT_ASSERT((ClearFifo & ~0x1) == 0);
    return (CoefControl & ~0x4) | (ClearFifo << 2);
}
static __inline UInt32 IQFIR_CoefControl_SET_Direction(UInt32 CoefControl, UInt32 Direction)
{
    DT_ASSERT((Direction & ~0x1) == 0);
    return (CoefControl & ~0x8) | (Direction << 3);
}

// Register WRITE function
static __inline void IQFIR_CoefControl_WRITE(DtBcIQFIR* pBc, UInt32 CoefControl)
{
    DtBc_RegWrite32((DtBc*)pBc, 0xC, CoefControl);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'CoefReadFifo' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Field READ function
static __inline UInt32 IQFIR_CoefReadFifo_READ(DtBcIQFIR* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x10) & 0x3FFFF;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'CoefWriteFifo' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Field WRITE function
static __inline void IQFIR_CoefWriteFifo_WRITE(DtBcIQFIR* pBc, UInt32 CoefWriteFifo)
{
    DT_ASSERT((CoefWriteFifo & ~0x3FFFF) == 0);
    DtBc_RegWrite32((DtBc*)pBc, 0x14, CoefWriteFifo);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Prescale' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Field WRITE function
static __inline void IQFIR_Prescale_WRITE(DtBcIQFIR* pBc, UInt32 Prescale)
{
    DT_ASSERT((Prescale & ~0xFFFF) == 0);
    DtBc_RegWrite32((DtBc*)pBc, 0x18, Prescale);
}

