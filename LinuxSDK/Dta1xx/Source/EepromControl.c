//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* EepromControl.c *#*#*#*#*#*#*#* (C) 2000-2007 DekTec
//
// Low-level serial EEPROM routines:
//	- For reading and writing to 93C66 (PCI cards with PLX chip)
//	- For reading and writing EPCS1, EPCS4 (FPGA)
//  - For reading and writing SPI (separate EEPROM)
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dta1xx driver, please refer to the Dta1xx readme


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "../Include/Dta1xx.h"
#include "../Include/Dta1xxRegs.h"
#include <linux/delay.h>		// udelay


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ 93C66 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Read and write serial EEPROM by directly using SK (Clock), CS (Chip Select), DI (Data
// Input) and DO (Data Output) pins of the serial EEPROM, instead of VPD routines in
// PCI9054/9056 (which are somewhat unreliable).
//
// WARNING
// =======
// Code to protect/unprotect writing to serial EEPROM CANNOT WORK:
//
//	1. The EEPROM in use on the DTA-100/102/120/122 is the 93C66L. This type does not
//	   support the Protect Register at all! The 93CS66L is required.
//
//	2. Even if the correct type of EEPROM is used, addresses ABOVE a threshold are
//	   protected. Unfortunately, the lay-out of the EEPROM (VPD read/write section at
//	   the end) was based on the assumption that address BELOW a certain value would be
//	   protected.
//	   So, if we want to protect the VPD read-only section, the VPD read/write section
//	   becomes read-only too.


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Tunable Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Serial EEPROM (minimum) data set-up time in microseconds
#define EEPROM_DELAY_DATA_SETUP_US		2

// Serial EEPROM (minimum) data hold time in microseconds
#define EEPROM_DELAY_DATA_HOLD_US		2

// Minimum CS low time in microseconds
#define EEPROM_MINIMUM_CS_LOW			4


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local Forwards -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
static void  EepromClearSkDoCs(DTA1XX_FDO*);
static void  EepromClockPulse(DTA1XX_FDO*);
static void  EepromCsFallingEdge(DTA1XX_FDO*);
static void  EepromEnableEedoPin(DTA1XX_FDO*);
static void  EepromFloatEedoPin(DTA1XX_FDO*);
static void  EepromInstruction(DTA1XX_FDO*, Int, Int, Int);
static UInt  EepromReadDi(DTA1XX_FDO*);
static void  EepromSetCs(DTA1XX_FDO*, Int);
static void  EepromSetCsDo(DTA1XX_FDO*, Int, Int);
static void  EepromSetDoAndClk(DTA1XX_FDO*, Int);
static void  EepromSetPeAndPre(DTA1XX_FDO*, Int, Int);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEepromReadWord16 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read one 16-bit word from the serial EEPROM.
//
Int  Dta1xxEepromReadWord16(
    DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	UInt  WordAddr,			// 16-bit word address
	UInt16*  pData)			// Data read from serial EEPROM
{
	Int  i;
	UInt  Word;

	// Protect access to EEPROM i/f lines on PLX
	if ( 0!=down_interruptible( &pFdo->m_VpdEepromMutex ) )
		return -EFAULT;

	// Initialisation
	EepromEnableEedoPin(pFdo);			// Make DO output (for 9056)
	EepromSetPeAndPre(pFdo, 0, 0);		// PE = 0, PRE = 0

	// READ Instruction, first part
	EepromInstruction(pFdo, 2, WordAddr, 0);

	// Turn around data line
	EepromFloatEedoPin(pFdo);			// Float DO pin
	EepromClockPulse(pFdo);				// Clock start bit (should be 0)

	// Read 16 bits into Word
	for (Word=0, i=0; i<16; i++) {
		Word = (Word<<1) | EepromReadDi(pFdo);
		EepromClockPulse(pFdo);
	}

	// Copy data to output parameter
	*pData = (UInt16)Word;

	// Finally: falling edge on CS
	EepromCsFallingEdge(pFdo);

	// Release EEPROM mutex
	up( &pFdo->m_VpdEepromMutex );

	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEepromWriteWord16 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write one 16-bit word to the serial EEPROM.
//
Int  Dta1xxEepromWriteWord16(
    DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	UInt WordAddr,			// 16-bit word address
	UInt Data)				// Data to be written to serial EEPROM
{
	Int  Count, i, Mask;
#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "Dta1xxEepromWriteWord16 Addr=0x%x Data=0x%04x",
				WordAddr, Data );
#endif

	// Protect access to EEPROM i/f lines on PLX
	if ( 0!=down_interruptible( &pFdo->m_VpdEepromMutex ) )
		return -EFAULT;

	// Initialisation
	EepromEnableEedoPin(pFdo);			// Make DO output (for 9056)
	EepromSetPeAndPre(pFdo, 1, 0);		// PE = 1, PRE = 0

	// WEN Instruction
	EepromInstruction(pFdo, 0, 0xFF, 1);

	// WRITE Instruction
	EepromInstruction(pFdo, 1, WordAddr, 0);

	// Data D15..D0
	for ( Mask=0x8000, i=0; i<16; i++, Mask>>=1 )
		EepromSetDoAndClk(pFdo, ((Data&Mask)==0 ? 0 : 1));

	// Falling edge on CS triggers programming action
	EepromCsFallingEdge(pFdo);			// Falling edge on CS
	EepromFloatEedoPin(pFdo);			// Float DO pin

	// Set CS to 1, to wait for EEPROM Ready signal
	EepromSetCs(pFdo, 1);

	// Wait while EEPROM signals Busy (DI==0), in 10ms chunks, with a maximum of 100ms
	Count = 0;
	do {
		udelay(10000);
	} while (EepromReadDi(pFdo)==0 && ++Count<10);

	// Return unsuccessful if DI still signals Busy (0)
	if (EepromReadDi(pFdo) == 0) {
		// Don't forget to release EEPROM mutex
		up( &pFdo->m_VpdEepromMutex );
		return -EFAULT;
	}

	// WDS Instruction
	EepromInstruction(pFdo, 3, 0, 1);

	// Release PE (write) signal
	EepromSetPeAndPre(pFdo, 0, 0);		// PE = 0, PRE = 0

	// Release EEPROM mutex
	up( &pFdo->m_VpdEepromMutex );

	return 0;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ HELPERS +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=



//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EepromClearSkDoCs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Clear serial EEPROM lines: SK, CS, DO.
//
static void  EepromClearSkDoCs(
    DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	UInt32  Control;
	volatile UInt8*  pPci905XRegs = pFdo->m_Pci905XConfRegs.m_pKernel;

	// Clear SK, CS, DO
	Control = READ_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
	Control &= ~(PCI905X_CNTRL_EESK | PCI905X_CNTRL_EECS | PCI905X_CNTRL_EEDO);
	WRITE_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EepromClockPulse -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Issue a clock pulse on the SK line of the serial EEPROM.
//
static void  EepromClockPulse(
    DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	UInt32  Control;
	volatile UInt8*  pPci905XRegs = pFdo->m_Pci905XConfRegs.m_pKernel;

	// Let current value on DO stabilise (data set-up time)
	udelay(EEPROM_DELAY_DATA_SETUP_US);

	// Write 1 to SK and wait
	Control = READ_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
	Control |= PCI905X_CNTRL_EESK;
	WRITE_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);
	udelay(EEPROM_DELAY_DATA_HOLD_US);

	// Write 0 to SK
	Control &= ~PCI905X_CNTRL_EESK;
	WRITE_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EepromCsFallingEdge -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Generate falling edge on CS and wait minimum CS time.
// Clear DO, too.
//
static void  EepromCsFallingEdge(
    DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	udelay(EEPROM_DELAY_DATA_HOLD_US);
	EepromSetCsDo(pFdo, 0, 0);
	udelay(EEPROM_MINIMUM_CS_LOW);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EepromEnableEedoPin -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For boards with the PCI9056, clear EEDO Input Enable.
// The PCI9054 does not have an enable, but an open-drain output. Therefore, nothing
// needs to be done when the output needs to be enabled.
//
static void  EepromEnableEedoPin(
    DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	UInt32  Control;
	volatile UInt8*  pPci905XRegs = pFdo->m_Pci905XConfRegs.m_pKernel;

	if (pFdo->m_Uses9056) {
		Control  = READ_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
		Control &= ~(PCI9056_CNTRL_EEDO_INPEN);
		WRITE_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);
	}
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EepromFloatEedoPin -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Set DO (for 9054) and EEDO Input Enable (for 9056)
//
static void  EepromFloatEedoPin(
    DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	UInt32  Control;
	volatile UInt8*  pPci905XRegs = pFdo->m_Pci905XConfRegs.m_pKernel;

	Control  = READ_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
	if (pFdo->m_Uses9056)
		Control |= PCI9056_CNTRL_EEDO_INPEN;
	else
		Control |= PCI905X_CNTRL_EEDO;
	WRITE_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EepromInstruction -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Issue instruction to serial EEPROM.
//
void  EepromInstruction(
    DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  Command,			// Command (2 bits)
	Int  Address,			// Address bits
	Int  Close)				// Close instruction: must be 1 for WEN, WDS
{
	Int  i, Mask;

	// Make sure CS is low for at least EEPROM_MINIMUM_CS_LOW microseconds
	EepromClearSkDoCs(pFdo);				// SK = DO = CS = 0
	udelay(EEPROM_MINIMUM_CS_LOW);

	// Start bit
	EepromSetCsDo(pFdo, 1, 1);			// CS = 1, DO = 1; Clock (Start bit)
	EepromClockPulse(pFdo);

	// Command
	EepromSetDoAndClk(pFdo, (Command>>1)&1);
	EepromSetDoAndClk(pFdo, Command&1);

	// Address
	for (Mask=0x80, i=0; i<8; i++, Mask>>=1)
		EepromSetDoAndClk(pFdo, ((Address&Mask)==0 ? 0 : 1));

	// For 11-clock instructions: falling edge on CS
	if (Close)
		EepromCsFallingEdge(pFdo);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EepromReadDi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read DI bit (which is DO of the serial EEPROM).
//
static UInt  EepromReadDi(
    DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	volatile UInt8*  pPci905XRegs = pFdo->m_Pci905XConfRegs.m_pKernel;

	if ((READ_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT) & PCI905X_CNTRL_EEDI) == 0)
		return 0;
	else
		return 1;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EepromSetCs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Give a value to CS.
//
static void  EepromSetCs(
    DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  CsValue)
{
	UInt32  Control;
	volatile UInt8*  pPci905XRegs = pFdo->m_Pci905XConfRegs.m_pKernel;

	Control = READ_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
	Control &= ~PCI905X_CNTRL_EECS;
	if (CsValue != 0)
		Control |= PCI905X_CNTRL_EECS;
	WRITE_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EepromSetCsDo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Give a value to CS and Do.
//
static void  EepromSetCsDo(
    DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  CsValue,
	Int  DoValue)
{
	UInt32  Control;
	volatile UInt8*  pPci905XRegs = pFdo->m_Pci905XConfRegs.m_pKernel;

	Control = READ_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
	Control &= ~(PCI905X_CNTRL_EECS | PCI905X_CNTRL_EEDO);
	if (CsValue != 0)
		Control |= PCI905X_CNTRL_EECS;
	if (DoValue != 0)
		Control |= PCI905X_CNTRL_EEDO;
	WRITE_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EepromSetDoAndClk -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Give a value to DO and issue a clock pulse.
//
static void  EepromSetDoAndClk(
    DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  DoValue)
{
	UInt32  Control;
	volatile UInt8*  pPci905XRegs = pFdo->m_Pci905XConfRegs.m_pKernel;

	// Set DO
	Control = READ_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
	Control &= ~PCI905X_CNTRL_EEDO;
	if (DoValue != 0)
		Control |= PCI905X_CNTRL_EEDO;
	WRITE_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, Control);

	// Issue clock pulse
	EepromClockPulse(pFdo);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EepromSetPeAndPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  EepromSetPeAndPre(
    DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  Pe,				// PE (write enable): 0 or 1
	Int  Pre)				// PRE (Protect Register Enable): 0 or 1
{
	Dta1xxGenCtrlRegSetPE(pFdo->m_pGenRegs, Pe);
	Dta1xxGenCtrlRegSetPRE(pFdo->m_pGenRegs, Pre);
}


//=+=+=+=+=+=+=+=+=+=+=+=+ ROUTINES THAT CANNOT WORK (See Header) +=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSetProtectRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dta1xxSetProtectRegister(
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	return -EFAULT;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSetProtectRegisterClear -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dta1xxSetProtectRegisterClear(
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	return -EFAULT;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ EPCS1/EPCS4 Read/Write +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  MAX_ERASE_COUNT	10000000		// Maximum busy count for erase
#define  MAX_READ_COUNT		10000			// Maximum busy count for read
#define  MAX_PROG_COUNT		100000			// Maximum busy count for program


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static void  Dta1xxEepromEpcEraseSector(DTA1XX_FDO*, UInt);
static void  Dta1xxEepromEpcLock(DTA1XX_FDO*);
static void  Dta1xxEepromEpcUnlock(DTA1XX_FDO*);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEepromReadLastPage -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read last page of EEPROM (EPCS1/EPCS4) into cache.
//
Int  Dta1xxEepromReadLastPage(
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	Int  Addr, i;
	UInt  w;
	UInt8*  pData;
	volatile UInt8*  pBase = pFdo->m_Dta1xxRegsBase.m_pKernel;

	// Unlock access to EPCS interface
	Dta1xxEepromEpcUnlock(pFdo);

#if LOG_LEVEL>2
	DTA1XX_LOG( KERN_INFO, "Dta1xxEepromReadLastPage: Start reading EEPROM" );
#endif

	// Read data in 256-byte (page size) bursts
	pData = pFdo->m_pEepromCache;
	for (Addr=pFdo->m_EepromAddrLastSector; Addr<pFdo->m_EepromSize; Addr+=0x100) {

		// Issue EEPROM READ command
		Dta1xxEpcsCtrlRegRead(pBase, Addr);

		// Wait until busy is released
		i = 0;
		while (Dta1xxEpcsCtrlRegIsBusy(pBase) && i<MAX_READ_COUNT)
			i++;

		// Read data from FIFO
		for (i=0; i<64; i++) {
			w = Dta1xxGenGetEpcsData(pBase);
			*pData++ = (w>>24) & 0xFF;
			*pData++ = (w>>16) & 0xFF;
			*pData++ = (w>>8) & 0xFF;
			*pData++ = (w) & 0xFF;
		}
	}

#if LOG_LEVEL>2
	DTA1XX_LOG( KERN_INFO, "Dta1xxEepromReadLastPage: Reading of EEPROM completed" );
#endif

	// Lock EEPROM again
	Dta1xxEepromEpcLock(pFdo);

	return 0;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEepromWriteLastPage -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write last page of EEPROM (EPCS1/EPCS4) from cache.
//
Int  Dta1xxEepromWriteLastPage(
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	Int  Addr, i;
	UInt  w;
	UInt8*  pData;
	volatile UInt8*  pBase = pFdo->m_Dta1xxRegsBase.m_pKernel;

	// Unlock access to EPCS interface
	Dta1xxEepromEpcUnlock(pFdo);

#if LOG_LEVEL>2
	DTA1XX_LOG( KERN_INFO,
				"Dta1xxEepromWriteLastPage: Writing EEPROM; Erase last sector" );
#endif

	// Erase sector
	Dta1xxEepromEpcEraseSector(pFdo, pFdo->m_EepromAddrLastSector);

#if LOG_LEVEL>2
	DTA1XX_LOG( KERN_INFO,
				"Dta1xxEepromWriteLastPage: Erasing of last sector completed" );
#endif

	// Reprogram sector in 256-byte (page size) bursts
	pData = pFdo->m_pEepromCache;
	for (Addr=pFdo->m_EepromAddrLastSector; Addr<pFdo->m_EepromSize; Addr+=0x100) {

		// Write data to FIFO
		for (i=0; i<64; i++) {
			w = (pData[0]<<24) +  (pData[1]<<16) + (pData[2]<<8) + pData[3];
			Dta1xxGenSetEpcsData(pBase, w);
			pData += 4;
		}

		// Issue EEPROM PROGRAM command
		Dta1xxEpcsCtrlRegProg(pBase, Addr);

		// Wait until busy is released
		i = 0;
		while (Dta1xxEpcsCtrlRegIsBusy(pBase) && i<MAX_PROG_COUNT)
			i++;
	}

#if LOG_LEVEL>2
	DTA1XX_LOG( KERN_INFO, "Dta1xxEepromWriteLastPage: Writing of EEPROM completed");
#endif

	// Lock EEPROM again
	Dta1xxEepromEpcLock(pFdo);

	return 0;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ EPCS1/EPCS4 Helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEepromEpcEraseSector -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Erase one sector of the EPCS1 or EPCS4
//
static void  Dta1xxEepromEpcEraseSector(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	UInt  Addr)
{
	Int  Count;
	volatile UInt8*  pBase = pFdo->m_Dta1xxRegsBase.m_pKernel;

#if LOG_LEVEL>2
	DTA1XX_LOG( KERN_INFO, "Dta1xxEepromEpcEraseSector: Erase EPCS sector; Sector "
				"Address=0x%08x", Addr );
#endif

	// Issue Sector-Erase command
	Dta1xxEpcsCtrlRegErase(pBase, Addr);

	// Wait until busy is released
	Count = 0;
	while (Dta1xxEpcsCtrlRegIsBusy(pBase) && Count<MAX_ERASE_COUNT)
		Count++;
#if LOG_LEVEL>2
	DTA1XX_LOG( KERN_INFO, "Dta1xxEepromEpcEraseSector: Erase wait count: %d", Count );
#endif

	// Still busy => print out error
	if ( Dta1xxEpcsCtrlRegIsBusy(pBase) )
	{
#if LOG_LEVEL>0
		DTA1XX_LOG( KERN_INFO, "ERASE ERROR: still busy after waiting time" );
#endif
	}
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEepromEpcLock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Generate lock sequence, for closing EPCS1/EPCS4 read/write operations
//
static void  Dta1xxEepromEpcLock(
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	volatile UInt8*  pBase = pFdo->m_Dta1xxRegsBase.m_pKernel;

#if LOG_LEVEL>2
	DTA1XX_LOG( KERN_INFO, "Dta1xxEepromEpcLock: Lock EPCS" );
#endif

	// Write '1' to let FPGA go to the Idle state (=locked)
	Dta1xxEpcsCtrlRegSetUnlock(pBase, 0x1);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEepromEpcUnlock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Unlock sequence, so that we can start reading/writing the EPCS1 or EPCS4
//
static void  Dta1xxEepromEpcUnlock(
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	volatile UInt8*  pBase = pFdo->m_Dta1xxRegsBase.m_pKernel;

#if LOG_LEVEL>2
	DTA1XX_LOG( KERN_INFO, "Dta1xxEepromEpcUnlock: Unlock EPCS" );
#endif

	// Init: write '1' to let FPGA go to the Idle state
	Dta1xxEpcsCtrlRegSetUnlock(pBase, 0x1);

	Dta1xxEpcsCtrlRegSetUnlock(pBase, 0x3);
	Dta1xxEpcsCtrlRegSetUnlock(pBase, 0xD);
	Dta1xxEpcsCtrlRegSetUnlock(pBase, 0x6);
	Dta1xxEpcsCtrlRegSetUnlock(pBase, 0xC);
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ SPI Read/Write +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  MAX_SPI_READ_COUNT		10000			// Maximum busy count for read
#define  MAX_SPI_PROG_COUNT		100000			// Maximum busy count for program


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static void  Dta1xxEepromSpiLock(DTA1XX_FDO*);
static void  Dta1xxEepromSpiUnlock(DTA1XX_FDO*);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEepromSpiRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read EEPROM into cache.
//
Int  Dta1xxEepromSpiRead(
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	Int  Addr, i;
	UInt  w;
	UInt8*  pData;
	volatile UInt8*  pBase = pFdo->m_Dta1xxRegsBase.m_pKernel;

	// Unlock access to SPI interface
	Dta1xxEepromSpiUnlock(pFdo);

#if LOG_LEVEL>2
	DTA1XX_LOG( KERN_INFO, "Dta1xxEepromSpiRead: Start reading EEPROM" );
#endif

	// Read data in page size bursts
	pData = pFdo->m_pEepromCache;
	for (Addr = 0; Addr < pFdo->m_VpdEepromSize; Addr+= pFdo->m_VpdEepromPageSize) {

		// Issue EEPROM READ command
		Dta1xxSpiCtrlRegRead(pBase, Addr);

		// Wait until busy is released
		i = 0;
		while (Dta1xxSpiCtrlRegIsBusy(pBase) && i<MAX_SPI_READ_COUNT)
			i++;

		// Read data from FIFO
		for (i = 0; i < (pFdo->m_VpdEepromPageSize / 4); i++) {
			w = Dta1xxGenGetSpiData(pBase);
			*pData++ = (w>>24) & 0xFF;
			*pData++ = (w>>16) & 0xFF;
			*pData++ = (w>>8) & 0xFF;
			*pData++ = (w) & 0xFF;
		}
	}

#if LOG_LEVEL>2
	DTA1XX_LOG( KERN_INFO, "Dta1xxEepromSpiRead: Reading of EEPROM completed" );
#endif

	// Lock EEPROM again
	Dta1xxEepromSpiLock(pFdo);

	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEepromSpiWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write EEPROM from cache.
//
Int  Dta1xxEepromSpiWrite(
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	Int  Addr, i;
	UInt  w;
	UInt8*  pData;
	volatile UInt8*  pBase = pFdo->m_Dta1xxRegsBase.m_pKernel;

	// Unlock access to SPI interface
	Dta1xxEepromSpiUnlock(pFdo);

#if LOG_LEVEL>2
	DTA1XX_LOG( KERN_INFO, "Dta1xxEepromSpiWrite: Writing EEPROM" );
#endif

	// Reprogram EEPROM in page size bursts
	pData = pFdo->m_pEepromCache;
	for (Addr = 0; Addr < pFdo->m_VpdEepromSize; Addr+= pFdo->m_VpdEepromPageSize) {

		// Write data to FIFO
		for (i = 0; i < (pFdo->m_VpdEepromPageSize / 4); i++) {
			w = (pData[0]<<24) +  (pData[1]<<16) + (pData[2]<<8) + pData[3];
			Dta1xxGenSetSpiData(pBase, w);
			pData += 4;
		}

		// Issue EEPROM PROGRAM command
		Dta1xxSpiCtrlRegProg(pBase, Addr);

		// Wait until busy is released
		i = 0;
		while (Dta1xxSpiCtrlRegIsBusy(pBase) && i<MAX_SPI_PROG_COUNT) i++;
	}

#if LOG_LEVEL>2
	DTA1XX_LOG( KERN_INFO, "Dta1xxEepromSpiWrite: Writing of EEPROM completed" );
#endif

	// Lock EEPROM again
	Dta1xxEepromSpiLock(pFdo);

	return 0;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ SPI Helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEepromSpiLock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Generate lock sequence, for closing SPI read/write operations
//
static void  Dta1xxEepromSpiLock(
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	volatile UInt8*  pBase = pFdo->m_Dta1xxRegsBase.m_pKernel;

#if LOG_LEVEL>2
	DTA1XX_LOG( KERN_INFO, "Dta1xxEepromSpiLock: Lock SPI" );
#endif

	// Write '1' to let FPGA go to the Idle state (=locked)
	Dta1xxSpiCtrlRegSetUnlock(pBase, 0x1);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEepromSpiUnlock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Unlock sequence, so that we can start reading/writing the SPI
//
static void  Dta1xxEepromSpiUnlock(
	DTA1XX_FDO* pFdo)		// Functional device object, representing the DTA-1xx card
{
	volatile UInt8*  pBase = pFdo->m_Dta1xxRegsBase.m_pKernel;

#if LOG_LEVEL>2
	DTA1XX_LOG( KERN_INFO, "Dta1xxEepromSpiUnlock: Unlock SPI" );
#endif

	// Init: write '1' to let FPGA go to the Idle state
	Dta1xxSpiCtrlRegSetUnlock(pBase, 0x1);

	Dta1xxSpiCtrlRegSetUnlock(pBase, 0x3);
	Dta1xxSpiCtrlRegSetUnlock(pBase, 0xD);
	Dta1xxSpiCtrlRegSetUnlock(pBase, 0x6);
	Dta1xxSpiCtrlRegSetUnlock(pBase, 0xC);
}

