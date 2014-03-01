//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dma.c *#*#*#*#*#*#*#*#*#*# (C) 2000-2008 DekTec

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
#include <asm/bitops.h> // set_bit
#include <linux/version.h>

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxDmaUserMemRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Transfer data from card to user buffer using DMA
//
Int  Dta1xxDmaUserMemRead(
	DTA1XX_FDO* pFdo,	// Functional device object, representing the DTA-1xx card
	Int  ChannelType,	// DTA1XX_TS_RX_CHANNEL
	Int  PortIndex,		// Port index
	UInt8* pUserBuf,	// User buffer receiving data
	Int NumBytes2Transf)// Number of bytes to transfer
{
	UInt8* pWrPos=NULL;
	Channel* pCh=NULL;
	UInt32 LocalDmaAddr;
	Int Status=0, NumBytesLeft, DmaBurstSize;

	// Check channel type
	if ( ChannelType != DTA1XX_TS_RX_CHANNEL )
	{
#if LOG_LEVEL_DMA > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxDmaUserMemRead: Channel-Type=%d INVALID",
					ChannelType );
#endif
		return -EFAULT;
	}
	// Check port index
	if ( PortIndex<0 || PortIndex>pFdo->m_NumNonIpChannels )
	{
#if LOG_LEVEL_DMA > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxDmaUserMemRead: PortIndex=%d INVALID",
					PortIndex );
#endif
		return -EFAULT;
	}

	// Number of bytes to transfer must be a multiple of 4
	if ( NumBytes2Transf<0 || (NumBytes2Transf%4)!=0)
	{
#if LOG_LEVEL_DMA > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxDmaUserMemRead: #bytes to transfer=%d INVALID",
					NumBytes2Transf );
#endif
		return -EFAULT;
	}
	// User-buffer address must 32-bit aligned
	if ( ((UIntPtr)pUserBuf)%4 !=0 )
	{
#if LOG_LEVEL_DMA > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxDmaUserMemRead: user buffer=%p INVALID",
					pUserBuf );
#endif
		return -EFAULT;
	}

	// Get channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// Divide transfer in burst with a max. size of our DMA buffer

	pWrPos = pUserBuf;
	NumBytesLeft = NumBytes2Transf;

	LocalDmaAddr = (UInt32)(   pCh->m_pRegBase
                                 + DTA1XX_RX_REG_FIFO_FIRST
                                 - pFdo->m_Dta1xxRegsBase.m_pKernel );

	// Acquire DMA lock
	if ( 0!=down_interruptible( &pCh->m_Dma.m_Lock ) )
		return -EFAULT;

	while ( NumBytesLeft > 0 )
	{
		// Determine DMA burst size
		DmaBurstSize = (NumBytesLeft <= DTA1XX_DMA_BUF_SIZE) ?
								NumBytesLeft : DTA1XX_DMA_BUF_SIZE;
		DmaBurstSize &= ~0x3;	// Must be 32-bit aligned

		// Start DMA transfer
		Status = Dta1xxDmaTransfer(pCh, LocalDmaAddr, DmaBurstSize, DTA1XX_DMA_READ);
		if ( Status != 0 ) {

			// Release lock
			up(&pCh->m_Dma.m_Lock);
			return Status;
		}

		// Copy the data from DMA buffer to user buffer
		Status = copy_to_user(pWrPos, pCh->m_Dma.m_DmaBuffer.m_pVirtStartAddr, DmaBurstSize);
		if ( Status!=0 )
		{
#if LOG_LEVEL_DMA > 0
			DTA1XX_LOG( KERN_INFO, "Dta1xxDmaUserMemRead: FAIL: copy data to user" );
#endif
			// Release lock
			up(&pCh->m_Dma.m_Lock);
			return -EFAULT;
		}

		// Move write pointer and update number of bytes left to transfer
		pWrPos += DmaBurstSize;
		NumBytesLeft -= DmaBurstSize;
	}

	// Release lock
	up(&pCh->m_Dma.m_Lock);
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxDmaUserMemWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Transfer data from user buffer to card using DMA
//
Int  Dta1xxDmaUserMemWrite(
	DTA1XX_FDO* pFdo,	// Functional device object, representing the DTA-1xx card
	Int  ChannelType,	// DTA1XX_TS_TX_CHANNEL
	Int  PortIndex,		// Port index
	UInt8* pUserBuf,	// User buffer with data to be transferred
	Int NumBytes2Transf)// Number of bytes to transfer
{
	UInt8* pRdPos=NULL;
	Channel* pCh=NULL;
	UInt32 LocalDmaAddr;
	Int Status=0, NumBytesLeft, DmaBurstSize;

	// Check channel type
	if ( ChannelType != DTA1XX_TS_TX_CHANNEL )
	{
#if LOG_LEVEL_DMA > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxDmaUserMemWrite: Channel-Type=%d INVALID",
					ChannelType );
#endif
		return -EFAULT;
	}
	// Check port index
	if ( PortIndex<0 || PortIndex>pFdo->m_NumNonIpChannels )
	{
#if LOG_LEVEL_DMA > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxDmaUserMemWrite: PortIndex=%d INVALID",
					PortIndex );
#endif
		return -EFAULT;
	}

	// Number of bytes to transfer must be a multiple of 4
	if ( NumBytes2Transf<0 || (NumBytes2Transf%4)!=0)
	{
#if LOG_LEVEL_DMA > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxDmaUserMemWrite: #bytes to transfer=%d INVALID",
					NumBytes2Transf );
#endif
		return -EFAULT;
	}
	// User-buffer address must 32-bit aligned
	if ( ((UIntPtr)pUserBuf)%4 !=0 )
	{
#if LOG_LEVEL_DMA > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxDmaUserMemWrite: user buffer=%p INVALID",
					pUserBuf );
