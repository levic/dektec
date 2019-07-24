//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtFile.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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

#ifndef __DT_FILE_H
#define __DT_FILE_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#ifdef  WINBUILD
    typedef HANDLE  DtFileHandle;
    #define DT_FILE_INVALID_HANDLE  NULL
#else
    typedef int  DtFileHandle;
    #define DT_FILE_INVALID_HANDLE  NULL
#endif

#define DT_FILE_MAX_NAME_LENGTH 512

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct  _DtFile
{
    DtFileHandle  m_Handle;     // Handle to the file
    char  m_Name[DT_FILE_MAX_NAME_LENGTH];  // File name (i.e. full path)
    Int64  m_Size;              // File size
}  DtFile;


// File access flags
#define DT_FILE_ACCESS_READ     0x0001      // Open file for reading
#define DT_FILE_ACCESS_WRITE    0x0002      // Open empty file for writing
#define DT_FILE_ACCESS_RW       0x0003      // Open file for read and write. If file does
                                            // not exist a new file is created

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtFile - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtFile*  DtFile_Open(const char*, Int  Mode);
void  DtFile_Close(DtFile*);
DtStatus  DtFile_Read(DtFile*, Int  NumToRead, UInt8*  pBuf, Int*  pNumRead);
DtStatus  DtFile_Write(DtFile*, Int  NumToWrite, const UInt8*  pBuf);

#endif // #ifndef __DT_FILE_H

