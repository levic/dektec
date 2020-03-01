//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcI2CM.c *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
#include "DtBcI2CM.h"
#include "DtBcI2CM_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcI2CM implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_I2CM_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcI2CM))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_I2CM_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(I2CM, pBc)

// Helper macro to cast a DtBc* to a DtBcI2CM*
#define BC_I2CM         ((DtBcI2CM*)pBc)


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcI2CM_Init(DtBc*);
static DtStatus  DtBcI2CM_OnEnable(DtBc*, Bool  Enable);
static void  DtBcI2CM_InterruptDpc(DtDpcArgs* pArgs);
static DtStatus  DtBcI2CM_InterruptHandler(DtBc*, Int, Int, void*);
static DtStatus  DtBcI2CM_RegisterIntHandlers(DtBcI2CM*);
static DtStatus  DtBcI2CM_Reset(DtBcI2CM* pBc);
static DtStatus  DtBcI2CM_WriteReadTransfer(DtBcI2CM* pBc, UInt SlaveAddr, 
                                              Int WriteLength, const UInt8 * pWriteBuffer, 
                                              Int ReadLength, UInt8 * pReadBuffer);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcI2CM - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcI2CM_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcI2CM_Close(DtBc*  pBc)
{
    BC_I2CM_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcI2CM_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcI2CM*  DtBcI2CM_Open(Int  Address, DtCore*  pCore, DtPt*  pPt,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_I2CM_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcI2CM, Id, DT_BLOCK_TYPE_I2CM, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcI2CM_Close;
    OpenParams.m_InitFunc = DtBcI2CM_Init;
    OpenParams.m_OnEnableFunc = DtBcI2CM_OnEnable;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcI2CM*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcI2CM_Read -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcI2CM_Read(DtBcI2CM* pBc, UInt SlaveAddr, Int Length, UInt8* pBuffer)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check    
    BC_I2CM_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (SlaveAddr >= (1<<7))
        return DT_STATUS_INVALID_PARAMETER;
    if (Length > pBc->m_FifoSize)
        return DT_STATUS_INVALID_PARAMETER;
    
    DtDbgOutBc(MAX, I2CM, pBc, "Read SlaveAddr: 0x%02X, Length: %d", SlaveAddr, Length);
    
    // Protect I2C access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Transfer data
    Status = DtBcI2CM_WriteReadTransfer(pBc, SlaveAddr, 0, NULL, Length, pBuffer);
    
    if (!DT_SUCCESS(Status))
    {
        // Transfer failed, reset and retry
        DtBcI2CM_Reset(pBc);
        Status = DtBcI2CM_WriteReadTransfer(pBc, SlaveAddr, 0, NULL, Length, pBuffer);
    }

    // Release I2C mutex
    DtFastMutexRelease(&pBc->m_AccessMutex);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcI2CM_Write -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcI2CM_Write(DtBcI2CM* pBc, UInt SlaveAddr, Int Length, const UInt8 * pBuffer)
{
    DtStatus  Status = DT_STATUS_OK;
 
    // Sanity check    
    BC_I2CM_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (SlaveAddr >= (1<<7))
        return DT_STATUS_INVALID_PARAMETER;
    if (Length > pBc->m_FifoSize)
        return DT_STATUS_INVALID_PARAMETER;

    DtDbgOutBc(MAX, I2CM, pBc, "Write SlaveAddr: 0x%02X, Length: %d", SlaveAddr, Length);
    
    // Protect I2C access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Transfer data
    Status = DtBcI2CM_WriteReadTransfer(pBc, SlaveAddr, Length, pBuffer, 0, NULL);
    
    if (!DT_SUCCESS(Status))
    {
        // Transfer failed, reset and retry
        DtBcI2CM_Reset(pBc);
        Status = DtBcI2CM_WriteReadTransfer(pBc, SlaveAddr, Length, pBuffer, 0, NULL);
    }

    // Release I2C mutex
    DtFastMutexRelease(&pBc->m_AccessMutex);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcI2CM_WriteRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcI2CM_WriteRead(DtBcI2CM* pBc, UInt SlaveAddr, 
                                              Int WriteLength, const UInt8 * pWriteBuffer,
                                              Int ReadLength, UInt8 * pReadBuffer)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check    
    BC_I2CM_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (SlaveAddr >= (1<<7))
        return DT_STATUS_INVALID_PARAMETER;
    if (WriteLength>pBc->m_FifoSize || ReadLength>pBc->m_FifoSize)
        return DT_STATUS_INVALID_PARAMETER;
     
    // Protect I2C access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Transfer data
    Status = DtBcI2CM_WriteReadTransfer(pBc, SlaveAddr, WriteLength, pWriteBuffer, 
                                                                 ReadLength, pReadBuffer);
    
    if (!DT_SUCCESS(Status))
    {
        // Transfer failed, reset and retry
        DtBcI2CM_Reset(pBc);
        Status = DtBcI2CM_WriteReadTransfer(pBc, SlaveAddr, WriteLength, pWriteBuffer, 
                                                                 ReadLength, pReadBuffer);
    }

    // Release I2C mutex
    DtFastMutexRelease(&pBc->m_AccessMutex);
    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcI2CM - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcI2CM_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcI2CM_Init(DtBc*  pBc)
{  
    DtStatus  Status=DT_STATUS_OK;
    UInt32  RegData;

    // Init interrupt DPC
    Status = DtDpcInit(&BC_I2CM->m_IntDpc, DtBcI2CM_InterruptDpc, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, I2CM, pBc, "ERROR: failed to init DPC (Status=0x%08X)", Status);
        return Status;
    }
    // Read configuration
    BC_I2CM->m_DeviceId = I2CM_Config1_READ_DeviceId(BC_I2CM);
    RegData = I2CM_Config2_READ(BC_I2CM);
    BC_I2CM->m_FifoSize = I2CM_Config2_GET_FifoSize(RegData);
    BC_I2CM->m_I2cRate = I2CM_Config2_GET_I2cRate(RegData);

    // Initialize I2C mutex against concurrent access
    DtFastMutexInit(&BC_I2CM->m_AccessMutex);

    // Init completion event
    Status = DtEventInit(&BC_I2CM->m_DoneEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, I2CM, pBc, 
                              "ERROR: failed to init done event (Status=0x%08X)", Status);
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    Status = DtBcI2CM_RegisterIntHandlers(BC_I2CM);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, I2CM, pBc, "ERROR: failed to register interrupt handlers");
        return Status;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcI2CM_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcI2CM_InterruptHandler(DtBc*  pBc, Int  Id, Int  Index, void*  pContext)
{
    DtStatus  Status=DT_STATUS_OK;
    DtDpcArgs  DpcArgs;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcI2CM));

    // Check ID is one of ours
    if (Id != DT_INTERRUPT_I2CM_CMDDONE)
    {
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Disable the I2C interrupt that was fired. 
    // NOTE: is re-enabled upon next transfer
    DtBc_InterruptDisable(pBc, Id);

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    DpcArgs.m_Data1.m_UInt32_2 = 0;
    
    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&((DtBcI2CM*)pBc)->m_IntDpc, &DpcArgs);
    DT_ASSERT(DT_SUCCESS(Status));

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcI2CM_InterruptDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcI2CM_InterruptDpc(DtDpcArgs* pArgs)
{
    DtBcI2CM*  pBc = (DtBcI2CM*)pArgs->m_pContext;
    DtDbgOutBc(MAX, I2CM, pBc, "I2C transfer processed");
    DtEventSet(&pBc->m_DoneEvent);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcI2CM_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcI2CM_OnEnable(DtBc* pBc, Bool  Enable)
{
    // NOTHING SPECIAL TODO FOR NOW
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcI2CM_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcI2CM_RegisterIntHandlers(DtBcI2CM*  pBc)
{
    Int  i=0;
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_I2CM_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch(Id)
        {
        case DT_INTERRUPT_I2CM_CMDDONE:
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id,
                                                         DtBcI2CM_InterruptHandler, NULL);
            if(!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, I2CM, pBc,
                               "ERROR: failed to register interrupt handler (ID=%d)", Id);
            }
            break;

        default:
            DT_ASSERT(FALSE);      // Unknown ID. New version of the block??
            break;
        }
    }
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcI2CM_Reset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcI2CM_Reset(DtBcI2CM* pBc)
{
    UInt32  RegStatus, RegControl, IsBusy;
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check    
    BC_I2CM_DEFAULT_PRECONDITIONS(pBc);

    // Check is busy
    RegStatus = I2CM_Status_READ(pBc);
    IsBusy = I2CM_Status_GET_IsBusy(RegStatus);
    if (IsBusy != 0)
    {
        DtDbgOutBc(ERR, I2CM, pBc, "ERROR: I2c is already busy");
        return DT_STATUS_BUSY;
    }

    // Reset done event
    DtEventReset(&pBc->m_DoneEvent);

    // Enable interrupts
    Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_I2CM_CMDDONE);
    if (!DT_SUCCESS(Status))
    {
        DT_ASSERT(FALSE);
        return Status;
    }

    // Set read/write count
    RegControl = I2CM_Control2_SET_ReadCount(0, 0);
    RegControl = I2CM_Control2_SET_WriteCount(RegControl, 0);
    I2CM_Control2_WRITE(pBc, RegControl);

    // Issue reset command
    RegControl = I2CM_Control1_SET_SlaveAddress(0, 0);
    RegControl = I2CM_Control1_SET_Command(0, I2CM_CMD_RESET);
    I2CM_Control1_WRITE(pBc, RegControl);

    // Wait for done interrupt
    Status = DtEventWaitUnInt(&pBc->m_DoneEvent, 5000);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, I2CM, pBc, "ERROR: Wait for done failed (Status=0x%08X)", Status);
        return Status;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcI2CM_WriteReadTransfer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcI2CM_WriteReadTransfer(DtBcI2CM* pBc, UInt SlaveAddr, 
                                              Int WriteLength, const UInt8 * pWriteBuffer,
                                              Int ReadLength, UInt8* pReadBuffer)
{
    UInt32  RegStatus, RegControl, IsBusy, I2cCommand;
    UInt32  AddressNack, WriteDataNack, Timeout;
    UInt32  Data;
    UInt32  WriteCount = WriteLength;
    UInt32  ReadCount = ReadLength;
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check    
    BC_I2CM_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(WriteLength==0 || pWriteBuffer!=NULL);
    DT_ASSERT(ReadLength==0 || pReadBuffer!=NULL);
    DT_ASSERT(WriteLength!=0 || ReadLength!=0);

    // Determine command
    if (WriteLength==0 && ReadLength>0)
        I2cCommand = I2CM_CMD_READ;
    else  if (WriteLength>0 && ReadLength==0)
        I2cCommand = I2CM_CMD_WRITE;
    else  if (WriteLength>0 && ReadLength>0)
        I2cCommand = I2CM_CMD_WRITE_READ;
    else
        return DT_STATUS_INVALID_PARAMETER;

    // Check is busy
    RegStatus = I2CM_Status_READ(pBc);
    IsBusy = I2CM_Status_GET_IsBusy(RegStatus);
    if (IsBusy != 0)
    {
        DtDbgOutBc(ERR, I2CM, pBc, "ERROR: I2c is already busy");
        return DT_STATUS_BUSY;
    }

    // Write?
    if (WriteLength > 0)
    { 
        // Write the 32-bit data words
        while (WriteLength >= 4)
        {
            Data = (pWriteBuffer[0]) | (pWriteBuffer[1]<<8) | (pWriteBuffer[2]<<16) 
                                                                  | (pWriteBuffer[3]<<24);
            I2CM_WriteData_WRITE(pBc, Data);
            pWriteBuffer += 4;
            WriteLength -= 4;
        }

        // Write remaining data
        switch(WriteLength)
        {
        case 1:
            Data = (pWriteBuffer[0]);
            I2CM_WriteData_WRITE(pBc, Data);
            break;
        case 2:
            Data = (pWriteBuffer[0]) | (pWriteBuffer[1]<<8);
            I2CM_WriteData_WRITE(pBc, Data);
            break;
        case 3:
            Data = (pWriteBuffer[0]) | (pWriteBuffer[1]<<8) | (pWriteBuffer[2]<<16);
            I2CM_WriteData_WRITE(pBc, Data);
            break;
        }
    }

    // Reset done event
    DtEventReset(&pBc->m_DoneEvent);

    // Enable interrupts
    Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_I2CM_CMDDONE);
    if (!DT_SUCCESS(Status))
    {
        DT_ASSERT(FALSE);
        return Status;
    }

    // Set read/write count
    RegControl = I2CM_Control2_SET_ReadCount(0, ReadCount);
    RegControl = I2CM_Control2_SET_WriteCount(RegControl, WriteCount);
    I2CM_Control2_WRITE(pBc, RegControl);

    // Start transfer
    RegControl = I2CM_Control1_SET_SlaveAddress(0, SlaveAddr);
    RegControl = I2CM_Control1_SET_Command(RegControl, I2cCommand);
    I2CM_Control1_WRITE(pBc, RegControl);

    // Wait for done interrupt
    Status = DtEventWaitUnInt(&pBc->m_DoneEvent, 5000);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, I2CM, pBc, "ERROR: Wait for done failed (Status=0x%08X)", Status);
        return Status;
    }

    // Check status
    RegStatus = I2CM_Status_READ(pBc);
    AddressNack = I2CM_Status_GET_AddressNack(RegStatus);
    WriteDataNack = I2CM_Status_GET_WriteDataNack(RegStatus);
    Timeout = I2CM_Status_GET_Timeout(RegStatus);

    if (AddressNack!=0 || WriteDataNack!=0)
        Status = DT_STATUS_FAIL;
    else if (Timeout != 0)
        Status = DT_STATUS_TIMEOUT;

    if (DT_SUCCESS(Status) && ReadLength>0)
    { 
        // Read the data
        while (ReadLength > 0)
        {
            Int  i;
            Data = I2CM_ReadData_READ(pBc);

            for (i=0; i<4 && ReadLength>0; i++)
            {
                *pReadBuffer++ = Data&0xFF;
                Data >>= 8;
                ReadLength--;
            }
        }
    }
    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcI2CM implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcI2CM