#endif
		return -EFAULT;
	}

	// Get channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// Divide transfer in burst with a max. size of our DMA buffer

	pRdPos = pUserBuf;
	NumBytesLeft = NumBytes2Transf;

	LocalDmaAddr = (UInt32)(   pCh->m_pRegBase
                                 + DTA1XX_TX_REG_FIFO_FIRST
				 - pFdo->m_Dta1xxRegsBase.m_pKernel );

	// Acquire DMA lock
	if ( 0!=down_interruptible( &pCh->m_Dma.m_Lock ) )
		return -EFAULT;

	while ( NumBytesLeft > 0 )
	{
		// Determine DMA burst size
		DmaBurstSize = (NumBytesLeft <= DTA1XX_DMA_BUF_SIZE) ?
								NumBytesLeft : DTA1XX_DMA_BUF_SIZE;
		DmaBurstSize &= ~0x3;	// Must be 32-bit aligned

		// Copy the data from user buffer to DMA buffer
		Status = copy_from_user(pCh->m_Dma.m_DmaBuffer.m_pVirtStartAddr, pRdPos, DmaBurstSize);
		if ( Status!=0 )
		{
#if LOG_LEVEL_DMA > 0
			DTA1XX_LOG( KERN_INFO, "Dta1xxDmaUserMemWrite: FAIL: copy data from user" );
#endif
			// Release lock
			up(&pCh->m_Dma.m_Lock);
			return -EFAULT;
		}

		// Start DMA transfer
		Status = Dta1xxDmaTransfer(pCh, LocalDmaAddr, DmaBurstSize, DTA1XX_DMA_WRITE);
		if ( Status != 0 ) {
			// Release lock
			up(&pCh->m_Dma.m_Lock);
			return Status;
		}

		// Move read pointer and update number of bytes left to transfer
		pRdPos += DmaBurstSize;
		NumBytesLeft -= DmaBurstSize;
	}

	// Release lock
	up(&pCh->m_Dma.m_Lock);
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxDmaKernelMemWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Transfer data from kernel buffer to card using DMA
//
Int  Dta1xxDmaKernelMemWrite(
	Channel* pCh,
	UInt8* pKernelBuf,	// Kernel buffer with data to be transferred.
	Int NumBytes2Transf)// Number of bytes to transfer
{
	DTA1XX_FDO* pFdo = pCh->m_pFdo;
	UInt32 LocalDmaAddr;
	Int Status=0, NumBytesLeft, DmaBurstSize;
	UInt8* pSrcBuf;

	// Check channel type
	if (pCh->m_ChannelType != DTA1XX_NW_TX_CHANNEL)
	{
#if LOG_LEVEL_DMA > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxDmaKernelMemWrite: Channel-Type=%d INVALID",
					pCh->m_ChannelType );
#endif
		return -EFAULT;
	}
	
	// Number of bytes to transfer must be a multiple of 4
	if ( NumBytes2Transf<0 )/*|| (NumBytes2Transf%4)!=0)*/
	{
#if LOG_LEVEL_DMA > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxDmaKernelMemWrite: #bytes to transfer=%d INVALID",
					NumBytes2Transf );
#endif
		return -EFAULT;
	}
	// Kernel-buffer address must 32-bit aligned
	if ( ((UIntPtr)pKernelBuf)%4 !=0 )
	{
#if LOG_LEVEL_DMA > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxDmaKernelMemWrite: kernel buffer=%p INVALID",
					pKernelBuf );
