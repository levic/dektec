//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Rate.c *#*#*#*#*#*#*#*#*#* (C) 2000-2007 DekTec

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
#include "../Include/Ad9789.h"

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxGetFreqEst -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get frequency estimate of external clock-input (DTA1XX_TS_TX_CHANNEL: DTA-102), or
// Transport-Stream input rate (DTA1XX_TS_RX_CHANNEL: DTA-120, DTA-122, DTA-140, DTA-160/2160)
//
Int  Dta1xxGetFreqEst(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int	 ChannelType,		// Channel type
	Int  PortIndex,			// Port index
	Int*  pFreqEst)			// Frequency estimate
{
 	Int64  Count, ClockFreq;
	Int PerIntFact = 1;

	// Do no crash on erroneous port index
	if (PortIndex<0 || PortIndex>=pFdo->m_NumNonIpChannels) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxGetFreqEst: PortIndex=%d INVALID", PortIndex );
#endif
		return -EFAULT;
	}

	// Read Valid Counter from the right channel
	if (ChannelType==DTA1XX_TS_TX_CHANNEL || ChannelType==DTA1XX_TS_RX_CHANNEL) {
		Count = pFdo->m_Channel[PortIndex].m_BrMeas.m_ValidCount256;
	} else {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxGetFreqEst: ChannelType=%d ILLEGAL", ChannelType );
#endif
		return -EFAULT;
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Determine Clock Frequency -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	// Clock frequency depends on board type.
	// For old DTA-120s, the frequency depends the firmware revision:
	//	- If last bit is '1', it's a "Cypress" DTA-120 with 27-MHz crystal and *1.5 PLL
	//	- If last bit is '0', it's a "Gennum" DTA-120 with 40-MHz crystal
	if (pFdo->m_TypeNumber==120 && pFdo->m_FirmwareVersion<4) {
		if ((pFdo->m_FirmwareVersion & 1) != 0)
			ClockFreq = 40500000;
		else
			ClockFreq = 40000000;
	}
	else if (   pFdo->m_TypeNumber==120
			|| (pFdo->m_TypeNumber==124 && pFdo->m_FirmwareVersion==0)
			||  pFdo->m_TypeNumber==140)
		ClockFreq = 40500000;
	else if (  pFdo->m_TypeNumber == 160
			|| pFdo->m_TypeNumber == 2160) {
		ClockFreq = DTA160_REF_CLOCK*1000;
		PerIntFact = 2;		// Periodic interrupt 2 times faster then normal
	}
	else if (   pFdo->m_TypeNumber==105 || pFdo->m_TypeNumber==145
             || pFdo->m_TypeNumber==111 
             || pFdo->m_TypeNumber==112 || pFdo->m_TypeNumber==115
             || pFdo->m_TypeNumber==116 || pFdo->m_TypeNumber==117
             || pFdo->m_TypeNumber==124
             || pFdo->m_TypeNumber==2135 || pFdo->m_TypeNumber==2136
             || pFdo->m_TypeNumber==2137 || pFdo->m_TypeNumber==2139
             || pFdo->m_TypeNumber==2142 || pFdo->m_TypeNumber==2144
             || pFdo->m_TypeNumber==2145 || pFdo->m_TypeNumber==2148
             || pFdo->m_TypeNumber==545)
		ClockFreq = 54000000;
	else
		ClockFreq = 27000000;

	// Compute frequency estimate
	*pFreqEst = (Int) ((Count * ClockFreq * PerIntFact) >> 26);

#if LOG_LEVEL>1
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxGetFreqEst: FreqEst=%d", PortIndex, *pFreqEst );
#endif

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxGetRxClkFreq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Get a receive channel's clock frequency.
//
Int Dta1xxGetRxClkFreq(DTA1XX_FDO* pFdo, DTA1XX_PORT_INTPAR* pPortIntPar)
{
	Int  PortIndex;
	Channel*  pChannel;

	// Check port index and get channel pointer
	PortIndex = pPortIntPar->m_PortIndex;
	if ((PortIndex < 0) || (PortIndex >= pFdo->m_NumNonIpChannels)) {
		return -EINVAL;
	}
	pChannel = &pFdo->m_Channel[PortIndex];

	// SPI capable channels/BT.656 support reading of Rx clock frequency
	if (pChannel->m_Capability&DTA1XX_CHCAP_SPI) {
		return Dta1xxSpiGetRxClkFreq(pChannel, &pPortIntPar->m_Parameter);
	}
	return -EFAULT;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxGetTxPhaseIncr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get a transmit channel's I/O clock to TS rate phase increment value
//
NTSTATUS Dta1xxGetTxPhaseIncr(
	DTA1XX_FDO* pFdo,
	DTA1XX_PORT_2INTPAR* pPortIntPar)
{
	Int  PortIndex;
	Channel*  pChannel;
	Dta1xxTx*  pDta1xxTx;

	// Check port index and channel pointer
	PortIndex = pPortIntPar->m_PortIndex;
	if ((PortIndex < 0) || (PortIndex >= pFdo->m_NumNonIpChannels)) {
		return -EINVAL;
	}
	pChannel = &pFdo->m_Channel[PortIndex];
	pDta1xxTx = (Dta1xxTx*)(pChannel->m_pRegBase);
	pPortIntPar->m_Parameter[0] = pDta1xxTx->m_TxClock;
	if (pChannel->m_Capability&DTA1XX_CHCAP_TSRATESEL) {
		pPortIntPar->m_Parameter[1] = pDta1xxTx->m_TxClockMod;
	} else {
		pPortIntPar->m_Parameter[1] = 0;
	}
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxGetTxRateSel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Get a transmit channel's TS rate clock selection
//
Int  Dta1xxGetTxRateSel(
	Channel*  pChannel,
	Int*  pRateSel)
{
	*pRateSel = DTA1XX_TSRATESEL_INTERNAL;
	// If channel has rate selection capability
	if (0!=(pChannel->m_Capability&DTA1XX_CHCAP_TSRATESEL)) 
	{
        *pRateSel = pChannel->m_TxRateSel;
		return 0;
	}
	return -EFAULT;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSetTxPhaseIncr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set a transmit channel's I/O clock to TS rate phase increment value and modulus
//
NTSTATUS Dta1xxSetTxPhaseIncr(
	DTA1XX_FDO* pFdo,
	DTA1XX_PORT_2INTPAR* pPortIntPar)
{
	Int  PortIndex;
	Channel*  pChannel;
	Dta1xxTx*  pDta1xxTx;

	PortIndex = pPortIntPar->m_PortIndex;
	if ((PortIndex < 0) || (PortIndex >= pFdo->m_NumNonIpChannels)) {
		return -EINVAL;
	}
	pChannel = &pFdo->m_Channel[PortIndex];
	if (!(pChannel->m_Capability&DTA1XX_CHCAP_PQNCO)) {
		return -EFAULT;
	}
	pDta1xxTx = (Dta1xxTx*)(pChannel->m_pRegBase);
	pDta1xxTx->m_TxClock = pPortIntPar->m_Parameter[0];
	pDta1xxTx->m_TxClockMod = pPortIntPar->m_Parameter[1];
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSetTxRateSel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Set a transmit channel's TS rate clock selection
//
Int  Dta1xxSetTxRateSel(
	Channel*  pChannel,
	Int  RateSel)
{
	// If channel has rate selection capability
	if (0!=(pChannel->m_Capability&DTA1XX_CHCAP_TSRATESEL)) {
		switch (RateSel) 
		{
			case DTA1XX_TSRATESEL_INTERNAL:
				Dta1xxTxCtrl2RegSetRateSelInt(pChannel->m_pRegBase);
				break;
			case DTA1XX_TSRATESEL_EXTCLK:
                Dta1xxTxCtrl2RegSetRateSelExt(pChannel->m_pRegBase);
                if ((pChannel->m_Capability&DTA1XX_CHCAP_PQNCO) != 0)
                {
                    Dta1xxTx*  pDta1xxTx= (Dta1xxTx*)(pChannel->m_pRegBase);
                    // Set 1:1 ratio
                    pDta1xxTx->m_TxClockMod = 0x80000000;
                    pDta1xxTx->m_TxClock = 0x80000000;
                }
                break;
            case DTA1XX_TSRATESEL_EXTRATIO:
             	Dta1xxTxCtrl2RegSetRateSelExt(pChannel->m_pRegBase);
				break;
			default:
				return -EINVAL;
		}
        pChannel->m_TxRateSel = RateSel;
		return 0;
	}
	return -EFAULT;
}

// Legacy
//NTSTATUS  Dta1xxTxGetTsRate

// Legacy
//NTSTATUS  Dta1xxTxSetTsRate

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxTxGetRate2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get transmit channel's TS-, symbol- or sample rate from the hardware.
//
Int  Dta1xxTxGetRate2(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  PortIndex,			// Port index
	Int*  pClockGenMode,	// Clock-generator mode
	Int*  pTsSymOrSampRate)	// Retrieved TS- or symbol- or sample rate
{
	Int  Multiplier;				// Symbol-rate multiplier
	Int  PckSize;					// Packet size: 188 / 204
	Int  TxMode;					// Packet transmit mode
	Int64  TsSymOrSampRate;			// TS-, symbol- or sample rate
	Int64  dPhi;					// Phase increment as UInt32
	Channel*  pCh;					// Channel
	volatile UInt8*  pTxRegs;				// Pointer to transmit register block

	// Check port index
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumNonIpChannels ) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxTxGetRate2: PortIndex=%d INVALID", PortIndex );
#endif
		return -EFAULT;
	}

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// Get pointer to transmit registers
	pTxRegs = pCh->m_pRegBase;

	// DTA-102 only: get clock-generator mode
	if (pCh->m_pFdo->m_TypeNumber == 102)
		*pClockGenMode = Dta1xxTxCtrlRegGetUseExtClk(pTxRegs);
	else
		*pClockGenMode = 0; // = DTAPI_TXCLOCK_INTERNAL

	// Get NCO phase increment from hardware
	dPhi = (Int64)Dta1xxTxGetTxClock(pTxRegs);

	// Special processing for modulator cards
	if ( pCh->m_pFdo->m_TypeNumber == 107 )
	{
		// Get symbol-rate multiplier
		Multiplier = Dta1xxTxModcRegGetM(pTxRegs);
		// Compute symbol rate, multiplied by 16 (for rounding)
		// Be careful with 1<<(25+M): cast 1 to 64-bit first!
		TsSymOrSampRate = Dta1xxBinDiv( (dPhi * (Int64)25000000),
										((Int64)1 << (25+Multiplier)), NULL);
		*pTsSymOrSampRate = (Int)((TsSymOrSampRate + 8) >> 4);
#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetRate2: TsSymOrSampRate=%d M=%d dPhi=0x%08x",
				    pCh->m_PortIndex, *pTsSymOrSampRate, Multiplier, (Int)dPhi );
#endif
	}
	else if (   pCh->m_pFdo->m_TypeNumber==110
			|| (pCh->m_pFdo->m_TypeNumber==111 && PortIndex==0)
			|| (pCh->m_pFdo->m_TypeNumber==112 && PortIndex==1)
			|| (pCh->m_pFdo->m_TypeNumber==115 && PortIndex==1)
			|| (pCh->m_pFdo->m_TypeNumber==116 && PortIndex==1)
			|| (pCh->m_pFdo->m_TypeNumber==117 && PortIndex==1) ) {
		// Compute symbol rate, multiplied by 16 (for rounding)
		if (   pCh->m_pFdo->m_TypeNumber==111 || pCh->m_pFdo->m_TypeNumber==112
			|| pCh->m_pFdo->m_TypeNumber==115 || pCh->m_pFdo->m_TypeNumber==116 )
		{
			// Check for Direct-IQ modes (use elementary clock rate computation)
			if (   pCh->m_ModType==DTA1XX_MOD_ATSC
				|| pCh->m_ModType==DTA1XX_MOD_ADTBT
				|| pCh->m_ModType==DTA1XX_MOD_DMBTH
				|| pCh->m_ModType==DTA1XX_MOD_DVBT
				|| pCh->m_ModType==DTA1XX_MOD_ISDBT)
				TsSymOrSampRate = Dta1xxBinDiv((dPhi*18000000), ((Int64)1 << 28), NULL);
			else
				TsSymOrSampRate = Dta1xxBinDiv((dPhi*9000000 ), ((Int64)1 << 28), NULL);
		}
		else if ( pCh->m_pFdo->m_TypeNumber==117 )
		{
			// Check for Direct-IQ modes (use elementary clock rate computation)
			if (   pCh->m_ModType==DTA1XX_MOD_ATSC
				|| pCh->m_ModType==DTA1XX_MOD_ADTBT
				|| pCh->m_ModType==DTA1XX_MOD_DMBTH
				|| pCh->m_ModType==DTA1XX_MOD_DVBT
				|| pCh->m_ModType==DTA1XX_MOD_ISDBT)
				TsSymOrSampRate = Dta1xxBinDiv((dPhi*22000000), ((Int64)1 << 28), NULL);
			else
				TsSymOrSampRate = Dta1xxBinDiv((dPhi*11000000 ), ((Int64)1 << 28), NULL);
		}
		else
		{
			// Be careful with 1<<(25+M): cast 1 to 64-bit first!
			TsSymOrSampRate = Dta1xxBinDiv( (dPhi*25000000+12500000),
										    ((Int64)1 << 27), NULL );
		}
		*pTsSymOrSampRate = (Int)((TsSymOrSampRate + 8) >> 4);		// Rounding
#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetRate2: TsSymOrSampRate=%d dPhi=0x%08x",
					pCh->m_PortIndex, *pTsSymOrSampRate, (Int)dPhi );
#endif
	} else if ((pCh->m_Capability&DTA1XX_CHCAP_AD9789) != 0) {

		// return saved sample rate
		TsSymOrSampRate = pCh->m_ModSampRate;

	}
	else {		// *** Not a modulator card

		// Determine current packet size
		TxMode = Dta1xxTxCtrlRegGetTxMode(pTxRegs);
		PckSize = Dta1xxTxMode2PacketSize(TxMode);

		// Compute transport-stream rate, depends on phase increment and packet size
		if (PckSize == 188) {
			*pTsSymOrSampRate = 
						(Int)((Dta1xxBinDiv((dPhi*421875*2), 8388608, NULL)+1)>>1);
		} else if (PckSize == 192) {
			*pTsSymOrSampRate = 
					(Int)((Dta1xxBinDiv((dPhi*19828125*2), 402653184, NULL)+1)>>1);
		} else {	// nPckSize == 204
			*pTsSymOrSampRate = 
					(Int)((Dta1xxBinDiv((dPhi*6609375*2), 142606336, NULL)+1)>>1);
		}
	

#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetRate2: ClockGenMode=%d TsRate=%d",
				    pCh->m_PortIndex, *pClockGenMode, *pTsSymOrSampRate );
#endif
	}
	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxTxSetRate2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set a transmit channel's TS-, symbol- or sample rate.
//
Int  Dta1xxTxSetRate2(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  PortIndex,			// Port index
	Int  ClockGenMode,		// Clock-generator mode
	Int  TsSymOrSampRateAsInt)	// TS-, Symbol or Sample rate
{
	Int  PckSize;					// Packet size: 188 / 204
	Int  TxMode;					// Packet transmit mode
	Int  Multiplier;				// Symbol-rate multiplier
//	Int64  RateBase;				// Intermediate variable in bit-rate computation
	Int64  TsSymOrSampRate = TsSymOrSampRateAsInt;
									// TS-, symbol- or sample rate as 64-bit integer
	UInt32  dPhi;					// Phase increment for NCO
	UInt32  dPhm;					// Phase modulus for NCO
	Channel*  pCh;					// Channel
	volatile UInt8*  pTxRegs;				// Pointer to transmit register block
	Int status = STATUS_SUCCESS;

	dPhi = 0;
	dPhm = 0;

	// Check port index
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumNonIpChannels ) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxTxSetRate2: PortIndex=%d INVALID", PortIndex );
#endif
		return -EFAULT;
	}

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// Store rate in TsTx structure, so that rate remains known when another
	// parameter influencing the bit rate (like packet size) is changed
	pCh->m_TsSymOrSampRate = TsSymOrSampRateAsInt;



	// Get pointer to transmit registers
	pTxRegs = pCh->m_pRegBase;

	// Determine current packet size
	TxMode = Dta1xxTxCtrlRegGetTxMode(pTxRegs);
	PckSize = Dta1xxTxMode2PacketSize(TxMode);

	// Special processing for modulator cards
	if (pCh->m_pFdo->m_TypeNumber == 107) {

		// Compute multiplier M
		if      (TsSymOrSampRate <= (Int64)175783)		Multiplier = 8;
		else if (TsSymOrSampRate <= (Int64)351566)		Multiplier = 7;
		else if (TsSymOrSampRate <= (Int64)703133)		Multiplier = 6;
		else if (TsSymOrSampRate <= (Int64)1406250)		Multiplier = 5;
		else if (TsSymOrSampRate <= (Int64)2812500)		Multiplier = 4;
		else if (TsSymOrSampRate <= (Int64)5625000)		Multiplier = 3;
		else if (TsSymOrSampRate <= (Int64)11250000)	Multiplier = 2;
		else if (TsSymOrSampRate <= (Int64)22500000)	Multiplier = 1;
		else											Multiplier = 0;

		// Compute dPhi (verified that this cannot overflow)
		dPhi = (UInt32)Dta1xxBinDiv((TsSymOrSampRate << (Multiplier+23)), 390625, NULL);

#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetRate2: TsSymOrSampRate=%d M=%d dPhi=0x%08x",
				    pCh->m_PortIndex, (Int)TsSymOrSampRate, Multiplier, dPhi );
