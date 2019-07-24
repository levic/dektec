//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtVectorDf.h *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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

#ifndef __DT_DF_VECTOR_H
#define __DT_DF_VECTOR_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtVector.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Defines / Constants +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public DtVectorDf functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
DtDf*  DtVectorDf_At(const DtVectorDf*, Int);
DtStatus  DtVectorDf_Clear(DtVectorDf*);
DtVectorDf*  DtVectorDf_Create(Int  Capacity, Int  GrowSize);
void  DtVectorDf_Cleanup(DtVectorDf*);
DtStatus  DtVectorDf_Erase(DtVectorDf*, Int);
DtDf*  DtVectorDf_FindByTypeAndRole(const DtVectorDf*, DtFunctionType,  const char*);
DtStatus  DtVectorDf_PushBack(DtVectorDf*, DtDf*);
DtStatus  DtVectorDf_Set(const DtVectorDf*, Int, DtDf*);
Int  DtVectorDf_Size(const DtVectorDf*);

#endif // #ifndef __DT_DF_VECTOR_H

