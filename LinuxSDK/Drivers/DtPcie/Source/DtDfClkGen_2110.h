// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfClkGen_2110.h *#*#*#*#*#*#* (C) 2021-2022 DekTec
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

#ifndef __DT_DF_CLKGEN_2110_H
#define __DT_DF_CLKGEN_2110_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcI2CM.h"
#include "DtBcXPLL.h"

// Name + short-name for the TXCLKCTRL_2178A function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_CLKGEN_2110_NAME          "ClkGen"
#define DT_DF_CLKGEN_2110_SHORTNAME     "CLKGEN"

// MACRO: to init an driver-function-ID for the CLKGEN_2110-DF
#define DT_DF_CLKGEN_2110_INIT_ID(ID, ROLE, INSTANCE, UUID)                              \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_CLKGEN_2110_NAME, DT_DF_CLKGEN_2110_SHORTNAME, ROLE,         \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkGen_2110 definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkGen_2110 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtDfClkGen_2110
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // Block controllers
    DtBcI2CM*  m_pBcI2Cm;               // I2C master block controller
    DtBcXPLL*  m_pBcXPLL1G;             // 1Gb PLL
    DtBcXPLL*  m_pBcXPLL10G;            // 10Gb PLL
}  DtDfClkGen_2110;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfClkGen_2110_Close(DtDf*);
DtDfClkGen_2110*  DtDfClkGen_2110_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                                               Int  Instance,  Int  Uuid);

#endif  // #ifndef __DT_DF_CLKGEN_2110_H

