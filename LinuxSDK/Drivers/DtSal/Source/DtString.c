//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtString.c *#*#*#*#*#*#*#*#* (C) 2010-2016 DekTec
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtSal.h>
#ifdef WINBUILD
#include <Ntstrsafe.h>
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtStringAlloc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtStringAlloc(DtString* pDtString, UInt16 DtStringCharCount)
{
    if (pDtString == NULL)
        return DT_STATUS_INVALID_PARAMETER;

#ifdef WINBUILD

    // Initialially empty string
    pDtString->Length = 0;

    // Allocate buffer (1 char extra for the 0 terminator)
    pDtString->Buffer = ExAllocatePoolWithTag(NonPagedPool,
                                                   DtStringCharCount*sizeof(DtStringChar),
                                                   SAL_TAG);
    
    // Check if buffer allocated and set maximumlength
    if (pDtString->Buffer == NULL)
    {
        pDtString->MaximumLength = 0;
        return DT_STATUS_OUT_OF_MEMORY;
    }
    
    // Set maximum length
    pDtString->MaximumLength = DtStringCharCount * sizeof(DtStringChar);
    
    // Always terminate with zero
    pDtString->Buffer[0] = L'\0';
    
#else

    // Initialially empty string
    pDtString->m_Length = 0;

    // Allocate buffer (1 char extra for the 0 terminator)
    pDtString->m_Buffer = kmalloc((DtStringCharCount+1)*sizeof(DtStringChar), GFP_KERNEL);

    // Check if buffer allocated and set maximumlength
    if (pDtString->m_Buffer == NULL)
    {
        pDtString->m_MaximumLength = 0;
        return DT_STATUS_OUT_OF_MEMORY;
    }
    
    // Set maximum length
    pDtString->m_MaximumLength = DtStringCharCount * sizeof(DtStringChar);
    
    // Always terminate with zero
    pDtString->m_Buffer[0] = '\0';

#endif

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtStringFree -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtStringFree(DtString* pDtString)
{
    if (pDtString == NULL)
        return DT_STATUS_INVALID_PARAMETER;

#ifdef WINBUILD

    // Free the buffer
    if (pDtString->Buffer != NULL)
    {
        ExFreePool(pDtString->Buffer);
        pDtString->Buffer = NULL;
    }

    // Set length/maximemlength to 0
    pDtString->Length = 0;
    pDtString->MaximumLength = 0;

#else

    // Free the buffer
    if (pDtString->m_Buffer != NULL)
    {
        kfree(pDtString->m_Buffer);
        pDtString->m_Buffer = NULL;
    }

    // Set length/maximemlength to 0
    pDtString->m_Length = 0;
    pDtString->m_MaximumLength = 0;

#endif

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtStringClear -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtStringClear(DtString* pDtString)
{
    // Check input param
    if (pDtString == NULL)
        return DT_STATUS_INVALID_PARAMETER;

#ifdef WINBUILD

    pDtString->Length = 0;
    if (pDtString->MaximumLength > 0)
        pDtString->Buffer[0] = L'\0';

#else

    pDtString->m_Length = 0;
    if (pDtString->m_MaximumLength > 0)
        pDtString->m_Buffer[0] = '\0';

#endif

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtStringAppendChars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtStringAppendChars(DtString* pDtStringDest, const Char* pStringSrc)
{
    DtStatus  Status = DT_STATUS_OK;
#ifdef WINBUILD
    ANSI_STRING  AnsiStringSrc;
    DtString  DtString;
#else
    Int SrcLength;
#endif

    // Check input params
    if (pDtStringDest==NULL || pStringSrc==NULL)
        return DT_STATUS_INVALID_PARAMETER;

#ifdef WINBUILD

    // Create temp string which holds the chars as unicode string
    RtlInitAnsiString(&AnsiStringSrc, pStringSrc);
    if (!NT_SUCCESS(RtlAnsiStringToUnicodeString(&DtString, &AnsiStringSrc, TRUE)))
        return DT_STATUS_BUFFER_OVERFLOW;

    // Append the chars (temp string)
    Status = DtStringAppendDtString(pDtStringDest, &DtString);

    // Free temp string
    RtlFreeUnicodeString(&DtString);
    
    // Zero terminator is added by DtStringAppendDtString...

#else

    // Get length of src string
    SrcLength = strlen(pStringSrc) * sizeof(DtStringChar);
    
    // Char == DtStringChar, no conversion -> just copy
    DT_ASSERT(sizeof(DtStringChar) == sizeof(Char));
    
    // Check for buffer overflow
    if ((pDtStringDest->m_MaximumLength-pDtStringDest->m_Length) < 
                                                           SrcLength+sizeof(DtStringChar))
        return DT_STATUS_BUFFER_OVERFLOW;

    // Append chars
    memcpy(pDtStringDest->m_Buffer + pDtStringDest->m_Length, pStringSrc,
           SrcLength * sizeof(DtStringChar));

    // Set new length
    pDtStringDest->m_Length += SrcLength;
    
    // Always terminate with zero
    pDtStringDest->m_Buffer[pDtStringDest->m_Length/sizeof(DtStringChar)] = '\0';

#endif

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtStringAppendDtString -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Appends pDtStringSrc to pDtStringDest. The pDtStringDest must be large enough to hold
// both strings, else no data is being copied.
//
DtStatus  DtStringAppendDtString(DtString* pDtStringDest, DtString* pDtStringSrc)
{
    DtStatus  Status = DT_STATUS_OK;
    
    // Check input params
    if (pDtStringDest==NULL || pDtStringSrc==NULL)
        return DT_STATUS_INVALID_PARAMETER;

#ifdef WINBUILD

    if (!NT_SUCCESS(RtlAppendUnicodeStringToString(pDtStringDest, pDtStringSrc)))
        return DT_STATUS_BUFFER_OVERFLOW;
    
    // Check space for zero terminator
    if (pDtStringDest->Length >= pDtStringDest->MaximumLength)
        return DT_STATUS_BUFFER_OVERFLOW;
    
    // Add zero terminator
    pDtStringDest->Buffer[pDtStringDest->Length/sizeof(DtStringChar)] = L'\0';

#else

    // Check for buffer overflow
    if (pDtStringDest->m_MaximumLength-pDtStringDest->m_Length <
                                              pDtStringSrc->m_Length+sizeof(DtStringChar))
        return DT_STATUS_BUFFER_OVERFLOW;

    // Append chars
    memcpy(pDtStringDest->m_Buffer + pDtStringDest->m_Length, pDtStringSrc->m_Buffer,
           pDtStringSrc->m_Length * sizeof(DtStringChar));

    // Set new length
    pDtStringDest->m_Length += pDtStringSrc->m_Length;
    
    // Always terminate with zero
    pDtStringDest->m_Buffer[pDtStringDest->m_Length/sizeof(DtStringChar)] = '\0';

#endif

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtStringAppendSubstring -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function adds a substring from pDtStringSrc to pDtStringDest.
// The pDtStringSrc is delimited by the 'DelimiterChar', The SubStringCount is the
// substring (index) to be copied.  
// Example: 
// 1) Input:  pDtStringSrc = "Settings\Test\Port", DelimiterChar='\', SubStringCount=2
//    Output: pDtStringDst+= "Test"
// 2) Input:  pDtStringSrc = "31-12-2008", DelimiterChar='-', SubStringCount=3
//    Output: pDtStringDst+= "2008"
//
DtStatus  DtStringAppendSubstring(
    DtString*  pDtStringDest, 
    DtString*  pDtStringSrc,
    Int  SubStringCount, 
    char  DelimiterChar)
{
    Int  NumFound = 0;
    Int  Index = 0;
    Int  Start = 0;
    Int  SrcLength = DtStringGetStringLength(pDtStringSrc);
    Int  i;
    
    // Check input params
    if (pDtStringDest==NULL || pDtStringSrc==NULL || SubStringCount<1)
        return DT_STATUS_INVALID_PARAMETER;

    // Search the substring
    while (NumFound!=SubStringCount && Index<SrcLength)
    {
#ifdef WINBUILD
        if (pDtStringSrc->Buffer[Index] == DelimiterChar)
#else
        if (pDtStringSrc->m_Buffer[Index] == DelimiterChar)
#endif
        {
            NumFound++;
            if (NumFound != SubStringCount) 
            {
                Index++;
                Start = Index;
            }

        } else 
            Index++;
    }
    if (NumFound==SubStringCount || NumFound==SubStringCount-1)
    {
        // String found. Check destination buffer length
        if ((Index-Start+DtStringGetStringLength(pDtStringDest)+1) > 
                                                DtStringGetMaxStringLength(pDtStringDest))
            return DT_STATUS_BUFFER_OVERFLOW;
        
        // Copy substring to destination buffer
        for (i=Start; i<Index; i++)
        {
#ifdef WINBUILD
            pDtStringDest->Buffer[pDtStringDest->Length/sizeof(DtStringChar)] = 
                                                                  pDtStringSrc->Buffer[i];
            pDtStringDest->Length += sizeof(DtStringChar);
            pDtStringDest->Buffer[pDtStringDest->Length/sizeof(DtStringChar)] = L'\0';
#else
            pDtStringDest->m_Buffer[pDtStringDest->m_Length/sizeof(DtStringChar)] = 
                                                               pDtStringDest->m_Buffer[i];
            pDtStringDest->m_Length += sizeof(DtStringChar);
            pDtStringDest->m_Buffer[pDtStringDest->m_Length/sizeof(DtStringChar)] = '\0';
#endif
        }
        return DT_STATUS_OK;
    }
    return DT_STATUS_NOT_FOUND;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtStringCompare -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtStringCompare(DtString* pDtString1, DtString* pDtString2)
{
#ifdef WINBUILD
    return RtlEqualUnicodeString(pDtString1, pDtString2, TRUE);
#else
    if (pDtString1->m_Length != pDtString2->m_Length)
        return FALSE;
    return strncmp(pDtString1->m_Buffer, pDtString2->m_Buffer, pDtString1->m_Length) == 0;
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtStringGetMaxStringLength -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns the maximal string length. (Not the buffer length)
//
UInt  DtStringGetMaxStringLength(DtString* pDtString)
{
#ifdef WINBUILD
    return pDtString->MaximumLength / sizeof(DtStringChar);
#else
    return pDtString->m_MaximumLength / sizeof(DtStringChar);
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtStringGetStringLength -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns the string length. (Not the occupied buffer length)
//
UInt  DtStringGetStringLength(DtString* pDtString)
{
#ifdef WINBUILD
    return pDtString->Length / sizeof(DtStringChar);
#else
    return pDtString->m_Length / sizeof(DtStringChar);
#endif
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtStringToCharString -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtStringToCharString(DtString* pDtString, Char* pStringDest, UInt Size)
{
    Int i;
    DtStatus  Status = DT_STATUS_OK;
#ifdef WINBUILD
    ANSI_STRING  AnsiStringDest;
#endif
    // Check input params
    if (pDtString == NULL)
        return DT_STATUS_INVALID_PARAMETER;

#ifdef WINBUILD
    
    // Convert to ANSI string
    Status = RtlUnicodeStringToAnsiString(&AnsiStringDest, pDtString, TRUE);
    if (NT_SUCCESS(Status))
    {
        // Check destination size 
        if (AnsiStringDest.Length >= Size)
            Status = DT_STATUS_BUFFER_OVERFLOW;
        
        if (NT_SUCCESS(Status))
        {
            // Copy Chars
            for (i=0; i<AnsiStringDest.Length; i++)
                pStringDest[i] = AnsiStringDest.Buffer[i];
            // Terminate with zero
            pStringDest[AnsiStringDest.Length] = '\0';
        }

        // Free ANSI string
        RtlFreeAnsiString(&AnsiStringDest);
    }

#else

    // Check destination size 
    if (pDtString->m_Length >= Size)
        return DT_STATUS_BUFFER_OVERFLOW;

    // Copy chars
    for (i=0; i<pDtString->m_Length; i++)
        pStringDest[i] = (Char)(pDtString->m_Buffer[i]);

    // Terminate with zero
    pStringDest[pDtString->m_Length] = '\0';

#endif
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtStringUIntegerToDtString -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtStringUIntegerToDtString(DtString* pDtString, UInt8 Base, UInt Value)
{
    DtStatus  Status = DT_STATUS_OK;
#ifdef WINBUILD
#else
    Int Length;
#endif

    // Check input params
    if (pDtString == NULL)
        return DT_STATUS_INVALID_PARAMETER;

#ifdef WINBUILD

    // Initially nothing written
    pDtString->Length = 0;

    // Convert integer
    if (!NT_SUCCESS(RtlIntegerToUnicodeString(Value, Base, pDtString)))
        return DT_STATUS_BUFFER_OVERFLOW;
    
    // Check space for zero terminator
    if (pDtString->Length >= pDtString->MaximumLength)
        return DT_STATUS_BUFFER_OVERFLOW;
    
    // Add zero terminator
    pDtString->Buffer[pDtString->Length/sizeof(DtStringChar)] = L'\0';

#else

    // Initially nothing written
    pDtString->m_Length = 0;

    // Convert the integer
    if (Base == 10)
        Length = snprintf(pDtString->m_Buffer, pDtString->m_MaximumLength, "%u", Value);
    else if (Base == 16)
        Length = snprintf(pDtString->m_Buffer, pDtString->m_MaximumLength, "%x", Value);
    else
        return DT_STATUS_NOT_SUPPORTED;

    // Check for errors
    if (Length < 0)
        return DT_STATUS_BUFFER_OVERFLOW;
    
    // Copy length
    pDtString->m_Length = (UInt16)Length;

#endif

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtStringUIntegerToDtStringAppend -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtStringUIntegerToDtStringAppend(DtString* pDtString, UInt8 Base, UInt Value)
{
    DtStatus  Status = DT_STATUS_OK;
    DtString  ValueString;
    DtStringChar  ValueStringBuffer[11];

    // Check input params
    if (pDtString == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Init buffer for ValueString
    DT_STRING_INIT(ValueString, ValueStringBuffer, 11);

    // Convert UInt32 to temp string
    Status = DtStringUIntegerToDtString(&ValueString, Base, Value);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Append temp string to input string
    return DtStringAppendDtString(pDtString, &ValueString);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtStringUInt64ToDtString -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtStringUInt64ToDtString(DtString* pDtString, UInt8 Base, UInt64 Value)
{
    DtStatus  Status = DT_STATUS_OK;
#ifdef WINBUILD
    NTSTATUS  NtStatus;
#else
    Int  Length;
#endif

    // Check input params
    if (pDtString == NULL)
        return DT_STATUS_INVALID_PARAMETER;

#ifdef WINBUILD

    // The best way to implement this would be to call RtlInt64ToUnicodeString, however
    // that function crashes as soon as bit 63 is set. As a result we use this workaround.
    if (Base == 10) 
        NtStatus = RtlUnicodeStringPrintf(pDtString, L"%llu", Value);
    else if (Base == 16)
        NtStatus = RtlUnicodeStringPrintf(pDtString, L"%llx", Value);
    else
        return DT_STATUS_NOT_SUPPORTED;

    if (!NT_SUCCESS(NtStatus))
        return DT_STATUS_BUFFER_OVERFLOW;
    
    // Check space for zero terminator
    if (pDtString->Length >= pDtString->MaximumLength)
        return DT_STATUS_BUFFER_OVERFLOW;
    
    // Add zero terminator
    pDtString->Buffer[pDtString->Length/sizeof(DtStringChar)] = L'\0';;

#else
    
    // Initially nothing written
    pDtString->m_Length = 0;

    // Convert the integer
    if (Base == 10)
        Length = snprintf(pDtString->m_Buffer, pDtString->m_MaximumLength, "%Lu", Value);
    else if (Base == 16)
        Length = snprintf(pDtString->m_Buffer, pDtString->m_MaximumLength, "%Lx", Value);
    else
        return DT_STATUS_NOT_SUPPORTED;

    // Check for errors
    if (Length < 0)
        return DT_STATUS_BUFFER_OVERFLOW;
    
    // Copy length
    pDtString->m_Length = (UInt16)Length;

#endif

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtStringUInt64ToDtStringAppend -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtStringUInt64ToDtStringAppend(DtString* pDtString, UInt8 Base, UInt64 Value)
{
    DtStatus  Status = DT_STATUS_OK;
    DtString  ValueString;
    DtStringChar  ValueStringBuffer[21];  // Max. length int64

    // Check input params
    if (pDtString == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Init buffer for ValueString
    DT_STRING_INIT(ValueString, ValueStringBuffer, 21);

    // Convert UInt64 to temp string
    Status = DtStringUInt64ToDtString(&ValueString, Base, Value);
    if (!DT_SUCCESS(Status))
        return Status;

    // Append temp string to input string
    return DtStringAppendDtString(pDtString, &ValueString);
}
