//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* IoConfig.c *#*#*#*#*#*#*#*#* (C) 2010-2016 DekTec
//
// Dta driver - IO configuration - Definition of IO configuration types/functions
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2016 DekTec Digital Video B.V.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtaIncludes.h>


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// DtaIoConfigNonIpPortUpdate
typedef struct _DtaIoConfigNonIpPortUpdate {
    DtaIoConfigValue  m_CfgValue[DT_IOCONFIG_COUNT];
    Bool  m_UpdateNeeded[DT_IOCONFIG_COUNT];
} DtaIoConfigNonIpPortUpdate;

// DtaIoConfigUpdate
typedef struct _DtaIoConfigUpdate {
    DtaIoConfigNonIpPortUpdate*  m_pNonIpPortUpdate;    // Array of tot. num.  NonIp ports
} DtaIoConfigUpdate;

// DtaIoConfigStorageNames
typedef struct _DtaIoConfigStorageNames {
    Char*  m_pCategory;
    Char*  m_pValueName;
    Char*  m_pIoCfgParXtraName[DT_MAX_PARXTRA_COUNT];
} DtaIoConfigStorageNames;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtaIoConfigUpdateApply(DtaDeviceData* pDvcData,
                               DtaIoConfigUpdate* pUpdate, Bool SaveToNonVolatileStorage);
static DtStatus  DtaIoConfigUpdateLoadCache(DtaDeviceData* pDvcData,
                                                              DtaIoConfigUpdate* pUpdate);
static DtStatus  DtaIoConfigUpdateLoadNonVolatileStorage(DtaDeviceData* pDvcData,
                                                             DtaIoConfigUpdate*  pUpdate);
static DtStatus  DtaIoConfigUpdateValidate(DtaDeviceData* pDvcData,
                                                             DtaIoConfigUpdate*  pUpdate);
static DtStatus  DtaIoConfigUpdateValidateIoDir(DtaNonIpPort* pNonIpPort, 
                                         DtaIoConfigNonIpPortUpdate*, DtaIoConfigUpdate*);
static DtStatus  DtaIoConfigUpdateValidateIoStd(DtaNonIpPort* pNonIpPort, 
                                         DtaIoConfigNonIpPortUpdate*, DtaIoConfigUpdate*);
static DtStatus  DtaIoConfigUpdateValidateRfClkSel(DtaNonIpPort* pNonIpPort,
                                         DtaIoConfigNonIpPortUpdate*, DtaIoConfigUpdate*);
static DtStatus  DtaIoConfigUpdateValidateSpiClkSel(DtaNonIpPort* pNonIpPort,
                                         DtaIoConfigNonIpPortUpdate*, DtaIoConfigUpdate*);
static DtStatus  DtaIoConfigUpdateValidateSpiMode(DtaNonIpPort* pNonIpPort,
                                         DtaIoConfigNonIpPortUpdate*, DtaIoConfigUpdate*);
static DtStatus  DtaIoConfigUpdateValidateSpiStd(DtaNonIpPort* pNonIpPort,
                                         DtaIoConfigNonIpPortUpdate*, DtaIoConfigUpdate*);
static DtStatus  DtaIoConfigUpdateValidateTsRateSel(DtaNonIpPort* pNonIpPort,
                                         DtaIoConfigNonIpPortUpdate*, DtaIoConfigUpdate*);
static DtStatus  DtaIoConfigUpdateValidateSwS2Apsk(DtaNonIpPort* pNonIpPort,
                                         DtaIoConfigNonIpPortUpdate*, DtaIoConfigUpdate*);
static DtStatus  DtaIoConfigUpdateValidateFailSafe(DtaNonIpPort* pNonIpPort,
                                         DtaIoConfigNonIpPortUpdate*, DtaIoConfigUpdate*);
static DtStatus  DtaIoConfigUpdateValidateGenLocked(DtaNonIpPort* pNonIpPort,
                                         DtaIoConfigNonIpPortUpdate*, DtaIoConfigUpdate*);
static DtStatus  DtaIoConfigUpdateValidateGenRef(DtaNonIpPort* pNonIpPort,
                                         DtaIoConfigNonIpPortUpdate*, DtaIoConfigUpdate*);
static DtStatus  DtaIoConfigUpdateValidateFracMode(DtaNonIpPort* pNonIpPort,
                                         DtaIoConfigNonIpPortUpdate*, DtaIoConfigUpdate*);
static DtStatus  DtaIoConfigWriteToNonVolatileStorage(DtaNonIpPort*  pNonIpPort,
                                                Int  IoGroup, DtaIoConfigValue  CfgValue);
