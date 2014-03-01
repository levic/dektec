//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Init.c *#*#*#*#*#*#*#*#*#* (C) 2000-2008 DekTec

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
#include <asm/bitops.h> // set_bit

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local Routines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int  Dta1xxCheckFirmwareVersion(DTA1XX_FDO*);
//Int  Dta1xxCreateSglDescBuf(DmaChannel* pDmaChannel);
Int  Dta1xxHandleUnitialisedEeprom(DTA1XX_FDO*);
void Dta1xxInitAsiSdi(Channel*);
Int  Dta1xxInitChannelsGeneric(DTA1XX_FDO*);
Int  Dta1xxInitFdo(DTA1XX_FDO*);
Int  Dta1xxInitDmaResources(Channel*);
Int  Dta1xxInitDeviceHardware(DTA1XX_FDO*);
Int  Dta1xxInitGenlock(DTA1XX_FDO*);
Int  Dta1xxDisableGenlock(DTA1XX_FDO*);
Int  Dta1xxInitLedFlashing(DTA1XX_FDO*);
Int  Dta1xxInitModulatorOffset(DTA1XX_FDO*);
Int  Dta1xxInitVpd(DTA1XX_FDO*);
NTSTATUS  Dta1xxInitWorkerThreads(PDTA1XX_FDO);
Int  Dta1xxWaitUntilFpgaIsReady(DTA1XX_FDO*);


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxInitDevice -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialise operation of Dta1xx device.
//
// PRE-CONDITION:
//	+ Device memory has been mapped into kernel space
//	+ PCI-9054/PCI-9056 has been initialised
//	+ pFdo->m_TypeNumber has been set
//
int  Dta1xxInitDevice(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	Int Status=0;

	// Generic register block starts at the same address for all DekTec boards
	// Pointer has to be initialised here, because Dta1xxHandleUnitialisedEeprom and
	// Dta1xxCheckFirmwareVersion need it
	pFdo->m_pGenRegs = pFdo->m_Dta1xxRegsBase.m_pKernel;

	// In some situations the OS will set the PLX chip from powermanagement state D3 to state D0.
	// This will initiate a software reset on the PLX chip and some boards
	// will initiate a FPGA reload.
	// If this is the case, the FPGA will not respond to any request until the FPGA is ready,
	// so we have to wait until the FPGA is up and running
	if (pFdo->m_UsesPlxChip)
		Dta1xxWaitUntilFpgaIsReady(pFdo);

	// Check Firmware Version
	if ( 0!= (Status=Dta1xxCheckFirmwareVersion(pFdo)) )
		return Status;

	// Generic initialisation of channel-related variables, this is:
	// No board-specific intialisations yet
	if ( 0 != (Status=Dta1xxInitChannelsGeneric(pFdo)) )
		return Status;

	// Initialisation of functional-device-object variables
	if ( 0 != (Status=Dta1xxInitFdo(pFdo)) )
		return Status;

	// Initialise device hardware
	if ( 0 != (Status=Dta1xxInitDeviceHardware(pFdo)) )
		return Status;

	// Initialise VPD caching, read and interpret VPD configuration resources
	if ( 0 != (Status=Dta1xxInitVpd(pFdo)) )
		return Status;

	// Initialse worker thread(s) for cards that use them
	if ( 0 != (Status=Dta1xxInitWorkerThreads(pFdo)) )
		return Status;

	// Initialise LED flashing
	if ( 0 != (Status=Dta1xxInitLedFlashing(pFdo)) )
		return Status;

	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxInitDevicePost -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialise operation of Dta1xx device after the interrupt is connected.
//
// PRE-CONDITION:
//	+ Interrupt is connected
//
Int Dta1xxInitDevicePost(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	Int  i;
	
	// Initialise Phy/Mac and sending of setup frame
	for (i=0; i<pFdo->m_NumIpPorts; i++) {
		Dta1xxNwResetPhyMacPowerup(&(pFdo->m_NwStore[i]));
	}

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxCheckFirmwareVersion -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Check whether the firmware version, as reported by the Altera itself, is compatible
// with this device driver.
//
Int  Dta1xxCheckFirmwareVersion(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	BOOLEAN  FirmwareCompatible;			// True if firmware compatible with driver

	// Mirror firmware version and firmware variant in device extension
	pFdo->m_FirmwareVersion = Dta1xxGenCtrlRegGetFirmwareRev(pFdo->m_pGenRegs);
	pFdo->m_FirmwareVariant = Dta1xxGenStatusRegGetFirmwareVariant(pFdo->m_pGenRegs);

	// Supported firmware versions:
	switch (pFdo->m_TypeNumber) {
	case 100:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 1); break;
	case 102:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 1); break;
	case 105:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 106:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 2); break;
	case 107:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 110:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 111:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 112:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 115:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 116:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 117:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 120:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 122:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 124:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 140:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 145:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 2111: FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 2135: FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 2136: FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 2137: FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 2142: FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 2144: FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 2145: FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 160:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 2160: FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	case 545:  FirmwareCompatible = (pFdo->m_FirmwareVersion >= 0); break;
	default:   FirmwareCompatible = FALSE;
	}

	// Firmware version not compatible => log message in event log and return
	if ( !FirmwareCompatible ) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxCheckFirmwareVersion: Firmware Version=%d not "
					"compatible with DTA-%d. DON'T START.",
					pFdo->m_FirmwareVersion, pFdo->m_TypeNumber );
