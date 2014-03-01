//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtRegs.h *#*#*#*#*#*#*#*#*# (C) 2011-2012 DekTec
//
// DtRegs - Definition of register sets used for all devices DTA/DTU(/DTE) 
// This file contains register offsets with mask and shift defines.

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

#ifndef __DTREGS_H
#define __DTREGS_H

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ General registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Generic
#define DT_GEN_REG_GENCTRL             0x0000
#define DT_GEN_REG_GENSTATUS           0x0004
#define DT_GEN_REG_PROGRAMMING         0x0008
#define DT_GEN_REG_REFCLKCNT           0x000C
#define DT_GEN_REG_EPCSCTRL            0x0010
#define DT_GEN_REG_EPCSDATA            0x0014
#define DT_GEN_REG_REFCLKCNTH          0x0018
#define DT_GEN_REG_PROGRAMMING2        0x001C
#define DT_GEN_REG_SPICTRL             0x0024
#define DT_GEN_REG_SPIDATA             0x0028
#define DT_GEN_REG_CONTROL0            0x0030
#define DT_GEN_REG_CONTROL1            0x0034
#define DT_GEN_REG_STATUS0             0x0038
#define DT_GEN_REG_STATUS1             0x003C
// Genlock specific
#define DT_GEN_REG_VCXOCTRL            0x0040
#define DT_GEN_REG_SDIPOS              0x0044 // latched on periodic int
#define DT_GEN_REG_REFCLKCNT_L         0x0048 // latched on periodic int
#define DT_GEN_REG_REFCLKCNTH_L        0x004C // latched on periodic int
// DMA (DTA) specific
#define DT_RX_REG_DMA_DESC_PTR         0x0044
#define DT_RX_REG_DMA_COMMAND_STAT     0x0048
#define DT_RX_REG_DMA_DAC              0x004C
#define DT_RX_REG_DMA_PCI_ADDR         0x0050
#define DT_RX_REG_DMA_LOCAL_ADDR       0x0054
#define DT_RX_REG_DMA_COUNT            0x0058
#define DT_RX_REG_DMA_NXTDESC_PTR      0x005C
#define DT_RX_REG_DMA_PCI_ADDRH        0x0060
// I2C specific
#define DT_GEN_REG_I2CBASE             0x0050
// Freq measurement
#define DT_GEN_REG_VCXOFREQ            0x0060
#define DT_GEN_REG_EXTCLKFREQ          0x0064
// For DVB-S2 receivers with L3 output
#define DT_GEN_REG_S2_FRAMEMODE        0x0068
#define DT_GEN_REG_S2_L3_SNR           0x006C
// License specific
#define DT_GEN_REG_LICSERIAL           0x0070
#define DT_GEN_REG_LICCODE             0x0074
#define DT_GEN_REG_LICSTATUS1          0x0078
#define DT_GEN_REG_LICSTATUS2          0x007C
// GS2960 receiver specific
#define DT_RX_REG_GS2960SPI            0x0060
#define DT_RX_REG_GS2960CTRL           0x0064
#define DT_RX_REG_GS2960STAT           0x0068
// GS2984 equaliser specific
#define DT_RX_REG_GS2984CTRL           0x0074
#define DT_RX_REG_GS2984STAT           0x0078



//-.-.-.-.-.-.-.-.-.-.-.-.- General-Control register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_GENCTRL_PE_MSK              0x00000001
#define DT_GENCTRL_PE_SH               0
#define DT_GENCTRL_PRE_MSK             0x00000002
#define DT_GENCTRL_PRE_SH              1
#define DT_GENCTRL_RESET_MSK           0x00000004
#define DT_GENCTRL_RESET_SH            2
#define DT_GENCTRL_PERINT_EN_MSK       0x00000010
#define DT_GENCTRL_PERINT_EN_SH        4
#define DT_GENCTRL_PERINTVAL_MSK       0x000000E0
#define DT_GENCTRL_PERINTVAL_SH        5
#define DT_GENCTRL_FWREV_MSK           0x0000FF00
#define DT_GENCTRL_FWREV_SH            8
#define DT_GENCTRL_TYPENUM_MSK         0x00FF0000
#define DT_GENCTRL_TYPENUM_SH          16
#define DT_GENCTRL_LED_CTRL            0x01000000
#define DT_GENCTRL_LED_CTRL_MSK        0x01000000
#define DT_GENCTRL_LED_CTRL_SH         24
#define DT_GENCTRL_LED_GREEN           0x02000000
#define DT_GENCTRL_LED_GREEN_MSK       0x02000000
#define DT_GENCTRL_LED_GREEN_SH        25
#define DT_GENCTRL_LED_RED             0x04000000
#define DT_GENCTRL_LED_RED_MSK         0x04000000
#define DT_GENCTRL_LED_RED_SH          26

//-.-.-.-.-.-.-.-.-.-.-.-.- General Status Register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.
#define DT_GENSTAT_PERINT_MSK          0x00000010
#define DT_GENSTAT_PERINT_SH           4
#define DT_GENSTAT_FIRMWVAR_MSK        0x00001F00
#define DT_GENSTAT_FIRMWVAR_SH         8
#define DT_GENSTAT_TYPEEXT_MSK         0x00070000
#define DT_GENSTAT_TYPEEXT_SH          16

//.-.-.-.-.-.-.-.-.-.-.-.- Programming Register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.
#define DT_PROG_TRST_MSK               0x00000001
#define DT_PROG_TRST_SH                0
#define DT_PROG_TDO_MSK                0x00000002
#define DT_PROG_TDO_SH                 1
#define DT_PROG_TMS_MSK                0x00000004
#define DT_PROG_TMS_SH                 2
#define DT_PROG_TCK_MSK                0x00000008
#define DT_PROG_TCK_SH                 3
#define DT_PROG_TDI_MSK                0x00000010
#define DT_PROG_TDI_SH                 4
#define DT_PROG_PRGEPC_MSK             0x00000020
#define DT_PROG_PRGEPC_SH              5
#define DT_PROG_BUSY_MSK               0x00000040
#define DT_PROG_BUSY_SH                6
#define DT_PROG_JTAGCNT_MSK            0x00003F00
#define DT_PROG_JTAGCNT_SH             8

//.-.-.-.-.-.-.-.-.-.-.-.-.- EPCS Control Register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_EPCSCTRL_ERASE_MSK          0x00000001
#define DT_EPCSCTRL_ERASE_SH           0
#define DT_EPCSCTRL_PROG_MSK           0x00000002
#define DT_EPCSCTRL_PROG_SH            1
#define DT_EPCSCTRL_READ_MSK           0x00000004
#define DT_EPCSCTRL_READ_SH            2
#define DT_EPCSCTRL_BUSY_MSK           0x00000080
#define DT_EPCSCTRL_BUSY_SH            3
#define DT_EPCSCTRL_ADDR_LSB_MSK       0x0FFFFF00
#define DT_EPCSCTRL_ADDR_LSB_SH        8
#define DT_EPCSCTRL_ADDR_LSB_NBITS     20
#define DT_EPCSCTRL_ADDR_MSB_MSK       0x00000078
#define DT_EPCSCTRL_ADDR_MSB_SH        3
#define DT_EPCSCTRL_UNLCK_MSK          0xF0000000
#define DT_EPCSCTRL_UNLCK_SH           28

//.-.-.-.-.-.-.-.-.-.-.-.- SPI Control Register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.
#define DT_SPICTRL_PROG_MSK            0x00000002
#define DT_SPICTRL_PROG_SH             1
#define DT_SPICTRL_READ_MSK            0x00000004
#define DT_SPICTRL_READ_SH             2
#define DT_SPICTRL_BUSY_MSK            0x00000080
#define DT_SPICTRL_BUSY_SH             3
#define DT_SPICTRL_ADDR_MSK            0x00FFFF00
#define DT_SPICTRL_ADDR_SH             8
#define DT_SPICTRL_UNLCK_MSK           0xF0000000
#define DT_SPICTRL_UNLCK_SH            28

//.-.-.-.-.-.-.-.-.-.-.-.- General Control0 Register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_GEN_CONTROL0_DBLBUF_MSK     0x00000001
#define DT_GEN_CONTROL0_DBLBUF_SH      0
#define DT_GEN_CONTROL0_DEMOD_MSK      0x00000001     // DTU-236 DEMOD/ASI mode
#define DT_GEN_CONTROL0_DEMOD_SH       0
#define DT_GEN_CONTROL0_VCOEN_MSK      0x00000002
#define DT_GEN_CONTROL0_VCOEN_SH       1
#define DT_GEN_CONTROL0_PWREN_MSK      0x00000030     // DTA-2111 specific Vdd
#define DT_GEN_CONTROL0_PWREN_SH       4
#define DT_GEN_CONTROL0_DVRTY_MSK      0x00000100     // DTA-2135 specific
#define DT_GEN_CONTROL0_DVRTY_SH       8

