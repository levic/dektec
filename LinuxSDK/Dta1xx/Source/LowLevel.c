//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* LowLevel.c *#*#*#*#*#*#*#*#* (C) 2000-2007 DekTec

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

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ TRANSPORT-STREAM RECEIVE CHANNEL +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxRxReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Issue soft reset of receive channel.
//
Int  Dta1xxRxReset(
	Channel*  pCh,			// Transport-Stream Receive Channel
    Int  ResetMode)			// DTAPI_FIFO_RESET / DTAPI_FULL_RESET
{
        Int  TypeNumber = pCh->m_pFdo->m_TypeNumber;
        Int  FirmwareVersion = pCh->m_pFdo->m_FirmwareVersion;

        // ResetMode determines which reset bit is used
        if (ResetMode == DTA1XX_RST_CLEARFIFO) {

                // For old revisions: Use mode DTA1XX_RST_CHANNEL with a recursive call
                if (   (TypeNumber==100 && FirmwareVersion<=2)
                        || (TypeNumber==102 && FirmwareVersion<=4)) {
                        Dta1xxRxReset(pCh, DTA1XX_RST_CHANNEL);
                }
                else {
                        // Card is sufficiently up-to-date to just reset the FIFO
                        Dta1xxRxCtrlRegClrFifo(pCh->m_pRegBase);
                }
        }
        else if (ResetMode == DTA1XX_RST_CHANNEL) {

                // Full reset on DTA-100/102, Transmit-Channel reset on new boards (DTA-140...)
                if (TypeNumber==100 || TypeNumber==102)
                        Dta1xxGenCtrlRegReset(pCh->m_pRegBase);
                else
                        Dta1xxRxCtrlRegRxReset(pCh->m_pRegBase);
        }
        return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxRxSetRxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set the channel's packet receive mode.
//
// Not implemented (yet). This is now only used to tell DVB-SPI about the RxMode
//
Int  Dta1xxRxSetRxMode(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int PortIndex,			// Port index
	Int RxMode)				// Packet receive mode
{
	Channel*  pCh;			// Channel pointer

	// Check port index
	if (PortIndex < 0 || PortIndex >= pFdo->m_NumNonIpChannels) {
		DTA1XX_LOG(KERN_INFO, "Dta1xxRxSetRxMode: PortIndex=%d INVALID", PortIndex);
		return -EFAULT;
	}

#if LOG_LEVEL > 0
	DTA1XX_LOG(KERN_INFO, "[%d] Dta1xxRxSetRxMode: RxMode=0x%x", PortIndex, RxMode);
#endif

	// Set channel pointer and pointer to transmit registers
	pCh = &pFdo->m_Channel[PortIndex];
	
	// Set receiver mode on DVB-SPI interface
	if ( pCh->m_Capability&DTA1XX_CHCAP_SPI )
		return Dta1xxSpiSetRxModeDvb(pCh, RxMode);

	return 0;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+ TRANSPORT-STREAM TRANSMIT CHANNEL +=+=+=+=+=+=+=+=+=+=+=+=+=+


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxTxDeriveModPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Try to derive current modulation parameters (m_ModType, m_XtraPar[]) for
// DTA-107/107S2/110/110T/115.
// If no meaningful value for the parameters can be derived, parameters are set to -1.
// Called from Dta1xxInitDeviceExtension.
//
void  Dta1xxTxDeriveModPars(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  PortIndex)			// Port index
{
	Int  ModTypeReg = -1;
	Int  RollOff = -1;
	Channel*  pCh;
	Int  TypeNumber = pFdo->m_TypeNumber;
	BOOLEAN  Is107 = (TypeNumber==107);
	BOOLEAN  Is11X = (   TypeNumber==110 || TypeNumber==111 || TypeNumber==112
					  || TypeNumber==115 || TypeNumber==116 || TypeNumber==117);
	BOOLEAN  Ad9789Cap;

	// Check typenumber and port index for validity
	if ( PortIndex<0 || PortIndex>pFdo->m_NumNonIpChannels )
		return;
	
	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// Check typenumber, or capability, for validity
	Ad9789Cap = (pCh->m_Capability&DTA1XX_CHCAP_AD9789) != 0;
	Is11X = Is11X || Ad9789Cap;

	// Initialise shadow variables to -1
	pCh->m_ModType = -1;
	pCh->m_XtraPar[0] = -1;
	pCh->m_XtraPar[1] = -1;
	pCh->m_XtraPar[2] = -1;

	// Read "low-level" Modulation-Type register from hardware
	// The register value cannot be mapped 1-to-1 to DTAPI modulation type!
	ModTypeReg = Dta1xxTxModcRegGetModType(pCh->m_pRegBase);

	// Read roll-off factor
	if (Is11X)
		RollOff = Dta1xxTxModcRegGetRollOffFactor(pCh->m_pRegBase);

#if LOG_LEVEL_MODULATION > 1
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxDeriveModPars: ModTypeReg=%d RollOff=%d",
				PortIndex, ModTypeReg, RollOff );
#endif

	// Derive modulation shadow variables from register values
	switch (ModTypeReg) {

	case DTA1XX_TXMODC_QPSK:
		if (Is107) {
			// DTA-107 in DVB-S QPSK mode: retrieve code rate from hardware
			pCh->m_ModType = DTA1XX_MOD_DVBS_QPSK;
			pCh->m_XtraPar[0] = Dta1xxTxModcRegGetCodeRate(pCh->m_pRegBase);
		}
		// else: QPSK, but not a DTA-107
		// pCh->m_ModType remains -1;
		break;

		// QAM MODES
		// We make use of the fact that DTA1XX_MOD_QAMnnn constant values are equal
		// to the corresponding DTA1XX_TXMODC_QAMnnn values.
		// The J.83 annex is derived from the roll-off factor
		// Code is separate for QAM64 and QAM256, because these have an Annex B variant
	case DTA1XX_TXMODC_QAM4:
	case DTA1XX_TXMODC_QAM16:
	case DTA1XX_TXMODC_QAM32:
	case DTA1XX_TXMODC_QAM128:
		if (Is11X) {
			pCh->m_ModType = ModTypeReg;
			switch (RollOff) {
			case DTA1XX_ROLLOFF_12:	pCh->m_XtraPar[0] = -1; break;
			case DTA1XX_ROLLOFF_13:	pCh->m_XtraPar[0] = DTA1XX_MOD_J83_C; break;
			case DTA1XX_ROLLOFF_15:	pCh->m_XtraPar[0] = DTA1XX_MOD_J83_A; break;
			case DTA1XX_ROLLOFF_18:	pCh->m_XtraPar[0] = -1; break;
			}
		}
		break;

	case DTA1XX_TXMODC_QAM64:
		if (Is11X) {
			pCh->m_ModType = ModTypeReg;
			switch (RollOff) {
			case DTA1XX_ROLLOFF_12:	pCh->m_XtraPar[0] = -1; break;
			case DTA1XX_ROLLOFF_13:	pCh->m_XtraPar[0] = DTA1XX_MOD_J83_C; break;
			case DTA1XX_ROLLOFF_15:	pCh->m_XtraPar[0] = DTA1XX_MOD_J83_A; break;
			case DTA1XX_ROLLOFF_18:	pCh->m_XtraPar[0] = DTA1XX_MOD_J83_B; break;
			}
		}
		break;

	case DTA1XX_TXMODC_QAM256:
		if (Is11X) {
			pCh->m_ModType = ModTypeReg;
			switch (RollOff) {
			case DTA1XX_ROLLOFF_12:	pCh->m_XtraPar[0] = DTA1XX_MOD_J83_B; break;
			case DTA1XX_ROLLOFF_13:	pCh->m_XtraPar[0] = DTA1XX_MOD_J83_C; break;
			case DTA1XX_ROLLOFF_15:	pCh->m_XtraPar[0] = DTA1XX_MOD_J83_A; break;
			case DTA1XX_ROLLOFF_18:	pCh->m_XtraPar[0] = -1; break;
			}
		}
		break;

	case DTA1XX_TXMODC_IQMAP:
		if (Is11X) {
			// Use roll-off factor for an intelligent (?) guess
			switch (RollOff) {
			case DTA1XX_ROLLOFF_12:
				pCh->m_ModType = DTA1XX_MOD_QAM256;
				pCh->m_XtraPar[0] = DTA1XX_MOD_J83_B;
				break;
			case DTA1XX_ROLLOFF_13:
			case DTA1XX_ROLLOFF_15:
				pCh->m_ModType = DTA1XX_MOD_QAM64;
				pCh->m_XtraPar[0] = DTA1XX_MOD_J83_C | DTA1XX_MOD_DVBC_SOFT;
				break;
			case DTA1XX_ROLLOFF_18:
				pCh->m_ModType = DTA1XX_MOD_QAM64;
				pCh->m_XtraPar[0] = DTA1XX_MOD_J83_B;
				break;
			}
		}
		if (Is107) {
			// DTA-107 in DVB-S2 mode: we cannot derive other parameters,
			// use default settings
			pCh->m_ModType = DTA1XX_MOD_DVBS2_QPSK;
			pCh->m_XtraPar[0] = DTA1XX_MOD_3_4;
			pCh->m_XtraPar[1] = 0;
		}
		break;

	case DTA1XX_TXMODC_IQDIRECT:
		// No intelligent guess possible, leave shadow variables unitialised
		break;
	}
#if LOG_LEVEL_MODULATION > 0
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxDeriveModPars: ModType=%d XtraPar[]=%d %d %d",
				PortIndex, pCh->m_ModType,
			    pCh->m_XtraPar[0], pCh->m_XtraPar[1], pCh->m_XtraPar[2] );
#endif
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxTxGetModControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get modulation-control parameters.
// Called from DeviceIoControl.
//
Int  Dta1xxTxGetModControl(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  PortIndex,			// Port index
	Int*  pModType,			// Modulation type
	Int*  pXtraPar0,		// Extra modulation-control parameter 0
	Int*  pXtraPar1,		// Extra modulation-control parameter 1
	Int*  pXtraPar2)		// Extra modulation-control parameter 2
{
	Channel*  pCh;
	Int  TypeNumber = pFdo->m_TypeNumber;
	BOOLEAN  Is107 = (TypeNumber==107);
	BOOLEAN  Is11X = (   TypeNumber==110 || TypeNumber==111 || TypeNumber==112
					  || TypeNumber==115 || TypeNumber==116 || TypeNumber==117);
	
	// Check port index
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumNonIpChannels ) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxTxGetModControl: PortIndex=%d INVALID",
					PortIndex );
