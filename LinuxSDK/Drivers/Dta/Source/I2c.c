//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* I2c.c *#*#*#*#*#*#*#*#*#*# (C) 2010-2012 DekTec
//
// Dta driver - Dta I2C read/write routines.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2012 DekTec Digital Video B.V.
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
#include <DtaIncludes.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void DtaI2cCompletedDpc(DtDpcArgs* pArgs);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaI2cInitValues -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaI2cInitValues(
    DtaDeviceData*  pDvcData,
    Bool  IsSupported, 
    Int  ClockFreq, 
    Int  I2cOffset, 
    DtaI2c*  pI2c)
{
    DtStatus  Status = DT_STATUS_OK;

    pI2c->m_IsSupported = IsSupported;
    pI2c->m_ClockFreq = ClockFreq;

    if (IsSupported)    
        pI2c->m_pI2cRegs = pDvcData->m_pGenRegs + I2cOffset;
    else 
        pI2c->m_pI2cRegs = NULL;

    // Initialize additional I2cLock function
    pI2c->m_pI2cLockFunction = NULL;
    pI2c->m_pLockOwner = NULL;

    // Initialize implementation data
    DtFastMutexInit(&pI2c->m_AccessMutex);
    DtMutexInit(&pI2c->m_LockMutex);
    
    Status = DtEventInit(&pI2c->m_LockEvent, TRUE);
    if (!DT_SUCCESS(Status))
        return Status;

    Status = DtEventSet(&pI2c->m_LockEvent);
    if (!DT_SUCCESS(Status))
        return Status;

    pI2c->m_Done = FALSE;
    Status = DtEventInit(&pI2c->m_I2cDoneEvent, FALSE);
    if (!DT_SUCCESS(Status))
        return Status;
    
    return DtDpcInit(&pI2c->m_I2cCompletedDpc, DtaI2cCompletedDpc, TRUE);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaI2cInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaI2cInit(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i;
    UInt16  I2cOffset = 0;
    Bool IsSupported = FALSE;
    Int  ClockFreq = 0;

    DtStatus  PropertyStatus = DT_STATUS_OK;
    DtPropertyValue  Value = (DtPropertyValue)-1;
    DtPropertyValueType  Type;
    DtPropertyScope  Scope;

    // Initialize properties    
    DtPropertyData*  pPropData = &pDvcData->m_PropData;    
       
    // Initialize properties on device level
    IsSupported = DtPropertiesGetBool(pPropData, "HARDWARE_HAS_I2C", -1);
    ClockFreq = DtPropertiesGetInt(pPropData, "HARDWARE_I2C_FREQ", -1);

    // Check if no property error occurred
    Status = DtaPropertiesReportDriverErrors(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Initialise I2c on device level    
    Status = DtaI2cInitValues(pDvcData, IsSupported, ClockFreq, DT_GEN_REG_I2CBASE, 
                                                                        &pDvcData->m_I2c);
    if (!DT_SUCCESS(Status))
        return Status;

    // Initialize I2C on port level
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {        
        // Get I2c port register direct without checking.
        // the I2c register may not be defined, which is OK.
        PropertyStatus = DtPropertiesGet(pPropData, 
                                   "REGISTERS_I2C", i, &Value, &Type, &Scope, -1, -1, -1);
        
        if (DT_SUCCESS(PropertyStatus))
        {            
            DT_ASSERT((Scope&PROPERTY_SCOPE_DRIVER) == PROPERTY_SCOPE_DRIVER);
            DT_ASSERT(Type == PROPERTY_VALUE_TYPE_UINT16);
            I2cOffset = (UInt16)Value;
            IsSupported = TRUE;
        } else {
            I2cOffset = 0;
            IsSupported = FALSE;
        }

        Status = DtaI2cInitValues(pDvcData, IsSupported, ClockFreq, I2cOffset, 
                                                       &pDvcData->m_pNonIpPorts[i].m_I2c);
        if (!DT_SUCCESS(Status))
            return Status;
    }     

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaI2cLockCallBackSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaI2cLockCallBackSet(DtaDeviceData* pDvcData, Int PortIndex,
                          void (*pFunc)(DtaDeviceData* pDvcData, Int PortIndex, Int Lock))
{
    DtaI2c*  pI2c;
 
    if (PortIndex < 0)
        pI2c = &pDvcData->m_I2c;
    else
        pI2c = &pDvcData->m_pNonIpPorts[PortIndex].m_I2c;

    // Currently only one callback function per I2c device is supported
    if (pI2c->m_pI2cLockFunction!=NULL && pI2c->m_pI2cLockFunction!=pFunc)
    {
        DtDbgOut(ERR, I2C, "I2cLockCallback function already in use");
        return DT_STATUS_IN_USE;
    }
        
    // Set additional I2cLock function
    pI2c->m_pI2cLockFunction = pFunc;        
    DtDbgOut(MAX, I2C, "I2cLockCallback set");

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaI2cCleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaI2cCleanup(DtaDeviceData* pDvcData)
{
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaI2cPowerUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaI2cPowerUp(DtaDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    volatile UInt8*  pI2cRegs;
    UInt  ClkDiv;
    Int  i;
    DtaI2c*  pI2c = &pDvcData->m_I2c;
    
    pI2cRegs = pI2c->m_pI2cRegs;

    if (pI2c->m_IsSupported && pI2c->m_ClockFreq > 0)
    {
        // Powerup on device level
        // Note: Device may specify to use I2C but do not require to set the speed.
        //       In that case, it is defined <= 0 (DeviceDescription.xml).
        // Powerup device level I2c
        Status = DtFastMutexAcquire(&pI2c->m_AccessMutex);
        if (!DT_SUCCESS(Status))
            return Status; 

        ClkDiv = pDvcData->m_DevInfo.m_RefClk/pI2c->m_ClockFreq;
        ClkDiv = ClkDiv/4;  // All I2C devices have a built-in multiplier of 4
        DtaRegI2cCtrlSetClkDiv(pI2cRegs, ClkDiv);
        DtFastMutexRelease(&pI2c->m_AccessMutex);
    }

    // Powerup on portlevel
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {   
        pI2c = &pDvcData->m_pNonIpPorts[i].m_I2c;
        pI2cRegs = pI2c->m_pI2cRegs;

        if (pI2c->m_IsSupported && pI2c->m_ClockFreq > 0)
        {
            Status = DtFastMutexAcquire(&pI2c->m_AccessMutex);
            if (!DT_SUCCESS(Status))
                return Status;

            ClkDiv = pDvcData->m_DevInfo.m_RefClk/pI2c->m_ClockFreq;
            ClkDiv = ClkDiv/4;  // All I2C devices have a built-in multiplier of 4
            DtaRegI2cCtrlSetClkDiv(pI2c->m_pI2cRegs, ClkDiv);
            DtFastMutexRelease(&pI2c->m_AccessMutex);            
        }        
    }    
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaI2cIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaI2cIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile, DtIoctlObject* pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    DtaIoctlI2cCmdInput*  pI2cCmdInput = (DtaIoctlI2cCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlI2cCmdOutput*  pI2cCmdOutput = (DtaIoctlI2cCmdOutput*)pIoctl->m_pOutputBuffer;
    
    InReqSize = OFFSETOF(DtaIoctlI2cCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlI2cCmdOutput, m_Data);

    // Check if we can read m_Cmd
    if (pIoctl->m_InputBufferSize < OFFSETOF(DtaIoctlI2cCmdInput, m_Data))
        return DT_STATUS_INVALID_PARAMETER;

    switch (pI2cCmdInput->m_Cmd)
    {
    case DTA_I2C_CMD_LOCK:
        pCmdStr = "DTA_I2C_CMD_LOCK";
        InReqSize += sizeof(DtaIoctlI2cCmdLockInput);
        // We expect no output buffer
        OutReqSize = 0;
        break;

    case DTA_I2C_CMD_UNLOCK:
        pCmdStr = "DTA_I2C_CMD_UNLOCK";
        // We expect no output buffer
        OutReqSize = 0;
        break;

    case DTA_I2C_CMD_READ:
        pCmdStr = "DTA_I2C_CMD_READ";
        InReqSize += sizeof(DtaIoctlI2cCmdReadInput);
        // Check for valid input buffer before reading it
        if (pIoctl->m_InputBufferSize >= InReqSize)
            OutReqSize += sizeof(UInt)+pI2cCmdInput->m_Data.m_Read.m_OutBufLen;
        break;

    case DTA_I2C_CMD_WRITE:
        pCmdStr = "DTA_I2C_CMD_WRITE";
        InReqSize += OFFSETOF(DtaIoctlI2cCmdWriteInput, m_Buf); // Exclude m_Buf
        // Check for valid input buffer before reading it
        if (pIoctl->m_InputBufferSize >= InReqSize)
            InReqSize += pI2cCmdInput->m_Data.m_Write.m_BufLen;
        // We expect no output buffer
        OutReqSize = 0;
        break;

    case DTA_I2C_CMD_WRITEREAD:
        pCmdStr = "DTA_I2C_CMD_WRITEREAD";
        InReqSize += OFFSETOF(DtaIoctlI2cCmdWriteReadInput, m_Buf); // Exclude m_Buf
        // Check for valid input buffer before reading it
        if (pIoctl->m_InputBufferSize >= InReqSize)
        {
            InReqSize += pI2cCmdInput->m_Data.m_WriteRead.m_BufLen;
            OutReqSize += sizeof(UInt)+pI2cCmdInput->m_Data.m_WriteRead.m_OutBufLen;
        }
        break;

    default:
        pCmdStr = "??UNKNOWN VPDCMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, I2C, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, I2C, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, I2C, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
            pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;
    

    if (DT_SUCCESS(Status))
    {
        switch (pI2cCmdInput->m_Cmd)
        {
        case DTA_I2C_CMD_LOCK:
            Status = DtaI2cLock(pDvcData, pI2cCmdInput->m_PortIndex, 
                                          pFile, pI2cCmdInput->m_Data.m_Lock.m_TimeoutMS);
            break;

        case DTA_I2C_CMD_UNLOCK:
            Status = DtaI2cUnlock(pDvcData, pI2cCmdInput->m_PortIndex, pFile, FALSE);
            break;

        case DTA_I2C_CMD_READ:
            Status = DtaI2cRead(pDvcData, pI2cCmdInput->m_PortIndex, pFile, 
                                                  pI2cCmdInput->m_Data.m_Read.m_DevAddr,  
                                                  pI2cCmdInput->m_Data.m_Read.m_OutBufLen, 
                                                  pI2cCmdOutput->m_Data.m_Read.m_Buf);
            if (DT_SUCCESS(Status))
                pI2cCmdOutput->m_Data.m_Read.m_Length =
                                                  pI2cCmdInput->m_Data.m_Read.m_OutBufLen;
            else
                pI2cCmdOutput->m_Data.m_Read.m_Length = 0;
            break;

        case DTA_I2C_CMD_WRITE:
            Status = DtaI2cWrite(pDvcData, pI2cCmdInput->m_PortIndex, pFile, 
                                                   pI2cCmdInput->m_Data.m_Write.m_DevAddr,
                                                   pI2cCmdInput->m_Data.m_Write.m_BufLen, 
                                                   pI2cCmdInput->m_Data.m_Write.m_Buf);
            break;

        case DTA_I2C_CMD_WRITEREAD:
            Status = DtaI2cWriteRead(pDvcData, pI2cCmdInput->m_PortIndex, pFile,
                                          pI2cCmdInput->m_Data.m_WriteRead.m_DevWriteAddr,
                                          pI2cCmdInput->m_Data.m_WriteRead.m_BufLen,
                                          pI2cCmdInput->m_Data.m_WriteRead.m_Buf,
                                          pI2cCmdInput->m_Data.m_WriteRead.m_DevReadAddr,
                                          pI2cCmdInput->m_Data.m_WriteRead.m_OutBufLen,
                                          pI2cCmdOutput->m_Data.m_Read.m_Buf);
            if (DT_SUCCESS(Status))
                pI2cCmdOutput->m_Data.m_Read.m_Length =
                                             pI2cCmdInput->m_Data.m_WriteRead.m_OutBufLen;
            else
                pI2cCmdOutput->m_Data.m_Read.m_Length = 0;
            break;

        default:
            Status = DT_STATUS_NOT_SUPPORTED;
        }
    }

    // If we failed, no data has te be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED)
            DtDbgOut(MIN, I2C, "I2CCmd=0x%x: NOT SUPPORTED", pI2cCmdInput->m_Cmd);
        else 
            DtDbgOut(MIN, I2C, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaI2cCompletedDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtaI2cCompletedDpc(DtDpcArgs* pArgs)
{
     DtaI2c*  pI2c = (DtaI2c*)pArgs->m_pContext;
     DtDbgOut(MAX, I2C, "I2C transfer processed"); 
     DtEventSet(&pI2c->m_I2cDoneEvent);
     pI2c->m_Done = TRUE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaI2cLock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaI2cLock(
    DtaDeviceData*  pDvcData, 
    Int  PortIndex, 
    DtFileObject*  pFile, 
    Int  TimeoutMS)
{
    DtStatus  Status = DT_STATUS_OK;
    void*  pFileHandle;
    DtaI2c*  pI2c;

    if (PortIndex < 0)
        pI2c = &pDvcData->m_I2c;
    else
        pI2c = &pDvcData->m_pNonIpPorts[PortIndex].m_I2c;

    // Do we support the I2C interface
    if (!pI2c->m_IsSupported)
    {
        DtDbgOut(ERR, I2C, "DTA-%d does not support the I2C interface",
                                                        pDvcData->m_DevInfo.m_TypeNumber);
        return DT_STATUS_NOT_SUPPORTED;
    }

    pFileHandle = DtFileGetHandle(pFile);
    
    // Wait for the mutex
    Status = DtMutexAcquire(&pI2c->m_LockMutex, -1);
    if (Status != DT_STATUS_OK)
        return Status;

    // We are now holding the mutex
    // Check if we are already holding the I2C lock
    if (pI2c->m_pLockOwner == pFileHandle)
    {
        pI2c->m_LockCount++;

        // Release mutex
        DtMutexRelease(&pI2c->m_LockMutex);
    } else {
        // Release mutex
        DtMutexRelease(&pI2c->m_LockMutex);
        Status = DtEventWait(&pI2c->m_LockEvent, TimeoutMS);
        if (DT_SUCCESS(Status)) 
        {
            DtMutexAcquire(&pI2c->m_LockMutex, -1);
            DT_ASSERT(pI2c->m_pLockOwner == NULL);
            DtDbgOut(MAX, I2C, "New I2C owner (file: 0x%p)", pFileHandle);
            pI2c->m_pLockOwner = pFileHandle;
            pI2c->m_LockCount = 1;
           
            // Check for additional lock function to lock
            if (pI2c->m_pI2cLockFunction != NULL)
                (*pI2c->m_pI2cLockFunction)(pDvcData, PortIndex, 1);

            // Release mutex
            DtMutexRelease(&pI2c->m_LockMutex);
        }
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaI2cUnlock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaI2cUnlock(
    DtaDeviceData*  pDvcData, 
    Int  PortIndex, 
    DtFileObject*  pFile, 
    Bool  Recursive)
{
    DtStatus  Status = DT_STATUS_OK;
    void*  pFileHandle;
    DtaI2c*  pI2c;

    if (PortIndex < 0)
        pI2c = &pDvcData->m_I2c;
    else
        pI2c = &pDvcData->m_pNonIpPorts[PortIndex].m_I2c;

    // Do we support the I2C interface
    if (!pI2c->m_IsSupported)
    {
        DtDbgOut(ERR, I2C, "DTA-%d port %d does not support the I2C interface",
                                           pDvcData->m_DevInfo.m_TypeNumber, PortIndex+1);
        return DT_STATUS_NOT_SUPPORTED;
    }
    
    // Check if we are holding the lock
    pFileHandle = DtFileGetHandle(pFile);
    if (pI2c->m_pLockOwner != pFileHandle)
        return DT_STATUS_FAIL;

    // Wait for the mutex
    Status = DtMutexAcquire(&pI2c->m_LockMutex, -1);
    if (Status != DT_STATUS_OK)
        return Status;
    
    // Decrement the lock count
    pI2c->m_LockCount--;

    // Check if this was the last one
    if (pI2c->m_LockCount==0 || Recursive)
    {
        // Check for additional lock function to unlock
        if (pI2c->m_pI2cLockFunction != NULL)
            (*pI2c->m_pI2cLockFunction)(pDvcData, PortIndex, 0);

        // Clear filehandle while holding the lock
        DtDbgOut(MAX, I2C, "Release I2C owner (file: 0x%p)", pFileHandle);
        pI2c->m_pLockOwner = NULL;

        // No owner: set LockCount to 0
        pI2c->m_LockCount = 0;
            
        // Trigger a possible waiting process
        DtEventSet(&pI2c->m_LockEvent);
    }
        
    // Release mutex
    DtMutexRelease(&pI2c->m_LockMutex);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaI2cRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaI2cRead(
    DtaDeviceData*  pDvcData,   // Device object
    Int  PortIndex,             // PortIndex
    DtFileObject*  pFile,       // File object for access verification
    UInt  DvcAddr,              // I2C Device address
    UInt  Length,               // Number of BYTES to read
    UInt8*  pBuf)               // Buffer for data read from I2C bus
{
    DtStatus  Status = DT_STATUS_OK;
    void*  pFileHandle;
    Int  i;
    UInt8*  p;
    UInt32  w;
    Int  BytesLeft = Length;    // Bytes left to transfer
    volatile UInt8*  pI2cRegs;
    DtaI2c*  pI2c;

    if (PortIndex < 0)
        pI2c = &pDvcData->m_I2c;
    else
        pI2c = &pDvcData->m_pNonIpPorts[PortIndex].m_I2c;

    pI2cRegs = pI2c->m_pI2cRegs;

    DtDbgOut(MAX, I2C, "DvcAddr: 0x%02X, Length: %d, pBuf: %p", DvcAddr, Length, pBuf);

    // Do we support the I2C interface
    if (!pI2c->m_IsSupported)
    {
        DtDbgOut(ERR, I2C, "DTA-%d port %d does not support the I2C interface",
                                           pDvcData->m_DevInfo.m_TypeNumber, PortIndex+1);
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Sanity check: valid buffer size
    if (Length > DTA_I2C_MAX_BUF_SIZE)
        return DT_STATUS_INVALID_PARAMETER;

    // Nothing to transfer
    if (BytesLeft == 0)
        return DT_STATUS_OK;

    // Protect I2C access against concurrent access
    Status = DtFastMutexAcquire(&pI2c->m_AccessMutex);
    if (!DT_SUCCESS(Status))
        return Status;

    // Check that I2C wasn't locked by someone else
    pFileHandle = DtFileGetHandle(pFile);
    if (pI2c->m_pLockOwner!=NULL && pI2c->m_pLockOwner!=pFileHandle)
    {
        // Release I2C mutex
        DtFastMutexRelease(&pI2c->m_AccessMutex);
        return DT_STATUS_IN_USE;
    }

    // clear events
    DtEventReset(&pI2c->m_I2cDoneEvent);
    pI2c->m_Done = FALSE;

    // Use higher address part as i2c bus address
    DtaRegI2cCtrlSetBusSel(pI2cRegs, DvcAddr>>8);

    // Start I2C read transfer by writing device adress (7 MSBs only) and transfer count
    DtaRegI2cAddrCntSetRdAddrAndCount(pI2cRegs, (DvcAddr>>1), Length);

    // Wait 5 secs for interrupt
    Status = DtEventWait(&pI2c->m_I2cDoneEvent, 5000);
    if (Status != DT_STATUS_OK)
    {
        if (Status == DT_STATUS_TIMEOUT)
            DtDbgOut(ERR, I2C, "Transfer Timeout!!");
        else
            DtDbgOut(ERR, I2C, "Transfer Error!!");

        // Release I2C mutex
        DtFastMutexRelease(&pI2c->m_AccessMutex);
        return Status;
    }

    // Check if transfer was Nack-ed
    if (DtaRegI2cStatusGetRdAddrNack(pI2cRegs) != 0)
    {
        DtDbgOut(ERR, I2C, "Address read NACK");

        // Release I2C mutex
        DtFastMutexRelease(&pI2c->m_AccessMutex);
        return DT_STATUS_FAIL;
    }

    // Read data from I2C onboard buffer (we must read 4 bytes at a time)
    p = pBuf;
    for (i=0; i<(BytesLeft/4); i++)
    {
        w = DtaRegI2cDataGet(pI2cRegs);
        *((UInt32*)p) = (UInt32)w;
        p += 4;
    }
    // Any bytes left to read
    BytesLeft %= 4;
    if (BytesLeft > 0)
    {
        // Read one extra word to get last few bytes
        w = DtaRegI2cDataGet(pI2cRegs);
        for (i=0; i<BytesLeft; i++)
        {
            p[i] = (UInt8)(w&0xFF);
            w >>= 8;
        }
    }

    // Release I2C mutex
    DtFastMutexRelease(&pI2c->m_AccessMutex);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaI2cWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaI2cWrite(
    DtaDeviceData*  pDvcData,   // Device object
    Int  PortIndex,             // Port index
    DtFileObject*  pFile,       // File object for access verification
    UInt  DvcAddr,              // I2C Device address
    UInt  Length,               // Number of BYTES to write
    UInt8*  pBuf)               // Buffer with data to be written to I2C bus
{
    DtStatus  Status = DT_STATUS_OK;
    void*  pFileHandle;
    Int  i;
    UInt8*  p;
    UInt32  w;
    Int  BytesLeft = Length;    // Bytes left to transfer
    volatile UInt8*  pI2cRegs;
    DtaI2c*  pI2c;

    if (PortIndex < 0)
        pI2c = &pDvcData->m_I2c;
    else
        pI2c = &pDvcData->m_pNonIpPorts[PortIndex].m_I2c;

    pI2cRegs = pI2c->m_pI2cRegs;

    DtDbgOut(MAX, I2C, "DvcAddr: 0x%02X, Length: %d, pBuf: %p", DvcAddr, Length, pBuf);

    // Do we support the I2C interface
    if (!pI2c->m_IsSupported)
    {
        DtDbgOut(ERR, I2C, "DTA-%d does not support the I2C interface",
                                                     pDvcData->m_DevInfo.m_TypeNumber);
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Sanity check: valid buffer size
    if (Length > DTA_I2C_MAX_BUF_SIZE)
        return DT_STATUS_INVALID_PARAMETER;

    // Nothing to transfer
    if (BytesLeft == 0)
        return DT_STATUS_OK;

    // Protect I2C access against concurrent access
    Status = DtFastMutexAcquire(&pI2c->m_AccessMutex);
    if (!DT_SUCCESS(Status))
        return Status;

    // Check that I2C wasn't locked by someone else
    pFileHandle = DtFileGetHandle(pFile);
    if (pI2c->m_pLockOwner!=NULL && pI2c->m_pLockOwner!=pFileHandle)
    {
        // Release I2C mutex
        DtFastMutexRelease(&pI2c->m_AccessMutex);
        return DT_STATUS_IN_USE;
    }

    // clear events
    DtEventReset(&pI2c->m_I2cDoneEvent);
    pI2c->m_Done = FALSE;

    // Use higher address part as i2c bus address
    DtaRegI2cCtrlSetBusSel(pI2cRegs, DvcAddr>>8);

    // First write I2C data to onboard I2C buffer (4 bytes at a time)
    p = pBuf;
    for (i=0; i<(BytesLeft/4); i++)
    {
        w = (p[3]<<24) + (p[2]<<16) + (p[1]<<8) + p[0];
        p += 4;
        DtaRegI2cDataSet(pI2cRegs, w);
    }
    // Check for remaining bytes
    BytesLeft %= 4;
    if (BytesLeft > 0)
    {
        // Write last few bytes
        w = 0;
        for (i=(BytesLeft-1); i>=0; i--)
        {
            w <<=8;
            w += p[i];
        }
        DtaRegI2cDataSet(pI2cRegs, w);
    }

    // Start I2C write transfer by writing device adress (7 MSBs only) and transfer count
    DtaRegI2cAddrCntSetWrAddrAndCount(pI2cRegs, (DvcAddr>>1), Length);

    // Wait 5 secs for interrupt
    Status = DtEventWait(&pI2c->m_I2cDoneEvent, 5000);
    if (Status != DT_STATUS_OK)
    {
        if (Status == DT_STATUS_TIMEOUT)
            DtDbgOut(ERR, I2C, "Transfer Timeout!!");
        else
            DtDbgOut(ERR, I2C, "Transfer Error!!");

        // Release I2C mutex
        DtFastMutexRelease(&pI2c->m_AccessMutex);
        return Status;
    }
    
    // Check if transfer was Nack-ed
    if (DtaRegI2cStatusGetWrAddrNack(pI2cRegs) != 0)
    {
        DtDbgOut(ERR, I2C, "Adress write NACK");
        Status = DT_STATUS_FAIL;
    }
    if (DtaRegI2cStatusGetWrDataNack(pI2cRegs) != 0)
    {
        DtDbgOut(ERR, I2C, "Data NACK");
        Status = DT_STATUS_FAIL;
    }

    // Release I2C mutex
    DtFastMutexRelease(&pI2c->m_AccessMutex);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaI2cWriteRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaI2cWriteRead(
    DtaDeviceData*  pDvcData,   // Device object
    Int  PortIndex,             // Port index
    DtFileObject*  pFile,       // File object for access verification
    UInt  DvcWriteAddr,         // I2C Device write address
    UInt  WriteLength,          // Number of BYTES to write
    UInt8*  pWriteBuf,          // Buffer with data to be written to I2C bus
    UInt  DvcReadAddr,          // I2C Device read address
    UInt  ReadLength,           // Number of BYTES to read
    UInt8*  pReadBuf)           // Buffer for data read from I2C bus
{
    DtStatus  Status = DT_STATUS_OK;
    void*  pFileHandle;
    Int  i;
    UInt8*  p;
    UInt32  w;
    Int  WriteBytesLeft = WriteLength;  // Bytes left to write
    Int  ReadBytesLeft = ReadLength;    // Bytes left to read
    //volatile UInt8*  pGenRegs = pDvcData->m_pGenRegs;
    volatile UInt8*  pI2cRegs;
    DtaI2c*  pI2c;

    if (PortIndex < 0)
        pI2c = &pDvcData->m_I2c;
    else
        pI2c = &pDvcData->m_pNonIpPorts[PortIndex].m_I2c;

    pI2cRegs = pI2c->m_pI2cRegs;

    DtDbgOut(MAX, I2C, 
        "DvcWriteAddr: 0x%02X, WriteLength: %d, pWriteBuf: %p, DvcReadAddr: 0x%02X, ReadLength: %d, pReadBuf: %p",
        DvcWriteAddr, WriteLength, pWriteBuf, DvcReadAddr, ReadLength, pReadBuf);

    // Do we support the I2C interface
    if (!pI2c->m_IsSupported)
    {
        DtDbgOut(ERR, I2C, "DTA-%d does not support the I2C interface",
                                                        pDvcData->m_DevInfo.m_TypeNumber);
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Sanity check: valid buffer size
    if (WriteLength>DTA_I2C_MAX_BUF_SIZE || ReadLength>DTA_I2C_MAX_BUF_SIZE)
        return DT_STATUS_INVALID_PARAMETER;

    // Sanity check: write and read address must be on the same bus
    if ((DvcWriteAddr&&~0xFF) != (DvcReadAddr&&~0xFF))
        return DT_STATUS_INVALID_PARAMETER;

    // Protect I2C access against concurrent access
    Status = DtFastMutexAcquire(&pI2c->m_AccessMutex);
    if (!DT_SUCCESS(Status))
        return Status;

    // Check that I2C wasn't locked by someone else
    pFileHandle = DtFileGetHandle(pFile);
    if (pI2c->m_pLockOwner!=NULL && pI2c->m_pLockOwner!=pFileHandle)
    {
        // Release I2C mutex
        DtFastMutexRelease(&pI2c->m_AccessMutex);
        return DT_STATUS_IN_USE;
    }

    // clear events
    DtEventReset(&pI2c->m_I2cDoneEvent);
    pI2c->m_Done = FALSE;

    // Use higher address part as i2c bus address
    DtaRegI2cCtrlSetBusSel(pI2cRegs, DvcWriteAddr>>8);

    // First write I2C data to onboard I2C buffer (4 bytes at a time)
    p = pWriteBuf;
    for (i=0; i<(WriteBytesLeft/4); i++)
    {
        w = (p[3]<<24) + (p[2]<<16) + (p[1]<<8) + p[0];
        p += 4;
        DtaRegI2cDataSet(pI2cRegs, w);
    }
    // Check for remaining bytes
    WriteBytesLeft %= 4;
    if (WriteBytesLeft > 0)
    {
        // Write last few bytes
        w = 0;
        for (i=(WriteBytesLeft-1); i>=0; i--)
        {
            w <<=8;
            w += p[i];
        }
        DtaRegI2cDataSet(pI2cRegs, w);
    }

    // Start I2C write transfer by writing device adress (7 MSBs only) and transfer counts
    DtaRegI2cAddrCntSetWrRdAddrAndCount(pI2cRegs, DvcWriteAddr>>1, WriteLength, DvcReadAddr>>1, ReadLength);

    // Wait 5 secs for interrupt
    Status = DtEventWait(&pI2c->m_I2cDoneEvent, 5000);
    if (Status != DT_STATUS_OK)
    {
        if (Status == DT_STATUS_TIMEOUT)
            DtDbgOut(ERR, I2C, "Transfer Timeout!!");
        else
            DtDbgOut(ERR, I2C, "Transfer Error!!");

        // Release I2C mutex
        DtFastMutexRelease(&pI2c->m_AccessMutex);
        return Status;
    }
    
    // Check if transfer was Nack-ed
    if (DtaRegI2cStatusGetWrAddrNack(pI2cRegs) != 0)
    {
        DtDbgOut(ERR, I2C, "Address write NACK");
        Status = DT_STATUS_FAIL;
    }
    if (DtaRegI2cStatusGetWrDataNack(pI2cRegs) != 0)
    {
        DtDbgOut(ERR, I2C, "Data NACK");
        Status = DT_STATUS_FAIL;
    }
    if (DtaRegI2cStatusGetRdAddrNack(pI2cRegs) != 0)
    {
        DtDbgOut(ERR, I2C, "Read address NACK");
        Status = DT_STATUS_FAIL;
    }

    if (Status != DT_STATUS_FAIL)
    {
        // Read data from I2C onboard buffer (we must read 4 bytes at a time)
        p = pReadBuf;
        for (i=0; i<(ReadBytesLeft/4); i++)
        {
            w = DtaRegI2cDataGet(pI2cRegs);
            *((UInt32*)p) = (UInt32)w;
            p += 4;
        }
        // Any bytes left to read
        ReadBytesLeft %= 4;
        if (ReadBytesLeft > 0)
        {
            // Read one extra word to get last few bytes
            w = DtaRegI2cDataGet(pI2cRegs);
            for (i=0; i<ReadBytesLeft; i++)
            {
                p[i] = (UInt8)(w&0xFF);
                w >>= 8;
            }
        }
    }

    // Release I2C mutex
    DtFastMutexRelease(&pI2c->m_AccessMutex);

    return Status;
}
