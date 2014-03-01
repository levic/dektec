//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* TxIoCtl.c *#*#*#*#*#*#*#*#*# (C) 2000-2008 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dtu2xx driver, please refer to the Dta1xx readme

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include file -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "Dtu2xx.h"
#include "Dtu2xxRegs.h"
#include "EzUsb.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dtu2xxTxIoCtlReset(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int RstMode)					// The reset mode
{
	Int r, Status = 0;
    UInt8 VendCmd=0;
	UInt BytesTransf=0;
	Channel*  pCh=NULL;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlReset: Reset-mode=%d",
			   PortIndex, RstMode);
#endif

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlReset: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// Check for valid value
	if ( RstMode!=DTU2XX_RST_CHANNEL && RstMode!=DTU2XX_RST_CLEARFIFO )
		return -EINVAL;

	if ( RstMode==DTU2XX_RST_CHANNEL ) {
		VendCmd = DTU2XX_USB_TX_RESET;
	} else {
		VendCmd = DTU2XX_USB_TX_CLEAR_FIFO;
	}

	// Clear all flags
	Dtu2xxTxIoCtlClearFlags(pFdo, PortIndex, 0xFFFFFFFF);

	// Clear internal buffer
	pCh->m_BulkTransf.m_Buffer.m_pRdPtr =
						pCh->m_BulkTransf.m_Buffer.m_pWrPtr;
	pCh->m_BulkTransf.m_Buffer.m_Load = 0;

	// Reset load counting
	r = down_interruptible(&pCh->m_LoadExtrapLock);
	pCh->m_RpFifoLoad = 0;
	pCh->m_RpTimestamp = Dtu2xxGetTickCount();
	pCh->m_WrittenAfterRp = 0;
	up(&pCh->m_LoadExtrapLock);
	

	// Simply send the vendor command to the device
	Status = Dtu2xxVendorRequest(pFdo, VendCmd, 0,0, DTU2XX_DIRECTION_WRITE,0,
								 NULL, &BytesTransf);
	if ( Status != 0 )
		return Status;

	// We need to update all cached registers after a reset
	return Dtu2xxCacheOpRegs(pFdo);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlGetFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dtu2xxTxIoCtlGetFlags(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	OUT Int* pStatus,
	OUT Int* pLatched)
{
	Int Status = 0;
	Dtu2xxTx* pTxRegs = NULL;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetFlags: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pTxRegs = &(pFdo->m_Channel[PortIndex].m_TxRegs);

	// Init to zero
	*pStatus = *pLatched = 0;

	// Update cache
	Status = Dtu2xxIoCtlReadRegister(pFdo, DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_TXSTATUS,
									  &(pTxRegs->m_TxStatus.All));
	if ( Status != 0 )
		return Status;

	// Check for status flags
	if ( pTxRegs->m_TxStatus.Fields.m_UflInt ==1 )
		*pStatus |= DTU2XX_TX_FIFO_UFL;
	if ( pTxRegs->m_TxStatus.Fields.m_SyncInt==1 )
		*pStatus |= DTU2XX_TX_SYNC_ERR;

	// Clear status flags (write back register value)
	Status = Dtu2xxIoCtlWriteRegister(pFdo, DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_TXSTATUS,
									   pTxRegs->m_TxStatus.All);
	if ( Status != 0 )
		return Status;

	// Clear flags in cache
	pTxRegs->m_TxStatus.All &= ~0x1F00;

	// Update the status of the latched flags
	pFdo->m_Channel[PortIndex].m_Latched |= *pStatus;
	// Return the latched flags
	*pLatched = pFdo->m_Channel[PortIndex].m_Latched;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetFlags: Status=0x%08X, Latched=0x%08X",
			   PortIndex, *pStatus, *pLatched);
#endif
	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlClearFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dtu2xxTxIoCtlClearFlags(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int Latched)
{
	Int Status = 0;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlClearFlags: Latched=0x%08X",
			   PortIndex, Latched);
#endif

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlClearFlags: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Clear the latched flags
	pFdo->m_Channel[PortIndex].m_Latched &= ~Latched;

	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlSetTsRateBps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for setting the bit-rate
//
Int  Dtu2xxTxIoCtlSetTsRateBps(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int TsRateInt)				// The bit-rate
{
	Int Status = 0;
	Int  TxPacketMode, TxStuffMode;
	Int  PckSize;				// Packet size: 188 / 204
	Int  TxClock;				// The TxClock value
	Int64 TsRate = TsRateInt;
	Dtu2xxTx* pTxRegs = NULL;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetTsRateBps: Ts-rate=%d",
			   PortIndex, TsRateInt);
#endif

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetTsRateBps: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pTxRegs = &(pFdo->m_Channel[PortIndex].m_TxRegs);

	// Check for valid rate
	if ( TsRate<0 || TsRate>216000000 )
		return -EINVAL;

	// Get current packet mode
	Status = Dtu2xxTxIoCtlGetTxMode(pFdo, PortIndex, &TxPacketMode, &TxStuffMode);
	if ( Status != 0 )
		return Status;

	// Determine current packet size
	PckSize = Dtu2xxTxMode2PacketSize(TxPacketMode);

	// Set phase-increment register, depends on transport-stream rate and packet size
	if (PckSize == 188) {
		// p / q = 2^32 / 216.000.000
		// p and q both divided by 512
		TxClock = (UInt32)( (Dtu2xxBinDiv(
						       (TsRate*8388608*2 ), 421875, NULL) + 1) >>1 );
	}
	else if (PckSize == 192) {
		TxClock = (UInt32)( (Dtu2xxBinDiv(
						       (TsRate*402653184*2), 19828125, NULL) +1 ) >> 1 );
	}
	else {	// PckSize == 204
		TxClock = (UInt32)( (Dtu2xxBinDiv(
						       (TsRate*142606336*2), 6609375, NULL) +1 ) >> 1 );
	}

	// Update cache
	pTxRegs->m_TxClock = TxClock;

	// Update register to match cache
	return Dtu2xxIoCtlWriteRegister(pFdo, DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_TXCLOCK,
									 pTxRegs->m_TxClock);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlSetTxControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for setting the Transmit-Control mode
