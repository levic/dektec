//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Flags.c *#*#*#*#*#*#*#*#*#* (C) 2000-2007 DekTec

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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxClearFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dta1xxClearFlags(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  ChannelType,		// DTA1XX_TS_TX_CHANNEL / DTA1XX_TS_RX_CHANNEL
	Int  PortIndex,			// Port index
	Int  FlagsToClear)		// Flags to be cleared
{
	Channel* pCh=NULL;

	// Check for valid port index
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumNonIpChannels )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxClearFlags: PortIndex=%d INVALID", PortIndex );
#endif
		return -EFAULT;
	}
	// Get channel
	pCh = &pFdo->m_Channel[PortIndex];

	// Code for Tx flags is different then code for Rx flags
	if ( ChannelType == DTA1XX_TS_TX_CHANNEL )
	{
#if LOG_LEVEL>1
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxClearFlags: TxChannel Flags=0x%x",
					PortIndex, FlagsToClear );
#endif
		//pCh->m_FlagsToClear = FlagsToClear;
		return Dta1xxTxClearLatchedStatusFlags(pCh, FlagsToClear);
	}
	else if ( ChannelType == DTA1XX_TS_RX_CHANNEL )
	{
#if LOG_LEVEL>1
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxClearFlags: RxChannel Flags=0x%x",
					PortIndex, FlagsToClear );
#endif
		//pCh->m_FlagsToClear = FlagsToClear;
		return Dta1xxRxClearLatchedStatusFlags(pCh, FlagsToClear);
	}

#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "Dta1xxClearFlags: ChannelType=%d ILLEGAL", ChannelType );
#endif
	return -EFAULT;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxGetFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read status flags.
//
Int  Dta1xxGetFlags(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	Int  ChannelType,		// DTA1XX_TS_TX_CHANNEL / DTA1XX_TS_RX_CHANNEL
	Int  PortIndex,			// Port index
	Int*  pStatus,			// Status flags
	Int*  pLatched)			// Latched status flags
{
	Int Status=0;
	Channel* pCh=NULL;

	// Check for valid port index
	if ( PortIndex<0 || PortIndex >= pFdo->m_NumNonIpChannels )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxGetFlags: PortIndex=%d INVALID", PortIndex );
#endif
		return -EFAULT;
	}
	// Get channel
	pCh = &pFdo->m_Channel[PortIndex];

	// Code for Tx flags is different then code for Rx flags
	if (ChannelType == DTA1XX_TS_TX_CHANNEL)
	{
		// Read status flags and update latched status flags
		Status = Dta1xxTxProcessStatusFlags(pCh);
		if ( Status != 0 )
			return Status;

		// Copy flags to OUT parameters
		*pStatus  = pCh->m_Status;
		*pLatched = pCh->m_Latched;

#if LOG_LEVEL>1
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxGetFlags: TxChannel Status=0x%x Latched=0x%x",
				    PortIndex, *pStatus, *pLatched );
#endif
	}
	else if (ChannelType == DTA1XX_TS_RX_CHANNEL)
	{
		// Read status flags and update latched status flags
		Status = Dta1xxRxProcessStatusFlags(pCh);
		if ( Status != 0 )
			return Status;

		// Copy flags to OUT parameters
		*pStatus  = pCh->m_Status;
		*pLatched = pCh->m_Latched;

#if LOG_LEVEL>1
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxGetFlags: RxChannel Status=0x%x Latched=0x%x",
				    PortIndex, *pStatus, *pLatched );
#endif
	}
	else
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxClearFlags: ChannelType=%d ILLEGAL",
					PortIndex, ChannelType );
#endif
		return -EFAULT;
	}
	return 0;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+ TRANSPORT-STREAM TRANSMIT CHANNEL +=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxTxClearLatchedStatusFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Clear latched status flags, as specified by FlagsToClear