//.-.-.-.-.-.-.-.-.-.-.-.- General Control1 Register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_GEN_CONTROL1_WATCHDOG_MSK   0x00000001
#define DT_GEN_CONTROL1_WATCHDOG_SH    0

//.-.-.-.-.-.-.-.-.-.-.-.-.- VCXO Control Register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_VCXOCTRL_EN_MSK             0x00000001
#define DT_VCXOCTRL_EN_SH              0
#define DT_VCXOCTRL_VIDSTD_MSK         0x00000002
#define DT_VCXOCTRL_VIDSTD_SH          1
#define DT_VCXOCTRL_SPEED_MSK          0x0003FF00
#define DT_VCXOCTRL_SPEED_SH           8

//-.-.-.-.-.-.-.-.-.-.-.- Transmit SDI Position: Bit fields -.-.-.-.-.-.-.-.-.-.-.-
#define DT_TXSDIPOS_SAMP_MSK           0x7FF
#define DT_TXSDIPOS_SAMP_SH            0
#define DT_TXSDIPOS_LINE_MSK           (0x3FF<<11)
#define DT_TXSDIPOS_LINE_SH            11
#define DT_TXSDIPOS_FRAME_MSK          (0x7FF<<21)
#define DT_TXSDIPOS_FRAME_SH           21

//.-.-.-.-.-.-.-.-.-.-.-.-.- S2 Framemode register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_S2_FRAMEMODE_MSK            0x0000000F
#define DT_S2_FRAMEMODE_SH             0

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ I2C registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_GEN_REG_I2CCTRL             0x0000
#define DT_GEN_REG_I2CSTAT             0x0004
#define DT_GEN_REG_I2CADDRCNT          0x0008
#define DT_GEN_REG_I2CDATA             0x000C

//.-.-.-.-.-.-.-.-.-.-.-.-.- I2C Control Registers: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_I2CCTRL_RDYINT_EN_MSK       0x00000001
#define DT_I2CCTRL_RDYINT_EN_SH        0
#define DT_I2CCTRL_BUSSEL_MSK          0x000001E0
#define DT_I2CCTRL_BUSSEL_SH           5
#define DT_I2CCTRL_CLKDIV_MSK          0x0FFF0000
#define DT_I2CCTRL_CLKDIV_SH           16

//-.-.-.-.-.-.-.-.-.-.-.-.-.- I2C Status Registers: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_I2CSTAT_RDYINT_MSK          0x00000001
#define DT_I2CSTAT_RDYINT_SH           0
#define DT_I2CSTAT_WRADDR_NACK_MSK     0x00000002
#define DT_I2CSTAT_WRADDR_NACK_SH      1
#define DT_I2CSTAT_WRDATA_NACK_MSK     0x00000004
#define DT_I2CSTAT_WRDATA_NACK_SH      2
#define DT_I2CSTAT_RDADDR_NACK_MSK     0x00000008
#define DT_I2CSTAT_RDADDR_NACK_SH      3
#define DT_I2CSTAT_TIMEOUT_MSK         0x00000010
#define DT_I2CSTAT_TIMEOUT_SH          4

//-.-.-.-.-.-.-.-.-.-.-.- I2C Address/Count Registers: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.
#define DT_I2CADDRCNT_WRADDR_MSK       0x0000007F
#define DT_I2CADDRCNT_WRADDR_SH        0
#define DT_I2CADDRCNT_WRCNT_MSK        0x0000FF80
#define DT_I2CADDRCNT_WRCNT_SH         7
#define DT_I2CADDRCNT_RDADDR_MSK       0x007F0000
#define DT_I2CADDRCNT_RDADDR_SH        16
#define DT_I2CADDRCNT_RDCNT_MSK        0xFF800000
#define DT_I2CADDRCNT_RDCNT_SH         23

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ HD Genlock registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// NOTE: Offsets relative to general register base address
#define DT_HDGENL_REG_CLKCONF           0x0080
#define DT_HDGENL_REG_CTRL              0x0084
#define DT_HDGENL_REG_STATUS            0x0088
#define DT_HDGENL_REG_LEDCTRL           0x008C
#define DT_HDGENL_REG_REFFRM_LSB        0x0090
#define DT_HDGENL_REG_REFFRM_MSB        0x0094
#define DT_HDGENL_REG_SOFFRM_LSB        0x0098
#define DT_HDGENL_REG_SOFFRM_MSB        0x009C

//.-.-.-.-.-.-.-.-.-.-.- Genlock Clock Config register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.-

#define DT_HDGENL_CLKCONF_REFSRC_MSK    0x0000000F
#define DT_HDGENL_CLKCONF_REFSRC_SH     0
#define DT_HDGENL_CLKCONF_ASYNCRST_MSK  0x00000010
#define DT_HDGENL_CLKCONF_ASYNCRST_SH   4
#define DT_HDGENL_CLKCONF_INTERL_MSK    0x00000020
#define DT_HDGENL_CLKCONF_INTERL_SH     5

//-.-.-.-.-.-.-.-.-.-.-.-.- Genlock Control register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.-.-

#define DT_HDGENL_CTRL_REPFRM_MSK       0x00000001
#define DT_HDGENL_CTRL_REPFRM_SH        0
#define DT_HDGENL_CTRL_LOCKINTEN_MSK    0x20000000
#define DT_HDGENL_CTRL_LOCKINTEN_SH     29
#define DT_HDGENL_CTRL_UNLOCKINTEN_MSK  0x40000000
#define DT_HDGENL_CTRL_UNLOCKINTEN_SH   30
#define DT_HDGENL_CTRL_SOFINTEN_MSK     0x80000000
#define DT_HDGENL_CTRL_SOFINTEN_SH      31

//-.-.-.-.-.-.-.-.-.-.-.-.- Genlock Status register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.-.-.

#define DT_HDGENL_STATUS_REPFRMCNT_MSK  0x0000FFFF
#define DT_HDGENL_STATUS_REPFRMCNT_SH   0
#define DT_HDGENL_STATUS_LOCKINT_MSK    0x20000000
#define DT_HDGENL_STATUS_LOCKINT_SH     29
#define DT_HDGENL_STATUS_UNLOCKINT_MSK  0x40000000
#define DT_HDGENL_STATUS_UNLOCKINT_SH   30
#define DT_HDGENL_STATUS_SOFINT_MSK     0x80000000
#define DT_HDGENL_STATUS_SOFINT_SH      31

//-.-.-.-.-.-.-.-.-.-.-.- Genlock LED Control register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.-

#define DT_HDGENL_LEDCTRL_LEDCTRL_MSK   0x00000001
#define DT_HDGENL_LEDCTRL_LEDCTRL_SH    0
#define DT_HDGENL_LEDCTRL_LEDGREEN_MSK  0x00000002
#define DT_HDGENL_LEDCTRL_LEDGREEN_SH   1
#define DT_HDGENL_LEDCTRL_LEDRED_MSK    0x00000004
#define DT_HDGENL_LEDCTRL_LEDRED_SH     2
#define DT_HDGENL_LEDCTRL_LEDBLUE_MSK   0x00000008
#define DT_HDGENL_LEDCTRL_LEDBLUE_SH    3
#define DT_HDGENL_LEDCTRL_PATTERN_MSK   0x000000F0
#define DT_HDGENL_LEDCTRL_PATTERN_SH    4

//-.-.-.-.-.-.-.-.-.-.- Genlock Reference Frame register: Bit Fields -.-.-.-.-.-.-.-.-.-.-

#define DT_HDGENL_REFFRMLSB_MSK         0xFFFFFFFF
#define DT_HDGENL_REFFRMLSB_SH          0
#define DT_HDGENL_REFFRMMSB_MSK         0xFFFFFFFF
#define DT_HDGENL_REFFRMMSB_SH          0

//.-.-.-.-.-.-.-.-.-.-.-.- Genlock SOF Frame register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.-.

#define DT_HDGENL_SOFFRMLSB_MSK         0xFFFFFFFF
#define DT_HDGENL_SOFFRMLSB_SH          0
#define DT_HDGENL_SOFFRMMSB_MSK         0xFFFFFFFF
#define DT_HDGENL_SOFFRMMSB_SH          0

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DVB SPI registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_SPI_REG_SPICTRL             0x0000
#define DT_SPI_REG_SPISTATUS           0x0004
#define DT_SPI_REG_SPIDDSDATA          0x0008
#define DT_SPI_REG_SPITESTDATA         0x000C
#define DT_SPI_REG_SPIPRNGERRORS       0x0010
#define DT_SPI_REG_SPIRXFREQ           0x0014

