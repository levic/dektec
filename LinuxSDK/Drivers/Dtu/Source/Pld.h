//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Pld.h *#*#*#*#*#*#*#*#*#*# (C) 2011-2015 DekTec
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

#ifndef __PLD_H
#define __PLD_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "Firmware.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ALTERA program mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DTU_START_PROG_CYCLE    1
#define DTU_STOP_PROG_CYCLE     0

#define DTU_FW_LOADED_CODE      0x5A2C

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtuPldInit(DtuDeviceData*  pDvcData);
DtStatus  DtuLoadPldFirmware(DtuDeviceData* pDvcData, const UInt8*  pFirmware, 
                                                                               Int  Size);
DtStatus  DtuFx3LoadPldFirmware(DtuDeviceData*  pDvcData, const UInt8*  pFirmware,
                                                                               Int  Size);
Bool  DtuPldIsFirmwareLoaded(DtuDeviceData*  pDvcData);

#endif // #ifndef __PLD_H

