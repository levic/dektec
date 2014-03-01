//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaPropertyStore.c *#*#*#*#*#*#*#*#*# (C) 2013 DekTec
//
// CapParser - Property Store - Tables storing device properties
//
// THIS FILE IS GENERATED, DO NOT EDIT!!!

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#include "DtaIncludes.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-100; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DVB-ASI output
// 
static const DtProperty  DtProperties100_0_01[] =
{
    { "FIRMWARE_LAST_VER", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_02[] =
{
    { "FIRMWARE_R_N3_0", -1, "Add clear-FIFO function next to full channel reset",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_03[] =
{
    { "FIRMWARE_R_N4_0", -1, "No functional changes", PROPERTY_VALUE_TYPE_STRING, -1, 0,
                                                       0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_CHAN0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_04[] =
{
    { "FIRMWARE_R_N4_1", -1, "Reduce on-board noise level by improving FPGA equations",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N5_0", -1, "Adds support for transmit mode DTAPI_TXMODE_MIN16",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_05[] =
{
    { "FIRMWARE_R_N6_0", -1, "Adds support for in-system upgrading of firmware",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_08[] =
{
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_LATENCY_TIMER", 64, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 27000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x00, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_HOTLINK, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0,
                                                      0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x07F, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x00, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 300, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN2_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN2_PORT", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0xD100, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties100_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x14B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_OLD, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_56[] =
{
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_59[] =
{
    { "VPD_ID", -1, "DVB-ASI output", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_60[] =
{
    { "PCI_DEVICE_ID", 0x9054, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets100_0[] =
{
    { NULL, 0 },
    { DtProperties100_0_01, 2 },
    { DtProperties100_0_02, 1 },
    { DtProperties100_0_03, 2 },
    { DtProperties100_0_04, 3 },
    { DtProperties100_0_05, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_0_08, 4 },
    { DtProperties100_0_09, 1 },
    { DtProperties100_0_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_0_14, 3 },
    { NULL, 0 },
    { DtProperties100_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_0_20, 1 },
    { DtProperties100_0_21, 1 },
    { DtProperties100_0_22, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_0_25, 4 },
    { DtProperties100_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_0_29, 1 },
    { DtProperties100_0_30, 1 },
    { DtProperties100_0_31, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_0_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_0_39, 1 },
    { DtProperties100_0_40, 2 },
    { NULL, 0 },
    { DtProperties100_0_42, 1 },
    { DtProperties100_0_43, 2 },
    { DtProperties100_0_44, 1 },
    { NULL, 0 },
    { DtProperties100_0_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_0_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_0_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_0_59, 1 },
    { DtProperties100_0_60, 1 },
    { DtProperties100_0_61, 1 },
    { NULL, 0 },
    { DtProperties100_0_63, 1 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-102; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DVB-spi output
// 
static const DtProperty  DtProperties102_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_01[] =
{
    { "FIRMWARE_LAST_VER", 8, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Add support for 192-byte packets",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_02[] =
{
    { "FIRMWARE_R_N3_0", -1,
      "After power-up/hard-reset: LVDS transmitters start up in tri-state mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_03[] =
{
    { "FIRMWARE_R_N3_1", -1, "Add LED flashing upon power up",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N4_0", -1, "Avoid read-back error while LVDS buffers are disabled",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_CHAN0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_04[] =
{
    { "CAP_SPI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N4_1", -1,
      "Short-circuit error has been made independent from the tx-clock rate",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N5_0", -1, "Avoid generation of partial packets at soft/hard resets",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_05[] =
{
    { "FIRMWARE_R_N5_1", -1, "Null-packet stuffing powers up in 'off' state",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N6_0", -1, "Reduce on-board noise level by improving FPGA equations",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_06[] =
{
    { "FIRMWARE_R_N5_2", -1, "Add LED-control bits to Receive-Control register",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N7_0", -1, "Add support for 130-byte packets",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_07[] =
{
    { "FIRMWARE_R_N5_3", -1,
      "Start-up LED flashing pattern now includes type number and fw-version",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N8_0", -1, "Add support for packet transmit mode DTAPI_TXMODE_MIN16",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_08[] =
{
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_LATENCY_TIMER", 64, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 27000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_SPI", 0x00, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x00, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_10[] =
{
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_14[] =
{
    { "CAP_SPILVDS1", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x07F, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x00, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN2_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN2_PORT", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0xD102, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_28[] =
{
    { "CAP_EXTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_32[] =
{
    { "DEFAULT_SPISTD", DT_IOCONFIG_SPILVDS1, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x14B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_37[] =
{
    { "CAP_SPIDVBMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_OLD, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_56[] =
{
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_59[] =
{
    { "VPD_ID", -1, "DVB-spi output", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_60[] =
{
    { "PCI_DEVICE_ID", 0x9054, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets102_0[] =
{
    { DtProperties102_0_00, 1 },
    { DtProperties102_0_01, 2 },
    { DtProperties102_0_02, 1 },
    { DtProperties102_0_03, 3 },
    { DtProperties102_0_04, 4 },
    { DtProperties102_0_05, 2 },
    { DtProperties102_0_06, 2 },
    { DtProperties102_0_07, 2 },
    { DtProperties102_0_08, 5 },
    { DtProperties102_0_09, 1 },
    { DtProperties102_0_10, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties102_0_14, 4 },
    { NULL, 0 },
    { DtProperties102_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties102_0_20, 1 },
    { NULL, 0 },
    { DtProperties102_0_22, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties102_0_25, 4 },
    { DtProperties102_0_26, 3 },
    { NULL, 0 },
    { DtProperties102_0_28, 1 },
    { DtProperties102_0_29, 1 },
    { DtProperties102_0_30, 1 },
    { NULL, 0 },
    { DtProperties102_0_32, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties102_0_35, 2 },
    { NULL, 0 },
    { DtProperties102_0_37, 1 },
    { NULL, 0 },
    { DtProperties102_0_39, 1 },
    { DtProperties102_0_40, 2 },
    { NULL, 0 },
    { DtProperties102_0_42, 1 },
    { DtProperties102_0_43, 2 },
    { DtProperties102_0_44, 1 },
    { NULL, 0 },
    { DtProperties102_0_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties102_0_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties102_0_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties102_0_59, 1 },
    { DtProperties102_0_60, 1 },
    { DtProperties102_0_61, 1 },
    { NULL, 0 },
    { DtProperties102_0_63, 1 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-105; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Dual DVB-ASI output
// 
static const DtProperty  DtProperties105_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Few minor bug fixes", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_01[] =
{
    { "FIRMWARE_LAST_VER", 3, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Bug fix: deadlock scenario on 66MHz PCI bus",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_02[] =
{
    { "FIRMWARE_R_N2_1", -1, "Bug fix: maximum FIFO size setting was ignored",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_0", -1, "Bug fix: stale of dma engine resulting in a system lockup",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_03[] =
{
    { "FIRMWARE_R_N3_1", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_08[] =
{
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x180, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x380, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_39[] =
{
    { "CAP_DBLBUF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 2, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_59[] =
{
    { "VPD_ID", -1, "Dual DVB-ASI output", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_60[] =
{
    { "PCI_DEVICE_ID", 0x69, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets105_0[] =
{
    { DtProperties105_0_00, 3 },
    { DtProperties105_0_01, 2 },
    { DtProperties105_0_02, 2 },
    { DtProperties105_0_03, 1 },
    { DtProperties105_0_04, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_0_08, 5 },
    { DtProperties105_0_09, 2 },
    { DtProperties105_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_0_14, 3 },
    { NULL, 0 },
    { DtProperties105_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_0_20, 1 },
    { DtProperties105_0_21, 2 },
    { DtProperties105_0_22, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_0_25, 2 },
    { DtProperties105_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_0_29, 2 },
    { DtProperties105_0_30, 1 },
    { DtProperties105_0_31, 2 },
    { DtProperties105_0_32, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_0_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_0_39, 2 },
    { DtProperties105_0_40, 3 },
    { NULL, 0 },
    { DtProperties105_0_42, 1 },
    { DtProperties105_0_43, 2 },
    { DtProperties105_0_44, 1 },
    { NULL, 0 },
    { DtProperties105_0_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_0_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_0_59, 1 },
    { DtProperties105_0_60, 1 },
    { DtProperties105_0_61, 2 },
    { NULL, 0 },
    { DtProperties105_0_63, 3 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-107; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DVB-S L-band modulator
// 
static const DtProperty  DtProperties107_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1,
      "Fixes synchronisation problems with 204-byte Transport-Streams",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_01[] =
{
    { "FIRMWARE_LAST_VER", 3, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Support for DVB-S2 (DTC-372-S2 license required)",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_02[] =
{
    { "FIRMWARE_R_N3_0", -1,
      "Support for SNR and RF level control (DTC-305-CM license required)",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_RFPLL", 107, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_03[] =
{
    { "MOD_HW_HAS_AD9789", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_04[] =
{
    { "CAP_TX_DVBS2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 107, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_08[] =
{
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_RANGE", 200, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_LATENCY_TIMER", 64, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 25000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x080, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_10[] =
{
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_11[] =
{
    { "CAP_SNR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties107_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x1FF, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_18[] =
{
    { "CAP_TX_DVBS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties107_0_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_ISDBS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 400, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN2_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN2_PORT", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0xD107, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_33[] =
{
    { "MOD_LEVEL_MAX", -270, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 88000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 88000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 45000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x14B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 45000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 700, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MIN", 88000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 2300, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 45000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_51[] =
{
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties107_0_53[] =
{
    { "CAP_ADJLVL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 950, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_55[] =
{
    { "MOD_FREQ_MAX", 2150, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 88000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_57[] =
{
    { "CAP_LBAND", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties107_0_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 45000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_59[] =
{
    { "MOD_HW_HAS_SYMIQ_MAPPING", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "DVB-S L-band modulator", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_60[] =
{
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets107_0[] =
{
    { DtProperties107_0_00, 1 },
    { DtProperties107_0_01, 3 },
    { DtProperties107_0_02, 2 },
    { DtProperties107_0_03, 2 },
    { DtProperties107_0_04, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties107_0_08, 5 },
    { DtProperties107_0_09, 1 },
    { DtProperties107_0_10, 2 },
    { DtProperties107_0_11, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties107_0_14, 3 },
    { NULL, 0 },
    { DtProperties107_0_16, 1 },
    { NULL, 0 },
    { DtProperties107_0_18, 1 },
    { NULL, 0 },
    { DtProperties107_0_20, 2 },
    { NULL, 0 },
    { DtProperties107_0_22, 3 },
    { NULL, 0 },
    { DtProperties107_0_24, 2 },
    { DtProperties107_0_25, 4 },
    { DtProperties107_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties107_0_29, 1 },
    { DtProperties107_0_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties107_0_33, 2 },
    { DtProperties107_0_34, 1 },
    { DtProperties107_0_35, 3 },
    { DtProperties107_0_36, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties107_0_39, 1 },
    { DtProperties107_0_40, 2 },
    { NULL, 0 },
    { DtProperties107_0_42, 1 },
    { DtProperties107_0_43, 4 },
    { DtProperties107_0_44, 1 },
    { DtProperties107_0_45, 2 },
    { DtProperties107_0_46, 2 },
    { DtProperties107_0_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties107_0_50, 1 },
    { DtProperties107_0_51, 1 },
    { NULL, 0 },
    { DtProperties107_0_53, 3 },
    { NULL, 0 },
    { DtProperties107_0_55, 1 },
    { DtProperties107_0_56, 3 },
    { DtProperties107_0_57, 1 },
    { DtProperties107_0_58, 1 },
    { DtProperties107_0_59, 2 },
    { DtProperties107_0_60, 1 },
    { DtProperties107_0_61, 1 },
    { NULL, 0 },
    { DtProperties107_0_63, 2 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-110; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// QAM UHF modulator
// 
static const DtProperty  DtProperties110_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "Test release", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_01[] =
{
    { "FIRMWARE_LAST_VER", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1,
      "Add support for OFDM modulation (DTA-110T license required)",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_02[] =
{
    { "FIRMWARE_R_N3_0", -1, "Add support for setting carrier frequency in 0.1MHz steps",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_RFPLL", 110, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 107, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_03[] =
{
    { "CAP_TX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "FIRMWARE_R_N4_0", -1, "Support for DTMB (DTC-374-DTMB license required)",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_AD9789", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_04[] =
{
    { "FIRMWARE_R_N4_1", -1, "Support for QAM-B", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 110, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 110, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties110_0_08[] =
{
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_RANGE", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_LATENCY_TIMER", 96, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 25000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x080, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_10[] =
{
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x1FF, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 300, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN2_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN2_PORT", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0xD10A, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_33[] =
{
    { "MOD_LEVEL_MAX", -270, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 5000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 5000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 7100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x14B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 7100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 400, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MIN", 5000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 870, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 7100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_53[] =
{
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 400, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_55[] =
{
    { "MOD_FREQ_MAX", 862, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 5000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 7100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "MOD_HW_HAS_SYMIQ_MAPPING", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "QAM UHF modulator", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_60[] =
{
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets110_0[] =
{
    { DtProperties110_0_00, 1 },
    { DtProperties110_0_01, 3 },
    { DtProperties110_0_02, 3 },
    { DtProperties110_0_03, 5 },
    { DtProperties110_0_04, 4 },
    { DtProperties110_0_05, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_0_08, 5 },
    { DtProperties110_0_09, 1 },
    { DtProperties110_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_0_14, 3 },
    { NULL, 0 },
    { DtProperties110_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_0_20, 2 },
    { NULL, 0 },
    { DtProperties110_0_22, 4 },
    { NULL, 0 },
    { DtProperties110_0_24, 1 },
    { DtProperties110_0_25, 4 },
    { DtProperties110_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_0_29, 1 },
    { DtProperties110_0_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_0_33, 2 },
    { DtProperties110_0_34, 1 },
    { DtProperties110_0_35, 3 },
    { DtProperties110_0_36, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_0_39, 1 },
    { DtProperties110_0_40, 2 },
    { NULL, 0 },
    { DtProperties110_0_42, 1 },
    { DtProperties110_0_43, 4 },
    { DtProperties110_0_44, 1 },
    { DtProperties110_0_45, 2 },
    { DtProperties110_0_46, 2 },
    { DtProperties110_0_47, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_0_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_0_53, 2 },
    { NULL, 0 },
    { DtProperties110_0_55, 1 },
    { DtProperties110_0_56, 3 },
    { NULL, 0 },
    { DtProperties110_0_58, 1 },
    { DtProperties110_0_59, 4 },
    { DtProperties110_0_60, 1 },
    { DtProperties110_0_61, 1 },
    { NULL, 0 },
    { DtProperties110_0_63, 2 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-111; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Multi-standard VHF/UHF modulator
// 
static const DtProperty  DtProperties111_0_01[] =
{
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 1, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_02[] =
{
    { "CAP_TX_CMMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_RFPLL", 115, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_03[] =
{
    { "CAP_TX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_HAS_AD9789", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_04[] =
{
    { "CAP_TX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 115, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties111_0_08[] =
{
    { "CAP_CM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "CAP_TX_T2SPLP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_RANGE", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_LATENCY_TIMER", 96, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x280, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_10[] =
{
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_14[] =
{
    { "CAP_TX_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x3FF, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties111_0_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties111_0_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties111_0_25[] =
{
    { "CAP_TX_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN2_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN2_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x6F, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_33[] =
{
    { "MOD_LEVEL_MAX", -150, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_41[] =
{
    { "CAP_TX_GOLD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties111_0_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_TX_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 30, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MIN", 1000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 999, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_51[] =
{
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties111_0_52[] =
{
    { "CAP_TX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_53[] =
{
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_55[] =
{
    { "MOD_FREQ_MAX", 862, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "MOD_HW_HAS_SYMIQ_MAPPING", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Multi-standard VHF/UHF modulator", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets111_0[] =
{
    { NULL, 0 },
    { DtProperties111_0_01, 2 },
    { DtProperties111_0_02, 2 },
    { DtProperties111_0_03, 3 },
    { DtProperties111_0_04, 3 },
    { DtProperties111_0_05, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties111_0_08, 7 },
    { DtProperties111_0_09, 1 },
    { DtProperties111_0_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties111_0_14, 4 },
    { NULL, 0 },
    { DtProperties111_0_16, 1 },
    { NULL, 0 },
    { DtProperties111_0_18, 1 },
    { DtProperties111_0_19, 1 },
    { DtProperties111_0_20, 2 },
    { NULL, 0 },
    { DtProperties111_0_22, 3 },
    { NULL, 0 },
    { DtProperties111_0_24, 2 },
    { DtProperties111_0_25, 5 },
    { DtProperties111_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties111_0_29, 2 },
    { DtProperties111_0_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties111_0_33, 2 },
    { DtProperties111_0_34, 1 },
    { DtProperties111_0_35, 3 },
    { DtProperties111_0_36, 1 },
    { DtProperties111_0_37, 1 },
    { NULL, 0 },
    { DtProperties111_0_39, 1 },
    { DtProperties111_0_40, 2 },
    { DtProperties111_0_41, 1 },
    { DtProperties111_0_42, 3 },
    { DtProperties111_0_43, 4 },
    { DtProperties111_0_44, 1 },
    { DtProperties111_0_45, 2 },
    { DtProperties111_0_46, 2 },
    { DtProperties111_0_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties111_0_50, 1 },
    { DtProperties111_0_51, 1 },
    { DtProperties111_0_52, 1 },
    { DtProperties111_0_53, 2 },
    { NULL, 0 },
    { DtProperties111_0_55, 1 },
    { DtProperties111_0_56, 3 },
    { NULL, 0 },
    { DtProperties111_0_58, 1 },
    { DtProperties111_0_59, 3 },
    { DtProperties111_0_60, 2 },
    { DtProperties111_0_61, 1 },
    { NULL, 0 },
    { DtProperties111_0_63, 2 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-112; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// ATSC/DVB-T/QAM VHF/UHF modulator
// 
static const DtProperty  DtProperties112_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_01[] =
{
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 2, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_02[] =
{
    { "CAP_TX_CMMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_RFPLL", 115, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_03[] =
{
    { "CAP_TX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_HAS_AD9789", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_04[] =
{
    { "CAP_TX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 115, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties112_0_06[] =
{
    { "REGISTERS_RX", 0x080, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_08[] =
{
    { "CAP_CM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_T2SPLP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_RANGE", 350, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_LATENCY_TIMER", 96, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x280, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_14[] =
{
    { "CAP_TX_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x3FF, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties112_0_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties112_0_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties112_0_25[] =
{
    { "CAP_TX_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 100, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN2_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN2_PORT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x70, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties112_0_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_33[] =
{
    { "MOD_LEVEL_MAX", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_TX_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 30, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MIN", 1000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 999, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties112_0_52[] =
{
    { "CAP_TX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_53[] =
{
    { "CAP_ADJLVL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_55[] =
{
    { "MOD_FREQ_MAX", 862, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "MOD_HW_HAS_SYMIQ_MAPPING", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "ATSC/DVB-T/QAM VHF/UHF modulator", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets112_0[] =
{
    { DtProperties112_0_00, 1 },
    { DtProperties112_0_01, 2 },
    { DtProperties112_0_02, 2 },
    { DtProperties112_0_03, 3 },
    { DtProperties112_0_04, 3 },
    { DtProperties112_0_05, 1 },
    { DtProperties112_0_06, 1 },
    { NULL, 0 },
    { DtProperties112_0_08, 10 },
    { DtProperties112_0_09, 2 },
    { DtProperties112_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties112_0_14, 5 },
    { NULL, 0 },
    { DtProperties112_0_16, 1 },
    { NULL, 0 },
    { DtProperties112_0_18, 1 },
    { DtProperties112_0_19, 1 },
    { DtProperties112_0_20, 2 },
    { DtProperties112_0_21, 1 },
    { DtProperties112_0_22, 4 },
    { NULL, 0 },
    { DtProperties112_0_24, 2 },
    { DtProperties112_0_25, 5 },
    { DtProperties112_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties112_0_29, 2 },
    { DtProperties112_0_30, 1 },
    { DtProperties112_0_31, 1 },
    { DtProperties112_0_32, 1 },
    { DtProperties112_0_33, 2 },
    { DtProperties112_0_34, 1 },
    { DtProperties112_0_35, 3 },
    { DtProperties112_0_36, 1 },
    { DtProperties112_0_37, 1 },
    { NULL, 0 },
    { DtProperties112_0_39, 1 },
    { DtProperties112_0_40, 2 },
    { NULL, 0 },
    { DtProperties112_0_42, 3 },
    { DtProperties112_0_43, 4 },
    { DtProperties112_0_44, 1 },
    { DtProperties112_0_45, 2 },
    { DtProperties112_0_46, 3 },
    { DtProperties112_0_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties112_0_50, 1 },
    { DtProperties112_0_51, 2 },
    { DtProperties112_0_52, 1 },
    { DtProperties112_0_53, 3 },
    { NULL, 0 },
    { DtProperties112_0_55, 1 },
    { DtProperties112_0_56, 3 },
    { NULL, 0 },
    { DtProperties112_0_58, 1 },
    { DtProperties112_0_59, 4 },
    { DtProperties112_0_60, 2 },
    { DtProperties112_0_61, 2 },
    { NULL, 0 },
    { DtProperties112_0_63, 3 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-115; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Multi-standard VHF/UHF modulator, ASI in+out
// 
static const DtProperty  DtProperties115_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Fixed stability issues with DVB-C modulation",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_01[] =
{
    { "FIRMWARE_LAST_VER", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1, "Support for DTMB (DTC-374-DTMB license required)",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1,
      "Fixed PCI performance issue when using both ports simultaneously",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 2, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_02[] =
{
    { "CAP_TX_CMMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "FIRMWARE_R_N1_2", -1, "Support for QAM-B", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_1", -1,
      "New QAM channel filters; including 12% and 18% filters for QAM-B",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_RFPLL", 115, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_03[] =
{
    { "CAP_TX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "FIRMWARE_R_N2_2", -1,
      "Improved carrier suppression => better MER / reduced carrier feed-through",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_AD9789", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_04[] =
{
    { "CAP_TX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "FIRMWARE_R_N2_3", -1, "SDRAM buffer is now used for OFDM and ATSC modulation",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 115, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties115_0_06[] =
{
    { "REGISTERS_RX", 0x080, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_08[] =
{
    { "CAP_CM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_T2SPLP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_RANGE", 350, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_LATENCY_TIMER", 96, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x280, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_14[] =
{
    { "CAP_TX_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x3FF, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties115_0_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties115_0_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties115_0_25[] =
{
    { "CAP_TX_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 300, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN2_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN2_PORT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x73, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties115_0_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_33[] =
{
    { "MOD_LEVEL_MAX", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_41[] =
{
    { "CAP_TX_GOLD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties115_0_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_TX_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS16, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 30, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MIN", 1000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 999, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties115_0_52[] =
{
    { "CAP_TX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_53[] =
{
    { "CAP_ADJLVL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_55[] =
{
    { "MOD_FREQ_MAX", 862, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "MOD_HW_HAS_SYMIQ_MAPPING", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Multi-standard VHF/UHF modulator, ASI in+out",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets115_0[] =
{
    { DtProperties115_0_00, 2 },
    { DtProperties115_0_01, 4 },
    { DtProperties115_0_02, 4 },
    { DtProperties115_0_03, 4 },
    { DtProperties115_0_04, 4 },
    { DtProperties115_0_05, 1 },
    { DtProperties115_0_06, 1 },
    { NULL, 0 },
    { DtProperties115_0_08, 10 },
    { DtProperties115_0_09, 2 },
    { DtProperties115_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties115_0_14, 5 },
    { NULL, 0 },
    { DtProperties115_0_16, 1 },
    { NULL, 0 },
    { DtProperties115_0_18, 1 },
    { DtProperties115_0_19, 1 },
    { DtProperties115_0_20, 2 },
    { DtProperties115_0_21, 1 },
    { DtProperties115_0_22, 4 },
    { NULL, 0 },
    { DtProperties115_0_24, 2 },
    { DtProperties115_0_25, 5 },
    { DtProperties115_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties115_0_29, 2 },
    { DtProperties115_0_30, 1 },
    { DtProperties115_0_31, 1 },
    { DtProperties115_0_32, 1 },
    { DtProperties115_0_33, 2 },
    { DtProperties115_0_34, 1 },
    { DtProperties115_0_35, 3 },
    { DtProperties115_0_36, 1 },
    { DtProperties115_0_37, 1 },
    { NULL, 0 },
    { DtProperties115_0_39, 1 },
    { DtProperties115_0_40, 2 },
    { DtProperties115_0_41, 1 },
    { DtProperties115_0_42, 3 },
    { DtProperties115_0_43, 4 },
    { DtProperties115_0_44, 1 },
    { DtProperties115_0_45, 2 },
    { DtProperties115_0_46, 3 },
    { DtProperties115_0_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties115_0_50, 1 },
    { DtProperties115_0_51, 2 },
    { DtProperties115_0_52, 1 },
    { DtProperties115_0_53, 3 },
    { NULL, 0 },
    { DtProperties115_0_55, 1 },
    { DtProperties115_0_56, 3 },
    { NULL, 0 },
    { DtProperties115_0_58, 1 },
    { DtProperties115_0_59, 4 },
    { DtProperties115_0_60, 2 },
    { DtProperties115_0_61, 2 },
    { NULL, 0 },
    { DtProperties115_0_63, 3 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-116; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Multi-standard 36MHz-IF/digital-IQ modulator
// 
static const DtProperty  DtProperties116_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1,
      "Fixed PCI performance issue when using both ports simultaneously",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_01[] =
{
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1,
      "New QAM channel filters; including 12% and 18% filters for QAM-B",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_02[] =
{
    { "CAP_TX_CMMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "FIRMWARE_R_N1_2", -1,
      "Improved carrier suppression => better MER / reduced carrier feed-through",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_RFPLL", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_03[] =
{
    { "CAP_TX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "FIRMWARE_R_N1_3", -1, "SDRAM buffer is now used for OFDM and ATSC modulation",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_AD9789", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_04[] =
{
    { "CAP_TX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 115, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties116_0_06[] =
{
    { "CAP_DIGIQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "REGISTERS_RX", 0x080, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_07[] =
{
    { "CAP_IF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties116_0_08[] =
{
    { "CAP_CM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_T2SPLP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_RANGE", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_LATENCY_TIMER", 96, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x280, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_14[] =
{
    { "CAP_TX_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x3FF, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties116_0_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties116_0_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties116_0_25[] =
{
    { "CAP_TX_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN2_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN2_PORT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x74, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties116_0_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_33[] =
{
    { "MOD_LEVEL_MAX", -110, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_TX_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS16, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MIN", 1000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 36, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties116_0_52[] =
{
    { "CAP_TX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_53[] =
{
    { "MOD_FREQ_MIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_55[] =
{
    { "MOD_FREQ_MAX", 36, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_59[] =
{
    { "MOD_HW_HAS_SYMIQ_MAPPING", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Multi-standard 36MHz-IF/digital-IQ modulator",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_60[] =
{
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets116_0[] =
{
    { DtProperties116_0_00, 2 },
    { DtProperties116_0_01, 3 },
    { DtProperties116_0_02, 3 },
    { DtProperties116_0_03, 4 },
    { DtProperties116_0_04, 3 },
    { DtProperties116_0_05, 1 },
    { DtProperties116_0_06, 2 },
    { DtProperties116_0_07, 1 },
    { DtProperties116_0_08, 10 },
    { DtProperties116_0_09, 2 },
    { DtProperties116_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties116_0_14, 5 },
    { NULL, 0 },
    { DtProperties116_0_16, 1 },
    { NULL, 0 },
    { DtProperties116_0_18, 1 },
    { DtProperties116_0_19, 1 },
    { DtProperties116_0_20, 2 },
    { DtProperties116_0_21, 1 },
    { DtProperties116_0_22, 4 },
    { NULL, 0 },
    { DtProperties116_0_24, 2 },
    { DtProperties116_0_25, 5 },
    { DtProperties116_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties116_0_29, 2 },
    { DtProperties116_0_30, 1 },
    { DtProperties116_0_31, 1 },
    { DtProperties116_0_32, 1 },
    { DtProperties116_0_33, 2 },
    { DtProperties116_0_34, 1 },
    { DtProperties116_0_35, 3 },
    { DtProperties116_0_36, 1 },
    { DtProperties116_0_37, 1 },
    { NULL, 0 },
    { DtProperties116_0_39, 1 },
    { DtProperties116_0_40, 2 },
    { NULL, 0 },
    { DtProperties116_0_42, 3 },
    { DtProperties116_0_43, 4 },
    { DtProperties116_0_44, 1 },
    { DtProperties116_0_45, 2 },
    { DtProperties116_0_46, 3 },
    { DtProperties116_0_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties116_0_50, 1 },
    { DtProperties116_0_51, 2 },
    { DtProperties116_0_52, 1 },
    { DtProperties116_0_53, 1 },
    { NULL, 0 },
    { DtProperties116_0_55, 1 },
    { DtProperties116_0_56, 3 },
    { NULL, 0 },
    { DtProperties116_0_58, 1 },
    { DtProperties116_0_59, 3 },
    { DtProperties116_0_60, 1 },
    { DtProperties116_0_61, 2 },
    { NULL, 0 },
    { DtProperties116_0_63, 3 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-117; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Multi-standard 44MHz-IF/digital-IQ modulator
// 
static const DtProperty  DtProperties117_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_01[] =
{
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_02[] =
{
    { "CAP_TX_CMMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_RFPLL", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_03[] =
{
    { "CAP_TX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_HAS_AD9789", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_04[] =
{
    { "CAP_TX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 117, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties117_0_06[] =
{
    { "CAP_DIGIQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "REGISTERS_RX", 0x080, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_07[] =
{
    { "CAP_IF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties117_0_08[] =
{
    { "CAP_CM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_T2SPLP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_RANGE", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_LATENCY_TIMER", 96, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x280, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_14[] =
{
    { "CAP_TX_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x3FF, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties117_0_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties117_0_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties117_0_25[] =
{
    { "CAP_TX_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN2_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN2_PORT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x75, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties117_0_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_33[] =
{
    { "MOD_LEVEL_MAX", -110, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_TX_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS16, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 44, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MIN", 1000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 44, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties117_0_52[] =
{
    { "CAP_TX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_53[] =
{
    { "MOD_FREQ_MIN", 44, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_55[] =
{
    { "MOD_FREQ_MAX", 44, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_59[] =
{
    { "MOD_HW_HAS_SYMIQ_MAPPING", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Multi-standard 44MHz-IF/digital-IQ modulator",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_60[] =
{
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets117_0[] =
{
    { DtProperties117_0_00, 1 },
    { DtProperties117_0_01, 2 },
    { DtProperties117_0_02, 2 },
    { DtProperties117_0_03, 3 },
    { DtProperties117_0_04, 3 },
    { DtProperties117_0_05, 1 },
    { DtProperties117_0_06, 2 },
    { DtProperties117_0_07, 1 },
    { DtProperties117_0_08, 10 },
    { DtProperties117_0_09, 2 },
    { DtProperties117_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties117_0_14, 5 },
    { NULL, 0 },
    { DtProperties117_0_16, 1 },
    { NULL, 0 },
    { DtProperties117_0_18, 1 },
    { DtProperties117_0_19, 1 },
    { DtProperties117_0_20, 2 },
    { DtProperties117_0_21, 1 },
    { DtProperties117_0_22, 4 },
    { NULL, 0 },
    { DtProperties117_0_24, 2 },
    { DtProperties117_0_25, 5 },
    { DtProperties117_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties117_0_29, 2 },
    { DtProperties117_0_30, 1 },
    { DtProperties117_0_31, 1 },
    { DtProperties117_0_32, 1 },
    { DtProperties117_0_33, 2 },
    { DtProperties117_0_34, 1 },
    { DtProperties117_0_35, 3 },
    { DtProperties117_0_36, 1 },
    { DtProperties117_0_37, 1 },
    { NULL, 0 },
    { DtProperties117_0_39, 1 },
    { DtProperties117_0_40, 2 },
    { NULL, 0 },
    { DtProperties117_0_42, 3 },
    { DtProperties117_0_43, 4 },
    { DtProperties117_0_44, 1 },
    { DtProperties117_0_45, 2 },
    { DtProperties117_0_46, 3 },
    { DtProperties117_0_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties117_0_50, 1 },
    { DtProperties117_0_51, 2 },
    { DtProperties117_0_52, 1 },
    { DtProperties117_0_53, 1 },
    { NULL, 0 },
    { DtProperties117_0_55, 1 },
    { DtProperties117_0_56, 3 },
    { NULL, 0 },
    { DtProperties117_0_58, 1 },
    { DtProperties117_0_59, 3 },
    { DtProperties117_0_60, 1 },
    { DtProperties117_0_61, 2 },
    { NULL, 0 },
    { DtProperties117_0_63, 3 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-120; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DVB-ASI input
// 
static const DtProperty  DtProperties120_0_01[] =
{
    { "FIRMWARE_LAST_VER", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_02[] =
{
    { "FIRMWARE_R_N3_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_03[] =
{
    { "FIRMWARE_R_N4_0", -1, "Add support for receive modes: ST188 ST204 and STRAW",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_CHAN0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_06[] =
{
    { "REGISTERS_RX", 0x20, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_LATENCY_TIMER", 64, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 40500000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 40500000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 40000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 40500000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 40000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_10[] =
{
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x07F, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_22[] =
{
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 300, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN2_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN2_PORT", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0xD114, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x14B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 4, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_OLD, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_56[] =
{
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "DVB-ASI input", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_60[] =
{
    { "PCI_DEVICE_ID", 0x9054, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets120_0[] =
{
    { NULL, 0 },
    { DtProperties120_0_01, 1 },
    { DtProperties120_0_02, 1 },
    { DtProperties120_0_03, 2 },
    { DtProperties120_0_04, 1 },
    { NULL, 0 },
    { DtProperties120_0_06, 2 },
    { NULL, 0 },
    { DtProperties120_0_08, 8 },
    { NULL, 0 },
    { DtProperties120_0_10, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_0_14, 4 },
    { NULL, 0 },
    { DtProperties120_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_0_20, 1 },
    { DtProperties120_0_21, 1 },
    { DtProperties120_0_22, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_0_25, 4 },
    { DtProperties120_0_26, 7 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_0_29, 1 },
    { DtProperties120_0_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_0_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_0_39, 1 },
    { DtProperties120_0_40, 2 },
    { NULL, 0 },
    { DtProperties120_0_42, 2 },
    { DtProperties120_0_43, 2 },
    { DtProperties120_0_44, 1 },
    { NULL, 0 },
    { DtProperties120_0_46, 6 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_0_50, 1 },
    { DtProperties120_0_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_0_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_0_59, 2 },
    { DtProperties120_0_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_0_63, 1 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-122; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DVB-spi input
// 
static const DtProperty  DtProperties122_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "Add LED flashing at power-up", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_01[] =
{
    { "FIRMWARE_LAST_VER", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Add receive-clock detector", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_02[] =
{
    { "FIRMWARE_R_N2_1", -1,
      "Add FIFO lock-up detector; fixes occasional freezing of receive FIFO",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_0", -1, "New; improved FIFO lock-up detector",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_03[] =
{
    { "FIRMWARE_R_N2_2", -1, "Introduce LED patterns similar to other DekTec devices",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_1", -1, "Reduce on-board noise level by improving FPGA equations",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N4_0", -1,
      "Processing of incoming packets made equivalent to DTA-120 and DTA-140",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_CHAN0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_04[] =
{
    { "CAP_SPI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_2", -1,
      "Fix bug that causes data-corruption when the rx-clock is not continuous",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N4_1", -1,
      "Fix packet-alignment errors in receive modes ST188 and ST204",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_06[] =
{
    { "REGISTERS_RX", 0x20, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_LATENCY_TIMER", 64, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 27000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_SPI", 0x00, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x00, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_10[] =
{
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_14[] =
{
    { "CAP_SPILVDS1", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x07F, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x00, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_22[] =
{
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN2_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN2_PORT", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0xD116, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_32[] =
{
    { "DEFAULT_SPISTD", DT_IOCONFIG_SPILVDS1, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x14B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_37[] =
{
    { "CAP_SPIDVBMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_56[] =
{
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "DVB-spi input", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_60[] =
{
    { "PCI_DEVICE_ID", 0x9054, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets122_0[] =
{
    { DtProperties122_0_00, 1 },
    { DtProperties122_0_01, 2 },
    { DtProperties122_0_02, 2 },
    { DtProperties122_0_03, 4 },
    { DtProperties122_0_04, 4 },
    { NULL, 0 },
    { DtProperties122_0_06, 2 },
    { NULL, 0 },
    { DtProperties122_0_08, 7 },
    { NULL, 0 },
    { DtProperties122_0_10, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_0_14, 4 },
    { NULL, 0 },
    { DtProperties122_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_0_20, 1 },
    { NULL, 0 },
    { DtProperties122_0_22, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_0_25, 4 },
    { DtProperties122_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_0_29, 1 },
    { DtProperties122_0_30, 1 },
    { NULL, 0 },
    { DtProperties122_0_32, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_0_35, 2 },
    { NULL, 0 },
    { DtProperties122_0_37, 1 },
    { NULL, 0 },
    { DtProperties122_0_39, 1 },
    { DtProperties122_0_40, 2 },
    { NULL, 0 },
    { DtProperties122_0_42, 1 },
    { DtProperties122_0_43, 2 },
    { DtProperties122_0_44, 1 },
    { NULL, 0 },
    { DtProperties122_0_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_0_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_0_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_0_59, 2 },
    { DtProperties122_0_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_0_63, 2 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-124; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Quad ASI/SDI input
// 
static const DtProperty  DtProperties124_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "Support for reception of 10-bit SDI",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_01[] =
{
    { "FIRMWARE_LAST_VER", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1, "Support for Huffman-coded SDI data",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Support for time-stamping of SDI frames",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_02[] =
{
    { "FIRMWARE_R_N2_1", -1, "Support transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_03[] =
{
    { "FIRMWARE_R_N2_2", -1, "Bug fix: could not read up to last word in HW FIFO",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_04[] =
{
    { "FIRMWARE_R_N2_3", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_06[] =
{
    { "REGISTERS_RX", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x180, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x160, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x240, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x320, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_07[] =
{
    { "CAP_SDITIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_LATENCY_TIMER", 64, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 64, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 2, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 2, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x3FF, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x3FF, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x280, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x2c0, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x340, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_22[] =
{
    { "PLX_DMA_THRESHOLD", 0x21102110, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 1, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00000010, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 100, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN2_MODE", 0x19C3, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 1, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x19C3, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 },
    { "PLX_DMA_CHAN2_PORT", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 1, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0xD118, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 2, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x14B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 1, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_50[] =
{
    { "PORT_COUNT", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_GS9060, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_GS9060, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_GS9060, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_GS9060, NULL, PROPERTY_VALUE_TYPE_INT, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x19C3, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 1, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x19C3, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 },
    { "PLX_DMA_CHAN1_PORT", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 1, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Quad ASI/SDI input", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_60[] =
{
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets124_0[] =
{
    { DtProperties124_0_00, 1 },
    { DtProperties124_0_01, 3 },
    { DtProperties124_0_02, 1 },
    { DtProperties124_0_03, 3 },
    { DtProperties124_0_04, 2 },
    { NULL, 0 },
    { DtProperties124_0_06, 8 },
    { DtProperties124_0_07, 4 },
    { DtProperties124_0_08, 11 },
    { NULL, 0 },
    { DtProperties124_0_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_0_14, 12 },
    { NULL, 0 },
    { DtProperties124_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_0_19, 4 },
    { DtProperties124_0_20, 1 },
    { DtProperties124_0_21, 4 },
    { DtProperties124_0_22, 2 },
    { DtProperties124_0_23, 4 },
    { NULL, 0 },
    { DtProperties124_0_25, 6 },
    { DtProperties124_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_0_29, 5 },
    { DtProperties124_0_30, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_0_35, 6 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_0_39, 1 },
    { DtProperties124_0_40, 2 },
    { NULL, 0 },
    { DtProperties124_0_42, 1 },
    { DtProperties124_0_43, 3 },
    { DtProperties124_0_44, 1 },
    { NULL, 0 },
    { DtProperties124_0_46, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_0_50, 1 },
    { DtProperties124_0_51, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_0_56, 8 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_0_59, 5 },
    { DtProperties124_0_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_0_63, 5 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-140; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DVB-ASI in+out
// 
static const DtProperty  DtProperties140_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_01[] =
{
    { "FIRMWARE_LAST_VER", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Improved load computation of receive FIFO",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_02[] =
{
    { "FIRMWARE_R_N2_1", -1,
      "Add LED flashing pattern when input channel is being reset",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_03[] =
{
    { "PLX_DMA_PRIO", DMA_PRIO_CHAN0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_LATENCY_TIMER", 64, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 40500000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_HOTLINK, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0,
                                                      0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x1FF, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 300, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN2_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN2_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0xD128, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties140_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x14B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_HOTLINK, NULL, PROPERTY_VALUE_TYPE_INT,
                                                0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_56[] =
{
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "DVB-ASI in+out", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_60[] =
{
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets140_0[] =
{
    { DtProperties140_0_00, 2 },
    { DtProperties140_0_01, 2 },
    { DtProperties140_0_02, 1 },
    { DtProperties140_0_03, 1 },
    { DtProperties140_0_04, 1 },
    { NULL, 0 },
    { DtProperties140_0_06, 1 },
    { NULL, 0 },
    { DtProperties140_0_08, 6 },
    { DtProperties140_0_09, 1 },
    { DtProperties140_0_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_0_14, 4 },
    { NULL, 0 },
    { DtProperties140_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_0_20, 1 },
    { DtProperties140_0_21, 2 },
    { DtProperties140_0_22, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_0_25, 4 },
    { DtProperties140_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_0_29, 1 },
    { DtProperties140_0_30, 1 },
    { DtProperties140_0_31, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_0_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_0_39, 1 },
    { DtProperties140_0_40, 2 },
    { NULL, 0 },
    { DtProperties140_0_42, 1 },
    { DtProperties140_0_43, 2 },
    { DtProperties140_0_44, 1 },
    { NULL, 0 },
    { DtProperties140_0_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_0_50, 1 },
    { DtProperties140_0_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_0_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_0_59, 2 },
    { DtProperties140_0_60, 1 },
    { DtProperties140_0_61, 1 },
    { NULL, 0 },
    { DtProperties140_0_63, 2 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-145; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// ASI/SDI in+out with bypass relais
// 
static const DtProperty  DtProperties145_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Support for reception/transmission of 10-bit SDI",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_01[] =
{
    { "FIRMWARE_LAST_VER", 10, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1, "Support for Huffman-coded SDI data",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Support for time-stamping of SDI frames",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_02[] =
{
    { "FIRMWARE_R_N2_1", -1, "Support transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_0", -1,
      "Fixes issue with DVB-ASI streams that have a bitrate >= 108Mbps",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_03[] =
{
    { "FIRMWARE_R_N2_2", -1, "Support for double-buffered mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_1", -1, "Support for SDI genlock", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N4_0", -1, "Intermediate version with some Genlock bugfixes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_04[] =
{
    { "CAP_GENLOCKED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_3", -1, "Bug fix: FIFO-size setting is ignored",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N5_0", -1, "Few minor bug-fixes", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_05[] =
{
    { "FIRMWARE_R_N2_4", -1,
      "Bug fix: Potential stale of dma engine resulting in a system lockup",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_06[] =
{
    { "FIRMWARE_R_N2_5", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_07[] =
{
    { "CAP_SDITIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N9_0", -1, "Support for RAW-ASI mode", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 9, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 9, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x180, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x380, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N9_1", -1, "Support for transmit on timestamp mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "GENLOCK_ASYNC_PORT", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "FIRMWARE_R_N9_2", -1, "Support for ASI bitrate lock", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_11[] =
{
    { "FIRMWARE_R_N9_3", -1,
      "Fixes issue; where certain PCs would detect the card as a graphics card",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_12[] =
{
    { "FIRMWARE_R_N9_4", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_15[] =
{
    { "CAP_GENREF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_16[] =
{
    { "CAP_TXONTIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N10_0", -1, "Fixes issue; Possible fail to sync to the input stream",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_17[] =
{
    { "FIRMWARE_R_N10_1", -1, "Several other minor bug fixes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_19[] =
{
    { "CAP_FAILSAFE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 100, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_31[] =
{
    { "CAP_RAWASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_32[] =
{
    { "CAP_LOOPTHR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_37[] =
{
    { "GENLOCK_ARCH", 145, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_39[] =
{
    { "CAP_DBLBUF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 9, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 9, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_58[] =
{
    { "CAP_LOCK2INP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "ASI/SDI in+out with bypass relais", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_60[] =
{
    { "PCI_DEVICE_ID", 0x91, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets145_0[] =
{
    { DtProperties145_0_00, 3 },
    { DtProperties145_0_01, 3 },
    { DtProperties145_0_02, 2 },
    { DtProperties145_0_03, 3 },
    { DtProperties145_0_04, 4 },
    { DtProperties145_0_05, 1 },
    { DtProperties145_0_06, 2 },
    { DtProperties145_0_07, 1 },
    { DtProperties145_0_08, 9 },
    { DtProperties145_0_09, 4 },
    { DtProperties145_0_10, 4 },
    { DtProperties145_0_11, 1 },
    { DtProperties145_0_12, 1 },
    { NULL, 0 },
    { DtProperties145_0_14, 3 },
    { DtProperties145_0_15, 1 },
    { DtProperties145_0_16, 4 },
    { DtProperties145_0_17, 1 },
    { NULL, 0 },
    { DtProperties145_0_19, 2 },
    { DtProperties145_0_20, 1 },
    { DtProperties145_0_21, 2 },
    { DtProperties145_0_22, 2 },
    { DtProperties145_0_23, 2 },
    { NULL, 0 },
    { DtProperties145_0_25, 2 },
    { DtProperties145_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties145_0_29, 3 },
    { DtProperties145_0_30, 1 },
    { DtProperties145_0_31, 4 },
    { DtProperties145_0_32, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties145_0_35, 4 },
    { NULL, 0 },
    { DtProperties145_0_37, 1 },
    { NULL, 0 },
    { DtProperties145_0_39, 2 },
    { DtProperties145_0_40, 2 },
    { NULL, 0 },
    { DtProperties145_0_42, 1 },
    { DtProperties145_0_43, 1 },
    { DtProperties145_0_44, 1 },
    { NULL, 0 },
    { DtProperties145_0_46, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties145_0_50, 1 },
    { DtProperties145_0_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties145_0_56, 2 },
    { NULL, 0 },
    { DtProperties145_0_58, 2 },
    { DtProperties145_0_59, 3 },
    { DtProperties145_0_60, 1 },
    { DtProperties145_0_61, 4 },
    { NULL, 0 },
    { DtProperties145_0_63, 3 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-160; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// GigE + 3x ASI ports
// 
static const DtProperty  DtProperties160_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Improved data throughput performance on ASI channels",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NETWORK_MAC_TYPE", MAC_TYPE_CAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_01[] =
{
    { "FIRMWARE_LAST_VER", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1,
      "New LED patterns to clearly indentify in- (=green) and outputs (=orange)",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Support transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_02[] =
{
    { "FIRMWARE_R_N1_2", -1, "Bug fix: carrier-extension was seen as an error",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_1", -1, "Improved configurable IO support",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_RT_TX_DMA", 0xC80, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_03[] =
{
    { "FIRMWARE_R_N1_3", -1,
      "Bug fix: occasional loss of IP packets when sending packets in a burst",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_2", -1,
      "Bug fix: occasional data-loss/duplication (for IP rx) in 66MHz PCI slots",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_04[] =
{
    { "FIRMWARE_R_N2_3", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x500, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_07[] =
{
    { "CAP_SDITIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_LATENCY_TIMER", 96, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x180, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x380, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x580, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "NW_REGISTERS_RX", 0xB00, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0xFFF, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x600, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_16[] =
{
    { "NW_REGISTERS_NRT_TX_DMA", 0xD80, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_17[] =
{
    { "CAP_IP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_MAC", 0x900, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "NW_REGISTERS_RT_TX", 0xC00, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x21102110, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 20, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 400, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN2_MODE", 0x19C3, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN2_PORT", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_26[] =
{
    { "NW_REGISTERS_GEN", 0xA00, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_SYSTEM_ID", 0xA0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 64, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_NRT_TX", 0xD00, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_32[] =
{
    { "CAP_LOOPTHR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_39[] =
{
    { "CAP_DBLBUF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_MAC_23_14", 0x000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS16, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_50[] =
{
    { "PORT_COUNT", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x19C3, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_59[] =
{
    { "NW_REGISTERS_RX_DMA", 0xB80, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "GigE + 3x ASI ports", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_60[] =
{
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets160_0[] =
{
    { DtProperties160_0_00, 5 },
    { DtProperties160_0_01, 3 },
    { DtProperties160_0_02, 3 },
    { DtProperties160_0_03, 3 },
    { DtProperties160_0_04, 2 },
    { NULL, 0 },
    { DtProperties160_0_06, 3 },
    { DtProperties160_0_07, 3 },
    { DtProperties160_0_08, 12 },
    { DtProperties160_0_09, 3 },
    { DtProperties160_0_10, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties160_0_14, 5 },
    { NULL, 0 },
    { DtProperties160_0_16, 2 },
    { DtProperties160_0_17, 3 },
    { NULL, 0 },
    { DtProperties160_0_19, 4 },
    { DtProperties160_0_20, 1 },
    { DtProperties160_0_21, 3 },
    { DtProperties160_0_22, 5 },
    { DtProperties160_0_23, 3 },
    { NULL, 0 },
    { DtProperties160_0_25, 4 },
    { DtProperties160_0_26, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties160_0_29, 4 },
    { DtProperties160_0_30, 1 },
    { DtProperties160_0_31, 5 },
    { DtProperties160_0_32, 6 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties160_0_35, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties160_0_39, 5 },
    { DtProperties160_0_40, 2 },
    { NULL, 0 },
    { DtProperties160_0_42, 1 },
    { DtProperties160_0_43, 2 },
    { DtProperties160_0_44, 1 },
    { NULL, 0 },
    { DtProperties160_0_46, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties160_0_50, 1 },
    { DtProperties160_0_51, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties160_0_56, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties160_0_59, 8 },
    { DtProperties160_0_60, 1 },
    { DtProperties160_0_61, 6 },
    { NULL, 0 },
    { DtProperties160_0_63, 5 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2107; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Multi-standard L-band modulator
// 
static const DtProperty  DtProperties2107_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1,
      "Bug fix:  incorrect frequency of the on-board reference clock",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_01[] =
{
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1, "Bug fix:  no null packets are sent on stop",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 1, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_02[] =
{
    { "MOD_HW_RFPLL", 2107, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_03[] =
{
    { "MOD_HW_HAS_AD9789", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_04[] =
{
    { "CAP_TX_DVBS2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 2107, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_08[] =
{
    { "CAP_CM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "HARDWARE_SUPPORTS_TXNULL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_RANGE", 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x180, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_11[] =
{
    { "CAP_SNR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties2107_0_12[] =
{
    { "CAP_ROLLOFF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2107_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "CAP_TX_DVBS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2107_0_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2107_0_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_ISDBS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_25[] =
{
    { "CAP_TX_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 210, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", 200000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_33[] =
{
    { "MOD_LEVEL_MAX", -100, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 88000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 88000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 55000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 50000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 700, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MIN", 88000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_44[] =
{
    { "CAP_S2APSK", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 2300, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 50000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_47[] =
{
    { "MOD_HW_SAMP_PACKING", 12, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_51[] =
{
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties2107_0_53[] =
{
    { "CAP_ADJLVL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 950, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_55[] =
{
    { "MOD_FREQ_MAX", 2150, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 88000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_57[] =
{
    { "CAP_LBAND", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2107_0_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 55000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_59[] =
{
    { "CAP_RFCLKEXT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SYMIQ_MAPPING", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Multi-standard L-band modulator", PROPERTY_VALUE_TYPE_STRING, -1, 0,
                                                       0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_60[] =
{
    { "PCI_DEVICE_ID", 0x83B, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2107_0[] =
{
    { DtProperties2107_0_00, 1 },
    { DtProperties2107_0_01, 3 },
    { DtProperties2107_0_02, 1 },
    { DtProperties2107_0_03, 1 },
    { DtProperties2107_0_04, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2107_0_08, 7 },
    { DtProperties2107_0_09, 1 },
    { DtProperties2107_0_10, 2 },
    { DtProperties2107_0_11, 1 },
    { DtProperties2107_0_12, 1 },
    { NULL, 0 },
    { DtProperties2107_0_14, 2 },
    { NULL, 0 },
    { DtProperties2107_0_16, 1 },
    { NULL, 0 },
    { DtProperties2107_0_18, 2 },
    { DtProperties2107_0_19, 1 },
    { DtProperties2107_0_20, 2 },
    { NULL, 0 },
    { DtProperties2107_0_22, 2 },
    { NULL, 0 },
    { DtProperties2107_0_24, 2 },
    { DtProperties2107_0_25, 3 },
    { DtProperties2107_0_26, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2107_0_29, 2 },
    { DtProperties2107_0_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2107_0_33, 2 },
    { DtProperties2107_0_34, 1 },
    { DtProperties2107_0_35, 3 },
    { DtProperties2107_0_36, 1 },
    { DtProperties2107_0_37, 1 },
    { NULL, 0 },
    { DtProperties2107_0_39, 1 },
    { DtProperties2107_0_40, 2 },
    { NULL, 0 },
    { DtProperties2107_0_42, 2 },
    { DtProperties2107_0_43, 3 },
    { DtProperties2107_0_44, 2 },
    { DtProperties2107_0_45, 2 },
    { DtProperties2107_0_46, 4 },
    { DtProperties2107_0_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2107_0_50, 1 },
    { DtProperties2107_0_51, 1 },
    { NULL, 0 },
    { DtProperties2107_0_53, 3 },
    { NULL, 0 },
    { DtProperties2107_0_55, 1 },
    { DtProperties2107_0_56, 1 },
    { DtProperties2107_0_57, 1 },
    { DtProperties2107_0_58, 1 },
    { DtProperties2107_0_59, 3 },
    { DtProperties2107_0_60, 1 },
    { DtProperties2107_0_61, 1 },
    { NULL, 0 },
    { DtProperties2107_0_63, 2 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2111; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Multi-standard VHF/UHF modulator
// 
static const DtProperty  DtProperties2111_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "Fixes spontaneous lockup issue",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_01[] =
{
    { "FIRMWARE_LAST_VER", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Support for 16MHz bandwidth", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_02[] =
{
    { "CAP_TX_CMMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_RFPLL", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_HW_MIN_TX_16MHZ", 4687500, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_03[] =
{
    { "CAP_TX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_HAS_AD9789", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_04[] =
{
    { "CAP_TX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 21112, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 21111, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_HW_MAX_TX_16MHZ", 18690000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2111_0_08[] =
{
    { "CAP_CM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "CAP_TX_T2SPLP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_RANGE", 230, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x180, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_14[] =
{
    { "CAP_TX_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties2111_0_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_TX_ISDBTMM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_SAMPRATE_HW_ABSMIN_TX_16MHZ", 4687500, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_25[] =
{
    { "CAP_TX_16MHZ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "CAP_TX_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_26[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX_TX_16MHZ", 18690000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0,
                               0, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", 200000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_33[] =
{
    { "MOD_LEVEL_MAX", -90, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN_TX_16MHZ", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 4687500, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 10937499, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX_TX_16MHZ", 21874998, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0,
                               0, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 9375000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_41[] =
{
    { "CAP_TX_GOLD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2111_0_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_TX_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 20, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MIN", 1000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MIN_TX_16MHZ", 1000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 1180, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 10937499, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX_TX_16MHZ", 21874998, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_51[] =
{
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties2111_0_52[] =
{
    { "CAP_TX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_53[] =
{
    { "CAP_ADJLVL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_55[] =
{
    { "MOD_FREQ_MAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 4687500, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 9375000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "MOD_HW_HAS_SYMIQ_MAPPING", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Multi-standard VHF/UHF modulator", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "PCI_DEVICE_ID", 0x83F, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2111_0[] =
{
    { DtProperties2111_0_00, 1 },
    { DtProperties2111_0_01, 3 },
    { DtProperties2111_0_02, 4 },
    { DtProperties2111_0_03, 3 },
    { DtProperties2111_0_04, 5 },
    { DtProperties2111_0_05, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2111_0_08, 6 },
    { DtProperties2111_0_09, 1 },
    { DtProperties2111_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2111_0_14, 3 },
    { NULL, 0 },
    { DtProperties2111_0_16, 1 },
    { NULL, 0 },
    { DtProperties2111_0_18, 1 },
    { DtProperties2111_0_19, 2 },
    { DtProperties2111_0_20, 2 },
    { NULL, 0 },
    { DtProperties2111_0_22, 3 },
    { NULL, 0 },
    { DtProperties2111_0_24, 3 },
    { DtProperties2111_0_25, 4 },
    { DtProperties2111_0_26, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2111_0_29, 2 },
    { DtProperties2111_0_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2111_0_33, 3 },
    { DtProperties2111_0_34, 1 },
    { DtProperties2111_0_35, 4 },
    { DtProperties2111_0_36, 1 },
    { DtProperties2111_0_37, 1 },
    { NULL, 0 },
    { DtProperties2111_0_39, 1 },
    { DtProperties2111_0_40, 2 },
    { DtProperties2111_0_41, 1 },
    { DtProperties2111_0_42, 3 },
    { DtProperties2111_0_43, 4 },
    { DtProperties2111_0_44, 1 },
    { DtProperties2111_0_45, 3 },
    { DtProperties2111_0_46, 2 },
    { DtProperties2111_0_47, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2111_0_50, 1 },
    { DtProperties2111_0_51, 1 },
    { DtProperties2111_0_52, 1 },
    { DtProperties2111_0_53, 3 },
    { NULL, 0 },
    { DtProperties2111_0_55, 1 },
    { DtProperties2111_0_56, 1 },
    { NULL, 0 },
    { DtProperties2111_0_58, 1 },
    { DtProperties2111_0_59, 4 },
    { DtProperties2111_0_60, 2 },
    { DtProperties2111_0_61, 1 },
    { NULL, 0 },
    { DtProperties2111_0_63, 2 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2131; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Multi-standard VHF/UHF receiver
// 
static const DtProperty  DtProperties2131_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_SRC", 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_11[] =
{
    { "REGISTERS_RPM", 0x600, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_17[] =
{
    { "CAP_RX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2131_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 1 } 
};
static const DtProperty  DtProperties2131_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_23[] =
{
    { "CAP_RX_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_IQD", 0x500, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_27[] =
{
    { "CAP_RX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2131_0_28[] =
{
    { "CAP_RX_ADV", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2131_0_29[] =
{
    { "CAP_RX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", 100000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_35[] =
{
    { "CAP_RX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_36[] =
{
    { "REGISTERS_ADC", 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_39[] =
{
    { "CAP_RX_GOLD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "EEPROM_SIZE", 2048, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_40[] =
{
    { "CAP_RX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 5,
                                                                                   2, 2 },
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 42, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 870, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_50[] =
{
    { "CAP_RX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_53[] =
{
    { "MOD_FREQ_MIN", 42, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_55[] =
{
    { "MOD_FREQ_MAX", 870, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_57[] =
{
    { "DEMOD_HW_HAS_I2C_RFPWR_MEAS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "REGISTERS_RXLED", 0x160, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Multi-standard VHF/UHF receiver", PROPERTY_VALUE_TYPE_STRING, -1, 0,
                                                       0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "PCI_DEVICE_ID", 0x853, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2131_0[] =
{
    { DtProperties2131_0_00, 1 },
    { DtProperties2131_0_01, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_0_04, 2 },
    { NULL, 0 },
    { DtProperties2131_0_06, 1 },
    { NULL, 0 },
    { DtProperties2131_0_08, 3 },
    { NULL, 0 },
    { DtProperties2131_0_10, 1 },
    { DtProperties2131_0_11, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_0_14, 2 },
    { NULL, 0 },
    { DtProperties2131_0_16, 1 },
    { DtProperties2131_0_17, 1 },
    { NULL, 0 },
    { DtProperties2131_0_19, 1 },
    { DtProperties2131_0_20, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_0_23, 1 },
    { NULL, 0 },
    { DtProperties2131_0_25, 2 },
    { DtProperties2131_0_26, 4 },
    { DtProperties2131_0_27, 1 },
    { DtProperties2131_0_28, 1 },
    { DtProperties2131_0_29, 2 },
    { DtProperties2131_0_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_0_35, 3 },
    { DtProperties2131_0_36, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_0_39, 2 },
    { DtProperties2131_0_40, 3 },
    { NULL, 0 },
    { DtProperties2131_0_42, 1 },
    { DtProperties2131_0_43, 2 },
    { DtProperties2131_0_44, 1 },
    { DtProperties2131_0_45, 1 },
    { DtProperties2131_0_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_0_50, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_0_53, 1 },
    { NULL, 0 },
    { DtProperties2131_0_55, 1 },
    { NULL, 0 },
    { DtProperties2131_0_57, 1 },
    { NULL, 0 },
    { DtProperties2131_0_59, 4 },
    { DtProperties2131_0_60, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_0_63, 1 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2135; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Dual DVB-T receiver
// 
static const DtProperty  DtProperties2135_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "Support transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 3, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1,
      "Fixes issue with receiving streams that have a bitrate >= ~23Mbps",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_02[] =
{
    { "FIRMWARE_R_N3_0", -1, "Support for 14-bit ADC on Rev 6 hardware",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x500, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x600, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_17[] =
{
    { "CAP_RX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 601, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_28[] =
{
    { "CAP_RX_ADV", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2135_0_29[] =
{
    { "CAP_RX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", 100000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_35[] =
{
    { "CAP_RX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_39[] =
{
    { "CAP_RX_GOLD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_42[] =
{
    { "DMA_LOCAL_ADDR_FIXED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 50, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 50, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 50, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 860, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 860, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_47[] =
{
    { "CAP_IFADC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_50[] =
{
    { "CAP_RX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "CAP_SHAREDANT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PORT_COUNT", 3, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_52[] =
{
    { "CAP_ANTPWR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_53[] =
{
    { "MOD_FREQ_MIN", 50, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 50, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 50, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_55[] =
{
    { "MOD_FREQ_MAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 860, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 860, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_57[] =
{
    { "DEMOD_HW_HAS_I2C_RFPWR_MEAS", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Dual DVB-T receiver", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_DEVICE_ID", 0x857, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2135_0[] =
{
    { DtProperties2135_0_00, 1 },
    { DtProperties2135_0_01, 4 },
    { DtProperties2135_0_02, 1 },
    { NULL, 0 },
    { DtProperties2135_0_04, 1 },
    { NULL, 0 },
    { DtProperties2135_0_06, 3 },
    { NULL, 0 },
    { DtProperties2135_0_08, 7 },
    { NULL, 0 },
    { DtProperties2135_0_10, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2135_0_14, 4 },
    { NULL, 0 },
    { DtProperties2135_0_16, 1 },
    { DtProperties2135_0_17, 2 },
    { NULL, 0 },
    { DtProperties2135_0_19, 2 },
    { DtProperties2135_0_20, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2135_0_25, 2 },
    { DtProperties2135_0_26, 3 },
    { NULL, 0 },
    { DtProperties2135_0_28, 1 },
    { DtProperties2135_0_29, 2 },
    { DtProperties2135_0_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2135_0_35, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2135_0_39, 2 },
    { DtProperties2135_0_40, 2 },
    { NULL, 0 },
    { DtProperties2135_0_42, 4 },
    { DtProperties2135_0_43, 4 },
    { DtProperties2135_0_44, 1 },
    { DtProperties2135_0_45, 3 },
    { DtProperties2135_0_46, 4 },
    { DtProperties2135_0_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2135_0_50, 3 },
    { NULL, 0 },
    { DtProperties2135_0_52, 3 },
    { DtProperties2135_0_53, 3 },
    { NULL, 0 },
    { DtProperties2135_0_55, 3 },
    { NULL, 0 },
    { DtProperties2135_0_57, 3 },
    { NULL, 0 },
    { DtProperties2135_0_59, 7 },
    { DtProperties2135_0_60, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2135_0_63, 4 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2136; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Dual QAM-A/B/C receiver
// 
static const DtProperty  DtProperties2136_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Bug fix: DMA controller issue for fast systems with > 4GB",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1,
      "Bug fix: Transparant mode synchronisation not possible when idle",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_02[] =
{
    { "CAP_RX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_03[] =
{
    { "CAP_RX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 2,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x180, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x280, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x380, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x480, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_29[] =
{
    { "HARDWARE_I2C_FREQ", 200000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_32[] =
{
    { "CAP_LOOPTHR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_IODIR", DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 54, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_50[] =
{
    { "CAP_SHAREDANT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PORT_COUNT", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_53[] =
{
    { "MOD_FREQ_MIN", 54, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_55[] =
{
    { "MOD_FREQ_MAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_57[] =
{
    { "DEMOD_HW_HAS_I2C_RFPWR_MEAS", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Dual QAM-A/B/C receiver", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_DEVICE_ID", 0x858, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2136_0[] =
{
    { DtProperties2136_0_00, 3 },
    { DtProperties2136_0_01, 6 },
    { DtProperties2136_0_02, 2 },
    { DtProperties2136_0_03, 2 },
    { DtProperties2136_0_04, 1 },
    { NULL, 0 },
    { DtProperties2136_0_06, 2 },
    { NULL, 0 },
    { DtProperties2136_0_08, 9 },
    { DtProperties2136_0_09, 2 },
    { DtProperties2136_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_0_14, 5 },
    { NULL, 0 },
    { DtProperties2136_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_0_19, 2 },
    { DtProperties2136_0_20, 1 },
    { DtProperties2136_0_21, 2 },
    { DtProperties2136_0_22, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_0_25, 2 },
    { DtProperties2136_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_0_29, 1 },
    { DtProperties2136_0_30, 1 },
    { DtProperties2136_0_31, 2 },
    { DtProperties2136_0_32, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_0_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_0_39, 1 },
    { DtProperties2136_0_40, 2 },
    { NULL, 0 },
    { DtProperties2136_0_42, 1 },
    { DtProperties2136_0_43, 3 },
    { DtProperties2136_0_44, 1 },
    { DtProperties2136_0_45, 2 },
    { DtProperties2136_0_46, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_0_50, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_0_53, 2 },
    { NULL, 0 },
    { DtProperties2136_0_55, 2 },
    { NULL, 0 },
    { DtProperties2136_0_57, 2 },
    { NULL, 0 },
    { DtProperties2136_0_59, 5 },
    { DtProperties2136_0_60, 3 },
    { DtProperties2136_0_61, 4 },
    { NULL, 0 },
    { DtProperties2136_0_63, 5 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2137; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Dual DVB-S/DVB-S2 receiver
// 
static const DtProperty  DtProperties2137_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Support for transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 8, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Support for STL3 and STL3TS receive modes for DVB-S2",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_02[] =
{
    { "CAP_RX_DVBS2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_1", -1, "Support for L3 loopthrough to an ASI port",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_0", -1, "Support for Generic Streams", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBTYPE1", 0x12E0003, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_03[] =
{
    { "FIRMWARE_R_N2_2", -1, "Support for filtering a specific TS to an ASI port",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N4_0", -1,
      "Fixes issue with extracting TS-packets from DVB-S2 baseband frames",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_04[] =
{
    { "FIRMWARE_R_N5_0", -1, "Bug fix: timing issue on demodulator input ports",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_05[] =
{
    { "FIRMWARE_R_N6_0", -1, "Support for latest DVB-S2 demodulator chip",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_06[] =
{
    { "FIRMWARE_R_N7_0", -1,
      "FrameId field has been removed from L.3 header in STL3 receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_07[] =
{
    { "FIRMWARE_R_N8_0", -1,
      "Bug fix: PLFrameId field from L.3 header counted incorrectly",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N8_1", -1, "Bug fix: DMA controller issue for fast systems with > 4GB",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N8_2", -1,
      "Bug fix: FIFO overflow flag was not set for some receive modes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 2,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "FIRMWARE_R_N8_3", -1,
      "Bug fix: Transparant mode synchronisation not possible when idle",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_11[] =
{
    { "FIRMWARE_R_N8_4", -1,
      "Bug fix: Certain PCs would detect the card as a graphics card",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_12[] =
{
    { "CAP_ROLLOFF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x180, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x280, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x380, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x480, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_16[] =
{
    { "CAP_DISABLED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RX_DVBS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_LOOPS2L3", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 302, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_28[] =
{
    { "CAP_L3MODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 7, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 7, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", 200000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_32[] =
{
    { "CAP_LOOPTHR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_IODIR", DT_IOCONFIG_LOOPTHR, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_LOOPTHR, NULL, PROPERTY_VALUE_TYPE_INT, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 950, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 950, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_44[] =
{
    { "CAP_S2APSK", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 2150, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 2150, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_50[] =
{
    { "CAP_SHAREDANT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PORT_COUNT", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_52[] =
{
    { "CAP_LNB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_53[] =
{
    { "MOD_FREQ_MIN", 950, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 950, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_54[] =
{
    { "CAP_SWS2APSK", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_55[] =
{
    { "MOD_FREQ_MAX", 2150, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 2150, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_57[] =
{
    { "CAP_LBAND", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEMOD_HW_HAS_I2C_RFPWR_MEAS", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_59[] =
{
    { "DEFAULT_PARXTRA0", 0, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_INT, 3, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Dual DVB-S/DVB-S2 receiver", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_60[] =
{
    { "PCI_DEVICE_ID", 0x859, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_62[] =
{
    { "CAP_LOOPS2TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2137_0[] =
{
    { DtProperties2137_0_00, 3 },
    { DtProperties2137_0_01, 4 },
    { DtProperties2137_0_02, 5 },
    { DtProperties2137_0_03, 2 },
    { DtProperties2137_0_04, 2 },
    { DtProperties2137_0_05, 1 },
    { DtProperties2137_0_06, 3 },
    { DtProperties2137_0_07, 1 },
    { DtProperties2137_0_08, 10 },
    { DtProperties2137_0_09, 3 },
    { DtProperties2137_0_10, 4 },
    { DtProperties2137_0_11, 1 },
    { DtProperties2137_0_12, 2 },
    { NULL, 0 },
    { DtProperties2137_0_14, 5 },
    { NULL, 0 },
    { DtProperties2137_0_16, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2137_0_19, 2 },
    { DtProperties2137_0_20, 1 },
    { DtProperties2137_0_21, 2 },
    { DtProperties2137_0_22, 4 },
    { DtProperties2137_0_23, 2 },
    { NULL, 0 },
    { DtProperties2137_0_25, 2 },
    { DtProperties2137_0_26, 3 },
    { NULL, 0 },
    { DtProperties2137_0_28, 2 },
    { DtProperties2137_0_29, 3 },
    { DtProperties2137_0_30, 1 },
    { DtProperties2137_0_31, 2 },
    { DtProperties2137_0_32, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2137_0_35, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2137_0_39, 1 },
    { DtProperties2137_0_40, 2 },
    { NULL, 0 },
    { DtProperties2137_0_42, 1 },
    { DtProperties2137_0_43, 3 },
    { DtProperties2137_0_44, 2 },
    { DtProperties2137_0_45, 2 },
    { DtProperties2137_0_46, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2137_0_50, 2 },
    { NULL, 0 },
    { DtProperties2137_0_52, 2 },
    { DtProperties2137_0_53, 2 },
    { DtProperties2137_0_54, 1 },
    { DtProperties2137_0_55, 2 },
    { DtProperties2137_0_56, 2 },
    { DtProperties2137_0_57, 4 },
    { NULL, 0 },
    { DtProperties2137_0_59, 5 },
    { DtProperties2137_0_60, 1 },
    { DtProperties2137_0_61, 2 },
    { DtProperties2137_0_62, 2 },
    { DtProperties2137_0_63, 5 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2138; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// DVB C2/T2 receiver
// 
static const DtProperty  DtProperties2138_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "Support for transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "FIRMWARE_LAST_VER", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1,
      "Bug fix: Transparant mode synchronisation not possible when idle",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_02[] =
{
    { "FIRMWARE_R_N2_1", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_17[] =
{
    { "CAP_RX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2138_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 101, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_29[] =
{
    { "HARDWARE_I2C_FREQ", 400000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_35[] =
{
    { "CAP_RX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_39[] =
{
    { "EEPROM_SIZE", 2048, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 46, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 876, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_50[] =
{
    { "CAP_RX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_53[] =
{
    { "MOD_FREQ_MIN", 46, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_55[] =
{
    { "MOD_FREQ_MAX", 876, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_57[] =
{
    { "DEMOD_HW_HAS_I2C_RFPWR_MEAS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "DVB C2/T2 receiver", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "PCI_DEVICE_ID", 0x85A, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2138_0[] =
{
    { DtProperties2138_0_00, 1 },
    { DtProperties2138_0_01, 4 },
    { DtProperties2138_0_02, 1 },
    { NULL, 0 },
    { DtProperties2138_0_04, 1 },
    { NULL, 0 },
    { DtProperties2138_0_06, 1 },
    { NULL, 0 },
    { DtProperties2138_0_08, 3 },
    { NULL, 0 },
    { DtProperties2138_0_10, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_0_14, 2 },
    { NULL, 0 },
    { DtProperties2138_0_16, 1 },
    { DtProperties2138_0_17, 1 },
    { NULL, 0 },
    { DtProperties2138_0_19, 1 },
    { DtProperties2138_0_20, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_0_25, 2 },
    { DtProperties2138_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_0_29, 1 },
    { DtProperties2138_0_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_0_35, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_0_39, 1 },
    { DtProperties2138_0_40, 2 },
    { NULL, 0 },
    { DtProperties2138_0_42, 1 },
    { DtProperties2138_0_43, 2 },
    { DtProperties2138_0_44, 1 },
    { DtProperties2138_0_45, 1 },
    { DtProperties2138_0_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_0_50, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_0_53, 1 },
    { NULL, 0 },
    { DtProperties2138_0_55, 1 },
    { NULL, 0 },
    { DtProperties2138_0_57, 1 },
    { NULL, 0 },
    { DtProperties2138_0_59, 3 },
    { DtProperties2138_0_60, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_0_63, 2 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2139; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Twelve-channel QAM Receiver
// 
static const DtProperty  DtProperties2139_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "Support for transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1,
      "Bug fix: Possible hang caused by DMA timeout not handled correctly",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_02[] =
{
    { "CAP_RX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_1", -1,
      "Bug fix: Did not synchronise in rx-control IDLE for Transparent mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_03[] =
{
    { "CAP_RX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_2", -1,
      "Bug fix: FIFO overflow flag was not set for some receive modes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_06[] =
{
    { "REGISTERS_RX", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x600, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x800, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xA00, NULL, PROPERTY_VALUE_TYPE_UINT16, 4, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xC00, NULL, PROPERTY_VALUE_TYPE_UINT16, 5, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xE00, NULL, PROPERTY_VALUE_TYPE_UINT16, 6, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1000, NULL, PROPERTY_VALUE_TYPE_UINT16, 7, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1200, NULL, PROPERTY_VALUE_TYPE_UINT16, 8, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1400, NULL, PROPERTY_VALUE_TYPE_UINT16, 9, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1600, NULL, PROPERTY_VALUE_TYPE_UINT16, 10, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1800, NULL, PROPERTY_VALUE_TYPE_UINT16, 11, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x280, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x480, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x680, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x880, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xA80, NULL, PROPERTY_VALUE_TYPE_UINT16, 4, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xC80, NULL, PROPERTY_VALUE_TYPE_UINT16, 5, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xE80, NULL, PROPERTY_VALUE_TYPE_UINT16, 6, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1080, NULL, PROPERTY_VALUE_TYPE_UINT16, 7, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1280, NULL, PROPERTY_VALUE_TYPE_UINT16, 8, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1480, NULL, PROPERTY_VALUE_TYPE_UINT16, 9, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1680, NULL, PROPERTY_VALUE_TYPE_UINT16, 10, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1880, NULL, PROPERTY_VALUE_TYPE_UINT16, 11, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_I2C", 0x350, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x550, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x750, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x950, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xB50, NULL, PROPERTY_VALUE_TYPE_UINT16, 4, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xD50, NULL, PROPERTY_VALUE_TYPE_UINT16, 5, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xF50, NULL, PROPERTY_VALUE_TYPE_UINT16, 6, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1150, NULL, PROPERTY_VALUE_TYPE_UINT16, 7, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1350, NULL, PROPERTY_VALUE_TYPE_UINT16, 8, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1550, NULL, PROPERTY_VALUE_TYPE_UINT16, 9, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1750, NULL, PROPERTY_VALUE_TYPE_UINT16, 10, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1950, NULL, PROPERTY_VALUE_TYPE_UINT16, 11, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_29[] =
{
    { "HARDWARE_I2C_FREQ", 200000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_39[] =
{
    { "EEPROM_SIZE", 2048, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 54, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 4, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 5, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 6, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 7, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 8, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 9, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 10, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 11, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 4, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 5, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 6, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 7, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 8, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 9, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 10, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 11, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_50[] =
{
    { "PORT_COUNT", 12, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_53[] =
{
    { "MOD_FREQ_MIN", 54, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 4, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 5, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 6, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 7, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 8, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 9, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 10, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 11, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_55[] =
{
    { "MOD_FREQ_MAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 4, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 5, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 6, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 7, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 8, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 9, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 10, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 11, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_57[] =
{
    { "DEMOD_HW_HAS_I2C_RFPWR_MEAS", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RXLED", 0x304, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x504, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x704, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x904, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xB04, NULL, PROPERTY_VALUE_TYPE_UINT16, 4, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xD04, NULL, PROPERTY_VALUE_TYPE_UINT16, 5, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xF04, NULL, PROPERTY_VALUE_TYPE_UINT16, 6, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1104, NULL, PROPERTY_VALUE_TYPE_UINT16, 7, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1304, NULL, PROPERTY_VALUE_TYPE_UINT16, 8, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1504, NULL, PROPERTY_VALUE_TYPE_UINT16, 9, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1704, NULL, PROPERTY_VALUE_TYPE_UINT16, 10, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1904, NULL, PROPERTY_VALUE_TYPE_UINT16, 11, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Twelve-channel QAM Receiver", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_DEVICE_ID", 0x85B, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2139_0[] =
{
    { DtProperties2139_0_00, 1 },
    { DtProperties2139_0_01, 26 },
    { DtProperties2139_0_02, 13 },
    { DtProperties2139_0_03, 13 },
    { DtProperties2139_0_04, 1 },
    { NULL, 0 },
    { DtProperties2139_0_06, 12 },
    { NULL, 0 },
    { DtProperties2139_0_08, 25 },
    { NULL, 0 },
    { DtProperties2139_0_10, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_0_14, 13 },
    { NULL, 0 },
    { DtProperties2139_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_0_19, 12 },
    { DtProperties2139_0_20, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_0_25, 2 },
    { DtProperties2139_0_26, 15 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_0_29, 1 },
    { DtProperties2139_0_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_0_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_0_39, 1 },
    { DtProperties2139_0_40, 2 },
    { NULL, 0 },
    { DtProperties2139_0_42, 1 },
    { DtProperties2139_0_43, 13 },
    { DtProperties2139_0_44, 1 },
    { DtProperties2139_0_45, 12 },
    { DtProperties2139_0_46, 13 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_0_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_0_53, 12 },
    { NULL, 0 },
    { DtProperties2139_0_55, 12 },
    { NULL, 0 },
    { DtProperties2139_0_57, 12 },
    { NULL, 0 },
    { DtProperties2139_0_59, 37 },
    { DtProperties2139_0_60, 13 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_0_63, 13 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2142; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Multi-mode serial and parallel port
// 
static const DtProperty  DtProperties2142_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Support for transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_01[] =
{
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1, "Support for transmit on timestamp mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_02[] =
{
    { "FIRMWARE_R_N1_2", -1,
      "Support for locking rate of ASI output to the rate of an ASI input",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_03[] =
{
    { "CAP_SPI525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_3", -1,
      "Fixes issue; where certain PCs would detect the card as a graphics card",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_04[] =
{
    { "CAP_SPI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_07[] =
{
    { "CAP_SDITIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_SPISER8B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_SPI", 0x080, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x180, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x380, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_SPICLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_14[] =
{
    { "CAP_SPIFIXEDCLK", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_SPILVDS1", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_15[] =
{
    { "CAP_SPI625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_SPICLKEXT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_SPILVDS2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_16[] =
{
    { "CAP_TXONTIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_26[] =
{
    { "CAP_SPILVTTL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 64, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_28[] =
{
    { "CAP_EXTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", 200000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_31[] =
{
    { "CAP_RAWASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DEFAULT_SPISTD", DT_IOCONFIG_SPILVDS1, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_36[] =
{
    { "CAP_SPISDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_37[] =
{
    { "CAP_SPIDVBMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_40[] =
{
    { "CAP_EXTRATIO", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_49[] =
{
    { "CAP_SPISER10B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Multi-mode serial and parallel port", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_60[] =
{
    { "PCI_DEVICE_ID", 0x85E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2142_0[] =
{
    { DtProperties2142_0_00, 2 },
    { DtProperties2142_0_01, 2 },
    { DtProperties2142_0_02, 1 },
    { DtProperties2142_0_03, 2 },
    { DtProperties2142_0_04, 2 },
    { NULL, 0 },
    { DtProperties2142_0_06, 2 },
    { DtProperties2142_0_07, 2 },
    { DtProperties2142_0_08, 9 },
    { DtProperties2142_0_09, 3 },
    { DtProperties2142_0_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2142_0_14, 5 },
    { DtProperties2142_0_15, 3 },
    { DtProperties2142_0_16, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2142_0_19, 2 },
    { DtProperties2142_0_20, 1 },
    { DtProperties2142_0_21, 1 },
    { DtProperties2142_0_22, 2 },
    { DtProperties2142_0_23, 1 },
    { NULL, 0 },
    { DtProperties2142_0_25, 2 },
    { DtProperties2142_0_26, 4 },
    { NULL, 0 },
    { DtProperties2142_0_28, 2 },
    { DtProperties2142_0_29, 3 },
    { DtProperties2142_0_30, 1 },
    { DtProperties2142_0_31, 1 },
    { DtProperties2142_0_32, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2142_0_35, 3 },
    { DtProperties2142_0_36, 1 },
    { DtProperties2142_0_37, 1 },
    { NULL, 0 },
    { DtProperties2142_0_39, 1 },
    { DtProperties2142_0_40, 4 },
    { NULL, 0 },
    { DtProperties2142_0_42, 1 },
    { DtProperties2142_0_43, 1 },
    { DtProperties2142_0_44, 1 },
    { NULL, 0 },
    { DtProperties2142_0_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2142_0_49, 1 },
    { DtProperties2142_0_50, 1 },
    { DtProperties2142_0_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2142_0_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2142_0_59, 3 },
    { DtProperties2142_0_60, 1 },
    { DtProperties2142_0_61, 2 },
    { NULL, 0 },
    { DtProperties2142_0_63, 3 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2144; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Quad ASI/SDI in+out
// 
static const DtProperty  DtProperties2144_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Intermediate version with some Genlock bugfixes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_01[] =
{
    { "FIRMWARE_LAST_VER", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Bug fix: FIFO-size setting is ignored",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_02[] =
{
    { "FIRMWARE_R_N2_1", -1,
      "Bug fix: Potential stale of dma engine resulting in a system lockup",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_0", -1, "Support for hardware revision 3",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_03[] =
{
    { "FIRMWARE_R_N4_0", -1, "Support for RAW-ASI mode", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_04[] =
{
    { "CAP_GENLOCKED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N4_1", -1, "Support for transmit on timestamp mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N5_0", -1, "Fixes issue; Gen lock functionality not working",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_05[] =
{
    { "FIRMWARE_R_N4_2", -1,
      "Fixes issue; where certain PCs would detect the card as a graphics card",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N5_1", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N6_0", -1, "Fixes issue; Possible fail to sync to the input stream",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_06[] =
{
    { "FIRMWARE_R_N4_3", -1, "Support for 64-bit timestamping",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N6_1", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x500, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x700, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_07[] =
{
    { "CAP_SDITIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 5, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 5, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 5, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 5, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x180, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x380, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x580, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x780, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "GENLOCK_ASYNC_PORT", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x600, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x800, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_15[] =
{
    { "CAP_GENREF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_16[] =
{
    { "CAP_TXONTIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TIMESTAMP64", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 300, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_31[] =
{
    { "CAP_RAWASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_37[] =
{
    { "GENLOCK_ARCH", 2144, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_39[] =
{
    { "CAP_DBLBUF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 3, 200,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 200,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_SPI_EPCS16, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 5, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 5, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 5, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 5, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_50[] =
{
    { "PORT_COUNT", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 3, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 3, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 3, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Quad ASI/SDI in+out", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_60[] =
{
    { "PCI_DEVICE_ID", 0x860, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 3, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 3, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 3, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2144_0[] =
{
    { DtProperties2144_0_00, 5 },
    { DtProperties2144_0_01, 2 },
    { DtProperties2144_0_02, 2 },
    { DtProperties2144_0_03, 1 },
    { DtProperties2144_0_04, 6 },
    { DtProperties2144_0_05, 3 },
    { DtProperties2144_0_06, 6 },
    { DtProperties2144_0_07, 4 },
    { DtProperties2144_0_08, 17 },
    { DtProperties2144_0_09, 5 },
    { DtProperties2144_0_10, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2144_0_14, 5 },
    { DtProperties2144_0_15, 1 },
    { DtProperties2144_0_16, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2144_0_19, 4 },
    { DtProperties2144_0_20, 1 },
    { DtProperties2144_0_21, 4 },
    { DtProperties2144_0_22, 8 },
    { DtProperties2144_0_23, 4 },
    { NULL, 0 },
    { DtProperties2144_0_25, 2 },
    { DtProperties2144_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2144_0_29, 5 },
    { DtProperties2144_0_30, 1 },
    { DtProperties2144_0_31, 8 },
    { DtProperties2144_0_32, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2144_0_35, 6 },
    { NULL, 0 },
    { DtProperties2144_0_37, 1 },
    { NULL, 0 },
    { DtProperties2144_0_39, 5 },
    { DtProperties2144_0_40, 2 },
    { NULL, 0 },
    { DtProperties2144_0_42, 3 },
    { DtProperties2144_0_43, 1 },
    { DtProperties2144_0_44, 1 },
    { NULL, 0 },
    { DtProperties2144_0_46, 9 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2144_0_50, 1 },
    { DtProperties2144_0_51, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2144_0_56, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2144_0_59, 9 },
    { DtProperties2144_0_60, 1 },
    { DtProperties2144_0_61, 8 },
    { NULL, 0 },
    { DtProperties2144_0_63, 5 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2145; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// ASI/SDI in+out with bypass relais
// 
static const DtProperty  DtProperties2145_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Support for reception/transmission of 10-bit SDI",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_01[] =
{
    { "FIRMWARE_LAST_VER", 10, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1, "Support for Huffman-coded SDI data",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Support for time-stamping of SDI frames",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_02[] =
{
    { "FIRMWARE_R_N2_1", -1, "Support transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_0", -1,
      "Fixes issue with DVB-ASI streams that have a bitrate >= 108Mbps",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_03[] =
{
    { "FIRMWARE_R_N2_2", -1, "Support for double-buffered mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_1", -1, "Support for SDI genlock", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N4_0", -1, "Intermediate version with some Genlock bugfixes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_04[] =
{
    { "CAP_GENLOCKED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_3", -1, "Bug fix: FIFO-size setting is ignored",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N5_0", -1, "Few minor bug-fixes", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_05[] =
{
    { "FIRMWARE_R_N2_4", -1,
      "Bug fix: Potential stale of dma engine resulting in a system lockup",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_06[] =
{
    { "FIRMWARE_R_N2_5", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_07[] =
{
    { "CAP_SDITIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N9_0", -1, "Support for RAW-ASI mode", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x180, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x380, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N9_1", -1, "Support for transmit on timestamp mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "GENLOCK_ASYNC_PORT", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "FIRMWARE_R_N9_2", -1, "Support for ASI bitrate lock", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_11[] =
{
    { "FIRMWARE_R_N9_3", -1,
      "Fixes issue; where certain PCs would detect the card as a graphics card",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_12[] =
{
    { "FIRMWARE_R_N9_4", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_15[] =
{
    { "CAP_GENREF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_16[] =
{
    { "CAP_TXONTIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N10_0", -1, "Fixes issue; Possible fail to sync to the input stream",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_17[] =
{
    { "FIRMWARE_R_N10_1", -1, "Several other minor bug fixes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_19[] =
{
    { "CAP_FAILSAFE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_31[] =
{
    { "CAP_RAWASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_32[] =
{
    { "CAP_LOOPTHR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_37[] =
{
    { "GENLOCK_ARCH", 145, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_39[] =
{
    { "CAP_DBLBUF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_58[] =
{
    { "CAP_LOCK2INP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "ASI/SDI in+out with bypass relais", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_60[] =
{
    { "PCI_DEVICE_ID", 0x861, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2145_0[] =
{
    { DtProperties2145_0_00, 3 },
    { DtProperties2145_0_01, 3 },
    { DtProperties2145_0_02, 2 },
    { DtProperties2145_0_03, 3 },
    { DtProperties2145_0_04, 4 },
    { DtProperties2145_0_05, 1 },
    { DtProperties2145_0_06, 2 },
    { DtProperties2145_0_07, 1 },
    { DtProperties2145_0_08, 7 },
    { DtProperties2145_0_09, 4 },
    { DtProperties2145_0_10, 4 },
    { DtProperties2145_0_11, 1 },
    { DtProperties2145_0_12, 1 },
    { NULL, 0 },
    { DtProperties2145_0_14, 3 },
    { DtProperties2145_0_15, 1 },
    { DtProperties2145_0_16, 4 },
    { DtProperties2145_0_17, 1 },
    { NULL, 0 },
    { DtProperties2145_0_19, 2 },
    { DtProperties2145_0_20, 1 },
    { DtProperties2145_0_21, 2 },
    { DtProperties2145_0_22, 2 },
    { DtProperties2145_0_23, 2 },
    { NULL, 0 },
    { DtProperties2145_0_25, 2 },
    { DtProperties2145_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2145_0_29, 3 },
    { DtProperties2145_0_30, 1 },
    { DtProperties2145_0_31, 4 },
    { DtProperties2145_0_32, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2145_0_35, 4 },
    { NULL, 0 },
    { DtProperties2145_0_37, 1 },
    { NULL, 0 },
    { DtProperties2145_0_39, 2 },
    { DtProperties2145_0_40, 2 },
    { NULL, 0 },
    { DtProperties2145_0_42, 1 },
    { DtProperties2145_0_43, 1 },
    { DtProperties2145_0_44, 1 },
    { NULL, 0 },
    { DtProperties2145_0_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2145_0_50, 1 },
    { DtProperties2145_0_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2145_0_56, 2 },
    { NULL, 0 },
    { DtProperties2145_0_58, 2 },
    { DtProperties2145_0_59, 3 },
    { DtProperties2145_0_60, 1 },
    { DtProperties2145_0_61, 4 },
    { NULL, 0 },
    { DtProperties2145_0_63, 3 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2152; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Dual HD-SDI in+out
// 
static const DtProperty  DtProperties2152_0_01[] =
{
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_04[] =
{
    { "CAP_GENLOCKED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_HDSDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_06[] =
{
    { "CAP_1080P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_07[] =
{
    { "CAP_720P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_08[] =
{
    { "CAP_720P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "GENLOCK_ASYNC_PORT", 3, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_GS2962, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0,
                                                      0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_GS2962, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_11[] =
{
    { "CAP_1080P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_15[] =
{
    { "CAP_GENREF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_16[] =
{
    { "CAP_1080I60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_23[] =
{
    { "CAP_1080P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_24[] =
{
    { "CAP_1080P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_25[] =
{
    { "CAP_FRACMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "INTERRUPT_PER_INT_CLK_BIT", 20, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_29[] =
{
    { "HARDWARE_I2C_FREQ", 400000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_36[] =
{
    { "CAP_1080I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_37[] =
{
    { "GENLOCK_ARCH", 2152, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MATRIX_MEM_PCK_NUM_BITS_USED", 160, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_38[] =
{
    { "CAP_720P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_45[] =
{
    { "CAP_MATRIX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_47[] =
{
    { "CAP_1080I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MATRIX_MEM_PCK_SIZE", 160, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_50[] =
{
    { "PORT_COUNT", 3, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_GS2961, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_GS2961, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MATRIX_RAM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_52[] =
{
    { "CAP_1080P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_54[] =
{
    { "CAP_720P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_59[] =
{
    { "CAP_720P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Dual HD-SDI in+out", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_60[] =
{
    { "PCI_DEVICE_ID", 0x868, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_61[] =
{
    { "MATRIX_MEM_LINE_ALIGNMENT", 512, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_0_63[] =
{
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2152_0[] =
{
    { NULL, 0 },
    { DtProperties2152_0_01, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2152_0_04, 6 },
    { NULL, 0 },
    { DtProperties2152_0_06, 5 },
    { DtProperties2152_0_07, 6 },
    { DtProperties2152_0_08, 10 },
    { DtProperties2152_0_09, 3 },
    { DtProperties2152_0_10, 3 },
    { DtProperties2152_0_11, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2152_0_14, 3 },
    { DtProperties2152_0_15, 3 },
    { DtProperties2152_0_16, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2152_0_20, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2152_0_23, 6 },
    { DtProperties2152_0_24, 3 },
    { DtProperties2152_0_25, 3 },
    { DtProperties2152_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2152_0_29, 1 },
    { DtProperties2152_0_30, 1 },
    { NULL, 0 },
    { DtProperties2152_0_32, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2152_0_35, 5 },
    { DtProperties2152_0_36, 6 },
    { DtProperties2152_0_37, 2 },
    { DtProperties2152_0_38, 3 },
    { DtProperties2152_0_39, 1 },
    { DtProperties2152_0_40, 2 },
    { NULL, 0 },
    { DtProperties2152_0_42, 1 },
    { DtProperties2152_0_43, 1 },
    { DtProperties2152_0_44, 1 },
    { DtProperties2152_0_45, 2 },
    { DtProperties2152_0_46, 3 },
    { DtProperties2152_0_47, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2152_0_50, 1 },
    { DtProperties2152_0_51, 3 },
    { DtProperties2152_0_52, 3 },
    { NULL, 0 },
    { DtProperties2152_0_54, 3 },
    { NULL, 0 },
    { DtProperties2152_0_56, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2152_0_59, 9 },
    { DtProperties2152_0_60, 1 },
    { DtProperties2152_0_61, 3 },
    { NULL, 0 },
    { DtProperties2152_0_63, 1 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2154; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Quad HD-SDI/ASI in+out
// 
static const DtProperty  DtProperties2154_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Support for RS-422 ports", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_01[] =
{
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1, "Improved DMA performance", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_02[] =
{
    { "FIRMWARE_R_N1_2", -1, "Bug fix: deadlock after DMA abort",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_03[] =
{
    { "FIRMWARE_R_N1_3", -1, "Bug fix: stability fixes for ASI load reporting",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_04[] =
{
    { "CAP_GENLOCKED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_HDSDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "GUARD_ID", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_06[] =
{
    { "CAP_1080P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x500, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_07[] =
{
    { "CAP_720P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_08[] =
{
    { "CAP_720P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x500, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 } 
};
static const DtProperty  DtProperties2154_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_11[] =
{
    { "CAP_1080P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_12[] =
{
    { "DEFAULT_IOSTD", DT_IOCONFIG_1080I50, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_1080I50, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x600, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_15[] =
{
    { "CAP_GENREF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_16[] =
{
    { "CAP_1080I60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_20[] =
{
    { "SUBDVC_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_22[] =
{
    { "CAP_TIMESTAMP64", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_23[] =
{
    { "CAP_1080P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_SEC", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_24[] =
{
    { "CAP_1080P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 20, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 100, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_29[] =
{
    { "HARDWARE_I2C_FREQ", 100000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 5, 2, 6 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 5, 2, 6 } 
};
static const DtProperty  DtProperties2154_0_34[] =
{
    { "REGISTERS_GENL", 0x080, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_36[] =
{
    { "CAP_1080I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_37[] =
{
    { "GENLOCK_ARCH", 2154, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MATRIX_MEM_PCK_NUM_BITS_USED", 120, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_38[] =
{
    { "CAP_720P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_39[] =
{
    { "EEPROM_SIZE", 2048, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_42[] =
{
    { "DMA_LOCAL_ADDR_FIXED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_CFI_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_45[] =
{
    { "CAP_MATRIX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 } 
};
static const DtProperty  DtProperties2154_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_CFI", 0x180, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_47[] =
{
    { "CAP_1080I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MATRIX_MEM_PCK_SIZE", 128, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_50[] =
{
    { "PORT_COUNT", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MATRIX_RAM_SIZE", 256, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_52[] =
{
    { "CAP_1080P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_53[] =
{
    { "CAP_RS422", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_54[] =
{
    { "CAP_1080P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_57[] =
{
    { "REGISTERS_RS422", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_59[] =
{
    { "CAP_720P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Quad HD-SDI/ASI in+out", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_60[] =
{
    { "PCI_DEVICE_ID", 0x86A, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_61[] =
{
    { "MATRIX_MEM_LINE_ALIGNMENT", 128, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First (beta) firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2154_0[] =
{
    { DtProperties2154_0_00, 3 },
    { DtProperties2154_0_01, 2 },
    { DtProperties2154_0_02, 1 },
    { DtProperties2154_0_03, 1 },
    { DtProperties2154_0_04, 7 },
    { NULL, 0 },
    { DtProperties2154_0_06, 5 },
    { DtProperties2154_0_07, 6 },
    { DtProperties2154_0_08, 8 },
    { DtProperties2154_0_09, 2 },
    { DtProperties2154_0_10, 3 },
    { DtProperties2154_0_11, 5 },
    { DtProperties2154_0_12, 2 },
    { NULL, 0 },
    { DtProperties2154_0_14, 3 },
    { DtProperties2154_0_15, 2 },
    { DtProperties2154_0_16, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2154_0_20, 1 },
    { DtProperties2154_0_21, 2 },
    { DtProperties2154_0_22, 2 },
    { DtProperties2154_0_23, 9 },
    { DtProperties2154_0_24, 3 },
    { DtProperties2154_0_25, 2 },
    { DtProperties2154_0_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2154_0_29, 1 },
    { DtProperties2154_0_30, 1 },
    { DtProperties2154_0_31, 2 },
    { DtProperties2154_0_32, 2 },
    { NULL, 0 },
    { DtProperties2154_0_34, 1 },
    { DtProperties2154_0_35, 5 },
    { DtProperties2154_0_36, 6 },
    { DtProperties2154_0_37, 2 },
    { DtProperties2154_0_38, 3 },
    { DtProperties2154_0_39, 1 },
    { DtProperties2154_0_40, 2 },
    { NULL, 0 },
    { DtProperties2154_0_42, 3 },
    { DtProperties2154_0_43, 1 },
    { DtProperties2154_0_44, 1 },
    { DtProperties2154_0_45, 2 },
    { DtProperties2154_0_46, 4 },
    { DtProperties2154_0_47, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2154_0_50, 1 },
    { DtProperties2154_0_51, 3 },
    { DtProperties2154_0_52, 3 },
    { DtProperties2154_0_53, 1 },
    { DtProperties2154_0_54, 5 },
    { NULL, 0 },
    { DtProperties2154_0_56, 3 },
    { DtProperties2154_0_57, 1 },
    { NULL, 0 },
    { DtProperties2154_0_59, 9 },
    { DtProperties2154_0_60, 1 },
    { DtProperties2154_0_61, 3 },
    { NULL, 0 },
    { DtProperties2154_0_63, 3 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2154; Sub-device=1 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Quad HD-SDI/ASI in+out
// 
static const DtProperty  DtProperties2154_1_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Support for RS-422 ports", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_01[] =
{
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1, "Improved DMA performance", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_02[] =
{
    { "FIRMWARE_R_N1_2", -1, "Bug fix: deadlock after DMA abort",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_03[] =
{
    { "FIRMWARE_R_N1_3", -1, "Bug fix: stability fixes for ASI load reporting",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_04[] =
{
    { "CAP_GENLOCKED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_HDSDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "GUARD_ID", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_06[] =
{
    { "CAP_1080P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x500, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_07[] =
{
    { "CAP_720P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_08[] =
{
    { "CAP_720P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x500, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 } 
};
static const DtProperty  DtProperties2154_1_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_11[] =
{
    { "CAP_1080P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_12[] =
{
    { "DEFAULT_IOSTD", DT_IOCONFIG_1080I50, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_1080I50, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x600, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_16[] =
{
    { "CAP_1080I60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_20[] =
{
    { "SUBDVC_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_22[] =
{
    { "CAP_TIMESTAMP64", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_23[] =
{
    { "CAP_1080P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_SEC", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_24[] =
{
    { "CAP_1080P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 20, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 100, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 5, 2, 6 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 5, 2, 6 } 
};
static const DtProperty  DtProperties2154_1_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_36[] =
{
    { "CAP_1080I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_37[] =
{
    { "MATRIX_MEM_PCK_NUM_BITS_USED", 120, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_38[] =
{
    { "CAP_720P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_39[] =
{
    { "EEPROM_SIZE", 2048, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_42[] =
{
    { "DMA_LOCAL_ADDR_FIXED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_45[] =
{
    { "CAP_MATRIX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 } 
};
static const DtProperty  DtProperties2154_1_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_47[] =
{
    { "CAP_1080I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MATRIX_MEM_PCK_SIZE", 128, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_50[] =
{
    { "PORT_COUNT", 3, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MATRIX_RAM_SIZE", 256, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_52[] =
{
    { "CAP_1080P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_53[] =
{
    { "CAP_RS422", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_54[] =
{
    { "CAP_1080P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_57[] =
{
    { "REGISTERS_RS422", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_59[] =
{
    { "CAP_720P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Quad HD-SDI/ASI in+out", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_60[] =
{
    { "PCI_DEVICE_ID", 0x186A, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_61[] =
{
    { "MATRIX_MEM_LINE_ALIGNMENT", 128, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2154_1_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First (beta) firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2154_1[] =
{
    { DtProperties2154_1_00, 3 },
    { DtProperties2154_1_01, 2 },
    { DtProperties2154_1_02, 1 },
    { DtProperties2154_1_03, 1 },
    { DtProperties2154_1_04, 6 },
    { NULL, 0 },
    { DtProperties2154_1_06, 4 },
    { DtProperties2154_1_07, 4 },
    { DtProperties2154_1_08, 7 },
    { DtProperties2154_1_09, 2 },
    { DtProperties2154_1_10, 3 },
    { DtProperties2154_1_11, 4 },
    { DtProperties2154_1_12, 2 },
    { NULL, 0 },
    { DtProperties2154_1_14, 3 },
    { NULL, 0 },
    { DtProperties2154_1_16, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2154_1_20, 1 },
    { DtProperties2154_1_21, 2 },
    { DtProperties2154_1_22, 2 },
    { DtProperties2154_1_23, 7 },
    { DtProperties2154_1_24, 2 },
    { DtProperties2154_1_25, 2 },
    { DtProperties2154_1_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2154_1_29, 1 },
    { DtProperties2154_1_30, 1 },
    { DtProperties2154_1_31, 2 },
    { DtProperties2154_1_32, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2154_1_35, 4 },
    { DtProperties2154_1_36, 4 },
    { DtProperties2154_1_37, 1 },
    { DtProperties2154_1_38, 2 },
    { DtProperties2154_1_39, 1 },
    { DtProperties2154_1_40, 2 },
    { NULL, 0 },
    { DtProperties2154_1_42, 3 },
    { DtProperties2154_1_43, 1 },
    { DtProperties2154_1_44, 1 },
    { DtProperties2154_1_45, 2 },
    { DtProperties2154_1_46, 3 },
    { DtProperties2154_1_47, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2154_1_50, 1 },
    { DtProperties2154_1_51, 3 },
    { DtProperties2154_1_52, 2 },
    { DtProperties2154_1_53, 1 },
    { DtProperties2154_1_54, 4 },
    { NULL, 0 },
    { DtProperties2154_1_56, 2 },
    { DtProperties2154_1_57, 1 },
    { NULL, 0 },
    { DtProperties2154_1_59, 7 },
    { DtProperties2154_1_60, 1 },
    { DtProperties2154_1_61, 3 },
    { NULL, 0 },
    { DtProperties2154_1_63, 3 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2160; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// GigE + 3x ASI ports
// 
static const DtProperty  DtProperties2160_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Support for transmit on timestamp mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NETWORK_MAC_TYPE", MAC_TYPE_CAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_01[] =
{
    { "FIRMWARE_LAST_VER", 3, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1,
      "Fixes issue; where certain PCs would detect the card as a graphics card",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Fixes issue; no null packets are sent on stop",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_02[] =
{
    { "FIRMWARE_R_N2_1", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_0", -1, "Bug fix; Possible fail to sync to the input stream",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_RT_TX_DMA", 0xC80, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_03[] =
{
    { "FIRMWARE_R_N3_1", -1, "Bug fix: Possible hang on full bandwidth(960Mbps)",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_04[] =
{
    { "FIRMWARE_R_N3_2", -1,
      "Bug fix: Possible old NDIS data transfered on 4K data transfer",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_05[] =
{
    { "FIRMWARE_R_N3_3", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x500, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_07[] =
{
    { "CAP_SDITIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x180, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x380, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x580, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "NW_REGISTERS_RX", 0xB00, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x600, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_16[] =
{
    { "CAP_TXONTIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_NRT_TX_DMA", 0xD80, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_17[] =
{
    { "CAP_IP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_MAC", 0x900, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "NW_REGISTERS_RT_TX", 0xC00, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 20, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 100, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_26[] =
{
    { "NW_REGISTERS_GEN", 0xA00, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 64, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_NRT_TX", 0xD00, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_32[] =
{
    { "CAP_LOOPTHR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_39[] =
{
    { "CAP_DBLBUF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_MAC_23_14", 0x010, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_50[] =
{
    { "PORT_COUNT", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_59[] =
{
    { "NW_REGISTERS_RX_DMA", 0xB80, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "GigE + 3x ASI ports", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_60[] =
{
    { "PCI_DEVICE_ID", 0x870, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2160_0[] =
{
    { DtProperties2160_0_00, 5 },
    { DtProperties2160_0_01, 3 },
    { DtProperties2160_0_02, 3 },
    { DtProperties2160_0_03, 1 },
    { DtProperties2160_0_04, 2 },
    { DtProperties2160_0_05, 1 },
    { DtProperties2160_0_06, 3 },
    { DtProperties2160_0_07, 3 },
    { DtProperties2160_0_08, 14 },
    { DtProperties2160_0_09, 3 },
    { DtProperties2160_0_10, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2160_0_14, 4 },
    { NULL, 0 },
    { DtProperties2160_0_16, 5 },
    { DtProperties2160_0_17, 3 },
    { NULL, 0 },
    { DtProperties2160_0_19, 4 },
    { DtProperties2160_0_20, 1 },
    { DtProperties2160_0_21, 3 },
    { DtProperties2160_0_22, 4 },
    { DtProperties2160_0_23, 3 },
    { NULL, 0 },
    { DtProperties2160_0_25, 2 },
    { DtProperties2160_0_26, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2160_0_29, 4 },
    { DtProperties2160_0_30, 1 },
    { DtProperties2160_0_31, 5 },
    { DtProperties2160_0_32, 6 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2160_0_35, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2160_0_39, 5 },
    { DtProperties2160_0_40, 2 },
    { NULL, 0 },
    { DtProperties2160_0_42, 1 },
    { DtProperties2160_0_43, 1 },
    { DtProperties2160_0_44, 1 },
    { NULL, 0 },
    { DtProperties2160_0_46, 8 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2160_0_50, 1 },
    { DtProperties2160_0_51, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2160_0_56, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2160_0_59, 8 },
    { DtProperties2160_0_60, 1 },
    { DtProperties2160_0_61, 6 },
    { NULL, 0 },
    { DtProperties2160_0_63, 5 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2162; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Dual GigE ports
// 
static const DtProperty  DtProperties2162_0_00[] =
{
    { "NETWORK_MAC_TYPE", MAC_TYPE_ALTERA, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_RT_RX_DMA", 0xC80, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0xC80, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_0_01[] =
{
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_02[] =
{
    { "NW_REGISTERS_RT_TX_DMA", 0xD80, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0xD80, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_07[] =
{
    { "NW_REGISTERS_ADDR_MATCH", 0x480, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x880, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_0_08[] =
{
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "NW_REGISTERS_NRT_RX_DMA", 0x580, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x980, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_0_15[] =
{
    { "NW_REGISTERS_RT_RX", 0x700, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0xb00, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_0_16[] =
{
    { "NW_REGISTERS_NRT_TX_DMA", 0x680, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0xA80, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_17[] =
{
    { "CAP_IP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_MAC", 0x1000, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1800, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 },
    { "NW_REGISTERS_RT_TX", 0x780, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0xb80, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 20, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_26[] =
{
    { "NW_REGISTERS_GEN", 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x800, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 },
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 64, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_29[] =
{
    { "CAP_IPPAIR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", 400000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_NRT_RX", 0x500, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x900, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_0_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_NRT_TX", 0x600, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0xA00, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_ADDR_MATCH_LU", 0xC00, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0xC00, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 },
    { "VPD_MAC_23_14", 0x012, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_52[] =
{
    { "NW_REGISTERS_MEMCTRL", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x140, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_0_59[] =
{
    { "VPD_ID", -1, "Dual GigE ports", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_60[] =
{
    { "PCI_DEVICE_ID", 0x872, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_SORTER", 0xD00, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0xD00, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2162_0[] =
{
    { DtProperties2162_0_00, 3 },
    { DtProperties2162_0_01, 1 },
    { DtProperties2162_0_02, 2 },
    { NULL, 0 },
    { DtProperties2162_0_04, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_0_07, 2 },
    { DtProperties2162_0_08, 3 },
    { NULL, 0 },
    { DtProperties2162_0_10, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_0_14, 3 },
    { DtProperties2162_0_15, 2 },
    { DtProperties2162_0_16, 3 },
    { DtProperties2162_0_17, 6 },
    { NULL, 0 },
    { DtProperties2162_0_19, 2 },
    { DtProperties2162_0_20, 1 },
    { NULL, 0 },
    { DtProperties2162_0_22, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_0_25, 2 },
    { DtProperties2162_0_26, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_0_29, 5 },
    { DtProperties2162_0_30, 1 },
    { DtProperties2162_0_31, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_0_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_0_39, 4 },
    { DtProperties2162_0_40, 2 },
    { NULL, 0 },
    { DtProperties2162_0_42, 1 },
    { DtProperties2162_0_43, 1 },
    { DtProperties2162_0_44, 1 },
    { NULL, 0 },
    { DtProperties2162_0_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_0_50, 1 },
    { NULL, 0 },
    { DtProperties2162_0_52, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_0_59, 1 },
    { DtProperties2162_0_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_0_63, 5 }
};


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertyStores -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// References to hash table per device type.
//
const DtPropertyStore  DtPropertyStores[] =
{
    { 100, 0, DtPropertyHashSets100_0, 64 },
    { 102, 0, DtPropertyHashSets102_0, 64 },
    { 105, 0, DtPropertyHashSets105_0, 64 },
    { 107, 0, DtPropertyHashSets107_0, 64 },
    { 110, 0, DtPropertyHashSets110_0, 64 },
    { 111, 0, DtPropertyHashSets111_0, 64 },
    { 112, 0, DtPropertyHashSets112_0, 64 },
    { 115, 0, DtPropertyHashSets115_0, 64 },
    { 116, 0, DtPropertyHashSets116_0, 64 },
    { 117, 0, DtPropertyHashSets117_0, 64 },
    { 120, 0, DtPropertyHashSets120_0, 64 },
    { 122, 0, DtPropertyHashSets122_0, 64 },
    { 124, 0, DtPropertyHashSets124_0, 64 },
    { 140, 0, DtPropertyHashSets140_0, 64 },
    { 145, 0, DtPropertyHashSets145_0, 64 },
    { 160, 0, DtPropertyHashSets160_0, 64 },
    { 2107, 0, DtPropertyHashSets2107_0, 64 },
    { 2111, 0, DtPropertyHashSets2111_0, 64 },
    { 2131, 0, DtPropertyHashSets2131_0, 64 },
    { 2135, 0, DtPropertyHashSets2135_0, 64 },
    { 2136, 0, DtPropertyHashSets2136_0, 64 },
    { 2137, 0, DtPropertyHashSets2137_0, 64 },
    { 2138, 0, DtPropertyHashSets2138_0, 64 },
    { 2139, 0, DtPropertyHashSets2139_0, 64 },
    { 2142, 0, DtPropertyHashSets2142_0, 64 },
    { 2144, 0, DtPropertyHashSets2144_0, 64 },
    { 2145, 0, DtPropertyHashSets2145_0, 64 },
    { 2152, 0, DtPropertyHashSets2152_0, 64 },
    { 2154, 0, DtPropertyHashSets2154_0, 64 },
    { 2154, 1, DtPropertyHashSets2154_1, 64 },
    { 2160, 0, DtPropertyHashSets2160_0, 64 },
    { 2162, 0, DtPropertyHashSets2162_0, 64 }
};
const Int  DtPropertyStoreCount = 32;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IoConfigCodeStrings[] -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Array to translate from DT_IOCONFIG code to DT_IOCONFIG name
//
const char*  IoConfigCodeStrings[] =
{
/*  0 */ "IODIR",
/*  1 */ "IOSTD",
/*  2 */ "RFCLKSEL",
/*  3 */ "SPICLKSEL",
/*  4 */ "SPIMODE",
/*  5 */ "SPISTD",
/*  6 */ "TSRATESEL",
/*  7 */ "BW",
/*  8 */ "FAILSAFE",
/*  9 */ "FRACMODE",
/* 10 */ "GENLOCKED",
/* 11 */ "GENREF",
/* 12 */ "SWS2APSK",
/* 13 */ "TRUE",
/* 14 */ "FALSE",
/* 15 */ "DISABLED",
/* 16 */ "INPUT",
/* 17 */ "OUTPUT",
/* 18 */ "SHAREDANT",
/* 19 */ "DBLBUF",
/* 20 */ "LOOPS2L3",
/* 21 */ "LOOPS2TS",
/* 22 */ "LOOPTHR",
/* 23 */ "3GSDI",
/* 24 */ "ASI",
/* 25 */ "DEMOD",
/* 26 */ "HDSDI",
/* 27 */ "IFADC",
/* 28 */ "IP",
/* 29 */ "MOD",
/* 30 */ "SDI",
/* 31 */ "SPI",
/* 32 */ "SPISDI",
/* 33 */ "1080P50",
/* 34 */ "1080P59_94",
/* 35 */ "1080P60",
/* 36 */ "1080I50",
/* 37 */ "1080I59_94",
/* 38 */ "1080I60",
/* 39 */ "1080P23_98",
/* 40 */ "1080P24",
/* 41 */ "1080P25",
/* 42 */ "1080P29_97",
/* 43 */ "1080P30",
/* 44 */ "720P23_98",
/* 45 */ "720P24",
/* 46 */ "720P25",
/* 47 */ "720P29_97",
/* 48 */ "720P30",
/* 49 */ "720P50",
/* 50 */ "720P59_94",
/* 51 */ "720P60",
/* 52 */ "525I59_94",
/* 53 */ "625I50",
/* 54 */ "SPI525I59_94",
/* 55 */ "SPI625I50",
/* 56 */ "RFCLKEXT",
/* 57 */ "RFCLKINT",
/* 58 */ "SPICLKEXT",
/* 59 */ "SPICLKINT",
/* 60 */ "SPIFIXEDCLK",
/* 61 */ "SPIDVBMODE",
/* 62 */ "SPISER8B",
/* 63 */ "SPISER10B",
/* 64 */ "SPILVDS1",
/* 65 */ "SPILVDS2",
/* 66 */ "SPILVTTL",
/* 67 */ "EXTTSRATE",
/* 68 */ "EXTRATIO",
/* 69 */ "INTTSRATE",
/* 70 */ "LOCK2INP"
};
const Int  IoConfigCodeStringCount = 71;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IoConfigCodes_XX[] -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Helper arrays to map from DT_IOCONFIG name to DT_IOCONFIG code.
// The hash of the DT_IOCONFIG names in IoConfigCodes_XX[] is XX.
//
static const IoConfigCode  IoConfigCodes_0[] =
{
    { "FAILSAFE", 8 }
};
static const IoConfigCode  IoConfigCodes_1[] =
{
    { "1080P30", 43 }
};
static const IoConfigCode  IoConfigCodes_3[] =
{
    { "LOOPS2L3", 20 },
    { "1080P50", 33 },
    { "720P23_98", 44 }
};
static const IoConfigCode  IoConfigCodes_5[] =
{
    { "SDI", 30 }
};
static const IoConfigCode  IoConfigCodes_6[] =
{
    { "FRACMODE", 9 }
};
static const IoConfigCode  IoConfigCodes_7[] =
{
    { "SPILVTTL", 66 }
};
static const IoConfigCode  IoConfigCodes_8[] =
{
    { "IOSTD", 1 },
    { "720P29_97", 47 },
    { "720P59_94", 50 }
};
static const IoConfigCode  IoConfigCodes_14[] =
{
    { "DEMOD", 25 }
};
static const IoConfigCode  IoConfigCodes_16[] =
{
    { "FALSE", 14 },
    { "625I50", 53 }
};
static const IoConfigCode  IoConfigCodes_17[] =
{
    { "GENLOCKED", 10 },
    { "HDSDI", 26 },
    { "SPI", 31 },
    { "SPISDI", 32 },
    { "1080I59_94", 37 },
    { "720P30", 48 }
};
static const IoConfigCode  IoConfigCodes_18[] =
{
    { "SPIDVBMODE", 61 }
};
static const IoConfigCode  IoConfigCodes_19[] =
{
    { "720P50", 49 }
};
static const IoConfigCode  IoConfigCodes_20[] =
{
    { "DBLBUF", 19 }
};
static const IoConfigCode  IoConfigCodes_21[] =
{
    { "INPUT", 16 },
    { "EXTRATIO", 68 }
};
static const IoConfigCode  IoConfigCodes_22[] =
{
    { "SPIMODE", 4 },
    { "SPICLKINT", 59 }
};
static const IoConfigCode  IoConfigCodes_27[] =
{
    { "SPIFIXEDCLK", 60 }
};
static const IoConfigCode  IoConfigCodes_28[] =
{
    { "IODIR", 0 },
    { "SPISTD", 5 },
    { "TSRATESEL", 6 },
    { "SPI625I50", 55 },
    { "SPICLKEXT", 58 }
};
static const IoConfigCode  IoConfigCodes_29[] =
{
    { "1080I60", 38 }
};
static const IoConfigCode  IoConfigCodes_30[] =
{
    { "BW", 7 }
};
static const IoConfigCode  IoConfigCodes_31[] =
{
    { "3GSDI", 23 }
};
static const IoConfigCode  IoConfigCodes_34[] =
{
    { "ASI", 24 },
    { "RFCLKINT", 57 }
};
static const IoConfigCode  IoConfigCodes_35[] =
{
    { "SWS2APSK", 12 },
    { "INTTSRATE", 69 }
};
static const IoConfigCode  IoConfigCodes_36[] =
{
    { "1080P60", 35 },
    { "1080P24", 40 },
    { "525I59_94", 52 }
};
static const IoConfigCode  IoConfigCodes_37[] =
{
    { "TRUE", 13 },
    { "MOD", 29 },
    { "1080P25", 41 }
};
static const IoConfigCode  IoConfigCodes_39[] =
{
    { "LOCK2INP", 70 }
};
static const IoConfigCode  IoConfigCodes_40[] =
{
    { "RFCLKEXT", 56 }
};
static const IoConfigCode  IoConfigCodes_41[] =
{
    { "EXTTSRATE", 67 }
};
static const IoConfigCode  IoConfigCodes_43[] =
{
    { "LOOPS2TS", 21 }
};
static const IoConfigCode  IoConfigCodes_45[] =
{
    { "LOOPTHR", 22 }
};
static const IoConfigCode  IoConfigCodes_47[] =
{
    { "SPICLKSEL", 3 }
};
static const IoConfigCode  IoConfigCodes_48[] =
{
    { "SPI525I59_94", 54 }
};
static const IoConfigCode  IoConfigCodes_51[] =
{
    { "1080P23_98", 39 }
};
static const IoConfigCode  IoConfigCodes_52[] =
{
    { "720P24", 45 },
    { "720P60", 51 }
};
static const IoConfigCode  IoConfigCodes_53[] =
{
    { "720P25", 46 },
    { "SPISER8B", 62 }
};
static const IoConfigCode  IoConfigCodes_54[] =
{
    { "OUTPUT", 17 }
};
static const IoConfigCode  IoConfigCodes_56[] =
{
    { "1080P59_94", 34 },
    { "1080P29_97", 42 }
};
static const IoConfigCode  IoConfigCodes_59[] =
{
    { "RFCLKSEL", 2 },
    { "SPILVDS1", 64 }
};
static const IoConfigCode  IoConfigCodes_60[] =
{
    { "GENREF", 11 },
    { "IFADC", 27 },
    { "1080I50", 36 },
    { "SPILVDS2", 65 }
};
static const IoConfigCode  IoConfigCodes_61[] =
{
    { "DISABLED", 15 }
};
static const IoConfigCode  IoConfigCodes_62[] =
{
    { "IP", 28 },
    { "SPISER10B", 63 }
};
static const IoConfigCode  IoConfigCodes_63[] =
{
    { "SHAREDANT", 18 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IoConfigCodeHashSets[] -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// IoConfigCodeHashSets maps from hash value to IoConfigCodes_XX[].
//
const IoConfigCodeHashSet  IoConfigCodeHashSets[] =
{
    { IoConfigCodes_0, 1 },
    { IoConfigCodes_1, 1 },
    { NULL, 0 },
    { IoConfigCodes_3, 3 },
    { NULL, 0 },
    { IoConfigCodes_5, 1 },
    { IoConfigCodes_6, 1 },
    { IoConfigCodes_7, 1 },
    { IoConfigCodes_8, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { IoConfigCodes_14, 1 },
    { NULL, 0 },
    { IoConfigCodes_16, 2 },
    { IoConfigCodes_17, 6 },
    { IoConfigCodes_18, 1 },
    { IoConfigCodes_19, 1 },
    { IoConfigCodes_20, 1 },
    { IoConfigCodes_21, 2 },
    { IoConfigCodes_22, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { IoConfigCodes_27, 1 },
    { IoConfigCodes_28, 5 },
    { IoConfigCodes_29, 1 },
    { IoConfigCodes_30, 1 },
    { IoConfigCodes_31, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { IoConfigCodes_34, 2 },
    { IoConfigCodes_35, 2 },
    { IoConfigCodes_36, 3 },
    { IoConfigCodes_37, 3 },
    { NULL, 0 },
    { IoConfigCodes_39, 1 },
    { IoConfigCodes_40, 1 },
    { IoConfigCodes_41, 1 },
    { NULL, 0 },
    { IoConfigCodes_43, 1 },
    { NULL, 0 },
    { IoConfigCodes_45, 1 },
    { NULL, 0 },
    { IoConfigCodes_47, 1 },
    { IoConfigCodes_48, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { IoConfigCodes_51, 1 },
    { IoConfigCodes_52, 2 },
    { IoConfigCodes_53, 2 },
    { IoConfigCodes_54, 1 },
    { NULL, 0 },
    { IoConfigCodes_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { IoConfigCodes_59, 2 },
    { IoConfigCodes_60, 4 },
    { IoConfigCodes_61, 1 },
    { IoConfigCodes_62, 2 },
    { IoConfigCodes_63, 1 }
};
const Int  IoConfigCodesHashCount = 64;
