//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* EzUsb.c *#*#*#*#*#*#*#*#*#* (C) 2011-2012 DekTec
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
    //      treat as a cold roboot => upload EzUsb firmware and re-enumerate
    *pReEnumerate = FALSE;

    IsEzUsbFwLoaded = EzUsbIsFirmwareLoaded(pDvcData);
    IsPldFwLoaded = DtuPldIsFirmwareLoaded(pDvcData);
    if (!IsEzUsbFwLoaded || IsPldFwLoaded)
    {
        if (IsPldFwLoaded)
            DtDbgOut(MIN, DTU, "PLD FW is already loaded => warm reboot");
        else
            DtDbgOut(MIN, DTU, "No EzUsb firmware loaded => cold reboot");

        // Lookup firmware
        pEzUsbFirmware = DtuGetEzUsbFirmware(pDvcData->m_DevInfo.m_ProductId,
                                                  pDvcData->m_DevInfo.m_HardwareRevision);
        if (pEzUsbFirmware == NULL)
        {
            DtDbgOut(ERR, DTU, "No EzUsb firmware avialable for DTU-%d", 
                                                        pDvcData->m_DevInfo.m_TypeNumber);
            return DT_STATUS_FAIL;
        }

        // Upload firmware for EzUsb chip
        Status = EzUsbLoadFirmware(pDvcData, pEzUsbFirmware);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTU, "Failed to upload FX firmware (Status=0x%08X)", Status);
            return Status;
        }
        *pReEnumerate = !IsPldFwLoaded;   // Device will reenumerate, if cold reboot
        return DT_STATUS_OK;
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
