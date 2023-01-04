// =+=+=+=+=+=+=+=+=+ DtBcDISEQC.h *#*#*#*#*#*#*#*#*#* (C) 2020 DekTec +=+=+=+=+=+=+=+=+=+
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

#ifndef __DT_BC_DISEQC_H
#define __DT_BC_DISEQC_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

#define DT_BC_DISEQC_NAME        "Lnbh25DiSeqC"
#define DT_BC_DISEQC_SHORTNAME   "DISEQC"

// MACRO: to init an block-controller-ID for the DISEQC-BC
#define DT_BC_DISEQC_INIT_ID(ID, ROLE, INSTANCE, UUID)                                   \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_DISEQC_NAME, DT_BC_DISEQC_SHORTNAME,  ROLE,                  \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDISEQC definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDISEQC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef  struct _DtBcDISEQC
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    DtFastMutex  m_AccessMutex;     // Access protection for the receiver/transmitter

    // Interrupts related
    DtDpc  m_IntDpc;
    DtEvent  m_RxDoneEvent;         // Reception done event
    DtEvent  m_TxDoneEvent;         // Transmit done event
    DtEvent  m_ErrorEvent;          // Receiver done event
    Bool  m_Failure;                // Transmit/receive error indication
    
    // Cached config
    Int  m_RxFifoSize;              // RX-Fifo Size
    Int  m_TxFifoSize;              // TX-Fifo Size
}  DtBcDISEQC;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDISEQC public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtBcDISEQC_Close(DtBc*);
DtBcDISEQC*  DtBcDISEQC_Open(Int  Address, DtCore*, DtPt*  pPt, 
                          const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub);
DtStatus  DtBcDISEQC_SendToneBurst(DtBcDISEQC*, Int ToneBurst);
DtStatus  DtBcDISEQC_EnableTone(DtBcDISEQC*, Int EnableTone);
DtStatus  DtBcDISEQC_SendMessage(DtBcDISEQC*, const UInt8* pMsgSend,
                                            Int NumToSend, UInt8* pMsgRcv, Int* pNumRcvd);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDISEQC definitions +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDISEQC -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an DISEQC Block
typedef struct _DtIoStubBcDISEQC
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcDISEQC;


//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDISEQC public functions -.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubBcDISEQC_Close(DtIoStub*);
DtIoStubBcDISEQC*  DtIoStubBcDISEQC_Open(DtBc*);


#endif  // #ifndef __DT_BC_DISEQC_H