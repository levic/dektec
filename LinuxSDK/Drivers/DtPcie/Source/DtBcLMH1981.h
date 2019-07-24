//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcLMH1981.h *#*#*#*#*#*#*#*#*#*# (C) 2019 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2019 DekTec Digital Video B.V.
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

#ifndef __DT_BC_LMH1981_H
#define __DT_BC_LMH1981_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"
#include "DtDfGenLockCtrl_CallbackFunction.h"

// Name and short-name of the Lmh1981 block (must match block ID)
#define DT_BC_LMH1981_NAME        "Lmh1981Ctrl"
#define DT_BC_LMH1981_SHORTNAME   "LMH1981"

// MACRO: to init an block-controller-ID for the LMH1981-BC
#define DT_BC_LMH1981_INIT_ID(ID, ROLE, INSTANCE, UUID)                                  \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_LMH1981_NAME, DT_BC_LMH1981_SHORTNAME, ROLE,                 \
                                                                        INSTANCE, UUID); \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcLMH1981 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

typedef  struct _DtBcLMH1981_Status
{
    Bool  m_Valid;          // If true other fields are valid
    Int  m_LinePeriod;      // Line period in ns
    Int  m_FramePeriod;     // Frame period in ns
    Int  m_NumLinesF1;      // Number of lines in Field 1
    Int  m_NumLinesF2;      // Number of lines in Field 2
    Int  m_VideoFormat;     // Video format
}  DtBcLMH1981_Status;


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcLMH1981
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached  
    Int  m_PortIndex;                   // Port index
    Bool  m_GenRefEnabled;              // GenRef is enabled. Timestamps will be sent to
                                        // GenLockCtrl.
    Int  m_GenRefVidStd;                // GenRef video standard

    // Interrupts related
    DtDpc  m_IntDpc;
    DtDfGenLockCtrl*  m_pDfGenLockCtrl;   // Genlock controllers
    DtSpinLock  m_Lock;                   // Spinlock to Sof-handlers and operational mode
}  DtBcLMH1981;

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLMH1981 public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

void  DtBcLMH1981_Close(DtBc*);
DtBcLMH1981*  DtBcLMH1981_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);
DtStatus  DtBcLMH1981_GetInputStatus(DtBcLMH1981* pBc, DtBcLMH1981_Status* pStatus);
DtStatus DtBcLMH1981_GetGenRefEnable(DtBcLMH1981* pBc, Bool* pEnable);
DtStatus DtBcLMH1981_GetGenRefVidStd(DtBcLMH1981* pBc, Int* pVidStd);
DtStatus DtBcLMH1981_SetGenRefEnable(DtBcLMH1981* pBc, Bool Enable);
DtStatus DtBcLMH1981_SetGenRefVidStd(DtBcLMH1981* pBc, Int VidStd);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcLMH1981 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLMH1981 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an LMH1981 Block
typedef struct _DtIoStubBcLMH1981
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcLMH1981;

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLMH1981 public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcLMH1981_Close(DtIoStub*);
DtIoStubBcLMH1981*  DtIoStubBcLMH1981_Open(DtBc*);

#endif  // #ifndef __DT_BC_LMH1981_H
