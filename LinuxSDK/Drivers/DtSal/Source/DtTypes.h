//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtTypes.h *#*#*#*#*#*#*#*#*# (C) 2010-2015 DekTec
//
// Basic types used in KMDF / Linux 2.6 device drivers.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2015 DekTec Digital Video B.V.
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

#ifndef __DT_TYPES_H
#define __DT_TYPES_H

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ OS Specific type definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

#ifdef WINBUILD
    // Windows types

    typedef char Char;

    typedef signed int  Int;
    typedef __int8  Int8;
    typedef __int16 Int16;
    typedef __int32 Int32;
    typedef __int64 Int64;
    typedef Int64 Int64A;

    typedef unsigned int  UInt;
    typedef unsigned __int8  UInt8;
    typedef unsigned __int16 UInt16;
    typedef unsigned __int32 UInt32;
    typedef unsigned __int64 UInt64;
    typedef UInt64  UInt64A;

    typedef BOOLEAN Bool;

#else
    // Linux types

    typedef char Char;

    typedef signed int Int;
    typedef s8  Int8;
    typedef s16 Int16;
    typedef s32 Int32;
    typedef s64 Int64;
    typedef Int64 Int64A __attribute__ ((aligned (8)));
    
    typedef unsigned int UInt;
    typedef u8  UInt8;
    typedef u16 UInt16;
    typedef u32 UInt32;
    typedef u64 UInt64;
    typedef UInt64 UInt64A __attribute__ ((aligned (8)));

    typedef UInt8 Bool;
    #ifndef FALSE
    #define  FALSE  0
    #endif
    #ifndef TRUE
    #define  TRUE  1
    #endif

    #define  NULL  ((void *)0)

#endif

#endif // __DT_TYPES_H
