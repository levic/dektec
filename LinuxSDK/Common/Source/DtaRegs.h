//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaRegs.h *#*#*#*#*#*#*#*#*# (C) 2010-2012 DekTec
//
// Dta driver - Definition of register sets of DTA PCI cards as access functions.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2012 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//  3. The source code may not be modified for the express purpose of enabling hardware
//     features for which no genuine license has been obtained.
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

#ifndef __DTA_REGS_H
#define __DTA_REGS_H

#include "DtRegs.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ General Access Functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- READ_UINT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline UInt32  READ_UINT(volatile UInt8* pBase, Int ByteOffset) {
    return *(volatile UInt32*)(pBase + ByteOffset);
}
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- WRITE_UINT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static __inline void  WRITE_UINT(UInt32 Val, volatile UInt8* pBase, Int ByteOffset) {
    *(volatile UInt32*)(pBase + ByteOffset) = Val;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- READ_UINT_MASKED -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline UInt32  READ_UINT_MASKED(volatile UInt8* pBase, Int ByteOffset, Int Mask, 
                                                                                Int Shift)
{
    UInt32 Value = READ_UINT(pBase, ByteOffset);
    Value &= Mask;
    Value = Value >> Shift;
    return Value;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- WRITE_UINT_MASKED -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline void  WRITE_UINT_MASKED(UInt32 Val, volatile UInt8* pBase, Int ByteOffset,
                                                                      Int Mask, Int Shift) 
{
    UInt32 RegValue = READ_UINT(pBase, ByteOffset);

    // Check new value
    DT_ASSERT(((Val << Shift) & ~Mask) == 0);
    //if (((Val << Shift) & ~Mask) != 0)
    //    DbgPrint("WRITE_UINT_MASKED: Val:0x%x, Offset 0x%x, Mask: 0x%x, Shift: %d",
    //                                                      Val, ByteOffset, Mask, Shift);

    // Or bits 
    RegValue &= ~Mask;
    RegValue |= Val<<Shift;

    WRITE_UINT(RegValue, pBase, ByteOffset);
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ General registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.- General Control Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.
// General Control: Register access
static __inline UInt  DtaRegGenCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_GENCTRL);
}
static __inline void  DtaRegGenCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_GENCTRL);
}

// General Control: PE
static __inline UInt  DtaRegGenCtrlGetPE(volatile UInt8* pBase) {
    return READ_UINT_MASKED( pBase, DT_GEN_REG_GENCTRL, DT_GENCTRL_PE_MSK, 
                                                                        DT_GENCTRL_PE_SH);
}
static __inline void  DtaRegGenCtrlSetPE(volatile UInt8* pBase, UInt PE) {
    WRITE_UINT_MASKED(PE, pBase, DT_GEN_REG_GENCTRL, DT_GENCTRL_PE_MSK, DT_GENCTRL_PE_SH);
}

// General Control: PRE
static __inline UInt  DtaRegGenCtrlGetPRE(volatile UInt8* pBase) {
    return READ_UINT_MASKED( pBase, DT_GEN_REG_GENCTRL, DT_GENCTRL_PRE_MSK, 
                                                                       DT_GENCTRL_PRE_SH);
}
static __inline void  DtaRegGenCtrlSetPRE(volatile UInt8* pBase, UInt PRE) {
    WRITE_UINT_MASKED(PRE, pBase, DT_GEN_REG_GENCTRL,  DT_GENCTRL_PRE_MSK, 
                                                                       DT_GENCTRL_PRE_SH);
}

// General Control: Reset
static __inline void  DtaRegGenCtrlReset(volatile UInt8* pBase) {
    WRITE_UINT_MASKED(1, pBase, DT_GEN_REG_GENCTRL, DT_GENCTRL_RESET_MSK, 
                                                                     DT_GENCTRL_RESET_SH);
}

// General Control: FirmwareRev
static __inline UInt  DtaRegGenCtrlGetFwRev(volatile UInt8* pBase) {
    return READ_UINT_MASKED( pBase, DT_GEN_REG_GENCTRL, DT_GENCTRL_FWREV_MSK, 
                                                                     DT_GENCTRL_FWREV_SH);
}

// General Control: TypeNumber
static __inline UInt  DtaRegGenCtrlGetTypeNumber(volatile UInt8* pBase) {
    return READ_UINT_MASKED( pBase, DT_GEN_REG_GENCTRL, DT_GENCTRL_TYPENUM_MSK, 
                                                                   DT_GENCTRL_TYPENUM_SH);
}

// General Control: PerIntEn
static __inline UInt  DtaRegGenCtrlGetPerIntEn(volatile UInt8* pBase) {
    return READ_UINT_MASKED( pBase, DT_GEN_REG_GENCTRL, DT_GENCTRL_PERINT_EN_MSK, 
                                                                 DT_GENCTRL_PERINT_EN_SH);

}
static __inline void  DtaRegGenCtrlSetPerIntEn(volatile UInt8* pBase, UInt PerIntEn) {
    WRITE_UINT_MASKED(PerIntEn, pBase, DT_GEN_REG_GENCTRL, DT_GENCTRL_PERINT_EN_MSK, 
                                                                 DT_GENCTRL_PERINT_EN_SH);
}

// General Control: PerIntInterval
static __inline UInt  DtaRegGenCtrlGetPerIntInterval(volatile UInt8* pBase) {
    return READ_UINT_MASKED( pBase, DT_GEN_REG_GENCTRL, DT_GENCTRL_PERINTVAL_MSK, 
                                                                 DT_GENCTRL_PERINTVAL_SH);
}
static __inline void  DtaRegGenCtrlSetPerIntInterval(volatile UInt8* pBase, UInt Value) {
    WRITE_UINT_MASKED(Value, pBase, DT_GEN_REG_GENCTRL, DT_GENCTRL_PERINTVAL_MSK,
                                                                 DT_GENCTRL_PERINTVAL_SH);
}

//.-.-.-.-.-.-.-.-.-.-.- General Status Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-
// General Status: Register access
static __inline UInt  DtaRegGenStatGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_GENSTATUS);
}
static __inline void  DtaRegGenStatSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_GENSTATUS);
}

// General Status: PerInt
static __inline UInt  DtaRegGenStatGetPerInt(volatile UInt8* pBase) {
    return READ_UINT_MASKED( pBase, DT_GEN_REG_GENSTATUS, DT_GENSTAT_PERINT_MSK, 
                                                                    DT_GENSTAT_PERINT_SH);
}
static __inline void  DtaRegGenStatClrPerInt(volatile UInt8* pBase) {
    WRITE_UINT(DT_GENSTAT_PERINT_MSK, pBase, DT_GEN_REG_GENSTATUS);
}

// General Status: Firmware Variant
static __inline UInt  DtaRegGenStatGetFwVariant(volatile UInt8* pBase) {
    return READ_UINT_MASKED( pBase, DT_GEN_REG_GENSTATUS, DT_GENSTAT_FIRMWVAR_MSK, 
                                                                  DT_GENSTAT_FIRMWVAR_SH);
}

// General Status: Type Number Extension
static __inline UInt  DtaRegGenStatGetTypeNumExt(volatile UInt8* pBase) {
    return READ_UINT_MASKED( pBase, DT_GEN_REG_GENSTATUS, DT_GENSTAT_TYPEEXT_MSK, 
                                                                   DT_GENSTAT_TYPEEXT_SH);
}


//-.-.-.-.-.-.-.-.-.- Reference Clock Count Register: Access Functions -.-.-.-.-.-.-.-.-.-

static __inline UInt32  DtaRegRefClkCntGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_REFCLKCNT);
}

static __inline UInt32  DtaRegRefClkCntGetH(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_REFCLKCNTH) & 0x3FFFFF;
}

static __inline UInt64  DtaRegRefClkCntGet64(volatile UInt8* pBase) {
    UInt32 RefClkH = DtaRegRefClkCntGetH(pBase);
    UInt32 RefClkL = DtaRegRefClkCntGet(pBase);

    if (RefClkH != DtaRegRefClkCntGetH(pBase)) {
        RefClkH++;
        RefClkL = DtaRegRefClkCntGet(pBase);
    }
    return ((UInt64) RefClkH << 32) + RefClkL;
}

//.-.-.-.-.- Periodic Interrupt Latched Clock Count Register: Access Functions -.-.-.-.-.-
// Note: Only present when firmware supports GENLOCK

static __inline UInt32  DtaRegRefClkCntLGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_REFCLKCNT_L);
}

static __inline UInt32  DtaRegRefClkCntLGetH(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_REFCLKCNTH_L) & 0x3FFFFF;
}

static __inline UInt64  DtaRegRefClkCntLGet64(volatile UInt8* pBase) {
    UInt32 RefClkH = DtaRegRefClkCntLGetH(pBase);
    UInt32 RefClkL = DtaRegRefClkCntLGet(pBase);

    if (RefClkH != DtaRegRefClkCntLGetH(pBase)) {
        RefClkH++;
        RefClkL = DtaRegRefClkCntLGet(pBase);
    }
    return ((UInt64) RefClkH << 32) + RefClkL;
}

//-.-.-.-.-.-.-.-.-.-.-.- EPCS Control Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

// EPCS Control: Register access
static __inline UInt  DtaRegEpcsCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_EPCSCTRL);
}
static __inline void  DtaRegEpcsCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_EPCSCTRL);
}

// EPCS Control: Erase
static __inline void  DtaRegEpcsCtrlSetAddrErase(volatile UInt8* pBase, UInt Addr) {
    UInt  Val = READ_UINT(pBase, DT_GEN_REG_EPCSCTRL);
    Val &= ~(DT_EPCSCTRL_ADDR_LSB_MSK | DT_EPCSCTRL_ADDR_MSB_MSK |
                     DT_EPCSCTRL_ERASE_MSK | DT_EPCSCTRL_PROG_MSK | DT_EPCSCTRL_READ_MSK);
    Val |= (Addr << DT_EPCSCTRL_ADDR_LSB_SH) & DT_EPCSCTRL_ADDR_LSB_MSK;
    Val |= ((Addr>>DT_EPCSCTRL_ADDR_LSB_NBITS)<<DT_EPCSCTRL_ADDR_MSB_SH)
                                                               & DT_EPCSCTRL_ADDR_MSB_MSK;
    Val |= DT_EPCSCTRL_ERASE_MSK;
    WRITE_UINT(Val, pBase, DT_GEN_REG_EPCSCTRL);
}

// EPCS Control: Page Program
static __inline void  DtaRegEpcsCtrlSetAddrProg(volatile UInt8* pBase, UInt Addr) {
    UInt  Val = READ_UINT(pBase, DT_GEN_REG_EPCSCTRL);
    Val &= ~(DT_EPCSCTRL_ADDR_LSB_MSK | DT_EPCSCTRL_ADDR_MSB_MSK |
                     DT_EPCSCTRL_ERASE_MSK | DT_EPCSCTRL_PROG_MSK | DT_EPCSCTRL_READ_MSK);
    Val |= (Addr << DT_EPCSCTRL_ADDR_LSB_SH) & DT_EPCSCTRL_ADDR_LSB_MSK;
    Val |= ((Addr>>DT_EPCSCTRL_ADDR_LSB_NBITS)<<DT_EPCSCTRL_ADDR_MSB_SH)
                                                               & DT_EPCSCTRL_ADDR_MSB_MSK;
    Val |= DT_EPCSCTRL_PROG_MSK;
    WRITE_UINT(Val, pBase, DT_GEN_REG_EPCSCTRL);
}

// EPCS Control: Read command
// After this call, data can be read from EPCS Data register
static __inline void  DtaRegEpcsCtrlSetAddrRead(volatile UInt8* pBase, UInt Addr) {
    UInt  Val = READ_UINT(pBase, DT_GEN_REG_EPCSCTRL);
    Val &= ~(DT_EPCSCTRL_ADDR_LSB_MSK | DT_EPCSCTRL_ADDR_MSB_MSK |
                     DT_EPCSCTRL_ERASE_MSK | DT_EPCSCTRL_PROG_MSK | DT_EPCSCTRL_READ_MSK);
    Val |= (Addr << DT_EPCSCTRL_ADDR_LSB_SH) & DT_EPCSCTRL_ADDR_LSB_MSK;
    Val |= ((Addr>>DT_EPCSCTRL_ADDR_LSB_NBITS)<<DT_EPCSCTRL_ADDR_MSB_SH)
                                                               & DT_EPCSCTRL_ADDR_MSB_MSK;
    Val |= DT_EPCSCTRL_READ_MSK;
    WRITE_UINT(Val, pBase, DT_GEN_REG_EPCSCTRL);
}

// EPCS Control: Busy
static __inline UInt  DtaRegEpcsCtrlGetBusy(volatile UInt8* pBase) {
    return READ_UINT_MASKED( pBase, DT_GEN_REG_EPCSCTRL, DT_EPCSCTRL_BUSY_MSK, 
                                                                     DT_EPCSCTRL_BUSY_SH);
}
// EPCS Control: Unlock
static __inline void  DtaRegEpcsCtrlSetUnlock(volatile UInt8* pBase, UInt UnlockValue) {
    WRITE_UINT_MASKED(UnlockValue, pBase, DT_GEN_REG_EPCSCTRL, DT_EPCSCTRL_UNLCK_MSK, 
                                                                    DT_EPCSCTRL_UNLCK_SH);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- EPCS Data Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// EPCS Data: Register access
static __inline UInt  DtaRegEpcsDataGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_EPCSDATA);
}
static __inline void DtaRegEpcsDataSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_EPCSDATA);
}

//.-.-.-.-.-.-.-.-.-.-.-.- SPI Control Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.
// SPI Control: Register access
static __inline UInt  DtaRegSpiCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_SPICTRL);
}
static __inline void  DtaRegSpiCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_SPICTRL);
}

// SPI Control: Page Program
static __inline void  DtaRegSpiCtrlSetAddrProg(volatile UInt8* pBase, UInt Addr) {
    UInt  Val = READ_UINT(pBase, DT_GEN_REG_SPICTRL);
    Val &= ~(DT_SPICTRL_ADDR_MSK | DT_SPICTRL_PROG_MSK | DT_SPICTRL_READ_MSK);
    Val |= (Addr << DT_SPICTRL_ADDR_SH) & DT_SPICTRL_ADDR_MSK;
    Val |= DT_SPICTRL_PROG_MSK;
    WRITE_UINT(Val, pBase, DT_GEN_REG_SPICTRL);
}

// SPI Control: Read command
// After this call, data can be read from SPI Data register
static __inline void  DtaRegSpiCtrlSetAddrRead(volatile UInt8* pBase, UInt Addr) {
    UInt  Val = READ_UINT(pBase, DT_GEN_REG_SPICTRL);
    Val &= ~(DT_SPICTRL_ADDR_MSK | DT_SPICTRL_PROG_MSK | DT_SPICTRL_READ_MSK);
    Val |= (Addr << DT_SPICTRL_ADDR_SH) & DT_SPICTRL_ADDR_MSK;
    Val |= DT_SPICTRL_READ_MSK;
    WRITE_UINT(Val, pBase, DT_GEN_REG_SPICTRL);
}

// SPI Control: Busy
static __inline UInt  DtaRegSpiCtrlGetBusy(volatile UInt8* pBase) {
    return READ_UINT_MASKED( pBase, DT_GEN_REG_SPICTRL, DT_SPICTRL_BUSY_MSK, 
                                                                     DT_SPICTRL_BUSY_SH);
}
// SPI Control: Unlock
static __inline void  DtaRegSpiCtrlSetUnlock(volatile UInt8* pBase, UInt UnlockValue) {
    WRITE_UINT_MASKED(UnlockValue, pBase, DT_GEN_REG_SPICTRL, DT_SPICTRL_UNLCK_MSK, 
                                                                    DT_SPICTRL_UNLCK_SH);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- SPI Data Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.

// SPI Data: Register access
static __inline UInt  DtaRegSpiDataGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_SPIDATA);
}
static __inline void  DtaRegSpiDataSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_SPIDATA);
}


//.-.-.-.-.-.-.-.-.-.-.- BoardControl0 Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  DtaRegControl0Get(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_CONTROL0);
}
static __inline void  DtaRegControl0Set(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_CONTROL0);
}

// Get VCO Enable
static __inline UInt  DtaRegControl0GetVcoEnable(volatile UInt8* pBase) {
    return READ_UINT_MASKED( pBase, DT_GEN_REG_CONTROL0, DT_GEN_CONTROL0_VCOEN_MSK, 
                                                                DT_GEN_CONTROL0_VCOEN_SH);
}

// Set VCO Enable
static __inline void  DtaRegControl0SetVcoEnable(volatile UInt8* pBase, UInt Enable) {
    WRITE_UINT_MASKED(Enable, pBase, DT_GEN_REG_CONTROL0, DT_GEN_CONTROL0_VCOEN_MSK,
                                                                DT_GEN_CONTROL0_VCOEN_SH);
}

// Get DblBuf Enable
static __inline UInt  DtaRegControl0GetDblBufEnable(volatile UInt8* pBase) {
    return READ_UINT_MASKED( pBase, DT_GEN_REG_CONTROL0, DT_GEN_CONTROL0_DBLBUF_MSK, 
                                                               DT_GEN_CONTROL0_DBLBUF_SH);
}

// Set DblBuf Enable
static __inline void  DtaRegControl0SetDblBufEnable(volatile UInt8* pBase, UInt Enable) {
    WRITE_UINT_MASKED(Enable, pBase, DT_GEN_REG_CONTROL0, DT_GEN_CONTROL0_DBLBUF_MSK, 
                                                               DT_GEN_CONTROL0_DBLBUF_SH);
}

// Get Diversity mode of board (DTA-2135)
static __inline UInt  DtaRegControl0GetDiversity(volatile UInt8* pBase) {
    return READ_UINT_MASKED( pBase, DT_GEN_REG_CONTROL0, DT_GEN_CONTROL0_DVRTY_MSK, 
                                                                DT_GEN_CONTROL0_DVRTY_SH);
}

// Set Diversity mode
static __inline void  DtaRegControl0SetDiversity(volatile UInt8* pBase, UInt Enable) {
    WRITE_UINT_MASKED(Enable, pBase, DT_GEN_REG_CONTROL0, DT_GEN_CONTROL0_DVRTY_MSK,
                                                                DT_GEN_CONTROL0_DVRTY_SH);
}

// Get 1V8SW and 3V3SW power enabled (DTA-2111)
static __inline UInt  DtaRegControl0GetPwrEnable(volatile UInt8* pBase) {
    return READ_UINT_MASKED( pBase, DT_GEN_REG_CONTROL0, DT_GEN_CONTROL0_PWREN_MSK, 
                                                                DT_GEN_CONTROL0_PWREN_SH);
}
// Set 1V8SW and 3V3SW power
static __inline void  DtaRegControl0SetPwrEnable(volatile UInt8* pBase, UInt Power) {
    WRITE_UINT_MASKED(Power, pBase, DT_GEN_REG_CONTROL0, DT_GEN_CONTROL0_PWREN_MSK, 
                                                                DT_GEN_CONTROL0_PWREN_SH);
}

//.-.-.-.-.-.-.-.-.-.-.- BoardControl1 Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  DtaRegControl1Get(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_CONTROL1);
}
static __inline void  DtaRegControl1Set(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_CONTROL1);
}

// Get watchdog bit
static __inline UInt  DtaRegControl1GetWatchdog(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_CONTROL1, DT_GEN_CONTROL1_WATCHDOG_MSK, 
                                                             DT_GEN_CONTROL1_WATCHDOG_SH);
}

// Set watchdog bit
static __inline void  DtaRegControl1SetWatchdog(volatile UInt8* pBase, UInt Watchdog) {
    WRITE_UINT_MASKED(Watchdog, pBase, DT_GEN_REG_CONTROL1, DT_GEN_CONTROL1_WATCHDOG_MSK,
                                                             DT_GEN_CONTROL1_WATCHDOG_SH);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- Status0 Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.

static __inline UInt  DtaRegStatus0Get(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_STATUS0);
}
static __inline void  DtaRegStatus0Set(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_STATUS0);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- Status1 Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.

static __inline UInt  DtaRegStatus1Get(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_STATUS1);
}
static __inline void  DtaRegStatus1Set(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_STATUS1);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- VCXO Control - Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

static __inline UInt  DtaRegVcxoCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_VCXOCTRL);
}
static __inline void  DtaRegVcxoCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_VCXOCTRL);
}

// Get VCXO enable
static __inline UInt DtaRegVcxoCtrlGetEnable(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_VCXOCTRL, DT_VCXOCTRL_EN_MSK, 
                                                                      DT_VCXOCTRL_EN_SH);
}

// Set VCXO enable
static __inline void  DtaRegVcxoCtrlSetEnable(volatile UInt8* pBase, UInt Ena) {
    WRITE_UINT_MASKED(Ena, pBase, DT_GEN_REG_VCXOCTRL, DT_VCXOCTRL_EN_MSK,
                                                                      DT_VCXOCTRL_EN_SH);
}

static __inline UInt DtaRegVcxoCtrlGetVidStd(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_VCXOCTRL, DT_VCXOCTRL_VIDSTD_MSK, 
                                                                   DT_VCXOCTRL_VIDSTD_SH);
}

static __inline void  DtaRegVcxoCtrlSetVidStd(volatile UInt8* pBase, UInt Std) {

    WRITE_UINT_MASKED(Std, pBase, DT_GEN_REG_VCXOCTRL, DT_VCXOCTRL_VIDSTD_MSK,
                                                                   DT_VCXOCTRL_VIDSTD_SH);
}

// Get VCXO speed
static __inline UInt  DtaRegVcxoCtrlGetSpeed(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_VCXOCTRL, DT_VCXOCTRL_SPEED_MSK, 
                                                                    DT_VCXOCTRL_SPEED_SH);
}
// Set VCXO speed (Speed = 0..1023)
static __inline void  DtaRegVcxoCtrlSetSpeed(volatile UInt8* pBase, UInt Speed) {
    WRITE_UINT_MASKED(Speed, pBase, DT_GEN_REG_VCXOCTRL, DT_VCXOCTRL_SPEED_MSK,
                                                                    DT_VCXOCTRL_SPEED_SH);
}

//-.-.-.-.-.-.-.-.-.-.-.- Transmit SDI Position - Access Functions -.-.-.-.-.-.-.-.-.-.-.-

static __inline UInt32  DtaRegTxSdiPosGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_SDIPOS);
}
static __inline void  DtaRegTxSdiPosSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_SDIPOS);
}

// Get Transmit SDI Position
// Return value: frame count; Line count and Sample count are returned through pointers
static __inline UInt  DtaRegTxSdiPosGetLineSample(volatile UInt8* pBase, UInt* pLineCnt,
                                                                           UInt* pSampCnt)
{
    UInt  RegVal = READ_UINT(pBase, DT_GEN_REG_SDIPOS);

    *pSampCnt = (RegVal & DT_TXSDIPOS_SAMP_MSK)  >> DT_TXSDIPOS_SAMP_SH;
    *pLineCnt = (RegVal & DT_TXSDIPOS_LINE_MSK)  >> DT_TXSDIPOS_LINE_SH;
    return      (RegVal & DT_TXSDIPOS_FRAME_MSK) >> DT_TXSDIPOS_FRAME_SH;
}

// Set Transmit SDI Position
// Enables to force TX/RX Counter synchronisation
static __inline void  DtaRegTxSdiPosSetLineSample(volatile UInt8* pBase, UInt pLineCnt,
                                                                            UInt pSampCnt)
{
    UInt Val = (pLineCnt << DT_TXSDIPOS_LINE_SH) + (pSampCnt<<DT_TXSDIPOS_SAMP_SH);
    DtaRegTxSdiPosSet(pBase, Val);
}

//-.-.-.-.-.-.-.-.-.-.-.- I2C Control Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.
// I2C Control: Register access
static __inline UInt  DtaRegI2cCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_I2CCTRL);
}
static __inline void  DtaRegI2cCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_I2CCTRL);
}

// I2C Control: RdyIntEn
static __inline UInt  DtaRegI2cCtrlGetRdyIntEn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_I2CCTRL, DT_I2CCTRL_RDYINT_EN_MSK, 
                                                                 DT_I2CCTRL_RDYINT_EN_SH);
}
static __inline void  DtaRegI2cCtrlSetRdyIntEn(volatile UInt8* pBase, UInt RdyIntEn) {
    WRITE_UINT_MASKED(RdyIntEn, pBase, DT_GEN_REG_I2CCTRL, DT_I2CCTRL_RDYINT_EN_MSK,
                                                                 DT_I2CCTRL_RDYINT_EN_SH);
}

// I2C Control: BusSel
static __inline UInt  DtaRegI2cCtrlGetBusSel(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_I2CCTRL, DT_I2CCTRL_BUSSEL_MSK, 
                                                                 DT_I2CCTRL_BUSSEL_SH);
}
static __inline void  DtaRegI2cCtrlSetBusSel(volatile UInt8* pBase, UInt BusSel) {
    WRITE_UINT_MASKED(BusSel, pBase, DT_GEN_REG_I2CCTRL, DT_I2CCTRL_BUSSEL_MSK,
                                                                    DT_I2CCTRL_BUSSEL_SH);
}

// I2C Control: ClkDiv
static __inline UInt  DtaRegI2cCtrlGetClkDiv(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_I2CCTRL, DT_I2CCTRL_CLKDIV_MSK, 
                                                                    DT_I2CCTRL_CLKDIV_SH);
}
static __inline void  DtaRegI2cCtrlSetClkDiv(volatile UInt8* pBase, UInt ClkDiv) {
    WRITE_UINT_MASKED(ClkDiv, pBase, DT_GEN_REG_I2CCTRL, DT_I2CCTRL_CLKDIV_MSK,
                                                                    DT_I2CCTRL_CLKDIV_SH);
}


//.-.-.-.-.-.-.-.-.-.-.-.- I2C Status Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.
// I2C Status: Register access
static __inline UInt  DtaRegI2cStatusGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_I2CSTAT);
}
static __inline void  DtaRegI2cStatusSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_I2CSTAT);
}

// I2C Control: RdyInt
static __inline UInt  DtaRegI2cStatusGetRdyInt(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_I2CSTAT, DT_I2CSTAT_RDYINT_MSK, 
                                                                    DT_I2CSTAT_RDYINT_SH);
}
static __inline void  DtaRegI2cStatusClrRdyInt(volatile UInt8* pBase) {
    WRITE_UINT(DT_I2CSTAT_RDYINT_MSK, pBase, DT_GEN_REG_I2CSTAT);
}

// I2C Control: Write-Address-Nacked
static __inline UInt  DtaRegI2cStatusGetWrAddrNack(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_I2CSTAT, DT_I2CSTAT_WRADDR_NACK_MSK, 
                                                               DT_I2CSTAT_WRADDR_NACK_SH);
}

// I2C Control: Write-Data-Nacked
static __inline UInt  DtaRegI2cStatusGetWrDataNack(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_I2CSTAT, DT_I2CSTAT_WRDATA_NACK_MSK, 
                                                               DT_I2CSTAT_WRDATA_NACK_SH);
}

// I2C Control: Read-Address-Nacked
static __inline UInt  DtaRegI2cStatusGetRdAddrNack(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_I2CSTAT, DT_I2CSTAT_RDADDR_NACK_MSK, 
                                                               DT_I2CSTAT_RDADDR_NACK_SH);
}

// I2C Control: Timeout
static __inline UInt  DtaRegI2cStatusGetTimeout(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_I2CSTAT, DT_I2CSTAT_TIMEOUT_MSK, 
                                                                   DT_I2CSTAT_TIMEOUT_SH);
}

//.-.-.-.-.-.-.-.-.-.- I2C Address/Count Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-

// I2C Address/Count: Register access
static __inline UInt  DtaRegI2cAddrCntGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_I2CADDRCNT);
}
static __inline void DtaRegI2cAddrCntSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_I2CADDRCNT);
}

// I2C Address/Count: Set Write Addres and Count in one go
static __inline void  DtaRegI2cAddrCntSetWrAddrAndCount(volatile UInt8* pBase, UInt Addr,
                                                                               UInt Count)
{
    UInt Val = (Addr<<DT_I2CADDRCNT_WRADDR_SH) & DT_I2CADDRCNT_WRADDR_MSK;
    Val |= (Count<<DT_I2CADDRCNT_WRCNT_SH) & DT_I2CADDRCNT_WRCNT_MSK;
    DtaRegI2cAddrCntSet(pBase, Val);
}

// I2C Address/Count: Write Address
static __inline UInt  DtaRegI2cAddrCntGetWrAddress(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_I2CADDRCNT, DT_I2CADDRCNT_WRADDR_MSK, 
                                                                 DT_I2CADDRCNT_WRADDR_SH);
}

// I2C Address/Count: Write Count
static __inline UInt  DtaRegI2cAddrCntGetWrCount(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_I2CADDRCNT, DT_I2CADDRCNT_WRCNT_MSK, 
                                                                 DT_I2CADDRCNT_WRCNT_SH);
}

// I2C Address/Count: Set Addres, WriteCount and ReadCount in one go
static __inline void  DtaRegI2cAddrCntSetWrRdAddrAndCount(volatile UInt8* pBase,
                           UInt WriteAddr, UInt WriteCount, UInt ReadAddr, UInt ReadCount)
{
    UInt Val = 0;
    // Skip write cycle if no bytes to write
    if (WriteCount > 0)
        Val |= (WriteAddr<<DT_I2CADDRCNT_WRADDR_SH) & DT_I2CADDRCNT_WRADDR_MSK;
    // Skip read cycle if no bytes to read
    if (ReadCount > 0)
        Val |= (ReadAddr<<DT_I2CADDRCNT_RDADDR_SH) & DT_I2CADDRCNT_RDADDR_MSK;
    Val |= (WriteCount<<DT_I2CADDRCNT_WRCNT_SH) & DT_I2CADDRCNT_WRCNT_MSK;
    Val |= (ReadCount<<DT_I2CADDRCNT_RDCNT_SH) & DT_I2CADDRCNT_RDCNT_MSK;
    DtaRegI2cAddrCntSet(pBase, Val);
}

// I2C Address/Count: Set Read Addres and Count in one go
static __inline void  DtaRegI2cAddrCntSetRdAddrAndCount( volatile UInt8* pBase, UInt Addr,
                                                                               UInt Count)
{
    UInt Val = (Addr<<DT_I2CADDRCNT_RDADDR_SH) & DT_I2CADDRCNT_RDADDR_MSK;
    Val |= (Count<<DT_I2CADDRCNT_RDCNT_SH) & DT_I2CADDRCNT_RDCNT_MSK;
    DtaRegI2cAddrCntSet(pBase, Val);
}

// I2C Address/Count: Read Address
static __inline UInt  DtaRegI2cAddrCntGetRdAddress(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_I2CADDRCNT, DT_I2CADDRCNT_RDADDR_MSK, 
                                                                 DT_I2CADDRCNT_RDADDR_SH);
}

// I2C Address/Count: Read Count
static __inline UInt  DtaRegI2cAddrCntGetRdCount(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_I2CADDRCNT, DT_I2CADDRCNT_RDCNT_MSK, 
                                                                  DT_I2CADDRCNT_RDCNT_SH);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- I2C Data Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// I2C Data: Register access
static __inline UInt  DtaRegI2cDataGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_I2CDATA);
}
static __inline void DtaRegI2cDataSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_I2CDATA);
}


//-.-.-.-.-.-.-.-.-.- VcxoFreq Measurement Registers: Access Functions -.-.-.-.-.-.-.-.-.-

static __inline UInt  DtaRegVcxoFreqGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_VCXOFREQ);
}

//.-.-.-.-.-.-.-.-.-.- ExtClk Measurement Registers: Access Functions -.-.-.-.-.-.-.-.-.-.

static __inline UInt  DtaRegExtClkFreqGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_EXTCLKFREQ);
}


//-.-.-.-.-.-.-.-.-.-.- DVB-S2 Frame Mode register: Access Functions -.-.-.-.-.-.-.-.-.-.-
// DVB-S2 Framemode: Register access
static __inline UInt  DtaGenS2FramemodeRegGet(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_GEN_REG_S2_FRAMEMODE, DT_S2_FRAMEMODE_MSK, 
                                                                      DT_S2_FRAMEMODE_SH);
}
static __inline void  DtaGenS2FramemodeRegSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT_MASKED(Val, pBase, DT_GEN_REG_S2_FRAMEMODE, DT_S2_FRAMEMODE_MSK,
                                                                      DT_S2_FRAMEMODE_SH);
}

