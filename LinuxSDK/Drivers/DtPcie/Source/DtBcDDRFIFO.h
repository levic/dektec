//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcDDRFIFO.h *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
// This file is not part of the PCIe driver. It can be used as template to create a new
// block controller. Replace "DDRFIFO" by the building block's shortname (all uppercase)
// and replace "Template" by the building block's (long) name.
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

#ifndef __DT_BC_DDRFIFO_H
#define __DT_BC_DDRFIFO_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the DdrFifo block (must match block ID)
#define DT_BC_DDRFIFO_NAME        "DdrFifo"
#define DT_BC_DDRFIFO_SHORTNAME   "DDRFIFO"

// MACRO: to init an block-controller-ID for the DDRFIFO-BC
#define DT_BC_DDRFIFO_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_DDRFIFO_NAME, DT_BC_DDRFIFO_SHORTNAME, ROLE,               \
                                                                        INSTANCE, UUID); \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDDRFIFO definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFIFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcDDRFIFO
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached settings
    Bool  m_BlockEnabled;
    Int  m_OperationalMode;
    Int  m_BurstSize;   // #bytes
    Int  m_FifoSize;    // #bytes
    Int  m_DataWidth;   // #bits
}  DtBcDDRFIFO;

//.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcDDRFIFO public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-

void  DtBcDDRFIFO_Close(DtBc*);
DtStatus  DtBcDDRFIFO_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile);
DtStatus  DtBcDDRFIFO_GetFifoLoad(DtBcDDRFIFO* pBc, Int* pFifoLoad);
DtStatus  DtBcDDRFIFO_GetOperationalMode(DtBcDDRFIFO* pBc, Int* pOpMode);
DtStatus  DtBcDDRFIFO_GetProperties(DtBcDDRFIFO* pBc, Int* pBurstSize, Int* pFifoSize, 
                                                                         Int* pDataWidth);
DtBcDDRFIFO*  DtBcDDRFIFO_Open(Int  Address, DtCore*, DtPt*  pPt, const char*  pRole,
                                               Int  Instance, Int  Uuid, Bool CreateStub);
DtStatus  DtBcDDRFIFO_SetOperationalMode(DtBcDDRFIFO* pBc, Int OpMode);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDDRFIFO definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFIFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an DDRFIFO Block
typedef struct _DtIoStubBcDDRFIFO
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcDDRFIFO;


//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSWITCH public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcDDRFIFO_Close(DtIoStub*);
DtIoStubBcDDRFIFO*  DtIoStubBcDDRFIFO_Open(DtBc*);
#endif  // #ifndef __DT_BC_DDRFIFO_H
