//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtNonVolatileSettings.h *#*#*#*#*#* (C) 2010-2016 DekTec
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

#ifndef __DT_NON_VOLATILE_SETTINGS_H
#define __DT_NON_VOLATILE_SETTINGS_H

#ifndef SKIP_NONVOL

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

DtStatus DtNonVolatileManufSettingsRead(DtDrvObject* pSalDrvObj, Int Type, Char* pName, 
                                                                        Int64* pBinValue);
DtStatus DtNonVolatileSettingsDelete(DtDrvObject* pSalDrvObj, UInt64 DvcSerial,
                                                                           UInt NumPorts);
DtStatus DtNonVolatileSettingsStringRead(DtDrvObject* pSalDrvObj, UInt64 DvcSerial, 
                         Int Port, Char* pCategory, Char* pName, Char* pValue, UInt Size);
DtStatus DtNonVolatileSettingsStringWrite(DtDrvObject* pSalDrvObj, UInt64 DvcSerial, 
                                    Int Port, Char* pCategory, Char* pName, Char* pValue);
DtStatus DtNonVolatileSettingsValueRead(DtDrvObject* pSalDrvObj, UInt64 DvcSerial, 
                                   Int Port, Char* pCategory, Char* pName, Int64* pValue);
DtStatus DtNonVolatileSettingsValueWrite(DtDrvObject* pSalDrvObj, UInt64 DvcSerial, 
                                     Int Port, Char* pCategory, Char* pName, Int64 Value);
#endif // SKIP_NONVOL
#endif // __DT_NON_VOLATILE_SETTINGS_H
