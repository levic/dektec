//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDpc.h *#*#*#*#*#*#*#*#*#* (C) 2010-2012 DekTec
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

#ifndef DT_DPC_H
#define DT_DPC_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDpcArgs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtDpcArgs
{
    void*  m_pContext;
    union
    {
        struct
        {
            UInt32  m_UInt32_1;
            UInt32  m_UInt32_2;
        };
        UInt64  m_UInt64;
    } m_Data1;
    union
    {
        struct
        {
            UInt32  m_UInt32_1;
            UInt32  m_UInt32_2;
        };
        UInt64  m_UInt64;
    } m_Data2;
} DtDpcArgs;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- pDtDpcWorker -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef void (*pDtDpcWorker)(DtDpcArgs* pArgs);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtDpc
{
    pDtDpcWorker  m_pWorker;
    Bool  m_QueueIfRunning;
    Int  m_State;
    DtDpcArgs  m_Args;
    DtDpcArgs  m_QueuedArgs;
#ifdef _DEBUG
    Bool  m_SchedulingEnabled;
#endif
#ifdef WINBUILD
    KDPC  m_Kdpc;
#else
    struct tasklet_struct  m_Tasklet;
#endif
} DtDpc;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public API -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtDpcInit(DtDpc* pDpc, pDtDpcWorker pWorker, Bool QueueIfRunning);
DtStatus  DtDpcSchedule(DtDpc* pDpc, DtDpcArgs* pArgs);
void DtDpcWaitForCompletion(DtDpc* pDpc);

#endif // DT_DPC_H
