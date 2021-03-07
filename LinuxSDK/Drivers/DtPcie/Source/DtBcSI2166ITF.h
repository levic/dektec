// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSI2166ITF.h *#*#*#*#*#*#*#*#*#* (C) 2020 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2020 DekTec Digital Video B.V.
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

#ifndef __DT_BC_SI2166ITF_H
#define __DT_BC_SI2166ITF_H

// Name and short-name of the IoSerialInput block (must match block ID)
#define DT_BC_SI2166ITF_NAME        "Si2166Itf"
#define DT_BC_SI2166ITF_SHORTNAME   "SI2166ITF"

// MACRO: to init an block-controller-ID for the SI2166ITF-BC
#define DT_BC_SI2166ITF_INIT_ID(ID, ROLE, INSTANCE, UUID)                                \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_SI2166ITF_NAME, DT_BC_SI2166ITF_SHORTNAME,                   \
                                                                 ROLE, INSTANCE, UUID);  \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSI2166ITF definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSI2166ITF -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcSI2166ITF
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached values
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_OperationalMode;     // Operational mode
    Int  m_Si2166Mode;          // Si2166 mode
    Int  m_DataType;            // Packet stream data type

} DtBcSI2166ITF;


// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSI2166ITF public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcSI2166ITF_Close(DtBc*);
DtBcSI2166ITF*  DtBcSI2166ITF_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcSI2166ITF_GetConfiguration(DtBcSI2166ITF*, Int* pSi2166Mode,Int* pDataType);
DtStatus  DtBcSI2166ITF_GetOperationalMode(DtBcSI2166ITF*, Int* pOpMode);
DtStatus  DtBcSI2166ITF_SetConfiguration(DtBcSI2166ITF*, Int Si2166Mode, Int DataType);
DtStatus  DtBcSI2166ITF_SetOperationalMode(DtBcSI2166ITF*, Int OpMode);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSI2166ITF definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSI2166ITF -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an SI2166ITF Block
typedef struct _DtIoStubBcSI2166ITF
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcSI2166ITF;


// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSI2166ITF public functions -.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcSI2166ITF_Close(DtIoStub*);
DtIoStubBcSI2166ITF*  DtIoStubBcSI2166ITF_Open(DtBc*);

#endif  // #ifndef __DT_BC_SI2166ITF_H
