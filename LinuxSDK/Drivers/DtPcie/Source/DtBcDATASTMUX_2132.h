//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcDATASTMUX_2132.h *#*#*#*#*#*#*#*#* (C) 2018 DekTec
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

#ifndef __DT_BC_DATASTMUX_2132_H
#define __DT_BC_DATASTMUX_2132_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the DataStMux block (must match block ID)
#define DT_BC_DATASTMUX_2132_NAME        "DataStMux"
#define DT_BC_DATASTMUX_2132_SHORTNAME   "DATASTMUX"

// MACRO: to init an block-controller-ID for the DATASTMUX_2132-BC
#define DT_BC_DATASTMUX_2132_INIT_ID(ID, ROLE, INSTANCE, UUID)                           \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_DATASTMUX_2132_NAME, DT_BC_DATASTMUX_2132_SHORTNAME,  ROLE,  \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDATASTMUX_2132 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATASTMUX_2132 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcDATASTMUX_2132
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    // Cached settings
    Int  m_Selection;       // Input selection
}  DtBcDATASTMUX_2132;

//-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATASTMUX_2132 public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcDATASTMUX_2132_Close(DtBc*);
DtBcDATASTMUX_2132*  DtBcDATASTMUX_2132_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcDATASTMUX_2132_GetSelection(DtBcDATASTMUX_2132*, Int* pSelect);
DtStatus  DtBcDATASTMUX_2132_SetSelection(DtBcDATASTMUX_2132*, Int Select);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDATASTMUX_2132 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDATASTMUX_2132 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an DATASTMUX_2132 Block
typedef struct _DtIoStubBcDATASTMUX_2132
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcDATASTMUX_2132;


//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDATASTMUX_2132 public functions -.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcDATASTMUX_2132_Close(DtIoStub*);
DtIoStubBcDATASTMUX_2132*  DtIoStubBcDATASTMUX_2132_Open(DtBc*);


#endif  // #ifndef __DT_BC_DATASTMUX_2132_H