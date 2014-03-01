//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaPropertyStore.c *#*#*#*#*#*#*#*#*# (C) 2013 DekTec
//
// CapParser - Property Store - Tables storing device properties
//
// THIS FILE IS GENERATED, DO NOT EDIT!!!

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#include "DtaIncludes.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-100 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// DVB-ASI output
// 
static const DtProperty  DtProperties100_01[] =
{
    { "FIRMWARE_LAST_VER", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_02[] =
{
    { "FIRMWARE_R_N3_0", -1, "Add clear-FIFO function next to full channel reset",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_03[] =
{
    { "FIRMWARE_R_N4_0", -1, "No functional changes", PROPERTY_VALUE_TYPE_STRING, -1, 0,
                                                       0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_CHAN0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_04[] =
{
    { "FIRMWARE_R_N4_1", -1, "Reduce on-board noise level by improving FPGA equations",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N5_0", -1, "Adds support for transmit mode DTAPI_TXMODE_MIN16",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_05[] =
{
    { "FIRMWARE_R_N6_0", -1, "Adds support for in-system upgrading of firmware",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_08[] =
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
static const DtProperty  DtProperties100_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_HOTLINK, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0,
                                                      0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties100_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x07F, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x00, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_25[] =
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
static const DtProperty  DtProperties100_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0xD100, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties100_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x14B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_OLD, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_56[] =
{
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_59[] =
{
    { "VPD_ID", -1, "DVB-ASI output", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_60[] =
{
    { "PCI_DEVICE_ID", 0x9054, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties100_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets100[] =
{
    { NULL, 0 },
    { DtProperties100_01, 2 },
    { DtProperties100_02, 1 },
    { DtProperties100_03, 2 },
    { DtProperties100_04, 3 },
    { DtProperties100_05, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_08, 4 },
    { DtProperties100_09, 1 },
    { DtProperties100_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_14, 3 },
    { NULL, 0 },
    { DtProperties100_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_21, 1 },
    { DtProperties100_22, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_25, 4 },
    { DtProperties100_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_29, 1 },
    { DtProperties100_30, 1 },
    { DtProperties100_31, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_39, 1 },
    { DtProperties100_40, 2 },
    { NULL, 0 },
    { DtProperties100_42, 1 },
    { DtProperties100_43, 2 },
    { DtProperties100_44, 1 },
    { NULL, 0 },
    { DtProperties100_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties100_59, 1 },
    { DtProperties100_60, 1 },
    { DtProperties100_61, 1 },
    { NULL, 0 },
    { DtProperties100_63, 1 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-102 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// DVB-spi output
// 
static const DtProperty  DtProperties102_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_01[] =
{
    { "FIRMWARE_LAST_VER", 8, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Add support for 192-byte packets",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_02[] =
{
    { "FIRMWARE_R_N3_0", -1,
      "After power-up/hard-reset: LVDS transmitters start up in tri-state mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_03[] =
{
    { "FIRMWARE_R_N3_1", -1, "Add LED flashing upon power up",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N4_0", -1, "Avoid read-back error while LVDS buffers are disabled",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_CHAN0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_04[] =
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
static const DtProperty  DtProperties102_05[] =
{
    { "FIRMWARE_R_N5_1", -1, "Null-packet stuffing powers up in 'off' state",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N6_0", -1, "Reduce on-board noise level by improving FPGA equations",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_06[] =
{
    { "FIRMWARE_R_N5_2", -1, "Add LED-control bits to Receive-Control register",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N7_0", -1, "Add support for 130-byte packets",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_07[] =
{
    { "FIRMWARE_R_N5_3", -1,
      "Start-up LED flashing pattern now includes type number and fw-version",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N8_0", -1, "Add support for packet transmit mode DTAPI_TXMODE_MIN16",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_08[] =
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
static const DtProperty  DtProperties102_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_10[] =
{
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties102_14[] =
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
static const DtProperty  DtProperties102_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_25[] =
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
static const DtProperty  DtProperties102_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0xD102, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_28[] =
{
    { "CAP_EXTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_32[] =
{
    { "DEFAULT_SPISTD", DT_IOCONFIG_SPILVDS1, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x14B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_37[] =
{
    { "CAP_SPIDVBMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_OLD, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_56[] =
{
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_59[] =
{
    { "VPD_ID", -1, "DVB-spi output", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_60[] =
{
    { "PCI_DEVICE_ID", 0x9054, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties102_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets102[] =
{
    { DtProperties102_00, 1 },
    { DtProperties102_01, 2 },
    { DtProperties102_02, 1 },
    { DtProperties102_03, 3 },
    { DtProperties102_04, 4 },
    { DtProperties102_05, 2 },
    { DtProperties102_06, 2 },
    { DtProperties102_07, 2 },
    { DtProperties102_08, 5 },
    { DtProperties102_09, 1 },
    { DtProperties102_10, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties102_14, 4 },
    { NULL, 0 },
    { DtProperties102_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties102_22, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties102_25, 4 },
    { DtProperties102_26, 3 },
    { NULL, 0 },
    { DtProperties102_28, 1 },
    { DtProperties102_29, 1 },
    { DtProperties102_30, 1 },
    { NULL, 0 },
    { DtProperties102_32, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties102_35, 2 },
    { NULL, 0 },
    { DtProperties102_37, 1 },
    { NULL, 0 },
    { DtProperties102_39, 1 },
    { DtProperties102_40, 2 },
    { NULL, 0 },
    { DtProperties102_42, 1 },
    { DtProperties102_43, 2 },
    { DtProperties102_44, 1 },
    { NULL, 0 },
    { DtProperties102_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties102_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties102_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties102_59, 1 },
    { DtProperties102_60, 1 },
    { DtProperties102_61, 1 },
    { NULL, 0 },
    { DtProperties102_63, 1 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-105 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Dual DVB-ASI output
// 
static const DtProperty  DtProperties105_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Few minor bug fixes", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_01[] =
{
    { "FIRMWARE_LAST_VER", 3, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Bug fix: deadlock scenario on 66MHz PCI bus",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_02[] =
{
    { "FIRMWARE_R_N2_1", -1, "Bug fix: maximum FIFO size setting was ignored",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_0", -1, "Bug fix: stale of dma engine resulting in a system lockup",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_03[] =
{
    { "FIRMWARE_R_N3_1", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_08[] =
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
static const DtProperty  DtProperties105_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties105_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_39[] =
{
    { "CAP_DBLBUF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 2, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_59[] =
{
    { "VPD_ID", -1, "Dual DVB-ASI output", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_60[] =
{
    { "PCI_DEVICE_ID", 0x69, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties105_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets105[] =
{
    { DtProperties105_00, 3 },
    { DtProperties105_01, 2 },
    { DtProperties105_02, 2 },
    { DtProperties105_03, 1 },
    { DtProperties105_04, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_08, 5 },
    { DtProperties105_09, 2 },
    { DtProperties105_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_14, 3 },
    { NULL, 0 },
    { DtProperties105_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_21, 2 },
    { DtProperties105_22, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_25, 2 },
    { DtProperties105_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_29, 2 },
    { DtProperties105_30, 1 },
    { DtProperties105_31, 2 },
    { DtProperties105_32, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_39, 2 },
    { DtProperties105_40, 3 },
    { NULL, 0 },
    { DtProperties105_42, 1 },
    { DtProperties105_43, 2 },
    { DtProperties105_44, 1 },
    { NULL, 0 },
    { DtProperties105_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties105_59, 1 },
    { DtProperties105_60, 1 },
    { DtProperties105_61, 2 },
    { NULL, 0 },
    { DtProperties105_63, 3 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-107 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// DVB-S L-band modulator
// 
static const DtProperty  DtProperties107_00[] =
{
    { "FIRMWARE_R_N1_0", -1,
      "Fixes synchronisation problems with 204-byte Transport-Streams",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_01[] =
{
    { "FIRMWARE_LAST_VER", 3, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Support for DVB-S2 (DTC-372-S2 license required)",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_02[] =
{
    { "FIRMWARE_R_N3_0", -1,
      "Support for SNR and RF level control (DTC-305-CM license required)",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_RFPLL", 107, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_03[] =
{
    { "MOD_HW_HAS_AD9789", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_04[] =
{
    { "CAP_TX_DVBS2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 107, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_08[] =
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
static const DtProperty  DtProperties107_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_10[] =
{
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_11[] =
{
    { "CAP_SNR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties107_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x1FF, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_18[] =
{
    { "CAP_TX_DVBS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties107_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_ISDBS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties107_25[] =
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
static const DtProperty  DtProperties107_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0xD107, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_33[] =
{
    { "MOD_LEVEL_MAX", -270, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 88000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 88000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 45000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x14B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 45000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_43[] =
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
static const DtProperty  DtProperties107_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_45[] =
{
    { "MOD_FREQ_ABSMAX", 2300, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 45000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_51[] =
{
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties107_53[] =
{
    { "CAP_ADJLVL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 950, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_55[] =
{
    { "MOD_FREQ_MAX", 2150, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 88000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_57[] =
{
    { "CAP_LBAND", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties107_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 45000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_59[] =
{
    { "MOD_HW_HAS_SYMIQ_MAPPING", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "DVB-S L-band modulator", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_60[] =
{
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties107_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets107[] =
{
    { DtProperties107_00, 1 },
    { DtProperties107_01, 3 },
    { DtProperties107_02, 2 },
    { DtProperties107_03, 2 },
    { DtProperties107_04, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties107_08, 5 },
    { DtProperties107_09, 1 },
    { DtProperties107_10, 2 },
    { DtProperties107_11, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties107_14, 3 },
    { NULL, 0 },
    { DtProperties107_16, 1 },
    { NULL, 0 },
    { DtProperties107_18, 1 },
    { NULL, 0 },
    { DtProperties107_20, 1 },
    { NULL, 0 },
    { DtProperties107_22, 3 },
    { NULL, 0 },
    { DtProperties107_24, 2 },
    { DtProperties107_25, 4 },
    { DtProperties107_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties107_29, 1 },
    { DtProperties107_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties107_33, 2 },
    { DtProperties107_34, 1 },
    { DtProperties107_35, 3 },
    { DtProperties107_36, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties107_39, 1 },
    { DtProperties107_40, 2 },
    { NULL, 0 },
    { DtProperties107_42, 1 },
    { DtProperties107_43, 4 },
    { DtProperties107_44, 1 },
    { DtProperties107_45, 2 },
    { DtProperties107_46, 2 },
    { DtProperties107_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties107_50, 1 },
    { DtProperties107_51, 1 },
    { NULL, 0 },
    { DtProperties107_53, 3 },
    { NULL, 0 },
    { DtProperties107_55, 1 },
    { DtProperties107_56, 3 },
    { DtProperties107_57, 1 },
    { DtProperties107_58, 1 },
    { DtProperties107_59, 2 },
    { DtProperties107_60, 1 },
    { DtProperties107_61, 1 },
    { NULL, 0 },
    { DtProperties107_63, 2 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-110 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// QAM UHF modulator
// 
static const DtProperty  DtProperties110_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "Test release", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_01[] =
{
    { "FIRMWARE_LAST_VER", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1,
      "Add support for OFDM modulation (DTA-110T license required)",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_02[] =
{
    { "FIRMWARE_R_N3_0", -1, "Add support for setting carrier frequency in 0.1MHz steps",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_RFPLL", 110, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 107, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_03[] =
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
static const DtProperty  DtProperties110_04[] =
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
static const DtProperty  DtProperties110_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties110_08[] =
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
static const DtProperty  DtProperties110_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_10[] =
{
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_REGS_ADDR_MASK", 0x1FF, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_DMA", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_22[] =
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
static const DtProperty  DtProperties110_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_25[] =
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
static const DtProperty  DtProperties110_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0xD10A, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_33[] =
{
    { "MOD_LEVEL_MAX", -270, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 5000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 5000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 7100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x14B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 7100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_43[] =
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
static const DtProperty  DtProperties110_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_45[] =
{
    { "MOD_FREQ_ABSMAX", 870, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 7100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_53[] =
{
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 400, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_55[] =
{
    { "MOD_FREQ_MAX", 862, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 5000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 7100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_59[] =
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
static const DtProperty  DtProperties110_60[] =
{
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties110_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets110[] =
{
    { DtProperties110_00, 1 },
    { DtProperties110_01, 3 },
    { DtProperties110_02, 3 },
    { DtProperties110_03, 5 },
    { DtProperties110_04, 4 },
    { DtProperties110_05, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_08, 5 },
    { DtProperties110_09, 1 },
    { DtProperties110_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_14, 3 },
    { NULL, 0 },
    { DtProperties110_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_20, 1 },
    { NULL, 0 },
    { DtProperties110_22, 4 },
    { NULL, 0 },
    { DtProperties110_24, 1 },
    { DtProperties110_25, 4 },
    { DtProperties110_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_29, 1 },
    { DtProperties110_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_33, 2 },
    { DtProperties110_34, 1 },
    { DtProperties110_35, 3 },
    { DtProperties110_36, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_39, 1 },
    { DtProperties110_40, 2 },
    { NULL, 0 },
    { DtProperties110_42, 1 },
    { DtProperties110_43, 4 },
    { DtProperties110_44, 1 },
    { DtProperties110_45, 2 },
    { DtProperties110_46, 2 },
    { DtProperties110_47, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties110_53, 2 },
    { NULL, 0 },
    { DtProperties110_55, 1 },
    { DtProperties110_56, 3 },
    { NULL, 0 },
    { DtProperties110_58, 1 },
    { DtProperties110_59, 4 },
    { DtProperties110_60, 1 },
    { DtProperties110_61, 1 },
    { NULL, 0 },
    { DtProperties110_63, 2 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-111 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Multi-standard VHF/UHF modulator
// 
static const DtProperty  DtProperties111_01[] =
{
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 1, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_02[] =
{
    { "CAP_TX_CMMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_RFPLL", 115, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_03[] =
{
    { "CAP_TX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_HAS_AD9789", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_04[] =
{
    { "CAP_TX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 115, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties111_08[] =
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
static const DtProperty  DtProperties111_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_10[] =
{
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_14[] =
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
static const DtProperty  DtProperties111_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties111_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties111_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties111_25[] =
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
static const DtProperty  DtProperties111_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x6F, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_33[] =
{
    { "MOD_LEVEL_MAX", -150, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties111_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_41[] =
{
    { "CAP_TX_GOLD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties111_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_TX_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_43[] =
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
static const DtProperty  DtProperties111_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_45[] =
{
    { "MOD_FREQ_ABSMAX", 999, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_51[] =
{
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties111_52[] =
{
    { "CAP_TX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties111_53[] =
{
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_55[] =
{
    { "MOD_FREQ_MAX", 862, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "MOD_HW_HAS_SYMIQ_MAPPING", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Multi-standard VHF/UHF modulator", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties111_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets111[] =
{
    { NULL, 0 },
    { DtProperties111_01, 2 },
    { DtProperties111_02, 2 },
    { DtProperties111_03, 3 },
    { DtProperties111_04, 3 },
    { DtProperties111_05, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties111_08, 7 },
    { DtProperties111_09, 1 },
    { DtProperties111_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties111_14, 4 },
    { NULL, 0 },
    { DtProperties111_16, 1 },
    { NULL, 0 },
    { DtProperties111_18, 1 },
    { DtProperties111_19, 1 },
    { DtProperties111_20, 1 },
    { NULL, 0 },
    { DtProperties111_22, 3 },
    { NULL, 0 },
    { DtProperties111_24, 2 },
    { DtProperties111_25, 5 },
    { DtProperties111_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties111_29, 2 },
    { DtProperties111_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties111_33, 2 },
    { DtProperties111_34, 1 },
    { DtProperties111_35, 3 },
    { DtProperties111_36, 1 },
    { DtProperties111_37, 1 },
    { NULL, 0 },
    { DtProperties111_39, 1 },
    { DtProperties111_40, 2 },
    { DtProperties111_41, 1 },
    { DtProperties111_42, 3 },
    { DtProperties111_43, 4 },
    { DtProperties111_44, 1 },
    { DtProperties111_45, 2 },
    { DtProperties111_46, 2 },
    { DtProperties111_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties111_50, 1 },
    { DtProperties111_51, 1 },
    { DtProperties111_52, 1 },
    { DtProperties111_53, 2 },
    { NULL, 0 },
    { DtProperties111_55, 1 },
    { DtProperties111_56, 3 },
    { NULL, 0 },
    { DtProperties111_58, 1 },
    { DtProperties111_59, 3 },
    { DtProperties111_60, 2 },
    { DtProperties111_61, 1 },
    { NULL, 0 },
    { DtProperties111_63, 2 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-112 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// ATSC/DVB-T/QAM VHF/UHF modulator
// 
static const DtProperty  DtProperties112_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_01[] =
{
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 2, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_02[] =
{
    { "CAP_TX_CMMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_RFPLL", 115, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_03[] =
{
    { "CAP_TX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_HAS_AD9789", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_04[] =
{
    { "CAP_TX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 115, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties112_06[] =
{
    { "REGISTERS_RX", 0x080, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_08[] =
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
static const DtProperty  DtProperties112_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_14[] =
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
static const DtProperty  DtProperties112_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties112_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties112_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_22[] =
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
static const DtProperty  DtProperties112_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties112_25[] =
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
static const DtProperty  DtProperties112_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x70, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties112_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_33[] =
{
    { "MOD_LEVEL_MAX", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties112_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_TX_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_43[] =
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
static const DtProperty  DtProperties112_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_45[] =
{
    { "MOD_FREQ_ABSMAX", 999, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties112_52[] =
{
    { "CAP_TX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties112_53[] =
{
    { "CAP_ADJLVL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_55[] =
{
    { "MOD_FREQ_MAX", 862, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_59[] =
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
static const DtProperty  DtProperties112_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties112_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets112[] =
{
    { DtProperties112_00, 1 },
    { DtProperties112_01, 2 },
    { DtProperties112_02, 2 },
    { DtProperties112_03, 3 },
    { DtProperties112_04, 3 },
    { DtProperties112_05, 1 },
    { DtProperties112_06, 1 },
    { NULL, 0 },
    { DtProperties112_08, 10 },
    { DtProperties112_09, 2 },
    { DtProperties112_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties112_14, 5 },
    { NULL, 0 },
    { DtProperties112_16, 1 },
    { NULL, 0 },
    { DtProperties112_18, 1 },
    { DtProperties112_19, 1 },
    { DtProperties112_20, 1 },
    { DtProperties112_21, 1 },
    { DtProperties112_22, 4 },
    { NULL, 0 },
    { DtProperties112_24, 2 },
    { DtProperties112_25, 5 },
    { DtProperties112_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties112_29, 2 },
    { DtProperties112_30, 1 },
    { DtProperties112_31, 1 },
    { DtProperties112_32, 1 },
    { DtProperties112_33, 2 },
    { DtProperties112_34, 1 },
    { DtProperties112_35, 3 },
    { DtProperties112_36, 1 },
    { DtProperties112_37, 1 },
    { NULL, 0 },
    { DtProperties112_39, 1 },
    { DtProperties112_40, 2 },
    { NULL, 0 },
    { DtProperties112_42, 3 },
    { DtProperties112_43, 4 },
    { DtProperties112_44, 1 },
    { DtProperties112_45, 2 },
    { DtProperties112_46, 3 },
    { DtProperties112_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties112_50, 1 },
    { DtProperties112_51, 2 },
    { DtProperties112_52, 1 },
    { DtProperties112_53, 3 },
    { NULL, 0 },
    { DtProperties112_55, 1 },
    { DtProperties112_56, 3 },
    { NULL, 0 },
    { DtProperties112_58, 1 },
    { DtProperties112_59, 4 },
    { DtProperties112_60, 2 },
    { DtProperties112_61, 2 },
    { NULL, 0 },
    { DtProperties112_63, 3 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-115 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Multi-standard VHF/UHF modulator, ASI in+out
// 
static const DtProperty  DtProperties115_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Fixed stability issues with DVB-C modulation",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_01[] =
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
static const DtProperty  DtProperties115_02[] =
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
static const DtProperty  DtProperties115_03[] =
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
static const DtProperty  DtProperties115_04[] =
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
static const DtProperty  DtProperties115_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties115_06[] =
{
    { "REGISTERS_RX", 0x080, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_08[] =
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
static const DtProperty  DtProperties115_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_14[] =
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
static const DtProperty  DtProperties115_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties115_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties115_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_22[] =
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
static const DtProperty  DtProperties115_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties115_25[] =
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
static const DtProperty  DtProperties115_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x73, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties115_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_33[] =
{
    { "MOD_LEVEL_MAX", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties115_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_41[] =
{
    { "CAP_TX_GOLD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties115_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_TX_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS16, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_43[] =
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
static const DtProperty  DtProperties115_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_45[] =
{
    { "MOD_FREQ_ABSMAX", 999, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties115_52[] =
{
    { "CAP_TX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties115_53[] =
{
    { "CAP_ADJLVL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_55[] =
{
    { "MOD_FREQ_MAX", 862, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_59[] =
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
static const DtProperty  DtProperties115_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties115_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets115[] =
{
    { DtProperties115_00, 2 },
    { DtProperties115_01, 4 },
    { DtProperties115_02, 4 },
    { DtProperties115_03, 4 },
    { DtProperties115_04, 4 },
    { DtProperties115_05, 1 },
    { DtProperties115_06, 1 },
    { NULL, 0 },
    { DtProperties115_08, 10 },
    { DtProperties115_09, 2 },
    { DtProperties115_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties115_14, 5 },
    { NULL, 0 },
    { DtProperties115_16, 1 },
    { NULL, 0 },
    { DtProperties115_18, 1 },
    { DtProperties115_19, 1 },
    { DtProperties115_20, 1 },
    { DtProperties115_21, 1 },
    { DtProperties115_22, 4 },
    { NULL, 0 },
    { DtProperties115_24, 2 },
    { DtProperties115_25, 5 },
    { DtProperties115_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties115_29, 2 },
    { DtProperties115_30, 1 },
    { DtProperties115_31, 1 },
    { DtProperties115_32, 1 },
    { DtProperties115_33, 2 },
    { DtProperties115_34, 1 },
    { DtProperties115_35, 3 },
    { DtProperties115_36, 1 },
    { DtProperties115_37, 1 },
    { NULL, 0 },
    { DtProperties115_39, 1 },
    { DtProperties115_40, 2 },
    { DtProperties115_41, 1 },
    { DtProperties115_42, 3 },
    { DtProperties115_43, 4 },
    { DtProperties115_44, 1 },
    { DtProperties115_45, 2 },
    { DtProperties115_46, 3 },
    { DtProperties115_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties115_50, 1 },
    { DtProperties115_51, 2 },
    { DtProperties115_52, 1 },
    { DtProperties115_53, 3 },
    { NULL, 0 },
    { DtProperties115_55, 1 },
    { DtProperties115_56, 3 },
    { NULL, 0 },
    { DtProperties115_58, 1 },
    { DtProperties115_59, 4 },
    { DtProperties115_60, 2 },
    { DtProperties115_61, 2 },
    { NULL, 0 },
    { DtProperties115_63, 3 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-116 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Multi-standard 36MHz-IF/digital-IQ modulator
// 
static const DtProperty  DtProperties116_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1,
      "Fixed PCI performance issue when using both ports simultaneously",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_01[] =
{
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1,
      "New QAM channel filters; including 12% and 18% filters for QAM-B",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_02[] =
{
    { "CAP_TX_CMMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "FIRMWARE_R_N1_2", -1,
      "Improved carrier suppression => better MER / reduced carrier feed-through",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_RFPLL", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_03[] =
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
static const DtProperty  DtProperties116_04[] =
{
    { "CAP_TX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 115, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties116_06[] =
{
    { "CAP_DIGIQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "REGISTERS_RX", 0x080, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_07[] =
{
    { "CAP_IF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties116_08[] =
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
static const DtProperty  DtProperties116_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_14[] =
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
static const DtProperty  DtProperties116_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties116_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties116_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_22[] =
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
static const DtProperty  DtProperties116_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties116_25[] =
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
static const DtProperty  DtProperties116_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x74, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties116_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_33[] =
{
    { "MOD_LEVEL_MAX", -110, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties116_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_TX_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS16, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_43[] =
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
static const DtProperty  DtProperties116_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_45[] =
{
    { "MOD_FREQ_ABSMAX", 36, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties116_52[] =
{
    { "CAP_TX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties116_53[] =
{
    { "MOD_FREQ_MIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_55[] =
{
    { "MOD_FREQ_MAX", 36, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_59[] =
{
    { "MOD_HW_HAS_SYMIQ_MAPPING", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Multi-standard 36MHz-IF/digital-IQ modulator",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_60[] =
{
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties116_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets116[] =
{
    { DtProperties116_00, 2 },
    { DtProperties116_01, 3 },
    { DtProperties116_02, 3 },
    { DtProperties116_03, 4 },
    { DtProperties116_04, 3 },
    { DtProperties116_05, 1 },
    { DtProperties116_06, 2 },
    { DtProperties116_07, 1 },
    { DtProperties116_08, 10 },
    { DtProperties116_09, 2 },
    { DtProperties116_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties116_14, 5 },
    { NULL, 0 },
    { DtProperties116_16, 1 },
    { NULL, 0 },
    { DtProperties116_18, 1 },
    { DtProperties116_19, 1 },
    { DtProperties116_20, 1 },
    { DtProperties116_21, 1 },
    { DtProperties116_22, 4 },
    { NULL, 0 },
    { DtProperties116_24, 2 },
    { DtProperties116_25, 5 },
    { DtProperties116_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties116_29, 2 },
    { DtProperties116_30, 1 },
    { DtProperties116_31, 1 },
    { DtProperties116_32, 1 },
    { DtProperties116_33, 2 },
    { DtProperties116_34, 1 },
    { DtProperties116_35, 3 },
    { DtProperties116_36, 1 },
    { DtProperties116_37, 1 },
    { NULL, 0 },
    { DtProperties116_39, 1 },
    { DtProperties116_40, 2 },
    { NULL, 0 },
    { DtProperties116_42, 3 },
    { DtProperties116_43, 4 },
    { DtProperties116_44, 1 },
    { DtProperties116_45, 2 },
    { DtProperties116_46, 3 },
    { DtProperties116_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties116_50, 1 },
    { DtProperties116_51, 2 },
    { DtProperties116_52, 1 },
    { DtProperties116_53, 1 },
    { NULL, 0 },
    { DtProperties116_55, 1 },
    { DtProperties116_56, 3 },
    { NULL, 0 },
    { DtProperties116_58, 1 },
    { DtProperties116_59, 3 },
    { DtProperties116_60, 1 },
    { DtProperties116_61, 2 },
    { NULL, 0 },
    { DtProperties116_63, 3 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-117 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Multi-standard 44MHz-IF/digital-IQ modulator
// 
static const DtProperty  DtProperties117_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_01[] =
{
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_02[] =
{
    { "CAP_TX_CMMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_RFPLL", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_03[] =
{
    { "CAP_TX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_HAS_AD9789", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_04[] =
{
    { "CAP_TX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 117, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties117_06[] =
{
    { "CAP_DIGIQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "REGISTERS_RX", 0x080, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_07[] =
{
    { "CAP_IF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties117_08[] =
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
static const DtProperty  DtProperties117_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_14[] =
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
static const DtProperty  DtProperties117_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties117_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties117_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_22[] =
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
static const DtProperty  DtProperties117_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties117_25[] =
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
static const DtProperty  DtProperties117_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x75, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties117_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_33[] =
{
    { "MOD_LEVEL_MAX", -110, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties117_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_TX_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS16, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_43[] =
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
static const DtProperty  DtProperties117_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_45[] =
{
    { "MOD_FREQ_ABSMAX", 44, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 10000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties117_52[] =
{
    { "CAP_TX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties117_53[] =
{
    { "MOD_FREQ_MIN", 44, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_55[] =
{
    { "MOD_FREQ_MAX", 44, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 2000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 15000000, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_59[] =
{
    { "MOD_HW_HAS_SYMIQ_MAPPING", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Multi-standard 44MHz-IF/digital-IQ modulator",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_60[] =
{
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties117_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets117[] =
{
    { DtProperties117_00, 1 },
    { DtProperties117_01, 2 },
    { DtProperties117_02, 2 },
    { DtProperties117_03, 3 },
    { DtProperties117_04, 3 },
    { DtProperties117_05, 1 },
    { DtProperties117_06, 2 },
    { DtProperties117_07, 1 },
    { DtProperties117_08, 10 },
    { DtProperties117_09, 2 },
    { DtProperties117_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties117_14, 5 },
    { NULL, 0 },
    { DtProperties117_16, 1 },
    { NULL, 0 },
    { DtProperties117_18, 1 },
    { DtProperties117_19, 1 },
    { DtProperties117_20, 1 },
    { DtProperties117_21, 1 },
    { DtProperties117_22, 4 },
    { NULL, 0 },
    { DtProperties117_24, 2 },
    { DtProperties117_25, 5 },
    { DtProperties117_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties117_29, 2 },
    { DtProperties117_30, 1 },
    { DtProperties117_31, 1 },
    { DtProperties117_32, 1 },
    { DtProperties117_33, 2 },
    { DtProperties117_34, 1 },
    { DtProperties117_35, 3 },
    { DtProperties117_36, 1 },
    { DtProperties117_37, 1 },
    { NULL, 0 },
    { DtProperties117_39, 1 },
    { DtProperties117_40, 2 },
    { NULL, 0 },
    { DtProperties117_42, 3 },
    { DtProperties117_43, 4 },
    { DtProperties117_44, 1 },
    { DtProperties117_45, 2 },
    { DtProperties117_46, 3 },
    { DtProperties117_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties117_50, 1 },
    { DtProperties117_51, 2 },
    { DtProperties117_52, 1 },
    { DtProperties117_53, 1 },
    { NULL, 0 },
    { DtProperties117_55, 1 },
    { DtProperties117_56, 3 },
    { NULL, 0 },
    { DtProperties117_58, 1 },
    { DtProperties117_59, 3 },
    { DtProperties117_60, 1 },
    { DtProperties117_61, 2 },
    { NULL, 0 },
    { DtProperties117_63, 3 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-120 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// DVB-ASI input
// 
static const DtProperty  DtProperties120_01[] =
{
    { "FIRMWARE_LAST_VER", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_02[] =
{
    { "FIRMWARE_R_N3_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_03[] =
{
    { "FIRMWARE_R_N4_0", -1, "Add support for receive modes: ST188 ST204 and STRAW",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_CHAN0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_06[] =
{
    { "REGISTERS_RX", 0x20, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_08[] =
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
static const DtProperty  DtProperties120_10[] =
{
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties120_14[] =
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
static const DtProperty  DtProperties120_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_22[] =
{
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_25[] =
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
static const DtProperty  DtProperties120_26[] =
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
static const DtProperty  DtProperties120_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x14B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 4, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_OLD, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_46[] =
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
static const DtProperty  DtProperties120_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_56[] =
{
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "DVB-ASI input", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_60[] =
{
    { "PCI_DEVICE_ID", 0x9054, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties120_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets120[] =
{
    { NULL, 0 },
    { DtProperties120_01, 1 },
    { DtProperties120_02, 1 },
    { DtProperties120_03, 2 },
    { DtProperties120_04, 1 },
    { NULL, 0 },
    { DtProperties120_06, 2 },
    { NULL, 0 },
    { DtProperties120_08, 8 },
    { NULL, 0 },
    { DtProperties120_10, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_14, 4 },
    { NULL, 0 },
    { DtProperties120_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_21, 1 },
    { DtProperties120_22, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_25, 4 },
    { DtProperties120_26, 7 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_29, 1 },
    { DtProperties120_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_39, 1 },
    { DtProperties120_40, 2 },
    { NULL, 0 },
    { DtProperties120_42, 2 },
    { DtProperties120_43, 2 },
    { DtProperties120_44, 1 },
    { NULL, 0 },
    { DtProperties120_46, 6 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_50, 1 },
    { DtProperties120_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_59, 2 },
    { DtProperties120_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties120_63, 1 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-122 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// DVB-spi input
// 
static const DtProperty  DtProperties122_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "Add LED flashing at power-up", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_01[] =
{
    { "FIRMWARE_LAST_VER", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Add receive-clock detector", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_02[] =
{
    { "FIRMWARE_R_N2_1", -1,
      "Add FIFO lock-up detector; fixes occasional freezing of receive FIFO",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_0", -1, "New; improved FIFO lock-up detector",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_03[] =
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
static const DtProperty  DtProperties122_04[] =
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
static const DtProperty  DtProperties122_06[] =
{
    { "REGISTERS_RX", 0x20, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_08[] =
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
static const DtProperty  DtProperties122_10[] =
{
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties122_14[] =
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
static const DtProperty  DtProperties122_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_22[] =
{
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_25[] =
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
static const DtProperty  DtProperties122_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0xD116, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_32[] =
{
    { "DEFAULT_SPISTD", DT_IOCONFIG_SPILVDS1, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x14B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_37[] =
{
    { "CAP_SPIDVBMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_56[] =
{
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "DVB-spi input", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_60[] =
{
    { "PCI_DEVICE_ID", 0x9054, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties122_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets122[] =
{
    { DtProperties122_00, 1 },
    { DtProperties122_01, 2 },
    { DtProperties122_02, 2 },
    { DtProperties122_03, 4 },
    { DtProperties122_04, 4 },
    { NULL, 0 },
    { DtProperties122_06, 2 },
    { NULL, 0 },
    { DtProperties122_08, 7 },
    { NULL, 0 },
    { DtProperties122_10, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_14, 4 },
    { NULL, 0 },
    { DtProperties122_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_22, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_25, 4 },
    { DtProperties122_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_29, 1 },
    { DtProperties122_30, 1 },
    { NULL, 0 },
    { DtProperties122_32, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_35, 2 },
    { NULL, 0 },
    { DtProperties122_37, 1 },
    { NULL, 0 },
    { DtProperties122_39, 1 },
    { DtProperties122_40, 2 },
    { NULL, 0 },
    { DtProperties122_42, 1 },
    { DtProperties122_43, 2 },
    { DtProperties122_44, 1 },
    { NULL, 0 },
    { DtProperties122_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_59, 2 },
    { DtProperties122_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties122_63, 2 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-124 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Quad ASI/SDI input
// 
static const DtProperty  DtProperties124_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "Support for reception of 10-bit SDI",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_01[] =
{
    { "FIRMWARE_LAST_VER", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1, "Support for Huffman-coded SDI data",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Support for time-stamping of SDI frames",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_02[] =
{
    { "FIRMWARE_R_N2_1", -1, "Support transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_03[] =
{
    { "FIRMWARE_R_N2_2", -1, "Bug fix: could not read up to last word in HW FIFO",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_PRIO", DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DMA_PRIO_ROTATING, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_04[] =
{
    { "FIRMWARE_R_N2_3", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_06[] =
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
static const DtProperty  DtProperties124_07[] =
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
static const DtProperty  DtProperties124_08[] =
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
static const DtProperty  DtProperties124_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 2, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 2, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
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
static const DtProperty  DtProperties124_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_19[] =
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
static const DtProperty  DtProperties124_21[] =
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
static const DtProperty  DtProperties124_22[] =
{
    { "PLX_DMA_THRESHOLD", 0x21102110, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 1, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00000010, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_23[] =
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
static const DtProperty  DtProperties124_25[] =
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
static const DtProperty  DtProperties124_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0xD118, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_29[] =
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
static const DtProperty  DtProperties124_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 2, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_35[] =
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
static const DtProperty  DtProperties124_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 1, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_46[] =
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
static const DtProperty  DtProperties124_50[] =
{
    { "PORT_COUNT", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_51[] =
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
static const DtProperty  DtProperties124_56[] =
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
static const DtProperty  DtProperties124_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Quad ASI/SDI input", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_60[] =
{
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties124_63[] =
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
static const DtPropertyHashSet  DtPropertyHashSets124[] =
{
    { DtProperties124_00, 1 },
    { DtProperties124_01, 3 },
    { DtProperties124_02, 1 },
    { DtProperties124_03, 3 },
    { DtProperties124_04, 2 },
    { NULL, 0 },
    { DtProperties124_06, 8 },
    { DtProperties124_07, 4 },
    { DtProperties124_08, 11 },
    { NULL, 0 },
    { DtProperties124_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_14, 12 },
    { NULL, 0 },
    { DtProperties124_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_19, 4 },
    { NULL, 0 },
    { DtProperties124_21, 4 },
    { DtProperties124_22, 2 },
    { DtProperties124_23, 4 },
    { NULL, 0 },
    { DtProperties124_25, 6 },
    { DtProperties124_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_29, 5 },
    { DtProperties124_30, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_35, 6 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_39, 1 },
    { DtProperties124_40, 2 },
    { NULL, 0 },
    { DtProperties124_42, 1 },
    { DtProperties124_43, 3 },
    { DtProperties124_44, 1 },
    { NULL, 0 },
    { DtProperties124_46, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_50, 1 },
    { DtProperties124_51, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_56, 8 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_59, 5 },
    { DtProperties124_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties124_63, 5 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-140 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// DVB-ASI in+out
// 
static const DtProperty  DtProperties140_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_01[] =
{
    { "FIRMWARE_LAST_VER", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Improved load computation of receive FIFO",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_02[] =
{
    { "FIRMWARE_R_N2_1", -1,
      "Add LED flashing pattern when input channel is being reset",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_03[] =
{
    { "PLX_DMA_PRIO", DMA_PRIO_CHAN0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_08[] =
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
static const DtProperty  DtProperties140_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_HOTLINK, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0,
                                                      0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties140_14[] =
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
static const DtProperty  DtProperties140_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_THRESHOLD", 0x99109910, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_25[] =
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
static const DtProperty  DtProperties140_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0xD128, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x10B5, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties140_35[] =
{
    { "EEPROM_PAGE_SIZE", 0, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x14B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_HOTLINK, NULL, PROPERTY_VALUE_TYPE_INT,
                                                0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_56[] =
{
    { "PLX_DMA_CHAN1_MODE", 0x21F43, NULL, PROPERTY_VALUE_TYPE_UINT32, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PLX_DMA_CHAN1_PORT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "DVB-ASI in+out", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_60[] =
{
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties140_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets140[] =
{
    { DtProperties140_00, 2 },
    { DtProperties140_01, 2 },
    { DtProperties140_02, 1 },
    { DtProperties140_03, 1 },
    { DtProperties140_04, 1 },
    { NULL, 0 },
    { DtProperties140_06, 1 },
    { NULL, 0 },
    { DtProperties140_08, 6 },
    { DtProperties140_09, 1 },
    { DtProperties140_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_14, 4 },
    { NULL, 0 },
    { DtProperties140_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_21, 2 },
    { DtProperties140_22, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_25, 4 },
    { DtProperties140_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_29, 1 },
    { DtProperties140_30, 1 },
    { DtProperties140_31, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_39, 1 },
    { DtProperties140_40, 2 },
    { NULL, 0 },
    { DtProperties140_42, 1 },
    { DtProperties140_43, 2 },
    { DtProperties140_44, 1 },
    { NULL, 0 },
    { DtProperties140_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_50, 1 },
    { DtProperties140_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties140_59, 2 },
    { DtProperties140_60, 1 },
    { DtProperties140_61, 1 },
    { NULL, 0 },
    { DtProperties140_63, 2 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-145 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// ASI/SDI in+out with bypass relais
// 
static const DtProperty  DtProperties145_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Support for reception/transmission of 10-bit SDI",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1, "Support for Huffman-coded SDI data",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Support for time-stamping of SDI frames",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_02[] =
{
    { "FIRMWARE_R_N2_1", -1, "Support transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_0", -1,
      "Fixes issue with DVB-ASI streams that have a bitrate >= 108Mbps",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_03[] =
{
    { "FIRMWARE_R_N2_2", -1, "Support for double-buffered mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_1", -1, "Support for SDI genlock", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N4_0", -1, "Intermediate version with some Genlock bugfixes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_04[] =
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
static const DtProperty  DtProperties145_05[] =
{
    { "FIRMWARE_R_N2_4", -1,
      "Bug fix: Potential stale of dma engine resulting in a system lockup",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_06[] =
{
    { "FIRMWARE_R_N2_5", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_07[] =
{
    { "CAP_SDITIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_08[] =
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
static const DtProperty  DtProperties145_09[] =
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
static const DtProperty  DtProperties145_10[] =
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
static const DtProperty  DtProperties145_11[] =
{
    { "FIRMWARE_R_N9_3", -1,
      "Fixes issue; where certain PCs would detect the card as a graphics card",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_12[] =
{
    { "FIRMWARE_R_N9_4", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_15[] =
{
    { "CAP_GENREF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_16[] =
{
    { "CAP_TXONTIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_19[] =
{
    { "CAP_FAILSAFE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 100, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_31[] =
{
    { "CAP_RAWASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_32[] =
{
    { "CAP_LOOPTHR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_35[] =
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
static const DtProperty  DtProperties145_37[] =
{
    { "GENLOCK_ARCH", 145, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_39[] =
{
    { "CAP_DBLBUF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_46[] =
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
static const DtProperty  DtProperties145_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_58[] =
{
    { "CAP_LOCK2INP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "ASI/SDI in+out with bypass relais", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_60[] =
{
    { "PCI_DEVICE_ID", 0x91, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties145_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets145[] =
{
    { DtProperties145_00, 3 },
    { DtProperties145_01, 3 },
    { DtProperties145_02, 2 },
    { DtProperties145_03, 3 },
    { DtProperties145_04, 4 },
    { DtProperties145_05, 1 },
    { DtProperties145_06, 2 },
    { DtProperties145_07, 1 },
    { DtProperties145_08, 9 },
    { DtProperties145_09, 4 },
    { DtProperties145_10, 4 },
    { DtProperties145_11, 1 },
    { DtProperties145_12, 1 },
    { NULL, 0 },
    { DtProperties145_14, 3 },
    { DtProperties145_15, 1 },
    { DtProperties145_16, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties145_19, 2 },
    { NULL, 0 },
    { DtProperties145_21, 2 },
    { DtProperties145_22, 2 },
    { DtProperties145_23, 2 },
    { NULL, 0 },
    { DtProperties145_25, 2 },
    { DtProperties145_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties145_29, 3 },
    { DtProperties145_30, 1 },
    { DtProperties145_31, 4 },
    { DtProperties145_32, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties145_35, 4 },
    { NULL, 0 },
    { DtProperties145_37, 1 },
    { NULL, 0 },
    { DtProperties145_39, 2 },
    { DtProperties145_40, 2 },
    { NULL, 0 },
    { DtProperties145_42, 1 },
    { DtProperties145_43, 1 },
    { DtProperties145_44, 1 },
    { NULL, 0 },
    { DtProperties145_46, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties145_50, 1 },
    { DtProperties145_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties145_56, 2 },
    { NULL, 0 },
    { DtProperties145_58, 2 },
    { DtProperties145_59, 3 },
    { DtProperties145_60, 1 },
    { DtProperties145_61, 4 },
    { NULL, 0 },
    { DtProperties145_63, 3 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-160 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// GigE + 3x ASI ports
// 
static const DtProperty  DtProperties160_00[] =
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
static const DtProperty  DtProperties160_01[] =
{
    { "FIRMWARE_LAST_VER", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1,
      "New LED patterns to clearly indentify in- (=green) and outputs (=orange)",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Support transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_02[] =
{
    { "FIRMWARE_R_N1_2", -1, "Bug fix: carrier-extension was seen as an error",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_1", -1, "Improved configurable IO support",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_RT_TX_DMA", 0xC80, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_03[] =
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
static const DtProperty  DtProperties160_04[] =
{
    { "FIRMWARE_R_N2_3", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x500, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_07[] =
{
    { "CAP_SDITIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_08[] =
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
static const DtProperty  DtProperties160_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_10[] =
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
static const DtProperty  DtProperties160_14[] =
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
static const DtProperty  DtProperties160_16[] =
{
    { "NW_REGISTERS_NRT_TX_DMA", 0xD80, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_17[] =
{
    { "CAP_IP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_MAC", 0x900, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "NW_REGISTERS_RT_TX", 0xC00, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_19[] =
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
static const DtProperty  DtProperties160_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_22[] =
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
static const DtProperty  DtProperties160_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_25[] =
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
static const DtProperty  DtProperties160_26[] =
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
static const DtProperty  DtProperties160_29[] =
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
static const DtProperty  DtProperties160_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_NRT_TX", 0xD00, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_32[] =
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
static const DtProperty  DtProperties160_35[] =
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
static const DtProperty  DtProperties160_39[] =
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
static const DtProperty  DtProperties160_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_PLX, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS16, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PLX_DMA_CHAN_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_46[] =
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
static const DtProperty  DtProperties160_50[] =
{
    { "PORT_COUNT", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_56[] =
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
static const DtProperty  DtProperties160_59[] =
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
static const DtProperty  DtProperties160_60[] =
{
    { "PCI_DEVICE_ID", 0x9056, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties160_63[] =
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
static const DtPropertyHashSet  DtPropertyHashSets160[] =
{
    { DtProperties160_00, 5 },
    { DtProperties160_01, 3 },
    { DtProperties160_02, 3 },
    { DtProperties160_03, 3 },
    { DtProperties160_04, 2 },
    { NULL, 0 },
    { DtProperties160_06, 3 },
    { DtProperties160_07, 3 },
    { DtProperties160_08, 12 },
    { DtProperties160_09, 3 },
    { DtProperties160_10, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties160_14, 5 },
    { NULL, 0 },
    { DtProperties160_16, 2 },
    { DtProperties160_17, 3 },
    { NULL, 0 },
    { DtProperties160_19, 4 },
    { NULL, 0 },
    { DtProperties160_21, 3 },
    { DtProperties160_22, 5 },
    { DtProperties160_23, 3 },
    { NULL, 0 },
    { DtProperties160_25, 4 },
    { DtProperties160_26, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties160_29, 4 },
    { DtProperties160_30, 1 },
    { DtProperties160_31, 5 },
    { DtProperties160_32, 6 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties160_35, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties160_39, 5 },
    { DtProperties160_40, 2 },
    { NULL, 0 },
    { DtProperties160_42, 1 },
    { DtProperties160_43, 2 },
    { DtProperties160_44, 1 },
    { NULL, 0 },
    { DtProperties160_46, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties160_50, 1 },
    { DtProperties160_51, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties160_56, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties160_59, 8 },
    { DtProperties160_60, 1 },
    { DtProperties160_61, 6 },
    { NULL, 0 },
    { DtProperties160_63, 5 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2107 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Multi-standard L-band modulator
// 
static const DtProperty  DtProperties2107_00[] =
{
    { "FIRMWARE_R_N1_0", -1,
      "Bug fix:  incorrect frequency of the on-board reference clock",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_01[] =
{
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1, "Bug fix:  no null packets are sent on stop",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 1, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_02[] =
{
    { "MOD_HW_RFPLL", 2107, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_03[] =
{
    { "MOD_HW_HAS_AD9789", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_04[] =
{
    { "CAP_TX_DVBS2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 2107, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_08[] =
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
static const DtProperty  DtProperties2107_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_11[] =
{
    { "CAP_SNR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties2107_12[] =
{
    { "CAP_ROLLOFF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2107_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "CAP_TX_DVBS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2107_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2107_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_ISDBS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2107_25[] =
{
    { "CAP_TX_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 210, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_26[] =
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
static const DtProperty  DtProperties2107_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", 200000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_33[] =
{
    { "MOD_LEVEL_MAX", -100, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 88000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 88000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 55000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 50000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2107_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 700, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MIN", 88000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_44[] =
{
    { "CAP_S2APSK", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_45[] =
{
    { "MOD_FREQ_ABSMAX", 2300, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 50000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_46[] =
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
static const DtProperty  DtProperties2107_47[] =
{
    { "MOD_HW_SAMP_PACKING", 12, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_51[] =
{
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties2107_53[] =
{
    { "CAP_ADJLVL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 950, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_55[] =
{
    { "MOD_FREQ_MAX", 2150, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 88000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_57[] =
{
    { "CAP_LBAND", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2107_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 55000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_59[] =
{
    { "CAP_RFCLKEXT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SYMIQ_MAPPING", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Multi-standard L-band modulator", PROPERTY_VALUE_TYPE_STRING, -1, 0,
                                                       0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_60[] =
{
    { "PCI_DEVICE_ID", 0x83B, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2107_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2107[] =
{
    { DtProperties2107_00, 1 },
    { DtProperties2107_01, 3 },
    { DtProperties2107_02, 1 },
    { DtProperties2107_03, 1 },
    { DtProperties2107_04, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2107_08, 7 },
    { DtProperties2107_09, 1 },
    { DtProperties2107_10, 2 },
    { DtProperties2107_11, 1 },
    { DtProperties2107_12, 1 },
    { NULL, 0 },
    { DtProperties2107_14, 2 },
    { NULL, 0 },
    { DtProperties2107_16, 1 },
    { NULL, 0 },
    { DtProperties2107_18, 2 },
    { DtProperties2107_19, 1 },
    { DtProperties2107_20, 1 },
    { NULL, 0 },
    { DtProperties2107_22, 2 },
    { NULL, 0 },
    { DtProperties2107_24, 2 },
    { DtProperties2107_25, 3 },
    { DtProperties2107_26, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2107_29, 2 },
    { DtProperties2107_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2107_33, 2 },
    { DtProperties2107_34, 1 },
    { DtProperties2107_35, 3 },
    { DtProperties2107_36, 1 },
    { DtProperties2107_37, 1 },
    { NULL, 0 },
    { DtProperties2107_39, 1 },
    { DtProperties2107_40, 2 },
    { NULL, 0 },
    { DtProperties2107_42, 2 },
    { DtProperties2107_43, 3 },
    { DtProperties2107_44, 2 },
    { DtProperties2107_45, 2 },
    { DtProperties2107_46, 4 },
    { DtProperties2107_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2107_50, 1 },
    { DtProperties2107_51, 1 },
    { NULL, 0 },
    { DtProperties2107_53, 3 },
    { NULL, 0 },
    { DtProperties2107_55, 1 },
    { DtProperties2107_56, 1 },
    { DtProperties2107_57, 1 },
    { DtProperties2107_58, 1 },
    { DtProperties2107_59, 3 },
    { DtProperties2107_60, 1 },
    { DtProperties2107_61, 1 },
    { NULL, 0 },
    { DtProperties2107_63, 2 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2111 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Multi-standard VHF/UHF modulator
// 
static const DtProperty  DtProperties2111_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "Fixes spontaneous lockup issue",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_01[] =
{
    { "FIRMWARE_LAST_VER", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Support for 16MHz bandwidth", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_02[] =
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
static const DtProperty  DtProperties2111_03[] =
{
    { "CAP_TX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_HAS_AD9789", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_04[] =
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
static const DtProperty  DtProperties2111_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2111_08[] =
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
static const DtProperty  DtProperties2111_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_14[] =
{
    { "CAP_TX_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties2111_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_TX_ISDBTMM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2111_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_SAMPRATE_HW_ABSMIN_TX_16MHZ", 4687500, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_25[] =
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
static const DtProperty  DtProperties2111_26[] =
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
static const DtProperty  DtProperties2111_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", 200000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_33[] =
{
    { "MOD_LEVEL_MAX", -90, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN_TX_16MHZ", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 4687500, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_35[] =
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
static const DtProperty  DtProperties2111_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 9375000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2111_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_41[] =
{
    { "CAP_TX_GOLD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2111_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_TX_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_43[] =
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
static const DtProperty  DtProperties2111_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_45[] =
{
    { "MOD_FREQ_ABSMAX", 1180, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 10937499, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX_TX_16MHZ", 21874998, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_47[] =
{
    { "MOD_HW_SAMP_PACKING", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_51[] =
{
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties2111_52[] =
{
    { "CAP_TX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2111_53[] =
{
    { "CAP_ADJLVL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_55[] =
{
    { "MOD_FREQ_MAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 4687500, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 9375000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_59[] =
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
static const DtProperty  DtProperties2111_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "PCI_DEVICE_ID", 0x83F, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2111_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2111[] =
{
    { DtProperties2111_00, 1 },
    { DtProperties2111_01, 3 },
    { DtProperties2111_02, 4 },
    { DtProperties2111_03, 3 },
    { DtProperties2111_04, 5 },
    { DtProperties2111_05, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2111_08, 6 },
    { DtProperties2111_09, 1 },
    { DtProperties2111_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2111_14, 3 },
    { NULL, 0 },
    { DtProperties2111_16, 1 },
    { NULL, 0 },
    { DtProperties2111_18, 1 },
    { DtProperties2111_19, 2 },
    { DtProperties2111_20, 1 },
    { NULL, 0 },
    { DtProperties2111_22, 3 },
    { NULL, 0 },
    { DtProperties2111_24, 3 },
    { DtProperties2111_25, 4 },
    { DtProperties2111_26, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2111_29, 2 },
    { DtProperties2111_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2111_33, 3 },
    { DtProperties2111_34, 1 },
    { DtProperties2111_35, 4 },
    { DtProperties2111_36, 1 },
    { DtProperties2111_37, 1 },
    { NULL, 0 },
    { DtProperties2111_39, 1 },
    { DtProperties2111_40, 2 },
    { DtProperties2111_41, 1 },
    { DtProperties2111_42, 3 },
    { DtProperties2111_43, 4 },
    { DtProperties2111_44, 1 },
    { DtProperties2111_45, 3 },
    { DtProperties2111_46, 2 },
    { DtProperties2111_47, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2111_50, 1 },
    { DtProperties2111_51, 1 },
    { DtProperties2111_52, 1 },
    { DtProperties2111_53, 3 },
    { NULL, 0 },
    { DtProperties2111_55, 1 },
    { DtProperties2111_56, 1 },
    { NULL, 0 },
    { DtProperties2111_58, 1 },
    { DtProperties2111_59, 4 },
    { DtProperties2111_60, 2 },
    { DtProperties2111_61, 1 },
    { NULL, 0 },
    { DtProperties2111_63, 2 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2131 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Multi-standard VHF/UHF receiver
// 
static const DtProperty  DtProperties2131_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_SRC", 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2131_11[] =
{
    { "REGISTERS_RPM", 0x600, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_17[] =
{
    { "CAP_RX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2131_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 1 } 
};
static const DtProperty  DtProperties2131_23[] =
{
    { "CAP_RX_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2131_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_26[] =
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
static const DtProperty  DtProperties2131_27[] =
{
    { "CAP_RX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2131_28[] =
{
    { "CAP_RX_ADV", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2131_29[] =
{
    { "CAP_RX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", 100000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_35[] =
{
    { "CAP_RX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_36[] =
{
    { "REGISTERS_ADC", 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_39[] =
{
    { "CAP_RX_GOLD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "EEPROM_SIZE", 2048, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_40[] =
{
    { "CAP_RX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 5,
                                                                                   2, 2 },
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 42, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_45[] =
{
    { "MOD_FREQ_ABSMAX", 870, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_50[] =
{
    { "CAP_RX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_53[] =
{
    { "MOD_FREQ_MIN", 42, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_55[] =
{
    { "MOD_FREQ_MAX", 870, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_57[] =
{
    { "DEMOD_HW_HAS_I2C_RFPWR_MEAS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_59[] =
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
static const DtProperty  DtProperties2131_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "PCI_DEVICE_ID", 0x853, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2131_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2131[] =
{
    { DtProperties2131_00, 1 },
    { DtProperties2131_01, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_04, 2 },
    { NULL, 0 },
    { DtProperties2131_06, 1 },
    { NULL, 0 },
    { DtProperties2131_08, 3 },
    { NULL, 0 },
    { DtProperties2131_10, 1 },
    { DtProperties2131_11, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_14, 2 },
    { NULL, 0 },
    { DtProperties2131_16, 1 },
    { DtProperties2131_17, 1 },
    { NULL, 0 },
    { DtProperties2131_19, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_23, 1 },
    { NULL, 0 },
    { DtProperties2131_25, 2 },
    { DtProperties2131_26, 4 },
    { DtProperties2131_27, 1 },
    { DtProperties2131_28, 1 },
    { DtProperties2131_29, 2 },
    { DtProperties2131_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_35, 3 },
    { DtProperties2131_36, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_39, 2 },
    { DtProperties2131_40, 3 },
    { NULL, 0 },
    { DtProperties2131_42, 1 },
    { DtProperties2131_43, 2 },
    { DtProperties2131_44, 1 },
    { DtProperties2131_45, 1 },
    { DtProperties2131_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_50, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_53, 1 },
    { NULL, 0 },
    { DtProperties2131_55, 1 },
    { NULL, 0 },
    { DtProperties2131_57, 1 },
    { NULL, 0 },
    { DtProperties2131_59, 4 },
    { DtProperties2131_60, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2131_63, 1 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2135 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Dual DVB-T receiver
// 
static const DtProperty  DtProperties2135_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "Support transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_01[] =
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
static const DtProperty  DtProperties2135_02[] =
{
    { "FIRMWARE_R_N3_0", -1, "Support for 14-bit ADC on Rev 6 hardware",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x500, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_08[] =
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
static const DtProperty  DtProperties2135_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2135_14[] =
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
static const DtProperty  DtProperties2135_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_17[] =
{
    { "CAP_RX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 601, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_28[] =
{
    { "CAP_RX_ADV", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2135_29[] =
{
    { "CAP_RX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", 100000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_35[] =
{
    { "CAP_RX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_39[] =
{
    { "CAP_RX_GOLD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_42[] =
{
    { "DMA_LOCAL_ADDR_FIXED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_43[] =
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
static const DtProperty  DtProperties2135_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_45[] =
{
    { "MOD_FREQ_ABSMAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 860, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 860, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_46[] =
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
static const DtProperty  DtProperties2135_47[] =
{
    { "CAP_IFADC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_50[] =
{
    { "CAP_RX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "CAP_SHAREDANT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PORT_COUNT", 3, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_52[] =
{
    { "CAP_ANTPWR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_53[] =
{
    { "MOD_FREQ_MIN", 50, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 50, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 50, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_55[] =
{
    { "MOD_FREQ_MAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 860, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 860, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_57[] =
{
    { "DEMOD_HW_HAS_I2C_RFPWR_MEAS", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_59[] =
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
static const DtProperty  DtProperties2135_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_DEVICE_ID", 0x857, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2135_63[] =
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
static const DtPropertyHashSet  DtPropertyHashSets2135[] =
{
    { DtProperties2135_00, 1 },
    { DtProperties2135_01, 4 },
    { DtProperties2135_02, 1 },
    { NULL, 0 },
    { DtProperties2135_04, 1 },
    { NULL, 0 },
    { DtProperties2135_06, 3 },
    { NULL, 0 },
    { DtProperties2135_08, 7 },
    { NULL, 0 },
    { DtProperties2135_10, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2135_14, 4 },
    { NULL, 0 },
    { DtProperties2135_16, 1 },
    { DtProperties2135_17, 2 },
    { NULL, 0 },
    { DtProperties2135_19, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2135_25, 2 },
    { DtProperties2135_26, 3 },
    { NULL, 0 },
    { DtProperties2135_28, 1 },
    { DtProperties2135_29, 2 },
    { DtProperties2135_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2135_35, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2135_39, 2 },
    { DtProperties2135_40, 2 },
    { NULL, 0 },
    { DtProperties2135_42, 4 },
    { DtProperties2135_43, 4 },
    { DtProperties2135_44, 1 },
    { DtProperties2135_45, 3 },
    { DtProperties2135_46, 4 },
    { DtProperties2135_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2135_50, 3 },
    { NULL, 0 },
    { DtProperties2135_52, 3 },
    { DtProperties2135_53, 3 },
    { NULL, 0 },
    { DtProperties2135_55, 3 },
    { NULL, 0 },
    { DtProperties2135_57, 3 },
    { NULL, 0 },
    { DtProperties2135_59, 7 },
    { DtProperties2135_60, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2135_63, 4 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2136 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Dual QAM-A/B/C receiver
// 
static const DtProperty  DtProperties2136_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_02[] =
{
    { "CAP_RX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_03[] =
{
    { "CAP_RX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_08[] =
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
static const DtProperty  DtProperties2136_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 2,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2136_14[] =
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
static const DtProperty  DtProperties2136_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_29[] =
{
    { "HARDWARE_I2C_FREQ", 200000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_32[] =
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
static const DtProperty  DtProperties2136_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 54, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_45[] =
{
    { "MOD_FREQ_ABSMAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_46[] =
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
static const DtProperty  DtProperties2136_50[] =
{
    { "CAP_SHAREDANT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PORT_COUNT", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_53[] =
{
    { "MOD_FREQ_MIN", 54, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 54, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_55[] =
{
    { "MOD_FREQ_MAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1002, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_57[] =
{
    { "DEMOD_HW_HAS_I2C_RFPWR_MEAS", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_59[] =
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
static const DtProperty  DtProperties2136_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_DEVICE_ID", 0x858, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2136_63[] =
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
static const DtPropertyHashSet  DtPropertyHashSets2136[] =
{
    { DtProperties2136_00, 2 },
    { DtProperties2136_01, 5 },
    { DtProperties2136_02, 2 },
    { DtProperties2136_03, 2 },
    { DtProperties2136_04, 1 },
    { NULL, 0 },
    { DtProperties2136_06, 2 },
    { NULL, 0 },
    { DtProperties2136_08, 9 },
    { DtProperties2136_09, 2 },
    { DtProperties2136_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_14, 5 },
    { NULL, 0 },
    { DtProperties2136_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_19, 2 },
    { NULL, 0 },
    { DtProperties2136_21, 2 },
    { DtProperties2136_22, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_25, 2 },
    { DtProperties2136_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_29, 1 },
    { DtProperties2136_30, 1 },
    { DtProperties2136_31, 2 },
    { DtProperties2136_32, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_39, 1 },
    { DtProperties2136_40, 2 },
    { NULL, 0 },
    { DtProperties2136_42, 1 },
    { DtProperties2136_43, 3 },
    { DtProperties2136_44, 1 },
    { DtProperties2136_45, 2 },
    { DtProperties2136_46, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_50, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2136_53, 2 },
    { NULL, 0 },
    { DtProperties2136_55, 2 },
    { NULL, 0 },
    { DtProperties2136_57, 2 },
    { NULL, 0 },
    { DtProperties2136_59, 5 },
    { DtProperties2136_60, 3 },
    { DtProperties2136_61, 2 },
    { NULL, 0 },
    { DtProperties2136_63, 5 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2137 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Dual DVB-S/DVB-S2 receiver
// 
static const DtProperty  DtProperties2137_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Support for transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_01[] =
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
static const DtProperty  DtProperties2137_02[] =
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
static const DtProperty  DtProperties2137_03[] =
{
    { "FIRMWARE_R_N2_2", -1, "Support for filtering a specific TS to an ASI port",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N4_0", -1,
      "Fixes issue with extracting TS-packets from DVB-S2 baseband frames",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_04[] =
{
    { "FIRMWARE_R_N5_0", -1, "Bug fix: timing issue on demodulator input ports",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_05[] =
{
    { "FIRMWARE_R_N6_0", -1, "Support for latest DVB-S2 demodulator chip",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_06[] =
{
    { "FIRMWARE_R_N7_0", -1,
      "FrameId field has been removed from L.3 header in STL3 receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_07[] =
{
    { "FIRMWARE_R_N8_0", -1,
      "Bug fix: PLFrameId field from L.3 header counted incorrectly",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_08[] =
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
static const DtProperty  DtProperties2137_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N8_2", -1,
      "Bug fix: FIFO overflow flag was not set for some receive modes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_10[] =
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
static const DtProperty  DtProperties2137_11[] =
{
    { "FIRMWARE_R_N8_4", -1,
      "Bug fix: Certain PCs would detect the card as a graphics card",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_12[] =
{
    { "CAP_ROLLOFF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_14[] =
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
static const DtProperty  DtProperties2137_16[] =
{
    { "CAP_DISABLED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RX_DVBS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_22[] =
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
static const DtProperty  DtProperties2137_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 302, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_28[] =
{
    { "CAP_L3MODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 7, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 7, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", 200000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_32[] =
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
static const DtProperty  DtProperties2137_35[] =
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
static const DtProperty  DtProperties2137_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 950, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 950, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_44[] =
{
    { "CAP_S2APSK", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_45[] =
{
    { "MOD_FREQ_ABSMAX", 2150, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 2150, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_46[] =
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
static const DtProperty  DtProperties2137_50[] =
{
    { "CAP_SHAREDANT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PORT_COUNT", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_52[] =
{
    { "CAP_LNB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_53[] =
{
    { "MOD_FREQ_MIN", 950, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 950, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_54[] =
{
    { "CAP_SWS2APSK", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_55[] =
{
    { "MOD_FREQ_MAX", 2150, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 2150, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_57[] =
{
    { "CAP_LBAND", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEMOD_HW_HAS_I2C_RFPWR_MEAS", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_59[] =
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
static const DtProperty  DtProperties2137_60[] =
{
    { "PCI_DEVICE_ID", 0x859, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_62[] =
{
    { "CAP_LOOPS2TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2137_63[] =
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
static const DtPropertyHashSet  DtPropertyHashSets2137[] =
{
    { DtProperties2137_00, 3 },
    { DtProperties2137_01, 4 },
    { DtProperties2137_02, 5 },
    { DtProperties2137_03, 2 },
    { DtProperties2137_04, 2 },
    { DtProperties2137_05, 1 },
    { DtProperties2137_06, 3 },
    { DtProperties2137_07, 1 },
    { DtProperties2137_08, 10 },
    { DtProperties2137_09, 3 },
    { DtProperties2137_10, 4 },
    { DtProperties2137_11, 1 },
    { DtProperties2137_12, 2 },
    { NULL, 0 },
    { DtProperties2137_14, 5 },
    { NULL, 0 },
    { DtProperties2137_16, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2137_19, 2 },
    { NULL, 0 },
    { DtProperties2137_21, 2 },
    { DtProperties2137_22, 4 },
    { DtProperties2137_23, 2 },
    { NULL, 0 },
    { DtProperties2137_25, 2 },
    { DtProperties2137_26, 3 },
    { NULL, 0 },
    { DtProperties2137_28, 2 },
    { DtProperties2137_29, 3 },
    { DtProperties2137_30, 1 },
    { DtProperties2137_31, 2 },
    { DtProperties2137_32, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2137_35, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2137_39, 1 },
    { DtProperties2137_40, 2 },
    { NULL, 0 },
    { DtProperties2137_42, 1 },
    { DtProperties2137_43, 3 },
    { DtProperties2137_44, 2 },
    { DtProperties2137_45, 2 },
    { DtProperties2137_46, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2137_50, 2 },
    { NULL, 0 },
    { DtProperties2137_52, 2 },
    { DtProperties2137_53, 2 },
    { DtProperties2137_54, 1 },
    { DtProperties2137_55, 2 },
    { DtProperties2137_56, 2 },
    { DtProperties2137_57, 4 },
    { NULL, 0 },
    { DtProperties2137_59, 5 },
    { DtProperties2137_60, 1 },
    { DtProperties2137_61, 2 },
    { DtProperties2137_62, 2 },
    { DtProperties2137_63, 5 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2138 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DVB C2/T2 receiver
// 
static const DtProperty  DtProperties2138_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "Support for transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2138_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_17[] =
{
    { "CAP_RX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2138_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 101, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_29[] =
{
    { "HARDWARE_I2C_FREQ", 400000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_35[] =
{
    { "CAP_RX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_39[] =
{
    { "EEPROM_SIZE", 2048, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 46, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_45[] =
{
    { "MOD_FREQ_ABSMAX", 876, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_50[] =
{
    { "CAP_RX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_53[] =
{
    { "MOD_FREQ_MIN", 46, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_55[] =
{
    { "MOD_FREQ_MAX", 876, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_57[] =
{
    { "DEMOD_HW_HAS_I2C_RFPWR_MEAS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "DVB C2/T2 receiver", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "PCI_DEVICE_ID", 0x85A, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2138_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2138[] =
{
    { DtProperties2138_00, 1 },
    { DtProperties2138_01, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_04, 1 },
    { NULL, 0 },
    { DtProperties2138_06, 1 },
    { NULL, 0 },
    { DtProperties2138_08, 3 },
    { NULL, 0 },
    { DtProperties2138_10, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_14, 2 },
    { NULL, 0 },
    { DtProperties2138_16, 1 },
    { DtProperties2138_17, 1 },
    { NULL, 0 },
    { DtProperties2138_19, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_25, 2 },
    { DtProperties2138_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_29, 1 },
    { DtProperties2138_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_35, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_39, 1 },
    { DtProperties2138_40, 2 },
    { NULL, 0 },
    { DtProperties2138_42, 1 },
    { DtProperties2138_43, 2 },
    { DtProperties2138_44, 1 },
    { DtProperties2138_45, 1 },
    { DtProperties2138_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_50, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_53, 1 },
    { NULL, 0 },
    { DtProperties2138_55, 1 },
    { NULL, 0 },
    { DtProperties2138_57, 1 },
    { NULL, 0 },
    { DtProperties2138_59, 3 },
    { DtProperties2138_60, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2138_63, 2 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2139 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Twelve-channel QAM Receiver
// 
static const DtProperty  DtProperties2139_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "Support for transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_01[] =
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
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_02[] =
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
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_03[] =
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
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_06[] =
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
static const DtProperty  DtProperties2139_08[] =
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
static const DtProperty  DtProperties2139_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2139_14[] =
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
static const DtProperty  DtProperties2139_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_19[] =
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
static const DtProperty  DtProperties2139_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_26[] =
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
static const DtProperty  DtProperties2139_29[] =
{
    { "HARDWARE_I2C_FREQ", 200000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_39[] =
{
    { "EEPROM_SIZE", 2048, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_43[] =
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
static const DtProperty  DtProperties2139_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_45[] =
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
static const DtProperty  DtProperties2139_46[] =
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
static const DtProperty  DtProperties2139_50[] =
{
    { "PORT_COUNT", 12, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_53[] =
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
static const DtProperty  DtProperties2139_55[] =
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
static const DtProperty  DtProperties2139_57[] =
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
static const DtProperty  DtProperties2139_59[] =
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
static const DtProperty  DtProperties2139_60[] =
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
static const DtProperty  DtProperties2139_63[] =
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
static const DtPropertyHashSet  DtPropertyHashSets2139[] =
{
    { DtProperties2139_00, 1 },
    { DtProperties2139_01, 25 },
    { DtProperties2139_02, 12 },
    { DtProperties2139_03, 12 },
    { DtProperties2139_04, 1 },
    { NULL, 0 },
    { DtProperties2139_06, 12 },
    { NULL, 0 },
    { DtProperties2139_08, 25 },
    { NULL, 0 },
    { DtProperties2139_10, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_14, 13 },
    { NULL, 0 },
    { DtProperties2139_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_19, 12 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_25, 2 },
    { DtProperties2139_26, 15 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_29, 1 },
    { DtProperties2139_30, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_39, 1 },
    { DtProperties2139_40, 2 },
    { NULL, 0 },
    { DtProperties2139_42, 1 },
    { DtProperties2139_43, 13 },
    { DtProperties2139_44, 1 },
    { DtProperties2139_45, 12 },
    { DtProperties2139_46, 13 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_53, 12 },
    { NULL, 0 },
    { DtProperties2139_55, 12 },
    { NULL, 0 },
    { DtProperties2139_57, 12 },
    { NULL, 0 },
    { DtProperties2139_59, 37 },
    { DtProperties2139_60, 13 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2139_63, 13 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2142 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Multi-mode serial and parallel port
// 
static const DtProperty  DtProperties2142_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Support for transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_01[] =
{
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1, "Support for transmit on timestamp mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_02[] =
{
    { "FIRMWARE_R_N1_2", -1,
      "Support for locking rate of ASI output to the rate of an ASI input",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_03[] =
{
    { "CAP_SPI525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_3", -1,
      "Fixes issue; where certain PCs would detect the card as a graphics card",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_04[] =
{
    { "CAP_SPI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_07[] =
{
    { "CAP_SDITIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_08[] =
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
static const DtProperty  DtProperties2142_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_SPICLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2142_14[] =
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
static const DtProperty  DtProperties2142_15[] =
{
    { "CAP_SPI625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_SPICLKEXT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_SPILVDS2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_16[] =
{
    { "CAP_TXONTIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_26[] =
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
static const DtProperty  DtProperties2142_28[] =
{
    { "CAP_EXTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", 200000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_31[] =
{
    { "CAP_RAWASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DEFAULT_SPISTD", DT_IOCONFIG_SPILVDS1, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_36[] =
{
    { "CAP_SPISDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_37[] =
{
    { "CAP_SPIDVBMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_40[] =
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
static const DtProperty  DtProperties2142_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_46[] =
{
    { "HARDWARE_HAS_PQNCO", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_49[] =
{
    { "CAP_SPISER10B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Multi-mode serial and parallel port", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_60[] =
{
    { "PCI_DEVICE_ID", 0x85E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2142_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2142[] =
{
    { DtProperties2142_00, 2 },
    { DtProperties2142_01, 2 },
    { DtProperties2142_02, 1 },
    { DtProperties2142_03, 2 },
    { DtProperties2142_04, 2 },
    { NULL, 0 },
    { DtProperties2142_06, 2 },
    { DtProperties2142_07, 2 },
    { DtProperties2142_08, 9 },
    { DtProperties2142_09, 3 },
    { DtProperties2142_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2142_14, 5 },
    { DtProperties2142_15, 3 },
    { DtProperties2142_16, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2142_19, 2 },
    { NULL, 0 },
    { DtProperties2142_21, 1 },
    { DtProperties2142_22, 2 },
    { DtProperties2142_23, 1 },
    { NULL, 0 },
    { DtProperties2142_25, 2 },
    { DtProperties2142_26, 4 },
    { NULL, 0 },
    { DtProperties2142_28, 2 },
    { DtProperties2142_29, 3 },
    { DtProperties2142_30, 1 },
    { DtProperties2142_31, 1 },
    { DtProperties2142_32, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2142_35, 3 },
    { DtProperties2142_36, 1 },
    { DtProperties2142_37, 1 },
    { NULL, 0 },
    { DtProperties2142_39, 1 },
    { DtProperties2142_40, 4 },
    { NULL, 0 },
    { DtProperties2142_42, 1 },
    { DtProperties2142_43, 1 },
    { DtProperties2142_44, 1 },
    { NULL, 0 },
    { DtProperties2142_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2142_49, 1 },
    { DtProperties2142_50, 1 },
    { DtProperties2142_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2142_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2142_59, 3 },
    { DtProperties2142_60, 1 },
    { DtProperties2142_61, 2 },
    { NULL, 0 },
    { DtProperties2142_63, 3 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2144 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Quad ASI/SDI in+out
// 
static const DtProperty  DtProperties2144_00[] =
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
static const DtProperty  DtProperties2144_01[] =
{
    { "FIRMWARE_LAST_VER", 5, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Bug fix: FIFO-size setting is ignored",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_02[] =
{
    { "FIRMWARE_R_N2_1", -1,
      "Bug fix: Potential stale of dma engine resulting in a system lockup",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_0", -1, "Support for hardware revision 3",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_03[] =
{
    { "FIRMWARE_R_N4_0", -1, "Support for RAW-ASI mode", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_04[] =
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
static const DtProperty  DtProperties2144_05[] =
{
    { "FIRMWARE_R_N4_2", -1,
      "Fixes issue; where certain PCs would detect the card as a graphics card",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N5_1", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_06[] =
{
    { "FIRMWARE_R_N4_3", -1, "Support for 64-bit timestamping",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x500, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x700, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_07[] =
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
static const DtProperty  DtProperties2144_08[] =
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
static const DtProperty  DtProperties2144_09[] =
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
static const DtProperty  DtProperties2144_10[] =
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
static const DtProperty  DtProperties2144_14[] =
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
static const DtProperty  DtProperties2144_15[] =
{
    { "CAP_GENREF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_16[] =
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
static const DtProperty  DtProperties2144_19[] =
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
static const DtProperty  DtProperties2144_21[] =
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
static const DtProperty  DtProperties2144_22[] =
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
static const DtProperty  DtProperties2144_23[] =
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
static const DtProperty  DtProperties2144_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 300, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_29[] =
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
static const DtProperty  DtProperties2144_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_31[] =
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
static const DtProperty  DtProperties2144_32[] =
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
static const DtProperty  DtProperties2144_35[] =
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
static const DtProperty  DtProperties2144_37[] =
{
    { "GENLOCK_ARCH", 2144, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_39[] =
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
static const DtProperty  DtProperties2144_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 3, 200,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 200,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_SPI_EPCS16, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_46[] =
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
static const DtProperty  DtProperties2144_50[] =
{
    { "PORT_COUNT", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_51[] =
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
static const DtProperty  DtProperties2144_56[] =
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
static const DtProperty  DtProperties2144_59[] =
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
static const DtProperty  DtProperties2144_60[] =
{
    { "PCI_DEVICE_ID", 0x860, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2144_61[] =
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
static const DtProperty  DtProperties2144_63[] =
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
static const DtPropertyHashSet  DtPropertyHashSets2144[] =
{
    { DtProperties2144_00, 5 },
    { DtProperties2144_01, 2 },
    { DtProperties2144_02, 2 },
    { DtProperties2144_03, 1 },
    { DtProperties2144_04, 6 },
    { DtProperties2144_05, 2 },
    { DtProperties2144_06, 5 },
    { DtProperties2144_07, 4 },
    { DtProperties2144_08, 17 },
    { DtProperties2144_09, 5 },
    { DtProperties2144_10, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2144_14, 5 },
    { DtProperties2144_15, 1 },
    { DtProperties2144_16, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2144_19, 4 },
    { NULL, 0 },
    { DtProperties2144_21, 4 },
    { DtProperties2144_22, 8 },
    { DtProperties2144_23, 4 },
    { NULL, 0 },
    { DtProperties2144_25, 2 },
    { DtProperties2144_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2144_29, 5 },
    { DtProperties2144_30, 1 },
    { DtProperties2144_31, 8 },
    { DtProperties2144_32, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2144_35, 6 },
    { NULL, 0 },
    { DtProperties2144_37, 1 },
    { NULL, 0 },
    { DtProperties2144_39, 5 },
    { DtProperties2144_40, 2 },
    { NULL, 0 },
    { DtProperties2144_42, 3 },
    { DtProperties2144_43, 1 },
    { DtProperties2144_44, 1 },
    { NULL, 0 },
    { DtProperties2144_46, 9 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2144_50, 1 },
    { DtProperties2144_51, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2144_56, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2144_59, 9 },
    { DtProperties2144_60, 1 },
    { DtProperties2144_61, 8 },
    { NULL, 0 },
    { DtProperties2144_63, 5 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2145 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// ASI/SDI in+out with bypass relais
// 
static const DtProperty  DtProperties2145_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Support for reception/transmission of 10-bit SDI",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1, "Support for Huffman-coded SDI data",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Support for time-stamping of SDI frames",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_02[] =
{
    { "FIRMWARE_R_N2_1", -1, "Support transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_0", -1,
      "Fixes issue with DVB-ASI streams that have a bitrate >= 108Mbps",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_03[] =
{
    { "FIRMWARE_R_N2_2", -1, "Support for double-buffered mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N3_1", -1, "Support for SDI genlock", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N4_0", -1, "Intermediate version with some Genlock bugfixes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_04[] =
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
static const DtProperty  DtProperties2145_05[] =
{
    { "FIRMWARE_R_N2_4", -1,
      "Bug fix: Potential stale of dma engine resulting in a system lockup",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_06[] =
{
    { "FIRMWARE_R_N2_5", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_07[] =
{
    { "CAP_SDITIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_08[] =
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
static const DtProperty  DtProperties2145_09[] =
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
static const DtProperty  DtProperties2145_10[] =
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
static const DtProperty  DtProperties2145_11[] =
{
    { "FIRMWARE_R_N9_3", -1,
      "Fixes issue; where certain PCs would detect the card as a graphics card",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_12[] =
{
    { "FIRMWARE_R_N9_4", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_14[] =
{
    { "DMA_SUPPORTS_64BIT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_15[] =
{
    { "CAP_GENREF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_16[] =
{
    { "CAP_TXONTIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_19[] =
{
    { "CAP_FAILSAFE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 21, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_31[] =
{
    { "CAP_RAWASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_32[] =
{
    { "CAP_LOOPTHR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_35[] =
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
static const DtProperty  DtProperties2145_37[] =
{
    { "GENLOCK_ARCH", 145, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_39[] =
{
    { "CAP_DBLBUF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_SPI_EPCS4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_58[] =
{
    { "CAP_LOCK2INP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "ASI/SDI in+out with bypass relais", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_60[] =
{
    { "PCI_DEVICE_ID", 0x861, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 6, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 6, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2145_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2145[] =
{
    { DtProperties2145_00, 3 },
    { DtProperties2145_01, 3 },
    { DtProperties2145_02, 2 },
    { DtProperties2145_03, 3 },
    { DtProperties2145_04, 4 },
    { DtProperties2145_05, 1 },
    { DtProperties2145_06, 2 },
    { DtProperties2145_07, 1 },
    { DtProperties2145_08, 7 },
    { DtProperties2145_09, 4 },
    { DtProperties2145_10, 4 },
    { DtProperties2145_11, 1 },
    { DtProperties2145_12, 1 },
    { NULL, 0 },
    { DtProperties2145_14, 3 },
    { DtProperties2145_15, 1 },
    { DtProperties2145_16, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2145_19, 2 },
    { NULL, 0 },
    { DtProperties2145_21, 2 },
    { DtProperties2145_22, 2 },
    { DtProperties2145_23, 2 },
    { NULL, 0 },
    { DtProperties2145_25, 2 },
    { DtProperties2145_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2145_29, 3 },
    { DtProperties2145_30, 1 },
    { DtProperties2145_31, 4 },
    { DtProperties2145_32, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2145_35, 4 },
    { NULL, 0 },
    { DtProperties2145_37, 1 },
    { NULL, 0 },
    { DtProperties2145_39, 2 },
    { DtProperties2145_40, 2 },
    { NULL, 0 },
    { DtProperties2145_42, 1 },
    { DtProperties2145_43, 1 },
    { DtProperties2145_44, 1 },
    { NULL, 0 },
    { DtProperties2145_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2145_50, 1 },
    { DtProperties2145_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2145_56, 2 },
    { NULL, 0 },
    { DtProperties2145_58, 2 },
    { DtProperties2145_59, 3 },
    { DtProperties2145_60, 1 },
    { DtProperties2145_61, 4 },
    { NULL, 0 },
    { DtProperties2145_63, 3 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2152 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Dual HD-SDI in+out
// 
static const DtProperty  DtProperties2152_01[] =
{
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_04[] =
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
static const DtProperty  DtProperties2152_06[] =
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
static const DtProperty  DtProperties2152_07[] =
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
static const DtProperty  DtProperties2152_08[] =
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
static const DtProperty  DtProperties2152_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "GENLOCK_ASYNC_PORT", 3, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_GS2962, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0,
                                                      0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_SER_ITF_GS2962, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2152_11[] =
{
    { "CAP_1080P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "REGISTERS_DMA", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_15[] =
{
    { "CAP_GENREF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_16[] =
{
    { "CAP_1080I60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_23[] =
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
static const DtProperty  DtProperties2152_24[] =
{
    { "CAP_1080P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_25[] =
{
    { "CAP_FRACMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "INTERRUPT_PER_INT_CLK_BIT", 20, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_26[] =
{
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_29[] =
{
    { "HARDWARE_I2C_FREQ", 400000, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_35[] =
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
static const DtProperty  DtProperties2152_36[] =
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
static const DtProperty  DtProperties2152_37[] =
{
    { "GENLOCK_ARCH", 2152, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_38[] =
{
    { "CAP_720P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_45[] =
{
    { "CAP_MATRIX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_47[] =
{
    { "CAP_1080I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_50[] =
{
    { "PORT_COUNT", 3, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_GS2961, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_GS2961, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MATRIX_RAM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_52[] =
{
    { "CAP_1080P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_54[] =
{
    { "CAP_720P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_59[] =
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
static const DtProperty  DtProperties2152_60[] =
{
    { "PCI_DEVICE_ID", 0x868, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2152_63[] =
{
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2152[] =
{
    { NULL, 0 },
    { DtProperties2152_01, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2152_04, 6 },
    { NULL, 0 },
    { DtProperties2152_06, 5 },
    { DtProperties2152_07, 6 },
    { DtProperties2152_08, 10 },
    { DtProperties2152_09, 3 },
    { DtProperties2152_10, 3 },
    { DtProperties2152_11, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2152_14, 3 },
    { DtProperties2152_15, 3 },
    { DtProperties2152_16, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2152_23, 6 },
    { DtProperties2152_24, 3 },
    { DtProperties2152_25, 3 },
    { DtProperties2152_26, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2152_29, 1 },
    { DtProperties2152_30, 1 },
    { NULL, 0 },
    { DtProperties2152_32, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2152_35, 5 },
    { DtProperties2152_36, 6 },
    { DtProperties2152_37, 1 },
    { DtProperties2152_38, 3 },
    { DtProperties2152_39, 1 },
    { DtProperties2152_40, 2 },
    { NULL, 0 },
    { DtProperties2152_42, 1 },
    { DtProperties2152_43, 1 },
    { DtProperties2152_44, 1 },
    { DtProperties2152_45, 2 },
    { DtProperties2152_46, 3 },
    { DtProperties2152_47, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2152_50, 1 },
    { DtProperties2152_51, 3 },
    { DtProperties2152_52, 3 },
    { NULL, 0 },
    { DtProperties2152_54, 3 },
    { NULL, 0 },
    { DtProperties2152_56, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2152_59, 9 },
    { DtProperties2152_60, 1 },
    { DtProperties2152_61, 2 },
    { NULL, 0 },
    { DtProperties2152_63, 1 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2160 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// GigE + 3x ASI ports
// 
static const DtProperty  DtProperties2160_00[] =
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
static const DtProperty  DtProperties2160_01[] =
{
    { "FIRMWARE_LAST_VER", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1,
      "Fixes issue; where certain PCs would detect the card as a graphics card",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Fixes issue; no null packets are sent on stop",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_02[] =
{
    { "FIRMWARE_R_N2_1", -1, "Several other minor bug fixes", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_RT_TX_DMA", 0xC80, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x300, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x500, NULL, PROPERTY_VALUE_TYPE_UINT16, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_07[] =
{
    { "CAP_SDITIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_08[] =
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
static const DtProperty  DtProperties2160_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_10[] =
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
static const DtProperty  DtProperties2160_14[] =
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
static const DtProperty  DtProperties2160_16[] =
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
static const DtProperty  DtProperties2160_17[] =
{
    { "CAP_IP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_MAC", 0x900, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "NW_REGISTERS_RT_TX", 0xC00, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_19[] =
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
static const DtProperty  DtProperties2160_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_22[] =
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
static const DtProperty  DtProperties2160_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 20, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 100, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_26[] =
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
static const DtProperty  DtProperties2160_29[] =
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
static const DtProperty  DtProperties2160_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_NRT_TX", 0xD00, NULL, PROPERTY_VALUE_TYPE_UINT16, 3, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_32[] =
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
static const DtProperty  DtProperties2160_35[] =
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
static const DtProperty  DtProperties2160_39[] =
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
static const DtProperty  DtProperties2160_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_43[] =
{
    { "HARDWARE_HAS_I2C", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_46[] =
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
static const DtProperty  DtProperties2160_50[] =
{
    { "PORT_COUNT", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, ASI_SDI_DESER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 2, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_59[] =
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
static const DtProperty  DtProperties2160_60[] =
{
    { "PCI_DEVICE_ID", 0x870, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 1, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2160_63[] =
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
static const DtPropertyHashSet  DtPropertyHashSets2160[] =
{
    { DtProperties2160_00, 5 },
    { DtProperties2160_01, 3 },
    { DtProperties2160_02, 2 },
    { NULL, 0 },
    { DtProperties2160_04, 1 },
    { NULL, 0 },
    { DtProperties2160_06, 3 },
    { DtProperties2160_07, 3 },
    { DtProperties2160_08, 14 },
    { DtProperties2160_09, 3 },
    { DtProperties2160_10, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2160_14, 4 },
    { NULL, 0 },
    { DtProperties2160_16, 5 },
    { DtProperties2160_17, 3 },
    { NULL, 0 },
    { DtProperties2160_19, 4 },
    { NULL, 0 },
    { DtProperties2160_21, 3 },
    { DtProperties2160_22, 4 },
    { DtProperties2160_23, 3 },
    { NULL, 0 },
    { DtProperties2160_25, 2 },
    { DtProperties2160_26, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2160_29, 4 },
    { DtProperties2160_30, 1 },
    { DtProperties2160_31, 5 },
    { DtProperties2160_32, 6 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2160_35, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2160_39, 5 },
    { DtProperties2160_40, 2 },
    { NULL, 0 },
    { DtProperties2160_42, 1 },
    { DtProperties2160_43, 1 },
    { DtProperties2160_44, 1 },
    { NULL, 0 },
    { DtProperties2160_46, 8 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2160_50, 1 },
    { DtProperties2160_51, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2160_56, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2160_59, 8 },
    { DtProperties2160_60, 1 },
    { DtProperties2160_61, 6 },
    { NULL, 0 },
    { DtProperties2160_63, 5 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2162 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Dual GigE ports
// 
static const DtProperty  DtProperties2162_00[] =
{
    { "NETWORK_MAC_TYPE", MAC_TYPE_ALTERA, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_RT_RX_DMA", 0xC80, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0xC80, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_01[] =
{
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_02[] =
{
    { "NW_REGISTERS_RT_TX_DMA", 0xD80, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0xD80, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_07[] =
{
    { "NW_REGISTERS_ADDR_MATCH", 0x480, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x880, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_08[] =
{
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_10[] =
{
    { "DMA_IN_FPGA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties2162_14[] =
{
    { "DMA_SUPPORTS_64BIT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "NW_REGISTERS_NRT_RX_DMA", 0x580, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x980, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_15[] =
{
    { "NW_REGISTERS_RT_RX", 0x700, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0xb00, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_16[] =
{
    { "NW_REGISTERS_NRT_TX_DMA", 0x680, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0xA80, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_17[] =
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
static const DtProperty  DtProperties2162_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_25[] =
{
    { "INTERRUPT_PER_INT_CLK_BIT", 20, NULL, PROPERTY_VALUE_TYPE_UINT8, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_LAST_REV", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_26[] =
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
static const DtProperty  DtProperties2162_29[] =
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
static const DtProperty  DtProperties2162_30[] =
{
    { "DMA_READ_UP_TO_LAST_BYTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "NW_REGISTERS_NRT_TX", 0x600, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0xA00, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_39[] =
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
static const DtProperty  DtProperties2162_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_SPI, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_44[] =
{
    { "INTERRUPT_HAS_PER_INT_ITV_REG", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_52[] =
{
    { "NW_REGISTERS_MEMCTRL", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x140, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2162_59[] =
{
    { "VPD_ID", -1, "Dual GigE ports", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_60[] =
{
    { "PCI_DEVICE_ID", 0x872, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2162_63[] =
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
static const DtPropertyHashSet  DtPropertyHashSets2162[] =
{
    { DtProperties2162_00, 3 },
    { DtProperties2162_01, 1 },
    { DtProperties2162_02, 2 },
    { NULL, 0 },
    { DtProperties2162_04, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_07, 2 },
    { DtProperties2162_08, 3 },
    { NULL, 0 },
    { DtProperties2162_10, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_14, 3 },
    { DtProperties2162_15, 2 },
    { DtProperties2162_16, 3 },
    { DtProperties2162_17, 6 },
    { NULL, 0 },
    { DtProperties2162_19, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_22, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_25, 2 },
    { DtProperties2162_26, 5 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_29, 5 },
    { DtProperties2162_30, 1 },
    { DtProperties2162_31, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_39, 4 },
    { DtProperties2162_40, 2 },
    { NULL, 0 },
    { DtProperties2162_42, 1 },
    { DtProperties2162_43, 1 },
    { DtProperties2162_44, 1 },
    { NULL, 0 },
    { DtProperties2162_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_50, 1 },
    { NULL, 0 },
    { DtProperties2162_52, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_59, 1 },
    { DtProperties2162_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2162_63, 5 }
};


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertyStores -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// References to hash table per device type.
//
const DtPropertyStore  DtPropertyStores[] =
{
    { 100, DtPropertyHashSets100, 64 },
    { 102, DtPropertyHashSets102, 64 },
    { 105, DtPropertyHashSets105, 64 },
    { 107, DtPropertyHashSets107, 64 },
    { 110, DtPropertyHashSets110, 64 },
    { 111, DtPropertyHashSets111, 64 },
    { 112, DtPropertyHashSets112, 64 },
    { 115, DtPropertyHashSets115, 64 },
    { 116, DtPropertyHashSets116, 64 },
    { 117, DtPropertyHashSets117, 64 },
    { 120, DtPropertyHashSets120, 64 },
    { 122, DtPropertyHashSets122, 64 },
    { 124, DtPropertyHashSets124, 64 },
    { 140, DtPropertyHashSets140, 64 },
    { 145, DtPropertyHashSets145, 64 },
    { 160, DtPropertyHashSets160, 64 },
    { 2107, DtPropertyHashSets2107, 64 },
    { 2111, DtPropertyHashSets2111, 64 },
    { 2131, DtPropertyHashSets2131, 64 },
    { 2135, DtPropertyHashSets2135, 64 },
    { 2136, DtPropertyHashSets2136, 64 },
    { 2137, DtPropertyHashSets2137, 64 },
    { 2138, DtPropertyHashSets2138, 64 },
    { 2139, DtPropertyHashSets2139, 64 },
    { 2142, DtPropertyHashSets2142, 64 },
    { 2144, DtPropertyHashSets2144, 64 },
    { 2145, DtPropertyHashSets2145, 64 },
    { 2152, DtPropertyHashSets2152, 64 },
    { 2160, DtPropertyHashSets2160, 64 },
    { 2162, DtPropertyHashSets2162, 64 }
};
const Int  DtPropertyStoreCount = 30;

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
