//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* RxIoCtl.c *#*#*#*#*#*#*#*#*# (C) 2000-2008 DekTec
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlLedControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-Handler for controlling the receive status LED
//
Int  Dtu2xxRxIoCtlLedControl(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int LedControl)				// The led mode
{
	Dtu2xxRx* pRxRegs=NULL;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlLedControl: Led-control=%d",
			   PortIndex, LedControl);
#endif

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlLedControl: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pRxRegs = &(pFdo->m_Channel[PortIndex].m_RxRegs);

	// Check for valid value
	if (   LedControl!=DTU2XX_LED_HARDWARE && LedControl!=DTU2XX_LED_OFF
		&& LedControl!=DTU2XX_LED_GREEN && LedControl!=DTU2XX_LED_RED
		&& LedControl!=DTU2XX_LED_YELLOW )
	{
		return -EINVAL;
	}

	// Modify LED bits
	switch ( LedControl )
	{
	case DTU2XX_LED_HARDWARE:	pRxRegs->m_RxControl.Fields.m_LedControl	= 0;
								break;

	case DTU2XX_LED_OFF:		pRxRegs->m_RxControl.Fields.m_LedControl	= 1;
								pRxRegs->m_RxControl.Fields.m_LedGreen		= 0;
								pRxRegs->m_RxControl.Fields.m_LedRed		= 0;
                                break;

	case DTU2XX_LED_GREEN:		pRxRegs->m_RxControl.Fields.m_LedControl	= 1;
								pRxRegs->m_RxControl.Fields.m_LedGreen		= 1;
								pRxRegs->m_RxControl.Fields.m_LedRed		= 0;
                                break;

	case DTU2XX_LED_RED:		pRxRegs->m_RxControl.Fields.m_LedControl	= 1;
								pRxRegs->m_RxControl.Fields.m_LedGreen		= 0;
								pRxRegs->m_RxControl.Fields.m_LedRed		= 1;
                                break;

	case DTU2XX_LED_YELLOW:		pRxRegs->m_RxControl.Fields.m_LedControl	= 1;
								pRxRegs->m_RxControl.Fields.m_LedGreen		= 1;
								pRxRegs->m_RxControl.Fields.m_LedRed		= 1;
                                break;
	}

	// Write cached value
	return Dtu2xxIoCtlWriteRegister( pFdo, DTU2XX_RX_BASE_ADDR+DTU2XX_RX_REG_RXCTRL,
									 pRxRegs->m_RxControl.All);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlEqualise -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dtu2xxRxIoCtlEqualise(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int Mode)					// Equaliser mode
{
#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlEqualise: Mode=%d",
			   PortIndex, Mode);
#endif
	return -EFAULT;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlGetFifoLoad -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-handler for geting the fifoload
//
Int  Dtu2xxRxIoCtlGetFifoLoad(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	OUT Int* pFifoLoad)				// The fifo load
{
	Int Status = 0;
	Dtu2xxRx* pRxRegs=NULL;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetFifoLoad: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pRxRegs = &(pFdo->m_Channel[PortIndex].m_RxRegs);

	// Init to zero
	*pFifoLoad = 0;

	// Do not read from register cache but directly from hardware
	Status = Dtu2xxIoCtlReadRegister(
						pFdo,
						DTU2XX_RX_BASE_ADDR+DTU2XX_RX_REG_FIFOLOAD,
						(UInt32*)pFifoLoad );

	// Update cache
	pRxRegs->m_FifoLoad = *pFifoLoad;

	// Add load of already pre-fetched data
	*pFifoLoad += pFdo->m_Channel[PortIndex].m_BulkTransf.m_Buffer.m_Load;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetFifoLoad: Load=%d",
			   PortIndex, *pFifoLoad);
#endif
	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlGetMaxFifoSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for getting the maximum fifo size
//
Int  Dtu2xxRxIoCtlGetMaxFifoSize(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int* pSize)					// The maximum fifo size
{
	Dtu2xxRx* pRxRegs=NULL;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetMaxFifoSize: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pRxRegs = &(pFdo->m_Channel[PortIndex].m_RxRegs);

	// Decode the SDRAM size field
	switch ( pRxRegs->m_RxStatus.Fields.m_SdramSize )
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
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetMaxFifoSize: MaxSize=%d",
			   PortIndex, *pSize);