//.-.-.-.-.-.-.-.-.-.-.-.- SPI Control register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.
#define DT_SPICTRL_IO_EN_MSK           0x00000001
#define DT_SPICTRL_IO_EN_SH            0
#define DT_SPICTRL_LVTTL_SEL_MSK       0x00000002
#define DT_SPICTRL_LVTTL_SEL_SH        1
#define DT_SPICTRL_FAILSAFE_EN_MSK     0x00000004
#define DT_SPICTRL_FAILSAFE_EN_SH      2
#define DT_SPICTRL_DDSIOUPDATE_MSK     0x00000008
#define DT_SPICTRL_DDSIOUPDATE_SH      3
#define DT_SPICTRL_SERIALMODE_MSK      0x00000010
#define DT_SPICTRL_SERIALMODE_SH       4
#define DT_SPICTRL_EXTCLK_SEL_MSK      0x00000020
#define DT_SPICTRL_EXTCLK_SEL_SH       5
#define DT_SPICTRL_TESTMODE_MSK        0x00000040
#define DT_SPICTRL_TESTMODE_SH         6
#define DT_SPICTRL_PRNGMODE_MSK        0x00000080
#define DT_SPICTRL_PRNGMODE_SH         7
#define DT_SPICTRL_MODE10B_MSK         0x00000100
#define DT_SPICTRL_MODE10B_SH          8

//-.-.-.-.-.-.-.-.-.-.-.-.-.- SPI Status register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DT_SPISTATUS_DDSBUSY_MSK       0x00000001
#define DT_SPISTATUS_DDSBUSY_SH        0
#define DT_SPISTATUS_IOERROR_MSK       0x00000002
#define DT_SPISTATUS_IOERROR_SH        1

//.-.-.-.-.-.-.-.-.-.-.-.-.- SPI DDS data register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_SPIDDSDATA_DDSDATA_MSK      0x000000FF
#define DT_SPIDDSDATA_DDSDATA_SH       0

//.-.-.-.-.-.-.-.-.-.-.-.-.- SPI Test data register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.
#define DT_SPITESTDATA_TESTDATA_MSK    0x000003FF
#define DT_SPITESTDATA_TESTDATA_SH     0

//-.-.-.-.-.-.-.-.-.-.-.-.- SPI PRNG Errors register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_SPIPRNGERRORS_ERRORS_MSK    0x0000FFFF
#define DT_SPIPRNGERRORS_ERRORS_SH     0

//-.-.-.-.-.-.-.-.-.-.-.-.- SPI Rx Frequency register:Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_SPIRXFREQ_FREQUENCY_MSK     0xFFFFFFFF
#define DT_SPIRXFREQ_FREQUENCY_SH      0


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ TX registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- TX Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Generic
#define DT_TX_REG_GENCTRL              0x0000
#define DT_TX_REG_TXCTRL               0x0004
#define DT_TX_REG_TXSTATUS             0x0008
#define DT_TX_REG_TXCLOCK              0x000C
#define DT_TX_REG_CLOCKCNT             0x0010
#define DT_TX_REG_TXCTRL2              0x0010
#define DT_TX_REG_FIFOSIZE             0x0014
#define DT_TX_REG_FIFOLOAD             0x0018
#define DT_TX_REG_DIAG                 0x001C
#define DT_TX_REG_LOOPBKDATA           0x0020
#define DT_TX_REG_THRESHCTRL           0x0024
#define DT_TX_REG_MOD_CONTROL3         0x0024 // Reserved for future use
#define DT_TX_REG_MOD_CONTROL1         0x0028
#define DT_TX_REG_MOD_CONTROL2         0x002C // for modulator channel
#define DT_TX_REG_TXCLOCK_MOD          0x002C // for ASI/SDI channel
#define DT_TX_REG_IDAC_CONTROL         0x0030
#define DT_TX_REG_QDAC_CONTROL         0x0034
#define DT_TX_REG_RF_CONTROL           0x0038  // DTA-107 and DTA-110
#define DT_TX_REG_RF_CONTROL1          0x0038
#define DT_TX_REG_IF_CONTROL           0x0038
#define DT_TX_REG_RF_CONTROL2          0x003C
#define DT_TX_REG_RF_CONTROL3          0x0040
#define DT_TX_REG_FIFO_FIRST           0x0040
#define DT_TX_REG_RFDAC_CONTROL        0x0050
#define DT_TX_REG_RFDAC_DATA           0x0054
#define DT_TX_REG_IQMAP_CONTROL        0x0044
#define DT_TX_REG_IQMAP_DATA           0x0048
#define DT_TX_REG_FILTER_CONTROL       0x004C
#define DT_TX_REG_FILTER_DATA          0x0050 
#define DT_TX_REG_DDS_CONTROL          0x0054
#define DT_TX_REG_DDS_DATA             0x0058
#define DT_TX_REG_SATUR_COUNT          0x005C
#define DT_TX_REG_FIFO_LAST            0x007C

//.-.-.-.-.-.-.-.-.-.-.-.-  Transmit-Control register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.
#define DT_TXCTRL_TXMODE_MSK           0x00000003
#define DT_TXCTRL_TXMODE_SH            0
#define DT_TXCTRL_BURSTMODE_MSK        0x00000004  // ASI: Bust mode 1 = burst
#define DT_TXCTRL_BURSTMODE_SH         2
#define DT_TXCTRL_SDI_LINEMODE_MSK     0x00000004  // SDI: Line mode 1 = 525 line mode
#define DT_TXCTRL_SDI_LINEMODE_SH      2
#define DT_TXCTRL_TXMODE_EXT_MSK       0x00000008
#define DT_TXCTRL_TXMODE_EXT_SH        3
#define DT_TXCTRL_PCKSTUFF_MSK         0x00000010
#define DT_TXCTRL_PCKSTUFF_SH          4
#define DT_TXCTRL_TXCTRL_MSK           0x00000060
#define DT_TXCTRL_TXCTRL_SH            5
#define DT_TXCTRL_TXDIS_MSK            0x00000080
#define DT_TXCTRL_TXDIS_SH             7
#define DT_TXCTRL_PERINT_EN_MSK        0x00000100
#define DT_TXCTRL_PERINT_EN_SH         8
#define DT_TXCTRL_UFLINT_EN_MSK        0x00000200
#define DT_TXCTRL_UFLINT_EN_SH         9
#define DT_TXCTRL_SYNCINT_EN_MSK       0x00000400
#define DT_TXCTRL_SYNCINT_EN_SH        10
#define DT_TXCTRL_SHORTINT_EN_MSK      0x00000800
#define DT_TXCTRL_SHORTINT_EN_SH       11
#define DT_TXCTRL_THRINT_EN_MSK        0x00001000
#define DT_TXCTRL_THRINT_EN_SH         12
#define DT_TXCTRL_FRAMEINT_EN_MSK      0x00002000
#define DT_TXCTRL_FRAMEINT_EN_SH       13
#define DT_TXCTRL_USE_EXTCLOCK_MSK     0x00010000
#define DT_TXCTRL_USE_EXTCLOCK_SH      16
#define DT_TXCTRL_ENAPWR_MSK           0x00020000
#define DT_TXCTRL_ENAPWR_SH            17
#define DT_TXCTRL_SHORTERROR_MSK       0x00040000
#define DT_TXCTRL_SHORTERROR_SH        18
#define DT_TXCTRL_LED_CTRL             0x00080000
#define DT_TXCTRL_LED_CTRL_MSK         0x00080000
#define DT_TXCTRL_LED_CTRL_SH          19
#define DT_TXCTRL_LED_GREEN            0x00100000
#define DT_TXCTRL_LED_GREEN_MSK        0x00100000
#define DT_TXCTRL_LED_GREEN_SH         20
#define DT_TXCTRL_LED_RED              0x00200000
#define DT_TXCTRL_LED_RED_MSK          0x00200000
#define DT_TXCTRL_LED_RED_SH           21
#define DT_TXCTRL_FIFO_CLR_MSK         0x00400000
#define DT_TXCTRL_FIFO_CLR_SH          22
#define DT_TXCTRL_TXRESET_MSK          0x00800000
#define DT_TXCTRL_TXRESET_SH           23
#define DT_TXCTRL_ASIINV_MSK           0x01000000
#define DT_TXCTRL_ASIINV_SH            24
#define DT_TXCTRL_SDIMODE_MSK          0x02000000
#define DT_TXCTRL_SDIMODE_SH           25
#define DT_TXCTRL_SDI10BIT_MSK         0x04000000
#define DT_TXCTRL_SDI10BIT_SH          26
#define DT_TXCTRL_OUT_EN_MSK           0x08000000
#define DT_TXCTRL_OUT_EN_SH            27
#define DT_TXCTRL_TXONTIME_MSK         0x10000000
#define DT_TXCTRL_TXONTIME_SH          28
#define DT_TXCTRL_LOCK2INP_MSK         0x20000000
#define DT_TXCTRL_LOCK2INP_SH          29
#define DT_TXCTRL_SDIHUFF_MSK          0x40000000
#define DT_TXCTRL_SDIHUFF_SH           30
#define DT_TXCTRL_TXTSREL_MSK          0x80000000
#define DT_TXCTRL_TXTSREL_SH           31

