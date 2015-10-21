//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtuPropertyStore.c *#*#*#*#*#*#*#*#*# (C) 2015 DekTec
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
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 500, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_0", -1, "Bug fix: PC-reboot failure when unit is connected",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N1_1", -1, "Bug fix: errorneous status-LED indication",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "Prepares the DTU-205 for SDI", PROPERTY_VALUE_TYPE_STRING,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_02[] =
{
    { "FIRMWARE_R_N1_2", -1, "Performance improvements", PROPERTY_VALUE_TYPE_STRING, -1,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_04[] =
{
    { "FIRMWARE_R_N5_0", -1, "Support for transmission of 10-bit SDI",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 500, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_05[] =
{
    { "FIRMWARE_R_N5_1", -1, "Support for Huffman-coded SDI data",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N6_0", -1, "Internal maintenance update, no functional changes",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_06[] =
{
    { "FIRMWARE_R_N5_2", -1,
      "Bug fix: potential occasional generation of unwanted extra bytes",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N7_0", -1,
      "Bug fix: Not all bytes in tx-FIFO were reported in FIFO load",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_07[] =
{
    { "FIRMWARE_R_N8_0", -1, "Internal maintenance update, no functional changes",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_08[] =
{
    { "FIRMWARE_R_N9_0", -1, "Internal maintenance update, no functional changes",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_READ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_RO_SIZE", 256, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "USB_DEVICE_ID", 0x205, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_17[] =
{
    { "VPD_RW_SIZE", 255, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_25[] =
{
    { "PCB_LAST_REV", 501, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 2, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 500, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_36[] =
{
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 6,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 500, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_41[] =
{
    { "VPD_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 6, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 500, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 2, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_59[] =
{
    { "VPD_ID", -1, "ASI/SDI output", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { DtProperties205_0_09, 3 },
    { DtProperties205_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_0_16, 1 },
    { DtProperties205_0_17, 1 },
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
    { DtProperties205_0_36, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_0_39, 1 },
    { DtProperties205_0_40, 4 },
    { DtProperties205_0_41, 1 },
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
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_01[] =
{
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 1, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_02[] =
{
    { "CAP_TX_CMMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_RFPLL", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_03[] =
{
    { "CAP_TX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_AD9789", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_04[] =
{
    { "CAP_TX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 21111, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_08[] =
{
    { "CAP_CM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "CAP_TX_T2SPLP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_RANGE", 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_READ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_RO_SIZE", 256, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_10[] =
{
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_DEVICE_ID", 0x20F, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_14[] =
{
    { "CAP_TX_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_17[] =
{
    { "MOD_LEVEL_STEP_SIZE", 5, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_RW_SIZE", 255, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_ISDBTMM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_25[] =
{
    { "CAP_TX_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_LAST_REV", 300, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_28[] =
{
    { "REGISTERS_MOD", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_33[] =
{
    { "MOD_LEVEL_MAX", -150, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 4687500, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 10937499, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 9375000, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_41[] =
{
    { "CAP_TX_GOLD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 20, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MIN", 1000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 1180, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 10937499, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_47[] =
{
    { "MOD_HW_SAMP_PACKING", 10, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_51[] =
{
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_52[] =
{
    { "CAP_TX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RF", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_53[] =
{
    { "CAP_ADJLVL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_55[] =
{
    { "MOD_FREQ_MAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 4687500, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 9375000, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SYMIQ_MAPPING", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "VHF/UHF modulator", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { DtProperties215_0_09, 3 },
    { DtProperties215_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties215_0_14, 1 },
    { NULL, 0 },
    { DtProperties215_0_16, 1 },
    { DtProperties215_0_17, 2 },
    { DtProperties215_0_18, 1 },
    { DtProperties215_0_19, 2 },
    { DtProperties215_0_20, 2 },
    { NULL, 0 },
    { DtProperties215_0_22, 2 },
    { NULL, 0 },
    { DtProperties215_0_24, 2 },
    { DtProperties215_0_25, 2 },
    { DtProperties215_0_26, 1 },
    { NULL, 0 },
    { DtProperties215_0_28, 1 },
    { DtProperties215_0_29, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties215_0_33, 2 },
    { DtProperties215_0_34, 1 },
    { DtProperties215_0_35, 2 },
    { DtProperties215_0_36, 2 },
    { DtProperties215_0_37, 1 },
    { NULL, 0 },
    { DtProperties215_0_39, 1 },
    { DtProperties215_0_40, 3 },
    { DtProperties215_0_41, 2 },
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
    { DtProperties215_0_52, 2 },
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
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N2_0", -1, "New driver interface; no functional changes",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_02[] =
{
    { "FIRMWARE_R_N3_0", -1, "Bug fix: PC-reboot failure when unit is connected",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_03[] =
{
    { "FIRMWARE_R_N3_1", -1, "Bug fix: FIFO load can increase above FIFO size",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N4_0", -1, "Add support for receiving SDI in 8-bit mode",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_04[] =
{
    { "FIRMWARE_R_N5_0", -1, "Support for reception of 10-bit SDI",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 500, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_05[] =
{
    { "FIRMWARE_R_N5_1", -1, "Support for Huffman-coded SDI data",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N6_0", -1, "Internal maintenance update, no functional changes",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_06[] =
{
    { "FIRMWARE_R_N5_2", -1,
      "Bug fix: potential occasional reception of unwanted extra bytes",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N7_0", -1,
      "Bug fix: sometimes fails to lock-on to input signal after a channel reset",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_07[] =
{
    { "FIRMWARE_R_N7_1", -1,
      "Bug fix: 32nd bit of clock-reference-counter was stuck to zero",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N8_0", -1, "Support transparent-packet receive mode",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N8_1", -1,
      "Fixes issue with DVB-ASI streams that have a bitrate >= 108Mbps",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N9_0", -1,
      "Fixes issue with stability of arrival timestamps assigned to packets",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_RO_SIZE", 256, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_10[] =
{
    { "USB_DEVICE_ID", 0x219, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_17[] =
{
    { "VPD_RW_SIZE", 255, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 8, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_25[] =
{
    { "PCB_LAST_REV", 501, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 5, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 500, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_36[] =
{
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 6,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 500, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_41[] =
{
    { "VPD_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 6, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 500, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
               PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 4, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "ASI/SDI input", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
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
    { DtProperties225_0_09, 2 },
    { DtProperties225_0_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_0_16, 1 },
    { DtProperties225_0_17, 1 },
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
    { DtProperties225_0_36, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_0_39, 1 },
    { DtProperties225_0_40, 4 },
    { DtProperties225_0_41, 1 },
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
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_02[] =
{
    { "CAP_RX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_RO_SIZE", 256, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_10[] =
{
    { "USB_DEVICE_ID", 0x222, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_12[] =
{
    { "CAP_RX_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_SENCORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_17[] =
{
    { "VPD_RW_SIZE", 255, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_25[] =
{
    { "PCB_LAST_REV", 800, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_36[] =
{
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_41[] =
{
    { "VPD_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 50, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_53[] =
{
    { "MOD_FREQ_MIN", 50, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_55[] =
{
    { "MOD_FREQ_MAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_56[] =
{
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "VSB/QAM-B receiver", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties234_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
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
    { DtProperties234_0_09, 2 },
    { DtProperties234_0_10, 2 },
    { NULL, 0 },
    { DtProperties234_0_12, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_0_16, 1 },
    { DtProperties234_0_17, 1 },
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
    { DtProperties234_0_36, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_0_39, 1 },
    { DtProperties234_0_40, 3 },
    { DtProperties234_0_41, 1 },
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
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_RO_SIZE", 256, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_10[] =
{
    { "USB_DEVICE_ID", 0x223, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_SENCORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_17[] =
{
    { "CAP_RX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_RW_SIZE", 255, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_25[] =
{
    { "PCB_LAST_REV", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_36[] =
{
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_41[] =
{
    { "VPD_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 50, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_53[] =
{
    { "MOD_FREQ_MIN", 50, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_55[] =
{
    { "MOD_FREQ_MAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_56[] =
{
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "DVB-T measurement receiver", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { DtProperties235_0_09, 2 },
    { DtProperties235_0_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_16, 1 },
    { DtProperties235_0_17, 2 },
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
    { DtProperties235_0_36, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_39, 1 },
    { DtProperties235_0_40, 3 },
    { DtProperties235_0_41, 1 },
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
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_02[] =
{
    { "CAP_RX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBTYPE1", 0xC80001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_03[] =
{
    { "CAP_RX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_RO_SIZE", 256, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_10[] =
{
    { "USB_DEVICE_ID", 0x224, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_12[] =
{
    { "CAP_RX_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_16[] =
{
    { "CAP_DISABLED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_SENCORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_17[] =
{
    { "VPD_RW_SIZE", 255, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_25[] =
{
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_DISABLED, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_36[] =
{
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_41[] =
{
    { "VPD_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 44, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 200, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 44, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 200, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 885, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
               PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_53[] =
{
    { "MOD_FREQ_MIN", 44, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 200, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 44, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_55[] =
{
    { "MOD_FREQ_MAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 200, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 885, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_56[] =
{
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "VPD_ID", -1, "QAM measurement receiver + ASI input", PROPERTY_VALUE_TYPE_STRING,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties236_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { DtProperties236_0_09, 2 },
    { DtProperties236_0_10, 2 },
    { NULL, 0 },
    { DtProperties236_0_12, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_0_16, 3 },
    { DtProperties236_0_17, 1 },
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
    { DtProperties236_0_36, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_0_39, 1 },
    { DtProperties236_0_40, 3 },
    { DtProperties236_0_41, 1 },
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-238; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DVB-C2/T2 receiver measurement receiver + ASI input
// 
static const DtProperty  DtProperties238_0_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_RO_SIZE", 256, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_10[] =
{
    { "USB_DEVICE_ID", 0x226, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_16[] =
{
    { "CAP_DISABLED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_SENCORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_17[] =
{
    { "CAP_RX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_RW_SIZE", 255, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_23[] =
{
    { "CAP_RX_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_25[] =
{
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_DISABLED, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_35[] =
{
    { "CAP_RX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_36[] =
{
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_41[] =
{
    { "VPD_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 42, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_50[] =
{
    { "CAP_RX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
               PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_53[] =
{
    { "MOD_FREQ_MIN", 42, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_55[] =
{
    { "MOD_FREQ_MAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_56[] =
{
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "VPD_ID", -1, "DVB-C2/T2 receiver measurement receiver + ASI input",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties238_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets238_0[] =
{
    { NULL, 0 },
    { DtProperties238_0_01, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties238_0_04, 1 },
    { NULL, 0 },
    { DtProperties238_0_06, 2 },
    { NULL, 0 },
    { DtProperties238_0_08, 5 },
    { DtProperties238_0_09, 2 },
    { DtProperties238_0_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties238_0_16, 3 },
    { DtProperties238_0_17, 2 },
    { NULL, 0 },
    { DtProperties238_0_19, 2 },
    { DtProperties238_0_20, 1 },
    { DtProperties238_0_21, 1 },
    { NULL, 0 },
    { DtProperties238_0_23, 1 },
    { NULL, 0 },
    { DtProperties238_0_25, 1 },
    { DtProperties238_0_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties238_0_29, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties238_0_32, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties238_0_35, 2 },
    { DtProperties238_0_36, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties238_0_39, 1 },
    { DtProperties238_0_40, 3 },
    { DtProperties238_0_41, 1 },
    { DtProperties238_0_42, 1 },
    { DtProperties238_0_43, 2 },
    { NULL, 0 },
    { DtProperties238_0_45, 1 },
    { DtProperties238_0_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties238_0_50, 2 },
    { DtProperties238_0_51, 1 },
    { NULL, 0 },
    { DtProperties238_0_53, 1 },
    { NULL, 0 },
    { DtProperties238_0_55, 1 },
    { DtProperties238_0_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties238_0_59, 4 },
    { DtProperties238_0_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties238_0_63, 3 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-245; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// ASI/SDI input+output
// 
static const DtProperty  DtProperties245_0_00[] =
{
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_04[] =
{
    { "FIRMWARE_R_N5_0", -1, "Support for reception of 10-bit SDI",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 500, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_05[] =
{
    { "FIRMWARE_R_N5_1", -1, "Support for Huffman-coded SDI data",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N6_0", -1, "Internal maintenance update, no functional changes",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_06[] =
{
    { "FIRMWARE_R_N5_2", -1,
      "Bug fix: potential reception/generation of unwanted extra bytes",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N7_0", -1,
      "Bug fix: sometimes fails to lock-on to input signal after a channel reset",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_07[] =
{
    { "FIRMWARE_R_N7_1", -1,
      "Bug fix: 32nd bit of clock-reference-counter was stuck to zero",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N8_0", -1, "Support transparent-packet receive mode",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N7_2", -1,
      "Bug fix: not all bytes in tx-FIFO were reported in FIFO load",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N8_1", -1,
      "Fixes issue with DVB-ASI streams that have a bitrate >= 108Mbps",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N9_0", -1,
      "Fixes issue with stability of arrival timestamps assigned to packets",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_RO_SIZE", 256, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "USB_DEVICE_ID", 0x22D, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_17[] =
{
    { "VPD_RW_SIZE", 255, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 8, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_25[] =
{
    { "PCB_LAST_REV", 501, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 5, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 5, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 500, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_31[] =
{
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_32[] =
{
    { "CAP_LOOPTHR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 7, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_36[] =
{
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 6,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 500, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_41[] =
{
    { "VPD_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 6, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 500, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
               PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "ASI/SDI input+output", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { DtProperties245_0_09, 3 },
    { DtProperties245_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_16, 1 },
    { DtProperties245_0_17, 1 },
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
    { DtProperties245_0_36, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_39, 1 },
    { DtProperties245_0_40, 4 },
    { DtProperties245_0_41, 1 },
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
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N9_0", -1, "See release notes for DTU-245", PROPERTY_VALUE_TYPE_STRING,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_RO_SIZE", 256, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "USB_DEVICE_ID", 0x237, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_17[] =
{
    { "VPD_RW_SIZE", 255, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_25[] =
{
    { "PCB_LAST_REV", 501, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_36[] =
{
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 6,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_41[] =
{
    { "VPD_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 6, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
               PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_59[] =
{
    { "TESTING_LOOP_BACK_32B_RX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "DTU-2X5 in manufacturing mode", PROPERTY_VALUE_TYPE_STRING, -1, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
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
    { DtProperties255_0_09, 3 },
    { DtProperties255_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_0_16, 1 },
    { DtProperties255_0_17, 1 },
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
    { DtProperties255_0_36, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_0_39, 1 },
    { DtProperties255_0_40, 3 },
    { DtProperties255_0_41, 1 },
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
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_08[] =
{
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_RO_SIZE", 256, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_10[] =
{
    { "USB_DEVICE_ID", 0x263, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_17[] =
{
    { "VPD_RW_SIZE", 255, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_25[] =
{
    { "PCB_LAST_REV", 501, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_36[] =
{
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 6,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_41[] =
{
    { "VPD_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 6, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_46[] =
{
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_50[] =
{
    { "PORT_COUNT", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_56[] =
{
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_59[] =
{
    { "VPD_ID", -1, "DTU-2X5 in manufacturing mode", PROPERTY_VALUE_TYPE_STRING, -1, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_63[] =
{
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { DtProperties299_0_09, 2 },
    { DtProperties299_0_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_16, 1 },
    { DtProperties299_0_17, 1 },
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
    { DtProperties299_0_36, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_39, 1 },
    { DtProperties299_0_40, 3 },
    { DtProperties299_0_41, 1 },
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-315; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Universal modulator
// 
static const DtProperty  DtProperties315_0_01[] =
{
    { "CAP_TX_ATSC3", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_NUM_ATTN", 1, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_02[] =
{
    { "CAP_TX_CMMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_RFPLL", 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_03[] =
{
    { "CAP_LOWPWR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_AD9789", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_04[] =
{
    { "CAP_TX_DVBS2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SYM_SAMP_GEN", 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_05[] =
{
    { "CAP_TX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_07[] =
{
    { "DEFAULT_PWRMODE", DT_IOCONFIG_LOWPWR, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_08[] =
{
    { "CAP_CM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "CAP_TX_T2SPLP", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_LEVEL_RANGE", 200, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0xFFFF, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_READ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_RO_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_10[] =
{
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_DEVICE_ID", 0x30F, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_11[] =
{
    { "CAP_SNR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_12[] =
{
    { "CAP_ROLLOFF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_14[] =
{
    { "CAP_TX_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_17[] =
{
    { "MOD_LEVEL_STEP_SIZE", 5, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_RW_SIZE", 3071, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI,
                                                                                0, 0, 0 },
    { "CAP_TX_DVBS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_19[] =
{
    { "CAP_TX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_ISDBTMM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SEL_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_ISDBS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_25[] =
{
    { "CAP_TX_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_LAST_REV", 400, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_27[] =
{
    { "TEMP_SENS_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_28[] =
{
    { "CAP_TX_DVBS2X", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_MOD", 0xFFFF, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_29[] =
{
    { "CAP_TX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_33[] =
{
    { "MOD_LEVEL_MAX", -250, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMIN", 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_ABSMAX", 43750000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 43750000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 70000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 70000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 43750000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 43750000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 70000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 70000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_39[] =
{
    { "EEPROM_SIZE", 4096, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_41[] =
{
    { "CAP_TX_GOLD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_SIZE", 3584, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_42[] =
{
    { "CAP_TX_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_ABSMIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 36, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 36, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 36, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MIN", 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_44[] =
{
    { "CAP_S2APSK", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_45[] =
{
    { "MOD_FREQ_ABSMAX", 2000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 2000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 2250, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 2250, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_SRC_MAX", 43750000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 43750000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 70000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 70000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_47[] =
{
    { "MOD_HW_SAMP_PACKING", 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_49[] =
{
    { "CAP_MODHQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_51[] =
{
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_52[] =
{
    { "CAP_TX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RF", 0xFFFF, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "TEMP_SENS_TYPE", TEMP_SENS_TYPE_FX3, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_53[] =
{
    { "CAP_ADJLVL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_FREQ_MIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 36, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 36, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 36, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_55[] =
{
    { "MOD_FREQ_MAX", 2000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 2000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 2150, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 2150, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 80000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_WRITE", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_57[] =
{
    { "CAP_LBAND", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_58[] =
{
    { "MOD_SAMPRATE_HW_ABSMAX", 43750000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 43750000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 70000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 70000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_HAS_SYMIQ_MAPPING", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "Universal modulator", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets315_0[] =
{
    { NULL, 0 },
    { DtProperties315_0_01, 3 },
    { DtProperties315_0_02, 5 },
    { DtProperties315_0_03, 6 },
    { DtProperties315_0_04, 7 },
    { DtProperties315_0_05, 1 },
    { NULL, 0 },
    { DtProperties315_0_07, 1 },
    { DtProperties315_0_08, 6 },
    { DtProperties315_0_09, 3 },
    { DtProperties315_0_10, 6 },
    { DtProperties315_0_11, 1 },
    { DtProperties315_0_12, 1 },
    { NULL, 0 },
    { DtProperties315_0_14, 1 },
    { NULL, 0 },
    { DtProperties315_0_16, 1 },
    { DtProperties315_0_17, 2 },
    { DtProperties315_0_18, 2 },
    { DtProperties315_0_19, 2 },
    { DtProperties315_0_20, 2 },
    { NULL, 0 },
    { DtProperties315_0_22, 5 },
    { NULL, 0 },
    { DtProperties315_0_24, 3 },
    { DtProperties315_0_25, 2 },
    { DtProperties315_0_26, 1 },
    { DtProperties315_0_27, 1 },
    { DtProperties315_0_28, 2 },
    { DtProperties315_0_29, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties315_0_33, 5 },
    { DtProperties315_0_34, 4 },
    { DtProperties315_0_35, 5 },
    { DtProperties315_0_36, 5 },
    { DtProperties315_0_37, 1 },
    { NULL, 0 },
    { DtProperties315_0_39, 1 },
    { DtProperties315_0_40, 3 },
    { DtProperties315_0_41, 2 },
    { DtProperties315_0_42, 3 },
    { DtProperties315_0_43, 9 },
    { DtProperties315_0_44, 1 },
    { DtProperties315_0_45, 8 },
    { DtProperties315_0_46, 2 },
    { DtProperties315_0_47, 4 },
    { NULL, 0 },
    { DtProperties315_0_49, 1 },
    { DtProperties315_0_50, 5 },
    { DtProperties315_0_51, 1 },
    { DtProperties315_0_52, 3 },
    { DtProperties315_0_53, 6 },
    { NULL, 0 },
    { DtProperties315_0_55, 4 },
    { DtProperties315_0_56, 5 },
    { DtProperties315_0_57, 1 },
    { DtProperties315_0_58, 4 },
    { DtProperties315_0_59, 6 },
    { DtProperties315_0_60, 1 },
    { DtProperties315_0_61, 1 },
    { NULL, 0 },
    { DtProperties315_0_63, 2 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-350; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// HD-SDI output
// 
static const DtProperty  DtProperties350_0_01[] =
{
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_04[] =
{
    { "CAP_HDSDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_06[] =
{
    { "CAP_1080P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_07[] =
{
    { "CAP_720P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_08[] =
{
    { "CAP_720P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                 PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 10, 1 },
    { "USB_END_POINT_READ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_RO_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "USB_DEVICE_ID", 0x332, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_11[] =
{
    { "CAP_1080P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_16[] =
{
    { "CAP_1080I60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_17[] =
{
    { "VPD_RW_SIZE", 3583, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_23[] =
{
    { "CAP_1080P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_24[] =
{
    { "CAP_1080P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_25[] =
{
    { "PCB_LAST_REV", 100, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_36[] =
{
    { "CAP_1080I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_38[] =
{
    { "CAP_720P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_39[] =
{
    { "EEPROM_SIZE", 4096, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_41[] =
{
    { "VPD_SIZE", 4096, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_45[] =
{
    { "CAP_MATRIX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                 PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 10, 1 } 
};
static const DtProperty  DtProperties350_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_47[] =
{
    { "CAP_1080I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_52[] =
{
    { "CAP_1080P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_54[] =
{
    { "CAP_720P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_WRITE", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_59[] =
{
    { "CAP_720P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "HD-SDI output", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_63[] =
{
    { "FIRMWARE_R_N0_0", -1, "See release notes for DTU-350", PROPERTY_VALUE_TYPE_STRING,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets350_0[] =
{
    { NULL, 0 },
    { DtProperties350_0_01, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties350_0_04, 2 },
    { NULL, 0 },
    { DtProperties350_0_06, 1 },
    { DtProperties350_0_07, 2 },
    { DtProperties350_0_08, 3 },
    { DtProperties350_0_09, 3 },
    { DtProperties350_0_10, 3 },
    { DtProperties350_0_11, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties350_0_16, 2 },
    { DtProperties350_0_17, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties350_0_20, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties350_0_23, 2 },
    { DtProperties350_0_24, 1 },
    { DtProperties350_0_25, 1 },
    { DtProperties350_0_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties350_0_29, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties350_0_35, 2 },
    { DtProperties350_0_36, 3 },
    { NULL, 0 },
    { DtProperties350_0_38, 1 },
    { DtProperties350_0_39, 1 },
    { DtProperties350_0_40, 3 },
    { DtProperties350_0_41, 1 },
    { DtProperties350_0_42, 1 },
    { DtProperties350_0_43, 1 },
    { NULL, 0 },
    { DtProperties350_0_45, 1 },
    { DtProperties350_0_46, 2 },
    { DtProperties350_0_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties350_0_50, 1 },
    { NULL, 0 },
    { DtProperties350_0_52, 1 },
    { NULL, 0 },
    { DtProperties350_0_54, 1 },
    { NULL, 0 },
    { DtProperties350_0_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties350_0_59, 3 },
    { NULL, 0 },
    { DtProperties350_0_61, 1 },
    { NULL, 0 },
    { DtProperties350_0_63, 1 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-351; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// HD-SDI input
// 
static const DtProperty  DtProperties351_0_00[] =
{
    { "FIRMWARE_R_N1_0", -1,
      "Serveral performance improvements fixing possible dropped frames issue",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_01[] =
{
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_04[] =
{
    { "CAP_HDSDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_06[] =
{
    { "CAP_1080P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_07[] =
{
    { "CAP_720P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_08[] =
{
    { "CAP_720P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_09[] =
{
    { "USB_END_POINT_READ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_RO_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_10[] =
{
    { "USB_DEVICE_ID", 0x333, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_11[] =
{
    { "CAP_1080P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_16[] =
{
    { "CAP_1080I60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_17[] =
{
    { "CAP_1080PSF25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_RW_SIZE", 3583, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_23[] =
{
    { "CAP_1080P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_24[] =
{
    { "CAP_1080P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_25[] =
{
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_31[] =
{
    { "CAP_1080PSF23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_34[] =
{
    { "DEFAULT_BW", DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_36[] =
{
    { "CAP_1080I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_38[] =
{
    { "CAP_720P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_39[] =
{
    { "EEPROM_SIZE", 4096, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_41[] =
{
    { "VPD_SIZE", 4096, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_45[] =
{
    { "CAP_1080PSF30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_MATRIX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 2, 6 } 
};
static const DtProperty  DtProperties351_0_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_47[] =
{
    { "CAP_1080I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_49[] =
{
    { "CAP_BW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
               PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_52[] =
{
    { "CAP_1080P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_54[] =
{
    { "CAP_720P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_WRITE", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_59[] =
{
    { "CAP_720P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", -1, "HD-SDI input", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_63[] =
{
    { "CAP_MATRIX2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_R_N0_0", -1, "See release notes for DTU-351", PROPERTY_VALUE_TYPE_STRING,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { DtProperties351_0_09, 2 },
    { DtProperties351_0_10, 2 },
    { DtProperties351_0_11, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties351_0_16, 3 },
    { DtProperties351_0_17, 2 },
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
    { DtProperties351_0_31, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties351_0_34, 1 },
    { DtProperties351_0_35, 2 },
    { DtProperties351_0_36, 4 },
    { NULL, 0 },
    { DtProperties351_0_38, 1 },
    { DtProperties351_0_39, 1 },
    { DtProperties351_0_40, 3 },
    { DtProperties351_0_41, 1 },
    { DtProperties351_0_42, 1 },
    { DtProperties351_0_43, 1 },
    { NULL, 0 },
    { DtProperties351_0_45, 2 },
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
    { DtProperties351_0_63, 2 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-399; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// DTU-3XX with uninitialized EEPROM
// 
static const DtProperty  DtProperties399_0_01[] =
{
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_08[] =
{
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_09[] =
{
    { "USB_END_POINT_READ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_RO_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_10[] =
{
    { "USB_DEVICE_ID", 0xF3, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_FIRMWARE", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_17[] =
{
    { "VPD_RW_SIZE", 511, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_20[] =
{
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_36[] =
{
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_39[] =
{
    { "EEPROM_SIZE", 4096, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_VENDOR_ID", 0x4B4, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_41[] =
{
    { "VPD_SIZE", 4096, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_46[] =
{
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_50[] =
{
    { "PORT_COUNT", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_56[] =
{
    { "USB_END_POINT_WRITE", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_59[] =
{
    { "VPD_ID", -1, "DTU-3XX with uninitialized EEPROM", PROPERTY_VALUE_TYPE_STRING, -1,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_63[] =
{
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { DtProperties399_0_09, 2 },
    { DtProperties399_0_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_17, 1 },
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
    { DtProperties399_0_36, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_39, 1 },
    { DtProperties399_0_40, 3 },
    { DtProperties399_0_41, 1 },
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
    { 238, 0, DtPropertyHashSets238_0, 64 },
    { 245, 0, DtPropertyHashSets245_0, 64 },
    { 255, 0, DtPropertyHashSets255_0, 64 },
    { 299, 0, DtPropertyHashSets299_0, 64 },
    { 315, 0, DtPropertyHashSets315_0, 64 },
    { 350, 0, DtPropertyHashSets350_0, 64 },
    { 351, 0, DtPropertyHashSets351_0, 64 },
    { 399, 0, DtPropertyHashSets399_0, 64 }
};
const Int  DtPropertyStoreCount = 14;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IoConfigCodeStrings[] -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Array to translate from DT_IOCONFIG code to DT_IOCONFIG name
//
const char*  IoConfigCodeStrings[] =
{
/*  0 */ "IODIR",
/*  1 */ "IOSTD",
/*  2 */ "PWRMODE",
/*  3 */ "RFCLKSEL",
/*  4 */ "SPICLKSEL",
/*  5 */ "SPIMODE",
/*  6 */ "SPISTD",
/*  7 */ "TSRATESEL",
/*  8 */ "BW",
/*  9 */ "FAILSAFE",
/* 10 */ "FRACMODE",
/* 11 */ "GENLOCKED",
/* 12 */ "GENREF",
/* 13 */ "SWS2APSK",
/* 14 */ "TRUE",
/* 15 */ "FALSE",
/* 16 */ "DISABLED",
/* 17 */ "INPUT",
/* 18 */ "OUTPUT",
/* 19 */ "SHAREDANT",
/* 20 */ "DBLBUF",
/* 21 */ "LOOPS2L3",
/* 22 */ "LOOPS2TS",
/* 23 */ "LOOPTHR",
/* 24 */ "3GSDI",
/* 25 */ "ASI",
/* 26 */ "DEMOD",
/* 27 */ "GPSTIME",
/* 28 */ "HDSDI",
/* 29 */ "IFADC",
/* 30 */ "IP",
/* 31 */ "MOD",
/* 32 */ "PHASENOISE",
/* 33 */ "RS422",
/* 34 */ "SDI",
/* 35 */ "SPI",
/* 36 */ "SPISDI",
/* 37 */ "1080P50",
/* 38 */ "1080P50B",
/* 39 */ "1080P59_94",
/* 40 */ "1080P59_94B",
/* 41 */ "1080P60",
/* 42 */ "1080P60B",
/* 43 */ "1080I50",
/* 44 */ "1080I59_94",
/* 45 */ "1080I60",
/* 46 */ "1080P23_98",
/* 47 */ "1080P24",
/* 48 */ "1080P25",
/* 49 */ "1080P29_97",
/* 50 */ "1080P30",
/* 51 */ "1080PSF23_98",
/* 52 */ "1080PSF24",
/* 53 */ "1080PSF25",
/* 54 */ "1080PSF29_97",
/* 55 */ "1080PSF30",
/* 56 */ "720P23_98",
/* 57 */ "720P24",
/* 58 */ "720P25",
/* 59 */ "720P29_97",
/* 60 */ "720P30",
/* 61 */ "720P50",
/* 62 */ "720P59_94",
/* 63 */ "720P60",
/* 64 */ "525I59_94",
/* 65 */ "625I50",
/* 66 */ "SPI525I59_94",
/* 67 */ "SPI625I50",
/* 68 */ "MODHQ",
/* 69 */ "LOWPWR",
/* 70 */ "RFCLKEXT",
/* 71 */ "RFCLKINT",
/* 72 */ "SPICLKEXT",
/* 73 */ "SPICLKINT",
/* 74 */ "SPIFIXEDCLK",
/* 75 */ "SPIDVBMODE",
/* 76 */ "SPISER8B",
/* 77 */ "SPISER10B",
/* 78 */ "SPILVDS1",
/* 79 */ "SPILVDS2",
/* 80 */ "SPILVTTL",
/* 81 */ "EXTTSRATE",
/* 82 */ "EXTRATIO",
/* 83 */ "INTTSRATE",
/* 84 */ "LOCK2INP"
};
const Int  IoConfigCodeStringCount = 85;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IoConfigCodes_XX[] -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Helper arrays to map from DT_IOCONFIG name to DT_IOCONFIG code.
// The hash of the DT_IOCONFIG names in IoConfigCodes_XX[] is XX.
//
static const IoConfigCode  IoConfigCodes_0[] =
{
    { "FAILSAFE", 9 }
};
static const IoConfigCode  IoConfigCodes_1[] =
{
    { "1080P30", 50 }
};
static const IoConfigCode  IoConfigCodes_2[] =
{
    { "RS422", 33 }
};
static const IoConfigCode  IoConfigCodes_3[] =
{
    { "PWRMODE", 2 },
    { "LOOPS2L3", 21 },
    { "1080P50", 37 },
    { "720P23_98", 56 }
};
static const IoConfigCode  IoConfigCodes_5[] =
{
    { "SDI", 34 }
};
static const IoConfigCode  IoConfigCodes_6[] =
{
    { "FRACMODE", 10 }
};
static const IoConfigCode  IoConfigCodes_7[] =
{
    { "SPILVTTL", 80 }
};
static const IoConfigCode  IoConfigCodes_8[] =
{
    { "IOSTD", 1 },
    { "720P29_97", 59 },
    { "720P59_94", 62 }
};
static const IoConfigCode  IoConfigCodes_12[] =
{
    { "1080PSF23_98", 51 }
};
static const IoConfigCode  IoConfigCodes_14[] =
{
    { "DEMOD", 26 }
};
static const IoConfigCode  IoConfigCodes_16[] =
{
    { "FALSE", 15 },
    { "625I50", 65 }
};
static const IoConfigCode  IoConfigCodes_17[] =
{
    { "GENLOCKED", 11 },
    { "HDSDI", 28 },
    { "SPI", 35 },
    { "SPISDI", 36 },
    { "1080I59_94", 44 },
    { "1080PSF29_97", 54 },
    { "720P30", 60 }
};
static const IoConfigCode  IoConfigCodes_18[] =
{
    { "SPIDVBMODE", 75 }
};
static const IoConfigCode  IoConfigCodes_19[] =
{
    { "720P50", 61 }
};
static const IoConfigCode  IoConfigCodes_20[] =
{
    { "DBLBUF", 20 }
};
static const IoConfigCode  IoConfigCodes_21[] =
{
    { "INPUT", 17 },
    { "EXTRATIO", 82 }
};
static const IoConfigCode  IoConfigCodes_22[] =
{
    { "SPIMODE", 5 },
    { "SPICLKINT", 73 }
};
static const IoConfigCode  IoConfigCodes_27[] =
{
    { "SPIFIXEDCLK", 74 }
};
static const IoConfigCode  IoConfigCodes_28[] =
{
    { "IODIR", 0 },
    { "SPISTD", 6 },
    { "TSRATESEL", 7 },
    { "SPI625I50", 67 },
    { "SPICLKEXT", 72 }
};
static const IoConfigCode  IoConfigCodes_29[] =
{
    { "1080I60", 45 },
    { "1080PSF24", 52 }
};
static const IoConfigCode  IoConfigCodes_30[] =
{
    { "BW", 8 },
    { "GPSTIME", 27 },
    { "1080PSF25", 53 }
};
static const IoConfigCode  IoConfigCodes_31[] =
{
    { "3GSDI", 24 }
};
static const IoConfigCode  IoConfigCodes_34[] =
{
    { "ASI", 25 },
    { "RFCLKINT", 71 }
};
static const IoConfigCode  IoConfigCodes_35[] =
{
    { "SWS2APSK", 13 },
    { "INTTSRATE", 83 }
};
static const IoConfigCode  IoConfigCodes_36[] =
{
    { "1080P60", 41 },
    { "1080P24", 47 },
    { "525I59_94", 64 }
};
static const IoConfigCode  IoConfigCodes_37[] =
{
    { "TRUE", 14 },
    { "MOD", 31 },
    { "1080P50B", 38 },
    { "1080P25", 48 }
};
static const IoConfigCode  IoConfigCodes_38[] =
{
    { "1080P60B", 42 }
};
static const IoConfigCode  IoConfigCodes_39[] =
{
    { "LOCK2INP", 84 }
};
static const IoConfigCode  IoConfigCodes_40[] =
{
    { "RFCLKEXT", 70 }
};
static const IoConfigCode  IoConfigCodes_41[] =
{
    { "EXTTSRATE", 81 }
};
static const IoConfigCode  IoConfigCodes_43[] =
{
    { "LOOPS2TS", 22 }
};
static const IoConfigCode  IoConfigCodes_45[] =
{
    { "LOOPTHR", 23 }
};
static const IoConfigCode  IoConfigCodes_47[] =
{
    { "SPICLKSEL", 4 }
};
static const IoConfigCode  IoConfigCodes_48[] =
{
    { "SPI525I59_94", 66 },
    { "LOWPWR", 69 }
};
static const IoConfigCode  IoConfigCodes_51[] =
{
    { "1080P23_98", 46 }
};
static const IoConfigCode  IoConfigCodes_52[] =
{
    { "PHASENOISE", 32 },
    { "720P24", 57 },
    { "720P60", 63 }
};
static const IoConfigCode  IoConfigCodes_53[] =
{
    { "720P25", 58 },
    { "SPISER8B", 76 }
};
static const IoConfigCode  IoConfigCodes_54[] =
{
    { "OUTPUT", 18 }
};
static const IoConfigCode  IoConfigCodes_56[] =
{
    { "1080P59_94", 39 },
    { "1080P29_97", 49 }
};
static const IoConfigCode  IoConfigCodes_58[] =
{
    { "1080P59_94B", 40 },
    { "1080PSF30", 55 }
};
static const IoConfigCode  IoConfigCodes_59[] =
{
    { "RFCLKSEL", 3 },
    { "SPILVDS1", 78 }
};
static const IoConfigCode  IoConfigCodes_60[] =
{
    { "GENREF", 12 },
    { "IFADC", 29 },
    { "1080I50", 43 },
    { "SPILVDS2", 79 }
};
static const IoConfigCode  IoConfigCodes_61[] =
{
    { "DISABLED", 16 }
};
static const IoConfigCode  IoConfigCodes_62[] =
{
    { "IP", 30 },
    { "MODHQ", 68 },
    { "SPISER10B", 77 }
};
static const IoConfigCode  IoConfigCodes_63[] =
{
    { "SHAREDANT", 19 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IoConfigCodeHashSets[] -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// IoConfigCodeHashSets maps from hash value to IoConfigCodes_XX[].
//
const IoConfigCodeHashSet  IoConfigCodeHashSets[] =
{
    { IoConfigCodes_0, 1 },
    { IoConfigCodes_1, 1 },
    { IoConfigCodes_2, 1 },
    { IoConfigCodes_3, 4 },
    { NULL, 0 },
    { IoConfigCodes_5, 1 },
    { IoConfigCodes_6, 1 },
    { IoConfigCodes_7, 1 },
    { IoConfigCodes_8, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { IoConfigCodes_12, 1 },
    { NULL, 0 },
    { IoConfigCodes_14, 1 },
    { NULL, 0 },
    { IoConfigCodes_16, 2 },
    { IoConfigCodes_17, 7 },
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
    { IoConfigCodes_29, 2 },
    { IoConfigCodes_30, 3 },
    { IoConfigCodes_31, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { IoConfigCodes_34, 2 },
    { IoConfigCodes_35, 2 },
    { IoConfigCodes_36, 3 },
    { IoConfigCodes_37, 4 },
    { IoConfigCodes_38, 1 },
    { IoConfigCodes_39, 1 },
    { IoConfigCodes_40, 1 },
    { IoConfigCodes_41, 1 },
    { NULL, 0 },
    { IoConfigCodes_43, 1 },
    { NULL, 0 },
    { IoConfigCodes_45, 1 },
    { NULL, 0 },
    { IoConfigCodes_47, 1 },
    { IoConfigCodes_48, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { IoConfigCodes_51, 1 },
    { IoConfigCodes_52, 3 },
    { IoConfigCodes_53, 2 },
    { IoConfigCodes_54, 1 },
    { NULL, 0 },
    { IoConfigCodes_56, 2 },
    { NULL, 0 },
    { IoConfigCodes_58, 2 },
    { IoConfigCodes_59, 2 },
    { IoConfigCodes_60, 4 },
    { IoConfigCodes_61, 1 },
    { IoConfigCodes_62, 3 },
    { IoConfigCodes_63, 1 }
};
const Int  IoConfigCodesHashCount = 64;
