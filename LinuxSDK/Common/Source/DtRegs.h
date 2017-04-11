//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtRegs.h *#*#*#*#*#*#*#*#*# (C) 2011-2016 DekTec
//
// DtRegs - Definition of register sets used for all devices DTA/DTU(/DTE) 
// This file contains register offsets with mask and shift defines.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2016 DekTec Digital Video B.V.
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
// Board ID
#define DT_GEN_REG_BOARDID0            0x0104
#define DT_GEN_REG_BOARDID1            0x0108

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
#define DT_GENCTRL_LED_BLUE_MSK        0x08000000
#define DT_GENCTRL_LED_BLUE_SH         27

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
#define DT_GEN_CONTROL0_DEMOD_MSK      0x00000001     // DTU-236/238 DEMOD/ASI mode
#define DT_GEN_CONTROL0_DEMOD_SH       0
#define DT_GEN_CONTROL0_VCOEN_MSK      0x00000002
#define DT_GEN_CONTROL0_VCOEN_SH       1
#define DT_GEN_CONTROL0_TXCLKRSTEN_MSK 0x00000004     // DTA-2152: keep TX clock in reset
#define DT_GEN_CONTROL0_TXCLKRSTEN_SH  2
#define DT_GEN_CONTROL0_PWREN_MSK      0x00000030     // DTA-2111 specific Vdd
#define DT_GEN_CONTROL0_PWREN_SH       4
#define DT_GEN_CONTROL0_DVRTY_MSK      0x00000100     // DTA-2135 specific
#define DT_GEN_CONTROL0_DVRTY_SH       8

//.-.-.-.-.-.-.-.-.-.-.-.- General Control1 Register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_GEN_CONTROL1_WATCHDOG_MSK   0x00000001
#define DT_GEN_CONTROL1_WATCHDOG_SH    0
#define DT_GEN_CONTROL1_HW_ENABLE_MSK  0x00000002
#define DT_GEN_CONTROL1_HW_ENABLE_SH   1
#define DT_GEN_CONTROL1_O_ENABLE_MSK   0x00000004
#define DT_GEN_CONTROL1_O_ENABLE_SH    2

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
#define DT_I2CCTRL_EDDC_EN_MSK         0x00000002
#define DT_I2CCTRL_EDDC_EN_SH          1
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
#define DT_I2CSTAT_EDDC_NACK_MSK       0x00000020
#define DT_I2CSTAT_EDDC_NACK_SH        5


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

#define DT_HDGENL_REG_CLKCONF           0x0000
#define DT_HDGENL_REG_CTRL              0x0004
#define DT_HDGENL_REG_STATUS            0x0008
#define DT_HDGENL_REG_LEDCTRL           0x000C
#define DT_HDGENL_REG_REFFRM_LSB        0x0010
#define DT_HDGENL_REG_REFFRM_MSB        0x0014
#define DT_HDGENL_REG_SOFFRM_LSB        0x0018
#define DT_HDGENL_REG_SOFFRM_MSB        0x001C

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
// Modulation registers
#define DT_TX_REG_MOD_CONTROL3         0x0024   // Reserved for future use
#define DT_TX_REG_MOD_CONTROL1         0x0028
#define DT_TX_REG_MOD_CONTROL2         0x002C   // for modulator channel
#define DT_TX_REG_MOD_CHANLEVEL        0x0030
#define DT_TX_REG_MOD_GLOBALGAIN       0x0034

// Phase modulator registers
#define DT_TX_REG_PHASENOISE_CONTROL   0x0020   // Phase noise control

// Non-modulation registers
#define DT_TX_REG_TXCLOCK_MOD          0x002C   // for ASI/SDI channel
#define DT_TX_REG_FIFO_FIRST           0x0040
// More modulation registers
#define DT_TX_REG_IQMAP_CONTROL        0x0044
#define DT_TX_REG_IQMAP_DATA           0x0048
#define DT_TX_REG_FILTER_CONTROL       0x004C
#define DT_TX_REG_FILTER_DATA          0x0050 
#define DT_TX_REG_DDS_CONTROL          0x0054
#define DT_TX_REG_NCO_CONTROL          0x0054
#define DT_TX_REG_DDS_DATA             0x0058
#define DT_TX_REG_NCO_DATA_LOW         0x0058
#define DT_TX_REG_NCO_DATA_HIGH        0x005C
#define DT_TX_REG_TIMSYNC_CONTROL      0x0060
#define DT_TX_REG_TIMSYNC_STATUS       0x0064
#define DT_TX_REG_SRC_CONTROL1         0x0068
#define DT_TX_REG_SRC_CONTROL2         0x006C
#define DT_TX_REG_SRC_CONTROL3         0x0070
#define DT_TX_REG_SRC_CONTROL4         0x0074
#define DT_TX_REG_TIMSYNC_DIFF         0x0078
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
#define DT_TXMODC_QPSK                 0x00
#define DT_TXMODC_BPSK                 0x01   // Not supported
#define DT_TXMODC_QAM4                 0x03
#define DT_TXMODC_QAM16                0x04
#define DT_TXMODC_QAM32                0x05
#define DT_TXMODC_QAM64                0x06
#define DT_TXMODC_QAM128               0x07
#define DT_TXMODC_QAM256               0x08
#define DT_TXMODC_IQMAP                0x09   // I/Q Mapping

// IQ Mapping
#define DT_TXMODC_SYMB8                0x08  // 8-bit symbol mode
#define DT_TXMODC_SYMB8E               0x09  // 8-bit extended symbol mode
#define DT_TXMODC_SYMB4                0x0A  // 4-bit symbol mode
#define DT_TXMODC_IQDIRBUF             0x0E  // Direct I/Q, buffered
#define DT_TXMODC_IQDIRPCK             0x0F  // Direct I/Q packed (DTA-2107)

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
#define DT_TXMODC2_MODTYPE_MSK         0x003F0000
#define DT_TXMODC2_MODTYPE_SH          16
// Modulation-Type field the extended version: values
#define DT_TXMODC2_QPSK                 0x00
#define DT_TXMODC2_QAM4                 0x03
#define DT_TXMODC2_QAM16                0x04
#define DT_TXMODC2_QAM32                0x05
#define DT_TXMODC2_QAM64                0x06
#define DT_TXMODC2_QAM128               0x07
#define DT_TXMODC2_QAM256               0x08
// IQ Mapping
#define DT_TXMODC2_SYMB8                0x09  // 8-bit symbol mode
#define DT_TXMODC2_SYMB8E               0x0A  // 8-bit extended symbol mode
#define DT_TXMODC2_SYMB4                0x0B  // 4-bit symbol mode
#define DT_TXMODC2_IQDIRBUF             0x0C  // Direct I/Q, buffered
#define DT_TXMODC2_IQDIRPCK             0x0D  // Direct I/Q packed

