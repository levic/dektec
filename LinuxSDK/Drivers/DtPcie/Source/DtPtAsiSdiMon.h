// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPtAsiSdiMon.h *#*#*#*#*#*#*#*#*#* (C) 2019 DekTec
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2019 DekTec Digital Video B.V.
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

#ifndef __DT_DF_ASISDIMON_H
#define __DT_DF_ASISDIMON_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtPt.h"
#include "DtDfSdiTxPhy.h"
#include "DtDfSpiCableDrvEq.h"


// Name for the AsiSdiRxTx port.
#define DT_DF_ASISDIMON_NAME   "ASISDIMON"


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtPtAsiSdiMon definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.DtPtAsiSdiMon -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtPtAsiSdiMon
{
    // NOTE: common port data must be the first members to allow casting to DtPt
    DT_PT_COMMON_DATA;

    // TX Driver functions
    DtDfSdiTxPhy*  m_pDfSdiTxPhy;       // SDI transmitter including PHY for ASI
    DtDfSpiCableDrvEq*  m_pDfSpiCableDrvEq; // Cable driver/equalizer
} DtPtAsiSdiMon;


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void DtPtAsiSdiMon_Close(DtPt*);
DtPtAsiSdiMon* DtPtAsiSdiMon_Open(DtCore*  pCore, Int  PortIndex, DtPortType);


#endif  // #ifndef __DT_DF_ASISDIMON_H

