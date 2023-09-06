//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSensTemp.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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
#include "DtDfSensTemp.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

#define FPGATEMP_ROLE_NONE   NULL
#define MXDS75TEMP_ROLE_NONE  NULL
#define MCTC72TEMP_ROLE_NONE  NULL

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSensTemp implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiTx function
#define DF_SENSTEMP_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfSensTemp))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_SENSTEMP_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(SENSTEMP, pDf)

// Helper macro to cast a DtDf* to a DtDfSensTemp*
#define DF_SENSTEMP      ((DtDfSensTemp*)pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfSensTemp_Init(DtDf*);
static DtStatus  DtDfSensTemp_LoadParameters(DtDf*);
static DtStatus  DtDfSensTemp_OpenChildren(DtDfSensTemp*);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSensTemp - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSensTemp_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfSensTemp_Close(DtDf*  pDf)
{
    DF_SENSTEMP_DEFAULT_PRECONDITIONS(pDf);


    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSensTemp_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfSensTemp*  DtDfSensTemp_Open(DtCore*  pCore, DtPt*  pPt,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    // Init open parameters
    DT_DF_SENSTEMP_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfSensTemp, Id, DT_FUNC_TYPE_SENSTEMP, pPt,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfSensTemp_Close;
    OpenParams.m_InitFunc = DtDfSensTemp_Init;
    OpenParams.m_LoadParsFunc = DtDfSensTemp_LoadParameters;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfSensTemp*)DtDf_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSensTemp_GetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSensTemp_GetProperties(DtDfSensTemp* pDf, 
                                                    DtDfSensTempProperties* pProperties)
{
    // Sanity check 
    DF_SENSTEMP_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pProperties == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Copy properties
    pProperties->m_TempSensorType = pDf->m_TempSensorType;
    pProperties->m_MaximumTemperature = pDf->m_MaxTemperature;
    pProperties->m_TargetTemperature = pDf->m_TargetTemperature;
    pProperties->m_pTempSensorName= pDf->m_pTempSensorName;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSensTemp_GetTemperature -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSensTemp_GetTemperature(DtDfSensTemp* pDf, Int* pTemperature)
{
     DtStatus  Status = DT_STATUS_OK;
     // Sanity check 
    DF_SENSTEMP_DEFAULT_PRECONDITIONS(pDf);

    DF_MUST_BE_ENABLED_IMPL(SENSTEMP, pDf);

    // Check parameter
    if (pTemperature == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Get temperature
    switch (pDf->m_TempSensorType)
    {
    case DT_DF_SENSTEMP_SENS_TYPE_FPGA:
        Status =  DtBcFPGATEMP_GetTemperature(pDf->m_pBcFpgaTemp, pTemperature);
        break;
    case DT_DF_SENSTEMP_SENS_TYPE_MCTC72:
        Status =  DtDfMcTc72Temp_GetTemperature(pDf->m_pDfMcTc72Temp, pTemperature);
        break;
    case DT_DF_SENSTEMP_SENS_TYPE_MXDS75:
        Status =  DtDfMxDs75Temp_GetTemperature(pDf->m_pDfMxDs75Temp, pTemperature);
        break;
    default:
        DT_ASSERT(FALSE);
        Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSensTemp - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSensTemp_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSensTemp_Init(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfSensTemp* pDf = (DtDfSensTemp*)pDfBase;

    // Sanity checks
    DF_SENSTEMP_DEFAULT_PRECONDITIONS(pDf);

    // Initialize members
    pDf->m_TempSensorType = DT_DF_SENSTEMP_SENS_TYPE_UNKNOWN;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfSensTemp_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SENSTEMP, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSensTemp_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSensTemp_LoadParameters(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfSensTemp* pDf = (DtDfSensTemp*)pDfBase;

    // List of fan manager  function parameters
    DtDfParameters  DFTEMP_PARS[] =
    {
        // Name,  Value Type,  Value*
        { "TARGET_TEMP", PROPERTY_VALUE_TYPE_INT, &(pDf->m_TargetTemperature) },
        { "MAX_TEMP", PROPERTY_VALUE_TYPE_INT, &(pDf->m_MaxTemperature) },
        { "NAME", PROPERTY_VALUE_TYPE_STRING, (void*)&(pDf->m_pTempSensorName) },
    };

    // Sanity checks
    DF_SENSTEMP_DEFAULT_PRECONDITIONS(pDf);

    // Set defaults
    pDf->m_MaxTemperature = 80;
    pDf->m_TargetTemperature = 50;
    pDf->m_pTempSensorName = "";
    
    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFTEMP_PARS), DFTEMP_PARS);
    if (!DT_SUCCESS(Status))
        return Status;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSensTemp_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSensTemp_OpenChildren(DtDfSensTemp*  pDf)
{
    Int NumTempSensFound = 0;
    DtStatus  Status = DT_STATUS_OK;

    // List of children supported by the the SENSTEMP function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] =
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_FPGATEMP, DT_BC_FPGATEMP_NAME,
                      FPGATEMP_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcFpgaTemp), FALSE},
        { DT_OBJECT_TYPE_DF, DT_FUNC_TYPE_MXDS75TEMP, DT_DF_MXDS75TEMP_NAME,
                  MXDS75TEMP_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pDfMxDs75Temp), FALSE},
        { DT_OBJECT_TYPE_DF, DT_FUNC_TYPE_MCTC72TEMP, DT_DF_MCTC72TEMP_NAME,
                  MCTC72TEMP_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pDfMcTc72Temp), FALSE},
    };

    DF_SENSTEMP_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    // Only one of the temperature controllers should be present
    if (pDf->m_pBcFpgaTemp != NULL)
        NumTempSensFound++;
    if (pDf->m_pDfMcTc72Temp != NULL)
        NumTempSensFound++;
    if (pDf->m_pDfMxDs75Temp != NULL)
        NumTempSensFound++;
    DT_ASSERT(NumTempSensFound == 1);

    // Determine sensor type
    if (pDf->m_pBcFpgaTemp != NULL)
        pDf->m_TempSensorType = DT_DF_SENSTEMP_SENS_TYPE_FPGA;
    else if (pDf->m_pDfMcTc72Temp != NULL)
        pDf->m_TempSensorType = DT_DF_SENSTEMP_SENS_TYPE_MCTC72;
    else if (pDf->m_pDfMxDs75Temp != NULL)
        pDf->m_TempSensorType = DT_DF_SENSTEMP_SENS_TYPE_MXDS75;
    else
        return DT_STATUS_FAIL;

    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfSensTemp implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtDf member to DtDfSensTemp
#define STUB_SENSTEMP   ((DtIoStubDfSensTemp*)pStub)
#define STUB_DF  ((DtDfSensTemp*)STUB_SENSTEMP->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus DtIoStubDfSensTemp_OnCmd(const DtIoStub* pStub,
    DtIoStubIoParams* pIoParams, Int * pOutSize);
