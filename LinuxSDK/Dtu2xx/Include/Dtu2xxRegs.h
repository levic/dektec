//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dtu2xxRegs.h *#*#*#*#*#*#*#*# (C) 2000-2010 DekTec
//
// Definition of USB commands for DTU-2XX USB devices

#ifndef __DTU2XXREGS_H
#define __DTU2XXREGS_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- General USB Vendor Commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DTU2XX_USB_RESET_DEVICE			0x80
#define DTU2XX_USB_SET_PROG_MODE		0x81
#define	DTU2XX_USB_PROGRAM_DONE			0x82
#define	DTU2XX_USB_DVC_LED_CONTROL		0x83	// OBSOLETE. USE DTU2XX_USB_WRITE_REG

#define DTU2XX_USB_READ_EEPROM			0x84
#define DTU2XX_USB_WRITE_EEPROM			0x85
#define DTU2XX_USB_GET_DEVICE_INF0		0x86
#define DTU2XX_USB_I2C_WRITE			0x87
#define DTU2XX_USB_I2C_READ				0x88

#define DTU2XX_USB_GET_USB_SPEED		0x8A
#define DTU2XX_USB_EEPROM_CONTROL		0x8B
#define DTU2XX_USB_FW_LOADED			0x8C

#define DTU2XX_USB_WRITE_REG			0x90
#define DTU2XX_USB_READ_REG				0x91

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- RX channel commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DTU2XX_USB_RX_SET_RX_CONTROL	0x30	// OBSOLETE. USE DTU2XX_USB_WRITE_REG
#define DTU2XX_USB_RX_GET_RX_CONTROL	0x31	// OBSOLETE. USE DTU2XX_USB_READ_REG
#define DTU2XX_USB_RX_SET_RX_MODE		0x32	// OBSOLETE. USE DTU2XX_USB_WRITE_REG
#define DTU2XX_USB_RX_GET_RX_MODE		0x33	// OBSOLETE. USE DTU2XX_USB_READ_REG
#define DTU2XX_USB_RX_EQUALISE			0x34	// OBSOLETE. USE DTU2XX_USB_WRITE_REG
#define DTU2XX_USB_RX_GET_FIFO_LOAD		0x35	// OBSOLETE. USE DTU2XX_USB_READ_REG
#define DTU2XX_USB_RX_GET_FLAGS			0x36	// OBSOLETE. USE DTU2XX_USB_READ_REG
#define DTU2XX_USB_RX_CLEAR_FLAGS		0x37	// OBSOLETE. USE DTU2XX_USB_WRITE_REG
#define DTU2XX_USB_RX_GET_STATUS		0x38	// OBSOLETE. USE DTU2XX_USB_READ_REG
#define DTU2XX_USB_RX_GET_TS_RATE_BPS	0x39	// OBSOLETE. USE DTU2XX_USB_READ_REG
#define DTU2XX_USB_RX_RESET				0x40	
#define DTU2XX_USB_RX_SET_LOOPBACK_MODE	0x41	// OBSOLETE. USE DTU2XX_USB_WRITE_REG
#define DTU2XX_USB_RX_LED_CONTROL		0x42	// OBSOLETE. USE DTU2XX_USB_WRITE_REG
#define	DTU2XX_USB_RX_CLEAR_FIFO		0x43	
#define	DTU2XX_USB_RX_GET_VIOL_COUNT	0x44	// OBSOLETE. USE DTU2XX_USB_READ_REG

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TX channel commands -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define	DTU2XX_USB_TX_SET_TX_CONTROL	0x51	// OBSOLETE. USE DTU2XX_USB_WRITE_REG
#define	DTU2XX_USB_TX_GET_TX_CONTROL	0x52	// OBSOLETE. USE DTU2XX_USB_READ_REG
#define	DTU2XX_USB_TX_SET_TX_MODE		0x53	// OBSOLETE. USE DTU2XX_USB_WRITE_REG
#define	DTU2XX_USB_TX_GET_TX_MODE		0x54	// OBSOLETE. USE DTU2XX_USB_READ_REG
#define	DTU2XX_USB_TX_RESET				0x55	
#define	DTU2XX_USB_TX_CLEAR_FIFO		0x56	
#define	DTU2XX_USB_TX_GET_FLAGS			0x57	// OBSOLETE. USE DTU2XX_USB_READ_REG
#define	DTU2XX_USB_TX_CLEAR_FLAGS		0x58	// OBSOLETE. USE DTU2XX_USB_WRITE_REG
#define	DTU2XX_USB_TX_GET_FIFO_LOAD		0x59	// OBSOLETE. USE DTU2XX_USB_READ_REG
#define	DTU2XX_USB_TX_SET_FIFO_SIZE		0x60	// OBSOLETE. USE DTU2XX_USB_WRITE_REG
#define	DTU2XX_USB_TX_SET_TX_CLOCK		0x61	// OBSOLETE. USE DTU2XX_USB_WRITE_REG
#define	DTU2XX_USB_TX_GET_TX_CLOCK		0x62	// OBSOLETE. USE DTU2XX_USB_READ_REG
#define	DTU2XX_USB_TX_SET_LOOPBACK_MODE	0x63	// OBSOLETE. USE DTU2XX_USB_WRITE_REG
#define	DTU2XX_USB_TX_RD_LOOPBACK_DATA	0x64	// OBSOLETE. USE DTU2XX_USB_READ_REG

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Helper values -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTU2XX_BULK_PACKETSIZE			0xFC00
#define DTU2XX_BULK_BUFCOUNT			0x2

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Operation register structures +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//

// The general registers base address
#define DTU2XX_GEN_BASE_ADDR			0x0000

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ General register offset +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
#define DTU2XX_GEN_REG_GENCTRL			0x0000
#define DTU2XX_GEN_REG_GENSTATUS		0x0004
#define DTU2XX_GEN_REG_PROGRAMMING		0x0008
#define DTU2XX_GEN_REG_REFCLKCNT		0x000C
#define DTU2XX_GEN_REG_PROGRAMMING2		0x001C
#define DTU2XX_GEN_REG_CONTROL0			0x0030
#define DTU2XX_GEN_REG_CONTROL1			0x0034
#define DTU2XX_GEN_REG_STATUS0			0x0038
#define DTU2XX_GEN_REG_STATUS1			0x003C

