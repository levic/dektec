// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIPSECG.h *#*#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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

#ifndef __DT_BC_IPSECG_H
#define __DT_BC_IPSECG_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the IpSecurityGuard block (must match block ID)
#define DT_BC_IPSECG_NAME        "IpSecurityGuard"
#define DT_BC_IPSECG_SHORTNAME   "IPSECG"

// MACRO: to init an block-controller-ID for the IPSECG-BC
#define DT_BC_IPSECG_INIT_ID(ID, ROLE, INSTANCE, UUID)                                   \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_IPSECG_NAME, DT_BC_IPSECG_SHORTNAME, ROLE, INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIPSECG definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSECG -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcIPSECG
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    DtFastMutex  m_AccessMutex;     // Access protection for IPSECG

}  DtBcIPSECG;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSECG public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcIPSECG_Close(DtBc*);
DtBcIPSECG*  DtBcIPSECG_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);
DtStatus  DtBcIPSECG_Challenge(DtBcIPSECG*, const UInt32  Challenge[3], 
                                                                     UInt32  Response[3]);
DtStatus  DtBcIPSECG_Check(DtBcIPSECG*, const UInt32*, Int  NumWords);
DtStatus  DtBcIPSECG_GetStatus(DtBcIPSECG*, Int64* pBoardId, Bool* pIsBusy, Bool*  pIsOk);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIPSECG definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIPSECG -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an IpSecurityGuard Block
typedef struct _DtIoStubBcIPSECG
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcIPSECG;

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIPSECG public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcIPSECG_Close(DtIoStub*);
DtIoStubBcIPSECG*  DtIoStubBcIPSECG_Open(DtBc*);


#endif  // #ifndef __DT_BC_IPSECG_H