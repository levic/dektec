//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPtAsiSdiRxTx.h *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
//
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

#ifndef __DT_DF_ASISDIRXTX_H
#define __DT_DF_ASISDIRXTX_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtPt.h"
#include "DtDfSdiTxPhy.h"
#include "DtDfSi534X.h"
#include "DtBcASITXG.h"
#include "DtBcASITXSER.h"
#include "DtBcBURSTFIFO.h"
#include "DtBcCDMAC.h"
#include "DtBcCONSTSINK.h"
#include "DtBcCONSTSOURCE.h"
#include "DtBcGS2988.h"
#include "DtBcKA.h"
#include "DtBcSDIDMX12G.h"
#include "DtBcSDIMUX12G.h"
#include "DtBcSDIRXF.h"
#include "DtBcSDITXF.h"
#include "DtBcSDITXP.h"
#include "DtBcST425LR.h"
#include "DtBcSWITCH.h"
#include "DtDfAsiRx.h"
#include "DtDfSdiRx.h"
#include "DtDfSpiCableDrvEq.h"
#include "DtDfTodClockCtrl.h"


// Name for the AsiSdiRxTx port.
#define DT_DF_ASISDIRXTX_NAME   "ASISDIRXTX"


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+DtPtAsiSdiRxTx definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.DtPtAsiSdiRxTx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtPTAsiSdiRxTx
{
    // NOTE: common port data must be the first members to allow casting to DtPt
    DT_PT_COMMON_DATA;

    // TX Driver functions
    DtDfSdiTxPhy*  m_pDfSdiTxPhy;       // SDI transmitter including PHY for ASI

    // TX Block controllers
    DtBcASITXG*  m_pBcAsiTxG;           // ASI transmitter gate
    DtBcASITXSER*  m_pBcAsiTxSer;       // ASI transmitter serializer
    DtBcSWITCH*  m_pBcSwitchFrontEndTx; // Output side switch
    DtBcSWITCH*  m_pBcSwitchBackEndTx;  // DMA-side switch
    DtBcSDITXF*  m_pBcSdiTxF;           // SDI-TX formatter simple
    DtBcSDITXP*  m_pBcSdiTxP;           // SDI-TX protocol
    DtBcKA*  m_pBcKeepAlive;            // Keep-alive for failover circuit
    DtBcGS2988*  m_pBcGs2988;           // GS2988 cable driver controller
    DtBcSWITCH*  m_pBcSwitchTestModeTx; // DMA-testmode switch
    DtBcCONSTSINK*  m_pBcConstSink;     // Constant rate sink block controller

    // 12G TX Block controllers
    DtBcSWITCH*  m_pBcSwitchSdiTxDmxOut; // SDI mux 12G output switch
    DtBcSDIDMX12G*  m_pBcSdiTxDmx12G;    // SDI demux 12G
    DtBcSWITCH*  m_pBcSwitchSdiTxDmxIn;  // SDI mux 12G input switch
    DtBcSWITCH*  m_pBcSwitchSdiTxFrom4LinkMaster;  // SDI from quad-link master switch


    // RX Driver functions
    DtDfAsiRx*  m_pDfAsiRx;             // ASI receiver
    DtDfSdiRx*  m_pDfSdiRx;             // SDI receiver including PHY for ASI

    // RX Block controllers
    DtBcSWITCH*  m_pBcSwitchFrontEndRx;   // Input side switch
    DtBcSWITCH*  m_pBcSwitchBackEndRx;    // DMA-side switch
    DtBcSDIRXF*  m_pBcSdiRxF;             // SDI-RX formatter simple
    DtBcSWITCH*  m_pBcSwitchTestModeRx;   // DMA-testmode switch
    DtBcCONSTSOURCE*  m_pBcConstSource;   // Constant rate source block controller

    // 12G RX Block controllers
    DtBcSWITCH*  m_pBcSwitchSdiRxMuxIn;  // SDI mux 12G input switch
    DtBcSDIMUX12G*  m_pBcSdiRxMux12G;    // SDI mux 12G
    DtBcST425LR*  m_pBcSdiRxSt425Lr;     // ST425 Link reorder
    DtBcSWITCH*  m_pBcSwitchSdiRxMuxOut; // SDI mux 12G output switch
    DtBcSWITCH*  m_pBcSwitchSdiRxTo4LinkMaster; // SDI to quad-link master switch

    // RX TX Block controllers
    DtBcCDMAC*  m_pBcCDmaC;               // DMA-controller
    DtBcBURSTFIFO*  m_pBcBURSTFIFO;       // Burst FIFO
    
    // Doubly buffered output controller
    DtBcSWITCH*  m_pBcSwitchTxDblBuf;     // Double-buffered buddy channel selector

    // RX TX Driver functions
    DtDfSpiCableDrvEq*  m_pDfSpiCableDrvEq; // Cable driver/equalizer
    
    // Common for all ports
    DtDfTodClockCtrl*  m_pDfTodClockCtrl;   // TimeOfDay clock control
} DtPtAsiSdiRxTx;


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void DtPtAsiSdiRxTx_Close(DtPt*);
DtPtAsiSdiRxTx* DtPtAsiSdiRxTx_Open(DtCore*  pCore, Int  PortIndex, DtPortType);


#endif  // #ifndef __DT_DF_ASISDIRXTX_H

