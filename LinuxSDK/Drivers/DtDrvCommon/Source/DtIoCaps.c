//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoCaps.c *#*#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
// Driver common - General driver utilities.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018 DekTec Digital Video B.V.
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


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtDrvCommon.h>

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoCapsClearCaps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtIoCapsClearCaps(DtIoCaps* pCaps)
{
    pCaps->m_Flags[0] =  pCaps->m_Flags[1] =  pCaps->m_Flags[2] = pCaps-> m_Flags[3] = 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoCapsClearCap -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtIoCapsClearCap(DtIoCaps* pCaps, DtIoCapValue  Cap)
{
    Int CapIdx = (Int)Cap;
    DT_ASSERT(CapIdx < DT_IOCAPS_COUNT);

    if (CapIdx < 1*64)
        pCaps->m_Flags[0] &= ~(1LL<<CapIdx);
    else if (CapIdx < 2*64)
        pCaps->m_Flags[1] &= ~(1LL<<(CapIdx-1*64));
    else if (CapIdx < 3*64)
        pCaps->m_Flags[2] &= ~(1LL<<(CapIdx-2*64));
    else if (CapIdx < 4*64)
        pCaps->m_Flags[3] &= ~(1LL<<(CapIdx-3*64));
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoCapHasCaps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool DtIoCapsHasCap(DtIoCaps* pCaps, DtIoCapValue Cap)
{
    Int CapIdx = (Int)Cap;
    DT_ASSERT(CapIdx < DT_IOCAPS_COUNT);

    if (CapIdx < 1*64)
        return (pCaps->m_Flags[0] & 1LL<<CapIdx) != 0;
    else if (CapIdx < 2*64)
        return (pCaps->m_Flags[1] & 1LL<<(CapIdx-1*64)) != 0;
    else if (CapIdx < 3*64)
        return (pCaps->m_Flags[2] & 1LL<<(CapIdx-2*64)) != 0;
    else if (CapIdx < 4*64)
        return (pCaps->m_Flags[3] & 1LL<<(CapIdx-3*64)) != 0;
    DT_ASSERT(1 == 0);
    return FALSE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoCapsSetCap -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtIoCapsSetCap(DtIoCaps* pCaps, DtIoCapValue  Cap)
{
    Int CapIdx = (Int)Cap;
    DT_ASSERT(CapIdx < DT_IOCAPS_COUNT);

    if (CapIdx < 1*64)
        pCaps->m_Flags[0] |= 1LL<<CapIdx;
    else if (CapIdx < 2*64)
        pCaps->m_Flags[1] |= 1LL<<(CapIdx-1*64);
    else if (CapIdx < 3*64)
        pCaps->m_Flags[2] |= 1LL<<(CapIdx-2*64);
    else if (CapIdx < 4*64)
        pCaps->m_Flags[3] |= 1LL<<(CapIdx-3*64);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoCapDefinitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// DtIoCapDefinitions maps from cap value to cap name
//
const DtIoCapDef  DtIoCapDefinitions[] =
{
    // Capability group BOOLIO - Boolean I/O capabilities
    { DT_IOCAP_AUTOBFGEN,    "CAP_AUTOBFGEN" },
    { DT_IOCAP_DMATESTMODE,  "CAP_DMATESTMODE" },
    { DT_IOCAP_FAILSAFE,     "CAP_FAILSAFE" },
    { DT_IOCAP_FRACMODE,     "CAP_FRACMODE" },
    { DT_IOCAP_GENLOCKED,    "CAP_GENLOCKED" },
    { DT_IOCAP_GENREF,       "CAP_GENREF" },
    { DT_IOCAP_SWS2APSK,     "CAP_SWS2APSK" },

    // Capability group IODIR - I/O direction
    { DT_IOCAP_DISABLED,   "CAP_DISABLED" },
    { DT_IOCAP_INPUT,      "CAP_INPUT" },
    { DT_IOCAP_INTINPUT,   "CAP_INTINPUT" },
    { DT_IOCAP_INTOUTPUT,  "CAP_INTOUTPUT" },
    { DT_IOCAP_MONITOR,    "CAP_MONITOR" },
    { DT_IOCAP_OUTPUT,     "CAP_OUTPUT" },

    // Subcapabilities of IODIR, DTAPI_CAP_INPUT
    { DT_IOCAP_SHAREDANT,  "CAP_SHAREDANT" },

    // Subcapabilities of IODIR, DTAPI_CAP_OUTPUT
    { DT_IOCAP_DBLBUF,     "CAP_DBLBUF" },
    { DT_IOCAP_LOOPS2L3,   "CAP_LOOPS2L3" },
    { DT_IOCAP_LOOPS2TS,   "CAP_LOOPS2TS" },
    { DT_IOCAP_LOOPTHR,    "CAP_LOOPTHR" },

    // Capability group IOPROPS - Miscellaneous I/O properties
    { DT_IOCAP_ASIPOL,     "CAP_ASIPOL" },
    { DT_IOCAP_GENREFSLAVE,"CAP_GENREFSLAVE" },
    { DT_IOCAP_HUFFMAN,    "CAP_HUFFMAN" },
    { DT_IOCAP_IPPAIR,     "CAP_IPPAIR" },
    { DT_IOCAP_L3MODE,     "CAP_L3MODE" },
    { DT_IOCAP_MATRIX,     "CAP_MATRIX" },
    { DT_IOCAP_MATRIX2,    "CAP_MATRIX2" },
    { DT_IOCAP_QUADLINK,   "CAP_QUADLINK" },
    { DT_IOCAP_RAWASI,     "CAP_RAWASI" },
    { DT_IOCAP_SDI10BNBO,  "CAP_SDI10BNBO" },
    { DT_IOCAP_SDITIME,    "CAP_SDITIME" },
    { DT_IOCAP_TIMESTAMP64,"CAP_TIMESTAMP64" },
    { DT_IOCAP_TRPMODE,    "CAP_TRPMODE" },
    { DT_IOCAP_TS,         "CAP_TS" },
    { DT_IOCAP_TXONTIME,   "CAP_TXONTIME" },
    { DT_IOCAP_VIRTUAL,    "CAP_VIRTUAL" },

    // Capability group IOSTD - I/O standard
    { DT_IOCAP_12GSDI,     "CAP_12GSDI" },
    { DT_IOCAP_3GSDI,      "CAP_3GSDI" },
    { DT_IOCAP_6GSDI,      "CAP_6GSDI" },
    { DT_IOCAP_ASI,        "CAP_ASI" },
    { DT_IOCAP_AVENC,      "CAP_AVENC" },
    { DT_IOCAP_DEKTECST,   "CAP_DEKTECST" },
    { DT_IOCAP_DEMOD,      "CAP_DEMOD" },
    { DT_IOCAP_GPSTIME,    "CAP_GPSTIME" },
    { DT_IOCAP_HDMI,       "CAP_HDMI" },
    { DT_IOCAP_HDSDI,      "CAP_HDSDI" },
    { DT_IOCAP_IFADC,      "CAP_IFADC" },
    { DT_IOCAP_IP,         "CAP_IP" },
    { DT_IOCAP_MOD,        "CAP_MOD" },
    { DT_IOCAP_PHASENOISE, "CAP_PHASENOISE" },
    { DT_IOCAP_RS422,      "CAP_RS422" },
    { DT_IOCAP_SDIRX,      "CAP_SDIRX" },
    { DT_IOCAP_SDI,        "CAP_SDI" },
    { DT_IOCAP_SPI,        "CAP_SPI" },
    { DT_IOCAP_SPISDI,     "CAP_SPISDI" },

    // Subcapabilities of IOSTD, DTAPI_CAP_12GSDI
    { DT_IOCAP_2160P50,    "CAP_2160P50" },
    { DT_IOCAP_2160P50B,   "CAP_2160P50B" },
    { DT_IOCAP_2160P59_94, "CAP_2160P59_94" },
    { DT_IOCAP_2160P59_94B,"CAP_2160P59_94B" },
    { DT_IOCAP_2160P60,    "CAP_2160P60" },
    { DT_IOCAP_2160P60B,   "CAP_2160P60B" },

    // Subcapabilities of IOSTD, DTAPI_CAP_3GSDI
    { DT_IOCAP_1080P50,    "CAP_1080P50" },
    { DT_IOCAP_1080P50B,   "CAP_1080P50B" },
    { DT_IOCAP_1080P59_94, "CAP_1080P59_94" },
    { DT_IOCAP_1080P59_94B,"CAP_1080P59_94B" },
    { DT_IOCAP_1080P60,    "CAP_1080P60" },
    { DT_IOCAP_1080P60B,   "CAP_1080P60B" },

    // Subcapabilities of IOSTD, DTAPI_CAP_6GSDI
    { DT_IOCAP_2160P23_98, "CAP_2160P23_98" },
    { DT_IOCAP_2160P24,    "CAP_2160P24" },
    { DT_IOCAP_2160P25,    "CAP_2160P25" },
    { DT_IOCAP_2160P29_97, "CAP_2160P29_97" },
    { DT_IOCAP_2160P30,    "CAP_2160P30" },

    // Subcapabilities of IOSTD, DTAPI_CAP_HDSDI
    { DT_IOCAP_1080I50,    "CAP_1080I50" },
    { DT_IOCAP_1080I59_94, "CAP_1080I59_94" },
    { DT_IOCAP_1080I60,    "CAP_1080I60" },
    { DT_IOCAP_1080P23_98, "CAP_1080P23_98" },
    { DT_IOCAP_1080P24,    "CAP_1080P24" },
    { DT_IOCAP_1080P25,    "CAP_1080P25" },
    { DT_IOCAP_1080P29_97, "CAP_1080P29_97" },
    { DT_IOCAP_1080P30,    "CAP_1080P30" },
    { DT_IOCAP_1080PSF23_98, "CAP_1080PSF23_98" },
    { DT_IOCAP_1080PSF24,  "CAP_1080PSF24" },
    { DT_IOCAP_1080PSF25,  "CAP_1080PSF25" },
    { DT_IOCAP_1080PSF29_97, "CAP_1080PSF29_97" },
    { DT_IOCAP_1080PSF30,  "CAP_1080PSF30" },
    { DT_IOCAP_720P23_98,  "CAP_720P23_98" },
    { DT_IOCAP_720P24,     "CAP_720P24" },
    { DT_IOCAP_720P25,     "CAP_720P25" },
    { DT_IOCAP_720P29_97,  "CAP_720P29_97" },
    { DT_IOCAP_720P30,     "CAP_720P30" },
    { DT_IOCAP_720P50,     "CAP_720P50" },
    { DT_IOCAP_720P59_94,  "CAP_720P59_94" },
    { DT_IOCAP_720P60,     "CAP_720P60" },

    // Subcapabilities of IOSTD, DTAPI_CAP_SDI
    { DT_IOCAP_525I59_94,  "CAP_525I59_94" },
    { DT_IOCAP_625I50,     "CAP_625I50" },

    // Subcapabilities of IOSTD, DTAPI_CAP_SPISDI
    { DT_IOCAP_SPI525I59_94, "CAP_SPI525I59_94" },
    { DT_IOCAP_SPI625I50,  "CAP_SPI625I50" },

    // Capability group PWRMODE - Power mode
    { DT_IOCAP_MODHQ,      "CAP_MODHQ" },
    { DT_IOCAP_LOWPWR,     "CAP_LOWPWR" },

    // Capability group RFCLKS
    { DT_IOCAP_RFCLKEXT,   "CAP_RFCLKEXT" },
    { DT_IOCAP_RFCLKINT,   "CAP_RFCLKINT" },
    
    // Capability group SPICLKSEL - Parallel port clock source selection
    { DT_IOCAP_SPICLKEXT,   "CAP_SPICLKEXT" },
    { DT_IOCAP_SPICLKINT,   "CAP_SPICLKINT" },

    // Capability group SPIMODE - Parallel port mode
    { DT_IOCAP_SPIFIXEDCLK,  "CAP_SPIFIXEDCL" },
    { DT_IOCAP_SPIDVBMODE,   "CAP_SPIDVBMODE" },
    { DT_IOCAP_SPISER8B,     "CAP_SPISER8B" },
    { DT_IOCAP_SPISER10B,    "CAP_SPISER10B" },

    // Capability group SPISTD - Parallel port I/O standard
    { DT_IOCAP_SPILVDS1,     "CAP_SPILVDS1" },
    { DT_IOCAP_SPILVDS2,     "CAP_SPILVDS2" },
    { DT_IOCAP_SPILVTTL,     "CAP_SPILVTTL" },

    // Capability group TSRATE
    { DT_IOCAP_EXTTSRATE,  "CAP_EXTTSRATE" },
    { DT_IOCAP_EXTRATIO,   "CAP_EXTRATIO" },
    { DT_IOCAP_INTTSRATE,  "CAP_INTTSRATE" },
    { DT_IOCAP_LOCK2INP,   "CAP_LOCK2INP" },
};
ASSERT_SIZE(DtIoCapDefinitions, sizeof(DtIoCapDef)*DT_IOCAPS_COUNT);