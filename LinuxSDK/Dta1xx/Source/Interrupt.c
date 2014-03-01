//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Interrupt.c *#*#*#*#*#*#*#*#* (C) 2000-2007 DekTec

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dta1xx driver, please refer to the Dta1xx readme

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <linux/version.h>
#include "../Include/Dta1xx.h"
#include "../Include/Dta1xxRegs.h"
#include <asm/bitops.h> // set_bit

// Check kernel version
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,4,0) && LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0)
	#define KERNELVERSION 24
#endif

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,5,0) && LINUX_VERSION_CODE < KERNEL_VERSION(2,7,0)
	#define KERNELVERSION 26
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local Forwards -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  Dta1xxPeriodicIntHandler(DTA1XX_FDO* pFdo);
void  Dta1xxDmaIntHandler(Channel* pCh);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxIsr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Interrupt service routine. Determines which interrupt(s), if any, is asserted and calls
// the appropiate interrupt handler
//
unsigned long Dta1xxIsr(
	int Irq,				// IRQ number (NOT USED)
	void* pContext,			// pointer to the FDO object
	struct pt_regs *pRegs)	// snapshot of processors contexts (NOT USED)
{
	Int i, PlxDmaChannel;
	UInt32  IntCsr=0, CmdStat=0;
	Channel*  pCh = NULL; // Channel pointer
	volatile UInt8*  pPci905XRegs=NULL;
	DTA1XX_FDO*  pFdo = (DTA1XX_FDO*)pContext;
	BOOLEAN  UseGenReg=FALSE, OurInterrupt=FALSE;

	// Check interrupt status in PLX
	if ( pFdo->m_UsesPlxChip )
	{
		// Get pointer to PCI-9054/9056
		pPci905XRegs = pFdo->m_Pci905XConfRegs.m_pKernel;
		// Read Interrupt Control / Status register in PCI 9054/9056
		IntCsr = READ_UINT32(pPci905XRegs, PCI905X_INT_CTRL_STAT);

		// Check for master PCI interrupt enable
		if ( (IntCsr & PCI905X_INTCSR_PCI_INTEN)==0 )
		{
			// Can't be ours
	#if KERNELVERSION < 26
			return 0;
	#else
			return IRQ_NONE;
	#endif
		}
	}

	// Can we use the General-Register block?
	UseGenReg = pFdo->m_UsesGenReg;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Check periodic interrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	if (   ( UseGenReg && 0!=Dta1xxGenStatusRegGetPerInt(pFdo->m_pGenRegs))
		|| ( !UseGenReg && 0!=Dta1xxTxStatusRegGetPerInt(pFdo->m_Channel[0].m_pRegBase)) )
	{
		// Call periodic interrupt handler
		Dta1xxPeriodicIntHandler(pFdo);

		// Process periodic interrupt for each network port
		for (i=0; i<pFdo->m_NumIpPorts; i++) {

			// Handle packet transmit IpTx
			InsertQueueDpc(&(pFdo->m_NwStore[i].m_IpTxDPC));

			// Handle bitrate measurement IpRx
			InsertQueueDpc(&pFdo->m_DpcBrmIpRx);
			
			// Handle MII Serial Management protocol
			Dta1xxNwMacMiiSmCheckState(&(pFdo->m_NwStore[i]));
		}

		OurInterrupt = TRUE;
	}