//-.-.-.-.-.-.-.-.-.-.-.- DVB-S2 L3 Snr register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-
// DVB-S2 L3 Snr: Register access
static __inline UInt  DtaGenS2L3SnrRegGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_S2_L3_SNR);
}
static __inline void  DtaGenS2L3SnrRegSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_S2_L3_SNR);
}

//.-.-.-.-.-.-.-.-.-.-.-.- Licensing Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// Licensing Serial-Number: Register access
static __inline UInt  DtaRegLicSerialGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_LICSERIAL);
}
static __inline void  DtaRegLicSerialSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_LICSERIAL);
}
// Licensing Code: Register access
static __inline UInt  DtaRegLicCodeGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_LICCODE);
}
static __inline void  DtaRegLicCodeSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_GEN_REG_LICCODE);
}
// Licensing Status: Register access
static __inline UInt  DtaRegLicStatus1Get(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_LICSTATUS1);
}
static __inline UInt  DtaRegLicStatus2Get(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_GEN_REG_LICSTATUS2);
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DVB SPI registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.- SPI Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

// SPI Control: Register access
static __inline UInt  DtaRegDvbSpiCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_SPI_REG_SPICTRL);
}
static __inline void  DtaRegDvbSpiCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_SPI_REG_SPICTRL);
}

// SPI Control: SPI driver/receiver enabling
static __inline UInt  DtaRegDvbSpiCtrlGetIoEnable(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_IO_EN_MSK, 
                                                                     DT_SPICTRL_IO_EN_SH);
}
static __inline void  DtaRegDvbSpiCtrlSetIoEnable(volatile UInt8* pBase, UInt IoEnable) {
    WRITE_UINT_MASKED(IoEnable, pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_IO_EN_MSK,
                                                                     DT_SPICTRL_IO_EN_SH);
}

// SPI Control: IO standard (LVDS/LVTTL)
static __inline UInt  DtaRegDvbSpiCtrlGetLvttlSelect(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_LVTTL_SEL_MSK, 
                                                                 DT_SPICTRL_LVTTL_SEL_SH);
}
static __inline void  DtaRegDvbSpiCtrlSetLvttlSelect(volatile UInt8* pBase,
                                                                       UInt LvttlSelect) {
    WRITE_UINT_MASKED(LvttlSelect, pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_LVTTL_SEL_MSK,
                                                                 DT_SPICTRL_LVTTL_SEL_SH);
}

// SPI Control: LVDS receover mode
static __inline UInt  DtaRegDvbSpiCtrlGetFailsafeEnable(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_FAILSAFE_EN_MSK, 
                                                               DT_SPICTRL_FAILSAFE_EN_SH);
}
static __inline void  DtaRegDvbSpiCtrlSetFailsafeEnable(volatile UInt8* pBase,
                                                                            UInt Enable) {
    WRITE_UINT_MASKED(Enable, pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_FAILSAFE_EN_MSK,
                                                               DT_SPICTRL_FAILSAFE_EN_SH);
}

// SPI Control: DDS IO update pulse
static __inline UInt  DtaRegDvbSpiCtrlGetDdsIoUpdate(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_DDSIOUPDATE_MSK, 
                                                               DT_SPICTRL_DDSIOUPDATE_SH);
}
static __inline void  DtaRegDvbSpiCtrlSetDdsIoUpdate(volatile UInt8* pBase,
                                                                          UInt IoUpdate) {
    WRITE_UINT_MASKED(IoUpdate, pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_DDSIOUPDATE_MSK,
                                                               DT_SPICTRL_DDSIOUPDATE_SH);
}

// SPI Control: DVB-SPI mode (parallel/serial)
static __inline UInt  DtaRegDvbSpiCtrlGetSerialMode(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_SERIALMODE_MSK, 
                                                                DT_SPICTRL_SERIALMODE_SH);
}
static __inline void  DtaRegDvbSpiCtrlSetSerialMode(volatile UInt8* pBase,
                                                                        UInt SerialMode) {
    WRITE_UINT_MASKED(SerialMode, pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_SERIALMODE_MSK,
                                                                DT_SPICTRL_SERIALMODE_SH);
}

// SPI Control: Select internal/external SPI clock
static __inline UInt  DtaRegDvbSpiCtrlGetExtClkSelect(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_EXTCLK_SEL_MSK, 
                                                                DT_SPICTRL_EXTCLK_SEL_SH);
}
static __inline void  DtaRegDvbSpiCtrlSetExtClkSelect(volatile UInt8* pBase,
                                                                      UInt ExtClkSelect) {
    WRITE_UINT_MASKED(ExtClkSelect, pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_EXTCLK_SEL_MSK,
                                                                DT_SPICTRL_EXTCLK_SEL_SH);
}

// SPI Control: Select Test Mode
static __inline UInt  DtaRegDvbSpiCtrlGetTestMode(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_TESTMODE_MSK, 
                                                                  DT_SPICTRL_TESTMODE_SH);
}
static __inline void  DtaRegDvbSpiCtrlSetTestMode(volatile UInt8* pBase, UInt TestMode) {
    WRITE_UINT_MASKED(TestMode, pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_TESTMODE_MSK,
                                                                  DT_SPICTRL_TESTMODE_SH);
}

// SPI Control: Select PRNG mode
static __inline UInt  DtaRegDvbSpiCtrlGetPrngMode(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_PRNGMODE_MSK, 
                                                                  DT_SPICTRL_PRNGMODE_SH);
}
static __inline void  DtaRegDvbSpiCtrlSetPrngMode(volatile UInt8* pBase, UInt PrngMode) {
    WRITE_UINT_MASKED(PrngMode, pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_PRNGMODE_MSK,
                                                                  DT_SPICTRL_PRNGMODE_SH);
}

// SPI Control: Select 10 bit (serial) mode
static __inline UInt  DtaRegDvbSpiCtrlGetMode10B(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_MODE10B_MSK, 
                                                                   DT_SPICTRL_MODE10B_SH);
}
static __inline void  DtaRegDvbSpiCtrlSetMode10B(volatile UInt8* pBase, UInt Mode10B) {
    WRITE_UINT_MASKED(Mode10B, pBase, DT_SPI_REG_SPICTRL, DT_SPICTRL_MODE10B_MSK,
                                                                   DT_SPICTRL_MODE10B_SH);
}

//.-.-.-.-.-.-.-.-.-.-.-.- SPI Status register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// SPI Status: Register access
static __inline UInt  DtaRegDvbSpiStatusGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_SPI_REG_SPISTATUS);
}
static __inline void  DtaRegDvbSpiStatusSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_SPI_REG_SPISTATUS);
}

// SPI Status: DDS I/O update / serial port transfer in progress
static __inline UInt  DtaRegDvbSpiStatusGetDdsBusy(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_SPI_REG_SPISTATUS, DT_SPISTATUS_DDSBUSY_MSK, 
                                                                 DT_SPISTATUS_DDSBUSY_SH);
}

// SPI Status: I/O error
static __inline UInt  DtaRegDvbSpiStatusGetIoError(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_SPI_REG_SPISTATUS, DT_SPISTATUS_IOERROR_MSK, 
                                                                 DT_SPISTATUS_IOERROR_SH);
}
static __inline void  DtaRegDvbSpiStatusSetIoError(volatile UInt8* pBase) {
    WRITE_UINT_MASKED(1, pBase, DT_SPI_REG_SPISTATUS, DT_SPISTATUS_IOERROR_MSK,
                                                                 DT_SPISTATUS_IOERROR_SH);
}

//-.-.-.-.-.-.-.-.-.-.-.- SPI DDS data register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.
// SPI DDS data: Register access
static __inline void  DtaRegDvbSpiDdsDataSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_SPI_REG_SPIDDSDATA);
}

// SPI DDS data: DDS data
static __inline void  DtaRegDvbSpiDdsDataSetDdsData(volatile UInt8* pBase, UInt DdsData) {
    WRITE_UINT_MASKED(DdsData, pBase, DT_SPI_REG_SPIDDSDATA, DT_SPIDDSDATA_DDSDATA_MSK,
                                                                DT_SPIDDSDATA_DDSDATA_SH);
}

//-.-.-.-.-.-.-.-.-.-.-.- SPI Test data register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-
// SPI Test data: Register access
static __inline UInt  DtaRegDvbSpiTestDataGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_SPI_REG_SPITESTDATA);
}
static __inline void  DtaRegDvbSpiTestDataSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_SPI_REG_SPITESTDATA);
}

// SPI Test data: Test data
static __inline UInt  DtaRegDvbSpiTestDataGetTestData(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_SPI_REG_SPITESTDATA, DT_SPITESTDATA_TESTDATA_MSK, 
                                                              DT_SPITESTDATA_TESTDATA_SH);
}
static __inline void  DtaRegDvbSpiDdsDataSetTestData(volatile UInt8* pBase, UInt TstData)
{
    WRITE_UINT_MASKED(TstData, pBase, DT_SPI_REG_SPITESTDATA, DT_SPITESTDATA_TESTDATA_MSK,
                                                              DT_SPITESTDATA_TESTDATA_SH);
}

//.-.-.-.-.-.-.-.-.-.-.- SPI PRNG Errors register: Access Functions -.-.-.-.-.-.-.-.-.-.-.
// SPI PRNG Errors: Register access
static __inline UInt  DtaRegDvbSpiPrngErrorsGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_SPI_REG_SPIPRNGERRORS);
}

// SPI PRNG Errors: Number of syndromes sinces PrngMode enabled, reset when disabled
static __inline UInt  DtaRegDvbSpiPrngErrorsGetErrors(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_SPI_REG_SPIPRNGERRORS, DT_SPIPRNGERRORS_ERRORS_MSK, 
                                                              DT_SPIPRNGERRORS_ERRORS_SH);
}

//-.-.-.-.-.-.-.-.-.-.- SPI Rx Frequency register: Access Functions -.-.-.-.-.-.-.-.-.-.-.

// SPI Rx Freq: Register access
static __inline UInt  DtaRegDvbSpiRxFreqGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_SPI_REG_SPIRXFREQ);
}

// SPI Rx Freq: Rx frequency count
static __inline UInt  DtaRegDvbSpiRxFreqGetFrequency(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_SPI_REG_SPIRXFREQ, DT_SPIRXFREQ_FREQUENCY_MSK, 
                                                              DT_SPIRXFREQ_FREQUENCY_SH);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Board ID +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

static __inline UInt64  DtaRegBoardId(volatile UInt8* pBase) {
    UInt64  Id = (UInt64)READ_UINT(pBase, DT_GEN_REG_BOARDID0);
    Id |= ((UInt64)READ_UINT(pBase, DT_GEN_REG_BOARDID1))<<32;
    return Id;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ HD Genlock registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.- Genlock Clock Config register: Access Functions -.-.-.-.-.-.-.-.-.-.

// Genlock Clock Config register: reference source
static __inline void  DtaRegHdGenlClkConfSetRefSrc(volatile UInt8* pBase, UInt  Src) {
    WRITE_UINT_MASKED(Src, pBase, DT_HDGENL_REG_CLKCONF, DT_HDGENL_CLKCONF_REFSRC_MSK,
                                                             DT_HDGENL_CLKCONF_REFSRC_SH);
}

// Genlock Clock Config register: ASYNC reset
static __inline void  DtaRegHdGenlClkConfSetAsyncReset(volatile UInt8* pBase, UInt  Rst) {
    WRITE_UINT_MASKED(Rst, pBase, DT_HDGENL_REG_CLKCONF, DT_HDGENL_CLKCONF_ASYNCRST_MSK,
                                                           DT_HDGENL_CLKCONF_ASYNCRST_SH);
}

// Genlock Clock Config register: interlaced
static __inline void  DtaRegHdGenlClkConfSetInterlaced(volatile UInt8* pBase, UInt  Ena) {
    WRITE_UINT_MASKED(Ena, pBase, DT_HDGENL_REG_CLKCONF, DT_HDGENL_CLKCONF_INTERL_MSK,
                                                             DT_HDGENL_CLKCONF_INTERL_SH);
}

//.-.-.-.-.-.-.-.-.-.-.- Genlock Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.

// Genlock Control register:  SOF-Frame interrupt enable
static __inline void  DtaRegHdGenlCtrlSetSofIntEna(volatile UInt8* pBase, UInt  Ena) {
    WRITE_UINT_MASKED(Ena, pBase, DT_HDGENL_REG_CTRL, DT_HDGENL_CTRL_SOFINTEN_MSK,
                                                              DT_HDGENL_CTRL_SOFINTEN_SH);
}

//.-.-.-.-.-.-.-.-.-.-.- Genlock Status register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-

// Genlock Status register: SOF-Frame interrupt 
static __inline UInt32  DtaRegHdGenlStatGetSofInt(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HDGENL_REG_STATUS, DT_HDGENL_STATUS_SOFINT_MSK,
                                                              DT_HDGENL_STATUS_SOFINT_SH);
}
static __inline void  DtaRegHdGenlStatClrSofInt(volatile UInt8* pBase)
{
    // Write '1' to the SOF-frame interrupt status bit to clear it
    WRITE_UINT(DT_HDGENL_STATUS_SOFINT_MSK, pBase, DT_HDGENL_REG_STATUS);
}

//-.-.-.-.-.-.- Genlock Reference Frame register (LSB+MSB): Access Functions -.-.-.-.-.-.-

static __inline Int64 DtaRegHdGenlRefFrameGet(volatile UInt8* pBase)
{
    Int64  Frame = 0;
    // Read LSB part first
    Frame = (Int64)READ_UINT(pBase, DT_HDGENL_REG_REFFRM_LSB);
    Frame |= ((Int64)READ_UINT(pBase, DT_HDGENL_REG_REFFRM_MSB))<<32;
    return Frame;
}

//.-.-.-.-.-.-.-.- Genlock SOF Frame register (LSB+MSB): Access Functions -.-.-.-.-.-.-.-.

static __inline void DtaRegHdGenlSofFrameSet(volatile UInt8* pBase, Int64  Frame)
{
    // Write LSB part first
    WRITE_UINT((UInt32)(Frame&0xFFFFFFFF), pBase, DT_HDGENL_REG_SOFFRM_LSB);
    WRITE_UINT((UInt32)((Frame>>32)&0xFFFFFFFF), pBase, DT_HDGENL_REG_SOFFRM_MSB);
}
static __inline Int64 DtaRegHdGenlSofFrameGet(volatile UInt8* pBase)
{
    Int64  Frame = 0;
    // Read LSB part first
    Frame = (Int64)READ_UINT(pBase, DT_HDGENL_REG_SOFFRM_LSB);
    Frame |= ((Int64)READ_UINT(pBase, DT_HDGENL_REG_SOFFRM_MSB))<<32;
    return Frame;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ TX registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.- -.-.-.-.-.-.-.-.-.-.-.-.-.DTA-2148-.-.-.-.-.-.-.-.-.-.-.-.- -.-.-.-.-.-.-.
// GS2962 Transmitter specific
#define DTA_TX_REG_GS2962SPI       0x0060
#define DTA_TX_REG_GS2962CTRL      0x0064
#define DTA_TX_REG_GS2962STAT      0x0068

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2162 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA_TX_REG_SLICE_STATUS    0x0044
#define DTA_TX_REG_SLICE_POINTER   0x0048
#define DAT_TX_REG_SLICE_SIZE      0x004C


//.-.-.-.-.-.-.-.-.-.-.-.- Tx Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// Tx Control: Register access
static __inline UInt  DtaRegTxCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_TXCTRL);
}
static __inline void  DtaRegTxCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_TXCTRL);
}

//Tx Control TxMode
static __inline UInt  DtaRegTxCtrlGetTxMode(volatile UInt8* pBase) {
    UInt  Val = DtaRegTxCtrlGet(pBase);
    UInt  RetVal = ((Val&DT_TXCTRL_TXMODE_MSK)>>DT_TXCTRL_TXMODE_SH);
    // Get 3rd bit
    RetVal |= ((Val&DT_TXCTRL_TXMODE_EXT_MSK)>>DT_TXCTRL_TXMODE_EXT_SH)<<2;
    return RetVal;
}

static __inline void  DtaRegTxCtrlSetTxMode(volatile UInt8* pBase, UInt TxMode) {
    UInt  Val = DtaRegTxCtrlGet(pBase);
    Val &= ~(DT_TXCTRL_TXMODE_MSK | DT_TXCTRL_TXMODE_EXT_MSK);
    Val |= ((TxMode<<DT_TXCTRL_TXMODE_SH) & DT_TXCTRL_TXMODE_MSK);
    // Set 3rd bit
    Val |= (((TxMode>>2)<<DT_TXCTRL_TXMODE_EXT_SH) & DT_TXCTRL_TXMODE_EXT_MSK);
    DtaRegTxCtrlSet(pBase, Val);
}

// Tx Control: BurstMode
static __inline UInt  DtaRegTxCtrlGetTxBurstMode(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_BURSTMODE_MSK, 
                                                                  DT_TXCTRL_BURSTMODE_SH);
}
static __inline void  DtaRegTxCtrlSetTxBurstMode(volatile UInt8* pBase, UInt BurstMode) {
    WRITE_UINT_MASKED(BurstMode, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_BURSTMODE_MSK,
                                                                  DT_TXCTRL_BURSTMODE_SH);
}

// Tx Control: SDI Line mode
static __inline UInt  DtaRegTxCtrlGetSdiLineMode(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_SDI_LINEMODE_MSK, 
                                                               DT_TXCTRL_SDI_LINEMODE_SH);
}
static __inline void  DtaRegTxCtrlSetSdiLineMode(volatile UInt8* pBase, UInt En525Mode) {
    WRITE_UINT_MASKED(En525Mode, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_SDI_LINEMODE_MSK,
                                                               DT_TXCTRL_SDI_LINEMODE_SH);
}

// Tx Control: Packet Stuffing
static __inline UInt  DtaRegTxCtrlGetTxPckStuff(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_PCKSTUFF_MSK, 
                                                                   DT_TXCTRL_PCKSTUFF_SH);
}
static __inline void  DtaRegTxCtrlSetTxPckStuff(volatile UInt8* pBase, UInt PckStuff) {
    WRITE_UINT_MASKED(PckStuff, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_PCKSTUFF_MSK,
                                                                   DT_TXCTRL_PCKSTUFF_SH);
}

// Tx Control: TxCtrl
static __inline UInt  DtaRegTxCtrlGetTxCtrl(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_TXCTRL_MSK, 
                                                                    DT_TXCTRL_TXCTRL_SH);
}
static __inline void  DtaRegTxCtrlSetTxCtrl(volatile UInt8* pBase, UInt TxCtrl) {
    WRITE_UINT_MASKED(TxCtrl, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_TXCTRL_MSK,
                                                                    DT_TXCTRL_TXCTRL_SH);
}

// Tx Control: PerIntEn
static __inline UInt  DtaRegTxCtrlGetTxPerIntEn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_PERINT_EN_MSK, 
                                                                  DT_TXCTRL_PERINT_EN_SH);
}
static __inline void  DtaRegTxCtrlSetTxPerIntEn(volatile UInt8* pBase, UInt TxPerIntEn) {
    WRITE_UINT_MASKED(TxPerIntEn, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_PERINT_EN_MSK,
                                                                  DT_TXCTRL_PERINT_EN_SH);
}

// Tx Control: TxUflIntEn
static __inline UInt  DtaRegTxCtrlGetTxUflIntEn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_UFLINT_EN_MSK, 
                                                                  DT_TXCTRL_UFLINT_EN_SH);
}
static __inline void  DtaRegTxCtrlSetTxUflIntEn(volatile UInt8* pBase, UInt TxUflIntEn) {
    WRITE_UINT_MASKED(TxUflIntEn, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_UFLINT_EN_MSK,
                                                                  DT_TXCTRL_UFLINT_EN_SH);
}

// Tx Control: TxSyncIntEn
static __inline UInt  DtaRegTxCtrlGetTxSyncIntEn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_SYNCINT_EN_MSK, 
                                                                 DT_TXCTRL_SYNCINT_EN_SH);
}
static __inline void  DtaRegTxCtrlSetTxSyncIntEn(volatile UInt8* pBase, UInt TxSyncIntEn)
{
    WRITE_UINT_MASKED(TxSyncIntEn, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_SYNCINT_EN_MSK,
                                                                 DT_TXCTRL_SYNCINT_EN_SH);
}

// Tx Control: TxShortIntEn
static __inline UInt  DtaRegTxCtrlGetTxShortIntEn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_SHORTINT_EN_MSK, 
                                                                DT_TXCTRL_SHORTINT_EN_SH);
}
static __inline void  DtaRegTxCtrlSetTxShortIntEn(volatile UInt8* pBase, 
                                                                      UInt TxShortIntEn) {
    WRITE_UINT_MASKED(TxShortIntEn, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_SHORTINT_EN_MSK,
                                                                DT_TXCTRL_SHORTINT_EN_SH);
}

// Tx Control: TxThrIntEn (thresh-hold)
static __inline UInt  DtaRegTxCtrlGetTxThrIntEn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_THRINT_EN_MSK, 
                                                               DT_TXCTRL_THRINT_EN_SH);
}
static __inline void  DtaRegTxCtrlSetTxThrIntEn(volatile UInt8* pBase, UInt TxThrIntEn) {
    WRITE_UINT_MASKED(TxThrIntEn, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_THRINT_EN_MSK,
                                                               DT_TXCTRL_THRINT_EN_SH);
}

// Tx Control: TxDis
static __inline UInt  DtaRegTxCtrlGetTxDis(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_TXDIS_MSK, 
                                                                      DT_TXCTRL_TXDIS_SH);
}
static __inline void  DtaRegTxCtrlSetTxDis(volatile UInt8* pBase, UInt TxDis) {
    WRITE_UINT_MASKED(TxDis, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_TXDIS_MSK,
                                                                      DT_TXCTRL_TXDIS_SH);
}

// Tx Control: UseExtClk
static __inline UInt  DtaRegTxCtrlGetUseExtClk(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_USE_EXTCLOCK_MSK, 
                                                               DT_TXCTRL_USE_EXTCLOCK_SH);
}
static __inline void  DtaRegTxCtrlSetUseExtClk(volatile UInt8* pBase, UInt UseExtClk) {
    WRITE_UINT_MASKED(UseExtClk, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_USE_EXTCLOCK_MSK,
                                                               DT_TXCTRL_USE_EXTCLOCK_SH);
}

// Tx Control: EnaPwr
static __inline UInt  DtaRegTxCtrlGetEnaPwr(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_ENAPWR_MSK, 
                                                                     DT_TXCTRL_ENAPWR_SH);
}
static __inline void  DtaRegTxCtrlSetEnaPwr(volatile UInt8* pBase, UInt EnaPwr) {
    WRITE_UINT_MASKED(EnaPwr, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_ENAPWR_MSK,
                                                                     DT_TXCTRL_ENAPWR_SH);
}

// Tx Control: LED_Control
static __inline UInt  DtaRegTxCtrlGetLED_Control(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_LED_CTRL_MSK, 
                                                                    DT_TXCTRL_LED_CTRL_SH);
}
static __inline void  DtaRegTxCtrlSetLED_Control(volatile UInt8* pBase, UInt LedControl) {
    WRITE_UINT_MASKED(LedControl, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_LED_CTRL_MSK,
                                                                    DT_TXCTRL_LED_CTRL_SH);
}

// Tx Control: LedGreen
static __inline UInt  DtaRegTxCtrlGetLedGreen(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_LED_GREEN_MSK, 
                                                                  DT_TXCTRL_LED_GREEN_SH);
}
static __inline void  DtaRegTxCtrlSetLedGreen(volatile UInt8* pBase, UInt LedGreen) {
    WRITE_UINT_MASKED(LedGreen, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_LED_GREEN_MSK,
                                                                  DT_TXCTRL_LED_GREEN_SH);
}

// Tx Control: LedRed
static __inline UInt  DtaRegTxCtrlGetLedRed(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_LED_RED_MSK, 
                                                                     DT_TXCTRL_LED_RED_SH);
}
static __inline void  DtaRegTxCtrlSetLedRed(volatile UInt8* pBase, UInt LedRed) {
    WRITE_UINT_MASKED(LedRed, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_LED_RED_MSK,
                                                                     DT_TXCTRL_LED_RED_SH);
}

// Tx Control: Clear-FIFO
static __inline void  DtaRegTxCtrlClrFifo(volatile UInt8* pBase) {
    WRITE_UINT_MASKED(1, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_FIFO_CLR_MSK,
                                                                  DT_TXCTRL_FIFO_CLR_SH);
}

// Tx Control: Reset
static __inline void  DtaRegTxCtrlTxReset(volatile UInt8* pBase) {
    WRITE_UINT_MASKED(1, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_TXRESET_MSK,
                                                                    DT_TXCTRL_TXRESET_SH);
}

// Tx Control: TxInvAsi
static __inline UInt  DtaRegTxCtrlGetTxAsiInv(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_ASIINV_MSK, 
                                                                     DT_TXCTRL_ASIINV_SH);
}
static __inline void  DtaRegTxCtrlSetTxAsiInv(volatile UInt8* pBase, UInt TxInvAsi) {
    WRITE_UINT_MASKED(TxInvAsi, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_ASIINV_MSK,
                                                                     DT_TXCTRL_ASIINV_SH);
}

// Tx Control: SDI mode
static __inline UInt  DtaRegTxCtrlGetSdiMode(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_SDIMODE_MSK, 
                                                                    DT_TXCTRL_SDIMODE_SH);
}
static __inline void  DtaRegTxCtrlSetSdiMode(volatile UInt8* pBase, UInt SdiMode) {
    WRITE_UINT_MASKED(SdiMode, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_SDIMODE_MSK,
                                                                    DT_TXCTRL_SDIMODE_SH);
}

// Tx Control: SDI 10-Bit mode
static __inline UInt  DtaRegTxCtrlGetSdiMode10Bit(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_SDI10BIT_MSK, 
                                                                   DT_TXCTRL_SDI10BIT_SH);
}
static __inline void  DtaRegTxCtrlSetSdiMode10Bit(volatile UInt8* pBase, 
                                                                      UInt SdiMode10Bit) {
    WRITE_UINT_MASKED(SdiMode10Bit, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_SDI10BIT_MSK,
                                                                   DT_TXCTRL_SDI10BIT_SH);
}

// Tx Control: Output enable
static __inline UInt  DtaRegTxCtrlGetOutputEn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_OUT_EN_MSK, 
                                                                     DT_TXCTRL_OUT_EN_SH);
}
static __inline void  DtaRegTxCtrlSetOutputEn(volatile UInt8* pBase, UInt OutputEnable) {
    WRITE_UINT_MASKED(OutputEnable, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_OUT_EN_MSK,
                                                                     DT_TXCTRL_OUT_EN_SH);
}

// Tx Control: Transmit on time(stamp)
static __inline UInt  DtaRegTxCtrlGetTxOnTime(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_TXONTIME_MSK, 
                                                                   DT_TXCTRL_TXONTIME_SH);
}
static __inline void  DtaRegTxCtrlSetTxOnTime(volatile UInt8* pBase, UInt TxOnTime) {
    WRITE_UINT_MASKED(TxOnTime, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_TXONTIME_MSK,
                                                                   DT_TXCTRL_TXONTIME_SH);
}

// Tx Control: lock-to-input
static __inline UInt  DtaRegTxCtrlGetLock2Input(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_LOCK2INP_MSK, 
                                                                   DT_TXCTRL_LOCK2INP_SH);
}
static __inline void  DtaRegTxCtrlSetLock2Input(volatile UInt8* pBase, UInt Lock2Inp) {
    WRITE_UINT_MASKED(Lock2Inp, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_LOCK2INP_MSK,
                                                                   DT_TXCTRL_LOCK2INP_SH);
}

// Tx Control: SDI Huffman decompression enable
static __inline UInt  DtaRegTxCtrlGetSdiDecomprModeEn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_SDIHUFF_MSK, 
                                                                    DT_TXCTRL_SDIHUFF_SH);
}
static __inline void  DtaRegTxCtrlSetSdiDecomprModeEn(volatile UInt8* pBase, 
                                                                            UInt HuffEn) {
    WRITE_UINT_MASKED(HuffEn, pBase, DT_TX_REG_TXCTRL, DT_TXCTRL_SDIHUFF_MSK,
                                                                    DT_TXCTRL_SDIHUFF_SH);
}

//-.-.-.-.-.-.-.-.-.-.-.- Tx Control 2 register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

// Tx Control2: Register access
static __inline UInt  DtaRegTxCtrl2Get(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_TXCTRL2);
}
static __inline void  DtaRegTxCtrl2Set(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_TXCTRL2);
}

// Tx Control2: Transmit output source select
static __inline UInt  DtaRegTxCtrl2GetSrcSel(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL2, DT_TXCTRL2_SRCSEL_MSK, 
                                                                    DT_TXCTRL2_SRCSEL_SH);
}
static __inline void  DtaRegTxCtrl2SetSrcSel(volatile UInt8* pBase, UInt SrcSel) {
    WRITE_UINT_MASKED(SrcSel, pBase, DT_TX_REG_TXCTRL2, DT_TXCTRL2_SRCSEL_MSK,
                                                                    DT_TXCTRL2_SRCSEL_SH);
}

// Tx Control2: Transmit lock channel select
static __inline UInt  DtaRegTxCtrl2GetLockSel(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL2, DT_TXCTRL2_LOCKSEL_MSK, 
                                                                   DT_TXCTRL2_LOCKSEL_SH);
}
static __inline void  DtaRegTxCtrl2SetLockSel(volatile UInt8* pBase, UInt LockSel) {
    WRITE_UINT_MASKED(LockSel, pBase, DT_TX_REG_TXCTRL2, DT_TXCTRL2_LOCKSEL_MSK,
                                                                   DT_TXCTRL2_LOCKSEL_SH);
}


// Tx Control2: Transmit rate select
static __inline int  DtaRegTxCtrl2GetRateSelExt(volatile UInt8* pBase){
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL2, DT_TXCTRL2_RATESEL_MSK, 
                                                                   DT_TXCTRL2_RATESEL_SH);
}
static __inline void  DtaRegTxCtrl2SetRateSelExt(volatile UInt8* pBase) {
    WRITE_UINT_MASKED(1, pBase, DT_TX_REG_TXCTRL2, DT_TXCTRL2_RATESEL_MSK,
                                                                   DT_TXCTRL2_RATESEL_SH);
}

static __inline int  DtaRegTxCtrl2GetRateSelInt(volatile UInt8* pBase){
    return (READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL2, DT_TXCTRL2_RATESEL_MSK, 
                                                     DT_TXCTRL2_RATESEL_SH) == 0) ? 1 : 0;
}
static __inline void  DtaRegTxCtrl2SetRateSelInt(volatile UInt8* pBase) {
    WRITE_UINT_MASKED(0, pBase, DT_TX_REG_TXCTRL2, DT_TXCTRL2_RATESEL_MSK,
                                                                   DT_TXCTRL2_RATESEL_SH);
}

// Tx Control2: Data direct mode
static __inline UInt  DtaRegTxCtrl2GetDirectMode(volatile UInt8* pBase){
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL2, DT_TXCTRL2_DIRMODE_MSK, 
                                                                   DT_TXCTRL2_DIRMODE_SH);
}
static __inline void  DtaRegTxCtrl2SetDirectMode(volatile UInt8* pBase, UInt DirMode) {
    WRITE_UINT_MASKED(DirMode, pBase, DT_TX_REG_TXCTRL2, DT_TXCTRL2_DIRMODE_MSK,
                                                                   DT_TXCTRL2_DIRMODE_SH);
}

// Tx Control2: Transmit output ISI
static __inline UInt  DtaRegTxCtrl2GetS2ISI(volatile UInt8* pBase){
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL2, DT_TXCTRL2_S2ISI_MSK, 
                                                                     DT_TXCTRL2_S2ISI_SH);
}
static __inline void  DtaRegTxCtrl2SetS2ISI(volatile UInt8* pBase, UInt Isi){
    WRITE_UINT_MASKED(Isi, pBase, DT_TX_REG_TXCTRL2, DT_TXCTRL2_S2ISI_MSK,
                                                                  DT_TXCTRL2_S2ISI_SH);
}
// Tx Control2: Transmit output S2 mode
static __inline UInt  DtaRegTxCtrl2GetS2Loopmode(volatile UInt8* pBase){
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXCTRL2, DT_TXCTRL2_S2LOOPMODE_MSK, 
                                                                DT_TXCTRL2_S2LOOPMODE_SH);
}
static __inline void  DtaRegTxCtrl2SetS2Loopmode(volatile UInt8* pBase, UInt S2Loopmode){
    WRITE_UINT_MASKED(S2Loopmode, pBase, DT_TX_REG_TXCTRL2, DT_TXCTRL2_S2LOOPMODE_MSK,
                                                                DT_TXCTRL2_S2LOOPMODE_SH);
}


