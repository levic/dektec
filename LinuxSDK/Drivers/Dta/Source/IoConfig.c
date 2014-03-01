//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* IoConfig.c *#*#*#*#*#*#*#*#* (C) 2010-2012 DekTec
//
// Dta driver - IO configuration - Definition of IO configuration types/functions
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2012 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//  3. The source code may not be modified for the express purpose of enabling hardware
//     features for which no genuine license has been obtained.
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
    DtaIoConfigNonIpPortUpdate  m_NonIpPortUpdate[MAX_NONIP_PORTS];
} DtaIoConfigUpdate;

// DtaIoConfigStorageNames
typedef struct _DtaIoConfigStorageNames {
    Char*  m_pCategory;
    Char*  m_pValueName;
    Char*  m_pIoCfgParXtraName[DT_MAX_PARXTRA_COUNT];
} DtaIoConfigStorageNames;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtaIoConfigReadFromNonVolatileStorage(DtaNonIpPort* pNonIpPort, 
                                                Int IoGroup, DtaIoConfigValue* pCfgValue);
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
static Bool  DtaIoConfigIsFractionalIoStd(Int  Value, Int  SubValue);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Global data -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// ParXtra names
static const char*   IoParXtraNames[DT_MAX_PARXTRA_COUNT] = {
        "IoParXtra0", 
        "IoParXtra1", 
        "IoParXtra2", 
        "IoParXtra3" };

