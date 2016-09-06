//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* EzUsb.c *#*#*#*#*#*#*#*#*#* (C) 2011-2016 DekTec
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtuIncludes.h>

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EzUsbInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  EzUsbInit(DtuDeviceData* pDvcData, Bool* pReEnumerate)
{
    DtStatus  Status = DT_STATUS_OK;
    const DtuIntelHexRecord*  pEzUsbFirmware = NULL;
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    Bool  IsEzUsbFwLoaded=FALSE, IsPldFwLoaded=FALSE;
    Int  FirmwareEndpoint;
    Int  ReadEndpoint;
    Int  WriteEndpoint;
    Int  ProductId;

    // Initialize properties
    FirmwareEndpoint = DtPropertiesGetInt(pPropData, "USB_END_POINT_FIRMWARE", -1);
    ReadEndpoint = DtPropertiesGetInt(pPropData, "USB_END_POINT_READ", -1);
    WriteEndpoint = DtPropertiesGetInt(pPropData, "USB_END_POINT_WRITE", -1);
        
    // Check if no property error occurred
    Status = DtuPropertiesReportDriverErrors(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    // Check if we need to load firmware. NOTE: there are two conditions to load the 
    // firmware namely:
    //   1. EzUsb firmware is not loaded yet
    //   2. PLD firmware is loaded already, which suggest a warm-reboot which we want to
    //      treat as a cold roboot => upload EzUsb firmware, but no re-enumeration
    *pReEnumerate = FALSE;

    IsEzUsbFwLoaded = EzUsbIsFirmwareLoaded(pDvcData);
    IsPldFwLoaded = FALSE;
    if (IsEzUsbFwLoaded && !(pDvcData->m_DevInfo.m_TypeNumber>=300
                                                 && pDvcData->m_DevInfo.m_TypeNumber<400))
        IsPldFwLoaded = DtuPldIsFirmwareLoaded(pDvcData);
    if (!IsEzUsbFwLoaded || IsPldFwLoaded)
    {
        if (IsPldFwLoaded)
            DtDbgOut(MIN, DTU, "PLD FW is already loaded => warm reboot");
        else
            DtDbgOut(MIN, DTU, "No EzUsb firmware loaded => cold reboot");

        if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
        {
            // Lookup firmware
            const DtuFx3HexRecord*  pFx3Firmware = Dtu3GetFx3Firmware(
                                                pDvcData->m_DevInfo.m_TypeNumber,
                                                -1,
                                                pDvcData->m_DevInfo.m_HardwareRevision);
            if (pFx3Firmware == NULL)
                DtDbgOut(ERR, DTU, "FX3 firmware not found for Typenumber: %d,"
                                                                     " HardwareRev: 0x%X",
                                                  pDvcData->m_DevInfo.m_TypeNumber,
                                                  pDvcData->m_DevInfo.m_HardwareRevision);

            if (!DtUsbManufNameEq(&pDvcData->m_Device, "Cypress"))
            {
                DtDbgOut(ERR, DTU, "DTU-3XX vid/pid found but wrong manufacturer string");
                return DT_STATUS_FAIL;
            }
            if (pDvcData->m_DevInfo.m_ProductId == DTU3_PID_UNIT_EEPROM)
                pDvcData->m_BootState = DTU_BOOTSTATE_FACTORY_COLD;
            else
                pDvcData->m_BootState = DTU_BOOTSTATE_COLD;

            //TODOTM: verify product string is "DTU-351"

            // Upload firmware for EzUsb chip
            Status = EzUsbLoadFirmwareFx3(pDvcData, pFx3Firmware);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, DTU, "Failed to upload FX3 firmware (Status=0x%08X)", Status);
                return Status;
            }
            DtDbgOut(ERR, DTU, "FX3 firmware uploaded");
        } else {
            // Lookup FX2 firmware
            // TypeNumber is already set
            ProductId = DtuTypeNumber2ProductId(pDvcData->m_DevInfo.m_TypeNumber, 
                                                         pDvcData->m_DevInfo.m_ProductId);
            pEzUsbFirmware = DtuGetEzUsbFirmware(ProductId,
                                                  -1,
                                                  pDvcData->m_DevInfo.m_HardwareRevision);
            if (pEzUsbFirmware == NULL)
            {
                DtDbgOut(ERR, DTU, "No EzUsb firmware available for DTU-%d", 
                                                        pDvcData->m_DevInfo.m_TypeNumber);
                return DT_STATUS_FAIL;
            }

            // Upload firmware for EzUsb chip
            Status = EzUsbLoadFirmware(pDvcData, pEzUsbFirmware);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, DTU, "Failed to upload FX2 firmware (Status=0x%08X)", Status);
                return Status;
            }
        }

        *pReEnumerate = !IsPldFwLoaded;   // Device will reenumerate, if cold reboot
        if (!IsPldFwLoaded)
            return DT_STATUS_OK;  // In case of cold reboot we are done (will reenumerate)
    }

    // Convert endpoint to pipe numbers
    if (FirmwareEndpoint != -1)
    {
        // Convert endpoint to pipe number
        pDvcData->m_EzUsb.m_FirmwarePipe = DtUsbGetBulkPipeNumber(&pDvcData->m_Device,
                                                 DT_USB_HOST_TO_DEVICE, FirmwareEndpoint);
        DT_ASSERT(pDvcData->m_EzUsb.m_FirmwarePipe != -1);
    }
    
    if (ReadEndpoint != -1)
    {
        // Convert endpoint to pipe number
        pDvcData->m_EzUsb.m_ReadPipe = DtUsbGetBulkPipeNumber(&pDvcData->m_Device,
                                 DT_USB_DEVICE_TO_HOST, ReadEndpoint);
        DT_ASSERT(pDvcData->m_EzUsb.m_ReadPipe != -1);
    }
    
    if (WriteEndpoint != -1)
    {
        // Convert endpoint to pipe number
        pDvcData->m_EzUsb.m_WritePipe = DtUsbGetBulkPipeNumber(&pDvcData->m_Device,
                                DT_USB_HOST_TO_DEVICE, WriteEndpoint);
        DT_ASSERT(pDvcData->m_EzUsb.m_WritePipe != -1);
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EzUsbIsFirmwareLoaded -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Check if microcode for the EzUsb chip has been loaded
//
Bool  EzUsbIsFirmwareLoaded(DtuDeviceData* pDvcData)
{
    DtStatus  Status;
    DtString  DtStr;
    DtStringChar  DtStrBuffer[64];
    
    // Connect DtStrBuffer to DtStr
    DT_STRING_INIT(DtStr, DtStrBuffer, 64);

    // Found a uninitialised DTU-2xx device with manuf microcode
    if (pDvcData->m_DevInfo.m_ProductId == DTU2xx_MANUF)
        return TRUE;

    // For DTU-3XX devices check that the manufacturer string is "DekTec".
    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
        return DtUsbManufNameEq(&pDvcData->m_Device, "DekTec");

    //.-.-.-.-.-.-.-.- Check for the availability of a string descriptor -.-.-.-.-.-.-.-.-
    //
    // If there is no microcode there will be no string descriptor

    Status = DtUsbQueuryString(&pDvcData->m_Device, &DtStr, 1);
    if (!DT_SUCCESS(Status))
        return FALSE;

    // Must find a string descriptor with a size >= 3
    return DtStringGetStringLength(&DtStr)>=3 ? TRUE : FALSE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EzUsbResetProcessor -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  EzUsbResetProcessor(DtuDeviceData* pDvcData, Bool Reset)
{
    Int  Dummy = 0;

    // Write 1 to reset CPU and zero to clear reset
    UInt8  ResetBit = Reset ? 1 : 0;
    return DtUsbVendorRequest(&pDvcData->m_Device, NULL, EZUSB_FX2_LOAD_INTERNAL, 
                                            EZUSB_FX2_CPUCS_REG, 0, DT_USB_HOST_TO_DEVICE,
                                            &ResetBit, 1, &Dummy, MAX_USB_REQ_TIMEOUT);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EzUsbLoadFirmware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  EzUsbLoadFirmware(
    DtuDeviceData*  pDvcData,
    const DtuIntelHexRecord*  pMicroCode)
{
    DtStatus  Status;
    Int  Dummy = 0;
    DtuIntelHexRecord*  pPtr = NULL;

    if (pMicroCode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    //.-.-.-.-.-.-.-.-.-.-.-.- Download firmware to internal RAM -.-.-.-.-.-.-.-.-.-.-.-.-
    //
    // Before starting the upload, stop the 8051

    Status = EzUsbResetProcessor(pDvcData, TRUE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTU, "Failed to stop the processor (Status=0x%08X)", Status);
        return Status;
    }

    pPtr = (DtuIntelHexRecord*)pMicroCode;
    while (pPtr->Type == 0)
    {
        // Send load command
        Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, EZUSB_FX2_LOAD_INTERNAL, 
                                   pPtr->Address, 0, DT_USB_HOST_TO_DEVICE, 
                                   pPtr->Data, pPtr->Length, &Dummy, MAX_USB_REQ_TIMEOUT);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTU, "Failed to send load command (Status=0x%08X)", Status);
            return Status;
        }
        pPtr++;
    }
    // Restart the 8051
    if (Status == DT_STATUS_OK)
    {
        Status = EzUsbResetProcessor(pDvcData, FALSE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTU, "Failed to start the processor (Status=0x%08X)", Status);
            return Status;
        }
    }
  
    return DT_STATUS_OK;
}

