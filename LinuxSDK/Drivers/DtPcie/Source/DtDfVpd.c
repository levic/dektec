//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfVpd.c *#*#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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
#include "DtDfVpd.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct
{
    Bool  m_IsDeleted;
    UInt8  m_KeywordLen;
    const char*  m_pKeyword;
    UInt16  m_ItemSize;
    const char*  m_pItem;
    const UInt8*  m_pNext;
} DtVpdItemPars;

typedef struct
{
    const char*  m_Name;        // Name, 0 terminated string
    Int  m_Length;              // Length of this resource (0 if resource doesn't exist)
    Int  m_Offset;              // Offset in EEPROM/FLASH for this resource
    UInt8*  m_pStart;           // Pointer to first byte in cache of this resource 
    UInt8*  m_pEnd;             // Pointer to last byte in cache of this resource
} DtVpdSectionPars;


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Define roles of SpiMasterFlash block in the VPD function
#define  SPIPROM_ROLE_NONE          NULL
#define  SPIMF_ROLE_EEPROM_SPI      "EEPROM_SPI"

// VPD item m_ItemTag fields
#define  VPD_ITEMTAG_KEYLEN_MASK  0xF0
#define  VPD_ITEMTAG_KEYLEN_POS   4
#define  VPD_ITEMTAG_STATE_MASK   0x0F
#define  VPD_ITEMTAG_STATE_8BDEL  0x00 // Deleted UInt8 item_size based structure
#define  VPD_ITEMTAG_STATE_16BDEL 0x01 // Deleted UInt16 item_size based structure
#define  VPD_ITEMTAG_STATE_16B    0x0D // UInt16 item_size based structure
#define  VPD_ITEMTAG_STATE_8B     0x0E // UInt8 item_size based structure
#define  VPD_ITEMTAG_STATE_ERASED 0x0F // Erased memory
#define  VALID_VPD_KEYW_CHAR(c)     (((c)>='0' && (c)<='9') || ((c)>='A' && (c)<='Z') || \
                                                                                 (c)=='_')

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfVpd implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfVpd function
#define DF_VPD_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfVpd))

// Helper macro to cast a DtDf* to a DtDfVpd*
#define DF_VPD      ((DtDfVpd*)pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfVpd_Init(DtDf*);
static DtStatus  DtDfVpd_LoadParameters(DtDf*);
static DtStatus  DtDfVpd_OpenChildren(DtDfVpd*);

static Bool  DtDfVpd_CheckVpdItem(const UInt8*, const DtVpdSectionPars*, DtVpdItemPars*);
static DtStatus  DtDfVpd_DeleteVpdItem(Int KeyLen, const char*, const DtVpdSectionPars*);
static DtStatus  DtDfVpd_FormatSection(DtDfVpd* pDf, Int SectionType, Int Flags);
static DtStatus  DtDfVpd_GetEepromSize(DtDfVpd* pDf, Int* pMemorySize);
static DtStatus  DtDfVpd_GetSectionPars(DtDfVpd* pDf, Int, DtVpdSectionPars*);
static DtStatus  DtDfVpd_ItemDeleteInSection(DtDfVpd*  pDf, Int  SectionType, Int  KeyLen, 
                                                       const char*  pKeyword,  Int Flags);
static DtStatus  DtDfVpd_ItemReadFromSection(DtDfVpd*,  Int  SectionFlags, 
                                              Int  KeyLen, const char*  pKeyword,
                                              Int  BufSize, const UInt8*, Int*  pNumRead);
static DtStatus  DtDfVpd_ItemWriteToSection(DtDfVpd*, Int  SectionType,
                                               Int  KeyLen, const char*  pKeyword, 
                                               Int  Flags, Int  NumToWrite, const UInt8*);
static DtStatus  DtDfVpd_SearchVpdFree(const DtVpdSectionPars*, const UInt8**);
static DtStatus  DtDfVpd_SearchVpdItem(Int KeyLen, const char*,
                                                 const DtVpdSectionPars*, DtVpdItemPars*);
