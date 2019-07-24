//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSpiProm.h *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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

#ifndef __DT_DF_SPIPROM_H
#define __DT_DF_SPIPROM_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcSPIM.h"

// Name + short-name for the SPIPROM function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_SPIPROM_NAME          "SpiProm"
#define DT_DF_SPIPROM_SHORTNAME     "SPIPROM"

// MACRO: to init an driver-function-ID for the SPIPROM-DF
#define DT_DF_SPIPROM_INIT_ID(ID, ROLE, INSTANCE, UUID)                                  \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_SPIPROM_NAME, DT_DF_SPIPROM_SHORTNAME, ROLE,                 \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSpiProm definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiProm -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtDfSpiProm
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    DtBcSPIM*  m_pBcSpiM;               // SPI master block controller
    DtFastMutex  m_AccessMutex;         // Access protection for SPI-Prom
    const struct DtDfSpiPromProperties*  m_pSpiPromProperties;  // SPI-prom propeties
    const struct DtDfSpiPromCommand*  m_pSpiPromCommands;       // SPI-prom commands
}  DtDfSpiProm;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfSpiProm_Close(DtDf*);
DtDfSpiProm*  DtDfSpiProm_Open(DtCore*, DtPt*  pPt, const char*  pRole, Int  Instance, 
                                                             Int  Uuid, Bool  CreateStub);
DtStatus  DtDfSpiProm_GetProperties(DtDfSpiProm*, Int* pPromSize, Int* pPageSize);
DtStatus  DtDfSpiProm_LockDevice(DtDfSpiProm*, Bool Lock);
DtStatus  DtDfSpiProm_Read(DtDfSpiProm*, Int StartAddress, Int NumToRead, UInt8* pBuf);
DtStatus  DtDfSpiProm_Write(DtDfSpiProm*, Int StartAddress, Int NumToWrite, 
                                                                       const UInt8* pBuf);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfSpiProm definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSpiProm -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an SPIPROM Driver-Function
typedef struct _DtIoStubDfSpiProm
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfSpiProm;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSpiProm public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubDfSpiProm_Close(DtIoStub*);
DtIoStubDfSpiProm*  DtIoStubDfSpiProm_Open(DtDf*);

#endif  // #ifndef __DT_DF_SPIPROM_H