#endif
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-handler for resetting the receive channel
//
Int  Dtu2xxRxIoCtlReset(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int RstMode)					// Reset mode
{
	Int Status = 0;
	UInt8 VendCmd=0;
	UInt BytesTransf=0;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlReset: Reset-mode=%d",
			   PortIndex, RstMode);
#endif

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlReset: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Check for valid value
	if ( RstMode!=DTU2XX_RST_CHANNEL && RstMode!=DTU2XX_RST_CLEARFIFO )
		return -EINVAL;

	if ( RstMode==DTU2XX_RST_CHANNEL ) {
		VendCmd = DTU2XX_USB_RX_RESET;
	} else {
		VendCmd = DTU2XX_USB_RX_CLEAR_FIFO;
	}

	// Clear all flags
	Dtu2xxRxIoCtlClearFlags(pFdo, PortIndex, 0xFFFFFFFF);

	// Clear internal buffer
	pFdo->m_Channel[PortIndex].m_BulkTransf.m_Buffer.m_pRdPtr =
				pFdo->m_Channel[PortIndex].m_BulkTransf.m_Buffer.m_pWrPtr;
	pFdo->m_Channel[PortIndex].m_BulkTransf.m_Buffer.m_Load = 0;

	// Simply send the vendor command to the device
	Status = Dtu2xxVendorRequest(pFdo, VendCmd, 0,0, DTU2XX_DIRECTION_WRITE,0,
								 NULL, &BytesTransf);
	if ( Status != 0 )
		return Status;

	// We need to update all cached registers after a reset
	return Dtu2xxCacheOpRegs(pFdo);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlRxPolarity -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-control handler for setting the receive-polarity
//
Int  Dtu2xxRxIoCtlRxPolarity(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int Polarity)				// Polarity
{
	Dtu2xxRx* pRxRegs=NULL;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlRxPolarity: Polarity=%d",
			   PortIndex, Polarity);
#endif

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlRxPolarity: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pRxRegs = &(pFdo->m_Channel[PortIndex].m_RxRegs);

	// Check for valid values
	if (   Polarity!=DTU2XX_POLARITY_AUTO && Polarity!=DTU2XX_POLARITY_NORMAL
		&& Polarity!=DTU2XX_POLARITY_INVERT )
		return -EINVAL;

	// Set polarity
	pRxRegs->m_RxControl.Fields.m_AsiInv = Polarity;

	// Update register value to match cache
	return Dtu2xxIoCtlWriteRegister( pFdo, DTU2XX_RX_BASE_ADDR+DTU2XX_RX_REG_RXCTRL,
									 pRxRegs->m_RxControl.All);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlGetFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-Handler for getting the status flags
//
Int  Dtu2xxRxIoCtlGetFlags(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	OUT Int* pStatus,				// The current status flags
	OUT Int* pLatched)				// The latched status flags
{
	Int Status = 0;
	Dtu2xxRx* pRxRegs=NULL;


	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetFlags: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pRxRegs = &(pFdo->m_Channel[PortIndex].m_RxRegs);

	// Init to zero
	*pStatus = *pLatched = 0;

	// Update cache
	Status = Dtu2xxIoCtlReadRegister( pFdo, DTU2XX_RX_BASE_ADDR+DTU2XX_RX_REG_RXSTATUS,
									  &(pRxRegs->m_RxStatus.All));
	if ( Status != 0 )
		return Status;

	// Check for status flags
	if ( pRxRegs->m_RxStatus.Fields.m_OvfInt==1 )
		*pStatus |= DTU2XX_RX_FIFO_OVF;
	if ( pRxRegs->m_RxStatus.Fields.m_SyncInt==1 )
		*pStatus |= DTU2XX_RX_SYNC_ERR;
	if ( pRxRegs->m_RxStatus.Fields.m_RateOvfInt==1 )
		*pStatus |= DTU2XX_RX_RATE_OVF;

	// Clear status flags (write back register value)
	Status = Dtu2xxIoCtlWriteRegister( pFdo, DTU2XX_RX_BASE_ADDR+DTU2XX_RX_REG_RXSTATUS,
									   pRxRegs->m_RxStatus.All);
	if ( Status != 0 )
		return Status;

	// Clear flags in cache
	pRxRegs->m_RxStatus.All &= ~0x3F00;

	// Update the status of the latched flags
	pFdo->m_Channel[PortIndex].m_Latched |= *pStatus;
	// Return the latched flags
	*pLatched = pFdo->m_Channel[PortIndex].m_Latched;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetFlags: Status=0x%08X, Latched=0x%08X",
			   PortIndex, *pStatus, *pLatched);
