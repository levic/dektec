//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCore_PROPS.c *#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_PROPS implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to get the PROPS-CF from a DtCore object
#define CF_PROPS(pCore)         (pCore->m_pCfProps)

// MACRO with default precondition checks for the DtCfProps core-function
#define CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore)                                        \
    CORE_DEFAULT_PRECONDITIONS(pCore);                                                   \
    DT_ASSERT(CF_PROPS(pCore) != NULL)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_PROPS - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PROPS_Find -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_PROPS_Find(
    DtCore*  pCore,
    const char*  pName, 
    DtPropertyFilterCriteria  Flt,
    const DtProperty**  ppProperty)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    // Must have the properties-cf
    if (CF_PROPS(pCore) == NULL)
        return DT_STATUS_FAIL;
    // Let the core-property-function do the heavy liftning
    return DtCfProps_Find(CF_PROPS(pCore), pName, Flt, ppProperty);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PROPS_Get -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_PROPS_Get(
    DtCore*  pCore,
    const char*  pName, 
    DtPropertyFilterCriteria  Flt,
    DtPropertyValue*  pValue,
    DtPropertyValueType*  pType,
    DtPropertyScope*  pScope,
    Bool  IsOptional,
    DtPropertyValue  Default)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    // Must have the properties-cf
    if (CF_PROPS(pCore) == NULL)
        return DT_STATUS_FAIL;
    // Let the core-property-function do the heavy liftning
    return DtCfProps_Get(CF_PROPS(pCore), pName, Flt, pValue, pType, pScope,
                                                                     IsOptional, Default);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PROPS_GetForType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCore_PROPS_GetForType(
    DtCore*  pCore,
    const char*  pTypeName,
    Int  TypeNumber,
    Int  SubDvc,
    DtPropertyFilterCriteria  Flt,
    const char*  pName,
    DtPropertyValue*  pValue,
    DtPropertyValueType*  pType,
    DtPropertyScope*  pScope)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    return DT_STATUS_NOT_IMPLEMENTED;

    //// Must have the properties-cf
    //if (CF_PROPS(pCore) == NULL)
    //    return DT_STATUS_FAIL;
    //
    //return DtCfProps_GetForType(pTypeName, TypeNumber, SubDvc, Flt.m_HwRev, 
    //                                   Flt.m_FwVersion, Flt.m_FwVariant,
    //                                   pName, Flt.m_PortIndex, pValue, pType, pScope, 
    //                                   Flt.m_DtapiMaj, Flt.m_DtapiMin, Flt.m_DtapiBugfix);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PROPS_GetStr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCore_PROPS_GetStr(
    DtCore*  pCore,
    const char*  pName, 
    DtPropertyFilterCriteria  Flt,
    char*  pStr,
    DtPropertyScope*  pScope)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    // Must have the properties-cf
    if (CF_PROPS(pCore) == NULL)
        return DT_STATUS_FAIL;
    // Let the core-property-function do the heavy liftning
    return DtCfProps_GetStr(CF_PROPS(pCore), pName, Flt, pStr, pScope);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PROPS_GetStrForType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCore_PROPS_GetStrForType(
    DtCore*  pCore, 
    const char*  pTypeName, 
    Int  TypeNumber, 
    Int  SubDvc, 
    DtPropertyFilterCriteria  Flt,
    const char*  pName, 
    char*  pStr,
    DtPropertyScope*  pScope)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    return DT_STATUS_NOT_IMPLEMENTED;

    //return DtPropertiesStrGetForType(pTypeName, TypeNumber, SubDvc, Flt.m_HwRev, 
    //                                   Flt.m_FwVersion, Flt.m_FwVariant,
    //                                   pName, Flt.m_PortIndex, pStr, pScope, 
    //                                   Flt.m_DtapiMaj, Flt.m_DtapiMin, Flt.m_DtapiBugfix);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PROPS_GetTable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_PROPS_GetTable(
    DtCore*  pCore, 
    const char*  pTableName, 
    Int  PortIndex, 
    UInt  MaxNumEntries, 
    UInt*  pNumEntries, 
    DtTableEntry*  pTableEntry,
    UInt  OutBufSize)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);
    return DT_STATUS_NOT_IMPLEMENTED;
    //return DtTableGet(&CF_PROPS(pCore).m_PropData, pTableName, PortIndex, MaxNumEntries,
    //                                               pNumEntries, pTableEntry,  OutBufSize);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PROPS_GetBool -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get the value of a Bool-property
//
Bool  DtCore_PROPS_GetBool(DtCore*  pCore, const char*  pName, Int  PortIndex,
                                                                            Bool  Default)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    // Must have the properties-cf
    if (CF_PROPS(pCore) == NULL)
        return Default;
    // Let the core-property-function do the heavy liftning
    return DtCfProps_GetBool(CF_PROPS(pCore), pName, PortIndex, Default);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PROPS_GetInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Get the value of an integer type property
