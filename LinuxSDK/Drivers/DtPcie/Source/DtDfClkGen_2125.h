// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfClkGen_2125.h *#*#*#*#*#*#* (C) 2021-2022 DekTec
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

#ifndef __DT_DF_CLKGEN_2125_H
#define __DT_DF_CLKGEN_2125_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcXPLL.h"
#include "DtBcI2CM.h"

// Name + short-name for the TXCLKCTRL_2178A function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_CLKGEN_2125_NAME          "ClkGen"
#define DT_DF_CLKGEN_2125_SHORTNAME     "CLKGEN"

// MACRO: to init an driver-function-ID for the CLKGEN_2125-DF
#define DT_DF_CLKGEN_2125_INIT_ID(ID, ROLE, INSTANCE, UUID)                              \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_CLKGEN_2125_NAME, DT_DF_CLKGEN_2125_SHORTNAME, ROLE,         \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkGen_2125 definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfClkGen_2125 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtDfClkGen_2125
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // Block controllers
    DtBcXPLL*  m_pBcXPLL10G;            // 10Gb PLL
    DtBcXPLL*  m_pBcXPLL25G;            // 25Gb PLL
}  DtDfClkGen_2125;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfClkGen_2125_Close(DtDf*);
DtDfClkGen_2125*  DtDfClkGen_2125_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                                               Int  Instance,  Int  Uuid);

#endif  // #ifndef __DT_DF_CLKGEN_2125_H

