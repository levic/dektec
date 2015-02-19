//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtUsb.c *#*#*#*#*#*#*#*#*#* (C) 2011-2015 DekTec
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
#include "DtUsb.h"

#ifndef SKIP_USB

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
#define USB_ENDPOINT_ADDRESS_MASK    0xF

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtUsbQueuryString -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtUsbQueuryString(
    DtDvcObject*  pUsbDevice,
    DtString*  pDtString,
    UInt  StringIndex)
{
    Int  StringSize = DtStringGetMaxStringLength(pDtString);
    
#ifdef WINBUILD
    // Windows implementation
    NTSTATUS  NtStatus;
    // In: StringSize = Unicode length
    NtStatus = WdfUsbTargetDeviceQueryString(pUsbDevice->m_UsbDevice, NULL, NULL,
                                                  pDtString->Buffer, (PUSHORT)&StringSize,
                                                  (UCHAR)StringIndex, 0x0409);
    if (NT_SUCCESS(NtStatus))
    {
        // Out: StringSize = char length
        pDtString->Length = (USHORT)StringSize * sizeof(DtStringChar);
        return DT_STATUS_OK;
    }
    return DT_STATUS_FAIL;
#else
    // Linux implementation
    Int  Status;
    Status = usb_get_descriptor(pUsbDevice->m_pUsbDev, USB_DT_STRING, StringIndex,
                                                         pDtString->m_Buffer, StringSize);
    if (Status < 0)
        return DT_STATUS_FAIL;
    pDtString->m_Length = Status;
    return DT_STATUS_OK;
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtUsbPipeRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtUsbPipeRead(
    DtDvcObject*  pUsbDevice, 
    void*  pRequestObject,
    Int  PipeNumber,
    UInt8*  pBuf,
    Int  NumToRead,
    Int  Timeout)           // Timeout in ms, -1 = No timeout
{
#ifdef WINBUILD
    // Windows variables
    NTSTATUS  NtStatus;
    WDFUSBPIPE  Pipe=NULL;
    WDF_USB_PIPE_INFORMATION  PipeInfo;
    WDF_MEMORY_DESCRIPTOR  MemDesc;
    WDF_REQUEST_SEND_OPTIONS  Options;
    WDF_REQUEST_SEND_OPTIONS*  pOptions = NULL;
     
#else
    // Linux variables
    Int  Status;
    Int  NumBytesTransfered;
    Int  TimeoutJiffies = 0;
    Int  Pipe;
#endif

    DtDbgOut(MAX, SAL_USB, "PipeNumber=%d, pBuf=%p, NumToRead=%d", PipeNumber, pBuf, 
                                                                               NumToRead);
    // Nothing to transfer
    if (NumToRead == 0)
        return DT_STATUS_OK;
    // Must have valid buffer
    if (pBuf == NULL)
        return DT_STATUS_INVALID_PARAMETER;

#ifdef WINBUILD
    // Windows implementation

    // Get pipe object
    WDF_USB_PIPE_INFORMATION_INIT(&PipeInfo);

    Pipe = WdfUsbInterfaceGetConfiguredPipe(pUsbDevice->m_UsbInterface, (UInt8)PipeNumber, 
                                                                               &PipeInfo);
    if (Pipe == NULL)
    {
        DtDbgOut(ERR, SAL_USB, "Failed to get pipe (PipeNumber=%d)", PipeNumber);
        return DT_STATUS_FAIL;
    }

    if (Timeout != -1)
    {
        pOptions = &Options;
        WDF_REQUEST_SEND_OPTIONS_INIT(pOptions, 0);
        WDF_REQUEST_SEND_OPTIONS_SET_TIMEOUT(pOptions, WDF_REL_TIMEOUT_IN_MS(Timeout));
    }

    // Read data from pipe
    WDF_MEMORY_DESCRIPTOR_INIT_BUFFER(&MemDesc, (PVOID)pBuf, NumToRead);
    NtStatus = WdfUsbTargetPipeReadSynchronously(Pipe, (WDFREQUEST)pRequestObject,
                                                                pOptions, &MemDesc, NULL);
    if (!NT_SUCCESS(NtStatus))
    {
        if (NtStatus == STATUS_IO_TIMEOUT)
        {
            DtDbgOut(ERR, SAL_USB, "Failed to read data from pipe (PipeNumber=%d). Timeout", 
                                                                               PipeNumber);
            return DT_STATUS_TIMEOUT;
        }

        DtDbgOut(ERR, SAL_USB, "Failed to read data from pipe (PipeNumber=%d)", PipeNumber);
        return DT_STATUS_FAIL;
    }
#else
    // Linux implementation
    if (Timeout != -1) 
        TimeoutJiffies = msecs_to_jiffies(Timeout);

    Pipe = usb_rcvbulkpipe(pUsbDevice->m_pUsbDev, PipeNumber);
    
    Status = usb_bulk_msg(pUsbDevice->m_pUsbDev, Pipe, pBuf,  NumToRead, 
                                                     &NumBytesTransfered, TimeoutJiffies);
    if (Status < 0)
    {
        if (Status == -ETIMEDOUT)
        {
            DtDbgOut(ERR, SAL_USB, "Failed to read data from pipe (PipeNumber=%d). Timeout", 
                                                                               PipeNumber);
            return DT_STATUS_TIMEOUT;
        }
        DtDbgOut(ERR, SAL_USB, "Failed to read data from pipe (PipeNumber=%d). Error: %i", PipeNumber, Status);
        return DT_STATUS_FAIL;
    }
#endif
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtUsbPipeWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtUsbPipeWrite(
    DtDvcObject*  pUsbDevice,
    void*  pRequestObject,
    Int  PipeNumber,
    UInt8*  pBuf,
    Int  NumToWrite,
    Int  Timeout)           // Timeout in ms, -1 = No timeout
{
#ifdef WINBUILD
    // Windows variables
    NTSTATUS  NtStatus;
    WDFUSBPIPE  Pipe=NULL;
    WDF_USB_PIPE_INFORMATION  PipeInfo;
    WDF_MEMORY_DESCRIPTOR  MemDesc;
    WDF_REQUEST_SEND_OPTIONS  Options;
    WDF_REQUEST_SEND_OPTIONS*  pOptions = NULL;
#else
    // Linux variables
    Int  Status;
    Int  NumBytesTransfered;
    Int  TimeoutJiffies = 0;
    Int  Pipe;
#endif

    DtDbgOut(MAX, SAL_USB, "PipeNumber=%d, pBuf=%p, NumToWrite=%d", PipeNumber, pBuf, 
                                                                              NumToWrite);
    // Nothing to transfer
    if (NumToWrite == 0)
        return DT_STATUS_OK;
    // Must have valid buffer
    if (pBuf == NULL)
        return DT_STATUS_INVALID_PARAMETER;

#ifdef WINBUILD
    // Windows implementation

    // Get pipe object
    WDF_USB_PIPE_INFORMATION_INIT(&PipeInfo);
    Pipe = WdfUsbInterfaceGetConfiguredPipe(pUsbDevice->m_UsbInterface, (UInt8)PipeNumber,
                                                                               &PipeInfo);
    if (Pipe == NULL)
    {
        DtDbgOut(ERR, SAL_USB, "Failed to get pipe (PipeNumber=%d).", PipeNumber);
        return DT_STATUS_FAIL;
    }

    if (Timeout != -1)
    {
        pOptions = &Options;
        WDF_REQUEST_SEND_OPTIONS_INIT(pOptions, 0);
        WDF_REQUEST_SEND_OPTIONS_SET_TIMEOUT(pOptions, WDF_REL_TIMEOUT_IN_MS(Timeout));
    }

    // Write data to pipe
    WDF_MEMORY_DESCRIPTOR_INIT_BUFFER(&MemDesc, (PVOID)pBuf, NumToWrite);
    NtStatus = WdfUsbTargetPipeWriteSynchronously(Pipe, (WDFREQUEST)pRequestObject,
                                                                pOptions, &MemDesc, NULL);
    if (!NT_SUCCESS(NtStatus))
    {
        if (NtStatus == STATUS_IO_TIMEOUT)
        {
            DtDbgOut(ERR, SAL_USB, "Failed to write data to pipe (PipeNumber=%d). Timeout", 
                                                                               PipeNumber);
            return DT_STATUS_TIMEOUT;
        }
        DtDbgOut(ERR, SAL_USB, "Failed to write data to pipe (PipeNumber=%d)", PipeNumber);
        return DT_STATUS_FAIL;
    }
#else
    // Linux implementation
    if (Timeout != -1) 
        TimeoutJiffies = msecs_to_jiffies(Timeout);

    Pipe = usb_sndbulkpipe(pUsbDevice->m_pUsbDev, PipeNumber);

    Status = usb_bulk_msg(pUsbDevice->m_pUsbDev, Pipe, pBuf,  NumToWrite, 
                                                     &NumBytesTransfered, TimeoutJiffies);
    if (Status < 0)
    {
        if (Status == -ETIMEDOUT)
        {
            DtDbgOut(ERR, SAL_USB, "Failed to write data to pipe (PipeNumber=%d). Timeout", 
                                                                               PipeNumber);
            return DT_STATUS_TIMEOUT;
        }
        DtDbgOut(ERR, SAL_USB, "Failed to write data to pipe (PipeNumber=%d). Error: %i", PipeNumber, Status);
        return DT_STATUS_FAIL;
    }
#endif
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtUsbVendorRequest -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtUsbVendorRequest(
    DtDvcObject*  pUsbDevice, 
    void*  pRequestObject,
    UInt8  Code,
    UInt16  Value,
    UInt16  Index, 
    Int  Dir,
    UInt8*  pBuf,
    Int  BufSize,
    Int*  pNumBytesTransferred,
    Int  Timeout)           // Timeout in ms, -1 = No timeout
{
#ifdef WINBUILD
    // Windows variables
    NTSTATUS  NtStatus;
    WDF_USB_CONTROL_SETUP_PACKET  CtrlSetupPacket;
    WDF_MEMORY_DESCRIPTOR  MemDesc;
    UInt  BytesTransferred = 0;
    WDF_REQUEST_SEND_OPTIONS  Options;
    WDF_REQUEST_SEND_OPTIONS*  pOptions = NULL;
#else
    // Linux variables
    Int  RequestType;
    Int  Pipe;
    Int  Status;
    Int  TimeoutJiffies = 0;
    UInt8*  pTmpBuf = NULL;
#endif

    if (BufSize == 2)
        DtDbgOut(MAX, SAL_USB, "Code=0x%02X, Value=0x%04X, Index=0x%04X, Dir=%d, Buf=%02X %02X",
                                               Code, Value, Index, Dir, pBuf[0], pBuf[1]);
    else
        DtDbgOut(MAX, SAL_USB, "Code=0x%02X, Value=0x%04X, Index=0x%04X, Dir=%d, Bufsize=%d",
                                                        Code, Value, Index, Dir, BufSize);

    if (Dir!=DT_USB_HOST_TO_DEVICE && Dir!=DT_USB_DEVICE_TO_HOST)
        return DT_STATUS_INVALID_PARAMETER;
    if (BufSize!=0 && pBuf==NULL)
        return DT_STATUS_INVALID_PARAMETER;

#ifdef WINBUILD
    // Windows implementation

    WDF_USB_CONTROL_SETUP_PACKET_INIT_VENDOR(&CtrlSetupPacket,
                                            (Dir==DT_USB_HOST_TO_DEVICE) ? 
                                             BmRequestHostToDevice: BmRequestDeviceToHost,
                                                   BmRequestToDevice, Code, Value, Index);

    WDF_MEMORY_DESCRIPTOR_INIT_BUFFER(&MemDesc, (PVOID)pBuf, BufSize);

    if (Timeout != -1)
    {
        pOptions = &Options;
        WDF_REQUEST_SEND_OPTIONS_INIT(pOptions, 0);
        WDF_REQUEST_SEND_OPTIONS_SET_TIMEOUT(pOptions, WDF_REL_TIMEOUT_IN_MS(Timeout));
    }

    NtStatus = WdfUsbTargetDeviceSendControlTransferSynchronously(
                                 pUsbDevice->m_UsbDevice, (WDFREQUEST)pRequestObject,
                                 pOptions, &CtrlSetupPacket, &MemDesc, &BytesTransferred);
    if (!NT_SUCCESS(NtStatus))
    {
        if (NtStatus == STATUS_IO_TIMEOUT)
        {
            DtDbgOut(ERR, SAL_USB, "Vendor request (0x%02X) failed. Timeout", Code);
            return DT_STATUS_TIMEOUT;
        }
        DtDbgOut(ERR, SAL_USB, "Vendor request (0x%02X) failed", Code);
        return DT_STATUS_FAIL;
    }
    if (pNumBytesTransferred != NULL)
        *pNumBytesTransferred = (Int)BytesTransferred;
#else
    // Linux implementation
    if (Dir == DT_USB_HOST_TO_DEVICE)
    {
        RequestType = USB_TYPE_VENDOR | USB_DIR_OUT | USB_RECIP_DEVICE;
        Pipe = usb_sndctrlpipe(pUsbDevice->m_pUsbDev, 0);
    } else {
        RequestType = USB_TYPE_VENDOR | USB_DIR_IN | USB_RECIP_DEVICE;
        Pipe = usb_rcvctrlpipe(pUsbDevice->m_pUsbDev, 0);
    }

    if (Timeout != -1) 
        TimeoutJiffies = msecs_to_jiffies(Timeout);

    // The buffer used to transfer the control message must be a DMA-aware buffer and
    // can't be a static allocated array. Also using the stack as buffer is not
    // preferable because of cache invalidating issues.
    // We create a temporary buffer for the data to transfer
    if (BufSize != 0)
    {
        pTmpBuf = kmalloc(BufSize, GFP_KERNEL);
        if (pTmpBuf == NULL)
            return DT_STATUS_OUT_OF_MEMORY;
    
        if (Dir == DT_USB_HOST_TO_DEVICE)
            memcpy(pTmpBuf, pBuf, BufSize);
    }
    
    Status = usb_control_msg(pUsbDevice->m_pUsbDev, Pipe, Code, RequestType, Value,
                                                    Index, pTmpBuf, BufSize, TimeoutJiffies);
    if (pTmpBuf != NULL) 
    {
        if (Dir==DT_USB_DEVICE_TO_HOST && Status>0)
            memcpy(pBuf, pTmpBuf, BufSize);

        kfree(pTmpBuf);
    }

    if (Status < 0)
    {
        if (Status == -ETIMEDOUT)
        {
            DtDbgOut(ERR, SAL_USB, "Vendor request (0x%02X) failed. Timeout", Code);
            return DT_STATUS_TIMEOUT;
        }
        DtDbgOut(ERR, SAL_USB, "Vendor request (0x%02X) failed. Error: %i", Code, Status);
        return DT_STATUS_FAIL;
    } else if (pNumBytesTransferred != NULL)
        *pNumBytesTransferred = BufSize;
#endif
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtUsbGetBulkPipeNumber -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns the number of the pipe used for the (bulk) endpoint
// < 0 is returned if the pipe is not found.
//
Int  DtUsbGetBulkPipeNumber(
    DtDvcObject*  pUsbDevice,
    Int  Dir,
    Int  EndPoint)
{
#ifdef WINBUILD
    // Windows implementation
    Int  i;
    WDF_USB_PIPE_INFORMATION  PipeInfo;

    for (i=0; i<WdfUsbInterfaceGetNumConfiguredPipes(pUsbDevice->m_UsbInterface); i++)
    {
        WDF_USB_PIPE_INFORMATION_INIT(&PipeInfo);

        if (WdfUsbInterfaceGetConfiguredPipe(pUsbDevice->m_UsbInterface, (UInt8)i, 
                                                                       &PipeInfo) == NULL)
        {
            DT_ASSERT(FALSE);
        } else {
            // EndpointAddress includes the direction (bit 8) + reserved fields.
            // Only the lowest 4 bits are used for the EndpointAddress
            if ((PipeInfo.EndpointAddress&USB_ENDPOINT_ADDRESS_MASK) == EndPoint)
            {
                DT_ASSERT((Dir==DT_USB_DEVICE_TO_HOST && 
                            WDF_USB_PIPE_DIRECTION_IN((UInt8)PipeInfo.EndpointAddress))
                            || (Dir==DT_USB_HOST_TO_DEVICE &&
                            WDF_USB_PIPE_DIRECTION_OUT((UInt8)PipeInfo.EndpointAddress)));
                DT_ASSERT(PipeInfo.PipeType == WdfUsbPipeTypeBulk);
                return i;
            }
        }
    }
    DtDbgOut(ERR, SAL_USB, "Pipe not found for endpoint %i", EndPoint);
    return -1;
    
#else
    // Linux implementation
    // For Linux, we return the endpoint number. We must create the pipe for each transfer
    return EndPoint;
#endif
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtUsbDumpPipeInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtUsbDumpPipeInfo(DtDvcObject* pUsbDevice)
{
#ifdef WINBUILD
    // Windows implementation
    Int  i;
    WDF_USB_PIPE_INFORMATION  PipeInfo;

    DtDbgOut(ERR, SAL_USB, "Number of pipes:%i", 
                        WdfUsbInterfaceGetNumConfiguredPipes(pUsbDevice->m_UsbInterface));
    
    for (i=0; i<WdfUsbInterfaceGetNumConfiguredPipes(pUsbDevice->m_UsbInterface); i++)
    {
        WDF_USB_PIPE_INFORMATION_INIT(&PipeInfo);

        if (WdfUsbInterfaceGetConfiguredPipe(pUsbDevice->m_UsbInterface, (UInt8)i, 
                                                                       &PipeInfo) == NULL)
        {
            DT_ASSERT(FALSE);
        } else {
            DtDbgOut(ERR, SAL_USB, "PipeNumber:%i: Endpoint:%i(%xh) Type:%i Dir: '%s'", i, 
                   PipeInfo.EndpointAddress &USB_ENDPOINT_ADDRESS_MASK, 
                   PipeInfo.EndpointAddress, PipeInfo.PipeType,
                   (WDF_USB_PIPE_DIRECTION_OUT((UInt8)PipeInfo.EndpointAddress)?"OUT":
                   WDF_USB_PIPE_DIRECTION_IN((UInt8)PipeInfo.EndpointAddress)?"IN":"??"));
        }
    }
#else
    // Linux implementation
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtUsbManufNameEq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtUsbManufNameEq(DtDvcObject*  pDvc, const char*  pManufName)
{
#ifdef WINBUILD
    DtString  ManufName;
    DtStringChar StringBufManufName[32];
    DtString  DtStr;
    DtStringChar  DtStrBuffer[32];
    DtStatus  Status;

    DT_STRING_INIT(ManufName, StringBufManufName, 32);
    DtStringAppendChars(&ManufName, pManufName);

    DT_STRING_INIT(DtStr, DtStrBuffer, 32);

    Status = DtUsbQueuryString(pDvc, &DtStr, pDvc->m_UsbDevDesc.iManufacturer);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, SAL_USB, "Failed to get manufacturer string");
        return FALSE;
    }

    return DtStringCompare(&DtStr, &ManufName);
#else
    if (pDvc->m_pUsbDev->manufacturer == NULL)
        return FALSE;
    return strcmp(pManufName, pDvc->m_pUsbDev->manufacturer) == 0;
#endif
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuGetCtrlMaxPacketSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtUsbGetCtrlMaxPacketSize(DtDvcObject* pUsbDevice, Int  UsbSpeed)
{
    
#ifdef WINBUILD
    Int  MaxPacketSize = pUsbDevice->m_UsbDevDesc.bMaxPacketSize0;
#else
    Int  MaxPacketSize = pUsbDevice->m_pUsbDev->descriptor.bMaxPacketSize0;
#endif
    // For USB1 and USB2 the MaxPacketSize field is the maximum packet size.
    if (UsbSpeed < 2)
        return MaxPacketSize;
    // For USB3 the MaxPacketSize field is a power of 2.
    return (1 << MaxPacketSize);
}

#endif // SKIP_USB
