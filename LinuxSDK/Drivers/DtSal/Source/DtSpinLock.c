//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtSpinLock.c *#*#*#*#*#*#*#*# (C) 2010-2016 DekTec
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtSal.h>


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtSpinLock +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtSpinLockInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtSpinLockInit(DtSpinLock* pSpinLock)
{
#ifdef WINBUILD
#ifdef USES_NDIS
    NdisAllocateSpinLock(&pSpinLock->m_SpinLock);
#else
    KeInitializeSpinLock(&pSpinLock->m_SpinLock);
#endif // USES_NDIS
#else
    spin_lock_init(&pSpinLock->m_SpinLock);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtSpinLockAcquire -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtSpinLockAcquire(DtSpinLock* pSpinLock)
{
#ifdef WINBUILD
#ifdef USES_NDIS
    NdisAcquireSpinLock(&pSpinLock->m_SpinLock);
#else
    KeAcquireSpinLock(&pSpinLock->m_SpinLock, &pSpinLock->m_OldIrql);
#endif // USES_NDIS
#else
    spin_lock_bh(&pSpinLock->m_SpinLock);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtSpinLockAcquireAtDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtSpinLockAcquireAtDpc(DtSpinLock* pSpinLock)
{
#ifdef WINBUILD
    DT_ASSERT(KeGetCurrentIrql() >= DISPATCH_LEVEL);
#ifdef USES_NDIS
    NdisAcquireSpinLock(&pSpinLock->m_SpinLock);
#else
    KeAcquireSpinLockAtDpcLevel(&pSpinLock->m_SpinLock);
#endif // USES_NDIS
#else
    spin_lock_bh(&pSpinLock->m_SpinLock);
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtSpinLockRelease -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtSpinLockRelease(DtSpinLock* pSpinLock)
{
#ifdef WINBUILD
#ifdef USES_NDIS
    NdisReleaseSpinLock(&pSpinLock->m_SpinLock);
#else
    KeReleaseSpinLock(&pSpinLock->m_SpinLock, pSpinLock->m_OldIrql);
#endif // USES_NDIS
#else
    spin_unlock_bh(&pSpinLock->m_SpinLock);
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtSpinLockReleaseFromDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtSpinLockReleaseFromDpc(DtSpinLock* pSpinLock)
{
#ifdef WINBUILD
    DT_ASSERT(KeGetCurrentIrql() >= DISPATCH_LEVEL);
#ifdef USES_NDIS
    NdisReleaseSpinLock(&pSpinLock->m_SpinLock);
#else
    KeReleaseSpinLockFromDpcLevel(&pSpinLock->m_SpinLock);
#endif // USES_NDIS
#else
    spin_unlock_bh(&pSpinLock->m_SpinLock);
#endif
}

#ifdef USES_NDIS  // Only used in windows builds
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtSpinLockFree -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtSpinLockFree(DtSpinLock* pSpinLock)
{
    NdisFreeSpinLock(&pSpinLock->m_SpinLock);
}
#endif