#ifdef DTU2XX_DRIVER

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Dtu2xxGen +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Register block for GENERIC fields.
//
typedef struct _Dtu2xxGen {

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- General-Control register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	union {
	struct {								
	volatile UInt32  m_PE			: 1;	// Serial EEPROM Program Enable
	volatile UInt32  m_PRE			: 1;	// Serial EEPROM Protect Register Enable
	volatile UInt32  m_Reset		: 1;	// Board-wide software reset
			 UInt32  m_Reserved1	: 1;	//
	volatile UInt32  m_PerIntEn		: 1;	// Periodic-Interrupt Enable
			 UInt32  m_Reserved2	: 3;	//
			 UInt32  m_FirmwRev		: 8;	// Firmware Revision
			 UInt32  m_TypeNum		: 8;	// Board Type Number
	volatile UInt32  m_LedControl	: 1;	// Take over LED Control
	volatile UInt32  m_LedGreen		: 1;	// State of Green LED (if LedControl is set)
	volatile UInt32  m_LedRed		: 1;	// State of Red LED (if LedControl is set)
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  m_GenControl;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- General-Status register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	union {
	 struct {
			  UInt32  m_Reserved1	: 4;	//
	 volatile UInt32  m_PerInt		: 1;	// Periodic Interrupt
	 }		 Fields;						// For access to individual field
	 UInt32  All;							// For access to GenStatus in one go
	}  m_GenStatus;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Programming register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	union {
	struct {								
	volatile UInt32  m_TRST			: 1;	// Control of TRST# pin
	volatile UInt32  m_TDO			: 1;	// Status of TDO pin
	volatile UInt32  m_TMS			: 1;	// Control of TMS pin
	volatile UInt32  m_TCK			: 1;	// Control of TCK pin
	volatile UInt32  m_TDI			: 1;	// Control of TDI pin
	volatile UInt32  m_PrgEpc		: 1;	// Enable access to programming i/f of EPC2
	         UInt32  m_Reserved		: 2;
	volatile UInt32  m_JtagCnt		: 8;	// Number of bits used in CPLD programming 2
	}			     Fields;				// For access to individual fields
			 UInt32  All;					// For access to complete register
	}  m_Programming;

	volatile UInt32  m_RefClkCnt;			// Reference clock counter
	volatile UInt32  m_EpcsCtrl;			// EPCS Control register
	volatile UInt32  m_EpcsData;			// EPCS Data register

	UInt32  m_Reserved1[4];					// Unused memory addresses
	
	volatile UInt32  m_BoardControl0;		// Generic board control register 0
	volatile UInt32  m_BoardControl1;		// Generic board control register 1
	volatile UInt32  m_BoardStatus0;		// Generic board status register 0
	volatile UInt32  m_BoardStatus1;		// Generic board status register 1

	UInt32  m_Reserved2[18];				// Unused memory addresses
} Dtu2xxGen;

#endif // #ifdef DTU2XX_DRIVER

// The transmit-channel base address
#define DTU2XX_TX_BASE_ADDR				0x0080

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ TX register offset +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
#define DTU2XX_TX_REG_GENCTRL			0x0000
#define DTU2XX_TX_REG_TXCTRL			0x0004
#define DTU2XX_TX_REG_TXSTATUS			0x0008
#define DTU2XX_TX_REG_TXCLOCK			0x000C
#define DTU2XX_TX_REG_TXCTRL2			0x0010
#define DTU2XX_TX_REG_FIFOSIZE			0x0014
#define DTU2XX_TX_REG_FIFOLOAD			0x0018
#define DTU2XX_TX_REG_DIAG				0x001C
#define DTU2XX_TX_REG_LOOPBKDATA		0x0020
#define DTU2XX_TX_REG_MODCONTROL3		0x0024
#define DTU2XX_TX_REG_MODCONTROL1		0x0028
#define DTU2XX_TX_REG_MODCONTROL2		0x002C
//#define DTU2XX_TX_REG_RFCONTROL1		0x0038
#define DTU2XX_TX_REG_RFCONTROL2		0x003C
#define DTU2XX_TX_REG_FIFO_DATA			0x0040
#define DTU2XX_TX_REG_AD9789CTRL		0x0050
#define DTU2XX_TX_REG_AD9789DATA		0x0054
#define DTU2XX_TX_REG_LAST				0x007C	// Marks end of TX register space


//-.-.-.-.-.-.-.-.-.- Modulation control register field value defines -.-.-.-.-.-.-.-.-.-.

// Modulation-Type field: values
// Values must be the same as DTA1XX_TXMODC_xxx
#define DTU2XX_TXMODC_QPSK			0
#define DTU2XX_TXMODC_BPSK			1
#define DTU2XX_TXMODC_QAM4			3
#define DTU2XX_TXMODC_QAM16			4
#define DTU2XX_TXMODC_QAM32			5
#define DTU2XX_TXMODC_QAM64			6
#define DTU2XX_TXMODC_QAM128		7
#define DTU2XX_TXMODC_QAM256		8
#define DTU2XX_TXMODC_IQMAP			9		// I/Q Mapping
											// On DTA-107S2: 4-bit symbol mode
#define DTU2XX_TXMODC_IQDIRBUF		14		// Direct I/Q, buffered
#define DTU2XX_TXMODC_IQDIRECT		15		// Direct I/Q

// I/Q Mapping field: values
// Values must be the same as DTA1XX_IQMAP_xxx
#define DTU2XX_IQMAP_QAM			0		// General QAM I/Q mapping
#define DTU2XX_IQMAP_QAM16			1		// Optimised I/Q mapping for 16-QAM
#define DTU2XX_IQMAP_QAM32			2		// Optimised I/Q mapping for 32-QAM
#define DTU2XX_IQMAP_QAM64			3		// Optimised I/Q mapping for 64-QAM
#define DTU2XX_IQMAP_VSB8			4
#define DTU2XX_IQMAP_VSB16			5

// Roll-Off factor field: values
// Values must be the same as DTA1XX_ROLOFF_xxx
#define DTU2XX_ROLLOFF_12			0
#define DTU2XX_ROLLOFF_13			1
#define DTU2XX_ROLLOFF_15			2
#define DTU2XX_ROLLOFF_18			3
#define DTU2XX_ROLLOFF_5			4

// Test patterns
// Values must be the same as DTA1XX_TP_xxx
#define DTU2XX_TP_NORMAL			0		// Normal mode
#define DTU2XX_TP_NYQUIST			1		// Nyquist test-pattern
#define DTU2XX_TP_HALFNYQ			2		// Half-nyquist tets-pattern
#define DTU2XX_TP_CW				3		// Carrier only
#define DTU2XX_TP_CWI				4		// Constant I
#define DTU2XX_TP_CWQ				5		// Constant Q

#ifdef DTU2XX_DRIVER

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Dtu2xxTx +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Register block for a TRANSMISSION channel.
//
typedef struct _Dtu2xxTx {

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- General-Control register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// On DTA-140, these fields are moved to the generic register block.
	//
	union {
	struct {								// General-Control register
	volatile UInt32  m_PE			: 1;	//  Serial EEPROM Program Enable
	volatile UInt32  m_PRE			: 1;	//  Serial EEPROM Protect Register Enable
	volatile UInt32  m_Reset		: 1;	//  Soft reset of DTA 100 circuitry
			 UInt32  m_Reserved1	: 5;	//
			 UInt32  m_FirmwRev		: 8;	//  Firmware Revision
			 UInt32  m_TypeNum		: 8;	//  Board Type Number
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  m_GenControl;

	//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Transmit-Control register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	union {
	struct {								
	volatile UInt32  m_TxMode		: 2;	//  Transmit Mode: 188/204/Add16/Raw
	volatile UInt32  m_BurstOrSdiMode	: 1;	//  ASI: Burst Mode: 1 = send packets in bursts
											//	SDI: Line Mode: 1 = 525 line mode
	volatile UInt32  m_TxModeExt	: 1;	//	Transmit Mode extension: 192
	volatile UInt32  m_PckStuff		: 1;	//  Null-Packet Stuffing On/Off
	volatile UInt32  m_TxCtrl		: 2;	//  Transmit Control: Idle/Hold/Send
	volatile UInt32  m_TxDis		: 1;	//  Disable LVDS outputs
	volatile UInt32  m_PerIntEn		: 1;	//  Periodic-Interrupt Enable
	volatile UInt32  m_UflIntEn		: 1;	//  Output-FIFO Underflow Interrupt Enable
	volatile UInt32  m_SyncIntEn	: 1;	//  Synchronisation-Error Interrupt Enable
	volatile UInt32  m_ShortIntEn	: 1;	//  Short-Circuit-Detected Interrupt Enable
	volatile UInt32  m_ThrIntEn		: 1;	//  Threshold-Crossed Interrupt Enable
			 UInt32  m_Reserved2	: 3;	//
	volatile UInt32  m_UseExtClk	: 1;	//  Use External Clock input
	volatile UInt32  m_EnaPwr		: 1;	//  Enable Power to target adapter
	volatile UInt32  m_ShortErr		: 1;	//  Short-Circuit-Detected Error
	volatile UInt32  m_LedControl	: 1;	//  Take over LED Control
	volatile UInt32  m_LedGreen		: 1;	//  State of Green LED (if LedControl is set)
	volatile UInt32  m_LedRed		: 1;	//  State of Red LED (if LedControl is set)
	volatile UInt32  m_ClrFifo		: 1;	//  Clear FIFO (write only)
	volatile UInt32  m_TxRst		: 1;	//  Reset Transmit Channel (write only)
	volatile UInt32  m_TxInvAsi		: 1;	//  Invert DVB/ASI output signal
	volatile UInt32  m_SdiMode		: 1;	//  SDI mode enable/disbale
	volatile UInt32  m_Sdi10Bit		: 1;	//  SDI 10-bit mode
	volatile UInt32  m_Reserved3	: 3;
	volatile UInt32  m_SdiHuffEn	: 1;	//  Enable Huffman compression 
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  m_TxControl;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Transmit-Status register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// Status register is a union.
	// WATCH OUT: DO NOT clear interrupts by something like p->Fields.m_PerInt = 1;
	// THIS WOULD CLEAR ALL STATUS FLAGS!!
	union {
	struct {
	volatile UInt32  m_FifoFilled	: 1;	//  Output-FIFO Filled flag
			 UInt32  m_Reserved1	: 3;	//
	volatile UInt32  m_SdramSize	: 4;	//  SDRAM Size
	volatile UInt32  m_PerInt		: 1;	//  Periodic Interrupt
	volatile UInt32  m_UflInt		: 1;	//  Output-FIFO Underflow Interrupt
	volatile UInt32  m_SyncInt		: 1;	//  Synchronisation-Error Interrupt
	volatile UInt32  m_ShortInt		: 1;	//  Short-Circuit-Detected Interrupt
	volatile UInt32  m_ThrInt		: 1;	//  Threshold-Crossed Interrupt
	}				 Fields;				// For access to individual field
			 UInt32  All;					// For access to TxStatus in one go
	} m_TxStatus;

	volatile UInt32  m_TxClock;				// Transmit-Clock register

	//-.-.-.-.-.-.-.-.-.-.-.-.-.- Transmit-Control 2 register -.-.-.-.-.-.-.-.-.-.-.-.-.-.

	union {
	struct {
	volatile UInt  m_TxScrSel		: 4;	//  Transmit Output Source Select
	volatile UInt  m_TxLockSel		: 4;	//  Transmit GenLock Channel Select
			 UInt  m_Reserved1		: 24;	//
	}				 Fields;				// For access to individual field
			 UInt32  All;					// For access to TxStatus in one go
	} m_TxControl2;

	volatile UInt32  m_FifoSize;			// FIFO-Size register
	volatile UInt32  m_FifoLoad;			// FIFO-Load register

	union {
	struct {								// Diagnostics register
	volatile UInt32  m_SfData		: 8;	//  Data at output of Smoothing FIFO
	volatile UInt32  m_LoopBack		: 1;	//	Loop-back output of Smoothing FIFO
	volatile UInt32  m_SfDtValid	: 1;	//  Smoothing FIFO Data Vaild
	volatile UInt32  m_BfFull		: 1;	//	Burst-FIFO-Full flag
	volatile UInt32  m_DmaReq		: 1;	//  DMA Request from Burst FIFO flag
	volatile UInt32  m_BfCanBurst	: 1;	//  Burst-FIFO Can-Burst flag
	volatile UInt32  m_SfCanBurst	: 1;	//  Smoothing-FIFO Can-Burst flag
	volatile UInt32  m_SfLoad		: 10;	//  Smoothing-FIFO Load in #bytes
	volatile UInt32  m_BfLoad		: 8;	//  Burst-FIFO Load in #32-bit words
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  m_Diag;

	volatile UInt32  m_LoopBackData;		// Data at output of Smoothing FIFO

	union {
	struct {								// Modulation-Control#3 register
	volatile UInt32  m_SourceHp		: 1;	//  Data Source for High-Priority Stream
	volatile UInt32  m_CodeRateHp	: 3;	//  Code Rate for High-Priority Stream
	volatile UInt32  m_DvbHSignHp	: 2;	//  DVB-H Signalling for High-Priority Stream
		     UInt32  m_Reserved3	: 2;	//
	volatile UInt32  m_SourceLp		: 1;	//  Data Source for Low-Priority Stream
	volatile UInt32  m_CodeRateLp	: 3;	//  Code Rate for Low-Priority Stream
	volatile UInt32  m_DvbHSignLp	: 2;	//  DVB-H Signalling for Low-Priority Stream
		     UInt32  m_Reserved2	: 2;	//
	volatile UInt32  m_HrchMode		: 2;	//  Hierarchical mode
	volatile UInt32  m_GuardItv		: 2;	//  Guard interval 
	volatile UInt32  m_InterlvMode	: 4;	//  Interleaving mode
	volatile UInt32  m_Constell		: 2;	//  Constellation
			 UInt32  m_Reserved1	: 6;	//

	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  m_ModControl3;

	union {
	struct {								// Modulation-Control#1 register
	volatile UInt32  m_IqMap		: 4;	//  I/Q mapping
	volatile UInt32  m_ModType		: 4;	//  Modulation type
	volatile UInt32  m_NoRnd		: 1;	//  Do not randomise payload of null packets
	volatile UInt32  m_IntPolSel	: 3;	//  Interpolator selection
	volatile UInt32  m_RollOff		: 4;	//  Roll-Off factor
	volatile UInt32  m_OutputLvl	: 8;	//  Output level (scaling factor)
	volatile UInt32  m_UserLvl		: 1;	//  Override output level
	volatile UInt32  m_SpecInv		: 1;	//  Spectral inversion
	volatile UInt32  m_MuteI		: 1;	//  Mute I
	volatile UInt32  m_MuteQ		: 1;	//  Mute Q
	volatile UInt32  m_TestPat		: 4;	//  Test pattern
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  m_ModControl1;

	union {
	struct {								// Modulation-Control#2 register
	volatile UInt32  m_OutpLvlOffset: 4;	//  Output level offset (scaling factor offset)
			 UInt32  m_Reserved1	: 28;	//  Reserved bits
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  m_ModControl2;

	union {
	struct {								// Threshold-Control register
	volatile UInt32  m_UpdBusy		: 1;	//  Threshold-Update process busy
	volatile UInt32  m_CmpB			: 1;	//  Output of comparator B
	volatile UInt32  m_CmpA			: 1;	//  Output of comparator A
	volatile UInt32  m_AorB			: 1;	//  Update threshold A or threshold B
			 UInt32  m_Reserved1	: 4;	//
	volatile UInt32  m_ThreshA		: 8;	//  Threshold A
	volatile UInt32  m_ThreshB		: 8;	//  Threshold B
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  m_ThreshControl;

	union {
	struct {								// RF-Control register2
	volatile UInt32  m_AgcSP		: 12;	//  AGC setpoint value
	volatile UInt32  m_OutpAttn		: 6;	//  Output attnuation control
	volatile UInt32  m_AgcEn		: 1;	//  AGC enbale
	volatile UInt32  m_InpAttn		: 4;	//  Input attnuation control
	volatile UInt32  m_LowNoiseEn	: 1;	//  Low Phase Noise Enable
	volatile UInt32  m_ChargePumpCur: 4;	//  Charge-pump current
	volatile UInt32  m_Pll1Lock		: 1;	//  PLL1 Locked Status
	volatile UInt32  m_Pll2Lock		: 1;	//  PLL2 Locked Status
	volatile UInt32  m_Pll3Lock		: 1;	//  PLL3 Locked Status
	volatile UInt32  m_AgcLock		: 1;	//  AGC Locked Status
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  m_RfControl2;

	volatile UInt32  m_FifoData;			// Transport-stream data: 4 bytes at a time

	union {
	struct {								// AD9789 SPI control register
	volatile UInt32  m_SpiReady		: 1;	//  Ready for new spi cycle
	volatile UInt32  m_SpiStart		: 1;	//  Start a SPI cycle (Write-Only bit)
	volatile UInt32  m_SpiNumBytes	: 2;	//  Number of bytes to transfer (0=1;1=2;2=3;3=4)
	volatile UInt32  m_SpiRdOrWrn	: 1;	//  Read ('1') or write ('0') transfer
	         UInt32  m_Reserved		: 11;	//
	volatile UInt32  m_SpiAddr		: 16;	//  SPI ADDRESS
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  m_Ad9789Control;

	volatile UInt32  m_Ad9789Data;			// AD9789 SPI data register

	volatile UInt32  m_Reserved2[14];		// Transport-stream data: 4 bytes at a time
} Dtu2xxTx;

#endif	// #ifdef DTU2XX_DRIVER

// The receive-channel base address
#define DTU2XX_RX_BASE_ADDR				0x0100

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ RX register offset +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DTU2XX_RX_REG_GENCTRL			0x0000
#define DTU2XX_RX_REG_RXCTRL			0x0004
#define DTU2XX_RX_REG_RXSTATUS			0x0008
#define DTU2XX_RX_REG_FIFOLOAD			0x0018
#define DTU2XX_RX_REG_DIAG				0x001C
#define DTU2XX_RX_REG_LOOPBKDATA		0x0020
#define DTU2XX_RX_REG_THRESHCTRL		0x0024
#define DTU2XX_RX_REG_PCKCOUNT			0x0028
#define DTU2XX_RX_REG_CLKCOUNT			0x002C
#define DTU2XX_RX_REG_VALIDCOUNT		0x0030
#define DTU2XX_RX_REG_VIOLCOUNT			0x0034

#define DTU2XX_RX_REG_FIFO_FIRST		0x0040
#define DTU2XX_RX_REG_FIFO_LAST			0x0044

#define DTU2XX_RX_REG_BITRATE			0x004C

#ifdef DTU2XX_DRIVER

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Dtu2xxRx +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//
// Register block for a RECEIVE channel.
//
typedef struct _Dtu2xxRx {

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- General-Control register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
	//
	// On DTA-140, these fields are moved to the generic register block.
	//
	union {
	struct {
	volatile UInt32  m_PE			: 1;	// Serial EEPROM Program Enable
	volatile UInt32  m_PRE			: 1;	// Serial EEPROM Protect Register Enable
	volatile UInt32  m_Reset		: 1;	// Soft reset of DTA 100 circuitry
			 UInt32  m_Reserved1	: 5;	//
			 UInt32  m_FirmwRev		: 8;	// Firmware Revision
			 UInt32  m_TypeNum		: 8;	// Board Type Number
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  m_GenControl;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Receive-Control register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

	union {
	struct {								
	volatile UInt32  m_RxMode		: 2;	//  Receive Mode: St188/St204/StMp2/StRaw
	volatile UInt32  m_AsiInv		: 2;	//  Invert ASI-input control (ASI cards)
	volatile UInt32  m_RxModeExt	: 1;	//  Third RxMode bit
	volatile UInt32  m_RxCtrl		: 1;	//  Receive Control: Idle/Rcv
			 UInt32  m_Reserved2	: 1;	//
	volatile UInt32  m_RxTimeStamp	: 1;	//  Insert Time Stamps
	volatile UInt32  m_PerIntEn		: 1;	//  Periodic-Interrupt Enable
	volatile UInt32  m_OvfIntEn		: 1;	//  Input-FIFO Overflow Interrupt Enable
	volatile UInt32  m_SyncIntEn	: 1;	//  Synchronisation-Error Interrupt Enable
			 UInt32  m_Reserved3	: 1;	//
	volatile UInt32  m_ThrIntEn		: 1;	//  Threshold-Crossed Interrupt Enable
	volatile UInt32  m_RateOvfIntEn	: 1;	//  Input-Rate Overflow Interrupt Enable
			 UInt32  m_Reserved4	: 3;	//
	volatile UInt32  m_EnaPwr		: 1;	//  Enable Power to target adapter
			 UInt32  m_Reserved5	: 1;	//
	volatile UInt32  m_LedControl	: 1;	//  Take over LED Control
	volatile UInt32  m_LedGreen		: 1;	//  State of Green LED (if LedControl is set)
	volatile UInt32  m_LedRed		: 1;	//  State of Red LED (if LedControl is set)
	volatile UInt32  m_RxClrFifo	: 1;	//  Clear Receive FIFO (write only)
	volatile UInt32  m_RxRst		: 1;	//  Reset Receive Channel (write only)
	volatile UInt32  m_SdiMode		: 1;	//  SDI mode enable/disbale
	volatile UInt32  m_Sdi10Bit		: 1;	//  SDI 10-bit mode
	volatile UInt32  m_SdiHuffEn	: 1;	//  SDI huffman compression
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  m_RxControl;

	//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Receive-Status register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
	//
	// Status register is a union.
	// WATCH OUT: DO NOT clear interrupts by something like p->Fields.m_PerInt = 1;
	// THIS WOULD CLEAR ALL STATUS FLAGS!!
	union {
	struct {
	volatile UInt32  m_PckSize		: 2;	//  Packet size: Rx188/Rx204/RxInv
	volatile UInt32  m_NumInv		: 2;	//  Number of invalid bytes per packet
	volatile UInt32  m_SdramSize	: 4;	//  SDRAM Size
	volatile UInt32  m_PerInt		: 1;	//  Periodic Interrupt
	volatile UInt32  m_OvfInt		: 1;	//  Input-FIFO Overflow Interrupt
	volatile UInt32  m_SyncInt		: 1;	//  Synchronisation-Error Interrupt
			 UInt32  m_Reserved1	: 1;	//
	volatile UInt32  m_ThrInt		: 1;	//  Threshold-Crossed Interrupt
	volatile UInt32  m_RateOvfInt	: 1;	//  Input-Rate Overflow Interrupt
	volatile UInt32  m_ClkDet		: 1;	//  Receive-Clock Detected (DTA-120: Asi CD)
	volatile UInt32  m_AsiLock		: 1;	//  DVB/ASI PLL locked to input signal
	volatile UInt32  m_RateOk		: 1;	//  Input rate above minimum input rate
	volatile UInt32  m_AsiInv		: 1;	//  DVB/ASI-Input Invert - Status
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	} m_RxStatus;

	volatile UInt32  m_Reserved[3];			// Not used

	volatile UInt32  m_FifoLoad;			// FIFO-Load register

	union {
	struct {								// Diagnostics register
			 UInt32  m_Reserved1	: 8;	//
	volatile UInt32  m_LoopBack		: 1;	//	Loop-back mode
	volatile UInt32  m_CdcRateOvf	: 1;	//	Clock-Domain-Converter Rate Overflow
	volatile UInt32  m_RfRateOvf	: 1;	//	Receive-FIFO Rate Overflow
	volatile UInt32  m_SdFull		: 1;	//	SDRAM-Full Flag
			 UInt32  m_Reserved2	: 7;	//

	// ONLY FOR: DTA-120, Firmware Revision < 4 (Later revs: Programming register is used)
	volatile UInt32  m_TDO			: 1;	// Status of TDO pin
	volatile UInt32  m_TMS			: 1;	// Control of TMS pin
	volatile UInt32  m_TCK			: 1;	// Control of TCK pin
	volatile UInt32  m_TDI			: 1;	// Control of TDI pin
	volatile UInt32  m_PrgEpc		: 1;	// Enable access to programming i/f of EPC2
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  m_Diag;

	volatile UInt32  m_LbData;				// Data written to Input FIFO (8 sign. bits)

	union {
	struct {								// Threshold-Control register
	volatile UInt32  m_UpdBusy		: 1;	//  Threshold-Update process busy
	volatile UInt32  m_CmpB			: 1;	//  Output of comparator B
	volatile UInt32  m_CmpA			: 1;	//  Output of comparator A
	volatile UInt32  m_AorB			: 1;	//  Update threshold A or threshold B
			 UInt32  m_Reserved1	: 4;	//
	volatile UInt32  m_ThreshA		: 8;	//  Threshold A
	volatile UInt32  m_ThreshB		: 8;	//  Threshold B
	}			     Fields;				//  For access to individual fields
			 UInt32  All;					//  For access to complete register
	}  m_ThreshControl;

	volatile UInt32  m_Reserved2[2];		// Not used

	volatile UInt32  m_ValidCnt;			// Valid Count (DTA-120, DTA-122)
	volatile UInt32  m_CodeViolCnt;			// Code-Violation Count (DTA-120)

	volatile UInt32  m_Reserved3[2];		// Not used

	volatile UInt32  m_FifoData[2];			// Transport-stream data: 4 bytes at a time
	volatile UInt32  m_Bitrate;				// Bit-rate
	volatile UInt32  m_Reserved4[13];		// Not used
} Dtu2xxRx;

#endif // #ifdef DTU2XX_DRIVER

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Ad9789 register offset +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
#define DTU2XX_AD9789_REG_SPICONTROL					0x00
#define DTU2XX_AD9789_REG_SATURATIONCOUNTER				0x01
#define DTU2XX_AD9789_REG_PARITYCOUNTER					0x02
#define DTU2XX_AD9789_REG_INTENA						0x03
#define DTU2XX_AD9789_REG_INTSSTATUSANDCLR				0x04
#define DTU2XX_AD9789_REG_CHANNELENA					0x05
#define DTU2XX_AD9789_REG_BYPASS						0x06
#define DTU2XX_AD9789_REG_QAMSRRCCONF					0x07
#define DTU2XX_AD9789_REG_SUMNODESCALAR					0x08
#define DTU2XX_AD9789_REG_INPUTSCALAR					0x09
#define DTU2XX_AD9789_REG_NCO0FREQTUNEWORD				0x0A//-0x0C
#define DTU2XX_AD9789_REG_NCO1FREQTUNEWORD				0x0D//-0x0F
#define DTU2XX_AD9789_REG_NCO2FREQTUNEWORD				0x10//-0x12
#define DTU2XX_AD9789_REG_NCO3FREQTUNEWORD				0x13//-0x15	
#define DTU2XX_AD9789_REG_RATECONVDENOMQ				0x16//-0x18
#define DTU2XX_AD9789_REG_RATECONVNUMP					0x19//-0x1B
#define DTU2XX_AD9789_REG_INTERPOLBPFCENTERFREQ			0x1C//-0x1D
#define DTU2XX_AD9789_REG_FREQUPDATE					0x1E
#define DTU2XX_AD9789_REG_HWVERSION						0x1F
#define DTU2XX_AD9789_REG_ITFCONF						0x20
#define DTU2XX_AD9789_REG_DATACONTROL					0x21
#define DTU2XX_AD9789_REG_DCOFREQ						0x22
#define DTU2XX_AD9789_REG_INTERNALCLKPHASEADJUST		0x23
#define DTU2XX_AD9789_REG_PARAMETERUPDATE				0x24
#define DTU2XX_AD9789_REG_CHANNEL0GAIN					0x25
#define DTU2XX_AD9789_REG_CHANNEL1GAIN					0x26
#define DTU2XX_AD9789_REG_CHANNEL2GAIN					0x27
#define DTU2XX_AD9789_REG_CHANNEL3GAIN					0x28
#define DTU2XX_AD9789_REG_SPECTERUMSHAPING				0x29
#define DTU2XX_AD9789_REG_MUDELAYCONTROL1				0x2F
#define DTU2XX_AD9789_REG_MUCONTROLDUTYCYCLE			0x30
#define DTU2XX_AD9789_REG_CLKRECEIVER1					0x31
#define DTU2XX_AD9789_REG_CLKRECEIVER2					0x32
#define DTU2XX_AD9789_REG_MUDELAYCONTROL2				0x33
#define DTU2XX_AD9789_REG_RESERVED1						0x34
#define DTU2XX_AD9789_REG_RESERVED2						0x35
#define DTU2XX_AD9789_REG_DACBIAS						0x36
#define DTU2XX_AD9789_REG_RESERVED3						0x37
#define DTU2XX_AD9789_REG_DACDECODER					0x38
#define DTU2XX_AD9789_REG_MUDELAYCONTROL3				0x39//-0x3A
#define DTU2XX_AD9789_REG_RESERVED4						0x3B
#define DTU2XX_AD9789_REG_FULLSCALECURRENT				0x3C//-0x3D
#define DTU2XX_AD9789_REG_PHASEDETECTORCONTROL			0x3E
#define DTU2XX_AD9789_REG_RESERVED5						0x3F
#define DTU2XX_AD9789_REG_BISTCONTROL					0x40
#define DTU2XX_AD9789_REG_BISTSTATUS					0x41
#define DTU2XX_AD9789_REG_BISTZEROPADDINGLENGTH			0x42//-0x43
#define DTU2XX_AD9789_REG_BISTVECTORLENGTH				0x44//-0x46
#define DTU2XX_AD9789_REG_BISTCLOCKADJUST				0x47
#define DTU2XX_AD9789_REG_SIGN0CONTROL					0x48
#define DTU2XX_AD9789_REG_SIGN0CLOCKADJUST				0x49
#define DTU2XX_AD9789_REG_SIGN1CONTROL					0x4A
#define DTU2XX_AD9789_REG_SIGN1CLOCKADJUST				0x4B
#define DTU2XX_AD9789_REG_REGFNL0FREQ					0x4C
#define DTU2XX_AD9789_REG_REGFNL1FREQ					0x4D
#define DTU2XX_AD9789_REG_BISTSIGNATURE0				0x4E//-0x50
#define DTU2XX_AD9789_REG_BISTSIGNATURE1				0x51//-0x53

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Ad9789Registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
// AD9789 SPI register map 
//
typedef struct _Ad9789Registers
{
	union
	{
		struct
		{
			UInt8  m_Mirrored		: 4;
			UInt8  m_LngInst		: 1;
			UInt8  m_Reset			: 1;
			UInt8  m_LsbFirst		: 1;
			UInt8  m_SdioDir		: 1;	
		}          Fields;
		UInt8      All;
	}  m_SpiControl;						// SPI Control Register (0x00)

	UInt8  m_SaturationCounter;				// Saturation Counter Register (0x01)
	UInt8  m_ParityCounter;					// Parity Counter Register (0x02)

	union
	{
		struct
		{
			UInt8  m_Reserved		: 1;
			UInt8  m_SatErr			: 1;
			UInt8  m_LockLost		: 1;
			UInt8  m_LockAcq		: 1;
			UInt8  m_ParmClr		: 1;	
			UInt8  m_ParmSet		: 1;
			UInt8  m_BistDone		: 1;
			UInt8  m_ParErr			: 1;
		}          Fields;
		UInt8      All;
	}  m_InterruptEnable;					// Interrupt Enable Register (0x03)

	union
	{
		struct
		{
			UInt8  m_Reserved		: 1;
			UInt8  m_SatErr			: 1;
			UInt8  m_LockLost		: 1;
			UInt8  m_LockAcq		: 1;
			UInt8  m_ParmClr		: 1;	
			UInt8  m_ParmSet		: 1;
			UInt8  m_BistDone		: 1;
			UInt8  m_ParErr			: 1;
		}          Fields;
		UInt8      All;
	}  m_IntStatusClear;					// Interrupt Status/Clear Register (0x04)

	union
	{
		struct
		{
			UInt8  m_ChanEn0		: 1;
			UInt8  m_ChanEn1		: 1;
			UInt8  m_ChanEn2		: 1;
			UInt8  m_ChanEn3		: 1;
			UInt8  m_Reserved		: 4;
		}          Fields;
		UInt8      All;
	}  m_ChannelEnable;						// Channel Enable Register (0x05)

	union
	{
		struct
		{
			UInt8  m_Int			: 5;
			UInt8  m_Reserved		: 1;
			UInt8  m_Srrc			: 1;
			UInt8  m_Qam			: 1;	
		}          Fields;
		UInt8      All;
	}  m_Bypass;							// Bypass Register (0x06)

	union
	{
		struct
		{
			UInt8  m_Mapping		: 3;
			UInt8  m_Reserved1		: 1;
			UInt8  m_Alpha			: 2;
			UInt8  m_Reserved2		: 2;	
		}          Fields;
		UInt8      All;
	}  m_QamSrrcConfiguration;				// QAM/SRRC Configuration Register (0x07)

	UInt8  m_SummingNodeScalar;				// Summing Node Scalar Register (0x08)
	UInt8  m_InputScalar;					// Input Scalar Register (0x09)

	UInt8  m_Nco0FreqTuningWord[3];			// NCO 0 Frequency Tuning Word Registers (0x0A-0x0C)
	UInt8  m_Nco1FreqTuningWord[3];			// NCO 1 Frequency Tuning Word Registers (0x0D-0x0F)
	UInt8  m_Nco2FreqTuningWord[3];			// NCO 2 Frequency Tuning Word Registers (0x10-0x12)
	UInt8  m_Nco3FreqTuningWord[3];			// NCO 3 Frequency Tuning Word Registers (0x13-0x15)
	
	UInt8  m_RateConverterDenomQ[3];		// Rate Converter Denominator (Q) Registers (0x16-0x18)
	UInt8  m_RateConverterNumP[3];			// Rate Converter Numerator (P) Registers (0x19-0x1B)

	UInt8  m_InterpolatingBpfCenterFreq[2];	// Interpolating BPF Center Frequency Registers (0x1C-0x1D)

	union
	{
		struct
		{
			UInt8  m_Reserved		: 7;
			UInt8  m_FreqNew		: 1;	
		}          Fields;
		UInt8      All;
	}  m_FrequencyUpdate;					// Frequency Update Register (0x1E)

	union
	{
		struct
		{
			UInt8  m_Ver			: 4;
			UInt8  m_Reserved		: 4;
		}          Fields;
		UInt8      All;
	}  m_HardwareVersion;					// Hardware Version Register (0x1F)

	union
	{
		struct
		{
			UInt8  m_Par			: 2;
			UInt8  m_ChanPri		: 1;
			UInt8  m_IfMode			: 1;
			UInt8  m_DcoInv			: 1;
			UInt8  m_Reserved		: 1;
			UInt8  m_CmosCtrl		: 1;
			UInt8  m_CmosBus		: 1;
		}          Fields;
		UInt8      All;
	}  m_Interfaceconfiguration;			// Interface Configuration Register (0x20)

	union
	{
		struct
		{
			UInt8  m_Ltncy			: 3;
			UInt8  m_Cmplx			: 1;
			UInt8  m_DatWdth		: 1;
			UInt8  m_BusWdth		: 2;
			UInt8  m_Bin			: 1;
		}          Fields;
		UInt8      All;
	}  m_DataControl;						// Data Control Register (0x21)

	union
	{
		struct
		{
			UInt8  m_Ones			: 4;
			UInt8  m_DcoDiv			: 3;
			UInt8  m_Reserved		: 1;
		}          Fields;
		UInt8      All;
	}  m_DcoFrequency;						// DCO Frequency Register (0x22)

	union
	{
		struct
		{
			UInt8  m_SncPhz			: 4;
			UInt8  m_DscPhz			: 4;
		}          Fields;
		UInt8      All;
	}  m_InternalClockPhaseAdjust;			// Internal Clock Phase Adjust Register (0x23)

	union
	{
		struct
		{
			UInt8  m_Reserved		: 7;
			UInt8  m_ParmNew		: 1;
		}          Fields;
		UInt8      All;
	}  m_ParameterUpdate;					// Parameter Update Register (0x24)
	
	UInt8  m_Channel0Gain;					// Channel 0 Gain Register (0x25)
	UInt8  m_Channel1Gain;					// Channel 1 Gain Register (0x26)
	UInt8  m_Channel2Gain;					// Channel 2 Gain Register (0x27)
	UInt8  m_Channel3Gain;					// Channel 3 Gain Register (0x28)

	union
	{
		struct
		{
			UInt8  m_SpecInv		: 1;
			UInt8  m_Reserved		: 7;
		}          Fields;
		UInt8      All;
	}  m_SpectrumShaping;					// Spectrum Shaping Register (0x29)

	union
	{
		struct
		{
			UInt8  m_Guardband		: 5;
			UInt8  m_TrackErr		: 1;
			UInt8  m_SearchErr		: 1;
			UInt8  m_SearchTol		: 1;
		}          Fields;
		UInt8      All;
	}  m_MuDelayControl1;					// Mu Delay Control 1 Register (0x2F)

	union
	{
		struct
		{
			UInt8  m_ManualAdj		: 6;
			UInt8  m_IncDec			: 1;
			UInt8  m_DutyCycleCorrEna	: 1;
		}          Fields;
		UInt8      All;
	}  m_MuControlDutyCycle;				// Mu Control Duty Cycle Register (0x30)

	union
	{
		struct
		{
			UInt8  m_Reserved		: 4;
			UInt8  m_ClkNCml		: 4;
		}          Fields;
		UInt8      All;
	}  m_ClockReceiver1;					// Clock Receiver 1 Register (0x31)

	union
	{
		struct
		{
			UInt8  m_NSign			: 1;
			UInt8  m_ClkPCml		: 4;
			UInt8  m_PSign			: 1;
			UInt8  m_Reserved		: 1;
			UInt8  m_ClkDis			: 1;
		}          Fields;
		UInt8      All;
	}  m_ClockReceiver2;					// Clock Receiver 2 Register (0x32)

	union
	{
		struct
		{
			UInt8  m_MuEn			: 1;
			UInt8  m_Gain			: 2;
			UInt8  m_MuSamp			: 1;
			UInt8  m_Mode			: 2;
			UInt8  m_Slope			: 1;
			UInt8  m_MuClkDis		: 1;
		}          Fields;
		UInt8      All;
	}  m_MuDelayControl2;					// Mu Delay Control 2 Register (0x33)

	UInt8  m_Reserved1;
	UInt8  m_Reserved2;

	union
	{
		struct
		{
			UInt8  m_MSel			: 2;
			UInt8  m_Reserved		: 5;
			UInt8  m_PdBias			: 1;
		}          Fields;
		UInt8      All;
	}  m_DacBias;							// DAC Bias Register (0x36)

	UInt8  m_Reserved3;	

	union
	{
		struct
		{
			UInt8  m_DacDecoderMode	: 2;
			UInt8  m_Reserved		: 6;
		}          Fields;
		UInt8      All;
	}  m_DacDecoder;						// DAC Decoder Register (0x38)

	union
	{
		struct
		{
			UInt8  m_MuPhz			: 5;
			UInt8  m_SearchDir		: 2;
			UInt8  m_MuDlyLsb		: 1;
			UInt8  m_MuDlyMsb		: 8;
		}          Fields;
		UInt16      All;
	}  m_MuDelayControl3;					// Mu Delay Control 3&4 Register (0x39-0x3A)

	UInt8  m_Reserved4;	

	union
	{
		struct
		{
			UInt8  m_FscLsb			: 8;
			UInt8  m_FscMsb			: 2;
			UInt8  m_Reserved		: 6;
		}          Fields;
		UInt16     All;
	} m_FullScaleCurrent;					// Full-Scale Current 1&2 Register (0x3C-0x3D)
	
	union
	{
		struct
		{
			UInt8  m_PhzDetBias		: 4;
			UInt8  m_AutoCal		: 1;
			UInt8  m_CmpBst			: 1;
			UInt8  m_Reserved		: 1;
			UInt8  m_PhzPd			: 1;
		}          Fields;
		UInt8      All;
	}  m_PhaseDetectorControl;				// Phase Detector Control Register (0x3E)

	UInt8  m_Reserved5;	
	
	union
	{
		struct
		{
			UInt8  m_BMode			: 4;
			UInt8  m_BEnable		: 1;
			UInt8  m_Reserved		: 1;
			UInt8  m_InputSel		: 1;
			UInt8  m_ClkShdn		: 1;
		}          Fields;
		UInt8      All;
	}  m_BistControl;						// 

	union
	{
		struct
		{
			UInt8  m_BStatus		: 7;
			UInt8  m_BDone			: 1;
		}          Fields;
		UInt8      All;
	}  m_BistStatus;						// 

	UInt8  m_BistZeroPaddingLength[2];		// 
	UInt8  m_BistVectorLength[3];			// 

	union
	{
		struct
		{
			UInt8  m_BClkPhz		: 4;
			UInt8  m_BClkDiv		: 4;
		}          Fields;
		UInt8      All;
	}  m_BistClockAdjust;					// 

	union
	{
		struct
		{
			UInt8  m_S0Sel			: 2;
			UInt8  m_S0FnLch		: 1;
			UInt8  m_S0Neg			: 1;
			UInt8  m_S0Zero			: 1;
			UInt8  m_S0Prng			: 1;
			UInt8  m_S0RdEn			: 1;
			UInt8  m_S0Enabl		: 1;
		}          Fields;
		UInt8      All;
	}  m_Sign0Control;						// 
	
	union
	{
		struct
		{
			UInt8  m_S0ClkPhz		: 4;
			UInt8  m_S0ClkDiv		: 4;
		}          Fields;
		UInt8      All;
	}  m_Sign0ClockAdjust;					// 

	union
	{
		struct
		{
			UInt8  m_S1Sel			: 2;
			UInt8  m_S1FnLch		: 1;
			UInt8  m_S1Neg			: 1;
			UInt8  m_S1Zero			: 1;
			UInt8  m_S1Prng			: 1;
			UInt8  m_S1RdEn			: 1;
			UInt8  m_S1Enabl		: 1;
		}          Fields;
		UInt8      All;
	}  m_Sign1Control;						// 
	
	union
	{
		struct
		{
			UInt8  m_S1ClkPhz		: 4;
			UInt8  m_S1ClkDiv		: 4;
		}          Fields;
		UInt8      All;
	}  m_Sign1ClockAdjust;					// 


	UInt8  m_RegFnl0Freq;					// 
	UInt8  m_RegFnl1Freq;					// 

	UInt8  m_BistSignature0[3];				// 
	UInt8  m_BistSignature1[3];				// 

} Ad9789Registers;

#endif // #ifndef __DTU2XXREGS_H
