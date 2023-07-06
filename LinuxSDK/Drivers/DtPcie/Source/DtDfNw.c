// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfNw.c *#*#*#*#*#*#*#*#*# (C) 2020-2021 DekTec
//
// This file is not part of the driver. It can be used as template to create a new
// driver function. Replace "TEMPLATE" by the driver function's short name 
// (all uppercase) and replace "Template" by the driver function's short name in camel
// case.
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDfNw.h"
#include "EthPrtcls.h"
#include "Messages.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        
#define ROLE_RX_HWQ        "RX_HWQ"
#define ROLE_TX_NRT_HWQ    "TX_NRT_HWQ"
#define ROLE_TX_RT_HWQ     "TX_RT_HWQ"
#define ROLE_RX_HWP        "RX_RT_HWP*"
#define ROLE_TX_HWP        "TX_RT_HWP*"

#define BC_EMAC10G_ROLE_NONE    NULL
#define BC_IP_SUMCHK_ROLE_NONE  NULL
#define BC_IP_SUMINS_ROLE_NONE  NULL
#define BC_IP_SCHED_ROLE_NONE   NULL
#define BC_ROLE_NONE            NULL

#define RX_HWQ_BUFSIZE            (128*1024*1024)
#define TX_RT_HWQ_BUFSIZE         (128*1024*1024)

#define NW_TAG  0x574e4644   // 'WNFD'

#define MAX_NUM_PIPES_EACH_PORT 2047

#define MAX_DELAY_TX_RT_HWP_MS 10000   // 10 seconds for now
#define MAX_DELAY_TX_RT_HWQ_MS 40      // 40 ms for now


const int PerIntInNs = 10000000;
const int NsInSec = 1000 * 1000 * 1000;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfNw implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DTDFNW_PIPE   ((DtDfNw*)pPipe->m_pPipeOwner)

// MACRO with default precondition checks for the DtDfNw function
#define DF_NW_DEFAULT_PRECONDITIONS(pDf)      \
                                 DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfNw))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_NW_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(NW, pDf)

