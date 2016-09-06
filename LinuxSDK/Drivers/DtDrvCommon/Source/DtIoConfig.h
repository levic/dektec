//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoConfig.h *#*#*#*#*#*#*#*# (C) 2010-2016 DekTec
//
// Driver common - IO configuration - Declaration of IO configuration types/functions
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

#ifndef __DT_IO_CONFIG_H
#define __DT_IO_CONFIG_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// DtIoConfigValue
typedef struct _DtIoConfigValueDriver {
    Int  m_Value;
    Int  m_SubValue;
    Int64  m_ParXtra[DT_MAX_PARXTRA_COUNT];
} DtIoConfigValueDriver;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#ifdef WINBUILD
DtStatus  DtIoConfigReadFromNonVolatileStorage(DtDrvObject*  pSalDrvObj, UInt64  Serial,
                                                       Int  PortIndex, Int  IoGroup,
                                                       DtIoConfigValueDriver*  pCfgValue);
#else
DtStatus  DtIoConfigReadFromIniFile(const char*  DriverName, UInt64  Serial,
                                                       Int  TypeNumber, Int  PortNumber,
                                                       DtIoConfigValueDriver* pCfgValues);
#endif
//DtStatus  DtaIoConfigGet(DtaDeviceData*  pDvcData, DtFileObject*  pFile, 
//                         DtIoConfigId* pIoCfgId, DtIoConfigValue* pIoCfgValue, Int Count);
//DtStatus  DtaIoConfigSet(DtaDeviceData* pDvcData, DtFileObject* pFile,
//                                                   DtIoConfigPars* pIoConfigs, Int Count);

#endif // __DT_IO_CONFIG_H
