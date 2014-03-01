//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dta1xxRegs.h *#*#*#*#*#*#*#*# (C) 2000-2008 DekTec
//
// Definition of register sets of DTA-1XX PCI cards:
//  - In the format of structs (for compatibility with olther software)
//	- In the format of access functions, with register offsets and bit fields
//	  defined as constants
// New register sets will be defined as access functions only

//#pragma once
#ifndef __DTA1XXREGS_H
#define __DTA1XXREGS_H

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ General Access Functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- READ_UINT, WRITE_UINT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline UInt32  READ_UINT(volatile UInt8* pBase, Int ByteOffset) {
	return *(volatile UInt32*)(pBase + ByteOffset);
}
static __inline void  WRITE_UINT(UInt32 Val, volatile UInt8* pBase, Int ByteOffset) {
	*(volatile UInt32*)(pBase + ByteOffset) = Val;
}



//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Dta1xxGen +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Register block for GENERIC fields.
//
// Supports: DTA-140, DTA-160
//
typedef struct _Dta1xxGen {

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- General-Control register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	struct {
	volatile UInt  m_PE				: 1;	// Serial EEPROM Program Enable
	volatile UInt  m_PRE			: 1;	// Serial EEPROM Protect Register Enable
	volatile UInt  m_Reset			: 1;	// Board-wide software reset
			 UInt  m_Reserved1		: 1;	//
	volatile UInt  m_PerIntEn		: 1;	// Periodic-Interrupt Enable
	volatile UInt  m_PerIntInterval	: 3;	// Periodic-Interrupt Interval
	volatile UInt  m_FirmwRev		: 8;	// Firmware Revision
	volatile UInt  m_TypeNum		: 8;	// Board Type Number
	volatile UInt  m_LedControl		: 1;	// Take over LED Control
	volatile UInt  m_LedGreen		: 1;	// State of Green LED (if LedControl is set)
	volatile UInt  m_LedRed			: 1;	// State of Red LED (if LedControl is set)
			 UInt  m_Reserved2		: 5;	//
	}  m_GenControl;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- General-Status register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	union {
	 struct {
			  UInt32  m_Reserved1	: 4;	//
	 volatile UInt32  m_PerInt		: 1;	// Periodic Interrupt
			  UInt32  m_Reserved2	: 3;	//
	 volatile UInt32  m_FirmwVar	: 5;	// Variant of product firmware (DTA-160)
	          UInt32  m_Reserved3   : 3;	//
	 volatile UInt32  m_TypeNumExt  : 3;	// Type number extention
	          UInt32  m_Reserved4   : 13;	//
	 }		 Fields;						// For access to individual field
	 UInt32  All;							// For access to GenStatus in one go
	}  m_GenStatus;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Programming register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	struct s_Programming {
	volatile UInt  m_TRST			: 1;	// Control of TRST# pin
	volatile UInt  m_TDO			: 1;	// Status of TDO pin
	volatile UInt  m_TMS			: 1;	// Control of TMS pin
	volatile UInt  m_TCK			: 1;	// Control of TCK pin
	volatile UInt  m_TDI			: 1;	// Control of TDI pin
	volatile UInt  m_PrgEpc			: 1;	// Enable access to programming i/f of EPC2
	volatile UInt  m_JtagBusy		: 1;	// JTAG busy, not idle
			 UInt  m_Reserved		: 1;
	volatile UInt  m_JtagCnt		: 8;	// Number of bits uses in CPLD programming 2
	}  m_Programming;

	UInt32  m_RefClkCnt;					// Reference clock counter
	UInt32  m_EpcsCtrl;						// EPCS Control register
	UInt32  m_EpcsData;						// EPCS Data register
	UInt32  m_RefClkCntH	: 22;			// Reference clock counter highest 22 bits
	UInt32  m_Reserved1		: 10;
	UInt32  m_Programming2;					// JTAG TDI/TDO stream when writing/reading
	UInt32  m_Reserved_20;					// Unused memory address
	UInt32  m_SpiCtrl;						// SPI control
	UInt32  m_SpiData;						// SPI data
	UInt32  m_Reserved_2C;					// Unused memory address
	UInt32  m_Control0;						// Board control #0
	UInt32  m_Control1;						// Board control #1
	UInt32  m_Status0;						// Board status #0
	UInt32  m_Status1;						// Board status #1
	UInt32  m_Reserved[16];					// Unused memory addresses
}  Dta1xxGen;

// Generic Registers: BYTE offset
#define DTA1XX_GEN_REG_GENCTRL		0x0000
#define DTA1XX_GEN_REG_GENSTATUS	0x0004
#define DTA1XX_GEN_REG_PROGRAMMING	0x0008
#define DTA1XX_GEN_REG_REFCLKCNT	0x000C
#define DTA1XX_GEN_REG_EPCSCTRL		0x0010
#define DTA1XX_GEN_REG_EPCSDATA		0x0014
#define DTA1XX_GEN_REG_REFCLKCNTH	0x0018
#define DTA1XX_GEN_REG_PROGRAMMING2	0x001C
#define DTA1XX_GEN_REG_SPICTRL		0x0024
#define DTA1XX_GEN_REG_SPIDATA		0x0028
#define DTA1XX_GEN_REG_CONTROL0		0x0030
#define DTA1XX_GEN_REG_CONTROL1		0x0034
#define DTA1XX_GEN_REG_STATUS0		0x0038
#define DTA1XX_GEN_REG_STATUS1		0x003C
// Genlock specific
#define DTA1XX_GEN_REG_VCXOCTRL		0x0040
#define DTA1XX_GEN_REG_SDIPOS		0x0044 // latched on periodic int
#define DTA1XX_GEN_REG_REFCLKCNT_L	0x0048 // latched on periodic int
#define DTA1XX_GEN_REG_REFCLKCNTH_L	0x004C // latched on periodic int
// I2C Specific
#define DTA1XX_GEN_REG_I2CCTRL		0x0050
#define DTA1XX_GEN_REG_I2CSTAT		0x0054
#define DTA1XX_GEN_REG_I2CADDRCNT	0x0058
#define DTA1XX_GEN_REG_I2CDATA		0x005C
// Freq measurement
#define DTA1XX_GEN_REG_VCXOFREQ		0x0060
#define DTA1XX_GEN_REG_EXTCLKFREQ	0x0064

#define DTA1XX_GEN_REG_LICSERIAL	0x0070
#define DTA1XX_GEN_REG_LICCODE		0x0074
#define DTA1XX_GEN_REG_LICSTATUS1	0x0078
#define DTA1XX_GEN_REG_LICSTATUS2	0x007C


//.-.-.-.-.-.-.-.-.-.-.- General Control Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.

// General Control: Bit fields
#define DTA1XX_GENCTRL_PE			0x00000001
#define DTA1XX_GENCTRL_PRE			0x00000002
#define DTA1XX_GENCTRL_RESET		0x00000004
#define DTA1XX_GENCTRL_PERINT_EN	0x00000010
#define DTA1XX_GENCTRL_PERINTVAL	0x000000E0
#define DTA1XX_GENCTRL_ALTERAREV	0x0000FF00
#define DTA1XX_GENCTRL_TYPENUM		0x00FF0000
#define DTA1XX_GENCTRL_LEDCTRL		0x01000000
#define DTA1XX_GENCTRL_GREENLED		0x02000000
#define DTA1XX_GENCTRL_REDLED		0x04000000

// General Control: Register access
static __inline UInt  Dta1xxGenGetGenCtrlReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_GENCTRL);
}
static __inline void Dta1xxGenSetGenCtrlReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_GENCTRL);
}

// General Control: PE
static __inline UInt  Dta1xxGenCtrlRegGetPE(volatile UInt8* pBase) {
	return ((Dta1xxGenGetGenCtrlReg(pBase)&DTA1XX_GENCTRL_PE)!=0) ? 1 : 0;
}
static __inline void  Dta1xxGenCtrlRegSetPE(volatile UInt8* pBase, UInt PE) {
	UInt  Val = Dta1xxGenGetGenCtrlReg(pBase);
	if (PE != 0)	Val |=  DTA1XX_GENCTRL_PE;
	else			Val &= ~DTA1XX_GENCTRL_PE;
	Dta1xxGenSetGenCtrlReg(pBase, Val);
}

// General Control: PRE
static __inline UInt  Dta1xxGenCtrlRegGetPRE(volatile UInt8* pBase) {
	return ((Dta1xxGenGetGenCtrlReg(pBase)&DTA1XX_GENCTRL_PRE)!=0) ? 1 : 0;
}
static __inline void  Dta1xxGenCtrlRegSetPRE(volatile UInt8* pBase, UInt PRE) {
	UInt  Val = Dta1xxGenGetGenCtrlReg(pBase);
	if (PRE != 0)	Val |=  DTA1XX_GENCTRL_PRE;
	else			Val &= ~DTA1XX_GENCTRL_PRE;
	Dta1xxGenSetGenCtrlReg(pBase, Val);
}

// General Control: Reset
static __inline void  Dta1xxGenCtrlRegReset(volatile UInt8* pBase) {
	Dta1xxGenSetGenCtrlReg(pBase, Dta1xxGenGetGenCtrlReg(pBase) | DTA1XX_GENCTRL_RESET);
}

// General Control: FirmwareRev
static __inline UInt  Dta1xxGenCtrlRegGetFirmwareRev(volatile UInt8* pBase) {
	return ((Dta1xxGenGetGenCtrlReg(pBase)&DTA1XX_GENCTRL_ALTERAREV) >> 8);
}

// General Control: TypeNumber
static __inline UInt  Dta1xxGenCtrlRegGetTypeNumber(volatile UInt8* pBase) {
	return ((Dta1xxGenGetGenCtrlReg(pBase)&DTA1XX_GENCTRL_TYPENUM) >> 16);
}

// General Control: PerIntEn
static __inline UInt  Dta1xxGenCtrlRegGetPerIntEn(volatile UInt8* pBase) {
	return ((Dta1xxGenGetGenCtrlReg(pBase)&DTA1XX_GENCTRL_PERINT_EN)!=0) ? 1 : 0;
}
static __inline void  Dta1xxGenCtrlRegSetPerIntEn(volatile UInt8* pBase, UInt PerIntEn) {
	UInt  Val = Dta1xxGenGetGenCtrlReg(pBase);
	if (PerIntEn != 0)	Val |=  DTA1XX_GENCTRL_PERINT_EN;
	else				Val &= ~DTA1XX_GENCTRL_PERINT_EN;
	Dta1xxGenSetGenCtrlReg(pBase, Val);
}

// General Control: PerIntInterval
static __inline UInt  Dta1xxGenCtrlRegGetPerIntInterval(volatile UInt8* pBase) {
	return ((Dta1xxGenGetGenCtrlReg(pBase)&DTA1XX_GENCTRL_PERINTVAL) >> 5);
}
static __inline void  Dta1xxGenCtrlRegSetPerIntInterval(volatile UInt8* pBase, UInt Value) {
	UInt  Val = Dta1xxGenGetGenCtrlReg(pBase);
	Val = Val & ~DTA1XX_GENCTRL_PERINTVAL;
	Val = Val | ((Value << 5) & DTA1XX_GENCTRL_PERINTVAL);
	Dta1xxGenSetGenCtrlReg(pBase, Val);
}

// General Control: LedControl
static __inline UInt  Dta1xxGenCtrlRegGetLedControl(volatile UInt8* pBase) {
	return ((Dta1xxGenGetGenCtrlReg(pBase)&DTA1XX_GENCTRL_LEDCTRL)!=0) ? 1 : 0;
}
static __inline void  Dta1xxGenCtrlRegSetLedControl(volatile UInt8* pBase, UInt LedControl) {
	UInt  Val = Dta1xxGenGetGenCtrlReg(pBase);
	if (LedControl != 0)	Val |=  DTA1XX_GENCTRL_LEDCTRL;
	else					Val &= ~DTA1XX_GENCTRL_LEDCTRL;
	Dta1xxGenSetGenCtrlReg(pBase, Val);
}

// General Control: LedGreen
static __inline UInt  Dta1xxGenCtrlRegGetLedGreen(volatile UInt8* pBase) {
	return ((Dta1xxGenGetGenCtrlReg(pBase)&DTA1XX_GENCTRL_GREENLED)!=0) ? 1 : 0;
}
static __inline void  Dta1xxGenCtrlRegSetLedGreen(volatile UInt8* pBase, UInt LedGreen) {
	UInt  Val = Dta1xxGenGetGenCtrlReg(pBase);
	if (LedGreen != 0)	Val |=  DTA1XX_GENCTRL_GREENLED;
	else				Val &= ~DTA1XX_GENCTRL_GREENLED;
	Dta1xxGenSetGenCtrlReg(pBase, Val);
}

// General Control: LedRed
static __inline UInt  Dta1xxGenCtrlRegGetLedRed(volatile UInt8* pBase) {
	return ((Dta1xxGenGetGenCtrlReg(pBase)&DTA1XX_GENCTRL_REDLED)!=0) ? 1 : 0;
}
static __inline void  Dta1xxGenCtrlRegSetLedRed(volatile UInt8* pBase, UInt LedRed) {
	UInt  Val = Dta1xxGenGetGenCtrlReg(pBase);
	if (LedRed != 0)	Val |=  DTA1XX_GENCTRL_REDLED;
	else				Val &= ~DTA1XX_GENCTRL_REDLED;
	Dta1xxGenSetGenCtrlReg(pBase, Val);
}

//.-.-.-.-.-.-.-.-.-.-.- General Status Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-

// General Status: Bit fields
#define DTA1XX_GENSTAT_PERINT		0x00000010
#define DTA1XX_GENSTAT_FIRMWVAR_MSK	0x00001F00
#define DTA1XX_GENSTAT_FIRMWVAR_SH	8
#define DTA1XX_GENSTAT_TYPEEXT_MSK	0x00070000
#define DTA1XX_GENSTAT_TYPEEXT_SH	16


// General Status: Register access
static __inline UInt  Dta1xxGenGetGenStatusReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_GENSTATUS);
}
static void Dta1xxGenSetGenStatusReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_GENSTATUS);
}

// General Status: PerInt
static __inline UInt  Dta1xxGenStatusRegGetPerInt(volatile UInt8* pBase) {
	return ((Dta1xxGenGetGenStatusReg(pBase)&DTA1XX_GENSTAT_PERINT)!=0) ? 1 : 0;
}
static __inline void  Dta1xxGenStatusRegClrPerInt(volatile UInt8* pBase) {
	Dta1xxGenSetGenStatusReg(pBase, DTA1XX_GENSTAT_PERINT);
}

// General Status: Firmware Variant
static __inline UInt  Dta1xxGenStatusRegGetFirmwareVariant(volatile UInt8* pBase) {
	UInt  Val = Dta1xxGenGetGenStatusReg(pBase) & DTA1XX_GENSTAT_FIRMWVAR_MSK;
	return (Val >> DTA1XX_GENSTAT_FIRMWVAR_SH);
}

// General Status: Type Number Extension
static __inline UInt  Dta1xxGenStatusRegGetTypeNumExt(volatile UInt8* pBase) {
	UInt  Val = Dta1xxGenGetGenStatusReg(pBase) & DTA1XX_GENSTAT_TYPEEXT_MSK;
	return (Val >> DTA1XX_GENSTAT_TYPEEXT_SH);
}

//.-.-.-.-.-.-.-.-.-.-.-.- Programming Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

// Programming register: Bit fields
#define DTA1XX_PROG_TRST			0x00000001
#define DTA1XX_PROG_TDO				0x00000002
#define DTA1XX_PROG_TMS				0x00000004
#define DTA1XX_PROG_TCK				0x00000008
#define DTA1XX_PROG_TDI				0x00000010
#define DTA1XX_PROG_PRGEPC			0x00000020
#define DTA1XX_PROG_JTAGCNT			0x00003F00

//-.-.-.-.-.-.-.-.-.- Reference Clock Count Register: Access Functions -.-.-.-.-.-.-.-.-.-

static __inline UInt32  Dta1xxGenGetRefClkCntReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_REFCLKCNT);
}

static __inline UInt32  Dta1xxGenGetRefClkCntRegH(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_REFCLKCNTH) & 0x3FFFFF;
}

static __inline UInt64  Dta1xxGenGetRefClkCntReg64(volatile UInt8* pBase) {
	UInt32 RefClkH = Dta1xxGenGetRefClkCntRegH(pBase);
	UInt32 RefClkL = Dta1xxGenGetRefClkCntReg(pBase);

	if (RefClkH != Dta1xxGenGetRefClkCntRegH(pBase)) {
		RefClkH++;
		RefClkL = Dta1xxGenGetRefClkCntReg(pBase);
	}
	return ((UInt64) RefClkH << 32) + RefClkL;
}

//-.-.-.-.-.-.-.-.-.- Periodic Interrupt Latched Clock Count Register: Access Functions -.-.-.-.-.-.-.-.-.-
// Note: Only present when firmware supports GENLOCK
static __inline UInt32  Dta1xxGenGetRefClkCntLReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_REFCLKCNT_L);
}

static __inline UInt32  Dta1xxGenGetRefClkCntLRegH(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_REFCLKCNTH_L) & 0x3FFFFF;
}

static __inline UInt64  Dta1xxGenGetRefClkCntLReg64(volatile UInt8* pBase) {
	UInt32 RefClkH = Dta1xxGenGetRefClkCntLRegH(pBase);
	UInt32 RefClkL = Dta1xxGenGetRefClkCntLReg(pBase);

	if (RefClkH != Dta1xxGenGetRefClkCntLRegH(pBase)) {
		RefClkH++;
		RefClkL = Dta1xxGenGetRefClkCntLReg(pBase);
	}
	return ((UInt64) RefClkH << 32) + RefClkL;
}


//-.-.-.-.-.-.-.-.-.-.-.- EPCS Control Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

// EPCS Control: Bit fields
#define DTA1XX_EPCSCTRL_ERASE			0x00000001
#define DTA1XX_EPCSCTRL_PROG			0x00000002
#define DTA1XX_EPCSCTRL_READ			0x00000004
#define DTA1XX_EPCSCTRL_BUSY			0x00000080
#define DTA1XX_EPCSCTRL_ADDR_LSB_MASK	0x0FFFFF00
#define DTA1XX_EPCSCTRL_ADDR_LSB_NBITS	20
#define DTA1XX_EPCSCTRL_ADDR_LSB_SHIFT	8
#define DTA1XX_EPCSCTRL_ADDR_MSB_MASK	0x00000078
#define DTA1XX_EPCSCTRL_ADDR_MSB_SHIFT	3
#define DTA1XX_EPCSCTRL_UNLCK_SHIFT		28

// EPCS Control: Register access
static __inline UInt  Dta1xxGenGetEpcsCtrl(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_EPCSCTRL);
}
static __inline void Dta1xxGenSetEpcsCtrl(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_EPCSCTRL);
}

// EPCS Control: Erase
static __inline void  Dta1xxEpcsCtrlRegErase(volatile UInt8* pBase, UInt Addr) {
	UInt  Val = Dta1xxGenGetEpcsCtrl(pBase);
	Val &= ~(DTA1XX_EPCSCTRL_ADDR_LSB_MASK | DTA1XX_EPCSCTRL_ADDR_MSB_MASK |
			 DTA1XX_EPCSCTRL_ERASE | DTA1XX_EPCSCTRL_PROG | DTA1XX_EPCSCTRL_READ);
	Val |= (Addr << DTA1XX_EPCSCTRL_ADDR_LSB_SHIFT) & DTA1XX_EPCSCTRL_ADDR_LSB_MASK;
	Val |= ((Addr>>DTA1XX_EPCSCTRL_ADDR_LSB_NBITS)<<DTA1XX_EPCSCTRL_ADDR_MSB_SHIFT)
				& DTA1XX_EPCSCTRL_ADDR_MSB_MASK;
	Val |= DTA1XX_EPCSCTRL_ERASE;
	Dta1xxGenSetEpcsCtrl(pBase, Val);
}

// EPCS Control: Page Program
static __inline void  Dta1xxEpcsCtrlRegProg(volatile UInt8* pBase, UInt Addr) {
	UInt  Val = Dta1xxGenGetEpcsCtrl(pBase);
	Val &= ~(DTA1XX_EPCSCTRL_ADDR_LSB_MASK | DTA1XX_EPCSCTRL_ADDR_MSB_MASK |
			 DTA1XX_EPCSCTRL_ERASE | DTA1XX_EPCSCTRL_PROG | DTA1XX_EPCSCTRL_READ);
	Val |= (Addr << DTA1XX_EPCSCTRL_ADDR_LSB_SHIFT) & DTA1XX_EPCSCTRL_ADDR_LSB_MASK;
	Val |= ((Addr>>DTA1XX_EPCSCTRL_ADDR_LSB_NBITS)<<DTA1XX_EPCSCTRL_ADDR_MSB_SHIFT)
				& DTA1XX_EPCSCTRL_ADDR_MSB_MASK;
	Val |= DTA1XX_EPCSCTRL_PROG;
	Dta1xxGenSetEpcsCtrl(pBase, Val);
}

// EPCS Control: Read command
// After this call, data can be read from EPCS Data register
static __inline void  Dta1xxEpcsCtrlRegRead(volatile UInt8* pBase, UInt Addr) {
	UInt  Val = Dta1xxGenGetEpcsCtrl(pBase);
	Val &= ~(DTA1XX_EPCSCTRL_ADDR_LSB_MASK | DTA1XX_EPCSCTRL_ADDR_MSB_MASK |
			 DTA1XX_EPCSCTRL_ERASE | DTA1XX_EPCSCTRL_PROG | DTA1XX_EPCSCTRL_READ);
	Val |= (Addr << DTA1XX_EPCSCTRL_ADDR_LSB_SHIFT) & DTA1XX_EPCSCTRL_ADDR_LSB_MASK;
	Val |= ((Addr>>DTA1XX_EPCSCTRL_ADDR_LSB_NBITS)<<DTA1XX_EPCSCTRL_ADDR_MSB_SHIFT)
				& DTA1XX_EPCSCTRL_ADDR_MSB_MASK;
	Val |= DTA1XX_EPCSCTRL_READ;
	Dta1xxGenSetEpcsCtrl(pBase, Val);
}

// EPCS Control: Busy
static __inline UInt  Dta1xxEpcsCtrlRegIsBusy(volatile UInt8* pBase) {
	return ((Dta1xxGenGetEpcsCtrl(pBase)&DTA1XX_EPCSCTRL_BUSY)!=0) ? 1 : 0;
}
// EPCS Control: Unlock
static __inline void  Dta1xxEpcsCtrlRegSetUnlock(volatile UInt8* pBase, UInt UnlockValue) {
	UInt  Val = Dta1xxGenGetEpcsCtrl(pBase);
	Val |= (UnlockValue << DTA1XX_EPCSCTRL_UNLCK_SHIFT);
	Dta1xxGenSetEpcsCtrl(pBase, Val);
}


//-.-.-.-.-.-.-.-.-.-.-.-.- EPCS Data Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// EPCS Data: Register access
static __inline UInt  Dta1xxGenGetEpcsData(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_EPCSDATA);
}
static __inline void Dta1xxGenSetEpcsData(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_EPCSDATA);
}


//.-.-.-.-.-.-.-.-.-.-.-.- SPI Control Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

// SPI Control: Bit fields
#define DTA1XX_SPICTRL_PROG				0x00000002
#define DTA1XX_SPICTRL_READ				0x00000004
#define DTA1XX_SPICTRL_BUSY				0x00000080
#define DTA1XX_SPICTRL_ADDR_MASK		0x0001FF00
#define DTA1XX_SPICTRL_ADDR_SHIFT		8
#define DTA1XX_SPICTRL_UNLCK_SHIFT		28

// SPI Control: Register access
static __inline UInt  Dta1xxGenGetSpiCtrl(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_SPICTRL);
}
static __inline void Dta1xxGenSetSpiCtrl(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_SPICTRL);
}

// SPI Control: Page Program
static __inline void  Dta1xxSpiCtrlRegProg(volatile UInt8* pBase, UInt Addr) {
	UInt  Val = Dta1xxGenGetSpiCtrl(pBase);
	Val &= ~(DTA1XX_SPICTRL_ADDR_MASK | DTA1XX_SPICTRL_PROG | DTA1XX_SPICTRL_READ);
	Val |= (Addr << DTA1XX_SPICTRL_ADDR_SHIFT) & DTA1XX_SPICTRL_ADDR_MASK;
	Val |= DTA1XX_SPICTRL_PROG;
	Dta1xxGenSetSpiCtrl(pBase, Val);
}

// SPI Control: Read command
// After this call, data can be read from SPI Data register
static __inline void  Dta1xxSpiCtrlRegRead(volatile UInt8* pBase, UInt Addr) {
	UInt  Val = Dta1xxGenGetSpiCtrl(pBase);
	Val &= ~(DTA1XX_SPICTRL_ADDR_MASK | DTA1XX_SPICTRL_PROG | DTA1XX_SPICTRL_READ);
	Val |= (Addr << DTA1XX_SPICTRL_ADDR_SHIFT) & DTA1XX_SPICTRL_ADDR_MASK;
	Val |= DTA1XX_SPICTRL_READ;
	Dta1xxGenSetSpiCtrl(pBase, Val);
}

