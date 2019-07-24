//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtAudioVideo.c *#*#*#*#*#*#*#* (C) 2014-2016 DekTec
//
// Driver common - Audio Video - Definition of audio/video types/functions
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2014-2016 DekTec Digital Video B.V.
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
#include <DtRegs.h>         // For DT_VIDSTD_XXX defines

static const Int  g_VidStds[] = {
    DT_VIDSTD_525I59_94,
    DT_VIDSTD_625I50,
    DT_VIDSTD_720P23_98,
    DT_VIDSTD_720P24,
    DT_VIDSTD_720P25,
    DT_VIDSTD_720P29_97,
    DT_VIDSTD_720P30,
    DT_VIDSTD_720P50,
    DT_VIDSTD_720P59_94,
    DT_VIDSTD_720P60,
    DT_VIDSTD_1080P23_98,
    DT_VIDSTD_1080P24,
    DT_VIDSTD_1080P25,
    DT_VIDSTD_1080P29_97,
    DT_VIDSTD_1080P30,
    DT_VIDSTD_1080PSF23_98,
    DT_VIDSTD_1080PSF24,
    DT_VIDSTD_1080PSF25,
    DT_VIDSTD_1080PSF29_97,
    DT_VIDSTD_1080PSF30,
    DT_VIDSTD_1080I50,
    DT_VIDSTD_1080I59_94,
    DT_VIDSTD_1080I60,
    DT_VIDSTD_1080P50,
    DT_VIDSTD_1080P59_94,
    DT_VIDSTD_1080P60,
    DT_VIDSTD_1080P50B,
    DT_VIDSTD_1080P59_94B,
    DT_VIDSTD_1080P60B,
    DT_VIDSTD_2160P23_98,
    DT_VIDSTD_2160P24,
    DT_VIDSTD_2160P25,
    DT_VIDSTD_2160P29_97,
    DT_VIDSTD_2160P30,
    DT_VIDSTD_2160P50,
    DT_VIDSTD_2160P59_94,
    DT_VIDSTD_2160P60,
    DT_VIDSTD_2160P50B,
    DT_VIDSTD_2160P59_94B,
    DT_VIDSTD_2160P60B,
    DT_VIDSTD_480P59_94,
    DT_VIDSTD_525P59_94,
    DT_VIDSTD_625P50,
};


static const Int64  Exp12 = 1000LL*1000LL*1000LL*1000LL; 
static const Int64  Exp11 = 100LL*1000LL*1000LL*1000LL; 
static const Int64  Exp10 = 10LL*1000LL*1000LL*1000LL;  
static const Int64  Exp9 = 1000LL*1000*1000; 
static const Int  Exp8 = 100*1000*1000; 
static const Int  Exp7 = 10*1000*1000; 
static const Int  Exp6 = 1000*1000; 
static const Int  Exp5 = 100*1000; 
static const Int  Exp4 = 10*1000; 
static const Int  Exp3 = 1000; 




