//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtUtilities.h *#*#*#*#*#*#*#*# (C) 2011-2016 DekTec
//
// Common driver - General driver utilities.
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


#ifndef __DT_UTILITIES_H
#define __DT_UTILITIES_H

// MACRO to determine size of and array (i.e. #elements in an array)
#ifndef DT_SIZEOF_ARRAY
#define  DT_SIZEOF_ARRAY(ARRAY)   (sizeof(ARRAY)/sizeof(ARRAY[0]))
#endif // #ifndef DT_SIZEOF_ARRAY


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtUtilitiesDeduceHardwareRevision(char* Buffer, UInt BufLen, 
                                                                  Int* pHardwareRevision);
DtStatus  DtUtilitiesDeriveSubType(DtPropertyData* pPropData, Int HardwareRevision, 
                                                                           Int* pSubType);

DtStatus  DtUtilitiesCamelCaseToUpper(const char* pStrIn, char*  pStrOut, Int  MaxLength);

DtStatus  DtUtilitiesSplitInstanceId(const char*  pInstanceId, const  char*  pPrefix, 
                                                         char*  pName, Int  MaxLengthName,
                                                         Int*  pInstanceNum);

#endif // __DT_UTILITIES_H
