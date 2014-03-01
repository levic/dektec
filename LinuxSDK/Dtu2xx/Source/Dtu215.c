//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dtu215.c *#*#*#*#*#*#*#*#*# (C) 2009-2010 DekTec
//
// Contains DTU-215 specific code

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "Dtu2xx.h"
#include "Dtu2xxRegs.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// DTU-215 I2C addresses
#define  DTU215_IIC_ADDR_PWR_SUPPLY		0xCA	// I2C adress of onboard power supply
// DTU215 I2C power-supply register address
#define  DTU215_IIC_ADDR_PWR_1V2		0x10	// 1V2 power supply control register
#define  DTU215_IIC_ADDR_PWR_1V5		0x20	// 1V5 power supply control register
#define  DTU215_IIC_ADDR_PWR_1V8		0x40	// 1V8 power supply control register
#define  DTU215_IIC_ADDR_PWR_3V3		0x80	// 3V3 power supply control register
// Power supply control data
#define  DTU215_IIC_PWR_1V2_CTRL		0x98	// 1V2 power supply control data
#define  DTU215_IIC_PWR_1V5_CTRL		0xA4	// 1V5 power supply control data
#define  DTU215_IIC_PWR_1V8_CTRL		0x8F	// 1V8 power supply control data
#define  DTU215_IIC_PWR_3V3_CTRL		0x8F	// 3V3 power supply control data

// Timeout for SPI transfers
#define  DTU215_SPI_TIMEOUT				50		// 50ms

