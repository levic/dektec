//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCore_DF.c *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_DF implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtCore_DF_OpenAll_Impl(DtCore*, DtPt*, const DtDfId*, Int  NumDfs);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_DF - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DF_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Closes a device-level driver-function and removes it to the DF-list
//
DtStatus  DtCore_DF_Close(DtDf*  pDf)
{
    Int  i=0, NumDf=0, Index=0;
    DtCore*  pCore = NULL;
    DtPt*  pPt = NULL;
    DtVectorDf*  pDfList;

    if (pDf == NULL)
        return DT_STATUS_OK;        // Nothing TODO

    // Sanity checks
    pCore = pDf->m_pCore;
    CORE_DEFAULT_PRECONDITIONS(pCore);

    // Find the DF-list
    pPt =  pDf->m_pPt;
    pDfList = (pPt!=NULL) ? pPt->m_pDfList : pCore->m_pDfList;

    // Must have a close function
    if (pDf->m_CloseFunc == NULL)
    {
        DtDbgOut(ERR, CORE, "ERROR: DF '%s' does not have a close function", 
                                                                       pDf->m_InstanceId);
        return DT_STATUS_FAIL;
    }

    // Find the function in the list and remove it
    NumDf = DtVectorDf_Size(pDfList);
    for (i=0; i<NumDf; i++)
    {
        const DtDf*  pDfInList = DtVectorDf_At(pDfList, i);
        if (pDfInList != pDf)
            continue;
        // Erase the entry
        DT_ASSERT(pDfInList!=NULL && pDfInList->m_Id.m_Uuid==pDf->m_Id.m_Uuid);
        DtVectorDf_Erase(pDfList, i);
        break;
    }
    // Clear entry in UUID lookup table
    Index = (Int)(pDf->m_Id.m_Uuid & DT_UUID_INDEX_MASK);
    NumDf = DtVectorDf_Size(pCore->m_pUuidToDf);
    if (Index < NumDf)
    {
        const DtDf*  pDfInList = DtVectorDf_At(pCore->m_pUuidToDf, Index);
        if (pDfInList != NULL)
        {
            // Clear entry
            DT_ASSERT(pDfInList->m_Id.m_Uuid==pDf->m_Id.m_Uuid);
            DtVectorDf_Set(pCore->m_pUuidToDf, Index, NULL);
        }
    }

    // Finally: close the DF
    pDf->m_CloseFunc(pDf);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DF_Find -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDf*  DtCore_DF_Find(DtCore*  pCore, DtPt*  pPt, DtFunctionType  Type, 
                                                                       const char*  pRole)
{
    DtVectorDf*  pDfList;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    // Find the DF-list
    pDfList = (pPt!=NULL) ? pPt->m_pDfList : pCore->m_pDfList;

    if (pDfList == NULL)
        return NULL;

    return DtVectorDf_FindByTypeAndRole(pDfList, Type, pRole);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DF_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Create a device-level driver-function and adds it to the DF-list
//
DtDf*  DtCore_DF_Open(DtCore*  pCore, DtFunctionType  Type, DtPt*  pPt, 
                                                     const DtDfId*  pId, Bool  CreateStub)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDf*  pDf=NULL;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pId != NULL);

    // Open the DF and add it to function list. 
    pDf = DtDf_OpenType(Type, pCore, pPt, pId, CreateStub);
    if (pDf != NULL)
    {
        // Find the DF-list
        DtVectorDf*  pDfList = (pPt!=NULL) ? pPt->m_pDfList : pCore->m_pDfList;

        // Add to DF-list
        Status = DtVectorDf_PushBack(pDfList, pDf);

        // Add non-core DFs to the UUID lookup list
        if (DT_SUCCESS(Status) && (pId->m_Uuid&DT_UUID_CORE_FLAG)==0)
        {
            Int  i=0;
            Int  NumDf = DtVectorDf_Size(pCore->m_pUuidToDf);
            Int  UuidIndex = (pId->m_Uuid & DT_UUID_INDEX_MASK);

            // Grow table if UuidIndex is out of range
            for (i=NumDf; i<=UuidIndex; i++)
            {
                Status = DtVectorDf_PushBack(pCore->m_pUuidToDf, NULL);
                if (!DT_SUCCESS(Status))
                {
                    DtDbgOut(ERR, CORE, "Failed to resize the UUID-to-DF list");
                    break;
                }
            }
            // If table has right size => add DF to lookup list
            if (DT_SUCCESS(Status))
            {
                DT_ASSERT(UuidIndex < DtVectorDf_Size(pCore->m_pUuidToDf));
                Status = DtVectorDf_Set(pCore->m_pUuidToDf, UuidIndex, pDf);
            }
        }
        if (!DT_SUCCESS(Status))
        {
            DT_ASSERT(pDf->m_CloseFunc != NULL);
            pDf->m_CloseFunc(pDf);
            DtDbgOut(ERR, CORE, "Failed to add DF '%s' to DF list", pId->m_pName);
            return NULL;
        }
    }
    return pDf;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DF_OpenAll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_DF_OpenAll(DtCore*  pCore, DtPt*  pPt)
{
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    return DtCore_DF_OpenAll_Impl(pCore, pPt, DT_DF_KNOWN, DT_DF_NUM_KNOWN);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_DF_OpenAll_Impl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCore_DF_OpenAll_Impl(DtCore*  pCore,  DtPt*  pPt, const DtDfId*  pIds,
                                                                              Int  NumIds)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i=0;
    Int  PortIndex = (pPt!=NULL) ? pPt->m_PortIndex : -1;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pIds!=NULL && NumIds>0);
    
    // Loop over all known block IDs and load properties 
    for (i=0; i<NumIds; i++)
    {
        Int  Instance=1;
        Bool  FoundProps = FALSE;
        do
        {
            DtDf*  pDf = NULL;
            DtDfCommonProps  DfProps;
            DtDfId  Id = pIds[i];    // Init ID
            // Are we looking for a specific instance?
            if (Id.m_Instance > 0)
                Instance = Id.m_Instance;
            else
                Id.m_Instance = Instance++;     // Try the next

            //.-.-.-.-.-.-.-.-.-.-.-.- Step 1: load DF properties -.-.-.-.-.-.-.-.-.-.-.-.

            FoundProps = FALSE;     // Assume the worst
            Status = DtDfCommonProps_Load(&DfProps, &Id, PortIndex, pCore);
            if (Status==DT_STATUS_NOT_FOUND_INCOMP_FW)
            {
                FoundProps = TRUE;      // Found but skip this one
                continue;
            }
            else if (!DT_SUCCESS(Status))
                continue;

            FoundProps = TRUE;

            // Fill in missing details in the ID
            Id.m_pRole = DfProps.m_pRole;   // Set role
            Id.m_Uuid = DfProps.m_Uuid;

            //.-.-.-.-.-.-.-.-.-.-.- Step 2: load the DF itself -.-.-.-.-.-.-.-.-.-.-.
            // Now that we have all the properties we can load the DF. But first check
            // if did not load the function already

            // Do we already have this DF in our function list. If not, load it
            pDf = DtCore_DF_Find(pCore, pPt, DfProps.m_Type, DfProps.m_pRole);
            if (pDf == NULL)
            {
                // Load the block and add it to block list
                pDf = DtCore_DF_Open(pCore, DfProps.m_Type, pPt, &Id,
                                                                DfProps.m_CreateStub);
                if(pDf == NULL)
                {
                    DtDbgOut(ERR, CORE, "Failed load DF '%s'", Id.m_pName);
                    return DT_STATUS_FAIL;
                }
            }
        }
        while (FoundProps);
    }
    return DT_STATUS_OK;
}