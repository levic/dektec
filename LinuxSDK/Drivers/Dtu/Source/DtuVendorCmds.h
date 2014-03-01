//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtuVendorCmds.h *#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//
// Dtu driver - USB vendor commands 

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2012 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//  3. The source code may not be modified for the express purpose of enabling hardware
//     features for which no genuine license has been obtained.
//
// THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
// PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL DEKTEC DIGITAL VIDEO BV, ITS AGENTS OR ITS EMPLOYEES BE LIABLE FOR
// ANY DIRECT, INDIRECT, CONSEQUENTIAL, INCIDENTAL, OR OTHER DAMAGES (INCLUDING DAMAGES
// FOR THE LOSS OF USE, INFORMATION, GOODWILL, PROFIT, WORK STOPPAGE, DATA, BUSINESS OR
// REVENUE) UNDER ANY CIRCUMSTANCES, OR UNDER ANY LEGAL THEORY, WHETHER IN CONTRACT, IN
// TORT, IN NEGLIGENCE, OR OTHERWISE, ARISING FROM THE USE OF, OR INABILITY TO USE THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

#ifndef __DTU_VENDOR_CMDS_H
#define __DTU_VENDOR_CMDS_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- General USB Vendor Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DTU_USB_RESET_DEVICE        0x80
#define DTU_USB_SET_PROG_MODE       0x81
#define	DTU_USB_PROGRAM_DONE        0x82
#define	DTU_USB_DVC_LED_CONTROL     0x83	// OBSOLETE. USE DTU_USB_WRITE_REG

#define DTU_USB_READ_EEPROM         0x84
#define DTU_USB_WRITE_EEPROM        0x85
#define DTU_USB_GET_DEVICE_INF0     0x86
#define DTU_USB_I2C_WRITE           0x87
#define DTU_USB_I2C_READ            0x88

#define DTU_USB_GET_USB_SPEED       0x8A
#define DTU_USB_EEPROM_CONTROL      0x8B
#define DTU_USB_FW_LOADED           0x8C

#define DTU_USB_WRITE_REG           0x90
#define DTU_USB_READ_REG            0x91



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TX channel commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define	DTU_USB_RX_RESET            0x40
#define	DTU_USB_RX_CLEAR_FIFO       0x43

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TX channel commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define	DTU_USB_TX_RESET            0x55
#define	DTU_USB_TX_CLEAR_FIFO       0x56

#endif // #ifndef __DTU_VENDOR_CMDS_H