#endif
		return -EFAULT;
	}

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// channel with modulator capability
	Is11X = Is11X || ((pCh->m_Capability&DTA1XX_CHCAP_AD9789) != 0); 

	// Must be a modulator card
	if (!Is107 && !Is11X)
		return -EFAULT;

	// Check channel type
	if (   pCh->m_ChannelType!=DTA1XX_TS_TX_CHANNEL
		&& pCh->m_ChannelType!=DTA1XX_IP_TX_CHANNEL
		&& pCh->m_ChannelType!=DTA1XX_NW_TX_CHANNEL)
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetModControl: ChannelType=%d INVALID",
					PortIndex, pCh->m_ChannelType );
#endif
		return -EFAULT;
	}

	// Copy modulation parameters from shadow variables in channel data
	*pModType  = pCh->m_ModType;
	*pXtraPar0 = pCh->m_XtraPar[0];
	*pXtraPar1 = pCh->m_XtraPar[1];
	*pXtraPar2 = pCh->m_XtraPar[2];

	// Pretty printing: Useful for easy debugging
	if (Is11X && pCh->m_ModType==DTA1XX_MOD_ATSC) {
#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetModControl: ModType=DTA1XX_MOD_ATSC "
				    "Constellation=%s", PortIndex, Dta1xxAtscConstel2Str(*pXtraPar0) );
#endif
	}
	else if (Is11X && pCh->m_ModType==DTA1XX_MOD_ADTBT) {
#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetModControl: ModType=DTA1XX_MOD_ADTBT",
				    PortIndex );
#endif
	}
	else if (Is11X && pCh->m_ModType==DTA1XX_MOD_DMBTH) {
#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetModControl: ModType=DTA1XX_MOD_DMBTH",
				    PortIndex );
#endif
	}
	else if (Is11X && pCh->m_ModType==DTA1XX_MOD_DVBT) {
#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetModControl: ModType=DTA1XX_MOD_DVBT "
				    "CodeRate=%s", PortIndex, Dta1xxDvbtCodeRate2Str(*pXtraPar0) );
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetModControl: XtraPar1=%s | %s |",
				    PortIndex, Dta1xxDvbtPar2BwStr(*pXtraPar1),
				    Dta1xxDvbtPar2ModeStr(*pXtraPar1) );
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetModControl:          %s | %s |",
				    PortIndex, Dta1xxDvbtPar2GuardStr(*pXtraPar1),
				    Dta1xxDvbtPar2ConstStr(*pXtraPar1) );
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetModControl:          %s",
				    PortIndex, Dta1xxDvbtPar2InDepthStr(*pXtraPar1) );
#endif
	}
	else if (Is11X && pCh->m_ModType==DTA1XX_MOD_DVBT2) {
#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetModControl: ModType=DTA1XX_MOD_DVBT2",
				    PortIndex);
#endif
	}
	else if (Is11X && pCh->m_ModType==DTA1XX_MOD_DVBT2) {
#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetModControl: ModType=DTA1XX_MOD_DVBT2",
					PortIndex );
#endif
	}
	else if (Is11X && pCh->m_ModType==DTA1XX_MOD_ISDBT) {
#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetModControl: ModType=DTA1XX_MOD_ISDBT "
				    "Input=%d", PortIndex, *pXtraPar0 & 1 );
#endif
	} else {
		// Not a DTA-110T/DTA-115 in DTMB, DVB-T or ISDB-T mode
#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxDeriveModPars: ModType=%d XtraPar[]=%d %d %d",
				    PortIndex, *pModType, *pXtraPar0, *pXtraPar1, *pXtraPar2 );
#endif
	}
	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxTxGetTxControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get the channel's transmit-control state.
//
Int  Dta1xxTxGetTxControl(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  PortIndex,			// Port index
	Int*  pTxControl)		// Transmit-control state
{
	Channel*  pCh;

	// Check port index
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumNonIpChannels ) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxTxGetTxControl: PortIndex=%d INVALID", PortIndex );
#endif
		return -EFAULT;
	}

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// Check channel type
	if (   pCh->m_ChannelType!=DTA1XX_TS_TX_CHANNEL
		&& pCh->m_ChannelType!=DTA1XX_IP_TX_CHANNEL
		&& pCh->m_ChannelType!=DTA1XX_NW_TX_CHANNEL) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetTxControl: ChannelType=%d INVALID",
				    PortIndex, pCh->m_ChannelType );
#endif
		return -EFAULT;
	}

	// Get Tx Control
	*pTxControl = Dta1xxTxCtrlRegGetTxCtrl(pCh->m_pRegBase);

#if LOG_LEVEL > 1
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetTxControl: TxControl=%d",
			    PortIndex, *pTxControl );
