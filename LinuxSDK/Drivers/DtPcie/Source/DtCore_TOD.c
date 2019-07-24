// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCore_TOD.c *#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtCoreIncludes.h"            // Core driver includes

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_TOD implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtCfTod core-function
#define CORE_CFTOD_DEFAULT_PRECONDITIONS(pCore)                                          \
    CORE_DEFAULT_PRECONDITIONS(pCore);                                                   \
    DT_ASSERT(CF_TOD(pCore) != NULL)

// Helper macro to get the TOD-CF from a DtCore object
#define CF_TOD(pCore)         (pCore->m_pCfTod)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore_TOD - Public fuctions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_TOD_GetPeriodicItv -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtCore_TOD_GetPeriodicItv(const DtCore* pCore, Int*  pIntervalMs)
{
    // Sanity checks
    CORE_CFTOD_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pIntervalMs != NULL);
    if (CF_TOD(pCore) == NULL)
        return DT_STATUS_FAIL;

    return DtCfTod_GetPeriodicItv(CF_TOD(pCore), pIntervalMs);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_TOD_GetTime -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCore_TOD_GetTime(const DtCore*  pCore, DtTodTime*  pTime)
{
    UInt  AdjustCount=0;
    // Sanity checks
    CORE_CFTOD_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pTime != NULL);
    if (CF_TOD(pCore) == NULL)
        return DT_STATUS_FAIL;
    return DtCfTod_GetTime(CF_TOD(pCore), pTime, &AdjustCount);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_TOD_PeriodicItvRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_TOD_PeriodicItvRegister(
                             DtCore*  pCore, const DtOnPeriodicIntervalRegData*  pRegData)
{
    // Sanity checks
    CORE_CFTOD_DEFAULT_PRECONDITIONS(pCore);
    if (CF_TOD(pCore) == NULL)
        return DT_STATUS_FAIL;
    return DtCfTod_PeriodicIntervalRegister(CF_TOD(pCore), pRegData);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_TOD_PeriodicItvUnregister -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCore_TOD_PeriodicItvUnregister(DtCore*  pCore, const DtObject*  pObject)
{
    // Sanity checks
    CORE_CFTOD_DEFAULT_PRECONDITIONS(pCore);
    if (CF_TOD(pCore) == NULL)
        return DT_STATUS_FAIL;
    return DtCfTod_PeriodicIntervalUnregister(CF_TOD(pCore), pObject);
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_TOD_Add -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns Time1 + Offset
//
DtTodTime DtCore_TOD_Add(DtTodTime Time, Int64 Offset)
{
    UInt64 T;
    DT_ASSERT(Time.m_Nanoseconds<1000*1000*1000 && Time.m_Nanoseconds<1000*1000*1000);
    T = Time.m_Seconds*1000LL*1000*1000 + Time.m_Nanoseconds;
    T = T + (UInt64)Offset;
    Time.m_Seconds = (UInt32)(T/(1000LL*1000*1000));
    Time.m_Nanoseconds = (UInt32)(T%(1000LL*1000*1000));
    return Time;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_TOD_TimeDiff -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Returns Time1 - Time2 in nano seconds
//
Int64 DtCore_TOD_TimeDiff(DtTodTime Time1, DtTodTime Time2)
{
    UInt64 T1, T2;
    DT_ASSERT(Time1.m_Nanoseconds<1000*1000*1000 && Time2.m_Nanoseconds<1000*1000*1000);
    T1 = Time1.m_Seconds*1000LL*1000*1000 + Time1.m_Nanoseconds;
    T2 = Time2.m_Seconds*1000LL*1000*1000 + Time2.m_Nanoseconds;
    return (Int64)(T1 - T2);
}