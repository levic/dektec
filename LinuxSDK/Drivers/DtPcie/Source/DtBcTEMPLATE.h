//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcTEMPLATE.h *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
// This file is not part of the PCIe driver. It can be used as template to create a new
// block controller. Replace "TEMPLATE" by the building block's shortname (all uppercase)
// and replace "Template" by the building block's (long) name.
// 

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018 DekTec Digital Video B.V.
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

#ifndef __DT_BC_TEMPLATE_H
#define __DT_BC_TEMPLATE_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the Template block (must match block ID)
#define DT_BC_TEMPLATE_NAME        "Template"
#define DT_BC_TEMPLATE_SHORTNAME   "TEMPLATE"

// MACRO: to init an block-controller-ID for the TEMPLATE-BC
#define DT_BC_TEMPLATE_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_TEMPLATE_NAME, DT_BC_TEMPLATE_SHORTNAME, ROLE,               \
                                                                        INSTANCE, UUID); \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcTEMPLATE definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTEMPLATE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcTEMPLATE
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached settings
    Bool  m_BlockEnabled;
    Int  m_OperationalMode;

    // Interrupts related
    DtDpc  m_IntDpc;
    DtEvent  m_DoneEvent;

}  DtBcTEMPLATE;

//.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcTEMPLATE public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-

void  DtBcTEMPLATE_Close(DtBc*);
DtBcTEMPLATE*  DtBcTEMPLATE_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);
DtStatus DtBcTEMPLATE_GetOperationalMode(DtBcTEMPLATE* pBc, Int* pOpMode);
DtStatus DtBcTEMPLATE_SetOperationalMode(DtBcTEMPLATE* pBc, Int OpMode);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcTEMPLATE definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTEMPLATE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an TEMPLATE Block
typedef struct _DtIoStubBcTEMPLATE
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcTEMPLATE;

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTEMPLATE public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcTEMPLATE_Close(DtIoStub*);
DtIoStubBcTEMPLATE*  DtIoStubBcTEMPLATE_Open(DtBc*);

#endif  // #ifndef __DT_BC_TEMPLATE_H