#endif

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxTxGetTxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get the channel's transmit-control state.
//
Int  Dta1xxTxGetTxMode(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  PortIndex,			// Port index
	Int*  pTxPacketMode,	// Packet transmit mode
	Int*  pTxStuffMode)		// Null-packet stuffing on/off
{
	Channel*  pCh;

	// Check port index
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumNonIpChannels ) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxTxGetTxMode: PortIndex=%d INVALID", PortIndex );
#endif
		return -EFAULT;
	}

	// Set channel pointer and pointer to transmit register block
	pCh = &pFdo->m_Channel[PortIndex];

	// Check channel type
	if (   pCh->m_ChannelType!=DTA1XX_TS_TX_CHANNEL
		&& pCh->m_ChannelType!=DTA1XX_IP_TX_CHANNEL
		&& pCh->m_ChannelType!=DTA1XX_NW_TX_CHANNEL) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetTxMode: ChannelType=%d INVALID",
				     PortIndex, pCh->m_ChannelType );
#endif
		return -EFAULT;
	}


	// Get packet mode and stuff mode
	*pTxPacketMode = Dta1xxTxCtrlRegGetTxMode(pCh->m_pRegBase);
	*pTxStuffMode = Dta1xxTxCtrlRegGetTxPckStuff(pCh->m_pRegBase);

	// Get burst mode for DTA-100/105/112/115/116/117/140/145/160/2160/545/2145
	if (    pCh->m_pFdo->m_TypeNumber==100
		||  pCh->m_pFdo->m_TypeNumber==105
		|| (pCh->m_pFdo->m_TypeNumber==112 && PortIndex==0)
		|| (pCh->m_pFdo->m_TypeNumber==115 && PortIndex==0)
		|| (pCh->m_pFdo->m_TypeNumber==116 && PortIndex==0)
		|| (pCh->m_pFdo->m_TypeNumber==117 && PortIndex==0)
		||  pCh->m_pFdo->m_TypeNumber==140
		||  pCh->m_pFdo->m_TypeNumber==145
		||  pCh->m_pFdo->m_TypeNumber==160
		||  pCh->m_pFdo->m_TypeNumber==2160
		||  pCh->m_pFdo->m_TypeNumber==545
		||  pCh->m_pFdo->m_TypeNumber==2145)
	{
		if ( 0!=Dta1xxTxCtrlRegGetTxBurstMode(pCh->m_pRegBase) )
			*pTxPacketMode |= DTA1XX_TXMODE_BURST;
	}

#if LOG_LEVEL > 1
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxGetTxMode: TxPacketMode=%d TxStuffMode=%d",
			     PortIndex, *pTxPacketMode, *pTxStuffMode );
#endif

	return 0;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxTxReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Issue soft reset of transmit channel.
//
Int  Dta1xxTxReset(
	Channel*  pCh,			// Transport-Stream Transmit Channel
	Int  ResetMode)			// DTAPI_FIFO_RESET / DTAPI_FULL_RESET
{
	Int  TypeNumber = pCh->m_pFdo->m_TypeNumber;
	Int  FirmwareVersion = pCh->m_pFdo->m_FirmwareVersion;

	// ResetMode determines which reset bit is used
	if (ResetMode == DTA1XX_RST_CLEARFIFO) {

		// For old revisions: Use mode DTA1XX_RST_CHANNEL with a recursive call
		if (   (TypeNumber==100 && FirmwareVersion<=2)
			|| (TypeNumber==102 && FirmwareVersion<=4)) {
			Dta1xxTxReset(pCh, DTA1XX_RST_CHANNEL);
		}
		else {
			// Card is sufficiently up-to-date to just reset the FIFO
			Dta1xxTxCtrlRegClrFifo(pCh->m_pRegBase);
		}
	}
	else if (ResetMode == DTA1XX_RST_CHANNEL) {

		// Full reset on DTA-100/102, Transmit-Channel reset on new boards (DTA-140...)
		if (TypeNumber==100 || TypeNumber==102)
			Dta1xxGenCtrlRegReset(pCh->m_pRegBase);
		else
			Dta1xxTxCtrlRegTxReset(pCh->m_pRegBase);
	}
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxTxSetModControl2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// THIS FUNCTION IS NOT USED ANY MORE
//
// Successor of Dta1xxTxSetModControl
//
// Set modulation-control parameters
// Modulation-control parameters are stored in the device extension, so that if a device
// is closed, the next process opening the device can recover (eg) DVB-T parameters
//
// NOTE: This routine used to call Dta1xxTxSetTsRate to set symbol- or sample rate, but
//       this code has been removed to make things less fuzzy.
//       Currently Dta1xxTxSetModControl does not set any rates.
//
// The modulation type is mapped onto a value for the modulation-control register:
//
// DTA1XX_MOD_DVBS_QPSK	Set modulation-type register to DTA1XX_TXMODC_QPSK
//
// DTA1XX_MOD_DVBS_QPSK_SOFT, DTA1XX_MOD_DVBS2_QPSK, DTA1XX_MOD_DVBS2_8PSK
//						Set modulation-type register to DTA1XX_TXMODC_IQMAP
//
// DTA1XX_MOD_ATSC, DTA1XX_MOD_DTMB, DTA1XX_MOD_DVBT, DTA1XX_MOD_DVBT2,  DTA1XX_MOD_ISDBT
//						Set modulation-type register to DTA1XX_TXMODC_IQDIRECT
//
// DTA1XX_MOD_QAMnnn, "Hard" QAM-A or QAM-C
//						Set modulation-type register to DTA1XX_TXMODC_QAMnnn
//						Set roll-off factor to 13% or 15%
//
// DTA1XX_MOD_QAMnnn, "Soft QAM-A or QAM-C", or QAM-B (always soft)
//						Set modulation-type register to DTA1XX_TXMODC_IQMAP
//						Set I/Q mapping to DTA1XX_IQMAP_QAM
//						Set roll-off factor to 12%, 13%, 15% or 18%
//
Int  Dta1xxTxSetModControl2(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  PortIndex,			// Port index
	Int  ModType,			// Modulation type
	Int  XtraPar0,			// Extra modulation-control parameter 0
	Int  XtraPar1,			// Extra modulation-control parameter 1
	Int  XtraPar2)			// Extra modulation-control parameter 2
{
	Int  IqMap, RollOff;
	Int  TypeNumber = pFdo->m_TypeNumber;
	Channel*  pCh;
	BOOLEAN  Is107 = (TypeNumber==107);
	BOOLEAN  Is111 = (TypeNumber==111);
	BOOLEAN  Is112_115_116_117 = (   TypeNumber==112 || TypeNumber==115
								  || TypeNumber==116 || TypeNumber==117);
	BOOLEAN  Is11X = (TypeNumber==110 || Is111 || Is112_115_116_117);

	// Check port index
	if (PortIndex>=pFdo->m_NumNonIpChannels || (Is112_115_116_117 && (PortIndex==0))) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxTxSetModControl2: PortIndex=%d INVALID",
					PortIndex );
#endif
		return -EFAULT;
	}

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// old style: Check type number: must be a modulator card
	if (!Is107 && !Is11X) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxTxSetModControl2: TypeNumber=%d ILLEGAL",
					TypeNumber );
#endif
		return -EFAULT;
	}

	// Check channel type
	if (   pCh->m_ChannelType!=DTA1XX_TS_TX_CHANNEL
		&& pCh->m_ChannelType!=DTA1XX_IP_TX_CHANNEL
		&& pCh->m_ChannelType!=DTA1XX_NW_TX_CHANNEL) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetModControl2: ChannelType=%d INVALID",
				    PortIndex, pCh->m_ChannelType );
#endif
		return -EFAULT;
	}

	// Save modulation type and parameters in shadow variables in device extension
	pCh->m_ModType = ModType;
	pCh->m_XtraPar[0] = XtraPar0;
	pCh->m_XtraPar[1] = XtraPar1;
	pCh->m_XtraPar[2] = XtraPar2;

	// For DTA-111/112/115/116/117: set scaler value to ensure a constant output level
	// accross all modulation types
	if (Is111 || Is112_115_116_117)
        Dta1xxSetModulatorScaler(pCh);


	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ATSC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	if (Is11X && ModType==DTA1XX_MOD_ATSC) {
#if LOG_LEVEL_MODULATION > 0
		// Pretty print new parameters
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetModControl2: ModType=DTA1XX_MOD_ATSC "
				    "Constellation=%s", PortIndex, Dta1xxAtscConstel2Str(XtraPar0) );
