// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtTrigEvt.c *#*#*#*#*#*#*#*# (C) 2010-2021 DekTec
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

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtTrigEvt +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtTrigEvtInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtTrigEvtInit(DtTrigEvt* pDtTrigEvt, DtTrigEvtDriver* pTrigEvtDriver)
{
    if (pDtTrigEvt->m_Initialized)
        return DT_STATUS_IN_USE;
    if (pTrigEvtDriver->m_KernelEvent)
    {
        DT_ASSERT(pTrigEvtDriver->m_pKernelEvent != NULL);
        pDtTrigEvt->m_pKernelEvent = pTrigEvtDriver->m_pKernelEvent;
    }
    else
    {
#ifdef WINBUILD
        NTSTATUS Status;
        Status = ObReferenceObjectByHandle(pTrigEvtDriver->m_hEvent, SYNCHRONIZE, 
                     *ExEventObjectType, UserMode, (PVOID*)&pDtTrigEvt->m_pTrigEvt, NULL);
        if (Status == STATUS_OBJECT_TYPE_MISMATCH)
            return DT_STATUS_NOT_SUPPORTED;
        if (Status == STATUS_INVALID_HANDLE)
            return DT_STATUS_INVALID_PARAMETER;
        if (Status == STATUS_ACCESS_DENIED)
            return DT_STATUS_NOT_ENOUGH_RIGHTS;
        if (!NT_SUCCESS(Status))
            return DT_STATUS_FAIL;
#else
        pDtTrigEvt->m_pTask = get_current();
        pDtTrigEvt->m_Pid = pDtTrigEvt->m_pTask->pid;
#if defined(LIN32)
        pDtTrigEvt->m_pVoid = (char*)(UInt32)pTrigEvtDriver->m_pVoid;
#else
        pDtTrigEvt->m_pVoid = (char*)(UInt64)pTrigEvtDriver->m_pVoid;
#endif
#endif
        
    }
    pDtTrigEvt->m_KernelEvent = pTrigEvtDriver->m_KernelEvent;
    pDtTrigEvt->m_Id = pTrigEvtDriver->m_Id; 
    pDtTrigEvt->m_Initialized = TRUE;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtTrigEvtSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtTrigEvtSet(DtTrigEvt* pDtTrigEvt)
{
    if (!pDtTrigEvt->m_Initialized)
        return DT_STATUS_NOT_INITIALISED;

    if (pDtTrigEvt->m_KernelEvent)
    {
        DtEventSet(pDtTrigEvt->m_pKernelEvent);
        return DT_STATUS_OK;
    }
#ifdef WINBUILD
    DT_ASSERT(KeGetCurrentIrql() <= DISPATCH_LEVEL);
    KeSetEvent(pDtTrigEvt->m_pTrigEvt, 0, FALSE);
#else
    {
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,20,0) || defined(__SIGINFO)
        struct kernel_siginfo SigInfo;
        memset(&SigInfo, 0, sizeof(struct kernel_siginfo));
#else
        struct siginfo SigInfo;
        memset(&SigInfo, 0, sizeof(struct siginfo));
#endif
        SigInfo.si_signo = SIGNAL_TRIG_EVT_DRIVER;
        SigInfo.si_code = SI_QUEUE;
        //SigInfo.si_int = pDtTrigEvt->m_Id;
        SigInfo.si_ptr = pDtTrigEvt->m_pVoid;
        if (send_sig_info(SIGNAL_TRIG_EVT_DRIVER, &SigInfo, pDtTrigEvt->m_pTask) < 0)
            return DT_STATUS_FAIL;
    }
#endif
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtTrigEvtClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtTrigEvtClose(DtTrigEvt* pDtTrigEvt)
{
    if (!pDtTrigEvt->m_Initialized)
        return DT_STATUS_NOT_INITIALISED;
#ifdef WINBUILD
    if (!pDtTrigEvt->m_KernelEvent)
        ObDereferenceObject(pDtTrigEvt->m_pTrigEvt);
#else

#endif
    pDtTrigEvt->m_Initialized = FALSE;
    return DT_STATUS_OK;
}
