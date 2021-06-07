// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfTxClkCtrl_2178A.h *#*#*#*#*#*#*#*# (C) 2021 DekTec
//
// This file is not part of the driver. It can be used as template to create a new
// driver function. Replace "TxClkCtrl_2178AName" by the driver function name (as in the 
// .FunctionDescriptionsXxx.xml), replace "TXCLKCTRL_2178A" by the driver function short name 
// (all uppercase) and replace "TXCLKCTRL_2178A" by the driver function short name in camel case.
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

#ifndef __DT_DF_TXCLKCTRL_2178A_H
#define __DT_DF_TXCLKCTRL_2178A_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcI2CM.h"
#include "DtDfTxPllMgr.h"

// Name + short-name for the TXCLKCTRL_2178A function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_TXCLKCTRL_2178A_NAME          "TxClkCtrl"
#define DT_DF_TXCLKCTRL_2178A_SHORTNAME     "TXCLKCTRL"

// MACRO: to init an driver-function-ID for the TXCLKCTRL_2178A-DF
#define DT_DF_TXCLKCTRL_2178A_INIT_ID(ID, ROLE, INSTANCE, UUID)                          \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_TXCLKCTRL_2178A_NAME, DT_DF_TXCLKCTRL_2178A_SHORTNAME, ROLE, \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxClkCtrl_2178A definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxClkCtrl_2178A -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtDfTxClkCtrl_2178A
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    Int  m_Si534XAddress;               // Address of  the SI-534X device
    Int  m_PrevBank;                    // Previous selected bank
    // Block controllers
    DtBcI2CM*  m_pBcI2Cm;               // I2C master block controller
    DtDfTxPllMgr*  m_pDfTxPllMgr;       // TX-PLL manager
}  DtDfTxClkCtrl_2178A;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfTxClkCtrl_2178A_Close(DtDf*);
DtDfTxClkCtrl_2178A*  DtDfTxClkCtrl_2178A_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                                               Int  Instance,  Int  Uuid);

#endif  // #ifndef __DT_DF_TXCLKCTRL_2178A_H