// Update structure
static DtaIoConfigUpdate  Update;


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

    // Load cache (initial cache holds the defaults which are not already applied)
    DT_RETURN_ON_ERROR(DtaIoConfigUpdateLoadCache(pDvcData, &Update));

    // Registry is only read after a cold start, else the cache already holds the correct
    // IO configurations.
    if (pDvcData->m_InitialPowerup)
    {
        // Load non volatile storage settings
        DT_RETURN_ON_ERROR(DtaIoConfigUpdateLoadNonVolatileStorage(pDvcData, &Update));

        // Validate configuration after registry manipulations are done
        Result = DtaIoConfigUpdateValidate(pDvcData, &Update);
        if (!DT_SUCCESS(Result))
        {
            DtDbgOut(ERR, IOCONFIG, "Configuration invalid. Apply initial configuration");
            // Reload cache (initial cache holds the defaults which are not already
            // applied)
            DT_RETURN_ON_ERROR(DtaIoConfigUpdateLoadCache(pDvcData, &Update));

            // Delete the registry settings for this device.
            if (!DT_SUCCESS(DtNonVolatileSettingsDelete(&pDvcData->m_Driver, 
                                                           pDvcData->m_DevInfo.m_Serial,
                                                           pDvcData->m_NumNonIpPorts+
                                                           pDvcData->m_NumIpPorts)))
                DtDbgOut(ERR, IOCONFIG, "Error deleting registry key");
                DT_ASSERT(FALSE);
        }
    }

    // Set UpdateNeeded flag for all IO configurations during init...
    for (NonIpIndex=0; NonIpIndex<pDvcData->m_NumNonIpPorts; NonIpIndex++)
    {
        pNonIpPort = &pDvcData->m_NonIpPorts[NonIpIndex];
        pPortUpdate = &Update.m_NonIpPortUpdate[pNonIpPort->m_PortIndex];
        for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
        {
            // Skip IO configs not applicable to the current port type
            if (pPortUpdate->m_CfgValue[IoConfig].m_Value == DT_IOCONFIG_NONE)
                continue;
            pPortUpdate->m_UpdateNeeded[IoConfig] = TRUE;
        }
    }

    // Apply (without saving everything to the non volatile storage)
    return DtaIoConfigUpdateApply(pDvcData, &Update, FALSE);
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
            pNonIpPort = &pDvcData->m_NonIpPorts[NonIpIndex];

            // Get the IO configuration and copy it to the caller variables
            Result = DtaNonIpIoConfigGet(pNonIpPort, IoGroup, &DtaIoCfgVal);
        } 
        else 
        {
            // Check IP port
            Result = DtaGetIpPortIndex(pDvcData, pIoCfgId[i].m_PortIndex, &IpIndex);
            if (!DT_SUCCESS(Result))
                return Result;

            pIpPort = &pDvcData->m_IpDevice.m_IpPorts[IpIndex];
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

    DtaIoConfigNonIpPortUpdate*  pPortUpdate;
    DtaIoConfigValue  IpDataConfigValue;
    DtaIpPort*  pIpPort;
    Int  IpIndex;
    Int  IoGroup;
    Int  ConfigValue;
    Int  ConfigSubValue;
    Int  ParXtra;
    

    // Get exclusive access lock to prevent ports from being opened/closed and other
    // IO configs from being applied
    Result = DtFastMutexAcquire(&pDvcData->m_ExclAccessMutex);
    if (pDvcData->m_RegistryWriteBusy)
    {
        DtFastMutexRelease(&pDvcData->m_ExclAccessMutex);
        return DT_STATUS_BUSY;
    }
    
    // Check if we are dealing with an IP port
    for (Cfg=0; Cfg<Count; Cfg++)
    {
        pCfg = &pIoConfigs[Cfg];
        Result = DtaGetIpPortIndex(pDvcData, pCfg->m_PortIndex, &IpIndex);
        if (!DT_SUCCESS(Result)) // continue with Non IP
        {
            Result = DT_STATUS_OK;
            break;
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


        pIpPort = &pDvcData->m_IpDevice.m_IpPorts[IpIndex];
        IpDataConfigValue.m_Value = ConfigValue;
        IpDataConfigValue.m_SubValue = ConfigSubValue;
        for (ParXtra=0; ParXtra<DT_MAX_PARXTRA_COUNT; ParXtra++)
            IpDataConfigValue.m_ParXtra[ParXtra] = pCfg->m_ParXtra[ParXtra];

        Result = DtaIpIoConfigSet(pIpPort, IoGroup, IpDataConfigValue);
        DtFastMutexRelease(&pDvcData->m_ExclAccessMutex);
        return Result;
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
                break;

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


            pNonIpPort = &pDvcData->m_NonIpPorts[NonIpIndex];
            pPortUpdate = &Update.m_NonIpPortUpdate[NonIpIndex];

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
                        DtDbgOut(ERR, IOCONFIG, "Port configuration needs to be updated, but"
                                               " port %i is in use", pNonIpPort->m_PortIndex);
                        break;
                    }
                }

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

    // Release exclusive access mutex
    DtFastMutexRelease(&pDvcData->m_ExclAccessMutex);

    if (DT_SUCCESS(Result))
        Result = DtaIoConfigUpdateApply(pDvcData, &Update, TRUE);

    return Result;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private helpers +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigReadFromNonVolatileStorage -.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaIoConfigReadFromNonVolatileStorage(
    DtaNonIpPort*  pNonIpPort,
    Int  IoGroup,
    DtaIoConfigValue*  pCfgValue)
{
    DtStatus  Result;
    Int32  Value;
    Int64  BinValue;
    Char StrValue[IOCONFIG_NAME_MAX_SIZE];
    Int  ParXtraIdx;
    DtaIoConfigValue  IoConfig;
    DtaDeviceData*  pDvcData = pNonIpPort->m_pDvcData;
    Char GroupName[IOCONFIG_NAME_MAX_SIZE];

    
    // Get the IO group name
    Result = IoConfigNameGet(IoGroup, GroupName, sizeof(GroupName) );
    if (!DT_SUCCESS(Result))
        return Result;

    // Get config value
    Result = DtNonVolatileSettingsStringRead(&pDvcData->m_Driver, 
                                    pDvcData->m_DevInfo.m_Serial, pNonIpPort->m_PortIndex,
                                    GroupName, "ConfigValue", StrValue, sizeof(StrValue));
    if (!DT_SUCCESS(Result))
        return Result;

    Result = IoConfigCodeGet(StrValue, &Value);
    if (!DT_SUCCESS(Result))
        return Result;
    IoConfig.m_Value = (Int)Value;

    // Get config subvalue
    Result = DtNonVolatileSettingsStringRead(&pDvcData->m_Driver, 
                                 pDvcData->m_DevInfo.m_Serial, pNonIpPort->m_PortIndex,
                                 GroupName, "ConfigSubValue", StrValue, sizeof(StrValue));
    if (!DT_SUCCESS(Result))
        return Result;

    Result = IoConfigCodeGet(StrValue, &Value);
    if (!DT_SUCCESS(Result))
        return Result;
    IoConfig.m_SubValue = Value;
    
    // Read ParXtra
    for (ParXtraIdx=0; ParXtraIdx<DT_MAX_PARXTRA_COUNT; ParXtraIdx++)
    {
        Result = DtNonVolatileSettingsValueRead(&pDvcData->m_Driver,
                                 pDvcData->m_DevInfo.m_Serial, pNonIpPort->m_PortIndex,
                                 GroupName, (Char*)IoParXtraNames[ParXtraIdx], &BinValue);
        if (!DT_SUCCESS(Result))
            return DT_STATUS_CONFIG_ERROR;
        IoConfig.m_ParXtra[ParXtraIdx] = BinValue;
    }

    // Copy result only if succeeded
    *pCfgValue = IoConfig;

    return DT_STATUS_OK;
}

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
        pNonIpPort = &pDvcData->m_NonIpPorts[NonIpIndex];
        pPortUpdate = &pUpdate->m_NonIpPortUpdate[NonIpIndex];
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
        pNonIpPort = &pDvcData->m_NonIpPorts[NonIpIndex];
        pPortUpdate = &pUpdate->m_NonIpPortUpdate[NonIpIndex];

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
    Int  IoConfig;
    DtaNonIpPort*  pNonIpPort;
    DtaIoConfigNonIpPortUpdate*  pPortUpdate = NULL;

    for (NonIpIndex=0; NonIpIndex<pDvcData->m_NumNonIpPorts; NonIpIndex++)
    {
        pNonIpPort = &pDvcData->m_NonIpPorts[NonIpIndex];
        pPortUpdate = &pUpdate->m_NonIpPortUpdate[NonIpIndex];
        for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
        {
            Result = DtaIoConfigReadFromNonVolatileStorage(pNonIpPort, IoConfig, 
                                                      &pPortUpdate->m_CfgValue[IoConfig]);
            if (!DT_SUCCESS(Result) && Result!=DT_STATUS_NOT_FOUND)
                break;
        }
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
        pNonIpPort = &pDvcData->m_NonIpPorts[NonIpPortIndex];
        pPortUpdate = &pUpdate->m_NonIpPortUpdate[NonIpPortIndex];
                
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
    
    DtDbgOut(MAX, IOCONFIG, "Configuration IODIR Value: %d SubValue: %d",
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
    case DT_IOCONFIG_INPUT:
        if (!pNonIpPort->m_CapInput)
            return DT_STATUS_CONFIG_ERROR;
            
        // If another port is set in APSK mode, we are not allowed to use this port
        if (!pNonIpPort->m_CapSwS2Apsk)
            break;            
        for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
        {                
            if(i != pNonIpPort->m_PortIndex)
            {                
                if (pUpdate->m_NonIpPortUpdate[i].m_CfgValue[DT_IOCONFIG_SWS2APSK].
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

            // check if the buddy port exists
            Status = DtaGetNonIpPortIndex(pDvcData, Buddy, &Buddy);
            if (!DT_SUCCESS(Status))
                return DT_STATUS_CONFIG_ERROR;

            // Buddy port must be configured as an input
            if (pUpdate->m_NonIpPortUpdate[Buddy].
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
                    if (pUpdate->m_NonIpPortUpdate[Buddy].
                              m_CfgValue[DT_IOCONFIG_IODIR].m_Value != DT_IOCONFIG_OUTPUT)
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
                    if (pUpdate->m_NonIpPortUpdate[Buddy].m_CfgValue[DT_IOCONFIG_IODIR].
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
                    if (pUpdate->m_NonIpPortUpdate[Buddy].
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
                    if (pUpdate->m_NonIpPortUpdate[Buddy].m_CfgValue[DT_IOCONFIG_IODIR].
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
    DtDbgOut(MAX, IOCONFIG, "Configuration IOSTD Value: %d SubValue: %d",
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
        case DT_IOCONFIG_1080P59_94:
            if (!pNonIpPort->m_Cap1080P59_94)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_1080P60:
            if (!pNonIpPort->m_Cap1080P60)
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
    case DT_IOCONFIG_DEMOD:
        if (!pNonIpPort->m_CapDemod)
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
    DtDbgOut(MAX, IOCONFIG, "Configuration RFCLKSEL Value: %d SubValue: %d",
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
    DtDbgOut(MAX, IOCONFIG, "Configuration SPICLKSEL Value: %d SubValue: %d",
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
    DtDbgOut(MAX, IOCONFIG, "Configuration SPIMODE Value: %d SubValue: %d",
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
    DtDbgOut(MAX, IOCONFIG, "Configuration SPISTD Value: %d SubValue: %d",
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
    DtDbgOut(MAX, IOCONFIG, "Configuration TSRATESEL Value: %d SubValue: %d",
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
    
    DtDbgOut(MAX, IOCONFIG, "Configuration SWS2APSK Value: %d SubValue: %d",
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
            pNonIpBuddyPort = &pDvcData->m_NonIpPorts[i];
            if(i != pNonIpPort->m_PortIndex)
            {
                if (pNonIpBuddyPort->m_CapInput && pUpdate->m_NonIpPortUpdate[i].
                            m_CfgValue[DT_IOCONFIG_IODIR].m_Value != DT_IOCONFIG_DISABLED)
                    return DT_STATUS_CONFIG_ERROR;

                if (pUpdate->m_NonIpPortUpdate[i].m_CfgValue[DT_IOCONFIG_SWS2APSK].
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
    DtDbgOut(MAX, IOCONFIG, "Configuration FAILSAFE Value: %d SubValue: %d",
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
    DtaDeviceData*  pDvcData = pNonIpPort->m_pDvcData;
    
    DtDbgOut(MAX, IOCONFIG, "Configuration GENLOCKED Value: %d SubValue: %d",
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

        // Check if a genref port has been set
        if (!pDvcData->m_Genlock.m_IsSupported)
        {
            DtDbgOut(ERR, IOCONFIG, "Genlock is not supported");
            return DT_STATUS_CONFIG_ERROR;
        }
        if (pDvcData->m_Genlock.m_RefPortIndex == -1)
        {
            DtDbgOut(ERR, IOCONFIG, "No genref port has been set");
            return DT_STATUS_CONFIG_ERROR;
        }
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
    Int  i, RefVidStd;
    DtaNonIpPort*  pOtherNonIpPort = NULL;
    DtaDeviceData*  pDvcData = pNonIpPort->m_pDvcData;

    DtDbgOut(MAX, IOCONFIG, "Configuration GENREF Value: %d SubValue: %d",
                                  pPortUpdate->m_CfgValue[DT_IOCONFIG_GENREF].m_Value,
                                  pPortUpdate->m_CfgValue[DT_IOCONFIG_GENREF].m_SubValue);

    RefVidStd = (Int)pPortUpdate->m_CfgValue[DT_IOCONFIG_GENREF].m_ParXtra[0];
    
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

        // You cannot disable Genref if an output port has Genlocked enabled
        for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
        {
            pOtherNonIpPort = &pDvcData->m_NonIpPorts[i];
            if (pOtherNonIpPort->m_PortIndex == pNonIpPort->m_PortIndex)
                continue;   // Skip ourselves
            if (!pOtherNonIpPort->m_CapGenLocked)
                continue;   // Skip port that does not support genlock

            // Check genlocked is enabled on other ports
            if (pUpdate->m_NonIpPortUpdate[i].m_CfgValue[DT_IOCONFIG_GENLOCKED].m_Value
                                                                      == DT_IOCONFIG_TRUE)
            {
                DtDbgOut(ERR, IOCONFIG, "Port %i has genlock enabled. Genref of Port %i"
                                   " can not be disabled.", 
                                   pOtherNonIpPort->m_PortIndex, pNonIpPort->m_PortIndex);
            
                return DT_STATUS_CONFIG_ERROR;
            }
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

        // Only one port may be enabled as genlock reference
        for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
        {
            pOtherNonIpPort = &pDvcData->m_NonIpPorts[i];
            if (pOtherNonIpPort->m_PortIndex == pNonIpPort->m_PortIndex)
                continue;   // Skip ourselves
            if (!pOtherNonIpPort->m_CapGenRef)
                continue;   // Skip port that does not support genref

            // Check genref is disabled on other port
            if (pUpdate->m_NonIpPortUpdate[i].m_CfgValue[DT_IOCONFIG_GENREF].m_Value
                                                                      == DT_IOCONFIG_TRUE)
            {
                DtDbgOut(ERR, IOCONFIG, "Port %i already enabled genref. Port %i can not"
                    "be enabled.", pOtherNonIpPort->m_PortIndex, pNonIpPort->m_PortIndex);
            
                return DT_STATUS_CONFIG_ERROR;
            }
        }

        // Check port supports specified reference video standard
        if (!DtaNonIpIsVidStdSupported(pNonIpPort, RefVidStd))
        {
            DtDbgOut(ERR, IOCONFIG, "Video standard %i not supported for port %i",
                                                      RefVidStd, pNonIpPort->m_PortIndex);
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
    Int  i, IoStdValue, IoStdSubValue;
    DtaNonIpPort*  pOtherIpPort = NULL;
    DtaDeviceData*  pDvcData = pNonIpPort->m_pDvcData;
    
    DtDbgOut(MAX, IOCONFIG, "Configuration FRACMODE Value: %d SubValue: %d",
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
            pOtherIpPort = &pDvcData->m_NonIpPorts[i];
            if (pOtherIpPort->m_PortIndex == pNonIpPort->m_PortIndex)
                continue;   // Skip ourselves
            if (!pOtherIpPort->m_CapFracMode)
                continue;   // Skip port that does not support fractional mode

            // Check io-config value for other port matches
            if (pUpdate->m_NonIpPortUpdate[i].m_CfgValue[DT_IOCONFIG_FRACMODE].m_Value
                                 != pPortUpdate->m_CfgValue[DT_IOCONFIG_FRACMODE].m_Value)
                return DT_STATUS_CONFIG_ERROR;
        }
        break;

    default:
        return DT_STATUS_CONFIG_ERROR;
    }

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
                                    pDvcData->m_DevInfo.m_Serial, pNonIpPort->m_PortIndex,
                                                      GroupName, "ConfigValue", StrValue);
    if (!DT_SUCCESS(Result))
        return Result;

    // Write config subvalue
    Result = IoConfigNameGet(CfgValue.m_SubValue, StrValue, sizeof(StrValue));
    if (!DT_SUCCESS(Result))
        return Result;

    Result = DtNonVolatileSettingsStringWrite(&pDvcData->m_Driver, 
                                    pDvcData->m_DevInfo.m_Serial, pNonIpPort->m_PortIndex,
                                                   GroupName, "ConfigSubValue", StrValue);
    if (!DT_SUCCESS(Result))
        return Result;
   
    // Write ParXtra
    for (ParXtraIdx=0; ParXtraIdx<DT_MAX_PARXTRA_COUNT; ParXtraIdx++)
    {
        Result = DtNonVolatileSettingsValueWrite(&pDvcData->m_Driver,
                                    pDvcData->m_DevInfo.m_Serial, pNonIpPort->m_PortIndex,
                                             GroupName, (Char*)IoParXtraNames[ParXtraIdx], 
                                                          CfgValue.m_ParXtra[ParXtraIdx]);
        if (!DT_SUCCESS(Result))
            return Result;
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigIsFractionalIoStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static Bool  DtaIoConfigIsFractionalIoStd(Int  Value,  Int  SubValue)
{
    switch (Value)
    {
    case DT_IOCONFIG_3GSDI:
        switch (SubValue)
        {
        case DT_IOCONFIG_1080P50:
        case DT_IOCONFIG_1080P60:       
            return FALSE;
        case DT_IOCONFIG_1080P59_94:    
            return TRUE;
        default:                        
            DT_ASSERT(1==0);
            return FALSE;
        }
        break;
           
    case DT_IOCONFIG_HDSDI:
        switch (SubValue)
        {
        case DT_IOCONFIG_1080I50:
        case DT_IOCONFIG_1080I60:
        case DT_IOCONFIG_1080P24:
        case DT_IOCONFIG_1080P25:
        case DT_IOCONFIG_1080P30:
        case DT_IOCONFIG_720P24:
        case DT_IOCONFIG_720P25:
        case DT_IOCONFIG_720P30:
        case DT_IOCONFIG_720P60:        
            return FALSE;
        case DT_IOCONFIG_1080I59_94:
        case DT_IOCONFIG_1080P23_98:
        case DT_IOCONFIG_1080P29_97:
        case DT_IOCONFIG_720P23_98:
        case DT_IOCONFIG_720P29_97:
        case DT_IOCONFIG_720P59_94:     
            return TRUE;
        default:
            DT_ASSERT(1==0);
            return FALSE;
        }
        break;
    
    case DT_IOCONFIG_SDI:        
        switch (SubValue)
        {
        case DT_IOCONFIG_625I50:
            return FALSE;
        case DT_IOCONFIG_525I59_94:
            return TRUE;
        default:
            DT_ASSERT(1==0);
            return FALSE;
        }
        break;
    
    case DT_IOCONFIG_SPISDI:           
        switch (SubValue)
        {
        case DT_IOCONFIG_625I50:
            return FALSE;
        
        case DT_IOCONFIG_525I59_94:
            return TRUE;
        
        default:
            DT_ASSERT(1==0);
            return FALSE;
        }
        break;

    case DT_IOCONFIG_ASI:
    case DT_IOCONFIG_DEMOD:
    case DT_IOCONFIG_IFADC:
    case DT_IOCONFIG_IP:
    case DT_IOCONFIG_MOD:
    case DT_IOCONFIG_SPI:
    default:
        return FALSE;
    }
}
