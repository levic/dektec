// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcAD9789ITF.h *#*#*#*#*#*#*#*#*#* (C) 2023 DekTec
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2023 DekTec Digital Video B.V.
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

#ifndef __DT_BC_AD9789ITF_H
#define __DT_BC_AD9789ITF_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the Ad9789Itf block (must match block ID)
#define DT_BC_AD9789ITF_NAME        "Ad9789Itf"
#define DT_BC_AD9789ITF_SHORTNAME   "AD9789ITF"

// MACRO: to init an block-controller-ID for the AD9789ITF-BC
#define DT_BC_AD9789ITF_INIT_ID(ID, ROLE, INSTANCE, UUID)                                \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_AD9789ITF_NAME, DT_BC_AD9789ITF_SHORTNAME, ROLE, INSTANCE,   \
                                                                               UUID);    \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcAD9789ITF definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD9789ITF -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcAD9789ITF
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached settings
    Bool  m_BlockEnabled;
    Int  m_OperationalMode;

}  DtBcAD9789ITF;

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD9789ITF public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

void  DtBcAD9789ITF_Close(DtBc*);
DtBcAD9789ITF*  DtBcAD9789ITF_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcAD9789ITF definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD9789ITF -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an AD9789ITF Block
typedef struct _DtIoStubBcAD9789ITF
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcAD9789ITF;

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD9789ITF public functions -.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcAD9789ITF_Close(DtIoStub*);
DtIoStubBcAD9789ITF*  DtIoStubBcAD9789ITF_Open(DtBc*);

#endif  // #ifndef __DT_BC_AD9789ITF_H