//
Int  Dtu2xxTxIoCtlSetTxControl(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int TxControl)				// Transmit-control
{
	Dtu2xxTx* pTxRegs = NULL;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetTxControl: Tx-control=%d",
			   PortIndex, TxControl);
#endif

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetTxControl: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pTxRegs = &(pFdo->m_Channel[PortIndex].m_TxRegs);


	// Check Transmit-Control value
	if (    TxControl != DTU2XX_TXCTRL_IDLE && TxControl != DTU2XX_TXCTRL_HOLD
		 && TxControl != DTU2XX_TXCTRL_SEND )
	{
		return -EINVAL;
	}

	// Set control field(s) in cache
	pTxRegs->m_TxControl.Fields.m_TxCtrl = TxControl;

	// Update register value to match cache
	return Dtu2xxIoCtlWriteRegister(pFdo, DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_TXCTRL,
									 pTxRegs->m_TxControl.All);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlSetTxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for setting the Transmit-Mode
//
Int  Dtu2xxTxIoCtlSetTxMode(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int TxPacketMode,			// Transmit-mode
	IN Int TxStuffMode)				// Stuff mode
{
	Int Status = 0;
	Int CurTsRateBps;
	UInt TxMode;
	Dtu2xxTx* pTxRegs = NULL;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetTxMode: Tx-mode=%d, Stuff-mode=%d",
			   PortIndex, TxPacketMode, TxStuffMode);
#endif

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetTxMode: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pTxRegs = &(pFdo->m_Channel[PortIndex].m_TxRegs);

	// Get current bit-rate (needed later to recopmpute the bit-rate)
	Status = Dtu2xxTxIoCtlGetTsRateBps(pFdo, PortIndex, &CurTsRateBps);
	if ( Status!=0 )
		return Status;

	// Check for valid mode
	TxMode = TxPacketMode & DTU2XX_TXMODE_MASK;
	if (    TxMode != DTU2XX_TXMODE_188   && TxMode != DTU2XX_TXMODE_204
		 && TxMode != DTU2XX_TXMODE_ADD16 && TxMode != DTU2XX_TXMODE_MIN16
		 && TxMode != DTU2XX_TXMODE_RAW
		 && TxMode != (DTU2XX_TXMODE_SDI_FULL & ~DTU2XX_TXMODE_SDI )
		 && TxMode != (DTU2XX_TXMODE_SDI_ACTVID & ~DTU2XX_TXMODE_SDI) )
	{
		return -EINVAL;
	}
	// Check for valid stuff mode (only for none SDI)
	if (   0==(TxPacketMode & DTU2XX_TXMODE_SDI)
		&& TxStuffMode!= DTU2XX_TXSTUFF_MODE_ON && TxStuffMode!=DTU2XX_TXSTUFF_MODE_OFF )
	{
		return -EINVAL;
	}

	// Set mode field(s) in cache
	pTxRegs->m_TxControl.Fields.m_TxMode	= TxPacketMode&0x3;
	pTxRegs->m_TxControl.Fields.m_TxModeExt = ((TxPacketMode&0x4)>>2);

	// Set stuff mode
	pTxRegs->m_TxControl.Fields.m_PckStuff = TxStuffMode;

	// Set SDI mode bits
	if ( 0!=(TxPacketMode & DTU2XX_TXMODE_SDI) )
		pTxRegs->m_TxControl.Fields.m_SdiMode = 1;
	else
		pTxRegs->m_TxControl.Fields.m_SdiMode = 0;

	// 10-bit or 8-bit modes
	if ( 0!=(TxPacketMode & DTU2XX_TXMODE_SDI_10B) )
		pTxRegs->m_TxControl.Fields.m_Sdi10Bit = 1;
	else
		pTxRegs->m_TxControl.Fields.m_Sdi10Bit = 0;

	// Huffman compression
	if ( 0!=(TxPacketMode & DTU2XX_TXMODE_SDI_HUFFMAN) )
		pTxRegs->m_TxControl.Fields.m_SdiHuffEn = 1;
	else
		pTxRegs->m_TxControl.Fields.m_SdiHuffEn = 0;

	// ASI or SDI mode?
	if (TxPacketMode & DTU2XX_TXMODE_SDI)
	{
		// Set SDI 525/625 line mode
		if ( 0!=(TxPacketMode & DTU2XX_TXMODE_SDI_625) )
			pTxRegs->m_TxControl.Fields.m_BurstOrSdiMode = 0;	// 625 line mode
		else
			pTxRegs->m_TxControl.Fields.m_BurstOrSdiMode = 1;	// 525 line mode
	}
	else
	{
		// Set burst mode
		if ( 0!=(TxPacketMode & DTU2XX_TXMODE_BURST) )
			pTxRegs->m_TxControl.Fields.m_BurstOrSdiMode = 1;
		else
			pTxRegs->m_TxControl.Fields.m_BurstOrSdiMode = 0;
	}

	// Update register value to match cache
	Status = Dtu2xxIoCtlWriteRegister(pFdo, DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_TXCTRL,
									   pTxRegs->m_TxControl.All);
	if ( Status!=0 )
		return Status;

	// Reset bit-rate after changing the TxMode (i.e. to apply 204/188 factor)
	return Dtu2xxTxIoCtlSetTsRateBps(pFdo, PortIndex, CurTsRateBps);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlSetSourceSelect -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for setting the transmit-source-selection
