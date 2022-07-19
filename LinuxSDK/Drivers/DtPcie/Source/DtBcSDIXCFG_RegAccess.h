//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDIXCFG_RegAccess.h *#*#*#*#*#*#*#* (C) 2022 DekTec
//
// Device driver - SdiXcvrReconfig v1 (SDIXCFG) - Register access functions
//
// The register access functions defined here are elementary inline functions to read
// from and write to registers of BB2.0 building block SdiXcvrReconfig.
// These functions should solely be used by the block controller for SdiXcvrReconfig.
//
// 2022.01.03 11:58 - AUTOGENERATED BY DtBlock - DO NOT EDIT THIS FILE BY HAND.

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
// IN No EVENT SHALL DEKTEC DIGITAL VIDEO BV, ITS AGENTS OR ITS EMPLOYEES BE LIABLE FOR
// ANY DIRECT, INDIRECT, CONSEQUENTIAL, INCIDENTAL, OR OTHER DAMAGES (INCLUDING DAMAGES
// FOR THE LOSS OF USE, INFORMATION, GOODWILL, PROFIT, WORK STOPPAGE, DATA, BUSINESS OR
// REVENUE) UNDER ANY CIRCUMSTANCES, OR UNDER ANY LEGAL THEORY, WHETHER IN CONTRACT, IN
// TORT, IN NEGLIGENCE, OR OTHERWISE, ARISING FROM THE USE OF, OR INABILITY TO USE THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ EXAMPLES +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// 1. Getting fields 'IsBusy' and 'AccessTimedout' from register 'Status':
//      UInt32  Reg_Status, IsBusy, AccessTimedout;
//      Reg_Status     = SDIXCFG_Status_READ(pBc);
//      IsBusy         = SDIXCFG_Status_GET_IsBusy(Reg_Status);
//      AccessTimedout = SDIXCFG_Status_GET_AccessTimedout(Reg_Status);
//
// 2. Single-function shorthand for reading field 'Error' from register
//    'PllControlStatus':
//      UInt32  Error = SDIXCFG_PllControlStatus_READ_Error(pBc);
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

// SDIXCFG_Config_GET_NumReconfigs
// SDIXCFG_Config_Indexed_GET_ChannelType
// SDIXCFG_Config_Indexed_GET_PortIdx
// SDIXCFG_Config_Indexed_READ
// SDIXCFG_Config_Indexed_READ_ChannelType
// SDIXCFG_Config_Indexed_READ_PortIdx
// SDIXCFG_Config_READ
// SDIXCFG_Config_READ_NumReconfigs
// SDIXCFG_PllControlStatus_GET_Busy
// SDIXCFG_PllControlStatus_GET_Error
// SDIXCFG_PllControlStatus_READ
// SDIXCFG_PllControlStatus_READ_Busy
// SDIXCFG_PllControlStatus_READ_Error
// SDIXCFG_PllControlStatus_SET_Read
// SDIXCFG_PllControlStatus_SET_Write
// SDIXCFG_PllControlStatus_WRITE
// SDIXCFG_PllData_READ
// SDIXCFG_PllData_WRITE
// SDIXCFG_PllLogicalChannel_READ
// SDIXCFG_PllLogicalChannel_WRITE
// SDIXCFG_PllOffset_READ
// SDIXCFG_PllOffset_WRITE
// SDIXCFG_Status_CLEAR_AccessTimedout
// SDIXCFG_Status_GET_AccessTimedout
// SDIXCFG_Status_GET_IsBusy
// SDIXCFG_Status_READ
// SDIXCFG_Status_READ_AccessTimedout
// SDIXCFG_Status_READ_IsBusy
// SDIXCFG_StreamerControlStatus_GET_Busy
// SDIXCFG_StreamerControlStatus_GET_Error
// SDIXCFG_StreamerControlStatus_GET_Mode
// SDIXCFG_StreamerControlStatus_READ
// SDIXCFG_StreamerControlStatus_READ_Busy
// SDIXCFG_StreamerControlStatus_READ_Error
// SDIXCFG_StreamerControlStatus_SET_Mode
// SDIXCFG_StreamerControlStatus_SET_Read
// SDIXCFG_StreamerControlStatus_SET_Write
// SDIXCFG_StreamerControlStatus_WRITE
// SDIXCFG_StreamerData_READ
// SDIXCFG_StreamerData_WRITE
// SDIXCFG_StreamerLogicalChannel_READ
// SDIXCFG_StreamerLogicalChannel_WRITE
// SDIXCFG_StreamerOffset_READ
// SDIXCFG_StreamerOffset_WRITE

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Include Files +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#include "DtBc.h"                     // Block controller structs and functions

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Constant Definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Value map 'ChanType'
#define SDIXCFG_CHANTYPE_NOT_USED  0x0  // This reconfiguration interface is not used
#define SDIXCFG_CHANTYPE_RX_CHAN   0x1  // Receive channel (used by software for all RX
                                        // settings)
#define SDIXCFG_CHANTYPE_TX_CHAN   0x2  // Transmit channel (must be connected, not used
                                        // by software)
