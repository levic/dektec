//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCfInt.c *#*#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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
#include "DtCfInt.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfInt implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtCfInt function
#define CF_INT_DEFAULT_PRECONDITIONS(pCf)      \
    DT_ASSERT(pCf!=NULL && pCf->m_Size==sizeof(DtCfInt))

// Helper macro to cast a DtDf* to a DtCfInt*
#define CF_INT      ((DtCfInt*)pCf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtCfInt_Init(DtDf*);
DtStatus  DtCfInt_OpenChildren(DtCfInt*);
DtStatus  DtCfInt_OnHandleInterrupt(Int  Vector, Int  Id, Int  Index, void*  pContext);


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfInt - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfInt_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtCfInt_Close(DtDf*  pCf)
{
    // Sanity checks
    CF_INT_DEFAULT_PRECONDITIONS(pCf);

    if (CF_INT->m_pIntHandlers != NULL)
    {
        DtVector_Cleanup(CF_INT->m_pIntHandlers);
        CF_INT->m_pIntHandlers = NULL;
    }

    // Let base function perform final clean-up
    DtDf_Close(pCf);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfInt_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtCfInt*  DtCfInt_Open(DtCore*  pCore, const char*  pRole, Int  Instance, 
                                                              Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    
    // Init open parameters
    DT_CF_INT_INIT_ID(Id, pRole, Instance, Uuid);
    DT_CF_INIT_OPEN_PARAMS(OpenParams, DtCfInt, Id, DT_FUNC_TYPE_CF_INT,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtCfInt_Close;
    OpenParams.m_InitFunc = DtCfInt_Init;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtCfInt*)DtDf_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfInt_OnInterrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtCfInt_OnInterrupt(DtCfInt*  pCf)
{
    // Sanity checks
    CF_INT_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pCf->m_pBcMsix != NULL);
    // Let the MISX-BC do the heavy lifting
    return DtBcMSIX_OnInterrupt(pCf->m_pBcMsix);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfInt_HandlerRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfInt_HandlerRegister(DtCfInt*  pCf, const DtBcIntHandlerRegData*  pRegData)
{
    Int  NumInterrupts = 0;
    
    // Sanity checks
    CF_INT_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pCf->m_pIntHandlers != NULL);

    // Check for out-of-bounds
    NumInterrupts = DtVector_Size(pCf->m_pIntHandlers);
    DT_ASSERT(pRegData != NULL);
    DT_ASSERT(pRegData->m_Index>=0 && pRegData->m_Index<NumInterrupts);
    if (pRegData->m_Index<0 || pRegData->m_Index>=NumInterrupts)
    {
        DtDbgOutCf(ERR, INT, pCf, "ERROR: Invalid index '%d' (#int=%d)", 
                                                        pRegData->m_Index, NumInterrupts);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Update registration data
    DT_ASSERT(pRegData->m_Id>0 && pRegData->m_Id<=65535);
    DT_ASSERT(pRegData->m_IntHandlerFunc != NULL);
    DT_ASSERT(pRegData->m_pBc != NULL);
    return DtVector_Set(pCf->m_pIntHandlers, pRegData->m_Index, pRegData);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfInt_HandlerUnregister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfInt_HandlerUnregister(DtCfInt*  pCf, Int  Index, const DtBc*  pBc)
{
    Int  NumInterrupts = 0;
    DtBcIntHandlerRegData*  pData = NULL;
    
    // Sanity checks
    CF_INT_DEFAULT_PRECONDITIONS(pCf);
    DT_ASSERT(pCf->m_pIntHandlers != NULL);

    // Check for out-of-bounds
    NumInterrupts = DtVector_Size(pCf->m_pIntHandlers);
    DT_ASSERT(Index>=0 && Index<NumInterrupts);
    if (Index<0 || Index>=NumInterrupts)
    {
        DtDbgOutCf(ERR, INT, pCf, "ERROR: Invalid index '%d' (#int=%d)", 
                                                                    Index, NumInterrupts);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Update callback data
    pData = (DtBcIntHandlerRegData*)DtVector_At(pCf->m_pIntHandlers, Index);
    // Not registered?
    if (pData->m_IntHandlerFunc==NULL && pData->m_pBc==NULL)
    {
        DtDbgOutCf(AVG, INT, pCf, "Interrupt handler not registered");
        return DT_STATUS_NOT_FOUND;
    }
    // Check if BC is valid
    if(pData->m_pBc != pBc)
    {
        DtDbgOutCf(ERR, INT, pCf, 
                              "ERROR: '%s' unregisters int-handler for other block '%s'",
                              (pBc!=NULL) ? pBc->m_Id.m_pName : "NULL",
                              (pData->m_pBc!=NULL) ? pData->m_pBc->m_Id.m_pName : "NULL");
        return DT_STATUS_FAIL;
    }
    pData->m_IntHandlerFunc = NULL;
    pData->m_Id = 0;
    pData->m_pContext = NULL;
    pData->m_pBc = NULL;
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfInt - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfInt_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfInt_Init(DtDf*  pCf)
{
    Int  i=0;
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    CF_INT_DEFAULT_PRECONDITIONS(pCf);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    Status = DtCfInt_OpenChildren(CF_INT);
    if (!DT_SUCCESS(Status))
    {   
        DtDbgOutCf(ERR, INT, pCf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    //.-.-.-.-.-.-.-.-.-.-.- Create the interrupt registration list -.-.-.-.-.-.-.-.-.-.-.

    DT_ASSERT(CF_INT->m_pIntHandlers == NULL);
    CF_INT->m_pIntHandlers = DtVector_Create(1, sizeof(DtBcIntHandlerRegData), 1);
    if (CF_INT->m_pIntHandlers == NULL)
    {
        DtDbgOutCf(ERR, INT, pCf, "ERROR: failed to create int-handler list");
        return DT_STATUS_OUT_OF_MEMORY;
    }

    // Fill the list
    DT_ASSERT(CF_INT->m_pBcMsix != NULL);
    for (i=0; i<CF_INT->m_pBcMsix->m_ConfNumInterrupts; i++)
    {
        DtBcIntHandlerRegData  RegData;
        DtMemZero(&RegData, sizeof(RegData));
        RegData.m_Index = i;
        Status = DtVector_PushBack(CF_INT->m_pIntHandlers, &RegData);
        DT_ASSERT(DT_SUCCESS(Status));
    }

    // Register the interrupt handler function
    Status = DtBcMSIX_HandlerRegister(CF_INT->m_pBcMsix, 
                                                       DtCfInt_OnHandleInterrupt, CF_INT);
    DT_ASSERT(DT_SUCCESS(Status));
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfInt_OnHandleInterrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfInt_OnHandleInterrupt(Int  Vector, Int  Id, Int  Index, void*  pContext)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  NumInterrupts = 0;
    DtCfInt*  pCf = (DtCfInt*)pContext;
    const DtBcIntHandlerRegData*  pRegData = NULL;

    // Sanity checks
    CF_INT_DEFAULT_PRECONDITIONS(pCf);

    NumInterrupts = DtVector_Size(pCf->m_pIntHandlers);

    // Check for valid ID
    if (Id == 0)
    {
        DtDbgOutCf(MAX, INT, pCf, "Interrupt: id=0x%04X, index=%d", Id, Index);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check for valid index
    if(Index<0 || Index>=NumInterrupts)
        return DT_STATUS_INVALID_PARAMETER;

    // Get handler for this specific interrupt
    pRegData = (DtBcIntHandlerRegData*)DtVector_At(pCf->m_pIntHandlers, Index);
    if (pRegData->m_IntHandlerFunc == NULL)
        return DT_STATUS_FAIL;      // No handler
    // Call the registered handler
    DT_ASSERT(pRegData->m_Id==Id && pRegData->m_pBc!=NULL);
    Status = pRegData->m_IntHandlerFunc(pRegData->m_pBc, Id, Index, pRegData->m_pContext);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfInt_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfInt_OpenChildren(DtCfInt*  pCf)
{
    DtStatus  Status = DT_STATUS_OK;
    
    // List of children supported by the the interrupt function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_MSIX, DT_BC_MSIX_NAME, NULL, 
                                               (DtObjectBcOrDf**)(&pCf->m_pBcMsix), TRUE},
    };

    // Sanity checks
    CF_INT_DEFAULT_PRECONDITIONS(pCf);

    // Use base function open all the supported children
    Status = DtDf_OpenChildren((DtDf*)pCf, SUPPORTED_CHILDREN, 
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    // Check mandatory children have been loaded (i.e. shortcut is valid)
    DT_ASSERT(pCf->m_pBcMsix != NULL);
    return DT_STATUS_OK;
}