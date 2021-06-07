//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoConfig.c *#*#*#*#*#*#*#*# (C) 2010-2016 DekTec
//
// Driver common - IO configuration - Definition of IO configuration types/functions
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


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtDrvCommon.h>

#ifdef LINBUILD

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
// New kernel file read functions in kernel >=5.10
// Fedora33 is missing include file: <linux/kernel_read_file.h>
#ifndef __kernel_read_file_id
#define __kernel_read_file_id(id)           \
    id(UNKNOWN, unknown)                    \
    id(FIRMWARE, firmware)                  \
    id(MODULE, kernel-module)               \
    id(KEXEC_IMAGE, kexec-image)            \
    id(KEXEC_INITRAMFS, kexec-initramfs)    \
    id(POLICY, security-policy)             \
    id(X509_CERTIFICATE, x509-certificate)  \
    id(MAX_ID, )

#define __fid_enumify(ENUM, dummy) READING_ ## ENUM,
enum kernel_read_file_id {
    __kernel_read_file_id(__fid_enumify)
};
#endif

int kernel_read_file_from_path(const char* path, loff_t offset,
    void** buf, size_t buf_size,
    size_t* file_size,
    enum kernel_read_file_id id);
#endif

// Implement kstrtos64 for kernel versions which don't include it.
#if defined(RHEL_RELEASE_CODE)
#if RHEL_RELEASE_CODE>=RHEL_RELEASE_VERSION(6,4)
#define SKIP_KSTRTOS_FOR_RHEL
#endif
#endif
// Split condition in two lines to make sure windows builds don't try to parse
// the part below.
#if LINUX_VERSION_CODE<KERNEL_VERSION(2,6,39) && !defined(SKIP_KSTRTOS_FOR_RHEL)
int  kstrtos64(const char*  p, int Base, UInt64*  pRes)
{
    *pRes = simple_strtoull(p, NULL, Base);
    return 0;
}
#endif

// Linux kernel versions >=5.1
#ifndef get_ds
#define get_ds() (KERNEL_DS)
#endif
#endif


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Global data -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

#ifdef WINBUILD
// ParXtra names
static const char*   IoParXtraNames[DT_MAX_PARXTRA_COUNT] = {
        "IoParXtra0", 
        "IoParXtra1", 
        "IoParXtra2", 
        "IoParXtra3" };



// .-.-.-.-.-.-.-.-.-.-.-.- DtIoConfigReadFromNonVolatileStorage -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoConfigReadFromNonVolatileStorage(
    DtDrvObject*  pSalDrvObj,
    UInt64  Serial,
    Int  FwVariant,
    Int  PortIndex,
    Int  IoGroup,
    DtIoConfigValueDriver*  pCfgValue)
{
    DtStatus  Result;
    Int32  Value;
    Int64  BinValue;
    Char StrValue[IOCONFIG_NAME_MAX_SIZE];
    Int  ParXtraIdx;
    DtIoConfigValueDriver  IoConfig;
    Char GroupName[IOCONFIG_NAME_MAX_SIZE];

    
    // Get the IO group name
    Result = IoConfigNameGet(IoGroup, GroupName, sizeof(GroupName) );
    if (!DT_SUCCESS(Result))
        return Result;

    // Get config value
    Result = DtNonVolatileSettingsStringRead(pSalDrvObj, Serial, FwVariant, PortIndex,
                                    GroupName, "ConfigValue", StrValue, sizeof(StrValue));
    if (!DT_SUCCESS(Result))
        return Result;

    Result = IoConfigCodeGet(StrValue, &Value);
    if (!DT_SUCCESS(Result))
        return Result;
    IoConfig.m_Value = (Int)Value;

    // Get config subvalue
    Result = DtNonVolatileSettingsStringRead(pSalDrvObj, Serial, FwVariant, PortIndex,
                                 GroupName, "ConfigSubValue", StrValue, sizeof(StrValue));
    if (!DT_SUCCESS(Result))
        return Result;

    Result = IoConfigCodeGet(StrValue, &Value);
    if (!DT_SUCCESS(Result))
        return Result;
    IoConfig.m_SubValue = Value;
    
    // Read ParXtra
    for (ParXtraIdx=0; ParXtraIdx<DT_MAX_PARXTRA_COUNT; ParXtraIdx++)
    {
        Result = DtNonVolatileSettingsValueRead(pSalDrvObj, Serial, FwVariant, PortIndex,
                                 GroupName, (Char*)IoParXtraNames[ParXtraIdx], &BinValue);
        if (!DT_SUCCESS(Result))
            return DT_STATUS_CONFIG_ERROR;
        IoConfig.m_ParXtra[ParXtraIdx] = BinValue;
    }

    // Copy result only if succeeded
    *pCfgValue = IoConfig;

    return DT_STATUS_OK;
}

