//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcADS4246_2132.h *#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

#ifndef __DT_BC_ADS4246_2132_H
#define __DT_BC_ADS4246_2132_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the AdcAds4246If block (must match block ID)
#define DT_BC_ADS4246_2132_NAME        "AdcAds4246If"
#define DT_BC_ADS4246_2132_SHORTNAME   "ADS4246"

// MACRO: to init an block-controller-ID for the ADS4246_2132-BC
#define DT_BC_ADS4246_2132_INIT_ID(ID, ROLE, INSTANCE, UUID)                             \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_ADS4246_2132_NAME, DT_BC_ADS4246_2132_SHORTNAME,  ROLE,      \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcADS4246_2132 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcADS4246_2132 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcADS4246_2132
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    // Cached settings
    Bool  m_BlockEnabled;        // Block enable
    Int  m_OperationalMode;     // Current operational mode

}  DtBcADS4246_2132;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcADS4246_2132 public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcADS4246_2132_Close(DtBc*);
DtBcADS4246_2132*  DtBcADS4246_2132_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcADS4246_2132_GetOperationalMode(DtBcADS4246_2132* pBc,  Int* pOpMode);
DtStatus  DtBcADS4246_2132_SetOperationalMode(DtBcADS4246_2132* pBc,  Int OpMode);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcADS4246_2132 definitions +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcADS4246_2132 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an ADS4246_2132 Block
typedef struct _DtIoStubBcADS4246_2132
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcADS4246_2132;


//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcADS4246_2132 public functions -.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcADS4246_2132_Close(DtIoStub*);
DtIoStubBcADS4246_2132*  DtIoStubBcADS4246_2132_Open(DtBc*);


#endif  // #ifndef __DT_BC_ADS4246_2132_H