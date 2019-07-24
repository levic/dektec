//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSpiProm.c *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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
#include "DtDfSpiProm.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

//SPI Master role
#define SPIM_ROLE_NONE        NULL

// SPI PROM Commands
#define SPI_PROM_MAX_CMD_SIZE 5       // Maximum command size

#define SPI_PROM_CMD_READ     0       // Read command
#define SPI_PROM_CMD_WRITE    1       // Write command
#define SPI_PROM_CMD_LOCK     2       // Lock command
#define SPI_PROM_CMD_UNLOCK   3       // Unlock command
#define SPI_PROM_CMD_WRENA    4       // Write enable command
#define SPI_PROM_CMD_WRDISA   5       // Write disable command
#define SPI_PROM_CMD_GETWRIP  6       // Get write in-progress command
#define SPI_PROM_CMD_COUNT    7       // Number of SPI-prom commands



// Structure for storing  a command
struct DtDfSpiPromCommand
{
    Int  m_TotalLength;     // Total command message length
    Int  m_SeqLength;       // Length of the command sequence     
    const UInt8 m_CommandSeq[SPI_PROM_MAX_CMD_SIZE];
};

// Structure for storing  the SPI-Prom properties
struct DtDfSpiPromProperties
{
    Int  m_MemorySize;      // Prom memory size
    Int  m_PageSize;        // Prom page size
    Int  m_WriteTimeout;    // Write timeout in ms
    Int  m_WriteInProgressFlag; // Write in progress flag
};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- 25AA160C EEPROM -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define CFG_25AA160C_SIZE    2048   // 2K byte Eeprom
#define CFG_25AA160C_PAGE    16     // 16 byte pages
#define CFG_25AA160C_WR_TIMEOUT  5  // Write timeout 5 milli seconds
#define STAT_25AA160C_WRIP_FLAG 0x01 // Write in progress flag

#define CMD_25AA160C_READ    0x03   // Read data from memory beginning at selected address
#define CMD_25AA160C_WRITE   0x02   // Write data to memory beginning at selected address
#define CMD_25AA160C_WRDI    0x04   // Disable write operations
#define CMD_25AA160C_WREN    0x06   // Enable write operations 
#define CMD_25AA160C_RDSR    0x05   // Read status register
#define CMD_25AA160C_WRSR    0x01   // Write status register 

// 25AA160C EEPROM properties
const struct DtDfSpiPromProperties SPI_25AA160C_Props =
{
    CFG_25AA160C_SIZE,              // Prom memory size
    CFG_25AA160C_PAGE,              // Prom page size
    CFG_25AA160C_WR_TIMEOUT,        // Write timeout
    STAT_25AA160C_WRIP_FLAG         // Write in progress flag
};

// 25AA160C EEPROM commands
const struct DtDfSpiPromCommand SPI_25AA160C_Cmds[SPI_PROM_CMD_COUNT] =
{
    { 3, 1, { CMD_25AA160C_READ } },           // Read command
    { 3, 1, { CMD_25AA160C_WRITE } },          // Write command
    { 2, 2, { CMD_25AA160C_WRSR, 0X0C } },     // Lock command
    { 2, 2, { CMD_25AA160C_WRSR, 0X00 } },     // Unlock command
    { 1, 1, { CMD_25AA160C_WREN } },           // Write enable command
    { 1, 1, { CMD_25AA160C_WRDI } },           // Write disable command
    { 1, 1, { CMD_25AA160C_RDSR } },           // Read status register 
};


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSpiProm implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtDfSpiProm function
#define DF_SPIPROM_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfSpiProm))


// Helper macro to cast a DtDf* to a DtDfSpiProm*
#define DF_SPIPROM      ((DtDfSpiProm*)pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfSpiProm_CreateCmdMessage(DtDfSpiProm*, Int Command, Int AddData,
                                                            Int* pLength, UInt8* pBuffer);
