//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtExclAccessObject.c *#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtExclAccessObject.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtExclAccessObject implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtExclAccessObject - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtExclAccessObject_Compare -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtExclAccessObject_Compare(const DtExclAccessObject*  pObj1, 
                                                         const DtExclAccessObject*  pObj2)
{
    if (pObj1 == pObj2)
        return TRUE;
    if (pObj1->m_Type != pObj2->m_Type)
        return FALSE;

    switch(pObj1->m_Type)
    {
    case DT_EXCL_OBJECT_TYPE_IS_FILE:
        // Use the file compare 
        return DtFileCompare(&pObj1->m_Owner.m_File, &pObj2->m_Owner.m_File);
#ifdef USE_DF_EXCL_ACCESS
    case DT_EXCL_OBJECT_TYPE_IS_DF:
        // Simple compare the pointers
        return (pObj1->m_Owner.m_pDf == pObj2->m_Owner.m_pDf);
#endif
    default:
        DT_ASSERT(FALSE);
        return FALSE;
    }
}