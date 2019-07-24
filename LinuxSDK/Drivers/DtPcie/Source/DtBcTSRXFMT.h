//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcTSRXFMT.h *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2017 DekTec Digital Video B.V.
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

#ifndef __DT_BC_TSRXFMT_H
#define __DT_BC_TSRXFMT_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the TsRxFmtTrp block (must match block ID)
#define DT_BC_TSRXFMT_NAME        "TsRxFmtTrp"
#define DT_BC_TSRXFMT_SHORTNAME   "TSRXFMT"


// MACRO: to init an block-controller-ID for the TSRXFMT-BC
#define DT_BC_TSRXFMT_INIT_ID(ID, ROLE, INSTANCE, UUID)                                  \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_TSRXFMT_NAME, DT_BC_TSRXFMT_SHORTNAME, ROLE, INSTANCE, UUID);\
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcTSRXFMT definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTSRXFMT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcTSRXFMT
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached settings
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_OperationalMode;     // Current operational mode
    Int  m_PacketMode;          // Current packetization mode
    Int  m_SyncMode;            // Current synchronization mode
}  DtBcTSRXFMT;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTSRXFMT public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcTSRXFMT_Close(DtBc*);
DtBcTSRXFMT*  DtBcTSRXFMT_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcTSRXFMT_GetOperationalStatus(DtBcTSRXFMT*,  Int* pOpStatus);
DtStatus  DtBcTSRXFMT_GetPacketMode(DtBcTSRXFMT*,  Int* pPckMode);
DtStatus  DtBcTSRXFMT_GetSyncMode(DtBcTSRXFMT*,  Int* pSyncMode);
DtStatus  DtBcTSRXFMT_GetTsPacketSize(DtBcTSRXFMT*, Int* pTsPckSize);
DtStatus  DtBcTSRXFMT_SetOperationalMode(DtBcTSRXFMT*,  Int OpMode);
DtStatus  DtBcTSRXFMT_SetPacketMode(DtBcTSRXFMT*,  Int PckMode);
DtStatus  DtBcTSRXFMT_SetSyncMode(DtBcTSRXFMT*,  Int SyncMode);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcTSRXFMT definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTSRXFMT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an TSRXFMT Block
typedef struct _DtIoStubBcTSRXFMT
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcTSRXFMT;


//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTSRXFMT public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcTSRXFMT_Close(DtIoStub*);
DtIoStubBcTSRXFMT*  DtIoStubBcTSRXFMT_Open(DtBc*);


#endif  // #ifndef __DT_BC_TSRXFMT_H
