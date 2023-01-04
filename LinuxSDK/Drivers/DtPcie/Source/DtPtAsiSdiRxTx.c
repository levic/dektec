// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPtAsiSdiRxTx.c *#*#*#*#*#*#*# (C) 2018-2022 DekTec
//
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2022 DekTec Digital Video B.V.
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
#include "DtPtAsiSdiRxTx.h"
#include "IoConfigCodes.h"
#include "DtAudioVideo.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtPtAsiSdiRxTx implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=



// MACRO to enable/disable a driver function; it returns on error
#define ENABLE_DRIVERFUNC_RETURN_ON_ERR(FUNC, ENABLE)                       \
    do {                                                                    \
        if (FUNC != NULL)                                                   \
        {                                                                   \
            DtStatus  dr = FUNC->m_EnableFunc((DtDf*)FUNC, ENABLE);         \
            if (!DT_SUCCESS(dr))                                            \
                return dr;                                                  \
        }                                                                   \
    } while (0)


// MACRO to enable/disable a block controller; it returns on error
#define ENABLE_BLOCKCTRL_RETURN_ON_ERR(BLOCK, ENABLE)                       \
    do {                                                                    \
        if (BLOCK != NULL)                                                  \
        {                                                                   \
            DtStatus  dr = BLOCK->m_EnableFunc((DtBc*)BLOCK, ENABLE);       \
            if (!DT_SUCCESS(dr))                                            \
                return dr;                                                  \
        }                                                                   \
    } while (0)

// MACRO with default precondition checks for the DfAsiTx function
#define PT_ASISDIRXTX_DEFAULT_PRECONDITIONS(pPt)      \
    DT_ASSERT(pPt!=NULL && pPt->m_Size==sizeof(DtPtAsiSdiRxTx))


// Helper macro to cast a DtDf* to aDtPtAsiSdiRxTx*
#define PT_ASISDIRXTX      ((DtPtAsiSdiRxTx*)pPt)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus DtPtAsiSdiRxTx_Init(DtPt*);
static DtStatus DtPtAsiSdiRxTx_CheckRxPrerequisites(DtPtAsiSdiRxTx*);
static DtStatus DtPtAsiSdiRxTx_CheckTxPrerequisites(DtPtAsiSdiRxTx*);
static DtStatus DtPtAsiSdiRxTx_OnCloseFile(DtPt*, DtFileObject*);
static DtStatus DtPtAsiSdiRxTx_Enable(DtPt*, Bool Enable);
static DtStatus DtPtAsiSdiRxTx_SetIoConfig(DtPt*, const DtCfIoConfigValue*, Int Num);
static DtStatus DtPtAsiSdiRxTx_SetIoConfigPrepare(DtPt*, const DtExclAccessObject*);
static DtStatus DtPtAsiSdiRxTx_SetIoConfigFinish(DtPt*, const DtExclAccessObject*);

static DtStatus DtPtAsiSdiRxTx_SetIoConfigIoDir(DtPtAsiSdiRxTx*, 
                                                                const DtCfIoConfigValue*);
static DtStatus DtPtAsiSdiRxTx_SetIoConfigTodRefSel(DtPtAsiSdiRxTx*, 
                                                                const DtCfIoConfigValue*);
static DtStatus DtPtAsiSdiRxTx_SetRxIoConfigIoStd(DtPtAsiSdiRxTx*, 
                                                                const DtCfIoConfigValue*);
static DtStatus DtPtAsiSdiRxTx_SetRxIoConfigDmaTestMode(DtPtAsiSdiRxTx*, 
                                                                const DtCfIoConfigValue*);
static DtStatus DtPtAsiSdiRxTx_SetRxIoConfigFailSafe(DtPtAsiSdiRxTx*, 
                                                                const DtCfIoConfigValue*);
static DtStatus DtPtAsiSdiRxTx_SetRxIoConfigGenLock(DtPtAsiSdiRxTx*, 
                                                                const DtCfIoConfigValue*);
static DtStatus DtPtAsiSdiRxTx_SetRxIoConfigIoDownscale(DtPtAsiSdiRxTx*, 
                                                                const DtCfIoConfigValue*);
static DtStatus DtPtAsiSdiRxTx_SetTxIoConfigIoStd(DtPtAsiSdiRxTx*, 
                                                                const DtCfIoConfigValue*);
static DtStatus DtPtAsiSdiRxTx_SetTxIoConfigIoStdDblBuf(DtPtAsiSdiRxTx*, 
                                                                const DtCfIoConfigValue*);
static DtStatus DtPtAsiSdiRxTx_SetTxIoConfigDmaTestMode(DtPtAsiSdiRxTx*, 
                                                                const DtCfIoConfigValue*);
static DtStatus DtPtAsiSdiRxTx_SetTxIoConfigFailSafe(DtPtAsiSdiRxTx*, 
                                                                const DtCfIoConfigValue*);
static DtStatus DtPtAsiSdiRxTx_SetTxIoConfigGenLock(DtPtAsiSdiRxTx*, 
                                                                const DtCfIoConfigValue*);
static DtStatus DtPtAsiSdiRxTx_SetTxIoConfigIoDownscale(DtPtAsiSdiRxTx*, 
                                                                const DtCfIoConfigValue*);
#ifdef LINBUILD
static DtStatus DtPtAsiSdiRxTx_Mmap(DtPt*, const DtFileObject*, DtVma*);
#endif // #ifdef LINBUILD

