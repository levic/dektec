//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Spi.c *#*#*#*#*#*#*#*#*#*# (C) 2008-2009 DekTec
//
// Low-level SPI routines:
//	- For I/O initialization and setting of DVB-SPI/BT-656 interface (DSS)
//	- For initialization and frequency setting of AD9952 (DSS)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	AT	2009.02.17	Created	

//	Contents:
//		local DVB-SPI/BT-656 general registers block type
//			Dta1xxSpiGen

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#ifdef WINBUILD
	#include "Dta1xx.h"
#else
	#include "../Include/Dta1xx.h"
	#include "../Include/Dta1xxRegs.h"
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- private methods -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
static UInt32	DssFrequency(Dta1xxSpiGen* pSpiGen, UInt32 frequency);
static void		DssInit(Dta1xxSpiGen* pSpiGen);
static void		DssUpdate(Dta1xxSpiGen* pSpiGen);
static void		DssWrite(Dta1xxSpiGen* pSpiGen, UInt8 data);
static NTSTATUS SetIntClkFreq(Channel* pChannel, Int64 frequency);



//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ public methods +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
NTSTATUS Dta1xxSpiGetTxRateBps(
	IN Channel*	 pChannel,
	OUT UInt32*  TxRateBps)
{
	Dta1xxSpiGen * pSpiReg;
	UInt64 tmp;

	pSpiReg = pChannel->m_pSpiReg;
	// return error when clock source is extern
	if (pSpiReg->m_Control.m_ExtClkSelect) {
#if LOG_LEVEL_SPI > 0
		DTA1XX_LOG(KERN_INFO,"[%d] Dta1xxSpiGetTxRateBps: Invalid device state: external clock selected", pChannel->m_PortIndex);
#endif
		return STATUS_INVALID_DEVICE_STATE;
	}

	// get I/O clock rate
	tmp = pChannel->m_DssFreq; // I/O clock in dHz

	if (pSpiReg->m_Control.m_SerialMode) {
		if (pSpiReg->m_Control.m_10bMode) {
			// 10 bit serial mode: bit rate is 8:10 times I/O clock rate
#ifdef WINBUILD
			tmp = ((tmp<<3)+50)/100;
#else
			tmp = Dta1xxBinDiv((tmp<<3)+50, 100, NULL);
#endif
		} else {
			// 8 bit serial mode: bit rate is I/O clock rate
#ifdef WINBUILD
			tmp = (tmp+5)/10;
#else
			tmp = Dta1xxBinDiv(tmp+5, 10, NULL);
#endif
		}
	} else {
		// parallel modi: bit rate is 8 times I/O clock rate
#ifdef WINBUILD
			tmp = ((tmp<<3)+5)/10;
#else
			tmp = Dta1xxBinDiv((tmp<<3)+5, 10, NULL);
#endif
	}

	*TxRateBps = (UInt32)tmp;
#if LOG_LEVEL_SPI > 0
		DTA1XX_LOG(KERN_INFO,"[%d] Dta1xxSpiGetTxRateBps: TxRateBps %d", pChannel->m_PortIndex, *TxRateBps);
#endif
	return STATUS_SUCCESS;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiGetSpiClksel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxSpiGetSpiClksel(
	IN Channel*	 pChannel,
	IN Int* pClksel)
{
	if (pChannel->m_pSpiReg->m_Control.m_ExtClkSelect)
		*pClksel = DTA1XX_SPICLKSEL_EXTCLK;
	else
		*pClksel = DTA1XX_SPICLKSEL_INTERNAL;
#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO,"[%d] Dta1xxSpiGetSpiClksel: Clksel %d", pChannel->m_PortIndex, (int)*pClksel);
#endif
	return STATUS_SUCCESS;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiGetSpiMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS Dta1xxSpiGetSpiMode(
	IN Channel*	 pChannel,
	OUT Int* pMode)
{
	*pMode = pChannel->m_SpiMode;
	
#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxSpiGetSpiMode: Mode %d",
		pChannel->m_PortIndex, *pMode);
#endif
	return STATUS_SUCCESS;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiGetSpiStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS Dta1xxSpiGetSpiStd(
	IN Channel*	 pChannel,
	IN Int* pStd)
{
	Dta1xxSpiGen*  pSpiReg; // Pointer to SPI general register block
	pSpiReg = pChannel->m_pSpiReg;
	if (pSpiReg->m_Control.m_IoEnable)
	{
		if (pSpiReg->m_Control.m_LvttlSelect)
			*pStd = DTA1XX_SPISTD_LVTTL;
		else
		{
			if (pSpiReg->m_Control.m_FailSafeEnable)
				*pStd = DTA1XX_SPISTD_LVDS2;
			else
				*pStd = DTA1XX_SPISTD_LVDS1;
		}
	} else 
        return STATUS_NOT_SUPPORTED;
	
#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxSpiGetSpiStd: Std %d", pChannel->m_PortIndex, *pStd);
#endif
	return STATUS_SUCCESS;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiGetRxClkFreq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxSpiGetRxClkFreq(
	IN Channel*	 pChannel,
	OUT Int* pClkFreq)
{
	*pClkFreq = pChannel->m_pSpiReg->m_SpiGenRxFrequency;
	// 3 counts are missing, so these must be added when a clock is present
	if (*pClkFreq>0) *pClkFreq += 3;
#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO,"[%d] Dta1xxSpiGetRxClkFreq: ClkFreq %d", pChannel->m_PortIndex, *pClkFreq);
#endif
	return STATUS_SUCCESS;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxSpiInit(
	IN	Channel*  pChannel)		  // Actual channel data
{
	Dta1xxSpiGen*  pSpiReg; // Pointer to SPI general register block

#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO,"[%d] Dta1xxSpiInit", pChannel->m_PortIndex);
#endif

	// Get pointer
	pSpiReg = pChannel->m_pSpiReg;

	// Reset I/O
	pSpiReg->m_Control.m_IoEnable		= 0;
	pSpiReg->m_Control.m_LvttlSelect	= 0;
	pSpiReg->m_Control.m_FailSafeEnable	= 0;
	pSpiReg->m_Control.m_SerialMode		= 0;
	pSpiReg->m_Control.m_ExtClkSelect	= 0;
	pSpiReg->m_Control.m_TestMode		= 0;
	pSpiReg->m_Control.m_PrngMode		= 0;
	pSpiReg->m_Status.m_IoError			= 1;
	pSpiReg->m_TestData.m_TestData		= 0;

	// Initialize DSS
	ExAcquireFastMutex(&pChannel->m_DssMutex);	// Protect access to DSS
	DssInit(pSpiReg);							// Initialize controls
	DssFrequency(pSpiReg,0),					// Set frequency 
	DssUpdate(pSpiReg);							// update DSS
	ExReleaseFastMutex(&pChannel->m_DssMutex);	// Release access to DSS

	// Update channel parameters
	pChannel->m_DssFreq = 0;
	pChannel->m_SpiMode = DTA1XX_SPIMODE_STANDARD;
	pChannel->m_TxRate = 0;

	return STATUS_SUCCESS;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiSetSpiClkSel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxSpiSetSpiClkSel(
	IN Channel*	 pChannel,
	IN Int  Clksel)
{
	Dta1xxSpiGen*  pSpiReg; // Pointer to SPI general register block
#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO,"[%d] Dta1xxSpiSetSpiClkSel: Clksel %d", pChannel->m_PortIndex, 
                                                                                  Clksel);
#endif
	// Get pointer to register map
	pSpiReg = pChannel->m_pSpiReg;
	switch (Clksel) 
	{
	case DTA1XX_SPICLKSEL_EXTCLK:
		pSpiReg->m_Control.m_ExtClkSelect = 1;
		// Turn off Internal clock
		SetIntClkFreq(pChannel,0);
		return STATUS_SUCCESS;
    case DTA1XX_SPICLKSEL_INTERNAL:
		pSpiReg->m_Control.m_ExtClkSelect = 0;
		return STATUS_SUCCESS;
	}
	DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxSpiSetSpiClkSel: invalid parameter Clksel %d",
                                                           pChannel->m_PortIndex, Clksel);
	return STATUS_INVALID_PARAMETER;

}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiSetSpiMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS Dta1xxSpiSetSpiMode(
	IN Channel*	 pChannel,
	IN Int  Mode)
{
	Dta1xxSpiGen*  pSpiReg; // Pointer to SPI general register block
	Int64 ClkFreqDhz;
	int TenBitMode;
	int SerialMode;
	NTSTATUS status = STATUS_SUCCESS;

#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxSpiSetIoMode: Mode %d", pChannel->m_PortIndex, Mode);
#endif

	// Get pointer to register map
	pSpiReg = pChannel->m_pSpiReg;

	// Set defaults
	TenBitMode = 0;
	SerialMode = 0;

	// Decode Mode parameter
	switch (Mode)
	{
	case DTA1XX_SPIMODE_STANDARD:
		break;
	case DTA1XX_SPIMODE_SERIAL10B:
		TenBitMode = 1;
	case DTA1XX_SPIMODE_SERIAL8B:
		SerialMode = 1;
	case DTA1XX_SPIMODE_FIXEDCLK:
		break;
	default:
		DTA1XX_LOG(KERN_INFO,
			"[%d] Dta1xxSpiSetIoMode: invalid parameter Mode %d", pChannel->m_PortIndex, Mode);
		return STATUS_INVALID_PARAMETER;
	}

	pSpiReg->m_Control.m_TestMode = 0;
	pSpiReg->m_Control.m_PrngMode = 0;
	pSpiReg->m_Control.m_SerialMode = SerialMode;
	pSpiReg->m_Control.m_10bMode = TenBitMode;
	pChannel->m_SpiMode = Mode;
	
	return status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiSetSpiStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS Dta1xxSpiSetSpiStd(
	IN Channel*	 pChannel,
	IN Int  Std)
{
	Dta1xxSpiGen*  pSpiReg; // Pointer to SPI general register block
#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxSpiSetIoStd: Std %d", pChannel->m_PortIndex, Std);
#endif
	pSpiReg = pChannel->m_pSpiReg;
	switch (Std) {
		case DTA1XX_SPISTD_LVDS1:
			pSpiReg->m_Control.m_FailSafeEnable		= 0;
			pSpiReg->m_Control.m_LvttlSelect		= 0;
			pSpiReg->m_Control.m_IoEnable			= 1;
			return STATUS_SUCCESS;
		case DTA1XX_SPISTD_LVDS2:
			pSpiReg->m_Control.m_FailSafeEnable		= 1;
			pSpiReg->m_Control.m_LvttlSelect		= 0;
			pSpiReg->m_Control.m_IoEnable			= 1;
			return STATUS_SUCCESS;
		case DTA1XX_SPISTD_LVTTL:
			pSpiReg->m_Control.m_LvttlSelect		= 1;
			pSpiReg->m_Control.m_FailSafeEnable		= 1;
			pSpiReg->m_Control.m_IoEnable			= 1;
			return STATUS_SUCCESS;
	}
	DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxSpiSetSpiStd: invalid parameter Std %d",
		pChannel->m_PortIndex, Std);
	return STATUS_INVALID_PARAMETER;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiSetRxModeDvb -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxSpiSetRxModeDvb(
	Channel*  pChannel,
	Int  RxMode)
{
#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxSpiSetRxModeDvb: RxMode %d", pChannel->m_PortIndex, RxMode);
#endif
	return STATUS_SUCCESS;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiSetSdiClock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
NTSTATUS  Dta1xxSpiSetSdiClock(Channel* pChannel) {
	if (pChannel->m_SpiMode == DTA1XX_SPIMODE_STANDARD)
		pChannel->m_TxRate = 270000000;
	return SetIntClkFreq(pChannel,(UInt64)pChannel->m_TxRate);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiSetSpiClock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
NTSTATUS  Dta1xxSpiSetSpiClock(Channel* pChannel, Int SpiClk) {
	return SetIntClkFreq(pChannel,(UInt64)SpiClk*10);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiSetTxRateBps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxSpiSetTxRateBps(
	IN Channel*  pChannel,
	IN UInt32 TxRateBps)
{
	NTSTATUS status;
	UInt64 TxRate;
#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxSpiSetTxRateBps: TxRateBps %u",
		pChannel->m_PortIndex, TxRateBps);
#endif

	// Update Tx channel rate
	// Convert pPhIncr and pPhMod to word rate in dHz = round(W/s*10) = INT(TxRateBps/8*10*4+2)/4
	TxRate = ((UInt64)TxRateBps) ;
	TxRate *= 5LL; // * 1/8*10*4
	TxRate += 2;  // + 2
	TxRate >>= 2; // / 4
	if (TxRate > 0xFFFFFFFFLL) return STATUS_INVALID_PARAMETER;
	pChannel->m_TxRate = (UInt32)TxRate;
	
	// Update I/O clock frequency
	status = STATUS_SUCCESS;
	if (  (pChannel->m_SpiMode==DTA1XX_SPIMODE_STANDARD) &&
	     !(pChannel->m_pSpiReg->m_Control.m_ExtClkSelect) ) 
	{
		status = SetIntClkFreq(pChannel,(UInt64)pChannel->m_TxRate);
	}

	return status;

} // Dta1xxSpiSetTxRateBps()



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ private DSS methods +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// device				   Analog Devices AD9952
// REFCLK				   27 MHz
// multiplication		   14x
// SYSCLK				   378 MHz
// VCO range			   250 to 400 Mhz
// SYNC_CLK output		   disabled
// serial interface		   MSB first, 3-wire
// frequency tuning word   round( frequency / 378 MHz * 2^32)


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DssFrequency -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static UInt32 DssFrequency(
	IN Dta1xxSpiGen* pSpiGen,
	IN UInt32  frequency) // unit: dHz (10th of 1 Hz)
{
	UInt64 ftw0;
#if LOG_LEVEL_SPI > 2
	DTA1XX_LOG(KERN_INFO, "DssFrequency: frequency %u", frequency);
#endif
	ftw0   = (UInt64)frequency;				// frequency
	ftw0 <<= 33;							// multiply with 2^32 * 2
#ifdef WINBUILD
	ftw0  /= (UInt64)3780000000LL;			// devide by 10 * 378 MHz
#else
	ftw0 = Dta1xxBinUDiv(ftw0, 3780000000LL, NULL);
#endif
	ftw0  += (UInt64)1LL;				// add +1 for rounding
	ftw0 >>= 1		;						// devide by 2
	DssWrite(pSpiGen,0x04	   );			// address FTW0 (Frequency Tuning Word 0)
	DssWrite(pSpiGen,(UInt8)(ftw0>>24));	// data bits 31:24
	DssWrite(pSpiGen,(UInt8)(ftw0>>16));	// data bits 23:16
	DssWrite(pSpiGen,(UInt8)(ftw0>> 8));	// data bits 15:8
	DssWrite(pSpiGen,(UInt8)ftw0);			// data bits 7:0
	ftw0  *= (UInt64)7560000000LL;			// multiply with 10 * 378 MHz * 2
	ftw0  += (UInt64)0x100000000LL;			// add 2^32 for rounding
	ftw0 >>= 33;							// devide by 2^32 * 2
	return (UInt32)ftw0;					// actual set frequency
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DssInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static void DssInit(
	IN Dta1xxSpiGen * pSpiGen)
{
#if LOG_LEVEL_SPI > 2
	DTA1XX_LOG(KERN_INFO, "DssInit");
#endif
	DssWrite(pSpiGen,0x00); // address CFR1 (Control Function Register 1)
	DssWrite(pSpiGen,0x00); // data bits 31:24
	DssWrite(pSpiGen,0x00); // data bits 23:16
	DssWrite(pSpiGen,0x02); // data bits 15:8
	DssWrite(pSpiGen,0x02); // data bits 7:0
	DssWrite(pSpiGen,0x01); // address CFR2 (Control Function Register 2)
	DssWrite(pSpiGen,0x00); // data bits 23:16
	DssWrite(pSpiGen,0x00); // data bits 15:8
	DssWrite(pSpiGen,0x74); // data bits	7:0
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DssUpdate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void DssUpdate(
	IN Dta1xxSpiGen * pSpiGen)
{
#if LOG_LEVEL_SPI > 2
	DTA1XX_LOG(KERN_INFO, "DssUpdate");
#endif
	while (pSpiGen->m_Status.m_DssBusy);
	pSpiGen->m_Control.m_DssIoUpdate = 1;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DssWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void DssWrite(
	IN Dta1xxSpiGen* pSpiGen,
	IN	UInt8  Data)
{ 
#if LOG_LEVEL_SPI > 2
	DTA1XX_LOG(KERN_INFO, "DssWrite Data 0x%02X",(UInt)Data);
#endif
	while (pSpiGen->m_Status.m_DssBusy);
	pSpiGen->m_DssData.m_DssData = Data;
}



//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ other private methods +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SetIntClkFreq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static NTSTATUS SetIntClkFreq(
	IN Channel*  pChannel,
	IN Int64  frequency) // dHz
{
	Dta1xxSpiGen*  pSpiReg; // Pointer to SPI general register block
#if LOG_LEVEL_SPI > 1
	DTA1XX_LOG(KERN_INFO, "[%d] SetIntClkFreq: frequency %ld", pChannel->m_PortIndex, (long int)frequency);
#endif

	// Check parameter
	if ((frequency < 0) || (frequency > 1890000000LL)) {
		DTA1XX_LOG(KERN_INFO,
			"[%d] SetIntClkFreq: invalid parameter frequency %ld", 
			pChannel->m_PortIndex, (long int)frequency);
		return STATUS_INVALID_PARAMETER;
	}
	
	// Get pointer
	pSpiReg = pChannel->m_pSpiReg;

	// Set DSS
	ExAcquireFastMutex(&pChannel->m_DssMutex);	// Protect access to DSS
	pChannel->m_DssFreq = DssFrequency(pSpiReg,(UInt32)frequency); // Set frequency
	DssUpdate(pSpiReg);							// update DSS
	ExReleaseFastMutex(&pChannel->m_DssMutex);	// Release access to DSS

	return STATUS_SUCCESS;

} // SetIntClkFreq()
