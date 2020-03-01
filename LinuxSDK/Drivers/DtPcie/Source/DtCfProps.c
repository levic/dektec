//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCfProps.c *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
#include "DtCfProps.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfProps implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtCfProps function
#define CF_PROPS_DEFAULT_PRECONDITIONS(pCf)      \
    DT_ASSERT(pCf!=NULL && pCf->m_Size==sizeof(DtCfProps))

// Helper macro to cast a DtDf* to a DtCfProps*
#define CF_PROPS    ((DtCfProps*)pCf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
DtStatus  DtCfProps_Init(DtDf*);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfProps - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtCfProps_Close(DtDf*  pCf)
{
    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    // Let base function perform final clean-up
    DtDf_Close(pCf);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtCfProps*  DtCfProps_Open(DtCore*  pCore, const char*  pRole, Int  Instance, 
                                                              Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    
    // Init open parameters
    DT_CF_PROPS_INIT_ID(Id, pRole, Instance, Uuid);
    DT_CF_INIT_OPEN_PARAMS(OpenParams, DtCfProps, Id, DT_FUNC_TYPE_CF_PROPS,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtCfProps_Close;
    OpenParams.m_InitFunc = DtCfProps_Init;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtCfProps*)DtDf_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps_Find -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfProps_Find(
    DtCfProps*  pCf,
    const char*  pName, 
    DtPropertyFilterCriteria  Flt,
    const DtProperty**  ppProperty)
{
    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    return DtPropertiesFind(&pCf->m_PropData, pName, Flt.m_PortIndex, ppProperty,
                                       Flt.m_DtapiMaj, Flt.m_DtapiMin, Flt.m_DtapiBugfix);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps_Get -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfProps_Get(
    DtCfProps*  pCf,
    const char*  pName, 
    DtPropertyFilterCriteria  Flt,
    DtPropertyValue*  pValue,
    DtPropertyValueType*  pType,
    DtPropertyScope*  pScope,
    Bool  IsOptional,
    DtPropertyValue  Default)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);
    
    // Get the property
    Status = DtPropertiesGet(&pCf->m_PropData, pName, Flt.m_PortIndex,
                                       pValue, pType, pScope, 
                                       Flt.m_DtapiMaj, Flt.m_DtapiMin, Flt.m_DtapiBugfix);
    if (!DT_SUCCESS(Status))
    {
        // If optional set to default
        if (IsOptional)
            *pValue = Default;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps_GetForType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfProps_GetForType(
    DtCfProps*  pCf,
    const char*  pTypeName,
    Int  TypeNumber,
    Int  SubType,
    DtPropertyFilterCriteria  Flt,
    const char*  pName,
    DtPropertyValue*  pValue,
    DtPropertyValueType*  pType,
    DtPropertyScope*  pScope)
{
    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    return DtPropertiesGetForType(pTypeName, TypeNumber, SubType, Flt.m_HwRev, 
                                       Flt.m_FwVersion, Flt.m_FwVariant,
                                       pName, Flt.m_PortIndex, pValue, pType, pScope, 
                                       Flt.m_DtapiMaj, Flt.m_DtapiMin, Flt.m_DtapiBugfix);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps_GetStr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfProps_GetStr(
    DtCfProps*  pCf,
    const char*  pName, 
    DtPropertyFilterCriteria  Flt,
    char*  pStr,
    DtPropertyScope*  pScope)
{
    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    return DtPropertiesStrGet(&pCf->m_PropData, pName, Flt.m_PortIndex, pStr, 
                               pScope, Flt.m_DtapiMaj, Flt.m_DtapiMin, Flt.m_DtapiBugfix);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps_GetStrForType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfProps_GetStrForType(
    DtCfProps*  pCf, 
    const char*  pTypeName, 
    Int  TypeNumber, 
    Int  SubType, 
    DtPropertyFilterCriteria  Flt,
    const char*  pName, 
    char*  pStr,
    DtPropertyScope*  pScope)
{
    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    return DtPropertiesStrGetForType(pTypeName, TypeNumber, SubType, Flt.m_HwRev, 
                                       Flt.m_FwVersion, Flt.m_FwVariant,
                                       pName, Flt.m_PortIndex, pStr, pScope, 
                                       Flt.m_DtapiMaj, Flt.m_DtapiMin, Flt.m_DtapiBugfix);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps_GetTable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfProps_GetTable(
    DtCfProps*  pCf, 
    const char*  pTableName, 
    Int  PortIndex, 
    UInt  MaxNumEntries, 
    UInt*  pNumEntries, 
    DtTableEntry*  pTableEntry,
    UInt  OutBufSize)
{
    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    return DtTableGet(&pCf->m_PropData, pTableName, PortIndex, MaxNumEntries,
                                                   pNumEntries, pTableEntry,  OutBufSize);
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps_GetTableForType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfProps_GetTableForType(
    DtCfProps*  pCf, 
    Int  TypeNumber, 
    Int  SubType, 
    DtPropertyFilterCriteria  Flt,
    const char*  pTableName, 
    UInt  MaxNumEntries, 
    UInt*  pNumEntries, 
    DtTableEntry*  pTableEntry,
    UInt  OutBufSize)
{
    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);
        
    // Now find the table
    return DtTableGetForType("DTA", TypeNumber, SubType, Flt.m_HwRev, Flt.m_FwVersion,
                              Flt.m_FwVariant, pTableName, Flt.m_PortIndex, MaxNumEntries,
                              pNumEntries, pTableEntry,  OutBufSize);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps_GetBool -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get the value of a Bool-property
//
Bool  DtCfProps_GetBool(DtCfProps*  pCf, const char*  pName, Int  PortIndex,
                                                                            Bool  Default)
{
    DtPropertyFilterCriteria  Flt;
    DtPropertyValue Value = Default ? 1 : 0;
    DtPropertyValueType  Type = PROPERTY_VALUE_TYPE_BOOL;
    DtPropertyScope  Scope = PROPERTY_SCOPE_DRIVER;

    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);
    DT_PROPERTY_FILTER_CRITERIA_INIT_FROM_PROPDATA(Flt, pCf->m_PropData, PortIndex);
    DtCfProps_Get(pCf, pName, Flt, &Value, &Type, &Scope, TRUE, ((Default) ? 1 : 0));

    return (Value != 0);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps_GetInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Get the value of an integer type property
//
Int  DtCfProps_GetInt(DtCfProps*  pCf, const char*  pName, Int  PortIndex, Int  Default)
{
    DtPropertyFilterCriteria  Flt;
    DtPropertyValue Value = Default;
    DtPropertyValueType  Type = PROPERTY_VALUE_TYPE_INT;
    DtPropertyScope  Scope = PROPERTY_SCOPE_DRIVER;

    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    DT_PROPERTY_FILTER_CRITERIA_INIT_FROM_PROPDATA(Flt, pCf->m_PropData, PortIndex);
    DtCfProps_Get(pCf, pName, Flt, &Value, &Type, &Scope, TRUE, Default);
    return (Int)Value;
}
// Get Int8-property
Int8  DtCfProps_GetInt8(DtCfProps*  pCf, const char*  pName, Int  PortIndex,
                                                                            Int8  Default)
{
    DtPropertyFilterCriteria  Flt;
    DtPropertyValue Value = Default;
    DtPropertyValueType  Type = PROPERTY_VALUE_TYPE_INT8;
    DtPropertyScope  Scope = PROPERTY_SCOPE_DRIVER;

    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    DT_PROPERTY_FILTER_CRITERIA_INIT_FROM_PROPDATA(Flt, pCf->m_PropData, PortIndex);
    DtCfProps_Get(pCf, pName, Flt, &Value, &Type, &Scope, TRUE, Default);
    return (Int8)Value;
}
// Get Int16-property
Int16  DtCfProps_GetInt16(DtCfProps*  pCf, const char*  pName, Int  PortIndex,
                                                                           Int16  Default)
{
    DtPropertyFilterCriteria  Flt;
    DtPropertyValue Value = Default;
    DtPropertyValueType  Type = PROPERTY_VALUE_TYPE_INT16;
    DtPropertyScope  Scope = PROPERTY_SCOPE_DRIVER;

    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    DT_PROPERTY_FILTER_CRITERIA_INIT_FROM_PROPDATA(Flt, pCf->m_PropData, PortIndex);
    DtCfProps_Get(pCf, pName, Flt, &Value, &Type, &Scope, TRUE, Default);
    return (Int16)Value;
}
// Get Int32-property
Int32  DtCfProps_GetInt32(DtCfProps*  pCf, const char*  pName, Int  PortIndex, 
                                                                           Int32  Default)
{
    DtPropertyFilterCriteria  Flt;
    DtPropertyValue Value = Default;
    DtPropertyValueType  Type = PROPERTY_VALUE_TYPE_INT32;
    DtPropertyScope  Scope = PROPERTY_SCOPE_DRIVER;

    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    DT_PROPERTY_FILTER_CRITERIA_INIT_FROM_PROPDATA(Flt, pCf->m_PropData, PortIndex);
    DtCfProps_Get(pCf, pName, Flt, &Value, &Type, &Scope, TRUE, Default);
    return (Int32)Value;
}
// Get Int64-property
Int64  DtCfProps_GetInt64(DtCfProps*  pCf, const char*  pName, Int  PortIndex, 
                                                                           Int64  Default)
{
    DtPropertyFilterCriteria  Flt;
    DtPropertyValue Value = Default;
    DtPropertyValueType  Type = PROPERTY_VALUE_TYPE_INT64;
    DtPropertyScope  Scope = PROPERTY_SCOPE_DRIVER;

    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    DT_PROPERTY_FILTER_CRITERIA_INIT_FROM_PROPDATA(Flt, pCf->m_PropData, PortIndex);
    DtCfProps_Get(pCf, pName, Flt, &Value, &Type, &Scope, TRUE, Default);
    return (Int64)Value;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps_GetUInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get the value of an unsigned-integer type property
//
UInt  DtCfProps_GetUInt(DtCfProps*  pCf, const char*  pName, Int  PortIndex, 
                                                                            UInt  Default)
{
    DtPropertyFilterCriteria  Flt;
    DtPropertyValue Value = Default;
    DtPropertyValueType  Type = PROPERTY_VALUE_TYPE_UINT;
    DtPropertyScope  Scope = PROPERTY_SCOPE_DRIVER;

    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    DT_PROPERTY_FILTER_CRITERIA_INIT_FROM_PROPDATA(Flt, pCf->m_PropData, PortIndex);
    DtCfProps_Get(pCf, pName, Flt, &Value, &Type, &Scope, TRUE, Default);
    return (UInt)Value;
}
// Get UInt8-property
UInt8  DtCfProps_GetUInt8(DtCfProps*  pCf, const char*  pName, Int  PortIndex, 
                                                                           UInt8  Default)
{
    DtPropertyFilterCriteria  Flt;
    DtPropertyValue Value = Default;
    DtPropertyValueType  Type = PROPERTY_VALUE_TYPE_UINT8;
    DtPropertyScope  Scope = PROPERTY_SCOPE_DRIVER;

    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    DT_PROPERTY_FILTER_CRITERIA_INIT_FROM_PROPDATA(Flt, pCf->m_PropData, PortIndex);
    DtCfProps_Get(pCf, pName, Flt, &Value, &Type, &Scope, TRUE, Default);
    return (UInt8)Value;
}
// Get UInt16-property
UInt16  DtCfProps_GetUInt16(DtCfProps*  pCf, const char*  pName, Int  PortIndex, 
                                                                          UInt16  Default)
{
    DtPropertyFilterCriteria  Flt;
    DtPropertyValue Value = Default;
    DtPropertyValueType  Type = PROPERTY_VALUE_TYPE_UINT16;
    DtPropertyScope  Scope = PROPERTY_SCOPE_DRIVER;

    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    DT_PROPERTY_FILTER_CRITERIA_INIT_FROM_PROPDATA(Flt, pCf->m_PropData, PortIndex);
    DtCfProps_Get(pCf, pName, Flt, &Value, &Type, &Scope, TRUE, Default);
    return (UInt16)Value;
}
// Get UInt32-property
UInt32  DtCfProps_GetUInt32(DtCfProps*  pCf, const char*  pName, Int  PortIndex, 
                                                                          UInt32  Default)
{
    DtPropertyFilterCriteria  Flt;
    DtPropertyValue Value = Default;
    DtPropertyValueType  Type = PROPERTY_VALUE_TYPE_UINT32;
    DtPropertyScope  Scope = PROPERTY_SCOPE_DRIVER;

    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    DT_PROPERTY_FILTER_CRITERIA_INIT_FROM_PROPDATA(Flt, pCf->m_PropData, PortIndex);
    DtCfProps_Get(pCf, pName, Flt, &Value, &Type, &Scope, TRUE, Default);
    return (UInt32)Value;
}
// Get UInt64-property
UInt64  DtCfProps_GetUInt64(DtCfProps*  pCf, const char*  pName, Int  PortIndex,
                                                                          UInt64  Default)
{
    DtPropertyFilterCriteria  Flt;
    DtPropertyValue Value = Default;
    DtPropertyValueType  Type = PROPERTY_VALUE_TYPE_UINT64;
    DtPropertyScope  Scope = PROPERTY_SCOPE_DRIVER;

    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    DT_PROPERTY_FILTER_CRITERIA_INIT_FROM_PROPDATA(Flt, pCf->m_PropData, PortIndex);
    DtCfProps_Get(pCf, pName, Flt, &Value, &Type, &Scope, TRUE, Default);
    return (UInt64)Value;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps_ReInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCfProps_ReInit(DtCfProps*  pCf)
{
    DtCore*  pCore = NULL;

    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);
    pCore = pCf->m_pCore;
    CORE_DEFAULT_PRECONDITIONS(pCore);

    // Re-initialize property data object
    pCf->m_PropData.m_PropertyNotFoundCounter = 0;
    //pCf->m_PropData.m_PropertyNotFoundString = ?;
    pCf->m_PropData.m_TypeName = pCore->m_TypeName;
    pCf->m_PropData.m_TypeNumber = pCore->m_pDevInfo->m_TypeNumber;
    pCf->m_PropData.m_SubDvcOrSubType = (Int)pCore->m_pDevInfo->m_SubType;
    pCf->m_PropData.m_HardwareRevision = pCore->m_pDevInfo->m_HardwareRevision;
    pCf->m_PropData.m_FirmwareVersion = pCore->m_pDevInfo->m_FirmwareVersion;
    pCf->m_PropData.m_FirmwareVariant = pCore->m_pDevInfo->m_FirmwareVariant;

    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCfProps - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCfProps_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCfProps_Init(DtDf*  pCf)
{
    DtStatus  Status = DT_STATUS_OK;
    DtCore*  pCore = NULL;

    // Sanity checks
    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);
    
    pCore = pCf->m_pCore;
    CORE_DEFAULT_PRECONDITIONS(pCore);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Init the property data -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // Initialize property data object
    CF_PROPS->m_PropData.m_pPropertyStore = NULL;
    CF_PROPS->m_PropData.m_PropertyNotFoundCounter = 0;
    //CF_PROPS->m_PropData.m_PropertyNotFoundString = ?;
    CF_PROPS->m_PropData.m_pTableStore = NULL;
    CF_PROPS->m_PropData.m_TypeName = pCore->m_TypeName;
    CF_PROPS->m_PropData.m_TypeNumber = pCore->m_pDevInfo->m_TypeNumber;
    CF_PROPS->m_PropData.m_SubDvcOrSubType = (Int)pCore->m_pDevInfo->m_SubType;
    CF_PROPS->m_PropData.m_HardwareRevision = pCore->m_pDevInfo->m_HardwareRevision;
    CF_PROPS->m_PropData.m_FirmwareVersion = pCore->m_pDevInfo->m_FirmwareVersion;
    CF_PROPS->m_PropData.m_FirmwareVariant = pCore->m_pDevInfo->m_FirmwareVariant;

    Status = DtPropertiesInit(&CF_PROPS->m_PropData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutCf(ERR, PROPS, pCf, "ERROR: failed to initialise property data");
        return Status;
    }

    // Init table properties
    Status = DtTablesInit(&CF_PROPS->m_PropData);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutCf(ERR, PROPS, pCf, "ERROR: failed to initialise table property data");
        return Status;
    }

    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCfProps implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the CfProps stub
#define STUB_CFPROPS_DEFAULT_PRECONDITIONS(pStub)      \
    DT_ASSERT(pStub!=NULL && pStub->m_Size>=sizeof(DtIoStubCfProps))

// MACRO to cast a stub object to a DtIoStubCfProps object
#define STUB_PROPS   ((DtIoStubCfProps*)pStub)
#define STUB_CF      ((DtCfProps*)STUB_PROPS->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtIoStubCfProps_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
static DtStatus  DtIoStubCfProps_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int* pOutSize);
static DtStatus  DtIoStubCfProps_OnCmdGetStr(const DtIoStubCfProps*,
                                                         const DtIoctlPropCmdGetStrInput*,
                                                         DtIoctlPropCmdGetStrOutput*);
static DtStatus  DtIoStubCfProps_OnCmdGetValue(const DtIoStubCfProps*,
                                                       const DtIoctlPropCmdGetValueInput*,
                                                       DtIoctlPropCmdGetValueOutput*);
static DtStatus  DtIoStubCfProps_OnCmdGetTable(const DtIoStubCfProps*,
                                                       const DtIoctlPropCmdGetTableInput*,
                                                       DtIoctlPropCmdGetTableOutput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_PROPERTY;

static const DtIoctlProperties  IOSTUB_CF_PROPS_IOCTLS[] = 
{
    DT_IOCTL_PROPS_PROPERTY_CMD(
        DtIoStubCfProps_OnCmd, 
        DtIoStubCfProps_AppendDynamicSize, 
        NULL),
};

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCfProps - Public fuctions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfProps_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubCfProps_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubCfProps));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfProps_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubCfProps*  DtIoStubCfProps_Open(DtDf*  pCf)
{
    DtIoStubCfProps*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    CF_PROPS_DEFAULT_PRECONDITIONS(pCf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubCfProps, pCf, NULL, 
                                                              DtIoStubCfProps_Close,
                                                              NULL,  // Use default IOCTL
                                                              IOSTUB_CF_PROPS_IOCTLS);
    pStub = (DtIoStubCfProps*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCfProps - Private fuctions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfProps_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCfProps_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlPropCmdInput*  pInData = NULL;

    STUB_CFPROPS_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_PROPERTY_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_PropCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_PROP_CMD_GET_TABLE:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlPropCmdGetTableInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlPropCmdGetTableOutput));
        // Add dynamic size (i.e. #bytes to read)
        pIoParams->m_OutReqSize += (pInData->m_GetTable.m_MaxNumEntries 
                                                                  * sizeof(DtTableEntry));
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfProps_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCfProps_OnCmd(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams,
    Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlPropCmdInput*  pInData = NULL;
    DtIoctlPropCmdOutput*  pOutData = NULL;
    
    STUB_CFPROPS_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_PROPERTY_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_PropCmd;
    DT_ASSERT(pIoParams->m_pOutData != NULL);
    pOutData = &pIoParams->m_pOutData->m_PropCmd;

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_PROP_CMD_GET_STR:
        Status = DtIoStubCfProps_OnCmdGetStr(STUB_PROPS, &pInData->m_GetStr,
                                                                     &pOutData->m_GetStr);
        break;
    case DT_PROP_CMD_GET_TABLE:
        Status = DtIoStubCfProps_OnCmdGetTable(STUB_PROPS, &pInData->m_GetTable, 
                                                                   &pOutData->m_GetTable);
        break;
    case DT_PROP_CMD_GET_VALUE:
        Status = DtIoStubCfProps_OnCmdGetValue(STUB_PROPS, &pInData->m_GetValue,
                                                                   &pOutData->m_GetValue);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfProps_OnCmdGetStr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubCfProps_OnCmdGetStr(const DtIoStubCfProps*  pStub,
                                                const DtIoctlPropCmdGetStrInput*  pInData,
                                                DtIoctlPropCmdGetStrOutput*  pOutData)
{
    DtStatus  Status=DT_STATUS_OK;
    DtPropertyFilterCriteria  Flt;

    STUB_CFPROPS_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    // Init property filter
    DT_PROPERTY_FILTER_CRITERIA_INIT(Flt, pInData->m_PortIndex);
    Flt.m_FwVersion = pInData->m_FirmwareVersion;
    Flt.m_FwVariant = pInData->m_FirmwareVariant;
    Flt.m_HwRev = pInData->m_HardwareRevision;
    Flt.m_DtapiMaj = pInData->m_DtapiMaj;
    Flt.m_DtapiMin = pInData->m_DtapiMin;
    Flt.m_DtapiBugfix = pInData->m_DtapiBugfix;

        
    // Get for specific type or for the attached devices
    if (pInData->m_TypeNumber==-1)
    {
        // Get the property for the current device
        Status = DtCfProps_GetStr(STUB_CF, pInData->m_Name, Flt,
                                                     pOutData->m_Str, &pOutData->m_Scope);
    }
    else
    {
        // NOTE: old DTAPI versions do not know DtIoctlPropCmdCommonInput::m_SubType 
        // exists and assume only a Int DtIoctlPropCmdCommonInput::m_SubDvc field, which 
        // it always sets to 0. Therefor if the Int16 DtIoctlPropCmdCommonInput::m_SubDvc 
        // is not -1, we have an old DTAPI and should ignore m_SubType and just use the 
        // current subtype in our property lookup
        Int  SubType = STUB_PROPS->m_pCore->m_pDevInfo->m_SubType;
        if (pInData->m_SubDvc==-1 && pInData->m_SubType!=-1)
            SubType = pInData->m_SubType;  // Use specified subtype
        
        // Property for a specific type was requested
        Status = DtCfProps_GetStrForType(STUB_CF, STUB_PROPS->m_pCore->m_TypeName,
                                                     pInData->m_TypeNumber, SubType,
                                                     Flt, pInData->m_Name,
                                                     pOutData->m_Str, &pOutData->m_Scope);
    }
    
    if (DT_SUCCESS(Status))
    {
        DT_UNUSED  const Int  Scope = pOutData->m_Scope;
        DT_ASSERT((Scope&PROPERTY_SCOPE_DTAPI) == PROPERTY_SCOPE_DTAPI);
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfProps_OnCmdGetTable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCfProps_OnCmdGetTable(
    const DtIoStubCfProps*  pStub,
    const DtIoctlPropCmdGetTableInput*  pInData,
    DtIoctlPropCmdGetTableOutput*  pOutData)
{
    STUB_CFPROPS_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    pOutData->m_NumEntries = 0;
    // Get for specific type or for the attached devices
    if (pInData->m_TypeNumber == -1)
        return DtCfProps_GetTable(STUB_CF, pInData->m_Name, pInData->m_PortIndex, 
                                           pInData->m_MaxNumEntries,
                                           &pOutData->m_NumEntries,
                                           pOutData->m_TableEntry, 
                                           pInData->m_MaxNumEntries*sizeof(DtTableEntry));
    else
    { 
        // Get table for specific typenumber, subtype, firmwarevariant, ....
        DtPropertyFilterCriteria  Filter;
        Filter.m_PortIndex    = pInData->m_PortIndex;
        Filter.m_HwRev        = pInData->m_HardwareRevision;
        Filter.m_FwVersion    = pInData->m_FirmwareVersion;
        Filter.m_FwVariant    = pInData->m_FirmwareVariant;
        Filter.m_DtapiMaj     = pInData->m_DtapiMaj;
        Filter.m_DtapiMin     = pInData->m_DtapiMin;
        Filter.m_DtapiBugfix  = pInData->m_DtapiBugfix;
        return DtCfProps_GetTableForType(STUB_CF, pInData->m_TypeNumber,
                                           pInData->m_SubType, Filter,
                                           pInData->m_Name, 
                                           pInData->m_MaxNumEntries,
                                           &pOutData->m_NumEntries,
                                           pOutData->m_TableEntry, 
                                           pInData->m_MaxNumEntries*sizeof(DtTableEntry));
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCfProps_OnCmdGetValue -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCfProps_OnCmdGetValue(const DtIoStubCfProps*  pStub,
                                              const DtIoctlPropCmdGetValueInput*  pInData,
                                              DtIoctlPropCmdGetValueOutput*  pOutData)
{
    DtStatus  Status=DT_STATUS_OK;
    DtPropertyFilterCriteria  Flt;

    STUB_CFPROPS_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    // Init property filter
    DT_PROPERTY_FILTER_CRITERIA_INIT(Flt, pInData->m_PortIndex);
    Flt.m_FwVersion = pInData->m_FirmwareVersion;
    Flt.m_FwVariant = pInData->m_FirmwareVariant;
    Flt.m_HwRev = pInData->m_HardwareRevision;
    Flt.m_DtapiMaj = pInData->m_DtapiMaj;
    Flt.m_DtapiMin = pInData->m_DtapiMin;
    Flt.m_DtapiBugfix = pInData->m_DtapiBugfix;

    // Get for specific type or for the attached devices
    if (pInData->m_TypeNumber == -1)
    {
        DtPropertyValue  Value;
        DtPropertyValueType  Type;
        DtPropertyScope  Scope;
    
        // Get the property for the current device
        Status = DtCfProps_Get(STUB_CF, pInData->m_Name, Flt, &Value, &Type,
                                                                       &Scope, FALSE, -1);
        pOutData->m_Value = Value;
        pOutData->m_Type = Type;
        pOutData->m_Scope = Scope;
    }
    else 
    {
        DtPropertyValue  Value;
        DtPropertyValueType  Type;
        DtPropertyScope  Scope;

        // NOTE: old DTAPI versions do not know DtIoctlPropCmdCommonInput::m_SubType 
        // exists and assume only a Int DtIoctlPropCmdCommonInput::m_SubDvc field, which 
        // it always sets to 0. Therefor if the Int16 DtIoctlPropCmdCommonInput::m_SubDvc 
        // is not -1, we have an old DTAPI and should ignore m_SubType and just use the 
        // current subtype in our property lookup
        Int  SubType = STUB_PROPS->m_pCore->m_pDevInfo->m_SubType;
        if (pInData->m_SubDvc==-1 && pInData->m_SubType!=-1)
            SubType = pInData->m_SubType;  // Use specified subtype
        
        // Property for a specific type was requested
        Status = DtCfProps_GetForType(STUB_CF, STUB_PROPS->m_pCore->m_TypeName,
                                             pInData->m_TypeNumber, SubType,
                                             Flt, pInData->m_Name, &Value, &Type, &Scope);
        pOutData->m_Value = Value;
        pOutData->m_Type = Type;
        pOutData->m_Scope = Scope;
    }

    if (DT_SUCCESS(Status))
    {
        DT_UNUSED  const Int  Scope = pOutData->m_Scope;
        DT_ASSERT((Scope&PROPERTY_SCOPE_DTAPI) == PROPERTY_SCOPE_DTAPI);
    }
    return Status;
}