//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Utility.c *#*#*#*#*#*#*#*#*# (C) 2000-2007 DekTec

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dta1xx driver, please refer to the Dta1xx readme

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "../Include/Dta1xx.h"
#include "../Include/Dta1xxRegs.h"

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSubsystemId2TypeNumber -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt16  Dta1xxSubsystemId2TypeNumber(
	Int  SubsystemId)
{
	// For the DTA-106, a wrong ID is filled in the Device ID.
	if (SubsystemId == 0x106) {
		return 106;
	}
	return SubsystemId - (SubsystemId < 0xD100 ? 0 : (0xD100-100));
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxTxMode2PacketSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt  Dta1xxTxMode2PacketSize(
	Int  TxPacketMode)			// Packet transmit mode (TxMode, BurstMode)
{
	switch (TxPacketMode & 0x0F) {
	case DTA1XX_TXMODE_188:		return 188;
	case DTA1XX_TXMODE_192:		return 192;
	case DTA1XX_TXMODE_204:		return 204;
	case DTA1XX_TXMODE_ADD16:	return 204;
	case DTA1XX_TXMODE_RAW:		return 188;
	case DTA1XX_TXMODE_MIN16:	return 204;
	default:					return 188;		// Should not occur
	}
}

// Dta1xxWaitMs
Int  Dta1xxWaitMs(
      Int TimeInMs)
{
  udelay(TimeInMs*1000);  // ms->us (x1000)
  return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxBinDiv -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// 64 bit division  returns  num/denom and rest
//
Int64 Dta1xxBinDiv(Int64 num, Int64 denom, Int64* pRest)
{

    Int denombits, numbits ;
    Int64 restbits, restmask, rest;
    Int64 answer = 0;
    Int sign = 0;

    if (num < 0) {
        num = -num;
        sign = 1;
    }
    if (denom < 0) {
        denom = -denom;
        sign ^= 1;
    }

    if (num < denom)
        return 0;

    denombits = 0;
    while ((Int64)1 << denombits < denom)
        denombits++;

    numbits = denombits;
    while ((Int64)1 << numbits < num)
        numbits++;

    restbits = numbits - denombits;
    restmask = ((Int64)1 << restbits) - 1;
    rest = num >> restbits;
    num = num & restmask;

    while (1)
    {
        answer <<= 1;
        if (rest >= denom)
        {
            answer |= 1;
            rest -= denom;
        }

        if (restbits == 0)
            break;
        restbits--;
        restmask >>= 1;
        rest = (rest << 1) | (num >> restbits);
        num = num & restmask;
    }

    if (pRest)
        *pRest = rest;

    if (sign)
        return -answer;
    return answer;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxBinUDiv -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// 64 bit unsigned division  returns  num/denom and rest
//
UInt64 Dta1xxBinUDiv(UInt64 num, UInt64 denom, UInt64* pRest)
{

    Int denombits, numbits ;
    UInt64 restbits, restmask, rest;
    UInt64 answer = 0;
    
    if (num < denom)
        return 0;

    denombits = 0;
    while ((UInt64)1 << denombits < denom)
        denombits++;

    numbits = denombits;
    while ((UInt64)1 << numbits < num)
        numbits++;

    restbits = numbits - denombits;
    restmask = ((UInt64)1 << restbits) - 1;
    rest = num >> restbits;
    num = num & restmask;

    while (1)
    {
        answer <<= 1;
        if (rest >= denom)
        {
            answer |= 1;
            rest -= denom;
        }

        if (restbits == 0)
            break;
        restbits--;
        restmask >>= 1;
        rest = (rest << 1) | (num >> restbits);
        num = num & restmask;
    }

    if (pRest)
        *pRest = rest;

    return answer;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxGetPeriodicIntIntervalMicroS -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns the current periodic interval in micro seconds
//
UInt32  Dta1xxGetPeriodicIntIntervalMicroS(DTA1XX_FDO*  pFdo)
{
	UInt32 Value = Dta1xxGenCtrlRegGetPerIntInterval(pFdo->m_pGenRegs);

	switch(Value)
	{
		case 0: return 2430;
		case 1: return 4850;
		case 2: return 9710;
		case 3: return 19420;
		case 4: return 38840;
		case 5: return 77670;
		case 6: return 155340;
		case 7: return 310690;
	}
	return 40000;	// unknown, 40ms
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxReadConfigSpace -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read PCI configuration data
//
Int  Dta1xxReadConfigSpace(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	UInt32 Offset,			// Offset in configuration space
	UInt32 Length,			// Number of bytes to read
	UInt8* pBuffer)			// Buffer to store read bytes
{
	Int i=0, r;

	// Check for valid pointer
	if ( pBuffer == NULL )
		return -EFAULT;

	if ( Length == 0 )
		return 0;	// Nothing to transfer

	// Read bytes
	for ( i=0; i<Length; i++ )
	{
		r = pci_read_config_byte(pFdo->m_pPdo, Offset+i, pBuffer+i);
		if ( r!=0 )
			return r;
	}
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxWriteConfigSpace -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write PCI configuration data.
//
Int  Dta1xxWriteConfigSpace(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	UInt32 Offset,			// Offset in configuration space
	UInt32 Length,			// Number of bytes to write
	UInt8* pBuffer)			// Buffer with data to be written
{
	Int i, r;

	// Check for valid pointer
	if ( pBuffer == NULL )
		return -EFAULT;

	if ( Length == 0 )
		return 0;	// Nothing to transfer

	// Write bytes
	for ( i=0; i<Length; i++ )
	{
		r = pci_write_config_byte(pFdo->m_pPdo, Offset+i, pBuffer[i]);
		if ( r!=0 )
			return r;
	}
	return 0;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Modulation untility functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxIsHardQamAorC2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Check for QAM-A or QAM-C, without DTA1XX_MOD_DVBC_SOFT defined
//
BOOLEAN  Dta1xxIsHardQamAorC2(Int ModType, Int ParXtra0)
{
	Int  Annex = ParXtra0 & DTA1XX_MOD_J83_MSK;

	if (  ModType!=DTA1XX_MOD_QAM4   && ModType!=DTA1XX_MOD_QAM16
	   && ModType!=DTA1XX_MOD_QAM32  && ModType!=DTA1XX_MOD_QAM64
	   && ModType!=DTA1XX_MOD_QAM128 && ModType!=DTA1XX_MOD_QAM256)
		return FALSE;
	// Rule out soft QAM
	if ((ParXtra0 & DTA1XX_MOD_DVBC_SOFT) != 0)
		return FALSE;
	return (Annex==DTA1XX_MOD_J83_A || Annex==DTA1XX_MOD_J83_C);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxIsHardQamA -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
BOOLEAN  Dta1xxIsHardQamA(Int ModType, Int ParXtra0)
{
	Int  Annex = ParXtra0 & DTA1XX_MOD_J83_MSK;
	// Check we hava hard QAM-A or QAM-C
	if ( !Dta1xxIsHardQamAorC2(ModType, ParXtra0) )
		return FALSE;
	return (Annex==DTA1XX_MOD_J83_A);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxIsHardQamC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
BOOLEAN  Dta1xxIsHardQamC(Int ModType, Int ParXtra0)
{
	Int  Annex = ParXtra0 & DTA1XX_MOD_J83_MSK;
	// Check we hava hard QAM-A or QAM-C
	if ( !Dta1xxIsHardQamAorC2(ModType, ParXtra0) )
		return FALSE;
	return (Annex==DTA1XX_MOD_J83_C);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxIsIqMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Is ModType a mode that requires I/Q samples?
//
BOOLEAN  Dta1xxIsIqMode(Int ModType)
{
	if (    ModType==DTA1XX_MOD_ADTBT || ModType==DTA1XX_MOD_ATSC
		 || ModType==DTA1XX_MOD_CMMB  || ModType==DTA1XX_MOD_DMBTH
		 || ModType==DTA1XX_MOD_DVBT  || ModType==DTA1XX_MOD_DVBT2 
		 || ModType==DTA1XX_MOD_IQDIRECT || ModType==DTA1XX_MOD_IQDIRECT_NOLIC
		 || ModType==DTA1XX_MOD_ISDBT || ModType==DTA1XX_MOD_T2MI )
		return TRUE;
	else
		return FALSE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxIsQamB -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
BOOLEAN  Dta1xxIsQamB(Int ModType, Int ParXtra0)
{
	Int  Annex = ParXtra0 & DTA1XX_MOD_J83_MSK;
	return (Dta1xxIsQamMode(ModType) && Annex==DTA1XX_MOD_J83_B);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxIsQamMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
BOOLEAN  Dta1xxIsQamMode(Int ModType)
{
	if (    ModType==DTA1XX_MOD_QAM4   || ModType==DTA1XX_MOD_QAM16
		 || ModType==DTA1XX_MOD_QAM32  || ModType==DTA1XX_MOD_QAM64
		 || ModType==DTA1XX_MOD_QAM128 || ModType==DTA1XX_MOD_QAM256)
		return TRUE;
	else
		return FALSE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxNumBitsPerSymbol -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dta1xxNumBitsPerSymbol(Int ModType)
{
	Int NumBits=1;
	switch ( ModType )
	{
	case DTA1XX_MOD_QAM4:    NumBits=2; break;
	case DTA1XX_MOD_QAM16:   NumBits=4; break;
	case DTA1XX_MOD_QAM32:   NumBits=5; break;
	case DTA1XX_MOD_QAM64:   NumBits=6; break;
	case DTA1XX_MOD_QAM128:  NumBits=7; break;
	case DTA1XX_MOD_QAM256:  NumBits=8; break;
	default:				 NumBits=-1; break;		// Unsupported modulation type
	}
	return NumBits;
}
