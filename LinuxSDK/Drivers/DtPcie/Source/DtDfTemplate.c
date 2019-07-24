//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfTemplate.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
// This file is not part of the driver. It can be used as template to create a new
// driver function. Replace "TEMPLATE" by the driver function's short name 
// (all uppercase) and replace "Template" by the driver function's short name in camel
// case.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018 DekTec Digital Video B.V.
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
#include "DtDfTemplate.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        
#define BLKCTRL_ROLE_NONE      NULL


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfTemplate implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiTx function
#define DF_TEMPLATE_DEFAULT_PRECONDITIONS(pDf)      \
                                 DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfTemplate))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_TEMPLATE_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(TEMPLATE, pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfTemplate_Init(DtDf*);

static DtStatus  DtDfTemplate_LoadParameters(DtDf*);
static DtStatus  DtDfTemplate_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfTemplate_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfTemplate_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfTemplate_OpenChildren(DtDfTemplate*);
static void  DtDfTemplate_HelperThreadEntry(DtThread*, void* pContext);
static void  DtDfTemplate_EnablePeriodicIntervalHandler(DtDfTemplate*, Bool  Enable);
static void  DtDfTemplate_PeriodicIntervalHandler(DtObject*, DtTodTime  Time);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfTemplate - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTemplate_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfTemplate_Close(DtDf*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    DF_TEMPLATE_DEFAULT_PRECONDITIONS(pDf);

    // Unregister periodic interrupt handler
    DtCore_TOD_PeriodicItvUnregister(pDf->m_pCore, (DtObject*)pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTemplate_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfTemplate*  DtDfTemplate_Open(DtCore* pCore, DtPt* pPt, const char* pRole, 
                                                  Int Instance, Int Uuid, Bool CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_TEMPLATE_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfTemplate, Id, DT_FUNC_TYPE_TEMPLATE, pPt,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfTemplate_Close;
    OpenParams.m_InitFunc = DtDfTemplate_Init;
    OpenParams.m_LoadParsFunc = DtDfTemplate_LoadParameters;
    OpenParams.m_OnCloseFileFunc = DtDfTemplate_OnCloseFile;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfTemplate_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfTemplate_OnEnablePreChildren;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfTemplate*)DtDf_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTemplate_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfTemplate_GetOperationalMode(DtDfTemplate* pDf, Int* pOpMode)
{
    // Sanity check
    DF_TEMPLATE_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_TEMPLATE_MUST_BE_ENABLED(pDf);

    // Return cached operational mode
    *pOpMode = pDf->m_OperationalMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTemplate_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfTemplate_SetOperationalMode(DtDfTemplate* pDf, Int OpMode)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_TEMPLATE_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (OpMode!=DT_FUNC_OPMODE_IDLE && OpMode!=DT_FUNC_OPMODE_STANDBY
                                                            && OpMode!=DT_FUNC_OPMODE_RUN)
    {
        DtDbgOutDf(ERR, TEMPLATE, pDf, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    DF_TEMPLATE_MUST_BE_ENABLED(pDf);

    // No change?
    if (pDf->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // TODO  implementation

    // Save new operational mode
    if (DT_SUCCESS(Status))
        pDf->m_OperationalMode = OpMode;

    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfTemplate - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTemplate_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfTemplate_Init(DtDf* pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfTemplate* pDf = (DtDfTemplate*)pDfBase;
    DtOnPeriodicIntervalRegData  RegData;

    // Sanity checks
    DF_TEMPLATE_DEFAULT_PRECONDITIONS(pDf);

    // Set defaults
    pDf->m_OperationalMode = DT_FUNC_OPMODE_IDLE;

    //Open children
    Status = DtDfTemplate_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, TEMPLATE, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    // Initialize the helper thread
    DT_RETURN_ON_ERROR(DtThreadInit(&pDf->m_HelperThread, DtDfTemplate_HelperThreadEntry,
                                                                                    pDf));
    DtEventInit(&pDf->m_HelperStopEvent, FALSE);


    // Initialize the periodic interval handler spinlock and enable flag
    DtSpinLockInit(&pDf->m_PerItvSpinLock);
    pDf->m_PerItvEnable = FALSE;

    // Register periodic interval handler
    RegData.m_OnPeriodicFunc = DtDfTemplate_PeriodicIntervalHandler;
    RegData.m_pObject = (DtObject*)pDf;
    Status = DtCore_TOD_PeriodicItvRegister(pDf->m_pCore, &RegData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, TEMPLATE, pDf, "ERROR: failed to register period itv handler");
        return Status;
    }
    return DT_STATUS_OK;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTemplate_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Note: LoadParameters() is called before the Init(). The loaded parameters can be used
// in the Init().
//
DtStatus  DtDfTemplate_LoadParameters(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfTemplate* pDf = (DtDfTemplate*)pDfBase;

    // List of Template function parameters
    DtDfParameters  DFTEMPLATE_PARS[] =
    {
        // Name,  Value Type,  Value*
        { "PARAM_1", PROPERTY_VALUE_TYPE_INT, &(pDf->m_Param1) },
        { "PARAM_2", PROPERTY_VALUE_TYPE_STRING, (void*)&(pDf->m_pParam2) },
    };

    // Sanity checks
    DF_TEMPLATE_DEFAULT_PRECONDITIONS(pDf);

    // Set defaults
    pDf->m_Param1 = -1;
    pDf->m_pParam2 = "";
    
    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFTEMPLATE_PARS), 
                                                                         DFTEMPLATE_PARS);
    if (!DT_SUCCESS(Status))
        return Status;

    return DT_STATUS_OK;
}
//.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTemplate_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfTemplate_OnEnablePostChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfTemplate* pDf = (DtDfTemplate*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
     
    // Sanity check
    DF_TEMPLATE_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutDf(AVG, TEMPLATE, pDf, "DISABLE -> ENABLE");

        // Operational mode must be IDLE
        DT_ASSERT(pDf->m_OperationalMode == DT_FUNC_OPMODE_IDLE);

        // Set operational mode of BLKCTRL to IDLE
        DT_RETURN_ON_ERROR(DtBcBLKCTRL_SetOperationalMode(pDf->m_pBcBlkCtrl,
                                                                   DT_BLOCK_OPMODE_IDLE));
        // Start the helper thread
        DtEventReset(&pDf->m_HelperStopEvent);
        Status = DtThreadStart(&pDf->m_HelperThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, TEMPLATE, pDf, "ERROR: failed to start HelperThread");
            return Status;
        }

        // Enable periodic interval handler
        DtDfTemplate_EnablePeriodicIntervalHandler(TRUE);
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTemplate_OnEnablePreChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfTemplate_OnEnablePreChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfTemplate* pDf = (DtDfTemplate*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
    // Sanity check
    DF_TEMPLATE_DEFAULT_PRECONDITIONS(pDf);
    if (!Enable)
    {
        // ENABLE -> DISABLE
        DtDbgOutDf(AVG, TEMPLATE, pDf, "ENABLE -> DISABLE");

        // Disable periodic interval handler
        DtDfTemplate_EnablePeriodicIntervalHandler(FALSE);

        // Stop helper thread
        DtEventSet(&pDf->m_HelperStopEvent);
        Status = DtThreadStop(&pDf->m_HelperThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, TEMPLATE, pDf, "ERROR: failed to stop HelperThread");
            return Status;
        }

        // Return to IDLE
        DT_RETURN_ON_ERROR(DtDfTemplate_SetOperationalMode(pDf, DT_FUNC_OPMODE_IDLE));
    }

    return DT_STATUS_OK;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTemplate_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtDfTemplate_OnCloseFile(DtDf* pDfBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfTemplate* pDf = (DtDfTemplate*)pDfBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    // Sanity checks
    DF_TEMPLATE_DEFAULT_PRECONDITIONS(pDf);

    // Check if the owner closed the file handle
    Status = DtDf_ExclAccessCheck((DtDf*)pDf, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutDf(AVG, TEMPLATE, pDf, "Go to IDLE");

        // Go to IDLE
        Status = DtDfTemplate_SetOperationalMode(pDf, DT_FUNC_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, TEMPLATE, pDf, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtDf_OnCloseFile((DtDf*)pDf, pFile);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTemplate_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfTemplate_OpenChildren(DtDfTemplate*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // List of children supported by the the TEMPLATE function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_BLKCTRL, DT_BC_BLKCTRL_NAME,
                         BLKCTRL_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcBlkCtrl), TRUE},
    };

    DF_TEMPLATE_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    // BLKCTRL should be present
    DT_ASSERT(pDf->m_pBcBlkCtrl != NULL);

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTemplate_HelperThreadEntry -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Note that for asynchronous calls that use a thread to execute the function you better
// can use DtWorkItem.
//
void  DtDfTemplate_HelperThreadEntry(DtThread* pThread, void* pContext)
{
    DtDfTemplate*  pDf = (DtDfTemplate*)pContext;
    Bool  StopThread = FALSE;
    const Int  LoopTimeOut = 100;

    while (!StopThread)
    {
        // Wait for stop event or poll-loop timeout
        DtStatus Status = DtEventWaitUnInt(&pDf->m_HelperStopEvent, LoopTimeOut);
        if (Status!=DT_STATUS_TIMEOUT || pThread->m_StopThread)
        {
            if (!DT_SUCCESS(Status))
                DtDbgOutDf(ERR, TEMPLATE, pDf, "ERROR: HelperThread failure");
            if (pThread->m_StopThread)
                DtDbgOutDf(AVG, TEMPLATE, pDf, "HelperThread stop request received");
            StopThread = TRUE;
            continue;
        }

        // Poll-loop timeout occurred
        // Do your work here
        // ...
    }
    // We have to wait until the thread received a stop command.
    DtThreadWaitForStop(pThread);
}


// -.-.-.-.-.-.-.-.-.-.- DtDfTemplate_EnablePeriodicIntervalHandler -.-.-.-.-.-.-.-.-.-.-.
//
void DtDfTemplate_EnablePeriodicIntervalHandler(DtDfTemplate* pDf, Bool Enable)
{
    // Enable/disable handling periodic interval
    // A spinlock is used to prevent that handling is disabled while the
    // periodic interval handler is running
    DtSpinLockAcquire(&pDf->m_PerItvSpinLock);
    pDf->m_PerItvEnable = Enable;
    DtSpinLockRelease(&pDf->m_PerItvSpinLock);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTemplate_PeriodicIntervalHandler -.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfTemplate_PeriodicIntervalHandler(DtObject* pObj, DtTodTime  Time)
{
    DtDfTemplate*  pDf = (DtDfTemplate*)pObj;
    DF_TEMPLATE_DEFAULT_PRECONDITIONS(pDf);

    DtSpinLockAcquireAtDpc(&pDf->m_PerItvSpinLock);
    // Only when enabled
    if (pDf->m_PerItvEnable)
    {

    }
    DtSpinLockReleaseFromDpc(&pDf->m_PerItvSpinLock);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfTemplate implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Df's stub
#define TEMPLATE_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                      DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfTemplate));
// Helper macro to cast stub's DtDf member to DtDfTemplate
#define STUB_TEMPLATE   ((DtIoStubDfTemplate*)pStub)
#define STUB_DF  ((DtDfTemplate*)STUB_TEMPLATE->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus DtIoStubDfTemplate_OnCmd(const DtIoStub* pStub, 
                                             DtIoStubIoParams* pIoParams, Int * pOutSize);
static DtStatus  DtIoStubDfTemplate_OnCmdGetOperationalMode(const DtIoStubDfTemplate*,
                                                      DtIoctlTemplateCmdGetOpModeOutput*);
static DtStatus  DtIoStubDfTemplate_OnCmdSetOperationalMode(const DtIoStubDfTemplate*,
                                                 const DtIoctlTemplateCmdSetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_TEMPLATE;

static const DtIoctlProperties  IOSTUB_DF_TEMPLATE_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubDf_OnExclAccessCmd,
        NULL, NULL),
    DT_IOCTL_PROPS_TEMPLATE_CMD(
        DtIoStubDfTemplate_OnCmd, 
        NULL, NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfTemplate - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTemplate_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubDfTemplate_Close(DtIoStub* pStub)
{
    TEMPLATE_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTemplate_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubDfTemplate*  DtIoStubDfTemplate_Open(DtDf*  pDf)
{
    DtIoStubDfTemplate*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    DF_TEMPLATE_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfTemplate, pDf, NULL, 
                                                             DtIoStubDfTemplate_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_DF_TEMPLATE_IOCTLS);
    pStub = (DtIoStubDfTemplate*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfTemplate - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTemplate_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfTemplate_OnCmd(
    const DtIoStub*  pStub, 
    DtIoStubIoParams*  pIoParams, 
    Int* pOutSize)
{
    DtStatus Status= DT_STATUS_OK;
    const DtIoctlTemplateCmdInput*  pInData = NULL;
    DtIoctlTemplateCmdOutput*  pOutData = NULL;
    
    TEMPLATE_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_TEMPLATE_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, TEMPLATE, pStub, "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_TemplateCmd;
    if (pIoParams->m_OutReqSize > 0)
    { 
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_TemplateCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_TEMPLATE_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfTemplate_OnCmdGetOperationalMode(STUB_TEMPLATE, 
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_TEMPLATE_CMD_SET_OPERATIONAL_MODE:
        Status = DtIoStubDfTemplate_OnCmdSetOperationalMode(STUB_TEMPLATE, 
                                                                   &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTemplate_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfTemplate_OnCmdGetOperationalMode(
    const DtIoStubDfTemplate*  pStub, 
    DtIoctlTemplateCmdGetOpModeOutput*  pOutData)
{
    TEMPLATE_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtDfTemplate_GetOperationalMode(STUB_DF, &pOutData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfTemplate_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfTemplate_OnCmdSetOperationalMode(
    const DtIoStubDfTemplate*  pStub, 
    const DtIoctlTemplateCmdSetOpModeInput*  pInData)
{
    TEMPLATE_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtDfTemplate_SetOperationalMode(STUB_DF, pInData->m_OpMode);
}
