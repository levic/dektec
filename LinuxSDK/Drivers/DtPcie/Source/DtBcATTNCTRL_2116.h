//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcATTNCTRL_2116.h *#*#*#*#*#*#*#*#*#*# (C) 2021 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2021 DekTec Digital Video B.V.
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

#ifndef __DT_BC_ATTNCTRL_2116_H
#define __DT_BC_ATTNCTRL_2116_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the AttenuatorsCtrl_2116 block (must match block ID)
#define DT_BC_ATTNCTRL_2116_NAME        "AttenuatorsCtrl2116"
#define DT_BC_ATTNCTRL_2116_SHORTNAME   "ATTNCTRL"

// MACRO: to init an block-controller-ID for the ATTNCTRL_2116-BC
#define DT_BC_ATTNCTRL_2116_INIT_ID(ID, ROLE, INSTANCE, UUID)                            \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_ATTNCTRL_2116_NAME, DT_BC_ATTNCTRL_2116_SHORTNAME, ROLE,     \
                                                                        INSTANCE, UUID); \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcATTNCTRL_2116 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcATTNCTRL_2116 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcATTNCTRL_2116
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    UInt8  m_FirstAttenuator;
    UInt8  m_MiddleAttenuator;
    UInt8  m_LastAttenuator;
}  DtBcATTNCTRL_2116;

//.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcATTNCTRL_2116 public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-

void  DtBcATTNCTRL_2116_Close(DtBc*);
DtBcATTNCTRL_2116*  DtBcATTNCTRL_2116_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcATTNCTRL_2116 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcATTNCTRL_2116 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an ATTNCTRL_2116 Block
typedef struct _DtIoStubBcATTNCTRL_2116
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcATTNCTRL_2116;

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcATTNCTRL_2116 public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcATTNCTRL_2116_Close(DtIoStub*);
DtIoStubBcATTNCTRL_2116*  DtIoStubBcATTNCTRL_2116_Open(DtBc*);

#endif  // #ifndef __DT_BC_ATTNCTRL_2116_H
