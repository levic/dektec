//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dtu2xxCodes.h *#*#*#*#*#*#*#*# (C) 2000-2004 DEKTEC
//
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Change History -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//	2004.07.19	MG	Created	

#ifndef __DTU2XXCODES_H
#define __DTU2XXCODES_H

typedef unsigned long DTU2XX_RESULT;


//+=+=+=+=+=+=+=+=+=+=+ General definitions for the DTU-2xx devices +=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ALTERA program mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DTU2XX_START_PROG_CYCLE		1
#define DTU2XX_STOP_PROG_CYCLE		0

#define DTU2XX_FW_LOADED_CODE		0x5A2C

//+=+=+=+=+=+=+=+=+ Definitions below must match definitions in DTAPI.h! +=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- I/O Configuration -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTU2XX_IOCONFIG_NOTSUP		0
#define  DTU2XX_IOCONFIG_INPUT		1
#define  DTU2XX_IOCONFIG_OUTPUT		2
#define  DTU2XX_IOCONFIG_DBLBUF		3
#define  DTU2XX_IOCONFIG_IP			4
#define  DTU2XX_IOCONFIG_FAILSAFE	5
#define  DTU2XX_IOCONFIG_DIVERSITY	6
#define  DTU2XX_IOCONFIG_LOOPTHR	7
#define  DTU2XX_IOCONFIG_SHARED		8

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EEPROM Control -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTU2XX_EEPROM_PROTECT		0xA74C
#define  DTU2XX_EEPROM_PROTECT_CLR	0xD2C4

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ASI Polarity-Control Status -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTU2XX_ASIINV_NORMAL	0
#define  DTU2XX_ASIINV_INVERT	1

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ASI Input-Clock Lock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTU2XX_ASI_NOLOCK		0
#define  DTU2XX_ASI_INLOCK		1

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Clock Detector -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTU2XX_CLKDET_FAIL		0
#define  DTU2XX_CLKDET_OK		1

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Input Rate Ok -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTU2XX_INPRATE_LOW		0
#define  DTU2XX_INPRATE_OK		1

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- #Invalid Bytes per Packet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTU2XX_NUMINV_NONE		0
#define  DTU2XX_NUMINV_16		1
#define  DTU2XX_NUMINV_OTHER		2

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Packet Size -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTU2XX_PCKSIZE_INV		0
#define  DTU2XX_PCKSIZE_188		2
#define  DTU2XX_PCKSIZE_204		3

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Receive Control -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTU2XX_RXCTRL_IDLE		0
#define  DTU2XX_RXCTRL_RCV		1

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Receive Mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTU2XX_RXMODE_ST188		0
#define  DTU2XX_RXMODE_ST204		1
#define  DTU2XX_RXMODE_STMP2		2
#define  DTU2XX_RXMODE_STRAW		3
#define  DTU2XX_RXMODE_IPRAW		5
#define  DTU2XX_RXMODE_STTRP		7
#define  DTU2XX_RX_TIMESTAMP		0x008		
#define  DTU2XX_RXMODE_SDI			0x010		// Flag: can be OR-ed with other modes
#define  DTU2XX_RXMODE_SDI_FULL		0x010	// Flag: can be OR-ed with other modes
#define  DTU2XX_RXMODE_SDI_ACTVID	0x011	// Flag: can be OR-ed with other modes
#define  DTU2XX_RXMODE_SDI_10B		0x020	// Flag: can be OR-ed with other modes
#define  DTU2XX_RXMODE_SDI_HUFFMAN	0x080	// SDI Huffman compression on

#define  DTU2XX_RXMODE_MASK			0x07	// Mask for RxMode without flags

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Clock-Generator Modes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTU2XX_TXCLOCK_INTERNAL	0
#define  DTU2XX_TXCLOCK_EXTERNAL	1

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Detach Mode flags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTU2XX_INSTANT_DETACH		1
#define  DTU2XX_ZERO_OUTPUT			2
#define  DTU2XX_TRISTATE_OUTPUT		4
#define  DTU2XX_WAIT_UNTIL_SENT		8

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- LED Control -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTU2XX_LED_OFF				0
#define  DTU2XX_LED_GREEN			1
#define  DTU2XX_LED_RED				2
#define  DTU2XX_LED_YELLOW			3
#define  DTU2XX_LED_HARDWARE		4

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Loop-Back Mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTU2XX_NO_LOOPBACK			0
#define  DTU2XX_LOOPBACK_MODE		1

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Packet Transmit Mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTU2XX_TXMODE_188			0
#define  DTU2XX_TXMODE_204			1
#define  DTU2XX_TXMODE_ADD16		2
#define  DTU2XX_TXMODE_RAW			3
#define  DTU2XX_TXMODE_192			4		// Supported on DTA-102 Firmware Rev >= 2
#define  DTU2XX_TXMODE_130			5		// Supported on DTA-102 Firmware Rev >= 7
#define  DTU2XX_TXMODE_MIN16		6		// Supported on DTA-100 Firmware Rev >= 5
											// and          DTA-102 Firmware Rev >= 8