//-.-.-.-.-.-.-.-.-.-.-.-.- Tx Status register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// Tx Status: Register access
static __inline UInt  DtaRegTxStatGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_TXSTATUS);
}
static __inline void  DtaRegTxStatSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_TXSTATUS);
}

// Tx Status: FifoFilled
static __inline UInt  DtaRegTxStatGetFifoFilled(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXSTATUS, DT_TXSTAT_FIFOFILLED_MSK, 
                                                                 DT_TXSTAT_FIFOFILLED_SH);
}

// Tx Status: SdramSize
static __inline UInt  DtaRegTxStatGetSdramSize(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXSTATUS, DT_TXSTAT_SDRAMSIZE_MSK, 
                                                                   DT_TXSTAT_SDRAMSIZE_SH);
}

// Tx Status: periodic interrupt
static __inline UInt  DtaRegTxStatGetPerInt(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXSTATUS, DT_TXSTAT_PERINT_MSK, 
                                                                     DT_TXSTAT_PERINT_SH);
}
static __inline void  DtaRegTxStatClrPerInt(volatile UInt8* pBase) {
    WRITE_UINT(DT_TXSTAT_PERINT_MSK, pBase, DT_TX_REG_TXSTATUS);
}

// Tx Status: underflow interrupt
static __inline UInt  DtaRegTxStatGetUflInt(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXSTATUS, DT_TXSTAT_UFLINT_MSK, 
                                                                     DT_TXSTAT_UFLINT_SH);
}
static __inline void  DtaRegTxStatClrUflInt(volatile UInt8* pBase) {
    WRITE_UINT(DT_TXSTAT_UFLINT_MSK, pBase, DT_TX_REG_TXSTATUS);
}

// Tx Status: sync interrupt
static __inline UInt  DtaRegTxStatGetSyncInt(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXSTATUS, DT_TXSTAT_SYNCINT_MSK, 
                                                                    DT_TXSTAT_SYNCINT_SH);
}
static __inline void  DtaRegTxStatClrSyncInt(volatile UInt8* pBase) {
    WRITE_UINT(DT_TXSTAT_SYNCINT_MSK, pBase, DT_TX_REG_TXSTATUS);
}

// Tx Status: short-circuit interrupt
static __inline UInt  DtaRegTxStatGetShortInt(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXSTATUS, DT_TXSTAT_SHORTINT_MSK, 
                                                                   DT_TXSTAT_SHORTINT_SH);
}
static __inline void  DtaRegTxStatClrShortInt(volatile UInt8* pBase) {
    WRITE_UINT(DT_TXSTAT_SHORTINT_MSK, pBase, DT_TX_REG_TXSTATUS);
}

// Tx Status: threshold interrupt
static __inline UInt  DtaRegTxStatGetThresholdInt(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_TXSTATUS, DT_TXSTAT_THRINT_MSK, 
                                                                     DT_TXSTAT_THRINT_SH);
}
static __inline void  DtaRegTxStatClrThresholdInt(volatile UInt8* pBase) {
    WRITE_UINT(DT_TXSTAT_THRINT_MSK, pBase, DT_TX_REG_TXSTATUS);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Transmit Clock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

static __inline UInt  DtaRegTxClockGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_TXCLOCK);
}
static __inline void  DtaRegTxClockSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_TXCLOCK);
}

//.-.-.-.-.-.-.-.-.-.-..- Clock Count register: Access Functions .-.-.-.-.-.-.-.-.-.-.-.-.

static __inline UInt  DtaRegTxClkCountGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_CLOCKCNT);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- FIFO-Size register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// FIFO Size: Register access
static __inline UInt  DtaRegTxFifoSizeGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_FIFOSIZE);
}
static __inline void  DtaRegTxFifoSizeSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_FIFOSIZE);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- FIFO-Load register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// FIFO Load: Register access
static __inline UInt  DtaRegTxFifoLoadGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_FIFOLOAD);
}

//.-.-.-.-.-.-.-.-.-.-.-.- Diagnostics register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

// Diagnostics register: Register access
static __inline UInt  DtaRegTxDiagGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_DIAG);
}
static __inline void  DtaRegTxDiagSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_DIAG);
}

// Diagnostics register: LoopBack
static __inline UInt  DtaRegTxDiagGetLoopBack(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_DIAG, DT_TXDIAG_LOOPBACK_MSK, 
                                                                   DT_TXDIAG_LOOPBACK_SH);
}
static __inline void  DtaRegTxDiagSetLoopBack(volatile UInt8* pBase, UInt LoopBack) {
    WRITE_UINT_MASKED(LoopBack, pBase, DT_TX_REG_DIAG, DT_TXDIAG_LOOPBACK_MSK,
                                                                   DT_TXDIAG_LOOPBACK_SH);
}

// Diagnostics register: BfLoad
static __inline UInt  DtaRegTxDiagGetBfLoad(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_DIAG, DT_TXDIAG_BFLOAD_MSK, 
                                                                     DT_TXDIAG_BFLOAD_SH);
}

//.-.-.-.-.-.-.-.-.-.-.- Loop-Back Data register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-

// Loop-Back Data: Register access
static __inline UInt  DtaRegTxLoopBackDataGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_LOOPBKDATA);
}

//.-.-.-.-.-.-.-.-.-.-.- ThresHold Ctrl register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-

// ThresHold Ctrl register: Register access
static __inline UInt  DtaRegThresHCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_THRESHCTRL);
}
static __inline void  DtaRegThresHCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_THRESHCTRL);
}

// ThresHold Ctrl register: Update busy
static __inline UInt  DtaRegThresHCtrlGetUpdateBsy(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_THRESHCTRL, DT_THRESHCTRL_UPDBUSY_MSK, 
                                                                DT_THRESHCTRL_UPDBUSY_SH);
}

// ThresHold Ctrl register: Comparator A
static __inline UInt  DtaRegThresHCtrlGetCmpA(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_THRESHCTRL, DT_THRESHCTRL_CMPA_MSK, 
                                                                   DT_THRESHCTRL_CMPA_SH);
}

// ThresHold Ctrl register: Comparator B
static __inline UInt  DtaRegThresHCtrlGetCmpB(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_THRESHCTRL, DT_THRESHCTRL_CMPB_MSK, 
                                                                   DT_THRESHCTRL_CMPB_SH);
}

// ThresHold Ctrl register: Threshold comparator A
static __inline void  DtaRegThresHCtrlSetThresHoldA(volatile UInt8* pBase, UInt ThresA) {
    UInt  Val = READ_UINT(pBase, DT_TX_REG_THRESHCTRL);
    Val &= ~(DT_THRESHCTRL_AORB_MSK | DT_THRESHCTRL_THRESHA_MSK);
    Val |= (ThresA<<DT_THRESHCTRL_THRESHA_SH) & DT_THRESHCTRL_THRESHA_MSK;
    WRITE_UINT(Val, pBase, DT_TX_REG_THRESHCTRL);
}

// ThresHold Ctrl register: Threshold comparator B
static __inline void  DtaRegThresHCtrlSetThresHoldB(volatile UInt8* pBase, UInt ThresB) {
    UInt  Val = READ_UINT(pBase, DT_TX_REG_THRESHCTRL);
    Val &= ~(DT_THRESHCTRL_AORB_MSK | DT_THRESHCTRL_THRESHB_MSK);
    Val |= (ThresB<<DT_THRESHCTRL_THRESHB_SH) & DT_THRESHCTRL_THRESHB_MSK;
    WRITE_UINT(Val, pBase, DT_TX_REG_THRESHCTRL);
}

//.-.-.-.-.-.-.-.-.-.- Modulation-Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-
// Modulation-Control register: Register access
static __inline UInt  DtaRegModCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_MOD_CONTROL1);
}
static __inline void  DtaRegModCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_MOD_CONTROL1);
}

// Modulation-Control register: Channel-Filter Roll-Off Factor
static __inline UInt  DtaRegModCtrlGetRollOffFactor(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_ROLLOFF_MSK, 
                                                                   DT_TXMODC1_ROLLOFF_SH);
}
static __inline void  DtaRegModCtrlSetRollOffFactor(volatile UInt8* pBase, UInt RollOff) {
    WRITE_UINT_MASKED(RollOff, pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_ROLLOFF_MSK,
                                                                   DT_TXMODC1_ROLLOFF_SH);
}

// Modulation-Control register: Code Rate
static __inline UInt  DtaRegModCtrlGetCodeRate(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_CODERATE_MSK, 
                                                                  DT_TXMODC1_CODERATE_SH);
}
static __inline void  DtaRegModCtrlSetCodeRate(volatile UInt8* pBase, UInt CodeRate) {
    WRITE_UINT_MASKED(CodeRate, pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_CODERATE_MSK,
                                                                  DT_TXMODC1_CODERATE_SH);
}

// Modulation-Control register: Enable user level
static __inline void  DtaRegModCtrlSetUserLevel(volatile UInt8* pBase, UInt Ena) {
    WRITE_UINT_MASKED(Ena, pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_USER_LEVEL_MSK,
                                                                DT_TXMODC1_USER_LEVEL_SH);
}

// Modulation-Control register: Enable spectral inversion
static __inline void  DtaRegModCtrlSetSpectralInv(volatile UInt8* pBase, UInt Ena) {
    WRITE_UINT_MASKED(Ena, pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_SPEC_INV_MSK,
                                                                  DT_TXMODC1_SPEC_INV_SH);
}

// Modulation-Control register: Interpolation Selection
static __inline UInt  DtaRegModCtrlGetIntPolSel(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_INTPOL_MSK, 
                                                                    DT_TXMODC1_INTPOL_SH);
}
static __inline void  DtaRegModCtrlSetIntPolSel(volatile UInt8* pBase, UInt IntPolSel) {
    WRITE_UINT_MASKED(IntPolSel, pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_INTPOL_MSK,
                                                                    DT_TXMODC1_INTPOL_SH);
}

// Modulation-Control register: I/Q Mapping
static __inline UInt  DtaRegModCtrlGetIqMap(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_IQMAP_MSK, 
                                                                     DT_TXMODC1_IQMAP_SH);
}
static __inline void  DtaRegModCtrlSetIqMap(volatile UInt8* pBase, UInt IqMap) {
    WRITE_UINT_MASKED(IqMap, pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_IQMAP_MSK,
                                                                      DT_TXMODC1_IQMAP_SH);
}

// Modulation-Control register: Modulation Type
static __inline UInt  DtaRegModCtrlGetModType(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_MODTYPE_MSK, 
                                                                   DT_TXMODC1_MODTYPE_SH);
}
static __inline void  DtaRegModCtrlSetModType(volatile UInt8* pBase, UInt ModType) {
    WRITE_UINT_MASKED(ModType, pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_MODTYPE_MSK,
                                                                   DT_TXMODC1_MODTYPE_SH);
}

// Modulation-Control register: Multiplier M
static __inline UInt  DtaRegModCtrlGetM(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_M_MSK, 
                                                                         DT_TXMODC1_M_SH);
}
static __inline void  DtaRegModCtrlSetM(volatile UInt8* pBase, UInt M) {
    WRITE_UINT_MASKED(M, pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_M_MSK,
                                                                         DT_TXMODC1_M_SH);
}

// Modulation-Control register: Mute I output
static __inline void  DtaRegModCtrlMuteI(volatile UInt8* pBase, UInt Mute) {
    WRITE_UINT_MASKED(Mute, pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_MUTE_I_MSK,
                                                                    DT_TXMODC1_MUTE_I_SH);
}

// Modulation-Control register: Mute Q output
static __inline void  DtaRegModCtrlMuteQ(volatile UInt8* pBase, UInt Mute) {
    WRITE_UINT_MASKED(Mute, pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_MUTE_Q_MSK,
                                                                    DT_TXMODC1_MUTE_Q_SH);
}

// Modulation-Control register: Output level (scaler value)
static __inline UInt  DtaRegModCtrlGetOutputLevel(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_OUTLVL_MSK, 
                                                                     DT_TXMODC1_OUTLVL_SH);
}
static __inline void  DtaRegModCtrlSetOutputLevel(volatile UInt8* pBase, UInt Level) {
    WRITE_UINT_MASKED(Level, pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_OUTLVL_MSK,
                                                                     DT_TXMODC1_OUTLVL_SH);
}

// Modulation-Control register: Test Pattern
static __inline UInt  DtaRegModCtrlGetTestPat(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_TESTPAT_MSK, 
                                                                   DT_TXMODC1_TESTPAT_SH);
}
static __inline void  DtaRegModCtrlSetTestPat(volatile UInt8* pBase, UInt TestPat) {
    WRITE_UINT_MASKED(TestPat, pBase, DT_TX_REG_MOD_CONTROL1, DT_TXMODC1_TESTPAT_MSK,
                                                                   DT_TXMODC1_TESTPAT_SH);
}

//-.-.-.-.-.-.-.-.-.- Modulation-Control 2 register: Access Functions -.-.-.-.-.-.-.-.-.-.

// Modulation-Control register: Register access
static __inline UInt  DtaRegModCtrl2Get(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_MOD_CONTROL2);
}
static __inline void  DtaRegModCtrl2Set(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_MOD_CONTROL2);
}

// Modulation-Control 2 register: Output level offset (scaler value offset)
static __inline Int32  DtaRegModCtrl2GetOutputLevelOffset(volatile UInt8* pBase) {
    Int32  Val = (Int32)READ_UINT_MASKED(pBase, DT_TX_REG_MOD_CONTROL2, 
                               DT_TXMODC2_OUTLVL_OFFSET_MSK, DT_TXMODC2_OUTLVL_OFFSET_SH);
    // Sign extension
    if ((Val&0x8)!=0) 
        Val |= ~0x0000000F;

    return Val;
}
static __inline void  DtaRegModCtrl2SetOutputLevelOffset(volatile UInt8* pBase,
                                                                           Int32 Offset) {
    // Remove extra sign bits
    Offset &= 0x0000000F;
    WRITE_UINT_MASKED(Offset, pBase, DT_TX_REG_MOD_CONTROL2, DT_TXMODC2_OUTLVL_OFFSET_MSK,
                                                             DT_TXMODC2_OUTLVL_OFFSET_SH);
}

// Modulation-Control 2 register: Noise level
static __inline UInt  DtaRegModCtrl2GetNoiseLevel(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_MOD_CONTROL2, DT_TXMODC2_NOISELVL_MSK, 
                                                                  DT_TXMODC2_NOISELVL_SH);
}
static __inline void  DtaRegModCtrl2SetNoiseLevel(volatile UInt8* pBase, UInt Level) {
    WRITE_UINT_MASKED(Level, pBase, DT_TX_REG_MOD_CONTROL2, DT_TXMODC2_NOISELVL_MSK,
                                                                  DT_TXMODC2_NOISELVL_SH);
}

// Modulation-Control 2 register: Noise range
static __inline UInt32  DtaRegModCtrl2GetNoiseRange(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_MOD_CONTROL2, DT_TXMODC2_NOISERNG_MSK, 
                                                                  DT_TXMODC2_NOISERNG_SH);
}
static __inline void  DtaRegModCtrl2SetNoiseRange(volatile UInt8* pBase, Int32 Range) {
    WRITE_UINT_MASKED(Range, pBase, DT_TX_REG_MOD_CONTROL2, DT_TXMODC2_NOISERNG_MSK,
                                                                  DT_TXMODC2_NOISERNG_SH);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TX Clock mod: Register acces -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// TX Clock Modulus: Register access
static __inline UInt  DtaRegTxClockModGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_TXCLOCK_MOD);
}
static __inline void  DtaRegTxClockModSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_TXCLOCK_MOD);
}

//-.-.-.-.-.-.-.-.-.- I- and Q-DAC-Control registers: Access Functions -.-.-.-.-.-.-.-.-.-

// I- and Q-DAC-Control registers:: Register access
static __inline UInt  DtaRegTxIDacCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_IDAC_CONTROL);
}
static __inline UInt  DtaRegTxQDacCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_QDAC_CONTROL);
}
static __inline void  DtaRegTxIDacCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_IDAC_CONTROL);
}
static __inline void  DtaRegTxQDacCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_QDAC_CONTROL);
}

// I- and Q-DAC-Control registers: Fine Gain
static __inline UInt  DtaRegTxIDacCtrlGetFineGain(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_IDAC_CONTROL, DT_TXDACC_FINEGAIN_MSK, 
                                                                   DT_TXDACC_FINEGAIN_SH);
}
static __inline UInt  DtaRegTxQDacCtrlGetFineGain(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_QDAC_CONTROL, DT_TXDACC_FINEGAIN_MSK, 
                                                                   DT_TXDACC_FINEGAIN_SH);
}
static __inline void  DtaRegTxIDacCtrlSetFineGain(volatile UInt8* pBase, UInt FineGain) {
    WRITE_UINT_MASKED(FineGain, pBase, DT_TX_REG_IDAC_CONTROL, DT_TXDACC_FINEGAIN_MSK,
                                                                   DT_TXDACC_FINEGAIN_SH);
}
static __inline void  DtaRegTxQDacCtrlSetFineGain(volatile UInt8* pBase, UInt FineGain) {
    WRITE_UINT_MASKED(FineGain, pBase, DT_TX_REG_QDAC_CONTROL, DT_TXDACC_FINEGAIN_MSK,
                                                                   DT_TXDACC_FINEGAIN_SH);
}

// I- and Q-DAC-Control registers: Offset Direction
static __inline UInt  DtaRegTxIDacCtrlGetOffsDir(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_IDAC_CONTROL, DT_TXDACC_OFFSDIR_MSK, 
                                                                    DT_TXDACC_OFFSDIR_SH);
}
static __inline UInt  DtaRegTxQDacCtrlGetOffsDir(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_QDAC_CONTROL, DT_TXDACC_OFFSDIR_MSK, 
                                                                    DT_TXDACC_OFFSDIR_SH);
}
static __inline void  DtaRegTxIDacCtrlSetOffsDir(volatile UInt8* pBase, UInt OffsDir) {
    WRITE_UINT_MASKED(OffsDir, pBase, DT_TX_REG_IDAC_CONTROL, DT_TXDACC_OFFSDIR_MSK,
                                                                    DT_TXDACC_OFFSDIR_SH);
}
static __inline void  DtaRegTxQDacCtrlSetOffsDir(volatile UInt8* pBase, UInt OffsDir) {
    WRITE_UINT_MASKED(OffsDir, pBase, DT_TX_REG_QDAC_CONTROL, DT_TXDACC_OFFSDIR_MSK,
                                                                    DT_TXDACC_OFFSDIR_SH);
}

// I- and Q-DAC-Control registers: Offset
static __inline UInt  DtaRegTxIDacCtrlGetOffset(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_IDAC_CONTROL, DT_TXDACC_OFFSET_MSK, 
                                                                     DT_TXDACC_OFFSET_SH);
}
static __inline UInt  DtaRegTxQDacCtrlGetOffset(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_QDAC_CONTROL, DT_TXDACC_OFFSET_MSK, 
                                                                     DT_TXDACC_OFFSET_SH);
}
static __inline void  DtaRegTxIDacCtrlSetOffset(volatile UInt8* pBase, UInt Offset) {
    WRITE_UINT_MASKED(Offset, pBase, DT_TX_REG_IDAC_CONTROL, DT_TXDACC_OFFSET_MSK,
                                                                     DT_TXDACC_OFFSET_SH);
}
static __inline void  DtaRegTxQDacCtrlSetOffset(volatile UInt8* pBase, UInt Offset) {
    WRITE_UINT_MASKED(Offset, pBase, DT_TX_REG_QDAC_CONTROL, DT_TXDACC_OFFSET_MSK,
                                                                     DT_TXDACC_OFFSET_SH);
}

//.-.-.-.-.-.-.-.-.-.-.-.- RF-Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// RF-Control register: Register access
static __inline UInt  DtaRegTxRfCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_RF_CONTROL);
}
static __inline void  DtaRegTxRfCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_RF_CONTROL);
}

// RF-Control register: RF PLL A-Counter Modulus
static __inline UInt  DtaRegTxRfCtrlGetPllA(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL, DT_TXRFCTRL_PLLA_MSK, 
                                                                     DT_TXRFCTRL_PLLA_SH);
}
static __inline void  DtaRegTxRfCtrlSetPllA(volatile UInt8* pBase, UInt PllA) {
    WRITE_UINT_MASKED(PllA, pBase, DT_TX_REG_RF_CONTROL, DT_TXRFCTRL_PLLA_MSK,
                                                                     DT_TXRFCTRL_PLLA_SH);
}

// RF-Control register: RF PLL B-Counter Modulus
static __inline UInt  DtaRegTxRfCtrlGetPllB(volatile UInt8* pBase) {
    UInt  RegVal = READ_UINT(pBase, DT_TX_REG_RF_CONTROL);
    UInt  PllBbit8to0 = (RegVal & DT_TXRFCTRL_PLLB_MSK)  >> DT_TXRFCTRL_PLLB_SH;
    UInt  PllBbit9    = (RegVal & DT_TXRFCTRL_PLLB9_MSK) >> DT_TXRFCTRL_PLLB9_SH;
    return ((PllBbit9 << 9) + PllBbit8to0);
}
static __inline void  DtaRegTxRfCtrlSetPllB(volatile UInt8* pBase, UInt PllB) {
    UInt  RegVal = READ_UINT(pBase, DT_TX_REG_RF_CONTROL);
    UInt  PllBbit8to0 =  PllB & 0x1FF;
    UInt  PllBbit9    = (PllB>>9) & 1;
    RegVal &= ~(DT_TXRFCTRL_PLLB_MSK | DT_TXRFCTRL_PLLB9_MSK);
    RegVal |= PllBbit8to0<<DT_TXRFCTRL_PLLB_SH;
    RegVal |= PllBbit9<<DT_TXRFCTRL_PLLB9_SH;
    WRITE_UINT(RegVal, pBase, DT_TX_REG_RF_CONTROL);
}

// RF-Control register: RF PLL Reference-Counter Modulus
static __inline UInt  DtaRegTxRfCtrlGetPllR(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL, DT_TXRFCTRL_PLLR_MSK, 
                                                                     DT_TXRFCTRL_PLLR_SH);
}
static __inline void  DtaRegTxRfCtrlSetPllR(volatile UInt8* pBase, UInt PllR) {
    WRITE_UINT_MASKED(PllR, pBase, DT_TX_REG_RF_CONTROL, DT_TXRFCTRL_PLLR_MSK,
                                                                     DT_TXRFCTRL_PLLR_SH);
}

// RF-Control register: PLL Locked Status
static __inline UInt  DtaRegTxRfCtrlGetPllLock(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL, DT_TXRFCTRL_PLL_LOCK_MSK, 
                                                                 DT_TXRFCTRL_PLL_LOCK_SH);
}

//-.-.-.-.-.-.-.-.-.-.-.- RF-Control-1 register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.
//
// NOTE: this register has the same location as the RF-control register in
// the DTA-107 and DTA-110
//

// RF-Control register 1: Register access
static __inline UInt  DtaRegTxRfCtrl1Get(volatile UInt8* pBase) {
    return DtaRegTxRfCtrlGet(pBase);
}
static __inline void  DtaRegTxRfCtrl1Set(volatile UInt8* pBase, UInt Val) {
    DtaRegTxRfCtrlSet(pBase, Val);
}

// RF-Control-1 register: RF-PLL2 Modulus value
static __inline UInt  DtaRegTxRfCtrl1GetPllM(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL1, DT_TXRFCTRL1_MOD_MSK, 
                                                                     DT_TXRFCTRL1_MOD_SH);
}
static __inline void  DtaRegTxRfCtrl1SetPllM(volatile UInt8* pBase, UInt PllM) {
    WRITE_UINT_MASKED(PllM, pBase, DT_TX_REG_RF_CONTROL1, DT_TXRFCTRL1_MOD_MSK,
                                                                     DT_TXRFCTRL1_MOD_SH);
}

// RF-Control-1 register: RF-PLL2 Fractional value
static __inline UInt  DtaRegTxRfCtrl1GetPllF(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL1, DT_TXRFCTRL1_FRAC_MSK, 
                                                                    DT_TXRFCTRL1_FRAC_SH);
}
static __inline void  DtaRegTxRfCtrl1SetPllF(volatile UInt8* pBase, UInt PllF) {
    WRITE_UINT_MASKED(PllF, pBase, DT_TX_REG_RF_CONTROL1, DT_TXRFCTRL1_FRAC_MSK,
                                                                    DT_TXRFCTRL1_FRAC_SH);
}

// RF-Control-1 register: RF-PLL2 Integer value
static __inline UInt  DtaRegTxRfCtrl1GetPllI(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL1, DT_TXRFCTRL1_INT_MSK, 
                                                                     DT_TXRFCTRL1_INT_SH);
}
static __inline void  DtaRegTxRfCtrl1SetPllI(volatile UInt8* pBase, UInt PllI) {
    WRITE_UINT_MASKED(PllI, pBase, DT_TX_REG_RF_CONTROL1, DT_TXRFCTRL1_INT_MSK,
                                                                     DT_TXRFCTRL1_INT_SH);
}

//.-.-.-.-.-.-.-.-.-.-.-.- IF-Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// IF-Control register: Register access
static __inline UInt  DtaRegTxIfCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_IF_CONTROL);
}
static __inline void  DtaRegTxIfCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_IF_CONTROL);
}

// IF-Control register: IF PLL A-Counter Modulus
static __inline UInt  DtaRegTxIfCtrlGetPllA(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_IF_CONTROL, DT_TXIFCTRL_PLLA_MSK, 
                                                                     DT_TXIFCTRL_PLLA_SH);
}
static __inline void  DtaRegTxIfCtrlSetPllA(volatile UInt8* pBase, UInt PllA) {
    WRITE_UINT_MASKED(PllA, pBase, DT_TX_REG_IF_CONTROL, DT_TXIFCTRL_PLLA_MSK,
                                                                     DT_TXIFCTRL_PLLA_SH);
}

// IF-Control register: IF PLL B-Counter Modulus
static __inline UInt  DtaRegTxIfCtrlGetPllB(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_IF_CONTROL, DT_TXIFCTRL_PLLB_MSK, 
                                                                     DT_TXIFCTRL_PLLB_SH);
}
static __inline void  DtaRegTxIfCtrlSetPllB(volatile UInt8* pBase, UInt PllB) {
    WRITE_UINT_MASKED(PllB, pBase, DT_TX_REG_IF_CONTROL, DT_TXIFCTRL_PLLB_MSK,
                                                                     DT_TXIFCTRL_PLLB_SH);
}

// IF-Control register: IF PLL Reference-Counter Modulus
static __inline UInt  DtaRegTxIfCtrlGetPllR(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_IF_CONTROL, DT_TXIFCTRL_PLLR_MSK, 
                                                                     DT_TXIFCTRL_PLLR_SH);
}
static __inline void  DtaRegTxIfCtrlSetPllR(volatile UInt8* pBase, UInt PllR) {
    WRITE_UINT_MASKED(PllR, pBase, DT_TX_REG_IF_CONTROL, DT_TXIFCTRL_PLLR_MSK,
                                                                     DT_TXIFCTRL_PLLR_SH);
}

//-.-.-.-.-.-.-.-.-.-.-.- RF-Control-2 register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

// RF-Control-2 register: Register access
static __inline UInt  DtaRegTxRfCtrl2Get(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_RF_CONTROL2);
}
static __inline void  DtaRegTxRfCtrl2Set(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_RF_CONTROL2);
}

// RF-Control-2 register: AGC setpoint
static __inline UInt  DtaRegTxRfCtrl2GetAgcSetPoint(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL2, DT_TXRFCTRL2_AGCSP_MSK, 
                                                                   DT_TXRFCTRL2_AGCSP_SH);
}

// RF-Control-2 register: RF Output Attenuator
static __inline UInt  DtaRegTxRfCtrl2GetOutpAttn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL2, DT_TXRFCTRL2_OUTATTN_MSK, 
                                                                 DT_TXRFCTRL2_OUTATTN_SH);
}
static __inline void  DtaRegTxRfCtrl2SetOutpAttn(volatile UInt8* pBase, UInt Attn) {
    WRITE_UINT_MASKED(Attn, pBase, DT_TX_REG_RF_CONTROL2, DT_TXRFCTRL2_OUTATTN_MSK,
                                                                 DT_TXRFCTRL2_OUTATTN_SH);
}

// RF-Control 2 register: 10-bit output level 
static __inline Int32  DtaRegTxRfCtrl2GetOutputLevel10bOffset(volatile UInt8* pBase) {
    Int32  Val = (Int32)READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL2, 
                     DT_TXRFCTRL2_OUTLVL10B_OFFSET_MSK, DT_TXRFCTRL2_OUTLVL10B_OFFSET_SH);
    // Sign extension
    if ((Val&0x200)!=0) 
        Val |= ~0x000003FF;
    return Val;
}
static __inline void  DtaRegTxRfCtrl2SetOutputLevel10bOffset(volatile UInt8* pBase, 
                                                                           Int32 Offset) {
    // Remove sign extension
    Offset &= 0x000003FF;
    WRITE_UINT_MASKED(Offset, pBase, DT_TX_REG_RF_CONTROL2,
                     DT_TXRFCTRL2_OUTLVL10B_OFFSET_MSK, DT_TXRFCTRL2_OUTLVL10B_OFFSET_SH);
}

// RF-Control-2 register: AGC Enable
static __inline UInt  DtaRegTxRfCtrl2GetAgcEn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL2, DT_TXRFCTRL2_AGC_EN_MSK, 
                                                                  DT_TXRFCTRL2_AGC_EN_SH);
}
static __inline void  DtaRegTxRfCtrl2SetAgcEn(volatile UInt8* pBase, UInt AgcEnable) {
    WRITE_UINT_MASKED(AgcEnable, pBase, DT_TX_REG_RF_CONTROL2, DT_TXRFCTRL2_AGC_EN_MSK, 
                                                                  DT_TXRFCTRL2_AGC_EN_SH);
}

// RF-Control-2 register: RF Input Attenuator
static __inline UInt  DtaRegTxRfCtrl2GetInpAttn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL2, DT_TXRFCTRL2_INPATTN_MSK, 
                                                                 DT_TXRFCTRL2_INPATTN_SH);
}
static __inline void  DtaRegTxRfCtrl2SetInpAttn(volatile UInt8* pBase, UInt Attn) {
    WRITE_UINT_MASKED(Attn, pBase, DT_TX_REG_RF_CONTROL2, DT_TXRFCTRL2_INPATTN_MSK, 
                                                                 DT_TXRFCTRL2_INPATTN_SH);
}

// RF-Control-2 register: Low Phase Noise Enable
static __inline UInt  DtaRegTxRfCtrl2GetLowPhaseNoiseEn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL2, DT_TXRFCTRL2_LOWNOISE_EN_MSK, 
                                                             DT_TXRFCTRL2_LOWNOISE_EN_SH);
}
static __inline void  DtaRegTxRfCtrl2SetLowPhaseNoiseEn(volatile UInt8* pBase, UInt Ena) {
    WRITE_UINT_MASKED(Ena, pBase, DT_TX_REG_RF_CONTROL2, DT_TXRFCTRL2_LOWNOISE_EN_MSK, 
                                                             DT_TXRFCTRL2_LOWNOISE_EN_SH);
}

// RF-Control-2 register: Charge-pump current
static __inline UInt  DtaRegTxRfCtrl2GetChargePumpCur(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL2, DT_TXRFCTRL2_CPC_MSK, 
                                                                     DT_TXRFCTRL2_CPC_SH);
}
static __inline void  DtaRegTxRfCtrl2SetChargePumpCur(volatile UInt8* pBase, UInt Cur) {
    WRITE_UINT_MASKED(Cur, pBase, DT_TX_REG_RF_CONTROL2, DT_TXRFCTRL2_CPC_MSK, 
                                                                     DT_TXRFCTRL2_CPC_SH);
}

// RF-Control-2 register: PLL1 Locked Status
static __inline UInt  DtaRegTxRfCtrl2GetPll1Lock(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL2, DT_TXRFCTRL2_PLL1_LOCK_MSK, 
                                                               DT_TXRFCTRL2_PLL1_LOCK_SH);
}

// RF-Control-2 register: PLL2 Locked Status
static __inline UInt  DtaRegTxRfCtrl2GetPll2Lock(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL2, DT_TXRFCTRL2_PLL2_LOCK_MSK, 
                                                               DT_TXRFCTRL2_PLL2_LOCK_SH);
}

