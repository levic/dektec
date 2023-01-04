// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPtModOutp.c *#*#*#*#*#*#*#*#*#*# (C) 2021 DekTec
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2021 DekTec Digital Video B.V.
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
#include "DtPtModOutp.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtPtModOutp implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
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
    DT_ASSERT(pPt!=NULL && pPt->m_Size==sizeof(DtPtModOutp))


// Helper macro to cast a DtDf* to aDtPtModOutp*
#define PT_ASISDIMON      ((DtPtModOutp*)pPt)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus DtPtModOutp_Init(DtPt*);
static DtStatus DtPtModOutp_OnCloseFile(DtPt*, DtFileObject*);
static DtStatus DtPtModOutp_Enable(DtPt*, Bool Enable);
static DtStatus DtPtModOutp_SetIoConfig(DtPt*, const DtCfIoConfigValue*, Int Num);
static DtStatus DtPtModOutp_SetIoConfigPrepare(DtPt*, const DtExclAccessObject*);
static DtStatus DtPtModOutp_SetIoConfigFinish(DtPt*, const DtExclAccessObject*);
static DtStatus DtPtModOutp_SetIoConfigIoDir(DtPtModOutp*, const DtCfIoConfigValue*);
static DtStatus DtPtModOutp_SetIoConfigIoStd(DtPtModOutp*, const DtCfIoConfigValue*);
static DtStatus DtPtModOutp_SetIoConfigIoRfClkSel(DtPtModOutp*, const DtCfIoConfigValue*);

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtPtModOutp - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtModOutp_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtPtModOutp_Close(DtPt*  pPt)
{
    PT_ASISDIMON_DEFAULT_PRECONDITIONS(pPt);

    // Let base function perform final clean-up
    DtPt_Close(pPt);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtModOutp_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtPtModOutp* DtPtModOutp_Open(DtCore* pCore, Int PortIndex, DtPortType Type)
{
    DtPtOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(PortIndex >= 0);

    DT_ASSERT(Type==DT_PORT_TYPE_MODOUTP);

    // Init open parameters
    DT_PT_INIT_OPEN_PARAMS(OpenParams,DtPtModOutp, DT_DF_MODOUTP_NAME,
                                                                  PortIndex, Type, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtPtModOutp_Close;
    OpenParams.m_InitFunc = DtPtModOutp_Init;
    OpenParams.m_EnableFunc = DtPtModOutp_Enable;
    OpenParams.m_OnCloseFileFunc = DtPtModOutp_OnCloseFile;
    OpenParams.m_SetIoConfigFunc = DtPtModOutp_SetIoConfig;
    OpenParams.m_SetIoConfigPrepareFunc = DtPtModOutp_SetIoConfigPrepare;
    OpenParams.m_SetIoConfigFinishFunc = DtPtModOutp_SetIoConfigFinish;
    
    // Use base function to allocate and perform standard initialization of function data
    return (DtPtModOutp*)DtPt_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtModOutp_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtModOutp_Init(DtPt* pPtBase)
{
    DtPtModOutp* pPt = (DtPtModOutp*)pPtBase;

    // Sanity check
    PT_ASISDIMON_DEFAULT_PRECONDITIONS(pPt);

    // -.-.-.-.-.-.-.-.-.-.-.-.-.- Find the general purpose IO -.-.-.-.-.-.-.-.-.-.-.-.-.-
    pPt->m_pBcIo2116 = (DtBcIO_2116*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_IO_2116, NULL);

    // Check whether all required driver functions are found
    if (pPt->m_pBcIo2116 == NULL)
        return DT_STATUS_FAIL;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtModOutp_Enable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Overrides the default implementation.
//
DtStatus DtPtModOutp_Enable(DtPt* pPt, Bool Enable)
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
        DtDbgOutPt(ERR, COMMON, pPt, "ERROR: failed to enable/disable children");
        return Status;
    }

    // Set new state
    Status = DtPt_SetOpState(pPt, Enable ? PT_STATE_ENABLED : PT_STATE_INITIALISED);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtModOutp_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtModOutp_OnCloseFile(DtPt* pPtBase, DtFileObject* pObject)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    PT_ASISDIMON_DEFAULT_PRECONDITIONS(pPtBase);

    // Let the base do the work
    Status = DtPt_OnCloseFile(pPtBase, pObject);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtModOutp_SetIoConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtModOutp_SetIoConfig(DtPt* pPtBase, const DtCfIoConfigValue* pIoCfgs, 
                                                                            Int NumIoCfgs)
{
    Int  IoCfg=0;
    DtPtModOutp* pPt = (DtPtModOutp*)pPtBase;

    // Sanity check
    PT_ASISDIMON_DEFAULT_PRECONDITIONS(pPt);


    // Check whether expected IO-configs are there
    for (IoCfg=0; IoCfg<NumIoCfgs; IoCfg++)
    {
        switch (IoCfg)
        {
        case DT_IOCONFIG_IODIR:
        case DT_IOCONFIG_IOSTD:
        case DT_IOCONFIG_RFCLKSEL:
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
    DT_RETURN_ON_ERROR(DtPtModOutp_SetIoConfigIoDir(pPt, &pIoCfgs[DT_IOCONFIG_IODIR]));

    // Perform IO-Config setting IOSTD. 
    if (DT_IOCONFIG_IOSTD >= NumIoCfgs)
        return DT_STATUS_INVALID_PARAMETER;
    DT_RETURN_ON_ERROR(DtPtModOutp_SetIoConfigIoStd(pPt, &pIoCfgs[DT_IOCONFIG_IOSTD]));

    // Perform IO-Config setting RFCLKSEL. 
    if (DT_IOCONFIG_RFCLKSEL >= NumIoCfgs)
        return DT_STATUS_INVALID_PARAMETER;
    DT_RETURN_ON_ERROR(DtPtModOutp_SetIoConfigIoRfClkSel(pPt,
                                                         &pIoCfgs[DT_IOCONFIG_RFCLKSEL]));

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtModOutp_SetIoConfigIoDir -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtModOutp_SetIoConfigIoDir(DtPtModOutp* pPt,
                                                          const DtCfIoConfigValue* pIoCfg)
{
    DT_ASSERT(pIoCfg != NULL);
    if (pIoCfg->m_Value != DT_IOCONFIG_OUTPUT)
    {
        DT_ASSERT(FALSE);
        return DT_STATUS_FAIL;
    }

    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtModOutp_SetIoConfigIoStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtModOutp_SetIoConfigIoStd(DtPtModOutp* pPt,
                                                          const DtCfIoConfigValue* pIoCfg)
{
    DT_ASSERT(pIoCfg != NULL);
    if (pIoCfg->m_Value != DT_IOCONFIG_MOD)
    {
        DT_ASSERT(FALSE);
        return DT_STATUS_FAIL;
    }

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtPtModOutp_SetIoConfigIoRfClkSel -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtModOutp_SetIoConfigIoRfClkSel(DtPtModOutp* pPt,
                                                          const DtCfIoConfigValue* pIoCfg)
{
    DT_ASSERT(pIoCfg!=NULL && pPt->m_pBcIo2116!=NULL);
    if (pIoCfg->m_Value == DT_IOCONFIG_RFCLKEXT)
        return DtBcIO_2116_Set10MhzClockSelect(pPt->m_pBcIo2116,
                                                           DT_IO_2116_CLKSELECT_EXTERNAL);
    if (pIoCfg->m_Value == DT_IOCONFIG_RFCLKINT)
        return DtBcIO_2116_Set10MhzClockSelect(pPt->m_pBcIo2116,
                                                           DT_IO_2116_CLKSELECT_INTERNAL);
    // else
    DT_ASSERT(FALSE);
    return DT_STATUS_FAIL;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtModOutp_SetIoConfigPrepare -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtModOutp_SetIoConfigPrepare(DtPt* pPt, const DtExclAccessObject* pObject)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    PT_ASISDIMON_DEFAULT_PRECONDITIONS(pPt);

    // Acquire exclusive access to children
    Status = DtPt_AcquireExclAccessChildren(pPt, pObject);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPt(ERR, ASISDIMON, pPt, "ERROR: Cannot acquire exclusive access");
        // Failed
        return Status;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtModOutp_SetIoConfigFinish -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtModOutp_SetIoConfigFinish(DtPt* pPt, const DtExclAccessObject* pObject)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    PT_ASISDIMON_DEFAULT_PRECONDITIONS(pPt);

    // Release exclusive access to children
    Status = DtPt_ReleaseExclAccessChildren(pPt, pObject);
    return Status;
}