#endif
		return -EFAULT;
	}

	LocalDmaAddr = (UInt32)(   pCh->m_pRegBase
                                 + DTA1XX_TX_REG_FIFO_FIRST
								 - pFdo->m_Dta1xxRegsBase.m_pKernel );

	// Acquire DMA lock
	if ( 0!=down_interruptible( &pCh->m_Dma.m_Lock ) )
		return -EFAULT;

	NumBytesLeft = NumBytes2Transf;
	pSrcBuf = pKernelBuf;
	while (NumBytesLeft > 0)
	{
		// Determine burst size
		DmaBurstSize = (NumBytesLeft <= DTA1XX_DMA_BUF_SIZE) ? NumBytesLeft : DTA1XX_DMA_BUF_SIZE;

		// Copy data to dma buffer
		memcpy(pCh->m_Dma.m_DmaBuffer.m_pVirtStartAddr, pSrcBuf, DmaBurstSize);
	
		// Start DMA transfer
		Status = Dta1xxDmaTransfer(pCh, LocalDmaAddr, DmaBurstSize, DTA1XX_DMA_WRITE);

		if (Status != 0)
		{
			// Release lock
			up(&pCh->m_Dma.m_Lock);
			return Status;
		}

		NumBytesLeft -= DmaBurstSize;
		pSrcBuf += DmaBurstSize;
	}

	// Release lock
	up(&pCh->m_Dma.m_Lock);
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxDmaTransfer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Executes a DMA transfer.
//
// Pre: The channel dma lock must be acquired
//
Int Dta1xxDmaTransfer(
	Channel* pCh,			// Channel pointer
	UInt32 LocalDmaAddr,	// Local address of the DMA transfer (local to DTA-1xx)
	Int DmaSize,			// Size of the DMA transfer
	Int DmaDir)				// DMA direction (DTA1XX_DMA_READ/DTA1XX_DMA_WRITE)
{
	Int ret=0;
	UInt DmaDesc, CmdStat;
	SglDesc* pSglDesc=NULL;
        DTA1XX_FDO* pFdo = pCh->m_pFdo;
	volatile UInt8* pPci905xConfRegs = pFdo->m_Pci905XConfRegs.m_pKernel;

	// "Normal" DMAs and CDMAs are mutual exclusive
	if ( pCh->m_CdmaDmaRunning )
		return -EFAULT;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Setup/Start the DMA -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	// Reset DMA done flag
	clear_bit( 0, (void*)&pCh->m_Dma.m_DmaDone );

	if ( pFdo->m_UsesDmaInFpga )
	{
		// Prepare scatter/gather descriptor
		pSglDesc = pCh->m_Dma.m_pSglDesc;

		pSglDesc->m_32PciAddress	= (UInt32)pCh->m_Dma.m_DmaBuffer.m_PhysStartAddr;
		pSglDesc->m_32TransferSize	= (UInt32)DmaSize;
		pSglDesc->m_32LocalAddress	= (UInt32)LocalDmaAddr;
		// This is only descriptor so point to itself
		pSglDesc->m_32NextSglDesc	= ((UInt32)pCh->m_Dma.m_PhysAddrSglDesc) & ~0x3;
        // Set direction
		if ( DmaDir == DTA1XX_DMA_READ )
			pSglDesc->m_32NextSglDesc |= PCI905X_DMADPR_DIROFTFR;
		// Mark as last descriptor
		pSglDesc->m_32NextSglDesc |= PCI905X_DMADPR_ENDOFCHAIN;
		// Use "Interrupt after Terminal Count" as DMA-done indicator
		pSglDesc->m_32NextSglDesc |= PCI905X_DMADPR_INTAFTERTC;

		// Write pointer to first SGL in DMA-Channel-0-Descriptor-Pointer register
		DmaDesc = pCh->m_Dma.m_PhysAddrSglDesc | PCI905X_DMADPR_DESCLOC_PCI;
		if ( DmaDir == DTA1XX_DMA_READ)
			DmaDesc |= PCI905X_DMADPR_DIROFTFR;
		WRITE_UINT32(pCh->m_Dma.m_pRegDmaDesc, 0, DmaDesc);

		// Start transfer!
		// NOTE: be careful with using WRITE_BYTE to access local registers
		//       in the ALTERA. No byte enables!!!
		CmdStat = PCI905X_DMACSR_ENABLE | PCI905X_DMACSR_START;
		WRITE_UINT8(pCh->m_Dma.m_pRegCmdStat, 0, CmdStat);
	}
	else
	{
		// PLX register helpers
		UInt Pci905XDmaMode, Pci905XDmaPciAddr, Pci905XDmaLocalAddr, Pci905XDmaCount, Pci905XDmaDescPtr,
			 Pci905XDmaCommandStat;

		// Check which PLX channel we are using
		if ( pCh->m_Dma.m_PlxChannel==0 )
		{
			Pci905XDmaMode = PCI905X_DMA0_MODE;
			Pci905XDmaPciAddr = PCI905X_DMA0_PCI_ADDR;
			Pci905XDmaLocalAddr = PCI905X_DMA0_LOCAL_ADDR;
			Pci905XDmaCount = PCI905X_DMA0_COUNT;
			Pci905XDmaDescPtr = PCI905X_DMA0_DESC_PTR;
			Pci905XDmaCommandStat = PCI905X_DMA0_COMMAND_STAT;
		}
		else /*if ( pCh->m_Dma.m_PlxChannel==1 )*/
		{
			Pci905XDmaMode = PCI905X_DMA1_MODE;
			Pci905XDmaPciAddr = PCI905X_DMA1_PCI_ADDR;
			Pci905XDmaLocalAddr = PCI905X_DMA1_LOCAL_ADDR;
			Pci905XDmaCount = PCI905X_DMA1_COUNT;
			Pci905XDmaDescPtr = PCI905X_DMA1_DESC_PTR;
			Pci905XDmaCommandStat = PCI905X_DMA1_COMMAND_STAT;
		}

		// Enable Block Mode in PCI 9054
		WRITE_UINT32(pPci905xConfRegs, Pci905XDmaMode,
					 PCI905X_DMAMODE_BUSWIDTH_32 | PCI905X_DMAMODE_READY_INPEN	|
					 PCI905X_DMAMODE_LOCALBURST	 /*| PCI905X_DMAMODE_SCATGATH*/ |
					 PCI905X_DMAMODE_DONE_INTEN	 | PCI905X_DMAMODE_LOC_CONST	|
					 PCI905X_DMAMODE_DEMAND		 | PCI905X_DMAMODE_INT2PCI);

		// Write target address
		WRITE_UINT32(pPci905xConfRegs, Pci905XDmaPciAddr,
					 (UInt32)pCh->m_Dma.m_DmaBuffer.m_PhysStartAddr );

		// Write local address
		WRITE_UINT32(pPci905xConfRegs, Pci905XDmaLocalAddr, LocalDmaAddr);

		// Write transfer size
		WRITE_UINT32(pPci905xConfRegs, Pci905XDmaCount, DmaSize);

		// Set transfer direction
		if ( DmaDir == DTA1XX_DMA_READ )
			WRITE_UINT32(pPci905xConfRegs, Pci905XDmaDescPtr, 8);
		else
			WRITE_UINT32(pPci905xConfRegs, Pci905XDmaDescPtr, 0);

		// Start transfer!
		WRITE_UINT8(pPci905xConfRegs, Pci905XDmaCommandStat,
					PCI905X_DMACSR_ENABLE | PCI905X_DMACSR_START);
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Wait for DMA done flag -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	ret = wait_event_interruptible( pCh->m_Dma.m_DmaDoneQueue,
     	     						( test_bit(0, (void*)&pCh->m_Dma.m_DmaDone) ) );

	// Interrupted by a signal?
	if ( ret != 0 /*-ERESTARTSYS*/)
	{
		// Have to cancel the current DMA
		if ( pFdo->m_UsesDmaInFpga )
			WRITE_UINT8(pCh->m_Dma.m_pRegCmdStat, 0, PCI905X_DMACSR_ABORT);
		else if ( pCh->m_Dma.m_PlxChannel==0 )
			WRITE_UINT8( pPci905xConfRegs, PCI905X_DMA0_COMMAND_STAT,
						 PCI905X_DMACSR_ABORT );
		else /*if ( pCh->m_Dma.m_PlxChannel==1 )*/
			WRITE_UINT8( pPci905xConfRegs, PCI905X_DMA1_COMMAND_STAT,
						 PCI905X_DMACSR_ABORT );
	}

	// Did we cancel the DMA (we were interrupted by a signal)
	return (ret!=0) ? -EFAULT : 0;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ CONTINUOUS DMA (CDMA) +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Set of DMA routines for executing Continuous DMA (CDMA). The last descriptor of the
// scatter/gather list points back to the first one.
// Application:
// - continuous streaming of I/Q sample values from a user program to a modulator card.
// - continuous DMA buffer for IpTx

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCdmaCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  Dta1xxCdmaCtrl(
	DTA1XX_FDO* pFdo,		// Functional device object, representing the DTA-1xx card
	DTA1XX_CDMA_CONTROL* pCdmaCtrl)	// CDMA Control input and output parameters
{
	Int Status=0;
	Channel* pCh=NULL;
	Int PortIndex = pCdmaCtrl->m_PortIndex;
	Int ChannelType = pCdmaCtrl->m_ChannelType;
	Int TypeNumber = pFdo->m_TypeNumber;
	BOOLEAN  Is110_112_115_116_117_2111 = (   TypeNumber==110 || TypeNumber==112
									  || TypeNumber==115 || TypeNumber==116
									  || TypeNumber==117 || TypeNumber==2111 );


	// For now only support for outputs
	if ( ChannelType != DTA1XX_TS_TX_CHANNEL ) {
#if LOG_LEVEL_CDMA > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxCdmaCtrl: NO SUPPORT FOR CHANNEL TYPE %d; "
					"SHOULD BE 0 (DTA1XX_TS_TX_CHANNEL)", ChannelType );
#endif
		return -EFAULT;
	}
	// Check for valid port index
	if (   (PortIndex!=0 && TypeNumber==110) 
		|| (PortIndex!=0 && TypeNumber==111) || (PortIndex!=1 && TypeNumber==112)
		|| (PortIndex!=1 && TypeNumber==115) || (PortIndex!=1 && TypeNumber==116)
		|| (PortIndex!=1 && TypeNumber==117) || (PortIndex!=1 && TypeNumber==2111) )
	{
#if LOG_LEVEL_CDMA > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxCdmaCtrl: NO SUPPORT FOR PORT INDEX %d",
					PortIndex );
#endif
		return -EFAULT;
	}
	// Check for support cards
	if ( !Is110_112_115_116_117_2111 ) {
#if LOG_LEVEL_CDMA > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxCdmaCtrl: ONLY FOR DTA-110/112/115/116/117/2111");
#endif
		return -EFAULT;
	}
	// Set channel pointer
	pCh = &pFdo->m_Channel[PortIndex];

	// Execute control code
	switch ( pCdmaCtrl->m_Control )
	{
	case DTA1XX_CDMA_START:
		Status = Dta1xxCdmaStart(pCh);
		break;

	case DTA1XX_CDMA_STOP:
		Status = Dta1xxCdmaStop(pCh);
		break;

	case DTA1XX_CDMA_WAITSEGM:
		Status = Dta1xxCdmaWaitSegment(pCh, pCdmaCtrl);
		break;

	case DTA1XX_CDMA_GETBUFPOS:
		Status = Dta1xxCdmaGetBufPos(pCh, pCdmaCtrl);
		break;

	default:
#if LOG_LEVEL_CDMA > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaCtrl: Control=%d UNKNOWN",
					pCdmaCtrl->m_PortIndex, pCdmaCtrl->m_Control );
#endif
		return -EFAULT;
	}
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCdmaInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Initialise the Sgl descriptor pointer
//
Int  Dta1xxCdmaInit(
	IN Channel*  pCh)			// Channel pointer
{
	Int  Status = 0;
	PDTA1XX_FDO  pFdo = pCh->m_pFdo;
	
	// CDMA may not be running
	if (pCh->m_CdmaDmaRunning) {
#if LOG_LEVEL_CDMA > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaInit: FAIL: CDMA ALREADY RUNNING",
				    pCh->m_PortIndex);
#endif
		return -EFAULT;
	}

