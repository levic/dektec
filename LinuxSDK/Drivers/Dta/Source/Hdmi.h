//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Hdmi.h *#*#*#*#*#*#*#*#*#* (C) 2015-2016 DekTec
//
// Dta driver - HDMI functionality - Declaration of generic HDMI functionality
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2015-2016 DekTec Digital Video B.V.
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

#ifndef _HDMI_H
#define _HDMI_H

typedef struct _DtaHdmiStatus
{
    Bool  m_CableDet;
    Bool  m_Encrypted;
    Bool  m_TmdsPllLocked;
    Bool  m_AudioLocked;
    Int  m_AudioChannelMode;
    Int  m_AudioType;
    Bool  m_AudioChanStatusValid;
    UInt8  m_AudioChannelStatus[5];
    Bool  m_AviInfoFrameValid;
    UInt8  m_AviInfoFrame[16];
    Bool  m_AudioInfoFrameValid;
    UInt8  m_AudioInfoFrame[14];
    Bool  m_VertFilterLocked;
    Bool  m_DeRegenFilterLocked;
    Int  m_PixelRepetition;
    Int  m_InpColorspace;
    Int  m_VidWidth;
    Int  m_VidHeight1; // Height of field 1
    Int  m_VidHeight2; // Height of field 2
    Bool  m_Interlaced;
    Int  m_BitsPerChannel;
    Int  m_TmdsFreq;
    Bool  m_CpFreeRun;
    Bool  m_HpaApplied;
    // STDI measurements
    Bool  m_StdiValid;
    Bool  m_StdiInterlaced;
    Int  m_StdiFcl; // Field count length. FCL*256=number of 28.6363 MHz clocks in field
} DtaHdmiStatus;

typedef struct _DtaHdmi
{
    const DtFwbHdmiAdv7610Ctrl*  m_pFwbHdmiAdv7610Ctrl;
    DtaI2cm  m_I2c;
    
    DtThread  m_ControlThread;

    DtEvent  m_IntEvent;
    DtDpc  m_HandleIntDpc;
    
    DtSpinLock  m_StatusLock;
    DtaHdmiStatus  m_Status;
    pDtaEnDecNewInputVidStd  m_NewVidStdCb;
    DtaNonIpPort* m_pPortCb;
} DtaHdmi;

DtStatus  DtaNonIpHdmiInit(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpHdmiInitPowerup(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpHdmiInitPowerUpPost(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpHdmiPowerdownPre(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpHdmiInterruptEnable(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpHdmiInterruptDisable(DtaNonIpPort* pNonIpPort);
Bool  DtaNonIpHdmiInterrupt(DtaNonIpPort* pNonIpPort);

DtStatus  DtaNonIpHdmiDetectVidStd(DtaNonIpPort* pNonIpPort, Int* pVidStd,
                                                                       Int* pAspectRatio);
DtStatus  DtaNonIpHdmiGetAudioStatus(DtaNonIpPort* pNonIpPort,
                                              DtaIoctlNonIpCmdGetAudioStatusOutput* pOut);
DtStatus  DtaNonIpHdmiSetNewVidStdCb(DtaNonIpPort* pNonIpPort,
                                       pDtaEnDecNewInputVidStd Cb, DtaNonIpPort* pPortCb);

#endif // _HDMI_H
