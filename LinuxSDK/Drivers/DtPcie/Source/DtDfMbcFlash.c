// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfMbcFlash.c *#*#*#*#*#*#*#*#*#*# (C) 2024 DekTec
//
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2024 DekTec Digital Video B.V.
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
#include "DtDfMbcFlash.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        
#define FMBC_ROLE_NONE      NULL


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMbcFlash implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfMbcFlash function
#define DF_MBCFLASH_DEFAULT_PRECONDITIONS(pDf)      \
                                 DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfMbcFlash))

// MACRO that checks the function has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_MBCFLASH_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(MBCFLASH, pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfMbcFlash_Init(DtDf*);
static DtStatus  DtDfMbcFlash_InitFlashProperties(DtDfMbcFlash*);
static Int DtDfMbcFlash_StringCompare(const char* Str1, const char* Str2);
static void DtDfMbcFlash_CopyAndReverseBits(UInt32* pDest, const UInt32* pSrc, Int Length);
static DtStatus DtDfMbcFlash_OpenFlashAccess(DtDfMbcFlash*);
static DtStatus DtDfMbcFlash_CloseFlashAccess(DtDfMbcFlash*);
static DtStatus  DtDfMbcFlash_LoadParameters(DtDf*);
static DtStatus  DtDfMbcFlash_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfMbcFlash_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfMbcFlash_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfMbcFlash_OpenChildren(DtDfMbcFlash*);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMbcFlash - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfMbcFlash_Close(DtDf* pDfBase)
{
    DtDfMbcFlash* pDf = (DtDfMbcFlash*)pDfBase;

    DF_MBCFLASH_DEFAULT_PRECONDITIONS(pDf);

    // Free the temporary buffer
    if (pDf->m_pTempBuffer != NULL)
    {
        DtMemFreePool(pDf->m_pTempBuffer, DF_TAG);
        pDf->m_pTempBuffer = NULL;
    }

    // Let base function perform final clean-up
    DtDf_Close(pDfBase);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfMbcFlash*  DtDfMbcFlash_Open(DtCore* pCore, DtPt* pPt, const char* pRole, 
                                                  Int Instance, Int Uuid, Bool CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_MbcFlash_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfMbcFlash, Id, DT_FUNC_TYPE_MBCFLASH, pPt,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfMbcFlash_Close;
    OpenParams.m_InitFunc = DtDfMbcFlash_Init;
    OpenParams.m_LoadParsFunc = DtDfMbcFlash_LoadParameters;
    OpenParams.m_OnCloseFileFunc = DtDfMbcFlash_OnCloseFile;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfMbcFlash_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfMbcFlash_OnEnablePreChildren;
    
    // Use base function to allocate and perform standard initialization of function data
    return (DtDfMbcFlash*)DtDf_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_Erase -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfMbcFlash_Erase(DtDfMbcFlash* pDf, Int StartAddress, Int NumToErase)
{
    DtStatus  Status;
    Int Timeout;
    UInt32 Data[2];

    DF_MBCFLASH_DEFAULT_PRECONDITIONS(pDf);

    // Must be enabled
    DF_MBCFLASH_MUST_BE_ENABLED(pDf);

    if (pDf->m_Locked)
    {
        DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: device is locked");
        return DT_STATUS_LOCKED;
    }

    // Open flash access
    Status = DtDfMbcFlash_OpenFlashAccess(pDf);
    if (Status != DT_STATUS_OK)
    {
        DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: Open flash failed");
        // We always close the flash. Some parts of the open flash could be executed 
        // already 
        DtDfMbcFlash_CloseFlashAccess(pDf);
        return Status;
    }

    // Complete or sector erase?
    if (StartAddress!=0 || NumToErase!=pDf->m_PropMemorySize)
    {
        // Sector erase. Check parameters
        if ((StartAddress+NumToErase)>pDf->m_PropMemorySize
            || (pDf->m_PropSectorSize==0)  // No sectors
            || (StartAddress%pDf->m_PropSectorSize)!=0
            || (NumToErase%pDf->m_PropSectorSize)!=0)
        {
            // Invalid parameters
            DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: erase area is not sector aligned");
            return DT_STATUS_INVALID_PARAMETER;
        }
    }
    // Issue and erase command. Pass the start address and the number of words to erase
    Data[0] = StartAddress;
    Data[1] = NumToErase/4;
    Timeout = (NumToErase/pDf->m_PropSectorSize) * pDf->m_PropMaxSectorEraseTime;
    Status =  DtBcFMBC_Write(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_QPSI_ERASE, 2, Data, Timeout);
    if (Status != DT_STATUS_OK)
    {
        DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: Write flash failed");
        DtDfMbcFlash_CloseFlashAccess(pDf);
        return Status;
    }

    // Close flash access
    Status = DtDfMbcFlash_CloseFlashAccess(pDf);
    if (Status != DT_STATUS_OK)
        DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: Close flash failed");
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_LockDevice -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfMbcFlash_LockDevice(DtDfMbcFlash* pDf, Bool Lock)
{
    DF_MBCFLASH_DEFAULT_PRECONDITIONS(pDf);

    // Must be enabled
    DF_MBCFLASH_MUST_BE_ENABLED(pDf);

    // Store new lock state
    pDf->m_Locked = Lock;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_Read -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfMbcFlash_Read(DtDfMbcFlash* pDf, Int StartAddress, Int NumToRead,
                                                                              UInt8* pBuf)
{
    DtStatus Status = DT_STATUS_OK;
    Int NumWordsRemaining, Address;
    Int Timeout = 10;

    DF_MBCFLASH_DEFAULT_PRECONDITIONS(pDf);

    // Must be enabled
    DF_MBCFLASH_MUST_BE_ENABLED(pDf);

    if (pDf->m_Locked)
    {
        DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: device is locked");
        return DT_STATUS_LOCKED;
    }

    // Check for parameter errors.
    if (pBuf==NULL || ((UIntPtr)pBuf&0x3)!=0
        || (StartAddress+NumToRead)>pDf->m_PropMemorySize
        || (StartAddress%4)!=0 || (NumToRead%4)!=0)
    {
        // Invalid start address or #read 
        DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: invalid params address: 0x%x,"
                                              "read-length: %d", StartAddress, NumToRead);
        return DT_STATUS_INVALID_PARAMETER;
    }
    
    // Open flash access
    Status = DtDfMbcFlash_OpenFlashAccess(pDf);
    if (Status != DT_STATUS_OK)
    {
        DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: Open flash failed");
        // We always close the flash. Some parts of the open flash could be executed 
        // already 
        DtDfMbcFlash_CloseFlashAccess(pDf);
        return Status;
    }

    NumWordsRemaining = NumToRead/4;
    Address = StartAddress;
    while (NumWordsRemaining > 0)
    {
        UInt32 NumWordsToRead = (NumWordsRemaining > pDf->m_FmbcMaxRespSize) ?
                            NumWordsToRead = pDf->m_FmbcMaxRespSize : NumWordsRemaining;
        UInt32 Data[2];
        Data[0] = Address;
        Data[1] = NumWordsToRead;
        Status = DtBcFMBC_WriteRead(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_QSPI_READ,
                                    2, Data, NumWordsToRead, pDf->m_pTempBuffer, Timeout);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: failed reading from address: 0x%x,"
                                                   "length: %d", Address, NumWordsToRead);
            DtDfMbcFlash_CloseFlashAccess(pDf);
            return Status;
        }
        DtDfMbcFlash_CopyAndReverseBits((UInt32*)pBuf, pDf->m_pTempBuffer, NumWordsToRead);
        Address += NumWordsToRead*4;
        pBuf += NumWordsToRead*4;
        NumWordsRemaining -= NumWordsToRead;
    }
    // Close flash access
    Status = DtDfMbcFlash_CloseFlashAccess(pDf);
    if (Status != DT_STATUS_OK)
        DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: Close flash failed");
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_Write -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfMbcFlash_Write(DtDfMbcFlash* pDf, Int StartAddress, Int NumToWrite,
                                                                        const UInt8* pBuf)
{
    DtStatus Status = DT_STATUS_OK;
    Int NumWordsRemaining, Address;
    DF_MBCFLASH_DEFAULT_PRECONDITIONS(pDf);

    // Must be enabled
    DF_MBCFLASH_MUST_BE_ENABLED(pDf);

    // Check for parameter errors.
    if (pBuf==NULL || ((UIntPtr)pBuf&0x3)!=0
        || (StartAddress+NumToWrite)>pDf->m_PropMemorySize
        || (StartAddress%pDf->m_PropPageSize)!=0
        || (NumToWrite%pDf->m_PropPageSize)!=0)
    {
        // Invalid start address or #write
        DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: invalid params address: 0x%x,"
            "write-length: %d", StartAddress, NumToWrite);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Open flash access
    Status = DtDfMbcFlash_OpenFlashAccess(pDf);
    if (Status != DT_STATUS_OK)
    {
        DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: Open flash failed");
        // We always close the flash. Some parts of the open flash could be executed 
        // already 
        DtDfMbcFlash_CloseFlashAccess(pDf);
        return Status;
    }

    NumWordsRemaining = NumToWrite/4;
    Address = StartAddress;
    while (NumWordsRemaining > 0)
    {
        Int Timeout;
        Int PageSizeNumWords = pDf->m_PropPageSize/4;
        UInt32 NumWordsToWrite = ((NumWordsRemaining+2) > pDf->m_FmbcMaxCmdSize) ?
                          NumWordsToWrite = pDf->m_FmbcMaxCmdSize-2 : NumWordsRemaining;
        // Write page aligned
        NumWordsToWrite = (NumWordsToWrite/PageSizeNumWords) * PageSizeNumWords;
        pDf->m_pTempBuffer[0] = Address;
        pDf->m_pTempBuffer[1] = NumWordsToWrite;
        DtDfMbcFlash_CopyAndReverseBits(&pDf->m_pTempBuffer[2],(UInt32*)pBuf,
                                                                         NumWordsToWrite);
        Timeout = ((NumWordsToWrite*4)/pDf->m_PropPageSize) * pDf->m_PropMaxPageWriteTime;
        Timeout += 10;
        Status = DtBcFMBC_Write(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_QSPI_WRITE,
                                          NumWordsToWrite+2, pDf->m_pTempBuffer, Timeout);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: failed writing to address: 0x%x,"
                                           "length: %d", Address, NumWordsToWrite);
            DtDfMbcFlash_CloseFlashAccess(pDf);
            return Status;
        }
        Address += NumWordsToWrite*4;
        pBuf += NumWordsToWrite*4;
        NumWordsRemaining -= NumWordsToWrite;
    }

    // Close flash access
    Status = DtDfMbcFlash_CloseFlashAccess(pDf);
    if (Status != DT_STATUS_OK)
        DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: Close flash failed");
    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMbcFlash - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfMbcFlash_Init(DtDf* pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMbcFlash* pDf = (DtDfMbcFlash*)pDfBase;

    // Sanity checks
    DF_MBCFLASH_DEFAULT_PRECONDITIONS(pDf);

    pDf->m_DeviceId = MBCFLASH_DEVICE_UNDEFINED;
    pDf->m_pTempBuffer = NULL;
    pDf->m_FmbcMaxCmdSize = 0;
    pDf->m_FmbcMaxRespSize = 0;
    pDf->m_Locked = TRUE;
    Status = DtDfMbcFlash_InitFlashProperties(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: failed to initialize flash properties");
        return DT_STATUS_FAIL;
    }

    //Open children
    Status = DtDfMbcFlash_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_InitFlashProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfMbcFlash_InitFlashProperties(DtDfMbcFlash* pDf)
{

    // Determine device ID
    if (pDf->m_pDeviceType == NULL)
        pDf->m_DeviceId = MBCFLASH_DEVICE_UNDEFINED;
    else if (!DtDfMbcFlash_StringCompare(pDf->m_pDeviceType, "MT25QU256"))
        pDf->m_DeviceId = MBCFLASH_DEVICE_MT25QU256;
    else
        pDf->m_DeviceId = MBCFLASH_DEVICE_UNDEFINED;

    // Initialize the device properties
    switch (pDf->m_DeviceId)
    {
    case MBCFLASH_DEVICE_MT25QU256:
        pDf->m_PropMemorySize = 32*1024*1024;   // 32MByte (256Mbit)
        pDf->m_PropPageSize = 256;              // 256 byte pages
        pDf->m_PropSectorSize = 64*1024;        // 64KByte sectors
        pDf->m_PropMaxSectorEraseTime = 1000;   // Maximum 1 second per sector
        pDf->m_PropMaxPageWriteTime = 3;        // Maximum 3 millisecond per page
        break;

    default:
        DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: unsupported flash: %d", pDf->m_DeviceId);
        return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_StringCompare -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Compares two C strings.
//
Int DtDfMbcFlash_StringCompare(const char* Str1, const char* Str2)
{
    while (*Str1 && (*Str1 == *Str2)) {
        Str1++;
        Str2++;
    }
    return *(unsigned char*)Str1 - *(unsigned char*)Str2;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_CopyAndReverseBits -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtDfMbcFlash_CopyAndReverseBits(UInt32* pDest, const UInt32* pSrc, Int Length)
{
    int i=Length;
    for (;i>0; i--)
    { 
        UInt32 Word = *pSrc++;
        Word = (Word & 0xF0F0F0F0) >> 4 | (Word & 0x0F0F0F0F) << 4;
        Word = (Word & 0xCCCCCCCC) >> 2 | (Word & 0x33333333) << 2;
        *pDest++ = (Word & 0xAAAAAAAA) >> 1 | (Word & 0x55555555) << 1;
    }
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_OpenFlashAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfMbcFlash_OpenFlashAccess(DtDfMbcFlash* pDf)
{
    DtStatus Status;
    Int Timeout = 10;
    // Open the access to the qspi flash
    Status = DtBcFMBC_Write(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_QSPI_OPEN, 0, NULL, Timeout);
    if (Status == DT_STATUS_OK)
    {
        // Select the qspi flash
        UInt32 Data = 0;
        Status = DtBcFMBC_Write(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_QSPI_SET_CS, 1, &Data,
                                                                                 Timeout);
    }
    return Status;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_CloseFlashAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfMbcFlash_CloseFlashAccess(DtDfMbcFlash* pDf)
{
    Int Timeout = 10;
    // Close flash device
    return DtBcFMBC_Write(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_QPSI_CLOSE, 0, NULL, Timeout);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Note: LoadParameters() is called before the Init(). The loaded parameters can be used
// in the Init().
//
DtStatus  DtDfMbcFlash_LoadParameters(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMbcFlash* pDf = (DtDfMbcFlash*)pDfBase;

    // List of MbcFlash function parameters
    DtDfParameters  DFMbcFlash_PARS[] =
    {
        // Name,  Value Type,  Value*
        { "DEVICE_TYPE", PROPERTY_VALUE_TYPE_STRING, (void*)&(pDf->m_pDeviceType) },
    };

    // Sanity checks
    DF_MBCFLASH_DEFAULT_PRECONDITIONS(pDf);

    // Set defaults
    pDf->m_pDeviceType = "";
    pDf->m_DeviceId = MBCFLASH_DEVICE_UNDEFINED;
    
    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFMbcFlash_PARS), 
                                                                         DFMbcFlash_PARS);
    if (!DT_SUCCESS(Status))
        return Status;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfMbcFlash_OnEnablePostChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfMbcFlash* pDf = (DtDfMbcFlash*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
    //UInt32 WrData[2];
    // Sanity check
    DF_MBCFLASH_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        Int TempBufSize = 0;

        // Get the FMBC properties
        Int ClockFreqHz;
        Status = DtBcFMBC_GetProperties(pDf->m_pBcFmbc, &pDf->m_FmbcMaxCmdSize,
                                                        &pDf->m_FmbcMaxRespSize,
                                                        &ClockFreqHz);
        // Allocate temporary buffer and add 2 words extra for a write command
        TempBufSize = (pDf->m_FmbcMaxCmdSize > pDf->m_FmbcMaxRespSize) ?
                               (pDf->m_FmbcMaxCmdSize+2)*4 : (pDf->m_FmbcMaxRespSize+2)*4;

        pDf->m_pTempBuffer = (UInt32*)DtMemAllocPool(DtPoolNonPaged, TempBufSize, DF_TAG);
        if (pDf->m_pTempBuffer  == NULL)
        {
            DtDbgOutDf(ERR, MBCFLASH, pDf, "ERROR: failed to allocate buffer");
            return DT_STATUS_OUT_OF_MEMORY;
        }

        //Status = DtBcFMBC_WriteRead(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_GET_CHIPID,
        //                                                  0, NULL, 2, pDf->m_pTempBuffer);
        //WrData[0] = 0x3;
        //Status = DtBcFMBC_WriteRead(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_GET_TEMPERATURE,
        //                                             1, WrData, 2, pDf->m_pTempBuffer);

        //Status = DtBcFMBC_Write(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_QSPI_OPEN, 0, NULL);
        //// Select the qspi flash
        //WrData[0] = 0;
        //Status = DtBcFMBC_Write(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_QSPI_SET_CS, 1, WrData);
        //WrData[0] = 0x0;
        //WrData[1] = 0x400;
        //Status = DtBcFMBC_WriteRead(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_QSPI_READ,
        //                                          2, WrData, 0x400, pDf->m_pTempBuffer);
        //Status = DtBcFMBC_Write(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_QPSI_CLOSE, 0, NULL);

    } else {
        // Free the temporary buffer
        if (pDf->m_pTempBuffer != NULL)
        {
            DtMemFreePool(pDf->m_pTempBuffer, DF_TAG);
            pDf->m_pTempBuffer = NULL;
        }
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_OnEnablePreChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfMbcFlash_OnEnablePreChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfMbcFlash* pDf = (DtDfMbcFlash*)pDfBase;
    // Sanity check
    DF_MBCFLASH_DEFAULT_PRECONDITIONS(pDf);

    if (!Enable)
    {
        // Lock device if necessary
        if (!pDf->m_Locked)
        {
            DtDfMbcFlash_LockDevice(pDf, TRUE);
            pDf->m_Locked = TRUE;
        }
    }

    return DT_STATUS_OK;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtDfMbcFlash_OnCloseFile(DtDf* pDfBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMbcFlash* pDf = (DtDfMbcFlash*)pDfBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    // Sanity checks
    DF_MBCFLASH_DEFAULT_PRECONDITIONS(pDf);

    // Check if the owner closed the file handle
    Status = DtDf_ExclAccessCheck((DtDf*)pDf, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        // Lock device if necessary
        if (!pDf->m_Locked)
        {
            DtDfMbcFlash_LockDevice(pDf, TRUE);
            pDf->m_Locked = TRUE;
        }
    }
    // Use base function to release exclusive access
    return DtDf_OnCloseFile((DtDf*)pDf, pFile);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcFlash_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfMbcFlash_OpenChildren(DtDfMbcFlash*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // List of children supported by the the MbcFlash function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_FMBC, DT_BC_FMBC_NAME,
                         FMBC_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcFmbc), TRUE},
    };

    DF_MBCFLASH_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    // BLKCTRL should be present
    DT_ASSERT(pDf->m_pBcFmbc != NULL);

    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfMbcFlash implementation +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtDf member to DtDfMbcFlash
#define STUB_MBCFLASH   ((DtIoStubDfMbcFlash*)pStub)
#define STUB_DF  ((DtDfMbcFlash*)STUB_MBCFLASH->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubDfMbcFlash_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
static DtStatus  DtIoStubDfMbcFlash_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int* pOutSize);
static DtStatus  DtIoStubDfMbcFlash_OnCmdErase(const DtIoStubDfMbcFlash*,
                                                        const DtIoctlSpiMfCmdEraseInput*);
static DtStatus  DtIoStubDfMbcFlash_OnCmdGetProperties(const DtIoStubDfMbcFlash*,
                                                     DtIoctlSpiMfCmdGetPropertiesOutput*);
static DtStatus  DtIoStubDfMbcFlash_OnCmdLockDevice(const DtIoStubDfMbcFlash*, Int  Cmd);
static DtStatus  DtIoStubDfMbcFlash_OnCmdRead(const DtIoStubDfMbcFlash*,
                                                          const DtIoctlSpiMfCmdReadInput*,
                                                          DtIoctlSpiMfCmdReadOutput*);
static DtStatus  DtIoStubDfMbcFlash_OnCmdWrite(const DtIoStubDfMbcFlash*,
                                                        const DtIoctlSpiMfCmdWriteInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_SPIMF;

static const DtIoctlProperties  IOSTUB_DF_MbcFlash_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubDf_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_SPIMF_CMD(
        DtIoStubDfMbcFlash_OnCmd,
        DtIoStubDfMbcFlash_AppendDynamicSize,
        NULL),
};


// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfMbcFlash - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcFlash_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubDfMbcFlash_Close(DtIoStub* pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfMbcFlash));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcFlash_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubDfMbcFlash* DtIoStubDfMbcFlash_Open(DtDf* pDf)
{
    DtIoStubDfMbcFlash* pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    DF_MBCFLASH_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfMbcFlash, pDf, NULL,
                                                               DtIoStubDfMbcFlash_Close,
                                                               NULL,  // Use default IOCTL
                                                               IOSTUB_DF_MbcFlash_IOCTLS);
    pStub = (DtIoStubDfMbcFlash*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfMbcFlash - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcFlash_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfMbcFlash_AppendDynamicSize(
    const DtIoStub* pStub,
    DtIoStubIoParams* pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSpiMfCmdInput* pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfMbcFlash));
    DT_ASSERT(pIoParams != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_SPIMF_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SpiMfCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_SPIMF_CMD_READ:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlSpiMfCmdReadInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlSpiMfCmdReadOutput));
        // Add dynamic size (i.e. #bytes to read)
        pIoParams->m_OutReqSize += pInData->m_Read.m_NumToRead;
        break;

    case DT_SPIMF_CMD_WRITE:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlSpiMfCmdWriteInput));
        // Add dynamic size (i.e. #bytes to write)
        pIoParams->m_InReqSize += pInData->m_Write.m_NumToWrite;
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcFlash_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfMbcFlash_OnCmd(const DtIoStub* pStub, DtIoStubIoParams* pIoParams,
    Int* pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSpiMfCmdInput* pInData = NULL;
    DtIoctlSpiMfCmdOutput* pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfMbcFlash));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_SPIMF_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, MBCFLASH, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SpiMfCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_SpiMfCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_SPIMF_CMD_ERASE:
        Status = DtIoStubDfMbcFlash_OnCmdErase(STUB_MBCFLASH, &pInData->m_Erase);
        break;
    case DT_SPIMF_CMD_GET_PROPERTIES:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfMbcFlash_OnCmdGetProperties(STUB_MBCFLASH,
            &pOutData->m_GetProperties);
        break;
    case DT_SPIMF_CMD_LOCK_DEVICE:
        Status = DtIoStubDfMbcFlash_OnCmdLockDevice(STUB_MBCFLASH, pInData->m_Lock.m_CmdEx);
        break;
    case DT_SPIMF_CMD_READ:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfMbcFlash_OnCmdRead(STUB_MBCFLASH,
                                                     &pInData->m_Read, &pOutData->m_Read);
        break;
    case DT_SPIMF_CMD_WRITE:
        Status = DtIoStubDfMbcFlash_OnCmdWrite(STUB_MBCFLASH, &pInData->m_Write);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcFlash_OnCmdErase -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfMbcFlash_OnCmdErase(
    const DtIoStubDfMbcFlash* pStub,
    const DtIoctlSpiMfCmdEraseInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfMbcFlash));
    DT_ASSERT(pInData != NULL);

    return DtDfMbcFlash_Erase(STUB_DF, pInData->m_StartAddress, pInData->m_NumBytes);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcFlash_OnCmdGetProperties -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfMbcFlash_OnCmdGetProperties(
    const DtIoStubDfMbcFlash* pStub,
    DtIoctlSpiMfCmdGetPropertiesOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfMbcFlash));
    DT_ASSERT(pOutData != NULL);

    pOutData->m_MemoryId = STUB_DF->m_DeviceId;
    pOutData->m_MemorySize = STUB_DF->m_PropMemorySize;
    pOutData->m_PageSize = STUB_DF->m_PropPageSize;
    pOutData->m_SectorSize = STUB_DF->m_PropSectorSize;
    pOutData->m_SpiClockRate = -1;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcFlash_OnCmdLockDevice -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfMbcFlash_OnCmdLockDevice(
    const DtIoStubDfMbcFlash* pStub,
    Int  Cmd)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfMbcFlash));
    DT_ASSERT(Cmd==DT_SPIMF_CMDEX_LOCK|| Cmd==DT_SPIMF_CMDEX_UNLOCK);

    return DtDfMbcFlash_LockDevice(STUB_DF, (Cmd==DT_SPIMF_CMDEX_LOCK));
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcFlash_OnCmdRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfMbcFlash_OnCmdRead(
    const DtIoStubDfMbcFlash* pStub,
    const DtIoctlSpiMfCmdReadInput* pInData,
    DtIoctlSpiMfCmdReadOutput* pOutData)
{

    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfMbcFlash));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    // Assume the worst
    pOutData->m_NumBytes = 0;
    Status = DtDfMbcFlash_Read(STUB_DF, pInData->m_StartAddress, pInData->m_NumToRead,
        pOutData->m_Buf);
    // If succesful, set number of bytes read
    if (DT_SUCCESS(Status))
        pOutData->m_NumBytes = pInData->m_NumToRead;
    return  Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcFlash_OnCmdWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfMbcFlash_OnCmdWrite(
    const DtIoStubDfMbcFlash* pStub,
    const DtIoctlSpiMfCmdWriteInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfMbcFlash));
    DT_ASSERT(pInData != NULL);

    return DtDfMbcFlash_Write(STUB_DF, pInData->m_StartAddress, pInData->m_NumToWrite,
                                                                          pInData->m_Buf);
}