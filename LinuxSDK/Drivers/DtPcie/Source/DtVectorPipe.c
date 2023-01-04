// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtVectorPipe.c *#*#*#*#*#*#*#*#*#*# (C) 2021 DekTec
//
#include "DtVectorPipe.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtVectorPipe implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_At -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtPipe* DtVectorPipe_At(const DtVectorPipe* pTheVector, Int Index)
{
    DtPipe** ppPipe = NULL;
    DT_ASSERT(pTheVector != NULL);
    ppPipe = (DtPipe**)DtVector_At((DtVector*)pTheVector, Index);
    return (ppPipe != NULL ? *ppPipe : NULL);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_Create -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtVectorPipe* DtVectorPipe_Create(Int Capacity, Int GrowSize)
{
    // Vector will hold Pipe-pointers
    return (DtVectorPipe*)DtVector_Create(Capacity, sizeof(DtPipe*), GrowSize);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_Cleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtVectorPipe_Cleanup(DtVectorPipe* pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    DtVector_Cleanup((DtVector*)pTheVector);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_SetEmpty -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorPipe_SetEmpty(DtVectorPipe* pTheVector, Int Index)
{
    DtPipe** ppPipe = NULL;
    DT_ASSERT(pTheVector != NULL);
    ppPipe = (DtPipe**)DtVector_At((const DtVector*)pTheVector, Index);
    DT_ASSERT(ppPipe != NULL);
    if (ppPipe != NULL)
        *ppPipe = NULL;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_FindEmpty -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int DtVectorPipe_FindEmpty(DtVectorPipe* pTheVector, const Int MaxPipes)
{
    Int i = 0, Size = 0;
    DT_ASSERT(pTheVector != NULL);

    Size = DtVectorPipe_Size(pTheVector);

    // Iterate over functions looking for one with the specified type and role
    for (i = 0; i < Size; i++)
    {
        DtPipe* pPipe = DtVectorPipe_At(pTheVector, i);
        if (pPipe == NULL)
            return i;
    }
    if (Size < MaxPipes)
    {
        // Add new entry
        DtVectorPipe_PushBack(pTheVector, NULL);
        DT_ASSERT(Size + 1 == DtVectorPipe_Size(pTheVector));
        return Size;
    }
    return -1;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_FindPipeIndex -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int DtVectorPipe_FindPipeIndex(const DtVectorPipe* pTheVector, DtPipe* pPipe)
{
    Int i = 0, Size = 0;
    DT_ASSERT(pTheVector != NULL);

    Size = DtVectorPipe_Size(pTheVector);

    // Iterate over functions looking for one with the specified type and role
    for (i = 0; i < Size; i++)
    {
        DtPipe* pPipe2 = DtVectorPipe_At(pTheVector, i);
        if (pPipe == pPipe2)
            return i;
    }
    return -1;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_Erase -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorPipe_Erase(DtVectorPipe* pTheVector, Int Index)
{
    return DtVector_Erase((DtVector*)pTheVector, Index);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_PushBack -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorPipe_PushBack(DtVectorPipe* pTheVector, const DtPipe* pPipe)
{
    return DtVector_PushBack((DtVector*)pTheVector, &pPipe);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_Begin -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtPipe* DtVectorPipe_Begin(const DtVectorPipe* pTheVector)
{
    return DtVector_Begin((const DtVector*)pTheVector);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_Next -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtPipe* DtVectorPipe_Next(const DtVectorPipe* pTheVector, DtPipe* pPipeCurrent)
{
    return DtVector_Next((const DtVector*)pTheVector, (void*)pPipeCurrent);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_End -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtPipe* DtVectorPipe_End(const DtVectorPipe* pTheVector)
{
    return DtVector_End((const DtVector*)pTheVector);
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_Reserve -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorPipe_Reserve(DtVectorPipe* pTheVector, Int NewCapacity)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Reserve((DtVector*)pTheVector, NewCapacity);
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_Resize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorPipe_Resize(DtVectorPipe* pTheVector, Int  NewSize)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Resize((DtVector*)pTheVector, NewSize);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_Set -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVectorPipe_Set(const DtVectorPipe* pTheVector, Int  Index, DtPipe* pPipe)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Set((DtVector*)pTheVector, Index, &pPipe);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_Size -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtVectorPipe_Size(const DtVectorPipe* pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Size((DtVector*)pTheVector);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVectorPipe_Clear -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVectorPipe_Clear(DtVectorPipe* pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    return DtVector_Clear((DtVector*)pTheVector);
}