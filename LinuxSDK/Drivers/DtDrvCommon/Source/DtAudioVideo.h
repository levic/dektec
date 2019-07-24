//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtAudioVideo.h *#*#*#*#*#*#*#* (C) 2014-2016 DekTec
//
// Driver common - Audio Video - audio/video types/functions
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

#ifndef __DT_AUDIO_VIDEO_H
#define __DT_AUDIO_VIDEO_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtAvGetFrameProps(Int VidStd, DtAvFrameProps*  pProps);
Int  DtAvGetNumPixelsPerLine(Int VidStd);
Int  DtAvGetNumVidStd(void);
DtStatus  DtAvGetFramePropsFromIdx(Int Idx, DtAvFrameProps*  pProps);

// Video-standard helpers
// Video-standard helpers
Int  DtAvIoStd2VidStd(Int  Value, Int  SubValue);
Int  DtAvVidStd2Fps(Int  VidStd);
Bool  DtAvVidStdIsFractional(Int  VidStd);
Bool  DtAvVidStdIsInterlaced(Int  VidStd);
Bool  DtAvVidStdIsPsf(Int  VidStd);
Bool  DtAvVidStdIs3gSdi(Int  VidStd);
Bool  DtAvVidStdIs3glvlBSdi(Int  VidStd);
Bool  DtAvVidStdIs6gSdi(Int  VidStd);
Bool  DtAvVidStdIs12gSdi(Int  VidStd);
Bool  DtAvVidStdIsHdSdi(Int  VidStd);
Bool  DtAvVidStdIsSdSdi(Int  VidStd);

Int  DtAvVidStd2FrameLength(Int  VidStd);
Int  DtAvVidStd2FramePeriod(Int VidStd);
Int   DtAvVidStdSymbOffset2TimeOffset(Int VidStd, Int SymbolOffset);
Bool  DtAvVidStdUsesFractionalClock(Int  VidStd);
// For debugging
char*  VidStdName(Int VidStd);

#endif // __DT_AUDIO_VIDEO_H