//-.-.-.-.-.-.-.-.-.-.-.-.- Tx SRC-Control1 register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_TXSRCCTRL1_POW2CTRL_MSK     0x0000000F
#define DT_TXSRCCTRL1_POW2CTRL_SH      0
#define DT_TXSRCCTRL1_MNINTINC_MSK     0x07FFFFF0
#define DT_TXSRCCTRL1_MNINTINC_SH      4

//.-.-.-.-.-.-.-.-.-.- Tx Modulation NCO control register: Bit fields -.-.-.-.-.-.-.-.-.-.
#define DT_TXNCOCTRL_NOISE_GAIN_MSK    0x0000001F
#define DT_TXNCOCTRL_NOISE_GAIN_SH     0
#define DT_TXNCOCTRL_CARRIER_LEVEL_MSK 0x3C000000
#define DT_TXNCOCTRL_CARRIER_LEVEL_SH  26
#define DT_TXNCOCTRL_CARRIER_ONLY_MSK  0x40000000
#define DT_TXNCOCTRL_CARRIER_ONLY_SH   30

//-.-.-.-.-.-.- Tx Modulation Synchronistation control register: Bit fields -.-.-.-.-.-.-.
#define DT_TXTSYNCCTRL_SYNC_ENA_MSK    0x00000001
#define DT_TXTSYNCCTRL_SYNC_ENA_SH     0
#define DT_TXTSYNCCTRL_MAX_DIFF_MSK    0x00FFFFF0
#define DT_TXTSYNCCTRL_MAX_DIFF_SH     4

//.-.-.-.-.-.-.- Tx Modulation Synchronistation status register: Bit fields -.-.-.-.-.-.-.
#define DT_TXTSYNCSTAT_INSYNC_MSK      0x00000001
#define DT_TXTSYNCSTAT_INSYNC_SH       0
#define DT_TXTSYNCSTAT_OVFLOW_MSK      0x00000002
#define DT_TXTSYNCSTAT_OVFLOW_SH       1
#define DT_TXTSYNCSTAT_TOOEARLY_MSK    0x000000F0
#define DT_TXTSYNCSTAT_TOOEARLY_SH     4
#define DT_TXTSYNCSTAT_TOOLATE_MSK     0x00000F00
#define DT_TXTSYNCSTAT_TOOLATE_SH      8
#define DT_TXTSYNCSTAT_OUTSYNC_MSK     0x00000F00
#define DT_TXTSYNCSTAT_OUTSYNC_SH      12
#define DT_TXTSYNCSTAT_ERRINV_MSK      0x0000F000
#define DT_TXTSYNCSTAT_ERRINV_SH       16

//-.-.-.-.-.-.-.- Tx Phase noise modulation controls register: Bit fields -.-.-.-.-.-.-.-.
#define DT_TXTPNCTRL_ENABLE_MSK      0x00000001
#define DT_TXTPNCTRL_ENABLE_SH       0


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ RF registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- RF Register offsets -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Note for most devices the registers have the same offset as the TX Registers. 
// Except for the DTA-2115 the offset is 0x460
//
#define DT_RF_REG_DAC_IRQCTRL          0x0020
#define DT_RF_REG_DAC_IRQSTATUS        0x0024
#define DT_RF_REG_IDAC_CONTROL         0x0030
#define DT_RF_REG_QDAC_CONTROL         0x0034
#define DT_RF_REG_DACITF_CONTROL       0x0030   // DTA-2115
#define DT_RF_REG_DACSPI_CONTROL       0x0034   // DTA-2115
#define DT_RF_REG_RF_CONTROL           0x0038   // DTA-107 and DTA-110
#define DT_RF_REG_RF_CONTROL1          0x0038
#define DT_RF_REG_IF_CONTROL           0x0038
#define DT_RF_REG_RF_CONTROL2          0x003C
#define DT_RF_REG_RF_CONTROL3          0x0040
#define DT_RF_REG_RFDAC_CONTROL        0x0050
#define DT_RF_REG_RFDAC_DATA           0x0054
#define DT_RF_REG_SATUR_COUNT          0x005C
#define DT_RF_REG_NLC_CONTROL1         0x006C
#define DT_RF_REG_NLC_CONTROL2         0x0070
//.-.-.-.-.-.-.-.-.-.- RF I- and Q-DAC-Control registers: Bit Fields -.-.-.-.-.-.-.-.-.-.-
#define DT_RFDACC_FINEGAIN_MSK         0x000000FF
#define DT_RFDACC_FINEGAIN_SH          0
#define DT_RFDACC_OFFSDIR_MSK          0x00000100
#define DT_RFDACC_OFFSDIR_SH           8
#define DT_RFDACC_OFFSET_MSK           0x0007FE00
#define DT_RFDACC_OFFSET_SH            9
//-.-.-.-.-.-.- RF DAC Interface Control register: Bit fields (for DTA-2115) -.-.-.-.-.-.-
#define DT_RFDACITFCTRL_MODE_MSK       0x00000003
#define DT_RFDACITFCTRL_MODE_SH        0
#define DT_RFDACITFCTRL_GAIN_MSK       0x00000004
#define DT_RFDACITFCTRL_GAIN_SH        2
#define DT_RFDACITFCTRL_RESETDET_MSK   0x00000020
#define DT_RFDACITFCTRL_RESETDET_SH    5
#define DT_RFDACITFCTRL_DACIRQ_MSK     0x00000040
#define DT_RFDACITFCTRL_DACIRQ_SH      6
// RF-DAC-ITF-Control: Mode
#define DT_RFDACITF_MODE_NORMAL        0
#define DT_RFDACITF_MODE_PARITY        1
// RF-DAC-ITF-Control: Gain
#define DT_RFDACITF_GAIN_0_14_4        0
#define DT_RFDACITF_GAIN_1_14_3        1
//-.-.-.-.-.-.-.-.- DAC SPI-Control register: Bit fields (for DTA-2115) -.-.-.-.-.-.-.-.-.
#define DT_RFDACSPICTRL_SPIDATA_MSK    0x000000FF
#define DT_RFDACSPICTRL_SPIDATA_SH     0
#define DT_RFDACSPICTRL_SPIADDR_MSK    0x00007F00
#define DT_RFDACSPICTRL_SPIADDR_SH     8
#define DT_RFDACSPICTRL_SPIRDORWRN_MSK 0x00008000
#define DT_RFDACSPICTRL_SPIRDORWRN_SH  15
#define DT_RFDACSPICTRL_SPIUPDATE_MSK  0x00010000
#define DT_RFDACSPICTRL_SPIUPDATE_SH   16
#define DT_RFDACSPICTRL_SPIRESET_MSK   0x00020000
#define DT_RFDACSPICTRL_SPIRESET_SH    17
#define DT_RFDACSPICTRL_SPIREADY_MSK   0x00040000
#define DT_RFDACSPICTRL_SPIREADY_SH    18
//-.-.-.-.-.-.-.-.-.-.-.-.-.- RF-Control  register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_RFCTRL_PLLA_MSK              0x0000001F
#define DT_RFCTRL_PLLA_SH               0
#define DT_RFCTRL_PLLB_MSK              0x00003FE0
#define DT_RFCTRL_PLLB_SH               5
#define DT_RFCTRL_PLL_LOCK_MSK          0x00020000
#define DT_RFCTRL_PLL_LOCK_SH           17
#define DT_RFCTRL_PLLB9_MSK              0x00040000
#define DT_RFCTRL_PLLB9_SH              18
#define DT_RFCTRL_PLLR_MSK              0xFFF80000
#define DT_RFCTRL_PLLR_SH               19
// RF-Control 1 register 1: Bit fields (for DTA-115)
#define DT_RFCTRL1_MOD_MSK              0x00000FFF
#define DT_RFCTRL1_MOD_SH               0
#define DT_RFCTRL1_FRAC_MSK             0x00FFF000
#define DT_RFCTRL1_FRAC_SH              12
#define DT_RFCTRL1_INT_MSK              0xFF000000
#define DT_RFCTRL1_INT_SH               24
#define DT_RFCTRL1_SPEC_INV_MSK         0x02000000  // Global spectrum inversion DTA-2115
#define DT_RFCTRL1_SPEC_INV_SH          25