//
Int Dtu2xxTxIoCtlSetSourceSelect(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int TxSrcSel)				// Output source selection
{
	Dtu2xxTx* pTxRegs = NULL;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetSourceSelect: Source-select=%d",
			   PortIndex, TxSrcSel);
#endif

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetSourceSelect: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pTxRegs = &(pFdo->m_Channel[PortIndex].m_TxRegs);

	// Check TX source selection
	if ( TxSrcSel<0 || TxSrcSel>1 )
		return -EINVAL;

	// Set control field(s) in cache
	pTxRegs->m_TxControl2.Fields.m_TxScrSel = TxSrcSel;

	// Update register value to match cache
	return Dtu2xxIoCtlWriteRegister(pFdo, DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_TXCTRL2,
									 pTxRegs->m_TxControl2.All);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlTxPolarity -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for setting the transmit-polarity
//
Int  Dtu2xxTxIoCtlTxPolarity(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int Polarity)				// Polarity
{
	Dtu2xxTx* pTxRegs = NULL;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlTxPolarity: Polarity=%d",
			   PortIndex, Polarity);
#endif

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlTxPolarity: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pTxRegs = &(pFdo->m_Channel[PortIndex].m_TxRegs);

	// Check for valid values
	if ( Polarity != DTU2XX_TXPOL_NORMAL && Polarity != DTU2XX_TXPOL_INVERTED )
		return -EINVAL;

	// Set polarity
	pTxRegs->m_TxControl.Fields.m_TxInvAsi = Polarity;

	// Update register value to match cache
	return Dtu2xxIoCtlWriteRegister(pFdo, DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_TXCTRL,
									 pTxRegs->m_TxControl.All);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlSetFifoSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for setting the fifo size
//
Int  Dtu2xxTxIoCtlSetFifoSize(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int FifoSize)				// The fifo size
{
	Dtu2xxTx* pTxRegs = NULL;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetFifoSize: Size=%d",
			   PortIndex, FifoSize);
#endif

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetFifoSize: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pTxRegs = &(pFdo->m_Channel[PortIndex].m_TxRegs);

	// Check for valid values
	if ( FifoSize<16 || 0!=(FifoSize % 16) || FifoSize>(16*1024*1024) )
		return -EINVAL;

	// Update cache
	pTxRegs->m_FifoSize = FifoSize;

	// Update register value to match cache
	return Dtu2xxIoCtlWriteRegister(pFdo, DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_FIFOSIZE,
									 pTxRegs->m_FifoSize);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlGetMaxFifoSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for getting the maximum fifo size
//
Int  Dtu2xxTxIoCtlGetMaxFifoSize(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int* pSize)					// The maximum fifo size
{
	//Int Status = 0;
	Dtu2xxTx* pTxRegs = NULL;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetMaxFifoSize: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pTxRegs = &(pFdo->m_Channel[PortIndex].m_TxRegs);

	// Decode the SDRAM size field
	switch ( pTxRegs->m_TxStatus.Fields.m_SdramSize )
	{
	case DTU2XX_SDRAM_SIZE_8MB:
		*pSize = (8*1024*1024);
		break;
	case DTU2XX_SDRAM_SIZE_16MB:
		*pSize = (16*1024*1024);
		break;
	case DTU2XX_SDRAM_SIZE_32MB:
		*pSize = (32*1024*1024);
		break;
	default:
		*pSize = (8*1024*1024);
		break;
	}
#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetMaxFifoSize: MaxSize=%d",
			   PortIndex, *pSize);
#endif
	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlGetFifoSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for getting the fifo size
//
Int  Dtu2xxTxIoCtlGetFifoSize(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int* pSize)					// Current fifo size
{
	//Int Status = 0;
	Dtu2xxTx* pTxRegs = NULL;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetFifoSize: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pTxRegs = &(pFdo->m_Channel[PortIndex].m_TxRegs);

	// Read from cache
	*pSize = pTxRegs->m_FifoSize;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetFifoSize: Size=%d",
			   PortIndex, *pSize);
#endif
	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlGetFifoLoad -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for getting the fifo load
