//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtuUtility.h *#*#*#*#*#*#*#*# (C) 2011-2012 DekTec
//
// Dtu driver - DTU utility functions.
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

#ifndef __DTU_UTILITY_H
#define __DTU_UTILITY_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Special DTU-2XX product IDs
#define DTU2xx_PID_UNIT_EEPROM      0x8613      // PID of an uninitialised DTU-2xx 
                                                // (=PID of FX2)
#define DTU225_PID_NOFW_OLD         0x0100      // Uninitilaised DTU-225 PID 
#define DTU225_PID_OLD              0x0200      // Initilaised DTU-225 PID 
#define DTU2x5_MANUF                0x0237      // DTU-2x5 in manuf test mode
#define DTU2xx_MANUF                0x0263      // DTU-2XX device with manuf code 

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

UInt16  DtuProductId2TypeNumber(Int ProductId);


DtStatus  DtuPropertiesInit(DtuDeviceData* pDvcData);
DtStatus  DtuPropertiesReportDriverErrors(DtuDeviceData* pDvcData);
DtStatus  DtuGetUsbAddress(DtuDeviceData*  pDvcData, Int*  pUsbAddr);
DtStatus  DtuGetUsbSpeed(DtuDeviceData*  pDvcData, UInt32*  pUsbSpeed);


#endif // #ifndef __DTU_UTILITY_H

