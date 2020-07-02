//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* FanControl.c *#*#*#*#*#*#*#*#*# (C) 2012-2016 DekTec
//
// Dta driver - Implements fan control support
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2012-2016 DekTec Digital Video B.V.
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
#include <DtaIncludes.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaFanControlInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaFanControlInit(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    UInt  OldPropertyNotFoundCounter = pPropData->m_PropertyNotFoundCounter;
    
    // Assume not sw controlled
    pDvcData->m_FanControl.m_SwControlled = FALSE;
    pDvcData->m_FanControl.m_FanCount = 0;    

    // Get fan type.
    // Do not set an error if not defined (not supported)
    pDvcData->m_FanControl.m_FanType = DtPropertiesGetInt(pPropData, "FAN_TYPE", -1);
    if (pDvcData->m_FanControl.m_FanType!=FAN_TYPE_MAX6639
                                     && pDvcData->m_FanControl.m_FanType != FAN_TYPE_FANM
                                     && pDvcData->m_FanControl.m_FanType != FAN_TYPE_FANC)
    {     
        pPropData->m_PropertyNotFoundCounter = OldPropertyNotFoundCounter;      
        return DT_STATUS_OK;
    }

    // Get fan count.
    // Do not set an error if fan count is 0.
    pDvcData->m_FanControl.m_FanCount = DtPropertiesGetInt(pPropData, "FAN_COUNT", -1);
    if (pDvcData->m_FanControl.m_FanCount <= 0)
    {
        DtDbgOut(ERR, FAN, "Failed to read number of fans error: 0x%x", Status);
        pPropData->m_PropertyNotFoundCounter = OldPropertyNotFoundCounter;
        return DT_STATUS_OK;
    }

    // Get correction foctor for computing the rotations per minute
    // Do not set an error if not defined
    pDvcData->m_FanControl.m_RpmDiv = DtPropertiesGetInt(pPropData, "FAN_RPM_DIVISOR",
                                                                                      -1);
    pDvcData->m_FanControl.m_RpmMult = DtPropertiesGetInt(pPropData, "FAN_RPM_MULTIPLIER",
                                                                                      -1);
    pDvcData->m_FanControl.m_RpmMinimum = DtPropertiesGetInt(pPropData, "FAN_RPM_MINIMUM",
                                                                                      -1);
    pPropData->m_PropertyNotFoundCounter = OldPropertyNotFoundCounter;
    // Set defaults when properties are not found
    if (pDvcData->m_FanControl.m_RpmDiv <= 0)
        pDvcData->m_FanControl.m_RpmDiv = 1;
    if (pDvcData->m_FanControl.m_RpmMult <= 0)
        pDvcData->m_FanControl.m_RpmMult = 1;
    // Call fan specific initialization
    if (pDvcData->m_FanControl.m_FanType == FAN_TYPE_MAX6639)
    {
        Status = DtaMax6639Init(pDvcData);
        // If the table for MAX6639 is filled, it must be software controlled 
        if (Status==DT_STATUS_OK && pDvcData->m_FanControl.m_Max6639.m_NumRegValues>0)        
            pDvcData->m_FanControl.m_SwControlled = TRUE;        
    }

    if (!DT_SUCCESS(Status))
        DtDbgOut(ERR, FAN, "Failed to read number of fans error: 0x%x", Status);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaFanControlPowerUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaFanControlPowerUp(DtaDeviceData* pDvcData)
{
    DtPropertyData*  pPropData = &pDvcData->m_PropData;

    // Execute fan specific initialization
    if (pDvcData->m_FanControl.m_FanType==FAN_TYPE_FANM 
                                       || pDvcData->m_FanControl.m_FanType==FAN_TYPE_FANC)
        pDvcData->m_FanControl.m_pFwbFanRegs = pDvcData->m_pGenRegs 
                                       + DtPropertiesGetUInt16(pPropData, "FAN_ADDR", -1);

    return DT_STATUS_OK;
}
