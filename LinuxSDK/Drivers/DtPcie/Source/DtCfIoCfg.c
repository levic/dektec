// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCfIoCfg.c *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtCfIoCfg.h"
#include "DtPt.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfIoCfg implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtCfIoCfg function
#define CF_IOCFG_DEFAULT_PRECONDITIONS(pCf)      \
    DT_ASSERT(pCf!=NULL && pCf->m_Size==sizeof(DtCfIoCfg))

// Helper macro to cast a DtDf* to a DtCfIoCfg*
#define CF_IOCFG      ((DtCfIoCfg*)pCf)

// ParXtra names
static const char*   IoParXtraNames[DT_MAX_PARXTRA_COUNT] = {
        "IoParXtra0", 
        "IoParXtra1", 
        "IoParXtra2", 
        "IoParXtra3" };


//  Structure for storing necessary updates
typedef struct _DtCfIoCfgPortUpdates
{
    Bool  m_pConfigValueUpdateNeeded[DT_IOCONFIG_COUNT];   // IO-cfg value updates
    Bool  m_PortUpdateNeeded;                              // IO-cfg port update needed
}DtCfIoCfgPortUpdates;



//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtCfIoCfg_Init(DtDf*);
static DtStatus  DtCfIoCfg_AcquireExclAccess(DtCfIoCfg*);
static DtStatus  DtCfIoCfg_ReleaseExclAccess(DtCfIoCfg*);
static DtStatus  DtCfIoCfg_ApplyConfig(DtCfIoCfg*, const DtExclAccessObject* ,
                                              DtCfIoCfgPortConfig*, DtCfIoCfgPortUpdates*,
                                              Bool SaveToNonVolatileStorage);
static DtStatus  DtCfIoCfg_ApplyConfigPrepare(DtCfIoCfg*, const DtExclAccessObject*,
                                                     DtCfIoCfgPortUpdates*, Int NumPorts);
static DtStatus  DtCfIoCfg_ApplyConfigFinish(DtCfIoCfg*, const DtExclAccessObject*,
                                                     DtCfIoCfgPortUpdates*, Int NumPorts);
static DtStatus  DtCfIoCfg_ClearConfig(DtCfIoCfg* , DtCfIoCfgPortConfig*);
static DtStatus  DtCfIoCfg_GetCachedConfig(DtCfIoCfg* , DtCfIoCfgPortConfig*);
static DtStatus  DtCfIoCfg_GetStoredConfig(DtCfIoCfg*, DtCfIoCfgPortConfig*);
static DtStatus  DtCfIoCfg_SetDefaultConfig(DtCfIoCfg* , DtCfIoCfgPortConfig*);
static DtStatus  DtCfIoCfg_ValidateConfig(DtCfIoCfg*, DtCfIoCfgPortConfig*);

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfIoCfg - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtCfIoCfg_Close(DtDf*  pCf)
{
    // Sanity checks
    CF_IOCFG_DEFAULT_PRECONDITIONS(pCf);

    // Free the IO-config settings buffer
    if (CF_IOCFG->m_pPortConfigs != NULL)
    {
        DtMemFreePool(CF_IOCFG->m_pPortConfigs, DF_TAG);
        CF_IOCFG->m_pPortConfigs = NULL;
    }

    // Let base function perform final clean-up
    DtDf_Close(pCf);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtCfIoCfg*  DtCfIoCfg_Open(DtCore*  pCore, const char*  pRole, Int  Instance, 
                                                              Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    
    // Init open parameters
    DT_CF_IOCFG_INIT_ID(Id, pRole, Instance, Uuid);
    DT_CF_INIT_OPEN_PARAMS(OpenParams, DtCfIoCfg, Id, DT_FUNC_TYPE_CF_IOCFG,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtCfIoCfg_Close;
    OpenParams.m_InitFunc = DtCfIoCfg_Init;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtCfIoCfg*)DtDf_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_Get -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfIoCfg_Get(
    DtCfIoCfg*  pCf, 
    const DtExclAccessObject*  pExclAccessObj,
    DtIoConfig*  pCfgs, 
    Int  Count)
{
    Int i;
    DtStatus Status = DT_STATUS_OK;

    // Sanity checks
    CF_IOCFG_DEFAULT_PRECONDITIONS(pCf);

    // Acquire exclusive access
    Status = DtCfIoCfg_AcquireExclAccess(pCf);
    DT_ASSERT(DT_SUCCESS(Status));

    for (i=0; i<Count; i++)
    {
        Int IoCfg = pCfgs[i].m_Group;
        Int PortIdx = pCfgs[i].m_PortIndex;
        Int j;
        // Check limits
        if (IoCfg<0 || IoCfg>=DT_IOCONFIG_COUNT || PortIdx<0 || PortIdx>=pCf->m_NumPorts)
        { 
            Status = DT_STATUS_INVALID_PARAMETER;
            break;
        }
        // Copy IO-config setting
        pCfgs[i].m_Value = pCf->m_pPortConfigs[PortIdx].m_CfgValue[IoCfg].m_Value;
        pCfgs[i].m_SubValue = pCf->m_pPortConfigs[PortIdx].m_CfgValue[IoCfg].m_SubValue;
        for (j=0; j<DT_MAX_PARXTRA_COUNT; j++)
        { 
            pCfgs[i].m_ParXtra[j] =
                              pCf->m_pPortConfigs[PortIdx].m_CfgValue[IoCfg].m_ParXtra[j];
        }
    }

    // Release exclusive access
    Status = DtCfIoCfg_ReleaseExclAccess(pCf);
    DT_ASSERT(DT_SUCCESS(Status));

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_Set -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfIoCfg_Set(
    DtCfIoCfg*  pCf, 
    const DtExclAccessObject*  pExclAccessObj,
    const DtIoConfig*  pCfgs,
    Int  Count)
{
    DtStatus  Status=DT_STATUS_OK, StatusTemp=DT_STATUS_OK;
    DtCfIoCfgPortConfig*  pNewIoConfig;
    DtCfIoCfgPortUpdates*  pConfigUpdates;      // Updates for all ports
    DtCfIoCfgPortUpdates*  pPortUpdate;         // Updates for a single port
    Int  i;
    UInt  AllocSize;
    Int NumPorts = pCf->m_NumPorts;

    // Sanity checks
    CF_IOCFG_DEFAULT_PRECONDITIONS(pCf);

    // Allocate memory for configuration updates
    AllocSize = sizeof(DtCfIoCfgPortUpdates) * NumPorts;
    pConfigUpdates = (DtCfIoCfgPortUpdates*)DtMemAllocPool(DtPoolNonPaged, AllocSize,
                                                                                  DF_TAG);
    if (pConfigUpdates == NULL)
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: failed to allocate memory (size=%d)",
                                                                               AllocSize);
        return DT_STATUS_OUT_OF_MEMORY;
    }
    DtMemZero(pConfigUpdates, AllocSize);


    // Allocate memory for IO-config settings
    AllocSize = NumPorts * sizeof(DtCfIoCfgPortConfig);
    pNewIoConfig = DtMemAllocPool(DtPoolNonPaged, AllocSize, DF_TAG);
    if (pNewIoConfig == NULL)
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: failed to allocate memory (size=%d)",
                                                                               AllocSize);
        DtMemFreePool(pConfigUpdates, DF_TAG);
        return DT_STATUS_OUT_OF_MEMORY;
    }

    // Acquire exclusive access
    Status = DtCfIoCfg_AcquireExclAccess(pCf);
    DT_ASSERT(DT_SUCCESS(Status));

    // Get cached IO-configuratoin
    Status = DtCfIoCfg_GetCachedConfig(pCf, pNewIoConfig);

    // Merge new IoConfigs
    for (i=0; i<Count && DT_SUCCESS(Status); i++)
    {
        DtCfIoConfigValue*  pPortIoCfg;
        Int IoCfg = pCfgs[i].m_Group;
        Int PortIdx = pCfgs[i].m_PortIndex;
        Int j;
        Bool IsChanged = FALSE;

        // Check limits
        if (IoCfg<0 || IoCfg>=DT_IOCONFIG_COUNT || PortIdx<0 || PortIdx>=NumPorts)
        {
            DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: Invalid IO-config (%d) for port: %d", 
                                                                        IoCfg, PortIdx+1);

            Status = DT_STATUS_INVALID_PARAMETER;
            break;
        }
        pPortIoCfg =  pNewIoConfig[PortIdx].m_CfgValue;
        pPortUpdate = &pConfigUpdates[PortIdx];

        // Config changed?
        IsChanged = pPortIoCfg[IoCfg].m_Value != pCfgs[i].m_Value;
        IsChanged |= pPortIoCfg[IoCfg].m_SubValue != pCfgs[i].m_SubValue;
        for (j=0; j<DT_MAX_PARXTRA_COUNT; j++)
            IsChanged |= pPortIoCfg[IoCfg].m_ParXtra[j] != pCfgs[i].m_ParXtra[j];
        if (IsChanged)
        {
            pPortUpdate->m_PortUpdateNeeded = TRUE;
            pPortUpdate->m_pConfigValueUpdateNeeded[IoCfg] = TRUE;

            // Copy setting
            pPortIoCfg[IoCfg].m_Value = pCfgs[i].m_Value;
            pPortIoCfg[IoCfg].m_SubValue = pCfgs[i].m_SubValue;
            for (j=0; j<DT_MAX_PARXTRA_COUNT; j++)
            {
                pPortIoCfg[IoCfg].m_ParXtra[j] = pCfgs[i].m_ParXtra[j];
            }
        }
    }

    // Validate
    if (DT_SUCCESS(Status))
    {
        Status = DtCfIoCfg_ValidateConfig(pCf, pNewIoConfig);
        if (!DT_SUCCESS(Status))
            DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: IO-config validation failed");
    }

    // Apply settings
    if (DT_SUCCESS(Status))
    {
        Status = DtCfIoCfg_ApplyConfig(pCf, pExclAccessObj, pNewIoConfig, pConfigUpdates, 
                                                                                    TRUE);
        if (!DT_SUCCESS(Status))
            DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: IO-config apply failed");
    }

     // Release exclusive access
    StatusTemp = DtCfIoCfg_ReleaseExclAccess(pCf);
    DT_ASSERT(DT_SUCCESS(StatusTemp));
    
    if (DT_SUCCESS(Status))
    {
        // Send IOCONFIG events
        DtDriverEvent IoCfgEvent;
        IoCfgEvent.m_EventType = DT_DRV_EVENT_TYPE_IOCONFIG;
        IoCfgEvent.m_EventValue2 = 0;
        for (i=0; i<NumPorts; i++)
        {
            if (!pConfigUpdates[i].m_PortUpdateNeeded)
                continue;
            IoCfgEvent.m_EventValue1 = i;
            DtCore_EVENTS_Set(pCf->m_pCore, IoCfgEvent , FALSE);
        }
    }

    DtMemFreePool(pConfigUpdates, DF_TAG);
    DtMemFreePool(pNewIoConfig, DF_TAG);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_Restore -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfIoCfg_Restore(DtCfIoCfg*  pCf)
{
    DtStatus  Result;
    UInt  AllocSize;
    Int  PortIndex, IoConfig;
    Int  NumPorts = pCf->m_NumPorts;
    DtCfIoCfgPortConfig*  pIoConfig;
    DtCfIoCfgPortUpdates* pConfigUpdates;
    DtExclAccessObject  ExclAccessObj;

    // Sanity checks
    CF_IOCFG_DEFAULT_PRECONDITIONS(pCf);

    if (NumPorts == 0)
        return DT_STATUS_OK;

    // Start with default configuration
    Result = DtCfIoCfg_SetDefaultConfig(pCf, pCf->m_pPortConfigs);
    if (!DT_SUCCESS(Result))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: DtCfIoCfg_SetDefaultConfig failed");
        return Result;
    }

    // Allocate memory for configuration updates
    AllocSize = sizeof(DtCfIoCfgPortUpdates) * NumPorts;
    pConfigUpdates = (DtCfIoCfgPortUpdates*)DtMemAllocPool(DtPoolNonPaged, AllocSize,
                                                                                  DF_TAG);
    if (pConfigUpdates == NULL)
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: failed to allocate memory (size=%d)",
                                                                               AllocSize);
        return DT_STATUS_OUT_OF_MEMORY;
    }
    DtMemZero(pConfigUpdates, AllocSize);

    // Allocate memory for new IO-config settings
    AllocSize = NumPorts * sizeof(DtCfIoCfgPortConfig);
    pIoConfig = DtMemAllocPool(DtPoolNonPaged, AllocSize, DF_TAG);
    if (pIoConfig == NULL)
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: failed to allocate memory (size=%d)",
                                                                               AllocSize);
        DtMemFreePool(pConfigUpdates, DF_TAG);
        return DT_STATUS_OUT_OF_MEMORY;
    }

    // Load cache (initial cache holds the defaults which are not already applied)
    Result = DtCfIoCfg_GetCachedConfig(pCf, pIoConfig);
    if (!DT_SUCCESS(Result))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: DtCfIoCfg_GetCachedConfig failed");
        DtMemFreePool(pConfigUpdates, DF_TAG);
        DtMemFreePool(pIoConfig, DF_TAG);
        return Result;
    }

    // Load stored configuratoni
    Result = DtCfIoCfg_GetStoredConfig(pCf, pIoConfig);
    if (!DT_SUCCESS(Result))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: DtCfIoCfg_GetStoredConfig failed");
        DtMemFreePool(pConfigUpdates, DF_TAG);
        DtMemFreePool(pIoConfig, DF_TAG);
        return Result;
    }

    // Validate configuration after registry manipulations are done
    Result = DtCfIoCfg_ValidateConfig(pCf, pIoConfig);
    if (!DT_SUCCESS(Result))
    {
        DtDbgOut(ERR, IOCONFIG, "Configuration invalid. Apply initial configuration");
        // Reload cache (initial cache holds the defaults which are not already
        // applied)
        Result = DtCfIoCfg_GetCachedConfig(pCf, pIoConfig);
        if (!DT_SUCCESS(Result))
        {
            DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: DtCfIoCfg_GetCachedConfig failed");
            DtMemFreePool(pConfigUpdates, DF_TAG);
            DtMemFreePool(pIoConfig, DF_TAG);
            return Result;
        }

        // Delete the registry settings for this device.
        if (!DT_SUCCESS(DtCore_SETTINGS_Delete(pCf->m_pCore, NumPorts)))
            DtDbgOut(ERR, IOCONFIG, "Error deleting registry key");
    }

    // Set UpdateNeeded flag for all IO configurations during init...
    for (PortIndex=0; PortIndex<NumPorts; PortIndex++)
    {
        DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;
        DtCfIoCfgPortUpdates* pPortUpdate = &pConfigUpdates[PortIndex];
        pPortUpdate->m_PortUpdateNeeded = TRUE;
        for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
        {
            // Skip IO configs not applicable to the current port type
            if (pPortIoCfg[IoConfig].m_Value == DT_IOCONFIG_NONE)
                continue;
            pPortUpdate->m_pConfigValueUpdateNeeded[IoConfig] = TRUE;
        }
    }

    // Apply (without saving everything to the non volatile storage)
     // Init the exclusive access object as a DF object
    ExclAccessObj.m_Type = DT_EXCL_OBJECT_TYPE_IS_DF;
    ExclAccessObj.m_Owner.m_pDf = (DtDf*)pCf;
    Result = DtCfIoCfg_ApplyConfig(pCf, &ExclAccessObj, pIoConfig, pConfigUpdates, FALSE);
    if (!DT_SUCCESS(Result))
            DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: DtCfIoCfg_ApplyConfig failed");

    DtMemFreePool(pConfigUpdates, DF_TAG);
    DtMemFreePool(pIoConfig, DF_TAG);
    return Result;
}
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfIoCfg - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfIoCfg_Init(DtDf*  pCf)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt  AllocSize;

    // Sanity checks
    CF_IOCFG_DEFAULT_PRECONDITIONS(pCf);


    DT_ASSERT(CF_IOCFG->m_pPortConfigs == NULL);

    CF_IOCFG->m_NumPorts = DtCore_DEVICE_GetNumPorts(pCf->m_pCore);

    // Allocate memory for IO-config settings
    AllocSize = CF_IOCFG->m_NumPorts * sizeof(DtCfIoCfgPortConfig);
    CF_IOCFG->m_pPortConfigs = DtMemAllocPool(DtPoolNonPaged, AllocSize, DF_TAG);
    if (CF_IOCFG->m_pPortConfigs == NULL)
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: failed to allocate memory (size=%d)",
                                                                               AllocSize);
        return DT_STATUS_OUT_OF_MEMORY;
    }
    
    // Clear the IO-configuration
    Status = DtCfIoCfg_ClearConfig(CF_IOCFG, CF_IOCFG->m_pPortConfigs);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: Clear IO-config failed");
        DtMemFreePool(CF_IOCFG->m_pPortConfigs, DF_TAG);
        CF_IOCFG->m_pPortConfigs = NULL;
        return Status;
    }

    // Initialisation, restoring and applying IO-configs has to be performed later
    // when ports are enabled

    // Initialize fast mutex
    DtFastMutexInit(&CF_IOCFG->m_IoCfgMutex);
    CF_IOCFG->m_IoConfigBusy = FALSE;
    DtEventInit(&CF_IOCFG->m_IoConfigDoneEvt, FALSE);

    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCore_IOCONFIG implementation +=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the CfIoCfg stub