#define  DTU2XX_TXMODE_IPRAW		7
#define  DTU2XX_TXMODE_MASK			0x00F	// Mask for TxMode without burst flag
#define  DTU2XX_TXMODE_BURST		0x010	// Can be OR-ed with one of 188/192/204/ADD16/RAW
#define	 DTU2XX_TXMODE_SDI			0x040
#define  DTU2XX_TXMODE_SDI_FULL		0x040
#define  DTU2XX_TXMODE_SDI_ACTVID	0x041
#define	 DTU2XX_TXMODE_SDI_10B		0x080	// Can be OR-ed with DTU2XX_TXMODE_SDI
#define  DTU2XX_TXMODE_SDI_525		0x000	// Can be OR-ed with DTU2XX_TXMODE_SDI/10BIT
#define  DTU2XX_TXMODE_SDI_625		0x100	// Can be OR-ed with DTU2XX_TXMODE_SDI/10BIT
#define  DTU2XX_TXMODE_SDI_HUFFMAN	0x200	// SDI huffman compression on

#define  DTU2XX_TXSTUFF_MODE_OFF	0		// Stuff-mode is disabled
#define  DTU2XX_TXSTUFF_MODE_ON		1		// Stuff-mode is enabled

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Transmit Polarity -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTU2XX_TXPOL_NORMAL		0
#define  DTU2XX_TXPOL_INVERTED		1

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Polarity Control -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTU2XX_POLARITY_AUTO		0
#define  DTU2XX_POLARITY_NORMAL		2
#define  DTU2XX_POLARITY_INVERT		3

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Reset Mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  DTU2XX_FIFO_RESET			0
#define  DTU2XX_FULL_RESET			1

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Status Flags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTU2XX_RX_DMA_PENDING		1
#define  DTU2XX_RX_FIFO_OVF			2
#define  DTU2XX_RX_SYNC_ERR			4
#define  DTU2XX_RX_RATE_OVF			8
#define  DTU2XX_RX_TARGET_ERR		16

#define  DTU2XX_TX_DMA_PENDING		1
#define  DTU2XX_TX_FIFO_UFL			2
#define  DTU2XX_TX_SYNC_ERR			4
#define  DTU2XX_TX_READBACK_ERR		8
#define  DTU2XX_TX_TARGET_ERR		16

#define  DTU2XX_AUTO_CLEARFLAGS_OFF	0
#define  DTU2XX_AUTO_CLEARFLAGS_ON	1

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Transmit Control -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTU2XX_TXCTRL_IDLE			0
#define  DTU2XX_TXCTRL_HOLD			1
#define  DTU2XX_TXCTRL_SEND			3

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- USB speed modes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DTU2XX_USB_FULL_SPEED		0
#define DTU2XX_USB_HIGH_SPEED		1

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SDRAM sizes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DTU2XX_SDRAM_SIZE_8MB		0
#define DTU2XX_SDRAM_SIZE_16MB		1
#define DTU2XX_SDRAM_SIZE_32MB		2
#define DTU2XX_SDRAM_SIZE_64MB		3
#define DTU2XX_SDRAM_SIZE_128MB		4

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Upconverter RF modes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTU2XX_UPCONV_NORMAL			0
#define DTU2XX_UPCONV_MUTE			1
#define DTU2XX_UPCONV_CW				2
#define DTU2XX_UPCONV_CWI			3
#define DTU2XX_UPCONV_CWQ			4