// RF-Control-2 register: PLL3 Locked Status
static __inline UInt  DtaRegTxRfCtrl2GetPll3Lock(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL2, DT_TXRFCTRL2_PLL3_LOCK_MSK, 
                                                               DT_TXRFCTRL2_PLL3_LOCK_SH);
}

// RF-Control-2 register: AGC Locked Status
static __inline UInt  DtaRegTxRfCtrl2GetAgcLock(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL2, DT_TXRFCTRL2_AGC_LOCK_MSK, 
                                                                DT_TXRFCTRL2_AGC_LOCK_SH);
}

//-.-.-.-.-.-.-.-.-.-.-.- RF-Control-3 register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

// RF-Control-3 register: Register access
static __inline UInt  DtaRegTxRfCtrl3Get(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_RF_CONTROL3);
}
static __inline void  DtaRegTxRfCtrl3Set(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_RF_CONTROL3);
}

// RF-Control-3 register: RF PLL reference clock selection 1: Int; 0 Ext
static __inline UInt  DtaRegTxRfCtrl3GetRfClkSel(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_TX_REG_RF_CONTROL3, DT_TXRFCTRL3_RFCLKSEL_MSK, 
                                                                DT_TXRFCTRL3_RFCLKSEL_SH);
}
static __inline void  DtaRegTxRfCtrl3SetRfClkSel(volatile UInt8* pBase, UInt RfClkSel) {
    WRITE_UINT_MASKED(RfClkSel, pBase, DT_TX_REG_RF_CONTROL3, DT_TXRFCTRL3_RFCLKSEL_MSK, 
                                                                DT_TXRFCTRL3_RFCLKSEL_SH);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- FIFO Data register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// FIFO Data: Register access
static __inline void  DtaRegTxFifoDataSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_FIFO_FIRST);
}
static __inline void  DtaRegTxFifoDataSetIdx(volatile UInt8* pBase, Int Idx, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_FIFO_FIRST + Idx*sizeof(UInt32));
}

//.-.-.-.-.-.-.-.-.-.-.- GS2962 Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  DtaRegTxGS2962CtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_TX_REG_GS2962CTRL);
}
static __inline void DtaRegTxGS2962CtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_TX_REG_GS2962CTRL);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- Slice Information: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.
static __inline UInt  DtaRegTxSliceStatGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_TX_REG_SLICE_STATUS);
}
static __inline UInt  DtaRegTxSlicePointerGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_TX_REG_SLICE_POINTER);
}
static __inline UInt  DtaRegTxSliceSizeGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DAT_TX_REG_SLICE_SIZE);
}

//-.-.-.-.-.-.-.-.-.-.- RFDAC SPI control register: Access Functions -.-.-.-.-.-.-.-.-.-.-
static __inline UInt  DtaRegTxRfdacCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_RFDAC_CONTROL);
}
static __inline void DtaRegTxRfdacCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_RFDAC_CONTROL);
}

//.-.-.-.-.-.-.-.-.-.-.- RFDAC SPI data register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  DtaRegTxRfdacDataGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_TX_REG_RFDAC_DATA);
}
static __inline void DtaRegTxRfdacDataSet(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DT_TX_REG_RFDAC_DATA);
}

//.-.-.-.-.-.-.-.-.-.-.-.- GS2962 Stat register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.
static __inline UInt  DtaRegTxGS2962StatGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_TX_REG_GS2962STAT);
}
static __inline void DtaRegTxGS2962StatSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_TX_REG_GS2962STAT);
}

//.-.-.-.-.-.-.-.-.-.-.-.- GS2962 SPI register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  DtaRegTxGS2962SpiGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_TX_REG_GS2962SPI);
}
static __inline void DtaRegTxGS2962SpiSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_TX_REG_GS2962SPI);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ RX Registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2162 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA_RX_REG_SLICE_STATUS    0x0044
#define DTA_RX_REG_SLICE_POINTER   0x0048
#define DTA_RX_REG_SLICE_SIZE      0x004C

//.-.-.-.-.-.-.-.-.-.-.-.- Rx Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// Rx Control: Register access
static __inline UInt  DtaRegRxCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_RX_REG_RXCTRL);
}
static __inline void DtaRegRxCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_RX_REG_RXCTRL);
}

// Rx Control: RxMode
static __inline UInt  DtaRegRxCtrlGetRxMode(volatile UInt8* pBase) {
    UInt  Val = DtaRegRxCtrlGet(pBase);
    UInt  RetVal = ((Val&DT_RXCTRL_RXMODE_MSK)>>DT_RXCTRL_RXMODE_SH);
    // Get 3rd bit
    RetVal |= ((Val&DT_RXCTRL_RXMODE_EXT_MSK)>>DT_RXCTRL_RXMODE_EXT_SH)<<2;
    return RetVal;
}
static __inline void  DtaRegRxCtrlSetRxMode(volatile UInt8* pBase, UInt RxMode) {
    UInt  Val = DtaRegRxCtrlGet(pBase);
    Val &= ~(DT_RXCTRL_RXMODE_MSK | DT_RXCTRL_RXMODE_EXT_MSK);
    Val |= ((RxMode<<DT_RXCTRL_RXMODE_SH) & DT_RXCTRL_RXMODE_MSK);
    // Set 3rd bit
    Val |= (((RxMode>>2)<<DT_RXCTRL_RXMODE_EXT_SH) & DT_RXCTRL_RXMODE_EXT_MSK);
    DtaRegRxCtrlSet(pBase, Val);
}

// Rx Control: AsiInv
static __inline UInt  DtaRegRxCtrlGetAsiInv(volatile UInt8* pBase) {
   return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_ASIINV_MSK, 
                                                                     DT_RXCTRL_ASIINV_SH);
}
static __inline void  DtaRegRxCtrlSetAsiInv(volatile UInt8* pBase, UInt AsiInv) {
    WRITE_UINT_MASKED(AsiInv, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_ASIINV_MSK, 
                                                                     DT_RXCTRL_ASIINV_SH);
}

// Rx Control: Equalise
static __inline UInt  DtaRegRxCtrlGetEqualise(volatile UInt8* pBase) {
   return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_EQUALISE_MSK, 
                                                                   DT_RXCTRL_EQUALISE_SH);
}
static __inline void  DtaRegRxCtrlSetEqualise(volatile UInt8* pBase, UInt Equalise) {
    WRITE_UINT_MASKED(Equalise, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_EQUALISE_MSK, 
                                                                   DT_RXCTRL_EQUALISE_SH);
}

// Rx Control: RxCtrl
static __inline UInt  DtaRegRxCtrlGetRxCtrl(volatile UInt8* pBase) {
   return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_RXCTRL_MSK, 
                                                                     DT_RXCTRL_RXCTRL_SH);
}
static __inline void  DtaRegRxCtrlSetRxCtrl(volatile UInt8* pBase, UInt RxCtrl) {
    WRITE_UINT_MASKED(RxCtrl, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_RXCTRL_MSK, 
                                                                     DT_RXCTRL_RXCTRL_SH);
}

// Rx Control: AntPwr (e.g. DTA-2135)
static __inline UInt  DtaRegRxCtrlGetAntPwr(volatile UInt8* pBase) {
   return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_ANTPWR_MSK, 
                                                                     DT_RXCTRL_ANTPWR_SH);
}
static __inline void  DtaRegRxCtrlSetAntPwr(volatile UInt8* pBase, UInt EnaPwr) {
    WRITE_UINT_MASKED(EnaPwr, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_ANTPWR_MSK, 
                                                                     DT_RXCTRL_ANTPWR_SH);
}

// Adc Control: AdcClock (e.g. DTA-2135)
// Note: The Adc control registers are highly compatible with Rx Control registers.
//       Only pBase is usually directed to the ADC interface base
static __inline void  DtaRegRxCtrlSetAdcClock(volatile UInt8* pBase, UInt AdcClock) {
    WRITE_UINT_MASKED(AdcClock, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_ADC_CLOCK_MSK, 
                                                                  DT_RXCTRL_ADC_CLOCK_SH);
}

// Rx Control: RxTimeStamp
static __inline UInt  DtaRegRxCtrlGetTimeStamping32(volatile UInt8* pBase) {
   return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_TIMESTAMP32_MSK, 
                                                                DT_RXCTRL_TIMESTAMP32_SH);
}
static __inline UInt  DtaRegRxCtrlGetTimeStamping64(volatile UInt8* pBase) {
   return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_TIMESTAMP64_MSK, 
                                                                DT_RXCTRL_TIMESTAMP64_SH);
}
static __inline void  DtaRegRxCtrlSetTimeStamping32(volatile UInt8* pBase, 
                                                                       UInt RxTimeStamp) {
    WRITE_UINT_MASKED(RxTimeStamp, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_TIMESTAMP32_MSK, 
                                                                DT_RXCTRL_TIMESTAMP32_SH);
}
static __inline void  DtaRegRxCtrlSetTimeStamping64(volatile UInt8* pBase, 
                                                                       UInt RxTimeStamp) {
    WRITE_UINT_MASKED(RxTimeStamp, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_TIMESTAMP32_MSK, 
                                                                DT_RXCTRL_TIMESTAMP64_SH);
}

// Rx Control: PerIntEn
static __inline UInt  DtaRegRxCtrlGetRxPerIntEn(volatile UInt8* pBase) {
   return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_PERINT_EN_MSK, 
                                                                  DT_RXCTRL_PERINT_EN_SH);
}
static __inline void  DtaRegRxCtrlSetRxPerIntEn(volatile UInt8* pBase, UInt RxPerIntEn) {
    WRITE_UINT_MASKED(RxPerIntEn, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_PERINT_EN_MSK, 
                                                                  DT_RXCTRL_PERINT_EN_SH);
}

// Rx Control: OvfIntEn
static __inline UInt  DtaRegRxCtrlGetOvfIntEn(volatile UInt8* pBase) {
   return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_OVFINT_EN_MSK, 
                                                                  DT_RXCTRL_OVFINT_EN_SH);
}
static __inline void  DtaRegRxCtrlSetOvfIntEn(volatile UInt8* pBase, UInt OvfIntEn) {
    WRITE_UINT_MASKED(OvfIntEn, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_OVFINT_EN_MSK, 
                                                                  DT_RXCTRL_OVFINT_EN_SH);
}

// Rx Control: SyncIntEn
static __inline UInt  DtaRegRxCtrlGetSyncIntEn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_SYNCINT_EN_MSK, 
                                                                 DT_RXCTRL_SYNCINT_EN_SH);
}
static __inline void  DtaRegRxCtrlSetSyncIntEn(volatile UInt8* pBase, UInt SyncIntEn) {
    WRITE_UINT_MASKED(SyncIntEn, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_SYNCINT_EN_MSK, 
                                                                 DT_RXCTRL_SYNCINT_EN_SH);
}

// Rx Control: EnaPwr (e.g. DTA-122)
static __inline UInt  DtaRegRxCtrlGetEnaPwr(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_ENAPWR_MSK, 
                                                                     DT_RXCTRL_ENAPWR_SH);
}
static __inline void  DtaRegRxCtrlSetEnaPwr(volatile UInt8* pBase, UInt EnaPwr) {
    WRITE_UINT_MASKED(EnaPwr, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_ENAPWR_MSK, 
                                                                     DT_RXCTRL_ENAPWR_SH);
}

// Rx Control: LedControl
static __inline UInt  DtaRegRxCtrlGetLedControl(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_LED_CTRL_MSK, 
                                                                   DT_RXCTRL_LED_CTRL_SH);
}
static __inline void  DtaRegRxCtrlSetLedControl(volatile UInt8* pBase, UInt LedControl) {
    WRITE_UINT_MASKED(LedControl, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_LED_CTRL_MSK, 
                                                                   DT_RXCTRL_LED_CTRL_SH);
}

// Rx Control: LedGreen
static __inline UInt  DtaRegRxCtrlGetLedGreen(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_LED_GREEN_MSK, 
                                                                  DT_RXCTRL_LED_GREEN_SH);
}
static __inline void  DtaRegRxCtrlSetLedGreen(volatile UInt8* pBase, UInt LedGreen) {
    WRITE_UINT_MASKED(LedGreen, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_LED_GREEN_MSK, 
                                                                  DT_RXCTRL_LED_GREEN_SH);
}

// Rx Control: LedRed
static __inline UInt  DtaRegRxCtrlGetLedRed(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_LED_RED_MSK, 
                                                                    DT_RXCTRL_LED_RED_SH);
}
static __inline void  DtaRegRxCtrlSetLedRed(volatile UInt8* pBase, UInt LedRed) {
    WRITE_UINT_MASKED(LedRed, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_LED_RED_MSK, 
                                                                    DT_RXCTRL_LED_RED_SH);
}

// Rx Control: Clear-FIFO
static __inline void  DtaRegRxCtrlClrFifo(volatile UInt8* pBase) {
    WRITE_UINT_MASKED(1, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_FIFO_CLR_MSK, 
                                                                   DT_RXCTRL_FIFO_CLR_SH);
}

// Rx Control: Reset
static __inline void  DtaRegRxCtrlRxReset(volatile UInt8* pBase) {
    WRITE_UINT_MASKED(1, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_RXRESET_MSK, 
                                                                    DT_RXCTRL_RXRESET_SH);
}

// Rx Control: SDI mode
static __inline UInt  DtaRegRxCtrlGetSdiMode(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_SDIMODE_MSK, 
                                                                    DT_RXCTRL_SDIMODE_SH);
}
static __inline void  DtaRegRxCtrlSetSdiMode(volatile UInt8* pBase, UInt SdiMode) {
    WRITE_UINT_MASKED(SdiMode, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_SDIMODE_MSK, 
                                                                    DT_RXCTRL_SDIMODE_SH);
}

// Rx Control: SDI 10-Bit mode
static __inline UInt DtaRegRxCtrlGetSdiMode10Bit(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_SDI10BIT_MSK, 
                                                                   DT_RXCTRL_SDI10BIT_SH);
}
static __inline void  DtaRegRxCtrlSetSdiMode10Bit(volatile UInt8* pBase, UInt Sdi10Bit) {
    WRITE_UINT_MASKED(Sdi10Bit, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_SDI10BIT_MSK, 
                                                                   DT_RXCTRL_SDI10BIT_SH);
}

// Rx Control: SDI compression mode enable
static __inline UInt DtaRegRxCtrlGetSdiComprModeEn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_SDIHUFF_MSK, 
                                                                    DT_RXCTRL_SDIHUFF_SH);
}
static __inline void  DtaRegRxCtrlSetSdiComprModeEn(volatile UInt8* pBase, UInt CompEna) {
    WRITE_UINT_MASKED(CompEna, pBase, DT_RX_REG_RXCTRL, DT_RXCTRL_SDIHUFF_MSK, 
                                                                    DT_RXCTRL_SDIHUFF_SH);
}


//-.-.-.-.-.-.-.-.-.-.-.-.- Rx Status register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// Rx Status: Register access
static __inline UInt  DtaRegRxStatGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_RX_REG_RXSTATUS);
}
static __inline void  DtaRegRxStatSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_RX_REG_RXSTATUS);
}

// Rx Status: PckSize
static __inline UInt  DtaRegRxStatGetPckSize(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXSTATUS, DT_RXSTAT_PCKSIZE_MSK, 
                                                                    DT_RXSTAT_PCKSIZE_SH);
}

// Rx Status: NumInv
static __inline UInt  DtaRegRxStatGetNumInv(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXSTATUS, DT_RXSTAT_NUMINV_MSK, 
                                                                     DT_RXSTAT_NUMINV_SH);
}

// Rx Status: SdramSize
static __inline UInt  DtaRegRxStatGetSdramSize(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXSTATUS, DT_RXSTAT_SDRAMSIZE_MSK, 
                                                                  DT_RXSTAT_SDRAMSIZE_SH);
}

// Rx Status: periodic interrupt
static __inline UInt  DtaRegRxStatGetPerInt(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXSTATUS, DT_RXSTAT_PERINT_MSK, 
                                                                     DT_RXSTAT_PERINT_SH);
}
static __inline void  DtaRegRxStatClrPerInt(volatile UInt8* pBase) {
    WRITE_UINT(DT_RXSTAT_PERINT_MSK, pBase, DT_RX_REG_RXSTATUS);
}

// Rx Status: overflow interrupt
static __inline UInt  DtaRegRxStatGetOvfInt(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXSTATUS, DT_RXSTAT_OVFINT_MSK, 
                                                                    DT_RXSTAT_OVFINT_SH);
}
static __inline void  DtaRegRxStatClrOvfInt(volatile UInt8* pBase) {
    WRITE_UINT(DT_RXSTAT_OVFINT_MSK, pBase, DT_RX_REG_RXSTATUS);
}

// Rx Status: sync interrupt
static __inline UInt  DtaRegRxStatGetSyncInt(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXSTATUS, DT_RXSTAT_SYNCINT_MSK, 
                                                                    DT_RXSTAT_SYNCINT_SH);
}
static __inline void  DtaRegRxStatClrSyncInt(volatile UInt8* pBase) {
    WRITE_UINT(DT_RXSTAT_SYNCINT_MSK, pBase, DT_RX_REG_RXSTATUS);
}

// Rx Status: threshold interrupt
static __inline UInt  DtaRegRxStatGetThresholdInt(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXSTATUS, DT_RXSTAT_THRINT_MSK, 
                                                                     DT_RXSTAT_THRINT_SH);
}
static __inline void  DtaRegRxStatClrThresholdInt(volatile UInt8* pBase) {
    WRITE_UINT(DT_RXSTAT_THRINT_MSK, pBase, DT_RX_REG_RXSTATUS);
}

// Rx Status: rate overflow interrupt
static __inline UInt  DtaRegRxStatGetRateOvfInt(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXSTATUS, DT_RXSTAT_RATEOVFINT_MSK, 
                                                                 DT_RXSTAT_RATEOVFINT_SH);
}
static __inline void  DtaRegRxStatClrRateOvfInt(volatile UInt8* pBase) {
    WRITE_UINT(DT_RXSTAT_RATEOVFINT_MSK, pBase, DT_RX_REG_RXSTATUS);
}

// Rx Status: ClkDet
static __inline UInt  DtaRegRxStatGetClkDet(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXSTATUS, DT_RXSTAT_CLKDET_MSK, 
                                                                     DT_RXSTAT_CLKDET_SH);
}

// Rx Status: AsiLock
static __inline UInt  DtaRegRxStatGetAsiLock(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXSTATUS, DT_RXSTAT_ASILOCK_MSK, 
                                                                    DT_RXSTAT_ASILOCK_SH);
}

// Rx Status: RateOk
static __inline UInt  DtaRegRxStatGetRateOk(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXSTATUS, DT_RXSTAT_RATEOK_MSK, 
                                                                     DT_RXSTAT_RATEOK_SH);
}

// Rx Status: AsiInv
static __inline UInt  DtaRegRxStatGetAsiInv(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_RXSTATUS, DT_RXSTAT_ASIINV_MSK, 
                                                                     DT_RXSTAT_ASIINV_SH);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- Rx SDI Position - Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.
static __inline UInt  DtaRegRxSdiPosGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_RX_REG_SDIPOS);
}

// Get Receive SDI Position
// Return value: framecount; Line and Sample count are returned through pointer argument
static __inline UInt  DtaRegRxSdiPosGetLineSample(volatile UInt8* pBase, UInt* pLineCnt, 
                                                                           UInt* pSampCnt) 
{
    UInt  RegVal = READ_UINT(pBase, DT_RX_REG_SDIPOS);
    *pSampCnt = (RegVal & DT_RXSDIPOS_SAMP_MSK)  >> DT_RXSDIPOS_SAMP_SH;
    *pLineCnt = (RegVal & DT_RXSDIPOS_LINE_MSK)  >> DT_RXSDIPOS_LINE_SH;
    return      (RegVal & DT_RXSDIPOS_FRAME_MSK) >> DT_RXSDIPOS_FRAME_SH;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Rx FIFO Load - Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

// FIFO Load: Register access
static __inline UInt  DtaRegRxFifoLoadGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_RX_REG_FIFOLOAD);
}

//.-.-.-.-.-.-.-.-.-.-.- Rx Diagnostics register - Access Functions -.-.-.-.-.-.-.-.-.-.-.

// Rx Diagnostics: Register access
static __inline UInt  DtaRegRxDiagGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_RX_REG_DIAG);
}
static __inline void DtaRegRxDiagSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_RX_REG_DIAG);
}

// Rx Diagnostics: LoopBack
static __inline UInt  DtaRegRxDiagGetLoopBack(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_DIAG, DT_RXDIAG_LOOPBACK_MSK, 
                                                                   DT_RXDIAG_LOOPBACK_SH);
}
static __inline void  DtaRegRxDiagSetLoopBack(volatile UInt8* pBase, UInt LoopBack) {
    WRITE_UINT_MASKED(LoopBack, pBase, DT_RX_REG_DIAG, DT_RXDIAG_LOOPBACK_MSK, 
                                                                   DT_RXDIAG_LOOPBACK_SH);
}

// Rx Diagnostics: AutoMemTest
static __inline UInt  DtaRegRxDiagAutoMemTestGet(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RX_REG_DIAG, DT_RXDIAG_AUTOMEMTEST_MSK,
                                                                DT_RXDIAG_AUTOMEMTEST_SH);
}

static __inline void  DtaRegRxDiagAutoMemTestSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT_MASKED(Val, pBase, DT_RX_REG_DIAG, DT_RXDIAG_AUTOMEMTEST_MSK,
                                                                DT_RXDIAG_AUTOMEMTEST_SH);
}

//.-.-.-.-.-.-.-.-.-.-.- Loop-Back Data register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-

// Loop-Back Data: Register access
static __inline void DtaRegRxLoopBackDataSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_RX_REG_LOOPBKDATA);
}

//.-.-.-.-.-.-.-.-.-.-.-.- Valid Count register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

static __inline UInt  DtaRegRxValidCountGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_RX_REG_VALIDCOUNT);
}

//.-.-.-.-.-.-.-.-.-.-.- Violation Count register: Access Functions -.-.-.-.-.-.-.-.-.-.-.

static __inline UInt  DtaRegRxViolCountGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_RX_REG_VIOLCOUNT);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- FIFO Data register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// FIFO Data: Register access
static __inline UInt  DtaRegRxFifoDataGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_RX_REG_FIFO_FIRST);
}
static __inline UInt  DtaRegRxFifoDataIdxGet(volatile UInt8* pBase, Int Idx) {
    return READ_UINT(pBase, DT_RX_REG_FIFO_FIRST + Idx*sizeof(UInt32));
}

//-.-.-.-.-.-.- -.-.-.-.-.-.-.-.-.-.-.-.-.-DTA-2148-.-.-.-.-.-.-.-.-.-.-.-.- -.-.-.-.-.-.-

//.-.-.-.-.-.-.-.-.-.-.- GS2984 Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  DtaRegRxGS2984CtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_RX_REG_GS2984CTRL);
}
static __inline void DtaRegRxGS2984CtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_RX_REG_GS2984CTRL);
}

//-.-.-.-.-.-.-.-.-.-.-.- GS2984 Status register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  DtaRegRxGS2984StatGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_RX_REG_GS2984STAT);
}
static __inline void DtaRegRxGS2984StatSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_RX_REG_GS2984STAT);
}

//.-.-.-.-.-.-.-.-.-.-.- GS2960 Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-

// Registers
#define DTA_GS2980_DATA_FORMAT_DS1 0x06
#define DTA_GS2980_DATA_FORMAT_DS2 0x07
#define DTA_GS2960_RASTER_STRUC_4  0x22

// Bitfields
#define DTA_GS2980_VD_STD_DS1      0x3F00
#define DTA_GS2980_VD_STD_DS2      0x3F00

// Values
#define DTA_GS2980_VDSTD_UNKNOWN   0x1D


static __inline UInt  DtaRegRxGS2960CtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_RX_REG_GS2960CTRL);
}
static __inline void DtaRegRxGS2960CtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_RX_REG_GS2960CTRL);
}

//.-.-.-.-.-.-.-.-.-.-.-.- GS2960 Stat register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.
static __inline UInt  DtaRegRxGS2960StatGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_RX_REG_GS2960STAT);
}
static __inline void DtaRegRxGS2960StatSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_RX_REG_GS2960STAT);
}

//.-.-.-.-.-.-.-.-.-.-.-.- GS2960 SPI register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  DtaRegRxGS2960SPIGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DT_RX_REG_GS2960SPI);
}
static __inline void DtaRegRxGS2960SPISet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DT_RX_REG_GS2960SPI);
}

static __inline UInt DtaRegRxGS2960InternalRead(volatile UInt8* pBase, UInt Reg) {

    UInt  Cmd;
    UInt  Result;

    Cmd = (1<<31) + (Reg << 16);        // programm read command
        // Bit 31: R/W bit (Read = 1 , Write = 0)
        // Bit 30: Reserved
        // Bit 29: Reserved
        // Bit 28: AutoInc
        // Bit 27 downto 16: Register address
        // Bit 15 downto 0: Data (N/A for reads)
    
    DtaRegRxGS2960SPISet(pBase, Cmd);
    Result = (1<<31);
    while ((Result & (1<<31)) != 0)
        Result = DtaRegRxGS2960SPIGet(pBase);

    return(Result);
}

static __inline void DtaRegRxGS2960InternalWrite(volatile UInt8* pBase, UInt Reg,
                                                                               UInt Val) {
    UInt  Cmd;
    UInt  Result;
    
    Cmd = (Reg << 16) + (Val & 0xFFFF);     // programm write command
        // Bit 31: R/W bit (Read = 1 , Write = 0)
        // Bit 30: Reserved
        // Bit 29: Reserved
        // Bit 28: AutoInc
        // Bit 27 downto 16: Register address
        // Bit 15 downto 0: Data (N/A for reads)
    
    DtaRegRxGS2960SPISet(pBase, Cmd);
    Result = (1<<31);
    while ((Result & (1<<31)) != 0)
        Result = DtaRegRxGS2960SPIGet(pBase);

    return;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- Slice Information: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.
static __inline UInt  DtaRegRxSliceStatGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_RX_REG_SLICE_STATUS);
}
static __inline UInt  DtaRegRxSlicePointerGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_RX_REG_SLICE_POINTER);
}
static __inline UInt  DtaRegRxSliceSizeGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_RX_REG_SLICE_SIZE);
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DMA Registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA_DMA_DESCPTR_REG            0x004
#define DTA_DMA_CMDSTAT_REG            0x008
#define DTA_DMA_PCIDADDR_REG           0x00C
#define DTA_DMA_DESCR_PREFETCH_REG     0x010
#define DTA_DMA_TIMEOUT_REG            0x014
#define DTA_DMA_TOTTR_CNT_REG          0x034

//-.-.-.-.-.-.-.-.-.-.-.-.- DMA Register: Timeout configuration -.-.-.-.-.-.-.-.-.-.-.-.-.
static __inline void  DtaDmaTimeOutSet(volatile UInt8* pBase, UInt Val) {
    // Val = in 2^14 clock cycle increments
    Bool  Enable = (Val != 0);
    Val = Val & 0xFF;
    if (Enable) Val = Val | (1 << 8);
    WRITE_UINT(Val, pBase, DTA_DMA_TIMEOUT_REG);
}

static __inline UInt32  DtaDmaTimeoutGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_DMA_TIMEOUT_REG) & 0x1FF;
}

//.-.-.-.-.-.-.-.-.-.-.- DMA Register: Descriptor frefetch control -.-.-.-.-.-.-.-.-.-.-.-
// Bit defines
#define DTA_DMA_PREFETCH_EN            (0x1  << 0)
#define DTA_DMA_DESC_OFFSET            (0x3  << 4)
#define DTA_DMA_NUM_SECRIPTORS         (0x3f << 8)

static __inline void  DtaDmaDescrPrefetchEn(volatile UInt8* pBase, UInt En, UInt Dma64) {
    // We always enable 16 descriptors
    UInt Val = DTA_DMA_PREFETCH_EN | (Dma64?2<<4 : 1<<4) | 16<<8;
    WRITE_UINT(Val, pBase, DTA_DMA_DESCR_PREFETCH_REG);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DMA Register: Command / Status -.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DTA_DMA_CMDSTAT_ABORT          (0x1 << 2)        // DMA Channal Abort
#define DTA_DMA_CMDSTAT_INTACT         (0x1 << 3)        // DMA Channel Interrupt Active
#define DTA_DMA_CMDSTAT_DONE           (0x1 << 4)        // DMA Channel Interrupt Done
#define DTA_DMA_CMDSTAT_DAC_EN         (0x1 << 5)        // Enable 64-bit addresses
#define DTA_DMA_CMDSTAT_SIZE_EN        (0x1 << 7)
#define DTA_DMA_CMDSTAT_SIZE_MASK      (0xFFFFFF << 8)

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ General Network Registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Utility functions for MAC-address conversion (also used in this file)
static __inline  UInt32  MAC_ADDRESS_LOW(UInt8* MacAddr) {
    return (UInt32)(MacAddr[0] | (MacAddr[1]<<8) | (MacAddr[2]<<16) | (MacAddr[3]<<24));
}
static __inline  UInt32  MAC_ADDRESS_HIGH(UInt8* MacAddr) {
    return (UInt32)(MacAddr[4] | (MacAddr[5]<<8));
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- General Network Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA_NW_CTRL                    0x0004
#define DTA_NW_STATUS                  0x0008
#define DTA_NW_90KHZ_REFCLK            0x000C             // Obsolete: use the ref. clock
#define DTA_NW_RX_PACKET_ERR_STAT      0x0024
#define DTA_NW_TX_PACKET_ERR_STAT      0x0028

//.-.-.-.-.-.-.-.-.-.-.- Network Control Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.
// Network Control: Bit fields
#define DTA_NWCTRL_MACINTEN            0x1
#define DTA_NWCTRL_MDIOINTEN           (0x1 << 1)
#define DTA_NWCTRL_IPTXERRINTEN        (0x1 << 3)

#define DTA_NWCTRL_LINKVALID           (0x1 << 8)
#define DTA_NWCTRL_TIMESLICE           (0x1F << 16)

// Network Control: Register access
static __inline UInt  DtaNwCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_NW_CTRL);
}
static __inline void  DtaNwCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_NW_CTRL);
}

// Network Control: MAC interrupt enable/disable (MacIntEn)
static __inline void  DtaNwCtrlSetMacIntEn(volatile UInt8* pBase, UInt Enable) {
    UInt  Val = DtaNwCtrlGet(pBase);
    if (Enable != 0) Val |=  DTA_NWCTRL_MACINTEN;
    else             Val &= ~DTA_NWCTRL_MACINTEN;
    DtaNwCtrlSet(pBase, Val);
}

// Network Control: PHY interrupt enable/disable (MdioIntEn)
static __inline void  DtaNwCtrlSetMdioIntEn(volatile UInt8* pBase, UInt Enable) {
    UInt  Val = DtaNwCtrlGet(pBase);
    if (Enable != 0) Val |=  DTA_NWCTRL_MDIOINTEN;
    else             Val &= ~DTA_NWCTRL_MDIOINTEN;
    DtaNwCtrlSet(pBase, Val);
}

static __inline UInt  DtaNwCtrlGetMdioIntEn(volatile UInt8* pBase) {
    UInt  Val = DtaNwCtrlGet(pBase);
    return ((Val & DTA_NWCTRL_MDIOINTEN) != 0 ? 1 : 0);
}


// Network Control: IpTx error interrupt enable/disable (IPTxErrorInt)
static __inline void  DtaNwCtrlSetIpTxErrIntEn(volatile UInt8* pBase, UInt Enable) {
    UInt  Val = DtaNwCtrlGet(pBase);
    if (Enable != 0) Val |=  DTA_NWCTRL_IPTXERRINTEN;
    else             Val &= ~DTA_NWCTRL_IPTXERRINTEN;
    DtaNwCtrlSet(pBase, Val);
}

// Network Control: Link valid state to FPGA
static __inline void  DtaNwCtrlSetLinkValid(volatile UInt8* pBase, UInt Valid) {
    UInt  Val = DtaNwCtrlGet(pBase);
    if (Valid != 0) Val |=  DTA_NWCTRL_LINKVALID;
    else            Val &= ~DTA_NWCTRL_LINKVALID;
    DtaNwCtrlSet(pBase, Val);
}

