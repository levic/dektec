// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSPIMF.c *#*#*#*#*#*#*#*# (C) 2017-2019 DekTec
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
#include "DtDf.h"
#include "DtBcSPIMF.h"
#include "DtBcSPIMF_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSPIMF implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_SPIMF_DEFAULT_PRECONDITIONS(pBc)      \
                                    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIMF))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_SPIMF_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(SPIMF, pBc)

// SPI master flash operation states
#define SPIMF_STATE_IDLE      0
#define SPIMF_STATE_READ      1
#define SPIMF_STATE_WRITE     2
#define SPIMF_STATE_ERASE     3

// MemoryId
#define DT_BC_SPIMF_MEM_UNKNOWN         0x0    // EEPROM memory
#define DT_BC_SPIMF_MEM_25Q064          0x1    // 64Mb NOR flash memory
#define DT_BC_SPIMF_MEM_25AA160C        0x2    // 16Kb serial EEPROM
#define DT_BC_SPIMF_MEM_25Q512          0x3    // 512Mb NOR flash memory
#define DT_BC_SPIMF_MEM_25Q256          0x4    // 256Mb NOR flash memory

// Unlock sequences
static const UInt32  SPIMF_UNLOCK_SEQ_V0[] = { 0x1, 0x3, 0xD, 0x6, 0xC };
static const UInt32  SPIMF_UNLOCK_SEQ_V1[] = { 0x2, 0x4, 0xE, 0x7, 0xD };
static const Int  SPIMF_UNLOCK_SEQ_SIZE = 5;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcSPIMF_RegisterIntHandlers(DtBcSPIMF*);
static DtStatus  DtBcSPIMF_Init(DtBc*);
static DtStatus  DtBcSPIMF_OnEnable(DtBc*, Bool  Enable);
static void  DtBcSPIMF_InterruptDpc(DtDpcArgs* pArgs);
static void  DtBcSPIMF_InterruptDpcErase(DtBcSPIMF*, UInt32);
static void  DtBcSPIMF_InterruptDpcRead(DtBcSPIMF*, UInt32);
static void  DtBcSPIMF_InterruptDpcWrite(DtBcSPIMF*, UInt32);
static DtStatus  DtBcSPIMF_InterruptHandler(DtBc*, Int, Int, void*);
static DtStatus  DtBcSPIMF_ExecuteCommand(DtBcSPIMF*,  Int  Cmd,  Int  StartAddress,
                                                               Int  TrSize, const UInt8*);

// Serial Flash Discovery Parameter (SFDP) functions
static DtStatus  DtBcSPIMF_InitializeFlashProperties(DtBcSPIMF*);
static DtStatus  DtBcSPIMF_SetDefaultFlashProperties(DtBcSPIMF*);
static DtStatus  DtBcSPIMF_ReadParamTable(DtBcSPIMF*, Int TableId, Int Major, Int Minor,
                                                            UInt32* pTable, Int* pLength);
static DtStatus  DtBcSPIMF_Determine3ByteAddrInstr(UInt8* pBuffer, Int EraseType, 
                                                                      UInt32* FlashProps);
static DtStatus  DtBcSPIMF_Determine4ByteAddrInstr(UInt8* pBuffer, Int EraseType, 
                                                                      UInt32* FlashProps);
static void  DtBcSPIMF_DetermineSectorSize(UInt8 * pBuffer, Bool SectorMapTablePresent,
                                                           UInt* pSize, UInt* pEraseType);
static UInt  DtBcSPIMF_GetAddressSize(UInt8* pBuffer);
static UInt  DtBcSPIMF_GetMaximumChipEraseTimeUs(UInt8* pBuffer);
static UInt  DtBcSPIMF_GetMaximumPageProgramTimeUs(UInt8* pBuffer);
static UInt  DtBcSPIMF_GetMaximumSectorEraseTimeUs(UInt8* pBuffer, Int EraseType);
static UInt64  DtBcSPIMF_GetMemorySize(UInt8* pBuffer);
static UInt  DtBcSPIMF_GetPageSize(UInt8* pBuffer);
static UInt  DtBcSPIMF_GetSectorEraseInstr(UInt8* pBuffer, Int EraseType);
static UInt  DtBcSPIMF_GetSectorSize(UInt8* pBuffer, Int EraseType);
static void  DtBcSPIMF_GetSoftResetInstr(UInt8* pBuffer, UInt32* pResetEnable,
                                                                     UInt32* pResetInstr);
static UInt  DtBcSPIMF_GetTypicalChipEraseTimeUs(UInt8* pBuffer);
static UInt  DtBcSPIMF_GetTypicalPageProgramTimeUs(UInt8* pBuffer);
static UInt  DtBcSPIMF_GetTypicalSectorEraseTimeUs(UInt8* pBuffer, Int EraseType);
static UInt  DtBcSPIMF_GetTypToMaxEraseTimeFactor(UInt8* pBuffer);
static UInt  DtBcSPIMF_ComputeSectorEraseTimeUs(UInt32 Units, UInt32 Count);
static  UInt64  DtBcSPIMF_Exp2(Int Value);
static UInt  DtBcSPIMF_Log2(UInt64 Value);
static UInt32  DtBcSPIMF_ReadDWord(const UInt8* pBuf, Int DWordNum);
static UInt  DtBcSPIMF_TimeoutUsToLog2Ms(UInt TimeoutUs);

