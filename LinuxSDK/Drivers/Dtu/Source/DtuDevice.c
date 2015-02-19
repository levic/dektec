//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtuDevice.c *#*#*#*#*#*#*#*#* (C) 2011-2015 DekTec
//
// Dtu driver - Dtu device functions.
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
#include <DtuIncludes.h>


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDvcReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Handler for reset device command
//
DtStatus  DtuDvcReset(DtuDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  Dummy;
    Int  i=0;

    // Reset temporariy for all channels
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        DtuNonIpPort*  pNonIpPort = &(pDvcData->m_pNonIpPorts[i]);

        // Clear temporariy buffer
        pNonIpPort->m_TempBufWrIndex = 0;
        pNonIpPort->m_TempBufRdIndex = 0;
    }

    // Simply send the vendor command to the device
    Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB_RESET_DEVICE, 
                                                     0, 0, DT_USB_HOST_TO_DEVICE, NULL, 0,
                                                     &Dummy, MAX_USB_REQ_TIMEOUT);

    DtDbgOut(AVG, DTU, "DTU-%d reset", pDvcData->m_DevInfo.m_TypeNumber);

    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDvcPowerSupplyInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initializes the Altera power supply
//
DtStatus  DtuDvcPowerSupplyInit(DtuDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8  I2cBuf[8];
    Int  TypeNumber = pDvcData->m_DevInfo.m_TypeNumber;

    if (TypeNumber==215)
    {
        // Start 1V2 power supply (ALTERA core voltage)
        // NOTE: we only start the 1V2 power supply here. Other power supplies are started
        // later when we init the device hardware.
        I2cBuf[0] = DTU215_IIC_ADDR_PWR_1V2;
        I2cBuf[1] = DTU215_IIC_PWR_1V2_CTRL;
        Status = DtuI2cWrite(pDvcData, NULL, DTU215_IIC_ADDR_PWR_SUPPLY, 2, I2cBuf);
        // Sleep shortly to allow power-supply to settle
        DtSleep(200);
        if (DT_SUCCESS(Status))
            DtDbgOut(AVG, DTU, "DTU-%d power supply started", TypeNumber);
        else
            DtDbgOut(ERR, DTU, "DTU-%d power supply start ERROR. Error: %xh", TypeNumber, Status);
    }
    else if (TypeNumber==351 && pDvcData->m_DevInfo.m_UsbSpeed==2)
    {
        Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_PNP_CMD,
                           DTU_PNP_CMD_DVC_POWER, DTU_DVC_POWER_ON, DT_USB_HOST_TO_DEVICE,
                           NULL, 0, NULL, MAX_USB_REQ_TIMEOUT);
        if (DT_SUCCESS(Status))
            DtSleep(100);
    }

    return Status;
}