//-.-.-.-.-.-.-.-.-.-.-.-.-.-  IF-Control register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_TXIFCTRL_PLLR_MSK            0x00003FFF
#define DT_TXIFCTRL_PLLR_SH             0
#define DT_TXIFCTRL_PLLB_MSK            0x07FFC000
#define DT_TXIFCTRL_PLLB_SH             14
#define DT_TXIFCTRL_PLLA_MSK            0xF8000000
#define DT_TXIFCTRL_PLLA_SH             27

//.-.-.-.-.-.-.-.-.-.-.-.-.- RF-Control-2 register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_RFCTRL2_AGCSP_MSK            0x00000FFF
#define DT_RFCTRL2_AGCSP_SH             0
#define DT_RFCTRL2_OUTATTN_MSK          0x0003F000
#define DT_RFCTRL2_OUTATTN_SH           12
#define DT_RFCTRL2_OUTLVL10B_OFFSET_MSK 0x003FF000
#define DT_RFCTRL2_OUTLVL10B_OFFSET_SH  12
#define DT_RFCTRL2_AGC_EN_MSK           0x00040000
#define DT_RFCTRL2_AGC_EN_SH            18
#define DT_RFCTRL2_INPATTN_MSK          0x00780000
#define DT_RFCTRL2_INPATTN_SH           19
#define DT_RFCTRL2_LOWNOISE_EN_MSK      0x00800000
#define DT_RFCTRL2_LOWNOISE_EN_SH       23
#define DT_RFCTRL2_CPC_MSK              0x0F000000
#define DT_RFCTRL2_CPC_SH               24
#define DT_RFCTRL2_PLL1_LOCK_MSK        0x10000000
#define DT_RFCTRL2_PLL1_LOCK_SH         28
#define DT_RFCTRL2_PLL2_LOCK_MSK        0x20000000
#define DT_RFCTRL2_PLL2_LOCK_SH         29
#define DT_RFCTRL2_PLL3_LOCK_MSK        0x40000000
#define DT_RFCTRL2_PLL3_LOCK_SH         30
#define DT_RFCTRL2_AGC_LOCK_MSK         0x80000000
#define DT_RFCTRL2_AGC_LOCK_SH          31
//-.-.-.-.-.-.-.-.-.-.- RF-Control-2 register: Bit fields  DTA-2115 -.-.-.-.-.-.-.-.-.-.-.
#define DT_RFCTRL2_ATTN1_MSK            0x0000003F
#define DT_RFCTRL2_ATTN1_SH             0
#define DT_RFCTRL2_ATTN2_MSK            0x00000FC0
#define DT_RFCTRL2_ATTN2_SH             6
#define DT_RFCTRL2_ATTN3_MSK            0x0003F000
#define DT_RFCTRL2_ATTN3_SH             12
//.-.-.-.-.-.-.-.-.-.-.-.-.- RF-Control-3 register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_RFCTRL3_RFCLKSEL_MSK         0x00000001
#define DT_RFCTRL3_RFCLKSEL_SH          0
#define DT_RFCTRL3_CLKFRESEL_MSK        0x00000002
#define DT_RFCTRL3_CLKFRESEL_SH         1
#define DT_RFCTRL3_FILTERSEL_MSK        0x0000000C
#define DT_RFCTRL3_FILTERSEL_SH         2
#define DT_RFCTRL3_AMPPOWER_MSK         0x00000010
#define DT_RFCTRL3_AMPPOWER_SH          4
// RF-Control-3: ClkFreqSel
#define DT_RFCLKFREQ_1600               0
#define DT_RFCLKFREQ_2400               1
// RF-Control-3: Post DAC Filter
#define DT_RFFILTER_BAND_1              0
#define DT_RFFILTER_BAND_2              1
#define DT_RFFILTER_BAND_3              2



