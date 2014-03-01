//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Vpd.h *#*#*#*#*#*#*#*#*#*# (C) 2011-2012 DekTec
//
// Dtu driver - Vital Product Data (VPD) read/write routines.
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

#ifndef __VPD_H
#define __VPD_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// DtuVpd
typedef struct _DtuVpd
{
    // Vpd properties
    Int   m_EepromIoItf;
    UInt  m_EepromPageSize;       // Size of 1 EEPROM page
    UInt  m_EepromSize;           // Size of the complete EEPROM, only used by EPC
                                  // implementation to calculate last sector address.

    // Implementation data
    UInt8*  m_pCache;             // Physical VPD cache buffer
    UInt  m_IdOffset;             // Offset in the VPD data where the DekTec ID section
                                  // starts
    UInt  m_IdLength;             // Length of Identifier string
    UInt  m_RoOffset;             // Offset of start PCI VPD-R
    UInt  m_RoLength;             // Length of PCI VPD-R
    UInt  m_RwOffset;             // Offset of start PCI VPD-W
    UInt  m_RwLength;             // Length of PCI VPD-W
    UInt  m_IntRoOffset;          // Offset of start internal VPD-R
    UInt  m_IntRoLength;          // Length of internal VPD-R
    UInt  m_IntRwOffset;          // Offset of start internal VPD-W
    UInt  m_IntRwLength;          // Length of internal VPD-W
    DtFastMutex  m_EepromMutex;
    DtFastMutex  m_IoctlMutex;
} DtuVpd;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  VPD_EEPROM_IO_PLX   0
#define  VPD_EEPROM_IO_FPGA  1
#define  VPD_EEPROM_IO_SPI   2
#define  VPD_EEPROM_IO_EZUSB 3

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtuVpdInit(DtuDeviceData* pDvcData);
void  DtuVpdCleanup(DtuDeviceData* pDvcData);
DtStatus  DtuVpdIoctl(DtuDeviceData* pDvcData, DtIoctlObject* pIoctl);
DtStatus  DtuVpdReadId(DtuDeviceData* pDvcData, char* pVpdId, UInt* pLength);
DtStatus  DtuVpdReadItemRo(DtuDeviceData* pDvcData, UInt KeywordLen, const char* pKeyword, 
                                                           char* pVpdItem, UInt* pLength);
DtStatus  DtuVpdReadItemRw(DtuDeviceData* pDvcData, UInt KeywordLen, const char* pKeyword, 
                                                           char* pVpdItem, UInt* pLength);



#endif // __VPD_H