// Tx Control: TxCtrl
#define DT_TXCTRL_IDLE                 0
#define DT_TXCTRL_HOLD                 1
#define DT_TXCTRL_NULL                 2
#define DT_TXCTRL_SEND                 3

// Tx Control: TxMode ASI
#define DT_TXMODE_188                  0
#define DT_TXMODE_204                  1
#define DT_TXMODE_ADD16                2
#define DT_TXMODE_RAW                  3
#define DT_TXMODE_192                  4
#define DT_TXMODE_RAWASI               5   // 10-bit ASI
#define DT_TXMODE_MIN16                6

// Tx Control: TxMode SDI
#define DT_TXMODE_SDI_FULL             0
#define DT_TXMODE_SDI_ACTVID           1

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Tx Status register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DT_TXSTAT_FIFOFILLED_MSK       0x00000001
#define DT_TXSTAT_FIFOFILLED_SH        0
#define DT_TXSTAT_SDRAMSIZE_MSK        0x000000F0
#define DT_TXSTAT_SDRAMSIZE_SH         4
#define DT_TXSTAT_PERINT_MSK           0x00000100
#define DT_TXSTAT_PERINT_SH            8
#define DT_TXSTAT_UFLINT_MSK           0x00000200
#define DT_TXSTAT_UFLINT_SH            9
#define DT_TXSTAT_SYNCINT_MSK          0x00000400
#define DT_TXSTAT_SYNCINT_SH           10
#define DT_TXSTAT_SHORTINT_MSK         0x00000800
#define DT_TXSTAT_SHORTINT_SH          11
#define DT_TXSTAT_THRINT_MSK           0x00001000
#define DT_TXSTAT_THRINT_SH            12

//-.-.-.-.-.-.-.-.-.-.-.-.- Tx Diagnostics register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.
#define DT_TXDIAG_SFDATA_MSK           0x000000FF
#define DT_TXDIAG_SFDATA_SH            0
#define DT_TXDIAG_LOOPBACK_MSK         0x00000100
#define DT_TXDIAG_LOOPBACK_SH          8
#define DT_TXDIAG_SFDTVALID_MSK        0x00000200
#define DT_TXDIAG_SFDTVALID_SH         9
#define DT_TXDIAG_BFFULL_MSK           0x00000400
#define DT_TXDIAG_BFFULL_SH            10
#define DT_TXDIAG_DMAREQ_MSK           0x00000800
#define DT_TXDIAG_DMAREQ_SH            11
#define DT_TXDIAG_BFCANBURST_MSK       0x00001000
#define DT_TXDIAG_BFCANBURST_SH        12
#define DT_TXDIAG_SFCANBURST_MSK       0x00002000
#define DT_TXDIAG_SFCANBURST_SH        13
#define DT_TXDIAG_SFLOAD_MSK           0x00FC0000
#define DT_TXDIAG_SFLOAD_SH            18
#define DT_TXDIAG_BFLOAD_MSK           0xFF000000
#define DT_TXDIAG_BFLOAD_SH            24

//-.-.-.-.-.-.-.-.-.-.-.-.- ThresHold Ctrl register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.
#define DT_THRESHCTRL_UPDBUSY_MSK      0x00000001
#define DT_THRESHCTRL_UPDBUSY_SH       0
#define DT_THRESHCTRL_CMPB_MSK         0x00000002
#define DT_THRESHCTRL_CMPB_SH          1
#define DT_THRESHCTRL_CMPA_MSK         0x00000004
#define DT_THRESHCTRL_CMPA_SH          2
#define DT_THRESHCTRL_AORB_MSK         0x00000008
#define DT_THRESHCTRL_AORB_SH          3
#define DT_THRESHCTRL_THRESHA_MSK      0x0000FF00
#define DT_THRESHCTRL_THRESHA_SH       8
#define DT_THRESHCTRL_THRESHB_MSK      0x00FF0000
#define DT_THRESHCTRL_THRESHB_SH       16

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Tx Control2 register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_TXCTRL2_SRCSEL_MSK          0x0000000F
#define DT_TXCTRL2_SRCSEL_SH           0
#define DT_TXCTRL2_LOCKSEL_MSK         0x000000F0
#define DT_TXCTRL2_LOCKSEL_SH          4
#define DT_TXCTRL2_RATESEL_MSK         0x00000100
#define DT_TXCTRL2_RATESEL_SH          8
#define DT_TXCTRL2_S2ISI_MSK           0x0000FF00
#define DT_TXCTRL2_S2ISI_SH            8
#define DT_TXCTRL2_S2LOOPMODE_MSK      0x00010000
#define DT_TXCTRL2_S2LOOPMODE_SH       16
#define DT_TXCTRL2_DIRMODE_MSK         0x00020000
#define DT_TXCTRL2_DIRMODE_SH          17

//-.-.-.-.-.-.-.-.-.-.-.- Modulation-Control1 register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-
#define DT_TXMODC1_M_MSK               0x0000000F
#define DT_TXMODC1_M_SH                0
#define DT_TXMODC1_IQMAP_MSK           0x0000000F
#define DT_TXMODC1_IQMAP_SH            0
#define DT_TXMODC1_MODTYPE_MSK         0x000000F0
#define DT_TXMODC1_MODTYPE_SH          4
#define DT_TXMODC1_CODERATE_MSK        0x00000F00
#define DT_TXMODC1_CODERATE_SH         8
#define DT_TXMODC1_INTPOL_MSK          0x00000E00
#define DT_TXMODC1_INTPOL_SH           9
#define DT_TXMODC1_ROLLOFF_MSK         0x0000F000
#define DT_TXMODC1_ROLLOFF_SH          12
#define DT_TXMODC1_OUTLVL_MSK          0x00FF0000
#define DT_TXMODC1_OUTLVL_SH           16
#define DT_TXMODC1_USER_LEVEL_MSK      0x01000000
#define DT_TXMODC1_USER_LEVEL_SH       24
#define DT_TXMODC1_SPEC_INV_MSK        0x02000000
#define DT_TXMODC1_SPEC_INV_SH         25
#define DT_TXMODC1_MUTE_I_MSK          0x04000000
#define DT_TXMODC1_MUTE_I_SH           26
#define DT_TXMODC1_MUTE_Q_MSK          0x08000000
#define DT_TXMODC1_MUTE_Q_SH           27
#define DT_TXMODC1_TESTPAT_MSK         0xF0000000
#define DT_TXMODC1_TESTPAT_SH          28

// Modulation-Type field: values
#define DT_TXMODC_QPSK                 0
#define DT_TXMODC_BPSK                 1   // Not supported
#define DT_TXMODC_QAM4                 3
#define DT_TXMODC_QAM16                4
#define DT_TXMODC_QAM32                5
#define DT_TXMODC_QAM64                6
#define DT_TXMODC_QAM128               7
#define DT_TXMODC_QAM256               8
#define DT_TXMODC_IQMAP                9   // I/Q Mapping
// IQ Mapping
#define DT_TXMODC_SYMB8                8   // 8-bit symbol mode
#define DT_TXMODC_SYMB8E               9   // 8-bit extended symbol mode
#define DT_TXMODC_SYMB4                10  // 4-bit symbol mode
#define DT_TXMODC_IQDIRBUF             14  // Direct I/Q, buffered
#define DT_TXMODC_IQDIRECT             15  // Direct I/Q
#define DT_TXMODC_IQDIRPCK             15  // Direct I/Q packed (DTA-2107)

// I/Q Mapping field: values
#define DT_IQMAP_QAM                   0   // General QAM I/Q mapping
#define DT_IQMAP_QAM16                 1   // Optimised I/Q mapping for 16-QAM
#define DT_IQMAP_QAM32                 2   // Optimised I/Q mapping for 32-QAM
#define DT_IQMAP_QAM64                 3   // Optimised I/Q mapping for 64-QAM
#define DT_IQMAP_VSB8                  4
#define DT_IQMAP_VSB16                 5

// DVB-S and DVB-S.2 code rates
#define DT_CODERATE_1_2                0x0 // Code rate 1/2
#define DT_CODERATE_2_3                0x1 // Code rate 2/3
#define DT_CODERATE_3_4                0x2 // Code rate 3/4
#define DT_CODERATE_4_5                0x3 // Code rate 4/5
#define DT_CODERATE_5_6                0x4 // Code rate 5/6
#define DT_CODERATE_6_7                0x5 // Code rate 6/7
#define DT_CODERATE_7_8                0x6 // Code rate 7/8
#define DT_CODERATE_1_4                0x7 // Code rate 1/4
#define DT_CODERATE_1_3                0x8 // Code rate 1/3
#define DT_CODERATE_2_5                0x9 // Code rate 2/5
#define DT_CODERATE_3_5                0xA // Code rate 3/5
#define DT_CODERATE_8_9                0xB // Code rate 8/9
#define DT_CODERATE_9_10               0xC // Code rate 9/10

