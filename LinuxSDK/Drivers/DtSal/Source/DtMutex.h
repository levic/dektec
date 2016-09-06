//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtMutex.h *#*#*#*#*#*#*#*#*# (C) 2010-2016 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2016 DekTec Digital Video B.V.
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

#ifndef  __DT_MUTEX_H
#define  __DT_MUTEX_H

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtFastMutex +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

typedef struct _DtFastMutex
{
#ifdef WINBUILD
    FAST_MUTEX  m_FastMutex;
#else
    struct semaphore  m_Semaphore;
#endif
} DtFastMutex;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

void  DtFastMutexInit(DtFastMutex* pDtFastMutex);
DtStatus  DtFastMutexAcquire(DtFastMutex* pDtFastMutex);
void  DtFastMutexRelease(DtFastMutex* pDtFastMutex);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtMutex +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

typedef struct _DtMutex
{
#ifdef WINBUILD
    KMUTEX  m_KMutex;
#else
    struct semaphore  m_Semaphore;
#endif
} DtMutex;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

void  DtMutexInit(DtMutex* pDtMutex);
DtStatus  DtMutexAcquire(DtMutex* pDtMutex, Int TimeoutMS);
void  DtMutexRelease(DtMutex* pDtMutex);


#endif  // __DT_MUTEX_H
