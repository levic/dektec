//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtuPropertyStore.c *#*#*#*#*#*#*#*#*# (C) 2012 DekTec
//
// CapParser - Property Store - Tables storing device properties
//
// THIS FILE IS GENERATED, DO NOT EDIT!!!

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#include "DtuIncludes.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-205 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// ASI/SDI output
// 
static const DtProperty  DtProperties205_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "Bug fix: PC-reboot failure when unit is connected",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N1_1", -1, "Bug fix: errorneous status-LED indication",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N2_0", -1, "Prepares the DTU-205 for SDI", PROPERTY_VALUE_TYPE_STRING,
                                                         -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_02[] =
{
    { "FIRMWARE_R_N1_2", -1, "Performance improvements", PROPERTY_VALUE_TYPE_STRING, -1,
                                                             0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_04[] =
{
    { "FIRMWARE_R_N5_0", -1, "Support for transmission of 10-bit SDI",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_05[] =
{
    { "FIRMWARE_R_N5_1", -1, "Support for Huffman compressed SDI formats",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N6_0", -1, "Internal maintenance update, no functional changes",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_06[] =
{
    { "FIRMWARE_R_N5_2", -1,
      "Bug fix: potential occasional generation of unwanted extra bytes",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N7_0", -1,
      "Bug fix: Not all bytes in tx-FIFO were reported in FIFO load",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_07[] =
{
    { "FIRMWARE_R_N8_0", -1, "Internal maintenance update, no functional changes",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_08[] =
{
    { "FIRMWARE_R_N9_0", -1, "Internal maintenance update, no functional changes",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REGISTERS_TX", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_READ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties205_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                                            0, 0, PROPERTY_SCOPE_DRIVER },
    { "USB_DEVICE_ID", 0x205, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties205_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_25[] =
{
    { "PCB_LAST_REV", 501, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 500,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties205_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties205_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                                                   PROPERTY_SCOPE_DTAPI },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties205_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 6, 0,
                                                                   PROPERTY_SCOPE_DTAPI },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_48[] =
{
    { "CAP_DVBT2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "TESTING_LOOP_BACK_32B", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 2, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties205_59[] =
{
    { "VPD_ID", -1, "ASI/SDI output", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties205_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtPropertyHashSet  DtPropertyHashSets205[] =
{
    { DtProperties205_00, 1 },
    { DtProperties205_01, 3 },
    { DtProperties205_02, 1 },
    { NULL, 0 },
    { DtProperties205_04, 3 },
    { DtProperties205_05, 2 },
    { DtProperties205_06, 2 },
    { DtProperties205_07, 1 },
    { DtProperties205_08, 3 },
    { DtProperties205_09, 2 },
    { DtProperties205_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_21, 1 },
    { DtProperties205_22, 1 },
    { DtProperties205_23, 1 },
    { NULL, 0 },
    { DtProperties205_25, 1 },
    { DtProperties205_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_29, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_39, 1 },
    { DtProperties205_40, 4 },
    { NULL, 0 },
    { DtProperties205_42, 2 },
    { DtProperties205_43, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_46, 2 },
    { NULL, 0 },
    { DtProperties205_48, 1 },
    { NULL, 0 },
    { DtProperties205_50, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_59, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties205_63, 2 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-215 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// VHF/UHF modulator
// 
static const DtProperty  DtProperties215_00[] =
{
    { "FIRMWARE_R_N1_0", -1,
      "Bug fix: Error in firmware which results in data duplication",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_01[] =
{
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "MOD_LEVEL_NUM_ATTN", 1, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_02[] =
{
    { "MOD_HW_RFPLL", 0, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_03[] =
{
    { "CAP_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "MOD_HW_HAS_AD9789", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "MOD_HW_SYM_SAMP_GEN", 21111, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_08[] =
{
    { "CAP_CM", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "CAP_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "MOD_LEVEL_RANGE", 315, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REGISTERS_TX", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_09[] =
{
    { "CAP_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_READ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties215_10[] =
{
    { "MOD_HW_HAS_PROG_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_DEVICE_ID", 0x20F, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties215_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_18[] =
{
    { "CAP_CW", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "CAP_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_20[] =
{
    { "MOD_LEVEL_HAS_VGA", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "MOD_HW_HAS_SEL_INTPOL", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_23[] =
{
    { "CAP_QAM_A", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_24[] =
{
    { "CAP_MOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "CAP_QAM_B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_25[] =
{
    { "CAP_QAM_C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "PCB_LAST_REV", 300, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_31[] =
{
    { "CAP_DTMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_33[] =
{
    { "MOD_LEVEL_MAX", -150, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "MOD_SAMPRATE_SRC_ABSMIN", 100000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_34[] =
{
    { "MOD_SAMPRATE_HW_MIN", 4687500, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER },
    { "MOD_SAMPRATE_SRC_ABSMAX", 10937499, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_36[] =
{
    { "MOD_SAMPRATE_HW_MAX", 9375000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties215_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties215_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "MOD_FREQ_ABSMIN", 20, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "MOD_SAMPRATE_SRC_MIN", 1000000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_45[] =
{
    { "MOD_FREQ_ABSMAX", 1180, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "MOD_SAMPRATE_SRC_MAX", 10937499, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_46[] =
{
    { "CAP_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_47[] =
{
    { "MOD_HW_SAMP_PACKING", 10, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "TESTING_LOOP_BACK_32B", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_51[] =
{
    { "CAP_MUTE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_53[] =
{
    { "CAP_ADJLVL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "CAP_RFCLKINT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "MOD_FREQ_MIN", 36, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_55[] =
{
    { "CAP_ATSC_MH", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "CAP_CMMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "MOD_FREQ_MAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_56[] =
{
    { "MOD_SAMPRATE_HW_ABSMIN", 4687500, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties215_58[] =
{
    { "CAP_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "MOD_SAMPRATE_HW_ABSMAX", 9375000, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "MOD_HW_HAS_SYMIQ_MAPPING", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "VPD_ID", -1, "VHF/UHF modulator", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties215_63[] =
{
    { "CAP_DAB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtPropertyHashSet  DtPropertyHashSets215[] =
{
    { DtProperties215_00, 1 },
    { DtProperties215_01, 2 },
    { DtProperties215_02, 1 },
    { DtProperties215_03, 2 },
    { DtProperties215_04, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties215_08, 5 },
    { DtProperties215_09, 3 },
    { DtProperties215_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties215_16, 1 },
    { NULL, 0 },
    { DtProperties215_18, 2 },
    { NULL, 0 },
    { DtProperties215_20, 1 },
    { NULL, 0 },
    { DtProperties215_22, 2 },
    { DtProperties215_23, 1 },
    { DtProperties215_24, 2 },
    { DtProperties215_25, 2 },
    { DtProperties215_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties215_29, 1 },
    { NULL, 0 },
    { DtProperties215_31, 1 },
    { NULL, 0 },
    { DtProperties215_33, 2 },
    { DtProperties215_34, 1 },
    { DtProperties215_35, 2 },
    { DtProperties215_36, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties215_39, 1 },
    { DtProperties215_40, 3 },
    { NULL, 0 },
    { DtProperties215_42, 1 },
    { DtProperties215_43, 3 },
    { NULL, 0 },
    { DtProperties215_45, 2 },
    { DtProperties215_46, 3 },
    { DtProperties215_47, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties215_50, 2 },
    { DtProperties215_51, 1 },
    { NULL, 0 },
    { DtProperties215_53, 3 },
    { NULL, 0 },
    { DtProperties215_55, 3 },
    { DtProperties215_56, 2 },
    { NULL, 0 },
    { DtProperties215_58, 2 },
    { DtProperties215_59, 3 },
    { DtProperties215_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties215_63, 3 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-225 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// ASI/SDI input
// 
static const DtProperty  DtProperties225_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "First firmware version released to the field",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N2_0", -1, "New driver interface; no functional changes",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_02[] =
{
    { "FIRMWARE_R_N3_0", -1, "Bug fix: PC-reboot failure when unit is connected",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_03[] =
{
    { "FIRMWARE_R_N3_1", -1, "Bug fix: FIFO load can increase above FIFO size",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N4_0", -1, "Add support for receiving SDI in 8-bit mode",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_04[] =
{
    { "FIRMWARE_R_N5_0", -1, "Support for reception of 10-bit SDI",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_05[] =
{
    { "FIRMWARE_R_N5_1", -1, "Support for Huffman compressed SDI formats",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N6_0", -1, "Internal maintenance update, no functional changes",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_06[] =
{
    { "FIRMWARE_R_N5_2", -1,
      "Bug fix: potential occasional reception of unwanted extra bytes",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N7_0", -1,
      "Bug fix: sometimes fails to lock-on to input signal after a channel reset",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_07[] =
{
    { "FIRMWARE_R_N7_1", -1,
      "Bug fix: 32nd bit of clock-reference-counter was stuck to zero",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N8_0", -1, "Support transparent-packet receive mode",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N8_1", -1,
      "Fixes issue with DVB-ASI streams that have a bitrate >= 108Mbps",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N9_0", -1,
      "Fixes issue with stability of arrival timestamps assigned to packets",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties225_10[] =
{
    { "USB_DEVICE_ID", 0x219, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties225_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 8, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_25[] =
{
    { "PCB_LAST_REV", 501, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 500,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties225_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties225_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                                                   PROPERTY_SCOPE_DTAPI },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties225_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 6, 0,
                                                                   PROPERTY_SCOPE_DTAPI },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "TESTING_LOOP_BACK_32B", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                                PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties225_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 4, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties225_59[] =
{
    { "VPD_ID", -1, "ASI/SDI input", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties225_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtPropertyHashSet  DtPropertyHashSets225[] =
{
    { DtProperties225_00, 1 },
    { DtProperties225_01, 2 },
    { DtProperties225_02, 1 },
    { DtProperties225_03, 2 },
    { DtProperties225_04, 3 },
    { DtProperties225_05, 2 },
    { DtProperties225_06, 3 },
    { DtProperties225_07, 2 },
    { DtProperties225_08, 4 },
    { DtProperties225_09, 1 },
    { DtProperties225_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_19, 1 },
    { NULL, 0 },
    { DtProperties225_21, 1 },
    { NULL, 0 },
    { DtProperties225_23, 1 },
    { NULL, 0 },
    { DtProperties225_25, 1 },
    { DtProperties225_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_29, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_35, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_39, 1 },
    { DtProperties225_40, 4 },
    { NULL, 0 },
    { DtProperties225_42, 2 },
    { DtProperties225_43, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_50, 2 },
    { DtProperties225_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_56, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_59, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties225_63, 1 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-234 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// VSB/QAM-B receiver
// 
static const DtProperty  DtProperties234_00[] =
{
    { "FIRMWARE_R_N1_0", -1, "First firmware version released to the field",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_03[] =
{
    { "CAP_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties234_10[] =
{
    { "USB_DEVICE_ID", 0x222, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties234_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_SENCORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_24[] =
{
    { "CAP_QAM_B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_25[] =
{
    { "PCB_LAST_REV", 800, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties234_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties234_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties234_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "MOD_FREQ_ABSMIN", 50, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_45[] =
{
    { "MOD_FREQ_ABSMAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "TESTING_LOOP_BACK_32B", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_53[] =
{
    { "MOD_FREQ_MIN", 50, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_55[] =
{
    { "MOD_FREQ_MAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_56[] =
{
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties234_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "VPD_ID", -1, "VSB/QAM-B receiver", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties234_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtPropertyHashSet  DtPropertyHashSets234[] =
{
    { DtProperties234_00, 1 },
    { DtProperties234_01, 2 },
    { NULL, 0 },
    { DtProperties234_03, 1 },
    { DtProperties234_04, 1 },
    { NULL, 0 },
    { DtProperties234_06, 1 },
    { NULL, 0 },
    { DtProperties234_08, 2 },
    { DtProperties234_09, 1 },
    { DtProperties234_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_24, 1 },
    { DtProperties234_25, 1 },
    { DtProperties234_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_29, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_35, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_39, 1 },
    { DtProperties234_40, 3 },
    { NULL, 0 },
    { DtProperties234_42, 1 },
    { DtProperties234_43, 2 },
    { NULL, 0 },
    { DtProperties234_45, 1 },
    { DtProperties234_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_50, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_53, 1 },
    { NULL, 0 },
    { DtProperties234_55, 1 },
    { DtProperties234_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_59, 2 },
    { DtProperties234_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties234_63, 1 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-235 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// DVB-T measurement receiver
// 
static const DtProperty  DtProperties235_00[] =
{
    { "FIRMWARE_R_N1_0", -1,
      "Bug fix: failure to acquire data-lock => cannot read transport-stream",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_08[] =
{
    { "CAP_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties235_10[] =
{
    { "USB_DEVICE_ID", 0x223, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties235_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_SENCORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_25[] =
{
    { "PCB_LAST_REV", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties235_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties235_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties235_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "MOD_FREQ_ABSMIN", 50, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_45[] =
{
    { "MOD_FREQ_ABSMAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_50[] =
{
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "TESTING_LOOP_BACK_32B", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_53[] =
{
    { "MOD_FREQ_MIN", 50, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_55[] =
{
    { "MOD_FREQ_MAX", 860, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_56[] =
{
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties235_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "VPD_ID", -1, "DVB-T measurement receiver", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties235_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtPropertyHashSet  DtPropertyHashSets235[] =
{
    { DtProperties235_00, 1 },
    { DtProperties235_01, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_04, 1 },
    { NULL, 0 },
    { DtProperties235_06, 1 },
    { NULL, 0 },
    { DtProperties235_08, 3 },
    { DtProperties235_09, 1 },
    { DtProperties235_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_25, 1 },
    { DtProperties235_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_29, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_35, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_39, 1 },
    { DtProperties235_40, 3 },
    { NULL, 0 },
    { DtProperties235_42, 1 },
    { DtProperties235_43, 2 },
    { NULL, 0 },
    { DtProperties235_45, 1 },
    { DtProperties235_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_50, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_53, 1 },
    { NULL, 0 },
    { DtProperties235_55, 1 },
    { DtProperties235_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_59, 2 },
    { DtProperties235_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties235_63, 2 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-236 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// QAM measurement receiver + ASI input
// 
static const DtProperty  DtProperties236_01[] =
{
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_02[] =
{
    { "SUBTYPE1", 0xC80001, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_03[] =
{
    { "CAP_ATSC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_04[] =
{
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_06[] =
{
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties236_10[] =
{
    { "USB_DEVICE_ID", 0x224, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties236_16[] =
{
    { "CAP_DISABLED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "PCB_MANUF", MANUF_PLANT_SENCORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_23[] =
{
    { "CAP_QAM_A", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_24[] =
{
    { "CAP_QAM_B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_25[] =
{
    { "CAP_QAM_C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER },
    { NULL, DT_IOCONFIG_DISABLED, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties236_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties236_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties236_40[] =
{
    { "HARDWARE_HAS_SEC", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties236_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_NOT_SUPP, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "MOD_FREQ_ABSMIN", 44, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 200,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 44, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_45[] =
{
    { "MOD_FREQ_ABSMAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 200,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 885, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "TESTING_LOOP_BACK_32B", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                                PROPERTY_VALUE_TYPE_INT, 1, 0, 0, PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties236_53[] =
{
    { "MOD_FREQ_MIN", 44, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 200,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 44, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_55[] =
{
    { "MOD_FREQ_MAX", 1002, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 200,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 885, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_56[] =
{
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties236_59[] =
{
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "VPD_ID", -1, "QAM measurement receiver + ASI input", PROPERTY_VALUE_TYPE_STRING,
                                                         -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties236_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtPropertyHashSet  DtPropertyHashSets236[] =
{
    { NULL, 0 },
    { DtProperties236_01, 2 },
    { DtProperties236_02, 1 },
    { DtProperties236_03, 1 },
    { DtProperties236_04, 1 },
    { NULL, 0 },
    { DtProperties236_06, 2 },
    { NULL, 0 },
    { DtProperties236_08, 3 },
    { DtProperties236_09, 1 },
    { DtProperties236_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_16, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_19, 2 },
    { NULL, 0 },
    { DtProperties236_21, 1 },
    { NULL, 0 },
    { DtProperties236_23, 1 },
    { DtProperties236_24, 1 },
    { DtProperties236_25, 2 },
    { DtProperties236_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_29, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_32, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_35, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_39, 1 },
    { DtProperties236_40, 3 },
    { NULL, 0 },
    { DtProperties236_42, 1 },
    { DtProperties236_43, 3 },
    { NULL, 0 },
    { DtProperties236_45, 2 },
    { DtProperties236_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_50, 3 },
    { DtProperties236_51, 1 },
    { NULL, 0 },
    { DtProperties236_53, 2 },
    { NULL, 0 },
    { DtProperties236_55, 2 },
    { DtProperties236_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_59, 2 },
    { DtProperties236_60, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties236_63, 3 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-245 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// ASI/SDI input+output
// 
static const DtProperty  DtProperties245_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_04[] =
{
    { "FIRMWARE_R_N5_0", -1, "Support for reception of 10-bit SDI",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_05[] =
{
    { "FIRMWARE_R_N5_1", -1, "Support for Huffman compressed SDI formats",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N6_0", -1, "Internal maintenance update, no functional changes",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_06[] =
{
    { "FIRMWARE_R_N5_2", -1,
      "Bug fix: potential reception/generation of unwanted extra bytes",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N7_0", -1,
      "Bug fix: sometimes fails to lock-on to input signal after a channel reset",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_07[] =
{
    { "FIRMWARE_R_N7_1", -1,
      "Bug fix: 32nd bit of clock-reference-counter was stuck to zero",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N8_0", -1, "Support transparent-packet receive mode",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N7_2", -1,
      "Bug fix: not all bytes in tx-FIFO were reported in FIFO load",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N8_1", -1,
      "Fixes issue with DVB-ASI streams that have a bitrate >= 108Mbps",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N9_0", -1,
      "Fixes issue with stability of arrival timestamps assigned to packets",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REGISTERS_TX", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties245_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1,
                                                            0, 0, PROPERTY_SCOPE_DRIVER },
    { "USB_DEVICE_ID", 0x22D, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties245_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_19[] =
{
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 8, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_25[] =
{
    { "PCB_LAST_REV", 501, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 500,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_32[] =
{
    { "CAP_LOOPTHR", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 7, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties245_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties245_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties245_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                                                   PROPERTY_SCOPE_DTAPI },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties245_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 6, 0,
                                                                   PROPERTY_SCOPE_DTAPI },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 500,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_48[] =
{
    { "CAP_DVBT2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "TESTING_LOOP_BACK_32B", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                                PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties245_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties245_59[] =
{
    { "VPD_ID", -1, "ASI/SDI input+output", PROPERTY_VALUE_TYPE_STRING, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties245_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtPropertyHashSet  DtPropertyHashSets245[] =
{
    { NULL, 0 },
    { DtProperties245_01, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_04, 3 },
    { DtProperties245_05, 2 },
    { DtProperties245_06, 3 },
    { DtProperties245_07, 2 },
    { DtProperties245_08, 6 },
    { DtProperties245_09, 2 },
    { DtProperties245_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_19, 1 },
    { NULL, 0 },
    { DtProperties245_21, 2 },
    { DtProperties245_22, 1 },
    { DtProperties245_23, 2 },
    { NULL, 0 },
    { DtProperties245_25, 1 },
    { DtProperties245_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_29, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_32, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_35, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_39, 1 },
    { DtProperties245_40, 4 },
    { NULL, 0 },
    { DtProperties245_42, 2 },
    { DtProperties245_43, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_46, 3 },
    { NULL, 0 },
    { DtProperties245_48, 1 },
    { NULL, 0 },
    { DtProperties245_50, 3 },
    { DtProperties245_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_56, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_59, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties245_63, 3 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-255 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// DTU-2X5 in manufacturing mode
// 
static const DtProperty  DtProperties255_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_04[] =
{
    { "FIRMWARE_R_N5_0", -1, "Support for reception of 10-bit SDI",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_05[] =
{
    { "FIRMWARE_R_N5_1", -1, "Support for Huffman compressed SDI formats",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N6_0", -1, "Internal maintenance update, no functional changes",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_06[] =
{
    { "FIRMWARE_R_N5_2", -1,
      "Bug fix: potential reception/generation of unwanted extra bytes",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N7_0", -1,
      "Bug fix: sometimes fails to lock-on to input signal after a channel reset",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "REGISTERS_RX", 0x100, NULL, PROPERTY_VALUE_TYPE_UINT16, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_07[] =
{
    { "FIRMWARE_R_N7_1", -1,
      "Bug fix: 32nd bit of clock-reference-counter was stuck to zero",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N8_0", -1, "Support transparent-packet receive mode",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_08[] =
{
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N7_2", -1,
      "Bug fix: not all bytes in tx-FIFO were reported in FIFO load",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N8_1", -1,
      "Fixes issue with DVB-ASI streams that have a bitrate >= 108Mbps",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N9_0", -1,
      "Fixes issue with stability of arrival timestamps assigned to packets",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REGISTERS_TX", 0x80, NULL, PROPERTY_VALUE_TYPE_UINT16, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_09[] =
{
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties255_10[] =
{
    { "ASI_SDI_HW_SER_ITF", ASI_SDI_SER_ITF_FPGA_BASED, NULL, PROPERTY_VALUE_TYPE_INT, 1,
                                                            0, 0, PROPERTY_SCOPE_DRIVER },
    { "USB_DEVICE_ID", 0x237, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties255_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_21[] =
{
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_22[] =
{
    { "CAP_INTTSRATE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_23[] =
{
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_25[] =
{
    { "PCB_LAST_REV", 501, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties255_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties255_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties255_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 6, 0,
                                                                   PROPERTY_SCOPE_DTAPI },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_46[] =
{
    { "HARDWARE_HAS_PQNCO", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_50[] =
{
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "TESTING_LOOP_BACK_32B", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 0, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_51[] =
{
    { "ASI_SDI_HW_DESER_ITF", ASI_SDI_DESER_ITF_FPGA_BASED, NULL,
                                PROPERTY_VALUE_TYPE_INT, 0, 0, 0, PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties255_56[] =
{
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties255_59[] =
{
    { "VPD_ID", -1, "DTU-2X5 in manufacturing mode", PROPERTY_VALUE_TYPE_STRING, -1, 0,
                                                                0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties255_63[] =
{
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtPropertyHashSet  DtPropertyHashSets255[] =
{
    { NULL, 0 },
    { DtProperties255_01, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_04, 2 },
    { DtProperties255_05, 2 },
    { DtProperties255_06, 3 },
    { DtProperties255_07, 2 },
    { DtProperties255_08, 6 },
    { DtProperties255_09, 2 },
    { DtProperties255_10, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_21, 2 },
    { DtProperties255_22, 1 },
    { DtProperties255_23, 2 },
    { NULL, 0 },
    { DtProperties255_25, 1 },
    { DtProperties255_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_29, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_35, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_39, 1 },
    { DtProperties255_40, 3 },
    { NULL, 0 },
    { DtProperties255_42, 2 },
    { DtProperties255_43, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_46, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_50, 3 },
    { DtProperties255_51, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_56, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_59, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties255_63, 3 }
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-299 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// DTU-2X5 in manufacturing mode
// 
static const DtProperty  DtProperties299_01[] =
{
    { "FIRMWARE_LAST_VER", 9, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties299_04[] =
{
    { "FIRMWARE_R_N5_0", -1, "Support for reception of 10-bit SDI",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "HARDWARE_HAS_EXT_CLK_FREQ", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties299_05[] =
{
    { "FIRMWARE_R_N5_1", -1, "Support for Huffman compressed SDI formats",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N6_0", -1, "Internal maintenance update, no functional changes",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties299_06[] =
{
    { "FIRMWARE_R_N5_2", -1,
      "Bug fix: potential reception/generation of unwanted extra bytes",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N7_0", -1,
      "Bug fix: sometimes fails to lock-on to input signal after a channel reset",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties299_07[] =
{
    { "FIRMWARE_R_N7_1", -1,
      "Bug fix: 32nd bit of clock-reference-counter was stuck to zero",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N8_0", -1, "Support transparent-packet receive mode",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties299_08[] =
{
    { "FIRMWARE_R_N7_2", -1,
      "Bug fix: not all bytes in tx-FIFO were reported in FIFO load",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N8_1", -1,
      "Fixes issue with DVB-ASI streams that have a bitrate >= 108Mbps",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "FIRMWARE_R_N9_0", -1,
      "Fixes issue with stability of arrival timestamps assigned to packets",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI },
    { "REF_CLK_FREQ", 54000000, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties299_09[] =
{
    { "USB_END_POINT_READ", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties299_10[] =
{
    { "USB_DEVICE_ID", 0x263, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "USB_END_POINT_FIRMWARE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties299_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties299_25[] =
{
    { "PCB_LAST_REV", 501, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties299_26[] =
{
    { "REF_CLK_WIDTH", 32, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties299_29[] =
{
    { "HARDWARE_I2C_FREQ", -1, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties299_35[] =
{
    { "EEPROM_PAGE_SIZE", 16, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties299_39[] =
{
    { "EEPROM_SIZE", 512, NULL, PROPERTY_VALUE_TYPE_UINT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties299_40[] =
{
    { "HARDWARE_HAS_SEC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI },
    { "USB_VENDOR_ID", 0x1297, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI },
    { "VPD_IO_ITF", VPD_EEPROM_IO_EZUSB, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties299_42[] =
{
    { "PROGRAMMING_ITF", PROG_ITF_JTAG_FAST, NULL, PROPERTY_VALUE_TYPE_INT, -1, 6, 0,
                                                                   PROPERTY_SCOPE_DTAPI },
    { NULL, PROG_ITF_JTAG_NORMAL, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                   PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties299_43[] =
{
    { "HARDWARE_HAS_I2C", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties299_46[] =
{
    { "REF_CLK_SUPPORTS_CNT", 0, NULL, PROPERTY_VALUE_TYPE_BOOL, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties299_50[] =
{
    { "PORT_COUNT", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                           PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties299_56[] =
{
    { "USB_END_POINT_WRITE", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, 0, 0,
                                                                  PROPERTY_SCOPE_DRIVER } 
};
static const DtProperty  DtProperties299_59[] =
{
    { "VPD_ID", -1, "DTU-2X5 in manufacturing mode", PROPERTY_VALUE_TYPE_STRING, -1, 0,
                                                                0, PROPERTY_SCOPE_DTAPI } 
};
static const DtProperty  DtProperties299_63[] =
{
    { "FIRMWARE_R_N0_0", -1, "First firmware version released to the field",
                             PROPERTY_VALUE_TYPE_STRING, -1, 0, 0, PROPERTY_SCOPE_DTAPI } 
};
static const DtPropertyHashSet  DtPropertyHashSets299[] =
{
    { NULL, 0 },
    { DtProperties299_01, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_04, 2 },
    { DtProperties299_05, 2 },
    { DtProperties299_06, 2 },
    { DtProperties299_07, 2 },
    { DtProperties299_08, 4 },
    { DtProperties299_09, 1 },
    { DtProperties299_10, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_25, 1 },
    { DtProperties299_26, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_29, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_35, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_39, 1 },
    { DtProperties299_40, 3 },
    { NULL, 0 },
    { DtProperties299_42, 2 },
    { DtProperties299_43, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_46, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_50, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_56, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_59, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties299_63, 1 }
};


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertyStores -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// References to hash table per device type.
//
const DtPropertyStore  DtPropertyStores[] =
{
    { 205, DtPropertyHashSets205, 64 },
    { 215, DtPropertyHashSets215, 64 },
    { 225, DtPropertyHashSets225, 64 },
    { 234, DtPropertyHashSets234, 64 },
    { 235, DtPropertyHashSets235, 64 },
    { 236, DtPropertyHashSets236, 64 },
    { 245, DtPropertyHashSets245, 64 },
    { 255, DtPropertyHashSets255, 64 },
    { 299, DtPropertyHashSets299, 64 }
};
const Int  DtPropertyStoreCount = 9;

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
/*  7 */ "FAILSAFE",
/*  8 */ "FRACMODE",
/*  9 */ "GENLOCKED",
/* 10 */ "GENREF",
/* 11 */ "SWS2APSK",
/* 12 */ "TRUE",
/* 13 */ "FALSE",
/* 14 */ "DISABLED",
/* 15 */ "INPUT",
/* 16 */ "OUTPUT",
/* 17 */ "SHAREDANT",
/* 18 */ "DBLBUF",
/* 19 */ "LOOPS2L3",
/* 20 */ "LOOPS2TS",
/* 21 */ "LOOPTHR",
/* 22 */ "3GSDI",
/* 23 */ "ASI",
/* 24 */ "DEMOD",
/* 25 */ "HDSDI",
/* 26 */ "IFADC",
/* 27 */ "IP",
/* 28 */ "MOD",
/* 29 */ "SDI",
/* 30 */ "SPI",
/* 31 */ "SPISDI",
/* 32 */ "1080P50",
/* 33 */ "1080P59_94",
/* 34 */ "1080P60",
/* 35 */ "1080I50",
/* 36 */ "1080I59_94",
/* 37 */ "1080I60",
/* 38 */ "1080P23_98",
/* 39 */ "1080P24",
/* 40 */ "1080P25",
/* 41 */ "1080P29_97",
/* 42 */ "1080P30",
/* 43 */ "720P23_98",
/* 44 */ "720P24",
/* 45 */ "720P25",
/* 46 */ "720P29_97",
/* 47 */ "720P30",
/* 48 */ "720P50",
/* 49 */ "720P59_94",
/* 50 */ "720P60",
/* 51 */ "525I59_94",
/* 52 */ "625I50",
/* 53 */ "SPI525I59_94",
/* 54 */ "SPI625I50",
/* 55 */ "RFCLKEXT",
/* 56 */ "RFCLKINT",
/* 57 */ "SPICLKEXT",
/* 58 */ "SPICLKINT",
/* 59 */ "SPIFIXEDCLK",
/* 60 */ "SPIDVBMODE",
/* 61 */ "SPISER8B",
/* 62 */ "SPISER10B",
/* 63 */ "SPILVDS1",
/* 64 */ "SPILVDS2",
/* 65 */ "SPILVTTL",
/* 66 */ "EXTTSRATE",
/* 67 */ "EXTRATIO",
/* 68 */ "INTTSRATE",
/* 69 */ "LOCK2INP"
};
const Int  IoConfigCodeStringCount = 70;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IoConfigCodes_XX[] -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Helper arrays to map from DT_IOCONFIG name to DT_IOCONFIG code.
// The hash of the DT_IOCONFIG names in IoConfigCodes_XX[] is XX.
//
static const IoConfigCode  IoConfigCodes_0[] =
{
    { "FAILSAFE", 7 }
};
static const IoConfigCode  IoConfigCodes_1[] =
{
    { "1080P30", 42 }
};
static const IoConfigCode  IoConfigCodes_3[] =
{
    { "LOOPS2L3", 19 },
    { "1080P50", 32 },
    { "720P23_98", 43 }
};
static const IoConfigCode  IoConfigCodes_5[] =
{
    { "SDI", 29 }
};
static const IoConfigCode  IoConfigCodes_6[] =
{
    { "FRACMODE", 8 }
};
static const IoConfigCode  IoConfigCodes_7[] =
{
    { "SPILVTTL", 65 }
};
static const IoConfigCode  IoConfigCodes_8[] =
{
    { "IOSTD", 1 },
    { "720P29_97", 46 },
    { "720P59_94", 49 }
};
static const IoConfigCode  IoConfigCodes_14[] =
{
    { "DEMOD", 24 }
};
static const IoConfigCode  IoConfigCodes_16[] =
{
    { "FALSE", 13 },
    { "625I50", 52 }
};
static const IoConfigCode  IoConfigCodes_17[] =
{
    { "GENLOCKED", 9 },
    { "HDSDI", 25 },
    { "SPI", 30 },
    { "SPISDI", 31 },
    { "1080I59_94", 36 },
    { "720P30", 47 }
};
static const IoConfigCode  IoConfigCodes_18[] =
{
    { "SPIDVBMODE", 60 }
};
static const IoConfigCode  IoConfigCodes_19[] =
{
    { "720P50", 48 }
};
static const IoConfigCode  IoConfigCodes_20[] =
{
    { "DBLBUF", 18 }
};
static const IoConfigCode  IoConfigCodes_21[] =
{
    { "INPUT", 15 },
    { "EXTRATIO", 67 }
};
static const IoConfigCode  IoConfigCodes_22[] =
{
    { "SPIMODE", 4 },
    { "SPICLKINT", 58 }
};
static const IoConfigCode  IoConfigCodes_27[] =
{
    { "SPIFIXEDCLK", 59 }
};
static const IoConfigCode  IoConfigCodes_28[] =
{
    { "IODIR", 0 },
    { "SPISTD", 5 },
    { "TSRATESEL", 6 },
    { "SPI625I50", 54 },
    { "SPICLKEXT", 57 }
};
static const IoConfigCode  IoConfigCodes_29[] =
{
    { "1080I60", 37 }
};
static const IoConfigCode  IoConfigCodes_31[] =
{
    { "3GSDI", 22 }
};
static const IoConfigCode  IoConfigCodes_34[] =
{
    { "ASI", 23 },
    { "RFCLKINT", 56 }
};
static const IoConfigCode  IoConfigCodes_35[] =
{
    { "SWS2APSK", 11 },
    { "INTTSRATE", 68 }
};
static const IoConfigCode  IoConfigCodes_36[] =
{
    { "1080P60", 34 },
    { "1080P24", 39 },
    { "525I59_94", 51 }
};
static const IoConfigCode  IoConfigCodes_37[] =
{
    { "TRUE", 12 },
    { "MOD", 28 },
    { "1080P25", 40 }
};
static const IoConfigCode  IoConfigCodes_39[] =
{
    { "LOCK2INP", 69 }
};
static const IoConfigCode  IoConfigCodes_40[] =
{
    { "RFCLKEXT", 55 }
};
static const IoConfigCode  IoConfigCodes_41[] =
{
    { "EXTTSRATE", 66 }
};
static const IoConfigCode  IoConfigCodes_43[] =
{
    { "LOOPS2TS", 20 }
};
static const IoConfigCode  IoConfigCodes_45[] =
{
    { "LOOPTHR", 21 }
};
static const IoConfigCode  IoConfigCodes_47[] =
{
    { "SPICLKSEL", 3 }
};
static const IoConfigCode  IoConfigCodes_48[] =
{
    { "SPI525I59_94", 53 }
};
static const IoConfigCode  IoConfigCodes_51[] =
{
    { "1080P23_98", 38 }
};
static const IoConfigCode  IoConfigCodes_52[] =
{
    { "720P24", 44 },
    { "720P60", 50 }
};
static const IoConfigCode  IoConfigCodes_53[] =
{
    { "720P25", 45 },
    { "SPISER8B", 61 }
};
static const IoConfigCode  IoConfigCodes_54[] =
{
    { "OUTPUT", 16 }
};
static const IoConfigCode  IoConfigCodes_56[] =
{
    { "1080P59_94", 33 },
    { "1080P29_97", 41 }
};
static const IoConfigCode  IoConfigCodes_59[] =
{
    { "RFCLKSEL", 2 },
    { "SPILVDS1", 63 }
};
static const IoConfigCode  IoConfigCodes_60[] =
{
    { "GENREF", 10 },
    { "IFADC", 26 },
    { "1080I50", 35 },
    { "SPILVDS2", 64 }
};
static const IoConfigCode  IoConfigCodes_61[] =
{
    { "DISABLED", 14 }
};
static const IoConfigCode  IoConfigCodes_62[] =
{
    { "IP", 27 },
    { "SPISER10B", 62 }
};
static const IoConfigCode  IoConfigCodes_63[] =
{
    { "SHAREDANT", 17 }
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
    { NULL, 0 },
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