#define DtDbgOutPipe(Level, pPipe, Msg, ...)                                             \
                DtDbgOutDf(Level, NW, DTDFNW_PIPE, "[%s:Id%i] " Msg,                     \
                                        PipeType2Str(pPipe->m_PipeType), pPipe->m_Id,    \
                                        ##__VA_ARGS__)

#define ETH_IS_MULTICAST(Address) \
    (Bool)(((UInt8*)(Address))[0] & ((UInt8)0x01))

#define ETH_IS_BROADCAST(Address)               \
    ((((UInt8*)(Address))[0]==((UInt8)0xff)) && (((UInt8*)(Address))[1]==((UInt8)0xff))  \
 && (((UInt8*)(Address))[2]==((UInt8)0xff)) && (((UInt8*)(Address))[3]==((UInt8)0xff))   \
 && (((UInt8*)(Address))[4]==((UInt8)0xff)) && (((UInt8*)(Address))[5]==((UInt8)0xff)))


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfNw_Init(DtDf*);

static DtStatus  DtDfNw_LoadParameters(DtDf*);
static DtStatus  DtDfNw_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfNw_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfNw_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfNw_OpenChildren(DtDfNw*);
static void  DtDfNw_RxHwqThread(DtThread*, void* pContext);
static void  DtDfNw_TxRtHwqThread(DtThread*, void* pContext);
static void  DtDfNw_EnablePeriodicIntervalHandler(DtDfNw*, Bool  Enable);
static void  DtDfNw_PeriodicIntervalHandler(DtObject*, DtTodTime  Time);
static void  DtDfNw_CleanupPipes(DtDfNw* pDf);
DtStatus DtDfNw_PipeOpen(DtDfNw* pDf, const DtFileObject* pFileObject, Int PipeType,
    UInt* pUuid, DtPipe** ppPipe);
DtStatus DtDfNw_PipeClose(DtDfNw* pDf, const DtFileObject* pFileObject, Int PipeType,
    UInt Uuid);
DtPipe* DtDfNw_FindPipe(DtDfNw* pDf, const DtFileObject* pFileObject);
DtStatus DtDfNw_RegisterKEvent(DtPipe* pPipe, DtEvent* pEvent, Int Id);
DtStatus DtDfNw_DeregisterKEvent(DtPipe* pPipe, DtEvent* pEvent, Int Id);
DtStatus DtDfNw_DeregisterEvent(DtPipe* pPipe, DtTrigEvtDriver* pTrigEvt);
DtStatus DtDfNw_CheckFilterDuplication(DtDfNw* pDf);


// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfNw - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfNw_Close(DtDf*  pDfBase)
{
    DtDfNw* pDf = (DtDfNw*)pDfBase;

    DF_NW_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_pCore->m_UnregisterChildDevice != NULL);

    // Report child we are going to idle
    DtSpinLockAcquire(&pDf->m_OpModeCBSpinlock);
    if (pDf->m_OpModeCB != NULL)
        pDf->m_OpModeCB(DT_FUNC_OPMODE_IDLE, pDf->m_pContexOpModeCB);
    DtSpinLockRelease(&pDf->m_OpModeCBSpinlock);

    // Unregister child device
    pDf->m_pCore->m_UnregisterChildDevice(pDf->m_pCore, (DtObject*)pDf->m_pPt);

    // Unregister periodic interrupt handler
    DtCore_TOD_PeriodicItvUnregister(pDf->m_pCore, (DtObject*)pDf);

    DtDfNw_CleanupPipes(pDf);
    
    // Let base function perform final clean-up
    DtDf_Close(pDfBase);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfNw*  DtDfNw_Open(DtCore* pCore, DtPt* pPt, const char* pRole, 
                                                  Int Instance, Int Uuid, Bool CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_NW_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfNw, Id, DT_FUNC_TYPE_NW, pPt,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfNw_Close;
    OpenParams.m_InitFunc = DtDfNw_Init;
    OpenParams.m_LoadParsFunc = DtDfNw_LoadParameters;
    OpenParams.m_OnCloseFileFunc = DtDfNw_OnCloseFile;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfNw_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfNw_OnEnablePreChildren;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfNw*)DtDf_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfNw_GetOperationalMode(DtDfNw* pDf, Int* pOpMode)
{
    // Sanity check
    DF_NW_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_NW_MUST_BE_ENABLED(pDf);

    // Return cached operational mode
    *pOpMode = pDf->m_OperationalMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfNw_SetOperationalMode(DtDfNw* pDf, Int OpMode)
{
    DtStatus  Status = DT_STATUS_OK;
    Int i;

    // Sanity checks
    DF_NW_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (OpMode!=DT_FUNC_OPMODE_IDLE && OpMode!=DT_FUNC_OPMODE_STANDBY
                                                            && OpMode!=DT_FUNC_OPMODE_RUN)
    {
        DtDbgOutDf(ERR, NW, pDf, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    DF_NW_MUST_BE_ENABLED(pDf);

    // No change?
    if (pDf->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Set operational mode of childs to the op mode selected
    DT_RETURN_ON_ERROR(DtBcEMAC10G_SetOperationalMode(pDf->m_pBcEMAC10G, OpMode));
    DT_RETURN_ON_ERROR(DtBcIPSUMCHK_SetOperationalMode(pDf->m_pBcIpSumChk, OpMode));
    DT_RETURN_ON_ERROR(DtBcIPSUMINS_SetOperationalMode(pDf->m_pBcIpSumIns, OpMode));

    // Set operational mode to all kernel pipes
    for (i = 0; i < pDf->m_NumStaticPipes; i++)
    {
        DtPipe* pPipe = DtVectorPipe_At(pDf->m_pPipeIdMap, i);
        int Count = 0;
        if ((pPipe->m_Capabilities & DT_PIPE_CAP_KBUF) == 0)
            continue;
        Status = DT_STATUS_NOT_INITIALISED;
        while (Status == DT_STATUS_NOT_INITIALISED && Count < 50)
        {
            Status = (DtPipe_SetOperationalMode(pPipe, OpMode));
            if (!DT_SUCCESS(Status))
                DtSleep(100);
            Count++;
        }
        if (!DT_SUCCESS(Status))
            DtDbgOutDf(ERR, NW, pDf, "Error setting operational mode for static pipe"
                                                                          " index %i", i);
    }

    // Save new operational mode
    if (DT_SUCCESS(Status))
        pDf->m_OperationalMode = OpMode;

    return Status;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfNw - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_CheckReducedFunctionalityMode -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfNw_CheckReducedFunctionalityMode(DtDfNw* pDf)
{
    DtBcDDRFIFO* pDdrFifo;
    pDf->m_ReducedFunctionality = TRUE;
    if (pDf->m_pCore->m_pBcList == NULL)
        return DT_STATUS_FAIL;
    pDdrFifo = (DtBcDDRFIFO*)DtVectorBc_FindByTypeAndRole(pDf->m_pBcList,
                                                             DT_BLOCK_TYPE_DDRFIFO, NULL);
    if (pDdrFifo == NULL)
        return DT_STATUS_FAIL;
    pDf->m_ReducedFunctionality = !DtBcDDRFIFO_IsMemCalibrated(pDdrFifo);
    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_InitPipeRxHwq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfNw_InitPipeRxHwq(DtDfNw* pDf, DtBcCDMACTO* pBcCdma, DtDfIpFifo* pDfIpFifo,
                                           DtBcETHIPPAD* pEthIpPad, DtBcDDRFIFO* pDdrFifo)
{
    DtStatus Status = DT_STATUS_OK;
    DtPipe* pPipe = DtPipe_Open(NULL, (DtObject*)pDf, DT_PIPE_RX_HWQ, 
                                                            pDf->m_NumStaticPipes + 1, 0);
    DtPipeRxHwq* pRx = (DtPipeRxHwq*)pPipe;
    if (pPipe == NULL)
    {
        DtDbgOutDf(ERR, NW, pDf, "ERROR: DtPipe_Open failed: DT_PIPE_RX_HWQ");
        return DT_STATUS_OUT_OF_MEMORY;
    }
    pRx->m_pBcCdma = pBcCdma;
    pRx->m_pDfIpFifo = pDfIpFifo;
    // Variant0 only:
    pRx->m_pEthIpPad = pEthIpPad;
    pRx->m_pDdrFifo = pDdrFifo;
    Status = DtVectorPipe_PushBack(pDf->m_pPipeIdMap, pPipe);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPipe(ERR, pPipe, "DtVectorPipe_PushBack. Error:%xh", Status);
        return Status;
    }
    pDf->m_NumStaticPipes++;
    pDf->m_IdMapRxHwq = pDf->m_NumStaticPipes;
    DT_ASSERT(pPipe->m_Id == DtVectorPipe_Size(pDf->m_pPipeIdMap));

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_InitPipeTxRtHwq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfNw_InitPipeTxRtHwq(DtDfNw* pDf, DtBcIPSCHED* pIpSched, DtBcCDMACTO* pBcCdma,
                                       DtBcBURSTFIFO* pBcBurstFifo, DtDfIpFifo* pDfIpFifo)
{
    DtStatus Status = DT_STATUS_OK;
    DtPipe* pPipe = DtPipe_Open(NULL, (DtObject*)pDf, DT_PIPE_TX_RT_HWQ, 
                                                              pDf->m_NumStaticPipes+1, 0);
    DtPipeTxRtHwq* pTxRt = (DtPipeTxRtHwq*)pPipe;
    if (pPipe == NULL)
    {
        DtDbgOutDf(ERR, NW, pDf, "ERROR: DtPipe_Open failed: DT_PIPE_TX_RT_HWQ");
        return DT_STATUS_OUT_OF_MEMORY;
    }
    pTxRt->m_pBcCdma = pBcCdma;
    pTxRt->m_pBurstFifo = pBcBurstFifo;
    pTxRt->m_pDfIpFifo = pDfIpFifo;

    // Initialise IP scheduler behavior
    DtBcIPSCHED_SetErrorBehavior(pIpSched, 0, TRUE);
    DtBcIPSCHED_SetMaxDelay(pIpSched, 0, MAX_DELAY_TX_RT_HWQ_MS);

    Status = DtVectorPipe_PushBack(pDf->m_pPipeIdMap, pPipe);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPipe(ERR, pPipe, "DtVectorPipe_PushBack. Error:%xh", Status);
        return Status;
    }
    pDf->m_NumStaticPipes++;
    pDf->m_IdMapTxRtHwq = pDf->m_NumStaticPipes;
    DT_ASSERT(pPipe->m_Id == DtVectorPipe_Size(pDf->m_pPipeIdMap));
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_InitPipeRxNrt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfNw_InitPipeRxNrt(DtDfNw* pDf)
{
    DtStatus Status = DT_STATUS_OK;
    DtPipe* pPipe = DtPipe_Open(NULL, (DtObject*)pDf, DT_PIPE_RX_NRT, 
                                                            pDf->m_NumStaticPipes + 1, 0);
    //DtPipeRxNrt* pRxNrt = (DtPipeRxNrt*)pPipe;
    if (pPipe == NULL)
    {
        DtDbgOutDf(ERR, NW, pDf, "ERROR: DtPipe_Open failed: DT_PIPE_RX_NRT");
        return DT_STATUS_OUT_OF_MEMORY;
    }
    
    Status = DtVectorPipe_PushBack(pDf->m_pPipeIdMap, pPipe);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPipe(ERR, pPipe, "DtVectorPipe_PushBack. Error:%xh", Status);
        return Status;
    }

    pDf->m_NumStaticPipes++;
    pDf->m_IdMapRxNrt = pDf->m_NumStaticPipes;
    DT_ASSERT(pPipe->m_Id == DtVectorPipe_Size(pDf->m_pPipeIdMap));
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_InitPipeTxNrt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfNw_InitPipeTxNrt(DtDfNw* pDf, DtBcCDMACTO* pBcCdma)
{
    DtStatus Status = DT_STATUS_OK;
    DtPipe* pPipe = DtPipe_Open(NULL, (DtObject*)pDf, DT_PIPE_TX_NRT, 
                                                            pDf->m_NumStaticPipes + 1, 0);
    DtPipeTxNrt* pTxNrt = (DtPipeTxNrt*)pPipe;
    if (pPipe == NULL)
    {
        DtDbgOutDf(ERR, NW, pDf, "ERROR: DtPipe_Open failed: DT_PIPE_TX_NRT");
        return DT_STATUS_OUT_OF_MEMORY;
    }
    
    Status = DtVectorPipe_PushBack(pDf->m_pPipeIdMap, pPipe);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPipe(ERR, pPipe, "DtVectorPipe_PushBack. Error:%xh", Status);
        return Status;
    }
    pDf->m_NumStaticPipes++;
    pDf->m_IdMapTxNrt = pDf->m_NumStaticPipes;
    pTxNrt->m_pBcCdma = pBcCdma;
    DT_ASSERT(pPipe->m_Id == DtVectorPipe_Size(pDf->m_pPipeIdMap));
    return Status;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_InitPipeTxRtHwp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfNw_InitPipeTxRtHwp(DtDfNw* pDf, DtBcIPSCHED* pIpSched,
                                      DtBcCDMACTO* pBcCdma[MAX_NUMBER_RT_HW_PIPES],
                                      DtBcBURSTFIFO* pBcBurstFifo[MAX_NUMBER_RT_HW_PIPES],
                                      DtDfIpFifo* pDfIpFifo[MAX_NUMBER_RT_HW_PIPES])
{
    DtStatus Status = DT_STATUS_OK;
    Int i;
    Int NumHwPipes = 0;
    // Count the number of hardware pipes
    for (i = 0; i < MAX_NUMBER_RT_HW_PIPES; i++)
    {
        if (pBcCdma[i] != NULL /* && pDfIpFifo[i] != NULL*/) // Variant0
            NumHwPipes++;
        else if (!(pBcCdma[i] == NULL/* && pDfIpFifo[i] == NULL*/))
        {
            DtDbgOutDf(ERR, NW, pDf, "ERROR: invalid xml config role: %s", ROLE_TX_HWP);
            return DT_STATUS_CONFIG_ERROR;
        }
    }
    pDf->m_pPipeTxRtHwpList = DtVectorPipe_Create(NumHwPipes, 0);

    for (i = 0; i < NumHwPipes; i++)
    {
        DtPipe* pPipe = DtPipe_Open(NULL, (DtObject*)pDf, DT_PIPE_TX_RT_HWP, 
                                                            pDf->m_NumStaticPipes + 1, i);
        DtPipeTxRtHwp* pHwp = (DtPipeTxRtHwp*)pPipe;
        if (pPipe == NULL)
        {
            DtDbgOutDf(ERR, NW, pDf, "ERROR: DtPipe_Open failed: DT_PIPE_TX_RT_HWP");
            return DT_STATUS_OUT_OF_MEMORY;
        }
        pHwp->m_pBcCdma = pBcCdma[i];
        pHwp->m_pBurstFifo = pBcBurstFifo[i];
        pHwp->m_pDfIpFifo = pDfIpFifo[i];
        pHwp->m_pIpSched = pIpSched;

        // Initialise IP scheduler behavior
        DtBcIPSCHED_SetErrorBehavior(pIpSched, i+1, FALSE);
        DtBcIPSCHED_SetMaxDelay(pIpSched, i+1, MAX_DELAY_TX_RT_HWP_MS);

        Status = DtVectorPipe_PushBack(pDf->m_pPipeIdMap, pPipe);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutPipe(ERR, pPipe, "DtVectorPipe_PushBack. Error:%xh", Status);
            return Status;
        }
        pDf->m_NumStaticPipes++;
        DT_ASSERT(pPipe->m_Id == DtVectorPipe_Size(pDf->m_pPipeIdMap));

        Status = DtVectorPipe_PushBack(pDf->m_pPipeTxRtHwpList, pPipe);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutPipe(ERR, pPipe, "DtVectorPipe_PushBack TxRtHwpList. Error:%xh", 
                                                                                  Status);
            return Status;
        }
        DT_ASSERT(i + 1 == DtVectorPipe_Size(pDf->m_pPipeTxRtHwpList));
   }
   return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_IsPacketForNetworkDriver -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtDfNw_IsPacketForNetworkDriver(DtDfNw* pDf, UInt8* pPacket)
{
    DtEthIp* pEthIp = (DtEthIp*)pPacket;
    EthernetIIHeader* pEthIIHeader = (EthernetIIHeader*)(pPacket + sizeof(DtEthIp));
    Bool Found = FALSE;
    UInt Items = 0;

    if (pEthIp->m_Hdr.m_PacketType == DT_ETHIP_PKTTYPE_TIMESTAMP)
        return TRUE;

    if (pDf->m_PacketFilter == DT_MAC_FLT_NOTSET)
    {
        DtDbgOutDf(MAX, NW, pDf, "Packet filter not set. Skip packets");
        return FALSE;
    }
    if ((pDf->m_PacketFilter & DT_MAC_FLT_PROMISCUOUS) != 0)
        return TRUE;
    
    if (!ETH_IS_MULTICAST(pEthIIHeader->m_DstMac))     // It's a unicast
    {
        if ((pDf->m_PacketFilter & DT_MAC_FLT_DIRECTED) == 0)
            return FALSE;
        return 
            pEthIIHeader->m_DstMac[0] == pDf->m_pBcEMAC10G->m_MacAddrCur[0] &&
            pEthIIHeader->m_DstMac[1] == pDf->m_pBcEMAC10G->m_MacAddrCur[1] &&
            pEthIIHeader->m_DstMac[2] == pDf->m_pBcEMAC10G->m_MacAddrCur[2] &&
            pEthIIHeader->m_DstMac[3] == pDf->m_pBcEMAC10G->m_MacAddrCur[3] &&
            pEthIIHeader->m_DstMac[4] == pDf->m_pBcEMAC10G->m_MacAddrCur[4] &&
            pEthIIHeader->m_DstMac[5] == pDf->m_pBcEMAC10G->m_MacAddrCur[5];
    }
    if (ETH_IS_BROADCAST(pEthIIHeader->m_DstMac))
        return (pDf->m_PacketFilter & DT_MAC_FLT_BROADCAST) != 0;

    // It's a multicast
    if ((pDf->m_PacketFilter & DT_MAC_FLT_ALL_MULTICAST) != 0)
        return TRUE;
    if ((pDf->m_PacketFilter & DT_MAC_FLT_MULTICAST) == 0)
        return FALSE;

    // Check multicast list
    DtFastMutexAcquire(&pDf->m_MulticastListFastMutex);
    for (Items = 0; Items < pDf->m_NumMulticasts && !Found; Items++)
    {
        if (pEthIIHeader->m_DstMac[0] == pDf->m_MulticastList[Items][0] &&
            pEthIIHeader->m_DstMac[1] == pDf->m_MulticastList[Items][1] &&
            pEthIIHeader->m_DstMac[2] == pDf->m_MulticastList[Items][2] &&
            pEthIIHeader->m_DstMac[3] == pDf->m_MulticastList[Items][3] &&
            pEthIIHeader->m_DstMac[4] == pDf->m_MulticastList[Items][4] &&
            pEthIIHeader->m_DstMac[5] == pDf->m_MulticastList[Items][5])
            Found = TRUE;
    }
    DtFastMutexRelease(&pDf->m_MulticastListFastMutex);
    if (!Found)
    {
        DtDbgOutDf(MAX, NW, pDf, "Multicast %02X:%02X:%02X:%02X:%02X:%02X not in list of %i items",
            pEthIIHeader->m_DstMac[0], pEthIIHeader->m_DstMac[1], pEthIIHeader->m_DstMac[2],
            pEthIIHeader->m_DstMac[3], pEthIIHeader->m_DstMac[4], pEthIIHeader->m_DstMac[5],
            pDf->m_NumMulticasts);
    }
    return Found;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_InitPipeRxRtHwp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfNw_InitPipeRxRtHwp(DtDfNw* pDf,
                                            DtBcCDMACTO* pBcCdma[MAX_NUMBER_RT_HW_PIPES], 
                                            DtBcIPROUT* pBcIpRout[MAX_NUMBER_RT_HW_PIPES])
{
    DtStatus Status = DT_STATUS_OK;
    Int i;
    Int NumHwPipes = 0;
    // Count the number of hardware pipes
    for (i = 0; i < MAX_NUMBER_RT_HW_PIPES; i++)
    {
        if (pBcCdma[i] != NULL && pBcIpRout[i] != NULL)
            NumHwPipes++;
        else if (!(pBcCdma[i] == NULL && pBcIpRout[i] == NULL))
        {
            DtDbgOutDf(ERR, NW, pDf, "ERROR: invalid xml config role: %s", ROLE_RX_HWP);
            return DT_STATUS_CONFIG_ERROR;
        }
    }
    pDf->m_pPipeRxRtHwpList = DtVectorPipe_Create(NumHwPipes, 0);

    for (i = 0; i < NumHwPipes; i++)
    {
        DtPipe* pPipe = DtPipe_Open(NULL, (DtObject*)pDf, DT_PIPE_RX_RT_HWP, 
                                                            pDf->m_NumStaticPipes + 1, i);
        DtPipeRxRtHwp* pHwp = (DtPipeRxRtHwp*)pPipe;
        if (pPipe == NULL)
        {
            DtDbgOutDf(ERR, NW, pDf, "ERROR: DtPipe_Open failed: DT_PIPE_RX_RT_HWP");
            return DT_STATUS_OUT_OF_MEMORY;
        }
        
        pHwp->m_pBcCdma = pBcCdma[i];
        pHwp->m_pIpRouter = pBcIpRout[i];

        Status = DtVectorPipe_PushBack(pDf->m_pPipeIdMap, pPipe);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutPipe(ERR, pPipe, "DtVectorPipe_PushBack. Error:%xh", Status);
            return Status;
        }
        pDf->m_NumStaticPipes++;
        DT_ASSERT(pPipe->m_Id == DtVectorPipe_Size(pDf->m_pPipeIdMap));
        
        Status = DtVectorPipe_PushBack(pDf->m_pPipeRxRtHwpList, pPipe);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutPipe(ERR, pPipe, "DtVectorPipe_PushBack. Error:%xh", Status);
            return Status;
        }
        DT_ASSERT(i + 1 == DtVectorPipe_Size(pDf->m_pPipeRxRtHwpList));
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_CleanupStaticPipes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfNw_CleanupPipes(DtDfNw* pDf)
{
    Int i;
    for (i = 0; i<pDf->m_NumStaticPipes; i++)
    {
        DtPipe* pPipe = DtVectorPipe_At(pDf->m_pPipeIdMap, i);
        DT_ASSERT(pPipe != NULL);
        if (pPipe != NULL)
            DtPipe_Close(pPipe);
    }
    for (i = pDf->m_NumStaticPipes; i < DtVectorPipe_Size(pDf->m_pPipeIdMap); i++)
    {
        DtPipe* pPipe = DtVectorPipe_At(pDf->m_pPipeIdMap, i);
        if (pPipe != NULL)
            DtPipe_Close(pPipe);
    }
    DtVectorPipe_Cleanup(pDf->m_PerIntEvtList);
    DtVectorPipe_Cleanup(pDf->m_pPipeIdMap);
    DtVectorPipe_Cleanup(pDf->m_pPipeRxRtSwpList);
    DtVectorPipe_Cleanup(pDf->m_pPipeTxRtSwpList);
    DtVectorPipe_Cleanup(pDf->m_pPipeRxRtHwpList);
    DtVectorPipe_Cleanup(pDf->m_pPipeTxRtHwpList);

}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfNw_Init(DtDf* pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfNw* pDf = (DtDfNw*)pDfBase;
    DtOnPeriodicIntervalRegData  RegData;
    
    DtDbgOutDf(MAX, NW, pDf, "Init");

    // Sanity checks
    DF_NW_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDfBase->m_pCore->m_RegisterChildDevice != NULL);
    DT_ASSERT(pDfBase->m_pCore->m_UnregisterChildDevice != NULL);

    // Set defaults
    pDf->m_OperationalMode = DT_FUNC_OPMODE_IDLE;

    // Create pipe map and swp lists
    pDf->m_pPipeIdMap = DtVectorPipe_Create(MAX_NUM_PIPES_EACH_PORT, 0);
    pDf->m_pPipeRxRtSwpList = DtVectorPipe_Create(10, 10);
    pDf->m_pPipeTxRtSwpList = DtVectorPipe_Create(10, 10);
    pDf->m_PerIntEvtList = DtVectorPipe_Create(10, 10);

    // Initialise list mutex and periodic int spinlock
    DtFastMutexInit(&pDf->m_PipeIdMapFMutex);
    DtFastMutexInit(&pDf->m_PipeRxRtHwpListFMutex);
    DtFastMutexInit(&pDf->m_PipeTxRtHwpListFMutex);
    DtFastMutexInit(&pDf->m_PipeRxRtSwpListFMutex);
    DtFastMutexInit(&pDf->m_PipeTxRtSwpListFMutex);
    DtFastMutexInit(&pDf->m_PipeRxNrtFMutex);
    DtFastMutexInit(&pDf->m_PipeTxNrtFMutex);
    DtFastMutexInit(&pDf->m_PerIntEvtFMutex);
    DtSpinLockInit(&pDf->m_PerItvSpinLock);
    DtSpinLockInit(&pDf->m_OpModeCBSpinlock);
    DtFastMutexInit(&pDf->m_MulticastListFastMutex);
    pDf->m_NumMulticasts = 0;

    pDf->m_PerItvEnable = FALSE;
    pDf->m_OpModeCB = NULL;

    //Open children
    Status = DtDfNw_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, NW, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }
    
    // Initialize the helper threads
    DT_RETURN_ON_ERROR(DtThreadInit(&pDf->m_RxHwqThread, DtDfNw_RxHwqThread, pDf));
    DtEventInit(&pDf->m_RxHwqStopEvent, FALSE);

    DT_RETURN_ON_ERROR(DtThreadInit(&pDf->m_TxRtHwqThread, DtDfNw_TxRtHwqThread, pDf));
    DtEventInit(&pDf->m_TxRtHwqStopEvent, FALSE);

    // Register periodic interval handler
    RegData.m_OnPeriodicFunc = DtDfNw_PeriodicIntervalHandler;
    RegData.m_pObject = (DtObject*)pDf;
    Status = DtCore_TOD_PeriodicItvRegister(pDf->m_pCore, &RegData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, NW, pDf, "ERROR: failed to register period itv handler");
        return Status;
    }

    // Register child device
    pDfBase->m_pCore->m_RegisterChildDevice(pDfBase->m_pCore, (DtObject*)pDf->m_pPt);
    DtDbgOutDf(MAX, NW, pDf, "Exit");

    return DT_STATUS_OK;
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Note: LoadParameters() is called before the Init(). The loaded parameters can be used
// in the Init().
//
DtStatus  DtDfNw_LoadParameters(DtDf*  pDfBase)
{
    /*DtStatus  Status = DT_STATUS_OK;
    DtDfNw* pDf = (DtDfNw*)pDfBase;

    // List of Template function parameters
    DtDfParameters  DFNw_PARS[] =
    {
        // Name,  Value Type,  Value*
        { "PHY_PORT", PROPERTY_VALUE_TYPE_INT, &(pDf->m_PhyPort) },
    };

    // Sanity checks
    DF_NW_DEFAULT_PRECONDITIONS(pDf);

    // Set defaults
    pDf->m_PhyPort = -1;
    
    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFNw_PARS), DFNw_PARS);
    if (!DT_SUCCESS(Status))
        return Status;
    */
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfNw_OnEnablePostChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfNw* pDf = (DtDfNw*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
     
    // Sanity check
    DF_NW_DEFAULT_PRECONDITIONS(pDf);

    if (pDf->m_ReducedFunctionality)
        return DT_STATUS_OK;

    if (Enable)
    {
        Int i;
        // DISABLE -> ENABLE
        DtDbgOutDf(AVG, NW, pDf, "DISABLE -> ENABLE");

        // Operational mode must be IDLE
        DT_ASSERT(pDf->m_OperationalMode == DT_FUNC_OPMODE_IDLE);

        // Set operational mode of childs to IDLE
       DT_RETURN_ON_ERROR(DtBcEMAC10G_SetOperationalMode(pDf->m_pBcEMAC10G,
                                                                   DT_BLOCK_OPMODE_IDLE));
        DT_RETURN_ON_ERROR(DtBcIPSUMCHK_SetOperationalMode(pDf->m_pBcIpSumChk, 
                                                                   DT_BLOCK_OPMODE_IDLE));
        DT_RETURN_ON_ERROR(DtBcIPSUMINS_SetOperationalMode(pDf->m_pBcIpSumIns, 
                                                                   DT_BLOCK_OPMODE_IDLE));
        //DT_RETURN_ON_ERROR(DtDfIpFifo_SetOperationalMode(pDf->m_pDfIpFifoRxHwq, 
        //                                                          DT_FUNC_OPMODE_IDLE));
        for (i = 0; i < pDf->m_NumStaticPipes; i++)
        {
            DtPipe* pPipe = DtVectorPipe_At(pDf->m_pPipeIdMap, i);
            DT_RETURN_ON_ERROR(DtPipe_SetOperationalMode(pPipe, DT_PIPE_OPMODE_IDLE));
        }

        // Start the helper threads
        DtEventReset(&pDf->m_RxHwqStopEvent);
        Status = DtThreadStart(&pDf->m_RxHwqThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, NW, pDf, "ERROR: failed to start Rx HWQ thread");
            return Status;
        }

        DtEventReset(&pDf->m_TxRtHwqStopEvent);
        Status = DtThreadStart(&pDf->m_TxRtHwqThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, NW, pDf, "ERROR: failed to start Tx RT HWQ thread");
            return Status;
        }

        // Enable periodic interval handler
        DtDfNw_EnablePeriodicIntervalHandler(pDf, TRUE);
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_OnEnablePreChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfNw_OnEnablePreChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfNw* pDf = (DtDfNw*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
    // Sanity check
    DF_NW_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        DtDfNw_CheckReducedFunctionalityMode(pDf);
        if (pDf->m_ReducedFunctionality)
        {
            DtString  Str;
            DtDbgOutDf(ERR, NW, pDf, "Hardware problem detected. Operating in reduced"
                " functionality mode.");
            DtStringAlloc(&Str, 256);
            DtStringAppendChars(&Str, "[SN=");
            DtStringUInt64ToDtStringAppend(&Str, 10, pDf->m_pCore->m_pDevInfo->m_Serial);
            DtStringAppendChars(&Str, "] ");
            DtStringAppendChars(&Str, "Hardware problem detected. Operating in reduced"
                " functionality mode.");
            DtEvtLogReport(&pDf->m_pCore->m_Device.m_EvtObject,
                DTPCIE_LOG_ERROR_GENERIC, &Str, NULL, NULL);
            DtStringFree(&Str);
        }
    }
    
    if (pDf->m_ReducedFunctionality)
        return DT_STATUS_OK;

    if (!Enable)
    {
        // ENABLE -> DISABLE
        DtDbgOutDf(AVG, NW, pDf, "ENABLE -> DISABLE");

        
        // Stop helper threads
        DtEventSet(&pDf->m_RxHwqStopEvent);
        Status = DtThreadStop(&pDf->m_RxHwqThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, NW, pDf, "ERROR: failed to stop RxHwqThread");
            //return Status;
        }

        DtEventSet(&pDf->m_TxRtHwqStopEvent);
        Status = DtThreadStop(&pDf->m_TxRtHwqThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, NW, pDf, "ERROR: failed to stop TxRtHwqThread");
            //return Status;
        }
        
        // Disable periodic interval handler
        DtDfNw_EnablePeriodicIntervalHandler(pDf, FALSE);

        // Return to IDLE
        DT_RETURN_ON_ERROR(DtDfNw_SetOperationalMode(pDf, DT_FUNC_OPMODE_IDLE));
    }

    return DT_STATUS_OK;
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtDfNw_OnCloseFile(DtDf* pDfBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfNw* pDf = (DtDfNw*)pDfBase;
    DtPipe* pPipe = NULL;
   
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    // Sanity checks
    DF_NW_DEFAULT_PRECONDITIONS(pDf);

    // Check if the owner closed the file handle
    Status = DtDf_ExclAccessCheck((DtDf*)pDf, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutDf(AVG, NW, pDf, "Go to IDLE");

        // Go to IDLE
        Status = DtDfNw_SetOperationalMode(pDf, DT_FUNC_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, NW, pDf, "ERROR: failed to set operational mode to IDLE");
        }
    }
    DtDbgOutDf(ERR, NW, pDf, "Check for closing pipe. FileObject: %p", 
                                                   DtFileGetHandle((DtFileObject*)pFile));

    // Clean up pipes still in use by the closing file handle
    do {
        pPipe = DtDfNw_FindPipe(pDf, pFile);
        if (pPipe != NULL)
        {
            Int Uuid = pDf->m_Id.m_Uuid | (pPipe->m_Id << 20);
            DtDbgOutPipe(ERR, pPipe, "Force pipe to close: Uuid: %xh", Uuid);
            DtDfNw_DeregisterEvent(pPipe, NULL);
            DtDfNw_PipeClose(pDf, pFile, pPipe->m_PipeType, Uuid);
            DtDfNw_CheckFilterDuplication(pDf);
        }
    } while (pPipe != NULL);

    // Use base function to release exclusive access
    return DtDf_OnCloseFile((DtDf*)pDf, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfNw_OpenChildren(DtDfNw* pDf)
{
    DtStatus  Status = DT_STATUS_OK;
    Int i;
    
    // Realtime hardware pipes
    DtBcCDMACTO* pBcCdmaTxRtHwp[MAX_NUMBER_RT_HW_PIPES];
    DtBcBURSTFIFO* pBcBurstFifoTxRtHwp[MAX_NUMBER_RT_HW_PIPES];
    DtBcCDMACTO* pBcCdmaRxRtHwp[MAX_NUMBER_RT_HW_PIPES];
    DtDfIpFifo* pDfIpFifoTxRtHwp[MAX_NUMBER_RT_HW_PIPES];
    DtBcIPROUT* pBcIpRoutRxRtHwp[MAX_NUMBER_RT_HW_PIPES];
    DtBcCDMACTO* pBcCdmaRxHwq = NULL;
    DtDfIpFifo* pDfIpFifoRxHwq = NULL;
    DtBcCDMACTO* pBcCdmaTxRtHwq = NULL;
    DtBcBURSTFIFO* pBcBurstFifoTxRtHwq = NULL;
    DtDfIpFifo* pDfIpFifoTxRtHwq = NULL;
    DtBcCDMACTO* pBcCdmaTxNrtHwq = NULL;
    DtBcETHIPPAD* pEthIpPadRxHwq = NULL;
    DtBcDDRFIFO* pDdrFifoRxHwq = NULL;
    
    // List of children supported by the DfNw function
    DtDfSupportedChild  SUPPORTED_CHILDREN[] =
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_ETHIPPAD, DT_BC_ETHIPPAD_NAME,
            BC_ROLE_NONE, (DtObjectBcOrDf**)(&pEthIpPadRxHwq), FALSE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_DDRFIFO, DT_BC_DDRFIFO_NAME,
            BC_ROLE_NONE, (DtObjectBcOrDf**)(&pDdrFifoRxHwq), FALSE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_CDMACTO, DT_BC_CDMACTO_NAME,
            ROLE_RX_HWQ, (DtObjectBcOrDf**)(&pBcCdmaRxHwq), TRUE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_CDMACTO, DT_BC_CDMACTO_NAME,
            ROLE_TX_NRT_HWQ, (DtObjectBcOrDf**)(&pBcCdmaTxNrtHwq), TRUE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_CDMACTO, DT_BC_CDMACTO_NAME,
            ROLE_TX_RT_HWQ, (DtObjectBcOrDf**)(&pBcCdmaTxRtHwq), TRUE },
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_BURSTFIFO, DT_BC_BURSTFIFO_NAME,
            ROLE_TX_RT_HWQ, (DtObjectBcOrDf**)(&pBcBurstFifoTxRtHwq), FALSE},
        { DT_OBJECT_TYPE_DF, DT_FUNC_TYPE_IPFIFO, DT_DF_IPFIFO_NAME,
            ROLE_TX_RT_HWQ, (DtObjectBcOrDf**)(&pDfIpFifoTxRtHwq), FALSE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_EMAC10G, DT_BC_EMAC10G_NAME,
            BC_EMAC10G_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcEMAC10G), TRUE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_IPSUMCHK, DT_BC_IPSUMCHK_NAME,
            BC_IP_SUMCHK_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcIpSumChk), TRUE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_IPSUMINS, DT_BC_IPSUMINS_NAME,
            BC_IP_SUMINS_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcIpSumIns), TRUE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_IPSCHED, DT_BC_IPSCHED_NAME,
            BC_IP_SCHED_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pIpSched), TRUE},
        { DT_OBJECT_TYPE_DF, DT_FUNC_TYPE_IPFIFO, DT_DF_IPFIFO_NAME,
            ROLE_RX_HWQ, (DtObjectBcOrDf**)(&pDfIpFifoRxHwq), FALSE},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_CDMACTO, DT_BC_CDMACTO_NAME,
            ROLE_RX_HWP, (DtObjectBcOrDf**)(&pBcCdmaRxRtHwp[0]), TRUE,
            MAX_NUMBER_RT_HW_PIPES},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_CDMACTO, DT_BC_CDMACTO_NAME,
            ROLE_TX_HWP, (DtObjectBcOrDf**)(&pBcCdmaTxRtHwp[0]), TRUE,
            MAX_NUMBER_RT_HW_PIPES},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_BURSTFIFO, DT_BC_BURSTFIFO_NAME,
            ROLE_TX_HWP, (DtObjectBcOrDf**)(&pBcBurstFifoTxRtHwp[0]), FALSE,
            MAX_NUMBER_RT_HW_PIPES},
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_IPROUT, DT_BC_IPROUT_NAME,
            ROLE_RX_HWP, (DtObjectBcOrDf**)(&pBcIpRoutRxRtHwp[0]), TRUE,
            MAX_NUMBER_RT_HW_PIPES},
        { DT_OBJECT_TYPE_DF, DT_FUNC_TYPE_IPFIFO, DT_DF_IPFIFO_NAME,
            ROLE_TX_HWP, (DtObjectBcOrDf**)(&pDfIpFifoTxRtHwp[0]), FALSE,
            MAX_NUMBER_RT_HW_PIPES},
    };

    DF_NW_DEFAULT_PRECONDITIONS(pDf);

    pDf->m_NumStaticPipes = 0;

    for (i=0; i<MAX_NUMBER_RT_HW_PIPES; i++)
    {
        pBcCdmaTxRtHwp[i] = NULL; 
        pBcBurstFifoTxRtHwp[i] = NULL;
        pBcCdmaRxRtHwp[i] = NULL;
        pDfIpFifoTxRtHwp[i] = NULL; 
        pBcIpRoutRxRtHwp[i] = NULL;
    }

    // Use base function to get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    Status = DtDfNw_InitPipeRxNrt(pDf);
    if (Status != DT_STATUS_OK)
        return Status;

    Status = DtDfNw_InitPipeTxNrt(pDf, pBcCdmaTxNrtHwq);
    if (Status != DT_STATUS_OK)
        return Status;

    // TODO: Make EthIpPad and DdrfifoRxHwq 1 DF?? 
    Status = DtDfNw_InitPipeRxHwq(pDf, pBcCdmaRxHwq, pDfIpFifoRxHwq, pEthIpPadRxHwq, 
                                                                           pDdrFifoRxHwq);
    if (Status != DT_STATUS_OK)
        return Status;
    
    Status = DtDfNw_InitPipeTxRtHwq(pDf, pDf->m_pIpSched, pBcCdmaTxRtHwq,
                                                   pBcBurstFifoTxRtHwq, pDfIpFifoTxRtHwq);
    if (Status != DT_STATUS_OK)
        return Status;

    Status = DtDfNw_InitPipeTxRtHwp(pDf, pDf->m_pIpSched, pBcCdmaTxRtHwp, 
                                                   pBcBurstFifoTxRtHwp, pDfIpFifoTxRtHwp);
    if (Status != DT_STATUS_OK)
        return Status;
    Status = DtDfNw_InitPipeRxRtHwp(pDf, pBcCdmaRxRtHwp, pBcIpRoutRxRtHwp);
    if (Status != DT_STATUS_OK)
        return Status;

    return DT_STATUS_OK;
}

