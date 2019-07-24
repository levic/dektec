//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtVector.c *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2017 DekTec Digital Video B.V.
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
#include <DtDrvCommon.h>

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtVector implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO, to get address of element at a specific index
#define  DT_VECTOR_ELEM_AT(IDX)  (pTheVector->m_Elems + ((IDX) * pTheVector->m_ElemSize))

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// The vector struct
struct _DtVector
{
    Int  m_Size;            // Number of elements in the vector
    Int  m_Capacity;        // Maximum #elements the vector can hold
    Int  m_GrowSize;        // Grow size for vector when capacity is not sufficient
    Int  m_ElemSize;        // Size of the elements the vector holds
    char*  m_Elems;         // Buffer holding the vector elements
};


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_At -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void*  DtVector_At(const DtVector*  pTheVector, Int  Index)
{
    DT_ASSERT(pTheVector!=NULL && pTheVector->m_Size>=0 && pTheVector->m_ElemSize>0);
    if (pTheVector->m_Elems==NULL || Index<0 || Index>=pTheVector->m_Size)
        return  NULL;
    return (pTheVector->m_Elems + (Index * pTheVector->m_ElemSize));
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_Back -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void*  DtVector_Back(const DtVector*  pTheVector)
{
    DT_ASSERT(pTheVector!=NULL && pTheVector->m_Size>=0 && pTheVector->m_ElemSize>0);
    if (pTheVector->m_Size == 0)
        return NULL;
    DT_ASSERT(pTheVector->m_Elems != NULL);
    return DtVector_At(pTheVector, pTheVector->m_Size-1);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_Cleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtVector_Cleanup(DtVector*  pTheVector)
{
    if (pTheVector == NULL)
        return;     // Nothing todo

    // Free element buffer
    if (pTheVector->m_Elems != NULL)
        DtMemFreePool(pTheVector->m_Elems, COMMON_TAG);

    // Free vector object
    DtMemFreePool(pTheVector, COMMON_TAG);
    pTheVector = NULL;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_Clear -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVector_Clear(DtVector* pTheVector)
{
    DT_ASSERT(pTheVector!=NULL && pTheVector->m_Size>=0 && pTheVector->m_ElemSize>0);
    pTheVector->m_Size = 0;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtVector*  DtVector_Create(Int  Capacity, Int  ElemenSize, Int  GrowSize)
{
    DtStatus   Status = DT_STATUS_OK;
    DtVector*  pNewVector = NULL;

    DT_ASSERT(Capacity>=0 && ElemenSize>0);

    // Allocate memory for vector object
    pNewVector = (DtVector*)DtMemAllocPool(DtPoolNonPaged, sizeof(DtVector), COMMON_TAG);
    if (pNewVector == NULL)
        return  NULL; 

    // Init to 'safe' intial values
    pNewVector->m_Elems = NULL;
    pNewVector->m_ElemSize = ElemenSize;
    pNewVector->m_Capacity = 0;
    pNewVector->m_Size = 0;
    pNewVector->m_GrowSize = (GrowSize>0) ? GrowSize : DT_VECTOR_DEFAULT_GROWSIZE;

    // Allocate initial buffer
    Status = DtVector_Reserve(pNewVector, Capacity);
    if (!DT_SUCCESS(Status))
    {
        DtVector_Cleanup(pNewVector);
        return NULL;
    }
    
    // Return the newly created vector
    return pNewVector;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_Data -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void*  DtVector_Data(const DtVector* pTheVector)
{
    DT_ASSERT(pTheVector!=NULL && pTheVector->m_Size>=0 && pTheVector->m_ElemSize>0);
    return DtVector_Front(pTheVector);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_Erase -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVector_Erase(DtVector*  pTheVector, Int  Index)
{
    Int  NumToMove = 0;
    DT_ASSERT(pTheVector!=NULL && pTheVector->m_Size>=0 && pTheVector->m_ElemSize>0);

    if (pTheVector->m_Elems==NULL || Index<0 || Index>=pTheVector->m_Size)
        return  DT_STATUS_INVALID_PARAMETER;

    // Move existing elements over the one we are erasing
    NumToMove = pTheVector->m_Size - Index - 1;
    if (NumToMove>0)
        DtMemMove(DT_VECTOR_ELEM_AT(Index), DT_VECTOR_ELEM_AT(Index+1), 
                                                        NumToMove*pTheVector->m_ElemSize);

    // Update the size
    pTheVector->m_Size--;
    DT_ASSERT(pTheVector->m_Size>=0);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_Front -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void*  DtVector_Front(const DtVector*  pTheVector)
{
    DT_ASSERT(pTheVector!=NULL && pTheVector->m_Size>=0 && pTheVector->m_ElemSize>0);
    if (pTheVector->m_Size == 0)
        return NULL;
    DT_ASSERT(pTheVector->m_Elems != NULL);
    return pTheVector->m_Elems;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_PopBack -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void*  DtVector_PopBack(DtVector*  pTheVector)
{
    void*  pElem = NULL;

    DT_ASSERT(pTheVector!=NULL && pTheVector->m_Size>=0 && pTheVector->m_ElemSize>0);

    // Are there any elements to pop
    if (pTheVector->m_Size == 0)
        return NULL;
    DT_ASSERT(pTheVector->m_Elems != NULL);
    
    // Get the last element and decrease the size
    pElem = DtVector_Back(pTheVector);
    pTheVector->m_Size--;
    DT_ASSERT(pTheVector->m_Size >= 0);
    return pElem;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_PopFront -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVector_PopFront(DtVector*  pTheVector, void*  pElem)
{
    DT_ASSERT(pTheVector!=NULL && pTheVector->m_Size>=0 && pTheVector->m_ElemSize>0);
    
    // Must contain elements
    if (pTheVector->m_Elems==NULL || pTheVector->m_Size<=0)
        return DT_STATUS_BUF_TOO_SMALL;

    // Copy the element, we cannot return the pointer since content is overwritten
    DtMemCopy(pElem, DT_VECTOR_ELEM_AT(0), pTheVector->m_ElemSize);
       
    // Remove front element 
    pTheVector->m_Size--;
    if (pTheVector->m_Size > 0)
        DtMemMove(DT_VECTOR_ELEM_AT(0), DT_VECTOR_ELEM_AT(1), 
                                             pTheVector->m_ElemSize * pTheVector->m_Size);

    return DT_STATUS_OK;
}



//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_PushBack -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVector_PushBack(DtVector*  pTheVector, const void*  pNewElem)
{
    DtStatus  Status = DT_STATUS_OK;

    DT_ASSERT(pTheVector!=NULL && pTheVector->m_Size>=0 && pTheVector->m_ElemSize>0);
    
    // Check if the new element fits and we can grow the list
    if (pTheVector->m_Size>=pTheVector->m_Capacity && pTheVector->m_GrowSize<=0)
        return DT_STATUS_BUF_TOO_SMALL;
    else if (pTheVector->m_Size >= pTheVector->m_Capacity)
    {
        // Grow the list to fit new entry
        const Int  NewCapacity = pTheVector->m_Capacity + pTheVector->m_GrowSize;
        Status = DtVector_Reserve(pTheVector, NewCapacity);
        if (!DT_SUCCESS(Status))
            return Status;
    }
    // Add new element to list and update size
    DT_ASSERT(pTheVector->m_Elems != NULL);
    DT_ASSERT((pTheVector->m_Size+1) <= pTheVector->m_Capacity);
    DtMemCopy(DT_VECTOR_ELEM_AT(pTheVector->m_Size), pNewElem, pTheVector->m_ElemSize);
    // Update the size
    pTheVector->m_Size++;
    DT_ASSERT(pTheVector->m_Size>=0 && pTheVector->m_Size<=pTheVector->m_Capacity);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_Reserve -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVector_Reserve(DtVector*  pTheVector, Int  NewCapacity)
{
    Int  NewSize=0;
    char*  pNewElems = NULL;

    DT_ASSERT(pTheVector!=NULL && pTheVector->m_Size>=0 && pTheVector->m_ElemSize>0);
    DT_ASSERT(NewCapacity >= 0);

    // Nothing to change
    if (NewCapacity == pTheVector->m_Capacity)
        return DT_STATUS_OK;

    // Allocate memory for new elements and copy old ones
    if (NewCapacity > 0)
    {
        const Int  AllocSize = NewCapacity * pTheVector->m_ElemSize;
        pNewElems = (char*)DtMemAllocPool(DtPoolNonPaged, AllocSize, COMMON_TAG);
        if (pNewElems == NULL)
            return DT_STATUS_OUT_OF_MEMORY;
        DtMemZero(pNewElems, AllocSize);    // Init buffer to all zero's

        // Copy existing elems. NOTE: only the ones that still fit
        if (pTheVector->m_Size>0)
        {
            DT_ASSERT(pTheVector->m_Elems != NULL);
            NewSize = min(NewCapacity, pTheVector->m_Size);
            DtMemCopy(pNewElems, pTheVector->m_Elems, pTheVector->m_ElemSize*NewSize);
        }
    }

    // Free old elements
    if (pTheVector->m_Elems != NULL)
        DtMemFreePool(pTheVector->m_Elems, COMMON_TAG);

    // Set new list and set size and capacity
    DT_ASSERT((pNewElems!=NULL && NewCapacity>0) || (pNewElems==NULL && NewCapacity==0));
    pTheVector->m_Elems = pNewElems;
    pTheVector->m_Capacity = NewCapacity;
    pTheVector->m_Size = NewSize;

    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_Resize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVector_Resize(DtVector*  pTheVector, Int  NewSize)
{
    DtStatus  Status = DT_STATUS_OK;

    DT_ASSERT(pTheVector!=NULL && pTheVector->m_Size>=0 && pTheVector->m_ElemSize>0);
    DT_ASSERT(NewSize >= 0);

    // Enough capacity to hold new size?
    if (NewSize > pTheVector->m_Capacity)
    {
        Status = DtVector_Reserve(pTheVector, NewSize + pTheVector->m_GrowSize);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    pTheVector->m_Size = NewSize;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_Set -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtVector_Set(const DtVector*  pTheVector, Int  Index, const void*  pElem)
{
    void*  pDest = NULL;
    DT_ASSERT(pTheVector!=NULL && pTheVector->m_Size>=0 && pTheVector->m_ElemSize>0);

    // Must have been initialised
    if (pTheVector->m_Elems==NULL || Index>=pTheVector->m_Size)
        return DT_STATUS_BUF_TOO_SMALL;
    DT_ASSERT(Index>=0 && Index<pTheVector->m_Size);
    pDest = DtVector_At(pTheVector, Index);
    DT_ASSERT(pDest!=NULL && pDest>=pTheVector->m_Elems);
    DtMemCopy(pDest, pElem, pTheVector->m_ElemSize);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_Size -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtVector_Size(const DtVector*  pTheVector)
{
    DT_ASSERT(pTheVector != NULL);
    return (pTheVector->m_Elems!=NULL) ? pTheVector->m_Size : 0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_Begin -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void*  DtVector_Begin(const DtVector*  pTheVector)
{
    DT_ASSERT(pTheVector!=NULL && pTheVector->m_ElemSize>0);
    return pTheVector->m_Elems; 
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_Next -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void*  DtVector_Next(const DtVector*  pTheVector, void*  pCurrent)
{
    char*  pNext = NULL;
    char*  pBegin = NULL;
    char*  pEnd = NULL;

    DT_ASSERT(pTheVector!=NULL && pTheVector->m_ElemSize>0);
    pBegin = (char*)DtVector_End(pTheVector);
    pEnd = (char*)DtVector_End(pTheVector);
    DT_ASSERT(pCurrent>=pBegin && pCurrent<pEnd);

    // Compute position of next element
    pNext = (char*)pCurrent + pTheVector->m_ElemSize;
    DT_ASSERT(pNext >= pBegin);

    // Clip on the end
    return (pNext <= pEnd) ? pNext : pEnd;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector_End -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void*  DtVector_End(const DtVector*  pTheVector)
{
    DT_ASSERT(pTheVector!=NULL && pTheVector->m_ElemSize>0 && pTheVector->m_Size>=0);
    return pTheVector->m_Elems + (pTheVector->m_Size * pTheVector->m_ElemSize);
}