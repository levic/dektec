//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcS2STATS_2132.h *#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

#ifndef __DT_BC_S2STATS_2132_H
#define __DT_BC_S2STATS_2132_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"


#define DT_BC_S2STATS_2132_FRAME_STATUS_BUF_SIZE  4096
typedef  struct _DtBcS2STATS_2132_FrameStatusBuffer
{
    DtS2Stats_2132_FrameStatus  m_Statistics[DT_BC_S2STATS_2132_FRAME_STATUS_BUF_SIZE];
    Int  m_ReadIdx;
    Int  m_WriteIdx;
    Bool  m_Overflow;
} DtBcS2STATS_2132_FrameStatusBuffer;

// Name and short-name of the DvbS2Statistics block (must match block ID)
#define DT_BC_S2STATS_2132_NAME        "DvbS2Statistics"
#define DT_BC_S2STATS_2132_SHORTNAME   "S2STATS"

// MACRO: to init an block-controller-ID for the S2STATS_2132-BC
#define DT_BC_S2STATS_2132_INIT_ID(ID, ROLE, INSTANCE, UUID)                             \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_S2STATS_2132_NAME, DT_BC_S2STATS_2132_SHORTNAME,  ROLE,      \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS2STATS_2132 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcS2STATS_2132
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    DtDpc  m_IntDpc;
    DtSpinLock  m_Lock;         // Lock to protect against mutual access of 
                                // m_FrameStatusBuffer and m_OperationalMode
    // Cached settings
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_OperationalMode;     // Current operational mode
    Int  m_FifoSize;            // Number of samples the firmware fifo can hold
    DtBcS2STATS_2132_FrameStatusBuffer  m_FrameStatusBuffer;
}  DtBcS2STATS_2132;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2STATS_2132 public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcS2STATS_2132_Close(DtBc*);
DtBcS2STATS_2132*  DtBcS2STATS_2132_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcS2STATS_2132_ClearStatistics(DtBcS2STATS_2132* pBc);
DtStatus  DtBcS2STATS_2132_GetOperationalMode(DtBcS2STATS_2132* pBc,  Int* pOpMode);
DtStatus  DtBcS2STATS_2132_GetStatisticsStatus(DtBcS2STATS_2132* pBc, Int* pBufferSize,
                                               Int* pNumAvailble, Int* pOverflow);
DtStatus  DtBcS2STATS_2132_ReadStatistics(DtBcS2STATS_2132* pBc,
                                                DtS2Stats_2132_FrameStatus *pFrameStatus,
                                                Int NumToRead, Int*pNumRead);
DtStatus  DtBcS2STATS_2132_SetOperationalMode(DtBcS2STATS_2132* pBc,  Int OpMode);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcS2STATS_2132 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS2STATS_2132 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an S2STATS_2132 Block
typedef struct _DtIoStubBcS2STATS_2132
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcS2STATS_2132;


//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS2STATS_2132 public functions -.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcS2STATS_2132_Close(DtIoStub*);
DtIoStubBcS2STATS_2132*  DtIoStubBcS2STATS_2132_Open(DtBc*);


#endif  // #ifndef __DT_BC_S2STATS_2132_H