//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Max6639.c *#*#*#*#*#*#*#*#* (C) 2012-2016 DekTec
//
// Dta driver - Max6639 - Implementation of Max6639 interface functions

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2012 DekTec Digital Video B.V.
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
#include <DtaIncludes.h>

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#define DTA_MAX6639_I2C_TIMEOUT     100

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaMax6639WriteReg(UInt DvcAddr, DtaDeviceData* pDvcData, UInt8 RegAddr,
                                                                             UInt8 Value);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMax6639Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Get device I2C address, and load the register values table
//
DtStatus  DtaMax6639Init(DtaDeviceData* pDvcData)
{   
    DtStatus  Status = DT_STATUS_OK;
    UInt  OldPropertyNotFoundCounter = pDvcData->m_PropData.m_PropertyNotFoundCounter;

    DtPropertyData*  pPropData = &pDvcData->m_PropData;
        
    pDvcData->m_FanControl.m_Max6639.m_pDvcData = pDvcData;    
    pDvcData->m_FanControl.m_Max6639.m_I2cAddress = 0;
    pDvcData->m_FanControl.m_Max6639.m_NumRegValues = 0;

    // Get the I2C address of the controller 
    pDvcData->m_FanControl.m_Max6639.m_I2cAddress = 
                                     DtPropertiesGetUInt16(pPropData, "FAN_I2C_ADDR", -1);

    if (OldPropertyNotFoundCounter != pDvcData->m_PropData.m_PropertyNotFoundCounter)  
        return DT_STATUS_NOT_FOUND;    

    // Fan control doesn't need to be initialized for the DTA-2154 starting with fw v5.
    // We currently have no way to give the table a max fw version so instead we
    // hardcode this here.
    if (pDvcData->m_PropData.m_TypeNumber==2154 &&
                                                pDvcData->m_PropData.m_FirmwareVersion>=5)
        return DT_STATUS_OK;

    // Get the register values 
    // BUG ALERT: we address port 1 (index 0), since currently the tables/capparser
    // cannot cope with device specific tables, see also TT#2093.
    Status = DtTableGet(pPropData, "FAN_CONTROL_MAX6639", 0, MAX_MAX6639_REG_VALUES,
                               &pDvcData->m_FanControl.m_Max6639.m_NumRegValues,
                               pDvcData->m_FanControl.m_Max6639.m_RegisterValues, 
                               sizeof(pDvcData->m_FanControl.m_Max6639.m_RegisterValues));

    // Not an error if there is no table defined:
    // In this case the controller is initialised by the firmware.
    if (Status == DT_STATUS_NOT_FOUND)
        return DT_STATUS_OK;
        
    // Return error
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, FAN, "Failed to load the fan control table. Error: 0x%x", Status);        
        return Status;
    }

    DT_ASSERT(MAX_MAX6639_REG_VALUES >= pDvcData->m_FanControl.m_Max6639.m_NumRegValues);

    DtDbgOut(MAX, FAN, 
                    "Found %d register values to initialise. Device I2C address: 0x%02X ", 
                    pDvcData->m_FanControl.m_Max6639.m_NumRegValues,
                    pDvcData->m_FanControl.m_Max6639.m_I2cAddress);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMax6639InitPowerUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaMax6639InitPowerUp(DtaMax6639* pMax6639)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt  i;
    UInt8  Register;
    UInt8  Value;    
    DtaDeviceData*  pDvcData = pMax6639->m_pDvcData;

    // Paranoia check, If the device is not software controlled return.
    if  (pDvcData->m_FanControl.m_SwControlled == FALSE)
        return DT_STATUS_OK;
    
    // Get I2C lock.
    // NOTE: Use DT_INVALID_FILE_OBJECT_PTR signals the driver wants to own the lock
    Status = DtaI2cLock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, 
                                                                 DTA_MAX6639_I2C_TIMEOUT);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, FAN, "Failed to get I2C lock. Error: 0x%x", Status);        
        return DT_STATUS_FAIL;
    }

    // First reset the controller
    Status = DtaMax6639WriteReg(pMax6639->m_I2cAddress,
                      pDvcData, DTA_MAX6639_GLOBAL_CONFIG, DTA_MAX6639_GLOBAL_CONFIG_RST);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, FAN, "Failed to reset the fan controller. Error: 0x%x", Status);
        DtaI2cUnlock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, FALSE);
        return DT_STATUS_FAIL;        
    }
    
    // Write settings from the table to the MAX6639.    
    for (i=0; i < pMax6639->m_NumRegValues; i++)
    {
        Register = (UInt8)pMax6639->m_RegisterValues[i].m_X;
        Value = (UInt8)pMax6639->m_RegisterValues[i].m_Y;

        DtDbgOut(MIN, FAN, "Writing: Register 0x%02x Value:0x%02X",Register, Value);
        Status = DtaMax6639WriteReg(pMax6639->m_I2cAddress, pDvcData, Register, Value);
                
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, FAN, "Failed to write 0x%02x to 0x%02X: 0x%X", 
                                                                 Register, Value, Status);
            break;
        }        
    }

    DtaI2cUnlock(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, FALSE);
    return Status;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMax6639WriteReg -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaMax6639WriteReg(
    UInt  DvcAddr, 
    DtaDeviceData*  pDvcData, 
    UInt8 RegAddr,  UInt8 Value)
{
    UInt8  I2cBuf[2];
    DtStatus  Status = DT_STATUS_OK;   

    // Write register
    I2cBuf[0] = RegAddr; I2cBuf[1] = Value;
    Status = DtaI2cWrite(pDvcData, -1, DT_INVALID_FILE_OBJECT_PTR, DvcAddr, 2, I2cBuf); 
  
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, GENL, "Failed to write register (addr=0x%02X, value=0x%02X). "
                                                   "Error: 0x%x", RegAddr, Value, Status);
    }
    return Status;
}

