// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQDEMOD.h *#*#*#*#*#*#*#*#*#*# (C) 2022 DekTec
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

#ifndef __DT_BC_IQDEMOD_H
#define __DT_BC_IQDEMOD_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the IqDemod block (must match block ID)
#define DT_BC_IQDEMOD_NAME        "IqDemodulator"
#define DT_BC_IQDEMOD_SHORTNAME   "IQDEMOD"

// MACRO: to init an block-controller-ID for the IQDEMOD-BC
#define DT_BC_IQDEMOD_INIT_ID(ID, ROLE, INSTANCE, UUID)                                  \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_IQDEMOD_NAME, DT_BC_IQDEMOD_SHORTNAME, ROLE, INSTANCE, UUID);\
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQDEMOD definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDEMOD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcIQDEMOD
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached settings
    Bool  m_BlockEnabled;
    Int  m_OperationalMode;
    Bool  m_Bypass;
    Int  m_NcoPhaseIncr;
}  DtBcIQDEMOD;

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDEMOD public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

void  DtBcIQDEMOD_Close(DtBc*);
DtBcIQDEMOD*  DtBcIQDEMOD_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);
DtStatus DtBcIQDEMOD_GetOperationalMode(DtBcIQDEMOD* pBc, Int* pOpMode);
DtStatus DtBcIQDEMOD_GetNcoPars(DtBcIQDEMOD*, Int*, Int*);
DtStatus DtBcIQDEMOD_SetOperationalMode(DtBcIQDEMOD* pBc, Int OpMode);
DtStatus DtBcIQDEMOD_SetNcoPars(DtBcIQDEMOD*, Int, Int);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQDEMOD definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDEMOD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an IQDEMOD Block
typedef struct _DtIoStubBcIQDEMOD
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcIQDEMOD;

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDEMOD public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcIQDEMOD_Close(DtIoStub*);
DtIoStubBcIQDEMOD*  DtIoStubBcIQDEMOD_Open(DtBc*);

#endif  // #ifndef __DT_BC_IQDEMOD_H