// Roll-Off factor field: values
#define DT_ROLLOFF_12                  0
#define DT_ROLLOFF_13                  1
#define DT_ROLLOFF_15                  2
#define DT_ROLLOFF_18                  3
#define DT_ROLLOFF_5                   4

// Roll-Off factor field: values
#define DT_INTPOL_QAM                  0  // Use interpolator optimised for n-QAM
#define DT_INTPOL_OFDM                 1  // Use interpolator optimised for OFDM

// Test patterns
#define DT_TP_NORMAL                   0  // Normal mode
#define DT_TP_NYQUIST                  1  // Nyquist test-pattern
#define DT_TP_HALFNYQ                  2  // Half-nyquist tets-pattern
#define DT_TP_CW                       3  // Carrier only
#define DT_TP_CWI                      4  // Constant I
#define DT_TP_CWQ                      5  // Constant Q

//-.-.-.-.-.-.-.-.-.-.- Tx Modulation-Control 2 register: Bit fields -.-.-.-.-.-.-.-.-.-.-
#define DT_TXMODC2_OUTLVL_OFFSET_MSK   0x0000000F
#define DT_TXMODC2_OUTLVL_OFFSET_SH    0
#define DT_TXMODC2_NOISELVL_MSK        0x00000FF0
#define DT_TXMODC2_NOISELVL_SH         4
#define DT_TXMODC2_NOISERNG_MSK        0x00007000
#define DT_TXMODC2_NOISERNG_SH         12

//.-.-.-.-.-.-.-.-.-.- Tx I- and Q-DAC-Control registers: Bit Fields -.-.-.-.-.-.-.-.-.-.-
#define DT_TXDACC_FINEGAIN_MSK         0x000000FF
#define DT_TXDACC_FINEGAIN_SH          0
#define DT_TXDACC_OFFSDIR_MSK          0x00000100
#define DT_TXDACC_OFFSDIR_SH           8
#define DT_TXDACC_OFFSET_MSK           0x0007FE00
#define DT_TXDACC_OFFSET_SH            9

//-.-.-.-.-.-.-.-.-.-.-.-.- Tx RF-Control  register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.
#define DT_TXRFCTRL_PLLA_MSK           0x0000001F
#define DT_TXRFCTRL_PLLA_SH            0
#define DT_TXRFCTRL_PLLB_MSK           0x00003FE0
#define DT_TXRFCTRL_PLLB_SH            5
#define DT_TXRFCTRL_PLL_LOCK_MSK       0x00020000
#define DT_TXRFCTRL_PLL_LOCK_SH        17
#define DT_TXRFCTRL_PLLB9_MSK          0x00040000
#define DT_TXRFCTRL_PLLB9_SH           18
#define DT_TXRFCTRL_PLLR_MSK           0xFFF80000
#define DT_TXRFCTRL_PLLR_SH            19
// Tx RF-Control 1 register 1: Bit fields (for DTA-115)
#define DT_TXRFCTRL1_MOD_MSK           0x00000FFF
#define DT_TXRFCTRL1_MOD_SH            0
#define DT_TXRFCTRL1_FRAC_MSK          0x00FFF000
#define DT_TXRFCTRL1_FRAC_SH           12
#define DT_TXRFCTRL1_INT_MSK           0xFF000000
#define DT_TXRFCTRL1_INT_SH            24

//.-.-.-.-.-.-.-.-.-.-.-.-.- Tx IF-Control register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.
#define DT_TXIFCTRL_PLLR_MSK           0x00003FFF
#define DT_TXIFCTRL_PLLR_SH            0
#define DT_TXIFCTRL_PLLB_MSK           0x07FFC000
#define DT_TXIFCTRL_PLLB_SH            14
#define DT_TXIFCTRL_PLLA_MSK           0xF8000000
#define DT_TXIFCTRL_PLLA_SH            27

//-.-.-.-.-.-.-.-.-.-.-.-.- Tx RF-Control-2 register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_TXRFCTRL2_AGCSP_MSK         0x00000FFF
#define DT_TXRFCTRL2_AGCSP_SH          0
#define DT_TXRFCTRL2_OUTATTN_MSK       0x0003F000
#define DT_TXRFCTRL2_OUTATTN_SH        12
#define DT_TXRFCTRL2_OUTLVL10B_OFFSET_MSK  0x003FF000
#define DT_TXRFCTRL2_OUTLVL10B_OFFSET_SH   12
#define DT_TXRFCTRL2_AGC_EN_MSK        0x00040000
#define DT_TXRFCTRL2_AGC_EN_SH         18
#define DT_TXRFCTRL2_INPATTN_MSK       0x00780000
#define DT_TXRFCTRL2_INPATTN_SH        19
#define DT_TXRFCTRL2_LOWNOISE_EN_MSK   0x00800000
#define DT_TXRFCTRL2_LOWNOISE_EN_SH    23
#define DT_TXRFCTRL2_CPC_MSK           0x0F000000
#define DT_TXRFCTRL2_CPC_SH            24
#define DT_TXRFCTRL2_PLL1_LOCK_MSK     0x10000000
#define DT_TXRFCTRL2_PLL1_LOCK_SH      28
#define DT_TXRFCTRL2_PLL2_LOCK_MSK     0x20000000
#define DT_TXRFCTRL2_PLL2_LOCK_SH      29
#define DT_TXRFCTRL2_PLL3_LOCK_MSK     0x40000000
#define DT_TXRFCTRL2_PLL3_LOCK_SH      30
#define DT_TXRFCTRL2_AGC_LOCK_MSK      0x80000000
#define DT_TXRFCTRL2_AGC_LOCK_SH       31

//-.-.-.-.-.-.-.-.-.-.-.-.- Tx RF-Control-3 register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_TXRFCTRL3_RFCLKSEL_MSK      0x00000001
#define DT_TXRFCTRL3_RFCLKSEL_SH       0

//.-.-.-.-.-.-.-.-.-.-.-.- Tx RFDAC-Control register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_TXRFDACCTRL_SPIREADY_MSK    0x00000001
#define DT_TXRFDACCTRL_SPIREADY_SH     0
#define DT_TXRFDACCTRL_SPISTART_MSK    0x00000002
#define DT_TXRFDACCTRL_SPISTART_SH     1
#define DT_TXRFDACCTRL_SPINUMBYTES_MSK 0x0000000C
#define DT_TXRFDACCTRL_SPINUMBYTES_SH  2
#define DT_TXRFDACCTRL_SPIRDORWRN_MS   0x00000010
#define DT_TXRFDACCTRL_SPIRDORWRN_SH   4
#define DT_TXRFDACCTRL_SPIADDR_MSK     0xFFFF0000
#define DT_TXRFDACCTRL_SPIADDR_SH      16
// RFDAC SPI control register
typedef union _DT_TXRFDAC_CONTROL
{
    struct
    {                                       
        volatile UInt  m_SpiReady    : 1;   //  Ready for new spi cycle
        volatile UInt  m_SpiStart    : 1;   //  Start a SPI cycle (Write-Only bit)
        volatile UInt  m_SpiNumBytes : 2;   //  Number of bytes to transfer (0=1;1=2;etc.)
        volatile UInt  m_SpiRdOrWrn  : 1;   //  Read ('1') or write ('0') transfer
                 UInt  m_Reserved    : 11;  //
        volatile UInt  m_SpiAddr     : 16;  //  SPI ADDRESS
    }  Fields;                              //  For access to individual fields
    UInt32  All;                            //  For access to complete register
} DT_TXRFDAC_CONTROL;



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ RX register offsets +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// The receive-channel base address
#define DT_RX_REG_GENCTRL              0x0000
#define DT_RX_REG_RXCTRL               0x0004
#define DT_RX_REG_RXSTATUS             0x0008
#define DT_RX_REG_SDIPOS               0x000C
#define DT_RX_REG_FIFOLOAD             0x0018
#define DT_RX_REG_DIAG                 0x001C
#define DT_RX_REG_LOOPBKDATA           0x0020
#define DT_RX_REG_THRESHCTRL           0x0024
#define DT_RX_REG_PCKCOUNT             0x0028
#define DT_RX_REG_CLKCOUNT             0x002C
#define DT_RX_REG_VALIDCOUNT           0x0030
#define DT_RX_REG_VIOLCOUNT            0x0034
#define DT_RX_REG_FIFO_FIRST           0x0040
#define DT_RX_REG_FIFO_LAST            0x0044
#define DT_RX_REG_BITRATE              0x004C