static void DtBcSPIMF_SetFlashProperties(DtBcSPIMF* pBc, UInt32* FlashProps);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSPIMF - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSPIMF_Close(DtBc*  pBc)
{
    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);
    
    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_Erase -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSPIMF_Erase(DtBcSPIMF*  pBc,  Int  StartAddress, Int  NumToErase)
{
    DtStatus  Status=DT_STATUS_OK;
    Int  EraseCmd = SPIMF_CMD_NOP;

    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SPIMF_MUST_BE_ENABLED(pBc);

    // Check whether flash properties are set/known
    if (!pBc->m_FlashPropsAreSet)
        return DT_STATUS_NOT_SUPPORTED;

    // Check parameters
    if (StartAddress==0 && NumToErase==pBc->m_ConfMemorySize)
    {
        EraseCmd = SPIMF_CMD_BULK_ERASE;    // Complete device erase
    }
    // If the device has no sectors only complete device erase is supported
    // Addresses must be sector alligned
    else if (   (StartAddress+NumToErase)>pBc->m_ConfMemorySize
             || (pBc->m_ConfSectorSize==0)  // No sectors
             || (StartAddress%pBc->m_ConfSectorSize)!=0
             || (NumToErase%pBc->m_ConfSectorSize)!=0)
    {
        // Invalid parameters
        return DT_STATUS_INVALID_PARAMETER;
    }
    else
    {
        // Sector erase
        EraseCmd = SPIMF_CMD_SECTOR_ERASE;
    }

    // Execute the erase command
    Status = DtBcSPIMF_ExecuteCommand(pBc,  EraseCmd, StartAddress,  NumToErase, NULL);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: erase-command failed (status=0x%X)", Status);
        return Status;
    }
    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_LockDevice -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSPIMF_LockDevice(DtBcSPIMF*  pBc, Bool  Lock)
{
    DtStatus  Status=DT_STATUS_OK;
    Int  i=0;
    Bool  IsEnabled=FALSE, IsBusy=FALSE, IsLocked=FALSE;
    UInt32  RegStatus=0;

    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);

    // Cache status register
    RegStatus = SPIMF_Status_READ(pBc);
    
    // Check is-enabled flag
    IsEnabled = (SPIMF_Status_GET_IsEnabled(RegStatus) != 0);
    if (!IsEnabled)
        return DT_STATUS_NO_POWER;

    // Check is-busy state
    IsBusy = (SPIMF_Status_GET_IsBusy(RegStatus) != 0);
    if (IsBusy)
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: device is busy");
        DT_ASSERT(!IsBusy);
        return DT_STATUS_BUSY;
    }

    // Check device locked state
    IsLocked = (SPIMF_Status_GET_IsLocked(RegStatus) != 0);
    if (IsLocked != Lock)
    {
        // Set command field to NOP, so that we donot accidentally issue a command below
        UInt32  RegControl = RegControl = SPIMF_Control_READ(pBc);
        RegControl = SPIMF_Control_SET_Command(0x0, SPIMF_CMD_NOP);

        // Not yet the correct state
        if (Lock)
        {
            // Writing a value not in the unlock sequence will lock the device
            RegControl = SPIMF_Control_SET_Unlock(RegControl, 0xF);
            SPIMF_Control_WRITE(pBc, RegControl);
        }
        else
        {
            // Write the unlock sequence
            const UInt32* pUnlockSeq = (pBc->m_SupportsV1) ? SPIMF_UNLOCK_SEQ_V1 :
                                                                      SPIMF_UNLOCK_SEQ_V0;
            for (i=0; i<SPIMF_UNLOCK_SEQ_SIZE; i++)
            {
                RegControl = SPIMF_Control_SET_Unlock(RegControl, pUnlockSeq[i]);
                SPIMF_Control_WRITE(pBc, RegControl);
            }
        }
        // Check result
        IsLocked = (SPIMF_Status_READ_IsLocked(pBc) != 0);
        if (IsLocked != Lock)
        {
            DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: (un)locking device failed");
            Status = DT_STATUS_FAIL;
        }
    }
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcSPIMF*  DtBcSPIMF_Open(Int  Address, DtCore*  pCore, DtPt* pPt,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_SPIMF_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcSPIMF, Id, DT_BLOCK_TYPE_SPIMF, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcSPIMF_Close;
    OpenParams.m_InitFunc = DtBcSPIMF_Init;
    OpenParams.m_OnEnableFunc = DtBcSPIMF_OnEnable;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcSPIMF*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_Read -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSPIMF_Read(DtBcSPIMF*  pBc, Int StartAddress, Int NumToRead, UInt8* pBuf)
{
    DtStatus  Status=DT_STATUS_OK;

    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);

    // Check for parameter errors.
    if (pBuf==NULL || ((UIntPtr)pBuf&0x3)!=0 
                   || (StartAddress+NumToRead)>pBc->m_ConfMemorySize
                   || (StartAddress%4)!=0 || (NumToRead%4)!=0)
    {
        // Invalid start address or #read 
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_SPIMF_MUST_BE_ENABLED(pBc);

    // Check whether flash properties are set/known
    if (!pBc->m_FlashPropsAreSet)
        return DT_STATUS_NOT_SUPPORTED;


    // Execute the read command
    Status = DtBcSPIMF_ExecuteCommand(pBc, SPIMF_CMD_READ, StartAddress, NumToRead, pBuf);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: read-command failed (status=0x%X)", Status);
        return Status;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_Write -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSPIMF_Write(DtBcSPIMF* pBc, Int StartAddress, Int NumToWrite, 
                                                                        const UInt8* pBuf)
{
    DtStatus  Status=DT_STATUS_OK;

    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);

    // Check for parameter errors.
    if (pBuf==NULL || ((UIntPtr)pBuf&0x3)!=0 
                   || (StartAddress+NumToWrite)>pBc->m_ConfMemorySize
                   || (StartAddress%pBc->m_ConfPageSize)!=0
                   || (NumToWrite%pBc->m_ConfPageSize)!=0)
    {
        // Invalid start address or #write
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_SPIMF_MUST_BE_ENABLED(pBc);

    // Check whether flash properties are set/known
    if (!pBc->m_FlashPropsAreSet)
        return DT_STATUS_NOT_SUPPORTED;


    // Execute the write command
    Status = DtBcSPIMF_ExecuteCommand(pBc, SPIMF_CMD_PAGE_WRITE, StartAddress, NumToWrite,
                                                                                    pBuf);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: write-command failed (status=0x%X)", Status);
        return Status;
    }
    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSPIMF - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_ExecuteCommand -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSPIMF_ExecuteCommand(DtBcSPIMF*  pBc, Int  Cmd, Int  StartAddress,
                                                    Int  TrSize, const UInt8*  pBuf)
{
    DtStatus  Status=DT_STATUS_OK;
    Bool  IsEnabled=FALSE, IsBusy=FALSE, IsLocked=FALSE, MustBeUnlocked=FALSE;
    UInt32  RegStatus=0, RegControl=0;
    UInt32  Timeout;

    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);

    // Cache status register
    RegStatus = SPIMF_Status_READ(pBc);

    // Check the SpiMasterFlash is enabled
    IsEnabled = (SPIMF_Status_GET_IsEnabled(RegStatus) != 0);
    if(!IsEnabled)
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: SpiMf is disabled");
        return DT_STATUS_FAIL;
    }

    // Check if the controller is already busy
    IsBusy = (SPIMF_Status_GET_IsBusy(RegStatus) != 0);
    if(IsBusy)
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: SpiMf is busy");
        DT_ASSERT(!IsBusy);
        return DT_STATUS_BUSY;
    }
    
    // Init state
    DT_ASSERT(pBc->m_State == SPIMF_STATE_IDLE);
    switch(Cmd)
    {
    case SPIMF_CMD_BULK_ERASE:
        DT_ASSERT(pBuf == NULL);
        pBc->m_State = SPIMF_STATE_ERASE;
        Timeout = pBc->m_BulkEraseTimeout;
        MustBeUnlocked = TRUE;      // Device must be unlocked for an erase
        break;
    case SPIMF_CMD_SECTOR_ERASE:
        DT_ASSERT(pBuf == NULL);
        pBc->m_State = SPIMF_STATE_ERASE;
        Timeout = pBc->m_SectorEraseTimeout;
        MustBeUnlocked = TRUE;      // Device must be unlocked for an erase
        break;
    case SPIMF_CMD_READ:
        DT_ASSERT(pBuf != NULL);
        pBc->m_State = SPIMF_STATE_READ;
        Timeout = 0;
        MustBeUnlocked = FALSE;
        break;
    case SPIMF_CMD_PAGE_WRITE:
        DT_ASSERT(pBuf != NULL);
        pBc->m_State = SPIMF_STATE_WRITE;
        Timeout = pBc->m_PageWriteTimeout;
        MustBeUnlocked = TRUE;      // Device must be unlocked for a write
        break;
    case SPIMF_CMD_READ_SFDP:
        DT_ASSERT(pBuf != NULL);
        pBc->m_State = SPIMF_STATE_READ;
        Timeout = 0;
        MustBeUnlocked = FALSE;
        DT_ASSERT(pBc->m_SupportsV1);     // Not supported in SPIMF V0
        break;

    default:
        DT_ASSERT(FALSE);
        return  DT_STATUS_NOT_SUPPORTED;      // Unsupported command
    }
    pBc->m_pData = (UInt32*)pBuf;
    pBc->m_NumToDo = (pBuf != NULL) ? TrSize : 0;
    pBc->m_Result = DT_STATUS_OK;

    // Must the device be unlocked? then check it
    if(MustBeUnlocked)
    {
        IsLocked = (SPIMF_Status_GET_IsLocked(RegStatus)!=0);
        if (IsLocked)
        {
            DtDbgOutBc(ERR, SPIMF, pBc, "Device is not unlocked");
            return DT_STATUS_LOCKED;
        }
    }

    // Set start address
    SPIMF_StartAddress_WRITE(pBc, StartAddress);
    // Set transfer size
    SPIMF_TransferSize_WRITE(pBc, TrSize);

    // Enable interrupts
    Status = DtEventReset(&pBc->m_OpComplEvent);
    DT_ASSERT(DT_SUCCESS(Status));
    Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_SPIMF_CMDDONE);
    DT_ASSERT(DT_SUCCESS(Status));

    if(pBc->m_State == SPIMF_STATE_READ)
    {
        // Enable the half-full-interrupt if more than FIFO-size has to be read
        if (pBc->m_NumToDo > pBc->m_ConfRxFifoSize)
        {
            Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_SPIMF_RXHALFFULL);
            DT_ASSERT(DT_SUCCESS(Status));
        }
    }
    else if(pBc->m_State == SPIMF_STATE_WRITE)
    {
        Int  i=0, CanWrite=0;

        // Write first burst of data to FIFO. Compute the number of words that can be 
        // written in the FIFO
        CanWrite = (TrSize > pBc->m_ConfTxFifoSize) ? pBc->m_ConfTxFifoSize
                                                          : TrSize;
        CanWrite /= 4;

        for (i=0; i<CanWrite; i++)
            SPIMF_WriteData_WRITE(pBc, *pBc->m_pData++);

        // Update number bytes to write
        pBc->m_NumToDo -= (CanWrite*4);
        
        // Enable the half-empty-interrupt if anything left after this first burst
        if (pBc->m_NumToDo > 0)
        {
            Status = DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_SPIMF_TXHALFEMPTY);
            DT_ASSERT(DT_SUCCESS(Status));
        }
    }
    
    // Issue the command, this will start the transfer
    RegControl = SPIMF_Control_READ(pBc);
    RegControl = SPIMF_Control_SET_Unlock(RegControl, 0);
    RegControl = SPIMF_Control_SET_Command(RegControl, Cmd);
    if (pBc->m_SupportsV1)
        RegControl = SPIMF_Control_SET_Timeout(RegControl, Timeout);
    SPIMF_Control_WRITE(pBc, RegControl);

    // Wait for completion of the operation, if necessary the firmware generates a timeout
    Status = DtEventWaitUnInt(&pBc->m_OpComplEvent, -1);
    if (!DT_SUCCESS(Status))
        return Status;

    // m_Result holds the result for the operation
    return pBc->m_Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_RegisterIntHandlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSPIMF_RegisterIntHandlers(DtBcSPIMF*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;
    Int  i=0;

    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);

    // Register interrupt callbacks, but do not enable the interrupts. We will enable 
    // them on demand
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;

        // Only register handler for known ID's
        switch(Id)
        {
        case DT_INTERRUPT_SPIMF_CMDDONE:
        case DT_INTERRUPT_SPIMF_RXHALFFULL:
        case DT_INTERRUPT_SPIMF_TXHALFEMPTY:
            // NOTE: the SPIMF uses one handler for all interrupt sources
            Status = DtBc_IntHandlerRegister((DtBc*)pBc, Id, 
                                                        DtBcSPIMF_InterruptHandler, NULL);
            if(!DT_SUCCESS(Status))
            {
                DtDbgOutBc(ERR, SPIMF, pBc,
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSPIMF_Init(DtBc*  pBcBase)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcSPIMF*  pBc = (DtBcSPIMF*)pBcBase;
    UInt32  RegConfig=0, RegControl=0;

    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Base initialisation -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    
    // Supports V1 interface (SFDP)
    pBc->m_SupportsV1 = (pBc->m_Version >= 1);

    // Device should not be busy, but if busy try to reset the device 
    RegControl = SPIMF_Control_READ(pBc);
    if (SPIMF_Status_READ_IsBusy(pBc) != 0)
    {
        RegControl = SPIMF_Control_SET_Command(RegControl, SPIMF_CMD_RESET);
        SPIMF_Control_WRITE(pBc, RegControl);
    }

    // Lock the device
    RegControl = SPIMF_Control_SET_Command(RegControl, SPIMF_CMD_NOP);
    RegControl = SPIMF_Control_SET_Unlock(RegControl, 0xF);
    SPIMF_Control_WRITE(pBc, RegControl);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Read flash properties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    // Cache configuration 1 register
    RegConfig = SPIMF_Config1_READ(pBc);

    // Configuration 1 - Memory-ID
    pBc->m_ConfMemoryId = SPIMF_Config1_GET_MemoryId(RegConfig);

    // Configuration 1 - Bit order
    pBc->m_ConfBitOrder = SPIMF_Config1_GET_BitOrder(RegConfig);
    // Configuration 2 - Clock-rate (read as units of Hz)
    pBc->m_ConfClockRateHz = SPIMF_Config2_READ_SpiClockRate(pBc);

    // Cache configuration 4 register
    RegConfig = SPIMF_Config4_READ(pBc);
    // Configuration 4 - Transmit-FIFO size
    pBc->m_ConfTxFifoSize = SPIMF_Config4_GET_TxFifoSize(RegConfig);
    DT_ASSERT(pBc->m_ConfTxFifoSize>0 && pBc->m_ConfTxFifoSize<(1<<16));
    // Configuration 4 - Receive-FIFO size
    pBc->m_ConfRxFifoSize = SPIMF_Config4_GET_RxFifoSize(RegConfig);
    DT_ASSERT(pBc->m_ConfRxFifoSize>0 && pBc->m_ConfRxFifoSize<(1<<16));

    DtDbgOutBc(AVG, SPIMF, pBc, "Configuration: Clock=%dHz, MemId=%d, Order=%d, "
                                "TxFifo=%d, RxFifo=%d",
                                pBc->m_ConfClockRateHz, pBc->m_ConfMemoryId,
                                pBc->m_ConfBitOrder,
                                pBc->m_ConfTxFifoSize, pBc->m_ConfRxFifoSize);

    // SPIMF V1 properties defaults; will be determined on enable
    pBc->m_FlashPropsAreSet = FALSE;
    if (!pBc->m_SupportsV1)
    { 
        // Cache configuration 3 register
        RegConfig = SPIMF_Config3_READ(pBc);
        // Configuration 3 - Page size (reads as log2(bytes))
        pBc->m_ConfPageSize = SPIMF_Config3_GET_PageSize(RegConfig);
        DT_ASSERT(pBc->m_ConfPageSize>=0 && pBc->m_ConfPageSize<32);
        pBc->m_ConfPageSize = 1 << pBc->m_ConfPageSize;
        DT_ASSERT(pBc->m_ConfPageSize > 0);
        // Configuration 3 - Sector size (reads as log2(bytes))
        pBc->m_ConfSectorSize = SPIMF_Config3_GET_SectorSize(RegConfig);
        DT_ASSERT(pBc->m_ConfSectorSize>=0 && pBc->m_ConfSectorSize<32);
        if (pBc->m_ConfSectorSize > 0)
            pBc->m_ConfSectorSize = 1 << pBc->m_ConfSectorSize;
        DT_ASSERT(pBc->m_ConfSectorSize >= 0);
        // Configuration 3 - Memory size (reads as log2(bytes))
        pBc->m_ConfMemorySize = SPIMF_Config3_GET_MemorySize(RegConfig);
        DT_ASSERT(pBc->m_ConfMemorySize>=0 && pBc->m_ConfMemorySize<32);
        pBc->m_ConfMemorySize = 1 << pBc->m_ConfMemorySize;
        DT_ASSERT(pBc->m_ConfMemorySize > 0);

        pBc->m_PageWriteTimeout = 0;
        pBc->m_BulkEraseTimeout = 0;
        pBc->m_SectorEraseTimeout = 0;
        // Flash properties for V0 are known
        pBc->m_FlashPropsAreSet = TRUE;

        // Print main properties
        DtDbgOutBc(AVG, SPIMF, pBc, 
                                  "Configuration: MemSize=%d, SectorSize=%d, PageSize=%d",
                                  pBc->m_ConfMemorySize, pBc->m_ConfSectorSize,
                                  pBc->m_ConfPageSize);

    }

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Init other attributes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    
    // Init interrupt DPC
    Status = DtDpcInit(&pBc->m_IntDpc, DtBcSPIMF_InterruptDpc, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: failed to init DPC (Status=0x%08X)", Status);
        return Status;
    }

    // Init state attributes
    pBc->m_State = SPIMF_STATE_IDLE;
    pBc->m_NumToDo = 0;
    pBc->m_pData = NULL;
    pBc->m_Result = DT_STATUS_OK;

    // Init completion event
    Status = DtEventInit(&pBc->m_OpComplEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SPIMF, pBc, 
                        "ERROR: failed to init completion event (Status=0x%08X)", Status);
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    
    Status = DtBcSPIMF_RegisterIntHandlers(pBc);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: failed to register interrupt handlers");
        return Status;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSPIMF_OnEnable(DtBc*  pBcBase, Bool  Enable)
{
    DtBcSPIMF*  pBc = (DtBcSPIMF*)pBcBase;
    if (Enable && pBc->m_SupportsV1)
    {
        // Determine and initialize the flash properties using SFDP
        DtStatus  Status = DtBcSPIMF_InitializeFlashProperties(pBc);
        DT_ASSERT(DT_SUCCESS(Status));
        // In case of failure and a DTA-2175 fallback to the default properties
        if (!DT_SUCCESS(Status) && DtCore_DEVICE_GetTypeNumber(pBcBase->m_pCore)==2175)
             Status = DtBcSPIMF_SetDefaultFlashProperties(pBc);
        pBc->m_FlashPropsAreSet = DT_SUCCESS(Status);
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_InterruptDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSPIMF_InterruptDpc(DtDpcArgs* pArgs)
{
    DtBcSPIMF*  pBc = (DtBcSPIMF*)pArgs->m_pContext;
    UInt32  IntReason = pArgs->m_Data1.m_UInt32_1;

    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);

    switch (pBc->m_State)
    {
    case SPIMF_STATE_ERASE:  DtBcSPIMF_InterruptDpcErase(pBc, IntReason); break;
    case SPIMF_STATE_READ:   DtBcSPIMF_InterruptDpcRead(pBc, IntReason);  break;
    case SPIMF_STATE_WRITE:  DtBcSPIMF_InterruptDpcWrite(pBc, IntReason); break;
    case SPIMF_STATE_IDLE:  /* No action*/;                                        break;
    default:
        DT_ASSERT(FALSE);   // Unreachable code
        break;
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_InterruptDpcErase -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSPIMF_InterruptDpcErase(DtBcSPIMF*  pBc, UInt32  Reason)
{
    Bool  Timeout = FALSE;
    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pBc->m_State == SPIMF_STATE_ERASE);

    // Check for timeout
    Timeout = (SPIMF_Status_READ_Timeout(pBc) != 0);
    if(Timeout)
    {
        pBc->m_Result = DT_STATUS_TIMEOUT;
    }

    // Signal completion
    pBc->m_State = SPIMF_STATE_IDLE;
    DtEventSet(&pBc->m_OpComplEvent);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_InterruptDpcRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSPIMF_InterruptDpcRead(DtBcSPIMF*  pBc, UInt32  Reason)
{
    Int  i=0, FifoLoad=0, CanRead=0;

    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pBc->m_State == SPIMF_STATE_READ);

    // Get FIFO-load
    FifoLoad = SPIMF_FifoLoad_READ_RxFifoLoad(pBc);
    // Compute the number of words that can be read from the FIFO
    CanRead = (pBc->m_NumToDo > FifoLoad) ? FifoLoad : pBc->m_NumToDo;
    CanRead /= 4;

    // Read data from FIFO
    for (i=0; i<CanRead; i++)
        *pBc->m_pData++ = SPIMF_ReadData_READ(pBc);

    // Update #bytes left to read
    pBc->m_NumToDo -= (CanRead*4);

    // Not yet done?
    if(Reason != DT_INTERRUPT_SPIMF_CMDDONE)
    {
        DT_ASSERT(Reason == DT_INTERRUPT_SPIMF_RXHALFFULL);
        // Enable half-full-interrupt if more than FIFO-size has to be read
        if (pBc->m_NumToDo > pBc->m_ConfRxFifoSize)
            DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_SPIMF_RXHALFFULL);
    }
    // Done interrupt
    else
    {
        // Check timeout
        Bool  Timeout = (SPIMF_Status_READ_Timeout(pBc) != 0);
        if (Timeout)
            pBc->m_Result = DT_STATUS_TIMEOUT;
        else if(pBc->m_NumToDo != 0)
        {
            DT_ASSERT(pBc->m_NumToDo == 0);
            pBc->m_Result = DT_STATUS_FAIL;
        }
        
        // Signal completion
        pBc->m_State = SPIMF_STATE_IDLE;
        DtEventSet(&pBc->m_OpComplEvent);
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_InterruptDpcWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSPIMF_InterruptDpcWrite(DtBcSPIMF*  pBc, UInt32  Reason)
{
    Int  i=0, CanWrite=0;

    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pBc->m_State == SPIMF_STATE_WRITE);

    // Not yet done?
    if(Reason != DT_INTERRUPT_SPIMF_CMDDONE)
    {
        Int  FifoFree = 0;
        DT_ASSERT(Reason == DT_INTERRUPT_SPIMF_TXHALFEMPTY);

        // Get current space in FIFO
        FifoFree = pBc->m_ConfTxFifoSize - SPIMF_FifoLoad_READ_TxFifoLoad(pBc);
        // Compute the number of words that can be written in the FIFO
        CanWrite = (pBc->m_NumToDo > FifoFree) ? FifoFree : pBc->m_NumToDo;
        CanWrite /= 4;

        // Write data to FIFO
        for (i=0; i<CanWrite; i++)
            SPIMF_WriteData_WRITE(pBc, *pBc->m_pData++);

        // Update number bytes to write
        pBc->m_NumToDo -= (CanWrite*4);

        // Enable the half empty interrupt if more data has to be written
        if (pBc->m_NumToDo > 0)
            DtBc_InterruptEnable((DtBc*)pBc, DT_INTERRUPT_SPIMF_TXHALFEMPTY);
    }
    // Done interrupt
    else
    {
        // Check timeout
        Bool  Timeout = (SPIMF_Status_READ_Timeout(pBc) != 0);
        if (Timeout)
            pBc->m_Result = DT_STATUS_TIMEOUT;
        else if (pBc->m_NumToDo != 0)
        {
            DT_ASSERT(pBc->m_NumToDo == 0);
            pBc->m_Result = DT_STATUS_FAIL;
        }
        //Signal completion event
        pBc->m_State = SPIMF_STATE_IDLE;
        DtEventSet(&pBc->m_OpComplEvent);
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_InterruptHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSPIMF_InterruptHandler(DtBc*  pBc, Int  Id, Int  Index, void*  pContext)
{
    DtStatus  Status=DT_STATUS_OK;
    DtDpcArgs  DpcArgs;

    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);

    // Check ID is one of ourss
    switch(Id)
    {
        // Known SpiMf interrupts
    case DT_INTERRUPT_SPIMF_CMDDONE:
    case DT_INTERRUPT_SPIMF_RXHALFFULL:
    case DT_INTERRUPT_SPIMF_TXHALFEMPTY:
        break;

        // Not a SpiMf interrupt
    default:
        DT_ASSERT(FALSE);   // Unreachable code
        return DT_STATUS_NOT_SUPPORTED;     
    }

    // Disable the SPI interrupt that was fired. NOTE: is re-enabled upon next transfer
    DtBc_InterruptDisable(pBc, Id);

    // Init DPC argument
    DpcArgs.m_pContext = pBc;
    DpcArgs.m_Data1.m_UInt32_1 = Id;
    DpcArgs.m_Data1.m_UInt32_2 = 0;
    
    // Schedule DPC to handle the interrupt
    Status = DtDpcSchedule(&((DtBcSPIMF*)pBc)->m_IntDpc, &DpcArgs);
    DT_ASSERT(DT_SUCCESS(Status));

    return DT_STATUS_OK;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+ Serial Flash Discoverable Parameters (SFDP) +=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
#define  SFDP_BASIC_FLASH_PARAMETER_TABLE       (0xFF00)
#define  SFDP_SECTOR_MAP_PARAMETER_TABLE        (0xFF81)
#define  SFDP_4BYTE_ADDR_INSTR_PARAMETER_TABLE  (0xFF84)

// BFP_DWORD 1
#define BFP_DW1_ADDR_LEN_MASK                   (3<<17)
#define BFP_DW1_ADDR_LEN_3_ONLY                 (0<<17)
#define BFP_DW1_ADDR_LEN_3_OR_4                 (1<<17)
#define BFP_DW1_ADDR_LEN_4_ONLY                 (2<<17)
#define BFP_DW1_SECTOR_ERASE_SIZE_MASK          (3<<0)
#define BFP_DW1_SECTOR_ERASE_4K                 (1<<0)
// BFP_DWORD 2
#define BFP_DW2_DENSITY_4G_FLAG                 (1<<31)
#define BFP_DW2_DENSITY_MASK                    (~BFP_DW2_DENSITY_4G_FLAG)
    // DWord 8
#define BFP_DW8_ERASE_TYPE2_INSTR_MASK          (0xFF<<24)
#define BFP_DW8_ERASE_TYPE2_INSTR_SHIFT         (24)
#define BFP_DW8_ERASE_TYPE2_SIZE_MASK           (0xFF<<16)
#define BFP_DW8_ERASE_TYPE2_SIZE_SHIFT          (16)
#define BFP_DW8_ERASE_TYPE1_INSTR_MASK          (0xFF<<8)
#define BFP_DW8_ERASE_TYPE1_INSTR_SHIFT         (8)
#define BFP_DW8_ERASE_TYPE1_SIZE_MASK           (0xFF<<0)
#define BFP_DW8_ERASE_TYPE1_SIZE_SHIFT          (0)
    // DWord 9
#define BFP_DW9_ERASE_TYPE4_INSTR_MASK          (0xFF<<24)
#define BFP_DW9_ERASE_TYPE4_INSTR_SHIFT         (24)
#define BFP_DW9_ERASE_TYPE4_SIZE_MASK           (0xFF<<16)
#define BFP_DW9_ERASE_TYPE4_SIZE_SHIFT          (16)
#define BFP_DW9_ERASE_TYPE3_INSTR_MASK          (0xFF<<8)
#define BFP_DW9_ERASE_TYPE3_INSTR_SHIFT         (8)
#define BFP_DW9_ERASE_TYPE3_SIZE_MASK           (0xFF<<0)
#define BFP_DW9_ERASE_TYPE3_SIZE_SHIFT          (0)
    // DWord 10
#define BFP_DW10_ERASE_TYPE4_UNIT_MASK          (3<<30)
#define BFP_DW10_ERASE_TYPE4_UNIT_SHIFT         (30)
#define BFP_DW10_ERASE_TYPE4_CNT_MASK           (0x1F<<25)
#define BFP_DW10_ERASE_TYPE4_CNT_SHIFT          (25)
#define BFP_DW10_ERASE_TYPE3_UNIT_MASK          (3<<23)
#define BFP_DW10_ERASE_TYPE3_UNIT_SHIFT         (23)
#define BFP_DW10_ERASE_TYPE3_CNT_MASK           (0x1F<<18)
#define BFP_DW10_ERASE_TYPE3_CNT_SHIFT          (18)
#define BFP_DW10_ERASE_TYPE2_UNIT_MASK          (3<<16)
#define BFP_DW10_ERASE_TYPE2_UNIT_SHIFT         (16)
#define BFP_DW10_ERASE_TYPE2_CNT_MASK           (0x1F<<11)
#define BFP_DW10_ERASE_TYPE2_CNT_SHIFT          (11)
#define BFP_DW10_ERASE_TYPE1_UNIT_MASK          (3<<9)
#define BFP_DW10_ERASE_TYPE1_UNIT_SHIFT         (9)
#define BFP_DW10_ERASE_TYPE1_CNT_MASK           (0x1F<<4)
#define BFP_DW10_ERASE_TYPE1_CNT_SHIFT          (4)
#define BFP_DW10_ERASE_MAX_TIME_CNT_MASK        (0xF<<0)
#define BFP_DW10_ERASE_MAX_TIME_CNT_SHIFT       (0)

// BFP_DWORD 11
#define BFP_DW11_CHIP_ERASE_TIME_MASK           (3<<29)
#define BFP_DW11_CHIP_ERASE_TIME_16MS           (0<<29)
#define BFP_DW11_CHIP_ERASE_TIME_256MS          (1<<29)
#define BFP_DW11_CHIP_ERASE_TIME_4SEC           (2<<29)
#define BFP_DW11_CHIP_ERASE_TIME_64SEC          (3<<29)
#define BFP_DW11_CHIP_ERASE_TIME_CNT_MASK       (0x1F<<24)
#define BFP_DW11_CHIP_ERASE_TIME_CNT_SHIFT      (24)
#define BFP_DW11_PAGE_PROG_TIME_MASK            (1<<13)
#define BFP_DW11_PAGE_PROG_TIME_8US             (0<<13)
#define BFP_DW11_PAGE_PROG_TIME_16US            (1<<13) 
#define BFP_DW11_PAGE_PROG_TIME_CNT_MASK        (0x1F<<8)
#define BFP_DW11_PAGE_PROG_TIME_CNT_SHIFT       (8)
#define BFP_DW11_PAGE_SIZE_MASK                 (0xF<<4)
#define BFP_DW11_PAGE_SIZE_SHIFT                (4)
#define BFP_DW11_PAGE_PROG_MAX_TIME_CNT_MASK    (0xF<<0)
#define BFP_DW11_PAGE_PROG_MAX_TIME_CNT_SHIFT   (0)

#define BFP_ERASE_TYPE4_UNIT_1MS                0
#define BFP_ERASE_TYPE4_UNIT_16MS               1
#define BFP_ERASE_TYPE4_UNIT_128MS              2
#define BFP_ERASE_TYPE4_UNIT_1SEC               3
// BFP_DWORD 16
#define BFP_DW16_SOFT_RESET_SUPPORT_MASK        (0x3F<<8)
#define BFP_DW16_SOFT_RESET_SUPPORT_NONE        (0x00)
#define BFP_DW16_SOFT_RESET_SUPPORT_F0H         (0x08<<8)
#define BFP_DW16_SOFT_RESET_SUPPORT_66H_99H     (0x10<<8)

// A4P_DWORD 1
#define A4P_DW1_TYPE4_ERASE_COMMAND_SUPPORT     (1<<12)
#define A4P_DW1_TYPE3_ERASE_COMMAND_SUPPORT     (1<<11)
#define A4P_DW1_TYPE2_ERASE_COMMAND_SUPPORT     (1<<10)
#define A4P_DW1_TYPE1_ERASE_COMMAND_SUPPORT     (1<<9)
#define A4P_DW1_PAGE_PROGRAM_COMMAND_SUPPORT    (1<<6)
#define A4P_DW1_READ_COMMAND_SUPPORT            (1<<0)

// A4P_DWORD 2
#define A4P_DW2_ERASE_TYPE4_INSTR_MASK          (0xFF<<24)
#define A4P_DW2_ERASE_TYPE4_INSTR_SHIFT         (24)
#define A4P_DW2_ERASE_TYPE3_INSTR_MASK          (0xFF<<16)
#define A4P_DW2_ERASE_TYPE3_INSTR_SHIFT         (16)
#define A4P_DW2_ERASE_TYPE2_INSTR_MASK          (0xFF<<8)
#define A4P_DW2_ERASE_TYPE2_INSTR_SHIFT         (8)
#define A4P_DW2_ERASE_TYPE1_INSTR_MASK          (0xFF<<0)
#define A4P_DW2_ERASE_TYPE1_INSTR_SHIFT         (0)

// Flash properties
enum DtBcSPIMF_FlashProperties
{
    DT_BC_SPIMF_PROP_INSTR_READ_SFDP = 0,   // Program opcode for SFDP read instruction
    DT_BC_SPIMF_PROP_INSTR_BULK_ERASE,      // Program opcode for bulk-erase instruction
    DT_BC_SPIMF_PROP_INSTR_PAGE_PROGRAM,    // Program opcode for page-programinstruction
    DT_BC_SPIMF_PROP_INSTR_READ,            // Program opcode for read instruction
    DT_BC_SPIMF_PROP_INSTR_READ_STATUS,     // Program opcode for read-status instruction
    DT_BC_SPIMF_PROP_INSTR_RESET,           // Program opcode for reset instruction
    DT_BC_SPIMF_PROP_INSTR_RESET_ENABLE,    // Program opcode for reset-enable instruction
    DT_BC_SPIMF_PROP_INSTR_SECTOR_ERASE,    // Program opcode for sector-erase instruction
    DT_BC_SPIMF_PROP_INSTR_WRITE_ENABLE,    // Program opcode for write-enable instruction
    DT_BC_SPIMF_PROP_SIZE_PAGE_LOG2BYTES,   // Page size expressed as log2(bytes)
    DT_BC_SPIMF_PROP_SIZE_SECTOR_LOG2BYTES, // Sector size expressed as log2(bytes)
    DT_BC_SPIMF_PROP_SIZE_MEM_LOG2BYTES,    // Size of flash memory expressed as log2
    DT_BC_SPIMF_PROP_SIZE_ADDR_BYTES,       // Size of address expressed in bytes
    DT_BC_SPIMF_PROP_COUNT,
};

// Default flash instructions
#define DT_BC_SPIMF_INSTR_BULK_ERASE        0xC7    // Bulk erase (not fixed)
#define DT_BC_SPIMF_INSTR_PAGE_PROGRAM      0x02    // Page Program
#define DT_BC_SPIMF_INSTR_PAGE_PROGRAM_4B   0x12    // Page Program 4-Byte Addressing
#define DT_BC_SPIMF_INSTR_READ              0x03    // Read
#define DT_BC_SPIMF_INSTR_READ_4B           0x13    // Read 4-Byte Addressing
#define DT_BC_SPIMF_INSTR_READ_SFDP         0x5A    // Read SFDP
#define DT_BC_SPIMF_INSTR_READ_STATUS       0x05    // Read Status
#define DT_BC_SPIMF_INSTR_RESET             0x99    // Reset (not fixed)
#define DT_BC_SPIMF_INSTR_RESET_ENABLE      0x66    // Reset enable (not fixed)
#define DT_BC_SPIMF_INSTR_SECTOR_ERASE      0xD8    // Sector erase (not fixed)
#define DT_BC_SPIMF_INSTR_WRITE_ENABLE      0x06    // Write Enable

// SFDP parameter header
typedef struct _DtBcSPIMF__SfdpParameterHeader 
{
    UInt8  m_IdLsb;
    UInt8  m_Minor;
    UInt8  m_Major;
    UInt8  m_Length;            // Length in dwords
    UInt8  m_ParTablePtr[3];    // byte address
    UInt8  m_IdMsb;
}DtBcSPIMF_SfdpParameterHeader;

// SFDP header
typedef struct _DtBcSPIMF_SfdpHeader
{
    UInt32  m_Signature;        // Ox50444653U <=> "SFDP"
    UInt8  m_Minor;
    UInt8  m_Major;
    UInt8  m_NumPars;           // 0-base number of parameter headers 
    UInt8  m_AccesProtocol;
}DtBcSPIMF_SfdpHeader;


// -.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_InitializeFlashProperties -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSPIMF_InitializeFlashProperties(DtBcSPIMF*  pBc)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt64  Size=0;
    UInt  TimeoutUs=0, SectorSize=0, EraseType=0;
    UInt32* pParamTable = NULL;
    UInt8* pBuffer = NULL;
    const Int  TableSize = 256;
    Int  TableLength;
    UInt32  FlashProps[DT_BC_SPIMF_PROP_COUNT] = {0};
    Bool  SectorMapTablePresent = FALSE;
    Bool  Uses4ByteAddressing = FALSE;
    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);
    
    // Not applicable for SPIMF V0
    if (!pBc->m_SupportsV1)
        return DT_STATUS_OK;

    // Allocate memory for the Parameter Tables
    pParamTable = (UInt32*)DtMemAllocPool(DtPoolNonPaged, sizeof(UInt32)*TableSize,
                                                                              COMMON_TAG);
    if (pParamTable == NULL)
        return  DT_STATUS_OUT_OF_MEMORY; 
    pBuffer = (UInt8*)pParamTable;

     // Initialize the SFDP instruction
    FlashProps[DT_BC_SPIMF_PROP_INSTR_READ_SFDP] = DT_BC_SPIMF_INSTR_READ_SFDP;
    DtBcSPIMF_SetFlashProperties(pBc, FlashProps);

   // Check whether there is a Sector Map Table present.
    // If there is, possibly not all erase type commands can be used for all sectors.
    // Currently this becomes to complicated and we try to fall back to the 4k sectors.
    TableLength = TableSize;
    Status = DtBcSPIMF_ReadParamTable(pBc, SFDP_SECTOR_MAP_PARAMETER_TABLE, 1, 0,
                                                               pParamTable, &TableLength);
    SectorMapTablePresent = (Status == DT_STATUS_OK);

    // Read the Basic Flash Parameter Table (Version 1.5 or higher)
    TableLength = TableSize;
    Status = DtBcSPIMF_ReadParamTable(pBc, SFDP_BASIC_FLASH_PARAMETER_TABLE, 1, 5,
                                                               pParamTable, &TableLength);
    if (DT_SUCCESS(Status) && TableLength<16)
    {
        DtMemFreePool(pParamTable, COMMON_TAG);
        return DT_STATUS_NOT_FOUND;
    }
    else if(!DT_SUCCESS(Status))
    {
        DtMemFreePool(pParamTable, COMMON_TAG);
        return Status;
    }

    // .-.-.-.-.-.-.-.-.-.-.-.- Determine  flash size properties -.-.-.-.-.-.-.-.-.-.-.-.-
    FlashProps[DT_BC_SPIMF_PROP_SIZE_ADDR_BYTES] = DtBcSPIMF_GetAddressSize(pBuffer);
    Size = DtBcSPIMF_GetMemorySize(pBuffer);
    FlashProps[DT_BC_SPIMF_PROP_SIZE_MEM_LOG2BYTES] = DtBcSPIMF_Log2(Size);
    Size = DtBcSPIMF_GetPageSize(pBuffer);
    FlashProps[DT_BC_SPIMF_PROP_SIZE_PAGE_LOG2BYTES] = DtBcSPIMF_Log2(Size);

    // Determine sector size that can be used
    DtBcSPIMF_DetermineSectorSize(pBuffer,SectorMapTablePresent, &SectorSize, &EraseType);
    if (SectorSize > 0)
        FlashProps[DT_BC_SPIMF_PROP_SIZE_SECTOR_LOG2BYTES] = DtBcSPIMF_Log2(SectorSize);
    else
        FlashProps[DT_BC_SPIMF_PROP_SIZE_SECTOR_LOG2BYTES] = 0;

    // .-.-.-.-.-.-.-.-.-.-.-.-.-.- Determine flash timeouts -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    // Chip erase timeout
    TimeoutUs = DtBcSPIMF_GetMaximumChipEraseTimeUs(pBuffer);
    pBc->m_BulkEraseTimeout = DtBcSPIMF_TimeoutUsToLog2Ms(TimeoutUs);
    // Sector erase timeout
    if (EraseType > 0)
    { 
        TimeoutUs = DtBcSPIMF_GetMaximumSectorEraseTimeUs(pBuffer, EraseType);
        pBc->m_SectorEraseTimeout = DtBcSPIMF_TimeoutUsToLog2Ms(TimeoutUs);
    }
    else
        pBc->m_SectorEraseTimeout  = 0;
    // Page program timeout
    TimeoutUs = DtBcSPIMF_GetMaximumPageProgramTimeUs(pBuffer);
    pBc->m_PageWriteTimeout = DtBcSPIMF_TimeoutUsToLog2Ms(TimeoutUs);

    // .-.-.-.-.-.-.-.-.-.-.-.-.- Determine flash instructions -.-.-.-.-.-.-.-.-.-.-.-.-.-

    // Set fixed instructions
    FlashProps[DT_BC_SPIMF_PROP_INSTR_BULK_ERASE] = DT_BC_SPIMF_INSTR_BULK_ERASE;
    FlashProps[DT_BC_SPIMF_PROP_INSTR_READ_SFDP] = DT_BC_SPIMF_INSTR_READ_SFDP;
    FlashProps[DT_BC_SPIMF_PROP_INSTR_READ_STATUS] = DT_BC_SPIMF_INSTR_READ_STATUS;
    FlashProps[DT_BC_SPIMF_PROP_INSTR_WRITE_ENABLE] = DT_BC_SPIMF_INSTR_WRITE_ENABLE;

    // Determine the reset instructions
    DtBcSPIMF_GetSoftResetInstr(pBuffer, &FlashProps[DT_BC_SPIMF_PROP_INSTR_RESET_ENABLE],
                                         &FlashProps[DT_BC_SPIMF_PROP_INSTR_RESET]);
    Uses4ByteAddressing = (FlashProps[DT_BC_SPIMF_PROP_SIZE_ADDR_BYTES] == 4);
    if (!Uses4ByteAddressing)
    {
        // Determine the 3-byte addressing instructions
        Status = DtBcSPIMF_Determine3ByteAddrInstr(pBuffer, EraseType, FlashProps);
    } else {
        // Read the 4-Byte Addressing Parameter Table
        TableLength = TableSize;
        Status = DtBcSPIMF_ReadParamTable(pBc, SFDP_4BYTE_ADDR_INSTR_PARAMETER_TABLE, 
                                                         1, 0, pParamTable, &TableLength);
        if (DT_SUCCESS(Status) && TableLength<2)
        {
            DtMemFreePool(pParamTable, COMMON_TAG);
            return DT_STATUS_NOT_FOUND;
        }
        else if(!DT_SUCCESS(Status))
        {
            DtMemFreePool(pParamTable, COMMON_TAG);
            return Status;
        }
        // Determine the 4-byte addressing instructions
        Status = DtBcSPIMF_Determine4ByteAddrInstr(pBuffer, EraseType, FlashProps);
    }
    if (!DT_SUCCESS(Status))
    {
        DtMemFreePool(pParamTable, COMMON_TAG);
        return Status;
    }

    // Set the flash properties
    DtBcSPIMF_SetFlashProperties(pBc, FlashProps);

    // Cache main flash properties
    pBc->m_ConfMemorySize = 
                (Int)DtBcSPIMF_Exp2((Int)FlashProps[DT_BC_SPIMF_PROP_SIZE_MEM_LOG2BYTES]);
    pBc->m_ConfSectorSize = 
             (Int)DtBcSPIMF_Exp2((Int)FlashProps[DT_BC_SPIMF_PROP_SIZE_SECTOR_LOG2BYTES]);
    pBc->m_ConfPageSize = 
               (Int)DtBcSPIMF_Exp2((Int)FlashProps[DT_BC_SPIMF_PROP_SIZE_PAGE_LOG2BYTES]);

    // Print main flash properties
    DtDbgOutBc(AVG, SPIMF, pBc, "Configuration: MemSize=%d, SectorSize=%d, PageSize=%d",
                                pBc->m_ConfMemorySize, pBc->m_ConfSectorSize,
                                pBc->m_ConfPageSize);

    // Free Basic Parameter Table
    DtMemFreePool(pParamTable, COMMON_TAG);

    return DT_STATUS_OK;
}
// -.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_SetDefaultFlashProperties -.-.-.-.-.-.-.-.-.-.-.-.-
//
// As workaround for setting the DTA-2175  N25Q064A flash properties
//
DtStatus DtBcSPIMF_SetDefaultFlashProperties(DtBcSPIMF* pBc)
{
    const Int  EepromSize = 8*1024*1024;    // 8 MByte eeprom
    const Int  SectorSize = 64*1024;        // 64kByte sectors
    const Int  PageSize = 256;              // 256Byte pages
    UInt32  FlashProps[DT_BC_SPIMF_PROP_COUNT] = {0};
    // Not applicable for SPIMF V0
    if (!pBc->m_SupportsV1)
        return DT_STATUS_OK;
    // Set default flash properties
    FlashProps[DT_BC_SPIMF_PROP_INSTR_READ_SFDP]       = DT_BC_SPIMF_INSTR_READ_SFDP;
    FlashProps[DT_BC_SPIMF_PROP_INSTR_BULK_ERASE]      = DT_BC_SPIMF_INSTR_BULK_ERASE;
    FlashProps[DT_BC_SPIMF_PROP_INSTR_PAGE_PROGRAM]    = DT_BC_SPIMF_INSTR_PAGE_PROGRAM;
    FlashProps[DT_BC_SPIMF_PROP_INSTR_READ]            = DT_BC_SPIMF_INSTR_READ;
    FlashProps[DT_BC_SPIMF_PROP_INSTR_READ_STATUS]     = DT_BC_SPIMF_INSTR_READ_STATUS;
    FlashProps[DT_BC_SPIMF_PROP_INSTR_RESET]           = DT_BC_SPIMF_INSTR_RESET;
    FlashProps[DT_BC_SPIMF_PROP_INSTR_RESET_ENABLE]    = DT_BC_SPIMF_INSTR_RESET_ENABLE;
    FlashProps[DT_BC_SPIMF_PROP_INSTR_SECTOR_ERASE]    = DT_BC_SPIMF_INSTR_SECTOR_ERASE;
    FlashProps[DT_BC_SPIMF_PROP_INSTR_WRITE_ENABLE]    = DT_BC_SPIMF_INSTR_WRITE_ENABLE;
    FlashProps[DT_BC_SPIMF_PROP_SIZE_PAGE_LOG2BYTES]   = DtBcSPIMF_Log2(PageSize);
    FlashProps[DT_BC_SPIMF_PROP_SIZE_SECTOR_LOG2BYTES] = DtBcSPIMF_Log2(SectorSize);
    FlashProps[DT_BC_SPIMF_PROP_SIZE_MEM_LOG2BYTES]    = DtBcSPIMF_Log2(EepromSize);
    FlashProps[DT_BC_SPIMF_PROP_SIZE_ADDR_BYTES]       = 3;
    // Set the flash properties
    DtBcSPIMF_SetFlashProperties(pBc, FlashProps);
    // Configure timeouts
    pBc->m_BulkEraseTimeout = 20;       // 2^20 ms (1049 seconds)
    pBc->m_SectorEraseTimeout = 11;     // 2^11 ms  (2 seconds)
    pBc->m_PageWriteTimeout  = 2;       // 2^2 ms   (4 milliseconds)
    // Cache main flash properties
    pBc->m_ConfMemorySize = EepromSize;
    pBc->m_ConfSectorSize = SectorSize;
    pBc->m_ConfPageSize = PageSize;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_ReadParamTable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSPIMF_ReadParamTable(DtBcSPIMF*  pBc, Int TableId, Int Major, Int Minor,
                                                             UInt32* pTable, Int* pLength)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  SfdpAddress=0, SfdpReadSize=0, SfdpNumDWords=0, NumParHeaders=0, i=0;
    Bool SfdpHdrFound = FALSE;
    DtBcSPIMF_SfdpHeader  SfdpHeader;
    DtBcSPIMF_SfdpParameterHeader* pSfdpParamTableHeaders = NULL;

    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);
    
    // Not applicable for SPIMF V0
    DT_ASSERT (pBc->m_SupportsV1);

    // Read the SFDP header
    Status = DtBcSPIMF_ExecuteCommand(pBc, SPIMF_CMD_READ_SFDP, 0, 
                                       sizeof(DtBcSPIMF_SfdpHeader), (UInt8*)&SfdpHeader);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: read-sfdp failed (status=0x%X)", Status);
        return Status;
    }

    // Check SFDP-signature
    if (SfdpHeader.m_Signature != 0x50444653 /*"SFDP"*/)
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: invalid sfdp signature");
        return DT_STATUS_FAIL;
    }

    // Determine number of SFDP parameter structures
    NumParHeaders = SfdpHeader.m_NumPars + 1;
    SfdpAddress = sizeof(DtBcSPIMF_SfdpHeader);
    SfdpReadSize = sizeof(DtBcSPIMF_SfdpParameterHeader) * NumParHeaders;

    // Allocate memory for the SFDP parameter structures
    pSfdpParamTableHeaders = (DtBcSPIMF_SfdpParameterHeader*)
                                 DtMemAllocPool(DtPoolNonPaged, SfdpReadSize, COMMON_TAG);
    if (pSfdpParamTableHeaders == NULL)
        return DT_STATUS_OUT_OF_MEMORY;
    
    // Read the SFDP parameter headers
    Status = DtBcSPIMF_ExecuteCommand(pBc, SPIMF_CMD_READ_SFDP, SfdpAddress, SfdpReadSize,
                                                          (UInt8*)pSfdpParamTableHeaders);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: read-sfdp failed (status=0x%X)", Status);
        // Free SFDP parameter structures
        DtMemFreePool(pSfdpParamTableHeaders, COMMON_TAG);
        return Status;
    }
    // Find the Basic Flash Parameter Table
    SfdpHdrFound = FALSE;
    for (i = 0; i<NumParHeaders && !SfdpHdrFound; i++)
    {
        Int Id = (pSfdpParamTableHeaders[i].m_IdMsb<<8) 
                                                      + pSfdpParamTableHeaders[i].m_IdLsb;
        Int MajorVers = pSfdpParamTableHeaders[i].m_Major;
        Int MinorVers = pSfdpParamTableHeaders[i].m_Minor;
        if (Id == TableId)
        {
            // Skip old versions
            if (MajorVers<Major || (MajorVers==Major && MinorVers<Minor))
                continue;
            SfdpAddress =    (pSfdpParamTableHeaders[i].m_ParTablePtr[2] << 16)
                           + (pSfdpParamTableHeaders[i].m_ParTablePtr[1] << 8)
                           + (pSfdpParamTableHeaders[i].m_ParTablePtr[0]);
            SfdpNumDWords = pSfdpParamTableHeaders[i].m_Length;
            SfdpHdrFound = TRUE;
        }
    }
    // Free SFDP parameter structures
    DtMemFreePool(pSfdpParamTableHeaders, COMMON_TAG);
    pSfdpParamTableHeaders = NULL;
    
    if (!SfdpHdrFound)
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: flash parameters not found");
        return DT_STATUS_FAIL;
    }
    if (SfdpNumDWords < 1)
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: flash parameters too short");
        return DT_STATUS_FAIL;
    }
    if (SfdpNumDWords > *pLength)
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: buffer too small");
        return DT_STATUS_BUF_TOO_SMALL;
    }

    // Read the SFDP basic flash parameter table
    SfdpReadSize = SfdpNumDWords * sizeof(UInt32);
    Status = DtBcSPIMF_ExecuteCommand(pBc, SPIMF_CMD_READ_SFDP, SfdpAddress, SfdpReadSize,
                                                                          (UInt8*)pTable);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: read-sfdp failed (status=0x%X)", Status);
        return Status;
    }
    *pLength = SfdpNumDWords;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_Determine3ByteAddrInstr -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSPIMF_Determine3ByteAddrInstr(UInt8*  pBuffer, Int EraseType, 
                                                                       UInt32* FlashProps)
{
    // Use the 3-Byte addressing instructions
    FlashProps[DT_BC_SPIMF_PROP_INSTR_PAGE_PROGRAM] = DT_BC_SPIMF_INSTR_PAGE_PROGRAM;
    FlashProps[DT_BC_SPIMF_PROP_INSTR_READ] = DT_BC_SPIMF_INSTR_READ;
    FlashProps[DT_BC_SPIMF_PROP_INSTR_SECTOR_ERASE] = 
                                        DtBcSPIMF_GetSectorEraseInstr(pBuffer, EraseType);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_Determine4ByteAddrInstr -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSPIMF_Determine4ByteAddrInstr(UInt8*  pBuffer, Int EraseType, 
                                                                       UInt32* FlashProps)
{
    UInt32  DWord1 =  DtBcSPIMF_ReadDWord(pBuffer, 1);
    UInt32  DWord2 =  DtBcSPIMF_ReadDWord(pBuffer, 2);
    Bool  Supported = FALSE;
    UInt32  EraseInstr = 0;

    // Check whether commands are supported
    if ((DWord1 & A4P_DW1_PAGE_PROGRAM_COMMAND_SUPPORT) == 0)
        return DT_STATUS_NOT_SUPPORTED;
    if ((DWord1 & A4P_DW1_READ_COMMAND_SUPPORT) == 0)
        return DT_STATUS_NOT_SUPPORTED;

    // Use the 4-Byte addressing instructions
    FlashProps[DT_BC_SPIMF_PROP_INSTR_PAGE_PROGRAM] = DT_BC_SPIMF_INSTR_PAGE_PROGRAM_4B;
    FlashProps[DT_BC_SPIMF_PROP_INSTR_READ] = DT_BC_SPIMF_INSTR_READ_4B;

    switch (EraseType)
    {
    case 0:
        Supported = TRUE;  // Sector erase is not used. Supported is n.a.
        EraseInstr = 0;
        break;
    case 1:
        Supported = (DWord1 & A4P_DW1_TYPE1_ERASE_COMMAND_SUPPORT) != 0;
        EraseInstr = (DWord2 & A4P_DW2_ERASE_TYPE1_INSTR_MASK)
                                                       >> A4P_DW2_ERASE_TYPE1_INSTR_SHIFT;
        break;
    case 2:
        Supported = (DWord1 & A4P_DW1_TYPE2_ERASE_COMMAND_SUPPORT) != 0;
        EraseInstr = (DWord2 & A4P_DW2_ERASE_TYPE2_INSTR_MASK)
                                                       >> A4P_DW2_ERASE_TYPE2_INSTR_SHIFT;
        break;
    case 3:
        Supported = (DWord1 & A4P_DW1_TYPE3_ERASE_COMMAND_SUPPORT) != 0;
        EraseInstr = (DWord2 & A4P_DW2_ERASE_TYPE3_INSTR_MASK)
                                                       >> A4P_DW2_ERASE_TYPE3_INSTR_SHIFT;
        break;
    case 4:
        Supported = (DWord1 & A4P_DW1_TYPE4_ERASE_COMMAND_SUPPORT) != 0;
        EraseInstr = (DWord2 & A4P_DW2_ERASE_TYPE4_INSTR_MASK)
                                                       >> A4P_DW2_ERASE_TYPE4_INSTR_SHIFT;
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_INVALID_PARAMETER;
    }

    if (!Supported)
        return DT_STATUS_NOT_SUPPORTED;

    // Store sector erase instruction
    FlashProps[DT_BC_SPIMF_PROP_INSTR_SECTOR_ERASE] = EraseInstr;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_DetermineSectorSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSPIMF_DetermineSectorSize(UInt8* pBuffer, Bool SectorMapTablePresent,
                                                            UInt *pSize, UInt* pEraseType)
{
    Bool Supports4kErase = FALSE;
    UInt i, EraseType, SectorSize;
    UInt32  DWord1;

    DT_ASSERT(pBuffer!=NULL && pSize!=NULL && pEraseType!=NULL);

    // Check whether 4 kilobyte Erase is supported throughout the device
    DWord1 = DtBcSPIMF_ReadDWord(pBuffer, 1);
    Supports4kErase = (DWord1&BFP_DW1_SECTOR_ERASE_SIZE_MASK) == BFP_DW1_SECTOR_ERASE_4K;

    EraseType = 0;
    SectorSize = 0;
    if (SectorMapTablePresent && Supports4kErase)
    {
        // Find the 4k sector erase type
        for (i = 1; i<=4; i++)
        {
            UInt32 Size = DtBcSPIMF_GetSectorSize(pBuffer, i);
            if (Size == 4096)
            {
                EraseType = i;
                SectorSize = Size;
                break;
            }
        }
    } 
    else if (!SectorMapTablePresent)
    { 
        // Use the largest sector size
        for (i = 1; i<=4; i++)
        {
            UInt32 Size = DtBcSPIMF_GetSectorSize(pBuffer, i);
            if (Size > SectorSize)
            {
                EraseType = i;
                SectorSize = Size;
            }
        }
    }
    else
    {
        // No sector erase supported
        EraseType = 0;
        SectorSize = 0;
    }
    *pSize = SectorSize;
    *pEraseType = EraseType;
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_GetAddressSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Returns address size in number of bytes
//
UInt  DtBcSPIMF_GetAddressSize(UInt8* pBuffer)
{
    UInt32  DWord1 =  DtBcSPIMF_ReadDWord(pBuffer, 1);
    switch(DWord1 & BFP_DW1_ADDR_LEN_MASK)
    {
        case BFP_DW1_ADDR_LEN_3_ONLY:   return 3;
        case BFP_DW1_ADDR_LEN_3_OR_4:   return 4;
        case BFP_DW1_ADDR_LEN_4_ONLY:   return 4;
        default: DT_ASSERT(FALSE);      return 3;
    }
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_GetMaximumChipEraseTimeUs -.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt  DtBcSPIMF_GetMaximumChipEraseTimeUs(UInt8*  pBuffer)
{
    return DtBcSPIMF_GetTypicalChipEraseTimeUs(pBuffer) 
                                          * DtBcSPIMF_GetTypToMaxEraseTimeFactor(pBuffer);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_GetMaximumPageProgramTimeUs -.-.-.-.-.-.-.-.-.-.-.-.
//
UInt  DtBcSPIMF_GetMaximumPageProgramTimeUs(UInt8* pBuffer)
{
    UInt32  DWord11 =  DtBcSPIMF_ReadDWord(pBuffer, 11);
    UInt ProgTimeout=0, Count=0;
    // Formula maximum time = 2 * (count + 1)*typical time 
    Count = (DWord11&BFP_DW11_PAGE_PROG_MAX_TIME_CNT_MASK)
                                                 >> BFP_DW11_PAGE_PROG_MAX_TIME_CNT_SHIFT;
    ProgTimeout =  2 * (Count + 1) * DtBcSPIMF_GetTypicalPageProgramTimeUs(pBuffer);
    return ProgTimeout;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_GetMaximumSectorEraseTimeUs -.-.-.-.-.-.-.-.-.-.-.-.
//
UInt  DtBcSPIMF_GetMaximumSectorEraseTimeUs(UInt8*  pBuffer, Int EraseType)
{
    return DtBcSPIMF_GetTypicalSectorEraseTimeUs(pBuffer, EraseType) 
                                          * DtBcSPIMF_GetTypToMaxEraseTimeFactor(pBuffer);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_GetMemorySize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Returns the memory size in bytes
//
UInt64  DtBcSPIMF_GetMemorySize(UInt8* pBuffer)
{
    UInt32  DWord2 =  DtBcSPIMF_ReadDWord(pBuffer, 2);    
    UInt64 MemSize = 0;
    // For densities 2 gigabits or less, bit-31 is set to 0b.  The field 30:0 defines 
    // the size in bits. Example: 00FFFFFFh = 16 megabits 
    // For densities 4 gigabits and above, bit-31 is set to 1. The field 30:0 defines ‘N’
    // where the density is computed as 2^N bits (N must be >= 32). 
    // Example: 80000021h = 2^33 = 8 gigabits 
    if ((DWord2&BFP_DW2_DENSITY_4G_FLAG) == 0)
         MemSize  = DWord2 + 1;
    else
        MemSize  = DtBcSPIMF_Exp2(DWord2 & BFP_DW2_DENSITY_MASK);
    // Should contain a whole number of bytes
    DT_ASSERT((MemSize & 0x7) == 0);
    return (MemSize>>3);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_GetPageSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns the memory size in bytes
//
UInt  DtBcSPIMF_GetPageSize(UInt8* pBuffer)
{
    UInt32  DWord11 = DtBcSPIMF_ReadDWord(pBuffer, 11);
    // This field specifies 'N' and is used to calculate page size = 2^N bytes. 
    return (UInt)DtBcSPIMF_Exp2((DWord11 & BFP_DW11_PAGE_SIZE_MASK)
                                                             >> BFP_DW11_PAGE_SIZE_SHIFT);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_GetSectorEraseInstr -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns the sector erase instruction (3-byte addressing) for a given erase type
//
UInt  DtBcSPIMF_GetSectorEraseInstr(UInt8*  pBuffer, Int EraseType)
{
    UInt32  DWord8 = DtBcSPIMF_ReadDWord(pBuffer, 8);
    UInt32  DWord9 = DtBcSPIMF_ReadDWord(pBuffer, 9);
    
    UInt  Instr = 0;

    switch (EraseType)
    {
    case 1:
        Instr = ((DWord8 & BFP_DW8_ERASE_TYPE1_INSTR_MASK)
                                                      >> BFP_DW8_ERASE_TYPE1_INSTR_SHIFT);
        break;
    case 2:
        Instr = ((DWord8 & BFP_DW8_ERASE_TYPE2_INSTR_MASK)
                                                      >> BFP_DW8_ERASE_TYPE2_INSTR_SHIFT);
        break;
    case 3:
        Instr = ((DWord9 & BFP_DW9_ERASE_TYPE3_INSTR_MASK)
                                                      >> BFP_DW9_ERASE_TYPE3_INSTR_SHIFT);
        break;
    case 4:
        Instr = ((DWord9 & BFP_DW9_ERASE_TYPE4_INSTR_MASK)
                                                      >> BFP_DW9_ERASE_TYPE4_INSTR_SHIFT);
        break;
    }
    return Instr;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_GetSectorSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Returns the sector size in bytes for a given erase type or 0 if not supported
//
UInt  DtBcSPIMF_GetSectorSize(UInt8* pBuffer, Int EraseType)
{
    UInt32  DWord8 = DtBcSPIMF_ReadDWord(pBuffer, 8);
    UInt32  DWord9 = DtBcSPIMF_ReadDWord(pBuffer, 9);
    
    UInt  SectorSize=0, Size=0;

    switch (EraseType)
    {
    case 1:
        Size = ((DWord8&BFP_DW8_ERASE_TYPE1_SIZE_MASK) >> BFP_DW8_ERASE_TYPE1_SIZE_SHIFT);
        break;
    case 2:
        Size = ((DWord8&BFP_DW8_ERASE_TYPE2_SIZE_MASK) >> BFP_DW8_ERASE_TYPE2_SIZE_SHIFT);
        break;
    case 3:
        Size = ((DWord9&BFP_DW9_ERASE_TYPE3_SIZE_MASK) >> BFP_DW9_ERASE_TYPE3_SIZE_SHIFT);
        break;
    case 4:
        Size = ((DWord9&BFP_DW9_ERASE_TYPE4_SIZE_MASK) >> BFP_DW9_ERASE_TYPE4_SIZE_SHIFT);
        break;
    }
    if (Size > 0)
        SectorSize = (Int)DtBcSPIMF_Exp2(Size);
    return SectorSize;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_GetSoftResetInstr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
//
void  DtBcSPIMF_GetSoftResetInstr(UInt8* pBuffer, UInt32* pResetEnable, 
                                                                      UInt32* pResetInstr)
{
    // Soft reset and rescue sequence support
    UInt32  DWord16 =  DtBcSPIMF_ReadDWord(pBuffer, 16);
    UInt32  SofResetSupport = DWord16 & BFP_DW16_SOFT_RESET_SUPPORT_MASK;
    *pResetEnable = 0;
    *pResetInstr = 0;
    if ((SofResetSupport & BFP_DW16_SOFT_RESET_SUPPORT_66H_99H) != 0)
    {
        *pResetEnable = 0x66;
        *pResetInstr = 0x99;
    }
    else if ((SofResetSupport & BFP_DW16_SOFT_RESET_SUPPORT_F0H) != 0)
    {
        *pResetEnable = 0x00;
        *pResetInstr = 0xF0;
    }
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_GetTypicalChipEraseTimeUs -.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt DtBcSPIMF_GetTypicalChipEraseTimeUs(UInt8*  pBuffer)
{
    UInt32  DWord11 =  DtBcSPIMF_ReadDWord(pBuffer, 11);
    UInt Timeout=0, Count=0;
    
    // Formula: typical time = (count + 1)*units 
    switch (DWord11 & BFP_DW11_CHIP_ERASE_TIME_MASK)
    {
        case BFP_DW11_CHIP_ERASE_TIME_16MS:  Timeout =      16*1000; break;
        case BFP_DW11_CHIP_ERASE_TIME_256MS: Timeout =     256*1000; break;
        case BFP_DW11_CHIP_ERASE_TIME_4SEC:  Timeout =  4*1000*1000; break;
        case BFP_DW11_CHIP_ERASE_TIME_64SEC: Timeout = 64*1000*1000; break;
    }
    // Formula: typical time = (count + 1)*units 
    Count = (DWord11 & BFP_DW11_CHIP_ERASE_TIME_CNT_MASK)
                                                    >> BFP_DW11_CHIP_ERASE_TIME_CNT_SHIFT;
    Timeout = Timeout * (Count + 1);
    return Timeout;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_GetTypicalPageProgramTimeUs -.-.-.-.-.-.-.-.-.-.-.-.
//
UInt  DtBcSPIMF_GetTypicalPageProgramTimeUs(UInt8* pBuffer)
{
    UInt32  DWord11 =  DtBcSPIMF_ReadDWord(pBuffer, 11);
    UInt ProgTimeout=0, Count=0;
    //Formula: typical page program time = (count + 1)*units The range is 8 µs to 2048 µs 
    //in two groups: 8 µs to 256 µs and 64 µs to 2048 µs 
    // Typical page program time                
    if ((DWord11 & BFP_DW11_PAGE_PROG_TIME_MASK) == BFP_DW11_PAGE_PROG_TIME_16US)
        ProgTimeout = 16;
    else
        ProgTimeout = 8;
    Count = (DWord11 & BFP_DW11_PAGE_PROG_TIME_CNT_MASK)
                                                     >> BFP_DW11_PAGE_PROG_TIME_CNT_SHIFT;
    ProgTimeout = ProgTimeout * (Count + 1);
    return ProgTimeout;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_GetTypicalSectorEraseTimeUs -.-.-.-.-.-.-.-.-.-.-.-.
//
UInt  DtBcSPIMF_GetTypicalSectorEraseTimeUs(UInt8*  pBuffer, Int EraseType)
{
    UInt32  DWord10 = DtBcSPIMF_ReadDWord(pBuffer, 10);
    UInt Units=0, Count=0;

    if (DtBcSPIMF_GetSectorSize(pBuffer, EraseType) <= 0)
        return 0;

    switch (EraseType)
    {
    case 1:
        Units = ((DWord10 & BFP_DW10_ERASE_TYPE1_UNIT_MASK)
                                                      >> BFP_DW10_ERASE_TYPE1_UNIT_SHIFT);
        Count = ((DWord10 & BFP_DW10_ERASE_TYPE1_CNT_MASK)
                                                       >> BFP_DW10_ERASE_TYPE1_CNT_SHIFT);
        break;
    case 2:
        Units = ((DWord10 & BFP_DW10_ERASE_TYPE2_UNIT_MASK)
                                                      >> BFP_DW10_ERASE_TYPE2_UNIT_SHIFT);
        Count = ((DWord10 & BFP_DW10_ERASE_TYPE2_CNT_MASK)
                                                       >> BFP_DW10_ERASE_TYPE2_CNT_SHIFT);
        break;
    case 3:
        Units = ((DWord10 & BFP_DW10_ERASE_TYPE3_UNIT_MASK)
                                                      >> BFP_DW10_ERASE_TYPE3_UNIT_SHIFT);
        Count = ((DWord10 & BFP_DW10_ERASE_TYPE3_CNT_MASK)
                                                       >> BFP_DW10_ERASE_TYPE3_CNT_SHIFT);
        break;
    case 4:
        Units = ((DWord10 & BFP_DW10_ERASE_TYPE4_UNIT_MASK)
                                                      >> BFP_DW10_ERASE_TYPE4_UNIT_SHIFT);
        Count = ((DWord10 & BFP_DW10_ERASE_TYPE4_CNT_MASK)
                                                       >> BFP_DW10_ERASE_TYPE4_CNT_SHIFT);
        break;
    }
    return DtBcSPIMF_ComputeSectorEraseTimeUs(Units, Count);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_GetTypToMaxEraseTimeFactor -.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt  DtBcSPIMF_GetTypToMaxEraseTimeFactor(UInt8*  pBuffer)
{
    UInt32  DWord10 =  DtBcSPIMF_ReadDWord(pBuffer, 10);
    UInt  Factor;
    // Formula maximum time = 2 * (count + 1)*typical time 
    Factor = (DWord10 & BFP_DW10_ERASE_MAX_TIME_CNT_MASK) >> BFP_DW10_ERASE_MAX_TIME_CNT_SHIFT;
    Factor =  2 * (Factor + 1);
    return Factor;
}
// -.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_ComputeSectorEraseTimeUs -.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt  DtBcSPIMF_ComputeSectorEraseTimeUs(UInt32  Units, UInt32  Count)
{
    UInt  Timeout = 0;
    switch (Units)
    {
        default:
        case BFP_ERASE_TYPE4_UNIT_1MS:   Timeout =      1*1000; break;
        case BFP_ERASE_TYPE4_UNIT_16MS:  Timeout =     16*1000; break;
        case BFP_ERASE_TYPE4_UNIT_128MS: Timeout =    128*1000; break;
        case BFP_ERASE_TYPE4_UNIT_1SEC:  Timeout = 1*1000*1000; break;
    }
    // Formula: typical time = (count + 1)*units 
    Timeout = Timeout * (Count + 1);
    return Timeout;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_Exp2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt64  DtBcSPIMF_Exp2(Int  Value)
{
    DT_ASSERT(Value>=0 && Value<64);
    return 1LL<<Value;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_Log2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt  DtBcSPIMF_Log2(UInt64  Value)
{
    Int Result = 0;
    DT_ASSERT(Value > 0);
    while (Value > 1)
    {
        Value >>= 1;
        Result++;
    }
    DT_ASSERT(Value == 1);
    return Result;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_ReadDWord -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt32  DtBcSPIMF_ReadDWord(const UInt8 *pBuf,  Int  DWordNum)
{
    DT_ASSERT(DWordNum >= 1);
    return ((UInt32*)pBuf)[DWordNum-1];
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_TimeoutUsToLog2Ms -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt  DtBcSPIMF_TimeoutUsToLog2Ms(UInt  TimeoutUs)
{
    // Convert to milliseconds
    Int TimeoutMs = (TimeoutUs + 999)/1000;
    
    // Find the smallest N  for which counts: 2^N >= TimeoutMs;
    Int Value = TimeoutMs;
    Int Result = 0;
    while (Value > 1)
    {
        Value >>= 1;
        Result++;
    }
    if ((1U<<Result) < TimeoutUs)
        Result++;
    return Result;   
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_SetFlashProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void   DtBcSPIMF_SetFlashProperties(DtBcSPIMF* pBc, UInt32* FlashProps)
{
    UInt i,  Prop, PropVal, RegData;
    for (i = 0; i<DT_BC_SPIMF_PROP_COUNT; i++)
    {
        switch (i)
        {
        case DT_BC_SPIMF_PROP_INSTR_READ_SFDP:
            Prop = SPIMF_PROP_INSTR_READ_SFDP;
            break;
        case DT_BC_SPIMF_PROP_INSTR_BULK_ERASE:
            Prop = SPIMF_PROP_INSTR_BULK_ERASE;
            break;
        case DT_BC_SPIMF_PROP_INSTR_PAGE_PROGRAM:
            Prop = SPIMF_PROP_INSTR_PAGE_PROGRAM;
            break;
        case DT_BC_SPIMF_PROP_INSTR_READ:
            Prop = SPIMF_PROP_INSTR_READ;
            break;
        case DT_BC_SPIMF_PROP_INSTR_READ_STATUS:
            Prop = SPIMF_PROP_INSTR_READ_STATUS;
            break;
        case DT_BC_SPIMF_PROP_INSTR_RESET:
            Prop = SPIMF_PROP_INSTR_RESET;
            break;
        case DT_BC_SPIMF_PROP_INSTR_RESET_ENABLE:
            Prop = SPIMF_PROP_INSTR_RESET_ENABLE;
            break;
        case DT_BC_SPIMF_PROP_INSTR_SECTOR_ERASE:
            Prop = SPIMF_PROP_INSTR_SECTOR_ERASE;
            break;
        case DT_BC_SPIMF_PROP_INSTR_WRITE_ENABLE:
            Prop = SPIMF_PROP_INSTR_WRITE_ENABLE;
            break;
        case DT_BC_SPIMF_PROP_SIZE_PAGE_LOG2BYTES:
            Prop = SPIMF_PROP_SIZE_PAGE_LOG2BYTES;
            break;
        case DT_BC_SPIMF_PROP_SIZE_SECTOR_LOG2BYTES:
            Prop = SPIMF_PROP_SIZE_SECTOR_LOG2BYTES;
            break;
        case DT_BC_SPIMF_PROP_SIZE_MEM_LOG2BYTES:
            Prop = SPIMF_PROP_SIZE_MEM_LOG2BYTES;
            break;
        case DT_BC_SPIMF_PROP_SIZE_ADDR_BYTES:
            Prop = SPIMF_PROP_SIZE_ADDR_BYTES;
            break;
        default: DT_ASSERT(FALSE); continue;
        }
        PropVal = FlashProps[i];
        RegData =  SPIMF_FlashProperties_SET_Property(0, Prop);
        RegData =  SPIMF_FlashProperties_SET_Value(RegData, PropVal);
        SPIMF_FlashProperties_WRITE(pBc, RegData);
    }
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSPIMF implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcSPIMF
#define SPIMF_STUB   ((DtIoStubBcSPIMF*)pStub)
#define SPIMF_BC  ((DtBcSPIMF*)SPIMF_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcSPIMF_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
static DtStatus  DtIoStubBcSPIMF_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int* pOutSize);
static DtStatus  DtIoStubBcSPIMF_OnCmdErase(const DtIoStubBcSPIMF*, 
                                                        const DtIoctlSpiMfCmdEraseInput*);
static DtStatus  DtIoStubBcSPIMF_OnCmdGetProperties(const DtIoStubBcSPIMF*, 
                                                     DtIoctlSpiMfCmdGetPropertiesOutput*);
static DtStatus  DtIoStubBcSPIMF_OnCmdLockDevice(const DtIoStubBcSPIMF*, Int  Cmd);
static DtStatus  DtIoStubBcSPIMF_OnCmdRead(const DtIoStubBcSPIMF*, 
                                                          const DtIoctlSpiMfCmdReadInput*,
                                                          DtIoctlSpiMfCmdReadOutput*);
static DtStatus  DtIoStubBcSPIMF_OnCmdWrite(const DtIoStubBcSPIMF*, 
                                                        const DtIoctlSpiMfCmdWriteInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_SPIMF;

static const DtIoctlProperties  IOSTUB_BC_SPIMF_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_SPIMF_CMD(
        DtIoStubBcSPIMF_OnCmd,
        DtIoStubBcSPIMF_AppendDynamicSize,
        NULL),
};


//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSPIMF - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIMF_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcSPIMF_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSPIMF));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIMF_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcSPIMF*  DtIoStubBcSPIMF_Open(DtBc*  pBc)
{
    DtIoStubBcSPIMF*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    BC_SPIMF_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcSPIMF, pBc, NULL, 
                                                              DtIoStubBcSPIMF_Close,
                                                              NULL,  // Use default IOCTL
                                                              IOSTUB_BC_SPIMF_IOCTLS);
    pStub = (DtIoStubBcSPIMF*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSPIMF - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIMF_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSPIMF_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSpiMfCmdInput*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSPIMF));
    DT_ASSERT(pIoParams != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_SPIMF_CMD);

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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIMF_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSPIMF_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSpiMfCmdInput*  pInData = NULL;
    DtIoctlSpiMfCmdOutput*  pOutData = NULL;
    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSPIMF));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_SPIMF_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, SPIMF, pStub, "ERROR: block is not locked by me");
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
        Status = DtIoStubBcSPIMF_OnCmdErase(SPIMF_STUB,  &pInData->m_Erase);
        break;
    case DT_SPIMF_CMD_GET_PROPERTIES:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSPIMF_OnCmdGetProperties(SPIMF_STUB, 
                                                              &pOutData->m_GetProperties);
        break;
    case DT_SPIMF_CMD_LOCK_DEVICE:
        Status = DtIoStubBcSPIMF_OnCmdLockDevice(SPIMF_STUB, pInData->m_Lock.m_CmdEx);
        break;
    case DT_SPIMF_CMD_READ:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSPIMF_OnCmdRead(SPIMF_STUB, 
                                                     &pInData->m_Read, &pOutData->m_Read);
        break;
    case DT_SPIMF_CMD_WRITE:
        Status = DtIoStubBcSPIMF_OnCmdWrite(SPIMF_STUB, &pInData->m_Write);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIMF_OnCmdErase -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSPIMF_OnCmdErase(
    const DtIoStubBcSPIMF*  pStub, 
    const DtIoctlSpiMfCmdEraseInput*  pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSPIMF));
    DT_ASSERT(pInData != NULL);

    return DtBcSPIMF_Erase(SPIMF_BC, pInData->m_StartAddress, pInData->m_NumBytes);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIMF_OnCmdGetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSPIMF_OnCmdGetProperties(
    const DtIoStubBcSPIMF*  pStub, 
    DtIoctlSpiMfCmdGetPropertiesOutput*  pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSPIMF));
    DT_ASSERT(pOutData != NULL);

    pOutData->m_MemoryId = SPIMF_BC->m_ConfMemoryId;
    pOutData->m_MemorySize = SPIMF_BC->m_ConfMemorySize;
    pOutData->m_PageSize = SPIMF_BC->m_ConfPageSize;
    pOutData->m_SectorSize = SPIMF_BC->m_ConfSectorSize;
    pOutData->m_SpiClockRate = SPIMF_BC->m_ConfClockRateHz;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIMF_OnCmdLockDevice -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSPIMF_OnCmdLockDevice(
    const DtIoStubBcSPIMF*  pStub,
    Int  Cmd)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSPIMF));
    DT_ASSERT(Cmd==DT_SPIMF_CMDEX_LOCK|| Cmd==DT_SPIMF_CMDEX_UNLOCK);

    return DtBcSPIMF_LockDevice(SPIMF_BC, (Cmd==DT_SPIMF_CMDEX_LOCK));
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIMF_OnCmdRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSPIMF_OnCmdRead(
    const DtIoStubBcSPIMF*  pStub, 
    const DtIoctlSpiMfCmdReadInput*  pInData,
    DtIoctlSpiMfCmdReadOutput*  pOutData)
{

    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSPIMF));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    // Assume the worst
    pOutData->m_NumBytes = 0;
    Status = DtBcSPIMF_Read(SPIMF_BC, pInData->m_StartAddress, pInData->m_NumToRead, 
                                                                         pOutData->m_Buf);
    // If succesful, set number of bytes read
    if (DT_SUCCESS(Status))
        pOutData->m_NumBytes = pInData->m_NumToRead;
    return  Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSPIMF_OnCmdWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSPIMF_OnCmdWrite(
    const DtIoStubBcSPIMF*  pStub, 
    const DtIoctlSpiMfCmdWriteInput*  pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSPIMF));
    DT_ASSERT(pInData != NULL);

    return DtBcSPIMF_Write(SPIMF_BC, pInData->m_StartAddress, pInData->m_NumToWrite, 
                                                                          pInData->m_Buf);
}
