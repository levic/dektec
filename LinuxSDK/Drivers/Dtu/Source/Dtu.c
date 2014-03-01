//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dtu.c *#*#*#*#*#*#*#*#*#*# (C) 2011-2012 DekTec
//
// Dtu driver - Interface for the Dtu common driver, used by the IAL.
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
#include <DtuIncludes.h>

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Forward declarations
static DtStatus  DtuCalculateNumPorts(DtuDeviceData* pDvcData);
static DtStatus  DtuPortsInit(DtuDeviceData* pDvcData);
static void  DtuPortsCleanUp(DtuDeviceData* pDvcData);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Dtu Common Interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDriverInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL and initializes the common (host independent) part
// of the driver.
//
DtStatus  DtuDriverInit(DtEvtLog* pEvtObject)
{
    DtStatus  Status = DT_STATUS_OK;
    DtString  DtStrVersion;
    DtStringChar  DtStrVersionBuffer[32];
    DT_STRING_DECL(DtStrDot, ".");

    // Connect DtStrVersionBuffer to DtStrVersion
    DT_STRING_INIT(DtStrVersion, DtStrVersionBuffer, 32);

    // Create driver version string
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTU_VERSION_MAJOR);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTU_VERSION_MINOR);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTU_VERSION_MICRO);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTU_VERSION_BUILD);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Finally report the event
    DtEvtLogReport(pEvtObject, DTU_LOG_DRIVER_LOADED, &DtStrVersion, NULL, NULL);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDriverExit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL when the driver is unloaded. This function should