#if LOG_LEVEL_CDMA > 0
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaInit", pCh->m_PortIndex );
#endif
	if (pFdo->m_UsesDmaInFpga) {
		UInt32 DescAddr;
		// Write pointer to first SGL in DMA-Channel-x-Descriptor-Pointer register
		DescAddr = pCh->m_CdmaShBuf.m_PhysAddrSglBuf + pCh->m_CdmaShBuf.m_FirstDescOffset;
		if (pCh->m_ChannelType == DTA1XX_IP_RX_CHANNEL) {
			DescAddr |= PCI905X_DMADPR_DIROFTFR;
		}
		WRITE_UINT32(pCh->m_Dma.m_pRegDmaDesc, 0, 
						DescAddr | PCI905X_DMADPR_DESCLOC_PCI);
	}
	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCdmaStart -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Start continuous streaming of computed sample values
//
Int  Dta1xxCdmaStart(
	Channel* pCh)			// Channel pointer
{
	Int  Status=0, Pci905xDmaMode, Pci905xDmaDescPtr, Pci905xDmaCommandStat;
	volatile UInt8*  pPci905XRegs = pCh->m_pFdo->m_Pci905XConfRegs.m_pKernel;
	UInt32  DescAddr;

	// CDMA starts with reading segment 0
	// In device extension, intialise CDMA segment# currently being DMA-ed to 0, and
	// get associated reference-clock sample
	pCh->m_CdmaSegment = 0;
	pCh->m_CdmaRefClkSamp = Dta1xxGenGetRefClkCntReg(pCh->m_pFdo->m_pGenRegs);

	// Reset CDMA done flag
	clear_bit( 0, (void*)&pCh->m_CdmaDone);

	// CDMA may not be running already
	if (pCh->m_CdmaDmaRunning)
	{
#if LOG_LEVEL_CDMA > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaStart: FAIL: CDMA ALREADY RUNNING",
					pCh->m_PortIndex );
#endif
		return -EFAULT;
	}
#if LOG_LEVEL_CDMA > 0
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaStart: RefClk=%u, PLX Channel=%d",
				pCh->m_PortIndex, (UInt)(pCh->m_CdmaRefClkSamp),
				pCh->m_Dma.m_PlxChannel );
#endif

	// Check which PLX DMA channel we should use and init address helpers
	if ( pCh->m_Dma.m_PlxChannel == 0 )
	{
		Pci905xDmaMode = PCI905X_DMA0_MODE;
		Pci905xDmaDescPtr = PCI905X_DMA0_DESC_PTR;
		Pci905xDmaCommandStat = PCI905X_DMA0_COMMAND_STAT;
	}
	else /*if ( pCh->m_Dma.m_PlxChannel == 1 )*/
	{
		Pci905xDmaMode = PCI905X_DMA1_MODE;
		Pci905xDmaDescPtr = PCI905X_DMA1_DESC_PTR;
		Pci905xDmaCommandStat = PCI905X_DMA1_COMMAND_STAT;
	}

	// Enable Scatter/Gather Mode in PCI 9054/9056
	WRITE_UINT32(pPci905XRegs, Pci905xDmaMode,
				 PCI905X_DMAMODE_BUSWIDTH_32 | PCI905X_DMAMODE_READY_INPEN	|
				 PCI905X_DMAMODE_LOCALBURST	 | PCI905X_DMAMODE_SCATGATH		|
				 PCI905X_DMAMODE_LOC_CONST	|
				 PCI905X_DMAMODE_DEMAND		 | PCI905X_DMAMODE_INT2PCI);

	// Write pointer to first SGL in DMA-Channel-x-Descriptor-Pointer register
	DescAddr = pCh->m_CdmaShBuf.m_PhysAddrSglBuf + pCh->m_CdmaShBuf.m_FirstDescOffset;
	
	if (pCh->m_ChannelType == DTA1XX_IP_RX_CHANNEL) {
		DescAddr |= PCI905X_DMADPR_DIROFTFR;
	}
		
	WRITE_UINT32(pPci905XRegs, Pci905xDmaDescPtr, DescAddr | PCI905X_DMADPR_DESCLOC_PCI);

	// Start transfer!
	WRITE_UINT32(pPci905XRegs, Pci905xDmaCommandStat,
				PCI905X_DMACSR_ENABLE | PCI905X_DMACSR_START);

	// Mark CDMA as running in device extension
	pCh->m_CdmaDmaRunning = TRUE;
	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCdmaStartSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Start continuous DMA with specified size for IpTx
// Pre: pCh->m_Dma.m_CurrentTransferLength: bytes to transfer. Must be multiple of 4.
//
Int  Dta1xxCdmaStartSize(
	IN Channel*  pCh)			// Channel pointer
{
	NTSTATUS  Status = STATUS_SUCCESS;
	PDTA1XX_FDO  pFdo = pCh->m_pFdo;
	
	ASSERT((pCh->m_Dma.m_CurrentTransferLength !=0) &&
		   (pCh->m_Dma.m_CurrentTransferLength %4 == 0));

#if LOG_LEVEL_CDMA > 1
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaStartSize: Transfer size=%u",
			    pCh->m_PortIndex, pCh->m_Dma.m_CurrentTransferLength );
#endif
	// Reset CDMA done flag
	clear_bit( 0, (void*)&pCh->m_CdmaDone);

	// Flush the buffer from cache
	pci_dma_sync_sg_for_device(pCh->m_pFdo->m_pPdo, 
								pCh->m_CdmaShBuf.m_pSglKernel, 
								pCh->m_CdmaShBuf.m_NumPages,
								DMA_TO_DEVICE);

	if (pFdo->m_UsesDmaInFpga) {
		UInt32 CmdStat;
		// Skip writing SglDescr pointer. It's already writting during the init
		// Set the DMA size and DMA size enable bits in the command/status register
		CmdStat =  (pCh->m_Dma.m_CurrentTransferLength << 8) | DTA1XX_DMA_CMDSTAT_SIZE_EN;
		CmdStat |= PCI905X_DMACSR_ENABLE | PCI905X_DMACSR_START;
		// Start transfer!
		WRITE_UINT32(pCh->m_Dma.m_pRegCmdStat, 0, CmdStat);
	} else {
#if LOG_LEVEL_CDMA > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaStartSize: NOT SUPPORTED",
				    pCh->m_PortIndex);