// SPI Control: Busy
static __inline UInt  Dta1xxSpiCtrlRegIsBusy(volatile UInt8* pBase) {
	return ((Dta1xxGenGetSpiCtrl(pBase)&DTA1XX_SPICTRL_BUSY)!=0) ? 1 : 0;
}
// SPI Control: Unlock
static __inline void  Dta1xxSpiCtrlRegSetUnlock(volatile UInt8* pBase, UInt UnlockValue) {
	UInt  Val = Dta1xxGenGetSpiCtrl(pBase);
	Val |= (UnlockValue << DTA1XX_SPICTRL_UNLCK_SHIFT);
	Dta1xxGenSetSpiCtrl(pBase, Val);
}


//-.-.-.-.-.-.-.-.-.-.-.-.- SPI Data Register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.

// SPI Data: Register access
static __inline UInt  Dta1xxGenGetSpiData(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_SPIDATA);
}
static __inline void Dta1xxGenSetSpiData(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_SPIDATA);
}


//-.-.-.-.-.-.-.-.-.-.-.-.- BoardControl0 Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA1XX_GEN_CONTROL0_DBLBUF		0x00000001
#define DTA1XX_GEN_CONTROL0_VCOEN		0x00000002
#define DTA1XX_GEN_CONTROL0_DIVERSITY	0x00000100		// DTA-2135 specific

static __inline UInt  Dta1xxGenGetControl0Reg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_CONTROL0);
}
static __inline void Dta1xxGenSetControl0Reg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_CONTROL0);
}

// Get VCO Enable
static __inline UInt Dta1xxGenVcoEnableGet(volatile UInt8* pBase) {
	return (Dta1xxGenGetControl0Reg(pBase) & DTA1XX_GEN_CONTROL0_VCOEN ? 1 : 0);
}

// Set VCO Enable
static __inline void Dta1xxGenVcoEnableSet(volatile UInt8* pBase, UInt Enable) {
	UInt  Val;
	Val = Dta1xxGenGetControl0Reg(pBase);
	if (Enable) Val = Val | DTA1XX_GEN_CONTROL0_VCOEN;
	else		Val = Val & ~DTA1XX_GEN_CONTROL0_VCOEN;
	Dta1xxGenSetControl0Reg(pBase, Val);
}

// Get DblBuf Enable
static __inline UInt Dta1xxGenDblBufEnableGet(volatile UInt8* pBase) {
	return (Dta1xxGenGetControl0Reg(pBase) & DTA1XX_GEN_CONTROL0_DBLBUF ? 1 : 0);
}

// Set DblBuf Enable
static __inline void Dta1xxGenDblBufEnableSet(volatile UInt8* pBase, UInt Enable) {
	UInt  Val;
	Val = Dta1xxGenGetControl0Reg(pBase);
	if (Enable) Val = Val | DTA1XX_GEN_CONTROL0_DBLBUF;
	else		Val = Val & ~DTA1XX_GEN_CONTROL0_DBLBUF;
	Dta1xxGenSetControl0Reg(pBase, Val);
}


// DTA-2135 specific

// Get Diversity mode of board
static __inline UInt Dta1xxGenDiversityGet(volatile UInt8* pBase) {
	return (Dta1xxGenGetControl0Reg(pBase) & DTA1XX_GEN_CONTROL0_DIVERSITY ? 1 : 0);
}

// Set Diversity mode
static __inline void Dta1xxGenDiversitySet(volatile UInt8* pBase, UInt Enable) {
	UInt  Val;
	Val = Dta1xxGenGetControl0Reg(pBase);
	if (Enable) Val = Val | DTA1XX_GEN_CONTROL0_DIVERSITY;
	else		Val = Val & ~DTA1XX_GEN_CONTROL0_DIVERSITY;
	Dta1xxGenSetControl0Reg(pBase, Val);
}


//-.-.-.-.-.-.-.-.-.-.-.-.- BoardControl1 Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA1XX_GEN_CONTROL1_WATCHDOG		0x00000001

static __inline UInt  Dta1xxGenGetControl1Reg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_CONTROL1);
}
static __inline void Dta1xxGenSetControl1Reg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_CONTROL1);
}

// Toggle watchdog 1->0 or 0->1
static __inline void Dta1xxGenToggleWatchdog(volatile UInt8* pBase) {
	UInt  Val;
	Val = Dta1xxGenGetControl1Reg(pBase);
	if (Val & DTA1XX_GEN_CONTROL1_WATCHDOG) Val = Val & ~DTA1XX_GEN_CONTROL1_WATCHDOG;
	else Val = Val | DTA1XX_GEN_CONTROL1_WATCHDOG;
	Dta1xxGenSetControl1Reg(pBase, Val);
}

// Pulse watchdog 1->0->1 or 0->1->0
static __inline void Dta1xxGenPulseWatchdog(volatile UInt8* pBase) {
	UInt  Val;
	Val = Dta1xxGenGetControl1Reg(pBase);

	// Toggle watchdog bit
	if (Val & DTA1XX_GEN_CONTROL1_WATCHDOG) {
		Dta1xxGenSetControl1Reg(pBase, Val & ~DTA1XX_GEN_CONTROL1_WATCHDOG);
	} else {
		Dta1xxGenSetControl1Reg(pBase, Val | DTA1XX_GEN_CONTROL1_WATCHDOG);
	}
	// Reset watchdog bit
	Dta1xxGenSetControl1Reg(pBase, Val);
}


//-.-.-.-.-.-.-.-.-.-.-.-.- Status0 Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.

static __inline UInt  Dta1xxGenGetStatus0Reg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_STATUS0);
}
static __inline void Dta1xxGenSetStatus0Reg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_STATUS0);
}


//-.-.-.-.-.-.-.-.-.-.-.-.- Status1 Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.

static __inline UInt  Dta1xxGenGetStatus1Reg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_STATUS1);
}
static __inline void Dta1xxGenSetStatus1Reg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_STATUS1);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.- VCXO Control - Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

#define DTA1XX_VCXOCTRL_EN_MASK			0x00000001
#define DTA1XX_VCXOCTRL_EN_SHIFT		0
#define DTA1XX_VCXOCTRL_VIDSTD_MASK		0x00000002
#define DTA1XX_VCXOCTRL_VIDSTD_SHIFT	1
#define DTA1XX_VCXOCTRL_SPEED_MASK		0x0003FF00
#define DTA1XX_VCXOCTRL_SPEED_SHIFT		8

static __inline UInt  Dta1xxGenGetVcxoCtrlReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_VCXOCTRL);
}
static __inline void  Dta1xxGenSetVcxoCtrlReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_VCXOCTRL);
}

// Get VCXO enable
static __inline int  Dta1xxGenGetVcxoEn(volatile UInt8* pBase) {
	return Dta1xxGenGetVcxoCtrlReg(pBase) & DTA1XX_VCXOCTRL_EN_MASK;
}

// Set VCXO enable
static __inline void  Dta1xxGenSetVcxoEn(volatile UInt8* pBase, UInt Ena) {
	UInt  Val = Dta1xxGenGetVcxoCtrlReg(pBase) & ~DTA1XX_VCXOCTRL_EN_MASK;
	Dta1xxGenSetVcxoCtrlReg(pBase, Val | (Ena << DTA1XX_VCXOCTRL_EN_SHIFT));
}

static __inline void  Dta1xxGenSetVcxoStd(volatile UInt8* pBase, UInt Std) {
	UInt  Val = Dta1xxGenGetVcxoCtrlReg(pBase) & ~DTA1XX_VCXOCTRL_VIDSTD_MASK;
	Dta1xxGenSetVcxoCtrlReg(pBase, Val | (Std << DTA1XX_VCXOCTRL_VIDSTD_SHIFT));
}

static __inline int  Dta1xxGenGetVcxoStd(volatile UInt8* pBase) {
	return (Dta1xxGenGetVcxoCtrlReg(pBase) & DTA1XX_VCXOCTRL_EN_MASK)==DTA1XX_VCXOCTRL_EN_MASK;
}

// Set VCXO speed
// Speed = 0..1023
static __inline void  Dta1xxGenSetVcxoSpeed(volatile UInt8* pBase, UInt Speed) {
	UInt  Val = Dta1xxGenGetVcxoCtrlReg(pBase) & ~DTA1XX_VCXOCTRL_SPEED_MASK;
	Dta1xxGenSetVcxoCtrlReg(pBase, Val | (Speed << DTA1XX_VCXOCTRL_SPEED_SHIFT));
}


//-.-.-.-.-.-.-.-.-.-.-.-.- VcxoFreq Measurement Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.

static __inline UInt  Dta1xxGenGetVcxoFreqReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_VCXOFREQ);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- ExtClk Measurement Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.

static __inline UInt  Dta1xxGenGetExtClkFreqReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_EXTCLKFREQ);
}

//-.-.-.-.-.-.-.-.-.-.-.- Transmit SDI Position - Access Functions -.-.-.-.-.-.-.-.-.-.-.-

#define DTA1XX_TXSDIPOS_LINE_SHIFT	11
#define DTA1XX_TXSDIPOS_FRAME_SHIFT	21

#define DTA1XX_TXSDIPOS_SAMP_MASK	0x7FF
#define DTA1XX_TXSDIPOS_LINE_MASK	0x3FF << DTA1XX_TXSDIPOS_LINE_SHIFT
#define DTA1XX_TXSDIPOS_FRAME_MASK	0x7FF << DTA1XX_TXSDIPOS_FRAME_SHIFT

// Get Transmit SDI Position
// Return value: frame count; Line count and Sample count are returned through pointer argument
static __inline UInt  Dta1xxGenGetTxSdiPos(volatile UInt8* pBase, UInt* pLineCnt, UInt* pSampCnt) {
	UInt  RegVal = READ_UINT(pBase, DTA1XX_GEN_REG_SDIPOS);

	*pSampCnt = RegVal & DTA1XX_TXSDIPOS_SAMP_MASK;
	*pLineCnt = (RegVal & DTA1XX_TXSDIPOS_LINE_MASK) >> DTA1XX_TXSDIPOS_LINE_SHIFT;
	return (RegVal & DTA1XX_TXSDIPOS_FRAME_MASK) >> DTA1XX_TXSDIPOS_FRAME_SHIFT;
}

// Set Transmit SDI Position
// Enables to force TX/RX Counter synchronisation
static __inline void  Dta1xxGenSetTxSdiPosReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_SDIPOS);
}

static __inline void Dta1xxGenSetTxSdiPos(volatile UInt8* pBase, UInt pLineCnt, UInt pSampCnt) {
	UInt Val = (pLineCnt << DTA1XX_TXSDIPOS_LINE_SHIFT) + pSampCnt;
	Dta1xxGenSetTxSdiPosReg(pBase, Val);
}

//-.-.-.-.-.-.-.-.-.-.-.- I2C Control Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

// I2C Control: Bit fields
#define DTA1XX_I2CCTRL_RDYINT_EN		0x00000001
#define DTA1XX_I2CCTRL_CLKDIV_MASK		0x0FFF0000
#define DTA1XX_I2CCTRL_CLKDIV_SHIFT		16

// I2C Control: Register access
static __inline UInt  Dta1xxGenGetI2cCtrl(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_I2CCTRL);
}
static __inline void Dta1xxGenSetI2cCtrl(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_I2CCTRL);
}

// I2C Control: RdyIntEn
static __inline UInt  Dta1xxGenI2cCtrlRegGetRdyIntEn(volatile UInt8* pBase) {
	return ((Dta1xxGenGetI2cCtrl(pBase)&DTA1XX_I2CCTRL_RDYINT_EN)!=0) ? 1 : 0;
}
static __inline void  Dta1xxGenI2cCtrlRegSetRdyIntEn(volatile UInt8* pBase, UInt RdyIntEn) {
	UInt  Val = Dta1xxGenGetI2cCtrl(pBase);
	if (RdyIntEn != 0)	Val |=  DTA1XX_I2CCTRL_RDYINT_EN;
	else				Val &= ~DTA1XX_I2CCTRL_RDYINT_EN;
	Dta1xxGenSetI2cCtrl(pBase, Val);
}

// I2C Control: ClkDiv
static __inline UInt  Dta1xxGenI2cCtrlRegGetClkDiv(volatile UInt8* pBase) {
	UInt Val = Dta1xxGenGetI2cCtrl(pBase) & DTA1XX_I2CCTRL_CLKDIV_MASK;
	return (Val >> DTA1XX_I2CCTRL_CLKDIV_SHIFT);
}
static __inline void  Dta1xxGenI2cCtrlRegSetClkDiv(volatile UInt8* pBase, UInt ClkDiv) {
	UInt  Val = Dta1xxGenGetI2cCtrl(pBase);
	Val &=  ~DTA1XX_I2CCTRL_CLKDIV_MASK;			
	Val |= ((ClkDiv << DTA1XX_I2CCTRL_CLKDIV_SHIFT) & DTA1XX_I2CCTRL_CLKDIV_MASK);
	Dta1xxGenSetI2cCtrl(pBase, Val);
}


//.-.-.-.-.-.-.-.-.-.-.-.- I2C Status Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

// I2C  Status: Bit fields
#define DTA1XX_I2CSTAT_RDYINT			0x00000001
#define DTA1XX_I2CSTAT_WRADDR_NACK		0x00000002
#define DTA1XX_I2CSTAT_WRDATA_NACK		0x00000004
#define DTA1XX_I2CSTAT_RDADDR_NACK		0x00000008
#define DTA1XX_I2CSTAT_TIMEOUT			0x00000010

// I2C Status: Register access
static __inline UInt  Dta1xxGenGetI2cStatus(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_I2CSTAT);
}
static __inline void Dta1xxGenSetI2cStatus(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_I2CSTAT);
}

// I2C Control: RdyInt
static __inline UInt  Dta1xxGenI2cStatusRegGetRdyInt(volatile UInt8* pBase) {
	return ((Dta1xxGenGetI2cStatus(pBase)&DTA1XX_I2CSTAT_RDYINT)!=0) ? 1 : 0;
}
static __inline void  Dta1xxGenI2cStatusRegClrRdyInt(volatile UInt8* pBase) {
	Dta1xxGenSetI2cStatus(pBase, DTA1XX_I2CSTAT_RDYINT);
}

// I2C Control: Write-Address-Nacked
static __inline UInt  Dta1xxGenI2cStatusRegGetWrAddrNack(volatile UInt8* pBase) {
	return ((Dta1xxGenGetI2cStatus(pBase)&DTA1XX_I2CSTAT_WRADDR_NACK)!=0) ? 1 : 0;
}

// I2C Control: Write-Data-Nacked
static __inline UInt  Dta1xxGenI2cStatusRegGetWrDataNack(volatile UInt8* pBase) {
	return ((Dta1xxGenGetI2cStatus(pBase)&DTA1XX_I2CSTAT_WRDATA_NACK)!=0) ? 1 : 0;
}

// I2C Control: Read-Address-Nacked
static __inline UInt  Dta1xxGenI2cStatusRegGetRdAddrNack(volatile UInt8* pBase) {
	return ((Dta1xxGenGetI2cStatus(pBase)&DTA1XX_I2CSTAT_RDADDR_NACK)!=0) ? 1 : 0;
}

// I2C Control: Timeout
static __inline UInt  Dta1xxGenI2cStatusRegGetTimeout(volatile UInt8* pBase) {
	return ((Dta1xxGenGetI2cStatus(pBase)&DTA1XX_I2CSTAT_TIMEOUT)!=0) ? 1 : 0;
}


//.-.-.-.-.-.-.-.-.-.- I2C Address/Count Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-

// I2C Address/Count: Bit fields
#define DTA1XX_I2CADDRCNT_WRADDR_MASK	0x0000007F
#define DTA1XX_I2CADDRCNT_WRADDR_SHIFT	0
#define DTA1XX_I2CADDRCNT_WRCNT_MASK	0x0000FF80
#define DTA1XX_I2CADDRCNT_WRCNT_SHIFT	7
#define DTA1XX_I2CADDRCNT_RDADDR_MASK	0x007F0000
#define DTA1XX_I2CADDRCNT_RDADDR_SHIFT	16
#define DTA1XX_I2CADDRCNT_RDCNT_MASK	0xFF800000
#define DTA1XX_I2CADDRCNT_RDCNT_SHIFT	23

// I2C Address/Count: Register access
static __inline UInt  Dta1xxGenGetI2cAddrCnt(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_I2CADDRCNT);
}
static __inline void Dta1xxGenSetI2cAddrCnt(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_I2CADDRCNT);
}

// I2C Address/Count: Set Write Addres and Count in one go
static __inline void Dta1xxGenI2cAddrCntRegSetWrAddrAndCount(
	volatile UInt8* pBase,
	UInt Addr,
	UInt Count)
{
	UInt Val = (Addr<<DTA1XX_I2CADDRCNT_WRADDR_SHIFT) & DTA1XX_I2CADDRCNT_WRADDR_MASK;
	Val |= (Count<<DTA1XX_I2CADDRCNT_WRCNT_SHIFT) & DTA1XX_I2CADDRCNT_WRCNT_MASK;
	Dta1xxGenSetI2cAddrCnt(pBase, Val);
}

// I2C Address/Count: Write Address
static __inline UInt  Dta1xxGenI2cAddrCntRegGetWrAddress(volatile UInt8* pBase) {
	UInt  Val = Dta1xxGenGetI2cAddrCnt(pBase) & DTA1XX_I2CADDRCNT_WRADDR_MASK;
	return (Val >> DTA1XX_I2CADDRCNT_WRADDR_SHIFT);
}
//static __inline void  Dta1xxGenI2cAddrCntRegSetWrAddress(volatile UInt8* pBase, UInt Addr) {
//	UInt  Val = Dta1xxGenGetI2cAddrCnt(pBase);
//	Val &= ~DTA1XX_I2CADDRCNT_WRADDR_MASK;
//	Val |= (Addr << DTA1XX_I2CADDRCNT_WRADDR_SHIFT) & DTA1XX_I2CADDRCNT_WRADDR_MASK;
//	Dta1xxGenSetI2cAddrCnt(pBase, Val);
//}

// I2C Address/Count: Write Count
static __inline UInt  Dta1xxGenI2cAddrCntRegGetWrCount(volatile UInt8* pBase) {
	UInt  Val = Dta1xxGenGetI2cAddrCnt(pBase) & DTA1XX_I2CADDRCNT_WRCNT_MASK;
	return (Val >> DTA1XX_I2CADDRCNT_WRCNT_SHIFT);
}
//static __inline void  Dta1xxGenI2cAddrCntRegSetWrCount(volatile UInt8* pBase, UInt Count) {
//	UInt  Val = Dta1xxGenGetI2cAddrCnt(pBase);
//	Val &= ~DTA1XX_I2CADDRCNT_WRCNT_MASK;
//	Val |= (Count << DTA1XX_I2CADDRCNT_WRCNT_SHIFT) & DTA1XX_I2CADDRCNT_WRCNT_MASK;
//	Dta1xxGenSetI2cAddrCnt(pBase, Val);
//}

// I2C Address/Count: Set Read Addres and Count in one go
static __inline void Dta1xxGenI2cAddrCntRegSetRdAddrAndCount(
	volatile UInt8* pBase,
	UInt Addr,
	UInt Count)
{
	UInt Val = (Addr<<DTA1XX_I2CADDRCNT_RDADDR_SHIFT) & DTA1XX_I2CADDRCNT_RDADDR_MASK;
	Val |= (Count<<DTA1XX_I2CADDRCNT_RDCNT_SHIFT) & DTA1XX_I2CADDRCNT_RDCNT_MASK;
	Dta1xxGenSetI2cAddrCnt(pBase, Val);
}

// I2C Address/Count: Read Address
static __inline UInt  Dta1xxGenI2cAddrCntRegGetRdAddress(volatile UInt8* pBase) {
	UInt  Val = Dta1xxGenGetI2cAddrCnt(pBase) & DTA1XX_I2CADDRCNT_RDADDR_MASK;
	return (Val >> DTA1XX_I2CADDRCNT_RDADDR_SHIFT);
}
//static __inline void  Dta1xxGenI2cAddrCntRegSetRdAddress(volatile UInt8* pBase, UInt Count) {
//	UInt  Val = Dta1xxGenGetI2cAddrCnt(pBase);
//	Val &= ~DTA1XX_I2CADDRCNT_RDADDR_MASK;
//	Val |= (Count << DTA1XX_I2CADDRCNT_RDADDR_SHIFT) & DTA1XX_I2CADDRCNT_RDADDR_MASK;
//	Dta1xxGenSetI2cAddrCnt(pBase, Val);
//}

// I2C Address/Count: Read Count
static __inline UInt  Dta1xxGenI2cAddrCntRegGetRdCount(volatile UInt8* pBase) {
	UInt  Val = Dta1xxGenGetI2cAddrCnt(pBase) & DTA1XX_I2CADDRCNT_RDCNT_MASK;
	return (Val >> DTA1XX_I2CADDRCNT_RDCNT_SHIFT);
}
//static __inline void  Dta1xxGenI2cAddrCntRegSetRdCount(volatile UInt8* pBase, UInt Addr) {
//	UInt  Val = Dta1xxGenGetI2cAddrCnt(pBase);
//	Val &= ~DTA1XX_I2CADDRCNT_RDCNT_MASK;
//	Val |= (Addr << DTA1XX_I2CADDRCNT_RDCNT_SHIFT) & DTA1XX_I2CADDRCNT_RDCNT_MASK;
//	Dta1xxGenSetI2cAddrCnt(pBase, Val);
//}

//-.-.-.-.-.-.-.-.-.-.-.-.- I2C Data Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// I2C Data: Register access
static __inline UInt  Dta1xxGenGetI2cData(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_I2CDATA);
}
static __inline void Dta1xxGenSetI2cData(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_I2CDATA);
}

