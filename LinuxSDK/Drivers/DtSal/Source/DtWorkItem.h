//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtWorkItem.h *#*#*#*#*#*#*#*# (C) 2011-2016 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2016 DekTec Digital Video B.V.
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

#ifndef DT_WORKITEM_H
#define DT_WORKITEM_H


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtWorkItemArgs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct _DtWorkItemArgs
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
} DtWorkItemArgs;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- pDtWorkItemWorker -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef void (*pDtWorkItemWorker)(DtWorkItemArgs* pArgs);

typedef struct _DtWorkItemStruct{
#ifdef WINBUILD
#ifdef USES_NDIS
    NDIS_WORK_ITEM  m_NdisWorkItem;
#else
    PIO_WORKITEM  m_pIoWorkItem;
#endif // USES_NDIS
#else
   struct work_struct  m_Work;
#endif
} DtWorkItemStruct;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct _DtWorkItem
{
    pDtWorkItemWorker  m_pWorker;
    Bool  m_QueueIfRunning;
    volatile Int  m_State;
    DtWorkItemArgs  m_Args;
    DtWorkItemArgs  m_QueuedArgs;
#ifdef _DEBUG
    Bool  m_SchedulingEnabled;
#endif
    Bool  m_Running;
    DtWorkItemStruct  m_WorkStruct;
    DtDvcObject*  m_pDevice;
} DtWorkItem;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public API -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtWorkItemInit(DtWorkItem* pWorkItem, pDtWorkItemWorker pWorker, 
                                               Bool QueueIfRunning, DtDvcObject* pDevice);
DtStatus  DtWorkItemSchedule(DtWorkItem* pWorkItem, DtWorkItemArgs* pArgs);
void  DtWorkItemWaitForCompletion(DtWorkItem* pWorkItem);

#endif // DT_WORKITEM_H
