//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* SpiMf.c *#*#*#*#*#*#*#*#*#* (C) 2015-2016 DekTec
//
// Dta driver -  SPIMF read/write operations
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2015-2016 DekTec Digital Video B.V.
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
#include <DtaIncludes.h>
#include <SpiMf.h>

// SPI master flash operation states
#define SPIMF_STATE_IDLE      0
#define SPIMF_STATE_READ      1
#define SPIMF_STATE_WRITE     2
#define SPIMF_STATE_ERASE     3

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
static void  SpiMfIntDpc(DtDpcArgs* pArgs);
DtStatus  SpiMfDeviceLock(DtaSpiMf* pSpiMf, DtFileObject*  pFile, Int Lock);
DtStatus  SpiMfErase(DtaSpiMf* pSpiMf, DtFileObject*  pFile, Int StartAddress,
                     Int SizeToErase);
DtStatus  SpiMfRead(DtaSpiMf* pSpiMf, DtFileObject* pFile, Int StartAddress,
                    Int NumToRead, UInt8* pBuf);
DtStatus  SpiMfWrite(DtaSpiMf* pSpiMf, DtFileObject* pFile, Int StartAddress,
                     Int NumToWrite, UInt8* pBuf);

// Operations handling the interrupt DPC
void SpiMfEraseIntDpc(DtaSpiMf* pSpiMf, UInt32 IntReason);
void SpiMfReadIntDpc(DtaSpiMf* pSpiMf, UInt32 IntReason);
void SpiMfWriteIntDpc(DtaSpiMf* pSpiMf, UInt32 IntReason);

// Operations for exclusive access
DtStatus  SpiMfExclAccessAcquire(DtaSpiMf* pSpiMf, DtaNonIpPort* pNonIpPort, 
                                                                     DtFileObject* pFile);
DtStatus  SpiMfExclAccessRelease(DtaSpiMf* pSpiMf, DtaNonIpPort* pNonIpPort, 
                                                                     DtFileObject* pFile);
DtStatus  SpiMfExclAccessProbe(DtaSpiMf* pSpiMf);
DtStatus  SpiMfCheckExclAccess(DtaSpiMf* pSpiMf, DtFileObject*  pFile);

