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

#ifndef __DT_BC_LEDB_H
#define __DT_BC_LEDB_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the Board Level LED Controller block (must match block ID)
#define DT_BC_LEDB_NAME        "LedBoard"
#define DT_BC_LEDB_SHORTNAME   "LEDB"

// MACRO: to init an block-controller-ID for the LEDB-BC
#define DT_BC_LEDB_INIT_ID(ID, ROLE, INSTANCE, UUID)                                     \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_LEDB_NAME, DT_BC_LEDB_SHORTNAME, ROLE, INSTANCE, UUID);      \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcLEDB definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLEDB -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcLEDB
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;
    // Cached settings
    Bool  m_Overrule;       // Overrule; if FALSE LEDs are automatically controlled
    Bool  m_GreenOn;        // if m_Overrule==TRUE green LED state: on/off
    Bool  m_RedOn;          // if m_Overrule==TRUE red LED state: on/off
    Bool  m_BlueOn;         // if m_Overrule==TRUE blue LED state on/off
}  DtBcLEDB;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcLEDB public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcLEDB_Close(DtBc*);
DtBcLEDB*  DtBcLEDB_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcLEDB_GetLedControl(DtBcLEDB* pBc, Bool* pOverrule, Bool* pGreen, Bool* pRed,
                                                                             Bool* pBlue);
DtStatus  DtBcLEDB_SetLedControl(DtBcLEDB* pBc, Bool Overrule, Bool Green, Bool Red, 
                                                                               Bool Blue);



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcLEDB definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLEDB -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an LEDB Block
typedef struct _DtIoStubBcLEDB
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcLEDB;


//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcLEDB public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcLEDB_Close(DtIoStub*);
DtIoStubBcLEDB*  DtIoStubBcLEDB_Open(DtBc*);


#endif  // #ifndef __DT_BC_LEDB_H