#endif
		return -EFAULT;
	}
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxInitChannelsGeneric -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Generic initialisation of channels structures.
//
Int  Dta1xxInitChannelsGeneric(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	Int  i;
	volatile UInt8*  pBase = pFdo->m_Dta1xxRegsBase.m_pKernel;
	Channel*  pCh;

	// General initialisation of Channel data structure
	for (i=0; i<MAX_CHANNELS; i++)
	{
		// Channel pointer
		pCh = &pFdo->m_Channel[i];

		// Identity: index and pointer back to Device Extension
		pCh->m_PortIndex = i;
		pCh->m_pFdo = pFdo;

		// Channel type starts as UNUSED
		pCh->m_ChannelType = DTA1XX_CHANNEL_UNUSED;

		// Channel capability
		pCh->m_Capability = 0;

		// Reset config
		pCh->m_IoConfig = 0;
		pCh->m_IoConfigPar = -1;

		// Initialise exclusive-access variables
		init_MUTEX(&pCh->m_ExclAccLock);
		pCh->m_ExclAccess = 0;

		// Initialise register pointer for LEGACY boards
		pCh->m_pRegBase = pBase;

		// Set scatter/gather pointers to NULL, in case initialisation fails prematurely
		pCh->m_Dma.m_pSglDesc = NULL;

		// Default PLX DMA Controller is #0
		pCh->m_Dma.m_PlxChannel = 0;

		// Initialise Continuous DMA (CDMA)
		// No buffer allocated, no Scatter-Gather list created, DMA is not running
		pCh->m_CdmaBufSize = 0;
		pCh->m_CdmaShBuf.m_Allocated = FALSE;
		pCh->m_CdmaShBuf.m_pBuf = NULL;
		pCh->m_CdmaShBuf.m_pUserBuf = NULL;
		pCh->m_CdmaShBuf.m_NumPages = 0;
		pCh->m_CdmaShBuf.m_PageList = NULL;
		pCh->m_CdmaShBuf.m_SglAllocated = FALSE;
		pCh->m_CdmaShBuf.m_pSglBuf = NULL;
		pCh->m_CdmaShBuf.m_pSglKernel = NULL;
		pCh->m_CdmaDmaRunning = FALSE;
		
		// Init modulation parameter cache to defaults
		pCh->m_RfFreq = -1;           // unknown state
		pCh->m_OutpLevelGain = 0;     // does not support output level control
		pCh->m_OutpLeveldBm = -275;   // default output level: -27.5 dBm
		pCh->m_OutpLevelOffset = 0;   // register value 0 results in 0 dBm
		pCh->m_FreqCompTable = NULL;  // no table
		pCh->m_FreqCompTableSize = 0; // 0 entries
	}

	// User-IP-Rx/Tx channel data pointers are initialised in Dta1xxInitFdo

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxInitFdo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Board-specific initialisation of functional-device-object variables.
//
// Initializes:
//	- m_UsesDmaInFpga, m_UsesGenReg, m_Uses9054, m_Uses9056
//	- Rx/Tx, IpRx/IpTx and NwRx/NwTx channels including m_pRegBase
//	- DMA-Adapter objects and device queues
//  - Bit-rate measurement
//	- Target-adapter-detection variables
//	- DPC for Periodic Interrupt
//
Int  Dta1xxInitFdo(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	Int  i, Status;
	volatile UInt8*  pBase = pFdo->m_Dta1xxRegsBase.m_pKernel;
	UInt32  CntrlReg;
	Channel*  pCh;
	volatile UInt8*  pPci905XRegs = pFdo->m_Pci905XConfRegs.m_pKernel;

	// Set default value for some of the board-dependent variables
	// These values may be overriden in the "big switch" that follows
	pFdo->m_NumNonIpChannels = 0;
	pFdo->m_NumIpPorts = 0;
	pFdo->m_Dta110T = FALSE;
	pFdo->m_UsesDmaInFpga = FALSE;
	pFdo->m_UsesGenReg = TRUE;
	pFdo->m_Uses9054 = FALSE;
	pFdo->m_Uses9056 = FALSE;
	pFdo->m_UseFpgaForVpd = FALSE;
	pFdo->m_VpdEepromSize = PLX_VPD_SIZE;
	pFdo->m_SupportsI2cItf = FALSE;

	// Set board-dependent variables
	switch (pFdo->m_TypeNumber)
	{

	case 100:
	case 102:	pFdo->m_Uses9054 = TRUE;
				pFdo->m_UsesGenReg = FALSE;
				pFdo->m_NumNonIpChannels = 1;

				// Init channel regsister map location helpers
				pFdo->m_Channel[0].m_AddrBaseOffset = 0x00;
				pFdo->m_Channel[0].m_AddrRxRegMapOffset = 0x00;	// Not used
				pFdo->m_Channel[0].m_AddrTxRegMapOffset = 0x00;
				pFdo->m_Channel[0].m_AddrDmaRegMapOffset = 0x00;	// Not used

				pFdo->m_Channel[0].m_ChannelType = DTA1XX_TS_TX_CHANNEL;
				break;

	case 105:	pFdo->m_UsesDmaInFpga = TRUE;
				pFdo->m_NumNonIpChannels = 2;

				for (i=0; i<2; i++)
				{
					// Init channel register map location helpers
					pCh = &pFdo->m_Channel[i];
					pCh->m_AddrBaseOffset = 0x100 + i*0x200;
					pCh->m_AddrRxRegMapOffset = 0x000;	// Not used
					pCh->m_AddrTxRegMapOffset = 0x080;
					pCh->m_AddrDmaRegMapOffset = 0x100;

					// Init channel type and address pointer
					pCh->m_ChannelType = DTA1XX_TS_TX_CHANNEL;

					// Compute offset to channel operation registers
					pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
												   + pCh->m_AddrTxRegMapOffset);
					pCh->m_Capability =
								(i==0? 0 : DTA1XX_CHCAP_DBLBUF | DTA1XX_CHCAP_OUT_PORT);
					// Default config for port 2 is output
					if ( i==1 )
						pCh->m_IoConfig = DTA1XX_IOCONFIG_OUTPUT;
               }

				//pFdo->m_EepromSize = 128*1024;
				//pFdo->m_EepromSectorSize = 32*1024;
				//pFdo->m_UseFpgaForVpd = TRUE;
				pFdo->m_VpdEepromPageSize = 16;
				break;

	case 106:	pFdo->m_UsesDmaInFpga = TRUE;
				pFdo->m_EepromSize = 128*1024;
				pFdo->m_EepromSectorSize = 32*1024;
				pFdo->m_UseFpgaForVpd = TRUE;

				// Init channel regsister map location helpers
				pFdo->m_Channel[0].m_AddrBaseOffset = 0x00;
				pFdo->m_Channel[0].m_AddrRxRegMapOffset = 0x00;	// Not used
				pFdo->m_Channel[0].m_AddrTxRegMapOffset = 0x00;	// Not used
				pFdo->m_Channel[0].m_AddrDmaRegMapOffset = 0x00;	// Not used

				break;

	case 107:
	case 110:	pFdo->m_Uses9056 = TRUE;
				pFdo->m_NumNonIpChannels = 1;

				// Init channel regsister map location helpers
				pCh = &pFdo->m_Channel[0];
				pCh->m_AddrBaseOffset = 0x80;
				pCh->m_AddrRxRegMapOffset = 0x00;	// Not used
				pCh->m_AddrTxRegMapOffset = 0x00;
				pCh->m_AddrDmaRegMapOffset = 0x00;	// Not used

				pCh->m_ChannelType = DTA1XX_TS_TX_CHANNEL;
				pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
											   + pCh->m_AddrTxRegMapOffset);

				// Initialise shadow modulator-setting variables in device extension
				Dta1xxTxDeriveModPars(pFdo, 0);

				// Set clock divider for CDMA buffer position computations (25MHz,
				// 4 samples per clock)
				pCh->m_CdmaClockDiv = 25000000/4;
				break;

	case 111:	pFdo->m_Uses9056 = TRUE;
				pFdo->m_NumNonIpChannels = 1;

				// QAM/IQ modulation channel on port[0] (not 1 as on 112!)
				pCh = &pFdo->m_Channel[0];
				pCh->m_AddrBaseOffset = 0x200;
				pCh->m_AddrRxRegMapOffset = 0x000;	// Not used
				pCh->m_AddrTxRegMapOffset = 0x080;
				pCh->m_AddrDmaRegMapOffset = 0x100;	// Not used

				pCh->m_ChannelType = DTA1XX_TS_TX_CHANNEL;
				pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
										 + pCh->m_AddrTxRegMapOffset);
				// DTA-111 modulation channel still uses DMA controller 1!
				pCh->m_Dma.m_PlxChannel = 1;

				// Initialise shadow modulator-setting variables in device extension
				Dta1xxTxDeriveModPars(pFdo, 0);
				break;

	case 112:
	case 115:
	case 116:
	case 117:	pFdo->m_Uses9056 = TRUE;
				pFdo->m_NumNonIpChannels = 2;

				// Default Port 1 is ASI input
				pCh = &pFdo->m_Channel[0];
				pCh->m_AddrBaseOffset = 0x080;
				pCh->m_AddrRxRegMapOffset = 0x000;
				pCh->m_AddrTxRegMapOffset = 0x080;
				pCh->m_AddrDmaRegMapOffset = 0x000;	// Not used

				pCh->m_ChannelType = DTA1XX_TS_RX_CHANNEL;
				// Compute offset to rx register map
				pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
											   + pCh->m_AddrRxRegMapOffset);
				pCh->m_Capability = DTA1XX_CHCAP_BIDIR_PORT;
				// ASI channel uses controller 0
				pCh->m_Dma.m_PlxChannel = 0;

				// Port 2 is our QAM/OFDM modulation channel
				pCh = &pFdo->m_Channel[1];
				pCh->m_AddrBaseOffset = 0x200;
				pCh->m_AddrRxRegMapOffset = 0x000;	// Not used
				pCh->m_AddrTxRegMapOffset = 0x080;
				pCh->m_AddrDmaRegMapOffset = 0x100;	// Not used

				pCh->m_ChannelType = DTA1XX_TS_TX_CHANNEL;
				pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
											   + pCh->m_AddrTxRegMapOffset);
				// Modulation channel uses controller 1
				pCh->m_Dma.m_PlxChannel = 1;

				// Initialise shadow modulator-setting variables in device extension
				Dta1xxTxDeriveModPars(pFdo, 1);

				// Set clock divider for CDMA buffer position computations (54MHz,
				// 4 samples per clock)
				pCh->m_CdmaClockDiv = 54000000/4;
				break;

	case 120:
	case 122:	pFdo->m_Uses9054 = TRUE;
				pFdo->m_UsesGenReg = (pFdo->m_FirmwareVersion >= 4);
				pFdo->m_NumNonIpChannels = 1;

				pCh = &pFdo->m_Channel[0];
				pCh->m_ChannelType = DTA1XX_TS_RX_CHANNEL;

				// Firmware Revision >= 4: Rx registers located at base+0x20
				if (pFdo->m_FirmwareVersion >= 4)
					pCh->m_AddrBaseOffset = 0x020;
				else
					pCh->m_AddrBaseOffset = 0x000;
				pCh->m_AddrRxRegMapOffset = 0x000;

				pCh->m_AddrTxRegMapOffset = 0x000;		// Not used
				pCh->m_AddrDmaRegMapOffset = 0x000;		// Not used
				pCh->m_pRegBase =  (pBase + pCh->m_AddrBaseOffset
												+ pCh->m_AddrRxRegMapOffset);
				break;

	case 124:	pFdo->m_Uses9056 = TRUE;
				pFdo->m_UsesDmaInFpga = TRUE;
				pFdo->m_NumNonIpChannels = 4;

				for (i=0; i<4; i++)
				{
					pCh = &pFdo->m_Channel[i];
					if (pFdo->m_FirmwareVersion > 0) {
						pCh->m_AddrBaseOffset = 0x0;
						pCh->m_AddrRxRegMapOffset = (i==0?0x80:(i==1?0x100:(i==2?0x180:0x200)));
						pCh->m_AddrDmaRegMapOffset =(i==0?0x280:(i==1?0x2C0:(i==2?0x300:0x340)));
					} else {
						pCh->m_AddrBaseOffset = 0x080 + (0x80*i);
						pCh->m_AddrDmaRegMapOffset = 0x000;		// Not used
						pCh->m_AddrRxRegMapOffset = 0x000;
					}

					pCh->m_AddrTxRegMapOffset = 0x000;		// Not used


					pCh->m_ChannelType = DTA1XX_TS_RX_CHANNEL;
					// Compute channel address offset
					pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
												   + pCh->m_AddrRxRegMapOffset);
				}
				// Set General Purpose Output of PCI9056 to '1'
				CntrlReg  = READ_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
				CntrlReg |= PCI905X_CNTRL_GENPURPOUTP;
				WRITE_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, CntrlReg);
				break;

	case 140:	pFdo->m_Uses9056 = TRUE;
				// Two channels: Channel 0 is Rx, Channel 1 is Tx
				// PLX DMA Controller for channel 0 is 1, for channel 1 is 0!!
				pFdo->m_NumNonIpChannels = 2;

				pCh = &pFdo->m_Channel[0];
				pCh->m_AddrBaseOffset = 0x100;
				pCh->m_AddrRxRegMapOffset = 0x000;
				pCh->m_AddrTxRegMapOffset = 0x000;		// Not used
				pCh->m_AddrDmaRegMapOffset = 0x000;		// Not used
				pCh->m_ChannelType = DTA1XX_TS_RX_CHANNEL;
				pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
											   + pCh->m_AddrRxRegMapOffset);
                pCh->m_Dma.m_PlxChannel = 1;

				pCh = &pFdo->m_Channel[1];
				pCh->m_AddrBaseOffset = 0x080;
				pCh->m_AddrRxRegMapOffset = 0x000;		// Not used
				pCh->m_AddrTxRegMapOffset = 0x000;
				pCh->m_AddrDmaRegMapOffset = 0x000;		// Not used

				pCh->m_ChannelType = DTA1XX_TS_TX_CHANNEL;
				pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
											   + pCh->m_AddrTxRegMapOffset);
				pCh->m_Dma.m_PlxChannel = 0;
				break;

	case 2111:
				pFdo->m_UsesDmaInFpga = TRUE;
				pFdo->m_NumNonIpChannels = 1;
				pFdo->m_VpdEepromPageSize = 16;

				// Init I2C interface mutex
				pFdo->m_SupportsI2cItf = TRUE;
				init_MUTEX(&pFdo->m_I2cMutex);
				
				// Port 1: QAM/OFDM modulation channel
				pCh = &pFdo->m_Channel[0];
				pCh->m_AddrBaseOffset = 0x100;
				pCh->m_AddrRxRegMapOffset = 0x000;	// Not used
				pCh->m_AddrTxRegMapOffset = 0x080;
				pCh->m_AddrDmaRegMapOffset = 0x100;

				pCh->m_Capability = DTA1XX_CHCAP_AD9789 | DTA1XX_CHCAP_DACVGA;
				pCh->m_ChannelType = DTA1XX_TS_TX_CHANNEL;
				pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
										 + pCh->m_AddrTxRegMapOffset);

				// Initialise frequency-response compensation table and RF parameters
				Dta2111Init(pCh);

        		pCh->m_TsSymOrSampRate = -1;

				// Initialise shadow modulator-setting variables in device extension
				Dta1xxTxDeriveModPars(pFdo, 0);
				
				// Set clock divider for CDMA buffer position computations (54 MHz
				// 4 samples per clock)
				pCh->m_CdmaClockDiv = 54000000/4;
				break;

	case 2142:
				pFdo->m_UsesDmaInFpga = TRUE;
				pFdo->m_NumNonIpChannels = 2;
				pFdo->m_VpdEepromPageSize = 16;

				// Init I2C interface mutex
				pFdo->m_SupportsI2cItf = TRUE;
				init_MUTEX(&pFdo->m_I2cMutex);
				
				// Channel 0 and 1
				for (i=0; i<pFdo->m_NumNonIpChannels; i++) {
					pCh = &pFdo->m_Channel[i];
					pCh->m_AddrBaseOffset = 0x0100 + (i*0x200);
					pCh->m_AddrRxRegMapOffset = 0x000;
					pCh->m_AddrTxRegMapOffset = 0x080;
					pCh->m_AddrDmaRegMapOffset = 0x100;
					pCh->m_Capability = DTA1XX_CHCAP_BIDIR_PORT |
						DTA1XX_CHCAP_RATESEL;
					pCh->m_ChannelType = DTA1XX_TS_RX_CHANNEL;
					pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
											 + pCh->m_AddrRxRegMapOffset);
					if (i==0) {
					} else if (i==1) {
						pCh->m_pSpiReg = (struct _Dta1xxSpiGen *) (pBase + 0x080);
						ExInitializeFastMutex(&pCh->m_DssMutex);
						pCh->m_Capability |= DTA1XX_CHCAP_SPI | DTA1XX_CHCAP_CLKSEL;
					}
				}
				break;

	case 2144:
				pFdo->m_UsesDmaInFpga = TRUE;
				pFdo->m_NumNonIpChannels = 4;

				// Channel 0..3: Non-IP channels. Default all in
				for (i=0; i<4; i++) {
					pCh = &pFdo->m_Channel[i];

					pCh->m_AddrBaseOffset = 0x0100 + (i*0x200);
					pCh->m_AddrRxRegMapOffset = 0x000;
					pCh->m_AddrTxRegMapOffset = 0x080;
					pCh->m_AddrDmaRegMapOffset = 0x100;

					pCh->m_Capability = (DTA1XX_CHCAP_BIDIR_PORT 
										| DTA1XX_CHCAP_DBLBUF 
										| DTA1XX_CHCAP_GENLOCKED);
					if (i==0) {
						// Port 0 accepts GENLOCK reference input
						// Future: Allow all ports to be configured as
						//		Genlock input (configuration only issue)
						pCh->m_Capability |= DTA1XX_CHCAP_GENREF;
					}
					pCh->m_ChannelType = DTA1XX_TS_RX_CHANNEL;

					// Compute offset to channel register map
					pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
											 + pCh->m_AddrRxRegMapOffset);
				}

				//pFdo->m_EepromSize = 128*1024;
				//pFdo->m_EepromSectorSize = 32*1024;
				//pFdo->m_UseFpgaForVpd = TRUE;
				pFdo->m_VpdEepromPageSize = 16;

				// Init I2C interface mutex
				pFdo->m_SupportsI2cItf = TRUE;
				init_MUTEX(&pFdo->m_I2cMutex);
				break;

	case 145:
	case 2145:
				pFdo->m_UsesDmaInFpga = TRUE;
				pFdo->m_NumNonIpChannels = 2;

				pCh = &pFdo->m_Channel[0];
				pCh->m_AddrBaseOffset = 0x0100;
				pCh->m_AddrRxRegMapOffset = 0x000;
				pCh->m_AddrTxRegMapOffset = 0x080;
				pCh->m_AddrDmaRegMapOffset = 0x100;

				pCh->m_ChannelType = DTA1XX_TS_RX_CHANNEL;
				pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
											   + pCh->m_AddrRxRegMapOffset);
				pCh->m_Capability = DTA1XX_CHCAP_BIDIR_PORT;

				// DTA-145/DTA-2145 firmware version with genlock support
				if (pFdo->m_FirmwareVersion >= 3) {
					// channel 0 can operate only as GENLOCK reference
					// (not as GENLOCKED OUTPUT)
					pCh->m_Capability |= DTA1XX_CHCAP_GENREF;								
				}


				pCh = &pFdo->m_Channel[1];
				pCh->m_AddrBaseOffset = 0x0300;
				pCh->m_AddrRxRegMapOffset = 0x000;	// Not used
				pCh->m_AddrTxRegMapOffset = 0x080;
				pCh->m_AddrDmaRegMapOffset = 0x100;

				pCh->m_ChannelType = DTA1XX_TS_TX_CHANNEL;
				pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
											   + pCh->m_AddrTxRegMapOffset);
				pCh->m_Capability = DTA1XX_CHCAP_FAILSAFE | DTA1XX_CHCAP_OUT_PORT;
				if ( pFdo->m_FirmwareVersion >= 2 )
					pCh->m_Capability |= (DTA1XX_CHCAP_DBLBUF | DTA1XX_CHCAP_LOOPTHR);

				// DTA-145/DTA-2145 firmware version with genlock support
				if (pFdo->m_FirmwareVersion >= 3) {
					// channel 1 can operate only as GENLOCKED Output
					// (not as GENLOCK REFERENCE INPUT)
					pCh->m_Capability |= DTA1XX_CHCAP_GENLOCKED;							
				}

				pCh->m_IoConfig = DTA1XX_IOCONFIG_OUTPUT;

				//pFdo->m_EepromSize = 128*1024;
				//pFdo->m_EepromSectorSize = 32*1024;
				//pFdo->m_UseFpgaForVpd = TRUE;
				pFdo->m_VpdEepromPageSize = 16;
				break;

	case 160:	pFdo->m_Uses9056 = TRUE;	// Only for DTA-160, not for DTA-2160
	case 2160:	
				pFdo->m_UsesDmaInFpga = TRUE;

				// DTA-160/2160 Channels:
				//	0..2: ASI/SDI Input or Output
				//  3   : IP Rx Main Queue
				//  4   : IP Tx Main Queue
				//  5   : NDIS Tx

				pFdo->m_NumNonIpChannels = 3;
				pFdo->m_NumIpPorts = 1;

				// Channel 0..2: Non-IP channels. Default 1 in, 2 out
				for (i=0; i<3; i++) {
					pCh = &pFdo->m_Channel[i];

					pCh->m_AddrBaseOffset = 0x0100 + (i*0x200);
					pCh->m_AddrRxRegMapOffset = 0x000;
					pCh->m_AddrTxRegMapOffset = 0x080;
					pCh->m_AddrDmaRegMapOffset = 0x100;

					pCh->m_Capability = DTA1XX_CHCAP_BIDIR_PORT;
					if ((pFdo->m_TypeNumber==2160) || (pFdo->m_FirmwareVersion>=2))
						pCh->m_Capability |= (DTA1XX_CHCAP_DBLBUF | DTA1XX_CHCAP_LOOPTHR);
					
					pCh->m_ChannelType = (i == 0 ? DTA1XX_TS_RX_CHANNEL : DTA1XX_TS_TX_CHANNEL);

					// Compute offset to channel register map
					if (i==0) {
						pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
													   + pCh->m_AddrRxRegMapOffset);
					} else {
						pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
													   + pCh->m_AddrTxRegMapOffset);
					}
				}

				// Channel 3: IP-Main-Queue Receive channel
				pCh = &pFdo->m_Channel[3];
				pCh->m_AddrBaseOffset = 0xB00;
				pCh->m_AddrRxRegMapOffset = 0x000;	// Not used
				pCh->m_AddrTxRegMapOffset = 0x000;	// Not used
				pCh->m_AddrDmaRegMapOffset = 0x000;	// Not used

				pCh->m_ChannelType = DTA1XX_IP_RX_CHANNEL;
				pCh->m_pRegBase = pBase + pCh->m_AddrBaseOffset;
				pFdo->m_IpMqRx[0] = pCh;

				// Channel 4: IP-Main-Queue Transmit channel
				pCh = &pFdo->m_Channel[4];
				pCh->m_AddrBaseOffset = 0xC00;
				pCh->m_AddrRxRegMapOffset = 0x000;	// Not used
				pCh->m_AddrTxRegMapOffset = 0x000;	// Not used
				pCh->m_AddrDmaRegMapOffset = 0x000;	// Not used

				pCh->m_ChannelType = DTA1XX_IP_TX_CHANNEL;
				pCh->m_pRegBase = pBase + pCh->m_AddrBaseOffset;
				pFdo->m_IpMqTx[0] = pCh;

				// NDIS Receive channel.
				// The NDIS Receive channel is not a separate DMA channel
				// anymore. The internal queue is stored in the m_NwStore.

				// Channel 5: NDIS Transmit channel
				pCh = &pFdo->m_Channel[5];
				pCh->m_AddrBaseOffset = 0xD00;
				pCh->m_AddrRxRegMapOffset = 0x000;	// Not used
				pCh->m_AddrTxRegMapOffset = 0x000;	// Not used
				pCh->m_AddrDmaRegMapOffset = 0x000;	// Not used

				pCh->m_ChannelType = DTA1XX_NW_TX_CHANNEL;
				pCh->m_pRegBase = pBase + pCh->m_AddrBaseOffset;
				pFdo->m_NwTx[0] = pCh;

				// Initialise NwStore
				pFdo->m_NwStore[0].m_IpPortIndex = 0;
				pFdo->m_NwStore[0].m_pFdo = pFdo;
				pFdo->m_NwStore[0].m_pRegBase = (UInt8*)pBase+0x900;
				pFdo->m_NwStore[0].m_NumRxFifoOverflows = 0;
				pFdo->m_NwStore[0].m_AdminStatus = DTA1XX_NW_ADMINSTATUS_UP;
				pFdo->m_NwStore[0].m_EnableNwiRx = FALSE;
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "Dta1xxInitFdo: [NwRx0] Dta1xxInitFdo: NWI DISABLED" );
#endif
				if (pFdo->m_Uses9056) {
					// Set General Purpose Output of PCI9056 to '1'
					// Local bus reset
					CntrlReg  = READ_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT);
					CntrlReg |= PCI905X_CNTRL_GENPURPOUTP;
					WRITE_UINT32(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, CntrlReg);
				} else {	// DTA-2160, VPD without PLX
					// Initialise VpdPageSize
					pFdo->m_VpdEepromPageSize = 16;
				}
				break;

	case 545:	pFdo->m_UsesDmaInFpga = TRUE;
				// Two channels: Channel 0 is Rx, Channel 1 is Tx
				pFdo->m_NumNonIpChannels = 2;

				// Init space location helpers
				pCh = &pFdo->m_Channel[0];
				pCh->m_AddrBaseOffset = 0x100;
				pCh->m_AddrRxRegMapOffset = 0x000;
				pCh->m_AddrTxRegMapOffset = 0x000;	// Not used
				pCh->m_AddrDmaRegMapOffset = 0x100;

				pCh->m_ChannelType = DTA1XX_TS_RX_CHANNEL;
				pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
											   + pCh->m_AddrRxRegMapOffset);

				pCh = &pFdo->m_Channel[1];
				pCh->m_AddrBaseOffset = 0x300;
				pCh->m_AddrRxRegMapOffset = 0x000;	// Not used
				pCh->m_AddrTxRegMapOffset = 0x080;
				pCh->m_AddrDmaRegMapOffset = 0x100;

				pCh->m_ChannelType = DTA1XX_TS_TX_CHANNEL;
				pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
											   + pCh->m_AddrTxRegMapOffset);

				pFdo->m_VpdEepromPageSize = 16;
				break;

	case 2135:	pFdo->m_UsesDmaInFpga = TRUE;

				// Card has two DVB-T rx-channels + an ADC channel	
				pFdo->m_NumNonIpChannels = 3;
				for (i=0; i<pFdo->m_NumNonIpChannels; i++)
				{
					pCh = &pFdo->m_Channel[i];
					pCh->m_AddrBaseOffset = 0x0100 + (i*0x200);
					pCh->m_AddrRxRegMapOffset = 0x000;
					pCh->m_AddrTxRegMapOffset = 0x080;		// not used
					pCh->m_AddrDmaRegMapOffset = 0x100;

                    pCh->m_ChannelType = DTA1XX_TS_RX_CHANNEL;
					pCh->m_Capability = DTA1XX_CHCAP_IN_PORT;

					// Port 1 is diversity capable
					if (i==0) 
						pCh->m_Capability |= DTA1XX_CHCAP_DIVERSITY;
					// Port 2 can share the antenna input from port 1
					else if (i==1) 
						pCh->m_Capability |= DTA1XX_CHCAP_SHARED;

					// Default IO-config is input
					pCh->m_IoConfig = DTA1XX_IOCONFIG_INPUT;

					pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
											       + pCh->m_AddrRxRegMapOffset);
				}
				//pFdo->m_EepromSize = 128*1024;
				//pFdo->m_EepromSectorSize = 32*1024;
				//pFdo->m_UseFpgaForVpd = TRUE;
				pFdo->m_VpdEepromPageSize = 16;

				// Init I2C interface mutex
				pFdo->m_SupportsI2cItf = TRUE;
				init_MUTEX(&pFdo->m_I2cMutex);
				break;

	case 2136:	pFdo->m_UsesDmaInFpga = TRUE;

				// Card has two DVB-C rx-channels and two ASI outputs
				pFdo->m_NumNonIpChannels = 4;
				for (i=0; i<pFdo->m_NumNonIpChannels; i++)
				{
					
					pCh = &pFdo->m_Channel[i];
					pCh->m_AddrBaseOffset = 0x0100 + (i*0x100);
					pCh->m_AddrRxRegMapOffset = 0x000;
					pCh->m_AddrTxRegMapOffset = 0x000;
					pCh->m_AddrDmaRegMapOffset = 0x080;
					
					if (i<2) {
						pCh->m_ChannelType	= DTA1XX_TS_RX_CHANNEL;
						pCh->m_Capability	= DTA1XX_CHCAP_IN_PORT;

						// Port 2 can share the antenna input from port 1
						if (i==1) pCh->m_Capability |= DTA1XX_CHCAP_SHARED;						
						
						// Default IO-config is input
						pCh->m_IoConfig		= DTA1XX_IOCONFIG_INPUT;

						pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
												 + pCh->m_AddrRxRegMapOffset);
					} else {
						pCh->m_ChannelType	= DTA1XX_TS_TX_CHANNEL;
						pCh->m_Capability	= DTA1XX_CHCAP_OUT_PORT;
						pCh->m_Capability   |= (DTA1XX_CHCAP_DBLBUF | DTA1XX_CHCAP_LOOPTHR);

						// Default IO-config must be output
						pCh->m_IoConfig		= DTA1XX_IOCONFIG_OUTPUT;

						pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
												 + pCh->m_AddrTxRegMapOffset);
					}					

					
				}
				//pFdo->m_EepromSize = 128*1024;
				//pFdo->m_EepromSectorSize = 32*1024;
				//pFdo->m_UseFpgaForVpd = TRUE;
				pFdo->m_VpdEepromPageSize = 16;

				// Init I2C interface mutex
				pFdo->m_SupportsI2cItf = TRUE;
				init_MUTEX(&pFdo->m_I2cMutex);
				break;

	case 2137:	pFdo->m_UsesDmaInFpga = TRUE;

				// Card has two DVB-S2 rx-channels and two ASI outputs
				pFdo->m_NumNonIpChannels = 4;
				for (i=0; i<pFdo->m_NumNonIpChannels; i++)
				{
					pCh = &pFdo->m_Channel[i];
					pCh->m_AddrBaseOffset = 0x0100 + (i*0x100);
					pCh->m_AddrRxRegMapOffset = 0x000;
					pCh->m_AddrTxRegMapOffset = 0x000;		// not used
					pCh->m_AddrDmaRegMapOffset = 0x080;

                    if (i<2) {
						pCh->m_ChannelType	= DTA1XX_TS_RX_CHANNEL;
						pCh->m_Capability	= DTA1XX_CHCAP_IN_PORT;
						pCh->m_Capability  |= DTA1XX_CHCAP_APSK;

						// Port 2 can share the antenna input from port 1
						if (i==1) pCh->m_Capability |= DTA1XX_CHCAP_SHARED;						
						
						// Default IO-config is input
						pCh->m_IoConfig		= DTA1XX_IOCONFIG_INPUT;

						pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
												 + pCh->m_AddrRxRegMapOffset);
					}
					else {
						pCh->m_ChannelType	= DTA1XX_TS_TX_CHANNEL;
						pCh->m_Capability	= DTA1XX_CHCAP_OUT_PORT;
						pCh->m_Capability   |= (DTA1XX_CHCAP_DBLBUF | DTA1XX_CHCAP_LOOPTHR);

						// Default IO-config is loopthrough copy
						pCh->m_IoConfig		= DTA1XX_IOCONFIG_LOOPTHR;
						pCh->m_IoConfigPar	= i-2;	// Copy of port 1 or 2

						pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
												+ pCh->m_AddrTxRegMapOffset);
					}
				}
				//pFdo->m_EepromSize = 128*1024;
				//pFdo->m_EepromSectorSize = 32*1024;
				//pFdo->m_UseFpgaForVpd = TRUE;
				pFdo->m_VpdEepromPageSize = 16;

				// Init I2C interface mutex
				pFdo->m_SupportsI2cItf = TRUE;
				init_MUTEX(&pFdo->m_I2cMutex);
				break;
	}

	if (pFdo->m_UseFpgaForVpd)
	{
		// Compute address of last sector in EEPROM
		pFdo->m_EepromAddrLastSector = pFdo->m_EepromSize - pFdo->m_EepromSectorSize;

		// Relative address of VPD data in m_pEepromCache (EEPROM cache)
		pFdo->m_EepromRelAddrVpd = pFdo->m_EepromSectorSize - pFdo->m_VpdEepromSize;
	}

	// Compute total number of channels
	pFdo->m_NumChannels = 3*pFdo->m_NumIpPorts +	//IpTx,NdisTx,Ip/NdisRx
							 pFdo->m_NumNonIpChannels;


	// Some debug output to check the results of the big switch