// Network Control: Set timeslice value
static __inline void  DtaNwCtrlSetTimeslice(volatile UInt8* pBase, UInt Value) {
    UInt  Val = DtaNwCtrlGet(pBase);
    Val &= ~DTA_NWCTRL_TIMESLICE;
    Val |= (Value << 16);
    DtaNwCtrlSet(pBase, Val);
}

//.-.-.-.-.-.-.-.-.-.-.- Network Status Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-
// Network Status: Bit fields
#define DTA_NWSTAT_MACINT              0x1
#define DTA_NWSTAT_MDIOINT             (0x1 << 1)
#define DTA_NWSTAT_IPTXERRINT          (0x1 << 3)

// Network  Status: Register access
static __inline UInt  DtaNwStatGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_NW_STATUS);
}
static __inline void  DtaNwStatSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_NW_STATUS);
}

// Network Status: MAC interrupt status 
static __inline UInt32  DtaNwStatGetMacInt(volatile UInt8* pBase) {
    UInt  Val = DtaNwStatGet(pBase);
    return ((Val & DTA_NWSTAT_MACINT) == 0 ? 0 : 1);
}

// Network Control: PHY interrupt status
static __inline UInt  DtaNwStatGetMdioInt(volatile UInt8* pBase) {
    UInt  Val = DtaNwStatGet(pBase);
    return ((Val & DTA_NWSTAT_MDIOINT) == 0 ? 0 : 1);
}

// Network Control: IpTx error interrupt status
static __inline UInt  DtaNwStatGetIpTxErrInt(volatile UInt8* pBase) {
    UInt  Val = DtaNwStatGet(pBase);
    return ((Val & DTA_NWSTAT_IPTXERRINT) == 0 ? 0 : 1);
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ PHY Registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Register defines for:
// National PHY:  DP83865:  DTA-160
// Marvel PHY:    88E1111:  DTA-160
//                88E1119R: DTA-2160
//                88E1121R: DTA-2162

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Phy address definitions: General -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA_PHY_ADDRESS_BMCR               (0x00)  // Basic Mode Control
#define DTA_PHY_ADDRESS_BMSR               (0x01)  // Basic Mode Status
#define DTA_PHY_ADDRESS_PHYIDR1            (0x02)  // Phy Identifier 1
#define DTA_PHY_ADDRESS_PHYIDR2            (0x03)  // Phy Identifier 2
#define DTA_PHY_ADDRESS_ANAR               (0x04)  // For Marvell: SWRESET needed
#define DTA_PHY_ADDRESS_ANLAR              (0x05)
#define DTA_PHY_ADDRESS_1KTCR              (0x09)  // For Marvell: SWRESET needed
#define DTA_PHY_ADDRESS_1KSTSR             (0x0A)  // 1K-T Status
#define DTA_PHY_ADDRESS_LINK_AN            (0x11)  // Bits Marvell/National not identical!

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Phy addess definitions: National -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA_PHY_ADDRESS_NAT_AUX_CTRL       (0x12)
#define DTA_PHY_ADDRESS_NAT_INT_STAT       (0x14)
#define DTA_PHY_ADDRESS_NAT_INT_MASK       (0x15)
#define DTA_PHY_ADDRESS_NAT_INT_CLR        (0x17)
#define DTA_PHY_ADDRESS_NAT_EXP_MEM_DATA   (0x1D)
#define DTA_PHY_ADDRESS_NAT_EXP_MEM_ADDR   (0x1E)

//-.-.-.-.-.-.-.-.-.-.-.- Phy addess definitions: Marvell General -.-.-.-.-.-.-.-.-.-.-.-.
#define DTA_PHY_ADDRESS_MAR_INT_MASK       (0x12)
#define DTA_PHY_ADDRESS_MAR_INT_STAT       (0x13)
#define DTA_PHY_ADDRESS_MAR_INT_CLR        (0x13)  // Reading the int. status will clear
                                                    // the interrupts.

//-.-.-.-.-.-.-.-.-.-.-.- Phy addess definitions: Marvell 88E1111 -.-.-.-.-.-.-.-.-.-.-.-.
#define DTA_PHY_ADDRESS_MAR_EXT_PHY_CTRL   (0x14)
#define DTA_PHY_ADDRESS_MAR_LED_CTRL       (0x18)
#define DTA_PHY_ADDRESS_MAR_EXT_ADDR       (0x1D)
#define DTA_PHY_ADDRESS_MAR_EXT_DATA       (0x1E)

//.-.-.-.-.-.-.-.-.- Phy address definitions: Marvell 88E1119R/88E1121R -.-.-.-.-.-.-.-.-.
#define DTA_PHY_ADDRESS_MAR_COP_SPEC_CTRL1 (0x10)  // Page 0
#define DTA_PHY_ADDRESS_MAR_LED_FUNC_CTRL  (0x10)  // Page 3
#define DTA_PHY_ADDRESS_MAR_LED_POL_CTRL   (0x11)  // Page 3
#define DTA_PHY_ADDRESS_MAR_LED_TIM_CTRL   (0x12)  // Page 3
#define DTA_PHY_ADDRESS_MAR_MAC_SPEC_CTRL  (0x15)  // Page 2

#define DTA_PHY_ADDRESS_MAR_PAGE           (0x16)  // Page selection

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Phy bit definitions: General -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// BMCR defines
#define DTA_PHY_BMCR_SWRESET_SHIFT         (15)
#define DTA_PHY_BMCR_SWRESET_MASK          (1  << DTA_PHY_BMCR_SWRESET_SHIFT)
#define DTA_PHY_BMCR_LOOPBACK_SHIFT        (14)
#define DTA_PHY_BMCR_LOOPBACK_MASK         (1  << DTA_PHY_BMCR_LOOPBACK_SHIFT)
#define DTA_PHY_BMCR_SPEED0_SHIFT          (13)
#define DTA_PHY_BMCR_SPEED0_MASK           (1  << DTA_PHY_BMCR_SPEED0_SHIFT)
#define DTA_PHY_BMCR_AUTO_NEG_EN_SHIFT     (12)
#define DTA_PHY_BMCR_AUTO_NEG_EN_MASK      (1  << DTA_PHY_BMCR_AUTO_NEG_EN_SHIFT)
#define DTA_PHY_BMCR_POWER_DOWN_SHIFT      (11)
#define DTA_PHY_BMCR_POWER_DOWN_MASK       (1  << DTA_PHY_BMCR_POWER_DOWN_SHIFT)
#define DTA_PHY_BMCR_AUTO_NEG_RST_SHIFT    (9)
#define DTA_PHY_BMCR_AUTO_NEG_RST_MASK     (1  << DTA_PHY_BMCR_AUTO_NEG_RST_SHIFT)
#define DTA_PHY_BMCR_DUPLEX_SHIFT          (8)
#define DTA_PHY_BMCR_DUPLEX_MASK           (1  << DTA_PHY_BMCR_DUPLEX_SHIFT)
#define DTA_PHY_BMCR_SPEED1_SHIFT          (6)
#define DTA_PHY_BMCR_SPEED1_MASK           (1  << DTA_PHY_BMCR_SPEED1_SHIFT)

// 1KTCR defines: 1Gb manual Master/Slave configuration
#define DTA_PHY_1KTCR_MSCFG_EN_SHIFT       (12)
#define DTA_PHY_1KTCR_MSCFG_EN_MASK        (1 << DTA_PHY_1KTCR_MSCFG_EN_SHIFT)
#define DTA_PHY_1KTCR_MSCFG_MASTER_SHIFT   (11)
#define DTA_PHY_1KTCR_MSCFG_MASTER_MASK    (1 << DTA_PHY_1KTCR_MSCFG_MASTER_SHIFT)
#define DTA_PHY_1KTCR_MSCFG_CAP_SHIFT      (8)
#define DTA_PHY_1KTCR_MSCFG_CAP_MASK       (3 << DTA_PHY_1KTCR_MSCFG_CAP_SHIFT)

// 1KSTSR defines: 1Gb Status register
#define DTA_PHY_1KSTR_MSCFG_FAULT_SHIFT    (15)
#define DTA_PHY_1KSTR_MSCFG_FAULT_MASK     (1 << DTA_PHY_1KSTR_MSCFG_FAULT_SHIFT)
#define DTA_PHY_1KSTR_MSCFG_MASTER_SHIFT   (14)
#define DTA_PHY_1KSTR_MSCFG_MASTER_MASK    (1 << DTA_PHY_1KSTR_MSCFG_MASTER_SHIFT)
#define DTA_PHY_1KSTR_LOC_RX_STAT_SHIFT    (13)
#define DTA_PHY_1KSTR_LOC_RX_STAT_MASK     (1 << DTA_PHY_1KSTR_LOC_RX_STAT_SHIFT)
#define DTA_PHY_1KSTR_REM_RX_STAT_SHIFT    (12)
#define DTA_PHY_1KSTR_REM_RX_STAT_MASK     (1 << DTA_PHY_1KSTR_REM_RXS_TAT_SHIFT)
#define DTA_PHY_1KSTR_LP_1GB_FD_CAP_SHIFT  (11)
#define DTA_PHY_1KSTR_LP_1GB_FD_CAP_MASK   (1 << DTA_PHY_1KSTR_LP_1GB_FD_CAP_SHIFT)
#define DTA_PHY_1KSTR_LP_1GB_HD_CAP_SHIFT  (10)
#define DTA_PHY_1KSTR_LP_1GB_HD_CAP_MASK   (1 << DTA_PHY_1KSTR_LP_1GB_HD_CAP_SHIFT)
#define DTA_PHY_1KSTR_IDLE_ERR_CNT_SHIFT   (0)
#define DTA_PHY_1KSTR_IDLE_ERR_CNT_MASK    (0xf << DTA_PHY_1KSTR_IDLE_ERR_CNT_SHIFT)

// LINK_AN defines generic
#define DTA_PHY_LINK_AN_SPEED_10           0x0
#define DTA_PHY_LINK_AN_SPEED_100          0x1
#define DTA_PHY_LINK_AN_SPEED_1000         0x2
#define DTA_PHY_LINK_AN_UNDEFINED          0x3

#define DTA_PHY_LINK_AN_DUPLEX_HALF        0x0
#define DTA_PHY_LINK_AN_DUPLEX_FULL        0x1

#define DTA_PHY_LINK_AN_LINK_DOWN          0x0
#define DTA_PHY_LINK_AN_LINK_UP            0x1


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Phy bit definitions: National -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// LINK_AN defines National
#define DTA_PHY_LINK_AN_NAT_SPEEDST_SHIFT  (3)
#define DTA_PHY_LINK_AN_NAT_SPEEDST_MASK   (0x3 << DTA_PHY_LINK_AN_NAT_SPEEDST_SHIFT)
#define DTA_PHY_LINK_AN_NAT_LINKST_SHIFT   (2)
#define DTA_PHY_LINK_AN_NAT_LINKST_MASK    (0x1 << DTA_PHY_LINK_AN_NAT_LINKST_SHIFT)
#define DTA_PHY_LINK_AN_NAT_DUPLEXST_SHIFT (1)
#define DTA_PHY_LINK_AN_NAT_DUPLEXST_MASK  (0x1 << DTA_PHY_LINK_AN_NAT_DUPLEXST_SHIFT)
#define DTA_PHY_LINK_AN_NAT_MASTER_SHIFT   (0)
#define DTA_PHY_LINK_AN_NAT_MASTER_MASK    (0x1 << DTA_PHY_LINK_AN_NAT_MASTER_SHIFT)

// Interrups mask bits National
#define DTA_PHY_NAT_INT_MASK_BITS          0xE010  // Speed, Link and Duplex change
                                                    // + autoneg ready

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Phy bit definitions: Marvell -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// LINK_AN defines MARVELL
#define DTA_PHY_LINK_AN_MAR_SPEEDST_SHIFT  (14)
#define DTA_PHY_LINK_AN_MAR_SPEEDST_MASK   (0x3 << DTA_PHY_LINK_AN_MAR_SPEEDST_SHIFT)
#define DTA_PHY_LINK_AN_MAR_LINKST_SHIFT   (10)
#define DTA_PHY_LINK_AN_MAR_LINKST_MASK    (0x1 << DTA_PHY_LINK_AN_MAR_LINKST_SHIFT)
#define DTA_PHY_LINK_AN_MAR_DUPLEXST_SHIFT (13)
#define DTA_PHY_LINK_AN_MAR_DUPLEXST_MASK  (0x1 << DTA_PHY_LINK_AN_MAR_DUPLEXST_SHIFT)

// EXT_PHY_CTRL
#define DTA_PHY_MAR_EPC_BLOCK_CAR_EXT_SHIFT (15)
#define DTA_PHY_MAR_EPC_BLOCK_CAR_EXT_MASK  (0x1 << DTA_PHY_MAR_EPC_BLOCK_CAR_EXT_SHIFT)
#define DTA_PHY_MAR_EPC_LINE_LOOPBACK_SHIFT (14)
#define DTA_PHY_MAR_EPC_LINE_LOOPBACK_MASK  (0x1 << DTA_PHY_MAR_EPC_LINE_LOOPBACK_SHIFT)
#define DTA_PHY_MAR_EPC_DIS_LINK_PULS_SHIFT (14)
#define DTA_PHY_MAR_EPC_DIS_LINK_PULS_MASK  (0x1 << DTA_PHY_MAR_EPC_DIS_LINK_PULS_SHIFT)
#define DTA_PHY_MAR_EPC_DOWN_SH_CNT_SHIFT   (9)
#define DTA_PHY_MAR_EPC_DOWN_SH_CNT_MASK    (0x7 << DTA_PHY_MAR_EPC_DOWN_SH_CNT_SHIFT)
#define DTA_PHY_MAR_EPC_DOWN_SH_EN_SHIFT    (8)
#define DTA_PHY_MAR_EPC_DOWN_SH_EN_MASK     (0x1 << DTA_PHY_MAR_EPC_DOWN_SH_EN_SHIFT)
#define DTA_PHY_MAR_EPC_RGMII_RX_TIME_SHIFT (7)
#define DTA_PHY_MAR_EPC_RGMII_RX_TIME_MASK  (0x1 << DTA_PHY_MAR_EPC_RGMII_RX_TIME_SHIFT)
#define DTA_PHY_MAR_EPC_DEF_MAC_SPEED_SHIFT (4)
#define DTA_PHY_MAR_EPC_DEF_MAC_SPEED_MASK  (0x7 << DTA_PHY_MAR_EPC_DEF_MAC_SPEED_SHIFT)
#define DTA_PHY_MAR_EPC_DTE_DETECT_EN_SHIFT (2)
#define DTA_PHY_MAR_EPC_DTE_DETECT_EN_MASK  (0x1 << DTA_PHY_MAR_EPC_DTE_DETECT_EN_SHIFT)
#define DTA_PHY_MAR_EPC_RGMII_TX_CTRL_SHIFT (1)
#define DTA_PHY_MAR_EPC_RGMII_TX_CTRL_MASK  (0x1 << DTA_PHY_MAR_EPC_RGMII_TX_CTRL_SHIFT)
#define DTA_PHY_MAR_EPC_TX_DIS_SHIFT        (1)
#define DTA_PHY_MAR_EPC_TX_DIS_MASK         (0x1 << DTA_PHY_MAR_EPC_TX_DIS_SHIFT)

// Autonegotiation disabled:
#define DTA_PHY_MAR_EPC_DEF_MAC_SPEED_10    (0x0 << DTA_PHY_MAR_EPC_DEF_MAC_SPEED_SHIFT)
#define DTA_PHY_MAR_EPC_DEF_MAC_SPEED_100   (0x1 << DTA_PHY_MAR_EPC_DEF_MAC_SPEED_SHIFT)
#define DTA_PHY_MAR_EPC_DEF_MAC_SPEED_1000  (0x3 << DTA_PHY_MAR_EPC_DEF_MAC_SPEED_SHIFT)
#define DTA_PHY_MAR_EPC_DEF_MAC_SPEED_TXCLK (0x4 << DTA_PHY_MAR_EPC_DEF_MAC_SPEED_SHIFT)

// Autonegotiation enabled:
#define DTA_PHY_MAR_EPC_DEF_MAC_SPEED_10_2d5MHZ_0          (0x0 << DTA_PHY_MAR_EPC_DEF_MAC_SPEED_SHIFT)
#define DTA_PHY_MAR_EPC_DEF_MAC_SPEED_100_25MHZ_0          (0x1 << DTA_PHY_MAR_EPC_DEF_MAC_SPEED_SHIFT)
#define DTA_PHY_MAR_EPC_DEF_MAC_SPEED_1000_0_0             (0x2 << DTA_PHY_MAR_EPC_DEF_MAC_SPEED_SHIFT)
#define DTA_PHY_MAR_EPC_DEF_MAC_SPEED_10_2d5MHZ_2d5MHZ     (0x4 << DTA_PHY_MAR_EPC_DEF_MAC_SPEED_SHIFT)
#define DTA_PHY_MAR_EPC_DEF_MAC_SPEED_100_25MHZ_25MHZ      (0x5 << DTA_PHY_MAR_EPC_DEF_MAC_SPEED_SHIFT)
#define DTA_PHY_MAR_EPC_DEF_MAC_SPEED_1000_2d5MHZ_2d5MHZ   (0x6 << DTA_PHY_MAR_EPC_DEF_MAC_SPEED_SHIFT)
#define DTA_PHY_MAR_EPC_DEF_MAC_SPEED_1000_25MHZ_25MHZ     (0x7 << DTA_PHY_MAR_EPC_DEF_MAC_SPEED_SHIFT)

// MAC_SPEC_CTRL defines
// Generic:
#define DTA_PHY_MAR_MSC_LINE_LOOPBACK_SHIFT (14)
#define DTA_PHY_MAR_MSC_LINE_LOOPBACK_MASK  (0x1 << DTA_PHY_MAR_MSC_LINE_LOOPBACK_SHIFT)
#define DTA_PHY_MAR_MSC_BLOCK_CAR_EXT_SHIFT (3)
#define DTA_PHY_MAR_MSC_BLOCK_CAR_EXT_MASK  (0x1 << DTA_PHY_MAR_MSC_BLOCK_CAR_EXT_SHIFT)

// 88E1119R:
#define DTA_PHY_MAR_MSC_DEF_MAC_SPEED_SHIFT (2)
#define DTA_PHY_MAR_MSC_DEF_MAC_SPEED_MASK  (0x7 << DTA_PHY_MAR_MSC_DEF_MAC_SPEED_SHIFT)

#define DTA_PHY_MAR_MSC_DEF_MAC_SPEED_10_2d5MHZ_0          0x0
#define DTA_PHY_MAR_MSC_DEF_MAC_SPEED_100_25MHZ_0          0x1
#define DTA_PHY_MAR_MSC_DEF_MAC_SPEED_1000_0_0             0x2
#define DTA_PHY_MAR_MSC_DEF_MAC_SPEED_10_2d5MHZ_2d5MHZ     0x4
#define DTA_PHY_MAR_MSC_DEF_MAC_SPEED_100_25MHZ_25MHZ      0x5
#define DTA_PHY_MAR_MSC_DEF_MAC_SPEED_1000_2d5MHZ_2d5MHZ   0x6
#define DTA_PHY_MAR_MSC_DEF_MAC_SPEED_1000_25MHZ_25MHZ     0x7

// 88E1121R:
#define DTA_PHY_MAC_MSC_DEF_MAC_SPEED_MASK  (0x2040)
#define DTA_PHY_MAR_MSC_DEF_MAC_SPEED_10    (0x0000)
#define DTA_PHY_MAR_MSC_DEF_MAC_SPEED_100   (0x2000)
#define DTA_PHY_MAR_MSC_DEF_MAC_SPEED_1000  (0x0040)


#define DTA_PHY_MAR_MSC_BLOCK_CAR_EXT_SHIFT (3)
#define DTA_PHY_MAR_MSC_BLOCK_CAR_EXT_MASK  (0x1 << DTA_PHY_MAR_MSC_BLOCK_CAR_EXT_SHIFT)


// Interrups mask bits Marvel
#define DTA_PHY_MAR_INT_MASK_BITS          0x6C00  // Speed, Link and Duplex change
                                                    // + autoneg ready

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ MAC Altera Registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// Supports: DTA-2162

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA_MACA_REVISION              0x000
#define DTA_MACA_SCRATCH               0x004
#define DTA_MACA_CMD_CONFIG            0x008
#define DTA_MACA_MAC_ADDR_LOW          0x00C
#define DTA_MACA_MAC_ADDR_HIGH         0x010
#define DTA_MACA_FRM_LENGTH            0x014
#define DTA_MACA_PAUSE_QUANT           0x018
#define DTA_MACA_RX_SECTION_EMPTY      0x01C
#define DTA_MACA_RX_SECTION_FULL       0x020
#define DTA_MACA_TX_SECTION_EMPTY      0x024
#define DTA_MACA_TX_SECTION_FULL       0x028
#define DTA_MACA_RX_ALMOST_EMPTY       0x02C
#define DTA_MACA_RX_ALMOST_FULL        0x030
#define DTA_MACA_TX_ALMOST_EMPTY       0x034
#define DTA_MACA_TX_ALMOST_FULL        0x038
#define DTA_MACA_MDIO_ADDR0            0x03C
#define DTA_MACA_MDIO_ADDR1            0x040
#define DTA_MACA_REG_STATUS            0x058
#define DTA_MACA_TX_IPG_LENGTH         0x05C

// Statistic counters
#define DTA_MACA_REG_TXFRMOK           0x0068      // #frames Tx'ed ok (pause frames included)
#define DTA_MACA_REG_RXFRMOK           0x006C      // #frames Rx'ed ok
#define DTA_MACA_REG_RXFCSERR          0x0070      // #frames Rx'ed and do not pass the FCS check
#define DTA_MACA_REG_RXALIGNERR        0x0074      // #frames Rx'ed with align errors
#define DTA_MACA_REG_TXOCTOK           0x0078      // #bytes Tx'ed ok
#define DTA_MACA_REG_RXOCTOK           0x007C      // #bytes Rx'ed ok
#define DTA_MACA_REG_TXPAUSE           0x0080      // #frames Tx'ed ok with pause operand
#define DTA_MACA_REG_RXPAUSE           0x0084      // #frames Rx'ed ok with MAC pause control
#define DTA_MACA_REG_RXERRORS          0x0088      // #frames Rx'ed with errors (new)
#define DTA_MACA_REG_TXERRORS          0x008C      // #frames Tx'ed with errors (new)
#define DTA_MACA_REG_RXUNIOK           0x0090      // #unicast frames Rx'ed ok
#define DTA_MACA_REG_RXMULTIOK         0x0094      // #multicast frames Rx'ed ok
#define DTA_MACA_REG_RXBROADOK         0x0098      // #broadcast frames Rx'ed ok
#define DTA_MACA_REG_TXUNIOK           0x00A0      // #unicast frames Tx'ed ok
#define DTA_MACA_REG_TXMULTIOK         0x00A4      // #multicast frames Tx'ed ok
#define DTA_MACA_REG_TXBROADOK         0x00A8      // #broadcast frames Tx'ed ok
#define DTA_MACA_REG_RXFRMSIZE0        0x00B8      // #Good packets Rx'ed grouped by size <=63 octets
#define DTA_MACA_REG_RXFRMSIZE1        0x00C0      // #Good packets Rx'ed grouped by size == 64 octets
#define DTA_MACA_REG_RXFRMSIZE2        0x00C4      // #Good packets Rx'ed grouped by size 65..127 octets
#define DTA_MACA_REG_RXFRMSIZE3        0x00C8      // #Good packets Rx'ed grouped by size 128..255 octets
#define DTA_MACA_REG_RXFRMSIZE4        0x00CC      // #Good packets Rx'ed grouped by size 256..511 octets
#define DTA_MACA_REG_RXFRMSIZE5        0x00D0      // #Good packets Rx'ed grouped by size 512..1023 octets
#define DTA_MACA_REG_RXFRMSIZE6        0x00D4      // #Good packets Rx'ed grouped by size 1024..1518 octets
#define DTA_MACA_REG_RXFRMSIZE7        0x00D8      // #Good packets Rx'ed grouped by size >1518 octets
#define DTA_MACA_REG_RXJABBERS         0x00DC      // #Too loog frames with CRC errors (new)
#define DTA_MACA_REG_RXFRAGMENTS       0x00E0      // #Too short frames with CRC errors (new)

#define DTA_MACA_REG_TX_CMD_STAT       0x00E8      // Tx Command/Status
#define DTA_MACA_REG_RX_CMD_STAT       0x00EC      // Rx Command/Status

// Transmit: No Altera MAC equivalent for following Cast MAC counters??
//#define DTA_SCM_REG_TXDEF              0x8136      // #frames Tx'ed ok, no coll, media busy
//#define DTA_SCM_REG_TXPAUSE            0x8137      // #frames Tx'ed ok with pause operand
//#define DTA_SCM_REG_TXLCERR            0x813A      // #frames Tx'ed with late collision
//#define DTA_SCM_REG_TXMACERR           0x813B      // #frames Tx'ed aborted due to underrun
//#define DTA_SCM_REG_TXCSERR            0x813C      // #frames Tx'ed with no carrier sense

// Hash table
#define DTA_MACA_HASH_TABLE            0x100
// PHY addresses
#define DTA_MACA_MDIO_SPACE0           0x200
#define DTA_MACA_MDIO_SPACE1           0x280

//.-.-.-.-.-.-.-.-.-.-.- Command Config Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-
// Command Config: Bit fields
#define DTA_MACA_CMD_CONFIG_TX_ENA             (1)
#define DTA_MACA_CMD_CONFIG_RX_ENA             (1<<1)
#define DTA_MACA_CMD_CONFIG_XON_GEN            (1<<2)
#define DTA_MACA_CMD_CONFIG_ETH_SPEED          (1<<3)
#define DTA_MACA_CMD_CONFIG_PROMIS_EN          (1<<4)
#define DTA_MACA_CMD_CONFIG_PAD_EN             (1<<5)
#define DTA_MACA_CMD_CONFIG_CRC_FWD            (1<<6)
#define DTA_MACA_CMD_CONFIG_PAUSE_FWD          (1<<7)
#define DTA_MACA_CMD_CONFIG_PAUSE_IGNORE       (1<<8)
#define DTA_MACA_CMD_CONFIG_TX_ADDR_INS        (1<<9)
#define DTA_MACA_CMD_CONFIG_HD_ENA             (1<<10)
#define DTA_MACA_CMD_CONFIG_EXCESS_COL         (1<<11)
#define DTA_MACA_CMD_CONFIG_LATE_COL           (1<<12)
#define DTA_MACA_CMD_CONFIG_SW_RESET           (1<<13)
#define DTA_MACA_CMD_CONFIG_MHAS_SEL           (1<<14)
#define DTA_MACA_CMD_CONFIG_LOOP_ENA           (1<<15)
#define DTA_MACA_CMD_CONFIG_TX_ADDR_SEL        (7<<16)
#define DTA_MACA_CMD_CONFIG_MAGIC_ENA          (1<<19)
#define DTA_MACA_CMD_CONFIG_SLEEP              (1<<20)
#define DTA_MACA_CMD_CONFIG_WAKEUP             (1<<21)
#define DTA_MACA_CMD_CONFIG_XOFF_GEN           (1<<22)
#define DTA_MACA_CMD_CONFIG_CNTL_FRM_ENA       (1<<23)
#define DTA_MACA_CMD_CONFIG_NO_LGTH_CHECK      (1<<24)
#define DTA_MACA_CMD_CONFIG_ENA_10             (1<<25)
#define DTA_MACA_CMD_CONFIG_RX_ERR_DISC        (1<<26)
#define DTA_MACA_CMD_CONFIG_DISABLE_RD_TIMEOUT (1<<27)
#define DTA_MACA_CMD_CONFIG_CNT_RESET          (1<<31)

// Command Config: Register access
static __inline UInt  DtaMacACmdConfigGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_MACA_CMD_CONFIG);
}

static __inline void  DtaMacACmdConfigSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_MACA_CMD_CONFIG);
}

// Command Config: Start/Stop Receive command
static __inline void  DtaMacAEnableRx(volatile UInt8* pBase, UInt Enable) {
    UInt32  Val = DtaMacACmdConfigGet(pBase);
    if (Enable != 0) Val |=  DTA_MACA_CMD_CONFIG_RX_ENA;
    else             Val &= ~DTA_MACA_CMD_CONFIG_RX_ENA;
    DtaMacACmdConfigSet(pBase, Val);
}

// Command Config: Start/Stop Transmit command
static __inline void  DtaMacAEnableTx(volatile UInt8* pBase, UInt Enable) {
    UInt32  Val = DtaMacACmdConfigGet(pBase);
    if (Enable != 0) Val |=  DTA_MACA_CMD_CONFIG_TX_ENA;
    else             Val &= ~DTA_MACA_CMD_CONFIG_TX_ENA;
    DtaMacACmdConfigSet(pBase, Val);
}

// Command Config: Check is Tx+Rx is disabled
static __inline UInt DtaMacAIsRxTxDisabled(volatile UInt8* pBase) {
    UInt32  Val = DtaMacACmdConfigGet(pBase);
    return (Val & DTA_MACA_CMD_CONFIG_TX_ENA)==0 && (Val & DTA_MACA_CMD_CONFIG_RX_ENA)==0;
}

// Command Config: Speed = 1: 1GBit, 0:10/100 MBit
static __inline void  DtaMacASetEthSpeed(volatile UInt8* pBase, UInt Speed) {
    UInt32  Val = DtaMacACmdConfigGet(pBase);
    if (Speed != 0) Val |=  DTA_MACA_CMD_CONFIG_ETH_SPEED;
    else            Val &= ~DTA_MACA_CMD_CONFIG_ETH_SPEED;
    DtaMacACmdConfigSet(pBase, Val);
}

// Command Config: Enable promiscuous mode
static __inline void  DtaMacASetPromisEn(volatile UInt8* pBase, UInt Enable) {
    UInt32  Val = DtaMacACmdConfigGet(pBase);
    if (Enable != 0) Val |=  DTA_MACA_CMD_CONFIG_PROMIS_EN;
    else             Val &= ~DTA_MACA_CMD_CONFIG_PROMIS_EN;
    DtaMacACmdConfigSet(pBase, Val);
}

// Command Config: Enable forwarding CRC to user application
static __inline void  DtaMacASetCrcFwd(volatile UInt8* pBase, UInt Enable) {
    UInt32  Val = DtaMacACmdConfigGet(pBase);
    if (Enable != 0) Val |=  DTA_MACA_CMD_CONFIG_CRC_FWD;
    else             Val &= ~DTA_MACA_CMD_CONFIG_CRC_FWD;
    DtaMacACmdConfigSet(pBase, Val);
}

// Command Config: Half duplex enable/disable
static __inline void  DtaMacASetHalfDuplex(volatile UInt8* pBase, UInt Enable) {
    UInt32  Val = DtaMacACmdConfigGet(pBase);
    if (Enable != 0) Val |=  DTA_MACA_CMD_CONFIG_HD_ENA;
    else             Val &= ~DTA_MACA_CMD_CONFIG_HD_ENA;
    DtaMacACmdConfigSet(pBase, Val);
}

// Command Config: 10MBit enable/disable
static __inline void  DtaMacASet10Mbit(volatile UInt8* pBase, UInt Enable) {
    UInt32  Val = DtaMacACmdConfigGet(pBase);
    if (Enable != 0) Val |=  DTA_MACA_CMD_CONFIG_ENA_10;
    else             Val &= ~DTA_MACA_CMD_CONFIG_ENA_10;
    DtaMacACmdConfigSet(pBase, Val);
}

// Command Config: Software Reset Set
static __inline void  DtaMacASwResetSet(volatile UInt8* pBase) {
    UInt32  Val = DtaMacACmdConfigGet(pBase);
    Val |=  DTA_MACA_CMD_CONFIG_SW_RESET;
    DtaMacACmdConfigSet(pBase, Val);
}

// Command Config: Software Reset Get status
static __inline UInt  DtaMacASwResetGet(volatile UInt8* pBase) {
    UInt32  Val = DtaMacACmdConfigGet(pBase);
    return (Val & DTA_MACA_CMD_CONFIG_SW_RESET ? 1 : 0);
}

//.-.-.-.-.-.-.-.-.-.-.- MAC Address Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-
// MAC Address: Set MAC address
static __inline void  DtaMacAMacAddressSet(volatile UInt8* pBase, UInt8* MacAddr) {
    WRITE_UINT(MAC_ADDRESS_LOW(MacAddr),  pBase, DTA_MACA_MAC_ADDR_LOW);
    WRITE_UINT(MAC_ADDRESS_HIGH(MacAddr), pBase, DTA_MACA_MAC_ADDR_HIGH);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- FIFO Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Set RxFifo empty thresshold
static __inline void  DtaMacARxSectionEmptySet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_MACA_RX_SECTION_EMPTY);
}

