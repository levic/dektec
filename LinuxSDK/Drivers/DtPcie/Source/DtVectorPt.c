//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtVectorPt.c *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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
#include "DtPt.h"
#include "DtVectorPt.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtVectorPt implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

DtPt*  DtVectorPt_At(const DtVectorPt*  pTheVector, Int  Index)
{
    DtPt**  ppPt = NULL;
    DT_ASSERT(pTheVector != NULL);
    ppPt = (DtPt**)DtVector_At((DtVector*)pTheVector, Index);
    return (ppPt != NULL) ? *ppPt : NULL;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPt_Create -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtVectorPt*  DtVectorPt_Create(Int  Capacity, Int  GrowSize)
{
    // Vector will hold DF-pointers
    return (DtVectorPt*)DtVector_Create(Capacity, sizeof(DtPt*), GrowSize);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPt_Clear -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorPt_Clear(DtVectorPt*  pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Clear((DtVector*)pTheVector);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPt_Cleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtVectorPt_Cleanup(DtVectorPt*  pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    DtVector_Cleanup((DtVector*)pTheVector);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPt_Erase -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorPt_Erase(DtVectorPt*  pTheVector, Int  Index)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Erase((DtVector*)pTheVector, Index);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPt_FindByPort -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtPt*  DtVectorPt_FindByPort(const DtVectorPt*  pTheVector, Int PortIndex)
{
    Int  i=0, Size=0;
    DT_ASSERT(pTheVector != NULL);

    Size = DtVectorPt_Size(pTheVector);

    // Iterate over functions looking for one with the specified type and role
    for(i=0; i<Size; i++)
    {
        DtPt*  pPt = DtVectorPt_At(pTheVector, i);
        if (pPt==NULL || pPt->m_PortIndex!=PortIndex)
            continue;  // Not the one
        return pPt;  // Found the one we are looking for
    }
    return NULL;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPt_PushBack -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorPt_PushBack(DtVectorPt*  pTheVector, DtPt*  pPt)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_PushBack((DtVector*)pTheVector, &pPt);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPt_Reserve -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorPt_Reserve(DtVectorPt*  pTheVector, Int  NewCapacity)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Reserve((DtVector*)pTheVector, NewCapacity);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPt_Resize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorPt_Resize(DtVectorPt*  pTheVector, Int  NewSize)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Resize((DtVector*)pTheVector, NewSize);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPt_Set -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorPt_Set(const DtVectorPt*  pTheVector, Int  Index, DtPt*  pPt)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Set((DtVector*)pTheVector, Index, &pPt);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPt_Size -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtVectorPt_Size(const DtVectorPt*  pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Size((DtVector*)pTheVector);
}