#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "Dta1xxInitFdo: Uses PLX chip=%d", pFdo->m_UsesPlxChip);
	DTA1XX_LOG( KERN_INFO, "Dta1xxInitFdo: Uses PCI9054=%d", pFdo->m_Uses9054);
	DTA1XX_LOG( KERN_INFO, "Dta1xxInitFdo: Uses PCI9056=%d", pFdo->m_Uses9056);
	DTA1XX_LOG( KERN_INFO, "Dta1xxInitFdo: Uses DMA Controller in FPGA=%d",
				pFdo->m_UsesDmaInFpga );
	DTA1XX_LOG( KERN_INFO, "Dta1xxInitFdo: General-Register block=%d", pFdo->m_UsesGenReg);
	DTA1XX_LOG( KERN_INFO, "Dta1xxInitFdo: #Non-IP=%d #Ip=%d #IpRx=Unlimited #IpTx=Unlimited",
				pFdo->m_NumNonIpChannels, pFdo->m_NumIpPorts );
#endif

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Initialise DMA resources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	for (i=0; i<pFdo->m_NumChannels; i++)
	{
		pCh = &pFdo->m_Channel[i];
		if ((Status=Dta1xxInitDmaResources(pCh)) != 0)
			return Status;
	}


	//-.-.-.-.-.-.-.-.-.-.-.-.-.- Initialise Receive Channels -.-.-.-.-.-.-.-.-.-.-.-.-.-.

	// Nothing specific for receive channels


	//-.-.-.-.-.-.-.-.-.-.-.-.-.- Initialise Transmit Channels -.-.-.-.-.-.-.-.-.-.-.-.-.-

	for (i=0; i<pFdo->m_NumNonIpChannels; i++) {
		Int  Dummy;
		if (pFdo->m_Channel[i].m_ChannelType != DTA1XX_TS_TX_CHANNEL)
			continue;

		// Initialise synchronisation variables
		init_MUTEX(&pFdo->m_Channel[i].m_MutexTxControl);

		// Initialise Transport-Stream rate shadow variable
		Dta1xxTxGetRate2(pFdo, i, &Dummy,
						 &pFdo->m_Channel[i].m_TsSymOrSampRate);
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxInitFdo: TsSymOrSampRate=%d",
					i, pFdo->m_Channel[i].m_TsSymOrSampRate );
