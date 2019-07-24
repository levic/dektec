//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtVector.h *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

#ifndef __DT_VECTOR_H
#define __DT_VECTOR_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Defines / Constants +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Default grow size of 16-elements
#define DT_VECTOR_DEFAULT_GROWSIZE  16

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVector -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Common data for all vector types
#define  DT_VECTOR_COMMON_DATA                                                           \

// The vector data struct is hidden and defined in DtVector.c
typedef struct  _DtVector  DtVector;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public DtVector functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void*  DtVector_At(const DtVector*, Int);
void*  DtVector_Back(const DtVector*);
void*  DtVector_Begin(const DtVector*);
void*  DtVector_Next(const DtVector*, void*);
void*  DtVector_End(const DtVector*);
void  DtVector_Cleanup(DtVector*);
DtStatus  DtVector_Clear(DtVector*);
DtVector*  DtVector_Create(Int  Capacity, Int  ElemenSize, Int  GrowSize);
void*  DtVector_Data(const DtVector*);
DtStatus  DtVector_Erase(DtVector*, Int);
void*  DtVector_Front(const DtVector*);
void*  DtVector_PopBack(DtVector*);
DtStatus  DtVector_PopFront(DtVector*, void*);
DtStatus  DtVector_PushBack(DtVector*, const void*);
DtStatus  DtVector_Reserve(DtVector*, Int);
DtStatus  DtVector_Resize(DtVector*, Int);
DtStatus  DtVector_Set(const DtVector*, Int, const void*);
Int  DtVector_Size(const DtVector*);

#endif // #ifndef __DT_VECTOR_H
