//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaIncludes.h *#*#*#*#*#*#*#*# (C) 2010-2016 DekTec
//
// Dta driver - Header file containing all standard driver includes.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2016 DekTec Digital Video B.V.
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

#ifndef __DTA_INLCUDES_H
#define __DTA_INLCUDES_H

#ifdef WINBUILD
// When building under linux we get warnings for unused variables. Enable this specific
// warning as well when building for linux so it gets spotted earlier.
#pragma warning(3 : 4101)
#endif

// OS wrapper
#include <DtSal.h>
#include <DtaIal.h>

// Driver common
#include <DtDrvCommon.h>

// Forward declaration of the device context
typedef struct _DtaDeviceData DtaDeviceData;
typedef struct _DtaChildDeviceData  DtaChildDeviceData;
typedef struct _DtaShBuffer DtaShBuffer;    // Needed for Dma.h
typedef struct _DtaIpPort DtaIpPort;        // Needed for NwPhyMac.h
typedef struct _DmaChannel DmaChannel;      // Needed for DtaUtility and Dma.h
typedef struct _DtaNonIpPort DtaNonIpPort;  

// Dta headers
#include <DtaUtility.h>
#include <DtFwbRegs.h>
#include <I2c.h>
#include <Eeprom.h>
#include <Vpd.h>
#include <Events.h>
#include <DtaCommon.h>
#include <DtRegs.h>
#include <DtaRegs.h>
#include <Pci905X.h>
#include <SpiMf.h>
#include <Uart.h>
#include <EnDec.h>
#include <SdiAvRx.h>
#include <EncD7Pro.h>
#include <Dma.h>
#include <Messages.h>
#include <IoConfig.h>
#include <ShBuffer.h>
#include <Hdmi.h>
#include <NonIp.h>
#include <NonIpRx.h>
#include <NonIpTx.h>
#include <EthPrtcls.h>
#include <NwPhyMac.h>
#include <Ip.h>
#include <IpTx.h>
#include <IpRx.h>
#include <DtaMatrix.h>
#include <Genlock.h>
#include <Gs296x.h>
#include <Rs422.h>
#include <FanControl.h>
#include <Ad9129.h>
#include <ProgItf.h>



// Dta.h
#include <Dta.h>

#endif // __DTA_INLCUDES_H