//=+=+=+=+=+=+=+=+=+=+=+=+=+DtPtAsiSdiRxTx - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+
// 
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtPtAsiSdiRxTx_Close(DtPt*  pPt)
{
    PT_ASISDIRXTX_DEFAULT_PRECONDITIONS(pPt);

    // Let base function perform final clean-up
    DtPt_Close(pPt);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtPtAsiSdiRxTx* DtPtAsiSdiRxTx_Open(DtCore* pCore, Int PortIndex, DtPortType Type)
{
    DtPtOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(PortIndex >= 0);

    DT_ASSERT(Type==DT_PORT_TYPE_ASISDIRX ||Type==DT_PORT_TYPE_ASISDITX 
                                                         ||Type==DT_PORT_TYPE_ASISDIRXTX);

    // Init open parameters
    DT_PT_INIT_OPEN_PARAMS(OpenParams,DtPtAsiSdiRxTx, DT_DF_ASISDIRXTX_NAME,
                                                                  PortIndex, Type, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtPtAsiSdiRxTx_Close;
    OpenParams.m_InitFunc = DtPtAsiSdiRxTx_Init;
    OpenParams.m_EnableFunc = DtPtAsiSdiRxTx_Enable;
    OpenParams.m_OnCloseFileFunc = DtPtAsiSdiRxTx_OnCloseFile;
    OpenParams.m_SetIoConfigFunc = DtPtAsiSdiRxTx_SetIoConfig;
    OpenParams.m_SetIoConfigPrepareFunc = DtPtAsiSdiRxTx_SetIoConfigPrepare;
    OpenParams.m_SetIoConfigFinishFunc = DtPtAsiSdiRxTx_SetIoConfigFinish;
#ifdef LINBUILD
    OpenParams.m_MmapFunc = DtPtAsiSdiRxTx_Mmap;
#endif

    // Use base function to allocate and perform standard initialisation of function data
    return (DtPtAsiSdiRxTx*)DtPt_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtAsiSdiRxTx_Init(DtPt* pPtBase)
{
   DtPtAsiSdiRxTx* pPt = (DtPtAsiSdiRxTx*)pPtBase;

    const char* FrontEndTxRoleName = "TX_FRONT_END";
    const char* BackEndTxRoleName =  "TX_BACK_END";
    const char* TestModeTxRoleName = "TX_TEST_MODE";
    const char* SdiTxDmxOutRoleName = "SDI_DEMUX_OUT";
    const char* SdiTxDmxInRoleName = "SDI_DEMUX_IN";
    const char* FrontEndRxRoleName = "RX_FRONT_END";
    const char* BackEndRxRoleName =  "RX_BACK_END";
    const char* TestModeRxRoleName = "RX_TEST_MODE";
    const char* SdiRxMuxOutRoleName = "SDI_MUX_OUT";
    const char* SdiRxMuxInRoleName = "SDI_MUX_IN";
    const char* SdiTxFrom4LinkMaster ="FROM_QUAD_LINK_MASTER";
    const char* SdiRxTo4LinkMaster ="TO_QUAD_LINK_MASTER";
    const char* TxDblBufRoleName = "TX_DBL_BUF";
 
    // Sanity check
    PT_ASISDIRXTX_DEFAULT_PRECONDITIONS(pPt);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Find the TX-driver functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
    pPt->m_pDfSdiTxPhy = (DtDfSdiTxPhy*)DtPt_FindDf(pPtBase, 
                                                           DT_FUNC_TYPE_SDITXPHY, NULL);

    // Find the TX-block controllers
    pPt->m_pBcAsiTxG = (DtBcASITXG*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_ASITXG, NULL);
    pPt->m_pBcAsiTxSer = (DtBcASITXSER*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_ASITXSER, 
                                                                                    NULL);
    pPt->m_pBcSwitchFrontEndTx = (DtBcSWITCH*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SWITCH,
                                                                      FrontEndTxRoleName);
    pPt->m_pBcSwitchBackEndTx = (DtBcSWITCH*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SWITCH,
                                                                       BackEndTxRoleName);
    pPt->m_pBcSwitchTestModeTx = (DtBcSWITCH*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SWITCH, 
                                                                      TestModeTxRoleName);
    pPt->m_pBcSdiTxF = (DtBcSDITXF*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SDITXF, NULL);
    pPt->m_pBcSdiTxP = (DtBcSDITXP*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SDITXP, NULL);
    pPt->m_pBcConstSink = (DtBcCONSTSINK*)DtPt_FindBc(pPtBase, 
                                                           DT_BLOCK_TYPE_CONSTSINK, NULL);

    //.-.-.-.-.-.-.-.-.-.-.-.- Find optional TX-block controllers -.-.-.-.-.-.-.-.-.-.-.-.
    pPt->m_pBcKeepAlive = (DtBcKA*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_KA, NULL);
    pPt->m_pBcGs2988 = (DtBcGS2988*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_GS2988, NULL);
    // 12G TX
    pPt->m_pBcSwitchSdiTxDmxOut = (DtBcSWITCH*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SWITCH,
                                                                     SdiTxDmxOutRoleName);
    pPt->m_pBcSwitchSdiTxDmxIn = (DtBcSWITCH*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SWITCH,
                                                                      SdiTxDmxInRoleName);
    pPt->m_pBcSdiTxDmx12G = (DtBcSDIDMX12G*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SDIDMX12G,
                                                                                    NULL);
    // 3G-TX QuadLink 
    pPt->m_pBcSwitchSdiTxFrom4LinkMaster = (DtBcSWITCH*)DtPt_FindBc(pPtBase, 
                                              DT_BLOCK_TYPE_SWITCH, SdiTxFrom4LinkMaster);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Find the RX-driver functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
    pPt->m_pDfAsiRx = (DtDfAsiRx*)DtPt_FindDf(pPtBase, DT_FUNC_TYPE_ASIRX, NULL);
    pPt->m_pDfSdiRx = (DtDfSdiRx*)DtPt_FindDf(pPtBase, DT_FUNC_TYPE_SDIRX, NULL);
    pPt->m_pDfChSdiRx = (DtDfChSdiRx*)DtPt_FindDf(pPtBase, DT_FUNC_TYPE_CHSDIRX, NULL);

    //.-.-.-.-.-.-.-.-.-.-.-.-.- Find the RX-block controllers -.-.-.-.-.-.-.-.-.-.-.-.-.-
    pPt->m_pBcSwitchFrontEndRx = (DtBcSWITCH*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SWITCH,
                                                                      FrontEndRxRoleName);
    pPt->m_pBcSwitchBackEndRx = (DtBcSWITCH*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SWITCH, 
                                                                       BackEndRxRoleName);
    pPt->m_pBcSwitchTestModeRx = (DtBcSWITCH*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SWITCH, 
                                                                      TestModeRxRoleName);
    pPt->m_pBcSdiRxF = (DtBcSDIRXF*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SDIRXF, NULL);

    pPt->m_pBcConstSource = (DtBcCONSTSOURCE*)DtPt_FindBc(pPtBase,
                                                         DT_BLOCK_TYPE_CONSTSOURCE, NULL);
    //.-.-.-.-.-.-.-.-.-.-.-.- Find optional RX-block controllers -.-.-.-.-.-.-.-.-.-.-.-.
    // 12G RX
    pPt->m_pBcSwitchSdiRxMuxIn = (DtBcSWITCH*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SWITCH,
                                                                      SdiRxMuxInRoleName);
    pPt->m_pBcSwitchSdiRxMuxOut = (DtBcSWITCH*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SWITCH,
                                                                     SdiRxMuxOutRoleName);
    pPt->m_pBcSdiRxMux12G = (DtBcSDIMUX12G*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SDIMUX12G,
                                                                                    NULL);
    pPt->m_pBcSdiRxSt425Lr = (DtBcST425LR*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_ST425LR,
                                                                                    NULL);
    // 3G-RX QuadLink 
    pPt->m_pBcSwitchSdiRxTo4LinkMaster = (DtBcSWITCH*)DtPt_FindBc(pPtBase, 
                                                DT_BLOCK_TYPE_SWITCH, SdiRxTo4LinkMaster);
    //-.-.-.-.-.-.-.-.-.-.-.-.- Find the RX/TX-block controllers -.-.-.-.-.-.-.-.-.-.-.-.-
    pPt->m_pBcCDmaC = (DtBcCDMAC*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_CDMAC, NULL);
    pPt->m_pBcBURSTFIFO = (DtBcBURSTFIFO*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_BURSTFIFO,
                                                                                    NULL);

    // Find the RX/TX driver functions
    pPt->m_pDfSpiCableDrvEq = (DtDfSpiCableDrvEq*)DtPt_FindDf(pPtBase, 
                                                        DT_FUNC_TYPE_SPICABLEDRVEQ, NULL);

    // Find the double-buffered switch
    pPt->m_pBcSwitchTxDblBuf = (DtBcSWITCH*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SWITCH, 
                                                                        TxDblBufRoleName);

    // Find the TimeOfDay clock control
    if (DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_TODREF_INTERNAL))
        pPt->m_pDfTodClockCtrl = (DtDfTodClockCtrl*)DtCore_DF_Find(pPt->m_pCore, NULL,
                                                           DT_FUNC_TYPE_TODCLKCTRL, NULL);
    else
        pPt->m_pDfTodClockCtrl = NULL;

    // Check the driver functions and blockcontrollers that were found
    DT_RETURN_ON_ERROR(DtPtAsiSdiRxTx_CheckRxPrerequisites(pPt));
    DT_RETURN_ON_ERROR(DtPtAsiSdiRxTx_CheckTxPrerequisites(pPt));

    // Clear lock state flags
    pPt->m_HoldExclAccessLock = pPt->m_HoldChannelLock = FALSE;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_CheckRxPrerequisites -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtAsiSdiRxTx_CheckRxPrerequisites(DtPtAsiSdiRxTx* pPt)
{
   Bool AsiCap = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_ASI);
   Bool SdiCap = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_SDI);
   Bool InpCap = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_INPUT);
   Bool RateTestCap = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_DMATESTMODE);
   Bool TodClkCtrlCap = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_TODREF_INTERNAL) 
                        || DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_TODREF_STEADYCLOCK); 
   if (!InpCap)
       return DT_STATUS_OK;

    // CDMAC is always required
    if (pPt->m_pBcCDmaC == NULL)
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: CMDAC not found");
        return DT_STATUS_FAIL;
    }
    // BURSTFIFO is always required
    if (pPt->m_pBcBURSTFIFO == NULL)
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: BURSTFIFO not found");
        return DT_STATUS_FAIL;
    }
    // For ASI the DfAsiRx needs to be present
    if (AsiCap && pPt->m_pDfAsiRx==NULL)
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: ASIRX not found");
        return DT_STATUS_FAIL;
    }

    // For ASI and SDI the DfSdiRx  needs to be present
    if (pPt->m_pDfSdiRx == NULL)
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: SDIRX not found");
        return DT_STATUS_FAIL;
    }

    // For SDI the DfSdiRxChan needs to be present
    if (SdiCap && pPt->m_pDfChSdiRx==NULL)
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: CHSDIRX not found");
        return DT_STATUS_FAIL;
    }

    // In case of both ASI and SDI, input switches are required
    if ((AsiCap && SdiCap) && (pPt->m_pBcSwitchFrontEndRx==NULL
                                                      || pPt->m_pBcSwitchBackEndRx==NULL))
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: SWITCH FE/BE RX not found");
        return DT_STATUS_FAIL;
    }

    // For SDI the SDIRXF block controller needs to be present
    if (SdiCap && pPt->m_pBcSdiRxF==NULL)
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: SDIRXF not found");
        return DT_STATUS_FAIL;
    }

    // DMA-rate test requires constant rate source and RX test mode switch
    if (RateTestCap && (pPt->m_pBcConstSource==NULL || pPt->m_pBcSwitchTestModeRx==NULL))
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: CONSTSOURCE/SWITCH not found");
        return DT_STATUS_FAIL;
    }

    // Check TimeOfDay clock control
    if (TodClkCtrlCap && pPt->m_pDfTodClockCtrl==NULL)
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: TODCLKCTRL not found");
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_CheckTxPrerequisites -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtAsiSdiRxTx_CheckTxPrerequisites(DtPtAsiSdiRxTx* pPt)
{
   Bool AsiCap  = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_ASI);
   Bool SdiCap  = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_SDI);
   Bool OutpCap  = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_OUTPUT);
   Bool FailSafeCap = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_FAILSAFE);
   Bool RateTestCap  = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_DMATESTMODE);
   Bool DblBufCap  = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_DBLBUF);
   Bool TodClkCtrlCap = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_TODREF_INTERNAL) 
                        || DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_TODREF_STEADYCLOCK); 

   if (!OutpCap)
       return DT_STATUS_OK;

    // CDMAC is always required
    if (pPt->m_pBcCDmaC == NULL)
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: CMDAC not found");
        return DT_STATUS_FAIL;
    }

    // BURSTFIFO is always required
    if (pPt->m_pBcBURSTFIFO == NULL)
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: BURSTFIFO not found");
        return DT_STATUS_FAIL;
    }
    // For SDI a TXPHY needs to be present
    if (SdiCap && pPt->m_pDfSdiTxPhy==NULL)
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: SDITXPHY not found");
        return DT_STATUS_FAIL;
    }
    
    // SDI requires a SDI formatter and protocol block
    if (SdiCap && (pPt->m_pBcSdiTxF==NULL || pPt->m_pBcSdiTxP==NULL))
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: SDITXF/SDITXP not found");
        return DT_STATUS_FAIL;
    }
     
    // For ASI a TXPHY or serializer needs to be present
    if (AsiCap && pPt->m_pDfSdiTxPhy==NULL && pPt->m_pBcAsiTxSer==NULL)
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: SDITXPHY/ASITXSER not found");
        return DT_STATUS_FAIL;
    }
    
    // ASI requires a ASI-TX-Gate
    if (AsiCap && pPt->m_pBcAsiTxG==NULL)
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: ASITXG not found");
        return DT_STATUS_FAIL;
    }

    // In case of ASI/SDI output switches are required
    if ((AsiCap && SdiCap) && (pPt->m_pBcSwitchFrontEndTx==NULL 
                                                      || pPt->m_pBcSwitchBackEndTx==NULL))
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: SWITCH FE/BE TX not found");
        return DT_STATUS_FAIL;
    }

    // Failsafe requires a Keep-alive block
    if  (FailSafeCap && pPt->m_pBcKeepAlive==NULL)
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: KEEPALIVE not found");
        return DT_STATUS_FAIL;
    }

    // DMA-rate test requires constant rate sink and TX test mode switch
    if (RateTestCap && (pPt->m_pBcConstSink==NULL || pPt->m_pBcSwitchTestModeTx==NULL))
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: CONSTSOURCE/SWITCH not found");
        return DT_STATUS_FAIL;
    }
    
    // In case of double-buffered output a buddy port selector switch is required
    if (DblBufCap && pPt->m_pBcSwitchTxDblBuf==NULL)
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: double-buffer switch not found");
        return DT_STATUS_FAIL;
    }

    // Check TimeOfDay clock control
    if (TodClkCtrlCap && pPt->m_pDfTodClockCtrl==NULL)
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: TODCLKCTRL not found");
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_Enable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Overrides the default implementation.
//
DtStatus DtPtAsiSdiRxTx_Enable(DtPt* pPt, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;

    PT_ASISDIRXTX_DEFAULT_PRECONDITIONS(pPt);

    // Will the state change??
    if ((pPt->m_OpState==PT_STATE_ENABLED && Enable) 
                                     || (pPt->m_OpState==PT_STATE_INITIALISED && !Enable))
        return DT_STATUS_OK;

    if (Enable)
    {
        // Wait for the set IO-config 
    }
    else
    {
        // Let the base disable the children
        Status = DtPt_EnableChildren(pPt, Enable);
    }

    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPt(ERR, COMMON, pPt, "ERROR: failed to enable/disbale children");
        return Status;
    }


    // Set new state
    Status = DtPt_SetOpState(pPt, Enable ? PT_STATE_ENABLED : PT_STATE_INITIALISED);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtAsiSdiRxTx_OnCloseFile(DtPt* pPtBase, DtFileObject* pObject)
{
     DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    PT_ASISDIRXTX_DEFAULT_PRECONDITIONS(pPtBase);

     // Let the base do the work
     Status = DtPt_OnCloseFile(pPtBase, pObject);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetIoConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtAsiSdiRxTx_SetIoConfig(DtPt* pPtBase, const DtCfIoConfigValue* pIoCfgs, 
                                                                            Int NumIoCfgs)
{
    Int  IoCfg=0;
    DtPtAsiSdiRxTx* pPt = (DtPtAsiSdiRxTx*)pPtBase;
    Bool IsInput = FALSE;
    Bool IsDblBufOutput = FALSE;

    // Sanity check
    PT_ASISDIRXTX_DEFAULT_PRECONDITIONS(pPt);


    // Check whether expected IO-configs are there
    for (IoCfg=0; IoCfg<NumIoCfgs; IoCfg++)
    {
        switch (IoCfg)
        {
        case DT_IOCONFIG_IODIR:
        case DT_IOCONFIG_IOSTD:
            // Mandatory IO-configs
            if (pIoCfgs[IoCfg].m_Value == DT_IOCONFIG_NONE)
            {
                Char IoCfgName[IOCONFIG_NAME_MAX_SIZE];
                IoCfgName[0] = '\0';
                IoConfigNameGet(IoCfg, IoCfgName, sizeof(IoCfgName));
                DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: Missing IO-Config: %s",
                                                                               IoCfgName);
                DT_ASSERT(FALSE);
                return DT_STATUS_INVALID_PARAMETER;
            }
            break;
        case DT_IOCONFIG_DMATESTMODE:
        case DT_IOCONFIG_FAILSAFE:
        case DT_IOCONFIG_GENLOCKED:
        case DT_IOCONFIG_IODOWNSCALE:
        case DT_IOCONFIG_TODREFSEL:
            // Optional IO-configs; will be tested later
            break;

        default:
            // Unsupported IO-configs
            if (pIoCfgs[IoCfg].m_Value != DT_IOCONFIG_NONE)
            {
                Char IoCfgName[IOCONFIG_NAME_MAX_SIZE];
                IoCfgName[0] = '\0';
                IoConfigNameGet(IoCfg, IoCfgName, sizeof(IoCfgName));
                DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR:Unsupported IO-Config: %s",
                                                                               IoCfgName);
                DT_ASSERT(FALSE);
                return DT_STATUS_NOT_SUPPORTED;
            }
            break;
        }
    }

    // Perform IO-Config setting IODIR
    if (DT_IOCONFIG_IODIR >= NumIoCfgs)
        return DT_STATUS_INVALID_PARAMETER;
    DT_RETURN_ON_ERROR(DtPtAsiSdiRxTx_SetIoConfigIoDir(pPt, &pIoCfgs[DT_IOCONFIG_IODIR]));

    // Determine direction; input or output
    DT_ASSERT(pIoCfgs[DT_IOCONFIG_IODIR].m_Value==DT_IOCONFIG_INPUT
                               || pIoCfgs[DT_IOCONFIG_IODIR].m_Value==DT_IOCONFIG_OUTPUT);
    IsInput = (pIoCfgs[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_INPUT);
    IsDblBufOutput = (pIoCfgs[DT_IOCONFIG_IODIR].m_Value==DT_IOCONFIG_OUTPUT && 
                               pIoCfgs[DT_IOCONFIG_IODIR].m_SubValue==DT_IOCONFIG_DBLBUF);


    // Perform IO-Config setting IOSTD. Dependent on direction
    if (DT_IOCONFIG_IOSTD >= NumIoCfgs)
        return DT_STATUS_INVALID_PARAMETER;
    if (IsInput)
        DT_RETURN_ON_ERROR(DtPtAsiSdiRxTx_SetRxIoConfigIoStd(pPt, 
                                                            &pIoCfgs[DT_IOCONFIG_IOSTD]));
    else if (!IsDblBufOutput)
        DT_RETURN_ON_ERROR(DtPtAsiSdiRxTx_SetTxIoConfigIoStd(pPt,
                                                            &pIoCfgs[DT_IOCONFIG_IOSTD]));
    else
        DT_RETURN_ON_ERROR(DtPtAsiSdiRxTx_SetTxIoConfigIoStdDblBuf(pPt,
                                                            &pIoCfgs[DT_IOCONFIG_IOSTD]));

    // Perform IO-Config setting DMATESTMODE; dependent on direction
    if (DT_IOCONFIG_DMATESTMODE >= NumIoCfgs)
        return DT_STATUS_INVALID_PARAMETER;
    if (IsInput)
        DT_RETURN_ON_ERROR(DtPtAsiSdiRxTx_SetRxIoConfigDmaTestMode(pPt, 
                                                      &pIoCfgs[DT_IOCONFIG_DMATESTMODE]));
    else if (!IsDblBufOutput)
        DT_RETURN_ON_ERROR(DtPtAsiSdiRxTx_SetTxIoConfigDmaTestMode(pPt, 
                                                      &pIoCfgs[DT_IOCONFIG_DMATESTMODE]));

    // Perform IO-Config setting FAILSAFE
    if (DT_IOCONFIG_FAILSAFE >= NumIoCfgs)
        return DT_STATUS_INVALID_PARAMETER;
    if (IsInput)
        DT_RETURN_ON_ERROR(DtPtAsiSdiRxTx_SetRxIoConfigFailSafe(pPt, 
                                                         &pIoCfgs[DT_IOCONFIG_FAILSAFE]));
    else if (!IsDblBufOutput)
        DT_RETURN_ON_ERROR(DtPtAsiSdiRxTx_SetTxIoConfigFailSafe(pPt, 
                                                         &pIoCfgs[DT_IOCONFIG_FAILSAFE]));
    // Perform IO-Config setting GENLOCKED
    if (DT_IOCONFIG_GENLOCKED >= NumIoCfgs)
        return DT_STATUS_INVALID_PARAMETER;
    if (IsInput)
        DT_RETURN_ON_ERROR(DtPtAsiSdiRxTx_SetRxIoConfigGenLock(pPt, 
                                                        &pIoCfgs[DT_IOCONFIG_GENLOCKED]));
    else if (!IsDblBufOutput)
        DT_RETURN_ON_ERROR(DtPtAsiSdiRxTx_SetTxIoConfigGenLock(pPt, 
                                                        &pIoCfgs[DT_IOCONFIG_GENLOCKED]));

    // Perform IO-Config setting IO-Downscale
    if (DT_IOCONFIG_IODOWNSCALE >= NumIoCfgs)
        return DT_STATUS_INVALID_PARAMETER;
    if (IsInput)
        DT_RETURN_ON_ERROR(DtPtAsiSdiRxTx_SetRxIoConfigIoDownscale(pPt, 
                                                      &pIoCfgs[DT_IOCONFIG_IODOWNSCALE]));
    else 
        DT_RETURN_ON_ERROR(DtPtAsiSdiRxTx_SetTxIoConfigIoDownscale(pPt, 
                                                      &pIoCfgs[DT_IOCONFIG_IODOWNSCALE]));

    // Perform TimeOfDay reference clock selection
    if (DT_IOCONFIG_TODREFSEL >= NumIoCfgs)
        return DT_STATUS_INVALID_PARAMETER;
    DT_RETURN_ON_ERROR(DtPtAsiSdiRxTx_SetIoConfigTodRefSel(pPt, 
                                                        &pIoCfgs[DT_IOCONFIG_TODREFSEL]));

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetIoConfigIoDir -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtAsiSdiRxTx_SetIoConfigIoDir(DtPtAsiSdiRxTx* pPt,
                                                          const DtCfIoConfigValue* pIoCfg)
{
   Bool InpCap  = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_INPUT);
   Bool OutpCap  = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_OUTPUT);
   
    DT_ASSERT(pIoCfg != NULL);
    DT_ASSERT((OutpCap && pIoCfg->m_Value==DT_IOCONFIG_OUTPUT)
           || (InpCap && pIoCfg->m_Value==DT_IOCONFIG_INPUT 
                                               && pIoCfg->m_SubValue==DT_IOCONFIG_INPUT));

    if (pIoCfg->m_Value==DT_IOCONFIG_OUTPUT && pIoCfg->m_SubValue==DT_IOCONFIG_DBLBUF)
    {
        // Configured for double-buffered output
        // Determine buddy port and double-buffer switch position
        Int  PortIndex = pPt->m_PortIndex+1;
        Int  Buddy = (Int)pIoCfg->m_ParXtra[0];
        Int  DblBufSelect = Buddy%4;
        if (Buddy < PortIndex)
            DblBufSelect++;
        DT_ASSERT(DblBufSelect>0 && DblBufSelect<=3);

        if (InpCap)
        { 
            // Disable all available RX driver-functions/blocks
            ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfChSdiRx, FALSE);
            ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfAsiRx, FALSE);
            ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSdiRx, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchFrontEndRx, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchBackEndRx, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchTestModeRx, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcConstSource, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiRxF, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiRxMuxOut, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiRxMux12G, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiRxSt425Lr, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiRxMuxIn, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiRxTo4LinkMaster, FALSE);
        }
        // Disable all TX-driver functions/blocks that are not used in double buffered
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcAsiTxG, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcAsiTxSer, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchFrontEndTx, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchBackEndTx, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchTestModeTx, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcConstSink, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxF, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxP, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiTxDmxOut, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxDmx12G, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiTxDmxIn, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiTxFrom4LinkMaster, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcKeepAlive, FALSE);

        // Enable and set double buffered switch position
        DT_ASSERT(pPt->m_pBcSwitchTxDblBuf != NULL);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchTxDblBuf, TRUE);
        DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchTxDblBuf,
                                                                        DblBufSelect, 0));
        DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchTxDblBuf,
                                                                    DT_BLOCK_OPMODE_RUN));

        // Enable all other TX-driver functions/blocks that are used in double buffered
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcGs2988, TRUE);
        ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSpiCableDrvEq, TRUE);
        if (pPt->m_pDfSpiCableDrvEq != NULL)
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetDirection(pPt->m_pDfSpiCableDrvEq,
                                                             DT_DF_SPICABLEDRVEQ_DIR_TX));
        ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSdiTxPhy, TRUE);
    }
    else if (pIoCfg->m_Value == DT_IOCONFIG_OUTPUT)
    {
        // Configured for output
        if (InpCap)
        { 
            // Disable all available RX driver-functions/blocks
            ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfChSdiRx, FALSE);
            ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfAsiRx, FALSE);
            ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSdiRx, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchFrontEndRx, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchBackEndRx, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchTestModeRx, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcConstSource, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiRxF, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiRxMuxOut, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiRxMux12G, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiRxSt425Lr, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiRxMuxIn, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiRxTo4LinkMaster, FALSE);
        }

        // Enable TX-driver functions/blocks that do not dependent on IOSTD or other
        // IO configs
        ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSpiCableDrvEq, TRUE);
        if (pPt->m_pDfSpiCableDrvEq != NULL)
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetDirection(pPt->m_pDfSpiCableDrvEq, 
                                                             DT_DF_SPICABLEDRVEQ_DIR_TX));
        ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSdiTxPhy, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchFrontEndTx, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchBackEndTx, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchTestModeTx, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcCDmaC, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcBURSTFIFO, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcKeepAlive, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcGs2988, TRUE);
        if (pPt->m_pBcSwitchTxDblBuf != NULL)
        { 
            // Pass own channel and enable double-buffer switch
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchTxDblBuf, TRUE);
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchTxDblBuf, 0, 0));
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchTxDblBuf,
                                                                    DT_BLOCK_OPMODE_RUN));
        }
        // Set direction
        DT_RETURN_ON_ERROR(DtBcBURSTFIFO_SetDirection(pPt->m_pBcBURSTFIFO, 
                                                                    DT_BURSTFIFO_DIR_TX));

    }
    else if (pIoCfg->m_Value == DT_IOCONFIG_INPUT)
    {
        // Configured for input
        if (OutpCap)
        { 
            // Disable all available TX driver-functions/blocks
            ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSdiTxPhy, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcAsiTxG, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcAsiTxSer, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchFrontEndTx, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchBackEndTx, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchTestModeTx, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcConstSink, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxF, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxP, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiTxDmxOut, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxDmx12G, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiTxDmxIn, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiTxFrom4LinkMaster, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcKeepAlive, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcGs2988, FALSE);
            if (pPt->m_pBcSwitchTxDblBuf != NULL)
            { 
                // Pass own channel and disable double-buffer switch
                ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchTxDblBuf, TRUE);
                DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchTxDblBuf, 0,0));
                ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchTxDblBuf, FALSE);
            }
        }

        // Enable RX-driver functions/blocks that do not dependent on IOSTD or other
        // IO configs
        // First enable the cable driver!! The DfSdiRx is using it!!
        ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSpiCableDrvEq, TRUE);
        if (pPt->m_pDfSpiCableDrvEq != NULL)
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetDirection(pPt->m_pDfSpiCableDrvEq, 
                                                             DT_DF_SPICABLEDRVEQ_DIR_RX));
        ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSdiRx, TRUE);
        ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfChSdiRx, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchFrontEndRx, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchBackEndRx, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchTestModeRx, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcCDmaC, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcBURSTFIFO, TRUE);
        DT_RETURN_ON_ERROR(DtBcBURSTFIFO_SetDirection(pPt->m_pBcBURSTFIFO, 
                                                                    DT_BURSTFIFO_DIR_RX));

    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetIoConfigTodRefSel -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtAsiSdiRxTx_SetIoConfigTodRefSel(DtPtAsiSdiRxTx* pPt, 
                                                          const DtCfIoConfigValue* pIoCfg)
{
    Int TodRefClk;
    DT_ASSERT(pIoCfg != NULL);

    // Nothing to configure?
    if (pIoCfg->m_Value == DT_IOCONFIG_NONE)
        return DT_STATUS_OK;

    switch (pIoCfg->m_Value)
    {
    case DT_IOCONFIG_TODREF_INTERNAL:  TodRefClk = DT_TODCLOCKCTRL_REF_INTERNAL; break;
    case DT_IOCONFIG_TODREF_STEADYCLOCK: 
                                       TodRefClk = DT_TODCLOCKCTRL_REF_STEADYCLOCK; break;
    default: DT_ASSERT(FALSE);         TodRefClk = DT_TODCLOCKCTRL_REF_INTERNAL; break;
    }
    DT_ASSERT(pPt->m_pDfTodClockCtrl!=NULL);
    return DtDfTodClockCtrl_SetTodReference(pPt->m_pDfTodClockCtrl, TodRefClk);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetRxIoConfigIoStd -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtAsiSdiRxTx_SetRxIoConfigIoStd(DtPtAsiSdiRxTx* pPt,
                                                          const DtCfIoConfigValue* pIoCfg)
{
    Int CurOpMode, SdiRate=DT_DRV_SDIRATE_UNKNOWN;
    DT_ASSERT(pIoCfg != NULL);

    // ASI or SDI?
    if (pIoCfg->m_Value ==  DT_IOCONFIG_ASI)
    {
        // ASI
        // Disable the SDI formatter block controller
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiRxF, FALSE);
        // Disable the 12G RX block controllers
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiRxMuxOut, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiRxMux12G, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiRxSt425Lr, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiRxMuxIn, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiRxTo4LinkMaster, FALSE);

        // Enable ASI RX driver function
        ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfAsiRx, TRUE);

        // Set switches in the ASI position
        if (pPt->m_pBcSwitchFrontEndRx != NULL)
        { 
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchFrontEndRx, 0, 0));
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchFrontEndRx,
                                                                    DT_BLOCK_OPMODE_RUN));
        }
        if (pPt->m_pBcSwitchBackEndRx != NULL)
        {
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchBackEndRx, 0, 0));
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchBackEndRx,
                                                                    DT_BLOCK_OPMODE_RUN));
        }
        // Set the receiver in ASI reception mode
        DT_RETURN_ON_ERROR(DtDfSdiRx_GetOperationalMode(pPt->m_pDfSdiRx, &CurOpMode));
        DT_RETURN_ON_ERROR(DtDfSdiRx_SetOperationalMode(pPt->m_pDfSdiRx, 
                                                                    DT_FUNC_OPMODE_IDLE));
        DT_RETURN_ON_ERROR(DtDfSdiRx_SetRxMode(pPt->m_pDfSdiRx, DT_SDIRX_RXMODE_ASI));
        DT_RETURN_ON_ERROR(DtDfSdiRx_SetOperationalMode(pPt->m_pDfSdiRx, CurOpMode));

        // Configure SDI-rate
        SdiRate = DT_DRV_SDIRATE_SD;
    } 
    else
    {
        // SDI
        // Disable ASI RX driver function
        ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfAsiRx, FALSE);
        
        // Enable the SDI formatter block controller
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiRxF, TRUE);
        // Enable the 12G RX block controllers
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiRxMuxOut, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiRxMux12G, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiRxSt425Lr, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiRxMuxIn, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiRxTo4LinkMaster, TRUE);

        // Set switches in the SDI position
        if (pPt->m_pBcSwitchFrontEndRx != NULL)
        { 
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchFrontEndRx, 0, 1));
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchFrontEndRx,
                                                                    DT_BLOCK_OPMODE_RUN));
        }
        if (pPt->m_pBcSwitchBackEndRx != NULL)
        { 
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchBackEndRx, 1, 0));
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchBackEndRx,
                                                                    DT_BLOCK_OPMODE_RUN));
        }
        // Set the  SDI reception mode
        DT_RETURN_ON_ERROR(DtDfSdiRx_GetOperationalMode(pPt->m_pDfSdiRx, &CurOpMode));
        DT_RETURN_ON_ERROR(DtDfSdiRx_SetOperationalMode(pPt->m_pDfSdiRx, 
                                                                    DT_FUNC_OPMODE_IDLE));
        DT_RETURN_ON_ERROR(DtDfSdiRx_SetRxMode(pPt->m_pDfSdiRx, DT_SDIRX_RXMODE_SDI));
       
        // Set the  SDI-rate
        switch (pIoCfg->m_Value)
        {
        case DT_IOCONFIG_SDI:    SdiRate = DT_DRV_SDIRATE_SD; break;
        case DT_IOCONFIG_HDSDI:  SdiRate = DT_DRV_SDIRATE_HD; break;
        case DT_IOCONFIG_3GSDI:  SdiRate = DT_DRV_SDIRATE_3G; break;
        case DT_IOCONFIG_6GSDI:  SdiRate = DT_DRV_SDIRATE_6G; break;
        case DT_IOCONFIG_12GSDI: SdiRate = DT_DRV_SDIRATE_12G; break;
        default: DT_ASSERT(FALSE); return DT_STATUS_FAIL;
        }
        DT_RETURN_ON_ERROR(DtDfSdiRx_SetSdiRate(pPt->m_pDfSdiRx, SdiRate));
        DT_RETURN_ON_ERROR(DtDfSdiRx_SetOperationalMode(pPt->m_pDfSdiRx, CurOpMode));
    }

    // Cable equalizer is configured for auto rate detection. This is much faster than the
    // manual rate since it will try to relock after each change.
    if (pPt->m_pDfSpiCableDrvEq != NULL)
        DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetRxSdiRate(pPt->m_pDfSpiCableDrvEq, 
                                                                                SdiRate));
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetIoConfigDmaTestMode -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtAsiSdiRxTx_SetRxIoConfigDmaTestMode(DtPtAsiSdiRxTx* pPt,
    const DtCfIoConfigValue* pIoCfg)
{
    DT_ASSERT(pIoCfg != NULL);

    // Nothing to configure?
    if (pIoCfg->m_Value == DT_IOCONFIG_NONE)
    {
        DT_ASSERT(pPt->m_pBcSwitchTestModeRx == NULL);
        return DT_STATUS_OK;
    }

    DT_ASSERT(pPt->m_pBcSwitchTestModeRx != NULL);
    if (pIoCfg->m_Value == DT_IOCONFIG_TRUE)
    {
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcConstSource, TRUE);
        DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchTestModeRx, 1, 0));
        DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchTestModeRx,
                                                                    DT_BLOCK_OPMODE_RUN));
        DT_RETURN_ON_ERROR(DtBcBURSTFIFO_SetRxFlowControl(pPt->m_pBcBURSTFIFO,
                                                             DT_BURSTFIFO_FLOW_PUSHPULL));
    }
    else if (pIoCfg->m_Value == DT_IOCONFIG_FALSE)
    {
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcConstSource, FALSE);
        DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchTestModeRx, 0, 0));
        DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchTestModeRx,
                                                                    DT_BLOCK_OPMODE_RUN));
        DT_RETURN_ON_ERROR(DtBcBURSTFIFO_SetRxFlowControl(pPt->m_pBcBURSTFIFO,
                                                             DT_BURSTFIFO_FLOW_VIRTPUSH));
    }
    else
    {
        DT_ASSERT(FALSE);
        return DT_STATUS_INVALID_PARAMETER;
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetRxIoConfigFailSafe -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtAsiSdiRxTx_SetRxIoConfigFailSafe(DtPtAsiSdiRxTx* pPt, 
                                                          const DtCfIoConfigValue* pIoCfg)
{
    DT_ASSERT(pIoCfg != NULL);

    // Failsafe cannot be configured for input
    if (pIoCfg->m_Value != DT_IOCONFIG_NONE)
    {
        DT_ASSERT(FALSE);
        return DT_STATUS_INVALID_PARAMETER;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetRxIoConfigGenLock -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtAsiSdiRxTx_SetRxIoConfigGenLock(DtPtAsiSdiRxTx* pPt, 
                                                          const DtCfIoConfigValue* pIoCfg)
{
    // Nothing to configure?
    if (pIoCfg->m_Value == DT_IOCONFIG_NONE)
    {
        DT_ASSERT(!DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_GENLOCKED));
        return DT_STATUS_OK;
    }

    DT_ASSERT(DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_GENLOCKED));

    // For inputs GenLocked must be switched off; but just ignore the setting
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetRxIoConfigIoDownscale -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtAsiSdiRxTx_SetRxIoConfigIoDownscale(DtPtAsiSdiRxTx* pPt, 
                                                          const DtCfIoConfigValue* pIoCfg)
{
    Int  CurOpMode, DownscaleMode;
    // Nothing to configure?
    if (pIoCfg->m_Value == DT_IOCONFIG_NONE)
        return DT_STATUS_OK;

    switch (pIoCfg->m_Value)
    {
        case DT_IOCONFIG_SCALE_12GTO3G: DownscaleMode = TRUE; break;
        case DT_IOCONFIG_SCALE_BYPASS:  DownscaleMode = FALSE; break;
        default: DT_ASSERT(FALSE);      DownscaleMode = FALSE; break;
    }

    // Temporarilly switch to IDLE
    DT_RETURN_ON_ERROR(DtDfSdiRx_GetOperationalMode(pPt->m_pDfSdiRx, &CurOpMode));
    DT_RETURN_ON_ERROR(DtDfSdiRx_SetOperationalMode(pPt->m_pDfSdiRx, 
                                                                DT_FUNC_OPMODE_IDLE));
    DT_RETURN_ON_ERROR(DtDfSdiRx_SetDownscaleMode(pPt->m_pDfSdiRx, DownscaleMode));
    DT_RETURN_ON_ERROR(DtDfSdiRx_SetOperationalMode(pPt->m_pDfSdiRx, CurOpMode));

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetTxIoConfigIoStd -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtAsiSdiRxTx_SetTxIoConfigIoStd(DtPtAsiSdiRxTx* pPt,
                                                          const DtCfIoConfigValue* pIoCfg)
{
    Bool  LevelA2BEnable = FALSE;
    Int  SdiRate, VidStd;
    Int  CurOpMode;
    DT_ASSERT(pIoCfg != NULL);

    // ASI or SDI?
    if (pIoCfg->m_Value == DT_IOCONFIG_ASI)
    {
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ASI -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        // Disable the SDI formatter block controller
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxF, FALSE);
        // Disable the SDI protocol block controller
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxP, FALSE);
        // Disable the 12G TX block controllers
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiTxDmxOut, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxDmx12G, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiTxDmxIn, FALSE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiTxFrom4LinkMaster, FALSE);

        // Enable ASI TX Gate block controller
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcAsiTxG, TRUE);
        // Enable ASI TX Serializer
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcAsiTxSer, TRUE);

        // Set switches in the ASI position
        if (pPt->m_pBcSwitchFrontEndTx != NULL)
        { 
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchFrontEndTx, 0, 0));
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchFrontEndTx,
                                                                     DT_FUNC_OPMODE_RUN));
        }
        if (pPt->m_pBcSwitchBackEndTx != NULL)
        {
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchBackEndTx, 0, 0));
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchBackEndTx,
                                                                     DT_FUNC_OPMODE_RUN));
        }

        // Set the transmitter in ASI  mode (switch to IDLE during setting)
        if (pPt->m_pDfSdiTxPhy != NULL)
        {
            DT_RETURN_ON_ERROR(DtDfSdiTxPhy_GetOperationalMode(pPt->m_pDfSdiTxPhy,
                                                                             &CurOpMode));
            DT_RETURN_ON_ERROR(DtDfSdiTxPhy_SetOperationalMode(pPt->m_pDfSdiTxPhy,
                                                                    DT_FUNC_OPMODE_IDLE));
            DT_RETURN_ON_ERROR(DtDfSdiTxPhy_SetTxMode(pPt->m_pDfSdiTxPhy,
                                                                 DT_SDITXPHY_TXMODE_ASI));
            DT_RETURN_ON_ERROR(DtDfSdiTxPhy_SetOperationalMode(pPt->m_pDfSdiTxPhy,
                                                                              CurOpMode));
        }

        // If we have GS2988 cable driver, set it for SD operation now
        if (pPt->m_pBcGs2988 != NULL)
            DT_RETURN_ON_ERROR(DtBcGS2988_SetSdiMode(pPt->m_pBcGs2988, 
                                                                   DT_GS2988_SDIMODE_SD));
        // If we have SPI-cable driver, set SDI-rate to SD
        if (pPt->m_pDfSpiCableDrvEq != NULL)
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetTxSdiRate(pPt->m_pDfSpiCableDrvEq, 
                                                                      DT_DRV_SDIRATE_SD));
    } 
    else
    {
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SDI -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        // Determine video standard
        VidStd = DtAvIoStd2VidStd(pIoCfg->m_Value, pIoCfg->m_SubValue);
        DT_ASSERT(VidStd != DT_VIDSTD_UNKNOWN);
        
        // Determine SDI-rate
        switch (pIoCfg->m_Value)
        {
        case DT_IOCONFIG_SDI:    SdiRate = DT_DRV_SDIRATE_SD; break;
        case DT_IOCONFIG_HDSDI:  SdiRate = DT_DRV_SDIRATE_HD; break;
        case DT_IOCONFIG_3GSDI:  SdiRate = DT_DRV_SDIRATE_3G; break;
        case DT_IOCONFIG_6GSDI:  SdiRate = DT_DRV_SDIRATE_6G; break;
        case DT_IOCONFIG_12GSDI: SdiRate = DT_DRV_SDIRATE_12G; break;
        default: DT_ASSERT(FALSE); return DT_STATUS_FAIL;
        }

        // Disable ASI TX Gate block controller
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcAsiTxG, FALSE);
        // Disable ASI TX Serializer
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcAsiTxSer, FALSE);

        // Enable the SDI formatter block controller
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxF, TRUE);
        // Enable the SDI protocol block controller
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxP, TRUE);
        // Enable the 12G TX block controllers
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiTxDmxOut, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxDmx12G, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiTxDmxIn, TRUE);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchSdiTxFrom4LinkMaster, TRUE);

        // Set switches in the SDI position
        if (pPt->m_pBcSwitchFrontEndTx != NULL)
        { 
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchFrontEndTx, 1, 0));
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchFrontEndTx,
                                                                    DT_BLOCK_OPMODE_RUN));
        }
        if (pPt->m_pBcSwitchBackEndTx != NULL)
        { 
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchBackEndTx, 0, 1));
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchBackEndTx,
                                                                    DT_BLOCK_OPMODE_RUN));
        }

        // Save current opmode
        DT_RETURN_ON_ERROR(DtDfSdiTxPhy_GetOperationalMode(pPt->m_pDfSdiTxPhy,
                                                                             &CurOpMode));
        DT_RETURN_ON_ERROR(DtDfSdiTxPhy_SetOperationalMode(pPt->m_pDfSdiTxPhy,
                                                                    DT_FUNC_OPMODE_IDLE));
        // Set the transmitter in SDI transmitter mode
        DT_RETURN_ON_ERROR(DtDfSdiTxPhy_SetTxMode(pPt->m_pDfSdiTxPhy,
                                                                 DT_SDITXPHY_TXMODE_SDI));
        // Set the new video standard
        DT_RETURN_ON_ERROR(DtDfSdiTxPhy_SetVidStd(pPt->m_pDfSdiTxPhy, VidStd));

        DT_RETURN_ON_ERROR(DtDfSdiTxPhy_SetOperationalMode(pPt->m_pDfSdiTxPhy,
                                                                              CurOpMode));

        //  Determine whether video standard is level-B
        LevelA2BEnable = DtAvVidStdIs3glvlBSdi(VidStd);
        DT_RETURN_ON_ERROR(DtBcSDITXP_SetSdiRateAndLvlA2BEna(pPt->m_pBcSdiTxP,
                                                           SdiRate, (Int)LevelA2BEnable));

        // If we have GS2988 cable driver, set it for SD or HD/3G mode
        if (pPt->m_pBcGs2988 != NULL)
        {
            Int  Mode = (SdiRate==DT_DRV_SDIRATE_SD) ? 
                                           DT_GS2988_SDIMODE_SD : DT_GS2988_SDIMODE_HD_3G;
            DT_RETURN_ON_ERROR(DtBcGS2988_SetSdiMode(pPt->m_pBcGs2988, Mode));
        }

        // If we have SPI-cable driver, set SDI-rate
        if (pPt->m_pDfSpiCableDrvEq != NULL)
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetTxSdiRate(pPt->m_pDfSpiCableDrvEq, 
                                                                                SdiRate));
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetTxIoConfigIoStdDblBuf -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtAsiSdiRxTx_SetTxIoConfigIoStdDblBuf(DtPtAsiSdiRxTx* pPt,
                                                          const DtCfIoConfigValue* pIoCfg)
{
    Int  SdiRate, VidStd;
    Int  CurOpMode;
    DT_ASSERT(pIoCfg != NULL);

    // ASI or SDI?
    if (pIoCfg->m_Value == DT_IOCONFIG_ASI)
    {
        // Set the transmitter in ASI  mode (switch to IDLE during setting)
        if (pPt->m_pDfSdiTxPhy != NULL)
        {
            DT_RETURN_ON_ERROR(DtDfSdiTxPhy_GetOperationalMode(pPt->m_pDfSdiTxPhy,
                                                                             &CurOpMode));
            DT_RETURN_ON_ERROR(DtDfSdiTxPhy_SetOperationalMode(pPt->m_pDfSdiTxPhy,
                                                                    DT_FUNC_OPMODE_IDLE));
            DT_RETURN_ON_ERROR(DtDfSdiTxPhy_SetTxMode(pPt->m_pDfSdiTxPhy,
                                                                 DT_SDITXPHY_TXMODE_ASI));
            DT_RETURN_ON_ERROR(DtDfSdiTxPhy_SetOperationalMode(pPt->m_pDfSdiTxPhy,
                                                                              CurOpMode));
        }

        // If we have GS2988 cable driver, set it for SD operation now
        if (pPt->m_pBcGs2988 != NULL)
            DT_RETURN_ON_ERROR(DtBcGS2988_SetSdiMode(pPt->m_pBcGs2988, 
                                                                   DT_GS2988_SDIMODE_SD));
        // If we have SPI-cable driver, set SDI-rate to SD
        if (pPt->m_pDfSpiCableDrvEq != NULL)
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetTxSdiRate(pPt->m_pDfSpiCableDrvEq, 
                                                                      DT_DRV_SDIRATE_SD));
    } 
    else
    {
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SDI -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        // Determine video standard
        VidStd = DtAvIoStd2VidStd(pIoCfg->m_Value, pIoCfg->m_SubValue);
        DT_ASSERT(VidStd != DT_VIDSTD_UNKNOWN);
        
        // Determine SDI-rate
        switch (pIoCfg->m_Value)
        {
        case DT_IOCONFIG_SDI:    SdiRate = DT_DRV_SDIRATE_SD; break;
        case DT_IOCONFIG_HDSDI:  SdiRate = DT_DRV_SDIRATE_HD; break;
        case DT_IOCONFIG_3GSDI:  SdiRate = DT_DRV_SDIRATE_3G; break;
        case DT_IOCONFIG_6GSDI:  SdiRate = DT_DRV_SDIRATE_6G; break;
        case DT_IOCONFIG_12GSDI: SdiRate = DT_DRV_SDIRATE_12G; break;
        default: DT_ASSERT(FALSE); return DT_STATUS_FAIL;
        }

        // Save current opmode
        DT_RETURN_ON_ERROR(DtDfSdiTxPhy_GetOperationalMode(pPt->m_pDfSdiTxPhy,
                                                                             &CurOpMode));
        DT_RETURN_ON_ERROR(DtDfSdiTxPhy_SetOperationalMode(pPt->m_pDfSdiTxPhy,
                                                                    DT_FUNC_OPMODE_IDLE));
        // Set the transmitter in SDI transmitter mode
        DT_RETURN_ON_ERROR(DtDfSdiTxPhy_SetTxMode(pPt->m_pDfSdiTxPhy,
                                                                 DT_SDITXPHY_TXMODE_SDI));
        // Set the new video standard
        DT_RETURN_ON_ERROR(DtDfSdiTxPhy_SetVidStd(pPt->m_pDfSdiTxPhy, VidStd));

        DT_RETURN_ON_ERROR(DtDfSdiTxPhy_SetOperationalMode(pPt->m_pDfSdiTxPhy,
                                                                              CurOpMode));

        // If we have GS2988 cable driver, set it for SD or HD/3G mode
        if (pPt->m_pBcGs2988 != NULL)
        {
            Int  Mode = (SdiRate==DT_DRV_SDIRATE_SD) ? 
                                           DT_GS2988_SDIMODE_SD : DT_GS2988_SDIMODE_HD_3G;
            DT_RETURN_ON_ERROR(DtBcGS2988_SetSdiMode(pPt->m_pBcGs2988, Mode));
        }

        // If we have SPI-cable driver, set SDI-rate
        if (pPt->m_pDfSpiCableDrvEq != NULL)
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetTxSdiRate(pPt->m_pDfSpiCableDrvEq, 
                                                                                SdiRate));
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetTxIoConfigDmaTestMode -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtAsiSdiRxTx_SetTxIoConfigDmaTestMode(DtPtAsiSdiRxTx* pPt,
                                                          const DtCfIoConfigValue* pIoCfg)
{
    DT_ASSERT(pIoCfg != NULL);

    // Nothing to configure?
    if (pIoCfg->m_Value == DT_IOCONFIG_NONE)
    {
        DT_ASSERT(pPt->m_pBcSwitchTestModeTx == NULL);
        return DT_STATUS_OK;
    }
    DT_ASSERT(pPt->m_pBcSwitchTestModeTx != NULL);
    if (pIoCfg->m_Value == DT_IOCONFIG_TRUE)
    {
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcConstSink, TRUE);
        DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchTestModeTx, 0, 1));
        DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchTestModeTx,
                                                                    DT_BLOCK_OPMODE_RUN));
    }
    else if (pIoCfg->m_Value == DT_IOCONFIG_FALSE)
    {
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcConstSink, FALSE);
        DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchTestModeTx, 0, 0));
        DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchTestModeTx,
                                                                    DT_BLOCK_OPMODE_RUN));
    }
    else
    {
        DT_ASSERT(FALSE);
        return DT_STATUS_INVALID_PARAMETER;
    }
    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetTxIoConfigFailSafe -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtAsiSdiRxTx_SetTxIoConfigFailSafe(DtPtAsiSdiRxTx* pPt, 
                                                          const DtCfIoConfigValue* pIoCfg)
{
    DT_ASSERT(pIoCfg != NULL);

    // Nothing to configure?
    if (pIoCfg->m_Value == DT_IOCONFIG_NONE)
    {
        DT_ASSERT(pPt->m_pBcKeepAlive == NULL);
        return DT_STATUS_OK;
    }

    DT_ASSERT(pPt->m_pBcKeepAlive != NULL);
    // If failsafe is enabled, disable the automatic keep-alive
    if (pIoCfg->m_Value == DT_IOCONFIG_TRUE)
        return DtBcKA_SetAutoKeepAlive(pPt->m_pBcKeepAlive, FALSE,
                                                               (Int)pIoCfg->m_ParXtra[0]);
    else if (pIoCfg->m_Value == DT_IOCONFIG_FALSE)
        return DtBcKA_SetAutoKeepAlive(pPt->m_pBcKeepAlive, TRUE, 0);
    else
    { 
        DT_ASSERT(FALSE);
        return DT_STATUS_INVALID_PARAMETER;
    }
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetTxIoConfigGenLock -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtAsiSdiRxTx_SetTxIoConfigGenLock(DtPtAsiSdiRxTx* pPt, 
                                                          const DtCfIoConfigValue* pIoCfg)
{
    // Nothing to configure?
    if (pIoCfg->m_Value==DT_IOCONFIG_NONE)
    {
        DT_ASSERT(!DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_GENLOCKED));
        return DT_STATUS_OK;
    }

    DT_ASSERT(DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_GENLOCKED));

    // Check genlocked values; no action is needed
    if (pIoCfg->m_Value==DT_IOCONFIG_TRUE || pIoCfg->m_Value==DT_IOCONFIG_FALSE)
        return DT_STATUS_OK;
    else
    { 
        DT_ASSERT(FALSE);
        return DT_STATUS_INVALID_PARAMETER;
    }
}