//-.-.-.-.-.-.-.-.-.-.-.-.-  RFDAC-Control register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-.
#define DT_RFDACCTRL_SPIREADY_MSK       0x00000001
#define DT_RFDACCTRL_SPIREADY_SH        0
#define DT_RFDACCTRL_SPISTART_MSK       0x00000002
#define DT_RFDACCTRL_SPISTART_SH        1
#define DT_RFDACCTRL_SPINUMBYTES_MSK    0x0000000C
#define DT_RFDACCTRL_SPINUMBYTES_SH     2
#define DT_RFDACCTRL_SPIRDORWRN_MS      0x00000010
#define DT_RFDACCTRL_SPIRDORWRN_SH      4
#define DT_RFDACCTRL_SPIADDR_MSK        0xFFFF0000
#define DT_RFDACCTRL_SPIADDR_SH         16
// RFDAC SPI control register
typedef union _DT_RFDAC_CONTROL
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
} DT_RFDAC_CONTROL;
//-.-.-.-.-.-.-.-.-.-.-.-.- RF NLC-Control1 register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_RFNLCCTRL1_NLC2BETA0_MSK     0x00003FFF
#define DT_RFNLCCTRL1_NLC2BETA0_SH      0
#define DT_RFNLCCTRL1_NLC2BETA1_MSK     0x0FFFC000
#define DT_RFNLCCTRL1_NLC2BETA1_SH      14
//-.-.-.-.-.-.-.-.-.-.-.-.- RF NLC-Control2 register: Bit fields -.-.-.-.-.-.-.-.-.-.-.-.-
#define DT_RFNLCCTRL2_NLC3BETA0_MSK     0x00003FFF
#define DT_RFNLCCTRL2_NLC3BETA0_SH      0
#define DT_RFNLCCTRL2_NLC3BETA1_MSK     0x0FFFC000
#define DT_RFNLCCTRL2_NLC3BETA1_SH      14
#define DT_RFNLCCTRL2_NLCENA_MSK        0x10000000
#define DT_RFNLCCTRL2_NLCENA_SH         28



//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ GPS-clock register offsets +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Offsets relative to GPS-clock reference register base address
#define DT_GPS_REG_CTRL                 0x0000
#define DT_GPS_REG_STATUS               0x0004
#define DT_GPS_REG_TIME_NS              0x0008
#define DT_GPS_REG_TIME_SEC             0x000C

//-.-.-.-.-.-.-.-.-.- GPS-clock register Control register: Bit Fields -.-.-.-.-.-.-.-.-.-.
#define DT_GPS_CTRL_1PPSTOL_MSK          0x00000003
#define DT_GPS_CTRL_1PPSTOL_SH           0

// GPS-clock register Control register: 1PPS-tolerance
#define DT_GPS_1PPSTOL_11NS             0x0
#define DT_GPS_1PPSTOL_18NS             0x1
#define DT_GPS_1PPSTOL_35NS             0x2
#define DT_GPS_1PPSTOL_67NS             0x3

//-.-.-.-.-.-.-.-.-.- GPS-clock register Control register: Bit Fields -.-.-.-.-.-.-.-.-.-.
#define DT_GPS_STAT_1PPSSYNC_MSK         0x00000001
#define DT_GPS_STAT_1PPSSYNC_SH          0
#define DT_GPS_STAT_EXT10MHZ_MSK         0x00000002
#define DT_GPS_STAT_EXT10MHZ_SH          1
#define DT_GPS_STAT_1PPSERR_MSK          0x000000F0
#define DT_GPS_STAT_1PPSERR_SH           4


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

// SDI FLAGS
#define DT_RXMODE_SDI_STAT             0x1000


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

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DMA Registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Offsets relative to DMA register block base address
#define DT_DMA_REG_DESCPTR              0x004
#define DT_DMA_REG_CMDSTAT              0x008
#define DT_DMA_REG_PCIDADDR             0x00C
#define DT_DMA_REG_DESCPRE              0x010
#define DT_DMA_REG_TIMEOUT              0x014
#define DT_DMA_REG_TOTTR_CNT            0x034

//-.-.-.-.-.-.-.-.-.-.- DMA Descriptor prefetch control: Bit fields -.-.-.-.-.-.-.-.-.-.-.
#define DT_DMA_DESCPRE_EN_MSK           0x00000001
#define DT_DMA_DESCPRE_EN_SH            0
#define DT_DMA_DESCPRE_DESCOFF_MSK      0x00000030 
#define DT_DMA_DESCPRE_DESCOFF_SH       4
#define DT_DMA_DESCPRE_NUMDESC_MSK      0x00003F00
#define DT_DMA_DESCPRE_NUMDESC_SH       8

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ HD channel register offsets +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Offsets relative to HD channel register base address
#define DT_HD_REG_CTRL1                 0x0000
#define DT_HD_REG_CTRL2                 0x0004
#define DT_HD_REG_STATUS                0x0008
#define DT_HD_REG_LEDCTRL               0x000C
#define DT_HD_REG_CURFRM_LSB            0x0010  // Legacy: this is the current frame 
#define DT_HD_REG_ASIBYTECNT_LSB        0x0010  // New Style: this is the ASI byte count
#define DT_HD_REG_CURFRM_MSB            0x0014  // Legacy: this is the current frame 
#define DT_HD_REG_ASIBYTECNT_MSB        0x0014  // New Style: this is the ASI byte count
#define DT_HD_REG_LASTFRM_LSB           0x0018
#define DT_HD_REG_LASTFRM_MSB           0x001C
#define DT_HD_REG_SOFFRM_LSB            0x0020
#define DT_HD_REG_SOFFRM_MSB            0x0024
#define DT_HD_REG_SOFLINE               0x0028
#define DT_HD_REG_SDIFORMAT1            0x002C
#define DT_HD_REG_SDISTATUS             0x0030
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
#define DT_HD_REG_MEMTRNUMB_ASI         0x0078
#define DT_HD_REG_ASIRATE               0x007C
#define DT_HD_REG_SPICTRL               0x0080
#define DT_HD_REG_FRM_TIME_LSB          0x0088
#define DT_HD_REG_FRM_TIME_MSB          0x008C
#define DT_HD_REG_S0_NEXTFRM_ADDR       0x0090
#define DT_HD_REG_S1_NEXTFRM_ADDR       0x0094
#define DT_HD_REG_FIFO_FIRST            0x0098
#define DT_HD_REG_FIFO_LAST             0x009C
#define DT_HD_REG_SDIFORMAT2            0x00A0
#define DT_HD_REG_FRMCONF6              0x00A4
#define DT_HD_REG_FRMCONF7              0x00A8

