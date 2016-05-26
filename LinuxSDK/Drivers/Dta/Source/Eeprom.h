//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Eeprom.h *#*#*#*#*#*#*#*#*# (C) 2010-2016 DekTec
//
// Dta driver - Low level EEPROM write/read functions.
//
// Low-level serial EEPROM routines:
//  - For reading and writing to 93C66 (PCI cards with PLX chip)
//  - For reading and writing EPCS1, EPCS4 (FPGA)
//  - For reading and writing SPI (separate EEPROM)
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

#ifndef __EEPROM_H
#define __EEPROM_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtaEepromInit(DtaDeviceData* pDvcData);
void  DtaEepromCleanup(DtaDeviceData* pDvcData);
DtStatus  DtaEepromPlxRead(DtaDeviceData* pDvcData, UInt StartOffset, UInt8* pBuf,
                                                                             UInt Length);
DtStatus  DtaEepromPlxWrite(DtaDeviceData* pDvcData, UInt StartOffset, UInt8* pBuf, 
                                                                             UInt Length);
DtStatus  DtaEepromSpiRead(DtaDeviceData* pDvcData, UInt StartOffset,  UInt8* pBuf, 
                                                                             UInt Length);
DtStatus  DtaEepromSpiWrite(DtaDeviceData* pDvcData, UInt StartOffset, UInt8* pBuf, 
                                                                             UInt Length);

DtStatus  DtaEepromCfiUnlock(volatile UInt8*  pCfiRegs);
DtStatus  DtaEepromCfiLock(volatile UInt8*  pCfiRegs);
DtStatus  DtaEepromCfiRead(volatile UInt8* pCfiRegs, UInt StartOffset, UInt8* pBuf,
                                                                           UInt Length);



#endif // __EEPROM_H
