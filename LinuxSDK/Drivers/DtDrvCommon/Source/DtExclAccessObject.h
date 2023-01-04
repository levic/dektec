//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtExclAccessObject.h *#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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


#ifndef __DT_EXCL_ACCESS_OBJECT_H
#define __DT_EXCL_ACCESS_OBJECT_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtDrvCommon.h>

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtExclAccessObjectType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef enum  _DtExclAccessObjectType
{
    DT_EXCL_OBJECT_TYPE_IS_FILE,
#ifdef USE_DF_EXCL_ACCESS
    DT_EXCL_OBJECT_TYPE_IS_DF,
#endif
}  DtExclAccessObjectType;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtExclAccessObject -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtExclAccessObject
{
    DtExclAccessObjectType  m_Type;     // Type of owner
    union
    {
        DtFileObject  m_File;
#ifdef USE_DF_EXCL_ACCESS
        void*  m_pDf;
#endif
    }  m_Owner;                         // THE owner object
}  DtExclAccessObject;

#ifdef USE_DF_EXCL_ACCESS

#define DECL_EXCL_ACCESS_OBJECT_DF(ExclObj, pDf)                                         \
    DtExclAccessObject  ExclObj;                                                         \
    do { ExclObj.m_Type = DT_EXCL_OBJECT_TYPE_IS_DF; ExclObj.m_Owner.m_pDf = (DtDf*)pDf; } while(0)*/
#endif
#define DECL_EXCL_ACCESS_OBJECT_FILE(ExclObj, pFile)                                     \
    DtExclAccessObject  ExclObj;                                                         \
    do { ExclObj.m_Type = DT_EXCL_OBJECT_TYPE_IS_FILE; ExclObj.m_Owner.m_File = *pFile; } while(0)

//-.-.-.-.-.-.-.-.-.-.-.-.- DtExclAccessObject - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-
Bool  DtExclAccessObject_Compare(const DtExclAccessObject*, const DtExclAccessObject*);


#endif // #ifndef __DT_EXCL_ACCESS_OBJECT_H

