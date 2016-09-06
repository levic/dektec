//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Gs296x.h *#*#*#*#*#*#*#*#*# (C) 2012-2016 DekTec
//
// Dta driver - Gennum GS296x - Declaration of GS296x interface functions
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2012-2016 DekTec Digital Video B.V.
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

#ifndef __GS296X_H
#define __GS296X_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// GS2961 device latencies in number of PCLKs.
// See GS2961 Datasheet, Table 2-4: AC Electrical Characteristics. NOTE: have derived a 
// average value from this table
#define DTA_GS2961_LATENCY_SD_BYPASS        7   // SD in bypass mode
#define DTA_GS2961_LATENCY_SD_SMPTE         46  // SD in SMPTE mode
#define DTA_GS2961_LATENCY_SD_NO_IOPROC     33  // SD in SMPTE mode + IOPROC disabled
#define DTA_GS2961_LATENCY_HD_BYPASS        7   // HD in bypass mode
#define DTA_GS2961_LATENCY_HD_SMPTE         46  // HD in SMPTE mode
#define DTA_GS2961_LATENCY_HD_NO_IOPROC     34  // HD in SMPTE mode + IOPROC disabled
#define DTA_GS2961_LATENCY_3G_BYPASS        7   // 3G in bypass mode
#define DTA_GS2961_LATENCY_3GA_SMPTE        46  // 3G Levl A in SMPTE mode
#define DTA_GS2961_LATENCY_3GB_SMPTE        112 // 3G level B in SMPTE mode
#define DTA_GS2961_LATENCY_3G_NO_IOPROC     34  // 3G in SMPTE mode + IOPROC disabled

// GS2962 device latencies in number of PCLKs
// See GS2962 Datasheet, Table 2-4: AC Electrical Characteristics
#define DTA_GS2962_LATENCY_SD_BYPASS        54  // SD in bypass mode
#define DTA_GS2962_LATENCY_SD_SMPTE         112 // SD in SMPTE mode
#define DTA_GS2962_LATENCY_SD_NO_IOPROC     94  // SD in SMPTE mode + IOPROC disabled
#define DTA_GS2962_LATENCY_HD_BYPASS        54  // HD in bypass mode
#define DTA_GS2962_LATENCY_HD_SMPTE         95  // HD in SMPTE mode
#define DTA_GS2962_LATENCY_HD_NO_IOPROC     98  // HD in SMPTE mode + IOPROC disabled
#define DTA_GS2962_LATENCY_3G_BYPASS        54  // 3G in bypass mode
#define DTA_GS2962_LATENCY_3G_SMPTE         95  // 3G in SMPTE mode
#define DTA_GS2962_LATENCY_3G_NO_IOPROC     94  // 3G in SMPTE mode + IOPROC disabled

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaGs2961Enable(DtaNonIpPort*  pNonIpPort, Bool  EnaAsiMode, Bool  EnaSdiProc);
DtStatus  DtaGs2961GetVideoStd(DtaNonIpPort*  pNonIpPort, Int*  pVidStd);
DtStatus  DtaGs2962Enable(DtaNonIpPort*  pNonIpPort);
DtStatus  DtaGs2962SetVpid(DtaNonIpPort*  pNonIpPort, UInt  Vpid);

#endif //#ifndef __GS296X_H