#define TOD_TO_NS(TOD) (TOD->m_TodSeconds * 1000000000 + TOD->m_TodNanoseconds)

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_IsTimeEarlier -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool DtDfNw_IsTimeEarlier(DtPipeTxRtSwp* pPipe, DtEthIpTod* pTodChk, DtEthIpTod* pTod)
{
    UInt64 NsPkt = TOD_TO_NS(pTodChk);
    UInt64 NsEnd = TOD_TO_NS(pTod);
    
    if (NsEnd == NsPkt)
        return FALSE;

    // Check if time is out of range. If so, we set the error flag and pipe will be 
    // blocked until a flush occurs
    if (NsEnd > NsPkt)
    {
        if (NsEnd - NsPkt > ((UInt64)MAX_DELAY_TX_RT_HWP_MS*1000*1000))
        {
            if (pPipe != NULL)
                pPipe->m_ErrorFlag |= DT_PIPE_ERROR_INVALID_TIME;
            return FALSE;
        }
        return TRUE;
    }
    else if (NsPkt - NsEnd > ((UInt64)MAX_DELAY_TX_RT_HWP_MS*1000*1000))
    {
        if (pPipe != NULL)
            pPipe->m_ErrorFlag |= DT_PIPE_ERROR_INVALID_TIME;
        return FALSE;
    }
    return FALSE;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_GetFirstTxTodPacketPipe -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtEthIpTod* DtDfNw_GetFirstTxTodPacketPipe(DtPipeTxRtSwp* pPipe, DtEthIpTod* pEndTod)
{
    DtEthIpTod* pTod;
    DtEthIp* pDtEthIp;
    // Check empty buffer
    if (pPipe->m_ReadOffset == pPipe->m_WriteOffset)
        return NULL;
    
    pDtEthIp = (DtEthIp*)(pPipe->m_pBuffer + pPipe->m_ReadOffset);
    DT_ASSERT(pDtEthIp->m_Hdr.m_SyncWord == DT_ETHIP_SYNCWORD);
    if (pPipe->m_ReadOffset + sizeof(DtEthIpHeader) < pPipe->m_BufSize)
        pTod = &pDtEthIp->m_Tod;
    else
        pTod = (DtEthIpTod*)pPipe->m_pBuffer;
    if (DtDfNw_IsTimeEarlier(pPipe, pTod, pEndTod))
        return pTod;
    return NULL;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_TxRtHwqThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtDfNw_TxRtHwqThread(DtThread* pThread, void* pContext)
{
    DtDfNw* pDf = (DtDfNw*)pContext;
    Bool StopThread = FALSE;
    const Int LoopTimeOut = 1000;
    DtEvent PerIntEvent;
    DtPipe* pPipe;
    DtStatus Status = DT_STATUS_OK;
    UInt Uuid = 0;
    UInt8* pBuffer;
    Int BufSize = 0;
    DtVectorPipe* pPipeListTime = NULL;
    
    pPipeListTime = DtVectorPipe_Create(10, 0);
    if (pPipeListTime == NULL)
    {
        DtDbgOutDf(ERR, NW, pDf, "ERROR: DtVectorPipe_Create out of memory");
        return;
    }

    DtEventInit(&PerIntEvent, TRUE);

    Status = DtDfNw_PipeOpen(pDf, NULL, DT_PIPE_TX_RT_HWQ, &Uuid, &pPipe);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, NW, pDf, "ERROR: DtDfNw_PipeOpen. Status: %08xh", Status);
        return;
    }

    // Issue flush
    DtPipe_IssuePipeFlush(pPipe);

    // Create kernel buffer for this pipe
    Status = DtPipe_SetSharedBufferKernel(pPipe, TX_RT_HWQ_BUFSIZE, &BufSize, NULL, 
                                                                                &pBuffer);
    if (!DT_SUCCESS(Status))
        DtDbgOutPipe(ERR, pPipe, "ERROR: DtPipe_SetSharedBufferKernel. Status: %08xh",
            Status);

    // Register periodic interrupt event
    if (DT_SUCCESS(Status))
        Status = DtDfNw_RegisterKEvent(pPipe, &PerIntEvent, DT_PIPE_EVENT_ID_PERINT);
    if (!DT_SUCCESS(Status))
        StopThread = TRUE;

    while (!StopThread)
    {
        Bool EndTimeLoop = FALSE;
        Bool FirstLoop = TRUE;
        DtTodTime Time;
        DtEthIpTod EndTod;
        DtPipeTxRtSwp* pCurPipe = NULL;
        DtEthIpTod* pCurTod = NULL;

        // Wait for periodic interrupt. The LoopTimeout should never be triggered.
        Status = DtEventWaitUnInt(&PerIntEvent, LoopTimeOut);
        if (pThread->m_StopThread)
        {
            DtDbgOutDf(AVG, NW, pDf, "Stop request received");
            StopThread = TRUE;
            continue;
        }
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, NW, pDf, "ERROR wait event: %xh", Status);
            continue;
        }

        // Get current time
        DtCore_TOD_GetTime(pDf->m_pCore, &Time);

        EndTod.m_TodNanoseconds = Time.m_Nanoseconds + PerIntInNs+ (PerIntInNs/2);
        EndTod.m_TodSeconds = Time.m_Seconds;
        // Check whole seconds
        if ((EndTod.m_TodNanoseconds / NsInSec) != 0)
        {
            EndTod.m_TodSeconds++;
            EndTod.m_TodNanoseconds = EndTod.m_TodNanoseconds - NsInSec;
        }

        DtFastMutexAcquire(&pDf->m_PipeTxRtSwpListFMutex);

        while (!EndTimeLoop && !DtThreadShouldStop(pThread))
        {
            DtVectorPipe* pPipeList = NULL;
            Int i;
            Int NumEmptyEntries = 0;
            // Create a list of pipes that have a timestamp in range for this period.
            // For the first loop, we take all pipes and iterate over that list.
            // For the second time, we take only the pipes that had valid timestamps 
            // in range the previous iteration. We stop when all pipes in the list
            // have no timestamps in range for this period
            if (FirstLoop)
            {
                pPipeList = pDf->m_pPipeTxRtSwpList;
                // Reset the list of pipes who have packets to transmit
                DtVectorPipe_Clear(pPipeListTime);
            }
            else
                pPipeList = pPipeListTime;

            for (i= 0; i<DtVectorPipe_Size(pPipeList); i++)
            {
                DtPipeTxRtSwp* pSwp = (DtPipeTxRtSwp*)DtVectorPipe_At(pPipeList, i);
                DtEthIpTod* pTod;
                if (pSwp==NULL || pSwp->m_OpMode!=DT_PIPE_OPMODE_RUN || 
                                     (pSwp->m_ErrorFlag& DT_PIPE_ERROR_INVALID_TIME) != 0)
                {
                    if (pSwp != NULL)
                    {
                        // If pipe is set to idle, clear from playout list
                        if (!FirstLoop)
                            DtVectorPipe_SetEmpty(pPipeListTime, i);
                    }
                    NumEmptyEntries++;
                    continue;
                }
                pTod = DtDfNw_GetFirstTxTodPacketPipe(pSwp, &EndTod);
                if (pTod == NULL)
                {
                    // Remove pipe from pipelist
                    if (!FirstLoop)
                        DtVectorPipe_SetEmpty(pPipeListTime, i);
                    NumEmptyEntries++;
                    continue;
                }
                else if (FirstLoop)
                {
                    // Add pipe to time list
                    DtVectorPipe_PushBack(pPipeListTime, (DtPipe*)pSwp);
                }
                // Check packet time
                if (pCurPipe == NULL)
                {
                    pCurTod = pTod;
                    pCurPipe = pSwp;
                }
                else
                {
                    if (DtDfNw_IsTimeEarlier(NULL, pTod, pCurTod))
                    {
                        pCurTod = pTod;
                        pCurPipe = pSwp;
                    }
                }
            }

            // Copy packet to realtime hardware queue
            if (pCurPipe != NULL)
            {
                DtEthIpHeader* pEthIp = (DtEthIpHeader*)(pCurPipe->m_pBuffer + 
                                                                  pCurPipe->m_ReadOffset);
                Status = DtPipe_CopyData((DtPipe*)pCurPipe, pCurPipe->m_ReadOffset, pPipe,
                                                        (UInt)pEthIp->m_SizeInQWords * 8);
                if (DT_SUCCESS(Status))
                {
                    // Packet copied to queue
                    pCurPipe->m_ReadOffset = (pCurPipe->m_ReadOffset + 
                                        pEthIp->m_SizeInQWords * 8) % pCurPipe->m_BufSize;
                    pCurPipe = NULL;
                }
                else if (Status == DT_STATUS_BUFFER_OVERFLOW)
                {
                    // End the loop by setting the number of empty entries to the list 
                    // size
                    NumEmptyEntries = DtVectorPipe_Size(pPipeList);
                    DtDbgOutPipe(ERR, pPipe, "Buffer OVERFLOW!");
                }
                else
                {
                    // Skip this pipe. Something wrong with settings
                    // Remove pipe from playout list
                    if (!FirstLoop)
                        DtVectorPipe_SetEmpty(pPipeListTime, i);
                    NumEmptyEntries++;
                    DtDbgOutPipe(ERR, pPipe, "[%s:Id%i] ERROR: DtPipe_CopyData. Skip this"
                                                       " packet and pipe. Status: %08xh",
                                                       PipeType2Str(pCurPipe->m_PipeType),
                                                       pCurPipe->m_Id, Status);
                    pCurPipe = NULL;
                }
            }
            FirstLoop = FALSE;
            if (NumEmptyEntries == DtVectorPipe_Size(pPipeList))
                EndTimeLoop = TRUE;
        }
        DtFastMutexRelease(&pDf->m_PipeTxRtSwpListFMutex);
    }
    DtPipe_SetOperationalMode(pPipe, DT_PIPE_OPMODE_IDLE);
    DtDfNw_DeregisterKEvent(pPipe, &PerIntEvent, DT_PIPE_EVENT_ID_PERINT);
    DtPipe_ReleaseSharedBuffer(pPipe);
    DtDfNw_PipeClose(pDf, (const DtFileObject*)NULL, DT_PIPE_TX_RT_HWQ, Uuid);
    DtVectorPipe_Cleanup(pPipeListTime);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_RxHwqThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfNw_RxHwqThread(DtThread* pThread, void* pContext)
{
    DtDfNw* pDf = (DtDfNw*)pContext;
    Bool  StopThread = FALSE;
    const Int  LoopTimeOut = 1000;
    DtEvent PerIntEvent;
    DtPipe* pPipe;
    DtStatus Status = DT_STATUS_OK;
    UInt Uuid = 0;
    UInt ReadOffset = 0;
    UInt8* pBuffer;
    Int BufSize = 0;
    UInt IntCnt = 0;
    UInt8 TmpBuffer[250];

    DtEventInit(&PerIntEvent, TRUE);

    Status = DtDfNw_PipeOpen(pDf, NULL, DT_PIPE_RX_HWQ, &Uuid, &pPipe);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, NW, pDf, "ERROR: DtDfNw_PipeOpen. Status: %08xh", Status);
        return;
    }

    // Issue flush
    DtPipe_IssuePipeFlush(pPipe);

    // Create kernel buffer for this pipe
    Status = DtPipe_SetSharedBufferKernel(pPipe, RX_HWQ_BUFSIZE, &BufSize, NULL, &pBuffer);
    if (!DT_SUCCESS(Status))
        DtDbgOutPipe(ERR, pPipe, "ERROR: DtPipe_SetSharedBufferKernel. Status: %08xh",
                                                                                  Status);
    
    // Register periodic interrupt event
    if (DT_SUCCESS(Status))
        Status = DtDfNw_RegisterKEvent(pPipe, &PerIntEvent, DT_PIPE_EVENT_ID_PERINT);
    if (!DT_SUCCESS(Status))
        StopThread = TRUE;

    // Initialise ReadOffset
    Status = DtPipe_GetRxWriteOffset(pPipe, &ReadOffset);
    DT_ASSERT(DT_SUCCESS(Status));
    
    while (!StopThread)
    {
        UInt WriteOffset = 0;
        Int Retry = 0;
        
        // Wait for periodic interrupt. The LoopTimeout should never be triggered.
        Status = DtEventWaitUnInt(&PerIntEvent, LoopTimeOut);
        if (pThread->m_StopThread)
        {
            DtDbgOutDf(AVG, NW, pDf, "Stop request received");
            StopThread = TRUE;
            continue;
        }
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, NW, pDf, "ERROR wait event: %xh", Status);
            continue;
        }
        IntCnt++;

        Status = DtPipe_GetRxWriteOffset(pPipe, &WriteOffset);
        DT_ASSERT(DT_SUCCESS(Status));
        
        while (ReadOffset != WriteOffset && !DtThreadShouldStop(pThread))
        {
            DtEthIpHeader* pEthIpHeader;
            
            Int BufLoad = 0;
            Int NumBytesToEnd = 0;
            Bool PacketForRtPipe = FALSE;
            UInt8* pPacket = pBuffer + ReadOffset;
            Int PacketSize;
            Int Flags = 0;
            BufLoad = (Int)((WriteOffset + BufSize - ReadOffset) % BufSize);

            // Datapath is minimal 64-bit aligned, so header must fit until end
            pEthIpHeader = (DtEthIpHeader*)pPacket;
            PacketSize = (Int)pEthIpHeader->m_SizeInQWords * 8;

            if (PacketSize > BufLoad)
            {
                if (pEthIpHeader->m_SyncWord == DT_ETHIP_SYNCWORD)
                {
                    DtDbgOutDf(AVG, NW, pDf, "Not a complete packet received with DMA. %i"
                         " bytes received. Needs %i bytes. Wait for complete packet (%i)."
                         " ReadOffset:%i WriteOffset:%i",
                         BufLoad, PacketSize, Retry + 1, ReadOffset, WriteOffset);
                    DtSleep(1);
                    Status = DtPipe_GetRxWriteOffset(pPipe, &WriteOffset);
                    DT_ASSERT(DT_SUCCESS(Status));
                    Retry++;
                    if (Retry < 10)
                        continue;
                }
                else
                    DtDbgOutDf(ERR, NW, pDf, "Not a complete packet received with DMA. %i"
                                    " bytes received with HEADER SYNC ERROR!!!", BufLoad);
            }
            Retry = 0;
            NumBytesToEnd = BufSize - ReadOffset;

            // Sanity check
            DT_ASSERT((pEthIpHeader->m_SyncWord == DT_ETHIP_SYNCWORD
                          && NumBytesToEnd>=sizeof(DtEthIpHeader) && PacketSize<=BufLoad 
                          && PacketSize-sizeof(DtEthIp) <= DT_ETH_IP_MAX_PACKETSIZE
                          && PacketSize>=pEthIpHeader->m_SizeInBytes+sizeof(DtEthIp))
                          || pPipe->m_OpMode==DT_PIPE_OPMODE_IDLE);
            if (pEthIpHeader->m_SyncWord!=DT_ETHIP_SYNCWORD
                          || NumBytesToEnd<sizeof(DtEthIpHeader) || PacketSize>BufLoad
                          || pEthIpHeader->m_SizeInQWords<2 
                          || PacketSize-sizeof(DtEthIp)>DT_ETH_IP_MAX_PACKETSIZE
                          || PacketSize<pEthIpHeader->m_SizeInBytes+sizeof(DtEthIp))
            {
                // Fatal error: clear RX buffer and log error.
                DtDbgOutDf(ERR, NW, pDf, "Packet error RX. Sync word: %xh SizeInQWords:%i"
                                               "ReadOffset:%i WriteOffset:%i BufSize:%i"
                                               "BufLoad:%i SizeInBytes:%i",
                                               (UInt32)pEthIpHeader->m_SyncWord, 
                                               (UInt32)pEthIpHeader->m_SizeInQWords,
                                               ReadOffset, WriteOffset, BufSize, BufLoad,
                                               (UInt32)pEthIpHeader->m_SizeInBytes);
                ReadOffset = WriteOffset;
                DtPipe_SetRxReadOffset(pPipe, ReadOffset);
                continue;
            }
            // Check if packet is an IP packet. If not, it's not for a realtime queue
            if (pEthIpHeader->m_PacketType == DT_ETHIP_PKTTYPE_IPV4 || 
                                      pEthIpHeader->m_PacketType == DT_ETHIP_PKTTYPE_IPV6)
            {
                UInt8* pIpAddress;
                UdpHeader* pUdpHeader;
                UInt16 DstPort;
                UInt16 SrcPort;
                UInt8* pSrcIp;
                UInt8* pDstIp;
                Int VlanId0 = 0;
                Int VlanId1 = 0;
                EthernetIIHeader* pEthIIHdr;
                EthernetIIHeaderVlan* pEthIIHdrVlan;
                Int i;

                Int HeaderSize = (Int)(pEthIpHeader->m_PortOffset*4) + sizeof(UdpHeader);
                if (HeaderSize > NumBytesToEnd)
                {
                    memcpy(TmpBuffer, pPacket, NumBytesToEnd);
                    memcpy(TmpBuffer + NumBytesToEnd, pBuffer, HeaderSize-NumBytesToEnd);
                    pPacket = TmpBuffer;
                }
                pEthIIHdr = (EthernetIIHeader*)(pPacket + sizeof(DtEthIp));
                pEthIIHdrVlan = (EthernetIIHeaderVlan*)pEthIIHdr;

                pIpAddress = pPacket + (pEthIpHeader->m_IpAddressOffset * 4);
                pUdpHeader = (UdpHeader*)(pPacket + (pEthIpHeader->m_PortOffset * 4));
                
                DstPort = NWCW(pUdpHeader->m_DestinationPort);
                SrcPort = NWCW(pUdpHeader->m_SourcePort);

                // Check Ipv4/Ipv6
                pSrcIp = pIpAddress;
                if (pEthIpHeader->m_PacketType == DT_ETHIP_PKTTYPE_IPV4)
                {
                    Flags |= (DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV4 | 
                                                        DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV4);
                    pDstIp = pIpAddress + 4;
                }
                else
                {
                    Flags |= (DT_PIPE_IPFLT_FLAG_EN_SRCIP_IPV6 | 
                                                        DT_PIPE_IPFLT_FLAG_EN_DSTIP_IPV6);
                    pDstIp = pIpAddress + 16;
                }

                // Check VLAN
                if (pEthIIHdr->m_Type == ETH_TYPE_VLAN_BE)
                {
                    VlanId0 = pEthIIHdrVlan->m_VId;
                    Flags |= DT_PIPE_IPFLT_FLAG_VLAN0_1AD;
                    if (pEthIIHdrVlan->m_Type == ETH_TYPE_VLAN_BE || 
                                               pEthIIHdrVlan->m_Type == ETH_TYPE_VLANS_BE)
                    {
                        VlanTag* pVlanTag = (VlanTag*)&pEthIIHdrVlan->m_Type;
                        VlanId1 = pVlanTag->m_VId;
                        Flags |= DT_PIPE_IPFLT_FLAG_VLAN1_1AD;
                    }
                }

                DtFastMutexAcquire(&pDf->m_PipeRxRtSwpListFMutex);
                // Loop through all realtime software pipes and copy received packet to
                // the pipe listening for this IP stream
                for (i = 0; i < DtVectorPipe_Size(pDf->m_pPipeRxRtSwpList); i++)
                {
                    DtPipeSwp* pSwp = (DtPipeSwp*)DtVectorPipe_At(pDf->m_pPipeRxRtSwpList, 
                                                                                       i);
                    if (DtPipe_IsPacketForPipe((DtPipeRxRtSwp*)pSwp, pDstIp, DstPort,
                                                pSrcIp, SrcPort, VlanId0, VlanId1, Flags))
                    {
                        // Copy packet to software receive pipe
                        Status = DtPipe_CopyData(pPipe, ReadOffset, (DtPipe*)pSwp, 
                                                                              PacketSize);
                        if (Status == DT_STATUS_BUFFER_OVERFLOW)
                            DtDbgOutPipe(ERR, pSwp, "Shared buffer overflow."
                                                                " Skip received packet.");

                        // Check if we need to inform the receiver by triggering an event
                        if (DtPipe_IsDataAvailCntEvent((DtPipe*)pSwp))
                        {
                            pSwp->m_TrigEvtCnt++;
                            if (pSwp->m_TrigEvtCnt >= pSwp->m_TrigEvtMaxCnt)
                            {
                                DtPipe_TriggerEvent((DtPipe*)pSwp);
                                pSwp->m_TrigEvtCnt = 0;
                            }
                        }
                        if (DtPipe_IsDataAvailThrEvent((DtPipe*)pSwp) && 
                                                             pSwp->m_TrigEvtCnt != IntCnt)
                        {
                            pSwp->m_TrigEvtCnt = IntCnt;
                            DtPipe_TriggerEvent((DtPipe*)pSwp);
                        }
                        PacketForRtPipe = TRUE;
                    }
                }
                DtFastMutexRelease(&pDf->m_PipeRxRtSwpListFMutex);
            }

            if (!PacketForRtPipe && DtDfNw_IsPacketForNetworkDriver(pDf, pPacket))
            {
                DtPipe* pNrt;
                
                DtFastMutexAcquire(&pDf->m_PipeRxNrtFMutex);
                pNrt = DtVectorPipe_At(pDf->m_pPipeIdMap, pDf->m_IdMapRxNrt-1);

                // Copy to NRT receive pipe
                Status = DtPipe_CopyData(pPipe, ReadOffset, pNrt, PacketSize);
                if (Status== DT_STATUS_BUFFER_OVERFLOW)
                    DtDbgOutPipe(ERR, pNrt, "Shared buffer overflow."
                                                                " Skip received packet.");
                if (DtPipe_IsDataAvailCntEvent(pNrt))
                {
                    // The network driver always wants 1 event each packet, so we don't 
                    // check the counters and trigger always
                    DT_ASSERT(((DtPipeSwp*)pNrt)->m_TrigEvtMaxCnt == 1);
                    DtPipe_TriggerEvent(pNrt);
                }
                DtFastMutexRelease(&pDf->m_PipeRxNrtFMutex);
            }

            ReadOffset = (ReadOffset + (pEthIpHeader->m_SizeInQWords * 8)) % BufSize;
            DtPipe_SetRxReadOffset(pPipe, ReadOffset);
        }
    }
    DtPipe_SetOperationalMode(pPipe, DT_PIPE_OPMODE_IDLE);
    DtDfNw_DeregisterKEvent(pPipe, &PerIntEvent, DT_PIPE_EVENT_ID_PERINT);
    DtPipe_ReleaseSharedBuffer(pPipe);
    DtDfNw_PipeClose(pDf, (const DtFileObject*)NULL, DT_PIPE_RX_HWQ, Uuid);
}