//-.-.-.-.-.-.-.-.-.-.-.-.-.- Rx Control register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DT_RXCTRL_RXMODE_MSK           0x00000003
#define DT_RXCTRL_RXMODE_SH            0
#define DT_RXCTRL_ADC_CLOCK_MSK        0x00000003
#define DT_RXCTRL_ADC_CLOCK_SH         0
#define DT_RXCTRL_ASIINV_MSK           0x0000000C
#define DT_RXCTRL_ASIINV_SH            2
#define DT_RXCTRL_RXMODE_EXT_MSK       0x00000010
#define DT_RXCTRL_RXMODE_EXT_SH        4
#define DT_RXCTRL_EQUALISE_MSK         0x00000010
#define DT_RXCTRL_EQUALISE_SH          4
#define DT_RXCTRL_RXCTRL_MSK           0x00000020
#define DT_RXCTRL_RXCTRL_SH            5
#define DT_RXCTRL_ANTPWR_MSK           0X00000040
#define DT_RXCTRL_ANTPWR_SH            6
#define DT_RXCTRL_TIMESTAMP32_MSK      0x00000080
#define DT_RXCTRL_TIMESTAMP32_SH       7
#define DT_RXCTRL_TIMESTAMP64_MSK      0x00000100      // Not used by DTUs
#define DT_RXCTRL_TIMESTAMP64_SH       8
#define DT_RXCTRL_PERINT_EN_MSK        0x00000100
#define DT_RXCTRL_PERINT_EN_SH         8
#define DT_RXCTRL_OVFINT_EN_MSK        0x00000200
#define DT_RXCTRL_OVFINT_EN_SH         9
#define DT_RXCTRL_SYNCINT_EN_MSK       0x00000400
#define DT_RXCTRL_SYNCINT_EN_SH        10
#define DT_RXCTRL_THRINT_EN_MSK        0x00001000
#define DT_RXCTRL_THRINT_EN_SH         12
#define DT_RXCTRL_RATEOVFINT_EN_MSK    0x00002000
#define DT_RXCTRL_RATEOVFINT_EN_SH     13
#define DT_RXCTRL_FRAMEINT_EN_MSK      0x00002000
#define DT_RXCTRL_FRAMEINT_EN_SH       13
#define DT_RXCTRL_ENAPWR_MSK           0x00020000
#define DT_RXCTRL_ENAPWR_SH            17
#define DT_RXCTRL_LED_CTRL             0x00080000
#define DT_RXCTRL_LED_CTRL_MSK         0x00080000
#define DT_RXCTRL_LED_CTRL_SH          19
#define DT_RXCTRL_LED_GREEN            0x00100000
#define DT_RXCTRL_LED_GREEN_MSK        0x00100000
#define DT_RXCTRL_LED_GREEN_SH         20
#define DT_RXCTRL_LED_RED              0x00200000
#define DT_RXCTRL_LED_RED_MSK          0x00200000
#define DT_RXCTRL_LED_RED_SH           21
#define DT_RXCTRL_FIFO_CLR_MSK         0x00400000
#define DT_RXCTRL_FIFO_CLR_SH           22
#define DT_RXCTRL_RXRESET_MSK          0x00800000
#define DT_RXCTRL_RXRESET_SH            23
#define DT_RXCTRL_SDIMODE_MSK          0x01000000
#define DT_RXCTRL_SDIMODE_SH           24
#define DT_RXCTRL_SDI10BIT_MSK         0x02000000
#define DT_RXCTRL_SDI10BIT_SH          25
#define DT_RXCTRL_SDIHUFF_MSK          0x04000000
#define DT_RXCTRL_SDIHUFF_SH           26
#define DT_RXCTRL_PIDFILT_EN_MSK       0x08000000
#define DT_RXCTRL_PIDFILT_EN_SH        27

// Rx Control: RxControl
#define DT_RXCTRL_IDLE                 0
#define DT_RXCTRL_RCV                  1

// Rx Control: RxMode
// ASI
#define DT_RXMODE_ST188                0
#define DT_RXMODE_ST204                1
#define DT_RXMODE_STMP2                2
#define DT_RXMODE_STRAW                3
#define DT_RXMODE_STL3                 4
#define DT_RXMODE_IPRAW                5
#define DT_RXMODE_RAWASI               5
#define DT_RXMODE_STTRP                7
#define DT_RXMODE_MASK                 7
#define DT_RX_TIMESTAMP                8
#define DT_RX_TIMESTAMP32              8
#define DT_RX_TIMESTAMP64              16
// SDI
#define DT_RXMODE_SDI_FULL             0
#define DT_RXMODE_SDI_ACTVID           1

// Rx Control: AsiInv values
#define DT_POLARITY_AUTO               0
#define DT_POLARITY_NORMAL             2
#define DT_POLARITY_INVERT             3

// Adc Control: AdcClock 
#define DT_ADCCLK_OFF                  0
#define DT_ADCCLK_27M1                 1
#define DT_ADCCLK_20M647               2
#define DT_ADCCLK_13M5                 3

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Rx Status register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DT_RXSTAT_PCKSIZE_MSK          0x00000003  // In SDI mode returns SDI standard
#define DT_RXSTAT_PCKSIZE_SH           0
#define DT_RXSTAT_NUMINV_MSK           0x0000000C
#define DT_RXSTAT_NUMINV_SH            2
#define DT_RXSTAT_SDRAMSIZE_MSK        0x000000F0
#define DT_RXSTAT_SDRAMSIZE_SH         4
#define DT_RXSTAT_PERINT_MSK           0x00000100
#define DT_RXSTAT_PERINT_SH            8
#define DT_RXSTAT_OVFINT_MSK           0x00000200
#define DT_RXSTAT_OVFINT_SH            9
#define DT_RXSTAT_SYNCINT_MSK          0x00000400
#define DT_RXSTAT_SYNCINT_SH           10
#define DT_RXSTAT_THRINT_MSK           0x00001000
#define DT_RXSTAT_THRINT_SH            12
#define DT_RXSTAT_RATEOVFINT_MSK       0x00002000
#define DT_RXSTAT_RATEOVFINT_SH        13
#define DT_RXSTAT_CLKDET_MSK           0x00004000
#define DT_RXSTAT_CLKDET_SH            14
#define DT_RXSTAT_ASILOCK_MSK          0x00008000
#define DT_RXSTAT_ASILOCK_SH           15
#define DT_RXSTAT_RATEOK_MSK           0x00010000
#define DT_RXSTAT_RATEOK_SH            16
#define DT_RXSTAT_ASIINV_MSK           0x00020000
#define DT_RXSTAT_ASIINV_SH            17

// Rx Status: PCKSIZE values
#define DT_PCKSIZE_INV                 0
#define DT_PCKSIZE_188                 2
#define DT_PCKSIZE_204                 3
#define DT_SDIMODE_INV                 0
#define DT_SDIMODE_525                 1
#define DT_SDIMODE_625                 2

// Rx Status: NUMINV values
#define DT_NUMINV_NONE                 0
#define DT_NUMINV_16                   1
#define DT_NUMINV_OTHER                2

// Rx Status: SDRAM sizes
#define DT_SDRAM_SIZE_8MB              0
#define DT_SDRAM_SIZE_16MB             1
#define DT_SDRAM_SIZE_32MB             2
#define DT_SDRAM_SIZE_64MB             3
#define DT_SDRAM_SIZE_128MB            4


//.-.-.-.-.-.-.-.-.-.-.-.-.- Rx SDI Position: Bit Fields -.-.-.-.-.-.-.-.-.-.-.-.-.
#define DT_RXSDIPOS_SAMP_MSK           0x7FF
#define DT_RXSDIPOS_SAMP_SH            0
#define DT_RXSDIPOS_LINE_MSK           (0x3FF<<11)
#define DT_RXSDIPOS_LINE_SH            11
#define DT_RXSDIPOS_FRAME_MSK          (0x7FF<<21)
#define DT_RXSDIPOS_FRAME_SH           21

//.-.-.-.-.-.-.-.-.-.-.-.-.- Rx Status Diagnostics: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_RXDIAG_LOOPBACK_MSK         0x00000100
#define DT_RXDIAG_LOOPBACK_SH          8
#define DT_RXDIAG_AUTOMEMTEST_MSK      0x00000200
#define DT_RXDIAG_AUTOMEMTEST_SH       9
#define DT_RXDIAG_RFRATEOVF_MSK        0x00000400
#define DT_RXDIAG_RFRATEOVF_SH         10
#define DT_RXDIAG_SDFULL_MSK           0x00000800
#define DT_RXDIAG_SDFULL_SH            11


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ HD channel register offsets +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Offsets relative to HD channel register base address
#define DT_HD_REG_CTRL1                 0x0000
#define DT_HD_REG_CTRL2                 0x0004
#define DT_HD_REG_STATUS                0x0008
#define DT_HD_REG_LEDCTRL               0x000C
#define DT_HD_REG_CURFRM_LSB            0x0010
#define DT_HD_REG_CURFRM_MSB            0x0014
#define DT_HD_REG_LASTFRM_LSB           0x0018
#define DT_HD_REG_LASTFRM_MSB           0x001C
#define DT_HD_REG_SOFFRM_LSB            0x0020
#define DT_HD_REG_SOFFRM_MSB            0x0024
#define DT_HD_REG_SOFLINE               0x0028
#define DT_HD_REG_MEMTRCTRL             0x0034
#define DT_HD_REG_MEMTRSTAT             0x0038
#define DT_HD_REG_MEMTRNUMS             0x003C
#define DT_HD_REG_MEMTRNUMB             0x0040
#define DT_HD_REG_S0BEGINADDR           0x0044
#define DT_HD_REG_S0ENDADDR             0x0048
#define DT_HD_REG_S0STARTADDR           0x004C
#define DT_HD_REG_S0ROWCONF             0x0050
#define DT_HD_REG_S1BEGINADDR           0x0054
#define DT_HD_REG_S1ENDADDR             0x0058
#define DT_HD_REG_S1STARTADDR           0x005C
#define DT_HD_REG_S1ROWCONF             0x0060
#define DT_HD_REG_FRMCONF1              0x0064
#define DT_HD_REG_FRMCONF2              0x0068
#define DT_HD_REG_FRMCONF3              0x006C
#define DT_HD_REG_FRMCONF4              0x0070
#define DT_HD_REG_FRMCONF5              0x0074
#define DT_HD_REG_GS29XXSPI             0x0080

