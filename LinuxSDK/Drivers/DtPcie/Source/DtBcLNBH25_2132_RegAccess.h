//*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcLNBH25_2132_2132_RegAccess.h *#*#*#*#* (C) 2017-2018 DekTec
//
// Device driver - DiseqcLnbh25Ctrl v-1 (LNBH25) - Register access functions
//
// The register access functions defined here are elementary inline functions to read
// from and write to registers of BB2.0 building block DiseqcLnbh25Ctrl.
// These functions should solely be used by the block controller for DiseqcLnbh25Ctrl.
//
// 2017.11.07 8:59 - AUTOGENERATED BY DtBlock - DO NOT EDIT THIS FILE BY HAND

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

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

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ EXAMPLES +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// 1. Getting fields 'TxFifoSize' and 'RxFifoSize' from register 'Configuration':
//      UInt32  Reg_Configuration, TxFifoSize, RxFifoSize;
//      Reg_Configuration = LNBH25_Configuration_READ(pBc);
//      TxFifoSize        = LNBH25_Configuration_GET_TxFifoSize(Reg_Configuration);
//      RxFifoSize        = LNBH25_Configuration_GET_RxFifoSize(Reg_Configuration);
//
// 2. Single-function shorthand for reading field 'ParityErrorCounter' from register
//    'RxStatus':
//      UInt32  ParityErrorCounter = LNBH25_RxStatus_READ_ParityErrorCounter(pBc);
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

// LNBH25_Configuration_GET_RxFifoSize
// LNBH25_Configuration_GET_TxFifoSize
// LNBH25_Configuration_READ
// LNBH25_Configuration_READ_RxFifoSize
// LNBH25_Configuration_READ_TxFifoSize
// LNBH25_FaultControl_PULSE_Reset
// LNBH25_RxControl_READ
// LNBH25_RxControl_SET_RxEnable
// LNBH25_RxControl_SET_RxFifoClear
// LNBH25_RxControl_WRITE
// LNBH25_RxData_READ
// LNBH25_RxStatus_GET_FifoLoad
// LNBH25_RxStatus_GET_ParityErrorCounter
// LNBH25_RxStatus_READ
// LNBH25_RxStatus_READ_FifoLoad
// LNBH25_RxStatus_READ_ParityErrorCounter
// LNBH25_TxControl_READ
// LNBH25_TxControl_SET_Command
// LNBH25_TxControl_SET_Reset
// LNBH25_TxControl_WRITE
// LNBH25_TxData_WRITE
// LNBH25_TxStatus_READ_FifoLoad

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Include Files +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#include "DtBc.h"                     // Block controller structs and functions

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Constant Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Value map 'TxControl'
#define LNBH25_CMD_NOP               0x0  // No operation
#define LNBH25_CMD_SEND_TX_FIFO      0x1  // Start transmitting DiSEqC FIFO
#define LNBH25_CMD_SEND_TONEBURST_A  0x2  // Start transmitting unmodulated tone burst
#define LNBH25_CMD_SEND_TONEBURST_B  0x3  // Start transmitting modulated tone burst

// Value map 'RxEnable'
#define LNBH25_CMD_DISABLED          0x0  // Not receiving
#define LNBH25_CMD_ENABLED           0x1  // Receiving

// Value map 'Reset'
#define LNBH25_CMD_NOP               0x0  // No operation
#define LNBH25_CMD_RESET             0x1  // Reset transmit DiSEqC and clear the transmit
                                          // FIFO

// Value map 'Clear'
#define LNBH25_CMD_NOP               0x0  // No operation
#define LNBH25_CMD_CLEAR             0x1  // Clear

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+ REGISTER-ACCESS FUNCTIONS - IMPLEMENTATION +=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Configuration' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32  LNBH25_Configuration_READ(DtBcLNBH25_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x10);
}

