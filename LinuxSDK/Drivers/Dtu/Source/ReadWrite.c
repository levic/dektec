//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* ReadWrite.c *#*#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//
// Dtu driver - Writing/reading data to/from DTU device
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtuIncludes.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtuRead(
    DtuDeviceData*  pDvcData,
    Int  PortIndex,
    UInt8*  pBuf,
    Int  NumToRead,
    Int*  pNumRead)

{
    DtStatus  Status = DT_STATUS_OK;
    UInt8*  pTempBuf = NULL;
    Int  TempBufWrIndex = 0;
    Int  TempBufRdIndex = 0;
    Int  NonIpPortIndex;
    DtuNonIpPort*  pNonIpPort;
    Int  BytesLeft = NumToRead;
    Int  BufIndex = 0;
    Int  NumCopy = 0;

    // Get pipe for reading data
    Int  Pipe = pDvcData->m_EzUsb.m_ReadPipe;

    // Nothing read yet
    *pNumRead = 0;

    DtDbgOut(MAX, RDWR, "Entry: pBuf=%p, NumToRead=%d", pBuf, NumToRead);

    // Check if the PortIndex is from the NonIpPort
    Status = DtuGetNonIpPortIndex(pDvcData, PortIndex, &NonIpPortIndex);
    if (!DT_SUCCESS(Status))
        return Status;
    pNonIpPort = &pDvcData->m_NonIpPorts[NonIpPortIndex];


    // Simple checks
    if (NumToRead == 0)
        return DT_STATUS_OK;
    if (pBuf == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Get temporary buffer
    pTempBuf = pNonIpPort->m_pTempBuf;
    TempBufWrIndex = pNonIpPort->m_TempBufWrIndex;
    TempBufRdIndex = pNonIpPort->m_TempBufRdIndex;

    DtDbgOut(MAX, RDWR, "NumToRead=%d, BytesLeft:%d, TempLoad=%d", 
                                   NumToRead, BytesLeft, TempBufWrIndex - TempBufRdIndex);

    //-.-.-.-.-.-.-.-.-.-.- First check for data already pre-fetched -.-.-.-.-.-.-.-.-.-.-
    if (TempBufRdIndex > 0)
    {
        // Determine how much we can copy from temporary buffer
        Int  NumInTempBuf = TempBufWrIndex - TempBufRdIndex;
        NumCopy = (NumInTempBuf > NumToRead) ? NumToRead : NumInTempBuf;

        // Copy from temporary buffer to user buffer
        DtMemCopyToUserBuf(&pBuf[BufIndex], &pTempBuf[TempBufRdIndex], NumCopy);

        // Update counters
        TempBufRdIndex += NumCopy;
        BytesLeft -= NumCopy;
        BufIndex += NumCopy;
        *pNumRead += NumCopy;

        DtDbgOut(MAX, RDWR, "NumCopy=%d, BytesLeft:%d, TempLoad=%d", 
                                       NumCopy, BytesLeft, TempBufWrIndex-TempBufRdIndex);
    }

    // Temporary buffer empty?
    if (TempBufRdIndex==TempBufWrIndex)
    {
        // Reset indices, so we don't need a wraparround
        TempBufRdIndex = TempBufWrIndex = 0;
    }

    //.-.-.-.-.-.-.-.-.- Directly copy multiples of DTU_BULK_TRANSFER_ALIGN -.-.-.-.-.-.-.-.-.
    while (DT_SUCCESS(Status) && BytesLeft>=DTU_BULK_TRANSFER_ALIGN)
    {
        if (BytesLeft>=DTU_BULK_PACKETSIZE)
            // Copy DTU_BULK_PACKETSIZE bytes directly
            NumCopy = DTU_BULK_PACKETSIZE;
        else if (BytesLeft%DTU_BULK_TRANSFER_ALIGN == 0)
            // Bytes left is multiple of DTU_BULK_TRANSFER_ALIGN copy it directly
            NumCopy = BytesLeft;
        else
            // Copy the rest via temporary buffer
            break;

       // Read data
#ifdef LINBUILD
        // For Linux, we have to read from a temp. buffer. We can not read from
        // the user buffer directly
        Status = DtUsbPipeRead(&pDvcData->m_Device, NULL, Pipe, pTempBuf, NumCopy,
                                                                      MAX_USB_RW_TIMEOUT);
        DtMemCopyToUserBuf(&pBuf[BufIndex], pTempBuf, NumCopy);
#else
        Status = DtUsbPipeRead(&pDvcData->m_Device, NULL, Pipe, &pBuf[BufIndex], NumCopy,
                                                                      MAX_USB_RW_TIMEOUT);
#endif

        // Update counters
        BytesLeft -= NumCopy;
        BufIndex += NumCopy;
        *pNumRead += NumCopy;

        DtDbgOut(MAX, RDWR, "NumCopy=%d, BytesLeft:%d", NumCopy, BytesLeft);
    }

    //.-.-.-.-.-.-.-.-.- Copy the remaining bytes via temporary buffer  -.-.-.-.-.-.-.-.-.
    if (DT_SUCCESS(Status) && BytesLeft>0)
    {
        Int NumToTemp;

        DT_ASSERT(TempBufRdIndex==0 && TempBufWrIndex==0);

        // Copy a multiple of DTU_BULK_TRANSFER_ALIGN
        NumToTemp = ((BytesLeft + DTU_BULK_TRANSFER_ALIGN-1)/DTU_BULK_TRANSFER_ALIGN) * 
                                                                  DTU_BULK_TRANSFER_ALIGN;
        
        // Read data for temporary buffer
        Status = DtUsbPipeRead(&pDvcData->m_Device, NULL, Pipe, pTempBuf, NumToTemp, 
                                                                      MAX_USB_RW_TIMEOUT);
   
        // Copy remaining bytes from temporary buffer to user buffer
        NumCopy = BytesLeft;
        DtMemCopyToUserBuf(&pBuf[BufIndex], pTempBuf, NumCopy);

        // Update indices and counters
        TempBufWrIndex = NumToTemp;
        TempBufRdIndex = NumCopy;
        BytesLeft -= NumCopy;
        BufIndex += NumCopy;
        *pNumRead += NumCopy;

        DtDbgOut(MAX, RDWR, "NumCopy=%d, BytesLeft:%d", NumCopy, BytesLeft);
    }


    // Save temporary buffer indices
    pNonIpPort->m_TempBufWrIndex = TempBufWrIndex;
    pNonIpPort->m_TempBufRdIndex = TempBufRdIndex;

    DtDbgOut(MAX, RDWR, "Exit: pBuf[0]: %d, BytesLeft:%d, TempLoad=%d",pBuf[0], BytesLeft,
                                                           TempBufWrIndex-TempBufRdIndex);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuWrite(
    DtuDeviceData*  pDvcData,
    Int  PortIndex,
    UInt8*  pBuf,
    Int  NumToWrite)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8*  pTempBuf = NULL;
    Int  TempBufWrIndex = 0;
    Int  TempBufRdIndex = 0;
    Int  NonIpPortIndex;
    DtuNonIpPort*  pNonIpPort;
    Int  BytesLeft = NumToWrite;
    Int  BufIndex = 0;
    Int  NumCopy = 0;

    // Get pipe for writing data
    Int  Pipe = pDvcData->m_EzUsb.m_WritePipe;

    DtDbgOut(MAX, RDWR, "Entry: pBuf=%p, NumToWrite=%d", pBuf, NumToWrite);

    // Check if the PortIndex is from the NonIpPort
    Status = DtuGetNonIpPortIndex(pDvcData, PortIndex, &NonIpPortIndex);
    if (!DT_SUCCESS(Status))
        return Status;
    pNonIpPort = &pDvcData->m_NonIpPorts[NonIpPortIndex];


    // Simple checks
    if (NumToWrite == 0)
        return DT_STATUS_OK;
    if (pBuf == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Get temporary buffer
    pTempBuf = pNonIpPort->m_pTempBuf;
    TempBufWrIndex = pNonIpPort->m_TempBufWrIndex;
    TempBufRdIndex = pNonIpPort->m_TempBufRdIndex;

    DT_ASSERT(TempBufRdIndex==0);

    DtDbgOut(MAX, RDWR, "NumToWrite=%d, BytesLeft:%d, TempLoad=%d", 
                                                 NumToWrite, BytesLeft, TempBufWrIndex );

    //-.-.-.-.-.-.-.-.-.-.-.-.- Check for minimum transfer size -.-.-.-.-.-.-.-.-.-.-.-.-.

    // First transfer data from internal buffer
    if (TempBufWrIndex>0 && (TempBufWrIndex+NumToWrite)>=DTU_BULK_TRANSFER_ALIGN)
    {
        // Determine maximum to copy in TempBuf such that TempBuf is full or
        // filled with a multiple of DTU_BULK_TRANSFER_ALIGN bytes
        if ((TempBufWrIndex + NumToWrite) >= DTU_BULK_PACKETSIZE)
            NumCopy = DTU_BULK_PACKETSIZE - TempBufWrIndex;
        else
            NumCopy = NumToWrite - (TempBufWrIndex+NumToWrite)%DTU_BULK_TRANSFER_ALIGN;
        
        DtMemCopyFromUserBuf(&pTempBuf[TempBufWrIndex], &pBuf[BufIndex], NumCopy);

        DtDbgOut(MAX, RDWR, "NumCopy=%d, BytesLeft:%d, TempLoad=%d", 
                                                      NumCopy, BytesLeft, TempBufWrIndex);

        // Write data
        Status = DtUsbPipeWrite(&pDvcData->m_Device, NULL, Pipe, pTempBuf,
                                              TempBufWrIndex+NumCopy, MAX_USB_RW_TIMEOUT);
        // Temporary buffer is empty now
        TempBufWrIndex = 0;   
        TempBufRdIndex = 0;

        // Update counters
        BytesLeft -= NumCopy;
        BufIndex += NumCopy;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Directly copy multiples of 1024 -.-.-.-.-.-.-.-.-.-.-.-.-.
    while (DT_SUCCESS(Status) && BytesLeft>=DTU_BULK_TRANSFER_ALIGN)
    {
        if (BytesLeft>DTU_BULK_PACKETSIZE)
            NumCopy = DTU_BULK_PACKETSIZE;
        else 
            NumCopy = BytesLeft - (BytesLeft%DTU_BULK_TRANSFER_ALIGN);
    

        DtDbgOut(MAX, RDWR, "NumCopy=%d, BytesLeft:%d, TempLoad=%d", 
                                                      NumCopy, BytesLeft, TempBufWrIndex);
        // Write data
#ifdef LINBUILD
        // For Linux, we have to write to a temp. buffer. We can not write to
        // the user buffer directly
        DtMemCopyFromUserBuf(pTempBuf, &pBuf[BufIndex], NumCopy);
        Status = DtUsbPipeWrite(&pDvcData->m_Device, NULL, Pipe, pTempBuf, NumCopy,
                                                                      MAX_USB_RW_TIMEOUT);
#else
        Status = DtUsbPipeWrite(&pDvcData->m_Device, NULL, Pipe, &pBuf[BufIndex], NumCopy,
                                                                      MAX_USB_RW_TIMEOUT);
#endif
    
        BytesLeft -= NumCopy;
        BufIndex += NumCopy;
    }

    //-.-.-.-.-.-.-.-.-.-.- Copy remaining data to temporary buffer -.-.-.-.-.-.-.-.-.-.-.
    if (DT_SUCCESS(Status) && BytesLeft > 0 )
    {
        NumCopy = BytesLeft;
        DtDbgOut(MAX, RDWR, "NumCopy=%d, BytesLeft:%d, TempLoad=%d", 
                                                      NumCopy, BytesLeft, TempBufWrIndex);
        DtMemCopyFromUserBuf(&pTempBuf[TempBufWrIndex], &pBuf[BufIndex], NumCopy);

        // Update index and counters
        TempBufWrIndex += NumCopy;
        BytesLeft -= NumCopy;
        BufIndex += NumCopy;
    }

    // Save temporary buffer indices
    pNonIpPort->m_TempBufWrIndex = TempBufWrIndex;
    pNonIpPort->m_TempBufRdIndex = TempBufRdIndex;

    DtDbgOut(MAX, RDWR, "Exit: BytesLeft:%d, TempLoad=%d",  BytesLeft, TempBufWrIndex);

    return Status;
}
