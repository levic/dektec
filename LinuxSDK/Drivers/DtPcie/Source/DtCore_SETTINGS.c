// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCore_SETTINGS.c *#*#*#*#*#*#* (C) 2017-2022 DekTec
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
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_SETTINGS implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_SETTINGS - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_SETTINGS_Delete -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCore_SETTINGS_Delete(DtCore*  pCore, Int  NumPorts)
{
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    return DtNonVolatileSettingsDelete(&pCore->m_Driver, pCore->m_pDevInfo->m_Serial, 
                                          pCore->m_pDevInfo->m_FirmwareVariant, NumPorts);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_SETTINGS_ManufRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_SETTINGS_ManufRead(DtCore*  pCore, const char*  pName, Int64*  pValue)
{
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    return DtNonVolatileManufSettingsRead(&pCore->m_Driver, 
                                                          pCore->m_pDevInfo->m_TypeNumber,
                                                          (char*)pName, pValue);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_SETTINGS_StringRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtCore_SETTINGS_StringRead(
    DtCore*  pCore, 
    Int  PortIndex, 
    const char*  pCategory, 
    const char*  pName,
    char*  pValue,
    Int  Size)
{
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    return DtNonVolatileSettingsStringRead(&pCore->m_Driver,
                                                   pCore->m_pDevInfo->m_Serial,
                                                   pCore->m_pDevInfo->m_FirmwareVariant,
                                                   PortIndex,
                                                   (char*)pCategory, (char*)pName,
                                                   (char*)pValue, Size);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_SETTINGS_StringWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCore_SETTINGS_StringWrite(
    DtCore*  pCore, 
    Int  PortIndex, 
    const char*  pCategory, 
    const char*  pName,
    const char*  pValue)
{
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    return DtNonVolatileSettingsStringWrite(&pCore->m_Driver,
                                                   pCore->m_pDevInfo->m_Serial,
                                                   pCore->m_pDevInfo->m_FirmwareVariant,
                                                   PortIndex,
                                                   (char*)pCategory, (char*)pName,
                                                   (char*)pValue);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_SETTINGS_ValueRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_SETTINGS_ValueRead(
    DtCore*  pCore, 
    Int  PortIndex, 
    const char*  pCategory, 
    const char*  pName,
    Int64*  pValue)
{
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    return DtNonVolatileSettingsValueRead(&pCore->m_Driver,
                                                   pCore->m_pDevInfo->m_Serial,
                                                   pCore->m_pDevInfo->m_FirmwareVariant,
                                                   PortIndex,
                                                   (char*)pCategory, (char*)pName,
                                                   pValue);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_SETTINGS_ValueWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCore_SETTINGS_ValueWrite(
    DtCore*  pCore, 
    Int  PortIndex, 
    const char*  pCategory, 
    const char*  pName,
    Int64  Value)
{
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    return DtNonVolatileSettingsValueWrite(&pCore->m_Driver,
                                                   pCore->m_pDevInfo->m_Serial,
                                                   pCore->m_pDevInfo->m_FirmwareVariant,
                                                   PortIndex,
                                                   (char*)pCategory, (char*)pName,
                                                   Value);
}