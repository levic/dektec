//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSPIM.c *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
#include "DtBc.h"
#include "DtBcSPIM.h"
#include "DtBcSPIM_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSPIM implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_SPIM_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIM))

// Helper macro to cast a DtBc* to a DtBcSPIM*
#define BC_SPIM         ((DtBcSPIM*)pBc)


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcSPIM_Init(DtBc*);
static DtStatus  DtBcSPIM_OnEnable(DtBc*, Bool  Enable);
static DtStatus  DtBcSPIM_CheckDeviceStatus(DtBcSPIM* pBc);
static DtStatus  DtBcSPIM_TransferData(DtBcSPIM* pBc, Int Direction, Bool Continue);
DT_UNUSED static const char*  DtBcSPIM_DeviceIdToString(Int  Id);
DT_UNUSED static const char*  DtBcSPIM_DuplexModeToString(Int  Mode);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSPIM - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIM_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSPIM_Close(DtBc*  pBc)
{
    BC_SPIM_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIM_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcSPIM*  DtBcSPIM_Open(Int  Address, DtCore*  pCore, DtPt* pPt,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    // Init open parameters
    DT_BC_SPIM_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcSPIM, Id, DT_BLOCK_TYPE_SPIM, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcSPIM_Close;
    OpenParams.m_InitFunc = DtBcSPIM_Init;
    OpenParams.m_OnEnableFunc = DtBcSPIM_OnEnable;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcSPIM*)DtBc_Open(&OpenParams);
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIM_GetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSPIM_GetProperties(DtBcSPIM* pBc, Int * pDeviceId, Int* pDuplexMode,
                                                      Int* pMaxTfTime, Int* pSpiClockRate)
{
    // Sanity check    
    BC_SPIM_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pDeviceId==NULL || pDuplexMode==NULL || pMaxTfTime==NULL || pSpiClockRate==NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Doesn't have to be enabled

    // Copy cached properties
    *pDeviceId = pBc->m_DeviceId;
    *pDuplexMode = pBc->m_DuplexMode;
    *pMaxTfTime = pBc->m_MaxTransferTime;
    *pSpiClockRate = pBc->m_SpiClockRate;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIM_Read -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSPIM_Read(DtBcSPIM* pBc, Int Length, UInt8* pBuffer)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check    
    BC_SPIM_DEFAULT_PRECONDITIONS(pBc);

    // Doesn't have to be enabled

    DtDbgOutBc(MAX, SPIM, pBc, "Read Length: %d", Length);

    // Protect SPI access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Check device status
    Status = DtBcSPIM_CheckDeviceStatus(pBc);
    if (!DT_SUCCESS(Status))
    {
        // Release SPI mutex
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return  Status;
    }

    // Read data
    while (Length > 0)
    {
        Int i;
        UInt32 Data;

        // Tranfer data
        Status = DtBcSPIM_TransferData(pBc, SPIM_TFDIR_RECEIVE,
                                                         Length > pBc->m_NumBytesPerWord);
        if (!DT_SUCCESS(Status))
        {
            // Release SPI mutex
            DtFastMutexRelease(&pBc->m_AccessMutex);
            return  Status;
        }

        // Read transfered word and copy to read buffer 
        Data = SPIM_ReceiveData_READ(pBc);
        for (i=0; i<pBc->m_NumBytesPerWord && Length>0; i++)
        {
            *pBuffer++ = Data&0xFF;
            Data >>= 8;
            Length--;
        }
    }

    // Release SPI mutex
    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIM_Write -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSPIM_Write(DtBcSPIM* pBc, Int Length, const UInt8* pBuffer)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check    
    BC_SPIM_DEFAULT_PRECONDITIONS(pBc);

    // Doesn't have to be enabled

    DtDbgOutBc(MAX, SPIM, pBc, "Write Length: %d", Length);

    // Protect SPI access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Check device status
    Status = DtBcSPIM_CheckDeviceStatus(pBc);
    if (!DT_SUCCESS(Status))
    {
        // Release SPI mutex
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return  Status;
    }

    // Write data
    while (Length > 0)
    {
        Int i;
        UInt32 Data = 0;
              
        // Write word that needs to be transfered
        for (i=0; i<pBc->m_NumBytesPerWord && Length>0; i++)
        {
            Data |= ((*pBuffer++)<<(i*8));
            Length--;
        }
        SPIM_TransmitData_WRITE(pBc, Data);

        // Tranfer data
        Status = DtBcSPIM_TransferData(pBc, SPIM_TFDIR_SEND, Length > 0);
        if (!DT_SUCCESS(Status))
        {
            // Release SPI mutex
            DtFastMutexRelease(&pBc->m_AccessMutex);
            return  Status;
        }
    }

    // Release SPI mutex
    DtFastMutexRelease(&pBc->m_AccessMutex);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIM_WriteRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSPIM_WriteRead(DtBcSPIM* pBc, Int WriteLength, const UInt8* pWriteBuffer,
                                                       Int ReadLength, UInt8* pReadBuffer)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check    
    BC_SPIM_DEFAULT_PRECONDITIONS(pBc);

    // Doesn't have to be enabled

    DtDbgOutBc(MAX, SPIM, pBc, "Write Length: %d  Read Length: %d", 
                                                                 WriteLength, ReadLength);

    // Protect SPI access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Check device status
    Status = DtBcSPIM_CheckDeviceStatus(pBc);
    if (!DT_SUCCESS(Status))
    {
        // Release SPI mutex
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return  Status;
    }

    // Write data
    while (WriteLength > 0)
    {
        Int i;
        UInt32 Data = 0;
              
        // Write word that needs to be transfered
        for (i=0; i<pBc->m_NumBytesPerWord && WriteLength>0; i++)
        {
            Data |= ((*pWriteBuffer++)<<(i*8));
            WriteLength--;
        }
        SPIM_TransmitData_WRITE(pBc, Data);

        // Tranfer data
        Status = DtBcSPIM_TransferData(pBc, SPIM_TFDIR_SEND, 
                                                           WriteLength>0 || ReadLength>0);
        if (!DT_SUCCESS(Status))
        {
            // Release SPI mutex
            DtFastMutexRelease(&pBc->m_AccessMutex);
            return  Status;
        }
    }

    // Read data
    while (ReadLength > 0)
    {
        Int i;
        UInt32 Data;

        // Tranfer data
        Status = DtBcSPIM_TransferData(pBc, SPIM_TFDIR_RECEIVE,
                                                     ReadLength > pBc->m_NumBytesPerWord);
        if (!DT_SUCCESS(Status))
        {
            // Release SPI mutex
            DtFastMutexRelease(&pBc->m_AccessMutex);
            return  Status;
        }

        // Read transfered word and copy to read buffer 
        Data = SPIM_ReceiveData_READ(pBc);
        for (i=0; i<pBc->m_NumBytesPerWord && ReadLength>0; i++)
        {
            *pReadBuffer++ = Data&0xFF;
            Data >>= 8;
            ReadLength--;
        }
    }

    // Release SPI mutex
    DtFastMutexRelease(&pBc->m_AccessMutex);

    return Status;
}



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIM_WriteWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSPIM_WriteWrite(DtBcSPIM* pBc, Int WriteLength1, const UInt8* pWriteBuffer1,
                                             Int WriteLength2, const UInt8* pWriteBuffer2)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check    
    BC_SPIM_DEFAULT_PRECONDITIONS(pBc);

    // Doesn't have to be enabled

    DtDbgOutBc(MAX, SPIM, pBc, "Write Length1: %d  Write Length2: %d", 
                                                              WriteLength1, WriteLength2);

    // Protect SPI access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Check device status
    Status = DtBcSPIM_CheckDeviceStatus(pBc);
    if (!DT_SUCCESS(Status))
    {
        // Release SPI mutex
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return  Status;
    }

    // Write data in two parts
    while (WriteLength1>0 || WriteLength2>0)
    {
        Int i;
        UInt32 Data = 0;         
        // Write word that needs to be transfered
        for (i=0; i<pBc->m_NumBytesPerWord && (WriteLength1>0 || WriteLength2>0); i++)
        {
            if (WriteLength1 > 0)
            {
                Data |= ((*pWriteBuffer1++)<<(i*8));
                WriteLength1--;
            } else {
                Data |= ((*pWriteBuffer2++)<<(i*8));
                WriteLength2--;
            }
        }
        SPIM_TransmitData_WRITE(pBc, Data);

        // Tranfer data
        Status = DtBcSPIM_TransferData(pBc, SPIM_TFDIR_SEND, WriteLength1>0
                                                                       || WriteLength2>0);
        if (!DT_SUCCESS(Status))
        {
            // Release SPI mutex
            DtFastMutexRelease(&pBc->m_AccessMutex);
            return  Status;
        }
    }

    // Release SPI mutex
    DtFastMutexRelease(&pBc->m_AccessMutex);

    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSPIM - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIM_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSPIM_Init(DtBc*  pBc)
{
    UInt32 RegConfig;

    // Initialize SPI mutex against concurrent access
    DtFastMutexInit(&BC_SPIM->m_AccessMutex);

    // Get device ID. 
    RegConfig = (Int)SPIM_Config1_READ_DeviceId(BC_SPIM);
    switch (RegConfig)
    {
    case SPIM_SPIDVC_25AA160C:  BC_SPIM->m_DeviceId = DT_SPIM_SPIDVC_25AA160C; break;
    case SPIM_SPIDVC_25AA640A:  BC_SPIM->m_DeviceId = DT_SPIM_SPIDVC_25AA640A; break;
    case SPIM_SPIDVC_AD9642:    BC_SPIM->m_DeviceId = DT_SPIM_SPIDVC_AD9642; break;
    case SPIM_SPIDVC_ADS4246:   BC_SPIM->m_DeviceId = DT_SPIM_SPIDVC_ADS4246; break;
    case SPIM_SPIDVC_GS1661:    BC_SPIM->m_DeviceId = DT_SPIM_SPIDVC_GS1661; break;
    case SPIM_SPIDVC_LMH0394:   BC_SPIM->m_DeviceId = DT_SPIM_SPIDVC_LMH0394; break;
    case SPIM_SPIDVC_GS3590:    BC_SPIM->m_DeviceId = DT_SPIM_SPIDVC_GS3590; break;
    case SPIM_SPIDVC_GS12090:   BC_SPIM->m_DeviceId = DT_SPIM_SPIDVC_GS12090; break;
    default: DT_ASSERT(FALSE); return DT_STATUS_FAIL; 
    }

    // Get SPI clock rate
    BC_SPIM->m_SpiClockRate = (Int)SPIM_Config2_READ_SpiClockRate(BC_SPIM);

    // Read config 3
    RegConfig = SPIM_Config3_READ(BC_SPIM);

    // Get word size
    BC_SPIM->m_WordSize = SPIM_Config3_GET_WordSize(RegConfig);
    // Wordsize must be a multipe of 8 and between 1 and 4 bytes
    DT_ASSERT(BC_SPIM->m_WordSize%8 == 0);
    DT_ASSERT((BC_SPIM->m_WordSize/8)>=1 && (BC_SPIM->m_WordSize/8)<=4);
    BC_SPIM->m_NumBytesPerWord = BC_SPIM->m_WordSize/8;

    // Get maximum transfer time
    BC_SPIM->m_MaxTransferTime = SPIM_Config3_GET_MaxTransferTime(RegConfig);

    // Get duplex mode
    if (SPIM_Config3_GET_DuplexMode(RegConfig) == SPIM_DPX_FULL_DUPLEX)
        BC_SPIM->m_DuplexMode = DT_SPIM_DPX_FULL_DUPLEX;
    else
        BC_SPIM->m_DuplexMode = DT_SPIM_DPX_HALF_DUPLEX;

    // Print configuration
    DtDbgOutBc(AVG, SPIM, pBc, "Configuration: clock=%dHz, dev_id=%s, word_size=%d, "
                                      "max_tx_time=%dns, duplex_mode=%s",
                                      BC_SPIM->m_SpiClockRate, 
                                      DtBcSPIM_DeviceIdToString(BC_SPIM->m_DeviceId),
                                      BC_SPIM->m_WordSize, BC_SPIM->m_MaxTransferTime,
                                      DtBcSPIM_DuplexModeToString(BC_SPIM->m_DuplexMode));

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIM_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSPIM_OnEnable(DtBc* pBc, Bool  Enable)
{
    // NOTHING SPECIAL TODO FOR NOW
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIM_CheckDeviceStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSPIM_CheckDeviceStatus(DtBcSPIM * pBc)
{
    UInt32 RegStatus;
    // Check is busy
    RegStatus = SPIM_Status_READ(pBc);
    if ( SPIM_Status_GET_IsBusy(RegStatus) != 0)
    {
        DtDbgOutBc(ERR, SPIM, pBc, "ERROR: SPI is already busy");
        return DT_STATUS_BUSY;
    }
    // Check is enabled 
    if (SPIM_Status_GET_IsEnabled(RegStatus) == 0)
    {

        DtDbgOutBc(ERR, SPIM, pBc, "ERROR: SPI is already busy");
        return  DT_STATUS_NOT_ENABLED;
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIM_TransferData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSPIM_TransferData(DtBcSPIM * pBc, Int Direction, Bool Continue)
{
    DtStatus  Status=DT_STATUS_OK;
    UInt32 RegControl, IsBusy;
    Int TimeoutCnt;

    // Start transfer
    RegControl = SPIM_Control_SET_StartTransfer(0, 1);
    RegControl = SPIM_Control_SET_ContinueTransfer(RegControl,
                                       Continue ? SPIM_CONTTF_CONTINUE : SPIM_CONTTF_END);
    RegControl = SPIM_Control_SET_TransferDirection(RegControl, Direction);
    SPIM_Control_WRITE(pBc, RegControl);

    // Typically IsBusy is deasserted after MaxTransferTime
    DtWaitBlock(1 + pBc->m_MaxTransferTime/1000);   // Round upwards
    IsBusy = SPIM_Status_READ_IsBusy(pBc);
  
    // Wait maximum double MaxTransferTime
    TimeoutCnt = 1 + pBc->m_MaxTransferTime/1000;
    for (; IsBusy!=0 && TimeoutCnt>0; TimeoutCnt--)
    {
        DtWaitBlock(1);
        IsBusy = SPIM_Status_READ_IsBusy(pBc);
    }

    // Timeout?
    if (IsBusy != 0)
    {
        DtDbgOutBc(ERR, SPIM, pBc, "ERROR: SPI timeout > %d ns", pBc->m_MaxTransferTime);
        Status = DT_STATUS_TIMEOUT;
    }

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIM_DeviceIdToString -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
const char*  DtBcSPIM_DeviceIdToString(Int  Id)
{
    switch (Id)
    {
    case DT_SPIM_SPIDVC_25AA160C:   return "25AA160C";
    case DT_SPIM_SPIDVC_25AA640A:   return "25AA640A";
    case DT_SPIM_SPIDVC_AD9642:     return "AD9642";
    case DT_SPIM_SPIDVC_ADS4246:    return "ADS4246";
    case DT_SPIM_SPIDVC_GS1661:     return "GS1661";
    case DT_SPIM_SPIDVC_GS3590:     return "GS3590";
    case DT_SPIM_SPIDVC_LMH0394:    return "LMH0394";
    case DT_SPIM_SPIDVC_GS12090:    return "GS12090";
    }
    DT_ASSERT(FALSE);
    return "???";
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIM_DuplexModeToString -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
const char*  DtBcSPIM_DuplexModeToString(Int  Mode)
{
    switch (Mode)
    {
    case DT_SPIM_DPX_FULL_DUPLEX:   return "Full-Duplex";
    case DT_SPIM_DPX_HALF_DUPLEX:   return "Half-Duplex";
    }
    DT_ASSERT(FALSE);
    return "???";
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSPIM implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcSPIM
#define SPIM_STUB   ((DtIoStubBcSPIM*)pStub)
#define SPIM_BC  ((DtBcSPIM*)SPIM_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcSPIM_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
static DtStatus  DtIoStubBcSPIM_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int* pOutSize);
DtStatus DtIoStubBcSPIM_OnCmdGetProperties(const DtIoStubBcSPIM*, 
                                                      DtIoctlSpiMCmdGetPropertiesOutput*);
static DtStatus  DtIoStubBcSPIM_OnCmdRead(const DtIoStubBcSPIM*,
                                                           const DtIoctlSpiMCmdReadInput*,
                                                           DtIoctlSpiMCmdReadOutput*);
static DtStatus  DtIoStubBcSPIM_OnCmdWrite(const DtIoStubBcSPIM*,
                                                         const DtIoctlSpiMCmdWriteInput*);
static DtStatus  DtIoStubBcSPIM_OnCmdWriteRead(const DtIoStubBcSPIM*,
                                                     const DtIoctlSpiMCmdWriteReadInput*,
                                                     DtIoctlSpiMCmdWriteReadOutput*);
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_SPIM;

static const DtIoctlProperties  IOSTUB_BC_SPIM_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_SPIM_CMD(
        DtIoStubBcSPIM_OnCmd,
        DtIoStubBcSPIM_AppendDynamicSize,
        NULL),
};


//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSPIM - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIM_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcSPIM_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSPIM));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIM_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcSPIM*  DtIoStubBcSPIM_Open(DtBc*  pBc)
{
    DtIoStubBcSPIM*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIM));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcSPIM, pBc, NULL,
        DtIoStubBcSPIM_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_SPIM_IOCTLS);
    pStub = (DtIoStubBcSPIM*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSPIM - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIM_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSPIM_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSpiMCmdInput*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSPIM));
    DT_ASSERT(pIoParams != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_SPIM_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SpiMCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_SPIM_CMD_READ:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlSpiMCmdReadInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlSpiMCmdReadOutput));
        // Add dynamic size (i.e. #bytes to read)
        pIoParams->m_OutReqSize += pInData->m_Read.m_NumToRead;
        break;

    case DT_SPIM_CMD_WRITE:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlSpiMCmdWriteInput));
        // Add dynamic size (i.e. #bytes to write)
        pIoParams->m_InReqSize += pInData->m_Write.m_NumToWrite;
        break;

    case DT_SPIM_CMD_WRITE_READ:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlSpiMCmdWriteReadInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlSpiMCmdWriteReadOutput));
        // Add dynamic size (i.e. #bytes to write)
        pIoParams->m_InReqSize += pInData->m_WriteRead.m_NumToWrite;
        // Add dynamic size (i.e. #bytes to read)
        pIoParams->m_OutReqSize += pInData->m_WriteRead.m_NumToRead;
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIM_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSPIM_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
    Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSpiMCmdInput*  pInData = NULL;
    DtIoctlSpiMCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSPIM));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_SPIM_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, SPIM, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SpiMCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_SpiMCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-
    switch (pIoParams->m_Cmd)
    {
    case DT_SPIM_CMD_GET_PROPERTIES:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSPIM_OnCmdGetProperties(SPIM_STUB, &pOutData->m_GetProperties);
        break;
    case DT_SPIM_CMD_READ:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSPIM_OnCmdRead(SPIM_STUB, &pInData->m_Read, &pOutData->m_Read);
        break;
    case DT_SPIM_CMD_WRITE:
        Status = DtIoStubBcSPIM_OnCmdWrite(SPIM_STUB, &pInData->m_Write);
        break;
    case DT_SPIM_CMD_WRITE_READ:
        Status = DtIoStubBcSPIM_OnCmdWriteRead(SPIM_STUB, &pInData->m_WriteRead,
                                                                  &pOutData->m_WriteRead);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIM_OnCmdGetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSPIM_OnCmdGetProperties(
    const DtIoStubBcSPIM*  pStub, 
    DtIoctlSpiMCmdGetPropertiesOutput*  pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSPIM));
    DT_ASSERT(pOutData != NULL);
    
    return DtBcSPIM_GetProperties(SPIM_BC, &pOutData->m_SpiDeviceId,
                                    &pOutData->m_DuplexMode, &pOutData->m_MaxTransferTime,
                                    &pOutData->m_SpiClockRate);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIM_OnCmdRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSPIM_OnCmdRead(
    const DtIoStubBcSPIM*  pStub,
    const DtIoctlSpiMCmdReadInput*  pInData,
    DtIoctlSpiMCmdReadOutput*  pOutData)
{

    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSPIM));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    // Assume the worst
    pOutData->m_NumBytes = 0;
    Status = DtBcSPIM_Read(SPIM_BC, pInData->m_NumToRead, pOutData->m_Buf);
    // If succesful, set number of bytes read
    if (DT_SUCCESS(Status))
        pOutData->m_NumBytes = pInData->m_NumToRead;
    return  Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIM_OnCmdWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSPIM_OnCmdWrite(
    const DtIoStubBcSPIM*  pStub,
    const DtIoctlSpiMCmdWriteInput*  pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSPIM));
    DT_ASSERT(pInData != NULL);

    return DtBcSPIM_Write(SPIM_BC, pInData->m_NumToWrite, pInData->m_Buf);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIM_OnCmdWriteRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSPIM_OnCmdWriteRead(
    const DtIoStubBcSPIM*  pStub,
    const DtIoctlSpiMCmdWriteReadInput*  pInData,
    DtIoctlSpiMCmdWriteReadOutput*  pOutData)
{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSPIM));
    DT_ASSERT(pInData != NULL);

    Status = DtBcSPIM_WriteRead(SPIM_BC, pInData->m_NumToWrite, pInData->m_Buf,
                                                   pInData->m_NumToRead, pOutData->m_Buf);
    // If succesful, set number of bytes read
    if (DT_SUCCESS(Status))
        pOutData->m_NumBytes = pInData->m_NumToRead;
    return  Status;
}