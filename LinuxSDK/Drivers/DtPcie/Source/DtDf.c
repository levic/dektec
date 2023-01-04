//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDf.c *#*#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
#define DT_DF_ADD_FUNCTION_INCLUDES
#include "DtPt.h"
#include "DtDf.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDf implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Df
#define DF_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size>=sizeof(DtDf))


#ifdef DT_DF_ADD_FUNCTION_INCLUDES

// List of all functions we know. NOTE: Role, Instance and UUID are not used
const DtDfId  DT_DF_KNOWN[] = 
{
    // Name,                      ShortName,                      Role, Instance, Uuid
    { DT_DF_ASIRX_NAME,           DT_DF_ASIRX_SHORTNAME,          NULL, -1, -1 },
    { DT_DF_GENLOCKCTRL_NAME,     DT_DF_GENLOCKCTRL_SHORTNAME,    NULL, -1, -1 },
    { DT_DF_CLKGEN_2110_NAME,     DT_DF_CLKGEN_2110_SHORTNAME,    NULL, -1, -1 },
    { DT_DF_NW_NAME,              DT_DF_NW_SHORTNAME,             NULL,  2, -1 },
    { DT_DF_IPFIFO_NAME,          DT_DF_IPFIFO_SHORTNAME,         NULL, -1, -1 },
    { DT_DF_DATAFIFO_NAME,        DT_DF_DATAFIFO_SHORTNAME,       NULL, -1, -1 },
    { DT_DF_MXDS75TEMP_NAME,      DT_DF_MXDS75TEMP_SHORTNAME,     NULL, -1, -1 },
    { DT_DF_SDITXPHY_NAME,        DT_DF_SDITXPHY_SHORTNAME,       NULL, -1, -1 },
    { DT_DF_SDIRX_NAME,           DT_DF_SDIRX_SHORTNAME,          NULL, -1, -1 },
    { DT_DF_CHSDIRX_NAME,         DT_DF_CHSDIRX_SHORTNAME,        NULL, -1, -1 },
    { DT_DF_CHSDIRXPHYONLY_NAME,  DT_DF_CHSDIRXPHYONLY_SHORTNAME, NULL, -1, -1 },
    { DT_DF_SDIXCFGMGR_NAME,      DT_DF_SDIXCFGMGR_SHORTNAME,     NULL, -1, -1 },
    { DT_DF_SENSTEMP_NAME,        DT_DF_SENSTEMP_SHORTNAME,       NULL, -1, -1 },
    { DT_DF_SI534X_NAME,          DT_DF_SI534X_SHORTNAME,         NULL, -1, -1 },
    { DT_DF_SPIPROM_NAME,         DT_DF_SPIPROM_SHORTNAME,        NULL, -1, -1 },
    { DT_DF_SPICABLEDRVEQ_NAME,   DT_DF_SPICABLEDRVEQ_SHORTNAME,  NULL, -1, -1 },
    { DT_DF_TEMPFANMGR_NAME,      DT_DF_TEMPFANMGR_SHORTNAME,     NULL, -1, -1 },
    { DT_DF_TODCLOCKCTRL_NAME,    DT_DF_TODCLOCKCTRL_SHORTNAME,   NULL, -1, -1 },
    { DT_DF_TXPLLMGR_NAME,        DT_DF_TXPLLMGR_SHORTNAME,       NULL, -1, -1 },
    { DT_DF_VIRTGENREF_NAME,      DT_DF_VIRTGENREF_SHORTNAME,     NULL, -1, -1 },
    { DT_DF_VPD_NAME,             DT_DF_VPD_SHORTNAME,            NULL, -1, -1 },
    { DT_DF_S2CRDEMOD_2132_NAME,  DT_DF_S2CRDEMOD_2132_SHORTNAME, NULL, -1, -1 },
    { DT_DF_TXCLKCTRL_2178A_NAME, DT_DF_TXCLKCTRL_2178A_SHORTNAME, NULL, -1, -1 },
    { DT_DF_CLKCTRL_2116_NAME,    DT_DF_CLKCTRL_2116_SHORTNAME,   NULL, -1, -1 },
};
const Int  DT_DF_NUM_KNOWN = DT_SIZEOF_ARRAY(DT_DF_KNOWN);