//-.-.-.-.-.-.-.-.-.-.-.- HD-Channel Control1 register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.-

#define DT_HD_CTRL1_IODIR_MSK            0x00000001
#define DT_HD_CTRL1_IODIR_SH             0
#define DT_HD_CTRL1_IOENA_MSK            0x00000002
#define DT_HD_CTRL1_IOENA_SH             1
#define DT_HD_CTRL1_IORST_MSK            0x00000004
#define DT_HD_CTRL1_IORST_SH             2
#define DT_HD_CTRL1_IOBYPASSEQ_MSK       0x00000008
#define DT_HD_CTRL1_IOBYPASSEQ_SH        3
#define DT_HD_CTRL1_IOPORT_MSK           0x000000F0
#define DT_HD_CTRL1_IOPORT_SH            4
#define DT_HD_CTRL1_OPMODE_MSK           0x00007000
#define DT_HD_CTRL1_OPMODE_SH            12
#define DT_HD_CTRL1_RXTXCTRL_MSK         0x00018000
#define DT_HD_CTRL1_RXTXCTRL_SH          15
#define DT_HD_CTRL1_TXNOEAV_MSK          0x04000000
#define DT_HD_CTRL1_TXNOEAV_SH           26
#define DT_HD_CTRL1_TXNOSAV_MSK          0x08000000
#define DT_HD_CTRL1_TXNOSAV_SH           27
#define DT_HD_CTRL1_LASTFRMINTEN_MSK     0x80000000
#define DT_HD_CTRL1_LASTFRMINTEN_SH      31

// HD-Channel Control register: op-mode value
#define DT_HD_OPMODE_DISBABLE           0x0
#define DT_HD_OPMODE_SD                 0x1
#define DT_HD_OPMODE_HD                 0x2
#define DT_HD_OPMODE_3G                 0x3
#define DT_HD_OPMODE_ASI                0x4

// HD-Channel Control register: rx/tx control values
#define DT_HD_RXTXCTRL_IDLE             0x0
#define DT_HD_RXTXCTRL_FREEZE           0x1
#define DT_HD_RXTXCTRL_RUN              0x2

//-.-.-.-.-.-.-.-.-.-.-.- HD-Channel Control2 register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.-
//
// NOTE: All bits in Control 2 register are write-only bits, so for now we can simple 
// write a 1 to the control bits (i.e. no need for read-modify-write)

#define DT_HD_CTRL2_REPFRM_MSK           0x00000001
#define DT_HD_CTRL2_REPFRM_SH            0
#define DT_HD_CTRL2_REALIGN_MSK          0x00000002
#define DT_HD_CTRL2_REALIGN_SH           1

//.-.-.-.-.-.-.-.-.-.-.- // HD-Channel Status register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.-

#define  DT_HD_STATUS_DETVIDSTD_MSK     0x000003FF
#define  DT_HD_STATUS_DETVIDSTD_SH      0
#define  DT_HD_STATUS_CD_MSK            0x00000400
#define  DT_HD_STATUS_CD_SH             10
#define  DT_HD_STATUS_REPCNT_MSK        0x07FFF800
#define  DT_HD_STATUS_REPCNT_SH         11
#define  DT_HD_STATUS_LASTFRMINT_MSK    0x80000000
#define  DT_HD_STATUS_LASTFRMINT_SH     31

// HD-Channel Status register: detected video standard values 
#define  DT_VIDSTD_UNKNOWN              0x000
#define  DT_VIDSTD_525I59_94            0x1D6
#define  DT_VIDSTD_625I50               0x158
#define  DT_VIDSTD_720P23_98            0x188
#define  DT_VIDSTD_720P24               0x108
#define  DT_VIDSTD_720P25               0x106
#define  DT_VIDSTD_720P29_97            0x182
#define  DT_VIDSTD_720P30               0x102
#define  DT_VIDSTD_720P50               0x104
#define  DT_VIDSTD_720P59_94            0x180
#define  DT_VIDSTD_720P60               0x100
#define  DT_VIDSTD_1080P23_98           0x190
#define  DT_VIDSTD_1080P24              0x110
#define  DT_VIDSTD_1080P25              0x10D
#define  DT_VIDSTD_1080P29_97           0x18B
#define  DT_VIDSTD_1080P30              0x10B
#define  DT_VIDSTD_1080I50              0x14C
#define  DT_VIDSTD_1080I59_94           0x1CA
#define  DT_VIDSTD_1080I60              0x14A
#define  DT_VIDSTD_1080P50              0x10C
#define  DT_VIDSTD_1080P59_94           0x18A
#define  DT_VIDSTD_1080P60              0x10A

//-.-.-.-.-.-.-.-.-.-.- HD-Channel LED Control register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.

#define DT_HD_LEDCTRL_LEDCTRL_MSK       0x00000001
#define DT_HD_LEDCTRL_LEDCTRL_SH        0
#define DT_HD_LEDCTRL_LEDGREEN_MSK      0x00000002
#define DT_HD_LEDCTRL_LEDGREEN_SH       1
#define DT_HD_LEDCTRL_LEDRED_MSK        0x00000004
#define DT_HD_LEDCTRL_LEDRED_SH         2
#define DT_HD_LEDCTRL_LEDBLUE_MSK       0x00000008
#define DT_HD_LEDCTRL_LEDBLUE_SH        3
#define DT_HD_LEDCTRL_PATTERN_MSK       0x000000F0
#define DT_HD_LEDCTRL_PATTERN_SH        4

//.-.-.-.-.-.-.-.-.-.- HD-Channel Current Frame register: Bit Fields -.-.-.-.-.-.-.-.-.-.-

#define DT_HD_CURFRMLSB_MSK             0xFFFFFFFF
#define DT_HD_CURFRMLSB_SH              0
#define DT_HD_CURFRMMSB_MSK             0xFFFFFFFF
#define DT_HD_CURFRMMSB_SH              0

//.-.-.-.-.-.-.-.-.-.-.- HD-Channel Last Frame register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.

#define DT_HD_LASTFRMLSB_MSK            0xFFFFFFFF
#define DT_HD_LASTFRMLSB_SH             0
#define DT_HD_LASTFRMMSB_MSK            0xFFFFFFFF
#define DT_HD_LASTFRMMSB_SH             0

//.-.-.-.-.-.-.-.-.-.-.- HD-Channel SOF Frame register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.-

#define DT_HD_SOFFRMLSB_MSK             0xFFFFFFFF
#define DT_HD_SOFFRMLSB_SH              0
#define DT_HD_SOFFRMMSB_MSK             0xFFFFFFFF
#define DT_HD_SOFFRMMSB_SH              0

//-.-.-.-.-.-.-.-.-.-.-.- HD-Channel SOF Line register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.-

#define DT_HD_SOFLINE_LINE_MSK          0x0000FFFF
#define DT_HD_SOFLINE_LINE_SH           0

//-.-.-.-.-.-.-.-.- HD-Channel Mem-Transfer Control register: Bit Fields -.-.-.-.-.-.-.-.-

#define DT_HD_MEMTRCTRL_NUML_MSK        0x0000FFFF
#define DT_HD_MEMTRCTRL_NUML_SH         0
#define DT_HD_MEMTRCTRL_TRCMD_MSK       0x00070000
#define DT_HD_MEMTRCTRL_TRCMD_SH        16
#define DT_HD_MEMTRCTRL_DATAFMT_MSK     0x00180000
#define DT_HD_MEMTRCTRL_DATAFMT_SH      19
#define DT_HD_MEMTRCTRL_SYMFLTMODE_MSK  0x00C00000
#define DT_HD_MEMTRCTRL_SYMFLTMODE_SH   22
#define DT_HD_MEMTRCTRL_SCMODE_MSK      0x06000000
#define DT_HD_MEMTRCTRL_SCMODE_SH       25
#define DT_HD_MEMTRCTRL_ANCFLTMODE_MSK  0x70000000
#define DT_HD_MEMTRCTRL_ANCFLTMODE_SH   28
#define DT_HD_MEMTRCTRL_STRIDEMODE_MSK  0x80000000
#define DT_HD_MEMTRCTRL_STRIDEMODE_SH   31

