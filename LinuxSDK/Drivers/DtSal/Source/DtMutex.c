//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtMutex.c *#*#*#*#*#*#*#*#*# (C) 2010-2012 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2012 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//  3. The source code may not be modified for the express purpose of enabling hardware
//     features for which no genuine license has been obtained.
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
#include <DtSal.h>


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtFastMutex +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtFastMutexInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtFastMutexInit(DtFastMutex* pDtFastMutex)
{
#ifdef WINBUILD
    ExInitializeFastMutex(&pDtFastMutex->m_FastMutex);
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)
    sema_init(&pDtFastMutex->m_Semaphore, 1);
#else
    init_MUTEX(&pDtFastMutex->m_Semaphore);
#endif
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtFastMutexAcquire -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtFastMutexAcquire(DtFastMutex* pDtFastMutex)
{
#ifdef WINBUILD
    ExAcquireFastMutex(&pDtFastMutex->m_FastMutex);
    return DT_STATUS_OK;
#else
    if (down_interruptible(&pDtFastMutex->m_Semaphore) != 0)
        return DT_STATUS_FAIL;
    return DT_STATUS_OK;
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtFastMutexRelease -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtFastMutexRelease(DtFastMutex* pDtFastMutex)
{
#ifdef WINBUILD
    ExReleaseFastMutex(&pDtFastMutex->m_FastMutex);
#else
    up(&pDtFastMutex->m_Semaphore);
#endif
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtMutex +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtMutexInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtMutexInit(DtMutex* pDtMutex)
{
#ifdef WINBUILD
    KeInitializeMutex(&pDtMutex->m_KMutex, 0);
#else
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 37)
    sema_init(&pDtMutex->m_Semaphore, 1);
#else
    init_MUTEX(&pDtMutex->m_Semaphore);
#endif
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtMutexAcquire -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtMutexAcquire(DtMutex* pDtMutex, Int TimeoutMS)
{
#ifdef WINBUILD
    NTSTATUS  Result;
    LARGE_INTEGER  Timeout;
    LARGE_INTEGER*  pTimeout;
    DT_ASSERT((KeGetCurrentIrql()<=APC_LEVEL && TimeoutMS>0) ||
                                    (KeGetCurrentIrql()<=DISPATCH_LEVEL && TimeoutMS<=0));

    if (TimeoutMS >= 0)
    {
        Timeout.QuadPart = (Int64)TimeoutMS * -10000;
        pTimeout = &Timeout;
    } else
        pTimeout = NULL;

    Result = KeWaitForSingleObject(&pDtMutex->m_KMutex, Executive, KernelMode, FALSE,
                                                                                pTimeout);
    if (Result != STATUS_SUCCESS)
    {
        if (Result == STATUS_TIMEOUT)
            return DT_STATUS_TIMEOUT;
        else 
            return DT_STATUS_FAIL;
    } else
        return DT_STATUS_OK;
#else
    Int  Result;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,26)
    if (TimeoutMS >= 0)
    {
        Int  Jiffies = msecs_to_jiffies(TimeoutMS);
        Result = down_timeout(&pDtMutex->m_Semaphore, Jiffies);
        if (Result != 0)
        {
            if (Result == -ETIME)
                return DT_STATUS_TIMEOUT;
            else
                return DT_STATUS_FAIL;
        }
    } else 
        down(&pDtMutex->m_Semaphore);
#else
    do
    {
        Result = down_trylock(&pDtMutex->m_Semaphore);
        if (Result != 0)
        {
            DtSleep(10);
            if (TimeoutMS >= 10)
                TimeoutMS -= 10;
            else
                TimeoutMS = 0;
        }
    } while (Result!=0 && TimeoutMS>0);
    if (Result != 0)
        return DT_STATUS_TIMEOUT;
#endif
    return DT_STATUS_OK;
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtMutexRelease -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtMutexRelease(DtMutex* pDtMutex)
{
#ifdef WINBUILD
    KeReleaseMutex(&pDtMutex->m_KMutex, FALSE);
#else
    up(&pDtMutex->m_Semaphore);
#endif
}
