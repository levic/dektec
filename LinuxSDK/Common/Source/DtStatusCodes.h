//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtStatusCodes.h *#*#*#*#*#*#*#* (C) 2010-2012 DekTec
//
// DTAPI - OS Support Abstraction Layer - Driver status codes
//
// This file contains the driver status codes which are use by the DTA driver, DTU driver
// and DTAPI.

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2012 DekTec Digital Video B.V.
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

#ifndef __DT_STATUS_CODES_H
#define __DT_STATUS_CODES_H


// Status type
typedef UInt32  DtStatus;

#ifndef DTAPI
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Status code macros -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define DT_SUCCESS(Status)            (Status == DT_STATUS_OK)
#define DT_RETURN_ON_ERROR(Expr)      do {                          \
                                           DtStatus  Status = Expr; \
                                           if (!DT_SUCCESS(Status)) \
                                               return Status;       \
                                      } while (0)
#else //DTAPI
#ifndef DTAPI_SERVICE
// In the DTAPI STATUS_SEVERITY_ERROR is not defined. Define STATUS_SEVERITY_ERROR here to
// be able to convert driver errors to DTAPI error codes.
#ifndef STATUS_SEVERITY_ERROR
#define STATUS_SEVERITY_ERROR        3
#elif STATUS_SEVERITY_ERROR != 3
#error STATUS_SEVERITY_ERROR != 3
#endif // STATUS_SEVERITY_ERROR
#endif // DTAPI_SERVICE
#endif // DTAPI

// OS specific status OK and error offset
#ifdef WINBUILD

// Check if define in DTAPI was correct...
#if STATUS_SEVERITY_ERROR != 3
#error STATUS_SEVERITY_ERROR != 3
#endif  // STATUS_SEVERITY_ERROR

#define DT_STATUS_OK                 ((DtStatus)STATUS_SUCCESS)
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +---+-+-+-----------------------+-------------------------------+
//  |Sev|C|R|     Facility          |               Code            |
//  +---+-+-+-----------------------+-------------------------------+
#define DT_STATUS_ERROR(i)           ((DtStatus)((STATUS_SEVERITY_ERROR<<30) \
                                                     | 0x20000000 | 0 | (i & 0x0000FFFF)))
#else
#define DT_STATUS_OK                 ((DtStatus)0x00000000)
#define DT_STATUS_ERROR(i)           ((DtStatus)(0x10000 + (i & 0x0000FFFF)))
#endif

// Failure status codes
#define DT_STATUS_FAIL                 DT_STATUS_ERROR(1)
#define DT_STATUS_OUT_OF_MEMORY        DT_STATUS_ERROR(2)
#define DT_STATUS_BUFFER_OVERFLOW      DT_STATUS_ERROR(3)
#define DT_STATUS_INVALID_PARAMETER    DT_STATUS_ERROR(4)
#define DT_STATUS_NOT_SUPPORTED        DT_STATUS_ERROR(5)
#define DT_STATUS_NOT_FOUND            DT_STATUS_ERROR(6)
#define DT_STATUS_TIMEOUT              DT_STATUS_ERROR(7)
#define DT_STATUS_NOT_INITIALISED      DT_STATUS_ERROR(8)
#define DT_STATUS_CONFIG_ERROR         DT_STATUS_ERROR(9)
#define DT_STATUS_IN_USE               DT_STATUS_ERROR(10)
#define DT_STATUS_OUT_OF_RESOURCES     DT_STATUS_ERROR(11)
#define DT_STATUS_POWERDOWN            DT_STATUS_ERROR(12)
#define DT_STATUS_CANCELLED            DT_STATUS_ERROR(13)
#define DT_STATUS_REQUEUE              DT_STATUS_ERROR(14)
#define DT_STATUS_BUSY                 DT_STATUS_ERROR(15)
#define DT_STATUS_UNKNOWN_PHY          DT_STATUS_ERROR(16)
#define DT_STATUS_NOT_STARTED          DT_STATUS_ERROR(17)
#define DT_STATUS_MULTICASTLIST_FULL   DT_STATUS_ERROR(18)
#define DT_STATUS_VERSION_MISMATCH     DT_STATUS_ERROR(19)

#endif // __DT_STATUS_CODES_H
