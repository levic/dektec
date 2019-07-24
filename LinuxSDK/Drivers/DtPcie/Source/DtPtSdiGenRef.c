// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPtSdiGenRef.c *#*#*#*#*#*#*#*#*#* (C) 2019 DekTec
//
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
#include "DtPtSdiGenRef.h"
#include "IoConfigCodes.h"
#include "DtAudioVideo.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtPtSdiGenRef implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
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
#define PT_SDIGENREF_DEFAULT_PRECONDITIONS(pPt)      \
    DT_ASSERT(pPt!=NULL && pPt->m_Size==sizeof(DtPtSdiGenRef))

// Helper macro to cast a DtDf* to aDtPtSdiGenRef*
#define PT_SDIGENREF      ((DtPtSdiGenRef*)pPt)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus DtPtSdiGenRef_Init(DtPt*);
static DtStatus DtPtSdiGenRef_CheckPrerequisites(DtPtSdiGenRef*);
static DtStatus DtPtSdiGenRef_SetIoConfigFinish(DtPt*, const DtExclAccessObject*);
static DtStatus DtPtSdiGenRef_SetIoConfigPrepare(DtPt*, const DtExclAccessObject*);
static DtStatus DtPtSdiGenRef_SetIoConfig(DtPt*, const DtCfIoConfigValue*, Int Num);
static DtStatus DtPtSdiGenRef_SetIoConfigGenRef(DtPtSdiGenRef*, const DtCfIoConfigValue*);
static DtStatus DtPtSdiGenRef_SetIoConfigIoStd(DtPtSdiGenRef*, const DtCfIoConfigValue*);


