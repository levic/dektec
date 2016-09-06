//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* EncD7Pro.h *#*#*#*#*#*#*#*#* (C) 2015-2016 DekTec
//
// Dta driver - Declares D7 Pro encoder related functions
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

#ifndef __ENCD7PRO_H
#define __ENCD7PRO_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEncD7ProPort -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtaEncD7ProPort
{
    DtMutex  m_StatusLock;          // Lock for state/status flags below
    Int  m_State;                   // Current chip state, DTA_D7PRO_STATE_*
    Int  m_Ext12FailCnt;            // Failure counter for external 12V
    Int  m_FanFailCnt;              // Failure counter for fan
    Int  m_PowerFailCnt;            // Failure counter for power good
    
    Int  m_VidStd;                  // Current selected video standard
    Int  m_InputSource;             // Current selected input source
    Int  m_SourcePortIndex[3];      // Ports connected to encoder
    Int  m_DetVidStd[3];            // Detected video standard
    pDtaNonIpExclusiveAccess  m_pFuncExclAccess;  // Callback function for requesting 
                                                  // exclusive access at the parent port
    DtThread  m_PowerControlThread; // Power control thread
    DtEvent  m_PowerControlEvent;   // Power control event
    DtEvent  m_PowerEnabledEvent;   // Power enabled event
    DtaUartPort  m_CtrlUart;
    DtaUartPort  m_DbgUart;
} DtaEncD7ProPort;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
 
DtStatus  DtaEncD7ProIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile,
                                           DtIoctlObject* pIoctl, Bool  PowerDownPending);
DtStatus  DtaEncD7ProInitPowerup(DtaNonIpPort*  pNonIpPort);
DtStatus  DtaEncD7ProInit(DtaNonIpPort* pNonIpPort, pDtaNonIpExclusiveAccess pExclAccess);
DtStatus  DtaEncD7ProPowerdownPre(DtaNonIpPort* pNonIpPort);
DtStatus  DtaEncD7ProPowerUpPost(DtaNonIpPort* pNonIpPort);
DtStatus  DtaEncD7ProUncleanDetach(DtaNonIpPort* pNonIpPort);
Bool  DtaEncD7proInterrupt(DtaNonIpPort* pNonIpPort);

DtStatus  DtaEncD7ProGetVidStd(DtaNonIpPort* pNonIpPort, Int* pVidStd);
DtStatus  DtaEncD7ProSetVidStd(DtaNonIpPort* pNonIpPort, Int VidStd);
DtStatus  DtaEncD7ProGetSourcePort(DtaNonIpPort* pNonIpPort, Int* pPortIndex);
DtStatus  DtaEncD7ProSetSourcePort(DtaNonIpPort* pNonIpPort, Int PortIndex);

DtStatus  DtaEncD7ProNewInputVidStd(DtaNonIpPort* pNonIpPort,
                                                        Int  SourcePortIndex, Int VidStd);


#endif // __ENCD7PRO_H
