//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtProperties.c *#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//
// Driver common - Device and port property get funcions.
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


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtDrvCommon.h>

// Generated property store
extern const Int  DtPropertyStoreCount;
extern const DtPropertyStore  DtPropertyStores[];

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
        if (DtPropertyStores[Index].m_TypeNumber == pPropData->m_TypeNumber)
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
DtStatus  DtPropertiesFind(
    DtPropertyData* pPropData,
    const char* pName,
    Int PortIndex,
    const DtProperty**  ppProperty)
{
    DtStatus  Status = DT_STATUS_OK;
    const DtPropertyStore*  pStore = (DtPropertyStore*)pPropData->m_pPropertyStore;
    const DtPropertyHashSet*  pHashSets =
                                         (DtPropertyHashSet*)pStore->m_pPropertyHashSets;
    UInt  HashSetCount = pStore->m_PropertyHashSetCount;
    Int  FwVersion = pPropData->m_FirmwareVersion;
    Int  HwRevision = pPropData->m_HardwareRevision;
    
    UInt  Hash = 0;
    Int  Index;
    Bool  PropertyNameFound = FALSE;
    const DtPropertyHashSet*  pHashSet = NULL;
    Int  PropertyCount = 0;
    *ppProperty = NULL;

    // Determine the name hash
    Hash = DtDjb2(pName) % HashSetCount;

    // Get correct hash set (if exists)
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
                    // Check minimal firmware version
                    if (FwVersion >= pProp->m_MinFw)
                    {
                        // Check minimal hardware version
                        if (HwRevision >= pProp->m_MinHw)
                        {
                            *ppProperty = pProp;

                            // We can stop here since the parser has ordened each
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
                DtDbgOut(ERR, PROP, "PropertyName %s is not found at all for %s-%d",
                                   pName, pPropData->m_TypeName, pPropData->m_TypeNumber);
        }
            
        // Check if the property was found
        if (*ppProperty == NULL)
        {
            Status = DT_STATUS_NOT_FOUND;
            DtDbgOut(AVG, PROP, "Failed to find property %s for %s-%d, FW %d, HW %d", 
                                                  pName, 
                                                  pPropData->m_TypeName,
                                                  pPropData->m_TypeNumber,
                                                  pPropData->m_FirmwareVersion,
                                                  pPropData->m_HardwareRevision);
        }
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtPropertiesGet(
    DtPropertyData*  pPropData,
    const char*  pName,
    Int  PortIndex,
    DtPropertyValue*  pValue,
    DtPropertyValueType*  pType,
    DtPropertyScope*  pScope)
{
    DtStatus  Status = DT_STATUS_OK;
    const DtPropertyStore*  pStore = (DtPropertyStore*)pPropData->m_pPropertyStore;
    const DtPropertyHashSet*  pHashSets =
                                         (DtPropertyHashSet*)pStore->m_pPropertyHashSets;

    // Lookup the property
    const  DtProperty*  pProperty = NULL;
    Status = DtPropertiesFind(pPropData, pName, PortIndex, &pProperty);
    
    // Get pProp details
    if (DT_SUCCESS(Status) && pProperty!=NULL)
    {
        // Copy property info
        *pValue = pProperty->m_Value;
        *pType = pProperty->m_Type;
        *pScope = pProperty->m_Scope;
        DtDbgOut(MAX, PROP, "Found property %s for %s-%d, FW %d, HW %d. Value: 0x%08x%08x, "
                                                   "Type:%i, Scope:%i", pName,
                                                   pPropData->m_TypeName,
                                                   pPropData->m_TypeNumber,
                                                   pPropData->m_FirmwareVersion,
                                                   pPropData->m_HardwareRevision,
                                                   (UInt32)(*pValue>>32),(UInt32)*pValue, 
                                                   *pType, *pScope);
    } 
    else if (Status == DT_STATUS_NOT_FOUND)
    {
        if (DtAnsiCharArrayStartsWith(pName, "CAP_"))
        {
            // Generate capability info
            *pValue = 0;
            *pType = PROPERTY_VALUE_TYPE_BOOL;
            *pScope = PROPERTY_SCOPE_DTAPI | PROPERTY_SCOPE_DRIVER;

            /* Pretend that propery was found */
            Status = DT_STATUS_OK;
            DtDbgOut(MAX, PROP, "Assume property %s is a capability that is not "
                                                                     "supported.", pName);
        }
    }

    return Status;
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
    DtPropertyScope*  pScope)
{
    Int  i=0;
    DtStatus  Status = DT_STATUS_OK;
    const DtPropertyStore*  pStore = (DtPropertyStore*)pPropData->m_pPropertyStore;
    const DtPropertyHashSet*  pHashSets =
                                         (DtPropertyHashSet*)pStore->m_pPropertyHashSets;

    // Lookup the property
    const  DtProperty*  pProperty = NULL;
    Status = DtPropertiesFind(pPropData, pName, PortIndex, &pProperty);
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
    Int  TypeNumber,
    Int  HwRev,
    Int  FwVer,
    const char* pName,
    Int PortIndex,
    DtPropertyValue* pValue,
    DtPropertyValueType* pType,
    DtPropertyScope* pScope)
{
    DtPropertyData  PropData;
    DtStatus  Status = DT_STATUS_OK;
        
    // Init property data structure
    PropData.m_TypeNumber = TypeNumber;
    PropData.m_FirmwareVersion = FwVer;
    PropData.m_HardwareRevision = HwRev;

    Status = DtPropertiesInit(&PropData);
    if (!DT_SUCCESS(Status))
        return Status;

    // Now find the property
    return DtPropertiesGet(&PropData, pName, PortIndex, pValue, pType, pScope);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPropertiesStrGetForType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtPropertiesStrGetForType(
    Int  TypeNumber,
    Int  HwRev,
    Int  FwVer,
    const char* pName,
    Int PortIndex,
    char* pStr,
    DtPropertyScope* pScope)
{
    DtPropertyData  PropData;
    DtStatus  Status = DT_STATUS_OK;
        
    // Init property data structure
    PropData.m_TypeNumber = TypeNumber;
    PropData.m_FirmwareVersion = FwVer;
    PropData.m_HardwareRevision = HwRev;

    Status = DtPropertiesInit(&PropData);
    if (!DT_SUCCESS(Status))
        return Status;

    // Now find the property
    return DtPropertiesStrGet(&PropData, pName, PortIndex, pStr, pScope);
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
    Status = DtPropertiesGet(pPropData, pName, Port, &Value, &Type, &Scope);
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