//.-.-.-.-.-.-.-.-.-.-.-.- Licensing Registers: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// Licensing Serial-Number: Register access
static __inline UInt  Dta1xxGenGetLicSerialReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_LICSERIAL);
}
static __inline void Dta1xxGenSetLicSerialReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_LICSERIAL);
}
// Licensing Code: Register access
static __inline UInt  Dta1xxGenGetLicCodeReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_LICCODE);
}
static __inline void Dta1xxGenSetLicCodeReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_GEN_REG_LICCODE);
}
// Licensing Status: Register access
static __inline UInt  Dta1xxGenGetLicStatus1Reg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_LICSTATUS1);
}
static __inline UInt  Dta1xxGenGetLicStatus2Reg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_GEN_REG_LICSTATUS2);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Dta1xxTx +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Register block for a TRANSMISSION channel.
//
// Supports: DTA-100, DTA-102, DTA-140
//
typedef struct _Dta1xxTx {

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- General-Control register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// On DTA-140, these fields are moved to the generic register block.
	//
	struct {								// General-Control register
	volatile UInt  m_PE				: 1;	//  Serial EEPROM Program Enable
	volatile UInt  m_PRE			: 1;	//  Serial EEPROM Protect Register Enable
	volatile UInt  m_Reset			: 1;	//  Soft reset of DTA 100 circuitry
			 UInt  m_Reserved1		: 5;	//
			 UInt  m_FirmwRev		: 8;	//  Firmware Revision
			 UInt  m_TypeNum		: 8;	//  Board Type Number
	}  m_GenControl;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Transmit-Control register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	struct {
	volatile UInt  m_TxMode			: 2;	//  Transmit Mode: 188/204/Add16/Raw
	volatile UInt  m_BurstMode		: 1;	//  Burst Mode: 1 = send packets in bursts
	volatile UInt  m_TxModeExt		: 1;	//	Transmit Mode extension: 192
	volatile UInt  m_PckStuff		: 1;	//  Null-Packet Stuffing On/Off
	volatile UInt  m_TxCtrl			: 2;	//  Transmit Control: Idle/Hold/Send
	volatile UInt  m_TxDis			: 1;	//  Disable LVDS outputs
	volatile UInt  m_PerIntEn		: 1;	//  Periodic-Interrupt Enable
	volatile UInt  m_UflIntEn		: 1;	//  Output-FIFO Underflow Interrupt Enable
	volatile UInt  m_SyncIntEn		: 1;	//  Synchronisation-Error Interrupt Enable
	volatile UInt  m_ShortIntEn		: 1;	//  Short-Circuit-Detected Interrupt Enable
	volatile UInt  m_ThrIntEn		: 1;	//  Threshold-Crossed Interrupt Enable
	volatile UInt  m_FrameIntEn		: 1;	//  Frame synchronisation Interrupt Enable
			 UInt  m_Reserved		: 2;
	volatile UInt  m_UseExtClk		: 1;	//  Use External Clock input
	volatile UInt  m_EnaPwr			: 1;	//  Enable Power to target adapter
	volatile UInt  m_ShortErr		: 1;	//  Short-Circuit-Detected Error
	volatile UInt  m_LedControl		: 1;	//  Take over LED Control
	volatile UInt  m_LedGreen		: 1;	//  State of Green LED (if LedControl is set)
	volatile UInt  m_LedRed			: 1;	//  State of Red LED (if LedControl is set)
	volatile UInt  m_ClrFifo		: 1;	//  Clear FIFO (write only)
	volatile UInt  m_TxRst			: 1;	//  Reset Transmit Channel (write only)
	volatile UInt  m_TxInvAsi		: 1;	//  Invert DVB/ASI output signal
	volatile UInt  m_SdiMode		: 1;	//  SDI mode enable/disbale
	volatile UInt  m_Sdi10Bit		: 1;	//  SDI 10-bit mode
	volatile UInt  m_TxOutEn		: 1;	//  Transmit channel output enable
    volatile UInt  m_TxTimestamp    : 1;    //  Transmit on time(stamp)
    volatile UInt  m_TxLock         : 1;    //  Lock transmit rate to input
	volatile UInt  m_SdiHuffEn		: 1;	//  Enable Huffman decompression
    volatile UInt  m_TxTsRel		: 1;	//  Use relative mode for tx-on-time

    }  m_TxControl;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Transmit-Status register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// Status register is a union.
	// WATCH OUT: DO NOT clear interrupts by something like p->Fields.m_PerInt = 1;
	// THIS WOULD CLEAR ALL STATUS FLAGS!!
	union {
	struct {
	volatile UInt  m_FifoFilled		: 1;	//  Output-FIFO Filled flag
			 UInt  m_Reserved1		: 3;	//
	volatile UInt  m_SdramSize		: 4;	//  SDRAM Size
	volatile UInt  m_PerInt			: 1;	//  Periodic Interrupt
	volatile UInt  m_UflInt			: 1;	//  Output-FIFO Underflow Interrupt
	volatile UInt  m_SyncInt		: 1;	//  Synchronisation-Error Interrupt
	volatile UInt  m_ShortInt		: 1;	//  Short-Circuit-Detected Interrupt
	volatile UInt  m_ThrInt			: 1;	//  Threshold-Crossed Interrupt
	}				 Fields;				// For access to individual field
			 UInt32  All;					// For access to TxStatus in one go
	} m_TxStatus;

	volatile UInt32  m_TxClock;				// Transmit-Clock register
	volatile UInt32  m_ClkCnt;				// Clock-Count register
	volatile UInt32  m_FifoSize;			// FIFO-Size register
	volatile UInt32  m_FifoLoad;			// FIFO-Load register

	struct {								// Diagnostics register
	volatile UInt  m_SfData			: 8;	//  Data at output of Smoothing FIFO
	volatile UInt  m_LoopBack		: 1;	//	Loop-back output of Smoothing FIFO
	volatile UInt  m_SfDtValid		: 1;	//  Smoothing FIFO Data Vaild
	volatile UInt  m_BfFull			: 1;	//	Burst-FIFO-Full flag
	volatile UInt  m_DmaReq			: 1;	//  DMA Request from Burst FIFO flag
	volatile UInt  m_BfCanBurst		: 1;	//  Burst-FIFO Can-Burst flag
	volatile UInt  m_SfCanBurst		: 1;	//  Smoothing-FIFO Can-Burst flag
	volatile UInt  m_SfLoad			: 10;	//  Smoothing-FIFO Load in #bytes
	volatile UInt  m_BfLoad			: 8;	//  Burst-FIFO Load in #32-bit words
	}  m_Diag;

	volatile UInt32  m_LoopBackData;		// Data at output of Smoothing FIFO

	struct {								// Threshold-Control register
	volatile UInt  m_UpdBusy		: 1;	//  Threshold-Update process busy
	volatile UInt  m_CmpB			: 1;	//  Output of comparator B
	volatile UInt  m_CmpA			: 1;	//  Output of comparator A
	volatile UInt  m_AorB			: 1;	//  Update threshold A or threshold B
			 UInt  m_Reserved1		: 4;	//
	volatile UInt  m_ThreshA		: 8;	//  Threshold A
	volatile UInt  m_ThreshB		: 8;	//  Threshold B
	}  m_ThreshControl;

	volatile UInt32  m_Reserved28;			// Not used
	volatile UInt32  m_TxClockMod;			// Transmit-Clock modulus register
	volatile UInt32  m_Reserved30;			// Not used
	volatile UInt32  m_Reserved34;			// Not used
	volatile UInt32  m_Reserved38;			// Not used
	volatile UInt32  m_Reserved3C;			// Not used
	volatile UInt32  m_FifoData[16];		// Transport-stream data: 4 bytes at a time
} Dta1xxTx;

// Tx Registers: BYTE offset
#define DTA1XX_TX_REG_GENCTRL		0x0000
#define DTA1XX_TX_REG_TXCTRL		0x0004
#define DTA1XX_TX_REG_TXSTATUS		0x0008
#define DTA1XX_TX_REG_TXCLOCK		0x000C
#define DTA1XX_TX_REG_CLOCKCNT		0x0010
#define DTA1XX_TX_REG_TXCTRL2		0x0010
#define DTA1XX_TX_REG_FIFOSIZE		0x0014
#define DTA1XX_TX_REG_FIFOLOAD		0x0018
#define DTA1XX_TX_REG_DIAG			0x001C
#define DTA1XX_TX_REG_LOOPBKDATA	0x0020
#define DTA1XX_TX_REG_THRESHCTRL	0x0024
#define DTA1XX_TX_REG_MOD_CONTROL3	0x0024		// Reserved for future use
#define DTA1XX_TX_REG_MOD_CONTROL	0x0028
#define DTA1XX_TX_REG_MOD_CONTROL2	0x002C // for modulator channel
#define DTA1XX_TX_REG_TXCLOCK_MOD	0x002C // for ASI/SDI channel
#define DTA1XX_TX_REG_IDAC_CONTROL	0x0030
#define DTA1XX_TX_REG_QDAC_CONTROL	0x0034
#define DTA1XX_TX_REG_RF_CONTROL	0x0038
#define DTA1XX_TX_REG_IF_CONTROL	0x0038
#define DTA1XX_TX_REG_RF_CONTROL2	0x003C
#define DTA1XX_TX_REG_FIFO_FIRST	0x0040
#define DTA1XX_TX_REG_RFDAC_CONTROL	0x0050
#define DTA1XX_TX_REG_RFDAC_DATA	0x0054

// GS2962 Transmitter specific
#define DTA1XX_TX_REG_GS2962SPI		0x0060
#define DTA1XX_TX_REG_GS2962CTRL	0x0064
#define DTA1XX_TX_REG_GS2962STAT	0x0068


#define DTA1XX_TX_REG_FIFO_LAST		0x007C

//.-.-.-.-.-.-.-.-.-.-.-.- Tx Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// Tx Control: Bit fields
#define DTA1XX_TXCTRL_TXMODE			0x00000003
#define DTA1XX_TXCTRL_BURSTMODE			0x00000004		// ASI
#define DTA1XX_TXCTRL_SDILINEMODE		0x00000004		// SDI
#define DTA1XX_TXCTRL_TXMODE_EXT		0x00000008
#define DTA1XX_TXCTRL_PCKSTUFF			0x00000010
#define DTA1XX_TXCTRL_TXCTRL			0x00000060
#define DTA1XX_TXCTRL_TXDIS				0x00000080
#define DTA1XX_TXCTRL_PERINT_EN			0x00000100
#define DTA1XX_TXCTRL_UFLINT_EN			0x00000200
#define DTA1XX_TXCTRL_SYNCINT_EN		0x00000400
#define DTA1XX_TXCTRL_SHORTINT_EN		0x00000800
#define DTA1XX_TXCTRL_THRINT_EN			0x00001000
#define DTA1XX_TXCTRL_FRAMEINT_EN		0X00002000
#define DTA1XX_TXCTRL_USE_EXTCLOCK		0x00010000
#define DTA1XX_TXCTRL_ENAPWR			0x00020000
#define DTA1XX_TXCTRL_SHORTERROR		0x00040000
#define DTA1XX_TXCTRL_LEDCTRL			0x00080000
#define DTA1XX_TXCTRL_GREENLED			0x00100000
#define DTA1XX_TXCTRL_REDLED			0x00200000
#define DTA1XX_TXCTRL_CLEARFIFO			0x00400000
#define DTA1XX_TXCTRL_TXRESET			0x00800000
#define DTA1XX_TXCTRL_TXINVASI			0x01000000
#define DTA1XX_TXCTRL_SDI_MODE			0x02000000
#define DTA1XX_TXCTRL_SDI_10BIT			0x04000000
#define DTA1XX_TXCTRL_OUT_EN			0x08000000
#define DTA1XX_TXCTRL_TXONTIME		0x10000000
#define DTA1XX_TXCTRL_LOCK2INP			0x20000000
#define DTA1XX_TXCTRL_HUFF_EN			0x40000000
#define DTA1XX_TXCTRL_TXTSREL			0x80000000

// Tx Control: Register access
static __inline UInt  Dta1xxTxGetTxCtrlReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_TXCTRL);
}
static __inline void Dta1xxTxSetTxCtrlReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_TXCTRL);
}

// Tx Control2: Register access
static __inline UInt  Dta1xxTxGetTxCtrl2Reg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_TXCTRL2);
}
static __inline void Dta1xxTxSetTxCtrl2Reg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_TXCTRL2);
}

// Tx Control: TxMode
static __inline UInt  Dta1xxTxCtrlRegGetTxMode(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);

	return (Val & DTA1XX_TXCTRL_TXMODE) |
			((Val & DTA1XX_TXCTRL_TXMODE_EXT) >> 1);
}
static __inline void  Dta1xxTxCtrlRegSetTxMode(volatile UInt8* pBase, UInt TxMode) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	Val &= ~(DTA1XX_TXCTRL_TXMODE | DTA1XX_TXCTRL_TXMODE_EXT);
	Val = Val | (TxMode & DTA1XX_TXCTRL_TXMODE);
	if (TxMode & 0x4) Val = Val | DTA1XX_TXCTRL_TXMODE_EXT;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: BurstMode
static __inline UInt  Dta1xxTxCtrlRegGetTxBurstMode(volatile UInt8* pBase) {
	return (Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_BURSTMODE)? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetTxBurstMode(volatile UInt8* pBase, UInt BurstMode) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (BurstMode != 0)	Val |=  DTA1XX_TXCTRL_BURSTMODE;
	else				Val &= ~DTA1XX_TXCTRL_BURSTMODE;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: SDI Line mode
static __inline UInt  Dta1xxTxCtrlRegGetSdiLineMode(volatile UInt8* pBase) {
	return (Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_SDILINEMODE)? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetSdiLineMode(volatile UInt8* pBase, UInt En525Mode) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (En525Mode != 0)	Val |=  DTA1XX_TXCTRL_SDILINEMODE;
	else				Val &= ~DTA1XX_TXCTRL_SDILINEMODE;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: Packet Stuffing
static __inline UInt  Dta1xxTxCtrlRegGetTxPckStuff(volatile UInt8* pBase) {
	return (Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_PCKSTUFF)? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetTxPckStuff(volatile UInt8* pBase, UInt PckStuff) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (PckStuff != 0)	Val |=  DTA1XX_TXCTRL_PCKSTUFF;
	else				Val &= ~DTA1XX_TXCTRL_PCKSTUFF;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: TxCtrl
static __inline UInt  Dta1xxTxCtrlRegGetTxCtrl(volatile UInt8* pBase) {
	return (Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_TXCTRL) >> 5;
}
static __inline void  Dta1xxTxCtrlRegSetTxCtrl(volatile UInt8* pBase, UInt TxCtrl) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	Val &= ~DTA1XX_TXCTRL_TXCTRL;
	Val = Val | (TxCtrl << 5);
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: PerIntEn
static __inline UInt  Dta1xxTxCtrlRegGetTxPerIntEn(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_PERINT_EN)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetTxPerIntEn(volatile UInt8* pBase, UInt TxPerIntEn) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (TxPerIntEn != 0)	Val |=  DTA1XX_TXCTRL_PERINT_EN;
	else					Val &= ~DTA1XX_TXCTRL_PERINT_EN;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: TxUflIntEn
static __inline UInt  Dta1xxTxCtrlRegGetTxUflIntEn(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_UFLINT_EN)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetTxUflIntEn(volatile UInt8* pBase, UInt TxUflIntEn) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (TxUflIntEn != 0)	Val |=  DTA1XX_TXCTRL_UFLINT_EN;
	else					Val &= ~DTA1XX_TXCTRL_UFLINT_EN;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: TxSyncIntEn
static __inline UInt  Dta1xxTxCtrlRegGetTxSyncIntEn(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_SYNCINT_EN)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetTxSyncIntEn(volatile UInt8* pBase, UInt TxSyncIntEn) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (TxSyncIntEn != 0)	Val |=  DTA1XX_TXCTRL_SYNCINT_EN;
	else					Val &= ~DTA1XX_TXCTRL_SYNCINT_EN;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: TxShortIntEn
static __inline UInt  Dta1xxTxCtrlRegGetTxShortIntEn(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_SHORTINT_EN)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetTxShortIntEn(volatile UInt8* pBase, UInt TxShortIntEn) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (TxShortIntEn != 0)	Val |=  DTA1XX_TXCTRL_SHORTINT_EN;
	else					Val &= ~DTA1XX_TXCTRL_SHORTINT_EN;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: TxThrIntEn (thresh-hold)
static __inline UInt  Dta1xxTxCtrlRegGetTxThrIntEn(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_THRINT_EN)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetTxThrIntEn(volatile UInt8* pBase, UInt TxThrIntEn) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (TxThrIntEn != 0)	Val |=  DTA1XX_TXCTRL_THRINT_EN;
	else					Val &= ~DTA1XX_TXCTRL_THRINT_EN;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: TxDis
static __inline UInt  Dta1xxTxCtrlRegGetTxDis(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_TXDIS)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetTxDis(volatile UInt8* pBase, UInt TxDis) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (TxDis != 0)	Val |=  DTA1XX_TXCTRL_TXDIS;
	else			Val &= ~DTA1XX_TXCTRL_TXDIS;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: UseExtClk
static __inline UInt  Dta1xxTxCtrlRegGetUseExtClk(volatile UInt8* pBase) {
        return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_USE_EXTCLOCK)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetUseExtClk(volatile UInt8* pBase, UInt UseExtClk) {
        UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
        if (UseExtClk != 0)	Val |=  DTA1XX_TXCTRL_USE_EXTCLOCK;
        else				Val &= ~DTA1XX_TXCTRL_USE_EXTCLOCK;
        Dta1xxTxSetTxCtrlReg(pBase, Val);
}


// Tx Control: EnaPwr
static __inline UInt  Dta1xxTxCtrlRegGetEnaPwr(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_ENAPWR)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetEnaPwr(volatile UInt8* pBase, UInt EnaPwr) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (EnaPwr != 0)	Val |=  DTA1XX_TXCTRL_ENAPWR;
	else				Val &= ~DTA1XX_TXCTRL_ENAPWR;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: LedControl
static __inline UInt  Dta1xxTxCtrlRegGetLedControl(volatile UInt8* pBase) {
        return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_LEDCTRL)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetLedControl(volatile UInt8* pBase, UInt LedControl) {
        UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
        if (LedControl != 0)	Val |=  DTA1XX_TXCTRL_LEDCTRL;
        else					Val &= ~DTA1XX_TXCTRL_LEDCTRL;
        Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: LedGreen
static __inline UInt  Dta1xxTxCtrlRegGetLedGreen(volatile UInt8* pBase) {
        return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_GREENLED)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetLedGreen(volatile UInt8* pBase, UInt LedGreen) {
        UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
        if (LedGreen != 0)	Val |=  DTA1XX_TXCTRL_GREENLED;
        else				Val &= ~DTA1XX_TXCTRL_GREENLED;
        Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: LedRed
static __inline UInt  Dta1xxTxCtrlRegGetLedRed(volatile UInt8* pBase) {
        return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_REDLED)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetLedRed(volatile UInt8* pBase, UInt LedRed) {
        UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
        if (LedRed != 0)	Val |=  DTA1XX_TXCTRL_REDLED;
        else				Val &= ~DTA1XX_TXCTRL_REDLED;
        Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: Clear-FIFO
static __inline void  Dta1xxTxCtrlRegClrFifo(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	Val |=  DTA1XX_TXCTRL_CLEARFIFO;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: Reset
static __inline void  Dta1xxTxCtrlRegTxReset(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	Val |=  DTA1XX_TXCTRL_TXRESET;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: TxInvAsi
static __inline UInt  Dta1xxTxCtrlRegGetTxAsiInv(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_TXINVASI)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetTxAsiInv(volatile UInt8* pBase, UInt TxInvAsi) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (TxInvAsi != 0)	Val |=  DTA1XX_TXCTRL_TXINVASI;
	else				Val &= ~DTA1XX_TXCTRL_TXINVASI;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: SDI mode
static __inline UInt  Dta1xxTxCtrlRegGetSdiMode(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_SDI_MODE)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetSdiMode(volatile UInt8* pBase, UInt SdiMode) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (SdiMode != 0)		Val |=  DTA1XX_TXCTRL_SDI_MODE;
	else					Val &= ~DTA1XX_TXCTRL_SDI_MODE;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: SDI 10-Bit mode
static __inline UInt Dta1xxTxCtrlRegGetSdiMode10Bit(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_SDI_10BIT)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetSdiMode10Bit(volatile UInt8* pBase, UInt SdiMode10Bit) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (SdiMode10Bit != 0)	Val |=  DTA1XX_TXCTRL_SDI_10BIT;
	else					Val &= ~DTA1XX_TXCTRL_SDI_10BIT;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: Output enable
static __inline UInt  Dta1xxTxCtrlRegGetOutputEn(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_OUT_EN)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetOutputEn(volatile UInt8* pBase, UInt OutputEnable) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (OutputEnable != 0)	Val |=  DTA1XX_TXCTRL_OUT_EN;
	else					Val &= ~DTA1XX_TXCTRL_OUT_EN;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: Transmit on time(stamp)
static __inline UInt Dta1xxTxCtrlRegGetTxOnTime(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_TXONTIME)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetTxOnTime(volatile UInt8* pBase, UInt TxOnTime) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (TxOnTime != 0)   Val |=  DTA1XX_TXCTRL_TXONTIME;
	else				 Val &= ~DTA1XX_TXCTRL_TXONTIME;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: lock-to-input
static __inline UInt Dta1xxTxCtrlRegGetLock2Input(volatile UInt8* pBase) {
    return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_LOCK2INP)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetLock2Input(volatile UInt8* pBase, UInt Lock2Inp) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (Lock2Inp != 0)   Val |=  DTA1XX_TXCTRL_LOCK2INP;
	else				 Val &= ~DTA1XX_TXCTRL_LOCK2INP;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

// Tx Control: SDI Huffman decompression enable
static __inline UInt Dta1xxTxCtrlRegGetSdiDecomprModeEn(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxCtrlReg(pBase)&DTA1XX_TXCTRL_HUFF_EN)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxCtrlRegSetSdiDecomprModeEn(volatile UInt8* pBase, UInt HuffEn) {
	UInt  Val = Dta1xxTxGetTxCtrlReg(pBase);
	if (HuffEn != 0)	Val |=  DTA1XX_TXCTRL_HUFF_EN;
	else				Val &= ~DTA1XX_TXCTRL_HUFF_EN;
	Dta1xxTxSetTxCtrlReg(pBase, Val);
}

//-.-.-.-.-.-.-.-.-.-.-.- Tx Control 2 register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

// Tx Control2: Bit fields
#define DTA1XX_TXCTRL2_SRCSEL			0x0000000F
#define DTA1XX_TXCTRL2_LOCKSEL			0x000000F0
#define DTA1XX_TXCTRL2_RATESEL			0x00000100

// Tx Control2: Transmit output source select
static __inline void Dta1xxTxCtrl2RegSetSrcSel(volatile UInt8* pBase, UInt SrcSel)
{
	UInt  Val = Dta1xxTxGetTxCtrl2Reg(pBase);
	Val &= ~DTA1XX_TXCTRL2_SRCSEL;
	Val |=  SrcSel;
	Dta1xxTxSetTxCtrl2Reg(pBase, Val);
}

// Tx Control2: Transmit lock channel select
static __inline void Dta1xxTxCtrl2RegSetLockSel(volatile UInt8* pBase, UInt LockSel)
{
	UInt  Val = Dta1xxTxGetTxCtrl2Reg(pBase);
	Val &= ~DTA1XX_TXCTRL2_LOCKSEL;
	Val |=  LockSel << 4;
	Dta1xxTxSetTxCtrl2Reg(pBase, Val);
}

// Tx Control2: Transmit rate select
static __inline void Dta1xxTxCtrl2RegSetRateSelExt(volatile UInt8* pBase)
{
	UInt  Val = Dta1xxTxGetTxCtrl2Reg(pBase);
	Val |= DTA1XX_TXCTRL2_RATESEL;
	Dta1xxTxSetTxCtrl2Reg(pBase, Val);
}
static __inline void Dta1xxTxCtrl2RegSetRateSelInt(volatile UInt8* pBase)
{
	UInt  Val = Dta1xxTxGetTxCtrl2Reg(pBase);
	Val &= ~DTA1XX_TXCTRL2_RATESEL;
	Dta1xxTxSetTxCtrl2Reg(pBase, Val);
}
static __inline int Dta1xxTxCtrl2RegIsRateSelExtClk(volatile UInt8* pBase)
{
	return (Dta1xxTxGetTxCtrl2Reg(pBase) & DTA1XX_TXCTRL2_RATESEL) == DTA1XX_TXCTRL2_RATESEL;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- Tx Status register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// Tx Status: Bit fields
#define DTA1XX_TXSTAT_FIFOFILL		0x00000001
#define DTA1XX_TXSTAT_SDRAMSIZE		0x000000F0
#define DTA1XX_TXSTAT_PERINT		0x00000100
#define DTA1XX_TXSTAT_UFLINT		0x00000200
#define DTA1XX_TXSTAT_SYNCINT		0x00000400
#define DTA1XX_TXSTAT_SHORTINT		0x00000800
#define DTA1XX_TXSTAT_THRINT		0x00001000

// Tx Status: Register access
static __inline UInt  Dta1xxTxGetTxStatusReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_TXSTATUS);
}
static __inline void  Dta1xxTxSetTxStatusReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_TXSTATUS);
}

// Tx Status: FifoFilled
static __inline UInt  Dta1xxTxStatusRegGetFifoFilled(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxStatusReg(pBase) & DTA1XX_TXSTAT_FIFOFILL)!=0) ? 1 : 0;
}

// Tx Status: SdramSize
static __inline UInt  Dta1xxTxStatusRegGetSdramSize(volatile UInt8* pBase) {
	return (Dta1xxTxGetTxStatusReg(pBase) & DTA1XX_TXSTAT_SDRAMSIZE) >> 4;
}

// Tx Status: periodic interrupt
static __inline UInt  Dta1xxTxStatusRegGetPerInt(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxStatusReg(pBase)&DTA1XX_TXSTAT_PERINT)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxStatusRegClrPerInt(volatile UInt8* pBase) {
	Dta1xxTxSetTxStatusReg(pBase, DTA1XX_TXSTAT_PERINT);
}

// Tx Status: underflow interrupt
static __inline UInt  Dta1xxTxStatusRegGetUflInt(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxStatusReg(pBase)&DTA1XX_TXSTAT_UFLINT)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxStatusRegClrUflInt(volatile UInt8* pBase) {
	Dta1xxTxSetTxStatusReg(pBase, DTA1XX_TXSTAT_UFLINT);
}

// Tx Status: sync interrupt
static __inline UInt  Dta1xxTxStatusRegGetSyncInt(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxStatusReg(pBase)&DTA1XX_TXSTAT_SYNCINT)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxStatusRegClrSyncInt(volatile UInt8* pBase) {
	Dta1xxTxSetTxStatusReg(pBase, DTA1XX_TXSTAT_SYNCINT);
}

// Tx Status: short-circuit interrupt
static __inline UInt  Dta1xxTxStatusRegGetShortInt(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxStatusReg(pBase)&DTA1XX_TXSTAT_SHORTINT)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxStatusRegClrShortInt(volatile UInt8* pBase) {
	Dta1xxTxSetTxStatusReg(pBase, DTA1XX_TXSTAT_SHORTINT);
}

// Tx Status: threshold interrupt
static __inline UInt  Dta1xxTxStatusRegGetThresholdInt(volatile UInt8* pBase) {
	return ((Dta1xxTxGetTxStatusReg(pBase)&DTA1XX_TXSTAT_THRINT)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxStatusRegClrThresholdInt(volatile UInt8* pBase) {
	Dta1xxTxSetTxStatusReg(pBase, DTA1XX_TXSTAT_THRINT);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Transmit Clock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

static __inline UInt  Dta1xxTxGetTxClock(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_TXCLOCK);
}
static __inline void  Dta1xxTxSetTxClock(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_TXCLOCK);
}

//.-.-.-.-.-.-.-.-.-.-..- Clock Count register: Access Functions .-.-.-.-.-.-.-.-.-.-.-.-.

static __inline UInt  Dta1xxTxGetClkCountReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_CLOCKCNT);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- FIFO-Size register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// FIFO Size: Register access
static __inline UInt  Dta1xxTxGetFifoSizeReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_FIFOSIZE);
}
static __inline void  Dta1xxTxSetFifoSizeReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_FIFOSIZE);
}

// FIFO Load: Register access
static __inline UInt  Dta1xxTxGetFifoLoadReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_FIFOLOAD);
}

//.-.-.-.-.-.-.-.-.-.-.-.- Diagnostics register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

// Diagnostics register: Bit fields
#define DTA1XX_TXDIAG_SFDATA		0x000000FF
#define DTA1XX_TXDIAG_LOOPBACK		0x00000100
#define DTA1XX_TXDIAG_SFDTVALID		0x00000200
#define DTA1XX_TXDIAG_BFFULL		0x00000400
#define DTA1XX_TXDIAG_DMAREQ		0x00000800
#define DTA1XX_TXDIAG_BFCANBURST	0x00001000
#define DTA1XX_TXDIAG_SFCANBURST	0x00002000
#define DTA1XX_TXDIAG_SFLOAD		0x00FC0000
#define DTA1XX_TXDIAG_BFLOAD		0xFF000000

