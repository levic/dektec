//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPciePropertyStore.c *#*#*#*#*#*#*#*#* (C) 2019 DekTec
//
// CapParser - Property Store - Tables storing device properties
//
// THIS FILE IS GENERATED, DO NOT EDIT!!!

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#include "DtPcieIncludes.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2100; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// HDMI 2.0b Output
// 
static const DtProperty  DtProperties2100_0_00[] =
{
    { "AF_REBOOT#1_UUID", 0x00020001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_01[] =
{
    { "BC_SPIMF#1_ADDRESS", 0x140, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1_UUID", 0x00000000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_02[] =
{
    { "DF_VPD#1_UUID", 0x00040002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_03[] =
{
    { "BC_MSIX#1_ADDRESS", 0x400, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_04[] =
{
    { "BC_TOD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_HDSDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_06[] =
{
    { "CAP_1080P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_07[] =
{
    { "CAP_720P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_08[] =
{
    { "BC_REBOOT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1_UUID", 0x00010000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_720P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_09[] =
{
    { "AF_REBOOT#1.1", DT_PVE, "BC_REBOOT#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1_TYPE", DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_10[] =
{
    { "BC_LEDB#1_UUID", 0x00010003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_11[] =
{
    { "AF_REBOOT#1_TYPE", DT_FUNC_TYPE_REBOOT, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1.1", DT_PVE, "DF_SPIPROM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_12[] =
{
    { "DEFAULT_IOSTD", DT_IOCONFIG_1080I50, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_CORE#1_TYPE", DT_FUNC_TYPE_CORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPIPROM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_13[] =
{
    { "BC_I2CM#1_ADDRESS", 0xa00, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_VPD#1_TYPE", DT_FUNC_TYPE_VPD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_14[] =
{
    { "BC_I2CM#2_ADDRESS", 0xb00, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_16[] =
{
    { "AF_HDMI20BTX#1.1", DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080I60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_17[] =
{
    { "AF_HDMI20BTX#1.2", DT_PVE, "BC_SDITXF#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_18[] =
{
    { "AF_HDMI20BTX#1.3", DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_3GSDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_19[] =
{
    { "AF_HDMI20BTX#1.4", DT_PVE, "BC_I2CM#2", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1_TYPE", DT_BLOCK_TYPE_VVI, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_20[] =
{
    { "AF_SPIMF#1_UUID", 0x00020002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXF#1_ADDRESS", 0x940, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_21[] =
{
    { "BC_LEDB#1_TYPE", DT_BLOCK_TYPE_LEDB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_22[] =
{
    { "BC_SPIM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_23[] =
{
    { "BC_VVI#1_ADDRESS", 0x000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_24[] =
{
    { "CAP_1080P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_25[] =
{
    { "BC_LEDB#1_ADDRESS", 0x0C0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_LAST_REV", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_26[] =
{
    { "BC_CDMAC#1_ADDRESS", 0x880, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TOD#1_UUID", 0x00010002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_28[] =
{
    { "AF_LEDBOARD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIMF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_29[] =
{
    { "AF_SPIMF#1.1", DT_PVE, "BC_SPIMF#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_30[] =
{
    { "BC_MSIX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_REBOOT#1_UUID", 0x00010001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_31[] =
{
    { "AF_SPIMF#1_TYPE", DT_FUNC_TYPE_SPIMF, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#2_UUID", 0x0001000A, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_34[] =
{
    { "DF_SPIPROM#1_UUID", 0x00040001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_35[] =
{
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_36[] =
{
    { "CAP_1080I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_37[] =
{
    { "BC_SDITXF#1_UUID", 0x00010008, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TOD#1_TYPE", DT_BLOCK_TYPE_TOD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_38[] =
{
    { "CAP_720P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_VPD#1_RO_SIZE", 768, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_39[] =
{
    { "AF_HDMI20BTX#1_UUID", 0x00020003, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_40[] =
{
    { "BC_I2CM#1", DT_PVE, "CONFIG", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_41[] =
{
    { "BC_I2CM#2", DT_PVE, "DDC", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_REBOOT#1_TYPE", DT_BLOCK_TYPE_REBOOT, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TOD#1_ADDRESS", 0x080, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_42[] =
{
    { "AF_REBOOT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#2_TYPE", DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.1", DT_PVE, "BC_VVI#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_43[] =
{
    { "BC_CDMAC#1_UUID", 0x00010007, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.2", DT_PVE, "BC_TOD#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPIPROM#1.1", DT_PVE, "BC_SPIM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_44[] =
{
    { "BC_SPIM#1_UUID", 0x00010004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_CORE#1.3", DT_PVE, "BC_MSIX#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_45[] =
{
    { "BC_REBOOT#1_ADDRESS", 0x040, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P59_94B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.4", DT_PVE, "DF_VPD#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPIPROM#1_TYPE", DT_FUNC_TYPE_SPIPROM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_46[] =
{
    { "DF_VPD#1_RW_SIZE", 1024, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_47[] =
{
    { "BC_SDITXF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_48[] =
{
    { "BC_SDITXF#1_TYPE", DT_BLOCK_TYPE_SDITXF, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_49[] =
{
    { "AF_HDMI20BTX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_50[] =
{
    { "AF_HDMI20BTX#1_TYPE", DT_FUNC_TYPE_HDMI20BTX, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_LEDBOARD#1_UUID", 0x00020000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIMF#1_UUID", 0x00010005, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_52[] =
{
    { "BC_LEDB#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_MSIX#1_UUID", 0x00010006, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_53[] =
{
    { "BC_CDMAC#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_54[] =
{
    { "BC_CDMAC#1_TYPE", DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MAIN_PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_55[] =
{
    { "BC_SPIM#1_TYPE", DT_BLOCK_TYPE_SPIM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_56[] =
{
    { "CAP_1080P50B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_57[] =
{
    { "CAP_1080P60B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_59[] =
{
    { "AF_LEDBOARD#1.1", DT_PVE, "BC_LEDB#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIM#1_ADDRESS", 0x100, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_720P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", DT_PVE, "HDMI 2.0b Output", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_60[] =
{
    { "PCI_DEVICE_ID", 0x834, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_61[] =
{
    { "AF_LEDBOARD#1_TYPE", DT_FUNC_TYPE_LEDBOARD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIMF#1_TYPE", DT_BLOCK_TYPE_SPIMF, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_62[] =
{
    { "AF_SPIMF#1", DT_PVE, "FPGA_FIRMWARE", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1_UUID", 0x00010009, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2100_0_63[] =
{
    { "BC_MSIX#1_TYPE", DT_BLOCK_TYPE_MSIX, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_MATRIX2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2100_0[] =
{
    { DtProperties2100_0_00, 2 },
    { DtProperties2100_0_01, 3 },
    { DtProperties2100_0_02, 1 },
    { DtProperties2100_0_03, 1 },
    { DtProperties2100_0_04, 2 },
    { NULL, 0 },
    { DtProperties2100_0_06, 1 },
    { DtProperties2100_0_07, 2 },
    { DtProperties2100_0_08, 4 },
    { DtProperties2100_0_09, 2 },
    { DtProperties2100_0_10, 1 },
    { DtProperties2100_0_11, 5 },
    { DtProperties2100_0_12, 3 },
    { DtProperties2100_0_13, 2 },
    { DtProperties2100_0_14, 1 },
    { NULL, 0 },
    { DtProperties2100_0_16, 4 },
    { DtProperties2100_0_17, 2 },
    { DtProperties2100_0_18, 2 },
    { DtProperties2100_0_19, 2 },
    { DtProperties2100_0_20, 3 },
    { DtProperties2100_0_21, 2 },
    { DtProperties2100_0_22, 1 },
    { DtProperties2100_0_23, 4 },
    { DtProperties2100_0_24, 1 },
    { DtProperties2100_0_25, 2 },
    { DtProperties2100_0_26, 4 },
    { NULL, 0 },
    { DtProperties2100_0_28, 2 },
    { DtProperties2100_0_29, 1 },
    { DtProperties2100_0_30, 2 },
    { DtProperties2100_0_31, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2100_0_34, 1 },
    { DtProperties2100_0_35, 2 },
    { DtProperties2100_0_36, 3 },
    { DtProperties2100_0_37, 2 },
    { DtProperties2100_0_38, 2 },
    { DtProperties2100_0_39, 1 },
    { DtProperties2100_0_40, 1 },
    { DtProperties2100_0_41, 3 },
    { DtProperties2100_0_42, 3 },
    { DtProperties2100_0_43, 3 },
    { DtProperties2100_0_44, 3 },
    { DtProperties2100_0_45, 5 },
    { DtProperties2100_0_46, 2 },
    { DtProperties2100_0_47, 2 },
    { DtProperties2100_0_48, 1 },
    { DtProperties2100_0_49, 1 },
    { DtProperties2100_0_50, 5 },
    { NULL, 0 },
    { DtProperties2100_0_52, 3 },
    { DtProperties2100_0_53, 1 },
    { DtProperties2100_0_54, 4 },
    { DtProperties2100_0_55, 1 },
    { DtProperties2100_0_56, 2 },
    { DtProperties2100_0_57, 1 },
    { NULL, 0 },
    { DtProperties2100_0_59, 5 },
    { DtProperties2100_0_60, 1 },
    { DtProperties2100_0_61, 2 },
    { DtProperties2100_0_62, 2 },
    { DtProperties2100_0_63, 2 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2132; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// High-End Satellite Receiver
// 
static const DtProperty  DtProperties2132_0_00[] =
{
    { "AF_REBOOT#1_UUID", 0x00020003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#3_UUID", 0x00010014, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_01[] =
{
    { "AF_I2CM#2_UUID", 0x0002000B, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIMF#1_ADDRESS", 0x100, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1_UUID", 0x00000000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_02[] =
{
    { "BC_ACCUFIFO#1_ADDRESS", 0x1A0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_AD5320#1_UUID", 0x00010015, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RX_DVBS2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_VPD#1_UUID", 0x00040004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_03[] =
{
    { "BC_MSIX#1_ADDRESS", 0x140, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_04[] =
{
    { "BC_S2DEC#1_UUID", 0x0001001A, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TOD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_05[] =
{
    { "BC_SWITCH#1_UUID", 0x00010008, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_S2CRDEMOD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_06[] =
{
    { "DF_S2CRDEMOD#1_TYPE", DT_FUNC_TYPE_S2CRDEMOD_2132, NULL, PROPERTY_VALUE_TYPE_INT,
                 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_07[] =
{
    { "AF_ADS4246#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_DMA#1_UUID", 0x00020009, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020014, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IQSRC2132#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_S2STATS#1_UUID", 0x00010011, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_08[] =
{
    { "AF_ADS4246#1_TYPE", DT_FUNC_TYPE_ADS4246_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_AFS2CRDEMOD#1.1", DT_PVE, "DF_S2CRDEMOD#1", PROPERTY_VALUE_TYPE_STRING, 0, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IQPWR#1_UUID", 0x0002000F, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IQSRC2132#1.1", DT_PVE, "BC_IQSRC2132#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IQSRC2132#1_TYPE", DT_BLOCK_TYPE_IQSRC2132_2132, NULL, PROPERTY_VALUE_TYPE_INT,
                 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_REBOOT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1_UUID", 0x00010000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_09[] =
{
    { "AF_REBOOT#1.1", DT_PVE, "BC_REBOOT#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1_TYPE", DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LNBH25#1_TYPE", DT_BLOCK_TYPE_LNBH25_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_10[] =
{
    { "AF_I2CM#1", DT_PVE, "MAX2121B", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_LNBH25#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ADS4246#1_ADDRESS", 0x200, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LEDB#1_UUID", 0x00010002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_11[] =
{
    { "AF_I2CM#2", DT_PVE, "CDCM6208", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_REBOOT#1_TYPE", DT_FUNC_TYPE_REBOOT, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#3_TYPE", DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1.1", DT_PVE, "DF_SPIPROM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_12[] =
{
    { "AF_I2CM#2_TYPE", DT_FUNC_TYPE_I2CM_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_I2CM#3", DT_PVE, "LNBH25", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_TEMPFANMGR_AF#1_UUID", 0x00020005, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_ROLLOFF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1_TYPE", DT_FUNC_TYPE_CORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPIPROM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_13[] =
{
    { "BC_AD5320#1_TYPE", DT_BLOCK_TYPE_AD5320_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1_ADDRESS", 0x240, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LNBH25#1_ADDRESS", 0x400, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_VPD#1_TYPE", DT_FUNC_TYPE_VPD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_14[] =
{
    { "AF_AD5320#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IQCGRAB#1_UUID", 0x0002000D, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_SPIM#1_UUID", 0x00020013, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#2_ADDRESS", 0x280, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IPSECG#1_UUID", 0x00010004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IQFIR#1_UUID", 0x00010012, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_15[] =
{
    { "BC_I2CM#3_ADDRESS", 0x440, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_S2DEC#1_TYPE", DT_BLOCK_TYPE_S2DEC_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#1", DT_PVE, "DMA_ACCU_SWITCH", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_16[] =
{
    { "BC_IQPWR#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_S2DEMOD#1_ADDRESS", 0x4000, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#1_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SENSTEMP#1.1", DT_PVE, "BC_FPGATEMP#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_17[] =
{
    { "AF_DMA#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IQPWR#1.1", DT_PVE, "BC_IQPWR#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_AD5320#1_ADDRESS", 0x480, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_S2STATS#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_18[] =
{
    { "AF_DMA#1_TYPE", DT_FUNC_TYPE_DMA, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_DMA, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_S2STATS#1.1", DT_PVE, "BC_S2STATS#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_S2STATS#1_TYPE", DT_BLOCK_TYPE_S2STATS_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_TEMPFANMGR#1.1", DT_PVE, "BC_FANC#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_19[] =
{
    { "AF_IQPWR#1_TYPE", DT_FUNC_TYPE_IQPWR_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ACCUFIFO#1_UUID", 0x00010009, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_S2DEC#1_ADDRESS", 0x2000, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_VVI#1_TYPE", DT_BLOCK_TYPE_VVI, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_TEMPFANMGR#1.2", DT_PVE, "DF_SENSTEMP#1", PROPERTY_VALUE_TYPE_STRING, -1, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_20[] =
{
    { "AF_SPIMF#1_UUID", 0x00020004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_21[] =
{
    { "AF_TEMPFANMGR_AF#1.1", DT_PVE, "DF_TEMPFANMGR#1", PROPERTY_VALUE_TYPE_STRING, -1,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LEDB#1_TYPE", DT_BLOCK_TYPE_LEDB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_22[] =
{
    { "BC_BURSTFIFO#1_ADDRESS", 0x520, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x5C0, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FPGATEMP#1_ADDRESS", 0x1D0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_IQCGRAB#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_23[] =
{
    { "AF_IQCGRAB#1.1", DT_PVE, "BC_IQCGRAB#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_SPIM#1.1", DT_PVE, "BC_SPIM#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_TEMPFANMGR_AF#1_TYPE", DT_FUNC_TYPE_TEMPFANMGR_AF, NULL,
               PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1_ADDRESS", 0x000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_25[] =
{
    { "AF_IQCGRAB#1_TYPE", DT_FUNC_TYPE_IQCGRAB_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_SPIM#1_TYPE", DT_FUNC_TYPE_SPIM_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IPSECG#1_TYPE", DT_BLOCK_TYPE_IPSECG, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IQFIR#1_TYPE", DT_BLOCK_TYPE_IQFIR_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LEDB#1_ADDRESS", 0x030, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_LAST_REV", 100, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_26[] =
{
    { "AF_DMAPERFACCU#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IQFIR#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CDMAC#1_ADDRESS", 0x540, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x580, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FANC#1_ADDRESS", 0x1C0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TOD#1_UUID", 0x00010003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_27[] =
{
    { "BC_ADS4246#1_UUID", 0x0001000C, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RX_IQ", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2132_0_28[] =
{
    { "AF_LEDBOARD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIMF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_L3MODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_29[] =
{
    { "AF_SPIMF#1.1", DT_PVE, "BC_SPIMF#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ACCUFIFO#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IPSECG#1_ADDRESS", 0x080, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IQFIR#1_ADDRESS", 0x380, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_30[] =
{
    { "BC_ACCUFIFO#1_TYPE", DT_BLOCK_TYPE_ACCUFIFO, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_MSIX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_REBOOT#1_UUID", 0x00010001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCIE_REQUIRED_BW", 7464, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_31[] =
{
    { "AF_AFS2CRDEMOD#1_UUID", 0x00020008, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IQSRC2132#1_UUID", 0x00020010, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_SPIMF#1_TYPE", DT_FUNC_TYPE_SPIMF, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#2_UUID", 0x0001000E, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_32[] =
{
    { "AF_I2CM#1_UUID", 0x0002000A, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_LNBH25#1_UUID", 0x00020011, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_33[] =
{
    { "BC_S2DEMOD#1_UUID", 0x0001001B, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_34[] =
{
    { "AF_I2CM#3_UUID", 0x0002000C, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SPIPROM#1_UUID", 0x00040002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_35[] =
{
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_36[] =
{
    { "AF_AD5320#1_UUID", 0x00020006, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_S2CRDEMOD#1.1", DT_PVE, "BC_S2DEC#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_37[] =
{
    { "BC_ADS4246#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TOD#1_TYPE", DT_BLOCK_TYPE_TOD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_S2CRDEMOD#1.2", DT_PVE, "BC_S2DEMOD#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_38[] =
{
    { "AF_ADS4246#1.1", DT_PVE, "BC_ADS4246#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ADS4246#1_TYPE", DT_BLOCK_TYPE_ADS4246_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IQPWR#1_UUID", 0x00010017, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_VPD#1_RO_SIZE", 768, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_39[] =
{
    { "BC_BURSTFIFO#1_UUID", 0x00010018, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001001D, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FPGATEMP#1_UUID", 0x0001000B, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_UUID", 0x00040001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_40[] =
{
    { "BC_I2CM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LNBH25#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_41[] =
{
    { "AF_AFS2CRDEMOD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_I2CM#1.1", DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IQSRC2132#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_LNBH25#1.1", DT_PVE, "BC_LNBH25#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_S2STATS#1_UUID", 0x00020012, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#2", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_REBOOT#1_TYPE", DT_BLOCK_TYPE_REBOOT, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TOD#1_ADDRESS", 0x040, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_TEMPFANMGR#1_UUID", 0x00040003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_42[] =
{
    { "AF_AFS2CRDEMOD#1_TYPE", DT_FUNC_TYPE_AFS2CRDEMOD_2132, NULL,
                PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_I2CM#2.1", DT_PVE, "BC_I2CM#2", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IQSRC2132#1_TYPE", DT_FUNC_TYPE_IQSRC2132_2132, NULL, PROPERTY_VALUE_TYPE_INT,
                                         0, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_REBOOT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#2_TYPE", DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#3", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.1", DT_PVE, "BC_VVI#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_43[] =
{
    { "AF_I2CM#1_TYPE", DT_FUNC_TYPE_I2CM_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_I2CM#3.1", DT_PVE, "BC_I2CM#3", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_LNBH25#1_TYPE", DT_FUNC_TYPE_LNBH25_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CDMAC#1_UUID", 0x00010019, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001001C, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FANC#1_UUID", 0x0001000A, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_S2DEMOD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_CORE#1.2", DT_PVE, "BC_TOD#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_MAX_TEMP", 85, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPIPROM#1.1", DT_PVE, "BC_SPIM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_44[] =
{
    { "BC_AD5320#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IQCGRAB#1_UUID", 0x00010010, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IQSRC2132#1_ADDRESS", 0x4C0, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_S2DEMOD#1_TYPE", DT_BLOCK_TYPE_S2DEMOD_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SPIM#1_UUID", 0x00010005, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001000F, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.3", DT_PVE, "BC_MSIX#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_TARGET_TEMP", 55, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_45[] =
{
    { "AF_AD5320#1.1", DT_PVE, "BC_AD5320#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_I2CM#3_TYPE", DT_FUNC_TYPE_I2CM_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_REBOOT#1_ADDRESS", 0x020, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.4", DT_PVE, "DF_VPD#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPIPROM#1_TYPE", DT_FUNC_TYPE_SPIPROM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_46[] =
{
    { "BC_S2DEC#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1_RW_SIZE", 1024, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_47[] =
{
    { "AF_AD5320#1_TYPE", DT_FUNC_TYPE_AD5320_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N1_0", DT_PVE, "Fix for missing statistic info",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_48[] =
{
    { "AF_DMA#1.1", DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_DMAPERFACCU#1_UUID", 0x00020000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1_UUID", 0x00020001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IQFIR#1_UUID", 0x0002000E, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_49[] =
{
    { "AF_DMA#1.2", DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_BURSTFIFO#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FPGATEMP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_IQPWR#1_TYPE", DT_BLOCK_TYPE_IQPWR_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SENSTEMP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_NAME", DT_PVE, "FPGA", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_50[] =
{
    { "AF_IQPWR#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_LEDBOARD#1_UUID", 0x00020002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_BURSTFIFO#1_TYPE", DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FPGATEMP#1_TYPE", DT_BLOCK_TYPE_FPGATEMP, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SPIMF#1_UUID", 0x00010006, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_TYPE", DT_FUNC_TYPE_SENSTEMP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_51[] =
{
    { "AF_S2STATS#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_TEMPFANMGR#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_52[] =
{
    { "AF_S2STATS#1_TYPE", DT_FUNC_TYPE_S2STATS_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LEDB#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_MSIX#1_UUID", 0x00010007, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#1_ADDRESS", 0x180, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_LNB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_TEMPFANMGR#1_TYPE", DT_FUNC_TYPE_TEMPFANMGR, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_53[] =
{
    { "BC_CDMAC#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FANC#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_IQPWR#1_ADDRESS", 0x500, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_54[] =
{
    { "AF_TEMPFANMGR_AF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CDMAC#1_TYPE", DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FANC#1_TYPE", DT_BLOCK_TYPE_FANC, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_S2STATS#1_ADDRESS", 0x340, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MAIN_PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_55[] =
{
    { "BC_IQCGRAB#1_TYPE", DT_BLOCK_TYPE_IQCGRAB_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIM#1_TYPE", DT_BLOCK_TYPE_SPIM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SPIM, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_56[] =
{
    { "AF_IQCGRAB#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_SPIM#1", DT_PVE, "ADS4246", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IPSECG#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IQFIR#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_57[] =
{
    { "AF_DMAPERFACCU#1.1", DT_PVE, "BC_ACCUFIFO#1", PROPERTY_VALUE_TYPE_STRING, -1, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1.1", DT_PVE, "BC_IPSECG#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IQFIR#1.1", DT_PVE, "BC_IQFIR#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_LBAND", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_58[] =
{
    { "AF_DMAPERFACCU#1.2", DT_PVE, "BC_SWITCH#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_TEMPFANMGR#1_MIN_FAN_SPEED", 40, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_59[] =
{
    { "AF_DMAPERFACCU#1_TYPE", DT_FUNC_TYPE_DMAPERFACCU, NULL, PROPERTY_VALUE_TYPE_INT,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1_TYPE", DT_FUNC_TYPE_IPSECG, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IQFIR#1_TYPE", DT_FUNC_TYPE_IQFIR_2132, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_LEDBOARD#1.1", DT_PVE, "BC_LEDB#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IQCGRAB#1_ADDRESS", 0x300, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIM#1_ADDRESS", 0x0C0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x2C0, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_S2CRDEMOD#1_UUID", 0x00040005, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", DT_PVE, "High-End Satellite Receiver", PROPERTY_VALUE_TYPE_STRING, -1,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_60[] =
{
    { "PCI_DEVICE_ID", 0x854, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_61[] =
{
    { "AF_ADS4246#1_UUID", 0x00020007, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_LEDBOARD#1_TYPE", DT_FUNC_TYPE_LEDBOARD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IQSRC2132#1_UUID", 0x00010016, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIMF#1_TYPE", DT_BLOCK_TYPE_SPIMF, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_BUILD_DATE_0", DT_PVE, "2019.06.17T14:19",
      PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_62[] =
{
    { "AF_SPIMF#1", DT_PVE, "FPGA_FIRMWARE", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1_UUID", 0x0001000D, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LNBH25#1_UUID", 0x00010013, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_BUILD_DATE_1", DT_PVE, "2019.07.09T10:40",
      PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2132_0_63[] =
{
    { "BC_MSIX#1_TYPE", DT_BLOCK_TYPE_MSIX, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_VIRTUAL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2132_0[] =
{
    { DtProperties2132_0_00, 2 },
    { DtProperties2132_0_01, 6 },
    { DtProperties2132_0_02, 4 },
    { DtProperties2132_0_03, 1 },
    { DtProperties2132_0_04, 2 },
    { DtProperties2132_0_05, 2 },
    { DtProperties2132_0_06, 1 },
    { DtProperties2132_0_07, 5 },
    { DtProperties2132_0_08, 9 },
    { DtProperties2132_0_09, 3 },
    { DtProperties2132_0_10, 4 },
    { DtProperties2132_0_11, 5 },
    { DtProperties2132_0_12, 6 },
    { DtProperties2132_0_13, 4 },
    { DtProperties2132_0_14, 6 },
    { DtProperties2132_0_15, 3 },
    { DtProperties2132_0_16, 5 },
    { DtProperties2132_0_17, 5 },
    { DtProperties2132_0_18, 5 },
    { DtProperties2132_0_19, 6 },
    { DtProperties2132_0_20, 2 },
    { DtProperties2132_0_21, 2 },
    { DtProperties2132_0_22, 6 },
    { DtProperties2132_0_23, 4 },
    { NULL, 0 },
    { DtProperties2132_0_25, 6 },
    { DtProperties2132_0_26, 9 },
    { DtProperties2132_0_27, 3 },
    { DtProperties2132_0_28, 3 },
    { DtProperties2132_0_29, 4 },
    { DtProperties2132_0_30, 4 },
    { DtProperties2132_0_31, 4 },
    { DtProperties2132_0_32, 2 },
    { DtProperties2132_0_33, 1 },
    { DtProperties2132_0_34, 2 },
    { DtProperties2132_0_35, 1 },
    { DtProperties2132_0_36, 2 },
    { DtProperties2132_0_37, 3 },
    { DtProperties2132_0_38, 4 },
    { DtProperties2132_0_39, 4 },
    { DtProperties2132_0_40, 2 },
    { DtProperties2132_0_41, 9 },
    { DtProperties2132_0_42, 7 },
    { DtProperties2132_0_43, 10 },
    { DtProperties2132_0_44, 9 },
    { DtProperties2132_0_45, 5 },
    { DtProperties2132_0_46, 3 },
    { DtProperties2132_0_47, 2 },
    { DtProperties2132_0_48, 5 },
    { DtProperties2132_0_49, 8 },
    { DtProperties2132_0_50, 9 },
    { DtProperties2132_0_51, 2 },
    { DtProperties2132_0_52, 6 },
    { DtProperties2132_0_53, 4 },
    { DtProperties2132_0_54, 6 },
    { DtProperties2132_0_55, 3 },
    { DtProperties2132_0_56, 4 },
    { DtProperties2132_0_57, 4 },
    { DtProperties2132_0_58, 2 },
    { DtProperties2132_0_59, 9 },
    { DtProperties2132_0_60, 1 },
    { DtProperties2132_0_61, 5 },
    { DtProperties2132_0_62, 4 },
    { DtProperties2132_0_63, 3 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2139; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Twelve-channel Cable/Terrestrial Receiver
// 
static const DtProperty  DtProperties2139_0_00[] =
{
    { "AF_REBOOT#1_UUID", 0x00020003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_MXDS75TEMP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_01[] =
{
    { "BC_SPIMF#1_ADDRESS", 0x100, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_DEMOD", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RX_QAMA", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "DF_CORE#1_UUID", 0x00000000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_02[] =
{
    { "BC_ACCUFIFO#1_ADDRESS", 0x1A0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_RX_QAMB", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "DF_VPD#1_UUID", 0x00040005, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_03[] =
{
    { "BC_MSIX#1_ADDRESS", 0x140, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_RX_QAMC", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2139_0_04[] =
{
    { "BC_TOD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_05[] =
{
    { "BC_SWITCH#1_UUID", 0x00010008, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_06[] =
{
    { "AF_TSTRPRX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 4, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 5, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 6, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 7, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 8, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 9, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 10, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 11, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_08[] =
{
    { "BC_REBOOT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1_UUID", 0x00010000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_09[] =
{
    { "AF_REBOOT#1.1", DT_PVE, "BC_REBOOT#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1_TYPE", DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_10[] =
{
    { "AF_I2CM#1", DT_PVE, "CXD2856", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "CXD2856", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "CXD2856", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "CXD2856", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "CXD2856", PROPERTY_VALUE_TYPE_STRING, 4, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "CXD2856", PROPERTY_VALUE_TYPE_STRING, 5, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "CXD2856", PROPERTY_VALUE_TYPE_STRING, 6, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "CXD2856", PROPERTY_VALUE_TYPE_STRING, 7, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "CXD2856", PROPERTY_VALUE_TYPE_STRING, 8, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "CXD2856", PROPERTY_VALUE_TYPE_STRING, 9, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "CXD2856", PROPERTY_VALUE_TYPE_STRING, 10, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "CXD2856", PROPERTY_VALUE_TYPE_STRING, 11, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LEDB#1_UUID", 0x00010002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_11[] =
{
    { "AF_REBOOT#1_TYPE", DT_FUNC_TYPE_REBOOT, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1.1", DT_PVE, "DF_SPIPROM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_12[] =
{
    { "AF_TEMPFANMGR_AF#1_UUID", 0x00020005, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1_TYPE", DT_FUNC_TYPE_CORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPIPROM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_13[] =
{
    { "BC_I2CM#1_ADDRESS", 0x0200, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x380, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x480, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x580, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x680, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x780, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x880, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x980, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xa80, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xb80, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xc80, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xd80, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xe80, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_VPD#1_TYPE", DT_FUNC_TYPE_VPD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_14[] =
{
    { "BC_IPSECG#1_UUID", 0x00010004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_15[] =
{
    { "BC_SWITCH#1", DT_PVE, "DMA_ACCU_SWITCH", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_16[] =
{
    { "BC_SWITCH#1_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SENSTEMP#1.1", DT_PVE, "DF_MXDS75TEMP#1", PROPERTY_VALUE_TYPE_STRING, -1, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_ANUBISS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_17[] =
{
    { "CAP_RX_DVBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2139_0_18[] =
{
    { "DF_TEMPFANMGR#1.1", DT_PVE, "BC_FANC#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_19[] =
{
    { "BC_ACCUFIFO#1_UUID", 0x00010009, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1_TYPE", DT_BLOCK_TYPE_VVI, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_TEMPFANMGR#1.2", DT_PVE, "DF_SENSTEMP#1", PROPERTY_VALUE_TYPE_STRING, -1, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_20[] =
{
    { "AF_SPIMF#1_UUID", 0x00020004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_21[] =
{
    { "AF_TEMPFANMGR_AF#1.1", DT_PVE, "DF_TEMPFANMGR#1", PROPERTY_VALUE_TYPE_STRING, -1,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LEDB#1_TYPE", DT_BLOCK_TYPE_LEDB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_22[] =
{
    { "BC_BURSTFIFO#1_ADDRESS", 0x320, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x420, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x520, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x620, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x720, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x820, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x920, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xa20, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xb20, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xc20, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xd20, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xe20, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_MXDS75TEMP#1_UUID", 0x00040001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_23[] =
{
    { "AF_TEMPFANMGR_AF#1_TYPE", DT_FUNC_TYPE_TEMPFANMGR_AF, NULL,
               PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1_ADDRESS", 0x000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_RX_ISDBT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 } 
};
static const DtProperty  DtProperties2139_0_25[] =
{
    { "BC_IPSECG#1_TYPE", DT_BLOCK_TYPE_IPSECG, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LEDB#1_ADDRESS", 0x030, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_LAST_REV", 200, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_26[] =
{
    { "AF_DMAPERFACCU#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CDMAC#1_ADDRESS", 0x340, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x440, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x540, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x640, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x740, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x840, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x940, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xa40, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xb40, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xc40, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xd40, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xe40, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FANC#1_ADDRESS", 0x1C0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TOD#1_UUID", 0x00010003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TSRXFMT#1_ADDRESS", 0x300, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x400, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x500, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x600, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x700, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x800, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x900, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xa00, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xb00, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xc00, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xd00, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0xe00, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_28[] =
{
    { "AF_LEDBOARD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_TSTRPRX#1_UUID", 0x00020007, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020009, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000B, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000D, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000F, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020011, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020013, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020015, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020017, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020019, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002001B, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002001D, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIMF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_29[] =
{
    { "AF_SPIMF#1.1", DT_PVE, "BC_SPIMF#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ACCUFIFO#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IPSECG#1_ADDRESS", 0x080, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_30[] =
{
    { "BC_ACCUFIFO#1_TYPE", DT_BLOCK_TYPE_ACCUFIFO, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_MSIX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_REBOOT#1_UUID", 0x00010001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCIE_REQUIRED_BW", 1292, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_31[] =
{
    { "AF_SPIMF#1_TYPE", DT_FUNC_TYPE_SPIMF, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_MXDS75TEMP#1.1", DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_32[] =
{
    { "AF_I2CM#1_UUID", 0x00020006, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020008, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000A, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000C, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000E, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020010, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020012, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020014, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020016, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020018, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002001A, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002001C, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_33[] =
{
    { "DF_MXDS75TEMP#1_TYPE", DT_FUNC_TYPE_MXDS75TEMP, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_34[] =
{
    { "DF_SPIPROM#1_UUID", 0x00040003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_35[] =
{
    { "CAP_RX_DVBT2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_37[] =
{
    { "AF_TSTRPRX#1.1", DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TOD#1_TYPE", DT_BLOCK_TYPE_TOD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_38[] =
{
    { "AF_TSTRPRX#1.2", DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_VPD#1_RO_SIZE", 768, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_39[] =
{
    { "AF_TSTRPRX#1.3", DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_TSTRPRX#1_TYPE", DT_FUNC_TYPE_TSTRPRX, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_TSTRPRX, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_TSTRPRX, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_TSTRPRX, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_TSTRPRX, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_TSTRPRX, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_TSTRPRX, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_TSTRPRX, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_TSTRPRX, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_TSTRPRX, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_TSTRPRX, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_TSTRPRX, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_BURSTFIFO#1_UUID", 0x0001000D, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010011, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010015, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010019, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001001D, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010021, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010025, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010029, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001002D, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010031, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010035, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010039, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SENSTEMP#1_UUID", 0x00040002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_40[] =
{
    { "BC_I2CM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_41[] =
{
    { "AF_I2CM#1.1", DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_REBOOT#1_TYPE", DT_BLOCK_TYPE_REBOOT, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TOD#1_ADDRESS", 0x040, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_TEMPFANMGR#1_UUID", 0x00040004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_42[] =
{
    { "AF_REBOOT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.1", DT_PVE, "BC_VVI#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_43[] =
{
    { "AF_I2CM#1_TYPE", DT_FUNC_TYPE_I2CM_2139, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_I2CM_2139, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_I2CM_2139, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_I2CM_2139, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_I2CM_2139, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_I2CM_2139, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_I2CM_2139, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_I2CM_2139, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_I2CM_2139, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_I2CM_2139, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_I2CM_2139, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_I2CM_2139, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CDMAC#1_UUID", 0x0001000E, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010012, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010016, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001001A, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001001E, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010022, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010026, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001002A, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001002E, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010032, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010036, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001003A, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FANC#1_UUID", 0x0001000A, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TSRXFMT#1_UUID", 0x0001000C, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010010, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010014, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010018, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001001C, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010020, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010024, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010028, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001002C, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010030, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010034, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010038, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.2", DT_PVE, "BC_TOD#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_MAX_TEMP", 75, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPIPROM#1.1", DT_PVE, "BC_SPIM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_44[] =
{
    { "BC_SPIM#1_UUID", 0x00010005, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_CORE#1.3", DT_PVE, "BC_MSIX#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_TARGET_TEMP", 55, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_45[] =
{
    { "BC_REBOOT#1_ADDRESS", 0x020, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.4", DT_PVE, "DF_VPD#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPIPROM#1_TYPE", DT_FUNC_TYPE_SPIPROM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_46[] =
{
    { "DF_VPD#1_RW_SIZE", 1024, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_48[] =
{
    { "AF_DMAPERFACCU#1_UUID", 0x00020000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1_UUID", 0x00020001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_49[] =
{
    { "BC_BURSTFIFO#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SENSTEMP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_NAME", DT_PVE, "Board", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_50[] =
{
    { "AF_LEDBOARD#1_UUID", 0x00020002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_BURSTFIFO#1_TYPE", DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIMF#1_UUID", 0x00010006, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_RX_DVBC2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "DF_SENSTEMP#1_TYPE", DT_FUNC_TYPE_SENSTEMP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PORT_COUNT", 12, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_51[] =
{
    { "DF_TEMPFANMGR#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_52[] =
{
    { "BC_LEDB#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_MSIX#1_UUID", 0x00010007, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#1_ADDRESS", 0x180, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_TEMPFANMGR#1_TYPE", DT_FUNC_TYPE_TEMPFANMGR, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_53[] =
{
    { "BC_CDMAC#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FANC#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TSRXFMT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_54[] =
{
    { "AF_TEMPFANMGR_AF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CDMAC#1_TYPE", DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FANC#1_TYPE", DT_BLOCK_TYPE_FANC, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TSRXFMT#1_TYPE", DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MAIN_PORT_COUNT", 12, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_55[] =
{
    { "BC_SPIM#1_TYPE", DT_BLOCK_TYPE_SPIM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_MXDS75TEMP#1_I2C_ADDR", 0x4F, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_56[] =
{
    { "BC_IPSECG#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_57[] =
{
    { "AF_DMAPERFACCU#1.1", DT_PVE, "BC_ACCUFIFO#1", PROPERTY_VALUE_TYPE_STRING, -1, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1.1", DT_PVE, "BC_IPSECG#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_58[] =
{
    { "AF_DMAPERFACCU#1.2", DT_PVE, "BC_SWITCH#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_TEMPFANMGR#1_MIN_FAN_SPEED", 50, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_59[] =
{
    { "AF_DMAPERFACCU#1_TYPE", DT_FUNC_TYPE_DMAPERFACCU, NULL, PROPERTY_VALUE_TYPE_INT,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1_TYPE", DT_FUNC_TYPE_IPSECG, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_LEDBOARD#1.1", DT_PVE, "BC_LEDB#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIM#1_ADDRESS", 0x0C0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_UHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "VPD_ID", DT_PVE, "Twelve-channel Cable/Terrestrial Receiver",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_60[] =
{
    { "CAP_VHF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "PCI_DEVICE_ID", 0xB85B, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_61[] =
{
    { "AF_LEDBOARD#1_TYPE", DT_FUNC_TYPE_LEDBOARD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIMF#1_TYPE", DT_BLOCK_TYPE_SPIMF, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_BUILD_DATE_0", DT_PVE, "2019.01.29T14:10",
      PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_62[] =
{
    { "AF_SPIMF#1", DT_PVE, "FPGA_FIRMWARE", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1_UUID", 0x0001000B, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001000F, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010013, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010017, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001001B, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001001F, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010023, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010027, NULL, PROPERTY_VALUE_TYPE_INT, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001002B, NULL, PROPERTY_VALUE_TYPE_INT, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001002F, NULL, PROPERTY_VALUE_TYPE_INT, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010033, NULL, PROPERTY_VALUE_TYPE_INT, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010037, NULL, PROPERTY_VALUE_TYPE_INT, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001003B, NULL, PROPERTY_VALUE_TYPE_INT, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2139_0_63[] =
{
    { "BC_MSIX#1_TYPE", DT_BLOCK_TYPE_MSIX, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 6, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 7, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 8, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 9, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 10, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 11, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2139_0[] =
{
    { DtProperties2139_0_00, 2 },
    { DtProperties2139_0_01, 27 },
    { DtProperties2139_0_02, 14 },
    { DtProperties2139_0_03, 13 },
    { DtProperties2139_0_04, 1 },
    { DtProperties2139_0_05, 1 },
    { DtProperties2139_0_06, 12 },
    { NULL, 0 },
    { DtProperties2139_0_08, 14 },
    { DtProperties2139_0_09, 14 },
    { DtProperties2139_0_10, 13 },
    { DtProperties2139_0_11, 3 },
    { DtProperties2139_0_12, 3 },
    { DtProperties2139_0_13, 14 },
    { DtProperties2139_0_14, 1 },
    { DtProperties2139_0_15, 1 },
    { DtProperties2139_0_16, 3 },
    { DtProperties2139_0_17, 12 },
    { DtProperties2139_0_18, 1 },
    { DtProperties2139_0_19, 15 },
    { DtProperties2139_0_20, 2 },
    { DtProperties2139_0_21, 2 },
    { DtProperties2139_0_22, 14 },
    { DtProperties2139_0_23, 14 },
    { NULL, 0 },
    { DtProperties2139_0_25, 3 },
    { DtProperties2139_0_26, 30 },
    { NULL, 0 },
    { DtProperties2139_0_28, 14 },
    { DtProperties2139_0_29, 3 },
    { DtProperties2139_0_30, 4 },
    { DtProperties2139_0_31, 2 },
    { DtProperties2139_0_32, 12 },
    { DtProperties2139_0_33, 1 },
    { DtProperties2139_0_34, 1 },
    { DtProperties2139_0_35, 13 },
    { NULL, 0 },
    { DtProperties2139_0_37, 13 },
    { DtProperties2139_0_38, 13 },
    { DtProperties2139_0_39, 37 },
    { DtProperties2139_0_40, 13 },
    { DtProperties2139_0_41, 15 },
    { DtProperties2139_0_42, 2 },
    { DtProperties2139_0_43, 40 },
    { DtProperties2139_0_44, 4 },
    { DtProperties2139_0_45, 3 },
    { DtProperties2139_0_46, 2 },
    { NULL, 0 },
    { DtProperties2139_0_48, 2 },
    { DtProperties2139_0_49, 14 },
    { DtProperties2139_0_50, 29 },
    { DtProperties2139_0_51, 1 },
    { DtProperties2139_0_52, 4 },
    { DtProperties2139_0_53, 25 },
    { DtProperties2139_0_54, 27 },
    { DtProperties2139_0_55, 2 },
    { DtProperties2139_0_56, 1 },
    { DtProperties2139_0_57, 2 },
    { DtProperties2139_0_58, 2 },
    { DtProperties2139_0_59, 17 },
    { DtProperties2139_0_60, 13 },
    { DtProperties2139_0_61, 3 },
    { DtProperties2139_0_62, 14 },
    { DtProperties2139_0_63, 13 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2172; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Dual 3G-SDI/ASI Ports
// 
static const DtProperty  DtProperties2172_0_00[] =
{
    { "AF_REBOOT#1_UUID", 0x00020004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_DMATESTMODE", DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DEFAULT_GENREF", DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_TRUE, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDITXPHY#1_TYPE", DT_FUNC_TYPE_SDITXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_SDITXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_01[] =
{
    { "BC_SPIMF#1_ADDRESS", 0x100, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1_UUID", 0x00000000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_02[] =
{
    { "BC_ACCUFIFO#1_ADDRESS", 0x1E0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SI534X#1_DEVICE_TYPE", 5342, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1_UUID", 0x00040006, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_03[] =
{
    { "BC_MSIX#1_ADDRESS", 0x140, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_04[] =
{
    { "BC_TOD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_GENLOCKED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_HDSDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_05[] =
{
    { "AF_ASISDIRX#1.1", DT_PVE, "BC_SDIRXF#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDIRXF#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#1_UUID", 0x0001000A, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010010, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010024, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_06[] =
{
    { "AF_ASISDIRX#1.2", DT_PVE, "DF_SDIRX#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "DF_SDIRX#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SPICABLEDRVEQ#1.1", DT_PVE, "BC_SPIM#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SPIM#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_07[] =
{
    { "AF_ASISDIRX#1.3", DT_PVE, "DF_ASIRX#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "DF_ASIRX#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_ASISDIRX#1_TYPE", DT_FUNC_TYPE_ASISDIRX, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_ASISDIRX, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_ASISDITX#1.1", DT_PVE, "BC_ASITXG#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_ASITXG#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_DMA#1_UUID", 0x00020009, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000C, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#1_UUID", 0x0001000D, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#3_UUID", 0x0001001A, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001002E, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_720P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_08[] =
{
    { "AF_ASISDITX#1.2", DT_PVE, "BC_SDITXF#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDITXF#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_GENL#1_ADDRESS", 0x220, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_REBOOT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1_UUID", 0x00010000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_720P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_ASIRX#1.1", DT_PVE, "BC_ASIRXP#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_ASIRXP#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_09[] =
{
    { "AF_ASISDITX#1.3", DT_PVE, "BC_SDITXP#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDITXP#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_ASISDITX#1_TYPE", DT_FUNC_TYPE_ASISDITX, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_ASISDITX, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_REBOOT#1.1", DT_PVE, "BC_REBOOT#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1_TYPE", DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#5_UUID", 0x00010016, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001002A, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_ASIRX#1.2", DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1_I2C_ADDR", 0x69, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_10[] =
{
    { "AF_ASISDITX#1.4", DT_PVE, "DF_SDITXPHY#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "DF_SDITXPHY#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ASIRXP#1_UUID", 0x00010012, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010026, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LEDB#1_UUID", 0x00010002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_11[] =
{
    { "AF_REBOOT#1_TYPE", DT_FUNC_TYPE_REBOOT, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1.1", DT_PVE, "DF_SPIPROM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_12[] =
{
    { "AF_SDIGENREF#1_UUID", 0x0002000D, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000E, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_TEMPFANMGR_AF#1_UUID", 0x00020006, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_IOSTD", DT_IOCONFIG_1080I50, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_1080I50, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_625I50, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_625I50, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_CORE#1_TYPE", DT_FUNC_TYPE_CORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDIRX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SPIPROM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_13[] =
{
    { "BC_I2CM#1_ADDRESS", 0x180, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1_TYPE", DT_FUNC_TYPE_VPD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_14[] =
{
    { "BC_IPSECG#1_UUID", 0x00010004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_15[] =
{
    { "BC_CONSTSINK#1_UUID", 0x00010019, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001002D, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#1", DT_PVE, "DMA_ACCU_SWITCH", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "RX_FRONT_END", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "RX_FRONT_END", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_GENREF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_16[] =
{
    { "BC_SWITCH#1_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#2", DT_PVE, "RX_BACK_END", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "RX_BACK_END", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080I60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TXONTIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                 PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 33, 2 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                 PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 33, 2 },
    { "DF_SENSTEMP#1.1", DT_PVE, "BC_FPGATEMP#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_17[] =
{
    { "AF_DMA#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#3", DT_PVE, "TX_BACK_END", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "TX_BACK_END", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080PSF25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_18[] =
{
    { "AF_DMA#1_TYPE", DT_FUNC_TYPE_DMA, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_DMA, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ASITXG#1_ADDRESS", 0xa50, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1250, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXF#1_ADDRESS", 0x980, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1180, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#1_TYPE", DT_BLOCK_TYPE_SDITXPLL, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDITXPLL#2", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#3_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#4", DT_PVE, "TX_FRONT_END", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "TX_FRONT_END", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_3GSDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SI534X#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_TEMPFANMGR#1.1", DT_PVE, "DF_SENSTEMP#1", PROPERTY_VALUE_TYPE_STRING, -1, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_19[] =
{
    { "AF_GENLOCKCTRL_AF#1_UUID", 0x00020001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ACCUFIFO#1_UUID", 0x0001000B, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#5", DT_PVE, "RX_TEST_MODE", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "RX_TEST_MODE", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_VVI#1_TYPE", DT_BLOCK_TYPE_VVI, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_20[] =
{
    { "AF_SPIMF#1_UUID", 0x00020005, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXF#1_ADDRESS", 0xa80, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1280, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#5_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#6", DT_PVE, "TX_TEST_MODE", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "TX_TEST_MODE", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VIRTGENREF#1_UUID", 0x0004000F, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_21[] =
{
    { "AF_SDIGENREF#1.1", DT_PVE, "BC_LMH1981#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "DF_VIRTGENREF#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_TEMPFANMGR_AF#1.1", DT_PVE, "DF_TEMPFANMGR#1", PROPERTY_VALUE_TYPE_STRING, -1,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ASIRXP#1_TYPE", DT_BLOCK_TYPE_ASIRXP, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_ASIRXP, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LEDB#1_TYPE", DT_BLOCK_TYPE_LEDB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_GENLOCKED", DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_22[] =
{
    { "BC_BURSTFIFO#1_ADDRESS", 0xc40, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1440, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FPGATEMP#1_ADDRESS", 0x0F0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LMH1981#1_ADDRESS", 0x200, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_23[] =
{
    { "AF_SDIGENREF#1_TYPE", DT_FUNC_TYPE_SDIGENREF, NULL, PROPERTY_VALUE_TYPE_INT, 2,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_SDIGENREF, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_TEMPFANMGR_AF#1_TYPE", DT_FUNC_TYPE_TEMPFANMGR_AF, NULL,
               PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1_ADDRESS", 0x000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_24[] =
{
    { "BC_SDIRXPHY#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_25[] =
{
    { "BC_ASIRXP#1_ADDRESS", 0x920, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1120, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_CONSTSINK#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_GENL#1_UUID", 0x0001000C, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_IPSECG#1_TYPE", DT_BLOCK_TYPE_IPSECG, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LEDB#1_ADDRESS", 0x030, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_LAST_REV", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_26[] =
{
    { "AF_DMAPERFACCU#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CDMAC#1_ADDRESS", 0xc00, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1400, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSINK#1_TYPE", DT_BLOCK_TYPE_CONSTSINK, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CONSTSINK, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSOURCE#1_ADDRESS", 0x9e0, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x11e0, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPHY#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TOD#1_UUID", 0x00010003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TSRXFMT#1_ADDRESS", 0x910, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1110, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_GENLOCKCTRL#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_28[] =
{
    { "AF_LEDBOARD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXP#1_ADDRESS", 0x940, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1140, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SPIMF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_29[] =
{
    { "AF_GENLOCKCTRL_AF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_SPIMF#1.1", DT_PVE, "BC_SPIMF#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ACCUFIFO#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IPSECG#1_ADDRESS", 0x080, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SPICABLEDRVEQ#1_UUID", 0x0004000A, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0004000E, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_30[] =
{
    { "AF_GENLOCKCTRL_AF#1_TYPE", DT_FUNC_TYPE_GENLOCKCTRL_AF, NULL,
               PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ACCUFIFO#1_TYPE", DT_BLOCK_TYPE_ACCUFIFO, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_MSIX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_REBOOT#1_UUID", 0x00010001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXP#1_ADDRESS", 0xac0, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x12c0, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SDITXPHY#1.1", DT_PVE, "BC_SDITXPHY#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDITXPHY#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDITXPHY#1_TX_PHY_DELAY_SD", 300, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 300, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCIE_REQUIRED_BW", 8000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_31[] =
{
    { "AF_SPIMF#1_TYPE", DT_FUNC_TYPE_SPIMF, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "DF_ASIRX#1_UUID", 0x00040007, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0004000B, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_VIRTGENREF#1_TYPE", DT_FUNC_TYPE_VIRTGENREF, NULL, PROPERTY_VALUE_TYPE_INT, 3,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_34[] =
{
    { "DF_SDIRX#1_UUID", 0x00040008, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0004000C, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SDITXPHY#1_TX_PHY_DELAY3G", 105, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 105, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SPIPROM#1_UUID", 0x00040004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_35[] =
{
    { "BC_ASITXG#1_UUID", 0x0001001B, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001002F, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_GENL#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDIRXF#1_UUID", 0x00010014, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010028, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_36[] =
{
    { "BC_GENL#1_TYPE", DT_BLOCK_TYPE_GENL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_37[] =
{
    { "BC_SDITXF#1_UUID", 0x0001001C, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010030, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TOD#1_TYPE", DT_BLOCK_TYPE_TOD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_38[] =
{
    { "AF_ASISDIRX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#2_UUID", 0x00010015, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010029, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_720P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_VPD#1_RO_SIZE", 3584, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_39[] =
{
    { "BC_BURSTFIFO#1_UUID", 0x00010021, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010035, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FPGATEMP#1_UUID", 0x00010006, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LMH1981#1_UUID", 0x00010037, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SENSTEMP#1_UUID", 0x00040002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPICABLEDRVEQ#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_40[] =
{
    { "AF_ASISDITX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDITXPLL#2_UUID", 0x0001000E, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#4_UUID", 0x0001001E, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010032, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1_UUID", 0x00040003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPICABLEDRVEQ#1_TYPE", DT_FUNC_TYPE_SPICABLEDRVEQ, NULL,
               PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_SPICABLEDRVEQ, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_41[] =
{
    { "BC_REBOOT#1_TYPE", DT_BLOCK_TYPE_REBOOT, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TOD#1_ADDRESS", 0x040, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_ASIRX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_TEMPFANMGR#1_UUID", 0x00040005, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_42[] =
{
    { "AF_REBOOT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#6_UUID", 0x00010018, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001002C, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_ASIRX#1_TYPE", DT_FUNC_TYPE_ASIRX, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_ASIRX, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.1", DT_PVE, "BC_VVI#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1_FRAC_CLK_IDX", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_43[] =
{
    { "BC_CDMAC#1_UUID", 0x00010020, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010034, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSOURCE#1_UUID", 0x00010017, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001002B, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TSRXFMT#1_UUID", 0x00010011, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010025, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_CORE#1.2", DT_PVE, "BC_TOD#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDIRX#1.1", DT_PVE, "BC_SDIRXPHY#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDIRXPHY#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_MAX_TEMP", 85, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPIPROM#1.1", DT_PVE, "BC_SPIM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PORT_TYPE", DT_PORT_TYPE_ASISDIRXTX, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PORT_TYPE_ASISDIRXTX, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PORT_TYPE_SDIGENREF, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PORT_TYPE_SDIGENREF, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_44[] =
{
    { "BC_SPIM#1_UUID", 0x00010005, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010022, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010036, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_CORE#1.3", DT_PVE, "BC_MSIX#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDIRX#1.2", DT_PVE, "BC_SDIRXP#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDIRXP#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_TARGET_TEMP", 60, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_45[] =
{
    { "BC_ASITXG#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_REBOOT#1_ADDRESS", 0x020, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXP#1_UUID", 0x00010013, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010027, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P59_94B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.4", DT_PVE, "DF_VPD#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDIRX#1_TYPE", DT_FUNC_TYPE_SDIRX, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_SDIRX, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SPIPROM#1_TYPE", DT_FUNC_TYPE_SPIPROM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_46[] =
{
    { "BC_ASITXG#1_TYPE", DT_BLOCK_TYPE_ASITXG, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_ASITXG, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXF#1_TYPE", DT_BLOCK_TYPE_SDIRXF, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDIRXF, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXPHY#1_UUID", 0x0001000F, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010023, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1_RW_SIZE", 4096, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_47[] =
{
    { "BC_SDITXF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXP#1_UUID", 0x0001001D, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010031, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_DMATESTMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_48[] =
{
    { "AF_DMA#1.1", DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_DMAPERFACCU#1_UUID", 0x00020000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1_UUID", 0x00020002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXF#1_TYPE", DT_BLOCK_TYPE_SDITXF, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDITXF, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPHY#1_UUID", 0x0001001F, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010033, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_GENLOCKCTRL#1_UUID", 0x00040001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_49[] =
{
    { "AF_DMA#1.2", DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_BURSTFIFO#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FPGATEMP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LMH1981#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#2_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_NAME", DT_PVE, "FPGA", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1.1", DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_50[] =
{
    { "AF_DMA#1.3", DT_PVE, "BC_CONSTSOURCE#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CONSTSOURCE#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_LEDBOARD#1_UUID", 0x00020003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_BURSTFIFO#1_TYPE", DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FPGATEMP#1_TYPE", DT_BLOCK_TYPE_FPGATEMP, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LMH1981#1_TYPE", DT_BLOCK_TYPE_LMH1981, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIMF#1_UUID", 0x00010007, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_TYPE", DT_FUNC_TYPE_SENSTEMP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1.2", DT_PVE, "BC_SDITXPLL#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PORT_COUNT", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_51[] =
{
    { "AF_DMA#1.4", DT_PVE, "BC_CONSTSINK#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CONSTSINK#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#2_TYPE", DT_BLOCK_TYPE_SDITXPLL, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#4_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDITXPHY#1_TX_PHY_DELAY_HD", 130, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 130, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SI534X#1.3", DT_PVE, "BC_SDITXPLL#2", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1_TYPE", DT_FUNC_TYPE_SI534X, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_TEMPFANMGR#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_52[] =
{
    { "BC_ASIRXP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LEDB#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_MSIX#1_UUID", 0x00010008, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#1_ADDRESS", 0x1D0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x900, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { NULL, 0x1100, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SI534X#1_NON_FRAC_CLK_IDX", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_TEMPFANMGR#1_TYPE", DT_FUNC_TYPE_TEMPFANMGR, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_53[] =
{
    { "BC_CDMAC#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSOURCE#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#2_ADDRESS", 0x9c0, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x11c0, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#6_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TSRXFMT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDITXPHY#1_UUID", 0x00040009, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0004000D, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_54[] =
{
    { "AF_SDIGENREF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_TEMPFANMGR_AF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CDMAC#1_TYPE", DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSOURCE#1_TYPE", DT_BLOCK_TYPE_CONSTSOURCE, NULL, PROPERTY_VALUE_TYPE_INT,
                 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CONSTSOURCE, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#1_ADDRESS", 0x240, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#3_ADDRESS", 0xa40, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1240, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TSRXFMT#1_TYPE", DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MAIN_PORT_COUNT", 4, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_55[] =
{
    { "BC_SDIRXP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDITXPLL#2_ADDRESS", 0x280, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SPIM#1_TYPE", DT_BLOCK_TYPE_SPIM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SPIM, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SPIM, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#4_ADDRESS", 0xb00, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1300, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_56[] =
{
    { "BC_IPSECG#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXP#1_TYPE", DT_BLOCK_TYPE_SDIRXP, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDIRXP, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#5_ADDRESS", 0x9d0, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x11d0, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P50B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_57[] =
{
    { "AF_DMAPERFACCU#1.1", DT_PVE, "BC_ACCUFIFO#1", PROPERTY_VALUE_TYPE_STRING, -1, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1.1", DT_PVE, "BC_IPSECG#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXPHY#1_TYPE", DT_BLOCK_TYPE_SDIRXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDIRXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDITXP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#6_ADDRESS", 0xa00, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1200, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P60B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_GENLOCKCTRL#1.1", DT_PVE, "BC_GENL#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_58[] =
{
    { "AF_DMAPERFACCU#1.2", DT_PVE, "BC_SWITCH#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXP#1_TYPE", DT_BLOCK_TYPE_SDITXP, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDITXP, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_GENLOCKCTRL#1.2", DT_PVE, "DF_SI534X#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_59[] =
{
    { "AF_DMAPERFACCU#1_TYPE", DT_FUNC_TYPE_DMAPERFACCU, NULL, PROPERTY_VALUE_TYPE_INT,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1_TYPE", DT_FUNC_TYPE_IPSECG, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_LEDBOARD#1.1", DT_PVE, "BC_LEDB#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPHY#1_TYPE", DT_BLOCK_TYPE_SDITXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDITXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SPIM#1_ADDRESS", 0x0C0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0xC80, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { NULL, 0x1480, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_720P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_GENLOCKCTRL#1_TYPE", DT_FUNC_TYPE_GENLOCKCTRL, NULL, PROPERTY_VALUE_TYPE_INT,
                -1, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", DT_PVE, "Dual 3G-SDI/ASI Ports", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_60[] =
{
    { "AF_ASISDIRX#1_UUID", 0x00020007, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000A, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_GENLOCKCTRL_AF#1.1", DT_PVE, "DF_GENLOCKCTRL#1", PROPERTY_VALUE_TYPE_STRING,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_DEVICE_ID", 0x87C, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_61[] =
{
    { "AF_LEDBOARD#1_TYPE", DT_FUNC_TYPE_LEDBOARD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXPHY#1_ADDRESS", 0x800, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1000, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SPIMF#1_TYPE", DT_BLOCK_TYPE_SPIMF, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_BUILD_DATE_0", DT_PVE, "2019.05.14T07:49",
      PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_62[] =
{
    { "AF_ASISDITX#1_UUID", 0x00020008, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000B, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_SPIMF#1", DT_PVE, "FPGA_FIRMWARE", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSINK#1_ADDRESS", 0xa20, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1220, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1_UUID", 0x00010009, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VIRTGENREF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2172_0_63[] =
{
    { "BC_MSIX#1_TYPE", DT_BLOCK_TYPE_MSIX, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDITXPHY#1_ADDRESS", 0xb20, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1320, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_MATRIX2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_VIRTUAL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SDITXPHY#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2172_0[] =
{
    { DtProperties2172_0_00, 9 },
    { DtProperties2172_0_01, 3 },
    { DtProperties2172_0_02, 3 },
    { DtProperties2172_0_03, 1 },
    { DtProperties2172_0_04, 7 },
    { DtProperties2172_0_05, 5 },
    { DtProperties2172_0_06, 8 },
    { DtProperties2172_0_07, 19 },
    { DtProperties2172_0_08, 13 },
    { DtProperties2172_0_09, 13 },
    { DtProperties2172_0_10, 5 },
    { DtProperties2172_0_11, 11 },
    { DtProperties2172_0_12, 11 },
    { DtProperties2172_0_13, 2 },
    { DtProperties2172_0_14, 1 },
    { DtProperties2172_0_15, 7 },
    { DtProperties2172_0_16, 17 },
    { DtProperties2172_0_17, 9 },
    { DtProperties2172_0_18, 18 },
    { DtProperties2172_0_19, 7 },
    { DtProperties2172_0_20, 9 },
    { DtProperties2172_0_21, 10 },
    { DtProperties2172_0_22, 7 },
    { DtProperties2172_0_23, 16 },
    { DtProperties2172_0_24, 6 },
    { DtProperties2172_0_25, 8 },
    { DtProperties2172_0_26, 16 },
    { NULL, 0 },
    { DtProperties2172_0_28, 4 },
    { DtProperties2172_0_29, 6 },
    { DtProperties2172_0_30, 11 },
    { DtProperties2172_0_31, 10 },
    { DtProperties2172_0_32, 2 },
    { NULL, 0 },
    { DtProperties2172_0_34, 5 },
    { DtProperties2172_0_35, 10 },
    { DtProperties2172_0_36, 13 },
    { DtProperties2172_0_37, 3 },
    { DtProperties2172_0_38, 9 },
    { DtProperties2172_0_39, 7 },
    { DtProperties2172_0_40, 9 },
    { DtProperties2172_0_41, 5 },
    { DtProperties2172_0_42, 7 },
    { DtProperties2172_0_43, 15 },
    { DtProperties2172_0_44, 8 },
    { DtProperties2172_0_45, 17 },
    { DtProperties2172_0_46, 8 },
    { DtProperties2172_0_47, 10 },
    { DtProperties2172_0_48, 9 },
    { DtProperties2172_0_49, 11 },
    { DtProperties2172_0_50, 12 },
    { DtProperties2172_0_51, 10 },
    { DtProperties2172_0_52, 13 },
    { DtProperties2172_0_53, 12 },
    { DtProperties2172_0_54, 21 },
    { DtProperties2172_0_55, 8 },
    { DtProperties2172_0_56, 11 },
    { DtProperties2172_0_57, 11 },
    { DtProperties2172_0_58, 4 },
    { DtProperties2172_0_59, 18 },
    { DtProperties2172_0_60, 4 },
    { DtProperties2172_0_61, 5 },
    { DtProperties2172_0_62, 7 },
    { DtProperties2172_0_63, 10 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2174; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Quad 3G-SDI/ASI Ports
// 
static const DtProperty  DtProperties2174_0_00[] =
{
    { "AF_REBOOT#1_UUID", 0x00020004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_DMATESTMODE", DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DEFAULT_GENREF", DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_TRUE, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDITXPHY#1_TYPE", DT_FUNC_TYPE_SDITXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_SDITXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_SDITXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_SDITXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_01[] =
{
    { "BC_SPIMF#1_ADDRESS", 0x100, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1_UUID", 0x00000000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_02[] =
{
    { "BC_ACCUFIFO#1_ADDRESS", 0x1E0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SI534X#1_DEVICE_TYPE", 5342, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1_UUID", 0x00040006, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_03[] =
{
    { "BC_MSIX#1_ADDRESS", 0x140, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_04[] =
{
    { "BC_TOD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_GENLOCKED", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_HDSDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_05[] =
{
    { "AF_ASISDIRX#1.1", DT_PVE, "BC_SDIRXF#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDIRXF#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDIRXF#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDIRXF#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#1_UUID", 0x0001000A, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010011, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010025, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010039, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001004D, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_06[] =
{
    { "AF_ASISDIRX#1.2", DT_PVE, "DF_SDIRX#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "DF_SDIRX#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "DF_SDIRX#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "DF_SDIRX#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SPICABLEDRVEQ#1.1", DT_PVE, "BC_SPIM#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SPIM#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SPIM#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SPIM#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_07[] =
{
    { "AF_ASISDIRX#1.3", DT_PVE, "DF_ASIRX#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "DF_ASIRX#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "DF_ASIRX#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "DF_ASIRX#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_ASISDIRX#1_TYPE", DT_FUNC_TYPE_ASISDIRX, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_ASISDIRX, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_ASISDIRX, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_ASISDIRX, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_ASISDITX#1.1", DT_PVE, "BC_ASITXG#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_ASITXG#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_ASITXG#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_ASITXG#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_DMA#1_UUID", 0x00020009, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000C, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000F, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020012, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#1_UUID", 0x0001000D, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#3_UUID", 0x0001001B, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001002F, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010043, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010057, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_720P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_08[] =
{
    { "AF_ASISDITX#1.2", DT_PVE, "BC_SDITXF#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDITXF#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDITXF#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDITXF#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_GENL#1_ADDRESS", 0x220, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_REBOOT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1_UUID", 0x00010000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_720P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_ASIRX#1.1", DT_PVE, "BC_ASIRXP#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_ASIRXP#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_ASIRXP#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_ASIRXP#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_09[] =
{
    { "AF_ASISDITX#1.3", DT_PVE, "BC_SDITXP#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDITXP#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDITXP#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDITXP#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_ASISDITX#1_TYPE", DT_FUNC_TYPE_ASISDITX, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_ASISDITX, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_ASISDITX, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_ASISDITX, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_REBOOT#1.1", DT_PVE, "BC_REBOOT#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1_TYPE", DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDITXPLL#3_UUID", 0x0001000F, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#5_UUID", 0x00010017, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001002B, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001003F, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010053, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_ASIRX#1.2", DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1_I2C_ADDR", 0x69, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_10[] =
{
    { "AF_ASISDITX#1.4", DT_PVE, "DF_SDITXPHY#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "DF_SDITXPHY#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "DF_SDITXPHY#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "DF_SDITXPHY#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ASIRXP#1_UUID", 0x00010013, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010027, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001003B, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001004F, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LEDB#1_UUID", 0x00010002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_11[] =
{
    { "AF_REBOOT#1_TYPE", DT_FUNC_TYPE_REBOOT, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1.1", DT_PVE, "DF_SPIPROM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_12[] =
{
    { "AF_SDIGENREF#1_UUID", 0x00020013, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020014, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_TEMPFANMGR_AF#1_UUID", 0x00020006, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_IOSTD", DT_IOCONFIG_1080I50, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_1080I50, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_1080I50, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_1080I50, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_625I50, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_625I50, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_CORE#1_TYPE", DT_FUNC_TYPE_CORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDIRX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SPIPROM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_13[] =
{
    { "BC_I2CM#1_ADDRESS", 0x180, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1_TYPE", DT_FUNC_TYPE_VPD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_14[] =
{
    { "BC_IPSECG#1_UUID", 0x00010004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_15[] =
{
    { "BC_CONSTSINK#1_UUID", 0x0001001A, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001002E, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010042, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010056, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#1", DT_PVE, "DMA_ACCU_SWITCH", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "RX_FRONT_END", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "RX_FRONT_END", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "RX_FRONT_END", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "RX_FRONT_END", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_GENREF", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_16[] =
{
    { "BC_SWITCH#1_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#2", DT_PVE, "RX_BACK_END", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "RX_BACK_END", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "RX_BACK_END", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "RX_BACK_END", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080I60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TXONTIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                 PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 33, 2 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                 PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 33, 2 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                 PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 33, 2 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                 PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 33, 2 },
    { "DF_SENSTEMP#1.1", DT_PVE, "BC_FPGATEMP#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_17[] =
{
    { "AF_DMA#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#3", DT_PVE, "TX_BACK_END", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "TX_BACK_END", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "TX_BACK_END", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "TX_BACK_END", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080PSF25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_18[] =
{
    { "AF_DMA#1_TYPE", DT_FUNC_TYPE_DMA, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_DMA, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_DMA, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_DMA, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ASITXG#1_ADDRESS", 0xa50, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1250, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1a50, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x2250, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXF#1_ADDRESS", 0x980, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1180, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1980, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x2180, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#1_TYPE", DT_BLOCK_TYPE_SDITXPLL, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDITXPLL#2", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#3_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#4", DT_PVE, "TX_FRONT_END", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "TX_FRONT_END", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "TX_FRONT_END", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "TX_FRONT_END", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_3GSDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SI534X#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_TEMPFANMGR#1.1", DT_PVE, "DF_SENSTEMP#1", PROPERTY_VALUE_TYPE_STRING, -1, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_19[] =
{
    { "AF_GENLOCKCTRL_AF#1_UUID", 0x00020001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ACCUFIFO#1_UUID", 0x0001000B, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#3", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#5", DT_PVE, "RX_TEST_MODE", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "RX_TEST_MODE", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "RX_TEST_MODE", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "RX_TEST_MODE", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_VVI#1_TYPE", DT_BLOCK_TYPE_VVI, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_20[] =
{
    { "AF_SPIMF#1_UUID", 0x00020005, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXF#1_ADDRESS", 0xa80, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1280, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1a80, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x2280, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#3_TYPE", DT_BLOCK_TYPE_SDITXPLL, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#5_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#6", DT_PVE, "TX_TEST_MODE", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "TX_TEST_MODE", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "TX_TEST_MODE", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "TX_TEST_MODE", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VIRTGENREF#1_UUID", 0x00040017, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_21[] =
{
    { "AF_SDIGENREF#1.1", DT_PVE, "BC_LMH1981#1", PROPERTY_VALUE_TYPE_STRING, 4, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "DF_VIRTGENREF#1", PROPERTY_VALUE_TYPE_STRING, 5, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_TEMPFANMGR_AF#1.1", DT_PVE, "DF_TEMPFANMGR#1", PROPERTY_VALUE_TYPE_STRING, -1,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ASIRXP#1_TYPE", DT_BLOCK_TYPE_ASIRXP, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_ASIRXP, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_ASIRXP, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_ASIRXP, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LEDB#1_TYPE", DT_BLOCK_TYPE_LEDB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_GENLOCKED", DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_22[] =
{
    { "BC_BURSTFIFO#1_ADDRESS", 0xc40, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1440, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1c40, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x2440, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FPGATEMP#1_ADDRESS", 0x0F0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LMH1981#1_ADDRESS", 0x200, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_23[] =
{
    { "AF_SDIGENREF#1_TYPE", DT_FUNC_TYPE_SDIGENREF, NULL, PROPERTY_VALUE_TYPE_INT, 4,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_SDIGENREF, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_TEMPFANMGR_AF#1_TYPE", DT_FUNC_TYPE_TEMPFANMGR_AF, NULL,
               PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1_ADDRESS", 0x000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_24[] =
{
    { "BC_SDIRXPHY#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_25[] =
{
    { "BC_ASIRXP#1_ADDRESS", 0x920, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1120, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1920, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x2120, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_CONSTSINK#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_GENL#1_UUID", 0x0001000C, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_IPSECG#1_TYPE", DT_BLOCK_TYPE_IPSECG, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LEDB#1_ADDRESS", 0x030, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_LAST_REV", 500, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_26[] =
{
    { "AF_DMAPERFACCU#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CDMAC#1_ADDRESS", 0xc00, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1400, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1c00, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x2400, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSINK#1_TYPE", DT_BLOCK_TYPE_CONSTSINK, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CONSTSINK, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CONSTSINK, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CONSTSINK, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSOURCE#1_ADDRESS", 0x9e0, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x11e0, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x19e0, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x21e0, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPHY#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TOD#1_UUID", 0x00010003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TSRXFMT#1_ADDRESS", 0x910, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1110, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1910, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x2110, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_GENLOCKCTRL#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_28[] =
{
    { "AF_LEDBOARD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXP#1_ADDRESS", 0x940, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1140, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1940, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x2140, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SPIMF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_29[] =
{
    { "AF_GENLOCKCTRL_AF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_SPIMF#1.1", DT_PVE, "BC_SPIMF#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ACCUFIFO#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IPSECG#1_ADDRESS", 0x080, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SPICABLEDRVEQ#1_UUID", 0x0004000A, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0004000E, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00040012, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00040016, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_30[] =
{
    { "AF_GENLOCKCTRL_AF#1_TYPE", DT_FUNC_TYPE_GENLOCKCTRL_AF, NULL,
               PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ACCUFIFO#1_TYPE", DT_BLOCK_TYPE_ACCUFIFO, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_MSIX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_REBOOT#1_UUID", 0x00010001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXP#1_ADDRESS", 0xac0, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x12c0, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1ac0, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x22c0, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SDITXPHY#1.1", DT_PVE, "BC_SDITXPHY#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDITXPHY#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDITXPHY#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDITXPHY#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDITXPHY#1_TX_PHY_DELAY_SD", 300, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 300, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 300, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 300, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCIE_REQUIRED_BW", 15500, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_31[] =
{
    { "AF_SPIMF#1_TYPE", DT_FUNC_TYPE_SPIMF, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0,
                                                                                   0, 0 },
    { "DF_ASIRX#1_UUID", 0x00040007, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0004000B, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0004000F, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00040013, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_VIRTGENREF#1_TYPE", DT_FUNC_TYPE_VIRTGENREF, NULL, PROPERTY_VALUE_TYPE_INT, 5,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_32[] =
{
    { "DEFAULT_IODIR", DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_INPUT, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_OUTPUT, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_34[] =
{
    { "DF_SDIRX#1_UUID", 0x00040008, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0004000C, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00040010, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00040014, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SDITXPHY#1_TX_PHY_DELAY3G", 105, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 105, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 105, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 105, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SPIPROM#1_UUID", 0x00040004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_35[] =
{
    { "BC_ASITXG#1_UUID", 0x0001001C, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010030, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010044, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010058, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_GENL#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDIRXF#1_UUID", 0x00010015, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010029, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001003D, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010051, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_36[] =
{
    { "BC_GENL#1_TYPE", DT_BLOCK_TYPE_GENL, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_37[] =
{
    { "BC_SDITXF#1_UUID", 0x0001001D, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010031, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010045, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010059, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TOD#1_TYPE", DT_BLOCK_TYPE_TOD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_38[] =
{
    { "AF_ASISDIRX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#2_UUID", 0x00010016, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001002A, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001003E, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010052, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_720P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_VPD#1_RO_SIZE", 3584, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_39[] =
{
    { "BC_BURSTFIFO#1_UUID", 0x00010022, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010036, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001004A, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001005E, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FPGATEMP#1_UUID", 0x00010006, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LMH1981#1_UUID", 0x00010060, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SENSTEMP#1_UUID", 0x00040002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPICABLEDRVEQ#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_40[] =
{
    { "AF_ASISDITX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDITXPLL#2_UUID", 0x0001000E, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#4_UUID", 0x0001001F, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010033, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010047, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001005B, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1_UUID", 0x00040003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPICABLEDRVEQ#1_TYPE", DT_FUNC_TYPE_SPICABLEDRVEQ, NULL,
               PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_SPICABLEDRVEQ, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_SPICABLEDRVEQ, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_SPICABLEDRVEQ, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_41[] =
{
    { "BC_REBOOT#1_TYPE", DT_BLOCK_TYPE_REBOOT, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TOD#1_ADDRESS", 0x040, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_ASIRX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_TEMPFANMGR#1_UUID", 0x00040005, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_42[] =
{
    { "AF_REBOOT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#6_UUID", 0x00010019, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001002D, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010041, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010055, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_ASIRX#1_TYPE", DT_FUNC_TYPE_ASIRX, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_ASIRX, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_ASIRX, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_ASIRX, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.1", DT_PVE, "BC_VVI#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1_FRAC_CLK_IDX", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_43[] =
{
    { "BC_CDMAC#1_UUID", 0x00010021, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010035, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010049, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001005D, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSOURCE#1_UUID", 0x00010018, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001002C, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010040, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010054, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TSRXFMT#1_UUID", 0x00010012, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010026, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001003A, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001004E, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_CORE#1.2", DT_PVE, "BC_TOD#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDIRX#1.1", DT_PVE, "BC_SDIRXPHY#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDIRXPHY#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDIRXPHY#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDIRXPHY#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_MAX_TEMP", 85, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPIPROM#1.1", DT_PVE, "BC_SPIM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PORT_TYPE", DT_PORT_TYPE_ASISDIRXTX, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PORT_TYPE_ASISDIRXTX, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PORT_TYPE_ASISDIRXTX, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PORT_TYPE_ASISDIRXTX, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PORT_TYPE_SDIGENREF, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PORT_TYPE_SDIGENREF, NULL, PROPERTY_VALUE_TYPE_INT, 5, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_44[] =
{
    { "BC_SPIM#1_UUID", 0x00010005, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010023, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010037, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001004B, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001005F, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_CORE#1.3", DT_PVE, "BC_MSIX#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDIRX#1.2", DT_PVE, "BC_SDIRXP#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDIRXP#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDIRXP#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "BC_SDIRXP#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_TARGET_TEMP", 60, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_45[] =
{
    { "BC_ASITXG#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_REBOOT#1_ADDRESS", 0x020, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXP#1_UUID", 0x00010014, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010028, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001003C, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010050, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P59_94B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.4", DT_PVE, "DF_VPD#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDIRX#1_TYPE", DT_FUNC_TYPE_SDIRX, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_SDIRX, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_SDIRX, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_SDIRX, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SPIPROM#1_TYPE", DT_FUNC_TYPE_SPIPROM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_46[] =
{
    { "BC_ASITXG#1_TYPE", DT_BLOCK_TYPE_ASITXG, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_ASITXG, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_ASITXG, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_ASITXG, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXF#1_TYPE", DT_BLOCK_TYPE_SDIRXF, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDIRXF, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDIRXF, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDIRXF, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXPHY#1_UUID", 0x00010010, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010024, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010038, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001004C, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1_RW_SIZE", 4096, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_47[] =
{
    { "BC_SDITXF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXP#1_UUID", 0x0001001E, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010032, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010046, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001005A, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_DMATESTMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_48[] =
{
    { "AF_DMA#1.1", DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_DMAPERFACCU#1_UUID", 0x00020000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1_UUID", 0x00020002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXF#1_TYPE", DT_BLOCK_TYPE_SDITXF, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDITXF, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDITXF, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDITXF, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPHY#1_UUID", 0x00010020, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010034, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010048, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001005C, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_GENLOCKCTRL#1_UUID", 0x00040001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_49[] =
{
    { "AF_DMA#1.2", DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_BURSTFIFO#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FPGATEMP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LMH1981#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#2_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_NAME", DT_PVE, "FPGA", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1.1", DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_50[] =
{
    { "AF_DMA#1.3", DT_PVE, "BC_CONSTSOURCE#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CONSTSOURCE#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CONSTSOURCE#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CONSTSOURCE#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_LEDBOARD#1_UUID", 0x00020003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_BURSTFIFO#1_TYPE", DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_FPGATEMP#1_TYPE", DT_BLOCK_TYPE_FPGATEMP, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LMH1981#1_TYPE", DT_BLOCK_TYPE_LMH1981, NULL, PROPERTY_VALUE_TYPE_INT, 4, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIMF#1_UUID", 0x00010007, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SENSTEMP#1_TYPE", DT_FUNC_TYPE_SENSTEMP, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1.2", DT_PVE, "BC_SDITXPLL#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PORT_COUNT", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_51[] =
{
    { "AF_DMA#1.4", DT_PVE, "BC_CONSTSINK#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CONSTSINK#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CONSTSINK#1", PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CONSTSINK#1", PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#2_TYPE", DT_BLOCK_TYPE_SDITXPLL, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#4_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDITXPHY#1_TX_PHY_DELAY_HD", 130, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 130, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 130, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 130, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SI534X#1.3", DT_PVE, "BC_SDITXPLL#2", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1_TYPE", DT_FUNC_TYPE_SI534X, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_TEMPFANMGR#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_52[] =
{
    { "BC_ASIRXP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LEDB#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_MSIX#1_UUID", 0x00010008, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#1_ADDRESS", 0x1D0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x900, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { NULL, 0x1100, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1900, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x2100, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SI534X#1.4", DT_PVE, "BC_SDITXPLL#3", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1_NON_FRAC_CLK_IDX", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_TEMPFANMGR#1_TYPE", DT_FUNC_TYPE_TEMPFANMGR, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_53[] =
{
    { "BC_CDMAC#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSOURCE#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#2_ADDRESS", 0x9c0, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x11c0, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x19c0, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x21c0, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#6_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TSRXFMT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDITXPHY#1_UUID", 0x00040009, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0004000D, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00040011, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00040015, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_54[] =
{
    { "AF_SDIGENREF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 4, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 5, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_TEMPFANMGR_AF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CDMAC#1_TYPE", DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSOURCE#1_TYPE", DT_BLOCK_TYPE_CONSTSOURCE, NULL, PROPERTY_VALUE_TYPE_INT,
                 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CONSTSOURCE, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CONSTSOURCE, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CONSTSOURCE, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#1_ADDRESS", 0x240, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#3_ADDRESS", 0xa40, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1240, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1a40, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x2240, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TSRXFMT#1_TYPE", DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MAIN_PORT_COUNT", 6, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_55[] =
{
    { "BC_SDIRXP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDITXPLL#2_ADDRESS", 0x280, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SPIM#1_TYPE", DT_BLOCK_TYPE_SPIM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SPIM, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SPIM, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SPIM, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SPIM, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#4_ADDRESS", 0xb00, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1300, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1b00, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x2300, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_56[] =
{
    { "BC_IPSECG#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXP#1_TYPE", DT_BLOCK_TYPE_SDIRXP, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDIRXP, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDIRXP, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDIRXP, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDITXPLL#3_ADDRESS", 0x2C0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#5_ADDRESS", 0x9d0, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x11d0, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x19d0, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x21d0, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P50B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_57[] =
{
    { "AF_DMAPERFACCU#1.1", DT_PVE, "BC_ACCUFIFO#1", PROPERTY_VALUE_TYPE_STRING, -1, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1.1", DT_PVE, "BC_IPSECG#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXPHY#1_TYPE", DT_BLOCK_TYPE_SDIRXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDIRXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDIRXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDIRXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDITXP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#6_ADDRESS", 0xa00, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1200, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1a00, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x2200, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P60B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_GENLOCKCTRL#1.1", DT_PVE, "BC_GENL#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_58[] =
{
    { "AF_DMAPERFACCU#1.2", DT_PVE, "BC_SWITCH#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXP#1_TYPE", DT_BLOCK_TYPE_SDITXP, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDITXP, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDITXP, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDITXP, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_GENLOCKCTRL#1.2", DT_PVE, "DF_SI534X#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_TEMPFANMGR#1_MIN_FAN_SPEED", 40, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_59[] =
{
    { "AF_DMAPERFACCU#1_TYPE", DT_FUNC_TYPE_DMAPERFACCU, NULL, PROPERTY_VALUE_TYPE_INT,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1_TYPE", DT_FUNC_TYPE_IPSECG, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_LEDBOARD#1.1", DT_PVE, "BC_LEDB#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPHY#1_TYPE", DT_BLOCK_TYPE_SDITXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDITXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDITXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SDITXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SPIM#1_ADDRESS", 0x0C0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0xC80, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { NULL, 0x1480, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1C80, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x2480, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_720P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 4, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_GENLOCKCTRL#1_TYPE", DT_FUNC_TYPE_GENLOCKCTRL, NULL, PROPERTY_VALUE_TYPE_INT,
                -1, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", DT_PVE, "Quad 3G-SDI/ASI Ports", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_60[] =
{
    { "AF_ASISDIRX#1_UUID", 0x00020007, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000A, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000D, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020010, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_GENLOCKCTRL_AF#1.1", DT_PVE, "DF_GENLOCKCTRL#1", PROPERTY_VALUE_TYPE_STRING,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_DEVICE_ID", 0xB87E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_61[] =
{
    { "AF_LEDBOARD#1_TYPE", DT_FUNC_TYPE_LEDBOARD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXPHY#1_ADDRESS", 0x800, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1000, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1800, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x2000, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SPIMF#1_TYPE", DT_BLOCK_TYPE_SPIMF, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_62[] =
{
    { "AF_ASISDITX#1_UUID", 0x00020008, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000B, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0002000E, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020011, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_SPIMF#1", DT_PVE, "FPGA_FIRMWARE", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSINK#1_ADDRESS", 0xa20, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1220, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x1a20, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x2220, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1_UUID", 0x00010009, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VIRTGENREF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2174_0_63[] =
{
    { "BC_MSIX#1_TYPE", DT_BLOCK_TYPE_MSIX, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDITXPHY#1_ADDRESS", 0xb20, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1320, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x1b20, NULL, PROPERTY_VALUE_TYPE_INT, 2, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x2320, NULL, PROPERTY_VALUE_TYPE_INT, 3, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_MATRIX2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_VIRTUAL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 5, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SDITXPHY#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 2, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 3, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2174_0[] =
{
    { DtProperties2174_0_00, 15 },
    { DtProperties2174_0_01, 3 },
    { DtProperties2174_0_02, 3 },
    { DtProperties2174_0_03, 1 },
    { DtProperties2174_0_04, 11 },
    { DtProperties2174_0_05, 9 },
    { DtProperties2174_0_06, 14 },
    { DtProperties2174_0_07, 33 },
    { DtProperties2174_0_08, 21 },
    { DtProperties2174_0_09, 24 },
    { DtProperties2174_0_10, 9 },
    { DtProperties2174_0_11, 15 },
    { DtProperties2174_0_12, 15 },
    { DtProperties2174_0_13, 2 },
    { DtProperties2174_0_14, 1 },
    { DtProperties2174_0_15, 11 },
    { DtProperties2174_0_16, 27 },
    { DtProperties2174_0_17, 15 },
    { DtProperties2174_0_18, 30 },
    { DtProperties2174_0_19, 12 },
    { DtProperties2174_0_20, 16 },
    { DtProperties2174_0_21, 16 },
    { DtProperties2174_0_22, 11 },
    { DtProperties2174_0_23, 22 },
    { DtProperties2174_0_24, 10 },
    { DtProperties2174_0_25, 12 },
    { DtProperties2174_0_26, 26 },
    { NULL, 0 },
    { DtProperties2174_0_28, 6 },
    { DtProperties2174_0_29, 8 },
    { DtProperties2174_0_30, 17 },
    { DtProperties2174_0_31, 16 },
    { DtProperties2174_0_32, 4 },
    { NULL, 0 },
    { DtProperties2174_0_34, 9 },
    { DtProperties2174_0_35, 16 },
    { DtProperties2174_0_36, 19 },
    { DtProperties2174_0_37, 5 },
    { DtProperties2174_0_38, 15 },
    { DtProperties2174_0_39, 11 },
    { DtProperties2174_0_40, 15 },
    { DtProperties2174_0_41, 7 },
    { DtProperties2174_0_42, 11 },
    { DtProperties2174_0_43, 25 },
    { DtProperties2174_0_44, 12 },
    { DtProperties2174_0_45, 29 },
    { DtProperties2174_0_46, 14 },
    { DtProperties2174_0_47, 18 },
    { DtProperties2174_0_48, 15 },
    { DtProperties2174_0_49, 17 },
    { DtProperties2174_0_50, 16 },
    { DtProperties2174_0_51, 16 },
    { DtProperties2174_0_52, 20 },
    { DtProperties2174_0_53, 24 },
    { DtProperties2174_0_54, 33 },
    { DtProperties2174_0_55, 14 },
    { DtProperties2174_0_56, 20 },
    { DtProperties2174_0_57, 19 },
    { DtProperties2174_0_58, 7 },
    { DtProperties2174_0_59, 26 },
    { DtProperties2174_0_60, 6 },
    { DtProperties2174_0_61, 6 },
    { DtProperties2174_0_62, 11 },
    { DtProperties2174_0_63, 18 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2175; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// 3G-SDI/ASI Input+Output with Relay Bypass
// 
static const DtProperty  DtProperties2175_0_00[] =
{
    { "AF_REBOOT#1_UUID", 0x00020003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_ASIPOL", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_DMATESTMODE", DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDITXPHY#1_TYPE", DT_FUNC_TYPE_SDITXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_01[] =
{
    { "BC_SPIMF#1_ADDRESS", 0x100, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1_UUID", 0x00000000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_02[] =
{
    { "BC_ACCUFIFO#1_ADDRESS", 0x1E0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SI534X#1_DEVICE_TYPE", 5342, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1_UUID", 0x00040004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_03[] =
{
    { "BC_MSIX#1_ADDRESS", 0x140, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_04[] =
{
    { "BC_TOD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_HDSDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DEFAULT_FAILSAFE", DT_IOCONFIG_FALSE, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_05[] =
{
    { "AF_ASISDIRX#1.1", DT_PVE, "BC_SDIRXF#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIXCFG#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#1_UUID", 0x00010009, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x0001000D, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001001A, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_06[] =
{
    { "AF_ASISDIRX#1.2", DT_PVE, "DF_SDIRX#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIXCFG#1_TYPE", DT_BLOCK_TYPE_SDIXCFG, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_07[] =
{
    { "AF_ASISDIRX#1.3", DT_PVE, "DF_ASIRX#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_ASISDIRX#1_TYPE", DT_FUNC_TYPE_ASISDIRX, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_ASISDITX#1.1", DT_PVE, "BC_ASITXG#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_DMA#1_UUID", 0x00020006, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00020008, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#1_UUID", 0x00010020, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#3_UUID", 0x00010014, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x00010023, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_720P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_08[] =
{
    { "AF_ASISDITX#1.2", DT_PVE, "BC_SDITXF#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_GS2988#1_UUID", 0x00010021, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_REBOOT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1_UUID", 0x00010000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_720P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_INPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_ASIRX#1.1", DT_PVE, "BC_ASIRXP#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_09[] =
{
    { "AF_ASISDITX#1.3", DT_PVE, "BC_SDITXP#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_ASISDITX#1_TYPE", DT_FUNC_TYPE_ASISDITX, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_REBOOT#1.1", DT_PVE, "BC_REBOOT#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1_TYPE", DT_BLOCK_TYPE_I2CM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_KA#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_OUTPUT", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_ASIRX#1.2", DT_PVE, "BC_TSRXFMT#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1_I2C_ADDR", 0x69, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_10[] =
{
    { "AF_ASISDITX#1.4", DT_PVE, "DF_SDITXPHY#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ASIRXP#1_UUID", 0x0001000E, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_KA#1_TYPE", DT_BLOCK_TYPE_KA, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LEDB#1_UUID", 0x00010002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_11[] =
{
    { "AF_KEEPALIVE#1_UUID", 0x00020009, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_REBOOT#1_TYPE", DT_FUNC_TYPE_REBOOT, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1.1", DT_PVE, "DF_SPIPROM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_12[] =
{
    { "DEFAULT_IOSTD", DT_IOCONFIG_1080I50, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_IOCONFIG_1080I50, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_CORE#1_TYPE", DT_FUNC_TYPE_CORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDIRX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SPIPROM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_13[] =
{
    { "BC_I2CM#1_ADDRESS", 0x180, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1_TYPE", DT_FUNC_TYPE_VPD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_14[] =
{
    { "BC_IPSECG#1_UUID", 0x00010004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_15[] =
{
    { "BC_CONSTSINK#1_UUID", 0x00010022, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#1", DT_PVE, "DMA_ACCU_SWITCH", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "RX_FRONT_END", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "TX_BACK_END", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_16[] =
{
    { "BC_SWITCH#1_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#2", DT_PVE, "RX_BACK_END", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "TX_FRONT_END", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080I60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TXONTIME", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                 PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 5, 33, 2 },
    { "DF_SDIXCFGMGR#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_17[] =
{
    { "AF_DMA#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#3", DT_PVE, "RX_TEST_MODE", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PVE, "TX_TEST_MODE", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080PSF25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_18[] =
{
    { "AF_DMA#1_TYPE", DT_FUNC_TYPE_DMA, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_FUNC_TYPE_DMA, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ASITXG#1_ADDRESS", 0x690, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXF#1_ADDRESS", 0x4c0, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#1_TYPE", DT_BLOCK_TYPE_SDITXPLL, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#3_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_3GSDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SI534X#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_19[] =
{
    { "BC_ACCUFIFO#1_UUID", 0x0001000A, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_GS2988#1_TYPE", DT_BLOCK_TYPE_GS2988, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_VVI#1_TYPE", DT_BLOCK_TYPE_VVI, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_FAILSAFE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TRPMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_20[] =
{
    { "AF_SPIMF#1_UUID", 0x00020004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXF#1_ADDRESS", 0x6a0, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_21[] =
{
    { "AF_KEEPALIVE#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ASIRXP#1_TYPE", DT_BLOCK_TYPE_ASIRXP, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LEDB#1_TYPE", DT_BLOCK_TYPE_LEDB, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_ASI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_22[] =
{
    { "AF_KEEPALIVE#1_TYPE", DT_FUNC_TYPE_KEEPALIVE, NULL, PROPERTY_VALUE_TYPE_INT, 1,
                                            -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_BURSTFIFO#1_ADDRESS", 0x560, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x640, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SPIM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_23[] =
{
    { "BC_GS2988#1_ADDRESS", 0x760, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_VVI#1_ADDRESS", 0x000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P24", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P60", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_525I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_24[] =
{
    { "BC_SDIRXPHY#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P25", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_25[] =
{
    { "BC_ASIRXP#1_ADDRESS", 0x440, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_CONSTSINK#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IPSECG#1_TYPE", DT_BLOCK_TYPE_IPSECG, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LEDB#1_ADDRESS", 0x030, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCB_LAST_REV", 201, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_26[] =
{
    { "AF_DMAPERFACCU#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CDMAC#1_ADDRESS", 0x580, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x600, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSINK#1_TYPE", DT_BLOCK_TYPE_CONSTSINK, NULL, PROPERTY_VALUE_TYPE_INT, 1,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSOURCE#1_ADDRESS", 0x520, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPHY#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TOD#1_UUID", 0x00010003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_TSRXFMT#1_ADDRESS", 0x460, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_28[] =
{
    { "AF_LEDBOARD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXP#1_ADDRESS", 0x480, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SPIMF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_29[] =
{
    { "AF_SPIMF#1.1", DT_PVE, "BC_SPIMF#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ACCUFIFO#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_IPSECG#1_ADDRESS", 0x080, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_30[] =
{
    { "BC_ACCUFIFO#1_TYPE", DT_BLOCK_TYPE_ACCUFIFO, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_MSIX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_REBOOT#1_UUID", 0x00010001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXP#1_ADDRESS", 0x6c0, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SDITXPHY#1.1", DT_PVE, "BC_SDITXPHY#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCIE_REQUIRED_BW", 4000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_31[] =
{
    { "AF_SPIMF#1_TYPE", DT_FUNC_TYPE_SPIMF, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TX_T2MI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_ASIRX#1_UUID", 0x00040005, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SDITXPHY#1.2", DT_PVE, "BC_SDITXPLL#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_34[] =
{
    { "DF_SDIRX#1_UUID", 0x00040006, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SPIPROM#1_UUID", 0x00040003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_35[] =
{
    { "BC_ASITXG#1_UUID", 0x0001001B, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXF#1_UUID", 0x00010011, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_625I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_36[] =
{
    { "CAP_1080I59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_37[] =
{
    { "BC_SDITXF#1_UUID", 0x0001001C, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TOD#1_TYPE", DT_BLOCK_TYPE_TOD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_38[] =
{
    { "AF_ASISDIRX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#2_UUID", 0x00010012, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x0001001E, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_720P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SDIXCFGMGR#1_UUID", 0x00040001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1_RO_SIZE", 768, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_39[] =
{
    { "BC_BURSTFIFO#1_UUID", 0x00010015, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010019, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_40[] =
{
    { "AF_ASISDITX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1_UUID", 0x00040002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_41[] =
{
    { "BC_REBOOT#1_TYPE", DT_BLOCK_TYPE_REBOOT, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TOD#1_ADDRESS", 0x040, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_ASIRX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_42[] =
{
    { "AF_REBOOT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIXCFG#1_ADDRESS", 0x800, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_ASIRX#1_TYPE", DT_FUNC_TYPE_ASIRX, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.1", DT_PVE, "BC_VVI#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1_FRAC_CLK_IDX", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_43[] =
{
    { "BC_CDMAC#1_UUID", 0x00010016, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x00010018, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSOURCE#1_UUID", 0x00010013, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TSRXFMT#1_UUID", 0x0001000F, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_CORE#1.2", DT_PVE, "BC_TOD#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDIRX#1.1", DT_PVE, "BC_SDIRXPHY#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPIPROM#1.1", DT_PVE, "BC_SPIM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PORT_TYPE", DT_PORT_TYPE_ASISDIRX, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_PORT_TYPE_ASISDITX, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_44[] =
{
    { "BC_SPIM#1_UUID", 0x00010005, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_CORE#1.3", DT_PVE, "BC_MSIX#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDIRX#1.2", DT_PVE, "BC_SDIRXP#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_45[] =
{
    { "BC_ASITXG#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_REBOOT#1_ADDRESS", 0x020, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXP#1_UUID", 0x00010010, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P59_94B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080PSF30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.4", DT_PVE, "DF_VPD#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDIRX#1_TYPE", DT_FUNC_TYPE_SDIRX, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SPIPROM#1_TYPE", DT_FUNC_TYPE_SPIPROM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_46[] =
{
    { "BC_ASITXG#1_TYPE", DT_BLOCK_TYPE_ASITXG, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_KA#1_ADDRESS", 0x1C0, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXF#1_TYPE", DT_BLOCK_TYPE_SDIRXF, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXPHY#1_UUID", 0x0001000C, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1_RW_SIZE", 1024, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_47[] =
{
    { "BC_SDITXF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXP#1_UUID", 0x0001001D, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080I50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_DMATESTMODE", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SDIXCFGMGR#1.1", DT_PVE, "BC_SDIXCFG#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N1_0", DT_PVE, "Fix for possible playout lock up",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_48[] =
{
    { "AF_DMA#1.1", DT_PVE, "BC_CONSTSOURCE#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_DMAPERFACCU#1_UUID", 0x00020000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1_UUID", 0x00020001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXF#1_TYPE", DT_BLOCK_TYPE_SDITXF, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPHY#1_UUID", 0x0001001F, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N1_1", DT_PVE, "Fix for 1080i format detection issue",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_49[] =
{
    { "AF_DMA#1.2", DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_BURSTFIFO#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_BURSTFIFO#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#2_TYPE", DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_SWITCH, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDIXCFGMGR#1_TYPE", DT_FUNC_TYPE_SDIXCFGMGR, NULL, PROPERTY_VALUE_TYPE_INT, -1,
                                           -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SI534X#1.1", DT_PVE, "BC_I2CM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_50[] =
{
    { "AF_DMA#1.3", DT_PVE, "BC_CDMAC#1", PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, "BC_CONSTSINK#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_LEDBOARD#1_UUID", 0x00020002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_BURSTFIFO#1_TYPE", DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 0,
                    -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_BURSTFIFO, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_GS2988#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SPIMF#1_UUID", 0x00010006, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_51[] =
{
    { "DF_SI534X#1_TYPE", DT_FUNC_TYPE_SI534X, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_52[] =
{
    { "AF_KEEPALIVE#1.1", DT_PVE, "BC_KA#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ASIRXP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_LEDB#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_MSIX#1_UUID", 0x00010007, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#1_ADDRESS", 0x1D0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 0x420, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { NULL, 0x680, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "CAP_1080P30", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SI534X#1_NON_FRAC_CLK_IDX", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_53[] =
{
    { "BC_CDMAC#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSOURCE#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SWITCH#2_ADDRESS", 0x500, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x700, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "BC_TSRXFMT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SDITXPHY#1_UUID", 0x00040007, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_54[] =
{
    { "BC_CDMAC#1_TYPE", DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, DT_BLOCK_TYPE_CDMAC, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSOURCE#1_TYPE", DT_BLOCK_TYPE_CONSTSOURCE, NULL, PROPERTY_VALUE_TYPE_INT,
                 0, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPLL#1_ADDRESS", 0x740, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SWITCH#3_ADDRESS", 0x540, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { NULL, 0x7a0, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1, PROPERTY_SCOPE_DRIVER,
                                                                                0, 0, 0 },
    { "BC_TSRXFMT#1_TYPE", DT_BLOCK_TYPE_TSRXFMT, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P50", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P23_98", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "MAIN_PORT_COUNT", 2, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_55[] =
{
    { "BC_SDIRXP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SPIM#1_TYPE", DT_BLOCK_TYPE_SPIM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_56[] =
{
    { "BC_IPSECG#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXP#1_TYPE", DT_BLOCK_TYPE_SDIRXP, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0,
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_1080P50B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_SDI", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_57[] =
{
    { "AF_DMAPERFACCU#1.1", DT_PVE, "BC_ACCUFIFO#1", PROPERTY_VALUE_TYPE_STRING, -1, -1,
                        0, 0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1.1", DT_PVE, "BC_IPSECG#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXPHY#1_TYPE", DT_BLOCK_TYPE_SDIRXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDITXP#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_1080P60B", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_58[] =
{
    { "AF_DMAPERFACCU#1.2", DT_PVE, "BC_SWITCH#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXP#1_TYPE", DT_BLOCK_TYPE_SDITXP, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_59[] =
{
    { "AF_DMAPERFACCU#1_TYPE", DT_FUNC_TYPE_DMAPERFACCU, NULL, PROPERTY_VALUE_TYPE_INT,
                                        -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_IPSECG#1_TYPE", DT_FUNC_TYPE_IPSECG, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_LEDBOARD#1.1", DT_PVE, "BC_LEDB#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDITXPHY#1_TYPE", DT_BLOCK_TYPE_SDITXPHY, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDIXCFG#1_UUID", 0x0001000B, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SPIM#1_ADDRESS", 0x0C0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_720P29_97", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_720P59_94", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "VPD_ID", DT_PVE, "3G-SDI/ASI Input+Output with Relay Bypass",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_60[] =
{
    { "AF_ASISDIRX#1_UUID", 0x00020005, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_DEVICE_ID", 0x87F, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_61[] =
{
    { "AF_LEDBOARD#1_TYPE", DT_FUNC_TYPE_LEDBOARD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                                0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_SDIRXPHY#1_ADDRESS", 0x400, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SPIMF#1_TYPE", DT_BLOCK_TYPE_SPIMF, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "FIRMWARE_RELEASE_BUILD_DATE_0", DT_PVE, "2019.01.18T09:35",
      PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_62[] =
{
    { "AF_ASISDITX#1_UUID", 0x00020007, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "AF_SPIMF#1", DT_PVE, "FPGA_FIRMWARE", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_CONSTSINK#1_ADDRESS", 0x780, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_I2CM#1_UUID", 0x00010008, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_RELEASE_BUILD_DATE_1", DT_PVE, "2019.07.24T15:30",
      PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_63[] =
{
    { "BC_KA#1_UUID", 0x00010017, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_MSIX#1_TYPE", DT_BLOCK_TYPE_MSIX, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_SDITXPHY#1_ADDRESS", 0x720, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "CAP_MATRIX2", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "CAP_TS", 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 0, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { NULL, 1, NULL, PROPERTY_VALUE_TYPE_BOOL, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SDITXPHY#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2175_0[] =
{
    { DtProperties2175_0_00, 6 },
    { DtProperties2175_0_01, 3 },
    { DtProperties2175_0_02, 3 },
    { DtProperties2175_0_03, 1 },
    { DtProperties2175_0_04, 4 },
    { DtProperties2175_0_05, 5 },
    { DtProperties2175_0_06, 4 },
    { DtProperties2175_0_07, 12 },
    { DtProperties2175_0_08, 8 },
    { DtProperties2175_0_09, 8 },
    { DtProperties2175_0_10, 4 },
    { DtProperties2175_0_11, 8 },
    { DtProperties2175_0_12, 5 },
    { DtProperties2175_0_13, 2 },
    { DtProperties2175_0_14, 1 },
    { DtProperties2175_0_15, 4 },
    { DtProperties2175_0_16, 12 },
    { DtProperties2175_0_17, 7 },
    { DtProperties2175_0_18, 10 },
    { DtProperties2175_0_19, 5 },
    { DtProperties2175_0_20, 3 },
    { DtProperties2175_0_21, 5 },
    { DtProperties2175_0_22, 4 },
    { DtProperties2175_0_23, 8 },
    { DtProperties2175_0_24, 3 },
    { DtProperties2175_0_25, 5 },
    { DtProperties2175_0_26, 11 },
    { NULL, 0 },
    { DtProperties2175_0_28, 3 },
    { DtProperties2175_0_29, 3 },
    { DtProperties2175_0_30, 6 },
    { DtProperties2175_0_31, 6 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2175_0_34, 2 },
    { DtProperties2175_0_35, 5 },
    { DtProperties2175_0_36, 6 },
    { DtProperties2175_0_37, 2 },
    { DtProperties2175_0_38, 7 },
    { DtProperties2175_0_39, 2 },
    { DtProperties2175_0_40, 3 },
    { DtProperties2175_0_41, 3 },
    { DtProperties2175_0_42, 5 },
    { DtProperties2175_0_43, 9 },
    { DtProperties2175_0_44, 4 },
    { DtProperties2175_0_45, 11 },
    { DtProperties2175_0_46, 6 },
    { DtProperties2175_0_47, 8 },
    { DtProperties2175_0_48, 7 },
    { DtProperties2175_0_49, 8 },
    { DtProperties2175_0_50, 9 },
    { DtProperties2175_0_51, 1 },
    { DtProperties2175_0_52, 10 },
    { DtProperties2175_0_53, 7 },
    { DtProperties2175_0_54, 12 },
    { DtProperties2175_0_55, 2 },
    { DtProperties2175_0_56, 6 },
    { DtProperties2175_0_57, 6 },
    { DtProperties2175_0_58, 2 },
    { DtProperties2175_0_59, 11 },
    { DtProperties2175_0_60, 2 },
    { DtProperties2175_0_61, 4 },
    { DtProperties2175_0_62, 5 },
    { DtProperties2175_0_63, 8 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2199; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// CDMAC RX TX test device
// 
static const DtProperty  DtProperties2199_0_00[] =
{
    { "AF_REBOOT#1_UUID", 0x00020000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_01[] =
{
    { "BC_SPIMF#1_ADDRESS", 0x140, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1_UUID", 0x00000000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_02[] =
{
    { "DF_VPD#1_UUID", 0x00040002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_03[] =
{
    { "BC_MSIX#1_ADDRESS", 0x400, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_04[] =
{
    { "BC_TOD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_08[] =
{
    { "BC_REBOOT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1_UUID", 0x00010000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_09[] =
{
    { "AF_REBOOT#1.1", DT_PVE, "BC_REBOOT#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_11[] =
{
    { "AF_REBOOT#1_TYPE", DT_FUNC_TYPE_REBOOT, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1.1", DT_PVE, "DF_SPIPROM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_12[] =
{
    { "DF_CORE#1_TYPE", DT_FUNC_TYPE_CORE, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPIPROM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_13[] =
{
    { "DF_VPD#1_TYPE", DT_FUNC_TYPE_VPD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_16[] =
{
    { "PCB_MANUF", MANUF_PLANT_KCS, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_19[] =
{
    { "BC_VVI#1_TYPE", DT_BLOCK_TYPE_VVI, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_20[] =
{
    { "AF_SPIMF#1_UUID", 0x00020001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "SUBDVC_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_22[] =
{
    { "BC_SPIM#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_23[] =
{
    { "BC_VVI#1_ADDRESS", 0x000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_25[] =
{
    { "PCB_LAST_REV", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_26[] =
{
    { "BC_TOD#1_UUID", 0x00010002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PCI_SUB_SYSTEM_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "PCI_VENDOR_ID", 0x1A0E, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_28[] =
{
    { "BC_SPIMF#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_29[] =
{
    { "AF_SPIMF#1.1", DT_PVE, "BC_SPIMF#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_30[] =
{
    { "BC_MSIX#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_REBOOT#1_UUID", 0x00010001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_31[] =
{
    { "AF_SPIMF#1_TYPE", DT_FUNC_TYPE_SPIMF, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_34[] =
{
    { "DF_SPIPROM#1_UUID", 0x00040001, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_35[] =
{
    { "PCI_SUB_VENDOR_ID", 0x0, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_37[] =
{
    { "BC_TOD#1_TYPE", DT_BLOCK_TYPE_TOD, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_38[] =
{
    { "DF_VPD#1_RO_SIZE", 1024, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_41[] =
{
    { "BC_REBOOT#1_TYPE", DT_BLOCK_TYPE_REBOOT, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_TOD#1_ADDRESS", 0x080, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_42[] =
{
    { "AF_REBOOT#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                          PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.1", DT_PVE, "BC_VVI#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_43[] =
{
    { "DF_CORE#1.2", DT_PVE, "BC_TOD#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPIPROM#1.1", DT_PVE, "BC_SPIM#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_44[] =
{
    { "BC_SPIM#1_UUID", 0x00010003, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_CORE#1.3", DT_PVE, "BC_MSIX#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_VPD#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_45[] =
{
    { "BC_REBOOT#1_ADDRESS", 0x040, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_CORE#1.4", DT_PVE, "DF_VPD#1", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "DF_SPIPROM#1_TYPE", DT_FUNC_TYPE_SPIPROM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1,
                                               0, 0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_46[] =
{
    { "DF_VPD#1_RW_SIZE", 1024, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "FIRMWARE_RELEASE_N0_0", DT_PVE, "First firmware version released to the field",
            PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_50[] =
{
    { "BC_SPIMF#1_UUID", 0x00010004, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_VVI#1", DT_PVE, NULL, PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_52[] =
{
    { "BC_MSIX#1_UUID", 0x00010005, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_54[] =
{
    { "MAIN_PORT_COUNT", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_55[] =
{
    { "BC_SPIM#1_TYPE", DT_BLOCK_TYPE_SPIM, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_59[] =
{
    { "BC_SPIM#1_ADDRESS", 0x100, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "VPD_ID", DT_PVE, "CDMAC RX TX test device", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_60[] =
{
    { "PCI_DEVICE_ID", 0x897, NULL, PROPERTY_VALUE_TYPE_UINT16, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_61[] =
{
    { "BC_SPIMF#1_TYPE", DT_BLOCK_TYPE_SPIMF, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0,
                           0, -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_62[] =
{
    { "AF_SPIMF#1", DT_PVE, "FPGA_FIRMWARE", PROPERTY_VALUE_TYPE_STRING, -1, -1, 0, 0,
                                                      -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2199_0_63[] =
{
    { "BC_MSIX#1_TYPE", DT_BLOCK_TYPE_MSIX, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                                                     -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2199_0[] =
{
    { DtProperties2199_0_00, 1 },
    { DtProperties2199_0_01, 3 },
    { DtProperties2199_0_02, 1 },
    { DtProperties2199_0_03, 1 },
    { DtProperties2199_0_04, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2199_0_08, 2 },
    { DtProperties2199_0_09, 1 },
    { NULL, 0 },
    { DtProperties2199_0_11, 3 },
    { DtProperties2199_0_12, 2 },
    { DtProperties2199_0_13, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2199_0_16, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2199_0_19, 1 },
    { DtProperties2199_0_20, 2 },
    { NULL, 0 },
    { DtProperties2199_0_22, 1 },
    { DtProperties2199_0_23, 1 },
    { NULL, 0 },
    { DtProperties2199_0_25, 1 },
    { DtProperties2199_0_26, 3 },
    { NULL, 0 },
    { DtProperties2199_0_28, 1 },
    { DtProperties2199_0_29, 1 },
    { DtProperties2199_0_30, 2 },
    { DtProperties2199_0_31, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2199_0_34, 1 },
    { DtProperties2199_0_35, 1 },
    { NULL, 0 },
    { DtProperties2199_0_37, 1 },
    { DtProperties2199_0_38, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2199_0_41, 2 },
    { DtProperties2199_0_42, 2 },
    { DtProperties2199_0_43, 2 },
    { DtProperties2199_0_44, 3 },
    { DtProperties2199_0_45, 3 },
    { DtProperties2199_0_46, 2 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2199_0_50, 3 },
    { NULL, 0 },
    { DtProperties2199_0_52, 1 },
    { NULL, 0 },
    { DtProperties2199_0_54, 1 },
    { DtProperties2199_0_55, 1 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { DtProperties2199_0_59, 2 },
    { DtProperties2199_0_60, 1 },
    { DtProperties2199_0_61, 1 },
    { DtProperties2199_0_62, 1 },
    { DtProperties2199_0_63, 1 }
};


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertyStores -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// References to hash table per device type.
//
const DtPropertyStore  DtPropertyStores[] =
{
    { 2100, 0, DtPropertyHashSets2100_0, 64 },
    { 2132, 0, DtPropertyHashSets2132_0, 64 },
    { 2139, 0, DtPropertyHashSets2139_0, 64 },
    { 2172, 0, DtPropertyHashSets2172_0, 64 },
    { 2174, 0, DtPropertyHashSets2174_0, 64 },
    { 2175, 0, DtPropertyHashSets2175_0, 64 },
    { 2199, 0, DtPropertyHashSets2199_0, 64 }
};
const Int  DtPropertyStoreCount = 7;

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
/*  9 */ "DMATESTMODE",
/* 10 */ "FAILSAFE",
/* 11 */ "FRACMODE",
/* 12 */ "GENLOCKED",
/* 13 */ "GENREF",
/* 14 */ "SWS2APSK",
/* 15 */ "TRUE",
/* 16 */ "FALSE",
/* 17 */ "DISABLED",
/* 18 */ "INPUT",
/* 19 */ "INTINPUT",
/* 20 */ "MONITOR",
/* 21 */ "OUTPUT",
/* 22 */ "SHAREDANT",
/* 23 */ "DBLBUF",
/* 24 */ "LOOPS2L3",
/* 25 */ "LOOPS2TS",
/* 26 */ "LOOPTHR",
/* 27 */ "12GSDI",
/* 28 */ "3GSDI",
/* 29 */ "6GSDI",
/* 30 */ "ASI",
/* 31 */ "AVENC",
/* 32 */ "DEKTECST",
/* 33 */ "DEMOD",
/* 34 */ "GPSTIME",
/* 35 */ "HDMI",
/* 36 */ "HDSDI",
/* 37 */ "IFADC",
/* 38 */ "IP",
/* 39 */ "MOD",
/* 40 */ "PHASENOISE",
/* 41 */ "RS422",
/* 42 */ "SDIRX",
/* 43 */ "SDI",
/* 44 */ "SPI",
/* 45 */ "SPISDI",
/* 46 */ "2160P50",
/* 47 */ "2160P50B",
/* 48 */ "2160P59_94",
/* 49 */ "2160P59_94B",
/* 50 */ "2160P60",
/* 51 */ "2160P60B",
/* 52 */ "1080P50",
/* 53 */ "1080P50B",
/* 54 */ "1080P59_94",
/* 55 */ "1080P59_94B",
/* 56 */ "1080P60",
/* 57 */ "1080P60B",
/* 58 */ "2160P23_98",
/* 59 */ "2160P24",
/* 60 */ "2160P25",
/* 61 */ "2160P29_97",
/* 62 */ "2160P30",
/* 63 */ "1080I50",
/* 64 */ "1080I59_94",
/* 65 */ "1080I60",
/* 66 */ "1080P23_98",
/* 67 */ "1080P24",
/* 68 */ "1080P25",
/* 69 */ "1080P29_97",
/* 70 */ "1080P30",
/* 71 */ "1080PSF23_98",
/* 72 */ "1080PSF24",
/* 73 */ "1080PSF25",
/* 74 */ "1080PSF29_97",
/* 75 */ "1080PSF30",
/* 76 */ "720P23_98",
/* 77 */ "720P24",
/* 78 */ "720P25",
/* 79 */ "720P29_97",
/* 80 */ "720P30",
/* 81 */ "720P50",
/* 82 */ "720P59_94",
/* 83 */ "720P60",
/* 84 */ "525I59_94",
/* 85 */ "625I50",
/* 86 */ "SPI525I59_94",
/* 87 */ "SPI625I50",
/* 88 */ "MODHQ",
/* 89 */ "LOWPWR",
/* 90 */ "RFCLKEXT",
/* 91 */ "RFCLKINT",
/* 92 */ "SPICLKEXT",
/* 93 */ "SPICLKINT",
/* 94 */ "SPIFIXEDCLK",
/* 95 */ "SPIDVBMODE",
/* 96 */ "SPISER8B",
/* 97 */ "SPISER10B",
/* 98 */ "SPILVDS1",
/* 99 */ "SPILVDS2",
/* 100 */ "SPILVTTL",
/* 101 */ "EXTTSRATE",
/* 102 */ "EXTRATIO",
/* 103 */ "INTTSRATE",
/* 104 */ "LOCK2INP"
};
const Int  IoConfigCodeStringCount = 105;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IoConfigCodes_XX[] -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Helper arrays to map from DT_IOCONFIG name to DT_IOCONFIG code.
// The hash of the DT_IOCONFIG names in IoConfigCodes_XX[] is XX.
//
static const IoConfigCode  IoConfigCodes_0[] =
{
    { "FAILSAFE", 10 },
    { "INTINPUT", 19 }
};
static const IoConfigCode  IoConfigCodes_1[] =
{
    { "1080P30", 70 }
};
static const IoConfigCode  IoConfigCodes_2[] =
{
    { "RS422", 41 }
};
static const IoConfigCode  IoConfigCodes_3[] =
{
    { "PWRMODE", 2 },
    { "LOOPS2L3", 24 },
    { "1080P50", 52 },
    { "720P23_98", 76 }
};
static const IoConfigCode  IoConfigCodes_4[] =
{
    { "2160P60", 50 },
    { "2160P24", 59 }
};
static const IoConfigCode  IoConfigCodes_5[] =
{
    { "SDI", 43 },
    { "2160P50B", 47 },
    { "2160P25", 60 }
};
static const IoConfigCode  IoConfigCodes_6[] =
{
    { "FRACMODE", 11 },
    { "2160P60B", 51 }
};
static const IoConfigCode  IoConfigCodes_7[] =
{
    { "HDMI", 35 },
    { "SPILVTTL", 100 }
};
static const IoConfigCode  IoConfigCodes_8[] =
{
    { "IOSTD", 1 },
    { "720P29_97", 79 },
    { "720P59_94", 82 }
};
static const IoConfigCode  IoConfigCodes_12[] =
{
    { "1080PSF23_98", 71 }
};
static const IoConfigCode  IoConfigCodes_14[] =
{
    { "DEMOD", 33 }
};
static const IoConfigCode  IoConfigCodes_15[] =
{
    { "12GSDI", 27 }
};
static const IoConfigCode  IoConfigCodes_16[] =
{
    { "FALSE", 16 },
    { "625I50", 85 }
};
static const IoConfigCode  IoConfigCodes_17[] =
{
    { "GENLOCKED", 12 },
    { "HDSDI", 36 },
    { "SPI", 44 },
    { "SPISDI", 45 },
    { "1080I59_94", 64 },
    { "1080PSF29_97", 74 },
    { "720P30", 80 }
};
static const IoConfigCode  IoConfigCodes_18[] =
{
    { "AVENC", 31 },
    { "SPIDVBMODE", 95 }
};
static const IoConfigCode  IoConfigCodes_19[] =
{
    { "2160P23_98", 58 },
    { "720P50", 81 }
};
static const IoConfigCode  IoConfigCodes_20[] =
{
    { "DBLBUF", 23 }
};
static const IoConfigCode  IoConfigCodes_21[] =
{
    { "INPUT", 18 },
    { "EXTRATIO", 102 }
};
static const IoConfigCode  IoConfigCodes_22[] =
{
    { "SPIMODE", 5 },
    { "SPICLKINT", 93 }
};
static const IoConfigCode  IoConfigCodes_24[] =
{
    { "2160P59_94", 48 },
    { "2160P29_97", 61 }
};
static const IoConfigCode  IoConfigCodes_26[] =
{
    { "2160P59_94B", 49 }
};
static const IoConfigCode  IoConfigCodes_27[] =
{
    { "SPIFIXEDCLK", 94 }
};
static const IoConfigCode  IoConfigCodes_28[] =
{
    { "IODIR", 0 },
    { "SPISTD", 6 },
    { "TSRATESEL", 7 },
    { "SPI625I50", 87 },
    { "SPICLKEXT", 92 }
};
static const IoConfigCode  IoConfigCodes_29[] =
{
    { "1080I60", 65 },
    { "1080PSF24", 72 }
};
static const IoConfigCode  IoConfigCodes_30[] =
{
    { "BW", 8 },
    { "GPSTIME", 34 },
    { "1080PSF25", 73 }
};
static const IoConfigCode  IoConfigCodes_31[] =
{
    { "3GSDI", 28 }
};
static const IoConfigCode  IoConfigCodes_33[] =
{
    { "2160P30", 62 }
};
static const IoConfigCode  IoConfigCodes_34[] =
{
    { "6GSDI", 29 },
    { "ASI", 30 },
    { "RFCLKINT", 91 }
};
static const IoConfigCode  IoConfigCodes_35[] =
{
    { "SWS2APSK", 14 },
    { "2160P50", 46 },
    { "INTTSRATE", 103 }
};
static const IoConfigCode  IoConfigCodes_36[] =
{
    { "1080P60", 56 },
    { "1080P24", 67 },
    { "525I59_94", 84 }
};
static const IoConfigCode  IoConfigCodes_37[] =
{
    { "TRUE", 15 },
    { "MOD", 39 },
    { "1080P50B", 53 },
    { "1080P25", 68 }
};
static const IoConfigCode  IoConfigCodes_38[] =
{
    { "1080P60B", 57 }
};
static const IoConfigCode  IoConfigCodes_39[] =
{
    { "LOCK2INP", 104 }
};
static const IoConfigCode  IoConfigCodes_40[] =
{
    { "RFCLKEXT", 90 }
};
static const IoConfigCode  IoConfigCodes_41[] =
{
    { "EXTTSRATE", 101 }
};
static const IoConfigCode  IoConfigCodes_43[] =
{
    { "LOOPS2TS", 25 }
};
static const IoConfigCode  IoConfigCodes_45[] =
{
    { "MONITOR", 20 },
    { "LOOPTHR", 26 }
};
static const IoConfigCode  IoConfigCodes_47[] =
{
    { "SPICLKSEL", 4 },
    { "SDIRX", 42 }
};
static const IoConfigCode  IoConfigCodes_48[] =
{
    { "SPI525I59_94", 86 },
    { "LOWPWR", 89 }
};
static const IoConfigCode  IoConfigCodes_51[] =
{
    { "1080P23_98", 66 }
};
static const IoConfigCode  IoConfigCodes_52[] =
{
    { "PHASENOISE", 40 },
    { "720P24", 77 },
    { "720P60", 83 }
};
static const IoConfigCode  IoConfigCodes_53[] =
{
    { "720P25", 78 },
    { "SPISER8B", 96 }
};
static const IoConfigCode  IoConfigCodes_54[] =
{
    { "OUTPUT", 21 }
};
static const IoConfigCode  IoConfigCodes_56[] =
{
    { "1080P59_94", 54 },
    { "1080P29_97", 69 }
};
static const IoConfigCode  IoConfigCodes_58[] =
{
    { "1080P59_94B", 55 },
    { "1080PSF30", 75 }
};
static const IoConfigCode  IoConfigCodes_59[] =
{
    { "RFCLKSEL", 3 },
    { "SPILVDS1", 98 }
};
static const IoConfigCode  IoConfigCodes_60[] =
{
    { "DMATESTMODE", 9 },
    { "GENREF", 13 },
    { "DEKTECST", 32 },
    { "IFADC", 37 },
    { "1080I50", 63 },
    { "SPILVDS2", 99 }
};
static const IoConfigCode  IoConfigCodes_61[] =
{
    { "DISABLED", 17 }
};
static const IoConfigCode  IoConfigCodes_62[] =
{
    { "IP", 38 },
    { "MODHQ", 88 },
    { "SPISER10B", 97 }
};
static const IoConfigCode  IoConfigCodes_63[] =
{
    { "SHAREDANT", 22 }
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IoConfigCodeHashSets[] -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// IoConfigCodeHashSets maps from hash value to IoConfigCodes_XX[].
//
const IoConfigCodeHashSet  IoConfigCodeHashSets[] =
{
    { IoConfigCodes_0, 2 },
    { IoConfigCodes_1, 1 },
    { IoConfigCodes_2, 1 },
    { IoConfigCodes_3, 4 },
    { IoConfigCodes_4, 2 },
    { IoConfigCodes_5, 3 },
    { IoConfigCodes_6, 2 },
    { IoConfigCodes_7, 2 },
    { IoConfigCodes_8, 3 },
    { NULL, 0 },
    { NULL, 0 },
    { NULL, 0 },
    { IoConfigCodes_12, 1 },
    { NULL, 0 },
    { IoConfigCodes_14, 1 },
    { IoConfigCodes_15, 1 },
    { IoConfigCodes_16, 2 },
    { IoConfigCodes_17, 7 },
    { IoConfigCodes_18, 2 },
    { IoConfigCodes_19, 2 },
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
    { IoConfigCodes_30, 3 },
    { IoConfigCodes_31, 1 },
    { NULL, 0 },
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
    { IoConfigCodes_61, 1 },
    { IoConfigCodes_62, 3 },
    { IoConfigCodes_63, 1 }
};
const Int  IoConfigCodesHashCount = 64;
