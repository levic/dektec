//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtuVendorCmds.h *#*#*#*#*#*#*#* (C) 2011-2015 DekTec
//
// Dtu driver - USB vendor commands 

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2015 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
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

#define DTU_USB3_FPGA_UPLOAD        0xD0
#define DTU_USB3_PNP_CMD            0xD1
#define DTU_USB3_READ_VALUE         0xD2
#define DTU_USB3_WRITE_VALUE        0xD3
#define DTU_USB3_WRITE_BULK         0xD4

// Values for DTU_USB3_PNP_CMD
#define DTU_PNP_CMD_RESET           0x01
#define DTU_PNP_CMD_DVC_POWER       0x02
#define DTU_PNP_CMD_FPGA_UPL_CMD    0x03
#define DTU_PNP_CMD_SUSPEND_MODE    0x04

// Sub-values for DTU_PNP_CMD_RESET
#define DTU_RESET_BOOTLOADER        0x01
#define DTU_RESET_DVC_STATE         0x02
#define DTU_RESET_VBUS_STATE        0x03
#define DTU_RESET_DMA_IN            0x04
#define DTU_RESET_DMA_OUT           0x05

// Sub-values for DTU_PNP_CMD_DVC_POWER
#define DTU_DVC_POWER_ON            0x01
#define DTU_DVC_POWER_OFF           0x02
#define DTU_DVC_FPGA_POWER_ON       0x03
#define DTU_DVC_FPGA_POWER_OFF      0x04
#define DTU_DVC_FRONTEND_POWER_ON   0x05
#define DTU_DVC_FRONTEND_POWER_OFF  0x06

// Sub-values for DTU_PNP_CMD_FPGA_UPL_CMD
#define DTU_FPGA_UPL_START          0x01
#define DTU_FPGA_UPL_FINISH         0x02

// Sub-values for DTU_PNP_CMD_FAN_CTRL
#define DTU_FAN_CTRL_ON             0x01
#define DTU_FAN_CTRL_OFF            0x02

// Sub-values for DTU_PNP_CMD_LED_CTRL
#define DTU_LED_CTRL_ON             0x01
#define DTU_LED_CTRL_FADE           0x02
#define DTU_LED_CTRL_OFF            0x03

// Device IDs for DTU_USB3_READ_VALUE / DTU_USB3_WRITE_VALUE
#define DTU_USB3_DEV_FX3            0x01
#define DTU_USB3_DEV_FPGA           0x02
#define DTU_USB3_DEV_GS1661         0x03

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TX channel commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define	DTU_USB_RX_RESET            0x40
#define	DTU_USB_RX_CLEAR_FIFO       0x43

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TX channel commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define	DTU_USB_TX_RESET            0x55
#define	DTU_USB_TX_CLEAR_FIFO       0x56

#endif // #ifndef __DTU_VENDOR_CMDS_H