// -.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetTxIoConfigIoDownscale -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtAsiSdiRxTx_SetTxIoConfigIoDownscale(DtPtAsiSdiRxTx* pPt, 
                                                          const DtCfIoConfigValue* pIoCfg)
{
    DT_ASSERT(pIoCfg != NULL);
    
    // Ignore this setting for the output

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetIoConfigPrepare -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtAsiSdiRxTx_SetIoConfigPrepare(DtPt* pPtBase, 
                                                        const DtExclAccessObject* pObject)
{
    DtStatus  Status=DT_STATUS_OK, TempStatus=DT_STATUS_OK;
    DtPtAsiSdiRxTx* pPt = (DtPtAsiSdiRxTx*)pPtBase;
    Bool MustAcquireExclAccess = TRUE;
    Int  OpStatus, OpMode;

    // Sanity check
    PT_ASISDIRXTX_DEFAULT_PRECONDITIONS(pPt);

    // is we have a SDI receive channel function, lock it so that no one else can attach 
    // or change its state
    if (pPt->m_pDfChSdiRx)
    {
        // Lock the channel
        Status = DtDfCh_Lock((DtDfCh*)pPt->m_pDfChSdiRx, 100);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: cannot lock SDI receive channel");
            return Status;
        }
        pPt->m_HoldChannelLock = TRUE;

        // Check if the channel has users and if yes, if this is one of the channels users
        if (DtDfCh_HasUsers((DtDfCh*)pPt->m_pDfChSdiRx))
        {
            const DtFileObject* pFile = &pObject->m_Owner.m_File;
            if (!DtDfCh_FindUser((DtDfCh*)pPt->m_pDfChSdiRx, pFile))
            {
                DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: SDI receive channel is in use");
                // Unlock the channel
                DtDfCh_Unlock((DtDfCh*)pPt->m_pDfChSdiRx);
                pPt->m_HoldChannelLock = FALSE;
                return DT_STATUS_IN_USE;      // Used by some one else
            }
            // When this is one of the channels users, do not acquire exclusive access on 
            // the children of channel, since the channel manages exclusive access for its
            // children.
            MustAcquireExclAccess = FALSE;
        }
        else
            MustAcquireExclAccess = TRUE;   // Channel has no users => lock children
    }

    // Acquire exclusive access to children
    if (MustAcquireExclAccess)
    {
        // Acquire exclusive access to children
        Status = DtPt_AcquireExclAccessChildren(pPtBase, pObject);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: Cannot acquire exclusive access");
            // Failed
            return Status;
        }
        pPt->m_HoldExclAccessLock = TRUE;
    }

    // Check operational mode/state of the childeren
    if (DT_SUCCESS(Status) && pPt->m_pDfSdiTxPhy!=NULL)
    { 
        // PHY may be in standby
        TempStatus = DtDfSdiTxPhy_GetOperationalMode(pPt->m_pDfSdiTxPhy, &OpMode);
        if (TempStatus!=DT_STATUS_NOT_ENABLED
                              && (!DT_SUCCESS(TempStatus) || OpMode==DT_FUNC_OPMODE_RUN))
            Status = DT_STATUS_BUSY;
    }

    if (DT_SUCCESS(Status) && pPt->m_pBcAsiTxG!=NULL)
    {
        TempStatus = DtBcASITXG_GetOperationalMode(pPt->m_pBcAsiTxG, &OpMode);
        if (TempStatus!=DT_STATUS_NOT_ENABLED
                             && (!DT_SUCCESS(TempStatus) || OpMode!=DT_BLOCK_OPMODE_IDLE))
            Status = DT_STATUS_BUSY;
    }
    if (DT_SUCCESS(Status) && pPt->m_pBcAsiTxSer!=NULL)
    {
        // Serializer may be in standby
        TempStatus = DtBcASITXSER_GetOperationalMode(pPt->m_pBcAsiTxSer, &OpMode);
        if (TempStatus!=DT_STATUS_NOT_ENABLED
                              && (!DT_SUCCESS(TempStatus) || OpMode==DT_BLOCK_OPMODE_RUN))
            Status = DT_STATUS_BUSY;
    }
    if (DT_SUCCESS(Status) && pPt->m_pBcSdiTxF!=NULL)
    {
        TempStatus = DtBcSDITXF_GetOperationalMode(pPt->m_pBcSdiTxF, &OpMode);
        if (TempStatus!=DT_STATUS_NOT_ENABLED 
                            && (!DT_SUCCESS(TempStatus) ||  OpMode!=DT_BLOCK_OPMODE_IDLE))
            Status = DT_STATUS_BUSY;
    }
    if (DT_SUCCESS(Status) && pPt->m_pBcSdiTxP!=NULL)
    {
        TempStatus = DtBcSDITXP_GetOperationalMode(pPt->m_pBcSdiTxP, &OpMode);
        if (TempStatus!=DT_STATUS_NOT_ENABLED 
                             && (!DT_SUCCESS(TempStatus) || OpMode!=DT_BLOCK_OPMODE_IDLE))
            Status = DT_STATUS_BUSY;
    }
    if (DT_SUCCESS(Status) && pPt->m_pBcSdiTxDmx12G!=NULL)
    {
        TempStatus = DtBcSDIDMX12G_GetOperationalMode(pPt->m_pBcSdiTxDmx12G, &OpMode);
        if (TempStatus!=DT_STATUS_NOT_ENABLED 
                             && (!DT_SUCCESS(TempStatus) || OpMode!=DT_BLOCK_OPMODE_IDLE))
            Status = DT_STATUS_BUSY;
    }
    if (DT_SUCCESS(Status) && pPt->m_pBcCDmaC!=NULL)
    {
        UInt32 StatusFlags, ErrorFlags; 
        TempStatus = DtBcCDMAC_GetStatus(pPt->m_pBcCDmaC, &OpStatus, &StatusFlags,
                                                                             &ErrorFlags);
        if (TempStatus!=DT_STATUS_NOT_ENABLED 
                         && (!DT_SUCCESS(TempStatus) || OpStatus!=DT_BLOCK_OPSTATUS_IDLE))
            Status = DT_STATUS_BUSY;
    }
    if (DT_SUCCESS(Status) && pPt->m_pBcBURSTFIFO!=NULL)
    {
        TempStatus = DtBcBURSTFIFO_GetOperationalMode(pPt->m_pBcBURSTFIFO, &OpMode);
        if (TempStatus!=DT_STATUS_NOT_ENABLED 
                             && (!DT_SUCCESS(TempStatus) || OpMode!=DT_BLOCK_OPMODE_IDLE))
            Status = DT_STATUS_BUSY;
    }
    // Check operational mode/state of the RX children
    if (DT_SUCCESS(Status) && pPt->m_pDfAsiRx!=NULL)
    {

        TempStatus = DtDfAsiRx_GetOperationalStatus(pPt->m_pDfAsiRx, &OpStatus);
        if (TempStatus!=DT_STATUS_NOT_ENABLED
                             && (!DT_SUCCESS(TempStatus) || OpStatus==DT_FUNC_OPMODE_RUN))
            Status = DT_STATUS_BUSY;
    }

    if (DT_SUCCESS(Status) && pPt->m_pDfSdiRx!=NULL)
    {
        // SDIRX3G may be in standby
        TempStatus = DtDfSdiRx_GetOperationalMode(pPt->m_pDfSdiRx, &OpMode);
        if (TempStatus!=DT_STATUS_NOT_ENABLED
                               && (!DT_SUCCESS(TempStatus) || OpMode==DT_FUNC_OPMODE_RUN))
            Status = DT_STATUS_BUSY;
    }

    if (DT_SUCCESS(Status) && pPt->m_pBcSdiRxF!=NULL)
    {
        TempStatus = DtBcSDIRXF_GetOperationalMode(pPt->m_pBcSdiRxF, &OpMode);
        if (TempStatus!=DT_STATUS_NOT_ENABLED 
                             && (!DT_SUCCESS(TempStatus) || OpMode!=DT_BLOCK_OPMODE_IDLE))
            Status = DT_STATUS_BUSY;
    }
    if (DT_SUCCESS(Status) && pPt->m_pBcSdiRxMux12G!=NULL)
    {
        TempStatus = DtBcSDIMUX12G_GetOperationalMode(pPt->m_pBcSdiRxMux12G, &OpMode);
        if (TempStatus!=DT_STATUS_NOT_ENABLED 
                             && (!DT_SUCCESS(TempStatus) || OpMode!=DT_BLOCK_OPMODE_IDLE))
            Status = DT_STATUS_BUSY;
    }
    if (DT_SUCCESS(Status) && pPt->m_pBcSdiRxSt425Lr!=NULL)
    {
        TempStatus = DtBcST425LR_GetOperationalMode(pPt->m_pBcSdiRxSt425Lr, &OpMode);
        if (TempStatus!=DT_STATUS_NOT_ENABLED 
                             && (!DT_SUCCESS(TempStatus) || OpMode!=DT_BLOCK_OPMODE_IDLE))
            Status = DT_STATUS_BUSY;
    }
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPt(ERR, ASISDIRXTX, pPt, "ERROR: Children not in IDLE");

        // Failed; Now release exclusive access to children and channel if needed
        if (pPt->m_HoldExclAccessLock)
        {
            DtPt_ReleaseExclAccessChildren(pPtBase, pObject);
            pPt->m_HoldExclAccessLock = FALSE;
        }
        if (pPt->m_HoldChannelLock)
        {
            // Unlock the channel
            DtDfCh_Unlock((DtDfCh*)pPt->m_pDfChSdiRx);
            pPt->m_HoldChannelLock = FALSE;
        }
    }
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_SetIoConfigFinish -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtAsiSdiRxTx_SetIoConfigFinish(DtPt* pPtBase, const DtExclAccessObject* pObject)
{
    DtStatus  Status = DT_STATUS_OK;
    DtPtAsiSdiRxTx* pPt = (DtPtAsiSdiRxTx*)pPtBase;

    // Sanity check
    PT_ASISDIRXTX_DEFAULT_PRECONDITIONS(pPt);

    // Release exclusive access to children and channel if needed
    if (pPt->m_HoldChannelLock && pPt->m_pDfChSdiRx)
        DtDfCh_Unlock((DtDfCh*)pPt->m_pDfChSdiRx);
    if (pPt->m_HoldExclAccessLock)
        Status = DtPt_ReleaseExclAccessChildren(pPtBase, pObject);
    // Clear lock flags
    pPt->m_HoldExclAccessLock = pPt->m_HoldChannelLock = FALSE;
    return Status;
}

