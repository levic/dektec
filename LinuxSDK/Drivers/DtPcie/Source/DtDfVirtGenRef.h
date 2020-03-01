// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfVirtGenRef.h *#*#*#*#*#*#*#*#*#* (C) 2019 DekTec
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

#ifndef __DT_DF_VIRTGENREF_H
#define __DT_DF_VIRTGENREF_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtDfGenLockCtrl_CallbackFunction.h"

// Name + short-name for the VIRTGENREF function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_VIRTGENREF_NAME          "VirtualGenRef"
#define DT_DF_VIRTGENREF_SHORTNAME     "VIRTGENREF"

// MACRO: to init an driver-function-ID for the VIRTGENREF-DF
#define DT_DF_VIRTGENREF_INIT_ID(ID, ROLE, INSTANCE, UUID)                               \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_VIRTGENREF_NAME, DT_DF_VIRTGENREF_SHORTNAME, ROLE,           \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVirtGenRef definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVirtGenRef -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtDfVirtGenRef
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    DtDfGenLockCtrl*  m_pDfGenLockCtrl;   // Genlock controllers
    DtSpinLock  m_Lock;                   // Spinlock to Sof-handlers and operational mode

    // Cached values
    Int  m_PortIndex;                   // Port index
    Bool  m_GenRefEnabled;              // GenRef is enabled. Timestamps will be sent to
                                        // GenLockCtrl.
    Int  m_GenRefVidStd;                // GenRef video standard
    Int  m_GenRefSofOffset;             // GenRef start-of-frame offset
}  DtDfVirtGenRef;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfVirtGenRef_Close(DtDf*);
DtDfVirtGenRef*  DtDfVirtGenRef_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                             Int  Instance,  Int  Uuid, Bool  CreateStub);
DtStatus DtDfVirtGenRef_GetGenRefEnable(DtDfVirtGenRef* pDf, Bool* pEnable);
DtStatus DtDfVirtGenRef_GetGenRefSofOffset(DtDfVirtGenRef* pDf, Int* pSofOffset);
DtStatus DtDfVirtGenRef_GetGenRefVidStd(DtDfVirtGenRef* pDf, Int* pVidStd);
DtStatus DtDfVirtGenRef_SetGenRefEnable(DtDfVirtGenRef* pDf, Bool Enable);
DtStatus DtDfVirtGenRef_SetGenRefSofOffset(DtDfVirtGenRef* pDf, Int SofOffset);
DtStatus DtDfVirtGenRef_SetGenRefVidStd(DtDfVirtGenRef* pDf, Int VidStd);


#endif  // #ifndef __DT_DF_VIRTGENREF_H

