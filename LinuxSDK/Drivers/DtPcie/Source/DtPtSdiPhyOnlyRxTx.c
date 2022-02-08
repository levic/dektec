// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPtSdiPhyOnlyRxTx.c *#*#*#*#*#*#*#*#* (C) 2020 DekTec
//
//
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2020 DekTec Digital Video B.V.
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
#include "DtPtSdiPhyOnlyRxTx.h"
#include "IoConfigCodes.h"
#include "DtAudioVideo.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtPtSdiPhyOnlyRxTx implementation +=+=+=+=+=+=+=+=+=+=+=+=+=
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

// MACRO with default precondition checks for the DfPt function
#define PT_SDIPHYONLYRXTX_DEFAULT_PRECONDITIONS(pPt)      \
    DT_ASSERT(pPt!=NULL && pPt->m_Size==sizeof(DtPtSdiPhyOnlyRxTx))


// Helper macro to cast a DtDf* to aDtPtSdiPhyOnlyRxTx*
#define PT_SDIPHYONLYRXTX      ((DtPtSdiPhyOnlyRxTx*)pPt)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus DtPtSdiPhyOnlyRxTx_Init(DtPt*);
static DtStatus DtPtSdiPhyOnlyRxTx_CheckRxPrerequisites(DtPtSdiPhyOnlyRxTx*);
static DtStatus DtPtSdiPhyOnlyRxTx_CheckTxPrerequisites(DtPtSdiPhyOnlyRxTx*);
static DtStatus DtPtSdiPhyOnlyRxTx_OnCloseFile(DtPt*, DtFileObject*);
static DtStatus DtPtSdiPhyOnlyRxTx_Enable(DtPt*, Bool Enable);
static DtStatus DtPtSdiPhyOnlyRxTx_SetIoConfig(DtPt*, const DtCfIoConfigValue*, Int Num);
static DtStatus DtPtSdiPhyOnlyRxTx_SetIoConfigPrepare(DtPt*, const DtExclAccessObject*);
static DtStatus DtPtSdiPhyOnlyRxTx_SetIoConfigFinish(DtPt*, const DtExclAccessObject*);

static DtStatus DtPtSdiPhyOnlyRxTx_SetIoConfigIoDir(DtPtSdiPhyOnlyRxTx*, 
                                                                const DtCfIoConfigValue*);
static DtStatus DtPtSdiPhyOnlyRxTx_SetRxIoConfigIoStd(DtPtSdiPhyOnlyRxTx*, 
                                                                const DtCfIoConfigValue*);
static DtStatus DtPtSdiPhyOnlyRxTx_SetRxIoConfigGenLock(DtPtSdiPhyOnlyRxTx*, 
                                                                const DtCfIoConfigValue*);
static DtStatus  DtPtSdiPhyOnlyRxTx_SetTxIoConfigIoStd(DtPtSdiPhyOnlyRxTx*, 
                                              const DtCfIoConfigValue*, Bool DblyBufMode);
static DtStatus DtPtSdiPhyOnlyRxTx_SetTxIoConfigGenLock(DtPtSdiPhyOnlyRxTx*, 
                                                                const DtCfIoConfigValue*);
