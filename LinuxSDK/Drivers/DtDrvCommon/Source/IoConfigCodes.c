//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* IoConfigCodes.c *#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//
// Driver common - Device and port property get funcions.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2012 DekTec Digital Video B.V.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtDrvCommon.h>

// Generated config store
extern const char*  IoConfigCodeStrings[];
extern const Int  IoConfigCodeStringCount;
extern const IoConfigCodeHashSet  IoConfigCodeHashSets[] ;
extern const Int  IoConfigCodesHashCount;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IoConfigCodesCodeGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  IoConfigCodeGet( const char*  pName, Int  *pConfigCode)
{
    DtStatus  Status = DT_STATUS_OK;
    const IoConfigCodeHashSet*  pHashSets = IoConfigCodeHashSets;
    UInt  HashSetCount = IoConfigCodesHashCount;
    
    UInt  Hash = 0;
    Int  Index;
    Bool  ConfigNameFound = FALSE;
    const IoConfigCodeHashSet*  pHashSet = NULL;
    Int  ConfigCount = 0;
    const IoConfigCode*  pConfig = NULL;

    // Special case empty string
    if (pName[0] == '\0')
    {
        *pConfigCode = -1;
        return Status;
    }

    // Determine the name hash
    Hash = DtDjb2(pName) % HashSetCount;

    // Get correct hash set (if exists)
    if (DT_SUCCESS(Status))
    {
        pHashSet = &pHashSets[Hash];
        ConfigCount = pHashSets[Hash].m_ConfigCodeCount;

        // Check if a hash set was available for this hash value
        if (pHashSet == NULL)
            Status = DT_STATUS_NOT_FOUND;
    }

    // Get correct config entry within hash set
    if (DT_SUCCESS(Status))
    {
        // Search all configs
        for (Index=0; Index<ConfigCount; Index++)
        {
            pConfig = &pHashSet->m_pConfigCodes[Index];

            // Compare name to check if we found the first occurrence
            if (DtAnsiCharArrayIsEqual(pName, pConfig->m_pName))
            {
                *pConfigCode = pConfig->m_Value;
                ConfigNameFound = TRUE;
                break;
            }
        }
        if (!ConfigNameFound)
            Status = DT_STATUS_NOT_FOUND;

    }

    if (!ConfigNameFound)
        DtDbgOut(ERR, PROP, "ConfigName %s is not found", pName);
    else
        DtDbgOut(MAX, PROP, "Found config code %d for %s", *pConfigCode, pName);

    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- IoConfigNameGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  IoConfigNameGet(Int  ConfigCode, char*  pName, Int Size)
{
    const char* pSrcName;
    Int i;

    // Special case
    if (ConfigCode==-1)
    {
        pName[0] = '\0';
        return DT_STATUS_OK;
    }

    // Check ConfigCode
    if (ConfigCode<0 || ConfigCode>=IoConfigCodeStringCount)
        return DT_STATUS_INVALID_PARAMETER;

    // Get soure
    pSrcName = IoConfigCodeStrings[ConfigCode];

    // Copy string;
    for(i=0; i<Size; i++)
    {
        pName[i] = pSrcName[i];
        if (pSrcName[i] == '\0')
            break;
    }

    if (i==Size)
        return DT_STATUS_BUFFER_OVERFLOW;
    else
        return DT_STATUS_OK;
}