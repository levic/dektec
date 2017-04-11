//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaHdmiTx.c *#*#*#*#*#*#*#*#*# (C) 2015-2016 DekTec
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2015 DekTec Digital Video B.V.
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

#include <DtaIncludes.h>            // Standard driver includes

#pragma pack (push, 1)
typedef struct {
    union {
        struct {
            UInt32  m_Checksum:8;
            UInt32  m_ScanInfo:2;
            UInt32  m_BarInfoDataValid:2;
            UInt32  m_ActiveInfoPresent:1;
            UInt32  m_YCbCrIndicator:2;
            UInt32  m_Reserved1:1;
            UInt32  m_ActiveFormatAspectRatio:4;
            UInt32  m_PictureAspectRatio:2;
            UInt32  m_Colorimetry:2;
            UInt32  m_NonUniformPictureScaling:2;
            UInt32  m_QuantizationRange:2;
            UInt32  m_ExtendedColorimetry:3;
            UInt32  m_ItContent:1;
            //
            UInt32  m_VIC:7;
            UInt32  m_Reserved2:1;
            UInt32  m_PictureRepetitionFactor:4;
            UInt32  m_ContentType:2;
            UInt32  m_YCCQuantizationRange:2;
            UInt32  m_LineNumberEndOfTopBar:16;
            //
            UInt32  m_LineNumberStartOfBottomBar:16;
            UInt32  m_PixelNumberEndOfLeftBar:16;
            //
            UInt32  m_PixelNumberStartOfRightBar:16;
            UInt32  m_Reserved3:16;
        };
        UInt32  m_Data[4];
    };
} AVI_INFOFRAME;

typedef struct {
    union {
        struct {
            UInt64  m_Length:5;
            UInt64  m_Checksum:8;
            UInt64  m_IEEE:24;
            UInt64  m_Reserved1:5;
            UInt64  m_HdmiVideoFormat:3;
            UInt64  m_HdmiVIC:8;
            UInt64  m_Reserved2:5;
            UInt64  m_3D_ExtData:3;
            UInt64  m_Reserved3:3;
        };
        UInt32  m_Data[2];
        UInt64  m_Data64;
    };
} HDMIVS_INFOFRAME;

typedef struct {
    union {
        struct {
            UInt32  m_Checksum:8;
            UInt32  m_CC:3;
            UInt32  m_Reserved1:1;
            UInt32  m_CT:4;
            UInt32  m_SS:2;
            UInt32  m_SF:3;
            UInt32  m_Reserved2:3;
            UInt32  m_CXT:8;
            UInt32  m_CA:8;
            UInt32  m_LFEPBL:2;
            UInt32  m_Reserved3:1;
            UInt32  m_LSV:4;
            UInt32  m_DM_INH:1;
            UInt32  m_Reserved4:16;
        };
        UInt32  m_Data[2];
    };
} AUD_INFOFRAME;

#pragma pack (pop)

typedef struct _HDMI_EDID_DTD
{
    UInt  m_HActivePixels;
    UInt  m_HBlankingPixels;
    UInt  m_VActiveLines;
    UInt  m_VBlankingLines;
    UInt  m_HFrontPorchPixels;
    UInt  m_HSyncPulseWidth;
    UInt  m_VFrontPorchLines;
    UInt  m_VSyncPulseWidth;
    UInt  m_HActiveVideoSize;
    UInt  m_VActiveVideoSize;
    UInt  m_HBorderPixels;
    UInt  m_VBorderLines;
    UInt  m_PixelClock;
    UInt  m_Flags;
} HDMI_EDID_DTD;


// SCDC Offset defines
#define  SCDC_SINK_VERSION_OFFSET           0x01
#define  SCDC_SOURCE_VERSION_OFFSET         0x02
#define  SCDC_UPDATA_FLAGS1_OFFSET          0x10
#define  SCDC_UPDATE_FLAGS2_OFFSET          0x11
#define  SCDC_TMDS_CONFIG_OFFSET            0x20
#define  SCDC_SCRAMBLER_STATUS_OFFSET       0x21
#define  SCDC_CONFIG_0_OFFSET               0x30
#define  SCDC_MANUF_OUI_1_OFFSET            0xD0
#define  SCDC_MANUF_OUI_2_OFFSET            0xD1
#define  SCDC_MANUF_OUI_3_OFFSET            0xD2
// 0xD3..0xDA: Device ID string
// 0xDB HW Version
// 0xDC SW Major rev.
// 0xDD SW Minor rev.
// 0xDE..FF Manuf. specific

// SCDC Bit defines
#define  SCDC_TMDS_CONFIG_SCRAMBLING_ENABLE  (1<<0)
#define  SCDC_TMDS_CONFIG_TMDS_CLK_RATIO     (1<<1)

#define  SCDC_CONFIG_0_RR_ENABLE             (1<<0)

#define  SN65DP159_I2C_ADDR                  0xBC

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
Int  DtHdmiEdidParseEedid(DtaHdmiTx* pHdmiTx, UInt8* pBlock0);
DtStatus  DtHdmiEdidParseExtension(DtaHdmiTx* pHdmiTx, UInt8* pBlockX);
DtStatus  DtHdmiTxEnableHdmiOutputDriver(DtaHdmiTx* pHdmiTx, Bool Enable);
Int  DtHdmiTxGetIndexVidStd(DtaHdmiVidStd VidStd);
DtStatus  DtHdmiTxUpdateVideoStd(DtaHdmiTx* pHdmiTx, DtaHdmiVidStd VidStd, Bool Force);
DtStatus  DtHdmiTxInitHdmiOutputDriver(DtaHdmiTx* pHdmiTx);


#define HDMI_IEEE_LLC_IDENTIFIER            0x000c03
#define HDMI_IEEE_FORUM_IDENTIFIER          0xc45dd8

//[Tmds][AudioFormats][N,CTS]
static const Int  DT_HDMI_AUDIO_FORMAT[][3][2] =
{   //1x32kHz      ,1x44.1kHz     ,1x48kHz
    {{4096,27000}  ,{6272,30000}  ,{6144,27000}},  // DT_HDMI_PIXCLK_27M
    //{{4096,27027}  ,{6272,30030}  ,{6144,27027}},  // DT_HDMI_PIXCLK_27M*1.001
    {{4096,74250}  ,{6272,82500}  ,{6144,74250}},  // DT_HDMI_PIXCLK_74.25
    {{11648,210937},{17836,234375},{11648,140625}},  // DT_HDMI_PIXCLK_74.25/1.001
    {{4096,148500} ,{6272,165000} ,{6144,148500}},  // DT_HDMI_PIXCLK_148.5
    {{11648,421875},{8918,234375} ,{5824,140625}},  // DT_HDMI_PIXCLK_148.5/1.001
    {{4096,297000} ,{4704,247500} ,{5120,247500}},  // DT_HDMI_PIXCLK_297 !! not recommended 32khz
    {{5824,421875} ,{4459,234375} ,{5824,281250}},  // DT_HDMI_PIXCLK_297/1.001
    {{4096, 594000},{9408,990000} ,{6144,594000}},  // DT_HDMI_PIXCLK_594 !! not recommended for 32khz
    {{5824, 843750},{8918,937500} ,{5824,562500}},  // DT_HDMI_PIXCLK_594/1.001
};

#define HDMI_IDX_VIC            0
#define HDMI_IDX_VIDSTD         1
#define HDMI_IDX_H_ACT          2
#define HDMI_IDX_V_ACT          3
#define HDMI_IDX_H_BLANC        4
#define HDMI_IDX_V_BLANC        5
#define HDMI_IDX_H_SYNC         6
#define HDMI_IDX_H_FPORCH       7
#define HDMI_IDX_H_BPORCH       8
#define HDMI_IDX_F1_V_SYNC      9
#define HDMI_IDX_F1_V_FPORCH    10
#define HDMI_IDX_F1_V_BPORCH    11
#define HDMI_IDX_F2_V_SYNC      12
#define HDMI_IDX_F2_V_FPORCH    13
#define HDMI_IDX_F2_V_BPORCH    14
#define HDMI_IDX_PIXEL_CLK      15
#define HDMI_IDX_FLAGS          16

// Text: "Video format not supported"
// Created through Paint
unsigned char textData[2048] = {
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x7F, 0xFF, 0x03,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xE0, 0x7F, 0xFF, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x7F, 0xFF, 0x03, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x7F, 0xFF, 0x03,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xE0, 0x7F, 0xFF, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x7F, 0xFF, 0x03, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0xFF, 0xFF, 0xC0, 0x1F, 0xF8, 0x1F, 0xE0, 0xC0, 0xFF, 0x80, 0x7F, 0xFE,
    0x07, 0xFF, 0xFF, 0xE0, 0x7F, 0xE0, 0x7F, 0xE0, 0x7F, 0x81, 0xF0, 0x3E,
    0x07, 0xE0, 0x70, 0x3F, 0x80, 0x7F, 0xFE, 0x07, 0x81, 0xFF, 0x81, 0xFF,
    0xF0, 0x0F, 0xFF, 0xF0, 0x07, 0xFF, 0x83, 0x03, 0xE0, 0x60, 0xFF, 0x03,
    0x07, 0xFF, 0xC0, 0xFF, 0xC0, 0xFF, 0xE0, 0x1F, 0xC0, 0x3F, 0xFC, 0x18,
    0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xC0, 0x1F, 0xF8, 0x1F, 0xC0, 0x00,
    0xFE, 0x00, 0x1F, 0xF0, 0x00, 0xFF, 0xFF, 0xE0, 0x7F, 0x00, 0x0F, 0xE0,
    0x7F, 0x81, 0xF0, 0x3E, 0x07, 0x80, 0x30, 0x7F, 0x00, 0x7F, 0xFE, 0x07,
    0x81, 0xFC, 0x00, 0x3F, 0xE0, 0x0F, 0xFF, 0xC0, 0x00, 0xFE, 0x01, 0x03,
    0xE0, 0x00, 0x3F, 0x00, 0x01, 0xFE, 0x00, 0x1F, 0xC0, 0xFF, 0xC0, 0x1F,
    0x00, 0x0F, 0xF8, 0x00, 0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xC0, 0x1F,
    0xF8, 0x1F, 0x80, 0x00, 0xFC, 0x00, 0x0F, 0xE0, 0x00, 0x7F, 0xFF, 0xE0,
    0x7E, 0x00, 0x07, 0xE0, 0x7F, 0x81, 0xF0, 0x3E, 0x07, 0x00, 0x00, 0x7E,
    0x00, 0x7F, 0xFE, 0x07, 0x81, 0xF8, 0x00, 0x1F, 0xC0, 0x0F, 0xFF, 0x80,
    0x00, 0x7E, 0x00, 0x03, 0xE0, 0x00, 0x3F, 0x00, 0x01, 0xFC, 0x00, 0x0F,
    0xC0, 0xFF, 0x80, 0x1E, 0x00, 0x07, 0xF0, 0x00, 0x1F, 0xFF, 0x00, 0x00,
    0xFF, 0xFF, 0x80, 0x0F, 0xF8, 0x1F, 0x00, 0x00, 0xF8, 0x00, 0x07, 0xC0,
    0x00, 0x3F, 0xFF, 0xE0, 0x7C, 0x00, 0x03, 0xE0, 0x7F, 0x81, 0xF0, 0x3E,
    0x07, 0x00, 0x00, 0x7E, 0x00, 0x7F, 0xFE, 0x07, 0x81, 0xF0, 0x00, 0x0F,
    0xC0, 0x0F, 0xFF, 0x81, 0xE0, 0x7C, 0x00, 0x03, 0xE0, 0x00, 0x1F, 0x00,
    0x00, 0xF8, 0x00, 0x07, 0xC0, 0xFF, 0x80, 0x1C, 0x00, 0x03, 0xE0, 0x00,
    0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x0F, 0xF8, 0x1F, 0x03, 0x80,
    0xF8, 0x1E, 0x03, 0xC0, 0xF0, 0x3F, 0xFF, 0xE0, 0x7C, 0x0F, 0x03, 0xE0,
    0x7F, 0x81, 0xF0, 0x3E, 0x07, 0x03, 0xC0, 0x7E, 0x07, 0xFF, 0xFE, 0x07,
    0x81, 0xF0, 0x3C, 0x0F, 0xC0, 0xFF, 0xFF, 0x01, 0xF0, 0x3C, 0x00, 0x03,
    0xE0, 0x38, 0x1F, 0x01, 0xC0, 0xF8, 0x1E, 0x07, 0xC0, 0xFF, 0x81, 0xFC,
    0x0F, 0x01, 0xE0, 0x70, 0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x80, 0x0F,
    0xF8, 0x1E, 0x07, 0xC0, 0xF0, 0x3F, 0xFF, 0x80, 0xF0, 0x1F, 0xFF, 0xE0,
    0x78, 0x0F, 0x01, 0xE0, 0x7F, 0x81, 0xF0, 0x3E, 0x07, 0x03, 0xE0, 0x7E,
    0x07, 0xFF, 0xFE, 0x07, 0x81, 0xE0, 0x3C, 0x07, 0xC0, 0xFF, 0xFF, 0xE3,
    0xF0, 0x3C, 0x06, 0x03, 0xE0, 0x7C, 0x0F, 0x03, 0xE0, 0x70, 0x1E, 0x03,
    0xC0, 0xFF, 0x81, 0xF8, 0x1F, 0xFF, 0xC0, 0xF8, 0x1F, 0xFF, 0x00, 0x00,
    0xFF, 0xFF, 0x02, 0x07, 0xF8, 0x1E, 0x07, 0xC0, 0xF0, 0x3F, 0xFF, 0x81,
    0xF8, 0x1F, 0xFF, 0xE0, 0x78, 0x1F, 0x81, 0xE0, 0x7F, 0x81, 0xF0, 0x3E,
    0x07, 0x01, 0xE0, 0x7E, 0x07, 0xFF, 0xFE, 0x07, 0x81, 0xE0, 0x7E, 0x07,
    0xC0, 0xFF, 0xFF, 0xFF, 0x00, 0x3C, 0x0F, 0x03, 0xE0, 0x7C, 0x0F, 0x03,
    0xE0, 0x70, 0x3F, 0x03, 0xC0, 0xFF, 0x81, 0xF8, 0x1F, 0xFF, 0xC0, 0xF8,
    0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0x02, 0x07, 0xF8, 0x1E, 0x07, 0xC0,
    0xF0, 0x00, 0x03, 0x81, 0xF8, 0x1F, 0xFF, 0xE0, 0x78, 0x1F, 0x81, 0xE0,
    0x7F, 0x81, 0xF0, 0x3E, 0x07, 0x80, 0x60, 0x7E, 0x07, 0xFF, 0xFE, 0x07,
    0x81, 0xE0, 0x7E, 0x07, 0xC0, 0xFF, 0xFF, 0xF0, 0x00, 0x3C, 0x0F, 0x03,
    0xE0, 0x7C, 0x0F, 0x03, 0xE0, 0x70, 0x3F, 0x03, 0xC0, 0xFF, 0x81, 0xF8,
    0x00, 0x01, 0xC0, 0xF8, 0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xFE, 0x02, 0x03,
    0xF8, 0x1E, 0x07, 0xC0, 0xF0, 0x00, 0x03, 0x81, 0xF8, 0x1F, 0xFF, 0xE0,
    0x78, 0x1F, 0x81, 0xE0, 0x7F, 0x81, 0xF0, 0x3E, 0x07, 0xC0, 0x00, 0x7E,
    0x07, 0xFF, 0xFE, 0x07, 0x81, 0xE0, 0x7E, 0x07, 0xC0, 0xFF, 0xFF, 0xE0,
    0x00, 0x7C, 0x0F, 0x03, 0xE0, 0x7C, 0x0F, 0x03, 0xE0, 0x70, 0x3F, 0x03,
    0xC0, 0xFF, 0x81, 0xF8, 0x00, 0x01, 0xC0, 0xF8, 0x1F, 0xFF, 0x00, 0x00,
    0xFF, 0xFE, 0x02, 0x03, 0xF8, 0x1E, 0x07, 0xC0, 0xF0, 0x00, 0x03, 0x81,
    0xF8, 0x1F, 0xFF, 0xE0, 0x78, 0x1F, 0x81, 0xE0, 0x7F, 0x81, 0xF0, 0x3E,
    0x07, 0xFC, 0x00, 0x7E, 0x07, 0xFF, 0xFE, 0x07, 0x81, 0xE0, 0x7E, 0x07,
    0xC0, 0xFF, 0xFF, 0xC0, 0x00, 0xFC, 0x0F, 0x03, 0xE0, 0x7C, 0x0F, 0x03,
    0xE0, 0x70, 0x3F, 0x03, 0xC0, 0xFF, 0x81, 0xF8, 0x00, 0x01, 0xC0, 0xF8,
    0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xFE, 0x07, 0x03, 0xF8, 0x1E, 0x07, 0xC0,
    0xF0, 0x3F, 0x03, 0x81, 0xF8, 0x1F, 0xFF, 0xE0, 0x78, 0x1F, 0x81, 0xE0,
    0x3F, 0x81, 0xF0, 0x3E, 0x07, 0xFF, 0xC0, 0x7E, 0x07, 0xFF, 0xFE, 0x07,
    0x81, 0xE0, 0x7E, 0x07, 0xC0, 0xFF, 0xFF, 0x80, 0x07, 0xFC, 0x0F, 0x03,
    0xE0, 0x7C, 0x0F, 0x03, 0xE0, 0x70, 0x3F, 0x03, 0xC0, 0x7F, 0x81, 0xF8,
    0x1F, 0x81, 0xC0, 0xF8, 0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xFC, 0x07, 0x01,
    0xF8, 0x1E, 0x07, 0xC0, 0xF0, 0x3F, 0x03, 0x80, 0xF0, 0x1F, 0xFF, 0xE0,
    0x78, 0x0F, 0x01, 0xE0, 0x1F, 0x80, 0xE0, 0x1C, 0x07, 0xF1, 0xE0, 0x7E,
    0x07, 0xFF, 0xFE, 0x03, 0x01, 0xE0, 0x3C, 0x07, 0xC0, 0xFF, 0xFF, 0x80,
    0xFF, 0xFC, 0x0F, 0x03, 0xE0, 0x7C, 0x0F, 0x03, 0xE0, 0x70, 0x1E, 0x03,
    0xC0, 0x3F, 0x81, 0xF8, 0x1F, 0x81, 0xC0, 0xF8, 0x1F, 0xFF, 0x00, 0x00,
    0xFF, 0xFC, 0x07, 0x01, 0xF8, 0x1F, 0x03, 0x80, 0xF8, 0x1E, 0x07, 0xC0,
    0xF0, 0x3F, 0xFF, 0x80, 0x0C, 0x0F, 0x03, 0xE0, 0x03, 0x80, 0x00, 0x00,
    0x07, 0x80, 0xE0, 0x78, 0x00, 0x7F, 0xFE, 0x00, 0x01, 0xF0, 0x3C, 0x0F,
    0x00, 0x0F, 0xFF, 0x81, 0xE3, 0xFC, 0x0F, 0x03, 0xE0, 0x38, 0x1F, 0x01,
    0xC0, 0xF8, 0x1E, 0x07, 0xC0, 0x06, 0x00, 0x1C, 0x0F, 0x03, 0xE0, 0x70,
    0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xFC, 0x0F, 0x81, 0xF8, 0x1F, 0x00, 0x00,
    0xF8, 0x00, 0x07, 0xC0, 0x00, 0x3F, 0xFF, 0x80, 0x0C, 0x00, 0x03, 0xE0,
    0x01, 0x80, 0x00, 0x00, 0x07, 0x80, 0x00, 0x78, 0x00, 0x7F, 0xFE, 0x00,
    0x01, 0xF0, 0x00, 0x0F, 0x00, 0x0F, 0xFF, 0x81, 0xC0, 0x7C, 0x0F, 0x03,
    0xE0, 0x00, 0x1F, 0x00, 0x00, 0xF8, 0x00, 0x07, 0xC0, 0x02, 0x00, 0x1C,
    0x00, 0x03, 0xE0, 0x00, 0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xF8, 0x0F, 0x80,
    0xF8, 0x1F, 0x80, 0x00, 0xFC, 0x00, 0x0F, 0xE0, 0x00, 0x7F, 0xFF, 0x80,
    0x0E, 0x00, 0x07, 0xE0, 0x01, 0x80, 0x00, 0x40, 0x0F, 0xC0, 0x00, 0xF8,
    0x00, 0x7F, 0xFE, 0x00, 0x03, 0xF8, 0x00, 0x1F, 0x00, 0x0F, 0xFF, 0xC0,
    0x00, 0xFC, 0x0F, 0x03, 0xE0, 0x00, 0x3F, 0x00, 0x01, 0xFC, 0x00, 0x0F,
    0xC0, 0x02, 0x00, 0x1E, 0x00, 0x07, 0xF0, 0x00, 0x1F, 0xFF, 0x00, 0x00,
    0xFF, 0xF8, 0x0F, 0x80, 0xF8, 0x1F, 0x80, 0x00, 0xFE, 0x00, 0x1F, 0xF8,
    0x00, 0xFF, 0xFF, 0x80, 0x0F, 0x80, 0x0F, 0xE0, 0x40, 0x81, 0x00, 0x60,
    0x1F, 0xE0, 0x01, 0xF8, 0x00, 0x7F, 0xFE, 0x04, 0x03, 0xFE, 0x00, 0x3F,
    0x00, 0x0F, 0xFF, 0xE0, 0x01, 0xFC, 0x0F, 0x03, 0xE0, 0x00, 0x7F, 0x00,
    0x03, 0xFF, 0x00, 0x1F, 0xC0, 0x80, 0x00, 0x1F, 0x00, 0x0F, 0xF0, 0x00,
    0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xF0, 0x1F, 0xC0, 0x78, 0x1F, 0xE0, 0xC0,
    0xFF, 0x80, 0x7F, 0xFE, 0x03, 0xFF, 0xFF, 0x80, 0x0F, 0xE0, 0x3F, 0xE0,
    0x60, 0x81, 0xC1, 0xF0, 0x3F, 0xF8, 0x03, 0xF8, 0x00, 0x7F, 0xFE, 0x06,
    0x0F, 0xFF, 0x80, 0xFF, 0x00, 0x0F, 0xFF, 0xF8, 0x07, 0xFC, 0x0F, 0x03,
    0xE0, 0x60, 0xFF, 0x03, 0x07, 0xFF, 0xC0, 0x7F, 0xC0, 0xC0, 0x00, 0x1F,
    0xC0, 0x3F, 0xFC, 0x18, 0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xF0, 0x1F, 0xC0,
    0x7F, 0xFF, 0xFF, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
    0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE,
    0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x1F, 0xFF, 0x00, 0x00,
    0xFF, 0xF0, 0x1F, 0xC0, 0x7F, 0xFF, 0xFF, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xE0, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFE, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8,
    0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xE0, 0x3F, 0xE0, 0x38, 0x1F, 0xFF, 0xC0,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x07, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x07, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x81, 0xFF,
    0xFF, 0xFF, 0xFF, 0xF8, 0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xE0, 0x3F, 0xE0,
    0x38, 0x1F, 0xFF, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0,
    0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xC1, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x1F, 0xFF, 0x00, 0x00,
    0xFF, 0xE0, 0x3F, 0xE0, 0x38, 0x1F, 0xFF, 0xC0, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xF0, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xC7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8,
    0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xC0, 0x3F, 0xE0, 0x18, 0x1F, 0xFF, 0xC0,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x07, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF7, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFD, 0xFF,
    0xFF, 0xFF, 0xFF, 0xF8, 0x1F, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00
};