#endif
		// Use Direct-IQ mode
		Dta1xxTxModcRegSetModType(pCh->m_pRegBase, DTA1XX_TXMODC_IQDIRECT);
		// DTA-111/112/115/116/117: Select QAM interpolation filter (is this OK??)
		if (Is111 || Is112_115_116_117)
			Dta1xxTxModcRegSetIntPolSel(pCh->m_pRegBase, DTA1XX_INTPOL_OFDM);
		return 0;
	}

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTMB -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	if (Is11X && (ModType==DTA1XX_MOD_ADTBT || ModType==DTA1XX_MOD_DMBTH))
	{
#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetModControl2: ModType=DTA1XX_MOD_DTMB",
				    PortIndex );
#endif
		// Use Direct-IQ mode
		Dta1xxTxModcRegSetModType(pCh->m_pRegBase, DTA1XX_TXMODC_IQDIRECT);
		// DTA-111/112/115/116/117: Select OFDM interpolation filter
		if (Is111 || Is112_115_116_117)
			Dta1xxTxModcRegSetIntPolSel(pCh->m_pRegBase, DTA1XX_INTPOL_OFDM);
		return 0;
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DVB-S -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	if (Is107 && ModType==DTA1XX_MOD_DVBS_QPSK) {
#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetModControl2: ModType=%s CodeRate=%d",
				    PortIndex, Dta1xxModType2Str(ModType), XtraPar0 );
#endif
		Dta1xxTxModcRegSetModType(pCh->m_pRegBase, DTA1XX_TXMODC_QPSK);
		Dta1xxTxModcRegSetCodeRate(pCh->m_pRegBase, XtraPar0);
		return 0;
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DVB-S2, Soft DVB-S -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	else if (Is107 && (  ModType==DTA1XX_MOD_DVBS_QPSK_SOFT
					  || ModType==DTA1XX_MOD_DVBS2_QPSK
					  || ModType==DTA1XX_MOD_DVBS2_8PSK)) {
#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetModControl2: ModType=%s XtraPar0=%d",
				    PortIndex, Dta1xxModType2Str(ModType), XtraPar0 );
#endif
		// Use I/Q Mapping mode
		Dta1xxTxModcRegSetModType(pCh->m_pRegBase, DTA1XX_TXMODC_IQMAP);
		return 0;
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DVB-T -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	if (Is11X && ModType==DTA1XX_MOD_DVBT) {
#if LOG_LEVEL_MODULATION > 0
		// Pretty print new parameters
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetModControl2: ModType=DTA1XX_MOD_DVBT "
				    "CodeRate=%s", PortIndex, Dta1xxDvbtCodeRate2Str(XtraPar0));
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetModControl2: XtraPar1=%s | %s |",
				    PortIndex, Dta1xxDvbtPar2BwStr(XtraPar1),
				    Dta1xxDvbtPar2ModeStr(XtraPar1) );
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetModControl2:          %s | %s |",
				    PortIndex, Dta1xxDvbtPar2GuardStr(XtraPar1),
				    Dta1xxDvbtPar2ConstStr(XtraPar1) );
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetModControl2:          %s",
				    PortIndex, Dta1xxDvbtPar2InDepthStr(XtraPar1) );
#endif
		// Use Direct-IQ mode
		Dta1xxTxModcRegSetModType(pCh->m_pRegBase, DTA1XX_TXMODC_IQDIRECT);
		// DTA-111/112/115/116/117: Select OFDM interpolation filter
		if (Is111 || Is112_115_116_117)
			Dta1xxTxModcRegSetIntPolSel(pCh->m_pRegBase, DTA1XX_INTPOL_OFDM);
		return 0;
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DVB-T2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	if (Is11X && ModType==DTA1XX_MOD_DVBT2) {
#if LOG_LEVEL_MODULATION > 0
		// Pretty print new parameters
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetModControl2: ModType=DTA1XX_MOD_DVBT2",
				    PortIndex );
#endif
		// Use Direct-IQ mode
		Dta1xxTxModcRegSetModType(pCh->m_pRegBase, DTA1XX_TXMODC_IQDIRECT);
		// DTA-111/112/115/116/117: Select OFDM interpolation filter
		if (Is111 || Is112_115_116_117)
			Dta1xxTxModcRegSetIntPolSel(pCh->m_pRegBase, DTA1XX_INTPOL_OFDM);
		return STATUS_SUCCESS;
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ISDB-T -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	else if (Is11X && ModType==DTA1XX_MOD_ISDBT) {
#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetModControl2: ModType=DTA1XX_MOD_ISDBT",
				    PortIndex );
#endif
		// Use Direct-IQ mode
		Dta1xxTxModcRegSetModType(pCh->m_pRegBase, DTA1XX_TXMODC_IQDIRECT);
		// DTA-111/112/115/116/117: Select OFDM interpolation filter
		if (Is111 || Is112_115_116_117)
			Dta1xxTxModcRegSetIntPolSel(pCh->m_pRegBase, DTA1XX_INTPOL_OFDM);
		return 0;
	}

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Hard QAM-A or QAM-C -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	else if (Is11X && Dta1xxIsHardQamAorC(pCh)) {
		RollOff = Dta1xxComputeRollOff(pCh);
#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetModControl2: ModType=%s XtraPar0=%d "
				    "RollOff=%d (Hard QAM-A or QAM-C)", PortIndex,
				    Dta1xxModType2Str(ModType), XtraPar0, RollOff );
#endif
		// For QAM, DTA1XX_TXMODC_ and DTA1XX_MOD_ constants are the same
		Dta1xxTxModcRegSetModType(pCh->m_pRegBase, ModType);
		Dta1xxTxModcRegSetRollOffFactor(pCh->m_pRegBase, RollOff);
		// DTA-111/112/115/116/117: Select QAM interpolation filter
		if (Is111 || Is112_115_116_117)
			Dta1xxTxModcRegSetIntPolSel(pCh->m_pRegBase, DTA1XX_INTPOL_QAM);
		return 0;
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.- Soft QAM-A or QAM-C, or QAM-B -.-.-.-.-.-.-.-.-.-.-.-.-.-
	else if (Is11X && Dta1xxIsSoftQam(pCh)) {
		// Use optimised I/Q mapping for 16-QAM, 32-QAM and 64-QAM
		switch (ModType) {
		case DTA1XX_MOD_QAM16: IqMap = DTA1XX_IQMAP_QAM16; break;
		case DTA1XX_MOD_QAM32: IqMap = DTA1XX_IQMAP_QAM32; break;
		case DTA1XX_MOD_QAM64: IqMap = DTA1XX_IQMAP_QAM64; break;
		default:			   IqMap = DTA1XX_IQMAP_QAM; break;	// Generic map otherwise
		}
		RollOff = Dta1xxComputeRollOff(pCh);

#if LOG_LEVEL_MODULATION > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetModControl2: ModType=%s XtraPar0=%d "
				    "IqMap=%d RollOff=%d (Soft QAM-A or QAM-C)", PortIndex,
				    Dta1xxModType2Str(ModType), XtraPar0, IqMap, RollOff );
#endif
		// Use I/Q Mapping mode
		Dta1xxTxModcRegSetModType(pCh->m_pRegBase, DTA1XX_TXMODC_IQMAP);
		Dta1xxTxModcRegSetIqMap(pCh->m_pRegBase, IqMap);
		Dta1xxTxModcRegSetRollOffFactor(pCh->m_pRegBase, RollOff);
		// DTA-111/112/115/116/117: Select QAM interpolation filter
		if (Is111 || Is112_115_116_117)
			Dta1xxTxModcRegSetIntPolSel(pCh->m_pRegBase, DTA1XX_INTPOL_QAM);
		return 0;
	}

	// Modulation mode not supported
