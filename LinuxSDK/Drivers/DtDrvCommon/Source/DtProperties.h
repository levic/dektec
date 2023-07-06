//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtProperties.h *#*#*#*#*#*#*#* (C) 2011-2016 DekTec
//
// Common driver - Device and port property get functions.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2016 DekTec Digital Video B.V.
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


#ifndef __DT_PROPERTIES_H
#define __DT_PROPERTIES_H

// #pragma warning (disable : 4245)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Type to store a single property
typedef struct _DtProperty
{
    const char*  m_pName;
    DtPropertyValue  m_Value;       // 'Normal' property value
    const char*  m_pStr;            // String property value
    DtPropertyValueType m_Type;
    Int  m_PortIndex;
    Int  m_FwVariant;
    Int  m_MinFw;
    Int  m_MinHw;
    Int  m_MaxHw;
    DtPropertyScope  m_Scope;
    Int  m_MinDtapiMaj;             // Minimum DTAPI major version
    Int  m_MinDtapiMin;             // Minimum DTAPI minor version
    Int  m_MinDtapiBugfix;          // Minimum DTAPI bugfix version
} DtProperty;

typedef struct  _DtPropertyFilterCriteria
{
    Int  m_PortIndex;               // Port index the property should apply to
    Int  m_HwRev;                   // Hardware revision the property should apply to
    Int  m_FwVersion;               // Firmware version the property should apply to
    Int  m_FwVariant;               // Firmware variant the property should apply to
    Int  m_DtapiMaj;                // Major DTAPI version the property should apply to
    Int  m_DtapiMin;                // Minor DTAPI version the property should apply to
    Int  m_DtapiBugfix;             // Bugfix DTAPI version the property should apply to
}  DtPropertyFilterCriteria;
// Init filter criteria to 'safe' initial values
#define DT_PROPERTY_FILTER_CRITERIA_INIT(F, PORT_INDEX)                     \
    do                                                                      \
    {                                                                       \
        DtMemZero(&F, sizeof(F));                                           \
        DT_ASSERT(PORT_INDEX >= -1);                                        \
        F.m_PortIndex = PORT_INDEX;                                         \
        F.m_DtapiMaj = F.m_DtapiMin = F.m_DtapiBugfix = -1;                 \
    } while(0)
#define DT_PROPERTY_FILTER_CRITERIA_INIT_FROM_PROPDATA(F, PD, PORT_INDEX)   \
    do                                                                      \
    {                                                                       \
        DT_PROPERTY_FILTER_CRITERIA_INIT(F, PORT_INDEX);                    \
        F.m_HwRev = PD.m_HardwareRevision;                                  \
        F.m_FwVersion = PD.m_FirmwareVersion;                               \
        F.m_FwVariant = PD.m_FirmwareVariant;                               \
    } while(0)

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
    Int  m_SubDvcOrSubType;     // In old drivers (Dta, Dtu, DtaNw) properties strores 
                                // are unqiue based on the combination of type-number and 
                                // sub-device, while for new drivers (DtPcie) the unique 
                                // combination is type-number and sub-type
    const DtPropertyHashSet*  m_pPropertyHashSets;
    UInt  m_PropertyHashSetCount;
} DtPropertyStore;

// Type to store property data
typedef struct _DtPropertyData
{
    const char* m_TypeName;     // DTA- or DTU-
    Int  m_TypeNumber;
    Int  m_SubDvcOrSubType;     // In old drivers (Dta, Dtu, DtaNw) properties strores 
                                // are unqiue based on the combination of type-number and 
                                // sub-device, while for new drivers (DtPcie) the unique 
                                // combination is type-number and sub-type
    Int  m_HardwareRevision;    // Hardware revision (e.g. 302 = 3.2)
    Int  m_FirmwareVariant;
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
                                             const DtProperty**  ppProperty,
                                             Int DtapiMaj, Int DtapiMin, Int DtapiBugfix);
DtStatus  DtPropertiesGet(DtPropertyData* pPropData, const char* pName, Int PortIndex,
                                      DtPropertyValue* pValue, DtPropertyValueType* pType,
                                      DtPropertyScope* pScope,
                                      Int  DtapiMaj, Int DtapiMin, Int DtapiBugfix);