#define I2CM_STUB   ((DtIoStubBcI2CM*)pStub)
#define I2CM_BC  ((DtBcI2CM*)I2CM_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcI2CM_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
static DtStatus  DtIoStubBcI2CM_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int* pOutSize);
static DtStatus  DtIoStubBcI2CM_OnCmdRead(const DtIoStubBcI2CM*, 
                                                          const DtIoctlI2cMCmdReadInput*,
                                                          DtIoctlI2cMCmdReadOutput*);
static DtStatus  DtIoStubBcI2CM_OnCmdWrite(const DtIoStubBcI2CM*, 
                                                        const DtIoctlI2cMCmdWriteInput*);
static DtStatus  DtIoStubBcI2CM_OnCmdWriteRead(const DtIoStubBcI2CM*, 
                                                      const DtIoctlI2cMCmdWriteReadInput*, 
                                                      DtIoctlI2cMCmdWriteReadOutput*);
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_I2CM;

static const DtIoctlProperties  IOSTUB_BC_I2CM_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_I2CM_CMD(
        DtIoStubBcI2CM_OnCmd,
        DtIoStubBcI2CM_AppendDynamicSize,
        NULL),
};


//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcI2CM - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcI2CM_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcI2CM_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcI2CM));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcI2CM_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcI2CM*  DtIoStubBcI2CM_Open(DtBc*  pBc)
{
    DtIoStubBcI2CM*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcI2CM));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcI2CM, pBc, NULL, 
                                                              DtIoStubBcI2CM_Close,
                                                              NULL,  // Use default IOCTL
                                                              IOSTUB_BC_I2CM_IOCTLS);
    pStub = (DtIoStubBcI2CM*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcI2CM - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcI2CM_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcI2CM_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlI2cMCmdInput*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcI2CM));
    DT_ASSERT(pIoParams != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_I2CM_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_I2cMCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_I2CM_CMD_READ:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlI2cMCmdReadInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlI2cMCmdReadOutput));
        // Add dynamic size (i.e. #bytes to read)
        pIoParams->m_OutReqSize += pInData->m_Read.m_NumToRead;
        break;

    case DT_I2CM_CMD_WRITE:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlI2cMCmdWriteInput));
        // Add dynamic size (i.e. #bytes to write)
        pIoParams->m_InReqSize += pInData->m_Write.m_NumToWrite;
        break;
    
    case DT_I2CM_CMD_WRITE_READ:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlI2cMCmdWriteReadInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlI2cMCmdWriteReadOutput));
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcI2CM_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcI2CM_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlI2cMCmdInput*  pInData = NULL;
    DtIoctlI2cMCmdOutput*  pOutData = NULL;
    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcI2CM));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_I2CM_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, I2CM, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_I2cMCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_I2cMCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_I2CM_CMD_READ:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcI2CM_OnCmdRead(I2CM_STUB, 
                                                     &pInData->m_Read, &pOutData->m_Read);
        break;
    case DT_I2CM_CMD_WRITE:
        Status = DtIoStubBcI2CM_OnCmdWrite(I2CM_STUB, &pInData->m_Write);
        break;
    case DT_I2CM_CMD_WRITE_READ:
        Status = DtIoStubBcI2CM_OnCmdWriteRead(I2CM_STUB, &pInData->m_WriteRead,
                                                                  &pOutData->m_WriteRead);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcI2CM_OnCmdRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcI2CM_OnCmdRead(
    const DtIoStubBcI2CM*  pStub, 
    const DtIoctlI2cMCmdReadInput*  pInData,
    DtIoctlI2cMCmdReadOutput*  pOutData)
{

    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcI2CM));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    // Assume the worst
    pOutData->m_NumBytes = 0;
    Status = DtBcI2CM_Read(I2CM_BC, pInData->m_SlaveAddr, pInData->m_NumToRead, 
                                                                         pOutData->m_Buf);
    // If succesful, set number of bytes read
    if (DT_SUCCESS(Status))
        pOutData->m_NumBytes = pInData->m_NumToRead;
    return  Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcI2CM_OnCmdWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcI2CM_OnCmdWrite(
    const DtIoStubBcI2CM*  pStub, 
    const DtIoctlI2cMCmdWriteInput*  pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcI2CM));
    DT_ASSERT(pInData != NULL);

    return DtBcI2CM_Write(I2CM_BC, pInData->m_SlaveAddr, pInData->m_NumToWrite, 
                                                                          pInData->m_Buf);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcI2CM_OnCmdWriteRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcI2CM_OnCmdWriteRead(
    const DtIoStubBcI2CM*  pStub, 
    const DtIoctlI2cMCmdWriteReadInput*  pInData,
    DtIoctlI2cMCmdWriteReadOutput*  pOutData)
{
    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcI2CM));
    DT_ASSERT(pInData != NULL);

    Status = DtBcI2CM_WriteRead(I2CM_BC, pInData->m_SlaveAddr,   
                                                   pInData->m_NumToWrite, pInData->m_Buf, 
                                                   pInData->m_NumToRead, pOutData->m_Buf);
    // If succesful, set number of bytes read
    if (DT_SUCCESS(Status))
        pOutData->m_NumBytes = pInData->m_NumToRead;
    return  Status;
}