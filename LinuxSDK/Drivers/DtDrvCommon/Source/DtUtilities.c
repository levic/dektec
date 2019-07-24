//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtUtilities.c *#*#*#*#*#*#*#*# (C) 2011-2016 DekTec
//
// Driver common - General driver utilities.
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


//.-.-.-.-.-.-.-.-.-.-.-.-.- DtUtilitiesDeduceHardwareRevision -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Deduce the hardware revision from the VPD "EC" resource
//
DtStatus  DtUtilitiesDeduceHardwareRevision(char* Buffer, UInt BufLen, 
                                                                   Int* pHardwareRevision)
{
    Int i;
    
    // Check all input parameters
    if (Buffer == NULL)
        return DT_STATUS_FAIL;
    if (BufLen == 0)
        return DT_STATUS_FAIL;
    if (pHardwareRevision == NULL)
        return DT_STATUS_FAIL;

    // Expected format of "EC" resource is a string "Rev X" or "Rev X.Y" => 
    // Minimum length should be 5 and the string should begin with "Rev "
    if (BufLen>=5 && DtAnsiCharArrayStartsWith(Buffer, "Rev "))
    {
        // Get major hardware revision
        *pHardwareRevision = (Int)(DtAnsiCharArrayToUInt(Buffer+4, BufLen-4, 10));
        *pHardwareRevision *= 100;
        if (*pHardwareRevision < 0)
            *pHardwareRevision = 0;
                
        // Add minor hardware revision 
        // Search for . in "EC" field and start from there
        for (i=4; i<(Int)(BufLen); i++)
        {
            if (Buffer[i]=='.' || Buffer[i]==',')
            {
                *pHardwareRevision += 
                                  (Int) DtAnsiCharArrayToUInt(Buffer+i+1, BufLen-i-1, 10);
                break;
            }
        }
                
        return DT_STATUS_OK;
    }

    return DT_STATUS_FAIL;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtUtilitiesDeriveSubType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtUtilitiesDeriveSubType(DtPropertyData* pPropData, Int HardwareRevision, 
                                                                            Int* pSubType)
{
    Int i;
    char  SubTypePropName[64];
    DtStatus  Status;
    DtPropertyScope  SubTypePropScope;
    DtPropertyValue  SubTypePropValue;
    DtPropertyValueType  SubTypePropType;

    // Check all input parameters
    if (pPropData == NULL)
        return DT_STATUS_FAIL;
    if (pSubType == NULL)
        return DT_STATUS_FAIL;

    // Derive subtype from SUBTYPEx properties, but this can only be done if
    // we know the hardware revision
    if (HardwareRevision < 0)
        return DT_STATUS_FAIL;

    // The hardware revision of the device is known, so subtype can never be un
    // undefined. Therefor we can set subtype to 0=none now
    *pSubType = 0;

    // Subtype is encoded in properties "SUBTYPE1", "SUBTYPE2", etc.
    // Each SUBTYPE property contains a hardware revision in the upper 16 bits
    // and a subtype (which applies if the device's hardware revision is greater
    // than the encoded hardware revision) in the lower 16 bits.
    // This code walks through the SUBTYPE properties until the device's hardware
    // revision exceeds the hardware revision encoded in the SUBTYPE property.
    DtMemCopy(SubTypePropName, "SUBTYPE1", 9);
    for (i=1; i<10; i++)
    {
        Int  HwRev, SubType;

        // Get the value of subtype property #i: SUBTYPE1, SUBTYPE2, ...
        SubTypePropName[7] = '0' + i;
        Status = DtPropertiesGet(pPropData, SubTypePropName, -1, &SubTypePropValue, 
                                         &SubTypePropType, &SubTypePropScope, -1, -1, -1);
        if (!DT_SUCCESS(Status))
            break;
                
        // Split SUBTYPE property value into hardware revision and subtype
        HwRev = (SubTypePropValue >> 16) & 0xFFFF;
        SubType = SubTypePropValue & 0xFFFF;

        // Compare the hardware revision of the current subtype property with the
        // board's hardware revision and copy the subtype if appropriate
        if (HardwareRevision >= HwRev)
            *pSubType = SubType;
        else
            // We can stop here since the SUBTYPE properties are ordened from
            // minimal hardware version to maximal hardware revision
            break;
    }

    return DT_STATUS_OK;
}



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtUtilitiesCamelCaseToUpper -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Convert a camel-case-string to an upper case string with underscores
// For example: "HelloWorld" => "HELLO_WORLD"
//
DtStatus  DtUtilitiesCamelCaseToUpper(const char*  pStrIn, char*  pStrOut, Int  MaxLength)
{
    Int  i=0, OutSize=0;
    Bool  PrevIsLowerCaseOrNum = FALSE;
    DT_ASSERT(pStrIn!=NULL && pStrOut!=NULL && MaxLength>0);

#define _DT_IS_NUMBER(x)  (x>='0' && x <='9')
#define _DT_IS_LOWER(x)  (x>='a' && x <='z')
#define _DT_IS_UPPER(x)  (x>='A' && x <='Z')
#define _DT_TO_UPPER(x)  (_DT_IS_LOWER(x)) ? ((x-'a')+'A') : x

    pStrOut[0] = '\0';
    for(i=0; pStrIn[i]!='\0' && OutSize<(MaxLength-1); i++)
    {
        // Insert underscore before every upper-case character preceeded by a lower-case
        // or number
        if (i!=0 && _DT_IS_UPPER(pStrIn[i]) && PrevIsLowerCaseOrNum)
            pStrOut[OutSize++] = '_';
        // Check for overflow of our output buffer
        if (OutSize==(MaxLength-1))
            return DT_STATUS_BUF_TOO_SMALL;
        // Convert the char to a upper case character
        pStrOut[OutSize++] = _DT_TO_UPPER(pStrIn[i]);

        // Update previous character was a lower-case or number
        PrevIsLowerCaseOrNum = _DT_IS_LOWER(pStrIn[i]) || _DT_IS_NUMBER(pStrIn[i]);
    }
    pStrOut[OutSize] = '\0';

#undef  _DT_IS_NUMBER
#undef  _DT_IS_LOWER
#undef  _DT_IS_UPPER
#undef  _DT_TO_UPPER

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtUtilitiesSplitInstanceId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Splits and instance ID into it's name and instance-number parts
// For example:  'DF_VPD#2' is split into name='VPD' and instance='2'
//
DtStatus  DtUtilitiesSplitInstanceId(
    const char*  pInstanceId, 
    const char*  pPrefix,
    char*  pName,
    Int  MaxLengthName, 
    Int*  pInstanceNum)
{
    Int i=0, Len=0;

    // Check for valid parameters
    DT_ASSERT(pInstanceId!=NULL && pName!=NULL && MaxLengthName>0 && pInstanceNum!=NULL);
    if (pInstanceId==NULL || pName==NULL || MaxLengthName<=0 || pInstanceNum==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Instance ID should start with 'BC_' or 'DF_'
    if (!DtAnsiCharArrayStartsWith(pInstanceId, pPrefix))
        return DT_STATUS_INVALID_PARAMETER;

    Len = DtAnsiCharArrayStrLength(pPrefix, 4);
    DT_ASSERT(Len >= 0);

    // Extract the name (i.e. part after prefix upto '#')
    pName[0] = '\0';
    pInstanceId += Len;   // Skip prefix
    for (i=0; i<MaxLengthName && *pInstanceId!='\0'; i++, pInstanceId++)
    {
        if (*pInstanceId=='#')
            break;

        pName[i] = *pInstanceId;
    }
    if (i >= MaxLengthName)
        return DT_STATUS_BUF_TOO_SMALL;
    pName[i] = '\0';

    if (pInstanceId[0] == '#')
    {
        pInstanceId++;
        // Compute length of instance# part (cannot be longer than 3-chars)
        for (Len=0; Len<3; Len++)
        {
            // Do we have a number
            if (pInstanceId[Len]<'0' || pInstanceId[Len]>'9')
                break;
        }
        *pInstanceNum = (Int)DtAnsiCharArrayToUInt(pInstanceId, Len, 10);
    }
    else
        *pInstanceNum = -1;

    return DT_STATUS_OK;
}