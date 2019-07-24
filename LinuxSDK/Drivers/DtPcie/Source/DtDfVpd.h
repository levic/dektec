//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfVpd.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2017 DekTec Digital Video B.V.
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

#ifndef __DT_DF_VPD_H
#define __DT_DF_VPD_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtDfSpiProm.h"

// Name + short-name for the VPD function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_VPD_NAME          "Vpd"
#define DT_DF_VPD_SHORTNAME     "VPD"

// MACRO: to init an driver-function-ID for the VPD-DF
#define DT_DF_VPD_INIT_ID(ID, ROLE, INSTANCE, UUID)                                      \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_VPD_NAME, DT_DF_VPD_SHORTNAME, ROLE, INSTANCE, UUID);        \
}                                                                                        \
while (0)

typedef struct _DtDfVpdProperties
{
    Int  m_RoOffset;    // Offset of start of RO section
    Int  m_RoSize;      // Size of the RO-section
    Int  m_RwOffset;    // Offset of start of RW section
    Int  m_RwSize;      // Size of the RW-section
    Int  m_EepromSize;  // Size of VPD Flash/Eeprom
    Int  m_MaxItemLength;   // Maximum item length
}  DtDfVpdProperties;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfVpd definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Wellknown VPD items
#define  DT_DF_VPD_SERIAL_KEYWORD       "SN"
#define  DT_DF_VPD_HWREV_KEYWORD        "EC"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
struct  _DtDfVpd
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    DtFastMutex  m_VpdMutex;        // Access protection for VPD data

    // VPD caching
    Int  m_CacheSize;               // Size of the cache buffer (equal to EEPROM size)
    UInt8*  m_pCache;               // Physical VPD cache buffer
    Int  m_RoOffset;                // Offset of start of RO-section
    Int  m_RoSize;                  // Size of the RO-section
    Int  m_RwOffset;                // Offset of start of RW section
    Int  m_RwSize;                  // Size of the RW-section

    DtDfSpiProm*  m_pDfSpiProm;     // SPI PROM for access to VPD EEPROM

};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfVpd_Close(DtDf*);
DtDfVpd*  DtDfVpd_Open(DtCore*, DtPt*  pPt, const char*  pRole, Int  Instance, 
                                                             Int  Uuid, Bool  CreateStub);
DtStatus  DtDfVpd_Format(DtDfVpd*, Int  SectionFlags, Int Flags);
DtStatus  DtDfVpd_GetProperties(DtDfVpd*, DtDfVpdProperties* pProperties);
DtStatus  DtDfVpd_ItemDelete(DtDfVpd*, Int  SectionFlags, Int  KeyLen, const char*,
                                                                              Int  Flags);
DtStatus  DtDfVpd_ItemRead(DtDfVpd*, Int  SectionFlags, Int  KeyLen, const char*, 
                                                            Int  Flags, Int  BufSize, 
                                                            const UInt8*, Int*  pNumRead);
DtStatus  DtDfVpd_ItemWrite(DtDfVpd*, Int  SectionFlags, Int  KeyLen, const char*, 
                                               Int  Flags, Int  NumToWrite, const UInt8*);
DtStatus  DtDfVpd_RawRead(DtDfVpd*, UInt  StartOffset, Int  NumToRead, UInt8*, 
                                                                          Int*  pNumRead);
DtStatus  DtDfVpd_RawWrite(DtDfVpd*,  UInt  StartOffset, Int  NumToWrite, const UInt8*);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfVpd definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfVpd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for a VPD Driver-Function
typedef struct _DtIoStubDfVpd
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfVpd;

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfVpd - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubDfVpd_Close(DtIoStub*);
DtIoStubDfVpd*  DtIoStubDfVpd_Open(DtDf*);

#endif  // #ifndef __DT_DF_VPD_H
