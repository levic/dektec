//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCore_INTERRUPT.c *#*#*#*#*#*#*#*#*# (C) 2017 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2017 DekTec Digital Video B.V.
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
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_INTERRUPT implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to get the INT-CF from a DtCore object
#define CF_INT(pCore)         (pCore->m_pCfInt)

// MACRO with default precondition checks for the DtCfInt core-function
#define CORE_CFINT_DEFAULT_PRECONDITIONS(pCore)                                          \
    CORE_DEFAULT_PRECONDITIONS(pCore);                                                   \
    DT_ASSERT(CF_INT(pCore) != NULL)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_INTERRUPT - Public fuctions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_INTERRUPT_Register -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_INTERRUPT_Register(DtCore*  pCore, const DtIntHandlerRegData*  pRegData)
{
    // Sanity checks
    CORE_CFINT_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pRegData!=NULL && pRegData->m_Size==sizeof(DtBcIntHandlerRegData));

    // Check we have an interrupt function
    if (CF_INT(pCore) == NULL)
        return DT_STATUS_FAIL;

    return DtCfInt_HandlerRegister(CF_INT(pCore), (DtBcIntHandlerRegData*)pRegData);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_INTERRUPT_Unregister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCore_INTERRUPT_Unregister(DtCore* pCore, Int  Index, const DtBc*  pBc)
{
    // Sanity checks
    CORE_CFINT_DEFAULT_PRECONDITIONS(pCore);

    // Check we have an interrupt function
    if (pCore->m_pCfInt == NULL)
        return DT_STATUS_FAIL;

    return DtCfInt_HandlerUnregister(CF_INT(pCore), Index, pBc);
}
