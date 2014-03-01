//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtProperties.h *#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//
// Common driver - Device and port property get functions.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2012 DekTec Digital Video B.V.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//  1. Redistributions of source code must retain the above copyright notice, this list
//     of conditions and the following disclaimer.
//  2. Redistributions in binary format must reproduce the above copyright notice, this
//     list of conditions and the following disclaimer in the documentation.
//  3. The source code may not be modified for the express purpose of enabling hardware
//     features for which no genuine license has been obtained.
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


#ifndef __DT_PROPERTIES_H
#define __DT_PROPERTIES_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Type to store a single property
typedef struct _DtProperty
{
    const char*  m_pName;
    DtPropertyValue  m_Value;       // 'Normal' property value
    const char*  m_pStr;            // String property value
    DtPropertyValueType m_Type;
    Int  m_PortIndex;
    Int  m_MinFw;
    Int  m_MinHw;
    DtPropertyScope  m_Scope;
} DtProperty;

// Type to store a set of properties whos names result in the same hash
typedef struct _DtPropertyHashSet
{
    const DtProperty*  m_pProperties;
    UInt  m_PropertyCount;
} DtPropertyHashSet;

// Type to store all properties for a single device
typedef struct _DtPropertyStore 
{
    Int  m_TypeNumber;
    const DtPropertyHashSet*  m_pPropertyHashSets;
    UInt  m_PropertyHashSetCount;
} DtPropertyStore;

// Type to store property data
typedef struct _DtPropertyData
{
    char* m_TypeName;           // DTA- or DTU-
    Int  m_TypeNumber;
    Int  m_HardwareRevision;    // Hardware revision (e.g. 302 = 3.2)
    Int  m_FirmwareVersion;     // Firmware Version (= Altera revision), e.g. 3 for
                                // "Firmware Version 3"
    // Property store
    void*  m_pPropertyStore;
    UInt  m_PropertyNotFoundCounter;
    DtString  m_PropertyNotFoundString;

    // Table store
    void*  m_pTableStore;
} DtPropertyData;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtPropertiesInit(DtPropertyData* pPropData);
void  DtPropertiesCleanup(DtPropertyData* pPropData);
DtStatus  DtPropertiesFind(DtPropertyData* pPropData, const char* pName, Int PortIndex,
                                                          const DtProperty**  ppProperty);
DtStatus  DtPropertiesGet(DtPropertyData* pPropData, const char* pName, Int PortIndex,
                                      DtPropertyValue* pValue, DtPropertyValueType* pType,
                                                                 DtPropertyScope* pScope);
DtStatus  DtPropertiesStrGet(DtPropertyData* pPropData, const char* pName, Int PortIndex,
                                                     char* pStr, DtPropertyScope* pScope);
DtStatus  DtPropertiesGetForType(Int  TypeNumber, Int  HwRev, Int  FwVer,
                                      const char* pName, Int PortIndex,
                                      DtPropertyValue* pValue, DtPropertyValueType* pType,
                                                                 DtPropertyScope* pScope);
DtStatus  DtPropertiesStrGetForType(Int  TypeNumber, Int  HwRev, Int  FwVer,
                                                     const char* pName, Int PortIndex,
                                                     char* pStr, DtPropertyScope* pScope);
DtStatus  DtPropertiesReportDriverErrors(DtPropertyData* pPropData);
void  DtResetPropertiesNotFoundCounter(DtPropertyData* pPropData);


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Driver property get functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesDriverGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtPropertyValue  DtPropertiesDriverGet(DtPropertyData* pPropData, const char* pName,
                                                Int Port, DtPropertyValueType ValueType);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetBool -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline Bool  DtPropertiesGetBool(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    DtPropertyValue  Value = DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                            PROPERTY_VALUE_TYPE_BOOL);
    return (Value != 0);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetUInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline UInt  DtPropertiesGetUInt(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (UInt)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                            PROPERTY_VALUE_TYPE_UINT);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetUInt8 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline UInt8  DtPropertiesGetUInt8(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (UInt8)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                           PROPERTY_VALUE_TYPE_UINT8);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetUInt16 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static __inline UInt16  DtPropertiesGetUInt16(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (UInt16)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                          PROPERTY_VALUE_TYPE_UINT16);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetUInt32 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static __inline UInt32  DtPropertiesGetUInt32(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (UInt32)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                          PROPERTY_VALUE_TYPE_UINT32);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetUInt64 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static __inline UInt64  DtPropertiesGetUInt64(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (UInt64)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                          PROPERTY_VALUE_TYPE_UINT64);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static __inline Int  DtPropertiesGetInt(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (Int)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                             PROPERTY_VALUE_TYPE_INT);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetInt8 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline Int8  DtPropertiesGetInt8(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int   PortIndex)
{
    return (Int8)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                            PROPERTY_VALUE_TYPE_INT8);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetInt16 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline Int16  DtPropertiesGetInt16(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (Int16)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                           PROPERTY_VALUE_TYPE_INT16);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetInt32 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline Int32  DtPropertiesGetInt32(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (Int32)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                           PROPERTY_VALUE_TYPE_INT32);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetInt64 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline Int64  DtPropertiesGetInt64(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (Int64)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                           PROPERTY_VALUE_TYPE_INT64);
}

#endif // __DT_PROPERTIES_H
