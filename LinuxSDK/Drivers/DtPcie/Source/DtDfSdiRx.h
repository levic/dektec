//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSdiRx.h *#*#*#*#*#*#*#*#* (C) 2017-2018 DekTec
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

#ifndef __DT_DF_SDIRX_H
#define __DT_DF_SDIRX_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcSDIRXP.h"
#include "DtBcSDIRXPHY.h"
#include "DtDfSdiXCfgMgr.h"

// Name + short-name for the SDIRX function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_SDIRX_NAME          "SdiRx"
#define DT_DF_SDIRX_SHORTNAME     "SDIRX"

// MACRO: to init an driver-function-ID for the SDIRX-DF
#define DT_DF_SDIRX_INIT_ID(ID, ROLE, INSTANCE, UUID)                                    \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_SDIRX_NAME, DT_DF_SDIRX_SHORTNAME, ROLE,                     \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSdiRx definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// SDI-lock states
typedef enum _DtDfSdiRxState
{
    SDIRX_STATE_INIT_XCVR,
    SDIRX_STATE_WAIT_SDI_LOCKED_TO_REF1,
    SDIRX_STATE_CHECK_SDI_LOCK1,
    SDIRX_STATE_SDI_LOCKED,
    SDIRX_STATE_SDI_VALID,
    SDIRX_STATE_SDI_DELAY,
    SDIRX_STATE_WAIT_SDI_LOCKED_TO_REF2,
    SDIRX_STATE_CHECK_SDI_LOCK2,
    SDIRX_STATE_WAIT_ASI_LOCKED_TO_REF,
    SDIRX_STATE_WAIT_ASI_CARRIER_DETECT,
    SDIRX_STATE_ASI_CARRIER_DETECTED,
}DtDfSdiRxState;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiRx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtDfSdiRx
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // Block controllers
    DtBcSDIRXP*  m_pBcSdiRxProt;        // SdiRxProtocol block-controller
    DtBcSDIRXPHY*  m_pBcSdiRxPhy;       // SdiRxPhy block-controller
    DtDfSdiXCfgMgr*  m_pDfSdiXCfgMgr;   // SDI Transceiver Reconfig Manager

    DtSpinLock  m_SpinLock;             // Lock to protect against mutual access of 
                                        // m_LockState and m_SdiMode
    DtDfSdiRxState  m_LockState;        // Lock state
    DtTodTime  m_StateTime;             // Time when entered new state
    Int  m_DelayCount;                  // Delay counter

    // Periodic interval 
    DtSpinLock  m_PerItvSpinLock;   // Spinlock to protect m_PerItvEnable changes
    Bool  m_PerItvEnable;           // Periodic interval hander enabled

    // Cached values
    Int  m_OperationalMode;     // Operational mode
    Int  m_CurrentSdiRate;      // Current SDI-rate
    Int  m_LastLockedSdiRate;   // Last locked SDI-rate
    Int  m_RxMode;              // ASI or SDI
    Int  m_ConfigSdiRate;       // Configured SDI-rate
    Int  m_PhyMaxSdiRate;       // Phy maximum SDI-rate
    Int  m_PhyDeviceFamily;     // Phy device family
}  DtDfSdiRx;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfSdiRx_Close(DtDf*);
DtDfSdiRx*  DtDfSdiRx_Open(DtCore*, DtPt*  pPt, const char*  pRole, Int  Instance, 
                                                             Int  Uuid, Bool  CreateStub);
DtStatus  DtDfSdiRx_GetMaxSdiRate(DtDfSdiRx*,Int* pMaxSdiRate);
DtStatus  DtDfSdiRx_GetOperationalMode(DtDfSdiRx*, Int* pOpMode);
DtStatus  DtDfSdiRx_GetRxMode(DtDfSdiRx*, Int* pRxMode);
DtStatus  DtDfSdiRx_GetSdiRate(DtDfSdiRx*,Int* pSdiRate);
DtStatus  DtDfSdiRx_GetSdiStatus(DtDfSdiRx*, Int* pCarrierDetect, Int* pSdiLock, 
                                Int* pLineLock,  Int* pValid, Int* pNumSymsHanc, 
                                Int* pNumSymsVidVanc, Int* pNumLinesF1, Int* pNumLinesF2, 
                                Int* pIsLevelB, UInt32* pPayloadId, Int* pFramePeriod);
DtStatus  DtDfSdiRx_SetOperationalMode(DtDfSdiRx*, Int OpMode);
DtStatus  DtDfSdiRx_SetRxMode(DtDfSdiRx*, Int RxMode);
DtStatus  DtDfSdiRx_SetSdiRate(DtDfSdiRx*, Int SdiRate);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfSdiRx definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiRx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an SDIRX Driver-Function
typedef struct _DtIoStubDfSdiRx
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfSdiRx;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSdiRx public functions -.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtIoStubDfSdiRx_Close(DtIoStub*);
DtIoStubDfSdiRx*  DtIoStubDfSdiRx_Open(DtDf*);

#endif  // #ifndef __DT_DF_SDIRX_H