#if LOG_LEVEL_MODULATION > 0
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetModControl2: ModType=%d XtraPar0=%d "
			    "UNSUPPORTED", PortIndex, ModType, XtraPar0 );
#endif

	return -EFAULT;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxTxSetTxControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set the channel's transmit-control state.
//
Int  Dta1xxTxSetTxControl(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  PortIndex,			// Port index
	Int  TxControl)			// Transmit-control state
{
	Channel*  pCh;

	// Check port index
	if (PortIndex >= pFdo->m_NumNonIpChannels) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxTxSetTxControl: PortIndex=%d INVALID", PortIndex );
#endif
		return -EFAULT;
	}

#if LOG_LEVEL > 1
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetTxControl: TxControl=0x%03x",
			    PortIndex, TxControl );
#endif

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// Check channel type
	if (   pCh->m_ChannelType!=DTA1XX_TS_TX_CHANNEL
		&& pCh->m_ChannelType!=DTA1XX_IP_TX_CHANNEL
		&& pCh->m_ChannelType!=DTA1XX_NW_TX_CHANNEL) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetTxControl: ChannelType=%d INVALID",
				    PortIndex, pCh->m_ChannelType );
#endif
		return -EFAULT;
	}

	if ( 0==down_interruptible(&pCh->m_MutexTxControl) ) 
	{
		Dta1xxTxCtrlRegSetTxCtrl(pCh->m_pRegBase, TxControl);
		up(&pCh->m_MutexTxControl);
	} 
	else
		return -EFAULT;

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxTxSetTxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set the channel's packet transmit mode.
//
Int  Dta1xxTxSetTxMode(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  PortIndex,			// Port index
	Int  TxPacketMode,		// Packet transmit mode (TxMode, BurstMode)
	Int  TxStuffMode)		// Null-packet stuffing on/off
{
	Int  ClockGenMode;		// Clock-generator mode (not really used)
	Int  NewPckSize;		// Packet size after mode change: 188 / 204
	Int  OldPckSize;		// Packet size before mode change: 188 / 204
	Int  OldTxMode;			// Packet transmit mode before change
	Int  TsRate;			// Transport stream rate
	Int  TxMode;			// Transmit mode (188/192/204/Add16/Raw)
	Channel*  pCh;			// Channel pointer

	// Check port index
	if ( PortIndex < 0 || PortIndex >= pFdo->m_NumNonIpChannels ) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxTxSetTxMode: PortIndex=%d INVALID", PortIndex );
#endif
		return -EFAULT;
	}

#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetTxMode: TxPacketMode=0x%x TxStuffMode=%d",
			    PortIndex, TxPacketMode, TxStuffMode );
#endif

	// Set channel pointer and pointer to transmit registers
	pCh = &pFdo->m_Channel[PortIndex];

	// Check channel type
	if (   pCh->m_ChannelType!=DTA1XX_TS_TX_CHANNEL
		&& pCh->m_ChannelType!=DTA1XX_IP_TX_CHANNEL
		&& pCh->m_ChannelType!=DTA1XX_NW_TX_CHANNEL) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxTxSetTxMode: ChannelType=%d INVALID",
				    PortIndex, pCh->m_ChannelType );
#endif
		return -EFAULT;
	}

	// when we are configuring the outputport in SDI mode
	if ((TxPacketMode & DTA1XX_TXMODE_SDI) != 0) {

		// When the channel is SPI capable
		if (pCh->m_Capability&DTA1XX_CHCAP_SPI) {
			// Set Tx channel rate for standard I/O mode
			Dta1xxSpiSetSdiClock(pCh);
		}

	} // SDI MODE

	// Get transmit mode from nTxPacketMode (AND-off burst-mode bit)
	TxMode = TxPacketMode & 0xF;

	// One thread at a time
	if ( 0!=down_interruptible(&pCh->m_MutexTxControl) )
		return -EFAULT;

	// Determine current packet size
	OldTxMode  = Dta1xxTxCtrlRegGetTxMode(pCh->m_pRegBase);
	OldPckSize = Dta1xxTxMode2PacketSize(OldTxMode);
	NewPckSize = Dta1xxTxMode2PacketSize(TxMode);

	// Read old bit rate if packet size changes
	if (OldPckSize != NewPckSize)
		Dta1xxTxGetRate2(pFdo, PortIndex, &ClockGenMode, &TsRate);

	// Set Stuffing mode
	Dta1xxTxCtrlRegSetTxPckStuff(pCh->m_pRegBase, TxStuffMode);

	// SDI mode
	Dta1xxTxCtrlRegSetSdiMode(pCh->m_pRegBase,
										 ((TxPacketMode & DTA1XX_TXMODE_SDI)!=0) ? 1 : 0);
	Dta1xxTxCtrlRegSetSdiMode10Bit(pCh->m_pRegBase,
								   ((TxPacketMode & DTA1XX_TXMODE_SDI_10BIT)!=0) ? 1 : 0);

	// Enable/Disable SDI decompressor
	Dta1xxTxCtrlRegSetSdiDecomprModeEn(pCh->m_pRegBase,
								 ((TxPacketMode & DTA1XX_TXMODE_SDI_HUFFMAN)!=0) ? 1 : 0);

	// Change packet transmit mode
	Dta1xxTxCtrlRegSetTxMode(pCh->m_pRegBase, TxMode & 0x7);

	if ((TxPacketMode & DTA1XX_TXMODE_SDI) != 0) {
		// SDI registers
		// Set SDI 525 / 625 line mode
		Dta1xxTxCtrlRegSetSdiLineMode(pCh->m_pRegBase,
									 ((TxPacketMode & DTA1XX_TXMODE_SDI_625)!=0) ? 0 : 1);
	} else {
		// ASI registers
		// Set burst mode for DTA-100/105/112/115/116/117/140/145/160/2160/545/2142/2145
		if (    pCh->m_pFdo->m_TypeNumber==100
			||  pCh->m_pFdo->m_TypeNumber==105
			|| (pCh->m_pFdo->m_TypeNumber==112 && PortIndex==0)
			|| (pCh->m_pFdo->m_TypeNumber==115 && PortIndex==0)
			|| (pCh->m_pFdo->m_TypeNumber==116 && PortIndex==0)
			|| (pCh->m_pFdo->m_TypeNumber==117 && PortIndex==0)
			||  pCh->m_pFdo->m_TypeNumber==140
			||  pCh->m_pFdo->m_TypeNumber==145
			||  pCh->m_pFdo->m_TypeNumber==160
			||  pCh->m_pFdo->m_TypeNumber==2160
			||  pCh->m_pFdo->m_TypeNumber==545
			||  pCh->m_pFdo->m_TypeNumber==2142
			||  pCh->m_pFdo->m_TypeNumber==2145) {

			if ((TxPacketMode & DTA1XX_TXMODE_BURST) != 0)
				Dta1xxTxCtrlRegSetTxBurstMode(pCh->m_pRegBase, 1);
			else
				Dta1xxTxCtrlRegSetTxBurstMode(pCh->m_pRegBase, 0);
		}
	}

	// Update rate if packet size has changed, but not for modulators
	if (OldPckSize!=NewPckSize && !(    pCh->m_pFdo->m_TypeNumber==107
									||  pCh->m_pFdo->m_TypeNumber==110
									|| (pCh->m_pFdo->m_TypeNumber==112 && PortIndex==1)
									|| (pCh->m_pFdo->m_TypeNumber==115 && PortIndex==1)
									|| (pCh->m_pFdo->m_TypeNumber==116 && PortIndex==1)
									|| (pCh->m_pFdo->m_TypeNumber==117 && PortIndex==1)
									|| (pCh->m_Capability==DTA1XX_CHCAP_AD9789)))
		Dta1xxTxSetRate2(pFdo, PortIndex, ClockGenMode, TsRate);

	// Don't forget to release the mutex
	up(&pCh->m_MutexTxControl);

	return 0;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ GENERIC +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSetGenLed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Set state of generic front-panel LED.
