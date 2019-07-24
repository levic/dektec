//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*# DtBcCONSTSINK.h *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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

#ifndef __DT_BC_CONSTSINK_H
#define __DT_BC_CONSTSINK_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the ConstRateSink block (must match block ID)
#define DT_BC_CONSTSINK_NAME        "ConstRateSink"
#define DT_BC_CONSTSINK_SHORTNAME   "CONSTSINK"

// MACRO: to init an block-controller-ID for the CONSTSINK-BC
#define DT_BC_CONSTSINK_INIT_ID(ID, ROLE, INSTANCE, UUID)                                \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_CONSTSINK_NAME, DT_BC_CONSTSINK_SHORTNAME, ROLE, INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+= DtBcCONSTSINK definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCONSTSINK -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcCONSTSINK
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
    Bool  m_CheckData;
    Int64  m_DataRate;

    // Internal State
    Bool  m_TestIntervalStarted;

    // Interrupts related

    DtDpc  m_IntDpc;
    DtEvent  m_TestIntervalDoneEvent;

}  DtBcCONSTSINK;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcCONSTSINK public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

void  DtBcCONSTSINK_Close(DtBc*);
DtBcCONSTSINK*  DtBcCONSTSINK_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);
DtStatus DtBcCONSTSINK_GetOperationalMode(DtBcCONSTSINK* pBc, Int* pOpMode);
DtStatus DtBcCONSTSINK_GetDataPattern(DtBcCONSTSINK* pBc, Int*  pDataPattern);
DtStatus DtBcCONSTSINK_GetTestInterval(DtBcCONSTSINK* pBc, Int* pTestItv);
DtStatus DtBcCONSTSINK_GetCheckData(DtBcCONSTSINK* pBc, Int* pCheckData);
DtStatus DtBcCONSTSINK_GetDataRate(DtBcCONSTSINK* pBc, Int64* pDataRate);
DtStatus DtBcCONSTSINK_GetUnderflowCnt(DtBcCONSTSINK* pBc, UInt32* pUnderflowCnt);
DtStatus DtBcCONSTSINK_GetDataErrorCnt(DtBcCONSTSINK* pBc, UInt32* pDataErrorCnt);
DtStatus DtBcCONSTSINK_SetOperationalMode(DtBcCONSTSINK* pBc, Int OpMode);
DtStatus DtBcCONSTSINK_SetDataPattern(DtBcCONSTSINK* pBc, Int DataPattern);
DtStatus DtBcCONSTSINK_SetTestInterval(DtBcCONSTSINK* pBc, Int TestItv);
DtStatus DtBcCONSTSINK_SetCheckData(DtBcCONSTSINK* pBc, Int CheckData);
DtStatus DtBcCONSTSINK_SetDataRate(DtBcCONSTSINK* pBc, Int64 DataRate);
DtStatus DtBcCONSTSINK_StartTestInterval(DtBcCONSTSINK* pBc);
DtStatus DtBcCONSTSINK_WaitForTestIntevalDone(DtBcCONSTSINK* pBc);
DtStatus DtBcCONSTSINK_RunTest(DtBcCONSTSINK* pBc, Int64 DataRate, Int TestItv,
                                            UInt32* pUnderflowCnt, UInt32* pDataErrorCnt);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcCONSTSINK definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an CONSTSINK Block
typedef struct _DtIoStubBcCONSTSINK
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcCONSTSINK;

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcCONSTSINK public functions -.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcCONSTSINK_Close(DtIoStub*);
DtIoStubBcCONSTSINK*  DtIoStubBcCONSTSINK_Open(DtBc*);

#endif  // #ifndef __DT_BC_CONSTSINK_H
