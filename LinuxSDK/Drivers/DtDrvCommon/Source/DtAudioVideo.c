//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtAudioVideo.c *#*#*#*#*#*#*#* (C) 2014-2015 DekTec
//
// Driver common - Audio Video - Definition of audio/video types/functions
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2014-2015 DekTec Digital Video B.V.
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

    case DT_VIDSTD_1080P60:
    case DT_VIDSTD_1080P60B:
    case DT_VIDSTD_1080P59_94:
    case DT_VIDSTD_1080P59_94B:
    case DT_VIDSTD_1080P50:
    case DT_VIDSTD_1080P50B:
        pProps->m_NumLines = 1125;

        if (VidStd==DT_VIDSTD_1080P50 || VidStd==DT_VIDSTD_1080P50B)
            pProps->m_Fps = 50;
        else
            pProps->m_Fps = 60;

        pProps->m_IsFractional = (VidStd==DT_VIDSTD_1080P59_94 ||
                                                           VidStd==DT_VIDSTD_1080P59_94B);
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
        if (VidStd==DT_VIDSTD_1080P60 || VidStd==DT_VIDSTD_1080P60B ||
                            VidStd==DT_VIDSTD_1080P59_94 || VidStd==DT_VIDSTD_1080P59_94B)
        {
            pProps->m_HancNumS = 268*2;
            // Set Sync point
            pProps->m_SyncPointPixelOff = 88;   // Sync point @pixel 88
        }
        else if (VidStd==DT_VIDSTD_1080P50 || VidStd==DT_VIDSTD_1080P50B)
        {
            pProps->m_HancNumS = 708*2;
            pProps->m_SyncPointPixelOff = 528;   // Sync point @pixel 528
        }
        else
            DT_ASSERT(1==0);

        pProps->m_EavNumS = 8*2;
        pProps->m_SavNumS = 4*2;
        break;

    case DT_VIDSTD_1080P30:
    case DT_VIDSTD_1080P29_97:
    case DT_VIDSTD_1080P25:
    case DT_VIDSTD_1080P24:
    case DT_VIDSTD_1080P23_98:
        pProps->m_NumLines = 1125;

        if (VidStd==DT_VIDSTD_1080P30 || VidStd==DT_VIDSTD_1080P29_97)
            pProps->m_Fps = 30;
        else if (VidStd==DT_VIDSTD_1080P25)
            pProps->m_Fps = 25;
        else if (VidStd==DT_VIDSTD_1080P24 || VidStd==DT_VIDSTD_1080P23_98)
            pProps->m_Fps = 24;
        else
            DT_ASSERT(1==0);

        pProps->m_IsFractional = (VidStd==DT_VIDSTD_1080P29_97 
                                                         || VidStd==DT_VIDSTD_1080P23_98);
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
        if (VidStd==DT_VIDSTD_1080P30 || VidStd==DT_VIDSTD_1080P29_97)
        {
            pProps->m_HancNumS = 268*2;
            pProps->m_SyncPointPixelOff = 88;   // Sync point @pixel 88
        }
        else if (VidStd == DT_VIDSTD_1080P25)
        {
            pProps->m_HancNumS = 708*2;
            pProps->m_SyncPointPixelOff = 528;   // Sync point @pixel 528
        }
        else if (VidStd==DT_VIDSTD_1080P24 || VidStd==DT_VIDSTD_1080P23_98)
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