// Diagnostics register: Register access
static __inline UInt  Dta1xxTxGetDiagReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_DIAG);
}
static __inline void  Dta1xxTxSetDiagReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_DIAG);
}

// Diagnostics register: LoopBack
static __inline UInt  Dta1xxTxDiagRegGetLoopBack(volatile UInt8* pBase) {
	return ((Dta1xxTxGetDiagReg(pBase)&DTA1XX_TXDIAG_LOOPBACK)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxDiagRegSetLoopBack(volatile UInt8* pBase, UInt LoopBack) {
	UInt  Val = Dta1xxTxGetDiagReg(pBase);
	if (LoopBack != 0)	Val |=  DTA1XX_TXDIAG_LOOPBACK;
	else				Val &= ~DTA1XX_TXDIAG_LOOPBACK;
	Dta1xxTxSetDiagReg(pBase, Val);
}

// Diagnostics register: BfLoad
static __inline UInt  Dta1xxTxDiagRegGetBfLoad(volatile UInt8* pBase) {
	return (Dta1xxTxGetDiagReg(pBase)&DTA1XX_TXDIAG_BFLOAD) >> 24;
}


//.-.-.-.-.-.-.-.-.-.-.- Loop-Back Data register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-

// Loop-Back Data: Register access
static __inline UInt  Dta1xxTxGetLoopBackDataReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_LOOPBKDATA);
}


//.-.-.-.-.-.-.-.-.-.- Modulation-Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-

// Modulation-Control register: Bit fields
#define DTA1XX_TXMODC_M_MSK			0x0000000F
#define DTA1XX_TXMODC_M_SH			0
#define DTA1XX_TXMODC_IQMAP_MSK		0x0000000F
#define DTA1XX_TXMODC_IQMAP_SH		0
#define DTA1XX_TXMODC_MODTYPE_MSK	0x000000F0
#define DTA1XX_TXMODC_MODTYPE_SH	4
#define DTA1XX_TXMODC_CODERATE_MSK	0x00000F00
#define DTA1XX_TXMODC_CODERATE_SH	8
#define DTA1XX_TXMODC_INTPOL_MSK	0x00000E00
#define DTA1XX_TXMODC_INTPOL_SH		9
#define DTA1XX_TXMODC_ROLLOFF_MSK	0x0000F000
#define DTA1XX_TXMODC_ROLLOFF_SH	12
#define DTA1XX_TXMODC_OUTLVL_MSK	0x00FF0000
#define DTA1XX_TXMODC_OUTLVL_SH		16
#define DTA1XX_TXMODC_USER_LEVEL	0x01000000
#define DTA1XX_TXMODC_SPEC_INV		0x02000000
#define DTA1XX_TXMODC_MUTE_I		0x04000000
#define DTA1XX_TXMODC_MUTE_I_SH     26
#define DTA1XX_TXMODC_MUTE_Q		0x08000000
#define DTA1XX_TXMODC_MUTE_Q_SH     27
#define DTA1XX_TXMODC_TESTPAT_MSK	0xF0000000
#define DTA1XX_TXMODC_TESTPAT_SH	28

// Modulation-Type field: values
// Values must be the same as DTU2XX_TXMODC_xxx
#define DTA1XX_TXMODC_QPSK			0
#define DTA1XX_TXMODC_BPSK			1
#define DTA1XX_TXMODC_QAM4			3
#define DTA1XX_TXMODC_QAM16			4
#define DTA1XX_TXMODC_QAM32			5
#define DTA1XX_TXMODC_QAM64			6
#define DTA1XX_TXMODC_QAM128		7
#define DTA1XX_TXMODC_QAM256		8
#define DTA1XX_TXMODC_IQMAP			9		// I/Q Mapping
											// On DTA-107S2: 4-bit symbol mode
#define DTA1XX_TXMODC_IQDIRBUF		14		// Direct I/Q, buffered
#define DTA1XX_TXMODC_IQDIRECT		15		// Direct I/Q

// I/Q Mapping field: values
// Values must be the same as DTU2XX_IQMAP_xxx
#define DTA1XX_IQMAP_QAM			0		// General QAM I/Q mapping
#define DTA1XX_IQMAP_QAM16			1		// Optimised I/Q mapping for 16-QAM
#define DTA1XX_IQMAP_QAM32			2		// Optimised I/Q mapping for 32-QAM
#define DTA1XX_IQMAP_QAM64			3		// Optimised I/Q mapping for 64-QAM
#define DTA1XX_IQMAP_VSB8			4
#define DTA1XX_IQMAP_VSB16			5

// Roll-Off factor field: values
// Values must be the same as DTU2XX_ROLOFF_xxx
#define DTA1XX_ROLLOFF_12			0
#define DTA1XX_ROLLOFF_13			1
#define DTA1XX_ROLLOFF_15			2
#define DTA1XX_ROLLOFF_18			3
#define DTA1XX_ROLLOFF_5			4

// Roll-Off factor field: values
// Values must be the same as DTU2XX_INTPOL_xxx
#define DTA1XX_INTPOL_QAM			0		// Use interpolator optimised for n-QAM
#define DTA1XX_INTPOL_OFDM			1		// Use interpolator optimised for OFDM

// Test patterns
// Values must be the same as DTU2XX_TP_xxx
#define DTA1XX_TP_NORMAL			0		// Normal mode
#define DTA1XX_TP_NYQUIST			1		// Nyquist test-pattern
#define DTA1XX_TP_HALFNYQ			2		// Half-nyquist tets-pattern
#define DTA1XX_TP_CW				3		// Carrier only
#define DTA1XX_TP_CWI				4		// Constant I
#define DTA1XX_TP_CWQ				5		// Constant Q

// Modulation-Control register: Register access
static __inline UInt  Dta1xxTxGetModControlReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_MOD_CONTROL);
}
static __inline void  Dta1xxTxSetModControlReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_MOD_CONTROL);
}

// Modulation-Control register: Channel-Filter Roll-Off Factor
static __inline UInt  Dta1xxTxModcRegGetRollOffFactor(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & DTA1XX_TXMODC_ROLLOFF_MSK;
	return (Val >> DTA1XX_TXMODC_ROLLOFF_SH);
}
static __inline void  Dta1xxTxModcRegSetRollOffFactor(volatile UInt8* pBase, UInt RollOff) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & ~DTA1XX_TXMODC_ROLLOFF_MSK;
	Dta1xxTxSetModControlReg(pBase, Val | (RollOff << DTA1XX_TXMODC_ROLLOFF_SH));
}

// Modulation-Control register: Code Rate
static __inline UInt  Dta1xxTxModcRegGetCodeRate(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & DTA1XX_TXMODC_CODERATE_MSK;
	return (Val >> DTA1XX_TXMODC_CODERATE_SH);
}
static __inline void  Dta1xxTxModcRegSetCodeRate(volatile UInt8* pBase, UInt CodeRate) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & ~DTA1XX_TXMODC_CODERATE_MSK;
	Dta1xxTxSetModControlReg(pBase, Val | (CodeRate << DTA1XX_TXMODC_CODERATE_SH));
}

// Modulation-Control register: Enable user level
static __inline void  Dta1xxTxModcRegEnableUserLevel(volatile UInt8* pBase, UInt Ena) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase);
	if (Ena == 0) Val &= ~DTA1XX_TXMODC_USER_LEVEL;
	else		  Val |=  DTA1XX_TXMODC_USER_LEVEL;
	Dta1xxTxSetModControlReg(pBase, Val);
}

// Modulation-Control register: Enable spectral inversion
static __inline void  Dta1xxTxModcRegEnableSpectralInv(volatile UInt8* pBase, UInt Ena) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase);
	if (Ena == 0) Val &= ~DTA1XX_TXMODC_SPEC_INV;
	else		  Val |=  DTA1XX_TXMODC_SPEC_INV;
	Dta1xxTxSetModControlReg(pBase, Val);
}

// Modulation-Control register: Interpolation Selection
static __inline UInt  Dta1xxTxModcRegGetIntPolSel(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & DTA1XX_TXMODC_INTPOL_MSK;
	return (Val >> DTA1XX_TXMODC_INTPOL_SH);
}
static __inline void  Dta1xxTxModcRegSetIntPolSel(volatile UInt8* pBase, UInt IntPolSel) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & ~DTA1XX_TXMODC_INTPOL_MSK;
	Dta1xxTxSetModControlReg(pBase, Val | (IntPolSel << DTA1XX_TXMODC_INTPOL_SH));
}

// Modulation-Control register: I/Q Mapping
static __inline UInt  Dta1xxTxModcRegGetIqMap(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & DTA1XX_TXMODC_IQMAP_MSK;
	return (Val >> DTA1XX_TXMODC_IQMAP_SH);
}
static __inline void  Dta1xxTxModcRegSetIqMap(volatile UInt8* pBase, UInt IqMap) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & ~DTA1XX_TXMODC_IQMAP_MSK;
	Dta1xxTxSetModControlReg(pBase, Val | (IqMap << DTA1XX_TXMODC_IQMAP_SH));
}

// Modulation-Control register: Modulation Type
static __inline UInt  Dta1xxTxModcRegGetModType(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & DTA1XX_TXMODC_MODTYPE_MSK;
	return (Val >> DTA1XX_TXMODC_MODTYPE_SH);
}
static __inline void  Dta1xxTxModcRegSetModType(volatile UInt8* pBase, UInt ModType) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & ~DTA1XX_TXMODC_MODTYPE_MSK;
	Dta1xxTxSetModControlReg(pBase, Val | (ModType << DTA1XX_TXMODC_MODTYPE_SH));
}

// Modulation-Control register: Multiplier M
static __inline UInt  Dta1xxTxModcRegGetM(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & DTA1XX_TXMODC_M_MSK;
	return (Val >> DTA1XX_TXMODC_M_SH);
}
static __inline void  Dta1xxTxModcRegSetM(volatile UInt8* pBase, UInt M) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & ~DTA1XX_TXMODC_M_MSK;
	Dta1xxTxSetModControlReg(pBase, Val | (M << DTA1XX_TXMODC_M_SH));
}

// Modulation-Control register: Mute I output
static __inline void  Dta1xxTxModcRegMuteI(volatile UInt8* pBase, UInt Mute) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase);
	if (Mute == 0) Val &= ~DTA1XX_TXMODC_MUTE_I;
	else		   Val |=  DTA1XX_TXMODC_MUTE_I;
	Dta1xxTxSetModControlReg(pBase, Val);
}

// Modulation-Control register: Mute Q output
static __inline void  Dta1xxTxModcRegMuteQ(volatile UInt8* pBase, UInt Mute) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase);
	if (Mute == 0) Val &= ~DTA1XX_TXMODC_MUTE_Q;
	else		   Val |=  DTA1XX_TXMODC_MUTE_Q;
	Dta1xxTxSetModControlReg(pBase, Val);
}

// Modulation-Control register: Output level (scaler value)
static __inline UInt  Dta1xxTxModcRegGetOutputLevel(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & DTA1XX_TXMODC_OUTLVL_MSK;
	return (Val >> DTA1XX_TXMODC_OUTLVL_SH);
}
static __inline void  Dta1xxTxModcRegSetOutputLevel(volatile UInt8* pBase, UInt Level) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & ~DTA1XX_TXMODC_OUTLVL_MSK;
	Dta1xxTxSetModControlReg(pBase, Val | (Level << DTA1XX_TXMODC_OUTLVL_SH));
}

// Modulation-Control register: Test Pattern
static __inline UInt  Dta1xxTxModcRegGetTestPat(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & DTA1XX_TXMODC_TESTPAT_MSK;
	return (Val >> DTA1XX_TXMODC_TESTPAT_SH);
}
static __inline void  Dta1xxTxModcRegSetTestPat(volatile UInt8* pBase, UInt TestPat) {
	UInt  Val = Dta1xxTxGetModControlReg(pBase) & ~DTA1XX_TXMODC_TESTPAT_MSK;
	Dta1xxTxSetModControlReg(pBase, Val | (TestPat << DTA1XX_TXMODC_TESTPAT_SH));
}

//-.-.-.-.-.-.-.-.-.- Modulation-Control 2 register: Access Functions -.-.-.-.-.-.-.-.-.-.

// Modulation-Control 2 register: Bit fields
#define DTA1XX_TXMODC2_OUTLVL_OFFSET_MSK	0x0000000F
#define DTA1XX_TXMODC2_OUTLVL_OFFSET_SH		0
#define DTA1XX_TXMODC2_NOISELVL_MSK			0x00000FF0
#define DTA1XX_TXMODC2_NOISELVL_SH			4
#define DTA1XX_TXMODC2_NOISERNG_MSK			0x00007000
#define DTA1XX_TXMODC2_NOISERNG_SH			12

// Modulation-Control register: Register access
static __inline UInt  Dta1xxTxGetModControl2Reg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_MOD_CONTROL2);
}
static __inline void  Dta1xxTxSetModControl2Reg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_MOD_CONTROL2);
}

// Modulation-Control 2 register: Output level offset (scaler value offset)
static __inline Int32  Dta1xxTxModc2RegGetOutputLevelOffset(volatile UInt8* pBase) {
	Int32  Val = (Int32)(Dta1xxTxGetModControl2Reg(pBase) & DTA1XX_TXMODC2_OUTLVL_OFFSET_MSK);
	Val >>= DTA1XX_TXMODC2_OUTLVL_OFFSET_SH;
	// Sign extension
	if ((Val&0x8)==0)	Val |= 0xFFFFFFF0;
	else				Val &= 0x0000000F;
	return Val;
}
static __inline void  Dta1xxTxModc2RegSetOutputLevelOffset(volatile UInt8* pBase, Int32 Offset) {
	UInt  Val = Dta1xxTxGetModControl2Reg(pBase) & ~DTA1XX_TXMODC2_OUTLVL_OFFSET_MSK;
	Offset <<= DTA1XX_TXMODC2_OUTLVL_OFFSET_SH;
	Offset &= DTA1XX_TXMODC2_OUTLVL_OFFSET_MSK;
	Dta1xxTxSetModControl2Reg(pBase, Val | Offset);
}

// Modulation-Control 2 register: Noise level
static __inline Int32  Dta1xxTxModc2RegGetNoiseLevel(volatile UInt8* pBase) {
	Int32  Val = (Int32)(Dta1xxTxGetModControl2Reg(pBase) & DTA1XX_TXMODC2_NOISELVL_MSK);
	return (Val>>DTA1XX_TXMODC2_NOISELVL_SH);
}
static __inline void  Dta1xxTxModc2RegSetNoiseLevel(volatile UInt8* pBase, Int32 Level) {
	UInt  Val = Dta1xxTxGetModControl2Reg(pBase) & ~DTA1XX_TXMODC2_NOISELVL_MSK;
	Level <<= DTA1XX_TXMODC2_NOISELVL_SH;
	Level &= DTA1XX_TXMODC2_NOISELVL_MSK;
	Dta1xxTxSetModControl2Reg(pBase, Val | Level);
}

// Modulation-Control 2 register: Noise range
static __inline Int32  Dta1xxTxModc2RegGetNoiseRange(volatile UInt8* pBase) {
	Int32  Val = (Int32)(Dta1xxTxGetModControl2Reg(pBase) & DTA1XX_TXMODC2_NOISERNG_MSK);
	return (Val>>DTA1XX_TXMODC2_NOISERNG_SH);
}
static __inline void  Dta1xxTxModc2RegSetNoiseRange(volatile UInt8* pBase, Int32 Range) {
	UInt  Val = Dta1xxTxGetModControl2Reg(pBase) & ~DTA1XX_TXMODC2_NOISERNG_MSK;
	Range <<= DTA1XX_TXMODC2_NOISERNG_SH;
	Range &= DTA1XX_TXMODC2_NOISERNG_MSK;
	Dta1xxTxSetModControl2Reg(pBase, Val | Range);
}

//.-.-.-.-.-.-.-.-.-.-.-.- TX Clock mod: Register acces -.-.-.-.-.-.-.-.-.-.-.-.-

// TX Clock Modulus: Register access
static __inline UInt  Dta1xxTxGetTxClockMod(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_TXCLOCK_MOD);
}
static __inline void  Dta1xxTxSetTxClockMod(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_TXCLOCK_MOD);
}

//-.-.-.-.-.-.-.-.-.- I- and Q-DAC-Control registers: Access Functions -.-.-.-.-.-.-.-.-.-

// I- and Q-DAC-Control registers: Bit fields
#define DTA1XX_TXDACC_FINEGAIN_MSK	0x000000FF
#define DTA1XX_TXDACC_FINEGAIN_SH	0
#define DTA1XX_TXDACC_OFFSDIR_MSK	0x00000100
#define DTA1XX_TXDACC_OFFSDIR_SH	8
#define DTA1XX_TXDACC_OFFSET_MSK	0x0007FE00
#define DTA1XX_TXDACC_OFFSET_SH		9

// I- and Q-DAC-Control registers:: Register access
static __inline UInt  Dta1xxTxGetIDacControlReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_IDAC_CONTROL);
}
static __inline UInt  Dta1xxTxGetQDacControlReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_QDAC_CONTROL);
}
static __inline void  Dta1xxTxSetIDacControlReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_IDAC_CONTROL);
}
static __inline void  Dta1xxTxSetQDacControlReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_QDAC_CONTROL);
}

// I- and Q-DAC-Control registers: Fine Gain
static __inline UInt  Dta1xxTxIDacCtrlGetFineGain(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetIDacControlReg(pBase) & DTA1XX_TXDACC_FINEGAIN_MSK;
	return (Val >> DTA1XX_TXDACC_FINEGAIN_SH);
}
static __inline UInt  Dta1xxTxQDacCtrlGetFineGain(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetQDacControlReg(pBase) & DTA1XX_TXDACC_FINEGAIN_MSK;
	return (Val >> DTA1XX_TXDACC_FINEGAIN_SH);
}
static __inline void  Dta1xxTxIDacCtrlSetFineGain(volatile UInt8* pBase, UInt FineGain) {
	UInt  Val = Dta1xxTxGetIDacControlReg(pBase) & ~DTA1XX_TXDACC_FINEGAIN_MSK;
	Dta1xxTxSetIDacControlReg(pBase, Val | (FineGain << DTA1XX_TXDACC_FINEGAIN_SH));
}
static __inline void  Dta1xxTxQDacCtrlSetFineGain(volatile UInt8* pBase, UInt FineGain) {
	UInt  Val = Dta1xxTxGetQDacControlReg(pBase) & ~DTA1XX_TXDACC_FINEGAIN_MSK;
	Dta1xxTxSetQDacControlReg(pBase, Val | (FineGain << DTA1XX_TXDACC_FINEGAIN_SH));
}

// I- and Q-DAC-Control registers: Offset Direction
static __inline UInt  Dta1xxTxIDacCtrlGetOffsDir(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetIDacControlReg(pBase) & DTA1XX_TXDACC_OFFSDIR_MSK;
	return (Val >> DTA1XX_TXDACC_OFFSDIR_SH);
}
static __inline UInt  Dta1xxTxQDacCtrlGetOffsDir(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetQDacControlReg(pBase) & DTA1XX_TXDACC_OFFSDIR_MSK;
	return (Val >> DTA1XX_TXDACC_OFFSDIR_SH);
}
static __inline void  Dta1xxTxIDacCtrlSetOffsDir(volatile UInt8* pBase, UInt OffsDir) {
	UInt  Val = Dta1xxTxGetIDacControlReg(pBase) & ~DTA1XX_TXDACC_OFFSDIR_MSK;
	Dta1xxTxSetIDacControlReg(pBase, Val | (OffsDir << DTA1XX_TXDACC_OFFSDIR_SH));
}
static __inline void  Dta1xxTxQDacCtrlSetOffsDir(volatile UInt8* pBase, UInt OffsDir) {
	UInt  Val = Dta1xxTxGetQDacControlReg(pBase) & ~DTA1XX_TXDACC_OFFSDIR_MSK;
	Dta1xxTxSetQDacControlReg(pBase, Val | (OffsDir << DTA1XX_TXDACC_OFFSDIR_SH));
}

// I- and Q-DAC-Control registers: Offset
static __inline UInt  Dta1xxTxIDacCtrlGetOffset(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetIDacControlReg(pBase) & DTA1XX_TXDACC_OFFSET_MSK;
	return (Val >> DTA1XX_TXDACC_OFFSET_SH);
}
static __inline UInt  Dta1xxTxQDacCtrlGetOffset(volatile UInt8* pBase) {
	UInt  Val = Dta1xxTxGetQDacControlReg(pBase) & DTA1XX_TXDACC_OFFSET_MSK;
	return (Val >> DTA1XX_TXDACC_OFFSET_SH);
}
static __inline void  Dta1xxTxIDacCtrlSetOffset(volatile UInt8* pBase, UInt Offset) {
	UInt  Val = Dta1xxTxGetIDacControlReg(pBase) & ~DTA1XX_TXDACC_OFFSET_MSK;
	Dta1xxTxSetIDacControlReg(pBase, Val | (Offset << DTA1XX_TXDACC_OFFSET_SH));
}
static __inline void  Dta1xxTxQDacCtrlSetOffset(volatile UInt8* pBase, UInt Offset) {
	UInt  Val = Dta1xxTxGetQDacControlReg(pBase) & ~DTA1XX_TXDACC_OFFSET_MSK;
	Dta1xxTxSetQDacControlReg(pBase, Val | (Offset << DTA1XX_TXDACC_OFFSET_SH));
}


//.-.-.-.-.-.-.-.-.-.-.-.- RF-Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// RF-Control register: Bit fields
#define DTA1XX_TXRFCTRL_PLLA_MSK			0x0000001F
#define DTA1XX_TXRFCTRL_PLLA_SH				0
#define DTA1XX_TXRFCTRL_PLLB_MSK			0x00003FE0
#define DTA1XX_TXRFCTRL_PLLB_SH				5
#define DTA1XX_TXRFCTRL_PLL_LOCK			0x00020000
#define DTA1XX_TXRFCTRL_PLLB9_MSK			0x00040000
#define DTA1XX_TXRFCTRL_PLLB9_SH			18
#define DTA1XX_TXRFCTRL_PLLR_MSK			0xFFF80000
#define DTA1XX_TXRFCTRL_PLLR_SH				19

// RF-Control register: Register access
static __inline UInt  Dta1xxTxGetRfCtrlReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_RF_CONTROL);
}
static __inline void  Dta1xxTxSetRfCtrlReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_RF_CONTROL);
}

// RF-Control register: RF PLL A-Counter Modulus
static __inline UInt  Dta1xxTxRfCtrlRegGetPllA(volatile UInt8* pBase) {
	UInt  RegVal = Dta1xxTxGetRfCtrlReg(pBase);
	return (RegVal & DTA1XX_TXRFCTRL_PLLA_MSK) >> DTA1XX_TXRFCTRL_PLLA_SH;
}
static __inline void  Dta1xxTxRfCtrlRegSetPllA(volatile UInt8* pBase, UInt PllA) {
	UInt  RegVal = Dta1xxTxGetRfCtrlReg(pBase) & ~DTA1XX_TXRFCTRL_PLLA_MSK;
	Dta1xxTxSetRfCtrlReg(pBase, RegVal | (PllA << DTA1XX_TXRFCTRL_PLLA_SH));
}

// RF-Control register: RF PLL B-Counter Modulus
static __inline UInt  Dta1xxTxRfCtrlRegGetPllB(volatile UInt8* pBase) {
	UInt  RegVal = Dta1xxTxGetRfCtrlReg(pBase);
	UInt  PllBbit8to0 = (RegVal & DTA1XX_TXRFCTRL_PLLB_MSK)  >> DTA1XX_TXRFCTRL_PLLB_SH;
	UInt  PllBbit9    = (RegVal & DTA1XX_TXRFCTRL_PLLB9_MSK) >> DTA1XX_TXRFCTRL_PLLB9_SH;
	return ((PllBbit9 << 9) + PllBbit8to0);
}
static __inline void  Dta1xxTxRfCtrlRegSetPllB(volatile UInt8* pBase, UInt PllB) {
	UInt  RegVal = Dta1xxTxGetRfCtrlReg(pBase) & ~DTA1XX_TXRFCTRL_PLLB_MSK
											   & ~DTA1XX_TXRFCTRL_PLLB9_MSK;
	UInt  PllBbit8to0 =  PllB & 0x3FF;
	UInt  PllBbit9    = (PllB >> 9) & 1;
	Dta1xxTxSetRfCtrlReg(pBase, RegVal | (PllBbit8to0 << DTA1XX_TXRFCTRL_PLLB_SH)
									   | (PllBbit9    << DTA1XX_TXRFCTRL_PLLB9_SH));
}