static DtStatus  DtDfVpd_UpdateCacheFromEeprom(DtDfVpd* pDf);
static DtStatus  DtDfVpd_UpdateEepromFromCache(DtDfVpd* pDf, Int Offset, Int Length);
static DtStatus  DtDfVpd_WriteVpdItem(const UInt8*  pVpdData, Int KeyLen, 
                               const char*  pKeyword, const UInt8*  pItem, Int  ItemSize);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfVpd - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfVpd_Close(DtDf*  pDf)
{
    DF_VPD_DEFAULT_PRECONDITIONS(pDf);

    // Free the cache buffer
    if (DF_VPD->m_pCache != NULL)
    {
        DtMemFreePool(DF_VPD->m_pCache, DF_TAG);
        DF_VPD->m_pCache = NULL;
        DF_VPD->m_CacheSize = 0;
    }

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfVpd*  DtDfVpd_Open(DtCore*  pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_VPD_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfVpd, Id, DT_FUNC_TYPE_VPD, pPt,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfVpd_Close;
    OpenParams.m_InitFunc = DtDfVpd_Init;
    OpenParams.m_LoadParsFunc = DtDfVpd_LoadParameters;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfVpd*)DtDf_Open(&OpenParams);
}
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_Format -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfVpd_Format(DtDfVpd* pDf, Int SectionFlags, Int Flags)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    DF_VPD_DEFAULT_PRECONDITIONS(pDf);

    // Parameter check
    if ((SectionFlags&(DT_VPD_SECT_RO | DT_VPD_SECT_RW))==0
                                  || (SectionFlags&~(DT_VPD_SECT_RO | DT_VPD_SECT_RW))!=0)
    {
        DtDbgOutDf(ERR, VPD, pDf, "ERROR: Invalid SectionFlags");
        return DT_STATUS_INVALID_PARAMETER;
    }
   
    // Prevent mutual access to VPD
    DtFastMutexAcquire(&pDf->m_VpdMutex);

    // Format RO-section?
    if ((SectionFlags&DT_VPD_SECT_RO) != 0)
        Status = DtDfVpd_FormatSection(pDf, DT_VPD_SECT_RO, Flags);

    if (DT_SUCCESS(Status) && (SectionFlags&DT_VPD_SECT_RW)!=0)
    {
        // Format RW-section?
            Status = DtDfVpd_FormatSection(pDf, DT_VPD_SECT_RW, Flags);
    }

    DtFastMutexRelease(&pDf->m_VpdMutex);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_GetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfVpd_GetProperties(DtDfVpd*  pDf, DtDfVpdProperties* pProperties)
{
    // Sanity checks
    DF_VPD_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pProperties != NULL);

    // Copy VPD properties
    pProperties->m_RoOffset = pDf->m_RoOffset;
    pProperties->m_RoSize = pDf->m_RoSize;
    pProperties->m_RwOffset = pDf->m_RwOffset;
    pProperties->m_RwSize = pDf->m_RwSize;
    pProperties->m_EepromSize = pDf->m_CacheSize;
    pProperties->m_MaxItemLength = DT_VPD_ITEM_MAX_LENGTH;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_ItemDelete -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfVpd_ItemDelete(
    DtDfVpd*  pDf, 
    Int  SectionFlags,
    Int  KeyLen, 
    const char*  pKeyword,
    Int  Flags)
{
    DtStatus  Status = DT_STATUS_FAIL;

    // Sanity checks
    DF_VPD_DEFAULT_PRECONDITIONS(pDf);

    // Parameter check
    if ((SectionFlags&(DT_VPD_SECT_RO | DT_VPD_SECT_RW))==0
                                  || (SectionFlags&~(DT_VPD_SECT_RO | DT_VPD_SECT_RW))!=0)
    {
        DtDbgOutDf(ERR, VPD, pDf, "ERROR: Invalid SectionFlags");
        return DT_STATUS_INVALID_PARAMETER;
    }
    
    // Prevent mutual access to VPD
    DtFastMutexAcquire(&pDf->m_VpdMutex);

    // Delete item in RO-section?
    if ((SectionFlags&DT_VPD_SECT_RO) != 0)
        Status = DtDfVpd_ItemDeleteInSection(pDf,  DT_VPD_SECT_RO, KeyLen,
                                                                         pKeyword, Flags);
    // Not yet deleted?  
    if ((SectionFlags&DT_VPD_SECT_RW)!=0 && !DT_SUCCESS(Status))
    { 
        // Delete item in RW-section
            Status = DtDfVpd_ItemDeleteInSection(pDf, DT_VPD_SECT_RW, KeyLen,
                                                                         pKeyword, Flags);
    }

    DtFastMutexRelease(&pDf->m_VpdMutex);
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_ItemRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read VPD item into a caller-allocated  buffer from the selected section.
//
//
DtStatus  DtDfVpd_ItemRead(
    DtDfVpd*  pDf, 
    Int  SectionFlags,
    Int  KeyLen, 
    const char*  pKeyword,
    Int  Flags,
    Int  BufSize, 
    const UInt8*  pBuf, 
    Int*  pNumRead)
{
    DtStatus  Status = DT_STATUS_FAIL;

    // Sanity checks
    DF_VPD_DEFAULT_PRECONDITIONS(pDf);

    // Parameter check
    if ((SectionFlags&(DT_VPD_SECT_RO | DT_VPD_SECT_RW))==0
                                  || (SectionFlags&~(DT_VPD_SECT_RO | DT_VPD_SECT_RW))!=0)
    {
        DtDbgOutDf(ERR, VPD, pDf, "ERROR: Invalid SectionFlags");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Prevent mutual access to VPD
    DtFastMutexAcquire(&pDf->m_VpdMutex);

    // Read item from RO-section?
    if ((SectionFlags&DT_VPD_SECT_RO) != 0)
        Status = DtDfVpd_ItemReadFromSection(pDf, DT_VPD_SECT_RO, KeyLen, pKeyword,
                                                                 BufSize, pBuf, pNumRead);
    // Not found?    
    if ((SectionFlags&DT_VPD_SECT_RW)!=0 && !DT_SUCCESS(Status))
    {
        // Read item from RW-section
            Status = DtDfVpd_ItemReadFromSection(pDf, DT_VPD_SECT_RW, KeyLen, pKeyword,
                                                                 BufSize, pBuf, pNumRead);
    }

    DtFastMutexRelease(&pDf->m_VpdMutex);
    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_ItemWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfVpd_ItemWrite(
    DtDfVpd*  pDf, 
    Int  SectionFlags,
    Int  KeyLen, 
    const char*  pKeyword,
    Int  Flags,
    Int  NumToWrite,
    const UInt8*  pBuf)
{
    DtStatus  Status = DT_STATUS_OK;
    
    // Sanity checks
    DF_VPD_DEFAULT_PRECONDITIONS(pDf);

    // Prevent mutual access to VPD
    DtFastMutexAcquire(&pDf->m_VpdMutex);

    // Write item to section; only one of the SectionsFlags should be set.
    Status = DtDfVpd_ItemWriteToSection(pDf,  SectionFlags, KeyLen,  pKeyword,
                                                                 Flags, NumToWrite, pBuf);

    DtFastMutexRelease(&pDf->m_VpdMutex);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_RawRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfVpd_RawRead(
    DtDfVpd*  pDf, 
    UInt  StartOffset,
    Int  NumToRead,
    UInt8*  pBuf,
    Int*  pNumRead)
{
    Int i, Offset;
    
    // Sanity checks
    DF_VPD_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pNumRead != NULL);

    // Nothing read so far
    *pNumRead = 0;

    // Parameter check
    if (pBuf==NULL || NumToRead<0 || pNumRead==NULL)
    {
        DtDbgOutDf(MIN, VPD, pDf, "Invalid VPD read parameters");
        return DT_STATUS_INVALID_PARAMETER;
    }
    if ((Int)(StartOffset+NumToRead) > pDf->m_CacheSize)
    {
        DtDbgOutDf(ERR, VPD, pDf, "Offset=%d or Length=%d INVALID", StartOffset,
                                                                               NumToRead);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Let's be robust
    if (NumToRead == 0)
        return DT_STATUS_OK;
    
    // Prevent mutual access to VPD
    DtFastMutexAcquire(&pDf->m_VpdMutex);

    // Read from VPD cache
    Offset = StartOffset;
    for (i=0; i<NumToRead; i++) 
        pBuf[i] = pDf->m_pCache[Offset++];

    *pNumRead = NumToRead;
    
    DtFastMutexRelease(&pDf->m_VpdMutex);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaVpdWriteRaw -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Write a raw byte array to the VPD EEMPROM
//
DtStatus  DtDfVpd_RawWrite(
    DtDfVpd*  pDf, 
    UInt  StartOffset,
    Int  NumToWrite,
    const UInt8*  pBuf)
{
    DtStatus  Status = DT_STATUS_OK;
    Int i, Offset;

    // Sanity checks
    DF_VPD_DEFAULT_PRECONDITIONS(pDf);
    
    // Parameter check
    if (pBuf==NULL || NumToWrite<0)
    {
        DtDbgOutDf(MIN, VPD, pDf, "Invalid VPD write parameters");
        return DT_STATUS_INVALID_PARAMETER;
    }
    if (((Int)StartOffset+NumToWrite) > pDf->m_CacheSize)
    {
        DtDbgOutDf(ERR, VPD, pDf, "Offset=%d or Length=%d INVALID", StartOffset, 
                                                                              NumToWrite);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Let's be robust
    if (NumToWrite == 0)
        return DT_STATUS_OK;
    
    // Prevent mutual access to VPD
    DtFastMutexAcquire(&pDf->m_VpdMutex);

    // Write into VPD cache
    Offset = StartOffset;
    for (i=0; i<NumToWrite; i++) 
        pDf->m_pCache[Offset++] = pBuf[i];

    // Update EEPROM 
    Status =  DtDfVpd_UpdateEepromFromCache(pDf, StartOffset, NumToWrite);

    DtFastMutexRelease(&pDf->m_VpdMutex);
    return Status;

}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfVpd - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfVpd_Init(DtDf*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  AllocSize = 0;

    // Sanity checks
    DF_VPD_DEFAULT_PRECONDITIONS(pDf);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    Status = DtDfVpd_OpenChildren(DF_VPD);
    if (!DT_SUCCESS(Status))
    {   
        DtDbgOutDf(ERR, VPD, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }

    // -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Init cache -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // Allocate cache. Cache size should equal size of EEPROM
    Status = DtDfVpd_GetEepromSize(DF_VPD, &AllocSize);
    if (!DT_SUCCESS(Status))
    {   
        DtDbgOutDf(ERR, VPD, pDf, "ERROR: failed to get EEPROM size");
        return DT_STATUS_FAIL;
    }
    DT_ASSERT(DF_VPD->m_pCache==NULL && DF_VPD->m_CacheSize==0);
    DF_VPD->m_pCache = DtMemAllocPool(DtPoolNonPaged, AllocSize, DF_TAG);
    if (DF_VPD->m_pCache == NULL)
    {
        DtDbgOutDf(ERR, VPD, pDf, "ERROR: failed to allocate cache memory (size=%d)",
                                                                               AllocSize);
        return DT_STATUS_OUT_OF_MEMORY;
    }
    DF_VPD->m_CacheSize = AllocSize;
    // Zero cache
    DtMemZero(DF_VPD->m_pCache, AllocSize);

    // Initialize fast mutex
    DtFastMutexInit(&DF_VPD->m_VpdMutex);

    // Check VPD properties
    DT_ASSERT(DF_VPD->m_RoSize >= 0);
    DT_ASSERT(DF_VPD->m_RwSize >= 0);

    // Compute offsets
    DF_VPD->m_RoOffset = 0x0000;
    DF_VPD->m_RwOffset = DF_VPD->m_RoOffset + DF_VPD->m_RoSize;

    // Update cache for the first time
    Status = DtDfVpd_UpdateCacheFromEeprom(DF_VPD);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, VPD, pDf, "ERROR: failed to init cache from EEPROM");
        return  Status;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfVpd_LoadParameters(DtDf*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;
 
    // List of VPD function parameters
    DtDfParameters  VPD_PARS[] =
    {
        // Name,  Value Type,  Value*
        { "RO_SIZE", PROPERTY_VALUE_TYPE_INT, &(DF_VPD->m_RoSize) },
        { "RW_SIZE", PROPERTY_VALUE_TYPE_INT, &(DF_VPD->m_RwSize) },
    };
 
    DF_VPD_DEFAULT_PRECONDITIONS(pDf);
 
    // Init parameters to their defaults
    DF_VPD->m_RoSize = -1;
    DF_VPD->m_RwSize = -1;
 
    // Load parameters from property store
    Status = DtDf_LoadParameters(pDf, DT_SIZEOF_ARRAY(VPD_PARS), VPD_PARS);
    if (!DT_SUCCESS(Status))
        return Status;
 
    // Check paramaters have been loaded succesfully
    DT_ASSERT(DF_VPD->m_RoSize >= 0);
    DT_ASSERT(DF_VPD->m_RwSize >= 0);
 
    // Report parameters
    DtDbgOutDf(MIN, VPD, pDf, "ro-size=%d, rw-size=%d", DF_VPD->m_RoSize, 
                                                                        DF_VPD->m_RwSize);
 
    return DT_STATUS_OK;
}
 
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfVpd_OpenChildren(DtDfVpd*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // List of children supported by the the VPD function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_DF, DT_FUNC_TYPE_SPIPROM, DT_DF_SPIPROM_NAME, SPIPROM_ROLE_NONE, 
                                           (DtObjectBcOrDf**)(&pDf->m_pDfSpiProm), TRUE },
    };

    DF_VPD_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the supported children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN, 
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    // All children must have been loaded
    DT_ASSERT(pDf->m_pDfSpiProm != NULL);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_CheckVpdItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Decodes and checks whether pVpdItem points to a correctly formatted VPD item in
// Returns decodeded parameters in  pVpdItemPars.
//
static Bool  DtDfVpd_CheckVpdItem(
    const UInt8* pVpdItem,                  // Pointer to item to be checked for validity
    const DtVpdSectionPars* pSectionPars,   // VPD section parameters
    DtVpdItemPars* pVpdItemPars)            // Decoded item parameter
{
    UInt8* p = (UInt8*)pVpdItem;
    Int  KeyLen, ItemSizeSize, ItemSize;
    UInt16  CheckSum;

    // Sanity check
    DT_ASSERT(pSectionPars!=NULL && pVpdItemPars!=NULL);

    // Free space (erased or emptied)?
    if (p==NULL || *p==0x00 || *p==0xFF)
        return FALSE;

    // item_state
    switch (*p & VPD_ITEMTAG_STATE_MASK)           
    {
    case VPD_ITEMTAG_STATE_8BDEL:
        pVpdItemPars->m_IsDeleted = TRUE;
        ItemSizeSize = 1;
        break;
    case VPD_ITEMTAG_STATE_8B:
        pVpdItemPars->m_IsDeleted = FALSE;
        ItemSizeSize = 1;
        break;
    case VPD_ITEMTAG_STATE_16BDEL:
        pVpdItemPars->m_IsDeleted = TRUE;
        ItemSizeSize = 2;
        break;
    case VPD_ITEMTAG_STATE_16B:
        pVpdItemPars->m_IsDeleted = FALSE;
        ItemSizeSize = 2;
        break;
    case VPD_ITEMTAG_STATE_ERASED:
    default:
        return FALSE;
    }

    // keyword_size
    KeyLen = (*p&VPD_ITEMTAG_KEYLEN_MASK) >> VPD_ITEMTAG_KEYLEN_POS; 
    if (KeyLen == 0)
        return FALSE;
    CheckSum = *p++&VPD_ITEMTAG_KEYLEN_MASK;

    // Check if item_size is within the resoruce
    if (p+KeyLen+ItemSizeSize > pSectionPars->m_pEnd)
        return FALSE;
    
    pVpdItemPars->m_KeywordLen = (UInt8)KeyLen;

    // keyword
    pVpdItemPars->m_pKeyword = p;
    while (KeyLen--)
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
    pVpdItemPars->m_ItemSize = (UInt16)ItemSize;

    // item
    pVpdItemPars->m_pItem = p;
    if (p+ItemSize+1 > pSectionPars->m_pEnd) // Check if check_sum is within resource
        return FALSE;
    while (ItemSize--)
        CheckSum += *p++;

    // check_sum
    CheckSum += (*p++) << 8;
    CheckSum += *p++;
    pVpdItemPars->m_pNext = p;
        
    return CheckSum==0;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_DeleteVpdItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Delete all VPD items with keyword pKeyword from the resource. 
//
static DtStatus  DtDfVpd_DeleteVpdItem(
    Int KeyLen,                             // Length of Keyword
    const char* pKeyword,                   // Name of VPD item to be deleted
    const DtVpdSectionPars* pSectionPars)   // Parameters of selected resource
{
    DtVpdItemPars  VpdItemPars;     // Parameters of VPD item to be deleted
    Int64  n;                       // Size of memory to be moved or erased
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    DT_ASSERT(KeyLen>0 && pKeyword!=NULL && pSectionPars!=NULL);

    // Check whether we can find the item in the resource
    Status = DtDfVpd_SearchVpdItem(KeyLen, pKeyword, pSectionPars, &VpdItemPars);
    if (Status != DT_STATUS_OK)
        return Status;

    // Delete resource in a loop, so that multiple instances are deleted too.
    do {
        // Copy all memory after this item, which is to be deleted, to this item
        UInt8*  pVpdData = (UInt8*)VpdItemPars.m_pItem;
        n = pSectionPars->m_pEnd - VpdItemPars.m_pNext + 1;
        if (n > 0) 
            DtMemMove(pVpdData, (void*)VpdItemPars.m_pNext, (UInt)n);
        else
            n=0;
        pVpdData += n;

        // Erase remaining memory in resource
        n = pSectionPars->m_pEnd - pVpdData + 1;
        if (n > 0)
            memset(pVpdData, 0xFF, (Int)n);

        // Find next item
        Status = DtDfVpd_SearchVpdItem(KeyLen, pKeyword, pSectionPars, &VpdItemPars);

    } while (Status == DT_STATUS_OK);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_FormatSection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfVpd_FormatSection(DtDfVpd* pDf, Int SectionType, Int Flags)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool  UpdateEeprom = (Flags&DT_VPD_FLAG_UPDATE_EEPROM) != 0;
    DtVpdSectionPars VpdSectionPars; // Parameters of the resource

    if (SectionType==DT_VPD_SECT_RO && (Flags&DT_VPD_FLAG_RO_WRITE_ALLOWED)==0)
    { 
        DtDbgOutDf(MIN, VPD, pDf, "Cannot delete from RO section");
        return DT_STATUS_FAIL;
    }

    // Initialization of section parameters
    if (DtDfVpd_GetSectionPars(pDf, SectionType, &VpdSectionPars) != DT_STATUS_OK)
    {
        DtDbgOutDf(MIN, VPD, pDf, "Resource %s doesn't exist", VpdSectionPars.m_Name);
        return DT_STATUS_NOT_FOUND;
    }

    // Format section
    memset(VpdSectionPars.m_pStart, 0xFF, VpdSectionPars.m_Length);
  
    // Update Eeprom?
    if (Status==DT_STATUS_OK && UpdateEeprom)
        Status = DtDfVpd_UpdateEepromFromCache(pDf, VpdSectionPars.m_Offset,
                                                                 VpdSectionPars.m_Length);     
    return Status;
}
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_GetEepromSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus DtDfVpd_GetEepromSize(DtDfVpd* pDf, Int* pMemorySize)
{
    Int PageSize;
    DF_VPD_DEFAULT_PRECONDITIONS(pDf);   
    *pMemorySize = 0;
    if (pDf->m_pDfSpiProm != NULL)
    {
        // Get the configured memory size
        if (DtDfSpiProm_GetProperties(pDf->m_pDfSpiProm, pMemorySize, &PageSize) 
                                                                          == DT_STATUS_OK)
            return DT_STATUS_OK;
    }

    DtDbgOutDf(ERR, VPD, pDf, "No Eeprom size found!");
    return DT_STATUS_FAIL;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_GetSectionPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Writes the parameters of section type in pSectionPars.
//
static DtStatus DtDfVpd_GetSectionPars(
    DtDfVpd*  pDf, 
    Int  SectionType,                // Selected section
    DtVpdSectionPars*  pSectionPars) // Parameters of selected section
{
    // Sanity check
    DT_ASSERT(pSectionPars!=NULL);
    switch (SectionType)
    {
    case DT_VPD_SECT_RO:
        pSectionPars->m_Name = "VPD-RO";
        pSectionPars->m_Offset = pDf->m_RoOffset;
        pSectionPars->m_Length = pDf->m_RoSize;
        break;
    case DT_VPD_SECT_RW:
        pSectionPars->m_Name = "VPD-RW";
        pSectionPars->m_Offset = pDf->m_RwOffset;
        pSectionPars->m_Length = pDf->m_RwSize;
        break;
    default:
        pSectionPars->m_Name = "UNKNOWN";
        pSectionPars->m_Length = 0;
    }
    if (pSectionPars->m_Length == 0)
    {
        pSectionPars->m_pStart = NULL;
        pSectionPars->m_pEnd = NULL;
        return DT_STATUS_FAIL;
    }
    pSectionPars->m_pStart = &pDf->m_pCache[pSectionPars->m_Offset];
    pSectionPars->m_pEnd = pSectionPars->m_pStart + pSectionPars->m_Length-1;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_ItemDeleteInSection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfVpd_ItemDeleteInSection(
    DtDfVpd*  pDf, 
    Int  SectionType,
    Int  KeyLen, 
    const char*  pKeyword,
    Int Flags)
{
    DtStatus  Status = DT_STATUS_OK;
    DtVpdSectionPars VpdSectionPars; // Parameters of the resource
    Bool  UpdateEeprom = (Flags&DT_VPD_FLAG_UPDATE_EEPROM) != 0;

    // Sanity check
    DF_VPD_DEFAULT_PRECONDITIONS(pDf);


    // Paramete check
    if (pKeyword==NULL || KeyLen<1 || KeyLen>DT_VPD_MAX_KEY_LENGTH)
    {
        DtDbgOutDf(MIN, VPD, pDf, "Illegal keyword (size: %d)", KeyLen);
        return DT_STATUS_INVALID_PARAMETER;
    }  
    if (SectionType==DT_VPD_SECT_RO && (Flags&DT_VPD_FLAG_RO_WRITE_ALLOWED)==0)
    { 
        DtDbgOutDf(MIN, VPD, pDf, "Cannot delete from RO section");
        return DT_STATUS_FAIL;
    }

    // Initialization of section parameters
    if (DtDfVpd_GetSectionPars(pDf, SectionType, &VpdSectionPars) != DT_STATUS_OK)
    {
        DtDbgOutDf(MIN, VPD, pDf, "Resource %s doesn't exist", VpdSectionPars.m_Name);
        return DT_STATUS_NOT_FOUND;
    }

    // Delete item
    Status = DtDfVpd_DeleteVpdItem(KeyLen, pKeyword, &VpdSectionPars);
    if (Status != DT_STATUS_OK)
        DtDbgOutDf(MIN, VPD, pDf, "Can't find item '%.*s' in %s resource", KeyLen,
                                                         pKeyword, VpdSectionPars.m_Name);
    // Update Eeprom?
    if (Status==DT_STATUS_OK && UpdateEeprom)
        Status = DtDfVpd_UpdateEepromFromCache(pDf, VpdSectionPars.m_Offset,
                                                                 VpdSectionPars.m_Length);

    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_ItemReadFromSection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfVpd_ItemReadFromSection(
    DtDfVpd*  pDf, 
    Int  SectionType,
    Int  KeyLen, 
    const char*  pKeyword,
    Int  BufSize, 
    const UInt8*  pBuf, 
    Int*  pNumRead)
{
    DtStatus  Status = DT_STATUS_OK;
    DtVpdSectionPars VpdSectionPars; // Parameters of the resource
    const UInt8* pFrom;             // Helper pointer to copy from
    UInt8*  pTo;                    // Helper pointer to copy to
    DtVpdItemPars  VpdItemPars;     // Parameters of the found item
    Int  CopySize;                  // Size to copy
    Int  i;                         // Iterator


    // Sanity check
    DF_VPD_DEFAULT_PRECONDITIONS(pDf);

    // Parameter check
    if (pKeyword==NULL || KeyLen<1 || KeyLen>DT_VPD_MAX_KEY_LENGTH)
    {
        DtDbgOutDf(MIN, VPD, pDf, "Illegal keyword (size: %d)", KeyLen);
        return DT_STATUS_INVALID_PARAMETER;
    }
    if (pBuf==NULL || BufSize<0 || pNumRead==NULL)
    {
        DtDbgOutDf(MIN, VPD, pDf, "Invalid VPD read parameters");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Initialization
    DtMemZero((void*)pBuf, BufSize);
    if (DtDfVpd_GetSectionPars(pDf, SectionType, &VpdSectionPars) != DT_STATUS_OK)
    {
        DtDbgOutDf(MIN, VPD, pDf, "Resource %s doesn't exist", VpdSectionPars.m_Name);
        return DT_STATUS_NOT_FOUND;
    }

    // Check whether we can find the item in the resource
    Status = DtDfVpd_SearchVpdItem(KeyLen, pKeyword, &VpdSectionPars, &VpdItemPars);
    if (Status != DT_STATUS_OK)
    {
        // Not found
        DtDbgOutDf(MIN, VPD, pDf, "Can't find item '%.*s' in %s resource", KeyLen, 
                                                        pKeyword, VpdSectionPars.m_Name);
        return DT_STATUS_NOT_FOUND;
    }

    // Copy item to buffer
    pFrom = VpdItemPars.m_pItem;
    pTo = (UInt8*)pBuf;
    *pNumRead = VpdItemPars.m_ItemSize;
    CopySize = BufSize;
    if (CopySize > *pNumRead)
        CopySize = *pNumRead;
    for (i=0; i<CopySize; i++)
        *pTo++ = *pFrom++;
    
    DtDbgOutDf(MAX, VPD, pDf, "Found item '%.*s' in %s resource",
                                                KeyLen, pKeyword, VpdSectionPars.m_Name);       
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_ItemWriteToSection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfVpd_ItemWriteToSection(
    DtDfVpd*  pDf, 
    Int  SectionType,
    Int  KeyLen, 
    const char*  pKeyword,
    Int Flags,
    Int  NumToWrite,
    const UInt8*  pBuf)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool  UpdateEeprom = (Flags&DT_VPD_FLAG_UPDATE_EEPROM) != 0;
    DtVpdSectionPars VpdSectionPars;
    UInt8*  pVpdFree=NULL;
    Int64  FreeSpace;
    Int ItemOverHead;

    // Sanity check
    DF_VPD_DEFAULT_PRECONDITIONS(pDf);

    // Parameter check
    if (pKeyword==NULL || KeyLen<1 || KeyLen>DT_VPD_MAX_KEY_LENGTH)
    {
        DtDbgOutDf(MIN, VPD, pDf, "Illegal keyword (size: %d)", KeyLen);
        return DT_STATUS_INVALID_PARAMETER;
    }
    if (pBuf==NULL || NumToWrite<0 || NumToWrite>DT_VPD_ITEM_MAX_LENGTH)
    {
        DtDbgOutDf(MIN, VPD, pDf, "Invalid VPD write parameters");
        return DT_STATUS_INVALID_PARAMETER;
    }

    if (SectionType==DT_VPD_SECT_RO && (Flags&DT_VPD_FLAG_RO_WRITE_ALLOWED)==0)
    { 
        DtDbgOutDf(MIN, VPD, pDf, "Cannot delete from RO section");
        return DT_STATUS_FAIL;
    }

    // Get the section parameters
    if (DtDfVpd_GetSectionPars(pDf, SectionType, &VpdSectionPars) != DT_STATUS_OK)
    {
        DtDbgOutDf(MIN, VPD, pDf, "Resource %s doesn't exist", VpdSectionPars.m_Name);
        return DT_STATUS_NOT_FOUND;
    }
    
    // Delete item if it exists
    DtDfVpd_DeleteVpdItem(KeyLen, pKeyword, &VpdSectionPars);
    
    // Search free space in resource
    if (DtDfVpd_SearchVpdFree(&VpdSectionPars, (const UInt8**)(&pVpdFree))!=DT_STATUS_OK)
    {
        DtDbgOutDf(MIN, VPD, pDf, "Resource %s has not enough room to store '%.*s' item", 
                                              VpdSectionPars.m_Name,  KeyLen, pKeyword);
        return DT_STATUS_OUT_OF_RESOURCES;
    } 

    // Calculate size of free space
    FreeSpace = (VpdSectionPars.m_pEnd - pVpdFree + 1);

    // Determine item overhead
    ItemOverHead = 1 /*item_state + keyword_Size*/ + 2 /*check_sum*/;
    ItemOverHead += (NumToWrite<=0xFF) ? 1 : 2;   // 1 or 2 byte item_size

    if ((KeyLen+NumToWrite+ItemOverHead) > FreeSpace)
    {
        DtDbgOutDf(MIN, VPD, pDf, "Resource %s has not enough room to store '%.*s' item",
                                                VpdSectionPars.m_Name,  KeyLen, pKeyword);
        return DT_STATUS_OUT_OF_RESOURCES;
    } 
    
    // Write item
    Status = DtDfVpd_WriteVpdItem(pVpdFree, KeyLen, pKeyword, pBuf, NumToWrite);

    // Update Eeprom?
    if (Status==DT_STATUS_OK && UpdateEeprom)
        Status = DtDfVpd_UpdateEepromFromCache(pDf,  VpdSectionPars.m_Offset,
                                                                 VpdSectionPars.m_Length);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_SearchVpdFree -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Search the first byte of free space in given VPD section. Return pointer to
// the free space if found, NULL otherwise. The free space is the first byte that is a
// non-valid item.
//
static DtStatus DtDfVpd_SearchVpdFree(
    const DtVpdSectionPars*  pSectionPars,  // Parameters of selected section
    const UInt8** pFree)                    // Pointer to free space
{
    DtVpdItemPars  VpdItemPars; // VPD item parameters
    const UInt8*  pItem;        // Running item pointer
    
    *pFree = NULL;
   
    // Sanity check
    DT_ASSERT(pSectionPars!=NULL);

    // Initialize
    pItem = pSectionPars->m_pStart;
    
    // Loop through resources
    while (DtDfVpd_CheckVpdItem(pItem, pSectionPars, &VpdItemPars))
        pItem = VpdItemPars.m_pNext;

    // No more valid keywords => found end, assert it is within the resource
    if (pItem > pSectionPars->m_pEnd)
        return DT_STATUS_NOT_FOUND;

    *pFree = pItem;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_SearchVpdItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Keyword-based item search in given VPD section. 
// Returns decodeded item parameters in pVpdItemPars.
//
static DtStatus DtDfVpd_SearchVpdItem(
    Int  KeyLen,                            // Length of Keyword
    const char*  pKeyword,                  // Keyword to be found
    const DtVpdSectionPars*  pSectionPars,  // Parameters of selected section
    DtVpdItemPars*  pVpdItemPars)           // Decodeded item parameters
{
    const UInt8*  pItem;            // Running item pointer
    
    // Sanity check
    DT_ASSERT(pKeyword!=NULL && KeyLen>0 && pSectionPars!=NULL &&  pVpdItemPars!=NULL);

    // Initialize
    pItem = pSectionPars->m_pStart;
    
    // Loop through resources
    while (DtDfVpd_CheckVpdItem(pItem, pSectionPars, pVpdItemPars))
    {
        if (  !(pVpdItemPars->m_IsDeleted)
            && (pVpdItemPars->m_KeywordLen==KeyLen)
            && (strncmp(pVpdItemPars->m_pKeyword, pKeyword, KeyLen)==0) )
        { 
            // Found keyword
            return DT_STATUS_OK;
        }
        pItem = pVpdItemPars->m_pNext;
    }

    // No more valid keywords => can't find keyword
    return DT_STATUS_NOT_FOUND;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_UpdateCacheFromEeprom -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfVpd_UpdateCacheFromEeprom(DtDfVpd* pDf)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    DF_VPD_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_pCache!=NULL && pDf->m_CacheSize>=0);

    // Can we use the SpiProm function?
    if (pDf->m_pDfSpiProm != NULL)
    {
        // Read data
        Status = DtDfSpiProm_Read(pDf->m_pDfSpiProm, 0, pDf->m_CacheSize, pDf->m_pCache);
    }
    else
    {
        // No function found
        DtDbgOutDf(ERR, VPD, pDf, "No Eeprom read function found!");
        Status = DT_STATUS_FAIL;
    }
    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_UpdateEepromFromCache -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus DtDfVpd_UpdateEepromFromCache(DtDfVpd* pDf, Int Offset, Int Length)
{
    DtStatus Status=DT_STATUS_OK, StatusTemp=DT_STATUS_OK;
    Int MemorySize, PageSize, Start;

    // Sanity check
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfVpd) && Offset>=0 && Length>=0);

    // Can we use the SpiProm function?
    if (pDf->m_pDfSpiProm != NULL)
    {
        // Get page size
        Status = DtDfSpiProm_GetProperties(pDf->m_pDfSpiProm, &MemorySize, &PageSize);
        if (PageSize<=0 || !DT_SUCCESS(Status))
        { 
            DtDbgOutDf(ERR, VPD, pDf, "ERROR: Unexpected page size!");
            return DT_STATUS_FAIL;
        }

        // Write entire pages to external EEPROM
        Start = (Offset/PageSize)*PageSize;
        Length = ((Offset + Length + PageSize-1)/PageSize)*PageSize - Start;

        // Unlock device
        Status = DtDfSpiProm_LockDevice(pDf->m_pDfSpiProm, FALSE);
        if (Status == DT_STATUS_OK)
        {
            // Write data
            Status = DtDfSpiProm_Write(pDf->m_pDfSpiProm, Start, Length, 
                                                                     pDf->m_pCache+Start);
            // Lock device
            StatusTemp = DtDfSpiProm_LockDevice(pDf->m_pDfSpiProm, TRUE);
            if (Status == DT_STATUS_OK)
                Status = StatusTemp;
        }
    }
    else
    {
        // No function found
        DtDbgOutDf(ERR, VPD, pDf, "No Eeprom write function found!");
        Status = DT_STATUS_FAIL;
    }
    return Status;
}
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfVpd_WriteVpdItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Unconditionally write a VPD item to VPD data
//
static DtStatus  DtDfVpd_WriteVpdItem(
    const UInt8*  pVpdData,         // Pointer to VPD data for item
    Int KeyLen,                     // Length of Keyword
    const char*  pKeyword,          // String buffer with keyword
    const UInt8*  pItem,            // Buffer with VPD Item
    Int  ItemSize)                  // Size of pItem
{
    UInt16  CheckSum;               // CheckSum
    UInt8 ItemHeader = 0;           // Item header
    UInt8* p = (UInt8*)pVpdData;    // Working  pointer
    Bool ItemSize16Bit = (ItemSize>0x0FF);  // 8 or 16 bit item size?

    // Sanity check
    DT_ASSERT(pVpdData!=NULL && pKeyword!=NULL && KeyLen>0 && pItem!=NULL && ItemSize>=0);

    // keyword_size
    ItemHeader = (KeyLen<<VPD_ITEMTAG_KEYLEN_POS);
    // item_state
    ItemHeader |= (ItemSize16Bit) ? VPD_ITEMTAG_STATE_16B : VPD_ITEMTAG_STATE_8B;
    
    // Write item header
    *p = ItemHeader;
    CheckSum = 0 - (*p++ & ~VPD_ITEMTAG_STATE_MASK);

    // Write keyword
    while (KeyLen--)
    {
        *p = *pKeyword++;
        CheckSum -= *p++;
    }

    // Write item_size
    if (ItemSize16Bit)
    { 
        *p = ItemSize >> 8;
        CheckSum -= *p++;
    }
    *p = ItemSize & 0xFF;
    CheckSum -= *p++;

    if (pItem==NULL) {
        // Write NULL item
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
    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfVpd implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtDf member to DtDfVpd
#define STUB_VPD   ((DtIoStubDfVpd*)pStub)
#define STUB_DF  ((DtDfVpd*)STUB_VPD->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtIoStubDfVpd_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
static DtStatus  DtIoStubDfVpd_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int* pOutSize);
static DtStatus  DtIoStubDfVpd_OnCmdFormat(const DtIoStubDfVpd*,
                                                         const DtIoctlVpdCmdFormatInput*);
static DtStatus  DtIoStubDfVpd_OnCmdGetProperties(const DtIoStubDfVpd*, 
                                                       DtIoctlVpdCmdGetPropertiesOutput*);
static DtStatus  DtIoStubDfVpd_OnCmdItemDelete(const DtIoStubDfVpd*, 
                                                     const DtIoctlVpdCmdItemDeleteInput*);
static DtStatus  DtIoStubDfVpd_OnCmdItemRead(const DtIoStubDfVpd*, 
                                                        const DtIoctlVpdCmdItemReadInput*,
                                                        DtIoctlVpdCmdItemReadOutput*);
static DtStatus  DtIoStubDfVpd_OnCmdItemWrite(const DtIoStubDfVpd*,
                                                      const DtIoctlVpdCmdItemWriteInput*);
static DtStatus  DtIoStubDfVpd_OnCmdRawRead(const DtIoStubDfVpd*,
                                                         const DtIoctlVpdCmdRawReadInput*,
                                                         DtIoctlVpdCmdRawReadOutput*);
static DtStatus  DtIoStubDfVpd_OnCmdRawWrite(const DtIoStubDfVpd*,
                                                       const DtIoctlVpdCmdRawWriteInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_VPD;

static const DtIoctlProperties  IOSTUB_DF_VPD_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubDf_OnExclAccessCmd,
        NULL, NULL),
    DT_IOCTL_PROPS_VPD_CMD(
        DtIoStubDfVpd_OnCmd, 
        DtIoStubDfVpd_AppendDynamicSize, 
        NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfVpd - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfVpd_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubDfVpd_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfVpd));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfVpd_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubDfVpd*  DtIoStubDfVpd_Open(DtDf*  pDf)
{
    DtIoStubDfVpd*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    DF_VPD_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfVpd, pDf, NULL, 
                                                              DtIoStubDfVpd_Close,
                                                              NULL,  // Use default IOCTL
                                                              IOSTUB_DF_VPD_IOCTLS);
    pStub = (DtIoStubDfVpd*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfVpd - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfVpd_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfVpd_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlVpdCmdInput*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfVpd));
    DT_ASSERT(pIoParams != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_VPD_CMD);
    
    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStub(ERR, VPD, pStub, "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_VpdCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_VPD_CMD_ITEM_READ:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlVpdCmdItemReadInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlVpdCmdItemReadOutput));
        // Add dynamic size (i.e. #bytes to read)
        pIoParams->m_OutReqSize += pInData->m_ItemRead.m_MaxNumToRead;
        break;

    case DT_VPD_CMD_ITEM_WRITE:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlVpdCmdItemWriteInput));
        // Add dynamic size (i.e. #bytes to write)
        pIoParams->m_InReqSize += pInData->m_ItemWrite.m_NumToWrite;
        break;
    case DT_VPD_CMD_RAW_READ:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlVpdCmdRawReadInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlVpdCmdRawReadOutput));
        // Add dynamic size (i.e. #bytes to read)
        pIoParams->m_OutReqSize += pInData->m_RawRead.m_NumToRead;
        break;

    case DT_VPD_CMD_RAW_WRITE:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlVpdCmdRawWriteInput));
        // Add dynamic size (i.e. #bytes to write)
        pIoParams->m_InReqSize += pInData->m_RawWrite.m_NumToWrite;
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfVpd_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfVpd_OnCmd(
    const DtIoStub*  pStub, 
    DtIoStubIoParams*  pIoParams, 
    Int* pOutSize)
{
    DtStatus Status= DT_STATUS_OK;
    const DtIoctlVpdCmdInput*  pInData = NULL;
    DtIoctlVpdCmdOutput*  pOutData = NULL;
    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfVpd));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_VPD_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStub(ERR, VPD, pStub, "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_VpdCmd;
    if (pIoParams->m_OutReqSize > 0)
    { 
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_VpdCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_VPD_CMD_FORMAT:
        Status = DtIoStubDfVpd_OnCmdFormat(STUB_VPD, &pInData->m_Format);
        break;
    case DT_VPD_CMD_GET_PROPERTIES:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfVpd_OnCmdGetProperties(STUB_VPD, &pOutData->m_GetProps);
        break;
    case DT_VPD_CMD_ITEM_DELETE:
        Status = DtIoStubDfVpd_OnCmdItemDelete(STUB_VPD, &pInData->m_ItemDelete);
        break;
    case DT_VPD_CMD_ITEM_READ:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfVpd_OnCmdItemRead(STUB_VPD, &pInData->m_ItemRead, 
                                                                   &pOutData->m_ItemRead);
        break;
    case DT_VPD_CMD_ITEM_WRITE:
        Status = DtIoStubDfVpd_OnCmdItemWrite(STUB_VPD, &pInData->m_ItemWrite);
        break;
    case DT_VPD_CMD_RAW_READ:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfVpd_OnCmdRawRead(STUB_VPD, &pInData->m_RawRead,
                                                                    &pOutData->m_RawRead);
        break;
    case DT_VPD_CMD_RAW_WRITE:
        Status = DtIoStubDfVpd_OnCmdRawWrite(STUB_VPD, &pInData->m_RawWrite);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfVpd_OnCmdFormat -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfVpd_OnCmdFormat(
    const DtIoStubDfVpd*  pStub, 
    const DtIoctlVpdCmdFormatInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfVpd));
    DT_ASSERT(pInData != NULL);

    return DtDfVpd_Format(STUB_DF,  pInData->m_SectionType, pInData->m_Flags);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfVpd_OnCmdGetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfVpd_OnCmdGetProperties(
    const DtIoStubDfVpd*  pStub, 
    DtIoctlVpdCmdGetPropertiesOutput*  pOutData)
{

    DtDfVpdProperties VpdProperties;
    DtStatus Status;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfVpd));
    DT_ASSERT(pOutData != NULL);

    Status = DtDfVpd_GetProperties(STUB_DF, &VpdProperties);
    
    // Copy the properties to pOutData
    pOutData->m_RoOffset = VpdProperties.m_RoOffset;
    pOutData->m_RoSize = VpdProperties.m_RoSize;
    pOutData->m_RwOffset = VpdProperties.m_RwOffset;
    pOutData->m_RwSize = VpdProperties.m_RwSize;
    pOutData->m_EepromSize = VpdProperties.m_EepromSize;
    pOutData->m_MaxItemLength = VpdProperties.m_MaxItemLength;

    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfVpd_OnCmdItemDelete -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfVpd_OnCmdItemDelete(
    const DtIoStubDfVpd*  pStub, 
    const DtIoctlVpdCmdItemDeleteInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfVpd));
    DT_ASSERT(pInData != NULL);

    return DtDfVpd_ItemDelete(STUB_DF, pInData->m_SectionType, pInData->m_KeywordLen, 
                                                                    pInData->m_Keyword, 
                                                                    pInData->m_Flags);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfVpd_OnCmdItemRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfVpd_OnCmdItemRead(
    const DtIoStubDfVpd*  pStub,
    const DtIoctlVpdCmdItemReadInput*  pInData,
    DtIoctlVpdCmdItemReadOutput*  pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfVpd));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    pOutData->m_NumRead = 0;  // Assume the worst
    return DtDfVpd_ItemRead(STUB_DF, pInData->m_SectionType, pInData->m_KeywordLen, 
                                                                  pInData->m_Keyword,
                                                                  pInData->m_Flags,
                                                                  pInData->m_MaxNumToRead,
                                                                  pOutData->m_Buf,
                                                                  &pOutData->m_NumRead);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfVpd_OnCmdItemWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfVpd_OnCmdItemWrite(
    const DtIoStubDfVpd*  pStub,
    const DtIoctlVpdCmdItemWriteInput*  pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfVpd));
    DT_ASSERT(pInData != NULL);

    return DtDfVpd_ItemWrite(STUB_DF, pInData->m_SectionType,  pInData->m_KeywordLen,
                                                                   pInData->m_Keyword,
                                                                   pInData->m_Flags,
                                                                   pInData->m_NumToWrite,
                                                                   pInData->m_Buf);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfVpd_OnCmdRawRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfVpd_OnCmdRawRead(
    const DtIoStubDfVpd*  pStub,
    const DtIoctlVpdCmdRawReadInput*  pInData,
    DtIoctlVpdCmdRawReadOutput*  pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfVpd));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    pOutData->m_NumRead = 0;  // Assume the worst
    return DtDfVpd_RawRead(STUB_DF, pInData->m_StartOffset,  pInData->m_NumToRead,
                                                   pOutData->m_Buf, &pOutData->m_NumRead);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfVpd_OnCmdRawWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfVpd_OnCmdRawWrite(
    const DtIoStubDfVpd*  pStub,
    const DtIoctlVpdCmdRawWriteInput*  pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfVpd));
    DT_ASSERT(pInData != NULL);

    return DtDfVpd_RawWrite(STUB_DF, pInData->m_StartOffset,
                                                   pInData->m_NumToWrite, pInData->m_Buf);
}