#define DT_HD_REG_LMH0387SPI            0x0188  // NOTE: WE ASSUME THE CHANNEL SPI CONTROL 
                                                // BLOCK IS LOCATED AT AN OFFSET (0x180)
                                                // FROM THE RX/TX CHANNEL BASE ADDRESS

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
#define DT_HD_CTRL1_DBLBUF_MSK           0x00000F00
#define DT_HD_CTRL1_DBLBUF_SH            8
#define DT_HD_CTRL1_OPMODE_MSK           0x00007000
#define DT_HD_CTRL1_OPMODE_SH            12
#define DT_HD_CTRL1_RXTXCTRL_MSK         0x00018000
#define DT_HD_CTRL1_RXTXCTRL_SH          15
#define DT_HD_CTRL1_TXNOVPIDREPLACE_MSK  0x00020000 
#define DT_HD_CTRL1_TXNOVPIDREPLACE_SH   17
#define DT_HD_CTRL1_ASIINVERT_MSK        0x000C0000
#define DT_HD_CTRL1_ASIINVERT_SH         18
#define DT_HD_CTRL1_ASIRXMODE_MSK        0x00E00000
#define DT_HD_CTRL1_ASIRXMODE_SH         21
#define DT_HD_CTRL1_ASIRXTSENA_MSK       0x01000000
#define DT_HD_CTRL1_ASIRXTSENA_SH        24
#define DT_HD_CTRL1_ASIRXTSSIZE_MSK      0x02000000
#define DT_HD_CTRL1_ASIRXTSSIZE_SH       25
#define DT_HD_CTRL1_TXNOEAV_MSK          0x04000000
#define DT_HD_CTRL1_TXNOEAV_SH           26
#define DT_HD_CTRL1_TXNOSAV_MSK          0x08000000
#define DT_HD_CTRL1_TXNOSAV_SH           27
#define DT_HD_CTRL1_RXSYNCERRINTEN_MSK   0x10000000
#define DT_HD_CTRL1_RXSYNCERRINTEN_SH    28
#define DT_HD_CTRL1_RXOVFERRINTEN_MSK    0x20000000
#define DT_HD_CTRL1_RXOVFERRINTEN_SH     29
#define DT_HD_CTRL1_LASTFRMINTEN_MSK     0x80000000
#define DT_HD_CTRL1_LASTFRMINTEN_SH      31

// HD-Channel Control register: io-direction
#define DT_HD_IODIR_OUTPUT               0x0
#define DT_HD_IODIR_INPUT                0x1

// HD-Channel Control register: double-buffered value
#define DT_HD_DBLBUF_NONE               0x0
#define DT_HD_DBLBUF_PORT1              0x1
#define DT_HD_DBLBUF_PORT2              0x2
#define DT_HD_DBLBUF_PORT3              0x3
#define DT_HD_DBLBUF_PORT4              0x4
#define DT_HD_DBLBUF_PORT5              0x5
#define DT_HD_DBLBUF_PORT6              0x6
#define DT_HD_DBLBUF_PORT7              0x7

// HD-Channel Control register: op-mode value
#define DT_HD_OPMODE_DISABLE            0x0
#define DT_HD_OPMODE_SD                 0x1
#define DT_HD_OPMODE_HD                 0x2
#define DT_HD_OPMODE_3G                 0x3
#define DT_HD_OPMODE_ASI                0x4
#define DT_HD_OPMODE_6G                 0x5
#define DT_HD_OPMODE_12G                0x6

// HD-Channel COntrol register: ASI Invert
#define DT_ASI_INV_RX_AUTO              0
#define DT_ASI_INV_RX_NORMAL            2
#define DT_ASI_INV_RX_INVERT            3

#define DT_ASI_INV_TX_NORMAL            0
#define DT_ASI_INV_TX_INVERT            1

// HD-Channel Control register: rx/tx control values
#define DT_HD_RXTXCTRL_IDLE             0x0
#define DT_HD_RXTXCTRL_FREEZE           0x1
#define DT_HD_RXTXCTRL_RUN              0x2

//-.-.-.-.-.-.-.-.-.-.-.- HD-Channel Control2 register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.-
//

#define DT_HD_CTRL2_REPFRM_MSK           0x00000001
#define DT_HD_CTRL2_REPFRM_SH            0
#define DT_HD_CTRL2_REALIGN_MSK          0x00000002
#define DT_HD_CTRL2_REALIGN_SH           1
#define DT_HD_CTRL2_FRACCLKSEL_MSK       0x00000004
#define DT_HD_CTRL2_FRACCLKSEL_SH        2
#define DT_HD_CTRL2_LVLBCONVEN_MSK       0x00000008
#define DT_HD_CTRL2_LVLBCONVEN_SH        3
#define DT_HD_CTRL2_IOSERDESRST_MSK      0x00000100
#define DT_HD_CTRL2_IOSERDESRST_SH       8


//.-.-.-.-.-.-.-.-.-.-.-.- HD-Channel Status register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.-.

#define  DT_HD_STATUS_RXLOCK_MSK        0x00000200
#define  DT_HD_STATUS_RXLOCK_SH         9
#define  DT_HD_STATUS_CD_MSK            0x00000400
#define  DT_HD_STATUS_CD_SH             10
#define  DT_HD_STATUS_CUR_LVLABFRM_MSK  0x00000800
#define  DT_HD_STATUS_CUR_LVLABFRM_SH   11
#define  DT_HD_STATUS_ASI_PCKSZ_MSK     0x06000000
#define  DT_HD_STATUS_ASI_PCKSZ_SH      25
#define  DT_HD_STATUS_ASIINV_MSK        0x08000000
#define  DT_HD_STATUS_ASIINV_SH         27
#define  DT_HD_STATUS_RXSYNCERRINT_MSK  0x10000000
#define  DT_HD_STATUS_RXSYNCERRINT_SH   28
#define  DT_HD_STATUS_TXSYNCERRINT_MSK  DT_HD_STATUS_RXSYNCERRINT_MSK
#define  DT_HD_STATUS_TXSYNCERRINT_SH   DT_HD_STATUS_RXSYNCERRINT_SH
#define  DT_HD_STATUS_RXOVFERRINT_MSK   0x20000000
#define  DT_HD_STATUS_RXOVFERRINT_SH    29
#define  DT_HD_STATUS_TXUFLERRINT_MSK   DT_HD_STATUS_RXOVFERRINT_MSK
#define  DT_HD_STATUS_TXUFLERRINT_SH    DT_HD_STATUS_RXOVFERRINT_SH
#define  DT_HD_STATUS_LASTFRMINT_MSK    0x80000000
#define  DT_HD_STATUS_LASTFRMINT_SH     31