#endif
	}


	//.-.-.-.-.-.-.-.-.-.- Initialise User-IP-Rx/Tx Channel Pointers -.-.-.-.-.-.-.-.-.-.-
	
	// Reset IpRx linked list pointer
	pFdo->m_pIpRx = NULL;
	
	// Reset IpTx linked list pointer
	pFdo->m_pIpTx = NULL;

	if (pFdo->m_NumIpPorts != 0) {
		// List-protection mutex IpRx thread
		ExInitializeFastMutex(&pFdo->m_IpRxFastMutex2);

		// List-protection mutex IpRx for search in linked list
		ExInitializeFastMutex(&pFdo->m_IpRxFastMutex);

		// List-protection spinlock IpRx for bitrate measurement
		KeInitializeSpinLock(&pFdo->m_IpRxBrmSpinlock);

		// DPC for calculating IpRx bitrate
		InitializeDpc(&pFdo->m_DpcBrmIpRx, Dta1xxNwIpRxBrmPerIntDpc, 
						pFdo);
		pFdo->m_IpRxBrmSkip = FALSE;

		// List-protection spinlock IpTx thread
		KeInitializeSpinLock(&pFdo->m_IpTxSpinLock);		
		
		// List-protection mutex IpTx for search in linked list
		ExInitializeFastMutex(&pFdo->m_IpTxFastMutex);
	}

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Initialise NwIpPorts -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	if (pFdo->m_NumIpPorts != 0) {
		for (i=0; i<pFdo->m_NumIpPorts; i++) {
			//InitializeSpinLock(&(pFdo->m_NwStore[i].m_StatusSpinLock));
			//pFdo->m_NwStore[i].m_StatusIrp = NULL;
			//InitializeInternalQueue(&pFdo->m_NwStore[i].m_NwRxQueue, FALSE);
			InitializeDpc(&pFdo->m_NwStore[i].m_IpTxDPC, Dta1xxNwIpTxWorkerDpc, 
						  &pFdo->m_NwStore[i]);
			
			// Create Cdma buffer for IpTx
			if (!NT_SUCCESS(Dta1xxNwIpTxCreateCdmaBuffer(&pFdo->m_NwStore[i]))) {
				return STATUS_INSUFFICIENT_RESOURCES;
			}
		}
	}
	//-.-.-.-.-.-.-.-.-.-.-.-.- Initialise Bit-Rate Measurement -.-.-.-.-.-.-.-.-.-.-.-.-.

	for (i=0; i<pFdo->m_NumChannels; i++) {
		pFdo->m_Channel[i].m_BrMeas.m_NumValidSamps = 0;	// No valid samples yet
		pFdo->m_Channel[i].m_BrMeas.m_ValidCount256 = 0;	// Initial bit rate is 0
	}


	//-.-.-.-.-.-.-.-.-.-.-.- Initialise Target Adapter Detection -.-.-.-.-.-.-.-.-.-.-.-.

	if (pFdo->m_TypeNumber==102 || pFdo->m_TypeNumber==122) {
		pFdo->m_Channel[0].m_TrgIdDet.m_TdState = DTA1XX_TD_INITIAL;
		pFdo->m_Channel[0].m_TrgIdDet.m_TargetError = FALSE;
	}

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxInitLedFlashing -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialise LED flashing.
//
Int  Dta1xxInitLedFlashing(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	// Common initialisation
	pFdo->m_LedFlashFlashCounter = 0;
	pFdo->m_LedFlashStateCounter = 0;
	pFdo->m_LedFlashState = DTA1XX_LEDFLASH_INIT;

	// Start LED flashing for DTA-102 (Firmware Version>=5), DTA-107, DTA-110, DTA-112,
	// DTA-115, DTA-116, DTA-117, DTA-120, DTA-122, DTA-124, DTA-140, DTA-160 and DTA-2160.
	// Parameters depend on clock frequency
	if (   (pFdo->m_TypeNumber==102 && pFdo->m_FirmwareVersion>=5)
		||  pFdo->m_TypeNumber==107
		||  pFdo->m_TypeNumber==110
		||  pFdo->m_TypeNumber==111
		||  pFdo->m_TypeNumber==112
		||  pFdo->m_TypeNumber==115
		||  pFdo->m_TypeNumber==116
		||  pFdo->m_TypeNumber==117
		||  pFdo->m_TypeNumber==122
		||  pFdo->m_TypeNumber==124
		||  pFdo->m_TypeNumber==160
		||  pFdo->m_TypeNumber==2160) {			// 27-MHz boards

		pFdo->m_LedFlashDurationInit = 7;
		pFdo->m_LedFlashDurationOn = 1;
		pFdo->m_LedFlashDurationOff = 1;
		pFdo->m_LedFlashDurationExit = 7;
		pFdo->m_LedFlashNumFlashes = 7;
		pFdo->m_LedFlashStartUp = TRUE;
		Dta1xxGenSetLed(pFdo, 0);
	}
	else if (pFdo->m_TypeNumber==120 || pFdo->m_TypeNumber==140) {

		pFdo->m_LedFlashDurationInit = 10;
		pFdo->m_LedFlashDurationOn = 1;
		pFdo->m_LedFlashDurationOff = 2;
		pFdo->m_LedFlashDurationExit = 10;
		pFdo->m_LedFlashNumFlashes = 7;
		pFdo->m_LedFlashStartUp = TRUE;
		Dta1xxGenSetLed(pFdo, 0);
	} else {
		// Boards without generic LED: do not start LED flashing
		pFdo->m_LedFlashStartUp = FALSE;
	}
	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxInitDeviceHardware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialise PCI card. Called from Dta1xxInitDevice.
//
Int  Dta1xxInitDeviceHardware(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	Int  i;
	Channel*  pCh;
	Int  Status;
	UInt8 CurrLatTimer=0, NewLatTimer=0, MaxLat=0, MinGnt=0;
	UInt32 PlxDmaThreshold;
	volatile UInt8*  pPci905XRegs = pFdo->m_Pci905XConfRegs.m_pKernel;

	// Get latency timer, max latency and minimum grant

	// Read latency timer
	Status = Dta1xxReadConfigSpace(pFdo, PCI905X_LATENCY_TIMER, 1, &CurrLatTimer);
	// Read minimum grant
	Status = Dta1xxReadConfigSpace(pFdo, PCI905X_MIN_GNT, 1, &MinGnt);
	// Read maximum latency
	Status = Dta1xxReadConfigSpace(pFdo, PCI905X_MAX_LAT, 1, &MaxLat);

#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "Dta1xxInitDeviceHardware: Current Latency Timer=%d, "
				"Min Grant=%d, Max Latency=%d", CurrLatTimer, MinGnt, MaxLat );
#endif

	// By default: New Latency Timer value is set to 64 PCI clocks
	NewLatTimer = 64;

	// By default: PLX DMA threshold should be setup as follows
	// These defaults are based on the assumption that the local bus is slower then
	// the PCI bus (i.e. PCI bus read/writes quicker to PLX DMA FIFO's then local bus)
	// C0PLAF=0x0 ( 4 LWords), C0PLAE=0x1 ( 8 LWords)
	// C0LPAF=0x9 (40 LWords), C0LPAE=0x9 (40 LWords)
	// C1PLAF=0x0 ( 4 LWords), C1PLAE=0x1 ( 8 LWords)
	// C1LPAF=0x9 (40 LWords), C1LPAE=0x9 (40 LWords)
	PlxDmaThreshold = 0x99109910;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xx Registers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	// Clear PE (Program Enable) and PRE (Protect Register Enable) bits
	Dta1xxGenCtrlRegSetPE(pFdo->m_pGenRegs, 0);
	Dta1xxGenCtrlRegSetPRE(pFdo->m_pGenRegs, 0);

	// Initialse PCI-card-specific registers

	switch (pFdo->m_TypeNumber) {

	case 100:
		pCh = &pFdo->m_Channel[0];

		Dta1xxTxCtrlRegSetTxMode(pCh->m_pRegBase, 0);		// Transmit Mode := 188
		Dta1xxTxCtrlRegSetTxBurstMode(pCh->m_pRegBase, 0);	// Burst Mode Off
		Dta1xxTxCtrlRegSetTxPckStuff(pCh->m_pRegBase, 0);	// Null-Packet Stuffing Off
		Dta1xxTxCtrlRegSetTxCtrl(pCh->m_pRegBase, 0);		// Transmit Control := Idle

		Dta1xxTxCtrlRegSetTxPerIntEn(pCh->m_pRegBase, 0);	// Periodic-Interrupt Enable := 0
															// (enabled later)
		Dta1xxTxCtrlRegSetTxUflIntEn(pCh->m_pRegBase, 0);	// Clear interrupt enable bits
		Dta1xxTxCtrlRegSetTxSyncIntEn(pCh->m_pRegBase, 0);	//   ,,     ,,       ,,    ,,

		Dta1xxTxSetTxClock(pCh->m_pRegBase, 0);				// Clock to 0 bps
		Dta1xxTxSetFifoSizeReg(pCh->m_pRegBase, 8*1024*1024);	// Maximum FIFO size

		Dta1xxTxDiagRegSetLoopBack(pCh->m_pRegBase, 0);		// No loopback
		break;

	case 102:
		pCh = &pFdo->m_Channel[0];

		Dta1xxTxCtrlRegSetTxMode(pCh->m_pRegBase, 0);		// Transmit Mode := 188
		Dta1xxTxCtrlRegSetTxBurstMode(pCh->m_pRegBase, 0);	// Burst Mode Off
		Dta1xxTxCtrlRegSetTxPckStuff(pCh->m_pRegBase, 0);	// Null-Packet Stuffing Off
		Dta1xxTxCtrlRegSetTxCtrl(pCh->m_pRegBase, 0);		// Transmit Control := Idle
		Dta1xxTxCtrlRegSetTxDis(pCh->m_pRegBase, 0);		// Disable LVDS outputs

		Dta1xxTxCtrlRegSetTxPerIntEn(pCh->m_pRegBase, 0);	// Periodic-Interrupt Enable := 0
															// (enabled later)
		Dta1xxTxCtrlRegSetTxUflIntEn(pCh->m_pRegBase, 0);	// Clear interrupt enable bits
		Dta1xxTxCtrlRegSetTxSyncIntEn(pCh->m_pRegBase, 0);	//   ,,     ,,       ,,    ,,
		Dta1xxTxCtrlRegSetTxShortIntEn(pCh->m_pRegBase, 0);	//   ,,     ,,       ,,    ,,
		Dta1xxTxCtrlRegSetTxThrIntEn(pCh->m_pRegBase, 0);	//   ,,     ,,       ,,    ,,

        Dta1xxTxCtrlRegSetUseExtClk(pCh->m_pRegBase, 0);    // Use internal clock

		Dta1xxTxCtrlRegSetEnaPwr(pCh->m_pRegBase, 0);		// Disable Power to target adapter
		Dta1xxTxCtrlRegSetLedControl(pCh->m_pRegBase, 0);	// Disable overrule of LED control

		Dta1xxTxSetTxClock(pCh->m_pRegBase, 0);				// Clock to 0 bps
		Dta1xxTxSetFifoSizeReg(pCh->m_pRegBase, 8*1024*1024);	// Maximum FIFO size

		Dta1xxTxDiagRegSetLoopBack(pCh->m_pRegBase, 0);		// No loopback

		break;

	case 106:
	case 107:
	case 110:
		pCh = &pFdo->m_Channel[0];

		Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 0);	// Periodic-Interrupt
															// Enable := 0 (enabled later)

		// Set output level to maximum and disable noise generation
		if ( pFdo->m_TypeNumber==107 && pFdo->m_FirmwareVersion>=3 )
		{
			Dta1xxTxModcRegSetOutputLevel(pCh->m_pRegBase, 0xFF);
			Dta1xxTxModc2RegSetNoiseLevel(pCh->m_pRegBase, 0);
			Dta1xxTxModc2RegSetNoiseRange(pCh->m_pRegBase, 0);
		}
		
		if ( pFdo->m_TypeNumber == 110 )
		{
			// For Direct-IQ modulation modes we need a more than usual PCI bandwitdh, so
			// we set the latency timer to 96 PCI clocks
			NewLatTimer = 96;
		}
		break;

	case 111:
		Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 0);	// Periodic-Interrupt
													// (enabled later)
		Dta1xxGenCtrlRegSetPerIntInterval(pFdo->m_pGenRegs, 4); // 4 = 38.83 ms

		// Initialise QAM/IQ channel
		pCh = &pFdo->m_Channel[0];

		// Init scaler value based on current modulation type
		Dta1xxSetModulatorScaler(pCh);
		// Apply default scaler offset
		Dta1xxTxModc2RegSetOutputLevelOffset(pCh->m_pRegBase, 3);	 // Offset = +3

		// Set I, F and M for 474 MHz
		Dta1xxTxRfCtrl1RegSetPllI(pCh->m_pRegBase, 105);
		Dta1xxTxRfCtrl1RegSetPllF(pCh->m_pRegBase, 7);
		Dta1xxTxRfCtrl1RegSetPllM(pCh->m_pRegBase, 8);

		// Select QAM interpolation filter
		Dta1xxTxModcRegSetIntPolSel(pCh->m_pRegBase, DTA1XX_INTPOL_QAM);

		// For Direct-IQ modulation modes we need a higher than usual PCI bandwitdh,
		// so we set the latency timer to 96 PCI clocks
		NewLatTimer = 96;
		break;

	case 112:
	case 115:
		Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 0);	// Periodic-Interrupt
															// Enable := 0 (enabled later)
		Dta1xxGenCtrlRegSetPerIntInterval(pFdo->m_pGenRegs, 4); // 4 = 38.83 ms

		// Initialise TS Tx/Rx
		pCh = &pFdo->m_Channel[0];
		Dta1xxInitAsiSdi(pCh);

		// Initialise QAM/OFDM channel
		pCh = &pFdo->m_Channel[1];

		// Init scaler value based on current modulation type
		Dta1xxSetModulatorScaler(pCh);

		// Freq. to 474 MHz and apply corresponding freq. response correction

		// Set I, F and M for 474 MHz
		Dta1xxTxRfCtrl1RegSetPllI(pCh->m_pRegBase, 105);
		Dta1xxTxRfCtrl1RegSetPllF(pCh->m_pRegBase, 7);
		Dta1xxTxRfCtrl1RegSetPllM(pCh->m_pRegBase, 8);

		// DTA-115: Select QAM interpolation filter
		Dta1xxTxModcRegSetIntPolSel(pCh->m_pRegBase, DTA1XX_INTPOL_QAM);

		// Set output level to a 'safe' low level (i.e. 27.5dB attn)
		// NOTE: At 474MHz our correction is 1dB => total attn needs to be 27.5+1
		// NOTE: Ideally we want the input attn to be set to 4.5dB
		Dta1xxTxRfCtrl2RegSetInpAttn(pCh->m_pRegBase, 9);
		Dta1xxTxRfCtrl2RegSetOutpAttn(pCh->m_pRegBase, 48);

		// For Direct-IQ modulation modes we need a more than usual PCI bandwitdh, so we set
		// the latency timer to 96 PCI clocks
		NewLatTimer = 96;
		break;

	case 116:
	case 117:
		Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 0);	// Periodic-Interrupt
															// Enable := 0 (enabled later)
		Dta1xxGenCtrlRegSetPerIntInterval(pFdo->m_pGenRegs, 4); // 4 = 38.83 ms

		// Initialise TS Tx/Rx
		pCh = &pFdo->m_Channel[0];
		Dta1xxInitAsiSdi(pCh);

		// Initialise QAM/OFDM channel
		pCh = &pFdo->m_Channel[1];

		// Init scaler value based on current modulation type
		Dta1xxSetModulatorScaler(pCh);

		if ( pFdo->m_TypeNumber == 116 )
		{
			// Init IF frequency to 36MHz
			Dta1xxTxIfCtrlRegSetPllA(pCh->m_pRegBase, 0);
			Dta1xxTxIfCtrlRegSetPllB(pCh->m_pRegBase, 9);
			Dta1xxTxIfCtrlRegSetPllR(pCh->m_pRegBase, 2);
		}
		else /*if ( pFdo->m_TypeNumber == 117 )*/
		{
			// Init IF frequency to 44MHz
			Dta1xxTxIfCtrlRegSetPllA(pCh->m_pRegBase, 0);
			Dta1xxTxIfCtrlRegSetPllB(pCh->m_pRegBase, 11);
			Dta1xxTxIfCtrlRegSetPllR(pCh->m_pRegBase, 2);
		}

		// For Direct-IQ modulation modes we need a more than usual PCI bandwitdh, so we set
		// the latency timer to 96 PCI clocks
		NewLatTimer = 96;
		break;
	
	case 2111:
		Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 0);	// Periodic-Interrupt
															// Enable := 0 (enabled later)
		Dta1xxGenCtrlRegSetPerIntInterval(pFdo->m_pGenRegs, 4); // 4 = 38.83 ms
		
		// switch 1V8SW and 3V3SW on.
		// they are controlled from the board control 0 regists bits 4 and 5
		Dta1xxGenSetControl0Reg(pFdo->m_pGenRegs,Dta1xxGenGetControl0Reg(pFdo->m_pGenRegs)|0x30);
		
		// sleep shortly to allow power-supply to settle
		Dta1xxWaitMs(1);
		
		if ( pFdo->m_SupportsI2cItf ) {
			// Init I2C clock speed to 200kHz. ClkDiv=54M/200k/4=0x43
 			Dta1xxGenI2cCtrlRegSetClkDiv(pFdo->m_pGenRegs, 0x43);
		}
		
		// Capability based channel initialisation, based on DTA1XX_CHCAP1_AD9789
		// capability. This code works, without any change, outside this switch
		// statement, in the generic part of this function.
        for (i=0; i<pFdo->m_NumNonIpChannels; i++) {
            volatile UInt8*  pTxRegs;
            pCh = &pFdo->m_Channel[i];
    		pTxRegs = (((volatile UInt8 *)pCh->m_pRegBase)+pCh->m_AddrTxRegMapOffset);
            if ((pCh->m_Capability&DTA1XX_CHCAP_AD9789) != 0) {
                // reset channel to force initialisation of all channel resources
                Dta1xxTxCtrlRegTxReset(pTxRegs);
                // initialise channel registers
                Dta1xxInitAsiSdi(pCh);
                // initialise AD9789 chip
                Ad9789Init(pCh);
                // set default mod-control (DVB-C)
                Ad9789SetModControl(pCh, DTA1XX_MOD_QAM64, DTA1XX_MOD_J83_A, -1, -1);
                // set default RF-frequency (474MHz)
                Ad9789SetRfControl(pCh, 474000000LL);
                // set default symbol rate (6.89MBd)
                Ad9789SetSymSampleRate(pCh, 6890000);
    		}
            // Disable DMA TimeOut
            Dta1xxDmaTimeOutSet(pCh->m_Dma.m_pRegBase, 0);
        }
		break;

	case 120:
		if (pFdo->m_FirmwareVersion >= 4)
		{
			// "New" DTA-120
			Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 0);	// Periodic-Interrupt
																// Enable := 0 (enabled later)
		}
		break;

	case 122:
        
		pCh = &pFdo->m_Channel[0];
		// Periodic-Interrupt Enable := 0 (enabled later)
		if (pFdo->m_FirmwareVersion >= 4) {
			// "New" DTA-122
			Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 0);	// Periodic-Interrupt
		} else
			Dta1xxRxCtrlRegSetRxPerIntEn(pCh->m_pRegBase, 0);	// Periodic-Interrupt
																// (enabled later)
		//pRxRegs->m_RxControl.m_RxMode = 0;			// Receive Mode := St188
		//pRxRegs->m_RxControl.m_RxCtrl = 0;			// Receive Control := Idle
		//pRxRegs->m_RxControl.m_OvfIntEn = 0;		// Clear interrupt enable bits
		//pRxRegs->m_RxControl.m_SyncIntEn = 0;		//   ,,     ,,       ,,    ,,
		//pRxRegs->m_RxControl.m_ThrIntEn = 0;		//   ,,     ,,       ,,    ,,
		//pRxRegs->m_RxControl.m_RateOvfIntEn = 0;	//   ,,     ,,       ,,    ,,
		//pRxRegs->m_RxControl.m_EnaPwr	= 0;		// Disable Power to target adapter
		//pRxRegs->m_RxControl.m_LedControl	= 0;	// Disable overrule of LED control

		//pRxRegs->m_Diag.m_LoopBack = 0;				// No loopback
		break;

	case 124:
		Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 0);	// Periodic-Interrupt
															// Enable := 0 (enabled later)

		if (pFdo->m_FirmwareVersion > 0)
		{
			// Initialise TS Tx/Rx
			for (i=0; i<pFdo->m_NumNonIpChannels; i++) {
				pCh = &pFdo->m_Channel[i];

				Dta1xxInitAsiSdi(pCh);
				// Disable DMA TimeOut
				Dta1xxDmaTimeOutSet(pCh->m_Dma.m_pRegBase, 0);
			}
		}

		// Initialise DMA controller
		if (pFdo->m_FirmwareVersion > 0)
		{
			// Channel 0: digital video data to PCI bus
			WRITE_UINT32(pPci905XRegs, PCI905X_DMA0_MODE,
						PCI905X_DMAMODE_BUSWIDTH_32 | PCI905X_DMAMODE_READY_INPEN	|
						PCI905X_DMAMODE_BTERM_INPEN | PCI905X_DMAMODE_LOCALBURST	|
						PCI905X_DMAMODE_DEMAND		 | PCI905X_DMAMODE_LOC_CONST);

			// Channel 1: scatter-gather data from PCI bus
			WRITE_UINT32(pPci905XRegs, PCI905X_DMA1_MODE,
						PCI905X_DMAMODE_BUSWIDTH_32 | PCI905X_DMAMODE_READY_INPEN	|
						PCI905X_DMAMODE_BTERM_INPEN | PCI905X_DMAMODE_LOCALBURST	|
						PCI905X_DMAMODE_DEMAND		 | PCI905X_DMAMODE_LOC_CONST);

			// DMA threshold. NOTE: Local bus is faster then PCI bus
			// C0PLAF=0x0 ( 4 LWords), C0PLAE=0x1 ( 8 LWords)
			// C0LPAF=0x1 ( 8 LWords), C0LPAE=0x2 (12 LWords)
			// C1PLAF=0x0 ( 4 LWords), C1PLAE=0x1 ( 8 LWords)
			// C1LPAF=0x1 ( 8 LWords), C1LPAE=0x2 (12 LWords)
			PlxDmaThreshold = 0x21102110;
		}
		else
		{
			// Channel 0: digital video data to PCI bus
			WRITE_UINT32(pPci905XRegs, PCI905X_DMA0_MODE,
						PCI905X_DMAMODE_BUSWIDTH_32 | PCI905X_DMAMODE_READY_INPEN	|
						PCI905X_DMAMODE_BTERM_INPEN | PCI905X_DMAMODE_LOCALBURST	|
						PCI905X_DMAMODE_DONE_INTEN	 | PCI905X_DMAMODE_LOC_CONST);

			// Channel 1: scatter-gather data from PCI bus
			WRITE_UINT32(pPci905XRegs, PCI905X_DMA1_MODE,
						PCI905X_DMAMODE_BUSWIDTH_32 | PCI905X_DMAMODE_READY_INPEN	|
						PCI905X_DMAMODE_BTERM_INPEN | PCI905X_DMAMODE_LOCALBURST	|
						PCI905X_DMAMODE_DONE_INTEN);

			// DMA threshold: use legacy value
			PlxDmaThreshold = 0x00000010;
		}
		break;

	case 140:
		Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 0);	// Periodic-Interrupt
															// Enable := 0 (enabled later)
		break;

	case 105:
	case 145:
	case 545:
	case 2142:
	case 2144:
	case 2145:
		Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 0);	// Periodic-Interrupt
															// Enable := 0 (enabled later)
		// Initialise TS Tx/Rx
		for (i=0; i<pFdo->m_NumNonIpChannels; i++) {
			pCh = &pFdo->m_Channel[i];
			if ((pCh->m_Capability&DTA1XX_CHCAP_SPI) != 0) {
				Dta1xxSpiInit(pCh);
			}
			Dta1xxSetTxRateSel(pCh,DTA1XX_IORATESEL_INTCLK);
			Dta1xxInitAsiSdi(pCh);
			// Disable DMA TimeOut
			Dta1xxDmaTimeOutSet(pCh->m_Dma.m_pRegBase, 0);
		}

		if ( pFdo->m_SupportsI2cItf ) {
			// Init I2C clock speed to 200kHz. ClkDiv=54M/200k/4=0x43
 			Dta1xxGenI2cCtrlRegSetClkDiv((volatile UInt8*)pFdo->m_pGenRegs, 0x43);
		}

		break;

	case 160:
	case 2160:
		Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 0);	// Periodic-Interrupt
															// Enable := 0 (enabled later)
		Dta1xxGenCtrlRegSetPerIntInterval(pFdo->m_pGenRegs, 3); // 3 = 19.42 ms

		// Initialise TS Tx/Rx
		for (i=0; i<pFdo->m_NumNonIpChannels; i++) {
			pCh = &pFdo->m_Channel[i];
			Dta1xxInitAsiSdi(pCh);
			// Disable DMA TimeOut
			Dta1xxDmaTimeOutSet(pCh->m_Dma.m_pRegBase, 0);
		}

		// Enable / Disable IP packets Tx/Rx
		for (i=0; i<pFdo->m_NumIpPorts; i++) {
			// Wait until the Phy is out of reset before writing to any network registers.
			Dta1xxEthWaitMacReady(&pFdo->m_NwStore[i]);

			// Enable NDIS/DVB receive
			Dta1xxNwRxCtrlSetRxControl(pFdo->m_IpMqRx[i]->m_pRegBase, 1);

			// Enable DVB transmit
			Dta1xxNwTxCtrlSetTxControl(pFdo->m_IpMqTx[i]->m_pRegBase, 1);
			Dta1xxNwTxCtrlSetTxOnTime(pFdo->m_IpMqTx[i]->m_pRegBase, 1);

			// Enable NDIS transmit
			Dta1xxNwTxCtrlSetTxControl(pFdo->m_NwTx[i]->m_pRegBase, 1);
			Dta1xxNwTxCtrlSetTxOnTime(pFdo->m_NwTx[i]->m_pRegBase, 0);

			// Enable deletion of incorrect packets by FPGA
			Dta1xxNwRxCtrlSetDelPckFpga(pFdo->m_IpMqRx[i]->m_pRegBase, 1);

			// Set receive delay
			Dta1xxDmaTimeOutSet(pFdo->m_IpMqRx[i]->m_Dma.m_pRegBase, 1);

			// Enable Rx Fifo Overflow interrupt enable
			Dta1xxNwRxCtrlSetRxFifoOvfIntEn(pFdo->m_IpMqRx[i]->m_pRegBase, 1);
		}

		// Initialise DMA controller, only for DTA-160
		if (pFdo->m_Uses9056) {
			// Channel 0: digital video data to PCI bus
			WRITE_UINT32(pPci905XRegs, PCI905X_DMA0_MODE,
						 PCI905X_DMAMODE_BUSWIDTH_32 | PCI905X_DMAMODE_READY_INPEN	|
						 PCI905X_DMAMODE_BTERM_INPEN | PCI905X_DMAMODE_LOCALBURST	|
						 PCI905X_DMAMODE_DEMAND		 | PCI905X_DMAMODE_LOC_CONST);

			// Channel 1: scatter-gather data from PCI bus
			WRITE_UINT32(pPci905XRegs, PCI905X_DMA1_MODE,
						 PCI905X_DMAMODE_BUSWIDTH_32 | PCI905X_DMAMODE_READY_INPEN	|
						 PCI905X_DMAMODE_BTERM_INPEN | PCI905X_DMAMODE_LOCALBURST	|
						 PCI905X_DMAMODE_DEMAND		 | PCI905X_DMAMODE_LOC_CONST);

			// DMA threshold. NOTE: Local bus is faster then PCI bus
			// C0PLAF=0x0 ( 4 LWords), C0PLAE=0x1 ( 8 LWords)
			// C0LPAF=0x1 ( 8 LWords), C0LPAE=0x2 (12 LWords)
			// C1PLAF=0x0 ( 4 LWords), C1PLAE=0x1 ( 8 LWords)
			// C1LPAF=0x1 ( 8 LWords), C1LPAE=0x2 (12 LWords)
			PlxDmaThreshold = 0x21102110;
		}

		// Initialise PHY/MAC('s)

		// For all network ports:
		//  - Disable Mdio Interrupt
		//  - Initialise MII Serial Management registers
		//  - Create DPC for link auto neg. update
		//  - Create DPC for sending setup frame
		for (i=0; i<pFdo->m_NumIpPorts; i++) {
			Dta1xxNwIntCtrlSetLinkValid(pFdo->m_NwStore[i].m_pRegBase, FALSE);
			Dta1xxNwIntCtrlSetMdioIntEn(pFdo->m_NwStore[i].m_pRegBase, FALSE);
			Dta1xxNwMacInitMiiSm(&(pFdo->m_NwStore[i]));
		}

		// The IP channel on the DTA-160 requires a more than usual PCI bandwitdh, so we set
		// the latency timer to 96 PCI clocks
		NewLatTimer = 96;
		break;

	case 2135:
		Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 0);	// Periodic-Interrupt
															// Enable := 0 (enabled later)

		// Init I2C clock speed to 200kHz. ClkDiv=54M/200k/4=0x43
		Dta1xxGenI2cCtrlRegSetClkDiv(pFdo->m_pGenRegs, 0x43);

		// Initialise DVB-T receiver channel
		// NOTE: register layout is the same as ASI/SDI channel
		for (i=0; i<2; i++) {
			pCh = &pFdo->m_Channel[i];
			
			Dta1xxInitAsiSdi(pCh);
			// Disable DMA TimeOut
			Dta1xxDmaTimeOutSet(pCh->m_Dma.m_pRegBase, 0);
		}
		
		// Init ADC channel
		pCh = &pFdo->m_Channel[2];
		// FOR NOW: init as if it were a normal channel
		Dta1xxInitAsiSdi(pCh);
		// Disable DMA TimeOut
		Dta1xxDmaTimeOutSet(pCh->m_Dma.m_pRegBase, 0);
		break;

	case 2136:
		Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 0);	// Periodic-Interrupt
																// Enable := 0 (enabled later)

		// Init I2C clock speed to 200kHz. ClkDiv=54M/200k/4=0x43
 		Dta1xxGenI2cCtrlRegSetClkDiv(pFdo->m_pGenRegs, 0x43);

		// Initialise DVB-C receiver channel
		// NOTE: register layout is the same as ASI/SDI channel
		for (i=0; i<pFdo->m_NumNonIpChannels; i++) {
			pCh = &pFdo->m_Channel[i];

			Dta1xxInitAsiSdi(pCh);
			// Disable DMA TimeOut
			Dta1xxDmaTimeOutSet(pCh->m_Dma.m_pRegBase, 0);
		}
		break;

	case 2137:
			Dta1xxGenCtrlRegSetPerIntEn(pFdo->m_pGenRegs, 0);	// Periodic-Interrupt
																// Enable := 0 (enabled later)

			// Init I2C clock speed to 200kHz. ClkDiv=54M/200k/4=0x43
 			Dta1xxGenI2cCtrlRegSetClkDiv(pFdo->m_pGenRegs, 0x43);

			// Initialise DVB-T receiver channel
			// NOTE: register layout is the same as ASI/SDI channel
			for (i=0; i<pFdo->m_NumNonIpChannels; i++) {
				pCh = &pFdo->m_Channel[i];
				
				Dta1xxInitAsiSdi(pCh);
				// Disable DMA TimeOut
				Dta1xxDmaTimeOutSet(pCh->m_Dma.m_pRegBase, 0);
			}
			break;

	} // switch (pFdo->m_TypeNumber)

	// Assign new latency timer value (if needed)
	if ( CurrLatTimer != NewLatTimer )
	{
		Status = Dta1xxWriteConfigSpace(pFdo, PCI905X_LATENCY_TIMER, 1, &NewLatTimer);
#if LOG_LEVEL > 0
		if ( Status != STATUS_SUCCESS ){
			DTA1XX_LOG( KERN_INFO, "Dta1xxInitDeviceHardware: failed to assign new latency timer value (New=%d, Current=%d)",
						NewLatTimer, CurrLatTimer );
		}
		else {
			DTA1XX_LOG( KERN_INFO, "Dta1xxInitDeviceHardware: assigned new latency timer value (New=%d, Current=%d)",
						NewLatTimer, CurrLatTimer );
		}

		// Read back latency timer
		Status = Dta1xxReadConfigSpace(pFdo, PCI905X_LATENCY_TIMER, 1, &CurrLatTimer);
		DTA1XX_LOG( KERN_INFO, "Dta1xxInitDeviceHardware: actual latency timer value=%d",
					CurrLatTimer );
#endif
	}

	// For devices with a PLX set the DMA read and write command
	// Write: B"0111" => PCI Write Command
	// Read:  B"1100" => PCI Memory Read Multiple Command
	if ( pFdo->m_Uses9054 || pFdo->m_Uses9056 )
		WRITE_UINT8(pPci905XRegs, PCI905X_EEPROM_CTRL_STAT, 0x7C);

	// Write DMA thresholds value (only for cards that use the 9056)
	if ( pFdo->m_Uses9056 )
		WRITE_UINT32(pPci905XRegs, PCI905X_DMA_THRESHOLD, PlxDmaThreshold);

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxInitGenlock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dta1xxInitGenlock(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	// Shortcut pointer to general registers
	volatile UInt8*  pGenReg = pFdo->m_pGenRegs;
	Int Capability, IoConfig, ParXtra;
	Channel* pCh;
	BOOLEAN EnableVCO = FALSE;

	// **
	// GENLOCK RX specifics
	// **

	// Retrieve IOConfig channel 0 through driver
	pCh = &pFdo->m_Channel[0];
	Capability = pCh->m_Capability;
	IoConfig = pCh->m_IoConfig;
	ParXtra = pCh->m_IoConfigPar;

	// Check for genlock RX support
	if ((Capability & DTA1XX_CHCAP_GENREF) != DTA1XX_CHCAP_GENREF)
		return STATUS_SUCCESS;


	// Check for Genlock RX enabled on channel 0
	if (IoConfig==DTA1XX_IOCONFIG_GENREF) {
		if ((ParXtra != DTA1XX_GENLOCK_SDI625) &&
			(ParXtra != DTA1XX_GENLOCK_SDI525)) {
			DTA1XX_LOG(KERN_INFO, "Dta1xxInitGenlock: Genlock configuration error");
			return -EFAULT;
		}

#if LOG_LEVEL_GENLOCK > 0
		if (ParXtra == DTA1XX_GENLOCK_SDI625) {
			DTA1XX_LOG(KERN_INFO, "Dta1xxInitGenlock: SDI 625 Genlock Enabled");
		}
		else if (ParXtra == DTA1XX_GENLOCK_SDI525) {
			DTA1XX_LOG(KERN_INFO, "Dta1xxInitGenlock: SDI 525 Genlock Enabled");
		}
#endif
			
		pFdo->m_Genlock_vidstd = ParXtra;
		
		// Force channel 0 to operate in SDI mode
#if LOG_LEVEL_GENLOCK > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxInitGenlock: Forcing channel 0 in SDI mode");
#endif
		Dta1xxRxCtrlRegSetSdiMode(pCh->m_pRegBase, 1);

		// Initialise Genlock RX logic
		pFdo->m_Genlock = 0;			// indicate out of lock
		pFdo->m_GenlockPrev = 0;
		pFdo->m_GraceCnt = 0;

		pFdo->m_I2cWait = 0;
		pFdo->m_I2cBusy = 0;
	
		// Note: This implementation only enables Genlock TX when
		//		 channel 0 is in Genlock RX mode
		// Note: Revise this when multiboard genlock is implemented
		EnableVCO = TRUE;
	}	

	// **
	// Genlock TX specifics
	// **


	// For now Always/Only enable the Vco for Genlock TX when channel 0 
	// is in Genlock RX mode
	// Note: Revise this when multiboard genlock is implemented
	if (EnableVCO)
	{
#if LOG_LEVEL_GENLOCK > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxInitGenlock:  Enabling on board VCO");
#endif
		// Enable Vcxo for device wide SDI TX Timing generator
		Dta1xxGenSetVcxoEn(pGenReg, 1);
		// Programm SDI TX Timing generator with the configured SDI standard
		Dta1xxGenSetVcxoStd(pGenReg, pFdo->m_Genlock_vidstd);
		// Start VCXO at 50%
		pFdo->m_VcxoCtrl = 512000;
		Dta1xxSetVcxoSpeed(pFdo);
		// Initialise SDI frame position
		Dta1xxGenSetTxSdiPosReg(pGenReg, 0x0);
	}
	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxDisableGenlock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dta1xxDisableGenlock(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	// This function is only called when Genlock is disables
	// after user issues an IOCONFIG request

	// Shortcut pointer to general registers
	volatile UInt8*  pGenReg = (volatile UInt8*) pFdo->m_pGenRegs;
	Int Capability, IoConfig;
	Channel* pCh;

	pCh = &pFdo->m_Channel[0];
	Capability = pCh->m_Capability;
	IoConfig = pCh->m_IoConfig;

	// Check for genlock support
	if ((Capability & DTA1XX_CHCAP_GENREF) != DTA1XX_CHCAP_GENREF)
	{
#if LOG_LEVEL_GENLOCK > 0
		DTA1XX_LOG(KERN_INFO, "Dta1xxDisableGenlock: ERROR: Non Genlock capable device");
#endif
		return 0;
	}

#if LOG_LEVEL_GENLOCK > 0
	DTA1XX_LOG(KERN_INFO, "Dta1xxDisableGenlock: Disabling Genlock Rx and Genlock Tx");
#endif

	// **
	// Genlock RX specific
	// **

	// reset SDI mode
	// Note: Rx channel must be inactive when turning off SDI mode
	Dta1xxRxCtrlRegSetSdiMode(pCh->m_pRegBase, 0);

	// **
	// Genlock TX specific
	// **

	// Disable Vcxo
	// Note: Tx channels must be inactive when disabling VCO
	// Note: This implementation only enables Genlock TX when
	//		 channel 0 is in Genlock RX mode
	// Note: Revise this when multiboard genlock is implemented
	Dta1xxGenSetVcxoEn(pGenReg, 0);
	
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxInitDmaResources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Initialise DMA resources
//
Int  Dta1xxInitDmaResources(
	Channel*  pCh)			// Channel pointer
{
	volatile UInt8*  pBase;
	DTA1XX_FDO*  pFdo = pCh->m_pFdo;

	// Direction
	if (  pCh->m_ChannelType==DTA1XX_TS_RX_CHANNEL
	   || pCh->m_ChannelType==DTA1XX_IP_RX_CHANNEL
	   || pCh->m_ChannelType==DTA1XX_NW_RX_CHANNEL) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxInitDmaResources: DTA1XX_DMA_READ",
					pCh->m_PortIndex );
#endif
		pCh->m_Dma.m_DmaDir = DTA1XX_DMA_READ;
	} else {
		pCh->m_Dma.m_DmaDir = DTA1XX_DMA_WRITE;
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxInitDmaResources: DTA1XX_DMA_WRITE",
					pCh->m_PortIndex );
#endif
	}

	// Initialise register pointers
	if ( (pFdo->m_TypeNumber == 105) ||
		((pFdo->m_TypeNumber == 124) && (pFdo->m_FirmwareVersion > 0)) ||
		 (pFdo->m_TypeNumber == 112) ||
		 (pFdo->m_TypeNumber == 115) ||
		 (pFdo->m_TypeNumber == 116) ||
		 (pFdo->m_TypeNumber == 117) ||
		 (pFdo->m_TypeNumber == 145) ||
		 (pFdo->m_TypeNumber == 160) ||
		 (pFdo->m_TypeNumber == 545) ||
		 (pFdo->m_TypeNumber == 2111)||
		 (pFdo->m_TypeNumber == 2135)||
		 (pFdo->m_TypeNumber == 2136)||
		 (pFdo->m_TypeNumber == 2137)||
		 (pFdo->m_TypeNumber == 2142)||
		 (pFdo->m_TypeNumber == 2144)||
		 (pFdo->m_TypeNumber == 2145)||
		 (pFdo->m_TypeNumber == 2160))
	{

		if ((pCh->m_ChannelType == DTA1XX_TS_RX_CHANNEL) ||
			(pCh->m_ChannelType == DTA1XX_TS_TX_CHANNEL))
		{
			pBase  = pCh->m_pFdo->m_Dta1xxRegsBase.m_pKernel;

			pBase += (pCh->m_AddrBaseOffset + pCh->m_AddrDmaRegMapOffset);
			pCh->m_Dma.m_pRegBase = pBase;
			pCh->m_Dma.m_pRegDmaDesc = pBase + 0x4;
			pCh->m_Dma.m_pRegCmdStat = pBase + 0x8;
		}
		else
		{	// For now only for DTA-160/2160
			pBase = (UInt8*) pCh->m_pRegBase;
			pCh->m_Dma.m_pRegBase = pBase + 0x80;
			pCh->m_Dma.m_pRegDmaDesc = pBase + 0x84;
			pCh->m_Dma.m_pRegCmdStat = pBase + 0x88;
		}
	}
	else
	{
		pBase = (UInt8*) pCh->m_pRegBase;
		pCh->m_Dma.m_pRegBase = NULL;		// m_pRegBase is not used for now
		pCh->m_Dma.m_pRegDmaDesc = pBase + DTA1XX_RX_REG_DMA_DESC_PTR;
		pCh->m_Dma.m_pRegCmdStat = pBase + DTA1XX_RX_REG_DMA_COMMAND_STAT;
	}

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Init CDMA resources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	init_waitqueue_head( &pCh->m_CdmaEvent );
	// Reset CDMA done flag
	clear_bit( 0, (void*)(&pCh->m_CdmaDone) );

	if (pCh->m_ChannelType == DTA1XX_IP_TX_CHANNEL) {
		// For IpTx channel, we use a completion routine
		InitializeDpc(&pCh->m_CdmaDpcCompleted, Dta1xxCdmaDpcDma, pCh);
	} 

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Init DMA resources -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	// Init DMA lock
	init_MUTEX(&pCh->m_Dma.m_Lock);
	// Init DMA wait queue
	init_waitqueue_head(&pCh->m_Dma.m_DmaDoneQueue);

	// Set DMA masks to make sure 'pci_alloc_consistent()' 
	//		allocates within the 32-bit address range.
	if (	(pci_set_dma_mask( pFdo->m_pPdo, DMA_32BIT_MASK ) < 0) ||
			(pci_set_consistent_dma_mask( pFdo->m_pPdo, DMA_32BIT_MASK ) < 0)) {
		DTA1XX_LOG( KERN_ERR, "Dta1xxInitDmaResources: Unable to set DMA mask!");
		return -EFAULT;
	}

	// For network Tx and Rx, we allocate the buffers somewere else
	if ((pCh->m_ChannelType != DTA1XX_NW_RX_CHANNEL) &&
		(pCh->m_ChannelType != DTA1XX_IP_RX_CHANNEL) &&
		(pCh->m_ChannelType != DTA1XX_IP_TX_CHANNEL)) {
		{
			// Allocate DMA buffer
			pCh->m_Dma.m_DmaBuffer.m_pVirtStartAddr =
					pci_alloc_consistent( pFdo->m_pPdo, DTA1XX_DMA_BUF_SIZE,
										  &pCh->m_Dma.m_DmaBuffer.m_PhysStartAddr );

			// Allocation succeeded?
			if ( pCh->m_Dma.m_DmaBuffer.m_pVirtStartAddr==NULL )
			{
				DTA1XX_LOG( KERN_CRIT, 
					"Dta1xxInitDmaResources: failed to allocate DMA buffer" );
				return -EFAULT;
			}

#if defined(CONFIG_X86_64)
			// Check (64-bit Linux only) whether allocated memory resides below 32-bit boundary
			if ( (pCh->m_Dma.m_DmaBuffer.m_PhysStartAddr>>32) != 0 ) {
				DTA1XX_LOG( KERN_CRIT, 
					"Dta1xxInitDmaResources: DMA buffer address exceeds 32-bit boundary!" );
				return -EFAULT;
			}
#endif
		}
	}

	// Allocate buffer for sgl-descriptor buffer (only for Devices with DMA in FPGA)
	pCh->m_Dma.m_pSglDesc = NULL;
	if ( pFdo->m_UsesDmaInFpga )
	{
		pCh->m_Dma.m_pSglDesc =
			pci_alloc_consistent( pFdo->m_pPdo, sizeof(SglDesc),
								  &pCh->m_Dma.m_PhysAddrSglDesc);

		// Allocation succeeded??
		if ( pCh->m_Dma.m_pSglDesc == NULL )
		{
			DTA1XX_LOG( KERN_CRIT, 
						"Dta1xxInitDmaResources: failed to allocate sgl-desc buffer" );
			return -EFAULT;
		}

#if defined(CONFIG_X86_64)
		// Check (64-bit linux only) whether allocated memory resides below 32-bit boundary
		if ( (pCh->m_Dma.m_PhysAddrSglDesc>>32) != 0 ) {
			DTA1XX_LOG( KERN_CRIT, 
				"Dta1xxInitDmaResources: Sgl-desc buffer address exceeds 32-bit boundary!" );
			return -EFAULT;
		}
#endif
	}
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxInitModulatorOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dta1xxInitModulatorOffset(
	DTA1XX_FDO*  pFdo)		// Functional device object, representing the DTA-1xx card
{
	Int  IOffsetSigned, QOffsetSigned, Status;
	UInt  IAdjust, IAdjustF1, IAdjustF2, IAdjustF3;
	UInt  QAdjust, QAdjustF1, QAdjustF2, QAdjustF3;
	UInt  IOffsDir, IOffset, IFineGain, QOffsDir, QOffset, QFineGain;
	char  StrM1[DTA1XX_VPD_ITEM_MAX_LENGTH+1];		// Buffer for storing VPD item string

	// Just for DTA-110(T) at the moment
	if ( pFdo->m_TypeNumber!=110 )
		return 0;

	// Read resource "M1" from VPD
	Status = Dta1xxVpdReadItemRoRw(pFdo, "M1", StrM1);
	if ( Status != 0 )
		return Status;		// "M1" does not exist (but not fatal)

	// Convert to 19-bit words
	IAdjustF1 = StrM1[0] - 0x20;
	IAdjustF2 = StrM1[1] - 0x20;
	IAdjustF3 = StrM1[2] - 0x20;
	QAdjustF1 = StrM1[3] - 0x20;
	QAdjustF2 = StrM1[4] - 0x20;
	QAdjustF3 = StrM1[5] - 0x20;

	IAdjust = (IAdjustF1<<12) | (IAdjustF2<<6) | IAdjustF3;
	QAdjust = (QAdjustF1<<12) | (QAdjustF2<<6) | QAdjustF3;

	// Reconstruct DAC offset and fine-gain values
	IOffsDir  = (IAdjust>>18) & 1;
	IOffset   = (IAdjust>> 8) & 0x3FF;
	IFineGain = (IAdjust    ) & 0xFF;
	QOffsDir  = (QAdjust>>18) & 1;
	QOffset   = (QAdjust>> 8) & 0x3FF;
	QFineGain = (QAdjust    ) & 0xFF;

	// Signed offset values
	IOffsetSigned = (IOffsDir == 0) ? IOffset : -(Int)IOffset;
	QOffsetSigned = (QOffsDir == 0) ? QOffset : -(Int)QOffset;

#if LOG_LEVEL_MODULATION > 0
	DTA1XX_LOG( KERN_INFO,
				"Dta1xxInitModulatorOffset: Init TxDAC: IFineGain=%d QFineGain=%d "
				"IOffset=%d QOffset=%d", 
				IFineGain, QFineGain, IOffsetSigned, QOffsetSigned );
#endif

	// Write values to TxDAC
	Dta1xxTxIDacCtrlSetFineGain(pFdo->m_Channel[0].m_pRegBase, IFineGain);
	Dta1xxTxIDacCtrlSetOffsDir(pFdo->m_Channel[0].m_pRegBase, IOffsDir);
	Dta1xxTxIDacCtrlSetOffset(pFdo->m_Channel[0].m_pRegBase, IOffset);

	Dta1xxTxQDacCtrlSetFineGain(pFdo->m_Channel[0].m_pRegBase, QFineGain);
	Dta1xxTxQDacCtrlSetOffsDir(pFdo->m_Channel[0].m_pRegBase, QOffsDir);
	Dta1xxTxQDacCtrlSetOffset(pFdo->m_Channel[0].m_pRegBase, QOffset);

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxInitPci905X -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// *** Called for PCI cards with PLX chip only ***
//
// In principle, initialising the PCI-9054 or PCI-9056 is required only if the serial
// EEPROM has not been programmed yet.
// However, due to a bug in the PCI-9054 chip (see Errata #9 in 9054ABerrata-16.pdf),
// the serial EEPROM is not reloaded when the Power Management State is changed from
// D3hot to D0. This is a problem e.g. when disabling and enabling a device driver.
//
// So: we simply always initialise the PCI-9054, and also the PCI-9056.
//
// REGISTER VALUES MUST MATCH VALUES PROGRAMMED INTO THE SERIAL EEPROM!
//
void  Dta1xxInitPci905X(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	volatile UInt8*  p905XRegs;

#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "Dta1xxInitPci905X: Initialise PCI-905X registers" );
#endif

	// Pointer to PCI-9054/9056 registers
	p905XRegs = pFdo->m_Pci905XConfRegs.m_pKernel;

	// LAS0RR is a problem, because value should be different for
	// DTA-107/110/112/115/116/117/124/140/160 compared to DTA-100/102/120/122.
	//
	// Uninitialised DTA-1xx board => don't initialise LAS0RR register at all
	if (pFdo->m_SubVendorId != 0x10B5)
	{
		if (     pFdo->m_TypeNumber==107
			  || pFdo->m_TypeNumber==110
			  || pFdo->m_TypeNumber==140)
			WRITE_UINT32(p905XRegs, PCI905X_SPACE0_RANGE, 0xFFFFFE00);
		else if (   pFdo->m_TypeNumber==111 || pFdo->m_TypeNumber==112
				 || pFdo->m_TypeNumber==115 || pFdo->m_TypeNumber==116
				 || pFdo->m_TypeNumber==117 || pFdo->m_TypeNumber==124 )
			WRITE_UINT32(p905XRegs, PCI905X_SPACE0_RANGE, 0xFFFFFC00);
		else if (pFdo->m_TypeNumber==160)
			WRITE_UINT32(p905XRegs, PCI905X_SPACE0_RANGE, 0xFFFFF000);
		else
			WRITE_UINT32(p905XRegs, PCI905X_SPACE0_RANGE, 0xFFFFFF80);
	}

	// Other registers
	WRITE_UINT32(p905XRegs, PCI905X_SPACE0_REMAP, 1);				// LAS0BA

	if (     pFdo->m_TypeNumber==111 ||   pFdo->m_TypeNumber==112 
		||   pFdo->m_TypeNumber==115 ||   pFdo->m_TypeNumber==116 
		||   pFdo->m_TypeNumber==117 ||   pFdo->m_TypeNumber==160
		|| ((pFdo->m_TypeNumber==124) && (pFdo->m_FirmwareVersion > 0))) {
		// Dma priority rotating
		WRITE_UINT32(p905XRegs, PCI905X_LOCAL_DMA_ARBIT, 0x02000000);	// MARBR
	} else {
		// Dma priority channel 0 (DTA-124 firmware version == 0)
		WRITE_UINT32(p905XRegs, PCI905X_LOCAL_DMA_ARBIT, 0x02080000);	// MARBR
	}
	WRITE_UINT8(p905XRegs, PCI905X_ENDIAN_DESC, 0);					// BIGEND
	WRITE_UINT8(p905XRegs, PCI905X_LOCAL_MISC, 0x04);				// LMISC
	WRITE_UINT16(p905XRegs, PCI905X_PROT_AREA, 0x007F);				// PROT_AREA
	WRITE_UINT32(p905XRegs, PCI905X_EXP_ROM_RANGE, 0x0000);			// EROMRR
	WRITE_UINT32(p905XRegs, PCI905X_EXP_ROM_REMAP, 0x0000);			// EROMBA
//	WRITE_UINT32(p905XRegs, PCI905X_SPACE0_ROM_DESC, 0x43430143);	// LBRD0
	WRITE_UINT32(p905XRegs, PCI905X_SPACE0_ROM_DESC, 0xF3430143);	// LBRD0

	WRITE_UINT32(p905XRegs, PCI905X_DM_RANGE, 0);					// DMRR
	WRITE_UINT32(p905XRegs, PCI905X_DM_MEM_BASE, 0);				// DMLBAM
	WRITE_UINT32(p905XRegs, PCI905X_DM_IO_BASE, 0);					// DMLBAI
	WRITE_UINT32(p905XRegs, PCI905X_DM_PCI_MEM_REMAP, 0);			// DMPBAM
	WRITE_UINT32(p905XRegs, PCI905X_DM_PCI_IO_CONFIG, 0);			// DMCFGA

	WRITE_UINT32(p905XRegs, PCI905X_SPACE1_RANGE, 0);				// LAS1RR
	WRITE_UINT32(p905XRegs, PCI905X_SPACE1_REMAP, 0);				// LAS1BA
	WRITE_UINT32(p905XRegs, PCI905X_SPACE1_DESC, 0x03430000);		// LBRD1
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxInitVpd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialise VPD caching, read and interpret VPD configuration resources
//
Int  Dta1xxInitVpd(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	Int  Status, i;
	size_t  AllocSize;

	// Initialisation PLX IO mutex
	init_MUTEX(&pFdo->m_VpdEepromMutex);

	// Initialisation VPD user access mutex
	init_MUTEX(&pFdo->m_VpdMutex);

	// Allocating EEPROM cache buffer
	if ( pFdo->m_UsesPlxChip || !pFdo->m_UseFpgaForVpd )
		AllocSize = pFdo->m_VpdEepromSize;
	else
		AllocSize = pFdo->m_EepromSectorSize;

#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "Dta1xxInitVpd: Allocate EEPROM cache buffer (%d bytes)",
				(Int)AllocSize );
#endif
	pFdo->m_pEepromCache = kmalloc( AllocSize, GFP_KERNEL );
	if (pFdo->m_pEepromCache == NULL)
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxInitVpd: Allocating EEPROM cache buffer FAILS" );
#endif
		return -ENOMEM;
	}

#if LOG_LEVEL > 0
	DTA1XX_LOG(KERN_INFO, "Dta1xxInitVpd: EEPROM cache buffer allocated succesfully");
#endif

	// Read EEPROM content
	if (pFdo->m_UsesPlxChip)
	{
		// Read entire content of VPD EEPROM (PLX) into cache buffer
		Status = Dta1xxVpdRead(pFdo, pFdo->m_pEepromCache, 0, PLX_VPD_SIZE);
		if (Status != 0)
			return Status;
		pFdo->m_pVpdEeprom = pFdo->m_pEepromCache;
	}
	else if (pFdo->m_UseFpgaForVpd)
	{
		// Read last page of FPGA EEPROM into cache buffer
		Status = Dta1xxEepromReadLastPage(pFdo);
		if (Status != 0)
			return Status;
		pFdo->m_pVpdEeprom = pFdo->m_pEepromCache + pFdo->m_EepromRelAddrVpd;
	}
	else
	{
		// Read separate EEPROM connected to FPGA for VPD data
		Status = Dta1xxEepromSpiRead(pFdo);
		if (Status != 0)
			return Status;
		pFdo->m_pVpdEeprom = pFdo->m_pEepromCache;
	}

	// Locate the start of the VpdId item.
	Dta1xxVpdInitOffset(pFdo);

	//.-.-.-.-.-.-.-.-.-.-.-.-.- Initialisation from VPD Items -.-.-.-.-.-.-.-.-.-.-.-.-.-

	// Initialise MAC address on cards with an Ethernet network connection
	for (i=0; i<pFdo->m_NumIpPorts; i++)
		Dta1xxNwMacInitMacAddress(&(pFdo->m_NwStore[i]));

	// Initialise channel direction and channel dubbel buffering from registry
//	Dta1xxInitChannelsFromRegistry(pFdo);

	// Initialise TxDAC offsets on modulator cards
	Dta1xxInitModulatorOffset(pFdo);

	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxInitWorkerThreads -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Initialise worker threads.
//
NTSTATUS  Dta1xxInitWorkerThreads(IN PDTA1XX_FDO pFdo)
{
	SIZE_T  AllocSize;
	Int	Counter;
	Int i;
	NwStore* pNwStore;
	NTSTATUS Status = STATUS_SUCCESS;
	
	// Initialise IP Receiver worker thread to handle DMA and extract packet headers
	for (i = 0; i < pFdo->m_NumIpPorts; i++) {
		pNwStore = &pFdo->m_NwStore[i]; 
		
		// Allocate IP Receive buffer before starting worker thread
#ifdef WINBUILD	
		AllocSize = IPRX_DMA_BUFFER_SIZE;
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[NwRx%d] Dta1xxInitWorkerThreads: Allocate IP Rx buffer (%d bytes * %d)",
				    i, AllocSize, NUM_IPRX_DMA_QUEUES );
#endif
		
		for (Counter=0; Counter<NUM_IPRX_DMA_QUEUES; Counter++) {
			
			pNwStore->m_pIpRxBuffer[Counter] = ExAllocatePoolWithTag(NonPagedPool, AllocSize, '1ATD');
			
			if (pNwStore->m_pIpRxBuffer[Counter] == NULL) {
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "[NwRx%d] Dta1xxInitWorkerThreads: Allocating IP Rx buffer %i"
						    "FAILS", i, Counter );
#endif
				return STATUS_INSUFFICIENT_RESOURCES;
			}
		}
#else
		AllocSize = IPRX_DMA_BUFFER_SIZE * NUM_IPRX_DMA_QUEUES;
		Status = AllocateCdmaBuffer(pFdo->m_IpMqRx[i], (UInt*)&AllocSize);

		if ((AllocSize % NUM_IPRX_DMA_QUEUES) != 0) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, 
						"[NwRx%d] Dta1xxInitWorkerThreads: Error allocating IP Rx buffer. AllocSize "
						"not multiple of number of queues. AllocSize: %u, #bufs: %i!",
						i, (Int)AllocSize, NUM_IPRX_DMA_QUEUES );	
