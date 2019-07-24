// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfAsiRx.h *#*#*#*#*#*#*#*# (C) 2017-2018 DekTec
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

#ifndef __DT_DF_ASIRX_H
#define __DT_DF_ASIRX_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcASIRXP.h"
#include "DtBcTSRXFMT.h"

// Name + short-name for the ASITSRX function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_ASIRX_NAME          "AsiRx"
#define DT_DF_ASIRX_SHORTNAME     "ASIRX"

// MACRO: to init an driver-function-ID for the ASITSRX-DF
#define DT_DF_ASIRX_INIT_ID(ID, ROLE, INSTANCE, UUID)                                    \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_ASIRX_NAME, DT_DF_ASIRX_SHORTNAME, ROLE, INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfAsiRx definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

typedef struct _PolarityControl
{
    DtSpinLock  m_PolaritySpinLock; // Access protection for PolarityControl data
    Int   m_PolarityCtrl;           // Current polarity control setting
    Int   m_BcPolarity;             // Current selected ASI polarity in ASIRXP BC
    Int   m_DetectState;            // Polarity detection state: LOCKED or UNKNOWN
    UInt  m_TsWordCntThresshold;    // TS word count thresshold for polarity inverse
}PolarityControl;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfAsiRx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtDfAsiRx
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    PolarityControl  m_PolControl;  // Polarity control
    
    // Cached values
    Int  m_OperationalMode;     // Operational mode
    
    // Periodic interval 
    DtSpinLock  m_PerItvSpinLock;   // Spinlock to protect m_PerItvEnable changes
    Bool  m_PerItvEnable;           // Periodic interval hander enabled

    // Block controllers
    DtBcASIRXP*  m_pBcAsiRxP;       // ASIRXP block-controller
    DtBcTSRXFMT* m_pBcTsRxFmt;      // TSRXFMT block-controller

}  DtDfAsiRx;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfAsiRx_Close(DtDf*);
DtDfAsiRx*  DtDfAsiRx_Open(DtCore*, DtPt*  pPt, const char*  pRole, Int  Instance, 
                                                             Int  Uuid, Bool  CreateStub);
DtStatus  DtDfAsiRx_GetOperationalStatus(DtDfAsiRx*, Int* pOpStatus);
DtStatus  DtDfAsiRx_GetPacketMode(DtDfAsiRx*, Int* pPckMode);
DtStatus  DtDfAsiRx_GetPolarityCtrl(DtDfAsiRx*, Int* pPolarityCtrl);
DtStatus  DtDfAsiRx_GetStatus(DtDfAsiRx*, Int* pPckSize, Int* pCarrierDet,
                                                        Int* pAsiLock, Int* pDetPolarity);
DtStatus  DtDfAsiRx_GetSyncMode(DtDfAsiRx*, Int* pSyncMode);
DtStatus  DtDfAsiRx_GetTsBitrate(DtDfAsiRx*, Int* pTsBitrate);
DtStatus  DtDfAsiRx_GetViolCount(DtDfAsiRx*, UInt* pViolCount);
DtStatus  DtDfAsiRx_SetOperationalMode(DtDfAsiRx*, Int OpMode);
DtStatus  DtDfAsiRx_SetPacketMode(DtDfAsiRx*,  Int PckMode);
DtStatus  DtDfAsiRx_SetPolarityCtrl(DtDfAsiRx*, Int PolarityCtrl);
DtStatus  DtDfAsiRx_SetSyncMode(DtDfAsiRx*, Int SyncMode);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfAsiRx definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an ASITSRX Driver-Function
typedef struct _DtIoStubDfAsiRx
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfAsiRx;

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfAsiRx public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubDfAsiRx_Close(DtIoStub*);
DtIoStubDfAsiRx*  DtIoStubDfAsiRx_Open(DtDf*);

#endif  // #ifndef __DT_DF_ASIRX_H

