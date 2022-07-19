//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtuPropertyStore.c *#*#*#*#*#*#*#*#*# (C) 2022 DekTec
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
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 500, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_08[] =
{
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
static const DtProperty  DtProperties205_0_19[] =
{
    { "ARCHITECTURE_TX", ARCHITECTURE_TX_LEGACY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_47[] =
{
    { "FIRMWARE_RELEASE_N1_0", DT_PVE,
      "Bug fix: PC-reboot failure when unit is connected", PROPERTY_VALUE_TYPE_STRING, -1,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_48[] =
{
    { "FIRMWARE_RELEASE_N1_1", DT_PVE, "Bug fix: errorneous status-LED indication",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N2_0", DT_PVE, "Prepares the DTU-205 for SDI",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_49[] =
{
    { "FIRMWARE_RELEASE_N1_2", DT_PVE, "Performance improvements",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_51[] =
{
    { "FIRMWARE_RELEASE_N5_0", DT_PVE, "Support for transmission of 10-bit SDI",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_52[] =
{
    { "FIRMWARE_RELEASE_N5_1", DT_PVE, "Support for Huffman-coded SDI data",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N6_0", DT_PVE,
      "Internal maintenance update, no functional changes", PROPERTY_VALUE_TYPE_STRING,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_53[] =
{
    { "FIRMWARE_RELEASE_N5_2", DT_PVE,
      "Bug fix: potential occasional generation of unwanted extra bytes",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N7_0", DT_PVE,
      "Bug fix: Not all bytes in tx-FIFO were reported in FIFO load",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_54[] =
{
    { "FIRMWARE_RELEASE_N8_0", DT_PVE,
      "Internal maintenance update, no functional changes", PROPERTY_VALUE_TYPE_STRING,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MAIN_PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties205_0_55[] =
{
    { "FIRMWARE_RELEASE_N9_0", DT_PVE,
      "Internal maintenance update, no functional changes", PROPERTY_VALUE_TYPE_STRING,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { "VPD_ID", DT_PVE, "ASI/SDI output", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
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
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets205_0[] =
{
    { DtProperties205_0_00, 1 },
    { DtProperties205_0_01, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_0_04, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_0_08, 3 },
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
    { DtProperties205_0_19, 1 },
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
    { DtProperties205_0_46, 3 },
    { DtProperties205_0_47, 1 },
    { DtProperties205_0_48, 2 },
    { DtProperties205_0_49, 1 },
    { DtProperties205_0_50, 1 },
    { DtProperties205_0_51, 1 },
    { DtProperties205_0_52, 2 },
    { DtProperties205_0_53, 2 },
    { DtProperties205_0_54, 2 },
    { DtProperties205_0_55, 1 },
    { DtProperties205_0_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_0_59, 1 },
    { NULL, 0 },
    { DtProperties205_0_61, 1 },
    { NULL, 0 },
    { DtProperties205_0_63, 1 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-215; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// VHF/UHF modulator
// 
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
    { "ARCHITECTURE_TX", ARCHITECTURE_TX_LEGACY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_38[] =
{
    { "CAP_TX_DRM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
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
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties215_0_47[] =
{
    { "FIRMWARE_RELEASE_N1_0", DT_PVE,
      "Bug fix: Error in firmware which results in data duplication",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
static const DtProperty  DtProperties215_0_54[] =
{
    { "MAIN_PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
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
    { "VPD_ID", DT_PVE, "VHF/UHF modulator", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets215_0[] =
{
    { NULL, 0 },
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
    { DtProperties215_0_19, 3 },
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
    { DtProperties215_0_38, 1 },
    { DtProperties215_0_39, 1 },
    { DtProperties215_0_40, 3 },
    { DtProperties215_0_41, 2 },
    { DtProperties215_0_42, 3 },
    { DtProperties215_0_43, 3 },
    { NULL, 0 },
    { DtProperties215_0_45, 2 },
    { DtProperties215_0_46, 3 },
    { DtProperties215_0_47, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties215_0_50, 1 },
    { DtProperties215_0_51, 1 },
    { DtProperties215_0_52, 2 },
    { DtProperties215_0_53, 3 },
    { DtProperties215_0_54, 1 },
    { DtProperties215_0_55, 1 },
    { DtProperties215_0_56, 2 },
    { NULL, 0 },
    { DtProperties215_0_58, 1 },
    { DtProperties215_0_59, 3 },
    { DtProperties215_0_60, 1 },
    { DtProperties215_0_61, 1 },
    { NULL, 0 },
    { DtProperties215_0_63, 1 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-225; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// ASI/SDI input
// 
static const DtProperty  DtProperties225_0_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 500, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
    { "ARCHITECTURE_RX", ARCHITECTURE_RX_LEGACY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
static const DtProperty  DtProperties225_0_47[] =
{
    { "FIRMWARE_RELEASE_N1_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_48[] =
{
    { "FIRMWARE_RELEASE_N2_0", DT_PVE, "New driver interface; no functional changes",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_49[] =
{
    { "FIRMWARE_RELEASE_N3_0", DT_PVE,
      "Bug fix: PC-reboot failure when unit is connected", PROPERTY_VALUE_TYPE_STRING, -1,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_50[] =
{
    { "FIRMWARE_RELEASE_N3_1", DT_PVE, "Bug fix: FIFO load can increase above FIFO size",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N4_0", DT_PVE, "Add support for receiving SDI in 8-bit mode",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
               PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N5_0", DT_PVE, "Support for reception of 10-bit SDI",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_52[] =
{
    { "FIRMWARE_RELEASE_N5_1", DT_PVE, "Support for Huffman-coded SDI data",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N6_0", DT_PVE,
      "Internal maintenance update, no functional changes", PROPERTY_VALUE_TYPE_STRING,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_53[] =
{
    { "FIRMWARE_RELEASE_N5_2", DT_PVE,
      "Bug fix: potential occasional reception of unwanted extra bytes",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N7_0", DT_PVE,
      "Bug fix: sometimes fails to lock-on to input signal after a channel reset",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_54[] =
{
    { "FIRMWARE_RELEASE_N7_1", DT_PVE,
      "Bug fix: 32nd bit of clock-reference-counter was stuck to zero",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N8_0", DT_PVE, "Support transparent-packet receive mode",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MAIN_PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_55[] =
{
    { "FIRMWARE_RELEASE_N8_1", DT_PVE,
      "Fixes issue with DVB-ASI streams that have a bitrate >= 108Mbps",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N9_0", DT_PVE,
      "Fixes issue with stability of arrival timestamps assigned to packets",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { "VPD_ID", DT_PVE, "ASI/SDI input", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties225_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets225_0[] =
{
    { NULL, 0 },
    { DtProperties225_0_01, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_0_04, 2 },
    { NULL, 0 },
    { DtProperties225_0_06, 1 },
    { NULL, 0 },
    { DtProperties225_0_08, 3 },
    { DtProperties225_0_09, 2 },
    { DtProperties225_0_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_0_16, 1 },
    { DtProperties225_0_17, 2 },
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
    { DtProperties225_0_47, 1 },
    { DtProperties225_0_48, 1 },
    { DtProperties225_0_49, 1 },
    { DtProperties225_0_50, 3 },
    { DtProperties225_0_51, 2 },
    { DtProperties225_0_52, 2 },
    { DtProperties225_0_53, 2 },
    { DtProperties225_0_54, 3 },
    { DtProperties225_0_55, 2 },
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
    { "ARCHITECTURE_RX", ARCHITECTURE_RX_LEGACY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
static const DtProperty  DtProperties234_0_47[] =
{
    { "FIRMWARE_RELEASE_N1_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
static const DtProperty  DtProperties234_0_54[] =
{
    { "MAIN_PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
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
    { "VPD_ID", DT_PVE, "VSB/QAM-B receiver", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { NULL, 0 },
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
    { DtProperties234_0_17, 2 },
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
    { DtProperties234_0_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_0_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_0_53, 1 },
    { DtProperties234_0_54, 1 },
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
    { "ARCHITECTURE_RX", ARCHITECTURE_RX_LEGACY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_47[] =
{
    { "FIRMWARE_RELEASE_N1_0", DT_PVE,
      "Bug fix: failure to acquire data-lock => cannot read transport-stream",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
static const DtProperty  DtProperties235_0_54[] =
{
    { "MAIN_PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
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
    { "VPD_ID", DT_PVE, "DVB-T measurement receiver", PROPERTY_VALUE_TYPE_STRING, -1, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties235_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets235_0[] =
{
    { NULL, 0 },
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
    { DtProperties235_0_17, 3 },
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
    { DtProperties235_0_46, 3 },
    { DtProperties235_0_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_53, 1 },
    { DtProperties235_0_54, 1 },
    { DtProperties235_0_55, 1 },
    { DtProperties235_0_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_59, 3 },
    { DtProperties235_0_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_0_63, 1 }
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
    { "ARCHITECTURE_RX", ARCHITECTURE_RX_LEGACY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, ARCHITECTURE_RX_LEGACY, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
static const DtProperty  DtProperties236_0_54[] =
{
    { "MAIN_PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
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
    { "VPD_ID", DT_PVE, "QAM measurement receiver + ASI input",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { DtProperties236_0_17, 3 },
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
    { DtProperties236_0_46, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_0_50, 1 },
    { DtProperties236_0_51, 1 },
    { NULL, 0 },
    { DtProperties236_0_53, 2 },
    { DtProperties236_0_54, 1 },
    { DtProperties236_0_55, 2 },
    { DtProperties236_0_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_0_59, 4 },
    { DtProperties236_0_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_0_63, 2 }
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
    { "DEMOD_HW_XTAL", 24000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 100, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 41000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
    { "ARCHITECTURE_RX", ARCHITECTURE_RX_LEGACY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, ARCHITECTURE_RX_LEGACY, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
    { "PCB_LAST_REV", 100, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
static const DtProperty  DtProperties238_0_54[] =
{
    { "MAIN_PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
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
    { "VPD_ID", DT_PVE, "DVB-C2/T2 receiver measurement receiver + ASI input",
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
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets238_0[] =
{
    { NULL, 0 },
    { DtProperties238_0_01, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties238_0_04, 3 },
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
    { DtProperties238_0_17, 4 },
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
    { DtProperties238_0_46, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties238_0_50, 2 },
    { DtProperties238_0_51, 1 },
    { NULL, 0 },
    { DtProperties238_0_53, 1 },
    { DtProperties238_0_54, 1 },
    { DtProperties238_0_55, 1 },
    { DtProperties238_0_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties238_0_59, 4 },
    { DtProperties238_0_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties238_0_63, 2 }
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
    { "FIRMWARE_LAST_VER", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 600, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, 600,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_02[] =
{
    { "SUBTYPE1", 0x2580002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 500, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_06[] =
{
    { "REGISTERS_RX", 0x200, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 600, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, -1, 0, 0, 600,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_07[] =
{
    { "CAP_SDITIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 600, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_SUPPORTS_TXNULL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REGISTERS_TX", 0x180, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, -1, 0, 600, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, -1, 0, 0, 600,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_RO_SIZE", 256, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 600, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 256, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, 600,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "USB_DEVICE_ID", 0x422D, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 600, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x22D, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
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
    { "ARCHITECTURE_RX", ARCHITECTURE_RX_LEGACY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_RW_SIZE", 255, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 600, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 255, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, 600,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_19[] =
{
    { "ARCHITECTURE_TX", ARCHITECTURE_TX_LEGACY, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 600, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 8, 0, 600,
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
    { "PCB_LAST_REV", 700, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_29[] =
{
    { "CAP_HUFFMAN", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 600, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 600, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 5, 0, 600,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 5, 0, 600,
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
    { "CAP_LOOPTHR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 600, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 7, 0, 600,
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
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 600, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, 600, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_36[] =
{
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                         600, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 6, 0, 600,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_39[] =
{
    { "EEPROM_SIZE", 2048, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 600, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, 600,
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
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 600,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, 600,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_41[] =
{
    { "VPD_SIZE", 1536, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 600, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, 600,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                         600, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 6, 0, 600,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
          PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 600, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "First firmware version released to the field",
           PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, 600, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_47[] =
{
    { "FIRMWARE_RELEASE_N1_0", DT_PVE, "Bug fix: Loopthrough mode",
          PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 600, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_48[] =
{
    { "FIRMWARE_RELEASE_N2_0", DT_PVE, "Bug fix: 188+16 mode",
          PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 600, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
               PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N5_0", DT_PVE, "Support for reception of 10-bit SDI",
           PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, 600, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_52[] =
{
    { "FIRMWARE_RELEASE_N5_1", DT_PVE, "Support for Huffman-coded SDI data",
           PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, 600, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N6_0", DT_PVE,
      "Internal maintenance update, no functional changes", PROPERTY_VALUE_TYPE_STRING,
                                       -1, -1, 0, 0, 600, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_53[] =
{
    { "FIRMWARE_RELEASE_N5_2", DT_PVE,
      "Bug fix: potential reception/generation of unwanted extra bytes",
           PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, 600, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N7_0", DT_PVE,
      "Bug fix: sometimes fails to lock-on to input signal after a channel reset",
           PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, 600, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_54[] =
{
    { "FIRMWARE_RELEASE_N7_1", DT_PVE,
      "Bug fix: 32nd bit of clock-reference-counter was stuck to zero",
           PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, 600, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N8_0", DT_PVE, "Support transparent-packet receive mode",
           PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, 600, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MAIN_PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_55[] =
{
    { "FIRMWARE_RELEASE_N7_2", DT_PVE,
      "Bug fix: not all bytes in tx-FIFO were reported in FIFO load",
           PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, 600, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N8_1", DT_PVE,
      "Fixes issue with DVB-ASI streams that have a bitrate >= 108Mbps",
           PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, 600, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N9_0", DT_PVE,
      "Fixes issue with stability of arrival timestamps assigned to packets",
           PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, 600, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { "VPD_ID", DT_PVE, "ASI/SDI input+output", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties245_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 600, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties245_0_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets245_0[] =
{
    { DtProperties245_0_00, 1 },
    { DtProperties245_0_01, 2 },
    { DtProperties245_0_02, 1 },
    { NULL, 0 },
    { DtProperties245_0_04, 2 },
    { NULL, 0 },
    { DtProperties245_0_06, 2 },
    { DtProperties245_0_07, 1 },
    { DtProperties245_0_08, 6 },
    { DtProperties245_0_09, 4 },
    { DtProperties245_0_10, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_16, 1 },
    { DtProperties245_0_17, 3 },
    { NULL, 0 },
    { DtProperties245_0_19, 3 },
    { DtProperties245_0_20, 1 },
    { DtProperties245_0_21, 2 },
    { DtProperties245_0_22, 1 },
    { DtProperties245_0_23, 2 },
    { NULL, 0 },
    { DtProperties245_0_25, 1 },
    { DtProperties245_0_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_29, 6 },
    { NULL, 0 },
    { DtProperties245_0_31, 1 },
    { DtProperties245_0_32, 4 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_35, 4 },
    { DtProperties245_0_36, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_39, 2 },
    { DtProperties245_0_40, 5 },
    { DtProperties245_0_41, 2 },
    { DtProperties245_0_42, 3 },
    { DtProperties245_0_43, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_46, 5 },
    { DtProperties245_0_47, 1 },
    { DtProperties245_0_48, 1 },
    { NULL, 0 },
    { DtProperties245_0_50, 1 },
    { DtProperties245_0_51, 2 },
    { DtProperties245_0_52, 2 },
    { DtProperties245_0_53, 2 },
    { DtProperties245_0_54, 3 },
    { DtProperties245_0_55, 3 },
    { DtProperties245_0_56, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_0_59, 2 },
    { NULL, 0 },
    { DtProperties245_0_61, 2 },
    { NULL, 0 },
    { DtProperties245_0_63, 2 }
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
    { "ARCHITECTURE_RX", ARCHITECTURE_RX_LEGACY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_RW_SIZE", 255, NULL, PROPERTY_VALUE_TYPE_UINT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_19[] =
{
    { "ARCHITECTURE_TX", ARCHITECTURE_TX_LEGACY, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
static const DtProperty  DtProperties255_0_54[] =
{
    { "MAIN_PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties255_0_55[] =
{
    { "FIRMWARE_RELEASE_N9_0", DT_PVE, "See release notes for DTU-245",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { "VPD_ID", DT_PVE, "DTU-2X5 in manufacturing mode", PROPERTY_VALUE_TYPE_STRING, -1,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { DtProperties255_0_08, 5 },
    { DtProperties255_0_09, 3 },
    { DtProperties255_0_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_0_16, 1 },
    { DtProperties255_0_17, 2 },
    { NULL, 0 },
    { DtProperties255_0_19, 1 },
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
    { DtProperties255_0_54, 1 },
    { DtProperties255_0_55, 1 },
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_46[] =
{
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_50[] =
{
    { "PORT_COUNT", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_54[] =
{
    { "MAIN_PORT_COUNT", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_56[] =
{
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties299_0_59[] =
{
    { "VPD_ID", DT_PVE, "DTU-2X5 in manufacturing mode", PROPERTY_VALUE_TYPE_STRING, -1,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { DtProperties299_0_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_54, 1 },
    { NULL, 0 },
    { DtProperties299_0_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_0_59, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-315; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Universal modulator
// 
static const DtProperty  DtProperties315_0_01[] =
{
    { "CAP_TX_ATSC3", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
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
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_ISDBS3", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DTAPI, 5, 29, 2 } 
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
    { "ARCHITECTURE_TX", ARCHITECTURE_TX_LEGACY_USB_315, NULL, PROPERTY_VALUE_TYPE_INT,
                 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
    { "CAP_TX_SWMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
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
    { "CAP_DVBCID", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_SAMPRATE_HW_MAX", 43750000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 43750000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 70000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 70000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PROGRAMMING_SEC_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_37[] =
{
    { "CAP_TX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_38[] =
{
    { "CAP_TX_DRM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_47[] =
{
    { "FIRMWARE_RELEASE_N1_0", DT_PVE,
      "Bug fix: Power-level 6dB too low for symbol rates > 47MBaud",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MOD_HW_SAMP_PACKING", 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 2, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 3, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties315_0_48[] =
{
    { "FIRMWARE_RELEASE_N1_1", DT_PVE,
      "Bug fix: Occasionally SetTxControl reported driver error",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
static const DtProperty  DtProperties315_0_54[] =
{
    { "MAIN_PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
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
    { "USB_END_POINT_WRITE", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
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
    { "VPD_ID", DT_PVE, "Universal modulator", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { DtProperties315_0_11, 2 },
    { DtProperties315_0_12, 1 },
    { NULL, 0 },
    { DtProperties315_0_14, 1 },
    { NULL, 0 },
    { DtProperties315_0_16, 1 },
    { DtProperties315_0_17, 2 },
    { DtProperties315_0_18, 2 },
    { DtProperties315_0_19, 3 },
    { DtProperties315_0_20, 2 },
    { NULL, 0 },
    { DtProperties315_0_22, 5 },
    { NULL, 0 },
    { DtProperties315_0_24, 3 },
    { DtProperties315_0_25, 2 },
    { DtProperties315_0_26, 1 },
    { DtProperties315_0_27, 1 },
    { DtProperties315_0_28, 2 },
    { DtProperties315_0_29, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties315_0_33, 5 },
    { DtProperties315_0_34, 4 },
    { DtProperties315_0_35, 5 },
    { DtProperties315_0_36, 6 },
    { DtProperties315_0_37, 1 },
    { DtProperties315_0_38, 1 },
    { DtProperties315_0_39, 1 },
    { DtProperties315_0_40, 3 },
    { DtProperties315_0_41, 2 },
    { DtProperties315_0_42, 3 },
    { DtProperties315_0_43, 9 },
    { DtProperties315_0_44, 1 },
    { DtProperties315_0_45, 8 },
    { DtProperties315_0_46, 3 },
    { DtProperties315_0_47, 5 },
    { DtProperties315_0_48, 1 },
    { DtProperties315_0_49, 1 },
    { DtProperties315_0_50, 5 },
    { DtProperties315_0_51, 1 },
    { DtProperties315_0_52, 3 },
    { DtProperties315_0_53, 6 },
    { DtProperties315_0_54, 5 },
    { DtProperties315_0_55, 4 },
    { DtProperties315_0_56, 5 },
    { DtProperties315_0_57, 1 },
    { DtProperties315_0_58, 4 },
    { DtProperties315_0_59, 6 },
    { DtProperties315_0_60, 1 },
    { DtProperties315_0_61, 1 },
    { NULL, 0 },
    { DtProperties315_0_63, 1 }
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
static const DtProperty  DtProperties350_0_19[] =
{
    { "ARCHITECTURE_TX", ARCHITECTURE_TX_LEGACY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "See release notes for DTU-350",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MAIN_PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
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
    { "VPD_ID", DT_PVE, "HD-SDI output", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties350_0_61[] =
{
    { "TESTING_LOOP_BACK_32B_TX", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { DtProperties350_0_19, 1 },
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
    { DtProperties350_0_46, 3 },
    { DtProperties350_0_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties350_0_50, 1 },
    { NULL, 0 },
    { DtProperties350_0_52, 1 },
    { NULL, 0 },
    { DtProperties350_0_54, 2 },
    { NULL, 0 },
    { DtProperties350_0_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties350_0_59, 3 },
    { NULL, 0 },
    { DtProperties350_0_61, 1 },
    { NULL, 0 },
    { NULL, 0 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-351; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// HD-SDI input
// 
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
    { "USB_END_POINT_READ", 3, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
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
    { "ARCHITECTURE_RX", ARCHITECTURE_RX_LEGACY_USB_SDI_MEMLESS, NULL,
      PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "See release notes for DTU-351",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties351_0_47[] =
{
    { "CAP_1080I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N1_0", DT_PVE,
      "Serveral performance improvements fixing possible dropped frames issue",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MAIN_PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
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
    { "VPD_ID", DT_PVE, "HD-SDI input", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
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
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets351_0[] =
{
    { NULL, 0 },
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
    { DtProperties351_0_17, 3 },
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
    { DtProperties351_0_46, 3 },
    { DtProperties351_0_47, 2 },
    { NULL, 0 },
    { DtProperties351_0_49, 1 },
    { DtProperties351_0_50, 1 },
    { DtProperties351_0_51, 1 },
    { DtProperties351_0_52, 1 },
    { NULL, 0 },
    { DtProperties351_0_54, 2 },
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_46[] =
{
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_50[] =
{
    { "PORT_COUNT", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_54[] =
{
    { "MAIN_PORT_COUNT", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_56[] =
{
    { "USB_END_POINT_WRITE", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties399_0_59[] =
{
    { "VPD_ID", DT_PVE, "DTU-3XX with uninitialized EEPROM", PROPERTY_VALUE_TYPE_STRING,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
    { DtProperties399_0_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_54, 1 },
    { NULL, 0 },
    { DtProperties399_0_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties399_0_59, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 }
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
/*  2 */ "IODOWNSCALE",
/*  3 */ "PWRMODE",
/*  4 */ "RFCLKSEL",
/*  5 */ "SPICLKSEL",
/*  6 */ "SPIMODE",
/*  7 */ "SPISTD",
/*  8 */ "TSRATESEL",
/*  9 */ "TODREFSEL",
/* 10 */ "AUTOBFGEN",
/* 11 */ "BW",
/* 12 */ "DMATESTMODE",
/* 13 */ "FAILSAFE",
/* 14 */ "FRACMODE",
/* 15 */ "GENLOCKED",
/* 16 */ "GENREF",
/* 17 */ "SWS2APSK",
/* 18 */ "TRUE",
/* 19 */ "FALSE",
/* 20 */ "DISABLED",
/* 21 */ "INPUT",
/* 22 */ "INTINPUT",
/* 23 */ "INTOUTPUT",
/* 24 */ "MONITOR",
/* 25 */ "OUTPUT",
/* 26 */ "SHAREDANT",
/* 27 */ "DBLBUF",
/* 28 */ "LOOPS2L3",
/* 29 */ "LOOPS2TS",
/* 30 */ "LOOPTHR",
/* 31 */ "12GSDI",
/* 32 */ "3GSDI",
/* 33 */ "6GSDI",
/* 34 */ "ASI",
/* 35 */ "AVENC",
/* 36 */ "DEKTECST",
/* 37 */ "DEMOD",
/* 38 */ "GPSTIME",
/* 39 */ "HDMI",
/* 40 */ "HDSDI",
/* 41 */ "IFADC",
/* 42 */ "IP",
/* 43 */ "MOD",
/* 44 */ "PHASENOISE",
/* 45 */ "RS422",
/* 46 */ "SDIRX",
/* 47 */ "SDI",
/* 48 */ "SPI",
/* 49 */ "SPISDI",
/* 50 */ "2160P50",
/* 51 */ "2160P50B",
/* 52 */ "2160P59_94",
/* 53 */ "2160P59_94B",
/* 54 */ "2160P60",
/* 55 */ "2160P60B",
/* 56 */ "1080P50",
/* 57 */ "1080P50B",
/* 58 */ "1080P59_94",
/* 59 */ "1080P59_94B",
/* 60 */ "1080P60",
/* 61 */ "1080P60B",
/* 62 */ "2160P23_98",
/* 63 */ "2160P24",
/* 64 */ "2160P25",
/* 65 */ "2160P29_97",
/* 66 */ "2160P30",
/* 67 */ "1080I50",
/* 68 */ "1080I59_94",
/* 69 */ "1080I60",
/* 70 */ "1080P23_98",
/* 71 */ "1080P24",
/* 72 */ "1080P25",
/* 73 */ "1080P29_97",
/* 74 */ "1080P30",
/* 75 */ "1080PSF23_98",
/* 76 */ "1080PSF24",
/* 77 */ "1080PSF25",
/* 78 */ "1080PSF29_97",
/* 79 */ "1080PSF30",
/* 80 */ "720P23_98",
/* 81 */ "720P24",
/* 82 */ "720P25",
/* 83 */ "720P29_97",
/* 84 */ "720P30",
/* 85 */ "720P50",
/* 86 */ "720P59_94",
/* 87 */ "720P60",
/* 88 */ "525I59_94",
/* 89 */ "625I50",
/* 90 */ "SPI525I59_94",
/* 91 */ "SPI625I50",
/* 92 */ "SCALE_12GTO3G",
/* 93 */ "SCALE_BYPASS",
/* 94 */ "MODHQ",
/* 95 */ "LOWPWR",
/* 96 */ "RFCLKEXT",
/* 97 */ "RFCLKINT",
/* 98 */ "SPICLKEXT",
/* 99 */ "SPICLKINT",
/* 100 */ "SPIFIXEDCLK",
/* 101 */ "SPIDVBMODE",
/* 102 */ "SPISER8B",
/* 103 */ "SPISER10B",
/* 104 */ "SPILVDS1",
/* 105 */ "SPILVDS2",
/* 106 */ "SPILVTTL",
/* 107 */ "EXTTSRATE",
/* 108 */ "EXTRATIO",
/* 109 */ "INTTSRATE",
/* 110 */ "LOCK2INP",
/* 111 */ "TODREF_INTERNAL",
/* 112 */ "TODREF_STEADYCLOCK"
};
const Int  IoConfigCodeStringCount = 113;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IoConfigCodes_XX[] -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Helper arrays to map from DT_IOCONFIG name to DT_IOCONFIG code.
// The hash of the DT_IOCONFIG names in IoConfigCodes_XX[] is XX.
//
static const IoConfigCode  IoConfigCodes_0[] =
{
    { "FAILSAFE", 13 },
    { "INTINPUT", 22 }
};
static const IoConfigCode  IoConfigCodes_1[] =
{
    { "INTOUTPUT", 23 },
    { "1080P30", 74 }
};
static const IoConfigCode  IoConfigCodes_2[] =
{
    { "RS422", 45 }
};
static const IoConfigCode  IoConfigCodes_3[] =
{
    { "PWRMODE", 3 },
    { "LOOPS2L3", 28 },
    { "1080P50", 56 },
    { "720P23_98", 80 }
};
static const IoConfigCode  IoConfigCodes_4[] =
{
    { "2160P60", 54 },
    { "2160P24", 63 }
};
static const IoConfigCode  IoConfigCodes_5[] =
{
    { "SDI", 47 },
    { "2160P50B", 51 },
    { "2160P25", 64 },
    { "TODREF_INTERNAL", 111 }
};
static const IoConfigCode  IoConfigCodes_6[] =
{
    { "FRACMODE", 14 },
    { "2160P60B", 55 }
};
static const IoConfigCode  IoConfigCodes_7[] =
{
    { "HDMI", 39 },
    { "SPILVTTL", 106 }
};
static const IoConfigCode  IoConfigCodes_8[] =
{
    { "IOSTD", 1 },
    { "720P29_97", 83 },
    { "720P59_94", 86 }
};
static const IoConfigCode  IoConfigCodes_12[] =
{
    { "1080PSF23_98", 75 }
};
static const IoConfigCode  IoConfigCodes_13[] =
{
    { "TODREFSEL", 9 }
};
static const IoConfigCode  IoConfigCodes_14[] =
{
    { "DEMOD", 37 }
};
static const IoConfigCode  IoConfigCodes_15[] =
{
    { "12GSDI", 31 }
};
static const IoConfigCode  IoConfigCodes_16[] =
{
    { "FALSE", 19 },
    { "625I50", 89 }
};
static const IoConfigCode  IoConfigCodes_17[] =
{
    { "GENLOCKED", 15 },
    { "HDSDI", 40 },
    { "SPI", 48 },
    { "SPISDI", 49 },
    { "1080I59_94", 68 },
    { "1080PSF29_97", 78 },
    { "720P30", 84 }
};
static const IoConfigCode  IoConfigCodes_18[] =
{
    { "AVENC", 35 },
    { "SPIDVBMODE", 101 }
};
static const IoConfigCode  IoConfigCodes_19[] =
{
    { "2160P23_98", 62 },
    { "720P50", 85 },
    { "SCALE_12GTO3G", 92 }
};
static const IoConfigCode  IoConfigCodes_20[] =
{
    { "DBLBUF", 27 }
};
static const IoConfigCode  IoConfigCodes_21[] =
{
    { "INPUT", 21 },
    { "EXTRATIO", 108 }
};
static const IoConfigCode  IoConfigCodes_22[] =
{
    { "SPIMODE", 6 },
    { "SPICLKINT", 99 }
};
static const IoConfigCode  IoConfigCodes_24[] =
{
    { "2160P59_94", 52 },
    { "2160P29_97", 65 }
};
static const IoConfigCode  IoConfigCodes_26[] =
{
    { "2160P59_94B", 53 }
};
static const IoConfigCode  IoConfigCodes_27[] =
{
    { "SPIFIXEDCLK", 100 }
};
static const IoConfigCode  IoConfigCodes_28[] =
{
    { "IODIR", 0 },
    { "SPISTD", 7 },
    { "TSRATESEL", 8 },
    { "SPI625I50", 91 },
    { "SPICLKEXT", 98 }
};
static const IoConfigCode  IoConfigCodes_29[] =
{
    { "1080I60", 69 },
    { "1080PSF24", 76 }
};
static const IoConfigCode  IoConfigCodes_30[] =
{
    { "BW", 11 },
    { "GPSTIME", 38 },
    { "1080PSF25", 77 },
    { "SCALE_BYPASS", 93 }
};
static const IoConfigCode  IoConfigCodes_31[] =
{
    { "3GSDI", 32 }
};
static const IoConfigCode  IoConfigCodes_32[] =
{
    { "AUTOBFGEN", 10 }
};
static const IoConfigCode  IoConfigCodes_33[] =
{
    { "2160P30", 66 }
};
static const IoConfigCode  IoConfigCodes_34[] =
{
    { "6GSDI", 33 },
    { "ASI", 34 },
    { "RFCLKINT", 97 }
};
static const IoConfigCode  IoConfigCodes_35[] =
{
    { "SWS2APSK", 17 },
    { "2160P50", 50 },
    { "INTTSRATE", 109 }
};
static const IoConfigCode  IoConfigCodes_36[] =
{
    { "1080P60", 60 },
    { "1080P24", 71 },
    { "525I59_94", 88 }
};
static const IoConfigCode  IoConfigCodes_37[] =
{
    { "TRUE", 18 },
    { "MOD", 43 },
    { "1080P50B", 57 },
    { "1080P25", 72 }
};
static const IoConfigCode  IoConfigCodes_38[] =
{
    { "1080P60B", 61 }
};
static const IoConfigCode  IoConfigCodes_39[] =
{
    { "LOCK2INP", 110 }
};
static const IoConfigCode  IoConfigCodes_40[] =
{
    { "RFCLKEXT", 96 }
};
static const IoConfigCode  IoConfigCodes_41[] =
{
    { "EXTTSRATE", 107 }
};
static const IoConfigCode  IoConfigCodes_43[] =
{
    { "LOOPS2TS", 29 }
};
static const IoConfigCode  IoConfigCodes_45[] =
{
    { "MONITOR", 24 },
    { "LOOPTHR", 30 }
};
static const IoConfigCode  IoConfigCodes_47[] =
{
    { "SPICLKSEL", 5 },
    { "SDIRX", 46 }
};
static const IoConfigCode  IoConfigCodes_48[] =
{
    { "SPI525I59_94", 90 },
    { "LOWPWR", 95 }
};
static const IoConfigCode  IoConfigCodes_51[] =
{
    { "1080P23_98", 70 }
};
static const IoConfigCode  IoConfigCodes_52[] =
{
    { "PHASENOISE", 44 },
    { "720P24", 81 },
    { "720P60", 87 }
};
static const IoConfigCode  IoConfigCodes_53[] =
{
    { "720P25", 82 },
    { "SPISER8B", 102 }
};
static const IoConfigCode  IoConfigCodes_54[] =
{
    { "OUTPUT", 25 }
};
static const IoConfigCode  IoConfigCodes_56[] =
{
    { "1080P59_94", 58 },
    { "1080P29_97", 73 }
};
static const IoConfigCode  IoConfigCodes_58[] =
{
    { "1080P59_94B", 59 },
    { "1080PSF30", 79 }
};
static const IoConfigCode  IoConfigCodes_59[] =
{
    { "RFCLKSEL", 4 },
    { "SPILVDS1", 104 }
};
static const IoConfigCode  IoConfigCodes_60[] =
{
    { "DMATESTMODE", 12 },
    { "GENREF", 16 },
    { "DEKTECST", 36 },
    { "IFADC", 41 },
    { "1080I50", 67 },
    { "SPILVDS2", 105 }
};
static const IoConfigCode  IoConfigCodes_61[] =
{
    { "IODOWNSCALE", 2 },
    { "DISABLED", 20 }
};
static const IoConfigCode  IoConfigCodes_62[] =
{
    { "IP", 42 },
    { "MODHQ", 94 },
    { "SPISER10B", 103 },
    { "TODREF_STEADYCLOCK", 112 }
};
static const IoConfigCode  IoConfigCodes_63[] =
{
    { "SHAREDANT", 26 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IoConfigCodeHashSets[] -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// IoConfigCodeHashSets maps from hash value to IoConfigCodes_XX[].
//
const IoConfigCodeHashSet  IoConfigCodeHashSets[] =
{
    { IoConfigCodes_0, 2 },
    { IoConfigCodes_1, 2 },
    { IoConfigCodes_2, 1 },
    { IoConfigCodes_3, 4 },
    { IoConfigCodes_4, 2 },
    { IoConfigCodes_5, 4 },
    { IoConfigCodes_6, 2 },
    { IoConfigCodes_7, 2 },
    { IoConfigCodes_8, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { IoConfigCodes_12, 1 },
    { IoConfigCodes_13, 1 },
    { IoConfigCodes_14, 1 },
    { IoConfigCodes_15, 1 },
    { IoConfigCodes_16, 2 },
    { IoConfigCodes_17, 7 },
    { IoConfigCodes_18, 2 },
    { IoConfigCodes_19, 3 },
    { IoConfigCodes_20, 1 },
    { IoConfigCodes_21, 2 },
    { IoConfigCodes_22, 2 },
    { NULL, 0 },
    { IoConfigCodes_24, 2 },
    { NULL, 0 },
    { IoConfigCodes_26, 1 },
    { IoConfigCodes_27, 1 },
    { IoConfigCodes_28, 5 },
    { IoConfigCodes_29, 2 },
    { IoConfigCodes_30, 4 },
    { IoConfigCodes_31, 1 },
    { IoConfigCodes_32, 1 },
    { IoConfigCodes_33, 1 },
    { IoConfigCodes_34, 3 },
    { IoConfigCodes_35, 3 },
    { IoConfigCodes_36, 3 },
    { IoConfigCodes_37, 4 },
    { IoConfigCodes_38, 1 },
    { IoConfigCodes_39, 1 },
    { IoConfigCodes_40, 1 },
    { IoConfigCodes_41, 1 },
    { NULL, 0 },
    { IoConfigCodes_43, 1 },
    { NULL, 0 },
    { IoConfigCodes_45, 2 },
    { NULL, 0 },
    { IoConfigCodes_47, 2 },
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
    { IoConfigCodes_60, 6 },
    { IoConfigCodes_61, 2 },
    { IoConfigCodes_62, 4 },
    { IoConfigCodes_63, 1 }
};
const Int  IoConfigCodesHashCount = 64;