#define DTU2XX_UPCONV_SPECINV		0x100	// Can be OR-ed with other RF modes


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Modulation Parameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// NOTE:
// DTU2XX_MOD constants below do NOT correspond 1-to-1 with modulation-type register
// For definition of modulation-type register values, see DTU2XX_TXMODC_ constants
// defined in Dtu2xxRegs.h
#define  DTU2XX_MOD_DVBS_QPSK		0		// Native DVB-S on DTA-107
#define  DTU2XX_MOD_DVBS_BPSK		1		// Not used yet
#define  DTU2XX_MOD_QAM4			3
#define  DTU2XX_MOD_QAM16			4
#define  DTU2XX_MOD_QAM32			5
#define  DTU2XX_MOD_QAM64			6
#define  DTU2XX_MOD_QAM128			7
#define  DTU2XX_MOD_QAM256			8
#define  DTU2XX_MOD_DVBT			9
#define  DTU2XX_MOD_ATSC			10
#define  DTU2XX_MOD_DVBT2			11
#define  DTU2XX_MOD_ISDBT			12
#define  DTU2XX_MOD_DVBS_QPSK_SOFT	16		// Soft DVB-S for DTA-107S2
#define  DTU2XX_MOD_DVBS2_QPSK		32
#define  DTU2XX_MOD_DVBS2_8PSK		33
#define  DTU2XX_MOD_DMBTH			48
#define  DTU2XX_MOD_ADTBT			49
#define  DTU2XX_MOD_CMMB			50
#define  DTU2XX_MOD_T2MI			51

#define  DTU2XX_MOD_ATSC_VSB8		0
#define  DTU2XX_MOD_ATSC_VSB16		1

#define  DTU2XX_MOD_1_2				0		// Code rate 1/2
#define  DTU2XX_MOD_2_3				1		// Code rate 2/3
#define  DTU2XX_MOD_3_4				2		// Code rate 3/4
#define  DTU2XX_MOD_4_5				3		// Code rate 4/5
#define  DTU2XX_MOD_5_6				4		// Code rate 5/6
#define  DTU2XX_MOD_6_7				5		// Code rate 6/7
#define  DTU2XX_MOD_7_8				6		// Code rate 7/8
#define  DTU2XX_MOD_1_4				7		// Code rate 1/4
#define  DTU2XX_MOD_1_3				8		// Code rate 1/3
#define  DTU2XX_MOD_2_5				9		// Code rate 2/5
#define  DTU2XX_MOD_3_5				10		// Code rate 3/5
#define  DTU2XX_MOD_8_9				11		// Code rate 8/9
#define  DTU2XX_MOD_9_10			12		// Code rate 9/10

#define  DTU2XX_MOD_J83_MSK			0x0003
#define  DTU2XX_MOD_J83_A			2		// J.83 annex A (DVB-C)
#define  DTU2XX_MOD_J83_B			3		// J.83 annex B (“American QAM”)
#define  DTU2XX_MOD_J83_C			1		// J.83 annex C (“Japanese QAM”)
#define  DTU2XX_MOD_DVBC_SOFT		0x0100	// Soft DVB-C flag

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Modulation Parameters (DVB-T) -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define  DTU2XX_MOD_DVBT_BW_MSK		0x0000000F
#define  DTU2XX_MOD_DVBT_5MHZ		0x00000001
#define  DTU2XX_MOD_DVBT_6MHZ		0x00000002
#define  DTU2XX_MOD_DVBT_7MHZ		0x00000003
#define  DTU2XX_MOD_DVBT_8MHZ		0x00000004
#define  DTU2XX_MOD_DVBT_CO_MSK		0x000000F0
#define  DTU2XX_MOD_DVBT_QPSK		0x00000010
#define  DTU2XX_MOD_DVBT_QAM16		0x00000020
#define  DTU2XX_MOD_DVBT_QAM64		0x00000030
#define  DTU2XX_MOD_DVBT_GU_MSK		0x00000F00
#define  DTU2XX_MOD_DVBT_G_1_32		0x00000100
#define  DTU2XX_MOD_DVBT_G_1_16		0x00000200
#define  DTU2XX_MOD_DVBT_G_1_8		0x00000300
#define  DTU2XX_MOD_DVBT_G_1_4		0x00000400
#define  DTU2XX_MOD_DVBT_IL_MSK		0x0000F000
#define  DTU2XX_MOD_DVBT_INDEPTH	0x00001000
#define  DTU2XX_MOD_DVBT_NATIVE		0x00002000
#define  DTU2XX_MOD_DVBT_MD_MSK		0x000F0000
#define  DTU2XX_MOD_DVBT_2K			0x00010000
#define  DTU2XX_MOD_DVBT_4K			0x00020000
#define  DTU2XX_MOD_DVBT_8K			0x00030000

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Firmware IDs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#define DTU2XX_FWID_USBITF		0x4000		// Can be OR-ed with values below to
											// id firmware for USB interface
