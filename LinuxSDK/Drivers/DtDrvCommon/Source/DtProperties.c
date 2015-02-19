//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtProperties.c *#*#*#*#*#*#*#* (C) 2011-2015 DekTec
//
// Driver common - Device and port property get functions
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2015 DekTec Digital Video B.V.
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


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#include <DtDrvCommon.h>

// Generated property store
extern const Int  DtPropertyStoreCount;
extern const DtPropertyStore  DtPropertyStores[];

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- AltPropNames[] -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// AltPropNames contains a table that can translate "old" capability names into 
// receiver and modulator specific capability names.
// It will be used when an application uses "old" capability names.

// Type to store alternative property names
typedef struct _DtAltPropNames
{
    const char*  m_pName;           // Property name
    int  m_NumAltNames;             // Alternative property names
    const char* m_pAltNames[2];
} DtAltPropNames;

static const int AltPropNamesCount = 17;
static const DtAltPropNames  AltPropNames[] =
{
    // Legacy-            Receiver-       Modulator capability name        
    { "CAP_ATSC",    2, { "CAP_RX_ATSC",  "CAP_TX_ATSC"   } },
    { "CAP_ATSC_MH", 2, { "CAP_RX_MH",    "CAP_TX_MH"     } },
    { "CAP_CMMB",    2, { "CAP_RX_CMMB",  "CAP_TX_CMMB"   } },
    { "CAP_DAB",     2, { "CAP_RX_DAB",   "CAP_TX_DAB"    } },
    { "CAP_DTMB",    2, { "CAP_RX_DTMB",  "CAP_TX_DTMB"   } },
    { "CAP_DVBC2",   2, { "CAP_RX_DVBC2", "CAP_TX_DVBC2"  } },
    { "CAP_DVBS",    2, { "CAP_RX_DVBS",  "CAP_TX_DVBS"   } },
    { "CAP_DVBS2",   2, { "CAP_RX_DVBS2", "CAP_TX_DVBS2"  } },
    { "CAP_DVBT",    2, { "CAP_RX_DVBT",  "CAP_TX_DVBT"   } },
    { "CAP_DVBT2",   2, { "CAP_RX_DVBT2", "CAP_TX_DVBT2"  } },
    { "CAP_IQ",      2, { "CAP_RX_IQ",    "CAP_TX_IQ"     } },
    { "CAP_ISDBS",   2, { "CAP_RX_ISDBS", "CAP_TX_ISDBS"  } },
    { "CAP_ISDBT",   2, { "CAP_RX_ISDBT", "CAP_TX_ISDBT"  } },
    { "CAP_QAM_A",   2, { "CAP_RX_QAMA",  "CAP_TX_QAMA"   } },
    { "CAP_QAM_B",   2, { "CAP_RX_QAMB",  "CAP_TX_QAMB"   } },
    { "CAP_QAM_C",   2, { "CAP_RX_QAMC",  "CAP_TX_QAMC"   } },
    { "CAP_DVBT2MI", 1, { "CAP_TX_T2MI",  ""              } },
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPropertiesInit(DtPropertyData* pPropData)
{
    Int  Index;

    // Default no property store found for our device
    pPropData->m_pPropertyStore = NULL;

    // Find the property store for our device
    for (Index=0; Index<DtPropertyStoreCount; Index++)
    {
        if (DtPropertyStores[Index].m_TypeNumber==pPropData->m_TypeNumber &&
                                  DtPropertyStores[Index].m_SubDvc == pPropData->m_SubDvc)
        {
            pPropData->m_pPropertyStore = (void*)&DtPropertyStores[Index];
            break;
        }
    }

    // Check if we found a property store for our device
    if (pPropData->m_pPropertyStore == NULL)
        return DT_STATUS_NOT_FOUND;

    pPropData->m_PropertyNotFoundCounter = 0;
    return DtStringAlloc(&pPropData->m_PropertyNotFoundString, 50);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesCleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtPropertiesCleanup(DtPropertyData* pPropData)
{
    DtStringFree(&pPropData->m_PropertyNotFoundString);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesFind -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPropertiesFind(DtPropertyData* pPropData, const char* pName, Int PortIndex,
               const DtProperty** ppProperty, Int DtapiMaj, Int DtapiMin, Int DtapiBugfix)
{
    DtStatus  Status = DT_STATUS_OK;
    const DtPropertyStore*  pStore = (DtPropertyStore*)pPropData->m_pPropertyStore;
    const DtPropertyHashSet*  pHashSets = (DtPropertyHashSet*)pStore->m_pPropertyHashSets;
    UInt  HashSetCount = pStore->m_PropertyHashSetCount;
    Int  FwVersion = pPropData->m_FirmwareVersion;
    Int  HwRevision = pPropData->m_HardwareRevision;
    
    UInt  Hash = 0;
    Int  Index;
    Bool  PropertyNameFound = FALSE;
    const DtPropertyHashSet*  pHashSet = NULL;
    Int  PropertyCount = 0;
    *ppProperty = NULL;

    // For devices with no 'EC' VPD resource and no registry key which forces the
    // hardware revision, treat the hardware revision as 0
    if (HwRevision < 0)
        HwRevision = 0;

    // Determine the name hash
    Hash = DtDjb2(pName) % HashSetCount;

    // Get correct hash set (if it exists)
    if (DT_SUCCESS(Status))
    {
        pHashSet = &pHashSets[Hash];
        PropertyCount = pHashSets[Hash].m_PropertyCount;

        // Check if a hash set was available for this hash value
        if (pHashSet == NULL)
            Status = DT_STATUS_NOT_FOUND;
    }

    // Get correct property entry within hash set
    if (DT_SUCCESS(Status))
    {
        // Search all properties
        for (Index=0; Index<PropertyCount; Index++)
        {
            const DtProperty*  pProp = &pHashSet->m_pProperties[Index];

            // Check the property name was already found
            if (PropertyNameFound)
            {
                // When the property name was found earlier, only accept entries without
                // a name. We just stop when (another) named entry is found.
                if (pProp->m_pName != NULL)
                    break;
            } else {
                // Compare name to check if we found the first occurrence
                if (DtAnsiCharArrayIsEqual(pName, pProp->m_pName))
                    PropertyNameFound = TRUE;
            }

            if (PropertyNameFound)
            {
                // Check port number
                if (PortIndex == pProp->m_PortIndex)
                {
                    // Check minimal firmware version and hardware version
                    if (FwVersion>=pProp->m_MinFw && HwRevision>=pProp->m_MinHw &&
                                        (pProp->m_MaxHw==-1 || HwRevision<pProp->m_MaxHw))
                    {
                        Bool  DtapiVerOk = FALSE;
                        // -1 means the request came from the driver
                        if (DtapiMaj==-1 && DtapiMin==-1 && DtapiBugfix==-1)
                            DtapiVerOk = TRUE;
                        else if (DtapiMaj > pProp->m_MinDtapiMaj)
                            DtapiVerOk = TRUE;
                        else if (DtapiMaj==pProp->m_MinDtapiMaj)
                        {
                            if (DtapiMin > pProp->m_MinDtapiMin)
                                DtapiVerOk = TRUE;
                            else if (DtapiMin==pProp->m_MinDtapiMin
                                                 && DtapiBugfix>= pProp->m_MinDtapiBugfix)
                                DtapiVerOk = TRUE;
                        }
                        // Check minimal DTAPI version
                        if (DtapiVerOk)
                        {
                            *ppProperty = pProp;

                            // We can stop here since the parser has ordered each
                            // property by minimal firmware version/hardware version.
                            // This means the first hit is the best one...
                            break;
                        }
                    }
                }
            }
        }

        if (!PropertyNameFound)
        {   
            if (DtAnsiCharArrayStartsWith(pName, "CAP_"))
                DtDbgOut(AVG, PROP, "PropertyName(Capability) %s is not found at all for"
                         " %s-%d", pName, pPropData->m_TypeName, pPropData->m_TypeNumber);
            else
                DtDbgOut(MIN, PROP, "PropertyName %s is not found at all for %s-%d",
                                   pName, pPropData->m_TypeName, pPropData->m_TypeNumber);
        }
            
        // Check if the property was found
        if (*ppProperty == NULL)
        {
            Status = DT_STATUS_NOT_FOUND;
            DtDbgOut(AVG, PROP, "Failed to find property %s for %s-%d, FW %d, HW %d", 
                             pName, pPropData->m_TypeName, pPropData->m_TypeNumber,
                             pPropData->m_FirmwareVersion, pPropData->m_HardwareRevision);
        }
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPropertiesGet(DtPropertyData* pPropData, const char* pName, Int PortIndex,
                                      DtPropertyValue* pValue, DtPropertyValueType* pType,
                                      DtPropertyScope* pScope,
                                      Int DtapiMaj, Int DtapiMin, Int DtapiBugfix)
{
    DtStatus  Status = DT_STATUS_OK;

    // Find the property in the property tables
    const  DtProperty*  pProperty = NULL;
    Status = DtPropertiesFind(pPropData, pName, PortIndex, &pProperty, DtapiMaj,
                                                                   DtapiMin, DtapiBugfix);
    if (Status == DT_STATUS_NOT_FOUND)
    {
        Int  i;
        Int  NumAltNames = 0;
        const char* const*  pAltNames;

        // If not found, look for alternative names
        // Get the alternative names
        DtPropertiesGetAltName(pName, &NumAltNames, &pAltNames);
        // Try to get the property for the alternative name
        for (i=0; i<NumAltNames && Status==DT_STATUS_NOT_FOUND; i++)
        {
            Status = DtPropertiesFind(pPropData, pAltNames[i], PortIndex, &pProperty,
                                                         DtapiMaj, DtapiMin, DtapiBugfix);
        }
    }  
    
    // Get property details
    if (DT_SUCCESS(Status) && pProperty!=NULL)
    {
        // Copy property info
        *pValue = pProperty->m_Value;
        *pType = pProperty->m_Type;
        *pScope = pProperty->m_Scope;
        DtDbgOut(MAX, PROP, "Found property %s for %s-%d, FW %d, HW %d. "
                            "Value: 0x%08x%08x, Type:%i, Scope:%i",
                              pName, pPropData->m_TypeName, pPropData->m_TypeNumber,
                              pPropData->m_FirmwareVersion, pPropData->m_HardwareRevision,
                              (UInt32)(*pValue>>32),(UInt32)*pValue, *pType, *pScope);
    } 
    else if (Status == DT_STATUS_NOT_FOUND)
    {
        if (DtAnsiCharArrayStartsWith(pName, "CAP_"))
        {
            // Generate capability info
            *pValue = 0;
            *pType = PROPERTY_VALUE_TYPE_BOOL;
            *pScope = PROPERTY_SCOPE_DTAPI | PROPERTY_SCOPE_DRIVER;

            // Pretend that property was found
            Status = DT_STATUS_OK;
            DtDbgOut(MAX, PROP, "Assume property %s is a capability that is not "
                                                                     "supported.", pName);
        }
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetAltName -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns an arrray of alternative property names
// 
void DtPropertiesGetAltName(const char* pName, Int* pNumAltNames, 
                                                           const char* const* pAltNames[])
{
    Int  i;
    Int  AltNameEntry = -1;
    for (i=0; i<AltPropNamesCount; i++)
    {
        if (DtAnsiCharArrayIsEqual(pName, AltPropNames[i].m_pName))
        {
            AltNameEntry = i;
            break;
        }
    }
    if (AltNameEntry < 0)
    {
        // No alternative name found
        *pNumAltNames = 0;
        *pAltNames = NULL;
    }
    else
    {
        // Return alternative names
        *pNumAltNames = AltPropNames[AltNameEntry].m_NumAltNames;
        *pAltNames = AltPropNames[AltNameEntry].m_pAltNames;
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesStrGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Get a string property
//
DtStatus  DtPropertiesStrGet(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex,
    char*  pStr,
    DtPropertyScope*  pScope,
    Int DtapiMaj,
    Int DtapiMin,
    Int DtapiBugfix)
{
    Int  i=0;
    DtStatus  Status = DT_STATUS_OK;

    // Lookup the property
    const  DtProperty*  pProperty = NULL;
    Status = DtPropertiesFind(pPropData, pName, PortIndex, &pProperty, DtapiMaj,
                                                                   DtapiMin, DtapiBugfix);
    // Check we have a string property
    if (DT_SUCCESS(Status) && pProperty!=NULL && 
                                            pProperty->m_Type!=PROPERTY_VALUE_TYPE_STRING)
        Status = DT_STATUS_NOT_FOUND; // Found the property, but it is not a string

    if (DT_SUCCESS(Status) && pProperty!=NULL)
    {
        // Copy string
        for (i=0; i<PROPERTY_STR_MAX_SIZE; i++)
            pStr[i] = pProperty->m_pStr[i];
        *pScope = pProperty->m_Scope;

        DtDbgOut(MAX, PROP, "Found property %s for %s-%d, FW %d, HW %d. Value: %s, "
                                                        "Type:%i, Scope:%i", pName,
                                                        pPropData->m_TypeName,
                                                        pPropData->m_TypeNumber,
                                                        pPropData->m_FirmwareVersion,
                                                        pPropData->m_HardwareRevision,
                                                        pStr, pProperty->m_Type, *pScope);
    }
    else
    {
        DtDbgOut(MAX, PROP, "Can not find string-property %s", pName);
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGetForType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPropertiesGetForType(
    const char*  pTypeName,
    Int  TypeNumber,
    Int  SubDvc,
    Int  HwRev,
    Int  FwVer,
    const char* pName,
    Int PortIndex,
    DtPropertyValue* pValue,
    DtPropertyValueType* pType,
    DtPropertyScope* pScope,
    Int DtapiMaj,
    Int DtapiMin,
    Int DtapiBugfix)
{
    DtPropertyData  PropData;
    DtStatus  Status = DT_STATUS_OK;
        
    // Init property data structure
    PropData.m_TypeNumber = TypeNumber;
    PropData.m_SubDvc = SubDvc;
    PropData.m_FirmwareVersion = FwVer;
    PropData.m_HardwareRevision = HwRev;
    PropData.m_TypeName = (char*)pTypeName;

    Status = DtPropertiesInit(&PropData);
    if (!DT_SUCCESS(Status))
        return Status;

    // Now find the property
    Status = DtPropertiesGet(&PropData, pName, PortIndex, pValue, pType, pScope, DtapiMaj,
                                                                   DtapiMin, DtapiBugfix);

    // Dont forget the cleanup or we're leaving allocated data hanging around
    DtPropertiesCleanup(&PropData);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesStrGetForType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPropertiesStrGetForType(
    const char*  pTypeName,
    Int  TypeNumber,
    Int  SubDvc,
    Int  HwRev,
    Int  FwVer,
    const char* pName,
    Int PortIndex,
    char* pStr,
    DtPropertyScope* pScope,
    Int DtapiMaj,
    Int DtapiMin,
    Int DtapiBugfix)
{
    DtPropertyData  PropData;
    DtStatus  Status = DT_STATUS_OK;
        
    // Init property data structure
    PropData.m_TypeNumber = TypeNumber;
    PropData.m_SubDvc = SubDvc;
    PropData.m_FirmwareVersion = FwVer;
    PropData.m_HardwareRevision = HwRev;
    PropData.m_TypeName = (char*)pTypeName;

    Status = DtPropertiesInit(&PropData);
    if (!DT_SUCCESS(Status))
        return Status;

    // Now find the property
    Status = DtPropertiesStrGet(&PropData, pName, PortIndex, pStr, pScope, DtapiMaj,
                                                                   DtapiMin, DtapiBugfix);
    // Dont forget the cleanup or we're leaving allocated data hanging around
    DtPropertiesCleanup(&PropData);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesDriverGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Macro used in the driver only to get a property from the store, validates its type,
// casts the value to the correct requested type and increments the not-found counter if
// the requested property was not found.
//
DtPropertyValue  DtPropertiesDriverGet(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  Port,
    DtPropertyValueType  ValueType)
{
    DtStatus  Status;
    DtPropertyValue  Value = (DtPropertyValue)-1;
    DtPropertyValueType  Type;
    DtPropertyScope  Scope;

    /* Get property */
    Status = DtPropertiesGet(pPropData, pName, Port, &Value, &Type, &Scope, -1, -1, -1);
    if (DT_SUCCESS(Status))
    {
        if(Type!=ValueType || (Scope&PROPERTY_SCOPE_DRIVER)!=PROPERTY_SCOPE_DRIVER)
            DtDbgOut(ERR, PROP, "Get property failure %s for %s-%d, FW %d, HW %d", pName,                                                  
                                                  pPropData->m_TypeName,
                                                  pPropData->m_TypeNumber,
                                                  pPropData->m_FirmwareVersion,
                                                  pPropData->m_HardwareRevision);

        /* Check if propery was designed to be used in the driver */
        DT_ASSERT((Scope&PROPERTY_SCOPE_DRIVER) == PROPERTY_SCOPE_DRIVER);
        /* Check if property type satisfies the expected type */
        DT_ASSERT(Type == ValueType);

    } else {
        // Increment property not found count and construct string
        pPropData->m_PropertyNotFoundCounter++;
        DtStringClear(&pPropData->m_PropertyNotFoundString);
        DtStringAppendChars(&pPropData->m_PropertyNotFoundString, pName);
        DtStringAppendChars(&pPropData->m_PropertyNotFoundString, "(MinFw: ");
        DtStringUIntegerToDtStringAppend(&pPropData->m_PropertyNotFoundString, 10, 
                                                            pPropData->m_FirmwareVersion);
        DtStringAppendChars(&pPropData->m_PropertyNotFoundString, ", MinHw: ");
        DtStringUIntegerToDtStringAppend(&pPropData->m_PropertyNotFoundString, 10, 
                                                           pPropData->m_HardwareRevision);
        DtStringAppendChars(&pPropData->m_PropertyNotFoundString, ")");

        DtDbgOut(AVG, PROP, "Failed to get property %s for %s-%d, FW %d, HW %d", pName,
                                                  pPropData->m_TypeName,
                                                  pPropData->m_TypeNumber,
                                                  pPropData->m_FirmwareVersion,
                                                  pPropData->m_HardwareRevision);
    }

    return Value;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesReportDriverErrors -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPropertiesReportDriverErrors(DtPropertyData* pPropData)
{
    DtStatus  Status = DT_STATUS_OK;

    // Check for errors
    if (pPropData->m_PropertyNotFoundCounter > 0)
        Status = DT_STATUS_CONFIG_ERROR;

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtResetPropertiesNotFoundCounter -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtResetPropertiesNotFoundCounter(DtPropertyData* pPropData)
{
    pPropData->m_PropertyNotFoundCounter = 0;
}