//=+=+=+=+=+=+=+=+=+=+=+=+=+DtPtSdiGenRef - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiGenRef_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtPtSdiGenRef_Close(DtPt*  pPt)
{
    PT_SDIGENREF_DEFAULT_PRECONDITIONS(pPt);

    // Let base function perform final clean-up
    DtPt_Close(pPt);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiGenRef_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtPtSdiGenRef* DtPtSdiGenRef_Open(DtCore* pCore, Int PortIndex, DtPortType Type)
{
    DtPtOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(PortIndex >= 0);

    DT_ASSERT(Type==DT_PORT_TYPE_SDIGENREF);

    // Init open parameters
    DT_PT_INIT_OPEN_PARAMS(OpenParams,DtPtSdiGenRef, DT_DF_SDIGENREF_NAME,
                                                                  PortIndex, Type, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtPtSdiGenRef_Close;
    OpenParams.m_InitFunc = DtPtSdiGenRef_Init;
    OpenParams.m_SetIoConfigFunc = DtPtSdiGenRef_SetIoConfig;
    OpenParams.m_SetIoConfigFinishFunc = DtPtSdiGenRef_SetIoConfigFinish;
    OpenParams.m_SetIoConfigPrepareFunc = DtPtSdiGenRef_SetIoConfigPrepare;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtPtSdiGenRef*)DtPt_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiGenRef_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtSdiGenRef_Init(DtPt* pPtBase)
{
   DtPtSdiGenRef* pPt = (DtPtSdiGenRef*)pPtBase;

    // Sanity check
    PT_SDIGENREF_DEFAULT_PRECONDITIONS(pPt);

    // Find the driver functions
    pPt->m_pDfVirtGenRef = (DtDfVirtGenRef*)DtPt_FindDf(pPtBase, 
                                                           DT_FUNC_TYPE_VIRTGENREF, NULL);
    // Find the block controllers
    pPt->m_pBcLmh1981 = (DtBcLMH1981*)DtPt_FindBc(pPtBase, DT_BLOCK_TYPE_LMH1981, NULL);

    // Check the driver functions and blockcontrollers that were found
    DT_RETURN_ON_ERROR(DtPtSdiGenRef_CheckPrerequisites(pPt));
    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiGenRef_CheckPrerequisites -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtSdiGenRef_CheckPrerequisites(DtPtSdiGenRef* pPt)
{
   Bool GenRefCap = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_GENREF);
   Bool VirtualCap = DtIoCapsHasCap(&pPt->m_IoCaps, DT_IOCAP_VIRTUAL);
   if (!GenRefCap)
       return DT_STATUS_CONFIG_ERROR;

    // LMH1981 or Virtual genref must be present
    if (pPt->m_pBcLmh1981==NULL && pPt->m_pDfVirtGenRef==NULL)
    {
        DtDbgOutPt(ERR, SDIGENREF, pPt, "ERROR: No GenRef DF/BC found");
        return DT_STATUS_FAIL;
    }
    if (pPt->m_pBcLmh1981!=NULL && pPt->m_pDfVirtGenRef!=NULL)
    {
        DtDbgOutPt(ERR, SDIGENREF, pPt, "ERROR: More than one GenRef DF/BC found");
        return DT_STATUS_FAIL;
    }
    if (pPt->m_pBcLmh1981==NULL && !VirtualCap)
    {
        DtDbgOutPt(ERR, SDIGENREF, pPt, "ERROR: LMH1981 not found");
        return DT_STATUS_FAIL;
    }
    if (pPt->m_pDfVirtGenRef==NULL && VirtualCap)
    {
        DtDbgOutPt(ERR, SDIGENREF, pPt, "ERROR: Virtual GenRef not found");
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiGenRef_SetIoConfigFinish -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtSdiGenRef_SetIoConfigFinish(DtPt* pPt, const DtExclAccessObject* pObject)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    PT_SDIGENREF_DEFAULT_PRECONDITIONS(pPt);

    // Release exclusive access to children
    Status = DtPt_ReleaseExclAccessChildren(pPt, pObject);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiGenRef_SetIoConfigPrepare -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtSdiGenRef_SetIoConfigPrepare(DtPt* pPt, const DtExclAccessObject* pObject)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    PT_SDIGENREF_DEFAULT_PRECONDITIONS(pPt);

    // Acquire exclusive access to children
    Status = DtPt_AcquireExclAccessChildren(pPt, pObject);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutPt(ERR, SDIGENREF, pPt, "ERROR: Cannot acquire exclusive access");
        // Failed
        return Status;
    }
    return Status;
}
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiGenRef_SetIoConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtSdiGenRef_SetIoConfig(DtPt* pPtBase, const DtCfIoConfigValue* pIoCfgs, 
                                                                            Int NumIoCfgs)
{
    Int  IoCfg=0;
    DtPtSdiGenRef* pPt = (DtPtSdiGenRef*)pPtBase;

    // Sanity check
    PT_SDIGENREF_DEFAULT_PRECONDITIONS(pPt);


    // Check whether expected IO-configs are there
    for (IoCfg=0; IoCfg<NumIoCfgs; IoCfg++)
    {
        switch (IoCfg)
        {
        case DT_IOCONFIG_GENREF:
        case DT_IOCONFIG_IOSTD:
            // Mandatory IO-configs
            if (pIoCfgs[IoCfg].m_Value == DT_IOCONFIG_NONE)
            {
                Char IoCfgName[IOCONFIG_NAME_MAX_SIZE];
                IoCfgName[0] = '\0';
                IoConfigNameGet(IoCfg, IoCfgName, sizeof(IoCfgName));
                DtDbgOutPt(ERR, SDIGENREF, pPt, "ERROR: Missing IO-Config: %s",
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
                DtDbgOutPt(ERR, SDIGENREF, pPt, "ERROR:Unsupported IO-Config: %s",
                                                                               IoCfgName);
                DT_ASSERT(FALSE);
                return DT_STATUS_NOT_SUPPORTED;
            }
            break;
        }
    }
    // Check whether all mandatory IO-Configs are there
    if (NumIoCfgs<=DT_IOCONFIG_IOSTD || NumIoCfgs<=DT_IOCONFIG_GENREF)
        return DT_STATUS_INVALID_PARAMETER;

    // Perform IO-Config
    DT_RETURN_ON_ERROR(DtPtSdiGenRef_SetIoConfigIoStd(pPt, &pIoCfgs[DT_IOCONFIG_IOSTD]));
    DT_RETURN_ON_ERROR(DtPtSdiGenRef_SetIoConfigGenRef(pPt, &pIoCfgs[DT_IOCONFIG_GENREF]));

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiGenRef_SetIoConfigIoStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtPtSdiGenRef_SetIoConfigIoStd(DtPtSdiGenRef* pPt,
                                                          const DtCfIoConfigValue* pIoCfg)
{
    Int  VidStd;
    DT_ASSERT(pIoCfg != NULL);

    // Convert IO-parameter
    VidStd = DtAvIoStd2VidStd(pIoCfg->m_Value, pIoCfg->m_SubValue);
    if (VidStd < 0)
    {
        DT_ASSERT(FALSE);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Set VideoStandard
    if (pPt->m_pDfVirtGenRef!=NULL)
        return DtDfVirtGenRef_SetGenRefVidStd(pPt->m_pDfVirtGenRef, VidStd);
    if (pPt->m_pBcLmh1981!=NULL)
        return DtBcLMH1981_SetGenRefVidStd(pPt->m_pBcLmh1981, VidStd);

    DT_ASSERT(FALSE);
    return DT_STATUS_FAIL;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtPtSdiGenRef_SetIoConfigGenRef -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtPtSdiGenRef_SetIoConfigGenRef(DtPtSdiGenRef* pPt,
                                                          const DtCfIoConfigValue* pIoCfg)
{
    Bool GenRefEnable = FALSE;
    DT_ASSERT(pIoCfg != NULL);

    // Convert IO-parameter
    switch (pIoCfg->m_Value)
    {
    case DT_IOCONFIG_FALSE: GenRefEnable = FALSE; break;
    case DT_IOCONFIG_TRUE: GenRefEnable = TRUE; break;
    default:  DT_ASSERT(FALSE); return DT_STATUS_INVALID_PARAMETER;
    }

    // Set GenRefEnable
    if (pPt->m_pDfVirtGenRef!=NULL)
        return DtDfVirtGenRef_SetGenRefEnable(pPt->m_pDfVirtGenRef, GenRefEnable);
    if (pPt->m_pBcLmh1981!=NULL)
        return DtBcLMH1981_SetGenRefEnable(pPt->m_pBcLmh1981, GenRefEnable);

    DT_ASSERT(FALSE);
    return DT_STATUS_FAIL;
}
