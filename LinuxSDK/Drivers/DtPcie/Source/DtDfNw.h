// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfNw.h *#*#*#*#*#*#*#*#*# (C) 2020-2021 DekTec
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

#ifndef __DT_DF_NW_H
#define __DT_DF_NW_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtVectorPipe.h"
#include "DtBcBURSTFIFO.h"
#include "DtBcCDMACTO.h"
#include "DtBcEMAC10G.h"
#include "DtBcIPSUMCHK.h"
#include "DtBcIPSUMINS.h"
#include "DtDfIpFifo.h"
#include "DtBcIPROUT.h"
#include "DtBcIPSCHED.h"
#include "DtBcDDRFIFO.h"
#include "DtBcETHIPPAD.h"
#include "DtBcDATACDC.h"

// Name + short-name for the IpNrt function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_NW_NAME          "Network"
#define DT_DF_NW_SHORTNAME     "NW"

// MACRO: to init an driver-function-ID for the TEMPLATE-DF
#define DT_DF_NW_INIT_ID(ID, ROLE, INSTANCE, UUID)                                    \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_NW_NAME, DT_DF_NW_SHORTNAME, ROLE,                     \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define MAX_NUMBER_RT_HW_PIPES 12

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtDfNw
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Pipe list +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
    Int m_IdMapRxNrt;
    Int m_IdMapTxNrt;
    Int m_IdMapRxHwq;
    Int m_IdMapTxRtHwq;
    Int m_NumStaticPipes;

    DtVectorPipe* m_pPipeIdMap;             // Mapping Pipe ID to Pipe struct
    DtVectorPipe* m_pPipeRxRtHwpList;
    DtVectorPipe* m_pPipeTxRtHwpList;
    DtVectorPipe* m_pPipeRxRtSwpList;
    DtVectorPipe* m_pPipeTxRtSwpList;
    DtFastMutex m_PipeIdMapFMutex;
    DtFastMutex m_PipeRxRtHwpListFMutex;
    DtFastMutex m_PipeTxRtHwpListFMutex;
    DtFastMutex m_PipeRxRtSwpListFMutex;
    DtFastMutex m_PipeTxRtSwpListFMutex;
    DtFastMutex m_PipeRxNrtFMutex;      // Protects all Rx NRT pipe/list actions
    DtFastMutex m_PipeTxNrtFMutex;      // Protects all Tx NRT pipe/list actions

    // =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Block controllers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
    // Common
    DtBcEMAC10G* m_pBcEMAC10G;
    DtBcIPSUMCHK* m_pBcIpSumChk;
    DtBcIPSUMINS* m_pBcIpSumIns;
    DtBcIPSCHED* m_pIpSched;

    // +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Threads/DPC +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

    // Periodic interval 
    DtSpinLock  m_PerItvSpinLock;   // Spinlock to protect m_PerItvEnable changes
    Bool  m_PerItvEnable;           // Periodic interval hander enabled
    DtVectorPipe* m_PerIntEvtList;  // All pipe events who want's to have the PerInt event
    DtFastMutex m_PerIntEvtFMutex;

    // RX HWQ thread
    DtThread m_RxHwqThread;         // Helper thread
    DtEvent m_RxHwqStopEvent;       // Helper stop event

    // TX RT HWQ thread
    DtThread m_TxRtHwqThread;       // Helper thread
    DtEvent m_TxRtHwqStopEvent;     // Helper stop event

    // Cached values
    Int  m_OperationalMode;         // Operational mode

    // Operational mode callback network driver
    OpModeFunc  m_OpModeCB;
    void* m_pContexOpModeCB;
    DtSpinLock  m_OpModeCBSpinlock;
}DtDfNw;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfNw_Close(DtDf*);
DtDfNw*  DtDfNw_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                             Int  Instance,  Int  Uuid, Bool  CreateStub);
DtStatus  DtDfNw_GetOperationalMode(DtDfNw*, Int*  pOpMode);
DtStatus  DtDfNw_SetOperationalMode(DtDfNw*, Int OpMode);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfNw definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//  Io-control stub for an Nw Driver-Function
typedef struct _DtIoStubDfNw
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfNw;

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubDfNw_Close(DtIoStub*);
DtIoStubDfNw*  DtIoStubDfNw_Open(DtDf*);



#endif  // #ifndef __DT_DF_NW_H