#endif
			return STATUS_UNSUCCESSFUL;
		}
		if (Status != STATUS_SUCCESS) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[NwRx%d] Dta1xxInitWorkerThreads: Allocating IP Rx buffer"
					    "FAILS", i );
#endif
			return STATUS_INSUFFICIENT_RESOURCES;
		}
		
		Status = Dta1xxCdmaSglCreate(pFdo->m_IpMqRx[i]);
		if (Status != STATUS_SUCCESS) {
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[NwRx%d] Dta1xxInitWorkerThreads: Creating Sgl IP Rx buffer"
					    "FAILS", i);
#endif
			return STATUS_INSUFFICIENT_RESOURCES;
		}
		pFdo->m_IpMqRx[i]->m_CdmaShBuf.m_SglAllocated = TRUE;

		for (Counter=0; Counter<NUM_IPRX_DMA_QUEUES; Counter++) {
			
			pNwStore->m_pIpRxBuffer[Counter] = 
				pFdo->m_IpMqRx[i]->m_CdmaShBuf.m_pBuf +
				(Counter * (pFdo->m_IpMqRx[i]->m_CdmaBufSize / NUM_IPRX_DMA_QUEUES));
		}
		
#endif
	}
	if (pFdo->m_NumIpPorts != 0) {
		if (Dta1xxNwIpRxCreateWorkerThreads(pFdo) != STATUS_SUCCESS) {
			return STATUS_INSUFFICIENT_RESOURCES;
		}
		pFdo->m_NwInitialised = TRUE;
	}
	return STATUS_SUCCESS;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ HELPERS +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxInitAsiSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialise Asi/Sdi Tx/Rx ports