//
Int  Dta1xxTxClearLatchedStatusFlags(
	Channel*  pTsTx,				// Transport-Stream Transmit Channel
	Int  FlagsToClear)			// Latched flags to clear
{
	// Clear latched flags
	pTsTx->m_Latched &= ~FlagsToClear;

	// Also clear flags in Transmit Status register, to avoid that flags in
	// m_Latched are set again in next periodic interrupt.
	if ( FlagsToClear & DTA1XX_TX_FIFO_UFL )
		Dta1xxTxStatusRegClrUflInt(pTsTx->m_pRegBase);
	if ( FlagsToClear & DTA1XX_TX_SYNC_ERR )
		Dta1xxTxStatusRegClrSyncInt(pTsTx->m_pRegBase);
	if (    (pTsTx->m_pFdo->m_TypeNumber == 102)
		&& ((FlagsToClear & DTA1XX_TX_READBACK_ERR) != 0) )
		Dta1xxTxStatusRegClrShortInt(pTsTx->m_pRegBase);

        return 0;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxTxProcessStatusFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Process flags in Transmit-Status register:
//	- Copy status flags from Tx Status register to channel's Status variable
//	- Update channel's Latched flags
//
// This routine is called:
//	- Every periodic interrupt (77ms).
//	- From DeviceIoControl (GetFlags), at elevated IRQL!
//
// PRECONDITION
//	- pTsTx points to a valid transmit channel!
//
Int Dta1xxTxProcessStatusFlags(
	Channel* pTsTx)		// Transport-Stream transmit Channel
{
	Int  Status;

	Status = 0;
	if ( 0!=Dta1xxTxStatusRegGetUflInt(pTsTx->m_pRegBase) ) {
		Status |= DTA1XX_TX_FIFO_UFL;
		Dta1xxTxStatusRegClrUflInt(pTsTx->m_pRegBase);
	}
	if ( 0!=Dta1xxTxStatusRegGetSyncInt(pTsTx->m_pRegBase) ) {
		Status |= DTA1XX_TX_SYNC_ERR;
		Dta1xxTxStatusRegClrSyncInt(pTsTx->m_pRegBase);
	}
	if ( 0!=Dta1xxTxStatusRegGetShortInt(pTsTx->m_pRegBase) ) {
		Status |= DTA1XX_TX_READBACK_ERR;
		Dta1xxTxStatusRegClrShortInt(pTsTx->m_pRegBase);
	}
	// Process target-error flag from target-detection state machine
	if (pTsTx->m_TrgIdDet.m_TargetError)
		Status |= DTA1XX_TX_TARGET_ERR;

	// TODO: DMA PENDING?!

	// Latch status flags
	pTsTx->m_Latched |= Status;
	pTsTx->m_Status = Status;

	// Clear target-error flag if target-detection state machine not in state ERROR
	if (pTsTx->m_TrgIdDet.m_TdState!=DTA1XX_TD_TARGET_ERROR)
		pTsTx->m_TrgIdDet.m_TargetError = 0;

        return 0;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+ TRANSPORT-STREAM RECEIVE CHANNEL +=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxRxClearLatchedStatusFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Clear latched status flags, as specified by FlagsToClear
//
Int  Dta1xxRxClearLatchedStatusFlags(
	Channel*  pTsRx,			// Transport-Stream Receive Channel
	Int  FlagsToClear)			// Latched flags to clear
{
	// Clear latched flags
	pTsRx->m_Latched &= ~FlagsToClear;

	// Also clear flags in Receive Status register, to avoid that flags in
	// m_Latched are set again in next periodic interrupt.
	if ( FlagsToClear & DTA1XX_RX_FIFO_OVF )
		Dta1xxRxStatusRegClrOvfInt(pTsRx->m_pRegBase);
	if ( FlagsToClear & DTA1XX_RX_SYNC_ERR )
		Dta1xxRxStatusRegClrSyncInt(pTsRx->m_pRegBase);
	if ( FlagsToClear & DTA1XX_RX_RATE_OVF )
		Dta1xxRxStatusRegClrRateOvfInt(pTsRx->m_pRegBase);

        return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxRxProcessStatusFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Process flags in Receive-Status register:
//	- Copy status flags from Rx Status register to channel's Status variable
//	- Update channel's Latched flags
//
// This routine is called:
//	- Every periodic interrupt (77ms).
//	- From DeviceIoControl (GetFlags), at elevated IRQL!
//
// PRECONDITION
//	- pTsRx points to a valid transmit channel!
//
Int Dta1xxRxProcessStatusFlags(
	Channel* pTsRx)	// Transport-Stream Receive Channel
{
	Int  Status;

	Status = 0;
	if ( 0!=Dta1xxRxStatusRegGetOvfInt(pTsRx->m_pRegBase) ) {
		Status |= DTA1XX_RX_FIFO_OVF;
		Dta1xxRxStatusRegClrOvfInt(pTsRx->m_pRegBase);
	}
	if ( 0!=Dta1xxRxStatusRegGetSyncInt(pTsRx->m_pRegBase) ) {
		Status |= DTA1XX_RX_SYNC_ERR;
		Dta1xxRxStatusRegClrSyncInt(pTsRx->m_pRegBase);
	}
	if ( 0!=Dta1xxRxStatusRegGetRateOvfInt(pTsRx->m_pRegBase) ) {
		Status |= DTA1XX_RX_RATE_OVF;
		Dta1xxRxStatusRegClrRateOvfInt(pTsRx->m_pRegBase);
	}

	// Process target-error flag from target-detection state machine
	if (pTsRx->m_TrgIdDet.m_TargetError)
		Status |= DTA1XX_TX_TARGET_ERR;

	// TODO: DMA PENDING?!

	// Latch status flags
	pTsRx->m_Latched |= Status;
	pTsRx->m_Status = Status;

	// Clear target-error flag if target-detection state machine not in state ERROR
	if (pTsRx->m_TrgIdDet.m_TdState!=DTA1XX_TD_TARGET_ERROR)
		pTsRx->m_TrgIdDet.m_TargetError = 0;

        return 0;
}
