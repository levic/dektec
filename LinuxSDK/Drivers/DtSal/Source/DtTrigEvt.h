// .-.-.-.-.-.-.-.-.- DtEvent.h *#*#*#*#*#*#*#*#*# (C) 2010-2016 DekTec -.-.-.-.-.-.-.-.-.
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

#ifndef  __DT_TRIGEVT_H
#define  __DT_TRIGEVT_H

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtTrigEvt +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// This interface is used to trigger an user event from kernel space

typedef struct _DtTrigEvt
{
#ifdef WINBUILD
    PKEVENT m_pTrigEvt;
    Int m_Id;
#else
    Int m_Id;
    Int m_Pid;
    void* m_pVoid;
    struct task_struct* m_pTask;
#endif
    DtEvent* m_pKernelEvent;
    Bool m_KernelEvent;
    Bool m_Initialized;
} DtTrigEvt;

struct DtTrigEvtDriver;
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

DtStatus  DtTrigEvtInit(DtTrigEvt* pDtTrigEvt, DtTrigEvtDriver* pTrigEvtDriver);
DtStatus  DtTrigEvtSet(DtTrigEvt* pDtTrigEvt);
DtStatus  DtTrigEvtClose(DtTrigEvt* pDtTrigEvt);

#endif // __DT_TRIGEVT_H