// =+=+=+=+=+=+=+=+=+=+=+=+ DtPtSdiPhyOnlyRxTx - Public functions +=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtPtSdiPhyOnlyRxTx_Close(DtPt*  pPt)
{
    PT_SDIPHYONLYRXTX_DEFAULT_PRECONDITIONS(pPt);

    // Let base function perform final clean-up
    DtPt_Close(pPt);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtPtSdiPhyOnlyRxTx* DtPtSdiPhyOnlyRxTx_Open(DtCore* pCore, Int PortIndex, DtPortType Type)
{
    DtPtOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(PortIndex >= 0);

    DT_ASSERT(Type==DT_PORT_TYPE_SDIPHYONLYRX ||Type==DT_PORT_TYPE_SDIPHYONLYTX 
                                                     ||Type==DT_PORT_TYPE_SDIPHYONLYRXTX);

    // Init open parameters
    DT_PT_INIT_OPEN_PARAMS(OpenParams,DtPtSdiPhyOnlyRxTx, DT_DF_SDIPHYONLYRXTX_NAME,
                                                                  PortIndex, Type, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtPtSdiPhyOnlyRxTx_Close;
    OpenParams.m_InitFunc = DtPtSdiPhyOnlyRxTx_Init;
    OpenParams.m_EnableFunc = DtPtSdiPhyOnlyRxTx_Enable;
    OpenParams.m_OnCloseFileFunc = DtPtSdiPhyOnlyRxTx_OnCloseFile;
    OpenParams.m_SetIoConfigFunc = DtPtSdiPhyOnlyRxTx_SetIoConfig;
    OpenParams.m_SetIoConfigPrepareFunc = DtPtSdiPhyOnlyRxTx_SetIoConfigPrepare;
    OpenParams.m_SetIoConfigFinishFunc = DtPtSdiPhyOnlyRxTx_SetIoConfigFinish;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtPtSdiPhyOnlyRxTx*)DtPt_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtSdiPhyOnlyRxTx_Init(DtPt* pPtBase)
{
    DtPtSdiPhyOnlyRxTx* pPt = (DtPtSdiPhyOnlyRxTx*)pPtBase;
    const char* TxDblBufRoleName = "TX_DBL_BUF";
 
    // Sanity check
    PT_SDIPHYONLYRXTX_DEFAULT_PRECONDITIONS(pPt);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Find the TX-driver functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
    pPt->m_pDfSdiTxPhy = (DtDfSdiTxPhy*)DtPt_FindDf(pPtBase, DT_FUNC_TYPE_SDITXPHY, NULL);

    // Find the TX-block controllers
    pPt->m_pBcSdiTxP = (DtBcSDITXP*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SDITXP, NULL);
    pPt->m_pBcSwitchTxDblBuf = (DtBcSWITCH*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SWITCH, 
                                                                        TxDblBufRoleName);
    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Find the RX-driver functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
    pPt->m_pDfSdiRx = (DtDfSdiRx*)DtPt_FindDf(pPtBase, DT_FUNC_TYPE_SDIRX, NULL);

    // Find the RX/TX driver functions
    pPt->m_pDfSpiCableDrvEq = (DtDfSpiCableDrvEq*)DtPt_FindDf(pPtBase, 
                                                        DT_FUNC_TYPE_SPICABLEDRVEQ, NULL);

    // Find the double-buffered switch
    pPt->m_pBcSwitchTxDblBuf = (DtBcSWITCH*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_SWITCH, 
                                                                        TxDblBufRoleName);

    // Check the driver functions and blockcontrollers that were found
    DT_RETURN_ON_ERROR(DtPtSdiPhyOnlyRxTx_CheckRxPrerequisites(pPt));
    DT_RETURN_ON_ERROR(DtPtSdiPhyOnlyRxTx_CheckTxPrerequisites(pPt));

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx_CheckRxPrerequisites -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtSdiPhyOnlyRxTx_CheckRxPrerequisites(DtPtSdiPhyOnlyRxTx* pPt)
{
   Bool InpCap = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_INPUT);

   if (!InpCap)
       return DT_STATUS_OK;

    // For for ASI/SDI the DfSdiRx  needs to be present
    if (pPt->m_pDfSdiRx == NULL)
    {
        DtDbgOutPt(ERR, SDIPHYONLYRXTX, pPt, "ERROR: SDIRX not found");
        return DT_STATUS_FAIL;
    }

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx_CheckTxPrerequisites -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtSdiPhyOnlyRxTx_CheckTxPrerequisites(DtPtSdiPhyOnlyRxTx* pPt)
{
   Bool OutpCap  = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_INTOUTPUT);
   Bool DblBufCap  = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_DBLBUF);

   if (!OutpCap)
       return DT_STATUS_OK;

    // A TXPHY needs to be present
    if (pPt->m_pDfSdiTxPhy==NULL)
    {
        DtDbgOutPt(ERR, SDIPHYONLYRXTX, pPt, "ERROR: SDITXPHY not found");
        return DT_STATUS_FAIL;
    }

    // In case of double-buffered output a buddy port selector switch is required
    if (DblBufCap && pPt->m_pBcSwitchTxDblBuf==NULL)
    {
        DtDbgOutPt(ERR, SDIPHYONLYRXTX, pPt, "ERROR: double-buffer switch not found");
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx_Enable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Overrides the default implementation.
//
DtStatus DtPtSdiPhyOnlyRxTx_Enable(DtPt* pPt, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;

    PT_SDIPHYONLYRXTX_DEFAULT_PRECONDITIONS(pPt);

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

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtSdiPhyOnlyRxTx_OnCloseFile(DtPt* pPtBase, DtFileObject* pObject)
{
     DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    PT_SDIPHYONLYRXTX_DEFAULT_PRECONDITIONS(pPtBase);

     // Let the base do the work
     Status = DtPt_OnCloseFile(pPtBase, pObject);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx_SetIoConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtSdiPhyOnlyRxTx_SetIoConfig(DtPt* pPtBase, const DtCfIoConfigValue* pIoCfgs, 
                                                                            Int NumIoCfgs)
{
    Int  IoCfg=0;
    DtPtSdiPhyOnlyRxTx* pPt = (DtPtSdiPhyOnlyRxTx*)pPtBase;
    Bool IsInput = FALSE;
    Bool IsDblBufOutput = FALSE;

    // Sanity check
    PT_SDIPHYONLYRXTX_DEFAULT_PRECONDITIONS(pPt);

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
                DtDbgOutPt(ERR, SDIPHYONLYRXTX, pPt, "ERROR: Missing IO-Config: %s",
                                                                               IoCfgName);
                DT_ASSERT(FALSE);
                return DT_STATUS_INVALID_PARAMETER;
            }
            break;
        case DT_IOCONFIG_GENLOCKED:
            // Optional IO-configs; will be tested later
            break;

        default:
            // Unsupported IO-configs
            if (pIoCfgs[IoCfg].m_Value != DT_IOCONFIG_NONE)
            {
                Char IoCfgName[IOCONFIG_NAME_MAX_SIZE];
                IoCfgName[0] = '\0';
                IoConfigNameGet(IoCfg, IoCfgName, sizeof(IoCfgName));
                DtDbgOutPt(ERR, SDIPHYONLYRXTX, pPt, "ERROR:Unsupported IO-Config: %s",
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
    DT_RETURN_ON_ERROR(DtPtSdiPhyOnlyRxTx_SetIoConfigIoDir(pPt, 
                                                            &pIoCfgs[DT_IOCONFIG_IODIR]));

    // Determine direction; input or output
    DT_ASSERT(pIoCfgs[DT_IOCONFIG_IODIR].m_Value==DT_IOCONFIG_INTINPUT
                            || pIoCfgs[DT_IOCONFIG_IODIR].m_Value==DT_IOCONFIG_INTOUTPUT);
    IsInput = (pIoCfgs[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_INTINPUT);
    IsDblBufOutput = (pIoCfgs[DT_IOCONFIG_IODIR].m_Value==DT_IOCONFIG_INTOUTPUT && 
                               pIoCfgs[DT_IOCONFIG_IODIR].m_SubValue==DT_IOCONFIG_DBLBUF);

    // Perform IO-Config setting IOSTD. Dependent on direction
    if (DT_IOCONFIG_IOSTD >= NumIoCfgs)
        return DT_STATUS_INVALID_PARAMETER;
    if (IsInput)
        DT_RETURN_ON_ERROR(DtPtSdiPhyOnlyRxTx_SetRxIoConfigIoStd(pPt, 
                                                            &pIoCfgs[DT_IOCONFIG_IOSTD]));
    else 
        DT_RETURN_ON_ERROR(DtPtSdiPhyOnlyRxTx_SetTxIoConfigIoStd(pPt,
                                            &pIoCfgs[DT_IOCONFIG_IOSTD], IsDblBufOutput));

    // Perform IO-Config setting GENLOCKED
    if (DT_IOCONFIG_GENLOCKED >= NumIoCfgs)
        return DT_STATUS_INVALID_PARAMETER;
    if (IsInput)
        DT_RETURN_ON_ERROR(DtPtSdiPhyOnlyRxTx_SetRxIoConfigGenLock(pPt, 
                                                        &pIoCfgs[DT_IOCONFIG_GENLOCKED]));
    else
        DT_RETURN_ON_ERROR(DtPtSdiPhyOnlyRxTx_SetTxIoConfigGenLock(pPt, 
                                                        &pIoCfgs[DT_IOCONFIG_GENLOCKED]));

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx_SetIoConfigIoDir -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtSdiPhyOnlyRxTx_SetIoConfigIoDir(DtPtSdiPhyOnlyRxTx* pPt,
                                                          const DtCfIoConfigValue* pIoCfg)
{
   Bool InpCap  = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_INTINPUT);
   Bool OutpCap  = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_INTOUTPUT);
   Bool DblBufCap  = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_DBLBUF);

    DT_ASSERT(pIoCfg != NULL);
    DT_ASSERT((OutpCap && pIoCfg->m_Value==DT_IOCONFIG_INTOUTPUT)
           || (InpCap && pIoCfg->m_Value==DT_IOCONFIG_INTINPUT 
                                            && pIoCfg->m_SubValue==DT_IOCONFIG_INTINPUT));

    if (pIoCfg->m_Value==DT_IOCONFIG_INTOUTPUT && pIoCfg->m_SubValue==DT_IOCONFIG_DBLBUF)
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
            ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSdiRx, FALSE);
        }
        // Disable all TX-driver functions/blocks that are not used in double buffered
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxP, FALSE);

        // Enable and set double buffered switch position
        DT_ASSERT(pPt->m_pBcSwitchTxDblBuf != NULL);
        ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchTxDblBuf, TRUE);
        DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchTxDblBuf,
                                                                        DblBufSelect, 0));
        DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchTxDblBuf,
                                                                    DT_BLOCK_OPMODE_RUN));

        // Enable all other TX-driver functions/blocks that are used in double buffered
        ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSdiTxPhy, TRUE);
        ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSpiCableDrvEq, TRUE);
        if (pPt->m_pDfSpiCableDrvEq != NULL)
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetDirection(pPt->m_pDfSpiCableDrvEq,
                                                             DT_DF_SPICABLEDRVEQ_DIR_TX));
    }
    else if (pIoCfg->m_Value == DT_IOCONFIG_INTOUTPUT)
    {
        DT_ASSERT(pIoCfg->m_SubValue == DT_IOCONFIG_INTOUTPUT);

        // Configured for output
        if (InpCap)
        { 
            // Disable all available RX driver-functions/blocks
            ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSdiRx, FALSE);
        }

        // Enable TX-driver functions/blocks that do not dependent on IOSTD or other
        // IO configs
        ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSdiTxPhy, TRUE);
        ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSpiCableDrvEq, TRUE);
        if (pPt->m_pBcSwitchTxDblBuf != NULL)
        { 
            // Pass own channel and enable double-buffer switch
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSwitchTxDblBuf, TRUE);
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetPosition(pPt->m_pBcSwitchTxDblBuf, 0, 0));
            DT_RETURN_ON_ERROR(DtBcSWITCH_SetOperationalMode(pPt->m_pBcSwitchTxDblBuf,
                                                                    DT_BLOCK_OPMODE_RUN));
        }
        // Set direction
        if (pPt->m_pDfSpiCableDrvEq != NULL)
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetDirection(pPt->m_pDfSpiCableDrvEq, 
                                                             DT_DF_SPICABLEDRVEQ_DIR_TX));
    }
    else if (pIoCfg->m_Value == DT_IOCONFIG_INTINPUT)
    {
        DT_ASSERT(pIoCfg->m_SubValue == DT_IOCONFIG_INTINPUT);
        // Configured for input
        if (OutpCap)
        { 
            // Disable all available TX driver-functions/blocks
            ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSdiTxPhy, FALSE);
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxP, FALSE);
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
        ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSdiRx, TRUE);
        ENABLE_DRIVERFUNC_RETURN_ON_ERR(pPt->m_pDfSpiCableDrvEq, TRUE);

        // Set direction
        if (pPt->m_pDfSpiCableDrvEq != NULL)
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetDirection(pPt->m_pDfSpiCableDrvEq, 
                                                             DT_DF_SPICABLEDRVEQ_DIR_RX));
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx_SetRxIoConfigIoStd -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtSdiPhyOnlyRxTx_SetRxIoConfigIoStd(DtPtSdiPhyOnlyRxTx* pPt,
                                                          const DtCfIoConfigValue* pIoCfg)
{
    Int CurOpMode, SdiRate;
    DT_ASSERT(pIoCfg != NULL);

    // ASI or SDI?
    if (pIoCfg->m_Value ==  DT_IOCONFIG_ASI)
    {
        // ASI
        // Set the receiver in ASI reception mode
        DT_RETURN_ON_ERROR(DtDfSdiRx_GetOperationalMode(pPt->m_pDfSdiRx, &CurOpMode));
        DT_RETURN_ON_ERROR(DtDfSdiRx_SetOperationalMode(pPt->m_pDfSdiRx, 
                                                                    DT_FUNC_OPMODE_IDLE));
        DT_RETURN_ON_ERROR(DtDfSdiRx_SetRxMode(pPt->m_pDfSdiRx, DT_SDIRX_RXMODE_ASI));
        DT_RETURN_ON_ERROR(DtDfSdiRx_SetOperationalMode(pPt->m_pDfSdiRx, CurOpMode));
    } 
    else
    {
        // SDI
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
        DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetSdiRate(pPt->m_pDfSpiCableDrvEq, 
                                                                 DT_DRV_SDIRATE_UNKNOWN));
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx_SetRxIoConfigGenLock -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtSdiPhyOnlyRxTx_SetRxIoConfigGenLock(DtPtSdiPhyOnlyRxTx* pPt, 
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

// .-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx_SetTxIoConfigIoStd -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtSdiPhyOnlyRxTx_SetTxIoConfigIoStd(DtPtSdiPhyOnlyRxTx* pPt,
                                        const DtCfIoConfigValue* pIoCfg, Bool DblyBufMode)
{
    // In double buffered mode the SDITXP is and stays disabled
    Bool  LevelA2BEnable = FALSE;
    Int  SdiRate, VidStd;
    Int  CurOpMode;
    DT_ASSERT(pIoCfg != NULL);

    // ASI or SDI?
    if (pIoCfg->m_Value == DT_IOCONFIG_ASI)
    {
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ASI -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        // Disable the SDI protocol block controller
        if (!DblyBufMode)
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxP, FALSE);

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
        // If we have SPI-cable driver, set SDI-rate to SD
        if (pPt->m_pDfSpiCableDrvEq != NULL)
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetSdiRate(pPt->m_pDfSpiCableDrvEq, 
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

        // Enable the SDI protocol block controller
        if (!DblyBufMode)
            ENABLE_BLOCKCTRL_RETURN_ON_ERR(pPt->m_pBcSdiTxP, TRUE);

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
        if (!DblyBufMode && pPt->m_pBcSdiTxP != NULL)
            DT_RETURN_ON_ERROR(DtBcSDITXP_SetSdiRateAndLvlA2BEna(pPt->m_pBcSdiTxP,
                                                           SdiRate, (Int)LevelA2BEnable));

        // If we have SPI-cable driver, set SDI-rate
        if (pPt->m_pDfSpiCableDrvEq != NULL)
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetSdiRate(pPt->m_pDfSpiCableDrvEq, 
                                                                                SdiRate));
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx_SetTxIoConfigGenLock -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtSdiPhyOnlyRxTx_SetTxIoConfigGenLock(DtPtSdiPhyOnlyRxTx* pPt, 
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

// .-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx_SetIoConfigPrepare -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtSdiPhyOnlyRxTx_SetIoConfigPrepare(DtPt* pPtBase, 
                                                        const DtExclAccessObject* pObject)
{
    DtStatus  Status=DT_STATUS_OK, TempStatus=DT_STATUS_OK;
    DtPtSdiPhyOnlyRxTx* pPt = (DtPtSdiPhyOnlyRxTx*)pPtBase;
    Int  OpMode;

    // Sanity check
    PT_SDIPHYONLYRXTX_DEFAULT_PRECONDITIONS(pPt);

    // Acquire exclusive access to children
    Status = DtPt_AcquireExclAccessChildren(pPtBase, pObject);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPt(ERR, SDIPHYONLYRXTX, pPt, "ERROR: Cannot acquire exclusive access");
        // Failed
        return Status;
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

    if (DT_SUCCESS(Status) && pPt->m_pBcSdiTxP!=NULL)
    {
        TempStatus = DtBcSDITXP_GetOperationalMode(pPt->m_pBcSdiTxP, &OpMode);
        if (TempStatus!=DT_STATUS_NOT_ENABLED 
                             && (!DT_SUCCESS(TempStatus) || OpMode!=DT_BLOCK_OPMODE_IDLE))
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
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPt(ERR, SDIPHYONLYRXTX, pPt, "ERROR: Children not in IDLE");

        // Failed; Now release exclusive access to children
         DtPt_ReleaseExclAccessChildren(pPtBase, pObject);
    }
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiPhyOnlyRxTx_SetIoConfigFinish -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtSdiPhyOnlyRxTx_SetIoConfigFinish(DtPt* pPt, const DtExclAccessObject* pObject)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    PT_SDIPHYONLYRXTX_DEFAULT_PRECONDITIONS(pPt);

    // Release exclusive access to children
    Status = DtPt_ReleaseExclAccessChildren(pPt, pObject);
    return Status;
}