#define DTU2XX_FWID_FPGA		0x8000		// Can be OR-ed with values below to
											// id firmware for onboard FPGA

#define DTU2XX_FWID_MANUF		0x0000		// Firmware ID for manufacturing FW
#define DTU2XX_FWID_DTU205		0x0001		// Firmware ID for DTU-205
#define DTU2XX_FWID_DTU225		0x0002		// Firmware ID for DTU-225
#define DTU2XX_FWID_DTU234		0x0003		// Firmware ID for DTU-234
#define DTU2XX_FWID_DTU235		0x0004		// Firmware ID for DTU-235
#define DTU2XX_FWID_DTU245		0x0005		// Firmware ID for DTU-245
#define DTU2XX_FWID_DTU2x5		0x0006		// Firmware ID for DTU-2x5
#define DTU2XX_FWID_DTU2x5_A	0x0007		// Firmware ID for DTU-2x5 with RX SDI support
#define DTU2XX_FWID_DTU2x5_B	0x0008		// Firmware ID for DTU-2x5 with TX SDI support
#define DTU2XX_FWID_DTU2x5_PROG	0x0009		// Firmware ID for DTU-2x5 with JTAG 
											// programming interface
#define DTU2XX_FWID_DTU236		0x000A		// Firmware ID for DTU-236
#define DTU2XX_FWID_DTU215		0x000B		// Firmware ID for DTU-215

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Result codes +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Feature not supported
#define  DTU2XX_NOT_SUPPORTED		-1

#define  DTU2XX_OK					0
#define  DTU2XX_E					0x1000
#define  DTU2XX_E_ATTACHED			(DTU2XX_E + 0)
#define  DTU2XX_E_BUF_TOO_SMALL		(DTU2XX_E + 1)
#define  DTU2XX_E_DEV_DRIVER		(DTU2XX_E + 2)
#define  DTU2XX_E_EEPROM_FULL		(DTU2XX_E + 3)
#define  DTU2XX_E_EEPROM_READ		(DTU2XX_E + 4)
#define  DTU2XX_E_EEPROM_WRITE		(DTU2XX_E + 5)
#define  DTU2XX_E_EEPROM_FORMAT		(DTU2XX_E + 6)
#define  DTU2XX_E_FIFO_FULL			(DTU2XX_E + 7)
#define  DTU2XX_E_IN_USE			(DTU2XX_E + 8)
#define  DTU2XX_E_INVALID_BUF		(DTU2XX_E + 9)
#define  DTU2XX_E_INVALID_CGMODE	(DTU2XX_E + 10)
#define  DTU2XX_E_INVALID_FLAGS		(DTU2XX_E + 11)
#define  DTU2XX_E_INVALID_MODE		(DTU2XX_E + 12)
#define  DTU2XX_E_INVALID_RATE		(DTU2XX_E + 13)
#define  DTU2XX_E_INVALID_SIZE		(DTU2XX_E + 14)
#define  DTU2XX_E_KEYWORD			(DTU2XX_E + 15)
#define  DTU2XX_E_NO_DTA_CARD		(DTU2XX_E + 16)
#define  DTU2XX_E_NO_LOOPBACK		(DTU2XX_E + 17)
#define  DTU2XX_E_NO_SUCH_DEVICE	(DTU2XX_E + 18)
#define  DTU2XX_E_NO_SUCH_OUTPUT	(DTU2XX_E + 19)
#define  DTU2XX_E_NO_TS_OUTPUT		(DTU2XX_E + 20)
#define  DTU2XX_E_NOT_ATTACHED		(DTU2XX_E + 21)
#define  DTU2XX_E_NOT_FOUND			(DTU2XX_E + 22)
#define  DTU2XX_E_NOT_SUPPORTED		(DTU2XX_E + 23)
#define  DTU2XX_E_PCICARD			(DTU2XX_E + 24)
#define  DTU2XX_E_TOO_LONG			(DTU2XX_E + 25)
#define  DTU2XX_E_UNDERFLOW			(DTU2XX_E + 26)
#define  DTU2XX_E_NO_SUCH_INPUT		(DTU2XX_E + 27)
#define  DTU2XX_E_NO_TS_INPUT		(DTU2XX_E + 28)

#endif // #ifndef __DTU2XXCODES_H
