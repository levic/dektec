//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Vpd.c *#*#*#*#*#*#*#*#*#*# (C) 2011-2012 DekTec
//
// Dtu driver - Vital Product Data (VPD) read/write routines.
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
#include <DtuIncludes.h>


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct
{
    Bool  m_IsDeleted;
    UInt8  m_KeywordLen;
    const char*  m_pKeyword;
    UInt16  m_ItemSize;
    const char*  m_pItem;
    const UInt8*  m_pNext;
} tVpdItemPars;

typedef struct
{
    const char *  m_Name;           // Name, 0 terminated string
    UInt32  m_Length;               // Length of this resource (0 if resource doesn't exist)
    UInt32  m_Offset;               // Offset in EEPROM/FLASH for this resource
    UInt8*  m_pStart;               // Pointer to first byte in cache of this resource 
    UInt8*  m_pEnd;                 // Pointer to last byte in cache of this resource
    UInt  m_LastKeywordLen;         // Length of last keyword
    const char*  m_LastKeyword;     // Last keyword used for this resource (RV or RW)
    UInt8  m_MinLastItemSize;       // Minimal size of last item (RV or RW)
} tVpdResourcePars;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Fixed Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// Internal VPD item m_ItemTag fields
#define  VPDINT_ITEMTAG_KEYLEN_MASK  0xF0
#define  VPDINT_ITEMTAG_KEYLEN_POS   4
#define  VPDINT_ITEMTAG_STATE_MASK   0x0F
#define  VPDINT_ITEMTAG_STATE_8BDEL  0x00 // Deleted UInt8 item_size based structure
#define  VPDINT_ITEMTAG_STATE_16BDEL 0x01 // Deleted UInt16 item_size based structure
#define  VPDINT_ITEMTAG_STATE_16B    0x0D // UInt16 item_size based structure
#define  VPDINT_ITEMTAG_STATE_8B     0x0E // UInt8 item_size based structure
#define  VPDINT_ITEMTAG_STATE_ERASED 0x0F // Erased memory

// VPD cache memory tag
#define  DTU_VPD_CACHE_MEM_TAG      0x43445056  // 'CDPV'

// VPD ID start indicator
#define  VPD_IDSTRING_TAG           0x82

#define  VPD_R_TAG                  0x90
#define  VPD_W_TAG                  0x91
#define  VPD_END_TAG                0x78
#define  VPD_INTSTART_TAG           0x76

// VPD section layout
#define  VPD_SIZE_DTU2               512
#define  VPD_END_DTU2                (VPD_SIZE_DTU2 - 1)
#define  VPD_RW_START_DTU2           0x100
#define  VPD_RW_LENGTH_DTU2          (VPD_END_DTU2 - VPD_RW_START_DTU2)

#define  VPD_SIZE_DTU3               4096
#define  VPD_END_DTU3                (VPD_SIZE_DTU3 - 1)
#define  VPD_RW_START_DTU3           0x200
#define  VPD_RW_LENGTH_DTU3          (VPD_END_DTU3 - VPD_RW_START_DTU3)

// Start and end of VPD sections
// Read-only section directly follows VPD-ID
#define  VPD_ID_START_DTU2            0x58
#define  VPD_ID_START_DTU3            0x20

// Macro to check for valid keyword chars
#define  VALID_VPD_KEYW_CHAR(c)     (((c)>='0' && (c)<='9') || ((c)>='A' && (c)<='Z') || \
                                                                                (c)=='_')

//-.-.-.-.-.-.-.-.-.-.-.- Private functions: forward declarations -.-.-.-.-.-.-.-.-.-.-.-.
static Bool  DtuVpdIntItemCheck(const UInt8* p, const UInt8* pResourceStart,
                                   const UInt8* pResourceEnd, tVpdItemPars* pVpdItemPars);
static DtStatus  DtuVpdIntItemDelete(DtuDeviceData* pDvcData, UInt8 SectionType,
                                                   UInt KeywordLen, const char* pKeyword);
static DtStatus  DtuVpdIntItemDeleteSub(const tVpdResourcePars* pVpdResourcePars, 
                                                   UInt KeywordLen, const char* pKeyword);
static DtStatus  DtuVpdIntItemRead(DtuDeviceData* pDvcData, UInt8 SectionType, 
                   UInt KeywordLen, const char* pKeyword, char*  pVpdItem, UInt* pLength);
static const UInt8* DtuVpdIntItemSearch(UInt KeywordLen, const char* pKeyword, 
                                   const UInt8* pResourceStart, const UInt8* pResourceEnd, 
                                   tVpdItemPars* pVpdItemPars);
static DtStatus  DtuVpdIntItemWrite(DtuDeviceData* pDvcData, UInt8 SectionType,
                 UInt KeywordLen, const char* pKeyword, const char* pItem, UInt ItemSize);
static DtStatus  DtuVpdIntItemWriteLastItem(const tVpdResourcePars* pVpdResourcePars);
static UInt8*  DtuVpdIntItemWriteVpdItemD(UInt8* p, UInt KeywordLen, const char* pKeyword, 
                                                        const char* pItem, UInt ItemSize);
static Bool  DtuVpdIntResourceParameters(const DtuDeviceData* pDvcData, UInt8 SectionType, 
                                                      tVpdResourcePars* pVpdResourcePars);
static const UInt8*  DtuVpdIntResourceSearchFree(
                                                const tVpdResourcePars* pVpdResourcePars);
static void  DtuVpdInitIdOffset(DtuDeviceData* pDvcData);
static DtStatus  DtuVpdUpdateCache(DtuDeviceData* pDvcData);
static DtStatus DtuVpdUpdateEepromFromCache(DtuDeviceData* pDvcData, UInt StartAddr,
                                                                             UInt Length);
static DtStatus  DtuVpdReadFromCache(DtuDeviceData* pDvcData, UInt8* pBuf,
                                                             UInt StartAddr, UInt Length);
static DtStatus  DtuVpdFindEndOfRoSection(DtuDeviceData* pDvcData, UInt8** pItem);
static DtStatus  DtuVpdCreateRV(DtuDeviceData* pDvcData);
static DtStatus  DtuVpdDeleteItemRo(DtuDeviceData* pDvcData, UInt KeywordLen, 
                                                                    const char* pKeyword);
static DtStatus  DtuVpdDeleteItemRw(DtuDeviceData* pDvcData, UInt KeywordLen, 
                                                                    const char* pKeyword);
static DtStatus  DtuVpdWriteId(DtuDeviceData* pDvcData, char* pVpdId, UInt IdLength);
static DtStatus  DtuVpdWriteItemRo(DtuDeviceData* pDvcData, UInt KeywordLen,
                                   const char* pKeyword, char* pVpdItem, UInt ItemLength);
static DtStatus  DtuVpdWriteItemRw(DtuDeviceData* pDvcData, UInt KeywordLen, 
                                   const char* pKeyword, char* pVpdItem, UInt ItemLength);
static DtStatus  DtuVpdWrite(DtuDeviceData* pDvcData, UInt8* pBuf, UInt StartAddr,
                                                                             UInt Length);
static DtStatus  DtuVpdReadItem(DtuDeviceData* pDvcData, Int SectionType, 
                       UInt KeywordLen, const char* pKeyWord, UInt8* pBuf, UInt* pBufLen);
static DtStatus  DtuVpdDeleteItem(DtuDeviceData* pDvcData, Int SectionType,
                                                   UInt KeywordLen, const char* pKeyWord); 
