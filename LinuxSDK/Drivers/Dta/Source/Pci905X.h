//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Pci905X.h *#*#*#*#*#*#*#*#*# (C) 2000-2016 DekTec
//
// Dta driver - Definition of register sets for the PCI905X PLX devices.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2000-2016 DekTec Digital Video B.V.
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dta driver, please refer to the Dta readme

#ifndef __PCI905X_H
#define __PCI905X_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register access -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
static __inline UInt8  Pci905xReadUInt8(volatile UInt8* pBase, Int ByteOffset)
{
    return *(volatile UInt8*)(pBase + ByteOffset);
}
static __inline UInt16  Pci905xReadUInt16(volatile UInt8* pBase, Int ByteOffset)
{
    return *(volatile UInt16*)(pBase + ByteOffset);
}
static __inline UInt32  Pci905xReadUInt32(volatile UInt8* pBase, Int ByteOffset)
{
    return *(volatile UInt32*)(pBase + ByteOffset);
}
static __inline void  Pci905xWriteUInt8(volatile UInt8* pBase, Int ByteOffset, UInt8 Val)
{
    *(volatile UInt8*)(pBase + ByteOffset) = Val;
}
static __inline void  Pci905xWriteUInt16(volatile UInt8* pBase, Int ByteOffset,
                                                                               UInt16 Val)
{
    *(volatile UInt16*)(pBase + ByteOffset) = Val;
}
static __inline void  Pci905xWriteUInt32(volatile UInt8* pBase, Int ByteOffset,
                                                                               UInt32 Val)
{
    *(volatile UInt32*)(pBase + ByteOffset) = Val;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// PLX PCI vendor and device ID
#define  PLX_VENDORID      0x10B5    // PLX vendor ID
#define  PLX9054_DEVICEID  0x9054    // PLX-9054 device ID
#define  PLX9056_DEVICEID  0x9056    // PLX-9056 device ID

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// This is the absolute maximum transfer size that can be programmed in a DMA transfer
// by the PCI 9054/9056.
// In scatter/gather mode this is the maximum transfer size per descriptor.
#define  PCI905X_MAX_DMA_TRANSFER_SIZE   0x7FFFFF        // 23 bits

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ REGISTER FIELDS +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// INTCSR: Interrupt Control / Status register
#define  PCI905X_INTCSR_PCI_INTEN    0x00000100  // PCI Interrupt Enable
#define  PCI905X_INTCSR_LOCAL_INTEN  0x00000800  // Local Interrupt Input Enable
#define  PCI905X_INTCSR_DMA0_INTEN   0x00040000  // DMA Channel 0 Interrupt Enable
#define  PCI905X_INTCSR_DMA1_INTEN   0x00080000  // DMA Channel 1 Interrupt Enable
#define  PCI905X_INTCSR_DMA0_INTACT  0x00200000 // DMA Channel 0 Interrupt Active
#define  PCI905X_INTCSR_DMA1_INTACT  0x00400000 // DMA Channel 1 Interrupt Active

// DMA Mode register
#define  PCI905X_DMAMODE_BUSWIDTH_32 0x00000003 // Local Bus Width is 32 bits
#define  PCI905X_DMAMODE_READY_INPEN 0x00000040 // Enable TA#/READY# input
#define  PCI905X_DMAMODE_BTERM_INPEN 0x00000080 // Enable BTERM# input
#define  PCI905X_DMAMODE_LOCALBURST  0x00000100 // Local Burst Enable
#define  PCI905X_DMAMODE_SCATGATH    0x00000200 // Scatter/Gather mode
#define  PCI905X_DMAMODE_DONE_INTEN  0x00000400 // DMA Done Interrupt Enable
#define  PCI905X_DMAMODE_LOC_CONST   0x00000800 // Hold Local Address Constant
#define  PCI905X_DMAMODE_DEMAND      0x00001000 // Scatter/Gather mode
#define  PCI905X_DMAMODE_INT2PCI     0x00020000 // Route Interrupts to PCI Bus
#define  PCI905X_DMAMODE_DAC         0x00040000 // Enable descriptor to load 
                                                // Dual Address Cycle

// DMA Descriptor Pointer register
#define  PCI905X_DMADPR_DESCLOC_PCI  0x00000001 // Descriptor location in PCI space
#define  PCI905X_DMADPR_ENDOFCHAIN   0x00000002 // End-of-chain indicator
#define  PCI905X_DMADPR_INTAFTERTC   0x00000004 // Interrupt after Terminal Count
#define  PCI905X_DMADPR_DIROFTFR     0x00000008 // Direction of Transfer

// DMA Command/Status
#define  PCI905X_DMACSR_ENABLE       0x00000001 // Enable channel to transfer data
#define  PCI905X_DMACSR_START        0x00000002 // Start transferring data
#define  PCI905X_DMACSR_ABORT        0x00000004 // Abort current transfer
#define  PCI905X_DMACSR_CLEARINT     0x00000008 // Clear interrupt
#define  PCI905X_DMACSR_DONE         0x00000010 // Transfer is complete

// Serial EEPROM Control, PCI Command Codes, User I/O Control and Init Control register
#define  PCI905X_CNTRL_GENPURPOUTP   0x00010000 // General Purpose Output
#define  PCI905X_CNTRL_EESK          0x01000000 // Serial EEPROM Clock
#define  PCI905X_CNTRL_EECS          0x02000000 // Serial EEPROM Chip Select
#define  PCI905X_CNTRL_EEDO          0x04000000 // Serial EEPROM Write bit (Data Out)
#define  PCI905X_CNTRL_EEDI          0x08000000 // Serial EEPROM Read bit (Data In)
#define  PCI9056_CNTRL_SOFTW_RESET   0x40000000 // Local-bus reset
#define  PCI9056_CNTRL_EEDO_INPEN    0x80000000 // Serial EEPROM EEDO Input Enable

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ REGISTER OFFSETS +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Base address when accessed from the PCI bus
#define  PCI905X_REG_BASE            0x000

//.-.-.-.-.-.-.-.-.-.-.-.-.- PCI Configuration Register Offsets -.-.-.-.-.-.-.-.-.-.-.-.-.
// PCI generic registers, valid for all PCI devices, not only PLX.
#define  PCI905X_VENDOR_ID           0x000
#define  PCI905X_COMMAND             0x004
#define  PCI905X_REV_ID              0x008
#define  PCI905X_CACHE_SIZE          0x00C
#define  PCI905X_LATENCY_TIMER       0x00D
#define  PCI905X_RTR_BASE            0x010
#define  PCI905X_RTR_IO_BASE         0x014
#define  PCI905X_LOCAL_BASE0         0x018
#define  PCI905X_LOCAL_BASE1         0x01C
#define  PCI905X_UNUSED_BASE1        0x020
#define  PCI905X_UNUSED_BASE2        0x024
#define  PCI905X_CIS_PTR             0x028
#define  PCI905X_SUB_ID              0x02C
#define  PCI905X_EXP_ROM_BASE        0x030
#define  PCI905X_CAP_PTR             0x034
#define  PCI905X_RESERVED2           0x038
#define  PCI905X_INT_LINE            0x03C
#define  PCI905X_MIN_GNT             0x03E
#define  PCI905X_MAX_LAT             0x03F
#define  PCI905X_PM_CAP_ID           0x040
#define  PCI905X_PM_CSR              0x044
#define  PCI905X_HS_CAP_ID           0x048
#define  PCI905X_VPD_CAP_ID          0x04C
#define  PCI905X_VPD_DATA            0x050

//-.-.-.-.-.-.-.-.-.-.-.-.- Local Configuration Register Offsets -.-.-.-.-.-.-.-.-.-.-.-.-
#define  PCI905X_SPACE0_RANGE        0x000
#define  PCI905X_SPACE0_REMAP        0x004
#define  PCI905X_LOCAL_DMA_ARBIT     0x008
#define  PCI905X_ENDIAN_DESC         0x00C
#define  PCI905X_LOCAL_MISC          0x00D
#define  PCI905X_PROT_AREA           0x00E   // 16-bit
#define  PCI905X_EXP_ROM_RANGE       0x010
#define  PCI905X_EXP_ROM_REMAP       0x014
#define  PCI905X_SPACE0_ROM_DESC     0x018
#define  PCI905X_DM_RANGE            0x01C
#define  PCI905X_DM_MEM_BASE         0x020
#define  PCI905X_DM_IO_BASE          0x024
#define  PCI905X_DM_PCI_MEM_REMAP    0x028
#define  PCI905X_DM_PCI_IO_CONFIG    0x02C
#define  PCI905X_SPACE1_RANGE        0x0F0
#define  PCI905X_SPACE1_REMAP        0x0F4
#define  PCI905X_SPACE1_DESC         0x0F8
#define  PCI905X_DM_DAC              0x0FC

// Runtime Registers
#define  PCI905X_MAILBOX0            0x078
#define  PCI905X_MAILBOX1            0x07C
#define  PCI905X_MAILBOX2            0x048
#define  PCI905X_MAILBOX3            0x04C
#define  PCI905X_MAILBOX4            0x050
#define  PCI905X_MAILBOX5            0x054
#define  PCI905X_MAILBOX6            0x058
#define  PCI905X_MAILBOX7            0x05C
#define  PCI905X_LOCAL_DOORBELL      0x060
#define  PCI905X_PCI_DOORBELL        0x064
#define  PCI905X_INT_CTRL_STAT       0x068
#define  PCI905X_EEPROM_CTRL_STAT    0x06C
#define  PCI905X_PERM_VENDOR_ID      0x070
#define  PCI905X_REVISION_ID         0x074

// DMA Registers
#define  PCI905X_DMA0_MODE           0x080
#define  PCI905X_DMA0_PCI_ADDR       0x084
#define  PCI905X_DMA0_LOCAL_ADDR     0x088
#define  PCI905X_DMA0_COUNT          0x08C
#define  PCI905X_DMA0_DESC_PTR       0x090
#define  PCI905X_DMA1_MODE           0x094
#define  PCI905X_DMA1_PCI_ADDR       0x098
#define  PCI905X_DMA1_LOCAL_ADDR     0x09C
#define  PCI905X_DMA1_COUNT          0x0A0
#define  PCI905X_DMA1_DESC_PTR       0x0A4
#define  PCI905X_DMA0_COMMAND_STAT   0x0A8
#define  PCI905X_DMA1_COMMAND_STAT   0x0A9
#define  PCI905X_DMA_ARBIT           0x0AC
#define  PCI905X_DMA_THRESHOLD       0x0B0
#define  PCI905X_DMA0_PCI_DAC        0x0B4
#define  PCI905X_DMA1_PCI_DAC        0x0B8

// Messaging FIFO Registers
#define  PCI905X_OUTPOST_INT_STAT    0x030
#define  PCI905X_OUTPOST_INT_MASK    0x034

#define  PCI905X_IN_FIFO_PORT        0x040
#define  PCI905X_OUT_FIFO_PORT       0x044

#define  PCI905X_MU_CONFIG           0x0C0
#define  PCI905X_FIFO_BASE_ADDR      0x0C4
#define  PCI905X_INFREE_HEAD_PTR     0x0C8
#define  PCI905X_INFREE_TAIL_PTR     0x0CC
#define  PCI905X_INPOST_HEAD_PTR     0x0D0
#define  PCI905X_INPOST_TAIL_PTR     0x0D4
#define  PCI905X_OUTFREE_HEAD_PTR    0x0D8
#define  PCI905X_OUTFREE_TAIL_PTR    0x0DC
#define  PCI905X_OUTPOST_HEAD_PTR    0x0E0
#define  PCI905X_OUTPOST_TAIL_PTR    0x0E4
#define  PCI905X_FIFO_CTRL_STAT      0x0E8

// Other defintions
#define MAX_PCI905X_REG_OFFSET      0x100


static __inline UInt32  Pci905xIntCtrlStatGet(UInt8* pBase) {
    return Pci905xReadUInt32(pBase, PCI905X_INT_CTRL_STAT);
}
static __inline void  Pci905xIntCtrlStatSet(UInt8* pBase, UInt32 Value) {
    Pci905xWriteUInt32(pBase, PCI905X_INT_CTRL_STAT, Value);
}

// Clear DMA Channel 0 interrupt (disables the channel, too)
static __inline void  Pci905xCmdStatDma0ClearInt(UInt8* pBase) {
    Pci905xWriteUInt8(pBase, PCI905X_DMA0_COMMAND_STAT, PCI905X_DMACSR_CLEARINT);
}

// Clear DMA Channel 1 interrupt (disables the channel, too)
static __inline void  Pci905xCmdStatDma1ClearInt(UInt8* pBase) {
    Pci905xWriteUInt8(pBase, PCI905X_DMA1_COMMAND_STAT, PCI905X_DMACSR_CLEARINT);
}

#endif // __PCI905X_H