// Table used to convert between video standard and HDMI video format
static const  Int  DT_HDMI_VIDSTD_2_FORMAT[][17] =
{
    //VIC, Format, HAW, FVAW, HBL, VBL, HS, HFPW, HBPW, F1VSW, F1VFPW, F1VBPW, F2VSW, F2VFPW, F2VBPW, PixelClk, Flags
    // HDMI Test pattern
    //{   3, DT_VIDSTD_480P60,    720, 480, 138, 45, 62, 16, 60, 6, 9, 30, -1, -1, -1,  DT_HDMI_PIXCLK_27M, DT_HDMI_FLAGS_HSPOL_NEG | DT_HDMI_FLAGS_VSPOL_NEG | DT_HDMI_FLAGS_MULTIPLMODE},
    {   2, DT_VIDSTD_480P59_94, 720, 480, 138, 45, 62, 16, 60, 6, 9, 30, -1, -1, -1,  DT_HDMI_PIXCLK_27M, DT_HDMI_FLAGS_HSPOL_NEG | DT_HDMI_FLAGS_VSPOL_NEG | DT_HDMI_FLAGS_ASPECT_RATIO_4_3}, // !! NO FRAC MODE == CORRECT!!
    {   3, DT_VIDSTD_480P59_94, 720, 480, 138, 45, 62, 16, 60, 6, 9, 30, -1, -1, -1,  DT_HDMI_PIXCLK_27M, DT_HDMI_FLAGS_HSPOL_NEG | DT_HDMI_FLAGS_VSPOL_NEG}, // !! NO FRAC MODE == CORRECT!!
    
    // SD formats
    {   6, DT_VIDSTD_525I59_94, 1440, 480/2, 276, 22, 124, 38, 114, 3, 4, 15, 3, 4, 16, DT_HDMI_PIXCLK_27M, DT_HDMI_FLAGS_INTERLACED | DT_HDMI_FLAGS_HSPOL_NEG | DT_HDMI_FLAGS_VSPOL_NEG | DT_HDMI_FLAGS_PIXELREP | DT_HDMI_FLAGS_ASPECT_RATIO_4_3},
    {   7, DT_VIDSTD_525I59_94, 1440, 480/2, 276, 22, 124, 38, 114, 3, 4, 15, 3, 4, 16, DT_HDMI_PIXCLK_27M, DT_HDMI_FLAGS_INTERLACED | DT_HDMI_FLAGS_HSPOL_NEG | DT_HDMI_FLAGS_VSPOL_NEG | DT_HDMI_FLAGS_PIXELREP},
    {  21, DT_VIDSTD_625I50, 1440, 576/2, 288, 24, 126, 24, 138, 3, 2, 19, 3, 2, 20, DT_HDMI_PIXCLK_27M, DT_HDMI_FLAGS_INTERLACED | DT_HDMI_FLAGS_HSPOL_NEG | DT_HDMI_FLAGS_VSPOL_NEG | DT_HDMI_FLAGS_PIXELREP | DT_HDMI_FLAGS_ASPECT_RATIO_4_3},
    {  22, DT_VIDSTD_625I50, 1440, 576/2, 288, 24, 126, 24, 138, 3, 2, 19, 3, 2, 20, DT_HDMI_PIXCLK_27M, DT_HDMI_FLAGS_INTERLACED | DT_HDMI_FLAGS_HSPOL_NEG | DT_HDMI_FLAGS_VSPOL_NEG | DT_HDMI_FLAGS_PIXELREP},

    // HD 720 formats
    // Removed support for VIC 60, video timing CEA-861-E spec is not compatible with SMPTE296M spec.
    //{  60, DT_VIDSTD_720P23_98, 1280, 720, 2845, 30, 40, 2585, 220, 5, 5, 20, -1,-1,-1, DT_HDMI_PIXCLK_74M25, DT_HDMI_FLAGS_FRACMODE},
    //{  60, DT_VIDSTD_720P24,    1280, 720, 2845, 30, 40, 2585, 220, 5, 5, 20, -1,-1,-1, DT_HDMI_PIXCLK_74M25, 0},
    {  61, DT_VIDSTD_720P25,    1280, 720, 2680, 30, 40, 2420, 220, 5, 5, 20, -1,-1,-1, DT_HDMI_PIXCLK_74M25, 0},
    {  62, DT_VIDSTD_720P29_97, 1280, 720, 2020, 30, 40, 1760, 220, 5, 5, 20, -1,-1,-1, DT_HDMI_PIXCLK_74M25, DT_HDMI_FLAGS_FRACMODE},
    {  62, DT_VIDSTD_720P30,    1280, 720, 2020, 30, 40, 1760, 220, 5, 5, 20, -1,-1,-1, DT_HDMI_PIXCLK_74M25, 0},
    {  19, DT_VIDSTD_720P50,    1280, 720,  700, 30, 40, 440,  220, 5, 5, 20, -1,-1,-1, DT_HDMI_PIXCLK_74M25, 0},
    {   4, DT_VIDSTD_720P59_94, 1280, 720,  370, 30, 40, 110,  220, 5, 5, 20, -1,-1,-1, DT_HDMI_PIXCLK_74M25, DT_HDMI_FLAGS_FRACMODE},
    {   4, DT_VIDSTD_720P60,    1280, 720,  370, 30, 40, 110,  220, 5, 5, 20, -1,-1,-1, DT_HDMI_PIXCLK_74M25, 0},

    // HD 1080 formats
    {  20, DT_VIDSTD_1080I50,      1920, 1080/2, 720, 22, 44,  528, 148, 5, 2, 15, 5, 2, 16, DT_HDMI_PIXCLK_74M25, DT_HDMI_FLAGS_INTERLACED},
    {   5, DT_VIDSTD_1080I59_94,   1920, 1080/2, 280, 22, 44,  88,  148, 5, 2, 15, 5, 2, 16, DT_HDMI_PIXCLK_74M25, DT_HDMI_FLAGS_INTERLACED | DT_HDMI_FLAGS_FRACMODE},
    {   5, DT_VIDSTD_1080I60,      1920, 1080/2, 280, 22, 44,  88,  148, 5, 2, 15, 5, 2, 16, DT_HDMI_PIXCLK_74M25, DT_HDMI_FLAGS_INTERLACED},
    {  32, DT_VIDSTD_1080P23_98,   1920, 1080,   830, 45, 44,  638, 148, 5, 4, 36, -1,-1,-1, DT_HDMI_PIXCLK_74M25, DT_HDMI_FLAGS_FRACMODE},
    {  32, DT_VIDSTD_1080P24,      1920, 1080,   830, 45, 44,  638, 148, 5, 4, 36, -1,-1,-1, DT_HDMI_PIXCLK_74M25, 0},
    {  33, DT_VIDSTD_1080P25,      1920, 1080,   720, 45, 44,  528, 148, 5, 4, 36, -1,-1,-1, DT_HDMI_PIXCLK_74M25, 0},
    {  34, DT_VIDSTD_1080P29_97,   1920, 1080,   280, 45, 44,  88,  148, 5, 4, 36, -1,-1,-1, DT_HDMI_PIXCLK_74M25, DT_HDMI_FLAGS_FRACMODE},
    {  34, DT_VIDSTD_1080P30,      1920, 1080,   280, 45, 44,  88,  148, 5, 4, 36, -1,-1,-1, DT_HDMI_PIXCLK_74M25, 0},
    //{  32, DT_VIDSTD_1080PSF23_98, 1920, 1080,   830, 45, 44,  638, 148, 5, 4, 36, -1,-1,-1, DT_HDMI_PIXCLK_74M25, DT_HDMI_FLAGS_FRACMODE},
    //{  32, DT_VIDSTD_1080PSF24,    1920, 1080,   830, 45, 44,  638, 148, 5, 4, 36, -1,-1,-1, DT_HDMI_PIXCLK_74M25, 0},
    {  20, DT_VIDSTD_1080PSF25,    1920, 1080/2, 720, 22, 44,  528, 148, 5, 2, 15, 5, 2, 16, DT_HDMI_PIXCLK_74M25, DT_HDMI_FLAGS_INTERLACED},
    {   5, DT_VIDSTD_1080PSF29_97, 1920, 1080/2, 280, 22, 44,  88,  148, 5, 2, 15, 5, 2, 16, DT_HDMI_PIXCLK_74M25, DT_HDMI_FLAGS_INTERLACED | DT_HDMI_FLAGS_FRACMODE},
    {   5, DT_VIDSTD_1080PSF30,    1920, 1080/2, 280, 22, 44,  88,  148, 5, 2, 15, 5, 2, 16, DT_HDMI_PIXCLK_74M25, DT_HDMI_FLAGS_INTERLACED},
    

    // 3G 1080 formats (LEVEL A)
    {  31, DT_VIDSTD_1080P50,      1920, 1080,   720, 45, 44,  528, 148, 5, 4, 36, -1,-1,-1, DT_HDMI_PIXCLK_148M5, 0},
    {  16, DT_VIDSTD_1080P59_94,   1920, 1080,   280, 45, 44,  88,  148, 5, 4, 36, -1,-1,-1, DT_HDMI_PIXCLK_148M5, DT_HDMI_FLAGS_FRACMODE},
    {  16, DT_VIDSTD_1080P60,      1920, 1080,   280, 45, 44,  88,  148, 5, 4, 36, -1,-1,-1, DT_HDMI_PIXCLK_148M5, 0},
    // 3G 1080 formats (LEVEL B)
    {  31, DT_VIDSTD_1080P50B,     1920, 1080,   720, 45, 44,  528, 148, 5, 4, 36, -1,-1,-1, DT_HDMI_PIXCLK_148M5, 0},
    {  16, DT_VIDSTD_1080P59_94B,  1920, 1080,   280, 45, 44,  88,  148, 5, 4, 36, -1,-1,-1, DT_HDMI_PIXCLK_148M5, DT_HDMI_FLAGS_FRACMODE},
    {  16, DT_VIDSTD_1080P60B,     1920, 1080,   280, 45, 44,  88,  148, 5, 4, 36, -1,-1,-1, DT_HDMI_PIXCLK_148M5, 0},

    // 6G 2160 formats
    {  93, DT_VIDSTD_2160P23_98,   3840, 2160,  1660, 90, 88,  1276, 296, 10, 8, 72, -1,-1,-1, DT_HDMI_PIXCLK_297M, DT_HDMI_FLAGS_FRACMODE | DT_HDMI_FLAGS_HDMI_VIC},
    {  93, DT_VIDSTD_2160P24,      3840, 2160,  1660, 90, 88,  1276, 296, 10, 8, 72, -1,-1,-1, DT_HDMI_PIXCLK_297M,  DT_HDMI_FLAGS_HDMI_VIC},
    {  94, DT_VIDSTD_2160P25,      3840, 2160,  1440, 90, 88,  1056, 296, 10, 8, 72, -1,-1,-1, DT_HDMI_PIXCLK_297M,  DT_HDMI_FLAGS_HDMI_VIC},
    {  95, DT_VIDSTD_2160P29_97,   3840, 2160,   560, 90, 88,  176,  296, 10, 8, 72, -1,-1,-1, DT_HDMI_PIXCLK_297M, DT_HDMI_FLAGS_FRACMODE | DT_HDMI_FLAGS_HDMI_VIC},
    {  95, DT_VIDSTD_2160P30,      3840, 2160,   560, 90, 88,  176,  296, 10, 8, 72, -1,-1,-1, DT_HDMI_PIXCLK_297M, DT_HDMI_FLAGS_HDMI_VIC},

    // 12G 2160 formats
    {  96, DT_VIDSTD_2160P50,      3840, 2160,  1440, 90, 88,  1056, 296, 10, 8, 72, -1,-1,-1, DT_HDMI_PIXCLK_594M, DT_HDMI_FLAGS_CLKRATIO_4K},
    {  97, DT_VIDSTD_2160P59_94,   3840, 2160,   560, 90, 88,  176,  296, 10, 8, 72, -1,-1,-1, DT_HDMI_PIXCLK_594M, DT_HDMI_FLAGS_FRACMODE | DT_HDMI_FLAGS_CLKRATIO_4K},
    {  97, DT_VIDSTD_2160P60,      3840, 2160,   560, 90, 88,  176,  296, 10, 8, 72, -1,-1,-1, DT_HDMI_PIXCLK_594M, DT_HDMI_FLAGS_CLKRATIO_4K},
};
static const  Int  DT_HDMI_VIDSTD_2_FORMAT_NUM_ENTRIES = sizeof(DT_HDMI_VIDSTD_2_FORMAT) / sizeof(DT_HDMI_VIDSTD_2_FORMAT[0]);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxGetIndexVidStd3g -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtHdmiTxGetIndexVidStd3g(DtaHdmiVidStd VidStd, DtaHdmiVidStd* pVidStd3g)
{
    DtaHdmiVidStd VidStd3g;
    VidStd3g.m_VidStd = DT_VIDSTD_UNKNOWN;
    VidStd3g.m_AspectRatio = VidStd.m_AspectRatio;
    if (pVidStd3g != NULL)
    { 
        pVidStd3g->m_VidStd  = DT_VIDSTD_UNKNOWN;
        pVidStd3g->m_AspectRatio = VidStd.m_AspectRatio;
    }

    // Convert 6G/12G to 3G video standard format
    switch (VidStd.m_VidStd)
    {
    case DT_VIDSTD_2160P60:     VidStd3g.m_VidStd = DT_VIDSTD_1080P60; break;
    case DT_VIDSTD_2160P59_94:  VidStd3g.m_VidStd = DT_VIDSTD_1080P59_94; break;
    case DT_VIDSTD_2160P50:     VidStd3g.m_VidStd = DT_VIDSTD_1080P50; break;
    case DT_VIDSTD_2160P30:     VidStd3g.m_VidStd = DT_VIDSTD_1080P30; break;
    case DT_VIDSTD_2160P29_97:  VidStd3g.m_VidStd = DT_VIDSTD_1080P29_97; break;
    case DT_VIDSTD_2160P25:     VidStd3g.m_VidStd = DT_VIDSTD_1080P25; break;
    case DT_VIDSTD_2160P24:     VidStd3g.m_VidStd = DT_VIDSTD_1080P24; break;
    case DT_VIDSTD_2160P23_98:  VidStd3g.m_VidStd = DT_VIDSTD_1080P23_98; break;
    default: return -1;
    }
    // Get index of 3G video standard format
    if (pVidStd3g != NULL)
        *pVidStd3g = VidStd3g;
    return DtHdmiTxGetIndexVidStd(VidStd3g);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxGetIndexVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtHdmiTxGetIndexVidStd(DtaHdmiVidStd VidStd)
{
    Int  i, AspectRatio;
    for (i=0; i<DT_HDMI_VIDSTD_2_FORMAT_NUM_ENTRIES; i++)
    {
        AspectRatio = ((DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_FLAGS]
                        &DT_HDMI_FLAGS_ASPECT_RATIO_4_3) != 0) ? DTA_AR_4_3 : DTA_AR_16_9;
        if (DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_VIDSTD]==VidStd.m_VidStd
                                                     && AspectRatio==VidStd.m_AspectRatio)
            return i;
    }
    return -1;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxFillAviInfoFrame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtHdmiTxFillAviInfoFrame(DtaHdmiTx* pHdmiTx, DtaHdmiVidStd VidStd, AVI_INFOFRAME* pInfoFrame)
{
    Int  Index = DtHdmiTxGetIndexVidStd(VidStd);
    Int  i;
    Int  Checksum;
    UInt8*  pData;
    DtMemZero(pInfoFrame, sizeof(AVI_INFOFRAME));

    DT_ASSERT(Index != -1);

    if (Index != -1)
    {
        pInfoFrame->m_VIC = 0;
        if ((DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_FLAGS]&DT_HDMI_FLAGS_HDMI_VIC) == 0)
            pInfoFrame->m_VIC = DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_VIC];

        if ((DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_FLAGS]&DT_HDMI_FLAGS_ASPECT_RATIO_4_3) == 0)
            pInfoFrame->m_PictureAspectRatio = 2; // = 16:9
        else
            pInfoFrame->m_PictureAspectRatio = 1; // = 4:3
    }
    
    switch (pHdmiTx->m_UsedVidMode)
    { 
    case DT_HDMI_VIDMOD_RGB_444: pInfoFrame->m_YCbCrIndicator = 0; break;
    case DT_HDMI_VIDMOD_YCBCR_422: pInfoFrame->m_YCbCrIndicator = 1; break;
    case DT_HDMI_VIDMOD_YCBCR_444: pInfoFrame->m_YCbCrIndicator = 2; break;
    default: 
        {   DtDbgOut(ERR, HDMI, "Invalid m_UsedVidMode: %xh", pHdmiTx->m_UsedVidMode);
            DT_ASSERT(FALSE);
        }
    }

    pInfoFrame->m_PictureRepetitionFactor = 
        ((DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_FLAGS]&DT_HDMI_FLAGS_PIXELREP)== 0?0:1);

    // Calculate Checksum (include the three header bytes)
    Checksum = 0x82 + 0x2 + 0xd; // PacketType + Version + Length
    pData = (UInt8*)&pInfoFrame->m_Data[0];
    for (i=0; i<sizeof(AVI_INFOFRAME); i++)
        Checksum += pData[i];
    pInfoFrame->m_Checksum = (UInt8)((UInt)256 - Checksum);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiHdmiVic2VidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtaHdmiVidStd  DtHdmiHdmiVic2VidStd(UInt8 HdmiVic)
{
    DtaHdmiVidStd VidStd;
    VidStd.m_VidStd = DT_VIDSTD_UNKNOWN;
    VidStd.m_AspectRatio = DTA_AR_16_9;
    switch (HdmiVic)
    {
    case 0x01: VidStd.m_VidStd = DT_VIDSTD_2160P30; break;  // 4kx2k 30Hz
    case 0x02: VidStd.m_VidStd = DT_VIDSTD_2160P25; break;  // 4kx2k 25Hz
    case 0x03: VidStd.m_VidStd = DT_VIDSTD_2160P24; break;  // 4kx2k 24Hz
    case 0x04:    // Not supported by Dektec: 4kx2k 24Hz (SMPTE)
    default:   VidStd.m_AspectRatio = DTA_AR_UNKNOWN; break;
    }
    return VidStd;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiVidStd2HdmiVic -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt8  DtHdmiVidStd2HdmiVic(DtaHdmiVidStd VidStd)
{
    switch(VidStd.m_VidStd)
    {
    case DT_VIDSTD_2160P29_97:
    case DT_VIDSTD_2160P30:
        return 0x01;
    case DT_VIDSTD_2160P25:
        return 0x02;
    case DT_VIDSTD_2160P23_98: 
    case DT_VIDSTD_2160P24:
        return 0x03;
    }
    return 0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxFillHdmiVsInfoFrame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtHdmiTxFillHdmiVsInfoFrame(DtaHdmiTx* pHdmiTx, DtaHdmiVidStd VidStd, 
                                                             HDMIVS_INFOFRAME* pInfoFrame)
{
    Int  Index = DtHdmiTxGetIndexVidStd(VidStd);
    Int  i;
    Int  Checksum;
    UInt8  HdmiVic = 0;
    UInt8*  pData;
    UInt64  Data;
    DtMemZero(pInfoFrame, sizeof(HDMIVS_INFOFRAME));

    pInfoFrame->m_Length = 5;
    pInfoFrame->m_IEEE = HDMI_IEEE_LLC_IDENTIFIER;
    
    DT_ASSERT(Index != -1);

    HdmiVic = DtHdmiVidStd2HdmiVic(VidStd);
    if (HdmiVic == 0)
    {
        DT_ASSERT(Index==-1 || 
              (DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_FLAGS]&DT_HDMI_FLAGS_HDMI_VIC)==0);
        pInfoFrame->m_HdmiVideoFormat = 0;  // HDMI VIC not present
    }
    else
        pInfoFrame->m_HdmiVideoFormat = 1;  // HDMI VIC present
    pInfoFrame->m_HdmiVIC = HdmiVic;

    // Calculate Checksum (include the three header bytes)
    Checksum = 0x81 + 0x1 + (Int)pInfoFrame->m_Length; // PacketType + Version + Length
    Data = pInfoFrame->m_Data64;
    Data = Data >> 5;
    pData = (UInt8*)&Data;
    for (i=0; i<7; i++)
        Checksum += pData[i];
    
    pInfoFrame->m_Checksum = (UInt8)((UInt)256 - Checksum);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxFillAudioInfoFrame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtHdmiTxFillAudioInfoFrame(DtaHdmiTx* pHdmiTx, DtaHdmiVidStd VidStd, Int AudStd, 
                                                                AUD_INFOFRAME* pInfoFrame)
{
    Int  Index = DtHdmiTxGetIndexVidStd(VidStd);
    Int  i;
    Int  Checksum;
    UInt8*  pData;
    DtMemZero(pInfoFrame, sizeof(AUD_INFOFRAME));

    DT_ASSERT(Index != -1);

    pInfoFrame->m_CC = 0; // Check stream header
    pInfoFrame->m_CT = 0; // Check stream header
    pInfoFrame->m_SS = 0; // Check stream header
    pInfoFrame->m_SF = 0; // Check stream header
    pInfoFrame->m_CA = 0; // Not used
    pInfoFrame->m_LSV = 0; // 0dB Level Shift
    pInfoFrame->m_DM_INH = 0;
    pInfoFrame->m_LFEPBL = 0; // No information
    
    
    // Calculate Checksum (include the three header bytes)
    Checksum = 0x84 + 0x1 + 0xa; // PacketType + Version + Length
    pData = (UInt8*)&pInfoFrame->m_Data[0];
    for (i=0; i<sizeof(AUD_INFOFRAME); i++)
        Checksum += pData[i];
    pInfoFrame->m_Checksum = (UInt8)((UInt)256 - Checksum);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiScdcI2cWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Pre: I2C lock is acquired
//
DtStatus  DtHdmiScdcI2cWrite(DtaHdmiTx* pHdmiTx, UInt8 Offset, UInt8 Data)
{
    DtaDeviceData*  pDvcData = pHdmiTx->m_pDvcData;
    Int PortIdx = pHdmiTx->m_PortIndex;
    DtStatus  Status;
    UInt8  I2cData[2];
    
    I2cData[0] = Offset;
    I2cData[1] = Data;

    Status = DtaI2cWrite(pDvcData, PortIdx, DT_INVALID_FILE_OBJECT_PTR, 
                                                  SCDC_I2C_ADDRRESS, 2, (UInt8*)&I2cData);
    if (!DT_SUCCESS(Status))
        DtDbgOut(ERR, HDMI, "Error writing data: %02xh to offset %02xh", Data, Offset);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiScdcI2cRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Pre: I2C lock is acquired
//
DtStatus  DtHdmiScdcI2cRead(DtaHdmiTx* pHdmiTx, UInt8 Offset, UInt8* pData)
{
    DtaDeviceData*  pDvcData = pHdmiTx->m_pDvcData;
    Int PortIdx = pHdmiTx->m_PortIndex;
    DtStatus  Status;
    
    Status = DtaI2cWriteRead(pDvcData, PortIdx, DT_INVALID_FILE_OBJECT_PTR, 
                                                    SCDC_I2C_ADDRRESS, 1, (UInt8*)&Offset,
                                                    SCDC_I2C_ADDRRESS, 1, pData);
    if (!DT_SUCCESS(Status))
        DtDbgOut(ERR, HDMI, "Error reading at offset %02xh", Offset);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiScdcGetScramblerStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtHdmiScdcGetScramblerStatus(DtaHdmiTx* pHdmiTx, Bool* pScramblerEn)
{
    DtaDeviceData*  pDvcData = pHdmiTx->m_pDvcData;
    DtStatus  Status;
    Int PortIdx = pHdmiTx->m_PortIndex;
    UInt8  Data;
    UInt8  Config;
    UInt8  ClkDetect;
    UInt8  Error[7];
    Data = 0;
    // Lock I2c
    Status = DtaI2cLock(pDvcData, PortIdx, DT_INVALID_FILE_OBJECT_PTR, 5000);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtHdmiScdcI2cRead(pHdmiTx, SCDC_TMDS_CONFIG_OFFSET, &Config);
    Status = DtHdmiScdcI2cRead(pHdmiTx, 0x40, &ClkDetect);
    Status = DtHdmiScdcI2cRead(pHdmiTx, 0x50, &Error[0]);
    Status = DtHdmiScdcI2cRead(pHdmiTx, 0x51, &Error[1]);
    Status = DtHdmiScdcI2cRead(pHdmiTx, 0x52, &Error[2]);
    Status = DtHdmiScdcI2cRead(pHdmiTx, 0x53, &Error[3]);
    Status = DtHdmiScdcI2cRead(pHdmiTx, 0x54, &Error[4]);
    Status = DtHdmiScdcI2cRead(pHdmiTx, 0x55, &Error[5]);
    Status = DtHdmiScdcI2cRead(pHdmiTx, 0x56, &Error[6]);
    Status = DtHdmiScdcI2cRead(pHdmiTx, SCDC_SCRAMBLER_STATUS_OFFSET, &Data);
    *pScramblerEn = (Data & 1) != 0;
    DtDbgOut(ERR, HDMI, "SCDC config 0x20: %x status 0x21: %x ClkDetect 0x40: %x", Config, Data, ClkDetect);
    DtDbgOut(ERR, HDMI, "ErrorCounters: 0x50..0x56 %x %x %x %x %x %x %x", Error[0], Error[1], Error[2], Error[3], Error[4], Error[5], Error[6]);
    DtaI2cUnlock(pDvcData, PortIdx, DT_INVALID_FILE_OBJECT_PTR, FALSE);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiScdcSetTmdsConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtHdmiScdcSetTmdsConfig(DtaHdmiTx* pHdmiTx, Int HdmiFlags)
{
    DtaDeviceData*  pDvcData = pHdmiTx->m_pDvcData;
    DtStatus  Status;
    Int PortIdx = pHdmiTx->m_PortIndex;
    UInt8  Data = 0;

    // Lock I2c
    Status = DtaI2cLock(pDvcData, PortIdx, DT_INVALID_FILE_OBJECT_PTR, 5000);
    if (!DT_SUCCESS(Status))
        return Status;

    if (HdmiFlags & DT_HDMI_FLAGS_CLKRATIO_4K)
        Data = (SCDC_TMDS_CONFIG_SCRAMBLING_ENABLE | SCDC_TMDS_CONFIG_TMDS_CLK_RATIO);
    
    Status = DtHdmiScdcI2cWrite(pHdmiTx, SCDC_TMDS_CONFIG_OFFSET, Data);

    DtaI2cUnlock(pDvcData, PortIdx, DT_INVALID_FILE_OBJECT_PTR, FALSE);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiInitScdc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtHdmiScdcInit(DtaHdmiTx* pHdmiTx)
{
    DtaDeviceData*  pDvcData = pHdmiTx->m_pDvcData;
    Int PortIdx = pHdmiTx->m_PortIndex;
    DtStatus  Status = DT_STATUS_OK;
    
    if (!pHdmiTx->m_SupportScDc)
        return DT_STATUS_OK;
    
    // Lock I2c
    Status = DtaI2cLock(pDvcData, PortIdx, DT_INVALID_FILE_OBJECT_PTR, 5000);
    if (!DT_SUCCESS(Status))
        return Status;

    // Write version number 1
    Status = DtHdmiScdcI2cWrite(pHdmiTx, SCDC_SOURCE_VERSION_OFFSET, 1);

    // Disable RR_Enable (Bit 0)
    if (DT_SUCCESS(Status))
        Status = DtHdmiScdcI2cWrite(pHdmiTx, SCDC_CONFIG_0_OFFSET, 0);

    // Clear TMDS config
    if (DT_SUCCESS(Status))
        Status = DtHdmiScdcI2cWrite(pHdmiTx, SCDC_TMDS_CONFIG_OFFSET, 0);
    
    DtaI2cUnlock(pDvcData, PortIdx, DT_INVALID_FILE_OBJECT_PTR, FALSE);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiResetFeatures -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtHdmiResetFeatures(DtaHdmiTx* pHdmiTx)
{
    pHdmiTx->m_MonSupportedFormats = 0;
    pHdmiTx->m_MonSupportedAudio = 0;
    pHdmiTx->m_SupportHdmi = FALSE;
    pHdmiTx->m_SupportYCbCr444 = FALSE;
    pHdmiTx->m_SupportYCbCr422 = FALSE;
    pHdmiTx->m_SupportBasicAudio = FALSE;
    pHdmiTx->m_SupportScDc = FALSE;
    pHdmiTx->m_SupportMonitorRangeLimits = FALSE;
    pHdmiTx->m_MaxPixelClk = 0;
    pHdmiTx->m_MinVRate = 0;
    pHdmiTx->m_MaxVRate = 0;
    pHdmiTx->m_MinHRate = 0;
    pHdmiTx->m_MaxHRate = 0;
    pHdmiTx->m_EdidError = FALSE;
}

//-.-.-.-.-.-.-.-.- DtHdmiGetSupportedFormatsFromDetailedTimeDescriptor -.-.-.-.-.-.-.-.-.
//
UInt64  DtHdmiGetSupportedFormatsFromDetailedTimeDescriptor(HDMI_EDID_DTD* pDtd,
                                                                              Bool Field2)
{
    UInt64  Supported = 0;
    UInt64  Mask = 1;
    Int  i;
    Int  PixelClk = -1;
    Bool  Found;
    DT_ASSERT(DT_HDMI_VIDSTD_2_FORMAT_NUM_ENTRIES <= 64);
    for (i=0; i<DT_HDMI_VIDSTD_2_FORMAT_NUM_ENTRIES; i++)
    {
        Found = TRUE;
        switch (pDtd->m_PixelClock)
        {
        case 13500000: PixelClk = DT_HDMI_PIXCLK_13M5; break;
        case 27000000: PixelClk = DT_HDMI_PIXCLK_27M; break;
        case 74250000: PixelClk = DT_HDMI_PIXCLK_74M25; break;
        case 148500000: PixelClk = DT_HDMI_PIXCLK_148M5; break;
        case 297000000: PixelClk = DT_HDMI_PIXCLK_297M; break;
        case 594000000: PixelClk = DT_HDMI_PIXCLK_594M; break;
        default: PixelClk = -1;
        }
        if (PixelClk != DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_PIXEL_CLK])
        {
            DtDbgOut(MAX, HDMI, "[%i] PixelClk not supported (%i %i)", i,PixelClk, 
                                          DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_PIXEL_CLK]);
            Found = FALSE;

        }
        else if (pDtd->m_HActivePixels != DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_H_ACT])
        {
            DtDbgOut(MAX, HDMI, "[%i] HActive not supported (%i %i)", i, 
                       pDtd->m_HActivePixels, DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_H_ACT]);
            Found = FALSE;
        }
        else if (pDtd->m_HBlankingPixels != DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_H_BLANC])
        {
            DtDbgOut(MAX, HDMI, "[%i] HBlanking not supported (%i %i)", i,
                   pDtd->m_HBlankingPixels, DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_H_BLANC]);
            Found = FALSE;
        }
        else if (pDtd->m_VActiveLines != DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_V_ACT])
        {
            DtDbgOut(MAX, HDMI, "[%i] VActive not supported (%i %i)", i, 
                        pDtd->m_VActiveLines, DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_V_ACT]);
            Found = FALSE;
        }
        else if (pDtd->m_VBlankingLines != DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_V_BLANC])
        {
            DtDbgOut(MAX, HDMI, "[%i] VBlanking not supported (%i %i)", i, 
                    pDtd->m_VBlankingLines, DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_V_BLANC]);
            Found = FALSE;
        }
        else if (pDtd->m_HFrontPorchPixels != 
                                            DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_H_FPORCH])
        {
            DtDbgOut(MAX, HDMI, "[%i] HFrontPorch not supported (%i %i)", i,
                pDtd->m_HFrontPorchPixels, DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_H_FPORCH]);
            Found = FALSE;
        }
        else if (pDtd->m_HSyncPulseWidth != DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_H_SYNC])
        {
            DtDbgOut(MAX, HDMI, "[%i] HSyncPulseWidth not supported (%i %i)", i,
                    pDtd->m_HSyncPulseWidth, DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_H_SYNC]);
            Found = FALSE;
        }
        else if ((pDtd->m_Flags&DT_HDMI_FLAGS_HSPOL_NEG) != 
                     (DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_FLAGS]&DT_HDMI_FLAGS_HSPOL_NEG))
        {
            DtDbgOut(MAX, HDMI, "[%i] HSPOL NEG not supported (%i %i)", i, 
                    (pDtd->m_Flags&DT_HDMI_FLAGS_HSPOL_NEG), 
                    (DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_FLAGS]&DT_HDMI_FLAGS_HSPOL_NEG));
            Found = FALSE;
        }
        else if ((pDtd->m_Flags&DT_HDMI_FLAGS_VSPOL_NEG) != 
                     (DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_FLAGS]&DT_HDMI_FLAGS_VSPOL_NEG))
        {
            DtDbgOut(MAX, HDMI, "[%i] VSPOL NEG not supported (%i %i)", i);
            Found = FALSE;
        }
        else if ((pDtd->m_Flags&DT_HDMI_FLAGS_INTERLACED) != 
                    (DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_FLAGS]&DT_HDMI_FLAGS_INTERLACED))
        {
            DtDbgOut(MAX, HDMI, "[%i] INTERLACED not supported (%i %i)", i, 
                    (pDtd->m_Flags&DT_HDMI_FLAGS_VSPOL_NEG), 
                    (DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_FLAGS]&DT_HDMI_FLAGS_VSPOL_NEG));
            Found = FALSE;
        }
        else if (Field2)
        {
            // Field 2
            if (pDtd->m_VFrontPorchLines != 
                                         DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_F2_V_FPORCH])
            {
                DtDbgOut(MAX, HDMI, "[%i] VFrontPorchLines F2 not supported (%i %i)", i,
                                        pDtd->m_VFrontPorchLines, 
                                        DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_F2_V_FPORCH]);
                Found = FALSE;
            }
            else if (pDtd->m_VSyncPulseWidth != 
                                           DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_F2_V_SYNC])
            {
                DtDbgOut(MAX, HDMI, "[%i] VSyncPuldeWidth F2 not supported (%i %i)", i,
                                          pDtd->m_VSyncPulseWidth, 
                                          DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_F2_V_SYNC]);
                Found = FALSE;
            }
        } else
        {
            // Field 1
            if (pDtd->m_VFrontPorchLines != 
                                         DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_F1_V_FPORCH])
            {
                DtDbgOut(MAX, HDMI, "[%i] VFrontPorchLines F1 not supported (%i %i)", i,
                                        pDtd->m_VFrontPorchLines,
                                        DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_F1_V_FPORCH]);
                Found = FALSE;
            }
            else if (pDtd->m_VSyncPulseWidth != 
                                           DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_F1_V_SYNC])
            {
                DtDbgOut(MAX, HDMI, "[%i] VSyncPulseWidth F1 not supported (%i %i)", i,
                                          pDtd->m_VSyncPulseWidth, 
                                          DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_F1_V_SYNC]);
                Found = FALSE;
            }
        }

        if (Found)
            Supported |= Mask;
        Mask = Mask << 1;
    }
    return Supported;
}