static DtStatus  DtaIoConfigUpdateValidateGenRefBoard(DtaDeviceData*, DtaIoConfigUpdate*);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Global data -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// ParXtra names
static const char*   IoParXtraNames[DT_MAX_PARXTRA_COUNT] = {
        "IoParXtra0", 
        "IoParXtra1", 
        "IoParXtra2", 
        "IoParXtra3" };



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIoConfigInit(DtaDeviceData* pDvcData)
{
    DtStatus  Result;
    Int  NonIpIndex;
    DtaNonIpPort*  pNonIpPort;
    DtaIoConfigNonIpPortUpdate*  pPortUpdate;
    Int  IoConfig;
    DtaIoConfigUpdate  Update;

    if (pDvcData->m_NumNonIpPorts == 0)
        return DT_STATUS_OK;

    // Use dynamic allocated structure, otherwise we get stack errors.
    Update.m_pNonIpPortUpdate = 
                 (DtaIoConfigNonIpPortUpdate*)DtMemAllocPool(DtPoolNonPaged, 
                 sizeof(DtaIoConfigNonIpPortUpdate) * pDvcData->m_NumNonIpPorts, DTA_TAG);
    if (Update.m_pNonIpPortUpdate == NULL)
        return DT_STATUS_OUT_OF_MEMORY;

    // Load cache (initial cache holds the defaults which are not already applied)
    Result = DtaIoConfigUpdateLoadCache(pDvcData, &Update);
    if(!DT_SUCCESS(Result))
    {
        DtMemFreePool(Update.m_pNonIpPortUpdate, DTA_TAG);
        return Result;
    }
        

    // Registry is only read after a cold start, else the cache already holds the correct
    // IO configurations.
    if (pDvcData->m_InitialPowerup)
    {
        // Load non volatile storage settings
        Result = DtaIoConfigUpdateLoadNonVolatileStorage(pDvcData, &Update);
        if(!DT_SUCCESS(Result))
        {
            DtMemFreePool(Update.m_pNonIpPortUpdate, DTA_TAG);
            return Result;
        }

        // Validate configuration after registry manipulations are done
        Result = DtaIoConfigUpdateValidate(pDvcData, &Update);
        if (!DT_SUCCESS(Result))
        {
            DtDbgOut(ERR, IOCONFIG, "Configuration invalid. Apply initial configuration");
            // Reload cache (initial cache holds the defaults which are not already
            // applied)
            Result = DtaIoConfigUpdateLoadCache(pDvcData, &Update);
            if(!DT_SUCCESS(Result))
            {
                DtMemFreePool(Update.m_pNonIpPortUpdate, DTA_TAG);
                return Result;
            }

            // Delete the registry settings for this device.
            if (!DT_SUCCESS(DtNonVolatileSettingsDelete(&pDvcData->m_Driver, 
                                                           pDvcData->m_DevInfo.m_UniqueId,
                                                           pDvcData->m_NumNonIpPorts+
                                                           pDvcData->m_NumIpPorts)))
                DtDbgOut(ERR, IOCONFIG, "Error deleting registry key");
        }
    }

    // Set UpdateNeeded flag for all IO configurations during init...
    for (NonIpIndex=0; NonIpIndex<pDvcData->m_NumNonIpPorts; NonIpIndex++)
    {
        pNonIpPort = &(pDvcData->m_pNonIpPorts[NonIpIndex]);
        pPortUpdate = &(Update.m_pNonIpPortUpdate[pNonIpPort->m_PortIndex]);
        for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
        {
            // Skip IO configs not applicable to the current port type
            if (pPortUpdate->m_CfgValue[IoConfig].m_Value == DT_IOCONFIG_NONE)
                continue;
            pPortUpdate->m_UpdateNeeded[IoConfig] = TRUE;
        }
    }

    // Apply (without saving everything to the non volatile storage)
    Result = DtaIoConfigUpdateApply(pDvcData, &Update, FALSE);
    DtMemFreePool(Update.m_pNonIpPortUpdate, DTA_TAG);
    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIoConfigGet(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoConfigId* pIoCfgId, 
    DtIoConfigValue* pIoCfgValue, 
    Int  Count)
{
    DtStatus  Result = DT_STATUS_OK;
    Int  i, NonIpIndex, IpIndex, IoGroup;
    DtaNonIpPort*  pNonIpPort;
    DtaIoConfigValue  DtaIoCfgVal;
    DtaIpPort*  pIpPort;
    Int64  ParXtra;

    if (Count == 0)
          return DT_STATUS_INVALID_PARAMETER;

    for (i=0; i<Count; i++)
    {
        // Get Idx of ConfigGroup
        Result = IoConfigCodeGet(pIoCfgId[i].m_Group, &IoGroup);
        if (!DT_SUCCESS(Result))
            return DT_STATUS_INVALID_PARAMETER;
                  
        // Find the correct port struct
        Result = DtaGetNonIpPortIndex(pDvcData, pIoCfgId[i].m_PortIndex, &NonIpIndex);
        if (DT_SUCCESS(Result))
        {
            pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpIndex];

            // Get the IO configuration and copy it to the caller variables
            Result = DtaNonIpIoConfigGet(pNonIpPort, IoGroup, &DtaIoCfgVal);
        } 
        else 
        {
            // Check IP port
            Result = DtaGetIpPortIndex(pDvcData, pIoCfgId[i].m_PortIndex, &IpIndex);
            if (!DT_SUCCESS(Result))
                return Result;

            pIpPort = &pDvcData->m_IpDevice.m_pIpPorts[IpIndex];
            Result = DtaIpIoConfigGet(pIpPort, IoGroup, &DtaIoCfgVal);
        }
        if (!DT_SUCCESS(Result)) 
            return Result;
    
        // Value to string
        Result = IoConfigNameGet(DtaIoCfgVal.m_Value, pIoCfgValue[i].m_Value, 
                                                                  IOCONFIG_NAME_MAX_SIZE);

        // Optionally convert Subvalue to string
        if (DT_SUCCESS(Result)) 
            Result = IoConfigNameGet(DtaIoCfgVal.m_SubValue, pIoCfgValue[i].m_SubValue, 
                                                                  IOCONFIG_NAME_MAX_SIZE);

        if (DT_SUCCESS(Result))
        {
            for(ParXtra=0; ParXtra<DT_MAX_PARXTRA_COUNT; ParXtra++)
                pIoCfgValue[i].m_ParXtra[ParXtra] = DtaIoCfgVal.m_ParXtra[ParXtra];
        }
    }
    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaIoConfigSet(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoConfigPars*  pIoConfigs,
    Int  Count)
{
    DtStatus  Result = DT_STATUS_OK;
    Int  Cfg;
    DtIoConfigPars*  pCfg;
    Int  NonIpIndex;
    DtaNonIpPort*  pNonIpPort;

    DtaIoConfigUpdate  Update;
    DtaIoConfigNonIpPortUpdate*  pPortUpdate;
    DtaIoConfigValue  IpDataConfigValue;
    DtaIpPort*  pIpPort;
    Int  IpIndex;
    Int  IoGroup;
    Int  ConfigValue;
    Int  ConfigSubValue;
    Int  ParXtra;
    Int  NumIpCfgs = 0;
    Bool*  pPortsUpdated = NULL;

    pPortsUpdated = (Bool*)DtMemAllocPool(DtPoolNonPaged,
                                            sizeof(Bool) * pDvcData->m_NumPorts, DTA_TAG);
    if (pPortsUpdated == NULL)
    {
        return DT_STATUS_OUT_OF_MEMORY;
    }
    DtMemZero(pPortsUpdated, sizeof(Bool) * pDvcData->m_NumPorts);

    // Get exclusive access lock to prevent ports from being opened/closed and other
    // IO configs from being applied
    Result = DtaDeviceAcquireExclAccess(pDvcData);
    
    // First do IoConfigSet for IP ports. We assume here that IoConfigs for IP and
    // non-IP ports are completely unrelated. To do this properly we should validate
    // the IoConfigs for both IP and non-IP ports first before applying any of them, but
    // since currently IoConfigSet for IP ports is always a no-op (but can return an
    // error for invalid values), we can set the IoConfigs for those first before worrying
    // about non-IP ports.
    for (Cfg=0; Cfg<Count; Cfg++)
    {
        pCfg = &pIoConfigs[Cfg];
        Result = DtaGetIpPortIndex(pDvcData, pCfg->m_PortIndex, &IpIndex);
        if (!DT_SUCCESS(Result))
        {
            // Ignore IoConfig for now if not for an IP port
            Result = DT_STATUS_OK;
            continue;
        }

        pPortsUpdated[pCfg->m_PortIndex] = TRUE;

        // Get Idx of ConfigGroup
        Result = IoConfigCodeGet(pCfg->m_Group, &IoGroup);
        if (!DT_SUCCESS(Result))
            break;

        // Get Idx of ConfigValue
        if (!DT_SUCCESS(IoConfigCodeGet(pCfg->m_Value, &ConfigValue)))
            ConfigValue = -1;
            
        // Get Idx of ConfigSubValue
        if (!DT_SUCCESS(IoConfigCodeGet(pCfg->m_SubValue, &ConfigSubValue)))
            ConfigSubValue = -1;


        pIpPort = &pDvcData->m_IpDevice.m_pIpPorts[IpIndex];
        IpDataConfigValue.m_Value = ConfigValue;
        IpDataConfigValue.m_SubValue = ConfigSubValue;
        for (ParXtra=0; ParXtra<DT_MAX_PARXTRA_COUNT; ParXtra++)
            IpDataConfigValue.m_ParXtra[ParXtra] = pCfg->m_ParXtra[ParXtra];

        Result = DtaIpIoConfigSet(pIpPort, IoGroup, IpDataConfigValue);
        if (!DT_SUCCESS(Result))
            break;
        NumIpCfgs++;
    }
    // Return on error or if all configs were for IP ports
    if (!DT_SUCCESS(Result) || NumIpCfgs==Count)
    {
        DtaDeviceReleaseExclAccess(pDvcData);
        DtMemFreePool(pPortsUpdated, DTA_TAG);
        return Result;
    }

    Update.m_pNonIpPortUpdate = 
                 (DtaIoConfigNonIpPortUpdate*)DtMemAllocPool(DtPoolNonPaged, 
                 sizeof(DtaIoConfigNonIpPortUpdate) * pDvcData->m_NumNonIpPorts, DTA_TAG);
    if (Update.m_pNonIpPortUpdate == NULL)
    {    
        DtDbgOut(ERR, IOCONFIG, "Failed to allocate %dkB for update structure",
            (int)((sizeof(DtaIoConfigNonIpPortUpdate) * pDvcData->m_NumNonIpPorts)/1024));

        DtaDeviceReleaseExclAccess(pDvcData);
        DtMemFreePool(pPortsUpdated, DTA_TAG);
        return DT_STATUS_OUT_OF_MEMORY;
    }
    else
    {
        DtDbgOut(MAX, IOCONFIG, "Allocated %dkB for update structure",
            (int)((sizeof(DtaIoConfigNonIpPortUpdate) * pDvcData->m_NumNonIpPorts)/1024));
    }

    // Non Ip configuration
    // Load cache
    if (DT_SUCCESS(Result))
        Result = DtaIoConfigUpdateLoadCache(pDvcData, &Update);

    // Merge new configs
    if (DT_SUCCESS(Result))
    {
        for (Cfg=0; Cfg<Count; Cfg++)
        {
            pCfg = &pIoConfigs[Cfg];
            Result = DtaGetNonIpPortIndex(pDvcData, pCfg->m_PortIndex, &NonIpIndex);
            if (!DT_SUCCESS(Result))
            {
                Result = DT_STATUS_OK;
                continue;
            }

            // Get Idx of ConfigGroup
            Result = IoConfigCodeGet(pCfg->m_Group, &IoGroup);
            if (!DT_SUCCESS(Result))
                break;

            // Get Idx of ConfigValue
            if (!DT_SUCCESS(IoConfigCodeGet(pCfg->m_Value, &ConfigValue)))
                ConfigValue = -1;
            
            // Get Idx of ConfigSubValue
            if (!DT_SUCCESS(IoConfigCodeGet(pCfg->m_SubValue, &ConfigSubValue)))
                ConfigSubValue = -1;


            pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpIndex];
            pPortUpdate = &Update.m_pNonIpPortUpdate[NonIpIndex];

            // Check if we should update the IO configuration
            if (pPortUpdate->m_CfgValue[IoGroup].m_Value != ConfigValue
               || pPortUpdate->m_CfgValue[IoGroup].m_SubValue != ConfigSubValue)
                pPortUpdate->m_UpdateNeeded[IoGroup] = TRUE;
            else
            {
                // Check if ParXtra's change
                for (ParXtra=0; ParXtra<DT_MAX_PARXTRA_COUNT; ParXtra++)
                {
                    if (pPortUpdate->m_CfgValue[IoGroup].
                                        m_ParXtra[ParXtra] != pCfg->m_ParXtra[ParXtra])
                        pPortUpdate->m_UpdateNeeded[IoGroup] = TRUE;
                }
            }

            // Set new value/parxtra
            if (pPortUpdate->m_UpdateNeeded[IoGroup])
            {
                // Check if nobody owns the exclusive access lock. During channel IO
                // configuration it's also OK if we own the exclusive lock
                if (!pCfg->m_SkipExclAccessCheck)
                {
                    Result = DtaNonIpHasAccess(pNonIpPort, pFile);
                    if (!DT_SUCCESS(Result))
                    {
                        DtDbgOutPort(ERR, IOCONFIG, pNonIpPort,
                            "Port configuration needs to be updated, but port is in use");
                        break;
                    }
                }
                
                pPortsUpdated[pCfg->m_PortIndex] = TRUE;

                pPortUpdate->m_CfgValue[IoGroup].m_Value = ConfigValue;
                pPortUpdate->m_CfgValue[IoGroup].m_SubValue = ConfigSubValue;
                for (ParXtra=0; ParXtra<DT_MAX_PARXTRA_COUNT; ParXtra++)
                {
                    pPortUpdate->m_CfgValue[IoGroup].m_ParXtra[ParXtra] =
                                                                 pCfg->m_ParXtra[ParXtra];
                }
            }
        }
    }

    // Validate
    if (DT_SUCCESS(Result))
        Result = DtaIoConfigUpdateValidate(pDvcData, &Update);
    
    // Apply (and save updates to the non volatile storage)
    // The DtaIoConfigUpdateApply must be running at passive level for Windows,
    // so we need to release the ExclAccessMutex here.
    // We set a flag to indicate that an update is still busy
    if (DT_SUCCESS(Result))
        pDvcData->m_RegistryWriteBusy = TRUE;

    // Release exclusive access mutex. Use DtFastMutexRelease instead of
    // DtaDeviceReleaseExclAccess to prevent setting the m_RegWriteDoneEvt event. It'll
    // be set by DtaIoConfigUpdateApply.
    DtFastMutexRelease(&pDvcData->m_ExclAccessMutex);

    if (DT_SUCCESS(Result))
    {
        Int  i;
        Result = DtaIoConfigUpdateApply(pDvcData, &Update, TRUE);

        for (i=0; i<pDvcData->m_NumPorts; i++)
        {
            if (!pPortsUpdated[i])
                continue;
            DtaEventsSet(pDvcData, NULL, DTA_EVENT_TYPE_IOCONFIG, i, 0);
        }
    }
    
    DtMemFreePool(pPortsUpdated, DTA_TAG);
    DtMemFreePool(Update.m_pNonIpPortUpdate, DTA_TAG);

    return Result;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateApply -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Post: pDvcData->m_RegWriteBusy flag must be reset
//
static DtStatus  DtaIoConfigUpdateApply(
    DtaDeviceData*  pDvcData,
    DtaIoConfigUpdate*  pUpdate,
                                // Contains the complete new IO configuration
    Bool  SaveToNonVolatileStorage)
{
    DtStatus  Result;
    Int  NonIpIndex;
    Int  IoConfig;
    DtaNonIpPort*  pNonIpPort;
    DtaIoConfigNonIpPortUpdate*  pPortUpdate;

    // Apply settings port after port
    for (NonIpIndex=0; NonIpIndex<pDvcData->m_NumNonIpPorts; NonIpIndex++)
    {
        pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpIndex];
        pPortUpdate = &pUpdate->m_pNonIpPortUpdate[NonIpIndex];
        for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
        {
            if (pPortUpdate->m_UpdateNeeded[IoConfig])
            {
                // Apply IO config
                Result = DtaNonIpIoConfigSet(pNonIpPort, IoConfig,
                                                       pPortUpdate->m_CfgValue[IoConfig]);
                if (!DT_SUCCESS(Result))
                {
                    pDvcData->m_RegistryWriteBusy = FALSE;
                    DtEventSet(&pDvcData->m_RegWriteDoneEvt);
                    return Result;
                }

                // Save IO config to non volatile storage
               if (SaveToNonVolatileStorage)
                   DtaIoConfigWriteToNonVolatileStorage(pNonIpPort, IoConfig,
                                                       pPortUpdate->m_CfgValue[IoConfig]);
            }
        }
    }
    pDvcData->m_RegistryWriteBusy = FALSE;
    DtEventSet(&pDvcData->m_RegWriteDoneEvt);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateLoadCache -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaIoConfigUpdateLoadCache(
    DtaDeviceData*  pDvcData,
    DtaIoConfigUpdate*  pUpdate)
                                // Contains the complete new IO configuration
{
    Int  NonIpIndex;
    Int  IoConfig;
    DtaNonIpPort*  pNonIpPort;
    DtaIoConfigNonIpPortUpdate*  pPortUpdate = NULL;

    // Load non IP IO configuration cache
    for (NonIpIndex=0; NonIpIndex<pDvcData->m_NumNonIpPorts; NonIpIndex++)
    {
        pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpIndex];
        pPortUpdate = &pUpdate->m_pNonIpPortUpdate[NonIpIndex];

        for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
        {
            // Get cached current values
            DtaNonIpIoConfigGet(pNonIpPort, IoConfig, &pPortUpdate->m_CfgValue[IoConfig]);
            pPortUpdate->m_UpdateNeeded[IoConfig] = FALSE;
        }
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateLoadNonVolatileStorage -.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaIoConfigUpdateLoadNonVolatileStorage(
    DtaDeviceData*  pDvcData,
    DtaIoConfigUpdate*  pUpdate)
                                // Contains the complete new IO configuration
{
    DtStatus  Result = DT_STATUS_OK;
    Int  NonIpIndex;
#ifdef WINBUILD
    Int  IoConfig;
#else
    Int  PortNumber;
#endif
    DtaNonIpPort*  pNonIpPort;
    DtaIoConfigNonIpPortUpdate*  pPortUpdate = NULL;

    for (NonIpIndex=0; NonIpIndex<pDvcData->m_NumNonIpPorts; NonIpIndex++)
    {
        pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpIndex];
        pPortUpdate = &pUpdate->m_pNonIpPortUpdate[NonIpIndex];
#ifdef WINBUILD
        for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
        {
            Result = DtIoConfigReadFromNonVolatileStorage(&pDvcData->m_Driver,
                                                      pDvcData->m_DevInfo.m_UniqueId,
                                                      pNonIpPort->m_PortIndex, IoConfig,
                                                      &pPortUpdate->m_CfgValue[IoConfig]);
            if (!DT_SUCCESS(Result) && Result!=DT_STATUS_NOT_FOUND)
                break;
        }
#else

        Result = DtaGetPortNumber(pDvcData, pNonIpPort->m_PortIndex, &PortNumber);
        if (!DT_SUCCESS(Result))
            break;
        Result = DtIoConfigReadFromIniFile("Dta", pDvcData->m_DevInfo.m_Serial,
                                                     pDvcData->m_DevInfo.m_TypeNumber,
                                                     PortNumber, pPortUpdate->m_CfgValue);
        if (!DT_SUCCESS(Result) && Result!=DT_STATUS_NOT_FOUND)
            break;
#endif
    }

    if (Result == DT_STATUS_NOT_FOUND)
        Result = DT_STATUS_OK;

    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateValidate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Validate the port dependencies for the new IO configuration.
// Port capabilities are already checked in the DTAPI
// Changes to ports with exclusive access pending are already checked in DtaIoConfigSet
//
static DtStatus  DtaIoConfigUpdateValidate(
    DtaDeviceData*  pDvcData,
    DtaIoConfigUpdate*  pUpdate)
                                // Contains the complete new IO configuration
{
    DtStatus  Result = DT_STATUS_OK;
    DtaNonIpPort*  pNonIpPort=NULL;
    DtaIoConfigNonIpPortUpdate*  pPortUpdate=NULL;
    Int  NonIpPortIndex;

    
    // Validate port dependancies
    for (NonIpPortIndex=0; NonIpPortIndex<pDvcData->m_NumNonIpPorts; NonIpPortIndex++)
    {
        pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpPortIndex];
        pPortUpdate = &pUpdate->m_pNonIpPortUpdate[NonIpPortIndex];
                
        // Validate DT_IOCONFIG_IODIR
        Result = DtaIoConfigUpdateValidateIoDir(pNonIpPort, pPortUpdate, pUpdate);
        if (!DT_SUCCESS(Result))
            return Result;

        // Validate DT_IOCONFIG_IOSTD
        Result = DtaIoConfigUpdateValidateIoStd(pNonIpPort, pPortUpdate, pUpdate);
        if (!DT_SUCCESS(Result))
            return Result;
              
        // Validate DT_IOCONFIG_RFCLKSEL
        Result = DtaIoConfigUpdateValidateRfClkSel(pNonIpPort, pPortUpdate, pUpdate);
        if (!DT_SUCCESS(Result))
            return Result;
      
        // Validate DT_IOCONFIG_SPICLKSEL
        Result = DtaIoConfigUpdateValidateSpiClkSel(pNonIpPort, pPortUpdate, pUpdate);
        if (!DT_SUCCESS(Result))
            return Result;
      
        // Validate DT_IOCONFIG_SPIMODE
        Result = DtaIoConfigUpdateValidateSpiMode(pNonIpPort, pPortUpdate, pUpdate);
        if (!DT_SUCCESS(Result))
            return Result;

        // Validate DT_IOCONFIG_SPISTD
        Result = DtaIoConfigUpdateValidateSpiStd(pNonIpPort, pPortUpdate, pUpdate);
        if (!DT_SUCCESS(Result))
            return Result;

        // Validate DT_IOCONFIG_TSRATESEL
        Result = DtaIoConfigUpdateValidateTsRateSel(pNonIpPort, pPortUpdate, pUpdate);
        if (!DT_SUCCESS(Result))
            return Result;

        // Validate DT_IOCONFIG_FAILSAFE
        Result = DtaIoConfigUpdateValidateFailSafe(pNonIpPort, pPortUpdate, pUpdate);
        if (!DT_SUCCESS(Result))
            return Result;

        // Validate DT_IOCONFIG_GENLOCKED
        Result = DtaIoConfigUpdateValidateGenLocked(pNonIpPort, pPortUpdate, pUpdate);
        if (!DT_SUCCESS(Result))
            return Result;

        // Validate DT_IOCONFIG_GENREF
        Result = DtaIoConfigUpdateValidateGenRef(pNonIpPort, pPortUpdate, pUpdate);
        if (!DT_SUCCESS(Result))
            return Result;
        
        // Validate DT_IOCONFIG_FRACMODE
        Result = DtaIoConfigUpdateValidateFracMode(pNonIpPort, pPortUpdate, pUpdate);
        if (!DT_SUCCESS(Result))
            return Result;

        // Validate DT_IOCONFIG_S2APSK
        Result = DtaIoConfigUpdateValidateSwS2Apsk(pNonIpPort, pPortUpdate, pUpdate);
        if (!DT_SUCCESS(Result))
            return Result;
    }
    // Validate if genref/genlock configs are consistent at board-level
    Result = DtaIoConfigUpdateValidateGenRefBoard(pDvcData, pUpdate);

    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateValidateIoDir -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaIoConfigUpdateValidateIoDir(
    DtaNonIpPort* pNonIpPort, 
    DtaIoConfigNonIpPortUpdate*  pPortUpdate,
    DtaIoConfigUpdate* pUpdate)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i, Buddy, Isi;
    DtaDeviceData*  pDvcData = pNonIpPort->m_pDvcData;
    
    DtDbgOutPort(MAX, IOCONFIG, pNonIpPort, "Configuration IODIR Value: %d SubValue: %d",
                                   pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_Value,
                                   pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_SubValue);

    switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Only non-functional ports can have no IO direction
        if (!pNonIpPort->m_IsNonFuntional)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_DISABLED:
        if (!pNonIpPort->m_CapDisabled)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_INTINPUT:
        if (!pNonIpPort->m_CapIntInput)
            return DT_STATUS_CONFIG_ERROR;
        if (pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_SubValue != DT_IOCONFIG_INTINPUT)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_MONITOR:
        if (!pNonIpPort->m_CapMonitor)
            return DT_STATUS_CONFIG_ERROR;
        if (pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_SubValue != DT_IOCONFIG_MONITOR)
            return DT_STATUS_CONFIG_ERROR;
        Buddy = (Int)pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_ParXtra[0];
        // No property to check which buddy port is valid. Hardcode based on typenumber
        // for now.
        if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber==2180 && Buddy!=3)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_INPUT:
        if (!pNonIpPort->m_CapInput)
            return DT_STATUS_CONFIG_ERROR;
            
        // If another port is set in APSK mode, we are not allowed to use this port
        for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
        {
            Int  PortIndex;
            DtaGetPortIndexNonIp(pDvcData, i, &PortIndex);
            if(PortIndex != pNonIpPort->m_PortIndex)
            {                
                if (pUpdate->m_pNonIpPortUpdate[i].m_CfgValue[DT_IOCONFIG_SWS2APSK].
                                                              m_Value == DT_IOCONFIG_TRUE)
                    return DT_STATUS_CONFIG_ERROR;
            }
        }
            
        // Validate DT_IOCONFIG_INPUT
        switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_SubValue)
        {
        case DT_IOCONFIG_INPUT:
            break;
        case DT_IOCONFIG_SHAREDANT:
            // Must be a demodulator and antenna must be selectable
            if (!pNonIpPort->m_CapDemod || !pNonIpPort->m_CapSharedAnt)
                return DT_STATUS_CONFIG_ERROR;

            Buddy = (Int)pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_ParXtra[0];
            // buddy is never our own index
            if (Buddy == pNonIpPort->m_PortIndex)
                return DT_STATUS_CONFIG_ERROR;

            if (Buddy < 0)
            {
                // Previous versions of the driver didn't check the buddy port parameter
                // correctly, and DtInfo set it to -1. To prevent breaking compatibility
                // with existing appliations we now check the BuddyPort parameter
                // properly but fix it silently when it was incorrect.
                Int  NumDemodPorts = 0;
                Int  FirstDemodPortIdx = -1;
                for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
                {
                    Int  PortIndex;
                    DtaGetPortIndexNonIp(pDvcData, i, &PortIndex);
                    if (pDvcData->m_pNonIpPorts[i].m_CapDemod)
                    {
                        NumDemodPorts++;
                        if (FirstDemodPortIdx==-1 && PortIndex!=pNonIpPort->m_PortIndex)
                            DtaGetPortIndexNonIp(pDvcData, i, &FirstDemodPortIdx);
                    }
                }
                if (NumDemodPorts == 2)
                {
                    pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_ParXtra[0] =
                                                                        FirstDemodPortIdx;
                    Buddy = FirstDemodPortIdx;
                }
            }

            // check if the buddy port exists
            Status = DtaGetNonIpPortIndex(pDvcData, Buddy, &Buddy);
            if (!DT_SUCCESS(Status))
                return DT_STATUS_CONFIG_ERROR;

            // Buddy port must be configured as an input
            if (pUpdate->m_pNonIpPortUpdate[Buddy].
                               m_CfgValue[DT_IOCONFIG_IODIR].m_Value != DT_IOCONFIG_INPUT)
                return DT_STATUS_CONFIG_ERROR;
            break;
        default:
            return DT_STATUS_CONFIG_ERROR;
        }
        break;

    case DT_IOCONFIG_OUTPUT:
        if (!pNonIpPort->m_CapOutput)
            return DT_STATUS_CONFIG_ERROR;

        switch(pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_SubValue)
        {
            case DT_IOCONFIG_OUTPUT:
                break;

            case DT_IOCONFIG_DBLBUF:
                if (!pNonIpPort->m_CapDblBuf)
                    return DT_STATUS_CONFIG_ERROR;
                // Check provided buddy is a valid non IP output port
                Buddy = (Int)pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_ParXtra[0];
                Status = DtaGetNonIpPortIndex(pDvcData, Buddy, &Buddy);
                if (DT_SUCCESS(Status))
                {
                    DtaIoConfigValue*  pCfgVal;

                    // Matrix capable ports can only double-up in pairs (i.e. port 2 can
                    // be a copy of port 1, port 4 of port 3, port 6 of port 5, etc).
                    if (pNonIpPort->m_CapMatrix)
                    {
                        Int  PortNumBuddy=-1, PortNum=-1;
                        Status = DtaGetPortNumber(pDvcData, Buddy, &PortNumBuddy);
                        DT_ASSERT(DT_SUCCESS(Status));
                        Status = DtaGetPortNumber(pDvcData, pNonIpPort->m_PortIndex, 
                                                                                &PortNum);
                        DT_ASSERT(DT_SUCCESS(Status));
                        if ((PortNumBuddy+1) != PortNum)
                        {
                            DtDbgOut(ERR, IOCONFIG, "Matrix capable double-up in pairs "
                                            "(port=%d, buddy=%d)", PortNum, PortNumBuddy);
                            return DT_STATUS_CONFIG_ERROR;
                        }
                    }

                    pCfgVal = &pUpdate->m_pNonIpPortUpdate[Buddy].
                                                            m_CfgValue[DT_IOCONFIG_IODIR];
                    if (pCfgVal->m_Value != DT_IOCONFIG_OUTPUT
                                             || pCfgVal->m_SubValue != DT_IOCONFIG_OUTPUT)
                        return DT_STATUS_CONFIG_ERROR;
                } else
                    return DT_STATUS_CONFIG_ERROR;
                break;

            case DT_IOCONFIG_LOOPS2L3:
                if (!pNonIpPort->m_CapLoopS2L3)
                    return DT_STATUS_CONFIG_ERROR;
                // Check provided buddy is a valid non IP input port
                Buddy = (Int)pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_ParXtra[0];
                Status = DtaGetNonIpPortIndex(pDvcData, Buddy, &Buddy);
                if (DT_SUCCESS(Status))
                {
                    if (pUpdate->m_pNonIpPortUpdate[Buddy].m_CfgValue[DT_IOCONFIG_IODIR].
                                                             m_Value != DT_IOCONFIG_INPUT)
                        return DT_STATUS_CONFIG_ERROR;
                } else
                    return DT_STATUS_CONFIG_ERROR;
                break;

            case DT_IOCONFIG_LOOPS2TS:
                if (!pNonIpPort->m_CapLoopS2Ts)
                    return DT_STATUS_CONFIG_ERROR;
                // Check provided buddy is a valid non IP input port
                Buddy = (Int)pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_ParXtra[0];
                Status = DtaGetNonIpPortIndex(pDvcData, Buddy, &Buddy);
                if (DT_SUCCESS(Status))
                {
                    if (pUpdate->m_pNonIpPortUpdate[Buddy].
                               m_CfgValue[DT_IOCONFIG_IODIR].m_Value != DT_IOCONFIG_INPUT)
                        return DT_STATUS_CONFIG_ERROR;
                    // Check ISI
                    Isi = (Int)pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_ParXtra[1];
                    if (Isi<0 || Isi>255)
                        return DT_STATUS_CONFIG_ERROR;
                } else
                    return DT_STATUS_CONFIG_ERROR;
                break;

            case DT_IOCONFIG_LOOPTHR:
                if (!pNonIpPort->m_CapLoopThr)
                    return DT_STATUS_CONFIG_ERROR;
                // Check provided buddy is a valid non IP input port
                Buddy = (Int)pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_ParXtra[0];
                Status = DtaGetNonIpPortIndex(pDvcData, Buddy, &Buddy);
                if (DT_SUCCESS(Status))
                {
                    if (pUpdate->m_pNonIpPortUpdate[Buddy].m_CfgValue[DT_IOCONFIG_IODIR].
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateValidateIoStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaIoConfigUpdateValidateIoStd(
    DtaNonIpPort* pNonIpPort, 
    DtaIoConfigNonIpPortUpdate*  pPortUpdate,
    DtaIoConfigUpdate* pUpdate)
{
    DtDbgOutPort(MAX, IOCONFIG, pNonIpPort, "Configuration IOSTD Value: %d SubValue: %d",
                                   pPortUpdate->m_CfgValue[DT_IOCONFIG_IOSTD].m_Value,
                                   pPortUpdate->m_CfgValue[DT_IOCONFIG_IOSTD].m_SubValue);

    switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_IOSTD].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Only non-functional ports can have no IO standard
        if (!pNonIpPort->m_IsNonFuntional)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_3GSDI:
        if (!pNonIpPort->m_Cap3GSdi)
            return DT_STATUS_CONFIG_ERROR;

        // Validate DT_IOCONFIG_HDSDI
        switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_IOSTD].m_SubValue)
        {
        case DT_IOCONFIG_1080P50:
            if (!pNonIpPort->m_Cap1080P50)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P50B:
            if (!pNonIpPort->m_Cap1080P50B)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P59_94:
            if (!pNonIpPort->m_Cap1080P59_94)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P59_94B:
            if (!pNonIpPort->m_Cap1080P59_94B)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P60:
            if (!pNonIpPort->m_Cap1080P60)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P60B:
            if (!pNonIpPort->m_Cap1080P60B)
                return DT_STATUS_CONFIG_ERROR;
            break;
        default:
            return DT_STATUS_CONFIG_ERROR;
        }
        break;
    case DT_IOCONFIG_ASI:
        if (!pNonIpPort->m_CapAsi)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_AVENC:
        if (!pNonIpPort->m_CapAvEnc)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_DEMOD:
        if (!pNonIpPort->m_CapDemod)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_GPSTIME:
        if (!pNonIpPort->m_CapGpsTime)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_HDMI:
        if (!pNonIpPort->m_CapHdmi)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_HDSDI:
        if (!pNonIpPort->m_CapHdSdi)
            return DT_STATUS_CONFIG_ERROR;

        // Validate DT_IOCONFIG_HDSDI
        switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_IOSTD].m_SubValue)
        {
        case DT_IOCONFIG_1080I50:
            if (!pNonIpPort->m_Cap1080I50)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080I59_94:
            if (!pNonIpPort->m_Cap1080I59_94)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080I60:
            if (!pNonIpPort->m_Cap1080I60)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P23_98:
            if (!pNonIpPort->m_Cap1080P23_98)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P24:
            if (!pNonIpPort->m_Cap1080P24)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P25:
            if (!pNonIpPort->m_Cap1080P25)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P29_97:
            if (!pNonIpPort->m_Cap1080P29_97)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P30:
            if (!pNonIpPort->m_Cap1080P30)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080PSF23_98:
            if (!pNonIpPort->m_Cap1080Psf23_98)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080PSF24:
            if (!pNonIpPort->m_Cap1080Psf24)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080PSF25:
            if (!pNonIpPort->m_Cap1080Psf25)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080PSF29_97:
            if (!pNonIpPort->m_Cap1080Psf29_97)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080PSF30:
            if (!pNonIpPort->m_Cap1080Psf30)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P23_98:
            if (!pNonIpPort->m_Cap720P23_98)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P24:
            if (!pNonIpPort->m_Cap720P24)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P25:
            if (!pNonIpPort->m_Cap720P25)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P29_97:
            if (!pNonIpPort->m_Cap720P29_97)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P30:
            if (!pNonIpPort->m_Cap720P30)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P50:
            if (!pNonIpPort->m_Cap720P50)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P59_94:
            if (!pNonIpPort->m_Cap720P59_94)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_720P60:
            if (!pNonIpPort->m_Cap720P60)
                return DT_STATUS_CONFIG_ERROR;
            break;
        default:
            return DT_STATUS_CONFIG_ERROR;
        }
        break;
    case DT_IOCONFIG_IFADC:
        if (!pNonIpPort->m_CapIfAdc)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_IP:
        if (!pNonIpPort->m_CapIp)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_MOD:
        if (!pNonIpPort->m_CapMod)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_PHASENOISE:
        if (!pNonIpPort->m_CapPhaseNoise)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_RS422:
        if (!pNonIpPort->m_CapRs422)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SDI:        
        if (!pNonIpPort->m_CapSdi)
            return DT_STATUS_CONFIG_ERROR;

        // Validate DT_IOCONFIG_SDI
        switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_IOSTD].m_SubValue)
        {
        case DT_IOCONFIG_525I59_94:
            if (!pNonIpPort->m_Cap525I59_94)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_625I50:
            if (!pNonIpPort->m_Cap625I50)
                return DT_STATUS_CONFIG_ERROR;
            break;
        default:
            return DT_STATUS_CONFIG_ERROR;
        }
        break;
    case DT_IOCONFIG_SDIRX:
        if (!pNonIpPort->m_CapSdiRx)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPI:
        if (!pNonIpPort->m_CapSpi)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPISDI:           
        if (!pNonIpPort->m_CapSpiSdi)
            return DT_STATUS_CONFIG_ERROR;

        // Validate DT_IOCONFIG_SPISDI
        switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_IOSTD].m_SubValue)
        {
        case DT_IOCONFIG_SPI525I59_94:
            if (!pNonIpPort->m_CapSpi525I59_94)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_SPI625I50:
            if (!pNonIpPort->m_CapSpi625I50)
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

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateValidateRfClkSel -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaIoConfigUpdateValidateRfClkSel(
    DtaNonIpPort* pNonIpPort, 
    DtaIoConfigNonIpPortUpdate*  pPortUpdate,
    DtaIoConfigUpdate* pUpdate)
{
    DtDbgOutPort(MAX, IOCONFIG, pNonIpPort, "Configuration RFCLKSEL Value: %d SubValue: %d",
                                pPortUpdate->m_CfgValue[DT_IOCONFIG_RFCLKSEL].m_Value,
                                pPortUpdate->m_CfgValue[DT_IOCONFIG_RFCLKSEL].m_SubValue);

    switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_RFCLKSEL].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Not applicable should only be set when we do not support ext RF clock
        DT_ASSERT(!pNonIpPort->m_CapRfClkInt && !pNonIpPort->m_CapRfClkExt);
        break;
    case DT_IOCONFIG_RFCLKINT:
        if (!pNonIpPort->m_CapRfClkInt)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_RFCLKEXT:
        if (!pNonIpPort->m_CapRfClkExt)
            return DT_STATUS_CONFIG_ERROR;
        break;
    default:
        return DT_STATUS_CONFIG_ERROR;
    }

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateValidateSpiClkSel -.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaIoConfigUpdateValidateSpiClkSel(
    DtaNonIpPort* pNonIpPort, 
    DtaIoConfigNonIpPortUpdate*  pPortUpdate,
    DtaIoConfigUpdate* pUpdate)
{
    DtDbgOutPort(MAX, IOCONFIG, pNonIpPort, "Configuration SPICLKSEL Value: %d SubValue: %d",
                               pPortUpdate->m_CfgValue[DT_IOCONFIG_SPICLKSEL].m_Value,
                               pPortUpdate->m_CfgValue[DT_IOCONFIG_SPICLKSEL].m_SubValue);

    switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_SPICLKSEL].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Not applicable should only be set when we do not support SPI
        DT_ASSERT(!pNonIpPort->m_CapSpiClkInt && !pNonIpPort->m_CapSpiClkExt);
        break;
    case DT_IOCONFIG_SPICLKINT:
        if (!pNonIpPort->m_CapSpiClkInt)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPICLKEXT:
        if (!pNonIpPort->m_CapSpiClkExt)
            return DT_STATUS_CONFIG_ERROR;
        // TODO Extra checks needed for DTA-2142
        break;
    default:
        return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateValidateSpiMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaIoConfigUpdateValidateSpiMode(
    DtaNonIpPort* pNonIpPort, 
    DtaIoConfigNonIpPortUpdate*  pPortUpdate,
    DtaIoConfigUpdate* pUpdate)
{
    DtDbgOutPort(MAX, IOCONFIG, pNonIpPort, "Configuration SPIMODE Value: %d SubValue: %d",
                                 pPortUpdate->m_CfgValue[DT_IOCONFIG_SPIMODE].m_Value,
                                 pPortUpdate->m_CfgValue[DT_IOCONFIG_SPIMODE].m_SubValue);

    switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_SPIMODE].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Not applicable should only be set when we do not support SPI
        DT_ASSERT(!pNonIpPort->m_CapSpi);
        break;
    case DT_IOCONFIG_SPIDVBMODE:
        if (!pNonIpPort->m_CapSpiDvbMode)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPIFIXEDCLK:
        if (!pNonIpPort->m_CapSpiFixedClk)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPISER8B:
        if (!pNonIpPort->m_CapSpiSer8B)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPISER10B:
        if (!pNonIpPort->m_CapSpiSer10B)
            return DT_STATUS_CONFIG_ERROR;
        break;
    default:
        return DT_STATUS_CONFIG_ERROR;
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateValidateSpiStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaIoConfigUpdateValidateSpiStd(
    DtaNonIpPort* pNonIpPort, 
    DtaIoConfigNonIpPortUpdate*  pPortUpdate,
    DtaIoConfigUpdate* pUpdate)
{
    DtDbgOutPort(MAX, IOCONFIG, pNonIpPort, "Configuration SPISTD Value: %d SubValue: %d",
                                  pPortUpdate->m_CfgValue[DT_IOCONFIG_SPISTD].m_Value,
                                  pPortUpdate->m_CfgValue[DT_IOCONFIG_SPISTD].m_SubValue);

    switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_SPISTD].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Not applicable should only be set when we do not support SPI
        DT_ASSERT(!pNonIpPort->m_CapSpi);
        break;
    case DT_IOCONFIG_SPILVDS1:
        if (!pNonIpPort->m_CapSpiLvds1)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPILVDS2:
        if (!pNonIpPort->m_CapSpiLvds2)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_SPILVTTL:
        if (!pNonIpPort->m_CapSpiLvttl)
            return DT_STATUS_CONFIG_ERROR;
        break;
    default:
        return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateValidateTsRateSel -.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaIoConfigUpdateValidateTsRateSel(
    DtaNonIpPort* pNonIpPort, 
    DtaIoConfigNonIpPortUpdate*  pPortUpdate,
    DtaIoConfigUpdate* pUpdate)
{
    DtDbgOutPort(MAX, IOCONFIG, pNonIpPort, "Configuration TSRATESEL Value: %d SubValue: %d",
                               pPortUpdate->m_CfgValue[DT_IOCONFIG_TSRATESEL].m_Value,
                               pPortUpdate->m_CfgValue[DT_IOCONFIG_TSRATESEL].m_SubValue);

    switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_TSRATESEL].m_Value)
    {
    case DT_IOCONFIG_NONE:
        break;
    case DT_IOCONFIG_INTTSRATE:
        if (!pNonIpPort->m_CapIntTsRate)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_EXTTSRATE:
        if (!pNonIpPort->m_CapExtTsRate)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_EXTRATIO:
        if (!pNonIpPort->m_CapExtRatio)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_LOCK2INP:
        if (!pNonIpPort->m_CapLock2Inp)
            return DT_STATUS_CONFIG_ERROR;
        break;
    default:
        return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateValidateSwS2Apsk -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaIoConfigUpdateValidateSwS2Apsk(
    DtaNonIpPort* pNonIpPort, 
    DtaIoConfigNonIpPortUpdate*  pPortUpdate,
    DtaIoConfigUpdate* pUpdate)
{
    Int  i;
    DtaNonIpPort*  pNonIpBuddyPort;
    DtaDeviceData*  pDvcData = pNonIpPort->m_pDvcData;
    
    DtDbgOutPort(MAX, IOCONFIG, pNonIpPort, "Configuration SWS2APSK Value: %d SubValue: %d",
                                pPortUpdate->m_CfgValue[DT_IOCONFIG_SWS2APSK].m_Value,
                                pPortUpdate->m_CfgValue[DT_IOCONFIG_SWS2APSK].m_SubValue);

    switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_SWS2APSK].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Not applicable should only be set when we do not support APSK configuration
        DT_ASSERT(!pNonIpPort->m_CapSwS2Apsk);
        break;
    case DT_IOCONFIG_FALSE:
        // Must be a demodulator and antenna must support APSK configuration
        if (!pNonIpPort->m_CapSwS2Apsk)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_TRUE:
        // Must be a demodulator and antenna must support APSK configuration
        if (!pNonIpPort->m_CapSwS2Apsk)
            return DT_STATUS_CONFIG_ERROR;

        // find all other input channels. 
        // - Only one is allowed in APSK mode
        // - All other input channels must be disabled
        for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
        {
            pNonIpBuddyPort = &pDvcData->m_pNonIpPorts[i];
            if(i != pNonIpPort->m_PortIndex)
            {
                if (pNonIpBuddyPort->m_CapInput && pUpdate->m_pNonIpPortUpdate[i].
                            m_CfgValue[DT_IOCONFIG_IODIR].m_Value != DT_IOCONFIG_DISABLED)
                    return DT_STATUS_CONFIG_ERROR;

                if (pUpdate->m_pNonIpPortUpdate[i].m_CfgValue[DT_IOCONFIG_SWS2APSK].
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

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateValidateFailSafe -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaIoConfigUpdateValidateFailSafe(
    DtaNonIpPort* pNonIpPort, 
    DtaIoConfigNonIpPortUpdate*  pPortUpdate,
    DtaIoConfigUpdate* pUpdate)
{
    DtDbgOutPort(MAX, IOCONFIG, pNonIpPort, "Configuration FAILSAFE Value: %d SubValue: %d",
                                pPortUpdate->m_CfgValue[DT_IOCONFIG_FAILSAFE].m_Value,
                                pPortUpdate->m_CfgValue[DT_IOCONFIG_FAILSAFE].m_SubValue);

    switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_FAILSAFE].m_Value)
    {
    case DT_IOCONFIG_NONE:
        // Not applicable should only be set when we do not support Failsafe
        DT_ASSERT(!pNonIpPort->m_CapFailSafe);
        break;
    case DT_IOCONFIG_FALSE:
        if (!pNonIpPort->m_CapFailSafe)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_TRUE:
        if (!pNonIpPort->m_CapFailSafe)
            return DT_STATUS_CONFIG_ERROR;
        break;
    default:
        return DT_STATUS_CONFIG_ERROR;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateValidateGenLocked -.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaIoConfigUpdateValidateGenLocked(
    DtaNonIpPort* pNonIpPort, 
    DtaIoConfigNonIpPortUpdate*  pPortUpdate,
    DtaIoConfigUpdate* pUpdate)
{
    DtDbgOutPort(MAX, IOCONFIG, pNonIpPort, "Configuration GENLOCKED Value: %d SubValue: %d",
                               pPortUpdate->m_CfgValue[DT_IOCONFIG_GENLOCKED].m_Value,
                               pPortUpdate->m_CfgValue[DT_IOCONFIG_GENLOCKED].m_SubValue);

    switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_GENLOCKED].m_Value)
    {
    case DT_IOCONFIG_NONE:
        break;
    case DT_IOCONFIG_FALSE:
        if (!pNonIpPort->m_CapGenLocked)
            return DT_STATUS_CONFIG_ERROR;
        break;
    case DT_IOCONFIG_TRUE:
        if (!pNonIpPort->m_CapGenLocked)
            return DT_STATUS_CONFIG_ERROR;
        break;
    default:
        return DT_STATUS_CONFIG_ERROR;
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateValidateGenRef -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaIoConfigUpdateValidateGenRef(
    DtaNonIpPort* pNonIpPort, 
    DtaIoConfigNonIpPortUpdate*  pPortUpdate,
    DtaIoConfigUpdate* pUpdate)
{
    Int  IoStdVal;
    DtDbgOutPort(MAX, IOCONFIG, pNonIpPort, "Configuration GENREF Value: %d SubValue: %d",
                                  pPortUpdate->m_CfgValue[DT_IOCONFIG_GENREF].m_Value,
                                  pPortUpdate->m_CfgValue[DT_IOCONFIG_GENREF].m_SubValue);
    
    switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_GENREF].m_Value)
    {
    case DT_IOCONFIG_NONE:
        break;

    case DT_IOCONFIG_FALSE:
        if (!pNonIpPort->m_CapGenRef)
        {
            DtDbgOut(ERR, IOCONFIG, "Genref not supported for port %i",
                                                                 pNonIpPort->m_PortIndex);
            return DT_STATUS_CONFIG_ERROR;
        }
        break;

    case DT_IOCONFIG_TRUE:
        if (!pNonIpPort->m_CapGenRef)
        {
            DtDbgOut(ERR, IOCONFIG, "Genref not supported for port %i",
                                                                 pNonIpPort->m_PortIndex);
            return DT_STATUS_CONFIG_ERROR;
        }

        // Port must be configured as input or must be a non-functional port
        if (pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_Value!=DT_IOCONFIG_INPUT 
                                                         && !pNonIpPort->m_IsNonFuntional)
        {
            DtDbgOut(ERR, IOCONFIG, "Port %i is not an input", pNonIpPort->m_PortIndex);
            return DT_STATUS_CONFIG_ERROR;
        }
        // Port must be configured for SDI
        IoStdVal = pPortUpdate->m_CfgValue[DT_IOCONFIG_IOSTD].m_Value;
        if (IoStdVal!=DT_IOCONFIG_SDI && IoStdVal!=DT_IOCONFIG_HDSDI &&
                                                              IoStdVal!=DT_IOCONFIG_3GSDI)
        {
            DtDbgOut(ERR, IOCONFIG, "Port %i is not an input", pNonIpPort->m_PortIndex);
            return DT_STATUS_CONFIG_ERROR;
        }
        break;

    default:
        DtDbgOut(ERR, IOCONFIG, "Unknown config value %i for port %i",
                                      pPortUpdate->m_CfgValue[DT_IOCONFIG_GENREF].m_Value,
                                      pNonIpPort->m_PortIndex);
        return DT_STATUS_CONFIG_ERROR;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateValidateFracMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaIoConfigUpdateValidateFracMode(
    DtaNonIpPort* pNonIpPort, 
    DtaIoConfigNonIpPortUpdate*  pPortUpdate,
    DtaIoConfigUpdate* pUpdate)
{
    Int  i;
    DtaNonIpPort*  pOtherIpPort = NULL;
    DtaDeviceData*  pDvcData = pNonIpPort->m_pDvcData;
    
    DtDbgOutPort(MAX, IOCONFIG, pNonIpPort, "Configuration FRACMODE Value: %d SubValue: %d",
                                pPortUpdate->m_CfgValue[DT_IOCONFIG_FRACMODE].m_Value,
                                pPortUpdate->m_CfgValue[DT_IOCONFIG_FRACMODE].m_SubValue);

    switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_FRACMODE].m_Value)
    {
    case DT_IOCONFIG_NONE:
        break;

    case DT_IOCONFIG_FALSE:
    case DT_IOCONFIG_TRUE:
        // Must support fractional mode
        if (!pNonIpPort->m_CapFracMode)
            return DT_STATUS_CONFIG_ERROR;
        
        // All ports must be configured the same (i.e. fractional mode must be enabled 
        // or disabled for everyone)
        for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
        {
            pOtherIpPort = &pDvcData->m_pNonIpPorts[i];
            if (pOtherIpPort->m_PortIndex == pNonIpPort->m_PortIndex)
                continue;   // Skip ourselves
            if (!pOtherIpPort->m_CapFracMode)
                continue;   // Skip port that does not support fractional mode

            // Check io-config value for other port matches
            if (pUpdate->m_pNonIpPortUpdate[i].m_CfgValue[DT_IOCONFIG_FRACMODE].m_Value
                                 != pPortUpdate->m_CfgValue[DT_IOCONFIG_FRACMODE].m_Value)
                return DT_STATUS_CONFIG_ERROR;
        }
        break;

    default:
        return DT_STATUS_CONFIG_ERROR;
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigUpdateValidateGenRefBoard -.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaIoConfigUpdateValidateGenRefBoard(
    DtaDeviceData*  pDvcData,
    DtaIoConfigUpdate* pUpdate)
{
    Int  i, NumGenlock=0, NumGenRef=0;
    Bool  HasVirtualGenref = FALSE;

    // You cannot disable Genref if an output port has Genlocked enabled
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        DtaNonIpPort*  pNonIpPort = &pDvcData->m_pNonIpPorts[i];
        if (pNonIpPort->m_CapGenRef)
        {
            if (pUpdate->m_pNonIpPortUpdate[i].m_CfgValue[DT_IOCONFIG_GENREF].m_Value
                                                                      == DT_IOCONFIG_TRUE)
                NumGenRef++;
            // Check for a virtual GENREF that is not a slave
            if (pNonIpPort->m_CapVirtual && !pNonIpPort->m_CapGenRefSlave)
                HasVirtualGenref = TRUE;
        }
        if (pNonIpPort->m_CapGenLocked)
        {
            if (pUpdate->m_pNonIpPortUpdate[i].m_CfgValue[DT_IOCONFIG_GENLOCKED].m_Value
                                                                      == DT_IOCONFIG_TRUE)
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
    // Ignore this for sub-devices.
    if (NumGenlock>0 && NumGenRef==0 && pDvcData->m_DevInfo.m_SubDvc==0)
        return DT_STATUS_CONFIG_ERROR;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigWriteToNonVolatileStorage -.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaIoConfigWriteToNonVolatileStorage(
    DtaNonIpPort*  pNonIpPort,
    Int  IoGroup,
    DtaIoConfigValue  CfgValue)
{
    DtStatus  Result;
    Int  ParXtraIdx;
    DtaDeviceData*  pDvcData = pNonIpPort->m_pDvcData;
    Char StrValue[IOCONFIG_NAME_MAX_SIZE];
    Char GroupName[IOCONFIG_NAME_MAX_SIZE];

 
    // Get the IO group name
    Result = IoConfigNameGet(IoGroup, GroupName, sizeof(GroupName) );
    if (!DT_SUCCESS(Result))
        return Result;
 
    // Write config value
    Result = IoConfigNameGet(CfgValue.m_Value, StrValue, sizeof(StrValue));
    if (!DT_SUCCESS(Result))
        return Result;

    Result = DtNonVolatileSettingsStringWrite(&pDvcData->m_Driver, 
                                  pDvcData->m_DevInfo.m_UniqueId, pNonIpPort->m_PortIndex,
                                                      GroupName, "ConfigValue", StrValue);
    if (!DT_SUCCESS(Result))
        return Result;

    // Write config subvalue
    Result = IoConfigNameGet(CfgValue.m_SubValue, StrValue, sizeof(StrValue));
    if (!DT_SUCCESS(Result))
        return Result;

    Result = DtNonVolatileSettingsStringWrite(&pDvcData->m_Driver, 
                                  pDvcData->m_DevInfo.m_UniqueId, pNonIpPort->m_PortIndex,
                                                   GroupName, "ConfigSubValue", StrValue);
    if (!DT_SUCCESS(Result))
        return Result;
   
    // Write ParXtra
    for (ParXtraIdx=0; ParXtraIdx<DT_MAX_PARXTRA_COUNT; ParXtraIdx++)
    {
        Result = DtNonVolatileSettingsValueWrite(&pDvcData->m_Driver,
                                  pDvcData->m_DevInfo.m_UniqueId, pNonIpPort->m_PortIndex,
                                             GroupName, (Char*)IoParXtraNames[ParXtraIdx], 
                                                          CfgValue.m_ParXtra[ParXtraIdx]);
        if (!DT_SUCCESS(Result))
            return Result;
    }

    return DT_STATUS_OK;
}