// Maximum output level (in 0.1 dBm units) for IQ mode
#define  DTU215_MAX_OUTP_LEVEL_IQ		-180	// -18dBm
// Maximum output level (in 0.1 dBm units) for QAM mode
#define  DTU215_MAX_OUTP_LEVEL_QAM		-150	// -15dBm
// Output level attnuation range (in 0.1 dBm units)
#define  DTU215_ATTN_RANGE				315		// 31.5dBm

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Internal functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int  Dtu215ComputeTxBytesPerSec(IN Channel*);
Int  Dtu215InitAd9789(IN Channel*);
Int  Dtu215IsSpiReady(IN Channel*, IN Int Timeout);
UInt8  Dtu215ModType2QamMapping(IN Int, IN Int);
UInt8  Dtu215ModType2Alpha(IN Int, IN Int);
Int  Dtu215SpiRegisterRead(IN Channel*, IN UInt16, IN UInt, OUT UInt8*);
Int  Dtu215SpiRegisterWrite(IN Channel*, IN UInt16, IN UInt, IN UInt8*);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Helper macros -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Helper MACRO for reading a SPI register
#define DTU215_SPI_RD(ch, addr, reg)										\
{																			\
	Status = Dtu215SpiRegisterRead(ch, addr, sizeof(reg), (UInt8*)&reg);	\
	if ( Status != 0 )											\
		return Status;														\
}
// Helper MACRO for writing to a SPI register
#define DTU215_SPI_WR(ch, addr, reg, value)									\
{																			\
	reg = value;															\
	Status = Dtu215SpiRegisterWrite(ch, addr, sizeof(reg), (UInt8*)&reg);	\
	if ( Status != 0 )											\
		return Status;														\
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTU-215 specific initialisation +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxInitDtu215MainPowerSupply -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dtu2xxInitDtu215MainPowerSupply(
	IN PDTU2XX_FDO  pFdo)		// Our device object
{
	Int  Status=0;
	UInt8 I2cBuf[8];

	// Make sure we have a DTU-215
	if ( pFdo->m_TypeNumber != 215 )
		return -EFAULT;

	// Start 1V2 power supply (ALTERA core voltage)
	// NOTE: we only start the 1V2 power supply here. Other power supplies are started
	// later when we init the device hardware.
	I2cBuf[0] = DTU215_IIC_ADDR_PWR_SUPPLY;
	I2cBuf[1] = DTU215_IIC_ADDR_PWR_1V2;
	I2cBuf[2] = DTU215_IIC_PWR_1V2_CTRL;
	Status = Dtu2xxIoCtlI2CWrite(pFdo, NULL, &I2cBuf[1], I2cBuf[0]&0xFE, 2);
	if ( Status != 0 )
	{
		DTU2XX_LOG(KERN_INFO, "Dtu2xxInitDtu215MainPowerSupply: Failed to enable 1V2 "
							  "power supply for DTU-%d", pFdo->m_TypeNumber );
		return Status;
	}
	// Sleep shortly to allow power-supply to settle
	Dtu2xxWaitMs(200);
	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxInitDtu215Hardware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dtu2xxInitDtu215Hardware(
	IN PDTU2XX_FDO  pFdo)		// Our device object
{
	Int i;
	Channel*  pCh=NULL;
	Int  Status=0;
	UInt8 I2cBuf[8];

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxInitDtu215Hardware: ENTER");
#endif

	//.-.-.-.-.-.-.-.-.-.-.-.-.- Init remaining power supplies -.-.-.-.-.-.-.-.-.-.-.-.-.-

	// Init 1V8SW and 3V3SW. They are control from the board control 0 register
	// (bits 4 and 5)
	pFdo->m_GenRegs.m_BoardControl0 |= 0x30;
	Status = Dtu2xxIoCtlWriteRegister(pFdo,
									  DTU2XX_GEN_BASE_ADDR + DTU2XX_GEN_REG_CONTROL0,
									  pFdo->m_GenRegs.m_BoardControl0);
	if ( Status != 0 ) {
		DTU2XX_LOG(KERN_INFO, "Dtu2xxInitDtu215Hardware: Failed to enable 1V8SW and "
							  "3V3SW power supplies for DTU-215" );
		return Status;
	}
	// Sleep shortly to allow power-supply to settle
	Dtu2xxWaitMs(200);
		
	// Finally Init 1V5 power supply
	I2cBuf[0] = DTU215_IIC_ADDR_PWR_SUPPLY;
	I2cBuf[1] = DTU215_IIC_ADDR_PWR_1V5;
	I2cBuf[2] = DTU215_IIC_PWR_1V5_CTRL;
	Status = Dtu2xxIoCtlI2CWrite(pFdo, NULL, &I2cBuf[1], I2cBuf[0]&0xFE, 2);
	if ( Status != 0 ) {

		DTU2XX_LOG(KERN_INFO, "Dtu2xxInitDtu215Hardware: Failed to enable 1V5 power supply "
							  "for DTU-215" );
		return Status;
	}
	// Sleep shortly to allow power-supply to settle
	Dtu2xxWaitMs(200);

	// Reset to force PLD to (re-)init devices that were just powered
	Dtu2xxIoCtlReset(pFdo);
	
	// Init channel(s)
	for ( i=0; i<pFdo->m_NumChannels; i++ )
	{
		pCh = &pFdo->m_Channel[i];
		
		// Step 1: basic channel initialisation
		Status = Dtu2xxInitModChannel(pCh);
		if ( Status != 0 )
			return Status;

		// Step 2: init AD9789 chip
		Status = Dtu215InitAd9789(pCh);
		if ( Status != 0 )
			return Status;

		// Step 3: Set default mod-control (DVB-C), RF-frequency (474MHz), output
		// level (-27.5dBm) and symbol rate (6.89MBd)
		Status = Dtu2xxTxIoCtlSetModControl(pFdo, i, DTU2XX_MOD_QAM64,
											DTU2XX_MOD_J83_A, -1,-1);
		if ( Status != 0 )
			return Status;
		Status = Dtu2xxTxIoCtlSetRfControl(pFdo, i, 474000000LL);
		if ( Status != 0 )
			return Status;
		Status = Dtu2xxTxIoCtlSetOutputLevel(pFdo, i, -275);
		if ( Status != 0 )
			return Status;
		Status = Dtu2xxTxIoCtlSetSymSampleRate(pFdo, i, 6890000);
		if ( Status != 0 )
			return Status;
	}
	return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Control methods +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu215GetMinMaxOutpLevel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  Dtu215GetMinMaxOutpLevel(IN Channel* pCh, OUT Int* pMinLevel, OUT Int* pMaxLevel)
{
	if (Dtu2xxIsIqMode(pCh->m_ModType) && pCh->m_ModType!=DTU2XX_MOD_ADTBT) {
		*pMinLevel = DTU215_MAX_OUTP_LEVEL_IQ - DTU215_ATTN_RANGE;
		*pMaxLevel = DTU215_MAX_OUTP_LEVEL_IQ;
	}
	else
	{
		*pMinLevel = DTU215_MAX_OUTP_LEVEL_QAM - DTU215_ATTN_RANGE;
		*pMaxLevel = DTU215_MAX_OUTP_LEVEL_QAM;
	}
}

//-.-.-.-.-.-.-.-.-.-.-.-.- Dtu215FrequencyResponseCompensation -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Apply frequency-response compensation, given the frequency stored in the channel data.

// Compensation table
// Full-scale-current is interpolated linearly between two points in this table.
struct {
	Int m_Freq;		// Frequency in MHz
	Int m_Fsc;		// Full scale current
} FreqCompTable[] = {
	{   0, 300 }, {  80, 329 }, { 180, 371 }, { 325, 415 }, { 484, 462 },
	{ 605, 511 }, { 680, 564 }, { 736, 619 }, { 792, 678 }, { 887, 740 },
	{ 1200, 780 }
};

Int  Dtu215FrequencyResponseCompensation(IN Channel* pCh)
{
	Int  Status = 0;
	Ad9789Registers* pSpiRegs = &(pCh->m_Ad9789);
	Int FreqInMHz, FreqL, FreqR, FscL, FscR, Idx;
	Int16 Fsc = 0;

	// Frequency in MHz
	FreqInMHz = (Int)(Dtu2xxBinDiv(pCh->m_RfFreq, 1000000LL, NULL));

	// Find frequency in compensation table
	for (Idx=1; Idx<sizeof(FreqCompTable); Idx++)
		if (FreqCompTable[Idx].m_Freq > FreqInMHz)
			break;

	// Idx points to {Freq, Fsc} pair to the "right" of RF frequency
	FreqL = FreqCompTable[Idx-1].m_Freq;
	FscL  = FreqCompTable[Idx-1].m_Fsc;
	FreqR = FreqCompTable[Idx].m_Freq;
	FscR  = FreqCompTable[Idx].m_Fsc;
	Fsc = FscL + (FscR - FscL) * (FreqInMHz - FreqL) / (FreqR - FreqL);

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "Dtu215FrequencyResponseCompensation: Freq=%d, Fsc=%d",
			   FreqInMHz, Fsc);
#endif

	// Write compensated Full Scale Current to DTU-215
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_FULLSCALECURRENT,
				  pSpiRegs->m_FullScaleCurrent.All, Fsc);

	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu215SetModControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dtu215SetModControl(
	IN Channel*  pCh,			// Our channel object
	IN Int  ModType,			// Modulation type
	IN Int  ParXtra0,			// Extra parameter #0
	IN Int  ParXtra1,			// Extra parameter #1
	IN Int  ParXtra2)			// Extra parameter #2
{
	Int  Status=0;
	Dtu2xxTx* pTxRegs = &(pCh->m_TxRegs);
	Ad9789Registers* pSpiRegs = &(pCh->m_Ad9789);
	UInt8 InScale = 0x20;
	UInt8 SumScale = 0x36;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "Dtu215SetModControl: ModType=%d, ParXtra0=%d, "
			   "ParXtra1=%d, ParXtra2=%d", ModType, ParXtra0, ParXtra1, ParXtra2);
#endif

	// Assume Tx FIFO load extrapolation is disabled
	pCh->m_EnaFifoLoadExtrap = FALSE;

	if (Dtu2xxIsIqMode(ModType))
	{
		// Modulation types that require I/Q sample mode

		// Use Direct-IQ mode
		pTxRegs->m_ModControl1.Fields.m_ModType = DTU2XX_TXMODC_IQDIRBUF;

		// Tx FIFO load will be extrapolated
		pCh->m_EnaFifoLoadExtrap = TRUE;

		// Bypass mapper and channel filter; Use 5 interpolation filters
		DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_BYPASS, pSpiRegs->m_Bypass.All, 0xC0);

		// SUMNODESCALAR and INPUTSCALAR depend on modulation type
		InScale = 0;
		SumScale = 0;
		switch (ModType) {
		case DTU2XX_MOD_ADTBT: InScale = 0x45; SumScale = 0x36; break;
		case DTU2XX_MOD_ATSC:  InScale = 0x50; SumScale = 0x36; break;
		case DTU2XX_MOD_CMMB:  InScale = 0x50; SumScale = 0x36; break;	// ??
		case DTU2XX_MOD_DMBTH: InScale = 0x32; SumScale = 0x36; break;
		case DTU2XX_MOD_DVBC2: InScale = 0x3A; SumScale = 0x36; break;	// ??
		case DTU2XX_MOD_DVBT:  InScale = 0x2E; SumScale = 0x36; break;
		case DTU2XX_MOD_DVBT2: InScale = 0x3A; SumScale = 0x36; break;	// ??
		case DTU2XX_MOD_IQDIRECT_NOLIC:
		case DTU2XX_MOD_IQDIRECT: InScale = 0x3A; SumScale = 0x36; break;
		case DTU2XX_MOD_ISDBT: InScale = 0x3A; SumScale = 0x36; break;
		case DTU2XX_MOD_T2MI:  InScale = 0x3A; SumScale = 0x36; break;	// ??
		}

		// Data bus 32-bits, 16-bits signed complex data format
		DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_DATACONTROL,
													   pSpiRegs->m_DataControl.All, 0x78);

		// Output level offset should be 0 for I/Q sample mode
		pTxRegs->m_ModControl2.Fields.m_OutpLvlOffset = 0x0;
	}
	else if (Dtu2xxIsQamMode(ModType))
	{
		// QAM-A/B/C - Common register settings first

		// QAM mapping
		pSpiRegs->m_QamSrrcConfiguration.Fields.m_Mapping = 
											  Dtu215ModType2QamMapping(ModType, ParXtra0);
		// Roll-off factor
		pSpiRegs->m_QamSrrcConfiguration.Fields.m_Alpha = 
												   Dtu215ModType2Alpha(ModType, ParXtra0);

		// Write mapping/filter settings to config register
		DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_QAMSRRCCONF,
					  pSpiRegs->m_QamSrrcConfiguration.All,
					  pSpiRegs->m_QamSrrcConfiguration.All);

		if (Dtu2xxIsQamB(ModType, ParXtra0))
		{
			// QAM-B

			// Use I/Q mapping mode
			pTxRegs->m_ModControl1.Fields.m_ModType = DTU2XX_TXMODC_IQMAP;

			// Use optimised I/Q mapping for 16-QAM, 32-QAM and 64-QAM
			// NOTE: 16-QAM and 32-QAM are not used for QAM-B, so could probably
			// be removed
			switch (ModType) {
			case DTU2XX_MOD_QAM16:
				pTxRegs->m_ModControl1.Fields.m_IqMap = DTU2XX_IQMAP_QAM16; break;
			case DTU2XX_MOD_QAM32:
				pTxRegs->m_ModControl1.Fields.m_IqMap = DTU2XX_IQMAP_QAM32; break;
			case DTU2XX_MOD_QAM64:
				pTxRegs->m_ModControl1.Fields.m_IqMap = DTU2XX_IQMAP_QAM64; break;
			// Generic map otherwise
			default:	pTxRegs->m_ModControl1.Fields.m_IqMap = DTU2XX_IQMAP_QAM; break;
													
			}

			// Bypass mapper and enable channel filter; Use 4 interpolation filters
			DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_BYPASS, pSpiRegs->m_Bypass.All, 0x90);

			SumScale = 0x2D;
			switch (ModType) {
			case DTU2XX_MOD_QAM64:  SumScale = 0x36; break;
			case DTU2XX_MOD_QAM256: SumScale = 0x36; break;
			}

			// Data bus 32-bits, 8-bits signed data, complex mode; no latency
			DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_DATACONTROL,
													   pSpiRegs->m_DataControl.All, 0x68);
		}
		else
		{
			// Hard QAM-A/C

			// For QAM, DTU2XX_TXMODC_ and DTU2XX_MOD_ constants are the same
			pTxRegs->m_ModControl1.Fields.m_ModType = ModType;

			// Enable mapper and channel filter; Use 4 interpolation filters
			DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_BYPASS, pSpiRegs->m_Bypass.All, 0x10);
				
			switch (ModType) {
			case DTU2XX_MOD_QAM4:   SumScale = 0x2D; break;
			case DTU2XX_MOD_QAM16:  SumScale = 0x2D; break;
			case DTU2XX_MOD_QAM32:  SumScale = 0x35; break;
			case DTU2XX_MOD_QAM64:  SumScale = 0x36; break;
			case DTU2XX_MOD_QAM128: SumScale = 0x4E; break;
			case DTU2XX_MOD_QAM256: SumScale = 0x36; break;
			}

			// Data bus 32-bits, 8-bits unsigned data; no latency
			DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_DATACONTROL,
													   pSpiRegs->m_DataControl.All, 0xE0);
		}

		// Output level offset should be +3 for QAM-based modulation types
		pTxRegs->m_ModControl2.Fields.m_OutpLvlOffset = 0x3;
	}
	else
		return -EINVAL;

	// Set scalers