#else // WINBUILD


#define CONFIG_MAX_FILE_SIZE (64*1024)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaIoConfigParseIniFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static void  DtaIoConfigParseIniFile(DtIoConfigValueDriver* pCfgValues, char* FileBuf,
                                                                            char* FileEnd)
{
    Int  ConfigCode = -1;
    char*  p = FileBuf;
    while (p < FileEnd)
    {
        // We parse per line, so find where the current line ends
        char*  pLineEnd = p;
        while (pLineEnd<FileEnd && *pLineEnd!='\n' && *pLineEnd!='\r')
            pLineEnd++;
        *pLineEnd = '\0';
        while (p<pLineEnd && (*p==' ' || *p=='\t'))
            p++;
        if (*p==';' || *p=='#')
        {
            // Rest of the line is a comment, ignore it
        }
        else if (p == pLineEnd)
        {
            // Empty line, skip
        }
        else if (*p == '[')
        {
            // Start of new section
            char*  pEndSectName;
            // Skip [ character
            p++;
            // Skip whitespace. Range check not needed since we have a terminating '\0'
            // at the end of the line.
            while (*p==' ' || *p=='\t')
                p++;
            pEndSectName = p;
            while (*pEndSectName!='\0' && *pEndSectName!=']')
                pEndSectName++;
            if (*pEndSectName != ']')
            {
                DtDbgOut(ERR, IOCONFIG, "Invalid section header");
                return;
            }
            // Go backwards to last non-whitespace character
            // Cannot go out of bounds since it'll find the opening [ eventually if
            // nothing else.
            while (pEndSectName[-1]==' ' || pEndSectName[-1]=='\t')
                pEndSectName--;
            *pEndSectName = '\0';
            if (!DT_SUCCESS(IoConfigCodeGet(p, &ConfigCode)))
            {
                DtDbgOut(ERR, IOCONFIG, "Unrecognized IoConfig category");
                return;
            }
        } else {
            // Assume a key=value pair on this line
            char*  pEq;
            char*  pVal;
            char*  pValEnd;

            if (ConfigCode == -1)
            {
                DtDbgOut(ERR, IOCONFIG, "key=value pair before first section");
                return;
            }
            if (*p == '=')
            {
                // Line starts with = sign
                DtDbgOut(ERR, IOCONFIG, "Invalid line: not allowed to start with =");
                return;
            }
            // Find equals sign
            pEq = p;
            while (pEq<pLineEnd && *pEq!='=')
                pEq++;
            if (pEq == pLineEnd)
            {
                DtDbgOut(ERR, IOCONFIG, "No = found");
                return;
            }
            pVal = pEq+1;
            // Search backspace for last whitespace. We're guaranteed to find something,
            // first non-whitespace was not '='.
            while (pEq[-1]==' ' || pEq[-1]=='\t')
                pEq--;
            *pEq = '\0';
            while (*pVal==' ' || *pVal=='\t')
                pVal++;
            // Find next whitespace or end-of-line.
            pValEnd = pVal;
            while (*pValEnd!=' ' && *pValEnd!='\t' && *pValEnd!='\0')
                pValEnd++;
            *pValEnd = '\0';

            if (strcmp(p, "ConfigValue") == 0)
            {
                if (!DT_SUCCESS(IoConfigCodeGet(pVal, &pCfgValues[ConfigCode].m_Value)))
                {
                    DtDbgOut(ERR, IOCONFIG, "Failed to parse ConfigValue");
                    return;
                }
            }
            else if (strcmp(p, "ConfigSubValue") == 0)
            {
                if (!DT_SUCCESS(IoConfigCodeGet(pVal,&pCfgValues[ConfigCode].m_SubValue)))
                {
                    DtDbgOut(ERR, IOCONFIG, "Failed to parse ConfigSubValue");
                    return;
                }
            }
            else if (strcmp(p, "IoParXtra0") == 0)
            {
                if (kstrtos64(pVal, 0, &pCfgValues[ConfigCode].m_ParXtra[0]) < 0)
                {
                    DtDbgOut(ERR, IOCONFIG, "Failed to parse IoParXtra0");
                    return;
                }
            }
            else if (strcmp(p, "IoParXtra1") == 0)
            {
                if (kstrtos64(pVal, 0, &pCfgValues[ConfigCode].m_ParXtra[1]) < 0)
                {
                    DtDbgOut(ERR, IOCONFIG, "Failed to parse IoParXtra1");
                    return;
                }
            }
            else if (strcmp(p, "IoParXtra2") == 0)
            {
                if (kstrtos64(pVal, 0, &pCfgValues[ConfigCode].m_ParXtra[2]) < 0)
                {
                    DtDbgOut(ERR, IOCONFIG, "Failed to parse IoParXtra2");
                    return;
                }
            }
            else if (strcmp(p, "IoParXtra3") == 0)
            {
                if (kstrtos64(pVal, 0, &pCfgValues[ConfigCode].m_ParXtra[3]) < 0)
                {
                    DtDbgOut(ERR, IOCONFIG, "Failed to parse IoParXtra3");
                    return;
                }
            } else {
                DtDbgOut(ERR, IOCONFIG, "Unknown key encountered");
                return;
            }
        }
        p = pLineEnd+1;
    }
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- KernelReadFileFromPath -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// The ppBuf will be allocated by this function. 
// if ppBuf!=NULL, the caller must free the buffer with vfree after use.
// If return value > 0, the number of bytes read from file is returned.
//
Int  KernelReadFileFromPath(const char* Filename, void** ppBuf, size_t BufSize)
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(5,10,0)
    size_t  FileSize;
    *ppBuf = NULL;
    return kernel_read_file_from_path(Filename, 0, ppBuf, BufSize, &FileSize, 
                                                                        READING_FIRMWARE);
#else
    struct file* File = NULL;
    mm_segment_t  OldFs;
    int  Err = 0;
    loff_t  Offset = 0;

    *ppBuf = vmalloc(BufSize);
    if (*ppBuf == NULL)
        return -ENOMEM;
    OldFs = get_fs();
    set_fs(get_ds());
    File = filp_open(Filename, O_RDONLY, 0);
    if (!IS_ERR(File))
    {
        // Read data and parse it
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4,14,0)
        Err = kernel_read(File, *ppBuf, BufSize - 1, &Offset);
#else
        Err = vfs_read(File, *ppBuf, BufSize - 1, &Offset);
#endif
        if (Err < 0)
        {
            vfree(*ppBuf);
            *ppBuf = NULL;
        }
        filp_close(File, NULL);
    }
    else {
        vfree(*ppBuf);
        *ppBuf = NULL;
        Err = -EIO;
    }
    set_fs(OldFs);
    return Err;
