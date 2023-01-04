// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfClkCtrl_2116.h *#**#*#*#*#*#*#*#*# (C) 2021 DekTec
//
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

#ifndef __DT_DF_CLKCTRL_2116_H
#define __DT_DF_CLKCTRL_2116_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcSPIM.h"


// Name + short-name for the CLKCTRL_2116 function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_CLKCTRL_2116_NAME          "ClkCtrl"
#define DT_DF_CLKCTRL_2116_SHORTNAME     "CLKCTRL"

// MACRO: to init an driver-function-ID for the CLKCTRL_2116-DF
#define DT_DF_CLKCTRL_2116_INIT_ID(ID, ROLE, INSTANCE, UUID)                             \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_CLKCTRL_2116_NAME, DT_DF_CLKCTRL_2116_SHORTNAME, ROLE,       \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkCtrl_2116 definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkCtrl_2116 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtDfClkCtrl_2116
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // Block controllers
    DtBcSPIM*  m_pBcSpiM;               // SPI master block controller
}  DtDfClkCtrl_2116;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfClkCtrl_2116_Close(DtDf*);
DtDfClkCtrl_2116*  DtDfClkCtrl_2116_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                                               Int  Instance,  Int  Uuid);

#endif  // #ifndef __DT_DF_CLKCTRL_2116_H