#endif
		return -EFAULT;
	}
	
	// Mark CDMA as running in device extension
	pCh->m_CdmaDmaRunning = TRUE;

	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCdmaStartSegment -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Start a DMA transfer starting from a segment for IpRx
// Pre: pCh->m_Dma.m_CurrentTransferLength: bytes to receive.
//
Int  Dta1xxCdmaStartSegment(
	IN Channel*  pCh,			// Channel pointer
	IN UInt SegmentIndex)		// Segment to start the DMA from
{
	NTSTATUS  Status = STATUS_SUCCESS;
	PDTA1XX_FDO  pFdo = pCh->m_pFdo;
	
	ASSERT((pCh->m_Dma.m_CurrentTransferLength !=0) &&
		   (pCh->m_Dma.m_CurrentTransferLength %4 == 0));

#if LOG_LEVEL_CDMA > 1
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaStartSegment: Transfer size=%u",
			    pCh->m_PortIndex, pCh->m_Dma.m_CurrentTransferLength );
#endif

	// Reset CDMA done flag
	clear_bit( 0, (void*)&pCh->m_CdmaDone);

	if (pFdo->m_UsesDmaInFpga) {
		UInt32 CmdStat;
		// Update the sgl descriptor for the given block
		UInt32 DescAddr;
		// Write pointer to first SGL in DMA-Channel-x-Descriptor-Pointer register
		DescAddr = pCh->m_CdmaSegmDesc[SegmentIndex].m_SglAddr;
	
		if (pCh->m_ChannelType == DTA1XX_IP_RX_CHANNEL) {
			DescAddr |= PCI905X_DMADPR_DIROFTFR;
		}
		WRITE_UINT32(pCh->m_Dma.m_pRegDmaDesc, 0, 
						DescAddr | PCI905X_DMADPR_DESCLOC_PCI);

		// Start transfer!
		CmdStat = PCI905X_DMACSR_ENABLE | PCI905X_DMACSR_START;

		if (pCh->m_ChannelType == DTA1XX_IP_RX_CHANNEL) {
			// Set the DMA size and DMA size enable bits in the command/status register
			// so if a timeout occurs, we can read back the number of bytes
			// not transfered.
			CmdStat|=  (pCh->m_Dma.m_CurrentTransferLength << 8) |
						DTA1XX_DMA_CMDSTAT_SIZE_EN;
		}  

		WRITE_UINT32(pCh->m_Dma.m_pRegCmdStat, 0, CmdStat);
	} else {
#if LOG_LEVEL_CDMA > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaStartSegment: NOT SUPPORTED",
				    pCh->m_PortIndex );
#endif
		return -EFAULT;
	}
	
	// Mark CDMA as running in device extension
	pCh->m_CdmaDmaRunning = TRUE;

	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCdmaStop -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Stop CDMA transfer
//
Int  Dta1xxCdmaStop(
	Channel* pCh)			// Channel pointer
{
	Int Status=0, Pci905xDmaCommandStat;
	volatile UInt8*  pPci905XRegs = pCh->m_pFdo->m_Pci905XConfRegs.m_pKernel;
	DTA1XX_FDO*  pFdo = pCh->m_pFdo;

#if LOG_LEVEL_CDMA > 0
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaStop: Abort CDMA transfer",
				pCh->m_PortIndex );
#endif

	// CDMA must be running
	if (!pCh->m_CdmaDmaRunning)
	{
#if LOG_LEVEL_CDMA > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaStop: FAIL: CDMA NOT RUNNING",
					pCh->m_PortIndex );
#endif
		return -EFAULT;
	}

	if (pFdo->m_UsesDmaInFpga)
	{
		// Disable and abort transfer
		WRITE_UINT32(pCh->m_Dma.m_pRegCmdStat, 0, PCI905X_DMACSR_ABORT);
		// Wait for the interrupt before the DMA is realy finished.
	}
	else
	{
		// Check which PLX DMA channel we should use and init address helpers
		if ( pCh->m_Dma.m_PlxChannel == 0 )
			Pci905xDmaCommandStat = PCI905X_DMA0_COMMAND_STAT;
		else /*if ( pCh->m_Dma.m_PlxChannel == 1 )*/
			Pci905xDmaCommandStat = PCI905X_DMA1_COMMAND_STAT;
		
		// Disable and abort transfer
		WRITE_UINT8(pPci905XRegs, Pci905xDmaCommandStat, PCI905X_DMACSR_ABORT);

		// Mark CDMA as no loner running
		pCh->m_CdmaDmaRunning = FALSE;

		// Wake-up any threads that might be waiting for CDMA done event
		set_bit(0, (void*)&pCh->m_CdmaDone);
		wake_up_interruptible( &pCh->m_CdmaEvent );
	}
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCdmaWaitSegment -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Wait for the segment interrupt
// pCh->m_CdmaSegment is set to the segment that is currently being read (=last completed
// segment + 1)
//
Int  Dta1xxCdmaWaitSegment(
	Channel* pCh,			// Channel pointer
	DTA1XX_CDMA_CONTROL* pCdmaCtrl)	// Output buffer
{
	Int Status=0, ret;

#if LOG_LEVEL_CDMA > 1
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaWaitSegment: Wait for segment interrupt",
			    pCh->m_PortIndex );
#endif

	// Wait for segment interrupt
	ret = wait_event_interruptible( pCh->m_CdmaEvent,
									(test_bit(0, (void*)&pCh->m_CdmaDone)) );

	// Reset CDMA done flag
	clear_bit( 0, (void*)&pCh->m_CdmaDone);

	// Were we interrupted by a signal?
	if ( ret!=0 /*-ERESTARTSYS*/ )
	{
		// Have to cancel the current CDMA
		Dta1xxCdmaStop(pCh);

		pCdmaCtrl->m_Segment = -1;		// For safety
		Status = -EFAULT;
	}
	else
	{
		// Get current segment# and associated reference-clock sample
		pCdmaCtrl->m_Segment = pCh->m_CdmaSegment;
		pCdmaCtrl->m_RefClkSamp = pCh->m_CdmaRefClkSamp;
	}
	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCdmaGetBufPos -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Get estimate of current read position in the CDMA buffer.