//.-.-.-.-.-.-.-.-.- DtHdmiGetSupportedFormatsFromShortVideoDescriptor -.-.-.-.-.-.-.-.-.-
//
UInt64  DtHdmiGetSupportedFormatsFromShortVideoDescriptor(UInt8  ShortVideoDescriptor)
{
    UInt64  Supported = 0;
    UInt64  Mask = 1;
    Int  i;
    DT_ASSERT(DT_HDMI_VIDSTD_2_FORMAT_NUM_ENTRIES <= 64);
    for (i=0; i<DT_HDMI_VIDSTD_2_FORMAT_NUM_ENTRIES; i++)
    {
        if (DT_HDMI_VIDSTD_2_FORMAT[i][HDMI_IDX_VIC] == (ShortVideoDescriptor & 0x7f))
        {
            Supported |= Mask;
        }
        Mask = Mask <<1;
    }
    return Supported;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiReadI2cBlock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtHdmiReadI2cBlock(DtaHdmiTx* pHdmiTx, Int BlockNo, UInt8* pData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaDeviceData*  pDvcData = pHdmiTx->m_pDvcData;
    Int  PortIdx = pHdmiTx->m_PortIndex;
    volatile UInt8* pI2cRegs = pDvcData->m_pNonIpPorts[PortIdx].m_I2c.m_pI2cRegs;
    Int  Segment;
    Int  i;
    Int  Offset;
    UInt8  I2cWriteData[2];

    // Lock I2c
    Status = DtaI2cLock(pDvcData, PortIdx, DT_INVALID_FILE_OBJECT_PTR, 5000);
    if (!DT_SUCCESS(Status))
        return Status;
    
    Segment = BlockNo/2;
    Offset = (BlockNo %2) * 128;

    // The firmware will to the I2C segment transfer to the EDDC_I2C_ADDRESS
    // because the Eddc control bit is enabled. We need to write the
    // segment and offset when using the DDC_I2C_ADDRESS address (and eddc is enabled)
    I2cWriteData[0] = (UInt8)Segment;
    I2cWriteData[1] = (UInt8)Offset;
    Status = DtaI2cWriteRead(pDvcData, PortIdx, DT_INVALID_FILE_OBJECT_PTR, 
                                                DDC_I2C_ADDRESS, 2, (UInt8*)&I2cWriteData,
                                                DDC_I2C_ADDRESS, 128, pData);
    // Check the EDDC NACK status bit
    if (DtaRegI2cStatusGetEddcNack(pI2cRegs) != 0)
    {
        // Firmware got a NACK writing segment pointer. 
        // This is not a problem if the segment pointer is 0.
        // (Not all HDMI targets supports the EDDC functionality.)
        DtDbgOut(AVG, HDMI, "NACK received writing segment pointer.");

        if (Segment != 0)
        {
            DtaI2cUnlock(pDvcData, PortIdx, DT_INVALID_FILE_OBJECT_PTR, FALSE);
            
            DtDbgOut(AVG, HDMI, "Monitor should support EDDC because segment"
                                                                         " pointer != 0");
            return DT_STATUS_CANCELLED;
        }
    }

    for (i=0; i<128/16;i++)
    {
        DtDbgOut(ERR, HDMI, "[%02x] %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x", i,
            pData[i*16+0], pData[i*16+1], pData[i*16+2], pData[i*16+3], pData[i*16+4], pData[i*16+5], pData[i*16+6], pData[i*16+7], pData[i*16+8], pData[i*16+9], 
            pData[i*16+10], pData[i*16+11], pData[i*16+12], pData[i*16+13], pData[i*16+14], pData[i*16+15]);
    }
    DtaI2cUnlock(pDvcData, PortIdx, DT_INVALID_FILE_OBJECT_PTR, FALSE);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiReadEdid -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtHdmiReadEdid(DtaHdmiTx* pHdmiTx)
{
    UInt8  Data[128];
    DtStatus  Status;
    Int  NumberOfExtensions = 0;
    Int  i;
    DtDbgOut(MAX, HDMI, "Start");
        
    Status = DtHdmiReadI2cBlock(pHdmiTx, 0, Data);
    if (!DT_SUCCESS(Status))
    {
        pHdmiTx->m_EdidError = TRUE;
        DtDbgOut(ERR, HDMI, "Error reading EEDID block 0");
        return DT_STATUS_OK;
    }

    NumberOfExtensions = DtHdmiEdidParseEedid(pHdmiTx, Data);
    if (NumberOfExtensions > 0)
    {
        for (i=0; i<NumberOfExtensions; i++)
        {
            Status = DtHdmiReadI2cBlock(pHdmiTx, i+1, Data);
            if (DT_SUCCESS(Status))
                DtHdmiEdidParseExtension(pHdmiTx, Data);
            else {
                pHdmiTx->m_EdidError = TRUE;
                DtDbgOut(ERR, HDMI, "Error reading EEDID block %i", i);
            }
        }
    }
    DtDbgOut(MAX, HDMI, "Exit");
    
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxHotplugWorkItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtHdmiTxHotplugWorkItem(DtWorkItemArgs* pArgs)
{
    DtaHdmiTx*  pHdmiTx = (DtaHdmiTx*)pArgs->m_pContext;
    DtStatus  Status = DT_STATUS_OK;
    DtaHdmiVidStd VidStdUnknown = {DT_VIDSTD_UNKNOWN, DTA_AR_UNKNOWN};
    pHdmiTx->m_MonDetected = DtaRegHdmiTxGenStatGetSinkStat(pHdmiTx->m_pHdmiRegs) != 0;

    if (pHdmiTx->m_ForceMonitorDetected)
    {
        DtMutexAcquire(&pHdmiTx->m_StateLock, -1);
        DtDbgOut(ERR, HDMI, "FORCE Monitor connected");
      
        DtHdmiTxEnableHdmiOutputDriver(pHdmiTx, TRUE);
        
        Status = DtHdmiTxUpdateVideoStd(pHdmiTx, VidStdUnknown, TRUE);
        if (!DT_SUCCESS(Status))
            DtDbgOut(ERR, HDMI, "Error updating video standard");
        DtMutexRelease(&pHdmiTx->m_StateLock);
        return DT_STATUS_OK;
    }
    
    DtMutexAcquire(&pHdmiTx->m_StateLock, -1);
    if (pHdmiTx->m_MonDetected)
    {
        DtDbgOut(ERR, HDMI, "HW power up");
        DtHdmiTxEnableHdmiOutputDriver(pHdmiTx, TRUE);

        // Only read EDDID in case hotplug interrupt occured
        if (pArgs->m_Data1.m_UInt32_1 == 0)
        {
            DtDbgOut(AVG, HDMI, "Monitor connected");
            Status = DtHdmiReadEdid(pHdmiTx);

            if (DT_SUCCESS(Status))
            {
                Status = DtHdmiScdcInit(pHdmiTx);
                if (!DT_SUCCESS(Status))
                    DtDbgOut(ERR, HDMI, "Error initializing SCDC");
            }
        }
        if (DT_SUCCESS(Status))
        {
            Status = DtHdmiTxUpdateVideoStd(pHdmiTx, VidStdUnknown, TRUE);
            if (!DT_SUCCESS(Status))
                DtDbgOut(ERR, HDMI, "Error updating video standard");
        }
    } else {
        if (!pHdmiTx->m_ForceMonitorDetected)
        {
            DtDbgOut(AVG, HDMI, "Monitor disconnected");
        
            // Disable HDMI
            DtaRegHdmiTxGenCtrlSetEnable(pHdmiTx->m_pHdmiRegs, 0);
            DtHdmiTxEnableHdmiOutputDriver(pHdmiTx, FALSE);

        }
    }
    DtMutexRelease(&pHdmiTx->m_StateLock);
    return Status;
}
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxHotplugDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtHdmiTxHotplugDpc(DtDpcArgs* pArgs)
{   
    DtaHdmiTx*  pHdmiTx = (DtaHdmiTx*)pArgs->m_pContext;
    DtWorkItemArgs  DtWorkItemArgs;
    DtWorkItemArgs.m_pContext = pHdmiTx;
    DtWorkItemArgs.m_Data1.m_UInt32_1 = pArgs->m_Data1.m_UInt32_1;
    
   // pHdmiTx->m_MonDetected = DtaRegHdmiTxGenStatGetSinkStat(pHdmiTx->m_pHdmiRegs) != 0;
    DtWorkItemSchedule(&pHdmiTx->m_HotplugWorkItem, &DtWorkItemArgs);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxInitHdmiOutputDriver -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtHdmiTxInitHdmiOutputDriver(DtaHdmiTx* pHdmiTx)
{
    DtStatus Status = DT_STATUS_OK;
    UInt8  I2cData[2];
    if (pHdmiTx->m_pDvcData->m_DevInfo.m_HardwareRevision < 200)
        return DT_STATUS_OK;
    I2cData[0] = 0x9;
    I2cData[1] = 0x6;
    Status = DtaI2cWrite(pHdmiTx->m_pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR,
                                                 SN65DP159_I2C_ADDR, 2, (UInt8*)&I2cData);
    if (!DT_SUCCESS(Status))
    {
        // Error writing SN65DP159_I2C_ADDR
        DtDbgOut(ERR, HDMI, "Error initilizing SN65DP159 device. Address offset: 0x9");
        return Status;
    }
    DtDbgOut(ERR, HDMI, "Initialized SN65DP159");
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxProgramHdmiOutputDriver -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtHdmiTxProgramHdmiOutputDriver(DtaHdmiTx* pHdmiTx, DtaHdmiVidStd VidStd)
{
    DtStatus Status = DT_STATUS_OK;
    Int  Index = DtHdmiTxGetIndexVidStd(VidStd);
    UInt8  I2cData[2];
    UInt8  RegA;
    UInt8  RegB;// = 0x80;
    //UInt8  RegC;
    //UInt8  RegD;
    if (pHdmiTx->m_pDvcData->m_DevInfo.m_HardwareRevision < 200)
        return DT_STATUS_OK;
    switch (DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_PIXEL_CLK])
    {
    case DT_HDMI_PIXCLK_594M: // HDMI 2.0
        RegA = 0x36;    // Automatic retimer for HDMI 2.0
        RegB = 0x1A;
        //RegC = 0xA1;
        break;
    default: // HDMI 1.4 
        RegA = 0x35;    // Automatic redriver to retimer crossover at 1.0 Gbps
        RegB = 0x01;
        //RegC = 0xA0;    // Set VSWING data decrease by 24%
        break;
    }
    I2cData[0] = 0xA;
    I2cData[1] = RegA;
    Status = DtaI2cWrite(pHdmiTx->m_pDvcData,  -1, DT_INVALID_FILE_OBJECT_PTR, 
                                                 SN65DP159_I2C_ADDR, 2, (UInt8*)&I2cData);
    if (!DT_SUCCESS(Status))
    {
        // Error writing SN65DP159_I2C_ADDR
        DtDbgOut(ERR, HDMI, "Error initilizing SN65DP159 device. Address offset: 0xA");
        return Status;
    }
    I2cData[0] = 0xB;
    I2cData[1] = RegB;
    Status = DtaI2cWrite(pHdmiTx->m_pDvcData,  -1, DT_INVALID_FILE_OBJECT_PTR, 
                                                 SN65DP159_I2C_ADDR, 2, (UInt8*)&I2cData);
    if (!DT_SUCCESS(Status))
    {
        // Error writing SN65DP159_I2C_ADDR
        DtDbgOut(ERR, HDMI, "Error initilizing SN65DP159 device. Address offset: 0xB");
        return Status;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxUpdateAviInfoFrame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtHdmiTxUpdateAviInfoFrame(DtaHdmiTx* pHdmiTx, DtaHdmiVidStd VidStd)
{
    Int  i;
    AVI_INFOFRAME AviInfoFrame;

    if (!pHdmiTx->m_SupportHdmi)
    {
        if (pHdmiTx->m_DisableEdidCheck)
            DtDbgOut(ERR, HDMI, "AVI Info Frames not supported, but forced by user.");
        else
        {
            DtDbgOut(ERR, HDMI, "AVI Info Frames not supported. No HDMI sink device.");
            return;
        }
    }

    // Fill AviInfoFrame
    DtHdmiTxFillAviInfoFrame(pHdmiTx, VidStd, &AviInfoFrame);

    // Write AviInfoFrame to firmware
    for (i=0; i<sizeof(AviInfoFrame.m_Data)/4; i++)
        DtaRegHdmiTxAviInfoFrameSet(pHdmiTx->m_pHdmiRegs, i, AviInfoFrame.m_Data[i]);

    // Enable AviInfoFrame
    DtaRegHdmiTxInfoFrameCtrlSetAviInfoFrameEnable(pHdmiTx->m_pHdmiRegs, 1);
    DtaRegHdmiTxInfoFrameCtrlSetAviInfoFrameLatch(pHdmiTx->m_pHdmiRegs);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxUpdateHdmiVsInfoFrame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtHdmiTxUpdateHdmiVsInfoFrame(DtaHdmiTx* pHdmiTx, DtaHdmiVidStd VidStd)
{
    Int  i;
    HDMIVS_INFOFRAME  InfoFrame;
    if (!pHdmiTx->m_SupportHdmi)
    {
        if (pHdmiTx->m_DisableEdidCheck)
            DtDbgOut(ERR, HDMI, "HDMI VS Info Frames not supported, but forced by user.");
        else
        {
            DtDbgOut(ERR, HDMI, 
                               "HDMI VS Info Frames not supported. No HDMI sink device.");
            return;
        }
    }

    // Fill HDMI VS InfoFrame
    DtHdmiTxFillHdmiVsInfoFrame(pHdmiTx, VidStd, &InfoFrame);

    // Write HDMI VS InfoFrame to firmware
    for (i=0; i<sizeof(InfoFrame.m_Data)/4; i++)
        DtaRegHdmiTxVsInfoFrameSet(pHdmiTx->m_pHdmiRegs, i, InfoFrame.m_Data[i]);

    // Enable HDMI VS InfoFrame
    DtaRegHdmiTxInfoFrameCtrlSetVsInfoFrameEnable(pHdmiTx->m_pHdmiRegs, 1);
    DtaRegHdmiTxInfoFrameCtrlSetVsInfoFrameLatch(pHdmiTx->m_pHdmiRegs);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxUpdateAudioInfoFrame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtHdmiTxUpdateAudioInfoFrame(DtaHdmiTx* pHdmiTx, DtaHdmiVidStd VidStd, Int AudioStd)
{
    Int  i;
    AUD_INFOFRAME  InfoFrame;
    if (!pHdmiTx->m_SupportHdmi)
    {
        if (pHdmiTx->m_DisableEdidCheck)
            DtDbgOut(ERR, HDMI, "AUDIO Info Frames not supported, but forced by user.");
        else
        {
            DtDbgOut(ERR, HDMI, "AUDIO Info Frames not supported. No HDMI sink device.");
            return;
        }
    }

    // Fill Audio InfoFrame
    DtHdmiTxFillAudioInfoFrame(pHdmiTx, VidStd, AudioStd, &InfoFrame);

    // Write AUDIO InfoFrame to firmware
    for (i=0; i<sizeof(InfoFrame.m_Data)/4; i++)
        DtaRegHdmiTxAudInfoFrameSet(pHdmiTx->m_pHdmiRegs, i, InfoFrame.m_Data[i]);

    // Enable HDMI VS InfoFrame
    DtaRegHdmiTxInfoFrameCtrlSetAudInfoFrameEnable(pHdmiTx->m_pHdmiRegs, 1);
    DtaRegHdmiTxInfoFrameCtrlSetAudInfoFrameLatch(pHdmiTx->m_pHdmiRegs);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxUpdateAudStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//

#define DT_AUDSTD_UNKNOWN   -1
#define DT_AUDSTD_32000     1
#define DT_AUDSTD_44100     2
#define DT_AUDSTD_48000     3
#define DT_AUDSTD_96000     4

#define DT_AUDIO_FORMAT_INDEX_N      0
#define DT_AUDIO_FORMAT_INDEX_CTS    1

DtStatus  DtHdmiTxUpdateAudStd(DtaHdmiTx* pHdmiTx, DtaHdmiVidStd VidStd, 
                                                                   Int AudStd, Bool Force)
{
    Int  Index;
    Int  Multiply = 1;
    Int  FreqIndex = 2;
    Int  PixelClkIndex = 0;
    Int  N, CTS;

    // Always force settings
    Force = TRUE;

    if (pHdmiTx->m_UsedVidStd.m_VidStd==VidStd.m_VidStd
                              && pHdmiTx->m_UsedVidStd.m_AspectRatio==VidStd.m_AspectRatio
                              && pHdmiTx->m_SelAudStd==AudStd && !Force)
        return DT_STATUS_OK;

    switch (AudStd)
    {
    case DT_AUDSTD_32000: FreqIndex = 0; break;
    case DT_AUDSTD_44100: FreqIndex = 1; break;
    case DT_AUDSTD_48000: FreqIndex = 2; break;
    case DT_AUDSTD_96000: FreqIndex = 2; Multiply = 2; break;
    default: DT_ASSERT(FALSE);
    }

    Index = DtHdmiTxGetIndexVidStd(VidStd);
    DT_ASSERT(Index != -1);
    if (Index == -1)
        Index = 1;

    switch(DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_PIXEL_CLK])
    {
    case DT_HDMI_PIXCLK_13M5: PixelClkIndex = 0; break;
    case DT_HDMI_PIXCLK_27M: PixelClkIndex = 0; break;
    case DT_HDMI_PIXCLK_74M25: PixelClkIndex = 1; break;
    case DT_HDMI_PIXCLK_148M5: PixelClkIndex = 3; break;
    case DT_HDMI_PIXCLK_297M: PixelClkIndex = 5; break;
    case DT_HDMI_PIXCLK_594M: PixelClkIndex = 7; break;
    default: DT_ASSERT(FALSE);
    }

    // MULTIPLMODE not supported by current firmware
    DT_ASSERT((DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_FLAGS] & DT_HDMI_FLAGS_MULTIPLMODE) == 0);

    if ((DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_FLAGS] & DT_HDMI_FLAGS_FRACMODE) != 0)
    {
        DT_ASSERT(PixelClkIndex != 0); // 27M: No FRACMODE data
        PixelClkIndex++;
    }

    N = DT_HDMI_AUDIO_FORMAT[PixelClkIndex][FreqIndex][DT_AUDIO_FORMAT_INDEX_N];
    CTS = DT_HDMI_AUDIO_FORMAT[PixelClkIndex][FreqIndex][DT_AUDIO_FORMAT_INDEX_CTS];
    DtaRegHdmiTxAudioCtsSet(pHdmiTx->m_pHdmiRegs, CTS);
    DtaRegHdmiTxAudioNSet(pHdmiTx->m_pHdmiRegs, N);

    DtDbgOut(MAX, HDMI, "Setting audio N:%i, CTS:%i", N, CTS);

    DtHdmiTxUpdateAudioInfoFrame(pHdmiTx, VidStd, AudStd);


    if (VidStd.m_VidStd == DT_VIDSTD_480P59_94) 
    {
        // Enable throttling when in testpicture mode and enable audio channel 1+2
        DtaRegHdmiTxAudioCtrlSet(pHdmiTx->m_pHdmiRegs, 0x02010101);
    } else {
        // Enable audo channel 1+2
        DtaRegHdmiTxAudioCtrlSet(pHdmiTx->m_pHdmiRegs, 0x02010001);
    }

    pHdmiTx->m_SelAudStd = AudStd;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxUpdateVideoStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Used on the internal interface
//
DtStatus  DtHdmiTxUpdateVideoStd(DtaHdmiTx* pHdmiTx, DtaHdmiVidStd VidStd, Bool Force)
{
    Int  Index;
    Int  VidMode;
    Int  Timeout;
    DtaHdmiVidStd  UsedVidStd;
    Bool  UseTestPicture = FALSE;
    DtStatus  Status = DT_STATUS_OK;

    if (VidStd.m_VidStd == DT_VIDSTD_TS)
        UseTestPicture = TRUE;
    else 
    {
        if (VidStd.m_VidStd==DT_VIDSTD_UNKNOWN || VidStd.m_AspectRatio==DTA_AR_UNKNOWN) 
            VidStd = pHdmiTx->m_SelVidStd;

        DtDbgOut(AVG, HDMI, "New vidstd: %i:%s (Old vidstd: %i)", VidStd.m_VidStd, 
                              VidStdName(VidStd.m_VidStd), pHdmiTx->m_SelVidStd.m_VidStd);
    }
    
    // Always force settings
    Force = TRUE;

    if (pHdmiTx->m_SelVidStd.m_VidStd==VidStd.m_VidStd
                    && pHdmiTx->m_SelVidStd.m_AspectRatio==VidStd.m_AspectRatio && !Force)
        return DT_STATUS_OK;
    
    UsedVidStd = VidStd;
    
    if (!pHdmiTx->m_MonDetected)
    {
        if (pHdmiTx->m_ForceMonitorDetected)
            DtDbgOut(ERR, HDMI, "Monitor not detected, but forcing is enabled");
        else 
        {
            DtDbgOut(MIN, HDMI, "Monitor not detected. Skip setting HDMI");
            pHdmiTx->m_SelVidStd = VidStd;
            pHdmiTx->m_UsedVidStd.m_VidStd = DT_VIDSTD_UNKNOWN;
            pHdmiTx->m_UsedVidStd.m_AspectRatio = DTA_AR_UNKNOWN;
            return DT_STATUS_OK;
        }
    }
    
    Index = DtHdmiTxGetIndexVidStd(UsedVidStd);
    if (pHdmiTx->m_ForceTestPicture)
    {
        DtDbgOut(ERR, HDMI, "Forcing test picture by user!");
        UseTestPicture = TRUE;
    } else if (Index == -1)
    {
        DtDbgOut(MIN, HDMI, "Video standard not found.");
        UseTestPicture = TRUE;
    } else if ((((UInt64)1<<Index) & pHdmiTx->m_MonSupportedFormats)==0 && 
                                                             !pHdmiTx->m_DisableEdidCheck)
    {
        // Check if we can use another format supported by display.
        if (DtaVidStdIsSdSdi(UsedVidStd.m_VidStd) 
                                                 && UsedVidStd.m_AspectRatio==DTA_AR_16_9)
        {
            // Check if 4:3 format is supported
            UsedVidStd.m_AspectRatio = DTA_AR_4_3;
            Index = DtHdmiTxGetIndexVidStd(UsedVidStd);
            DT_ASSERT(Index != -1);
            if (Index==-1 || (((UInt64)1<<Index) & pHdmiTx->m_MonSupportedFormats)==0)
                UseTestPicture = TRUE;
            else
                DtDbgOut(MIN, HDMI, "Use SD-SDI 4:3 mode");
        } else if (DtaVidStdIs6gSdi(UsedVidStd.m_VidStd) 
                                                || DtaVidStdIs12gSdi(UsedVidStd.m_VidStd))
        {
            DtaHdmiVidStd  VidStd3g;
            // Check if we can do one subimage in case we have a 6G or 12G image
            Index = DtHdmiTxGetIndexVidStd3g(UsedVidStd, &VidStd3g);
            DT_ASSERT(Index != -1);
            if (Index==-1 || (((UInt64)1<<Index) & pHdmiTx->m_MonSupportedFormats)==0)
                UseTestPicture = TRUE;
            else
            {
                UsedVidStd = VidStd3g;
                DtDbgOut(MIN, HDMI, "Use 3G video standard: %i in stead of 6/12G: %i", 
                                                                      UsedVidStd, VidStd);
            }
        } else
            UseTestPicture = TRUE;
    }

    // Disable HDMI output
    DtaRegHdmiTxGenCtrlSetEnable(pHdmiTx->m_pHdmiRegs, 0);
    
    if (UseTestPicture)
    {
        UsedVidStd.m_VidStd = DT_VIDSTD_480P59_94;
        UsedVidStd.m_AspectRatio = DTA_AR_16_9;
        Index = DtHdmiTxGetIndexVidStd(UsedVidStd);
        if ((((UInt64)1<<Index) & pHdmiTx->m_MonSupportedFormats) == 0)
        {
            UsedVidStd.m_AspectRatio = DTA_AR_4_3;
            Index = DtHdmiTxGetIndexVidStd(UsedVidStd);
            DtDbgOut(MIN, HDMI, "Setting DT_VIDSTD_480P59_94 4:3 mode");
        } else
            DtDbgOut(MIN, HDMI, "Setting DT_VIDSTD_480P59_94 16:9 mode");
    }
    DtaRegHdmiTxGenCtrlSetTestPatternEnable(pHdmiTx->m_pHdmiRegs, UseTestPicture);

    // MULTIPLMODE not supported by current firmware
    DT_ASSERT((DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_FLAGS] & DT_HDMI_FLAGS_MULTIPLMODE)
                                                                                    == 0);

    // Pixel clk
    DtaRegHdmiTxGenCtrlSetPixelClk(pHdmiTx->m_pHdmiRegs, 
                                      DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_PIXEL_CLK]);
    Timeout = 0;
    while (DtaRegHdmiTxGenStatGetPllLockStat(pHdmiTx->m_pHdmiRegs)==0 && Timeout<100)
    {
        DtSleep(10);
        Timeout++;
    }
    if (Timeout >= 100)
        DtDbgOut(ERR, HDMI, "Timeout waiting for PLL locked!!");

    DtHdmiTxUpdateAudStd(pHdmiTx, UsedVidStd, DT_AUDSTD_48000, FALSE);

    // Set pixel repetition
    if ((DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_FLAGS]&DT_HDMI_FLAGS_PIXELREP)== 0)
        DtaRegHdmiTxGenCtrlSetPixelRep(pHdmiTx->m_pHdmiRegs, 0);
    else
        DtaRegHdmiTxGenCtrlSetPixelRep(pHdmiTx->m_pHdmiRegs, 1);

    // Enable HDMI mode
    DtaRegHdmiTxGenCtrlSetHdmiMode(pHdmiTx->m_pHdmiRegs, (pHdmiTx->m_SupportHdmi?1:0));

    // Fractional mode
    DtaRegGenCtrlSetFracClkEnable(pHdmiTx->m_pHdmiRegs,
          (DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_FLAGS] & DT_HDMI_FLAGS_FRACMODE) != 0);

    // Clock ratio
    if ((DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_FLAGS] & DT_HDMI_FLAGS_CLKRATIO_4K) == 0)
    {
        DtaRegGenCtrlSetScramblerEnable(pHdmiTx->m_pHdmiRegs, 0);
        DtaRegHdmiTxGenCtrlSetTmdsClockRatio(pHdmiTx->m_pHdmiRegs, 
                                                             DT_HDMI_TMDS_CLKRATIO_L3_4G);
    } else {
        DtaRegGenCtrlSetScramblerEnable(pHdmiTx->m_pHdmiRegs, 1);
        DtaRegHdmiTxGenCtrlSetTmdsClockRatio(pHdmiTx->m_pHdmiRegs, 
                                                               DT_HDMI_TMDS_CLKRATIO_L6G);
    }

    // Video mode
    // Check if video mode is supported
    VidMode = pHdmiTx->m_SelVidMode;
    switch (VidMode)
    {
        case DT_HDMI_VIDMOD_YCBCR_422: 
            if (!pHdmiTx->m_SupportYCbCr422)
            {
                if (pHdmiTx->m_DisableEdidCheck)
                      DtDbgOut(ERR, HDMI, "YCbCr422 not supported by display,"
                                                                  " but forced by user!");
                else if (!pHdmiTx->m_SupportYCbCr444)
                    VidMode = DT_HDMI_VIDMOD_RGB_444;
                else
                    VidMode = DT_HDMI_VIDMOD_YCBCR_444;
            }
            break;
        case DT_HDMI_VIDMOD_YCBCR_444:
            if (!pHdmiTx->m_SupportYCbCr444)
            {
                if (pHdmiTx->m_DisableEdidCheck)
                      DtDbgOut(ERR, HDMI, "YCbCr444 not supported by display,"
                                                                  " but forced by user!");
                else if (!pHdmiTx->m_SupportYCbCr422)
                    VidMode = DT_HDMI_VIDMOD_RGB_444;
                else
                    VidMode = DT_HDMI_VIDMOD_YCBCR_422;
            }
            break;
        case DT_HDMI_VIDMOD_RGB_444:
            break;
        default:
            DT_ASSERT(FALSE);
    }
    if (VidMode != pHdmiTx->m_SelVidMode)
        DtDbgOut(AVG, HDMI, "VidMode %i not supported. Forcing %i", 
                                                          pHdmiTx->m_SelVidMode, VidMode);
    pHdmiTx->m_UsedVidMode = VidMode;
    DtaRegHdmiTxVideoCtrlSetVideoMode(pHdmiTx->m_pHdmiRegs, VidMode);

    // Progressive/Interlaced
    if ((DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_FLAGS] & DT_HDMI_FLAGS_INTERLACED) == 0)
        DtaRegHdmiTxVideoCtrlSetInterlacedEnable(pHdmiTx->m_pHdmiRegs, 0);
    else
        DtaRegHdmiTxVideoCtrlSetInterlacedEnable(pHdmiTx->m_pHdmiRegs, 1);
    
    // H sync. polarity
    if ((DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_FLAGS] & DT_HDMI_FLAGS_HSPOL_NEG) == 0)
        DtaRegHdmiTxVideoCtrlSetHSyncPolPositive(pHdmiTx->m_pHdmiRegs, 1);
    else
        DtaRegHdmiTxVideoCtrlSetHSyncPolPositive(pHdmiTx->m_pHdmiRegs, 0);
    // V sync. polarity
    if ((DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_FLAGS] & DT_HDMI_FLAGS_VSPOL_NEG) == 0)
        DtaRegHdmiTxVideoCtrlSetVSyncPolPositive(pHdmiTx->m_pHdmiRegs, 1);
    else
        DtaRegHdmiTxVideoCtrlSetVSyncPolPositive(pHdmiTx->m_pHdmiRegs, 0);
    
    // Video Timing
    DtaRegHdmiTxHActWidthSet(pHdmiTx->m_pHdmiRegs, DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_H_ACT]);
    DtaRegHdmiTxHSyncWidthSet(pHdmiTx->m_pHdmiRegs, DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_H_SYNC]);
    DtaRegHdmiTxHFPWidthSet(pHdmiTx->m_pHdmiRegs, DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_H_FPORCH]);
    DtaRegHdmiTxHBPWidthSet(pHdmiTx->m_pHdmiRegs, DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_H_BPORCH]);

    DtaRegHdmiTxF1VSyncWidthSet(pHdmiTx->m_pHdmiRegs, DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_F1_V_SYNC]);
    DtaRegHdmiTxF1VFPWidthSet(pHdmiTx->m_pHdmiRegs, DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_F1_V_FPORCH]);
    DtaRegHdmiTxF1VBPWidthSet(pHdmiTx->m_pHdmiRegs, DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_F1_V_BPORCH]);
    if ((DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_FLAGS] & DT_HDMI_FLAGS_INTERLACED) == 0)
        DtaRegHdmiTxF1VActWidthSet(pHdmiTx->m_pHdmiRegs, DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_V_ACT]);
    else 
    {
        DtaRegHdmiTxF1VActWidthSet(pHdmiTx->m_pHdmiRegs, DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_V_ACT]);
        DtaRegHdmiTxF2VActWidthSet(pHdmiTx->m_pHdmiRegs, DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_V_ACT]);
        DtaRegHdmiTxF2VSyncWidthSet(pHdmiTx->m_pHdmiRegs, DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_F2_V_SYNC]);
        DtaRegHdmiTxF2VFPWidthSet(pHdmiTx->m_pHdmiRegs, DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_F2_V_FPORCH]);
        DtaRegHdmiTxF2VBPWidthSet(pHdmiTx->m_pHdmiRegs, DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_F2_V_BPORCH]);
    }

    // Info frame control
    DtHdmiTxUpdateAviInfoFrame(pHdmiTx, UsedVidStd);
    DtHdmiTxUpdateHdmiVsInfoFrame(pHdmiTx, UsedVidStd);

    // SCDC
    if (pHdmiTx->m_SupportScDc || pHdmiTx->m_DisableEdidCheck)
    {
        Status = DtHdmiScdcSetTmdsConfig(pHdmiTx, 
                                          DT_HDMI_VIDSTD_2_FORMAT[Index][HDMI_IDX_FLAGS]);
        if (!DT_SUCCESS(Status))
            DtDbgOut(AVG, HDMI, "Error setting SCDC TMDS config");
        else
            DtDbgOut(MAX, HDMI, "Setting SCDC TMDS config succeeded");
        if (pHdmiTx->m_DisableEdidCheck)
            Status = DT_STATUS_OK;
    }

    Status = DtHdmiTxProgramHdmiOutputDriver(pHdmiTx, UsedVidStd);
    if (!DT_SUCCESS(Status))
        DtDbgOut(ERR, HDMI, "Error updating HDMI output driver");

    // Reset
    DtaRegHdmiTxGenCtrlReset(pHdmiTx->m_pHdmiRegs);

    // Enable HDMI output
    if (!pHdmiTx->m_DisableHdmiOutput)
    {
        DtSleep(100);
        DtaRegHdmiTxGenCtrlSetEnable(pHdmiTx->m_pHdmiRegs, 1);
    }

    
    pHdmiTx->m_SelVidStd = VidStd;
    pHdmiTx->m_UsedVidStd = UsedVidStd;
    pHdmiTx->m_UsingTestPicture = UseTestPicture;
    return Status;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Eedid helper functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidChecksumOk -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtHdmiEdidChecksumOk(UInt8* pBlock)
{
    Int  i;
    UInt8  Checksum = 0;

    // All bytes must be
    for (i=0; i<EDID_BLOCK_SIZE; i++)
    {
        Checksum += pBlock[i];
    }
    DT_ASSERT(Checksum == 0);
    return Checksum == 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidDecodeFeatureSupport -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtHdmiEdidDecodeFeatureSupport(UInt8 Features)
{
    if ((Features & 0x80) != 0)
        DtDbgOut(MAX, HDMI, "Standby supported");
    else
        DtDbgOut(MAX, HDMI, "Standby NOT supported");
    if ((Features & 0x40) != 0)
        DtDbgOut(MAX, HDMI, "Suspend supported");
    else
        DtDbgOut(MAX, HDMI, "Suspend NOT supported");
    if ((Features & 0x20) != 0)
        DtDbgOut(MAX, HDMI, "Active off supported");
    else
        DtDbgOut(MAX, HDMI, "Active off NOT supported");
    if (((Features>>3) & 0x3) == 0)
        DtDbgOut(MAX, HDMI, "Monochrome Display");
    else if (((Features>>3) & 0x3) == 1)
        DtDbgOut(MAX, HDMI, "RGB Display");
    else if (((Features>>3) & 0x3) == 2)
        DtDbgOut(MAX, HDMI, "Non-RGB Display");
    else
        DtDbgOut(MAX, HDMI, "Undefined Display");
    if ((Features & 0x4) != 0)
        DtDbgOut(MAX, HDMI, "sRGB supported");
    else
        DtDbgOut(MAX, HDMI, "sRGB NOT supported");
    if ((Features & 0x2) != 0)
        DtDbgOut(MAX, HDMI, "Preferred Timing supported");
    else
        DtDbgOut(MAX, HDMI, "Preferred Timing NOT supported");
    if ((Features & 0x1) != 0)
        DtDbgOut(MAX, HDMI, "GTF supported");
    else
        DtDbgOut(MAX, HDMI, "GTF NOT supported");
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidDecodeEstablishedTimings -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtHdmiEdidDecodeEstablishedTimings(UInt8* pETimings)
{
    if ((pETimings[0] & 0x80) != 0)
        DtDbgOut(MAX, HDMI, "720 x 400 @ 70Hz supported");
    if ((pETimings[0] & 0x40) != 0)
        DtDbgOut(MAX, HDMI, "720 x 400 @ 88Hz supported");
    if ((pETimings[0] & 0x20) != 0)
        DtDbgOut(MAX, HDMI, "640 x 480 @ 60Hz supported");
    if ((pETimings[0] & 0x10) != 0)
        DtDbgOut(MAX, HDMI, "640 x 480 @ 67Hz supported");
    if ((pETimings[0] & 0x08) != 0)
        DtDbgOut(MAX, HDMI, "640 x 480 @ 72Hz supported");
    if ((pETimings[0] & 0x04) != 0)
        DtDbgOut(MAX, HDMI, "640 x 480 @ 75Hz supported");
    if ((pETimings[0] & 0x02) != 0)
        DtDbgOut(MAX, HDMI, "800 x 600 @ 56Hz supported");
    if ((pETimings[0] & 0x01) != 0)
        DtDbgOut(MAX, HDMI, "800 x 600 @ 60Hz supported");

    if ((pETimings[1] & 0x80) != 0)
        DtDbgOut(MAX, HDMI, "800 x 600 @ 72Hz supported");
    if ((pETimings[1] & 0x40) != 0)
        DtDbgOut(MAX, HDMI, "800 x 600 @ 75Hz supported");
    if ((pETimings[1] & 0x20) != 0)
        DtDbgOut(MAX, HDMI, "832 x 624 @ 75Hz supported");
    if ((pETimings[1] & 0x10) != 0)
        DtDbgOut(MAX, HDMI, "1024 x 768 @ 87Hz(I) supported");
    if ((pETimings[1] & 0x08) != 0)
        DtDbgOut(MAX, HDMI, "1024 x 768 @ 60Hz supported");
    if ((pETimings[1] & 0x04) != 0)
        DtDbgOut(MAX, HDMI, "1024 x 768 @ 70Hz supported");
    if ((pETimings[1] & 0x02) != 0)
        DtDbgOut(MAX, HDMI, "1024 x 768 @ 75Hz supported");
    if ((pETimings[1] & 0x01) != 0)
        DtDbgOut(MAX, HDMI, "1280 x 1024 @ 75Hz supported");

    if ((pETimings[2] & 0x80) != 0)
        DtDbgOut(MAX, HDMI, "1152 x 870 @ 75Hz supported");
    if ((pETimings[2] & 0x7F) != 0)
        DtDbgOut(MAX, HDMI, "Other manuf. specific timings supported");
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidDecodeEstablishedTimings3 -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtHdmiEdidDecodeEstablishedTimings3(UInt8* pETimings)
{
    if ((pETimings[0] & 0x80) != 0)
        DtDbgOut(MAX, HDMI, "640 x 350 @ 85Hz supported");
    if ((pETimings[0] & 0x40) != 0)
        DtDbgOut(MAX, HDMI, "640 x 400 @ 85Hz supported");
    if ((pETimings[0] & 0x20) != 0)
        DtDbgOut(MAX, HDMI, "720 x 400 @ 85Hz supported");
    if ((pETimings[0] & 0x10) != 0)
        DtDbgOut(MAX, HDMI, "640 x 480 @ 85Hz supported");
    if ((pETimings[0] & 0x08) != 0)
        DtDbgOut(MAX, HDMI, "848 x 480 @ 60Hz supported");
    if ((pETimings[0] & 0x04) != 0)
        DtDbgOut(MAX, HDMI, "800 x 600 @ 85Hz supported");
    if ((pETimings[0] & 0x02) != 0)
        DtDbgOut(MAX, HDMI, "1024 x 768 @ 85Hz supported");
    if ((pETimings[0] & 0x01) != 0)
        DtDbgOut(MAX, HDMI, "1152 x 864 @ 75Hz supported");

    if ((pETimings[1] & 0x80) != 0)
        DtDbgOut(MAX, HDMI, "1280 x 768 @ 60Hz (RB) supported");
    if ((pETimings[1] & 0x40) != 0)
        DtDbgOut(MAX, HDMI, "1280 x 768 @ 60Hz supported");
    if ((pETimings[1] & 0x20) != 0)
        DtDbgOut(MAX, HDMI, "1280 x 768 @ 75Hz supported");
    if ((pETimings[1] & 0x10) != 0)
        DtDbgOut(MAX, HDMI, "1280 x 768 @ 85Hz supported");
    if ((pETimings[1] & 0x08) != 0)
        DtDbgOut(MAX, HDMI, "1280 x 960 @ 60Hz supported");
    if ((pETimings[1] & 0x04) != 0)
        DtDbgOut(MAX, HDMI, "1280 x 960 @ 85Hz supported");
    if ((pETimings[1] & 0x02) != 0)
        DtDbgOut(MAX, HDMI, "1280 x 1024 @ 60Hz supported");
    if ((pETimings[1] & 0x01) != 0)
        DtDbgOut(MAX, HDMI, "1280 x 1024 @ 85Hz supported");

    if ((pETimings[2] & 0x80) != 0)
        DtDbgOut(MAX, HDMI, "1360 x 768 @ 60Hz supported");
    if ((pETimings[2] & 0x40) != 0)
        DtDbgOut(MAX, HDMI, "1440 x 900 @ 60Hz (RB) supported");
    if ((pETimings[2] & 0x20) != 0)
        DtDbgOut(MAX, HDMI, "1440 x 900 @ 60Hz supported");
    if ((pETimings[2] & 0x10) != 0)
        DtDbgOut(MAX, HDMI, "1440 x 900 @ 75Hz supported");
    if ((pETimings[2] & 0x08) != 0)
        DtDbgOut(MAX, HDMI, "1440 x 900 @ 85Hz supported");
    if ((pETimings[2] & 0x04) != 0)
        DtDbgOut(MAX, HDMI, "1400 x 1050 @ 60Hz supported");
    if ((pETimings[2] & 0x02) != 0)
        DtDbgOut(MAX, HDMI, "1400 x 1050 @ 60Hz supported");
    if ((pETimings[2] & 0x01) != 0)
        DtDbgOut(MAX, HDMI, "1400 x 1050 @ 75Hz supported");

    if ((pETimings[3] & 0x80) != 0)
        DtDbgOut(MAX, HDMI, "1400 x 1050 @ 85Hz supported");
    if ((pETimings[3] & 0x40) != 0)
        DtDbgOut(MAX, HDMI, "1680 x 1050 @ 60Hz supported");
    if ((pETimings[3] & 0x20) != 0)
        DtDbgOut(MAX, HDMI, "1680 x 1050 @ 60Hz supported");
    if ((pETimings[3] & 0x10) != 0)
        DtDbgOut(MAX, HDMI, "1680 x 1050 @ 75Hz supported");
    if ((pETimings[3] & 0x08) != 0)
        DtDbgOut(MAX, HDMI, "1680 x 1050 @ 85Hz supported");
    if ((pETimings[3] & 0x04) != 0)
        DtDbgOut(MAX, HDMI, "1600 x 1200 @ 60Hz supported");
    if ((pETimings[3] & 0x02) != 0)
        DtDbgOut(MAX, HDMI, "1600 x 1200 @ 65Hz supported");
    if ((pETimings[3] & 0x01) != 0)
        DtDbgOut(MAX, HDMI, "1600 x 1200 @ 70Hz supported");

    if ((pETimings[4] & 0x80) != 0)
        DtDbgOut(MAX, HDMI, "1600 x 1200 @ 75Hz supported");
    if ((pETimings[4] & 0x40) != 0)
        DtDbgOut(MAX, HDMI, "1600 x 1200 @ 85Hz supported");
    if ((pETimings[4] & 0x20) != 0)
        DtDbgOut(MAX, HDMI, "1792 x 1344 @ 60Hz supported");
    if ((pETimings[4] & 0x10) != 0)
        DtDbgOut(MAX, HDMI, "1792 x 1344 @ 75Hz supported");
    if ((pETimings[4] & 0x08) != 0)
        DtDbgOut(MAX, HDMI, "1856 x 1392 @ 60Hz supported");
    if ((pETimings[4] & 0x04) != 0)
        DtDbgOut(MAX, HDMI, "1856 x 1392 @ 75Hz supported");
    if ((pETimings[4] & 0x02) != 0)
        DtDbgOut(MAX, HDMI, "1920 x 1200 @ 60Hz (RB) supported");
    if ((pETimings[4] & 0x01) != 0)
        DtDbgOut(MAX, HDMI, "1920 x 1200 @ 60Hz supported");

    if ((pETimings[5] & 0x80) != 0)
        DtDbgOut(MAX, HDMI, "1920 x 1200 @ 75Hz supported");
    if ((pETimings[5] & 0x40) != 0)
        DtDbgOut(MAX, HDMI, "1920 x 1200 @ 85Hz supported");
    if ((pETimings[5] & 0x20) != 0)
        DtDbgOut(MAX, HDMI, "1920 x 1440 @ 60Hz supported");
    if ((pETimings[5] & 0x10) != 0)
        DtDbgOut(MAX, HDMI, "1920 x 1440 @ 75Hz supported");
    if ((pETimings[5] & 0x0F) != 0)
        DtDbgOut(MAX, HDMI, "Reserved bits: %xh", pETimings[5] & 0x0F);
    if (pETimings[6]!=0 || pETimings[7]!=0 || pETimings[8]!=0 || pETimings[9]!=0 || 
                                                     pETimings[10]!=0 || pETimings[11]!=0)
        DtDbgOut(MAX, HDMI, "Reserved bytes <>0");
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidDecodeStandardTimings -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtHdmiEdidDecodeStandardTimings(UInt8* pStandardTimings, Int Count)
{
    Int  HorzAddrPixels;
    Int  AR;
    Int  RefrRate;
    Int  i;
    for (i=0; i<Count; i++)
    {
        if (pStandardTimings[i*2] == 0x1 && pStandardTimings[i*2+1] == 0x1)
            DtDbgOut(MAX, HDMI, "Standard Timing %i: not used", i+1);
        else {
            HorzAddrPixels = (pStandardTimings[i*2] + 31) * 8;
            AR = pStandardTimings[i*2+1]>>6 & 3;
            RefrRate = (pStandardTimings[i*2+1] & 0x3f) + 60;
            DtDbgOut(MAX, HDMI, "Standard Timing %i: horz addr. pixels: %i, "
                          "FieldRefreshRate: %i, AR:%s", i+1,
                          HorzAddrPixels, RefrRate, 
                          (AR==0?"16:10":(AR==1?"4:3":(AR==2?"5:4":(AR==3?"16:9":"?")))));
            // Vertical addressble line count may be calculated from the aspect ratio and 
            // horz.addr. pixels
        }
    }
}

//-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidDecodeDetailedTimingDesciptor -.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtHdmiEdidDecodeDetailedTimingDesciptor(UInt8* pDetailTimings, HDMI_EDID_DTD* pDtd)
{
    pDtd->m_Flags = 0;
    pDtd->m_PixelClock = pDetailTimings[0] + ((UInt)pDetailTimings[1]<<8);
    pDtd->m_PixelClock *= 10000;
    pDtd->m_HActivePixels = pDetailTimings[2] + ((UInt)(pDetailTimings[4]&0xF0)<<4);
    pDtd->m_HBlankingPixels = pDetailTimings[3] + ((UInt)(pDetailTimings[4]&0xF)<<8);
    pDtd->m_VActiveLines = pDetailTimings[5] + ((UInt)(pDetailTimings[7]&0xF0)<<4);
    pDtd->m_VBlankingLines = pDetailTimings[6] + ((UInt)(pDetailTimings[7]&0xF)<<8);
    
    pDtd->m_HFrontPorchPixels = pDetailTimings[8] + ((UInt)(pDetailTimings[11]&0xC0)<<2);
    pDtd->m_HSyncPulseWidth = pDetailTimings[9] + ((UInt)(pDetailTimings[11]&0x30)<<4);
    pDtd->m_VFrontPorchLines = ((pDetailTimings[10]&0xf0)>>4) + 
                                                           ((pDetailTimings[11]&0x0C)<<2);
    pDtd->m_VSyncPulseWidth = (pDetailTimings[10]&0x0f) + ((pDetailTimings[11]&0x03)<<4);

    pDtd->m_HActiveVideoSize = pDetailTimings[12] + ((UInt)(pDetailTimings[14]&0xF0)<<4);
    pDtd->m_VActiveVideoSize = pDetailTimings[13] + ((UInt)(pDetailTimings[14]&0x0F)<<8);

    pDtd->m_HBorderPixels = pDetailTimings[15];
    pDtd->m_VBorderLines = pDetailTimings[16];

    if ((pDetailTimings[17]&0x80) != 0)
    {
        DtDbgOut(MAX, HDMI, "Flag: Interlaced (1 frame = 2 fields)");
        pDtd->m_Flags |= DT_HDMI_FLAGS_INTERLACED;
    } else
        DtDbgOut(MAX, HDMI, "Flag: Non-Interlaced (1 frame = 1 field)");
    if ((pDetailTimings[17]&0x60) == 0)
        DtDbgOut(MAX, HDMI, "Flag: Normal Display - No Stereo.");
    else if ((pDetailTimings[17]&0x61) == 0x20)
        DtDbgOut(MAX, HDMI, "Flag: Field sequential stereo, right image when stereo sync"
                                                                           " signal = 1");
    else if ((pDetailTimings[17]&0x61) == 0x40)
        DtDbgOut(MAX, HDMI, "Flag: Field sequential stereo, left image when stereo sync" 
                                                                           " signal = 1");
    else if ((pDetailTimings[17]&0x61) == 0x21)
        DtDbgOut(MAX, HDMI, "Flag: 2-way interleaced stereo, right image on even lines");
    else if ((pDetailTimings[17]&0x61) == 0x41)
        DtDbgOut(MAX, HDMI, "Flag: 2-way interleaced stereo, left image on even lines");
    else if ((pDetailTimings[17]&0x61) == 0x60)
        DtDbgOut(MAX, HDMI, "Flag: 4-way interleaced stereo");
    else if ((pDetailTimings[17]&0x61) == 0x61)
        DtDbgOut(MAX, HDMI, "Flag: Side-by-side interleaved stereo");
    if ((pDetailTimings[17]&0x10) == 0)
    {   // Analog sync signal
        if ((pDetailTimings[17]&0x08) == 0)
            DtDbgOut(MAX, HDMI, "Flag: Analog Composite Sync:");
        else
            DtDbgOut(MAX, HDMI, "Flag: Bipolar Analog Composite Sync:");
        if ((pDetailTimings[17]&0x02) == 0)
            DtDbgOut(MAX, HDMI, "Flag: -->without serrations;");
        else
            DtDbgOut(MAX, HDMI, "Flag: -->with serrations (H-sync during V-sync);");
        if ((pDetailTimings[17]&0x01) == 0)
            DtDbgOut(MAX, HDMI, "Flag: -->sync on green signal only;");
        else
            DtDbgOut(MAX, HDMI, "Flag: -->sync on all three (RGB) video signals;");
    } else
    {   // Digital sync signal
        if ((pDetailTimings[17]&0x08) == 0)
            DtDbgOut(MAX, HDMI, "Flag: Digital Composite Sync:");
        else
            DtDbgOut(MAX, HDMI, "Flag: Digital Separate Sync:");

        if ((pDetailTimings[17]&0x0c) == 0x00)
            DtDbgOut(MAX, HDMI, "Flag:-->without serrations;");
        else if ((pDetailTimings[17]&0x0c) == 0x04)
            DtDbgOut(MAX, HDMI, "Flag:-->with serrations (H-sync during V-sync);");
        else if ((pDetailTimings[17]&0x0c) == 0x08)
        {
            DtDbgOut(MAX, HDMI, "Flag:-->vertical sync is negative;");
            pDtd->m_Flags |= DT_HDMI_FLAGS_VSPOL_NEG;
        } else if ((pDetailTimings[17]&0x0c) == 0x0c)
            DtDbgOut(MAX, HDMI, "Flag:-->vertical sync is positive;");
        
        if ((pDetailTimings[17]&0x02) == 0x0)
        {
            DtDbgOut(MAX, HDMI, "Flag:-->horizontal sync is negative (outside of"
                                                                             " V-sync);");
            pDtd->m_Flags |= DT_HDMI_FLAGS_HSPOL_NEG;
        } else
            DtDbgOut(MAX, HDMI, "Flag:-->horizontal sync is positive (outside of"
                                                                             " V-sync);");
    }
    
    DtDbgOut(MAX, HDMI, "Detailed Timing: PixelClock:%i HActive:%i "
                 "HBlanking:%i VActive:%i VBlanking:%i HFrontPorch:%i HSyncPulseWidth:%i "
                 "VFrontPorch:%i VSyncPulseWidth:%i"
                 " Flags:%xh...etc. ",
                 pDtd->m_PixelClock, pDtd->m_HActivePixels, pDtd->m_HBlankingPixels,
                 pDtd->m_VActiveLines, pDtd->m_VBlankingLines,
                 pDtd->m_HFrontPorchPixels, pDtd->m_HSyncPulseWidth,
                 pDtd->m_VFrontPorchLines, pDtd->m_VSyncPulseWidth,
                 pDetailTimings[17]);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidDecodeDisplayRangeLimits -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtHdmiEdidDecodeDisplayRangeLimits(DtaHdmiTx* pHdmiTx, UInt8* pDisplayRL)
{
    Int  MaxVOffset=0;
    Int  MinVOffset=0;
    Int  MaxHOffset=0;
    Int  MinHOffset=0;
    Int  MaxPixClkOffset = 0;

    if ((pDisplayRL[0] & 0x03) == 0)
        DtDbgOut(MAX, HDMI, "Vertical Rate Offsets are zero");
    else if ((pDisplayRL[0] & 0x03) == 1)
        DtDbgOut(MAX, HDMI, "Vertical Rate Offsets RESERVED");
    else if ((pDisplayRL[0] & 0x03) == 2)
    {
        DtDbgOut(MAX, HDMI, "Max. Vertical Rate+255Hz offset; Min. Vertical Rate is not"
                                                                               " offset");
        MaxVOffset = 255;
    }
    else if ((pDisplayRL[0] & 0x03) == 3)
    {
        DtDbgOut(MAX, HDMI, "Max. Vertical Rate+255Hz offset; Min. Vertical Rate+255Hz"
                                                                               " offset");
        MaxVOffset = 255;
        MinVOffset = 255;
    }

    if ((pDisplayRL[0] & 0x0C) == 0)
        DtDbgOut(MAX, HDMI, "Horizontal Rate Offsets are zero");
    else if ((pDisplayRL[0] & 0x0C) == 0x4)
        DtDbgOut(MAX, HDMI, "Horizontal Rate Offsets RESERVED");
    else if ((pDisplayRL[0] & 0x0C) == 0x8)
    {
        DtDbgOut(MAX, HDMI, "Max. Horizontal Rate+255Hz offset; Min. Horizontal Rate is" 
                                                                           " not offset");
        MaxHOffset = 255;
    }
    else if ((pDisplayRL[0] & 0x03) == 0xC)
    {
        DtDbgOut(MAX, HDMI, "Max. Horitontal Rate+255Hz offset; Min. Horizontal"
                                                                    " Rate+255Hz offset");
        MaxHOffset = 255;
        MinHOffset = 255;
    }

    pHdmiTx->m_SupportMonitorRangeLimits = TRUE;
    pHdmiTx->m_MinVRate = (UInt)pDisplayRL[1] + MinVOffset;
    pHdmiTx->m_MaxVRate = (UInt)pDisplayRL[2] + MaxVOffset;
    pHdmiTx->m_MinHRate = (UInt)pDisplayRL[3] + MinHOffset;
    pHdmiTx->m_MaxHRate = (UInt)pDisplayRL[4] + MaxHOffset;
    pHdmiTx->m_MaxPixelClk = (UInt)pDisplayRL[5] * 10;

    DtDbgOut(MAX, HDMI, "Min. vertial rate:%i Hz, Max. vertical rate:%i Hz", 
                                      pDisplayRL[1]+MinVOffset, pDisplayRL[2]+MaxVOffset);
    DtDbgOut(MAX, HDMI, "Min. horizontal rate:%i Hz, Max. horizontal rate:%i Hz", 
                                      pDisplayRL[3]+MinHOffset, pDisplayRL[4]+MaxHOffset);
    DtDbgOut(MAX, HDMI, "Max. pixel clock: %i MHz", (Int)pDisplayRL[5]*10);
    if (pDisplayRL[5] == 0)
        DtDbgOut(MAX, HDMI, "Default GTF support if feature is supported");
    if ((pDisplayRL[5]&0x1) != 0)
        DtDbgOut(MAX, HDMI, "Range limits only--no additional timing information");
    if ((pDisplayRL[5]&0x2) != 0)
        DtDbgOut(MAX, HDMI, "Secondary GTF supported");
    if ((pDisplayRL[5]&0x4) != 0)
        DtDbgOut(MAX, HDMI, "CVT supported if feature is supported");
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidDecodeDisplayProductName -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtHdmiEdidDecodeDisplayProductName(UInt8* pProductName)
{
    char  Name[14];
    Int  i = 0;

    DtMemZero(Name, 14);
    while(i<13 && pProductName[i]!=0xa)
    {
        Name[i] = pProductName[i];
        i++;
    }
    DtDbgOut(MAX, HDMI, "Display Product Name: '%s'", Name);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidDecodeDisplaySerial -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtHdmiEdidDecodeDisplaySerial(UInt8* pSerial)
{
    char  Serial[14];
    Int  i = 0;

    DtMemZero(Serial, 14);
    while(i<13 && pSerial[i]!=0xa)
    {
        Serial[i] = pSerial[i];
        i++;
    }
    DtDbgOut(MAX, HDMI, "Display Serial: '%s'", Serial);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidDecodeDisplayDescriptor -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtHdmiEdidDecodeDisplayDescriptor(DtaHdmiTx* pHdmiTx, UInt8* pDisplDescr)
{
    if (pDisplDescr[3] == 0xFF)
    {
        DtDbgOut(MAX, HDMI, "-->Display Product Serial Number");
        DtHdmiEdidDecodeDisplaySerial(&pDisplDescr[5]);
    }
    else if (pDisplDescr[3] == 0xFE)
        DtDbgOut(MAX, HDMI, "-->Alphanumeric Data String");
    else if (pDisplDescr[3] == 0xFD)
    {
        DtDbgOut(MAX, HDMI, "-->Display Range Limits");
        DtHdmiEdidDecodeDisplayRangeLimits(pHdmiTx, &pDisplDescr[4]);
    }
    else if (pDisplDescr[3] == 0xFC)
    {
        DtDbgOut(MAX, HDMI, "-->Display Product Name");
        DtHdmiEdidDecodeDisplayProductName(&pDisplDescr[5]);
    }
    else if (pDisplDescr[3] == 0xFB)
        DtDbgOut(MAX, HDMI, "-->Color Point Data");
    else if (pDisplDescr[3] == 0xFA)
    {
        DtDbgOut(MAX, HDMI, "-->Standard Timing Identifications");
        DtHdmiEdidDecodeStandardTimings(&pDisplDescr[5], 6);
    }
    else if (pDisplDescr[3] == 0xF9)
        DtDbgOut(MAX, HDMI, "-->Display Color Management (DCM) data");
    else if (pDisplDescr[3] == 0xF8)
        DtDbgOut(MAX, HDMI, "-->CVT 3 Byte Timing Codes");
    else if (pDisplDescr[3] == 0xF7)
    {
        DtDbgOut(MAX, HDMI, "-->Established Timings III");
        DtHdmiEdidDecodeEstablishedTimings3(&pDisplDescr[6]);
    }
    else if (pDisplDescr[3] == 0x10)
        DtDbgOut(MAX, HDMI, "-->Dummy Descriptor");
    else if (pDisplDescr[3] <= 0x0F)
        DtDbgOut(MAX, HDMI, "-->Manufacturer Specified Display Descriptors");
    else
        DtDbgOut(MAX, HDMI, "-->Reserved: %xh", pDisplDescr[3]);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidDecodeDataDescriptors -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtHdmiEdidDecodeDataDescriptors(DtaHdmiTx* pHdmiTx, UInt8* pDataDescr, Int Count)
{
    UInt8*  pDescr;
    Bool  Empty;
    Int  i, j;
    Bool  Field2 = FALSE;
    UInt64  SupportedField1;
    UInt64  Supported;

    for (i=0; i<Count; i++)
    {
        pDescr = pDataDescr + (i * 18);
        Empty = TRUE;
        for (j=0; j<18 && Empty; j++)
            if (pDescr[j] !=0)
                Empty = FALSE;
        if (Empty)
        {
            Field2 = FALSE;
            DtDbgOut(MAX, HDMI, "Data Descriptor %i EMPTY", i);
        } else if (pDescr[0]==0 && pDescr[1]==0 /*&&  pDescr[2]==0*/)
        {
            DtDbgOut(MAX, HDMI, "Data Descriptor %i: Display Descriptor", i);
            DtHdmiEdidDecodeDisplayDescriptor(pHdmiTx, pDescr);
            Field2 = FALSE;
        }
        else 
        {
            // Detailed timing descriptor
            HDMI_EDID_DTD  Dtd;
            
            DtDbgOut(MAX, HDMI, "Data Descriptor %i: Detailed Timing Descriptor", i);
            DtHdmiEdidDecodeDetailedTimingDesciptor(pDescr, &Dtd);
            
            // For interlaced formats Field1 properties are in the first DTD,
            // Field2 properties are in the second DTD
            if ((Dtd.m_Flags & DT_HDMI_FLAGS_INTERLACED) == 0)
                Field2 = FALSE;

            Supported = DtHdmiGetSupportedFormatsFromDetailedTimeDescriptor(&Dtd, Field2);

            if ((Dtd.m_Flags & DT_HDMI_FLAGS_INTERLACED) == 0)
            {
                pHdmiTx->m_MonSupportedFormats |= Supported;
                if (Supported != 0)
                    DtDbgOut(MAX, HDMI, "[%i] DTD: Supported: %llxh (+ %llxh)", i, 
                                               pHdmiTx->m_MonSupportedFormats, Supported);
                else
                    DtDbgOut(MAX, HDMI, "[%i] DTD: Not supported", i);
            }
            else
            {
                if (!Field2)
                {
                    Field2 = TRUE;
                    SupportedField1 = Supported;
                } else 
                {
                    pHdmiTx->m_MonSupportedFormats |= (SupportedField1 & Supported);
                    if ((SupportedField1 & Supported) != 0)
                        DtDbgOut(MAX, HDMI, "[%i] DTD: Supported: %llxh (+ %llxh)", 
                                                        i, pHdmiTx->m_MonSupportedFormats,
                                                        (SupportedField1 & Supported));
                    Field2 = FALSE;
                }
            }
        }
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidParseEedid -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtHdmiEdidParseEedid(DtaHdmiTx* pHdmiTx, UInt8* pBlock0)
{
    Int  NumberOfExtensions = 0;

    DtHdmiResetFeatures(pHdmiTx);

    // Checksum check
    if (!DtHdmiEdidChecksumOk(pBlock0))
    {
        pHdmiTx->m_EdidError = TRUE;
        DtDbgOut(ERR, HDMI, "EEDID Checksum error");
        return -1;
    }

    // Header check
    if (pBlock0[0]!=0x00 && pBlock0[1]!=0xff && pBlock0[2]!=0xff && pBlock0[3]!=0xff &&
             pBlock0[4]!=0xff && pBlock0[5]!=0xff && pBlock0[6]!=0xff && pBlock0[7]!=0x00)
        DtDbgOut(ERR, HDMI, "HEADER BLOCK0 error");
    
    // Structure version check
    if (pBlock0[0x12] != 1)
        DtDbgOut(ERR, HDMI, "EEDID version number:%i. Expected:1", pBlock0[0x12]);
    DtDbgOut(MAX, HDMI, "EEDID structure version:%i revision:%i", pBlock0[0x12], 
                                                                           pBlock0[0x13]);
    
    // Aspect Ratio// 0x15, 0x16
    
    // Feature Support 0x18
    DtHdmiEdidDecodeFeatureSupport(pBlock0[0x18]);
    // 0x19..0x22: Color Characteristics
    // 0x23..0x25: Established Timings I && II
    DtHdmiEdidDecodeEstablishedTimings(&pBlock0[0x23]);
    // 0x26..0x35: Standard Timings
    DtHdmiEdidDecodeStandardTimings(&pBlock0[0x26], 8);
    //0x36..0x7D: 4xData descriptors (detailed timing data or other data)
    DtHdmiEdidDecodeDataDescriptors(pHdmiTx, &pBlock0[0x36], 4);

    // Number of extensions
    NumberOfExtensions = pBlock0[0x7E];
    /* Some devices have been incorrectly designed so that the block map is not counted in the
        extension count. Design of compliant devices should take compatibility with those non-compliant
        devices into consideration. For example, when a source finds an extension count of 2, it may
        attempt to read 3 extensions on the chance that the sink has incorrectly set its count, or it may
        use the information in the block map as a more accurate guide.
    */
    
    DtDbgOut(MAX, HDMI, "Number of extensions:%i", NumberOfExtensions);
    
    return NumberOfExtensions;
}

//-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidParseCeaShortAudioDescriptors -.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtHdmiEdidParseCeaShortAudioDescriptors(UInt8* pSAD, UInt Size)
{
    Int  i;
    for (i=0; i<(Int)Size/3; i++)
    {
        Int  NumChannels = (pSAD[i*3+0]&0x7) + 1;
    
        switch ((pSAD[i*3+0]>>3) & 0x0f)
        {
        case 1: DtDbgOut(MAX, HDMI, "%i: Audio format='Linear Pulse Code Modulation' #CH:%i", i, NumChannels);break;
        case 2: DtDbgOut(MAX, HDMI, "%i: Audio format='AC-3' #CH:%i", i, NumChannels);break;
        case 3: DtDbgOut(MAX, HDMI, "%i: Audio format='MPEG1 (Layers 1 and 2)' #CH:%i", i, NumChannels);break;
        case 4: DtDbgOut(MAX, HDMI, "%i: Audio format='MP3' #CH:%i", i, NumChannels);break;
        case 5: DtDbgOut(MAX, HDMI, "%i: Audio format='MPEG2' #CH:%i", i, NumChannels);break;
        case 6: DtDbgOut(MAX, HDMI, "%i: Audio format='AAC' #CH:%i", i, NumChannels);break;
        case 7: DtDbgOut(MAX, HDMI, "%i: Audio format='DTS' #CH:%i", i, NumChannels);break;
        case 8: DtDbgOut(MAX, HDMI, "%i: Audio format='ATRAC' #CH:%i", i, NumChannels);break;
        case 9: DtDbgOut(MAX, HDMI, "%i: Audio format='One-bit audio aka SACD' #CH:%i", i, NumChannels);break;
        case 10: DtDbgOut(MAX, HDMI, "%i: Audio format='DD+' #CH:%i", i, NumChannels);break;
        case 11: DtDbgOut(MAX, HDMI, "%i: Audio format='DTS-HD' #CH:%i", i, NumChannels);break;
        case 12: DtDbgOut(MAX, HDMI, "%i: Audio format='MLP/Dolby TrueHD' #CH:%i", i, NumChannels);break;
        case 13: DtDbgOut(MAX, HDMI, "%i: Audio format='DTS Audio' #CH:%i", i, NumChannels);break;
        case 14: DtDbgOut(MAX, HDMI, "%i: Audio format='Microsoft WMA Pro' #CH:%i", i, NumChannels);break;
        default:
             DtDbgOut(MAX, HDMI, "%i: Audio format=RESERVED #CH:%i", i, NumChannels);break;
        }
        DtDbgOut(MAX, HDMI, "%i: Res=%i 192kHz=%i 176khz=%i 96kHz=%i 88kHz=%i 48kHz=%i"
                                " 44kHz=%i 32kHz=%i", i, 
                                ((pSAD[i*3+1]&0x80)!=0?1:0), ((pSAD[i*3+1]&0x40)!=0?1:0), 
                                ((pSAD[i*3+1]&0x20)!=0?1:0), ((pSAD[i*3+1]&0x10)!=0?1:0),
                                ((pSAD[i*3+1]&0x08)!=0?1:0), ((pSAD[i*3+1]&0x04)!=0?1:0), 
                                ((pSAD[i*3+1]&0x02)!=0?1:0), ((pSAD[i*3+1]&0x01)!=0?1:0));
        if (((pSAD[i*3+0]>>3) & 0x0f) == 1)
        {
            // LPCM
            DtDbgOut(MAX, HDMI, "%i: Audio bit depth: Res=%xh 24-bit=%i 20-bit=%i" 
                                " 16-bit=%i", i, pSAD[i*3+2]>>3, (pSAD[i*3+2]&0x4)!=0?1:0,
                                (pSAD[i*3+2]&0x2)!=0?1:0,(pSAD[i*3+2]&0x1)!=0?1:0);
        } else
        {
            DtDbgOut(MAX, HDMI, "%i: Audio max. supported bitrate: %i kbit/s", i, 
                                                                    (Int)pSAD[i*3+2] * 8);
        }
    }
}

//-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidParseCeaShortVideoDescriptors -.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtHdmiEdidParseCeaShortVideoDescriptors(DtaHdmiTx* pHdmiTx, UInt8* pSVD, UInt Size)
{
    Int  i;
    UInt8  VideoFormat;
    UInt64  Supported;
    for (i=0; i<(Int)Size; i++)
    {
        VideoFormat = pSVD[i]&0x7f;
        if (VideoFormat>107 || VideoFormat==0)
            DtDbgOut(MAX, HDMI, "[%i]: SVD %i RESERVED", i, VideoFormat);
        else 
        {
            Supported = DtHdmiGetSupportedFormatsFromShortVideoDescriptor(VideoFormat);
            if (Supported != 0)
            {
                pHdmiTx->m_MonSupportedFormats |= Supported;
                DtDbgOut(MAX, HDMI, "[%i] SVD %i Supported: %llxh (+ %llxh)", i, 
                                  VideoFormat, pHdmiTx->m_MonSupportedFormats, Supported);
            } else
                DtDbgOut(MAX, HDMI, "[%i] SVD %i not supported by DekTec", i, 
                                                                             VideoFormat);
        }
    }
}

//-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidParseCeaExtendedTagDescriptors -.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtHdmiEdidParseCeaExtendedTagDescriptors(UInt8* pCeaExtendedTag, UInt Size)
{
    if (pCeaExtendedTag[0] == 0x0)
        DtDbgOut(MAX, HDMI, "Video Capability Data Block");
    else if (pCeaExtendedTag[0] == 0x1)
        DtDbgOut(MAX, HDMI, "Vendor-Specific Video Data Block");
    else if (pCeaExtendedTag[0] == 0x2)
        DtDbgOut(MAX, HDMI, "Reserved for VESA Video Display Device Information Data"
                                                                                " Block");
    else if (pCeaExtendedTag[0] == 0x3)
        DtDbgOut(MAX, HDMI, "Reserved for VESA Video Data Block");
    else if (pCeaExtendedTag[0] == 0x4)
        DtDbgOut(MAX, HDMI, "Reserved for HDMI Video Data Block");
    else if (pCeaExtendedTag[0] == 0x5)
        DtDbgOut(MAX, HDMI, "Colorimetry Data Block");
    else if (pCeaExtendedTag[0] == 0x10)
        DtDbgOut(MAX, HDMI, "CEA Miscellaneous Audio Fields");
    else if (pCeaExtendedTag[0] == 0x11)
        DtDbgOut(MAX, HDMI, "Vendor-Specific Audio Data Block");
    else if (pCeaExtendedTag[0] == 0x12)
        DtDbgOut(MAX, HDMI, "Reserved for HDMI Audio Data Block");
    else 
        DtDbgOut(MAX, HDMI, "Reserved data block type %xh", pCeaExtendedTag[0]);
    return TRUE;
}

//.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidParseCeaVendorSpecificDataBlock -.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtHdmiEdidParseCeaVendorSpecificDataBlock(DtaHdmiTx* pHdmiTx, 
                                                     UInt8* pCeaVendorSpecific, UInt Size)
{
    UInt32  IEEE_Identifier = pCeaVendorSpecific[0] | ((UInt32)pCeaVendorSpecific[1] << 8)
                                                  | ((UInt32)pCeaVendorSpecific[2] << 16);
    if (IEEE_Identifier == HDMI_IEEE_LLC_IDENTIFIER)
    {
        DtDbgOut(MAX, HDMI, "Found HDMI-LLC Vendor-Specific Data Block (HDMI VSDB)");
        if (Size < 5)
            DtDbgOut(MAX, HDMI, "Illegal length: %i. Length must be >= 5", Size);
        else {
            pHdmiTx->m_SupportHdmi = TRUE;
            DtDbgOut(MAX, HDMI, "  Source Physical Address: %i.%i.%i.%i", 
                pCeaVendorSpecific[3]>>4,pCeaVendorSpecific[3]&0xf, 
                pCeaVendorSpecific[4]>>4,pCeaVendorSpecific[4]&0xf);
            if (Size > 5)
            {
                DtDbgOut(MAX, HDMI, "  SupportAI:%i, DC48bit:%i, DC36bit:%i, DC30bit:%i, "
                                "DCY444:%i, Rsvd1:%i, Rsvd2:%i, DVIDual:%i", 
                (pCeaVendorSpecific[5]&0x80)!=0?1:0, (pCeaVendorSpecific[5]&0x40)!=0?1:0,
                (pCeaVendorSpecific[5]&0x20)!=0?1:0, (pCeaVendorSpecific[5]&0x10)!=0?1:0,
                (pCeaVendorSpecific[5]&0x08)!=0?1:0, (pCeaVendorSpecific[5]&0x04)!=0?1:0,
                (pCeaVendorSpecific[5]&0x02)!=0?1:0, (pCeaVendorSpecific[5]&0x01)!=0?1:0);
            }
            if (Size > 6)
                DtDbgOut(MAX, HDMI, "  Max TMDS Clock: %iMHz", 
                                                         (UInt)pCeaVendorSpecific[6] * 5);
            if (Size > 7)
            {
                UInt8  Offset = 8;
                // We have additional fields
                if ((pCeaVendorSpecific[7] & 0x01) != 0)
                    DtDbgOut(MAX, HDMI, "  Graphics processing support");
                if ((pCeaVendorSpecific[7] & 0x02) != 0)
                    DtDbgOut(MAX, HDMI, "  Photo processing support");
                if ((pCeaVendorSpecific[7] & 0x04) != 0)
                    DtDbgOut(MAX, HDMI, "  Cinema processing support");
                if ((pCeaVendorSpecific[7] & 0x08) != 0)
                    DtDbgOut(MAX, HDMI, "  Game processing support");
                if ((pCeaVendorSpecific[7] & 0x80) != 0)
                {
                    // Audio/Video latency present
                    DtDbgOut(MAX, HDMI, "  Video latency: %i,  Audio latency: %i", 
                                pCeaVendorSpecific[Offset], pCeaVendorSpecific[Offset+1]);
                    Offset+=2;
                }
                if ((pCeaVendorSpecific[7] & 0x40) != 0)
                {
                    // Interlaced Audio/Video latency present
                    DtDbgOut(MAX, HDMI, "  Interlaced Video latency: %i,"
                                " Interlaced Audio latency: %i", 
                                pCeaVendorSpecific[Offset], pCeaVendorSpecific[Offset+1]);
                    Offset+=2;
                }
                if ((pCeaVendorSpecific[7] & 0x20) != 0)
                {
                    UInt  HdmiVicLen; 
                    UInt  Hdmi3DLen;
                    UInt  i;
                    // Additional video format capabilities are present
                    DtDbgOut(MAX, HDMI, "  3DSupport: %i, Reserved: %xh", 
                                                 (pCeaVendorSpecific[Offset]&0x80)!=0?1:0,
                                                 pCeaVendorSpecific[Offset]&0x7f);
                    Offset++;
                    HdmiVicLen = pCeaVendorSpecific[Offset] >> 5;
                    Hdmi3DLen = pCeaVendorSpecific[Offset] & 0x1f;
                    DtDbgOut(MAX, HDMI, "  HdmiVicLen: %i, Hdmi3DLen: %i", 
                                                                   HdmiVicLen, Hdmi3DLen);
                    Offset++;

                    // Parse HDMI VIC codes
                    for (i=0; i<HdmiVicLen && (Offset+i)<Size; i++)
                    {
                        DtaHdmiVidStd  VidStd = DtHdmiHdmiVic2VidStd(pCeaVendorSpecific[Offset]);
                        Int  Index;
                        if (VidStd.m_VidStd != DT_VIDSTD_UNKNOWN)
                        {
                            Index = DtHdmiTxGetIndexVidStd(VidStd);
                            if (Index != -1)
                            {
                                pHdmiTx->m_MonSupportedFormats |= ((UInt64)1 << Index);
                                DtDbgOut(MAX, HDMI, "  HdmiVic: %i,"
                                   " VidStd: %i(%s), Supported: %llxh (+ %llxh)", 
                                   pCeaVendorSpecific[Offset], VidStd.m_VidStd, 
                                   VidStdName(VidStd.m_VidStd),
                                   pHdmiTx->m_MonSupportedFormats, ((UInt64)1 << Index));
                            }
                        } else
                            DtDbgOut(MAX, HDMI, "  HdmiVic: %i not supported by Dektec",
                                                              pCeaVendorSpecific[Offset]);
                        Offset++;
                    }
                }
            }
        }
    } else if (IEEE_Identifier == HDMI_IEEE_FORUM_IDENTIFIER)
    {
        DtDbgOut(MAX, HDMI, "Found HDMI Forum Vendor-Specific Data Block (HF VSDB)"
                                                   " version: %i", pCeaVendorSpecific[3]);
        if (Size < 7)
            DtDbgOut(MAX, HDMI, "Illegal length: %i. Length must be >= 7", Size);
        else {
            if (pCeaVendorSpecific[4] == 0)
                DtDbgOut(MAX, HDMI, "Sink does not support TMDS > 340 Mcsc");
            else
                DtDbgOut(MAX, HDMI, "Max. TMDS character rate: %i MHz", 
                                                            (Int)pCeaVendorSpecific[4]*5);
            pHdmiTx->m_SupportScDc = (pCeaVendorSpecific[5] & 0x80) != 0;
            DtDbgOut(MAX, HDMI, "SCDC:%i RR:%i LTE_340Mcsc_scr:%i Indep_view:%i"
                            " Dual_view:%i 3d_OSD_Dis[:%i DC_48bit_420:%i DC_36bit_420:%i"
                            " DC_30bit_420:%i",
                            ((pCeaVendorSpecific[5] & 0x80) == 0)?0:1, 
                            ((pCeaVendorSpecific[5] & 0x40) == 0)?0:1, 
                            ((pCeaVendorSpecific[5] & 0x08) == 0)?0:1,
                            ((pCeaVendorSpecific[5] & 0x04) == 0)?0:1,
                            ((pCeaVendorSpecific[5] & 0x02) == 0)?0:1,
                            ((pCeaVendorSpecific[5] & 0x01) == 0)?0:1,
                            ((pCeaVendorSpecific[6] & 0x04) == 0)?0:1,
                            ((pCeaVendorSpecific[6] & 0x02) == 0)?0:1,
                            ((pCeaVendorSpecific[6] & 0x01) == 0)?0:1);
        }

    } else
        DtDbgOut(MAX, HDMI, "Unknown Vendor-Specific Data Block (%02x%02x%02x)",
                     pCeaVendorSpecific[0], pCeaVendorSpecific[1], pCeaVendorSpecific[2]);
    return TRUE;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidParseCeaDataBlockCollection -.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtHdmiEdidParseCeaDataBlockCollection(DtaHdmiTx* pHdmiTx, UInt8* pCeaBlock, UInt Size)
{
    Int  BlockLength;
    Int  BlockType;
    Int  Offset = 0;
    while (Offset < (Int)Size)
    {
        BlockType = pCeaBlock[Offset] >> 5;
        BlockLength = pCeaBlock[Offset] & 0x1f;
        if (BlockType == 1) // Audio
            DtHdmiEdidParseCeaShortAudioDescriptors(&pCeaBlock[Offset+1], BlockLength);
        else if (BlockType == 2) // Video
            DtHdmiEdidParseCeaShortVideoDescriptors(pHdmiTx, &pCeaBlock[Offset+1], 
                                                                             BlockLength);
        else if (BlockType == 3) // Vendor Specific
        {
            DtDbgOut(MAX, HDMI, "Vendor specific block detected at offset: %i", Offset);
            DtHdmiEdidParseCeaVendorSpecificDataBlock(pHdmiTx, &pCeaBlock[Offset+1], 
                                                                             BlockLength);
        }
        else if (BlockType == 4) // Speaker allocation
            DtDbgOut(MAX, HDMI, "Speaker allocation block detected at offset: %i", 
                                                                                  Offset);
        else if (BlockType == 5) // VESA DTC
            DtDbgOut(MAX, HDMI, "VESA DTC block detected at offset: %i", Offset);
        else if (BlockType == 7) // Extended tag
        {
            DtDbgOut(MAX, HDMI, "Extended tag detected at offset: %i", Offset);
            DtHdmiEdidParseCeaExtendedTagDescriptors(&pCeaBlock[Offset+1], BlockLength);
        } else // reserved
            DtDbgOut(MAX, HDMI, "Reserved block type %xh detected at offset: %i", 
                                                                       BlockType, Offset);
        Offset += BlockLength + 1;
    }
    if (Offset != Size)
        DtDbgOut(MAX, HDMI, "Offset(%i) != Size(%i)", Offset, Size);
    return TRUE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidParseCeaExtension -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtHdmiEdidParseCeaExtension(DtaHdmiTx* pHdmiTx, UInt8* pBlockCea)
{
    UInt8  DescriptorsOffset;
    UInt8  NumDataDescriptors;
    DtDbgOut(MAX, HDMI, "CEA extension revision: %i", pBlockCea[1]);
    DescriptorsOffset = pBlockCea[2];
    if (DescriptorsOffset == 0)
    {
        DtDbgOut(MAX, HDMI, "No DTDs and non-DTDs data present.");
        return DT_STATUS_OK;
    }
    if (DescriptorsOffset > 4)
        DtDbgOut(MAX, HDMI, "Non-DTD data present.");
    if (DescriptorsOffset < 4)
    {
        DtDbgOut(ERR, HDMI, "Illegal descriptor offset: %xh", DescriptorsOffset);
        return DT_STATUS_FAIL;
    }
    pHdmiTx->m_SupportYCbCr422 = (pBlockCea[3] & (1<<4)) != 0;
    pHdmiTx->m_SupportYCbCr444 = (pBlockCea[3] & (1<<5)) != 0;
    pHdmiTx->m_SupportBasicAudio = (pBlockCea[3] & (1<<6)) != 0;
    
    DtDbgOut(ERR, HDMI, "YCbCr422:%i YCbCr444:%i BasicAudio:%i", 
                                                        pHdmiTx->m_SupportYCbCr422?1:0,
                                                        pHdmiTx->m_SupportYCbCr444?1:0,
                                                        pHdmiTx->m_SupportBasicAudio?1:0);

    if (DescriptorsOffset > 4)
    {
        DtHdmiEdidParseCeaDataBlockCollection(pHdmiTx, &pBlockCea[4], 
                                                                   DescriptorsOffset - 4);
        NumDataDescriptors = pBlockCea[3] & 0xf;
    } else // TODO: Check empty spaces and remove from NumDataDescriptors
        NumDataDescriptors = (128 - DescriptorsOffset) / 18;
    DtHdmiEdidDecodeDataDescriptors(pHdmiTx, &pBlockCea[DescriptorsOffset], 
                                                                      NumDataDescriptors);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiEdidParseExtension -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtHdmiEdidParseExtension(DtaHdmiTx* pHdmiTx, UInt8* pBlockX)
{
    // Check checksum
    if (!DtHdmiEdidChecksumOk(pBlockX))
    {
        pHdmiTx->m_EdidError = TRUE;
        DtDbgOut(ERR, HDMI, "Extension Checksum error");
        return DT_STATUS_FAIL;
    }

    // We only support the CEA extension
    if (pBlockX[0] == EDID_EXTENSION_BLOCK_TAG_CEA)
        return DtHdmiEdidParseCeaExtension(pHdmiTx, pBlockX);
    DtDbgOut(MAX, HDMI, "Extension %xh not supported", pBlockX[0]);
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtHdmiTxInit(DtaNonIpPort* pNonIpPort)
{
    DtaHdmiTx* pHdmiTx = &pNonIpPort->m_HdmiTx;
    DtStatus  Status;
    DtPropertyData*  pPropData = &pNonIpPort->m_pDvcData->m_PropData;
    pHdmiTx->m_pDvcData = pNonIpPort->m_pDvcData;
    pHdmiTx->m_PortIndex = pNonIpPort->m_PortIndex;
    pHdmiTx->m_SelVidStd.m_VidStd = DT_VIDSTD_UNKNOWN;
    pHdmiTx->m_SelVidStd.m_AspectRatio = DTA_AR_UNKNOWN;
    pHdmiTx->m_UsedVidStd.m_VidStd = DT_VIDSTD_UNKNOWN;
    pHdmiTx->m_UsedVidStd.m_AspectRatio = DTA_AR_UNKNOWN;
    pHdmiTx->m_SelAudStd = DT_AUDSTD_UNKNOWN;
    pHdmiTx->m_UsingTestPicture = FALSE;
    pHdmiTx->m_SelVidMode = DT_HDMI_VIDMOD_YCBCR_422;
    pHdmiTx->m_UsedVidMode = DT_HDMI_VIDMOD_UNKNOWN;
    pHdmiTx->m_MonDetected = FALSE;
    pHdmiTx->m_DisableHdmiOutput = FALSE;
        
    pHdmiTx->m_HdmiRegsOffset = DtPropertiesGetUInt16(pPropData, "REGISTERS_HDMITX",
                                                                 pNonIpPort->m_PortIndex);
    
    DtMutexInit(&pHdmiTx->m_StateLock);

    // Initialize hotplug interrupt DPC
    Status = DtDpcInit(&pHdmiTx->m_HotplugDpc, DtHdmiTxHotplugDpc, TRUE);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtWorkItemInit(&pHdmiTx->m_HotplugWorkItem, DtHdmiTxHotplugWorkItem, TRUE,
                                                          &pHdmiTx->m_pDvcData->m_Device);
    if (!DT_SUCCESS(Status))
        return Status;

    DtHdmiResetFeatures(pHdmiTx);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxInitPowerUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtHdmiTxInitPowerUp(DtaNonIpPort* pNonIpPort)
{
    DtaHdmiTx* pHdmiTx = &pNonIpPort->m_HdmiTx;
    pHdmiTx->m_pHdmiRegs = pHdmiTx->m_pDvcData->m_pGenRegs + pHdmiTx->m_HdmiRegsOffset;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxEnableHdmiOutputDriver -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtHdmiTxEnableHdmiOutputDriver(DtaHdmiTx* pHdmiTx, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    if (pHdmiTx->m_pDvcData->m_DevInfo.m_HardwareRevision < 200)
        DtaRegControl1SetHwEnable(pHdmiTx->m_pDvcData->m_pGenRegs, (Enable?1:0));
    else
    {
        // Power up/down SN65DP159
        UInt8  I2cData[2];
        I2cData[0] = 0x9;
        if (Enable)
            I2cData[1] = 0x6;   // Power up: default 100kbps I2C communication, 
                                // no automatic powerdown during hotplug event
        else 
            I2cData[1] = 0xa;   // Power down: default 100kbps I2C communication

        Status = DtaI2cWrite(pHdmiTx->m_pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR,
                                                 SN65DP159_I2C_ADDR, 2, (UInt8*)&I2cData);
        if (!DT_SUCCESS(Status))
        {
            // Error writing SN65DP159_I2C_ADDR
            DtDbgOut(ERR, HDMI, "Error writing power state to SN65DP159 device. "
                                "Address offset: 0x9");
            return Status;
        }
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxInitPowerUpPost -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtHdmiTxInitPowerUpPost(DtaNonIpPort* pNonIpPort)
{
    DtaHdmiTx* pHdmiTx = &pNonIpPort->m_HdmiTx;
    Int  i;
    DtStatus  Status = DT_STATUS_OK;
    DtWorkItemArgs  DtWorkItemArgs;

    // Set the EDDC enable bit needed for HDMI EDID communication
    DtaRegI2cCtrlSetEddcEn(pNonIpPort->m_I2c.m_pI2cRegs, 1);

    // Default no monitor detected. Can be changed in HotplugWorkItem
    pHdmiTx->m_MonDetected = FALSE;

    // Initialise text bitmap
    for (i=0; i<sizeof(textData); i++)
        DtaRegHdmiTxTextSet(pHdmiTx->m_pHdmiRegs, i, textData[i]);

    // Init output driver
    Status = DtHdmiTxInitHdmiOutputDriver(pHdmiTx);
    if (!DT_SUCCESS(Status))
        return Status;

    DtWorkItemArgs.m_pContext = pHdmiTx;
    DtWorkItemArgs.m_Data1.m_UInt32_1 = 0;
    Status = DtWorkItemSchedule(&pHdmiTx->m_HotplugWorkItem, &DtWorkItemArgs);
    // If already running by ISR, skip this one
    if (Status == DT_STATUS_IN_USE)
        Status = DT_STATUS_OK;
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxUpdateVideoStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Used on the external interface
//
DtStatus DtHdmiTxUpdateVidStd(DtaNonIpPort*  pNonIpPort, DtaHdmiVidStd VidStd, Bool Force)
{
    DtStatus Status;
    DtaHdmiTx* pHdmiTx = &pNonIpPort->m_HdmiTx;
    DtMutexAcquire(&pHdmiTx->m_StateLock, -1);
    Status =  DtHdmiTxUpdateVideoStd(pHdmiTx, VidStd, Force);
    DtMutexRelease(&pHdmiTx->m_StateLock);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxInterruptDisable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtHdmiTxInterruptDisable(DtaNonIpPort* pNonIpPort)
{
    DtaHdmiTx* pHdmiTx = &pNonIpPort->m_HdmiTx;

    // Hotplug detect interrupts
    DtaRegHdmiTxGenCtrlSetSinkConIntEnable(pHdmiTx->m_pHdmiRegs, 0);
    DtaRegHdmiTxGenCtrlSetSinkDisIntEnable(pHdmiTx->m_pHdmiRegs, 0);

    // Underflow interrupt
    DtaRegHdmiTxGenCtrlSetUnderflowIntEnable(pHdmiTx->m_pHdmiRegs, 0);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxInterruptEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtHdmiTxInterruptEnable(DtaNonIpPort* pNonIpPort)
{
    DtaHdmiTx* pHdmiTx = &pNonIpPort->m_HdmiTx;

    // Hotplug detect interrupts
    DtaRegHdmiTxGenCtrlSetSinkConIntEnable(pHdmiTx->m_pHdmiRegs, 1);
    DtaRegHdmiTxGenCtrlSetSinkDisIntEnable(pHdmiTx->m_pHdmiRegs, 1);

    // Underflow interrupt
    DtaRegHdmiTxGenCtrlSetUnderflowIntEnable(pHdmiTx->m_pHdmiRegs, 1);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxPowerDown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtHdmiTxPowerDown(DtaNonIpPort* pNonIpPort)
{
    DtaHdmiTx* pHdmiTx = &pNonIpPort->m_HdmiTx;

    // Wait for DPC's and workitems to be completed
    DtDpcWaitForCompletion(&pHdmiTx->m_HotplugDpc);
    DtWorkItemWaitForCompletion(&pHdmiTx->m_HotplugWorkItem);

    // Disable HDMI
    DtaRegHdmiTxGenCtrlSetEnable(pHdmiTx->m_pHdmiRegs, 0);
    DtDbgOut(ERR, HDMI, "Disable HDMI");
    
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxInterrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtHdmiTxInterrupt(DtaNonIpPort*  pNonIpPort)
{
    DtaHdmiTx*  pHdmiTx = &pNonIpPort->m_HdmiTx;
    Bool  IrqHandled = FALSE;

    if (DtaRegHdmiTxGenStatGetSinkConInt(pHdmiTx->m_pHdmiRegs) ||
                                   DtaRegHdmiTxGenStatGetSinkDisInt(pHdmiTx->m_pHdmiRegs))
    {
        // HDMI connect/disconnect interrupt.
        // Fire DPC
        DtDpcArgs  DpcArgs;
        UInt32  Status1 = DtaRegHdmiTxGenStatGet(pHdmiTx->m_pHdmiRegs);
        UInt32  Status2;
        DtaRegHdmiTxGenStatClrSinkConInt(pHdmiTx->m_pHdmiRegs);
        DtaRegHdmiTxGenStatClrSinkDisInt(pHdmiTx->m_pHdmiRegs);
        Status2 = DtaRegHdmiTxGenStatGet(pHdmiTx->m_pHdmiRegs);
        DtDbgOut(ERR, HDMI, "Status1:%08xh Status2:%08xh HOTPLUG", Status1, Status2);
        DpcArgs.m_pContext = pHdmiTx;
        DpcArgs.m_Data1.m_UInt32_1 = 0;
        DtDpcSchedule(&pHdmiTx->m_HotplugDpc, &DpcArgs);
        IrqHandled = TRUE;
    }
    if (DtaRegHdmiTxGenStatGetUnderflowInt(pHdmiTx->m_pHdmiRegs))
    {
        DtDpcArgs  DpcArgs;
        UInt32  Status1 = DtaRegHdmiTxGenStatGet(pHdmiTx->m_pHdmiRegs);
        UInt32  Status2;
        DtaRegHdmiTxGenStatClrUnderflowInt(pHdmiTx->m_pHdmiRegs);
        Status2 = DtaRegHdmiTxGenStatGet(pHdmiTx->m_pHdmiRegs);
        DtDbgOut(ERR, HDMI, "Status1:%08xh Status2:%08xh UNDERFLOW", Status1, Status2);
        DpcArgs.m_pContext = pHdmiTx;
        DpcArgs.m_Data1.m_UInt32_1 = 1;
        DtDpcSchedule(&pHdmiTx->m_HotplugDpc, &DpcArgs);
        IrqHandled = TRUE;
    }
    return IrqHandled;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtHdmiTxIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtHdmiTxIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile, DtIoctlObject* pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    Int  NonIpPortIndex;        // Index in the non ip port struct

    DtaIoctlHdmiTxCmdInput* pHdmiCmdInput = (DtaIoctlHdmiTxCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlHdmiTxCmdOutput* pHdmiCmdOutput =(DtaIoctlHdmiTxCmdOutput*)pIoctl->m_pOutputBuffer;
    DtaNonIpPort*  pNonIpPort;
    DtaHdmiTx*  pHdmiTx;

    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlHdmiTxCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlHdmiTxCmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    if (!DT_SUCCESS(DtaGetNonIpPortIndex(pDvcData, pHdmiCmdInput->m_PortIndex,
                                                                        &NonIpPortIndex)))
        return DT_STATUS_INVALID_PARAMETER;
    
    // Get the NonIpPort
    pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpPortIndex];
    if (!pNonIpPort->m_CapHdmiTx)
        return DT_STATUS_NOT_SUPPORTED;

    pHdmiTx = &pNonIpPort->m_HdmiTx;

    // Determine final required output/input sizes
    switch (pHdmiCmdInput->m_Cmd)
    {
    case DTA_HDMI_TX_CMD_GET_HDMI_STATUS:
        pCmdStr = "DTA_HDMI_TX_CMD_GET_HDMI_STATUS";
        InReqSize = 0;
        OutReqSize += sizeof(DtaIoctlHdmiTxCmdGetHdmiStatusOutput);
        break;
    case DTA_HDMI_TX_CMD_GET_VIDSTD:
        pCmdStr = "DTA_HDMI_TX_CMD_GET_VIDSTD";
        InReqSize += sizeof(DtaIoctlHdmiTxCmdGetVidStdInput);
        OutReqSize += sizeof(DtaIoctlHdmiTxCmdGetVidStdOutput);
        break;

    case DTA_HDMI_TX_CMD_SET_VIDMOD:
        pCmdStr = "DTA_HDMI_TX_CMD_SET_VIDMOD";
        InReqSize += sizeof(DtaIoctlHdmiTxCmdSetVidModInput);
        OutReqSize = 0;
        break;
    case DTA_HDMI_TX_CMD_FORCE_TEST_PICTURE:
        pCmdStr = "DTA_HDMI_TX_CMD_FORCE_TEST_PICTURE";
        InReqSize += sizeof(DtaIoctlHdmiTxCmdForceTestPictureInput);
        OutReqSize = 0;
        break;
    case DTA_HDMI_TX_CMD_FORCE_MONITOR_DETECTED:
        pCmdStr = "DTA_HDMI_TX_CMD_FORCE_MONITOR_DETECTED";
        InReqSize += sizeof(DtaIoctlHdmiTxCmdForceMonitorDetectedInput);
        OutReqSize = 0;
        break;
    case DTA_HDMI_TX_CMD_DISABLE_EDID_CHECK:
        pCmdStr = "DTA_HDMI_TX_CMD_DISABLE_EDID_CHECK";
        InReqSize += sizeof(DtaIoctlHdmiTxCmdDisableEdidCheckInput);
        OutReqSize = 0;
        break;
    case DTA_HDMI_TX_CMD_DISABLE_OUTPUT:
        pCmdStr = "DTA_HDMI_TX_CMD_DISABLE_OUTPUT";
        InReqSize += sizeof(DtaIoctlHdmiTxCmdDisableOutputInput);
        OutReqSize = 0;
        break;
    default:
        pCmdStr = "??UNKNOWN HDMI_CMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, HDMI, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, HDMI, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, HDMI, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
            pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        // Execute cmd
        switch (pHdmiCmdInput->m_Cmd)
        {
        case DTA_HDMI_TX_CMD_GET_HDMI_STATUS:
            DtMutexAcquire(&pHdmiTx->m_StateLock, -1);
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_MonDetected = pHdmiTx->m_MonDetected;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_EdidError = pHdmiTx->m_EdidError;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_SupportHdmi = pHdmiTx->m_SupportHdmi;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_SupportYCbCr444 = 
                                                               pHdmiTx->m_SupportYCbCr444;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_SupportYCbCr422 = 
                                                               pHdmiTx->m_SupportYCbCr422;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_SupportBasicAudio =
                                                             pHdmiTx->m_SupportBasicAudio;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_SupportScDc = pHdmiTx->m_SupportScDc;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_SupportMonitorRangeLimits =
                                                     pHdmiTx->m_SupportMonitorRangeLimits;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_ForceTestPicture =
                                                              pHdmiTx->m_ForceTestPicture;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_UsingTestPicture = 
                                                              pHdmiTx->m_UsingTestPicture;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_ForceMonitorDetected =
                                                          pHdmiTx->m_ForceMonitorDetected;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_DisableEdidCheck = 
                                                              pHdmiTx->m_DisableEdidCheck;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_DisableHdmiOutput = 
                                                             pHdmiTx->m_DisableHdmiOutput;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_UsedVidStd = 
                                                           pHdmiTx->m_UsedVidStd.m_VidStd;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_UsedAspectRatio =
                                                      pHdmiTx->m_UsedVidStd.m_AspectRatio;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_SelectedVidStd =
                                                            pHdmiTx->m_SelVidStd.m_VidStd;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_SelectedAspectRatio =
                                                       pHdmiTx->m_SelVidStd.m_AspectRatio;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_UsedVidMod = pHdmiTx->m_UsedVidMode;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_SelectedVidMod= pHdmiTx->m_SelVidMode;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_MaxPixelClk = pHdmiTx->m_MaxPixelClk;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_MinVRate = pHdmiTx->m_MinVRate;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_MaxVRate = pHdmiTx->m_MaxVRate;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_MinHRate = pHdmiTx->m_MinHRate;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_MaxHRate = pHdmiTx->m_MaxHRate;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_MonSupportedFormats =
                                                           pHdmiTx->m_MonSupportedFormats;
            pHdmiCmdOutput->m_Data.m_GetHdmiStatus.m_MonSupportedAudio = 
                                                             pHdmiTx->m_MonSupportedAudio;
            DtMutexRelease(&pHdmiTx->m_StateLock);
            break;

        case DTA_HDMI_TX_CMD_GET_VIDSTD:
            if (pHdmiCmdInput->m_Data.m_GetVidStd.m_Index<0 ||
                pHdmiCmdInput->m_Data.m_GetVidStd.m_Index>=
                                                      DT_HDMI_VIDSTD_2_FORMAT_NUM_ENTRIES)
                Status = DT_STATUS_INVALID_PARAMETER;
            else
            {
                Int Idx = pHdmiCmdInput->m_Data.m_GetVidStd.m_Index;
                Int VidStd = DT_HDMI_VIDSTD_2_FORMAT[Idx][HDMI_IDX_VIDSTD];
                Int AspectRatio = ((DT_HDMI_VIDSTD_2_FORMAT[Idx][HDMI_IDX_FLAGS] &
                         DT_HDMI_FLAGS_ASPECT_RATIO_4_3) != 0) ? DTA_AR_4_3 : DTA_AR_16_9;
                pHdmiCmdOutput->m_Data.m_GetVidStd.m_VidStd = VidStd;
                pHdmiCmdOutput->m_Data.m_GetVidStd.m_AspectRatio = AspectRatio;
            }
            break;
        case DTA_HDMI_TX_CMD_SET_VIDMOD:
            switch(pHdmiCmdInput->m_Data.m_SetVidMod.m_VidMod)
            {
            case DT_HDMI_VIDMOD_RGB_444:
            case DT_HDMI_VIDMOD_YCBCR_422:
            case DT_HDMI_VIDMOD_YCBCR_444:
                DtMutexAcquire(&pHdmiTx->m_StateLock, -1);
                if (pHdmiCmdInput->m_Data.m_SetVidMod.m_VidMod != pHdmiTx->m_SelVidMode)
                {
                    Int OldMode = pHdmiTx->m_SelVidMode;
                    // Update video mode
                    pHdmiTx->m_SelVidMode = pHdmiCmdInput->m_Data.m_SetVidMod.m_VidMod;
                    Status = DtHdmiTxUpdateVideoStd(pHdmiTx, pHdmiTx->m_SelVidStd, TRUE);
                    if (!DT_SUCCESS(Status))
                        pHdmiTx->m_SelVidMode = OldMode;
                }
                DtMutexRelease(&pHdmiTx->m_StateLock);
                break;
            
            default: Status = DT_STATUS_INVALID_PARAMETER;
            }
            break;
        case DTA_HDMI_TX_CMD_FORCE_TEST_PICTURE:
                DtMutexAcquire(&pHdmiTx->m_StateLock, -1);
                pHdmiTx->m_ForceTestPicture = 
                                     pHdmiCmdInput->m_Data.m_ForceTestPicture.m_Enable==1;
                if (pHdmiTx->m_ForceTestPicture)
                {
                    Int  i;
                    // Remove text bitmap
                    for (i=0; i<sizeof(textData); i++)
                        DtaRegHdmiTxTextSet(pHdmiTx->m_pHdmiRegs, i, 0xff);
                } else 
                {
                    Int  i;
                    // Initialise text bitmap
                    for (i=0; i<sizeof(textData); i++)
                        DtaRegHdmiTxTextSet(pHdmiTx->m_pHdmiRegs, i, textData[i]);
                }
                Status = DtHdmiTxUpdateVideoStd(pHdmiTx, pHdmiTx->m_SelVidStd, TRUE);
                DtMutexRelease(&pHdmiTx->m_StateLock);
                break;
        case DTA_HDMI_TX_CMD_FORCE_MONITOR_DETECTED:
            {
                DtWorkItemArgs  DtWorkItemArgs;
                pHdmiTx->m_ForceMonitorDetected = 
                                 pHdmiCmdInput->m_Data.m_ForceMonitorDetected.m_Enable==1;
                DtWorkItemArgs.m_pContext = pHdmiTx;
                DtWorkItemArgs.m_Data1.m_UInt32_1 = 1;  // Don't read EDID
                DtWorkItemSchedule(&pHdmiTx->m_HotplugWorkItem, &DtWorkItemArgs);
            }
            break;
        case DTA_HDMI_TX_CMD_DISABLE_EDID_CHECK:
            DtMutexAcquire(&pHdmiTx->m_StateLock, -1);
            pHdmiTx->m_DisableEdidCheck = 
                                    pHdmiCmdInput->m_Data.m_DisableEdidCheck.m_Disable==1;
            Status = DtHdmiTxUpdateVideoStd(pHdmiTx, pHdmiTx->m_SelVidStd, TRUE);
            DtMutexRelease(&pHdmiTx->m_StateLock);
           break;
        case DTA_HDMI_TX_CMD_DISABLE_OUTPUT:
            DtMutexAcquire(&pHdmiTx->m_StateLock, -1);
            pHdmiTx->m_DisableHdmiOutput =
                                   pHdmiCmdInput->m_Data.m_DisableHdmiOutput.m_Disable==1;
            if (pHdmiTx->m_DisableHdmiOutput)
                DtaRegHdmiTxGenCtrlSetEnable(pHdmiTx->m_pHdmiRegs, 0);
            else
                Status = DtHdmiTxUpdateVideoStd(pHdmiTx, pHdmiTx->m_SelVidStd, TRUE);
            DtMutexRelease(&pHdmiTx->m_StateLock);
            break;

        default:
            Status = DT_STATUS_NOT_SUPPORTED;
        }
    }

    // If we failed, no data has te be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED)
            DtDbgOut(MIN, HDMI, "HdmiCmd=0x%x: NOT SUPPORTED", pHdmiCmdInput->m_Cmd);
        else 
            DtDbgOut(MIN, HDMI, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}