//
// Pre: pCh->m_Capability: DTA1XX_CHCAP_BIDIR_PORT must be set for Bi-Directional Ts ports
// For DTA1XX_CHCAP_BIDIR_PORT: pBase_Tx(i) = pBase_Rx(i)+0x80;
//
void Dta1xxInitAsiSdi(Channel*  pCh)
{
	volatile UInt8*  pTxRegs = NULL;
	DTA1XX_FDO*  pFdo = pCh->m_pFdo;

	if (pCh->m_ChannelType == DTA1XX_TS_TX_CHANNEL)
	{
		// ASI/SDI Tx
		Dta1xxTxReset(pCh, DTA1XX_RST_CHANNEL);
		Dta1xxTxCtrlRegSetTxMode(pCh->m_pRegBase, 0);		// Transmit Mode := 188
		Dta1xxTxCtrlRegSetTxPckStuff(pCh->m_pRegBase, 0);	// No Null-Packet Stuffing
		Dta1xxTxCtrlRegSetTxCtrl(pCh->m_pRegBase, 0);		// Transmit Control := Idle
		Dta1xxTxCtrlRegSetTxUflIntEn(pCh->m_pRegBase, 0);	// Clear interrupt enables
		Dta1xxTxCtrlRegSetTxSyncIntEn(pCh->m_pRegBase,0);	//   ,,     ,,       ,,
		Dta1xxTxCtrlRegSetTxAsiInv(pCh->m_pRegBase, 0);		// Disable ASI inversion
		Dta1xxTxCtrlRegSetSdiMode(pCh->m_pRegBase, 0);		// Disable SDI mode
		Dta1xxTxSetFifoSizeReg(pCh->m_pRegBase, 8388608);	// Maximum FIFO size
		Dta1xxTxSetTxClock(pCh->m_pRegBase, 0);				// Clock to 0 bps
		Dta1xxTxDiagRegSetLoopBack(pCh->m_pRegBase, 0);		// Disable loopback
		if ((pCh->m_Capability & DTA1XX_CHCAP_BIDIR_PORT) == DTA1XX_CHCAP_BIDIR_PORT)
		{
			// Enable output
			Dta1xxTxCtrlRegSetOutputEn(pCh->m_pRegBase, 1);
		}
		// Double-buffered capable ports
		if (  (pCh->m_Capability & DTA1XX_CHCAP_DBLBUF)!=0
			&& pCh->m_IoConfig!=DTA1XX_IOCONFIG_LOOPTHR )
		{
			// Enable/Disable double-buffered mode
			if ( pCh->m_IoConfig==DTA1XX_IOCONFIG_DBLBUF )
			{
				// Route output to buddy (as indicated by m_IoConfigPar)
				if ( pFdo->m_TypeNumber==105 && pFdo->m_FirmwareVersion==0 )
					Dta1xxGenDblBufEnableSet(pFdo->m_pGenRegs, 1); 
				else
					Dta1xxTxCtrl2RegSetSrcSel(pCh->m_pRegBase, pCh->m_IoConfigPar);
			}
			else
			{
				// Route output to ourselves
				if ( pFdo->m_TypeNumber==105 && pFdo->m_FirmwareVersion==0 )
					Dta1xxGenDblBufEnableSet(pFdo->m_pGenRegs, 0); 
				else
					Dta1xxTxCtrl2RegSetSrcSel(pCh->m_pRegBase, pCh->m_PortIndex);
			}
		}
		// Loop-through capable ports
		else if (  (pCh->m_Capability & DTA1XX_CHCAP_LOOPTHR)!=0
			     && pCh->m_IoConfig!=DTA1XX_IOCONFIG_DBLBUF )
		{
			// Enable/Disable loop-through mode
			if ( pCh->m_IoConfig==DTA1XX_IOCONFIG_LOOPTHR )
				// Route output to buddy (as indicated by m_IoConfigPar)
				Dta1xxTxCtrl2RegSetSrcSel(pCh->m_pRegBase, pCh->m_IoConfigPar);
			else
				// Route output to ourselves
				Dta1xxTxCtrl2RegSetSrcSel(pCh->m_pRegBase, pCh->m_PortIndex);
		}
		// No double-buffered or loop-through capbility; route output to ourselves
		else
		{
			if ( pFdo->m_TypeNumber!=105 || pFdo->m_FirmwareVersion!=0 )
				Dta1xxTxCtrl2RegSetSrcSel(pCh->m_pRegBase, pCh->m_PortIndex);
		}
	}
	else
	{
		// ASI/SDI Rx
		Dta1xxRxReset(pCh, DTA1XX_RST_CHANNEL);
		Dta1xxRxCtrlRegSetRxMode(pCh->m_pRegBase, 0);		// Receive Mode := St188
		Dta1xxRxCtrlRegSetRxCtrl(pCh->m_pRegBase, 0);		// Receive Control := Idle
		Dta1xxRxCtrlRegSetOvfIntEn(pCh->m_pRegBase, 0);		// Disable overflow int en.
		Dta1xxRxCtrlRegSetSyncIntEn(pCh->m_pRegBase, 0);	// Disable sync int. enable
		if (pFdo->m_TypeNumber==2135 || pFdo->m_TypeNumber==2136 || pFdo->m_TypeNumber==2137)
			Dta1xxRxCtrlRegSetAsiInv(pCh->m_pRegBase, 2);	// Set ASI invert to 'normal'
		else
			Dta1xxRxCtrlRegSetAsiInv(pCh->m_pRegBase, 0);	// Auto detect ASI inversion
		Dta1xxRxCtrlRegSetTimeStamping(pCh->m_pRegBase, 0);	// No timestamp inserting
		Dta1xxRxCtrlRegSetLedControl(pCh->m_pRegBase, 0);	// Disable LED overrule
		Dta1xxRxCtrlRegSetSdiMode(pCh->m_pRegBase, 0);		// Disable SDI mode

		if ((pCh->m_Capability & DTA1XX_CHCAP_BIDIR_PORT) == DTA1XX_CHCAP_BIDIR_PORT)
		{
			// Disable output enable for the Tx channel, first compute position of
			// tx-registers
			pTxRegs = ((UInt8*)pCh->m_pRegBase) - pCh->m_AddrRxRegMapOffset
												+ pCh->m_AddrTxRegMapOffset;
			Dta1xxTxCtrlRegSetOutputEn(pTxRegs, 0);
		}
	}
	return;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxAsiGetIoClksel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dta1xxAsiGetIoClksel(
	Channel*  pChannel,
	Int*  pIoConfig,
	Int*  pIoConfigPar)
{
	*pIoConfig = DTA1XX_IOCLKSEL_INTCLK;
	*pIoConfigPar = 0;
	return 0;
}



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxAsiSetIoClksel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dta1xxAsiSetIoClksel(
	Channel*  pChannel,
	Int  IoConfig,
	Int  IoConfigPar)
{
	if (IoConfig==DTA1XX_IOCLKSEL_INTCLK)
		return 0;

	return -EINVAL;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxGetIoConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Get configuration of an I/O port.
//
Int  Dta1xxGetIoConfig(
	DTA1XX_FDO* pFdo,	// Functional device object, representing the DTA-1xx card
	DTA1XX_IOCONFIG*  pIoConfig)	// IO Configuration Info
{
	Int Capability, IoConfig;
	Channel* pCh;
	if ( pIoConfig->m_PortIndex<0 || pIoConfig->m_PortIndex>=pFdo->m_NumChannels )
		return -EFAULT;

	pCh = &pFdo->m_Channel[pIoConfig->m_PortIndex];

	IoConfig = pCh->m_IoConfig;
	Capability = pCh->m_Capability;

	// Get extra parameter
	pIoConfig->m_ParXtra = pCh->m_IoConfigPar;

	if (pCh->m_ChannelType == DTA1XX_TS_TX_CHANNEL)
		pIoConfig->m_IoConfig = DTA1XX_IOCONFIG_OUTPUT;
	else if (pCh->m_ChannelType == DTA1XX_TS_RX_CHANNEL)
		pIoConfig->m_IoConfig = DTA1XX_IOCONFIG_INPUT;
	else if (pCh->m_ChannelType == DTA1XX_IP_RX_CHANNEL)	// Port index 4 (DTA-160/2160)
		pIoConfig->m_IoConfig = DTA1XX_IOCONFIG_IP;
	else
	{
		pIoConfig->m_IoConfig = DTA1XX_IOCONFIG_NOTSUP;
		return -EFAULT;
	}
	
	if ((Capability&DTA1XX_CHCAP_DBLBUF) && IoConfig==DTA1XX_IOCONFIG_DBLBUF)
		pIoConfig->m_IoConfig = DTA1XX_IOCONFIG_DBLBUF;
	else if ((Capability&DTA1XX_CHCAP_LOOPTHR) && IoConfig==DTA1XX_IOCONFIG_LOOPTHR)
		pIoConfig->m_IoConfig = DTA1XX_IOCONFIG_LOOPTHR;
	else if ((Capability&DTA1XX_CHCAP_FAILSAFE) && IoConfig==DTA1XX_IOCONFIG_FAILSAFE)
		pIoConfig->m_IoConfig = DTA1XX_IOCONFIG_FAILSAFE;
	else if ((Capability&DTA1XX_CHCAP_DIVERSITY) && IoConfig==DTA1XX_IOCONFIG_DIVERSITY)
		pIoConfig->m_IoConfig = DTA1XX_IOCONFIG_DIVERSITY;
	else if ((Capability&DTA1XX_CHCAP_SHARED) && IoConfig==DTA1XX_IOCONFIG_SHARED)
		pIoConfig->m_IoConfig = DTA1XX_IOCONFIG_SHARED;
	else if ((Capability&DTA1XX_CHCAP_GENREF) && IoConfig==DTA1XX_IOCONFIG_GENREF)
		pIoConfig->m_IoConfig = DTA1XX_IOCONFIG_GENREF;
	else if ((Capability&DTA1XX_CHCAP_GENLOCKED) && IoConfig==DTA1XX_IOCONFIG_GENLOCKED)
		pIoConfig->m_IoConfig = DTA1XX_IOCONFIG_GENLOCKED;
	else if ((Capability&DTA1XX_CHCAP_APSK) && IoConfig==DTA1XX_IOCONFIG_INPUT_APSK)
		pIoConfig->m_IoConfig = DTA1XX_IOCONFIG_INPUT_APSK;
	else if ((Capability&DTA1XX_CHCAP_APSK) && IoConfig==DTA1XX_IOCONFIG_DISABLED)
		pIoConfig->m_IoConfig = DTA1XX_IOCONFIG_DISABLED;
		
#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxGetIoConfig: IoConfig=%d",
				pIoConfig->m_PortIndex, pIoConfig->m_IoConfig );
#endif

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxGetIoConfig2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Get configurations of an I/O port.
//
Int  Dta1xxGetIoConfig2(
	DTA1XX_FDO* pFdo,	// Functional device object, representing the DTA-1xx card
	DTA1XX_IOCONFIG2*  pIoConfig)		// IO Configuration Info
{
	Int  Status=0, Capability;
	Channel*  pChannel;

#if LOG_LEVEL > 0
	DTA1XX_LOG(KERN_INFO,"Dta1xxGetIoConfig2: PortIndex %d, ConfigCode %d",
			   (int)pIoConfig->m_PortIndex, (int)pIoConfig->m_ConfigCode);
#endif

	// Default: return zero
	pIoConfig->m_IoConfig = 0;
	pIoConfig->m_ParXtra = 0;

	// Check port index
	if (pIoConfig->m_PortIndex<0 || pIoConfig->m_PortIndex>=pFdo->m_NumChannels)
		return -EFAULT;

	// Get channel pointer, capabilities
	pChannel = &pFdo->m_Channel[pIoConfig->m_PortIndex];
	Capability = pChannel->m_Capability;

	// Dispatch on configuration code
	Status = -EFAULT;
	switch (pIoConfig->m_ConfigCode) {

	case DTA1XX_IOCONFIG_IOCLKSEL:
		if (Capability&DTA1XX_CHCAP_SPI)
			Status = Dta1xxSpiGetIoClksel(pChannel, &pIoConfig->m_IoConfig);
		else
			Status = Dta1xxAsiGetIoClksel(pChannel, &pIoConfig->m_IoConfig, &pIoConfig->m_ParXtra);
		break;

	case DTA1XX_IOCONFIG_IOCONFIG:
	{
		// Convert data and call legacy code
		DTA1XX_IOCONFIG ioConfig;
		ioConfig.m_PortIndex = pIoConfig->m_PortIndex;
		Status = Dta1xxGetIoConfig(pFdo,&ioConfig);
		pIoConfig->m_IoConfig = ioConfig.m_IoConfig;
		pIoConfig->m_ParXtra = ioConfig.m_ParXtra;
		break;
	}

	case DTA1XX_IOCONFIG_IOMODE:
		// DVB-SPI/BT.656 ports have the capability
		if (Capability&DTA1XX_CHCAP_SPI)
			Status = Dta1xxSpiGetIoMode(pChannel, &pIoConfig->m_IoConfig, 
										&pIoConfig->m_ParXtra);
		break;

	case DTA1XX_IOCONFIG_IORATESEL:
		// if channel has rate selection capability
		if (pChannel->m_Capability&DTA1XX_CHCAP_RATESEL)
			Status = Dta1xxGetTxRateSel(pChannel, &pIoConfig->m_IoConfig);
		break;

	case DTA1XX_IOCONFIG_IOSTD:
		// DVB-SPI/BT.656 ports have the capability
		if (Capability&DTA1XX_CHCAP_SPI)
			Status = Dta1xxSpiGetIoStd(pChannel, &pIoConfig->m_IoConfig);
		break;

	default:
		Status = STATUS_INVALID_PARAMETER;
		break;
	}
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSetIoConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set the configuration of an I/O port.
//
Int  Dta1xxSetIoConfig(
	DTA1XX_FDO* pFdo,	// Functional device object, representing the DTA-1xx card
	DTA1XX_IOCONFIG*  pIoConfig,	// IO Configuration Info
	BOOLEAN Init)		// TRUE: Function used during initialisation
{
	Int  i, Dummy, TypeNumber, Capability, IoConfigTemp=0;
	Channel  *pCh=NULL, *pBuddyCh=NULL, *pChTemp=NULL;
	BOOLEAN NoError=FALSE, IsOutput, DisableGenlock=FALSE;
	volatile UInt8*  pBase = pFdo->m_Dta1xxRegsBase.m_pKernel;
	TypeNumber = pFdo->m_TypeNumber;
	
	// For now, only non-IP Channels supported
	if ( pIoConfig->m_PortIndex >= pFdo->m_NumNonIpChannels )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: INVALID PORT NUMBER "
					"NumNonIpChannels=%d", pIoConfig->m_PortIndex,
					pFdo->m_NumNonIpChannels );
#endif
		return -EFAULT;
	}

	// Get channel pointer
	pCh = &pFdo->m_Channel[pIoConfig->m_PortIndex];
	// Get capabilty
	Capability = pCh->m_Capability;

	// Only supported on bi-directional ports, DblBufferd, Loop-through, failsafe,
	// diversity, shared antenna or APSK ports
	if ( ((pCh->m_Capability & DTA1XX_CHCAP_BIDIR_PORT) != DTA1XX_CHCAP_BIDIR_PORT) &&
		 ((pCh->m_Capability & DTA1XX_CHCAP_DBLBUF) == 0) &&
		 ((pCh->m_Capability & DTA1XX_CHCAP_FAILSAFE) == 0) &&
		 ((pCh->m_Capability & DTA1XX_CHCAP_DIVERSITY) == 0) &&
		 ((pCh->m_Capability & DTA1XX_CHCAP_SHARED) == 0) &&
		 ((pCh->m_Capability & DTA1XX_CHCAP_LOOPTHR) == 0) &&
		 ((pCh->m_Capability & DTA1XX_CHCAP_GENREF) == 0) &&
		 ((pCh->m_Capability & DTA1XX_CHCAP_GENLOCKED) == 0) &&
		 ((pCh->m_Capability & DTA1XX_CHCAP_APSK) == 0) 
	) {
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: CAPABILITY NOT SUPPORTED",
				     pIoConfig->m_PortIndex );
#endif
		return -EFAULT;
	}

	// Check IoConfig parameters
	switch ( pIoConfig->m_IoConfig )
	{
	case DTA1XX_IOCONFIG_OUTPUT:     
		NoError =    ((Capability & DTA1XX_CHCAP_OUT_PORT)==DTA1XX_CHCAP_OUT_PORT)
				  || ((Capability & DTA1XX_CHCAP_BIDIR_PORT)==DTA1XX_CHCAP_BIDIR_PORT);
		break;

	case DTA1XX_IOCONFIG_INPUT:
		NoError =    ((pCh->m_Capability & DTA1XX_CHCAP_IN_PORT)	== DTA1XX_CHCAP_IN_PORT)
				  || ((pCh->m_Capability & DTA1XX_CHCAP_BIDIR_PORT)	== DTA1XX_CHCAP_BIDIR_PORT)
				  || ((pCh->m_Capability & DTA1XX_CHCAP_SHARED)		!= 0)
				  || ((pCh->m_Capability & DTA1XX_CHCAP_APSK)		!= 0)
				  || ((pCh->m_Capability & DTA1XX_CHCAP_DIVERSITY)	!= 0);

		// Perform some additional check when APSK-mode is supported on this port
		if ((pCh->m_Capability & DTA1XX_CHCAP_APSK) != 0)
		{
			// Look up other channel
			pChTemp = (pIoConfig->m_PortIndex == 0) ? &(pFdo->m_Channel[1]) : &(pFdo->m_Channel[0]);

			// Check current IOCONFIG for other channel is not set to IOCONFIG_INPUT_APSK
			if (pChTemp->m_IoConfig == DTA1XX_IOCONFIG_INPUT_APSK) 
			{
				
#if LOG_LEVEL > 1
				DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: OTHER CHANNEL MUST NOT BE CONFIGURED "
							"FOR APSK MODE", pIoConfig->m_PortIndex);
#endif
				NoError = FALSE;	
			}
		}

		break;

	case DTA1XX_IOCONFIG_FAILSAFE:
		NoError = 0!=(Capability & DTA1XX_CHCAP_FAILSAFE);
		break;

	case DTA1XX_IOCONFIG_DBLBUF:

		// First check double-buffered-mode is supported on this port
		NoError = 0!=(Capability & DTA1XX_CHCAP_DBLBUF);

		// Buddy-port should be set as output 

		// Special case: for DTA-105/145/2145 the buddy port is always port 1 and for legacy
		// reasons the buddy does not need to be specified in m_ParXtra
		if ( pIoConfig->m_ParXtra==-1 )
		{
			if ( TypeNumber==105 || TypeNumber==145 || TypeNumber==2145 )
				pIoConfig->m_ParXtra = 0; // Set to default value (port index 0)
		}
		if (   pIoConfig->m_ParXtra<0 || pIoConfig->m_ParXtra>=pFdo->m_NumNonIpChannels
			|| pIoConfig->m_ParXtra==pCh->m_PortIndex )
		{
			NoError = FALSE;	// Invalid buddy-port specified

#if LOG_LEVEL > 1
			DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: INVALID BUDDY-PORT", 
						pIoConfig->m_PortIndex );
#endif
		}
		// Now check buddy is configured as output
		pBuddyCh = &(pFdo->m_Channel[pIoConfig->m_ParXtra]);
		if ( pBuddyCh->m_ChannelType != DTA1XX_TS_TX_CHANNEL )
		{
			NoError = FALSE;	// Buddy is not an output
#if LOG_LEVEL > 1
			DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: BUDDY NOT AN OUTPUT", 
						pIoConfig->m_PortIndex );