#ifdef LINBUILD
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiRxTx_Mmap -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Maps channels DMA buffer to the address space of the calling process, provided an 
// enabled SDI receive channel is available.
//
DtStatus DtPtAsiSdiRxTx_Mmap(DtPt* pPtBase, const DtFileObject* pFile, DtVma* pVma)
{
    DtStatus Status=DT_STATUS_OK;
    DtPtAsiSdiRxTx* pPt = (DtPtAsiSdiRxTx*)pPtBase;
    
    PT_ASISDIRXTX_DEFAULT_PRECONDITIONS(pPt);

    // Check parameters
    if (!pVma)
        return DT_STATUS_INVALID_PARAMETER;

    // Do we have a enabled channel function. Mmap is not support unless we have an 
    // enabled channel function.
    if (!pPt->m_pDfChSdiRx || !DtDf_IsEnabled((DtDf*)pPt->m_pDfChSdiRx))
        return DT_STATUS_NOT_SUPPORTED; 

    Status = DtDfChSdiRx_Lock(pPt->m_pDfChSdiRx, 100);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPt(ERR,SDIPHYONLYRXTX,pPt, "ERROR: cannot lock SDI receive channel");
        return Status;
    }
    Status = DtDfChSdiRx_Mmap(pPt->m_pDfChSdiRx, pFile, pVma);
    if (!DT_SUCCESS(Status))
        DtDbgOutPt(ERR, SDIPHYONLYRXTX, pPt, "ERROR: failed to map memory");

    // Do not forget to unlock channel
    DtDfChSdiRx_Unlock(pPt->m_pDfChSdiRx);
    return Status;
}
#endif // #ifdef LINBUILD