// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcDATAFIFO.h *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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

#ifndef __DT_BC_DATAFIFO_H
#define __DT_BC_DATAFIFO_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the DataDdrFifo block (must match block ID)
#define DT_BC_DATAFIFO_NAME        "DataDdrFifo"
#define DT_BC_DATAFIFO_SHORTNAME   "DATAFIFO"

// MACRO: to init an block-controller-ID for the DATAFIFO-BC
#define DT_BC_DATAFIFO_INIT_ID(ID, ROLE, INSTANCE, UUID)                                \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_DATAFIFO_NAME, DT_BC_DATAFIFO_SHORTNAME, ROLE,             \
                                                                        INSTANCE, UUID); \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDATAFIFO definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcDATAFIFO
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached settings
    Bool  m_BlockEnabled;     // Block enabled
    Int  m_OperationalMode;   // Operational mode
    Int  m_MaxFifoSize;       // Maximum FIFO Size in #bytes
    Int  m_BurstSize;         // Maximum Burst Size in #bytes
    Int  m_Capabilities;      // RX/TX capability flags
    Int  m_DataWidth;         // Datawidth in bits
    Int  m_Direction;         // FIFO direction
}  DtBcDATAFIFO;

//.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcDATAFIFO public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-

void  DtBcDATAFIFO_Close(DtBc*);
DtBcDATAFIFO*  DtBcDATAFIFO_Open(Int  Address, DtCore*, DtPt*  pPt,
                                 const char*  pRole, Int  Instance, Int  Uuid,
                                 Bool  CreateStub);
DtStatus  DtBcDATAFIFO_ClearFifoMax(DtBcDATAFIFO*, Bool MaxFree, Bool MaxLoad);
DtStatus  DtBcDATAFIFO_GetDirection(DtBcDATAFIFO*, Int* pDirection);
DtStatus  DtBcDATAFIFO_GetFifoStatus(DtBcDATAFIFO*, Int* pCurLoad, Int* pMaxFree, 
                                                                           Int* pMaxLoad);
DtStatus  DtBcDATAFIFO_GetOperationalMode(DtBcDATAFIFO*, Int* pOpMode);
DtStatus  DtBcDATAFIFO_GetUnderflowCount(DtBcDATAFIFO*, UInt32* pUflCount);
DtStatus  DtBcDATAFIFO_GetOverflowCount(DtBcDATAFIFO*, UInt32* pOvrCount);
DtStatus  DtBcDATAFIFO_GetProperties(DtBcDATAFIFO * pBc, UInt32* pCapabilities, 
                                     Int* pBurstSize, Int* pMaxFifoSize, Int *pDataWidth);
DtStatus  DtBcDATAFIFO_SetDirection(DtBcDATAFIFO*, Int Direction);
DtStatus  DtBcDATAFIFO_SetFifoAddr(DtBcDATAFIFO*, UInt32 FifoAddr);
DtStatus  DtBcDATAFIFO_SetFifoSize(DtBcDATAFIFO*, UInt32 FifoSize);
DtStatus  DtBcDATAFIFO_SetOperationalMode(DtBcDATAFIFO* pBc, Int OpMode);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDATAFIFO definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDATAFIFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an DATAFIFO Block
typedef struct _DtIoStubBcDATAFIFO
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcDATAFIFO;

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDATAFIFO public functions -.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcDATAFIFO_Close(DtIoStub*);
DtIoStubBcDATAFIFO*  DtIoStubBcDATAFIFO_Open(DtBc*);

#endif  // #ifndef __DT_BC_DATAFIFO_H
