//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* EzUsb.h *#*#*#*#*#*#*#*#*#* (C) 2000-2004 DEKTEC
//
// Definitions for EzUsb (FX2) cypress USB chip

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	2004.07.27	MG	Created	

#ifndef __EZUSB_H
#define __EZUSB_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Build-in/Cypress commands +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Load command for loading to internal RAM
#define ANCHOR_LOAD_INTERNAL  0xA0		

// These commands are not implemented in the core.  Requires firmware
#define ANCHOR_LOAD_EXTERNAL  0xA3		// Load to external RAM
#define ANCHOR_ISFX2          0xAC		// Check if we are dealing with an FX2

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Internal registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// EZ-USB Control and Status Register.  Bit 0 controls 8051 reset
#define CPUCS_REG_EZUSB    0x7F92
#define CPUCS_REG_FX2      0xE600

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ End-point buffers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

#define EZUSB_ENDPOINT_0	0x00		// END-POINT 0 is exclusively used for control
#define EZUSB_ENDPOINT_2	0x02		// END-POINT 2 is used for firmware uploads (on
										// the DTU-2XX devices)
#define EZUSB_ENDPOINT_4	0x04		// END-POINT 4 is used for writing data to the 
										// DTU-2xx devices
#define EZUSB_ENDPOINT_6	0x06		// END-POINT 6 is used for reading data from the 
										// DTU-2xx devices
#define EZUSB_ENDPOINT_8	0x08		// END-POINT 8 is not used on DTU-2xx devices

#define EZUSB_EP0_MAX_TR_SIZE	64		// Maximum allowed amount of data to be transferred
										// to or from EP0

#endif //#ifndef __EZUSB_H

