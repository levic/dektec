//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQMISC.h *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
// This file is not part of the PCIe driver. It can be used as template to create a new
// block controller. Replace "IQMISC" by the building block's shortname (all uppercase)
// and replace "IqMisc" by the building block's (long) name.
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

#ifndef __DT_BC_IQMISC_H
#define __DT_BC_IQMISC_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the IqMisc block (must match block ID)
#define DT_BC_IQMISC_NAME        "IqMiscProc"
#define DT_BC_IQMISC_SHORTNAME   "IQMISC"

// MACRO: to init an block-controller-ID for the IQMISC-BC
#define DT_BC_IQMISC_INIT_ID(ID, ROLE, INSTANCE, UUID)                                   \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_IQMISC_NAME, DT_BC_IQMISC_SHORTNAME, ROLE, INSTANCE, UUID);  \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQMISC definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcIQMISC
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached settings
    Bool  m_BlockEnabled;   // Block enabled
    Int  m_OperationalMode; // Operational mode
    Bool  m_MuteI;          // Mute I samples
    Bool  m_MuteQ;          // Mute Q samples
    Bool  m_SpectrumInv;    // Enable spectrum inversion (swap I and Q)
    Int  m_TestPattern;     // Select testpattern
    Int  m_NoiseLevel;      // Added noise level in 2.16 format
    Int  m_OutputLevel;     // Multiplier for I and Q (2.16 format)
    Int  m_Offset;         // I and Q sample offset (2's-complement)
}  DtBcIQMISC;

//.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcIQMISC public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-

void  DtBcIQMISC_Close(DtBc*);
DtBcIQMISC*  DtBcIQMISC_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);
DtStatus DtBcIQMISC_GetOperationalMode(DtBcIQMISC* pBc, Int* pOpMode);
DtStatus DtBcIQMISC_SetOperationalMode(DtBcIQMISC* pBc, Int OpMode);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQMISC definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an IQMISC Block
typedef struct _DtIoStubBcIQMISC
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcIQMISC;

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcIQMISC_Close(DtIoStub*);
DtIoStubBcIQMISC*  DtIoStubBcIQMISC_Open(DtBc*);

#endif  // #ifndef __DT_BC_IQMISC_H
