// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCore_EVENTS.c *#*#*#*#*#*#*# (C) 2017-2022 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2017-2022 DekTec Digital Video B.V.
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
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_EVENTS implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to get the EVT-CF from a DtCore object
#define CF_EVT(pCore)         (pCore->m_pCfEvt)

// MACRO with default precondition checks for the DtCfInt core-function
#define CORE_CFEVT_DEFAULT_PRECONDITIONS(pCore)                                          \
    CORE_DEFAULT_PRECONDITIONS(pCore);                                                   \
    DT_ASSERT(CF_EVT(pCore) != NULL)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_EVENTS - Public fuctions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_EVENTS_CleanupEvtData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtCore_EVENTS_CleanupEvtData(DtCfEvtData* pEvtData)
{
    DT_ASSERT(pEvtData != NULL);
    DtCfEvt_CleanupEventsData(pEvtData);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_EVENTS_CreateEvtData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtCfEvtData*  DtCore_EVENTS_CreateEvtData()
{
    return DtCfEvt_CreateEventsData();
}

#ifdef LINBUILD
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_EVENTS_Poll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt  DtCore_EVENTS_Poll(DtCore* pCore, DtFileObject* pFile, poll_table*  pPollTable)
{
    // Sanity checks
    CORE_CFEVT_DEFAULT_PRECONDITIONS(pCore);

    // Check we have an events core-fuction
    if (CF_EVT(pCore) == NULL)
        return 0;

    return DtCfEvt_Poll(CF_EVT(pCore), pFile, pPollTable);
}
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_EVENTS_Set -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_EVENTS_Set(DtCore*  pCore, DtDriverEvent Event, Bool AddIfExists)
{
    // Sanity checks
    CORE_CFEVT_DEFAULT_PRECONDITIONS(pCore);

    // Check we have an events core-fuction
    if (CF_EVT(pCore) == NULL)
        return DT_STATUS_FAIL;

    // Let the EVT-CF do the heavy lifting
    return DtCfEvt_Set(CF_EVT(pCore), Event, AddIfExists);
}

