//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* EnDec.h *#*#*#*#*#*#*#*#*#* (C) 2015-2016 DekTec
//
// Dta driver - Declares decoder/encoder common functions
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

#ifndef _ENDEC_H
#define _ENDEC_H

typedef DtStatus  (*pDtaNonIpExclusiveAccess)(DtaNonIpPort* pNonIpPort, 
                                                          DtFileObject*  pFile, Int  Cmd);
typedef DtStatus  (*pDtaEnDecNewInputVidStd)(DtaNonIpPort* pNonIpPort, 
                                                        Int  SourcePortIndex, Int VidStd);
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaEnDecPort -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtaEnDecPort
{
    // Exclusive access
    Bool  m_ExclAccess;
    DtFileObject  m_ExclAccessOwner;
    DtSpinLock  m_ExclAccessLock;
} DtaEnDecPort;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

DtStatus  DtaEncoderInit(DtaDeviceData*  pDvcData);
DtStatus  DtaEnDecClose(DtaNonIpPort* pNonIpPort, DtFileObject* pFile);
DtStatus  DtaEnDecIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile,
                                           DtIoctlObject* pIoctl, Bool  PowerDownPending);
DtStatus  DtaEnDecInitPowerup(DtaNonIpPort*  pNonIpPort);
DtStatus  DtaEnDecInit(DtaNonIpPort* pNonIpPort);
DtStatus  DtaEnDecPowerdownPre(DtaNonIpPort* pNonIpPort);
DtStatus  DtaEnDecPowerUpPost(DtaNonIpPort* pNonIpPort);

#endif // _ENDEC_H
