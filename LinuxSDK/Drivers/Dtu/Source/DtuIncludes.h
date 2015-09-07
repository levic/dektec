//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtuIncludes.h *#*#*#*#*#*#*#*# (C) 2011-2015 DekTec
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

#ifndef __DTU_INLCUDES_H
#define __DTU_INLCUDES_H

#ifdef WINBUILD
// When building under linux we get warnings for unused variables. Enable this specific
// warning as well when building for linux so it gets spotted earlier.
#pragma warning(3 : 4101)
#endif

// OS wrapper
#include <DtSal.h>
#include <DtuIal.h>

// Driver common
#include <DtDrvCommon.h>

// Forward declaration of the device context
typedef struct _DtuDeviceData DtuDeviceData;
typedef struct _DtuShBuffer DtuShBuffer;

#include <DtuCommon.h>
#include <Vpd.h>



#include <Messages.h>

// Dtu sub-module headers
#include <DtuDevice.h>
#include <DtFwbRegs.h>
#include <DtRegs.h>
#include <DtuRegs.h>
#include <DtuVendorCmds.h>
#include <DtuUtility.h>
#include <Events.h>
#include <Firmware.h>
#include <Eeprom.h>
#include <EzUsb.h>
#include <Pld.h>
#include <Demod.h>
#include <Vpd.h>
#include <I2c.h>
#include <IoConfig.h>
#include <ShBuffer.h>
#include <NonIp.h>
#include <ReadWrite.h>


// Dtu.h
#include <Dtu.h>



#endif // __DTU_INLCUDES_H
