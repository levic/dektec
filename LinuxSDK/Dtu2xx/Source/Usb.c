//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Usb.c *#*#*#*#*#*#*#*#*#*# (C) 2000-2008 DekTec
//
// Defines functions for communicating with the Cypress EzUsb (FX2) device

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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxVendorRequest -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Send a USB vendor request to our device
//
Int Dtu2xxVendorRequest(
	DTU2XX_FDO* pFdo,			// The functional device object
	UInt8 RequestCode,			// Request code
	UInt16 RequestValue,		// Request specific value
	UInt16 Index,				// Specifies the device-defined index
	Int Direction,				// Transfer direction
	Int BufSize,				// Size of the data buffer (i.e. number of bytes to transfer)
	UInt8* pData,				// Data buffer
	UInt* pBytesTransf)			// Number of bytes transferred
{
	Int Ret, RequestType, Pipe;

	// Init to 'safe' value
	*pBytesTransf = 0;

	if ( Direction == DTU2XX_DIRECTION_READ )
	{
		RequestType = USB_TYPE_VENDOR | USB_DIR_IN | USB_RECIP_DEVICE;
		Pipe = usb_rcvctrlpipe(pFdo->m_pUDev, 0);
	}
	else
	{
		RequestType = USB_TYPE_VENDOR | USB_DIR_OUT | USB_RECIP_DEVICE;
		Pipe = usb_sndctrlpipe(pFdo->m_pUDev, 0);
	}

	Ret = usb_control_msg(
				pFdo->m_pUDev, Pipe,
				RequestCode,
				RequestType,
				RequestValue,
				Index,
				(char*)pData,
				BufSize,
				10000);

	if ( Ret >= 0 )
	{
		*pBytesTransf = Ret;
		return 0;
	}
	else
		return -EFAULT;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xx8051Reset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Reset the 8051 CPU in the EzUsb chip
//
Int  Dtu2xx8051Reset(
	IN PDTU2XX_FDO pFdo,			// Functional device object
	IN UInt8 ResetBit)				// Reset bit. 0=clear reset, 1=reset CPU
{
	UInt Dummy;
	Int Status=0;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Set the FX2 reset bit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	Status = Dtu2xxVendorRequest( pFdo, ANCHOR_LOAD_INTERNAL, CPUCS_REG_FX2, 0,
								  DTU2XX_DIRECTION_WRITE, 1, &ResetBit, &Dummy );

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxSimpleBulkTransfer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Sends/Gets data to/from the specified END point (using a bulk transfer)
//
Int  Dtu2xxSimpleBulkTransfer(
	DTU2XX_FDO* pFdo,			// The functional device object
	Int Direction,				// Transfer direction
	Int EndPoint,				// USB end point number
	Int BufSize,				// Size of the data buffer (i.e. number of bytes to transfer)
	UInt8* pData)				// Data buffer
{

	Int Ret, Pipe=-1, NumBytesTransf=0;

#if LOG_LEVEL_DMA > 2
	DTU2XX_LOG(KERN_INFO, "Dtu2xxSimpleBulkTransfer: BufSize=%d, EP:%d", BufSize,
			   EndPoint);
#endif

	// Nothing to transfer
	if ( BufSize == 0 )
		return 0;

	//.-.-.-.-.-.-.-.-.-.-.-.- Check if we have a valid end-point -.-.-.-.-.-.-.-.-.-.-.-.

	switch (EndPoint)
	{
	case EZUSB_ENDPOINT_0:
		// Not allowed to send data to EP0
		return -EINVAL;

	case EZUSB_ENDPOINT_2:
		// Supported:  By all DTU-2xx devices
                // Get the pipe
                Pipe = usb_sndbulkpipe(pFdo->m_pUDev, EndPoint);
		break;

	case EZUSB_ENDPOINT_4:
		// Not support by DTU-2xx devices
		DTU2XX_LOG(KERN_INFO,
				   "Dtu2xxSimpleBulkTransfer: Invalid end-point (%d) for DTU-%d",
				   EndPoint, pFdo->m_TypeNumber);
		return -EINVAL;

	case EZUSB_ENDPOINT_6:
		// Supported:  DTU-225, DTU-234, DTU-235, DTU-236 and DTU-245
		if (   pFdo->m_TypeNumber!=225 && pFdo->m_TypeNumber!=234
			&& pFdo->m_TypeNumber!=235 && pFdo->m_TypeNumber!=236
			&& pFdo->m_TypeNumber!=245 && pFdo->m_TypeNumber!=255)
		{
			DTU2XX_LOG(KERN_INFO,
					   "Dtu2xxSimpleBulkTransfer: Invalid end-point (%d) for DTU-%d",
					   EndPoint, pFdo->m_TypeNumber);
			return -EINVAL;
		}
                // Get the pipe
                Pipe = usb_rcvbulkpipe(pFdo->m_pUDev, EndPoint);
		break;

	case EZUSB_ENDPOINT_8:
		// Not support by DTU-2xx devices
		DTU2XX_LOG(KERN_INFO,
				   "Dtu2xxSimpleBulkTransfer: Invalid end-point (%d) for DTU-%d",
				   EndPoint, pFdo->m_TypeNumber);
		return -EINVAL;

	default:
		return -EINVAL;
	}

	// Transfer data to/from pipe
	Ret = usb_bulk_msg(pFdo->m_pUDev, Pipe, pData,  BufSize, &NumBytesTransf, 2000);
	if ( Ret != 0 )
		return -EFAULT;
	return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxReadEeProm -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Gets the contents of the I2C EEPROM on the DTU-2xx
//
Int  Dtu2xxReadEeProm(
	IN PDTU2XX_FDO pFdo,		// Our device object
	IN UInt Offset,				// Offset address within EEPROM (i.e. start read here)
	IN UInt NumBytes,			// Number of bytes to get (i.e. size of pEepromBuf)
	OUT UInt8* pEepromBuf)		// Buffer that receives the EEPROM contents
{
	Int Status = 0;
	UInt TransferSize, ActualTransferred, BufIndex;
	UInt BytesLeft = NumBytes;	// Bytes left to transfer

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "Dtu2xxReadEeProm: NumToRead=%d", NumBytes);
#endif

	// Nothing to transfer
	if ( BytesLeft == 0 )
		return 0;

	BufIndex = 0;
    while ( BytesLeft > 0 )
	{
		// Do not transfer more than maximum allowed
		TransferSize = ( BytesLeft > EZUSB_EP0_MAX_TR_SIZE ) ?
									EZUSB_EP0_MAX_TR_SIZE : BytesLeft;

		Status = Dtu2xxVendorRequest(
								pFdo, DTU2XX_USB_READ_EEPROM,
								Offset+BufIndex,	// Continue where we left of
								0,
								DTU2XX_DIRECTION_READ,
								TransferSize, &(pEepromBuf[BufIndex]),
								&ActualTransferred);

		// Transfer must be succesful
		if ( Status<0 )
			break;

		// Must have transferred all
		if ( ActualTransferred != TransferSize ) {
			Status = -EFAULT;
			break;
		}
		// update bytes left and index with buffer
		BytesLeft -= ActualTransferred;
		BufIndex += ActualTransferred;
	}
	return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu2xxWriteEeProm -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Sets the contents of the I2C EEPROM on the DTU-2xx
//
Int  Dtu2xxWriteEeProm(
	IN PDTU2XX_FDO pFdo,		// Our device object
	IN UInt Offset,				// Offset address within EEPROM (i.e. start write here)
	IN UInt NumBytes,			// Number of bytes to set (i.e. size of pEepromBuf)
	IN UInt8* pEepromBuf)		// Buffer with EEPROM contents to write
{
	Int Status = 0;
	UInt TransferSize, ActualTransferred, BufIndex;
	UInt BytesLeft = NumBytes;	// Bytes left to transfer

#if LOG_LEVEL > 1
	DTU2XX_LOG(KERN_INFO, "Dtu2xxWriteEeProm: NumToWrite=%d", NumBytes);
#endif

	// Nothing to transfer
	if ( BytesLeft == 0 )
		return 0;

	BufIndex = 0;
    while ( BytesLeft > 0 )
	{
		// Do not transfer more than maximum allowed
		TransferSize = ( BytesLeft > EZUSB_EP0_MAX_TR_SIZE ) ?
									EZUSB_EP0_MAX_TR_SIZE : BytesLeft;

		Status = Dtu2xxVendorRequest(
								pFdo, DTU2XX_USB_WRITE_EEPROM,
								Offset+BufIndex,	// Continue where we left of
								0,
								DTU2XX_DIRECTION_WRITE,
								TransferSize, &(pEepromBuf[BufIndex]),
								&ActualTransferred);

		// Transfer must be succesful
		if ( Status<0 ) {
			break;
		}

		// Must have transferred all
		if ( ActualTransferred != TransferSize ) {
			Status = -EFAULT;
			break;
		}
		// update bytes left and index with buffer
		BytesLeft -= ActualTransferred;
		BufIndex += ActualTransferred;
	}
	return Status;
}