//
Int  Dta1xxGenSetLed(
	DTA1XX_FDO* pFdo,		// Device extension
	Int  LedState)			// -1 = LED controlled by hardware
							//  0 = LED off
							//  1 = LED orange
{
	switch (LedState)
	{

	case 0:		// LED off
		if ( pFdo->m_UsesGenReg )
		{
			Dta1xxGenCtrlRegSetLedControl(pFdo->m_pGenRegs, 1);
			Dta1xxGenCtrlRegSetLedGreen(pFdo->m_pGenRegs, 0);
			Dta1xxGenCtrlRegSetLedRed(pFdo->m_pGenRegs, 0);
		} else
		{
			// LEGACY)
			if (pFdo->m_TypeNumber==100 || pFdo->m_TypeNumber==102) {
				Dta1xxTxCtrlRegSetLedControl(pFdo->m_Channel[0].m_pRegBase, 1);
				Dta1xxTxCtrlRegSetLedGreen(pFdo->m_Channel[0].m_pRegBase, 0);
				Dta1xxTxCtrlRegSetLedRed(pFdo->m_Channel[0].m_pRegBase, 0);
			}
			else if (pFdo->m_TypeNumber==120 || pFdo->m_TypeNumber==122) {
				Dta1xxRxCtrlRegSetLedControl(pFdo->m_Channel[0].m_pRegBase, 1);
				Dta1xxRxCtrlRegSetLedGreen(pFdo->m_Channel[0].m_pRegBase, 0);
				Dta1xxRxCtrlRegSetLedRed(pFdo->m_Channel[0].m_pRegBase, 0);
			}
		}
		break;

	case 1:		// LED orange
		if ( pFdo->m_UsesGenReg )
		{
			Dta1xxGenCtrlRegSetLedControl(pFdo->m_pGenRegs, 1);
			Dta1xxGenCtrlRegSetLedGreen(pFdo->m_pGenRegs, 1);
			Dta1xxGenCtrlRegSetLedRed(pFdo->m_pGenRegs, 1);
		} else
		{
			// LEGACY
			if (pFdo->m_TypeNumber==100 || pFdo->m_TypeNumber==102) {
				Dta1xxTxCtrlRegSetLedControl(pFdo->m_Channel[0].m_pRegBase, 1);
				Dta1xxTxCtrlRegSetLedGreen(pFdo->m_Channel[0].m_pRegBase, 1);
				Dta1xxTxCtrlRegSetLedRed(pFdo->m_Channel[0].m_pRegBase, 1);
			}
			else if (pFdo->m_TypeNumber==120 || pFdo->m_TypeNumber==122) {
				Dta1xxRxCtrlRegSetLedControl(pFdo->m_Channel[0].m_pRegBase, 1);
				Dta1xxRxCtrlRegSetLedGreen(pFdo->m_Channel[0].m_pRegBase, 1);
				Dta1xxRxCtrlRegSetLedRed(pFdo->m_Channel[0].m_pRegBase, 1);
			}
		}
		break;

	default:	// Transfer LED control back to hardware
		if ( pFdo->m_UsesGenReg )
			Dta1xxGenCtrlRegSetLedControl(pFdo->m_pGenRegs, 0);
		else {
			// LEGACY
			if (pFdo->m_TypeNumber==100 || pFdo->m_TypeNumber==102) {
				Dta1xxTxCtrlRegSetLedControl(pFdo->m_Channel[0].m_pRegBase, 0);
			}
			else if (pFdo->m_TypeNumber==120 || pFdo->m_TypeNumber==122) {
				Dta1xxRxCtrlRegSetLedControl(pFdo->m_Channel[0].m_pRegBase, 0);
			}
		}
		break;
	}
	return 0;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Modulation Helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxComputeRollOff -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dta1xxComputeRollOff(Channel* pCh)
{
	Int  Annex = pCh->m_XtraPar[0] & DTA1XX_MOD_J83_MSK;

	if (Annex == DTA1XX_MOD_J83_A)
		return DTA1XX_ROLLOFF_15;
	if (Annex==DTA1XX_MOD_J83_B && pCh->m_ModType==DTA1XX_MOD_QAM64)
		return DTA1XX_ROLLOFF_18;
	if (Annex==DTA1XX_MOD_J83_B && pCh->m_ModType==DTA1XX_MOD_QAM256)
		return DTA1XX_ROLLOFF_12;
	if (Annex == DTA1XX_MOD_J83_C)
		return DTA1XX_ROLLOFF_13;
	return DTA1XX_ROLLOFF_15;			// Default
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxIsHardQamAorC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Check for QAM-A or QAM-C, without DTA1XX_MOD_DVBC_SOFT defined
//
BOOLEAN  Dta1xxIsHardQamAorC(Channel* pCh)
{
	Int  Annex = pCh->m_XtraPar[0] & DTA1XX_MOD_J83_MSK;

	if (  pCh->m_ModType!=DTA1XX_MOD_QAM4   && pCh->m_ModType!=DTA1XX_MOD_QAM16
	   && pCh->m_ModType!=DTA1XX_MOD_QAM32  && pCh->m_ModType!=DTA1XX_MOD_QAM64
	   && pCh->m_ModType!=DTA1XX_MOD_QAM128 && pCh->m_ModType!=DTA1XX_MOD_QAM256)
		return FALSE;
	// Rule out soft QAM
	if ((pCh->m_XtraPar[0] & DTA1XX_MOD_DVBC_SOFT) != 0)
		return FALSE;
	return (Annex==DTA1XX_MOD_J83_A || Annex==DTA1XX_MOD_J83_C);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxIsSoftQam -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Check for QAM-B, or QAM-A / QAM-C with DTA1XX_MOD_DVBC_SOFT set in XtraPar0
//
BOOLEAN  Dta1xxIsSoftQam(Channel* pCh)
{
	Int  Annex = pCh->m_XtraPar[0] & DTA1XX_MOD_J83_MSK;

	if (  pCh->m_ModType!=DTA1XX_MOD_QAM4   && pCh->m_ModType!=DTA1XX_MOD_QAM16
	   && pCh->m_ModType!=DTA1XX_MOD_QAM32  && pCh->m_ModType!=DTA1XX_MOD_QAM64
	   && pCh->m_ModType!=DTA1XX_MOD_QAM128 && pCh->m_ModType!=DTA1XX_MOD_QAM256)
		return FALSE;
	return (Annex==DTA1XX_MOD_J83_B || (pCh->m_XtraPar[0] & DTA1XX_MOD_DVBC_SOFT)!=0);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ I2C routines +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxI2cRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dta1xxI2cRead(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
    struct file*  pFileObj, // Caller file object for lock checking
	UInt  DvcAddr,			// I2C Device address
	UInt  Length,			// Number of BYTES to read
	UInt8*  pBuf)			// Buffer for data read from I2C bus
{
	Int i, Status=0;
	UInt8* p;
	UInt w, Delay, WaitTime, MaxWaitTime;
	Int  BytesLeft = Length;			// Bytes left to transfer
	volatile UInt8* pGenRegs = pFdo->m_pGenRegs;

#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "Dta1xxI2cRead: DvcAddr: 0x%02X, Length: %d, pBuf: %p",
				DvcAddr, Length, pBuf );
#endif

	// Do we support the I2C interface
	if (pFdo->m_SupportsI2cItf != TRUE) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxI2cRead: DTA-%d doesnot support the I2C interface",
				    pFdo->m_TypeNumber );
#endif
        return -EFAULT;
	}

	// Sanity check: valid buffer size
	if (Length > DTA1XX_I2C_BUF_SIZE)
		return -EFAULT;

	// Nothing to transfer
	if (BytesLeft == 0)
		return 0;

	// Protect I2C access against concurrent access
	if ( 0!=down_interruptible(&pFdo->m_I2cMutex) )
		return -EFAULT;

    // Check that the I2C is not locked or is locked by our caller
	if (pFdo->m_pI2cExclAccFileObj!=NULL && pFdo->m_pI2cExclAccFileObj!=pFileObj) 
    {
        // Release I2C mutex
	    up(&pFdo->m_I2cMutex);
        return -EFAULT;
    }

	// Start I2C read transfer by writing device adress (7 MSBs only) and transfer count
	Dta1xxGenI2cAddrCntRegSetRdAddrAndCount(pGenRegs, (DvcAddr>>1), Length);

	// Wait for I2C transfer to complete. Max. wait time is 5 seconds
	// NOTE: wait time is expressed in us units
	MaxWaitTime = 5*1000000;
	Delay = 8;		// Minimum delay is 8us
    WaitTime = 0;
	while ( Dta1xxGenI2cStatusRegGetRdyInt(pGenRegs) == 0 )
	{
		udelay(Delay);

		// We wait no longer than max allowed
		WaitTime += Delay;
		if (WaitTime > MaxWaitTime) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "Dta1xxI2cRead: Transfer TIMEOUT!!" );
#endif
			// Release I2C mutex
			up(&pFdo->m_I2cMutex);
			return -EFAULT;	// time-out
		}
	}

	// Check if transfer was Nack-ed
	if ( Dta1xxGenI2cStatusRegGetRdAddrNack(pGenRegs) != 0 )
	{
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "Dta1xxI2cRead: Read adress Nack-ed!!" );
#endif
		// Release I2C mutex
		up(&pFdo->m_I2cMutex);
		return -EFAULT;
	}

	// Read data from I2C onboard buffer (we must read 4 bytes at a time)
	p = pBuf;
	for (i=0; i<BytesLeft; i+=4) {
		w = Dta1xxGenGetI2cData(pGenRegs);
		*((UInt32*)p) = (UInt32)w;
		p += 4;
	}
	// Any bytes left to read
	BytesLeft %= 4;
	if (BytesLeft > 0) {
		// Read one extra word to get last few bytes
		w = Dta1xxGenGetI2cData(pGenRegs);
		for (i=0; i<BytesLeft; i++) {
			p[i] = w&0xFF;
			w >>= 8;
		}
	}

	// Release I2C mutex
	up(&pFdo->m_I2cMutex);

	return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxI2cWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dta1xxI2cWrite(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
    struct file*  pFileObj, // Caller file object for lock checking
	UInt  DvcAddr,			// I2C Device address
	UInt  Length,			// Number of BYTES to write
	UInt8*  pBuf)			// Buffer with data to be written to I2C bus
{
	Int i, Status=0;
	UInt8* p;
	UInt w, Delay, WaitTime, MaxWaitTime;
	Int  BytesLeft = Length;			// Bytes left to transfer
	volatile UInt8* pGenRegs = pFdo->m_pGenRegs;

#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "Dta1xxI2cWrite: DvcAddr: 0x%02X, Length: %d, pBuf: %p",
				DvcAddr, Length, pBuf );
