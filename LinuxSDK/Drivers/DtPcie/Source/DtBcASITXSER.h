//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcASITXSER.h *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
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

#ifndef __DT_BC_ASITXSER_H
#define __DT_BC_ASITXSER_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the AsiTxSer block (must match block ID)
#define DT_BC_ASITXSER_NAME        "AsiTxSer"
#define DT_BC_ASITXSER_SHORTNAME   "ASITXSER"

// MACRO: to init an block-controller-ID for the ASITXSER-BC
#define DT_BC_ASITXSER_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_ASITXSER_NAME, DT_BC_ASITXSER_SHORTNAME, ROLE, INSTANCE,     \
                                                                                 UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcASITXSER definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXSER -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcASITXSER
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    // Cached settings
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_OperationalMode;     // Current operational mode

}  DtBcASITXSER;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXSER public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcASITXSER_Close(DtBc*);
DtBcASITXSER*  DtBcASITXSER_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcASITXSER_GetOperationalMode(DtBcASITXSER* pBc,  Int* pOpMode);
DtStatus  DtBcASITXSER_SetOperationalMode(DtBcASITXSER* pBc,  Int OpMode);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcASITXSER definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcASITXSER -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an ASITXSER Block
typedef struct _DtIoStubBcASITXSER
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcASITXSER;


//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcASITXSER public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcASITXSER_Close(DtIoStub*);
DtIoStubBcASITXSER*  DtIoStubBcASITXSER_Open(DtBc*);


#endif  // #ifndef __DT_BC_ASITXSER_H