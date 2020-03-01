//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoCaps.h *#*#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
//
// Driver common - IO capabilities - Declaration of IO capabilities unctions
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

#ifndef __DT_IO_CAPS_H
#define __DT_IO_CAPS_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// List of DT_IOCAP values, used for specifiying IO capabilities
typedef enum _DtIoCapValue
{
    // Capability group BOOLIO - Boolean I/O capabilities
    DT_IOCAP_AUTOBFGEN = 0,    // Automatic black-frame generation capability
    DT_IOCAP_DMATESTMODE,      // DMA-rate testmode is supported
    DT_IOCAP_FAILSAFE,         // A fail-over relay is available
    DT_IOCAP_FRACMODE,         // Fractional mode is supported
    DT_IOCAP_GENLOCKED,        // Locked to a genlock reference
    DT_IOCAP_GENREF,           // Genlock reference input
    DT_IOCAP_SWS2APSK,         // DVB-S2 APSK mode


    // Capability group IODIR - I/O direction
    DT_IOCAP_DISABLED,         // Port is disabled
    DT_IOCAP_INPUT,            // Uni-directional input
    DT_IOCAP_INTINPUT,         // Internal input port
    DT_IOCAP_INTOUTPUT,        // Internal output port
    DT_IOCAP_MONITOR,          // Monitor of input or output
    DT_IOCAP_OUTPUT,           // Uni-directional output

    // Subcapabilities of IODIR, DTAPI_CAP_INPUT
    DT_IOCAP_SHAREDANT,        // Get antenna signal from another port

    // Subcapabilities of IODIR, DTAPI_CAP_INTOUTPUT, DTAPI_CAP_OUTPUT
    DT_IOCAP_DBLBUF,           // Double buffered output
    DT_IOCAP_LOOPS2L3,         // Loop-through of DVB-S2 in L3-frames
    DT_IOCAP_LOOPS2TS,         // Loop-through of an DVB-S(2) input
    DT_IOCAP_LOOPTHR,          // Loop-through of another input

    // Capability group IOPROPS - Miscellaneous I/O properties
    DT_IOCAP_ASIPOL,           // ASI output signal can be inverted
    DT_IOCAP_GENREFSLAVE,      // Slaved genlock reference
    DT_IOCAP_HUFFMAN,          // Huffman coding for SDI
    DT_IOCAP_IPPAIR,           // Network port supports failover
    DT_IOCAP_L3MODE,           // L3-frame mode
    DT_IOCAP_MATRIX,           // Matrix API support
    DT_IOCAP_MATRIX2,          // High-level Matrix API support
    DT_IOCAP_QUADLINK,         // Forms a Quad-link group with next 3 ports
    DT_IOCAP_RAWASI,           // Raw ASI
    DT_IOCAP_SDI10BNBO,        // 10-bit network byte order
    DT_IOCAP_SDITIME,          // SDI timestamping
    DT_IOCAP_TIMESTAMP64,      // 64-bit timestamping
    DT_IOCAP_TRPMODE,          // Transparent mode
    DT_IOCAP_TS,               // MPEG-2 transport stream
    DT_IOCAP_TXONTIME,         // Transmit on timestamp
    DT_IOCAP_VIRTUAL,          // Virtual port, no physical connection

    // Capability group IOSTD - I/O standard
    DT_IOCAP_12GSDI,           // 12G-SDI
    DT_IOCAP_3GSDI,            // 3G-SDI
    DT_IOCAP_6GSDI,            // 6G-SDI
    DT_IOCAP_ASI,              // DVB-ASI transport stream
    DT_IOCAP_AVENC,            // Audio/video encoder
    DT_IOCAP_DEKTECST,         // DekTec Streaming-data
    DT_IOCAP_DEMOD,            // Demodulation
    DT_IOCAP_GPSTIME,          // 1PPS and 10MHz GPS-clock input
    DT_IOCAP_HDMI,             // HDMI
    DT_IOCAP_HDSDI,            // HD-SDI
    DT_IOCAP_IFADC,            // IF A/D converter
    DT_IOCAP_IP,               // Transport stream over IP
    DT_IOCAP_MOD,              // Modulator output
    DT_IOCAP_PHASENOISE,       // Phase noise injection
    DT_IOCAP_RS422,            // RS422 port
    DT_IOCAP_SDIRX,            // SDI receiver
    DT_IOCAP_SDI,              // SD-SDI
    DT_IOCAP_SPI,              // DVB-SPI transport stream
    DT_IOCAP_SPISDI,           // SD-SDI on a parallel port

    // Subcapabilities of IOSTD, DTAPI_CAP_12GSDI
    DT_IOCAP_2160P50,          // 2160p/50 lvl A
    DT_IOCAP_2160P50B,         // 2160p/50 lvl B
    DT_IOCAP_2160P59_94,       // 2160p/59.94 lvl A
    DT_IOCAP_2160P59_94B,      // 2160p/59.94 lvl B
    DT_IOCAP_2160P60,          // 2160p/60 lvl A
    DT_IOCAP_2160P60B,         // 2160p/60 lvl B

    // Subcapabilities of IOSTD, DTAPI_CAP_3GSDI
    DT_IOCAP_1080P50,          // 1080p/50 lvl A
    DT_IOCAP_1080P50B,         // 1080p/50 lvl B
    DT_IOCAP_1080P59_94,       // 1080p/59.94 lvl A
    DT_IOCAP_1080P59_94B,      // 1080p/59.94 lvl B
    DT_IOCAP_1080P60,          // 1080p/60 lvl A
    DT_IOCAP_1080P60B,         // 1080p/60 lvl B

    // Subcapabilities of IOSTD, DTAPI_CAP_6GSDI
    DT_IOCAP_2160P23_98,       // 2160p/23.98
    DT_IOCAP_2160P24,          // 2160p/24
    DT_IOCAP_2160P25,          // 2160p/25
    DT_IOCAP_2160P29_97,       // 2160p/29.97
    DT_IOCAP_2160P30,          // 2160p/30

    // Subcapabilities of IOSTD, DTAPI_CAP_HDSDI
    DT_IOCAP_1080I50,          // 1080i/50
    DT_IOCAP_1080I59_94,       // 1080i/59.94
    DT_IOCAP_1080I60,          // 1080i/60
    DT_IOCAP_1080P23_98,       // 1080p/23.98
    DT_IOCAP_1080P24,          // 1080p/24
    DT_IOCAP_1080P25,          // 1080p/25
    DT_IOCAP_1080P29_97,       // 1080p/29.97
    DT_IOCAP_1080P30,          // 1080p/30
    DT_IOCAP_1080PSF23_98,     // 1080psf/23.98
    DT_IOCAP_1080PSF24,        // 1080psf/24
    DT_IOCAP_1080PSF25,        // 1080psf/25
    DT_IOCAP_1080PSF29_97,     // 1080psf/29.97
    DT_IOCAP_1080PSF30,        // 1080psf/30
    DT_IOCAP_720P23_98,        // 720p/23.98
    DT_IOCAP_720P24,           // 720p/24
    DT_IOCAP_720P25,           // 720p/25
    DT_IOCAP_720P29_97,        // 720p/29.97
    DT_IOCAP_720P30,           // 720p/30
    DT_IOCAP_720P50,           // 720p/50
    DT_IOCAP_720P59_94,        // 720p/59.94
    DT_IOCAP_720P60,           // 720p/60

    // Subcapabilities of IOSTD, DTAPI_CAP_SDI
    DT_IOCAP_525I59_94,        // 525i/59.94
    DT_IOCAP_625I50,           // 625i/50

    // Subcapabilities of IOSTD, DTAPI_CAP_SPISDI
    DT_IOCAP_SPI525I59_94,     // SPI 525i/59.94
    DT_IOCAP_SPI625I50,        // SPI 625i/50

    // Capability group PWRMODE - Power mode
    DT_IOCAP_MODHQ,            // High-quality modulation
    DT_IOCAP_LOWPWR,           // Low-power mode

    // Capability group RFCLKSEL - RF clock source selection
    DT_IOCAP_RFCLKEXT,         // External RF clock input
    DT_IOCAP_RFCLKINT,         // Internal RF clock reference

    // Capability group SPICLKSEL - Parallel port clock source selection
    DT_IOCAP_SPICLKEXT,        // External clock input
    DT_IOCAP_SPICLKINT,        // Internal clock reference

    // Capability group SPIMODE - Parallel port mode
    DT_IOCAP_SPIFIXEDCLK,      // SPI fixed clock with valid signal
    DT_IOCAP_SPIDVBMODE,       // SPI DVB mode
    DT_IOCAP_SPISER8B,         // SPI serial 8-bit mode
    DT_IOCAP_SPISER10B,        // SPI serial 10-bit mode

    // Capability group SPISTD - Parallel port I/O standard
    DT_IOCAP_SPILVDS1,         // LVDS1
    DT_IOCAP_SPILVDS2,         // LVDS2
    DT_IOCAP_SPILVTTL,         // LVTTL

    // Capability group TSRATESEL - Transport-stream rate selection
    DT_IOCAP_EXTTSRATE,        // External TS rate clock input
    DT_IOCAP_EXTRATIO,         // External TS rate clock with ratio
    DT_IOCAP_INTTSRATE,        // Internal TS rate clock reference
    DT_IOCAP_LOCK2INP,         // Lock TS rate to input port
    DT_IOCAPS_COUNT
} DtIoCapValue;

// Type to store a IO capability and corresponding name
typedef struct _DtIoCapDef
{
    DtIoCapValue m_Cap;
    const char*  m_pCapName;
} DtIoCapDef;

// Type to store IO-capabilities
typedef struct _DtIoCaps
{
    UInt64  m_Flags[4];
}DtIoCaps;

// DtIoCapDefinitions maps from cap value to cap name
extern const DtIoCapDef  DtIoCapDefinitions[];

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

void DtIoCapsClearCaps(DtIoCaps* pCaps);
void DtIoCapsClearCap(DtIoCaps* pCaps, DtIoCapValue  Cap);
Bool DtIoCapsHasCap(DtIoCaps* pCaps, DtIoCapValue Cap);
void DtIoCapsSetCap(DtIoCaps* pCaps, DtIoCapValue  Cap);


#endif // __DT_IO_CAPS_H
