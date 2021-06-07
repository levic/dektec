//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCore_BC.c *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_BC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

static DtStatus DtCore_BC_OpenInstances(DtCore* pCore, DtPt* pPt, DtBcId Id);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_BC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_BC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Closes a device-level block-controller and removes it from the BC-list
//
DtStatus  DtCore_BC_Close(DtBc*  pBc)
{
    Int  i=0, NumBc=0, Index=0;
    DtCore*  pCore = NULL;
    DtPt*  pPt = NULL;
    DtVectorBc*  pBcList;

    if (pBc == NULL)
        return DT_STATUS_OK;        // Nothing TODO

    // Sanity checks
    pCore = pBc->m_pCore;
    CORE_DEFAULT_PRECONDITIONS(pCore);

    // Find the block list
    pPt = pBc->m_pPt;
    pBcList = (pPt!=NULL) ? pPt->m_pBcList : pCore->m_pBcList;

    // Must have a close function
    if (pBc->m_CloseFunc == NULL)
    {
        DtDbgOut(ERR, CORE, "ERROR: BC '%s' does not have a close function", 
                                                                       pBc->m_InstanceId);
        return DT_STATUS_FAIL;
    }

    // Find the block in the list and remove it
    NumBc = DtVectorBc_Size(pBcList);
    for (i=0; i<NumBc; i++)
    {
        const DtBc*  pBcInList = DtVectorBc_At(pBcList, i);
        if (pBcInList != pBc)
            continue;
        // Erase the entry
        DT_ASSERT(pBcInList!=NULL && pBcInList->m_Id.m_Uuid==pBc->m_Id.m_Uuid);
        DtVectorBc_Erase(pBcList, i);
        break;
    }
    // Clear entry in UUID lookup table
    Index = (Int)(pBc->m_Id.m_Uuid & DT_UUID_INDEX_MASK);
    NumBc = DtVectorBc_Size(pCore->m_pUuidToBc);
    if (Index < NumBc)
    {
        const DtBc*  pBcInList = DtVectorBc_At(pCore->m_pUuidToBc, Index);
        if (pBcInList != NULL)
        {
            // Clear entry
            DT_ASSERT(pBcInList->m_Id.m_Uuid==pBc->m_Id.m_Uuid);
            DtVectorBc_Set(pCore->m_pUuidToBc, Index, NULL);
        }
    }

    // Finally: close the BC
    pBc->m_CloseFunc(pBc);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_BC_Find -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBc*  DtCore_BC_Find(DtCore*  pCore, DtPt*  pPt, DtBcType  Type, const char*  pRole)
{
    DtVectorBc*  pBcList;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    
    // Find the BC-list
    pBcList = (pPt!=NULL) ? pPt->m_pBcList : pCore->m_pBcList;

    if (pBcList == NULL)
        return NULL;
    return DtVectorBc_FindByTypeAndRole(pBcList, Type, pRole);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_BC_FindByAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBc*  DtCore_BC_FindByAddress(DtCore* pCore, DtPt*  pPt, Int  Address)
{
    DtVectorBc*  pBcList;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    
    // Find the block list
    pBcList = (pPt!=NULL) ? pPt->m_pBcList : pCore->m_pBcList;

    if (pBcList == NULL)
        return NULL;

    return DtVectorBc_FindByAddress(pBcList, Address);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_BC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Create a device-level and port level block-controller and adds it to the BC-list 
//
DtBc*  DtCore_BC_Open(DtCore*  pCore, DtBcType  Type, Int  Address, DtPt*  pPt,
                                                    const DtBcId*  pId,  Bool  CreateStub)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBc*  pBc=NULL;
    
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pId != NULL);
    
    // Open the block and add it to block list. 
    pBc = DtBc_OpenType(Type, Address, pCore, pPt, pId->m_pRole, pId->m_Instance, 
                                                                 pId->m_Uuid, CreateStub);
    if (pBc != NULL)
    {
        // Find the BC-list
        DtVectorBc*  pBcList = (pPt!=NULL) ? pPt->m_pBcList : pCore->m_pBcList;
    
        // Add to BC-list
        Status = DtVectorBc_PushBack(pBcList, pBc);

        // Add non-core block-controllers to the UUID lookup list
        if (DT_SUCCESS(Status) && (pId->m_Uuid&DT_UUID_CORE_FLAG)==0)
        {
            Int  i=0;
            Int  NumBc = DtVectorBc_Size(pCore->m_pUuidToBc);
            Int  UuidIndex = (pId->m_Uuid & DT_UUID_INDEX_MASK);

            // Grow table if UuidIndex is out of range
            for (i=NumBc; i<=UuidIndex; i++)
            {
                Status = DtVectorBc_PushBack(pCore->m_pUuidToBc, NULL);
                if (!DT_SUCCESS(Status))
                {
                    DtDbgOut(ERR, CORE, "Failed to resize the UUID-to-BC list");
                    break;
                }
            }
            // If table has right size => add BC to lookup list
            if (DT_SUCCESS(Status))
            {
                DT_ASSERT(UuidIndex < DtVectorBc_Size(pCore->m_pUuidToBc));
                Status = DtVectorBc_Set(pCore->m_pUuidToBc, UuidIndex, pBc);
            }
        }
        if (!DT_SUCCESS(Status))
        {
            DT_ASSERT(pBc->m_CloseFunc != NULL);
            pBc->m_CloseFunc(pBc);
            DtDbgOut(ERR, CORE, "Failed to add BC '%s' to BC list", pId->m_pName);
            return NULL;
        }
    }
    return pBc;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_BC_OpenAll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Create all device-levels block-controllers
//
DtStatus  DtCore_BC_OpenAll(DtCore*  pCore, DtPt*  pPt)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i=0;
    Int  PortIndex = (pPt!=NULL) ? pPt->m_PortIndex : -1;
        
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    // Loop over all known block IDs and load properties 
    for (i=0; i<DT_BC_NUM_KNOWN; i++)
    {
            DtBcId  Id = DT_BC_KNOWN[i];    // Init ID
            DT_RETURN_ON_ERROR(DtCore_BC_OpenInstances(pCore, pPt, Id));
    }
    return DT_STATUS_OK;
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_BC_OpenEssentials -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtCore_BC_OpenEssentials(DtCore* pCore)
{
    DtBcId  Id;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    // Open SPIMF
    DT_BC_SPIMF_INIT_ID(Id, NULL, -1, -1);
    DtCore_BC_OpenInstances(pCore, NULL, Id);

    // Open IPSECG
    DT_BC_IPSECG_INIT_ID(Id, NULL, -1, -1);
    DtCore_BC_OpenInstances(pCore, NULL, Id);

    // Open Reboot
    DT_BC_REBOOT_INIT_ID(Id, NULL, -1, -1);
    DtCore_BC_OpenInstances(pCore, NULL, Id);

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_BC_OpenInstances -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtCore_BC_OpenInstances(DtCore* pCore, DtPt* pPt, DtBcId Id)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  PortIndex = (pPt!=NULL) ? pPt->m_PortIndex : -1;
    Int  Instance = 1;
    Bool  FoundProps = FALSE;
    do
    {
        DtBc*  pBc = NULL;
        DtBcCommonProps  BcProps;
        Id.m_Instance = Instance++;

        // .-.-.-.-.-.-.-.-.-.-.-.- Step 1: load block properties -.-.-.-.-.-.-.-.-.-.-.-.

        FoundProps = FALSE;     // Assume the worst
        Status = DtBcCommonProps_Load(&BcProps, &Id, PortIndex, pCore);
        if (!DT_SUCCESS(Status))
            continue;
        FoundProps = TRUE;

        // Fill in missing details in the ID
        Id.m_pRole = BcProps.m_pRole;   // Set role
        Id.m_Uuid = BcProps.m_Uuid;

        // .-.-.-.-.-.-.-.-.-.-.-.- Step 2: load the block itself -.-.-.-.-.-.-.-.-.-.-.-.
        // Now that we have all the properties we can load the block. But first 
        // check if did not load the block already

        // Do we already have this block in our block list. If not, load it
        pBc = DtCore_BC_FindByAddress(pCore, pPt, BcProps.m_Address);
        if (pBc == NULL)
        {
            // Load the block and add it to block list
            pBc = DtCore_BC_Open(pCore, BcProps.m_Type, BcProps.m_Address,
                                                          pPt, &Id, BcProps.m_CreateStub);
            if (pBc == NULL)
            {
                DtDbgOut(ERR, CORE, "Failed load BC '%s'", Id.m_pName);
                return DT_STATUS_FAIL;
            }
        }
    } while (FoundProps);
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_BC_OpenVvi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_BC_OpenVvi(DtCore*  pCore)
{
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    pCore->m_pBcVvi = (DtBcVVI*)DtCore_BC_FindByAddress((DtCore*)pCore, NULL, 0x000);
    DT_ASSERT(pCore->m_pBcVvi==NULL || pCore->m_pBcVvi->m_Type==DT_BLOCK_TYPE_VVI);
    if(pCore->m_pBcVvi == NULL)
    {
        DtBcId  Id;
        // No role, by definition only one instance and assign core UUID
        DT_BC_VVI_INIT_ID(Id, NULL, 1, CORE_BC_UUID);
        
        // Load the VviBlock
        pCore->m_pBcVvi = (DtBcVVI*)DtCore_BC_Open((DtCore*)pCore, DT_BLOCK_TYPE_VVI, 
                                                                 0x000, NULL, &Id, FALSE);
        if (pCore->m_pBcVvi == NULL)
        {
            DtDbgOut(ERR, CORE, "ERROR: failed to load VVI-block");
            return DT_STATUS_FAIL;
        }
    }
    return  DT_STATUS_OK; 
}