#endif

	// Do we support the I2C interface
	if (pFdo->m_SupportsI2cItf != TRUE) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxI2cWrite: DTA-%d doesnot support the I2C interface",
				    pFdo->m_TypeNumber );
#endif
        return -EFAULT;
	}

	// Sanity check: valid buffer size
	if (Length > DTA1XX_I2C_BUF_SIZE)
		return -EFAULT;

	// Nothing to transfer
	if (BytesLeft == 0)
		return 0;

	// Protect I2C access against concurrent access
	if ( 0!=down_interruptible(&pFdo->m_I2cMutex) )
		return -EFAULT;

    // Check that the I2C is not locked or is locked by our caller
	if (pFdo->m_pI2cExclAccFileObj!=NULL && pFdo->m_pI2cExclAccFileObj!=pFileObj) 
    {
        // Release I2C mutex
	    up(&pFdo->m_I2cMutex);
        return -EFAULT;
    }

	// First write I2C data to onboard I2C buffer (4 bytes at a time)
	p = pBuf;
	for (i=0; i<(BytesLeft/4); i++) {
		w = (p[3]<<24) + (p[2]<<16) + (p[1]<<8) + p[0];
		p += 4;
		Dta1xxGenSetI2cData(pGenRegs, w);
	}
	// Check for remaining bytes
	BytesLeft %= 4;
	if (BytesLeft > 0) {
		// Write last few bytes
		w = 0;
		for (i=(BytesLeft-1); i>=0; i--) {
			w <<=8;
			w += p[i];
		}
		Dta1xxGenSetI2cData(pGenRegs, w);
	}

	// Start I2C write transfer by writing device adress (7 MSBs only) and transfer count
	Dta1xxGenI2cAddrCntRegSetWrAddrAndCount(pGenRegs, (DvcAddr>>1), Length);

	// Wait for I2C transfer to complete. Max. wait time is 5 seconds
	// NOTE: wait time is expressed in us units
	MaxWaitTime = 5*1000000;
	Delay = 8;		// Minimum delay is 8us
    WaitTime = 0;
	while ( Dta1xxGenI2cStatusRegGetRdyInt(pGenRegs) == 0 )
	{
		udelay(Delay);

		// We wait no longer than max allowed
		WaitTime += Delay;
		if (WaitTime > MaxWaitTime) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "Dta1xxI2cWrite: Transfer TIMEOUT!!" );