#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "Dtu215SetModControl: InScale=%d, SumScale=%d",
			   InScale, SumScale);
#endif
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_SUMNODESCALAR,
												 pSpiRegs->m_SummingNodeScalar, SumScale);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_INPUTSCALAR, pSpiRegs->m_InputScalar, InScale);

	// Write modulation control register to hardware
	Status = Dtu2xxIoCtlWriteRegister(pCh->m_pFdo,
									  DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_MODCONTROL1,
									  (Int)(pTxRegs->m_ModControl1.All));
	if (Status != 0)
		return Status;

	// Write 'new' offset-level to hardware
	Status = Dtu2xxIoCtlWriteRegister(
						pCh->m_pFdo, DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_MODCONTROL2,
						(Int)(pTxRegs->m_ModControl2.All));
	if (Status != 0)
		return Status;

	// (Re-)compute number of tx bytes per sec
	Dtu215ComputeTxBytesPerSec(pCh);

	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu215SetRfControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dtu215SetRfControl(
	IN Channel*  pCh,			// Our channel object
	IN Int64  RfFreq)
{
	Int I, C;
	Int  Status=0;
	Ad9789Registers* pSpiRegs = &(pCh->m_Ad9789);

	// Setting the carrier frequency requires the initialisation of the NCO and
	// Band-pass-filter

	// Compute NCO increment I. I=RfFreq/(2400MHz/16) * (2^24-1)

	I = (Int)(Dtu2xxBinDiv((RfFreq*16777215LL), 150000000LL, NULL)) & 0x00FFFFFF;
	// Compute BPF value C. C=RfFreq/(2400MHz) * (2^16-1)
	C = (Int)(Dtu2xxBinDiv((RfFreq*65535LL), 2400000000LL, NULL)) & 0x0000FFFF;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "Dtu215SetRfControl: RfFreq=%I64d, I=%d, C=%d",
			   RfFreq, I, C);
