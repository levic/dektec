//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDITXP.h *#*#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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

#ifndef __DT_BC_SDITXP_H
#define __DT_BC_SDITXP_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the SdiTxProtocol block (must match block ID)
#define DT_BC_SDITXP_NAME           "SdiTxProtocol"
#define DT_BC_SDITXP_SHORTNAME      "SDITXP"
#define DT_BC_SDITXP6G12G_NAME      "SdiTxProtocol6G12G"
#define DT_BC_SDITXP6G12G_SHORTNAME "SDITXP6G12G"

// MACRO: to init an block-controller-ID for the SDITXP-BC
#define DT_BC_SDITXP_INIT_ID(ID, TYPE, ROLE, INSTANCE, UUID)                             \
do                                                                                       \
{                                                                                        \
    if (TYPE == DT_BLOCK_TYPE_SDITXP)                                                    \
        DT_BC_INIT_ID(ID, DT_BC_SDITXP_NAME, DT_BC_SDITXP_SHORTNAME, ROLE, INSTANCE,     \
                                                                                 UUID);  \
    else                                                                                 \
        DT_BC_INIT_ID(ID, DT_BC_SDITXP6G12G_NAME, DT_BC_SDITXP6G12G_SHORTNAME, ROLE,     \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXP definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcSDITXP
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached values
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_OperationalMode;     // Operational mode
    Int  m_SdiRate;             // SDI-rate
    Int  m_MaxSdiRate;          // Maximum supported SDI-rate
    Bool  m_ClampEnable;        // Clamp enabled
    Bool  m_AdpChecksumEnable;  // ADP checksum insertion enabled
    Bool  m_LineCrcEnable;      // Line CRC insertion enabled
    Bool  m_LevelA2BEnable;     // Level A to Level B convertion enabled
}  DtBcSDITXP;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcSDITXP_Close(DtBc*);
DtBcSDITXP*  DtBcSDITXP_Open(Int  Address, DtCore*, DtPt*  pPt, DtBcType  Type,
                         const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcSDITXP_GetClampedSymsCount(DtBcSDITXP*, UInt* pClampedCount);
DtStatus  DtBcSDITXP_GetGenerationMode(DtBcSDITXP*, Bool*  pClampEna, 
                                              Bool*  pAdpChecksumEna, Bool*  pLineCrcEna);
DtStatus  DtBcSDITXP_GetMaxSdiRate(DtBcSDITXP*, Int* pMaxSdiRate);
DtStatus  DtBcSDITXP_GetOperationalMode(DtBcSDITXP*, Int* pOpMode);
DtStatus  DtBcSDITXP_GetSdiRateAndLvlA2BEna(DtBcSDITXP*, Int* pSdiRate, Int* pLvlA2BEna);
DtStatus  DtBcSDITXP_SetGenerationMode(DtBcSDITXP*, Bool  ClampEna, 
                                                  Bool  AdpChecksumEna, Bool  LineCrcEna);
DtStatus  DtBcSDITXP_SetOperationalMode(DtBcSDITXP*, Int OpMode);
DtStatus  DtBcSDITXP_SetSdiRateAndLvlA2BEna(DtBcSDITXP*, Int SdiRate, Int LvlA2BEna);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDITXP definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXP -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an SdiTxTsSel Block
typedef struct _DtIoStubBcSDITXP
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcSDITXP;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXP public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcSDITXP_Close(DtIoStub*);
DtIoStubBcSDITXP*  DtIoStubBcSDITXP_Open(DtBc*);

#endif  // #ifndef __DT_BC_SDITXP_H
