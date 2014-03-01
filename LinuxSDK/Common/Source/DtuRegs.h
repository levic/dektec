//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtuRegs.h *#*#*#*#*#*#*#*#*# (C) 2011-2012 DekTec
//
// Dtu driver - Definition of register sets of DTU USB adapaters.

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

#ifndef __DTUREGS_H
#define __DTUREGS_H

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ General register offsets +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// The general registers base address
#define DTU_GEN_BASE_ADDR           0x0000

//.-.-.-.-.-.-.-.-.-.-.- Control0 register: Access Functions -.-.-.-.-.-.-.-.-.-.-.
#define DTU_GEN_CONTROL0_PWREN_MSK     0x00000030     // DTU-215 specific Vdd
#define DTU_GEN_CONTROL0_PWREN_SH      4



//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DTU-215 I2C addresses -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#define DTU215_IIC_ADDR_PWR_SUPPLY     0xCA    // I2C adress of onboard power supply
// DTU215 I2C power-supply register address
#define DTU215_IIC_ADDR_PWR_1V2        0x10    // 1V2 power supply control register
#define DTU215_IIC_ADDR_PWR_1V5        0x20    // 1V5 power supply control register
#define DTU215_IIC_ADDR_PWR_1V8        0x40    // 1V8 power supply control register
#define DTU215_IIC_ADDR_PWR_3V3        0x80    // 3V3 power supply control register
// Power supply control data
#define DTU215_IIC_PWR_1V2_CTRL        0x98    // 1V2 power supply control data
#define DTU215_IIC_PWR_1V5_CTRL        0xA4    // 1V5 power supply control data
#define DTU215_IIC_PWR_1V8_CTRL        0x8F    // 1V8 power supply control data
#define DTU215_IIC_PWR_3V3_CTRL        0x8F    // 3V3 power supply control data

//=+=+=+=+=+=+=+=+=+=+=+=+=+ Declarations below are driver ONLY +=+=+=+=+=+=+=+=+=+=+=+=+=

#ifdef USB_DRIVER

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public interface -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtuRegRead(DtuDeviceData*  pDvcData, Int  RegAddr, UInt32*  pValue);
DtStatus  DtuRegWrite(DtuDeviceData*  pDvcData, Int  RegAddr, UInt32  Value);
DtStatus  DtuRegWriteMasked(DtuDeviceData*  pDvcData, Int  RegAddr, UInt32  Mask, 
                                                            UInt32  Shift, UInt32  Value);
DtStatus  Dtu3RegRead(DtuDeviceData*  pDvcData, UInt16  Dvc, UInt16  RegAddr,
                                                                         UInt16*  pValue);
DtStatus  Dtu3RegWrite(DtuDeviceData*  pDvcData, UInt16  Dvc, UInt16  RegAddr,
                                                                           UInt16  Value);
#endif // #ifdef USB_DRIVER

#endif // #ifndef __DTUREGS_H