#endif

		// Set multiplier
		Dta1xxTxModcRegSetM(pTxRegs, Multiplier);
	}
	else if (   pCh->m_pFdo->m_TypeNumber==110
			|| (pCh->m_pFdo->m_TypeNumber==111 && PortIndex==0)
			|| (pCh->m_pFdo->m_TypeNumber==112 && PortIndex==1)
			|| (pCh->m_pFdo->m_TypeNumber==115 && PortIndex==1)
			|| (pCh->m_pFdo->m_TypeNumber==116 && PortIndex==1)
			|| (pCh->m_pFdo->m_TypeNumber==117 && PortIndex==1) ) {

		// Save sample rate for FIFO-load computations (Direct IQ modes)
		pCh->m_ModSampRate = (Int)TsSymOrSampRate;

		// Compute dPhi
		if (   pCh->m_pFdo->m_TypeNumber==111 || pCh->m_pFdo->m_TypeNumber==112
			|| pCh->m_pFdo->m_TypeNumber==115 || pCh->m_pFdo->m_TypeNumber==116 ) {
			// Check for Direct-IQ modes (use elementary clock rate computation)
			if (   pCh->m_ModType==DTA1XX_MOD_ATSC
				|| pCh->m_ModType==DTA1XX_MOD_ADTBT
				|| pCh->m_ModType==DTA1XX_MOD_DMBTH
				|| pCh->m_ModType==DTA1XX_MOD_DVBT
				|| pCh->m_ModType==DTA1XX_MOD_ISDBT)
				dPhi = (UInt32)Dta1xxBinDiv((TsSymOrSampRate*((Int64)1<<32)), 18000000, NULL);
			else
				dPhi = (UInt32)Dta1xxBinDiv((TsSymOrSampRate*((Int64)1<<32)), 9000000, NULL);
		}
		else if ( pCh->m_pFdo->m_TypeNumber==117 )
		{
			// Check for Direct-IQ modes (use elementary clock rate computation)
			if (   pCh->m_ModType==DTA1XX_MOD_ATSC
				|| pCh->m_ModType==DTA1XX_MOD_ADTBT
				|| pCh->m_ModType==DTA1XX_MOD_DMBTH
				|| pCh->m_ModType==DTA1XX_MOD_DVBT
				|| pCh->m_ModType==DTA1XX_MOD_ISDBT)
				dPhi = (UInt32)Dta1xxBinDiv((TsSymOrSampRate*((Int64)1<<32)), 22000000, NULL);
			else
				dPhi = (UInt32)Dta1xxBinDiv((TsSymOrSampRate*((Int64)1<<32)), 11000000, NULL);
		}
		else
			dPhi = (UInt32)Dta1xxBinDiv((TsSymOrSampRate << 31), 25000000, NULL);

#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetRate2: TsSymOrSampRate=%d "
				    "ModType=%d dPhi=0x%08x",
				    pCh->m_PortIndex, (Int)TsSymOrSampRate, pCh->m_ModType, dPhi );