// HD-Channel Mem-Transfer register: Transfer command values
#define  DT_MEMTR_TRCMD_IDLE            0x0
#define  DT_MEMTR_TRCMD_SECT0           0x1
#define  DT_MEMTR_TRCMD_SECT1           0x2
#define  DT_MEMTR_TRCMD_DUALFLD         0x4
#define  DT_MEMTR_TRCMD_FRAME           0x7

// HD-Channel Mem-Transfer register: Data format values
#define  DT_MEMTR_DATAFMT_8B            0x0
#define  DT_MEMTR_DATAFMT_10B           0x1
#define  DT_MEMTR_DATAFMT_16B           0x2

// HD-Channel Mem-Transfer register: Symbol filter mode values
#define  DT_MEMTR_SYMFLTMODE_ALL        0x0
#define  DT_MEMTR_SYMFLTMODE_LUM        0x1
#define  DT_MEMTR_SYMFLTMODE_CHROM      0x2
#define  DT_MEMTR_SYMFLTMODE_SWAP       0x3

// HD-Channel Mem-Transfer register: Scaling mode values
#define  DT_MEMTR_SCMODE_OFF            0x0
#define  DT_MEMTR_SCMODE_1_4            0x1
#define  DT_MEMTR_SCMODE_1_16           0x2

// HD-Channel Mem-Transfer register: Scaling mode values
#define  DT_MEMTR_ANCFLTMODE_OFF        0x0
#define  DT_MEMTR_ANCFLTMODE_HANCMIN    0x1
#define  DT_MEMTR_ANCFLTMODE_HANCALL    0x2
#define  DT_MEMTR_ANCFLTMODE_VANCMIN    0x5
#define  DT_MEMTR_ANCFLTMODE_VANCALL    0x6

//-.-.-.-.-.-.-.-.- HD-Channel Mem-Transfer Status register: Bit Fields -.-.-.-.-.-.-.-.-.

//#define DT_HD_MEMTRSTAT_XXX_MSK       0xFFFFFFFF
//#define DT_HD_MEMTRSTAT_XXX_SH        0

//-.-.-.-.-.-.-.-.-.- HD-Channel Mem-TR #Symbols register: Bit Fields -.-.-.-.-.-.-.-.-.-.

#define DT_HD_MEMTRNUMS_NUMS_MSK        0xFFFFFFFF
#define DT_HD_MEMTRNUMS_NUMS_SH         0

//.-.-.-.-.-.-.-.-.-.- HD-Channel Mem-TR # Bytes register: Bit Fields -.-.-.-.-.-.-.-.-.-.

#define DT_HD_MEMTRNUMS_NUMB_MSK        0xFFFFFFFF
#define DT_HD_MEMTRNUMS_NUMB_SH         0

//.-.-.-.-.-.-.-.-.-.- HD-Channel S0 Begin Addr register: Bit Fields -.-.-.-.-.-.-.-.-.-.-

#define DT_HD_S0BEGINADDR_ADDR_MSK      0xFFFFFFFF
#define DT_HD_S0BEGINADDR_ADDR_SH       0

//-.-.-.-.-.-.-.-.-.-.- HD-Channel S0 End Addr register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.

#define DT_HD_S0ENDADDR_ADDR_MSK        0xFFFFFFFF
#define DT_HD_S0ENDADDR_ADDR_SH         0

//.-.-.-.-.-.-.-.-.-.- HD-Channel S0 Start Addr register: Bit Fields -.-.-.-.-.-.-.-.-.-.-

#define DT_HD_S0STARTADDR_ADDR_MSK      0xFFFFFFFF
#define DT_HD_S0STARTADDR_ADDR_SH       0

//.-.-.-.-.-.-.-.-.-.- HD-Channel S0 Row Config register: Bit Fields -.-.-.-.-.-.-.-.-.-.-

#define DT_HD_S0ROWCONF_NUMS_MSK      0x0000FFFF
#define DT_HD_S0ROWCONF_NUMS_SH       0
#define DT_HD_S0ROWCONF_NUMB_MSK      0xFFFF0000
#define DT_HD_S0ROWCONF_NUMB_SH       16

//.-.-.-.-.-.-.-.-.-.- HD-Channel S1 Begin Addr register: Bit Fields -.-.-.-.-.-.-.-.-.-.-

#define DT_HD_S1BEGINADDR_ADDR_MSK      0xFFFFFFFF
#define DT_HD_S1BEGINADDR_ADDR_SH       0

//-.-.-.-.-.-.-.-.-.-.- HD-Channel S1 End Addr register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.

#define DT_HD_S1ENDADDR_ADDR_MSK        0xFFFFFFFF
#define DT_HD_S1ENDADDR_ADDR_SH         0

//.-.-.-.-.-.-.-.-.-.- HD-Channel S1 Start Addr register: Bit Fields -.-.-.-.-.-.-.-.-.-.-

#define DT_HD_S1STARTADDR_ADDR_MSK      0xFFFFFFFF
#define DT_HD_S1STARTADDR_ADDR_SH       0

//.-.-.-.-.-.-.-.-.-.- HD-Channel S1 Row Config register: Bit Fields -.-.-.-.-.-.-.-.-.-.-

#define DT_HD_S1ROWCONF_NUMS_MSK      0x0000FFFF
#define DT_HD_S1ROWCONF_NUMS_SH       0
#define DT_HD_S1ROWCONF_NUMB_MSK      0xFFFF0000
#define DT_HD_S1ROWCONF_NUMB_SH       16

//.-.-.-.-.-.-.-.-.-.- HD-Channel Frame Config 1 register: Bit Fields -.-.-.-.-.-.-.-.-.-.

#define DT_HD_FRMCONF1_NUML_MSK         0x0000FFFF
#define DT_HD_FRMCONF1_NUML_SH          0
#define DT_HD_FRMCONF1_INTERL_MSK       0x00010000
#define DT_HD_FRMCONF1_INTERL_SH        16

//.-.-.-.-.-.-.-.-.-.- HD-Channel Frame Config 2 register: Bit Fields -.-.-.-.-.-.-.-.-.-.

#define DT_HD_FRMCONF2_FLD1START_MSK    0x0000FFFF
#define DT_HD_FRMCONF2_FLD1START_SH     0
#define DT_HD_FRMCONF2_FLD2START_MSK    0xFFFF0000
#define DT_HD_FRMCONF2_FLD2START_SH     16

//.-.-.-.-.-.-.-.-.-.- HD-Channel Frame Config 3 register: Bit Fields -.-.-.-.-.-.-.-.-.-.

#define DT_HD_FRMCONF3_FLD1END_MSK      0x0000FFFF
#define DT_HD_FRMCONF3_FLD1END_SH       0
#define DT_HD_FRMCONF3_FLD2END_MSK      0xFFFF0000
#define DT_HD_FRMCONF3_FLD2END_SH       16

//.-.-.-.-.-.-.-.-.-.- HD-Channel Frame Config 4 register: Bit Fields -.-.-.-.-.-.-.-.-.-.

#define DT_HD_FRMCONF4_VID1START_MSK    0x0000FFFF
#define DT_HD_FRMCONF4_VID1START_SH     0
#define DT_HD_FRMCONF4_VID2START_MSK    0xFFFF0000
#define DT_HD_FRMCONF4_VID2START_SH     16

//.-.-.-.-.-.-.-.-.-.- HD-Channel Frame Config 5 register: Bit Fields -.-.-.-.-.-.-.-.-.-.

#define DT_HD_FRMCONF5_VID1END_MSK      0x0000FFFF
#define DT_HD_FRMCONF5_VID1END_SH       0
#define DT_HD_FRMCONF5_VID2END_MSK      0xFFFF0000
#define DT_HD_FRMCONF5_VID2END_SH       16

//.-.-.-.-.-.-.-.-.- HD-Channel GS29XX SPI Control register: Bit Fields -.-.-.-.-.-.-.-.-.

#define DT_HD_GS29XXSPI_DATA_MSK    0x0000FFFF
#define DT_HD_GS29XXSPI_DATA_SH     0
#define DT_HD_GS29XXSPI_ADDR_MSK    0x0FFF0000
#define DT_HD_GS29XXSPI_ADDR_SH     16
#define DT_HD_GS29XXSPI_AUTOINC_MSK 0x10000000
#define DT_HD_GS29XXSPI_AUTOINC_SH  28
#define DT_HD_GS29XXSPI_READ_MSK    0x20000000
#define DT_HD_GS29XXSPI_READ_SH     29
#define DT_HD_GS29XXSPI_START_MSK   0x40000000
#define DT_HD_GS29XXSPI_START_SH    30
#define DT_HD_GS29XXSPI_DONE_MSK    0x80000000
#define DT_HD_GS29XXSPI_DONE_SH     31

#endif // __DT_REGS_H
