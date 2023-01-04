// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtVectorDfChUser.h *#*#*#*#*#*#*#*#*# (C) 2022 DekTec
//
// 
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2022 DekTec Digital Video B.V.
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

#ifndef __DT_VECTOR_DF_CH_USER_H
#define __DT_VECTOR_DF_CH_USER_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtVector.h"

// Forward 
typedef struct DtVector DtVectorDfChUser;

// .-.-.-.-.-.-.-.-.-.-.-.-.- Public DtVectorDfChUser functions -.-.-.-.-.-.-.-.-.-.-.-.-.
DtDfChUser* DtVectorDfChUser_At(const DtVectorDfChUser*, Int);
DtStatus DtVectorDfChUser_Clear(DtVectorDfChUser*);
DtVectorDfChUser* DtVectorDfChUser_Create(Int Capacity, Int GrowSize);
void DtVectorDfChUser_Cleanup(DtVectorDfChUser*);
DtStatus DtVectorDfChUser_Erase(DtVectorDfChUser*, Int);
DtStatus DtVectorDfChUser_PushBack(DtVectorDfChUser*, DtDfChUser*);
DtStatus DtVectorDfChUser_Set(const DtVectorDfChUser*, Int, DtDfChUser*);
Int DtVectorDfChUser_Size(const DtVectorDfChUser*);

#endif // #ifndef __DT_VECTOR_DF_CH_USER_H