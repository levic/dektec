//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcACCUFIFO.h *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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

#ifndef __DT_BC_ACCUFIFO_H
#define __DT_BC_ACCUFIFO_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the AccuFifo block (must match block ID)
#define DT_BC_ACCUFIFO_NAME        "AccuFifo"
#define DT_BC_ACCUFIFO_SHORTNAME   "ACCUFIFO"

// MACRO: to init an block-controller-ID for the ACCUFIFO-BC
#define DT_BC_ACCUFIFO_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_ACCUFIFO_NAME, DT_BC_ACCUFIFO_SHORTNAME, ROLE,               \
                                                                        INSTANCE, UUID); \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcACCUFIFO definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcACCUFIFO
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached settings
    Int  m_AccuPeriodNumClks;       // Accumulation period in clock frequency ticks
    Int  m_MaxNumMeasurements;      // Maximum number of measurements
    Int  m_NumMeasurements;         // Number of measurements
    Int64  m_ClockFrequency;        // Clock frequency
}  DtBcACCUFIFO;

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcACCUFIFO public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

void  DtBcACCUFIFO_Close(DtBc*);
DtBcACCUFIFO*  DtBcACCUFIFO_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);
DtStatus DtBcACCUFIFO_GetAccuPeriod(DtBcACCUFIFO* pBc, Int* pPeriodNs);
DtStatus DtBcACCUFIFO_GetMaxNumMeasurements(DtBcACCUFIFO* pBc, Int* pMaxNumMeasm);
DtStatus DtBcACCUFIFO_GetNumMeasurements(DtBcACCUFIFO* pBc, Int* pNumMeasm);
DtStatus DtBcACCUFIFO_GetMeasurementStatus(DtBcACCUFIFO* pBc, Int* pNumAva, Bool* pDone);
DtStatus DtBcACCUFIFO_ReadMeasurements(DtBcACCUFIFO* pBc, Int* pMesmBuf, Int NumToRead,
                                                                           Int* pNumRead);
DtStatus DtBcACCUFIFO_RestartMeasurement(DtBcACCUFIFO* pBc);
DtStatus DtBcACCUFIFO_SetAccuPeriod(DtBcACCUFIFO* pBc, Int PeriodNs);
DtStatus DtBcACCUFIFO_SetNumMeasurements(DtBcACCUFIFO* pBc, Int NumMeasm);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcACCUFIFO definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcACCUFIFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an ACCUFIFO Block
typedef struct _DtIoStubBcACCUFIFO
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcACCUFIFO;

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcACCUFIFO public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcACCUFIFO_Close(DtIoStub*);
DtIoStubBcACCUFIFO*  DtIoStubBcACCUFIFO_Open(DtBc*);

#endif  // #ifndef __DT_BC_ACCUFIFO_H
