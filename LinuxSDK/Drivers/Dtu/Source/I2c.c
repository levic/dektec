//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* I2c.c *#*#*#*#*#*#*#*#*#*# (C) 2011-2015 DekTec
//
// Dtu driver - Dtu I2C read/write routines.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2015 DekTec Digital Video B.V.
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
#include <DtuIncludes.h>

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuI2cInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuI2cInit(DtuDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;

    // Initialize properties
    pDvcData->m_I2c.m_IsSupported = DtPropertiesGetBool(&pDvcData->m_PropData, 
                                                                  "HARDWARE_HAS_I2C", -1);

    // Check if no property error occurred
    Status = DtuPropertiesReportDriverErrors(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    // Initialize implementation data
    DtFastMutexInit(&pDvcData->m_I2c.m_AccessMutex);
    DtMutexInit(&pDvcData->m_I2c.m_LockMutex);
    DtEventInit(&pDvcData->m_I2c.m_LockEvent, TRUE);
    DtEventSet(&pDvcData->m_I2c.m_LockEvent);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuI2cCleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtuI2cCleanup(DtuDeviceData* pDvcData)
{
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuI2cIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuI2cIoctl(DtuDeviceData* pDvcData, DtFileObject* pFile, DtIoctlObject* pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    DtuIoctlI2cCmdInput*  pI2cCmdInput = (DtuIoctlI2cCmdInput*)pIoctl->m_pInputBuffer;
    DtuIoctlI2cCmdOutput*  pI2cCmdOutput = (DtuIoctlI2cCmdOutput*)pIoctl->m_pOutputBuffer;
    
    InReqSize = OFFSETOF(DtuIoctlI2cCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtuIoctlI2cCmdOutput, m_Data);

    // Check if we can read m_Cmd
    if (pIoctl->m_InputBufferSize < OFFSETOF(DtuIoctlI2cCmdInput, m_Data))
        return DT_STATUS_INVALID_PARAMETER;

    switch (pI2cCmdInput->m_Cmd)
    {
    case DTU_I2C_CMD_LOCK:
        pCmdStr = "DTU_I2C_CMD_LOCK";
        InReqSize += sizeof(DtuIoctlI2cCmdLockInput);
        // We expect no output buffer
        OutReqSize = 0;
        break;

    case DTU_I2C_CMD_UNLOCK:
        pCmdStr = "DTU_I2C_CMD_UNLOCK";
        // We expect no output buffer
        OutReqSize = 0;
        break;

    case DTU_I2C_CMD_READ:
        pCmdStr = "DTU_I2C_CMD_READ";
        InReqSize += sizeof(DtuIoctlI2cCmdReadInput);
        // Check for valid input buffer before reading it
        if (pIoctl->m_InputBufferSize >= InReqSize)
            OutReqSize += sizeof(UInt)+pI2cCmdInput->m_Data.m_Read.m_OutBufLen;
        break;

    case DTU_I2C_CMD_WRITE:
        pCmdStr = "DTU_I2C_CMD_WRITE";
        InReqSize += OFFSETOF(DtuIoctlI2cCmdWriteInput, m_Buf); // Exclude m_Buf
        // Check for valid input buffer before reading it
        if (pIoctl->m_InputBufferSize >= InReqSize)
            InReqSize += pI2cCmdInput->m_Data.m_Write.m_BufLen;
        // We expect no output buffer
        OutReqSize = 0;
        break;

    default:
        pCmdStr = "??UNKNOWN VPDCMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }
    DtDbgOut(MAX, I2C, "DtuI2cIoctl!!!");

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
        case DTU_I2C_CMD_LOCK:
            Status = DtuI2cLock(pDvcData, pFile, pI2cCmdInput->m_Data.m_Lock.m_TimeoutMS);
            break;

        case DTU_I2C_CMD_UNLOCK:
            Status = DtuI2cUnlock(pDvcData, pFile, FALSE);
            break;

        case DTU_I2C_CMD_READ:
            Status = DtuI2cRead(pDvcData, pFile, pI2cCmdInput->m_Data.m_Read.m_DevAddr,
                pI2cCmdInput->m_Data.m_Read.m_OutBufLen, pI2cCmdOutput->m_Data.m_Read.m_Buf);
            if (DT_SUCCESS(Status))
                pI2cCmdOutput->m_Data.m_Read.m_Length =
                                                      pI2cCmdInput->m_Data.m_Read.m_OutBufLen;
            else
                pI2cCmdOutput->m_Data.m_Read.m_Length = 0;
            break;

        case DTU_I2C_CMD_WRITE:
            Status = DtuI2cWrite(pDvcData, pFile, pI2cCmdInput->m_Data.m_Write.m_DevAddr,
                   pI2cCmdInput->m_Data.m_Write.m_BufLen, pI2cCmdInput->m_Data.m_Write.m_Buf);
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuI2cLock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuI2cLock(DtuDeviceData* pDvcData, DtFileObject* pFile, Int TimeoutMS)
{
    DtStatus  Status = DT_STATUS_OK;
    void*  pFileHandle = NULL;

    // Do we support the I2C interface
    if (!pDvcData->m_I2c.m_IsSupported)
    {
        DtDbgOut(ERR, I2C, "DTU-%d does not support the I2C interface",
                                                        pDvcData->m_DevInfo.m_TypeNumber);
        return DT_STATUS_NOT_SUPPORTED;
    }

    pFileHandle = DtFileGetHandle(pFile);
    
    // Wait for the mutex
    Status = DtMutexAcquire(&pDvcData->m_I2c.m_LockMutex, -1);
    if (Status != DT_STATUS_OK)
        return Status;

    // We are now holding the mutex
    // Check if we are already holding the I2C lock
    if (pDvcData->m_I2c.m_pLockOwner == pFileHandle)
    {
        pDvcData->m_I2c.m_LockCount++;

        // Release mutex
        DtMutexRelease(&pDvcData->m_I2c.m_LockMutex);
    } else {
        // Release mutex
        DtMutexRelease(&pDvcData->m_I2c.m_LockMutex);
        Status = DtEventWait(&pDvcData->m_I2c.m_LockEvent, TimeoutMS);
        if (DT_SUCCESS(Status)) 
        {
            DtMutexAcquire(&pDvcData->m_I2c.m_LockMutex, -1);
            DT_ASSERT(pDvcData->m_I2c.m_pLockOwner == NULL);
            DtDbgOut(MAX, I2C, "New I2C owner (file: 0x%p)", pFileHandle);
            pDvcData->m_I2c.m_pLockOwner = pFileHandle;
            pDvcData->m_I2c.m_LockCount = 1;

            // Release mutex
            DtMutexRelease(&pDvcData->m_I2c.m_LockMutex);
        }
    }
    
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuI2cUnlock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuI2cUnlock(DtuDeviceData* pDvcData, DtFileObject* pFile, Bool Recursive)
{
    DtStatus  Status = DT_STATUS_OK;
    void*  pFileHandle = NULL;

    // Do we support the I2C interface
    if (!pDvcData->m_I2c.m_IsSupported)
    {
        DtDbgOut(ERR, I2C, "DTU-%d does not support the I2C interface",
                                                        pDvcData->m_DevInfo.m_TypeNumber);
        return DT_STATUS_NOT_SUPPORTED;
    }
    
    // Check if we are holding the lock
    pFileHandle = DtFileGetHandle(pFile);
    if (pDvcData->m_I2c.m_pLockOwner != pFileHandle)
        return DT_STATUS_FAIL;

    // Wait for the mutex
    Status = DtMutexAcquire(&pDvcData->m_I2c.m_LockMutex, -1);
    if (Status != DT_STATUS_OK)
        return Status;
    
    // Decrement the lock count
    pDvcData->m_I2c.m_LockCount--;

    // Check if this was the last one
    if (pDvcData->m_I2c.m_LockCount==0 || Recursive)
    {
        // Clear filehandle while holding the lock
        DtDbgOut(MAX, I2C, "Release I2C owner (file: 0x%p)", pFileHandle);
        pDvcData->m_I2c.m_pLockOwner = NULL;
            
        // Trigger a possible waiting process
        DtEventSet(&pDvcData->m_I2c.m_LockEvent);
    }
        
    // Release mutex
    DtMutexRelease(&pDvcData->m_I2c.m_LockMutex);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu1xxI2cRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuI2cRead(
    DtuDeviceData*  pDvcData,   // Device object
    DtFileObject*  pFile,       // File object for access verification
    UInt  DvcAddr,              // I2C Device address
    UInt  Length,               // Number of BYTES to read
    UInt8*  pBuf)               // Buffer for data read from I2C bus
{
    DtStatus  Status = DT_STATUS_OK;
    void*  pFileHandle = NULL;
    UInt TransferSize, ActualTransferred, BufIndex;
    Int  BytesLeft = Length;    // Bytes left to transfer
    Int  MaxCtrlTrSize = DtUsbGetCtrlMaxPacketSize(&pDvcData->m_Device,
                                                          pDvcData->m_DevInfo.m_UsbSpeed);

    DtDbgOut(MAX, I2C, "DvcAddr: 0x%02X, Length: %d, pBuf: %p", DvcAddr, Length, pBuf);

    // Do we support the I2C interface
    if (!pDvcData->m_I2c.m_IsSupported)
    {
        DtDbgOut(ERR, I2C, "DTU-%d does not support the I2C interface",
                                                        pDvcData->m_DevInfo.m_TypeNumber);
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Sanity check: valid buffer size
    if (Length > DTU_I2C_MAX_BUF_SIZE)
        return DT_STATUS_INVALID_PARAMETER;

    // Nothing to transfer
    if (BytesLeft == 0)
        return DT_STATUS_OK;

    // Protect I2C access against concurrent access
    Status = DtFastMutexAcquire(&pDvcData->m_I2c.m_AccessMutex);
    if (!DT_SUCCESS(Status))
        return Status;

    // Check that I2C wasn't locked by someone else
    if (pFile != NULL)
        pFileHandle = DtFileGetHandle(pFile);
    if (pDvcData->m_I2c.m_pLockOwner!=NULL && pDvcData->m_I2c.m_pLockOwner!=pFileHandle)
    {
        // Release I2C mutex
        DtFastMutexRelease(&pDvcData->m_I2c.m_AccessMutex);
        return DT_STATUS_IN_USE;
    }

    // Divide transfer is chucks suitable for EP0
    BufIndex = 0;
    while (BytesLeft > 0)
    {
        // Do not transfer more than maximum allowed
        TransferSize = ( BytesLeft > MaxCtrlTrSize ) ? MaxCtrlTrSize : BytesLeft;

        Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB_I2C_READ, 
                                          (UInt16)DvcAddr, 0, DT_USB_DEVICE_TO_HOST,
                                          pBuf+BufIndex, TransferSize, &ActualTransferred,
                                          MAX_USB_REQ_TIMEOUT);

        // Transfer must be succesful
        if (!DT_SUCCESS(Status))
            break;

        // Must have transferred all
        if ( ActualTransferred != TransferSize )
        {
            Status = DT_STATUS_FAIL;
            break;
        }

        // update bytes left and index with buffer
        BytesLeft -= TransferSize;
        BufIndex += TransferSize;
    }
    
 
    // Release I2C mutex
    DtFastMutexRelease(&pDvcData->m_I2c.m_AccessMutex);

    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu1xxI2cWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuI2cWrite(
    DtuDeviceData*  pDvcData,   // Device object
    DtFileObject*  pFile,       // File object for access verification
    UInt  DvcAddr,              // I2C Device address
    UInt  Length,               // Number of BYTES to write
    UInt8*  pBuf)               // Buffer with data to be written to I2C bus
{
    DtStatus  Status = DT_STATUS_OK;
    void*  pFileHandle = NULL;
    UInt TransferSize, ActualTransferred, BufIndex;
    Int  BytesLeft = Length;    // Bytes left to transfer
    Int  MaxCtrlTrSize = DtUsbGetCtrlMaxPacketSize(&pDvcData->m_Device,
                                                          pDvcData->m_DevInfo.m_UsbSpeed);

    DtDbgOut(MAX, I2C, "DvcAddr: 0x%02X, Length: %d, pBuf: %p", DvcAddr, Length, pBuf);

    // Do we support the I2C interface
    if (!pDvcData->m_I2c.m_IsSupported)
    {
        DtDbgOut(ERR, I2C, "DTU-%d does not support the I2C interface",
                                                     pDvcData->m_DevInfo.m_TypeNumber);
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Sanity check: valid buffer size
    if (Length > DTU_I2C_MAX_BUF_SIZE)
        return DT_STATUS_INVALID_PARAMETER;

    // Nothing to transfer
    if (BytesLeft == 0)
        return DT_STATUS_OK;

    // Protect I2C access against concurrent access
    Status = DtFastMutexAcquire(&pDvcData->m_I2c.m_AccessMutex);
    if (!DT_SUCCESS(Status))
        return Status;

    // Check that I2C wasn't locked by someone else
    if (pFile != NULL)
        pFileHandle = DtFileGetHandle(pFile);
    if (pDvcData->m_I2c.m_pLockOwner!=NULL && pDvcData->m_I2c.m_pLockOwner!=pFileHandle)
    {
        // Release I2C mutex
        DtFastMutexRelease(&pDvcData->m_I2c.m_AccessMutex);
        return DT_STATUS_IN_USE;
    }
    
    BufIndex = 0;
    while (BytesLeft > 0)
    {
        // Do not transfer more than maximum allowed
        TransferSize = ( BytesLeft > MaxCtrlTrSize ) ? MaxCtrlTrSize : BytesLeft;

        Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB_I2C_WRITE, 
                                          (UInt16)DvcAddr, 0, DT_USB_HOST_TO_DEVICE, 
                                          pBuf+BufIndex, TransferSize, &ActualTransferred,
                                          MAX_USB_REQ_TIMEOUT);
        
        // Transfer must be succesful
        if (!DT_SUCCESS(Status))
            break;
        
        // Must have transferred all
        if (ActualTransferred != TransferSize)
        {
            DtDbgOut(ERR, I2C, "DTU-%d transfer size mismatch. Actual: %i, Needed: %i",
                                                         pDvcData->m_DevInfo.m_TypeNumber,
                                                         ActualTransferred, TransferSize);
            Status = DT_STATUS_FAIL;
            break;
        }
       
        // update bytes left and index with buffer
        BytesLeft -= TransferSize;
        BufIndex += TransferSize;
    }

    // Release I2C mutex
    DtFastMutexRelease(&pDvcData->m_I2c.m_AccessMutex);

    return Status;
}
