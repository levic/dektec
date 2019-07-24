//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSdiXCfgMgr.c *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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
#include "DtDfSdiXCfgMgr.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Structure that stores the block controller and the logical channel number
// per channel type
typedef struct  _DtDfSdiXCfgMgr_SdiXCfgChans
{
    DtBcSDIXCFG*  m_pRxChanBcSdiXCfg;
    Int  m_RxChanLogChannel;
    DtBcSDIXCFG*  m_pTxChanBcSdiXCfg;
    Int  m_TxChanLogChannel;
    DtBcSDIXCFG*  m_pTxPllBcSdiXCfg;
    Int  m_TxPllLogChannel;
}DtDfSdiXCfgMgr_SdiXCfgChans;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSdiXCfgMgr implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiRx function
#define DF_SDIXCFGMGR_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfSdiXCfgMgr))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_SDIXCFGMGR_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(SDIXCFGMGR, pDf)

// Helper macro to cast a DtDf* to a DtDfSdiXCfgMgr*
#define DF_SDIXCFGMGR      ((DtDfSdiXCfgMgr*)pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfSdiXCfgMgr_Init(DtDf*);
static DtStatus  DtDfSdiXCfgMgr_CleanUp(DtDfSdiXCfgMgr*);
static DtStatus  DtDfSdiXCfgMgr_OnEnablePostChildren(DtDf*, Bool  Enable);

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSdiXCfgMgr - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiXCfgMgr_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfSdiXCfgMgr_Close(DtDf*  pDf)
{
     DF_SDIXCFGMGR_DEFAULT_PRECONDITIONS(pDf);

    // Clean-up
    DtDfSdiXCfgMgr_CleanUp(DF_SDIXCFGMGR);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiXCfgMgr_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfSdiXCfgMgr*  DtDfSdiXCfgMgr_Open(DtCore*  pCore, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_SDIXCFGMGR_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfSdiXCfgMgr, Id, DT_FUNC_TYPE_SDIXCFGMGR, pPt,
                                                                            FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfSdiXCfgMgr_Close;
    OpenParams.m_InitFunc = DtDfSdiXCfgMgr_Init;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfSdiXCfgMgr_OnEnablePostChildren;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfSdiXCfgMgr*)DtDf_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiXCfgMgr_SetXcvrMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiXCfgMgr_SetXcvrMode(DtDfSdiXCfgMgr* pDf, Int PortIndex, 
                                                            Int ChannelType, Int XcvrMode)
{
    DtBcSDIXCFG*  pBcSdiXCfg = NULL;
    DtDfSdiXCfgMgr_SdiXCfgChans*  pChanSdiXCfg = NULL;
    Int  LogChannelNr = -1;

    // Sanity checks
    DF_SDIXCFGMGR_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_pBcSdiXCfgs!=NULL && pDf->m_pPortIdxToSdiXCfgsChans!=NULL);

    // Operational state must be enabled
    DF_SDIXCFGMGR_MUST_BE_ENABLED(pDf);

 
    // Check parameters
    if (PortIndex<0 || PortIndex>=DtVector_Size(pDf->m_pPortIdxToSdiXCfgsChans))
    {
        DtDbgOutDf(ERR, SDIXCFGMGR, pDf, "ERROR: Invalid portindex: %d", PortIndex);
        return DT_STATUS_INVALID_PARAMETER;
    }
    if (ChannelType!=DT_SDIXCFG_CHANTYPE_RX_CHAN
                                               && ChannelType!=DT_SDIXCFG_CHANTYPE_TX_CHAN
                                               && ChannelType!=DT_SDIXCFG_CHANTYPE_TX_PLL)
    {
        DtDbgOutDf(ERR, SDIXCFGMGR, pDf, "ERROR: Invalid channel type: %d", ChannelType);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Get configuration channels for this port
    pChanSdiXCfg = (DtDfSdiXCfgMgr_SdiXCfgChans*)DtVector_At(
                                               pDf->m_pPortIdxToSdiXCfgsChans, PortIndex);
    DT_ASSERT(pChanSdiXCfg != NULL);

    // Get BC and logical channel number for the channel type
    switch (ChannelType)
    {
    case DT_SDIXCFG_CHANTYPE_RX_CHAN:
        pBcSdiXCfg = pChanSdiXCfg->m_pRxChanBcSdiXCfg;
        LogChannelNr = pChanSdiXCfg->m_RxChanLogChannel;
        break;
    case DT_SDIXCFG_CHANTYPE_TX_CHAN:
        pBcSdiXCfg = pChanSdiXCfg->m_pTxChanBcSdiXCfg;
        LogChannelNr = pChanSdiXCfg->m_TxChanLogChannel;
        break;
    case DT_SDIXCFG_CHANTYPE_TX_PLL:
        pBcSdiXCfg = pChanSdiXCfg->m_pTxPllBcSdiXCfg;
        LogChannelNr = pChanSdiXCfg->m_TxPllLogChannel;
        break;
    }

    //   SDIXCFG BC not found?
    if (pBcSdiXCfg == NULL)
    { 
        DtDbgOutDf(ERR, SDIXCFGMGR, pDf, "ERROR: BC SDIXCFG not found for portindex: %d, "
                                               "channeltype: %d", PortIndex, ChannelType);
        return DT_STATUS_NOT_ENABLED;
    }

    // Let the SDIXCFG BC perform the reconfiguration
    return DtBcSDIXCFG_SetMode(pBcSdiXCfg, LogChannelNr, XcvrMode);
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSdiXCfgMgr - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiXCfgMgr_CleanUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSdiXCfgMgr_CleanUp(DtDfSdiXCfgMgr*  pDf)
{
    if (pDf->m_pBcSdiXCfgs != NULL)
    {
        DtVectorBc_Cleanup(pDf->m_pBcSdiXCfgs);
        pDf->m_pBcSdiXCfgs = NULL;
    }
    if (pDf->m_pPortIdxToSdiXCfgsChans != NULL)
    {
        DtVector_Cleanup(pDf->m_pPortIdxToSdiXCfgsChans);
        pDf->m_pPortIdxToSdiXCfgsChans = NULL;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiXCfgMgr_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSdiXCfgMgr_Init(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfSdiXCfgMgr* pDf = (DtDfSdiXCfgMgr*)pDfBase;

    // Sanity checks
    DF_SDIXCFGMGR_DEFAULT_PRECONDITIONS(pDf);


    // Create list for the children
    DT_ASSERT(pDf->m_pBcSdiXCfgs== NULL);
    pDf->m_pBcSdiXCfgs = DtVectorBc_Create(1, 1);
    DT_ASSERT(pDf->m_pBcSdiXCfgs != NULL);

    // Create look-up table
    DT_ASSERT(pDf->m_pPortIdxToSdiXCfgsChans == NULL);
    pDf->m_pPortIdxToSdiXCfgsChans = DtVector_Create(1, 
                                                  sizeof(DtDfSdiXCfgMgr_SdiXCfgChans), 1);
    DT_ASSERT(pDf->m_pPortIdxToSdiXCfgsChans != NULL);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDf_OpenChildrenOfBcType(pDfBase, DT_BLOCK_TYPE_SDIXCFG, pDf->m_pBcSdiXCfgs);
    if (!DT_SUCCESS(Status))
    {

        DtDbgOutDf(ERR, SDIXCFGMGR, pDf, "ERROR: failed to open children");
        DtDfSdiXCfgMgr_CleanUp(pDf);
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.- DtDfSdiXCfgMgr_InitPortIdxToSdiXCfgsChans -.-.-.-.-.-.-.-.-.-.-.-
//
//  Creates and initializes the PortIndex to reconfiguration channel look-up table
//
DtStatus DtDfSdiXCfgMgr_InitPortIdxToSdiXCfgsChans(DtDfSdiXCfgMgr* pDf)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i, NumPorts, NumBcs;
    const DtDfSdiXCfgMgr_SdiXCfgChans  EmptyChanSdiXCfg = {NULL, -1, NULL, -1, NULL, -1};
    DtDfSdiXCfgMgr_SdiXCfgChans* pChanSdiXCfg = NULL;
    DtBcSDIXCFG*  pBcSdiXCfg = NULL;

    NumPorts = DtCore_DEVICE_GetNumPorts(pDf->m_pCore);

    // Childeren should be opened
    DT_ASSERT(pDf->m_pBcSdiXCfgs != NULL);

    // Resize the look-up table
    DT_ASSERT(pDf->m_pPortIdxToSdiXCfgsChans != NULL);
    Status = DtVector_Resize(pDf->m_pPortIdxToSdiXCfgsChans, NumPorts); 
    DT_ASSERT(DT_SUCCESS(Status));

    // Clear the entries in the look-up table
    for (i=0; i<NumPorts; i++)
    {
        pChanSdiXCfg = (DtDfSdiXCfgMgr_SdiXCfgChans*)DtVector_At(
                                                       pDf->m_pPortIdxToSdiXCfgsChans, i);
        *pChanSdiXCfg = EmptyChanSdiXCfg;
    }

    // Get the channel configuration from each SDIXCFG BC
    NumBcs = DtVectorBc_Size(pDf->m_pBcSdiXCfgs);
    for (i=0; i<NumBcs; i++)
    {
        Int  j, NumChanConfigs = 0;
        DtSdiXCfgChanConfig   XCfgChanConfig[DT_BC_SDIXCFG_MAX_NUM_LOG_CHAN];
        
        // Get the SDIXCFG BC
        pBcSdiXCfg = (DtBcSDIXCFG*)DtVectorBc_At(pDf->m_pBcSdiXCfgs, i);
        DT_ASSERT(pBcSdiXCfg != NULL);

        // Get the channel configuration from this BC
        Status =  DtBcSDIXCFG_GetConfig(pBcSdiXCfg, XCfgChanConfig,
                                         DT_BC_SDIXCFG_MAX_NUM_LOG_CHAN, &NumChanConfigs);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, SDIXCFGMGR, pDf, "ERROR: DtBcSDIXCFG_GetConfig failed");
            return Status;
        }

        // Update the look-up table with this information
        for (j=0; j<NumChanConfigs; j++)
        {
            Int  PortIndex = XCfgChanConfig[j].m_PortIndex;
            Int  LogChanNr = XCfgChanConfig[j].m_LogChanNum;
            Int  ChanType = XCfgChanConfig[j].m_ChanType;

            if (PortIndex<0 || PortIndex>=NumPorts)
            {
                // Skip  Invalid port index
                DtDbgOutDf(ERR, SDIXCFGMGR, pDf, "WARNING: unexpected portindex: %d",
                                                                               PortIndex);
                continue;
            }

            // Get look-up table entry for this port
            pChanSdiXCfg = (DtDfSdiXCfgMgr_SdiXCfgChans*)DtVector_At(
                                               pDf->m_pPortIdxToSdiXCfgsChans, PortIndex);
            DT_ASSERT(pChanSdiXCfg != NULL);

            // Update the look-up table entry
            switch (ChanType)
            {
            case DT_SDIXCFG_CHANTYPE_NOT_USED:
                break;
            case DT_SDIXCFG_CHANTYPE_RX_CHAN:
                pChanSdiXCfg->m_pRxChanBcSdiXCfg = pBcSdiXCfg;
                pChanSdiXCfg->m_RxChanLogChannel = LogChanNr;
                break;
            case DT_SDIXCFG_CHANTYPE_TX_CHAN:
                pChanSdiXCfg->m_pTxChanBcSdiXCfg = pBcSdiXCfg;
                pChanSdiXCfg->m_TxChanLogChannel = LogChanNr;
                break;
            case DT_SDIXCFG_CHANTYPE_TX_PLL:
                pChanSdiXCfg->m_pTxPllBcSdiXCfg = pBcSdiXCfg;
                pChanSdiXCfg->m_TxPllLogChannel = LogChanNr;
                break;
            default:
                DT_ASSERT(FALSE);
            }
        }
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSdiXCfgMgr_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSdiXCfgMgr_OnEnablePostChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfSdiXCfgMgr* pDf = (DtDfSdiXCfgMgr*)pDfBase;

    // Sanity checks
    DF_SDIXCFGMGR_DEFAULT_PRECONDITIONS(pDf);


    // If enable, update the look-up table
    if (Enable)
        Status = DtDfSdiXCfgMgr_InitPortIdxToSdiXCfgsChans(pDf);

    return Status;
}