//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvGetNumVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtAvGetNumVidStd()
{
    return sizeof(g_VidStds)/sizeof(g_VidStds[0]);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvGetFramePropsFromIdx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtAvGetFramePropsFromIdx(Int Idx, DtAvFrameProps*  pProps)
{
    if (Idx<0 || Idx>=DtAvGetNumVidStd())
        return DT_STATUS_INVALID_PARAMETER;
    return DtAvGetFrameProps(g_VidStds[Idx], pProps);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvGetFrameProps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Returns the AV frame properties for the given video standard
//
DtStatus  DtAvGetFrameProps(Int VidStd, DtAvFrameProps*  pProps)
{
    DT_ASSERT(pProps != NULL);

    switch (VidStd)
    {
    case DT_VIDSTD_525I59_94:
        pProps->m_NumLines = 525;
        pProps->m_Fps = 30;
        pProps->m_IsFractional = TRUE;
        pProps->m_IsInterlaced = TRUE;
        pProps->m_IsHd = FALSE;

        pProps->m_Field1Start = 1;
        pProps->m_Field1End = 262;
        pProps->m_Field1ActVidStart = 17;
        pProps->m_Field1ActVidEnd = 260;
        pProps->m_SwitchingLines[0] = 7;

        pProps->m_Field2Start = 263;
        pProps->m_Field2End = 525;
        pProps->m_Field2ActVidStart = 280;
        pProps->m_Field2ActVidEnd = 522;
        pProps->m_SwitchingLines[1] = 270;

        pProps->m_VancNumS = pProps->m_ActVidNumS = 720*2;
        pProps->m_HancNumS = 268;
        pProps->m_SavNumS = 4;
        pProps->m_EavNumS = 4;

        pProps->m_SyncPointPixelOff = 16;   // Sync point @pixel 16
        
        break;

    case DT_VIDSTD_525P59_94:
    case DT_VIDSTD_480P59_94:
        pProps->m_NumLines = 525;
        pProps->m_Fps = 60;
        pProps->m_IsFractional = TRUE;
        pProps->m_IsInterlaced = FALSE;
        pProps->m_IsHd = FALSE;

        pProps->m_Field1Start = 1;
        pProps->m_Field1End = 525;
        pProps->m_Field1ActVidStart = 17;
        pProps->m_Field1ActVidEnd = 496;

        pProps->m_SwitchingLines[0] = 7;
        pProps->m_SwitchingLines[1] = -1;

        pProps->m_Field2Start = 0;
        pProps->m_Field2End = 0;
        pProps->m_Field2ActVidStart = 0;
        pProps->m_Field2ActVidEnd  = 0;

        if (VidStd == DT_VIDSTD_480P59_94)
            pProps->m_VancNumS = pProps->m_ActVidNumS = 640*2;
        else
            pProps->m_VancNumS = pProps->m_ActVidNumS = 720*2;
        pProps->m_HancNumS = 268;
        pProps->m_SavNumS = 4;
        pProps->m_EavNumS = 4;

        pProps->m_SyncPointPixelOff = 0;
        break;

    case DT_VIDSTD_625I50:
        pProps->m_NumLines = 625;
        pProps->m_Fps = 25;
        pProps->m_IsFractional = FALSE;
        pProps->m_IsInterlaced = TRUE;
        pProps->m_IsHd = FALSE;

        pProps->m_Field1Start = 1;    
        pProps->m_Field1End = 312;
        pProps->m_Field1ActVidStart = 23;   
        pProps->m_Field1ActVidEnd = 310;
        pProps->m_SwitchingLines[0] = 6;

        pProps->m_Field2Start = 313;  
        pProps->m_Field2End = 625;
        pProps->m_Field2ActVidStart = 336;  
        pProps->m_Field2ActVidEnd  = 623;
        pProps->m_SwitchingLines[1] = 319;

        pProps->m_VancNumS = pProps->m_ActVidNumS = 720*2;
        pProps->m_HancNumS = 280;
        pProps->m_SavNumS = 4;
        pProps->m_EavNumS = 4;

        pProps->m_SyncPointPixelOff = 12;   // Sync point @pixel 12
        break;

    case DT_VIDSTD_625P50:
        pProps->m_NumLines = 625;
        pProps->m_Fps = 50;
        pProps->m_IsFractional = FALSE;
        pProps->m_IsInterlaced = FALSE;
        pProps->m_IsHd = FALSE;

        pProps->m_Field1Start = 1;
        pProps->m_Field1End = 625;
        pProps->m_Field1ActVidStart = 23;
        pProps->m_Field1ActVidEnd = 598;

        pProps->m_SwitchingLines[0] = 6;
        pProps->m_SwitchingLines[1] = -1;

        pProps->m_Field2Start = 0;
        pProps->m_Field2End = 0;
        pProps->m_Field2ActVidStart = 0;
        pProps->m_Field2ActVidEnd  = 0;

        pProps->m_VancNumS = pProps->m_ActVidNumS = 720*2;
        pProps->m_HancNumS = 280;
        pProps->m_SavNumS = 4;
        pProps->m_EavNumS = 4;

        pProps->m_SyncPointPixelOff = 0;
        break;

    case DT_VIDSTD_2160P60:
    case DT_VIDSTD_2160P60B:
    case DT_VIDSTD_2160P59_94:
    case DT_VIDSTD_2160P59_94B:
    case DT_VIDSTD_2160P50:
    case DT_VIDSTD_2160P50B:
    case DT_VIDSTD_1080P60:
    case DT_VIDSTD_1080P60B:
    case DT_VIDSTD_1080P59_94:
    case DT_VIDSTD_1080P59_94B:
    case DT_VIDSTD_1080P50:
    case DT_VIDSTD_1080P50B:

        pProps->m_NumLines = 1125;

        if (VidStd==DT_VIDSTD_1080P50 || VidStd==DT_VIDSTD_1080P50B
                               || VidStd==DT_VIDSTD_2160P50 || VidStd==DT_VIDSTD_2160P50B)

            pProps->m_Fps = 50;
        else
            pProps->m_Fps = 60;

        pProps->m_IsFractional = (VidStd==DT_VIDSTD_1080P59_94
                                                        || VidStd==DT_VIDSTD_1080P59_94B
                                                        || VidStd==DT_VIDSTD_2160P59_94
                                                        || VidStd==DT_VIDSTD_2160P59_94B);
        pProps->m_IsInterlaced = FALSE;
        pProps->m_IsHd = TRUE;

        pProps->m_Field1Start = 1;
        pProps->m_Field1End = 1125;

        pProps->m_Field1ActVidStart = 42;
        pProps->m_Field1ActVidEnd = 1121;

        pProps->m_SwitchingLines[0] = 7;
        pProps->m_SwitchingLines[1] = -1;

        pProps->m_Field2Start = 0;
        pProps->m_Field2End = 0;
        
        pProps->m_Field2ActVidStart = 0;
        pProps->m_Field2ActVidEnd  = 0;
        
        pProps->m_VancNumS = pProps->m_ActVidNumS = 1920*2;
        if (VidStd==DT_VIDSTD_1080P60 || VidStd==DT_VIDSTD_1080P60B
                         || VidStd==DT_VIDSTD_1080P59_94 || VidStd==DT_VIDSTD_1080P59_94B
                         || VidStd==DT_VIDSTD_2160P60 || VidStd==DT_VIDSTD_2160P60B
                         || VidStd==DT_VIDSTD_2160P59_94 || VidStd==DT_VIDSTD_2160P59_94B)

        {
            pProps->m_HancNumS = 268*2;
            // Set Sync point
            pProps->m_SyncPointPixelOff = 88;   // Sync point @pixel 88
        }
        else if (VidStd==DT_VIDSTD_1080P50 || VidStd==DT_VIDSTD_1080P50B 
                               || VidStd==DT_VIDSTD_2160P50 || VidStd==DT_VIDSTD_2160P50B)

        {
            pProps->m_HancNumS = 708*2;
            pProps->m_SyncPointPixelOff = 528;   // Sync point @pixel 528
        }
        else
            DT_ASSERT(1==0);

        pProps->m_EavNumS = 8*2;
        pProps->m_SavNumS = 4*2;
        break;

    case DT_VIDSTD_2160P30:
    case DT_VIDSTD_2160P29_97:
    case DT_VIDSTD_2160P25:
    case DT_VIDSTD_2160P24:
    case DT_VIDSTD_2160P23_98:
    case DT_VIDSTD_1080P30:
    case DT_VIDSTD_1080P29_97:
    case DT_VIDSTD_1080P25:
    case DT_VIDSTD_1080P24:
    case DT_VIDSTD_1080P23_98:

        pProps->m_NumLines = 1125;

        if (VidStd==DT_VIDSTD_1080P30 || VidStd==DT_VIDSTD_1080P29_97
                             || VidStd==DT_VIDSTD_2160P30 || VidStd==DT_VIDSTD_2160P29_97)
            pProps->m_Fps = 30;
        else if (VidStd==DT_VIDSTD_1080P25 || VidStd==DT_VIDSTD_2160P25)
            pProps->m_Fps = 25;
        else if (VidStd==DT_VIDSTD_1080P24 || VidStd==DT_VIDSTD_1080P23_98
                             || VidStd==DT_VIDSTD_2160P24 || VidStd==DT_VIDSTD_2160P23_98)
            pProps->m_Fps = 24;
        else
            DT_ASSERT(1==0);

        pProps->m_IsFractional = (VidStd==DT_VIDSTD_1080P29_97 
                                                         || VidStd==DT_VIDSTD_1080P23_98
                                                         || VidStd==DT_VIDSTD_2160P29_97
                                                         || VidStd==DT_VIDSTD_2160P23_98);
        pProps->m_IsInterlaced = FALSE;
        pProps->m_IsHd = TRUE;

        pProps->m_Field1Start = 1;
        pProps->m_Field1End = 1125;

        pProps->m_Field1ActVidStart = 42;
        pProps->m_Field1ActVidEnd = 1121;
        pProps->m_SwitchingLines[0] = 7;

        pProps->m_Field2Start = 0;
        pProps->m_Field2End = 0;
        pProps->m_Field2ActVidStart = 0;
        pProps->m_Field2ActVidEnd = 0;
        pProps->m_SwitchingLines[1] = -1;

        pProps->m_VancNumS = pProps->m_ActVidNumS = 1920*2;
        if (VidStd==DT_VIDSTD_1080P30 || VidStd==DT_VIDSTD_1080P29_97
                             || VidStd==DT_VIDSTD_2160P30 || VidStd==DT_VIDSTD_2160P29_97)
        {
            pProps->m_HancNumS = 268*2;
            pProps->m_SyncPointPixelOff = 88;   // Sync point @pixel 88
        }
        else if (VidStd==DT_VIDSTD_1080P25 || VidStd==DT_VIDSTD_2160P25)
        {
            pProps->m_HancNumS = 708*2;
            pProps->m_SyncPointPixelOff = 528;   // Sync point @pixel 528
        }
        else if (VidStd==DT_VIDSTD_1080P24 || VidStd==DT_VIDSTD_1080P23_98
                             || VidStd==DT_VIDSTD_2160P24 || VidStd==DT_VIDSTD_2160P23_98)
        {
            pProps->m_HancNumS = 818*2;
            pProps->m_SyncPointPixelOff = 638;   // Sync point @pixel 638
        }
        else
            DT_ASSERT(1==0);

        pProps->m_EavNumS = 8*2;
        pProps->m_SavNumS = 4*2;

        break;

    case DT_VIDSTD_1080I60:
    case DT_VIDSTD_1080I59_94:
    case DT_VIDSTD_1080I50:
    case DT_VIDSTD_1080PSF30:
    case DT_VIDSTD_1080PSF29_97:
    case DT_VIDSTD_1080PSF25:
    case DT_VIDSTD_1080PSF24:
    case DT_VIDSTD_1080PSF23_98:
        pProps->m_NumLines = 1125;

        if (VidStd==DT_VIDSTD_1080I60 || VidStd==DT_VIDSTD_1080I59_94 
                         || VidStd==DT_VIDSTD_1080PSF30 || VidStd==DT_VIDSTD_1080PSF29_97)
            pProps->m_Fps = 30;
        else if (VidStd==DT_VIDSTD_1080I50 || VidStd==DT_VIDSTD_1080PSF25)
            pProps->m_Fps = 25;
        else if (VidStd==DT_VIDSTD_1080PSF24 || VidStd==DT_VIDSTD_1080PSF23_98)
            pProps->m_Fps = 24;
        else
            DT_ASSERT(1==0);

        pProps->m_IsFractional = (VidStd==DT_VIDSTD_1080I59_94 ||
                                  VidStd==DT_VIDSTD_1080PSF29_97 ||
                                  VidStd==DT_VIDSTD_1080PSF23_98);
        pProps->m_IsInterlaced = TRUE;
        pProps->m_IsHd = TRUE;

        pProps->m_Field1Start = 1; 
        pProps->m_Field1End = 563;
        pProps->m_Field1ActVidStart = 21; 
        pProps->m_Field1ActVidEnd = 560;
        pProps->m_SwitchingLines[0] = 7;

        pProps->m_Field2Start = 564;
        pProps->m_Field2End = 1125;
        pProps->m_Field2ActVidStart = 584;
        pProps->m_Field2ActVidEnd = 1123;
        pProps->m_SwitchingLines[1] = 569;

        pProps->m_VancNumS = pProps->m_ActVidNumS = 1920*2;
        if (VidStd==DT_VIDSTD_1080I60 || VidStd==DT_VIDSTD_1080I59_94 ||
            VidStd==DT_VIDSTD_1080PSF30 || VidStd==DT_VIDSTD_1080PSF29_97)
        {
            pProps->m_HancNumS = 268*2;
            pProps->m_SyncPointPixelOff = 88;   // Sync point @pixel 88
        }
        else if (VidStd==DT_VIDSTD_1080I50 || VidStd==DT_VIDSTD_1080PSF25)
        {
            pProps->m_HancNumS = 708*2;
            pProps->m_SyncPointPixelOff = 528;   // Sync point @pixel 528
        }
        else if (VidStd==DT_VIDSTD_1080PSF24 || VidStd==DT_VIDSTD_1080PSF23_98)
        {
            pProps->m_HancNumS = 818*2;
            pProps->m_SyncPointPixelOff = 638;   // Sync point @pixel 638
        }
        else
            DT_ASSERT(1==0);
        
        pProps->m_EavNumS = 8*2;
        pProps->m_SavNumS = 4*2;
        break;

    case DT_VIDSTD_720P60:
    case DT_VIDSTD_720P59_94:
    case DT_VIDSTD_720P50:
    case DT_VIDSTD_720P30:
    case DT_VIDSTD_720P29_97:
    case DT_VIDSTD_720P25:
    case DT_VIDSTD_720P24:
    case DT_VIDSTD_720P23_98:
        pProps->m_NumLines = 750;

        if (VidStd==DT_VIDSTD_720P60 || VidStd==DT_VIDSTD_720P59_94)
            pProps->m_Fps = 60;
        else if (VidStd==DT_VIDSTD_720P50)
            pProps->m_Fps = 50;
        else if (VidStd==DT_VIDSTD_720P30 || VidStd==DT_VIDSTD_720P29_97)
            pProps->m_Fps = 30;
        else if (VidStd==DT_VIDSTD_720P25)
            pProps->m_Fps = 25;
        else if (VidStd==DT_VIDSTD_720P24 || VidStd==DT_VIDSTD_720P23_98)
            pProps->m_Fps = 24;
        else
            DT_ASSERT(1==0);

        pProps->m_IsFractional = (VidStd==DT_VIDSTD_720P59_94 
                                                          || VidStd==DT_VIDSTD_720P29_97 
                                                          || VidStd==DT_VIDSTD_720P23_98);
        pProps->m_IsInterlaced = FALSE;
        pProps->m_IsHd = TRUE;

        pProps->m_Field1Start = 1;
        pProps->m_Field1End = 750;
        pProps->m_Field1ActVidStart = 26; 
        pProps->m_Field1ActVidEnd = 745;
        pProps->m_SwitchingLines[0] = 7;

        pProps->m_Field2Start = 0;
        pProps->m_Field2End = 0;
        pProps->m_Field2ActVidStart = 0; 
        pProps->m_Field2ActVidEnd = 0;
        pProps->m_SwitchingLines[1] = -1;

        pProps->m_VancNumS = pProps->m_ActVidNumS = 1280*2;
        if (VidStd==DT_VIDSTD_720P60 || VidStd==DT_VIDSTD_720P59_94)
        {
            pProps->m_HancNumS = 358*2;
            pProps->m_SyncPointPixelOff = 110;   // Sync point @pixel 110
        }
        else if (VidStd==DT_VIDSTD_720P50)
        {
            pProps->m_HancNumS = 688*2;
            pProps->m_SyncPointPixelOff = 440;   // Sync point @pixel 440
        }
        else if (VidStd==DT_VIDSTD_720P30 || VidStd==DT_VIDSTD_720P29_97)
        {
            pProps->m_HancNumS = 2008*2;
            pProps->m_SyncPointPixelOff = 1760;   // Sync point @pixel 88
        }
        else if (VidStd==DT_VIDSTD_720P25)
        {
            pProps->m_HancNumS = 2668*2;
            pProps->m_SyncPointPixelOff = 2420;   // Sync point @pixel 88
        }
        else if (VidStd==DT_VIDSTD_720P24 || VidStd==DT_VIDSTD_720P23_98)
        {
            pProps->m_HancNumS = 2833*2;
            pProps->m_SyncPointPixelOff = 2585;   // Sync point @pixel 88
        }
        else
            DT_ASSERT(1==0);

        pProps->m_EavNumS = 8*2;
        pProps->m_SavNumS = 4*2;
        break;

    default:
        DtDbgOut(ERR, AV, "Unknown IO-standard");
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Store the video video standard
    pProps->m_VidStd = VidStd;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvGetNumPixelsPerLine -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtAvGetNumPixelsPerLine(Int VidStd)
{
    DtAvFrameProps  AvProps;
    DtStatus  Status = DtAvGetFrameProps(VidStd, &AvProps);
    if (Status != DT_STATUS_OK)
        return -1;
    // NOTE: there are two symbols in one pixel => divide by 2 
    return (AvProps.m_EavNumS + AvProps.m_HancNumS + AvProps.m_SavNumS 
                                                                + AvProps.m_VancNumS) / 2;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Video Standard helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvIoStd2VidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtAvIoStd2VidStd(Int  Value, Int  SubValue)
{
    switch (SubValue)
    {
    case DT_IOCONFIG_1080P50:       return DT_VIDSTD_1080P50;
    case DT_IOCONFIG_1080P50B:      return DT_VIDSTD_1080P50B;
    case DT_IOCONFIG_1080P59_94:    return DT_VIDSTD_1080P59_94;
    case DT_IOCONFIG_1080P59_94B:   return DT_VIDSTD_1080P59_94B;
    case DT_IOCONFIG_1080P60:       return DT_VIDSTD_1080P60;
    case DT_IOCONFIG_1080P60B:      return DT_VIDSTD_1080P60B;
    case DT_IOCONFIG_1080I50:       return DT_VIDSTD_1080I50;
    case DT_IOCONFIG_1080I59_94:    return DT_VIDSTD_1080I59_94;
    case DT_IOCONFIG_1080I60:       return DT_VIDSTD_1080I60;
    case DT_IOCONFIG_1080P23_98:    return DT_VIDSTD_1080P23_98;
    case DT_IOCONFIG_1080P24:       return DT_VIDSTD_1080P24;
    case DT_IOCONFIG_1080P25:       return DT_VIDSTD_1080P25;
    case DT_IOCONFIG_1080P29_97:    return DT_VIDSTD_1080P29_97;
    case DT_IOCONFIG_1080P30:       return DT_VIDSTD_1080P30;
    case DT_IOCONFIG_1080PSF23_98:  return DT_VIDSTD_1080PSF23_98;
    case DT_IOCONFIG_1080PSF24:     return DT_VIDSTD_1080PSF24;
    case DT_IOCONFIG_1080PSF25:     return DT_VIDSTD_1080PSF25;
    case DT_IOCONFIG_1080PSF29_97:  return DT_VIDSTD_1080PSF29_97;
    case DT_IOCONFIG_1080PSF30:     return DT_VIDSTD_1080PSF30;
    case DT_IOCONFIG_2160P23_98:    return DT_VIDSTD_2160P23_98;
    case DT_IOCONFIG_2160P24:       return DT_VIDSTD_2160P24;
    case DT_IOCONFIG_2160P25:       return DT_VIDSTD_2160P25;
    case DT_IOCONFIG_2160P29_97:    return DT_VIDSTD_2160P29_97;
    case DT_IOCONFIG_2160P30:       return DT_VIDSTD_2160P30;
    case DT_IOCONFIG_2160P50:       return DT_VIDSTD_2160P50;
    case DT_IOCONFIG_2160P50B:      return DT_VIDSTD_2160P50B;
    case DT_IOCONFIG_2160P59_94:    return DT_VIDSTD_2160P59_94;
    case DT_IOCONFIG_2160P59_94B:   return DT_VIDSTD_2160P59_94B;
    case DT_IOCONFIG_2160P60:       return DT_VIDSTD_2160P60;
    case DT_IOCONFIG_2160P60B:      return DT_VIDSTD_2160P60B;
    case DT_IOCONFIG_720P23_98:     return DT_VIDSTD_720P23_98;
    case DT_IOCONFIG_720P24:        return DT_VIDSTD_720P24;
    case DT_IOCONFIG_720P25:        return DT_VIDSTD_720P25;
    case DT_IOCONFIG_720P29_97:     return DT_VIDSTD_720P29_97;
    case DT_IOCONFIG_720P30:        return DT_VIDSTD_720P30;
    case DT_IOCONFIG_720P50:        return DT_VIDSTD_720P50;
    case DT_IOCONFIG_720P59_94:     return DT_VIDSTD_720P59_94;
    case DT_IOCONFIG_720P60:        return DT_VIDSTD_720P60;
    case DT_IOCONFIG_525I59_94:     return DT_VIDSTD_525I59_94;
    case DT_IOCONFIG_625I50:        return DT_VIDSTD_625I50;
    }
    return DT_VIDSTD_UNKNOWN;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvVidStd2Fps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtAvVidStd2Fps(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_720P59_94:
    case DT_VIDSTD_720P60:
    case DT_VIDSTD_1080P59_94:
    case DT_VIDSTD_1080P59_94B:
    case DT_VIDSTD_1080P60:
    case DT_VIDSTD_1080P60B:
    case DT_VIDSTD_2160P59_94:
    case DT_VIDSTD_2160P59_94B:
    case DT_VIDSTD_2160P60:
    case DT_VIDSTD_2160P60B:
    case DT_VIDSTD_480P59_94:
    case DT_VIDSTD_525P59_94:
        return 60;

    case DT_VIDSTD_720P50:
    case DT_VIDSTD_1080P50:
    case DT_VIDSTD_1080P50B:
    case DT_VIDSTD_2160P50:
    case DT_VIDSTD_2160P50B:
    case DT_VIDSTD_625P50:
        return 50;

    case DT_VIDSTD_525I59_94:
    case DT_VIDSTD_720P29_97:
    case DT_VIDSTD_720P30:
    case DT_VIDSTD_1080P30:
    case DT_VIDSTD_1080P29_97:
    case DT_VIDSTD_1080PSF30:
    case DT_VIDSTD_1080PSF29_97:
    case DT_VIDSTD_1080I59_94:
    case DT_VIDSTD_1080I60: 
    case DT_VIDSTD_2160P29_97:
    case DT_VIDSTD_2160P30:
        return 30;

    case DT_VIDSTD_625I50:
    case DT_VIDSTD_720P25:
    case DT_VIDSTD_1080P25:
    case DT_VIDSTD_1080PSF25:
    case DT_VIDSTD_1080I50:
    case DT_VIDSTD_2160P25:
        return 25;
    
    case DT_VIDSTD_720P23_98:
    case DT_VIDSTD_720P24:
    case DT_VIDSTD_1080P24:
    case DT_VIDSTD_1080P23_98:
    case DT_VIDSTD_1080PSF24:
    case DT_VIDSTD_1080PSF23_98:
    case DT_VIDSTD_2160P23_98:
    case DT_VIDSTD_2160P24:
        return 24;

   default:
        break;
    }
    return -1;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvVidStdIsFractional -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtAvVidStdIsFractional(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_720P59_94:
    case DT_VIDSTD_1080P59_94:
    case DT_VIDSTD_1080P59_94B:
    case DT_VIDSTD_525I59_94:
    case DT_VIDSTD_720P29_97:
    case DT_VIDSTD_1080P29_97:
    case DT_VIDSTD_1080PSF29_97:
    case DT_VIDSTD_1080I59_94:
    case DT_VIDSTD_720P23_98:
    case DT_VIDSTD_1080P23_98:
    case DT_VIDSTD_1080PSF23_98:
    case DT_VIDSTD_480P59_94:
    case DT_VIDSTD_525P59_94:
    case DT_VIDSTD_2160P23_98:
    case DT_VIDSTD_2160P29_97:
    case DT_VIDSTD_2160P59_94:
    case DT_VIDSTD_2160P59_94B:
        return TRUE;
    }
    return FALSE;

}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvVidStdIsInterlaced -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtAvVidStdIsInterlaced(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_525I59_94:
    case DT_VIDSTD_625I50:
    case DT_VIDSTD_1080I50:
    case DT_VIDSTD_1080I59_94:
    case DT_VIDSTD_1080I60:
    case DT_VIDSTD_1080PSF23_98:
    case DT_VIDSTD_1080PSF24:
    case DT_VIDSTD_1080PSF25:
    case DT_VIDSTD_1080PSF29_97:
    case DT_VIDSTD_1080PSF30:
        return TRUE;
    }
    return FALSE;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvVidStdIsPsf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtAvVidStdIsPsf(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_1080PSF30:
    case DT_VIDSTD_1080PSF29_97:
    case DT_VIDSTD_1080PSF25:
    case DT_VIDSTD_1080PSF24:
    case DT_VIDSTD_1080PSF23_98:
        return TRUE;
    }
    return FALSE;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvVidStdIs3gSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtAvVidStdIs3gSdi(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_1080P50:
    case DT_VIDSTD_1080P50B:
    case DT_VIDSTD_1080P59_94:
    case DT_VIDSTD_1080P59_94B:
    case DT_VIDSTD_1080P60:
    case DT_VIDSTD_1080P60B:
        return TRUE;
    }
    return FALSE;
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvVidStdIs6gSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtAvVidStdIs6gSdi(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_2160P23_98:
    case DT_VIDSTD_2160P24:
    case DT_VIDSTD_2160P25:
    case DT_VIDSTD_2160P29_97:
    case DT_VIDSTD_2160P30:
        return TRUE;
    }
    return FALSE;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvVidStdIs12gSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtAvVidStdIs12gSdi(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_2160P50:
    case DT_VIDSTD_2160P50B:
    case DT_VIDSTD_2160P59_94:
    case DT_VIDSTD_2160P59_94B:
    case DT_VIDSTD_2160P60:
    case DT_VIDSTD_2160P60B:
        return TRUE;
    }
    return FALSE;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvVidStdIs3glvlBSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtAvVidStdIs3glvlBSdi(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_1080P50B:
    case DT_VIDSTD_1080P59_94B:
    case DT_VIDSTD_1080P60B:
        return TRUE;
    }
    return FALSE;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvVidStdIsHdSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtAvVidStdIsHdSdi(Int  VidStd)
{
    // Not 3G/6G/12G-SDI and also not SD-SDI, than it must be HD-SDI
    return (!DtAvVidStdIs12gSdi(VidStd) && !DtAvVidStdIs6gSdi(VidStd) && 
                                !DtAvVidStdIs3gSdi(VidStd) && !DtAvVidStdIsSdSdi(VidStd));
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvVidStdIsSdSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtAvVidStdIsSdSdi(Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_525I59_94:
    case DT_VIDSTD_625I50:
        return TRUE;
    }
    return FALSE;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- VidStdName -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
char*  VidStdName(Int VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_1080P50:       return "1080P50";
    case DT_VIDSTD_1080P50B:      return "1080P50B";
    case DT_VIDSTD_1080P59_94:    return "1080P59.94";
    case DT_VIDSTD_1080P59_94B:   return "1080P59.94B";
    case DT_VIDSTD_1080P60:       return "1080P60";
    case DT_VIDSTD_1080P60B:      return "1080P60B";
    case DT_VIDSTD_1080I50:       return "1080I50";
    case DT_VIDSTD_1080I59_94:    return "1080I59.94";
    case DT_VIDSTD_1080I60:       return "1080I60";
    case DT_VIDSTD_1080P23_98:    return "1080P23.98";
    case DT_VIDSTD_1080P24:       return "1080P24";
    case DT_VIDSTD_1080P25:       return "1080P25";
    case DT_VIDSTD_1080P29_97:    return "1080P29.97";
    case DT_VIDSTD_1080P30:       return "1080P30";
    case DT_VIDSTD_1080PSF23_98:  return "1080PSF23.98";
    case DT_VIDSTD_1080PSF24:     return "1080PSF24";
    case DT_VIDSTD_1080PSF25:     return "1080PSF25";
    case DT_VIDSTD_1080PSF29_97:  return "1080PSF29.97";
    case DT_VIDSTD_1080PSF30:     return "1080PSF30";
    case DT_VIDSTD_2160P23_98:    return "2160P23.98";
    case DT_VIDSTD_2160P24:       return "2160P24";
    case DT_VIDSTD_2160P25:       return "2160P25";
    case DT_VIDSTD_2160P29_97:    return "2160P29.97";
    case DT_VIDSTD_2160P30:       return "2160P30";
    case DT_VIDSTD_2160P50:       return "2160P50";
    case DT_VIDSTD_2160P50B:      return "2160P50B";
    case DT_VIDSTD_2160P59_94:    return "2160P59.94";
    case DT_VIDSTD_2160P59_94B:   return "2160P59.94B";
    case DT_VIDSTD_2160P60:       return "2160P60";
    case DT_VIDSTD_2160P60B:      return "2160P60B";
    case DT_VIDSTD_720P23_98:     return "720P23.98";
    case DT_VIDSTD_720P24:        return "720P24";
    case DT_VIDSTD_720P25:        return "720P25";
    case DT_VIDSTD_720P29_97:     return "720P29.97";
    case DT_VIDSTD_720P30:        return "720P30";
    case DT_VIDSTD_720P50:        return "720P50";
    case DT_VIDSTD_720P59_94:     return "720P59.94";
    case DT_VIDSTD_720P60:        return "720P60";
    case DT_VIDSTD_525I59_94:     return "525I59.94";
    case DT_VIDSTD_625I50:        return "625I50";
    }
    return "Unknown";
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvVidStd2FrameLength -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Return the number of clock ticks in a frame
//
Int DtAvVidStd2FrameLength(Int VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_720P59_94:
    case DT_VIDSTD_720P60:
    case DT_VIDSTD_1080P59_94:
    case DT_VIDSTD_1080P59_94B:
    case DT_VIDSTD_1080P60:
    case DT_VIDSTD_1080P60B:
        return 2475000;     // 60fps

    case DT_VIDSTD_720P50:
    case DT_VIDSTD_1080P50:
    case DT_VIDSTD_1080P50B:
        return 2970000;     // 50 fps

    case DT_VIDSTD_720P29_97:
    case DT_VIDSTD_720P30:
    case DT_VIDSTD_1080P30:
    case DT_VIDSTD_1080P29_97:
    case DT_VIDSTD_1080PSF30:
    case DT_VIDSTD_1080PSF29_97:
    case DT_VIDSTD_1080I59_94:
    case DT_VIDSTD_1080I60: 
        return 4950000;     // 30 fps

    case DT_VIDSTD_525I59_94:
        return 4954950;     // 29.97fps (based on non-fractional clock)

    case DT_VIDSTD_625I50:
    case DT_VIDSTD_720P25:
    case DT_VIDSTD_1080P25:
    case DT_VIDSTD_1080PSF25:
    case DT_VIDSTD_1080I50:
        return 5940000;     // 25 fps
    
    case DT_VIDSTD_720P23_98:
    case DT_VIDSTD_720P24:
    case DT_VIDSTD_1080P24:
    case DT_VIDSTD_1080P23_98:
    case DT_VIDSTD_1080PSF24:
    case DT_VIDSTD_1080PSF23_98:
        return 6187500;     // 24 fps

   default:
        break;
    }
    return -1;
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvVidStd2FramePeriod -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Return the frame period in nanoseconds
//
Int DtAvVidStd2FramePeriod(Int VidStd)
{
    Int Fps = DtAvVidStd2Fps(VidStd); 
    Bool  IsFract = DtAvVidStdIsFractional(VidStd);
    UInt64  Period;
    if (Fps <= 0)
        return -1;
    if (IsFract)
        Period = DtDivide64((UInt64)(1001LL*1*Exp9 + (1000*Fps/2)), (UInt64)(1000*Fps),
                                                                                    NULL);
    else
        Period = DtDivide64((UInt64)(1*Exp9 + Fps/2), (UInt64)Fps, NULL);
    return (Int)Period;
}



// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvVidStdSymbOffset2TimeOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
//  Returns time offset in nano seconds
//
Int DtAvVidStdSymbOffset2TimeOffset(Int VidStd, Int SymbolOffset)
{
    UInt64 OffsetNs = 0;

    // Determine sample clock
    UInt  SampleClock;
    if (DtAvVidStdIs3gSdi(VidStd))
        SampleClock = 2*1485*100*1000;
    else  if (DtAvVidStdIsHdSdi(VidStd))
        SampleClock = 2*1485*100*1000/2;
    else if (DtAvVidStdIsSdSdi(VidStd))
        SampleClock = 2*1485*100*1000/11;
    else
    {
        DT_ASSERT(FALSE);
        return 0;
    }

    // Compute time offet in nano seconds
    OffsetNs = DtDivide64((UInt64)(SymbolOffset*Exp9 + SampleClock/2), SampleClock, NULL);
    if (DtAvVidStdUsesFractionalClock(VidStd))
        OffsetNs = DtDivide64(OffsetNs*1001, 1000, NULL);

    return (Int)OffsetNs;
}



// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtAvVidStdUsesFractionalClock -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool DtAvVidStdUsesFractionalClock(Int VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_720P59_94:
    case DT_VIDSTD_1080P59_94:
    case DT_VIDSTD_1080P59_94B:
    case DT_VIDSTD_720P29_97:
    case DT_VIDSTD_1080P29_97:
    case DT_VIDSTD_1080PSF29_97:
    case DT_VIDSTD_1080I59_94:
    case DT_VIDSTD_720P23_98:
    case DT_VIDSTD_1080P23_98:
    case DT_VIDSTD_1080PSF23_98:
        return TRUE;
    }
    return FALSE;
}