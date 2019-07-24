//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtFile.c *#*#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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
#include <DtSal.h>

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtFile implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static Int64  DtFile_GetSize(DtFile*);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtFile - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtFile_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtFile*  DtFile_Open(const char*  pFilename, Int  Mode)
{
    DtStatus  Status=DT_STATUS_OK;
    DtFile*  pFile = NULL;
    Int  i=0, StrLen=0;

    // Check parameters
    if (pFilename == NULL)
        return NULL;
    if (Mode!=DT_FILE_ACCESS_READ && Mode!=DT_FILE_ACCESS_WRITE
                                                               && Mode!=DT_FILE_ACCESS_RW)
    {
        DtDbgOut(ERR, SAL, "ERROR: Unknown file mode '%d' used for '%s'",
                                                                         Mode, pFilename);
        return NULL;
    }

    DtDbgOut(MIN, SAL, "Opening file '%s' with mode=%d", pFilename, Mode);

    // Allocate file object
    pFile = (DtFile*)DtMemAllocPool(DtPoolNonPaged, sizeof(DtFile), SAL_TAG);
    if (pFile == NULL)
    {
        DtDbgOut(ERR, SAL, "ERROR: could not allocate memory for file object");
        return NULL;
    }

    // Init file object to 'safe' initial state
    DtMemZero(pFile, sizeof(DtFile));
    pFile->m_Handle = DT_FILE_INVALID_HANDLE;
    // Store the file name
    StrLen = DtAnsiCharArrayStrLength(pFilename, DT_FILE_MAX_NAME_LENGTH-1);
    DT_ASSERT(StrLen>0 && StrLen<DT_FILE_MAX_NAME_LENGTH);
    DtMemCopy(pFile->m_Name, pFilename, StrLen);

#ifdef WINBUILD
    // Windows specific implementation
    {
        NTSTATUS  NtStatus = STATUS_SUCCESS;
        IO_STATUS_BLOCK  IoStatus;
        UInt32  DesiredAccess=0, ShareAccess=FILE_SHARE_READ, CreateDisposition=0;
        DtString  FileNameUnicode;
        OBJECT_ATTRIBUTES   ObjAttr;

        // Create a UNICODE string for the file name (path)
        Status = DtStringAlloc(&FileNameUnicode, 512);
        if (!DT_SUCCESS(Status))
        {
            DT_ASSERT(DT_SUCCESS(Status));
            DtFile_Close(pFile);
            return NULL;
        }
        Status = DtStringAppendChars(&FileNameUnicode, pFilename);
        if (!DT_SUCCESS(Status))
        {
            DT_ASSERT(DT_SUCCESS(Status));
            // Donot forget to free the unicode string buffer
            DtStringFree(&FileNameUnicode);
            DtFile_Close(pFile);
            return NULL;
        }

        // Initialise object attributes
        InitializeObjectAttributes(&ObjAttr, &FileNameUnicode,
                                    OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);

        // Set access and creation flags based on deseried file mode
        switch (Mode)
        {
        case DT_FILE_ACCESS_READ:
            DesiredAccess = GENERIC_READ | SYNCHRONIZE;
            CreateDisposition = FILE_OPEN;          // Open (must exist)
            break;
        case DT_FILE_ACCESS_WRITE:
            DesiredAccess = GENERIC_WRITE | SYNCHRONIZE;
            CreateDisposition = FILE_OPEN_IF;       // Open if exist, else create
        case DT_FILE_ACCESS_RW:
            DesiredAccess = GENERIC_ALL | SYNCHRONIZE;
            CreateDisposition = FILE_OPEN_IF;       // Open if exist, else create
            break;

        default:
            DT_ASSERT(FALSE);
            break;
        }
    
        // Create/open the file
        NtStatus = ZwCreateFile(&pFile->m_Handle, DesiredAccess,
                                       &ObjAttr,  &IoStatus, NULL,
                                       FILE_ATTRIBUTE_NORMAL,
                                       FILE_SHARE_READ,    // Allow shared read access
                                       CreateDisposition,
                                       FILE_SYNCHRONOUS_IO_NONALERT,  // Open synchroneous
                                       NULL, 0);

        // Donot forget to free the unicode string buffer
        DtStringFree(&FileNameUnicode);
        
        // Did we succeed to create/open the file
        if (!NT_SUCCESS(NtStatus))
        {
            DtDbgOut(ERR, SAL, "ERROR: failed to open file '%s' (Status=0x%08X)",
                                                                     pFilename, NtStatus);
            DtFile_Close(pFile);
            return NULL;
        }
    }
#else
    // Linux specific implementation
    {
    }
#endif

    // Init file size
    pFile->m_Size = DtFile_GetSize(pFile);
    DT_ASSERT(pFile->m_Size >= 0);

    DtDbgOut(MIN, SAL, "Opened file '%s': mode=%d, size=%lld", pFilename, Mode, 
                                                                           pFile->m_Size);
    return pFile;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtFile_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtFile_Close(DtFile*  pFile)
{
    if (pFile == NULL)
        return;

#ifdef WINBUILD
    // Windows specific implementation
    if (pFile->m_Handle != DT_FILE_INVALID_HANDLE)
        ZwClose(pFile->m_Handle);
#else
    // Linux specific implementation
#endif

    // Free the file structure
    DtMemFreePool(pFile, SAL_TAG);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtFile_Read -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtFile_Read(DtFile*  pFile, Int  NumToRead, UInt8*  pBuf, Int*  pNumRead)
{
    // Check parameters
    if (pFile==NULL || pFile->m_Handle==DT_FILE_INVALID_HANDLE)
        return  DT_STATUS_INVALID_PARAMETER;
    if (pBuf==NULL || NumToRead<0)
        return DT_STATUS_INVALID_PARAMETER;

    // Init number-of-bytes-read to 'safe' initial value
    if (pNumRead != NULL)
        *pNumRead = 0;

#ifdef WINBUILD
    // Windows specific implementation
    {
        NTSTATUS  NtStatus = STATUS_SUCCESS;
        IO_STATUS_BLOCK  IoStatus;

        NtStatus = ZwReadFile(pFile->m_Handle, NULL, NULL, NULL, &IoStatus,
                                                             pBuf, NumToRead, NULL, NULL);

        // Did we succeed to create/open the file
        if (!NT_SUCCESS(NtStatus))
        {
            DtDbgOut(ERR, SAL, "ERROR: failed to read from file '%s' (Status=0x%08X)",
                                                                 pFile->m_Name, NtStatus);
            return (NtStatus == STATUS_END_OF_FILE) ? DT_STATUS_EOF : DT_STATUS_FAIL;
        }
        
        // Should we report the number-of-bytes actually read?
        if (pNumRead != NULL)
            *pNumRead = (Int)IoStatus.Information;
    }
#else
    // Linux specific implementation
#endif
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtFile_Write -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtFile_Write(DtFile*  pFile, Int  NumToWrite, const UInt8*  pBuf)
{
    // Check parameters
    if (pFile==NULL || pFile->m_Handle==DT_FILE_INVALID_HANDLE)
        return DT_STATUS_INVALID_PARAMETER;
    if (pBuf==NULL || NumToWrite<0)
        return DT_STATUS_INVALID_PARAMETER;

    // Anything to write at all
    if (NumToWrite == 0)
        return DT_STATUS_OK;

#ifdef WINBUILD
    // Windows specific implementation
    {
        NTSTATUS  NtStatus = STATUS_SUCCESS;
        IO_STATUS_BLOCK  IoStatus;

        NtStatus = ZwWriteFile(pFile->m_Handle, NULL, NULL, NULL, &IoStatus,
                                                     (void*)pBuf, NumToWrite, NULL, NULL);

        // Did we succeed to create/open the file
        if (!NT_SUCCESS(NtStatus))
        {
            DtDbgOut(ERR, SAL, "ERROR: failed to write to file '%s' (Status=0x%08X)",
                                                                 pFile->m_Name, NtStatus);
            return (NtStatus == STATUS_END_OF_FILE) ? DT_STATUS_EOF : DT_STATUS_FAIL;
        }
    }
#else
    // Linux specific implementation
#endif

    // Update the file size after a succesfull write
    pFile->m_Size += NumToWrite;
    DT_ASSERT(pFile->m_Size == DtFile_GetSize(pFile));

    return DT_STATUS_OK;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtFile - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

Int64  DtFile_GetSize(DtFile*  pFile)
{
    Int64  Size = 0;

    DT_ASSERT(pFile!=NULL && pFile->m_Handle!=DT_FILE_INVALID_HANDLE);

    if (pFile->m_Handle == DT_FILE_INVALID_HANDLE)
        return -1;

#ifdef WINBUILD
    // Windows specific implementation
    {
        NTSTATUS  NtStatus = STATUS_SUCCESS;
        IO_STATUS_BLOCK  IoStatus;
        FILE_STANDARD_INFORMATION  FileInfo;

        NtStatus = ZwQueryInformationFile(pFile->m_Handle, &IoStatus, &FileInfo, 
                                               sizeof(FileInfo), FileStandardInformation);
        if (!NT_SUCCESS(NtStatus))
        {
            DtDbgOut(ERR, SAL, "ERROR: failed to query file info '%s' (Status=0x%08X)",
                                                                 pFile->m_Name, NtStatus);
            return -1;
        }
        // Get the size of the file
        Size = (Int64)FileInfo.EndOfFile.QuadPart;
    }
#else
    // Linux specific implementation
#endif
    return Size;
}