//
Int  DtCore_PROPS_GetInt(DtCore*  pCore, const char*  pName, Int  PortIndex, Int  Default)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    // Must have the properties-cf
    if (CF_PROPS(pCore) == NULL)
        return Default;
    // Let the core-property-function do the heavy liftning
    return DtCfProps_GetInt(CF_PROPS(pCore), pName, PortIndex, Default);
}
// Get Int8-property
Int8  DtCore_PROPS_GetInt8(DtCore*  pCore, const char*  pName, Int  PortIndex,
                                                                            Int8  Default)
{
        // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    // Must have the properties-cf
    if (CF_PROPS(pCore) == NULL)
        return Default;
    // Let the core-property-function do the heavy liftning
    return DtCfProps_GetInt8(CF_PROPS(pCore), pName, PortIndex, Default);
}
// Get Int16-property
Int16  DtCore_PROPS_GetInt16(DtCore*  pCore, const char*  pName, Int  PortIndex,
                                                                           Int16  Default)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    // Must have the properties-cf
    if (CF_PROPS(pCore) == NULL)
        return Default;
    // Let the core-property-function do the heavy liftning
    return DtCfProps_GetInt16(CF_PROPS(pCore), pName, PortIndex, Default);
}
// Get Int32-property
Int32  DtCore_PROPS_GetInt32(DtCore*  pCore, const char*  pName, Int  PortIndex, 
                                                                           Int32  Default)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    // Must have the properties-cf
    if (CF_PROPS(pCore) == NULL)
        return Default;
    // Let the core-property-function do the heavy liftning
    return DtCfProps_GetInt32(CF_PROPS(pCore), pName, PortIndex, Default);
}
// Get Int64-property
Int64  DtCore_PROPS_GetInt64(DtCore*  pCore, const char*  pName, Int  PortIndex, 
                                                                           Int64  Default)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    // Must have the properties-cf
    if (CF_PROPS(pCore) == NULL)
        return Default;
    // Let the core-property-function do the heavy liftning
    return DtCfProps_GetInt64(CF_PROPS(pCore), pName, PortIndex, Default);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PROPS_GetUInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get the value of an unsigned-integer type property
//
UInt  DtCore_PROPS_GetUInt(DtCore*  pCore, const char*  pName, Int  PortIndex, 
                                                                            UInt  Default)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    // Must have the properties-cf
    if (CF_PROPS(pCore) == NULL)
        return Default;
    // Let the core-property-function do the heavy liftning
    return DtCfProps_GetUInt(CF_PROPS(pCore), pName, PortIndex, Default);
}
// Get UInt8-property
UInt8  DtCore_PROPS_GetUInt8(DtCore*  pCore, const char*  pName, Int  PortIndex, 
                                                                           UInt8  Default)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    // Must have the properties-cf
    if (CF_PROPS(pCore) == NULL)
        return Default;
    // Let the core-property-function do the heavy liftning
    return DtCfProps_GetUInt8(CF_PROPS(pCore), pName, PortIndex, Default);
}
// Get UInt16-property
UInt16  DtCore_PROPS_GetUInt16(DtCore*  pCore, const char*  pName, Int  PortIndex, 
                                                                          UInt16  Default)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    // Must have the properties-cf
    if (CF_PROPS(pCore) == NULL)
        return Default;
    // Let the core-property-function do the heavy liftning
    return DtCfProps_GetUInt16(CF_PROPS(pCore), pName, PortIndex, Default);
}
// Get UInt32-property
UInt32  DtCore_PROPS_GetUInt32(DtCore*  pCore, const char*  pName, Int  PortIndex, 
                                                                          UInt32  Default)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    // Must have the properties-cf
    if (CF_PROPS(pCore) == NULL)
        return Default;
    // Let the core-property-function do the heavy liftning
    return DtCfProps_GetUInt32(CF_PROPS(pCore), pName, PortIndex, Default);
}
// Get UInt64-property
UInt64  DtCore_PROPS_GetUInt64(DtCore*  pCore, const char*  pName, Int  PortIndex,
                                                                          UInt64  Default)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    // Must have the properties-cf
    if (CF_PROPS(pCore) == NULL)
        return Default;
    // Let the core-property-function do the heavy liftning
    return DtCfProps_GetUInt64(CF_PROPS(pCore), pName, PortIndex, Default);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_PROPS_ReInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_PROPS_ReInit(DtCore*  pCore)
{
    // Sanity checks
    CORE_CFPROPS_DEFAULT_PRECONDITIONS(pCore);

    // Must have the properties-cf
    if (CF_PROPS(pCore) == NULL)
        return DT_STATUS_NOT_INITIALISED;
    return DtCfProps_ReInit(CF_PROPS(pCore));
}