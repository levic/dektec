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
#include "DtBcEMAC25G.h"
#include "DtBcIPSUMCHK.h"
#include "DtBcIPSUMINS.h"
#include "DtDfIpFifo.h"
#include "DtBcIPROUT.h"
#include "DtBcIPSCHED.h"
#include "DtBcDDRFIFO.h"
#include "DtBcETHIPPAD.h"
#include "DtBcDATACDC.h"
#include "DtDfSfp.h"

// Name + short-name for the IpNrt function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_NW_NAME          "Network"
#define DT_DF_NW_SHORTNAME     "NW"

// MACRO: to init an driver-function-ID for the TEMPLATE-DF
#define DT_DF_NW_INIT_ID(ID, ROLE, INSTANCE, UUID)                                       \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_NW_NAME, DT_DF_NW_SHORTNAME, ROLE,                           \
                                                                     INSTANCE, UUID);    \
}                                                                                        \
while (0)

// -.-.-.-.-.-.-.-.-.-.-.-.-.- MAC interface function defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef DtStatus (*DtMacGetOperationalModeFunc)(DtBc* pBc, Int* pOpMode);
typedef DtStatus (*DtMacSetOperationalModeFunc)(DtBc* pBc, Int OpMode);
typedef DtStatus (*DtMacGetCounterFunc)(DtBc* pBc, const UInt CounterId, UInt64* pValue);
typedef DtStatus (*DtMacGetSupportedCountersFunc)(DtBc* pBc, UInt MaxCounterIds,
                                                 UInt* pNumCounterIds, UInt* pCounterIds);
typedef DtStatus (*DtMacGetPhySpeedFunc)(DtBc* pBc, UInt* pPhySpeed);
typedef DtStatus (*DtMacSetPhySpeedFunc)(DtBc* pBc, UInt PhySpeed);
typedef DtStatus (*DtMacGetMacAddressPermFunc)(DtBc* pBc, UInt8* pMacAddress);
typedef DtStatus (*DtMacGetMacAddressCurrFunc)(DtBc* pBc, UInt8* pMacAddress);
typedef DtStatus (*DtMacSetMacAddressFunc)(DtBc* pBc, UInt8* pMacAddress);
typedef DtStatus (*DtMacRegisterLinkCallbackFunc)(DtBc* pBc, DtLinkStatFunc LinkStatCB,
    void* pContext);
typedef DtStatus (*DtMacUnregisterLinkCallbackFunc)(DtBc* pBc, void* pContext);
typedef DtStatus (*DtMacGetLinkStatusFunc)(DtBc* pBc);
typedef void (*DtMacSetSfpIntCallbackFunc)(DtBc* pBc, DtSfpIntFunc SfpIntCB, 
                                                                          void* pContext);
typedef void (*DtMacSetSfpCapabilitiesFunc)(DtBc* pBc, UInt Capabilities);

typedef struct _DtEMACItf
{
    DtBc* pBcEMAC;
    DtMacGetOperationalModeFunc GetOperationalMode;
    DtMacSetOperationalModeFunc SetOperationalMode;
    DtMacGetCounterFunc GetCounter;
    DtMacGetSupportedCountersFunc GetSupportedCounters;
    DtMacGetPhySpeedFunc GetPhySpeed;
    DtMacSetPhySpeedFunc SetPhySpeed;
    DtMacGetMacAddressPermFunc GetMacAddressPerm;
    DtMacSetMacAddressFunc GetMacAddressCurr;
    DtMacSetMacAddressFunc SetMacAddress;
    DtMacRegisterLinkCallbackFunc RegisterLinkCallback;
    DtMacUnregisterLinkCallbackFunc UnregisterLinkCallback;
    DtMacGetLinkStatusFunc GetLinkStatus;
    DtMacSetSfpCapabilitiesFunc SetSfpCapabilities;
    DtMacSetSfpIntCallbackFunc SetSfpIntCallbackFunc;

} DtEMACItf;

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define MAX_NUMBER_RT_HW_PIPES 8
#define MAX_MULTICAST_ITEMS 100


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
    DtBcIPSUMCHK* m_pBcIpSumChk;
    DtBcIPSUMINS* m_pBcIpSumIns;
    DtBcIPSCHED* m_pBcIpSched;

    // =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Driver functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
    DtDfSfp* m_pDfSfp;
    
    // +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ EMAC Interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
    DtEMACItf m_EMACItf;

    // +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Threads/DPC +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

    // Periodic interval 
    DtSpinLock  m_PerItvSpinLock;   // Spinlock to protect m_PerItvEnable changes
    Bool  m_PerItvEnable;           // Periodic interval hander enabled
    DtVectorPipe* m_PerIntEvtList;  // All pipe events interested in the PerInt event
    DtFastMutex m_PerIntEvtFMutex;

    // RX HWQ thread
    DtThread m_RxHwqThread;         // Helper thread
    DtEvent m_RxHwqStopEvent;       // Helper stop event

    // RX SWP port mapping
    DtRxRtSwpLinkedListEl* m_RxRtSwpPortMap[0xFFFF+1];
    DtFastMutex m_RxRtSwpPortMapFMutex;

    // Multicast list
    DtFastMutex m_MulticastListFastMutex;   // Mutex to protect m_MulticastList
    UInt8 m_MulticastList[MAX_MULTICAST_ITEMS][6];
    UInt m_NumMulticasts;
    UInt m_PacketFilter;

    // TX RT HWQ thread
    DtThread m_TxRtHwqThread;       // Realtime transmit HWQ thread
    DtEvent m_TxRtHwqStopEvent;     // Stop event HWQ thread

    // Cached values
    Int  m_OperationalMode;         // Operational mode

    // Operational mode callback network driver
    OpModeFunc  m_OpModeCB;
    void* m_pContexOpModeCB;
    DtSpinLock  m_OpModeCBSpinlock;

    Bool m_ReducedFunctionality;
    Int m_PacketAlignment;
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

