// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCore_PT.c *#*#*#*#*#*#*#*# (C) 2018-2022 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018-2022 DekTec Digital Video B.V.
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
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_PT implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_PT - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PT_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Closes a device-level port and removes it to the PT-list
//
DtStatus  DtCore_PT_Close(DtPt*  pPt)
{
    DtCore*  pCore = NULL;
    Int  i=0, NumPt=0;

    if (pPt == NULL)
        return DT_STATUS_OK;        // Nothing TODO
                                    
    // Sanity checks
    pCore = pPt->m_pCore;
    CORE_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pCore->m_pPtList != NULL);

    // Must have a close function
    if (pPt->m_CloseFunc == NULL)
    {
        DtDbgOut(ERR, CORE, "ERROR: PT '%s' does not have a close function", 
                                                                       pPt->m_InstanceId);
        return DT_STATUS_FAIL;
    }

    // Find the port in the list and remove it
    NumPt = DtVectorPt_Size(pCore->m_pPtList);
    for (i=0; i<NumPt; i++)
    {
        const DtPt*  pPtInList = DtVectorPt_At(pCore->m_pPtList, i);
        if (pPtInList != pPt)
            continue;
        // Erase the entry
        DtVectorPt_Erase(pCore->m_pPtList, i);
        break;
    }

    // Finally: close the PT
    pPt->m_CloseFunc(pPt);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PT_Find -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtPt*  DtCore_PT_Find(DtCore*  pCore, Int  PortIndex)
{
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    if (pCore->m_pPtList == NULL)
        return NULL;
    return DtVectorPt_FindByPort(pCore->m_pPtList, PortIndex);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PT_GetPortIoCaps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtCore_PT_GetPortIoCaps(DtCore* pCore, Int PortIndex, DtIoCaps* pIoCaps)
{
    DtPt*  pPt = NULL;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pIoCaps != NULL);

    // Find the port
    pPt = DtCore_PT_Find(pCore, PortIndex);
    if (pPt == NULL)
        return DT_STATUS_NOT_FOUND;

    // return the IO-caps
    *pIoCaps = pPt->m_IoCaps;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PT_GetPortIoCaps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtCore_PT_GetIoCaps(DtPt*  pPt, DtIoCaps* pIoCaps)
{
    if (pPt == NULL)
        return DT_STATUS_NOT_FOUND;

    // return the IO-caps
    *pIoCaps = pPt->m_IoCaps;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PT_GetPortIndex -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int DtCore_PT_GetPortIndex(DtPt*  pPt)
{
    if (pPt != NULL)
        return pPt->m_PortIndex;
    else 
        return -1;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PT_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Create a device-level driver-function and adds it to the PT-list
//
DtPt*  DtCore_PT_Open(DtCore*  pCore, DtPortType  Type, Int  PortIndex)
{
    DtStatus  Status = DT_STATUS_OK;
    DtPt*  pPt=NULL;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    // Open the PT and add it to function list. 
    pPt = DtPt_OpenType(Type, pCore, PortIndex);
    if (pPt != NULL)
    {
        // Add to PT-list
        Status = DtVectorPt_PushBack(pCore->m_pPtList, pPt);
        if (!DT_SUCCESS(Status))
        {
            DT_ASSERT(pPt->m_CloseFunc != NULL);
            pPt->m_CloseFunc(pPt);
            DtDbgOut(ERR, CORE, "Failed to add PT '%d' to PT list", PortIndex);
            return NULL;
        }
    }
    return pPt;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PT_OpenAll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_PT_OpenAll(DtCore*  pCore, Int NumPorts)
{
    Int  PortIndex = 0;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    // Loop over all ports
    for (PortIndex=0; PortIndex<NumPorts; PortIndex++)
    {
        DtPt*  pPt = NULL;
        DtPortType  PortType;
        PortType = (DtPortType)DtCore_PROPS_GetInt(pCore, "PORT_TYPE", PortIndex,
                                                                       DT_PORT_TYPE_BASE);
        // Load the port and add it to the port list
        pPt = DtCore_PT_Open(pCore, PortType, PortIndex);
        if (pPt == NULL)
        {
            DtDbgOut(ERR, CORE, "Failed  to load PT for port %d", PortIndex+1);
            return DT_STATUS_FAIL;
        }
    }
    return DT_STATUS_OK;
}