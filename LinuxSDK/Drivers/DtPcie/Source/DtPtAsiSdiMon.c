// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPtAsiSdiMon.c *#*#*#*#*#*#*#*#*#* (C) 2019 DekTec
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2019 DekTec Digital Video B.V.
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
#include "DtPtAsiSdiMon.h"
#include "IoConfigCodes.h"
#include "DtAudioVideo.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtPtAsiSdiMon implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
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
#define PT_ASISDIMON_DEFAULT_PRECONDITIONS(pPt)      \
    DT_ASSERT(pPt!=NULL && pPt->m_Size==sizeof(DtPtAsiSdiMon))


// Helper macro to cast a DtDf* to aDtPtAsiSdiMon*
#define PT_ASISDIMON      ((DtPtAsiSdiMon*)pPt)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus DtPtAsiSdiMon_Init(DtPt*);
static DtStatus DtPtAsiSdiMon_OnCloseFile(DtPt*, DtFileObject*);
static DtStatus DtPtAsiSdiMon_Enable(DtPt*, Bool Enable);
static DtStatus DtPtAsiSdiMon_SetIoConfig(DtPt*, const DtCfIoConfigValue*, Int Num);
static DtStatus DtPtAsiSdiMon_SetIoConfigPrepare(DtPt*, const DtExclAccessObject*);
static DtStatus DtPtAsiSdiMon_SetIoConfigFinish(DtPt*, const DtExclAccessObject*);
static DtStatus DtPtAsiSdiMon_SetIoConfigIoDir(DtPtAsiSdiMon*, const DtCfIoConfigValue*);
static DtStatus DtPtAsiSdiMon_SetIoConfigIoStd(DtPtAsiSdiMon*, const DtCfIoConfigValue*);

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtPtAsiSdiMon - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiMon_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtPtAsiSdiMon_Close(DtPt*  pPt)
{
    PT_ASISDIMON_DEFAULT_PRECONDITIONS(pPt);

    // Let base function perform final clean-up
    DtPt_Close(pPt);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiMon_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtPtAsiSdiMon* DtPtAsiSdiMon_Open(DtCore* pCore, Int PortIndex, DtPortType Type)
{
    DtPtOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(PortIndex >= 0);

    DT_ASSERT(Type==DT_PORT_TYPE_ASISDIRX ||Type==DT_PORT_TYPE_ASISDITX 
                                                         ||Type==DT_PORT_TYPE_ASISDIMON);

    // Init open parameters
    DT_PT_INIT_OPEN_PARAMS(OpenParams,DtPtAsiSdiMon, DT_DF_ASISDIMON_NAME,
                                                                  PortIndex, Type, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtPtAsiSdiMon_Close;
    OpenParams.m_InitFunc = DtPtAsiSdiMon_Init;
    OpenParams.m_EnableFunc = DtPtAsiSdiMon_Enable;
    OpenParams.m_OnCloseFileFunc = DtPtAsiSdiMon_OnCloseFile;
    OpenParams.m_SetIoConfigFunc = DtPtAsiSdiMon_SetIoConfig;
    OpenParams.m_SetIoConfigPrepareFunc = DtPtAsiSdiMon_SetIoConfigPrepare;
    OpenParams.m_SetIoConfigFinishFunc = DtPtAsiSdiMon_SetIoConfigFinish;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtPtAsiSdiMon*)DtPt_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiMon_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtAsiSdiMon_Init(DtPt* pPtBase)
{
   DtPtAsiSdiMon* pPt = (DtPtAsiSdiMon*)pPtBase;

    // Sanity check
    PT_ASISDIMON_DEFAULT_PRECONDITIONS(pPt);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Find the TX-driver functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
    pPt->m_pDfSdiTxPhy = (DtDfSdiTxPhy*)DtPt_FindDf(pPtBase, 
                                                           DT_FUNC_TYPE_SDITXPHY, NULL);
    pPt->m_pDfSpiCableDrvEq = (DtDfSpiCableDrvEq*)DtPt_FindDf(pPtBase, 
                                                        DT_FUNC_TYPE_SPICABLEDRVEQ, NULL);

    // Check whether all required driver functions are found
    if (pPt->m_pDfSdiTxPhy==NULL || pPt->m_pDfSpiCableDrvEq==NULL)
        return DT_STATUS_FAIL;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiMon_Enable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Overrides the default implementation.
//
DtStatus DtPtAsiSdiMon_Enable(DtPt* pPt, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;

    PT_ASISDIMON_DEFAULT_PRECONDITIONS(pPt);

    // Will the state change??
    if ((pPt->m_OpState==PT_STATE_ENABLED && Enable) 
                                     || (pPt->m_OpState==PT_STATE_INITIALISED && !Enable))
        return DT_STATUS_OK;

    // Let the base enable/disable the children
    Status = DtPt_EnableChildren(pPt, Enable);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPt(ERR, COMMON, pPt, "ERROR: failed to enable/disbale children");
        return Status;
    }

    // Set new state
    Status = DtPt_SetOpState(pPt, Enable ? PT_STATE_ENABLED : PT_STATE_INITIALISED);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiMon_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtAsiSdiMon_OnCloseFile(DtPt* pPtBase, DtFileObject* pObject)
{
     DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    PT_ASISDIMON_DEFAULT_PRECONDITIONS(pPtBase);

     // Let the base do the work
     Status = DtPt_OnCloseFile(pPtBase, pObject);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiMon_SetIoConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtAsiSdiMon_SetIoConfig(DtPt* pPtBase, const DtCfIoConfigValue* pIoCfgs, 
                                                                            Int NumIoCfgs)
{
    Int  IoCfg=0;
    DtPtAsiSdiMon* pPt = (DtPtAsiSdiMon*)pPtBase;

    // Sanity check
    PT_ASISDIMON_DEFAULT_PRECONDITIONS(pPt);


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
                DtDbgOutPt(ERR, ASISDIMON, pPt, "ERROR: Missing IO-Config: %s",
                                                                               IoCfgName);
                DT_ASSERT(FALSE);
                return DT_STATUS_INVALID_PARAMETER;
            }
            break;

        case DT_IOCONFIG_GENLOCKED:
            break;

        default:
            // Unsupported IO-configs
            if (pIoCfgs[IoCfg].m_Value != DT_IOCONFIG_NONE)
            {
                Char IoCfgName[IOCONFIG_NAME_MAX_SIZE];
                IoCfgName[0] = '\0';
                IoConfigNameGet(IoCfg, IoCfgName, sizeof(IoCfgName));
                DtDbgOutPt(ERR, ASISDIMON, pPt, "ERROR:Unsupported IO-Config: %s",
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
    DT_RETURN_ON_ERROR(DtPtAsiSdiMon_SetIoConfigIoDir(pPt, &pIoCfgs[DT_IOCONFIG_IODIR]));

    // Perform IO-Config setting IOSTD. 
    if (DT_IOCONFIG_IOSTD >= NumIoCfgs)
        return DT_STATUS_INVALID_PARAMETER;
    DT_RETURN_ON_ERROR(DtPtAsiSdiMon_SetIoConfigIoStd(pPt, &pIoCfgs[DT_IOCONFIG_IOSTD]));

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiMon_SetIoConfigIoDir -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtAsiSdiMon_SetIoConfigIoDir(DtPtAsiSdiMon* pPt,
                                                          const DtCfIoConfigValue* pIoCfg)
{
    DT_ASSERT(pIoCfg != NULL);
    if (pIoCfg->m_Value != DT_IOCONFIG_MONITOR)
    {
        DT_ASSERT(FALSE);
        return DT_STATUS_FAIL;
    }

    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiMon_SetIoConfigIoStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtAsiSdiMon_SetIoConfigIoStd(DtPtAsiSdiMon* pPt,
                                                          const DtCfIoConfigValue* pIoCfg)
{
    Int  SdiRate, VidStd;
    Int  CurOpMode;
    DT_ASSERT(pIoCfg != NULL);

    // ASI or SDI?
    if (pIoCfg->m_Value == DT_IOCONFIG_ASI)
    {
        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ASI -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
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

        // If we have SPI-cable driver, set SDI-rate
        if (pPt->m_pDfSpiCableDrvEq != NULL)
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetTxSdiRate(pPt->m_pDfSpiCableDrvEq, 
                                                                                SdiRate));
    }
    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiMon_SetIoConfigPrepare -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtAsiSdiMon_SetIoConfigPrepare(DtPt* pPtBase, 
                                                        const DtExclAccessObject* pObject)
{
    DtStatus  Status=DT_STATUS_OK, TempStatus=DT_STATUS_OK;
    DtPtAsiSdiMon* pPt = (DtPtAsiSdiMon*)pPtBase;
    Int  OpMode;

    // Sanity check
    PT_ASISDIMON_DEFAULT_PRECONDITIONS(pPt);

    // Acquire exclusive access to children
    Status = DtPt_AcquireExclAccessChildren(pPtBase, pObject);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPt(ERR, ASISDIMON, pPt, "ERROR: Cannot acquire exclusive access");
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

    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPt(ERR, ASISDIMON, pPt, "ERROR: Children not in IDLE");

        // Failed; Now release exclusive access to children
         DtPt_ReleaseExclAccessChildren(pPtBase, pObject);
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtAsiSdiMon_SetIoConfigFinish -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtAsiSdiMon_SetIoConfigFinish(DtPt* pPt, const DtExclAccessObject* pObject)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    PT_ASISDIMON_DEFAULT_PRECONDITIONS(pPt);

    // Release exclusive access to children
    Status = DtPt_ReleaseExclAccessChildren(pPt, pObject);
    return Status;
}