#define FX3_MAX_CHUNK_SIZE  4096

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- EzUsbLoadFirmware -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  EzUsbLoadFirmwareFx3(
    DtuDeviceData*  pDvcData,
    const DtuFx3HexRecord*  pMicroCode)
{
    DtStatus  Status;
    Int  Dummy = 0;
    const DtuFx3HexRecord*  pPtr = pMicroCode;
    Bool  Cont = TRUE;

    if (pMicroCode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    DtDbgOut(MAX, DTU, "Loading firmware");
    
    while (Cont)
    {
        UInt32  Len = pPtr->Length;
        UInt32  Addr = pPtr->Address;
        UInt8*  Data = (UInt8*)(pPtr->Data);
        UInt32  NumToWrite;
        Cont = (pPtr->Length != 0);
        do
        {
            NumToWrite = Len < FX3_MAX_CHUNK_SIZE ? Len : FX3_MAX_CHUNK_SIZE;
            
            // Send load command
            Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, EZUSB_FX2_LOAD_INTERNAL, 
                                       Addr&0xFFFF, Addr>>16, DT_USB_HOST_TO_DEVICE, 
                                       Data, NumToWrite, &Dummy, MAX_USB_REQ_TIMEOUT);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, DTU, "Failed to send load command (Status=0x%08X)", Status);
                return Status;
            }
            Len -= NumToWrite;
            Addr += NumToWrite;
            Data += NumToWrite;
        } while (Len > 0);
        pPtr++;
    }
  
    return DT_STATUS_OK;
}