// Set RxFifo full thresshold
static __inline void  DtaMacARxSectionFullSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_MACA_RX_SECTION_FULL);
}

// Set TxFifo empty thresshold
static __inline void  DtaMacATxSectionEmptySet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_MACA_TX_SECTION_EMPTY);
}

// Set TxFifo full thresshold
static __inline void  DtaMacATxSectionFullSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_MACA_TX_SECTION_FULL);
}

// Set RxFifo almost empty thresshold
static __inline void  DtaMacARxSectionAlmostEmptySet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_MACA_RX_ALMOST_EMPTY);
}

// Set RxFifo almost full thresshold
static __inline void  DtaMacARxSectionAlmostFullSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_MACA_RX_ALMOST_FULL);
}

// Set TxFifo almost empty thresshold
static __inline void  DtaMacATxSectionAlmostEmptySet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_MACA_TX_ALMOST_EMPTY);
}

// Set TxFifo almost full thresshold
static __inline void  DtaMacATxSectionAlmostFullSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_MACA_TX_ALMOST_FULL);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- MAC Phy Registers: Access functions -.-.-.-.-.-.-.-.-.-.-.-.-.
// Set Address of phy 0
static __inline void  DtaMacASetPhyAddress0(volatile UInt8* pBase, UInt Address) {
    WRITE_UINT(Address&0x1f, pBase, DTA_MACA_MDIO_ADDR0);
}

// Set Address of phy 1
static __inline void  DtaMacASetPhyAddress1(volatile UInt8* pBase, UInt Address) {
    WRITE_UINT(Address&0x1f, pBase, DTA_MACA_MDIO_ADDR1);
}

// Get phy register
static __inline UInt  DtaMacAGetPhyRegister(volatile UInt8* pBase, UInt Reg) {
    return READ_UINT((pBase+DTA_MACA_MDIO_SPACE0), Reg);
}

// Set phy register
static __inline void  DtaMacASetPhyRegister(volatile UInt8* pBase, UInt Reg, UInt Val) {
    WRITE_UINT(Val, (pBase+DTA_MACA_MDIO_SPACE0), Reg);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- Counter Registers: Access function -.-.-.-.-.-.-.-.-.-.-.-.-.
static __inline UInt  DtaMacAGetCounter(volatile UInt8* pBase, UInt CounterOffset) {
    return READ_UINT(pBase, CounterOffset);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- Tx Command/Stat: Access functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
// Tx Command/Stat: Bit fields
#define DTA_MACA_TXCMDSTAT_OMIT_CRC      (1 << 17)
#define DTA_MACA_TXCMDSTAT_SHIFT_16      (1 << 18)  // Add 16 bytes padding before packet

// Tx Command/Stat: Register Access
static __inline void  DtaMacASetTxCmdStatReg(volatile UInt8* pBase, UInt Value) {
    WRITE_UINT(Value, pBase, DTA_MACA_REG_TX_CMD_STAT);
}

static __inline UInt  DtaMacAGetTxCmdStatReg(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_MACA_REG_TX_CMD_STAT);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- Rx Command/Stat: Access functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
// Rx Command/Stat: Bit fields
#define DTA_MACA_RXCMDSTAT_SHIFT_16      (1 << 25)  // 16 bytes padding are added before
                                                    // packet

// Rx Command/Stat: Register Access
static __inline void  DtaMacASetRxCmdStatReg(volatile UInt8* pBase, UInt Value) {
    WRITE_UINT(Value, pBase, DTA_MACA_REG_RX_CMD_STAT);
}

static __inline UInt  DtaMacAGetRxCmdStatReg(volatile UInt8* pBase, UInt Value) {
    return READ_UINT(pBase, DTA_MACA_REG_RX_CMD_STAT);
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ MAC Cast Registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// Supports: DTA-160/DTA-2160
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA_MACC_BUS_MODE              0x000       // CSR0
#define DTA_MACC_TX_POLL_DEMAND        0x008       // CSR1
#define DTA_MACC_RX_POLL_DEMAND        0x010       // CSR2
#define DTA_MACC_RX_DESCR_LIST_BAR     0x018       // CSR3
#define DTA_MACC_TX_DESCR_LIST_BAR     0x020       // CSR4
#define DTA_MACC_STATUS                0x028       // CSR5
#define DTA_MACC_OPER_MODE             0x030       // CSR6
#define DTA_MACC_INT_ENABLE            0x038       // CSR7
#define DTA_MACC_MISSED_FRAMES_OVF     0x040       // CSR8
#define DTA_MACC_SERIAL_ROM            0x048       // CSR9
#define DTA_MACC_MII_SERIAL_MANAG      0x050       // CSR10
#define DTA_MACC_GP_TIM_INT_MIR        0x058       // CSR11
#define DTA_MACC_MAC_ADDR_LOW          0x080       // CSR16
#define DTA_MACC_MAC_ADDR_HIGH         0x088       // CSR17
#define DTA_MACC_PAUSE_CACHE_TRSH      0x090       // CSR18
#define DTA_MACC_FIFO_TRSH             0x098       // CSR19
#define DTA_MACC_FLOW_CONTROL          0x0A0       // CSR20
#define DTA_MACC_SC_MODE               0x0A8       // CSR21
#define DTA_MACC_SC_READ_VALUE         0x0B0       // CSR22

//.-.-.-.-.-.-.-.-.-.-.-.-.- BusMode  register: Access function -.-.-.-.-.-.-.-.-.-.-.-.-.

// BusMode register: Register access
static __inline UInt32 DtaMacCBusModeGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_MACC_BUS_MODE);
}

static __inline void DtaMacCBusModeSet(volatile UInt8* pBase, UInt32 Value) {
    WRITE_UINT(Value, pBase, DTA_MACC_BUS_MODE);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Status register: Access function -.-.-.-.-.-.-.-.-.-.-.-.-.-
// Status register: Bit fields
#define DTA_MACC_STAT_RECEIVE_BUSY_MASK  (7 << 17)
#define DTA_MACC_STAT_TRANSMIT_BUSY_MASK (7 << 20)

// Status register: Register access
static __inline UInt  DtaMacCStatusGet(volatile UInt8* pBase, UInt MacBug) {
    if (MacBug) {
        UInt  Val = READ_UINT(pBase, DTA_MACC_STATUS);
        // Workaround for R/W MAC problem in FPGA
        while (Val == 0)    // Some bits always 1
            Val = READ_UINT(pBase, DTA_MACC_STATUS);
        return Val;
    }
    return READ_UINT(pBase, DTA_MACC_STATUS);
}

// Status register: Is Rx busy
static __inline UInt  DtaMacCStatIsRxBusy(volatile UInt8* pBase, UInt MacBug) {
    return (DtaMacCStatusGet(pBase, MacBug) & DTA_MACC_STAT_RECEIVE_BUSY_MASK) != 0;
}

// Status register: Is Tx busy
static __inline UInt  DtaMacCStatIsTxBusy(volatile UInt8* pBase, UInt MacBug) {
    return (DtaMacCStatusGet(pBase, MacBug) & DTA_MACC_STAT_TRANSMIT_BUSY_MASK) != 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Operation Mode: Access function -.-.-.-.-.-.-.-.-.-.-.-.-.-.
// OperMode: Bit fields
#define DTA_MACC_OPMODE_STARTSTOPRX        (1<<1)
#define DTA_MACC_OPMODE_PROMISCUOUS        (1<<6)
#define DTA_MACC_OPMODE_PASS_ALL_MULTICAST (1<<7)
#define DTA_MACC_OPMODE_ENABLE_FULLDUPLEX  (1<<9)
#define DTA_MACC_OPMODE_STARTSTOPTX        (1<<13)
#define DTA_MACC_OPMODE_ENABLE_1GB         (1<<16)
#define DTA_MACC_OPMODE_STORE_AND_FORWARD  (1<<21)
#define DTA_MACC_OPMODE_RECEIVE_ALL        (1<<30)


// OperMode: Register access
static __inline UInt32  DtaMacCOperModeGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_MACC_OPER_MODE);
}

static __inline void  DtaMacCOperModeSet(volatile UInt8* pBase, UInt32 Value) {
    WRITE_UINT(Value, pBase, DTA_MACC_OPER_MODE);
}

// OperMode: Start/Stop Receive
static __inline void  DtaMacCEnableRx(volatile UInt8* pBase, UInt Enable) {
    UInt32  Val = DtaMacCOperModeGet(pBase);
    if (Enable != 0) Val |=  DTA_MACC_OPMODE_STARTSTOPRX;
    else             Val &= ~DTA_MACC_OPMODE_STARTSTOPRX;
    DtaMacCOperModeSet(pBase, Val);
}

// OperMode: Start/Stop Transmit
static __inline void  DtaMacCEnableTx(volatile UInt8* pBase, UInt Enable) {
    UInt32  Val = DtaMacCOperModeGet(pBase);
    if (Enable != 0) Val |=  DTA_MACC_OPMODE_STARTSTOPTX;
    else             Val &= ~DTA_MACC_OPMODE_STARTSTOPTX;
    DtaMacCOperModeSet(pBase, Val);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Missed frames: Access functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Missed frames: Register access
static __inline UInt32  DtaMacCMissedFramesCounterGet(volatile UInt8* pBase) {
    UInt32  Val = READ_UINT(pBase, DTA_MACC_MISSED_FRAMES_OVF);
    return ((Val >> 17) & 0x7ff) +  // Fifo Overflow Counter
            (Val  & 0xffff);        // Missed frames counter
}

//-.-.-.-.-.-.-.-.-.-.-.- MII Serial Management: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.
// MII SM: Bit fields
#define DTA_MACC_MIISM_START_BUSY_MASK         (0x1 << 31)
#define DTA_MACC_MIISM_OPCODE_READ             (0x2 << 26)
#define DTA_MACC_MIISM_OPCODE_WRITE            (0x1 << 26)
#define DTA_MACC_MIISM_OPCODE_CLKDIV           (0x3 << 26)
#define DTA_MACC_MIISM_CLKDIV_MASK             (7 << 28)
#define DTA_MACC_MIISM_CLKDIV_8                (0 << 28)
#define DTA_MACC_MIISM_CLKDIV_16               (1 << 28)
#define DTA_MACC_MIISM_CLKDIV_32               (2 << 28)
#define DTA_MACC_MIISM_CLKDIV_64               (3 << 28)
#define DTA_MACC_MIISM_CLKDIV_128              (4 << 28)
#define DTA_MACC_MIISM_CLKDIV_256              (5 << 28)
#define DTA_MACC_MIISM_CLKDIV_512              (6 << 28)
#define DTA_MACC_MIISM_CLKDIV_1024             (7 << 28)
#define DTA_MACC_MIISM_PHYADDR                 (1 << 21)
#define DTA_MACC_MIISM_REG_ADDRESS_SHIFT       (16)
#define DTA_MACC_MIISM_REG_ADDRESS_MASK        (0x1F << DTA_MACC_MIISM_REG_ADDRESS_SHIFT)
#define DTA_MACC_MIISM_DATA_MASK               (0xFFFF)

// MII SM: Register Access
static __inline UInt  DtaMacCMiiSmRegGet(volatile UInt8* pBase, UInt MacBug) {
    if (MacBug)
    {
        UInt32  Value = 0;
        while (Value == 0)
            Value = READ_UINT(pBase, DTA_MACC_MII_SERIAL_MANAG);
        return Value;
    }
    return READ_UINT(pBase, DTA_MACC_MII_SERIAL_MANAG);
}

static __inline void  DtaMacCMiiSmRegSet(volatile UInt8* pBase, UInt Val, UInt MacBug) {

    WRITE_UINT(Val, pBase, DTA_MACC_MII_SERIAL_MANAG);
    if (MacBug)
    {
        // Workaround for R/W MAC problem in FPGA
        while ((READ_UINT(pBase, DTA_MACC_MII_SERIAL_MANAG) & 0x0fffffff) 
                                                                    != (Val & 0x0fffffff))
            WRITE_UINT(Val, pBase, DTA_MACC_MII_SERIAL_MANAG);
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.- MacAddress register: Access function -.-.-.-.-.-.-.-.-.-.-.-.-

static __inline void  DtaMacCMacAddressSet(volatile UInt8* pBase, UInt8* pMacAddr, 
                                                                            UInt MacBug) {
    WRITE_UINT(MAC_ADDRESS_LOW(pMacAddr),  pBase, DTA_MACC_MAC_ADDR_LOW);
    WRITE_UINT(MAC_ADDRESS_HIGH(pMacAddr), pBase, DTA_MACC_MAC_ADDR_HIGH);

    if (MacBug)
    {
        // Workaround for R/W MAC problem in FPGA
        while (READ_UINT(pBase, DTA_MACC_MAC_ADDR_LOW) != MAC_ADDRESS_LOW(pMacAddr))
            WRITE_UINT(MAC_ADDRESS_LOW(pMacAddr), pBase, DTA_MACC_MAC_ADDR_LOW);
        while (READ_UINT(pBase, DTA_MACC_MAC_ADDR_HIGH) != MAC_ADDRESS_HIGH(pMacAddr))
            WRITE_UINT(MAC_ADDRESS_HIGH(pMacAddr), pBase, DTA_MACC_MAC_ADDR_HIGH);
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SC Mode: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// SC Mode: Bit fields
#define DTA_MACC_SCM_LOCK_COUNTER      0x8000

// SC Mode: Receive Counters
#define DTA_MACC_SCM_RXFRMOK           0x8100  // #frames Rx'ed ok
#define DTA_MACC_SCM_RXOCTOK           0x8101  // #bytes Rx'ed ok
#define DTA_MACC_SCM_RXUNIOK           0x8102  // #unicast frames Rx'ed ok
#define DTA_MACC_SCM_RXMULTIOK         0x8103  // #multicast frames Rx'ed ok
#define DTA_MACC_SCM_RXBROADOK         0x8104  // #broadcast frames Rx'ed ok
#define DTA_MACC_SCM_RXFRMSIZE0        0x8105  // #Good packets Rx'ed <=63 octets
#define DTA_MACC_SCM_RXFRMSIZE1        0x8106  // #Good packets Rx'ed == 64 octets
#define DTA_MACC_SCM_RXFRMSIZE2        0x8107  // #Good packets Rx'ed 65..127 octets
#define DTA_MACC_SCM_RXFRMSIZE3        0x8108  // #Good packets Rx'ed 128..255 octets
#define DTA_MACC_SCM_RXFRMSIZE4        0x8109  // #Good packets Rx'ed 256..511 octets
#define DTA_MACC_SCM_RXFRMSIZE5        0x810A  // #Good packets Rx'ed 512..1023 octets
#define DTA_MACC_SCM_RXFRMSIZE6        0x810B  // #Good packets Rx'ed 1024..1518 octets
#define DTA_MACC_SCM_RXFRMSIZE7        0x810C  // #Good packets Rx'ed >1518 octest
#define DTA_MACC_SCM_RXPAUSE           0x810D  // #frames Rx'ed ok with MAC pause control
#define DTA_MACC_SCM_RXALIGNERR        0x810E  // #frames Rx'ed with align errors
#define DTA_MACC_SCM_RXFCSERR          0x810F  // #frames Rx'ed not pass the FCS check

// SC Mode: Transmit counters
#define DTA_MACC_SCM_TXFRMOK           0x8120  // #frames Tx'ed ok (broadcasts,
                                                // multicast, unicast included)
#define DTA_MACC_SCM_TXOCTOK           0x8121  // #bytes Tx'ed ok
#define DTA_MACC_SCM_TXUNIOK           0x8122  // #unicast frames Tx'ed ok
#define DTA_MACC_SCM_TXMULTIOK         0x8123  // #multicast frames Tx'ed ok
#define DTA_MACC_SCM_TXBROADOK         0x8124  // #broadcast frames Tx'ed ok
#define DTA_MACC_SCM_COLL0             0x8125  // #frames Tx'ed ok with 0 collisions
#define DTA_MACC_SCM_COLL1             0x8126  // #frames Tx'ed ok with 1 collisions
#define DTA_MACC_SCM_COLL2             0x8127  // #frames Tx'ed ok with 2 collisions
#define DTA_MACC_SCM_COLL3             0x8128  // #frames Tx'ed ok with 3 collisions
#define DTA_MACC_SCM_COLL4             0x8129  // #frames Tx'ed ok with 4 collisions
#define DTA_MACC_SCM_COLL5             0x812A  // #frames Tx'ed ok with 5 collisions
#define DTA_MACC_SCM_COLL6             0x812B  // #frames Tx'ed ok with 6 collisions
#define DTA_MACC_SCM_COLL7             0x812C  // #frames Tx'ed ok with 7 collisions
#define DTA_MACC_SCM_COLL8             0x812D  // #frames Tx'ed ok with 8 collisions
#define DTA_MACC_SCM_COLL9             0x812E  // #frames Tx'ed ok with 9 collisions
#define DTA_MACC_SCM_COLL10            0x812F  // #frames Tx'ed ok with 10 collisions
#define DTA_MACC_SCM_COLL11            0x8130  // #frames Tx'ed ok with 11 collisions
#define DTA_MACC_SCM_COLL12            0x8131  // #frames Tx'ed ok with 12 collisions
#define DTA_MACC_SCM_COLL13            0x8132  // #frames Tx'ed ok with 13 collisions
#define DTA_MACC_SCM_COLL14            0x8133  // #frames Tx'ed ok with 14 collisions
#define DTA_MACC_SCM_COLL15            0x8134  // #frames Tx'ed ok with 15 collisions
#define DTA_MACC_SCM_COLL16            0x8135  // #frames Tx'ed aborted after 16
                                                // collisions
#define DTA_MACC_SCM_TXDEF             0x8136  // #frames Tx'ed ok, no coll, media busy
#define DTA_MACC_SCM_TXPAUSE           0x8137  // #frames Tx'ed ok with pause operand
#define DTA_MACC_SCM_TXLCERR           0x813A  // #frames Tx'ed with late collision
#define DTA_MACC_SCM_TXMACERR          0x813B  // #frames Tx'ed aborted due to underrun
#define DTA_MACC_SCM_TXCSERR           0x813C  // #frames Tx'ed with no carrier sense

// SC Mode: Counter access functions
static __inline void  DtaMacCScModeLockCounters(volatile UInt8* pBase) {
    WRITE_UINT(DTA_MACC_SCM_LOCK_COUNTER, pBase, DTA_MACC_SC_MODE);
}

static __inline UInt  DtaMacCScModeAreCountersLocked(volatile UInt8* pBase) {
    return (READ_UINT(pBase, DTA_MACC_SC_MODE) & DTA_MACC_SCM_LOCK_COUNTER) != 0;
}

static __inline UInt32 DtaMacCScModeGetCounterValue(volatile UInt8* pBase, UInt Register,
                                                                            UInt MacBug) {
    // Set SC mode register to read mode with the register to read
    WRITE_UINT(Register, pBase, DTA_MACC_SC_MODE);

    if (MacBug)
    {
        // Workaround for R/W MAC problem in FPGA
        while (READ_UINT(pBase, DTA_MACC_SC_MODE) != Register) 
        {
            // Set SC mode register to read mode with the register to read
            WRITE_UINT(Register, pBase, DTA_MACC_SC_MODE);
        }
    }

    // Read the value
    return READ_UINT(pBase, DTA_MACC_SC_READ_VALUE);
}

static __inline void DtaMacCScModeUnLockCounters(volatile UInt8* pBase) {
    // Clear the read request
    // Workaround for R/W MAC problem in FPGA
    // A 1 is written in stead of 0, to detect reading a value <> 0
    // by checking the status of the unlocked state.
    WRITE_UINT(0x1, pBase, DTA_MACC_SC_MODE);
}

static __inline UInt  DtaMacCScModeAreCountersUnLocked(volatile UInt8* pBase) {
    // Are counters UnLocked?
    // Workaround for R/W MAC problem in FPGA
    // 0x1 is used in stead of 0. See DtaEthScModeUnLockCounters
    return (READ_UINT(pBase, DTA_MACC_SC_MODE)) == 0x1;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Network Transmit Registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.- Transmit Network Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA_NWTX_CTRL          0x0004
#define DTA_NWTX_STAT          0x0008
#define DTA_NWTX_FIFO_LOAD     0x0018
#define DTA_NWTX_PKT_CNT       0x0030

//-.-.-.-.-.-.-.-.- Network Transmit Control Register: Access Functions -.-.-.-.-.-.-.-.-.
// Bit fields
#define DTA_NWTX_CTRL_TX_ON_TIME_ENA       (1<<0)
// bit 1..18 reserved
#define DTA_NWTX_CTRL_TX_CNT_OVF_INT_EN    (1<<19)
#define DTA_NWTX_CTRL_TX_ERR_INT_EN        (1<<20)
#define DTA_NWTX_CTRL_TX_FIFO_OVF_INT_EN   (1<<21)
// bit 22..23 reserved
#define DTA_NWTX_CTRL_CLR_FIFO             (1<<24)
#define DTA_NWTX_CTRL_RST_FIFO             (1<<25)
#define DTA_NWTX_CTRL_TX_CONTROL           (1<<26)
#define DTA_NWTX_CTRL_SLICE_AVAIL_INT_EN   (1<<27)  // RT, type2
#define DTA_NWTX_CTRL_DMA_ABORT_EN         (1<<28)  // RT, type2
// bit 29 reserved
#define DTA_NWTX_CTRL_DMA_READY_INT_EN     (1<<30)  // RT, type2
// bit 31 reserved

// Set/get  control enable bits
static __inline UInt32  DtaNwTxCtrlGet(volatile UInt8* pBase){
    return READ_UINT(pBase, DTA_NWTX_CTRL);
}

static __inline void  DtaNwTxCtrlSet(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_NWTX_CTRL);
}

static __inline void  DtaNwTxCtrlTxControlSet(volatile UInt8* pBase, UInt TxEna) {
    UInt32  Val = DtaNwTxCtrlGet(pBase);
    if (TxEna)  Val |= DTA_NWTX_CTRL_TX_CONTROL;
    else        Val &= ~DTA_NWTX_CTRL_TX_CONTROL;
    DtaNwTxCtrlSet(pBase, Val);
}

static __inline void  DtaNwTxCtrlTxOnTimeSet(volatile UInt8* pBase, UInt TimeEna) {
    UInt32  Val = DtaNwTxCtrlGet(pBase);
    if (TimeEna) Val |= DTA_NWTX_CTRL_TX_ON_TIME_ENA;
    else         Val &= ~DTA_NWTX_CTRL_TX_ON_TIME_ENA;
    DtaNwTxCtrlSet(pBase, Val);
}

static __inline void  DtaNwTxCtrlClrFifoSet(volatile UInt8* pBase) {
    UInt32  Val = DtaNwTxCtrlGet(pBase);
    Val |= DTA_NWTX_CTRL_CLR_FIFO;
    DtaNwTxCtrlSet(pBase, Val);
}


static __inline void  DtaNwTxCtrlSetDmaAbortEn(volatile UInt8* pBase, UInt IntEna) {
    UInt32  Val = DtaNwTxCtrlGet(pBase);
    if (IntEna) Val |= DTA_NWTX_CTRL_DMA_ABORT_EN;
    else        Val &= ~DTA_NWTX_CTRL_DMA_ABORT_EN;
    DtaNwTxCtrlSet(pBase, Val);
}

static __inline void  DtaNwTxCtrlSetDmaReadyIntEn(volatile UInt8* pBase, UInt IntEna) {
    UInt32  Val = DtaNwTxCtrlGet(pBase);
    if (IntEna) Val |= DTA_NWTX_CTRL_DMA_READY_INT_EN;
    else        Val &= ~DTA_NWTX_CTRL_DMA_READY_INT_EN;
    DtaNwTxCtrlSet(pBase, Val);
}

//.-.-.-.-.-.-.-.-.- Network Transmit Status Register: Access Functions -.-.-.-.-.-.-.-.-.
// Network Transmit Statis: Bit fields
#define DTA_NWTX_STAT_VAL_CNT_OVF_INT      (1<<19)
#define DTA_NWTX_STAT_TX_FIFO_OVF_INT      (1<<21)
#define DTA_NWTX_STAT_TX_FIFO_FULL_INT     (1<<22)

#define DTA_NWTX_STAT_SLICE_AVAIL_INT      (1<<27)  // RT, type2
#define DTA_NWTX_STAT_DMA_TRUNC_INT        (1<<28)  // RT, type2
#define DTA_NWTX_STAT_DMA_LATE_INT         (1<<29)  // RT, type2
#define DTA_NWTX_STAT_DMA_READY_INT        (1<<30)  // RT, type2


// Network Receive Status: Register access
static __inline UInt  DtaNwTxStatGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_NWTX_STAT);
}
static __inline void  DtaNwTxStatSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_NWTX_STAT);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ NETWORK RECEIVE +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Register block for Network Rx
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA_NWRX_CTRL          0x0004
#define DTA_NWRX_STAT          0x0008
#define DTA_NWRX_FIFO_LOAD     0x0018
#define DTA_NWRX_PKT_VAL_CNT   0x0030
#define DTA_NWRX_PKT_INV_CNT   0x0034


//.-.-.-.-.-.-.-.-.- Network Receive Control Register: Access Functions -.-.-.-.-.-.-.-.-.
// Network Receive Control: Bit fields
#define DTA_NWRX_CTRL_DEL_LEN_ERR          (1<<0)
#define DTA_NWRX_CTRL_DEL_CRC_ERR          (1<<1)
#define DTA_NWRX_CTRL_DEL_RX_ERR           (1<<2)
#define DTA_NWRX_CTRL_DEL_DRIB_ERR         (1<<3)
// bit 4..5 reserved
#define DTA_NWRX_CTRL_DEL_COL_ERR          (1<<6)
#define DTA_NWRX_CTRL_DEL_FTL_ERR          (1<<7)
// bit 8..10 reserved
#define DTA_NWRX_CTRL_DEL_RNT_ERR          (1<<11)
// bit 12..15 reserved
#define DTA_NWRX_CTRL_DEL_IPSUM_ERR        (1<<16)
#define DTA_NWRX_CTRL_DEL_UDPSUM_ERR       (1<<17)
#define DTA_NWRX_CTRL_DEL_TCPSUM_ERR       (1<<18)

#define DTA_NWRX_CTRL_DEL_MASK             (DTA_NWRX_CTRL_DEL_LEN_ERR  | \
                                             DTA_NWRX_CTRL_DEL_CRC_ERR  | \
                                             DTA_NWRX_CTRL_DEL_COL_ERR  | \
                                             DTA_NWRX_CTRL_DEL_FTL_ERR  | \
                                             DTA_NWRX_CTRL_DEL_DRIB_ERR | \
                                             DTA_NWRX_CTRL_DEL_RNT_ERR  | \
                                             DTA_NWRX_CTRL_DEL_IPSUM_ERR | \
                                             DTA_NWRX_CTRL_DEL_UDPSUM_ERR | \
                                             DTA_NWRX_CTRL_DEL_TCPSUM_ERR)
/* Temp disable GMII error for Tandberg problem in DTA_NWRX_CTRL_DEL_MASK
    DTA_NWRX_CTRL_DEL_RX_ERR   | \*/

#define DTA_NWRX_CTRL_VAL_CNT_OVF_INT_EN   (1<<19)
#define DTA_NWRX_CTRL_INV_CNT_OVF_INT_EN   (1<<20)
#define DTA_NWRX_CTRL_RX_FIFO_OVF_INT_EN   (1<<21)
// bit 22..23 reserved
#define DTA_NWRX_CTRL_CLR_FIFO             (1<<24)
#define DTA_NWRX_CTRL_RST_FIFO             (1<<25)
#define DTA_NWRX_CTRL_RX_CONTROL           (1<<26)
#define DTA_NWRX_CTRL_SLICE_INT_EN         (1<<27)
// bit 28..31 reserved

// Network Receive Control: Register access
static __inline void  DtaNwRxCtrlSet(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_NWRX_CTRL);
}
static __inline UInt32  DtaNwRxCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_NWRX_CTRL);
}

// Network Receive Control: Rx Control
static __inline void  DtaNwRxCtrlRxControlSet(volatile UInt8* pBase, UInt RxEna) {
    UInt32  Val = DtaNwRxCtrlGet(pBase);
    if (RxEna)  Val |= DTA_NWRX_CTRL_RX_CONTROL;
    else        Val &= ~DTA_NWRX_CTRL_RX_CONTROL;
    DtaNwRxCtrlSet(pBase, Val);
}

// Network Receive Control: Rx Fifo overflow int. enable
static __inline void  DtaNwRxCtrlSetRxFifoOvfIntEn(volatile UInt8* pBase, UInt IntEna) {
    UInt32  Val = DtaNwRxCtrlGet(pBase);
    if (IntEna)  Val |= DTA_NWRX_CTRL_RX_FIFO_OVF_INT_EN;
    else        Val &= ~DTA_NWRX_CTRL_RX_FIFO_OVF_INT_EN;
    DtaNwRxCtrlSet(pBase, Val);
}

// Network Receive Control: Rx Fifo clear
static __inline void  DtaNwRxCtrlSetClrFifo(volatile UInt8* pBase) {
    UInt32  Val = DtaNwRxCtrlGet(pBase);
    Val |= DTA_NWRX_CTRL_CLR_FIFO;
    DtaNwRxCtrlSet(pBase, Val);
}

// Network Receive Control: Rx set deletion packets with deletion mask
static __inline void  DtaNwRxCtrlSetDelPckFpga(volatile UInt8* pBase, UInt EnaDel) {
    UInt32  Val = DtaNwRxCtrlGet(pBase);
    if (EnaDel) Val |= DTA_NWRX_CTRL_DEL_MASK;
    else        Val &= ~DTA_NWRX_CTRL_DEL_MASK;
    DtaNwRxCtrlSet(pBase, Val);
}

// Network Receive Control: Rx set deletion packets length error
static __inline void  DtaNwRxCtrlSetDelLenPckFpga(volatile UInt8* pBase) {
    UInt32  Val = DtaNwRxCtrlGet(pBase);
    Val |= DTA_NWRX_CTRL_DEL_LEN_ERR;
    DtaNwRxCtrlSet(pBase, Val);
}

// Network Receive Control: Set slice interrupt enable
static __inline void  DtaNwRxCtrlSetSliceIntEn(volatile UInt8* pBase, UInt IntEna) {
    UInt32  Val = DtaNwRxCtrlGet(pBase);
    if (IntEna) Val |= DTA_NWRX_CTRL_SLICE_INT_EN;
    else        Val &= ~DTA_NWRX_CTRL_SLICE_INT_EN;
    DtaNwRxCtrlSet(pBase, Val);
}

//.-.-.-.-.-.-.-.-.- Network Receive Status Register: Access Functions -.-.-.-.-.-.-.-.-.-
// Network Receive Statis: Bit fields
#define DTA_NWRX_STAT_VAL_CNT_OVF_INT      (1<<19)
#define DTA_NWRX_STAT_INV_CNT_OVF_INT      (1<<20)
#define DTA_NWRX_STAT_RX_FIFO_OVF_INT      (1<<21)

#define DTA_NWRX_STAT_SLICE_AVAIL_INT      (1<<27)
#define DTA_NWRX_STAT_SLICE_OVF_INT        (1<<28)

// Network Receive Status: Register access
static __inline UInt  DtaNwRxStatGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_NWRX_STAT);
}
static __inline void  DtaNwRxStatSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_NWRX_STAT);
}

//.-.-.-.-.- Network Receive Invalid Packet Counter Register: Access Functions -.-.-.-.-.-

// Network Receive Invalid Packet Counter: Register access
static __inline UInt32  DtaNwRxInvCntGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_NWRX_PKT_INV_CNT);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Address Matcher +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Register block for Address Matcher
// DTA-2162
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA_ADDR_MATCH_CTRL              0x0004
#define DTA_ADDR_MATCH_STAT              0x0008
#define DTA_ADDR_MATCH_MAC_ADDR_LOW      0x0010
#define DTA_ADDR_MATCH_MAC_ADDR_HIGH     0x0014

#define DTA_MACADDR_FILTER_CTRL          0x0044
#define DTA_MACADDR_FILTER_STAT          0x0048

