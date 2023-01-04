//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQFIR.h *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
// This file is not part of the PCIe driver. It can be used as template to create a new
// block controller. Replace "IQFIR" by the building block's shortname (all uppercase)
// and replace "IqFir" by the building block's (long) name.
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

#ifndef __DT_BC_IQFIR_H
#define __DT_BC_IQFIR_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the IqFir block (must match block ID)
#define DT_BC_IQFIR_NAME        "IqFirFilter"
#define DT_BC_IQFIR_SHORTNAME   "IQFIR"

// MACRO: to init an block-controller-ID for the IQFIR-BC
#define DT_BC_IQFIR_INIT_ID(ID, ROLE, INSTANCE, UUID)                                    \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_IQFIR_NAME, DT_BC_IQFIR_SHORTNAME, ROLE, INSTANCE, UUID);    \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQFIR definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcIQFIR
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    DtFastMutex  m_AccessMutex;     // Access protection for the filter
    // Cached settings
    Bool  m_BlockEnabled;
    Int  m_OperationalMode;

    Int  m_NumberTaps;      // Number of coefficients
    Int  m_CoefficientSize;         // Size of the Coefficients (number of bits)
	
    // Interrupts related
    DtDpc  m_IntDpc;
    DtEvent  m_DoneEvent;

}  DtBcIQFIR;

//.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcIQFIR public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-

void  DtBcIQFIR_Close(DtBc*);
DtBcIQFIR*  DtBcIQFIR_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);
DtStatus DtBcIQFIR_GetOperationalMode(DtBcIQFIR* pBc, Int* pOpMode);
DtStatus DtBcIQFIR_SetOperationalMode(DtBcIQFIR* pBc, Int OpMode);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQFIR definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an IQFIR Block
typedef struct _DtIoStubBcIQFIR
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcIQFIR;

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcIQFIR_Close(DtIoStub*);
DtIoStubBcIQFIR*  DtIoStubBcIQFIR_Open(DtBc*);

#endif  // #ifndef __DT_BC_IQFIR_H
