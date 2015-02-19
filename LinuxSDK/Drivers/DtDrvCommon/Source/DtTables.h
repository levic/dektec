//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtTables.h *#*#*#*#*#*#*#*#* (C) 2011-2015 DekTec
//
// Common driver - Tables definition and get funcions.
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

#ifndef __DT_TABLES_H
#define __DT_TABLES_H

// Type to store a link to a table
typedef struct _DtTableLink
{
    char*  m_pName;
    Int  m_PortIndex;
    Int  m_MinFw;
    Int  m_MinHw;
    UInt  m_TableEntryCount;
    const DtTableEntry*  m_pTableEntries;
} DtTableLink;

// Type to store all properties for a single device
typedef struct _DtTableStore {
    Int  m_TypeNumber;
    Int  m_SubDvc;
    UInt  m_TableLinkCount;
    const DtTableLink*  m_pTableLink;
} DtTableStore;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtTablesInit(DtPropertyData* pPropData);
DtStatus  DtTableGet(DtPropertyData* pPropData,
                     const char*  pTableName, Int  PortIndex, UInt  MaxNumEntries,
                     UInt* pNumEntries, DtTableEntry* pTableEntry2, UInt OutBufSize);
DtStatus  DtTableGetForType(const char*  pTypeName, Int  TypeNumber, Int  SubDvc, 
                          Int  HwRev, Int  FwVer, const char*  pTableName, Int  PortIndex, 
                          UInt  MaxNumEntries, UInt* pNumEntries, 
                                             DtTableEntry* pTableEntry2, UInt OutBufSize);
#endif // __DT_TABLES_H