#endif
	} else if ((pCh->m_Capability&DTA1XX_CHCAP_AD9789) != 0) {

		// Save sample rate for FIFO-load computations (Direct IQ modes)
		pCh->m_ModSampRate = (Int)TsSymOrSampRate;

	} else {	// *** Not a modulator card

		// Set phase-increment register, depends on transport-stream rate and packet size
		if (PckSize == 188) {
			// p / q = 2^32 / 216.000.000
			// p and q both divided by 512
			dPhi = (UInt32)( (Dta1xxBinDiv(
						(TsSymOrSampRate*8388608*2 ), 421875, NULL) + 1) >>1 );
		} else if (PckSize == 192) {
			dPhi = (UInt32)( (Dta1xxBinDiv(
						 (TsSymOrSampRate*402653184*2), 19828125, NULL) +1 ) >> 1 );
		} else {	// nPckSize == 204
			dPhi = (UInt32)( (Dta1xxBinDiv(
						  (TsSymOrSampRate*142606336*2), 6609375, NULL) +1 ) >> 1 );
		}

#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetRate2: ClockGenMode=%d TsOrSymRate=%d",
				    pCh->m_PortIndex, ClockGenMode, TsSymOrSampRateAsInt );
#endif
		status = STATUS_SUCCESS;

		if (pCh->m_Capability&DTA1XX_CHCAP_TSRATESEL) {
			if (pCh->m_Capability&DTA1XX_CHCAP_SPI) {
				Int Mode = DTA1XX_SPIMODE_STANDARD;
				UInt64 TxRateBps = Dta1xxBinDiv((TsSymOrSampRate * PckSize) + 94, 188, NULL);
				if (TxRateBps>1000000000) TxRateBps=1000000000; // Limit to 1Gb/s (125 MHz)
				status = Dta1xxSpiGetSpiMode(pCh,&Mode);
				if (Mode==DTA1XX_SPIMODE_STANDARD) {
					status = Dta1xxSpiSetTxRateBps(pCh,(UInt32)TxRateBps);
					// dPhm = dPhi = 0 will generate 0 bps, because the NCO will not increment
					dPhi = 1;       // SPI: I/O clock to symbol ratio 1:1
					dPhm = 1; 				                  
				} else {
					// SPI: set clock to symbol ratio to <wanted rate>:<I/O clock rate>
					dPhi = (UInt32)TxRateBps; // wanted rate
					dPhm = dPhi;
					status = Dta1xxSpiGetTxRateBps(pCh,&dPhm); // I/O clock rate
				}
				// ensure the increment is not higher then the modulus
				if (dPhi>dPhm) dPhi=dPhm;
				// ensure dPhm is high, otherwise the NCO may take a long time to stabilize
				while ((dPhm&0x80000000)==0) {
					dPhi <<= 1;
					dPhm <<= 1;
				}
			} else {
				dPhm = 0;          // ASI: 27 MHz to symbol ratio dPhi : 2^32
			}
			Dta1xxTxSetTxClockMod(pTxRegs, dPhm);
		}
	}

	// Set phase increment for NCO
	Dta1xxTxSetTxClock(pTxRegs, dPhi);

	// DTA-102 only: set clock-generator mode
	if (pCh->m_pFdo->m_TypeNumber == 102)
		Dta1xxTxCtrlRegSetUseExtClk(pTxRegs, ClockGenMode);

	return status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxTxSetRate3 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set a modulator's symbol- or sample rate.
