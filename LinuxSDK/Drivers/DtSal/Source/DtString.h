//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtString.h *#*#*#*#*#*#*#*#* (C) 2010-2016 DekTec
//
// This file contains functions to create / manipulate DtString objects.
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

#ifndef __DT_STRING_H
#define __DT_STRING_H

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Type definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// DtString examples:
//
// Constant DtString:
// DT_STRING_DECL(TestString, "Test string");
//
// Stack allocated DtString:
// DtStringChar TestStringBuffer[12];
// DT_STRING_INIT(TestString, TestStringBuffer, 12);
// DtStringAppendChars(&TestString, "Test string");
//
// Other heap/stack allocated constant DtString (must be '\0' terminated):
// e.g. DtStringChar OtherStringBuffer[12]= {"Test string"};
// DT_STRING_INIT_CONST(TestString, OtherStringBuffer, 11);

//
// Heap allocated DtString:
// DtStringAlloc(&TestString, 12);
// DtStringAppendChars(&TestString, "Test string");
// DtStringFree(&TestString);

#ifdef WINBUILD
    typedef WCHAR DtStringChar;
    typedef UNICODE_STRING DtString;
    #define DT_STRING_DECL(Name, Value)              \
        DtString Name =	                             \
        {                                            \
            sizeof(L##Value) - sizeof(DtStringChar), \
            sizeof(L##Value),                        \
            L##Value                                 \
        }
    #define DT_STRING_INIT(Name, CharBuffer, BufLen) do {       \
            Name.Length = 0;                                    \
            Name.MaximumLength = BufLen * sizeof(DtStringChar); \
            Name.Buffer = CharBuffer;                           \
        } while(0)
    #define DT_STRING_INIT_CONST(Name, CharBuffer, StringLen) do {     \
            Name.Length = StringLen * sizeof(DtStringChar);            \
            Name.MaximumLength = (StringLen+1) * sizeof(DtStringChar); \
            Name.Buffer = CharBuffer;                                  \
        } while(0)
#else
    typedef char DtStringChar;
    typedef struct _DtString
    {
        UInt16 m_Length;        // Current length of string in m_Buffer in BYTES WITHOUT 0 termininator
        UInt16 m_MaximumLength; // Maximum length of m_Buffer in BYTES
        DtStringChar* m_Buffer;
    } DtString;
    #define DT_STRING_DECL(Name, Value)           \
        DtString Name =                           \
        {                                         \
            sizeof(Value) - sizeof(DtStringChar), \
            sizeof(Value),                        \
            Value,                                \
        }
    #define DT_STRING_INIT(Name, CharBuffer, BufLen) do {           \
            Name.m_Length = 0;                                      \
            Name.m_MaximumLength = BufLen * sizeof(DtStringChar);   \
            Name.m_Buffer = CharBuffer;                             \
        } while(0)
    #define DT_STRING_INIT_CONST(Name, CharBuffer, StringLen) do {         \
            Name.m_Length = StringLen * sizeof(DtStringChar);              \
            Name.m_MaximumLength = (StringLen+1) * sizeof(DtStringChar);   \
            Name.m_Buffer = CharBuffer;                                    \
        } while(0)
#endif


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

DtStatus  DtStringAlloc(DtString* pDtString, UInt16 DtStringCharCount);
DtStatus  DtStringFree(DtString* pDtString);
DtStatus  DtStringClear(DtString* pDtString);
DtStatus  DtStringAppendChars(DtString* pDtStringDest, const Char* pStringSrc);
DtStatus  DtStringAppendDtString(DtString* pDtStringDest, DtString* pDtStringSrc);
DtStatus  DtStringAppendSubstring(DtString* pDtStringDest, DtString* pDtStringSrc,
                                                  Int SubStringCount, char DelimiterChar);
Bool  DtStringCompare(DtString* pDtString1, DtString* pDtString2);
UInt  DtStringGetMaxStringLength(DtString* pDtString);
UInt  DtStringGetStringLength(DtString* pDtString);
DtStatus  DtStringToCharString(DtString* pDtString, Char* pStringDest, UInt Size);
DtStatus  DtStringUIntegerToDtString(DtString* pDtString, UInt8 Base, UInt Value);
DtStatus  DtStringUIntegerToDtStringAppend(DtString* pDtString, UInt8 Base, UInt Value);
DtStatus  DtStringUInt64ToDtString(DtString* pDtString, UInt8 Base, UInt64 Value);
DtStatus  DtStringUInt64ToDtStringAppend(DtString* pDtString, UInt8 Base, UInt64 Value);

#endif // __DT_STRING_H