//.-.-.-.-.-.-.-.-.- Address Matcher Control Register: Access Functions -.-.-.-.-.-.-.-.-.
#define DTA_ADDR_MATCH_CTRL_PROM_DIS     (1<<1)
#define DTA_ADDR_MATCH_CTRL_LUTUPD       (1<<2)
#define DTA_ADDR_MATCH_CTRL_DELFILT      (1<<4)
#define DTA_ADDR_MATCH_CTRL_IPRXOVERL_EN (1<<20)

static __inline void  DtaAddrMatchCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_ADDR_MATCH_CTRL);
}

static __inline UInt  DtaAddrMatchCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_ADDR_MATCH_CTRL);
}

static __inline void  DtaAddrMatchPromDis(volatile UInt8* pBase, UInt Dis) {
    UInt32  Val = DtaAddrMatchCtrlGet(pBase);
    if (Dis) Val |= DTA_ADDR_MATCH_CTRL_PROM_DIS;
    else    Val &= ~DTA_ADDR_MATCH_CTRL_PROM_DIS;
    DtaAddrMatchCtrlSet(pBase, Val);
}

static __inline void  DtaAddrMatchLutUpdate(volatile UInt8* pBase) {
    UInt32  Val = DtaAddrMatchCtrlGet(pBase);
    Val |= DTA_ADDR_MATCH_CTRL_LUTUPD;
    DtaAddrMatchCtrlSet(pBase, Val);
}

//.-.-.-.-.-.-.-.-.- Address Matcher Status Register: Access Functions -.-.-.-.-.-.-.-.-.-
#define DTA_ADDR_MATCH_STAT_LUTSEL       (1<<2)
#define DTA_ADDR_MATCH_STAT_LUTUPDATPEND (1<<3)
#define DTA_ADDR_MATCH_STAT_FILTOFLINT   (1<<20)

static __inline void  DtaAddrMatchStatSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_ADDR_MATCH_STAT);
}

static __inline UInt  DtaAddrMatchStatGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_ADDR_MATCH_STAT);
}

static __inline UInt32  DtaAddrMatchStatGetLutSel(volatile UInt8* pBase) {
    return  (DtaAddrMatchStatGet(pBase) & DTA_ADDR_MATCH_STAT_LUTSEL) >> 2;
}

static __inline UInt32  DtaAddrMatchStatGetLutUpdatePending(volatile UInt8* pBase) {
    return  (DtaAddrMatchStatGet(pBase) & DTA_ADDR_MATCH_STAT_LUTUPDATPEND) >> 3;
}

//.-.-.-.-.-.-.-.- Address Matcher MAC-Address Register: Access Functions -.-.-.-.-.-.-.-.
static __inline void  DtaAddrMatchMacAddressSet(volatile UInt8* pBase, UInt8* MacAddr){
    WRITE_UINT(MAC_ADDRESS_LOW(MacAddr),  pBase, DTA_ADDR_MATCH_MAC_ADDR_LOW);
    WRITE_UINT(MAC_ADDRESS_HIGH(MacAddr), pBase, DTA_ADDR_MATCH_MAC_ADDR_HIGH);
}

//-.-.-.-.-.-.- Address Matcher MAC-Address Filter Control: Access Functions -.-.-.-.-.-.-
#define DTA_MACADDR_FILTER_CTRL_EN               (1<<0)
#define DTA_MACADDR_FILTER_CTRL_LUTUPD           (1<<2)
#define DTA_MACADDR_FILTER_CTRL_DELFILT          (1<<4)
#define DTA_MACADDR_FILTER_CTRL_FILTCNTOVERL_EN  (1<<20)

static __inline void  DtaMacAddrFilterCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_MACADDR_FILTER_CTRL);
}

static __inline UInt  DtaMacAddrFilterCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_MACADDR_FILTER_CTRL);
}

static __inline void  DtaMacAddrFilterEnable(volatile UInt8* pBase, UInt En) {
    UInt32  Val = DtaMacAddrFilterCtrlGet(pBase);
    if (En) Val |= DTA_MACADDR_FILTER_CTRL_EN;
    else    Val &= ~DTA_MACADDR_FILTER_CTRL_EN;
    DtaMacAddrFilterCtrlSet(pBase, Val);
}

static __inline void  DtaMacAddrFilterLutUpdate(volatile UInt8* pBase) {
    UInt32  Val = DtaMacAddrFilterCtrlGet(pBase);
    Val |= DTA_MACADDR_FILTER_CTRL_LUTUPD;
    DtaMacAddrFilterCtrlSet(pBase, Val);
}

static __inline void  DtaMacAddrFilterDelFilteredEn(volatile UInt8* pBase, UInt En) {
    UInt32  Val = DtaMacAddrFilterCtrlGet(pBase);
    if (En) Val |= DTA_MACADDR_FILTER_CTRL_DELFILT;
    else    Val &= ~DTA_MACADDR_FILTER_CTRL_DELFILT;
    DtaMacAddrFilterCtrlSet(pBase, Val);
}

//-.-.-.-.-.-.- Address Matcher MAC-Address Filter Status: Access Functions -.-.-.-.-.-.-.
#define DTA_MACADDR_FILTER_STAT_LUTSEL       (1<<2)
#define DTA_MACADDR_FILTER_LUTUPDATPEND      (1<<3)
#define DTA_MACADDR_FILTER_STAT_FILTOFLINT   (1<<20)

static __inline void  DtaMacAddrFilterStatSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_MACADDR_FILTER_STAT);
}

static __inline UInt  DtaMacAddrFilterStatGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_MACADDR_FILTER_STAT);
}

static __inline UInt32  DtaMacAddrFilterStatGetLutSel(volatile UInt8* pBase) {
    return (DtaMacAddrFilterStatGet(pBase) & DTA_MACADDR_FILTER_STAT_LUTSEL) >> 2;
}

static __inline UInt32  DtaMacAddrFilterStatGetLutUpdatePending(volatile UInt8* pBase) {
    return (DtaMacAddrFilterStatGet(pBase) & DTA_MACADDR_FILTER_LUTUPDATPEND) >> 3;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Address Matcher Lookup +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Register block for Address Matcher Lookup
// DTA-2162
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA_ADDRM_LOOKUP_CTRL            0x0010
#define DTA_ADDRM_LOOKUP_ADDR            0x0014
#define DTA_ADDRM_LOOKUP_DATA0           0x0020
#define DTA_ADDRM_LOOKUP_DATA1           0x0024
#define DTA_ADDRM_LOOKUP_DATA2           0x0028
#define DTA_ADDRM_LOOKUP_DATA3           0x002C
#define DTA_ADDRM_LOOKUP_DATA4           0x0030
#define DTA_ADDRM_LOOKUP_DATA5           0x0034

#define DTA_MACADDR_LOOKUP_CRTL          0x0050
#define DTA_MACADDR_LOOKUP_ADDR          0x0054
#define DTA_MACADDR_LOOKUP_DATA0         0x0060
#define DTA_MACADDR_LOOKUP_DATA1         0x0064

//-.-.-.-.-.-.-.-.-.- Address Matcher Lookup Control: Access Functions -.-.-.-.-.-.-.-.-.-
// Address Matcher Lookup: Control
#define DTA_ADDRM_LOOKUP_CTRL_NUMENTRY_TBL0  (0xfff)
#define DTA_ADDRM_LOOKUP_CTRL_NUMENTRY_TBL1  (0xfff<<12)

static __inline UInt32  DtaAddrMLookupCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_ADDRM_LOOKUP_CTRL);
}
static __inline void  DtaAddrMLookupCtrlSet(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_ADDRM_LOOKUP_CTRL);
}

static __inline void  DtaAddrMLookupCtrlSetNumEntryTbl0(volatile UInt8* pBase, UInt32 NumEntries) {
    UInt32  Val = DtaAddrMLookupCtrlGet(pBase);
    Val &= ~DTA_ADDRM_LOOKUP_CTRL_NUMENTRY_TBL0;
    Val |= NumEntries;
    DtaAddrMLookupCtrlSet(pBase, Val);
}

static __inline UInt32  DtaAddrMLookupCtrlGetNumEntryTbl0(volatile UInt8* pBase) {
    return  DtaAddrMLookupCtrlGet(pBase) & DTA_ADDRM_LOOKUP_CTRL_NUMENTRY_TBL0;
}

static __inline void  DtaAddrMLookupCtrlSetNumEntryTbl1(volatile UInt8* pBase, 
                                                                      UInt32 NumEntries) {
    UInt32  Val = DtaAddrMLookupCtrlGet(pBase);
    Val &= ~DTA_ADDRM_LOOKUP_CTRL_NUMENTRY_TBL1;
    Val |= NumEntries<<12;
    DtaAddrMLookupCtrlSet(pBase, Val);
}

static __inline UInt32  DtaAddrMLookupCtrlGetNumEntryTbl1(volatile UInt8* pBase) {
    return  (DtaAddrMLookupCtrlGet(pBase) & DTA_ADDRM_LOOKUP_CTRL_NUMENTRY_TBL1) >> 12;
}

//.-.-.-.-.-.-.-.- Address Matcher Lookup Table Address: Access Functions -.-.-.-.-.-.-.-.
static __inline UInt32  DtaAddrMLookupAddrGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_ADDRM_LOOKUP_ADDR);
}
static __inline void  DtaAddrMLookupAddrSet(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_ADDRM_LOOKUP_ADDR);
}

//-.-.-.-.-.-.-.-.- Address Matcher Lookup Table Data: Access Functions -.-.-.-.-.-.-.-.-.
static __inline void  DtaAddrMLookupData0Set(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_ADDRM_LOOKUP_DATA0);
}
static __inline void  DtaAddrMLookupData1Set(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_ADDRM_LOOKUP_DATA1);
}
static __inline void  DtaAddrMLookupData2Set(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_ADDRM_LOOKUP_DATA2);
}
static __inline void  DtaAddrMLookupData3Set(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_ADDRM_LOOKUP_DATA3);
}
static __inline void  DtaAddrMLookupData4Set(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_ADDRM_LOOKUP_DATA4);
}
static __inline void  DtaAddrMLookupData5Set(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_ADDRM_LOOKUP_DATA5);
}

//-.-.-.-.-.-.-.-.-.-.- MAC-Address Lookup Control: Access Functions -.-.-.-.-.-.-.-.-.-.-
#define DTA_MACADDR_LOOKUP_CTRL_NUMENTRY_PHY0  (0x01f)
#define DTA_MACADDR_LOOKUP_CTRL_NUMENTRY_PHY1  (0x01f<<16)

static __inline UInt32  DtaMacAddrLookupCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_MACADDR_LOOKUP_CRTL);
}
static __inline void  DtaMacAddrLookupCtrlSet(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_MACADDR_LOOKUP_CRTL);
}

static __inline void  DtaMacAddrLookupCtrlSetNumEntryPhy0(volatile UInt8* pBase,
                                                                      UInt32 NumEntries) {
    UInt32  Val = DtaMacAddrLookupCtrlGet(pBase);
    Val &= ~DTA_MACADDR_LOOKUP_CTRL_NUMENTRY_PHY0;
    Val |= NumEntries;
    DtaMacAddrLookupCtrlSet(pBase, Val);
}

static __inline UInt32  DtaMacAddrLookupCtrlGetNumEntryPhy0(volatile UInt8* pBase) {
    return  DtaMacAddrLookupCtrlGet(pBase) & DTA_MACADDR_LOOKUP_CTRL_NUMENTRY_PHY0;
}

static __inline void  DtaMacAddrLookupCtrlSetNumEntryPhy1(volatile UInt8* pBase,
                                                                      UInt32 NumEntries) {
    UInt32  Val = DtaMacAddrLookupCtrlGet(pBase);
    Val &= ~DTA_MACADDR_LOOKUP_CTRL_NUMENTRY_PHY1;
    Val |= NumEntries<<16;
    DtaMacAddrLookupCtrlSet(pBase, Val);
}

static __inline UInt32  DtaMacAddrLookupCtrlGetNumEntryPhy1(volatile UInt8* pBase) {
    return  (DtaMacAddrLookupCtrlGet(pBase) & 
                                             DTA_MACADDR_LOOKUP_CTRL_NUMENTRY_PHY1) >> 16;
}

//.-.-.-.-.-.-.-.-.- MAC-Address Lookup Table Address: Access Functions -.-.-.-.-.-.-.-.-.
#define DTA_MACADDR_LOOKUP_ADDR_PHY1      (1<<31)

static __inline UInt32  DtaMacAddrLookupAddrGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_MACADDR_LOOKUP_ADDR);
}
static __inline void  DtaMacAddrLookupAddrSet(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_MACADDR_LOOKUP_ADDR);
}

//-.-.-.-.-.-.-.-.-.- MAC-Address Lookup Table Data: Access Functions -.-.-.-.-.-.-.-.-.-.
static __inline void  DtaMacAddrLookupData0Set(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_MACADDR_LOOKUP_DATA0);
}
static __inline void  DtaMacAddrLookupData1Set(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_MACADDR_LOOKUP_DATA1);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Frame Sorter +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Register block for Frame Sorter
// DTA-2162
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA_FRAME_SRT_CTRL          0x0004
#define DTA_FRAME_SRT_STATUS        0x0008

//.-.-.-.-.-.-.-.-.-.-.-.- Frame Sorter Control: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.
#define DTA_FRAME_SRT_CTRL_EN       0x1
#define DTA_FRAME_SRT_CTRL_INT_EN   (0x1<<3)

static __inline UInt32  DtaFrameSrtCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_FRAME_SRT_CTRL);
}
static __inline void  DtaFrameSrtCtrlSet(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_FRAME_SRT_CTRL);
}

static __inline void  DtaFrameSrtCtrlEn(volatile UInt8* pBase, UInt32 Enable) {
    UInt32  Val = DtaFrameSrtCtrlGet(pBase);
    if (Enable) Val |= DTA_FRAME_SRT_CTRL_EN;
    else        Val &= ~DTA_FRAME_SRT_CTRL_EN;
    DtaFrameSrtCtrlSet(pBase, Val);
}

static __inline void  DtaFrameSrtCtrlIntEn(volatile UInt8* pBase, UInt32 Enable) {
    UInt32  Val = DtaFrameSrtCtrlGet(pBase);
    if (Enable) Val |= DTA_FRAME_SRT_CTRL_INT_EN;
    else        Val &= ~DTA_FRAME_SRT_CTRL_INT_EN;
    DtaFrameSrtCtrlSet(pBase, Val);
}

//.-.-.-.-.-.-.-.-.-.-.-.- Frame Sorter Status: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA_FRAME_SRT_STATUS_BUSY_SORT    0x1
#define DTA_FRAME_SRT_STATUS_BUSY_WRITE   (0x1<<1)
#define DTA_FRAME_SRT_STATUS_CUR_CHAN     (0x1<<2)
#define DTA_FRAME_SRT_STATUS_SRT_INT      (0x1<<3)

static __inline UInt32  DtaFrameSrtStatusGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_FRAME_SRT_STATUS);
}
static __inline void  DtaFrameSrtStatusSet(volatile UInt8* pBase, UInt32 Val) {
    WRITE_UINT(Val, pBase, DTA_FRAME_SRT_STATUS);
}

static __inline UInt  DtaFrameSrtStatusBusySrtGet(volatile UInt8* pBase) {
    return ((DtaFrameSrtStatusGet(pBase)&DTA_FRAME_SRT_STATUS_BUSY_SORT)!=0?1:0);
}

static __inline UInt  DtaFrameSrtStatusBusyWriteGet(volatile UInt8* pBase) {
    return ((DtaFrameSrtStatusGet(pBase)&DTA_FRAME_SRT_STATUS_BUSY_WRITE)!=0?1:0);
}

static __inline UInt  DtaFrameSrtStatusCurChanGet(volatile UInt8* pBase) {
    return ((DtaFrameSrtStatusGet(pBase)&DTA_FRAME_SRT_STATUS_CUR_CHAN)!=0?1:0);
}

static __inline UInt  DtaFrameSrtStatusSrtIntGet(volatile UInt8* pBase) {
    return ((DtaFrameSrtStatusGet(pBase)&DTA_FRAME_SRT_STATUS_SRT_INT)!=0?1:0);
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Memory Controller +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Register block for Memory Controller
// DTA-2162
//
#define DTA_MEM_NRT_RX_BUF_START         0x000
#define DTA_MEM_NRT_RX_BUF_END           0x004
#define DTA_MEM_RT_RX_BUF_START          0x008
#define DTA_MEM_RT_RX_BUF_END            0x00C
#define DTA_MEM_NRT_TX_BUF_START         0x010
#define DTA_MEM_NRT_TX_BUF_END           0x014
#define DTA_MEM_RT_TX_BUF_START          0x018
#define DTA_MEM_RT_TX_BUF_END            0x01C
#define DTA_MEM_PKT_ORDER_BUF_START      0x020
#define DTA_MEM_PKT_ORDER_BUF_END        0x024

static __inline UInt  DtaMemGetPointer(volatile UInt8* pBase, UInt Offset) {
    return READ_UINT(pBase, Offset);
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Memory Tester +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// 
// Register block for memory tester
// Dta-2162
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTA_MEMTST_CTRL                0x0004
#define  DTA_MEMTST_STAT                0x0008
#define  DTA_MEMTST_ADDRESS             0x0010
#define  DTA_MEMTST_DATA                0x0014

//-.-.-.-.-.-.-.-.-.-.-.-  Memory Tester Control: Access Functions -.-.-.-.-.-.-.-.-.-.-.-
//
#define  DTA_MEMTST_CTRL_NUMREPEATS     0x000000FF
#define  DTA_MEMTST_CTRL_START          0x00000100
#define  DTA_MEMTST_CTRL_STOP           0x00000200
#define  DTA_MEMTST_CTRL_RUNWALKONE     0x00010000
#define  DTA_MEMTST_CTRL_RUNWALKZERO    0x00020000
#define  DTA_MEMTST_CTRL_RUNALLONE      0x00040000
#define  DTA_MEMTST_CTRL_RUNALLZERO     0x00080000
#define  DTA_MEMTST_CTRL_RUNAAAA        0x00100000
#define  DTA_MEMTST_CTRL_RUN5555        0x00200000
#define  DTA_MEMTST_CTRL_RUNAA55        0x00400000
#define  DTA_MEMTST_CTRL_RUN55AA        0x00800000
#define  DTA_MEMTST_CTRL_RUNCOUNT       0x01000000
#define  DTA_MEMTST_CTRL_RUNALL         0x01FF0000

static __inline UInt  DtaMemTstCtrlGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_MEMTST_CTRL);
}
static __inline void DtaMemTstCtrlSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_MEMTST_CTRL);
}

static __inline void  DtaMemTstCtrlNumRepeatsSet(volatile UInt8* pBase, UInt NumRepeats) {
    UInt  Val = DtaMemTstCtrlGet(pBase);
    Val &= ~DTA_MEMTST_CTRL_NUMREPEATS;
    Val |= (NumRepeats & DTA_MEMTST_CTRL_NUMREPEATS);
    DtaMemTstCtrlSet(pBase, Val);
}

static __inline void  DtaMemTstCtrlEnableTestSet(volatile UInt8* pBase, UInt Tests) {
    UInt  Val = DtaMemTstCtrlGet(pBase);
    Val &= ~DTA_MEMTST_CTRL_RUNALL;
    Val |= (Tests & DTA_MEMTST_CTRL_RUNALL);
    DtaMemTstCtrlSet(pBase, Val);
}

static __inline void  DtaMemTstCtrlStartTestSet(volatile UInt8* pBase) {
    UInt  Val = DtaMemTstCtrlGet(pBase);
    Val |= DTA_MEMTST_CTRL_START;
    DtaMemTstCtrlSet(pBase, Val);
}

static __inline void  DtaMemTstCtrlStopTestSet(volatile UInt8* pBase) {
    UInt  Val = DtaMemTstCtrlGet(pBase);
    Val |= DTA_MEMTST_CTRL_STOP;
    DtaMemTstCtrlSet(pBase, Val);
}

//.-.-.-.-.-.-.-.-.-.-.-.- Memory Tester Status: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.
#define  DTA_MEMTST_STAT_NUMITERATIONS   0x000000FF
#define  DTA_MEMTST_STAT_TESTBUSY        0x00000100
#define  DTA_MEMTST_STAT_TESTERROR       0x00000200
#define  DTA_MEMTST_STAT_TESTOK          0x00000400
#define  DTA_MEMTST_STAT_TESTWALKONE     0x00010000
#define  DTA_MEMTST_STAT_TESTWALKZERO    0x00020000
#define  DTA_MEMTST_STAT_TESTALLONE      0x00040000
#define  DTA_MEMTST_STAT_TESTALLZERO     0x00080000
#define  DTA_MEMTST_STAT_TESTAAAA        0x00100000
#define  DTA_MEMTST_STAT_TEST5555        0x00200000
#define  DTA_MEMTST_STAT_TESTAA55        0x00400000
#define  DTA_MEMTST_STAT_TEST55AA        0x00800000
#define  DTA_MEMTST_STAT_TESTCOUNT       0x01000000
#define  DTA_MEMTST_STAT_TESTALL         0x01FF0000

static __inline UInt  DtaMemTstStatGet(volatile UInt8* pBase) {
    return READ_UINT(pBase, DTA_MEMTST_STAT);
}
static __inline void DtaMemTstStatSet(volatile UInt8* pBase, UInt Val) {
    WRITE_UINT(Val, pBase, DTA_MEMTST_STAT);
}

static __inline UInt  DtaMemTstStatNumIterationsGet(volatile UInt8* pBase) {
    return DtaMemTstStatGet(pBase) & DTA_MEMTST_STAT_NUMITERATIONS;
}

static __inline UInt  DtaMemTstStatIsTestBusy(volatile UInt8* pBase) {
    return ((DtaMemTstStatGet(pBase)&DTA_MEMTST_STAT_TESTBUSY)!=0?1:0);
}

static __inline UInt  DtaMemTstStatIsTestError(volatile UInt8* pBase) {
    return ((DtaMemTstStatGet(pBase)&DTA_MEMTST_STAT_TESTERROR)!=0?1:0);
}

static __inline UInt  DtaMemTstStatIsTestOk(volatile UInt8* pBase) {
    return ((DtaMemTstStatGet(pBase)&DTA_MEMTST_STAT_TESTOK)!=0?1:0);
}

static __inline UInt  DtaMemTstStatTestResultGet(volatile UInt8* pBase) {
    return DtaMemTstStatGet(pBase)&DTA_MEMTST_STAT_TESTALL;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Miscellaneous +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Locall address offset DMA fifo of a channel
#define DTA_LOCALADDR_FIFODATA         0x40



//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Demodulator Registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Register blocks DTA-2131 specific
//

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ ADC +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// ADC register block offsets
//
#define  DTA_ADC_REG_ADCCTRL            0x0000
#define  DTA_ADC_REG_STEPSIZES          0x000C
#define  DTA_ADC_REG_TWINDOW            0x0010
#define  DTA_ADC_REG_OFLOWTHRES         0x0014
#define  DTA_ADC_REG_WORKTHRES          0x0018
#define  DTA_ADC_REG_MANGAIN            0x001C
#define  DTA_ADC_REG_MSWINDOW           0x0020
#define  DTA_ADC_REG_MEANSQUARE         0x0024
#define  DTA_ADC_REG_OFFSWINDOW         0x0028
#define  DTA_ADC_REG_RBOFFSET           0x002C
#define  DTA_ADC_REG_SPICMD             0x0030
#define  DTA_ADC_REG_SPIDATA            0x0034
#define  DTA_ADC_REG_STATUS             0x0040
#define  DTA_ADC_REG_RBGAIN             0x0044

//-.-.-.-.-.-.-.-.-.-.-.-.-.- ADC-Control register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTA_ADCCTRL_CAPENA_MSK         0x00000001
#define  DTA_ADCCTRL_CAPENA_SH          0
#define  DTA_ADCCTRL_GCENA_MSK          0x00000010
#define  DTA_ADCCTRL_GCENA_SH           4
#define  DTA_ADCCTRL_TESTENA_MSK        0x00000100
#define  DTA_ADCCTRL_TESTENA_SH         8
#define  DTA_ADCCTRL_OCENA_MSK          0x00001000
#define  DTA_ADCCTRL_OCENA_SH           12

//-.-.-.-.-.-.-.-.-.-.-.-.- ADC Step Sizes register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTA_ADCSTEPSIZE_UNLSTEP_MSK    0x000000FF
#define  DTA_ADCSTEPSIZE_UNLSTEP_SH     0
#define  DTA_ADCSTEPSIZE_LCKSTEP_MSK    0x00FF0000
#define  DTA_ADCSTEPSIZE_LCKSTEP_SH     16

//-.-.-.-.-.-.-.-.-.-.-.-.- ADC SPI command register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTA_ADCSPICMD_ADDRESS_MSK      0x000000FF
#define  DTA_ADCSPICMD_ADDRESS_SH       0
#define  DTA_ADCSPICMD_BUSY_MSK         0x00000100
#define  DTA_ADCSPICMD_BUSY_SH          8

//.-.-.-.-.-.-.-.-.-.-.-.-.- ADC SPI data register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTA_ADCSPIDTA_DATA_MSK         0x000000FF
#define  DTA_ADCSPIDTA_DATA_SH          0

//-.-.-.-.-.-.-.-.-.-.-.-.-.- ADC Status register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTA_ADCSTAT_LOCK_MSK           0x00000001
#define  DTA_ADCSTAT_LOCK_SH            0


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Receiver Channel LED +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// RX LED register block offsets
//
#define DTA_RXLED_REG_LEDCTRL           0x0000

//-.-.-.-.-.-.-.-.-.-.-.-.- Rx LED Control register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.
#define DTA_RXLEDCTRL_FLASHTYPE_MSK     0x00060000
#define DTA_RXLEDCTRL_FLASHTYPE_SH      17
#define DTA_RXLEDCTRL_GREEN             0x00100000
#define DTA_RXLEDCTRL_GREEN_MSK         0x00100000
#define DTA_RXLEDCTRL_GREEN_SH          20
#define DTA_RXLEDCTRL_RED               0x00200000
#define DTA_RXLEDCTRL_RED_MSK           0x00200000
#define DTA_RXLEDCTRL_RED_SH            21


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Sample Rate Converter  +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Sample rate converter register block offsets
// 
#define  DTA_SRC_REG_SRCCTRL            0x0000
#define  DTA_SRC_REG_RATEFRAC           0x0010
#define  DTA_SRC_REG_GAIN               0x0014

//.-.-.-.-.-.-.-.-.- Sample rate converter control register: Bit fields -.-.-.-.-.-.-.-.-.
#define  DTA_SRCCTRL_SRCENA_MSK         0x00000001
#define  DTA_SRCCTRL_SRCENA_SH          0

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IQ demodulator +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// IQ demodulator register block offsets
// 
#define  DTA_IQD_REG_IQDCTRL            0x0000
#define  DTA_IQD_REG_IFFREQ             0x0004
#define  DTA_IQD_REG_SCALEFAC           0x0008
#define  DTA_IQD_REG_COEFFCTRL          0x0010
#define  DTA_IQD_REG_COEFFDATA          0x0014

//-.-.-.-.-.-.-.-.-.-.- IQ demodulator control register: Bit fields -.-.-.-.-.-.-.-.-.-.-.
#define  DTA_IQDCTRL_BYPASS_MSK         0x00000001
#define  DTA_IQDCTRL_BYPASS_SH          0
#define  DTA_IQDCTRL_TESTENA_MSK        0x00000010
#define  DTA_IQDCTRL_TESTENA_SH         4

//-.-.-.-.-.-.-.-.-.- IQ demodulator scale factor register: Bit fields -.-.-.-.-.-.-.-.-.-
#define  DTA_IQDSF_SCALEFACTOR_MSK      0x0000FFFF
#define  DTA_IQDSF_SCALEFACTOR_SH       0
#define  DTA_IQDSF_SCALEENA_MSK         0x00010000
#define  DTA_IQDSF_SCALEENA_SH          16

//.-.-.-.-.-.- IQ demodulator coefficient control/status register: Bit fields -.-.-.-.-.-.
#define  DTA_IQDCOEFF_READY_MSK         0x00000001
#define  DTA_IQDCOEFF_READY_SH          0
#define  DTA_IQDCOEFF_START_MSK         0x00000002
#define  DTA_IQDCOEFF_START_SH          1
#define  DTA_IQDCOEFF_CLRFIFO_MSK       0x00000004
#define  DTA_IQDCOEFF_CLRFIFO_SH        2  
#define  DTA_IQDCOEFF_RDNWR_MSK         0x00000008
#define  DTA_IQDCOEFF_RDNWR_SH          3
#define  DTA_IQDCOEFF_FLTSEL_MSK        0x00000070
#define  DTA_IQDCOEFF_FLTSEL_SH         4

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ RF Power Measurement +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// RF power measurement register block offsets
// 
#define  DTA_RPM_REG_RPMCTRL            0x0000
#define  DTA_RPM_REG_MEASURE            0x0004
#define  DTA_RPM_REG_STATUS             0x0008

//.-.-.-.-.-.-.-.-.- RF Power Measurement Control Register: Bit fields  -.-.-.-.-.-.-.-.-.
// RF Power Measurement control bit defines
#define  DTA_RPMCTRL_MEASENA_MSK        0x00000001
#define  DTA_RPMCTRL_MEASENA_SH         0
#define  DTA_RPMCTRL_MEASHLT_MSK        0x00000002
#define  DTA_RPMCTRL_MEASHLT_SH         1
#define  DTA_RPMCTRL_K_MSK              0x000F0000
#define  DTA_RPMCTRL_K_SH               16
#define  DTA_RPMCTRL_TEMPWIN_MSK        0x00F00000
#define  DTA_RPMCTRL_TEMPWIN_SH         20
#define  DTA_RPMCTRL_IDLEWIN_MSK        0xFF000000
#define  DTA_RPMCTRL_IDLEWIN_SH         24

//.-.-.-.-.-.-.-.-.- RF Power Measurement Measure Register: Bit fields  -.-.-.-.-.-.-.-.-.-
#define  DTA_RPMMEAS_POWER_MSK          0x000000FF
#define  DTA_RPMMEAS_POWER_SH           0
#define  DTA_RPMMEAS_TEMP_MSK           0x00FF0000
#define  DTA_RPMMEAS_TEMP_SH            16


//.-.-.-.-.-.-.-.-.- RF Power Measurement Status Register: Bit fields  -.-.-.-.-.-.-.-.-.-
#define  DTA_RPMSTAT_BUSY_MSK           0x00000001
#define  DTA_RPMSTAT_BUSY_SH            0


#define  DTA2131_RPM_REG_RPMCTRL        0x0600  // DTA-2131 RF power measurement control

// RF Power Measurement control: Enable Measurement (TODOTD this is DTA-2131 specific)
static __inline UInt  DtaRegRpmCtrlGetEnaMeas(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase+DTA2131_RPM_REG_RPMCTRL, DTA_RPM_REG_RPMCTRL, 
                                         DTA_RPMCTRL_MEASENA_MSK, DTA_RPMCTRL_MEASENA_SH);
}
static __inline void  DtaRegRpmCtrlSetEnaMeas(volatile UInt8* pBase, UInt Ena) {
    WRITE_UINT_MASKED(Ena, pBase+DTA2131_RPM_REG_RPMCTRL, DTA_RPM_REG_RPMCTRL,
                                         DTA_RPMCTRL_MEASENA_MSK, DTA_RPMCTRL_MEASENA_SH);
}

// RF Power Measurement control: Pause Measurement (e.g. DTA-2131)
static __inline UInt  DtaRegRpmCtrlGetHaltMeas(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase+DTA2131_RPM_REG_RPMCTRL, DTA_RPM_REG_RPMCTRL,
                                         DTA_RPMCTRL_MEASHLT_MSK, DTA_RPMCTRL_MEASHLT_SH);
}
static __inline void  DtaRegRpmCtrlSetHaltMeas(volatile UInt8* pBase, UInt Halt) {
    WRITE_UINT_MASKED(Halt, pBase+DTA2131_RPM_REG_RPMCTRL, DTA_RPM_REG_RPMCTRL,
                                         DTA_RPMCTRL_MEASHLT_MSK, DTA_RPMCTRL_MEASHLT_SH);
}

// RF Power Measurement Status: IsBusy  (TODOTD this is DTA-2131 specific)
static __inline UInt  DtaRegRpmStatIsBusy(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase+DTA2131_RPM_REG_RPMCTRL, DTA_RPM_REG_STATUS, 
                                            DTA_RPMSTAT_BUSY_MSK, DTA_RPMSTAT_BUSY_SH);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ HD-Channel Registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.- HD-Channel Control1 register: Access Functions -.-.-.-.-.-.-.-.-.-.

