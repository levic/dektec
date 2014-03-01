//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DacVga.c *#*#*#*#*#*#*#*#*# (C) 2009-2010 DekTec
//
// Contains DAC controlled VGA specific code

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  2010.03.31  AT  Created

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "../Include/Dta1xx.h"
#include "../Include/Dta1xxRegs.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Ad9789 register offset +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Internal functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DacVgaFrequencyResponseCompensation -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Apply frequency-response compensation, given the frequency stored in the channel data.
// Full-scale-current is interpolated linearly between two points in this table.

NTSTATUS DacVgaFrequencyResponseCompensation(Channel * pCh)
{
    Int FreqInMHz, FreqL, FreqR, AttnL, AttnR, Attn, Idx, DacValue;
	volatile UInt8 *  pTxBase = pCh->m_pRegBase;

    if (!pCh) return STATUS_INVALID_PARAMETER;


	Attn = 0;
	if (pCh->m_FreqCompTable!=NULL && pCh->m_FreqCompTableSize>=2) {
		// Frequency in MHz
#if defined(WIN32) || defined(WIN64)
		FreqInMHz = (Int)(pCh->m_RfFreq / 1000000LL);
#else
		FreqInMHz = (Int)(Dta1xxBinDiv(pCh->m_RfFreq, 1000000LL, 0));
#endif

		// Find frequency in compensation table
		for (Idx=1; Idx<pCh->m_FreqCompTableSize; Idx++)
			if (pCh->m_FreqCompTable[Idx].m_Freq > FreqInMHz)
				break;

		if (Idx>=pCh->m_FreqCompTableSize) {
			Attn = pCh->m_FreqCompTable[pCh->m_FreqCompTableSize-1].m_Attn;
		} else if (pCh->m_FreqCompTable[0].m_Freq >= FreqInMHz) {
			Attn = pCh->m_FreqCompTable[0].m_Attn;
		} else {
			// Idx points to {Freq, Fsc} pair to the "right" of RF frequency
			// use TRUNC( (f(x)*256+128) / 256 ) for rounding
			FreqL = pCh->m_FreqCompTable[Idx-1].m_Freq;
			AttnL = pCh->m_FreqCompTable[Idx-1].m_Attn;
			FreqR = pCh->m_FreqCompTable[Idx].m_Freq;
			AttnR = pCh->m_FreqCompTable[Idx].m_Attn;
			Attn  = AttnL + (AttnR - AttnL) * (FreqInMHz - FreqL) / (FreqR - FreqL);
		}
	}

    // calculate DAC value
    DacValue = pCh->m_OutpLeveldBm - Attn - pCh->m_OutpLevelOffset;
    DacValue = DacValue * pCh->m_OutpLevelGain;
    DacValue = (DacValue+512) >> 10;         // round value to integer
    
    // limit DAC value
    if (DacValue < 0) DacValue = 0;
    else if (DacValue > 1023) DacValue = 1023;
    
#if LOG_LEVEL > 1
    DTA1XX_LOG(KERN_INFO,
        "[%d] DacVgaFrequencyResponseCompensationgrep: OutpLeveldBm=%d, FreqInMHz=%d, Attn=%d cBm, DacValue=%d",
        pCh->m_PortIndex, pCh->m_OutpLeveldBm, FreqInMHz, Attn, DacValue);
#endif

    // Write compensated DAC value
    Dta1xxTxRfCtrl2RegSetOutputLevel10bOffset(pTxBase,DacValue);
    
    return STATUS_SUCCESS;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DacVgaGetOutputLevel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS DacVgaGetOutputLevel(Channel * pCh, Int * pOutputLevel)
{
    if (!pCh && !pOutputLevel) return STATUS_INVALID_PARAMETER;

    *pOutputLevel = pCh->m_OutpLeveldBm;

#if LOG_LEVEL > 1
    DTA1XX_LOG ( KERN_INFO, "[%d] DacVgaGetOutputLevel: OutputLevel=%d",
        pCh->m_PortIndex, *pOutputLevel);
#endif

    return STATUS_SUCCESS;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DacVgaSetOutputLevel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS DacVgaSetOutputLevel(Channel * pCh, Int OutputLevel)
{
	if (!pCh) return STATUS_INVALID_PARAMETER;

#if LOG_LEVEL > 1
    DTA1XX_LOG(KERN_INFO, "[%d] DacVgaSetOutputLevel: OutputLevel=%d",
        pCh->m_PortIndex, OutputLevel);
#endif

    pCh->m_OutpLeveldBm = OutputLevel;
    DacVgaFrequencyResponseCompensation(pCh);

    return STATUS_SUCCESS;
}