#endif

	// Set tuning word
	pSpiRegs->m_Nco0FreqTuningWord[0] = (UInt8)(I & 0xFF);
	pSpiRegs->m_Nco0FreqTuningWord[1] = (UInt8)((I>>8) & 0xFF);
	pSpiRegs->m_Nco0FreqTuningWord[2] = (UInt8)((I>>16) & 0xFF);

	Status = Dtu215SpiRegisterWrite(pCh, DTU2XX_AD9789_REG_NCO0FREQTUNEWORD,
									sizeof(pSpiRegs->m_Nco0FreqTuningWord),
									(UInt8*)pSpiRegs->m_Nco0FreqTuningWord);
	if ( Status != 0 )
		return Status;

	// Set Band-Pass-Filter
	pSpiRegs->m_InterpolatingBpfCenterFreq[0] = (UInt8)(C & 0xFF);
	pSpiRegs->m_InterpolatingBpfCenterFreq[1] = (UInt8)((C>>8) & 0xFF);

	Status = Dtu215SpiRegisterWrite(pCh, DTU2XX_AD9789_REG_INTERPOLBPFCENTERFREQ,
									sizeof(pSpiRegs->m_InterpolatingBpfCenterFreq),
									(UInt8*)pSpiRegs->m_InterpolatingBpfCenterFreq);
	if ( Status != 0 )
		return Status;

	// Finally set update bit
	pSpiRegs->m_FrequencyUpdate.Fields.m_FreqNew = 1;
	return Dtu215SpiRegisterWrite(pCh, DTU2XX_AD9789_REG_FREQUPDATE,
								  sizeof(pSpiRegs->m_FrequencyUpdate.All),
								  (UInt8*)&pSpiRegs->m_FrequencyUpdate.All);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu215SetRfMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dtu215SetRfMode(
	IN Channel*  pCh,			// Our channel object
	IN Int RfMode)				// Desired RF-mode
{
	Int  Mode = 0;
	Int  Status = 0;
	Ad9789Registers* pSpiRegs = &(pCh->m_Ad9789);

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "Dtu215SetRfMode: RfMode=%d", RfMode);
#endif

	// Enable/disable AD9789 channel based on requested mode. NOTE: channel 1-3
	// are always disabled
	pSpiRegs->m_ChannelEnable.Fields.m_ChanEn1 = 0;
	pSpiRegs->m_ChannelEnable.Fields.m_ChanEn2 = 0;
	pSpiRegs->m_ChannelEnable.Fields.m_ChanEn3 = 0;

	Mode = RfMode & ~DTU2XX_UPCONV_SPECINV;
	switch (Mode) {

	case DTU2XX_UPCONV_CW:
	case DTU2XX_UPCONV_CWI:
	case DTU2XX_UPCONV_CWQ:
	case DTU2XX_UPCONV_NORMAL:
		// Enable channel 0
		pSpiRegs->m_ChannelEnable.Fields.m_ChanEn0 = 1;
		break;

	case DTU2XX_UPCONV_MUTE:
		// Fully disable channel
		pSpiRegs->m_ChannelEnable.Fields.m_ChanEn0 = 0;
		break;
	
	default:
		return -EINVAL;
	}

	// Write channel enable state to AD9789
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_CHANNELENA,
				  pSpiRegs->m_ChannelEnable.All,
				  pSpiRegs->m_ChannelEnable.All);

	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu215SetSymSampleRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dtu215SetSymSampleRate(
	IN Channel*  pCh,			// Our channel object
	IN Int  SymSamplRate)
{
	Int P, Q;
	Int  Status = 0;
	Ad9789Registers* pSpiRegs = &(pCh->m_Ad9789);

	// Check for valid symbol/sample rate (4.6875 - 9.375MHz/bd)
	if (SymSamplRate<=4687500 || SymSamplRate>9375000)
	{
#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu215SetSymSampleRate: INVALID SYMSAMPLRATE (%d)",
			   SymSamplRate);
#endif
		return -EINVAL;
	}

	// Compute P and Q for symbol/sample rate. P = 4687500 / SymOrSampRate * Q
	// NOTE: For now we use a fixed Q of 0xFFFFFF
	Q = 0xFFFFFF;
	P = (Int)(Dtu2xxBinDiv((4687500LL * Q), (Int64)SymSamplRate, NULL));

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "Dtu215SetSymSampleRate: SymSamplRate=%d, P=0x%08X, Q=0x%08X",
			   SymSamplRate, P, Q);