// .-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_EnablePeriodicIntervalHandler -.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtDfNw_EnablePeriodicIntervalHandler(DtDfNw* pDf, Bool Enable)
{
    // Enable/disable handling periodic interval
    // A spinlock is used to prevent that handling is disabled while the
    // periodic interval handler is running
    DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
    pDf->m_PerItvEnable = Enable;
    DtSpinLockRelease(&pDf->m_PerItvSpinLock);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_PeriodicIntervalHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfNw_PeriodicIntervalHandler(DtObject* pObj, DtTodTime  Time)
{
    DtDfNw*  pDf = (DtDfNw*)pObj;
    DF_NW_DEFAULT_PRECONDITIONS(pDf);

    DtSpinLockAcquireAtDpc(&pDf->m_PerItvSpinLock);
    // Only when enabled
    if (pDf->m_PerItvEnable)
    {
        Int i;
        for (i = 0; i < DtVectorPipe_Size(pDf->m_PerIntEvtList); i++)
        {
            DtPipe* pPipe = DtVectorPipe_At(pDf->m_PerIntEvtList, i);
            DtPipe_TriggerEvent(pPipe);
        }
    }
    DtSpinLockReleaseFromDpc(&pDf->m_PerItvSpinLock);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_PipeOpen -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfNw_PipeOpen(DtDfNw* pDf, const DtFileObject* pFileObject, Int PipeType, 
                                                             UInt* pUuid, DtPipe** ppPipe)
{
    DtStatus Status = DT_STATUS_OK;
    DtPipe* pPipe = NULL;
    UInt Uuid = 0;

    DtFastMutexAcquire(&pDf->m_PipeIdMapFMutex);
    switch (PipeType)
    {
    case DT_PIPE_RX_NRT:
        DtFastMutexAcquire(&pDf->m_PipeRxNrtFMutex);
        pPipe = DtVectorPipe_At(pDf->m_pPipeIdMap, pDf->m_IdMapRxNrt-1);
        if (pPipe->m_InUse)
            Status = DT_STATUS_IN_USE;
        else
        {
            Uuid = pDf->m_Id.m_Uuid | (pDf->m_IdMapRxNrt << 20);
            pPipe->m_InUse = TRUE;
        }
        DtFastMutexRelease(&pDf->m_PipeRxNrtFMutex);
        break;
    case DT_PIPE_TX_NRT:
        DtFastMutexAcquire(&pDf->m_PipeTxNrtFMutex);

        pPipe = DtVectorPipe_At(pDf->m_pPipeIdMap, pDf->m_IdMapTxNrt-1);
        if (pPipe->m_InUse)
            Status = DT_STATUS_IN_USE;
        else
        {
            Uuid = pDf->m_Id.m_Uuid | (pDf->m_IdMapTxNrt << 20);
            pPipe->m_InUse = TRUE;
        }
        DtFastMutexRelease(&pDf->m_PipeTxNrtFMutex);
        break;
    case DT_PIPE_RX_HWQ:
        pPipe = DtVectorPipe_At(pDf->m_pPipeIdMap, pDf->m_IdMapRxHwq-1);
        if (pPipe->m_InUse)
            Status = DT_STATUS_IN_USE;
        else
        {
            *pUuid = pDf->m_Id.m_Uuid | (pDf->m_IdMapRxHwq << 20);
            pPipe->m_InUse = TRUE;
        }
        break;
    case DT_PIPE_TX_RT_HWQ:
        pPipe = DtVectorPipe_At(pDf->m_pPipeIdMap, pDf->m_IdMapTxRtHwq-1);
        if (pPipe->m_InUse)
            Status = DT_STATUS_IN_USE;
        else
        {
            *pUuid = pDf->m_Id.m_Uuid | (pDf->m_IdMapTxRtHwq << 20);
            pPipe->m_InUse = TRUE;
        }
        break;
    case DT_PIPE_RX_RT_HWP:
    case DT_PIPE_TX_RT_HWP:
    {
        DtVectorPipe* pVectorPipe = pDf->m_pPipeRxRtHwpList;
        DtFastMutex* pFastMutex = &pDf->m_PipeRxRtHwpListFMutex;
        Int Id = -1;
        Int i = 0;
        if (PipeType == DT_PIPE_TX_RT_HWP)
        {
            pVectorPipe = pDf->m_pPipeTxRtHwpList;
            pFastMutex = &pDf->m_PipeTxRtHwpListFMutex;
        }
        // Search for a HW port not in use
        DtFastMutexAcquire(pFastMutex);
        for (i = 0; i < DtVectorPipe_Size(pVectorPipe); i++)
        {
            pPipe = DtVectorPipe_At(pVectorPipe, i);
            if (!pPipe->m_InUse)
            {
                Id = pPipe->m_Id;
                pPipe->m_FileObject = *pFileObject;
                pPipe->m_InUse = TRUE;
                break;
            }
        }
        if (Id == -1)
            Status = DT_STATUS_IN_USE;
        else
            Uuid = pDf->m_Id.m_Uuid | (Id << 20);
        DtFastMutexRelease(pFastMutex);
        break;
    }
    case DT_PIPE_RX_RT_SWP:
    case DT_PIPE_TX_RT_SWP:
    {
        Int Id = DtVectorPipe_FindEmpty(pDf->m_pPipeIdMap, MAX_NUM_PIPES_EACH_PORT);
        if (Id == -1)
            Status = DT_STATUS_OUT_OF_RESOURCES;
        else
        {
            Id++;
            Uuid = pDf->m_Id.m_Uuid | (Id << 20);
            pPipe = DtPipe_Open(pFileObject, (DtObject*)pDf, PipeType, Id, -1);
            if (pPipe == NULL)
                Status = DT_STATUS_OUT_OF_MEMORY;
            else
            {
                DtVectorPipe* pVectorPipe = pDf->m_pPipeRxRtSwpList;
                DtFastMutex* pFastMutex = &pDf->m_PipeRxRtSwpListFMutex;
                pPipe->m_FileObject = *pFileObject;
                pPipe->m_InUse = TRUE;

                // Add pipe to the IdMap list
                DtVectorPipe_Set(pDf->m_pPipeIdMap, Id - 1, pPipe);

                // Add pipe to the type specific list
                if (PipeType == DT_PIPE_TX_RT_SWP)
                {
                    pVectorPipe = pDf->m_pPipeTxRtSwpList;
                    pFastMutex = &pDf->m_PipeTxRtSwpListFMutex;
                }
                DtFastMutexAcquire(pFastMutex);
                Status = DtVectorPipe_PushBack(pVectorPipe, pPipe);
                DtFastMutexRelease(pFastMutex);
            }
        }
        break;
    }
    default:
        Status = DT_STATUS_NOT_SUPPORTED;
    }
    
    if (DT_SUCCESS(Status))
    {
        if (ppPipe != NULL)
            *ppPipe = pPipe;
        if (pUuid != NULL)
            *pUuid = Uuid;
        DtDbgOutPipe(AVG, pPipe, "Opened pipe. Uuid: %xh File: %p", Uuid, 
                                                   DtFileGetHandle(&pPipe->m_FileObject));
    }
    DtFastMutexRelease(&pDf->m_PipeIdMapFMutex);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_FindPipe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtPipe* DtDfNw_FindPipe(DtDfNw* pDf, const DtFileObject* pFileObject)
{
    DtPipe* pPipe = NULL;
    int i;
    if (pFileObject == NULL)
        return NULL;

    DtFastMutexAcquire(&pDf->m_PipeIdMapFMutex);
    for (i = 0; i < DtVectorPipe_Size(pDf->m_pPipeIdMap); i++)
    {
        pPipe = DtVectorPipe_At(pDf->m_pPipeIdMap, i);
        if (pPipe == NULL)
            continue;
        
        DtDbgOutPipe(AVG, pPipe, "Index:%i InUse: %i. pFile:%p", i, pPipe->m_InUse ?1:0,
                                                   DtFileGetHandle(&pPipe->m_FileObject));
        if (pPipe->m_InUse && DtFileCompare(pFileObject, &pPipe->m_FileObject))
            break;
        pPipe = NULL;
    }
    DtFastMutexRelease(&pDf->m_PipeIdMapFMutex);
    return pPipe;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_PipeClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfNw_PipeClose(DtDfNw* pDf, const DtFileObject* pFileObject, Int PipeType,
                                                                                UInt Uuid)
{
    DtStatus Status = DT_STATUS_OK;
    DtPipe* pPipe = NULL;
    Int Id = -1;
    Int Timeout;

    // Check input parameters
    Id = Uuid >> 20;
    if (Id == 0)
        return DT_STATUS_INVALID_PARAMETER;
    DtFastMutexAcquire(&pDf->m_PipeIdMapFMutex);

    pPipe = DtVectorPipe_At(pDf->m_pPipeIdMap, Id-1);
    if (pPipe == NULL)
        Status = DT_STATUS_INVALID_PARAMETER;
    else if (!pPipe->m_InUse)
        Status = DT_STATUS_NOT_INITIALISED;
    else if (PipeType!=-1 && pPipe->m_PipeType != PipeType)
        Status = DT_STATUS_INVALID_PARAMETER;
    else if (pFileObject!=NULL && !DtFileCompare(pFileObject, &pPipe->m_FileObject))
        Status = DT_STATUS_NOT_FOUND;
    if (!DT_SUCCESS(Status))
    {
        DtFastMutexRelease(&pDf->m_PipeIdMapFMutex);
        return Status;
    }
    PipeType = pPipe->m_PipeType;
    Timeout = 100;
    while (pPipe->m_Locked && Timeout !=0)
    {
        // Wait for pipe to be released
        if (Timeout == 100)
            DtDbgOutPipe(AVG, pPipe, "Pipe locked, waiting. Uuid: %xh", Uuid);
        DtSleep(100);
        Timeout--;
    }
    if (Timeout==0 && pPipe->m_Locked)
        DtDbgOutPipe(ERR, pPipe, "Pipe still locked after timeout. Uuid: %xh", Uuid);
    else if (Timeout != 100)
        DtDbgOutPipe(AVG, pPipe, "Pipe unlocked. Uuid: %xh", Uuid);

    DtDbgOutPipe(AVG, pPipe, "Closing pipe. Uuid: %xh", Uuid);
    switch (PipeType)
    {
    case DT_PIPE_RX_NRT:
        DtFastMutexAcquire(&pDf->m_PipeRxNrtFMutex);
        pPipe->m_InUse = FALSE;
        DtPipe_ReleaseResources(pPipe);
        DtFastMutexRelease(&pDf->m_PipeRxNrtFMutex);
        break;
    case DT_PIPE_TX_NRT:
        DtFastMutexAcquire(&pDf->m_PipeTxNrtFMutex);
        pPipe->m_InUse = FALSE;
        DtPipe_ReleaseResources(pPipe);
        DtFastMutexRelease(&pDf->m_PipeTxNrtFMutex);
        break;
    case DT_PIPE_RX_HWQ:
        pPipe->m_InUse = FALSE;
        DtPipe_ReleaseResources(pPipe);
        break;
    case DT_PIPE_TX_RT_HWQ:
        pPipe->m_InUse = FALSE;
        DtPipe_ReleaseResources(pPipe);
        break;
    case DT_PIPE_RX_RT_HWP:
    case DT_PIPE_TX_RT_HWP:
    {
        DtVectorPipe* pVectorPipe = pDf->m_pPipeRxRtHwpList;
        DtFastMutex* pFastMutex = &pDf->m_PipeRxRtHwpListFMutex;
        if (PipeType == DT_PIPE_TX_RT_HWP)
        {
            pVectorPipe = pDf->m_pPipeTxRtHwpList;
            pFastMutex = &pDf->m_PipeTxRtHwpListFMutex;
        }
        DtFastMutexAcquire(pFastMutex);
        pPipe->m_InUse = FALSE;
        DtPipe_ReleaseResources(pPipe);
        DtFastMutexRelease(pFastMutex);
        break;
    }
    case DT_PIPE_RX_RT_SWP:
    case DT_PIPE_TX_RT_SWP:
    {
        DtVectorPipe* pVectorPipe = pDf->m_pPipeRxRtSwpList;
        DtFastMutex* pFastMutex = &pDf->m_PipeRxRtSwpListFMutex;
        Int Index = -1;
        // Remove pipe form the IdMap list
        DtVectorPipe_SetEmpty(pDf->m_pPipeIdMap, Id - 1);
        
        // Remove pipe from the type specific list
        if (PipeType == DT_PIPE_TX_RT_SWP)
        {
            pVectorPipe = pDf->m_pPipeTxRtSwpList;
            pFastMutex = &pDf->m_PipeTxRtSwpListFMutex;
        }
        DtFastMutexAcquire(pFastMutex);
        Index = DtVectorPipe_FindPipeIndex(pVectorPipe, pPipe);
        if (Index != -1)
            DtVectorPipe_Erase(pVectorPipe, Index);
        DtPipe_Close(pPipe);
        DtFastMutexRelease(pFastMutex);
        break;
    }
    default:
        Status = DT_STATUS_NOT_SUPPORTED;
    }
    // We can't use the pPipe object in the logging. The pPipe could be pointing to freed
    // memory
    if (DT_SUCCESS(Status))
        DtDbgOutDf(AVG, NW, pDf, "Closed pipe. Uuid: % xh", Uuid);
    else
        DtDbgOutDf(ERR, NW, pDf, "Error closing pipe. Uuid: %xh", Uuid);
    DtFastMutexRelease(&pDf->m_PipeIdMapFMutex);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_CheckFilterDuplication -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfNw_CheckFilterDuplication(DtDfNw* pDf)
{
    int i, j;

    DtFastMutexAcquire(&pDf->m_PipeRxRtSwpListFMutex);
    DtFastMutexAcquire(&pDf->m_PipeRxRtHwpListFMutex);

    // Loop all Hwp's snd Swp's
    for (i = 0; i < DtVectorPipe_Size(pDf->m_pPipeRxRtHwpList); i++)
    {
        DtPipeRxRtHwp* pHwp = (DtPipeRxRtHwp*)DtVectorPipe_At(pDf->m_pPipeRxRtHwpList, i);
        Bool Duplicate = FALSE;
        if (!pHwp->m_InUse || !pHwp->m_IpParsSet)
            continue;

        // SWP duplicate with this HWP
        for (j = 0; j < DtVectorPipe_Size(pDf->m_pPipeRxRtSwpList) && !Duplicate; j++)
        {
            DtPipeRxRtSwp* pSwp = (DtPipeRxRtSwp*)DtVectorPipe_At(pDf->m_pPipeRxRtSwpList, 
                                                                                       j);
            if (!pSwp->m_InUse || !pSwp->m_IpParsSet)
                continue;
            Duplicate = DtPipe_IsDuplicateFilter((DtPipe*)pHwp, (DtPipe*)pSwp);
        }
        if (!Duplicate)
        {
            // Check other HWP
            for (j=i+1; j<DtVectorPipe_Size(pDf->m_pPipeRxRtHwpList) && !Duplicate; j++)
            {
                DtPipeRxRtHwp* pHwp2 = (DtPipeRxRtHwp*)
                                              DtVectorPipe_At(pDf->m_pPipeRxRtHwpList, j);
                Duplicate = DtPipe_IsDuplicateFilter((DtPipe*)pHwp, (DtPipe*)pHwp2);
            }
        }
        DtPipe_SetIpFilterDuplicate((DtPipe*)pHwp, Duplicate);
    }

    DtFastMutexRelease(&pDf->m_PipeRxRtHwpListFMutex);
    DtFastMutexRelease(&pDf->m_PipeRxRtSwpListFMutex);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_DeregisterEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfNw_DeregisterEvent(DtPipe* pPipe, DtTrigEvtDriver* pTrigEvt)
{
    if (DtPipe_IsPerIntEvent(pPipe))
    {
        Int Index;
        DtFastMutexAcquire(&DTDFNW_PIPE->m_PerIntEvtFMutex);
        Index = DtVectorPipe_FindPipeIndex(DTDFNW_PIPE->m_PerIntEvtList, pPipe);
        if (Index != -1)
        {
            // Wait until PerIntHandler finished
            DtSpinLockAcquire(&DTDFNW_PIPE->m_PerItvSpinLock);
            DtVectorPipe_Erase(DTDFNW_PIPE->m_PerIntEvtList, Index);
            DtSpinLockRelease(&DTDFNW_PIPE->m_PerItvSpinLock);
        }
        DtFastMutexRelease(&DTDFNW_PIPE->m_PerIntEvtFMutex);
    }
    return DtPipe_DeregisterEvent(pPipe, pTrigEvt);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_DeregisterKEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfNw_DeregisterKEvent(DtPipe* pPipe, DtEvent* pEvent, Int Id)
{
    DtTrigEvtDriver TrigEvtDriver;
    TrigEvtDriver.m_KernelEvent = TRUE;
    TrigEvtDriver.m_Id = Id;
    TrigEvtDriver.m_pKernelEvent = pEvent;
    return DtDfNw_DeregisterEvent(pPipe, &TrigEvtDriver);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_RegisterEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfNw_RegisterEvent(DtPipe* pPipe, DtTrigEvtDriver* pTrigEvt, UInt Count)
{
    DtStatus Status = DtPipe_RegisterEvent(pPipe, pTrigEvt, Count);
    if (DT_SUCCESS(Status))
    {
        if (DtPipe_IsPerIntEvent(pPipe))
        {
            DtFastMutexAcquire(&DTDFNW_PIPE->m_PerIntEvtFMutex);
            // Wait until PerIntHandler finished
            DtSpinLockAcquire(&DTDFNW_PIPE->m_PerItvSpinLock);
            Status = DtVectorPipe_PushBack(DTDFNW_PIPE->m_PerIntEvtList, pPipe);
            DT_ASSERT(DT_SUCCESS(Status));
            DtSpinLockRelease(&DTDFNW_PIPE->m_PerItvSpinLock);
            DtFastMutexRelease(&DTDFNW_PIPE->m_PerIntEvtFMutex);
        }
    }
    if (!DT_SUCCESS(Status))
        DtDbgOutPipe(ERR, pPipe, "Error registering event. Status: %xh", Status);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfNw_RegisterKEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfNw_RegisterKEvent(DtPipe* pPipe, DtEvent* pEvent, Int Id)
{
    DtTrigEvtDriver TrigEvtDriver;
    TrigEvtDriver.m_KernelEvent = TRUE;
    TrigEvtDriver.m_Id = Id;
    TrigEvtDriver.m_pKernelEvent = pEvent;
    // Count is always 0. Assert if AVAIL_CNT is used.
    DT_ASSERT(Id != DT_PIPE_EVENT_ID_DATA_AVAIL_CNT);
    return DtDfNw_RegisterEvent(pPipe, &TrigEvtDriver, 0);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfNw implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Df's stub
#define NW_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                      DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfNw));
// Helper macro to cast stub's DtDf member to DtDfNw
#define STUB_NW   ((DtIoStubDfNw*)pStub)
#define STUB_DF  ((DtDfNw*)STUB_NW->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus DtIoStubDfNwOnCmd(const DtIoStub* pStub, 
                                             DtIoStubIoParams* pIoParams, Int * pOutSize);
static DtStatus  DtIoStubDfNwOnCmdGetOperationalMode(const DtIoStubDfNw*,
                                                      DtIoctlNwCmdGetOpModeOutput*);
static DtStatus  DtIoStubDfNwOnCmdSetOperationalMode(const DtIoStubDfNw*,
                                                 const DtIoctlNwCmdSetOpModeInput*);
static DtStatus  DtIoStubDfNwOnCmdSetPipeOperationalMode(const DtIoStubDfNw*, DtPipe*,
                                                     const DtIoctlPipeCmdSetOpModeInput*);
static DtStatus  DtIoStubDfNwOnCmdPipeOpen(const DtIoStubDfNw* pStub,
                                                const DtIoctlNwCmdPipeOpenInput* pInpData,
                                                DtIoctlNwCmdPipeOpenOutput* pOutData, 
                                                const DtFileObject* pFileObject);
static DtStatus  DtIoStubDfNwOnCmdPipeClose(const DtIoStubDfNw* pStub,
                                               const DtIoctlNwCmdPipeCloseInput* pInpData, 
                                               const DtFileObject* pFileObject);
static DtStatus  DtIoStubDfNwOnCmdSetSharedBuffer(const DtIoStubDfNw* pStub, 
                                        DtPipe* pPipe,
                                        const DtIoctlPipeCmdSetSharedBufferInput* pInData,
                                        UInt8* pBuffer, Int BufSize);
static DtStatus  DtIoStubDfNwOnCmdSetSharedKBuffer(const DtIoStubDfNw* pStub, DtPipe*,
                                      const DtIoctlPipeCmdSetSharedKBufferInput* pInpData,
                                          DtIoctlPipeCmdSetSharedKBufferOutput* pOutData);
static DtStatus DtIoStubDfNwOnCmdReleaseSharedBuffer(const DtIoStubDfNw* pStub, DtPipe*);
static DtStatus DtIoStubDfNwOnCmdGetProperties(const DtIoStubDfNw* pStub, DtPipe*, 
                                                      DtIoctlPipeCmdGetPropertiesOutput*);
static DtStatus DtIoStubDfNwOnCmdGetStatus(const DtIoStubDfNw* pStub, 
                                                 DtPipe*, DtIoctlPipeCmdGetStatusOutput*);
static DtStatus DtIoStubDfNwOnCmdIssuePipeFlush(const DtIoStubDfNw* pStub, DtPipe*);
static DtStatus  DtIoStubDfNwOnCmdGetRxWriteOffset(const DtIoStubDfNw* pStub, DtPipe*,
                                          DtIoctlPipeCmdGetRxWriteOffsetOutput* pOutData);
static DtStatus  DtIoStubDfNwOnCmdSetRxReadOffset(const DtIoStubDfNw* pStub, DtPipe*,
                                       const DtIoctlPipeCmdSetRxReadOffsetInput* pInData);
static DtStatus  DtIoStubDfNwOnCmdGetTxReadOffset(const DtIoStubDfNw* pStub, DtPipe*, 
                                           DtIoctlPipeCmdGetTxReadOffsetOutput* pOutData);
static DtStatus  DtIoStubDfNwOnCmdSetTxWriteOffset(const DtIoStubDfNw* pStub, DtPipe*,
                                      const DtIoctlPipeCmdSetTxWriteOffsetInput* pInData);
static DtStatus DtIoStubDfNwOnCmdRegisterEvent(const DtIoStubDfNw* pStub, DtPipe*,
                                         const DtIoctlPipeCmdRegisterEventInput* pInData);
static DtStatus DtIoStubDfNwOnCmdDeregisterEvent(const DtIoStubDfNw* pStub, DtPipe*,
                                       const DtIoctlPipeCmdDeregisterEventInput* pInData);
static DtStatus DtIoStubDfNwOnCmdSetIpFilter(const DtIoStubDfNw* pStub, DtPipe*,
                                           const DtIoctlPipeCmdSetIpFilterInput* pInData);
static DtStatus  DtIoStubDfEMACOnCmd(const DtIoStub* pStub,
                                              DtIoStubIoParams* pIoParams, Int* pOutSize);
static DtStatus  DtIoStubDfNw_OnCmdGetMacAddressPerm(const DtIoStubDfNw* pStub,
                                         DtIoctlEMACCmdGetMacAddressPermOutput* pOutData);
static DtStatus  DtIoStubDfNw_OnCmdSetMacAddress(const DtIoStubDfNw* pStub,
                                         const DtIoctlEMACCmdSetMacAddressInput* pInData);
static DtStatus  DtIoStubDfNw_OnCmdGetMacAddress(const DtIoStubDfNw* pStub,
                                             DtIoctlEMACCmdGetMacAddressOutput* pOutData);
static DtStatus  DtIoStubDfNw_OnCmdGetMaxMulticastList(const DtIoStubDfNw* pStub,
                                       DtIoctlEMACCmdGetMaxMulticastListOutput* pOutData);
static DtStatus  DtIoStubDfNw_OnCmdSetMulticastList(const DtIoStubDfNw* pStub,
                                      const DtIoctlEMACCmdSetMulticastListInput* pInData);
static DtStatus  DtIoStubDfNw_OnCmdSetPacketFilter(const DtIoStubDfNw* pStub,
                                       const DtIoctlEMACCmdSetPacketFilterInput* pInData);
static DtStatus  DtIoStubDfNw_OnCmdGetPacketFilter(const DtIoStubDfNw* pStub,
                                           DtIoctlEMACCmdGetPacketFilterOutput* pOutData);
static DtStatus  DtIoStubDfNw_OnCmdGetCounter(const DtIoStubDfNw* pStub,
                                             const DtIoctlEMACCmdGetCounterInput* pInData,
                                             DtIoctlEMACCmdGetCounterOutput* pOutData);
static DtStatus  DtIoStubDfNw_OnCmdGetLinkStatus(const DtIoStubDfNw* pStub);
static DtStatus  DtIoStubDfNw_OnCmdGetPhySpeed(const DtIoStubDfNw* pStub,
                                               DtIoctlEMACCmdGetPhySpeedOutput* pOutData);
static DtStatus  DtIoStubDfNw_OnCmdSetPhySpeed(const DtIoStubDfNw* pStub,
                                           const DtIoctlEMACCmdSetPhySpeedInput* pInData);
static DtStatus  DtIoStubDfNw_OnCmdRegisterCallback(const DtIoStubDfNw* pStub,
                                      const DtIoctlEMACCmdRegisterCallbackInput* pInData);
static DtStatus  DtIoStubDfNw_OnCmdUnregisterCallback(const DtIoStubDfNw* pStub,
                                    const DtIoctlEMACCmdUnregisterCallbackInput* pInData);
static DtStatus  DtIoStubDfNw_OnCmdResetNwStatus(const DtIoStubDfNw* pStub,
                                         const DtIoctlEMACCmdResetNwStatusInput* pInData);
static DtStatus  DtIoStubDfNwOnCmdRegisterOpModeCallback(const DtIoStubDfNw* pStub,
                                  const DtIoctlNwCmdRegisterOpModeCallbackInput* pInData);
DtStatus  DtIoStubDfPipeOnCmd(const DtIoStub* pStub, DtIoStubIoParams* pIoParams,
                                                                           Int* pOutSize);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_NW;
DECL_DT_IOCTL_CMD_PROPS_EMAC;
DECL_DT_IOCTL_CMD_PROPS_PIPE;

static const DtIoctlProperties  IOSTUB_DF_NW_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubDf_OnExclAccessCmd,
        NULL, NULL),
    DT_IOCTL_PROPS_NW_CMD(
        DtIoStubDfNwOnCmd,
        NULL, NULL),
    DT_IOCTL_PROPS_EMAC_CMD(
        DtIoStubDfEMACOnCmd,
        NULL, NULL),
    DT_IOCTL_PROPS_PIPE_CMD(
        DtIoStubDfPipeOnCmd,
        NULL, NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfNw - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubDfNw_Close(DtIoStub* pStub)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubDfNw*  DtIoStubDfNw_Open(DtDf*  pDf)
{
    DtIoStubDfNw*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    DF_NW_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfNw, pDf, NULL, 
                                                             DtIoStubDfNw_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_DF_NW_IOCTLS);
    pStub = (DtIoStubDfNw*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfNw - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfNwOnCmd +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfNwOnCmd(const DtIoStub* pStub, DtIoStubIoParams* pIoParams, 
                                                                            Int* pOutSize)
{
    DtStatus Status= DT_STATUS_OK;
    const DtIoctlNwCmdInput*  pInData = NULL;
    DtIoctlNwCmdOutput*  pOutData = NULL;
    
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_NW_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(STUB_NW->m_pDf, &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, NW, pStub, "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_NwCmd;
    if (pIoParams->m_OutReqSize > 0)
    { 
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_NwCmd;
    }

    if (STUB_DF->m_ReducedFunctionality)
        return DT_STATUS_NOT_INITIALISED;

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_NW_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfNwOnCmdGetOperationalMode(STUB_NW, &pOutData->m_GetOpMode);
        break;
    case DT_NW_CMD_SET_OPERATIONAL_MODE:
        Status = DtIoStubDfNwOnCmdSetOperationalMode(STUB_NW, &pInData->m_SetOpMode);
        break;
    case DT_NW_CMD_PIPE_OPEN:
        Status = DtIoStubDfNwOnCmdPipeOpen(STUB_NW, &pInData->m_PipeOpen, 
                                               &pOutData->m_PipeOpen, pIoParams->m_pFile);
        break;
    case DT_NW_CMD_PIPE_CLOSE:
        Status = DtIoStubDfNwOnCmdPipeClose(STUB_NW, &pInData->m_PipeClose, 
                                                                      pIoParams->m_pFile);
        break;
    case DT_NW_CMD_REGISTER_OPMODE_CALLBACK:
        Status = DtIoStubDfNwOnCmdRegisterOpModeCallback(STUB_NW, 
                                                      &pInData->m_RegisterOpModeCallback);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfNwOnCmdGetOperationalMode(const DtIoStubDfNw* pStub,
                                                    DtIoctlNwCmdGetOpModeOutput* pOutData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtDfNw_GetOperationalMode(STUB_DF, &pOutData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfNwOnCmdSetOperationalMode(const DtIoStubDfNw* pStub,
                                                const DtIoctlNwCmdSetOpModeInput* pInData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtDfNw_SetOperationalMode(STUB_DF, pInData->m_OpMode);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdPipeOpen -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfNwOnCmdPipeOpen(const DtIoStubDfNw* pStub,
                    const DtIoctlNwCmdPipeOpenInput* pInData,
                    DtIoctlNwCmdPipeOpenOutput* pOutData, const DtFileObject* pFileObject)
{
    DtStatus Status = DT_STATUS_OK;
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL && pOutData!=NULL);

    Status = DtDfNw_PipeOpen(STUB_DF, pFileObject, pInData->m_PipeType, 
                                                             &pOutData->m_PipeUuid, NULL);
    if (Status == DT_STATUS_IN_USE && pInData->m_PipeTypeFallback != -1)
        Status = DtDfNw_PipeOpen(STUB_DF, pFileObject, pInData->m_PipeTypeFallback, 
                                                             &pOutData->m_PipeUuid, NULL);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdPipeClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfNwOnCmdPipeClose(const DtIoStubDfNw* pStub,
               const DtIoctlNwCmdPipeCloseInput* pInData, const DtFileObject* pFileObject)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    return DtDfNw_PipeClose(STUB_DF, pFileObject, -1, pInData->m_CmdHdr.m_Uuid);
}


// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdRegisterOpModeCallback -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfNwOnCmdRegisterOpModeCallback(const DtIoStubDfNw* pStub,
                                   const DtIoctlNwCmdRegisterOpModeCallbackInput* pInData)
{
    if (STUB_DF->m_OpModeCB != NULL && pInData->m_OpModeCallback != NULL)
        return DT_STATUS_IN_USE;
    DtSpinLockAcquire(&STUB_DF->m_OpModeCBSpinlock);
    STUB_DF->m_OpModeCB = pInData->m_OpModeCallback;
    STUB_DF->m_pContexOpModeCB = pInData->m_pContext;
    DtSpinLockRelease(&STUB_DF->m_OpModeCBSpinlock);
    return DT_STATUS_OK;
}


// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ EMACOnCmd +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfEMACOnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfEMACOnCmd(const DtIoStub* pStub, DtIoStubIoParams* pIoParams,
                                                                            Int* pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlEMACCmdInput*  pInData = NULL;
    DtIoctlEMACCmdOutput*  pOutData = NULL;

    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_EMAC_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, NW, pStub, "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_EMACCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_EMACCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_EMAC_CMD_GET_MACADDRESS_PERM:
        Status = DtIoStubDfNw_OnCmdGetMacAddressPerm(STUB_NW,
                                                          &pOutData->m_GetMacAddressPerm);
        break;
    case DT_EMAC_CMD_SET_MACADDRESS:
        Status = DtIoStubDfNw_OnCmdSetMacAddress(STUB_NW, &pInData->m_SetMacAddress);
        break;
    case DT_EMAC_CMD_GET_MACADDRESS:
        Status = DtIoStubDfNw_OnCmdGetMacAddress(STUB_NW, &pOutData->m_GetMacAddress);
        break;
    case DT_EMAC_CMD_GET_MAX_MULTICAST_LIST:
        Status = DtIoStubDfNw_OnCmdGetMaxMulticastList(STUB_NW,
                                                        &pOutData->m_GetMaxMulticastList);
        break;
    case DT_EMAC_CMD_SET_MULTICAST_LIST:
        Status = DtIoStubDfNw_OnCmdSetMulticastList(STUB_NW,
                                                            &pInData->m_SetMulticastList);
        break;
    case DT_EMAC_CMD_SET_PACKET_FILTER:
        Status = DtIoStubDfNw_OnCmdSetPacketFilter(STUB_NW, &pInData->m_SetPacketFilter);
        break;
    case DT_EMAC_CMD_GET_PACKET_FILTER:
        Status = DtIoStubDfNw_OnCmdGetPacketFilter(STUB_NW, &pOutData->m_GetPacketFilter);
        break;
    case DT_EMAC_CMD_GET_COUNTER:
        Status = DtIoStubDfNw_OnCmdGetCounter(STUB_NW, &pInData->m_GetCounter,
                                                                 &pOutData->m_GetCounter);
        break;
    case DT_EMAC_CMD_GET_LINKSTATUS:
        // This will trigger the callback function to update the status
        Status = DtIoStubDfNw_OnCmdGetLinkStatus(STUB_NW);
        break;
    case DT_EMAC_CMD_GET_PHY_SPEED:
        Status = DtIoStubDfNw_OnCmdGetPhySpeed(STUB_NW, &pOutData->m_GetPhySpeed);
        break;
    case DT_EMAC_CMD_SET_PHY_SPEED:
        Status = DtIoStubDfNw_OnCmdSetPhySpeed(STUB_NW, &pInData->m_SetPhySpeed);
        break;
    case DT_EMAC_CMD_REGISTER_CALLBACK:
        Status = DtIoStubDfNw_OnCmdRegisterCallback(STUB_NW,
                                                            &pInData->m_RegisterCallback);
        break;
    case DT_EMAC_CMD_UNREGISTER_CALLBACK:
        Status = DtIoStubDfNw_OnCmdUnregisterCallback(STUB_NW,
                                                          &pInData->m_UnregisterCallback);
        break;
    case DT_EMAC_CMD_RESET_NW_STATUS:
        Status = DtIoStubDfNw_OnCmdResetNwStatus(STUB_NW, &pInData->m_ResetNwStatus);
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw_OnCmdGetMacAddressPerm -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfNw_OnCmdGetMacAddressPerm(const DtIoStubDfNw* pStub,
                                          DtIoctlEMACCmdGetMacAddressPermOutput* pOutData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    DT_ASSERT(STUB_DF->m_pBcEMAC10G);
    return DtBcEMAC10G_GetMacAddressPerm(STUB_DF->m_pBcEMAC10G, pOutData->m_Address);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw_OnCmdSetMacAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfNw_OnCmdSetMacAddress(const DtIoStubDfNw* pStub,
                                          const DtIoctlEMACCmdSetMacAddressInput* pInData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    DT_ASSERT(STUB_DF->m_pBcEMAC10G != NULL);
    return DtBcEMAC10G_SetMacAddress(STUB_DF->m_pBcEMAC10G, 
                                                        (UInt8*)&(pInData->m_Address[0]));
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw_OnCmdGetMacAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfNw_OnCmdGetMacAddress(const DtIoStubDfNw* pStub,
                                              DtIoctlEMACCmdGetMacAddressOutput* pOutData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    DT_ASSERT(STUB_DF->m_pBcEMAC10G);
    return DtBcEMAC10G_GetMacAddressCurr(STUB_DF->m_pBcEMAC10G, pOutData->m_Address);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw_OnCmdGetMaxMulticastList -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfNw_OnCmdGetMaxMulticastList(const DtIoStubDfNw* pStub,
                                        DtIoctlEMACCmdGetMaxMulticastListOutput* pOutData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    pOutData->m_MaxListSize = MAX_MULTICAST_ITEMS;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw_OnCmdSetMulticastList -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfNw_OnCmdSetMulticastList(const DtIoStubDfNw* pStub,
                                       const DtIoctlEMACCmdSetMulticastListInput* pInData)
{
    UInt Items = 0;
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    // Get multicast list lock
    if (pInData->m_NumItems > MAX_MULTICAST_ITEMS)
        return DT_STATUS_INVALID_PARAMETER;
    DtFastMutexAcquire(&STUB_DF->m_MulticastListFastMutex);
    DtMemCopy(STUB_DF->m_MulticastList, &pInData->m_Items[0], pInData->m_NumItems * 6);
    STUB_DF->m_NumMulticasts = pInData->m_NumItems;
    DtDbgOutIoStubDf(AVG, NW, pStub, "New multicast list. #Items: %i", 
                                                                STUB_DF->m_NumMulticasts);
    for (Items = 0; Items < STUB_DF->m_NumMulticasts; Items++)
    {
        DtDbgOutDf(AVG, NW, STUB_DF, "[%i] Multicast %02X:%02X:%02X:%02X:%02X:%02X",
                  Items,
                  STUB_DF->m_MulticastList[Items][0], STUB_DF->m_MulticastList[Items][1], 
                  STUB_DF->m_MulticastList[Items][2], STUB_DF->m_MulticastList[Items][3], 
                  STUB_DF->m_MulticastList[Items][4], STUB_DF->m_MulticastList[Items][5]);
    }
    DtFastMutexRelease(&STUB_DF->m_MulticastListFastMutex);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw_OnCmdSetPacketFilter -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfNw_OnCmdSetPacketFilter(const DtIoStubDfNw* pStub,
                                        const DtIoctlEMACCmdSetPacketFilterInput* pInData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    STUB_DF->m_PacketFilter = pInData->m_PacketFilter;
    DtBcIPSUMCHK_SetPromicuousMode(STUB_DF->m_pBcIpSumChk, 
                                 (pInData->m_PacketFilter & DT_MAC_FLT_PROMISCUOUS) != 0);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw_OnCmdGetPacketFilter -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfNw_OnCmdGetPacketFilter(const DtIoStubDfNw* pStub,
                                            DtIoctlEMACCmdGetPacketFilterOutput* pOutData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    pOutData->m_PacketFilter = STUB_DF->m_PacketFilter;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw_OnCmdGetCounter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfNw_OnCmdGetCounter(const DtIoStubDfNw* pStub,
   const DtIoctlEMACCmdGetCounterInput* pInData, DtIoctlEMACCmdGetCounterOutput* pOutData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    DT_ASSERT(pOutData != NULL);
    DT_ASSERT(STUB_DF->m_pBcEMAC10G != NULL);
    return DtBcEMAC10G_GetCounter(STUB_DF->m_pBcEMAC10G, pInData->m_CounterId,
                                                                      &pOutData->m_Value);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw_OnCmdGetLinkStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfNw_OnCmdGetLinkStatus(const DtIoStubDfNw* pStub)
{
    // This will trigger the callback function to update the status
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    return DtBcEMAC10G_GetLinkStatus(STUB_DF->m_pBcEMAC10G);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw_OnCmdGetPhySpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfNw_OnCmdGetPhySpeed(const DtIoStubDfNw* pStub,
                                                DtIoctlEMACCmdGetPhySpeedOutput* pOutData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    return DtBcEMAC10G_GetPhySpeed(STUB_DF->m_pBcEMAC10G, &pOutData->m_Speed);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw_OnCmdSetPhySpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfNw_OnCmdSetPhySpeed(const DtIoStubDfNw* pStub,
                                            const DtIoctlEMACCmdSetPhySpeedInput* pInData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw_OnCmdRegisterCallback -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfNw_OnCmdRegisterCallback(const DtIoStubDfNw* pStub,
                                       const DtIoctlEMACCmdRegisterCallbackInput* pInData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    DT_ASSERT(STUB_DF->m_pBcEMAC10G != NULL);

    return DtBcEMAC10G_RegisterCallback(STUB_DF->m_pBcEMAC10G, 
                                      pInData->m_LinkStatusCallback, pInData->m_pContext);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw_OnCmdUnregisterCallback -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfNw_OnCmdUnregisterCallback(const DtIoStubDfNw* pStub,
                                     const DtIoctlEMACCmdUnregisterCallbackInput* pInData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    DT_ASSERT(STUB_DF->m_pBcEMAC10G != NULL);
    return DtBcEMAC10G_UnregisterCallback(STUB_DF->m_pBcEMAC10G, pInData->m_pContext);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNw_OnCmdResetNwStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfNw_OnCmdResetNwStatus(
    const DtIoStubDfNw* pStub,
    const DtIoctlEMACCmdResetNwStatusInput* pInData)
{
    //UInt  Offset;
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    // Nothing to do for now, we moved it to the pipe itself. Issue Flush pipe
    
    
    return DT_STATUS_OK;
}

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ PipeOnCmd +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfPipeOnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfPipeOnCmd(const DtIoStub* pStub, DtIoStubIoParams* pIoParams,
                                                                            Int* pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlPipeCmdInput* pInData = NULL;
    DtIoctlPipeCmdOutput* pOutData = NULL;
    Int PipeId = (pIoParams->m_Uuid >> 20) & 0xFFF;
    DtPipe* pPipe = NULL;

    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_PIPE_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);
    DT_ASSERT(PipeId != 0);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, NW, pStub, "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_PipeCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_PipeCmd;
    }

    // Get pipe object
    if (PipeId >= STUB_DF->m_NumStaticPipes)
        DtFastMutexAcquire(&STUB_DF->m_PipeIdMapFMutex);
    pPipe = DtVectorPipe_At(STUB_DF->m_pPipeIdMap, PipeId-1);
    if (pPipe == NULL)
    {
        DtDbgOutIoStubDf(ERR, NW, pStub, "ERROR: DtPipe object not found for ID: %d,"
                                                  " Uuid:%xh", PipeId, pIoParams->m_Uuid);
        if (PipeId >= STUB_DF->m_NumStaticPipes)
            DtFastMutexRelease(&STUB_DF->m_PipeIdMapFMutex);
        return DT_STATUS_INVALID_PARAMETER;
    }
    pPipe->m_Locked = TRUE;
    if (PipeId >= STUB_DF->m_NumStaticPipes)
        DtFastMutexRelease(&STUB_DF->m_PipeIdMapFMutex);

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_PIPE_CMD_SET_OPERATIONAL_MODE:
        Status = DtIoStubDfNwOnCmdSetPipeOperationalMode(STUB_NW, pPipe, 
                                                                   &pInData->m_SetOpMode);
        break;
        
    case DT_PIPE_CMD_SET_SHARED_BUFFER:
    {
        UInt8* pDmaBuffer;
        UInt  DmaBufferSize;
#if defined(WINBUILD)
        PMDL  pMdl;
        NTSTATUS  NtStatus;
        // Retrieve MDL and virtual buffer from request object
        NtStatus = WdfRequestRetrieveOutputWdmMdl(pIoParams->m_pIoctl->m_WdfRequest,
                                                                                   &pMdl);
        if (NtStatus != STATUS_SUCCESS)
        {
            DtDbgOutIoStubDf(ERR, NW, pStub, "ERROR: WdfRequestRetrieveOutputWdmMdl "
                                                           "error code:  %08x", NtStatus);
            Status = DT_STATUS_OUT_OF_RESOURCES;
        }
        if (DT_SUCCESS(Status))
        {
            pDmaBuffer = MmGetMdlVirtualAddress(pMdl);
            if (pDmaBuffer == NULL)
                Status = DT_STATUS_OUT_OF_MEMORY;
            DmaBufferSize = MmGetMdlByteCount(pMdl);
        }
#else // LINBUILD
        DmaBufferSize = (UInt)pInData->m_SetSharedBuffer.m_BufferSize;
#if defined(LIN32)
        pDmaBuffer = (char*)(UInt32)pInData->m_SetSharedBuffer.m_BufferAddr;
#else
        pDmaBuffer = (char*)(UInt64)pInData->m_SetSharedBuffer.m_BufferAddr;
#endif
#endif
        DtDbgOutIoStubDf(MIN, NW, pStub, "DT_PIPE_CMD_SET_SHARED_BUFFER: pBuffer:%p"
                                                   " Size:%i", pDmaBuffer, DmaBufferSize);
        if (DT_SUCCESS(Status))
            Status = DtIoStubDfNwOnCmdSetSharedBuffer(STUB_NW, pPipe,
                &pInData->m_SetSharedBuffer, pDmaBuffer, DmaBufferSize);
        break;
    }
    case DT_PIPE_CMD_SET_SHAREDK_BUFFER:
        Status = DtIoStubDfNwOnCmdSetSharedKBuffer(STUB_NW, pPipe,
            &pInData->m_SetSharedKBuffer, &pOutData->m_SetSharedKBuffer);
        break;
    case DT_PIPE_CMD_RELEASE_SHARED_BUFFER:
        Status = DtIoStubDfNwOnCmdReleaseSharedBuffer(STUB_NW, pPipe);
        break;
    case DT_PIPE_CMD_GET_PROPERTIES:
        Status = DtIoStubDfNwOnCmdGetProperties(STUB_NW, pPipe,
                                                              &pOutData->m_GetProperties);
        break;
    case DT_PIPE_CMD_GET_STATUS:
        Status = DtIoStubDfNwOnCmdGetStatus(STUB_NW, pPipe, &pOutData->m_GetStatus);
        break;
    case DT_PIPE_CMD_ISSUE_PIPE_FLUSH:
        Status = DtIoStubDfNwOnCmdIssuePipeFlush(STUB_NW, pPipe); 
        break;

    case DT_PIPE_CMD_GET_RX_WRITE_OFFSET:
        Status = DtIoStubDfNwOnCmdGetRxWriteOffset(STUB_NW, pPipe, 
            &pOutData->m_GetRxWriteOffset);
        break;
    case DT_PIPE_CMD_SET_RX_READ_OFFSET:
        Status = DtIoStubDfNwOnCmdSetRxReadOffset(STUB_NW, pPipe,
            &pInData->m_SetRxReadOffset);
        break;
    case DT_PIPE_CMD_GET_TX_READ_OFFSET:
        Status = DtIoStubDfNwOnCmdGetTxReadOffset(STUB_NW, pPipe,
            &pOutData->m_GetTxReadOffset);
        break;
    case DT_PIPE_CMD_SET_TX_WRITE_OFFSET:
        Status = DtIoStubDfNwOnCmdSetTxWriteOffset(STUB_NW, pPipe,
            &pInData->m_SetTxWriteOffset);
        break;
     case DT_PIPE_CMD_REGISTER_EVENT:
         Status = DtIoStubDfNwOnCmdRegisterEvent(STUB_NW, pPipe,
             &pInData->m_RegisterEvent);

         break;
     case DT_PIPE_CMD_DEREGISTER_EVENT:
         Status = DtIoStubDfNwOnCmdDeregisterEvent(STUB_NW, pPipe,
             &pInData->m_DeregisterEvent);
         break;
     case DT_PIPE_CMD_SET_IPFILTER:
         Status = DtIoStubDfNwOnCmdSetIpFilter(STUB_NW, pPipe, &pInData->m_SetIpFilter);
         break;
    default:
        DT_ASSERT(FALSE);
        Status = DT_STATUS_NOT_SUPPORTED;
    }
    // Release locked state
    pPipe->m_Locked = FALSE;
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdSetPipeOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfNwOnCmdSetPipeOperationalMode(const DtIoStubDfNw* pStub, 
                               DtPipe* pPipe, const DtIoctlPipeCmdSetOpModeInput* pInData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    return DtPipe_SetOperationalMode(pPipe, pInData->m_OpMode);
}


// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdSetSharedBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfNwOnCmdSetSharedBuffer(const DtIoStubDfNw* pStub, DtPipe* pPipe,
        const DtIoctlPipeCmdSetSharedBufferInput* pInData, UInt8* pBuffer, Int BufferSize)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL && pBuffer != NULL);
    return DtPipe_SetSharedBuffer(pPipe, pBuffer, BufferSize);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdGetRxBuffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfNwOnCmdSetSharedKBuffer(const DtIoStubDfNw* pStub, DtPipe* pPipe,
                                       const DtIoctlPipeCmdSetSharedKBufferInput* pInData, 
                                       DtIoctlPipeCmdSetSharedKBufferOutput* pOutData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL && pOutData != NULL);
    return DtPipe_SetSharedBufferKernel(pPipe, pInData->m_RequestedBufferSize, 
                                      &pOutData->m_ActualBufferSize, &pOutData->m_MaxLoad, 
                                      (UInt8**)&pOutData->m_pBufferAddr);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdReleaseSharedBuffer -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfNwOnCmdReleaseSharedBuffer(const DtIoStubDfNw* pStub, DtPipe* pPipe)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    if ((pPipe->m_Capabilities & (DT_PIPE_CAP_SWP | DT_PIPE_CAP_RT)) == 
                                                       (DT_PIPE_CAP_SWP | DT_PIPE_CAP_RT))
    {
        DtDfNw* pDf = (DtDfNw*)((DtIoStubDf*)pStub)->m_pDf;
        DtStatus Status;
        if ((pPipe->m_Capabilities & DT_PIPE_CAP_TX) == DT_PIPE_CAP_TX)
        {
            // Take the DtDfNw_TxRtHwqThread mutex to be shure the thread is not using
            // this channel
            DtFastMutexAcquire(&pDf->m_PipeTxRtSwpListFMutex);
            Status = DtPipe_ReleaseSharedBuffer(pPipe);
            DtFastMutexRelease(&pDf->m_PipeTxRtSwpListFMutex);
            return Status;
        }
        else 
        {
            // Take the DtDfNw_RxHwqThread mutex to be shure the thread is not using
            // this channel
            DtFastMutexAcquire(&pDf->m_PipeRxRtSwpListFMutex);
            Status = DtPipe_ReleaseSharedBuffer(pPipe);
            DtFastMutexRelease(&pDf->m_PipeRxRtSwpListFMutex);
            return Status;
        }
    }
    return DtPipe_ReleaseSharedBuffer(pPipe);

}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdGetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfNwOnCmdGetProperties(const DtIoStubDfNw* pStub, DtPipe* pPipe, 
                                              DtIoctlPipeCmdGetPropertiesOutput* pOutData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    return DtPipe_GetProperties(pPipe, &pOutData->m_Capabilities, 
            &pOutData->m_PrefetchSize, &pOutData->m_PipeDataWidth, &pOutData->m_PipeType);

}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdGetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfNwOnCmdGetStatus(const DtIoStubDfNw* pStub, DtPipe* pPipe, 
                                                  DtIoctlPipeCmdGetStatusOutput* pOutData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    return DtPipe_GetStatus(pPipe, &pOutData->m_OpStatus, &pOutData->m_StatusFlags, 
                                                                 &pOutData->m_ErrorFlags);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdIssuePipeFlush -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfNwOnCmdIssuePipeFlush(const DtIoStubDfNw* pStub, DtPipe* pPipe)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    return DtPipe_IssuePipeFlush(pPipe);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdGetRxWriteOffset -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfNwOnCmdGetRxWriteOffset(const DtIoStubDfNw* pStub, DtPipe* pPipe,
                                           DtIoctlPipeCmdGetRxWriteOffsetOutput* pOutData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    return DtPipe_GetRxWriteOffset(pPipe, &pOutData->m_RxWriteOffset);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdSetRxReadOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfNwOnCmdSetRxReadOffset(const DtIoStubDfNw* pStub, DtPipe* pPipe,
                                        const DtIoctlPipeCmdSetRxReadOffsetInput* pInData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    return DtPipe_SetRxReadOffset(pPipe, pInData->m_RxReadOffset);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdGetTxReadOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfNwOnCmdGetTxReadOffset(const DtIoStubDfNw* pStub, DtPipe* pPipe,
                                            DtIoctlPipeCmdGetTxReadOffsetOutput* pOutData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    return DtPipe_GetTxReadOffset(pPipe, &pOutData->m_TxReadOffset);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdSetTxWriteOffset -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfNwOnCmdSetTxWriteOffset(const DtIoStubDfNw* pStub, DtPipe* pPipe,
                                       const DtIoctlPipeCmdSetTxWriteOffsetInput* pInData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtPipe_SetTxWriteOffset(pPipe, pInData->m_TxWriteOffset);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdRegisterEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfNwOnCmdRegisterEvent(const DtIoStubDfNw* pStub, DtPipe* pPipe,
                                          const DtIoctlPipeCmdRegisterEventInput* pInData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    return DtDfNw_RegisterEvent(pPipe, (DtTrigEvtDriver*)&pInData->m_TrigEvt, 
                                                                        pInData->m_Count);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdDeregisterEvent -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfNwOnCmdDeregisterEvent(const DtIoStubDfNw* pStub, DtPipe* pPipe,
                                        const DtIoctlPipeCmdDeregisterEventInput* pInData)
{
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    return DtDfNw_DeregisterEvent(pPipe, (DtTrigEvtDriver*)&pInData->m_TrigEvt);
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfNwOnCmdSetIpFilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfNwOnCmdSetIpFilter(const DtIoStubDfNw* pStub, DtPipe* pPipe,
                                            const DtIoctlPipeCmdSetIpFilterInput* pInData)
{
    DtStatus Status = DT_STATUS_OK;
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    NW_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    Status = DtPipe_SetIpFilter(pPipe, (UInt8*)pInData->m_DstIp, 
                                    (UInt16*)pInData->m_DstPort,
                                    (UInt8*)pInData->m_SrcIp, (UInt16*)pInData->m_SrcPort,
                                    (Int*)pInData->m_VlanId, (Int)pInData->m_Flags);
    if (!DT_SUCCESS(Status))
        return Status;
    return DtDfNw_CheckFilterDuplication(STUB_DF);
}
