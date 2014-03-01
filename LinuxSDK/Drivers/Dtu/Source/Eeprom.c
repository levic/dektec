//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Eeprom.c *#*#*#*#*#*#*#*#*# (C) 2011-2012 DekTec
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEepromInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtuEepromInit(DtuDeviceData* pDvcData)
{
    // Initialisation VPD/Eeprom access mutex
    DtFastMutexInit(&pDvcData->m_Vpd.m_EepromMutex);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEepromCleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtuEepromCleanup(DtuDeviceData* pDvcData)
{
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEepromEzUsbRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuEepromEzUsbRead(
    DtuDeviceData* pDvcData,
    UInt StartOffset,
    UInt8* pBuf,
    UInt Length)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt BurstSize, BufIndex;
    UInt  BytesLeft = Length;

    // Nothing to transfer
    if (BytesLeft == 0)
        return DT_STATUS_OK;

    BufIndex = 0;
    while (BytesLeft > 0)
    {
        // Do not transfer more than maximum allowed
        BurstSize = ( BytesLeft > EZUSB_MAX_CTRL_TR_SIZE ) ? 
                                    EZUSB_MAX_CTRL_TR_SIZE : BytesLeft;

        Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB_READ_EEPROM, 
                                    StartOffset + BufIndex, // Continue transfer
                                    0, DT_USB_DEVICE_TO_HOST, 
                                                          pBuf+BufIndex, BurstSize, NULL, MAX_USB_REQ_TIMEOUT);

        // Transfer must be succesful
        if (!DT_SUCCESS(Status))
            break;
       
        // update bytes left and index with buffer
        BytesLeft -= BurstSize;
        BufIndex += BurstSize;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEepromEzUsbWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuEepromEzUsbWrite(
    DtuDeviceData* pDvcData,
    UInt StartOffset,
    UInt8* pBuf,
    UInt Length)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt BurstSize, BufIndex;
    UInt  BytesLeft = Length;

    // Nothing to transfer
    if (BytesLeft == 0)
        return DT_STATUS_OK;

    BufIndex = 0;
    while (BytesLeft > 0)
    {
        // Do not transfer more than maximum allowed
        BurstSize = ( BytesLeft > EZUSB_MAX_CTRL_TR_SIZE ) ? 
                                    EZUSB_MAX_CTRL_TR_SIZE : BytesLeft;

        Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB_WRITE_EEPROM, 
                                     StartOffset+BufIndex, 0, DT_USB_HOST_TO_DEVICE, 
                                     pBuf+BufIndex, BurstSize, NULL, MAX_USB_REQ_TIMEOUT);
        
        // Transfer must be succesful
        if (!DT_SUCCESS(Status))
            break;
       
        // update bytes left and index with buffer
        BytesLeft -= BurstSize;
        BufIndex += BurstSize;
    }
    return Status;
}

