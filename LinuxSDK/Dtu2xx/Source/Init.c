//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Init.c *#*#*#*#*#*#*#*#*#* (C) 2000-2010 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This program is free software; You can freely use/modify this source code in any way
// you desire, including for commercial applications.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// For a revision history of the Dtu2xx driver, please refer to the Dta1xx readme

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "Dtu2xx.h"
#include "Dtu2xxRegs.h"
#include "EzUsb.h"
#include "Cyclone205.h"
#include "Cyclone215.h"
#include "Cyclone225.h"
#include "Cyclone234.h"
#include "Cyclone235.h"
#include "Cyclone236.h"
#include "Cyclone245.h"
#include "Cyclone2x5.h"
#include "Cypress205.h"
#include "Cypress215.h"
#include "Cypress225.h"
#include "Cypress234.h"
#include "Cypress235.h"
#include "Cypress236.h"
#include "Cypress245.h"
#include "Cypress2x5.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Internal functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
Int  Dtu2xxInitPld(IN PDTU2XX_FDO);
Int  Dtu2xxInitPowerSupply(IN PDTU2XX_FDO);
Int  Dtu2xxInitChannelsGeneric(IN PDTU2XX_FDO);
Int  Dtu2xxInitDeviceExtension(IN PDTU2XX_FDO);
Int  Dtu2xxInitDeviceHardware(IN PDTU2XX_FDO);
Int  Dtu2xxInitVpd(IN PDTU2XX_FDO);
Int  Dtu2xxInitAsiSdiChannel(IN Channel*);
Int  Dtu2xxInitModChannel(IN Channel*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxInitDevice -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialises the DTU-2xx device
//
Int  Dtu2xxInitDevice(
	IN PDTU2XX_FDO pFdo)			// The functional device object
{
	Int Status=0;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxInitDevice: ENTER");
#endif

	// Generic initialisation of channel-related variables, this is:
	// No board-specific intialisations yet
	if ((Status=Dtu2xxInitChannelsGeneric(pFdo)) != 0)
		return Status;

	// Initialisation of device-extension variables
	if ((Status=Dtu2xxInitDeviceExtension(pFdo)) != 0)
		return Status;

	// Initialisation of power-supply
	if ((Status=Dtu2xxInitPowerSupply(pFdo)) != 0)
		return Status;

	// Initialisation of onboard PLD
	if ((Status=Dtu2xxInitPld(pFdo)) != 0)
		return Status;

	// Initialise VPD caching, read and interpret VPD configuration resources
	if ((Status=Dtu2xxInitVpd(pFdo)) != 0)
		return Status;

	// Initialise device hardware
	if ((Status=Dtu2xxInitDeviceHardware(pFdo)) != 0)
		return Status;

	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxInitPowerSupply -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Init onboard power supplies needed before uploading PLD firmware
//
Int  Dtu2xxInitPowerSupply(IN PDTU2XX_FDO pFdo)
{
#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxInitPowerSupply: Enabling power-supply DTU-%d",
			   pFdo->m_TypeNumber);
#endif

	// At the moment only the DTU-215 has any power supplies to enable
	if ( pFdo->m_TypeNumber == 215 )
		return Dtu2xxInitDtu215MainPowerSupply(pFdo);
	else
		return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxInitPld -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialise the onboard PLD, by uploading firmware
//
Int  Dtu2xxInitPld(IN PDTU2XX_FDO pFdo)
{
	Int Status = 0;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxInitPld: Start programming of DTU-%d PLD",
			   pFdo->m_TypeNumber);
#endif

	// Do we have DTU-2x5 hardware (ignore if we have to load the manuf code)??
	if ( pFdo->m_IsDtu2x5Hw && pFdo->m_TypeNumber!=299 )
		Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_FPGA | DTU2XX_FWID_DTU2x5);
	else
	{
		switch ( pFdo->m_TypeNumber )
		{
		case 205:
			Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_FPGA | DTU2XX_FWID_DTU205);
			break;

		case 215:
			Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_FPGA | DTU2XX_FWID_DTU215);
			break;

		case 225:
			Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_FPGA | DTU2XX_FWID_DTU225);
			break;

		case 234:
			Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_FPGA | DTU2XX_FWID_DTU234);
			break;

		case 235:
			Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_FPGA | DTU2XX_FWID_DTU235);
			break;

		case 236:
			Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_FPGA | DTU2XX_FWID_DTU236);
			break;

		case 245:
			Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_FPGA | DTU2XX_FWID_DTU245);
			break;

		// Special case: Load manufacturer PLD firmware
		case 299:
			Status = Dtu2xxUploadFirmware(pFdo, DTU2XX_FWID_FPGA | DTU2XX_FWID_MANUF);
			break;

		default:
			break;
		}
	}
	if ( Status!=0 ) {
		DTU2XX_LOG(KERN_INFO,
				   "Dtu2xxInitPld: Failed to program the DTU-%d PLD",
				   pFdo->m_TypeNumber);
		return Status;
	}

	// Get firmware version
	pFdo->m_FirmwareVersion = Dtu2xxGetFirmwareVersion(pFdo);
#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO,
			   "Dtu2xxInitPld: Successfully uploaded firmware version %d to DTU-%d",
			   pFdo->m_FirmwareVersion, pFdo->m_TypeNumber);
