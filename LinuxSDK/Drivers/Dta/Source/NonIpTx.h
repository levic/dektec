//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NonIpTx.h *#*#*#*#*#*#*#*#*# (C) 2010-2015 DekTec
//
// Dta driver - Non IP TX functionality - Declaration of TX specific functionality for
//                                        non IP ports.
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

#ifndef _NON_IP_TX_H
#define _NON_IP_TX_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaNonIpTxIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile,
                                                                   DtIoctlObject* pIoctl);
// TX Flags
DtStatus  DtaNonIpTxGetFlags(DtaNonIpPort* pNonIpPort, Int* pStatus, Int* pLatched);
DtStatus  DtaNonIpTxClearFlags(DtaNonIpPort* pNonIpPort, Int FlagsToClear);
void  DtaNonIpTxProcessFlagsFromDpc(DtaNonIpPort* pNonIpPort);
void  DtaNonIpTxProcessFlagsFromUser(DtaNonIpPort* pNonIpPort);
// Failsafe
DtStatus  DtaNonIpTxSetFailsafeCfg(DtaNonIpPort* pNonIpPort, Int Enable, Int Timeout);
DtStatus  DtaNonIpTxSetFailsafeAlive(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpTxGetFailsafeInfo(DtaNonIpPort* pNonIpPort, Int* pEnable, Int* pTimeout,
                                                                             Int* pAlive);

#endif // _NON_IP_TX_H
