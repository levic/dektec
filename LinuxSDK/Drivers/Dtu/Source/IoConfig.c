//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* IoConfig.c *#*#*#*#*#*#*#*#* (C) 2010-2016 DekTec
//
// Dtu driver - IO configuration - Definition of IO configuration types/functions
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
#include <DtuIncludes.h>


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// DtuIoConfigNonIpPortUpdate
typedef struct _DtuIoConfigNonIpPortUpdate {
    DtuIoConfigValue  m_CfgValue[DT_IOCONFIG_COUNT];
    Bool  m_UpdateNeeded[DT_IOCONFIG_COUNT];
} DtuIoConfigNonIpPortUpdate;

// DtuIoConfigUpdate
typedef struct _DtuIoConfigUpdate {
    DtuIoConfigNonIpPortUpdate*  m_pNonIpPortUpdate;    // Array of tot. num. NonIpPorts
} DtuIoConfigUpdate;

// DtuIoConfigStorageNames
typedef struct _DtuIoConfigStorageNames {
    Char*  m_pCategory;
    Char*  m_pValueName;
    Char*  m_pIoCfgParXtraName[DT_MAX_PARXTRA_COUNT];
} DtuIoConfigStorageNames;


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtuIoConfigUpdateApply(DtuDeviceData* pDvcData,
                               DtuIoConfigUpdate* pUpdate, Bool SaveToNonVolatileStorage);
static DtStatus  DtuIoConfigUpdateLoadCache(DtuDeviceData* pDvcData,
                                                              DtuIoConfigUpdate* pUpdate);
static DtStatus  DtuIoConfigUpdateLoadNonVolatileStorage(DtuDeviceData* pDvcData,
                                                              DtuIoConfigUpdate* pUpdate);
static DtStatus  DtuIoConfigUpdateValidate(DtuDeviceData* pDvcData,
                                                              DtuIoConfigUpdate* pUpdate);
static DtStatus  DtuIoConfigWriteToNonVolatileStorage(DtuNonIpPort* pNonIpPort, 
                                                  Int IoGroup, DtuIoConfigValue CfgValue);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Global data -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// ParXtra names
static const char*   IoParXtraNames[DT_MAX_PARXTRA_COUNT] = {
        "IoParXtra0", 
        "IoParXtra1", 
        "IoParXtra2", 
        "IoParXtra3" };

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuIoConfigInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuIoConfigInit(DtuDeviceData* pDvcData)
{
    DtStatus  Result;
    Int  NonIpIndex;
    DtuNonIpPort*  pNonIpPort;
    DtuIoConfigNonIpPortUpdate*  pPortUpdate;
    Int  IoConfig;
    DtuIoConfigUpdate  Update;

    DtDbgOut(MAX, IOCONFIG, "Entry");

    // Use a dynamic allocated structure to avoid stack errors.
    Update.m_pNonIpPortUpdate = 
                 (DtuIoConfigNonIpPortUpdate*)DtMemAllocPool(DtPoolNonPaged, 
                 sizeof(DtuIoConfigNonIpPortUpdate) * pDvcData->m_NumNonIpPorts, DTU_TAG);
    if (Update.m_pNonIpPortUpdate == NULL)
        return DT_STATUS_OUT_OF_MEMORY;

    // Load cache (initial cache holds the defaults which are not already applied)
    Result = DtuIoConfigUpdateLoadCache(pDvcData, &Update);
    if (!DT_SUCCESS(Result))
    {
        DtMemFreePool(Update.m_pNonIpPortUpdate, DTU_TAG);
        return Result;
    }

    // Registry is only read after a cold start, else the cache already holds the correct
    // IO configurations.
    if (pDvcData->m_InitialPowerup)
    {
        // Load non volatile storage settings
        Result = DtuIoConfigUpdateLoadNonVolatileStorage(pDvcData, &Update);
        if (!DT_SUCCESS(Result))
        {
            DtMemFreePool(Update.m_pNonIpPortUpdate, DTU_TAG);
            return Result;
        }

        // Validate configuration after registry manipulations are done
        Result = DtuIoConfigUpdateValidate(pDvcData, &Update);
        if (!DT_SUCCESS(Result))
        {
            DtDbgOut(ERR, IOCONFIG, "Configuration invalid. Apply initial configuration");
            // Reload cache (initial cache holds the defaults which are not already
            // applied)
            Result = DtuIoConfigUpdateLoadCache(pDvcData, &Update);
            if (!DT_SUCCESS(Result))
            {
                DtMemFreePool(Update.m_pNonIpPortUpdate, DTU_TAG);
                return Result;
            }

            // Delete the registry settings for this device.
            if (!DT_SUCCESS(DtNonVolatileSettingsDelete(&pDvcData->m_Driver, 
                                                           pDvcData->m_DevInfo.m_Serial,
                                                           pDvcData->m_NumNonIpPorts+
                                                           pDvcData->m_NumIpPorts)))
                DtDbgOut(ERR, IOCONFIG, "Error deleting registry key");
        }
    }

    // Set UpdateNeeded flag for all IO configurations during init...
    for (NonIpIndex=0; NonIpIndex<pDvcData->m_NumNonIpPorts; NonIpIndex++)
    {
        pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpIndex];
        pPortUpdate = &Update.m_pNonIpPortUpdate[pNonIpPort->m_PortIndex];
        for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
        {
            // Skip IO configs not applicable to the current port type
            if (pPortUpdate->m_CfgValue[IoConfig].m_Value == DT_IOCONFIG_NONE)
                continue;
            pPortUpdate->m_UpdateNeeded[IoConfig] = TRUE;
        }
    }


    // Apply (without saving everything to the non volatile storage)
    Result = DtuIoConfigUpdateApply(pDvcData, &Update, FALSE);
    DtMemFreePool(Update.m_pNonIpPortUpdate, DTU_TAG);
    
    DtDbgOut(MAX, IOCONFIG, "Exit");

    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuIoConfigGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