// HD-Channel Status register: detected video standard values 
#define  DT_VIDSTD_UNKNOWN              0x0000
#define  DT_VIDSTD_525I59_94            0x01D6
#define  DT_VIDSTD_625I50               0x0158
#define  DT_VIDSTD_720P23_98            0x0188
#define  DT_VIDSTD_720P24               0x0108
#define  DT_VIDSTD_720P25               0x0106
#define  DT_VIDSTD_720P29_97            0x0182
#define  DT_VIDSTD_720P30               0x0102
#define  DT_VIDSTD_720P50               0x0104
#define  DT_VIDSTD_720P59_94            0x0180
#define  DT_VIDSTD_720P60               0x0100
#define  DT_VIDSTD_1080P23_98           0x0190
#define  DT_VIDSTD_1080P24              0x0110
#define  DT_VIDSTD_1080P25              0x010D
#define  DT_VIDSTD_1080P29_97           0x018B
#define  DT_VIDSTD_1080P30              0x010B
#define  DT_VIDSTD_1080I50              0x014C
#define  DT_VIDSTD_1080I59_94           0x01CA
#define  DT_VIDSTD_1080I60              0x014A
#define  DT_VIDSTD_1080P50              0x010C
#define  DT_VIDSTD_1080P59_94           0x018A
#define  DT_VIDSTD_1080P60              0x010A
#define  DT_VIDSTD_1080P50B             0x030C
#define  DT_VIDSTD_1080P59_94B          0x038A
#define  DT_VIDSTD_1080P60B             0x030A
#define  DT_VIDSTD_1080PSF23_98         0x0001
#define  DT_VIDSTD_1080PSF24            0x0002
#define  DT_VIDSTD_1080PSF25            0x0003
#define  DT_VIDSTD_1080PSF29_97         0x0004
#define  DT_VIDSTD_1080PSF30            0x0005
#define  DT_VIDSTD_480P59_94            0x0006
#define  DT_VIDSTD_525P59_94            0x0007
#define  DT_VIDSTD_625P50               0x0008
// 6G 2160 formats
#define  DT_VIDSTD_2160P23_98           0x1001
#define  DT_VIDSTD_2160P24              0x1002
#define  DT_VIDSTD_2160P25              0x1003
#define  DT_VIDSTD_2160P29_97           0x1004
#define  DT_VIDSTD_2160P30              0x1005
// 12G 2160 formats
#define  DT_VIDSTD_2160P50              0x1006
#define  DT_VIDSTD_2160P59_94           0x1007
#define  DT_VIDSTD_2160P60              0x1008
#define  DT_VIDSTD_2160P50B             0x1009
#define  DT_VIDSTD_2160P59_94B          0x100A
#define  DT_VIDSTD_2160P60B             0x100B

#define  DT_VIDSTD_TS                   -1      // Special case


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

//-.-.-.-.-.-.-.-.-.-.- HD-Channel  SDI Format register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.
#define  DT_HD_SDIFMT_VIDEOID_MSK        0x000000FF
#define  DT_HD_SDIFMT_VIDEOID_SH         0
#define  DT_HD_SDIFMT_PICTRATE_MSK       0x00000F00
#define  DT_HD_SDIFMT_PICTRATE_SH        8
#define  DT_HD_SDIFMT_PROGRESSIVE_MSK    0x0000C000
#define  DT_HD_SDIFMT_PROGRESSIVE_SH     14

// HD-Channel SDI Format register: Video ID values
#define  DT_HD_SDIFMT_VIDEOID_SD         129
#define  DT_HD_SDIFMT_VIDEOID_HD720      132
#define  DT_HD_SDIFMT_VIDEOID_HD1080     133
#define  DT_HD_SDIFMT_VIDEOID_3GLVLA     137
#define  DT_HD_SDIFMT_VIDEOID_3GLVLB     138
#define  DT_HD_SDIFMT_VIDEOID_6G         192
#define  DT_HD_SDIFMT_VIDEOID_12G        206  
#define  DT_HD_SDIFMT_VIDEOID_12G_ADEAS  161    // TODOTD can be removed after fix


// HD-Channel SDI Format register: Picture rate values
#define  DT_HD_SDIFMT_PICTRATE_23_98     2     
#define  DT_HD_SDIFMT_PICTRATE_24        3
#define  DT_HD_SDIFMT_PICTRATE_25        5
#define  DT_HD_SDIFMT_PICTRATE_29_97     6
#define  DT_HD_SDIFMT_PICTRATE_30        7
#define  DT_HD_SDIFMT_PICTRATE_50        9
#define  DT_HD_SDIFMT_PICTRATE_59_94     10
#define  DT_HD_SDIFMT_PICTRATE_60        11

// HD-Channel SDI Format register: Progressive values
#define  DT_HD_SDIFMT_PROGRESSIVE_OFF    0x0
#define  DT_HD_SDIFMT_PROGRESSIVE_ON     0x3
#define  DT_HD_SDIFMT_PROGRESSIVE_PSF    0x1

//-.-.-.-.-.-.-.-.-.-.- HD-Channel  SDI Status register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.
// NOTE: bit fields are identical to SDI Format register
#define  DT_HD_SDISTAT_VIDEOID_MSK       DT_HD_SDIFMT_VIDEOID_MSK    
#define  DT_HD_SDISTAT_VIDEOID_SH        DT_HD_SDIFMT_VIDEOID_SH     
#define  DT_HD_SDISTAT_PICTRATE_MSK      DT_HD_SDIFMT_PICTRATE_MSK   
#define  DT_HD_SDISTAT_PICTRATE_SH       DT_HD_SDIFMT_PICTRATE_SH    
#define  DT_HD_SDISTAT_PROGRESSIVE_MSK   DT_HD_SDIFMT_PROGRESSIVE_MSK
#define  DT_HD_SDISTAT_PROGRESSIVE_SH    DT_HD_SDIFMT_PROGRESSIVE_SH 

// HD-Channel SDI Format register: Video ID values
#define  DT_HD_SDISTAT_VIDEOID_SD        DT_HD_SDIFMT_VIDEOID_SD    
#define  DT_HD_SDISTAT_VIDEOID_HD720     DT_HD_SDIFMT_VIDEOID_HD720 
#define  DT_HD_SDISTAT_VIDEOID_HD1080    DT_HD_SDIFMT_VIDEOID_HD1080
#define  DT_HD_SDISTAT_VIDEOID_3GLVLA    DT_HD_SDIFMT_VIDEOID_3GLVLA
#define  DT_HD_SDISTAT_VIDEOID_3GLVLB    DT_HD_SDIFMT_VIDEOID_3GLVLB

