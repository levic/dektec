// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfIpFifo.h *#*#*#*#*#*#*#*# (C) 2018-2021 DekTec
//
// This file is not part of the driver. It can be used as template to create a new
// driver function. Replace "IpFifo" by the driver function name (as in the 
// .FunctionDescriptionsXxx.xml), replace "IPFIFO" by the driver function short name 
// (all uppercase) and replace "IPFIFO" by the driver function short name in camel case.
// 

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018 DekTec Digital Video B.V.
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

#ifndef __DT_DF_IPFIFO_H
#define __DT_DF_IPFIFO_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcIPFIFO.h"

// Name + short-name for the IPFIFO function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_IPFIFO_NAME          "IpFifo"
#define DT_DF_IPFIFO_SHORTNAME     "IPFIFO"

// MACRO: to init an driver-function-ID for the IPFIFO-DF
#define DT_DF_IPFIFO_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_IPFIFO_NAME, DT_DF_IPFIFO_SHORTNAME, ROLE,               \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfIpFifo definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfIpFifo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtDfIpFifo
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // Parameters
    UInt32  m_FifoAddr;
    UInt32  m_FifoSize;

    // Block controllers
    DtBcIPFIFO*  m_pBcIpFifo;     // Ip DDR Fifo block-controller

    // Cached values
    Int  m_OperationalMode;         // Operational mode
}  DtDfIpFifo;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfIpFifo_Close(DtDf*);
DtDfIpFifo*  DtDfIpFifo_Open(DtCore*, DtPt*  pPt, const char*  pRole, Int  Instance,
                                                                              Int  Uuid);
DtStatus  DtDfIpFifo_GetOperationalMode(DtDfIpFifo*, Int*  pOpMode);
DtStatus  DtDfIpFifo_SetOperationalMode(DtDfIpFifo*, Int OpMode);
#endif  // #ifndef __DT_DF_IPFIFO_H

