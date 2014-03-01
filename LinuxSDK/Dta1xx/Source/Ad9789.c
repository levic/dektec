//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Ad9789.c *#*#*#*#*#*#*#*#*# (C) 2009-2010 DekTec
//
// Contains AD9789 specific code

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	2010.01.27	AT	Created

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "../Include/Dta1xx.h"
#include "../Include/Dta1xxRegs.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Timeout for SPI transfers
#define  AD9789_SPI_TIMEOUT				50		// 50ms

// Maximum output level (in 0.1 dBm units) for IQ mode
#define  AD9789_MAX_OUTP_LEVEL_IQ		-180	// -18dBm
// Maximum output level (in 0.1 dBm units) for QAM mode
#define  AD9789_MAX_OUTP_LEVEL_QAM		-150	// -15dBm
// Output level attnuation range (in 0.1 dBm units)
#define  AD9789_ATTN_RANGE				315		// 31.5dBm

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Ad9789 register offset +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
#define AD9789_REG_SPICONTROL					0x00
#define AD9789_REG_SATURATIONCOUNTER			0x01
#define AD9789_REG_PARITYCOUNTER				0x02
#define AD9789_REG_INTENA						0x03
#define AD9789_REG_INTSSTATUSANDCLR				0x04
#define AD9789_REG_CHANNELENA					0x05
#define AD9789_REG_BYPASS						0x06
#define AD9789_REG_QAMSRRCCONF					0x07
#define AD9789_REG_SUMNODESCALAR				0x08
#define AD9789_REG_INPUTSCALAR					0x09
#define AD9789_REG_NCO0FREQTUNEWORD				0x0A//-0x0C
#define AD9789_REG_NCO1FREQTUNEWORD				0x0D//-0x0F
#define AD9789_REG_NCO2FREQTUNEWORD				0x10//-0x12
#define AD9789_REG_NCO3FREQTUNEWORD				0x13//-0x15	
#define AD9789_REG_RATECONVDENOMQ				0x16//-0x18
#define AD9789_REG_RATECONVNUMP					0x19//-0x1B
#define AD9789_REG_INTERPOLBPFCENTERFREQ		0x1C//-0x1D
#define AD9789_REG_FREQUPDATE					0x1E
#define AD9789_REG_HWVERSION					0x1F
#define AD9789_REG_ITFCONF						0x20
#define AD9789_REG_DATACONTROL					0x21
#define AD9789_REG_DCOFREQ						0x22
#define AD9789_REG_INTERNALCLKPHASEADJUST		0x23
#define AD9789_REG_PARAMETERUPDATE				0x24
#define AD9789_REG_CHANNEL0GAIN					0x25
#define AD9789_REG_CHANNEL1GAIN					0x26
#define AD9789_REG_CHANNEL2GAIN					0x27
#define AD9789_REG_CHANNEL3GAIN					0x28
#define AD9789_REG_SPECTERUMSHAPING				0x29
#define AD9789_REG_MUDELAYCONTROL1				0x2F
#define AD9789_REG_MUCONTROLDUTYCYCLE			0x30
#define AD9789_REG_CLKRECEIVER1					0x31
#define AD9789_REG_CLKRECEIVER2					0x32
#define AD9789_REG_MUDELAYCONTROL2				0x33
#define AD9789_REG_RESERVED1					0x34
#define AD9789_REG_RESERVED2					0x35
#define AD9789_REG_DACBIAS						0x36
#define AD9789_REG_RESERVED3					0x37
#define AD9789_REG_DACDECODER					0x38
#define AD9789_REG_MUDELAYCONTROL3				0x39//-0x3A
#define AD9789_REG_RESERVED4					0x3B
#define AD9789_REG_FULLSCALECURRENT				0x3C//-0x3D
#define AD9789_REG_PHASEDETECTORCONTROL			0x3E
#define AD9789_REG_RESERVED5					0x3F
#define AD9789_REG_BISTCONTROL					0x40
#define AD9789_REG_BISTSTATUS					0x41
#define AD9789_REG_BISTZEROPADDINGLENGTH		0x42//-0x43
#define AD9789_REG_BISTVECTORLENGTH				0x44//-0x46
#define AD9789_REG_BISTCLOCKADJUST				0x47
#define AD9789_REG_SIGN0CONTROL					0x48
#define AD9789_REG_SIGN0CLOCKADJUST				0x49
#define AD9789_REG_SIGN1CONTROL					0x4A
#define AD9789_REG_SIGN1CLOCKADJUST				0x4B
#define AD9789_REG_REGFNL0FREQ					0x4C
#define AD9789_REG_REGFNL1FREQ					0x4D
#define AD9789_REG_BISTSIGNATURE0				0x4E//-0x50
#define AD9789_REG_BISTSIGNATURE1				0x51//-0x53

