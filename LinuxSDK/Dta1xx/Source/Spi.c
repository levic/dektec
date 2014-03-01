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
		DTA1XX_LOG(KERN_INFO,"Dta1xxSpiGetTxRateBps: Invalid device state: external clock selected\n");
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
		DTA1XX_LOG(KERN_INFO,"Dta1xxSpiGetTxRateBps: TxRateBps %d\n",*TxRateBps);
#endif
	return STATUS_SUCCESS;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiGetIoClksel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxSpiGetIoClksel(
	IN Channel*	 pChannel,
	IN Int* pClksel)
{
	if (pChannel->m_pSpiReg->m_Control.m_ExtClkSelect)
		*pClksel = DTA1XX_IOCLKSEL_EXTCLK;
	else
		*pClksel = DTA1XX_IOCLKSEL_INTCLK;
#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO,"Dta1xxSpiGetIoClksel: Clksel %d",(int)*pClksel);
#endif
	return STATUS_SUCCESS;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiGetIoMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS Dta1xxSpiGetIoMode(
	IN Channel*	 pChannel,
	OUT Int* pMode,
	OUT Int* pClkFreq)
{
	*pMode = pChannel->m_IoMode;
	// ClkFreq = round(ClkFreq/10)
#ifdef WINBUILD
	*pClkFreq = (Int)((((Int64)(pChannel->m_DssFreq)) + 5LL) / 10LL);
#else
	*pClkFreq = (Int)Dta1xxBinDiv(((Int64)(pChannel->m_DssFreq) + 5LL), 10LL, NULL);
#endif

#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO, "Dta1xxSpiGetIoMode: Mode %d, ClkFreq %d",
		*pMode, *pClkFreq);
#endif
	return STATUS_SUCCESS;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiGetIoStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxSpiGetIoStd(
	IN Channel*	 pChannel,
	IN Int* pStd)
{
	Dta1xxSpiGen*  pSpiReg; // Pointer to SPI general register block
	pSpiReg = pChannel->m_pSpiReg;
	if (pSpiReg->m_Control.m_IoEnable)
	{
		if (pSpiReg->m_Control.m_LvttlSelect)
			*pStd = DTA1XX_IOSTD_LVTTL;
		else
		{
			if (pSpiReg->m_Control.m_FailSafeEnable)
				*pStd = DTA1XX_IOSTD_LVDS2;
			else
				*pStd = DTA1XX_IOSTD_LVDS1;
		}
	}
	else
		*pStd = DTA1XX_IOSTD_DISABLED;
#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO, "Dta1xxSpiGetIoStd: Std %d", *pStd);
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
	if (pClkFreq>0) pClkFreq += 3;
#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO,"Dta1xxSpiGetRxClkFreq: ClkFreq %d",*pClkFreq);
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
	DTA1XX_LOG(KERN_INFO,"Dta1xxSpiInit");
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
	pChannel->m_IoMode = DTA1XX_IOMODE_STANDARD;
	pChannel->m_TxRate = 0;

	return STATUS_SUCCESS;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiSetIoClksel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxSpiSetIoClksel(
	IN Channel*	 pChannel,
	IN Int  Clksel)
{
	Dta1xxSpiGen*  pSpiReg; // Pointer to SPI general register block
#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO,"Dta1xxSpiSetIoClksel: Clksel %d",Clksel);
#endif
	// Get pointer to register map
	pSpiReg = pChannel->m_pSpiReg;
	switch (Clksel) 
	{
	case DTA1XX_IOCLKSEL_EXTCLK:
		pSpiReg->m_Control.m_ExtClkSelect = 1;
		// Turn off Internal clock
		SetIntClkFreq(pChannel,0);
		return STATUS_SUCCESS;
	case DTA1XX_IOCLKSEL_INTCLK:
		pSpiReg->m_Control.m_ExtClkSelect = 0;
		return STATUS_SUCCESS;
	}
	DTA1XX_LOG(KERN_INFO, "Dta1xxSpiSetIoClksel: invalid parameter Clksel %d", Clksel);
	return STATUS_INVALID_PARAMETER;

}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiSetIoMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS Dta1xxSpiSetIoMode(
	IN Channel*	 pChannel,
	IN Int  Mode,
	IN Int  ClkFreq)
{
	Dta1xxSpiGen*  pSpiReg; // Pointer to SPI general register block
	Int64 ClkFreqDhz;
	int TenBitMode;
	int SerialMode;
	NTSTATUS status;

#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO, "Dta1xxSpiSetIoMode: Mode %d, ClkFreq %d", Mode, ClkFreq);
#endif

	// Get pointer to register map
	pSpiReg = pChannel->m_pSpiReg;

	// Set defaults
	ClkFreqDhz = (Int64)ClkFreq * 10LL;
	TenBitMode = 0;
	SerialMode = 0;

	// Decode Mode parameter
	switch (Mode)
	{
	case DTA1XX_IOMODE_STANDARD:
		// I/O rate = Tx rate
		ClkFreqDhz = pChannel->m_TxRate;
		// limit values to min or max frequency
		if (ClkFreqDhz > 1980000000LL) ClkFreq = 1980000000LL;
		if (ClkFreqDhz < 0LL) ClkFreq = 0LL;
		break;
	case DTA1XX_IOMODE_SERIAL10B:
		TenBitMode = 1;
	case DTA1XX_IOMODE_SERIAL8B:
		SerialMode = 1;
	case DTA1XX_IOMODE_FIXEDCLK:
		break;
	default:
		DTA1XX_LOG(KERN_INFO,
			"Dta1xxSpiSetIoMode: invalid parameter Mode %d", Mode);
		return STATUS_INVALID_PARAMETER;
	}

	// Turn internal clock off when clock source is extern
	if (pSpiReg->m_Control.m_ExtClkSelect)
		ClkFreqDhz = 0;

	// Set mode
	status = SetIntClkFreq(pChannel,ClkFreqDhz);
	if ( status==STATUS_SUCCESS )
	{
		pSpiReg->m_Control.m_TestMode = 0;
		pSpiReg->m_Control.m_PrngMode = 0;
		pSpiReg->m_Control.m_SerialMode = SerialMode;
		pSpiReg->m_Control.m_10bMode = TenBitMode;
		pChannel->m_IoMode = Mode;
	}
	
	return status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiSetIoStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxSpiSetIoStd(
	IN Channel*	 pChannel,
	IN Int  Std)
{
	Dta1xxSpiGen*  pSpiReg; // Pointer to SPI general register block
#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO, "Dta1xxSpiSetIoStd: Std %d", Std);
#endif
	pSpiReg = pChannel->m_pSpiReg;
	switch (Std) {
		case DTA1XX_IOSTD_DISABLED:
			pSpiReg->m_Control.m_IoEnable			= 0;
			pSpiReg->m_Control.m_FailSafeEnable		= 1;
			pSpiReg->m_Control.m_LvttlSelect		= 0;
			return STATUS_SUCCESS;
		case DTA1XX_IOSTD_LVDS1:
			pSpiReg->m_Control.m_FailSafeEnable		= 0;
			pSpiReg->m_Control.m_LvttlSelect		= 0;
			pSpiReg->m_Control.m_IoEnable			= 1;
			return STATUS_SUCCESS;
		case DTA1XX_IOSTD_LVDS2:
			pSpiReg->m_Control.m_FailSafeEnable		= 1;
			pSpiReg->m_Control.m_LvttlSelect		= 0;
			pSpiReg->m_Control.m_IoEnable			= 1;
			return STATUS_SUCCESS;
		case DTA1XX_IOSTD_LVTTL:
			pSpiReg->m_Control.m_LvttlSelect		= 1;
			pSpiReg->m_Control.m_FailSafeEnable		= 1;
			pSpiReg->m_Control.m_IoEnable			= 1;
			return STATUS_SUCCESS;
	}
	DTA1XX_LOG(KERN_INFO, "Dta1xxSpiSetIoStd: invalid parameter Std %d",
		Std);
	return STATUS_INVALID_PARAMETER;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiSetRxModeDvb -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS Dta1xxSpiSetRxModeDvb(
	Channel*  pChannel,
	Int  RxMode)
{
#if LOG_LEVEL_SPI > 0
	DTA1XX_LOG(KERN_INFO, "Dta1xxSpiSetRxModeDvb: RxMode %d", RxMode);
#endif
	return STATUS_SUCCESS;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSpiSetSdiClock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
NTSTATUS  Dta1xxSpiSetSdiClock(Channel* pChannel) {
	if (pChannel->m_IoMode == DTA1XX_IOMODE_STANDARD)
		pChannel->m_TxRate = 270000000;
	return SetIntClkFreq(pChannel,(UInt64)pChannel->m_TxRate);
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
	DTA1XX_LOG(KERN_INFO, "Dta1xxSpiSetTxRateBps: TxRateBps %u",
		TxRateBps);
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
	if (  (pChannel->m_IoMode==DTA1XX_IOMODE_STANDARD) &&
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
	DTA1XX_LOG(KERN_INFO, "SetIntClkFreq: frequency %ld", (long int)frequency);
#endif

	// Check parameter
	if ((frequency < 0) || (frequency > 1890000000LL)) {
		DTA1XX_LOG(KERN_INFO,
			"SetIntClkFreq: invalid parameter frequency %ld", 
			(long int)frequency);
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
