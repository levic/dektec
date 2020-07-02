// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcS12GTO3G.h *#*#*#*#*#*#*#*#*#*# (C) 2020 DekTec
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2020 DekTec Digital Video B.V.
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

#ifndef __DT_BC_S12GTO3G_H
#define __DT_BC_S12GTO3G_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the Switch block (must match block ID)
#define DT_BC_S12GTO3G_NAME         "Sdi12Gto3G"
#define DT_BC_S12GTO3G_SHORTNAME    "S12GTO3G"

// MACRO: to init an block-controller-ID for the S12GTO3G-BC
#define DT_BC_S12GTO3G_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_S12GTO3G_NAME, DT_BC_S12GTO3G_SHORTNAME, ROLE,               \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS12GTO3G definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS12GTO3G -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcS12GTO3G
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    // Cached settings
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_OperationalMode;     // Current operational mode
    Bool m_ScalingEnabled;      // Scaling enabled

}  DtBcS12GTO3G;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS12GTO3G public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcS12GTO3G_Close(DtBc*);
DtBcS12GTO3G*  DtBcS12GTO3G_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcS12GTO3G_GetOperationalMode(DtBcS12GTO3G* pBc,  Int* pOpMode);
DtStatus  DtBcS12GTO3G_GetScalingEnable(DtBcS12GTO3G* pBc, Int* pEnable);
DtStatus  DtBcS12GTO3G_SetOperationalMode(DtBcS12GTO3G* pBc,  Int OpMode);
DtStatus  DtBcS12GTO3G_SetScalingEnable(DtBcS12GTO3G* pBc, Int Enable);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcS12GTO3G definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS12GTO3G -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an S12GTO3G Block
typedef struct _DtIoStubBcS12GTO3G
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcS12GTO3G;

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS12GTO3G public functions -.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcS12GTO3G_Close(DtIoStub*);
DtIoStubBcS12GTO3G*  DtIoStubBcS12GTO3G_Open(DtBc*);


#endif  // #ifndef __DT_BC_S12GTO3G_H