// This is the successor of Dta1xxTxSetRate2, for modulators only.
// The intention is that this routine does not contain any computation based on the
// modulation parameters, so that:
//  - All modulation-specific logic is in DTAPI
//  - New modes can be added without having to update the driver
//
Int  Dta1xxTxSetRate3(
    DTA1XX_FDO*  pFdo,         // Functional device object, representing the DTA-1xx card
    Int  PortIndex,            // Port index
    Int  ClockGenMode,         // Clock-generator mode
    Int  SymOrSampRateAsInt)   // Symbol or Sample rate (*2 for HW modulation)
{
    Int  Multiplier;                // Symbol-rate multiplier
    //Int64  RateBase;                // Intermediate variable in bit-rate computation
    Int64  SymOrSampRate = SymOrSampRateAsInt;
                                    // Symbol- or sample rate as 64-bit integer
    UInt32  dPhi = 0;               // Phase increment for NCO
    Channel*  pCh;                  // Channel
    Dta1xxTx*  pDta1xxTx;           // Pointer to transmit register block
    Int  Status = STATUS_SUCCESS;

    // Check port index
    if (PortIndex >= pFdo->m_NumNonIpChannels) {
        DTA1XX_LOG(KERN_INFO, "Dta1xxTxSetRate3: PortIndex=%d INVALID", PortIndex);
        return STATUS_UNSUCCESSFUL;
    }

    // Set channel pointer
    pCh = &pFdo->m_Channel[PortIndex];

    // Get pointer to transmit registers
    pDta1xxTx = (Dta1xxTx*)(pCh->m_pRegBase);

    // Processing depends on modulator card
    if (pCh->m_pFdo->m_TypeNumber == 107)
    {
        // Compute multiplier M
        if      (SymOrSampRate <= (Int64)175783)   Multiplier = 8; 
        else if (SymOrSampRate <= (Int64)351566)   Multiplier = 7; 
        else if (SymOrSampRate <= (Int64)703133)   Multiplier = 6; 
        else if (SymOrSampRate <= (Int64)1406250)  Multiplier = 5; 
        else if (SymOrSampRate <= (Int64)2812500)  Multiplier = 4; 
        else if (SymOrSampRate <= (Int64)5625000)  Multiplier = 3; 
        else if (SymOrSampRate <= (Int64)11250000) Multiplier = 2;
        else if (SymOrSampRate <= (Int64)22500000) Multiplier = 1;
        else                                       Multiplier = 0;

        // Compute dPhi (verified that this cannot overflow)
        dPhi = (UInt32) (Dta1xxBinDiv((SymOrSampRate << (Multiplier+23)), 390625, NULL));

#if LOG_LEVEL_MODULATION > 0
        DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxTxSetRate3: SymOrSampRate=%d M=%d dPhi=0x%08x",
                                 pCh->m_PortIndex, (Int)SymOrSampRate, Multiplier, dPhi);
#endif
        // Set multiplier
        Dta1xxTxModcRegSetM((UInt8*)pDta1xxTx, Multiplier);
    }
    else if (pCh->m_pFdo->m_TypeNumber==110
                                  || (pCh->m_pFdo->m_TypeNumber==111 && PortIndex==0)
                                  || (pCh->m_pFdo->m_TypeNumber==112 && PortIndex==1)
                                  || (pCh->m_pFdo->m_TypeNumber==115 && PortIndex==1)
                                  || (pCh->m_pFdo->m_TypeNumber==116 && PortIndex==1)
                                  || (pCh->m_pFdo->m_TypeNumber==117 && PortIndex==1) )
    {
        // Save sample rate for FIFO-load computations (Direct IQ modes)
        pCh->m_ModSampRate = (Int)SymOrSampRate;

        // Compute dPhi
        if (pCh->m_pFdo->m_TypeNumber==111 || pCh->m_pFdo->m_TypeNumber==112
                || pCh->m_pFdo->m_TypeNumber==115 || pCh->m_pFdo->m_TypeNumber==116)
            dPhi = (UInt32)(Dta1xxBinDiv(SymOrSampRate * ((Int64)1<<32), (Int64)18000000, NULL));
        else if (pCh->m_pFdo->m_TypeNumber==117)
            dPhi = (UInt32)(Dta1xxBinDiv(SymOrSampRate * ((Int64)1<<32), (Int64)22000000, NULL));
        else
            dPhi = (UInt32) (Dta1xxBinDiv((SymOrSampRate << 31), 25000000, NULL));

#if LOG_LEVEL_MODULATION > 0
        DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxTxSetRate3: SymOrSampRate=%d ModType=%d"
              " dPhi=0x%08x", pCh->m_PortIndex, (Int)SymOrSampRate, pCh->m_ModType, dPhi);
#endif
    }
    else if ((pCh->m_Capability&DTA1XX_CHCAP_AD9789) != 0)
    {
        // Save sample rate for FIFO-load computations (Direct IQ modes)
        pCh->m_ModSampRate = (Int)SymOrSampRate;
    }

    // Set phase increment for NCO
    pDta1xxTx->m_TxClock = dPhi;

    return Status;
}