//
Int  Dtu2xxTxIoCtlGetFifoLoad(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	OUT Int* pFifoLoad)
{
	Int r, Status = 0;
	Channel* pCh=NULL;
	Dtu2xxTx* pTxRegs = NULL;
	UInt64 CurrTime, TimeSinceLastRp;
	Int DeltaL=0, TxControl=0;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetFifoLoad: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];
	// Get register cache
	pTxRegs = &(pCh->m_TxRegs);
	// Init to zero
	*pFifoLoad = 0;

	// Get tx-control state
	TxControl = pTxRegs->m_TxControl.Fields.m_TxCtrl;

	// Get current time
	CurrTime = Dtu2xxGetTickCount();

	// Read FIFO load from hardware or extrapolate FIFO load.
	// In extrapolation mode, a reference point is created every two seconds.
	// In IDLE or HOLD mode, extrapolation is not used.
	TimeSinceLastRp = CurrTime - pCh->m_RpTimestamp;
	if (!pCh->m_EnaFifoLoadExtrap || TxControl!=DTU2XX_TXCTRL_SEND
															   || TimeSinceLastRp>2000000)
	{
		// Get FIFO load from hardware
		Status = Dtu2xxIoCtlReadRegister(pFdo,
										 DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_FIFOLOAD,
										 pFifoLoad);

		// Create new refence point
		if (pCh->m_EnaFifoLoadExtrap)
		{
			r = down_interruptible(&pCh->m_LoadExtrapLock);
			pCh->m_RpFifoLoad = *pFifoLoad;
			pCh->m_RpTimestamp = CurrTime;
			pCh->m_WrittenAfterRp = 0;
			up(&pCh->m_LoadExtrapLock);

#if LOG_LEVEL > 2
			DTU2XX_LOG(KERN_INFO, "Dtu2xxTxIoCtlGetFifoLoad: REFPOINT: RpFifoLoad=%d, "
					       "TimeSinceLastRp=%llu", pCh->m_RpFifoLoad, TimeSinceLastRp);
#endif
		}

		// Update cache
		pTxRegs->m_FifoLoad = *pFifoLoad;
	}
	else
	{
		// Extrapolate FIFO load
		r = down_interruptible(&pCh->m_LoadExtrapLock);
		DeltaL =   pCh->m_WrittenAfterRp
				 - (Int)(Dtu2xxBinUDiv((TimeSinceLastRp*pCh->m_NumTxBytesPerSec), 1000000LL, NULL));
		*pFifoLoad = pCh->m_RpFifoLoad + DeltaL;
		up(&pCh->m_LoadExtrapLock);

#if LOG_LEVEL > 2
		DTU2XX_LOG(KERN_INFO, "Dtu2xxTxIoCtlGetFifoLoad: RpFifoLoad=%d, "
							  "TimeSinceLastRp=%llu, WrittenAfterRp=%d, DeltaL=%d, "
							  "FifoLoad=%d", pCh->m_RpFifoLoad, TimeSinceLastRp,
							  pCh->m_WrittenAfterRp, DeltaL, *pFifoLoad);
#endif

		// Clip FIFO load to min and max
		if (*pFifoLoad < 0)
			*pFifoLoad = 0;
		if ((UInt32)(*pFifoLoad) > pTxRegs->m_FifoSize+256)
			*pFifoLoad = (pTxRegs->m_FifoSize+256);
	}
	
	// Add data in bulk transfer buffer to the load
	*pFifoLoad += pCh->m_BulkTransf.m_Buffer.m_Load;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetFifoLoad: Load=%d, Int. Load=%d",
			   PortIndex, *pFifoLoad, pCh->m_BulkTransf.m_Buffer.m_Load);
#endif

	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlGetTsRateBps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for getting the bit-rate
//
Int  Dtu2xxTxIoCtlGetTsRateBps(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	OUT Int* pTsRate)				// The bit-rate
{
	Int Status = 0;
	Int  TxPacketMode, TxStuffMode;
	Int  PckSize;					// Packet size: 188 / 204
	Int64  PhaseIncr;				// The TxClock value
	Dtu2xxTx* pTxRegs = NULL;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetTsRateBps: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pTxRegs = &(pFdo->m_Channel[PortIndex].m_TxRegs);

	// Init to zero
	*pTsRate = 0;

	// Get current value from cache
	PhaseIncr = (Int64)pTxRegs->m_TxClock;

	// Get current packet mode
	Status = Dtu2xxTxIoCtlGetTxMode(pFdo, PortIndex, &TxPacketMode, &TxStuffMode);
	if ( Status != 0 )
		return Status;
	// Determine current packet size
	PckSize = Dtu2xxTxMode2PacketSize(TxPacketMode);

	// Compute transport-stream rate, depends on phase increment and packet size
	if (PckSize == 188)
		*pTsRate = (Int)((Dtu2xxBinDiv((PhaseIncr*421875*2), 8388608, NULL)+1)>>1);
	else if (PckSize == 192)
		*pTsRate = (Int)((Dtu2xxBinDiv((PhaseIncr*19828125*2), 402653184, NULL)+1)>>1);
	else	// nPckSize == 204
		*pTsRate = (Int)((Dtu2xxBinDiv((PhaseIncr*6609375*2), 142606336, NULL)+1)>>1);

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetTsRateBps: Ts-rate=%d",
			   PortIndex, *pTsRate);
#endif
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlGetTxControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for getting the transmit-control mode
//
Int  Dtu2xxTxIoCtlGetTxControl(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	OUT Int* pTxControl)
{
	Int Status = 0;
	Dtu2xxTx* pTxRegs = NULL;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetTxControl: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pTxRegs = &(pFdo->m_Channel[PortIndex].m_TxRegs);

	// Get control value from cache
	*pTxControl = pTxRegs->m_TxControl.Fields.m_TxCtrl;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetTxControl: Tx-control=%d",
			   PortIndex, *pTxControl);