#endif
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxInitChannelsGeneric -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Generic initialisation of channels structures.
//
Int  Dtu2xxInitChannelsGeneric(IN PDTU2XX_FDO pFdo)
{
	Int  i;
	Channel*  pCh;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxInitChannelsGeneric: ENTER");
#endif

	// General initialisation of Channel data structure
	for (i=0; i<DTU2XX_MAX_CHANNELS; i++) {

		// Channel pointer
		pCh = &pFdo->m_Channel[i];

		// Identity: index and pointer back to Device Extension
		pCh->m_PortIndex = i;
		pCh->m_pFdo = pFdo;

		// Channel type starts as UNUSED
		pCh->m_ChannelType = DTU2XX_CHANNEL_UNUSED;

		// Channel capability
		pCh->m_Capability = 0;

		// Reset config
		pCh->m_IoConfig = 0;
		pCh->m_IoConfigPar = -1;

		// Initialise exclusive-access variables
		init_MUTEX(&pCh->m_ExclAccLock);
		pCh->m_ExclAccess = 0;

		// Init bulk-transfer resources to safe initial values
		pCh->m_BulkTransf.m_Buffer.m_pStart = NULL;
		pCh->m_BulkTransf.m_Buffer.m_pRdPtr = NULL;
		pCh->m_BulkTransf.m_Buffer.m_pWrPtr = NULL;
		pCh->m_BulkTransf.m_Buffer.m_Load = 0;
		pCh->m_BulkTransf.m_Buffer.m_TotalSize = 0;

		pCh->m_TempBufSize = 0;
		pCh->m_pTempBuf = NULL;

		// Init load counting members
		init_MUTEX(&pCh->m_LoadExtrapLock);
		pCh->m_EnaFifoLoadExtrap = FALSE;
		pCh->m_RpFifoLoad = 0;
		pCh->m_RpTimestamp = Dtu2xxGetTickCount();
		pCh->m_WrittenAfterRp = 0;
		pCh->m_NumTxBytesPerSec = 0;
	}
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxInitDeviceExtension -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Board-specific initialisation of device-extension variables.
//
// Initializes:
//	- m_PortIndex, m_Capability, m_ChannelType, m_IoConfig and m_IoConfigPar
//  - Bulk-transfer resources
//
Int  Dtu2xxInitDeviceExtension(IN PDTU2XX_FDO pFdo)
{
	Int Status = 0;
	Int i=0, BulkBufferSize=0;
	Channel* pCh=NULL;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxInitDeviceExtension: ENTER");
#endif

	// Set default value for some of the board-dependent variables
	// These values may be overriden in the "big switch" that follows
	pFdo->m_NumChannels = 0;

	switch ( pFdo->m_TypeNumber )
	{
	case 205:
		pFdo->m_NumChannels = 1;

		// Init tx-channel
		pFdo->m_Channel[0].m_PortIndex = 0;
		pFdo->m_Channel[0].m_Capability = DTU2XX_CHCAP_OUT_PORT;
		pFdo->m_Channel[0].m_ChannelType = DTU2XX_TS_TX_CHANNEL;
		pFdo->m_Channel[0].m_IoConfig = DTU2XX_IOCONFIG_OUTPUT;
		pFdo->m_Channel[0].m_IoConfigPar = -1;
		break;

	case 215:
		pFdo->m_NumChannels = 1;

		// Init tx-channel
		pFdo->m_Channel[0].m_PortIndex = 0;
		pFdo->m_Channel[0].m_Capability = DTU2XX_CHCAP_OUT_PORT;
		pFdo->m_Channel[0].m_ChannelType = DTU2XX_TS_TX_CHANNEL;
		pFdo->m_Channel[0].m_IoConfig = DTU2XX_IOCONFIG_OUTPUT;
		pFdo->m_Channel[0].m_IoConfigPar = -1;

		// Init modulation parameter cache to "unknown" state
		pFdo->m_Channel[0].m_ModType = -1;
		pFdo->m_Channel[0].m_ParXtra[0] = -1;
		pFdo->m_Channel[0].m_ParXtra[1] = -1;
		pFdo->m_Channel[0].m_ParXtra[2] = -1;
		pFdo->m_Channel[0].m_TsSymOrSampRate = -1;
		pFdo->m_Channel[0].m_RfFreq = -1;
		pFdo->m_Channel[0].m_OutpLeveldBm = -275; // -27.5dBm
		break;

	case 225:
	case 234:
	case 235:
	case 236:
		pFdo->m_NumChannels = 1;

		// Init rx-channel
		pFdo->m_Channel[0].m_PortIndex = 0;
		pFdo->m_Channel[0].m_Capability = DTU2XX_CHCAP_IN_PORT;
		pFdo->m_Channel[0].m_ChannelType = DTU2XX_TS_RX_CHANNEL;
		pFdo->m_Channel[0].m_IoConfig = DTU2XX_IOCONFIG_INPUT;
		pFdo->m_Channel[0].m_IoConfigPar = -1;
		break;

	case 245:
	case 255:
		pFdo->m_NumChannels = 2;

		// Init rx-channel
		pFdo->m_Channel[0].m_PortIndex = 0;
		pFdo->m_Channel[0].m_Capability = DTU2XX_CHCAP_IN_PORT;
		pFdo->m_Channel[0].m_ChannelType = DTU2XX_TS_RX_CHANNEL;
		pFdo->m_Channel[0].m_IoConfig = DTU2XX_IOCONFIG_INPUT;
		pFdo->m_Channel[0].m_IoConfigPar = -1;

		// Init tx-channel
		pFdo->m_Channel[1].m_PortIndex = 1;
		pFdo->m_Channel[1].m_Capability = DTU2XX_CHCAP_OUT_PORT | DTU2XX_CHCAP_LOOPTHR;
		pFdo->m_Channel[1].m_ChannelType = DTU2XX_TS_TX_CHANNEL;
		pFdo->m_Channel[1].m_IoConfig = DTU2XX_IOCONFIG_OUTPUT;
		pFdo->m_Channel[1].m_IoConfigPar = -1;
		break;

	// Special case: Manuf device has no channels
	case 299:
		break;

	default:
		break;
	}

	// Init bulk-transfer resources
	for ( i=0; i<pFdo->m_NumChannels; i++ )
	{
		pCh = &pFdo->m_Channel[i];

		if ( pCh->m_ChannelType == DTU2XX_TS_RX_CHANNEL )
			BulkBufferSize = DTU2XX_RX_RING_BUF_SIZE;
		else
			BulkBufferSize = DTU2XX_TX_RING_BUF_SIZE;

		// Need to create a buffer?
		if ( BulkBufferSize == 0 )
			continue;

		pCh->m_BulkTransf.m_TransfDir = (pCh->m_ChannelType == DTU2XX_TS_RX_CHANNEL) ?
									DTU2XX_DIRECTION_READ : DTU2XX_DIRECTION_WRITE;

		Status = Dtu2xxAllocRingBuffer(&pCh->m_BulkTransf.m_Buffer, BulkBufferSize);
		if ( Status != 0 )
			return Status;

		// Init temporary buffer
		pCh->m_TempBufSize = DTU2XX_BULK_PACKETSIZE;
		pCh->m_pTempBuf = (UInt8*)kmalloc(pCh->m_TempBufSize, GFP_KERNEL);
		if ( pCh->m_pTempBuf == NULL ) {
			DTU2XX_LOG(KERN_INFO,
					   "Dtu2xxInitDeviceExtension: Failed to allocate memory for "
					   "temporary buffer");
			return -ENOMEM;
		}
	}
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxInitVpd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialise VPD caching, read and interpret VPD configuration resources
//
Int  Dtu2xxInitVpd(IN PDTU2XX_FDO pFdo)
{
	Int Status = 0;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxInitVpd: ENTER");
#endif

	// Initialisation VPD user access mutex
	init_MUTEX(&pFdo->m_VpdMutex);

	// Cache EEMPROM contents
	Status = Dtu2xxVpdRead(pFdo, pFdo->m_EepromCache, 0, DTU2XX_VPD_BUF_SIZE);
	if ( Status != 0 )
	{
		DTU2XX_LOG(KERN_INFO, "Dtu2xxInitVpd: Failed to init EEPROM cache");
		return Status;
	}
	// Set pointer to VPD EEPROM data (for now: simple the start of EEPROM cache)
	pFdo->m_pVpdEeprom = pFdo->m_EepromCache;

	// Locate the start of the VpdId item.
	Dtu2xxVpdInitOffset(pFdo);
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxProgramEzUsb -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Program the Cypress EzUsb chip (firmware download). Takes an INTEL HEX format
// file/buffer as input
//
Int  Dtu2xxProgramEzUsb(
	IN PDTU2XX_FDO pFdo,	// Our device object
	IN const DTU2XX_INTEL_HEX_RECORD* pFirmware)
									// The firmware
{
	UInt Dummy=0;
	Int  Status	= 0;
	DTU2XX_INTEL_HEX_RECORD* pPtr = NULL;
	UInt8* pBuf = NULL;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxProgramEzUsb: Start programming of DTU-%d FX2",
			   pFdo->m_TypeNumber);
#endif

	// Helper buffer for usb_control_msg
	pBuf = (UInt8 *)kmalloc( MAX_INTEL_HEX_RECORD_LENGTH, GFP_KERNEL );
	if ( pBuf == NULL )
		return -ENOMEM;

	//.-.-.-.-.-.-.-.-.-.-.-.- Download firmware to internal RAM -.-.-.-.-.-.-.-.-.-.-.-.-
	//
    // Before starting the download, stop the 8051

	Dtu2xx8051Reset(pFdo,1);
	pPtr = (DTU2XX_INTEL_HEX_RECORD*)pFirmware;
	while (pPtr->Type == 0)
	{
		// Load firmware into EzUsb device using a buffer, 'usb_control_msg' cannot handle
		// statically allocated arrays as request data.
		memcpy( pBuf, pPtr->Data, pPtr->Length );

		// Send the load command
		Status  = Dtu2xxVendorRequest( pFdo, ANCHOR_LOAD_INTERNAL, pPtr->Address,
									   0, DTU2XX_DIRECTION_WRITE,
									   pPtr->Length, pBuf, &Dummy );

		if ( Status!=0 )
			break;
		pPtr++;
	}
	// Free our helper buffer
	kfree( pBuf );

	// Restart the 8051
	if ( Status==0 )
        Dtu2xx8051Reset(pFdo,0);

	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxProgramAltera -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Program the ALTERA on the DTU-2XX
//
Int  Dtu2xxProgramAltera(
	IN PDTU2XX_FDO pFdo,	// Our device object
	IN UInt BufSize,			// Size of firmware buffer
	IN const UInt8* pFirmware)	// The firmware
{
	UInt8 BitTest;
	UInt Total, i, l, BitNr, Dummy;
	UInt16 WordBits[1024];
	Int Status = 0;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxProgramAltera: Start programming of DTU-%d PLD",
			   pFdo->m_TypeNumber);
#endif

	Total=0;
	BitNr=0;

	//.-.-.-.-.-.-.-.-.-.- Reset the ALTERA and activate program mode -.-.-.-.-.-.-.-.-.-.
	//
	// Toggles nConfig on the ALTERA

	Status = Dtu2xxVendorRequest(
		pFdo, DTU2XX_USB_SET_PROG_MODE, DTU2XX_STOP_PROG_CYCLE, 0,
		DTU2XX_DIRECTION_WRITE, 0, NULL, &Dummy);
	if ( Status!=0 )
		return Status;

	Status = Dtu2xxVendorRequest(
		pFdo, DTU2XX_USB_SET_PROG_MODE, DTU2XX_START_PROG_CYCLE, 0,
		DTU2XX_DIRECTION_WRITE, 0, NULL, &Dummy);
	if ( Status!=0 )
		return Status;

	//-.-.-.-.-.-.-.-.-.- Read all bytes and send them to the DTU-2xx -.-.-.-.-.-.-.-.-.-.

	// Add 10 bytes extra to have 80 (=10*8) extra clock cycles to init Altera
	for ( i=0; i<BufSize+10; i++)
	{
		if ( i<BufSize )
			BitTest = pFirmware[i];
		else
			BitTest = 0; // Do not really care about data here; we only want some extra
						 // clock cycles to make sure the ALTERA intialises

		// Generate clock data waveform for byte

		for ( l=0; l<8; l++)
		{
			if (BitTest&0x01)
			{
				WordBits[BitNr]=1;
				WordBits[BitNr+1]=3;
			}
			else
			{
				WordBits[BitNr]=0;
				WordBits[BitNr+1]=2;
			}
			BitNr+=2;
			BitTest>>=1;
		}

		// Max number of bits (send data to DTU-2xx)
		if (BitNr==1024){
			Status = Dtu2xxSimpleBulkTransfer(
						pFdo, DTU2XX_DIRECTION_WRITE, EZUSB_ENDPOINT_2, (BitNr*2),
						(UInt8*)WordBits );

			if ( Status!=0 ) {
				return Status;
			}
			BitNr=0;
		}
	}
	// Write last chuck
	Status = Dtu2xxSimpleBulkTransfer(
			pFdo, DTU2XX_DIRECTION_WRITE, EZUSB_ENDPOINT_2, (BitNr*2), (UInt8*)WordBits);
	if ( Status!=0 ) {
		return Status;
	}

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Signal end of program -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	Status = Dtu2xxVendorRequest(
		pFdo, DTU2XX_USB_PROGRAM_DONE, 1, 0, DTU2XX_DIRECTION_WRITE, 0, NULL, &Dummy);

	// Issue a reset to init ALTERA
	if ( Status == 0 )
		Dtu2xxIoCtlReset(pFdo);

	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxInitDeviceHardware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialise USB device. Called from Dtu2xxInitDevice.
//
Int Dtu2xxInitDeviceHardware(
	IN PDTU2XX_FDO pFdo)	// The functional device extension object
{
	Int  i=0;
	Int  Status = 0;
	Channel*  pCh=NULL;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxInitDeviceHardware: ENTER");
#endif

	// First init register cache
	Status = Dtu2xxCacheOpRegs(pFdo);
	if ( Status != 0 ) {
		DTU2XX_LOG(KERN_INFO,
				   "Dtu2xxInitDeviceHardware: Failed init register cache for DTU-%d",
				   pFdo->m_TypeNumber);
		return Status;
	}

	// Init channels to default start-up state
	switch (pFdo->m_TypeNumber)
	{
	case 205:

		for ( i=0; i<pFdo->m_NumChannels; i++ )
		{
			pCh = &pFdo->m_Channel[i];
			// Init channel
			Dtu2xxInitAsiSdiChannel(pCh);
		}
		break;

	case 215:
		// Call DTU-215 specific hardware initialisation
		Status = Dtu2xxInitDtu215Hardware(pFdo);
		break;

	case 225:
		for ( i=0; i<pFdo->m_NumChannels; i++ )
		{
			pCh = &pFdo->m_Channel[i];
			// Init channel
			Dtu2xxInitAsiSdiChannel(pCh);
		}
		break;

	case 234:

		for ( i=0; i<pFdo->m_NumChannels; i++ )
		{
			pCh = &pFdo->m_Channel[i];
			// Init channel
			Dtu2xxInitModChannel(pCh);
		}

		// Additional DTU-234 specific initialisation
		Status = Dtu2xxInitDtu234Hardware(pFdo);
		if ( Status != 0 ) {
			DTU2XX_LOG( KERN_INFO,
					    "Dtu2xxInitDeviceHardware: Dtu2xxInitDtu234Hardware FAILED!!");
		}
		break;

	case 235:
	case 236:

		for ( i=0; i<pFdo->m_NumChannels; i++ )
		{
			pCh = &pFdo->m_Channel[i];
			// Init channel
			Dtu2xxInitModChannel(pCh);
		}
		break;

	case 245:
	case 255:

		for ( i=0; i<pFdo->m_NumChannels; i++ )
		{
			pCh = &pFdo->m_Channel[i];
			// Init channel
			Dtu2xxInitAsiSdiChannel(pCh);
		}
		break;

	case 299:
		break;

	default:
		break;
	}

	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxInitAsiSdiChannel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Initialise Asi/Sdi Tx/Rx ports
//
Int  Dtu2xxInitAsiSdiChannel(
	IN Channel* pCh)
{
	Int Status = 0;
	PDTU2XX_FDO pFdo = pCh->m_pFdo;
	Int PortIndex = pCh->m_PortIndex;
	Int MaxFifoSize=0;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxInitAsiSdiChannel: ENTER");
#endif

	if (pCh->m_ChannelType == DTU2XX_TS_TX_CHANNEL) {
		// ASI/SDI tx-channel

		// Transmit Control := Idle
		Status = Dtu2xxTxIoCtlSetTxControl(pFdo, PortIndex, DTU2XX_TXCTRL_IDLE);
		if ( Status!=0 ) return Status;

		// Transmit Mode := 188
		// Burst Mode Off
		// Null-Packet Stuffing Off
		Status = Dtu2xxTxIoCtlSetTxMode(pFdo, PortIndex, DTU2XX_TXMODE_188,
										DTU2XX_TXSTUFF_MODE_OFF);
		if ( Status!=0 ) return Status;

		// Tx-polarity := normal
		Status = Dtu2xxTxIoCtlTxPolarity(pFdo, PortIndex, DTU2XX_TXPOL_NORMAL);
		if ( Status!=0 ) return Status;

		// Maximum FIFO size
		switch (pCh->m_TxRegs.m_TxStatus.Fields.m_SdramSize)
		{
		case DTU2XX_SDRAM_SIZE_8MB:   MaxFifoSize = 8*1024*1024; break;
		case DTU2XX_SDRAM_SIZE_16MB:  MaxFifoSize = 16*1024*1024; break;
		case DTU2XX_SDRAM_SIZE_32MB:  MaxFifoSize = 32*1024*1024; break;
		default:					  MaxFifoSize = 8*1024*1024; break;
		}
		Status = Dtu2xxTxIoCtlSetFifoSize(pFdo, PortIndex, MaxFifoSize);
		if ( Status!=0 ) return Status;

		// Check for loop-through capability
		if ( 0!=(pCh->m_Capability & DTU2XX_CHCAP_LOOPTHR) )
		{
			// Enable/Disable loop-through mode
			if ( pCh->m_IoConfig==DTU2XX_IOCONFIG_LOOPTHR )
				// Route output to buddy (as indicated by m_IoConfigPar)
				Dtu2xxTxIoCtlSetSourceSelect(pFdo, PortIndex,
											 pCh->m_IoConfigPar);
			else
				// Route output to ourselves
				Dtu2xxTxIoCtlSetSourceSelect(pFdo, PortIndex, PortIndex);
		}
	}
	else {
		// ASI/SDI rx-channel
	}
	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxInitModChannel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialise modulation Tx/Rx ports
//
Int  Dtu2xxInitModChannel(
	IN Channel* pCh)
{
	Int Status = 0;
	PDTU2XX_FDO pFdo = pCh->m_pFdo;
	Int PortIndex = pCh->m_PortIndex;
	Int MaxFifoSize=0;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxInitModChannel: ENTER");
#endif

	if (pCh->m_ChannelType == DTU2XX_TS_TX_CHANNEL) {
		// tx-channel

		// Transmit Control := Idle
		Status = Dtu2xxTxIoCtlSetTxControl(pFdo, PortIndex, DTU2XX_TXCTRL_IDLE);
		if ( Status!=0 ) return Status;

		// Transmit Mode := 188
		// Burst Mode Off
		// Null-Packet Stuffing Off
		Status = Dtu2xxTxIoCtlSetTxMode(pFdo, PortIndex, DTU2XX_TXMODE_188,
										DTU2XX_TXSTUFF_MODE_OFF);
		if ( Status!=0 ) return Status;

		// Tx-polarity := normal
		Status = Dtu2xxTxIoCtlTxPolarity(pFdo, PortIndex, DTU2XX_TXPOL_NORMAL);
		if ( Status!=0 ) return Status;

		// Maximum FIFO size
		switch (pCh->m_TxRegs.m_TxStatus.Fields.m_SdramSize)
		{
		case DTU2XX_SDRAM_SIZE_8MB:   MaxFifoSize = 8*1024*1024; break;
		case DTU2XX_SDRAM_SIZE_16MB:  MaxFifoSize = 16*1024*1024; break;
		case DTU2XX_SDRAM_SIZE_32MB:  MaxFifoSize = 32*1024*1024; break;
		default:					  MaxFifoSize = 8*1024*1024; break;
		}
		Status = Dtu2xxTxIoCtlSetFifoSize(pFdo, PortIndex, MaxFifoSize);
		if ( Status!=0 ) return Status;
	}
	else {
		// Rx-channel

		// Disable ASI polarity (not relevant for a demodulator
		Status = Dtu2xxRxIoCtlRxPolarity(pFdo, PortIndex,
										 DTU2XX_POLARITY_NORMAL);
		if ( Status!=0 ) return Status;
	}
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxAllocRingBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Allocates and initilises a ring buffer
//
Int  Dtu2xxAllocRingBuffer(
	IN PDTU2XX_RING_BUFFER pRingBuf,	// The ring buffer structure
	IN Int Size)						// Size of the ring buffer
{
	Int Status = 0;

	// Init all members to 0
	pRingBuf->m_pStart		= NULL;
	pRingBuf->m_pRdPtr		= NULL;
	pRingBuf->m_pWrPtr		= NULL;
	pRingBuf->m_TotalSize	= 0;
	pRingBuf->m_Load		= 0;

	if ( Size == 0 )
		return -EFAULT;

	// Allocate the memory
	pRingBuf->m_pStart = kmalloc(Size, GFP_KERNEL);
	if ( pRingBuf->m_pStart == NULL ) {
		DTU2XX_LOG(KERN_INFO,
				   "Dtu2xxAllocRingBuffer: Failed to allocate memory for the ring buffer");
		return -ENOMEM;
	}
#if LOG_LEVEL >0
	DTU2XX_LOG(KERN_INFO,
			   "Dtu2xxAllocRingBuffer: Allocated %d bytes for the ring buffer", Size);
#endif
	// Set size
	pRingBuf->m_TotalSize	= Size;
	// Init pointers
	pRingBuf->m_pRdPtr = pRingBuf->m_pWrPtr = pRingBuf->m_pStart;
	// Init spin lock
	spin_lock_init(&(pRingBuf->m_SpinLock));

	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxGetDeviceAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Gets the USB physical address assigned to the DTU-2xx module
//
Int  Dtu2xxGetDeviceAddress(
	IN PDTU2XX_FDO pFdo)	// Our device object
{
	Int Status;
	UInt BytesTransf=0;
	UInt8 TempBuf[3];
	Int UsbAddress = 512;

	// Get the device info (includes firmware version)
	Status = Dtu2xxVendorRequest(pFdo,
						DTU2XX_USB_GET_DEVICE_INF0,
						0,0, DTU2XX_DIRECTION_READ, 3, TempBuf,
						&BytesTransf);

	if ( BytesTransf >= 3 ) {
		UsbAddress = (Int)(TempBuf[2]);
#if LOG_LEVEL > 0
		DTU2XX_LOG(KERN_INFO, "Dtu2xxGetDeviceAddress: USB device address: 0x%02X",
				   UsbAddress);
#endif
	}
	return UsbAddress;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxGetFirmwareVersion -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get the firmware version of the device
//
Int  Dtu2xxGetFirmwareVersion(
	IN PDTU2XX_FDO pFdo)	// Our device object
{
	Int Status;
	UInt BytesTransf=0;
	UInt8 TempBuf[3];
	Int FwVer = -1;

	// Get the device info (includes firmware version)
	Status = Dtu2xxVendorRequest(pFdo,
						DTU2XX_USB_GET_DEVICE_INF0,
						0,0, DTU2XX_DIRECTION_READ, 3, TempBuf,
						&BytesTransf);

	if ( BytesTransf >= 2 ) {
		FwVer = ((Int)(TempBuf[0]));
#if LOG_LEVEL > 0
		DTU2XX_LOG(KERN_INFO, "Dtu2xxGetFirmwareVersion: FW version: %d", FwVer);
#endif
	}
	return FwVer;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxCacheOpRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Cache the operational registers
//
Int  Dtu2xxCacheOpRegs(
	IN PDTU2XX_FDO pFdo)	// Our device object
{
	UInt32*  pReg;
	Int  i, RegAddr, StartAddr, EndAddr;
	Channel*  pCh=NULL;
	Int  Status = 0;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Cache general registers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	StartAddr = DTU2XX_GEN_BASE_ADDR + DTU2XX_GEN_REG_GENCTRL;
	EndAddr = StartAddr + DTU2XX_GEN_REG_STATUS1;
	pReg = (UInt32*)(&(pFdo->m_GenRegs));
	for ( RegAddr=StartAddr; RegAddr<=EndAddr; RegAddr+=4)
	{
		// Get register
		Status = Dtu2xxIoCtlReadRegister(pFdo, RegAddr, pReg);
		if ( Status != 0 )
			return Status;

#if LOG_LEVEL > 0
		DTU2XX_LOG(KERN_INFO,
				   "Dtu2xxCacheOpRegs: Caching general-register: 0x%08X, pReg=0x%p, "
				   "Value: 0x%08X", RegAddr, pReg, (UInt)*pReg);
#endif

		// Move to next register in map
		pReg++;
	}

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Cache channel registers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

	for ( i=0; i<pFdo->m_NumChannels; i++ )
	{
		pCh = &pFdo->m_Channel[i];
		if ( pCh->m_ChannelType == DTU2XX_TS_TX_CHANNEL ) {
			StartAddr = DTU2XX_TX_BASE_ADDR + DTU2XX_TX_REG_GENCTRL;
			EndAddr = StartAddr + DTU2XX_TX_REG_LAST;
			pReg = (UInt32*)(&(pCh->m_TxRegs));
		}
		else {
			StartAddr = DTU2XX_RX_BASE_ADDR + DTU2XX_RX_REG_GENCTRL;
			EndAddr = StartAddr + DTU2XX_RX_REG_BITRATE;
			pReg = (UInt32*)(&(pCh->m_RxRegs));
		}

		for ( RegAddr=StartAddr; RegAddr<=EndAddr; RegAddr+=4)
		{
			// Get register
			Status = Dtu2xxIoCtlReadRegister(pFdo, RegAddr, pReg);
			if ( Status != 0 )
				return Status;

#if LOG_LEVEL > 0
		DTU2XX_LOG(KERN_INFO,
				   "[%d] Dtu2xxCacheOpRegs: Caching register: 0x%08X, pReg=0x%p, "
				   "Value: 0x%08X", pCh->m_PortIndex, RegAddr, pReg, (UInt)*pReg);
#endif

			// Move to next register in map
			pReg++;
		}
	}
	return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxIsFwLoaded -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Check if the firmware for the FX2 chip has been loaded
//
BOOLEAN   Dtu2xxIsFwLoaded(
	IN PDTU2XX_FDO pFdo)	// Our device object
{
	Int Status = 0;
	BOOLEAN FwLoaded=FALSE;
	UInt8* pBuf = NULL;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxIsFwLoaded: Checking if FX2 firmware has been loaded");
#endif

	// Allocate memory for helper buffer
	pBuf = (UInt8 *)kmalloc( 128, GFP_KERNEL );
	if ( pBuf == NULL )
		return FALSE;

	//.-.-.-.-.-.-.-.- Check for the availability of a string descriptor -.-.-.-.-.-.-.-.-

	// Get the string descriptor
	Status = usb_get_descriptor( pFdo->m_pUDev, USB_DT_STRING, 1, pBuf, 128 );

	// Must find a string descriptor size should 3 or more
	if ( Status < 3 )
		FwLoaded = FALSE;
	else
		FwLoaded = TRUE;

	// Free allocated memory
	kfree( pBuf );

	return FwLoaded;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxUploadFirmware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Uploads specified firmware to USB device
//
Int  Dtu2xxUploadFirmware(
	IN PDTU2XX_FDO pFdo,	// Our device object
	IN UInt FirmwareId)			// Firmware ID
{
	Int Status = 0;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "Dtu2xxUploadFirmware: FWID: 0x%04X", FirmwareId);
#endif

	// Which firmware should we upload?
	switch ( FirmwareId & 0xFF )
	{
//	case DTU2XX_FWID_MANUF:
//		if ( (FirmwareId & DTU2XX_FWID_FPGA) != 0 )
//			Status = Dtu2xxProgramAltera(pFdo, sizeof(c_CycloneCodeManuf), c_CycloneCodeManuf);
//		else
//			Status = Dtu2xxProgramEzUsb(pFdo, c_CypressCodeManuf);
//		break;

	case DTU2XX_FWID_DTU205:
		if ( (FirmwareId & DTU2XX_FWID_FPGA) != 0 )
			Status = Dtu2xxProgramAltera(pFdo, sizeof(c_CycloneCode205), c_CycloneCode205);
		else
			Status = Dtu2xxProgramEzUsb(pFdo, c_CypressCode205);
		break;

	case DTU2XX_FWID_DTU215:
		if ( (FirmwareId & DTU2XX_FWID_FPGA) != 0 )
			Status = Dtu2xxProgramAltera(pFdo, sizeof(c_CycloneCode215), c_CycloneCode215);
		else
			Status = Dtu2xxProgramEzUsb(pFdo, c_CypressCode215);
		break;

	case DTU2XX_FWID_DTU225:
		if ( (FirmwareId & DTU2XX_FWID_FPGA) != 0 )
			Status = Dtu2xxProgramAltera(pFdo, sizeof(c_CycloneCode225), c_CycloneCode225);
		else
			Status = Dtu2xxProgramEzUsb(pFdo, c_CypressCode225);
		break;

	case DTU2XX_FWID_DTU234:
		if ( (FirmwareId & DTU2XX_FWID_FPGA) != 0 )
			Status = Dtu2xxProgramAltera(pFdo, sizeof(c_CycloneCode234), c_CycloneCode234);
		else
			Status = Dtu2xxProgramEzUsb(pFdo, c_CypressCode234);
		break;

	case DTU2XX_FWID_DTU235:
		if ( (FirmwareId & DTU2XX_FWID_FPGA) != 0 )
			Status = Dtu2xxProgramAltera(pFdo, sizeof(c_CycloneCode235), c_CycloneCode235);
		else
			Status = Dtu2xxProgramEzUsb(pFdo, c_CypressCode235);
		break;

	case DTU2XX_FWID_DTU236:
		if ( (FirmwareId & DTU2XX_FWID_FPGA) != 0 )
			Status = Dtu2xxProgramAltera(pFdo, sizeof(c_CycloneCode236), c_CycloneCode236);
		else
			Status = Dtu2xxProgramEzUsb(pFdo, c_CypressCode236);
		break;

	case DTU2XX_FWID_DTU245:
		if ( (FirmwareId & DTU2XX_FWID_FPGA) != 0 )
			Status = Dtu2xxProgramAltera(pFdo, sizeof(c_CycloneCode245), c_CycloneCode245);
		else
			Status = Dtu2xxProgramEzUsb(pFdo, c_CypressCode245);
		break;

	case DTU2XX_FWID_DTU2x5:
		if ( (FirmwareId & DTU2XX_FWID_FPGA) != 0 )
			Status = Dtu2xxProgramAltera(pFdo, sizeof(c_CycloneCode2x5), c_CycloneCode2x5);
		else
			Status = Dtu2xxProgramEzUsb(pFdo, c_CypressCode2x5);
		break;

	case DTU2XX_FWID_DTU2x5_A:
		if ( (FirmwareId & DTU2XX_FWID_FPGA) != 0 )
			Status = Dtu2xxProgramAltera(pFdo, sizeof(c_CycloneCode205), c_CycloneCode205);
		else
			Status = Dtu2xxProgramEzUsb(pFdo, c_CypressCode2x5);
		break;

	case DTU2XX_FWID_DTU2x5_B:
		if ( (FirmwareId & DTU2XX_FWID_FPGA) != 0 )
			Status = Dtu2xxProgramAltera(pFdo, sizeof(c_CycloneCode2x5), c_CycloneCode2x5);
		else
			Status = Dtu2xxProgramEzUsb(pFdo, c_CypressCode2x5);
		break;

//	case DTU2XX_FWID_DTU2x5_PROG:
//		if ( (FirmwareId & DTU2XX_FWID_FPGA) != 0 )
//			Status = Dtu2xxProgramAltera(pFdo, sizeof(c_CycloneCode2x5ProgCtrl),
//										 c_CycloneCode2x5ProgCtrl);
//		else
//			Status = Dtu2xxProgramEzUsb(pFdo, c_CypressCode2x5);
//		break;

	default:
		DTU2XX_LOG(KERN_INFO, "Dtu2xxUploadFirmware: UNKNOWN FWID: 0x%04X", FirmwareId);
		Status = -EFAULT;	// Unknown firmware ID
		break;
	}
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxGetIoConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dtu2xxGetIoConfig(
	IN PDTU2XX_FDO pFdo,	// Our device object
	DTU2XX_IOCONFIG* pIoConfig)	// IO Configuration Info
{
	Int Capability, IoConfig;
	Channel* pCh=NULL;

	if (pIoConfig->m_PortIndex<0 || pIoConfig->m_PortIndex>=pFdo->m_NumChannels)
		return -EINVAL;

	pCh = &pFdo->m_Channel[pIoConfig->m_PortIndex];

	IoConfig = pCh->m_IoConfig;
	Capability = pCh->m_Capability;

	// Set extra parameter to default value
	pIoConfig->m_ParXtra = -1;

	if (pCh->m_ChannelType == DTU2XX_TS_TX_CHANNEL)
		pIoConfig->m_IoConfig = DTU2XX_IOCONFIG_OUTPUT;
	else if (pCh->m_ChannelType == DTU2XX_TS_RX_CHANNEL)
		pIoConfig->m_IoConfig = DTU2XX_IOCONFIG_INPUT;
	else {
		pIoConfig->m_IoConfig = DTU2XX_IOCONFIG_NOTSUP;
		return -EINVAL;
	}

	// Following capbilities use extra parameter => init from channel
	pIoConfig->m_ParXtra = pCh->m_IoConfigPar;

	if ((Capability&DTU2XX_CHCAP_DBLBUF) && IoConfig==DTU2XX_IOCONFIG_DBLBUF)
		pIoConfig->m_IoConfig = DTU2XX_IOCONFIG_DBLBUF;
	else if ((Capability&DTU2XX_CHCAP_LOOPTHR) && IoConfig==DTU2XX_IOCONFIG_LOOPTHR)
		pIoConfig->m_IoConfig = DTU2XX_IOCONFIG_LOOPTHR;
	else if ((Capability&DTU2XX_CHCAP_FAILSAFE) && IoConfig==DTU2XX_IOCONFIG_FAILSAFE)
		pIoConfig->m_IoConfig = DTU2XX_IOCONFIG_FAILSAFE;
	else if ((Capability&DTU2XX_CHCAP_DIVERSITY) && IoConfig==DTU2XX_IOCONFIG_DIVERSITY)
		pIoConfig->m_IoConfig = DTU2XX_IOCONFIG_DIVERSITY;
	else if ((Capability&DTU2XX_CHCAP_SHARED) && IoConfig==DTU2XX_IOCONFIG_SHARED)
		pIoConfig->m_IoConfig = DTU2XX_IOCONFIG_SHARED;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "[%d.%d] Dtu2xxGetIoConfig: IoConfig=%d, ParXtra=%d",
			   pFdo->m_TypeNumber, pIoConfig->m_PortIndex, pIoConfig->m_IoConfig,
			   pIoConfig->m_ParXtra);
#endif
	return 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxSetIoConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  Dtu2xxSetIoConfig(
	IN PDTU2XX_FDO pFdo,	// Our device object
	DTU2XX_IOCONFIG* pIoConfig,	// IO Configuration Info
	BOOLEAN Init)				// TRUE: Function used during initialisation
{
	Int Status = 0;
	Channel*  pCh=NULL;

#if LOG_LEVEL > 0
	DTU2XX_LOG(KERN_INFO, "[%d.%d] Dtu2xxSetIoConfig: IoConfig=%d, ParXtra=%d",
			   pFdo->m_TypeNumber, pIoConfig->m_PortIndex, pIoConfig->m_IoConfig,
			   pIoConfig->m_ParXtra);
#endif


	// FOR NOW: only supported on DTU-245 port index 1
	if ( pFdo->m_TypeNumber!=245 || pIoConfig->m_PortIndex!=1  )
	{
		DTU2XX_LOG(KERN_INFO, "[%d.%d] Dtu2xxSetIoConfig: INVALID TYPE AND/OR PORT NUMBER",
				   pFdo->m_TypeNumber, pIoConfig->m_PortIndex);

		return  -EFAULT;
	}

	// FOR NOW: we only support output and loop-through options
	if (   pIoConfig->m_IoConfig!=DTU2XX_IOCONFIG_OUTPUT
		&& pIoConfig->m_IoConfig!=DTU2XX_IOCONFIG_LOOPTHR )
	{
		DTU2XX_LOG(KERN_INFO, "[%d.%d] Dtu2xxSetIoConfig: INVALID CONFIG (%d)",
				   pFdo->m_TypeNumber, pIoConfig->m_PortIndex, pIoConfig->m_IoConfig);
		return  -EFAULT;
	}

	// Get Tx-channel on DTU-245
	pCh = &pFdo->m_Channel[pIoConfig->m_PortIndex];

	// No one can attach to this channel during the update
	if ( 0!=down_interruptible(&pCh->m_ExclAccLock) )
		return -EFAULT;

	if (pCh->m_ExclAccess) {
		DTU2XX_LOG(KERN_INFO, "[%d.%d] Dtu2xxSetIoConfig: CHANNEL IS NOT DETACHED",
				   pFdo->m_TypeNumber, pIoConfig->m_PortIndex);

		up(&pCh->m_ExclAccLock);
		return -EFAULT;
	}

	// Apply Io-configuartion
	switch ( pIoConfig->m_IoConfig )
	{
	case DTU2XX_IOCONFIG_LOOPTHR:
		Status = Dtu2xxTxIoCtlSetSourceSelect(pFdo, 0, 0 /*loop-through copy*/);
		break;

	case DTU2XX_IOCONFIG_OUTPUT:
	default:
		Status = Dtu2xxTxIoCtlSetSourceSelect(pFdo, 0, 1 /*output*/);
		break;
	}

	// Store new configuration
	pCh->m_IoConfig = pIoConfig->m_IoConfig;
	pCh->m_IoConfigPar = pIoConfig->m_ParXtra;

	// IO-config completed => release exclusive access lock
	up(&pCh->m_ExclAccLock);
	return Status;
}
