//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*# DtBcCONSTSOURCE.h *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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

#ifndef __DT_BC_CONSTSOURCE_H
#define __DT_BC_CONSTSOURCE_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the ConstRateSource block (must match block ID)
#define DT_BC_CONSTSOURCE_NAME        "ConstRateSource"
#define DT_BC_CONSTSOURCE_SHORTNAME   "CONSTSOURCE"

// MACRO: to init an block-controller-ID for the CONSTSOURCE-BC
#define DT_BC_CONSTSOURCE_INIT_ID(ID, ROLE, INSTANCE, UUID)                              \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_CONSTSOURCE_NAME, DT_BC_CONSTSOURCE_SHORTNAME, ROLE,         \
                                                                        INSTANCE, UUID); \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+= DtBcCONSTSOURCE definitions =+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSOURCE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcCONSTSOURCE
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Chached configuration
    UInt32  m_SysClockFreq;
    Int  m_DataWidth;

    // Cached settings
    Bool  m_BlockEnabled;
    Int  m_OperationalMode;
    Int  m_DataPattern;
    Int  m_TestInterval;
    Int64  m_DataRate;

    // Internal State
    Bool  m_TestIntervalStarted;

    // Interrupts related
    DtDpc  m_IntDpc;
    DtEvent  m_TestIntervalDoneEvent;

}  DtBcCONSTSOURCE;

//.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSOURCE public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-

void  DtBcCONSTSOURCE_Close(DtBc*);
DtBcCONSTSOURCE*  DtBcCONSTSOURCE_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);
DtStatus DtBcCONSTSOURCE_GetOperationalMode(DtBcCONSTSOURCE* pBc, Int* pOpMode);
DtStatus DtBcCONSTSOURCE_GetDataPattern(DtBcCONSTSOURCE* pBc, Int*  pDataPattern);
DtStatus DtBcCONSTSOURCE_GetTestInterval(DtBcCONSTSOURCE* pBc, Int* pTestItv);
DtStatus DtBcCONSTSOURCE_GetDataRate(DtBcCONSTSOURCE* pBc, Int64* pDataRate);
DtStatus DtBcCONSTSOURCE_GetOverflowCnt(DtBcCONSTSOURCE* pBc, UInt32* pOverflowCnt);
DtStatus DtBcCONSTSOURCE_SetOperationalMode(DtBcCONSTSOURCE* pBc, Int OpMode);
DtStatus DtBcCONSTSOURCE_SetDataPattern(DtBcCONSTSOURCE* pBc, Int DataPattern);
DtStatus DtBcCONSTSOURCE_SetTestInterval(DtBcCONSTSOURCE* pBc, Int TestItv);
DtStatus DtBcCONSTSOURCE_SetDataRate(DtBcCONSTSOURCE* pBc, Int64 DataRate);
DtStatus DtBcCONSTSOURCE_StartTestInterval(DtBcCONSTSOURCE* pBc);
DtStatus DtBcCONSTSOURCE_WaitForTestIntervalDone(DtBcCONSTSOURCE* pBc);
DtStatus DtBcCONSTSOURCE_RunTest(DtBcCONSTSOURCE* pBc, Int64 DataRate, Int TestItv,
                                                                    UInt32* pOverflowCnt);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcCONSTSOURCE definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an CONSTSOURCE Block
typedef struct _DtIoStubBcCONSTSOURCE
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcCONSTSOURCE;

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSOURCE public functions -.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcCONSTSOURCE_Close(DtIoStub*);
DtIoStubBcCONSTSOURCE*  DtIoStubBcCONSTSOURCE_Open(DtBc*);

#endif  // #ifndef __DT_BC_CONSTSOURCE_H