#endif
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlGetTxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for getting the transmit-mode
//
Int  Dtu2xxTxIoCtlGetTxMode(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	OUT Int* pTxPacketMode,
	OUT Int* pTxStuffMode)
{
	Int Status = 0;
	Dtu2xxTx* pTxRegs = NULL;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetTxMode: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pTxRegs = &(pFdo->m_Channel[PortIndex].m_TxRegs);

	// Get current packet mode (from cache)
	*pTxPacketMode = pTxRegs->m_TxControl.Fields.m_TxMode;
	*pTxPacketMode |= pTxRegs->m_TxControl.Fields.m_TxModeExt<<2;

	// Are we in SDI or ASI mode
	if ( pTxRegs->m_TxControl.Fields.m_SdiMode == 1 )
	{
		// Get SDI mode
		if ( pTxRegs->m_TxControl.Fields.m_BurstOrSdiMode == 0 )
			*pTxPacketMode |= DTU2XX_TXMODE_SDI_625;
	}
	else
	{
		// Get burst status
		if ( pTxRegs->m_TxControl.Fields.m_BurstOrSdiMode == 1 )
			*pTxPacketMode |= DTU2XX_TXMODE_BURST;
	}

	// Get SDI mode bit
	if ( pTxRegs->m_TxControl.Fields.m_SdiMode == 1 )
		*pTxPacketMode |= DTU2XX_TXMODE_SDI;
	// Get SDI 10-bit or 8-bit mode
	if ( pTxRegs->m_TxControl.Fields.m_Sdi10Bit == 1 )
		*pTxPacketMode |= DTU2XX_TXMODE_SDI_10B;
	// Get huffman compressed settings
	if ( pTxRegs->m_TxControl.Fields.m_SdiHuffEn == 1 )
		*pTxPacketMode |= DTU2XX_TXMODE_SDI_HUFFMAN;

	// Get stuff mode
	*pTxStuffMode = pTxRegs->m_TxControl.Fields.m_PckStuff;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetTxMode: Tx-mode=%d, Stuff-mode=%d",
			   PortIndex, *pTxPacketMode, *pTxStuffMode);
#endif
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlReadLoopBackData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dtu2xxTxIoCtlReadLoopBackData(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	OUT UInt8* pUserBuf,
	IN Int BytesToRead)
{
#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlReadLoopBackData: pUserBuf=0x%p, "
			   "BytesToRead=%d", PortIndex, pUserBuf, BytesToRead);
#endif
	return -EFAULT;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlSetLoopBackMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dtu2xxTxIoCtlSetLoopBackMode(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int Mode)
{
#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetLoopBackMode: Mode=%d",
			   PortIndex, Mode);
#endif
	return -EFAULT;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlGetModControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for getting modulation control
//
Int  Dtu2xxTxIoCtlGetModControl(
	IN PDTU2XX_FDO pFdo,		// Our device object
	IN Int PortIndex,			// Port index
	OUT Int* pModType,
	OUT Int* pParXtra0,
	OUT Int* pParXtra1,
	OUT Int* pParXtra2)
{
	Channel*  pCh = NULL;
	
	// Only supported by DTU-215
	if ( pFdo->m_TypeNumber!=215 ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetModControl: TypeNumber=%d ILLEGAL",
				   PortIndex, pFdo->m_TypeNumber);
		return -EFAULT;
	}

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetModControl: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// Copy modulation parameters from shadow variables in channel data
	*pModType  = pCh->m_ModType;
	*pParXtra0 = pCh->m_ParXtra[0];
	*pParXtra1 = pCh->m_ParXtra[1];
	*pParXtra2 = pCh->m_ParXtra[2];

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetModControl: ModType=%d, ParXtra0=%d, "
			   "ParXtra1=%d, ParXtra2=%d",
			   PortIndex, *pModType, *pParXtra0, *pParXtra1, *pParXtra2);
#endif
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlSetModControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for setting modulation control
//
Int  Dtu2xxTxIoCtlSetModControl(
	IN PDTU2XX_FDO pFdo,		// Our device object
	IN Int PortIndex,			// Port index
	IN Int ModType,
	IN Int ParXtra0,
	IN Int ParXtra1,
	IN Int ParXtra2)
{
	Int Status = 0;
	Dtu2xxTx* pTxRegs = NULL;
	Channel*  pCh=NULL;
	BOOLEAN  Is215 = (pFdo->m_TypeNumber==215);

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetModControl: ModType=%d, ParXtra0=%d, "
			   "ParXtra1=%d, ParXtra2=%d",
			   PortIndex, ModType, ParXtra0, ParXtra1, ParXtra2);
#endif

	// Only supported by DTU-215
	if ( !Is215 ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetModControl: TypeNumber=%d ILLEGAL",
				   PortIndex, pFdo->m_TypeNumber);
		return -EFAULT;
	}

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetModControl: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];
	// Get register cache
	pTxRegs = &(pCh->m_TxRegs);

	// Save modulation type and parameters in shadow variables in device extension
	pCh->m_ModType = ModType;
	pCh->m_ParXtra[0] = ParXtra0;
	pCh->m_ParXtra[1] = ParXtra1;
	pCh->m_ParXtra[2] = ParXtra2;

	// Call board specific handler
	if (Is215) {
		Status = Dtu215SetModControl(pCh, ModType, ParXtra0, ParXtra1, ParXtra2);
		if (Status != 0)
			return Status;
	}

	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlSetRfMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for setting RF-mode
