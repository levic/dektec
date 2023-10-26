// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCfTod.c *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
#include "DtCfTod.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfTod implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtCfTod function
#define CF_TOD_DEFAULT_PRECONDITIONS(pCf)      \
    DT_ASSERT(pCf!=NULL && pCf->m_Size==sizeof(DtCfTod))

// Helper macro to cast a DtDf* to a DtCfTod*
#define CF_TOD      ((DtCfTod*)pCf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtCfTod_Init(DtDf*);
static DtStatus  DtCfTod_OpenChildren(DtCfTod*);
static void  DtCfTod_OnPeriodicInterval(DtObject*, DtTodTime  Time);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfTod - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtCfTod_Close(DtDf*  pCf)
{
    // Sanity checks
    CF_TOD_DEFAULT_PRECONDITIONS(pCf);

    if (CF_TOD->m_pOnPeriodicHandlers != NULL)
    {
        DtVector_Cleanup(CF_TOD->m_pOnPeriodicHandlers);
        CF_TOD->m_pOnPeriodicHandlers = NULL;
    }
    
    // Let base function perform final clean-up
    DtDf_Close(pCf);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtCfTod*  DtCfTod_Open(DtCore*  pCore, const char*  pRole, Int  Instance, 
                                                              Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    
    // Init open parameters
    DT_CF_TOD_INIT_ID(Id, pRole, Instance, Uuid);
    DT_CF_INIT_OPEN_PARAMS(OpenParams, DtCfTod, Id, DT_FUNC_TYPE_CF_TOD,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtCfTod_Close;
    OpenParams.m_InitFunc = DtCfTod_Init;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtCfTod*)DtDf_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_AdjustPhaseIncr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfTod_AdjustPhaseIncr(DtCfTod* pCf, Int64 ScaledPpm)
{
    // Sanity checks
    CF_TOD_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pCf->m_pBcTod != NULL);

    // Be defensive and fail if we donot have a TOD-BC
    if (pCf->m_pBcTod == NULL)
    {
        DtDbgOutCf(ERR, TOD, pCf, "ERROR: no TOD-BC has been loaded");
        return DT_STATUS_FAIL;
    }
    // FOR NOW: just let the TOD-BC do all of the heavy lifting
    return DtBcTOD_AdjustPhaseIncr(pCf->m_pBcTod, ScaledPpm);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_AdjustTime -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfTod_AdjustTime(DtCfTod*  pCf, Int64  DeltaNanoseconds)
{
    // Sanity checks
    CF_TOD_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pCf->m_pBcTod != NULL);

    // Be defensive and fail if we donot have a TOD-BC
    if (pCf->m_pBcTod == NULL)
    {
        DtDbgOutCf(ERR, TOD, pCf, "ERROR: no TOD-BC has been loaded");
        return DT_STATUS_FAIL;
    }
    // FOR NOW: just let the TOD-BC do all of the heavy lifting
    return DtBcTOD_Adjust(pCf->m_pBcTod,  DeltaNanoseconds);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_GetAdjustPpb -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtCfTod_GetAdjustPpb(DtCfTod* pCf, Int* pAdjustPpb)
{
    Int64 AdjustScaledPpm = 0, AdjustPpb=0;
    DtStatus Status = DT_STATUS_OK;
    Bool Neg = FALSE;

    // Sanity checks
    CF_TOD_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pCf->m_pBcTod != NULL);
    // Be defensive and fail if we donot have a TOD-BC
    if (pCf->m_pBcTod == NULL)
    {
        DtDbgOutCf(ERR, TOD, pCf, "ERROR: no TOD-BC has been loaded");
        return DT_STATUS_FAIL;
    }
    // FOR NOW: just let the TOD-BC do all of the heavy lifting
    Status =  DtBcTOD_GetPhaseIncrAdjust(pCf->m_pBcTod, &AdjustScaledPpm);
    if (Status != DT_STATUS_OK)
        return Status;

    // Convert scaled ppm to ppb
    Neg = (AdjustScaledPpm < 0);
    if (Neg)
        AdjustScaledPpm = -AdjustScaledPpm;
    AdjustPpb = DtDivide64(AdjustScaledPpm*1000 + (1<<15), 1<<16, NULL);
    if (Neg)
        *pAdjustPpb = (Int)-AdjustPpb;
    else
        *pAdjustPpb = (Int)AdjustPpb;
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_GetPeriodicItv -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtCfTod_GetPeriodicItv(DtCfTod* pCf, Int* pIntervalMs)
{
    // Sanity checks
    CF_TOD_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pCf->m_pBcTod != NULL);
    // Be defensive and fail if we donot have a TOD-BC
    if (pCf->m_pBcTod == NULL)
    {
        DtDbgOutCf(ERR, TOD, pCf, "ERROR: no TOD-BC has been loaded");
        return DT_STATUS_FAIL;
    }
    // FOR NOW: just let the TOD-BC do all of the heavy lifting
    return DtBcTOD_GetPeriodicItv(pCf->m_pBcTod, pIntervalMs);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_GetPhaseIncr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfTod_GetPhaseIncr(DtCfTod*  pCf, UInt* pClockPhaseIncr)
{
    // Sanity checks
    CF_TOD_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pCf->m_pBcTod != NULL);

    // Be defensive and fail if we donot have a TOD-BC
    if (pCf->m_pBcTod == NULL)
    {
        DtDbgOutCf(ERR, TOD, pCf, "ERROR: no TOD-BC has been loaded");
        return DT_STATUS_FAIL;
    }
    // FOR NOW: just let the TOD-BC do all of the heavy lifting
    return DtBcTOD_GetPhaseIncr(pCf->m_pBcTod, pClockPhaseIncr);
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_GetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfTod_GetProperties(DtCfTod*  pCf, UInt* pSysClockFreqHz, UInt* pAccuracy)
{
    // Sanity checks
    CF_TOD_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pCf->m_pBcTod != NULL);

    // Be defensive and fail if we donot have a TOD-BC
    if (pCf->m_pBcTod == NULL)
    {
        DtDbgOutCf(ERR, TOD, pCf, "ERROR: no TOD-BC has been loaded");
        return DT_STATUS_FAIL;
    }
    // FOR NOW: just let the TOD-BC do all of the heavy lifting
    return DtBcTOD_GetProperties(pCf->m_pBcTod, pSysClockFreqHz, pAccuracy);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_GetTime -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfTod_GetTime(DtCfTod*  pCf, DtTodTime*  pTime, UInt* pAdjustCount)
{
    // Sanity checks
    CF_TOD_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pCf->m_pBcTod != NULL);
    DT_ASSERT(pTime!=NULL && pAdjustCount!=NULL);

    // Be defensive and fail if we donot have a TOD-BC
    if (pCf->m_pBcTod == NULL)
    {
        DtDbgOutCf(ERR, TOD, pCf, "ERROR: no TOD-BC has been loaded");
        return DT_STATUS_FAIL;
    }
    // FOR NOW: just let the TOD-BC do all of the heavy lifting
    return DtBcTOD_GetTime(pCf->m_pBcTod, pTime, pAdjustCount);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_SetPhaseIncr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfTod_SetPhaseIncr(DtCfTod*  pCf, UInt  ClockPhaseIncr)
{
    // Sanity checks
    CF_TOD_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pCf->m_pBcTod != NULL);

    // Be defensive and fail if we donot have a TOD-BC
    if (pCf->m_pBcTod == NULL)
    {
        DtDbgOutCf(ERR, TOD, pCf, "ERROR: no TOD-BC has been loaded");
        return DT_STATUS_FAIL;
    }
    // FOR NOW: just let the TOD-BC do all of the heavy lifting
    return DtBcTOD_SetPhaseIncr(pCf->m_pBcTod, ClockPhaseIncr);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_SetTime -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfTod_SetTime(DtCfTod*  pCf, DtTodTime  Time)
{
    // Sanity checks
    CF_TOD_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pCf->m_pBcTod != NULL);

    // Be defensive and fail if we donot have a TOD-BC
    if (pCf->m_pBcTod == NULL)
    {
        DtDbgOutCf(ERR, TOD, pCf, "ERROR: no TOD-BC has been loaded");
        return DT_STATUS_FAIL;
    }

    // FOR NOW: just let the TOD-BC do all of the heavy lifting
    return DtBcTOD_SetTime(pCf->m_pBcTod, Time);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_PeriodicIntervalRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfTod_PeriodicIntervalRegister(
                              DtCfTod*  pCf, const DtOnPeriodicIntervalRegData*  pRegData)
{
    Int  i, NumElems = 0;

    // Sanity checks
    CF_TOD_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pCf->m_pOnPeriodicHandlers != NULL);
    DT_ASSERT(pRegData->m_pObject != NULL);
    DT_ASSERT(pRegData->m_pObject->m_ObjectType==DT_OBJECT_TYPE_BC || 
                                    pRegData->m_pObject->m_ObjectType==DT_OBJECT_TYPE_DF);

    // Check if we already have an entry for this object
    NumElems = DtVector_Size(pCf->m_pOnPeriodicHandlers);
    for (i=0; i<NumElems; i++)
    {
        DtOnPeriodicIntervalRegData*  p = 
                 (DtOnPeriodicIntervalRegData*)DtVector_At(pCf->m_pOnPeriodicHandlers, i);
        if (p->m_pObject != pRegData->m_pObject)
            continue;

        // Overwrite existing element
        p->m_OnPeriodicFunc = pRegData->m_OnPeriodicFunc;
        return DT_STATUS_OK;
    }
    // Add new element
    return DtVector_PushBack(pCf->m_pOnPeriodicHandlers, pRegData);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_PeriodicIntervalUnregister -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfTod_PeriodicIntervalUnregister(DtCfTod*  pCf, const DtObject*  pObject)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i, NumElems = 0;

    // Sanity check
    CF_TOD_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pCf->m_pOnPeriodicHandlers != NULL);
    DT_ASSERT(pObject != NULL);
    DT_ASSERT(pObject->m_ObjectType==DT_OBJECT_TYPE_BC || 
                                                pObject->m_ObjectType==DT_OBJECT_TYPE_DF);

    // Find entry for the specified object and erase it
    NumElems = DtVector_Size(pCf->m_pOnPeriodicHandlers);
    for (i=0; i<NumElems; i++)
    {
        const DtOnPeriodicIntervalRegData*  p = 
                 (DtOnPeriodicIntervalRegData*)DtVector_At(pCf->m_pOnPeriodicHandlers, i);
        if (p->m_pObject != pObject)
            continue;

        // Erase the element
        Status = DtVector_Erase(pCf->m_pOnPeriodicHandlers, i);
        return Status;
    }
    return DT_STATUS_NOT_FOUND;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfTod - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfTod_Init(DtDf*  pCf)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    CF_TOD_DEFAULT_PRECONDITIONS(pCf);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    Status = DtCfTod_OpenChildren(CF_TOD);
    if (!DT_SUCCESS(Status))
    {   
        DtDbgOutCf(ERR, TOD, pCf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    // -.-.-.-.-.-.-.-.- Create the periodic-interval-registration list -.-.-.-.-.-.-.-.-.

    DT_ASSERT(CF_TOD->m_pOnPeriodicHandlers == NULL);
    CF_TOD->m_pOnPeriodicHandlers = 
                               DtVector_Create(8, sizeof(DtOnPeriodicIntervalRegData), 4);
    if (CF_TOD->m_pOnPeriodicHandlers == NULL)
    {
        DtDbgOutCf(ERR, TOD, pCf, "ERROR: failed to create int-handler list");
        return DT_STATUS_OUT_OF_MEMORY;
    }
       
    // Register the periodic-interval-handler function
    Status = DtBcTOD_PeriodicIntervalRegister(CF_TOD->m_pBcTod, 
                                                               DtCfTod_OnPeriodicInterval,
                                                               (DtObject*)CF_TOD);
    DT_ASSERT(DT_SUCCESS(Status));
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfTod_OpenChildren(DtCfTod*  pCf)
{
    DtStatus  Status = DT_STATUS_OK;
    
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_TOD, DT_BC_TOD_NAME, NULL,
                                               (DtObjectBcOrDf**)(&pCf->m_pBcTod), TRUE },
    };

    // Sanity checks
    CF_TOD_DEFAULT_PRECONDITIONS(pCf);

    // Use base function to open all the supported children
    Status = DtDf_OpenChildren((DtDf*)pCf, SUPPORTED_CHILDREN, 
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    // Check mandatory children have been loaded (i.e. shortcut is valid)
    DT_ASSERT(pCf->m_pBcTod != NULL);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfTod_OnPeriodicInterval -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtCfTod_OnPeriodicInterval(DtObject* pObject, DtTodTime  Time)
{
    Int  i=0, NumElems=0;
    DtCfTod*  pCf = (DtCfTod*)pObject;

    // Sanity checks
    CF_TOD_DEFAULT_PRECONDITIONS(pCf);
    if (pCf == NULL)
        return;

    // Notify all
    NumElems = DtVector_Size(pCf->m_pOnPeriodicHandlers);
    for (i=0; i<NumElems; i++)
    {
        const DtOnPeriodicIntervalRegData*  pRegData = 
                 (DtOnPeriodicIntervalRegData*)DtVector_At(pCf->m_pOnPeriodicHandlers, i);
        DT_ASSERT(pRegData!=NULL && pRegData->m_OnPeriodicFunc!=NULL);
        if (pRegData==NULL || pRegData->m_OnPeriodicFunc==NULL)
            continue;
        DT_ASSERT(pRegData->m_pObject != NULL);
        pRegData->m_OnPeriodicFunc(pRegData->m_pObject, Time);
    }
    return;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCfTod implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the CfTod stub
#define STUB_CFTOD_DEFAULT_PRECONDITIONS(pStub)      \
    DT_ASSERT(pStub!=NULL && pStub->m_Size>=sizeof(DtIoStubCfTod))

// MACRO to cast a stub object to a DtIoStubCfTod object
#define STUB_TOD     ((DtIoStubCfTod*)pStub)
#define STUB_CF      ((DtCfTod*)STUB_TOD->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtIoStubCfTod_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubCfTod_OnCmdAdjustPpm(const DtIoStubCfTod*,
                                                      const DtIoctlTodCmdAdjustPpmInput*);
static DtStatus  DtIoStubCfTod_OnCmdAdjustTime(const DtIoStubCfTod*,
                                                     const DtIoctlTodCmdAdjustTimeInput*);
static DtStatus  DtIoStubCfTod_OnCmdGetPhaseIncr(const DtIoStubCfTod*,
                                                        DtIoctlTodCmdGetPhaseIncrOutput*);
static DtStatus  DtIoStubCfTod_OnCmdGetProperties(const DtIoStubCfTod*,
                                                       DtIoctlTodCmdGetPropertiesOutput*);
static DtStatus  DtIoStubCfTod_OnCmdGetTime(const DtIoStubCfTod*,
                                                             DtIoctlTodCmdGetTimeOutput*);
static DtStatus  DtIoStubCfTod_OnCmdSetPhaseIncr(const DtIoStubCfTod*, 
                                                   const DtIoctlTodCmdSetPhaseIncrInput*);
static DtStatus  DtIoStubCfTod_OnCmdSetTime(const DtIoStubCfTod*,
                                                        const DtIoctlTodCmdSetTimeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_TOD;

static const DtIoctlProperties  IOSTUB_CF_TOD_IOCTLS[] = 
{
    DT_IOCTL_PROPS_TOD_CMD(
        DtIoStubCfTod_OnCmd, 
        NULL, NULL),
};

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCfTod - Public fuctions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfTod_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubCfTod_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubCfTod));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfTod_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubCfTod*  DtIoStubCfTod_Open(DtDf*  pCf)
{
    DtIoStubCfTod*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    CF_TOD_DEFAULT_PRECONDITIONS(pCf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubCfTod, pCf, NULL, 
                                                              DtIoStubCfTod_Close,
                                                              NULL,  // Use default IOCTL
                                                              IOSTUB_CF_TOD_IOCTLS);
    pStub = (DtIoStubCfTod*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}


// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCfTod - Private fuctions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfTod_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCfTod_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlTodCmdInput*  pInData = NULL;
    DtIoctlTodCmdOutput*  pOutData = NULL;
    
    // Sanity check
    STUB_CFTOD_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_TOD_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_TodCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_TodCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_TOD_CMD_ADJUST_TIME:
        Status = DtIoStubCfTod_OnCmdAdjustTime(STUB_TOD, &pInData->m_AdjustTime);
        break;
    case DT_TOD_CMD_GET_PHASE_INCR:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubCfTod_OnCmdGetPhaseIncr(STUB_TOD, &pOutData->m_GetPhaseIncr);
        break;
    case DT_TOD_CMD_GET_PROPERTIES:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubCfTod_OnCmdGetProperties(STUB_TOD, &pOutData->m_GetProperties);
        break;
    case DT_TOD_CMD_GET_TIME:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubCfTod_OnCmdGetTime(STUB_TOD, &pOutData->m_GetTime);
        break;
    case DT_TOD_CMD_SET_PHASE_INCR:
        Status = DtIoStubCfTod_OnCmdSetPhaseIncr(STUB_TOD, &pInData->m_SetPhaseIncr);
        break;
    case DT_TOD_CMD_SET_TIME:
        Status = DtIoStubCfTod_OnCmdSetTime(STUB_TOD, &pInData->m_SetTime);
        break;
    case DT_TOD_CMD_ADJUST_PPM:
        Status = DtIoStubCfTod_OnCmdAdjustPpm(STUB_TOD, &pInData->m_AdjustPpm);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfTod_OnCmdAdjustPpm -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtIoStubCfTod_OnCmdAdjustPpm(
    const DtIoStubCfTod* pStub,
    const DtIoctlTodCmdAdjustPpmInput* pInData)
{
    STUB_CFTOD_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtCfTod_AdjustPhaseIncr(STUB_CF, pInData->m_ScaledPpm);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfTod_OnCmdAdjustTime -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubCfTod_OnCmdAdjustTime(
    const DtIoStubCfTod*  pStub, 
    const DtIoctlTodCmdAdjustTimeInput*  pInData)
{
    STUB_CFTOD_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtCfTod_AdjustTime(STUB_CF, pInData->m_DeltaNs);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfTod_OnCmdGetPhaseIncr -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCfTod_OnCmdGetPhaseIncr(
    const DtIoStubCfTod*  pStub, 
    DtIoctlTodCmdGetPhaseIncrOutput*  pOutData)
{
    STUB_CFTOD_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    return DtCfTod_GetPhaseIncr(STUB_CF, &pOutData->m_PhaseIncr);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfTod_OnCmdGetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCfTod_OnCmdGetProperties(
    const DtIoStubCfTod*  pStub, 
    DtIoctlTodCmdGetPropertiesOutput*  pOutData)
{
    STUB_CFTOD_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    return DtCfTod_GetProperties(STUB_CF, &pOutData->m_SysClockFreq, 
                                                           &pOutData->m_SysClockAccuracy);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfTod_OnCmdGetTime -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubCfTod_OnCmdGetTime(
    const DtIoStubCfTod*  pStub, 
    DtIoctlTodCmdGetTimeOutput*  pOutData)
{
    STUB_CFTOD_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    return DtCfTod_GetTime(STUB_CF, &pOutData->m_Time, &pOutData->m_AdjustmentCount);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfTod_OnCmdSetPhaseIncr -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCfTod_OnCmdSetPhaseIncr(    
    const DtIoStubCfTod*  pStub, 
    const DtIoctlTodCmdSetPhaseIncrInput*  pInData)
{
    STUB_CFTOD_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtCfTod_SetPhaseIncr(STUB_CF, pInData->m_PhaseIncr);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfTod_OnCmdSetTime -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubCfTod_OnCmdSetTime(    
    const DtIoStubCfTod*  pStub, 
    const DtIoctlTodCmdSetTimeInput*  pInData)
{
    STUB_CFTOD_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtCfTod_SetTime(STUB_CF, pInData->m_Time);
}
