//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* VpdCommon.h *#*#*#*#*#*#*#*#* (C) 2011-2016 DekTec
//
// Driver common - Vital Product Data (VPD) read/write routines.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2016 DekTec Digital Video B.V.
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

#ifndef __DT_VPD_COMMON_H
#define __DT_VPD_COMMON_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// 
typedef struct _DtVpd
{
    // Vpd properties
    Int   m_EepromIoItf;
    UInt  m_EepromPageSize;       // Size of 1 EEPROM page
    UInt  m_EepromSize;           // Size of the complete EEPROM, only used by EPC
                                  // implementation to calculate last sector address.
    UInt  m_EepromVpdSize;        // Size of VPD data section within the EEPROM

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

    // Section layout
    UInt  m_VpdEnd;               // End of non-internal VPD

    DtFastMutex  m_EepromMutex;
    DtFastMutex  m_IoctlMutex;
} DtVpd;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define  VPD_EEPROM_IO_NOT_SUPP -1
#define  VPD_EEPROM_IO_PLX   0
#define  VPD_EEPROM_IO_FPGA  1
#define  VPD_EEPROM_IO_SPI   2
#define  VPD_EEPROM_IO_EZUSB 3


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtVpdReadId(DtVpd* pVpd, char* pVpdId, UInt* pLength);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Only to be used by Vpd.c
DtStatus  DtVpdFindStartOfRoSection(DtVpd*  pVpdData, UInt8**  pRoStart, Int  VpdRwStart);
DtStatus  DtVpdDeleteResource(UInt KeywordLen, const char* pKeyword, UInt8* pSection,
                                                                         UInt SectLength);
UInt  DtVpdGetSectionLength(UInt8* pSection, UInt Length);
Bool  DtVpdFindResource(UInt KeywordLen, const char* pKeyword, UInt8* pSection,
                                                     UInt SectLength, UInt8** ppResource);
Bool  DtVpdIsValidResource(UInt8* pResource, UInt8* pSection, UInt SectLength);

#endif // __DT_VPD_COMMON_H