// Field GET functions
static __inline UInt32  LNBH25_Configuration_GET_TxFifoSize(UInt32 Configuration)
{
    return Configuration & 0x1F;
}
static __inline UInt32  LNBH25_Configuration_GET_RxFifoSize(UInt32 Configuration)
{
    return (Configuration & 0x3E0) >> 5;
}

// Field READ functions
static __inline UInt32  LNBH25_Configuration_READ_TxFifoSize(DtBcLNBH25_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x10) & 0x1F;
}
static __inline UInt32  LNBH25_Configuration_READ_RxFifoSize(DtBcLNBH25_2132* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x10) & 0x3E0) >> 5;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'TxControl' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32  LNBH25_TxControl_READ(DtBcLNBH25_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x14);
}

// Field SET functions
static __inline UInt32  LNBH25_TxControl_SET_Command(UInt32 TxControl, UInt32 Command)
{
    DT_ASSERT((Command & ~0x3) == 0);
    return (TxControl & ~0x3) | Command;
}
static __inline UInt32  LNBH25_TxControl_SET_Reset(UInt32 TxControl, UInt32 Reset)
{
    DT_ASSERT((Reset & ~0x1) == 0);
    return (TxControl & ~0x4) | (Reset << 2);
}

// Register WRITE function
static __inline void  LNBH25_TxControl_WRITE(DtBcLNBH25_2132* pBc, UInt32 TxControl)
{
    DtBc_RegWrite32((DtBc*)pBc, 0x14, TxControl);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'TxStatus' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Field READ function
static __inline UInt32  LNBH25_TxStatus_READ_FifoLoad(DtBcLNBH25_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x18) & 0x1F;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'TxData' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Field WRITE function
static __inline void  LNBH25_TxData_WRITE(DtBcLNBH25_2132* pBc, UInt32 TxData)
{
    DT_ASSERT((TxData & ~0xFF) == 0);
    DtBc_RegWrite32((DtBc*)pBc, 0x1C, TxData);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'RxControl' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32  LNBH25_RxControl_READ(DtBcLNBH25_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x20);
}

// Field SET functions
static __inline UInt32  LNBH25_RxControl_SET_RxEnable(UInt32 RxControl, UInt32 RxEnable)
{
    DT_ASSERT((RxEnable & ~0x1) == 0);
    return (RxControl & ~0x1) | RxEnable;
}
static __inline UInt32  LNBH25_RxControl_SET_RxFifoClear(UInt32 RxControl, UInt32 RxFifoClear)
{
    DT_ASSERT((RxFifoClear & ~0x1) == 0);
    return (RxControl & ~0x2) | (RxFifoClear << 1);
}

// Register WRITE function
static __inline void  LNBH25_RxControl_WRITE(DtBcLNBH25_2132* pBc, UInt32 RxControl)
{
    DtBc_RegWrite32((DtBc*)pBc, 0x20, RxControl);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'RxStatus' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32  LNBH25_RxStatus_READ(DtBcLNBH25_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x24);
}

// Field GET functions
static __inline UInt32  LNBH25_RxStatus_GET_FifoLoad(UInt32 RxStatus)
{
    return RxStatus & 0x1F;
}
static __inline UInt32  LNBH25_RxStatus_GET_ParityErrorCounter(UInt32 RxStatus)
{
    return (RxStatus & 0x1FFE0) >> 5;
}

// Field READ functions
static __inline UInt32  LNBH25_RxStatus_READ_FifoLoad(DtBcLNBH25_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x24) & 0x1F;
}
static __inline UInt32  LNBH25_RxStatus_READ_ParityErrorCounter(DtBcLNBH25_2132* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x24) & 0x1FFE0) >> 5;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'RxData' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Field READ function
static __inline UInt32  LNBH25_RxData_READ(DtBcLNBH25_2132* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x28) & 0x1FF;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'FaultControl' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Field PULSE function
static __inline void  LNBH25_FaultControl_PULSE_Reset(DtBcLNBH25_2132* pBc)
{
    DtBc_RegWrite32((DtBc*)pBc, 0x2C, 0x1);
}