#endif
		}
		break;

	case DTA1XX_IOCONFIG_LOOPTHR:
				
		// First check loop-through-mode is supported on this port
		NoError = 0!=(Capability & DTA1XX_CHCAP_LOOPTHR);

		// Buddy-port should be set as input 

		// Special case: for DTA-105/145/2145 the buddy port is always port 1 and for legacy
		// reasons the buddy does not need to be specified in m_ParXtra
		if ( pIoConfig->m_ParXtra==-1 )
		{
			if ( TypeNumber==105 || TypeNumber==145 || TypeNumber==2145 )
				pIoConfig->m_ParXtra = 0; // Set to default value (port index 0)
		}
		if (   pIoConfig->m_ParXtra<0 || pIoConfig->m_ParXtra>pFdo->m_NumNonIpChannels
			|| pIoConfig->m_ParXtra==pCh->m_PortIndex )
		{
			NoError = FALSE;	// Invalid buddy-port specified

#if LOG_LEVEL > 1
			DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: INVALID BUDDY-PORT", 
						pIoConfig->m_PortIndex );
#endif
		}
		// Now check buddy is configured as output
		pBuddyCh = &(pFdo->m_Channel[pIoConfig->m_ParXtra]);
		if ( pBuddyCh->m_ChannelType != DTA1XX_TS_RX_CHANNEL )
		{
			NoError = FALSE;	// Buddy is not an input
#if LOG_LEVEL > 1
			DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: BUDDY NOT AN INPUT", 
						pIoConfig->m_PortIndex );
#endif
		}
		break;

	case DTA1XX_IOCONFIG_DIVERSITY:

		// First check DIVERSITY-mode is supported on this port
		NoError = 0!=(Capability & DTA1XX_CHCAP_DIVERSITY);

		// Buddy-port administration (for future use)
		// Special case: for DTA-2135 the buddy port is always port 2 (i.e. idx=1) and for
		// ease of use the buddy does not need to be specified in m_ParXtra
		if ( pIoConfig->m_ParXtra==-1 )
		{
			if ( TypeNumber==2135 )
				pIoConfig->m_ParXtra = 1; // Set to default value (port index 1)
		}

		// Check Buddy-port
		if (   pIoConfig->m_ParXtra<0 || pIoConfig->m_ParXtra>pFdo->m_NumNonIpChannels
			|| pIoConfig->m_ParXtra==pCh->m_PortIndex )
		{
			NoError = FALSE;	// Invalid buddy-port specified

#if LOG_LEVEL > 1
			DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: INVALID BUDDY-PORT", 
						pIoConfig->m_PortIndex );
#endif
		}	
		// FOR NOW: no further checks needed on the buddy port
		break;

	case DTA1XX_IOCONFIG_SHARED:

		// First check SHARED-mode is supported on this port
		NoError = ((Capability & DTA1XX_CHCAP_SHARED) != 0);
		
		// Buddy-port administration (for future use)
		// Special case: for DTA-2135/2136/2137 the buddy port is always port 1 (i.e. idx=0) and
		// for ease of use the buddy does not need to be specified in m_ParXtra
		if (pIoConfig->m_ParXtra == -1)
		{
			if ( TypeNumber==2135 || TypeNumber==2136 || TypeNumber==2137 )
				pIoConfig->m_ParXtra = 0; // Set to default value (port index 0)
		}

		// Check Buddy-port
		if (   pIoConfig->m_ParXtra<0 || pIoConfig->m_ParXtra>pFdo->m_NumNonIpChannels
			|| pIoConfig->m_ParXtra==pCh->m_PortIndex )
		{
			NoError = FALSE;	// Invalid buddy-port specified

#if LOG_LEVEL > 1
			DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: INVALID BUDDY-PORT", 
						pIoConfig->m_PortIndex );
#endif
		}		

		// Now check buddy is not configured in DIVERSITY mode
		pBuddyCh = &(pFdo->m_Channel[pIoConfig->m_ParXtra]);
		
		if ( pBuddyCh->m_IoConfig != DTA1XX_IOCONFIG_INPUT)		// check current IOCONFIG for buddy channel
		{
			NoError = FALSE;	// Buddy is not an input
#if LOG_LEVEL > 1
			DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: BUDDY NOT AN INPUT", 
						pIoConfig->m_PortIndex );
#endif
		}
		break;

	case DTA1XX_IOCONFIG_INPUT_APSK:
		// First check APSK-mode is supported on this port
		NoError = ((pCh->m_Capability & DTA1XX_CHCAP_APSK) != 0);

		if (pIoConfig->m_ParXtra != -1) {
			NoError = FALSE;
#if LOG_LEVEL > 1
			DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: INVALID PARXTRA FOR APSK IOCONFIG",
					    pIoConfig->m_PortIndex);
#endif
		}

		// Look up other channel
		pChTemp = (pIoConfig->m_PortIndex == 0) ? &(pFdo->m_Channel[0]) : &(pFdo->m_Channel[1]);

		// Check current IOCONFIG for other channel is not set to IOCONFIG_INPUT_APSK
		if (pChTemp->m_IoConfig == DTA1XX_IOCONFIG_INPUT_APSK) {
			
#if LOG_LEVEL > 1
			DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: OTHER CHANNEL MUST NOT BE "
						"CONFIGURED FOR APSK MODE", pIoConfig->m_PortIndex);
#endif
			NoError = FALSE;	
			break;
		}
		break;

	case DTA1XX_IOCONFIG_DISABLED:
		// Ports can only be disabled when the port(s) support APSK
		// So first check APSK-mode is supported on this port
		NoError = ((pCh->m_Capability & DTA1XX_CHCAP_APSK) != 0);

		break;

	case DTA1XX_IOCONFIG_GENREF:
		// Check for capability
		NoError = ((pCh->m_Capability & DTA1XX_CHCAP_GENREF) == DTA1XX_CHCAP_GENREF);

		// Check ParXtra
		if ((pIoConfig->m_ParXtra != DTA1XX_GENLOCK_SDI625) &&
			(pIoConfig->m_ParXtra != DTA1XX_GENLOCK_SDI525))
		{
			
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO,
						"[%d] Dta1xxSetIoConfig: INCORRECT/MISSING GENLOCK VIDEO STANDARD",
						pIoConfig->m_PortIndex);
#endif

			NoError = FALSE;	// Invalid video standard specified
			break;
		}

		// Strict configuration rules:
		// Prevent any IOCONFIG_GENREF changes when any port are configured in DTA1XX_IOCONFIG_GENLOCKED mode
		
		for ( i=0; i<pFdo->m_NumNonIpChannels; i++ )
		{
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: Checking IOCONFIG port %d for "
						"DTA1XX_IOCONFIG_GENLOCKED", pIoConfig->m_PortIndex, i);
#endif

			// Lookup IOConfig of channel i
			pChTemp = &pFdo->m_Channel[i];
			IoConfigTemp = pChTemp->m_IoConfig;
			if ( IoConfigTemp == DTA1XX_IOCONFIG_GENLOCKED )
			{
#if LOG_LEVEL > 0
				DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: IOCONFIG_GENLOCKED must "
					        "be disabled first for port %d", pIoConfig->m_PortIndex, i+1);
#endif
				NoError = FALSE;	// Can't continue
				break;
			}
		}	
		break;

	case DTA1XX_IOCONFIG_GENLOCKED:

		// Check for capability
		NoError = ((pCh->m_Capability & DTA1XX_CHCAP_GENLOCKED) == DTA1XX_CHCAP_GENLOCKED);

		// Note: in the current GENLOCK implementation an Genlocked TX channel
		//		 videomode is linked to the GENLOCK REFERENCE input video mode,
		//		 hence it can only be configured when the channel 0 is in IOCONFIG_GENREF mode
		//		 Note: the videostandard itself is checked when within the setTxMode function

		//		 Revise this when crossboard Genlock is implemented

		// Lookup IOConfig of channel 0
		pChTemp = &pFdo->m_Channel[0];
		IoConfigTemp = pChTemp->m_IoConfig;

		if (IoConfigTemp != DTA1XX_IOCONFIG_GENREF)
		{
#if LOG_LEVEL > 0
			DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: IOCONFIG_GENREF must be set"
						"first", pIoConfig->m_PortIndex);
#endif
			NoError = FALSE;	// Genlock RX must be enabled first
			break;
		}

		// Check Videostandard matches configured GENREF standard

		break;

	default:
		NoError = FALSE;
		break;
	}

	if ( !NoError )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( 
			KERN_INFO,
			"[%d] Dta1xxSetIoConfig: IoConfig=%d, ParXtra=%d INVALID CONFIGURATION",
			pIoConfig->m_PortIndex, pIoConfig->m_IoConfig, pIoConfig->m_ParXtra );