#endif
			// Release I2C mutex
			up(&pFdo->m_I2cMutex);
			return -EFAULT;	// time-out
		}
	}

	// Check if transfer was Nack-ed
	if ( Dta1xxGenI2cStatusRegGetWrAddrNack(pGenRegs) != 0 )
		Status = -EFAULT;
	if ( Dta1xxGenI2cStatusRegGetWrDataNack(pGenRegs) != 0 )
		Status = -EFAULT;

	// Release I2C mutex
	up(&pFdo->m_I2cMutex);

	return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ STRING-CONVERSION FOR DEBUG INFO +=+=+=+=+=+=+=+=+=+=+=+=+=+


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxChType2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
char*  Dta1xxChType2Str(Int ChannelType)
{
	switch (ChannelType) {
	case DTA1XX_TS_TX_CHANNEL: return "TsTx";
	case DTA1XX_TS_RX_CHANNEL: return "TsRx";
	case DTA1XX_NW_TX_CHANNEL: return "NwTx";
	case DTA1XX_NW_RX_CHANNEL: return "NwRx";
	case DTA1XX_IP_TX_CHANNEL: return "IpTx";
	case DTA1XX_IP_RX_CHANNEL: return "IpRx";
	}
	return "??";
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxModType2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
char*  Dta1xxModType2Str(Int ModType)
{
	switch (ModType) {
	case DTA1XX_MOD_ATSC:		return "DTA1XX_MOD_ATSC";
	case DTA1XX_MOD_ADTBT:		return "DTA1XX_MOD_ADTBT";
	case DTA1XX_MOD_DMBTH:		return "DTA1XX_MOD_DMBTH";
	case DTA1XX_MOD_DVBS_BPSK:	return "DTA1XX_MOD_DVBS_BPSK";
	case DTA1XX_MOD_DVBS_QPSK:	return "DTA1XX_MOD_DVBS_QPSK";
	case DTA1XX_MOD_DVBS2_QPSK:	return "DTA1XX_MOD_DVBS2_QPSK";
	case DTA1XX_MOD_DVBS2_8PSK:	return "DTA1XX_MOD_DVBS2_8PSK";
	case DTA1XX_MOD_ISDBT:		return "DTA1XX_MOD_ISDBT";
	case DTA1XX_MOD_DVBT:		return "DTA1XX_MOD_DVBT";
	case DTA1XX_MOD_DVBT2:		return "DTA1XX_MOD_DVBT2";
	case DTA1XX_MOD_QAM4:		return "DTA1XX_MOD_QAM4";
	case DTA1XX_MOD_QAM16:		return "DTA1XX_MOD_QAM16";
	case DTA1XX_MOD_QAM32:		return "DTA1XX_MOD_QAM32";
	case DTA1XX_MOD_QAM64:		return "DTA1XX_MOD_QAM64";
	case DTA1XX_MOD_QAM128:		return "DTA1XX_MOD_QAM128";
	case DTA1XX_MOD_QAM256:		return "DTA1XX_MOD_QAM256";
	}
	return "DTA1XX_MOD_???";
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxAtscConstel2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
char*  Dta1xxAtscConstel2Str(Int Constel)
{
	switch (Constel) {
	case DTA1XX_MOD_ATSC_VSB8:  return "DTA1XX_MOD_ATSC_VSB8";
	case DTA1XX_MOD_ATSC_VSB16: return "DTA1XX_MOD_ATSC_VSB16";
	}
	return "DTA1XX_MOD_ATSC_???";
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxDvbtCodeRate2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
char*  Dta1xxDvbtCodeRate2Str(Int CodeRate)
{
	switch (CodeRate) {
	case DTA1XX_MOD_1_2: return "DTA1XX_MOD_1_2";
	case DTA1XX_MOD_2_3: return "DTA1XX_MOD_2_3";
	case DTA1XX_MOD_3_4: return "DTA1XX_MOD_3_4";
	case DTA1XX_MOD_5_6: return "DTA1XX_MOD_5_6";
	case DTA1XX_MOD_7_8: return "DTA1XX_MOD_7_8";
	}
	return "DTA1XX_MOD_???";
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxOfdmPar12BwStr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
char*  Dta1xxDvbtPar2BwStr(Int ParXtra1)
{
	switch (ParXtra1 & DTA1XX_MOD_DVBT_BW_MSK) {
	case DTA1XX_MOD_DVBT_5MHZ: return "DTA1XX_MOD_DVBT_5MHZ";
	case DTA1XX_MOD_DVBT_6MHZ: return "DTA1XX_MOD_DVBT_6MHZ";
	case DTA1XX_MOD_DVBT_7MHZ: return "DTA1XX_MOD_DVBT_7MHZ";
	case DTA1XX_MOD_DVBT_8MHZ: return "DTA1XX_MOD_DVBT_8MHZ";
	}
	return "DTA1XX_MOD_DVBT_?MHZ";
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxDvbtPar2ConstStr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
char*  Dta1xxDvbtPar2ConstStr(Int ParXtra1)
{
	switch (ParXtra1 & DTA1XX_MOD_DVBT_CO_MSK) {
	case DTA1XX_MOD_DVBT_QPSK:  return "DTA1XX_MOD_DVBT_QPSK";
	case DTA1XX_MOD_DVBT_QAM16: return "DTA1XX_MOD_DVBT_QAM16";
	case DTA1XX_MOD_DVBT_QAM64: return "DTA1XX_MOD_DVBT_QAM64";
	}
	return "DTA1XX_MOD_DVBT_CO??";
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxDvbtPar2GuardStr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
char*  Dta1xxDvbtPar2GuardStr(Int ParXtra1)
{
	switch (ParXtra1 & DTA1XX_MOD_DVBT_GU_MSK) {
	case DTA1XX_MOD_DVBT_G_1_32: return "DTA1XX_MOD_DVBT_G_1_32";
	case DTA1XX_MOD_DVBT_G_1_16: return "DTA1XX_MOD_DVBT_G_1_16";
	case DTA1XX_MOD_DVBT_G_1_8:  return "DTA1XX_MOD_DVBT_G_1_8";
	case DTA1XX_MOD_DVBT_G_1_4:  return "DTA1XX_MOD_DVBT_G_1_4";
	}
	return "DTA1XX_MOD_DVBT_G_???";
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxDvbtPar2InDepthStr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
char*  Dta1xxDvbtPar2InDepthStr(Int ParXtra1)
{
	switch (ParXtra1 & DTA1XX_MOD_DVBT_IL_MSK) {
	case DTA1XX_MOD_DVBT_INDEPTH: return "DTA1XX_MOD_DVBT_INDEPTH";
	case DTA1XX_MOD_DVBT_NATIVE:  return "DTA1XX_MOD_DVBT_NATIVE";
	}
	return "DTA1XX_MOD_DVBT_IL???";
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxDvbtPar2ModeStr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
char*  Dta1xxDvbtPar2ModeStr(Int ParXtra1)
{
	switch (ParXtra1 & DTA1XX_MOD_DVBT_MD_MSK) {
	case DTA1XX_MOD_DVBT_2K: return "DTA1XX_MOD_DVBT_2K";
	case DTA1XX_MOD_DVBT_4K: return "DTA1XX_MOD_DVBT_4K";
	case DTA1XX_MOD_DVBT_8K: return "DTA1XX_MOD_DVBT_8K";
	}
	return "DTA1XX_MOD_DVBT_?K";
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSetModulatorScaler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set output scaler value (output level) for a modulator, dependent on modulation type.
// The modulation type is read from the channel.
//
void  Dta1xxSetModulatorScaler(
	Channel*  pCh)			// Channel pointer
{
	Int  Annex, Scaler=75;

	// Supported for DTA-112/115/116/117 only
	if (   pCh->m_pFdo->m_TypeNumber!=112 && pCh->m_pFdo->m_TypeNumber!=115
		&& pCh->m_pFdo->m_TypeNumber!=116 && pCh->m_pFdo->m_TypeNumber!=117 )
		return;

	// Get Annex (for QAM)
	Annex = pCh->m_XtraPar[0] & DTA1XX_MOD_J83_MSK;

	// Scaler value depends on modulation type
	switch (pCh->m_ModType)
	{
	case DTA1XX_MOD_ATSC:                 Scaler = 111;  break;
	
	case DTA1XX_MOD_ADTBT:                Scaler = 81;  break;
	case DTA1XX_MOD_DMBTH:                Scaler = 81;  break;	
	
	case DTA1XX_MOD_DVBT:                 Scaler = 75;  break;
	case DTA1XX_MOD_DVBT2:                Scaler = 75;  break;
	
	case DTA1XX_MOD_ISDBT:                Scaler = 95;  break;

	case DTA1XX_MOD_QAM4:                 Scaler = 112;  break;
	case DTA1XX_MOD_QAM16:                Scaler = 112;  break;
	case DTA1XX_MOD_QAM32:                Scaler = 106;  break;
	case DTA1XX_MOD_QAM64:
		if ( Annex == DTA1XX_MOD_J83_B )  Scaler = 106;  // QAM-B
		else                              Scaler = 110;  // QAM-A/C
        break;
	case DTA1XX_MOD_QAM128:               Scaler = 157;  break;
	case DTA1XX_MOD_QAM256:
		if ( Annex == DTA1XX_MOD_J83_B )  Scaler = 106;  // QAM-B
		else							  Scaler = 109;  // QAM-A/C
        break;

	default:                              Scaler = 75;  break;
	}

	// Set the value (enable user output (scaler) level)
	Dta1xxTxModcRegEnableUserLevel(pCh->m_pRegBase, 1);
	Dta1xxTxModcRegSetOutputLevel(pCh->m_pRegBase, Scaler);
}