// RF-Control register: RF PLL Reference-Counter Modulus
static __inline UInt  Dta1xxTxRfCtrlRegGetPllR(volatile UInt8* pBase) {
	UInt  RegVal = Dta1xxTxGetRfCtrlReg(pBase);
	return (RegVal & DTA1XX_TXRFCTRL_PLLR_MSK) >> DTA1XX_TXRFCTRL_PLLR_SH;
}
static __inline void  Dta1xxTxRfCtrlRegSetPllR(volatile UInt8* pBase, UInt PllR) {
	UInt  RegVal = Dta1xxTxGetRfCtrlReg(pBase) & ~DTA1XX_TXRFCTRL_PLLR_MSK;
	Dta1xxTxSetRfCtrlReg(pBase, RegVal | (PllR << DTA1XX_TXRFCTRL_PLLR_SH));
}

// RF-Control register: PLL Locked Status
static __inline UInt  Dta1xxTxRfCtrlRegGetPllLock(volatile UInt8* pBase) {
	return ((Dta1xxTxGetRfCtrlReg(pBase)&DTA1XX_TXRFCTRL_PLL_LOCK)!=0) ? 1 : 0;
}

//-.-.-.-.-.-.-.-.-.-.-.- RF-Control-1 register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

// RF-Control register 1: Bit fields (for DTA-115)
#define DTA1XX_TXRFCTRL1_MOD_MSK			0x00000FFF
#define DTA1XX_TXRFCTRL1_MOD_SH				0
#define DTA1XX_TXRFCTRL1_FRAC_MSK			0x00FFF000
#define DTA1XX_TXRFCTRL1_FRAC_SH			12
#define DTA1XX_TXRFCTRL1_INT_MSK			0xFF000000
#define DTA1XX_TXRFCTRL1_INT_SH				24

// RF-Control-1 register: Register access
//
// NOTE: this register has the same location as the RF-control register in
// the DTA-107 and DTA-110
//
static __inline UInt  Dta1xxTxGetRfCtrl1Reg(volatile UInt8* pBase) {
	return Dta1xxTxGetRfCtrlReg(pBase);
}
static __inline void  Dta1xxTxSetRfCtrl1Reg(volatile UInt8* pBase, UInt Val) {
	Dta1xxTxSetRfCtrlReg(pBase, Val);
}

// RF-Control-1 register: RF-PLL2 Modulus value
static __inline UInt  Dta1xxTxRfCtrl1RegGetPllM(volatile UInt8* pBase) {
	UInt  RegVal = Dta1xxTxGetRfCtrl1Reg(pBase);
	return (RegVal & DTA1XX_TXRFCTRL1_MOD_MSK) >> DTA1XX_TXRFCTRL1_MOD_SH;
}
static __inline void  Dta1xxTxRfCtrl1RegSetPllM(volatile UInt8* pBase, UInt PllM) {
	UInt  RegVal = Dta1xxTxGetRfCtrl1Reg(pBase) & ~DTA1XX_TXRFCTRL1_MOD_MSK;
	Dta1xxTxSetRfCtrl1Reg(pBase, RegVal | (PllM << DTA1XX_TXRFCTRL1_MOD_SH));
}

// RF-Control-1 register: RF-PLL2 Fractional value
static __inline UInt  Dta1xxTxRfCtrl1RegGetPllF(volatile UInt8* pBase) {
	UInt  RegVal = Dta1xxTxGetRfCtrl1Reg(pBase);
	return (RegVal & DTA1XX_TXRFCTRL1_FRAC_MSK) >> DTA1XX_TXRFCTRL1_FRAC_SH;
}
static __inline void  Dta1xxTxRfCtrl1RegSetPllF(volatile UInt8* pBase, UInt PllF) {
	UInt  RegVal = Dta1xxTxGetRfCtrl1Reg(pBase) & ~DTA1XX_TXRFCTRL1_FRAC_MSK;
	Dta1xxTxSetRfCtrl1Reg(pBase, RegVal | (PllF << DTA1XX_TXRFCTRL1_FRAC_SH));
}

// RF-Control-1 register: RF-PLL2 Integer value
static __inline UInt  Dta1xxTxRfCtrl1RegGetPllI(volatile UInt8* pBase) {
	UInt  RegVal = Dta1xxTxGetRfCtrl1Reg(pBase);
	return (RegVal & DTA1XX_TXRFCTRL1_INT_MSK) >> DTA1XX_TXRFCTRL1_INT_SH;
}
static __inline void  Dta1xxTxRfCtrl1RegSetPllI(volatile UInt8* pBase, UInt PllI) {
	UInt  RegVal = Dta1xxTxGetRfCtrl1Reg(pBase) & ~DTA1XX_TXRFCTRL1_INT_MSK;
	Dta1xxTxSetRfCtrl1Reg(pBase, RegVal | (PllI << DTA1XX_TXRFCTRL1_INT_SH));
}

//.-.-.-.-.-.-.-.-.-.-.-.- IF-Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// IF-Control register: Bit fields
#define DTA1XX_TXIFCTRL_PLLR_MSK			0x00003FFF
#define DTA1XX_TXIFCTRL_PLLR_SH				0
#define DTA1XX_TXIFCTRL_PLLB_MSK			0x07FFC000
#define DTA1XX_TXIFCTRL_PLLB_SH				14
#define DTA1XX_TXIFCTRL_PLLA_MSK			0xF8000000
#define DTA1XX_TXIFCTRL_PLLA_SH				27

// IF-Control register: Register access
static __inline UInt  Dta1xxTxGetIfCtrlReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_IF_CONTROL);
}
static __inline void  Dta1xxTxSetIfCtrlReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_IF_CONTROL);
}

// IF-Control register: IF PLL A-Counter Modulus
static __inline UInt  Dta1xxTxIfCtrlRegGetPllA(volatile UInt8* pBase) {
	UInt  RegVal = Dta1xxTxGetIfCtrlReg(pBase);
	return (RegVal & DTA1XX_TXIFCTRL_PLLA_MSK) >> DTA1XX_TXIFCTRL_PLLA_SH;
}
static __inline void  Dta1xxTxIfCtrlRegSetPllA(volatile UInt8* pBase, UInt PllA) {
	UInt  RegVal = Dta1xxTxGetIfCtrlReg(pBase) & ~DTA1XX_TXIFCTRL_PLLA_MSK;
	Dta1xxTxSetIfCtrlReg(pBase, RegVal | (PllA << DTA1XX_TXIFCTRL_PLLA_SH));
}

// IF-Control register: IF PLL B-Counter Modulus
static __inline UInt  Dta1xxTxIfCtrlRegGetPllB(volatile UInt8* pBase) {
	UInt  RegVal = Dta1xxTxGetIfCtrlReg(pBase);
	return (RegVal & DTA1XX_TXIFCTRL_PLLB_MSK) >> DTA1XX_TXIFCTRL_PLLB_SH;
}
static __inline void  Dta1xxTxIfCtrlRegSetPllB(volatile UInt8* pBase, UInt PllB) {
	UInt  RegVal = Dta1xxTxGetIfCtrlReg(pBase) & ~DTA1XX_TXIFCTRL_PLLB_MSK;
	Dta1xxTxSetIfCtrlReg(pBase, RegVal | (PllB << DTA1XX_TXIFCTRL_PLLB_SH));
}

// IF-Control register: IF PLL Reference-Counter Modulus
static __inline UInt  Dta1xxTxIfCtrlRegGetPllR(volatile UInt8* pBase) {
	UInt  RegVal = Dta1xxTxGetIfCtrlReg(pBase);
	return (RegVal & DTA1XX_TXIFCTRL_PLLR_MSK) >> DTA1XX_TXIFCTRL_PLLR_SH;
}
static __inline void  Dta1xxTxIfCtrlRegSetPllR(volatile UInt8* pBase, UInt PllR) {
	UInt  RegVal = Dta1xxTxGetIfCtrlReg(pBase) & ~DTA1XX_TXIFCTRL_PLLR_MSK;
	Dta1xxTxSetIfCtrlReg(pBase, RegVal | (PllR << DTA1XX_TXIFCTRL_PLLR_SH));
}

//-.-.-.-.-.-.-.-.-.-.-.- RF-Control-2 register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.

// RF-Control register 2: Bit fields
#define DTA1XX_TXRFCTRL2_AGCSP_MSK		0x00000FFF
#define DTA1XX_TXRFCTRL2_AGCSP_SH		0
#define DTA1XX_TXRFCTRL2_OUTATTN_MSK	0x0003F000
#define DTA1XX_TXRFCTRL2_OUTATTN_SH		12
#define DTA1XX_TXRFCTRL2_OUTLVL10B_OFFSET_MSK	0x003FF000
#define DTA1XX_TXRFCTRL2_OUTLVL10B_OFFSET_SH	12
#define DTA1XX_TXRFCTRL2_AGC_EN			0x00040000
#define DTA1XX_TXRFCTRL2_INPATTN_MSK	0x00780000
#define DTA1XX_TXRFCTRL2_INPATTN_SH	19
#define DTA1XX_TXRFCTRL2_LOWNOISE_EN	0x00800000
#define DTA1XX_TXRFCTRL2_CPC_MSK		0x0F000000
#define DTA1XX_TXRFCTRL2_CPC_SH			24
#define DTA1XX_TXRFCTRL2_PLL1_LOCK		0x10000000
#define DTA1XX_TXRFCTRL2_PLL2_LOCK		0x20000000
#define DTA1XX_TXRFCTRL2_PLL3_LOCK		0x40000000
#define DTA1XX_TXRFCTRL2_AGC_LOCK		0x80000000


// RF-Control-2 register: Register access
static __inline UInt  Dta1xxTxGetRfCtrl2Reg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_RF_CONTROL2);
}
static __inline void  Dta1xxTxSetRfCtrl2Reg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_RF_CONTROL2);
}

// RF-Control-2 register: AGC setpoint
static __inline UInt  Dta1xxTxRfCtrl2RegGetAgcSetPoint(volatile UInt8* pBase) {
	UInt  RegVal = Dta1xxTxGetRfCtrl2Reg(pBase);
	return (RegVal & DTA1XX_TXRFCTRL2_AGCSP_MSK) >> DTA1XX_TXRFCTRL2_AGCSP_SH;
}

// RF-Control-2 register: RF Output Attenuator
static __inline UInt  Dta1xxTxRfCtrl2RegGetOutpAttn(volatile UInt8* pBase) {
	UInt  RegVal = Dta1xxTxGetRfCtrl2Reg(pBase);
	return (RegVal & DTA1XX_TXRFCTRL2_OUTATTN_MSK) >> DTA1XX_TXRFCTRL2_OUTATTN_SH;
}
static __inline void  Dta1xxTxRfCtrl2RegSetOutpAttn(volatile UInt8* pBase, UInt Attn) {
	UInt  RegVal = Dta1xxTxGetRfCtrl2Reg(pBase) & ~DTA1XX_TXRFCTRL2_OUTATTN_MSK;
	Dta1xxTxSetRfCtrl2Reg(pBase, RegVal | (Attn << DTA1XX_TXRFCTRL2_OUTATTN_SH));
}

// RF-Control 2 register: 10-bit output level 
static __inline Int32  Dta1xxTxRfCtrl2RegGetOutputLevel10bOffset(volatile UInt8* pBase) {
	Int32  Val = (Int32)(Dta1xxTxGetRfCtrl2Reg(pBase) & DTA1XX_TXRFCTRL2_OUTLVL10B_OFFSET_MSK);
	Val >>= DTA1XX_TXRFCTRL2_OUTLVL10B_OFFSET_SH;
	Val &= 0x000003FF;
	return Val;
}
static __inline void  Dta1xxTxRfCtrl2RegSetOutputLevel10bOffset(volatile UInt8* pBase, Int32 Offset) {
	UInt  Val = Dta1xxTxGetRfCtrl2Reg(pBase) & ~DTA1XX_TXRFCTRL2_OUTLVL10B_OFFSET_MSK;
	Offset <<= DTA1XX_TXRFCTRL2_OUTLVL10B_OFFSET_SH;
	Offset &= DTA1XX_TXRFCTRL2_OUTLVL10B_OFFSET_MSK;
	Dta1xxTxSetRfCtrl2Reg(pBase, Val | Offset);
}

// RF-Control-2 register: AGC Enable
static __inline UInt  Dta1xxTxRfCtrl2RegGetAgcEn(volatile UInt8* pBase) {
	return ((Dta1xxTxGetRfCtrl2Reg(pBase)&DTA1XX_TXRFCTRL2_AGC_EN)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxRfCtrl2RegSetAgcEn(volatile UInt8* pBase, UInt AgcEnable) {
	UInt  Val = Dta1xxTxGetRfCtrl2Reg(pBase);
	if (AgcEnable != 0)		Val |=  DTA1XX_TXRFCTRL2_AGC_EN;
	else					Val &= ~DTA1XX_TXRFCTRL2_AGC_EN;
	Dta1xxTxSetRfCtrl2Reg(pBase, Val);
}

// RF-Control-2 register: RF Input Attenuator
static __inline UInt  Dta1xxTxRfCtrl2RegGetInpAttn(volatile UInt8* pBase) {
	UInt  RegVal = Dta1xxTxGetRfCtrl2Reg(pBase);
	return (RegVal & DTA1XX_TXRFCTRL2_INPATTN_MSK) >> DTA1XX_TXRFCTRL2_INPATTN_SH;
}
static __inline void  Dta1xxTxRfCtrl2RegSetInpAttn(volatile UInt8* pBase, UInt Attn) {
	UInt  RegVal = Dta1xxTxGetRfCtrl2Reg(pBase) & ~DTA1XX_TXRFCTRL2_INPATTN_MSK;
	Dta1xxTxSetRfCtrl2Reg(pBase, RegVal | (Attn << DTA1XX_TXRFCTRL2_INPATTN_SH));
}

// RF-Control-2 register: Low Phase Noise Enable
static __inline UInt  Dta1xxTxRfCtrl2RegGetLowPhaseNoiseEn(volatile UInt8* pBase) {
	return ((Dta1xxTxGetRfCtrl2Reg(pBase)&DTA1XX_TXRFCTRL2_LOWNOISE_EN)!=0) ? 1 : 0;
}
static __inline void  Dta1xxTxRfCtrl2RegSetLowPhaseNoiseEn(volatile UInt8* pBase, UInt Enable) {
	UInt  Val = Dta1xxTxGetRfCtrl2Reg(pBase);
	if (Enable != 0)	Val |=  DTA1XX_TXRFCTRL2_LOWNOISE_EN;
	else				Val &= ~DTA1XX_TXRFCTRL2_LOWNOISE_EN;
	Dta1xxTxSetRfCtrl2Reg(pBase, Val);
}

// RF-Control-2 register: Charge-pump current
static __inline UInt  Dta1xxTxRfCtrl2RegGetChargePumpCur(volatile UInt8* pBase) {
	UInt  RegVal = Dta1xxTxGetRfCtrl2Reg(pBase);
	return (RegVal & DTA1XX_TXRFCTRL2_CPC_MSK) >> DTA1XX_TXRFCTRL2_CPC_SH;
}
static __inline void  Dta1xxTxRfCtrl2RegSetChargePumpCur(volatile UInt8* pBase, UInt Current) {
	UInt  RegVal = Dta1xxTxGetRfCtrl2Reg(pBase) & ~DTA1XX_TXRFCTRL2_CPC_MSK;
	Dta1xxTxSetRfCtrl2Reg(pBase, RegVal | (Current << DTA1XX_TXRFCTRL2_CPC_SH));
}

// RF-Control-2 register: PLL1 Locked Status
static __inline UInt  Dta1xxTxRfCtrl2RegGetPll1Lock(volatile UInt8* pBase) {
	return ((Dta1xxTxGetRfCtrl2Reg(pBase)&DTA1XX_TXRFCTRL2_PLL1_LOCK)!=0) ? 1 : 0;
}

// RF-Control-2 register: PLL2 Locked Status
static __inline UInt  Dta1xxTxRfCtrl2RegGetPll2Lock(volatile UInt8* pBase) {
	return ((Dta1xxTxGetRfCtrl2Reg(pBase)&DTA1XX_TXRFCTRL2_PLL2_LOCK)!=0) ? 1 : 0;
}

// RF-Control-2 register: PLL3 Locked Status
static __inline UInt  Dta1xxTxRfCtrl2RegGetPll3Lock(volatile UInt8* pBase) {
	return ((Dta1xxTxGetRfCtrl2Reg(pBase)&DTA1XX_TXRFCTRL2_PLL3_LOCK)!=0) ? 1 : 0;
}

// RF-Control-2 register: AGC Locked Status
static __inline UInt  Dta1xxTxRfCtrl2RegGetAgcLock(volatile UInt8* pBase) {
	return ((Dta1xxTxGetRfCtrl2Reg(pBase)&DTA1XX_TXRFCTRL2_AGC_LOCK)!=0) ? 1 : 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.- FIFO Data register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// FIFO Data: Register access
static __inline void  Dta1xxTxSetFifoDataReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_FIFO_FIRST);
}
static __inline void  Dta1xxTxSetFifoDataRegIdx(volatile UInt8* pBase, Int Idx, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_FIFO_FIRST + Idx*sizeof(UInt32));
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.- TEMP FOR DTA-2142 EXTERNAL CLOCK -.-.-.-.-.-.-.-.-.-.-.-.-.-

#define DTA1XX_TMP_TXCONTROL2		0x0190

// SPI Control: Register access
static __inline UInt  Dta1xxTxGetTxControl2Reg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TMP_TXCONTROL2);
}
static __inline void  Dta1xxTxSetTxControl2Reg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TMP_TXCONTROL2);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-		DTA-2148				-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.- GS2962 Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  Dta1xxTxGetGS2962CtrlReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_GS2962CTRL);
}
static __inline void Dta1xxTxSetGS2962CtrlReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_GS2962CTRL);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- RFDAC SPI control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// RFDAC-Control register: Bit fields
#define DTA1XX_TXRFDACCTRL_SPIREADY_MSK		0x00000001
#define DTA1XX_TXRFDACCTRL_SPISTART_MSK		0x00000002
#define DTA1XX_TXRFDACCTRL_SPINUMBYTES_MSK	0x0000000C
#define DTA1XX_TXRFDACCTRL_SPINUMBYTES_SH	2
#define DTA1XX_TXRFDACCTRL_SPIRDORWRN		0x00000010
#define DTA1XX_TXRFDACCTRL_SPIADDR_MASK		0xFFFF0000
#define DTA1XX_TXRFDACCTRL_SPIADDR_SH		16
typedef struct {
	unsigned SpiReady		: 1;
	unsigned SpiStart		: 1;
	unsigned SpiNumBytes	: 2;
	unsigned SpiRdOrWrn		: 1;
	unsigned SpiReserved	: 11;
	unsigned SpiAddr		: 16;
} tDta1xx_TxRfDacCtrl;

static __inline UInt  Dta1xxTxGetRfdacCtrlReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_RFDAC_CONTROL);
}
static __inline void Dta1xxTxSetRfdacCtrlReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_RFDAC_CONTROL);
}
static __inline int  Dta1xxTxGetRfdacCtrlRegSpiReady(volatile UInt8* pBase) {
	return Dta1xxTxGetRfdacCtrlReg(pBase)&DTA1XX_TXRFDACCTRL_SPIREADY_MSK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- RFDAC SPI data register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  Dta1xxTxGetRfdacDataReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_RFDAC_DATA);
}
static __inline void Dta1xxTxSetRfdacDataReg(volatile UInt8* pBase, UInt32 Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_RFDAC_DATA);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- GS2962 Stat register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  Dta1xxTxGetGS2962StatReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_GS2962STAT);
}
static __inline void Dta1xxTxSetGS2962StatReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_GS2962STAT);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- GS2962 SPI register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  Dta1xxTxGetGS2962SPIReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_TX_REG_GS2962SPI);
}
static __inline void Dta1xxTxSetGS2962SPIReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_TX_REG_GS2962SPI);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Dta1xxRx +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Register block for a RECEIVE channel.
//
// Supports: DTA-120, DTA-122, DTA-140
//
typedef struct _Dta1xxRx {

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- General-Control register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// On DTA-140, these fields are moved to the generic register block.
	//
	struct {
	volatile UInt  m_PE				: 1;	// Serial EEPROM Program Enable
	volatile UInt  m_PRE			: 1;	// Serial EEPROM Protect Register Enable
	volatile UInt  m_Reset			: 1;	// Soft reset of DTA 100 circuitry
			 UInt  m_Reserved1		: 5;	//
			 UInt  m_FirmwRev		: 8;	// Firmware Revision
			 UInt  m_TypeNum		: 8;	// Board Type Number
	}  m_GenControl;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Receive-Control register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	struct {
	volatile UInt  m_RxMode			: 2;	//  Receive Mode: St188/St204/StMp2/StRaw
	volatile UInt  m_AsiInv			: 2;	//  Invert ASI-input control (ASI cards)
	volatile UInt  m_Equalise		: 1;	//  Equaliser control (ASI cards)
	volatile UInt  m_RxCtrl			: 1;	//  Receive Control: Idle/Rcv
	volatile UInt  m_RxAntPwr		: 1;	//  Antenna Power Control: Off/On
	volatile UInt  m_RxTimeStamp	: 1;	//  Insert Time Stamps
	volatile UInt  m_PerIntEn		: 1;	//  Periodic-Interrupt Enable
	volatile UInt  m_OvfIntEn		: 1;	//  Input-FIFO Overflow Interrupt Enable
	volatile UInt  m_SyncIntEn		: 1;	//  Synchronisation-Error Interrupt Enable
			 UInt  m_Reserved3		: 1;	//
	volatile UInt  m_ThrIntEn		: 1;	//  Threshold-Crossed Interrupt Enable
	volatile UInt  m_RateOvfIntEn	: 1;	//  Input-Rate Overflow Interrupt Enable
			 UInt  m_Reserved4		: 3;	//
	volatile UInt  m_EnaPwr			: 1;	//  Enable Power to target adapter
			 UInt  m_Reserved5		: 1;	//
	volatile UInt  m_LedControl		: 1;	//  Take over LED Control
	volatile UInt  m_LedGreen		: 1;	//  State of Green LED (if LedControl is set)
	volatile UInt  m_LedRed			: 1;	//  State of Red LED (if LedControl is set)
	volatile UInt  m_RxClrFifo		: 1;	//  Clear Receive FIFO (write only)
	volatile UInt  m_RxRst			: 1;	//  Reset Receive Channel (write only)
	volatile UInt  m_SdiMode		: 1;	//  SDI mode enable/disbale
	volatile UInt  m_Sdi10Bit		: 1;	//  SDI 10-bit mode
	volatile UInt  m_SdiHuffEn		: 1;	//  SDI Huffman compression enable
	}  m_RxControl;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Receive-Status register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	// Status register is a union.
	// WATCH OUT: DO NOT clear interrupts by something like p->Fields.m_PerInt = 1;
	// THIS WOULD CLEAR ALL STATUS FLAGS!!
	union {
	struct {
	volatile UInt  m_PckSize		: 2;	//  Packet size: Rx188/Rx204/RxInv
	volatile UInt  m_NumInv			: 2;	//  Number of invalid bytes per packet
	volatile UInt  m_SdramSize		: 4;	//  SDRAM Size
	volatile UInt  m_PerInt			: 1;	//  Periodic Interrupt
	volatile UInt  m_OvfInt			: 1;	//  Input-FIFO Overflow Interrupt
	volatile UInt  m_SyncInt		: 1;	//  Synchronisation-Error Interrupt
			 UInt  m_Reserved1		: 1;	//
	volatile UInt  m_ThrInt			: 1;	//  Threshold-Crossed Interrupt
	volatile UInt  m_RateOvfInt		: 1;	//  Input-Rate Overflow Interrupt
	volatile UInt  m_ClkDet			: 1;	//  Receive-Clock Detected (DTA-120: Asi CD)
	volatile UInt  m_AsiLock		: 1;	//  DVB/ASI PLL locked to input signal
	volatile UInt  m_RateOk			: 1;	//  Input rate above minimum input rate
	volatile UInt  m_AsiInv			: 1;	//  DVB/ASI-Input Invert - Status
	}				 Fields;				// For access to individual fields in RxStatus

			 UInt32  All;					// For access to RxStatus in one go
	} m_RxStatus;

	volatile UInt32  m_Reserved[3];			// Not used

	volatile UInt32  m_FifoLoad;			// FIFO-Load register

	struct {								// Diagnostics register
			 UInt  m_Reserved1		: 8;	//
	volatile UInt  m_LoopBack		: 1;	//	Loop-back mode
	volatile UInt  m_CdcRateOvf		: 1;	//	Clock-Domain-Converter Rate Overflow
	volatile UInt  m_RfRateOvf		: 1;	//	Receive-FIFO Rate Overflow
	volatile UInt  m_SdFull			: 1;	//	SDRAM-Full Flag
			 UInt  m_Reserved2		: 7;	//

	// ONLY FOR: DTA-120, Firmware Revision < 4 (Later revs: Programming register is used)
	volatile UInt  m_TDO			: 1;	// Status of TDO pin
	volatile UInt  m_TMS			: 1;	// Control of TMS pin
	volatile UInt  m_TCK			: 1;	// Control of TCK pin
	volatile UInt  m_TDI			: 1;	// Control of TDI pin
	volatile UInt  m_PrgEpc			: 1;	// Enable access to programming i/f of EPC2

	}  m_Diag;

	volatile UInt32  m_LbData;				// Data written to Input FIFO (8 sign. bits)

	struct {								// Threshold-Control register
	volatile UInt  m_UpdBusy		: 1;	//  Threshold-Update process busy
	volatile UInt  m_CmpB			: 1;	//  Output of comparator B
	volatile UInt  m_CmpA			: 1;	//  Output of comparator A
	volatile UInt  m_AorB			: 1;	//  Update threshold A or threshold B
			 UInt  m_Reserved1		: 4;	//
	volatile UInt  m_ThreshA		: 8;	//  Threshold A
	volatile UInt  m_ThreshB		: 8;	//  Threshold B
	}  m_ThreshControl;

	volatile UInt32  m_PckCnt;				// Packet Count (DTA-122)
	volatile UInt32  m_ClkCnt;				// Clock Count (DTA-122)
	volatile UInt32  m_ValidCnt;			// Valid Count (DTA-120, DTA-122)
	volatile UInt32  m_CodeViolCnt;			// Code-Violation Count (DTA-120)

	volatile UInt32  m_Reserved2[1];		// Not used

	volatile UInt32  m_LockFilter;			// Control value for Lock Filter (DTA-120)

	volatile UInt32  m_FifoData[16];		// Transport-stream data: 4 bytes at a time
} Dta1xxRx;

