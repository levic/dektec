// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtVectorBc.h *#*#*#*#*#*#*#*# (C) 2017-2022 DekTec
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

#ifndef __DT_BC_VECTOR_H
#define __DT_BC_VECTOR_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtVector.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Defines / Constants +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorBc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public DtVectorBc functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
DtBc*  DtVectorBc_At(const DtVectorBc*, Int);
DtStatus  DtVectorBc_Clear(DtVectorBc*);
DtVectorBc*  DtVectorBc_Create(Int  Capacity, Int  GrowSize);
void  DtVectorBc_Cleanup(DtVectorBc*);
DtStatus  DtVectorBc_Erase(DtVectorBc*, Int);
DtBc*  DtVectorBc_FindByAddress(const DtVectorBc*, Int  Address);
DtBc*  DtVectorBc_FindByTypeAndRole(const DtVectorBc*, DtBcType, const char*);
DtStatus  DtVectorBc_PushBack(DtVectorBc*, DtBc*);
DtStatus  DtVectorBc_Set(const DtVectorBc*, Int, DtBc*);
Int  DtVectorBc_Size(const DtVectorBc*);

#endif // #ifndef __DT_BC_VECTOR_H