//
Int  Dtu2xxTxIoCtlSetRfMode(
	IN PDTU2XX_FDO pFdo,		// Our device object
	IN Int PortIndex,			// Port index
	IN Int RfMode)
{
	Int  Mode = RfMode & ~DTU2XX_UPCONV_SPECINV;
	Int Status = 0;
	Dtu2xxTx* pTxRegs = NULL;
	Channel*  pCh=NULL;
	BOOLEAN  Is215 = (pFdo->m_TypeNumber==215);

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetRfMode: RfMode=%d", PortIndex, RfMode);
#endif

	// Only supported by DTU-215
	if ( !Is215 ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetRfMode: TypeNumber=%d ILLEGAL",
				   PortIndex, pFdo->m_TypeNumber);
		return -EFAULT;
	}

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetRfMode: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];
	// Get register cache
	pTxRegs = &(pCh->m_TxRegs);
	

	// Check for valid mode
	if (   Mode!=DTU2XX_UPCONV_CW  && Mode!=DTU2XX_UPCONV_CWI
		&& Mode!=DTU2XX_UPCONV_CWQ && Mode!=DTU2XX_UPCONV_MUTE
		&& Mode!=DTU2XX_UPCONV_NORMAL)
	{
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetRfMode: INVALID MODE=%d ",
				   PortIndex, RfMode);
		return -EINVAL;
	}

	// First call board specific handler
	if ( Is215 ) {
		Status = Dtu215SetRfMode(pCh, RfMode);
		if ( Status != 0 )
			return Status;
	}

	// Set requested mode
	switch (Mode) 
	{

	case DTU2XX_UPCONV_CW:
		// Set CW test-pattern
		pTxRegs->m_ModControl1.Fields.m_TestPat = DTU2XX_TP_CW;
		// Remove mute
		pTxRegs->m_ModControl1.Fields.m_MuteI = 0;
		pTxRegs->m_ModControl1.Fields.m_MuteQ = 0;
		break;

	case DTU2XX_UPCONV_CWI:
		// Set CWI test-pattern
		pTxRegs->m_ModControl1.Fields.m_TestPat = DTU2XX_TP_CWI;
		// Remove mute
		pTxRegs->m_ModControl1.Fields.m_MuteI = 0;
		pTxRegs->m_ModControl1.Fields.m_MuteQ = 0;
		break;

	case DTU2XX_UPCONV_CWQ:
		// Set CWQ test-pattern
		pTxRegs->m_ModControl1.Fields.m_TestPat = DTU2XX_TP_CWQ;
		// Remove mute
		pTxRegs->m_ModControl1.Fields.m_MuteI = 0;
		pTxRegs->m_ModControl1.Fields.m_MuteQ = 0;	
		break;

	case DTU2XX_UPCONV_MUTE:
		// Reset test-pattern
		pTxRegs->m_ModControl1.Fields.m_TestPat = DTU2XX_TP_NORMAL;
		// Mute I and Q => no output
		pTxRegs->m_ModControl1.Fields.m_MuteI = 1;
		pTxRegs->m_ModControl1.Fields.m_MuteQ = 1;
		break;

	case DTU2XX_UPCONV_NORMAL:
		// Reset test-pattern
		pTxRegs->m_ModControl1.Fields.m_TestPat = DTU2XX_TP_NORMAL;
		// Remove mute
		pTxRegs->m_ModControl1.Fields.m_MuteI = 0;
		pTxRegs->m_ModControl1.Fields.m_MuteQ = 0;
		break;		break;
	}

	// Check for spectral inversion
	if ((RfMode & DTU2XX_UPCONV_SPECINV) != 0)
		pTxRegs->m_ModControl1.Fields.m_SpecInv = 1;
	else
		pTxRegs->m_ModControl1.Fields.m_SpecInv = 0;

	// Finally: update register value to match cache
	return Dtu2xxIoCtlWriteRegister(pFdo, DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_MODCONTROL1,
									 (Int)(pTxRegs->m_ModControl1.All));
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlGetSymSampleRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for getting symbol/sample rate
//
Int  Dtu2xxTxIoCtlGetSymSampleRate(
	IN PDTU2XX_FDO pFdo,		// Our device object
	IN Int PortIndex,			// Port index
	OUT Int* pSymSampelRate)
{
	Int  Status = 0;
	Channel*  pCh=NULL;
	BOOLEAN  Is215 = (pFdo->m_TypeNumber==215);

	// Only supported by DTU-215
	if ( !Is215 ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetSymSampleRate: TypeNumber=%d ILLEGAL",
				   PortIndex, pFdo->m_TypeNumber);
		return -EFAULT;
	}

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetSymSampleRate: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// Simple return cached value
	*pSymSampelRate = pCh->m_TsSymOrSampRate;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetSymSampleRate: SymSampelRate=%d",
			   PortIndex, *pSymSampelRate);
#endif
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlSetSymSampleRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for setting symbol/sample rate
//
Int  Dtu2xxTxIoCtlSetSymSampleRate(
	IN PDTU2XX_FDO pFdo,		// Our device object
	IN Int PortIndex,			// Port index
	IN Int SymSampelRate)
{
	Int  Status = 0;
	Channel*  pCh=NULL;
	BOOLEAN  Is215 = (pFdo->m_TypeNumber==215);

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetSymSampleRate: SymSampelRate=%d",
			   PortIndex, SymSampelRate);
#endif

	// Only supported by DTU-215
	if ( !Is215 ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetSymSampleRate: TypeNumber=%d ILLEGAL",
				   PortIndex, pFdo->m_TypeNumber);
		return -EFAULT;
	}

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetSymSampleRate: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// Store symbol/sample rate in cache
	pCh->m_TsSymOrSampRate = SymSampelRate;

	// Call device specific implementation
	Status = Dtu215SetSymSampleRate(pCh, SymSampelRate);
	if ( Status != 0 )
		return Status;

	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlGetRfControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for getting rf-control value (i.e. carrier freq) 