// Local functions
DtStatus  SpiMfInitPowerup(DtaSpiMf* pSpiMf, volatile UInt8*  pFwbRegs);
DtStatus  SpiMfControllerInit(DtaSpiMf* pSpiMf);
DtStatus SpiMfDummyCallback(DtaNonIpPort*  pNonIpPort, DtFileObject*  pFile);
void   ControlRegWrite(volatile UInt8* pFwbRegs, const DtFwField*  pField, UInt32  Value);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSpiMfDvcInitPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Power-up initialisation at board level
//
DtStatus  DtaSpiMfDvcInitPowerup(DtaDeviceData* pDvcData)
{
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    Int  RegOffset;

    if (!pDvcData->m_SpiMf.m_IsSupported)
        return DT_STATUS_NOT_SUPPORTED;

    // Get register offset
    RegOffset = (Int)DtPropertiesGetUInt16(pPropData, "REGISTERS_SPIMF", -1);

    // Used for event logging
    pDvcData->m_SpiMf.m_pDvcData = pDvcData;

    return SpiMfInitPowerup(&pDvcData->m_SpiMf, pDvcData->m_pGenRegs+RegOffset);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSpiMfDvcInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Initialisation at board level
//
DtStatus  DtaSpiMfDvcInit(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    Int ProgItf;

    // Assume SPI master flash is not supported
    pDvcData->m_SpiMf.m_IsSupported = FALSE;

    // Get properties
    ProgItf = DtPropertiesGetInt(pPropData, "PROGRAMMING_ITF", -1);
    if (ProgItf == PROG_ITF_SPI_MF)
    {
        // Initialize SpiMf structure
        Status = DtaSpiMfInit(&pDvcData->m_SpiMf, SpiMfDummyCallback,
                                                  SpiMfDummyCallback, &FwbSpiMasterFlash);
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSpiMfInitPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Power-up initialisation at port level
//
DtStatus  DtaSpiMfInitPowerup(DtaNonIpPort*  pNonIpPort)
{
    // Used for event logging
    pNonIpPort->m_SpiMf.m_pDvcData = pNonIpPort->m_pDvcData;

    return SpiMfInitPowerup(&pNonIpPort->m_SpiMf, pNonIpPort->m_pFwbRegs);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSpiMfInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Initialisation common for board and port
//
DtStatus  DtaSpiMfInit(DtaSpiMf* pSpiMf, pSpiMfCallback pSpiMfPrepare, 
                       pSpiMfCallback pSpiMfRelease, const DtFwbSpiMasterFlash* pFwbSpiMf)
{
    // Store the SPI-master flash firmware block description
    // Becomes relevant when it is also used in composite firmware blocks
    pSpiMf->m_pFwbSpiMf = pFwbSpiMf;
    
    DtDpcInit(&pSpiMf->m_IntDpc, SpiMfIntDpc, TRUE);

    // Initialize common data first
    pSpiMf->m_ExclAccess = FALSE;
    pSpiMf->m_State = SPIMF_STATE_IDLE;
    pSpiMf->m_pDvcData = NULL;
    pSpiMf->m_pSpiMfPrepare = pSpiMfPrepare;
    pSpiMf->m_pSpiMfRelease = pSpiMfRelease;
    pSpiMf->m_IsInitialized = FALSE;
    pSpiMf->m_IsSupported = TRUE;
    DtMutexInit(&pSpiMf->m_ExclAccessLock);

    // Init event
    DtEventInit(&pSpiMf->m_OpComplEvent, TRUE);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSpiMfIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaSpiMfIoctl(
    DtaDeviceData* pDvcData,
    DtFileObject* pFile,
    DtIoctlObject* pIoctl,
    Bool  PowerDownPending)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    Int  NonIpPortIndex;        // Index in the nonip port struct
    DtaSpiMf* pSpiMf;           // 
    DtaNonIpPort*  pNonIpPort = NULL;
    DtaIoctlSpiMfCmdInput*  pSpiMfCmdInput =
                                           (DtaIoctlSpiMfCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlSpiMfCmdOutput*  pSpiMfCmdOutput =
                                         (DtaIoctlSpiMfCmdOutput*)pIoctl->m_pOutputBuffer;

    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlSpiMfCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlSpiMfCmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    pNonIpPort = NULL;  // Assume a board level request
    if (pSpiMfCmdInput->m_PortIndex == -1)
        NonIpPortIndex = -1;    // Is a board level request
    else if (DT_SUCCESS(DtaGetNonIpPortIndex(pDvcData, pSpiMfCmdInput->m_PortIndex,
                                                                        &NonIpPortIndex)))
        pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpPortIndex];
    else
        return DT_STATUS_INVALID_PARAMETER;

    // Get pointer to SpiMf structure
    if (NonIpPortIndex == -1)
        pSpiMf = &pDvcData->m_SpiMf;
    else
        pSpiMf = &pNonIpPort->m_SpiMf;

    // Check supported
    if (!pSpiMf->m_IsSupported)
    {
        DtDbgOut(ERR, SPIMF, "Device is not supported");
        return DT_STATUS_NOT_SUPPORTED;
    }
    // Check initialized
    if (!pSpiMf->m_IsInitialized)
    {
        DtDbgOut(ERR, SPIMF, "Device is not initialized");
        return DT_STATUS_NOT_INITIALISED;
    }
    // Determine final required output/input sizes
    switch (pSpiMfCmdInput->m_Cmd)
    {

    case DTA_SPIMF_CMD_EXCLUSIVE_ACCESS:
        pCmdStr = "DTA_SPMIF_CMD_EXCLUSIVE_ACCESS";
        InReqSize += sizeof(DtaIoctlNonIpCmdExclusiveAccessInput);
        OutReqSize = 0;
        break;

    case DTA_SPIMF_CMD_DEVICE_LOCK:
        pCmdStr = "DTA_SPIMF_CMD_DEVICE_LOCK";
        InReqSize += sizeof(DtaIoctlSpiMfCmdDeviceLockInput);
        OutReqSize = 0;
        break;

    case DTA_SPIMF_CMD_GET_PROPERTIES:
        pCmdStr = "DTA_SPIMF_CMD_STATUS_GET";
        // We expect no additional data in the input buffer
        OutReqSize += sizeof(DtaIoctlSpiMfCmdGetPropertiesOutput);
        break;

    case DTA_SPIMF_CMD_ERASE:
        pCmdStr = "DTA_SPIMF_CMD_ERASE";
        InReqSize += sizeof(DtaIoctlSpiMfCmdEraseInput);
        OutReqSize = 0;
        break;

    case DTA_SPIMF_CMD_READ:
        pCmdStr = "DTA_SPIMF_CMD_READ";
        InReqSize += sizeof(DtaIoctlSpiMfCmdReadInput);
        OutReqSize += OFFSETOF(DtaIoctlSpiMfCmdReadOutput, m_Buf); // Exclude m_Buf
        // Check for valid input buffer before reading it
        if (pIoctl->m_InputBufferSize >= InReqSize)
            OutReqSize += pSpiMfCmdInput->m_Data.m_Read.m_NumBytesToRead;
        break;

    case DTA_SPIMF_CMD_WRITE:
        pCmdStr = "DTA_SPIMF_CMD_WRITE";
        InReqSize += OFFSETOF(DtaIoctlSpiMfCmdWriteInput, m_Buf); // Exclude m_Buf
        // Check for valid input buffer before reading it
        if (pIoctl->m_InputBufferSize >= InReqSize)
            InReqSize += pSpiMfCmdInput->m_Data.m_Write.m_NumBytesToWrite;
        OutReqSize = 0;
        break;

    default:
        pCmdStr = "??UNKNOWN SPIMF_CMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (PowerDownPending)
    {
        // Only skip flash device specific IOCTL's
        switch (pSpiMfCmdInput->m_Cmd)
        {
        case DTA_SPIMF_CMD_ERASE:
        case DTA_SPIMF_CMD_READ:
        case DTA_SPIMF_CMD_WRITE:
            DtDbgOut(ERR, DTA, "%s: SpiMf cmd because powerdown  occured!", pCmdStr);
            return DT_STATUS_POWERDOWN;
        }
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, SPIMF, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr,
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, SPIMF, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr,
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, SPIMF, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
            pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    }

    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        Int Cmd;
        switch (pSpiMfCmdInput->m_Cmd)
        {
        case DTA_SPIMF_CMD_EXCLUSIVE_ACCESS:
            Cmd = pSpiMfCmdInput->m_Data.m_ExclusiveAccess.m_Cmd;
            if (Cmd == DTA_EXCLUSIVE_ACCESS_CMD_ACQUIRE)
                Status = SpiMfExclAccessAcquire(pSpiMf, pNonIpPort,  pFile);
            else if (Cmd == DTA_EXCLUSIVE_ACCESS_CMD_RELEASE)
                Status = SpiMfExclAccessRelease(pSpiMf, pNonIpPort, pFile);
            else if (Cmd == DTA_EXCLUSIVE_ACCESS_CMD_PROBE)
                Status = SpiMfExclAccessProbe(pSpiMf);
            else if (Cmd == DTA_EXCLUSIVE_ACCESS_CMD_CHECK)
                Status = SpiMfCheckExclAccess(pSpiMf, pFile);
            else
                Status = DT_STATUS_NOT_SUPPORTED;
            break;

        case DTA_SPIMF_CMD_DEVICE_LOCK:
            Status = SpiMfDeviceLock(pSpiMf, pFile, pSpiMfCmdInput->m_Data.m_Lock.m_Lock);
            break;

        case DTA_SPIMF_CMD_ERASE:
            Status = SpiMfErase(pSpiMf, pFile,
                                            pSpiMfCmdInput->m_Data.m_Erase.m_StartAddress,
                                            pSpiMfCmdInput->m_Data.m_Erase.m_NumBytes);
            break;

        case DTA_SPIMF_CMD_GET_PROPERTIES:
            pSpiMfCmdOutput->m_Data.m_GetProperties.m_MemoryId = pSpiMf->m_MemoryId;
            pSpiMfCmdOutput->m_Data.m_GetProperties.m_MemorySize = pSpiMf->m_MemorySize;
            pSpiMfCmdOutput->m_Data.m_GetProperties.m_PageSize = pSpiMf->m_PageSize;
            pSpiMfCmdOutput->m_Data.m_GetProperties.m_SectorSize = pSpiMf->m_SectorSize;
            pSpiMfCmdOutput->m_Data.m_GetProperties.m_SpiClockRate=pSpiMf->m_SpiClockRate;
            Status = DT_STATUS_OK;
            break;

        case DTA_SPIMF_CMD_READ:
            Status = SpiMfRead(pSpiMf, pFile,
                                           pSpiMfCmdInput->m_Data.m_Read.m_StartAddress,
                                           pSpiMfCmdInput->m_Data.m_Read.m_NumBytesToRead,
                                           pSpiMfCmdOutput->m_Data.m_Read.m_Buf);
           if (DT_SUCCESS(Status))
                pSpiMfCmdOutput->m_Data.m_Read.m_NumBytes =
                                           pSpiMfCmdInput->m_Data.m_Read.m_NumBytesToRead;
            else
                pSpiMfCmdOutput->m_Data.m_Read.m_NumBytes = 0;
            break;

        case DTA_SPIMF_CMD_WRITE:
            Status = SpiMfWrite(pSpiMf, pFile,
                                         pSpiMfCmdInput->m_Data.m_Write.m_StartAddress,
                                         pSpiMfCmdInput->m_Data.m_Write.m_NumBytesToWrite,
                                         pSpiMfCmdInput->m_Data.m_Write.m_Buf);
            break;
        }
    }

    // If we failed, no data has to be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED)
            DtDbgOut(MIN, SPIMF, "SpiMfCmd=0x%x: NOT SUPPORTED", pSpiMfCmdInput->m_Cmd);
        else if (Status == DT_STATUS_IO_PENDING)
            DtDbgOut(MAX, SPIMF, "%s: ERROR %xh", pCmdStr, Status); // NOT A REAL ERROR
        else
            DtDbgOut(MIN, SPIMF, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSpiMfClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaSpiMfClose(DtaSpiMf* pSpiMf, DtaNonIpPort* pNonIpPort, DtFileObject*  pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    if (pSpiMf->m_IsSupported && pSpiMf->m_IsInitialized)
        Status = SpiMfExclAccessRelease(pSpiMf, pNonIpPort, pFile);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSpiMfInterrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaSpiMfInterrupt(DtaSpiMf*  pSpiMf)
{
    volatile UInt8* pFwbRegs = pSpiMf->m_pFwbRegs;
    const DtFwbSpiMasterFlash* pFwbSpiMf = pSpiMf->m_pFwbSpiMf;
    UInt32 IntStatus;

    // Check initialized
    if (!pSpiMf->m_IsInitialized)
        return FALSE;

    // Check for interrupts
    IntStatus = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_All)
                                                              & DTFWB_SPIMF_STAT_INT_MASK;
    if (IntStatus != 0)
    {
        DtDpcArgs  DpcArgs;
        UInt32 EnabledInts;

        // Disable fired interupts interrupts
        EnabledInts = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Control_AllInts);
        EnabledInts = (EnabledInts & (~IntStatus)) & DTFWB_SPIMF_STAT_INT_MASK;
        ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_AllInts, EnabledInts);

        // Clear interrupts
        DtaFwbRegWrite(pFwbRegs, &pFwbSpiMf->Status_All, IntStatus);

        // Schedule DPC to handle the interrupt.
        DpcArgs.m_pContext = pSpiMf;
        DpcArgs.m_Data1.m_UInt32_1 = IntStatus; // Interrupt reason
        DpcArgs.m_Data1.m_UInt32_2 = 0;
        DtDpcSchedule(&pSpiMf->m_IntDpc, &DpcArgs);

        // Interrupt was ours
        return TRUE;
    }
    return FALSE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SpiMfInitPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  SpiMfInitPowerup(DtaSpiMf* pSpiMf, volatile UInt8*  pFwbRegs)
{
    DtStatus  Result = DT_STATUS_OK;
    const DtFwbSpiMasterFlash* pFwbSpiMf = pSpiMf->m_pFwbSpiMf;

    // Not yet initialized
    pSpiMf->m_IsInitialized = FALSE;

        // Store firmware register address
    pSpiMf->m_pFwbRegs = pFwbRegs;

    // Store SPI-master flash properties
    pSpiMf->m_SpiClockRate = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Config1_SpiClockRate);
    pSpiMf->m_MemoryId =  DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Config1_MemoryId);                
    pSpiMf->m_PageSize = (1<<DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Config2_PageSize));        
    pSpiMf->m_MemorySize = (1<< DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Config2_MemorySize));
    pSpiMf->m_SectorSize =  (1<<DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Config2_SectorSize));
    // In case no sectors are supported, sector size is equal to the memory size
    if (pSpiMf->m_SectorSize == 1)
        pSpiMf->m_SectorSize = pSpiMf->m_MemorySize;
    pSpiMf->m_TxFifoSize = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Config3_TxFifoSize); 
    pSpiMf->m_RxFifoSize = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Config3_RxFifoSize); 

    // Initialize the SPI master flash controller
    Result =  SpiMfControllerInit(pSpiMf);
    
    pSpiMf->m_IsInitialized = (Result == DT_STATUS_OK);

    return Result;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SpiMfControllerInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  SpiMfControllerInit(DtaSpiMf* pSpiMf)
{
    volatile UInt8* pFwbRegs = pSpiMf->m_pFwbRegs;
    const DtFwbSpiMasterFlash* pFwbSpiMf = pSpiMf->m_pFwbSpiMf;
    UInt32 IsBusy;

    // Disable all interrupts
    ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_AllInts, 0);
    // Clear status flags
    DtaFwbRegWrite(pFwbRegs, &pFwbSpiMf->Status_All, ~0);

    // Device should not be busy, but if busy try to reset the device 
    IsBusy = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_IsBusy);
    if (IsBusy)
    {
        // Issue the reset command
        ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_Command, DTFWB_SPIMF_CMD_RESET);
        if (pSpiMf->m_pDvcData != NULL)
            DtEvtLogReport(&pSpiMf->m_pDvcData->m_Device.m_EvtObject,
                                        DTA_LOG_UNEXPECTED_SPIMF_STATE, NULL, NULL, NULL);
    }

    // Lock the device
    ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_Unlock, 0x1);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SpiMfDummyCallback -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus SpiMfDummyCallback(DtaNonIpPort* pNonIpPort, DtFileObject*  pFile)
{
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SpiMfIntDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  SpiMfIntDpc(DtDpcArgs* pArgs)
{
    DtaSpiMf*  pSpiMf = (DtaSpiMf*)pArgs->m_pContext;
    UInt32 IntReason = pArgs->m_Data1.m_UInt32_1;

    switch (pSpiMf->m_State)
    {
    case SPIMF_STATE_ERASE:  SpiMfEraseIntDpc(pSpiMf, IntReason); break;
    case SPIMF_STATE_READ:   SpiMfReadIntDpc(pSpiMf, IntReason);  break;
    case SPIMF_STATE_WRITE:  SpiMfWriteIntDpc(pSpiMf, IntReason); break;
    case SPIMF_STATE_IDLE:  /* No action*/;                      break;
    default:
        DT_ASSERT(FALSE);   // Unreachable code
        break;
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SpiMfErase -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  SpiMfErase(DtaSpiMf* pSpiMf, DtFileObject*  pFile,  Int StartAddress, 
                                                                          Int SizeToErase)
{
    DtStatus  Result = DT_STATUS_OK;
    volatile UInt8* pFwbRegs = pSpiMf->m_pFwbRegs;
    const DtFwbSpiMasterFlash* pFwbSpiMf = pSpiMf->m_pFwbSpiMf;
    UInt32 IsBusy, IsLocked, IsEnabled;
    UInt32  EraseCmd;
 
    // Check initialized
    DT_ASSERT(pSpiMf->m_IsInitialized);

    // Check access lock
    Result = SpiMfCheckExclAccess(pSpiMf, pFile);
    if (Result != DT_STATUS_OK)
    {
        DtDbgOut(ERR, SPIMF, "Device is not locked by me");
        return Result;
    }

    // Check is-enabled flag
    IsEnabled = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_IsEnabled);
    if (!IsEnabled)
        return DT_STATUS_NO_POWER;

    // Check is-busy state
    IsBusy = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_IsBusy);
    if (IsBusy)
    {
        DtDbgOut(ERR, SPIMF, "Device is busy");
        DT_ASSERT(!IsBusy);
        return DT_STATUS_BUSY;
    }

    // Check device locked state
    IsLocked = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_IsLocked);
    if (IsLocked)
    {
        DtDbgOut(ERR, SPIMF, "Device is not unlocked");
        return DT_STATUS_LOCKED;
    }

    // First check special cases
    if (StartAddress==0 && SizeToErase==pSpiMf->m_MemorySize)
    {
        // Complete device erase
        EraseCmd = DTFWB_SPIMF_CMD_BULK_ERASE;
    }
    // Check for errors.
    // If the device has no sectors only complete device erase is supported.  
    // Addresses must be sector alligned
    else if (   (StartAddress+SizeToErase)>pSpiMf->m_MemorySize
             || (StartAddress%pSpiMf->m_SectorSize)!=0
             || (SizeToErase%pSpiMf->m_SectorSize)!=0)
    {
        // Invalid parameters
        return DT_STATUS_INVALID_PARAMETER;
    }
    else
    {
        // Sector erase
        EraseCmd = DTFWB_SPIMF_CMD_SECT_ERASE;
    }

    // Clear a possible time-out status
    DtaFwbRegClear(pFwbRegs, &pFwbSpiMf->Status_Timeout);

    // Initialize erase administration
    DT_ASSERT(pSpiMf->m_State == SPIMF_STATE_IDLE);
    pSpiMf->m_State = SPIMF_STATE_ERASE;
    pSpiMf->m_pData = NULL;
    pSpiMf->m_NumToDo = 0;
    pSpiMf->m_Result = DT_STATUS_OK;

    // Set start address
    DtaFwbRegWrite(pFwbRegs, &pFwbSpiMf->StartAddress, StartAddress);

    // Set size to erase
    DtaFwbRegWrite(pFwbRegs, &pFwbSpiMf->TransferSize, SizeToErase);

    // Enable the done interrupt
    DtEventReset(&pSpiMf->m_OpComplEvent);
    ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_DoneIntEnable, 1);

    // Issue the erase command
    ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_Command, EraseCmd);

    // Wait for operation completion
    DtEventWaitUnInt(&pSpiMf->m_OpComplEvent, -1);

    return pSpiMf->m_Result;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SpiMfDeviceLock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  SpiMfDeviceLock(DtaSpiMf* pSpiMf, DtFileObject*  pFile, Int Lock)
{
    DtStatus  Result = DT_STATUS_OK;
    volatile UInt8* pFwbRegs = pSpiMf->m_pFwbRegs;
    const DtFwbSpiMasterFlash* pFwbSpiMf = pSpiMf->m_pFwbSpiMf;
    UInt32 IsBusy, IsLocked, IsEnabled;

    // Check initialized
    DT_ASSERT(pSpiMf->m_IsInitialized);

    // Check access lock
    Result = SpiMfCheckExclAccess(pSpiMf, pFile);
    if (Result != DT_STATUS_OK)
    {
        DtDbgOut(ERR, SPIMF, "Device is not locked by me");
        return Result;
    }
    
    // Check is-enabled flag
    IsEnabled = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_IsEnabled);
    if (!IsEnabled)
        return DT_STATUS_NO_POWER;

    // Check is-busy state
    IsBusy = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_IsBusy);
    if (IsBusy)
    {
        DtDbgOut(ERR, SPIMF, "Device is busy");
        DT_ASSERT(!IsBusy);
        return DT_STATUS_BUSY;
    }

    // Check device locked state
    IsLocked = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_IsLocked);
    if ((IsLocked!=0) != (Lock!=0))
    {
        // Not yet the correct state
        if (Lock)
        {
            // Lock the device
            ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_Unlock, 0x1);
        }
        else
        {
            // Write the unlock sequence: 1, 3, D, 6, C 
            ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_Unlock, 0x1);
            ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_Unlock, 0x3);
            ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_Unlock, 0xD);
            ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_Unlock, 0x6);
            ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_Unlock, 0xC);
        }
        // Check result
        IsLocked = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_IsLocked);
        if ((IsLocked!=0) != (Lock!=0))
        {
            DtDbgOut(ERR, SPIMF, "Devicelock failed");
            Result = DT_STATUS_FAIL;
        }
    }
    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SpiMfWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  SpiMfRead(DtaSpiMf* pSpiMf, DtFileObject*  pFile, Int StartAddress,
                                                               Int NumToRead, UInt8* pBuf)
{
    DtStatus  Result = DT_STATUS_OK;
    volatile UInt8* pFwbRegs = pSpiMf->m_pFwbRegs;
    const DtFwbSpiMasterFlash* pFwbSpiMf = pSpiMf->m_pFwbSpiMf;
    UInt32 IsBusy, IsEnabled;

    // Check initialized
    DT_ASSERT(pSpiMf->m_IsInitialized);
    
    // Check access lock
    Result = SpiMfCheckExclAccess(pSpiMf, pFile);
    if (Result != DT_STATUS_OK)
    {
        DtDbgOut(ERR, SPIMF, "Device is not locked by me");
        return Result;
    }
    
    // Check is-enabled flag
    IsEnabled = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_IsEnabled);
    if (!IsEnabled)
        return DT_STATUS_NO_POWER;

    // Check is-busy state
    IsBusy = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_IsBusy);
    if (IsBusy)
    {
        DtDbgOut(ERR, SPIMF, "Device is busy");
        DT_ASSERT(!IsBusy);
        return DT_STATUS_BUSY;
    }

    // Check for parameter errors.
    if (pBuf==NULL || ((UInt64)pBuf%4)!=0 
                   || (StartAddress+NumToRead)>pSpiMf->m_MemorySize
                   || (StartAddress%4)!=0 || (NumToRead%4)!=0)
    {
        // Invalid start address or read 
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Initialize read administration
    DT_ASSERT(pSpiMf->m_State == SPIMF_STATE_IDLE);
    pSpiMf->m_State = SPIMF_STATE_READ;
    pSpiMf->m_pData = (UInt32*)pBuf;
    pSpiMf->m_NumToDo = NumToRead;
    pSpiMf->m_Result = DT_STATUS_OK;

    // Set start address
    DtaFwbRegWrite(pFwbRegs, &pFwbSpiMf->StartAddress, StartAddress);

    // Set size to erase
    DtaFwbRegWrite(pFwbRegs, &pFwbSpiMf->TransferSize, NumToRead);

    // Enable the done interrupt
    DtEventReset(&pSpiMf->m_OpComplEvent);
    ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_DoneIntEnable, 1);

    // Enable the half full interrupt if more than FIFO-size has to be read
    if (pSpiMf->m_NumToDo >  pSpiMf->m_RxFifoSize)
        ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_RxHalfFullIntEnable, 1);

    // Issue the read command
    ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_Command, DTFWB_SPIMF_CMD_READ);

    // Wait for completion of the operation
    Result = DtEventWaitUnInt(&pSpiMf->m_OpComplEvent, -1);

    return pSpiMf->m_Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SpiMfWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  SpiMfWrite(DtaSpiMf* pSpiMf, DtFileObject*  pFile, Int StartAddress,
                                                              Int NumToWrite, UInt8* pBuf)
{
    DtStatus  Result = DT_STATUS_OK;
    volatile UInt8* pFwbRegs = pSpiMf->m_pFwbRegs;
    const DtFwbSpiMasterFlash* pFwbSpiMf = pSpiMf->m_pFwbSpiMf;
    UInt32 IsBusy, IsLocked, IsEnabled;
    Int  i, CanWrite;


    // Check initialized
    DT_ASSERT(pSpiMf->m_IsInitialized);

    // Check access lock
    Result = SpiMfCheckExclAccess(pSpiMf, pFile);
    if (Result != DT_STATUS_OK)
    {
        DtDbgOut(ERR, SPIMF, "Device is not locked by me");
        return Result;
    }
    
    // Check is-enabled flag
    IsEnabled = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_IsEnabled);
    if (!IsEnabled)
        return DT_STATUS_NO_POWER;

    // Check is-busy state
    IsBusy = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_IsBusy);
    if (IsBusy)
    {
        DtDbgOut(ERR, SPIMF, "Device is busy");
        DT_ASSERT(!IsBusy);
        return DT_STATUS_BUSY;
    }

    // Check device locked state
    IsLocked = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_IsLocked);
    if (IsLocked)
    {
        DtDbgOut(ERR, SPIMF, "Device is not unlocked");
        return DT_STATUS_LOCKED;
    }
    
    // Check for parameter errors. Addresses must be page alligned
    if (pBuf==NULL || ((UInt64)pBuf%4)!=0
                   || (StartAddress+NumToWrite)>pSpiMf->m_MemorySize
                   || (StartAddress%pSpiMf->m_PageSize)!=0
                   || (NumToWrite%pSpiMf->m_PageSize)!=0)
    {
        // Invalid start address or write 
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Initialize write administration
    DT_ASSERT(pSpiMf->m_State == SPIMF_STATE_IDLE);
    pSpiMf->m_State = SPIMF_STATE_WRITE;
    pSpiMf->m_pData = (UInt32*)pBuf;
    pSpiMf->m_NumToDo = NumToWrite;
    pSpiMf->m_Result = DT_STATUS_OK;

    // Clear a possible time-out status
    DtaFwbRegClear(pFwbRegs, &pFwbSpiMf->Status_Timeout);

    // Set start address
    DtaFwbRegWrite(pFwbRegs, &pFwbSpiMf->StartAddress, StartAddress);

    // Set size to write
    DtaFwbRegWrite(pFwbRegs, &pFwbSpiMf->TransferSize, NumToWrite);

    // Enable the done interrupt
    DtEventReset(&pSpiMf->m_OpComplEvent);
    ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_DoneIntEnable, 1);

    // Issue the write command
    ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_Command, DTFWB_SPIMF_CMD_PAGE_WRITE);

    // Compute the number of words that can be written in the FIFO
    CanWrite = (pSpiMf->m_NumToDo > pSpiMf->m_TxFifoSize) ? pSpiMf->m_TxFifoSize
                                                          : pSpiMf->m_NumToDo;
    CanWrite /= 4;

    for (i=0; i<CanWrite; i++)
        DtaFwbRegWrite(pFwbRegs, &pFwbSpiMf->WriteData, *pSpiMf->m_pData++);

    // Update number bytes to write
    pSpiMf->m_NumToDo -= (CanWrite*4);

    // Enable the half empty interrupt if more than FIFO-size has to be written
    if (pSpiMf->m_NumToDo > 0)
        ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_TxHalfEmptyIntEnable, 1);


    // Wait for completion of the operation
    Result = DtEventWaitUnInt(&pSpiMf->m_OpComplEvent, -1);

    return pSpiMf->m_Result;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SpiMfEraseIntDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void SpiMfEraseIntDpc(DtaSpiMf* pSpiMf, UInt32 IntReason)
{
    const DtFwbSpiMasterFlash* pFwbSpiMf = pSpiMf->m_pFwbSpiMf;
    volatile UInt8* pFwbRegs = pSpiMf->m_pFwbRegs;

    // Check timeout
    Int IsTimedOut = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_Timeout);
    if (IsTimedOut)
        pSpiMf->m_Result = DT_STATUS_TIMEOUT;

    //Signal completion event
    pSpiMf->m_State = SPIMF_STATE_IDLE;
    DtEventSet(&pSpiMf->m_OpComplEvent);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SpiMfReadIntDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void SpiMfReadIntDpc(DtaSpiMf* pSpiMf, UInt32 IntReason)
{
    const DtFwbSpiMasterFlash* pFwbSpiMf = pSpiMf->m_pFwbSpiMf;
    volatile UInt8* pFwbRegs = pSpiMf->m_pFwbRegs;
    Int i, CanRead, FifoLoad;

    // Get current space in FIFO
    FifoLoad = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->FifoLoad_RxFifoLoad);


    // Compute the number of words that can be read from the FIFO
    CanRead = (pSpiMf->m_NumToDo  > FifoLoad) ? FifoLoad : pSpiMf->m_NumToDo;
    CanRead /= 4;

    for (i=0; i<CanRead; i++)
        *pSpiMf->m_pData++ = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->ReadData);

    // Update number bytes to read
    pSpiMf->m_NumToDo -= (CanRead*4);

    // Not yet done?
    if ((IntReason&DTFWB_SPIMF_STAT_DONE) == 0)
    {
        // Enable the half full interrupt if more than FIFO-size has to be read
        if (pSpiMf->m_NumToDo >  pSpiMf->m_RxFifoSize)
            ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_RxHalfFullIntEnable, 1);
    }
    else
    {
        // Done 
        if (pSpiMf->m_NumToDo != 0)
        {
            DT_ASSERT(pSpiMf->m_NumToDo == 0);
            pSpiMf->m_Result = DT_STATUS_FAIL;
        }

        //Signal completion event
        pSpiMf->m_State = SPIMF_STATE_IDLE;
        DtEventSet(&pSpiMf->m_OpComplEvent);
    }
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SpiMfWriteIntDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void SpiMfWriteIntDpc(DtaSpiMf* pSpiMf, UInt32 IntReason)
{
    const DtFwbSpiMasterFlash* pFwbSpiMf = pSpiMf->m_pFwbSpiMf;
    volatile UInt8* pFwbRegs = pSpiMf->m_pFwbRegs;
    Int i, CanWrite;


    // Half empty interrupt?
    if ((IntReason&DTFWB_SPIMF_STAT_TX_HALFEMPTY) != 0)
    {
        // Get current space in FIFO
        Int FifoFree = pSpiMf->m_TxFifoSize -
            DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->FifoLoad_TxFifoLoad);
        // Compute the number of words that can be written in the FIFO
        CanWrite = (pSpiMf->m_NumToDo > FifoFree) ? FifoFree : pSpiMf->m_NumToDo;
        CanWrite /= 4;

        for (i=0; i<CanWrite; i++)
            DtaFwbRegWrite(pFwbRegs, &pFwbSpiMf->WriteData, *pSpiMf->m_pData++);

        // Update number bytes to write
        pSpiMf->m_NumToDo -= (CanWrite*4);

        // Enable the half empty interrupt if more than FIFO-size has to be written
        if (pSpiMf->m_NumToDo > 0)
            ControlRegWrite(pFwbRegs, &pFwbSpiMf->Control_TxHalfEmptyIntEnable, 1);

    }
    // Done interrupt
    if ((IntReason&DTFWB_SPIMF_STAT_DONE) != 0)
    {
        // Check timeout
        Int IsTimedOut = DtaFwbRegRead(pFwbRegs, &pFwbSpiMf->Status_Timeout);
        if (IsTimedOut)
            pSpiMf->m_Result = DT_STATUS_TIMEOUT;
        else if (pSpiMf->m_NumToDo != 0)
        {
            DT_ASSERT(pSpiMf->m_NumToDo == 0);
            pSpiMf->m_Result = DT_STATUS_FAIL;
        }
        //Signal completion event
        pSpiMf->m_State = SPIMF_STATE_IDLE;
        DtEventSet(&pSpiMf->m_OpComplEvent);
    }
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SpiMfExclAccessAcquire -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  SpiMfExclAccessAcquire(DtaSpiMf* pSpiMf, DtaNonIpPort* pNonIpPort, 
                                                                      DtFileObject* pFile)
{
    DtStatus  Result = DT_STATUS_OK;
    if (DtMutexAcquire(&pSpiMf->m_ExclAccessLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;

    if (!pSpiMf->m_ExclAccess)
    {
        // Prepare port for exclusive use by SPI-Masterflash
        Result = pSpiMf->m_pSpiMfPrepare(pNonIpPort, pFile);
        if (DT_SUCCESS(Result))
        {
            pSpiMf->m_ExclAccess = TRUE;
            pSpiMf->m_ExclAccessOwner = *pFile;
        }
    }
    else
        Result = DT_STATUS_IN_USE;
    DtMutexRelease(&pSpiMf->m_ExclAccessLock);

    return Result;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SpiMfExclAccessRelease -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  SpiMfExclAccessRelease(DtaSpiMf* pSpiMf, DtaNonIpPort* pNonIpPort, 
                                                                      DtFileObject* pFile)
{
    DtStatus  Result = DT_STATUS_OK;

    // Don't forget to lock the device again
    if (SpiMfCheckExclAccess(pSpiMf, pFile) == DT_STATUS_OK)
        SpiMfDeviceLock(pSpiMf, pFile, 1);

    // Thereafter release exclusive access
    if (DtMutexAcquire(&pSpiMf->m_ExclAccessLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;
    if (pSpiMf->m_ExclAccess)
    {
        if (DtFileCompare(&pSpiMf->m_ExclAccessOwner, pFile))
        {
            // Release port for exclusive use by SPI-Masterflash
            Result = pSpiMf->m_pSpiMfRelease(pNonIpPort, pFile);
            pSpiMf->m_ExclAccess = FALSE;
        }
        else
            Result = DT_STATUS_IN_USE;
    }
    DtMutexRelease(&pSpiMf->m_ExclAccessLock);

    return Result;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SpiMfExclAccessProbe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  SpiMfExclAccessProbe(DtaSpiMf* pSpiMf)
{
    DtStatus  Result = DT_STATUS_OK;
    if (DtMutexAcquire(&pSpiMf->m_ExclAccessLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;
    if (pSpiMf->m_ExclAccess)
        Result = DT_STATUS_IN_USE;
    DtMutexRelease(&pSpiMf->m_ExclAccessLock);

    return Result;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- SpiMfCheckExclAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  SpiMfCheckExclAccess(DtaSpiMf* pSpiMf, DtFileObject*  pFile)
{
    DtStatus  Result = DT_STATUS_OK;
    if (DtMutexAcquire(&pSpiMf->m_ExclAccessLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;
    if (pSpiMf->m_ExclAccess)
    {
        if (!DtFileCompare(&pSpiMf->m_ExclAccessOwner, pFile))
            Result = DT_STATUS_IN_USE;
    }
    else
        Result = DT_STATUS_EXCL_ACCESS_REQD;
    
    DtMutexRelease(&pSpiMf->m_ExclAccessLock);

    return Result;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- ControlRegWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void   ControlRegWrite(volatile UInt8* pFwbRegs, const DtFwField*  pField, UInt32  Value)
{
    if (pField->NumBits == 32)
    {
        WRITE_UINT(Value, pFwbRegs, pField->Offset);
    }
    else
    {
        // Read current value
        UInt32 Mask;
        UInt32 RegValue = READ_UINT(pFwbRegs, pField->Offset);

        // Mask the Command and Unlock
        RegValue &= DTFWB_SPIMF_CTRL_INT_MASK;
        Mask = ((1<<pField->NumBits)-1);
        Mask <<= pField->StartBit;

        // Or bits 
        RegValue &= ~Mask;
        RegValue |= Value<< pField->StartBit;

        // Write new value
        WRITE_UINT(RegValue, pFwbRegs, pField->Offset);
    }   
}