//
DtStatus  DtuIoConfigGet(
    DtuDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoConfigId* pIoCfgId, 
    DtIoConfigValue* pIoCfgValue, 
    Int  Count)

{
    DtStatus  Result = DT_STATUS_OK;
    Int  i, NonIpIndex, IoGroup;
    DtuNonIpPort*  pNonIpPort;
    DtuIoConfigValue  DtuIoCfgVal;
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
        Result = DtuGetNonIpPortIndex(pDvcData, pIoCfgId[i].m_PortIndex, &NonIpIndex);
        if (!DT_SUCCESS(Result)) 
            return Result;
        pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpIndex];

        // Get the IO configuration and copy it to the caller variables
        Result = DtuNonIpIoConfigGet(pNonIpPort, IoGroup, &DtuIoCfgVal);
        if (!DT_SUCCESS(Result)) 
            return Result;
            
        // Value to string
        Result = IoConfigNameGet(DtuIoCfgVal.m_Value, pIoCfgValue[i].m_Value, 
                                                                  IOCONFIG_NAME_MAX_SIZE);

        // Optionally convert Subvalue to string
        if (DT_SUCCESS(Result)) 
            Result = IoConfigNameGet(DtuIoCfgVal.m_SubValue, pIoCfgValue[i].m_SubValue, 
                                                                  IOCONFIG_NAME_MAX_SIZE);

        if (DT_SUCCESS(Result))
        {
            for(ParXtra=0; ParXtra<DT_MAX_PARXTRA_COUNT; ParXtra++)
                pIoCfgValue[i].m_ParXtra[ParXtra] = DtuIoCfgVal.m_ParXtra[ParXtra];
        }
    }
    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuIoConfigSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuIoConfigSet(
    DtuDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoConfigPars*  pIoConfigs,
    Int  Count)
{
    DtStatus  Result = DT_STATUS_OK;
    Int  Cfg;
    DtIoConfigPars*  pCfg;
    Int  NonIpIndex;
    DtuNonIpPort*  pNonIpPort;
    
    DtuIoConfigUpdate  Update;
    DtuIoConfigNonIpPortUpdate*  pPortUpdate;
    //DtuIoConfigValue  IpDataConfigValue;
    //DtuIpPort*  pIpPort;
    //Int  IpIndex;
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
    
    // Use a dynamic allocated structure to avoid stack errors.
     Update.m_pNonIpPortUpdate = 
                 (DtuIoConfigNonIpPortUpdate*)DtMemAllocPool(DtPoolNonPaged, 
                 sizeof(DtuIoConfigNonIpPortUpdate) * pDvcData->m_NumNonIpPorts, DTU_TAG);
    if (Update.m_pNonIpPortUpdate == NULL)
    {
        DtFastMutexRelease(&pDvcData->m_ExclAccessMutex);
        return DT_STATUS_OUT_OF_MEMORY;
    }
    
    // Non Ip configuration
    // Load cache
    if (DT_SUCCESS(Result))
        Result = DtuIoConfigUpdateLoadCache(pDvcData, &Update);

    // Merge new configs
    if (DT_SUCCESS(Result))
    {
        for (Cfg=0; Cfg<Count; Cfg++)
        {
            pCfg = &pIoConfigs[Cfg];
            Result = DtuGetNonIpPortIndex(pDvcData, pCfg->m_PortIndex, &NonIpIndex);
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

            pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpIndex];
            pPortUpdate = &Update.m_pNonIpPortUpdate[NonIpIndex];

            // Check if we should update the IO configuration
            // The DT_IOCONFIG_BW is special in that we always want to try and reapply
            // it to try and grab bandwidth if it previously failed.
            if (pPortUpdate->m_CfgValue[IoGroup].m_Value != ConfigValue
               || pPortUpdate->m_CfgValue[IoGroup].m_SubValue != ConfigSubValue
               || IoGroup == DT_IOCONFIG_BW)
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
                    Result = DtuNonIpHasAccess(pNonIpPort, pFile);
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
        Result = DtuIoConfigUpdateValidate(pDvcData, &Update);
    
    // Apply (and save updates to the non volatile storage)
    // The DtuIoConfigUpdateApply must be running at passive level for Windows,
    // so we need to release the ExclAccessMutex here.
    // We set a flag to indicate that an update is still busy
    if (DT_SUCCESS(Result))
        pDvcData->m_RegistryWriteBusy = TRUE;

    // Release exclusive access mutex
    DtFastMutexRelease(&pDvcData->m_ExclAccessMutex);

    if (DT_SUCCESS(Result))
        Result = DtuIoConfigUpdateApply(pDvcData, &Update, TRUE);

    DtMemFreePool(Update.m_pNonIpPortUpdate, DTU_TAG);

    return Result;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Private helpers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuIoConfigUpdateApply -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Post: pDvcData->m_RegWriteBusy flag must be reset
//
static DtStatus  DtuIoConfigUpdateApply(
    DtuDeviceData*  pDvcData,
    DtuIoConfigUpdate*  pUpdate,
                                // Contains the complete new IO configuration
    Bool  SaveToNonVolatileStorage)
{
    DtStatus  Result;
    Int  NonIpIndex;
    Int  IoConfig;
    DtuNonIpPort*  pNonIpPort;
    DtuIoConfigNonIpPortUpdate*  pPortUpdate;
    DtDbgOut(MAX, IOCONFIG, "Entry");

    // Apply settings port after port
    for (NonIpIndex=0; NonIpIndex<pDvcData->m_NumNonIpPorts; NonIpIndex++)
    {
        pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpIndex];
        pPortUpdate = &pUpdate->m_pNonIpPortUpdate[NonIpIndex];
        for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
        {
          DtDbgOut(MAX, IOCONFIG, "DtuIoConfigUpdateApply: IoConfig=%d m_UpdateNeeded=%d",
                                         IoConfig, pPortUpdate->m_UpdateNeeded[IoConfig]);

            if (pPortUpdate->m_UpdateNeeded[IoConfig])
            {
                // Apply IO config
                Result = DtuNonIpIoConfigSet(pNonIpPort, IoConfig,
                                                       pPortUpdate->m_CfgValue[IoConfig]);
                if (!DT_SUCCESS(Result))
                {
                    pDvcData->m_RegistryWriteBusy = FALSE;
                    return Result;
                }

                // Save IO config to non volatile storage
                if (SaveToNonVolatileStorage)
                    DtuIoConfigWriteToNonVolatileStorage(pNonIpPort, IoConfig,
                                                       pPortUpdate->m_CfgValue[IoConfig]);
            }
        }
    }
    pDvcData->m_RegistryWriteBusy = FALSE;
    DtDbgOut(MAX, IOCONFIG, "Exit");
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuIoConfigUpdateLoadCache -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtuIoConfigUpdateLoadCache(
    DtuDeviceData*  pDvcData,
    DtuIoConfigUpdate*  pUpdate)
                                // Contains the complete new IO configuration
{
    Int  NonIpIndex;
    Int  IoConfig;
    DtuNonIpPort*  pNonIpPort;
    DtuIoConfigNonIpPortUpdate*  pPortUpdate;

    // Load non IP IO configuration cache
    for (NonIpIndex=0; NonIpIndex<pDvcData->m_NumNonIpPorts; NonIpIndex++)
    {
        pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpIndex];
        pPortUpdate = &pUpdate->m_pNonIpPortUpdate[NonIpIndex];

        for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
        {
            // Get cached current values
            DtuNonIpIoConfigGet(pNonIpPort, IoConfig, &pPortUpdate->m_CfgValue[IoConfig]);
            pPortUpdate->m_UpdateNeeded[IoConfig] = FALSE;
        }
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.- DtuIoConfigUpdateLoadNonVolatileStorage -.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtuIoConfigUpdateLoadNonVolatileStorage(
    DtuDeviceData*  pDvcData,
    DtuIoConfigUpdate*  pUpdate)
                                // Contains the complete new IO configuration
{
    DtStatus  Result = DT_STATUS_OK;
    Int  NonIpIndex;
#ifdef WINBUILD
    Int  IoConfig;
#else
    Int  PortNumber;
#endif
    DtuNonIpPort*  pNonIpPort;
    DtuIoConfigNonIpPortUpdate*  pPortUpdate;

    for (NonIpIndex=0; NonIpIndex<pDvcData->m_NumNonIpPorts; NonIpIndex++)
    {
        pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpIndex];
        pPortUpdate = &pUpdate->m_pNonIpPortUpdate[NonIpIndex];
#ifdef WINBUILD
        for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
        {
            Result = DtIoConfigReadFromNonVolatileStorage(&pDvcData->m_Driver,
                                                      pDvcData->m_DevInfo.m_Serial,
                                                      pNonIpPort->m_PortIndex, IoConfig,
                                                      &pPortUpdate->m_CfgValue[IoConfig]);
            if (!DT_SUCCESS(Result) && Result!=DT_STATUS_NOT_FOUND)
                break;
        }
#else

        Result = DtuGetPortNumber(pDvcData, pNonIpPort->m_PortIndex, &PortNumber);
        if (!DT_SUCCESS(Result))
            break;
        Result = DtIoConfigReadFromIniFile("Dtu", pDvcData->m_DevInfo.m_Serial,
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuIoConfigUpdateValidate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Validate the port dependencies for the new IO configuration.
// Port capabilities are already checked in the DTUPI
// Changes to ports with exclusive access pending are already checked in DtuIoConfigSet
//
static DtStatus  DtuIoConfigUpdateValidate(
    DtuDeviceData*  pDvcData,
    DtuIoConfigUpdate*  pUpdate)
{
    DtStatus  Result = DT_STATUS_OK;
    Int  NonIpIndex;
    DtuNonIpPort*  pNonIpPort;
    DtuNonIpPort*  pNonIpBuddyPort;
    DtuIoConfigNonIpPortUpdate*  pPortUpdate;
    Int  Buddy, Isi;
    Int i;

    for (NonIpIndex=0; NonIpIndex<pDvcData->m_NumNonIpPorts; NonIpIndex++)
    {

        pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpIndex];
        pPortUpdate = &pUpdate->m_pNonIpPortUpdate[NonIpIndex];

        // Validate DT_IOCONFIG_IODIR
        DtDbgOut(MAX, IOCONFIG, "Configuration IODIR Value: %d SubValue: %d",
            pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_Value,
            pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_SubValue);

        switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_Value)
        {
        case DT_IOCONFIG_DISABLED:
            if (!pNonIpPort->m_CapDisabled)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_INPUT:
            if (!pNonIpPort->m_CapInput)
                return DT_STATUS_CONFIG_ERROR;
            
            // If another port is set in APSK mode, we are not allowed to use this port
            for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
            {                
                if(i != pNonIpPort->m_PortIndex)
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
                if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber == 236 ||
                    pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber == 238)
                {
                    // Only one of the two DTU-236/238 ports can be enabled at any time
                    DtuIoConfigNonIpPortUpdate*  pPortUpdateBuddy =
                                             &pUpdate->m_pNonIpPortUpdate[NonIpIndex ^ 1];
                    if (pPortUpdateBuddy->m_CfgValue[DT_IOCONFIG_IODIR].m_Value !=
                                                                     DT_IOCONFIG_DISABLED)
                        return DT_STATUS_CONFIG_ERROR;
                }
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
                Result = DtuGetNonIpPortIndex(pDvcData, Buddy, &Buddy);
                if (!DT_SUCCESS(Result))
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
                    Result = DtuGetNonIpPortIndex(pDvcData, Buddy, &Buddy);
                    if (DT_SUCCESS(Result))
                    {
                        if (pUpdate->m_pNonIpPortUpdate[Buddy].
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
                    Result = DtuGetNonIpPortIndex(pDvcData, Buddy, &Buddy);
                    if (DT_SUCCESS(Result))
                    {
                        if (pUpdate->m_pNonIpPortUpdate[Buddy].
                               m_CfgValue[DT_IOCONFIG_IODIR].m_Value != DT_IOCONFIG_INPUT)
                            return DT_STATUS_CONFIG_ERROR;
                    } else
                        return DT_STATUS_CONFIG_ERROR;
                    break;

                case DT_IOCONFIG_LOOPS2TS:
                    if (!pNonIpPort->m_CapLoopS2Ts)
                        return DT_STATUS_CONFIG_ERROR;
                    // Check provided buddy is a valid non IP input port
                    Buddy = (Int)pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].m_ParXtra[0];
                    Result = DtuGetNonIpPortIndex(pDvcData, Buddy, &Buddy);
                    if (DT_SUCCESS(Result))
                    {
                        if (pUpdate->m_pNonIpPortUpdate[Buddy].
                               m_CfgValue[DT_IOCONFIG_IODIR].m_Value != DT_IOCONFIG_INPUT)
                            return DT_STATUS_CONFIG_ERROR;

                        // Check ISI
                        Isi = (Int)pPortUpdate->m_CfgValue[DT_IOCONFIG_IODIR].
                                                                             m_ParXtra[1];
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
                    Result = DtuGetNonIpPortIndex(pDvcData, Buddy, &Buddy);
                    if (DT_SUCCESS(Result))
                    {
                        if (pUpdate->m_pNonIpPortUpdate[Buddy].
                               m_CfgValue[DT_IOCONFIG_IODIR].m_Value != DT_IOCONFIG_INPUT)
                            return DT_STATUS_CONFIG_ERROR;                        
                    } 
                    else
                        return DT_STATUS_CONFIG_ERROR;
                    break;

                default:
                    return DT_STATUS_CONFIG_ERROR;
            }
            break;

        default:
            return DT_STATUS_CONFIG_ERROR;
        }


        // Validate DT_IOCONFIG_IOSTD
        DtDbgOut(MAX, IOCONFIG, "Configuration IOSTD Value: %d SubValue: %d",
            pPortUpdate->m_CfgValue[DT_IOCONFIG_IOSTD].m_Value,
            pPortUpdate->m_CfgValue[DT_IOCONFIG_IOSTD].m_SubValue);

        switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_IOSTD].m_Value)
        {
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

        // Ignore DT_IOCONFIG_BW
        DtDbgOut(MAX, IOCONFIG, "Configuration BW Value: %d ParXtra0: %lld",
                                    pPortUpdate->m_CfgValue[DT_IOCONFIG_BW].m_Value,
                                    pPortUpdate->m_CfgValue[DT_IOCONFIG_BW].m_ParXtra[0]);

        // Validate DT_IOCONFIG_PWRMODE
        DtDbgOut(MAX, IOCONFIG, "Configuration PWRMODE Value: %d SubValue: %d",
            pPortUpdate->m_CfgValue[DT_IOCONFIG_PWRMODE].m_Value,
            pPortUpdate->m_CfgValue[DT_IOCONFIG_PWRMODE].m_SubValue);

        switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_PWRMODE].m_Value)
        {
        case DT_IOCONFIG_NONE:
            // Not applicable should only be set when we do not support PWRMODE
            DT_ASSERT(!pNonIpPort->m_CapLowPwr && !pNonIpPort->m_CapModHq);
            break;
        case DT_IOCONFIG_LOWPWR:
            if (!pNonIpPort->m_CapLowPwr)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_MODHQ:
            if (!pNonIpPort->m_CapModHq)
                return DT_STATUS_CONFIG_ERROR;
            break;
        default:
            return DT_STATUS_CONFIG_ERROR;
        }

        // Validate DT_IOCONFIG_RFCLKSEL
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
      
        // Validate DT_IOCONFIG_SPICLKSEL
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
      
        // Validate DT_IOCONFIG_SPIMODE
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


        // Validate DT_IOCONFIG_SPISTD
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

        // Validate DT_IOCONFIG_TSRATESEL
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

        // Validate DT_IOCONFIG_FAILSAFE
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

        // Validate DT_IOCONFIG_GENLOCKED
        DtDbgOut(MAX, IOCONFIG, "Configuration GENLOCKED Value: %d SubValue: %d",
            pPortUpdate->m_CfgValue[DT_IOCONFIG_GENLOCKED].m_Value,
            pPortUpdate->m_CfgValue[DT_IOCONFIG_GENLOCKED].m_SubValue);

        switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_GENLOCKED].m_Value)
        {
        // Genlock not supported yet
        case DT_IOCONFIG_NONE:
            // Not applicable should only be set when we do not support GenLock
            DT_ASSERT(!pNonIpPort->m_CapGenLocked);
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
 
        // Validate DT_IOCONFIG_GENREF
        DtDbgOut(MAX, IOCONFIG, "Configuration GENREF Value: %d SubValue: %d",
            pPortUpdate->m_CfgValue[DT_IOCONFIG_GENREF].m_Value,
            pPortUpdate->m_CfgValue[DT_IOCONFIG_GENREF].m_SubValue);
        switch (pPortUpdate->m_CfgValue[DT_IOCONFIG_GENREF].m_Value)
        {
        case DT_IOCONFIG_NONE:
            // Not applicable should only be set when we do not support GENREF
            DT_ASSERT(!pNonIpPort->m_CapGenRef);
            break;
        case DT_IOCONFIG_FALSE:
            if (!pNonIpPort->m_CapGenRef)
                return DT_STATUS_CONFIG_ERROR;
            break;
        case DT_IOCONFIG_TRUE:
            if (!pNonIpPort->m_CapGenRef)
                return DT_STATUS_CONFIG_ERROR;
            break;
        default:
            return DT_STATUS_CONFIG_ERROR;
        }

        // Validate DT_IOCONFIG_S2APSK
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

    }

    return Result;
}



//-.-.-.-.-.-.-.-.-.-.-.-.- DtuIoConfigWriteToNonVolatileStorage -.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtuIoConfigWriteToNonVolatileStorage(
    DtuNonIpPort*  pNonIpPort,
    Int  IoGroup,
    DtuIoConfigValue  CfgValue)
{
    DtStatus  Result;
    Int  ParXtraIdx;
    DtuDeviceData*  pDvcData = pNonIpPort->m_pDvcData;
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