#endif
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoConfigReadFromIniFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoConfigReadFromIniFile(const char* DriverName, UInt64 Serial, Int TypeNumber,
                                        Int PortNumber, DtIoConfigValueDriver* pCfgValues)
{
    char  Filename[256];
    Int  Length;
    Int  Err = 0;
    char*  pFileBuf = NULL;
    
    Length = snprintf(Filename, sizeof(Filename), "/etc/DekTec/%s/%d/Port%d.ini",
                                                      DriverName, TypeNumber, PortNumber);
    Err = KernelReadFileFromPath(Filename, (void**)&pFileBuf, CONFIG_MAX_FILE_SIZE);
    if (Err > 0 && Err < (CONFIG_MAX_FILE_SIZE - 1))
    {
        DtDbgOut(AVG, IOCONFIG, "Succesfully read file %s. Filesize %d bytes", Filename, 
                                                                                     Err);
        pFileBuf[Err] = '\0';
        DtaIoConfigParseIniFile(pCfgValues, pFileBuf, pFileBuf + Err);
    }
    else 
    {
        DtDbgOut(ERR, IOCONFIG, "Error reading config file %s. Error: %xh", Filename, 
                                                                                     Err);
    }
    if (pFileBuf != NULL)
        vfree(pFileBuf);
    pFileBuf = NULL;

    Length = snprintf(Filename, sizeof(Filename), "/etc/DekTec/%s/%llu/Port%d.ini",
                                                          DriverName, Serial, PortNumber);
    Err = KernelReadFileFromPath(Filename, (void**)&pFileBuf, CONFIG_MAX_FILE_SIZE);
    if (Err > 0 && Err < (CONFIG_MAX_FILE_SIZE - 1))
    {
        DtDbgOut(AVG, IOCONFIG, "Succesfully read file %s. Filesize %d bytes", Filename, 
                                                                                     Err);
        pFileBuf[Err] = '\0';
        DtaIoConfigParseIniFile(pCfgValues, pFileBuf, pFileBuf + Err);
    }
    else 
    {
        DtDbgOut(ERR, IOCONFIG, "Error reading config file %s. Error: %xh", Filename,
                                                                                     Err);
    }
    if (pFileBuf != NULL)
        vfree(pFileBuf);
    pFileBuf = NULL;
    return DT_STATUS_OK;
}
#endif