#define SDIXCFG_CHANTYPE_TX_PLL    0x3  // Transmit PLL (used by software for all TX
                                        // settings)

// Value map 'StreamerMode'
#define SDIXCFG_SM_MIF             0x0  // MIF
#define SDIXCFG_SM_DIRECT          0x1  // Direct Mode

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+ REGISTER-ACCESS FUNCTIONS - IMPLEMENTATION +=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Config' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32 SDIXCFG_Config_READ(DtBcSDIXCFG* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x4);
}

// Field GET function
static __inline UInt32 SDIXCFG_Config_GET_NumReconfigs(UInt32 Config)
{
    return Config & 0xF;
}

// Field READ function
static __inline UInt32 SDIXCFG_Config_READ_NumReconfigs(DtBcSDIXCFG* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x4) & 0xF;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Status' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32 SDIXCFG_Status_READ(DtBcSDIXCFG* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8);
}

// Field GET functions
static __inline UInt32 SDIXCFG_Status_GET_IsBusy(UInt32 Status)
{
    return Status & 0x1;
}
static __inline UInt32 SDIXCFG_Status_GET_AccessTimedout(UInt32 Status)
{
    return (Status & 0x2) >> 1;
}

// Field READ functions
static __inline UInt32 SDIXCFG_Status_READ_IsBusy(DtBcSDIXCFG* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x8) & 0x1;
}
static __inline UInt32 SDIXCFG_Status_READ_AccessTimedout(DtBcSDIXCFG* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x8) & 0x2) >> 1;
}

// Field CLEAR function
static __inline void SDIXCFG_Status_CLEAR_AccessTimedout(DtBcSDIXCFG* pBc)
{
    UInt32  Status = DtBc_RegRead32((DtBc*)pBc, 0x8);
    DtBc_RegWrite32((DtBc*)pBc, 0x8, Status | 0x2);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'Config_Indexed' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Register READ function
static __inline UInt32 SDIXCFG_Config_Indexed_READ(DtBcSDIXCFG* pBc, Int Idx)
{
    return DtBc_RegRead32((DtBc*)pBc, 0xC+4*Idx);
}

// Field GET functions
static __inline UInt32 SDIXCFG_Config_Indexed_GET_PortIdx(UInt32 Config_Indexed)
{
    return Config_Indexed & 0xFF;
}
static __inline UInt32 SDIXCFG_Config_Indexed_GET_ChannelType(UInt32 Config_Indexed)
{
    return (Config_Indexed & 0x300) >> 8;
}

// Field READ functions
static __inline UInt32 SDIXCFG_Config_Indexed_READ_PortIdx(DtBcSDIXCFG* pBc, Int Idx)
{
    return DtBc_RegRead32((DtBc*)pBc, 0xC+4*Idx) & 0xFF;
}
static __inline UInt32 SDIXCFG_Config_Indexed_READ_ChannelType(DtBcSDIXCFG* pBc, Int Idx)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0xC+4*Idx) & 0x300) >> 8;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'StreamerLogicalChannel' -.-.-.-.-.-.-.-.-.-.-.-.-.

// Field READ function
static __inline UInt32 SDIXCFG_StreamerLogicalChannel_READ(DtBcSDIXCFG* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x2E0) & 0x3FF;
}

// Field WRITE function
static __inline void SDIXCFG_StreamerLogicalChannel_WRITE(DtBcSDIXCFG* pBc, UInt32 StreamerLogicalChannel)
{
    DT_ASSERT((StreamerLogicalChannel & ~0x3FF) == 0);
    DtBc_RegWrite32((DtBc*)pBc, 0x2E0, StreamerLogicalChannel);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'StreamerControlStatus' -.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32 SDIXCFG_StreamerControlStatus_READ(DtBcSDIXCFG* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x2E8);
}

// Field GET functions
static __inline UInt32 SDIXCFG_StreamerControlStatus_GET_Mode(UInt32 StreamerControlStatus)
{
    return (StreamerControlStatus & 0xC) >> 2;
}
static __inline UInt32 SDIXCFG_StreamerControlStatus_GET_Busy(UInt32 StreamerControlStatus)
{
    return (StreamerControlStatus & 0x100) >> 8;
}
static __inline UInt32 SDIXCFG_StreamerControlStatus_GET_Error(UInt32 StreamerControlStatus)
{
    return (StreamerControlStatus & 0x200) >> 9;
}

// Field READ functions
static __inline UInt32 SDIXCFG_StreamerControlStatus_READ_Busy(DtBcSDIXCFG* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x2E8) & 0x100) >> 8;
}
static __inline UInt32 SDIXCFG_StreamerControlStatus_READ_Error(DtBcSDIXCFG* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x2E8) & 0x200) >> 9;
}

