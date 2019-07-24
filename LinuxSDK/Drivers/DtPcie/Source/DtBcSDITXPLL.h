//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDITXPLL.h *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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

#ifndef __DT_BC_SDITXPLL_H
#define __DT_BC_SDITXPLL_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the Switch block (must match block ID)
#define DT_BC_SDITXPLL_NAME        "SdiTxPll"
#define DT_BC_SDITXPLL_SHORTNAME   "SDITXPLL"

// MACRO: to init an block-controller-ID for the SDITXPLL-BC
#define DT_BC_SDITXPLL_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_SDITXPLL_NAME, DT_BC_SDITXPLL_SHORTNAME, ROLE,               \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXPLL definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Device family
#define DT_BC_SDITXPLL_FAMILY_UNKNOWN      -1  // Unknown
#define DT_BC_SDITXPLL_FAMILY_CV            0  // Intel Cyclone V
#define DT_BC_SDITXPLL_FAMILY_C10           1  // Intel Cyclone 10
#define DT_BC_SDITXPLL_FAMILY_A10           2  // Intel Arria 10

// PLL-type
#define DT_BC_SDITXPLL_PLLTYPE_UNKNOWN     -1  // Unknown
#define DT_BC_SDITXPLL_PLLTYPE_CMU          0  // Clock Multiplier Unit PLL or Channel PLL
#define DT_BC_SDITXPLL_PLLTYPE_FPLL         1  // Fractional PLL
#define DT_BC_SDITXPLL_PLLTYPE_ATX          2  // Advanced Transmit PLL

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPLL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcSDITXPLL
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    // Cached settings
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_PllId;               // PLL-Id
    Int  m_DeviceFamily;        // Device family
    Int  m_PllType;             // PLL-type
}  DtBcSDITXPLL;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPLL public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcSDITXPLL_Close(DtBc*);
DtBcSDITXPLL*  DtBcSDITXPLL_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                            const char*  pRole, Int  Instance, Int  Uuid);
DtStatus  DtBcSDITXPLL_GetPllId(DtBcSDITXPLL* pBc, Int* pPllId);
DtStatus  DtBcSDITXPLL_IsPllLocked(DtBcSDITXPLL* pBc, Bool* pLocked);
DtStatus  DtBcSDITXPLL_ResetClock(DtBcSDITXPLL* pBc);
#endif  // #ifndef __DT_BC_SDITXPLL_H
