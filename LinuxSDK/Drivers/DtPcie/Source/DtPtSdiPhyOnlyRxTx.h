// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPtSdiPhyOnlyRxTx.h *#*#*#*#*#*#*#*#* (C) 2020 DekTec
//
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

#ifndef __DT_DF_SDIPHYONLYRXTX_H
#define __DT_DF_SDIPHYONLYRXTX_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtPt.h"
#include "DtBcSDITXP.h"
#include "DtBcSWITCH.h"
#include "DtDfChSdiRxPhyOnly.h"
#include "DtDfSdiRx.h"
#include "DtDfSdiTxPhy.h"
#include "DtDfSpiCableDrvEq.h"

// Name for the SDIPHYONLYRXTX port.
#define DT_DF_SDIPHYONLYRXTX_NAME   "SDIPHYONLYRXTX"


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtPtSdiPhyOnlyRxTx definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtPtSdiPhyOnlyRxTx
{
    // NOTE: common port data must be the first members to allow casting to DtPt
    DT_PT_COMMON_DATA;

    // TX Driver functions
    DtDfSdiTxPhy*  m_pDfSdiTxPhy;       // SDI transmitter including PHY for ASI

    // TX Block controllers
    DtBcSDITXP*  m_pBcSdiTxP;           // SDI-TX protocol
    DtBcSWITCH*  m_pBcSwitchTxDblBuf;   // Double-buffered buddy channel selector

    // RX Driver functions
    DtDfSdiRx*  m_pDfSdiRx;             // SDI receiver including PHY for ASI
    DtDfChSdiRxPhyOnly*  m_pDfChSdiRxPhyOnly;  // SDI PHY-only receive channel

    // RX TX Driver functions
    DtDfSpiCableDrvEq*  m_pDfSpiCableDrvEq; // Cable driver/equalizer

    Bool m_HoldExclAccessLock;      // True, when we hold the exclusive access lock 
                                    // for all children.
    Bool m_HoldChannelLock;         // True, when we hold the channel lock for the SDI 
                                    // PHY-only receive channel
} DtPtSdiPhyOnlyRxTx;


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void DtPtSdiPhyOnlyRxTx_Close(DtPt*);
DtPtSdiPhyOnlyRxTx* DtPtSdiPhyOnlyRxTx_Open(DtCore*  pCore, Int  PortIndex, DtPortType);

#endif  // #ifndef __DT_DF_SDIPHYONLYRXTX_H

