//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSPIMF.c *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

// Helper macro to cast a DtBc* to a DtBcSPIMF*
#define BC_SPIMF         ((DtBcSPIMF*)pBc)

// SPI master flash operation states
#define SPIMF_STATE_IDLE      0
#define SPIMF_STATE_READ      1
#define SPIMF_STATE_WRITE     2
#define SPIMF_STATE_ERASE     3

// Unlock sequence: 0x1, 0x3, 0xD, 0x6, 0xC
static const UInt32  SPIMF_UNLOCK_SEQ[] = { 0x1,0x3, 0xD, 0x6, 0xC };
static const Int  SPIMF_UNLOCK_SEQ_SIZE = 
                                     sizeof(SPIMF_UNLOCK_SEQ)/sizeof(SPIMF_UNLOCK_SEQ[0]);

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

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSPIMF - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSPIMF_Close(DtBc*  pBc)
{
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIMF));
    
    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_Erase -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSPIMF_Erase(DtBcSPIMF*  pBc,  Int  StartAddress, Int  NumToErase)
{
    DtStatus  Status=DT_STATUS_OK;
    Int  EraseCmd = SPIMF_CMD_NOP;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIMF));

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
    UInt32  Reg_SPIMF_Status=0;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIMF));

    // Cache status register
    Reg_SPIMF_Status = SPIMF_Status_READ(pBc);
    
    // Check is-enabled flag
    IsEnabled = (SPIMF_Status_GET_IsEnabled(Reg_SPIMF_Status) != 0);
    if (!IsEnabled)
        return DT_STATUS_NO_POWER;

    // Check is-busy state
    IsBusy = (SPIMF_Status_GET_IsBusy(Reg_SPIMF_Status) != 0);
    if (IsBusy)
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: device is busy");
        DT_ASSERT(!IsBusy);
        return DT_STATUS_BUSY;
    }

    // Check device locked state
    IsLocked = (SPIMF_Status_GET_IsLocked(Reg_SPIMF_Status) != 0);
    if (IsLocked != Lock)
    {
        // Set command field to NOP, so that we donot accidentally issue a command below
        UInt32  Reg_SPIMF_Control = SPIMF_Control_SET_Command(0x0, SPIMF_CMD_NOP);

        // Not yet the correct state
        if (Lock)
        {
            // Writing a value not in the unlock sequence will lock the device
            Reg_SPIMF_Control = SPIMF_Control_SET_Unlock(Reg_SPIMF_Control, 0xF);
            SPIMF_Control_WRITE(pBc, Reg_SPIMF_Control);
        }
        else
        {
            // Write the unlock sequence: 0x1, 0x3, 0xD, 0x6, 0xC
            for (i=0; i<SPIMF_UNLOCK_SEQ_SIZE; i++)
            {
                Reg_SPIMF_Control = 
                         SPIMF_Control_SET_Unlock(Reg_SPIMF_Control, SPIMF_UNLOCK_SEQ[i]);
                SPIMF_Control_WRITE(pBc, Reg_SPIMF_Control);
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

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIMF));

    // Check for parameter errors.
    if (pBuf==NULL || ((UInt64)pBuf%4)!=0 
                   || (StartAddress+NumToRead)>pBc->m_ConfMemorySize
                   || (StartAddress%4)!=0 || (NumToRead%4)!=0)
    {
        // Invalid start address or #read 
        return DT_STATUS_INVALID_PARAMETER;
    }

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

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIMF));

    // Check for parameter errors.
    if (pBuf==NULL || ((UInt64)pBuf%4)!=0 
                   || (StartAddress+NumToWrite)>pBc->m_ConfMemorySize
                   || (StartAddress%pBc->m_ConfPageSize)!=0
                   || (NumToWrite%pBc->m_ConfPageSize)!=0)
    {
        // Invalid start address or #write
        return DT_STATUS_INVALID_PARAMETER;
    }

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
    UInt32  Reg_SPIMF_Status=0, Reg_SPIMF_Control=0;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIMF));

    // Cache status register
    Reg_SPIMF_Status = SPIMF_Status_READ(pBc);

    // Check the SpiMasterFlash is enabled
    IsEnabled = (SPIMF_Status_GET_IsEnabled(Reg_SPIMF_Status) != 0);
    if(!IsEnabled)
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: SpiMf is disabled");
        return DT_STATUS_FAIL;
    }

    // Check if the controller is already busy
    IsBusy = (SPIMF_Status_GET_IsBusy(Reg_SPIMF_Status) != 0);
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
    case SPIMF_CMD_SECTOR_ERASE:
        DT_ASSERT(pBuf == NULL);
        pBc->m_State = SPIMF_STATE_ERASE;
        MustBeUnlocked = TRUE;      // Device must be unlocked for an erase
        break;
    case SPIMF_CMD_READ:
        DT_ASSERT(pBuf != NULL);
        pBc->m_State = SPIMF_STATE_READ;
        MustBeUnlocked = FALSE;
        break;
    case SPIMF_CMD_PAGE_WRITE:
        DT_ASSERT(pBuf != NULL);
        pBc->m_State = SPIMF_STATE_WRITE;
        MustBeUnlocked = TRUE;      // Device must be unlocked for a write
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
        IsLocked = (SPIMF_Status_GET_IsLocked(Reg_SPIMF_Status)!=0);
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
    Reg_SPIMF_Control = SPIMF_Control_SET_Command(0, Cmd);
    SPIMF_Control_WRITE(pBc, Reg_SPIMF_Control);

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

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIMF));

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
DtStatus  DtBcSPIMF_Init(DtBc*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;
    UInt32  Reg_SPIMF=0, Reg_SPIMF_Control=0;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIMF));

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Base initialisation -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // Device should not be busy, but if busy try to reset the device 
    if (SPIMF_Status_READ_IsBusy(BC_SPIMF) != 0)
    {
        Reg_SPIMF_Control = SPIMF_Control_SET_Command(0, SPIMF_CMD_RESET);
        SPIMF_Control_WRITE(BC_SPIMF, Reg_SPIMF_Control);
    }

    // Lock the device
    Reg_SPIMF_Control = SPIMF_Control_SET_Command(0, SPIMF_CMD_NOP);
    Reg_SPIMF_Control = SPIMF_Control_SET_Unlock(Reg_SPIMF_Control, 0xF);
    SPIMF_Control_WRITE(BC_SPIMF, Reg_SPIMF_Control);


    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Read flash properties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    // Cache configuration 1 register
    Reg_SPIMF = SPIMF_Config1_READ(BC_SPIMF);
    // Configuration 1 - Memory-ID
    BC_SPIMF->m_ConfMemoryId = SPIMF_Config1_GET_MemoryId(Reg_SPIMF);
    // Configuration 1 - Bit order
    BC_SPIMF->m_ConfBitOrder = SPIMF_Config1_GET_BitOrder(Reg_SPIMF);

    // Configuration 2 - Clock-rate (read as units of Hz)
    BC_SPIMF->m_ConfClockRateHz = SPIMF_Config2_READ_SpiClockRate(BC_SPIMF);


    // Cache configuration 3 register
    Reg_SPIMF = SPIMF_Config3_READ(BC_SPIMF);
    // Configuration 3 - Page size (reads as log2(bytes))
    BC_SPIMF->m_ConfPageSize = SPIMF_Config3_GET_PageSize(Reg_SPIMF);
    DT_ASSERT(BC_SPIMF->m_ConfPageSize>=0 && BC_SPIMF->m_ConfPageSize<32);
    BC_SPIMF->m_ConfPageSize = 1 << BC_SPIMF->m_ConfPageSize;
    DT_ASSERT(BC_SPIMF->m_ConfPageSize > 0);
    // Configuration 3 - Sector size (reads as log2(bytes))
    BC_SPIMF->m_ConfSectorSize = SPIMF_Config3_GET_SectorSize(Reg_SPIMF);
    DT_ASSERT(BC_SPIMF->m_ConfSectorSize>=0 && BC_SPIMF->m_ConfSectorSize<32);
    if (BC_SPIMF->m_ConfSectorSize > 0)
        BC_SPIMF->m_ConfSectorSize = 1 << BC_SPIMF->m_ConfSectorSize;
    DT_ASSERT(BC_SPIMF->m_ConfSectorSize >= 0);
    // Configuration 3 - Memory size (reads as log2(bytes))
    BC_SPIMF->m_ConfMemorySize = SPIMF_Config3_GET_MemorySize(Reg_SPIMF);
    DT_ASSERT(BC_SPIMF->m_ConfMemorySize>=0 && BC_SPIMF->m_ConfMemorySize<32);
    BC_SPIMF->m_ConfMemorySize = 1 << BC_SPIMF->m_ConfMemorySize;
    DT_ASSERT(BC_SPIMF->m_ConfMemorySize > 0);

    // Cache configuration 4 register
    Reg_SPIMF = SPIMF_Config4_READ(BC_SPIMF);
    // Configuration 4 - Transmit-FIFO size
    BC_SPIMF->m_ConfTxFifoSize = SPIMF_Config4_GET_TxFifoSize(Reg_SPIMF);
    DT_ASSERT(BC_SPIMF->m_ConfTxFifoSize>0 && BC_SPIMF->m_ConfTxFifoSize<(1<<16));
    // Configuration 4 - Receive-FIFO size
    BC_SPIMF->m_ConfRxFifoSize = SPIMF_Config4_GET_RxFifoSize(Reg_SPIMF);
    DT_ASSERT(BC_SPIMF->m_ConfRxFifoSize>0 && BC_SPIMF->m_ConfRxFifoSize<(1<<16));
    
    // Print configuration
    DtDbgOutBc(AVG, SPIMF, pBc, "Configuration: clock=%dHz, mem_id=%d, order=%d, "
                                 "page=%d, sector=%d, mem_size=%d tx_fifo=%d, rx_fifo=%d",
                                 BC_SPIMF->m_ConfClockRateHz, BC_SPIMF->m_ConfMemoryId,
                                 BC_SPIMF->m_ConfBitOrder, BC_SPIMF->m_ConfPageSize,
                                 BC_SPIMF->m_ConfSectorSize, BC_SPIMF->m_ConfMemorySize,
                                 BC_SPIMF->m_ConfTxFifoSize, BC_SPIMF->m_ConfRxFifoSize);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Init other attributes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    
    // Init interrupt DPC
    Status = DtDpcInit(&BC_SPIMF->m_IntDpc, DtBcSPIMF_InterruptDpc, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: failed to init DPC (Status=0x%08X)", Status);
        return Status;
    }

    // Init state attributes
    BC_SPIMF->m_State = SPIMF_STATE_IDLE;
    BC_SPIMF->m_NumToDo = 0;
    BC_SPIMF->m_pData = NULL;
    BC_SPIMF->m_Result = DT_STATUS_OK;

    // Init completion event
    Status = DtEventInit(&BC_SPIMF->m_OpComplEvent, TRUE);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SPIMF, pBc, 
                        "ERROR: failed to init completion event (Status=0x%08X)", Status);
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Register interrupt handlers -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    
    Status = DtBcSPIMF_RegisterIntHandlers(BC_SPIMF);
    if(!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, SPIMF, pBc, "ERROR: failed to register interrupt handlers");
        return Status;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSPIMF_OnEnable(DtBc*  pBc, Bool  Enable)
{
    // NOTHING SPECIAL TODO FOR NOW
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSPIMF_InterruptDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSPIMF_InterruptDpc(DtDpcArgs* pArgs)
{
    DtBcSPIMF*  pBc = (DtBcSPIMF*)pArgs->m_pContext;
    UInt32  IntReason = pArgs->m_Data1.m_UInt32_1;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIMF));

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
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIMF));
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

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIMF));
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

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIMF));
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

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIMF));

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

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSPIMF));

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