// HD-Channel SDI Format register: Picture rate values
#define  DT_HD_SDISTAT_PICTRATE_23_98    DT_HD_SDIFMT_PICTRATE_23_98    
#define  DT_HD_SDISTAT_PICTRATE_24       DT_HD_SDIFMT_PICTRATE_24   
#define  DT_HD_SDISTAT_PICTRATE_25       DT_HD_SDIFMT_PICTRATE_25   
#define  DT_HD_SDISTAT_PICTRATE_29_97    DT_HD_SDIFMT_PICTRATE_29_97
#define  DT_HD_SDISTAT_PICTRATE_30       DT_HD_SDIFMT_PICTRATE_30   
#define  DT_HD_SDISTAT_PICTRATE_50       DT_HD_SDIFMT_PICTRATE_50   
#define  DT_HD_SDISTAT_PICTRATE_59_94    DT_HD_SDIFMT_PICTRATE_59_94
#define  DT_HD_SDISTAT_PICTRATE_60       DT_HD_SDIFMT_PICTRATE_60   

// HD-Channel SDI Format register: Progressive values
#define  DT_HD_SDISTAT_PROGRESSIVE_OFF   DT_HD_SDIFMT_PROGRESSIVE_OFF
#define  DT_HD_SDISTAT_PROGRESSIVE_ON    DT_HD_SDIFMT_PROGRESSIVE_ON 

//-.-.-.-.-.-.-.-.- HD-Channel Mem-Transfer Control register: Bit Fields -.-.-.-.-.-.-.-.-

#define DT_HD_MEMTRCTRL_NUML_MSK        0x0000FFFF
#define DT_HD_MEMTRCTRL_NUML_SH         0
#define DT_HD_MEMTRCTRL_TRCMD_MSK       0x00070000
#define DT_HD_MEMTRCTRL_TRCMD_SH        16
#define DT_HD_MEMTRCTRL_DATAFMT_MSK     0x00180000
#define DT_HD_MEMTRCTRL_DATAFMT_SH      19
#define DT_HD_MEMTRCTRL_RGBMODE_MSK     0x00200000
#define DT_HD_MEMTRCTRL_RGBMODE_SH      21
#define DT_HD_MEMTRCTRL_SYMFLTMODE_MSK  0x00C00000
#define DT_HD_MEMTRCTRL_SYMFLTMODE_SH   22
#define DT_HD_MEMTRCTRL_SCMODE_MSK      0x06000000
#define DT_HD_MEMTRCTRL_SCMODE_SH       25
#define DT_HD_MEMTRCTRL_ABORT_MSK       0x08000000
#define DT_HD_MEMTRCTRL_ABORT_SH        27
#define DT_HD_MEMTRCTRL_ANCFLTMODE_MSK  0x70000000
#define DT_HD_MEMTRCTRL_ANCFLTMODE_SH   28
#define DT_HD_MEMTRCTRL_STRIDEMODE_MSK  0x80000000
#define DT_HD_MEMTRCTRL_STRIDEMODE_SH   31

// HD-Channel Mem-Transfer register: Transfer command values
#define  DT_MEMTR_TRCMD_IDLE            0x0
#define  DT_MEMTR_TRCMD_SECT0           0x1
#define  DT_MEMTR_TRCMD_SECT1           0x2
#define  DT_MEMTR_TRCMD_ASIRD           0x3
#define  DT_MEMTR_TRCMD_DUALFLD         0x4
#define  DT_MEMTR_TRCMD_ASIWR           0x5
#define  DT_MEMTR_TRCMD_FRAME           0x7

// HD-Channel Mem-Transfer register: Data format values
#define  DT_MEMTR_DATAFMT_8B            0x0
#define  DT_MEMTR_DATAFMT_10B           0x1
#define  DT_MEMTR_DATAFMT_16B           0x2
#define  DT_MEMTR_DATAFMT_10B_NBO       0x3

// HD-Channel Mem-Transfer register: RGB-mode values
#define  DT_MEMTR_RGBMODE_OFF           0x0
#define  DT_MEMTR_RGBMODE_ON            0x1

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

//.-.-.-.-.-.-.-.-.-.- HD-Channel Frame Config 6 register: Bit Fields -.-.-.-.-.-.-.-.-.-.

#define DT_HD_FRMCONF6_VPIDLINE1_MSK    0x0000FFFF
#define DT_HD_FRMCONF6_VPIDLINE1_SH     0
#define DT_HD_FRMCONF6_VPIDLINE2_MSK    0xFFFF0000
#define DT_HD_FRMCONF6_VPIDLINE2_SH     16

//.-.-.-.-.-.-.-.-.-.- HD-Channel Frame Config 7 register: Bit Fields -.-.-.-.-.-.-.-.-.-.

#define DT_HD_FRMCONF7_PIXELDELAY_MSK   0x0000FFFF
#define DT_HD_FRMCONF7_PIXELDELAY_SH    0

//-.-.-.-.-.-.-.-.-.-.- HD-Channel SPI Control register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.
#define DT_HD_SPICTRL_DATA_MSK    0x0000FFFF
#define DT_HD_SPICTRL_DATA_SH     0
#define DT_HD_SPICTRL_ADDR_MSK    0x0FFF0000
#define DT_HD_SPICTRL_ADDR_SH     16
#define DT_HD_SPICTRL_AUTOINC_MSK 0x10000000
#define DT_HD_SPICTRL_AUTOINC_SH  28
#define DT_HD_SPICTRL_DEVSEL_MSK  0x10000000
#define DT_HD_SPICTRL_DEVSEL_SH   28
#define DT_HD_SPICTRL_READ_MSK    0x20000000
#define DT_HD_SPICTRL_READ_SH     29
#define DT_HD_SPICTRL_START_MSK   0x40000000
#define DT_HD_SPICTRL_START_SH    30
#define DT_HD_SPICTRL_DONE_MSK    0x80000000
#define DT_HD_SPICTRL_DONE_SH     31



//-.-.-.-.-.-.- HD-Channel S0 Next Frame Start Address register: Bit Fields -.-.-.-.-.-.-.

