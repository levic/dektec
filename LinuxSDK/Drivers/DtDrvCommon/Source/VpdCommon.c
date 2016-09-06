//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* VpdCommon.c *#*#*#*#*#*#*#*#* (C) 2011-2016 DekTec
//
// Driver common - Vital Product Data (VPD) read/write routines.
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


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtDrvCommon.h>

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Fixed Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// VPD ID start indicator
#define  VPD_IDSTRING_TAG           0x82

#define  VPD_R_TAG                  0x90
#define  VPD_W_TAG                  0x91
#define  VPD_END_TAG                0x78
#define  VPD_INTSTART_TAG           0x76

// Macro to check for valid keyword chars
#define  VALID_VPD_KEYW_CHAR(c)     (((c)>='0' && (c)<='9') || ((c)>='A' && (c)<='Z') || \
                                                                                (c)=='_')

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVpdReadId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read the VPD Id into a caller-allocated string buffer.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
DtStatus  DtVpdReadId(
    DtVpd* pVpdData,            // VPD data
    char*  pVpdId,              // String buffer for storing VPD Id
    UInt*  pLength)             // In: Length of pVpdId buffer
                                // Out: Length of string
{
    char*  pFrom;               // Helper character pointer
    char*  pTo;                 // Helper character pointer
    UInt  i, Length;
    UInt8*  pVpd;               // Byte pointer in VPD data
    
    // Add parameter checks here
    if (*pLength == 0)
        DtDbgOut(ERR, VPD, "Invalid length: 0");
    
    // Initialisation
    pTo = pVpdId;               // For later copying to VPD item
    DtMemZero(pVpdId, *pLength);

    // Start at beginning of VPD-ID + read-only section
    pVpd = &pVpdData->m_pCache[pVpdData->m_IdOffset];
    if (pVpd[0] != VPD_IDSTRING_TAG)
    {
        DtDbgOut(ERR, VPD, "ERROR: VPD-ID section starts with 0x%02x i.s.o. %02x", pVpd[0],
                                                                        VPD_IDSTRING_TAG);
        return DT_STATUS_FAIL;
    }

    Length = pVpd[1] + (pVpd[2]<<8);
    pFrom = (char*)(pVpd + 3);

    // Protect against invalid content of EEPROM or too small buffer
    if (Length > *pLength)
        Length = *pLength;
    for (i=0; i<Length; i++)
        *pTo++ = *pFrom++;
    *pLength = Length;

    DtDbgOut(MAX, VPD, "Found VPD Id, value='%.*s'", Length, pVpdId);

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaVpdFindStartOfRoSection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtVpdFindStartOfRoSection(
    DtVpd*  pVpdData,
    UInt8**  pRoStart,
    Int  VpdRwStart)
{
    UInt8*  pVpd;
    UInt8*  pVpdRoEnd;
    
    // Compute a number of helper variables
    pVpdRoEnd = &pVpdData->m_pCache[VpdRwStart];

    // Start at beginning of VPD-ID + read-only section
    pVpd = &pVpdData->m_pCache[pVpdData->m_IdOffset];

    if (pVpd[0] != VPD_IDSTRING_TAG) {
        DtDbgOut(ERR, VPD, "ERROR: VPD-ID section starts with 0x%02x i.s.o. 0x%02x", 
                                                               pVpd[0], VPD_IDSTRING_TAG);
        return DT_STATUS_FAIL;
    }

    // Move to start of VPD read-only section
    pVpd += 3 + pVpd[1] + (pVpd[2]<<8);

    // Consistency checks
    if (pVpd >= pVpdRoEnd) {
        DtDbgOut(MAX, VPD, "ERROR: VPD-RO section starts beyond max. end address");
        return DT_STATUS_FAIL;
    }
    if (pVpd[0] != VPD_R_TAG) {
        DtDbgOut(MAX, VPD, "ERROR: VPD-RO section starts with 0x%02x i.s.o. 0x%02x", 
                                                                      pVpd[0], VPD_R_TAG);
        return DT_STATUS_FAIL;
    }

    *pRoStart = pVpd;
    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVpdDeleteResource -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Keyword-based VPD-resource delete in given VPD section (RO or RW).
// All VPD-resources with the pKeyword found are deleted.
//
DtStatus  DtVpdDeleteResource(
    UInt KeywordLen,            // Length of Keyword
    const char*  pKeyword,      // Keyword to be found
    UInt8*  pSection,           // VPD section (starts with VPD tag)
    UInt  SectLength)           // Real section length
{
    UInt8*  pRes;               // Running resource pointer
        
    // Loop through resources
    if (!DtVpdFindResource(KeywordLen, pKeyword, pSection, SectLength, &pRes))
        return DT_STATUS_NOT_FOUND;

    // Delete resource in a loop, so that multiple instances are deleted too.
    do {
        UInt8*  pTo = pRes;
        UInt8*  pNext = pRes + 3 + pRes[2]; // Next resource
        UInt  i;
        while (DtVpdIsValidResource(pNext, pSection, SectLength))
        {
            UInt  NextResourceLength = 3 + pNext[2];
            for (i=0; i<NextResourceLength; i++)
                *pTo++ = *pNext++;
        }
        // Fill rest of section with zero's
        while (pTo < pSection+SectLength)
            *pTo++ = 0;
    } while (DtVpdFindResource(KeywordLen, pKeyword, pSection, SectLength, &pRes));

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVpdGetSectionLength -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt  DtVpdGetSectionLength(
    UInt8*  pSection,           // Points to VPD-tag that starts the section
    UInt  Length)               // Total length of section, including stuffing
{
    UInt  LengthSoFar = 3;
    UInt8*  pResource = pSection + 3; // Skip VPD-tag and 2-byte length

    while (DtVpdIsValidResource(pResource, pSection, Length))
    {
        UInt  ResLength = 3 + pResource[2]; // 3 for keyword + 1-byte length
        pResource += ResLength;
        LengthSoFar += ResLength;
    }
    return LengthSoFar;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVpdFindResource -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Keyword-based VPD-resource search in given VPD section (RO or RW)
//
Bool  DtVpdFindResource(
    UInt KeywordLen,            // Length of Keyword
    const char*  pKeyword,      // Keyword to be found
    UInt8*  pSection,           // VPD section (starts with VPD tag)
    UInt  SectLength,           // Real section length
    UInt8**  ppResource)        // Points to VPD resource if keyword is found
{
    UInt8*  pRes;               // Running resource pointer
    UInt  ResLength;            // Resource length
    
    pRes = pSection + 3;

    // Loop through resources
    while (DtVpdIsValidResource(pRes, pSection, SectLength))
    {
        if (strncmp((char*)pRes, (char*)pKeyword, KeywordLen) == 0)
        {
            *ppResource = pRes;
            return TRUE;
        }
        ResLength = pRes[2] + 3;
        pRes += ResLength;
    }

    // No more valid keywords => can't find keyword
    return FALSE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtVpdIsValidResource -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Check whether pResource points to a correctly formatted VPD resource.
//
Bool  DtVpdIsValidResource(
    UInt8*  pResource,       // Resource to be checked for validity
    UInt8*  pSection,        // Section in which keyword is located
    UInt  SectLength)        // Real section length (should correspond to pSection[1..2]+3
{
    if (pResource > pSection+SectLength-3)
        return FALSE;
    if (!VALID_VPD_KEYW_CHAR(pResource[0]) || !VALID_VPD_KEYW_CHAR(pResource[1]))
        return FALSE;
    return (pResource+3+(UInt)(pResource[2]) <= pSection+SectLength);
}