typedef
	union {
	struct {								// AD9789 SPI control register
	volatile UInt32  m_SpiReady		: 1;	//  Ready for new spi cycle
	volatile UInt32  m_SpiStart		: 1;	//  Start a SPI cycle (Write-Only bit)
	volatile UInt32  m_SpiNumBytes	: 2;	//  Number of bytes to transfer (0=1;1=2;2=3;3=4)
	volatile UInt32  m_SpiRdOrWrn	: 1;	//  Read ('1') or write ('0') transfer
	         UInt32  m_Reserved		: 11;	//
	volatile UInt32  m_SpiAddr		: 16;	//  SPI ADDRESS
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  t_Ad9789Control;

typedef
	union
	{
		struct
		{
			UInt8  m_ChanEn0		: 1;
			UInt8  m_ChanEn1		: 1;
			UInt8  m_ChanEn2		: 1;
			UInt8  m_ChanEn3		: 1;
			UInt8  m_Reserved		: 4;
		}          Fields;
		UInt8      All;
	}  t_ChannelEnable;						// Channel Enable Register (0x05)

typedef
	union {
	struct {								// Modulation-Control#1 register
	volatile UInt32  m_IqMap		: 4;	//  I/Q mapping
	volatile UInt32  m_ModType		: 4;	//  Modulation type
	volatile UInt32  m_NoRnd		: 1;	//  Do not randomise payload of null packets
	volatile UInt32  m_IntPolSel	: 3;	//  Interpolator selection
	volatile UInt32  m_RollOff		: 4;	//  Roll-Off factor
	volatile UInt32  m_OutputLvl	: 8;	//  Output level (scaling factor)
	volatile UInt32  m_UserLvl		: 1;	//  Override output level
	volatile UInt32  m_SpecInv		: 1;	//  Spectral inversion
	volatile UInt32  m_MuteI		: 1;	//  Mute I
	volatile UInt32  m_MuteQ		: 1;	//  Mute Q
	volatile UInt32  m_TestPat		: 4;	//  Test pattern
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  t_ModControl1;

typedef
	union {
	struct {								// Modulation-Control#2 register
	volatile UInt32  m_OutpLvlOffset: 4;	//  Output level offset (scaling factor offset)
			 UInt32  m_Reserved1	: 28;	//  Reserved bits
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  t_ModControl2;

typedef
	union {
	struct {
		UInt8  m_Mapping		: 3;
		UInt8  m_Reserved1		: 1;
		UInt8  m_Alpha			: 2;
		UInt8  m_Reserved2		: 2;	
	}          Fields;
		UInt8  All;
	}  t_QamSrrcConfiguration;				// QAM/SRRC Configuration Register (0x07)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Internal functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
NTSTATUS  Ad9789Init(Channel*);
NTSTATUS  Ad9789IsSpiReady(Channel*, Int Timeout);
UInt8  Ad9789ModType2QamMapping(Int, Int);
UInt8  Ad9789ModType2Alpha(Int, Int);
NTSTATUS  Ad9789SpiRegisterWrite(Channel*, UInt16, UInt, UInt32);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Helper macros -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Helper MACRO for writing to a SPI register
#define AD9789_SPI_WR(ch, addr, size, value)							\
{																			\
	Status = Ad9789SpiRegisterWrite(ch, addr, size, value);			 \
	if ( Status != STATUS_SUCCESS )											\
		return Status;														\
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Control methods +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ad9789GetMinMaxOutpLevel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  Ad9789GetMinMaxOutpLevel(Channel* pCh, Int* pMinLevel, Int* pMaxLevel)
{
	if (Dta1xxIsIqMode(pCh->m_ModType) && pCh->m_ModType!=DTA1XX_MOD_ADTBT) {
		*pMinLevel = AD9789_MAX_OUTP_LEVEL_IQ - AD9789_ATTN_RANGE;
		*pMaxLevel = AD9789_MAX_OUTP_LEVEL_IQ;
	}
	else
	{
		*pMinLevel = AD9789_MAX_OUTP_LEVEL_QAM - AD9789_ATTN_RANGE;
		*pMaxLevel = AD9789_MAX_OUTP_LEVEL_QAM;
	}
}

//-.-.-.-.-.-.-.-.-.-.-.-.- Ad9789FrequencyResponseCompensation -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Apply frequency-response compensation, given the frequency stored in the channel data.
// Full-scale-current is interpolated linearly between two points in this table.

NTSTATUS  Ad9789FrequencyResponseCompensation(Channel* pCh)
{
	NTSTATUS  Status = STATUS_SUCCESS;
	Int FreqInMHz, FreqL, FreqR, FscL, FscR, Idx;
	Int16 Fsc = 0;
	PFreqCompTable FreqCompTable = pCh->m_FreqCompTable;
	
	Fsc = 512;
	if (pCh->m_FreqCompTable!=NULL && pCh->m_FreqCompTableSize>=2) {
		// Frequency in MHz

#if defined(WIN32) || defined(WIN64)
		FreqInMHz = (Int)(pCh->m_RfFreq / 1000000LL);
#else
		FreqInMHz = (Int)(Dta1xxBinDiv(pCh->m_RfFreq, 1000000LL, 0));
#endif

		// Find frequency in compensation table
		for (Idx=1; Idx<pCh->m_FreqCompTableSize; Idx++)
			if (FreqCompTable[Idx].m_Freq > FreqInMHz)
				break;

		if (Idx>=pCh->m_FreqCompTableSize) {
			Fsc = pCh->m_FreqCompTable[pCh->m_FreqCompTableSize-1].m_Fsc;
		} else if (pCh->m_FreqCompTable[0].m_Freq >= FreqInMHz) {
			Fsc = pCh->m_FreqCompTable[0].m_Fsc;
		} else {
			// Idx points to {Freq, Fsc} pair to the "right" of RF frequency
			FreqL = FreqCompTable[Idx-1].m_Freq;
			FscL  = FreqCompTable[Idx-1].m_Fsc;
			FreqR = FreqCompTable[Idx].m_Freq;
			FscR  = FreqCompTable[Idx].m_Fsc;
			Fsc = FscL + (FscR - FscL) * (FreqInMHz - FreqL) / (FreqR - FreqL);
		}
	}

#if LOG_LEVEL > 1
	DTA1XX_LOG(KERN_INFO, "[%d] Ad9789FrequencyResponseCompensation: Freq=%d, Fsc=%d",
			   pCh->m_PortIndex, FreqInMHz, Fsc);
#endif

	// Write compensated Full Scale Current to AD9789
	AD9789_SPI_WR(pCh, AD9789_REG_FULLSCALECURRENT,
				  2, Fsc);

	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ad9789GetRfControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS  Ad9789GetRfControl(
	Channel*  pCh,			// Our channel object
	Int64 *  pRfFreq,
	Int *  pLockStatus)
{
	// Getting the carrier frequency by reading the NCO

	UInt32  I;
	UInt64  RfFreq=0;
	Int  Status=0;

	// Get tuning word from cache
	I  = pCh->m_Nco0FreqTuningWord & 0xFFFFFF;

	// Compute RF frequency. RfFreq=I/(2^24-1) * (2400MHz/16)
	if (I==0) {
#ifdef WINBUILD
		RfFreq = ((UInt64)(0x01000000))*150000000LL / 16777215LL;
#else
		RfFreq = Dta1xxBinUDiv(((UInt64)(0x01000000))*150000000LL,16777215LL,NULL); 
#endif
	} else {
#ifdef WINBUILD
		RfFreq = ((UInt64)(I&0x00FFFFFF))*150000000LL / 16777215LL; 
#else
		RfFreq = Dta1xxBinUDiv(((UInt64)(I&0x00FFFFFF))*150000000LL,16777215LL,NULL); 
#endif
	}

#if LOG_LEVEL > 1
	DTA1XX_LOG(KERN_INFO, "[%d] Ad9789GetRfControl: RfFreq=%I64d, I=%d",
			   pCh->m_PortIndex, RfFreq, I);
#endif

	// set return values
	if (pRfFreq)
		*pRfFreq=RfFreq;
	if (pLockStatus)
		*pLockStatus=1; // NCO generated frequency: PLL's will be always locked

	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ad9789SetModControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS  Ad9789SetModControl(
	Channel*  pCh,			// Our channel object
	Int  ModType,			// Modulation type
	Int  ParXtra0,			// Extra parameter #0
	Int  ParXtra1,			// Extra parameter #1
	Int  ParXtra2)			// Extra parameter #2
{
	NTSTATUS  Status=STATUS_SUCCESS;
	volatile UInt8 *  pTxBase = pCh->m_pRegBase;
	UInt8 InScale = 0x20;
	UInt8 SumScale = 0x36;
	t_ModControl1  m_ModControl1;
	t_ModControl2  m_ModControl2;
	t_QamSrrcConfiguration  m_QamSrrcConfiguration;

	m_ModControl1.All = 0;
	m_ModControl2.All = 0;
	m_QamSrrcConfiguration.All = 0;

#if LOG_LEVEL > 1
	DTA1XX_LOG(KERN_INFO, "[%d] Ad9789SetModControl: ModType=%d, ParXtra0=%d, "
			   "ParXtra1=%d, ParXtra2=%d", pCh->m_PortIndex, ModType, ParXtra0, ParXtra1, ParXtra2);
#endif

	if (Dta1xxIsIqMode(ModType))
	{
		// Modulation types that require I/Q sample mode

		// Use Direct-IQ mode
		m_ModControl1.Fields.m_ModType = DTA1XX_TXMODC_IQDIRBUF;

		// Bypass mapper and channel filter; Use 5 interpolation filters
		AD9789_SPI_WR(pCh, AD9789_REG_BYPASS, 1, 0xC0);

		// SUMNODESCALAR and INPUTSCALAR depend on modulation type
		InScale = 0;
		SumScale = 0;
		switch (ModType) {
		case DTA1XX_MOD_ADTBT: InScale = 0x45; SumScale = 0x36; break;
		case DTA1XX_MOD_ATSC:  InScale = 0x50; SumScale = 0x36; break;
		case DTA1XX_MOD_CMMB:  InScale = 0x50; SumScale = 0x36; break;	// ??
		case DTA1XX_MOD_DMBTH: InScale = 0x32; SumScale = 0x36; break;
		case DTA1XX_MOD_DVBT:  InScale = 0x2E; SumScale = 0x36; break;
		case DTA1XX_MOD_DVBT2: InScale = 0x3A; SumScale = 0x36; break;	// ??
		case DTA1XX_MOD_IQDIRECT_NOLIC:
		case DTA1XX_MOD_IQDIRECT: InScale = 0x3A; SumScale = 0x36; break;
		case DTA1XX_MOD_ISDBT: InScale = 0x3A; SumScale = 0x36; break;
		}

		// Data bus 32-bits, 16-bits signed complex data format
		AD9789_SPI_WR(pCh, AD9789_REG_DATACONTROL,
													   1, 0x78);

		// Output level offset should be 0 for I/Q sample mode
		m_ModControl2.Fields.m_OutpLvlOffset = 0x0;
	}
	else if (Dta1xxIsQamMode(ModType))
	{
		// QAM-A/B/C - Common register settings first

		// QAM mapping
		m_QamSrrcConfiguration.Fields.m_Mapping = Ad9789ModType2QamMapping(ModType, ParXtra0);

		// Roll-off factor
		m_QamSrrcConfiguration.Fields.m_Alpha = Ad9789ModType2Alpha(ModType, ParXtra0);

		// Write mapping/filter settings to config register
		AD9789_SPI_WR(pCh, AD9789_REG_QAMSRRCCONF,
					  sizeof(m_QamSrrcConfiguration),
					  m_QamSrrcConfiguration.All);

		if (Dta1xxIsQamB(ModType, ParXtra0))
		{
			// QAM-B

			// Use I/Q mapping mode
			m_ModControl1.Fields.m_ModType = DTA1XX_TXMODC_IQMAP;

			// Use optimised I/Q mapping for 16-QAM, 32-QAM and 64-QAM
			// NOTE: 16-QAM and 32-QAM are not used for QAM-B, so could probably
			// be removed
			switch (ModType) {
			case DTA1XX_MOD_QAM16:
				m_ModControl1.Fields.m_IqMap = DTA1XX_IQMAP_QAM16; break;
			case DTA1XX_MOD_QAM32:
				m_ModControl1.Fields.m_IqMap = DTA1XX_IQMAP_QAM32; break;
			case DTA1XX_MOD_QAM64:
				m_ModControl1.Fields.m_IqMap = DTA1XX_IQMAP_QAM64; break;
			// Generic map otherwise
			default:	m_ModControl1.Fields.m_IqMap = DTA1XX_IQMAP_QAM; break;
													
			}

			// Bypass mapper and enable channel filter; Use 4 interpolation filters
			AD9789_SPI_WR(pCh, AD9789_REG_BYPASS, 1, 0x90);

			SumScale = 0x2D;
			switch (ModType) {
			case DTA1XX_MOD_QAM64:  SumScale = 0x36; break;
			case DTA1XX_MOD_QAM256: SumScale = 0x36; break;
			}

			// Data bus 32-bits, 8-bits signed data, complex mode; no latency
			AD9789_SPI_WR(pCh, AD9789_REG_DATACONTROL,
													   1, 0x68);
		}
		else
		{
			// Hard QAM-A/C

			// For QAM, DTA1XX_TXMODC_ and DTA1XX_MOD_ constants are the same
			m_ModControl1.Fields.m_ModType = ModType;

			// Enable mapper and channel filter; Use 4 interpolation filters
			AD9789_SPI_WR(pCh, AD9789_REG_BYPASS, 1, 0x10);
				
			switch (ModType) {
			case DTA1XX_MOD_QAM4:   SumScale = 0x2D; break;
			case DTA1XX_MOD_QAM16:  SumScale = 0x2D; break;
			case DTA1XX_MOD_QAM32:  SumScale = 0x35; break;
			case DTA1XX_MOD_QAM64:  SumScale = 0x36; break;
			case DTA1XX_MOD_QAM128: SumScale = 0x4E; break;
			case DTA1XX_MOD_QAM256: SumScale = 0x36; break;
			}

			// Data bus 32-bits, 8-bits unsigned data; no latency
			AD9789_SPI_WR(pCh, AD9789_REG_DATACONTROL,
													   1, 0xE0);
		}

		// Output level offset should be +3 for QAM-based modulation types
		m_ModControl2.Fields.m_OutpLvlOffset = 0x3;
	}
	else
		return STATUS_INVALID_PARAMETER;

	// Set scalers
#if LOG_LEVEL > 1
	DTA1XX_LOG(KERN_INFO, "[%d] Ad9789SetModControl: InScale=%d, SumScale=%d",
			   pCh->m_PortIndex, InScale, SumScale);
#endif
	AD9789_SPI_WR(pCh, AD9789_REG_SUMNODESCALAR,
												 1, SumScale);
	AD9789_SPI_WR(pCh, AD9789_REG_INPUTSCALAR, 1, InScale);

	// Write modulation control register to hardware
	Dta1xxTxSetModControlReg(pTxBase,
		                     (Int)(m_ModControl1.All));

	// Write 'new' offset-level to hardware
	Dta1xxTxSetModControl2Reg(pTxBase,
		                      (Int)(m_ModControl2.All));


	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ad9789SetRfControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS  Ad9789SetRfControl(
	Channel*  pCh,			// Our channel object
	Int64  RfFreq)
{
	Int I, C;
	NTSTATUS  Status=STATUS_SUCCESS;

	// Setting the carrier frequency requires the initialisation of the NCO and
	// Band-pass-filter

	// Compute NCO increment I. I=RfFreq/(2400MHz/16) * (2^24-1)
    //I = (Int)((RfFreq*16777215LL) / 150000000LL) & 0x00FFFFFF;
    // 16777215/150000000 = (1118481/78125)/2^7
#ifdef WINBUILD
	I = (Int)((RfFreq*1118481LL/78125LL)>>7) & 0x00FFFFFF;
#else
	I = (Int)(Dta1xxBinUDiv(RfFreq*1118481LL,78125LL,NULL)>>7) & 0x00FFFFFF;
#endif
	// Compute BPF value C. C=RfFreq/(2400MHz) * (2^16-1)
    // C = (Int)((RfFreq*65535LL) / 2400000000LL) & 0x0000FFFF;
    // 65535/2400000000 = (4369/78125)2^11
#ifdef WINBUILD
    C = (Int)((RfFreq*4369LL/78125LL)>>11) & 0x0000FFFF;
#else
    C = (Int)(Dta1xxBinUDiv(RfFreq*4369LL,78125LL,NULL)>>11) & 0x0000FFFF;
#endif

#if LOG_LEVEL > 1
	DTA1XX_LOG(KERN_INFO, "[%d] Ad9789SetRfControl: RfFreq=%I64d, I=%d, C=%d",
			   pCh->m_PortIndex, RfFreq, I, C);
#endif

	// Set tuning word
	pCh->m_Nco0FreqTuningWord = I; // cache tuning word
	AD9789_SPI_WR(pCh, AD9789_REG_NCO0FREQTUNEWORD,
									3,
									pCh->m_Nco0FreqTuningWord);

	// Set Band-Pass-Filter
	AD9789_SPI_WR(pCh, AD9789_REG_INTERPOLBPFCENTERFREQ,
									2,
									C);

	// Finally set update bit
	return Ad9789SpiRegisterWrite(pCh, AD9789_REG_FREQUPDATE,
								  1,
								  0x80);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ad9789SetRfMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS  Ad9789SetRfMode(
	Channel*  pCh,			// Our channel object
	Int RfMode)				// Desired RF-mode
{
	Int  Mode = 0;
	NTSTATUS  Status = STATUS_SUCCESS;
	t_ChannelEnable  m_ChannelEnable;

#if LOG_LEVEL > 1
	DTA1XX_LOG(KERN_INFO, "[%d] Ad9789SetRfMode: RfMode=%d", pCh->m_PortIndex, RfMode);
#endif

	// Enable/disable AD9789 channel based on requested mode. NOTE: channel 1-3
	// are always disabled
	m_ChannelEnable.All = 0;

	Mode = RfMode & ~DTA1XX_UPCONV_SPECINV;
	switch (Mode) {

	case DTA1XX_UPCONV_CW:
	case DTA1XX_UPCONV_CWI:
	case DTA1XX_UPCONV_CWQ:
	case DTA1XX_UPCONV_NORMAL:
		// Enable channel 0
		m_ChannelEnable.Fields.m_ChanEn0 = 1;
		break;

	case DTA1XX_UPCONV_MUTE:
		// Fully disable channel
		m_ChannelEnable.Fields.m_ChanEn0 = 0;
		break;
	
	default:
		return STATUS_INVALID_PARAMETER;
	}

	// Write channel enable state to AD9789
	AD9789_SPI_WR(pCh, AD9789_REG_CHANNELENA,
				  1,
				  m_ChannelEnable.All);

	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ad9789SetSymSampleRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS  Ad9789SetSymSampleRate(
	Channel*  pCh,			// Our channel object
	Int  SymSamplRate)
{
	Int P, Q;
	NTSTATUS  Status = STATUS_SUCCESS;

	// Compute P and Q for symbol/sample rate. P = 4687500 / SymOrSampRate * Q
	// NOTE: For now we use a fixed Q of 0xFFFFFF
	Q = 0xFFFFFF;

	// catch parameters that result in a overflow condition
	// minimal SymOrSampRate = Q*4687500/0x00FFFFFF = 4577637
	if (SymSamplRate<4577637) 
			return STATUS_INVALID_PARAMETER;

	// calculate P using round(x) = (128*(x)+64)/128 = ( 4687500*128*Q / SymOrSampRate + 64 ) / 128
#ifdef WINBUILD
	P = (UInt32)((((UInt64)600000000LL)*((UInt64)Q)/((UInt64)SymSamplRate)+((UInt64)64LL))>>7);
#else
	P = (UInt32)((Dta1xxBinUDiv(((UInt64)Q)*600000000LL,(UInt64)SymSamplRate,NULL)+64LL)>>7);
#endif

	// recalculate rate using round(x) = (128*(x)+64)/128 = ( 4687500*128*Q / P + 64 ) / 128
#ifdef WINBUILD
	pCh->m_TsSymOrSampRate = (UInt32)((((UInt64)600000000LL)*((UInt64)Q)/((UInt64)P)+((UInt64)64LL))>>7);
#else
	pCh->m_TsSymOrSampRate = (UInt32)((Dta1xxBinUDiv(((UInt64)Q)*600000000LL,(UInt64)P,NULL)+64LL)>>7);
#endif

#if LOG_LEVEL > 1
	DTA1XX_LOG(KERN_INFO, "[%d] Ad9789SetSymSampleRate: SymSamplRate=%d, P=0x%08X, Q=0x%08X",
			   pCh->m_PortIndex, SymSamplRate, P, Q);
#endif

	// Set P
	AD9789_SPI_WR(pCh, AD9789_REG_RATECONVNUMP, 3, P);

	// Set Q
	AD9789_SPI_WR(pCh, AD9789_REG_RATECONVDENOMQ, 3, Q);

	// Set update bit
	AD9789_SPI_WR(pCh, AD9789_REG_FREQUPDATE, 1, 0x80);

	return STATUS_SUCCESS;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ AD9789 helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ad9789Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS  Ad9789Init(Channel* pCh)
{
	NTSTATUS  Status = STATUS_SUCCESS;

#if LOG_LEVEL > 0
	DTA1XX_LOG(KERN_INFO, "[%d] Ad9789Init: ENTER", pCh->m_PortIndex);
#endif

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Init all AD9789 registers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	// First setup SI control register
	AD9789_SPI_WR(pCh, AD9789_REG_SPICONTROL, 1, 0x18);
	
	// Avoid transients; disable all channels for now 
	AD9789_SPI_WR(pCh, AD9789_REG_CHANNELENA, 1, 0x00);
	
	// fDCO = 37.5MHz
	AD9789_SPI_WR(pCh, AD9789_REG_DCOFREQ, 1, 0x4F);
	// Phase internal clocks
	AD9789_SPI_WR(pCh, AD9789_REG_INTERNALCLKPHASEADJUST,
				  1, 0x07);
	
	// Enable duty-cycle correction
	AD9789_SPI_WR(pCh, AD9789_REG_MUDELAYCONTROL1,
				  1, 0x80);

	// set common-mode input level clock receiver
	AD9789_SPI_WR(pCh, AD9789_REG_CLKRECEIVER1,
				  1, 0xF0);
	AD9789_SPI_WR(pCh, AD9789_REG_CLKRECEIVER2,
				  1, 0x9E);
	
	// Synchronization sequence as advised by ADI
	AD9789_SPI_WR(pCh, AD9789_REG_PHASEDETECTORCONTROL,
				  1, 0x38);
	
	// Update params
	AD9789_SPI_WR(pCh, AD9789_REG_PARAMETERUPDATE,
				  1, 0x00);
	AD9789_SPI_WR(pCh, AD9789_REG_PARAMETERUPDATE,
				  1, 0x80);

	AD9789_SPI_WR(pCh, AD9789_REG_MUDELAYCONTROL1,
				  1, 0xCE);
	AD9789_SPI_WR(pCh, AD9789_REG_MUDELAYCONTROL2,
				  1, 0x42);
	AD9789_SPI_WR(pCh, AD9789_REG_MUDELAYCONTROL3,
				  2, 0x6C4E);
	
	// Disable all interrupts 
	AD9789_SPI_WR(pCh, AD9789_REG_INTENA, 1, 0x00);
	// Clear all pending interrupts
	AD9789_SPI_WR(pCh, AD9789_REG_INTSSTATUSANDCLR,
				  1, 0xFF);			
	// Enable LockAcq and LockLost Interrupts
	AD9789_SPI_WR(pCh, AD9789_REG_INTENA, 1, 0x0C);

	// Enable mu-delay controller
	AD9789_SPI_WR(pCh, AD9789_REG_MUDELAYCONTROL1,
				  1, 0x43);

	// Wait atleast 1ms to allow the DLL to lock
	Dta1xxWaitMs(1);

	// Sample clock phase
	AD9789_SPI_WR(pCh, AD9789_REG_MUDELAYCONTROL2,
				  1, 0x4B);
	AD9789_SPI_WR(pCh, AD9789_REG_MUDELAYCONTROL2,
				  1, 0x43);

	// Register 0x04 should read 0x08 for the DLL to be locked
	// Register 0x39 should read 0x4E for the correct clock phase

	// Channelizer mode, variable priority, disable parity check
	AD9789_SPI_WR(pCh, AD9789_REG_ITFCONF,
				  1, 0xC0);
	// Data bus 32-bits, 8-bits unsigned data; no latency
	AD9789_SPI_WR(pCh, AD9789_REG_DATACONTROL,
				  1, 0xE0);
	// Enable mapper, chan. filter; 4 Interpolation Filters
	AD9789_SPI_WR(pCh, AD9789_REG_BYPASS, 1, 0x10);
	// 15% Channel Filter, DVB-C 64-QAM
	AD9789_SPI_WR(pCh, AD9789_REG_QAMSRRCCONF,
			      1, 0x24);
	// Scaler for QAM/OFDM; optimized for single-channel 
	AD9789_SPI_WR(pCh, AD9789_REG_SUMNODESCALAR, 
				  1, 0x36);
	// Input scaler for OFDM and Sym2IQ
	AD9789_SPI_WR(pCh, AD9789_REG_INPUTSCALAR, 1, 0x20);

	// set NCO#0 to 474MHz RF frequency
	pCh->m_Nco0FreqTuningWord = 0x28F5C2; // cache tuning word
	AD9789_SPI_WR(pCh, AD9789_REG_NCO0FREQTUNEWORD, 3, pCh->m_Nco0FreqTuningWord);
	// Channel is not used; set NCO#1 to 0Hz to save power
	AD9789_SPI_WR(pCh, AD9789_REG_NCO1FREQTUNEWORD, 3, 0x000000);
	// Channel is not used; set NCO#2 to 0Hz to save power
	AD9789_SPI_WR(pCh, AD9789_REG_NCO2FREQTUNEWORD, 3, 0x000000);
	// Channel is not used; set NCO#3 to 0Hz to save power
	AD9789_SPI_WR(pCh, AD9789_REG_NCO3FREQTUNEWORD, 3, 0x000000);
	
	// Set rate converter to 6.89MBd: value for Q
	AD9789_SPI_WR(pCh, AD9789_REG_RATECONVDENOMQ, 3, 0xFFFFFF);
	// Value for P
	AD9789_SPI_WR(pCh, AD9789_REG_RATECONVNUMP, 3, 0xAE2A5A);

	// Set BPF to 474MHz RF frequency
	AD9789_SPI_WR(pCh, AD9789_REG_INTERPOLBPFCENTERFREQ, 2, 0x328F);

	// Update Rate Converter and BPF
	AD9789_SPI_WR(pCh, AD9789_REG_FREQUPDATE,
				  1, 0x80);

	// Gain channel 0 = 1
	AD9789_SPI_WR(pCh, AD9789_REG_CHANNEL0GAIN, 1, 0x80);
	// Gain channel 1 = 0
	AD9789_SPI_WR(pCh, AD9789_REG_CHANNEL1GAIN, 1, 0x00);
	// Gain channel 2 = 0
	AD9789_SPI_WR(pCh, AD9789_REG_CHANNEL2GAIN, 1, 0x00);
	// Gain channel 3 = 0
	AD9789_SPI_WR(pCh, AD9789_REG_CHANNEL3GAIN, 1, 0x00);

	// No spectral inversion
	AD9789_SPI_WR(pCh, AD9789_REG_SPECTERUMSHAPING,
				  1, 0x00);
			
	// Bypass mirror roll-off
	AD9789_SPI_WR(pCh, AD9789_REG_DACBIAS, 1, 0x00);
	
	// DAC in Normal Mode (1st Nyquist zone)
	AD9789_SPI_WR(pCh, AD9789_REG_DACDECODER, 1, 0x00);
	
	// 19.2mA DAC full-scale current
	AD9789_SPI_WR(pCh, AD9789_REG_FULLSCALECURRENT,
				  2, 0x01CE);
	
	// Enable only LockLost Interrupts
	AD9789_SPI_WR(pCh, AD9789_REG_INTENA, 1, 0x04);
	// Clear all pending interrupts
	AD9789_SPI_WR(pCh, AD9789_REG_INTSSTATUSANDCLR,
				  1, 0xFF);			
	
	// Enable channel#0
	AD9789_SPI_WR(pCh, AD9789_REG_CHANNELENA, 1, 0x01);

	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ad9789IsSpiReady -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS  Ad9789IsSpiReady(
	Channel*  pCh,			// [in] Channel object
	Int  Timeout)			// [in] Timeout in ms
{
	Int32 RfdacCtrlReg;
	volatile UInt8 *  pTxBase = pCh->m_pRegBase;

	// Check for valid value (0 - 10000ms)
	if ( Timeout<=0 || Timeout>10000 )
		return STATUS_INVALID_PARAMETER;

	// Get AD9789 control register
	RfdacCtrlReg = Dta1xxTxGetRfdacCtrlReg(pTxBase);

	// Wait for ready bit
	while ( !(RfdacCtrlReg&DTA1XX_TXRFDACCTRL_SPIREADY_MSK) && Timeout>=0 )
	{
		// Wait shortly
		Dta1xxWaitMs(1);
		
		// Re-get AD9789 control register
		RfdacCtrlReg = Dta1xxTxGetRfdacCtrlReg(pTxBase);

		Timeout--;
	}
	return (Timeout>=0) ? STATUS_SUCCESS : STATUS_UNSUCCESSFUL;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ad9789SpiRegisterWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS  Ad9789SpiRegisterWrite(
	Channel*  pCh,			// [in] Channel object
	UInt16  RegAddr,			// [in] SPI register start address
	UInt  NumToWrite,		// [in] Number of bytes to write
	UInt32  Data)			// [in] Buffer with data to write
{
	NTSTATUS  Status=STATUS_SUCCESS;
	volatile UInt8 *  pTxBase = pCh->m_pRegBase;
	t_Ad9789Control  m_Ad9789Control;

	m_Ad9789Control.All = 0;

	// Check for valid values
	if ( NumToWrite<1 || NumToWrite>4 )
		return STATUS_INVALID_PARAMETER;

	// Check previous SPI-transfer has been completed
	Status = Ad9789IsSpiReady(pCh, AD9789_SPI_TIMEOUT);
	if ( Status!=STATUS_SUCCESS )
		return Status;

	// First write data to data register
#if LOG_LEVEL > 1
	DTA1XX_LOG(KERN_INFO, "[%d] Ad9789SpiRegisterWrite: RegAddr=0x%04X, NumToWrite=%d, "
			   "Data=0x%08X", pCh->m_PortIndex, RegAddr, NumToWrite, Data);
#endif
	Dta1xxTxSetRfdacDataReg(pTxBase,Data);

	// Setup control register for a write transfer
	// NOTE: In MSB bit first mode addresses will decrement, so we must start the 
	// transfer at MSB address
	m_Ad9789Control.Fields.m_SpiStart	= 1;
	m_Ad9789Control.Fields.m_SpiNumBytes = (NumToWrite-1);
	m_Ad9789Control.Fields.m_SpiRdOrWrn = 0;
	m_Ad9789Control.Fields.m_SpiAddr = RegAddr+(NumToWrite-1);

	Dta1xxTxSetRfdacCtrlReg ( pTxBase,
									  (Int)(m_Ad9789Control.All));

	// Wait for transfer to complete
	Status = Ad9789IsSpiReady(pCh, AD9789_SPI_TIMEOUT);
	if ( Status!=STATUS_SUCCESS )
		return Status;

	return STATUS_SUCCESS;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ MISCELLANEOUS HELPERS +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ad9789ModType2Alpha -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Converts modulation type to the corresponding alpha (roll-off) setting 
//
UInt8  Ad9789ModType2Alpha(
	Int  ModType,
	Int  ParXtra0)
{
	BOOLEAN IsQamA=FALSE, IsQamB=FALSE, IsQamC=FALSE;

	// Check for QAM-A, B or C
	IsQamA = Dta1xxIsHardQamA(ModType, ParXtra0);
	IsQamB = Dta1xxIsQamB(ModType, ParXtra0);
	IsQamC = Dta1xxIsHardQamC(ModType, ParXtra0);

	// Set roll-off
	if ( IsQamA )
		return 0x2; // QAM-A: 15% rolloff
	else if ( IsQamC )
		return 0x3; // QAM-C: 13% rolloff
	else /* if ( IsQamB ) */
	{
		// QAM-B: 12% rolloff for 256-QAM or 18% for 64-QAM
		return (ModType==DTA1XX_MOD_QAM256) ? 0x0 : 0x1;
	}
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Ad9789ModType2QamMapping -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Converts modulation type to the corresponding QAM mapping setting
//
UInt8  Ad9789ModType2QamMapping(
	Int  ModType,
	Int  ParXtra0)
{
	BOOLEAN IsQamA=FALSE, IsQamB=FALSE, IsQamC=FALSE;

	// Check for QAM-A, B or C
	IsQamA = Dta1xxIsHardQamA(ModType, ParXtra0);
	IsQamB = Dta1xxIsQamB(ModType, ParXtra0);
	IsQamC = Dta1xxIsHardQamC(ModType, ParXtra0);
				
	// Set mapping
	switch ( ModType )
	{
	// NOTE: we use QAM16 mapping for QAM-4 (as a trick-mode)
	case DTA1XX_MOD_QAM4:	return 0x2; 
	case DTA1XX_MOD_QAM16:	return 0x2; 
	case DTA1XX_MOD_QAM32:	return 0x3; 
	case DTA1XX_MOD_QAM64:	return IsQamB ? 0x0 : 0x4; // Use DOCSIS 64-QAM for QAM-B
	case DTA1XX_MOD_QAM128:	return 0x5;
	case DTA1XX_MOD_QAM256:	return IsQamB ? 0x1 : 0x6; // Use DOCSIS 256-QAM for QAM-B
	}

	return 0x4; // By default assume QAM-64
}