// Method: read reference-clock counter register, and extrapolate using the reference-
// clock count samples at the last segment interrupt
//
Int  Dta1xxCdmaGetBufPos(
	Channel*  pCh,			// Channel pointer
	DTA1XX_CDMA_CONTROL*  pCdmaCtrl)	// Output buffer
{
	Int64 Offset;
	UInt32 RefClkSamp;
	Int Status=0, RefClkDiff, SegmA, SegmB;

	// Get sample of the reference clock register
	RefClkSamp = Dta1xxGenGetRefClkCntReg(pCh->m_pFdo->m_pGenRegs);

	// Get CDMA-buffer segment from which we currently read, as well as the value of
	// the reference clock at the last segment interrupt
	// This code may be interrupted by the segment interrupt, so read segment twice
	// and loop until both values are equal
	do {
		SegmA = pCh->m_CdmaSegment;
		RefClkDiff = RefClkSamp - pCh->m_CdmaRefClkSamp;
		SegmB = pCh->m_CdmaSegment;		// Read again
	} while (SegmA != SegmB);

	// Safety check. Divider must be larger than 0
	if (pCh->m_CdmaClockDiv <= 0) {
#if LOG_LEVEL_CDMA > 1
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaGetBufPos: Invalid value (%d) for the CDMA"
				"clock divider", pCh->m_PortIndex, (Int)pCh->m_CdmaClockDiv );
#endif
		return -EFAULT;
	}

	// Compute position in CDMA buffer

    // 64-bit division are not allowed in the kernel => use Dta1xxBinDiv function
	Offset = (Int64)RefClkDiff * (Int64)(pCh->m_ModSampRate);
	Offset = Dta1xxBinDiv(Offset, pCh->m_CdmaClockDiv, NULL);

	pCdmaCtrl->m_BufPos =  pCh->m_CdmaSegmDesc[SegmA].m_CdmaRelAddr + (Int)(Offset);

	// Check whether we have wrapped past the end of the CDMA buffer
	if (pCdmaCtrl->m_BufPos >= pCh->m_CdmaBufSize)
		pCdmaCtrl->m_BufPos -= pCh->m_CdmaBufSize;

	// CDMA not running => set BufPos to 0
	if ( !pCh->m_CdmaDmaRunning )
		pCdmaCtrl->m_BufPos = 0;

#if LOG_LEVEL_CDMA > 2
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaGetBufPos RefClk=%u Segment=%d RefClkDiff=%u",
				pCh->m_PortIndex, (UInt)(RefClkSamp), SegmA, RefClkDiff);
#endif
#if LOG_LEVEL_CDMA > 1
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaGetBufPos: BufPos=%d, ModSampRate=%d",
                pCh->m_PortIndex, pCdmaCtrl->m_BufPos, pCh->m_ModSampRate );
#endif
	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCdmaSglCreate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Create Scatter-Gather list for CDMA buffer
// For Channeltype == DTA1XX_IP_TX_CHANNEL, we don't create segments
// For Channeltype == DTA1XX_IP_RX_CHANNEL, we create NUM_IPRX_DMA_QUEUES segments
// and each segment will have the end of chain mark and interrupt.
//
Int  Dta1xxCdmaSglCreate(
	Channel* pCh)			// Channel pointer
{
	Int i, Segment, AccuLength, Temp, SglSize=0;
	UInt32* pDesc=NULL;
	UInt32 PhysAddrSglFirst, PhysAddrSgl;
	ShBufDesc* pShBufDesc = &(pCh->m_CdmaShBuf);
	DTA1XX_FDO* pFdo = pCh->m_pFdo;
	struct scatterlist* pSgl=NULL;
	
#if LOG_LEVEL_CDMA > 0
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaSglCreate: Creating CDMA scatter-gather-list",
				pCh->m_PortIndex );
#endif
	// If we already have a scatter-gather buffer release it
	if ( pShBufDesc->m_SglAllocated != FALSE && pShBufDesc->m_pSglBuf != NULL )
	{
#if LOG_LEVEL_CDMA > 2
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaSglCreate: Freeing previous SGL-buffer",
					pCh->m_PortIndex );
#endif
		pci_free_consistent(pCh->m_pFdo->m_pPdo, pShBufDesc->m_SglBufSize,
							pShBufDesc->m_pSglBuf, pShBufDesc->m_PhysAddrSglBuf);

	}
	if ( pShBufDesc->m_SglAllocated != FALSE && pShBufDesc->m_pSglKernel != NULL )
	{
		kfree(pShBufDesc->m_pSglKernel);
	}

	// Init members
	pShBufDesc->m_pSglBuf = NULL;
	pShBufDesc->m_SglBufSize = 0;
	pShBufDesc->m_FirstDescOffset = 0;
	pShBufDesc->m_pSglKernel = NULL;
	pShBufDesc->m_NumSglEntries = 0;
	pShBufDesc->m_SglAllocated = FALSE;

	//.-.-.-.-.-.-.-.-.-.-.-.- Create kernel Scatter/Gather list -.-.-.-.-.-.-.-.-.-.-.-.-

	pShBufDesc->m_pSglKernel = kmalloc( sizeof(struct scatterlist)*pShBufDesc->m_NumPages, 
										GFP_KERNEL );
	if ( pShBufDesc->m_pSglKernel == NULL )
	{
#if LOG_LEVEL_CDMA > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaSglCreate: Failed to allocate kernel "
					"scatter-gather-list", pCh->m_PortIndex );
#endif
		return -EFAULT;
	}

	// Init scatter-gather table
	Dta1xxSgInitTable(pShBufDesc->m_pSglKernel, pShBufDesc->m_NumPages);

	// Init kernel scatter-gather-list
	for ( i=0; i<pShBufDesc->m_NumPages; i++ )
	{
		// Make sure the address = 0 for 2.4.x kernels
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,5,10)
		pShBufDesc->m_pSglKernel[i].address = 0; 
#endif
		// Last page may not be fully used
		if ( (i+1)==pShBufDesc->m_NumPages && (pCh->m_CdmaBufSize%PAGE_SIZE)!=0 )
			Dta1xxSgSetPage(&pShBufDesc->m_pSglKernel[i], pShBufDesc->m_PageList[i],
							pCh->m_CdmaBufSize % PAGE_SIZE, 0);
		else
			Dta1xxSgSetPage(&pShBufDesc->m_pSglKernel[i], pShBufDesc->m_PageList[i],
							PAGE_SIZE, 0);
	}

	// Map scatter-gather-list
	pShBufDesc->m_NumSglEntries = pci_map_sg(
		pFdo->m_pPdo,				// Physical device
		pShBufDesc->m_pSglKernel,	// Scatter/Gather list
		pShBufDesc->m_NumPages,		// # user pages to map
		(pCh->m_ChannelType == DTA1XX_IP_RX_CHANNEL ? 
				DMA_FROM_DEVICE : DMA_TO_DEVICE));	// DMA to device (read/write)

	if ( pShBufDesc->m_NumSglEntries <= 0 )
	{
#if LOG_LEVEL_CDMA > 0
		DTA1XX_LOG( KERN_INFO,
					"[%d] Dta1xxCdmaSglCreate: Failed to map scatter-gather-list",
					pCh->m_PortIndex );
#endif
		kfree(pShBufDesc->m_pSglKernel);
		pShBufDesc->m_pSglKernel = NULL;
		return -EFAULT;
	}

	//-.-.-.-.-.-.-.-.-.-.- Allocate physical scatter-gather buffer -.-.-.-.-.-.-.-.-.-.-.

	// Scatter-gather list must be alligned to 16-bytes => allocate 16 extra bytes
	SglSize = (pShBufDesc->m_NumSglEntries * (4*sizeof(UInt32))) + 16;

	pShBufDesc->m_pSglBuf = (UInt8*)pci_alloc_consistent(pCh->m_pFdo->m_pPdo, SglSize,
														 &pShBufDesc->m_PhysAddrSglBuf);