static DtStatus  DtDfSpiProm_Init(DtDf*);
static DtStatus  DtDfSpiProm_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfSpiProm_OpenChildren(DtDfSpiProm*);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSpiProm - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiProm_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfSpiProm_Close(DtDf*  pDf)
{
    DF_SPIPROM_DEFAULT_PRECONDITIONS(pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiProm_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfSpiProm*  DtDfSpiProm_Open(DtCore*  pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_DF_SPIPROM_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfSpiProm, Id, DT_FUNC_TYPE_SPIPROM, pPt,
                                                                       CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfSpiProm_Close;
    OpenParams.m_InitFunc = DtDfSpiProm_Init;
    OpenParams.m_OnCloseFileFunc = DtDfSpiProm_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfSpiProm*)DtDf_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiProm_GetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSpiProm_GetProperties(DtDfSpiProm* pDf, Int* pPromSize, Int* pPageSize)
{
    // Sanity checks
    DF_SPIPROM_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pPromSize!=NULL && pPageSize!=NULL);
    DT_ASSERT(pDf->m_pSpiPromProperties != NULL);
    if (pDf->m_pSpiPromProperties == NULL)
        return DT_STATUS_NOT_INITIALISED;

    // Copy the properties
    *pPromSize = pDf->m_pSpiPromProperties->m_MemorySize;
    *pPageSize  = pDf->m_pSpiPromProperties->m_PageSize;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiProm_LockDevice -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSpiProm_LockDevice(DtDfSpiProm* pDf, Bool Lock)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8 CmdBuffer[SPI_PROM_MAX_CMD_SIZE];
    Int CmdLength = sizeof(CmdBuffer);
    Int TimeoutCnt;

    // Sanity checks
    DF_SPIPROM_DEFAULT_PRECONDITIONS(pDf);

    DtDbgOutDf(MAX, SPIPROM, pDf, "Lock device: %d", Lock);

    // Protect SPI-Prom against concurrent access
    DtFastMutexAcquire(&pDf->m_AccessMutex);

    // Enable writing
    CmdLength = sizeof(CmdBuffer);
    Status = DtDfSpiProm_CreateCmdMessage(pDf, SPI_PROM_CMD_WRENA, 0,
                                                                   &CmdLength, CmdBuffer);
    if (DT_SUCCESS(Status))
        Status = DtBcSPIM_Write(pDf->m_pBcSpiM, CmdLength, CmdBuffer);

    // Create lock/unlock  command message
    CmdLength = sizeof(CmdBuffer);
    if (DT_SUCCESS(Status))
        Status = DtDfSpiProm_CreateCmdMessage(pDf, Lock ? SPI_PROM_CMD_LOCK :
                                           SPI_PROM_CMD_UNLOCK, 0, &CmdLength, CmdBuffer);
    // Write lock/unlock command
    if (DT_SUCCESS(Status))
        Status = DtBcSPIM_Write(pDf->m_pBcSpiM, CmdLength, CmdBuffer);
    
    // Create get-write-in-progress command message
    CmdLength = sizeof(CmdBuffer);
    if (DT_SUCCESS(Status))
        Status = DtDfSpiProm_CreateCmdMessage(pDf, SPI_PROM_CMD_GETWRIP, 0, &CmdLength, 
                                                                                CmdBuffer);
    // Wait for write completed
    for (TimeoutCnt=0; TimeoutCnt<=pDf->m_pSpiPromProperties->m_WriteTimeout
                                                      && DT_SUCCESS(Status); TimeoutCnt++)
    {
        UInt8 RespBuffer;
        DtWaitBlock(1000);
        Status = DtBcSPIM_WriteRead(pDf->m_pBcSpiM, CmdLength, CmdBuffer, 1, &RespBuffer);
        // Write completed?
        if ((RespBuffer & pDf->m_pSpiPromProperties->m_WriteInProgressFlag) == 0)
            break;
    }
    // Timeout?
    if (DT_SUCCESS(Status) && TimeoutCnt>pDf->m_pSpiPromProperties->m_WriteTimeout)
        Status = DT_STATUS_TIMEOUT;

    // Release SPI-Prom mutex
    DtFastMutexRelease(&pDf->m_AccessMutex);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiProm_Read -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSpiProm_Read(DtDfSpiProm* pDf, Int StartAddress, Int NumToRead, UInt8* pBuf)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8 CmdBuffer[SPI_PROM_MAX_CMD_SIZE];
    Int CmdLength = sizeof(CmdBuffer);

    // Sanity checks
    DF_SPIPROM_DEFAULT_PRECONDITIONS(pDf);
    
    // Check parameters
    DT_ASSERT(pDf->m_pSpiPromProperties != NULL);
    if (StartAddress+NumToRead > pDf->m_pSpiPromProperties->m_MemorySize)
        return DT_STATUS_INVALID_PARAMETER;

    DtDbgOutDf(MAX, SPIPROM, pDf, "Read from Address: %d, NumToRead: %d", 
                                                                 StartAddress, NumToRead);
    // Create read command message
    DT_RETURN_ON_ERROR(DtDfSpiProm_CreateCmdMessage(pDf, SPI_PROM_CMD_READ, StartAddress, 
                                                                  &CmdLength, CmdBuffer));

    // Protect SPI-Prom against concurrent access
    DtFastMutexAcquire(&pDf->m_AccessMutex);
    
    // Write read command and read back data
    Status = DtBcSPIM_WriteRead(pDf->m_pBcSpiM, CmdLength, CmdBuffer, NumToRead, pBuf);

    // Release SPI-Prom mutex
    DtFastMutexRelease(&pDf->m_AccessMutex);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiProm_Write -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSpiProm_Write(DtDfSpiProm* pDf, Int StartAddress, Int NumToWrite,
                                                                        const UInt8* pBuf)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8 CmdBuffer[SPI_PROM_MAX_CMD_SIZE];
    Int CmdLength = sizeof(CmdBuffer);
    Int PageSize, TimeoutCnt;

    // Sanity checks
    DF_SPIPROM_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    // Write only complete pages
    DT_ASSERT(pDf->m_pSpiPromProperties != NULL);
    PageSize = pDf->m_pSpiPromProperties->m_PageSize;
    if ((StartAddress%PageSize)!=0 || (NumToWrite%PageSize)!=0)
        return DT_STATUS_INVALID_PARAMETER;

    if (StartAddress+NumToWrite > pDf->m_pSpiPromProperties->m_MemorySize)
        return DT_STATUS_INVALID_PARAMETER;

    DtDbgOutDf(MAX, SPIPROM, pDf, "Write Address: %d, NumToWrite: %d", 
                                                                StartAddress, NumToWrite);
    // Protect SPI-Prom against concurrent access
    DtFastMutexAcquire(&pDf->m_AccessMutex);

    // Write pages
    while (NumToWrite>0 && DT_SUCCESS(Status))
    {
        // Enable writing
        CmdLength = sizeof(CmdBuffer);
        Status = DtDfSpiProm_CreateCmdMessage(pDf, SPI_PROM_CMD_WRENA, 0,
                                                                   &CmdLength, CmdBuffer);
        if (DT_SUCCESS(Status))
            Status = DtBcSPIM_Write(pDf->m_pBcSpiM, CmdLength, CmdBuffer);

        // Create write command message
        CmdLength = sizeof(CmdBuffer);
        if (DT_SUCCESS(Status))
            Status =  DtDfSpiProm_CreateCmdMessage(pDf, SPI_PROM_CMD_WRITE,
                                                     StartAddress, &CmdLength, CmdBuffer);
        // Write page of data
        if (DT_SUCCESS(Status))
            Status =  DtBcSPIM_WriteWrite(pDf->m_pBcSpiM, CmdLength, CmdBuffer,
                                                                          PageSize, pBuf);

        // Create get-write-in-progress command message
        CmdLength = sizeof(CmdBuffer);
        if (DT_SUCCESS(Status))
            Status = DtDfSpiProm_CreateCmdMessage(pDf, SPI_PROM_CMD_GETWRIP,
                                                                0, &CmdLength, CmdBuffer);
        // Wait for write completed
        for (TimeoutCnt=0; TimeoutCnt<=pDf->m_pSpiPromProperties->m_WriteTimeout
                                                      && DT_SUCCESS(Status); TimeoutCnt++)
        {
            UInt8 RespBuffer;
            DtWaitBlock(1000);
            Status = DtBcSPIM_WriteRead(pDf->m_pBcSpiM, CmdLength, CmdBuffer,
                                                                          1, &RespBuffer);
            // Write completed?
            if ((RespBuffer & pDf->m_pSpiPromProperties->m_WriteInProgressFlag) == 0)
                break;
        }
        // Timeout?
        if (DT_SUCCESS(Status) && TimeoutCnt>pDf->m_pSpiPromProperties->m_WriteTimeout)
            Status = DT_STATUS_TIMEOUT;
        else
        {
            // Next page
            pBuf +=  PageSize;
            StartAddress +=  PageSize;
            NumToWrite -= PageSize;
        }
    }

    // Disable writing
    CmdLength = sizeof(CmdBuffer);
    if (DT_SUCCESS(DtDfSpiProm_CreateCmdMessage(pDf, SPI_PROM_CMD_WRDISA, 0, &CmdLength,
                                                                              CmdBuffer)))
        DtBcSPIM_Write(pDf->m_pBcSpiM, CmdLength, CmdBuffer);

    // Release SPI-Prom mutex
    DtFastMutexRelease(&pDf->m_AccessMutex);

    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSpiProm - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiProm_CreateCmdMessage -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSpiProm_CreateCmdMessage(DtDfSpiProm* pDf, Int Command, Int Address,
                                                             Int* pLength, UInt8* pBuffer)
{
    Int Idx;
    DT_ASSERT(pDf->m_pSpiPromCommands != NULL);
    DT_ASSERT(Command>=0 && Command<SPI_PROM_CMD_COUNT);
    DT_ASSERT(*pLength >= pDf->m_pSpiPromCommands[Command].m_TotalLength);
   
    if (pDf->m_pSpiPromCommands == NULL)
        return DT_STATUS_NOT_INITIALISED;

    switch (Command)
    {
        // Read/write data command
    case SPI_PROM_CMD_READ:
    case SPI_PROM_CMD_WRITE:
        DtMemCopy(pBuffer, pDf->m_pSpiPromCommands[Command].m_CommandSeq,
                                            pDf->m_pSpiPromCommands[Command].m_SeqLength);
        // Thereafter Address, MSB first
        Idx = pDf->m_pSpiPromCommands[Command].m_SeqLength;
        pBuffer[Idx++] = (Address>>8)&0xFF;
        pBuffer[Idx] = Address&0xFF;
        break;

        // Other commands
    default:
        DtMemCopy(pBuffer, pDf->m_pSpiPromCommands[Command].m_CommandSeq,
                                            pDf->m_pSpiPromCommands[Command].m_SeqLength);
        break;
    }

    // Set command length
    *pLength = pDf->m_pSpiPromCommands[Command].m_TotalLength;

    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiProm_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSpiProm_Init(DtDf*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_SPIPROM_DEFAULT_PRECONDITIONS(pDf);

    // Initialize SPI mutex against concurrent access
    DtFastMutexInit(&DF_SPIPROM->m_AccessMutex);


    // SPI-EEPROM is not yet known
    DF_SPIPROM->m_pSpiPromProperties = NULL;
    DF_SPIPROM->m_pSpiPromCommands = NULL;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfSpiProm_OpenChildren(DF_SPIPROM);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SPIPROM, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }
  
    // Determine the SPI-EEPROM properties  and commands to be used
    switch (DF_SPIPROM->m_pBcSpiM->m_DeviceId)
    {
    case DT_SPIM_SPIDVC_25AA160C:
        DF_SPIPROM->m_pSpiPromProperties = &SPI_25AA160C_Props;
        DF_SPIPROM->m_pSpiPromCommands = SPI_25AA160C_Cmds;
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiProm_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtDfSpiProm_OnCloseFile(DtDf* pDf, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    // Sanity checks
    DF_SPIPROM_DEFAULT_PRECONDITIONS(pDf);

    // Check if the owner closed the file handle
    Status = DtDf_ExclAccessCheck(pDf, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutDf(AVG, SPIPROM, pDf, "Lock device");
        DtDfSpiProm_LockDevice(DF_SPIPROM, TRUE);

    }
    // Use base function to release exclusive access
    return DtDf_OnCloseFile(pDf, pFile);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiProm_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSpiProm_OpenChildren(DtDfSpiProm*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

     // List of children supported by the the SPIPROM function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_SPIM, DT_BC_SPIM_NAME, 
                               SPIM_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcSpiM), TRUE}
    };

    DF_SPIPROM_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
     if (!DT_SUCCESS(Status))
        return Status;

    // Check mandatory children have been loaded (i.e. shortcut is valid)
    DT_ASSERT(pDf->m_pBcSpiM != NULL);
    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfSpiProm implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtDfSpiProm
#define SPIPROM_STUB   ((DtIoStubDfSpiProm*)pStub)
#define SPIPROM_DF ((DtDfSpiProm*)SPIPROM_STUB->m_pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubDfSpiProm_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
static DtStatus  DtIoStubDfSpiProm_OnCmd(const DtIoStub*, DtIoStubIoParams*, 
                                                                           Int* pOutSize);
static DtStatus  DtIoStubDfSpiProm_OnCmdGetProperties(const DtIoStubDfSpiProm*, 
                                                   DtIoctlSpiPromCmdGetPropertiesOutput*);
static DtStatus  DtIoStubDfSpiProm_OnCmdLockDevice(const DtIoStubDfSpiProm*, Int  Cmd);
static DtStatus  DtIoStubDfSpiProm_OnCmdRead(const DtIoStubDfSpiProm*, 
                                                        const DtIoctlSpiPromCmdReadInput*,
                                                        DtIoctlSpiPromCmdReadOutput*);
static DtStatus  DtIoStubDfSpiProm_OnCmdWrite(const DtIoStubDfSpiProm*, 
                                                      const DtIoctlSpiPromCmdWriteInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_SPIPROM;

static const DtIoctlProperties  IOSTUB_DF_SPIPROM_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubDf_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_SPIPROM_CMD(
        DtIoStubDfSpiProm_OnCmd,
        DtIoStubDfSpiProm_AppendDynamicSize,
        NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfSpiProm - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSpiProm_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubDfSpiProm_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSpiProm));

    // Let base function perform final clean-up
    DtIoStubDf_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSpiProm_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubDfSpiProm*  DtIoStubDfSpiProm_Open(DtDf*  pDf)
{
    DtIoStubDfSpiProm*  pStub = NULL;
    DtIoStubDfOpenParams  OpenParams;

    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfSpiProm));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBDF_INIT_OPEN_PARAMS(OpenParams, DtIoStubDfSpiProm, pDf, NULL, 
                                                              DtIoStubDfSpiProm_Close,
                                                              NULL,  // Use default IOCTL
                                                              IOSTUB_DF_SPIPROM_IOCTLS);
    pStub = (DtIoStubDfSpiProm*)DtIoStubDf_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfSpiProm - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSpiProm_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSpiProm_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSpiPromCmdInput*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSpiProm));
    DT_ASSERT(pIoParams != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_SPIMF_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SpiPromCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_SPIMF_CMD_READ:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlSpiPromCmdReadInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlSpiPromCmdReadOutput));
        // Add dynamic size (i.e. #bytes to read)
        pIoParams->m_OutReqSize += pInData->m_Read.m_NumToRead;
        break;

    case DT_SPIMF_CMD_WRITE:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlSpiPromCmdWriteInput));
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSpiProm_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSpiProm_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSpiPromCmdInput*  pInData = NULL;
    DtIoctlSpiPromCmdOutput*  pOutData = NULL;
    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSpiProm));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_SPIPROM_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtDf_ExclAccessCheck(((DtIoStubDf*)pStub)->m_pDf, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubDf(ERR, SPIPROM, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SpiPromCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_SpiPromCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_SPIPROM_CMD_GET_PROPERTIES:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSpiProm_OnCmdGetProperties(SPIPROM_STUB, 
                                                              &pOutData->m_GetProperties);
        break;
    case DT_SPIPROM_CMD_LOCK_DEVICE:
        Status = DtIoStubDfSpiProm_OnCmdLockDevice(SPIPROM_STUB, pInData->m_Lock.m_CmdEx);
        break;
    case DT_SPIPROM_CMD_READ:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubDfSpiProm_OnCmdRead(SPIPROM_STUB, 
                                                     &pInData->m_Read, &pOutData->m_Read);
        break;
    case DT_SPIPROM_CMD_WRITE:
        Status = DtIoStubDfSpiProm_OnCmdWrite(SPIPROM_STUB, &pInData->m_Write);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSpiProm_OnCmdGetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSpiProm_OnCmdGetProperties(
    const DtIoStubDfSpiProm*  pStub, 
    DtIoctlSpiPromCmdGetPropertiesOutput*  pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSpiProm));
    DT_ASSERT(pOutData != NULL);

    return DtDfSpiProm_GetProperties(SPIPROM_DF, &pOutData->m_MemorySize,
                                                                   &pOutData->m_PageSize);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSpiProm_OnCmdLockDevice -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfSpiProm_OnCmdLockDevice(
    const DtIoStubDfSpiProm*  pStub,
    Int  Cmd)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSpiProm));
    DT_ASSERT(Cmd==DT_SPIPROM_CMDEX_LOCK|| Cmd==DT_SPIPROM_CMDEX_UNLOCK);

    return DtDfSpiProm_LockDevice(SPIPROM_DF, (Cmd==DT_SPIPROM_CMDEX_LOCK));
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSpiProm_OnCmdRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubDfSpiProm_OnCmdRead(
    const DtIoStubDfSpiProm*  pStub, 
    const DtIoctlSpiPromCmdReadInput*  pInData,
    DtIoctlSpiPromCmdReadOutput*  pOutData)
{

    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSpiProm));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    // Assume the worst
    pOutData->m_NumBytes = 0;
    Status = DtDfSpiProm_Read(SPIPROM_DF, pInData->m_StartAddress, pInData->m_NumToRead, 
                                                                         pOutData->m_Buf);
    // If succesful, set number of bytes read
    if (DT_SUCCESS(Status))
        pOutData->m_NumBytes = pInData->m_NumToRead;
    return  Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfSpiProm_OnCmdWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubDfSpiProm_OnCmdWrite(
    const DtIoStubDfSpiProm*  pStub, 
    const DtIoctlSpiPromCmdWriteInput*  pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubDfSpiProm));
    DT_ASSERT(pInData != NULL);

    return DtDfSpiProm_Write(SPIPROM_DF, pInData->m_StartAddress, pInData->m_NumToWrite, 
                                                                          pInData->m_Buf);
}