#endif
	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlClearFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-Handler for clear the latched status flags
//
Int  Dtu2xxRxIoCtlClearFlags(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int Latched)
{
	Int Status=0;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlClearFlags: Latched=0x%08X",
			   PortIndex, Latched);
#endif

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlClearFlags: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Clear the latched flags
	pFdo->m_Channel[PortIndex].m_Latched &= ~Latched;

	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlGetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-handler for getting the input status
//
Int  Dtu2xxRxIoCtlGetStatus(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	OUT Int* pPacketSize,			// The packet size
	OUT Int* pClkDet,				// Clock/Carrier detected
	OUT Int* pAsiLock,				// ASI lock
	OUT Int* pRateOk,				// Input rate is OK
	OUT Int* pAsiInv)				// DVB-ASI inversion status
{
	Int Status = 0;
	Dtu2xxRx* pRxRegs=NULL;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetStatus: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pRxRegs = &(pFdo->m_Channel[PortIndex].m_RxRegs);

	// Init to not-supported
	*pPacketSize	= DTU2XX_NOT_SUPPORTED;
	*pClkDet		= DTU2XX_NOT_SUPPORTED;
	*pAsiLock		= DTU2XX_NOT_SUPPORTED;
	*pRateOk		= DTU2XX_NOT_SUPPORTED;
	*pAsiInv		= DTU2XX_NOT_SUPPORTED;

	// Update cache
	Status = Dtu2xxIoCtlReadRegister( pFdo, DTU2XX_RX_BASE_ADDR+DTU2XX_RX_REG_RXSTATUS,
									  &(pRxRegs->m_RxStatus.All));
	if ( Status != 0 )
		return Status;

	// Get status fields
	*pPacketSize	= pRxRegs->m_RxStatus.Fields.m_PckSize;
	*pClkDet		= pRxRegs->m_RxStatus.Fields.m_ClkDet;
	*pAsiLock		= pRxRegs->m_RxStatus.Fields.m_AsiLock;
	*pRateOk		= pRxRegs->m_RxStatus.Fields.m_RateOk;
	// Inversion status is support by DTU-245 and DTU-2x5 based boards
	if ( pFdo->m_TypeNumber==245 || pFdo->m_IsDtu2x5Hw )
		*pAsiInv	= pRxRegs->m_RxStatus.Fields.m_AsiInv;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetStatus: Pck-size=%d, ClkDet=%d, "
			   "AsiLock=%d, RateOk=%d, AsiInv=%d", PortIndex, *pPacketSize, *pClkDet,
			   *pAsiLock, *pRateOk, *pAsiInv);
#endif
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlSetRxControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-handler for setting the recive control
//
Int  Dtu2xxRxIoCtlSetRxControl(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int RxControl)				// The receive control value
{
	Dtu2xxRx* pRxRegs=NULL;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlSetRxControl: Rx-control=%d",
			   PortIndex, RxControl);
#endif

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlSetRxControl: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pRxRegs = &(pFdo->m_Channel[PortIndex].m_RxRegs);

	// Check for valid value
	if ( RxControl!=DTU2XX_RXCTRL_IDLE && RxControl!=DTU2XX_RXCTRL_RCV )
		return -EINVAL;

	// Set control field in cache
	pRxRegs->m_RxControl.Fields.m_RxCtrl = RxControl;

	// Update register value to match cache
	return Dtu2xxIoCtlWriteRegister( pFdo, DTU2XX_RX_BASE_ADDR+DTU2XX_RX_REG_RXCTRL,
									 pRxRegs->m_RxControl.All);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlGetRxControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-Handler for getting the receive-control value