#if defined(CONFIG_X86_64)
	// For 64-bit linux also check whether allocated memory resides below 32-bit boundary
	if ( (pShBufDesc->m_pSglBuf == NULL) || ((pShBufDesc->m_PhysAddrSglBuf>>32) != 0) )
#else
	if ( pShBufDesc->m_pSglBuf == NULL )
#endif
	{

#if LOG_LEVEL_CDMA > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxCdmaSglCreate: Failed to allocate SGL buffer");
#endif

		pci_unmap_sg(pFdo->m_pPdo,				// Physical device
					 pShBufDesc->m_pSglKernel,	// Scatter/Gather list
					 pShBufDesc->m_NumPages,	// # user pages mapped
					 DMA_TO_DEVICE);			// DMA to device (write)
		kfree(pShBufDesc->m_pSglKernel);
		pShBufDesc->m_pSglKernel = NULL;
		return -EFAULT;
	}
	pShBufDesc->m_SglBufSize = SglSize;

	// Compute offset to for first descriptor (16-byte alligned)
	pShBufDesc->m_FirstDescOffset = ( ((IntPtr)(pShBufDesc->m_pSglBuf) + 0xF) & ~0xF)
										- (IntPtr)(pShBufDesc->m_pSglBuf);

	//.-.-.-.-.-.-.-.-.-.-.-.- Init DMA descriptor for all pages -.-.-.-.-.-.-.-.-.-.-.-.-

	Temp = pCh->m_CdmaBufSize;

	// Get first descriptor
	pDesc = (UInt32*)(pShBufDesc->m_pSglBuf + pShBufDesc->m_FirstDescOffset);
	PhysAddrSglFirst = pShBufDesc->m_PhysAddrSglBuf + pShBufDesc->m_FirstDescOffset;
	PhysAddrSgl = PhysAddrSglFirst;

	for ( i=0, pSgl=pShBufDesc->m_pSglKernel; i<pShBufDesc->m_NumSglEntries; i++, pSgl++ )
	{
		// Set PCI address
		pDesc[0]	= (UInt32)sg_dma_address(pSgl);
		// Set local address
		pDesc[1]	= (UInt32)(  pCh->m_pRegBase + DTA1XX_RX_REG_FIFO_FIRST
									- pCh->m_pFdo->m_Dta1xxRegsBase.m_pKernel );
		// Set transfer count
		pDesc[2]	= (UInt32)sg_dma_len(pSgl);

		Temp -= pDesc[2];

		// Set next desc pointer
		PhysAddrSgl	+= 4*sizeof(UInt32);
		// Check for last (make cyclic)
		if ( Temp <= 0 )
			pDesc[3]	= PhysAddrSglFirst | PCI905X_DMADPR_DESCLOC_PCI;
		else
			pDesc[3]	= PhysAddrSgl | PCI905X_DMADPR_DESCLOC_PCI;
		if (pCh->m_ChannelType == DTA1XX_IP_RX_CHANNEL) {
			// Set direction to read 
			pDesc[3] |= PCI905X_DMADPR_DIROFTFR;
		}

#if LOG_LEVEL_CDMA > 2
		DTA1XX_LOG( KERN_INFO, "SGL Descriptor [%d]: PCI=0x%08X, Local=0x%08X, "
					"Size=%d, Next=0x%08X", i,
					(UInt)(pDesc[0]), (UInt)(pDesc[1]),
					(UInt)(pDesc[2]), (UInt)(pDesc[3]) );
#endif
		// Move to next desc position
		pDesc+=4;

	}

#if LOG_LEVEL_CDMA > 1
	pDesc = (UInt32*)(pShBufDesc->m_pSglBuf + pShBufDesc->m_FirstDescOffset);
	DTA1XX_LOG( KERN_INFO, "First SGL Descriptor (0x%08X): PCI=0x%08X, Local=0x%08X, "
				"Size=%d, Next=0x%08X",
				(UInt)(PhysAddrSglFirst),
				(UInt)(pDesc[0]), (UInt)(pDesc[1]),
				(UInt)(pDesc[2]), (UInt)(pDesc[3]) );
