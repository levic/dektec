//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* SdiAvRx.h *#*#*#*#*#*#*#*#*# (C) 2015-2016 DekTec
//
// Dta driver - Declares SDI Audio/Video Receiver related functions
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

#ifndef __SDIAVRX_H
#define __SDIAVRX_H


#define  DTA_SDIAVRX_FRAMEINT_TIMEOUT   200000  // 200ms frame interrupt timeout
                                                // after this timeout AV status is 
                                                // cleared

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSdiAvRxPort -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSdiAvRxAncExStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtaSdiAvRxAncExStatus
{
    DtSpinLock  m_Lock;                 // Spinlock to protect buffer
    Int  m_BufLoad;                     // Number of bytes in m_Buf
    UInt32  m_Buf[1024];                // Contents of FW fifo at last frame interrupt
} DtaSdiAvRxAncExStatus;

// Audio Chan pair status
typedef struct _DtaSdiAvRxAudStatus
{
    Int  m_ChanPairSelect;              // Selected audio channel pair
    Int  m_IsAcpPresent;                // Is audio control packet present?
    Int  m_IsAsynchronous;              // Is Chan pair asynchronous?
    Int  m_Rate;                        // Audio sample rate
    Int  m_IsChan1Active;               // Is audio Channel 1 active?
    Int  m_IsChan2Active;               // Is audio Channel 2 active?
    UInt32  m_Chan1StatusData[6];       // Channel 1 status data
    UInt32  m_Chan2StatusData[6];       // Channel 2 status data
} DtaSdiAvRxAudStatus;

typedef struct _DtaSdiAvRxVidStatus
{
    Int  m_VideoFormat;                 // Video format
    Int  m_PictureRate;                 // Picture rate
    Int  m_IsSdiValid;                  // Is SDI valid?
    Int  m_IsTrsLocked;                 // Is TRS locked?
    Int  m_IsVpidValid;                 // Is VPID valid?
    Int  m_IsVpidBValid;                // Is VPID link B valid?
    UInt32  m_CrcErrorCount;            // SDI crc error count
    UInt32  m_Vpid;                     // Video payload identifier (VPID)
    UInt32  m_VpidB;                    // Video payload identifier B (VPID B)
} DtaSdiAvRxVidStatus;

typedef struct _DtaSdiAvRxPort
{
    DtSpinLock  m_StateLock;        // Spinlock to protect state
    DtDpc  m_IntDpc;
    Int  m_UpdateAll;               // Update all status fields
    Int  m_SdiStandard;             // SDI standard
    Int  m_FrameIntTimeoutUs;       // Frame interrupt timeout in micro seconds
    Int  m_PerIntItvUs;             // Periodic interrupt interval in micro seconds
    DtaSdiAvRxVidStatus  m_VideoStatus;
    UInt32  m_AudioPresence;
    Int  m_NumAudioExtractors;
    DtaSdiAvRxAudStatus  m_AudioStatus[DTA_SDIAVRX_MAX_NUM_AUDPAIRS];
    DtaSdiAvRxAncExStatus  m_AncEx; // ANC extractor
    pDtaEnDecNewInputVidStd  m_NewVidStdCb;
    DtaNonIpPort* m_pPortCb;
} DtaSdiAvRxPort;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
 
DtStatus  DtaSdiAvRxIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile,
                                           DtIoctlObject* pIoctl, Bool  PowerDownPending);
DtStatus  DtaSdiAvRxInitPowerup(DtaNonIpPort*  pNonIpPort);
Bool  DtaSdiAvRxInterrupt(DtaNonIpPort*  pNonIpPort);
DtStatus  DtaNonIpSdiAvRxInit(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpSdiAvRxInterruptEnable(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpSdiAvRxInterruptDisable(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpSdiAvRxPeriodicInt(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpSdiAvRxDetectVidStd(DtaNonIpPort* pNonIpPort, Int* pVidStd,
                                                               UInt* pVpid, UInt* pVpid2);
DtStatus  DtaNonIpSdiAvRxGetAudioStatus(DtaNonIpPort* pNonIpPort,
                                              DtaIoctlNonIpCmdGetAudioStatusOutput* pOut);
DtStatus  DtaNonIpSdiAvRxSetNewVidStdCb(DtaNonIpPort* pNonIpPort,
                                       pDtaEnDecNewInputVidStd Cb, DtaNonIpPort* pPortCb);
#endif // __SDIAVRX_H