void DtPropertiesGetAltName(const char* pName, Int* pNumAltNames, 
                                                          const char* const* pAltNames[]);
DtStatus  DtPropertiesStrGet(DtPropertyData* pPropData, const char* pName, Int PortIndex,
                                             char* pStr, DtPropertyScope* pScope,
                                             Int DtapiMaj, Int DtapiMin, Int DtapiBugfix);
DtStatus  DtPropertiesGetForType(const char*  pTypeName, Int  TypeNumber, 
                                      Int  SubDvcOrSubType,
                                      Int  HwRev, Int  FwVer, Int FwVariant,
                                      const char* pName, Int PortIndex,
                                      DtPropertyValue* pValue, DtPropertyValueType* pType,
                                      DtPropertyScope* pScope,
                                      Int DtapiMaj, Int DtapiMin, Int DtapiBugfix);
DtStatus  DtPropertiesStrGetForType(const char*  pTypeName, Int  TypeNumber, 
                                             Int  SubDvcOrSubType,
                                             Int  HwRev, Int  FwVer, Int FwVariant,
                                             const char* pName, Int PortIndex,
                                             char* pStr, DtPropertyScope* pScope,
                                             Int DtapiMaj, Int DtapiMin, Int DtapiBugfix);
DtStatus  DtPropertiesReportDriverErrors(DtPropertyData* pPropData);
void  DtResetPropertiesNotFoundCounter(DtPropertyData* pPropData);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Driver property get functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

