//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCore_DEVICE_.c *#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
#include "DtCoreIncludes.h"            // Core driver includes

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_DEVICE implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtCore_Device_InitFirmwareStatus(DtCore*  pCore);
static DtStatus  DtCore_Device_GetFirmwareReleaseBuildDate(DtCore*, UInt Version, 
                                               DtDrvFwBuildDateTime* , Bool* pIsObsolete);
static DtStatus  DtCore_Device_CompareFirmwareBuildDate(DtCore*, DtDrvFwBuildDateTime*,
                        DtDrvFwBuildDateTime*,  Bool* pIsEqual, Bool* pEmptyFwRelBldDate);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_DEVICE - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEVICE_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when the device is found in the system by the bus
// driver. Typenumber cannot be determined yet (for uninitialized PLX devices).
// This function therefore cannot perform device type specific actions. The device 
// hardware will be initialized later and is not mapped yet, only PCI config space can be
// accessed (but this is already done in the IAL)!
//
DtStatus  DtCore_DEVICE_Init(DtCore*  pCore, DtIoStubCoreOpenFunc  pOpenStubFunc)
{
    DtDbgOut(MAX, CORE, "Start");

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pCore->m_pDevInfo != NULL);
    DT_ASSERT(pOpenStubFunc != NULL);

    // Initialize file handle info mutex
    DtFastMutexInit(&pCore->m_FileHandleInfoMutex);
    // Initialize file handle info
    pCore->m_pFileHandleInfo = NULL;

    // Open the core io-stub
    DT_ASSERT(pCore->m_pIoStub == NULL);
    pCore->m_pIoStub = pOpenStubFunc(pCore);
    if (pCore->m_pIoStub == NULL)
    {
        DtDbgOut(ERR, CORE, "Failed to open core io-stub");
        return DT_STATUS_OUT_OF_MEMORY;
    }
    
    // Create the events data
    DT_ASSERT(pCore->m_pCfEvtData == NULL);
    pCore->m_pCfEvtData = DtCore_EVENTS_CreateEvtData();
    if (pCore->m_pCfEvtData == NULL)
    {
        DtDbgOut(ERR, CORE, "Failed to create events data");
        return DT_STATUS_OUT_OF_MEMORY;
    }
    // Init block-controller-list (with an initial capacity of 8 BCs + grow with 4)
    DT_ASSERT(pCore->m_pBcList == NULL);
    pCore->m_pBcList = DtVectorBc_Create(8, 4);
    if (pCore->m_pBcList == NULL)
    {
        DtDbgOut(ERR, CORE, "[DTA-%d] failed to create BC-list", 
                                                         pCore->m_pDevInfo->m_TypeNumber);
        return DT_STATUS_OUT_OF_MEMORY;
    }
    DT_ASSERT(pCore->m_pUuidToBc == NULL);
    pCore->m_pUuidToBc = DtVectorBc_Create(8, 4);
    if (pCore->m_pUuidToBc == NULL)
    {
        DtDbgOut(ERR, CORE, "[DTA-%d] failed to create UUID-to-BC lookup table", 
                                                         pCore->m_pDevInfo->m_TypeNumber);
        return DT_STATUS_OUT_OF_MEMORY;
    }

    // Init driver-function-list (with an initial capacity of 8 DFs + grow with 1)
    DT_ASSERT(pCore->m_pDfList == NULL);
    pCore->m_pDfList = DtVectorDf_Create(8, 1);
    if (pCore->m_pDfList == NULL)
    {
        DtDbgOut(ERR, CORE, "[DTA-%d] failed to create DF-list", 
                                                         pCore->m_pDevInfo->m_TypeNumber);
        return DT_STATUS_OUT_OF_MEMORY;
    }
    DT_ASSERT(pCore->m_pUuidToDf == NULL);
    pCore->m_pUuidToDf = DtVectorDf_Create(8, 1);
    if (pCore->m_pUuidToDf == NULL)
    {
        DtDbgOut(ERR, CORE, "[DTA-%d] failed to create UUID-to-DF lookup table", 
                                                         pCore->m_pDevInfo->m_TypeNumber);
        return DT_STATUS_OUT_OF_MEMORY;
    }

    // Init port-list (with an initial capacity of 4 PTs + grow with 1)
    DT_ASSERT(pCore->m_pPtList == NULL);
    pCore->m_pPtList = DtVectorPt_Create(4, 1);
    if (pCore->m_pPtList == NULL)
    {
        DtDbgOut(ERR, CORE, "[DTA-%d] failed to create PT-list", 
                                                         pCore->m_pDevInfo->m_TypeNumber);
        return DT_STATUS_OUT_OF_MEMORY;
    }

    DtDbgOut(MAX, CORE, "Exit");

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEVICE_PowerUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called when the device must enter the active (D0) state. It should
// wake up the corresponding device and initialize it into active state. It should restore
// any saved settings. These settings can be saved during power down. This function can be
// executed in two different scenarios.
//
//
DtStatus  DtCore_DEVICE_PowerUp(DtCore*  pCore)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDriverEvent PowerEvent;

    DtDbgOut(MAX, CORE, "Start");

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    //.-.-.-.-.-.-.-.-.-.-.- Step 1: Init VVI and wait for firmware -.-.-.-.-.-.-.-.-.-.-.

    // !!! LOAD VITAL_VERSION_INFO BLOCK FIRST !!!
    // !!! WE NEED IT FOR GETTING FW-VERSION INFO AND TESTING IF THE FPGA IS READY !!!
    Status = DtCore_BC_OpenVvi((DtCore*)pCore);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, CORE, "ERROR: failed to load VVI-block");
        return DT_STATUS_FAIL;
    }

    // Wait until FPGA is ready for use
    DT_ASSERT(pCore->m_pBcVvi != NULL);
    Status = DtBcVVI_WaitUntilFpgaIsReady(pCore->m_pBcVvi);
    if (!DT_SUCCESS(Status))
    {
        if (Status == DT_STATUS_TIMEOUT)
            DtEvtLogReport(&pCore->m_Device.m_EvtObject, DTPCIE_LOG_FPGA_TIMEOUT, 
                                                                        NULL, NULL, NULL);
        else
            DtDbgOut(ERR, CORE, "ERROR: wait for FPGA to be ready failed");

        return Status;
    }

    // Hardware revision is determined later
    DT_ASSERT(pCore->m_pDevInfo != NULL);
    pCore->m_pDevInfo->m_HardwareRevision = 0;

    // Get firmware version info
    Status = DtBcVVI_InitFirmwareVersionInfo(pCore->m_pBcVvi, pCore->m_pDevInfo);
    DT_ASSERT(DT_SUCCESS(Status));

    // Init the PROPS core-module
    //Status = DtCore_EVENTS_Init(pCore);
    //if (!DT_SUCCESS(Status))
    //{
    //    DtDbgOut(ERR, CORE, "ERROR: failed to init the EVENTS core-module");
    //    return Status;
    //}
    //Status = DtCore_SETTINGS_Init(pCore);
    //if (!DT_SUCCESS(Status))
    //{
    //    DtDbgOut(ERR, CORE, "ERROR: failed to init the SETTINGS core-module");
    //    return Status;
    //}
    //Status = DtCore_IOCONFIG_Init(pCore);
    //if (!DT_SUCCESS(Status))
    //{
    //    DtDbgOut(ERR, CORE, "ERROR: failed to init the IOCONFIG core-module");
    //    return Status;
    //}
    //Status = DtCore_DEBUG_Init(pCore);
    //if (!DT_SUCCESS(Status))
    //{
    //    DtDbgOut(ERR, CORE, "ERROR: failed to init the DEBUG core-module");
    //    return Status;
    //}

    //.-.-.-.-.-.-.-.-.-.-.-.-.- Step 2: load core DFs and BCs  -.-.-.-.-.-.-.-.-.-.-.-.-.

    // Has the derived class registered it's own implementation?
    if (pCore->m_OpenChildrenFunc != NULL)  
        Status = pCore->m_OpenChildrenFunc(pCore);
    else                                    
        Status = DtCore_OpenChildren(pCore);

    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, CORE, "ERROR: failed to open core-children");
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.- Step 3: load non-core device DFs and BCs -.-.-.-.-.-.-.-.-.-.-
    // Open non-core device driver-functions
    Status = DtCore_DF_OpenAll((DtCore*)pCore,  NULL);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, CORE, "Failed to load DFs (ERROR: 0x%08X)", Status);
        return Status;
    }
    // Open non-core device block-controllers
    Status = DtCore_BC_OpenAll((DtCore*)pCore, NULL);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, CORE, "Failed to open BCs (ERROR: 0x%08X)", Status);
        return Status;
    }

    //.-.-.-.-.-.-.-.-.-.-.-.- Step 4: determine firmware status -.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtCore_Device_InitFirmwareStatus((DtCore*)pCore);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, CORE, "Failed to init firmware status (ERROR: 0x%08X)", Status);
        return Status;
    }
    // Don't continue if the firmware status is tainted
    if (pCore->m_pDevInfo->m_FirmwareStatus == DT_FWSTATUS_TAINTED)
    {
        DtDbgOut(MAX, CORE, "Exit because firmware is tainted");
        return DT_STATUS_OK;
    }

    //.-.-.-.-.-.-.-.-.-.-.- Step 5 load PTs with their DFs and BCs -.-.-.-.-.-.-.-.-.-.-.
    // Open ports including their driver-functions and block-controllers
    Status = DtCore_PT_OpenAll((DtCore*)pCore, pCore->m_NumPorts);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, CORE, "Failed to load PTs (ERROR: 0x%08X)", Status);
        return Status;
    }

    // Set power up event
    PowerEvent.m_EventType = DT_DRV_EVENT_TYPE_POWER;
    PowerEvent.m_EventValue1 = DT_DRV_EVENT_VALUE1_POWER_UP;
    PowerEvent.m_EventValue2 = 0;
    DtCore_EVENTS_Set((DtCore*)pCore, PowerEvent , FALSE);
    DtDbgOut(MAX, CORE, "Exit");

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_Device_InitFirmwareStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This functions determines the firmware status
//
DtStatus  DtCore_Device_InitFirmwareStatus(DtCore*  pCore)
{
    Int  FwVers;
    Int  FwLastVers;
    DtDrvFwBuildDateTime FwReleaseBuildDate;
    Bool  IsObsolete, IsEqualFwDate, IsEmptyReleaseBuildDate;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    // Current version
    FwVers = pCore->m_pDevInfo->m_FirmwareVersion;

    // Get latest know version
    FwLastVers = DtCore_PROPS_GetInt(pCore, "FIRMWARE_LAST_VER", -1, -1);

    // Get firmware release build date of current version and obsolete status
    DT_RETURN_ON_ERROR(DtCore_Device_GetFirmwareReleaseBuildDate(pCore, FwVers,
                                                      &FwReleaseBuildDate, &IsObsolete));
    // Compare firmware build date and firmware build date
    DT_RETURN_ON_ERROR(DtCore_Device_CompareFirmwareBuildDate(pCore, 
                                   &pCore->m_pDevInfo->m_FwBuildDate, &FwReleaseBuildDate,
                                   &IsEqualFwDate, &IsEmptyReleaseBuildDate));
    // Determine firmware status
    if  (FwLastVers < 0)
        pCore->m_pDevInfo->m_FirmwareStatus = DT_FWSTATUS_UNDEFINED;
    else if (FwVers==FwLastVers && !IsObsolete && IsEqualFwDate)
        pCore->m_pDevInfo->m_FirmwareStatus = DT_FWSTATUS_UPTODATE;
    else if (FwVers==FwLastVers && !IsObsolete && IsEmptyReleaseBuildDate)
        pCore->m_pDevInfo->m_FirmwareStatus = DT_FWSTATUS_BETA;
    else if (FwVers<FwLastVers && !IsObsolete && IsEqualFwDate)
        pCore->m_pDevInfo->m_FirmwareStatus = DT_FWSTATUS_OLD;
    else if (FwVers > FwLastVers)
        pCore->m_pDevInfo->m_FirmwareStatus = DT_FWSTATUS_NEW;
    else if (FwVers<=FwLastVers && !IsEqualFwDate)
        pCore->m_pDevInfo->m_FirmwareStatus = DT_FWSTATUS_TAINTED;
    else if (FwVers<=FwLastVers && IsObsolete)
        pCore->m_pDevInfo->m_FirmwareStatus = DT_FWSTATUS_OBSOLETE;
    else
    {
        DT_ASSERT(FALSE);
        pCore->m_pDevInfo->m_FirmwareStatus = DT_FWSTATUS_UNDEFINED;
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.- DtCore_Device_GetFirmwareReleaseBuildDate -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_Device_GetFirmwareReleaseBuildDate(
    DtCore*  pCore,
    UInt  Version,
    DtDrvFwBuildDateTime*  pFwReleaseBuildDate,
    Bool*  pIsObsolete)
{
    DtString  PropString;
    DtStatus  Status = DT_STATUS_OK;
    char  pPropName[PROPERTY_NAME_MAX_SIZE+1];
    DtPropertyFilterCriteria  Flt;
    Int i;
    const char* pFwRelDateStr;
    static const char  MaxDateStr[] = "9999.19.39T29:59";
    static const char  MinDateStr[] = "0000.00.00T00:00";
    const  DtProperty*  pProperty=NULL;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pFwReleaseBuildDate!=NULL && pIsObsolete!=NULL);

    // Set default result
    pFwReleaseBuildDate->m_Year = pFwReleaseBuildDate->m_Month = 0;
    pFwReleaseBuildDate->m_Day= pFwReleaseBuildDate->m_Hour = 0;
    pFwReleaseBuildDate->m_Minute = 0;
    *pIsObsolete = FALSE;
    
    // Allocate string buffers
    Status = DtStringAlloc(&PropString, sizeof(pPropName));
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DF_COMMON, "ERROR: failed to allocate string-object");
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Clean-up macro on error -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  _CHECK_RESULT(Func)                    \
    do {                                        \
        Status = Func;                          \
        if(!DT_SUCCESS(Status))                 \
        {                                       \
            /* Free string buffers */           \
            DtStringFree(&PropString);          \
            return Status;                      \
        }                                       \
    } while(0)

    //.-.-.-.-.-.-.-.-.- Construct  firmware release date property name -.-.-.-.-.-.-.-.-.
    _CHECK_RESULT(DtStringAppendChars(&PropString, "FIRMWARE_RELEASE_BUILD_DATE_"));
    // Add the version number
    _CHECK_RESULT(DtStringUInt64ToDtStringAppend(&PropString, 10, Version));

    // Convert to a 'normal' character string
    pPropName[0] = '\0';
    _CHECK_RESULT(DtStringToCharString(&PropString, pPropName, sizeof(pPropName)));

    // Get the property
    DT_PROPERTY_FILTER_CRITERIA_INIT(Flt, -1);
    Status = DtCore_PROPS_Find(pCore, pPropName, Flt, &pProperty);
    if (Status!=DT_STATUS_OK || pProperty->m_pStr==NULL)
    {
        // Free string buffer
        DtStringFree(&PropString);

        if (Status == DT_STATUS_NOT_FOUND)
            Status = DT_STATUS_OK;
        return Status;
    }

    // Get the firmware release date string
    DT_ASSERT(pProperty->m_Type == PROPERTY_VALUE_TYPE_STRING);
    pFwRelDateStr = pProperty->m_pStr;

    // Check format
    for (i=0; i<sizeof(MaxDateStr); i++)
    {
        if (pFwRelDateStr[i]<MinDateStr[i] || pFwRelDateStr[i]>MaxDateStr[i])
        {
            // Invalid format
            // Free string buffer
            DtStringFree(&PropString);
            return DT_STATUS_FAIL;
        }
    }
    // Convert string to date and time
    pFwReleaseBuildDate->m_Year = (pFwRelDateStr[0]-'0')*1000 + (pFwRelDateStr[1]-'0')*100
                                     + (pFwRelDateStr[2]-'0')*10 + (pFwRelDateStr[3]-'0');
    pFwReleaseBuildDate->m_Month  = (pFwRelDateStr[5]-'0')*10  + (pFwRelDateStr[6]-'0');
    pFwReleaseBuildDate->m_Day    = (pFwRelDateStr[8]-'0')*10  + (pFwRelDateStr[9]-'0');
    pFwReleaseBuildDate->m_Hour   = (pFwRelDateStr[11]-'0')*10 + (pFwRelDateStr[12]-'0');
    pFwReleaseBuildDate->m_Minute = (pFwRelDateStr[14]-'0')*10 + (pFwRelDateStr[15]-'0');

    // Now get the obsolete flag
    _CHECK_RESULT(DtStringClear(&PropString));
    _CHECK_RESULT(DtStringAppendChars(&PropString, "FIRMWARE_RELEASE_OBSOLETE_"));
    // Add the version number
    _CHECK_RESULT(DtStringUInt64ToDtStringAppend(&PropString, 10, Version));
    // Convert to a 'normal' character string
    pPropName[0] = '\0';
    _CHECK_RESULT(DtStringToCharString(&PropString, pPropName, sizeof(pPropName)));

    *pIsObsolete = DtCore_PROPS_GetBool(pCore, pPropName, -1, FALSE);

    // Free string buffer
    DtStringFree(&PropString);

    #undef _CHECK_RESULT

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtCore_Device_CompareFirmwareBuildDate -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCore_Device_CompareFirmwareBuildDate(
    DtCore*  pCore,
    DtDrvFwBuildDateTime*  pFwBuildDate,
    DtDrvFwBuildDateTime*  pFwRelBuildDate,
    Bool*  pIsEqual,
    Bool*  pEmptyFwReleaseBuildData)
{
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pFwBuildDate!=NULL && pFwRelBuildDate!=NULL && pIsEqual!=NULL);

    // Set defaults
    *pIsEqual = FALSE;
    *pEmptyFwReleaseBuildData = FALSE;

    // Check date equality
    if (   pFwRelBuildDate->m_Year == pFwBuildDate->m_Year
        && pFwRelBuildDate->m_Month == pFwBuildDate->m_Month
        && pFwRelBuildDate->m_Day == pFwBuildDate->m_Day
        && pFwRelBuildDate->m_Hour == pFwBuildDate->m_Hour
        && pFwRelBuildDate->m_Minute == pFwBuildDate->m_Minute)
    {
        *pIsEqual = TRUE;
    }
    // Check for empty release build date
    if (   pFwRelBuildDate->m_Year == 0
        && pFwRelBuildDate->m_Month == 0
        && pFwRelBuildDate->m_Day == 0
        && pFwRelBuildDate->m_Hour == 0
        && pFwRelBuildDate->m_Minute == 0)
    {
        *pEmptyFwReleaseBuildData = TRUE;
    }

    return DT_STATUS_OK;
}
//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEVICE_InterruptEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called when the device interrupts must be enabled.
// This is done between PowerUp and PowerUpPost
//
DtStatus  DtCore_DEVICE_InterruptEnable(DtCore*  pCore)
{
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    // Base implementation has nothing TODO
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEVICE_PowerUpPost -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called when the device must enter the active (D0) state after PowerUp
// is called and the interrupts are enabled. It should only perform actions to put the
// device in power up mode that need the interrupts to be enabled.
//
DtStatus  DtCore_DEVICE_PowerUpPost(DtCore* pCore)
{
    Int i=0, NumPt=0, NumDf=0, NumBc=0;
    DtStatus  Status = DT_STATUS_OK;

    DtDbgOut(MAX, CORE, "Start");

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Enable PTs, DFs and BCs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // Enable all device DFs
    NumDf = DtVectorDf_Size(pCore->m_pDfList);
    for (i=0; i<NumDf && DT_SUCCESS(Status); i++)
    {
        DtDf*  pDf = DtVectorDf_At(pCore->m_pDfList, i);
        if (pDf==NULL || pDf->m_EnableFunc==NULL)
            continue;
        Status = pDf->m_EnableFunc(pDf, TRUE);
        DT_ASSERT(DT_SUCCESS(Status));
    }

    // Enable all device BCs
    NumBc = DtVectorBc_Size(pCore->m_pBcList);
    for (i=0; i<NumBc && DT_SUCCESS(Status); i++)
    {
        DtBc*  pBc = DtVectorBc_At(pCore->m_pBcList, i);
        if (pBc==NULL || pBc->m_EnableFunc==NULL)
            continue;
        Status = pBc->m_EnableFunc(pBc, TRUE);
        DT_ASSERT(DT_SUCCESS(Status));
    }
    
    // First enable all PTs
    NumPt = DtVectorPt_Size(pCore->m_pPtList);
    for (i=0; i<NumPt && DT_SUCCESS(Status); i++)
    {
        DtPt*  pPt = DtVectorPt_At(pCore->m_pPtList, i);
        if (pPt==NULL || pPt->m_EnableFunc==NULL)
            continue;
        Status = pPt->m_EnableFunc(pPt, TRUE);
        DT_ASSERT(DT_SUCCESS(Status));
    }

    if (!DT_SUCCESS(Status))
    {
        // Enabling failed. Disable all object again.
        // Save the fail status
        DtStatus FailStatus = Status;
        // -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Disable all PTs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
        // NOTE: disable them in reverse order from enabling
        NumPt = DtVectorPt_Size(pCore->m_pPtList);
        for (i=(NumPt-1); i>=0; i--)
        {
            DtPt*  pPt = DtVectorPt_At(pCore->m_pPtList, i);
            if (pPt==NULL || pPt->m_EnableFunc==NULL)
                continue;
            Status = pPt->m_EnableFunc(pPt, FALSE);
        }

        // -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Disable all DFs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
        // NOTE: disable them in reverse order from enabling
        NumDf = DtVectorDf_Size(pCore->m_pDfList);
        for (i=(NumDf-1); i>=0; i--)
        {
            DtDf*  pDf = DtVectorDf_At(pCore->m_pDfList, i);
            if (pDf==NULL || pDf->m_EnableFunc==NULL)
                continue;
            Status = pDf->m_EnableFunc(pDf, FALSE);
        }

        // -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Disable all BCs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
        // NOTE: disable them in reverse order from enabling
        NumBc = DtVectorBc_Size(pCore->m_pBcList);
        for (i=(NumBc-1); i>=0; i--)
        {
            DtBc*  pBc = DtVectorBc_At(pCore->m_pBcList, i);
            if (pBc==NULL || pBc->m_EnableFunc==NULL)
                continue;
            Status = pBc->m_EnableFunc(pBc, FALSE);
        }
        return  FailStatus;
    }

    // Finally restore the IO-configuratoin
    Status = DtCore_IOCONFIG_Restore(pCore);
    if (!DT_SUCCESS(Status))
    { 
        DtDbgOut(ERR, CORE, "ERROR: Could not restore the IO-configuration");
        // Restore can only fail if firmware status is tainted
        DT_ASSERT(pCore->m_pDevInfo->m_FirmwareStatus == DT_FWSTATUS_TAINTED);
    }

    DtDbgOut(MAX, CORE, "Exit");

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEVICE_PowerDownPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called when the device must leave the active (D0) state before the
// interrupts are disabled. It should only perform actions to put the device in power
// down mode that need the interrupts to be enabled.
//
DtStatus  DtCore_DEVICE_PowerDownPre(DtCore* pCore)
{
    Int i=0, NumPt=0, NumDf=0, NumBc=0;
    DtStatus  Status = DT_STATUS_OK;

    DtDbgOut(MAX, CORE, "Start");

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);


    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Disable all PTs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // NOTE: disable them in reverse order from enabling

    NumPt = DtVectorPt_Size(pCore->m_pPtList);
    for (i=(NumPt-1); i>=0; i--)
    {
        DtPt*  pPt = DtVectorPt_At(pCore->m_pPtList, i);
        if (pPt==NULL || pPt->m_EnableFunc==NULL)
            continue;
        Status = pPt->m_EnableFunc(pPt, FALSE);
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Disable all DFs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // NOTE: disable them in reverse order from enabling

    NumDf = DtVectorDf_Size(pCore->m_pDfList);
    for (i=(NumDf-1); i>=0; i--)
    {
        DtDf*  pDf = DtVectorDf_At(pCore->m_pDfList, i);
        if (pDf==NULL || pDf->m_EnableFunc==NULL)
            continue;
        Status = pDf->m_EnableFunc(pDf, FALSE);
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Disable all BCs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // NOTE: disable them in reverse order from enabling

    NumBc = DtVectorBc_Size(pCore->m_pBcList);
    for (i=(NumBc-1); i>=0; i--)
    {
        DtBc*  pBc = DtVectorBc_At(pCore->m_pBcList, i);
        if (pBc==NULL || pBc->m_EnableFunc==NULL)
            continue;
        Status = pBc->m_EnableFunc(pBc, FALSE);
    }

    DtDbgOut(MAX, CORE, "Exit");

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEVICE_InterruptDisable -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called when the device interrupts must be disabled. This is done
// Between DevicePowerDownPre and DevicePowerDown.
//
DtStatus  DtCore_DEVICE_InterruptDisable(DtCore* pCore)
{
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    // Base implementation has nothing TODO
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEVICE_PowerDown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called when the device must leave the active (D0) state. This
// function is called after PowerDownPre is called and the interrupts are disabled. It
// should stop receive / transmit threads, store the device state and put the device in
// low power mode.
//
DtStatus  DtCore_DEVICE_PowerDown(DtCore* pCore)
{
    Int  i=0;
    DtFileHandleInfo*  pFileHandleInfo;
    DtStatus  Status = DT_STATUS_OK;
    DtDriverEvent PowerEvent;
    DtDbgOut(MAX, CORE, "Start");

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    // Set power down event
    PowerEvent.m_EventType = DT_DRV_EVENT_TYPE_POWER;
    PowerEvent.m_EventValue1 = DT_DRV_EVENT_VALUE1_POWER_DOWN;
    PowerEvent.m_EventValue2 = 0;
    DtCore_EVENTS_Set((DtCore*)pCore, PowerEvent , FALSE);

    // Set power down pending to prevent new IOCTL's
    DtFastMutexAcquire(&pCore->m_FileHandleInfoMutex);

    pFileHandleInfo = pCore->m_pFileHandleInfo;
    while (pFileHandleInfo != NULL)
    {
        pFileHandleInfo->m_PowerDownPending = TRUE;
        pFileHandleInfo = pFileHandleInfo->m_pNext;
    }

    DtFastMutexRelease(&pCore->m_FileHandleInfoMutex);

    //-.-.-.-.-.-.-.-.-.-.- Step 1: close PTs with their DFs and BCs -.-.-.-.-.-.-.-.-.-.-

    // Close port first
    if (pCore->m_pPtList != NULL)
    {
        // In reverse order to prevent clashes with vector erase in DtCore_PT_Close
        Int  NumPt = DtVectorPt_Size(pCore->m_pPtList);
        for (i=(NumPt-1); i>=0; i--)
        {
            DtPt*  pPt = DtVectorPt_At(pCore->m_pPtList, i);
            if (pPt==NULL)
                continue;
            Status = DtCore_PT_Close(pPt);
            DT_ASSERT(DT_SUCCESS(Status));
        }
    }

    //.-.-.-.-.-.-.-.- Step 2: close 'normal' (i.e. non-core) DFs and BCs -.-.-.-.-.-.-.-.

    // Close driver function first
    if (pCore->m_pDfList != NULL)
    {
        // In reverse order to prevent clashes with vector erase in DtCore_DF_Close
        Int  NumDf = DtVectorDf_Size(pCore->m_pDfList);
        for (i=(NumDf-1); i>=0; i--)
        {
            DtDf*  pDf = DtVectorDf_At(pCore->m_pDfList, i);
            if (pDf==NULL || (pDf->m_Id.m_Uuid&DT_UUID_CORE_FLAG)!=0)
                continue;
            Status = DtCore_DF_Close(pDf);
            DT_ASSERT(DT_SUCCESS(Status));
        }
    }

    // Close any remaining block controllers
    if (pCore->m_pBcList != NULL)
    {
        // In reverse order to prevent clashes with vector erase in DtCore_BC_Close
        Int  NumBc = DtVectorBc_Size(pCore->m_pBcList);
        for (i=(NumBc-1); i>=0; i--)
        {
            DtBc*  pBc = DtVectorBc_At(pCore->m_pBcList, i);
            if (pBc==NULL || (pBc->m_Id.m_Uuid&DT_UUID_CORE_FLAG)!=0)
                continue;
            Status = DtCore_BC_Close(pBc);
            DT_ASSERT(DT_SUCCESS(Status));
        }
    }

    //.-.-.-.-.-.-.-.-.-.-.-.- Step 3: close the core DFs and BCs -.-.-.-.-.-.-.-.-.-.-.-.
    if (pCore->m_CloseChildrenFunc != NULL)
        pCore->m_CloseChildrenFunc(pCore);
    else
        DtCore_CloseChildren(pCore);

    DtDbgOut(MAX, CORE, "Exit");
    
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEVICE_ExitPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is executed before the device is removed from the system. All objects
// must not be used anymore and set in the idle/done state. All active transactions must
// be stopped.
//
DtStatus  DtCore_DEVICE_ExitPre(DtCore* pCore)
{
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    // Base implementation has nothing TODO
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEVICE_Exit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when the device is being disconnected. This
// function should undo everything done by DeviceInitPost and DeviceInit.
//
void  DtCore_DEVICE_Exit(DtCore*  pCore)
{
    DtDbgOut(MAX, CORE, "Start");

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

     
    // Now clean-up the DF and BC lists
    if (pCore->m_pBcList != NULL)
    {
        DtVectorBc_Cleanup(pCore->m_pBcList);
        pCore->m_pBcList = NULL;
    }
    if (pCore->m_pUuidToBc != NULL)
    {
        DtVectorBc_Cleanup(pCore->m_pUuidToBc);
        pCore->m_pUuidToBc = NULL;
    }

    if (pCore->m_pDfList != NULL)
    {
        DtVectorDf_Cleanup(pCore->m_pDfList);
        pCore->m_pDfList = NULL;
    }
    if (pCore->m_pUuidToDf != NULL)
    {
        DtVectorDf_Cleanup(pCore->m_pUuidToDf);
        pCore->m_pUuidToDf = NULL;
    }
    if (pCore->m_pPtList != NULL)
    {
        DtVectorDf_Cleanup(pCore->m_pPtList);
        pCore->m_pPtList = NULL;
    }

    // Clean-up event data
    if (pCore->m_pCfEvtData != NULL)
    { 
        DtCore_EVENTS_CleanupEvtData(pCore->m_pCfEvtData);
        pCore->m_pCfEvtData = NULL;
    }
    // Clean-up core io-stub
    if (pCore->m_pIoStub!=NULL)
    {
        DT_ASSERT(pCore->m_pIoStub->m_CloseFunc != NULL);
        if (pCore->m_pIoStub->m_CloseFunc != NULL)
            pCore->m_pIoStub->m_CloseFunc((DtIoStub*)pCore->m_pIoStub);
    }

    DtDbgOut(MAX, CORE, "Exit");
}


// .-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEVICE_GetFirmwareVariant -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int DtCore_DEVICE_GetFirmwareVariant(DtCore* pCore)
{
    DT_ASSERT(pCore->m_pDevInfo != NULL);
    if (pCore->m_pDevInfo != NULL)
        return pCore->m_pDevInfo->m_FirmwareVariant;
    else
        return -1;
}
// .-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEVICE_GetFirmwareVersion -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int DtCore_DEVICE_GetFirmwareVersion(DtCore* pCore)
{
    DT_ASSERT(pCore->m_pDevInfo != NULL);
    if (pCore->m_pDevInfo != NULL)
        return pCore->m_pDevInfo->m_FirmwareVersion;
    else
        return -1;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEVICE_GetNumPorts -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int DtCore_DEVICE_GetNumPorts(DtCore* pCore)
{
    return pCore->m_NumPorts;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEVICE_GetSubType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int DtCore_DEVICE_GetSubType(DtCore* pCore)
{
    DT_ASSERT(pCore->m_pDevInfo != NULL);
    if (pCore->m_pDevInfo != NULL)
        return pCore->m_pDevInfo->m_SubType;
    else
        return -1;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DEVICE_GetTypeNumber -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int DtCore_DEVICE_GetTypeNumber(DtCore* pCore)
{
    DT_ASSERT(pCore->m_pDevInfo != NULL);
    if (pCore->m_pDevInfo != NULL)
        return pCore->m_pDevInfo->m_TypeNumber;
    else
        return -1;
}
