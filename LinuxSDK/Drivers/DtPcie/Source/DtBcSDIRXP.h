//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDIRXP.h *#*#*#*#*#*#*#*# (C) 2017-2018 DekTec
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

#ifndef __DT_BC_SDIRXP_H
#define __DT_BC_SDIRXP_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

#define DT_BC_SDIRXP_NAME           "SdiRxProtocol"
#define DT_BC_SDIRXP_SHORTNAME      "SDIRXP"

// MACRO: to init an block-controller-ID for the SDIRXP-BC
#define DT_BC_SDIRXP_INIT_ID(ID, TYPE, ROLE, INSTANCE, UUID)                             \
do                                                                                       \
{                                                                                        \
        DT_BC_INIT_ID(ID, DT_BC_SDIRXP_NAME, DT_BC_SDIRXP_SHORTNAME, ROLE, INSTANCE,     \
                                                                                 UUID);  \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIRXP definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcSDIRXP
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached values
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_OperationalMode;     // Operational mode
    Int  m_SdiRate;             // SD-rate (SD,HD, 3G, 6G, 12G)
    Int  m_MaxSdiRate;          // Maximum supported SDI-rate
}  DtBcSDIRXP;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXP public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcSDIRXP_Close(DtBc*);
DtBcSDIRXP*  DtBcSDIRXP_Open(Int  Address, DtCore*, DtPt*  pPt, DtBcType  Type,
                         const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcSDIRXP_GetMaxSdiRate(DtBcSDIRXP*,Int* pMaxSdiRate);
DtStatus  DtBcSDIRXP_GetOperationalMode(DtBcSDIRXP*, Int* pOpMode);
DtStatus  DtBcSDIRXP_GetSdiLineLock(DtBcSDIRXP*, Bool* pLineLock);
DtStatus  DtBcSDIRXP_GetSdiRate(DtBcSDIRXP*,Int* pSdiRate);
DtStatus DtBcSDIRXP_GetSdiStatus(DtBcSDIRXP* pBc, Int* pSdiLock, Int* pLineLock,
                                 Int* pValid, Int* pSdiRate, Int* pNumSymsHanc, 
                                 Int* pNumSymsVidVanc, Int* pNumLinesF1, Int* pNumLinesF2, 
                                 Int* pIsLevelB, UInt32* pPayloadId, Int* pFramePeriod);
DtStatus  DtBcSDIRXP_GetCrcErrorCount(DtBcSDIRXP*, UInt* pCrcErrorCnt);
DtStatus  DtBcSDIRXP_SetOperationalMode(DtBcSDIRXP*, Int OpMode);
DtStatus  DtBcSDIRXP_SetSdiRate(DtBcSDIRXP*, Int SdiRate);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIRXP definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXP -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an SDIRXP Block
typedef struct _DtIoStubBcSDIRXP
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcSDIRXP;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXP public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcSDIRXP_Close(DtIoStub*);
DtIoStubBcSDIRXP*  DtIoStubBcSDIRXP_Open(DtBc*);

#endif  // #ifndef __DT_BC_SDIRXP_H