//
Int  Dtu2xxTxIoCtlGetRfControl(
	IN PDTU2XX_FDO pFdo,		// Our device object
	IN Int PortIndex,			// Port index
	OUT Int64*  pRfFreq)
{
	Channel*  pCh=NULL;
	BOOLEAN  Is215 = (pFdo->m_TypeNumber==215);

	// Only supported by DTU-215
	if ( !Is215 ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetRfControl: TypeNumber=%d ILLEGAL",
				   PortIndex, pFdo->m_TypeNumber);
		return -EFAULT;
	}

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetRfControl: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// Simple return cached value
	*pRfFreq = pCh->m_RfFreq;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlGetRfControl: pRfFreq=%llu",
			   PortIndex, *pRfFreq);
#endif

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlSetRfControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control handler for setting rf-control value (i.e. carrier freq) 
//
Int  Dtu2xxTxIoCtlSetRfControl(
	IN PDTU2XX_FDO  pFdo,	// Our device object
	IN Int  PortIndex,			// Port index
	IN Int64  RfFreq)
{
	Int  Status = 0;
	Channel*  pCh=NULL;
	BOOLEAN  Is215 = (pFdo->m_TypeNumber==215);

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetRfControl: RfFreq=%llu",
			   PortIndex, RfFreq);
#endif

	// Only supported by DTU-215
	if ( !Is215 ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetRfControl: TypeNumber=%d ILLEGAL",
				   PortIndex, pFdo->m_TypeNumber);
		return -EFAULT;
	}

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetRfControl: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// Call device specific implementation
	Status = Dtu215SetRfControl(pCh, RfFreq);
	if ( Status != 0 )
		return Status;

	// Cache rf-frequency
	pCh->m_RfFreq = RfFreq;

	// Apply frequency response compensation
	if (Is215)
		Status = Dtu215FrequencyResponseCompensation(pCh);

	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlSetOutputLevel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for setting output level
//
Int  Dtu2xxTxIoCtlSetOutputLevel(
	IN PDTU2XX_FDO  pFdo,	// Our device object
	IN Int  PortIndex,			// Port index
	IN Int  LeveldBm)
{
	Dtu2xxTx* pTxRegs = NULL;
	Channel*  pCh = NULL;
	Int  MinLevel=0, MaxLevel=0;
	BOOLEAN  Is215 = (pFdo->m_TypeNumber==215);

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetOutputLevel: LeveldBm=%d",
			   PortIndex, LeveldBm);