#define STUB_CFIOCFG_DEFAULT_PRECONDITIONS(pStub)      \
    DT_ASSERT(pStub!=NULL && pStub->m_Size>=sizeof(DtIoStubCfIoCfg))

// MACRO to cast a stub object to a DtIoStubCfIoCfg object
#define STUB_IOCFG   ((DtIoStubCfIoCfg*)pStub)
#define STUB_CF      ((DtCfIoCfg*)STUB_IOCFG->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtIoStubCfIoCfg_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
static DtStatus  DtIoStubCfIoCfg_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int* pOutSize);
static DtStatus  DtIoStubCfIoCfg_OnCmdGet(const DtIoStubCfIoCfg*, 
                                                const DtExclAccessObject*,
                                                const DtIoctlIoConfigCmdGetIoConfigInput*,
                                                DtIoctlIoConfigCmdGetIoConfigOutput*);
static DtStatus  DtIoStubCfIoCfg_OnCmdSet(const DtIoStubCfIoCfg*,
                                               const DtExclAccessObject*,
                                               const DtIoctlIoConfigCmdSetIoConfigInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_IOCONFIG;

static const DtIoctlProperties  IOSTUB_CF_IOCFG_IOCTLS[] = 
{
    DT_IOCTL_PROPS_IOCONFIG_CMD(
        DtIoStubCfIoCfg_OnCmd, 
        DtIoStubCfIoCfg_AppendDynamicSize, 
        NULL),
};


//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCore_IOCONFIG - Public fuctions +=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfIoCfg_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubCfIoCfg_Close(DtIoStub*  pStub)
{
     DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubCfIoCfg));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfIoCfg_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubCfIoCfg*  DtIoStubCfIoCfg_Open(DtDf*  pCf)
{
    DtIoStubCfIoCfg*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    CF_IOCFG_DEFAULT_PRECONDITIONS(pCf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubCfIoCfg, pCf, NULL, 
                                                              DtIoStubCfIoCfg_Close,
                                                              NULL,  // Use default IOCTL
                                                              IOSTUB_CF_IOCFG_IOCTLS);
    pStub = (DtIoStubCfIoCfg*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCore_IOCONFIG - Private fuctions +=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfIoCfg_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubCfIoCfg_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIoConfigCmdInput*  pInData = NULL;
    
    STUB_CFIOCFG_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_IOCONFIG_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IoConfig;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_IOCONFIG_CMD_GET_IOCONFIG:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlIoConfigCmdGetIoConfigInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlIoConfigCmdGetIoConfigOutput));
        // Add dynamic size (i.e. #io-configs to read)
        pIoParams->m_InReqSize += (pInData->m_GetIoConfig.m_IoConfigCount 
                                                             * sizeof(DtIoctlIoConfigId));
        pIoParams->m_OutReqSize += (pInData->m_GetIoConfig.m_IoConfigCount 
                                                          * sizeof(DtIoctlIoConfigValue));
        break;

    case DT_IOCONFIG_CMD_SET_IOCONFIG:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlIoConfigCmdSetIoConfigInput));
        // Add dynamic size (i.e. #ioconfig to set)
        pIoParams->m_InReqSize += (pInData->m_SetIoConfig.m_IoConfigCount 
                                                                * sizeof(DtIoctlIoConfig));
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfIoCfg_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubCfIoCfg_OnCmd(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams,
    Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIoConfigCmdInput*  pInData = NULL;
    DtIoctlIoConfigCmdOutput*  pOutData = NULL;
    
    STUB_CFIOCFG_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_IOCONFIG_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IoConfig;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IoConfig;
    }


    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IOCONFIG_CMD_GET_IOCONFIG:
        Status = DtIoStubCfIoCfg_OnCmdGet(STUB_IOCFG, &pIoParams->m_ExclAccessObj,
                                                                &pInData->m_GetIoConfig,
                                                                &pOutData->m_GetIoConfig);
        break;
    case DT_IOCONFIG_CMD_SET_IOCONFIG:
        Status = DtIoStubCfIoCfg_OnCmdSet(STUB_IOCFG, &pIoParams->m_ExclAccessObj,
                                                                 &pInData->m_SetIoConfig);
        break;
    
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfIoCfg_OnCmdGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCfIoCfg_OnCmdGet(
    const DtIoStubCfIoCfg*  pStub,
    const DtExclAccessObject*  pExclAccessObj,
    const DtIoctlIoConfigCmdGetIoConfigInput*  pInData,
    DtIoctlIoConfigCmdGetIoConfigOutput*  pOutData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i=0, j=0, AllocSize = 0;
    DtIoConfig*  pCfgs = NULL;

    STUB_CFIOCFG_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    pOutData->m_IoConfigCount = 0;
    if (pInData->m_IoConfigCount == 0)
        return DT_STATUS_OK;

    //.-.-.-.-.-.- Step 1: convert IOCTL params to internal DtIoConfig array -.-.-.-.-.-.-

    // Allocate array for IO-config objects
    AllocSize = sizeof(DtIoConfig)*pInData->m_IoConfigCount;
    pCfgs = (DtIoConfig*)DtMemAllocPool(DtPoolNonPaged, AllocSize, DF_TAG);
    if (pCfgs == NULL)
        return DT_STATUS_OUT_OF_MEMORY;
    DtMemZero(pCfgs, AllocSize);

    // Set the group and port index of the IO-config we wnat to get
    for (i=0; i<pInData->m_IoConfigCount; i++)
    {
        pCfgs[i].m_PortIndex = pInData->m_IoCfgId[i].m_PortIndex;
        Status = IoConfigCodeGet(pInData->m_IoCfgId[i].m_Group, &pCfgs[i].m_Group);
        DT_ASSERT(DT_SUCCESS(Status));
    }

    //.-.-.-.-.-.-.-.-.-.-.-.- Step 2: Get the IO-configurations -.-.-.-.-.-.-.-.-.-.-.-.-
    
    Status =  DtCfIoCfg_Get(STUB_CF, pExclAccessObj, pCfgs, pInData->m_IoConfigCount);

    if (DT_SUCCESS(Status))
    {
        // Convert to IOCTL structures
        for (i=0; i<pInData->m_IoConfigCount; i++)
        {
            // Convert value to value string
            Status = IoConfigNameGet(pCfgs[i].m_Value, pOutData->m_IoCfgValue[i].m_Value,
                                                                  IOCONFIG_NAME_MAX_SIZE);

            // Optionally, convert the sub-value as well
            if (DT_SUCCESS(Status))
            {
                Status = IoConfigNameGet(pCfgs[i].m_SubValue, 
                                                     pOutData->m_IoCfgValue[i].m_SubValue,
                                                     IOCONFIG_NAME_MAX_SIZE);
            }
            for (j=0; j<DT_MAX_PARXTRA_COUNT; j++)
                pOutData->m_IoCfgValue[i].m_ParXtra[j] = pCfgs[i].m_ParXtra[j];
        }
        pOutData->m_IoConfigCount = pInData->m_IoConfigCount;
    }

    // Donot forget to free the io-config list
    if (pCfgs != NULL)
        DtMemFreePool(pCfgs, DF_TAG);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfIoCfg_OnCmdSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCfIoCfg_OnCmdSet(
    const DtIoStubCfIoCfg*  pStub,
    const DtExclAccessObject*  pExclAccessObj,
    const DtIoctlIoConfigCmdSetIoConfigInput*  pInData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i=0, j=0, AllocSize = 0;
    DtIoConfig*  pCfgs = NULL;

    STUB_CFIOCFG_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    if (pInData->m_IoConfigCount == 0)
        return DT_STATUS_OK;

    //.-.-.-.-.-.- Step 1: convert IOCTL params to internal DtIoConfig array -.-.-.-.-.-.-

    // Allocate array for IO-config objects
    AllocSize = sizeof(DtIoConfig)*pInData->m_IoConfigCount;
    pCfgs = (DtIoConfig*)DtMemAllocPool(DtPoolNonPaged, AllocSize, DF_TAG);
    if (pCfgs == NULL)
        return DT_STATUS_OUT_OF_MEMORY;

    // Convert IOCTL structures to DtIoConfig
    for (i=0; i<pInData->m_IoConfigCount; i++)
    {
        pCfgs[i].m_PortIndex = pInData->m_IoCfgPars[i].m_PortIndex;
        Status = IoConfigCodeGet(pInData->m_IoCfgPars[i].m_Group, &pCfgs[i].m_Group);
        DT_ASSERT(DT_SUCCESS(Status));
        
        // Convert string-value/subvalue to code
        Status = IoConfigCodeGet(pInData->m_IoCfgPars[i].m_Value, &pCfgs[i].m_Value);
        DT_ASSERT(DT_SUCCESS(Status));
        Status = IoConfigCodeGet(pInData->m_IoCfgPars[i].m_SubValue, 
                                                                    &pCfgs[i].m_SubValue);
        DT_ASSERT(DT_SUCCESS(Status));

        for (j=0; j<DT_MAX_PARXTRA_COUNT; j++)
            pCfgs[i].m_ParXtra[j] = pInData->m_IoCfgPars[i].m_ParXtra[j];
    }
    
    //-.-.-.-.-.-.-.-.-.-.-.- Step 2: Apply the io-configurations -.-.-.-.-.-.-.-.-.-.-.-.

    Status =  DtCfIoCfg_Set(STUB_CF, pExclAccessObj, pCfgs, pInData->m_IoConfigCount);

    // Donot forget to free the io-config list
    if (pCfgs != NULL)
        DtMemFreePool(pCfgs, DF_TAG);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtCfIoCfg_ReadFromNonVolatileStorage(DtCfIoCfg*, Int PortIndex,
                                                         Int IoGroup, DtCfIoConfigValue*);
static DtStatus  DtCfIoCfg_WriteToNonVolatileStorage(DtCfIoCfg*, Int PortIndex, 
                                                          Int IoGroup, DtCfIoConfigValue);
static DtStatus  DtCfIoCfg_SetDefaultPortConfig(DtCfIoCfg* , DtCfIoCfgPortConfig*,
                                                                             Int PortIdx);
static DtStatus  DtCfIoCfg_ValidateIoDir(DtCfIoCfg*, DtCfIoCfgPortConfig*,
                                                             Int PortIndex, Int NumPorts);
static DtStatus  DtCfIoCfg_ValidateIoStd(DtCfIoCfg*, DtCfIoCfgPortConfig*,
                                                             Int PortIndex, Int NumPorts);
static DtStatus  DtCfIoCfg_ValidateRfClkSel(DtCfIoCfg*, DtCfIoCfgPortConfig*,
                                                             Int PortIndex, Int NumPorts);
static DtStatus  DtCfIoCfg_ValidateSpiClkSel(DtCfIoCfg*, DtCfIoCfgPortConfig*,
                                                             Int PortIndex, Int NumPorts);
static DtStatus  DtCfIoCfg_ValidateSpiMode(DtCfIoCfg*, DtCfIoCfgPortConfig*,
                                                             Int PortIndex, Int NumPorts);
static DtStatus  DtCfIoCfg_ValidateSpiStd(DtCfIoCfg*, DtCfIoCfgPortConfig*,
                                                             Int PortIndex, Int NumPorts);
static DtStatus  DtCfIoCfg_ValidateTsRateSel(DtCfIoCfg*, DtCfIoCfgPortConfig*,
                                                             Int PortIndex, Int NumPorts);
static DtStatus  DtCfIoCfg_ValidateSwS2Apsk(DtCfIoCfg*, DtCfIoCfgPortConfig*,
                                                             Int PortIndex, Int NumPorts);
static DtStatus  DtCfIoCfg_ValidateDmaTestMode(DtCfIoCfg*, DtCfIoCfgPortConfig*,
                                                             Int PortIndex, Int NumPorts);
static DtStatus  DtCfIoCfg_ValidateFailSafe(DtCfIoCfg*, DtCfIoCfgPortConfig*,
                                                             Int PortIndex, Int NumPorts);
static DtStatus  DtCfIoCfg_ValidateGenLocked(DtCfIoCfg*, DtCfIoCfgPortConfig*,
                                                             Int PortIndex, Int NumPorts);
static DtStatus  DtCfIoCfg_ValidateGenRef(DtCfIoCfg*, DtCfIoCfgPortConfig*,
                                                             Int PortIndex, Int NumPorts);
static DtStatus  DtCfIoCfg_ValidateFracMode(DtCfIoCfg*, DtCfIoCfgPortConfig*,
                                                             Int PortIndex, Int NumPorts);
static DtStatus  DtCfIoCfg_ValidateGenRefBoard(DtCfIoCfg*, DtCfIoCfgPortConfig*,
                                                                             Int NumPorts);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_AcquireExclAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfIoCfg_AcquireExclAccess(DtCfIoCfg*  pCf)
{
    DtStatus  Result = DtFastMutexAcquire(&pCf->m_IoCfgMutex);
    while (DT_SUCCESS(Result) && pCf->m_IoConfigBusy)
    {
        DtEventReset(&pCf->m_IoConfigDoneEvt);
        DtFastMutexRelease(&pCf->m_IoCfgMutex);
        DtEventWait(&pCf->m_IoConfigDoneEvt, -1);
        Result = DtFastMutexAcquire(&pCf->m_IoCfgMutex);
    }
    if (DT_SUCCESS(Result))
    {
        pCf->m_IoConfigBusy = TRUE;
        DtFastMutexRelease(&pCf->m_IoCfgMutex);
    }
    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaDeviceReleaseExclAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfIoCfg_ReleaseExclAccess(DtCfIoCfg*  pCf)
{
    DtStatus  Result = DtFastMutexAcquire(&pCf->m_IoCfgMutex);
    if (DT_SUCCESS(Result))
    {
        pCf->m_IoConfigBusy = FALSE;
        // Wake up any other threads waiting for the IO-config done event
        DtEventSet(&pCf->m_IoConfigDoneEvt);
        DtFastMutexRelease(&pCf->m_IoCfgMutex);
    }
    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ApplyConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtCfIoCfg_ApplyConfig(
    DtCfIoCfg*  pCf,
    const DtExclAccessObject*  pExclAccessObj,
    DtCfIoCfgPortConfig*  pNewIoConfig,
    DtCfIoCfgPortUpdates*  pConfigUpdates,
    Bool  SaveToNonVolatileStorage)
{
    DtStatus  Status=DT_STATUS_OK, StatusTemp=DT_STATUS_OK;
    Int  PortIndex;
    Int NumPorts = pCf->m_NumPorts;
    Int  IoConfig;
    DtPt*  pPt;
    
    // Sanity checks
    CF_IOCFG_DEFAULT_PRECONDITIONS(pCf);


    // Prepare ports for the set IO-config. Need to have exclusive access and ports must
    // be IDLE
    Status = DtCfIoCfg_ApplyConfigPrepare(pCf, pExclAccessObj, pConfigUpdates, NumPorts);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: ApplyConfigPrepare failed");
        return Status;
    }

    // Apply settings port after port
    for (PortIndex=0; PortIndex<NumPorts && DT_SUCCESS(Status); PortIndex++)
    {
        DtCfIoConfigValue*  pPortIoCfg =  pNewIoConfig[PortIndex].m_CfgValue;
        DtCfIoCfgPortUpdates*  pPortUpdate = &pConfigUpdates[PortIndex];
        if (!pPortUpdate->m_PortUpdateNeeded)
            continue;

        
        pPt = DtCore_PT_Find(pCf->m_pCore, PortIndex);
        DT_ASSERT(pPt != NULL);
        if (pPt == NULL)
            continue;

        // Perform the set IO-config
        DT_ASSERT(pPt->m_SetIoConfigFunc != NULL);
        Status = pPt->m_SetIoConfigFunc(pPt, pPortIoCfg, DT_IOCONFIG_COUNT);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: SetIoConfig failed for port: %d",
                                                                             PortIndex+1);
            break;
        }

        // Write updates to cache and non volatile storage
        for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
        {
            if (pPortUpdate->m_pConfigValueUpdateNeeded[IoConfig])
            {
                // Save IO config to cache
                 pCf->m_pPortConfigs[PortIndex].m_CfgValue[IoConfig] 
                                                                   = pPortIoCfg[IoConfig];
                // Save IO config to non volatile storage
                if (SaveToNonVolatileStorage)
                    DtCfIoCfg_WriteToNonVolatileStorage(pCf, PortIndex, IoConfig,
                                                                    pPortIoCfg[IoConfig]);
            }
        }
    }

    StatusTemp = DtCfIoCfg_ApplyConfigFinish(pCf, pExclAccessObj, pConfigUpdates, NumPorts);
    if (!DT_SUCCESS(StatusTemp))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: DtCfIoCfg_ApplyConfigFinish failed");
        return StatusTemp;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ApplyConfigPrepare -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfIoCfg_ApplyConfigPrepare(DtCfIoCfg* pCf,
    const DtExclAccessObject* pExclAccessObj,
    DtCfIoCfgPortUpdates* pConfigUpdates,
    Int  NumPorts)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  PortIndex;
    DtPt* pPt;

    // Apply settings port after port
    for (PortIndex=0; PortIndex<NumPorts && DT_SUCCESS(Status); PortIndex++)
    {
        DtCfIoCfgPortUpdates*  pPortUpdate = &pConfigUpdates[PortIndex];
        if (!pPortUpdate->m_PortUpdateNeeded)
            continue;

        pPt = DtCore_PT_Find(pCf->m_pCore, PortIndex);
        DT_ASSERT(pPt != NULL);
        if (pPt == NULL)
            continue;
        DT_ASSERT(pPt->m_SetIoConfigPrepareFunc != NULL);
        Status = pPt->m_SetIoConfigPrepareFunc(pPt, pExclAccessObj);
    }

    // Preparation failed?
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: SetIoConfigPrepare failed for port: %d",
                                                                               PortIndex);
        DtCfIoCfg_ApplyConfigFinish(pCf, pExclAccessObj, pConfigUpdates, PortIndex-1);
    }

    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ApplyConfigFinish -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfIoCfg_ApplyConfigFinish(DtCfIoCfg* pCf,
    const DtExclAccessObject* pExclAccessObj,
    DtCfIoCfgPortUpdates* pConfigUpdates,
    Int  NumPorts)
{
    DtStatus  Status=DT_STATUS_OK, StatusTemp=DT_STATUS_OK;
    Int  PortIndex;
    DtPt* pPt;

    for (PortIndex=0; PortIndex<NumPorts; PortIndex++)
    {
        DtCfIoCfgPortUpdates*  pPortUpdate = &pConfigUpdates[PortIndex];
        if (!pPortUpdate->m_PortUpdateNeeded)
            continue;

        pPt = DtCore_PT_Find(pCf->m_pCore, PortIndex);
        DT_ASSERT(pPt != NULL);
        if (pPt == NULL)
            continue;

        DT_ASSERT(pPt->m_SetIoConfigFinishFunc != NULL);
        StatusTemp = pPt->m_SetIoConfigFinishFunc(pPt, pExclAccessObj);
        if (!DT_SUCCESS(StatusTemp))
        {
            DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: SetIoConfigFinish failed for port: %d",
                                                                             PortIndex+1);
            Status = StatusTemp;
        }
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ClearConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtCfIoCfg_ClearConfig(DtCfIoCfg*  pCf, DtCfIoCfgPortConfig* pIoConfig)
{
    Int NumPorts = pCf->m_NumPorts;
    Int  PortIndex, ParXtra, IoCfg;

    // Sanity checks
    CF_IOCFG_DEFAULT_PRECONDITIONS(pCf);

    for (PortIndex=0; PortIndex<NumPorts; PortIndex++)
    {
        DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;

        // Initial default IO configurations
        for (IoCfg=0; IoCfg<DT_IOCONFIG_COUNT; IoCfg++)
        {
            pPortIoCfg[IoCfg].m_Value = DT_IOCONFIG_NONE;
            pPortIoCfg[IoCfg].m_SubValue = DT_IOCONFIG_NONE;

            for (ParXtra=0; ParXtra<DT_MAX_PARXTRA_COUNT; ParXtra++)
            {
                pPortIoCfg[IoCfg].m_ParXtra[ParXtra] = -1;
            }
        }
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ReadFromNonVolatileStorage -.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtCfIoCfg_ReadFromNonVolatileStorage(
    DtCfIoCfg*  pCf,
    Int  PortIndex,
    Int  IoGroup,
    DtCfIoConfigValue*  pCfgValue)
{
    DtStatus  Result;
    DtCfIoConfigValue  IoConfig;
    Int32  Value;
    Int64  BinValue;
    Char StrValue[IOCONFIG_NAME_MAX_SIZE];
    Int  ParXtraIdx;
    Char GroupName[IOCONFIG_NAME_MAX_SIZE];

    
    // Get the IO group name
    Result = IoConfigNameGet(IoGroup, GroupName, sizeof(GroupName) );
    if (!DT_SUCCESS(Result))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: IoConfigNameGet failed for IOGroup: %d",
                                                                                 IoGroup);
        return Result;
    }

    // Get config value
    Result = DtCore_SETTINGS_StringRead(pCf->m_pCore, PortIndex, GroupName,
                                              "ConfigValue", StrValue,  sizeof(StrValue));
    if (!DT_SUCCESS(Result))
    {
        if (Result != DT_STATUS_NOT_FOUND)
            DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: Read failed for GroupName: %s, "
                                                      "port: %d", GroupName, PortIndex+1);
        return Result;
    }

    Result = IoConfigCodeGet(StrValue, &Value);
    if (!DT_SUCCESS(Result))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: IoConfigCodeGet  StrValue: %s"
                            "(GroupName: %s, port: %d)",StrValue, GroupName, PortIndex+1);
        return Result;
    }
    IoConfig.m_Value = (Int)Value;

    // Get config subvalue
    Result = DtCore_SETTINGS_StringRead(pCf->m_pCore, PortIndex, GroupName,
                                            "ConfigSubValue", StrValue, sizeof(StrValue));
    if (!DT_SUCCESS(Result))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: String-read failed for GroupName: %s, "
                                                      "port: %d", GroupName, PortIndex+1);
        return Result;
    }

    Result = IoConfigCodeGet(StrValue, &Value);
    if (!DT_SUCCESS(Result))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: IoConfigCodeGet  StrValue: %s"
                            "(GroupName: %s, port: %d)",StrValue, GroupName, PortIndex+1);
        return Result;
    }
    IoConfig.m_SubValue = Value;
    
    // Read ParXtra
    for (ParXtraIdx=0; ParXtraIdx<DT_MAX_PARXTRA_COUNT; ParXtraIdx++)
    {
        Result = DtCore_SETTINGS_ValueRead(pCf->m_pCore, PortIndex, GroupName,
                                            (Char*)IoParXtraNames[ParXtraIdx], &BinValue);
        if (!DT_SUCCESS(Result))
        {
            DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: Value-read failed for GroupName: %s, "
                                                      "port: %d", GroupName, PortIndex+1);
            return DT_STATUS_CONFIG_ERROR;
        }
        IoConfig.m_ParXtra[ParXtraIdx] = BinValue;
    }

    // Copy result only if succeeded
    *pCfgValue = IoConfig;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_WriteToNonVolatileStorage -.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtCfIoCfg_WriteToNonVolatileStorage(
    DtCfIoCfg*  pCf,
    Int  PortIndex,
    Int  IoGroup,
    DtCfIoConfigValue  CfgValue)
{
    DtStatus  Result;
    Int  ParXtraIdx;
    Char StrValue[IOCONFIG_NAME_MAX_SIZE];
    Char GroupName[IOCONFIG_NAME_MAX_SIZE];

    // Get the IO group name
    Result = IoConfigNameGet(IoGroup, GroupName, sizeof(GroupName));
    if (!DT_SUCCESS(Result))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: IoConfigNameGet failed for IoGroup: %d",
                                                                                 IoGroup);
        return Result;
    }
 
    // Write config value
    Result = IoConfigNameGet(CfgValue.m_Value, StrValue, sizeof(StrValue));
    if (!DT_SUCCESS(Result))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: IoConfigNameGet failed for Value: %d "
                    "(GroupName: %s, port: %d)",CfgValue.m_Value, GroupName, PortIndex+1);
        return Result;
    }

    Result = DtCore_SETTINGS_StringWrite(pCf->m_pCore, PortIndex, GroupName,
                                                                 "ConfigValue", StrValue);
    if (!DT_SUCCESS(Result))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: StringWrited failed for GroupName: %s "
                                                    "(port: %d)", GroupName, PortIndex+1);
        return Result;
    }

    // Write config subvalue
    Result = IoConfigNameGet(CfgValue.m_SubValue, StrValue, sizeof(StrValue));
    if (!DT_SUCCESS(Result))
    if (!DT_SUCCESS(Result))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: IoConfigNameGet failed for SubValue: %d "
                 "(GroupName: %s, port: %d)",CfgValue.m_SubValue, GroupName, PortIndex+1);
        return Result;
    }

    Result = DtCore_SETTINGS_StringWrite(pCf->m_pCore, PortIndex, GroupName,
                                                              "ConfigSubValue", StrValue);
    if (!DT_SUCCESS(Result))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: String-write failed for GroupName: %s, "
                                                      "port: %d", GroupName, PortIndex+1);
        return Result;
    }
   
    // Write ParXtra
    for (ParXtraIdx=0; ParXtraIdx<DT_MAX_PARXTRA_COUNT; ParXtraIdx++)
    {
        Result = DtCore_SETTINGS_ValueWrite(pCf->m_pCore, PortIndex, GroupName,
                                                        (Char*)IoParXtraNames[ParXtraIdx], 
                                                        CfgValue.m_ParXtra[ParXtraIdx]);
        if (!DT_SUCCESS(Result))
        {
            DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: Value-write failed for GroupName: %s, "
                                                      "port: %d", GroupName, PortIndex+1);
            return Result;
        }
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_GetCachedConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtCfIoCfg_GetCachedConfig(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig)
{
    Int  PortIndex, IoConfig;
    Int  NumPorts = pCf->m_NumPorts;

    // Sanity checks
    CF_IOCFG_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pIoConfig != NULL);

    // Get cached current values
    for (PortIndex=0; PortIndex<NumPorts; PortIndex++)
    {
        DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;
        DtCfIoConfigValue*  pCachePortIoCfg =  pCf->m_pPortConfigs[PortIndex].m_CfgValue;
        for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
            pPortIoCfg[IoConfig] = pCachePortIoCfg[IoConfig];
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_GetStoredConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfIoCfg_GetStoredConfig(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig)
{
    DtStatus  Result = DT_STATUS_OK;
    Int  PortIndex;
    Int  NumPorts = pCf->m_NumPorts;
    Int  IoConfig;

    
    for (PortIndex=0; PortIndex<NumPorts; PortIndex++)
    {
        DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;
        for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
        {
            Result = DtCfIoCfg_ReadFromNonVolatileStorage(pCf, PortIndex, IoConfig,
                                                                   &pPortIoCfg[IoConfig]);
            if (!DT_SUCCESS(Result) && Result!=DT_STATUS_NOT_FOUND)
                break;
        }
    }

    if (Result == DT_STATUS_NOT_FOUND)
        Result = DT_STATUS_OK;

    return Result;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_SetDefaultConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfIoCfg_SetDefaultConfig(DtCfIoCfg*  pCf, DtCfIoCfgPortConfig*  pIoConfig)
{
    Int NumPorts = pCf->m_NumPorts;
    Int  PortIndex;
    
     // Sanity checks
    CF_IOCFG_DEFAULT_PRECONDITIONS(pCf);
    
    
    // Clear the configuration
    DT_RETURN_ON_ERROR(DtCfIoCfg_ClearConfig(pCf, pIoConfig));

    // Initiate port configurations
    for (PortIndex=0; PortIndex<NumPorts; PortIndex++)
    {
        DT_RETURN_ON_ERROR(DtCfIoCfg_SetDefaultPortConfig(pCf, pIoConfig, PortIndex));

    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_SetDefaultPortConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfIoCfg_SetDefaultPortConfig(DtCfIoCfg* pCf,  DtCfIoCfgPortConfig* pIoConfig,
                                                                              Int PortIdx)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool IsNonFunctional, IsBidir;
    Int ParXtra, DefIoStd;
    DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIdx].m_CfgValue;
    DtIoCaps  IoCaps;

     // Sanity checks
    CF_IOCFG_DEFAULT_PRECONDITIONS(pCf);
    
    // Get the IO-capability of the port
    Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, PortIdx, &IoCaps);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, IOCONFIG, pCf, "ERROR: DtCore_PT_GetPortIoCaps failed");
        return Status;
    }

    // A non-functional port has no input and output capability
    IsNonFunctional = !DtIoCapsHasCap(&IoCaps, DT_IOCAP_INPUT) 
                                             && !DtIoCapsHasCap(&IoCaps, DT_IOCAP_OUTPUT);
    // Is it a bidir port?
    IsBidir = DtIoCapsHasCap(&IoCaps, DT_IOCAP_INPUT) 
                                              && DtIoCapsHasCap(&IoCaps, DT_IOCAP_OUTPUT);
    
    // If the port is not a configurable input or output port,
    // we don't have a DEFAULT_IOCONFIG property.
    if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_DBLBUF) 
                                  || DtIoCapsHasCap(&IoCaps, DT_IOCAP_LOOPTHR) || IsBidir)
    {
        Int IodirVal = DtCore_PROPS_GetInt(pCf->m_pCore, "DEFAULT_IODIR", PortIdx, -1);
        switch (IodirVal)
        {
        case DT_IOCONFIG_DISABLED:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_DISABLED));
            pPortIoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_DISABLED;
            pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_DISABLED;
            break;
        case DT_IOCONFIG_INPUT:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_INPUT));
            pPortIoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_INPUT;
            pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_INPUT;
            break;
        case DT_IOCONFIG_INTINPUT:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_INTINPUT));
            pPortIoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_INTINPUT;
            pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_INTINPUT;
            break;
        case DT_IOCONFIG_MONITOR:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_MONITOR));
            pPortIoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_MONITOR;
            pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_MONITOR;
            ParXtra = DtCore_PROPS_GetInt(pCf->m_pCore, "DEFAULT_PARXTRA0", PortIdx, -1);
            DT_ASSERT(ParXtra!=-1);
            pPortIoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            break;
        case DT_IOCONFIG_SHAREDANT:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_SHAREDANT));
            pPortIoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_INPUT;
            pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_SHAREDANT;
            ParXtra = DtCore_PROPS_GetInt(pCf->m_pCore, "DEFAULT_PARXTRA0", PortIdx, -1);
            DT_ASSERT(ParXtra!=-1);
            pPortIoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            break;
        case DT_IOCONFIG_OUTPUT:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_OUTPUT));
            pPortIoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
            pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_OUTPUT;
            break;
        case DT_IOCONFIG_DBLBUF:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_DBLBUF));
            pPortIoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
            pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_DBLBUF;
            ParXtra = DtCore_PROPS_GetInt(pCf->m_pCore, "DEFAULT_PARXTRA0", PortIdx, -1);
            DT_ASSERT(ParXtra!=-1);
            pPortIoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            break;
        case DT_IOCONFIG_LOOPS2L3:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_LOOPS2L3));
            pPortIoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
            pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_LOOPS2L3;
            ParXtra = DtCore_PROPS_GetInt(pCf->m_pCore, "DEFAULT_PARXTRA0", PortIdx, -1);
            DT_ASSERT(ParXtra!=-1);
            pPortIoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            break;
        case DT_IOCONFIG_LOOPS2TS:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_LOOPS2TS));
            pPortIoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
            pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_LOOPS2TS;
            ParXtra = DtCore_PROPS_GetInt(pCf->m_pCore, "DEFAULT_PARXTRA0", PortIdx, -1);
            DT_ASSERT(ParXtra!=-1);
            pPortIoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            ParXtra = DtCore_PROPS_GetInt(pCf->m_pCore, "DEFAULT_PARXTRA1", PortIdx, -1);
            DT_ASSERT(ParXtra!=-1);
            pPortIoCfg[DT_IOCONFIG_IODIR].m_ParXtra[1] = ParXtra;
            break;
        case DT_IOCONFIG_LOOPTHR:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_LOOPTHR));
            pPortIoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
            pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_LOOPTHR;
            ParXtra = DtCore_PROPS_GetInt(pCf->m_pCore, "DEFAULT_PARXTRA0", PortIdx, -1);
            DT_ASSERT(ParXtra!=-1);
            pPortIoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            break;
        default:
            DtDbgOutDf(ERR, IOCONFIG, pCf, "Unsupported DEFAULT_IODIR for port: %d",
                                                                               PortIdx+1);

            DT_ASSERT(FALSE);
            break;
        }
    }
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_INPUT))
    {
        pPortIoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_INPUT;
        pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_INPUT;
    }
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_OUTPUT))
    {
        pPortIoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
        pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_OUTPUT;
    }
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_DISABLED))
    {
        pPortIoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_DISABLED;
    }
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_INTINPUT))
    {
        pPortIoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_INTINPUT;
        pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_INTINPUT;
    }
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_MONITOR))
    {
        pPortIoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_MONITOR;
        pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_MONITOR;
        ParXtra = DtCore_PROPS_GetInt(pCf->m_pCore, "DEFAULT_PARXTRA0", PortIdx, -1);
        DT_ASSERT(ParXtra!=-1);
        pPortIoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
    }
    else if (!IsNonFunctional)
    {
        // IODIR must have a value
        DT_ASSERT(FALSE);
    }
    
    // DT_IOCONFIG_IOSTD; first check if a default has been defined
    DefIoStd = DtCore_PROPS_GetInt(pCf->m_pCore, "DEFAULT_IOSTD", PortIdx, -1);
    if (DefIoStd != -1)
    {
        DtDbgOutDf(MIN, IOCONFIG, pCf, "Default IO-STD = %d for port: %d", DefIoStd,
                                                                               PortIdx+1);
        switch (DefIoStd)
        {
        case DT_IOCONFIG_ASI:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_ASI));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_ASI;
            break;
        case DT_IOCONFIG_AVENC:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_AVENC));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_AVENC;
            break;
        case DT_IOCONFIG_DEKTECST:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_DEKTECST));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCAP_DEKTECST;
            break;
        case DT_IOCONFIG_DEMOD:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_DEMOD));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_DEMOD;
            break;
        case DT_IOCONFIG_GPSTIME:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_GPSTIME));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_GPSTIME;
            break;
        case DT_IOCONFIG_HDMI:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_HDMI));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDMI;
            break;
        case DT_IOCONFIG_IFADC:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_IFADC));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_IFADC;
            break;
        case DT_IOCONFIG_IP:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_IP));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_IP;
            break;
        case DT_IOCONFIG_MOD:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_MOD));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_MOD;
            break;
        case DT_IOCONFIG_PHASENOISE:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_PHASENOISE));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_PHASENOISE;
            break;
        case DT_IOCONFIG_SDIRX:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_SDIRX));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SDIRX;
            break;
        case DT_IOCONFIG_SPI:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPI));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SPI;
            break;
        case DT_IOCONFIG_12GSDI:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_12GSDI));
            // Select any of the 12G-SDI standards
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_12GSDI;
            if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P60))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P60;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P50))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P50;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P59_94))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P59_94;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P50B))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P50B;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P59_94B))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue =  DT_IOCONFIG_2160P59_94B;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P60B))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P60B;
            else
                DT_ASSERT(1==0);
            break;
        case DT_IOCONFIG_2160P50:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P50));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_12GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P50;
            break;
        case DT_IOCONFIG_2160P50B:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P50B));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_12GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P50B;
            break;
        case DT_IOCONFIG_2160P59_94:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P59_94));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_12GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P59_94;
            break;
        case DT_IOCONFIG_2160P59_94B:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P59_94B));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_12GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P59_94B;
            break;
        case DT_IOCONFIG_2160P60:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P60));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_12GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P60;
            break;
        case DT_IOCONFIG_2160P60B:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P60B));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_12GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P60B;
            break;

        case DT_IOCONFIG_3GSDI:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_3GSDI));
            // Select any of the 3G-SDI standards
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
            if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P60))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P60;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P50))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P50;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P59_94))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P59_94;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P60B))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P60B;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P50B))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P50B;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P59_94B))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P59_94B;
            else
                DT_ASSERT(1==0);
            break;
        case DT_IOCONFIG_1080P50:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P50));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P50;
            break;
        case DT_IOCONFIG_1080P50B:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P50B));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P50B;
            break;
        case DT_IOCONFIG_1080P59_94:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P59_94));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P59_94;
            break;
        case DT_IOCONFIG_1080P59_94B:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P59_94B));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P59_94B;
            break;
        case DT_IOCONFIG_1080P60:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P60));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P60;
            break;
        case DT_IOCONFIG_1080P60B:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P60B));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P60B;
            break;

        case DT_IOCONFIG_6GSDI:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_6GSDI));
            // Select any of the 6G-SDI standards
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_6GSDI;
            if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P23_98))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P23_98;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P24))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P24;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P25))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P25;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P29_97))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P29_97;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P30))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P30;
            else
                DT_ASSERT(1==0);
            break;
        case DT_IOCONFIG_2160P23_98:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P23_98));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_6GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P23_98;
            break;
        case DT_IOCONFIG_2160P24:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P24));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_6GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P24;
            break;
        case DT_IOCONFIG_2160P25:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P25));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_6GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P25;
            break;
        case DT_IOCONFIG_2160P29_97:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P29_97));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_6GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P29_97;
            break;
        case DT_IOCONFIG_2160P30:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P30));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_6GSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P30;
            break;

        case DT_IOCONFIG_HDSDI:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_HDSDI));
            // Select any of the HD-SDI standards
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080I50))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080I50;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080I59_94))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080I59_94;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P60))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P60;
            else
                DT_ASSERT(1 == 0);
            break;
        case DT_IOCONFIG_1080I50:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080I50));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080I50;
            break;
        case DT_IOCONFIG_1080I59_94:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080I59_94));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080I59_94;
            break;
        case DT_IOCONFIG_1080I60:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080I60));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080I60;
            break;
        case DT_IOCONFIG_1080P23_98:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P23_98));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P23_98;
            break;
        case DT_IOCONFIG_1080P24:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P24));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P24;
            break;
        case DT_IOCONFIG_1080P25:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P25));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P25;
            break;
        case DT_IOCONFIG_1080P29_97:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P29_97));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P29_97;
            break;
        case DT_IOCONFIG_1080P30:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P30));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P30;
            break;
        case DT_IOCONFIG_1080PSF23_98:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080PSF23_98));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080PSF23_98;
            break;
        case DT_IOCONFIG_1080PSF24:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080PSF24));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080PSF24;
            break;
        case DT_IOCONFIG_1080PSF25:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080PSF25));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080PSF25;
            break;
        case DT_IOCONFIG_1080PSF29_97:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080PSF29_97));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080PSF29_97;
            break;
        case DT_IOCONFIG_1080PSF30:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080PSF30));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080PSF30;
            break;
        case DT_IOCONFIG_720P23_98:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P23_98));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P23_98;
            break;
        case DT_IOCONFIG_720P24:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P24));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P24;
            break;
        case DT_IOCONFIG_720P25:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P25));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P25;
            break;
        case DT_IOCONFIG_720P29_97:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P29_97));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P29_97;
            break;
        case DT_IOCONFIG_720P30:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P30));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P30;
            break;
        case DT_IOCONFIG_720P50:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P50));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P50;
            break;
        case DT_IOCONFIG_720P59_94:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P59_94));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P59_94;
            break;
        case DT_IOCONFIG_720P60:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P60));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P60;
            break;

        case DT_IOCONFIG_SDI:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_SDI));
            // Select any of the SDI standards
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SDI;
            if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_625I50))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_625I50;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_525I59_94))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_525I59_94;
            else
                DT_ASSERT(1==0);
            break;
        case DT_IOCONFIG_525I59_94:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_525I59_94));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_525I59_94;
            break;
        case DT_IOCONFIG_625I50:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_625I50));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_625I50;
            break;

        case DT_IOCONFIG_RS422:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_RS422));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_RS422;
            break;

        case DT_IOCONFIG_SPISDI:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPISDI));
            // Select any of the SPISDI standards
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SPISDI;
            if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPI625I50))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_SPI625I50;
            else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPI525I59_94))
                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_SPI525I59_94;
            else
                DT_ASSERT(1==0);
            break;
        case DT_IOCONFIG_SPI525I59_94:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPI525I59_94));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SPISDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_SPI525I59_94;
            break;
        case DT_IOCONFIG_SPI625I50:
            DT_ASSERT(DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPI625I50));
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SPISDI;
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_SPI625I50;
            break;

        default:
            DtDbgOutDf(ERR, IOCONFIG, pCf, "Unsupported DEFAULT_IOSTD for  port: %d",
                                                                               PortIdx+1);
            DT_ASSERT(1==0);
            break;
        }
    }
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_ASI))
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_ASI;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_AVENC))
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_AVENC;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_12GSDI) 
                                           && DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P60))
    {
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_12GSDI;
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P60;
    }
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_3GSDI) 
                                           && DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P50))
    {
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P60;
    }
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_6GSDI) 
                                           && DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P30))
    {
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_6GSDI;
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_2160P30;
    }
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_HDSDI) 
                                           && DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080I50))
    {
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080I50;
    }
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_DEKTECST))
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_DEKTECST;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_DEMOD))
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_DEMOD;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_GPSTIME))
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_GPSTIME;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_HDMI))
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDMI;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_IFADC))
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_IFADC;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_IP))
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_IP;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_MOD))
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_MOD;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_PHASENOISE))
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_PHASENOISE;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_RS422))
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_RS422;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_SDI))
    {
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SDI;
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_625I50) 
                                         && DtIoCapsHasCap(&IoCaps, DT_IOCAP_525I59_94))
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_525I59_94;
        else
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_625I50;
    }
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_SDIRX))
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SDIRX;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPI))
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SPI;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPISDI))
    {
        pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SPISDI;
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPI625I50) 
                                      && DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPI525I59_94))
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_SPI525I59_94;
        else
            pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_SPI625I50;
    }
    else  if (!IsNonFunctional)
    {
        // IOSTD must have a value
        DT_ASSERT(FALSE);
    }

    // DT_IOCONFIG_RFCLKSEL
    if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_RFCLKINT))
        pPortIoCfg[DT_IOCONFIG_RFCLKSEL].m_Value = DT_IOCONFIG_RFCLKINT;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_RFCLKEXT))
        pPortIoCfg[DT_IOCONFIG_RFCLKSEL].m_Value = DT_IOCONFIG_RFCLKEXT;

    // DT_IOCONFIG_SPICLKSEL
    if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPICLKINT))
        pPortIoCfg[DT_IOCONFIG_SPICLKSEL].m_Value = DT_IOCONFIG_SPICLKINT;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPICLKEXT))
        pPortIoCfg[DT_IOCONFIG_SPICLKSEL].m_Value = DT_IOCONFIG_SPICLKEXT;

    // DT_IOCONFIG_SPIMODE
    if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPIDVBMODE))
        pPortIoCfg[DT_IOCONFIG_SPIMODE].m_Value = DT_IOCONFIG_SPIDVBMODE;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPIFIXEDCLK))
        pPortIoCfg[DT_IOCONFIG_SPIMODE].m_Value = DT_IOCONFIG_SPIFIXEDCLK;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPISER8B))
        pPortIoCfg[DT_IOCONFIG_SPIMODE].m_Value = DT_IOCONFIG_SPISER8B;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPISER10B))
        pPortIoCfg[DT_IOCONFIG_SPIMODE].m_Value = DT_IOCONFIG_SPISER10B;

    // DT_IOCONFIG_SPISTD
    if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPI))
        pPortIoCfg[DT_IOCONFIG_SPISTD].m_Value =
                         DtCore_PROPS_GetInt(pCf->m_pCore, "DEFAULT_SPISTD", PortIdx, -1);

    // DT_IOCONFIG_TSRATESEL
    if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_INTTSRATE))
        pPortIoCfg[DT_IOCONFIG_TSRATESEL].m_Value = DT_IOCONFIG_INTTSRATE;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_EXTTSRATE))
        pPortIoCfg[DT_IOCONFIG_TSRATESEL].m_Value = DT_IOCONFIG_EXTTSRATE;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_EXTRATIO))
        pPortIoCfg[DT_IOCONFIG_TSRATESEL].m_Value = DT_IOCONFIG_EXTRATIO;
    else if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_LOCK2INP))
        pPortIoCfg[DT_IOCONFIG_TSRATESEL].m_Value = DT_IOCONFIG_LOCK2INP;

    // DT_IOCONFIG_SWS2APSK
    if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_SWS2APSK))
        pPortIoCfg[DT_IOCONFIG_SWS2APSK].m_Value = DT_IOCONFIG_FALSE;

    // DT_IOCONFIG_DMATESTMODEM
    if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_DMATESTMODE))
        pPortIoCfg[DT_IOCONFIG_DMATESTMODE].m_Value = DT_IOCONFIG_FALSE;

    // DT_IOCONFIG_FAILSAFE
    if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_FAILSAFE))
    { 
        pPortIoCfg[DT_IOCONFIG_FAILSAFE].m_Value = DT_IOCONFIG_FALSE;
        pPortIoCfg[DT_IOCONFIG_FAILSAFE].m_ParXtra[0] = 0;
    }

    // DT_IOCONFIG_GENREF
    if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_GENREF))
    {
         pPortIoCfg[DT_IOCONFIG_GENREF].m_Value = 
                         DtCore_PROPS_GetInt(pCf->m_pCore, "DEFAULT_GENREF", PortIdx, -1);
    }
    
    // DT_IOCONFIG_GENLOCK
    if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_GENLOCKED))
        pPortIoCfg[DT_IOCONFIG_GENLOCKED].m_Value = DT_IOCONFIG_FALSE;

    // DT_IOCONFIG_FRACMODE
    if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_FRACMODE))
        pPortIoCfg[DT_IOCONFIG_FRACMODE].m_Value = DT_IOCONFIG_FALSE;
 
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ValidateConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Validates the IO configuration.
// Port capabilities are already set
//
DtStatus  DtCfIoCfg_ValidateConfig(DtCfIoCfg*  pCf, DtCfIoCfgPortConfig*  pIoConfig)
{
    DtStatus  Result = DT_STATUS_OK;
    Int NumPorts = pCf->m_NumPorts;
    Int  PortIndex;
    
    // Validate port dependancies
    for (PortIndex=0; PortIndex<NumPorts; PortIndex++)
    {    

        // Validate DT_IOCONFIG_IODIR
        Result = DtCfIoCfg_ValidateIoDir(pCf, pIoConfig, PortIndex, NumPorts);
        if (!DT_SUCCESS(Result))
            return Result;

        // Validate DT_IOCONFIG_IOSTD
        Result = DtCfIoCfg_ValidateIoStd(pCf, pIoConfig, PortIndex, NumPorts);
        if (!DT_SUCCESS(Result))
            return Result;
              
        // Validate DT_IOCONFIG_RFCLKSEL
        Result = DtCfIoCfg_ValidateRfClkSel(pCf, pIoConfig, PortIndex, NumPorts);
        if (!DT_SUCCESS(Result))
            return Result;
      
        // Validate DT_IOCONFIG_SPICLKSEL
        Result = DtCfIoCfg_ValidateSpiClkSel(pCf, pIoConfig, PortIndex,NumPorts);
        if (!DT_SUCCESS(Result))
            return Result;
      
        // Validate DT_IOCONFIG_SPIMODE
        Result = DtCfIoCfg_ValidateSpiMode(pCf, pIoConfig, PortIndex, NumPorts);
        if (!DT_SUCCESS(Result))
            return Result;

        // Validate DT_IOCONFIG_SPISTD
        Result = DtCfIoCfg_ValidateSpiStd(pCf, pIoConfig, PortIndex, NumPorts);
        if (!DT_SUCCESS(Result))
            return Result;

        // Validate DT_IOCONFIG_TSRATESEL
        Result = DtCfIoCfg_ValidateTsRateSel(pCf, pIoConfig, PortIndex,NumPorts);
        if (!DT_SUCCESS(Result))
            return Result;

        // Validate  DT_IOCONFIG_DMATESTMODE
        Result = DtCfIoCfg_ValidateDmaTestMode(pCf, pIoConfig, PortIndex, NumPorts);
        if (!DT_SUCCESS(Result))
            return Result;
        
        // Validate DT_IOCONFIG_FAILSAFE
        Result = DtCfIoCfg_ValidateFailSafe(pCf, pIoConfig, PortIndex, NumPorts);
        if (!DT_SUCCESS(Result))
            return Result;

        // Validate DT_IOCONFIG_GENLOCKED
        Result = DtCfIoCfg_ValidateGenLocked(pCf, pIoConfig, PortIndex,NumPorts);
        if (!DT_SUCCESS(Result))
            return Result;

        // Validate DT_IOCONFIG_GENREF
        Result = DtCfIoCfg_ValidateGenRef(pCf, pIoConfig, PortIndex, NumPorts);
        if (!DT_SUCCESS(Result))
            return Result;
        
        // Validate DT_IOCONFIG_FRACMODE
        Result = DtCfIoCfg_ValidateFracMode(pCf, pIoConfig, PortIndex, NumPorts);
        if (!DT_SUCCESS(Result))
            return Result;

        // Validate DT_IOCONFIG_S2APSK
        Result = DtCfIoCfg_ValidateSwS2Apsk(pCf, pIoConfig, PortIndex, NumPorts);
        if (!DT_SUCCESS(Result))
            return Result;
    }
    // Validate if genref/genlock configs are consistent at board-level
    Result = DtCfIoCfg_ValidateGenRefBoard(pCf, pIoConfig, NumPorts);

    return Result;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ValidateIoDir -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfIoCfg_ValidateIoDir(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig,
    Int PortIndex, 
    Int NumPorts)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i, Buddy, Isi;
    Bool IsNonFunctional;
    DtIoCaps  IoCaps;        // Port IO capabilities
    DtCfIoConfigValue* pCfgVal;
    DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;
    
    // Get the IO-capability of the port
    Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, PortIndex, &IoCaps);
    if (!DT_SUCCESS(Status))
        return Status;
    
    IsNonFunctional = !DtIoCapsHasCap(&IoCaps, DT_IOCAP_INPUT) 
                                           && !DtIoCapsHasCap(&IoCaps, DT_IOCAP_OUTPUT);
    DtDbgOutDf(MAX, IOCONFIG, pCf, "Configuration IODIR Value: %d SubValue: %d",
                                   pPortIoCfg[DT_IOCONFIG_IODIR].m_Value,
                                   pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue);

    switch (pPortIoCfg[DT_IOCONFIG_IODIR].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Only non-functional ports can have no IO direction
        if (!IsNonFunctional)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_DISABLED:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_DISABLED))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_INTINPUT:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_INTINPUT))
            return DT_STATUS_CONFIG_ERROR;
        if (pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue != DT_IOCONFIG_INTINPUT)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_MONITOR:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_MONITOR))
            return DT_STATUS_CONFIG_ERROR;
        if (pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue != DT_IOCONFIG_MONITOR)
            return DT_STATUS_CONFIG_ERROR;
        Buddy = (Int)pPortIoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0];
        // No property to check which buddy port index is valid. 
        // Hardcode based on typenumber for now.
        return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_INPUT:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_INPUT))
            return DT_STATUS_CONFIG_ERROR;
            
        // If another port is set in APSK mode, we are not allowed to use this port
        for (i=0; i<NumPorts; i++)
        {
            if(i != PortIndex)
            {                
                if (pIoConfig[i].m_CfgValue[DT_IOCONFIG_SWS2APSK].
                                                              m_Value == DT_IOCONFIG_TRUE)
                    return DT_STATUS_CONFIG_ERROR;
            }
        }
            
        // Validate DT_IOCONFIG_INPUT
        switch (pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue)
        {
        case DT_IOCONFIG_INPUT:
            break;
        case DT_IOCONFIG_SHAREDANT:
            // Must be a demodulator and antenna must be selectable
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_DEMOD) 
                                        || !DtIoCapsHasCap(&IoCaps, DT_IOCAP_SHAREDANT))
                return DT_STATUS_CONFIG_ERROR;

            Buddy = (Int)pPortIoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0];
            // buddy is never our own index
            if (Buddy == PortIndex)
                return DT_STATUS_CONFIG_ERROR;

            // check if the buddy port exists
            if (Buddy<0 || Buddy>=NumPorts)
                return DT_STATUS_CONFIG_ERROR;

            // Buddy port must be configured as an input
            if (pIoConfig[Buddy].m_CfgValue[DT_IOCONFIG_IODIR].m_Value 
                                                                     != DT_IOCONFIG_INPUT)
                return DT_STATUS_CONFIG_ERROR;
            break;
        default:
            return DT_STATUS_CONFIG_ERROR;
        }
        break;

    case DT_IOCONFIG_OUTPUT:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_OUTPUT))
            return DT_STATUS_CONFIG_ERROR;

        switch(pPortIoCfg[DT_IOCONFIG_IODIR].m_SubValue)
        {
            case DT_IOCONFIG_OUTPUT:
                break;

            case DT_IOCONFIG_DBLBUF:
                if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_DBLBUF))
                    return DT_STATUS_CONFIG_ERROR;
                // Check provided buddy is a valid output port
                Buddy = (Int)pPortIoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0];
                if (Buddy>=0 && Buddy<NumPorts)
                {
                    // Matrix capable ports can only double-up in pairs (i.e. port 2 can
                    // be a copy of port 1, port 4 of port 3, port 6 of port 5, etc).
                    if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_MATRIX))
                    {
                         if ((Buddy+1) != PortIndex)
                        {
                            DtDbgOutDf(ERR, IOCONFIG, pCf, 
                                            "Matrix capable double-up in pairs "
                                            "(port=%d, buddy=%d)", PortIndex+1, Buddy+1);
                            return DT_STATUS_CONFIG_ERROR;
                        }
                    }

                    pCfgVal = &pIoConfig[Buddy].m_CfgValue[DT_IOCONFIG_IODIR];
                    if (pCfgVal->m_Value != DT_IOCONFIG_OUTPUT
                                             || pCfgVal->m_SubValue != DT_IOCONFIG_OUTPUT)
                        return DT_STATUS_CONFIG_ERROR;
                } else
                    return DT_STATUS_CONFIG_ERROR;
                break;

            case DT_IOCONFIG_LOOPS2L3:
                if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_LOOPS2L3))
                    return DT_STATUS_CONFIG_ERROR;
                // Check provided buddy is a valid input port
                Buddy = (Int)pPortIoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0];
                if (Buddy>=0 && Buddy<NumPorts)
                {
                    if (pIoConfig[Buddy].m_CfgValue[DT_IOCONFIG_IODIR].
                                                             m_Value != DT_IOCONFIG_INPUT)
                        return DT_STATUS_CONFIG_ERROR;
                } else
                    return DT_STATUS_CONFIG_ERROR;
                break;

            case DT_IOCONFIG_LOOPS2TS:
                if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_LOOPS2TS))
                    return DT_STATUS_CONFIG_ERROR;
                // Check provided buddy is a valid input port
                Buddy = (Int)pPortIoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0];
                if (Buddy>=0 && Buddy<NumPorts)
                {
                    if (pIoConfig[Buddy].m_CfgValue[DT_IOCONFIG_IODIR].m_Value
                                                                     != DT_IOCONFIG_INPUT)
                        return DT_STATUS_CONFIG_ERROR;
                    // Check ISI
                    Isi = (Int)pPortIoCfg[DT_IOCONFIG_IODIR].m_ParXtra[1];
                    if (Isi<0 || Isi>255)
                        return DT_STATUS_CONFIG_ERROR;
                } else
                    return DT_STATUS_CONFIG_ERROR;
                break;

            case DT_IOCONFIG_LOOPTHR:
                if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_LOOPTHR))
                    return DT_STATUS_CONFIG_ERROR;
                // Check provided buddy is a valid input port
                Buddy = (Int)pPortIoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0];
                if (Buddy>=0 && Buddy<NumPorts)
                {
                    if (pIoConfig[Buddy].m_CfgValue[DT_IOCONFIG_IODIR].
                                                             m_Value != DT_IOCONFIG_INPUT)
                        return DT_STATUS_CONFIG_ERROR;
                } else
                    return DT_STATUS_CONFIG_ERROR;
                break;

            default:
                return DT_STATUS_CONFIG_ERROR;
        }
        break;

    default:
        return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ValidateIoStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfIoCfg_ValidateIoStd(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig,
    Int PortIndex, 
    Int NumPorts)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoCaps  IoCaps;        // Port IO capabilities
    Bool IsNonFunctional;
    DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;
    
    // Get the IO-capability of the port
    Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, PortIndex, &IoCaps);
    if (!DT_SUCCESS(Status))
        return Status;


    IsNonFunctional = !DtIoCapsHasCap(&IoCaps, DT_IOCAP_INPUT) 
                                           && !DtIoCapsHasCap(&IoCaps, DT_IOCAP_OUTPUT);


    DtDbgOutDf(MAX, IOCONFIG, pCf, "Configuration IOSTD Value: %d SubValue: %d",
                                                pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value,
                                                pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue);

    switch (pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Only non-functional ports can have no IO standard
        if (!IsNonFunctional)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_12GSDI:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_12GSDI))
            return DT_STATUS_CONFIG_ERROR;
        // Validate DT_IOCONFIG_12GSDI
        switch (pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue)
        {
        case DT_IOCONFIG_2160P50:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P50))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_2160P59_94:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P59_94))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_2160P60:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P60))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_2160P50B:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P50B))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_2160P59_94B:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P59_94B))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_2160P60B:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P60B))
                return DT_STATUS_CONFIG_ERROR;
            break;
        default:
            return DT_STATUS_CONFIG_ERROR;
        }
        break;
    case DT_IOCONFIG_3GSDI:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_3GSDI))
            return DT_STATUS_CONFIG_ERROR;

        // Validate DT_IOCONFIG_3GSDI
        switch (pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue)
        {
        case DT_IOCONFIG_1080P50:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P50))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P50B:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P50B))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P59_94:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P59_94))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P59_94B:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P59_94B))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P60:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P60))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P60B:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P60B))
                return DT_STATUS_CONFIG_ERROR;
            break;
        default:
            return DT_STATUS_CONFIG_ERROR;
        }
        break;
    case DT_IOCONFIG_6GSDI:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_6GSDI))
            return DT_STATUS_CONFIG_ERROR;
        // Validate DT_IOCONFIG_6GSDI
        switch (pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue)
        {
        case DT_IOCONFIG_2160P30:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P30))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_2160P29_97:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P29_97))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_2160P25:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P25))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_2160P24:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P24))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_2160P23_98:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_2160P23_98))
                return DT_STATUS_CONFIG_ERROR;
            break;
        default:
            return DT_STATUS_CONFIG_ERROR;
        }
        break;
    case DT_IOCONFIG_ASI:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_ASI))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_AVENC:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_AVENC))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_DEKTECST:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_DEKTECST))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_DEMOD:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_DEMOD))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_GPSTIME:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_GPSTIME))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_HDMI:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_HDMI))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_HDSDI:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_HDSDI))
            return DT_STATUS_CONFIG_ERROR;

        // Validate DT_IOCONFIG_HDSDI
        switch (pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue)
        {
        case DT_IOCONFIG_1080I50:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080I50))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080I59_94:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080I59_94))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080I60:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080I60))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P23_98:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P23_98))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P24:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P24))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P25:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P25))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P29_97:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P29_97))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P30:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080P30))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080PSF23_98:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080PSF23_98))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080PSF24:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080PSF24))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080PSF25:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080PSF25))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080PSF29_97:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080PSF29_97))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080PSF30:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_1080PSF30))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P23_98:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P23_98))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P24:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P24))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P25:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P25))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P29_97:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P29_97))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P30:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P30))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P50:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P50))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P59_94:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P59_94))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P60:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_720P60))
                return DT_STATUS_CONFIG_ERROR;
            break;
        default:
            return DT_STATUS_CONFIG_ERROR;
        }
        break;
    case DT_IOCONFIG_IFADC:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_IFADC))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_IP:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_IP))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_MOD:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_MOD))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_PHASENOISE:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_PHASENOISE))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_RS422:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_RS422))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SDI:        
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SDI))
            return DT_STATUS_CONFIG_ERROR;

        // Validate DT_IOCONFIG_SDI
        switch (pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue)
        {
        case DT_IOCONFIG_525I59_94:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_525I59_94))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_625I50:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_625I50))
                return DT_STATUS_CONFIG_ERROR;
            break;
        default:
            return DT_STATUS_CONFIG_ERROR;
        }
        break;
    case DT_IOCONFIG_SDIRX:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SDIRX))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPI:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPI))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPISDI:           
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPISDI))
            return DT_STATUS_CONFIG_ERROR;

        // Validate DT_IOCONFIG_SPISDI
        switch (pPortIoCfg[DT_IOCONFIG_IOSTD].m_SubValue)
        {
        case DT_IOCONFIG_SPI525I59_94:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPI525I59_94))
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_SPI625I50:
            if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPI625I50))
                return DT_STATUS_CONFIG_ERROR;
            break;
        default:
            return DT_STATUS_CONFIG_ERROR;
        }
    break;

    default:
        return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ValidateRfClkSel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfIoCfg_ValidateRfClkSel(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig,
    Int PortIndex, 
    Int NumPorts)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoCaps  IoCaps;        // Port IO capabilities
    DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;
    
    // Get the IO-capability of the port
    Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, PortIndex, &IoCaps);
    if (!DT_SUCCESS(Status))
        return Status;


    DtDbgOutDf(MAX, IOCONFIG, pCf, "Configuration RFCLKSEL Value: %d SubValue: %d",
                                pPortIoCfg[DT_IOCONFIG_RFCLKSEL].m_Value,
                                pPortIoCfg[DT_IOCONFIG_RFCLKSEL].m_SubValue);

    switch (pPortIoCfg[DT_IOCONFIG_RFCLKSEL].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Not applicable should only be set when we do not support ext RF clock
        DT_ASSERT(!DtIoCapsHasCap(&IoCaps, DT_IOCAP_RFCLKINT)
                                        && !DtIoCapsHasCap(&IoCaps, DT_IOCAP_RFCLKEXT));
        break;
    case DT_IOCONFIG_RFCLKINT:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_RFCLKINT))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_RFCLKEXT:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_RFCLKEXT))
            return DT_STATUS_CONFIG_ERROR;
        break;
    default:
        return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ValidateSpiClkSel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfIoCfg_ValidateSpiClkSel(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig,
    Int PortIndex, 
    Int NumPorts)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoCaps  IoCaps;        // Port IO capabilities
    DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;

    // Get the IO-capability of the port
    Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, PortIndex, &IoCaps);
    if (!DT_SUCCESS(Status))
        return Status;

    DtDbgOutDf(MAX, IOCONFIG, pCf, "Configuration SPICLKSEL Value: %d SubValue: %d",
                               pPortIoCfg[DT_IOCONFIG_SPICLKSEL].m_Value,
                               pPortIoCfg[DT_IOCONFIG_SPICLKSEL].m_SubValue);

    switch (pPortIoCfg[DT_IOCONFIG_SPICLKSEL].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Not applicable should only be set when we do not support SPI
        DT_ASSERT(!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPICLKINT)
                                         && !DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPICLKEXT));
        break;
    case DT_IOCONFIG_SPICLKINT:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPICLKINT))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPICLKEXT:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPICLKEXT))
            return DT_STATUS_CONFIG_ERROR;
        // TODO Extra checks needed for DTA-2142
        break;
    default:
        return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ValidateSpiMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfIoCfg_ValidateSpiMode(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig, 
    Int PortIndex, 
    Int NumPorts)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoCaps  IoCaps;        // Port IO capabilities
    DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;

    // Get the IO-capability of the port
    Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, PortIndex, &IoCaps);
    if (!DT_SUCCESS(Status))
        return Status;
    
    DtDbgOutDf(MAX, IOCONFIG, pCf, "Configuration SPIMODE Value: %d SubValue: %d",
                                 pPortIoCfg[DT_IOCONFIG_SPIMODE].m_Value,
                                 pPortIoCfg[DT_IOCONFIG_SPIMODE].m_SubValue);

    switch (pPortIoCfg[DT_IOCONFIG_SPIMODE].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Not applicable should only be set when we do not support SPI
        DT_ASSERT(!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPI));
        break;
    case DT_IOCONFIG_SPIDVBMODE:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPIDVBMODE))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPIFIXEDCLK:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPIFIXEDCLK))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPISER8B:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPISER8B))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPISER10B:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPISER10B))
            return DT_STATUS_CONFIG_ERROR;
        break;
    default:
        return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ValidateSpiStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfIoCfg_ValidateSpiStd(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig,
    Int PortIndex, 
    Int NumPorts)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoCaps  IoCaps;        // Port IO capabilities
    DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;
    
    // Get the IO-capability of the port
    Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, PortIndex, &IoCaps);
    if (!DT_SUCCESS(Status))
        return Status;

     DtDbgOutDf(MAX, IOCONFIG, pCf, "Configuration SPISTD Value: %d SubValue: %d",
                                  pPortIoCfg[DT_IOCONFIG_SPISTD].m_Value,
                                  pPortIoCfg[DT_IOCONFIG_SPISTD].m_SubValue);

    switch (pPortIoCfg[DT_IOCONFIG_SPISTD].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Not applicable should only be set when we do not support SPI
        DT_ASSERT(!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPI));
        break;
    case DT_IOCONFIG_SPILVDS1:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPILVDS1))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPILVDS2:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPILVDS2))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPILVTTL:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SPILVTTL))
            return DT_STATUS_CONFIG_ERROR;
        break;
    default:
        return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ValidateTsRateSel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfIoCfg_ValidateTsRateSel(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig,
    Int PortIndex, 
    Int NumPorts)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoCaps  IoCaps;        // Port IO capabilities
    DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;

    // Get the IO-capability of the port
    Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, PortIndex, &IoCaps);
    if (!DT_SUCCESS(Status))
        return Status;
    
    DtDbgOutDf(MAX, IOCONFIG, pCf, "Configuration TSRATESEL Value: %d SubValue: %d",
                               pPortIoCfg[DT_IOCONFIG_TSRATESEL].m_Value,
                               pPortIoCfg[DT_IOCONFIG_TSRATESEL].m_SubValue);

    switch (pPortIoCfg[DT_IOCONFIG_TSRATESEL].m_Value)
    {
    case DT_IOCONFIG_NONE:
        break;
    case DT_IOCONFIG_INTTSRATE:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_INTTSRATE))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_EXTTSRATE:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_EXTTSRATE))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_EXTRATIO:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_EXTRATIO))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_LOCK2INP:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_LOCK2INP))
            return DT_STATUS_CONFIG_ERROR;
        break;
    default:
        return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ValidateSwS2Apsk -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfIoCfg_ValidateSwS2Apsk(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig,
    Int PortIndex, 
    Int NumPorts)
{
    Int  i;
    DtStatus  Status = DT_STATUS_OK;
    DtIoCaps  IoCaps;        // Port IO capabilities
    DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;

    // Get the IO-capability of the port
    Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, PortIndex, &IoCaps);
    if (!DT_SUCCESS(Status))
        return Status;
    
    DtDbgOutDf(MAX, IOCONFIG, pCf, "Configuration SWS2APSK Value: %d SubValue: %d",
                                pPortIoCfg[DT_IOCONFIG_SWS2APSK].m_Value,
                                pPortIoCfg[DT_IOCONFIG_SWS2APSK].m_SubValue);

    switch (pPortIoCfg[DT_IOCONFIG_SWS2APSK].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Not applicable should only be set when we do not support APSK configuration
        DT_ASSERT(!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SWS2APSK));
        break;
    case DT_IOCONFIG_FALSE:
        // Must be a demodulator and antenna must support APSK configuration
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SWS2APSK))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_TRUE:
        // Must be a demodulator and antenna must support APSK configuration
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_SWS2APSK))
            return DT_STATUS_CONFIG_ERROR;

        // find all other input channels. 
        // - Only one is allowed in APSK mode
        // - All other input channels must be disabled
        for (i=0; i<NumPorts; i++)
        {
            if(i != PortIndex)
            {
                DtIoCaps  BuddyPortCaps;
                // Get the IO-capability of the buddy port
                Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, i, &BuddyPortCaps);
                if (!DT_SUCCESS(Status))
                    return Status;

                if (DtIoCapsHasCap(&BuddyPortCaps, DT_IOCAP_INPUT) && pIoConfig[i].
                            m_CfgValue[DT_IOCONFIG_IODIR].m_Value != DT_IOCONFIG_DISABLED)
                    return DT_STATUS_CONFIG_ERROR;

                if (pIoConfig[i].m_CfgValue[DT_IOCONFIG_SWS2APSK].
                                                              m_Value == DT_IOCONFIG_TRUE)
                    return DT_STATUS_CONFIG_ERROR;
            }
        }
        break;

    default:
        return DT_STATUS_CONFIG_ERROR;
    }    
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ValidateDmaTestMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtCfIoCfg_ValidateDmaTestMode(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig,
    Int PortIndex, 
    Int NumPorts)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoCaps  IoCaps;        // Port IO capabilities
    DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;

    // Get the IO-capability of the port
    Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, PortIndex, &IoCaps);
    if (!DT_SUCCESS(Status))
        return Status;

     DtDbgOutDf(MAX, IOCONFIG, pCf, "Configuration DMATESTMODE Value: %d SubValue: %d",
                                          pPortIoCfg[DT_IOCONFIG_DMATESTMODE].m_Value,
                                          pPortIoCfg[DT_IOCONFIG_DMATESTMODE].m_SubValue);

    switch (pPortIoCfg[DT_IOCONFIG_DMATESTMODE].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Not applicable should only be set when we do not support DMA-rate testmode
        DT_ASSERT(!DtIoCapsHasCap(&IoCaps, DT_IOCAP_DMATESTMODE));
        break;
    case DT_IOCONFIG_FALSE:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_DMATESTMODE))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_TRUE:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_DMATESTMODE))
            return DT_STATUS_CONFIG_ERROR;
        break;
    default:
        return DT_STATUS_CONFIG_ERROR;
    }

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ValidateFailSafe -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfIoCfg_ValidateFailSafe(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig,
    Int PortIndex, 
    Int NumPorts)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoCaps  IoCaps;        // Port IO capabilities
    DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;

    // Get the IO-capability of the port
    Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, PortIndex, &IoCaps);
    if (!DT_SUCCESS(Status))
        return Status;

     DtDbgOutDf(MAX, IOCONFIG, pCf, "Configuration FAILSAFE Value: %d SubValue: %d",
                                pPortIoCfg[DT_IOCONFIG_FAILSAFE].m_Value,
                                pPortIoCfg[DT_IOCONFIG_FAILSAFE].m_SubValue);

    switch (pPortIoCfg[DT_IOCONFIG_FAILSAFE].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Not applicable should only be set when we do not support Failsafe
        DT_ASSERT(!DtIoCapsHasCap(&IoCaps, DT_IOCAP_FAILSAFE));
        break;
    case DT_IOCONFIG_FALSE:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_FAILSAFE))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_TRUE:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_FAILSAFE))
            return DT_STATUS_CONFIG_ERROR;
        break;
    default:
        return DT_STATUS_CONFIG_ERROR;
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ValidateGenLocked -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfIoCfg_ValidateGenLocked(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig,
    Int PortIndex, 
    Int NumPorts)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoCaps  IoCaps;        // Port IO capabilities
    DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;
    
    // Get the IO-capability of the port
    Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, PortIndex, &IoCaps);
    if (!DT_SUCCESS(Status))
        return Status;

    DtDbgOutDf(MAX, IOCONFIG, pCf, "Configuration GENLOCKED Value: %d SubValue: %d",
                               pPortIoCfg[DT_IOCONFIG_GENLOCKED].m_Value,
                               pPortIoCfg[DT_IOCONFIG_GENLOCKED].m_SubValue);

    switch (pPortIoCfg[DT_IOCONFIG_GENLOCKED].m_Value)
    {
    case DT_IOCONFIG_NONE:
        break;
    case DT_IOCONFIG_FALSE:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_GENLOCKED))
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_TRUE:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_GENLOCKED))
            return DT_STATUS_CONFIG_ERROR;
        break;
    default:
        return DT_STATUS_CONFIG_ERROR;
    }

    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ValidateGenRef -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfIoCfg_ValidateGenRef(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig,
    Int PortIndex, 
    Int NumPorts)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoCaps  IoCaps;        // Port IO capabilities
    DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;
    Int  IoStdVal;
    Bool  IsNonFunctional;

    
    // Get the IO-capability of the port
    Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, PortIndex, &IoCaps);
    if (!DT_SUCCESS(Status))
        return Status;

    IsNonFunctional = !DtIoCapsHasCap(&IoCaps, DT_IOCAP_INPUT) 
                                            && !DtIoCapsHasCap(&IoCaps, DT_IOCAP_OUTPUT);

    DtDbgOutDf(MAX, IOCONFIG, pCf, "Configuration GENREF Value: %d SubValue: %d",
                                  pPortIoCfg[DT_IOCONFIG_GENREF].m_Value,
                                  pPortIoCfg[DT_IOCONFIG_GENREF].m_SubValue);
    
    switch (pPortIoCfg[DT_IOCONFIG_GENREF].m_Value)
    {
    case DT_IOCONFIG_NONE:
        break;

    case DT_IOCONFIG_FALSE:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_GENREF))
        {
            DtDbgOutDf(ERR, IOCONFIG, pCf, "Genref not supported for port %i", PortIndex);
            return DT_STATUS_CONFIG_ERROR;
        }
        break;

    case DT_IOCONFIG_TRUE:
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_GENREF))
        {
            DtDbgOutDf(ERR, IOCONFIG, pCf, "Genref not supported for port %i", PortIndex);
            return DT_STATUS_CONFIG_ERROR;
        }

        // Port must be configured as input or must be a non-functional port
        if (pPortIoCfg[DT_IOCONFIG_IODIR].m_Value!=DT_IOCONFIG_INPUT && !IsNonFunctional)
        {
            DtDbgOutDf(ERR, IOCONFIG, pCf, "Port %i is not an input", PortIndex);
            return DT_STATUS_CONFIG_ERROR;
        }
        // Port must be configured for SDI
        IoStdVal = pPortIoCfg[DT_IOCONFIG_IOSTD].m_Value;
        if (IoStdVal!=DT_IOCONFIG_SDI && IoStdVal!=DT_IOCONFIG_HDSDI &&
                                                              IoStdVal!=DT_IOCONFIG_3GSDI)
        {
            DtDbgOutDf(ERR, IOCONFIG, pCf, "Port %i is not an input", PortIndex);
            return DT_STATUS_CONFIG_ERROR;
        }
        break;

    default:
        DtDbgOutDf(ERR, IOCONFIG, pCf, "Unknown config value %i for port %i",
                                      pPortIoCfg[DT_IOCONFIG_GENREF].m_Value,
                                      PortIndex);
        return DT_STATUS_CONFIG_ERROR;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ValidateFracMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfIoCfg_ValidateFracMode(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig,
    Int PortIndex, 
    Int NumPorts)
{
    Int  i;
    DtStatus  Status = DT_STATUS_OK;
    DtIoCaps  IoCaps;        // Port IO capabilities
    DtCfIoConfigValue*  pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;
    
    // Get the IO-capability of the port
    Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, PortIndex, &IoCaps);
    if (!DT_SUCCESS(Status))
        return Status;

    DtDbgOutDf(MAX, IOCONFIG, pCf, "Configuration FRACMODE Value: %d SubValue: %d",
                                pPortIoCfg[DT_IOCONFIG_FRACMODE].m_Value,
                                pPortIoCfg[DT_IOCONFIG_FRACMODE].m_SubValue);

    switch (pPortIoCfg[DT_IOCONFIG_FRACMODE].m_Value)
    {
    case DT_IOCONFIG_NONE:
        break;

    case DT_IOCONFIG_FALSE:
    case DT_IOCONFIG_TRUE:
        // Must support fractional mode
        if (!DtIoCapsHasCap(&IoCaps, DT_IOCAP_FRACMODE))
            return DT_STATUS_CONFIG_ERROR;
        
        // All ports must be configured the same (i.e. fractional mode must be enabled 
        // or disabled for everyone)
        for (i=0; i<NumPorts; i++)
        {
            DtIoCaps OtherPortCaps;
            if (i == PortIndex)
                continue;   // Skip ourselves
            // Get the IO-capability of the port
            Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, i, &OtherPortCaps);
            if (!DT_SUCCESS(Status))
                return Status;
            if (!DtIoCapsHasCap(&OtherPortCaps, DT_IOCAP_FRACMODE))
                continue;   // Skip port that does not support fractional mode

            // Check io-config value for other port matches
            if (pIoConfig[i].m_CfgValue[DT_IOCONFIG_FRACMODE].m_Value
                                             != pPortIoCfg[DT_IOCONFIG_FRACMODE].m_Value)
                return DT_STATUS_CONFIG_ERROR;
        }
        break;

    default:
        return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfIoCfg_ValidateGenRefBoard -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfIoCfg_ValidateGenRefBoard(
    DtCfIoCfg*  pCf,
    DtCfIoCfgPortConfig*  pIoConfig,
    Int NumPorts)
{
    Int  PortIndex, NumGenlock=0, NumGenRef=0;
    Bool  HasVirtualGenref = FALSE;

    // You cannot disable Genref if an output port has Genlocked enabled
    for (PortIndex=0; PortIndex<NumPorts; PortIndex++)
    {
        DtCfIoConfigValue*  pPortIoCfg;
        DtIoCaps  IoCaps;
        // Get the IO-capability of the port
        DtStatus  Status = DtCore_PT_GetPortIoCaps(pCf->m_pCore, PortIndex, &IoCaps);
        if (!DT_SUCCESS(Status))
            return Status;

        pPortIoCfg =  pIoConfig[PortIndex].m_CfgValue;
        if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_GENREF))
        {
            if (pPortIoCfg[DT_IOCONFIG_GENREF].m_Value == DT_IOCONFIG_TRUE)
                NumGenRef++;
            // Check for a virtual GENREF that is not a slave
            if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_VIRTUAL)
                                      && !DtIoCapsHasCap(&IoCaps, DT_IOCAP_GENREFSLAVE))
                HasVirtualGenref = TRUE;
        }
        if (DtIoCapsHasCap(&IoCaps, DT_IOCAP_GENLOCKED))
        {
            if (pPortIoCfg[DT_IOCONFIG_GENLOCKED].m_Value == DT_IOCONFIG_TRUE)
                NumGenlock++;
        }
    }

    // More than one genref port is not supported
    if (NumGenRef > 1)
        return DT_STATUS_CONFIG_ERROR;
    // Devices with a virtual genref port must always have exactly one port configured
    // as genref port
    if (NumGenRef==0 && HasVirtualGenref)
        return DT_STATUS_CONFIG_ERROR;
    // If any port is configured as genlock port we must have a genref port as well.
    if (NumGenlock>0 && NumGenRef==0)
        return DT_STATUS_CONFIG_ERROR;
    return DT_STATUS_OK;
}