#define DT_HD_S0NEXTFRMADDR_ADDR_MSK      0xFFFFFFFF
#define DT_HD_S0NEXTFRMADDR_ADDR_SH       0

//-.-.-.-.-.-.- HD-Channel S1 Next Frame Start Address register: Bit Fields -.-.-.-.-.-.-.

#define DT_HD_S1NEXTFRMADDR_ADDR_MSK      0xFFFFFFFF
#define DT_HD_S1NEXTFRMADDR_ADDR_SH       0

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ RS-422 Registers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

#define DT_RS422_REG_CTRL               0x0000
#define DT_RS422_REG_TX_DATA            0x0004
#define DT_RS422_REG_STAT               0x0008
#define DT_RS422_REG_RX_DATA            0x000C

//-.-.-.-.-.-.-.-.-.-.-.-.- RS-422 Control register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.-.-.

#define DT_RS422_CTRL_MASTER_MSK         0x00000001
#define DT_RS422_CTRL_MASTER_SH          0
#define DT_RS422_CTRL_POLINV_MSK         0x00000002
#define DT_RS422_CTRL_POLINV_SH          1
#define DT_RS422_CTRL_TXSTART_MSK        0x00000010
#define DT_RS422_CTRL_TXSTART_SH         4
#define DT_RS422_CTRL_RXENA_MSK          0x00000020
#define DT_RS422_CTRL_RXENA_SH           5
#define DT_RS422_CTRL_RX_FIFO_CLR_MSK    0x00000100
#define DT_RS422_CTRL_RX_FIFO_CLR_SH     8
#define DT_RS422_CTRL_TX_FIFO_CLR_MSK    0x00000200
#define DT_RS422_CTRL_TX_FIFO_CLR_SH     9
#define DT_RS422_CTRL_RX_DATA_INT_EN_MSK 0x40000000
#define DT_RS422_CTRL_RX_DATA_INT_EN_SH  30
#define DT_RS422_CTRL_TX_RDY_INT_EN_MSK  0x80000000
#define DT_RS422_CTRL_TX_RDY_INT_EN_SH   31

//.-.-.-.-.-.-.-.-.-.-.-.-.- RS-422 Status register: Bit Fields -.-.-.-.-.-.-.-.-.-.-.-.-.

#define DT_RS422_STAT_RX_NUMBYTES_MSK    0x000000FF
#define DT_RS422_STAT_RX_NUMBYTES_SH     0
#define DT_RS422_STAT_RX_DATA_INT_MSK    0x40000000
#define DT_RS422_STAT_RX_DATA_INT_SH     30
#define DT_RS422_STAT_TX_RDY_INT_MSK     0x80000000
#define DT_RS422_STAT_TX_RDY_INT_SH      31

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- RS-422 RX data register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

#define DT_RS422_RXDATA_MSK              0x000000FF
#define DT_RS422_RXDATA_SH               0

//-.-.-.-.-.-.-.-.-.- Fast-flash programming interface register offsets -.-.-.-.-.-.-.-.-.

#define DT_REG_FAST_FLASH_PROG_CTRL             0x00
#define DT_REG_FAST_FLASH_PROG_ADDR             0x04
#define DT_REG_FAST_FLASH_PROG_DATA             0x08
#define DT_REG_FAST_FLASH_PROG_NBR_READ_WORDS   0x0C
#define DT_REG_FAST_FLASH_PROG_UNLOCK           0x10

//.-.-.-.-.-.- Fast-flash programming interface control register: Bit fields -.-.-.-.-.-.-

#define DT_REG_FAST_FLASH_PROG_CTRL_BUSY_MSK            0x00000001
#define DT_REG_FAST_FLASH_PROG_CTRL_BUSY_SH             0
#define DT_REG_FAST_FLASH_PROG_CTRL_DATA_VALID_MSK      0x00000002
#define DT_REG_FAST_FLASH_PROG_CTRL_DATA_VALID_SH       1

// THESE FIELDS ARE DEPRECATED AND REPLACED BY SINGLE 3-BIT COMMAND FIELD
#define DT_REG_FAST_FLASH_PROG_CTRL_READ_STATUS_CMD_MSK 0x00000004
#define DT_REG_FAST_FLASH_PROG_CTRL_READ_STATUS_CMD_SH  2
#define DT_REG_FAST_FLASH_PROG_CTRL_READ_DATA_CMD_MSK   0x00000008
#define DT_REG_FAST_FLASH_PROG_CTRL_READ_DATA_CMD_SH    3

// THESE FIELDS ARE DEPRECATED AND REPLACED BY SINGLE 3-BIT COMMAND FIELD
#define DT_REG_FAST_FLASH_PROG_CTRL_COMMAND_MSK         0x0000001C
#define DT_REG_FAST_FLASH_PROG_CTRL_COMMAND_SH          2

#define DT_REG_FAST_FLASH_PROG_CTRL_RECONFIGDELAY_MSK   0x0001FF00
#define DT_REG_FAST_FLASH_PROG_CTRL_RECONFIGDELAY_SH    8

//.-.-.-.-.-.- Fast-flash programming interface address register: Bit fields -.-.-.-.-.-.-

#define DT_REG_FAST_FLASH_PROG_ADDR_MSK                 0x007FFFFF // 23 bits wide
#define DT_REG_FAST_FLASH_PROG_ADDR_SH                  0

//.-.-.-.-.-.-.- Fast-flash programming interface data register: Bit fields -.-.-.-.-.-.-.

#define DT_REG_FAST_FLASH_PROG_DATA_MSK                 0x0000FFFF // 16 bits wide
#define DT_REG_FAST_FLASH_PROG_DATA_SH                  0

//.-.-.- Fast-flash programming interface number of read words register: Bit fields -.-.-.

#define DT_REG_FAST_FLASH_PROG_NBR_READ_WORDS_MSK       0x0000001F // 5 bits wide
#define DT_REG_FAST_FLASH_PROG_NBR_READ_WORDS_SH        0

//-.-.-.-.-.-.- Fast-flash programming interface unlock register: Bit fields -.-.-.-.-.-.-

#define DT_REG_FAST_FLASH_PROG_UNLOCK_MSK               0x0000000F // 4 bits wide
#define DT_REG_FAST_FLASH_PROG_UNLOCK_SH                0
#define DT_REG_FAST_FLASH_PROG_LOCKBIT_MSK              0x00000010
#define DT_REG_FAST_FLASH_PROG_LOCKBIT_SH               4

#endif // __DT_REGS_H