// HD-Channel Control register: IO direction
static __inline Int  DtaRegHdCtrl1GetIoDir(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_CTRL1, DT_HD_CTRL1_IODIR_MSK, 
                                                                    DT_HD_CTRL1_IODIR_SH);
}
static __inline void  DtaRegHdCtrl1SetIoDir(volatile UInt8* pBase, UInt  Dir)
{
    WRITE_UINT_MASKED(Dir, pBase, DT_HD_REG_CTRL1, DT_HD_CTRL1_IODIR_MSK, 
                                                                    DT_HD_CTRL1_IODIR_SH);
}

// HD-Channel Control register: IO enable
static __inline void  DtaRegHdCtrl1SetIoEnable(volatile UInt8* pBase, UInt  Ena)
{
    WRITE_UINT_MASKED(Ena, pBase, DT_HD_REG_CTRL1, DT_HD_CTRL1_IOENA_MSK, 
                                                                    DT_HD_CTRL1_IOENA_SH);
}

// HD-Channel Control register: IO reset
static __inline void  DtaRegHdCtrl1SetIoReset(volatile UInt8* pBase, UInt  Rst)
{
    WRITE_UINT_MASKED(Rst, pBase, DT_HD_REG_CTRL1, DT_HD_CTRL1_IORST_MSK, 
                                                                    DT_HD_CTRL1_IORST_SH);
}

// HD-Channel Control1 register:  op-mode
static __inline void  DtaRegHdCtrl1SetOpMode(volatile UInt8* pBase, UInt  Mode)
{
    WRITE_UINT_MASKED(Mode, pBase, DT_HD_REG_CTRL1, DT_HD_CTRL1_OPMODE_MSK,
                                                                   DT_HD_CTRL1_OPMODE_SH);
}

// HD-Channel Control1 register:  rx/tx-control
static __inline void  DtaRegHdCtrl1SetRxTxCtrl(volatile UInt8* pBase, UInt  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_CTRL1, DT_HD_CTRL1_RXTXCTRL_MSK,
                                                                 DT_HD_CTRL1_RXTXCTRL_SH);
}

// HD-Channel Control1 register:  RX sync error interrupt enable
static __inline void  DtaRegHdCtrl1SetRxSyncErrIntEn(volatile UInt8* pBase, UInt  Ena)
{
    WRITE_UINT_MASKED(Ena, pBase, DT_HD_REG_CTRL1, DT_HD_CTRL1_RXSYNCERRINTEN_MSK,
                                                           DT_HD_CTRL1_RXSYNCERRINTEN_SH);
}

// HD-Channel Control1 register:  RX overflow error interrupt enable
static __inline void  DtaRegHdCtrl1SetRxOvfErrIntEn(volatile UInt8* pBase, UInt  Ena)
{
    WRITE_UINT_MASKED(Ena, pBase, DT_HD_REG_CTRL1, DT_HD_CTRL1_RXOVFERRINTEN_MSK,
                                                            DT_HD_CTRL1_RXOVFERRINTEN_SH);
}

// HD-Channel Control1 register:  Last-Frame interrupt enable
static __inline void  DtaRegHdCtrl1SetLastFrameIntEn(volatile UInt8* pBase, UInt  Ena)
{
    WRITE_UINT_MASKED(Ena, pBase, DT_HD_REG_CTRL1, DT_HD_CTRL1_LASTFRMINTEN_MSK,
                                                             DT_HD_CTRL1_LASTFRMINTEN_SH);
}

//-.-.-.-.-.-.-.-.-.-.- HD-Channel Status register: Access Functions -.-.-.-.-.-.-.-.-.-.-

static __inline UInt32  DtaRegHdStatGetCarrierDetect(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_STATUS, DT_HD_STATUS_CD_MSK,
                                                                      DT_HD_STATUS_CD_SH);
}

// HD-Channel Status register:  RX sync error interrupt
static __inline UInt32  DtaRegHdStatGetRxSyncErrInt(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_STATUS, DT_HD_STATUS_RXSYNCERRINT_MSK,
                                                            DT_HD_STATUS_RXSYNCERRINT_SH);
}
static __inline void  DtaRegHdStatClrRxSyncErrInt(volatile UInt8* pBase)
{
    // Write '1' to the RX sync error interrupt status bit to clear it
    WRITE_UINT(DT_HD_STATUS_RXSYNCERRINT_MSK, pBase, DT_HD_REG_STATUS);
}

// HD-Channel Status register:  RX overflow error interrupt
static __inline UInt32  DtaRegHdStatGetRxOvfErrInt(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_STATUS, DT_HD_STATUS_RXOVFERRINT_MSK,
                                                             DT_HD_STATUS_RXOVFERRINT_SH);
}
static __inline void  DtaRegHdStatClrRxOvfErrInt(volatile UInt8* pBase)
{
    // Write '1' to the RX overflow error interrupt status bit to clear it
    WRITE_UINT(DT_HD_STATUS_RXOVFERRINT_MSK, pBase, DT_HD_REG_STATUS);
}

// HD-Channel Status register:  Last-Frame interrupt
static __inline UInt32  DtaRegHdStatGetLastFrameInt(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_STATUS, DT_HD_STATUS_LASTFRMINT_MSK,
                                                              DT_HD_STATUS_LASTFRMINT_SH);
}
static __inline void  DtaRegHdStatClrLastFrameInt(volatile UInt8* pBase)
{
    // Write '1' to the last-frame interrupt status bit to clear it
    WRITE_UINT(DT_HD_STATUS_LASTFRMINT_MSK, pBase, DT_HD_REG_STATUS);
}

//-.-.-.-.-.-.-.-.- HD-Channel Current Frame register: Access Functions -.-.-.-.-.-.-.-.-.

static __inline Int64 DtaRegHdCurrentFrameGet(volatile UInt8* pBase)
{
    Int64  Frame = 0;
    // Read LSB part first
    Frame = (Int64)READ_UINT(pBase, DT_HD_REG_CURFRM_LSB);
    Frame |= ((Int64)READ_UINT(pBase, DT_HD_REG_CURFRM_MSB))<<32;
    return Frame;
}
static __inline void  DtaRegHdCurrentFrameSet(volatile UInt8* pBase, Int64  Val)
{
    // Write LSB part first
    WRITE_UINT((UInt32)Val, pBase, DT_HD_REG_CURFRM_LSB);
    WRITE_UINT((UInt32)(Val>>32), pBase, DT_HD_REG_CURFRM_MSB);
}

//.-.-.-.-.-.-.-.- HD-Channel  ASI Byte Count register: Access Functions -.-.-.-.-.-.-.-.-

static __inline Int64 DtaRegHdAsiByteCountGet(volatile UInt8* pBase)
{
    Int64  Frame = 0;
    // Read LSB part first
    Frame = (Int64)READ_UINT(pBase, DT_HD_REG_ASIBYTECNT_LSB);
    Frame |= ((Int64)READ_UINT(pBase, DT_HD_REG_ASIBYTECNT_MSB))<<32;
    return Frame;
}
static __inline void  DtaRegHdAsiByteCountSet(volatile UInt8* pBase, Int64  Val)
{
    // Write LSB part first
    WRITE_UINT((UInt32)Val, pBase, DT_HD_REG_ASIBYTECNT_LSB);
    WRITE_UINT((UInt32)(Val>>32), pBase, DT_HD_REG_ASIBYTECNT_MSB);
}

//-.-.-.-.-.-.-.-.-.- HD-Channel Last Frame register: Access Functions -.-.-.-.-.-.-.-.-.-

static __inline Int64 DtaRegHdLastFrameGet(volatile UInt8* pBase)
{
    Int64  Frame = 0;
    // Read LSB part first
    Frame = (Int64)READ_UINT(pBase, DT_HD_REG_LASTFRM_LSB);
    Frame |= ((Int64)READ_UINT(pBase, DT_HD_REG_LASTFRM_MSB))<<32;
    return Frame;
}

//-.-.-.-.-.-.-.-.-.- HD-Channel SOF Frame register: Access Functions -.-.-.-.-.-.-.-.-.-.

static __inline Int64 DtaRegHdSofFrameGet(volatile UInt8* pBase)
{
    Int64  Frame = 0;
    // Read LSB part first
    Frame = (Int64)READ_UINT(pBase, DT_HD_REG_SOFFRM_LSB);
    Frame |= ((Int64)READ_UINT(pBase, DT_HD_REG_SOFFRM_MSB))<<32;
    return Frame;
}

//.-.-.-.-.-.-.-.-.-.- HD-Channel SOF Line register: Access Functions -.-.-.-.-.-.-.-.-.-.

static __inline Int DtaRegHdSofLineGetLine(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_SOFLINE, DT_HD_SOFLINE_LINE_MSK, 
                                                                   DT_HD_SOFLINE_LINE_SH);
}

//-.-.-.-.-.-.-.-.-.- HD-Channel SDI format register: Access Functions -.-.-.-.-.-.-.-.-.-

static __inline UInt32  DtaRegHdSdiFormatGet(volatile UInt8* pBase)
{
    return READ_UINT(pBase, DT_HD_REG_SDIFORMAT);
}
static __inline void  DtaRegHdSdiFormatSet(volatile UInt8* pBase, UInt  Val)
{
    WRITE_UINT(Val, pBase, DT_HD_REG_SDIFORMAT);
}

// HD-Channel SDI format register: Video ID
static __inline UInt32  DtaRegHdSdiFormatGetVideoId(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_SDIFORMAT, DT_HD_SDIFMT_VIDEOID_MSK, 
                                                                DT_HD_SDIFMT_VIDEOID_SH);
}
static __inline void  DtaRegHdSdiFormatSetVideoId(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_SDIFORMAT, DT_HD_SDIFMT_VIDEOID_MSK, 
                                                                 DT_HD_SDIFMT_VIDEOID_SH);
}

// HD-Channel SDI format register: Picture Rate
static __inline UInt32  DtaRegHdSdiFormatGetPictureRate(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_SDIFORMAT, DT_HD_SDIFMT_PICTRATE_MSK, 
                                                               DT_HD_SDIFMT_PICTRATE_SH);
}
static __inline void  DtaRegHdSdiFormatSetPictureRate(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_SDIFORMAT, DT_HD_SDIFMT_PICTRATE_MSK, 
                                                                 DT_HD_SDIFMT_PICTRATE_SH);
}

// HD-Channel SDI format register: Progressive
static __inline UInt32  DtaRegHdSdiFormatGetProgressive(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_SDIFORMAT, DT_HD_SDIFMT_PROGRESSIVE_MSK, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_SH);
}
static __inline void  DtaRegHdSdiFormatSetProgressive(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_SDIFORMAT, DT_HD_SDIFMT_PROGRESSIVE_MSK, 
                                                             DT_HD_SDIFMT_PROGRESSIVE_SH);
}

//-.-.-.-.-.-.-.-.-.- HD-Channel SDI Status Register: Access Functions -.-.-.-.-.-.-.-.-.-

// HD-Channel SDI Status Register: read
static __inline UInt32  DtaRegHdSdiStatusGet(volatile UInt8* pBase)
{
    return READ_UINT(pBase, DT_HD_REG_SDISTATUS);
}

// HD-Channel SDI Status Register: Video ID
static __inline UInt32  DtaRegHdSdiStatusGetVideoId(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_SDISTATUS, DT_HD_SDISTAT_VIDEOID_MSK, 
                                                                DT_HD_SDISTAT_VIDEOID_SH);
}

// HD-Channel SDI Status Register: Picture Rate
static __inline UInt32  DtaRegHdSdiStatusGetPictureRate(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_SDISTATUS, DT_HD_SDISTAT_PICTRATE_MSK, 
                                                               DT_HD_SDISTAT_PICTRATE_SH);
}

// HD-Channel SDI Status Register: Progressive
static __inline UInt32  DtaRegHdSdiStatusGetProgressive(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_SDISTATUS, DT_HD_SDISTAT_PROGRESSIVE_MSK, 
                                                            DT_HD_SDISTAT_PROGRESSIVE_SH);
}


//.-.-.-.-.-.- HD-Channel Memory Transfer Control register: Access Functions -.-.-.-.-.-.-

// HD-Channel Memory Transfer Control register:  # Lines
static __inline void  DtaRegHdMemTrControlSetNumLines(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_MEMTRCTRL, DT_HD_MEMTRCTRL_NUML_MSK, 
                                                                 DT_HD_MEMTRCTRL_NUML_SH);
}


// HD-Channel Memory Transfer Control register:  Transfer Command
static __inline void  DtaRegHdMemTrControlSetTrCmd(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_MEMTRCTRL, DT_HD_MEMTRCTRL_TRCMD_MSK, 
                                                                DT_HD_MEMTRCTRL_TRCMD_SH);
}

// HD-Channel Memory Transfer Control register:  Data Format
static __inline void  DtaRegHdMemTrControlSetDataFormat(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_MEMTRCTRL, DT_HD_MEMTRCTRL_DATAFMT_MSK, 
                                                              DT_HD_MEMTRCTRL_DATAFMT_SH);
}

// HD-Channel Memory Transfer Control register:  RGB Mode
static __inline void  DtaRegHdMemTrControlSetRgbMode(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_MEMTRCTRL, DT_HD_MEMTRCTRL_RGBMODE_MSK, 
                                                              DT_HD_MEMTRCTRL_RGBMODE_SH);
}

// HD-Channel Memory Transfer Control register:  Symbol Filter Mode
static __inline void  DtaRegHdMemTrControlSetSymFltMode(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_MEMTRCTRL, DT_HD_MEMTRCTRL_SYMFLTMODE_MSK, 
                                                           DT_HD_MEMTRCTRL_SYMFLTMODE_SH);
}

// HD-Channel Memory Transfer Control register:  Scaling Mode
static __inline void  DtaRegHdMemTrControlSetScalingMode(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_MEMTRCTRL, DT_HD_MEMTRCTRL_SCMODE_MSK, 
                                                               DT_HD_MEMTRCTRL_SCMODE_SH);
}

// HD-Channel Memory Transfer Control register: Abort 
static __inline void  DtaRegHdMemTrControlSetAbort(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_MEMTRCTRL, DT_HD_MEMTRCTRL_ABORT_MSK, 
                                                                DT_HD_MEMTRCTRL_ABORT_SH);
}

// HD-Channel Memory Transfer Control register:  Ancillary Filter Mode
static __inline void  DtaRegHdMemTrControlSetAncFltMode(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_MEMTRCTRL, DT_HD_MEMTRCTRL_ANCFLTMODE_MSK, 
                                                           DT_HD_MEMTRCTRL_ANCFLTMODE_SH);
}

// HD-Channel Memory Transfer Control register:  Stride Mode
static __inline void  DtaRegHdMemTrControlSetStrideMode(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_MEMTRCTRL, DT_HD_MEMTRCTRL_STRIDEMODE_MSK, 
                                                           DT_HD_MEMTRCTRL_STRIDEMODE_SH);
}

//.-.-.-.-.- HD-channel Memory Transfer # of Symbols register: Access Functions -.-.-.-.-.

static __inline UInt32 DtaRegHdMemTrNumSymbolsGet(volatile UInt8* pBase)
{
    return READ_UINT(pBase, DT_HD_REG_MEMTRNUMS);
}
static __inline  void DtaRegHdMemTrNumSymbolsSet(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT(Val, pBase, DT_HD_REG_MEMTRNUMS);
}

//-.-.-.-.-.- HD-channel Memory Transfer # of Bytes register: Access Functions -.-.-.-.-.-

static __inline UInt32 DtaRegHdMemTrNumBytesGet(volatile UInt8* pBase)
{
    return READ_UINT(pBase, DT_HD_REG_MEMTRNUMB);
}
static __inline  void DtaRegHdMemTrNumBytesSet(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT(Val, pBase, DT_HD_REG_MEMTRNUMB);
}

//.-.-.-.-.-.-.-.- HD-Channel S0 Begin Address register: Access Functions -.-.-.-.-.-.-.-.

static __inline UInt32 DtaRegHdS0BeginAddrGet(volatile UInt8* pBase)
{
    return READ_UINT(pBase, DT_HD_REG_S0BEGINADDR);
}
static __inline void DtaRegHdS0BeginAddrSet(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT(Val, pBase, DT_HD_REG_S0BEGINADDR);
}

//-.-.-.-.-.-.-.-.- HD-Channel S0 End Address register: Access Functions -.-.-.-.-.-.-.-.-

static __inline UInt32 DtaRegHdS0EndAddrGet(volatile UInt8* pBase)
{
    return READ_UINT(pBase, DT_HD_REG_S0ENDADDR);
}
static __inline void DtaRegHdS0EndAddrSet(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT(Val, pBase, DT_HD_REG_S0ENDADDR);
}


//.-.-.-.-.-.-.-.- HD-Channel S0 Start Address register: Access Functions -.-.-.-.-.-.-.-.

static __inline UInt32 DtaRegHdS0StartAddrGet(volatile UInt8* pBase)
{
    return READ_UINT(pBase, DT_HD_REG_S0STARTADDR);
}
static __inline void DtaRegHdS0StartAddrSet(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT(Val, pBase, DT_HD_REG_S0STARTADDR);
}


//-.-.-.-.-.-.-.-.- HD-Channel S0 Row Config register: Access Functions -.-.-.-.-.-.-.-.-.

// HD-Channel S0 Row Config register:  Number of symbols
static __inline UInt32 DtaRegHdS0RowConfigGetNumSymbols(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_S0ROWCONF, DT_HD_S0ROWCONF_NUMS_MSK, 
                                                                 DT_HD_S0ROWCONF_NUMS_SH);
}
static __inline void DtaRegHdS0RowConfigSetNumSymbols(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_S0ROWCONF, DT_HD_S0ROWCONF_NUMS_MSK, 
                                                                 DT_HD_S0ROWCONF_NUMS_SH);
}

// HD-Channel S0 Row Config register:  Number of bytes
static __inline UInt32 DtaRegHdS0RowConfigGetNumBytes(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_S0ROWCONF, DT_HD_S0ROWCONF_NUMB_MSK, 
                                                                 DT_HD_S0ROWCONF_NUMB_SH);
}
static __inline void DtaRegHdS0RowConfigSetNumBytes(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_S0ROWCONF, DT_HD_S0ROWCONF_NUMB_MSK, 
                                                                 DT_HD_S0ROWCONF_NUMB_SH);
}

//.-.-.-.-.-.-.-.- HD-Channel S1 Begin Address register: Access Functions -.-.-.-.-.-.-.-.

static __inline UInt32 DtaRegHdS1BeginAddrGet(volatile UInt8* pBase)
{
    return READ_UINT(pBase, DT_HD_REG_S1BEGINADDR);
}
static __inline void DtaRegHdS1BeginAddrSet(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT(Val, pBase, DT_HD_REG_S1BEGINADDR);
}

//-.-.-.-.-.-.-.-.- HD-Channel S1 End Address register: Access Functions -.-.-.-.-.-.-.-.-

static __inline UInt32 DtaRegHdS1EndAddrGet(volatile UInt8* pBase)
{
    return READ_UINT(pBase, DT_HD_REG_S1ENDADDR);
}
static __inline void DtaRegHdS1EndAddrSet(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT(Val, pBase, DT_HD_REG_S1ENDADDR);
}


//.-.-.-.-.-.-.-.- HD-Channel S1 Start Address register: Access Functions -.-.-.-.-.-.-.-.

static __inline UInt32 DtaRegHdS1StartAddrGet(volatile UInt8* pBase)
{
    return READ_UINT(pBase, DT_HD_REG_S1STARTADDR);
}
static __inline void DtaRegHdS1StartAddrSet(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT(Val, pBase, DT_HD_REG_S1STARTADDR);
}


//-.-.-.-.-.-.-.-.- HD-Channel S1 Row Config register: Access Functions -.-.-.-.-.-.-.-.-.

// HD-Channel S1 Row Config register:  Number of symbols
static __inline UInt32 DtaRegHdS1RowConfigGetNumSymbols(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_S1ROWCONF, DT_HD_S1ROWCONF_NUMS_MSK, 
                                                                 DT_HD_S1ROWCONF_NUMS_SH);
}
static __inline void DtaRegHdS1RowConfigSetNumSymbols(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_S1ROWCONF, DT_HD_S1ROWCONF_NUMS_MSK, 
                                                                 DT_HD_S0ROWCONF_NUMS_SH);
}

// HD-Channel S1 Row Config register:  Number of bytes
static __inline UInt32 DtaRegHdS1RowConfigGetNumBytes(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_HD_REG_S1ROWCONF, DT_HD_S1ROWCONF_NUMB_MSK, 
                                                                 DT_HD_S1ROWCONF_NUMB_SH);
}
static __inline void DtaRegHdS1RowConfigSetNumBytes(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_S1ROWCONF, DT_HD_S1ROWCONF_NUMB_MSK, 
                                                                 DT_HD_S1ROWCONF_NUMB_SH);
}

//.-.-.-.-.-.-.-.-.- HD-Channel FrameConfig1 register: Access Functions -.-.-.-.-.-.-.-.-.

// HD-Channel FrameConfig1 register: number of lines
static __inline void  DtaRegHdFrameConfig1SetNumLines(volatile UInt8* pBase, UInt  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_FRMCONF1, DT_HD_FRMCONF1_NUML_MSK,
                                                                  DT_HD_FRMCONF1_NUML_SH);
}

// HD-Channel FrameConfig1 register: interlaced flag
static __inline void  DtaRegHdFrameConfig1SetInterlaced(volatile UInt8* pBase, UInt  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_FRMCONF1, DT_HD_FRMCONF1_INTERL_MSK,
                                                                DT_HD_FRMCONF1_INTERL_SH);
}

//.-.-.-.-.-.-.-.-.- HD-Channel FrameConfig2 register: Access Functions -.-.-.-.-.-.-.-.-.

// HD-Channel FrameConfig2 register: field 1 start line
static __inline void  DtaRegHdFrameConfig2SetFld1Start(volatile UInt8* pBase, UInt  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_FRMCONF2, DT_HD_FRMCONF2_FLD1START_MSK,
                                                             DT_HD_FRMCONF2_FLD1START_SH);
}

// HD-Channel FrameConfig2 register: field 2 start line
static __inline void  DtaRegHdFrameConfig2SetFld2Start(volatile UInt8* pBase, UInt  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_FRMCONF2, DT_HD_FRMCONF2_FLD2START_MSK,
                                                             DT_HD_FRMCONF2_FLD2START_SH);
}

//.-.-.-.-.-.-.-.-.- HD-Channel FrameConfig3 register: Access Functions -.-.-.-.-.-.-.-.-.

// HD-Channel FrameConfig3 register: field 1 end line
static __inline void  DtaRegHdFrameConfig3SetFld1End(volatile UInt8* pBase, UInt  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_FRMCONF3, DT_HD_FRMCONF3_FLD1END_MSK,
                                                               DT_HD_FRMCONF3_FLD1END_SH);
}

// HD-Channel FrameConfig3 register: field 2 end line
static __inline void  DtaRegHdFrameConfig3SetFld2End(volatile UInt8* pBase, UInt  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_FRMCONF3, DT_HD_FRMCONF3_FLD2END_MSK,
                                                               DT_HD_FRMCONF3_FLD2END_SH);
}

//.-.-.-.-.-.-.-.-.- HD-Channel FrameConfig4 register: Access Functions -.-.-.-.-.-.-.-.-.

// HD-Channel FrameConfig4 register: video field 1 start line
static __inline void  DtaRegHdFrameConfig4SetVid1Start(volatile UInt8* pBase, UInt  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_FRMCONF4, DT_HD_FRMCONF4_VID1START_MSK,
                                                             DT_HD_FRMCONF4_VID1START_SH);
}

// HD-Channel FrameConfig4 register: video field 2 start line
static __inline void  DtaRegHdFrameConfig4SetVid2Start(volatile UInt8* pBase, UInt  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_FRMCONF4, DT_HD_FRMCONF4_VID2START_MSK,
                                                             DT_HD_FRMCONF4_VID2START_SH);
}

//.-.-.-.-.-.-.-.-.- HD-Channel FrameConfig5 register: Access Functions -.-.-.-.-.-.-.-.-.

// HD-Channel FrameConfig5 register: video field 1 end line
static __inline void  DtaRegHdFrameConfig5SetVid1End(volatile UInt8* pBase, UInt  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_FRMCONF5, DT_HD_FRMCONF5_VID1END_MSK,
                                                               DT_HD_FRMCONF5_VID1END_SH);
}

// HD-Channel FrameConfig5 register: video field 2 end line
static __inline void  DtaRegHdFrameConfig5SetVid2End(volatile UInt8* pBase, UInt  Val)
{
    WRITE_UINT_MASKED(Val, pBase, DT_HD_REG_FRMCONF5, DT_HD_FRMCONF5_VID2END_MSK,
                                                               DT_HD_FRMCONF5_VID2END_SH);
}

static __inline void  DtaRegHdSetAsiControl1(volatile UInt8* pBase, UInt  Val)
{
    WRITE_UINT(Val, pBase, DT_HD_REG_ASI_CONTROL1);
}

//-.-.-.-.-.-.- HD-Channel S0 Next Frame Address register: Access Functions -.-.-.-.-.-.-.

static __inline UInt32 DtaRegHdS0NextFrmAddrGet(volatile UInt8* pBase)
{
    return READ_UINT(pBase, DT_HD_REG_S0_NEXTFRM_ADDR);
}
static __inline void DtaRegHdS0NextFrmAddrSet(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT(Val, pBase, DT_HD_REG_S0_NEXTFRM_ADDR);
}

//-.-.-.-.-.-.- HD-Channel S1 Next Frame Address register: Access Functions -.-.-.-.-.-.-.

static __inline UInt32 DtaRegHdS1NextFrmAddrGet(volatile UInt8* pBase)
{
    return READ_UINT(pBase, DT_HD_REG_S1_NEXTFRM_ADDR);
}
static __inline void DtaRegHdS1NextFrmAddrSet(volatile UInt8* pBase, UInt32  Val)
{
    WRITE_UINT(Val, pBase, DT_HD_REG_S1_NEXTFRM_ADDR);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ RS-411 registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Rs422 Control register: Master (1) or Slave (0) mode
static __inline Int  DtaRegRs422CtrlGetMaster(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_RS422_REG_CTRL, DT_RS422_CTRL_MASTER_MSK, 
                                                                 DT_RS422_CTRL_MASTER_SH);
}
static __inline void  DtaRegRs422CtrlSetMaster(volatile UInt8* pBase, UInt  Master)
{
    WRITE_UINT_MASKED(Master, pBase, DT_RS422_REG_CTRL, DT_RS422_CTRL_MASTER_MSK,
                                                                 DT_RS422_CTRL_MASTER_SH);
}

// Rs422 Control register: Polarity control
static __inline Int  DtaRegRs422CtrlGetPolarity(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_RS422_REG_CTRL, DT_RS422_CTRL_POLINV_MSK, 
                                                                 DT_RS422_CTRL_POLINV_SH);
}
static __inline void  DtaRegRs422CtrlSetPolarity(volatile UInt8* pBase, UInt  Inv)
{
    WRITE_UINT_MASKED(Inv, pBase, DT_RS422_REG_CTRL, DT_RS422_CTRL_POLINV_MSK,
                                                                 DT_RS422_CTRL_POLINV_SH);
}

// Rs422 Control register: TX start
static __inline void  DtaRegRs422CtrlSetTxStart(volatile UInt8* pBase)
{
    WRITE_UINT_MASKED(1, pBase, DT_RS422_REG_CTRL, DT_RS422_CTRL_TXSTART_MSK,
                                                                DT_RS422_CTRL_TXSTART_SH);
}

// Rs422 Control register: RX enable
static __inline Int  DtaRegRs422CtrlGetRxEnable(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_RS422_REG_CTRL, DT_RS422_CTRL_RXENA_MSK, 
                                                                  DT_RS422_CTRL_RXENA_SH);
}
static __inline void  DtaRegRs422CtrlSetRxEnable(volatile UInt8* pBase, UInt  RxEnable)
{
    WRITE_UINT_MASKED(RxEnable, pBase, DT_RS422_REG_CTRL, DT_RS422_CTRL_RXENA_MSK,
                                                                  DT_RS422_CTRL_RXENA_SH);
}

// Rs422 Control register: Clear RX fifo
static __inline void  DtaRegRs422CtrlClrRxFifo(volatile UInt8* pBase)
{
    WRITE_UINT_MASKED(1, pBase, DT_RS422_REG_CTRL, DT_RS422_CTRL_RX_FIFO_CLR_MSK,
                                                            DT_RS422_CTRL_RX_FIFO_CLR_SH);
}

// Rs422 Control register: Clear TX fifo
static __inline void  DtaRegRs422CtrlClrTxFifo(volatile UInt8* pBase)
{
    WRITE_UINT_MASKED(1, pBase, DT_RS422_REG_CTRL, DT_RS422_CTRL_TX_FIFO_CLR_MSK,
                                                            DT_RS422_CTRL_TX_FIFO_CLR_SH);
}

// Rs422 Control register: data received IRQ enable
static __inline UInt  DtaRegRs422CtrlGetRxDataAvailIntEn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RS422_REG_CTRL, DT_RS422_CTRL_RX_DATA_INT_EN_MSK,
                                                         DT_RS422_CTRL_RX_DATA_INT_EN_SH);

}
static __inline void  DtaRegRs422CtrlSetRxDataAvailIntEn(
    volatile UInt8* pBase,
    UInt IntEn)
{
    WRITE_UINT_MASKED(IntEn, pBase, DT_RS422_REG_CTRL, DT_RS422_CTRL_RX_DATA_INT_EN_MSK, 
                                                         DT_RS422_CTRL_RX_DATA_INT_EN_SH);
}

// Rs422 Control register: data transmitted IRQ enable
static __inline UInt  DtaRegRs422CtrlGetTxDataTransmitIntEn(volatile UInt8* pBase) {
    return READ_UINT_MASKED(pBase, DT_RS422_REG_CTRL, DT_RS422_CTRL_TX_RDY_INT_EN_MSK,
                                                          DT_RS422_CTRL_TX_RDY_INT_EN_SH);

}
static __inline void  DtaRegRs422CtrlSetTxDataTransmitIntEn(
    volatile UInt8* pBase,
    UInt IntEn)
{
    WRITE_UINT_MASKED(IntEn, pBase, DT_RS422_REG_CTRL, DT_RS422_CTRL_TX_RDY_INT_EN_MSK, 
                                                          DT_RS422_CTRL_TX_RDY_INT_EN_SH);
}

// Rs422 TX data register: Write data to TX fifo.
static __inline void  DtaRegRs422TxDataSet(volatile UInt8* pBase, UInt  TxData) {
    WRITE_UINT(TxData, pBase, DT_RS422_REG_TX_DATA);
}

// Rs422 Status register: RX num bytes available
static __inline UInt32  DtaRegRs422StatGetRxNumBytes(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_RS422_REG_STAT, DT_RS422_STAT_RX_NUMBYTES_MSK,
                                                            DT_RS422_STAT_RX_NUMBYTES_SH);
}

// Rs422 Status register: RX data available interrupt
static __inline UInt32  DtaRegRs422StatGetRxDataAvailInt(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_RS422_REG_STAT, DT_RS422_STAT_RX_DATA_INT_MSK,
                                                            DT_RS422_STAT_RX_DATA_INT_SH);
}
static __inline void  DtaRegRs422StatClrRxDataAvailInt(volatile UInt8* pBase)
{
    WRITE_UINT(DT_RS422_STAT_RX_DATA_INT_MSK, pBase, DT_RS422_REG_STAT);
}

// Rs422 Status register: TX ready interrupt
static __inline UInt32  DtaRegRs422StatGetTxReadyInt(volatile UInt8* pBase)
{
    return READ_UINT_MASKED(pBase, DT_RS422_REG_STAT, DT_RS422_STAT_TX_RDY_INT_MSK,
                                                             DT_RS422_STAT_TX_RDY_INT_SH);
}
static __inline void  DtaRegRs422StatClrTxReadyInt(volatile UInt8* pBase)
{
    WRITE_UINT(DT_RS422_STAT_TX_RDY_INT_MSK, pBase, DT_RS422_REG_STAT);
}

// Rs422 RX data register: Read data from RX fifo.
static __inline UInt8  DtaRegRs422RxDataGet(volatile UInt8* pBase) {
    return (UInt8)READ_UINT_MASKED(pBase, DT_RS422_REG_RX_DATA, DT_RS422_RXDATA_MSK,
                                                                      DT_RS422_RXDATA_SH);
}

#endif // __DTA_REGS_H
