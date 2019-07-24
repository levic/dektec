//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtVectorDf.c *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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
#include "DtSal.h"
#include "DtDf.h"
#include "DtVectorDf.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtVectorDf implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

DtDf*  DtVectorDf_At(const DtVectorDf*  pTheVector, Int  Index)
{
    DtDf**  ppDf = NULL;
    DT_ASSERT(pTheVector != NULL);
    ppDf = (DtDf**)DtVector_At((DtVector*)pTheVector, Index);
    return (ppDf != NULL) ? *ppDf : NULL;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDf_Create -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtVectorDf*  DtVectorDf_Create(Int  Capacity, Int  GrowSize)
{
    // Vector will hold DF-pointers
    return (DtVectorDf*)DtVector_Create(Capacity, sizeof(DtDf*), GrowSize);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDf_Clear -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorDf_Clear(DtVectorDf*  pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Clear((DtVector*)pTheVector);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDf_Cleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtVectorDf_Cleanup(DtVectorDf*  pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    DtVector_Cleanup((DtVector*)pTheVector);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDf_Erase -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorDf_Erase(DtVectorDf*  pTheVector, Int  Index)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Erase((DtVector*)pTheVector, Index);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDf_FindByTypeAndRole -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDf*  DtVectorDf_FindByTypeAndRole(const DtVectorDf*  pTheVector,
                                                 DtFunctionType  Type, const char*  pRole)
{
    Int  i=0, Size=0;
    DT_ASSERT(pTheVector != NULL);

    Size = DtVectorDf_Size(pTheVector);

    // Iterate over functions looking for one with the specified type and role
    for(i=0; i<Size; i++)
    {
        DtDf*  pDf = DtVectorDf_At(pTheVector, i);
        if (pDf==NULL || pDf->m_Type!=Type)
            continue;  // Not the one
        // Check role
        else if (!DtAnsiCharArrayIsEqual(pRole, pDf->m_Id.m_pRole))
            continue;  // Not the one
        return pDf;  // Found the one we are looking for
    }
    return NULL;


}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDf_PushBack -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorDf_PushBack(DtVectorDf*  pTheVector, DtDf*  pDf)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_PushBack((DtVector*)pTheVector, &pDf);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDf_Reserve -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorDf_Reserve(DtVectorDf*  pTheVector, Int  NewCapacity)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Reserve((DtVector*)pTheVector, NewCapacity);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDf_Resize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorDf_Resize(DtVectorDf*  pTheVector, Int  NewSize)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Resize((DtVector*)pTheVector, NewSize);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDf_Set -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorDf_Set(const DtVectorDf*  pTheVector, Int  Index, DtDf*  pDf)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Set((DtVector*)pTheVector, Index, &pDf);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorDf_Size -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtVectorDf_Size(const DtVectorDf*  pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Size((DtVector*)pTheVector);
}