	// Check for DMA done interrupt
	if ( pFdo->m_UsesDmaInFpga )
	{
		// Check all channels
		for ( i=0; i<pFdo->m_NumChannels; i++ )
		{
			pCh = &pFdo->m_Channel[i];

			CmdStat = READ_UINT32(pCh->m_Dma.m_pRegCmdStat, 0);
			// DMA interrupt from this channel?
			if ( (CmdStat & DTA1XX_DMACSR_INTACT) != 0 )
			{
				// Call DMA interrupt handler
				Dta1xxDmaIntHandler(pCh);
				OurInterrupt = TRUE;
			}
		}
	}
	else
	{
		// DMA controller in PLX: check for DMA channel interrupts in PCI905X

		for ( PlxDmaChannel=0; PlxDmaChannel<2; PlxDmaChannel++ )
		{
			// Check for DMA channel 0 or 1 interrupt
			if ( PlxDmaChannel==0 && (IntCsr & PCI905X_INTCSR_DMA0_INTACT)==0 )
				continue;	// Skip; no DMA channel 0 interupt
			if ( PlxDmaChannel==1 && (IntCsr & PCI905X_INTCSR_DMA1_INTACT)==0 )
				continue;	// Skip; no DMA channel 1 interupt

			// Find channel related our PLX DMA channel
			pCh = NULL;
			for ( i=0; i<pFdo->m_NumChannels; i++ )
			{
				pCh = &pFdo->m_Channel[i];
				if ( pCh->m_Dma.m_PlxChannel==PlxDmaChannel )
					break;
				pCh = NULL;
			}
			// Call DMA interrupt handler
			if ( pCh != NULL )
				Dta1xxDmaIntHandler(pCh);
			OurInterrupt =  TRUE;
		}
	}

	// Other ethernet interrupts
	if (pFdo->m_NumIpPorts != 0) {
		UInt RxStat;

		for (i=0; i<pFdo->m_NumIpPorts; i++) {

			// Is MDIO interrupt active?
			if ((Dta1xxNwIntStatGet(pFdo->m_NwStore[i].m_pRegBase) & 
										DTA1XX_NWINTCTRL_MDIOINTEN) != 0) {
				// Disable interrupt enable, clear interrupt and process it
				Dta1xxNwIntCtrlSetMdioIntEn(pFdo->m_NwStore[i].m_pRegBase, FALSE);

				if (Dta1xxNwMacMiiSmCheckState(&(pFdo->m_NwStore[i]))) {
					// If no other actions are busy, clear interrupt
					Dta1xxNwMacClearInterrupts(&(pFdo->m_NwStore[i]));
				} else {
					pFdo->m_NwStore[i].m_InterruptPending = TRUE;
				}
				
				OurInterrupt = TRUE;

				// When the interrupt clear is ready, the Link status is read
				// automatically in the state machine
			}

			// Due to a bug in the DTA-160 firmware the IpRxFifoOvfInt can be set 
			// when disabled. So clear this flag if enabled
			RxStat = Dta1xxNwRxStatGet(pFdo->m_IpMqRx[i]->m_pRegBase);
			RxStat = RxStat & (DTA1XX_NWRX_CTRL_VAL_CNT_OVF_INT_EN |
							   DTA1XX_NWRX_CTRL_INV_CNT_OVF_INT_EN |
							   DTA1XX_NWRX_CTRL_RX_FIFO_OVF_INT_EN); 
			
			if (RxStat) {
				if (RxStat & DTA1XX_NWRX_CTRL_RX_FIFO_OVF_INT_EN) {
					pFdo->m_NwStore[i].m_NumRxFifoOverflows++;
				}
				// We don't do anything with the other interrupts, 
				// but we have to report it to windows
				OurInterrupt = TRUE;
			}

			// Reset the RxStat interrupts
			Dta1xxNwRxStatSet(pFdo->m_IpMqRx[i]->m_pRegBase, RxStat);			
			
		}
	}