// Rx Registers: BYTE offset
#define DTA1XX_RX_REG_GENCTRL			0x0000
#define DTA1XX_RX_REG_RXCTRL			0x0004
#define DTA1XX_RX_REG_RXSTATUS			0x0008
#define DTA1XX_RX_REG_SDIPOS			0x000C
#define DTA1XX_RX_REG_FIFOLOAD			0x0018
#define DTA1XX_RX_REG_DIAG				0x001C
#define DTA1XX_RX_REG_LOOPBKDATA		0x0020
#define DTA1XX_RX_REG_THRESHCTRL		0x0024
#define DTA1XX_RX_REG_PCKCOUNT			0x0028
#define DTA1XX_RX_REG_CLKCOUNT			0x002C
#define DTA1XX_RX_REG_VALIDCOUNT		0x0030
#define DTA1XX_RX_REG_VIOLCOUNT			0x0034
#define DTA1XX_RX_REG_FIFO_FIRST		0x0040
#define DTA1XX_RX_REG_FIFO_LAST			0x007C
#define DTA1XX_RX_REG_DMA_DESC_PTR		0x0044
#define DTA1XX_RX_REG_DMA_COMMAND_STAT	0x0048
#define DTA1XX_RX_REG_DMA_DAC			0x004C
#define DTA1XX_RX_REG_DMA_PCI_ADDR		0x0050
#define DTA1XX_RX_REG_DMA_LOCAL_ADDR	0x0054
#define DTA1XX_RX_REG_DMA_COUNT			0x0058
#define DTA1XX_RX_REG_DMA_NXTDESC_PTR	0x005C
#define DTA1XX_RX_REG_DMA_PCI_ADDRH		0x0060

//-.-.-.-.-.-.-.-.-.-.-.-.-.-		DTA-2148				-.-.-.-.-.-.-.-.-.-.-.-.-.-
// GS2984 Equaliser specific
#define DTA1XX_RX_REG_GS2984CTRL		0x0074
#define DTA1XX_RX_REG_GS2984STAT		0x0078

// GS2960 Receiver specific
#define DTA1XX_RX_REG_GS2960SPI			0x0060
#define DTA1XX_RX_REG_GS2960CTRL		0x0064
#define DTA1XX_RX_REG_GS2960STAT		0x0068



//.-.-.-.-.-.-.-.-.-.-.-.- Rx Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// Rx Control: Bit fields
#define DTA1XX_RXCTRL_RXMODE			0x00000003
#define DTA1XX_RXCTRL_ASIINV			0x0000000C
#define DTA1XX_RXCTRL_RXMODE_EXT		0x00000010
#define DTA1XX_RXCTRL_EQUALISE			0x00000010
#define DTA1XX_RXCTRL_RXCTRL			0x00000020
#define DTA1XX_RXCTRL_ANTPWR			0X00000040
#define DTA1XX_RXCTRL_TIMESTAMP			0x00000080 // OBSOLETE
#define DTA1XX_RXCTRL_TIMESTAMP32		0x00000080
#define DTA1XX_RXCTRL_TIMESTAMP64		0x00000100
#define DTA1XX_RXCTRL_PERINT_EN			0x00000100
#define DTA1XX_RXCTRL_OVFINT_EN			0x00000200
#define DTA1XX_RXCTRL_SYNCINT_EN		0x00000400
#define DTA1XX_RXCTRL_THRINT_EN			0x00001000
#define DTA1XX_RXCTRL_RATEOVFINT_EN		0x00002000
#define DTA1XX_RXCTRL_FRAMEINT_EN		0x00002000
#define DTA1XX_RXCTRL_ENAPWR			0x00020000
#define DTA1XX_RXCTRL_LEDCTRL			0x00080000
#define DTA1XX_RXCTRL_GREENLED			0x00100000
#define DTA1XX_RXCTRL_REDLED			0x00200000
#define DTA1XX_RXCTRL_CLEARFIFO			0x00400000
#define DTA1XX_RXCTRL_RXRESET			0x00800000
#define DTA1XX_RXCTRL_SDI_MODE			0x01000000
#define DTA1XX_RXCTRL_SDI_10BIT			0x02000000
#define DTA1XX_RXCTRL_SDI_COMPR_EN		0x04000000
#define DTA1XX_RXCTRL_PIDFILT_EN		0x08000000


// Rx Control: Register access
static __inline UInt  Dta1xxRxGetRxCtrlReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_RX_REG_RXCTRL);
}
static __inline void Dta1xxRxSetRxCtrlReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_RX_REG_RXCTRL);
}

// Rx Control: RxMode
static __inline UInt  Dta1xxRxCtrlRegGetRxMode(volatile UInt8* pBase) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	return (Val & DTA1XX_RXCTRL_RXMODE) |
		   ((Val & DTA1XX_RXCTRL_RXMODE_EXT) >> 2);
}
static __inline void  Dta1xxRxCtrlRegSetRxMode(volatile UInt8* pBase, UInt RxMode) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	Val &= ~(DTA1XX_RXCTRL_RXMODE | DTA1XX_RXCTRL_RXMODE_EXT);
	Val |=  RxMode & DTA1XX_RXCTRL_RXMODE;
	if (RxMode & 0x4) Val |= DTA1XX_RXCTRL_RXMODE_EXT;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: AsiInv
static __inline UInt  Dta1xxRxCtrlRegGetAsiInv(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_ASIINV)>>2);
}
static __inline void  Dta1xxRxCtrlRegSetAsiInv(volatile UInt8* pBase, UInt AsiInv) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	Val &= ~DTA1XX_RXCTRL_ASIINV;
	Val |=  AsiInv << 2;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: Equalise
static __inline UInt  Dta1xxRxCtrlRegGetEqualise(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_EQUALISE)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxCtrlRegSetEqualise(volatile UInt8* pBase, UInt Equalise) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	if (Equalise != 0)	Val |=  DTA1XX_RXCTRL_EQUALISE;
	else				Val &= ~DTA1XX_RXCTRL_EQUALISE;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: RxCtrl
static __inline UInt  Dta1xxRxCtrlRegGetRxCtrl(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_RXCTRL)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxCtrlRegSetRxCtrl(volatile UInt8* pBase, UInt RxCtrl) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	if (RxCtrl != 0)	Val |=  DTA1XX_RXCTRL_RXCTRL;
	else				Val &= ~DTA1XX_RXCTRL_RXCTRL;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: AntPwr (e.g. DTA-2135)
static __inline UInt  Dta1xxRxCtrlRegGetAntPwr(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_ANTPWR)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxCtrlRegSetAntPwr(volatile UInt8* pBase, UInt EnaPwr) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	if (EnaPwr != 0)	Val |=  DTA1XX_RXCTRL_ANTPWR;
	else				Val &= ~DTA1XX_RXCTRL_ANTPWR;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: RxTimeStamp
#define Dta1xxRxCtrlRegGetTimeStamping Dta1xxRxCtrlRegGetTimeStamping32 // OBSOLETE
#define Dta1xxRxCtrlRegSetTimeStamping Dta1xxRxCtrlRegSetTimeStamping32 // OBSOLETE
static __inline UInt  Dta1xxRxCtrlRegGetTimeStamping32(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_TIMESTAMP32)!=0) ? 1 : 0;
}
static __inline UInt  Dta1xxRxCtrlRegGetTimeStamping64(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_TIMESTAMP64)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxCtrlRegSetTimeStamping32(volatile UInt8* pBase, UInt RxTimeStamp) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	if (RxTimeStamp != 0)	Val |=  DTA1XX_RXCTRL_TIMESTAMP32;
	else					Val &= ~DTA1XX_RXCTRL_TIMESTAMP32;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}
static __inline void  Dta1xxRxCtrlRegSetTimeStamping64(volatile UInt8* pBase, UInt RxTimeStamp) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	if (RxTimeStamp != 0)	Val |=  DTA1XX_RXCTRL_TIMESTAMP64;
	else					Val &= ~DTA1XX_RXCTRL_TIMESTAMP64;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: PerIntEn
static __inline UInt  Dta1xxRxCtrlRegGetRxPerIntEn(volatile UInt8* pBase) {
        return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_PERINT_EN)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxCtrlRegSetRxPerIntEn(volatile UInt8* pBase, UInt RxPerIntEn) {
        UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
        if (RxPerIntEn != 0)	Val |=  DTA1XX_RXCTRL_PERINT_EN;
        else					Val &= ~DTA1XX_RXCTRL_PERINT_EN;
        Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: OvfIntEn
static __inline UInt  Dta1xxRxCtrlRegGetOvfIntEn(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_OVFINT_EN)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxCtrlRegSetOvfIntEn(volatile UInt8* pBase, UInt OvfIntEn) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	if (OvfIntEn != 0)	Val |=  DTA1XX_RXCTRL_OVFINT_EN;
	else				Val &= ~DTA1XX_RXCTRL_OVFINT_EN;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: SyncIntEn
static __inline UInt  Dta1xxRxCtrlRegGetSyncIntEn(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_SYNCINT_EN)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxCtrlRegSetSyncIntEn(volatile UInt8* pBase, UInt SyncIntEn) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	if (SyncIntEn != 0)	Val |=  DTA1XX_RXCTRL_SYNCINT_EN;
	else				Val &= ~DTA1XX_RXCTRL_SYNCINT_EN;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: EnaPwr (e.g. DTA-122)
static __inline UInt  Dta1xxRxCtrlRegGetEnaPwr(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_ENAPWR)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxCtrlRegSetEnaPwr(volatile UInt8* pBase, UInt EnaPwr) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	if (EnaPwr != 0)	Val |=  DTA1XX_RXCTRL_ENAPWR;
	else				Val &= ~DTA1XX_RXCTRL_ENAPWR;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: LedControl
static __inline UInt  Dta1xxRxCtrlRegGetLedControl(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_LEDCTRL)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxCtrlRegSetLedControl(volatile UInt8* pBase, UInt LedControl) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	if (LedControl != 0)	Val |=  DTA1XX_RXCTRL_LEDCTRL;
	else					Val &= ~DTA1XX_RXCTRL_LEDCTRL;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: LedGreen
static __inline UInt  Dta1xxRxCtrlRegGetLedGreen(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_GREENLED)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxCtrlRegSetLedGreen(volatile UInt8* pBase, UInt LedGreen) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	if (LedGreen != 0)	Val |=  DTA1XX_RXCTRL_GREENLED;
	else				Val &= ~DTA1XX_RXCTRL_GREENLED;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: LedRed
static __inline UInt  Dta1xxRxCtrlRegGetLedRed(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_REDLED)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxCtrlRegSetLedRed(volatile UInt8* pBase, UInt LedRed) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	if (LedRed != 0)	Val |=  DTA1XX_RXCTRL_REDLED;
	else				Val &= ~DTA1XX_RXCTRL_REDLED;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: Clear-FIFO
static __inline void  Dta1xxRxCtrlRegClrFifo(volatile UInt8* pBase) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	Val |=  DTA1XX_RXCTRL_CLEARFIFO;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: Reset
static __inline void  Dta1xxRxCtrlRegRxReset(volatile UInt8* pBase) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	Val |=  DTA1XX_RXCTRL_RXRESET;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: SDI mode
static __inline UInt  Dta1xxRxCtrlRegGetSdiMode(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_SDI_MODE)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxCtrlRegSetSdiMode(volatile UInt8* pBase, UInt SdiMode) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	if (SdiMode != 0)		Val |=  DTA1XX_RXCTRL_SDI_MODE;
	else					Val &= ~DTA1XX_RXCTRL_SDI_MODE;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: SDI 10-Bit mode
static __inline UInt Dta1xxRxCtrlRegGetSdiMode10Bit(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_SDI_10BIT)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxCtrlRegSetSdiMode10Bit(volatile UInt8* pBase, UInt SdiMode10Bit) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	if (SdiMode10Bit != 0)	Val |=  DTA1XX_RXCTRL_SDI_10BIT;
	else					Val &= ~DTA1XX_RXCTRL_SDI_10BIT;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}

// Rx Control: SDI compression mode enable
static __inline UInt Dta1xxRxCtrlRegGetSdiComprModeEn(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxCtrlReg(pBase)&DTA1XX_RXCTRL_SDI_COMPR_EN)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxCtrlRegSetSdiComprModeEn(volatile UInt8* pBase, UInt SdiComprModeEn) {
	UInt  Val = Dta1xxRxGetRxCtrlReg(pBase);
	if (SdiComprModeEn != 0)	Val |=  DTA1XX_RXCTRL_SDI_COMPR_EN;
	else						Val &= ~DTA1XX_RXCTRL_SDI_COMPR_EN;
	Dta1xxRxSetRxCtrlReg(pBase, Val);
}


//-.-.-.-.-.-.-.-.-.-.-.-.- Rx Status register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// Rx Status: Bit fields
#define DTA1XX_RXSTAT_PCKSIZE		0x00000003		// In SDI mode returns SDI standard
#define DTA1XX_RXSTAT_NUMINV		0x0000000C
#define DTA1XX_RXSTAT_SDRAMSIZE		0x000000F0
#define DTA1XX_RXSTAT_PERINT		0x00000100
#define DTA1XX_RXSTAT_OVFINT		0x00000200
#define DTA1XX_RXSTAT_SYNCINT		0x00000400
#define DTA1XX_RXSTAT_THRINT		0x00001000
#define DTA1XX_RXSTAT_RATEOVFINT	0x00002000
#define DTA1XX_RXSTAT_CLKDET		0x00004000
#define DTA1XX_RXSTAT_ASILOCK		0x00008000
#define DTA1XX_RXSTAT_RATEOK		0x00010000
#define DTA1XX_RXSTAT_ASIINV		0x00020000

// Rx Status: Register access
static __inline UInt  Dta1xxRxGetRxStatusReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_RX_REG_RXSTATUS);
}
static __inline void  Dta1xxRxSetRxStatusReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_RX_REG_RXSTATUS);
}

// Rx Status: PckSize
static __inline UInt  Dta1xxRxStatusRegGetPckSize(volatile UInt8* pBase) {
	return (Dta1xxRxGetRxStatusReg(pBase) & DTA1XX_RXSTAT_PCKSIZE);
}

// Rx Status: NumInv
static __inline UInt  Dta1xxRxStatusRegGetNumInv(volatile UInt8* pBase) {
	return (Dta1xxRxGetRxStatusReg(pBase) & DTA1XX_RXSTAT_NUMINV) >> 2;
}

// Rx Status: SdramSize
static __inline UInt  Dta1xxRxStatusRegGetSdramSize(volatile UInt8* pBase) {
	return (Dta1xxRxGetRxStatusReg(pBase) & DTA1XX_RXSTAT_SDRAMSIZE) >> 4;
}

// Rx Status: periodic interrupt
static __inline UInt  Dta1xxRxStatusRegGetPerInt(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxStatusReg(pBase)&DTA1XX_RXSTAT_PERINT)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxStatusRegClrPerInt(volatile UInt8* pBase) {
	Dta1xxRxSetRxStatusReg(pBase, DTA1XX_RXSTAT_PERINT);
}

// Rx Status: overflow interrupt
static __inline UInt  Dta1xxRxStatusRegGetOvfInt(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxStatusReg(pBase)&DTA1XX_RXSTAT_OVFINT)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxStatusRegClrOvfInt(volatile UInt8* pBase) {
	Dta1xxRxSetRxStatusReg(pBase, DTA1XX_RXSTAT_OVFINT);
}

// Rx Status: sync interrupt
static __inline UInt  Dta1xxRxStatusRegGetSyncInt(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxStatusReg(pBase)&DTA1XX_RXSTAT_SYNCINT)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxStatusRegClrSyncInt(volatile UInt8* pBase) {
	Dta1xxRxSetRxStatusReg(pBase, DTA1XX_RXSTAT_SYNCINT);
}

// Rx Status: threshold interrupt
static __inline UInt  Dta1xxRxStatusRegGetThresholdInt(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxStatusReg(pBase)&DTA1XX_RXSTAT_THRINT)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxStatusRegClrThresholdInt(volatile UInt8* pBase) {
	Dta1xxRxSetRxStatusReg(pBase, DTA1XX_RXSTAT_THRINT);
}

// Rx Status: rate overflow interrupt
static __inline UInt  Dta1xxRxStatusRegGetRateOvfInt(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxStatusReg(pBase)&DTA1XX_RXSTAT_RATEOVFINT)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxStatusRegClrRateOvfInt(volatile UInt8* pBase) {
	Dta1xxRxSetRxStatusReg(pBase, DTA1XX_RXSTAT_RATEOVFINT);
}

// Rx Status: ClkDet
static __inline UInt  Dta1xxRxStatusRegGetClkDet(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxStatusReg(pBase)&DTA1XX_RXSTAT_CLKDET)!=0) ? 1 : 0;
}

// Rx Status: AsiLock
static __inline UInt  Dta1xxRxStatusRegGetAsiLock(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxStatusReg(pBase)&DTA1XX_RXSTAT_ASILOCK)!=0) ? 1 : 0;
}

// Rx Status: RateOk
static __inline UInt  Dta1xxRxStatusRegGetRateOk(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxStatusReg(pBase)&DTA1XX_RXSTAT_RATEOK)!=0) ? 1 : 0;
}

// Rx Status: AsiInv
static __inline UInt  Dta1xxRxStatusRegGetAsiInv(volatile UInt8* pBase) {
	return ((Dta1xxRxGetRxStatusReg(pBase)&DTA1XX_RXSTAT_ASIINV)!=0) ? 1 : 0;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.- Rx SDI Position - Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.

#define DTA1XX_RXSDIPOS_LINE_SHIFT	11
#define DTA1XX_RXSDIPOS_FRAME_SHIFT	21

#define DTA1XX_RXSDIPOS_SAMP_MASK	0x7FF
#define DTA1XX_RXSDIPOS_LINE_MASK	0x3FF << DTA1XX_RXSDIPOS_LINE_SHIFT
#define DTA1XX_RXSDIPOS_FRAME_MASK	0x7FF << DTA1XX_RXSDIPOS_FRAME_SHIFT


// Get Receive SDI Position
// Return value: framecount; Line and Sample count are returned through pointer argument
static __inline UInt  Dta1xxRxGetSdiPos(volatile UInt8* pBase, UInt* pLineCnt, UInt* pSampCnt) {
	UInt  RegVal = READ_UINT(pBase, DTA1XX_RX_REG_SDIPOS);
	*pSampCnt = RegVal & DTA1XX_RXSDIPOS_SAMP_MASK;
	*pLineCnt = (RegVal & DTA1XX_RXSDIPOS_LINE_MASK) >> DTA1XX_RXSDIPOS_LINE_SHIFT;
	return (RegVal & DTA1XX_RXSDIPOS_FRAME_MASK) >> DTA1XX_RXSDIPOS_FRAME_SHIFT;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.- Rx FIFO Load - Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

// FIFO Load: Register access
static __inline UInt  Dta1xxRxGetFifoLoadReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_RX_REG_FIFOLOAD);
}


//.-.-.-.-.-.-.-.-.-.-.- Rx Diagnostics register - Access Functions -.-.-.-.-.-.-.-.-.-.-.

// Rx Diagnostics: Bit fields
#define DTA1XX_RXDIAG_LOOPBACK		0x00000100
#define DTA1XX_RXDIAG_RFRATEOVF		0x00000400
#define DTA1XX_RXDIAG_SDFULL		0x00000800

// Rx Diagnostics: Register access
static __inline UInt  Dta1xxRxGetDiagReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_RX_REG_DIAG);
}
static __inline void Dta1xxRxSetDiagReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_RX_REG_DIAG);
}

// Rx Diagnostics: LoopBack
static __inline UInt  Dta1xxRxDiagRegGetLoopBack(volatile UInt8* pBase) {
	return ((Dta1xxRxGetDiagReg(pBase)&DTA1XX_RXDIAG_LOOPBACK)!=0) ? 1 : 0;
}
static __inline void  Dta1xxRxDiagRegSetLoopBack(volatile UInt8* pBase, UInt LoopBack) {
	UInt  Val = Dta1xxRxGetDiagReg(pBase);
	if (LoopBack != 0)	Val |=  DTA1XX_RXDIAG_LOOPBACK;
	else				Val &= ~DTA1XX_RXDIAG_LOOPBACK;
	Dta1xxRxSetDiagReg(pBase, Val);
}

//.-.-.-.-.-.-.-.-.-.-.- Loop-Back Data register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-

// Loop-Back Data: Register access
static __inline void Dta1xxRxSetLoopBackData(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_RX_REG_LOOPBKDATA);
}

//.-.-.-.-.-.-.-.-.-.-.- Valid Count register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-

static __inline UInt  Dta1xxRxGetValidCountReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_RX_REG_VALIDCOUNT);
}

//.-.-.-.-.-.-.-.-.-.-.- Violation Count register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-

static __inline UInt  Dta1xxRxGetViolCountReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_RX_REG_VIOLCOUNT);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- FIFO Data register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-

// FIFO Data: Register access
static __inline UInt  Dta1xxRxGetFifoDataReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_RX_REG_FIFO_FIRST);
}
static __inline UInt  Dta1xxRxGetFifoDataRegIdx(volatile UInt8* pBase, Int Idx) {
	return READ_UINT(pBase, DTA1XX_RX_REG_FIFO_FIRST + Idx*sizeof(UInt32));
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-		DTA-2148				-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.- GS2984 Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  Dta1xxRxGetGS2984CtrlReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_RX_REG_GS2984CTRL);
}
static __inline void Dta1xxRxSetGS2984CtrlReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_RX_REG_GS2984CTRL);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- GS2984 Status register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  Dta1xxRxGetGS2984StatReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_RX_REG_GS2984STAT);
}
static __inline void Dta1xxRxSetGS2984StatReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_RX_REG_GS2984STAT);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- GS2960 Control register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  Dta1xxRxGetGS2960CtrlReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_RX_REG_GS2960CTRL);
}
static __inline void Dta1xxRxSetGS2960CtrlReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_RX_REG_GS2960CTRL);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- GS2960 Stat register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  Dta1xxRxGetGS2960StatReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_RX_REG_GS2960STAT);
}
static __inline void Dta1xxRxSetGS2960StatReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_RX_REG_GS2960STAT);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- GS2960 SPI register: Access Functions -.-.-.-.-.-.-.-.-.-.-.-.-
static __inline UInt  Dta1xxRxGetGS2960SPIReg(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_RX_REG_GS2960SPI);
}
static __inline void Dta1xxRxSetGS2960SPIReg(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_RX_REG_GS2960SPI);
}

static __inline UInt Dta1xxRxReadGS2960InternalReg(volatile UInt8* pBase, 
												   UInt Reg) {

	UInt Cmd;
	UInt Result;

	Cmd = (1<<31) + (Reg << 16);		// programm read command
		// Bit 31: R/W bit (Read = 1 , Write = 0)
		// Bit 30: Reserved
		// Bit 29: Reserved
		// Bit 28: AutoInc
		// Bit 27 downto 16: Register address
		// Bit 15 downto 0: Data (N/A for reads)
	
	Dta1xxRxSetGS2960SPIReg(pBase, Cmd);
	Result = (1<<31);
	while ((Result & (1<<31)) != 0)
		Result = Dta1xxRxGetGS2960SPIReg(pBase);

	return(Result);
}

static __inline void Dta1xxRxWriteGS2960InternalReg(volatile UInt8* pBase, 
													UInt Reg, UInt Val) {
	UInt Cmd;
	UInt Result;
	
	Cmd = (Reg << 16) + (Val & 0xFFFF);		// programm write command
		// Bit 31: R/W bit (Read = 1 , Write = 0)
		// Bit 30: Reserved
		// Bit 29: Reserved
		// Bit 28: AutoInc
		// Bit 27 downto 16: Register address
		// Bit 15 downto 0: Data (N/A for reads)
	
	Dta1xxRxSetGS2960SPIReg(pBase, Cmd);
	Result = (1<<31);
	while ((Result & (1<<31)) != 0)
		Result = Dta1xxRxGetGS2960SPIReg(pBase);

	return;
}

//.-.-.-.-.-.-.-.-.-.- DMA Command/Status register: Access Functions -.-.-.-.-.-.-.-.-.-.-

