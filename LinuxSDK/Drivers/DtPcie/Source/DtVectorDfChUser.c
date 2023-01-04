// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtVectorDfChUser.c *#*#*#*#*#*#*#*#*# (C) 2022 DekTec
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtSal.h"
#include "DtDfCh.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtVectorDfChUser implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDfChUser_At -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfChUser*  DtVectorDfChUser_At(const DtVectorDfChUser* pTheVector, Int Index)
{
    DtDfChUser**  ppChUser = NULL;
    DT_ASSERT(pTheVector != NULL);
    ppChUser = (DtDfChUser**)DtVector_At((DtVector*)pTheVector, Index);
    return (ppChUser != NULL) ? *ppChUser : NULL;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDfChUser_Create -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtVectorDfChUser*  DtVectorDfChUser_Create(Int Capacity, Int GrowSize)
{
    // Vector will hold DtDfChUser-pointers
    return (DtVectorDfChUser*)DtVector_Create(Capacity, sizeof(DtDfChUser*), GrowSize);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDfChUser_Clear -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorDfChUser_Clear(DtVectorDfChUser* pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Clear((DtVector*)pTheVector);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDfChUser_Cleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtVectorDfChUser_Cleanup(DtVectorDfChUser* pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    DtVector_Cleanup((DtVector*)pTheVector);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDfChUser_Erase -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorDfChUser_Erase(DtVectorDfChUser* pTheVector, Int Index)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Erase((DtVector*)pTheVector, Index);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDfChUser_PushBack -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorDfChUser_PushBack(DtVectorDfChUser* pTheVector, DtDfChUser* pChUser)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_PushBack((DtVector*)pTheVector, &pChUser);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDfChUser_Reserve -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorDfChUser_Reserve(DtVectorDfChUser* pTheVector, Int NewCapacity)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Reserve((DtVector*)pTheVector, NewCapacity);
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDfChUser_Resize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorDfChUser_Resize(DtVectorDfChUser* pTheVector, Int NewSize)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Resize((DtVector*)pTheVector, NewSize);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDfChUser_Set -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorDfChUser_Set(
                       const DtVectorDfChUser* pTheVector, Int Index, DtDfChUser* pChUser)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Set((DtVector*)pTheVector, Index, &pChUser);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDfChUser_Size -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtVectorDfChUser_Size(const DtVectorDfChUser* pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Size((DtVector*)pTheVector);
}