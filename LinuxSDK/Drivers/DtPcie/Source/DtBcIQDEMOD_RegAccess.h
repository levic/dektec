// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQDEMOD_RegAccess.h *#*#*#*#*#*#*#* (C) 2022 DekTec
//
// Device driver - IqDemodulator v0 (IQDEMOD) - Register access functions
//
// The register access functions defined here are elementary inline functions to read
// from and write to registers of BB2.0 building block IqDemodulator.
// These functions should solely be used by the block controller for IqDemodulator.
//
// 2022.12.08 9:23 - AUTOGENERATED BY DtBlock - DO NOT EDIT THIS FILE BY HAND.

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

// IQDEMOD_Control_READ
// IQDEMOD_Control_SET_BlockEnable
// IQDEMOD_Control_SET_Bypass
// IQDEMOD_Control_SET_OperationalMode
// IQDEMOD_Control_WRITE
// IQDEMOD_NcoPhaseIncr_WRITE

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Include Files +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#include "DtBc.h"                     // Block controller structs and functions

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Constant Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Value map 'BlockEnable'
#define IQDEMOD_BLKENA_DISABLED  0x0  // Block is entirely disabled
#define IQDEMOD_BLKENA_ENABLED   0x1  // Block is enabled and processes data

// Value map 'OperationalMode'
#define IQDEMOD_OPMODE_IDLE      0x0  // Block is inactive
#define IQDEMOD_OPMODE_STANDBY   0x1  // Block is partially operational
#define IQDEMOD_OPMODE_RUN       0x2  // Block performs its normal function

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+ REGISTER-ACCESS FUNCTIONS - IMPLEMENTATION +=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Control' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function.
static __inline UInt32 IQDEMOD_Control_READ(DtBcIQDEMOD* Bc)
{
    return DtBc_RegRead32((DtBc*)Bc, 0x8);
}

// Field SET functions.
static __inline UInt32 IQDEMOD_Control_SET_BlockEnable(UInt32 Control, UInt32 BlockEnable)
{
    DT_ASSERT((BlockEnable & ~0x1) == 0);
    return (Control & ~0x1) | BlockEnable;
}
static __inline UInt32 IQDEMOD_Control_SET_OperationalMode(UInt32 Control, UInt32 OperationalMode)
{
    DT_ASSERT((OperationalMode & ~0x3) == 0);
    return (Control & ~0x6) | (OperationalMode << 1);
}
static __inline UInt32 IQDEMOD_Control_SET_Bypass(UInt32 Control, UInt32 Bypass)
{
    DT_ASSERT((Bypass & ~0x1) == 0);
    return (Control & ~0x8) | (Bypass << 3);
}

// Register WRITE function.
static __inline void IQDEMOD_Control_WRITE(DtBcIQDEMOD* Bc, UInt32 Control)
{
    DtBc_RegWrite32((DtBc*)Bc, 0x8, Control);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'NcoPhaseIncr' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Field WRITE function.
static __inline void IQDEMOD_NcoPhaseIncr_WRITE(DtBcIQDEMOD* Bc, UInt32 NcoPhaseIncr)
{
    DT_ASSERT((NcoPhaseIncr & ~0x3FFFF) == 0);
    DtBc_RegWrite32((DtBc*)Bc, 0xC, NcoPhaseIncr);
}