#endif
		return -EFAULT;
	}

	// No one can attach to this channel during the update
	if ( 0!=down_interruptible(&pCh->m_ExclAccLock) )
		return -EFAULT;

	if ( pCh->m_ExclAccess )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: CHANNEL IS NOT DETACHED",
					pIoConfig->m_PortIndex );
#endif
		up(&pCh->m_ExclAccLock);
		return -EFAULT;
	}

#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: IoConfig=%d",
				pIoConfig->m_PortIndex, pIoConfig->m_IoConfig );
#endif

	// It's possible that the Isr or the IsrDpc is using the channel during the channel
	// updating below. This is not a problem anymore because the channel pointers are
	// not changed. It's possible that the readback of the code violation counter or
	// valid counter is not correct during this update.


	// Apply new configuration

	if (DisableGenlock)
		Dta1xxDisableGenlock(pFdo);

	switch ( pIoConfig->m_IoConfig )
	{
	case DTA1XX_IOCONFIG_OUTPUT:

		// Route output to ourselves
		if ( pFdo->m_TypeNumber==105 && pFdo->m_FirmwareVersion==0 )
			Dta1xxGenDblBufEnableSet(pFdo->m_pGenRegs, 0);
		else
			Dta1xxTxCtrl2RegSetSrcSel(pCh->m_pRegBase, pCh->m_PortIndex);

		break;

	case DTA1XX_IOCONFIG_INPUT:
		// FOR DTA-2135 port 1: disable diversity mode routing
		if ( pFdo->m_TypeNumber==2135 && pIoConfig->m_PortIndex==0 )
			Dta1xxGenDiversitySet(pFdo->m_pGenRegs, 0);
		break;

	case DTA1XX_IOCONFIG_FAILSAFE:
		// Route output to ourselves
		if ( pFdo->m_TypeNumber==105 && pFdo->m_FirmwareVersion==0 )
			Dta1xxGenDblBufEnableSet(pFdo->m_pGenRegs, 0);
		else
			Dta1xxTxCtrl2RegSetSrcSel(pCh->m_pRegBase, pCh->m_PortIndex);

		// Reset timeout value to default 200ms
		pCh->m_FailsafeTimeout = 10;
		pCh->m_FailsafeTimeoutCnt = (UInt32)(200000 /
									Dta1xxGetPeriodicIntIntervalMicroS(pFdo));
		break;

	case DTA1XX_IOCONFIG_DBLBUF:
		// Route output to double-buffered-buddy
		if ( pFdo->m_TypeNumber==105 && pFdo->m_FirmwareVersion==0 )
			Dta1xxGenDblBufEnableSet(pFdo->m_pGenRegs, 1);
		else
			Dta1xxTxCtrl2RegSetSrcSel(pCh->m_pRegBase, pBuddyCh->m_PortIndex);

#if LOG_LEVEL > 0
		DTA1XX_LOG(
			KERN_INFO, "[%d] Dta1xxSetIoConfig: DBLBUF is %s, buddy=%d",
			pIoConfig->m_PortIndex,
			(pIoConfig->m_IoConfig == DTA1XX_IOCONFIG_DBLBUF ? "enabled" : "disabled"),
			pIoConfig->m_ParXtra);
#endif
		break;

	case DTA1XX_IOCONFIG_LOOPTHR:
		// Route output to loop-through-buddy
		Dta1xxTxCtrl2RegSetSrcSel(pCh->m_pRegBase, pBuddyCh->m_PortIndex);

#if LOG_LEVEL > 0
		DTA1XX_LOG(
			KERN_INFO, "[%d] Dta1xxSetIoConfig: LOOPTHR is %s, buddy=%d",
			pIoConfig->m_PortIndex,
			(pIoConfig->m_IoConfig == DTA1XX_IOCONFIG_LOOPTHR ? "enabled" : "disabled"),
			pIoConfig->m_ParXtra);
#endif
		break;

	case DTA1XX_IOCONFIG_DIVERSITY:

		// FOR DTA-2135: enable diversity mode routing
		if ( pFdo->m_TypeNumber==2135 )
			Dta1xxGenDiversitySet(pFdo->m_pGenRegs, 1);

#if LOG_LEVEL > 0
		DTA1XX_LOG(
			KERN_INFO, "[%d] Dta1xxSetIoConfig: DIVERISTY-MODE is enabled, buddy=%d",
			pIoConfig->m_PortIndex, pIoConfig->m_ParXtra);
#endif
		break;

	case DTA1XX_IOCONFIG_SHARED:

#if LOG_LEVEL > 0
		DTA1XX_LOG(
			KERN_INFO, "[%d] Dta1xxSetIoConfig: SHARED-MODE is enabled, buddy=%d",
			pIoConfig->m_PortIndex, pIoConfig->m_ParXtra);
#endif
		break;

	case DTA1XX_IOCONFIG_INPUT_APSK:
		// FOR NOW: NOTHING TO DO
		// The actual initialisation of single or dual mode is done in the DtapiService
		break;

	case DTA1XX_IOCONFIG_GENREF:
		// FOR NOW: NOTHING TO DO
		break;

	case DTA1XX_IOCONFIG_GENLOCKED:
		// Route output to ourselves
		if (pFdo->m_TypeNumber==105 && pFdo->m_FirmwareVersion==0)
			Dta1xxGenDblBufEnableSet(pFdo->m_pGenRegs, 0);
		else
			Dta1xxTxCtrl2RegSetSrcSel(pCh->m_pRegBase, pCh->m_PortIndex);
		break;

	default:
		break;
	}

	// Check the direction of the channel and change I/O if not correct
	IsOutput = (pIoConfig->m_IoConfig == DTA1XX_IOCONFIG_DBLBUF) ||
			   (pIoConfig->m_IoConfig == DTA1XX_IOCONFIG_FAILSAFE) ||
			   (pIoConfig->m_IoConfig == DTA1XX_IOCONFIG_OUTPUT) ||
			   (pIoConfig->m_IoConfig == DTA1XX_IOCONFIG_LOOPTHR) ||
			   (pIoConfig->m_IoConfig == DTA1XX_IOCONFIG_GENLOCKED);

	// IMPORTANT: Store current IoConfig, 
	// before calling Dta1xxInitAsiSdi or Dta1xxInitGenlock		
	pCh->m_IoConfig = pIoConfig->m_IoConfig;
	pCh->m_IoConfigPar = pIoConfig->m_ParXtra;

	// Check genlock needs to be initialised
	if (pIoConfig->m_IoConfig == DTA1XX_IOCONFIG_GENREF) 
		// Always Initialise Genlock when IOCONFIG_GENREF is set
		Dta1xxInitGenlock(pFdo);

	if ( IsOutput && (pCh->m_ChannelType != DTA1XX_TS_TX_CHANNEL) )
	{
		// Change Rx to Tx
		pCh->m_ChannelType = DTA1XX_TS_TX_CHANNEL;

		// Compute offset to channels tx-register map
		pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
									   + pCh->m_AddrTxRegMapOffset);
		pCh->m_Dma.m_DmaDir = DTA1XX_DMA_WRITE;

		Dta1xxInitAsiSdi(pCh);

		// Initialise synchronisation variables
		init_MUTEX(&pCh->m_MutexTxControl);

		// Initialise Transport-Stream rate shadow variable
		Dta1xxTxGetRate2(pFdo, pCh->m_PortIndex, &Dummy, &pCh->m_TsSymOrSampRate);
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: Initial TsSymOrSampRate=%d",
					pCh->m_PortIndex, pCh->m_TsSymOrSampRate);
#endif
	}
	else if ( !IsOutput && (pCh->m_ChannelType != DTA1XX_TS_RX_CHANNEL) )
	{
		// Change Tx to Rx
		pCh->m_ChannelType = DTA1XX_TS_RX_CHANNEL;
		// Compute offset to channels rx-register map
		pCh->m_pRegBase = (pBase + pCh->m_AddrBaseOffset
									   + pCh->m_AddrRxRegMapOffset);
		pCh->m_Dma.m_DmaDir = DTA1XX_DMA_READ;

		Dta1xxInitAsiSdi(pCh);

	}
	else
	{
		// No change needed
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetIoConfig: No I/O change",
				    pIoConfig->m_PortIndex );
#endif
	}

	// The Vpd cache is located in paged pool, so the lock must be released
	// before executing the registry functions
	up(&pCh->m_ExclAccLock);
	if (!Init) {
		// Update registry
//		Dta1xxSetRegistryValueFromChannel(pFdo, pCh, L"IoConfig",
//										  pIoConfig->m_IoConfig);
	}
	return 0;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSetIoConfig2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Set a configuration of an I/O port.
//
Int  Dta1xxSetIoConfig2(
	DTA1XX_FDO* pFdo,	// Functional device object, representing the DTA-1xx card
	DTA1XX_IOCONFIG2*  pIoConfig,	// IO Configuration Info
	BOOLEAN Init)		// TRUE: Function used during initialisation
{
	Int Capability, Status=0;
	Channel* pChannel;
	char* pIoConfigName;
	char* pParXtraName;

#if LOG_LEVEL > 0
	DTA1XX_LOG(KERN_INFO,"Dta1xxSetIoConfig2: PortIndex %d, ConfigCode %d, IoConfig %d, ParXtra %d, Init %d",
			  (int)pIoConfig->m_PortIndex, (int)pIoConfig->m_ConfigCode,
			  (int)pIoConfig->m_IoConfig, (int)pIoConfig->m_ParXtra, (int)Init);
#endif

	// Check port index
	if (pIoConfig->m_PortIndex<0 || pIoConfig->m_PortIndex>=pFdo->m_NumChannels)
		return -EFAULT;

	// Get channel pointer, capabilities
	pChannel = &pFdo->m_Channel[pIoConfig->m_PortIndex];
	Capability = pChannel->m_Capability;

	// Dispatch on configuration code
	Status = -EFAULT;
	pIoConfigName = 0;
	pParXtraName = 0;
	switch (pIoConfig->m_ConfigCode) {

	case DTA1XX_IOCONFIG_IOCLKSEL:
		pIoConfigName = "IoClksel";
		pIoConfigName = "IoClkselPar";

		if (Capability&DTA1XX_CHCAP_SPI)
			Status = Dta1xxSpiSetIoClksel(pChannel, pIoConfig->m_IoConfig);
		else
			Status = Dta1xxAsiSetIoClksel(pChannel, pIoConfig->m_IoConfig,
										  pIoConfig->m_ParXtra);
		break;

	case DTA1XX_IOCONFIG_IOCONFIG: 
	{
		// convert data and call legacy code
		DTA1XX_IOCONFIG ioConfig;
		ioConfig.m_PortIndex = pIoConfig->m_PortIndex;
		ioConfig.m_IoConfig = pIoConfig->m_IoConfig;
		ioConfig.m_ParXtra = pIoConfig->m_ParXtra;
		Status = Dta1xxSetIoConfig(pFdo,&ioConfig,Init);
		break;
	}
	
	case DTA1XX_IOCONFIG_IOMODE:
		pIoConfigName = "IoMode";
		pParXtraName = "IoModePar";
		// DVB-SPI/BT.656 ports have the capability
		if (0!=(Capability&DTA1XX_CHCAP_SPI))
			Status = Dta1xxSpiSetIoMode(pChannel, pIoConfig->m_IoConfig,
										pIoConfig->m_ParXtra);
		break;

	case DTA1XX_IOCONFIG_IORATESEL:
		pIoConfigName = "IoRatesel";
		// if channel has rate selection capability
		if (pChannel->m_Capability&DTA1XX_CHCAP_RATESEL)
			Status = Dta1xxSetTxRateSel(pChannel, pIoConfig->m_IoConfig);
		break;

	case DTA1XX_IOCONFIG_IOSTD:
		pIoConfigName = "IoStd";
		// DVB-SPI/BT.656 ports have the capability
		if (Capability&DTA1XX_CHCAP_SPI)
			Status = Dta1xxSpiSetIoStd(pChannel, pIoConfig->m_IoConfig);
	break;

	default:
		Status = STATUS_INVALID_PARAMETER;
		break;
	}

	// if no error, not initializing
	if ( Status==0 && !Init)
	{
//		// if a name is given for the configuration parameter
//		if (pIoConfigName) {
//			// Then update registry
//			Dta1xxSetRegistryValueFromChannel(pFdo, pChannel, pIoConfigName,
//											  pIoConfig->m_IoConfig);
//		// if a name is given for the extra parameter
//		if (pParXtraName) {
//			// Then update registry
//			Dta1xxSetRegistryValueFromChannel(pFdo, pChannel, pParXtraName,
//											  pIoConfig->m_ParXtra);
//		}
	}

	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSetFailsafeState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Set the configuration of an I/O port.
//
Int  Dta1xxSetFailsafeState(
	DTA1XX_FDO* pFdo,	// Functional device object, representing the DTA-1xx card
	DTA1XX_FAILSAFE*  pFailsafe)			// Failsafe data
{
	Channel*  pCh;

	// Only non-IP Channels supported
	if ( pFailsafe->m_PortIndex > pFdo->m_NumNonIpChannels )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetFailsafe: INVALID PORT NUMBER "
					"NumNonIpChannels=%d",
					pFailsafe->m_PortIndex, pFdo->m_NumNonIpChannels);
#endif
		return -EFAULT;
	}

	// Get channel pointer
	pCh = &pFdo->m_Channel[pFailsafe->m_PortIndex];

	// Only supported on failsafe ports
	if ( (pCh->m_Capability & DTA1XX_CHCAP_FAILSAFE) == 0 )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetFailsafe: NOT SUPPORTED",
				    pFailsafe->m_PortIndex );
#endif
		return -EFAULT;
	}

	//
	if ( pCh->m_IoConfig != DTA1XX_IOCONFIG_FAILSAFE )
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxSetFailsafe: NOT IN FAILSAFE MODE",
					pFailsafe->m_PortIndex );
#endif
		return -EFAULT;
	}

	// Enable/Disable failsafe
	if (pFailsafe->m_Mode & 0x1)
		pCh->m_FailsafeEnable = (pFailsafe->m_Enable? TRUE : FALSE);

	if (pFailsafe->m_Mode & 0x2)
	{
		// Timeout
		pCh->m_FailsafeTimeout = pFailsafe->m_Timeout;

		// Convert to PerInt tick count
		pCh->m_FailsafeTimeoutCnt = (UInt32)Dta1xxBinDiv(
										(Int64)pFailsafe->m_Timeout*(Int64)20000,
										(Int64)Dta1xxGetPeriodicIntIntervalMicroS(pFdo),
										NULL);
	}

	// Alive, reset Failsafe counter
	if (pFailsafe->m_Mode & 0x4)
		pCh->m_FailsafeCnt = 0;

	return 0;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxSetFailsafeState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Set the configuration of an I/O port.
//
Int  Dta1xxGetFailsafeState(
	DTA1XX_FDO* pFdo,	// Functional device object, representing the DTA-1xx card
	DTA1XX_FAILSAFE*  pFailsafe)			// Failsafe data
{
	Channel*  pCh;

	// Only non-IP Channels supported
	if (pFailsafe->m_PortIndex > pFdo->m_NumNonIpChannels)
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxGetFailsafeState: INVALID PORT NUMBER "
					"NumNonIpChannels=%d",
					pFailsafe->m_PortIndex, pFdo->m_NumNonIpChannels );
#endif
		return -EFAULT;
	}

	// Get channel pointer
	pCh = &pFdo->m_Channel[pFailsafe->m_PortIndex];

	// Only supported on failsafe ports
	if ((pCh->m_Capability & DTA1XX_CHCAP_FAILSAFE) == 0)
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxGetFailsafeState: NOT SUPPORTED",
				    pFailsafe->m_PortIndex );
#endif
		return -EFAULT;
	}

	if (pCh->m_IoConfig != DTA1XX_IOCONFIG_FAILSAFE)
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxGetFailsafeState: NOT IN FAILSAFE MODE",
				    pFailsafe->m_PortIndex );
#endif
		return -EFAULT;
	}

	pFailsafe->m_Reserved = 0;
	pFailsafe->m_Mode = 0x3;
	pFailsafe->m_Enable = pCh->m_FailsafeEnable;
	pFailsafe->m_Timeout = pCh->m_FailsafeTimeout;

	// Check current alive state
	if (pCh->m_FailsafeCnt < pCh->m_FailsafeTimeoutCnt) {
		pFailsafe->m_Mode |= 0x4;

	}
#if LOG_LEVEL > 0
	DTA1XX_LOG( KERN_INFO, "[%d] Dta1xxGetFailsafeState: FailsafeTimeoutCnt=%d "
				"FailsafeCnt=%d", pFailsafe->m_PortIndex, (Int)pCh->m_FailsafeTimeoutCnt,
				(Int)pCh->m_FailsafeCnt );
#endif

	return 0;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dta1xxWaitUntilFpgaIsReady -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Wait until the FPGA is configured.
// If the FPGA is not responding within time, a warning message is written in the event log
//
Int  Dta1xxWaitUntilFpgaIsReady(
	DTA1XX_FDO* pFdo)	// Functional device object, representing the DTA-1xx card
{
	UInt32  Timeout = 0;
	volatile UInt8* pGenRegs = pFdo->m_pGenRegs;
	UInt8  TypeNumber;
	UInt8  TypeNumberExt;

	Dta1xxWaitMs(300);

	TypeNumber = Dta1xxGenCtrlRegGetTypeNumber(pGenRegs);
	TypeNumberExt = Dta1xxGenStatusRegGetTypeNumExt(pGenRegs);

	// Wait until type number is read correctly or timeout occurs
	while (   (TypeNumber != (pFdo->m_TypeNumber - (TypeNumberExt == 1 ?
												  2100 : (TypeNumberExt == 2 ? 500 : 0))))
		   && (Timeout < 1000)) {

		Dta1xxWaitMs(1);
		Timeout++;

		// Read typenumber again
		TypeNumber = Dta1xxGenCtrlRegGetTypeNumber(pGenRegs);
		TypeNumberExt = Dta1xxGenStatusRegGetTypeNumExt(pGenRegs);
	}

	if (Timeout >= 1000) 
	{
#if LOG_LEVEL > 0
		DTA1XX_LOG( KERN_INFO, "Dta1xxWaitUntilFpgaIsReady: FPGA DID NOT RESPOND WITHIN "
					"1000ms" );
#endif
		return -EFAULT;
	}
	return 0;
}