static DtStatus DtuVpdWriteItem(DtuDeviceData* pDvcData, Int SectionType, 
                UInt KeywordLen, const char* pKeyWord, char* pBuf, Int BufLen, Int Flags);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuVpdInit(DtuDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    DtVpd*  pVpd = &pDvcData->m_Vpd;

    // Initialize properties
    pVpd->m_EepromIoItf = DtPropertiesGetInt(pPropData, "VPD_IO_ITF", -1);
    pVpd->m_EepromVpdSize = DtPropertiesGetUInt(pPropData, "VPD_SIZE", -1);
    pVpd->m_EepromPageSize = DtPropertiesGetUInt(pPropData, "EEPROM_PAGE_SIZE", -1);
    pVpd->m_EepromSize = DtPropertiesGetUInt(pPropData, "EEPROM_SIZE", -1);
    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
    {
        pVpd->m_VpdEnd = VPD_END_DTU3;
    } else {
        pVpd->m_VpdEnd = VPD_END_DTU2;
    }

    // Check if no property error occurred
    Status = DtuPropertiesReportDriverErrors(pDvcData); 
    if (!DT_SUCCESS(Status))
        return Status;

    if (pVpd->m_EepromIoItf != VPD_EEPROM_IO_NOT_SUPP)
    {
        // VPD-size must be smaller or equal to full EEPROM size
        if ((pVpd->m_EepromVpdSize==0 || pVpd->m_EepromVpdSize>pVpd->m_EepromSize))
        {
            DtDbgOut(ERR, VPD, "ERROR: VPD-size (%d) is illegal (EEPROM size = %d)",
                                                                    pVpd->m_EepromVpdSize,
                                                                    pVpd->m_EepromSize);
            return DT_STATUS_CONFIG_ERROR;
        }
        // Page size must be valid
        if (pVpd->m_EepromPageSize==0 || (pVpd->m_EepromSize%pVpd->m_EepromPageSize)!=0)
        {
            DtDbgOut(ERR, VPD, "ERROR: Page-size (%d) is invalid  (EEPROM size = %d)",
                                                                   pVpd->m_EepromPageSize,
                                                                   pVpd->m_EepromSize);
            return DT_STATUS_CONFIG_ERROR;
        }

        // Check vpd size aligns on page boundaries
        if ((pVpd->m_EepromVpdSize%pVpd->m_EepromPageSize) != 0)
        {
            DtDbgOut(ERR, VPD, "ERROR: VPD-size (%d) no page (%d) aligned",
                                                                  pVpd->m_EepromVpdSize,
                                                                  pVpd->m_EepromPageSize);
            return DT_STATUS_CONFIG_ERROR;
        }
    }

    // Initialize EEPROM module
    DtuEepromInit(pDvcData);

    // Initialize VPD IOCTL mutex
    DtFastMutexInit(&pVpd->m_IoctlMutex);

    // Allocate VPD cache
    pVpd->m_pCache = (UInt8*)DtMemAllocPool(DtPoolNonPaged, pVpd->m_EepromVpdSize,
                                                                   DTU_VPD_CACHE_MEM_TAG);
    if (pVpd->m_pCache != NULL)
        // Initial load of the cache
        Status = DtuVpdUpdateCache(pDvcData);
    else
        Status = DT_STATUS_OUT_OF_MEMORY;

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdCleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtuVpdCleanup(DtuDeviceData* pDvcData)
{
    DtVpd*  pVpd = &pDvcData->m_Vpd;

    // Free cache
    if (pVpd->m_pCache != NULL)
        DtMemFreePool(pVpd->m_pCache, DTU_VPD_CACHE_MEM_TAG);

    // Cleanup Eeprom module
    DtuEepromCleanup(pDvcData);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdReadItemRo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read VPD item into a caller-allocated string buffer from the read-only section.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
DtStatus  DtuVpdReadItemRo(
    DtuDeviceData*  pDvcData,   // Device extension
    UInt KeywordLen,            // Length of Keyword
    const char*  pKeyword,      // Name of VPD item to be read
    char*  pVpdItem,            // String buffer for storing VPD item
    UInt*  pLength)             // In: Length of pVpdItem buffer
                                // Out: Length of string
{
    DtStatus  Status;
    UInt  i, Length, RoLength;
    char*  pFrom;               // Helper character pointer
    char*  pTo;                 // Helper character pointer
    UInt8*  pVpd;               // Byte pointer in VPD data
    UInt8*  pVpdRoEnd;          // Pointer one after RO section
    UInt8*  pResource;          // Resource to be read
    Int  VpdRwStart;
    DtVpd*  pVpdData = &pDvcData->m_Vpd;

    // Parameter check
    if (*pLength == 0)
        DtDbgOut(ERR, VPD, "Invalid length: 0");
    
    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
        VpdRwStart = VPD_RW_START_DTU3;
    else
        VpdRwStart = VPD_RW_START_DTU2;

    // Compute a number of helper variables
    pVpdRoEnd = &pVpdData->m_pCache[VpdRwStart];

    // Initialisation
    pTo = pVpdItem;             // For later copying to VPD item
    DtMemZero(pVpdItem, *pLength);

    Status = DtVpdFindStartOfRoSection(pVpdData, &pVpd, VpdRwStart);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Length of RO section
    RoLength = (UInt)(pVpdRoEnd - pVpd);

    // Check whether we can find resource in RO section
    if (DtVpdFindResource(KeywordLen, pKeyword, pVpd, RoLength, &pResource))
    {
        pFrom = pResource + 3;
        Length = pResource[2];
        // Protect against invalid content of EEPROM or too small buffer
        if (Length > *pLength)
            Length = *pLength;
        for (i=0; i<Length; i++)
            *pTo++ = *pFrom++;
        *pLength = Length;
        DtDbgOut(MAX, VPD, "Found VPD item '%.*s' in RO section, value='%.*s'", 
                                                                  KeywordLen, pKeyword,
                                                                  Length, pVpdItem);
        return DT_STATUS_OK;
    }

    // Not found
    if (pKeyword[0] == 'V')
        DtDbgOut(MAX, VPD, "Can't find VPD item '%.*s'", KeywordLen, pKeyword);
    else
        DtDbgOut(MIN, VPD, "Can't find VPD item '%.*s'", KeywordLen, pKeyword);

    return DT_STATUS_NOT_FOUND;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdReadItemRw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read VPD item into a caller-allocated string buffer from the read-write section.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
DtStatus  DtuVpdReadItemRw(
    DtuDeviceData*  pDvcData,   // Device extension
    UInt KeywordLen,            // Length of Keyword
    const char*  pKeyword,      // Name of VPD item to be read
    char*  pVpdItem,            // String buffer for storing VPD item
    UInt*  pLength)             // In: Length of pVpdItem buffer
                                // Out: Length of string
{
    UInt  i, Length;
    char*  pFrom;               // Helper character pointer
    char*  pTo;                 // Helper character pointer
    UInt8*  pVpd;               // Byte pointer in VPD data
    UInt8*  pResource;          // Resource to be read
    Int  VpdRwStart;
    Int  VpdRwLength;
    DtVpd*  pVpdData = &pDvcData->m_Vpd;

    // Add parameter checks here
    if (*pLength == 0)
        DtDbgOut(ERR, VPD, "Invalid length: 0");

    // Initialisation
    pTo = pVpdItem;             // For later copying to VPD item
    DtMemZero(pVpdItem, *pLength);
    
    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
    {
        VpdRwStart = VPD_RW_START_DTU3;
        VpdRwLength = VPD_RW_LENGTH_DTU3;
    } else {
        VpdRwStart = VPD_RW_START_DTU2;
        VpdRwLength = VPD_RW_LENGTH_DTU2;
    }

    // Check whether we can find resource in RW section
    pVpd = &pVpdData->m_pCache[VpdRwStart];

    if (pVpd[0] != VPD_W_TAG)
    {
        DtDbgOut(ERR, VPD, "ERROR: VPD-RW section starts with 0x%02x i.s.o. 0x91", 
                                                                                 pVpd[0]);
        return DT_STATUS_FAIL;
    }

    if (DtVpdFindResource(KeywordLen, pKeyword, pVpd, VpdRwLength, &pResource))
    {
        pFrom = pResource + 3;
        Length = pResource[2];
        // Protect against invalid content of EEPROM or too small buffer
        if (Length > *pLength)
            Length = *pLength;
        for (i=0; i<Length; i++)
            *pTo++ = *pFrom++;
        *pLength = Length;
        DtDbgOut(MAX, VPD, "Found VPD item '%.*s' in RW section, value='%.*s'", 
                                                                    KeywordLen, pKeyword,
                                                                    Length, pVpdItem);
        return DT_STATUS_OK;
    }

    // Not found
    if (pKeyword[0] == 'V')
        DtDbgOut(MAX, VPD, "Can't find VPD item '%.*s'", KeywordLen, pKeyword);
    else
        DtDbgOut(MIN, VPD, "Can't find VPD item '%.*s'", KeywordLen, pKeyword);

    return DT_STATUS_NOT_FOUND;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Implementation of DTU_IOCTL_VPD_DATA
//
DtStatus  DtuVpdIoctl(DtuDeviceData* pDvcData, DtIoctlObject* pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  ItemLen;
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    DtuIoctlVpdCmdInput*  pVpdCmdInput = (DtuIoctlVpdCmdInput*)pIoctl->m_pInputBuffer;
    DtuIoctlVpdCmdOutput*  pVpdCmdOutput = (DtuIoctlVpdCmdOutput*)pIoctl->m_pOutputBuffer;
    
    InReqSize = OFFSETOF(DtuIoctlVpdCmdInput, m_Data);
    OutReqSize = 0;

    // Check if we can read m_Cmd
    if (pIoctl->m_InputBufferSize < OFFSETOF(DtuIoctlVpdCmdInput, m_Data))
        return DT_STATUS_INVALID_PARAMETER;

    switch (pVpdCmdInput->m_Cmd)
    {
    case DTU_VPD_CMD_ITEM_READ:
        pCmdStr = "DTU_VPD_CMD_ITEM_READ";
        InReqSize += sizeof(DtuIoctlVpdCmdReadItemInput);
        // Check for valid input buffer before reading it
        OutReqSize += sizeof(UInt);
        if (pIoctl->m_InputBufferSize >= InReqSize)
            OutReqSize += pVpdCmdInput->m_Data.m_ReadItem.m_OutBufLen;
        break;

    case DTU_VPD_CMD_ITEM_WRITE:
        pCmdStr = "DTU_VPD_CMD_ITEM_WRITE";
        InReqSize += OFFSETOF(DtuIoctlVpdCmdWriteItemInput, m_Buf); // Exclude m_Buf
        // Check for valid input buffer before reading it
        if (pIoctl->m_InputBufferSize >= InReqSize)
            InReqSize += pVpdCmdInput->m_Data.m_WriteItem.m_BufLen;
        // We expect no output buffer
        OutReqSize = 0;
        break;

    case DTU_VPD_CMD_ITEM_DELETE:
        pCmdStr = "DTU_VPD_CMD_ITEM_DELETE";
        InReqSize += sizeof(DtuIoctlVpdCmdDeleteItemInput);
        // We expect no output buffer
        OutReqSize = 0;
        break;

    case DTU_VPD_CMD_RAW_READ:
        pCmdStr = "DTU_VPD_CMD_RAW_READ";
        InReqSize += sizeof(DtuIoctlVpdCmdReadRawInput); 
        // Check for valid input buffer before reading it
        OutReqSize += sizeof(UInt);
        if (pIoctl->m_InputBufferSize >= InReqSize)
            OutReqSize += pVpdCmdInput->m_Data.m_ReadRaw.m_OutBufLen;
        break;

    case DTU_VPD_CMD_RAW_WRITE:
        pCmdStr = "DTU_VPD_CMD_RAW_WRITE";
        InReqSize += OFFSETOF(DtuIoctlVpdCmdWriteRawInput, m_Buf);
        // Check for valid input buffer before reading it
        if (pIoctl->m_InputBufferSize >= InReqSize)
            InReqSize += pVpdCmdInput->m_Data.m_WriteRaw.m_BufLen;
        // We expect no output buffer
        OutReqSize = 0;
        break;

    default:
        pCmdStr = "??UNKNOWN VPDCMD CODE??";
        InReqSize = 0;
        OutReqSize = 0;
    }

    // Check buffer sizes
    if (pIoctl->m_InputBufferSize < InReqSize)
    {
        DtDbgOut(ERR, VPD, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                    pIoctl->m_InputBufferSize, InReqSize);
        return DT_STATUS_INVALID_PARAMETER;
    }
    if (pIoctl->m_OutputBufferSize < OutReqSize)
    {
        DtDbgOut(ERR, VPD, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
        return DT_STATUS_INVALID_PARAMETER;
    }

    DtDbgOut(MAX, VPD, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr, 
            pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    // Acquire VPD Mutex
    DtFastMutexAcquire(&pDvcData->m_Vpd.m_IoctlMutex);
    
    switch (pVpdCmdInput->m_Cmd)
    {
    case DTU_VPD_CMD_ITEM_DELETE:
        Status = DtuVpdDeleteItem(pDvcData,
                                          pVpdCmdInput->m_Data.m_DeleteItem.m_SectionType,
                                          pVpdCmdInput->m_Data.m_DeleteItem.m_KeywordLen,
                                          pVpdCmdInput->m_Data.m_DeleteItem.m_Keyword);
        break;

    case DTU_VPD_CMD_ITEM_READ:
        // Assert NUL terminated keyword
        ItemLen = pVpdCmdInput->m_Data.m_ReadItem.m_OutBufLen;
        Status = DtuVpdReadItem(pDvcData, pVpdCmdInput->m_Data.m_ReadItem.m_SectionType,
                                        pVpdCmdInput->m_Data.m_DeleteItem.m_KeywordLen,
                                        pVpdCmdInput->m_Data.m_ReadItem.m_Keyword,
                                        pVpdCmdOutput->m_Data.m_ReadItem.m_Buf, &ItemLen);
        if (DT_SUCCESS(Status))
            pVpdCmdOutput->m_Data.m_ReadItem.m_Length = ItemLen;
        else
            pVpdCmdOutput->m_Data.m_ReadItem.m_Length = 0;
        break;

    case DTU_VPD_CMD_ITEM_WRITE:
        Status = DtuVpdWriteItem(pDvcData, pVpdCmdInput->m_Data.m_WriteItem.m_SectionType,
                                           pVpdCmdInput->m_Data.m_DeleteItem.m_KeywordLen,
                                           pVpdCmdInput->m_Data.m_WriteItem.m_Keyword,
                                           pVpdCmdInput->m_Data.m_WriteItem.m_Buf,
                                           pVpdCmdInput->m_Data.m_WriteItem.m_BufLen,
                                           pVpdCmdInput->m_Data.m_WriteItem.m_Flags);
        break;

    case DTU_VPD_CMD_RAW_READ:
        Status = DtuVpdReadFromCache(pDvcData, pVpdCmdOutput->m_Data.m_ReadRaw.m_Buf,
                                             pVpdCmdInput->m_Data.m_ReadRaw.m_StartOffset,
                                             pVpdCmdInput->m_Data.m_ReadRaw.m_OutBufLen);
        if (DT_SUCCESS(Status))
            pVpdCmdOutput->m_Data.m_ReadRaw.m_Length =
                                               pVpdCmdInput->m_Data.m_ReadRaw.m_OutBufLen;
        else
            pVpdCmdOutput->m_Data.m_ReadRaw.m_Length = 0;
        break;

    case DTU_VPD_CMD_RAW_WRITE:
        Status = DtuVpdWrite(pDvcData, pVpdCmdInput->m_Data.m_WriteRaw.m_Buf,
                                            pVpdCmdInput->m_Data.m_WriteRaw.m_StartOffset,
                                            pVpdCmdInput->m_Data.m_WriteRaw.m_BufLen);
        break;

    default:
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    // Release VPD Mutex
    DtFastMutexRelease(&pDvcData->m_Vpd.m_IoctlMutex);

    // If we failed, no data has te be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED) 
            DtDbgOut(MIN, VPD, "VpdCmd=0x%x: NOT SUPPORTED", pVpdCmdInput->m_Cmd);
        else if (pVpdCmdInput->m_Cmd == DTU_VPD_CMD_ITEM_READ)
            DtDbgOut(MAX, VPD, "%s: ERROR %xh", pCmdStr, Status);
        else
            DtDbgOut(MIN, VPD, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdIntItemCheck -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Decodes and checks whether p points to a correctly formatted internal VPD item in
// resource between pResourceStart and pResourceEnd. Returns decodeded parameters in
// pVpdItemPars.
//
static Bool  DtuVpdIntItemCheck(
    const UInt8*  p,                // Pointer to item to be checked for validity
    const UInt8*  pResourceStart,   // First byte of resource in which keyword is located
    const UInt8*  pResourceEnd,     // Last byte of resource in which keyword is located
    tVpdItemPars*  pVpdItemPars)    // Decoded item parameters
{
    UInt8  KeywordLen;
    UInt8  ItemSizeSize;
    UInt16  ItemSize;
    UInt16  CheckSum;

    // Sanity check
    if (pResourceStart==NULL || p<pResourceStart || p>pResourceEnd)
        return FALSE;

    // item_state
    switch (*p & VPDINT_ITEMTAG_STATE_MASK)           
    {
    case VPDINT_ITEMTAG_STATE_8BDEL:
        pVpdItemPars->m_IsDeleted = TRUE;
        ItemSizeSize = 1;
        break;
    case VPDINT_ITEMTAG_STATE_8B:
        pVpdItemPars->m_IsDeleted = FALSE;
        ItemSizeSize = 1;
        break;
    case VPDINT_ITEMTAG_STATE_16BDEL:
        pVpdItemPars->m_IsDeleted = TRUE;
        ItemSizeSize = 2;
        break;
    case VPDINT_ITEMTAG_STATE_16B:
        pVpdItemPars->m_IsDeleted = FALSE;
        ItemSizeSize = 2;
        break;
    default:
        return FALSE;
    }

    // keyword_size
    KeywordLen = (*p&VPDINT_ITEMTAG_KEYLEN_MASK) >> VPDINT_ITEMTAG_KEYLEN_POS; 
    if (KeywordLen == 0)
        return FALSE;
    CheckSum = *p++&VPDINT_ITEMTAG_KEYLEN_MASK;

    // Check if item_size is within the resoruce
    if (p+KeywordLen+ItemSizeSize > pResourceEnd)
        return FALSE;
    
    pVpdItemPars->m_KeywordLen = KeywordLen;

    // keyword
    pVpdItemPars->m_pKeyword = p;
    while (KeywordLen--)
    {
        if (!VALID_VPD_KEYW_CHAR(*p))
            return FALSE;
        CheckSum += *p++;
    }

    // item_size
    if (ItemSizeSize > 1)
    {
        ItemSize = *p <<8;
        CheckSum += *p++;
    } else
        ItemSize = 0;
    ItemSize |= *p;
    CheckSum += *p++;
    pVpdItemPars->m_ItemSize = ItemSize;

    // item
    pVpdItemPars->m_pItem = p;
    if (p+ItemSize+1 > pResourceEnd) // Check if check_sum is within resoruce
        return FALSE;
    while (ItemSize--)
        CheckSum += *p++;

    // check_sum
    CheckSum += (*p++) << 8;
    CheckSum += *p++;
    pVpdItemPars->m_pNext = p;
        
    return CheckSum==0;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdIntItemDelete -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Delete all VPD items with keyword pKeyword from the internal resource. Does update the
// last keyword ("RV" or "RW").
//
static DtStatus  DtuVpdIntItemDelete(
    DtuDeviceData*  pDvcData,       // Device extension
    UInt8  SectionType,             // Internal resource selector
    UInt KeywordLen,                // Length of Keyword
    const char*  pKeyword)          // Name of VPD item to be deleted
{
    DtVpd*  pVpd;                   // Pointer to VPD parameters in DtuDeviceData
    tVpdResourcePars  VpdResourcePars; // Parameters of selected resource
    DtStatus  StatusDelete;         // Returned status of keyword deletion
    DtStatus  StatusWriteLastItem;  // Returned status of last item write

    // Sanity check
    if (pKeyword==NULL || KeywordLen<1 || KeywordLen>DTU_VPD_MAX_KEY_LENGTH)
    {
        DtDbgOut(MIN, VPD, "Illegal keyword (size: %d)", KeywordLen);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Initialization
    pVpd = &pDvcData->m_Vpd; 
    if (!DtuVpdIntResourceParameters(pDvcData, SectionType, &VpdResourcePars))
    {
        DtDbgOut(MIN, VPD, "Resource %s doesn't exist", VpdResourcePars.m_Name);
        return DT_STATUS_NOT_FOUND;
    }
    
    // Delete last item if it exists (ignore status)
    DtuVpdIntItemDeleteSub(&VpdResourcePars, VpdResourcePars.m_LastKeywordLen, 
                                                           VpdResourcePars.m_LastKeyword);

    // Delete item
    StatusDelete = DtuVpdIntItemDeleteSub(&VpdResourcePars, KeywordLen, pKeyword);
    if (StatusDelete != DT_STATUS_OK)
        DtDbgOut(MIN, VPD, "Can't find item '%.*s' in %s resource", KeywordLen, pKeyword, 
                                                                VpdResourcePars.m_Name);

    // Update last item
    StatusWriteLastItem = DtuVpdIntItemWriteLastItem(&VpdResourcePars);

    // Report status
    if (DT_SUCCESS(StatusDelete))
        return StatusWriteLastItem;
    else
        return StatusDelete;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdIntItemDeleteSub -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Delete all VPD items with keyword pKeyword from the internal resource. Does not update
// the last keyword ("RV" or "RW").
//
static DtStatus  DtuVpdIntItemDeleteSub(
    const tVpdResourcePars*  pVpdResourcePars, // Parameters of selected resource
    UInt KeywordLen,            // Length of Keyword
    const char*  pKeyword)          // Name of VPD item to be deleted
{
    UInt8*  p;                      // Pointer to VPD data in cache
    tVpdItemPars  VpdItemPars;      // Parameters of VPD item to be deleted
    size_t  n;                       // Size of memory to be moved or erased

    // Check whether we can find the item in the resource
    p = (UInt8*) DtuVpdIntItemSearch(KeywordLen, pKeyword, pVpdResourcePars->m_pStart,
                                                  pVpdResourcePars->m_pEnd, &VpdItemPars);
    if (p == NULL)
        return DT_STATUS_NOT_FOUND;

    // Delete resource in a loop, so that multiple instances are deleted too.
    do {
        // Copy all memory after this item, which is to be deleted, to this item
        n = pVpdResourcePars->m_pEnd - VpdItemPars.m_pNext + 1;
        if (n>0) 
            DtMemMove(p, (void*)VpdItemPars.m_pNext, n);
        else n=0;
        p += n;

        // Erase remaining memory in resource 
        n = pVpdResourcePars->m_pEnd - p + 1;
        if (n>0)
            DtMemZero(p, n);

        // Find next item
        p = (UInt8*) DtuVpdIntItemSearch(KeywordLen, pKeyword, pVpdResourcePars->m_pStart, 
                                                  pVpdResourcePars->m_pEnd, &VpdItemPars);
    } while (p != NULL);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdIntItemRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read VPD item into a caller-allocated string buffer from the selecte internal resource.
//
static DtStatus  DtuVpdIntItemRead(
    DtuDeviceData*  pDvcData,       // Device extension
    UInt8  SectionType,             // Internal resource selector
    UInt KeywordLen,            // Length of Keyword
    const char*  pKeyword,          // Name of VPD item to be read
    char*  pVpdItem,                // String buffer for storing VPD item
    UInt*  pLength)                 // In: Length of pVpdItem buffer
                                    // Out: Length of string
{
    tVpdResourcePars VpdResourcePars; // Parameters of the resource
    const char*  pFrom;             // Helper character pointer to copy from
    char*  pTo;                     // Helper character pointer to copy to
    const UInt8*  pItem;            // Item to be read
    tVpdItemPars  VpdItemPars;      // Parameters of the found item
    UInt  Length;                   // Size to copy
    UInt  i;                        // Iterator

    // Sanity check
    if (pKeyword==NULL || KeywordLen<1 || KeywordLen>DTU_VPD_MAX_KEY_LENGTH)
    {
        DtDbgOut(MIN, VPD, "Illegal keyword (size: %d)", KeywordLen);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Initialization
    if (!DtuVpdIntResourceParameters(pDvcData, SectionType, &VpdResourcePars))
    {
        DtDbgOut(MIN, VPD, "Resource %s doesn't exist", VpdResourcePars.m_Name);
        return DT_STATUS_NOT_FOUND;
    }
    DtMemZero(pVpdItem, *pLength);
    
    // Check whether we can find the item in the resource
    pItem = DtuVpdIntItemSearch(KeywordLen, pKeyword, VpdResourcePars.m_pStart, 
                                                    VpdResourcePars.m_pEnd, &VpdItemPars);
    if (pItem == NULL)
    {
        // Not found
        DtDbgOut(MIN, VPD, "Can't find item '%.*s' in %s resource", KeywordLen, pKeyword, 
                                                                  VpdResourcePars.m_Name);
        return DT_STATUS_NOT_FOUND;
    }

    // Copy item to buffer
    pFrom = VpdItemPars.m_pItem;
    pTo = pVpdItem;
    Length = *pLength;
    *pLength = VpdItemPars.m_ItemSize;
    if (Length > *pLength)
        Length = *pLength;
    for (i=0; i<Length; i++)
        *pTo++ = *pFrom++;
    DtDbgOut(MAX, VPD, "Found item '%.*s' in %s resource, value='%.*s'",
                          KeywordLen, pKeyword, VpdResourcePars.m_Name, Length, pVpdItem);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdIntItemSearch -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Keyword-based item search in given internal VPD resource. Return pointer to item if
// found, NULL otherwise. Returns decodeded item parameters in
// pVpdItemPars.
//
static const UInt8*  DtuVpdIntItemSearch(
    UInt KeywordLen,                // Length of Keyword
    const char*  pKeyword,          // Keyword to be found
    const UInt8*  pResourceStart,   // Start of resource (starts with VPD tag)
    const UInt8*  pResourceEnd,     // End of resource
    tVpdItemPars*  pVpdItemPars)    // Decodeded item parameters
{
    const UInt8*  pItem;            // Running item pointer
    
    // Sanity check
    if (pKeyword==NULL || KeywordLen==0 || pResourceStart==NULL || 
                                        pResourceEnd<pResourceStart || pVpdItemPars==NULL)
        return NULL;

    // Initialize
    pItem = pResourceStart + 3;
    
    // Loop through resources
    while (DtuVpdIntItemCheck(pItem, pResourceStart, pResourceEnd, pVpdItemPars))
    {
        if (
               !(pVpdItemPars->m_IsDeleted)
            && (pVpdItemPars->m_KeywordLen==KeywordLen)
            && (strncmp(pVpdItemPars->m_pKeyword, pKeyword, KeywordLen)==0)
        )
            // Found keyword
            return pItem;
        pItem = pVpdItemPars->m_pNext;
    }

    // No more valid keywords => can't find keyword
    return NULL;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdIntItemWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write a VPD Item into the RO section of the VPD cache. If the Item already exists, 
// the old Item will be erased.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
static DtStatus  DtuVpdIntItemWrite(
    DtuDeviceData*  pDvcData,       // Device extension
    UInt8  SectionType,             // Internal resource selector
    UInt KeywordLen,                // Length of Keyword
    const char*  pKeyword,          // String buffer with keyword
    const char*  pItem,             // String buffer with VPD Item
    UInt  ItemSize)                 // Size of pVpdItem
{
    tVpdResourcePars VpdResourcePars; // Parameters of the resource
    UInt8*  p;                      // Pointer to VPD data
    Int64   n;                      // Size of area in VPD memory

    // Sanity check and initialization
    if (pKeyword==NULL || pItem==NULL)
        return DT_STATUS_INVALID_PARAMETER;
    if (KeywordLen<1 || KeywordLen>DTU_VPD_MAX_KEY_LENGTH)
    {
        DtDbgOut(MIN, VPD, "Illegal keyword size: %d", KeywordLen);
        return DT_STATUS_INVALID_PARAMETER;
    }
    if (ItemSize > 0xFFFF)
    {
        DtDbgOut(MIN, VPD, "Illegal item size: %d", ItemSize);
        return DT_STATUS_INVALID_PARAMETER;
    }
    if (!DtuVpdIntResourceParameters(pDvcData, SectionType, &VpdResourcePars))
    {
        DtDbgOut(MIN, VPD, "Resource %s doesn't exist", VpdResourcePars.m_Name);
        return DT_STATUS_FAIL;
    }

    // Delete last item if it exists
    DtuVpdIntItemDeleteSub(&VpdResourcePars, VpdResourcePars.m_LastKeywordLen, 
                                                           VpdResourcePars.m_LastKeyword);

    // Delete item if it exists
    DtuVpdIntItemDeleteSub(&VpdResourcePars, KeywordLen, pKeyword);
    
    // Search free space in resource
    p = (UInt8*) DtuVpdIntResourceSearchFree(&VpdResourcePars);
    
    // Calculate size of free space
    if (p==NULL)
        n = 0;
    else
        n = VpdResourcePars.m_pEnd - p + 1 - 
                          VpdResourcePars.m_MinLastItemSize; // reserve room for last item

    if ((signed)(KeywordLen+ItemSize+5) > n)
    {
        DtDbgOut(MIN, VPD, "Resource %s has not enough room to store '%.*s' item", 
                                           VpdResourcePars.m_Name,  KeywordLen, pKeyword);
        return DT_STATUS_OUT_OF_RESOURCES;
    }
    
    // Write item
    DtuVpdIntItemWriteVpdItemD(p, KeywordLen, pKeyword, pItem, ItemSize);

    return DtuVpdIntItemWriteLastItem(&VpdResourcePars);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdIntItemWriteLastItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Last item may not be present before calling this procedure
//
static DtStatus  DtuVpdIntItemWriteLastItem(
    const tVpdResourcePars*  pVpdResourcePars) // Parameters of selected resource
{
    UInt KeywordLen;                // Length of Keyword
    const char*  pKeyword;          // Keyword
    UInt8*  pItem;                  // Pointer to item in VPD data cache
    UInt8*  p;                      // Pointer to VPD data cache
    Int64  ItemSize;                // Item size
    Int8  CheckSum;                 // CheckSum of all VPD data up to "RV" check sum byte
    UInt16  ItemCheckSum;           // CheckSum of "RV" item
    UInt8*  pItemCheckSum;          // Pointer to item check sum in VPD data cache

    // Initialize
    pKeyword = pVpdResourcePars->m_LastKeyword;
    KeywordLen = pVpdResourcePars->m_LastKeywordLen;

    // Search free space in resource
    pItem = (UInt8*) DtuVpdIntResourceSearchFree(pVpdResourcePars);

    // Calculate (total) item size for last item
    ItemSize = (pVpdResourcePars->m_pEnd) - pItem + 1;
    if (ItemSize < pVpdResourcePars->m_MinLastItemSize)
    {
        DtDbgOut(MIN, VPD, "Resource %s has not enough room to store '%.*s' item", 
                                           pVpdResourcePars->m_Name,  KeywordLen, pKeyword);
        return DT_STATUS_OUT_OF_RESOURCES;
    }
    ItemSize -= 5; // Subtract structure overhead
    ItemSize -= KeywordLen;

    // Write item
    if (ItemSize > 0xFFFF)
        ItemSize = 0xFFFF;
    pItemCheckSum = DtuVpdIntItemWriteVpdItemD(pItem, KeywordLen, pKeyword, NULL, 
                                                                    (UInt16)ItemSize) - 2;

    // Check if keyword is "RV" (checksum for VPD-R resource)
    if (strncmp(pKeyword, "RV", KeywordLen) == 0)
    {
        // Create checksum
        CheckSum = 0;
        for (p=pVpdResourcePars->m_pStart; p<pItem+5; ++p)
            CheckSum -= *p;
        pItem[5] = CheckSum;

        // Read item check sum
        ItemCheckSum = ((UInt16)(*pItemCheckSum))<<8;
        ItemCheckSum |= *(pItemCheckSum+1);

        // Correct for the changed data in pItem[5]
        ItemCheckSum -= pItem[5];

        // Write corrected item check sum
        *pItemCheckSum++ = ItemCheckSum>>8;
        *pItemCheckSum = ItemCheckSum & 0xFF;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdIntItemWriteVpdItemD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Unconditionally write a vpd_item_D type structure to pItem, filling in all item data 
// and parameters.
//
static UInt8*  DtuVpdIntItemWriteVpdItemD(
    UInt8*  p,                      // Pointer to VPD data for item
    UInt KeywordLen,                // Length of Keyword
    const char*  pKeyword,          // String buffer with keyword
    const char*  pItem,             // String buffer with VPD Item
    UInt  ItemSize)                 // Size of pVpdItem
{
    UInt16  CheckSum;               // CheckSum

    KeywordLen = KeywordLen & (VPDINT_ITEMTAG_KEYLEN_MASK>>VPDINT_ITEMTAG_KEYLEN_POS);
    
    // Write item_state, keyword_size
    *p = VPDINT_ITEMTAG_STATE_16B | (UInt8)(KeywordLen<<VPDINT_ITEMTAG_KEYLEN_POS);
    CheckSum = 0 - (*p++ & ~VPDINT_ITEMTAG_STATE_MASK);

    // Write keyword
    while (KeywordLen--)
    {
        *p = *pKeyword++;
        CheckSum -= *p++;
    }

    // Write item_size
    *p = ItemSize >> 8;
    CheckSum -= *p++;
    *p = ItemSize & 0xFF;
    CheckSum -= *p++;

    if (pItem==NULL) {
        // Write NUL item
        while (ItemSize--)
            *p++ = 0;
    } else {
        // Write item
        while (ItemSize--)
        {
            *p = *pItem++;
            CheckSum -= *p++;
        }
    }

    // Write check_sum
    *p++ = CheckSum >> 8;
    *p++ = CheckSum & 0xFF;
    return p;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdIntResourceParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Writes the internal resource parameters of section type in pVpdResourcePars.
// Returns true if succesful, false otherwise.
//
static Bool DtuVpdIntResourceParameters(
    const DtuDeviceData*  pDvcData, // Device extension
    UInt8  SectionType,             // Selected resource
    tVpdResourcePars*  pVpdResourcePars) // Parameters of selected resource
{
    const DtVpd*  pVpd = &pDvcData->m_Vpd;
    switch (SectionType)
    {
    case DTU_VPD_SECT_RO:
        pVpdResourcePars->m_Name = "internal VPD-R";
        pVpdResourcePars->m_Offset = pVpd->m_IntRoOffset;
        pVpdResourcePars->m_Length = pVpd->m_IntRoLength;
        pVpdResourcePars->m_LastKeywordLen = 2;
        pVpdResourcePars->m_LastKeyword = "RV";
        pVpdResourcePars->m_MinLastItemSize = 8;
        break;
    case DTU_VPD_SECT_RW:
        pVpdResourcePars->m_Name = "internal VPD-W";
        pVpdResourcePars->m_Offset = pVpd->m_IntRwOffset;
        pVpdResourcePars->m_Length = pVpd->m_IntRwLength;
        pVpdResourcePars->m_LastKeywordLen = 2;
        pVpdResourcePars->m_LastKeyword = "RW";
        pVpdResourcePars->m_MinLastItemSize = 7;
        break;
    default:
        pVpdResourcePars->m_Name = "unknown internal";
        pVpdResourcePars->m_Length = 0;
    }
    if (pVpdResourcePars->m_Length == 0)
    {
        pVpdResourcePars->m_pStart = NULL;
        pVpdResourcePars->m_pEnd = NULL;
        return FALSE;
    }
    pVpdResourcePars->m_pStart = &pVpd->m_pCache[pVpdResourcePars->m_Offset];
    pVpdResourcePars->m_pEnd = 
                              pVpdResourcePars->m_pStart + pVpdResourcePars->m_Length + 2;
    return TRUE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdIntResourceSearchFree -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Search the first byte of free space in given internal VPD resource. Return pointer to
// the free space if found, NULL otherwise. The free space is the first byte that is a
// non-valid item.
//
static const UInt8*  DtuVpdIntResourceSearchFree(
    const tVpdResourcePars*  pVpdResourcePars) // Parameters of selected resource
{
    const UInt8*  pResourceStart;   // Pointer to first byte of resource
    const UInt8*  pResourceEnd;     // Pointer to last byte of resource
    const UInt8*  pItem;            // Running item pointer
    tVpdItemPars  VpdItemPars;      // VPD item parameters
    
    // Sanity check
    if (pVpdResourcePars==NULL || pVpdResourcePars->m_Length==0)
        return NULL;

    // Initialize
    pResourceStart = pVpdResourcePars->m_pStart;
    pResourceEnd = pVpdResourcePars->m_pEnd;
    pItem = pResourceStart + 3;
    
    // Loop through resources
    while (DtuVpdIntItemCheck(pItem, pResourceStart, pResourceEnd, &VpdItemPars))
        pItem = VpdItemPars.m_pNext;

    // No more valid keywords => found end, assert it is within the resource
    if (pItem > pResourceEnd)
        return NULL;
    return pItem;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdInitIdOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Locate the start of the VpdId item. 
// 9056 boards: offset VPD_PLX_ID_START_NEW (0x63)
// Other boards: offset VPD_PLX_ID_START (0x58)
//
// Determine offsets and lengths of ID, VPD-R and VPD-W resources.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
static void  DtuVpdInitIdOffset(DtuDeviceData* pDvcData)
{
    
    DtVpd*  pVpd = &pDvcData->m_Vpd;
    const UInt8 *  p;
    UInt  i;
    Int  VpdIdStart = -1;

    // Default offsets
    pVpd->m_IdOffset = 0;
    pVpd->m_RoOffset = 0;
    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
    {
        pVpd->m_RwOffset = VPD_RW_START_DTU3;
        pVpd->m_IntRoOffset = VPD_END_DTU3 + 8;
        pVpd->m_IntRwOffset = VPD_END_DTU3 + 8;
        VpdIdStart = VPD_ID_START_DTU3;
    } else {
        pVpd->m_RwOffset = VPD_RW_START_DTU2;
        pVpd->m_IntRoOffset = VPD_END_DTU2 + 8;
        pVpd->m_IntRwOffset = VPD_END_DTU2 + 8;
        VpdIdStart = VPD_ID_START_DTU2;
    }

    // Default lengths
    pVpd->m_IdLength = 0;
    pVpd->m_RoLength = 0;
    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
        pVpd->m_RwLength = VPD_RW_LENGTH_DTU3;
    else
        pVpd->m_RwLength = VPD_RW_LENGTH_DTU2;
    pVpd->m_IntRoLength = 0;
    pVpd->m_IntRwLength = 0;

    // Search ID string tag
    if (pVpd->m_pCache[VpdIdStart] == VPD_IDSTRING_TAG)
    {
        pVpd->m_IdOffset = VpdIdStart;
        DtDbgOut(MAX, VPD, "VpdOffset: %x", pVpd->m_IdOffset);
    }
    else
        DtDbgOut(MIN, VPD, "Empty EEPROM or illegal layout");
    p = &pVpd->m_pCache[pVpd->m_IdOffset];
    if (*p == VPD_IDSTRING_TAG)
    {
        Int  IdLength = 3 + p[1] + (p[2]<<8);
        if ((Int)(pVpd->m_IdOffset + IdLength) < pVpd->m_VpdEnd)
        {
            pVpd->m_IdLength = IdLength;
            pVpd->m_RoOffset = pVpd->m_IdOffset + pVpd->m_IdLength;
            p = &pVpd->m_pCache[pVpd->m_RoOffset];
            if (*p == VPD_R_TAG)
                pVpd->m_RoLength = pVpd->m_RwOffset - pVpd->m_RoOffset;
        }
    }

    i = pVpd->m_VpdEnd+1;
    p = pVpd->m_pCache;
    if (i+8<pVpd->m_EepromVpdSize && *(p+i)==VPD_INTSTART_TAG && !strncmp(p+i+1,"DekTec",6))
    {
        Bool loop;
        // DekTec's internal start resource found
        i += 7;

        loop = TRUE;
        while (loop)
        {
            switch (pVpd->m_pCache[i])
            {
            case VPD_R_TAG:         // Internal VPD-R resource found
                pVpd->m_IntRoOffset = i;
                pVpd->m_IntRoLength = p[i+1] + (p[i+2]<<8);
                i += 3 + pVpd->m_IntRoLength;
                break;
            case VPD_W_TAG:         // Internal VPD-W resource found
                pVpd->m_IntRwOffset = i;
                pVpd->m_IntRwLength = p[i+1] + (p[i+2]<<8);
                i += 3 + pVpd->m_IntRwLength;
                break;
            case VPD_END_TAG:       // End tag found
                loop = FALSE;
                break;
            default:                // Inconsistency found
                pVpd->m_IntRoLength = 0;
                pVpd->m_IntRwLength = 0;
                loop = FALSE;
                break;
            }
        }
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdUpdateCache -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Load the VPD cache with the current EEPROM content.
//
static DtStatus  DtuVpdUpdateCache(DtuDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;

    // Load VPD cache
    switch (pDvcData->m_Vpd.m_EepromIoItf)
    {
    case VPD_EEPROM_IO_EZUSB:
        // Read entire content of VPD EEPROM (EzUsb) into cache buffer
        Status = DtuEepromEzUsbRead(pDvcData, 0, pDvcData->m_Vpd.m_pCache,
                                                         pDvcData->m_Vpd.m_EepromVpdSize);
        break;

    default:
        Status = DT_STATUS_NOT_SUPPORTED;
        break;
    }

    if (DT_SUCCESS(Status))
    {
        Bool  VpdEmpty = TRUE;
        UInt  i;

        // Reset VPD corrupt flag. It'll be set again if required.
        pDvcData->m_StateFlags &= ~DTU_DEV_FLAG_VPD_CORRUPT;

        for (i=0; i<pDvcData->m_Vpd.m_EepromVpdSize; i++)
        {
            if (pDvcData->m_Vpd.m_pCache[i] != 0xFF)
            {
                VpdEmpty = FALSE;
                break;
            }
        }
        if (VpdEmpty)
        {
            pDvcData->m_BootState = DTU_BOOTSTATE_FACTORY_WARM;
            pDvcData->m_StateFlags |= DTU_DEV_FLAG_VPD_CORRUPT;
            DtDbgOut(MIN, VPD, "Empty EEPROM");
        } else {
            // Determine ID offset
            DtuVpdInitIdOffset(pDvcData);

            // Check start byte
            if (pDvcData->m_Vpd.m_pCache[pDvcData->m_Vpd.m_IdOffset] != VPD_IDSTRING_TAG)
            {
                pDvcData->m_StateFlags |= DTU_DEV_FLAG_VPD_CORRUPT;
                DtDbgOut(MIN, VPD, "Illegal layout");
            }
        }
    }

    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdUpdateEepromFromCache -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write an array of bytes to the serial EEPROM.
// Writing is complicated by the fact that we can only write 32-bit words to the
// PLX EEPROM.
//
static DtStatus DtuVpdUpdateEepromFromCache(
    DtuDeviceData*  pDvcData,   // Device extension
    UInt  StartAddr,            // Start address (BYTE address)
    UInt  Length)               // Number of BYTES to write
{
    DtStatus  Status = DT_STATUS_OK;

    // Load VPD cache
    switch (pDvcData->m_Vpd.m_EepromIoItf)
    {
    case VPD_EEPROM_IO_EZUSB:
        // Write only changed data.
        Status = DtuEepromEzUsbWrite(pDvcData, StartAddr,
                                              pDvcData->m_Vpd.m_pCache+StartAddr, Length);
        break;   

    default:
        Status = DT_STATUS_NOT_SUPPORTED;
        break;
    }

    // Always read back EEPROM when a write attempt is done and update ID offset
    if (Status != DT_STATUS_NOT_SUPPORTED)
        DtuVpdUpdateCache(pDvcData);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdReadFromCache -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Read an array of bytes from the EEPROM cache.
//
static DtStatus  DtuVpdReadFromCache(
    DtuDeviceData*  pDvcData,   // Device extension
    UInt8*  pBuf,               // Buffer for data read from serial EEPROM
    UInt  StartAddr,            // Start address (BYTE address)
    UInt  Length)               // Number of BYTES to read
{
    DtDbgOut(MAX, VPD, "StartAddr=0x%03x Length=%d", StartAddr, Length);

    // Check parameters
    if (StartAddr >= pDvcData->m_Vpd.m_EepromVpdSize)
    {
        DtDbgOut(ERR, VPD, "StartAddr=0x%03x INVALID", StartAddr);
        return DT_STATUS_FAIL;
    }
    if (StartAddr+Length > pDvcData->m_Vpd.m_EepromVpdSize)
    {
        DtDbgOut(ERR, VPD, "Length=%d INVALID", Length);
        return DT_STATUS_FAIL;
    }

    // Let's be robust
    if (Length == 0)
        return DT_STATUS_OK;

    // Read from VPD cache for all cards
    DtMemCopy(pBuf, &pDvcData->m_Vpd.m_pCache[StartAddr], Length);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdFindEndOfRoSection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtuVpdFindEndOfRoSection(
    DtuDeviceData*  pDvcData,   // Device extension
    UInt8**  pItem)
{
    UInt8*  pEndOfRo;
    DtStatus  Status = DT_STATUS_OK;
    Int  VpdRwStart;
    DtVpd*  pVpd = &pDvcData->m_Vpd;
    
    *pItem = NULL;
    
    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
        VpdRwStart = VPD_RW_START_DTU3;
    else
        VpdRwStart = VPD_RW_START_DTU2;
    
    Status = DtVpdFindStartOfRoSection(pVpd, &pEndOfRo, VpdRwStart);
    if (Status != DT_STATUS_OK)
        return Status;

    // Skip the section header. This has to be done before the next loop, since the 3rd
    // byte of the section header is not necessarily 0.
    pEndOfRo += 3;

    while ((pEndOfRo < pVpd->m_pCache+VpdRwStart-3)
           && pEndOfRo[0]!='\0' && !(pEndOfRo[0]=='R' && pEndOfRo[1]=='V'))
        pEndOfRo = pEndOfRo + 3 + pEndOfRo[2];

    if (pEndOfRo > pVpd->m_pCache+VpdRwStart-4)
        return DT_STATUS_FAIL;
    
    *pItem = pEndOfRo;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdCreateRV -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Create a RV item in the RO section.
//
static DtStatus  DtuVpdCreateRV(DtuDeviceData*  pDvcData)
{
    UInt  CheckSum = 0;
    UInt8*  pVpd;               // Byte pointer in VPD data
    UInt8*  pRv;
    DtStatus  Status;
    Int  VpdRwStart;
    
    Status = DtuVpdFindEndOfRoSection(pDvcData, &pRv);
    if (!DT_SUCCESS(Status))
        return Status;
    
    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
        VpdRwStart = VPD_RW_START_DTU3;
    else
        VpdRwStart = VPD_RW_START_DTU2;

    // Create "RV" item
    pRv[0] = 'R';
    pRv[1] = 'V';
    pRv[2] = (UInt8)DtPtrToUInt((void*)(&pDvcData->m_Vpd.m_pCache[VpdRwStart] - &pRv[3]));

    // Compute checksum
    for (pVpd=pDvcData->m_Vpd.m_pCache+pDvcData->m_Vpd.m_IdOffset; pVpd<=pRv+2; pVpd++)
        CheckSum += *pVpd;
    pRv[3] = (~CheckSum + 1) & 0xFF;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdDeleteItemRo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Delete all VPD items with keyword pKeyword from the read-only section.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
static DtStatus  DtuVpdDeleteItemRo(
    DtuDeviceData*  pDvcData,   // Device extension
    UInt KeywordLen,            // Length of Keyword
    const char*  pKeyword)      // Name of VPD item to be deleted
{
    UInt  RoLength;
    UInt8*  pVpd;               // Byte pointer in VPD data
    UInt8*  pVpdRoEnd;          // Pointer one after RO section
    DtStatus  Status;
    Int  VpdRwStart;
    
    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
        VpdRwStart = VPD_RW_START_DTU3;
    else
        VpdRwStart = VPD_RW_START_DTU2;
    
    // Compute a number of helper variables
    pVpdRoEnd = &pDvcData->m_Vpd.m_pCache[VpdRwStart];

    // Start at beginning of VPD-ID + read-only section
    Status = DtVpdFindStartOfRoSection(&pDvcData->m_Vpd, &pVpd, VpdRwStart);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Length of RO section
    RoLength = (UInt)(pVpdRoEnd - pVpd);

    return DtVpdDeleteResource(KeywordLen, pKeyword, pVpd, RoLength);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdDeleteItemRw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Delete all VPD items with keyword pKeyword from the read-write section.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
static DtStatus  DtuVpdDeleteItemRw(
    DtuDeviceData*  pDvcData,   // Device extension
    UInt KeywordLen,            // Length of Keyword
    const char*  pKeyword)      // Name of VPD item to be deleted
{
    UInt8*  pVpd;               // Byte pointer in VPD data
    Int  VpdRwStart;
    Int  VpdRwLength;
    
    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
    {
        VpdRwStart = VPD_RW_START_DTU3;
        VpdRwLength = VPD_RW_LENGTH_DTU3;
    } else {
        VpdRwStart = VPD_RW_START_DTU2;
        VpdRwLength = VPD_RW_LENGTH_DTU2;
    }

    // Check whether we can find resource in RW section
    pVpd = &pDvcData->m_Vpd.m_pCache[VpdRwStart];
    return DtVpdDeleteResource(KeywordLen, pKeyword, pVpd, VpdRwLength);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdWriteId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write the VPD Id into the cache/EEPROM. If the VPD Id already exists, 
// the old ID will be erased and the RO section will be moved.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
static DtStatus  DtuVpdWriteId(
    DtuDeviceData*  pDvcData,   // Device extension
    char*  pVpdId,              // String buffer with VPD Id
    UInt  IdLength)
{
    UInt  Length, SizeAvail;
    UInt8*  pVpd;               // Byte pointer in VPD data
    UInt8*  pEndOfRo;
    Int  VpdRwStart;
    
    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
        VpdRwStart = VPD_RW_START_DTU3;
    else
        VpdRwStart = VPD_RW_START_DTU2;
    
    // Initialisation
    SizeAvail = VpdRwStart - pDvcData->m_Vpd.m_IdOffset - 3;
    Length = 0;

    // Start at beginning of VPD-ID + read-only section
    pVpd = &pDvcData->m_Vpd.m_pCache[pDvcData->m_Vpd.m_IdOffset];
    if (pVpd[0] == VPD_IDSTRING_TAG)
    {
        // VPDID already exists. Check if enough space is available.
        Length = pVpd[1] + ((UInt)pVpd[2]<<8);

        // Find last item
        pEndOfRo = pVpd + 3 + Length;

        if (pEndOfRo[0] == VPD_R_TAG)
        {
            Int Size;
            UInt8* pSize;
            if (DT_SUCCESS(DtuVpdFindEndOfRoSection(pDvcData, &pEndOfRo)))
                SizeAvail = VpdRwStart - DtPtrToUInt(
                                        (void*)(pEndOfRo - pDvcData->m_Vpd.m_pCache)) - 4;
            else
                SizeAvail = 0;
            
            if (SizeAvail+Length < IdLength) {
                DtDbgOut(MIN, VPD, "VPD RO SECTION FULL");
                return DT_STATUS_FAIL;
            }

            // Move existing RO section
            DtMemMove(pVpd + 3 + IdLength, pVpd + 3 + Length, 
                        VpdRwStart - pDvcData->m_Vpd.m_IdOffset - SizeAvail - 3 - Length);

            // Update size of RO section.
            pSize = pVpd + 3 + IdLength;
            Size = pSize[1] + ((UInt)pSize[2] << 8);
            Size = Size + Length - IdLength;
            pSize[1] = Size & 0xff;
            pSize[2] = Size >> 8;

            // Set all bytes left over to 0
            if (Length > IdLength)
                DtMemZero(pEndOfRo + 4 - (Length - IdLength), Length - IdLength);
        } else
            SizeAvail = VpdRwStart -
                              DtPtrToUInt((void*)(pEndOfRo-pDvcData->m_Vpd.m_pCache)) - 3;
    }

    // Write VPDID
    if (IdLength > SizeAvail+Length)
    {
        DtDbgOut(MIN, VPD, "VPD RO SECTION VPDID too long");
        return DT_STATUS_FAIL;
    }
    pVpd[0] = VPD_IDSTRING_TAG;
    pVpd[1] = IdLength & 0xff;
    pVpd[2] = (IdLength >> 8);
    DtMemCopy(pVpd + 3, pVpdId, IdLength);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdWriteItemRo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write a VPD Item into the RO section of the VPD cache. If the Item already exists, 
// the old Item will be erased.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
static DtStatus  DtuVpdWriteItemRo(
    DtuDeviceData*  pDvcData,   // Device extension
    UInt KeywordLen,            // Length of Keyword
    const char*  pKeyword,      // String buffer with 2 bytes keyword
    char*  pVpdItem,            // String buffer with VPD Item
    UInt  ItemLength)           // Length of pVpdItem
{
    UInt  SizeAvail;
    UInt8*  pEndOfRo;
    DtStatus  Status;
    Int  VpdRwStart;
    
    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
        VpdRwStart = VPD_RW_START_DTU3;
    else
        VpdRwStart = VPD_RW_START_DTU2;

    DT_ASSERT(KeywordLen==2);

    DtuVpdDeleteItemRo(pDvcData, KeywordLen, pKeyword);
    
    Status = DtuVpdFindEndOfRoSection(pDvcData, &pEndOfRo);
    if (!DT_SUCCESS(Status))
        return Status;

    SizeAvail = VpdRwStart - DtPtrToUInt((void*)(pEndOfRo-pDvcData->m_Vpd.m_pCache));
    if (pKeyword[0]!='R' || pKeyword[1]!='V')
        SizeAvail -= 4;
            
    if (SizeAvail < (ItemLength + 3))
    {
        DtDbgOut(MIN, VPD, "VPD RO SECTION FULL");
        return DT_STATUS_FAIL;
    }

    pEndOfRo[0] = pKeyword[0];
    pEndOfRo[1] = pKeyword[1];
    pEndOfRo[2] = (UInt8)ItemLength;

    DtMemCopy(pEndOfRo + 3, pVpdItem, ItemLength);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdWriteItemRw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Write a VPD Item into the RW section of the VPD cache. If the VPD Item already exists, 
// the old Item will be erased.
//
// PRE: EEPROM contents have been read into VPD-EEPROM cache (m_pVpdEeprom)
//
static DtStatus  DtuVpdWriteItemRw(
    DtuDeviceData*  pDvcData,   // Device extension
    UInt KeywordLen,            // Length of Keyword
    const char*  pKeyword,      // String buffer with 2 bytes keyword
    char*  pVpdItem,            // String buffer with VPD Item
    UInt  ItemLength)           // Length of pVpdItem
{
    UInt  BytesUsed, SizeAvail;
    UInt8*  pVpdRw;             // Byte pointer in VPD data
    Int  VpdRwStart;
    Int  VpdRwLength;
    
    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
    {
        VpdRwStart = VPD_RW_START_DTU3;
        VpdRwLength = VPD_RW_LENGTH_DTU3;
    } else {
        VpdRwStart = VPD_RW_START_DTU2;
        VpdRwLength = VPD_RW_LENGTH_DTU2;
    }
    pVpdRw = &pDvcData->m_Vpd.m_pCache[VpdRwStart];

    DT_ASSERT(KeywordLen==2);

    // Check structure in serial EEPROM
    if (pVpdRw[0]!=VPD_W_TAG || pVpdRw[1]!=0xFC || pVpdRw[2]!=0x00 || 
                                                                pVpdRw[0xFF]!=VPD_END_TAG)
    {
        // Basic VPD Read/Write Resources data structure not present. Create it.
        DtMemZero(pVpdRw, VpdRwLength);
        pVpdRw[0] = VPD_W_TAG;
        pVpdRw[1] = 0xFC;
        pVpdRw[0xFF] = VPD_END_TAG;
    }
    
    DtuVpdDeleteItemRw(pDvcData, KeywordLen, pKeyword);

    // Calculate available bytes
    BytesUsed = DtVpdGetSectionLength(pVpdRw, VpdRwLength);
    SizeAvail = VpdRwLength - BytesUsed;
                
    if (SizeAvail < ItemLength+3)
    {
        DtDbgOut(MIN, VPD, "VPD RW SECTION FULL");
        return DT_STATUS_FAIL;
    }

    pVpdRw += BytesUsed;
    pVpdRw[0] = pKeyword[0];
    pVpdRw[1] = pKeyword[1];
    pVpdRw[2] = (UInt8)ItemLength;

    DtMemCopy(pVpdRw + 3, pVpdItem, ItemLength);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write an array of bytes to the VPD cache and update the serial EEPROM.
//
static DtStatus  DtuVpdWrite(
    DtuDeviceData*  pDvcData,   // Device extension
    UInt8*  pBuf,               // Buffer with data to write to serial EEPROM
    UInt  StartAddr,            // Start address (BYTE address)
    UInt  Length)               // Number of BYTES to write
{
    UInt  Addr;
    UInt  ByteIndex;
    
    DtDbgOut(MAX, VPD, "StartAddr=0x%03x Length=%d pBuf=0x%p", StartAddr, Length, pBuf);

    // Check parameters
    if (StartAddr >= pDvcData->m_Vpd.m_EepromVpdSize)
    {
        DtDbgOut(ERR, VPD, "StartAddr=0x%03x INVALID", StartAddr);
        return DT_STATUS_FAIL;
    }
    if (StartAddr+Length > pDvcData->m_Vpd.m_EepromVpdSize)
    {
        DtDbgOut(ERR, VPD, "Length=%d INVALID", Length);
        return DT_STATUS_FAIL;
    }

    // Let's be robust
    if (Length == 0)
        return DT_STATUS_OK;

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Write into VPD cache -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Addr = StartAddr;
    for (ByteIndex=0; ByteIndex<Length; ByteIndex++) 
        pDvcData->m_Vpd.m_pCache[Addr++] = pBuf[ByteIndex];

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Update EEPROM -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    return DtuVpdUpdateEepromFromCache(pDvcData, StartAddr, Length);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdReadItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtuVpdReadItem(
    DtuDeviceData* pDvcData,
    Int SectionType,
    UInt KeywordLen,
    const char* pKeyword,
    UInt8* pBuf,
    UInt* pBufLen)
{
    DtStatus  Status = DT_STATUS_FAIL;
    
    // Read from the right section
    if (SectionType == DTU_VPD_SECT_ID)
        Status = DtVpdReadId(&pDvcData->m_Vpd, (char*)pBuf, pBufLen);
    else {
        if (!DT_SUCCESS(Status) && ((SectionType&DTU_VPD_SECT_RO)!=0))
        {
            if (KeywordLen == 2)
                Status = DtuVpdReadItemRo(pDvcData, KeywordLen, pKeyword, pBuf, pBufLen);
            else if (KeywordLen > 2)
                Status = DtuVpdIntItemRead(pDvcData, DTU_VPD_SECT_RO, KeywordLen,
                                                                 pKeyword, pBuf, pBufLen);
            else
                Status = DT_STATUS_FAIL;
        }
        if (!DT_SUCCESS(Status) && ((SectionType&DTU_VPD_SECT_RW)!=0))
        {
            if (KeywordLen == 2)
                Status = DtuVpdReadItemRw(pDvcData, KeywordLen, pKeyword, pBuf, pBufLen);
            else if (KeywordLen > 2)
                Status = DtuVpdIntItemRead(pDvcData, DTU_VPD_SECT_RW, KeywordLen, 
                                                                 pKeyword, pBuf, pBufLen);
            else
                Status = DT_STATUS_FAIL;
        }
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdDeleteItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Delete all VPD items in the specified sections
//
static DtStatus  DtuVpdDeleteItem(
    DtuDeviceData*  pDvcData,   // Device extension
    Int  SectionType,
    UInt KeywordLen,            // Length of Keyword
    const char*  pKeyword)      // Name of VPD item to be deleted
{
    DtStatus  Status = DT_STATUS_OK;
    UInt  StartAddr;
    UInt  Length = 0;
    
    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
        StartAddr = VPD_RW_START_DTU3;
    else
        StartAddr = VPD_RW_START_DTU2;

    if ((SectionType&DTU_VPD_SECT_RO) != 0)
    {      
        if (KeywordLen == 2)
        {
            Status = DtuVpdDeleteItemRo(pDvcData, KeywordLen, pKeyword);
            StartAddr = pDvcData->m_Vpd.m_RoOffset;
            Length = pDvcData->m_Vpd.m_RoLength;
        }  else if (KeywordLen > 2) {
            Status = DtuVpdIntItemDelete(pDvcData, DTU_VPD_SECT_RO, KeywordLen, pKeyword);
            StartAddr = pDvcData->m_Vpd.m_IntRoOffset;
            Length = pDvcData->m_Vpd.m_IntRoLength;
        } else {
            Status = DT_STATUS_FAIL;
        }
        if (!DT_SUCCESS(Status))
            return Status;
    }

    if ((SectionType&DTU_VPD_SECT_RW) != 0)
    {
        if (KeywordLen == 2)
        {
            Status = DtuVpdDeleteItemRw(pDvcData, KeywordLen, pKeyword);
            StartAddr = pDvcData->m_Vpd.m_RwOffset;
            Length = pDvcData->m_Vpd.m_RwLength;
        }  else if (KeywordLen > 2) {
            Status = DtuVpdIntItemDelete(pDvcData, DTU_VPD_SECT_RW, KeywordLen, pKeyword);
            StartAddr = pDvcData->m_Vpd.m_RwOffset;
            Length = pDvcData->m_Vpd.m_RwLength;
        } else {
            Status = DT_STATUS_FAIL;
        }
        if (!DT_SUCCESS(Status))
            return Status;
    }

    // Update RO and RW section if needed
    if (Length != 0)
        Status = DtuVpdUpdateEepromFromCache(pDvcData, StartAddr, Length);
    
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuVpdWriteItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus DtuVpdWriteItem(
    DtuDeviceData*  pDvcData,   // Device extension
    Int  SectionType,
    UInt KeywordLen,            // Length of Keyword
    const char*  pKeyword,      // Name of VPD item to be deleted
    char*  pBuf,
    Int  BufLen,
    Int  Flags)
{
    Bool  UpdateEeprom = (Flags&DTU_VPD_FLAG_UPDATE_EEPROM) != 0;
    Bool  UpdateRV = FALSE;
    DtStatus  Status = DT_STATUS_OK;
    UInt  Start = 0;
    UInt  Length = 0;
    switch (SectionType)
    {
    case DTU_VPD_SECT_ID:
        if ((Flags&DTU_VPD_FLAG_RO_WRITE_ALLOWED) == 0)
            return DT_STATUS_FAIL;
        Status = DtuVpdWriteId(pDvcData, pBuf, BufLen);
        Start = pDvcData->m_Vpd.m_IdOffset;
        // Include RO-section because it can be moved if not enough space for ID
        // The section length is recalculated in DtaVpdWriteId
        Length = pDvcData->m_Vpd.m_IdLength + pDvcData->m_Vpd.m_RoLength;
        UpdateRV = TRUE;
        break;

    case DTU_VPD_SECT_RO:
        if ((Flags&DTU_VPD_FLAG_RO_WRITE_ALLOWED) == 0)
            return DT_STATUS_FAIL;
        if (KeywordLen == 2)
        {
            Status = DtuVpdWriteItemRo(pDvcData, KeywordLen, pKeyword, pBuf, BufLen);
            Start = pDvcData->m_Vpd.m_RoOffset;
            Length = pDvcData->m_Vpd.m_RoLength;
            UpdateRV = TRUE;
        }  else if (KeywordLen > 2) {
            Status = DtuVpdIntItemWrite(pDvcData, DTU_VPD_SECT_RO, KeywordLen, pKeyword, 
                                                                            pBuf, BufLen);
            Start = pDvcData->m_Vpd.m_IntRoOffset;
            Length = pDvcData->m_Vpd.m_IntRoLength;
        } else {
            Status = DT_STATUS_FAIL;
        }
        break;
        
    case DTU_VPD_SECT_RW:
        if (KeywordLen == 2)
        {
            Status = DtuVpdWriteItemRw(pDvcData, KeywordLen, pKeyword, pBuf, BufLen);
            Start = pDvcData->m_Vpd.m_RwOffset;
            Length = pDvcData->m_Vpd.m_RwLength;
        }  else if (KeywordLen > 2) {
            Status = DtuVpdIntItemWrite(pDvcData, DTU_VPD_SECT_RW, KeywordLen, pKeyword, 
                                                                            pBuf, BufLen);
            Start = pDvcData->m_Vpd.m_IntRwOffset;
            Length = pDvcData->m_Vpd.m_IntRwLength;
        } else {
            Status = DT_STATUS_FAIL;
        }
        break;
    }

    if (DT_SUCCESS(Status) && UpdateEeprom)
        Status = DtuVpdCreateRV(pDvcData);

    if (DT_SUCCESS(Status) && UpdateEeprom) 
        Status = DtuVpdUpdateEepromFromCache(pDvcData, Start, Length);

    return Status;
}
