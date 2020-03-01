//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCorePcie.c *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
//
// DtPcie driver - Interface for the DtPcie common driver, used by the IAL.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtPcieIncludes.h"            // Standard driver includes

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCorePcie implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCorePcie - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function will be called by the IAL every time an application opens a handle to the
// device.
//
DtStatus  DtCorePcie_Open(DtCorePcie* pCore, DtFileObject* pFile)
{
    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);

    // FOR NOW: let base function handle this
    return DtCore_Open((DtCore*)pCore, pFile);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL when an application closes a device which it has
// opened before. All allocated resources which were not deallocated synchronously must
// be checked by this function. If a resource was not deallocated yet it must be done by
// this function. Also exclusive access for a channel held by the current handle to be
// closed will be released.
//
DtStatus  DtCorePcie_Close(DtCorePcie* pCore, DtFileObject* pFile)
{
    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);

    // FOR NOW: let base function handle this
    return DtCore_Close((DtCore*)pCore, pFile);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_Ioctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCorePcie_Ioctl(DtCorePcie* pCore, DtFileObject* pFile, DtIoctlObject* pIoctl)
{
    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);

    // No further actions required => let base class do the heavy lifting
    return DtCore_Ioctl((DtCore*)pCore, pFile, pIoctl);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_IoctlChild -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called for every IO request a child driver sends to this driver.
// This function handles all the functional requests. If the request is a DMA transfer,
// it should be queued to the DMA channel queue. This queue must be power managed if
// supported by the host implementation.
//
DtStatus  DtCorePcie_IoctlChild(DtPcieChildDeviceData* pCore,
                                               DtFileObject* pFile, DtIoctlObject* pIoctl)
{
    DtStatus  Status = DT_STATUS_NOT_SUPPORTED;
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_AcquireExclAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCorePcie_AcquireExclAccess(DtCorePcie*  pCore)
{
    DtStatus  Result = DtFastMutexAcquire(&pCore->m_ExclAccessMutex);
    return Result;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_ReleaseExclAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtCorePcie_ReleaseExclAccess(DtCorePcie*  pCore)
{
    // Sanity checks
    COREPCIE_DEFAULT_PRECONDITIONS(pCore);

    DtFastMutexRelease(&pCore->m_ExclAccessMutex);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCorePcie_driver implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtCorePcie_DRIVER - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DRIVER_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL and initializes the common (host independent) part
// of the drivers
//
DtStatus  DtCorePcie_DRIVER_Init(DtEvtLog* pEvtObject)
{
    DtStatus  Status = DT_STATUS_OK;
    DtString  DtStrVersion;
    DtStringChar  DtStrVersionBuffer[32];
    DT_STRING_DECL(DtStrDot, ".");

    // Connect DtStrVersionBuffer to DtStrVersion
    DT_STRING_INIT(DtStrVersion, DtStrVersionBuffer, 32);

    // Create driver version string
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTPCIE_VERSION_MAJOR);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTPCIE_VERSION_MINOR);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTPCIE_VERSION_MICRO);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTPCIE_VERSION_BUILD);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Finally report the event
    DtEvtLogReport(pEvtObject, DTPCIE_LOG_DRIVER_LOADED, &DtStrVersion, NULL, NULL);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCorePcie_DRIVER_Exit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when the driver is unloaded. This function should
