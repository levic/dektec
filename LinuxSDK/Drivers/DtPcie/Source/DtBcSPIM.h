//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSPIM.h *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

#ifndef __DT_BC_SPIM_H
#define __DT_BC_SPIM_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the SPIMrotocol block (must match block ID)
#define DT_BC_SPIM_NAME        "SpiMaster"
#define DT_BC_SPIM_SHORTNAME   "SPIM"

// MACRO: to init an block-controller-ID for the SPIM-BC
#define DT_BC_SPIM_INIT_ID(ID, ROLE, INSTANCE, UUID)                                     \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_SPIM_NAME, DT_BC_SPIM_SHORTNAME, ROLE, INSTANCE, UUID);      \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSPIM definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


typedef  struct _DtBcSPIMProperties
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached configuration data
    Int  m_DeviceId;                // Device ID
    Int  m_SpiClockRate;            // SPI clock rate
    Int  m_WordSize;                // SPI word size in number of bits
    Int  m_NumBytesPerWord;         // SPI word size in number of bytes
    Int  m_DuplexMode;              // Duplex mode: full or half duplex
    Int  m_MaxTransferTime;         // Maximum transfer time in ns 

    DtFastMutex  m_AccessMutex;     // Access protection for SPI
}  DtBcSPIM;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIM public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcSPIM_Close(DtBc*);
DtBcSPIM * DtBcSPIM_Open(Int Address, DtCore* pCore, DtPt* pPt, const char* pRole, 
                                                Int Instance, Int Uuid, Bool CreateStub);
DtStatus  DtBcSPIM_Read(DtBcSPIM*, Int Length, UInt8* pBuffer);
DtStatus  DtBcSPIM_Write(DtBcSPIM*,  Int Length, const UInt8* pBuffer);
DtStatus  DtBcSPIM_WriteRead(DtBcSPIM*, Int WriteLength, const UInt8* pWriteBuffer,
                                                      Int ReadLength, UInt8* pReadBuffer);
DtStatus  DtBcSPIM_WriteWrite(DtBcSPIM*, Int WriteLength1, const UInt8* pWriteBuffer1,
                                            Int WriteLength2, const UInt8* pWriteBuffer2);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSPIM definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIM -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an SPIM Block
typedef struct _DtIoStubBcSPIM
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcSPIM;


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIM public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcSPIM_Close(DtIoStub*);
DtIoStubBcSPIM*  DtIoStubBcSPIM_Open(DtBc*);

#endif  // #ifndef __DT_BC_SPIM_H
