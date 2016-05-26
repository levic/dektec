//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Ad9129.h *#*#*#*#*#*#*#*#*# (C) 2015-2016 DekTec
//
// Dta driver - Ad9129 - Declaration of Ad9129 interface functions

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

#ifndef __AD9129_H
#define __AD9129_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus DtaRfDacInitPowerup(DtaDeviceData* pDvcData);
DtStatus  DtaAd9129Init(DtaNonIpPort*  pNonIpPort);

#define DTA_MAX_NUM_MULTIMOD_CHANS  8
typedef struct
{
    Bool  m_IsSupported;                    // Multi modulator is supported
    DtFastMutex  m_MultiModRfLevelMutex;    // Mutex
    volatile UInt8*  m_pRfRegs;             // Pointer to RF-register block
    // Pointers to the modulator register blocks
    volatile UInt8*  m_pModRegs[DTA_MAX_NUM_MULTIMOD_CHANS];    
    // Parameters used for computing the modulator's RF-level setting
    // the CommonAttenFactor is pow(10, TotalAtten/20.0 + constant). However we cannot 
    // do the floating point computation in the driver.
    // Modulator RF-level setting = ChanTotalGainFactor * CommonAttenFactor
    Int  m_ChanTotalGainFactor[DTA_MAX_NUM_MULTIMOD_CHANS];
    Int  m_CommonAttenFactor[DTA_MAX_NUM_MULTIMOD_CHANS];
    Int  m_FreqMHz[DTA_MAX_NUM_MULTIMOD_CHANS];     // Modulator frequency (for info only)
    Int  m_Atten1[DTA_MAX_NUM_MULTIMOD_CHANS];      // Modulator attenuator 1 setting
    Int  m_Atten2[DTA_MAX_NUM_MULTIMOD_CHANS];      // Modulator attenuator 2 setting 
    Int  m_Atten3[DTA_MAX_NUM_MULTIMOD_CHANS];      // Modulator attenuator 3 setting
    Bool  m_InUse[DTA_MAX_NUM_MULTIMOD_CHANS];      // In use
    DtFileObject  m_CurrentOwner[DTA_MAX_NUM_MULTIMOD_CHANS];  // Owner of the channel
}DtaMultiModData;

DtStatus  DtaMultiModInit(DtaDeviceData*  pDvcData);
DtStatus  DtaMultiModInitPowerup(DtaDeviceData* pDvcData); 
DtStatus  DtaMultiModIoctl(DtaDeviceData*  pDvcData, DtFileObject*  pFile,  
                                          DtIoctlObject*  pIoctl, Bool  PowerDownPending);
DtStatus  DtaMultiModClose(DtaDeviceData*  pDvcData, DtFileObject* pFile);

#endif //#ifndef __AD9129_H