// Bit fields
// NOTE: other fields in this register follow PCI905X definitions:
//			PCI905X_DMACSR_ENABLE, PCI905X_DMACSR_START, PCI905X_DMACSR_ABORT,
//			PCI905X_DMACSR_CLEARINT, PCI905X_DMACSR_DONE
#define DTA1XX_DMACSR_INTACT		0x00000008		// DMA Channel Interrupt Active


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Dta140Regs +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// DTA-140 registers
// Same lay-out applies to DTA-120 Firware Revision >= 4, but without Tx registers
//
typedef struct _Dta140Regs {
	Dta1xxGen  m_Gen;						// General
	Dta1xxTx  m_Tx;							// DVB/ASI transmit channel
	Dta1xxRx  m_Rx;							// DVB/ASI receive channel
} Dta140Regs;


//.-.-.-.-.-.-.-.-.-.-.-.-.- Fields in General-Status Register -.-.-.-.-.-.-.-.-.-.-.-.-.-
// Defined this way for efficient access to status flags (don't need multiple PCI
// cycles if we want to access multiple flags.)
#define  DTA1XX_GENSTATUS_PERINT		0x10

//.-.-.-.-.-.-.-.-.-.-.-.-.- Fields in Receive-Status Register -.-.-.-.-.-.-.-.-.-.-.-.-.-
// Defined this way for efficient access to status flags (don't need multiple PCI
// cycles if we want to access multiple flags.)
#define  DTA1XX_RXSTATUS_OVFINT			0x200
#define  DTA1XX_RXSTATUS_SYNCINT		0x400
#define  DTA1XX_RXSTATUS_RATEOVFINT		0x2000

//.-.-.-.-.-.-.-.-.-.-.-.-.- Fields in Transmit-Status Register -.-.-.-.-.-.-.-.-.-.-.-.-.
// Defined this way for efficient access to status flags (don't need multiple PCI
// cycles if we want to access multiple flags.)
#define  DTA1XX_TXSTATUS_PERINT			0x100			// LEGACY
#define  DTA1XX_TXSTATUS_UFLINT			0x200
#define  DTA1XX_TXSTATUS_SYNCINT		0x400
#define  DTA1XX_TXSTATUS_SHORTINT		0x800

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Miscellanea -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Local address of FIFO-data register
#define  DTA1XX_LOCALADDR_FIFODATA		64


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ GENERAL NETWORK CONTROL +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Register block for General Network Control
//
// Supports: DTA-160/2160

// GENERAL-NETWORK-CONTROL Registers: BYTE offset
#define  DTA1XX_NW_INT_CONTROL			0x0104
#define  DTA1XX_NW_INT_STATUS			0x0108
#define  DTA1XX_NW_90KHZ_REFCLK			0x010c

//.- GENERAL NETWORK CONTROL - 90KHz Reference Clock Count Register - Access Functions -.-
static __inline UInt  Dta1xxNwGet90KhzRefClkCnt(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_NW_90KHZ_REFCLK);
}
static __inline void  Dta1xxNwSet90KHzRefClkCnt(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_NW_90KHZ_REFCLK);
}

//-.-.-.-.-.-.-.- GENERAL NETWORK CONTROL - MAC Address - Access Functions -.-.-.-.-.-.-.-

// Utility functions for MAC-address conversion
static __inline  UInt32  MAC_ADDRESS_LOW(UInt8* MacAddr) {
	return (UInt32)(MacAddr[0] | (MacAddr[1]<<8) | (MacAddr[2]<<16) | (MacAddr[3]<<24));
}
static __inline  UInt32  MAC_ADDRESS_HIGH(UInt8* MacAddr) {
	return (UInt32)(MacAddr[4] | (MacAddr[5]<<8));
}

//.-.-.-.-.-.- GENERAL NETWORK CONTROL - Interrupt Control - Access Functions -.-.-.-.-.-.

// Bit fields
#define  DTA1XX_NWINTCTRL_MACINTEN		0x1
#define  DTA1XX_NWINTCTRL_MDIOINTEN		0x2
#define  DTA1XX_NWINTCNRL_LINKVALID		0x100

// Network Interrupt Control: Register access
static __inline UInt  Dta1xxNwIntCtrlGet(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_NW_INT_CONTROL);
}
static __inline void  Dta1xxNwIntCtrlSet(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_NW_INT_CONTROL);
}

// Network Interrupt Status: Register access
static __inline UInt  Dta1xxNwIntStatGet(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_NW_INT_STATUS);
}
static __inline void  Dta1xxNwIntStatSet(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_NW_INT_STATUS);
}

// Ethernet PHY interrupt enable/disable (MdioIntEn)
static __inline void  Dta1xxNwIntCtrlSetMdioIntEn(volatile UInt8* pBase, UInt Enable)
{
	UInt32  Val = Dta1xxNwIntCtrlGet(pBase);
	if (Enable != 0) Val |=  DTA1XX_NWINTCTRL_MDIOINTEN;
	else			 Val &= ~DTA1XX_NWINTCTRL_MDIOINTEN;
	Dta1xxNwIntCtrlSet(pBase, Val);
}

// Ethernet MAC interrupt enable/disable (MacIntEn)
static __inline void  Dta1xxNwIntCtrlSetMacIntEn(volatile UInt8* pBase, UInt Enable)
{
	UInt32  Val = Dta1xxNwIntCtrlGet(pBase);
	if (Enable != 0) Val |=  DTA1XX_NWINTCTRL_MACINTEN;
	else			 Val &= ~DTA1XX_NWINTCTRL_MACINTEN;
	Dta1xxNwIntCtrlSet(pBase, Val);
}

// Ethernet link valid yes/no
static __inline void  Dta1xxNwIntCtrlSetLinkValid(volatile UInt8* pBase, UInt Valid)
{
	UInt32  Val = Dta1xxNwIntCtrlGet(pBase);
	if (Valid != 0) Val |=  DTA1XX_NWINTCNRL_LINKVALID;
	else			Val &= ~DTA1XX_NWINTCNRL_LINKVALID;
	Dta1xxNwIntCtrlSet(pBase, Val);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ NETWORK RECEIVE +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Register block for Network Rx
//
// Supports: DTA-160/2160

// NETWORK RECEIVE Registers: BYTE offset
#define  DTA1XX_NWRX_CTRL			0x0004
#define  DTA1XX_NWRX_STAT			0x0008
#define  DTA1XX_NWRX_FIFO_LOAD		0x0018
#define  DTA1XX_NWRX_PKT_VAL_CNT	0x0030
#define  DTA1XX_NWRX_PKT_INV_CNT	0x0034


//-.-.-.-.-.-.- NETWORK RECEIVE - Receive Control - Access Functions -.-.-.-.-.-.-.

// Bit fields
#define  DTA1XX_NWRX_CTRL_DEL_LEN_ERR			(1<<0)
#define  DTA1XX_NWRX_CTRL_DEL_CRC_ERR			(1<<1)
#define  DTA1XX_NWRX_CTRL_DEL_RX_ERR			(1<<2)
#define  DTA1XX_NWRX_CTRL_DEL_DRIB_ERR			(1<<3)
// bit 4..5 reserved
#define  DTA1XX_NWRX_CTRL_DEL_COL_ERR			(1<<6)
#define  DTA1XX_NWRX_CTRL_DEL_FTL_ERR			(1<<7)
// bit 8..10 reserved
#define  DTA1XX_NWRX_CTRL_DEL_RNT_ERR			(1<<11)
// bit 12..15 reserved
#define  DTA1XX_NWRX_CTRL_DEL_IPSUM_ERR			(1<<16)
#define  DTA1XX_NWRX_CTRL_DEL_UDPSUM_ERR		(1<<17)
#define  DTA1XX_NWRX_CTRL_DEL_TCPSUM_ERR		(1<<18)

#define  DTA1XX_NWRX_CTRL_DEL_MASK		(DTA1XX_NWRX_CTRL_DEL_LEN_ERR  | \
										 DTA1XX_NWRX_CTRL_DEL_CRC_ERR  | \
										 DTA1XX_NWRX_CTRL_DEL_COL_ERR  | \
										 DTA1XX_NWRX_CTRL_DEL_FTL_ERR  | \
										 DTA1XX_NWRX_CTRL_DEL_DRIB_ERR | \
										 DTA1XX_NWRX_CTRL_DEL_RNT_ERR  | \
										 DTA1XX_NWRX_CTRL_DEL_IPSUM_ERR | \
										 DTA1XX_NWRX_CTRL_DEL_UDPSUM_ERR | \
										 DTA1XX_NWRX_CTRL_DEL_TCPSUM_ERR)
/* Temp disable GMII error for Tandberg problem in DTA1XX_NWRX_CTRL_DEL_MASK
	DTA1XX_NWRX_CTRL_DEL_RX_ERR   | \*/

#define  DTA1XX_NWRX_CTRL_VAL_CNT_OVF_INT_EN	(1<<19)
#define  DTA1XX_NWRX_CTRL_INV_CNT_OVF_INT_EN	(1<<20)
#define  DTA1XX_NWRX_CTRL_RX_FIFO_OVF_INT_EN	(1<<21)
// bit 22..23 reserved
#define DTA1XX_NWRX_CTRL_CLR_FIFO				(1<<24)
#define DTA1XX_NWRX_CTRL_RST_FIFO				(1<<25)
#define DTA1XX_NWRX_CTRL_RX_CONTROL				(1<<26)
// bit 27..31 reserved

// Network Rx control registers
static __inline void  Dta1xxNwRxCtrlSet(volatile UInt8* pBase, UInt32 Val)
{
	WRITE_UINT(Val, pBase, DTA1XX_NWRX_CTRL);
}

static __inline UInt32  Dta1xxNwRxCtrlGet(volatile UInt8* pBase)
{
	return READ_UINT(pBase, DTA1XX_NWRX_CTRL);
}

static __inline void  Dta1xxNwRxCtrlSetRxControl(volatile UInt8* pBase, UInt RxEna)
{
	UInt32  Val = Dta1xxNwRxCtrlGet(pBase);
	if (RxEna)  Val |= DTA1XX_NWRX_CTRL_RX_CONTROL;
	else		Val &= ~DTA1XX_NWRX_CTRL_RX_CONTROL;
	Dta1xxNwRxCtrlSet(pBase, Val);
}

static __inline void  Dta1xxNwRxCtrlSetRxFifoOvfIntEn(volatile UInt8* pBase, UInt IntEna)
{
	UInt32  Val = Dta1xxNwRxCtrlGet(pBase);
	if (IntEna)  Val |= DTA1XX_NWRX_CTRL_RX_FIFO_OVF_INT_EN;
	else		Val &= ~DTA1XX_NWRX_CTRL_RX_FIFO_OVF_INT_EN;
	Dta1xxNwRxCtrlSet(pBase, Val);
}

static __inline void  Dta1xxNwRxCtrlSetDelPckFpga(volatile UInt8* pBase, UInt EnaDel)
{
	UInt32  Val = Dta1xxNwRxCtrlGet(pBase);
	if (EnaDel) Val |= DTA1XX_NWRX_CTRL_DEL_MASK;
	else		Val &= ~DTA1XX_NWRX_CTRL_DEL_MASK;
	Dta1xxNwRxCtrlSet(pBase, Val);
}

static __inline void  Dta1xxNwRxCtrlSetDelLenPckFpga(volatile UInt8* pBase)
{
	UInt32  Val = Dta1xxNwRxCtrlGet(pBase);
	Val |= DTA1XX_NWRX_CTRL_DEL_LEN_ERR;
	Dta1xxNwRxCtrlSet(pBase, Val);
}

// Network Rx Status: Register access
static __inline UInt  Dta1xxNwRxStatGet(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_NWRX_STAT);
}
static __inline void  Dta1xxNwRxStatSet(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_NWRX_STAT);
}

