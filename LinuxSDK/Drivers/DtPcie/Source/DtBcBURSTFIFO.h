// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcBURSTFIFO.h *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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

#ifndef __DT_BC_BURSTFIFO_H
#define __DT_BC_BURSTFIFO_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the BurstFifo block (must match block ID)
#define DT_BC_BURSTFIFO_NAME        "BurstFifo"
#define DT_BC_BURSTFIFO_SHORTNAME   "BURSTFIFO"

// MACRO: to init an block-controller-ID for the BURSTFIFO-BC
#define DT_BC_BURSTFIFO_INIT_ID(ID, ROLE, INSTANCE, UUID)                                \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_BURSTFIFO_NAME, DT_BC_BURSTFIFO_SHORTNAME, ROLE,             \
                                                                        INSTANCE, UUID); \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcBURSTFIFO definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcBURSTFIFO
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached settings
    Bool  m_BlockEnabled;       // Block enabled
    Int  m_OperationalMode;     // Operational mode
    Int  m_BurstFifoSize;       // Burst fifo size in number of bytes
    Int  m_Capabilities;        // RX/TX capability flags
    Int  m_DataWidth;           // Data width
    UInt  m_NumBytesPerDataWord; // Number of bytes per data word
    Int  m_Direction;           // FIFO direction
    Int  m_RxFlowControl;       // Input flow control
}  DtBcBURSTFIFO;

//.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcBURSTFIFO public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-

void  DtBcBURSTFIFO_Close(DtBc*);
DtBcBURSTFIFO*  DtBcBURSTFIFO_Open(Int  Address, DtCore*, DtPt*  pPt,
                                 const char*  pRole, Int  Instance, Int  Uuid,
                                 Bool  CreateStub);
DtStatus  DtBcBURSTFIFO_ClearFifoMax(DtBcBURSTFIFO*, Bool MaxFree, Bool MaxLoad);
DtStatus  DtBcBURSTFIFO_GetDirection(DtBcBURSTFIFO*, Int* pDirection);
DtStatus  DtBcBURSTFIFO_GetFifoStatus(DtBcBURSTFIFO*, Int* pCurFree, Int* pCurLoad, 
                                                            Int* pMaxFree, Int* pMaxLoad);
DtStatus  DtBcBURSTFIFO_GetOperationalMode(DtBcBURSTFIFO*, Int* pOpMode);
DtStatus  DtBcBURSTFIFO_GetOverUnderflowCount(DtBcBURSTFIFO*, UInt32* pOvflUflCount);
DtStatus  DtBcBURSTFIFO_GetProperties(DtBcBURSTFIFO*, UInt32* pCapabilities,
                                                    Int* pDataWidth, Int* pBurstFifoSize);
DtStatus  DtBcBURSTFIFO_GetRxFlowControl(DtBcBURSTFIFO*, Int* pFlowControl);
DtStatus  DtBcBURSTFIFO_SetDirection(DtBcBURSTFIFO*, Int Direction);
DtStatus  DtBcBURSTFIFO_SetOperationalMode(DtBcBURSTFIFO*, Int OpMode);
DtStatus  DtBcBURSTFIFO_SetRxFlowControl(DtBcBURSTFIFO*, Int  FlowControl);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcBURSTFIFO definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an BURSTFIFO Block
typedef struct _DtIoStubBcBURSTFIFO
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcBURSTFIFO;

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO public functions -.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcBURSTFIFO_Close(DtIoStub*);
DtIoStubBcBURSTFIFO*  DtIoStubBcBURSTFIFO_Open(DtBc*);

#endif  // #ifndef __DT_BC_BURSTFIFO_H