//
Int  Dtu2xxRxIoCtlGetRxControl(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	OUT Int* pRxControl)			// The receive control value
{
	Dtu2xxRx* pRxRegs=NULL;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetRxControl: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pRxRegs = &(pFdo->m_Channel[PortIndex].m_RxRegs);

	// Get from cache
	*pRxControl = pRxRegs->m_RxControl.Fields.m_RxCtrl;
#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetRxControl: Rx-control=%d",
			   PortIndex, *pRxControl);
#endif
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlSetRxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-handler for setting the Receive-Mode
//
Int  Dtu2xxRxIoCtlSetRxMode(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int RxMode)					// The receive mode
{
	Int StMode;
	Dtu2xxRx* pRxRegs=NULL;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlSetRxMode: Rx-mode=0x%08X",
			   PortIndex, RxMode);
#endif

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlSetRxMode: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pRxRegs = &(pFdo->m_Channel[PortIndex].m_RxRegs);

	StMode = RxMode & DTU2XX_RXMODE_MASK;

	// Check for valid values
	// The checks are already performed in the DTAPI, so it's not realy needed here
	if (    StMode != DTU2XX_RXMODE_ST188 && StMode != DTU2XX_RXMODE_ST204
		 && StMode != DTU2XX_RXMODE_STRAW && StMode != DTU2XX_RXMODE_STMP2
		 && StMode != DTU2XX_RXMODE_STTRP
		 && StMode != (DTU2XX_RXMODE_SDI_FULL & ~DTU2XX_RXMODE_SDI)
		 && StMode != (DTU2XX_RXMODE_SDI_ACTVID & ~DTU2XX_RXMODE_SDI) )
	{
		return -EINVAL;
	}

	if ( 0!=(RxMode & ~(  DTU2XX_RXMODE_MASK | DTU2XX_RX_TIMESTAMP | DTU2XX_RXMODE_SDI
		                | DTU2XX_RXMODE_SDI_10B | DTU2XX_RXMODE_SDI_HUFFMAN)) )
	{
		return -EINVAL;
	}

	// Set mode bits
	pRxRegs->m_RxControl.Fields.m_RxMode = StMode&0x3;
	pRxRegs->m_RxControl.Fields.m_RxModeExt = (StMode&0x4)>>2;

	// Set timestamp
	if ( 0 != (RxMode & DTU2XX_RX_TIMESTAMP) )
		pRxRegs->m_RxControl.Fields.m_RxTimeStamp = 1;
	else
		pRxRegs->m_RxControl.Fields.m_RxTimeStamp = 0;

	// Set SDI Mode
	if ( 0!=(RxMode & DTU2XX_RXMODE_SDI) )
		pRxRegs->m_RxControl.Fields.m_SdiMode = 1;
	else
		pRxRegs->m_RxControl.Fields.m_SdiMode = 0;

	// Set SDI 10Bit Mode
	if ( 0!=(RxMode & DTU2XX_RXMODE_SDI_10B) )
		pRxRegs->m_RxControl.Fields.m_Sdi10Bit = 1;
	else
		pRxRegs->m_RxControl.Fields.m_Sdi10Bit = 0;

	// Set SDI Huffman compression
	if ( 0!=(RxMode & DTU2XX_RXMODE_SDI_HUFFMAN) )
		pRxRegs->m_RxControl.Fields.m_SdiHuffEn = 1;
	else
		pRxRegs->m_RxControl.Fields.m_SdiHuffEn = 0;


	// Update register to match cache
	return Dtu2xxIoCtlWriteRegister( pFdo, DTU2XX_RX_BASE_ADDR+DTU2XX_RX_REG_RXCTRL,
									 pRxRegs->m_RxControl.All);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlGetRxMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-handler for getting the Receive-Mode
