// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcCLKCNT.h *#*#*#*#*#*#*#*#*#*#* (C) 2023 DekTec
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

#ifndef __DT_BC_CLKCNT_H
#define __DT_BC_CLKCNT_H

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#include "DtBc.h"

// Name and short-name of the ClockCounter block (must match block ID)
#define DT_BC_CLKCNT_NAME        "ClockCounter"
#define DT_BC_CLKCNT_SHORTNAME   "CLKCNT"

// MACRO: to init an block-controller-ID for the CLKCNT-BC
#define DT_BC_CLKCNT_INIT_ID(ID, ROLE, INSTANCE, UUID)                                   \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_CLKCNT_NAME, DT_BC_CLKCNT_SHORTNAME, ROLE, INSTANCE, UUID);  \
}                                                                                        \
while (0)

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcCLKCNT definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCLKCNT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef  struct _DtBcCLKCNT
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached state
    Int m_ClockFreqHz;                // Frequency of the clock the counter runs on.

}  DtBcCLKCNT;

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCLKCNT public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

void DtBcCLKCNT_Close(DtBc*);
DtBcCLKCNT* DtBcCLKCNT_Open(Int Address, DtCore*, DtPt* pPt,
                                                const char* pRole, Int Instance, Int Uuid,
                                                Bool CreateStub);
DtStatus DtBcCLKCNT_GetTickCountAndFreq(
                                      DtBcCLKCNT*, UInt32* pTickCount, Int* pClockFreqHz);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcCLKCNT definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCLKCNT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Io-control stub for an CLKCNT Block
//
typedef struct _DtIoStubBcCLKCNT
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcCLKCNT;

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCLKCNT public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void DtIoStubBcCLKCNT_Close(DtIoStub*);
DtIoStubBcCLKCNT* DtIoStubBcCLKCNT_Open(DtBc*);

#endif // #ifndef __DT_BC_CLKCNT_H