#endif

	// Set P
	pSpiRegs->m_RateConverterNumP[0] = (UInt8)(P & 0xFF);
	pSpiRegs->m_RateConverterNumP[1] = (UInt8)((P>>8) & 0xFF);
	pSpiRegs->m_RateConverterNumP[2] = (UInt8)((P>>16) & 0xFF);

	Status = Dtu215SpiRegisterWrite(pCh, DTU2XX_AD9789_REG_RATECONVNUMP,
									sizeof(pSpiRegs->m_RateConverterNumP),
									(UInt8*)pSpiRegs->m_RateConverterNumP);
	if ( Status != 0 )
		return Status;

	// Set Q
	pSpiRegs->m_RateConverterDenomQ[0] = (UInt8)(Q & 0xFF);
	pSpiRegs->m_RateConverterDenomQ[1] = (UInt8)((Q>>8) & 0xFF);
	pSpiRegs->m_RateConverterDenomQ[2] = (UInt8)((Q>>16) & 0xFF);

	Status = Dtu215SpiRegisterWrite(pCh, DTU2XX_AD9789_REG_RATECONVDENOMQ,
									sizeof(pSpiRegs->m_RateConverterDenomQ),
									(UInt8*)pSpiRegs->m_RateConverterDenomQ);
	if ( Status != 0 )
		return Status;

	// Set update bit
	pSpiRegs->m_FrequencyUpdate.Fields.m_FreqNew = 1;
	Status = Dtu215SpiRegisterWrite(pCh, DTU2XX_AD9789_REG_FREQUPDATE,
								    sizeof(pSpiRegs->m_FrequencyUpdate.All),
								    (UInt8*)&pSpiRegs->m_FrequencyUpdate.All);
	if ( Status != 0 )
		return Status;

	// (Re-)compute number of tx bytes per sec
	Dtu215ComputeTxBytesPerSec(pCh);
	return 0;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ AD9789 helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu215ComputeTxBytesPerSec -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Compute m_NumTxBytesPerSec, which is used to extrapolate FIFO load in I/Q sample mode.
//
Int  Dtu215ComputeTxBytesPerSec(IN Channel* pCh)
{
	Int  r=0;
	Int64  TsSymOrSampRate256 = 0;
		
	// Init to "safe" value
	pCh->m_NumTxBytesPerSec = 0;

	if (pCh->m_TsSymOrSampRate <= 0)
		return 0;		// No symbol or sample

	// Only required when Tx FIFO load is extrapolated
	if (pCh->m_EnaFifoLoadExtrap)
	{
		// Multiply by 256 to prevent rounding issues in the following calculations
		TsSymOrSampRate256 = ((Int64)pCh->m_TsSymOrSampRate)<<8;

		r = down_interruptible(&pCh->m_LoadExtrapLock);
		
		// The samples are stored in the HW-FIFO in compressed frame form.
		// One frame of 4096 16-bit words, contains 1 sync word + 6552 10-bit I+Q samples
		// NOTE: Do not forget to divide by 256 again
		pCh->m_NumTxBytesPerSec = (Int)(Dtu2xxBinDiv((TsSymOrSampRate256*4LL*4096LL), 6552LL, NULL)>>8);

		up(&pCh->m_LoadExtrapLock);
	}

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO,
			   "Dtu215ComputeTxBytesPerSec: SymSamplRate=%d, NumTxBytesPerSec=%d",
			   pCh->m_TsSymOrSampRate, pCh->m_NumTxBytesPerSec);