	// Initial LED flashing
	if (pFdo->m_LedFlashStartUp)
	{
		// Compute next state
		switch (pFdo->m_LedFlashState) {

		case DTA1XX_LEDFLASH_INIT:
			if (pFdo->m_LedFlashStateCounter >= pFdo->m_LedFlashDurationInit) {
				Dta1xxGenSetLed(pFdo, 1);
				pFdo->m_LedFlashState = DTA1XX_LEDFLASH_ON;
				pFdo->m_LedFlashStateCounter = 0;
			}
			break;

		case DTA1XX_LEDFLASH_ON:
			if (pFdo->m_LedFlashStateCounter >= pFdo->m_LedFlashDurationOn) {
				Dta1xxGenSetLed(pFdo, 0);
				pFdo->m_LedFlashState = DTA1XX_LEDFLASH_OFF;
				pFdo->m_LedFlashStateCounter = 0;
			}
			break;

		case DTA1XX_LEDFLASH_OFF:
			if (pFdo->m_LedFlashStateCounter >= pFdo->m_LedFlashDurationOff) {
				pFdo->m_LedFlashFlashCounter++;
				if (pFdo->m_LedFlashFlashCounter >= pFdo->m_LedFlashNumFlashes) {
					pFdo->m_LedFlashState = DTA1XX_LEDFLASH_EXIT;
				} else {
					Dta1xxGenSetLed(pFdo, 1);
					pFdo->m_LedFlashState = DTA1XX_LEDFLASH_ON;
				}
				pFdo->m_LedFlashStateCounter = 0;
			}
			break;

		case DTA1XX_LEDFLASH_EXIT:
			if (pFdo->m_LedFlashStateCounter >= pFdo->m_LedFlashDurationExit) {
				Dta1xxGenSetLed(pFdo, -1);
				pFdo->m_LedFlashStartUp = FALSE;
			}
			break;
		}
		pFdo->m_LedFlashStateCounter++;
	}

#if KERNELVERSION < 26
	return 0;
#else
	return (OurInterrupt) ? IRQ_HANDLED : IRQ_NONE;
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxDisableInterrupts -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Disables the interrupts used by the driver
//
void Dta1xxDisableInterrupts(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	UInt32  IntCsr;
	volatile UInt8*  pPci905XRegs = pFdo->m_Pci905XConfRegs.m_pKernel;

	//.-.-.-.-.-.-.-.-.- Reset the global interrupt enable bits in PLX -.-.-.-.-.-.-.-.-.-
	//
	// Also disable the DMA channel 0 & 1 interrupts
	//
	if ( pFdo->m_UsesPlxChip )
	{
		IntCsr = READ_UINT32(pPci905XRegs, PCI905X_INT_CTRL_STAT);
		IntCsr &= ~(  PCI905X_INTCSR_PCI_INTEN  | PCI905X_INTCSR_LOCAL_INTEN
					| PCI905X_INTCSR_DMA0_INTEN | PCI905X_INTCSR_DMA1_INTEN );
		WRITE_UINT32(pPci905XRegs, PCI905X_INT_CTRL_STAT, IntCsr);
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.- Reset the cards interrupt bits -.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	// The driver only uses the periodic interrupt
	//
	if ( pFdo->m_UsesGenReg )
		Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 0);
	else // LEGACY
		Dta1xxTxCtrlRegSetTxPerIntEn(pFdo->m_Channel[0].m_pRegBase, 0);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxEnableInterrupts -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Enables the interrupts used by the driver
//
void Dta1xxEnableInterrupts(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	UInt32  IntCsr;
	volatile UInt8*  pPci905XRegs = pFdo->m_Pci905XConfRegs.m_pKernel;

	// Enable interrupts in PCI905X
	if (pFdo->m_UsesPlxChip)
	{
		IntCsr = READ_UINT32(pPci905XRegs, PCI905X_INT_CTRL_STAT);

		// NOTE: DTA-124 uses DMA interrupts on local bus!
		IntCsr |= PCI905X_INTCSR_PCI_INTEN
				| PCI905X_INTCSR_LOCAL_INTEN
				| PCI905X_INTCSR_DMA0_INTEN;

		// DTA-111/112/115/116/117/124/140/160: enable interrupt for DMA channel 1 too
		if (   pFdo->m_TypeNumber==111
			|| pFdo->m_TypeNumber==112
			|| pFdo->m_TypeNumber==115
			|| pFdo->m_TypeNumber==116
			|| pFdo->m_TypeNumber==117
			|| pFdo->m_TypeNumber==124
			|| pFdo->m_TypeNumber==140
			|| pFdo->m_TypeNumber==160)
			IntCsr |= PCI905X_INTCSR_DMA1_INTEN;

		WRITE_UINT32(pPci905XRegs, PCI905X_INT_CTRL_STAT, IntCsr);
	}

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- Set the cards interrupt bits -.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// The driver only uses the periodic interrupt
	//
	if ( pFdo->m_UsesGenReg  )
		Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 1);
	else // LEGACY
		Dta1xxTxCtrlRegSetTxPerIntEn(pFdo->m_Channel[0].m_pRegBase, 1);
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Interrupt handlers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxPeriodicIntHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Periodic Interrupt handler routine.
//
void Dta1xxPeriodicIntHandler(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	Channel* pCh=NULL;
	Int  i, PortIndex, dDiff, EldestIndex, NewIndex, ValidCnt, ValidDiff;
	Int64  ValidCountTemp=0;
	BOOLEAN FoundGenlockRef=FALSE;
	BitRateMeas*  pBrm;				// Bit-rate measurement variables

	// First clear periodic interrupt flag (without clearing other status flags!)
	if ( pFdo->m_UsesGenReg )
		Dta1xxGenStatusRegClrPerInt(pFdo->m_pGenRegs);
	else
		Dta1xxTxStatusRegClrPerInt(pFdo->m_Channel[0].m_pRegBase);

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Genlock processing -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
		
	// Check for reference channels
	FoundGenlockRef = FALSE;
	for ( i=0; i<pFdo->m_NumChannels; i++ )
	{
		if ( pFdo->m_Channel[i].m_IoConfig==DTA1XX_IOCONFIG_GENREF ) {
			Dta1xxUpdateGenlockTiming(pFdo);
			Dta1xxUpdateGenlockRx(pFdo);
			FoundGenlockRef = TRUE;
			break;	// Only need to call methods above once 
		}
	}

	// Check for genlocked outputs
	for ( i=0; i<pFdo->m_NumChannels && FoundGenlockRef; i++ )
	{
		if ((pFdo->m_Channel[i].m_IoConfig==DTA1XX_IOCONFIG_GENREF)) {
			Dta1xxUpdateGenlockTx(pFdo);
			break;	// Only need to call methods above once 
		}
	}
	// note: revise code above for crossboard genlock

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Process failsafe outputs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	for ( PortIndex=0; PortIndex<pFdo->m_NumNonIpChannels; PortIndex++ )
	{
		pCh = &pFdo->m_Channel[PortIndex];
		if ( pCh->m_Capability & DTA1XX_CHCAP_FAILSAFE )
		{
			if ( pCh->m_IoConfig == DTA1XX_IOCONFIG_FAILSAFE )
			{
				// Check if application is still alive
				if (   ( pCh->m_FailsafeTimeoutCnt == pCh->m_FailsafeCnt )
					|| (!pCh->m_FailsafeEnable) )
				{
					// Application did not respond within time or Failsafe disabled
				}
				else
				{
					pCh->m_FailsafeCnt++;
					Dta1xxGenPulseWatchdog(pFdo->m_pGenRegs);
				}
			}
			else
			{
				// No failsafe enabled, toggle always
				Dta1xxGenPulseWatchdog( pFdo->m_pGenRegs );
			}
		}
	}

	//-.-.-.-.-.-.-.-.-.-.-.-.- Read valid-counter sample values -.-.-.-.-.-.-.-.-.-.-.-.-

	if ( pFdo->m_TypeNumber==102 )
	{
		pCh = &pFdo->m_Channel[0];
		// Valid-Count register in DTA-102 shares it's position with Tx-Clock register
		// on other cards
		pCh->m_BrMeas.m_ValidCountSample = Dta1xxTxGetClkCountReg(pCh->m_pRegBase);
	}
	else
	{
		// Read valid counter for all Rx channels
		for (PortIndex=0; PortIndex<pFdo->m_NumChannels; PortIndex++)
		{
			pCh = &pFdo->m_Channel[PortIndex];
			// Check for RX channel
			if ( pCh->m_ChannelType != DTA1XX_TS_RX_CHANNEL )
				continue;

			pCh->m_BrMeas.m_ValidCountSample =
						Dta1xxRxGetValidCountReg(pCh->m_pRegBase);
		}
	}

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Estimate Input Bit Rate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	for ( PortIndex=0; PortIndex<pFdo->m_NumNonIpChannels; PortIndex++ )
	{
		// For DTA-102					: Estimate frequency of External-Clock input
		// For DTA-112/115/116/117/120/122/124/140/145/2135/2142/2144/2145/160/2160/545
		//								: Estimate rate of Transport-Stream input
		if (    pFdo->m_TypeNumber==102 ||  pFdo->m_TypeNumber==120
			||  pFdo->m_TypeNumber==122 ||  pFdo->m_TypeNumber==124
			|| (pFdo->m_TypeNumber==140 && PortIndex==1)
			|| ((pFdo->m_TypeNumber==112  || pFdo->m_TypeNumber==115  ||
				 pFdo->m_TypeNumber==116  || pFdo->m_TypeNumber==117  ||
				 pFdo->m_TypeNumber==145  || pFdo->m_TypeNumber==160  ||
				 pFdo->m_TypeNumber==545  ||
				 pFdo->m_TypeNumber==2135 || pFdo->m_TypeNumber==2137 ||
				 pFdo->m_TypeNumber==2142 || pFdo->m_TypeNumber==2144 || 
				 pFdo->m_TypeNumber==2145 || pFdo->m_TypeNumber==2160) &&
				pFdo->m_Channel[PortIndex].m_ChannelType==DTA1XX_TS_RX_CHANNEL))
		{

			if ( pFdo->m_TypeNumber==102 )
				pBrm = &pFdo->m_Channel[0].m_BrMeas;
			else if ( pFdo->m_TypeNumber==140 )
				pBrm = &pFdo->m_Channel[0].m_BrMeas;	// Rx channel
			else
				pBrm = &pFdo->m_Channel[PortIndex].m_BrMeas;

			// Get Valid Counter and compute difference with last difference
			ValidCnt = pBrm->m_ValidCountSample;
			ValidDiff = ValidCnt - pBrm->m_LastValidCnt;
			pBrm->m_LastValidCnt = ValidCnt;

			// Compute index of location where to store new sample, with wrap-around
			NewIndex = pBrm->m_LastIndex + 1;
			if (NewIndex >= NUM_RX_MEAS_PERIODS)
				NewIndex = 0;

			// Compute index of eldest sample
			EldestIndex = NewIndex - pBrm->m_NumValidSamps;
			if (EldestIndex < 0)
				EldestIndex += NUM_RX_MEAS_PERIODS;

			// Compute delta between new and last difference
			dDiff = ValidDiff - pBrm->m_ValidDiff[pBrm->m_LastIndex];
			if (dDiff<-2 || dDiff>2) {
				//.-.-.- New difference differs significantly from previous difference -.-.-.-

				// Start all over again
				pBrm->m_NumBytesTotal = 0;
				pBrm->m_NumValidSamps = 1;
			} else {
				//.-.-.-.-.-.-.- New difference is close to previous difference -.-.-.-.-.-.-.

				// If buffer is full => subtract eldest element
				if (pBrm->m_NumValidSamps == NUM_RX_MEAS_PERIODS) {
					pBrm->m_NumBytesTotal -= pBrm->m_ValidDiff[EldestIndex];
				} else	// Buffer not full => we have an extra sample
					pBrm->m_NumValidSamps++;
			}

			// Process the Valid-Counter differential value
			pBrm->m_ValidDiff[NewIndex] = ValidDiff;
			pBrm->m_NumBytesTotal += ValidDiff;

			// Next time, last index is current new index
			pBrm->m_LastIndex = NewIndex;

			// Compute estimate for Valid Count
			ValidCountTemp  = (Int64)pBrm->m_NumBytesTotal;
			ValidCountTemp <<= 8; // x256
			// THE FOLLOWING LINE CAUSES A LINK ERROR "unresolved symbol __divdi3"
			//ValidCountTemp /= (Int64)pBrm->m_NumValidSamps;
			ValidCountTemp = Dta1xxBinDiv(ValidCountTemp, 
						      (Int64)pBrm->m_NumValidSamps, NULL);
			pBrm->m_ValidCount256 =	(Int)ValidCountTemp;
		}
	}

	//.-.-.-.-.-.-.-.-.-.-.-.- DTA-102, DTA-122: Target Detection -.-.-.-.-.-.-.-.-.-.-.-.

	if (pFdo->m_TypeNumber==102 || pFdo->m_TypeNumber==122) {
		Dta1xxTargetDetectStateMachine(pFdo);		// Target detection
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Statistics -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	// For DTA-112/115/116/117/120/140/145/160/545/2135/2142/2144/2145/2160:
	// Sample code-violation counter
	if ( pFdo->m_TypeNumber==112  || pFdo->m_TypeNumber==115  ||
		 pFdo->m_TypeNumber==116  || pFdo->m_TypeNumber==117  ||
		 pFdo->m_TypeNumber==120  || pFdo->m_TypeNumber==140  ||
		 pFdo->m_TypeNumber==145  || pFdo->m_TypeNumber==160  ||
		 pFdo->m_TypeNumber==545  ||
		 pFdo->m_TypeNumber==2135 || pFdo->m_TypeNumber==2137 ||
		 pFdo->m_TypeNumber==2142 || pFdo->m_TypeNumber==2144 || 
		 pFdo->m_TypeNumber==2145 || pFdo->m_TypeNumber==2160)
	{
		for (PortIndex=0; PortIndex<pFdo->m_NumNonIpChannels; PortIndex++)
		{
			pCh = &pFdo->m_Channel[PortIndex];
			if ( pCh->m_ChannelType==DTA1XX_TS_RX_CHANNEL )
				pCh->m_ViolCountSamp = Dta1xxRxGetViolCountReg(pCh->m_pRegBase);
		}
	}

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Process status flags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	for (PortIndex=0; PortIndex<pFdo->m_NumNonIpChannels; PortIndex++)
	{
		pCh = &pFdo->m_Channel[PortIndex];
		if ( pCh->m_ChannelType==DTA1XX_TS_RX_CHANNEL )
		{
			// Process Rx-status flags
			Dta1xxRxProcessStatusFlags(pCh);

			// Make sure that all interrupt flags are removed. Even ones unused by driver
			Dta1xxRxSetRxStatusReg(  pCh->m_pRegBase, 
								 	 DTA1XX_RXSTAT_PERINT  | DTA1XX_RXSTAT_OVFINT
								   | DTA1XX_RXSTAT_SYNCINT | DTA1XX_RXSTAT_THRINT
								   | DTA1XX_RXSTAT_RATEOVFINT );

		}
		else if ( pCh->m_ChannelType==DTA1XX_TS_TX_CHANNEL )
		{
			// Process Tx-status flags
			Dta1xxTxProcessStatusFlags(pCh);

			// Make sure that all interrupt flags are removed. Even ones unused by driver
			Dta1xxTxSetTxStatusReg(  pCh->m_pRegBase,
									 DTA1XX_TXSTAT_PERINT   | DTA1XX_TXSTAT_UFLINT
								   | DTA1XX_TXSTAT_SYNCINT  | DTA1XX_TXSTAT_THRINT
								   | DTA1XX_TXSTAT_SHORTINT );

		}
	}
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxDmaIntHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// DMA done interrupt handler
//
void  Dta1xxDmaIntHandler(
	Channel* pCh)			// Channel pointer
{
	UInt32 RefClkSamp, DescPtr;
	UInt Pci905XDmaDescPtrOffset, Pci905XDmaCommandStatOffset;
	DTA1XX_FDO* pFdo = pCh->m_pFdo;
	volatile UInt8*  pPci905XRegs=NULL;

	// For new cards that use DMA controller in Altera (e.g. DTA-105/115/124/160/2160/545):
	// Check for DMA channel interrupts in operational registers, not in PLX
	if ( pFdo->m_UsesDmaInFpga )
	{
		UInt32  CmdStat;

		CmdStat = READ_UINT32(pCh->m_Dma.m_pRegCmdStat, 0);
		
		// Clear DMA Channel interrupt (disables the channel, too)
		WRITE_UINT32(pCh->m_Dma.m_pRegCmdStat, 0, PCI905X_DMACSR_CLEARINT);

		// If DMA is from a IpRx channel, we set the number of bytes not finished
		// in the DMA status field
		if (pCh->m_ChannelType == DTA1XX_IP_RX_CHANNEL) {
			pCh->m_Dma.m_CurrentTransferLength = 0;

			if ((CmdStat & PCI905X_DMACSR_DONE) == 0) {
				// Timeout => Retrieve the number of bytes still to transfer
				pCh->m_Dma.m_CurrentTransferLength = 
									   ((CmdStat & DTA1XX_DMA_CMDSTAT_SIZE) >> 8);
			}
			// Set the DMA done flag
			set_bit(0, (void*)&pCh->m_CdmaDone);

			// Awaken any waiting threads
			wake_up_interruptible( &pCh->m_CdmaEvent );

		} else if (pCh->m_ChannelType == DTA1XX_IP_TX_CHANNEL) {
			// Queue CDMA-completed DPC
			InsertQueueDpc(&(pCh->m_CdmaDpcCompleted));
		} else {

			// Set the DMA done flag
			set_bit(0, (void*)&pCh->m_Dma.m_DmaDone);

			// Awaken any waiting threads
			wake_up_interruptible( &pCh->m_Dma.m_DmaDoneQueue );
		}
	}
	else
	{	// DMA controller in PLX

		// Init PLX register helpers
		if ( pCh->m_Dma.m_PlxChannel==0 )
		{
			Pci905XDmaDescPtrOffset = PCI905X_DMA0_DESC_PTR;
			Pci905XDmaCommandStatOffset = PCI905X_DMA0_COMMAND_STAT;
		}
		else /*if ( pCh->m_Dma.m_PlxChannel==1 )*/
		{
			Pci905XDmaDescPtrOffset = PCI905X_DMA1_DESC_PTR;
			Pci905XDmaCommandStatOffset = PCI905X_DMA1_COMMAND_STAT;
		}
		// Get pointer to PCI-9054/9056
		pPci905XRegs = pFdo->m_Pci905XConfRegs.m_pKernel;

		// Is this CDMA (Continuous DMA)?
		if ( pCh->m_CdmaDmaRunning )
		{
			// CDMA: this is a "CDMA-buffer-segment completed" interrupt

			// Get sample of the reference clock register
			RefClkSamp = Dta1xxGenGetRefClkCntReg(pFdo->m_pGenRegs);
			// Read DMA Channel X Descriptor Pointer to figure out where we are
			DescPtr = READ_UINT32(pPci905XRegs, Pci905XDmaDescPtrOffset);

			// Clear DMA Channel X interrupt, but leave DMA enabled
			WRITE_UINT8(pPci905XRegs, Pci905XDmaCommandStatOffset,
						PCI905X_DMACSR_ENABLE | PCI905X_DMACSR_CLEARINT);

			// Store reference pointer
			pCh->m_CdmaRefClkSamp = RefClkSamp;

			// Derive which segment is being processed now
			// If the Scatter-Gather-List pointer at the instance of the interrupt points to an
			// address in segment 0, then the current segment is also segment #0
			if		(DescPtr < pCh->m_CdmaSegmDesc[1].m_SglAddr)
				pCh->m_CdmaSegment = 0;
			else if (DescPtr < pCh->m_CdmaSegmDesc[2].m_SglAddr)
				pCh->m_CdmaSegment = 1;
			else if (DescPtr < pCh->m_CdmaSegmDesc[3].m_SglAddr)
				pCh->m_CdmaSegment = 2;
			else
				pCh->m_CdmaSegment = 3;

			// Set the CDMA done flag
			set_bit(0, (void*)&pCh->m_CdmaDone);
			// Awaken any waiting threads
			wake_up_interruptible( &pCh->m_CdmaEvent );
		}
		else
		{
			// "Normal" DMA

		    // Clear DMA Channel 0 interrupt (disables the channel, too)
			WRITE_UINT8(pPci905XRegs, Pci905XDmaCommandStatOffset,
						PCI905X_DMACSR_CLEARINT);

			// Set the DMA done flag
			set_bit(0, (void*)&pCh->m_Dma.m_DmaDone);
			// Awaken any waiting threads
	   		wake_up_interruptible( &pCh->m_Dma.m_DmaDoneQueue );
		}
	}
}