// Field SET functions
static __inline UInt32 SDIXCFG_StreamerControlStatus_SET_Write(UInt32 StreamerControlStatus, UInt32 Write)
{
    DT_ASSERT((Write & ~0x1) == 0);
    return (StreamerControlStatus & ~0x1) | Write;
}
static __inline UInt32 SDIXCFG_StreamerControlStatus_SET_Read(UInt32 StreamerControlStatus, UInt32 Read)
{
    DT_ASSERT((Read & ~0x1) == 0);
    return (StreamerControlStatus & ~0x2) | (Read << 1);
}
static __inline UInt32 SDIXCFG_StreamerControlStatus_SET_Mode(UInt32 StreamerControlStatus, UInt32 Mode)
{
    DT_ASSERT((Mode & ~0x3) == 0);
    return (StreamerControlStatus & ~0xC) | (Mode << 2);
}

// Register WRITE function
static __inline void SDIXCFG_StreamerControlStatus_WRITE(DtBcSDIXCFG* pBc, UInt32 StreamerControlStatus)
{
    DtBc_RegWrite32((DtBc*)pBc, 0x2E8, StreamerControlStatus);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'StreamerOffset' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Field READ function
static __inline UInt32 SDIXCFG_StreamerOffset_READ(DtBcSDIXCFG* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x2EC) & 0xFFFF;
}

// Field WRITE function
static __inline void SDIXCFG_StreamerOffset_WRITE(DtBcSDIXCFG* pBc, UInt32 StreamerOffset)
{
    DT_ASSERT((StreamerOffset & ~0xFFFF) == 0);
    DtBc_RegWrite32((DtBc*)pBc, 0x2EC, StreamerOffset);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'StreamerData' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Field READ function
static __inline UInt32 SDIXCFG_StreamerData_READ(DtBcSDIXCFG* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x2F0);
}

// Field WRITE function
static __inline void SDIXCFG_StreamerData_WRITE(DtBcSDIXCFG* pBc, UInt32 StreamerData)
{
    DtBc_RegWrite32((DtBc*)pBc, 0x2F0, StreamerData);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'PllLogicalChannel' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Field READ function
static __inline UInt32 SDIXCFG_PllLogicalChannel_READ(DtBcSDIXCFG* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x300) & 0x3FF;
}

// Field WRITE function
static __inline void SDIXCFG_PllLogicalChannel_WRITE(DtBcSDIXCFG* pBc, UInt32 PllLogicalChannel)
{
    DT_ASSERT((PllLogicalChannel & ~0x3FF) == 0);
    DtBc_RegWrite32((DtBc*)pBc, 0x300, PllLogicalChannel);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'PllControlStatus' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Register READ function
static __inline UInt32 SDIXCFG_PllControlStatus_READ(DtBcSDIXCFG* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x308);
}

// Field GET functions
static __inline UInt32 SDIXCFG_PllControlStatus_GET_Busy(UInt32 PllControlStatus)
{
    return (PllControlStatus & 0x100) >> 8;
}
static __inline UInt32 SDIXCFG_PllControlStatus_GET_Error(UInt32 PllControlStatus)
{
    return (PllControlStatus & 0x200) >> 9;
}

// Field READ functions
static __inline UInt32 SDIXCFG_PllControlStatus_READ_Busy(DtBcSDIXCFG* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x308) & 0x100) >> 8;
}
static __inline UInt32 SDIXCFG_PllControlStatus_READ_Error(DtBcSDIXCFG* pBc)
{
    return (DtBc_RegRead32((DtBc*)pBc, 0x308) & 0x200) >> 9;
}

// Field SET functions
static __inline UInt32 SDIXCFG_PllControlStatus_SET_Write(UInt32 PllControlStatus, UInt32 Write)
{
    DT_ASSERT((Write & ~0x1) == 0);
    return (PllControlStatus & ~0x1) | Write;
}
static __inline UInt32 SDIXCFG_PllControlStatus_SET_Read(UInt32 PllControlStatus, UInt32 Read)
{
    DT_ASSERT((Read & ~0x1) == 0);
    return (PllControlStatus & ~0x2) | (Read << 1);
}

// Register WRITE function
static __inline void SDIXCFG_PllControlStatus_WRITE(DtBcSDIXCFG* pBc, UInt32 PllControlStatus)
{
    DtBc_RegWrite32((DtBc*)pBc, 0x308, PllControlStatus);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'PllOffset' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Field READ function
static __inline UInt32 SDIXCFG_PllOffset_READ(DtBcSDIXCFG* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x30C) & 0xFFFF;
}

// Field WRITE function
static __inline void SDIXCFG_PllOffset_WRITE(DtBcSDIXCFG* pBc, UInt32 PllOffset)
{
    DT_ASSERT((PllOffset & ~0xFFFF) == 0);
    DtBc_RegWrite32((DtBc*)pBc, 0x30C, PllOffset);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register 'PllData' -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Field READ function
static __inline UInt32 SDIXCFG_PllData_READ(DtBcSDIXCFG* pBc)
{
    return DtBc_RegRead32((DtBc*)pBc, 0x310);
}

// Field WRITE function
static __inline void SDIXCFG_PllData_WRITE(DtBcSDIXCFG* pBc, UInt32 PllData)
{
    DtBc_RegWrite32((DtBc*)pBc, 0x310, PllData);
}