#endif  // #ifdef DT_DF_ADD_FUNCTION_INCLUDES

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtDf_Enable(DtDf*, Bool  Enable);
DtStatus  DtDf_OpenChild(DtDf*, const DtDfChildProps*, DtObjectBcOrDf**);
DtStatus  DtDf_OpenChildrenOfBcOrDfType(DtDf*, Int, Int, DtVectorBc* , DtVectorDf*);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDf- Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_AddBc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Adds a BC to the function's child BC-list
//
DtStatus  DtDf_AddBc(DtDf*  pDf, DtBc*  pChildBc)
{
    // Add BC to list
    DF_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pChildBc!=NULL && pChildBc->m_Size>=sizeof(DtBc));
    return  DtVectorBc_PushBack(pDf->m_pBcList, pChildBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_AddBc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Adds a DF to the function's child DF-list
//
DtStatus  DtDf_AddDf(DtDf*  pDf, DtDf*  pChildDf)
{
    // Add block to list
    DF_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pChildDf!=NULL && pChildDf->m_Size>=sizeof(DtDf));
    return  DtVectorDf_PushBack(pDf->m_pDfList, pChildDf);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDf_Close(DtDf*  pDf)
{
    if (pDf == NULL)
        return;     // Nothing to close

    DtDbgOutDf(MIN, COMMON, pDf, "Closing DF");

    // Close stub
    if (pDf->m_pIoStub != NULL)
        DtIoStubDf_Close((DtIoStub*)pDf->m_pIoStub);

    // Close any children
    DtDf_CloseChildren(pDf);

    // Cleanup BC and DF lists
    if (pDf->m_pBcList != NULL)
        DtVectorBc_Cleanup(pDf->m_pBcList);
    if (pDf->m_pDfList != NULL)
        DtVectorDf_Cleanup(pDf->m_pDfList);


    // Free function data
    DtObject_Close((DtObject*)pDf, DF_TAG);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_ExclAccessAcquire -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Acquire exclusive access to the DF
//
DtStatus  DtDf_ExclAccessAcquire(DtDf*  pDf, const DtExclAccessObject*  pObject)
{
    DtStatus  Status=DT_STATUS_OK;
    DF_DEFAULT_PRECONDITIONS(pDf);

    // Acquire exclusive access state lock
    if(DtMutexAcquire(&pDf->m_ExclAccessLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;

    // Check if someone else already has access 
    if (!pDf->m_ExclAccess)
    {
        DT_ASSERT(pObject != NULL);
        pDf->m_ExclAccess = TRUE;
        pDf->m_ExclAccessOwner = *pObject;
    }
    else
        Status = DT_STATUS_IN_USE;

    DtMutexRelease(&pDf->m_ExclAccessLock);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_ExclAccessCheck -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Check if we (pObject) have exclusive access
//
DtStatus  DtDf_ExclAccessCheck(DtDf*  pDf, const DtExclAccessObject*  pObject)
{
    DtStatus  Status=DT_STATUS_OK;
    DF_DEFAULT_PRECONDITIONS(pDf);

    // Acquire exclusive access state lock
    if(DtMutexAcquire(&pDf->m_ExclAccessLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;

    // Does someone has exclusive access. If yes check if it is us
    if(pDf->m_ExclAccess)
    {
        if (!DtExclAccessObject_Compare(&pDf->m_ExclAccessOwner, pObject))
            Status = DT_STATUS_IN_USE;  // Someone else has access
    }
    else
        Status = DT_STATUS_EXCL_ACCESS_REQD;  // No one has access yet => must acquire it

    DtMutexRelease(&pDf->m_ExclAccessLock);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_ExclAccessProbe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Probe if someone has exclusive access
//
DtStatus  DtDf_ExclAccessProbe(DtDf*  pDf)
{
    DtStatus  Status=DT_STATUS_OK;
    DF_DEFAULT_PRECONDITIONS(pDf);

    if (DtMutexAcquire(&pDf->m_ExclAccessLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;

    if (pDf->m_ExclAccess)
        Status = DT_STATUS_IN_USE;

    DtMutexRelease(&pDf->m_ExclAccessLock);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_ExclAccessRelease -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Release the block access lock
//
DtStatus  DtDf_ExclAccessRelease(DtDf*  pDf, const DtExclAccessObject*  pObject)
{
    DtStatus  Status=DT_STATUS_OK;
    DF_DEFAULT_PRECONDITIONS(pDf);

    // Acquire exclusive access state lock
    if(DtMutexAcquire(&pDf->m_ExclAccessLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;

    // Does someone has exclusive access. If yes check if it is us
    if(pDf->m_ExclAccess)
    {
        if(DtExclAccessObject_Compare(&pDf->m_ExclAccessOwner, pObject))
            pDf->m_ExclAccess = FALSE;
        else
            Status = DT_STATUS_IN_USE;  // Someone else has access
    }

    DtMutexRelease(&pDf->m_ExclAccessLock);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDf_OnCloseFile(DtDf*  pDf, const DtFileObject*  pFile)
{
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    DF_DEFAULT_PRECONDITIONS(pDf);


    DtDbgOutDf(MAX, COMMON, pDf, "Releasing resources held by File");

    // Release exclusive access lock the File may be holding
    DtDf_ExclAccessRelease(pDf, &ExclAccessObj);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Load driver parameters from property store
//
DtStatus  DtDf_LoadParameters(const DtDf*  pDf, Int  NumPars, DtDfParameters*  pPars)
{
    Int  i=0;
    DtStatus  Status = DT_STATUS_OK;
    DtCore*  pCore = NULL;
    Int  PortIndex = -1;
    
    DF_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT((NumPars==0) || (NumPars>0 && pPars!=NULL));
    DT_ASSERT(pDf->m_pCore != NULL);
    
    pCore = pDf->m_pCore;
    
    if (NumPars == 0)
        return DT_STATUS_OK;

    // Determine portindex
    PortIndex = (pDf->m_pPt!=NULL) ?  pDf->m_pPt->m_PortIndex : -1;

    for (i=0; i<NumPars; i++)
    {
        char  PropName[PROPERTY_NAME_MAX_SIZE];

        DT_ASSERT(pPars[i].m_pValue != NULL);

        // Generate property name for parameter
        Status = DtDf_ToPropertyName(pDf, pPars[i].m_Name, PropName,
                                                               DT_SIZEOF_ARRAY(PropName));
        DT_ASSERT(DT_SUCCESS(Status));

        //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Get the property -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

        switch(pPars[i].m_ValueType)
        {
        case PROPERTY_VALUE_TYPE_BOOL:
            *((Bool*)(pPars[i].m_pValue)) = DtCore_PROPS_GetBool(
                                                           pCore, PropName,
                                                           PortIndex,
                                                           *((Bool*)(pPars[i].m_pValue)));
            break;
        
        case PROPERTY_VALUE_TYPE_INT:
            *((Int*)(pPars[i].m_pValue)) = DtCore_PROPS_GetInt(pCore, PropName,
                                                            PortIndex,
                                                            *((Int*)(pPars[i].m_pValue)));
            break;

        case PROPERTY_VALUE_TYPE_INT8:
            *((Int8*)(pPars[i].m_pValue)) = DtCore_PROPS_GetInt8(
                                                           pCore, PropName,
                                                           PortIndex,
                                                           *((Int8*)(pPars[i].m_pValue)));
            break;

        case PROPERTY_VALUE_TYPE_INT16:
            *((Int16*)(pPars[i].m_pValue)) = DtCore_PROPS_GetInt16(
                                                          pCore, PropName,
                                                          PortIndex,
                                                          *((Int16*)(pPars[i].m_pValue)));
            break;

        case PROPERTY_VALUE_TYPE_INT32:
            *((Int32*)(pPars[i].m_pValue)) = DtCore_PROPS_GetInt32(
                                                          pCore, PropName,
                                                          PortIndex,
                                                          *((Int32*)(pPars[i].m_pValue)));
            break;

        case PROPERTY_VALUE_TYPE_INT64:
            *((Int64*)(pPars[i].m_pValue)) = DtCore_PROPS_GetInt64(
                                                          pCore, PropName,
                                                          PortIndex,
                                                          *((Int64*)(pPars[i].m_pValue)));
            break;

        case PROPERTY_VALUE_TYPE_UINT:
            *((UInt*)(pPars[i].m_pValue)) = DtCore_PROPS_GetUInt(
                                                           pCore, PropName,
                                                           PortIndex,
                                                           *((UInt*)(pPars[i].m_pValue)));
            break;

        case PROPERTY_VALUE_TYPE_UINT8:
            *((UInt8*)(pPars[i].m_pValue)) = DtCore_PROPS_GetUInt8(
                                                          pCore, PropName,
                                                          PortIndex,
                                                          *((UInt8*)(pPars[i].m_pValue)));
            break;

        case PROPERTY_VALUE_TYPE_UINT16:
            *((UInt16*)(pPars[i].m_pValue)) = DtCore_PROPS_GetUInt16(
                                                         pCore, PropName,
                                                         PortIndex,
                                                         *((UInt16*)(pPars[i].m_pValue)));
            break;

        case PROPERTY_VALUE_TYPE_UINT32:
            *((UInt32*)(pPars[i].m_pValue)) = DtCore_PROPS_GetUInt32(
                                                         pCore, PropName,
                                                         PortIndex,
                                                         *((UInt32*)(pPars[i].m_pValue)));
            break;

        case PROPERTY_VALUE_TYPE_UINT64:
            *((UInt64*)(pPars[i].m_pValue)) = DtCore_PROPS_GetUInt64(
                                                         pCore, PropName,
                                                         PortIndex,
                                                         *((UInt64*)(pPars[i].m_pValue)));
            break;

        case PROPERTY_VALUE_TYPE_STRING:
        {
            DtPropertyFilterCriteria  Flt;
            const  DtProperty*  pProperty=NULL;
            DT_PROPERTY_FILTER_CRITERIA_INIT(Flt, PortIndex);
            // Find the property
            pProperty = NULL;
            Status = DtCore_PROPS_Find(pCore, PropName, Flt, &pProperty);
            if (DT_SUCCESS(Status))
            { 
                DT_ASSERT(pProperty->m_Type == PROPERTY_VALUE_TYPE_STRING);
                *((const char**)(pPars[i].m_pValue)) = pProperty->m_pStr;
            }
        }
        break;

        default:
            DT_ASSERT(FALSE);
            return  DT_STATUS_INVALID_PARAMETER;
        }
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDf*  DtDf_Open(const DtDfOpenParams*  pParams)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDf*  pDf = NULL;

    DT_ASSERT(pParams != NULL);
    DT_ASSERT(pParams->m_Size >= sizeof(DtDf));
    DT_ASSERT(pParams->m_pCore!=NULL && pParams->m_pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(pParams->m_CloseFunc != NULL);

    //.-.-.-.-.-.-.-.-.-.-.- Step 1: Allocate memory for DF object -.-.-.-.-.-.-.-.-.-.-.-

    pDf = (DtDf*)DtObject_Open(DT_OBJECT_TYPE_DF, pParams->m_Size, DF_TAG);
    if (pDf == NULL)
        return NULL;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 2: Init DF attributes -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    
    // Init exclusive access mutex
    DtMutexInit(&pDf->m_ExclAccessLock);
    pDf->m_ExclAccess = FALSE;

    // Set name, role, type, UUID and IsCf
    DT_ASSERT(pParams->m_Id.m_pName!=NULL && pParams->m_Id.m_pShortName!=NULL);
    DT_ASSERT(pParams->m_Id.m_Instance > 0);
    DT_ASSERT(pParams->m_Id.m_Uuid >= 0);
    pDf->m_Id = pParams->m_Id;
    pDf->m_Type = pParams->m_Type;
    pDf->m_IsCf = pParams->m_IsCf;
    
    // Init instance-ID string.
    // NOTE: CFs are special, as they use the instance ID of THE CORE-function (DF_CORE#1)
    Status = DtDf_ToInstanceIdFromStrings(
                                   !pDf->m_IsCf ? pDf->m_Id.m_pShortName : "CORE",
                                   !pDf->m_IsCf ? pDf->m_Id.m_Instance : 1,
                                   pDf->m_InstanceId, DT_SIZEOF_ARRAY(pDf->m_InstanceId));
    DT_ASSERT(DT_SUCCESS(Status));
    

    // Set shortcut to core services data
    pDf->m_pCore = pParams->m_pCore;
    // Set port
    pDf->m_pPt = pParams->m_pPt;

    // Init child BC list. Set initial capacity to 1 and grow size as well
    DT_ASSERT(pDf->m_pBcList == NULL);
    pDf->m_pBcList = DtVectorBc_Create(1, 1);
    DT_ASSERT(pDf->m_pBcList != NULL);

    // Init child DF list. Set initial capacity to 1 and grow size as well
    DT_ASSERT(pDf->m_pDfList == NULL);
    pDf->m_pDfList = DtVectorDf_Create(1, 1);
    DT_ASSERT(pDf->m_pDfList != NULL);
    
    // Register our close function and other callbacks
    pDf->m_CloseFunc = pParams->m_CloseFunc;
    pDf->m_InitFunc = pParams->m_InitFunc;
    pDf->m_LoadParsFunc = pParams->m_LoadParsFunc;

    // Did the derived class define it's own Enable callback?
    if (pParams->m_EnableFunc != NULL)
        pDf->m_EnableFunc = pParams->m_EnableFunc;
    else
        pDf->m_EnableFunc = DtDf_Enable;
    // Set pre- and post-children OnEnable callbacks
    pDf->m_OnEnablePreChildrenFunc = pParams->m_OnEnablePreChildrenFunc;
    pDf->m_OnEnablePostChildrenFunc = pParams->m_OnEnablePostChildrenFunc;
    
    // Did the derived class define it's own File-Close callback?
    if (pParams->m_OnCloseFileFunc != NULL)
        pDf->m_OnCloseFileFunc = pParams->m_OnCloseFileFunc;
    else
        pDf->m_OnCloseFileFunc = DtDf_OnCloseFile;      // Use default function

    // Load function parameters (if function has any)
    if (pDf->m_LoadParsFunc != NULL)
    {
        Status = pDf->m_LoadParsFunc(pDf);
        if (!DT_SUCCESS(Status))
            DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed to load parameters");
    }

    // Perform initilisation (if an init function exists)
    if (pDf->m_InitFunc != NULL)
    {
        Status = pDf->m_InitFunc(pDf);
        if (!DT_SUCCESS(Status))
        {
            DT_ASSERT(pDf->m_CloseFunc != NULL);
            pDf->m_CloseFunc(pDf);
            return NULL;
        }
    }
    
    // Finally: open an io-stub for the DF
    if (pParams->m_CreateStub)
    {
        pDf->m_pIoStub = DtIoStubDf_OpenType(pDf);
        DT_ASSERT(pDf->m_pIoStub != NULL);
    }

    // Set the state to initialised
    Status = DtDf_SetOpState(pDf, DF_STATE_INITIALISED);
    DT_ASSERT(DT_SUCCESS(Status));
    
    return pDf; 
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDf_OpenChildren(DtDf*  pDf, const DtDfSupportedChild*  pSupported,
                                                                        Int  NumSupported)
{
    Int  i=0;
    DtStatus  Status = DT_STATUS_OK;
    DtCore*  pCore = NULL;
    DtVector*  pPropListChildren = NULL;
    Bool IsParentCore = (pDf!=NULL && (pDf->m_Id.m_Uuid&DT_UUID_CORE_FLAG)!=0);
    
    DF_DEFAULT_PRECONDITIONS(pDf);
    pCore = pDf->m_pCore;
    CORE_DEFAULT_PRECONDITIONS(pCore);

    //.-.-.-.-.-.- Step 1: load properties for all of the functions children -.-.-.-.-.-.-

    // Allocate vector to receive properties
    pPropListChildren = DtVector_Create(1, sizeof(DtDfChildProps), 1);
    if (pPropListChildren == NULL)
    {
        DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed create vector for child properties");
        return DT_STATUS_OUT_OF_MEMORY;
    }
    // Now get the properties
    Status = DtDf_LoadChildPropsAll(pDf, pPropListChildren);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed to load child properties");
        DtVector_Cleanup(pPropListChildren);
        return Status;
    }

    // If necessary set the Core flag of children
    for (i=0; i<DtVector_Size(pPropListChildren) && IsParentCore; i++)
    {
        DtDfChildProps*  pChildProps = DtVector_At(pPropListChildren, i);
        if (pChildProps->m_ObjectType==DT_OBJECT_TYPE_BC)
            pChildProps->m_Props.m_Bc.m_Uuid |= DT_UUID_CORE_FLAG;
        else if (pChildProps->m_ObjectType == DT_OBJECT_TYPE_DF)
            pChildProps->m_Props.m_Df.m_Uuid |= DT_UUID_CORE_FLAG;
    }

    //-.-.-.-.-.-.-.-.-.-.-.- Step 2: load the supported children -.-.-.-.-.-.-.-.-.-.-.-.

    for (i=0; i<NumSupported; i++)
    {
        Int n=0;
        Bool FoundChild = FALSE;
        const DtDfSupportedChild*  pSuppChild = &(pSupported[i]);
        Bool MultiInst = FALSE;
        char MultiInstRole[255];
        Int MultiInstRoleLength = 0;
        Int NumInst = 0;

        DT_ASSERT(pSuppChild->m_pName != NULL);

        if (pSuppChild->m_pRole != NULL)
        {
            // Check if we have multiple instances with same rol name. The role ends
            // with a '*'.
            MultiInstRoleLength = DtAnsiCharArrayStrLength(pSuppChild->m_pRole, 255);
            if (pSuppChild->m_pRole[MultiInstRoleLength - 1] == '*')
            {
                MultiInst = TRUE;
                DtMemCopy(MultiInstRole, pSuppChild->m_pRole, MultiInstRoleLength);
                MultiInstRole[MultiInstRoleLength - 1] = '\0';
            }
        }

        // Check we have the properties for this child
        for (n=0; n<DtVector_Size(pPropListChildren) && !FoundChild; n++)
        {
            const char*  pRole = NULL;
            const DtDfChildProps*  pChildProps = DtVector_At(pPropListChildren, n);
            UInt InstanceId = 0;
            
            if (pChildProps->m_ObjectType != pSuppChild->m_ObjectType)
                continue;
            // Check which type of BC/DF this is
            if (pChildProps->m_ObjectType==DT_OBJECT_TYPE_BC)
            {
                const DtBcCommonProps*  pBcProps = &pChildProps->m_Props.m_Bc;
                if (pBcProps->m_Type != (DtBcType)pSuppChild->m_Type)
                    continue;
                pRole = pBcProps->m_pRole;
                
            }
            else if (pChildProps->m_ObjectType == DT_OBJECT_TYPE_DF)
            {
                const DtDfCommonProps*  pDfProps = &pChildProps->m_Props.m_Df;
                if (pDfProps->m_Type != (DtFunctionType)pSuppChild->m_Type)
                    continue;
                pRole = pDfProps->m_pRole;
            }
            else
            {
                DT_ASSERT(FALSE);
                continue;
            }

            // Check the role matches
            if (MultiInst && pRole!=NULL)
            {
                Int StrLength;
                if (!DtAnsiCharArrayStartsWith(pRole, MultiInstRole))
                    continue;
                StrLength = DtAnsiCharArrayStrLength(pRole + MultiInstRoleLength - 1, 4);
                InstanceId = DtAnsiCharArrayToUInt(pRole + MultiInstRoleLength - 1, StrLength, 10);
                if (InstanceId >= pSuppChild->m_MaxInstance)
                {
                    DtDbgOutDf(ERR, COMMON, pDf, "ERROR: [InstanceId:%i >= %i] child '%s#%d' role=%s",
                        InstanceId, pSuppChild->m_MaxInstance, pChildProps->m_ShortName, pChildProps->m_Instance, pRole);
                    DtVector_Cleanup(pPropListChildren);
                    return Status;
                }
            } else if (!DtAnsiCharArrayIsEqual(pRole, pSuppChild->m_pRole))
                continue;

            // Found the child => open it
            Status = DtDf_OpenChild(pDf, pChildProps, &pSuppChild->m_ppChild[InstanceId]);
            if (!DT_SUCCESS(Status))
            {
                // Donot forget to cleanup the child property list
                DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed add child '%s#%d'",
                                                                 pChildProps->m_ShortName,
                                                                 pChildProps->m_Instance);
                DtVector_Cleanup(pPropListChildren);

                return Status;
            }
            NumInst++;
            
            // Remove entry for this child from the list
            Status = DtVector_Erase(pPropListChildren, n);
            DT_ASSERT(DT_SUCCESS(Status));

            // For a MultiInst block, we have to continue the list with same supported
            // child
            if (MultiInst)
                n--;
            else
                FoundChild = TRUE;
        }

        // Did we find the child
        if (NumInst == 0)
        {
            DtDbgOutDf(ERR, COMMON, pDf, 
                                    "%s: could not find child '%s' (type=%d, role=%s)",
                                    pSuppChild->m_IsMandatory ? "ERROR" : "WARNING",
                                    pSuppChild->m_pName,
                                    pSuppChild->m_Type,
                                    pSuppChild->m_pRole!=NULL ? pSuppChild->m_pRole : "");
            // Exit with an error only if the child is marked as mandatory
            if (pSuppChild->m_IsMandatory)
                return DT_STATUS_FAIL;
        }
    }

    //.-.-.-.-.-.-.-.-.-.-.- Step 3: Report any left over children -.-.-.-.-.-.-.-.-.-.-.-
    // Leftovers are children which the function did not mark as being supported. 
    // NOTE: for a CF (core-function) this is expected as a CF only supports/uses a
    // subset of the COREs children.

    if (pPropListChildren != NULL)
    {
        for (i=0; i<DtVector_Size(pPropListChildren); i++)
        {
            // Get child properties
            DT_UNUSED const DtDfChildProps*  pChildProps = 
                                                        DtVector_At(pPropListChildren, i);
        
            // Log an unsupported child as an WARNING for normal DF, but for CF this 
            // is expected as a CF implements a part of the CORE function and only
            // uses some of the CORE children
            if (pDf->m_IsCf)
            {
                // For CF log as infomational
                DtDbgOutDf(MIN, COMMON, pDf, "Skipping unused core-child '%s#%d'", 
                                                                 pChildProps->m_ShortName,
                                                                 pChildProps->m_Instance);
            }
            else
            {
                // For normal-DF log as WARNING
                DtDbgOutDf(ERR, COMMON, pDf, "WARNING: found unsupported child '%s#%d'", 
                                                                 pChildProps->m_ShortName,
                                                                 pChildProps->m_Instance);
            }
        }
    }

    // Donot forget to cleanup the child property list
    if (pPropListChildren != NULL)
        DtVector_Cleanup(pPropListChildren);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_OpenChildrenOfBcType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDf_OpenChildrenOfBcType(DtDf*  pDf, DtBcType  BcType, DtVectorBc*  pChildBcs)
{
    return DtDf_OpenChildrenOfBcOrDfType(pDf, DT_OBJECT_TYPE_BC, (Int)BcType, 
                                                                         pChildBcs, NULL);
}
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_OpenChildrenOfDfType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDf_OpenChildrenOfDfType(DtDf*  pDf, DtFunctionType  DfType, 
                                                                   DtVectorDf*  pChildDfs)
{
    return DtDf_OpenChildrenOfBcOrDfType(pDf, DT_OBJECT_TYPE_DF, (Int)DfType, 
                                                                         NULL, pChildDfs);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_OpenChildrenOfBcOrDfType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDf_OpenChildrenOfBcOrDfType(DtDf*  pDf, Int  ObjectType, Int  Type, 
                                           DtVectorBc* pChildBcs,  DtVectorDf*  pChildDfs)
{
    Int  i=0, n=0;
    DtStatus  Status = DT_STATUS_OK;
    DtCore*  pCore = NULL;
    DtVector*  pPropListChildren = NULL;
    Bool IsParentCore = (pDf!=NULL && (pDf->m_Id.m_Uuid&DT_UUID_CORE_FLAG)!=0);
    
    DF_DEFAULT_PRECONDITIONS(pDf);
    pCore = pDf->m_pCore;
    CORE_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT((ObjectType==DT_OBJECT_TYPE_BC && pChildBcs!=NULL)
                                  || (ObjectType==DT_OBJECT_TYPE_DF  && pChildDfs!=NULL));

    // Make the current list empty
    if (ObjectType == DT_OBJECT_TYPE_BC)
        DtVectorBc_Clear(pChildBcs);
    else
        DtVectorDf_Clear(pChildDfs);

    //.-.-.-.-.-.- Step 1: load properties for all of the functions children -.-.-.-.-.-.-

    // Allocate vector to receive properties
    pPropListChildren = DtVector_Create(1, sizeof(DtDfChildProps), 1);
    if (pPropListChildren == NULL)
    {
        DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed create vector for child properties");
        return DT_STATUS_OUT_OF_MEMORY;
    }
    // Now get the properties
    Status = DtDf_LoadChildPropsAll(pDf, pPropListChildren);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed to load child properties");
        DtVector_Cleanup(pPropListChildren);
        return Status;
    }

    // If necessary set the Core flag of children
    for (i=0; i<DtVector_Size(pPropListChildren) && IsParentCore; i++)
    {
        DtDfChildProps*  pChildProps = DtVector_At(pPropListChildren, i);
        if (pChildProps->m_ObjectType==DT_OBJECT_TYPE_BC)
            pChildProps->m_Props.m_Bc.m_Uuid |= DT_UUID_CORE_FLAG;
        else if (pChildProps->m_ObjectType == DT_OBJECT_TYPE_DF)
            pChildProps->m_Props.m_Df.m_Uuid |= DT_UUID_CORE_FLAG;
    }

    //-.-.-.-.-.-.-.-.-.-.-.- Step 2: load the supported children -.-.-.-.-.-.-.-.-.-.-.-.

    // Check we have the properties for this child
    for (n=0; n<DtVector_Size(pPropListChildren); n++)
    {
        DtObjectBcOrDf*  pBcOrDfObject = NULL;
        const char*  pRole = NULL;
        const DtDfChildProps*  pChildProps = DtVector_At(pPropListChildren, n);

        if (pChildProps->m_ObjectType != ObjectType)
            continue;
        // Check which type of BC/DF this is
        if (pChildProps->m_ObjectType==DT_OBJECT_TYPE_BC)
        {
            const DtBcCommonProps*  pBcProps = &pChildProps->m_Props.m_Bc;
            if (pBcProps->m_Type != (DtBcType)Type)
                continue;
            pRole = pBcProps->m_pRole;
                
        }
        else if (pChildProps->m_ObjectType == DT_OBJECT_TYPE_DF)
        {
            const DtDfCommonProps*  pDfProps = &pChildProps->m_Props.m_Df;
            if (pDfProps->m_Type != (DtFunctionType)Type)
                continue;
              pRole = pDfProps->m_pRole;
        }
        else
        {
            DT_ASSERT(FALSE);
            continue;
        }

        // Found the child => open it
        Status = DtDf_OpenChild(pDf, pChildProps, &pBcOrDfObject);
        if (!DT_SUCCESS(Status))
        {
            // Donot forget to cleanup the child property list
            DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed add child '%s#%d'",
                                                                pChildProps->m_ShortName,
                                                                pChildProps->m_Instance);
            DtVector_Cleanup(pPropListChildren);
            return Status;
        }

        // Add the child to our list
        if (pChildProps->m_ObjectType==DT_OBJECT_TYPE_BC)
            Status = DtVectorBc_PushBack(pChildBcs, (DtBc*)pBcOrDfObject);
        else
            Status = DtVectorDf_PushBack(pChildDfs, (DtDf*)pBcOrDfObject);
        DT_ASSERT(DT_SUCCESS(Status));
    }

    // Donot forget to cleanup the child property list
    if (pPropListChildren != NULL)
        DtVector_Cleanup(pPropListChildren);

    return DT_STATUS_OK;
}
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_CloseChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDf_CloseChildren(DtDf*  pDf)
{
    Int  i=0;

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 1: close child DFs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // NOTE: we close them in reverse order as they were loaded

    if (pDf->m_pDfList != NULL)
    {
        Int  NumDf = DtVectorDf_Size(pDf->m_pDfList);
        for (i=(NumDf-1); i>=0; i--)
        {
            DtDf*  pChildDf = DtVectorDf_At(pDf->m_pDfList, i);
            if (pChildDf == NULL)
                continue;
            DtCore_DF_Close(pChildDf);
            DtVectorDf_Set(pDf->m_pDfList, i, NULL);
        }
        DtVectorDf_Clear(pDf->m_pDfList);
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 2: close child BCs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // NOTE: we close them in reverse order as they were loaded

    if (pDf->m_pBcList != NULL)
    {
        Int  NumBc = DtVectorBc_Size(pDf->m_pBcList);
        for (i=(NumBc-1); i>=0; i--)
        {
            DtBc*  pChildBc = DtVectorBc_At(pDf->m_pBcList, i);
            if (pChildBc == NULL)
                continue;
            DtCore_BC_Close(pChildBc);
            DtVectorBc_Set(pDf->m_pBcList, i, NULL);
        }
        DtVectorBc_Clear(pDf->m_pBcList);
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_EnableChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDf_EnableChildren(DtDf*  pDf, Bool  Enable)
{
    Int  i=0, NumBc=0, NumDf=0;
    DtStatus  Status = DT_STATUS_OK;

    DF_DEFAULT_PRECONDITIONS(pDf);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Enable/Disable all Child-DFs -.-.-.-.-.-.-.-.-.-.-.-.-.-

    DT_ASSERT(pDf->m_pDfList != NULL);
    NumDf = DtVectorDf_Size(pDf->m_pDfList);
    for (i=0; i<NumDf; i++)
    {
        DtDf*  pChildDf = DtVectorDf_At(pDf->m_pDfList, i);
        if (pChildDf == NULL)
            continue;
        
        if (pChildDf->m_EnableFunc == NULL)
        {
            DtDbgOutDf(ERR, COMMON, pDf, "ERROR: no enable-callback for '%s' DF",
                                                                  pChildDf->m_InstanceId);
            DT_ASSERT(pDf->m_EnableFunc != NULL);
            continue;
        }
        Status = pChildDf->m_EnableFunc(pChildDf, Enable);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed to enable '%s' DF",
                                                                  pChildDf->m_InstanceId);
            return Status;
        }
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Enable/Disable all Child-BCs -.-.-.-.-.-.-.-.-.-.-.-.-.-

    DT_ASSERT(pDf->m_pBcList != NULL);
    NumBc = DtVectorBc_Size(pDf->m_pBcList);
    for (i=0; i<NumBc; i++)
    {
        DtBc*  pChildBc = DtVectorBc_At(pDf->m_pBcList, i);
        if (pChildBc == NULL)
            continue;
    
        if (pChildBc->m_EnableFunc == NULL)
        {
            DtDbgOutDf(ERR, COMMON, pDf, "ERROR: no enable-callback for '%s' BC",
                                                                  pChildBc->m_InstanceId);
            DT_ASSERT(pChildBc->m_EnableFunc != NULL);
            continue;
        }
        Status = pChildBc->m_EnableFunc(pChildBc, Enable);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed to enable '%s' BC",
                                                                  pChildBc->m_InstanceId);
            return Status;
        }
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_OpenType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDf*  DtDf_OpenType(DtFunctionType  Type, DtCore*  pCore, DtPt*  pPt, 
                                                     const DtDfId*  pId, Bool  CreateStub)
{

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(pId != NULL);

    // NOTE: don't forget to add the driver function to the DT_DF_KNOWN list
    switch(Type)
    {
    case DT_FUNC_TYPE_ASISDIRX:
        break;
    case DT_FUNC_TYPE_ASISDITX:
        break;
    case DT_FUNC_TYPE_ASIRX:
        return (DtDf*)DtDfAsiRx_Open(pCore, pPt, pId->m_pRole, 
                                                pId->m_Instance, pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_CHSDIRX:
        return (DtDf*)DtDfChSdiRx_Open(pCore, pPt, pId->m_pRole, 
                                                pId->m_Instance, pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_CHSDIRXPHYONLY:
        return (DtDf*)DtDfChSdiRxPhyOnly_Open(pCore, pPt, pId->m_pRole, 
                                                pId->m_Instance, pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_DATAFIFO:
        return (DtDf*)DtDfDataFifo_Open(pCore, pPt, pId->m_pRole,
                                                pId->m_Instance, pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_GENLOCKCTRL:
        return (DtDf*)DtDfGenLockCtrl_Open(pCore, pPt, pId->m_pRole, 
                                                pId->m_Instance, pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_IPFIFO:
        return (DtDf*)DtDfIpFifo_Open(pCore, pPt, pId->m_pRole, pId->m_Instance, 
                                                                             pId->m_Uuid);
    case DT_FUNC_TYPE_NW:
         return (DtDf*)DtDfNw_Open(pCore, pPt, pId->m_pRole, pId->m_Instance, 
                                                                 pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_MXDS75TEMP:
        return (DtDf*)DtDfMxDs75Temp_Open(pCore, pPt, pId->m_pRole, 
                                                pId->m_Instance, pId->m_Uuid, CreateStub);

    case DT_FUNC_TYPE_SDITXPHY:
        return (DtDf*)DtDfSdiTxPhy_Open(pCore, pPt, pId->m_pRole, 
                                                pId->m_Instance, pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_SDIRX:
        return (DtDf*)DtDfSdiRx_Open(pCore, pPt, pId->m_pRole, 
                                                pId->m_Instance, pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_SDIXCFGMGR:
        return (DtDf*)DtDfSdiXCfgMgr_Open(pCore, pPt, pId->m_pRole, 
                                                            pId->m_Instance, pId->m_Uuid);
    case DT_FUNC_TYPE_SENSTEMP:
        return (DtDf*)DtDfSensTemp_Open(pCore, pPt, pId->m_pRole, 
                                                pId->m_Instance, pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_SI534X:
        return (DtDf*)DtDfSi534X_Open(pCore, pPt, pId->m_pRole, 
                                                pId->m_Instance, pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_SPICABLEDRVEQ:
        return (DtDf*)DtDfSpiCableDrvEq_Open(pCore, pPt, pId->m_pRole, 
                                                pId->m_Instance, pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_SPIMF:
        break;
    case DT_FUNC_TYPE_SPIPROM:
        return (DtDf*)DtDfSpiProm_Open(pCore, pPt, pId->m_pRole, 
                                                pId->m_Instance, pId->m_Uuid, CreateStub);
     case DT_FUNC_TYPE_TEMPFANMGR:
        return (DtDf*)DtDfTempFanMgr_Open(pCore, pPt, pId->m_pRole, 
                                                pId->m_Instance, pId->m_Uuid, CreateStub);
     case DT_FUNC_TYPE_TODCLKCTRL:
        return (DtDf*)DtDfTodClockCtrl_Open(pCore, pPt, pId->m_pRole, 
                                                pId->m_Instance, pId->m_Uuid, CreateStub);
     case DT_FUNC_TYPE_TXPLLMGR:
        return (DtDf*)DtDfTxPllMgr_Open(pCore, pPt, pId->m_pRole, 
                                                            pId->m_Instance, pId->m_Uuid);
     case DT_FUNC_TYPE_VIRTGENREF:
        return (DtDf*)DtDfVirtGenRef_Open(pCore, pPt, pId->m_pRole, 
                                                     pId->m_Instance, pId->m_Uuid, FALSE);
     case DT_FUNC_TYPE_VPD:
        return (DtDf*)DtDfVpd_Open(pCore, pPt, pId->m_pRole, 
                                                pId->m_Instance, pId->m_Uuid, CreateStub);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Core functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    
    case DT_FUNC_TYPE_CF_EVT:
        return (DtDf*)DtCfEvt_Open(pCore, pId->m_pRole, pId->m_Instance,
                                                                 pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_CF_IOCFG:
        return (DtDf*)DtCfIoCfg_Open(pCore, pId->m_pRole, pId->m_Instance,
                                                                 pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_CF_INT:
        return (DtDf*)DtCfInt_Open(pCore, pId->m_pRole, pId->m_Instance,
                                                                 pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_CF_PROPS:
        return (DtDf*)DtCfProps_Open(pCore, pId->m_pRole, pId->m_Instance,
                                                                 pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_CF_TOD:
        return (DtDf*)DtCfTod_Open(pCore, pId->m_pRole, pId->m_Instance,
                                                                 pId->m_Uuid, CreateStub);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    case DT_FUNC_TYPE_S2CRDEMOD_2132:
        return (DtDf*)DtDfS2CrDemod_2132_Open(pCore, pPt, pId->m_pRole,
                                                pId->m_Instance, pId->m_Uuid, CreateStub);
    case DT_FUNC_TYPE_TXCLKCTRL_2178:
        return (DtDf*)DtDfTxClkCtrl_2178A_Open(pCore, pPt, pId->m_pRole,
                                                            pId->m_Instance, pId->m_Uuid);
    case DT_FUNC_TYPE_CLKCTRL_2116:
        return (DtDf*)DtDfClkCtrl_2116_Open(pCore, pPt, pId->m_pRole,
                                                            pId->m_Instance, pId->m_Uuid);
    case DT_FUNC_TYPE_CLKGEN_2110:
        return (DtDf*)DtDfClkGen_2110_Open(pCore, pPt, pId->m_pRole, pId->m_Instance, 
                                                                             pId->m_Uuid);
    default:
        DT_ASSERT(FALSE);
        break;
    }
    return NULL;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_SetOpState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDf_SetOpState(DtDf*  pDf, DtDfState  NewState)
{

    DT_UNUSED DtDfState  OldState = pDf->m_OpState;
    pDf->m_OpState = NewState;
    DtDbgOutDf(MIN, COMMON, pDf, "State change: %s->%s", DtDf_OpStateToString(OldState),
                                                          DtDf_OpStateToString(NewState));
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_OpStateToString -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
const char*  DtDf_OpStateToString(DtDfState  State)
{
    switch (State)
    {
    case DF_STATE_ENABLED:          return "ENABLED";
    case DF_STATE_INITIALISED:      return "INITIALISED";
    case DF_STATE_UNDEFINED:        return "UNDEFINED";
    }
    return "???";
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_IsEnabled -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtDf_IsEnabled(const DtDf*  pDf)
{
    DF_DEFAULT_PRECONDITIONS(pDf);
    return (pDf->m_OpState == DF_STATE_ENABLED);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_IsInitialised -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtDf_IsInitialised(const DtDf*  pDf)
{
    DF_DEFAULT_PRECONDITIONS(pDf);
    return (pDf->m_OpState == DF_STATE_INITIALISED);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_ToInstanceId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDf_ToInstanceId(const DtDf*  pDf, char*  pInstanceId, Int  MaxLength)
{
    DF_DEFAULT_PRECONDITIONS(pDf);

    // Let the string version do the heavy lifting
    return DtDf_ToInstanceIdFromStrings(pDf->m_Id.m_pShortName, pDf->m_Id.m_Instance, 
                                                                  pInstanceId, MaxLength);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_ToInstanceIdFromStrings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDf_ToInstanceIdFromStrings(const char*  pShortName, Int  Instance,
                                                       char*  pInstanceId, Int  MaxLength)
{
    DtString  TheString;
    DtStatus  Status = DT_STATUS_OK;

    DT_ASSERT(pInstanceId!=NULL && MaxLength>0);
       
    // Allocate string buffer
    Status = DtStringAlloc(&TheString, MaxLength);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DF_COMMON, "ERROR: failed to allocate string-object");
        return Status;
    }

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Construct Instance ID -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    // Format: DF_<short_name>#<instance> (e.g. DF_VPD#1)
    
#define  _CHECK_RESULT(Func)                    \
    do {                                        \
        Status = Func;                          \
        if(!DT_SUCCESS(Status))                 \
        {                                       \
            /* Free string buffer */            \
            DtStringFree(&TheString);           \
            return Status;                      \
        }                                       \
    } while(0)

    // Part 1: add fixed "DF_" prefix
    _CHECK_RESULT(DtStringAppendChars(&TheString, "DF_"));

    // Part 2: add the short-name. NOTE: short-name is expected to be all capitals
    DT_ASSERT(pShortName!=NULL && pShortName[0]!='\0');
    _CHECK_RESULT(DtStringAppendChars(&TheString, pShortName));
    
    // Part 3: add instance number
    DT_ASSERT(Instance > 0);
    _CHECK_RESULT(DtStringAppendChars(&TheString, "#"));
    _CHECK_RESULT(DtStringUIntegerToDtStringAppend(&TheString, 10, Instance));
    
    // convert to a 'normal' character string
    _CHECK_RESULT(DtStringToCharString(&TheString, pInstanceId, MaxLength));

    // Free string buffer
    DtStringFree(&TheString);
#undef  _CHECK_RESULT

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_ToChildInstanceId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDf_ToChildInstanceId(const DtDf*  pDf, Int  ChildInstance,
                                                      char*  pInstanceId, Int  MaxLength)
{
    DF_DEFAULT_PRECONDITIONS(pDf);
    // Let the string version do the heavy lifting
    return DtDf_ToChildInstanceIdFromStrings(pDf->m_InstanceId, ChildInstance,
                                                                  pInstanceId, MaxLength);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_ToChildInstanceIdFromStrings -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDf_ToChildInstanceIdFromStrings(
    const char*  pFuncInstanceId,
    Int  ChildInstance,
    char*  pInstanceId,
    Int  MaxLength)
{
    DtString  TheString;
    DtStatus  Status = DT_STATUS_OK;
    
    DT_ASSERT(pFuncInstanceId!=NULL && pFuncInstanceId[0]!='\0');
    DT_ASSERT(pInstanceId!=NULL && MaxLength>0);
       
    // Allocate string buffer
    Status = DtStringAlloc(&TheString, MaxLength);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DF_COMMON, "ERROR: failed to allocate string-object");
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Construct Child Instance ID -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // Format:<func_instance_id>.<child_instance> (e.g. DF_VPD#1.2)

#define  _CHECK_RESULT(Func)                    \
    do {                                        \
        Status = Func;                          \
        if(!DT_SUCCESS(Status))                 \
        {                                       \
            /* Free string buffer */            \
            DtStringFree(&TheString);           \
            return Status;                      \
        }                                       \
    } while(0)

    // Part 1: Create instance ID
    _CHECK_RESULT(DtStringAppendChars(&TheString, pFuncInstanceId));
    
    // Part 2: add child instance number
    DT_ASSERT(ChildInstance > 0);
    _CHECK_RESULT(DtStringAppendChars(&TheString, "."));
    _CHECK_RESULT(DtStringUIntegerToDtStringAppend(&TheString, 10, ChildInstance));
    
    // convert to a 'normal' character string
    _CHECK_RESULT(DtStringToCharString(&TheString, pInstanceId, MaxLength));

    // Free string buffer
    DtStringFree(&TheString);
#undef  _CHECK_RESULT

    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_ToPropertyName -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDf_ToPropertyName(const DtDf*  pDf, const char*  pProp,
                                                     char*  pFullPropName, Int  MaxLength)
{   
    DF_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pFullPropName!=NULL && MaxLength>0);

    // Let string version do the heavy lifting
    return DtDf_ToPropertyNameFromStrings(pDf->m_InstanceId, pProp, pFullPropName,
                                                                               MaxLength);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_ToPropertyNameFromStrings -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDf_ToPropertyNameFromStrings(
    const char*  pInstanceId, 
    const char*  pProp,
    char*  pFullPropName, 
    Int  MaxLength)
{
    DtString  TheString;
    DtStatus  Status = DT_STATUS_OK;
    
    DT_ASSERT(pInstanceId!=NULL && pInstanceId[0]!='\0');
    DT_ASSERT(pFullPropName!=NULL && MaxLength>0);

    // Allocate string buffer
    Status = DtStringAlloc(&TheString, MaxLength);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DF_COMMON, "ERROR: failed to allocate string-object");
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Construct full property name -.-.-.-.-.-.-.-.-.-.-.-.-.-
    // Format: <df_instance_id>[_<property>] (e.g. DF_VPD#1_RO_SIZE)

#define  _CHECK_RESULT(Func)                    \
    do {                                        \
        Status = Func;                          \
        if(!DT_SUCCESS(Status))                 \
        {                                       \
            /* Free string buffer */            \
            DtStringFree(&TheString);           \
            return Status;                      \
        }                                       \
    } while(0)

    // Part 1: Append instance ID
    _CHECK_RESULT(DtStringAppendChars(&TheString, pInstanceId));

    // Part 2: add the property. 
    // NOTE1: property already is an upper case string
    // NOTE2: can be an empty string
    if (pProp!=NULL && pProp[0] != '\0')
    {
        _CHECK_RESULT(DtStringAppendChars(&TheString, "_"));
        _CHECK_RESULT(DtStringAppendChars(&TheString, pProp));
    }
    
    // Finally: convert to a 'normal' character string
    pFullPropName[0] = '\0';
    Status = DtStringToCharString(&TheString, pFullPropName, MaxLength);

    // Free string buffer
    DtStringFree(&TheString);

#undef _CHECK_RESULT

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_LoadChildProps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDf_LoadChildProps(
                       const DtDf*  pDf, Int  ChildInstance, DtDfChildProps*  pChildProps)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  PortIndex = (pDf->m_pPt!=NULL) ? pDf->m_pPt->m_PortIndex : -1;
    char  Id[DT_BCORDF_INSTANCE_ID_MAX_SIZE];

    // Sanity checks
    DF_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pChildProps != NULL);

    // Generate child-instance-id
    Status = DtDf_ToChildInstanceId(pDf, ChildInstance, Id, DT_SIZEOF_ARRAY(Id));
    if (!DT_SUCCESS(Status))
        return Status;

    // Let the string version do the heavy lifting
    return DtDf_LoadChildPropsFromString(Id, pDf->m_pCore, PortIndex, pChildProps);

}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_LoadChildPropsFromString -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDf_LoadChildPropsFromString(
    const char*  pChildInstanceId,
    DtCore*  pCore,
    Int  PortIndex, 
    DtDfChildProps*  pChildProps)
{
    DtStatus  Status = DT_STATUS_OK;
    DtPropertyFilterCriteria  Flt;
    const DtProperty*  pProp = NULL;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pChildInstanceId!=NULL && pChildInstanceId[0]!='\0');
    DT_ASSERT(pChildProps != NULL);

    DT_PROPERTY_FILTER_CRITERIA_INIT(Flt, PortIndex);
    
    // Find a the child-instance property in the property store 
    Status = DtCore_PROPS_Find(pCore, pChildInstanceId, Flt, &pProp);
    if (!DT_SUCCESS(Status))
        return Status;
    
    DT_ASSERT(pProp != NULL);
    DT_ASSERT(pProp->m_Type == PROPERTY_VALUE_TYPE_STRING);
    DT_ASSERT(pProp->m_pStr != NULL);
    
    // Load the common props for the child. Check type of child
    pChildProps->m_ShortName[0] = '\0';
    if (DtAnsiCharArrayStartsWith(pProp->m_pStr, "BC_"))
    {
        // Mark as BC props
        pChildProps->m_ObjectType = DT_OBJECT_TYPE_BC;
        // Load BC properties 
        Status = DtBcCommonProps_LoadFromString(&(pChildProps->m_Props.m_Bc),
                                                                  pProp->m_pStr, 
                                                                  Flt.m_PortIndex, pCore);
    }
    else if (DtAnsiCharArrayStartsWith(pProp->m_pStr, "DF_"))
    {
        // Mark as DF props
        pChildProps->m_ObjectType = DT_OBJECT_TYPE_DF;
        // Load BC properties 
        Status = DtDfCommonProps_LoadFromString(&(pChildProps->m_Props.m_Df),
                                                                  pProp->m_pStr, 
                                                                  Flt.m_PortIndex, pCore);
    }
    else
    {
        DT_ASSERT(FALSE);
        return DT_STATUS_FAIL;
    }
    
    // Did we succeed to load the properties?
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DF_COMMON, "Failed to load properties for child '%s'",
                                                                           pProp->m_pStr);
        return  Status;
    }

    // Finally: extract the short-name and instance# from the instance-id string
    Status = DtUtilitiesSplitInstanceId(
                           pProp->m_pStr,
                           (pChildProps->m_ObjectType==DT_OBJECT_TYPE_BC) ? "BC_" : "DF_",
                           pChildProps->m_ShortName, 
                           DT_SIZEOF_ARRAY(pChildProps->m_ShortName),
                           &(pChildProps->m_Instance));
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_LoadChildPropsAll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDf_LoadChildPropsAll(const DtDf*  pDf, DtVector*  pPropList)
{
     Int  PortIndex =  -1;
     // Sanity checks
    DF_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pPropList != NULL);

    PortIndex = (pDf->m_pPt!=NULL) ? pDf->m_pPt->m_PortIndex : -1;

    // Let the string version do the heavy lifting
    return DtDf_LoadChildPropsAllFromString(pDf->m_InstanceId, pDf->m_pCore,
                                                                    PortIndex, pPropList);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_LoadChildPropsAllFromString -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDf_LoadChildPropsAllFromString(
    const char*  pFuncInstanceId,
    DtCore*  pCore,
    Int  PortIndex,
    DtVector*  pPropList)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  ChildInstance=0;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pFuncInstanceId!=NULL  && pFuncInstanceId[0]!='\0');
    DT_ASSERT(pPropList != NULL);

    ChildInstance = 1;
    do
    {
        DtDfChildProps  ChildProps;
        char  Id[DT_BCORDF_INSTANCE_ID_MAX_SIZE];
        Status = DtDf_ToChildInstanceIdFromStrings(pFuncInstanceId, ChildInstance++, 
                                                                 Id, DT_SIZEOF_ARRAY(Id));
        if (!DT_SUCCESS(Status))
            return Status;

        Status = DtDf_LoadChildPropsFromString(Id, pCore, PortIndex, &ChildProps);
        if (Status == DT_STATUS_NOT_FOUND_INCOMP_FW)
        { 
            Status = DT_STATUS_OK;
            continue;               // More children but skip this one
        }
        else if (Status == DT_STATUS_NOT_FOUND)
            return DT_STATUS_OK;   // No more children => we are done
        else if (!DT_SUCCESS(Status))
            return Status;

        Status = DtVector_PushBack(pPropList, &ChildProps);
        DT_ASSERT(DT_SUCCESS(Status));
    }  while (DT_SUCCESS(Status));

    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDf - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_Enable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDf_Enable(DtDf*  pDf, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;

    DF_DEFAULT_PRECONDITIONS(pDf);

    // Will the state change??
    if ((pDf->m_OpState==DF_STATE_ENABLED && Enable) 
                                     || (pDf->m_OpState==DF_STATE_INITIALISED && !Enable))
        return DT_STATUS_OK;

    if (pDf->m_OnEnablePreChildrenFunc != NULL)
    {
        Status = pDf->m_OnEnablePreChildrenFunc(pDf, Enable);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, COMMON, pDf, "ERROR: OnEnablePreChildren failed ");
            return Status;
        }
    }

    // Enable/disable all the children
    Status = DtDf_EnableChildren(pDf, Enable);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed to enable/disbale children");
        return Status;
    }

    if (pDf->m_OnEnablePostChildrenFunc != NULL)
    {
        Status = pDf->m_OnEnablePostChildrenFunc(pDf, Enable);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, COMMON, pDf, "ERROR: OnEnablePostChildrenFunc failed ");
            return Status;
        }
    }

    // Set new state
    Status = DtDf_SetOpState(pDf, Enable ? DF_STATE_ENABLED : DF_STATE_INITIALISED);
    return Status;

}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDf_OpenChild -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDf_OpenChild(
    DtDf*  pDf,
    const DtDfChildProps*  pChildProps,
    DtObjectBcOrDf**  ppChild)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pChildProps!=NULL);
    DT_ASSERT(pChildProps->m_ObjectType==DT_OBJECT_TYPE_BC 
                                         || pChildProps->m_ObjectType==DT_OBJECT_TYPE_DF);

    if (pChildProps->m_ObjectType == DT_OBJECT_TYPE_BC)
    {
        const DtBcCommonProps*  pBcProps = &pChildProps->m_Props.m_Bc;

        // Check if the BC already exists (if not, try to open it now)
        DtBc*  pChildBc = DtCore_BC_FindByAddress(pDf->m_pCore, pDf->m_pPt,
                                                                     pBcProps->m_Address);
        if (pChildBc == NULL)
        {
            DtBcId  Id;

            Id.m_pName = NULL;      // Not known here
            Id.m_pShortName = pChildProps->m_ShortName;
            Id.m_pRole = pChildProps->m_Props.m_Bc.m_pRole;
            Id.m_Uuid = pChildProps->m_Props.m_Bc.m_Uuid;
            Id.m_Instance = pChildProps->m_Instance;
         
            pChildBc = DtCore_BC_Open(pDf->m_pCore, pBcProps->m_Type, 
                                                                  pBcProps->m_Address,
                                                                  pDf->m_pPt, &Id,
                                                                  pBcProps->m_CreateStub);
        }
        // Add BC to function BC list
        if (pChildBc != NULL)
        {
            Status = DtDf_AddBc(pDf, pChildBc);
            if (!DT_SUCCESS(Status))
                return Status;

            if (ppChild != NULL)
                *ppChild = (DtObjectBcOrDf*)pChildBc;

            DtDbgOutDf(MIN, COMMON, pDf, "Added child '%s'", pChildBc->m_InstanceId);
        }
        else
        {
            DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed to add child '%s:%d'",
                                            pChildProps->m_ShortName,
                                            (pBcProps->m_Uuid&DT_UUID_INDEX_MASK));
            return DT_STATUS_FAIL;
        }
    }
    else if (pChildProps->m_ObjectType == DT_OBJECT_TYPE_DF)
    {
        const DtDfCommonProps*  pDfProps = &pChildProps->m_Props.m_Df;

        // Check if the DF already exists (if not, try to open it now)
        DtDf*  pChildDf = DtCore_DF_Find(pDf->m_pCore, pDf->m_pPt, 
                                                     pDfProps->m_Type, pDfProps->m_pRole);
        if (pChildDf == NULL)
        {
            DtDfId  Id;

            Id.m_pName = NULL;      // Not known here
            Id.m_pShortName = pChildProps->m_ShortName;
            Id.m_pRole = pChildProps->m_Props.m_Df.m_pRole;
            Id.m_Uuid = pChildProps->m_Props.m_Df.m_Uuid;
            Id.m_Instance = pChildProps->m_Instance;
         
            pChildDf = DtCore_DF_Open(pDf->m_pCore, pDfProps->m_Type, pDf->m_pPt,
                                                             &Id, pDfProps->m_CreateStub);
        }
        // Add DF to function DF list
        if (pChildDf != NULL)
        {
            Status = DtDf_AddDf(pDf, pChildDf);
            if (!DT_SUCCESS(Status))
                return Status;

            if (ppChild != NULL)
                *ppChild = (DtObjectBcOrDf*)pChildDf;

            DtDbgOutDf(MIN, COMMON, pDf, "Added child '%s'", pChildDf->m_InstanceId);
        }
        else
        {
            DtDbgOutDf(ERR, COMMON, pDf, "ERROR: failed to add child '%s:%d'",
                                                   pChildProps->m_ShortName,
                                                   (pDfProps->m_Uuid&DT_UUID_INDEX_MASK));
            return DT_STATUS_FAIL;
        }

    }
    else
    {
        DT_ASSERT(FALSE);
        return DT_STATUS_INVALID_PARAMETER;
    }
        
    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfCommonProps implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCommonProps_Load -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfCommonProps_Load(
    DtDfCommonProps*  pTheProps, 
    const DtDfId*  pId,
    Int  PortIndex, 
    DtCore*  pCore)
{
    DtStatus  Status = DT_STATUS_OK;
    char  Id[PROPERTY_NAME_MAX_SIZE];
    
    DT_ASSERT(pTheProps!=NULL && pCore!=NULL && pId!=NULL);

    // Generate an instance ID (e.g. 'DF_VPD#1')
    Status = DtDf_ToInstanceIdFromStrings(pId->m_pShortName, pId->m_Instance, Id,
                                                                     DT_SIZEOF_ARRAY(Id));
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Let string version do the heavy lifting
    return DtDfCommonProps_LoadFromString(pTheProps, Id, PortIndex, pCore);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfCommonProps_LoadFromString -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfCommonProps_LoadFromString(
    DtDfCommonProps*  pTheProps, 
    const char*  pInstanceId,
    Int  PortIndex, 
    DtCore*  pCore)
{
    DtStatus  Status = DT_STATUS_OK;
    char  PropName[PROPERTY_NAME_MAX_SIZE+1];
    const  DtProperty*  pProperty=NULL;
    DtPropertyFilterCriteria  Flt;

    static const DtFunctionType  DEF_TYPE = DT_FUNC_TYPE_UNKNOWN;
    static const Int  DEF_UUID = -1;

    DT_ASSERT(pInstanceId!=NULL && pInstanceId[0]!='\0');
    DT_ASSERT(pTheProps!=NULL && pCore!=NULL);

    // Init to 'safe' initial values
    pTheProps->m_pRole = NULL;
    pTheProps->m_Type = DEF_TYPE;
    pTheProps->m_Uuid = DEF_UUID;
    pTheProps->m_CreateStub = FALSE;

    DT_PROPERTY_FILTER_CRITERIA_INIT(Flt, PortIndex);
    
    //.-.-.-.-.-.-.-.-.-.-.-.-.-.- Get function-role property -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // Construct property name
    Status = DtDf_ToPropertyNameFromStrings(pInstanceId, NULL, PropName,
                                                               DT_SIZEOF_ARRAY(PropName));
    DT_ASSERT(DT_SUCCESS(Status));

    // Find the property for the role of the functions
    pProperty = NULL;
    Status = DtCore_PROPS_Find(pCore, PropName, Flt, &pProperty);
    if (!DT_SUCCESS(Status))
        return Status;

    DT_ASSERT(pProperty->m_Type == PROPERTY_VALUE_TYPE_STRING);
    pTheProps->m_pRole = pProperty->m_pStr;
    // If the property has a DTAPI scope a stub must be created
    pTheProps->m_CreateStub = TRUE; //(pProperty->m_Scope&PROPERTY_SCOPE_DTAPI)!=0;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.- Get function type property -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // Construct property name
    Status = DtDf_ToPropertyNameFromStrings(pInstanceId, "TYPE", PropName, 
                                                               DT_SIZEOF_ARRAY(PropName));
    DT_ASSERT(DT_SUCCESS(Status));

    pTheProps->m_Type = (DtFunctionType)DtCore_PROPS_GetInt(pCore, PropName, 
                                                                     PortIndex, DEF_TYPE);
    if (pTheProps->m_Type == DEF_TYPE)
    {
        DtDbgOut(ERR, DF_COMMON, "Failed to get function-type property '%s'", PropName);
        return DT_STATUS_FAIL;
    }

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.- Get function UUID property -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // Construct property name
    Status = DtDf_ToPropertyNameFromStrings(pInstanceId, "UUID", PropName, 
                                                               DT_SIZEOF_ARRAY(PropName));
    DT_ASSERT(DT_SUCCESS(Status));

    pTheProps->m_Uuid = DtCore_PROPS_GetInt(pCore, PropName, PortIndex, DEF_UUID);
    if (pTheProps->m_Uuid == DEF_UUID)
    {
        DtDbgOut(ERR, DF_COMMON, "Failed to get function-UUID property '%s'", PropName);
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDf implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO to cast a io-stub-pointer (pStub) to a DtIoStubDf-pointer
#define IOSTUB_DF    ((DtIoStubDf*)pStub)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDf_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubDf_Close(DtIoStub*  pStub)
{
    if (pStub == NULL)
        return;
    DT_ASSERT(pStub->m_Size>=sizeof(DtIoStubDf));

    // Let the base implementation do the heavy lifting 
    DtIoStub_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDf_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubDf*  DtIoStubDf_Open(DtIoStubDfOpenParams*  pParams)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoStubDf*  pStub = NULL;
    const DtDfId* pId = NULL;
    char  Name[DT_IOSTUB_NAME_MAX_SIZE];
    DtStringChar  TempStrBuf[DT_IOSTUB_NAME_MAX_SIZE];
    DtString  TempStr;

    DT_ASSERT(pParams->m_Size >= sizeof(DtIoStubDf));
    DT_ASSERT(pParams->m_pDf!=NULL && pParams->m_pDf->m_Size>=sizeof(DtDf));
    DT_ASSERT(pParams->m_CloseFunc!=NULL);

    pId = &pParams->m_pDf->m_Id;

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 1: Get name -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    // Use the instance ID + UUID index as the friendly-name of the stub ('DF_VPD#1:3')
    DT_STRING_INIT(TempStr, TempStrBuf, DT_SIZEOF_ARRAY(TempStrBuf));
    
    Status = DtStringAppendChars(&TempStr, pParams->m_pDf->m_InstanceId);
    DT_ASSERT(DT_SUCCESS(Status));
    Status = DtStringAppendChars(&TempStr, ":");
    DT_ASSERT(DT_SUCCESS(Status));
    Status = DtStringUIntegerToDtStringAppend(&TempStr, 10, 
                                                          pId->m_Uuid&DT_UUID_INDEX_MASK);
    DT_ASSERT(DT_SUCCESS(Status));
    Status = DtStringToCharString(&TempStr, Name, DT_SIZEOF_ARRAY(Name));
    DT_ASSERT(DT_SUCCESS(Status));

    //-.-.-.-.-.-.-.-.- Step 2: Allocate memory for ioctl stub structure -.-.-.-.-.-.-.-.-

    // Did the derived struct set its own name
    if (pParams->m_pName == NULL)
        pParams->m_pName = Name;
   
    // Let the base implementation do the heavy lifting
    pStub = (DtIoStubDf*)DtIoStub_Open((DtIoStubOpenParams*)pParams);
    if (pStub == NULL)
        return NULL;
    
    //-.-.-.-.-.-.-.-.-.-.-.-.- Step 3: Init DF stub attributes -.-.-.-.-.-.-.-.-.-.-.-.-.
    // Base attributes are already initialised, so only need to init DF specfic ones

    // Set the DF we control
    pStub->m_pDf = pParams->m_pDf;

    // Print stub information
    DT_ASSERT(pId->m_pName != NULL);
    DtDbgOutIoStubDf(MIN, COMMON, pStub, "Created io-control stub for '%s' DF", 
                                                                            pId->m_pName);
    return pStub;
}

#ifdef DT_DF_ADD_FUNCTION_INCLUDES

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDf_OpenType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubDf*  DtIoStubDf_OpenType(DtDf*  pDf)
{
    DF_DEFAULT_PRECONDITIONS(pDf);

    switch(pDf->m_Type)
    {
    case DT_FUNC_TYPE_ASIRX:
        return (DtIoStubDf*)DtIoStubDfAsiRx_Open(pDf);
    case DT_FUNC_TYPE_DATAFIFO:
        return (DtIoStubDf*)DtIoStubDfDataFifo_Open(pDf);
    case DT_FUNC_TYPE_GENLOCKCTRL:
        return (DtIoStubDf*)DtIoStubDfGenLockCtrl_Open(pDf);
    case DT_FUNC_TYPE_NW:
        return (DtIoStubDf*)DtIoStubDfNw_Open(pDf);
    case DT_FUNC_TYPE_CHSDIRXPHYONLY:
        return (DtIoStubDf*)DtIoStubDfChSdiRxPhyOnly_Open(pDf);
    case DT_FUNC_TYPE_SDITXPHY:
        return (DtIoStubDf*)DtIoStubDfSdiTxPhy_Open(pDf);
    case DT_FUNC_TYPE_SDIRX:
        return (DtIoStubDf*)DtIoStubDfSdiRx_Open(pDf);
    case DT_FUNC_TYPE_CHSDIRX:
        return (DtIoStubDf*)DtIoStubDfChSdiRx_Open(pDf);
    case DT_FUNC_TYPE_SPIPROM:
        return (DtIoStubDf*)DtIoStubDfSpiProm_Open(pDf);
    case DT_FUNC_TYPE_SENSTEMP:
        return (DtIoStubDf*)DtIoStubDfSensTemp_Open(pDf);
    case DT_FUNC_TYPE_TEMPFANMGR:
        return (DtIoStubDf*)DtIoStubDfTempFanMgr_Open(pDf);
    case DT_FUNC_TYPE_TODCLKCTRL:
        return (DtIoStubDf*)DtIoStubDfTodClockCtrl_Open(pDf);
    case DT_FUNC_TYPE_VPD:
        return (DtIoStubDf*)DtIoStubDfVpd_Open(pDf);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Core functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    case DT_FUNC_TYPE_CF_EVT:
        return (DtIoStubDf*)DtIoStubCfEvt_Open(pDf);
    case DT_FUNC_TYPE_CF_IOCFG:
        return (DtIoStubDf*)DtIoStubCfIoCfg_Open(pDf);
    case DT_FUNC_TYPE_CF_PROPS:
        return (DtIoStubDf*)DtIoStubCfProps_Open(pDf);
    case DT_FUNC_TYPE_CF_TOD:
        return (DtIoStubDf*)DtIoStubCfTod_Open(pDf);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    case DT_FUNC_TYPE_S2CRDEMOD_2132:
        return (DtIoStubDf*)DtIoStubDfS2CrDemod_2132_Open(pDf);
    
    default:
        DT_ASSERT(FALSE);
        break;
    }
    return NULL;
}

#endif  // #ifdef DT_DF_ADD_FUNCTION_INCLUDES

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDf_OnExclAccessCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDf_OnExclAccessCmd(
    const DtIoStub*  pStub, 
    DtIoStubIoParams*  pIoParams, 
    Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlExclAccessCmdInput*  pInData = NULL;
        
    DT_ASSERT(pStub!=NULL && pStub->m_Size>=sizeof(DtIoStubBc));
    DT_ASSERT(IOSTUB_DF->m_pDf!=NULL);
    DT_ASSERT((IOSTUB_DF->m_pDf->m_pPt==NULL && pIoParams->m_PortIndex==-1)
              || (IOSTUB_DF->m_pDf->m_pPt!=NULL 
                      && IOSTUB_DF->m_pDf->m_pPt->m_PortIndex == pIoParams->m_PortIndex));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_EXCL_ACCESS_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_ExclAccessCmd;

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_EXCLUSIVE_ACCESS_CMD_ACQUIRE:
        Status = DtDf_ExclAccessAcquire(IOSTUB_DF->m_pDf, &pIoParams->m_ExclAccessObj);
        break;
    case DT_EXCLUSIVE_ACCESS_CMD_CHECK:
        Status = DtDf_ExclAccessCheck(IOSTUB_DF->m_pDf, &pIoParams->m_ExclAccessObj);
        break;
    case DT_EXCLUSIVE_ACCESS_CMD_PROBE:
        Status = DtDf_ExclAccessProbe(IOSTUB_DF->m_pDf);
        break;
    case DT_EXCLUSIVE_ACCESS_CMD_RELEASE:
        Status = DtDf_ExclAccessRelease(IOSTUB_DF->m_pDf, &pIoParams->m_ExclAccessObj);
        break;
    default:
        DT_ASSERT(FALSE);
        Status = DT_STATUS_NOT_SUPPORTED;
        break;
    }
    return Status;
}
                                                                                                 
