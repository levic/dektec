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

#ifndef __DT_BC_SWITCH_H
#define __DT_BC_SWITCH_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the Switch block (must match block ID)
#define DT_BC_SWITCH_NAME        "Switch"
#define DT_BC_SWITCH_SHORTNAME   "SWITCH"

// MACRO: to init an block-controller-ID for the SWITCH-BC
#define DT_BC_SWITCH_INIT_ID(ID, ROLE, INSTANCE, UUID)                                   \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_SWITCH_NAME, DT_BC_SWITCH_SHORTNAME, ROLE, INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSWITCH definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSWITCH -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcSWITCH
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    // Cached settings
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_OperationalMode;     // Current operational mode
    Int  m_NumInputPorts;       // Number of input ports
    Int  m_NumOutputPorts;      // Number of output ports
    Int  m_InputIndex;          // Switch position; index of selected input port
    Int  m_OutputIndex;         // Switch position; index of selected output port
}  DtBcSWITCH;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSWITCH public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcSWITCH_Close(DtBc*);
DtBcSWITCH*  DtBcSWITCH_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcSWITCH_Clear(DtBcSWITCH* pBc);
DtStatus  DtBcSWITCH_GetConfiguration(DtBcSWITCH* pBc, Int* pNumInputs, Int* pNumOutputs);
DtStatus  DtBcSWITCH_GetOperationalMode(DtBcSWITCH* pBc,  Int* pOpMode);
DtStatus  DtBcSWITCH_GetPosition(DtBcSWITCH* pBc, Int* pInputIdx, Int* pOutputIdx);
DtStatus  DtBcSWITCH_SetOperationalMode(DtBcSWITCH* pBc,  Int OpMode);
DtStatus  DtBcSWITCH_SetPosition(DtBcSWITCH* pBc, Int InputIdx, Int OutputIdx);




//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSWITCH definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSWITCH -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an SWITCH Block
typedef struct _DtIoStubBcSWITCH
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcSWITCH;


//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSWITCH public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcSWITCH_Close(DtIoStub*);
DtIoStubBcSWITCH*  DtIoStubBcSWITCH_Open(DtBc*);


#endif  // #ifndef __DT_BC_SWITCH_H