DtPropertyValue  DtPropertiesDriverGet(DtPropertyData* pPropData, 
                                                  const char* pName,
                                                  Int Port, DtPropertyValueType ValueType,
                                                  Bool  IsOptionalProp,
                                                  DtPropertyValue  Default);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetBool -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline Bool  DtPropertiesGetBool(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    DtPropertyValue  Value = DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                     PROPERTY_VALUE_TYPE_BOOL, FALSE, -1);
    return (Value != 0);
}
// Gets an optional Bool property (returns specified default if prop was not found)
static __inline Bool  DtPropertiesGetBoolWithDefault(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex,
    Bool  Default)
{
    // Get as optional property (i.e. no error if it does not exist)
    DtPropertyValue Value = DtPropertiesDriverGet(pPropData, pName, PortIndex, 
                                                                 PROPERTY_VALUE_TYPE_BOOL,
                                                                 TRUE, // Is optional
                                                                 ((Default) ? 1 : 0));
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
                                                     PROPERTY_VALUE_TYPE_UINT, FALSE, -1);
}
// Gets an optional UInt property (returns specified default if prop was not found)
static __inline UInt  DtPropertiesGetUIntWithDefault(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex,
    UInt  Default)
{
    // Get as optional property (i.e. no error if it does not exist)
    return (UInt)DtPropertiesDriverGet(pPropData, pName, PortIndex, 
                                                                 PROPERTY_VALUE_TYPE_UINT,
                                                                 TRUE, Default);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetUInt8 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline UInt8  DtPropertiesGetUInt8(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (UInt8)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                    PROPERTY_VALUE_TYPE_UINT8, FALSE, -1);
}
// Gets an optional UInt8 property (returns specified default if prop was not found)
static __inline UInt8  DtPropertiesGetUInt8WithDefault(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex,
    UInt8  Default)
{
    // Get as optional property (i.e. no error if it does not exist)
    return (UInt8)DtPropertiesDriverGet(pPropData, pName, PortIndex, 
                                                                PROPERTY_VALUE_TYPE_UINT8,
                                                                TRUE, Default);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetUInt16 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static __inline UInt16  DtPropertiesGetUInt16(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (UInt16)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                   PROPERTY_VALUE_TYPE_UINT16, FALSE, -1);
}
// Gets an optional UInt16 property (returns specified default if prop was not found)
static __inline UInt16  DtPropertiesGetUInt16WithDefault(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex,
    UInt16  Default)
{
    // Get as optional property (i.e. no error if it does not exist)
    return (UInt16)DtPropertiesDriverGet(pPropData, pName, PortIndex, 
                                                               PROPERTY_VALUE_TYPE_UINT16,
                                                               TRUE, Default);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetUInt32 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static __inline UInt32  DtPropertiesGetUInt32(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (UInt32)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                   PROPERTY_VALUE_TYPE_UINT32, FALSE, -1);
}
// Gets an optional UInt32 property (returns specified default if prop was not found)
static __inline UInt32  DtPropertiesGetUInt32WithDefault(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex,
    UInt32  Default)
{
    // Get as optional property (i.e. no error if it does not exist)
    return (UInt32)DtPropertiesDriverGet(pPropData, pName, PortIndex, 
                                                               PROPERTY_VALUE_TYPE_UINT32,
                                                               TRUE, Default);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetUInt64 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static __inline UInt64  DtPropertiesGetUInt64(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (UInt64)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                   PROPERTY_VALUE_TYPE_UINT64, FALSE, -1);
}
// Gets an optional UInt64 property (returns specified default if prop was not found)
static __inline UInt64  DtPropertiesGetUInt64WithDefault(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex,
    UInt64  Default)
{
    // Get as optional property (i.e. no error if it does not exist)
    return (UInt64)DtPropertiesDriverGet(pPropData, pName, PortIndex, 
                                                               PROPERTY_VALUE_TYPE_UINT64,
                                                               TRUE, Default);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static __inline Int  DtPropertiesGetInt(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (Int)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                      PROPERTY_VALUE_TYPE_INT, FALSE, -1);
}
// Gets an optional Int property (returns specified default if prop was not found)
static __inline Int  DtPropertiesGetIntWithDefault(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex,
    Int  Default)
{
    // Get as optional property (i.e. no error if it does not exist)
    return (Int)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                  PROPERTY_VALUE_TYPE_INT, TRUE, Default);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetInt8 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline Int8  DtPropertiesGetInt8(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int   PortIndex)
{
    return (Int8)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                     PROPERTY_VALUE_TYPE_INT8, FALSE, -1);
}
// Gets an optional Int8 property (returns specified default if prop was not found)
static __inline Int8  DtPropertiesGetInt8WithDefault(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex,
    Int8  Default)
{
    // Get as optional property (i.e. no error if it does not exist)
    return (Int8)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                 PROPERTY_VALUE_TYPE_INT8, TRUE, Default);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetInt16 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline Int16  DtPropertiesGetInt16(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (Int16)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                    PROPERTY_VALUE_TYPE_INT16, FALSE, -1);
}
// Gets an optional Int16 property (returns specified default if prop was not found)
static __inline Int16  DtPropertiesGetInt16WithDefault(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex,
    Int16  Default)
{
    // Get as optional property (i.e. no error if it does not exist)
    return (Int16)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                PROPERTY_VALUE_TYPE_INT16, TRUE, Default);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetInt32 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline Int32  DtPropertiesGetInt32(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (Int32)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                    PROPERTY_VALUE_TYPE_INT32, FALSE, -1);
}
// Gets an optional Int32 property (returns specified default if prop was not found)
static __inline Int32  DtPropertiesGetInt32WithDefault(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex,
    Int32  Default)
{
    // Get as optional property (i.e. no error if it does not exist)
    return (Int32)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                PROPERTY_VALUE_TYPE_INT32, TRUE, Default);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetInt64 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static __inline Int64  DtPropertiesGetInt64(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex)
{
    return (Int64)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                    PROPERTY_VALUE_TYPE_INT64, FALSE, -1);
}
// Gets an optional Int64 property (returns specified default if prop was not found)
static __inline Int64  DtPropertiesGetInt64WithDefault(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex,
    Int64  Default)
{
    // Get as optional property (i.e. no error if it does not exist)
    return (Int64)DtPropertiesDriverGet(pPropData, pName, PortIndex,
                                                PROPERTY_VALUE_TYPE_INT64, TRUE, Default);
}

#endif // __DT_PROPERTIES_H
