//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtuPropertyStore.c *#*#*#*#*#*#*#*#*# (C) 2013 DekTec
//
// CapParser - Property Store - Tables storing device properties
//
// THIS FILE IS GENERATED, DO NOT EDIT!!!

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#include "DtuIncludes.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-205; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// ASI/SDI output
// 
static const DtProperty  DtProperties205_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 500,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Bug fix: PC-reboot failure when unit is connected",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1, "Bug fix: errorneous status-LED indication",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Prepares the DTU-205 for SDI", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_02[] =
{
    { "FIRMWARE_R_N1_2", -1, "Performance improvements", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_04[] =
{
    { "FIRMWARE_R_N5_0", -1, "Support for transmission of 10-bit SDI",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_05[] =
{
    { "FIRMWARE_R_N5_1", -1, "Support for Huffman-coded SDI data",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N6_0", -1, "Internal maintenance update, no functional changes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_06[] =
{
    { "FIRMWARE_R_N5_2", -1,
      "Bug fix: potential occasional generation of unwanted extra bytes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N7_0", -1,
      "Bug fix: Not all bytes in tx-FIFO were reported in FIFO load",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_07[] =
{
    { "FIRMWARE_R_N8_0", -1, "Internal maintenance update, no functional changes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_08[] =
{
    { "FIRMWARE_R_N9_0", -1, "Internal maintenance update, no functional changes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_READ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "USB_DEVICE_ID", 0x205, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_25[] =
{
    { "PCB_LAST_REV", 501, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 500,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties205_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 6, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_59[] =
{
    { "VPD_ID", -1, "ASI/SDI output", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets205_0[] =
{
    { DtProperties205_0_00, 2 },
    { DtProperties205_0_01, 3 },
    { DtProperties205_0_02, 1 },
    { NULL, 0 },
    { DtProperties205_0_04, 3 },
    { DtProperties205_0_05, 2 },
    { DtProperties205_0_06, 2 },
    { DtProperties205_0_07, 1 },
    { DtProperties205_0_08, 4 },
    { DtProperties205_0_09, 2 },
    { DtProperties205_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_0_20, 1 },
    { DtProperties205_0_21, 1 },
    { DtProperties205_0_22, 1 },
    { DtProperties205_0_23, 1 },
    { NULL, 0 },
    { DtProperties205_0_25, 1 },
    { DtProperties205_0_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_0_29, 3 },
    { NULL, 0 },
    { DtProperties205_0_31, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_0_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_0_39, 1 },
    { DtProperties205_0_40, 4 },
    { NULL, 0 },
    { DtProperties205_0_42, 2 },
    { DtProperties205_0_43, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_0_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_0_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_0_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_0_59, 1 },
    { NULL, 0 },
    { DtProperties205_0_61, 1 },
    { NULL, 0 },
    { DtProperties205_0_63, 2 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-215; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// VHF/UHF modulator
// 
static const DtProperty  DtProperties215_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1,
      "Bug fix: Error in firmware which results in data duplication",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_01[] =
{
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 1, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_02[] =
{
    { "CAP_TX_CMMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_RFPLL", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_03[] =
{
    { "CAP_TX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "MOD_HW_HAS_AD9789", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_04[] =
{
    { "CAP_TX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 21111, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties215_0_08[] =
{
    { "CAP_CM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "CAP_TX_T2SPLP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_RANGE", 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_READ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_10[] =
{
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_DEVICE_ID", 0x20F, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_14[] =
{
    { "CAP_TX_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties215_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties215_0_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties215_0_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties215_0_25[] =
{
    { "CAP_TX_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "PCB_LAST_REV", 300, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_33[] =
{
    { "MOD_LEVEL_MAX", -150, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 4687500, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 10937499, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 9375000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_41[] =
{
    { "CAP_TX_GOLD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties215_0_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_TX_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 20, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MIN", 1000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 1180, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 10937499, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_47[] =
{
    { "MOD_HW_SAMP_PACKING", 10, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_51[] =
{
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties215_0_52[] =
{
    { "CAP_TX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_53[] =
{
    { "CAP_ADJLVL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_55[] =
{
    { "MOD_FREQ_MAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 4687500, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 9375000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "MOD_HW_HAS_SYMIQ_MAPPING", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "VHF/UHF modulator", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties215_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets215_0[] =
{
    { DtProperties215_0_00, 1 },
    { DtProperties215_0_01, 2 },
    { DtProperties215_0_02, 2 },
    { DtProperties215_0_03, 2 },
    { DtProperties215_0_04, 3 },
    { DtProperties215_0_05, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties215_0_08, 6 },
    { DtProperties215_0_09, 2 },
    { DtProperties215_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties215_0_14, 1 },
    { NULL, 0 },
    { DtProperties215_0_16, 1 },
    { NULL, 0 },
    { DtProperties215_0_18, 1 },
    { DtProperties215_0_19, 1 },
    { DtProperties215_0_20, 2 },
    { NULL, 0 },
    { DtProperties215_0_22, 2 },
    { NULL, 0 },
    { DtProperties215_0_24, 2 },
    { DtProperties215_0_25, 2 },
    { DtProperties215_0_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties215_0_29, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties215_0_33, 2 },
    { DtProperties215_0_34, 1 },
    { DtProperties215_0_35, 2 },
    { DtProperties215_0_36, 1 },
    { DtProperties215_0_37, 1 },
    { NULL, 0 },
    { DtProperties215_0_39, 1 },
    { DtProperties215_0_40, 3 },
    { DtProperties215_0_41, 1 },
    { DtProperties215_0_42, 3 },
    { DtProperties215_0_43, 3 },
    { NULL, 0 },
    { DtProperties215_0_45, 2 },
    { DtProperties215_0_46, 2 },
    { DtProperties215_0_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties215_0_50, 1 },
    { DtProperties215_0_51, 1 },
    { DtProperties215_0_52, 1 },
    { DtProperties215_0_53, 3 },
    { NULL, 0 },
    { DtProperties215_0_55, 1 },
    { DtProperties215_0_56, 2 },
    { NULL, 0 },
    { DtProperties215_0_58, 1 },
    { DtProperties215_0_59, 3 },
    { DtProperties215_0_60, 1 },
    { DtProperties215_0_61, 1 },
    { NULL, 0 },
    { DtProperties215_0_63, 2 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-225; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// ASI/SDI input
// 
static const DtProperty  DtProperties225_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "New driver interface; no functional changes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_02[] =
{
    { "FIRMWARE_R_N3_0", -1, "Bug fix: PC-reboot failure when unit is connected",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_03[] =
{
    { "FIRMWARE_R_N3_1", -1, "Bug fix: FIFO load can increase above FIFO size",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N4_0", -1, "Add support for receiving SDI in 8-bit mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_04[] =
{
    { "FIRMWARE_R_N5_0", -1, "Support for reception of 10-bit SDI",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_05[] =
{
    { "FIRMWARE_R_N5_1", -1, "Support for Huffman-coded SDI data",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N6_0", -1, "Internal maintenance update, no functional changes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_06[] =
{
    { "FIRMWARE_R_N5_2", -1,
      "Bug fix: potential occasional reception of unwanted extra bytes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N7_0", -1,
      "Bug fix: sometimes fails to lock-on to input signal after a channel reset",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_07[] =
{
    { "FIRMWARE_R_N7_1", -1,
      "Bug fix: 32nd bit of clock-reference-counter was stuck to zero",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N8_0", -1, "Support transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N8_1", -1,
      "Fixes issue with DVB-ASI streams that have a bitrate >= 108Mbps",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N9_0", -1,
      "Fixes issue with stability of arrival timestamps assigned to packets",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_10[] =
{
    { "USB_DEVICE_ID", 0x219, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 8, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_25[] =
{
    { "PCB_LAST_REV", 501, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 5, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 500,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 6, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 4, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "ASI/SDI input", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets225_0[] =
{
    { DtProperties225_0_00, 1 },
    { DtProperties225_0_01, 2 },
    { DtProperties225_0_02, 1 },
    { DtProperties225_0_03, 2 },
    { DtProperties225_0_04, 3 },
    { DtProperties225_0_05, 2 },
    { DtProperties225_0_06, 3 },
    { DtProperties225_0_07, 2 },
    { DtProperties225_0_08, 5 },
    { DtProperties225_0_09, 1 },
    { DtProperties225_0_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_0_19, 1 },
    { DtProperties225_0_20, 1 },
    { DtProperties225_0_21, 1 },
    { NULL, 0 },
    { DtProperties225_0_23, 1 },
    { NULL, 0 },
    { DtProperties225_0_25, 1 },
    { DtProperties225_0_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_0_29, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_0_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_0_39, 1 },
    { DtProperties225_0_40, 4 },
    { NULL, 0 },
    { DtProperties225_0_42, 2 },
    { DtProperties225_0_43, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_0_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_0_50, 1 },
    { DtProperties225_0_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_0_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_0_59, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_0_63, 1 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-234; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// VSB/QAM-B receiver
// 
static const DtProperty  DtProperties234_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_02[] =
{
    { "CAP_RX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties234_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_10[] =
{
    { "USB_DEVICE_ID", 0x222, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_12[] =
{
    { "CAP_RX_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties234_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_SENCORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_25[] =
{
    { "PCB_LAST_REV", 800, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 50, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_53[] =
{
    { "MOD_FREQ_MIN", 50, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_55[] =
{
    { "MOD_FREQ_MAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_56[] =
{
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "VSB/QAM-B receiver", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties234_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets234_0[] =
{
    { DtProperties234_0_00, 1 },
    { DtProperties234_0_01, 2 },
    { DtProperties234_0_02, 1 },
    { NULL, 0 },
    { DtProperties234_0_04, 1 },
    { NULL, 0 },
    { DtProperties234_0_06, 1 },
    { NULL, 0 },
    { DtProperties234_0_08, 3 },
    { DtProperties234_0_09, 1 },
    { DtProperties234_0_10, 2 },
    { NULL, 0 },
    { DtProperties234_0_12, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_0_20, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_0_25, 1 },
    { DtProperties234_0_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_0_29, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_0_35, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_0_39, 1 },
    { DtProperties234_0_40, 3 },
    { NULL, 0 },
    { DtProperties234_0_42, 1 },
    { DtProperties234_0_43, 2 },
    { NULL, 0 },
    { DtProperties234_0_45, 1 },
    { DtProperties234_0_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_0_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_0_53, 1 },
    { NULL, 0 },
    { DtProperties234_0_55, 1 },
    { DtProperties234_0_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_0_59, 3 },
    { DtProperties234_0_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_0_63, 1 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-235; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DVB-T measurement receiver
// 
static const DtProperty  DtProperties235_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1,
      "Bug fix: failure to acquire data-lock => cannot read transport-stream",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_10[] =
{
    { "USB_DEVICE_ID", 0x223, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_SENCORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_17[] =
{
    { "CAP_RX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties235_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_25[] =
{
    { "PCB_LAST_REV", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 50, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_53[] =
{
    { "MOD_FREQ_MIN", 50, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_55[] =
{
    { "MOD_FREQ_MAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_56[] =
{
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "DVB-T measurement receiver", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties235_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets235_0[] =
{
    { DtProperties235_0_00, 1 },
    { DtProperties235_0_01, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_04, 1 },
    { NULL, 0 },
    { DtProperties235_0_06, 1 },
    { NULL, 0 },
    { DtProperties235_0_08, 3 },
    { DtProperties235_0_09, 1 },
    { DtProperties235_0_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_16, 1 },
    { DtProperties235_0_17, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_20, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_25, 1 },
    { DtProperties235_0_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_29, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_35, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_39, 1 },
    { DtProperties235_0_40, 3 },
    { NULL, 0 },
    { DtProperties235_0_42, 1 },
    { DtProperties235_0_43, 2 },
    { NULL, 0 },
    { DtProperties235_0_45, 1 },
    { DtProperties235_0_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_53, 1 },
    { NULL, 0 },
    { DtProperties235_0_55, 1 },
    { DtProperties235_0_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_59, 3 },
    { DtProperties235_0_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_63, 2 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-236; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// QAM measurement receiver + ASI input
// 
static const DtProperty  DtProperties236_0_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_02[] =
{
    { "CAP_RX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "SUBTYPE1", 0xC80001, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_03[] =
{
    { "CAP_RX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties236_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_10[] =
{
    { "USB_DEVICE_ID", 0x224, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_12[] =
{
    { "CAP_RX_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties236_0_16[] =
{
    { "CAP_DISABLED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_SENCORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_25[] =
{
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_DISABLED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 44, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 200,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 44, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 200,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 885, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 1, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_53[] =
{
    { "MOD_FREQ_MIN", 44, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 200,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 44, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_55[] =
{
    { "MOD_FREQ_MAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 200,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 885, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_56[] =
{
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "QAM measurement receiver + ASI input", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0,
                                                                                      0 } 
};
static const DtProperty  DtProperties236_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets236_0[] =
{
    { NULL, 0 },
    { DtProperties236_0_01, 3 },
    { DtProperties236_0_02, 2 },
    { DtProperties236_0_03, 1 },
    { DtProperties236_0_04, 1 },
    { NULL, 0 },
    { DtProperties236_0_06, 2 },
    { NULL, 0 },
    { DtProperties236_0_08, 5 },
    { DtProperties236_0_09, 1 },
    { DtProperties236_0_10, 2 },
    { NULL, 0 },
    { DtProperties236_0_12, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_0_16, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_0_19, 2 },
    { DtProperties236_0_20, 1 },
    { DtProperties236_0_21, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_0_25, 1 },
    { DtProperties236_0_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_0_29, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_0_32, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_0_35, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_0_39, 1 },
    { DtProperties236_0_40, 3 },
    { NULL, 0 },
    { DtProperties236_0_42, 1 },
    { DtProperties236_0_43, 3 },
    { NULL, 0 },
    { DtProperties236_0_45, 2 },
    { DtProperties236_0_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_0_50, 1 },
    { DtProperties236_0_51, 1 },
    { NULL, 0 },
    { DtProperties236_0_53, 2 },
    { NULL, 0 },
    { DtProperties236_0_55, 2 },
    { DtProperties236_0_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_0_59, 4 },
    { DtProperties236_0_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_0_63, 3 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-245; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// ASI/SDI input+output
// 
static const DtProperty  DtProperties245_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_04[] =
{
    { "FIRMWARE_R_N5_0", -1, "Support for reception of 10-bit SDI",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_05[] =
{
    { "FIRMWARE_R_N5_1", -1, "Support for Huffman-coded SDI data",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N6_0", -1, "Internal maintenance update, no functional changes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_06[] =
{
    { "FIRMWARE_R_N5_2", -1,
      "Bug fix: potential reception/generation of unwanted extra bytes",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N7_0", -1,
      "Bug fix: sometimes fails to lock-on to input signal after a channel reset",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_07[] =
{
    { "FIRMWARE_R_N7_1", -1,
      "Bug fix: 32nd bit of clock-reference-counter was stuck to zero",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N8_0", -1, "Support transparent-packet receive mode",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N7_2", -1,
      "Bug fix: not all bytes in tx-FIFO were reported in FIFO load",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N8_1", -1,
      "Fixes issue with DVB-ASI streams that have a bitrate >= 108Mbps",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N9_0", -1,
      "Fixes issue with stability of arrival timestamps assigned to packets",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "USB_DEVICE_ID", 0x22D, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 8, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_25[] =
{
    { "PCB_LAST_REV", 501, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 5, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 5, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 500,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties245_0_32[] =
{
    { "CAP_LOOPTHR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 7, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 6, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "ASI/SDI input+output", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets245_0[] =
{
    { DtProperties245_0_00, 1 },
    { DtProperties245_0_01, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_04, 3 },
    { DtProperties245_0_05, 2 },
    { DtProperties245_0_06, 3 },
    { DtProperties245_0_07, 2 },
    { DtProperties245_0_08, 8 },
    { DtProperties245_0_09, 2 },
    { DtProperties245_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_19, 1 },
    { DtProperties245_0_20, 1 },
    { DtProperties245_0_21, 2 },
    { DtProperties245_0_22, 1 },
    { DtProperties245_0_23, 2 },
    { NULL, 0 },
    { DtProperties245_0_25, 1 },
    { DtProperties245_0_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_29, 4 },
    { NULL, 0 },
    { DtProperties245_0_31, 1 },
    { DtProperties245_0_32, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_35, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_39, 1 },
    { DtProperties245_0_40, 4 },
    { NULL, 0 },
    { DtProperties245_0_42, 2 },
    { DtProperties245_0_43, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_50, 1 },
    { DtProperties245_0_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_56, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_59, 2 },
    { NULL, 0 },
    { DtProperties245_0_61, 1 },
    { NULL, 0 },
    { DtProperties245_0_63, 3 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-255; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DTU-2X5 in manufacturing mode
// 
static const DtProperty  DtProperties255_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N9_0", -1, "See release notes for DTU-245", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1,
                                                   0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "USB_DEVICE_ID", 0x237, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_25[] =
{
    { "PCB_LAST_REV", 501, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 6, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "DTU-2X5 in manufacturing mode", PROPERTY_VALUE_TYPE_STRING, -1, 0,
                                                       0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets255_0[] =
{
    { DtProperties255_0_00, 1 },
    { DtProperties255_0_01, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_0_04, 1 },
    { NULL, 0 },
    { DtProperties255_0_06, 1 },
    { NULL, 0 },
    { DtProperties255_0_08, 6 },
    { DtProperties255_0_09, 2 },
    { DtProperties255_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_0_20, 1 },
    { DtProperties255_0_21, 2 },
    { DtProperties255_0_22, 1 },
    { DtProperties255_0_23, 2 },
    { NULL, 0 },
    { DtProperties255_0_25, 1 },
    { DtProperties255_0_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_0_29, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_0_35, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_0_39, 1 },
    { DtProperties255_0_40, 3 },
    { NULL, 0 },
    { DtProperties255_0_42, 2 },
    { DtProperties255_0_43, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_0_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_0_50, 1 },
    { DtProperties255_0_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_0_56, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_0_59, 2 },
    { NULL, 0 },
    { DtProperties255_0_61, 1 },
    { NULL, 0 },
    { DtProperties255_0_63, 2 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-299; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DTU-2X5 in manufacturing mode
// 
static const DtProperty  DtProperties299_0_01[] =
{
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_08[] =
{
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_10[] =
{
    { "USB_DEVICE_ID", 0x263, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_25[] =
{
    { "PCB_LAST_REV", 501, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 6, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_46[] =
{
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_50[] =
{
    { "PORT_COUNT", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_56[] =
{
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_59[] =
{
    { "VPD_ID", -1, "DTU-2X5 in manufacturing mode", PROPERTY_VALUE_TYPE_STRING, -1, 0,
                                                       0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_63[] =
{
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets299_0[] =
{
    { NULL, 0 },
    { DtProperties299_0_01, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_04, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_08, 1 },
    { DtProperties299_0_09, 1 },
    { DtProperties299_0_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_20, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_25, 1 },
    { DtProperties299_0_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_29, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_35, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_39, 1 },
    { DtProperties299_0_40, 3 },
    { NULL, 0 },
    { DtProperties299_0_42, 2 },
    { DtProperties299_0_43, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_46, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_59, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_63, 1 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-351; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// HD-SDI input
// 
static const DtProperty  DtProperties351_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1,
      "Serveral performance improvements fixing possible dropped frames issue",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_01[] =
{
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_04[] =
{
    { "CAP_HDSDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_06[] =
{
    { "CAP_1080P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_07[] =
{
    { "CAP_720P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_08[] =
{
    { "CAP_720P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_09[] =
{
    { "USB_END_POINT_READ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_10[] =
{
    { "USB_DEVICE_ID", 0x333, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_11[] =
{
    { "CAP_1080P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_16[] =
{
    { "CAP_1080I60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_23[] =
{
    { "CAP_1080P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_24[] =
{
    { "CAP_1080P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_25[] =
{
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_36[] =
{
    { "CAP_1080I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_38[] =
{
    { "CAP_720P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_39[] =
{
    { "EEPROM_SIZE", 4096, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_45[] =
{
    { "CAP_MATRIX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 } 
};
static const DtProperty  DtProperties351_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_47[] =
{
    { "CAP_1080I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_49[] =
{
    { "CAP_BW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                       PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_52[] =
{
    { "CAP_1080P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_54[] =
{
    { "CAP_720P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_WRITE", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_59[] =
{
    { "CAP_720P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "HD-SDI input", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_63[] =
{
    { "FIRMWARE_R_N0_0", -1, "See release notes for DTU-351", PROPERTY_VALUE_TYPE_STRING,
                                                -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets351_0[] =
{
    { DtProperties351_0_00, 1 },
    { DtProperties351_0_01, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties351_0_04, 2 },
    { NULL, 0 },
    { DtProperties351_0_06, 2 },
    { DtProperties351_0_07, 2 },
    { DtProperties351_0_08, 3 },
    { DtProperties351_0_09, 1 },
    { DtProperties351_0_10, 2 },
    { DtProperties351_0_11, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties351_0_16, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties351_0_20, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties351_0_23, 2 },
    { DtProperties351_0_24, 1 },
    { DtProperties351_0_25, 1 },
    { DtProperties351_0_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties351_0_29, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties351_0_35, 2 },
    { DtProperties351_0_36, 2 },
    { NULL, 0 },
    { DtProperties351_0_38, 1 },
    { DtProperties351_0_39, 1 },
    { DtProperties351_0_40, 3 },
    { NULL, 0 },
    { DtProperties351_0_42, 1 },
    { DtProperties351_0_43, 1 },
    { NULL, 0 },
    { DtProperties351_0_45, 1 },
    { DtProperties351_0_46, 2 },
    { DtProperties351_0_47, 1 },
    { NULL, 0 },
    { DtProperties351_0_49, 1 },
    { DtProperties351_0_50, 1 },
    { DtProperties351_0_51, 1 },
    { DtProperties351_0_52, 1 },
    { NULL, 0 },
    { DtProperties351_0_54, 1 },
    { NULL, 0 },
    { DtProperties351_0_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties351_0_59, 3 },
    { NULL, 0 },
    { DtProperties351_0_61, 1 },
    { NULL, 0 },
    { DtProperties351_0_63, 1 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-399; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DTU-3XX with uninitialized EEPROM
// 
static const DtProperty  DtProperties399_0_01[] =
{
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_08[] =
{
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_09[] =
{
    { "USB_END_POINT_READ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_10[] =
{
    { "USB_DEVICE_ID", 0xF3, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_39[] =
{
    { "EEPROM_SIZE", 4096, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x4B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_46[] =
{
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_50[] =
{
    { "PORT_COUNT", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_56[] =
{
    { "USB_END_POINT_WRITE", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_59[] =
{
    { "VPD_ID", -1, "DTU-3XX with uninitialized EEPROM", PROPERTY_VALUE_TYPE_STRING, -1,
                                                    0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_63[] =
{
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                    PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets399_0[] =
{
    { NULL, 0 },
    { DtProperties399_0_01, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_04, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_08, 1 },
    { DtProperties399_0_09, 1 },
    { DtProperties399_0_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_20, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_29, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_35, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_39, 1 },
    { DtProperties399_0_40, 3 },
    { NULL, 0 },
    { DtProperties399_0_42, 1 },
    { DtProperties399_0_43, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_46, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_59, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_63, 1 }
};


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertyStores -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// References to hash table per device type.
//
const DtPropertyStore  DtPropertyStores[] =
{
    { 205, 0, DtPropertyHashSets205_0, 64 },
    { 215, 0, DtPropertyHashSets215_0, 64 },
    { 225, 0, DtPropertyHashSets225_0, 64 },
    { 234, 0, DtPropertyHashSets234_0, 64 },
    { 235, 0, DtPropertyHashSets235_0, 64 },
    { 236, 0, DtPropertyHashSets236_0, 64 },
    { 245, 0, DtPropertyHashSets245_0, 64 },
    { 255, 0, DtPropertyHashSets255_0, 64 },
    { 299, 0, DtPropertyHashSets299_0, 64 },
    { 351, 0, DtPropertyHashSets351_0, 64 },
    { 399, 0, DtPropertyHashSets399_0, 64 }
};
const Int  DtPropertyStoreCount = 11;

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