// undo everything done by DtDriverInit.
//
void  DtuDriverExit()
{
    // NOTHING TODO FOR NOW
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDeviceInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL when the device is found in the system by the bus
// driver. 
//
DtStatus  DtuDeviceInit(DtuDeviceData* pDvcData)
{
    Int  i;
    
    DtDbgOut(MAX, DTU, "Start");

    // Deduce typenumber
    pDvcData->m_DevInfo.m_TypeNumber = 
                                 DtuProductId2TypeNumber(pDvcData->m_DevInfo.m_ProductId);

    DtDbgOut(MIN, DTU, "Found: pid 0x%04X, vendor 0x%04X => DTU-%d", 
                       pDvcData->m_DevInfo.m_ProductId, pDvcData->m_DevInfo.m_VendorId,
                                                        pDvcData->m_DevInfo.m_TypeNumber);

    // The first powerup is special to initialize resources that need the hardware
    pDvcData->m_InitialPowerup = TRUE;

    // Initialize file handle info mutex
    DtFastMutexInit(&pDvcData->m_FileHandleInfoMutex);
    // Initialize file handle info
    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
    {
        pDvcData->m_FileHandleInfo[i].m_pHandle = NULL;
        pDvcData->m_FileHandleInfo[i].m_PowerDownPending = FALSE;
    }

    // Initialize Exclusive access fast mutex
    DtFastMutexInit(&pDvcData->m_ExclAccessMutex);

    // Initialize events
    DtuEventsInit(pDvcData);

    DtDbgOut(MAX, DTU, "Exit (device: DTU-%d)", pDvcData->m_DevInfo.m_TypeNumber);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDeviceOpen -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function will be called by the IAL every time an application opens a handle to the
// device.
//
DtStatus  DtuDeviceOpen(DtuDeviceData* pDvcData, DtFileObject* pFile)
{
    Int  i;

    // Skip user access if not completly initialised
    if (pDvcData->m_InitialPowerup)
        return DT_STATUS_NOT_INITIALISED;

    // Add file handle to the filehandle info array
    DtFastMutexAcquire(&pDvcData->m_FileHandleInfoMutex);
    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
    {
        if (pDvcData->m_FileHandleInfo[i].m_pHandle == NULL)
        {
            pDvcData->m_FileHandleInfo[i].m_pHandle = DtFileGetHandle(pFile);
            pDvcData->m_FileHandleInfo[i].m_PowerDownPending = FALSE;
            break;
        }
    }
    DtFastMutexRelease(&pDvcData->m_FileHandleInfoMutex);

    // Check if we found an unused entry
    if (i == MAX_NUM_FILE_HANDLES)
        return DT_STATUS_OUT_OF_RESOURCES;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDevicePowerUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called when the device must enter the active (D0) state. It should
// wake up the corresponding device and initialize it into active state. It should restore
// any saved settings. These settings can be saved during power down.
// This function can be executed in two different scenarios. The first one is during the
// initial powerup. In this case it checks whether the device is supported and allocates
// all the software resources, for example continuous memory buffers for DMA and DPC
// objects. The second scenario is the powerup after powerdown. In this case the
// resources are already allocated. The device hardware only needs to be initialized.
// Since we don't support streams to resume after a powerdown, the channel HW doesn't
// have to be initialized, but the ethernet for example must be re-inintialized with the
// same setttings (like multicast list) that it was using before the power down.
// It is possible that resources have changed during powerdown, therefore the register
// mapping to userspace must be destroyed during powerdown. Also the register pointers
// in the driver must be reinitialized durint the powerup.
//
DtStatus  DtuDevicePowerUp(DtuDeviceData* pDvcData)
{
    Int  i;
    char  Buffer[64];
    UInt  BufLen = sizeof(Buffer);
    UInt32  Value = 0;
    Int64  ForcedHardwareRevision;
    DtStatus  Status;
    Bool  ReEnumerate = FALSE;
    DtPropertyData*  pPropData = &pDvcData->m_PropData;

    DtDbgOut(MAX, DTU, "Start");

    // Make sure property not found counter is reset after a power resume
    DtResetPropertiesNotFoundCounter(&pDvcData->m_PropData);

    if (pDvcData->m_InitialPowerup)
    {
        // Initialize property data object
        pPropData->m_pPropertyStore = NULL;
        pPropData->m_PropertyNotFoundCounter = 0;
        //pPropData->m_PropertyNotFoundString = ?;
        pPropData->m_pTableStore = NULL;
        pPropData->m_TypeName = "DTU";
        pPropData->m_TypeNumber = pDvcData->m_DevInfo.m_TypeNumber;
        pPropData->m_FirmwareVersion = 0;
        pPropData->m_HardwareRevision = 0;

        // (Re-)initialise the property store
        Status = DtuPropertiesInit(pDvcData);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTU, "failed to initialise the property store");
            return Status;
        }

        // Initialise the table store
        Status = DtTablesInit(pPropData);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTU, "failed to initialise the property table store");
            return Status;
        }
    }

    // Init EzUsb module
    ReEnumerate = FALSE;
    Status = EzUsbInit(pDvcData, &ReEnumerate);
    if (!DT_SUCCESS(Status))
        return Status;
    if (ReEnumerate)
        return Status;  // No futher action required, device will re-enumerate now


    if (pDvcData->m_InitialPowerup)
    {   
        // Init I2C module
        Status = DtuI2cInit(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;
     }

    // Switch on power
    Status = DtuDvcPowerSupplyInit(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    // Initialize PLD
    Status = DtuPldInit(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    if (pDvcData->m_InitialPowerup)
    {
        // Get hardware revision
        pDvcData->m_DevInfo.m_HardwareRevision = 0;
        // Get firmware variant
        pDvcData->m_DevInfo.m_FirmwareVariant = 0;

        // Get USB speed
        Status = DtuGetUsbSpeed(pDvcData, &Value);
        if (!DT_SUCCESS(Status))
            return Status;
        pDvcData->m_DevInfo.m_UsbSpeed = (Int)Value;

        // Get USB address
        Status = DtuGetUsbAddress(pDvcData, &Value);
        if (!DT_SUCCESS(Status))
            return Status;
        pDvcData->m_DevInfo.m_UsbAddress = (Int)Value;

        // Get Firmware version       
        Status = DtuRegRead(pDvcData, DT_GEN_REG_GENCTRL, &Value);
        if (!DT_SUCCESS(Status))
            return Status;
        pDvcData->m_DevInfo.m_FirmwareVersion = (Value & DT_GENCTRL_FWREV_MSK)  >>
                                                                    DT_GENCTRL_FWREV_SH;

        // Initialise the number and type of ports the card supports. 
        Status = DtuCalculateNumPorts(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;

         // Check if no property errors occurred
        Status = DtuPropertiesReportDriverErrors(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;
        
        // Init VPD module
        Status = DtuVpdInit(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;

        // Get serial number
        pDvcData->m_DevInfo.m_Serial = -1;
        Status = DtuVpdReadItemRo(pDvcData, 2, "SN", Buffer, &BufLen);
        if (DT_SUCCESS(Status))
            pDvcData->m_DevInfo.m_Serial = DtAnsiCharArrayToUInt64(Buffer, BufLen, 10);
        else {
            DtDbgOut(ERR, DTU, "Failed to read serial from VPD (0x%x)", Status);
            DtEvtLogReport(&pDvcData->m_Device.m_EvtObject, DTU_LOG_SERIAL_FAILED, NULL,
                                                                              NULL, NULL);
        }

        // Get hardware revision from VPD "EC" resource
        // Initialize hardware revision to -1, indicating that it is not defined yet
        pDvcData->m_DevInfo.m_HardwareRevision = -1;
        BufLen = sizeof(Buffer);
        Status = DtuVpdReadItemRo(pDvcData, 2, "EC", Buffer, &BufLen);
        if (DT_SUCCESS(Status))
        {
            // VPD "EC" resource can be read.
            Status = DtUtilitiesDeduceHardwareRevision(Buffer, BufLen, 
                                                 &pDvcData->m_DevInfo.m_HardwareRevision); 
            if (DT_SUCCESS(Status))
            {
                DtDbgOut(MIN, DTU, "'EC' is %s, hence deduced hardware rev is %d", 
                                          Buffer, pDvcData->m_DevInfo.m_HardwareRevision);
            } else {
                // VPD "EC" resource is formatted incorrectly
                // Consider this a non-critical error
                pDvcData->m_DevInfo.m_HardwareRevision = 0;
                DtDbgOut(MIN, DTU, "'EC' resouce is formatted incorrectly (%s), "
                                   "hence hardware rev set to 0", Buffer);
            }
        } else {
            // VPD "EC" resource cannot be read, probably because this is a newly 
            // produced card with blank VPD PROM.
            // Check for registry key which defines the hardware revision
            Status = DtNonVolatileManufSettingsRead(&pDvcData->m_Driver, 
                              pDvcData->m_PropData.m_TypeNumber, "ForcedHardwareRevision", 
                                                                 &ForcedHardwareRevision);
            if (DT_SUCCESS(Status))
            {
                pDvcData->m_DevInfo.m_HardwareRevision = (Int)(ForcedHardwareRevision);
                DtDbgOut(MIN, DTU, "Hardware revision set to %d from registry key "
                                   "'ForcedHardwareRevision'", 
                                                  pDvcData->m_DevInfo.m_HardwareRevision);
            } else {
                DtDbgOut(MIN, DTU, "Can't find registry key 'ForcedHardwareRevision'");
            }
        }
        pDvcData->m_PropData.m_HardwareRevision = pDvcData->m_DevInfo.m_HardwareRevision;
        pPropData->m_HardwareRevision = pDvcData->m_DevInfo.m_HardwareRevision;

        // Verify the hardware revision 
        if (pDvcData->m_DevInfo.m_HardwareRevision < 0)
        {
            // Hardware revision could not be set (no 'EC' resource and no registry key)
            // Consider this a critical error
            DtDbgOut(MIN, DTU, "Hardware revision could not be set (no 'EC' resource"
                                " and no registry key 'ForcedHardwareRevision')");
            return DT_STATUS_FAIL;
        }

        // *** Set device subtype (-1=undefined, 0=none, 1=A, ...)

        // Initialize subtype to -1, indicating that subtype is not defined yet
        pDvcData->m_DevInfo.m_SubType = -1;

        Status = DtUtilitiesDeriveSubType(&pDvcData->m_PropData, 
                  pDvcData->m_DevInfo.m_HardwareRevision, &pDvcData->m_DevInfo.m_SubType);
        if (DT_SUCCESS(Status))
            DtDbgOut(MIN, DTU, "SubType set to %d", pDvcData->m_DevInfo.m_SubType);
        else
            // Consider this a non-critical error
            DtDbgOut(MIN, DTU, "SubType could not be set");


        // Update Property data
        pPropData->m_FirmwareVersion = pDvcData->m_DevInfo.m_FirmwareVersion;

        // Initialize ports (SW)
        Status = DtuPortsInit(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    // Initialise demodulator
    Status = DtuDemodInit(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;
    // Perform IO configurations
    Status = DtuIoConfigInit(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    // First powerup is done
    pDvcData->m_InitialPowerup = FALSE;

    // Set power up event
    DtuEventsSet(pDvcData, NULL, DTU_EVENT_TYPE_POWER, DTU_EVENT_VALUE1_POWER_UP, 0);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDevicePowerDown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called when the device must leave the active (D0) state. It
// should stop receive / transmit threads, store the device state and put the device in
// low power mode.
//
DtStatus  DtuDevicePowerDown(DtuDeviceData* pDvcData)
{
    Int  i;
    DtDbgOut(MAX, DTU, "Start");
  
    // Set power down pending to prevent new IOCTL's
    DtFastMutexAcquire(&pDvcData->m_FileHandleInfoMutex);
    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
    {
        if (pDvcData->m_FileHandleInfo[i].m_pHandle != NULL) 
            pDvcData->m_FileHandleInfo[i].m_PowerDownPending = TRUE;
    }
    DtFastMutexRelease(&pDvcData->m_FileHandleInfoMutex);

    // Set power down event
    DtuEventsSet(pDvcData, NULL, DTU_EVENT_TYPE_POWER, DTU_EVENT_VALUE1_POWER_DOWN, 0);
    DtDbgOut(MAX, DTU, "Exit");

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDeviceExit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL when the device is being disconnected. This
// function should undo everything done by DeviceInitPost and DeviceInit.
//
void  DtuDeviceExit(DtuDeviceData* pDvcData)
{
    // Clean up ports
    DtuPortsCleanUp(pDvcData);
    
    // Cleanup I2C module
    DtuI2cCleanup(pDvcData);

    // Cleanup VPD module
    DtuVpdCleanup(pDvcData);

    // Cleanup Properties module
    DtPropertiesCleanup(&pDvcData->m_PropData);

    // Cleanup events
    DtuEventsCleanup(pDvcData);
}



//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDeviceClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when an application closes a device which it has
// opened before. All allocated resources which were not deallocated synchronously must
// be checked by this function. If a resource was not deallocated yet it must be done by
// this function. Also exclusive access for a channel held by the current handle to be
// closed will be released.
//
DtStatus  DtuDeviceClose(DtuDeviceData* pDvcData, DtFileObject* pFile)
{
    Int  i;

    // Unlock all recursive I2C locks for this file object
    if (pDvcData->m_I2c.m_IsSupported)
        DtuI2cUnlock(pDvcData, pFile, TRUE);

    // Remove file handle from the filehandle info array
    DtFastMutexAcquire(&pDvcData->m_FileHandleInfoMutex);
    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
{
        if (pDvcData->m_FileHandleInfo[i].m_pHandle == DtFileGetHandle(pFile))
        {
            pDvcData->m_FileHandleInfo[i].m_pHandle = NULL;
            break;
        }
    }
    DtFastMutexRelease(&pDvcData->m_FileHandleInfoMutex);

    // Unregister events
    DtuEventsUnregister(pDvcData, pFile); 

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDeviceIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called for every IO request an application sends to the device
// driver. It handles all the functional requests.
//
DtStatus  DtuDeviceIoctl(
    DtuDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoctlObject*  pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pIoctlStr = NULL;
    UInt  InReqSize = 0;
    UInt  OutReqSize = 0;
    DtuIoctlInputData*  pInBuf = (DtuIoctlInputData*)pIoctl->m_pInputBuffer;
    DtuIoctlOutputData*  pOutBuf = (DtuIoctlOutputData*)pIoctl->m_pOutputBuffer;
    DtPropertyData*  pPropData = &pDvcData->m_PropData;

    Int  i;
    Bool  PowerDownPending = FALSE;

    DT_ASSERT(!pDvcData->m_InitialPowerup);

    // Get powerdown pending flag for this file handle from the filehandle info array
    DtFastMutexAcquire(&pDvcData->m_FileHandleInfoMutex);
    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
    {
        if (pDvcData->m_FileHandleInfo[i].m_pHandle == DtFileGetHandle(pFile))
        {
            PowerDownPending = pDvcData->m_FileHandleInfo[i].m_PowerDownPending;
            break;
        }
    }
    DtFastMutexRelease(&pDvcData->m_FileHandleInfoMutex);

    // Get minimal input/output buffer sizes
    switch (pIoctl->m_IoctlCode)
    {
    case DTU_IOCTL_GET_PROPERTY:
        pIoctlStr = "DTU_IOCTL_GET_PROPERTY";
        InReqSize = sizeof(DtuIoctlGetPropertyInput);
        OutReqSize = sizeof(DtuIoctlGetPropertyOutput);
        break;
    case DTU_IOCTL_GET_DEV_INFO:
        pIoctlStr = "DTU_IOCTL_GET_DEV_INFO";
        InReqSize = 0;
        OutReqSize = sizeof(DtuIoctlGetDevInfoOutput);
        break;
    case DTU_IOCTL_VPD_CMD:
        pIoctlStr = "DTU_IOCTL_VPD_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTU_IOCTL_GET_DRIVER_VERSION:
        pIoctlStr = "DTU_IOCTL_GET_DRIVER_VERSION";
        InReqSize = 0;
        OutReqSize = sizeof(DtuIoctlGetDriverVersionOutput);
        break;
    case DTU_IOCTL_I2C_CMD:
        pIoctlStr = "DTU_IOCTL_I2C_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTU_IOCTL_REG_READ:
        pIoctlStr = "DTU_IOCTL_REG_READ";
        InReqSize = sizeof(DtuIoctlRegReadInput);
        OutReqSize = sizeof(DtuIoctlRegReadOutput);
        break;
    case DTU_IOCTL_REG_WRITE_MASKED:
        pIoctlStr = "DTU_IOCTL_REG_WRITE_MASKED";
        InReqSize = sizeof(DtuIoctlRegWriteMaskedInput);
        OutReqSize = 0;
        break;
    case DTU_IOCTL_GET_EVENT:
        pIoctlStr = "DTU_IOCTL_GET_EVENT";
        InReqSize = 0;
        OutReqSize = sizeof(DtuIoctlGetEventOutput);
        break;
    case DTU_IOCTL_REGISTER_EVENTS:
        pIoctlStr = "DTU_IOCTL_REGISTER_EVENTS";
        InReqSize = sizeof(DtuIoctlRegisterEventsInput);
        OutReqSize = 0;
        break;
    case DTU_IOCTL_UNREGISTER_EVENTS:
        pIoctlStr = "DTU_IOCTL_UNREGISTER_EVENTS";
        InReqSize = 0;
        OutReqSize = 0;
        break;
    case DTU_IOCTL_READ_DATA:
        pIoctlStr = "DTU_IOCTL_READ_DATA";
        InReqSize = sizeof(DtuIoctlReadDataInput);
#ifdef WINBUILD
        OutReqSize = 0;
#else
        OutReqSize = sizeof(DtuIoctlReadDataOutput);
#endif
        break;
    case DTU_IOCTL_WRITE_DATA:
        pIoctlStr = "DTU_IOCTL_WRITE_DATA";
        InReqSize = sizeof(DtuIoctlWriteDataInput);
        OutReqSize = 0;
        break;
    case DTU_IOCTL_GET_IOCONFIG:
        pIoctlStr = "DTU_IOCTL_GET_IOCONFIG";
        InReqSize = OFFSETOF(DtuIoctlGetIoConfigInput, m_IoCfgId);
        OutReqSize = OFFSETOF(DtuIoctlGetIoConfigOutput, m_IoCfgValue);
        break;
    case DTU_IOCTL_SET_IOCONFIG:
        pIoctlStr = "DTU_IOCTL_SET_IOCONFIG";
        InReqSize = OFFSETOF(DtuIoctlSetIoConfigInput, m_IoConfig);
        OutReqSize = 0;
        break;
    case DTU_IOCTL_NONIP_CMD:
        pIoctlStr = "DTU_IOCTL_NONIP_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTU_IOCTL_NONIP_RX_CMD:
        pIoctlStr = "DTU_IOCTL_NONIP_RX_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTU_IOCTL_NONIP_TX_CMD:
        pIoctlStr = "DTU_IOCTL_NONIP_TX_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTU_IOCTL_RESET_SOFT:
        pIoctlStr = "DTU_IOCTL_RESET_SOFT";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTU_IOCTL_GET_TABLE:
        pIoctlStr = "DTU_IOCTL_GET_TABLE";
        InReqSize = sizeof(DtuIoctlGetTableInput);
        OutReqSize = sizeof(DtuIoctlGetTableOutput);
        break;
    case DTU_IOCTL_GET_STR_PROPERTY:
        pIoctlStr = "DTU_IOCTL_GET_STR_PROPERTY";
        InReqSize = sizeof(DtuIoctlGetStrPropertyInput);
        OutReqSize = sizeof(DtuIoctlGetStrPropertyOutput);
        break;
    case DTU_IOCTL_GET_DEV_SUBTYPE:
        pIoctlStr = "DTU_IOCTL_GET_DEV_SUBTYPE";
        InReqSize = 0;
        OutReqSize = sizeof(DtuIoctlGetDevSubTypeOutput);
        break;
    default:
        Status = DT_STATUS_NOT_SUPPORTED;
        break;
    }

    if (PowerDownPending)
    {
        // Only skip channel specific IOCTL's
        switch (pIoctl->m_IoctlCode) 
        {
            case DTU_IOCTL_REG_READ:
            case DTU_IOCTL_REG_WRITE_MASKED:
            case DTU_IOCTL_READ_DATA:
            case DTU_IOCTL_WRITE_DATA:
            case DTU_IOCTL_NONIP_RX_CMD:
            case DTU_IOCTL_NONIP_TX_CMD:
                DtDbgOut(ERR, DTU, "%s: Skipping IOCTL because powerdown  occured!",
                                                                               pIoctlStr);
                return DT_STATUS_POWERDOWN;
        } 
    }

     // Check if buffer sizes satisfy
    if (DT_SUCCESS(Status))
    {
        if (pIoctl->m_InputBufferSize < InReqSize) 
        {
            DtDbgOut(ERR, DTU, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) INPUT BUFFER TOO SMALL!",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
            Status = DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize) 
        {
            DtDbgOut(ERR, DTU, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) OUTPUT BUFFER TOO"
                                          " SMALL!",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
            Status = DT_STATUS_INVALID_PARAMETER;
        }
        if (DT_SUCCESS(Status))
            DtDbgOut(MAX, DTU, "%s: In=%d (Rq=%d) Out=%d (Rq=%d)",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    // Handle IOCTLS
    if (DT_SUCCESS(Status))
    {
        switch (pIoctl->m_IoctlCode)
        {
        case DTU_IOCTL_GET_PROPERTY:

            // Get for specific type or for the attached devices
            if (pInBuf->m_GetProperty.m_TypeNumber == -1)
            {
                DtPropertyValue  Value;
                DtPropertyValueType  Type;
                DtPropertyScope  Scope;
                
                // Get the property for the current device
                Status = DtPropertiesGet(pPropData, pInBuf->m_GetProperty.m_Name,
                                                    pInBuf->m_GetProperty.m_PortIndex,
                                                    &Value, &Type, &Scope);
                pOutBuf->m_GetProperty.m_Value = Value;
                pOutBuf->m_GetProperty.m_Type = Type;
                pOutBuf->m_GetProperty.m_Scope = Scope;
            }
            else
            {
                DtPropertyValue  Value;
                DtPropertyValueType  Type;
                DtPropertyScope  Scope;

                // Property for a specific type was requested
                Status = DtPropertiesGetForType(pInBuf->m_GetProperty.m_TypeNumber,
                                                 pInBuf->m_GetProperty.m_HardwareRevision, 
                                                 pInBuf->m_GetProperty.m_FirmwareVersion,
                                                 pInBuf->m_GetProperty.m_Name,
                                                 pInBuf->m_GetProperty.m_PortIndex,
                                                 &Value, &Type, &Scope);
                pOutBuf->m_GetProperty.m_Value = Value;
                pOutBuf->m_GetProperty.m_Type = Type;
                pOutBuf->m_GetProperty.m_Scope = Scope;
            }

            if (DT_SUCCESS(Status))
            {
                DT_ASSERT((pOutBuf->m_GetProperty.m_Scope&PROPERTY_SCOPE_DTAPI) ==
                                                                    PROPERTY_SCOPE_DTAPI);
            }
            break;
         case DTU_IOCTL_GET_DEV_INFO:
            pOutBuf->m_GetDevInfo.m_ProductId = pDvcData->m_DevInfo.m_ProductId;
            pOutBuf->m_GetDevInfo.m_VendorId = pDvcData->m_DevInfo.m_VendorId;
            pOutBuf->m_GetDevInfo.m_UsbAddress = pDvcData->m_DevInfo.m_UsbAddress; 
            pOutBuf->m_GetDevInfo.m_TypeNumber = pDvcData->m_DevInfo.m_TypeNumber;
            pOutBuf->m_GetDevInfo.m_HardwareRevision =
                                                   pDvcData->m_DevInfo.m_HardwareRevision; 
            pOutBuf->m_GetDevInfo.m_FirmwareVersion =
                                                    pDvcData->m_DevInfo.m_FirmwareVersion;
            pOutBuf->m_GetDevInfo.m_FirmwareVariant =
                                                    pDvcData->m_DevInfo.m_FirmwareVariant;
            pOutBuf->m_GetDevInfo.m_Serial = pDvcData->m_DevInfo.m_Serial;
            pOutBuf->m_GetDevInfo.m_UsbSpeed = pDvcData->m_DevInfo.m_UsbSpeed;
            break;
        case DTU_IOCTL_VPD_CMD:
            Status = DtuVpdIoctl(pDvcData, pIoctl);
            break;
        case DTU_IOCTL_GET_DRIVER_VERSION:
            pOutBuf->m_GetDriverVersion.m_Major = DTU_VERSION_MAJOR;
            pOutBuf->m_GetDriverVersion.m_Minor = DTU_VERSION_MINOR;
            pOutBuf->m_GetDriverVersion.m_Micro = DTU_VERSION_MICRO;
            pOutBuf->m_GetDriverVersion.m_Build = DTU_VERSION_BUILD;
            break;
        case DTU_IOCTL_I2C_CMD:
            Status = DtuI2cIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTU_IOCTL_REG_READ:
            Status = DtuRegRead(pDvcData, pInBuf->m_RegRead.m_RegAddr, 
                                                             &pOutBuf->m_RegRead.m_Value);
            break;
        case DTU_IOCTL_REG_WRITE_MASKED:
            Status = DtuRegWriteMasked(pDvcData, pInBuf->m_RegWriteMasked.m_RegAddr, 
                                                     pInBuf->m_RegWriteMasked.m_FieldMask, 
                                                    pInBuf->m_RegWriteMasked.m_FieldShift, 
                                                   pInBuf->m_RegWriteMasked.m_FieldValue);
            break;
        case DTU_IOCTL_GET_EVENT:
            Status = DtuEventsGet(pDvcData, pFile, &pOutBuf->m_GetEvent.m_EventType,
                                                    &pOutBuf->m_GetEvent.m_Value1,
                                                    &pOutBuf->m_GetEvent.m_Value2, FALSE);
            break;
        case DTU_IOCTL_REGISTER_EVENTS:
            Status = DtuEventsRegister(pDvcData, pFile,
                                                pInBuf->m_RegisterEvents.m_EventTypeMask);
            break;
        case DTU_IOCTL_UNREGISTER_EVENTS:
            DtuEventsUnregister(pDvcData, pFile);
            break;
        case DTU_IOCTL_READ_DATA:
            {
                UInt8*  pBuffer = NULL;
                Int  ReqSize = 0;
                Int  NumRead = 0;
#if defined(WINBUILD)
                PMDL  pMdl;
                
                // Retrieve MDL and buffer from request object
                WdfRequestRetrieveOutputWdmMdl(pIoctl->m_WdfRequest, &pMdl);
                pBuffer = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);
                if (pBuffer == NULL)
                    Status = DT_STATUS_OUT_OF_MEMORY;
                else
                    ReqSize = MmGetMdlByteCount(pMdl);
#else  // LINBUILD
                ReqSize = pInBuf->m_ReadData.m_NumBytesToRead;
#if defined(LIN32)
                pBuffer = (char*)(UInt32)pInBuf->m_ReadData.m_BufferAddr;
#else
                pBuffer = (char*)(UInt64)pInBuf->m_ReadData.m_BufferAddr;
#endif
#endif
                if (DT_SUCCESS(Status))
                    Status = DtuRead(pDvcData, pInBuf->m_ReadData.m_PortIndex,
                                                              pBuffer, ReqSize, &NumRead);
                
#if defined(WINBUILD)
                if (DT_SUCCESS(Status))
                    pIoctl->m_OutputBufferBytesWritten = NumRead;
                else
                    pIoctl->m_OutputBufferBytesWritten = 0;
#else // LINBUILD
                // For Linux, the pIoctl->m_OutputBufferBytesWritten contains the output
                // struct size
                if (DT_SUCCESS(Status))
                    pOutBuf->m_ReadData.m_NumBytesRead = NumRead;
                else
                    pOutBuf->m_ReadData.m_NumBytesRead = 0;
#endif
            }
            break;
        case DTU_IOCTL_WRITE_DATA:
            {
                UInt8*  pBuffer = NULL;
                Int  Size = 0;
#if defined(WINBUILD)
                PMDL  pMdl;
                
                // Retrieve MDL and buffer from request object
                WdfRequestRetrieveOutputWdmMdl(pIoctl->m_WdfRequest, &pMdl);
                pBuffer = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);
                if (pBuffer == NULL)
                    Status = DT_STATUS_OUT_OF_MEMORY;
                else
                    Size = MmGetMdlByteCount(pMdl);
#else // LINBUILD
                Size = pInBuf->m_WriteData.m_NumBytesToWrite;
#if defined(LIN32)
                pBuffer = (char*)(UInt32)pInBuf->m_WriteData.m_BufferAddr;
#else
                pBuffer = (char*)(UInt64)pInBuf->m_WriteData.m_BufferAddr;
#endif
#endif
                if (DT_SUCCESS(Status))
                    Status = DtuWrite(pDvcData, pInBuf->m_WriteData.m_PortIndex, 
                                                                           pBuffer, Size);
            }
            break;
        case DTU_IOCTL_GET_IOCONFIG:
            // Update required buffer sizes
            InReqSize += pInBuf->m_GetIoConfig.m_IoConfigCount * sizeof(DtIoConfigId);
            OutReqSize += pInBuf->m_GetIoConfig.m_IoConfigCount * sizeof(DtIoConfigValue);

            // Check input+output parameter size
            if (pIoctl->m_InputBufferSize < InReqSize)
            {
                DtDbgOut(ERR, DTU, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) OUTPUT BUFFER TOO"
                                          " SMALL!",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
                Status = DT_STATUS_INVALID_PARAMETER;
            }
            // check output buffer size
            else if (pIoctl->m_OutputBufferSize < OutReqSize)
            {
                DtDbgOut(ERR, DTU, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) OUTPUT BUFFER TOO"
                                          " SMALL!",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
                Status = DT_STATUS_INVALID_PARAMETER;

            }
            else
            {
                Status = DtuIoConfigGet(pDvcData, pFile, 
                                                   pInBuf->m_GetIoConfig.m_IoCfgId,
                                                   pOutBuf->m_GetIoConfig.m_IoCfgValue,
                                                   pInBuf->m_GetIoConfig.m_IoConfigCount);
                // Set actual number of bytes written/returned
                if (DT_SUCCESS(Status))
                    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

            }
            break;
        case DTU_IOCTL_SET_IOCONFIG:
            if (pIoctl->m_InputBufferSize < sizeof(DtuIoctlSetIoConfigInput) +
                                pInBuf->m_SetIoConfig.m_IoConfigCount*sizeof(DtIoConfigPars))
                Status = DT_STATUS_INVALID_PARAMETER;
            else
                Status = DtuIoConfigSet(pDvcData, pFile,
                                                   pInBuf->m_SetIoConfig.m_IoConfig,
                                                   pInBuf->m_SetIoConfig.m_IoConfigCount);
            break;
        case DTU_IOCTL_NONIP_CMD:
            Status = DtuNonIpIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTU_IOCTL_NONIP_RX_CMD:
            Status = DtuNonIpRxIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTU_IOCTL_NONIP_TX_CMD:
            Status = DtuNonIpTxIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTU_IOCTL_RESET_SOFT:
            Status = DtuDvcReset(pDvcData);
            break;
        case DTU_IOCTL_GET_TABLE: 
            Status = DtTableGet(pPropData, pInBuf->m_GetTable.m_Name,
                     pInBuf->m_GetTable.m_PortIndex, pInBuf->m_GetTable.m_MaxNumEntries,
                     &pOutBuf->m_GetTable.m_NumEntries, pOutBuf->m_GetTable.m_TableEntry,
               pIoctl->m_OutputBufferSize-OFFSETOF(DtuIoctlGetTableOutput, m_TableEntry));
            if (DT_SUCCESS(Status)) {
                if (pInBuf->m_GetTable.m_MaxNumEntries>=pOutBuf->m_GetTable.m_NumEntries)
                    pIoctl->m_OutputBufferBytesWritten += 
                                  pOutBuf->m_GetTable.m_NumEntries * sizeof(DtTableEntry);
            }
            break;
        case DTU_IOCTL_GET_STR_PROPERTY:
            // Get for specific type or for the attached devices
            if (pInBuf->m_GetProperty.m_TypeNumber == -1)
            {
                // Get the property for the current device
                Status = DtPropertiesStrGet(pPropData, pInBuf->m_GetStrProperty.m_Name,
                                                 pInBuf->m_GetStrProperty.m_PortIndex,
                                                 pOutBuf->m_GetStrProperty.m_Str,
                                                      &pOutBuf->m_GetStrProperty.m_Scope);
            }
            else
            {
                // Property for a specific type was requested
                Status = DtPropertiesStrGetForType(pInBuf->m_GetStrProperty.m_TypeNumber,
                                              pInBuf->m_GetStrProperty.m_HardwareRevision, 
                                              pInBuf->m_GetStrProperty.m_FirmwareVersion,
                                              pInBuf->m_GetStrProperty.m_Name,
                                              pInBuf->m_GetStrProperty.m_PortIndex,
                                              pOutBuf->m_GetStrProperty.m_Str,
                                                      &pOutBuf->m_GetStrProperty.m_Scope);
            }

            if (DT_SUCCESS(Status))
            {
                DT_ASSERT((pOutBuf->m_GetProperty.m_Scope&PROPERTY_SCOPE_DTAPI) ==
                                                                    PROPERTY_SCOPE_DTAPI);
            }
            break;
         case DTU_IOCTL_GET_DEV_SUBTYPE:
             pOutBuf->m_GetDevSubType.m_SubType = pDvcData->m_DevInfo.m_SubType;
            break;
        default:
            Status = DT_STATUS_NOT_SUPPORTED;
            break;
        }
    }

    // If we failed, no data has te be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED) 
            DtDbgOut(MIN, DTU, "Ioctl: %xh NOT SUPPORTED", pIoctl->m_IoctlCode);
    }
    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuGetNonIpPortIndex -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuGetNonIpPortIndex(
    DtuDeviceData*  pDvcData,
    Int  PortIndex,
    Int*  pNonIpPortIndex)
{
    if (PortIndex >= pDvcData->m_NumPorts)
        return DT_STATUS_NOT_FOUND;
    if (pDvcData->m_PortLookup[PortIndex].m_PortType != DTU_PORT_TYPE_NONIP)
        return DT_STATUS_NOT_FOUND;

    *pNonIpPortIndex = pDvcData->m_PortLookup[PortIndex].m_Index;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuGetPortIndexNonIp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuGetPortIndexNonIp(
    DtuDeviceData*  pDvcData, 
    Int  PortIndex,
    Int*  pNonIpPortIndex)
{
    if (PortIndex >= pDvcData->m_NumPorts)
        return DT_STATUS_NOT_FOUND;
    if (pDvcData->m_PortLookup[PortIndex].m_PortType != DTU_PORT_TYPE_NONIP)
        return DT_STATUS_NOT_FOUND;

    *pNonIpPortIndex = pDvcData->m_PortLookup[PortIndex].m_Index;

    return DT_STATUS_OK;

}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuCalculateNumPorts -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function calculates the number of NonIp and IpPorts and fills the reverse lookup
// table
//
DtStatus  DtuCalculateNumPorts(DtuDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  PortCount;
    Bool  CapAsi, CapMod, CapDemod;
    Int  i;
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    
    pDvcData->m_NumNonIpPorts = 0;
    pDvcData->m_NumIpPorts = 0; // Always 0 for now 
    
    // Get number of ports property
    PortCount = DtPropertiesGetInt(pPropData, "PORT_COUNT", -1);

    if (PortCount > MAX_PORTS)
        return DT_STATUS_CONFIG_ERROR;

    // Initialize reverse lookup structures
    for (i=0; i<PortCount; i++)
    {
        CapAsi = DtPropertiesGetBool(pPropData, "CAP_ASI", i);
        CapMod = DtPropertiesGetBool(pPropData, "CAP_MOD", i);
        CapDemod = DtPropertiesGetBool(pPropData, "CAP_DEMOD", i);
        
        if (CapAsi || CapDemod || CapMod)
        {
            // Setup reverse lookup
            pDvcData->m_PortLookup[i].m_PortType = DTU_PORT_TYPE_NONIP;
            pDvcData->m_PortLookup[i].m_Index = pDvcData->m_NumNonIpPorts;

            // We found one, increment number nonip ports
            pDvcData->m_NumNonIpPorts++;
        } else {
            Status = DT_STATUS_NOT_SUPPORTED;
            DtDbgOut(ERR, DTU, "[%d] Port type not supported", i);
            break;
        }
    }
    pDvcData->m_NumPorts = pDvcData->m_NumNonIpPorts + pDvcData->m_NumIpPorts;

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuPortsCleanUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtuPortsCleanUp(DtuDeviceData* pDvcData)
{ 
    Int  i;
    Int  PortIndex;
    
    // Cleanup NonIp port structures
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        if (DT_SUCCESS(DtuGetPortIndexNonIp(pDvcData, i, &PortIndex)))
            DtuNonIpCleanup(pDvcData, PortIndex, &pDvcData->m_NonIpPorts[i]);
        else
            DT_ASSERT(FALSE);
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuPortsInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuPortsInit(DtuDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i;
    Int  PortIndex;
    
    // Initialize NonIp port structures
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        if (DT_SUCCESS(DtuGetPortIndexNonIp(pDvcData, i, &PortIndex)))
        {
            Status = DtuNonIpInit(pDvcData, PortIndex, &pDvcData->m_NonIpPorts[i]);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, DTU, "[%d] Error initialising NonIp Port %i.", i, 
                                                                               PortIndex);
                return Status;
            }
        } else
            DT_ASSERT(FALSE);
    }

    return Status;
}

