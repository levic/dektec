//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQPWR_2132.h *#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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

#ifndef __DT_BC_IQPWR_2132_H
#define __DT_BC_IQPWR_2132_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the IqPwrFilter block (must match block ID)
#define DT_BC_IQPWR_2132_NAME        "IqPowerEstimator"
#define DT_BC_IQPWR_2132_SHORTNAME   "IQPWR"

// MACRO: to init an block-controller-ID for the IQPWR_2132-BC
#define DT_BC_IQPWR_2132_INIT_ID(ID, ROLE, INSTANCE, UUID)                               \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_IQPWR_2132_NAME, DT_BC_IQPWR_2132_SHORTNAME,  ROLE,          \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQPWR_2132 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_2132 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcIQPWR_2132
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    // Cached settings
    Int  m_EstWindow;       // Window Used for Estimation
}  DtBcIQPWR_2132;

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_2132 public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcIQPWR_2132_Close(DtBc*);
DtBcIQPWR_2132*  DtBcIQPWR_2132_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcIQPWR_2132_GetEstPower(DtBcIQPWR_2132*, UInt32* pPower);
DtStatus  DtBcIQPWR_2132_GetEstWindow(DtBcIQPWR_2132* pBc, Int* pWindow);
DtStatus  DtBcIQPWR_2132_SetEstWindow(DtBcIQPWR_2132* pBc, Int Window);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQPWR_2132 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_2132 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an IQPWR_2132 Block
typedef struct _DtIoStubBcIQPWR_2132
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcIQPWR_2132;


//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_2132 public functions -.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcIQPWR_2132_Close(DtIoStub*);
DtIoStubBcIQPWR_2132*  DtIoStubBcIQPWR_2132_Open(DtBc*);


#endif  // #ifndef __DT_BC_IQPWR_2132_H