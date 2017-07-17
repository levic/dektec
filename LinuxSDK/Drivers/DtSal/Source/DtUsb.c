//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtUsb.c *#*#*#*#*#*#*#*#*#* (C) 2011-2016 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2016 DekTec Digital Video B.V.
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

#ifdef LINBUILD
#if LINUX_VERSION_CODE<KERNEL_VERSION(2,6,34)
#define usb_alloc_coherent usb_buffer_alloc
#define usb_free_coherent usb_buffer_free
#endif
#endif

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
            DtDbgOut(ERR, SAL_USB, "Failed to read data from pipe (PipeNumber=%d)."
                                                                  " Timeout", PipeNumber);
            return DT_STATUS_TIMEOUT;
        }

        DtDbgOut(ERR, SAL_USB, "Failed to read data from pipe (PipeNumber=%d)", 
                                                                              PipeNumber);
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
            DtDbgOut(ERR, SAL_USB, "Failed to read data from pipe (PipeNumber=%d)."
                                                                  " Timeout", PipeNumber);
            return DT_STATUS_TIMEOUT;
        }
        DtDbgOut(ERR, SAL_USB, "Failed to read data from pipe (PipeNumber=%d). Error: %i",
                                                                      PipeNumber, Status);
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
            DtDbgOut(ERR, SAL_USB, "Failed to write data to pipe (PipeNumber=%d)."
                                                                  " Timeout", PipeNumber);
            return DT_STATUS_TIMEOUT;
        }
        DtDbgOut(ERR, SAL_USB, "Failed to write data to pipe (PipeNumber=%d)",
                                                                              PipeNumber);
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
            DtDbgOut(ERR, SAL_USB, "Failed to write data to pipe (PipeNumber=%d)."
                                                                  " Timeout", PipeNumber);
            return DT_STATUS_TIMEOUT;
        }
        DtDbgOut(ERR, SAL_USB, "Failed to write data to pipe (PipeNumber=%d). Error: %i",
                                                                      PipeNumber, Status);
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
        DtDbgOut(MAX, SAL_USB, "Code=0x%02X, Value=0x%04X, Index=0x%04X, Dir=%d,"
                             " Buf=%02X %02X", Code, Value, Index, Dir, pBuf[0], pBuf[1]);
    else
        DtDbgOut(MAX, SAL_USB, "Code=0x%02X, Value=0x%04X, Index=0x%04X, Dir=%d,"
                                         " Bufsize=%d", Code, Value, Index, Dir, BufSize);

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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuEvtAsyncComplete -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
#ifdef WINBUILD
// Windows specific
EVT_WDF_REQUEST_COMPLETION_ROUTINE  DtuEvtAsyncComplete;
void  DtuEvtAsyncComplete(
    WDFREQUEST  Request,
    WDFIOTARGET  Target,
    PWDF_REQUEST_COMPLETION_PARAMS  Params,
    void*  pContext)
{
    AsyncRequest* pAsyncRequest = (AsyncRequest*)pContext;
    UInt32  TransferLength;

    pAsyncRequest->m_Result = DT_STATUS_OK;
    
    // Check error status of URB
    if (Params->IoStatus.Status != STATUS_SUCCESS)
    {
        DtDbgOut(ERR, SAL_USB, "IoStatus: 0x%X", Params->IoStatus.Status);
        if (Params->IoStatus.Status == STATUS_IO_TIMEOUT)
            pAsyncRequest->m_Result = DT_STATUS_TIMEOUT;
        else
            pAsyncRequest->m_Result = DT_STATUS_FAIL;
    } else if (pAsyncRequest->m_pUrb->UrbHeader.Status != USBD_STATUS_SUCCESS)
    {
        DtDbgOut(ERR, SAL_USB, "UrbHeader.Status: 0x%X", 
                                                 pAsyncRequest->m_pUrb->UrbHeader.Status);
        pAsyncRequest->m_Result = DT_STATUS_FAIL;
    }
    TransferLength = 
                   pAsyncRequest->m_pUrb->UrbBulkOrInterruptTransfer.TransferBufferLength;
#else
// Linux specific
void  DtuEvtAsyncComplete(struct urb* pUrb)
{
    void*  pContext = pUrb->context;
    AsyncRequest*  pAsyncRequest = (AsyncRequest*)pContext;
    UInt32  TransferLength;
    pAsyncRequest->m_Result = DT_STATUS_OK;

    if (pUrb->status != 0)
    {
        DtDbgOut(ERR, SAL_USB, "pUrb->status: 0x%X", pUrb->status);
        
        if (pUrb->status == -ETIMEDOUT)
            pAsyncRequest->m_Result = DT_STATUS_TIMEOUT;
        else
            pAsyncRequest->m_Result = DT_STATUS_FAIL;
    }
    if (usb_pipein(pUrb->pipe) != 0)
    {
        // copy data back to user buffer
        memcpy(pAsyncRequest->m_pBuf, pUrb->transfer_buffer, pUrb->actual_length);
    }
    TransferLength = pUrb->actual_length;
#endif
    // Execute device specific code if needed
    if (pAsyncRequest->m_pEvtAsyncComplete != NULL)
        pAsyncRequest->m_pEvtAsyncComplete(pAsyncRequest, TransferLength);
    // Set done event to signall finished state
    DtEventSet(&pAsyncRequest->m_EvtRequestDone);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuCleanupAsyncRequest -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtuCleanupAsyncRequest(AsyncRequest* pReq, Bool Wait)
{
    if (!pReq->m_IsValid)
        return;
#ifdef WINBUILD
    //WdfRequestCancelSentRequest(Req->m_WdfRequest);
    // Wait until request is either cancelled or completed.
    if (Wait)
        DtEventWait(&pReq->m_EvtRequestDone, -1);
    WdfObjectDelete(pReq->m_WdfRequest);
    WdfObjectDelete(pReq->m_UrbMemory);
#else
    if (pReq->m_pUrb == NULL)
        return;
    // Wait until request is either cancelled or completed.
    if (Wait)
        usb_kill_urb(pReq->m_pUrb);
    usb_free_coherent(pReq->m_pUrb->dev, pReq->m_UrbBufLen,
                               pReq->m_pUrb->transfer_buffer, pReq->m_pUrb->transfer_dma);
    usb_free_urb(pReq->m_pUrb);
#endif
    pReq->m_pUrb = NULL;
    pReq->m_IsValid = FALSE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuAllocateAsyncRequests -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
AsyncRequest*  DtuAllocateAsyncRequests(Int NumAsyncRequests, Int ContextSize)
{
    Int  i;
    AsyncRequest*  pRequests;
    pRequests = (AsyncRequest*)DtMemAllocPool(DtPoolNonPaged, 
                          (ContextSize+sizeof(AsyncRequest)) * NumAsyncRequests, SAL_TAG);
    if (pRequests == NULL)
    {
        DtDbgOut(ERR, SAL_USB, "Failed to allocate async request buffer");
        return NULL;
    }

    // Default-initialize all request, so we can clean up properly later.
    for (i=0; i<NumAsyncRequests; i++)
    {
        pRequests[i].m_pUrb = NULL;
        pRequests[i].m_IsValid = FALSE;

        // Set DTU-xxx context specific at end of default request buffer
        pRequests[i].m_pContext = ((char*)(&pRequests[NumAsyncRequests]) + i*ContextSize);
    }
    return pRequests;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuFreeAsyncRequests -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtuFreeAsyncRequests(Int NumAsyncRequests, AsyncRequest* pRequests)
{
    Int  i;
    DtDbgOut(AVG, SAL_USB, "DtuFreeAsyncRequests start");
    
    for (i=0; i<NumAsyncRequests; i++)
        DtuCleanupAsyncRequest(&pRequests[i], TRUE);

    DtMemFreePool(pRequests, SAL_TAG);
    DtDbgOut(AVG, SAL_USB, "DtuFreeAsyncRequests done");
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuInitAsyncRequest -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtuInitAsyncRequest(AsyncRequest* pReq, Int PipeNumber, DtDvcObject* pUsbDevice, 
                   Int Dir, void* pBuf, Int BufLen, pDtUsbAsyncComplete pEvtAsyncComplete)
{
#ifdef WINBUILD
    WDFUSBPIPE  Pipe;
    USBD_PIPE_HANDLE  wdmhUSBPipe;
    WDFIOTARGET  IoTarget;
    Int  UrbSize;
    NTSTATUS  NtStatus;
#else
    UInt8*  pUrbMem = NULL;
    UInt  Pipe;
#endif

    if (!DT_SUCCESS(DtEventInit(&pReq->m_EvtRequestDone, TRUE)))
    {
        DtDbgOut(ERR, SAL_USB, "DtEventInit failed");
        return FALSE;
    }
    pReq->m_pEvtAsyncComplete = pEvtAsyncComplete;
    
#ifdef WINBUILD
    Pipe = WdfUsbInterfaceGetConfiguredPipe(pUsbDevice->m_UsbInterface, 0, NULL);
    wdmhUSBPipe = WdfUsbTargetPipeWdmGetPipeHandle(Pipe);
    IoTarget = WdfUsbTargetDeviceGetIoTarget(pUsbDevice->m_UsbDevice);
    UrbSize = sizeof(struct _URB_BULK_OR_INTERRUPT_TRANSFER);
    NtStatus = WdfMemoryCreate(WDF_NO_OBJECT_ATTRIBUTES, NonPagedPool, SAL_TAG,
                                       UrbSize, &pReq->m_UrbMemory, (void**)&pReq->m_pUrb);
    if (!NT_SUCCESS(NtStatus))
    {
        DtDbgOut(ERR, SAL_USB, "WdfMemoryCreate failed");
        pReq->m_pUrb = NULL;
        return FALSE;
    }
    NtStatus = WdfRequestCreate(WDF_NO_OBJECT_ATTRIBUTES, IoTarget, &pReq->m_WdfRequest);
    if (!NT_SUCCESS(NtStatus))
    {
        DtDbgOut(ERR, SAL_USB, "WdfRequestCreate failed");
        WdfObjectDelete(pReq->m_UrbMemory);
        pReq->m_pUrb = NULL;
        return FALSE;
    }
    
    RtlZeroMemory(pReq->m_pUrb, UrbSize);
    pReq->m_pUrb->UrbBulkOrInterruptTransfer.Hdr.Length = (USHORT)UrbSize;
    pReq->m_pUrb->UrbBulkOrInterruptTransfer.Hdr.Function = 
                                                  URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER;
    pReq->m_pUrb->UrbBulkOrInterruptTransfer.PipeHandle = wdmhUSBPipe;
    if (Dir == DT_USB_DEVICE_TO_HOST)
        pReq->m_pUrb->UrbBulkOrInterruptTransfer.TransferFlags = 
                                      USBD_TRANSFER_DIRECTION_IN | USBD_SHORT_TRANSFER_OK;
    else 
        pReq->m_pUrb->UrbBulkOrInterruptTransfer.TransferFlags = 
                                                              USBD_TRANSFER_DIRECTION_OUT;

    NtStatus = WdfUsbTargetPipeFormatRequestForUrb(Pipe, pReq->m_WdfRequest,
                                                                 pReq->m_UrbMemory, NULL);
    if (!NT_SUCCESS(NtStatus))
    {
        DtDbgOut(ERR, SAL_USB, "WdfUsbTargetPipeFormatRequestForUrb failed");
        WdfObjectDelete(pReq->m_WdfRequest);
        WdfObjectDelete(pReq->m_UrbMemory);
        pReq->m_pUrb = NULL;
        NtStatus = STATUS_UNSUCCESSFUL;
        return FALSE;
    }
    WdfRequestSetCompletionRoutine(pReq->m_WdfRequest, DtuEvtAsyncComplete, pReq);
#else
    pReq->m_pBuf = pBuf;
    pReq->m_UrbBufLen = BufLen;
    pReq->m_pUrb = usb_alloc_urb(0, GFP_ATOMIC);
    if (pReq->m_pUrb == NULL)
    {
        DtDbgOut(ERR, SAL_USB, "Failed to allocate urb");
        return FALSE;
    }
    pUrbMem = usb_alloc_coherent(pUsbDevice->m_pUsbDev, BufLen, GFP_KERNEL, 
                                                             &pReq->m_pUrb->transfer_dma);
    if (pUrbMem == NULL)
    {
        DtDbgOut(ERR, SAL_USB, "Failed to allocate memory for urb");
        usb_free_urb(pReq->m_pUrb);
        pReq->m_pUrb = NULL;
        return FALSE;
    }
    if (Dir == DT_USB_DEVICE_TO_HOST)
        Pipe =  usb_rcvbulkpipe(pUsbDevice->m_pUsbDev, PipeNumber);
    else
        Pipe = usb_sndbulkpipe(pUsbDevice->m_pUsbDev, PipeNumber);
    
    usb_fill_bulk_urb(pReq->m_pUrb, pUsbDevice->m_pUsbDev, Pipe, pUrbMem, BufLen, 
                                                               DtuEvtAsyncComplete, pReq);
    if (pBuf!=NULL && Dir==DT_USB_DEVICE_TO_HOST)
        memcpy(pReq->m_pUrb->transfer_buffer, pBuf, BufLen);

    pReq->m_pUrb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
#endif
    // If URP is to device, we init the request done to a signalled state.
    if (Dir == DT_USB_HOST_TO_DEVICE)
        DtEventSet(&pReq->m_EvtRequestDone);
    pReq->m_IsValid = TRUE;
    return TRUE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuReInitAsyncRequest -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtuReInitAsyncRequest(AsyncRequest* pReq, Int PipeNumber, DtDvcObject* pUsbDevice, 
                                                          Int Dir, void* pBuf, Int BufLen)
{
#ifdef WINBUILD
    WDFUSBPIPE  Pipe;
    USBD_PIPE_HANDLE  wdmhUSBPipe;
    Int  UrbSize;
    NTSTATUS  NtStatus;
    WDF_REQUEST_REUSE_PARAMS  ReusePars;

    Pipe = WdfUsbInterfaceGetConfiguredPipe(pUsbDevice->m_UsbInterface, 0, NULL);
    wdmhUSBPipe = WdfUsbTargetPipeWdmGetPipeHandle(Pipe);
    UrbSize = sizeof(struct _URB_BULK_OR_INTERRUPT_TRANSFER);

    WDF_REQUEST_REUSE_PARAMS_INIT(&ReusePars, WDF_REQUEST_REUSE_NO_FLAGS, STATUS_SUCCESS);
    NtStatus = WdfRequestReuse(pReq->m_WdfRequest, &ReusePars);
    if (!NT_SUCCESS(NtStatus))
    {
        DtDbgOut(ERR, SAL_USB, "WdfRequestReuse failed");
        return FALSE;
    }

    pReq->m_pUrb->UrbBulkOrInterruptTransfer.Hdr.Length = (USHORT)UrbSize;
    pReq->m_pUrb->UrbBulkOrInterruptTransfer.Hdr.Function = 
                                                  URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER;
    pReq->m_pUrb->UrbBulkOrInterruptTransfer.PipeHandle = wdmhUSBPipe;
    if (Dir == DT_USB_DEVICE_TO_HOST)
        pReq->m_pUrb->UrbBulkOrInterruptTransfer.TransferFlags = 
                                      USBD_TRANSFER_DIRECTION_IN | USBD_SHORT_TRANSFER_OK;
    else 
        pReq->m_pUrb->UrbBulkOrInterruptTransfer.TransferFlags = 
                                                              USBD_TRANSFER_DIRECTION_OUT;
    pReq->m_pUrb->UrbBulkOrInterruptTransfer.TransferBuffer = pBuf;
    pReq->m_pUrb->UrbBulkOrInterruptTransfer.TransferBufferLength = BufLen;

    NtStatus = WdfUsbTargetPipeFormatRequestForUrb(Pipe, pReq->m_WdfRequest,
                                                                 pReq->m_UrbMemory, NULL);
    if (!NT_SUCCESS(NtStatus))
    {
        DtDbgOut(ERR, SAL_USB, "WdfUsbTargetPipeFormatRequestForUrb failed");
        return FALSE;
    }
    WdfRequestSetCompletionRoutine(pReq->m_WdfRequest, DtuEvtAsyncComplete, pReq);

#else
    Int Pipe;
    pReq->m_pBuf = pBuf;
    if (pBuf!=NULL && Dir==DT_USB_HOST_TO_DEVICE)
    {
        if (Dir == DT_USB_DEVICE_TO_HOST)
            Pipe =  usb_rcvbulkpipe(pUsbDevice->m_pUsbDev, PipeNumber);
        else
            Pipe = usb_sndbulkpipe(pUsbDevice->m_pUsbDev, PipeNumber);
    
        usb_fill_bulk_urb(pReq->m_pUrb, pUsbDevice->m_pUsbDev, Pipe, 
                        pReq->m_pUrb->transfer_buffer, BufLen, DtuEvtAsyncComplete, pReq);
        memcpy(pReq->m_pUrb->transfer_buffer, pBuf, BufLen);
    }
#endif
    return TRUE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuSendAsyncRequest -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// If DtuSendAsyncRequest failes, the request object is cleaned up because it is a fatal
// error. If this behaviour is not desired in all cases, the cleanup of the request
// object must be moved after a failed call of this function in some cases and other
// actions (e.g. activating the EventDoneRequest object again) in other cases 
//
DtStatus  DtuSendAsyncRequest(AsyncRequest* pReq, DtDvcObject* pUsbDevice, Int Timeout)
{
#ifdef WINBUILD
    WDF_REQUEST_SEND_OPTIONS  SendOptions;
    WDF_REQUEST_SEND_OPTIONS*  pSendOptions = NULL;
    WDFIOTARGET  IoTarget = WdfUsbTargetDeviceGetIoTarget(pUsbDevice->m_UsbDevice);
    
    if (Timeout != -1)
    {
        // Initialize send options used for URB timeouts
        WDF_REQUEST_SEND_OPTIONS_INIT(&SendOptions, WDF_REQUEST_SEND_OPTION_TIMEOUT);
        WDF_REQUEST_SEND_OPTIONS_SET_TIMEOUT(&SendOptions, -10*1000*Timeout);
        pSendOptions = &SendOptions;
    }
    if (WdfRequestSend(pReq->m_WdfRequest, IoTarget, pSendOptions) == FALSE)
    {
        DtDbgOut(ERR, SAL_USB, "WdfRequestSend failed.");
        DtuCleanupAsyncRequest(pReq, FALSE);
        return DT_STATUS_FAIL;
    }
#else
    // TODO: TIMEOUT?
    int res = usb_submit_urb(pReq->m_pUrb, GFP_ATOMIC);
    if (res != 0)
    {
        DtDbgOut(ERR, SAL_USB, "usb_submit_urb failed. Res = %d", res);
        DtuCleanupAsyncRequest(pReq, FALSE);
        return DT_STATUS_FAIL;
    }
#endif
    return DT_STATUS_OK;
}

#endif // SKIP_USB
