//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtVectorBc.c *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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
#include "DtBc.h"
#include "DtVectorBc.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtVectorBc implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorBc_At -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBc*  DtVectorBc_At(const DtVectorBc*  pTheVector, Int  Index)
{
    DtBc**  ppBc = NULL;
    DT_ASSERT(pTheVector != NULL);
    ppBc = (DtBc**)DtVector_At((DtVector*)pTheVector, Index);
    return (ppBc != NULL) ? *ppBc : NULL;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorBc_Create -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtVectorBc*  DtVectorBc_Create(Int  Capacity, Int  GrowSize)
{
    // Vector will hold BC-pointers
    return (DtVectorBc*)DtVector_Create(Capacity, sizeof(DtBc*), GrowSize);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorBc_Clear -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorBc_Clear(DtVectorBc*  pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Clear((DtVector*)pTheVector);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorBc_Cleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtVectorBc_Cleanup(DtVectorBc*  pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    DtVector_Cleanup((DtVector*)pTheVector);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorBc_Erase -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorBc_Erase(DtVectorBc*  pTheVector, Int  Index)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Erase((DtVector*)pTheVector, Index);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorBc_FindByAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBc*  DtVectorBc_FindByAddress(const DtVectorBc*  pTheVector, Int  Address)
{
    Int  i=0, Size=0;
    DT_ASSERT(pTheVector != NULL);
    
    Size = DtVectorBc_Size(pTheVector);
    
    // Iterate over blocks looking for one with the specified address
    for(i=0; i<Size; i++)
    {
        DtBc*  pBc = DtVectorBc_At(pTheVector, i);
        if (pBc==NULL || pBc->m_Address!=Address)
            continue;  // Not the one
        // Found the one we are looking for
        return pBc;
    }
    return NULL;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorBc_FindByTypeAndRole -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBc*  DtVectorBc_FindByTypeAndRole(const DtVectorBc*  pTheVector,  DtBcType  Type, 
                                                                       const char*  pRole)
{
    Int  i=0, Size=0;
    DT_ASSERT(pTheVector != NULL);

    Size = DtVectorBc_Size(pTheVector);

    // Iterate over blocks looking for one with the specified type and role
    for(i=0; i<Size; i++)
    {
        DtBc*  pBc = DtVectorBc_At(pTheVector, i);
        if (pBc==NULL || pBc->m_Type!=Type)
            continue;  // Not the one
        // Check role
        else if (!DtAnsiCharArrayIsEqual(pRole, pBc->m_Id.m_pRole))
            continue;  // Not the one
        return pBc;  // Found the one we are looking for
    }
    return NULL;


}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorBc_PushBack -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorBc_PushBack(DtVectorBc*  pTheVector, DtBc*  pBc)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_PushBack((DtVector*)pTheVector, &pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorBc_Reserve -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorBc_Reserve(DtVectorBc*  pTheVector, Int  NewCapacity)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Reserve((DtVector*)pTheVector, NewCapacity);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorBc_Resize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorBc_Resize(DtVectorBc*  pTheVector, Int  NewSize)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Resize((DtVector*)pTheVector, NewSize);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorBc_Set -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorBc_Set(const DtVectorBc*  pTheVector, Int  Index, DtBc*  pBc)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Set((DtVector*)pTheVector, Index, &pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorBc_Size -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtVectorBc_Size(const DtVectorBc*  pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Size((DtVector*)pTheVector);
}