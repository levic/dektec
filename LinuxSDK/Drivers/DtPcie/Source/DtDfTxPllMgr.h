// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfTxPllMgr.h *#*#*#*#*#*#*#*#*#*# (C) 2020 DekTec
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

#ifndef __DT_DF_TXPLLMGR_H
#define __DT_DF_TXPLLMGR_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcSDITXPLL.h"

// Name + short-name for the TXPLLMGR function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_TXPLLMGR_NAME          "TxPllMgr"
#define DT_DF_TXPLLMGR_SHORTNAME     "TXPLLMGR"

// MACRO: to init an driver-function-ID for the ASITSRX-DF
#define DT_DF_TXPLLMGR_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_TXPLLMGR_NAME, DT_DF_TXPLLMGR_SHORTNAME, ROLE, INSTANCE,     \
                                                                                   UUID);\
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfTxPllMgr definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Structure that stores the TXPLL block controller and the PLL-Id
typedef struct  _DtDfTxPllMgrTxPll
{
    DtBcType  m_BcTxPllType;        // Block controller type  (SDITXPLL or HDMITXPLL) 
    DtBc*  m_pBcTxPll;              // Block controller
    Int  m_PllId;                   // PLL-ID
}DtDfTxPllMgrTxPll;


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxPllMgr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtDfTxPllMgr
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    Bool  m_CalibrateOnStartUp;         // Calibrate PLL on start-up
    DtVectorBc*  m_pBcSdiTxPlls;        // SDITXPLL block controllers
    //DtVectorBc*  m_pBcHdmiTxPlls;     // HDMITXPLL block controllers
    DtVector*  m_pTxPllTable;           // TXPLL look-up table
    DtFastMutex  m_AccessMutex;         // Access protection
}  DtDfTxPllMgr;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfTxPllMgr_Close(DtDf*);
DtDfTxPllMgr*  DtDfTxPllMgr_Open(DtCore*, DtPt*  pPt, const char*  pRole, 
                                                              Int  Instance,  Int  Uuid);
DtStatus  DtDfTxPllMgr_IsPllLocked(DtDfTxPllMgr*, Int PllId, Bool* pLocked);
DtStatus  DtDfTxPllMgr_Calibrate(DtDfTxPllMgr*);

#endif  // #ifndef __DT_DF_TXPLLMGR_H