//
Int  Dtu2xxRxIoCtlGetRxMode(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	OUT Int* pRxMode)				// The receive mode
{
	Dtu2xxRx* pRxRegs=NULL;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetRxMode: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pRxRegs = &(pFdo->m_Channel[PortIndex].m_RxRegs);

	// Get mode from cache
	*pRxMode = pRxRegs->m_RxControl.Fields.m_RxMode;
	*pRxMode |= (pRxRegs->m_RxControl.Fields.m_RxModeExt<<2);
	// Check for Timestamp mode
	if ( pRxRegs->m_RxControl.Fields.m_RxTimeStamp == 1 )
		*pRxMode |= DTU2XX_RX_TIMESTAMP;
	// Check for SDI Mode
	if ( pRxRegs->m_RxControl.Fields.m_SdiMode == 1 )
		*pRxMode |= DTU2XX_RXMODE_SDI;
	// Check for SDI 10Bit Mode
	if ( pRxRegs->m_RxControl.Fields.m_Sdi10Bit == 1 )
		*pRxMode |= DTU2XX_RXMODE_SDI_10B;
	// Check for huffman compressed
	if ( pRxRegs->m_RxControl.Fields.m_SdiHuffEn == 1 )
		*pRxMode |= DTU2XX_RXMODE_SDI_HUFFMAN;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetRxMode: Rx-mode=0x%08X",
			   PortIndex, *pRxMode);
#endif

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlGetTsRateBps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-handler for getting the the bit-rate
//
Int  Dtu2xxRxIoCtlGetTsRateBps(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	OUT Int* pTsRate)				// The bit-rate
{
	Int Status = 0;
	Dtu2xxRx* pRxRegs=NULL;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetTsRateBps: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pRxRegs = &(pFdo->m_Channel[PortIndex].m_RxRegs);

	// Init to zero
	*pTsRate = 0;

	// Read the bit-rate register (not from cache)
	Status = Dtu2xxIoCtlReadRegister( pFdo, DTU2XX_RX_BASE_ADDR+DTU2XX_RX_REG_BITRATE,
									  (UInt32*)pTsRate );
	if ( Status != 0 )
		return Status;

	// Update cache to match current register value
	pRxRegs->m_Bitrate = *pTsRate;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetTsRateBps: Ts-rate=%d",
			   PortIndex, *pTsRate);
#endif
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlGetViolationCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Io-handler for getting the the violation count
//
Int  Dtu2xxRxIoCtlGetViolationCount(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	OUT Int* pViolCount)			// Value of violation count register
{
	Int Status = 0;
	Dtu2xxRx* pRxRegs=NULL;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetViolationCount: INVALID PORT NUMBER "
				   "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Get register cache
	pRxRegs = &(pFdo->m_Channel[PortIndex].m_RxRegs);

	// Init to zero
	*pViolCount = 0;

	// Do not read from register cache but directly from hardware
	Status = Dtu2xxIoCtlReadRegister(
						pFdo,
						DTU2XX_RX_BASE_ADDR+DTU2XX_RX_REG_VIOLCOUNT,
						(UInt32*)pViolCount );

	// Update cache
	pRxRegs->m_CodeViolCnt = *pViolCount;

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlGetViolationCount: Viol-count=%d",
			   PortIndex, *pViolCount);
#endif
	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlSetLoopBackMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dtu2xxRxIoCtlSetLoopBackMode(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int Mode)					// The mode
{
#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlSetLoopBackMode: Mode=%d",
			   PortIndex, Mode);
#endif
	return -EFAULT;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxRxIoCtlBulkRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read data from the DTU-2XX (uses bulk transfers)
