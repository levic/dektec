//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* EzUsb.h *#*#*#*#*#*#*#*#*#* (C) 2011-2012 DekTec
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

#ifndef __EZUSB_H
#define __EZUSB_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "Firmware.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Build-in/EzUsb vendor commands
#define EZUSB_FX2_LOAD_INTERNAL     0xA0    // Commend for loading to internal RAM
#define EZUSB_FX2_LOAD_EXTERNAL     0xA3	// Command for loading to external RAM
#define EZUSB_FX2_ISFX2             0xAC	// Command for checking if chip is an FX2

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Internal registers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Control and Status Register.  Bit 0 controls 8051 reset
#define EZUSB_CPUCS_REG             0x7F92      
#define EZUSB_FX2_CPUCS_REG         0xE600

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#define EZUSB_MAX_CTRL_TR_SIZE	64		    // Max transfer size for control pipe

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// DtuEzUsb
typedef struct _DtuEzUsb
{
    Int  m_FirmwarePipe;        // Pipe number for uploading PLD firmware
    Int  m_ReadPipe;            // Pipe number for reading data
    Int  m_WritePipe;           // Pipe number for writing of data
} DtuEzUsb;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  EzUsbInit(DtuDeviceData* pDvcData,  Bool*  pReEnumerate);

Bool  EzUsbIsFirmwareLoaded(DtuDeviceData*  pDvcData);
DtStatus  EzUsbResetProcessor(DtuDeviceData*  pDvcData, Bool  Reset);
DtStatus  EzUsbLoadFirmware(DtuDeviceData*  pDvcData, 
                                                 const DtuIntelHexRecord*  pMicroCode);

#endif // #ifndef __EZUSB_H

