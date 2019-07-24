//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPcieTableStore.c *#*#*#*#*#*#*#*#*# (C) 2019 DekTec
//
// CapParser - Device Table Store - Tables with calibration data, filter coefficients, ...
//
// THIS FILE IS GENERATED, DO NOT EDIT!!!


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#include "DtPcieIncludes.h"


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTA-2175; Sub-device=0 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static const DtTableEntry  DtTable2175_0_000[] =
{
    { 0, 0 }
};

static const DtTableLink  DtTableLink2175_0[] = 
{
    { "DUMMY_TABLE", 0, -1, 0, 0, 1, DtTable2175_0_000 }
};

const Int  DtTableStoreCount = 1;
const DtTableStore  DtTableStores[] = 
{
    { 2175, 0, 1, DtTableLink2175_0 }
};
