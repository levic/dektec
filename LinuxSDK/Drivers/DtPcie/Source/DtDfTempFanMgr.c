//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfTempFanMgr.c *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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
#include "DtDfTempFanMgr.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

#define  FANC_ROLE_NONE   NULL

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfTempFanMgr implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiTx function
#define DF_TEMPFANMGR_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfTempFanMgr))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_TEMPFANMGR_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(TEMPFANMGR, pDf)

// Helper macro to cast a DtDf* to a DtDfTempFanMgr*
#define DF_TEMPFANMGR      ((DtDfTempFanMgr*)pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfTempFanMgr_Init(DtDf*);
static DtStatus  DtDfTempFanMgr_CleanUp(DtDfTempFanMgr*);
static DtStatus  DtDfTempFanMgr_LoadParameters(DtDf*);
static DtStatus  DtDfTempFanMgr_OpenChildren(DtDfTempFanMgr*);
static DtStatus  DtDfTempFanMgr_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfTempFanMgr_OnEnablePreChildren(DtDf*, Bool  Enable);
static void  DtDfTempFanMgr_PeriodicIntervalHandler(DtObject*, DtTodTime  Time);

static void DtDfTempFanMgr_UpdateFanSpeed(DtDfTempFanMgr* pDf);


//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfTempFanMgr - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfTempFanMgr_Close(DtDf*  pDf)
{
    DF_TEMPFANMGR_DEFAULT_PRECONDITIONS(pDf);

    // Clean-up
    DtDfTempFanMgr_CleanUp(DF_TEMPFANMGR);

    // Unregister periodic interrupt handler
    DtCore_TOD_PeriodicItvUnregister(pDf->m_pCore, (DtObject*)pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfTempFanMgr*  DtDfTempFanMgr_Open(DtCore*  pCore, DtPt*  pPt,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    // Init open parameters
    DT_DF_TEMPFANMGR_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfTempFanMgr, Id, DT_FUNC_TYPE_TEMPFANMGR, pPt,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfTempFanMgr_Close;
    OpenParams.m_InitFunc = DtDfTempFanMgr_Init;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfTempFanMgr_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfTempFanMgr_OnEnablePreChildren;
    OpenParams.m_LoadParsFunc = DtDfTempFanMgr_LoadParameters;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfTempFanMgr*)DtDf_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_GetFanStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfTempFanMgr_GetFanStatus(DtDfTempFanMgr* pDf, Int  FanIdx, Int* pFanSpeedPct,
                              Int*  pFanSpeedRpm, Bool*  pIsWdTimeout, Bool*  pIsFanStuck)
{
    Int FanSpeed;
    // Sanity check
    DF_TEMPFANMGR_DEFAULT_PRECONDITIONS(pDf);

    DF_MUST_BE_ENABLED_IMPL(TEMPFANMGR, pDf);

    // Check parameters
    if (FanIdx<0 || FanIdx>=pDf->m_NumFans)
        return DT_STATUS_INVALID_PARAMETER;
    if (pFanSpeedPct==NULL || pFanSpeedRpm==NULL || pIsWdTimeout==NULL
                                                                     || pIsFanStuck==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    DT_ASSERT(pDf->m_pBcFanCtrl != NULL);

    // Get fanspeed and convert to percentage
    DT_RETURN_ON_ERROR(DtBcFANC_GetFanSpeed(pDf->m_pBcFanCtrl, &FanSpeed));
    *pFanSpeedPct = (FanSpeed*100) / DT_BC_FANC_MAX_SPEED;

    // Get fan status
    return DtBcFANC_GetStatus(pDf->m_pBcFanCtrl, pFanSpeedRpm, pIsWdTimeout, pIsFanStuck);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_GetNumFans -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfTempFanMgr_GetNumFans(DtDfTempFanMgr* pDf, Int* pNumFans)
{
    // Sanity check
    DF_TEMPFANMGR_DEFAULT_PRECONDITIONS(pDf);

    DF_MUST_BE_ENABLED_IMPL(TEMPFANMGR, pDf);

    // Check parameter
    if (pNumFans == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return number of fans
    *pNumFans = pDf->m_NumFans;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_GetNumTempSensors -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfTempFanMgr_GetNumTempSensors(DtDfTempFanMgr* pDf, Int* pNumTempSensors)
{
    // Sanity check
    DF_TEMPFANMGR_DEFAULT_PRECONDITIONS(pDf);

    DF_MUST_BE_ENABLED_IMPL(TEMPFANMGR, pDf);

    // Check parameter
    if (pNumTempSensors == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return number of temperature sensors
    *pNumTempSensors = pDf->m_NumTempSensors;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_GetTemperature -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfTempFanMgr_GetTemperature(DtDfTempFanMgr* pDf, Int TempIdx, 
                                                                        Int* pTemperature)
{
    DtDfSensTemp* pTempSensor =  NULL;
    // Sanity check
    DF_TEMPFANMGR_DEFAULT_PRECONDITIONS(pDf);

    DF_MUST_BE_ENABLED_IMPL(TEMPFANMGR, pDf);

    // Check parameter
    if (TempIdx<0 || TempIdx>=pDf->m_NumTempSensors)
        return DT_STATUS_INVALID_PARAMETER;
    if (pTemperature == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Get temperature sensor
    pTempSensor = (DtDfSensTemp*)DtVectorDf_At(pDf->m_pDfTempSensors, TempIdx);
    DT_ASSERT(pTempSensor != NULL);

    // Get temperature
    return DtDfSensTemp_GetTemperature(pTempSensor, pTemperature);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_GetTempProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfTempFanMgr_GetTempProperties(DtDfTempFanMgr* pDf, Int TempIdx, 
                                                       DtDfSensTempProperties* pTempProps)
{
    DtDfSensTempProperties* pTempSensorProps =  NULL;
    // Sanity check
    DF_TEMPFANMGR_DEFAULT_PRECONDITIONS(pDf);

    DF_MUST_BE_ENABLED_IMPL(TEMPFANMGR, pDf);

    // Check parameter
    if (TempIdx<0 || TempIdx>=pDf->m_NumTempSensors)
        return DT_STATUS_INVALID_PARAMETER;
    if (pTempProps == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Get temperature sensor
    pTempSensorProps = DtVector_At(pDf->m_pTempSensorProps, TempIdx);
    DT_ASSERT(pTempSensorProps != NULL);

    // Copy properties
    *pTempProps = *pTempSensorProps;
    return DT_STATUS_OK;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfTempFanMgr - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfTempFanMgr_Init(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfTempFanMgr* pDf = (DtDfTempFanMgr*)pDfBase;
    DtOnPeriodicIntervalRegData  RegData;

    // Sanity checks
    DF_TEMPFANMGR_DEFAULT_PRECONDITIONS(pDf);

    // Set defaults
    pDf->m_NumFans = 0;
    pDf->m_NumTempSensors = 0;

    // Init Fan control state and Spinlock
    pDf->m_FanCtrlState = TEMPFANMGR_FANCTRL_DISABLED;
    DtSpinLockInit(&pDf->m_FanCtrlStateSpinLock);

    // Delay and MinFanSpeed should have a valid value
    DT_ASSERT(pDf->m_Delay > 0);
    DT_ASSERT(pDf->m_MinFanSpeed >= DT_BC_FANC_MIN_SPEED);
    DT_ASSERT(pDf->m_MinFanSpeed <= DT_BC_FANC_MAX_SPEED);
    // Set default fanspeed
    pDf->m_FanSpeed_x_Delay = DT_BC_FANC_DEFAULT_SPEED * pDf->m_Delay;

    // Create list for the temperature sensors
    DT_ASSERT(pDf->m_pDfTempSensors == NULL);
    pDf->m_pDfTempSensors = DtVectorDf_Create(1, 1);
    DT_ASSERT(pDf->m_pDfTempSensors != NULL);

    // Create list for the temperature sensor properties
    DT_ASSERT(pDf->m_pTempSensorProps == NULL);
    pDf->m_pTempSensorProps = DtVector_Create(1, sizeof(DtDfSensTempProperties), 1);
    DT_ASSERT(pDf->m_pTempSensorProps != NULL);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfTempFanMgr_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDfTempFanMgr_CleanUp(pDf);
        DtDbgOutDf(ERR, TEMPFANMGR, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    // Register periodic interval handler
    RegData.m_OnPeriodicFunc = DtDfTempFanMgr_PeriodicIntervalHandler;
    RegData.m_pObject = (DtObject*)pDf;
    Status = DtCore_TOD_PeriodicItvRegister(pDf->m_pCore, &RegData);
    if (!DT_SUCCESS(Status))
    {
        DtDfTempFanMgr_CleanUp(pDf);
        DtDbgOutDf(ERR, TEMPFANMGR, pDf, "ERROR: failed to register period itv handler");
        return Status;
    }

    // Get periodic interval
    Status = DtCore_TOD_GetPeriodicItv(pDf->m_pCore, &pDf->m_PeriodicInterval);
    if (!DT_SUCCESS(Status))
    {
        DtDfTempFanMgr_CleanUp(pDf);
        DtDbgOutDf(ERR, TEMPFANMGR, pDf, "ERROR: DtCore_TOD_GetPeriodicItv failed");
        return Status;
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_InitTempSensorProperties -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfTempFanMgr_InitTempSensorProperties(DtDfTempFanMgr* pDf)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfSensTemp* pTempSensor =  NULL;
    DtDfSensTempProperties* pTempProps = NULL;
    Int i;

    // Childeren should be opened
    DT_ASSERT(pDf->m_pTempSensorProps != NULL);

    // Resize the look-up table
    Status = DtVector_Resize(pDf->m_pTempSensorProps, pDf->m_NumTempSensors); 
    DT_ASSERT(DT_SUCCESS(Status));

    // Get the properties of the temperatur sensors
    for (i=0; i<pDf->m_NumTempSensors; i++)
    {
        // Get temperature sensor
        pTempSensor = (DtDfSensTemp*)DtVectorDf_At(pDf->m_pDfTempSensors, i);
        DT_ASSERT(pTempSensor != NULL);
        
        // Get temperature properties store
        pTempProps = (DtDfSensTempProperties*)DtVector_At(pDf->m_pTempSensorProps, i);
        DT_ASSERT(pTempSensor != NULL);

        // Get properties
        DT_RETURN_ON_ERROR(DtDfSensTemp_GetProperties(pTempSensor, pTempProps));
    }
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfTempFanMgr_LoadParameters(DtDf*  pDfBase)
{
    DtDfTempFanMgr* pDf = (DtDfTempFanMgr*)pDfBase;

    // List of fan manager  function parameters
    DtDfParameters  DFTEMPFANMGR_PARS[] =
    {
        // Name,  Value Type,  Value*
        { "CONTROL_DELAY", PROPERTY_VALUE_TYPE_INT, &(pDf->m_Delay) },
        { "MIN_FAN_SPEED", PROPERTY_VALUE_TYPE_INT, &(pDf->m_MinFanSpeed) },
    };

    // Sanity checks
    DF_TEMPFANMGR_DEFAULT_PRECONDITIONS(pDf);


    // Invalidate parameters
    pDf->m_Delay = -1;
    pDf->m_MinFanSpeed = -1;
    // Load parameters from property store
    DT_RETURN_ON_ERROR(DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFTEMPFANMGR_PARS), 
                                                                      DFTEMPFANMGR_PARS));
    // Check paramaters have been loaded succesfully
    DT_ASSERT(pDf->m_Delay > 0);
    DT_ASSERT(pDf->m_MinFanSpeed >= DT_BC_FANC_MIN_SPEED);
    DT_ASSERT(pDf->m_MinFanSpeed <= DT_BC_FANC_MAX_SPEED);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfTempFanMgr_OnEnablePostChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfTempFanMgr* pDf = (DtDfTempFanMgr*)pDfBase;

    // Sanity checks
    DF_TEMPFANMGR_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
         // Determine the number of fans
        pDf->m_NumFans = 0;
        if (pDf->m_pBcFanCtrl != NULL)
        {
            Bool  HasNoFan;
            Int  MeasPeriod, WdTimeout;
            DT_RETURN_ON_ERROR(DtBcFANC_GetConfig(pDf->m_pBcFanCtrl, &HasNoFan, 
                                                                &MeasPeriod, &WdTimeout));
            if (!HasNoFan)
            {
                // One fan found
                pDf->m_NumFans = 1;
                // Set initial fan speed
                DT_RETURN_ON_ERROR(DtBcFANC_SetFanSpeed(pDf->m_pBcFanCtrl, 
                                                   pDf->m_FanSpeed_x_Delay/pDf->m_Delay));
            }
        }

        // Get the temperature sensor properties
        DT_RETURN_ON_ERROR(DtDfTempFanMgr_InitTempSensorProperties(pDf));

        // If there are fans, enable the fan control
        DtSpinLockAcquire(&pDf->m_FanCtrlStateSpinLock);
        if (pDf->m_NumFans > 0)
           pDf->m_FanCtrlState = TEMPFANMGR_FANCTRL_ENABLED;
        else
           pDf->m_FanCtrlState = TEMPFANMGR_FANCTRL_DISABLED;
        DtSpinLockRelease(&pDf->m_FanCtrlStateSpinLock);
    }
    return Status;
}
//-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfTempFanMgr_OnEnablePreChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtDfTempFanMgr* pDf = (DtDfTempFanMgr*)pDfBase;

    // Sanity checks
    DF_TEMPFANMGR_DEFAULT_PRECONDITIONS(pDf);

    if (!Enable)
    { 
        // Disable the fan control
        DtSpinLockAcquire(&pDf->m_FanCtrlStateSpinLock);
        pDf->m_FanCtrlState = TEMPFANMGR_FANCTRL_DISABLED;
        DtSpinLockRelease(&pDf->m_FanCtrlStateSpinLock);
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_CleanUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfTempFanMgr_CleanUp(DtDfTempFanMgr*  pDf)
{
    // Clean-up temperature sensors
    if (pDf->m_pDfTempSensors != NULL)
    {
        DtVectorDf_Cleanup(pDf->m_pDfTempSensors);
        pDf->m_pDfTempSensors = NULL;
    }
    // Clean-up temperature properties
    if (pDf->m_pTempSensorProps != NULL)
    {
        DtVector_Cleanup(pDf->m_pTempSensorProps);
        pDf->m_pTempSensorProps = NULL;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfTempFanMgr_OpenChildren(DtDfTempFanMgr*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // List of children supported by the the TEMPFANMGR function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] =
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_FANC, DT_BC_FANC_NAME, FANC_ROLE_NONE,
                                          (DtObjectBcOrDf**)(&pDf->m_pBcFanCtrl), FALSE },
    };

    DF_TEMPFANMGR_DEFAULT_PRECONDITIONS(pDf);

    // Get the fan controller
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, TEMPFANMGR, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    // Get the temperature sensors
    Status = DtDf_OpenChildrenOfDfType((DtDf*)pDf, DT_FUNC_TYPE_SENSTEMP, 
                                                                   pDf->m_pDfTempSensors);
    if (!DT_SUCCESS(Status))
    {

        DtDbgOutDf(ERR, TEMPFANMGR, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }
    // Get the number of temperature sensors found
    pDf->m_NumTempSensors = DtVectorDf_Size(pDf->m_pDfTempSensors);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_PeriodicIntervalHandler -.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfTempFanMgr_PeriodicIntervalHandler(DtObject* pObj, DtTodTime  Time)
{
    DtDfTempFanMgr*  pDf = (DtDfTempFanMgr*)pObj;
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfTempFanMgr));

    DtSpinLockAcquireAtDpc(&pDf->m_FanCtrlStateSpinLock);
    // Only when enabled update the fan speed
    if (pDf->m_FanCtrlState == TEMPFANMGR_FANCTRL_ENABLED)
        DtDfTempFanMgr_UpdateFanSpeed(pDf);
    DtSpinLockReleaseFromDpc(&pDf->m_FanCtrlStateSpinLock);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTempFanMgr_UpdateFanSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Called from DPC
//
void DtDfTempFanMgr_UpdateFanSpeed(DtDfTempFanMgr*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfSensTemp* pTempSensor =  NULL;
    DtDfSensTempProperties* pTempProps = NULL;
    Int i;
    Int  Temperature;
    Int  MaxTempDiff = -1000;
    Int  FanSpeed;

    // We cannot control if there are no temperature sensors
    if (pDf->m_NumTempSensors == 0)
        return;

    // Do for all temperature sensors
    for (i=0; i<pDf->m_NumTempSensors; i++)
    {
        Int TempDiff = 0;
        // Get temperature sensor
        pTempSensor = (DtDfSensTemp*)DtVectorDf_At(pDf->m_pDfTempSensors, i);
        DT_ASSERT(pTempSensor != NULL);
        // Get temperature
        Status =  DtDfSensTemp_GetTemperature(pTempSensor, &Temperature);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, TEMPFANMGR, pDf, "ERROR: failed to get temperature");
            continue;
        }

        // Get temperature properties
        pTempProps = (DtDfSensTempProperties*)DtVector_At(pDf->m_pTempSensorProps, i);
        DT_ASSERT(pTempSensor != NULL);

        // Determine delta between current temperature and target
        TempDiff = Temperature - pTempProps->m_TargetTemperature;
        if (TempDiff > MaxTempDiff)
            MaxTempDiff = TempDiff;
    }

    // FanSpeed += MaxTempDiff*PeriodTime/Delay
    pDf->m_FanSpeed_x_Delay +=  MaxTempDiff * pDf->m_PeriodicInterval;
    if (pDf->m_FanSpeed_x_Delay > DT_BC_FANC_MAX_SPEED*pDf->m_Delay)
        pDf->m_FanSpeed_x_Delay = DT_BC_FANC_MAX_SPEED*pDf->m_Delay;
    else if (pDf->m_FanSpeed_x_Delay < pDf->m_MinFanSpeed*pDf->m_Delay)
        pDf->m_FanSpeed_x_Delay = pDf->m_MinFanSpeed*pDf->m_Delay;

    FanSpeed = pDf->m_FanSpeed_x_Delay / pDf->m_Delay;

    // Set new fan-speed
    Status = DtBcFANC_SetFanSpeed(pDf->m_pBcFanCtrl, FanSpeed);
    if (!DT_SUCCESS(Status))
        DtDbgOutDf(ERR, TEMPFANMGR, pDf, "ERROR: failed to set fan-speed");
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfTempFanMgr implementation +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtDf member to DtDfTempFanMgr
#define STUB_TEMPFANMGR   ((DtIoStubDfTempFanMgr*)pStub)
#define STUB_DF  ((DtDfTempFanMgr*)STUB_TEMPFANMGR->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus DtIoStubDfTempFanMgr_OnCmd(const DtIoStub* pStub,
    DtIoStubIoParams* pIoParams, Int * pOutSize);
static DtStatus  DtIoStubDfTempFanMgr_OnCmdGetFanStatus(const DtIoStubDfTempFanMgr*,
                                             const DtIoctlTempFanMgrCmdGetFanStatusInput*,
                                             DtIoctlTempFanMgrCmdGetFanStatusOutput*);
static DtStatus  DtIoStubDfTempFanMgr_OnCmdGetNumFans(const DtIoStubDfTempFanMgr*,
                                                   DtIoctlTempFanMgrCmdGetNumFansOutput*);
static DtStatus  DtIoStubDfTempFanMgr_OnCmdGetNumTempSens(const DtIoStubDfTempFanMgr*,
                                            DtIoctlTempFanMgrCmdGetNumTempSensorsOutput*);
static DtStatus  DtIoStubDfTempFanMgr_OnCmdGetTemperature(const DtIoStubDfTempFanMgr*,
                                           const DtIoctlTempFanMgrCmdGetTemperatureInput*,
                                           DtIoctlTempFanMgrCmdGetTemperatureOutput*);
static DtStatus  DtIoStubDfTempFanMgr_OnCmdGetTempProperties(const DtIoStubDfTempFanMgr*,
                                        const DtIoctlTempFanMgrCmdGetTempPropertiesInput*,
                                        DtIoctlTempFanMgrCmdGetTempPropertiesOutput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_TEMPFANMGR;

static const DtIoctlProperties  IOSTUB_DF_TEMPFANMGR_IOCTLS[] =
{
    DT_IOCTL_PROPS_TEMPFANMGR_CMD(
        DtIoStubDfTempFanMgr_OnCmd,
        NULL, NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfTempFanMgr - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTempFanMgr_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubDfTempFanMgr_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfTempFanMgr));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTempFanMgr_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubDfTempFanMgr*  DtIoStubDfTempFanMgr_Open(DtDf*  pDf)
{
    DtIoStubDfTempFanMgr*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    DF_TEMPFANMGR_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfTempFanMgr, pDf, NULL,
        DtIoStubDfTempFanMgr_Close,
        NULL,  // Use default IOCTL
        IOSTUB_DF_TEMPFANMGR_IOCTLS);
    pStub = (DtIoStubDfTempFanMgr*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;

    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfTempFanMgr - Private functions +=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTempFanMgr_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfTempFanMgr_OnCmd(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams,
    Int* pOutSize)
{
    DtStatus Status= DT_STATUS_OK;
    const DtIoctlTempFanMgrCmdInput*  pInData = NULL;
    DtIoctlTempFanMgrCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfTempFanMgr));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_TEMPFANMGR_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, TEMPFANMGR, pStub,
                                                   "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_TempFanMgrCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_TempFanMgrCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_TEMPFANMGR_CMD_GET_FAN_STATUS:
        DT_ASSERT(pOutData!=NULL && pInData!=NULL);
        Status = DtIoStubDfTempFanMgr_OnCmdGetFanStatus(STUB_TEMPFANMGR,
                                     &pInData->m_GetFanStatus, &pOutData->m_GetFanStatus);
        break;
    case DT_TEMPFANMGR_CMD_GET_NUM_FANS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfTempFanMgr_OnCmdGetNumFans(STUB_TEMPFANMGR,
                                                                 &pOutData->m_GetNumFans);
        break;
    case DT_TEMPFANMGR_CMD_GET_NUM_TEMPSENS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfTempFanMgr_OnCmdGetNumTempSens(STUB_TEMPFANMGR,
                                                                &pOutData->m_GetNumTemps);
        break;
    case DT_TEMPFANMGR_CMD_GET_TEMPERATURE:
        DT_ASSERT(pOutData!=NULL && pInData!=NULL);
        Status = DtIoStubDfTempFanMgr_OnCmdGetTemperature(STUB_TEMPFANMGR,
                                 &pInData->m_GetTemperature, &pOutData->m_GetTemperature);
        break;
    case DT_TEMPFANMGR_CMD_GET_TEMP_PROPS:
        DT_ASSERT(pOutData!=NULL && pInData!=NULL);
        Status = DtIoStubDfTempFanMgr_OnCmdGetTempProperties(STUB_TEMPFANMGR,
                                     &pInData->m_GetTempProps, &pOutData->m_GetTempProps);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTempFanMgr_OnCmdGetFanStatus -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfTempFanMgr_OnCmdGetFanStatus(
    const DtIoStubDfTempFanMgr*  pStub,
    const DtIoctlTempFanMgrCmdGetFanStatusInput*  pInData,
    DtIoctlTempFanMgrCmdGetFanStatusOutput* pOutData)
{
    Bool IsFanStuck, IsWdTimeout;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfTempFanMgr));
    DT_RETURN_ON_ERROR(DtDfTempFanMgr_GetFanStatus(STUB_DF, pInData->m_FanIndex,
                                       &pOutData->m_FanSpeedPct, &pOutData->m_FanSpeedRpm,
                                       &IsWdTimeout, &IsFanStuck));
    pOutData->m_IsWatchdogTimeout = (Int)IsWdTimeout;
    pOutData->m_IsFanStuck = (Int)IsFanStuck;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTempFanMgr_OnCmdGetNumFans -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfTempFanMgr_OnCmdGetNumFans(
    const DtIoStubDfTempFanMgr* pStub,
    DtIoctlTempFanMgrCmdGetNumFansOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfTempFanMgr));

    return DtDfTempFanMgr_GetNumFans(STUB_DF, &pOutData->m_NumFans);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTempFanMgr_OnCmdGetNumTempSens -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfTempFanMgr_OnCmdGetNumTempSens(
    const DtIoStubDfTempFanMgr*  pStub,
    DtIoctlTempFanMgrCmdGetNumTempSensorsOutput*  pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfTempFanMgr));

    return DtDfTempFanMgr_GetNumTempSensors(STUB_DF, &pOutData->m_NumTempSens);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTempFanMgr_OnCmdGetTemperature -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubDfTempFanMgr_OnCmdGetTemperature(
    const DtIoStubDfTempFanMgr*  pStub,
    const DtIoctlTempFanMgrCmdGetTemperatureInput*  pInData,
    DtIoctlTempFanMgrCmdGetTemperatureOutput*  pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfTempFanMgr));
    return DtDfTempFanMgr_GetTemperature(STUB_DF, pInData->m_TempSensIndex, 
                                                                &pOutData->m_Temperature);
}


//-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTempFanMgr_OnCmdGetTempProperties -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubDfTempFanMgr_OnCmdGetTempProperties(
    const DtIoStubDfTempFanMgr*  pStub,
    const DtIoctlTempFanMgrCmdGetTempPropertiesInput*  pInData,
    DtIoctlTempFanMgrCmdGetTempPropertiesOutput*  pOutData)
{
    Int  i;
    DtDfSensTempProperties  TempProps;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfTempFanMgr));

    // Get the properties
    DT_RETURN_ON_ERROR(DtDfTempFanMgr_GetTempProperties(STUB_DF, pInData->m_TempSensIndex,
                                                                             &TempProps));
    // Copy properties
    pOutData->m_MaximumTemperature = TempProps.m_MaximumTemperature;
    pOutData->m_TargetTemperature = TempProps.m_TargetTemperature;
    // Copy name
    i=0;
    if (TempProps.m_pTempSensorName != NULL)
    {
        for (; i<PROPERTY_STR_MAX_SIZE-1; i++)
        {
            pOutData->m_TempSensorName[i] = TempProps.m_pTempSensorName[i];
            // Check for end of string
            if (TempProps.m_pTempSensorName[i] == '\0')
                break;
        }
    }
    pOutData->m_TempSensorName[i] = '\0';

    return DT_STATUS_OK;
}

