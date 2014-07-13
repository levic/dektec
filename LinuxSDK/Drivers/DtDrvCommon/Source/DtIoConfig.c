//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoConfig.c *#*#*#*#*#*#*#*#*#*#* (C) 2014 DekTec
//
// Driver common - IO configuration - Definition of IO configuration types/functions
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2014 DekTec Digital Video B.V.
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

// Implement kstrtos64 for kernel versions which don't include it.
#ifdef LINBUILD
// Split condition in two lines to make sure windows builds don't try to parse
// the part below.
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,39)
int  kstrtos64(const char*  p, int Base, UInt64*  pRes)
{
    *pRes = simple_strtoull(p, NULL, Base);
    return 0;
}
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



DtStatus  DtIoConfigReadFromNonVolatileStorage(
    DtDrvObject*  pSalDrvObj,
    UInt64  Serial,
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
    Result = DtNonVolatileSettingsStringRead(pSalDrvObj, Serial, PortIndex,
                                    GroupName, "ConfigValue", StrValue, sizeof(StrValue));
    if (!DT_SUCCESS(Result))
        return Result;

    Result = IoConfigCodeGet(StrValue, &Value);
    if (!DT_SUCCESS(Result))
        return Result;
    IoConfig.m_Value = (Int)Value;

    // Get config subvalue
    Result = DtNonVolatileSettingsStringRead(pSalDrvObj, Serial, PortIndex,
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
        Result = DtNonVolatileSettingsValueRead(pSalDrvObj, Serial, PortIndex,
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
static void  DtaIoConfigParseIniFile(
    DtIoConfigValueDriver* pCfgValues,
    char*  FileBuf,
    char*  FileEnd)
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

#define DRVCOMMON_TAG  0x544e4d43 // 'TNMC'

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoConfigReadFromIniFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoConfigReadFromIniFile(
    const char*  DriverName,
    UInt64  Serial,
    Int  TypeNumber,
    Int  PortNumber,
    DtIoConfigValueDriver* pCfgValues)
{
    char  Filename[256];
    Int  Length;
    struct file*  File = NULL;
    mm_segment_t  OldFs;
    int  Err = 0;
    char*  FileBuf = NULL;
    loff_t  Offset = 0;

    FileBuf = DtMemAllocPool(DtPoolNonPaged, CONFIG_MAX_FILE_SIZE, DRVCOMMON_TAG);

    OldFs = get_fs();
    set_fs(get_ds());
    Length = snprintf(Filename, sizeof(Filename), "/etc/DekTec/%s/%d/Port%d.ini",
                                                      DriverName, TypeNumber, PortNumber);
    File = filp_open(Filename, O_RDONLY, 0);
    if(!IS_ERR(File))
    {
        // Read data and parse it
        Err = vfs_read(File, FileBuf, CONFIG_MAX_FILE_SIZE-1, &Offset);
        if (Err >= 0)
        {
            DtDbgOut(AVG, IOCONFIG, "Succesfully read file %s. Filesize %d bytes",
                                                                           Filename, Err);
            FileBuf[Err] = '\0';
            DtaIoConfigParseIniFile(pCfgValues, FileBuf, FileBuf+Err);
        } else {
            DtDbgOut(ERR, IOCONFIG, "Error reading config file %s. Error: %d",
                                                                          Filename, -Err);
        }
        filp_close(File, NULL);
    }
    Length = snprintf(Filename, sizeof(Filename), "/etc/DekTec/%s/%llu/Port%d.ini",
                                                          DriverName, Serial, PortNumber);
    File = filp_open(Filename, O_RDONLY, 0);
    if(!IS_ERR(File))
    {
        Int  Size = CONFIG_MAX_FILE_SIZE;
        // Read data and parse it
        Err = vfs_read(File, FileBuf, Size, &Offset);
        if (Err >= 0)
        {
            DtDbgOut(AVG, IOCONFIG, "Succesfully read file %s. Filesize %d bytes",
                                                                           Filename, Err);
            FileBuf[Err] = '\0';
            DtaIoConfigParseIniFile(pCfgValues, FileBuf, FileBuf+Err);
        } else {
            DtDbgOut(ERR, IOCONFIG, "Error reading config file %s. Error: %d",
                                                                          Filename, -Err);
        }
        filp_close(File, NULL);
    }
    set_fs(OldFs);
    DtMemFreePool(FileBuf, DRVCOMMON_TAG);
    return DT_STATUS_OK;
}
#endif