#endif

	// Only supported by DTU-215
	if ( !Is215 ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetOutputLevel: TypeNumber=%d ILLEGAL",
				   PortIndex, pFdo->m_TypeNumber);
		return -EFAULT;
	}

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetOutputLevel: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];
	// Get register cache
	pTxRegs = &(pCh->m_TxRegs);

	// Get minimum and maximum level
	Dtu215GetMinMaxOutpLevel(pCh, &MinLevel, &MaxLevel);

	// Check whether level is within allowed range
	if ( LeveldBm<MinLevel || LeveldBm>MaxLevel ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlSetOutputLevel: OUT OF RANGE "
				   "(%d<=Level<=%d)", PortIndex, MinLevel, MaxLevel);
		return -EINVAL;
	}

	// Cache output level
	pCh->m_OutpLeveldBm = LeveldBm;

	// Compute the number of .5dB attenuator steps needed for the desired level
	pTxRegs->m_RfControl2.Fields.m_OutpAttn = abs((LeveldBm - MaxLevel) / 5);
	
	// Finally: update register value to match cache
	return Dtu2xxIoCtlWriteRegister(pFdo, DTU2XX_TX_BASE_ADDR+DTU2XX_TX_REG_RFCONTROL2,
									 (Int)(pTxRegs->m_RfControl2.All));
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxTxIoCtlBulkWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write data to the DTU-2XX (uses bulk transfers)
//
Int  Dtu2xxTxIoCtlBulkWrite(
	IN PDTU2XX_FDO pFdo,		// Our device object
	IN Int PortIndex,			// Port index
	IN Int TransferSize,		// Number of bytes to transfer
	IN UInt8* pBuf)				// Buffer with data to send
{
	Int r, Status=0;
	Int n, BytesLeft, BufIndex, NumCopy, CopiedFromIntBuf, TempBufSize, TotalNumWritten=0;
    UInt8* pTempBuf=NULL;
	Channel*  pCh=NULL;
	PDTU2XX_RING_BUFFER pRingBuf = NULL;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxTxIoCtlBulkWrite: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	if ( TransferSize == 0 )
		return 0;
	if ( pBuf == NULL )
		return -EINVAL;

	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// Transmit control may not be idle, because that will hang the driver
	if (pCh->m_TxRegs.m_TxControl.Fields.m_TxCtrl == DTU2XX_TXCTRL_IDLE)
		return -EFAULT;

	TotalNumWritten = 0;
	CopiedFromIntBuf = 0;
	BytesLeft = TransferSize;
	BufIndex = 0;

	TempBufSize = pFdo->m_Channel[PortIndex].m_TempBufSize;
	pTempBuf = pFdo->m_Channel[PortIndex].m_pTempBuf;
	pRingBuf = &(pFdo->m_Channel[PortIndex].m_BulkTransf.m_Buffer);

#if LOG_LEVEL_DMA > 2
	DTU2XX_LOG(KERN_INFO,
			   "Dtu2xxTxIoCtlBulkWrite: TransferSize=%d, pBuf=0x%p, Int. Load=%d",
			   TransferSize, pBuf, pRingBuf->m_Load);
#endif

	//-.-.-.-.-.-.-.-.-.-.-.-.- Check for minimum transfer size -.-.-.-.-.-.-.-.-.-.-.-.-.

	// Do we have minimum required data?
	if ( (pRingBuf->m_Load + TransferSize) < DTU2XX_BULK_TRANSFER_ALIGN )
	{
		// First copy from user buffer to our temporary buffer
		Status = copy_from_user(pTempBuf, pBuf, TransferSize);

		// Copy all data to the internal buffer
		if ( TransferSize!=Dtu2xxBuf2RingBuffer(pTempBuf, TransferSize, pRingBuf) )
			return -EFAULT;
		else
			return 0;
	}

	// First transfer data from internal buffer
	if ( pRingBuf->m_Load>0 )
	{
		// Compute the ideal transfer size (i.e closest multiple of
		// DTU2XX_BULK_TRANSFER_ALIGN)
		NumCopy = (pRingBuf->m_Load / DTU2XX_BULK_TRANSFER_ALIGN) *
												DTU2XX_BULK_TRANSFER_ALIGN;
		if ( 0!=( pRingBuf->m_Load % DTU2XX_BULK_TRANSFER_ALIGN) )
			NumCopy += DTU2XX_BULK_TRANSFER_ALIGN;

		// If we donot have enough data for the ideal size copy as much as we can
		n = pRingBuf->m_Load + TransferSize;
		if ( NumCopy > n  )
			NumCopy = n - (n%DTU2XX_BULK_TRANSFER_ALIGN);

		// Donot copy more than what fits in our temporary buffer
		NumCopy = (NumCopy > TempBufSize) ? TempBufSize : NumCopy;
		NumCopy = NumCopy - (NumCopy % DTU2XX_BULK_TRANSFER_ALIGN);

		// Now copy data left in internal buffer to the temporary
		if ( NumCopy <= pRingBuf->m_Load )
			CopiedFromIntBuf = Dtu2xxRingBuffer2Buf(pRingBuf, NumCopy, pTempBuf);
		else
			CopiedFromIntBuf = Dtu2xxRingBuffer2Buf(pRingBuf, pRingBuf->m_Load,
													pTempBuf);

		// Do we need to add additional data from our buffer?
		if ( NumCopy > CopiedFromIntBuf )
		{
			Status = copy_from_user( &(pTempBuf[CopiedFromIntBuf]),
									 &(pBuf[BufIndex]),
									  (NumCopy-CopiedFromIntBuf) );
			// Update index
			BufIndex += (NumCopy-CopiedFromIntBuf);
		}

		// Transfer data to device
		if ( NumCopy != 0 )
		{
			Status = Dtu2xxSimpleBulkTransfer(
							pFdo, DTU2XX_DIRECTION_WRITE, EZUSB_ENDPOINT_2,
							NumCopy, pTempBuf );

			if ( Status!=0 )
				return Status;

			// Update total number of bytes written to card
			TotalNumWritten += NumCopy;
		}

		// Update data left count (do not count bytes that, where copied from the internal
		// buffer)
		BytesLeft -= (NumCopy-CopiedFromIntBuf);
	}

	//-.-.-.-.-.-.-.-.-.-.-.-.- Directly copy multiples of 1024 -.-.-.-.-.-.-.-.-.-.-.-.-.

	while ( BytesLeft>=DTU2XX_BULK_TRANSFER_ALIGN )
	{
		NumCopy = (BytesLeft>DTU2XX_BULK_PACKETSIZE) ? DTU2XX_BULK_PACKETSIZE : BytesLeft;
		// Only multiples of DTU2XX_BULK_TRANSFER_ALIGN
		NumCopy = NumCopy - (NumCopy%DTU2XX_BULK_TRANSFER_ALIGN);

		// Copy data from user buffer to our temporary buffer
		Status = copy_from_user(pTempBuf, &(pBuf[BufIndex]), NumCopy);
		if ( Status != 0 )
			break;

		Status = Dtu2xxSimpleBulkTransfer(
							pFdo,
							DTU2XX_DIRECTION_WRITE,
							EZUSB_ENDPOINT_2,
							NumCopy, pTempBuf );

		if ( Status!=0 )
			break;

		BytesLeft -= NumCopy;
		BufIndex += NumCopy;

		// Update total number of bytes written to card
		TotalNumWritten += NumCopy;
 	}

	// Update number of bytes written to card
	if ( pCh->m_EnaFifoLoadExtrap ) 
	{
		r = down_interruptible(&pCh->m_LoadExtrapLock);
		pCh->m_WrittenAfterRp += TotalNumWritten;
		up(&pCh->m_LoadExtrapLock);
	}

	//.-.-.-.-.-.-.-.-.-.-.- Copy remaining data to internal buffer -.-.-.-.-.-.-.-.-.-.-.

	if ( BytesLeft > 0 ) {

		// Sanity check: bytes left must fit in our temp and ring buffers
		if ( BytesLeft>TempBufSize || BytesLeft>pRingBuf->m_TotalSize )
			return -EFAULT;

		// First copy data from user buffer to our temporary buffer
		Status = copy_from_user(pTempBuf, &(pBuf[BufIndex]), BytesLeft);
		// Copy to ring buffer
		Dtu2xxBuf2RingBuffer(pTempBuf, BytesLeft, pRingBuf);

#if LOG_LEVEL_DMA > 2
		DTU2XX_LOG(KERN_INFO, "Dtu2xxTxIoCtlBulkWrite: Bytes left: %d, Int. Load: %d",
				   BytesLeft, pRingBuf->m_Load);
#endif
	}
	return Status;
}
