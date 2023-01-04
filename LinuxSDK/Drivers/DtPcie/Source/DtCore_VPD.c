// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCore_VPD.c *#*#*#*#*#*#*#*# (C) 2018-2022 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2017-2022 DekTec Digital Video B.V.
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtCoreIncludes.h"            // Core driver includes

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_VPD implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtDfVpd core-function
#define CORE_DFVPD_DEFAULT_PRECONDITIONS(pCore)                                          \
    CORE_DEFAULT_PRECONDITIONS(pCore);                                                   \
    DT_ASSERT(DF_VPD(pCore) != NULL)

// Helper macro to get the VPD-DF from a DtCore object
#define DF_VPD(pCore)         (pCore->m_pDfVpd)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_VPD - Public fuctions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_VPD_ReadItemRo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtCore_VPD_ReadItemRo(const DtCore* pCore, const char* pKeyword,
                                           UInt8*  pItemBuf, Int  BufSize, Int*  pNumRead)
{
    Int  KeyLen;
    // Sanity checks
    CORE_DFVPD_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pKeyword!=NULL && pItemBuf!=NULL && pNumRead!=NULL);

    if (DF_VPD(pCore) == NULL)
        return DT_STATUS_FAIL;

    // Determine keyword length
    KeyLen = DtAnsiCharArrayStrLength(pKeyword, DT_VPD_MAX_KEY_LENGTH);
    if (KeyLen <= 0)
        return DT_STATUS_FAIL;

    *pNumRead = 0;
    DtMemZero(pItemBuf, BufSize);
    return DtDfVpd_ItemRead(DF_VPD(pCore), DT_VPD_SECT_RO, KeyLen, pKeyword, 0, 
                                                           BufSize, pItemBuf, pNumRead);
}