// Get invalid packet count
static __inline UInt32  Dta1xxNwRxInvCntGet(volatile UInt8* pBase)
{
	return READ_UINT(pBase, DTA1XX_NWRX_PKT_INV_CNT);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ NETWORK TRANSMIT +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Register block for Network Tx
//
// Supports: DTA-160/2160

// NETWORK TRANSMIT Registers: BYTE offset
#define  DTA1XX_NWTX_CTRL			0x0004
#define  DTA1XX_NWTX_STAT			0x0008
#define  DTA1XX_NWTX_FIFO_LOAD		0x0018
#define  DTA1XX_NWRX_PKT_CNT		0x0030

//.-.-.-.-.-.-.-.- NETWORK TRANSMIT - Transmit Control - Access Functions -.-.-.-.-.-.-.-.

// Bit fields
#define  DTA1XX_NWTX_CTRL_TX_ON_TIME_ENA		(1<<0)
// bit 1..18 reserved
#define  DTA1XX_NWTX_CTRL_TX_CNT_OVF_INT_EN		(1<<19)
#define  DTA1XX_NWTX_CTRL_TX_ERR_INT_EN			(1<<20)
#define  DTA1XX_NWTX_CTRL_TX_FIFO_OVF_INT_EN	(1<<21)
// bit 22..23 reserved
#define  DTA1XX_NWTX_CTRL_CLR_FIFO				(1<<24)
#define  DTA1XX_NWTX_CTRL_RST_FIFO				(1<<25)
#define  DTA1XX_NWTX_CTRL_TX_CONTROL			(1<<26)
// bit 27..31 reserved


// Set/get  control enable bits
static __inline UInt32  Dta1xxNwTxCtrlGet(volatile UInt8* pBase)
{
	return READ_UINT(pBase, DTA1XX_NWTX_CTRL);
}
static __inline void  Dta1xxNwTxCtrlSet(volatile UInt8* pBase, UInt32 Val)
{
	WRITE_UINT(Val, pBase, DTA1XX_NWTX_CTRL);
}

static __inline void  Dta1xxNwTxCtrlSetTxControl(volatile UInt8* pBase, UInt TxEna)
{
	UInt32  Val = Dta1xxNwTxCtrlGet(pBase);
	if (TxEna)  Val |= DTA1XX_NWTX_CTRL_TX_CONTROL;
	else		Val &= ~DTA1XX_NWTX_CTRL_TX_CONTROL;
	Dta1xxNwTxCtrlSet(pBase, Val);
}


static __inline void  Dta1xxNwTxCtrlSetTxOnTime(volatile UInt8* pBase, UInt TimeEna)
{
	UInt32  Val = Dta1xxNwTxCtrlGet(pBase);
	if (TimeEna) Val |= DTA1XX_NWTX_CTRL_TX_ON_TIME_ENA;
	else		 Val &= ~DTA1XX_NWTX_CTRL_TX_ON_TIME_ENA;
	Dta1xxNwTxCtrlSet(pBase, Val);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ ETHERNET CONTROLLER +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// Register block for Ethernet Controller
//
// Supports: DTA-160/2160

// ETHERNET-CONTROLLER Registers: BYTE offset
#define DTA1XX_ETH_BUS_MODE				0x000	// CSR0
#define DTA1XX_ETH_TX_POLL_DEMAND		0x008	// CSR1
#define DTA1XX_ETH_RX_POLL_DEMAND		0x010	// CSR2
#define DTA1XX_ETH_RX_DESCR_LIST_BAR	0x018	// CSR3
#define DTA1XX_ETH_TX_DESCR_LIST_BAR	0x020	// CSR4
#define DTA1XX_ETH_STATUS				0x028	// CSR5
#define DTA1XX_ETH_OPER_MODE			0x030	// CSR6
#define DTA1XX_ETH_INT_ENABLE			0x038	// CSR7
#define DTA1XX_ETH_MISSED_FRAMES_OVF	0x040	// CSR8
#define DTA1XX_ETH_SERIAL_ROM			0x048	// CSR9
#define DTA1XX_ETH_MII_SERIAL_MANAG		0x050	// CSR10
#define DTA1XX_ETH_GP_TIM_INT_MIR		0x058	// CSR11

#define DTA1XX_ETH_MAC_ADDR_LOW			0x080	// CSR16
#define DTA1XX_ETH_MAC_ADDR_HIGH		0x088	// CSR17
#define DTA1XX_ETH_PAUSE_CACHE_TRSH		0x090	// CSR18
#define DTA1XX_ETH_FIFO_TRSH			0x098	// CSR19
#define DTA1XX_ETH_FLOW_CONTROL			0x0A0	// CSR20
#define DTA1XX_ETH_SC_MODE				0x0A8	// CSR21
#define DTA1XX_ETH_SC_READ_VALUE		0x0B0	// CSR22

//.-.-.-.-.-.- ETHERNET CONTROLLER - Status register - Access Functions -.-.-.-.-.-.
//
static __inline UInt  Dta1xxEthStatusGet(volatile UInt8* pBase) {
	UInt  Val = READ_UINT(pBase, DTA1XX_ETH_STATUS);

	// Workaround for R/W MAC problem in FPGA
	while (Val == 0) {	// Some bits always 1
		Val = READ_UINT(pBase, DTA1XX_ETH_STATUS);
	}
	return Val;
}

//.-.-.-.-.-.- ETHERNET CONTROLLER - MII Serial Management - Access Functions -.-.-.-.-.-.
//
static __inline UInt  Dta1xxEthMiiSmRegGet(volatile UInt8* pBase) {
	return READ_UINT(pBase, DTA1XX_ETH_MII_SERIAL_MANAG);
}

static __inline void  Dta1xxEthMiiSmRegSet(volatile UInt8* pBase, UInt Val) {

	WRITE_UINT(Val, pBase, DTA1XX_ETH_MII_SERIAL_MANAG);

	// Workaround for R/W MAC problem in FPGA
	while ((READ_UINT(pBase, DTA1XX_ETH_MII_SERIAL_MANAG)&0x0fffffff) != (Val&0x0fffffff)) {
		WRITE_UINT(Val, pBase, DTA1XX_ETH_MII_SERIAL_MANAG);
	}
}

//.-.-.-.-.-.-.- ETHERNET CONTROLLER - Sc Mode - Access Functions + defines -.-.-.-.-.-.-.
// Receive
#define DTA1XX_SCM_REG_RXFRMOK			0x8100		// #frames Rx'ed ok
#define DTA1XX_SCM_REG_RXOCTOK			0x8101		// #bytes Rx'ed ok
#define DTA1XX_SCM_REG_RXUNIOK			0x8102		// #unicast frames Rx'ed ok
#define DTA1XX_SCM_REG_RXMULTIOK		0x8103		// #multicast frames Rx'ed ok
#define DTA1XX_SCM_REG_RXBROADOK		0x8104		// #broadcast frames Rx'ed ok
#define DTA1XX_SCM_REG_RXFRMSIZE0		0x8105		// #Good packets Rx'ed grouped by size
#define DTA1XX_SCM_REG_RXFRMSIZE1		0x8106		// #Good packets Rx'ed grouped by size
#define DTA1XX_SCM_REG_RXFRMSIZE2		0x8107		// #Good packets Rx'ed grouped by size
#define DTA1XX_SCM_REG_RXFRMSIZE3		0x8108		// #Good packets Rx'ed grouped by size
#define DTA1XX_SCM_REG_RXFRMSIZE4		0x8109		// #Good packets Rx'ed grouped by size
#define DTA1XX_SCM_REG_RXFRMSIZE5		0x810A		// #Good packets Rx'ed grouped by size
#define DTA1XX_SCM_REG_RXFRMSIZE6		0x810B		// #Good packets Rx'ed grouped by size
#define DTA1XX_SCM_REG_RXFRMSIZE7		0x810C		// #Good packets Rx'ed grouped by size
#define DTA1XX_SCM_REG_RXPAUSE			0x810D		// #frames Rx'ed ok with MAC pause control
#define DTA1XX_SCM_REG_RXALIGNERR		0x810E		// #frames Rx'ed with align errors
#define DTA1XX_SCM_REG_RXFCSERR			0x810F		// #frames Rx'ed and do not pass the FCS check

// Transmit
#define DTA1XX_SCM_REG_TXFRMOK			0x8120		// #frames Tx'ed ok (broadcasts,
													// multicast, unicast included)
#define DTA1XX_SCM_REG_TXOCTOK			0x8121		// #bytes Tx'ed ok
#define DTA1XX_SCM_REG_TXUNIOK			0x8122		// #unicast frames Tx'ed ok
#define DTA1XX_SCM_REG_TXMULTIOK		0x8123		// #multicast frames Tx'ed ok
#define DTA1XX_SCM_REG_TXBROADOK		0x8124		// #broadcast frames Tx'ed ok
#define DTA1XX_SCM_REG_COLL0			0x8125		// #frames Tx'ed ok with 0 collisions
#define DTA1XX_SCM_REG_COLL1			0x8126		// #frames Tx'ed ok with 1 collisions
#define DTA1XX_SCM_REG_COLL2			0x8127		// #frames Tx'ed ok with 2 collisions
#define DTA1XX_SCM_REG_COLL3			0x8128		// #frames Tx'ed ok with 3 collisions
#define DTA1XX_SCM_REG_COLL4			0x8129		// #frames Tx'ed ok with 4 collisions
#define DTA1XX_SCM_REG_COLL5			0x812A		// #frames Tx'ed ok with 5 collisions
#define DTA1XX_SCM_REG_COLL6			0x812B		// #frames Tx'ed ok with 6 collisions
#define DTA1XX_SCM_REG_COLL7			0x812C		// #frames Tx'ed ok with 7 collisions
#define DTA1XX_SCM_REG_COLL8			0x812D		// #frames Tx'ed ok with 8 collisions
#define DTA1XX_SCM_REG_COLL9			0x812E		// #frames Tx'ed ok with 9 collisions
#define DTA1XX_SCM_REG_COLL10			0x812F		// #frames Tx'ed ok with 10 collisions
#define DTA1XX_SCM_REG_COLL11			0x8130		// #frames Tx'ed ok with 11 collisions
#define DTA1XX_SCM_REG_COLL12			0x8131		// #frames Tx'ed ok with 12 collisions
#define DTA1XX_SCM_REG_COLL13			0x8132		// #frames Tx'ed ok with 13 collisions
#define DTA1XX_SCM_REG_COLL14			0x8133		// #frames Tx'ed ok with 14 collisions
#define DTA1XX_SCM_REG_COLL15			0x8134		// #frames Tx'ed ok with 15 collisions
#define DTA1XX_SCM_REG_COLL16			0x8135		// #frames Tx'ed aborted after 16
													// collisions
#define DTA1XX_SCM_REG_TXDEF			0x8136		// #frames Tx'ed ok, no coll, media busy
#define DTA1XX_SCM_REG_TXPAUSE			0x8137		// #frames Tx'ed ok with pause operand
#define DTA1XX_SCM_REG_TXLCERR			0x813A		// #frames Tx'ed with late collision
#define DTA1XX_SCM_REG_TXMACERR			0x813B		// #frames Tx'ed aborted due to underrun
#define DTA1XX_SCM_REG_TXCSERR			0x813C		// #frames Tx'ed with no carrier sense
													// or dropped during send

static __inline void Dta1xxEthScModeLockCounters(volatile UInt8* pBase)
{
	// Lock the counters
	WRITE_UINT(0x8000, pBase, DTA1XX_ETH_SC_MODE);
}

static __inline BOOLEAN Dta1xxEthScModeAreCountersLocked(volatile UInt8* pBase)
{
	// Are counters locked?
	return (READ_UINT(pBase, DTA1XX_ETH_SC_MODE) & 0x8000) != 0;
}

static __inline UInt32 Dta1xxEthScModeGetCounterValue(volatile UInt8* pBase, UInt Register)
{
	// Set SC mode register to read mode with the register to read
	WRITE_UINT(Register, pBase, DTA1XX_ETH_SC_MODE);

	// Workaround for R/W MAC problem in FPGA
	while (READ_UINT(pBase, DTA1XX_ETH_SC_MODE) != Register) {
		// Set SC mode register to read mode with the register to read
		WRITE_UINT(Register, pBase, DTA1XX_ETH_SC_MODE);
	}

	// Read the value
	return READ_UINT(pBase, DTA1XX_ETH_SC_READ_VALUE);
}

static __inline void Dta1xxEthScModeUnLockCounters(volatile UInt8* pBase)
{
	// Clear the read request
	// Workaround for R/W MAC problem in FPGA
	// A 1 is written in stead of 0, to detect reading a value <> 0
	// by checking the status of the unlocked state.
	WRITE_UINT(0x1, pBase, DTA1XX_ETH_SC_MODE);
}

static __inline BOOLEAN Dta1xxEthScModeAreCountersUnLocked(volatile UInt8* pBase)
{
	// Are counters UnLocked?
	// Workaround for R/W MAC problem in FPGA
	// 0x1 is used in stead of 0. See Dta1xxEthScModeUnLockCounters
	return (READ_UINT(pBase, DTA1XX_ETH_SC_MODE)) == 0x1;
}

static __inline UInt32 Dta1xxEthBusModeGet(volatile UInt8* pBase)
{
	return READ_UINT(pBase, DTA1XX_ETH_BUS_MODE);
}

static __inline void Dta1xxEthBusModeSet(volatile UInt8* pBase, UInt32 Value)
{
	WRITE_UINT(Value, pBase, DTA1XX_ETH_BUS_MODE);
}

//.-.-.-.-.-.- ETHERNET CONTROLLER - Missed Frames Counter - Access Functions -.-.-.-.-.-.
static __inline UInt32 Dta1xxEthMissedFramesCounterGet(volatile UInt8* pBase)
{
	UInt32  Val = READ_UINT(pBase, DTA1XX_ETH_MISSED_FRAMES_OVF);

	return ((Val >> 17) & 0x7ff) +	// Fifo Overflow Counter
		    (Val  & 0xffff);		// Missed frames counter
}

//-.-.-.-.-.-.-.- ETHERNET CONTROLLER - Operation Mode - Access Functions -.-.-.-.-.-.-.-.

// Operation Mode: Register access
static __inline UInt  Dta1xxEthOperModeGet(volatile UInt8* pBase) {
	UInt  Val = READ_UINT(pBase, DTA1XX_ETH_OPER_MODE);

	// Workaround for R/W MAC problem in FPGA
	while (Val == 0) {	// Some bits always 1
		Val = READ_UINT(pBase, DTA1XX_ETH_OPER_MODE);
	}
	return Val;
}
static __inline void  Dta1xxEthOperModeSet(volatile UInt8* pBase, UInt Val) {
	WRITE_UINT(Val, pBase, DTA1XX_ETH_OPER_MODE);

	// Workaround for R/W MAC problem in FPGA
	while (READ_UINT(pBase, DTA1XX_ETH_OPER_MODE) != Val) {
		WRITE_UINT(Val, pBase, DTA1XX_ETH_OPER_MODE);
	}
}

// Bit fields
#define  DTA1XX_ETHOPM_STARTSTOPRX		(1<<1)
#define  DTA1XX_ETHOPM_STARTSTOPTX		(1<<13)

// Start/Stop Receive command
static __inline void  Dta1xxEthEnableRx(volatile UInt8* pBase, UInt Enable)
{
	UInt32  Val = Dta1xxEthOperModeGet(pBase);
	if (Enable != 0) Val |=  DTA1XX_ETHOPM_STARTSTOPRX;
	else			 Val &= ~DTA1XX_ETHOPM_STARTSTOPRX;
	Dta1xxEthOperModeSet(pBase, Val);
}

// Start/Stop Transmit command
static __inline void  Dta1xxEthEnableTx(volatile UInt8* pBase, UInt Enable)
{
	UInt32  Val = Dta1xxEthOperModeGet(pBase);
	if (Enable != 0) Val |=  DTA1XX_ETHOPM_STARTSTOPTX;
	else			 Val &= ~DTA1XX_ETHOPM_STARTSTOPTX;
	Dta1xxEthOperModeSet(pBase, Val);
}

// Set MAC address
static __inline void  Dta1xxEthMacAddressSet(volatile UInt8* pBase, UInt8* MacAddr)
{
	WRITE_UINT(MAC_ADDRESS_LOW(MacAddr),  pBase, DTA1XX_ETH_MAC_ADDR_LOW);
	WRITE_UINT(MAC_ADDRESS_HIGH(MacAddr), pBase, DTA1XX_ETH_MAC_ADDR_HIGH);

	// Workaround for R/W MAC problem in FPGA
	while (READ_UINT(pBase, DTA1XX_ETH_MAC_ADDR_LOW) != MAC_ADDRESS_LOW(MacAddr)) {
		WRITE_UINT(MAC_ADDRESS_LOW(MacAddr), pBase, DTA1XX_ETH_MAC_ADDR_LOW);
	}
	while (READ_UINT(pBase, DTA1XX_ETH_MAC_ADDR_HIGH) != MAC_ADDRESS_HIGH(MacAddr)) {
		WRITE_UINT(MAC_ADDRESS_HIGH(MacAddr), pBase, DTA1XX_ETH_MAC_ADDR_HIGH);
	}
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ ETHERNET TO DO +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//Status reg bit defines
#define DTA1XX_ETH_STATUS_SEND_RECEIVE_MASK			((7 << 17) | (7 <<20))

//MMII bit definitions
#define DTA1XX_ETH_MIISM_START_BUSY_MASK			(1    << 31)
#define DTA1XX_ETH_MIISM_OPCODE_READ				(0x2  << 26)
#define DTA1XX_ETH_MIISM_OPCODE_WRITE				(0x1  << 26)
#define DTA1XX_ETH_MIISM_OPCODE_CLKDIV				(0x3  << 26)
#define DTA1XX_ETH_MIISM_CLKDIV_MASK				(7    << 28)
#define DTA1XX_ETH_MIISM_CLKDIV_8					(0    << 28)
#define DTA1XX_ETH_MIISM_CLKDIV_16					(1    << 28)
#define DTA1XX_ETH_MIISM_CLKDIV_32					(2    << 28)
#define DTA1XX_ETH_MIISM_CLKDIV_64					(3    << 28)
#define DTA1XX_ETH_MIISM_CLKDIV_128					(4    << 28)
#define DTA1XX_ETH_MIISM_CLKDIV_256					(5    << 28)
#define DTA1XX_ETH_MIISM_CLKDIV_512					(6    << 28)
#define DTA1XX_ETH_MIISM_CLKDIV_1024				(7    << 28)
#define DTA1XX_ETH_MIISM_PHYADDR					(1    << 21)
#define DTA1XX_ETH_MIISM_REG_ADDRESS_SHIFT			(16)
#define DTA1XX_ETH_MIISM_REG_ADDRESS_MASK			(0x1F << DTA1XX_ETH_MIISM_REG_ADDRESS_SHIFT)
#define DTA1XX_ETH_MIISM_DATA_MASK					(0xFFFF)

// Address definitions general
#define DTA1XX_ETH_MIISM_ADDRESS_BMCR				(0x00)		// Basic Mode Control
#define DTA1XX_ETH_MIISM_ADDRESS_BMSR				(0x01)		// Basic Mode Status
#define DTA1XX_ETH_MIISM_ADDRESS_PHYIDR1			(0x02)		// Phy Identifier 1
#define DTA1XX_ETH_MIISM_ADDRESS_PHYIDR2			(0x03)		// Phy Identifier 2
#define DTA1XX_ETH_MIISM_ADDRESS_ANAR				(0x04)		// For Marvell: SWRESET needed
#define DTA1XX_ETH_MIISM_ADDRESS_ANLAR				(0x05)
#define DTA1XX_ETH_MIISM_ADDRESS_1KTCR				(0x09)		// For Marvell: SWRESET needed
#define DTA1XX_ETH_MIISM_ADDRESS_1KSTSR				(0x0A)		// 1K-T Status
#define DTA1XX_ETH_MIISM_ADDRESS_LINK_AN			(0x11)		// Bits Marvell/National not identical!

// Extra address definitions National
#define DTA1XX_ETH_MIISM_ADDRESS_NAT_AUX_CTRL		(0x12)
#define DTA1XX_ETH_MIISM_ADDRESS_NAT_INT_STAT		(0x14)
#define DTA1XX_ETH_MIISM_ADDRESS_NAT_INT_MASK		(0x15)
#define DTA1XX_ETH_MIISM_ADDRESS_NAT_INT_CLR		(0x17)
#define DTA1XX_ETH_MIISM_ADDRESS_NAT_EXP_MEM_DATA	(0x1D)
#define DTA1XX_ETH_MIISM_ADDRESS_NAT_EXP_MEM_ADDR	(0x1E)

// Extra address definitions Marvell
#define DTA1XX_ETH_MIISM_ADDRESS_MAR_INT_MASK		(0x12)
#define DTA1XX_ETH_MIISM_ADDRESS_MAR_INT_STAT		(0x13)
#define DTA1XX_ETH_MIISM_ADDRESS_MAR_INT_CLR		(0x13)		// Reading the int. status will clear the interrupts.


// Marvell 88E1111
#define DTA1XX_ETH_MIISM_ADDRESS_MAR_EXT_PHY_CTRL	(0x14)
#define DTA1XX_ETH_MIISM_ADDRESS_MAR_LED_CTRL		(0x18)
#define DTA1XX_ETH_MIISM_ADDRESS_MAR_EXT_ADDR		(0x1D)
#define DTA1XX_ETH_MIISM_ADDRESS_MAR_EXT_DATA		(0x1E)

// Marvell 88E1119R
#define DTA1XX_ETH_MIISM_ADDRESS_MAR_COP_SPEC_CTRL1	(0x10)	// Page 0
#define DTA1XX_ETH_MIISM_ADDRESS_MAR_LED_FUNC_CTRL	(0x10)	// Page 3
#define DTA1XX_ETH_MIISM_ADDRESS_MAR_LED_POL_CTRL	(0x11)	// Page 3
#define DTA1XX_ETH_MIISM_ADDRESS_MAR_LED_TIM_CTRL	(0x12)	// Page 3
#define DTA1XX_ETH_MIISM_ADDRESS_MAR_MAC_SPEC_CTRL	(0x15)	// Page 2

#define DTA1XX_ETH_MIISM_ADDRESS_MAR_PAGE			(0x16)	// Page selection

// BMCR defines
#define DTA1XX_ETH_BMCR_SWRESET_SHIFT				(15)
#define DTA1XX_ETH_BMCR_SWRESET_MASK				(1  << DTA1XX_ETH_BMCR_SWRESET_SHIFT)
#define DTA1XX_ETH_BMCR_LOOPBACK_SHIFT				(14)
#define DTA1XX_ETH_BMCR_LOOPBACK_MASK				(1  << DTA1XX_ETH_BMCR_LOOPBACK_SHIFT)
#define DTA1XX_ETH_BMCR_SPEED0_SHIFT				(13)
#define DTA1XX_ETH_BMCR_SPEED0_MASK					(1  << DTA1XX_ETH_BMCR_SPEED0_SHIFT)
#define DTA1XX_ETH_BMCR_AUTO_NEG_EN_SHIFT			(12)
#define DTA1XX_ETH_BMCR_AUTO_NEG_EN_MASK			(1  << DTA1XX_ETH_BMCR_AUTO_NEG_EN_SHIFT)
#define DTA1XX_ETH_BMCR_POWER_DOWN_SHIFT			(11)
#define DTA1XX_ETH_BMCR_POWER_DOWN_MASK				(1  << DTA1XX_ETH_BMCR_POWER_DOWN_SHIFT)
#define DTA1XX_ETH_BMCR_AUTO_NEG_RST_SHIFT			(9)
#define DTA1XX_ETH_BMCR_AUTO_NEG_RST_MASK			(1  << DTA1XX_ETH_BMCR_AUTO_NEG_RST_SHIFT)
#define DTA1XX_ETH_BMCR_DUPLEX_SHIFT				(8)
#define DTA1XX_ETH_BMCR_DUPLEX_MASK					(1  << DTA1XX_ETH_BMCR_DUPLEX_SHIFT)
#define DTA1XX_ETH_BMCR_SPEED1_SHIFT				(6)
#define DTA1XX_ETH_BMCR_SPEED1_MASK					(1  << DTA1XX_ETH_BMCR_SPEED1_SHIFT)

// 1KTCR defines: 1Gb manual Master/Slave configuration
#define DTA1XX_ETH_1KTCR_MSCFG_EN_SHIFT				(12)
#define DTA1XX_ETH_1KTCR_MSCFG_EN_MASK				(1 << DTA1XX_ETH_1KTCR_MSCFG_EN_SHIFT)
#define DTA1XX_ETH_1KTCR_MSCFG_MASTER_SHIFT			(11)
#define DTA1XX_ETH_1KTCR_MSCFG_MASTER_MASK			(1 << DTA1XX_ETH_1KTCR_MSCFG_MASTER_SHIFT)
#define DTA1XX_ETH_1KTCR_MSCFG_CAP_SHIFT			(8)
#define DTA1XX_ETH_1KTCR_MSCFG_CAP_MASK				(3 << DTA1XX_ETH_1KTCR_MSCFG_CAP_SHIFT)


// LINK_AN defines NATIONAL
#define DTA1XX_ETH_LINK_AN_NAT_SPEEDST_SHIFT			(3)
#define DTA1XX_ETH_LINK_AN_NAT_SPEEDST_MASK				(0x3 << DTA1XX_ETH_LINK_AN_NAT_SPEEDST_SHIFT)

#define DTA1XX_ETH_LINK_AN_NAT_LINKST_SHIFT				(2)
#define DTA1XX_ETH_LINK_AN_NAT_LINKST_MASK				(0x1 << DTA1XX_ETH_LINK_AN_NAT_LINKST_SHIFT)

#define DTA1XX_ETH_LINK_AN_NAT_DUPLEXST_SHIFT			(1)
#define DTA1XX_ETH_LINK_AN_NAT_DUPLEXST_MASK			(0x1 << DTA1XX_ETH_LINK_AN_NAT_DUPLEXST_SHIFT)

#define DTA1XX_ETH_LINK_AN_NAT_MASTER_SHIFT				(0)
#define DTA1XX_ETH_LINK_AN_NAT_MASTER_MASK				(0x1 << DTA1XX_ETH_LINK_AN_NAT_MASTER_SHIFT)

// LINK_AN defines MARVELL
#define DTA1XX_ETH_LINK_AN_MAR_SPEEDST_SHIFT			(14)
#define DTA1XX_ETH_LINK_AN_MAR_SPEEDST_MASK				(0x3 << DTA1XX_ETH_LINK_AN_MAR_SPEEDST_SHIFT)

#define DTA1XX_ETH_LINK_AN_MAR_LINKST_SHIFT				(10)
#define DTA1XX_ETH_LINK_AN_MAR_LINKST_MASK				(0x1 << DTA1XX_ETH_LINK_AN_MAR_LINKST_SHIFT)

#define DTA1XX_ETH_LINK_AN_MAR_DUPLEXST_SHIFT			(13)
#define DTA1XX_ETH_LINK_AN_MAR_DUPLEXST_MASK			(0x1 << DTA1XX_ETH_LINK_AN_MAR_DUPLEXST_SHIFT)

//#define DTA1XX_ETH_LINK_AN_MAR_MASTER_SHIFT				()
//#define DTA1XX_ETH_LINK_AN_MAR_MASTER_MASK				(0x1 << DTA1XX_ETH_LINK_AN_MAR_MASTER_SHIFT)


//MMII State defines
#define MIISM_STATE_IDLE				0
#define MIISM_STATE_WRITE_WAIT_RDY		1
#define MIISM_STATE_READ_WAIT_RDY		2
#define MIISM_STATE_PREPARING			3
#define MIISM_STATE_INITIALIZING		4


#define MIISM_SUBSTATE_NO_ACTION		0
#define MIISM_SUBSTATE_READY			0

#define MIISM_SUBSTATE_BUSY				1
#define MIISM_SUBSTATE_BUSYRST			2

#define MIISM_CMD_INIT_PHY				0x01
#define MIISM_CMD_SET_CLK_DIVIDER		0x01
#define MIISM_CMD_SET_INT_MASK			0x02
#define MIISM_CMD_SET_CLR_INT			0x03
#define MIISM_CMD_SET_PHY_CONTROL		0x04
#define MIISM_CMD_SET_CLR_INT_INIT		0x05
#define MIISM_CMD_SET_SETUP_FRAME		0x06		// This is only an internal CMD
#define MIISM_CMD_SET_PHY_CONTROL_INIT	0x07
#define MIISM_CMD_SET_1GB_CTRL			0x08
#define MIISM_CMD_SET_STATCNT			0x09
#define MIISM_CMD_SET_PHY_EXTCONTROL	0x0A
#define MIISM_CMD_SET_PHY_CONTROL2		0x0B
#define MIISM_CMD_SET_PHY_SPECIFIC_INT	0x0C
#define MIISM_CMD_SET_PHY_LED_FUNC_CTRL	0x0D
#define MIISM_CMD_SET_PHY_LED_POL_CTRL	0x0E
#define MIISM_CMD_SET_PHY_LED_TIM_CTRL	0x0F
#define MIISM_CMD_SET_PHY_PAGE_ADDR_LED	0x10
#define MIISM_CMD_SET_COP_SPEC_CTRL		0x11
#define MIISM_CMD_SET_PHY_PAGE_ADDR_LPB	0x12
#define MIISM_CMD_SET_LOOPBACKSPEED		0x13

#define MIISM_CMD_GET_LINK_AUTONEG		0x81
#define MIISM_CMD_GET_BMSR				0x82
#define MIISM_CMD_GET_STATCNT			0x83
#define MIISM_CMD_GET_PHY_ID1			0x84
#define MIISM_CMD_GET_PHY_ID2			0x85

// Network speed
#define DTA1XX_ETH_NW_SPEED_1000		0x2
#define DTA1XX_ETH_NW_SPEED_100			0x1
#define DTA1XX_ETH_NW_SPEED_10			0x0

// Network link status
#define DTA1XX_ETH_NW_LINK_DOWN			0x0
#define DTA1XX_ETH_NW_LINK_UP			0x1

// Network duplex status
#define DTA1XX_ETH_NW_DUPLEX_HALF		0x0
#define DTA1XX_ETH_NW_DUPLEX_FULL		0x1

#define DTA1XX_ETH_NW_NAT_INT_MASK_BITS	0xE010			// Speed, Link and Duplex change + autoneg ready
#define DTA1XX_ETH_NW_MAR_INT_MASK_BITS	0x6C00			// Speed, Link and Duplex change + autoneg ready

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- MAC Setup frame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

typedef struct _Dta1xxPFAddr
{	UInt32	PhAddr0		:16;			// Physical Address (bit 15..0)
	UInt32	Unknown0	:16;
	UInt32	PhAddr1		:16;			// Physical Address (bit 31..16)
	UInt32	Unknown1	:16;
	UInt32	PhAddr2		:16;			// Physical Address (bit 47..32)
	UInt32	Unknown2	:16;
} Dta1xxPFAddr;

typedef struct _Dta1xxHFItem
{	UInt32	Item0		:16;			// Hash filter (bit 15..0)*i, i:0..32
	UInt32	Unknown0	:16;
} Dta1xxHFItem;

typedef struct _Dta1xxHFTable
{	Dta1xxHFItem HFItem[32];			// Hash filter table
	UInt32	Unknown1[7];
	Dta1xxPFAddr PFAddress;				// Unicast MAC address
	UInt32 Unknown2[6];
} Dta1xxHFTable;


#define  DTA1XX_FT_PERFECT		0		// Perfect filtering
#define  DTA1XX_FT_HASH			1		// Hash filtering
#define  DTA1XX_FT_INVERSE		2		// Inverse filtering
#define  DTA1XX_FT_HASH_ONLY	3		// Hash only filerting: 512 bit hash table

typedef struct _Dta1xxDmaTxHeader {
	UInt32	Tag;						// 445441A0h
	UInt32	Length:24;					// Header length
	UInt32	Version:8;					// Header version
	UInt32  Timestamp;					// 24 bits used by FPGA
	UInt32  Reserved2;
	struct {
		UInt32  PacketLength	: 11;	// Excluding header
		UInt32	Reserved1		: 11;
		UInt32  FilteringType0	: 1;
		UInt32  PaddingOff		: 1;
		UInt32	Reserved2		: 2;
		UInt32  CRCGenerateOff	: 1;
		UInt32  SetupFrame		: 1;
		UInt32  FilteringType1	: 1;
		UInt32	Reserved3		: 2;
		UInt32  IRQOnTx			: 1;
	} TransmitControl;
	UInt32  Reserved4[3];
} Dta1xxDmaTxHeader;

typedef struct _Dta1xxDmaRxHeader {
	UInt32	Tag;						// 445441A0h
	UInt32	Length:24;					// Header length
	UInt32	Version:8;					// Header version
	UInt64  Timestamp:64;				// Timestamp: not all bits are used
	struct {
		UInt32	Zero			: 1;
		UInt32	CRCError		: 1;
		UInt32	Dribbling		: 1;
		UInt32	ReportGMIIError	: 1;
		UInt32	Reserved1		: 1;
		UInt32	FrameType		: 1;
		UInt32	CollisionSeen	: 1;
		UInt32  FrameTooLong	: 1;
		UInt32  LastDescriptor	: 1;
		UInt32  FirstDescriptor	: 1;
		UInt32  MulticastFrame	: 1;
		UInt32  RuntFrame		: 1;
		UInt32  Reserved2		: 2;
		UInt32  DescriptorError	: 1;
		UInt32  ErrorSummary	: 1;
		UInt32  FrameLength		: 14;	// Ethernet packet length (including checksum)
		UInt32  FilteringFail	: 1;
		UInt32  Reserved3		: 1;
	} ReceiveStatus;					//(RDES0) register of the ethernet MAC
	struct{
		UInt32  IPChksumValid	: 1;
		UInt32  UDPChksumValid	: 1;
		UInt32  TCPChksumValid	: 1;
		UInt32	Reserved		: 29;
	} ChecksumStatus;
	UInt32  Reserved2[2];
} Dta1xxDmaRxHeader;

typedef struct _Dta1xxIpRawHeader {
	UInt16  Tag;						// 0x44A0h = 'D'160
	UInt16	Length;						// IP Packet Length;
	UInt32	TimeStamp;					// Timestamp
} Dta1xxIpRawHeader;

typedef struct _Dta1xxSetupFrame {
	Dta1xxDmaTxHeader  Header;
	union{
		Dta1xxPFAddr	PFAddress[16];
		Dta1xxHFTable	HTable;
	} FT;
	// Dta1xxPFAddr and Dta1xxHFTable must be off the same size in order to use
	// sizeof(Dta1xxSetupFrame) to return actual used bytes.s
} Dta1xxSetupFrame;


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DMA Registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

#define DTA1XX_DMA_DESCPTR_REG			0x004
#define DTA1XX_DMA_CMDSTAT_REG			0x008
#define DTA1XX_DMA_PCIUPADDR_REG		0x00C
#define DTA1XX_DMA_TIMEOUT_REG			0x014


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Timeout configuration register -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static __inline void  Dta1xxDmaTimeOutSet(volatile UInt8* pBase, UInt32 Val)
{	// Val = in 2^14 clock cycle increments
	BOOLEAN Enable;
	Enable = (Val != 0);
	Val = Val & 0xFF;
	if (Enable) Val = Val | (1 << 8);

	WRITE_UINT(Val, pBase, DTA1XX_DMA_TIMEOUT_REG);
}

static __inline UInt32  Dta1xxDmaTimeoutGet(volatile UInt8* pBase)
{
	return READ_UINT(pBase, DTA1XX_DMA_TIMEOUT_REG) & 0x1FF;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Command / Status bits -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA1XX_DMA_CMDSTAT_SIZE_EN		(0x1 << 7)
#define DTA1XX_DMA_CMDSTAT_SIZE			(0xFFFFFF << 8)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Dta1xxSpiGen +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Register block for SPI general DVB-SPI/BT-656 control
//
// Keep this data structure hidden: use exported functions to access the SPI general
// register map
//
// Supports: DTA-2142
//
typedef struct _Dta1xxSpiGen {

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- SPI general control register -.-.-.-.-.-.-.-.-.-.-.-.-.-
	struct {
		volatile UInt m_IoEnable		:  1; //	 0	RW I/O enable
		volatile UInt m_LvttlSelect		:  1; //	 1	RW LVTTL mode
		volatile UInt m_FailSafeEnable	:  1; //	 2	RW LVDS type-2 failsafe mode
		volatile UInt m_DssIoUpdate		:  1; //	 3	WO initiate DSS I/O update cycle
		volatile UInt m_SerialMode		:  1; //	 4	RW propriarity serial DVB mode
		volatile UInt m_ExtClkSelect	:  1; //	 5	RW select ExtClk input
		volatile UInt m_TestMode		:  1; //	 6	RW enable test mode
		volatile UInt m_PrngMode		:  1; //	 7	RW enable PRNG mode
		volatile UInt m_10bMode			:  1; //	 8	RW DVB raw 10-bit mode
		volatile UInt m_Reserved		: 23; // 31..10 not used
	} m_Control;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- SPI general status register -.-.-.-.-.-.-.-.-.-.-.-.-.-.
	struct {
		volatile UInt m_DssBusy			:  1; //	 0	RO DSS data cycle or IO update in
											  //		   progress
		volatile UInt m_IoError			:  1; //	 1	RC I/O error detected
		volatile UInt m_Reserved		: 30; // 31..2	   not used
	}  m_Status;

	//.-.-.-.-.-.-.-.-.-.-.-.-.- SPI general DSS data register -.-.-.-.-.-.-.-.-.-.-.-.-.-
	struct m_DssData_s {
		volatile UInt m_DssData			:  8; //  7..0	WO send data byte to DSS
		volatile UInt m_Reserved		: 24; // 31..8	   not used
	}  m_DssData;

	//-.-.-.-.-.-.-.-.-.-.-.- SPI general reserved register -.-.-.-.-.-.-.-.-.-.-.-
	UInt m_Reserved	  ; // Reserved

	//.-.-.-.-.-.-.-.-.-.-.-.-.- SPI general test data register -.-.-.-.-.-.-.-.-.-.-.-.-.
				 struct {
		volatile UInt m_TestData		:  8; //  9..0	RW send / received data word
		volatile UInt m_Reserved		: 24; // 31..10	   not used
	}  m_TestData;

	//-.-.-.-.-.-.-.-.-.-.-.-.- SPI general PRNG errors register -.-.-.-.-.-.-.-.-.-.-.-.-
	struct {
		volatile UInt m_Errors			: 16; // 15..0	RO Number of syndromes since
											  //		   PrngMode enable
		volatile UInt m_Reserved		: 16; // 31..8	   not used
	}  m_PrngErrors;

	//-.-.-.-.-.-.-.-.-.-.- SPI general receiver frequency register .-.-.-.-.-.-.-.-.-.-.-
		volatile UInt m_SpiGenRxFrequency	; // 31..0	RO receiver frequency count (1/s)

} Dta1xxSpiGen;

#endif // #ifndef __DTA1XXREGS_H