#endif

	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu215InitAd9789 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dtu215InitAd9789(IN Channel* pCh)
{
	Int  Status = 0;
	Ad9789Registers* pSpiRegs = &(pCh->m_Ad9789);

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu215InitAd9789: ENTER");
#endif

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Init all AD9789 registers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	// First setup SI control register
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_SPICONTROL, pSpiRegs->m_SpiControl.All, 0x18);
	
	// Avoid transients; disable all channels for now 
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_CHANNELENA, pSpiRegs->m_ChannelEnable.All, 0x00);
	
	// fDCO = 37.5MHz
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_DCOFREQ, pSpiRegs->m_DcoFrequency.All, 0x4F);
	// Phase internal clocks
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_INTERNALCLKPHASEADJUST,
				  pSpiRegs->m_InternalClockPhaseAdjust.All, 0x07);
	
	// Enable duty-cycle correction
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_MUDELAYCONTROL1,
				  pSpiRegs->m_MuDelayControl1.All, 0x80);

	// set common-mode input level clock receiver
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_CLKRECEIVER1,
				  pSpiRegs->m_ClockReceiver1.All, 0xF0);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_CLKRECEIVER2,
				  pSpiRegs->m_ClockReceiver2.All, 0x9E);
	
	// Synchronization sequence as advised by ADI
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_PHASEDETECTORCONTROL,
				  pSpiRegs->m_PhaseDetectorControl.All, 0x38);
	
	// Update params
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_PARAMETERUPDATE,
				  pSpiRegs->m_ParameterUpdate.All, 0x00);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_PARAMETERUPDATE,
				  pSpiRegs->m_ParameterUpdate.All, 0x80);

	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_MUDELAYCONTROL1,
				  pSpiRegs->m_MuDelayControl1.All, 0xCE);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_MUDELAYCONTROL2,
				  pSpiRegs->m_MuDelayControl2.All, 0x42);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_MUDELAYCONTROL3,
				  pSpiRegs->m_MuDelayControl3.All, 0x6C4E);
	
	// Disable all interrupts 
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_INTENA, pSpiRegs->m_InterruptEnable.All, 0x00);
	// Clear all pending interrupts
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_INTSSTATUSANDCLR,
				  pSpiRegs->m_IntStatusClear.All, 0xFF);			
	// Enable LockAcq and LockLost Interrupts
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_INTENA, pSpiRegs->m_InterruptEnable.All, 0x0C);

	// Enable mu-delay controller
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_MUDELAYCONTROL1,
				  pSpiRegs->m_MuDelayControl1.All, 0x43);

	// Wait atleast 1ms to allow the DLL to lock
	Dtu2xxWaitMs(1);

	// Sample clock phase
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_MUDELAYCONTROL2,
				  pSpiRegs->m_MuDelayControl2.All, 0x4B);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_MUDELAYCONTROL2,
				  pSpiRegs->m_MuDelayControl2.All, 0x43);

	// Register 0x04 should read 0x08 for the DLL to be locked
	// Register 0x39 should read 0x4E for the correct clock phase

	// Channelizer mode, variable priority, disable parity check
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_ITFCONF,
				  pSpiRegs->m_Interfaceconfiguration.All, 0xC0);
	// Data bus 32-bits, 8-bits unsigned data; no latency
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_DATACONTROL,
				  pSpiRegs->m_DataControl.All, 0xE0);
	// Enable mapper, chan. filter; 4 Interpolation Filters
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_BYPASS, pSpiRegs->m_Bypass.All, 0x10);
	// 15% Channel Filter, DVB-C 64-QAM
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_QAMSRRCCONF,
			      pSpiRegs->m_QamSrrcConfiguration.All, 0x24);
	// Scaler for QAM/OFDM; optimized for single-channel 
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_SUMNODESCALAR, 
				  pSpiRegs->m_SummingNodeScalar, 0x36);
	// Input scaler for OFDM and Sym2IQ
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_INPUTSCALAR, pSpiRegs->m_InputScalar, 0x20);

	// set NCO#0 to 474MHz RF frequency
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_NCO0FREQTUNEWORD,
				  pSpiRegs->m_Nco0FreqTuningWord[0], 0xC2);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_NCO0FREQTUNEWORD+1,
				  pSpiRegs->m_Nco0FreqTuningWord[1], 0xF5);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_NCO0FREQTUNEWORD+2,
				  pSpiRegs->m_Nco0FreqTuningWord[2], 0x28);
	// Channel is not used; set NCO#1 to 0Hz to save power
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_NCO1FREQTUNEWORD,
				  pSpiRegs->m_Nco1FreqTuningWord[0], 0x00);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_NCO1FREQTUNEWORD+1,
				  pSpiRegs->m_Nco1FreqTuningWord[1], 0x00);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_NCO1FREQTUNEWORD+2,
				  pSpiRegs->m_Nco1FreqTuningWord[2], 0x00);
	// Channel is not used; set NCO#2 to 0Hz to save power
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_NCO2FREQTUNEWORD,
				  pSpiRegs->m_Nco2FreqTuningWord[0], 0x00);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_NCO2FREQTUNEWORD+1,
				  pSpiRegs->m_Nco2FreqTuningWord[1], 0x00);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_NCO2FREQTUNEWORD+2,
				  pSpiRegs->m_Nco2FreqTuningWord[2], 0x00);
	// Channel is not used; set NCO#3 to 0Hz to save power
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_NCO3FREQTUNEWORD, 
				  pSpiRegs->m_Nco3FreqTuningWord[0], 0x00);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_NCO3FREQTUNEWORD+1,
				  pSpiRegs->m_Nco3FreqTuningWord[1], 0x00);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_NCO3FREQTUNEWORD+2,
				  pSpiRegs->m_Nco3FreqTuningWord[2], 0x00);
	
	// Set rate converter to 6.89MBd: value for Q
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_RATECONVDENOMQ,
				  pSpiRegs->m_RateConverterDenomQ[0], 0xFF);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_RATECONVDENOMQ+1, 
				  pSpiRegs->m_RateConverterDenomQ[1], 0xFF);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_RATECONVDENOMQ+2, 
				  pSpiRegs->m_RateConverterDenomQ[2], 0xFF);
	// Value for P
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_RATECONVNUMP, 
				  pSpiRegs->m_RateConverterNumP[0], 0x5A);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_RATECONVNUMP+1,
				  pSpiRegs->m_RateConverterNumP[1], 0x2A);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_RATECONVNUMP+2,
				  pSpiRegs->m_RateConverterNumP[2], 0xAE);

	// Set BPF to 474MHz RF frequency
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_INTERPOLBPFCENTERFREQ,
				  pSpiRegs->m_InterpolatingBpfCenterFreq[0], 0x8F);
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_INTERPOLBPFCENTERFREQ+1,
				  pSpiRegs->m_InterpolatingBpfCenterFreq[1], 0x32);

	// Update Rate Converter and BPF
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_FREQUPDATE,
				  pSpiRegs->m_FrequencyUpdate.All, 0x80);

	// Gain channel 0 = 1
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_CHANNEL0GAIN, pSpiRegs->m_Channel0Gain, 0x80);
	// Gain channel 1 = 0
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_CHANNEL1GAIN, pSpiRegs->m_Channel1Gain, 0x00);
	// Gain channel 2 = 0
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_CHANNEL2GAIN, pSpiRegs->m_Channel2Gain, 0x00);
	// Gain channel 3 = 0
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_CHANNEL3GAIN, pSpiRegs->m_Channel3Gain, 0x00);

	// No spectral inversion
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_SPECTERUMSHAPING,
				  pSpiRegs->m_SpectrumShaping.All, 0x00);
			
	// Bypass mirror roll-off
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_DACBIAS, pSpiRegs->m_DacBias.All, 0x00);
	
	// DAC in Normal Mode (1st Nyquist zone)
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_DACDECODER, pSpiRegs->m_DacDecoder.All, 0x00);
	
	// 19.2mA DAC full-scale current
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_FULLSCALECURRENT,
				  pSpiRegs->m_FullScaleCurrent.All, 0x01CE);
	
	// Enable only LockLost Interrupts
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_INTENA, pSpiRegs->m_InterruptEnable.All, 0x04);
	// Clear all pending interrupts
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_INTSSTATUSANDCLR,
				  pSpiRegs->m_IntStatusClear.All, 0xFF);			
	
	// Enable channel#0
	DTU215_SPI_WR(pCh, DTU2XX_AD9789_REG_CHANNELENA, pSpiRegs->m_ChannelEnable.All, 0x01);

	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu215IsSpiReady -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dtu215IsSpiReady(
	IN Channel*  pCh,			// [in] Channel object
	IN Int  Timeout)			// [in] Timeout in ms
{
	Int  Status=0;
	Dtu2xxTx* pTxRegs = &(pCh->m_TxRegs);

	// Check for valid value (0 - 10000ms)
	if ( Timeout<=0 || Timeout>10000 )
		return -EINVAL;

	// Get AD9789 control register
	Status = Dtu2xxIoCtlReadRegister(pCh->m_pFdo,
									 DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_AD9789CTRL,
									 (Int*)(&pTxRegs->m_Ad9789Control.All));
	if ( Status != 0 )
		return Status;

	// Wait for ready bit
	while ( !pTxRegs->m_Ad9789Control.Fields.m_SpiReady && Timeout>=0 )
	{
		// Wait shortly
		Dtu2xxWaitMs(1);
		
		// Re-get AD9789 control register
		Status = Dtu2xxIoCtlReadRegister(pCh->m_pFdo,
									 DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_AD9789CTRL,
									 (Int*)(&pTxRegs->m_Ad9789Control.All));
		if ( Status != 0 )
			return Status;

		Timeout--;
	}
	return (Timeout>=0) ? 0 : -EFAULT;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu215SpiRegisterRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dtu215SpiRegisterRead(
	IN Channel*  pCh,			// [in] Channel object
	IN UInt16  RegAddr,			// [in] SPI register start address
	IN UInt  NumToRead,			// [in] Number of bytes to read
	OUT UInt8*  pData)			// [out] Buffer to receive data
{
	UInt i;
	Int  Status=0;
	Dtu2xxTx* pTxRegs = &(pCh->m_TxRegs);

	// Check for valid values
	if ( pData == NULL )
		return -EINVAL;
	if ( NumToRead<1 || NumToRead>4 )
		return -EINVAL;

	// Check previous SPI-transfer has been completed
	Status = Dtu215IsSpiReady(pCh, DTU215_SPI_TIMEOUT);
	if ( Status!=0 )
		return Status;

	// Setup control register for a read transfer
	// NOTE: In MSB bit first mode addresses will decrement, so we must start the 
	// transfer at MSB address
	pTxRegs->m_Ad9789Control.Fields.m_SpiStart	= 1;
	pTxRegs->m_Ad9789Control.Fields.m_SpiNumBytes = (NumToRead-1);
	pTxRegs->m_Ad9789Control.Fields.m_SpiRdOrWrn = 1;
	pTxRegs->m_Ad9789Control.Fields.m_SpiAddr = RegAddr+(NumToRead-1);

	Status = Dtu2xxIoCtlWriteRegister(pCh->m_pFdo,
									  DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_AD9789CTRL,
									  (Int)(pTxRegs->m_Ad9789Control.All));
	if ( Status != 0 ) 
			return Status;

	// Wait for transfer to complete
	Status = Dtu215IsSpiReady(pCh, DTU215_SPI_TIMEOUT);
	if ( Status!=0 )
		return Status;
		
	// Read data from SPI data register
    Status = Dtu2xxIoCtlReadRegister(pCh->m_pFdo,
									 DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_AD9789DATA,
									 (Int*)&(pTxRegs->m_Ad9789Data));
	if ( Status != 0 ) 
		return Status;

	// Copy data to buffer
	for ( i=0; i<NumToRead; i++ )
		pData[i] = (UInt8)((pTxRegs->m_Ad9789Data>>(i*8))&0xFF);

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "Dtu215SpiRegisterRead: RegAddr=0x%04X, NumToRead=%d, "
			   "Data=0x%08X", RegAddr, NumToRead, pTxRegs->m_Ad9789Data);
#endif

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu215SpiRegisterWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dtu215SpiRegisterWrite(
	IN Channel*  pCh,			// [in] Channel object
	IN UInt16  RegAddr,			// [in] SPI register start address
	IN UInt  NumToWrite,		// [in] Number of bytes to write
	IN UInt8*  pData)			// [in] Buffer with data to write
{
	UInt i;
	Int  Status=0;
	Dtu2xxTx* pTxRegs = &(pCh->m_TxRegs);

	// Check for valid values
	if ( pData == NULL )
		return -EINVAL;
	if ( NumToWrite<1 || NumToWrite>4 )
		return -EINVAL;

	// Check previous SPI-transfer has been completed
	Status = Dtu215IsSpiReady(pCh, DTU215_SPI_TIMEOUT);
	if ( Status!=0 )
		return Status;

	// First write data to data register
	pTxRegs->m_Ad9789Data = 0;
	for ( i=0; i<NumToWrite; i++ )
		pTxRegs->m_Ad9789Data |= ((UInt32)pData[i])<<(i*8);

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "Dtu215SpiRegisterWrite: RegAddr=0x%04X, NumToWrite=%d, "
			   "Data=0x%08X", RegAddr, NumToWrite, pTxRegs->m_Ad9789Data);
#endif

	Status = Dtu2xxIoCtlWriteRegister(pCh->m_pFdo,
									  DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_AD9789DATA,
									  (Int)(pTxRegs->m_Ad9789Data));
	if ( Status != 0 )
		return Status;


	// Setup control register for a write transfer
	// NOTE: In MSB bit first mode addresses will decrement, so we must start the 
	// transfer at MSB address
	pTxRegs->m_Ad9789Control.Fields.m_SpiStart	= 1;
	pTxRegs->m_Ad9789Control.Fields.m_SpiNumBytes = (NumToWrite-1);
	pTxRegs->m_Ad9789Control.Fields.m_SpiRdOrWrn = 0;
	pTxRegs->m_Ad9789Control.Fields.m_SpiAddr = RegAddr+(NumToWrite-1);

	Status = Dtu2xxIoCtlWriteRegister(pCh->m_pFdo,
									  DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_AD9789CTRL,
									  (Int)(pTxRegs->m_Ad9789Control.All));
	if ( Status != 0 ) 
			return Status;

	// Wait for transfer to complete
	Status = Dtu215IsSpiReady(pCh, DTU215_SPI_TIMEOUT);
	if ( Status!=0 )
		return Status;

	return 0;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ MISCELLANEOUS HELPERS +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu215ModType2Alpha -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Converts modulation type to the corresponding alpha (roll-off) setting 
//
UInt8  Dtu215ModType2Alpha(
	IN Int  ModType,
	IN Int  ParXtra0)
{
	BOOLEAN IsQamA=FALSE, IsQamB=FALSE, IsQamC=FALSE;

	// Check for QAM-A, B or C
	IsQamA = Dtu2xxIsHardQamA(ModType, ParXtra0);
	IsQamB = Dtu2xxIsQamB(ModType, ParXtra0);
	IsQamC = Dtu2xxIsHardQamC(ModType, ParXtra0);

	// Set roll-off
	if ( IsQamA )
		return 0x2; // QAM-A: 15% rolloff
	else if ( IsQamC )
		return 0x3; // QAM-C: 13% rolloff
	else /* if ( IsQamB ) */
	{
		// QAM-B: 12% rolloff for 256-QAM or 18% for 64-QAM
		return (ModType==DTU2XX_MOD_QAM256) ? 0x0 : 0x1;
	}
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu215ModType2QamMapping -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Converts modulation type to the corresponding QAM mapping setting
//
UInt8  Dtu215ModType2QamMapping(
	IN Int  ModType,
	IN Int  ParXtra0)
{
	BOOLEAN IsQamA=FALSE, IsQamB=FALSE, IsQamC=FALSE;

	// Check for QAM-A, B or C
	IsQamA = Dtu2xxIsHardQamA(ModType, ParXtra0);
	IsQamB = Dtu2xxIsQamB(ModType, ParXtra0);
	IsQamC = Dtu2xxIsHardQamC(ModType, ParXtra0);
				
	// Set mapping
	switch ( ModType )
	{
	// NOTE: we use QAM16 mapping for QAM-4 (as a trick-mode)
	case DTU2XX_MOD_QAM4:	return 0x2; 
	case DTU2XX_MOD_QAM16:	return 0x2; 
	case DTU2XX_MOD_QAM32:	return 0x3; 
	case DTU2XX_MOD_QAM64:	return IsQamB ? 0x0 : 0x4; // Use DOCSIS 64-QAM for QAM-B
	case DTU2XX_MOD_QAM128:	return 0x5;
	case DTU2XX_MOD_QAM256:	return IsQamB ? 0x1 : 0x6; // Use DOCSIS 256-QAM for QAM-B
	}

	return 0x4; // By default assume QAM-64
}


