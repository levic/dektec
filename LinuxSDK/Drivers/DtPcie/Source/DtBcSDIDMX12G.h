// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDIDMX12G.h *#*#*#*#*#*#*#*#*#* (C) 2019 DekTec
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

#ifndef __DT_BC_SDIDMX12G_H
#define __DT_BC_SDIDMX12G_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the Switch block (must match block ID)
#define DT_BC_SDIDMX12G_NAME        "SdiDemux12G"
#define DT_BC_SDIDMX12G_SHORTNAME   "SDIDMX12G"

// MACRO: to init an block-controller-ID for the SDIDMX12G-BC
#define DT_BC_SDIDMX12G_INIT_ID(ID, ROLE, INSTANCE, UUID)                                \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_SDIDMX12G_NAME, DT_BC_SDIDMX12G_SHORTNAME, ROLE, INSTANCE,   \
                                                                                 UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIDMX12G definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIDMX12G -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcSDIDMX12G
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    // Cached settings
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_OperationalMode;     // Current operational mode
    Int  m_SdiRate;             // Current SDI-rate
}  DtBcSDIDMX12G;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIDMX12G public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcSDIDMX12G_Close(DtBc*);
DtBcSDIDMX12G*  DtBcSDIDMX12G_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcSDIDMX12G_GetOperationalMode(DtBcSDIDMX12G* pBc,  Int* pOpMode);
DtStatus  DtBcSDIDMX12G_GetSdiRate(DtBcSDIDMX12G* pBc, Int* pSdiRate);
DtStatus  DtBcSDIDMX12G_SetOperationalMode(DtBcSDIDMX12G* pBc,  Int OpMode);
DtStatus  DtBcSDIDMX12G_SetSdiRate(DtBcSDIDMX12G* pBc, Int SdiRate);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIDMX12G definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIDMX12G -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an SDIDMX12G Block
typedef struct _DtIoStubBcSDIDMX12G
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcSDIDMX12G;


// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIDMX12G public functions -.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcSDIDMX12G_Close(DtIoStub*);
DtIoStubBcSDIDMX12G*  DtIoStubBcSDIDMX12G_Open(DtBc*);


#endif  // #ifndef __DT_BC_SDIDMX12G_H