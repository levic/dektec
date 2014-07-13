//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtNonVolatileSettings.c *#*#*#*#*#* (C) 2010-2012 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2012 DekTec Digital Video B.V.
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtSal.h>

#ifndef SKIP_NONVOL
#ifdef WINBUILD

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Windows Private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//-.-.-.-.-.-.-.-.-.-.-.-.- PathAppendSettingsSerialPortCategory -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  PathAppendSettingsSerialPortCategory(
    DtString*  pPath,
    UInt64  DvcSerial,
    Int  Port,
    Char*  pCategory)
{
    DtStatus  Status = DT_STATUS_OK;
    DT_STRING_DECL(SettingsStr, "Settings\\");
    DT_STRING_DECL(PortStr, "\\Port");
        
    // Registry path starts with <\Settings\>
    Status = DtStringAppendDtString(pPath, &SettingsStr);
    if (DT_SUCCESS(Status))
    {
        // Convert serial to unicode string and append to path
        Status = DtStringUInt64ToDtStringAppend(pPath, 10, DvcSerial);
    }

    if (DT_SUCCESS(Status))
    {
        // Append port if provided
        if (Port >= 0)
        {
            Status = DtStringAppendDtString(pPath, &PortStr);
            if (DT_SUCCESS(Status))
            {
                // Convert Port to unicode string
                Status = DtStringUIntegerToDtStringAppend(pPath, 10, Port);
            }
        }
        
        if (DT_SUCCESS(Status))
        {
            if (pCategory != NULL) 
            {
                // Append category
                Status = DtStringAppendChars(pPath, "\\");
                if (DT_SUCCESS(Status))
                    Status = DtStringAppendChars(pPath, pCategory);
            }
        }
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.- PathAppendSettingsSerialDeviceCategory -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  PathAppendSettingsSerialDeviceCategory(
    DtString*  pPath,
    UInt64  DvcSerial,
    Char*  pCategory)
{
    DtStatus  Status = DT_STATUS_OK;
    DT_STRING_DECL(SettingsStr, "Settings\\");
    DT_STRING_DECL(DeviceStr, "\\Device");
        
    // Registry path starts with <\Settings\>
    Status = DtStringAppendDtString(pPath, &SettingsStr);
    if (DT_SUCCESS(Status))    
        // Convert serial to unicode string and append to path
        Status = DtStringUInt64ToDtStringAppend(pPath, 10, DvcSerial);

    if (DT_SUCCESS(Status))
    {
        Status = DtStringAppendDtString(pPath, &DeviceStr);
        if (DT_SUCCESS(Status))
        {
            if (pCategory != NULL) 
            {
                // Append category
                Status = DtStringAppendChars(pPath, "\\");
                if (DT_SUCCESS(Status))
                    Status = DtStringAppendChars(pPath, pCategory);
            }
        }
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- CheckAndCreateRegistryPath -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  CheckAndCreateRegistryPath(
    WDFDRIVER  Driver,
    DtString*  pItemPath)
{
    DtStringChar*  pRegistryPath;
    DtString  RegistryPath;
    DtString  FullKeyName;
    UInt  PathLength;
    DtStatus  Status;
    Int  i;
    DT_STRING_DECL(ParamItemName, "\\Parameters");

    // Build the full path
    pRegistryPath = WdfDriverGetRegistryPath(Driver);
    PathLength = wcslen(pRegistryPath); 

    DT_STRING_INIT_CONST(RegistryPath, pRegistryPath, PathLength);

    // Allocate a new DtString buffer for the complete path inclusive a '\0' character and
    // extra '\'
    if (!DT_SUCCESS(DtStringAlloc(&FullKeyName, PathLength+
                                                  DtStringGetStringLength(&ParamItemName)+
                                                   DtStringGetStringLength(pItemPath)+2)))
        return STATUS_NO_MEMORY;

    DtStringAppendDtString(&FullKeyName, &RegistryPath);
    DtStringAppendDtString(&FullKeyName, &ParamItemName);

    if (RtlCheckRegistryKey(RTL_REGISTRY_ABSOLUTE, FullKeyName.Buffer) != STATUS_SUCCESS)
        RtlCreateRegistryKey(RTL_REGISTRY_ABSOLUTE, FullKeyName.Buffer);

    Status = DT_STATUS_OK;

    i = 1;
    // Get all subitems from pItemPath and check if the registry entry exist.
    // If not, create the registry entry.
    // This function is needed, because Wuindows only allows us to create one registry entry
    // at a time and not a complete path.
    while (Status == DT_STATUS_OK)
    {
        DtStringAppendChars(&FullKeyName, "\\");
        Status = DtStringAppendSubstring(&FullKeyName, pItemPath, i, '\\');
        if (DT_SUCCESS(Status))
        {
            if (RtlCheckRegistryKey(RTL_REGISTRY_ABSOLUTE, FullKeyName.Buffer) 
                                                                        != STATUS_SUCCESS)
                RtlCreateRegistryKey(RTL_REGISTRY_ABSOLUTE, FullKeyName.Buffer);
        }
        i++;
    }
    DtStringFree(&FullKeyName);
    return DT_STATUS_OK;    
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DriverParametersKeyDelete -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS  DriverParametersKeyDelete(
    WDFDRIVER  Driver,
    DtString*  pKeyName)
{
    NTSTATUS  NtStatus;
    WDFKEY  ParametersKey;
    WDFKEY  Key = NULL;
    
    DT_ASSERT(KeGetCurrentIrql()<=PASSIVE_LEVEL);

    // Open the drivers parameters key (under services)
    NtStatus = WdfDriverOpenParametersRegistryKey(Driver, KEY_WRITE,
                                                WDF_NO_OBJECT_ATTRIBUTES, &ParametersKey);
    if (!NT_SUCCESS(NtStatus))
    {
        DtDbgOut(ERR, SAL, "WdfDriverOpenParametersRegistryKey failed. Error: 0x%x", 
                                                                                NtStatus);
        return NtStatus;
    }

    // Open subkey
    NtStatus = WdfRegistryOpenKey(ParametersKey, pKeyName, KEY_WRITE, 
                                                          WDF_NO_OBJECT_ATTRIBUTES, &Key);
    if (!NT_SUCCESS(NtStatus))
    {
        if (NtStatus == STATUS_OBJECT_NAME_NOT_FOUND)
            DtDbgOut(MAX, SAL, "WdfRegistryOpenKey error:'STATUS_OBJECT_NAME_NOT_FOUND'");
        else
            DtDbgOut(ERR, SAL, "WdfRegistryOpenKey failed. Error: 0x%x", NtStatus);
    }

    if (NT_SUCCESS(NtStatus))
    {
        // Delete the key
        NtStatus = WdfRegistryRemoveKey(Key);
        if (!NT_SUCCESS(NtStatus))
            DtDbgOut(ERR, SAL, "WdfRegistryRemoveKey failed. Error: 0x%x", NtStatus);
        else 
            Key = NULL;
    }
    if (Key != NULL)
        WdfRegistryClose(Key);
    WdfRegistryClose(ParametersKey);
    return NtStatus;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DriverParametersSubKeyDelete -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS  DriverParametersSubKeyDelete(
    WDFDRIVER  Driver,
    DtString*  pKeyName)
{
    NTSTATUS  NtStatus = STATUS_SUCCESS;
    DtStringChar*  pRegistryPath;
    DtString  RegistryPath;
    DtString  FullKeyName;
    UInt  PathLength;
    HANDLE  hKey;
    OBJECT_ATTRIBUTES  ObjectAttributes;
    KEY_BASIC_INFORMATION*  pKeyInfo;
    ULONG  Size;
    ULONG  ResultSize;
    Int  Index;
    
    DT_STRING_DECL(ParamItemName, "\\Parameters\\");

    DT_ASSERT(KeGetCurrentIrql()<=PASSIVE_LEVEL);

    // Build the full path
    pRegistryPath = WdfDriverGetRegistryPath(Driver);
    PathLength = wcslen(pRegistryPath); 

    DT_STRING_INIT_CONST(RegistryPath, pRegistryPath, PathLength);

    // Allocate struct for key information result
    Size = sizeof(KEY_BASIC_INFORMATION)+100;
    pKeyInfo = DtMemAllocPool(DtPoolNonPaged, Size, SAL_TAG);
    if (pKeyInfo == NULL)
        return STATUS_NO_MEMORY;

    // Allocate a new DtString buffer for the complete path inclusive a '\0' character
    // and extra '\\'
    if (!DT_SUCCESS(DtStringAlloc(&FullKeyName, PathLength+
                                                  DtStringGetStringLength(&ParamItemName)+
                                                  DtStringGetStringLength(pKeyName)+
                                                  100+1+1)))
    {
        DtMemFreePool(pKeyInfo, SAL_TAG);
        return STATUS_NO_MEMORY;
    }

    DtStringAppendDtString(&FullKeyName, &RegistryPath);
    DtStringAppendDtString(&FullKeyName, &ParamItemName);
    DtStringAppendDtString(&FullKeyName, pKeyName);

    // Initialize key to open
    InitializeObjectAttributes(&ObjectAttributes, &FullKeyName, OBJ_KERNEL_HANDLE, NULL,
                                                                                    NULL);
    
    NtStatus = ZwOpenKey(&hKey, KEY_ENUMERATE_SUB_KEYS , &ObjectAttributes);
    
    if (NT_SUCCESS(NtStatus)) 
    {
        Index = 0;
        NtStatus = STATUS_SUCCESS;
        // Enumerate all keys
        while (NtStatus != STATUS_NO_MORE_ENTRIES)
        {
            NtStatus = ZwEnumerateKey(hKey, Index, KeyBasicInformation, pKeyInfo, Size,
                                                                             &ResultSize);
            if (NT_SUCCESS(NtStatus))
            {
                DtString SubKey;

                // Build key to delete
                pKeyInfo->Name[pKeyInfo->NameLength/2] = L'\0';
                DT_STRING_INIT_CONST(SubKey, pKeyInfo->Name, ((USHORT)pKeyInfo->NameLength/2));

                DtStringClear(&FullKeyName);
                DtStringAppendDtString(&FullKeyName, pKeyName);
                DtStringAppendChars(&FullKeyName, "\\");
                DtStringAppendDtString(&FullKeyName, &SubKey);
                
                DtDbgOut(MAX, SAL, "Delete SubKey %S.", FullKeyName.Buffer);
                NtStatus = DriverParametersKeyDelete(Driver, &FullKeyName);
                if (!NT_SUCCESS(NtStatus))
                    DtDbgOut(ERR, SAL, "Error deleting SubKey %S. Error: %x", 
                                                            FullKeyName.Buffer, NtStatus);
            }
            // In case deletion failed, skip this entry
            if (!NT_SUCCESS(NtStatus)) 
                Index++;
        }
        NtStatus = ZwDeleteKey(hKey);
        ZwClose(hKey);
    }
    DtMemFreePool(pKeyInfo, SAL_TAG);
    DtStringFree(&FullKeyName);
    return NtStatus;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DriverParametersKeyRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
NTSTATUS  DriverParametersKeyRead(
    WDFDRIVER  Driver,
    DtString*  pKeyName,
    DtString*  pValueName,
    Int64*  pBinValue,
    DtString*  pStrValue)
{
    NTSTATUS  NtStatus;
    WDFKEY  ParametersKey;
    WDFKEY  Key = NULL;
    WDFSTRING WdfString = NULL;   
    DtString  RegStrValue;

    DT_ASSERT(KeGetCurrentIrql()<=PASSIVE_LEVEL);


    // Only one value can be requested
    DT_ASSERT((pBinValue!=NULL && pStrValue==NULL)||(pBinValue==NULL && pStrValue!=NULL));

    // Open the drivers parameters key (under services)
    NtStatus = WdfDriverOpenParametersRegistryKey(Driver, KEY_READ,
                                                WDF_NO_OBJECT_ATTRIBUTES, &ParametersKey);
    if (!NT_SUCCESS(NtStatus))
    {
        DtDbgOut(ERR, SAL, "WdfDriverOpenParametersRegistryKey failed. Error: 0x%x", 
                                                                                NtStatus);
        return NtStatus;
    }

    // Open subkey
    NtStatus = WdfRegistryOpenKey(ParametersKey, pKeyName, KEY_READ, 
                                                          WDF_NO_OBJECT_ATTRIBUTES, &Key);
    if (!NT_SUCCESS(NtStatus))
    {
        if (NtStatus == STATUS_OBJECT_NAME_NOT_FOUND)
            DtDbgOut(MAX, SAL, "WdfRegistryOpenKey error:'STATUS_OBJECT_NAME_NOT_FOUND'");
        else
            DtDbgOut(ERR, SAL, "WdfRegistryOpenKey failed. Error: 0x%x", NtStatus);
    }

    if (NT_SUCCESS(NtStatus))
    {
        // Read string or binary value
        if (pStrValue != NULL)
        {
            // Create Wdf string
            NtStatus = WdfStringCreate(NULL, WDF_NO_OBJECT_ATTRIBUTES, &WdfString);

            // Read string from registry
            if (NT_SUCCESS(NtStatus))
                NtStatus = WdfRegistryQueryString(Key, pValueName, WdfString);

            // Convert WdfString to DtString
            if (NT_SUCCESS(NtStatus))
                WdfStringGetUnicodeString(WdfString, &RegStrValue);

            // Make a copy of the string
            if (NT_SUCCESS(NtStatus))
                NtStatus = DtStringClear(pStrValue);
            if (NT_SUCCESS(NtStatus))
                NtStatus = DtStringAppendDtString(pStrValue, &RegStrValue);
        }
        else
            NtStatus = WdfRegistryQueryValue(Key, pValueName, sizeof(Int64), pBinValue,
                                                                              NULL, NULL);

        if (!NT_SUCCESS(NtStatus))
        {
            if (NtStatus == STATUS_OBJECT_NAME_NOT_FOUND)
                DtDbgOut(MAX, SAL, "WdfRegistryQueryValue error:"
                                                        "'STATUS_OBJECT_NAME_NOT_FOUND'");
            else
                DtDbgOut(ERR, SAL, "WdfRegistryQueryValue failed. Error: 0x%x", NtStatus);
        }
    }
    if (Key != NULL)
        WdfRegistryClose(Key);
    WdfRegistryClose(ParametersKey);
    return NtStatus;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DriverParametersKeyWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
NTSTATUS  DriverParametersKeyWrite(
    WDFDRIVER  Driver,
    DtString*  pKeyName,
    DtString*  pValueName,
    Int64  BinValue,
    DtString*  pStrValue)
{
    NTSTATUS  NtStatus;
    WDFKEY  ParametersKey;
    WDFKEY  Key = NULL;
    WDFSTRING WdfString;
    
    DT_ASSERT(KeGetCurrentIrql()<=PASSIVE_LEVEL);
   
    // Check if the registry path already exists. If not, create the registry path.
    if (!DT_SUCCESS(CheckAndCreateRegistryPath(Driver, pKeyName)))
        return STATUS_UNSUCCESSFUL;
       
    // Open the drivers parameters key (under services)
    NtStatus = WdfDriverOpenParametersRegistryKey(Driver, KEY_WRITE,
                                                WDF_NO_OBJECT_ATTRIBUTES, &ParametersKey);
    if (!NT_SUCCESS(NtStatus))
    {
        DtDbgOut(ERR, SAL, "WdfDriverOpenParametersRegistryKey failed. Error: 0x%x", 
                                                                                NtStatus);
        return NtStatus;
    }

    // Open the key (including part of path)
    NtStatus = WdfRegistryOpenKey(ParametersKey, pKeyName, KEY_WRITE, 
                                                          WDF_NO_OBJECT_ATTRIBUTES, &Key);
    if (!NT_SUCCESS(NtStatus))
        DtDbgOut(ERR, SAL, "WdfRegistryOpenKey failed. Error: 0x%x", NtStatus);

    if (NT_SUCCESS(NtStatus))
    {

        // Write string or binary value
        if (pStrValue != NULL)
        {
            NtStatus = WdfStringCreate(pStrValue, WDF_NO_OBJECT_ATTRIBUTES, &WdfString);

            if (NT_SUCCESS(NtStatus))
                NtStatus = WdfRegistryAssignString(Key, pValueName, WdfString);
        }
        else
            NtStatus = WdfRegistryAssignValue(Key, pValueName, REG_QWORD, sizeof(Int64),
                                                                               &BinValue);
        if (!NT_SUCCESS(NtStatus))
            DtDbgOut(ERR, SAL, "WdfRegistryAssignValue failed. Error: 0x%x", NtStatus);
    }
    
    if (Key != NULL)
        WdfRegistryClose(Key);
    WdfRegistryClose(ParametersKey);
    return NtStatus;
}

#else

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Linux Private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.


#endif

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtNonVolatileSettingsDelete -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtNonVolatileSettingsDelete(
    DtDrvObject*  pDrvObj,
    UInt64  DvcSerial,
    UInt  NumPorts)
{
    DtStatus  Status = DT_STATUS_OK;

#ifdef WINBUILD

    NTSTATUS  NtStatus;
    DtString  RegKeyName;
    Int  i;

    // Allocate RegKeyName buffer
    Status = DtStringAlloc(&RegKeyName, 150);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Remove all port settings
    for (i = 0; i < (Int)NumPorts; i++)
    {
        Status = DtStringClear(&RegKeyName);
        if (!DT_SUCCESS(Status))
        {
            DtStringFree(&RegKeyName);
            return Status;
        }
            
        Status = PathAppendSettingsSerialPortCategory(&RegKeyName, DvcSerial, i, NULL);
        if (!DT_SUCCESS(Status))
        {
            DtStringFree(&RegKeyName);
            return Status;
        }

        // First delete all subitems of PortX (=Categories)
        NtStatus = DriverParametersSubKeyDelete(pDrvObj->m_WdfDriver, &RegKeyName);
    
        // Delete registry item: PortX
        NtStatus = DriverParametersKeyDelete(pDrvObj->m_WdfDriver, &RegKeyName);
    }

    // Remove the device settings
    Status = DtStringClear(&RegKeyName);
    if (!DT_SUCCESS(Status))
    {
        DtStringFree(&RegKeyName);
        return Status;
    }

    Status = PathAppendSettingsSerialDeviceCategory(&RegKeyName, DvcSerial, NULL);
    if (!DT_SUCCESS(Status))
    {
        DtStringFree(&RegKeyName);
        return Status;
    }

    // First delete all subitems of device(=Categories)
    NtStatus = DriverParametersSubKeyDelete(pDrvObj->m_WdfDriver, &RegKeyName);

    // Delete registry item: Device
    NtStatus = DriverParametersKeyDelete(pDrvObj->m_WdfDriver, &RegKeyName);


    // Now remove the serial number
    Status = DtStringClear(&RegKeyName);
    if (!DT_SUCCESS(Status))
    {
        DtStringFree(&RegKeyName);
        return Status;
    }

    // Create path without port number
    Status = PathAppendSettingsSerialPortCategory(&RegKeyName, DvcSerial, -1, NULL);
    if (!DT_SUCCESS(Status))
    {
        DtStringFree(&RegKeyName);
        return Status;
    }
        
    // Delete registry item: serial number
    NtStatus = DriverParametersKeyDelete(pDrvObj->m_WdfDriver, &RegKeyName);
    if (!NT_SUCCESS(NtStatus))
    {
        DtStringFree(&RegKeyName);
        return DT_STATUS_FAIL;
    }
    DtStringFree(&RegKeyName);
#else
    // Not yet implemented in Linux.
    Status = DT_STATUS_OK;
#endif

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtNonVolatileManufSettingsRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read a registry key from the manufacturing section, e.g. from
// ../Dta/Parameters/Settings/Manuf/2137/ForcedHardwareRevision
//
DtStatus DtNonVolatileManufSettingsRead(
    DtDrvObject* pSalDrvObj, 
    Int Type, 
    Char* pName, 
    Int64* pBinValue)
{
    DtStatus  Status = DT_STATUS_OK;

#ifdef WINBUILD

    // Windows uses the registry to store non volatile settings
    NTSTATUS  NtStatus;
    DtString  RegKeyName;
    DtString  ValueName;

    // Allocate RegKeyName buffer
    Status = DtStringAlloc(&RegKeyName, 150);
    if (DT_SUCCESS(Status))
    {
        Status = DtStringAlloc(&ValueName, 40);
        if (DT_SUCCESS(Status))
        {
            // Create registry path string
            DT_STRING_DECL(SettingsStr, "Settings\\Manuf\\");
            
            // Registry path starts with <\Settings\Manuf\>
            Status = DtStringAppendDtString(&RegKeyName, &SettingsStr);
            if (DT_SUCCESS(Status) && Type!=-1)
                // Convert type to unicode string and append to path
                Status = DtStringUIntegerToDtStringAppend(&RegKeyName, 10, (UInt)(Type));
            
            if (DT_SUCCESS(Status))
            {
                // Convert value name to DtString
                Status = DtStringAppendChars(&ValueName, pName);
                if (DT_SUCCESS(Status))
                {
                    // Read register binary value
                    NtStatus = DriverParametersKeyRead(pSalDrvObj->m_WdfDriver, 
                                                &RegKeyName, &ValueName, pBinValue, NULL);
                    
                    if (!NT_SUCCESS(NtStatus))
                    {
                        Status = DT_STATUS_FAIL;

                        // Be more specific if object name was not found
                        if (NtStatus == STATUS_OBJECT_NAME_NOT_FOUND)
                            Status = DT_STATUS_NOT_FOUND;
                    }
                }
            }

            // Free ValueName
            DtStringFree(&ValueName);
        }
        
        // Free RegKeyName
        DtStringFree(&RegKeyName);
    }

#else
    // Not yet implemented in Linux.
    Status = DT_STATUS_NOT_FOUND;
#endif

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtNonVolatileSettingsRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtNonVolatileSettingsRead(
    DtDrvObject*  pDrvObj,
    UInt64  DvcSerial,
    Int  Port,
    Char*  pCategory,
    Char*  pName,
    Int64*  pBinValue,
    Char*  pStrValue,
    UInt  StrSize)
{
    DtStatus  Status = DT_STATUS_OK;

#ifdef WINBUILD

    // Windows uses the registry to store non volatile settings
    NTSTATUS  NtStatus;
    DtString  RegKeyName;
    DtString  ValueName;

    // Allocate RegKeyName buffer
    Status = DtStringAlloc(&RegKeyName, 150);
    if (DT_SUCCESS(Status))
    {
        Status = DtStringAlloc(&ValueName, 20);
        if (DT_SUCCESS(Status))
        {
            // Create registry path string
            if (Port == -1)
                Status = PathAppendSettingsSerialDeviceCategory(&RegKeyName, 
                                                                    DvcSerial, pCategory);
            else
                Status = PathAppendSettingsSerialPortCategory(&RegKeyName, 
                                                              DvcSerial, Port, pCategory);

            if (DT_SUCCESS(Status))
            {
                // Convert value name to DtString
                Status = DtStringAppendChars(&ValueName, pName);
                if (DT_SUCCESS(Status))
                {
                    if (pStrValue!=NULL)
                    {
                        DtString  StrValue;
                        Status = DtStringAlloc(&StrValue, (UInt16)StrSize);
                        if (DT_SUCCESS(Status))
                        {
                            // Write register string value
                            NtStatus = DriverParametersKeyRead(pDrvObj->m_WdfDriver, 
                                                &RegKeyName, &ValueName, NULL, &StrValue);
                            // Convert value to char string 
                            if (DT_SUCCESS(NtStatus))
                                Status = DtStringToCharString(&StrValue, pStrValue, 
                                                                                StrSize);
                            // Free ValueName
                            DtStringFree(&StrValue);
                        }
                    }
                    else
                    {
                        // Read register binary value
                        NtStatus = DriverParametersKeyRead(pDrvObj->m_WdfDriver, 
                                                &RegKeyName, &ValueName, pBinValue, NULL);
                    }

                    if (!NT_SUCCESS(NtStatus))
                    {
                        Status = DT_STATUS_FAIL;

                        // Be more specific if object name was not found
                        if (NtStatus == STATUS_OBJECT_NAME_NOT_FOUND)
                            Status = DT_STATUS_NOT_FOUND;
                    }
                }
            }

            // Free ValueName
            DtStringFree(&ValueName);
        }
        
        // Free RegKeyName
        DtStringFree(&RegKeyName);
    }

#else

    // Linux can use config files, but they should not be read / write from a kernel
    // module --> http://www.linuxjournal.com/article/8110
    // Maybe use hotplug event in user space to have helper application to perform
    // driver load and do the initial settings. Helper application can be DtapiService
    // or a simple script.

    Status = DT_STATUS_NOT_FOUND;

#endif

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtNonVolatileSettingsWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtNonVolatileSettingsWrite(
    DtDrvObject*  pDrvObj,
    UInt64  DvcSerial,
    Int  Port,
    Char*  pCategory,
    Char*  pName,
    Int64  BinValue,
    Char*  pStrValue)
{
    DtStatus  Status = DT_STATUS_OK;

#ifdef WINBUILD

    // Windows uses the registry to store non volatile settings
    NTSTATUS  NtStatus;
    DtString  RegKeyName;
    DtString  ValueName;
    
    // Allocate RegKeyName buffer
    Status = DtStringAlloc(&RegKeyName, 150);
    if (DT_SUCCESS(Status))
    {
        Status = DtStringAlloc(&ValueName, 50);
        if (DT_SUCCESS(Status))
        {
            // Create registry path string
            Status = PathAppendSettingsSerialPortCategory(&RegKeyName, DvcSerial, Port, 
                                                                               pCategory);
            if (DT_SUCCESS(Status))
            {
                // Convert value name to DtString
                Status = DtStringAppendChars(&ValueName, pName);
                if (DT_SUCCESS(Status))
                {
                    if (pStrValue!=NULL)
                    {
                        DtString  StrValue;
                        Status = DtStringAlloc(&StrValue, 50);
                        if (DT_SUCCESS(Status))
                        {
                            // Convert value to DtString
                            Status = DtStringAppendChars(&StrValue, pStrValue);
                            if (DT_SUCCESS(Status))
                            {
                                // Write register string value
                                NtStatus = DriverParametersKeyWrite(pDrvObj->m_WdfDriver, 
                                                  &RegKeyName, &ValueName, -1, &StrValue);
                            }
                            // Free ValueName
                            DtStringFree(&StrValue);
                        }
                    }
                    else
                    {
                        // Write register binary value
                        NtStatus = DriverParametersKeyWrite(pDrvObj->m_WdfDriver, 
                                                 &RegKeyName, &ValueName, BinValue, NULL);
                    }

                    if (!NT_SUCCESS(NtStatus))
                        Status = DT_STATUS_FAIL;
                }
            }

            // Free ValueName
            DtStringFree(&ValueName);
        }

        // Free RegKeyName
        DtStringFree(&RegKeyName);
    }

#else

    // Linux can use config files, but they should not be read / write from a kernel
    // module --> http://www.linuxjournal.com/article/8110
    // Maybe use hotplug event in user space to have helper application to perform
    // driver load and do the initial settings. Helper application can be DtapiService
    // or a simple script.

    Status = DT_STATUS_OK;

#endif

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtNonVolatileSettingsStringRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtNonVolatileSettingsStringRead(DtDrvObject* pSalDrvObj, UInt64 DvcSerial, 
                          Int Port, Char* pCategory, Char* pName, Char* pValue, UInt Size)
{
    return DtNonVolatileSettingsRead(pSalDrvObj, DvcSerial, Port, pCategory, pName, 
                                                                      NULL, pValue, Size);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtNonVolatileSettingsStringWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtNonVolatileSettingsStringWrite(DtDrvObject* pSalDrvObj, UInt64 DvcSerial, 
                                    Int Port, Char* pCategory, Char* pName, Char* pValue)
{
    return DtNonVolatileSettingsWrite(pSalDrvObj, DvcSerial, Port, pCategory, pName, 
                                                                              -1, pValue);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtNonVolatileSettingsValueRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtNonVolatileSettingsValueRead(DtDrvObject* pSalDrvObj, UInt64 DvcSerial, 
                                   Int Port, Char* pCategory, Char* pName, Int64* pValue)
{
    return DtNonVolatileSettingsRead(pSalDrvObj, DvcSerial, Port, pCategory, pName, 
                                                                         pValue, NULL, 0);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtNonVolatileSettingsValueWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtNonVolatileSettingsValueWrite(DtDrvObject* pSalDrvObj, UInt64 DvcSerial, 
                                     Int Port, Char* pCategory, Char* pName, Int64 Value)
{
    return DtNonVolatileSettingsWrite(pSalDrvObj, DvcSerial, Port, pCategory, pName, 
                                                                            Value, NULL);
}



#endif // SKIP_NONVOL