static DtStatus  DtIoStubDfSensTemp_OnCmdGetProperties(const DtIoStubDfSensTemp*,
                                                DtIoctlSensTempCmdGetPropertiesOutput*);
static DtStatus  DtIoStubDfSensTemp_OnCmdGetTemperature(const DtIoStubDfSensTemp*,
                                               DtIoctlSensTempCmdGetTemperatureOutput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_SENSTEMP;

static const DtIoctlProperties  IOSTUB_DF_SENSTEMP_IOCTLS[] =
{
    DT_IOCTL_PROPS_SENSTEMP_CMD(
        DtIoStubDfSensTemp_OnCmd,
        NULL, NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfSensTemp - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSensTemp_Close -   -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubDfSensTemp_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSensTemp));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSensTemp_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubDfSensTemp*  DtIoStubDfSensTemp_Open(DtDf*  pDf)
{
    DtIoStubDfSensTemp*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    DF_SENSTEMP_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfSensTemp, pDf, NULL,
        DtIoStubDfSensTemp_Close,
        NULL,  // Use default IOCTL
        IOSTUB_DF_SENSTEMP_IOCTLS);
    pStub = (DtIoStubDfSensTemp*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfSensTemp - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSensTemp_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfSensTemp_OnCmd(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams,
    Int* pOutSize)
{
    DtStatus Status= DT_STATUS_OK;
    const DtIoctlSensTempCmdInput*  pInData = NULL;
    DtIoctlSensTempCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSensTemp));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_SENSTEMP_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf,
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, SENSTEMP, pStub, "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SensTempCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_SensTempCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_SENSTEMP_CMD_GET_PROPERTIES:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSensTemp_OnCmdGetProperties(STUB_SENSTEMP, 
                                                              &pOutData->m_GetProperties);
        break;
    case DT_SENSTEMP_CMD_GET_TEMPERATURE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSensTemp_OnCmdGetTemperature(STUB_SENSTEMP,
                                                             &pOutData->m_GetTemperature);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSensTemp_OnCmdGetProperties -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfSensTemp_OnCmdGetProperties(
    const DtIoStubDfSensTemp*  pStub,
    DtIoctlSensTempCmdGetPropertiesOutput* pOutData)
{
    DtDfSensTempProperties  Properties;
    Int i=0;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSensTemp));
    DT_ASSERT(pOutData != NULL);

    DT_RETURN_ON_ERROR(DtDfSensTemp_GetProperties(STUB_DF, &Properties));

    // Copy properties
    pOutData->m_MaximumTemperature = Properties.m_MaximumTemperature;
    pOutData->m_TargetTemperature = Properties.m_TargetTemperature;
    // Copy name
    i=0;
    if (Properties.m_pTempSensorName != NULL)
    {
        for (; i<PROPERTY_STR_MAX_SIZE-1; i++)
        {
            pOutData->m_TempSensorName[i] = Properties.m_pTempSensorName[i];
            // Check for end of string
            if (Properties.m_pTempSensorName[i] == '\0')
                break;
        }
    }
    pOutData->m_TempSensorName[i] = '\0';

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSensTemp_OnCmdGetTemperature -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSensTemp_OnCmdGetTemperature(
    const DtIoStubDfSensTemp*  pStub,
    DtIoctlSensTempCmdGetTemperatureOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSensTemp));
    DT_ASSERT(pOutData != NULL);

    return DtDfSensTemp_GetTemperature(STUB_DF, &pOutData->m_Temperature);
}
