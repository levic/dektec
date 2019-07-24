//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSdiTxPhy.h *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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

#ifndef __DT_DF_SDITXPHY_H
#define __DT_DF_SDITXPHY_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcSDITXPHY.h"
#include "DtBcSDITXPLL.h"
#include "DtBcASITXG.h"
#include "DtBcSDITXP.h"
#include "DtDfSdiXCfgMgr.h"
#include "DtDfSi534X.h"

// Name + short-name for the SDITXPHY function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_SDITXPHY_NAME          "SdiTxPhy"
#define DT_DF_SDITXPHY_SHORTNAME     "SDITXPHY"

// MACRO: to init an driver-function-ID for the SDITXPHY-DF
#define DT_DF_SDITXPHY_INIT_ID(ID, ROLE, INSTANCE, UUID)                               \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_SDITXPHY_NAME, DT_DF_SDITXPHY_SHORTNAME, ROLE,           \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// SDI-lock states
typedef enum _DtDfSdiTxState
{
    SDITXPHY_STATE_RECONFIG,
    SDITXPHY_STATE_WAIT_FOR_PLL_LOCK,
    SDITXPHY_STATE_WAIT_FOR_PHY_READY,
    SDITXPHY_STATE_TX_LOCKED,
}DtDfSdiTxState;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiTxPhy -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtDfSdiTxPhy
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // Block controllers
    DtBcSDITXPHY*    m_pBcSdiTxPhy;     // SdiTxPhy block-controller
    DtBcSDITXPLL*    m_pBcSdiTxPll;     // SdiTxPll block-controller
    DtDfSdiXCfgMgr*  m_pDfSdiXCfgMgr;   // SDI Transceiver Reconfig Manager
    DtDfSi534X*  m_pDfSi534X;           // Silabs SI534X clock controller

    DtSpinLock  m_SpinLock;             // Lock to protect against mutual access of 
                                        // m_LockState and m_SdiMode
    DtDfSdiTxState  m_LockState;        // Lock state

    // Periodic interval 
    DtSpinLock  m_PerItvSpinLock;   // Spinlock to protect m_PerItvEnable changes
    Bool  m_PerItvEnable;           // Periodic interval hander enabled
                                    
    // Cached values
    Int  m_OperationalMode;     // Operational mode
    Int  m_TxMode;              // ASI or SDI
    Int  m_SdiRate;             // SDI-rate
    Bool  m_FractionalClock;    // Fractional clock
    Int  m_PhyNumClocks;        // Number of clocks for the PHY (1 or 2)
    Int  m_PhyDeviceFamily;     // TxPhy device family
    Int  m_PhyMaxSdiRate;       // TxPhy maximum SDI-rate
}  DtDfSdiTxPhy;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfSdiTxPhy_Close(DtDf*);
DtDfSdiTxPhy*  DtDfSdiTxPhy_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                             Int  Instance,  Int  Uuid, Bool  CreateStub);
DtStatus  DtDfSdiTxPhy_ClearUnderflowFlag(DtDfSdiTxPhy*);
DtStatus  DtDfSdiTxPhy_GetOperationalMode(DtDfSdiTxPhy*, Int*  pOpMode);
DtStatus  DtDfSdiTxPhy_GetSdiRate(DtDfSdiTxPhy*,Int* pSdiRate,  Bool*  pFractional);
DtStatus  DtDfSdiTxPhy_GetSdiStatus(DtDfSdiTxPhy*, Int*  pTxLock);
DtStatus  DtDfSdiTxPhy_GetTxMode(DtDfSdiTxPhy*, Int*  pTxMode);
DtStatus  DtDfSdiTxPhy_GetUnderflowFlag(DtDfSdiTxPhy*, Bool*  pUflFlag);
DtStatus  DtDfSdiTxPhy_SetOperationalMode(DtDfSdiTxPhy*, Int OpMode);
DtStatus  DtDfSdiTxPhy_SetSdiRate(DtDfSdiTxPhy*, Int  SdiRate, Bool  Fractional);
DtStatus  DtDfSdiTxPhy_SetTxMode(DtDfSdiTxPhy*, Int  TxMode);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfSdiTxPhy definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an SDITXPHY Driver-Function
typedef struct _DtIoStubDfSdiTxPhy
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfSdiTxPhy;

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiTxPhy public functions -.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubDfSdiTxPhy_Close(DtIoStub*);
DtIoStubDfSdiTxPhy*  DtIoStubDfSdiTxPhy_Open(DtDf*);

#endif  // #ifndef __DT_DF_SDITXPHY_H

