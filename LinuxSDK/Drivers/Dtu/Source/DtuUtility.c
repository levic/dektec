//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtuUtility.c *#*#*#*#*#*#*#*# (C) 2011-2016 DekTec
//
// Dtu driver - Dtu utility functions.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtuIncludes.h>

#define DTU_PID_UNIT_EEPROM      0x8613      // PID of an uninitialised DTU-2xx 
                                                // (=PID of FX2)
#define DTU225_PID_NOFW_OLD         0x0100      // Uninitilaised DTU-225 PID 
#define DTU225_PID_OLD              0x0200      // Initilaised DTU-225 PID 

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuProductId2TypeNumber -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Deduces the type number based on the USB product ID.
//
// There are three cases:
//  1) EEPROM is uninitialised and device uses the PID of the Cypress FX2 chip
//  2) Old DTUs use a different PID (0x2##0) for when the FX2 micro code has not been 
//     loaded yet
//  3) The normal case the the PID is 0x0200 + last two digits of the type number 
//     (e.g. 0x0205 for DTU-205 (0x0200 + 5)
//
UInt16  DtuProductId2TypeNumber(Int ProductId)
{
    if (ProductId == DTU_PID_UNIT_EEPROM)
        return 299;
    else if (ProductId >= 0x2000)
        return 200 + (ProductId>>4 & 0xFF);
    else if (ProductId > 0x300)
        return 300 + (ProductId & 0xFF);
    else if (ProductId > 0x200)
        return 200 + (ProductId & 0xFF);
    else if (ProductId==DTU225_PID_OLD || ProductId==DTU225_PID_NOFW_OLD)
        return 205;
    else
        DT_ASSERT(FALSE); // Unknown device
    return 200;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuPropertiesInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuPropertiesInit(DtuDeviceData* pDvcData)
{
    DtString  DtStrType;
    DtStringChar  DtStrTypeBuffer[9];
    DtPropertyData* pPropData = &pDvcData->m_PropData;
    DtEvtLog* pEvtObject = &pDvcData->m_Device.m_EvtObject;
    DtStatus  Status;
    
    // Connect DtStrTypeBuffer to DtStrType
    DT_STRING_INIT(DtStrType, DtStrTypeBuffer,
                                            sizeof(DtStrTypeBuffer)/sizeof(DtStringChar));

    // Init properties failed?
    Status = DtPropertiesInit(pPropData);
    if (Status != DT_STATUS_OK)
    {
         // Use event to report driver errors 
        DtStringAppendChars(&DtStrType, "DTU-");
        DtStringUIntegerToDtStringAppend(&DtStrType, 10, pPropData->m_TypeNumber);
        DtEvtLogReport(pEvtObject, DTU_LOG_PROP_STORE_NOT_FOUND, &DtStrType, NULL, NULL);
    }
    return Status;
}



//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuPropertiesReportDriverErrors -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuPropertiesReportDriverErrors(DtuDeviceData* pDvcData)
{
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    DtEvtLog*  pEvtObject = &pDvcData->m_Device.m_EvtObject;

    // Errors to report?
    DtStatus  Status = DtPropertiesReportDriverErrors(pPropData);
    if (Status != DT_STATUS_OK)
    {
        // Use event to report driver errors 
        DtEvtLogReport(pEvtObject, DTU_LOG_CONFIG_FAILED,
                                        &pPropData->m_PropertyNotFoundString, NULL, NULL);
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuGetUsbAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuGetUsbAddress(
    DtuDeviceData*  pDvcData,
    Int*  pUsbAddr)      // The USB bus speed
{
    DtStatus  Status;
    Int  BytesTransf = 0;
    UInt8*  pTempBuf = NULL;

    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
    {
        // Implemented in firmware for DTU2 devices but not for DTU3. Simply return 0
        // as dummy address.
        *pUsbAddr = 0;
        return DT_STATUS_OK;
    }

    // Init to 512
    *pUsbAddr = 512;

    // Cannot use value from stack as buffer, in vendor request => allocate temp buffer
    // for reading register value from device
    pTempBuf = (UInt8*)DtMemAllocPool(DtPoolNonPaged, sizeof(UInt8)*3, DTU_TAG);
    if (pTempBuf == NULL)
        return DT_STATUS_OUT_OF_MEMORY;

    // Get USB info from DTU
    Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB_GET_DEVICE_INF0, 
                                   0, 0, DT_USB_DEVICE_TO_HOST, pTempBuf, sizeof(UInt8)*3,
                                   &BytesTransf, MAX_USB_REQ_TIMEOUT);

    if ( BytesTransf >= 3 )
    {
        *pUsbAddr = (Int)pTempBuf[2];
    }

    // Free buffer
    DtMemFreePool(pTempBuf, DTU_TAG);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuGetUsbSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuGetUsbSpeed(
    DtuDeviceData*  pDvcData,
    UInt32*  pUsbSpeed)    // The USB bus speed
{
    DtStatus  Status;
    Int  BytesTransf = 0;
    UInt8*  pTempBuf = NULL;

    // Init to zero
    *pUsbSpeed = 0;

    if (pDvcData->m_DevInfo.m_TypeNumber == 351)
    {
        // Read FX3 register 0x00: UsbSpeed
        UInt16  UsbSpeed;
        Status = Dtu35xRegRead(pDvcData, DTU_USB3_DEV_FX3, 0x00, &UsbSpeed);
        *pUsbSpeed = UsbSpeed;
    }
    else if (pDvcData->m_DevInfo.m_TypeNumber>=300 &&pDvcData->m_DevInfo.m_TypeNumber<400)
    {
        // Read FX3 register 0x00: UsbSpeed
        Status = Dtu3RegRead(pDvcData, DTU3_FX3_BLOCK_OFFSET, &FwbFx3.UsbSpeed, 
                                                                               pUsbSpeed);
    } else {
        // Cannot use value from stack as buffer, in vendor request => allocate temp
        // buffer for reading register value from device
        pTempBuf = (UInt8*)DtMemAllocPool(DtPoolNonPaged, sizeof(UInt8)*4, DTU_TAG);
        if (pTempBuf == NULL)
            return DT_STATUS_OUT_OF_MEMORY;
        // Get USB speed from DTU
        Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB_GET_USB_SPEED, 
                                   0, 0, DT_USB_DEVICE_TO_HOST, pTempBuf, sizeof(UInt8)*4,
                                   &BytesTransf, MAX_USB_REQ_TIMEOUT);
        if (BytesTransf >= 2)
        {
            *pUsbSpeed = pTempBuf[0];
            *pUsbSpeed |= (pTempBuf[1]<<8);
        }

        // Free buffer
        DtMemFreePool(pTempBuf, DTU_TAG);
    }

    return Status;
}