//
Int  Dtu2xxRxIoCtlBulkRead(
	IN PDTU2XX_FDO pFdo,			// Our device object
	IN Int PortIndex,				// Port index
	IN Int TransferSize,			// Number of bytes to transfer
	OUT Int* pBytesRead,			// Number of bytes actually read
	OUT UInt8* pBuf)				// The buffer to receive data
{
	Int Status = 0;
	UInt8* pTempBuf;
	Int BytesLeft, BufIndex, ByteCount, NumCopy, Prefetch, TempBufSize;
	PDTU2XX_RING_BUFFER pRingBuf = NULL;

	// Check port index is valid
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumChannels ) {
		DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlBulkRead: INVALID PORT NUMBER "
							  "NumChannels=%d", PortIndex, pFdo->m_NumChannels);
		return -EINVAL;
	}

	// Init to zero
	*pBytesRead = 0;

	// Handle nothing to transfer case
	if ( TransferSize == 0 )
		return 0;
	// Check for valid buffer
	if ( pBuf == NULL )
		return -EINVAL;

	BytesLeft = TransferSize;
	BufIndex = 0;

	TempBufSize = pFdo->m_Channel[PortIndex].m_TempBufSize;
	pTempBuf = pFdo->m_Channel[PortIndex].m_pTempBuf;
	pRingBuf = &(pFdo->m_Channel[PortIndex].m_BulkTransf.m_Buffer);

#if LOG_LEVEL_DMA > 2
	DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxRxIoCtlBulkRead: TransferSize=%d",
			   PortIndex, TransferSize);
#endif

	//-.-.-.-.-.-.-.-.-.-.- First check for data already pre-fetched -.-.-.-.-.-.-.-.-.-.-

	if ( pRingBuf->m_Load != 0 )
	{
		NumCopy = (pRingBuf->m_Load > TransferSize) ? TransferSize : pRingBuf->m_Load;
		// Copy from ring buffer to user buffer
		Dtu2xxRingBuffer2Buf(pRingBuf, NumCopy, pTempBuf);
		Status = copy_to_user(&(pBuf[BufIndex]), pTempBuf, NumCopy);
		// Update # bytes left to copy + index
		BytesLeft -= NumCopy;
		BufIndex += NumCopy;
	}
	if ( BytesLeft == 0 )
		return 0;

	//.-.-.-.-.-.-.- Directly copy multiples of DTU2XX_BULK_TRANSFER_ALIGN -.-.-.-.-.-.-.-

	while ( BytesLeft>=DTU2XX_BULK_TRANSFER_ALIGN  )
	{
		ByteCount = (BytesLeft>DTU2XX_BULK_PACKETSIZE) ?
										DTU2XX_BULK_PACKETSIZE : BytesLeft;
		// Only multiples of DTU2XX_BULK_TRANSFER_ALIGN
		ByteCount = ByteCount - (ByteCount % DTU2XX_BULK_TRANSFER_ALIGN);

		// Execute bulk transfer
		Status = Dtu2xxSimpleBulkTransfer(
							pFdo,
							DTU2XX_DIRECTION_READ,
							EZUSB_ENDPOINT_6,
							ByteCount, pTempBuf );

		if ( Status!=0 )
		{
#if LOG_LEVEL_DMA > 0
			DTU2XX_LOG(KERN_INFO, "[%d] Dtu2xxSimpleBulkTransfer: Failed=0x%08X",
					   PortIndex, Status);
#endif
			break;
		}
		// Copy to user buffer
		Status = copy_to_user(&(pBuf[BufIndex]), pTempBuf, ByteCount);

		BytesLeft -= ByteCount;
		BufIndex += ByteCount;
 	}
	if ( Status != 0 || BytesLeft==0 )
		return Status;

	//.-.-.- Get final bytes by pre-fetching multiple DTU2XX_BULK_TRANSFER_ALIGN -.-.-.

	// First prefetch as much as needed (must be multiple of DTU2XX_BULK_TRANSFER_ALIGN)
	Prefetch = (DTU2XX_BULK_TRANSFER_ALIGN-(BytesLeft%DTU2XX_BULK_TRANSFER_ALIGN))
						+ BytesLeft;


	// Transfer data to our temporary buffer
	Status = Dtu2xxSimpleBulkTransfer( pFdo, DTU2XX_DIRECTION_READ,
									   EZUSB_ENDPOINT_6, Prefetch,
									   pTempBuf );

	if ( Status == 0 )
	{
		// Copy remaining data to target buf
		Status = copy_to_user(&(pBuf[BufIndex]), pTempBuf, BytesLeft);

		// Store left over in ring buffer
		NumCopy = (Prefetch-BytesLeft);
		Dtu2xxBuf2RingBuffer(&pTempBuf[BytesLeft], NumCopy, pRingBuf);
    }
	return Status;
}