// undo everything done by DtCorePcie_DRIVER_Init.
//
void  DtCorePcie_DRIVER_Exit()
{
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCorePcie implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtIoStubCorePcie_Init(DtIoStubCorePcie*);
static DtStatus  DtIoStubCorePcie_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
static DtStatus  DtIoStubCorePcie_OnGetDevInfo(const DtIoStub*, DtIoStubIoParams*,
                                                                          Int*  pOutSize);
static DtStatus  DtIoStubCorePcie_OnGetDriverVersion(const DtIoStub*, DtIoStubIoParams*, 
                                                                          Int*  pOutSize);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_DEBUG;
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_EVENT;
DECL_DT_IOCTL_CMD_PROPS_GET_DEV_INFO;
DECL_DT_IOCTL_CMD_PROPS_GET_DRIVER_VERSION;
DECL_DT_IOCTL_CMD_PROPS_IOCONFIG;
DECL_DT_IOCTL_CMD_PROPS_PROPERTY;
DECL_DT_IOCTL_CMD_PROPS_REBOOT;
DECL_DT_IOCTL_CMD_PROPS_TOD;
DECL_DT_IOCTL_CMD_PROPS_VPD;

static const DtIoctlProperties  IOSTUB_CORE_PCIE_IOCTLS[] = 
{
    DT_IOCTL_PROPS_DEBUG_CMD(
        DtIoStubCore_DEBUG_OnCmd,
        DtIoStubCore_DEBUG_AppendDynamicSize,
        NULL),
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubCore_OnExclAccessCmd,
        NULL, NULL),
    DT_IOCTL_PROPS_EVENT_CMD(
        NULL, NULL,
        DtIoStubCore_GetChildStub),
    DT_IOCTL_PROPS_GET_DEV_INFO(
        DtIoStubCorePcie_OnGetDevInfo,
        DtIoStubCorePcie_AppendDynamicSize,
        NULL),
    DT_IOCTL_PROPS_GET_DRIVER_VERSION(
        DtIoStubCorePcie_OnGetDriverVersion, 
        NULL, NULL),
    DT_IOCTL_PROPS_IOCONFIG_CMD(
        NULL, NULL,
        DtIoStubCore_GetChildStub),
    DT_IOCTL_PROPS_PROPERTY_CMD(
        NULL, NULL,
        DtIoStubCore_GetChildStub),
    DT_IOCTL_PROPS_REBOOT_CMD(
        NULL, NULL,
        DtIoStubCore_GetChildStub),
    DT_IOCTL_PROPS_TOD_CMD(
        NULL, NULL,
        DtIoStubCore_GetChildStub),
    DT_IOCTL_PROPS_VPD_CMD(
        NULL, NULL, 
        DtIoStubCore_GetChildStub),
};

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCorePcie - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCorePcie_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubCorePcie_Close(DtIoStub*  pStub)
{
    if (pStub == NULL)
        return;
    DT_ASSERT(pStub->m_Size>=sizeof(DtIoStubCorePcie));

    // Let base implementation do the heavy lifting 
    DtIoStub_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCorePcie_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubCore*  DtIoStubCorePcie_Open(DtCore*  pCore)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoStubCorePcie*  pStub = NULL;
    DtIoStubOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL  && pCore->m_Size>=sizeof(DtCorePcie));

    //.-.-.-.-.-.-.-.-.-.- Step 1: Allocate memory for io-stub object -.-.-.-.-.-.-.-.-.-.

    DT_IOSTUB_INIT_OPEN_PARAMS(OpenParams, DtIoStubCorePcie, pCore, "DF_CORE#1", 
                                                               DtIoStubCorePcie_Close,
                                                               NULL,  // Use default IOCTL
                                                               IOSTUB_CORE_PCIE_IOCTLS);

    // Let root implementation do the heavy lifting
    pStub = (DtIoStubCorePcie*)DtIoStub_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;

    // Init stub
    Status = DtIoStubCorePcie_Init(pStub);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutIoStub(ERR, CORE, pStub, "ERROR: failed to initialise stub");
        DtIoStubCorePcie_Close((DtIoStub*)pStub);
        return NULL;
    }
    return (DtIoStubCore*)pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCorePcie - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCorePcie_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCorePcie_Init(DtIoStubCorePcie*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size>=sizeof(DtIoStubCorePcie));
    // NOTHING SPECIAL TO DO => LET BASE CLASS DO HEAVY LIFTING
    return DtIoStubCore_Init((DtIoStubCore*)pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCorePcie_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubCorePcie_AppendDynamicSize(
    const DtIoStub*  pStub, 
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlGetDevInfoInput*  pInData = NULL;
    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubCorePcie));
    DT_ASSERT(pIoParams != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_GET_DEV_INFO);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_GetDevInfo;
    
    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_IOCTL_CMD_NOP:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlGetDevInfoInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlGetDevInfoCommonOutput));
        // Add dynamic size (i.e. #bytes to read)
        pIoParams->m_OutReqSize += sizeof(DtIoctlDevInfoPcie);
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCorePcie_OnExclAccessCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCorePcie_OnExclAccessCmd(const DtIoStub*  pStub, Int PortIndex, 
                                                            Int Cmd, DtFileObject*  pFile)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size>=sizeof(DtIoStubCorePcie));
    // NOTHING SPECIAL TO DO => LET BASE CLASS DO HEAVY LIFTING
    return  DT_STATUS_NOT_IMPLEMENTED;// DtIoStubCore_OnExclAccessCmd(pStub, PortIndex, Cmd, pFile);
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCorePcie_OnGetDevInfo -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCorePcie_OnGetDevInfo(
    const DtIoStub*  pStub, 
    DtIoStubIoParams*  pIoParams, 
    Int*  pOutSize)
{
    DtCorePcie*  pCore = NULL;
    const DtIoctlGetDevInfoInput*  pInData = NULL;
    DtIoctlGetDevInfoOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubCorePcie));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_GET_DEV_INFO);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    pCore = (DtCorePcie*)pStub->m_pCore;
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=(sizeof(DtCorePcie)));
    
    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_GetDevInfo;
    DT_ASSERT(pIoParams->m_pOutData != NULL);
    pOutData = &pIoParams->m_pOutData->m_GetDevInfo;
    
    // Handle the command
    if (pInData->m_Cmd != DT_IOCTL_CMD_NOP)
    {
        DT_ASSERT(pInData->m_Cmd == DT_IOCTL_CMD_NOP);
        return DT_STATUS_NOT_SUPPORTED;
    }

    pOutData->m_DeviceId = pCore->m_DevInfo.m_DeviceId;
    pOutData->m_SubType = pCore->m_DevInfo.m_SubType;
    pOutData->m_VendorId = pCore->m_DevInfo.m_VendorId;
    pOutData->m_SubSystemId = pCore->m_DevInfo.m_SubSystemId;
    pOutData->m_SubVendorId = pCore->m_DevInfo.m_SubVendorId;
    pOutData->m_TypeNumber = pCore->m_DevInfo.m_TypeNumber;
    pOutData->m_SubDvc = pCore->m_DevInfo.m_SubDvc;
    pOutData->m_HardwareRevision = pCore->m_DevInfo.m_HardwareRevision;
    pOutData->m_FirmwareVersion = pCore->m_DevInfo.m_FirmwareVersion;
    pOutData->m_FirmwareVariant = pCore->m_DevInfo.m_FirmwareVariant;
    pOutData->m_FwPackageVersion = pCore->m_DevInfo.m_FwPackageVersion;
    pOutData->m_FwBuildDate = pCore->m_DevInfo.m_FwBuildDate;
    pOutData->m_FirmwareStatus = pCore->m_DevInfo.m_FirmwareStatus;
    pOutData->m_Serial = pCore->m_DevInfo.m_Serial;
    pOutData->m_DevSpecific.m_Pcie.m_BusNumber = pCore->m_DevInfo.m_BusNumber;
    pOutData->m_DevSpecific.m_Pcie.m_SlotNumber = pCore->m_DevInfo.m_SlotNumber;
    pOutData->m_DevSpecific.m_Pcie.m_PcieNumLanes = pCore->m_DevInfo.m_PcieNumLanes;
    pOutData->m_DevSpecific.m_Pcie.m_PcieMaxLanes = pCore->m_DevInfo.m_PcieMaxLanes;
    pOutData->m_DevSpecific.m_Pcie.m_PcieLinkSpeed = pCore->m_DevInfo.m_PcieLinkSpeed;
    pOutData->m_DevSpecific.m_Pcie.m_PcieMaxSpeed = pCore->m_DevInfo.m_PcieMaxSpeed;
    pOutData->m_DevSpecific.m_Pcie.m_PcieMaxPayloadSize = 
                                                    pCore->m_DevInfo.m_PcieMaxPayloadSize;
    pOutData->m_DevSpecific.m_Pcie.m_PcieMaxReadRequestSize = 
                                                pCore->m_DevInfo.m_PcieMaxReadRequestSize;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.- DtIoStubCorePcie_OnGetDriverVersion -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubCorePcie_OnGetDriverVersion(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams,
    Int*  pOutSize)
{
    const DtIoctlGetDriverVersionInput*  pInData = NULL;
    DtIoctlGetDriverVersionOutput*  pOutData = NULL;
    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubCorePcie));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_GET_DRIVER_VERSION);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_GetDriverVersion;
    DT_ASSERT(pIoParams->m_pOutData != NULL);
    pOutData = &pIoParams->m_pOutData->m_GetDriverVersion;

    // Handle the command
    if (pInData->m_Cmd != DT_IOCTL_CMD_NOP)
    {
        DT_ASSERT(pInData->m_Cmd == DT_IOCTL_CMD_NOP);
        return DT_STATUS_NOT_SUPPORTED;
    }

    pOutData->m_Major = DTPCIE_VERSION_MAJOR;
    pOutData->m_Minor = DTPCIE_VERSION_MINOR;
    pOutData->m_Micro = DTPCIE_VERSION_MICRO;
    pOutData->m_Build = DTPCIE_VERSION_BUILD;
    
    return DT_STATUS_OK;
}