#endif

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Init CDMA segments -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	// Get first descriptor
	pDesc = (UInt32*)(pShBufDesc->m_pSglBuf + pShBufDesc->m_FirstDescOffset);

	// CDMA buffer is divided into 4 segments 0..3
	// Start with CDMA segment# 0
	Segment = 0;
	pCh->m_CdmaSegmDesc[0].m_CdmaRelAddr = 0;
	pCh->m_CdmaSegmDesc[0].m_SglAddr = PhysAddrSglFirst;

	PhysAddrSgl = PhysAddrSglFirst;
	AccuLength = 0;

	for ( i=0; i<pShBufDesc->m_NumPages; i++ )
	{
		// Update accumulated size
		AccuLength += pDesc[2];
		if (pCh->m_ChannelType == DTA1XX_IP_RX_CHANNEL) {
			// Determine if we have passed a segment boundary
			// Segment boundaries are at (1/NUM_IPRX_DMA_QUEUES) and end of the CDMA buffer
			// For each segment boundary:
			//  - Set "Interrupt after Terminal Count" bit, to generate interrupts at
			//	  these time instances
			//  - Save Scatter-Gather-List address
			//  - Mark as last descriptor
			ASSERT((pCh->m_CdmaBufSize % NUM_IPRX_DMA_QUEUES) == 0);
			if ( AccuLength == ((Segment+1) * pCh->m_CdmaBufSize / NUM_IPRX_DMA_QUEUES) )
			{
				// Set interrupt flag
				pDesc[3] |= PCI905X_DMADPR_INTAFTERTC;
				// Mark as end of chain
				pDesc[3] |= PCI905X_DMADPR_ENDOFCHAIN;
#if LOG_LEVEL_CDMA > 1
					DTA1XX_LOG( KERN_INFO, "Set Segment [%d] end of chain:\n"
								" SGL Descriptor (0x%08X): PCI=0x%08X, Local=0x%08X, "
								"Size=%d, Next=0x%08X",
								Segment,
								(UInt)(PhysAddrSgl),
								(UInt)(pDesc[0]), (UInt)(pDesc[1]),
								(UInt)(pDesc[2]), (UInt)(pDesc[3]) );
	#endif
			}
			if ( AccuLength > ((Segment+1) * pCh->m_CdmaBufSize / NUM_IPRX_DMA_QUEUES) )
			{
				if (Segment < (NUM_IPRX_DMA_QUEUES - 1))
				{
					pCh->m_CdmaSegmDesc[Segment+1].m_CdmaRelAddr = AccuLength;
					pCh->m_CdmaSegmDesc[Segment+1].m_SglAddr = PhysAddrSgl;

	#if LOG_LEVEL_CDMA > 1
					DTA1XX_LOG( KERN_INFO, "Set Segment [%d] start:\n"
								" SGL Descriptor (0x%08X): PCI=0x%08X, Local=0x%08X, "
								"Size=%d, Next=0x%08X",
								Segment+1,
								(UInt)(PhysAddrSgl),
								(UInt)(pDesc[0]), (UInt)(pDesc[1]),
								(UInt)(pDesc[2]), (UInt)(pDesc[3]) );
	#endif
				}
				Segment++;
			}
			
		} else if (pCh->m_ChannelType != DTA1XX_IP_TX_CHANNEL) {
			// Determine if we have passed a segment boundary
			// Segment boundaries are at 1/4, 2/4, 3/4 and end of the CDMA buffer
			// For each segment boundary:
			//  - Set "Interrupt after Terminal Count" bit, to generate interrupts at
			//	  these time instances
			//  - Save Scatter-Gather-List address
			if ( AccuLength >= ((Segment+1) * pCh->m_CdmaBufSize / 4) )
			{
				// Set interrupt flag
				pDesc[3] |= PCI905X_DMADPR_INTAFTERTC;
				if (Segment < 3)
				{
					pCh->m_CdmaSegmDesc[Segment+1].m_CdmaRelAddr = AccuLength;
					pCh->m_CdmaSegmDesc[Segment+1].m_SglAddr = PhysAddrSgl;

	#if LOG_LEVEL_CDMA > 1
					DTA1XX_LOG( KERN_INFO, "Set Segment [%d] boundary:\n"
								" SGL Descriptor (0x%08X): PCI=0x%08X, Local=0x%08X, "
								"Size=%d, Next=0x%08X",
								Segment,
								(UInt)(PhysAddrSgl),
								(UInt)(pDesc[0]), (UInt)(pDesc[1]),
								(UInt)(pDesc[2]), (UInt)(pDesc[3]) );
	#endif
				}
				Segment++;
			}
		}

		// Move to next descriptor
		pDesc += 4;
		PhysAddrSgl += 4*sizeof(UInt32);
	}

	if (pFdo->m_UsesDmaInFpga)
		Dta1xxCdmaInit(pCh);
	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCdmaSglCleanUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Clean up Scatter-Gather List for CDMA buffer
//
Int  Dta1xxCdmaSglCleanUp(Channel* pCh)
{
	ShBufDesc* pShBufDesc = &(pCh->m_CdmaShBuf);
	DTA1XX_FDO* pFdo = pCh->m_pFdo;

#if LOG_LEVEL_CDMA > 0
    DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaSglCleanUp: Clean-up CDMA scatter-gather-list",
                pCh->m_PortIndex );
#endif

	// Do we have a SGL list to release?
	if ( pCh->m_CdmaShBuf.m_SglAllocated == FALSE )
	{
#if LOG_LEVEL_CDMA > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaSglCleanUp: No CDMA SGL-buffer allocated",
					pCh->m_PortIndex );
#endif
		return 0; // Nothing to free
    }

	// Sanity check: verify pointer before using it
	if ( pShBufDesc->m_pSglKernel != NULL )
	{
		// Unmap kernel scatter-gather-list
		pci_unmap_sg(pFdo->m_pPdo,				// Physical device
					 pShBufDesc->m_pSglKernel,	// Scatter/Gather list
					 pShBufDesc->m_NumPages,	// # user pages mapped
					 DMA_TO_DEVICE);			// DMA to device (write)
		kfree(pShBufDesc->m_pSglKernel);
		pShBufDesc->m_pSglKernel = NULL;
	}
	
	// Sanity check: verify pointer before using it
	if ( pShBufDesc->m_pSglBuf != NULL )
	{
		// Free buffer allocated for scatter-gather-descriptors
		pci_free_consistent(pCh->m_pFdo->m_pPdo, pShBufDesc->m_SglBufSize,
							pShBufDesc->m_pSglBuf, pShBufDesc->m_PhysAddrSglBuf);
		pShBufDesc->m_pSglBuf = NULL;
	}
	// Reset members
	pShBufDesc->m_SglAllocated = FALSE;
    pShBufDesc->m_SglBufSize = 0;
	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCdmaDpcDma -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This DPC is queued after a DMA is finished for IpTx.
//
#ifdef WINBUILD
VOID  Dta1xxCdmaDpcDma(
	IN PKDPC    Dpc,				// Supplies the DPC object
	IN PVOID    DeferredContext,	// Deferred Context = Channel pointer
									// Passed when KeInitializeDpc was called
	IN PVOID    SystemArgument1,	// Not used
	IN PVOID    SystemArgument2)	// Not used
#else
void  Dta1xxCdmaDpcDma(
	unsigned long  DeferredContext)	// Pointer to NwStore
#endif
{
	Channel*  pCh = (Channel*) DeferredContext;
	// For now, only 1 ethernet port for DTA-160/2160
	NwStore* pNwStore = &pCh->m_pFdo->m_NwStore[0];
	UInt8* pRead;
	LONG NumBytesToTransfer;
	ASSERT(MAX_IP_PORTS == 1);

#if LOG_LEVEL_CDMA > 1
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxCdmaDpcDma: Dma completed", pCh->m_PortIndex );
#endif

	// Update read pointer
	pRead = pNwStore->m_pIpTxRead;
	pRead+= pCh->m_Dma.m_CurrentTransferLength;
	if (pRead>= pCh->m_CdmaShBuf.m_pBuf+pNwStore->m_IpTxBufSize) {
		pRead = pRead - pNwStore->m_IpTxBufSize;
	}
	pNwStore->m_pIpTxRead = pRead;


	// Check if a new dma must be started
	NumBytesToTransfer = InterlockedExchangeAdd(&pNwStore->m_IpTxNumBytesToSend, 
												-1 * pCh->m_Dma.m_CurrentTransferLength);
	pCh->m_Dma.m_CurrentTransferLength = NumBytesToTransfer -1 * pCh->m_Dma.m_CurrentTransferLength;

	if (pCh->m_Dma.m_CurrentTransferLength && !pNwStore->m_IpTxStop) {
		// Still more bytes to send
		Dta1xxCdmaStartSize(pCh);
	} else {
		pCh->m_CdmaDmaRunning = FALSE;
	}
		
	// The event can be used for abort completion triggering...
	// Signal cdma event that dma is ready.
	// KeSetEvent(&(pCh->m_CdmaEvent), 0, FALSE);
}
