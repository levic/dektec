//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPciePropertyStore.c *#*#*#*#*#*#*#*#* (C) 2019 DekTec
//
// CapParser - Property Store - Tables storing device properties
//
// THIS FILE IS GENERATED, DO NOT EDIT!!!

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#include "DtPcieIncludes.h"

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
    { "DF_TEMPFANMGR#1_CONTROL_DELAY", 5000, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0,
                              -1, PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
    { "DF_SENSTEMP#1_MAX_TEMP", 60, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
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
    { "DF_SENSTEMP#1_TARGET_TEMP", 50, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
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
    { DtProperties2139_0_01, 28 },
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
    { "FIRMWARE_LAST_VER", 0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_02[] =
{
    { "BC_ACCUFIFO#1_ADDRESS", 0x1E0, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
    { "AF_ASISDITX#1.4", DT_PVE, "BC_KA#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_ASIRXP#1_UUID", 0x0001000E, NULL, PROPERTY_VALUE_TYPE_INT, 0, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 },
    { "BC_KA#1_TYPE", DT_BLOCK_TYPE_KA, NULL, PROPERTY_VALUE_TYPE_INT, 1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "BC_LEDB#1_UUID", 0x00010002, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
};
static const DtProperty  DtProperties2175_0_11[] =
{
    { "AF_ASISDITX#1.5", DT_PVE, "DF_SDITXPHY#1", PROPERTY_VALUE_TYPE_STRING, 1, -1, 0,
                                                   0, -1, PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
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
                                                  0, -1, PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
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
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
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
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 } 
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
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
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
                                  PROPERTY_SCOPE_DRIVER | PROPERTY_SCOPE_DTAPI, 0, 0, 0 },
    { "DF_SI534X#1_NUM_CLOCKS", 1, NULL, PROPERTY_VALUE_TYPE_INT, -1, -1, 0, 0, -1,
                                                         PROPERTY_SCOPE_DRIVER, 0, 0, 0 } 
};
static const DtPropertyHashSet  DtPropertyHashSets2175_0[] =
{
    { DtProperties2175_0_00, 6 },
    { DtProperties2175_0_01, 3 },
    { DtProperties2175_0_02, 2 },
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
    { DtProperties2175_0_16, 11 },
    { DtProperties2175_0_17, 7 },
    { DtProperties2175_0_18, 10 },
    { DtProperties2175_0_19, 4 },
    { DtProperties2175_0_20, 3 },
    { DtProperties2175_0_21, 4 },
    { DtProperties2175_0_22, 3 },
    { DtProperties2175_0_23, 8 },
    { DtProperties2175_0_24, 3 },
    { DtProperties2175_0_25, 5 },
    { DtProperties2175_0_26, 11 },
    { NULL, 0 },
    { DtProperties2175_0_28, 3 },
    { DtProperties2175_0_29, 3 },
    { DtProperties2175_0_30, 6 },
    { DtProperties2175_0_31, 5 },
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
    { DtProperties2175_0_42, 4 },
    { DtProperties2175_0_43, 9 },
    { DtProperties2175_0_44, 4 },
    { DtProperties2175_0_45, 11 },
    { DtProperties2175_0_46, 6 },
    { DtProperties2175_0_47, 7 },
    { DtProperties2175_0_48, 6 },
    { DtProperties2175_0_49, 8 },
    { DtProperties2175_0_50, 9 },
    { DtProperties2175_0_51, 1 },
    { DtProperties2175_0_52, 8 },
    { DtProperties2175_0_53, 7 },
    { DtProperties2175_0_54, 12 },
    { DtProperties2175_0_55, 2 },
    { DtProperties2175_0_56, 6 },
    { DtProperties2175_0_57, 6 },
    { DtProperties2175_0_58, 2 },
    { DtProperties2175_0_59, 11 },
    { DtProperties2175_0_60, 2 },
    { DtProperties2175_0_61, 4 },
    { DtProperties2175_0_62, 4 },
    { DtProperties2175_0_63, 9 }
};


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertyStores -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// References to hash table per device type.
//
const DtPropertyStore  DtPropertyStores[] =
{
    { 2139, 0, DtPropertyHashSets2139_0, 64 },
    { 2175, 0, DtPropertyHashSets2175_0, 64 }
};
const Int  DtPropertyStoreCount = 2;

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
