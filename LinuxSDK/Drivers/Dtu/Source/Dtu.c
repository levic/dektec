//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* Dtu.c *#*#*#*#*#*#*#*#*#*# (C) 2011-2015 DekTec
//
// Dtu driver - Interface for the Dtu common driver, used by the IAL.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtuIncludes.h>

#ifdef LINBUILD
#if LINUX_VERSION_CODE<KERNEL_VERSION(2,6,34)
#define usb_alloc_coherent usb_buffer_alloc
#define usb_free_coherent usb_buffer_free
#endif
#endif

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// Forward declarations
static DtStatus  DtuCalculateAndCreatePortsStruct(DtuDeviceData* pDvcData);
static void  DtuCleanupPortStructs(DtuDeviceData* pDvcData);
static DtStatus  DtuPortsInit(DtuDeviceData* pDvcData);
static void  DtuPortsCleanUp(DtuDeviceData* pDvcData);
static Bool DtuDeviceIsInuse(DtuDeviceData* pDvcData);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Dtu Common Interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDriverInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL and initializes the common (host independent) part
// of the driver.
//
DtStatus  DtuDriverInit(DtEvtLog* pEvtObject)
{
    DtStatus  Status = DT_STATUS_OK;
    DtString  DtStrVersion;
    DtStringChar  DtStrVersionBuffer[32];
    DT_STRING_DECL(DtStrDot, ".");

    // Connect DtStrVersionBuffer to DtStrVersion
    DT_STRING_INIT(DtStrVersion, DtStrVersionBuffer, 32);

    // Create driver version string
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTU_VERSION_MAJOR);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTU_VERSION_MINOR);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTU_VERSION_MICRO);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringAppendDtString(&DtStrVersion, &DtStrDot);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtStringUIntegerToDtStringAppend(&DtStrVersion, 10, DTU_VERSION_BUILD);
    if (!DT_SUCCESS(Status))
        return Status;
    
    // Finally report the event
    DtEvtLogReport(pEvtObject, DTU_LOG_DRIVER_LOADED, &DtStrVersion, NULL, NULL);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDriverExit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL when the driver is unloaded. This function should
// undo everything done by DtDriverInit.
//
void  DtuDriverExit()
{
    // NOTHING TODO FOR NOW
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDeviceInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL when the device is found in the system by the bus
// driver. 
//
DtStatus  DtuDeviceInit(DtuDeviceData* pDvcData)
{
    Int  i;
    DtStatus  Status = DT_STATUS_OK;
    
    DtDbgOut(MAX, DTU, "Start");

    // Deduce typenumber
    if (pDvcData->m_DevInfo.m_ProductId == DTU3_PID_UNIT_EEPROM)
    {
        Int64  TypeNumber = -1;
        Status = DtNonVolatileManufSettingsRead(&pDvcData->m_Driver, -1, "DtuTypeNumber",
                                                                             &TypeNumber);
        pDvcData->m_DevInfo.m_TypeNumber = (Int)TypeNumber;
        if (!DT_SUCCESS(Status))
            DtDbgOut(ERR, DTU, "FX3 device with uninitialized eeprom found but \"DtuTypeNumber\" not set");
    } else {
        pDvcData->m_DevInfo.m_TypeNumber = 
                                 DtuProductId2TypeNumber(pDvcData->m_DevInfo.m_ProductId);
    }

    DtDbgOut(MIN, DTU, "Found: pid 0x%04X, vendor 0x%04X => DTU-%d", 
                       pDvcData->m_DevInfo.m_ProductId, pDvcData->m_DevInfo.m_VendorId,
                                                        pDvcData->m_DevInfo.m_TypeNumber);

    // The first powerup is special to initialize resources that need the hardware
    pDvcData->m_InitialPowerup = TRUE;
    
    pDvcData->m_BootState = DTU_BOOTSTATE_WARM;

    // Initialize file handle info mutex
    DtFastMutexInit(&pDvcData->m_FileHandleInfoMutex);
    // Initialize file handle info
    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
    {
        pDvcData->m_FileHandleInfo[i].m_pHandle = NULL;
        pDvcData->m_FileHandleInfo[i].m_PowerDownPending = FALSE;
    }

    // Initialize Exclusive access fast mutex
    DtFastMutexInit(&pDvcData->m_ExclAccessMutex);

    // Initialize events
    DtuEventsInit(pDvcData);

    DtDbgOut(MAX, DTU, "Exit (device: DTU-%d)", pDvcData->m_DevInfo.m_TypeNumber);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDeviceOpen -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function will be called by the IAL every time an application opens a handle to the
// device.
//
DtStatus  DtuDeviceOpen(DtuDeviceData* pDvcData, DtFileObject* pFile)
{
    Int  i;

    // Skip user access if not completly initialised
    if (pDvcData->m_InitialPowerup)
        return DT_STATUS_NOT_INITIALISED;

    // Add file handle to the filehandle info array
    DtFastMutexAcquire(&pDvcData->m_FileHandleInfoMutex);
    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
    {
        if (pDvcData->m_FileHandleInfo[i].m_pHandle == NULL)
        {
            pDvcData->m_FileHandleInfo[i].m_pHandle = DtFileGetHandle(pFile);
            pDvcData->m_FileHandleInfo[i].m_PowerDownPending = FALSE;
            break;
        }
    }
    DtFastMutexRelease(&pDvcData->m_FileHandleInfoMutex);

    // Check if we found an unused entry
    if (i == MAX_NUM_FILE_HANDLES)
        return DT_STATUS_OUT_OF_RESOURCES;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct  AsyncRequestDtu351 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct  _AsyncRequestDtu351
{
#ifdef WINBUILD
    PURB  m_pUrb;
    WDFMEMORY  m_UrbMemory;
    WDFREQUEST  m_WdfRequest;
#else
    struct urb*  m_pUrb;
    void*  m_pBuf;
#endif
    DtEvent  m_EvtRequestDone;
    Bool  m_IsValid;
    Dtu351DataHdr*  m_DataHeaders;
    Int  m_FirstIdx;
} AsyncRequestDtu351;

#ifdef WINBUILD
EVT_WDF_REQUEST_COMPLETION_ROUTINE  Dtu351EvtComplete;
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu351EvtComplete -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  Dtu351EvtComplete(
    WDFREQUEST  Request,
    WDFIOTARGET  Target,
    PWDF_REQUEST_COMPLETION_PARAMS  Params,
    void*  pContext
    )
{
    AsyncRequestDtu351*  Req = (AsyncRequestDtu351*)pContext;
    Int  NumPackets = 1;

    if (Params->IoStatus.Status != STATUS_SUCCESS)
    {
        DtDbgOut(ERR, DTU, "IoStatus: 0x%X", Params->IoStatus.Status);
        Req->m_DataHeaders[Req->m_FirstIdx].m_NumValid = 0;
    }
    else if (Req->m_pUrb->UrbHeader.Status != USBD_STATUS_SUCCESS)
    {
        DtDbgOut(ERR, DTU, "UrbHeader.Status: 0x%X", Req->m_pUrb->UrbHeader.Status);
        Req->m_DataHeaders[Req->m_FirstIdx].m_NumValid = 0;
    } else {
        Req->m_DataHeaders[Req->m_FirstIdx].m_NumValid = 
                             Req->m_pUrb->UrbBulkOrInterruptTransfer.TransferBufferLength;
    }

    Req->m_DataHeaders[Req->m_FirstIdx].m_Flags = 1;
    
    DtEventSet(&Req->m_EvtRequestDone);
}
#else
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu351EvtComplete -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  Dtu351EvtComplete(struct urb*  pUrb)
{
    AsyncRequestDtu351*  Req = (AsyncRequestDtu351*)pUrb->context;
    
    if (pUrb->status != 0)
    {
        DtDbgOut(ERR, DTU, "pUrb->status: 0x%X", pUrb->status);
    }
    memcpy(Req->m_pBuf, pUrb->transfer_buffer, pUrb->actual_length);

    Req->m_DataHeaders[Req->m_FirstIdx].m_NumValid = pUrb->actual_length;
    Req->m_DataHeaders[Req->m_FirstIdx].m_Flags = 1;

    DtEventSet(&Req->m_EvtRequestDone);
}
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct  AsyncRequestDtu315 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct  _AsyncRequestDtu315
{
#ifdef WINBUILD
    PURB  m_pUrb;
    WDFMEMORY  m_UrbMemory;
    WDFREQUEST  m_WdfRequest;
#else
    struct urb*  m_pUrb;
    void*  m_pBuf;
#endif
    DtEvent  m_EvtRequestDone;
    Bool  m_IsValid;
    DtStatus  m_Result;
    UInt  m_OffsetStart;
    UInt  m_Size;
} AsyncRequestDtu315;

#ifdef WINBUILD
EVT_WDF_REQUEST_COMPLETION_ROUTINE  Dtu315EvtComplete;
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu315EvtComplete -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  Dtu315EvtComplete(
    WDFREQUEST  Request,
    WDFIOTARGET  Target,
    PWDF_REQUEST_COMPLETION_PARAMS  Params,
    void*  pContext
    )
{
    AsyncRequestDtu315*  Req = (AsyncRequestDtu315*)pContext;

    if (Params->IoStatus.Status != STATUS_SUCCESS)
    {
        DtDbgOut(ERR, DTU, "IoStatus: 0x%X", Params->IoStatus.Status);
        if (Params->IoStatus.Status == STATUS_IO_TIMEOUT)
            Req->m_Result = DT_STATUS_TIMEOUT;
        else
            Req->m_Result = DT_STATUS_FAIL;
    }
    else if (Req->m_pUrb->UrbHeader.Status != USBD_STATUS_SUCCESS)
    {
        DtDbgOut(ERR, DTU, "UrbHeader.Status: 0x%X", Req->m_pUrb->UrbHeader.Status);
        Req->m_Result = DT_STATUS_FAIL;
    } else {
        Req->m_Result = DT_STATUS_OK;
    }
    
    DtEventSet(&Req->m_EvtRequestDone);
}
#else
void  Dtu315EvtComplete(struct urb*  pUrb)
{
    AsyncRequestDtu315*  Req = (AsyncRequestDtu315*)pUrb->context;
    
    if (pUrb->status != 0)
    {
        DtDbgOut(ERR, DTU, "pUrb->status: 0x%X", pUrb->status);
    }
    //TODOTM: linux implementation DTU-315

    DtEventSet(&Req->m_EvtRequestDone);
}
#endif

void  Dtu351WorkerThreadCheckLock(DtuDeviceData*, DtuNonIpPort*);
void  Dtu351WorkerThreadReadData(DtuDeviceData*, DtuNonIpPort*);
void  Dtu315WorkerThreadWriteData(DtuDeviceData*, DtuNonIpPort*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu3WorkerThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  Dtu3WorkerThread(DtThread* pThread, void* pContext)
{
    DtuNonIpPort*  pPort = (DtuNonIpPort*)pContext;
    DtuDeviceData*  pDvcData = pPort->m_pDvcData;
    Bool  StopThread = FALSE;

    while (!StopThread)
    {
        DtDbgOut(AVG, DTU, "USB3 worker thread state: %d", pPort->m_State);
        switch (pPort->m_State)
        {
        case DTU3_STATE_IDLE:
            // Nothing to do
            break;

        case DTU3_STATE_EXIT:
            StopThread = TRUE;
            break;

        case DTU3_STATE_DET_VIDSTD:
            Dtu351WorkerThreadCheckLock(pDvcData, pPort);
            break;

        case DTU3_STATE_READ351:
            Dtu351WorkerThreadReadData(pDvcData, pPort);
            break;

        case DTU3_STATE_WRITE315:
            Dtu315WorkerThreadWriteData(pDvcData, pPort);
            break;

        default:
            DT_ASSERT(FALSE);
        }
        if (!StopThread)
        {
            DtEventWait(&pPort->m_StateChanged, -1);
            pPort->m_State = pPort->m_NextState;
            DtEventReset(&pPort->m_StateChanged);
        }
        DtEventSet(&pPort->m_StateChangeCmpl);
    }

    DtThreadWaitForStop(pThread);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- GetConfiguredVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  GetConfiguredVidStd(DtuNonIpPort*  pPort, Int*  pVidStd, Bool*  pIsHd)
{
    DtuIoConfigValue  DtuIoCfgVal;
    DT_RETURN_ON_ERROR(DtuNonIpIoConfigGet(pPort, DT_IOCONFIG_IOSTD, &DtuIoCfgVal));
    switch (DtuIoCfgVal.m_Value)
    {
    case DT_IOCONFIG_SDI:
        if (pIsHd != NULL)
            *pIsHd = FALSE;
        switch (DtuIoCfgVal.m_SubValue)
        {
        case DT_IOCONFIG_525I59_94: *pVidStd = DT_VIDSTD_525I59_94; break;
        case DT_IOCONFIG_625I50:    *pVidStd = DT_VIDSTD_625I50; break;
        default: return DT_STATUS_FAIL;
        };
        break;
    case DT_IOCONFIG_HDSDI:
        if (pIsHd != NULL)
            *pIsHd = TRUE;
        switch (DtuIoCfgVal.m_SubValue)
        {
        case DT_IOCONFIG_1080I50:    *pVidStd = DT_VIDSTD_1080I50; break;
        case DT_IOCONFIG_1080I59_94: *pVidStd = DT_VIDSTD_1080I59_94; break;
        case DT_IOCONFIG_1080I60:    *pVidStd = DT_VIDSTD_1080I60; break;
        case DT_IOCONFIG_1080P23_98: *pVidStd = DT_VIDSTD_1080P23_98; break;
        case DT_IOCONFIG_1080P24:    *pVidStd = DT_VIDSTD_1080P24; break;
        case DT_IOCONFIG_1080P25:    *pVidStd = DT_VIDSTD_1080P25; break;
        case DT_IOCONFIG_1080P29_97: *pVidStd = DT_VIDSTD_1080P29_97; break;
        case DT_IOCONFIG_1080P30:    *pVidStd = DT_VIDSTD_1080P30; break;
        case DT_IOCONFIG_1080PSF23_98: *pVidStd = DT_VIDSTD_1080PSF23_98; break;
        case DT_IOCONFIG_1080PSF24:    *pVidStd = DT_VIDSTD_1080PSF24; break;
        case DT_IOCONFIG_1080PSF25:    *pVidStd = DT_VIDSTD_1080PSF25; break;
        case DT_IOCONFIG_1080PSF29_97: *pVidStd = DT_VIDSTD_1080PSF29_97; break;
        case DT_IOCONFIG_1080PSF30:    *pVidStd = DT_VIDSTD_1080PSF30; break;
        case DT_IOCONFIG_720P23_98:  *pVidStd = DT_VIDSTD_720P23_98; break;
        case DT_IOCONFIG_720P24:     *pVidStd = DT_VIDSTD_720P24; break;
        case DT_IOCONFIG_720P25:     *pVidStd = DT_VIDSTD_720P25; break;
        case DT_IOCONFIG_720P29_97:  *pVidStd = DT_VIDSTD_720P29_97; break;
        case DT_IOCONFIG_720P30:     *pVidStd = DT_VIDSTD_720P30; break;
        case DT_IOCONFIG_720P50:     *pVidStd = DT_VIDSTD_720P50; break;
        case DT_IOCONFIG_720P59_94:  *pVidStd = DT_VIDSTD_720P59_94; break;
        case DT_IOCONFIG_720P60:     *pVidStd = DT_VIDSTD_720P60; break;
        default: return DT_STATUS_FAIL;
        };
        break;
    case DT_IOCONFIG_3GSDI:
        if (pIsHd != NULL)
            *pIsHd = TRUE;
        switch (DtuIoCfgVal.m_SubValue)
        {
        case DT_IOCONFIG_1080P50:    *pVidStd = DT_VIDSTD_1080P50; break;
        case DT_IOCONFIG_1080P59_94: *pVidStd = DT_VIDSTD_1080P59_94; break;
        case DT_IOCONFIG_1080P60:    *pVidStd = DT_VIDSTD_1080P60; break;
        default: return DT_STATUS_FAIL;
        };
        break;
    default:
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu351DoVidStdDetect -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  Dtu351DoVidStdDetect(DtuNonIpPort*  pPort)
{
    Int  VidStd, CfgVidStd;
    DtStatus  Status = DtuNonIpDetectVidStd(pPort, &VidStd);
    if (!DT_SUCCESS(Status) || VidStd==DT_VIDSTD_UNKNOWN)
    {
        pPort->m_DetVidStd = DT_VIDSTD_UNKNOWN;
        pPort->m_StateFlags |= DTU_PORT_FLAG_SDI_NO_LOCK | DTU_PORT_FLAG_SDI_INVALID;
    } else {
        pPort->m_DetVidStd = VidStd;
        // Clear the no-lock flag
        pPort->m_StateFlags &= ~DTU_PORT_FLAG_SDI_NO_LOCK;
        // If the detected video standard is the same as the configured one, clear
        // the SDI-invalid flag as well.
        Status = GetConfiguredVidStd(pPort, &CfgVidStd, NULL);
        if (DT_SUCCESS(Status) && VidStd==CfgVidStd)
            pPort->m_StateFlags &= ~DTU_PORT_FLAG_SDI_INVALID;
        else
            pPort->m_StateFlags |= DTU_PORT_FLAG_SDI_INVALID;
    }
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu351WorkerThreadCheckLock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  Dtu351WorkerThreadCheckLock(
    DtuDeviceData*  pDvcData,
    DtuNonIpPort*  pPort)
{
    do {
        Dtu351DoVidStdDetect(pPort);
    } while (DtEventWait(&pPort->m_StateChanged, 500) == DT_STATUS_TIMEOUT);
}

#ifdef WINBUILD
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu351InitAsyncRequest -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  Dtu351InitAsyncRequest(AsyncRequestDtu351  *Req, void* pBuf, Int  BufLen,
    USBD_PIPE_HANDLE wdmhUSBPipe,
    WDFUSBPIPE  Pipe,
    Int  UrbSize,
    WDFIOTARGET  IoTarget)
{
    NTSTATUS  NtStatus = WdfMemoryCreate(WDF_NO_OBJECT_ATTRIBUTES, NonPagedPool, DTU_TAG,
                                    UrbSize, &Req->m_UrbMemory, (void**)&Req->m_pUrb);
    if (!NT_SUCCESS(NtStatus))
    {
        DtDbgOut(ERR, DTU, "WdfMemoryCreate failed");
        Req->m_pUrb = NULL;
        return FALSE;
    }

    NtStatus = WdfRequestCreate(WDF_NO_OBJECT_ATTRIBUTES, IoTarget, &Req->m_WdfRequest);
    if (!NT_SUCCESS(NtStatus))
    {
        DtDbgOut(ERR, DTU, "WdfRequestCreate failed");
        WdfObjectDelete(Req->m_UrbMemory);
        Req->m_pUrb = NULL;
        return FALSE;
    }

    RtlZeroMemory(Req->m_pUrb, UrbSize);
    Req->m_pUrb->UrbBulkOrInterruptTransfer.Hdr.Length = (USHORT)UrbSize;
    Req->m_pUrb->UrbBulkOrInterruptTransfer.Hdr.Function = URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER;
    Req->m_pUrb->UrbBulkOrInterruptTransfer.PipeHandle = wdmhUSBPipe;
    Req->m_pUrb->UrbBulkOrInterruptTransfer.PipeHandle = wdmhUSBPipe;
    Req->m_pUrb->UrbBulkOrInterruptTransfer.TransferFlags = USBD_TRANSFER_DIRECTION_IN
                                                                | USBD_SHORT_TRANSFER_OK;
    Req->m_pUrb->UrbBulkOrInterruptTransfer.TransferBuffer = pBuf;
    Req->m_pUrb->UrbBulkOrInterruptTransfer.TransferBufferLength = BufLen;
    NtStatus = WdfUsbTargetPipeFormatRequestForUrb(Pipe, Req->m_WdfRequest,
                                                                Req->m_UrbMemory, NULL);
    if (!NT_SUCCESS(NtStatus))
    {
        DtDbgOut(ERR, DTU, "WdfUsbTargetPipeFormatRequestForUrb failed");
        WdfObjectDelete(Req->m_WdfRequest);
        WdfObjectDelete(Req->m_UrbMemory);
        Req->m_pUrb = NULL;
        NtStatus = STATUS_UNSUCCESSFUL;
        return FALSE;
    }
    if (!DT_SUCCESS(DtEventInit(&Req->m_EvtRequestDone, TRUE)))
    {
        DtDbgOut(ERR, DTU, "DtEventInit failed");
        WdfObjectDelete(Req->m_WdfRequest);
        WdfObjectDelete(Req->m_UrbMemory);
        Req->m_pUrb = NULL;
        NtStatus = STATUS_UNSUCCESSFUL;
        return FALSE;
    }
    WdfRequestSetCompletionRoutine(Req->m_WdfRequest, Dtu351EvtComplete, Req);
    return TRUE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu351ReInitAsyncRequest -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  Dtu351ReInitAsyncRequest(AsyncRequestDtu351  *Req, void* pBuf, Int  BufLen,
    USBD_PIPE_HANDLE wdmhUSBPipe,
    WDFUSBPIPE  Pipe,
    Int  UrbSize,
    WDFIOTARGET  IoTarget)
{
    NTSTATUS  NtStatus;
    Req->m_pUrb->UrbBulkOrInterruptTransfer.Hdr.Length = (USHORT)UrbSize;
    Req->m_pUrb->UrbBulkOrInterruptTransfer.Hdr.Function = URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER;
    Req->m_pUrb->UrbBulkOrInterruptTransfer.PipeHandle = wdmhUSBPipe;
    Req->m_pUrb->UrbBulkOrInterruptTransfer.PipeHandle = wdmhUSBPipe;
    Req->m_pUrb->UrbBulkOrInterruptTransfer.TransferFlags = USBD_TRANSFER_DIRECTION_IN
                                                                | USBD_SHORT_TRANSFER_OK;
    Req->m_pUrb->UrbBulkOrInterruptTransfer.TransferBuffer = pBuf;
    Req->m_pUrb->UrbBulkOrInterruptTransfer.TransferBufferLength = BufLen;

    NtStatus = WdfUsbTargetPipeFormatRequestForUrb(Pipe, Req->m_WdfRequest,
                                                                  Req->m_UrbMemory, NULL);
    if (!NT_SUCCESS(NtStatus))
    {
        DtDbgOut(ERR, DTU, "WdfUsbTargetPipeFormatRequestForUrb failed");
        return FALSE;
    }
    WdfRequestSetCompletionRoutine(Req->m_WdfRequest, Dtu351EvtComplete, Req);
    return TRUE;
}
#else
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu351InitAsyncRequest -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  Dtu351InitAsyncRequest(AsyncRequestDtu351  *Req, void* pBuf, Int  BufLen,
    DtuDeviceData*  pDvcData,
    UInt  Pipe)
{
    UInt8*  pUrbMem = NULL;
    struct usb_device*  pDev = pDvcData->m_Device.m_pUsbDev;
    if (!DT_SUCCESS(DtEventInit(&Req->m_EvtRequestDone, TRUE)))
    {
        DtDbgOut(ERR, DTU, "DtEventInit failed");
        return FALSE;
    }

    Req->m_pUrb = usb_alloc_urb(0, GFP_ATOMIC);
    if (Req->m_pUrb == NULL)
    {
        DtDbgOut(ERR, DTU, "Failed to allocate urb");
        return FALSE;
    }
    pUrbMem = usb_alloc_coherent(pDev, BufLen, GFP_KERNEL, &Req->m_pUrb->transfer_dma);
    if (pUrbMem == NULL)
    {
        DtDbgOut(ERR, DTU, "Failed to allocate memory for urb");
        usb_free_urb(Req->m_pUrb);
        Req->m_pUrb = NULL;
        return FALSE;
    }

    usb_fill_bulk_urb(Req->m_pUrb, pDev, Pipe, pUrbMem, BufLen, Dtu351EvtComplete, Req);
    Req->m_pUrb->transfer_flags |= URB_NO_TRANSFER_DMA_MAP;
    Req->m_pBuf = pBuf;
    return TRUE;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu351ReInitAsyncRequest -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  Dtu351ReInitAsyncRequest(AsyncRequestDtu351  *Req, void* pBuf, Int  BufLen,
    DtuDeviceData*  pDvcData,
    UInt  Pipe)
{
    Req->m_pBuf = pBuf;
    return TRUE;
}
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu351WorkerThreadReadData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  Dtu351WorkerThreadReadData(
    DtuDeviceData*  pDvcData,
    DtuNonIpPort*  pPort)
{
#ifdef WINBUILD
    NTSTATUS  NtStatus = STATUS_SUCCESS;
    USBD_PIPE_HANDLE wdmhUSBPipe;
    WDFUSBPIPE  Pipe = NULL;
    WDFIOTARGET  IoTarget;
    WDF_REQUEST_SEND_OPTIONS  SendOptions;
    Int  OldPriority;
    Int  UrbSize = -1;
#else
    UInt  Pipe;
#endif
    DtStatus  Status = DT_STATUS_OK;
    Dtu351BufHdr*  BufHeader;
    UInt8*  Buf;
    Int  i;
    AsyncRequestDtu351*  Requests;
    Bool  FatalError = FALSE;
    Int  NumAsyncRequests;
    Dtu351DataHdr*  DataBufHeaders;
    Int  DataBufSize;
    Int  NextBufIdx = 0;
    Bool  IsHd = FALSE;
    Int  LoopCtr = 0;
    Int  SingleDataBufSize;

    DtDbgOut(MAX, DTU, "Start");
#ifdef WINBUILD
    OldPriority = KeSetPriorityThread(KeGetCurrentThread(), 30);
    DtDbgOut(MAX, DTU, "Old priority: %d", OldPriority);
#endif

    DT_ASSERT(pPort->m_SharedBuffer.m_Initialised);

    BufHeader = (Dtu351BufHdr*)pPort->m_SharedBuffer.m_pBuffer;
    DataBufHeaders = (Dtu351DataHdr*)(pPort->m_SharedBuffer.m_pBuffer + 
                                                                    sizeof(Dtu351BufHdr));
    Buf = pPort->m_SharedBuffer.m_pBuffer + sizeof(Dtu351BufHdr)
                                          + BufHeader->m_NumBuffers*sizeof(Dtu351DataHdr);

    DtuGetuFrameSize(pPort, &SingleDataBufSize);
    DataBufSize = BufHeader->m_NumBuffers * SingleDataBufSize;
    DT_ASSERT(BufHeader->m_TotalBufSize == sizeof(Dtu351BufHdr) + 
                             BufHeader->m_NumBuffers*sizeof(Dtu351DataHdr) + DataBufSize);
    
    if (BufHeader->m_TotalBufSize != sizeof(Dtu351BufHdr) + 
                              BufHeader->m_NumBuffers*sizeof(Dtu351DataHdr) + DataBufSize)
    {
        DtDbgOut(ERR, DTU, "Total buffer size is inconsistent");
        DtEventWait(&pPort->m_StateChanged, -1);
        return;
    }
    
    NumAsyncRequests = BufHeader->m_NumAsyncReads;
    DT_ASSERT(NumAsyncRequests < (Int)BufHeader->m_NumBuffers);
    if (NumAsyncRequests >= (Int)BufHeader->m_NumBuffers)
    {
        DtDbgOut(ERR, DTU, "Not enough data buffers");
        DtEventWait(&pPort->m_StateChanged, -1);
        return;
    }
    
#ifdef WINBUILD
    // Initialize send options used for URB timeouts
    WDF_REQUEST_SEND_OPTIONS_INIT(&SendOptions, WDF_REQUEST_SEND_OPTION_TIMEOUT);
    // 500*NumAsyncRequests ms Timeout in 100ns units
    WDF_REQUEST_SEND_OPTIONS_SET_TIMEOUT(&SendOptions, -10*1000*500*NumAsyncRequests);
#endif
    
    // Isochronous transfers are only started when the signal is valid. If the signal
    // turns out to be invalid we'll stop the transfers and detect it soon enough. While
    // we are transferring data we don't check the lock flag of the gennum but instead
    // assume the signals stays valid.
    // We clear this flag here since there is a race condition otherwise that can happen
    // like this:
    // 1. Dtapi detects signal is valid, so starts transfers.
    // 2. Driver checks gennum state, lock has been lost, so sets SDI_INVALID flag.
    // 3. This function is started, flag is still set. In the meantime the gennum is in
    //    lock again with the correct video standard.
    // 4. Dtapi reads data without timeouts and without errors.
    // 5. Users sees valid frames but SDI_INVALID flag is still set.
    pPort->m_StateFlags &= ~(DTU_PORT_FLAG_SDI_NO_LOCK | DTU_PORT_FLAG_SDI_INVALID);

    // For the same reason we don't update the status flags while we're receiving data,
    // we hardcode the detected video standard here too.
    Status = GetConfiguredVidStd(pPort, &pPort->m_DetVidStd, &IsHd);
    DT_ASSERT(DT_SUCCESS(Status));
    
    DtDbgOut(MAX, DTU, "m_TotalBufSize = 0x%0X", BufHeader->m_TotalBufSize);
    DtDbgOut(MAX, DTU, "m_Overflow = 0x%0X", BufHeader->m_Overflow);
    DtDbgOut(MAX, DTU, "m_NumAsyncReads = 0x%0X", BufHeader->m_NumAsyncReads);

    for (i=0; i<(Int)BufHeader->m_NumBuffers; i++)
    {
        if (DataBufHeaders[i].m_Flags != 0)
        {
            DtDbgOut(ERR, DTU, "m_Flags != 0");
            DataBufHeaders[i].m_Flags = 0;
        }
    }
    
    Requests = DtMemAllocPool(DtPoolNonPaged, sizeof(AsyncRequestDtu351)*NumAsyncRequests,
                                                                                 DTU_TAG);
    
#ifdef WINBUILD
    Pipe = WdfUsbInterfaceGetConfiguredPipe(pDvcData->m_Device.m_UsbInterface, 0, NULL);
    
    wdmhUSBPipe = WdfUsbTargetPipeWdmGetPipeHandle(Pipe);
    IoTarget = WdfUsbTargetDeviceGetIoTarget(pDvcData->m_Device.m_UsbDevice);

    UrbSize = sizeof(struct _URB_BULK_OR_INTERRUPT_TRANSFER);
#else
    Pipe = usb_rcvbulkpipe(pDvcData->m_Device.m_pUsbDev, 3);
#endif

    // Set gennum rate selection to manual and hd/sd
    Status = Dtu35xRegWrite(pDvcData, DTU_USB3_DEV_GS1661, 0x24, IsHd ? 0 : 1);
    DT_ASSERT(DT_SUCCESS(Status));
    
    // Set FPGA register SdiDefinition
    Status = Dtu35xRegWrite(pDvcData, DTU_USB3_DEV_FPGA, 0x04, IsHd ? 1 : 0);
    DT_ASSERT(DT_SUCCESS(Status));

    // Default-initialize all request, so we can clean up properly later.
    for (i=0; i<NumAsyncRequests; i++)
    {
        AsyncRequestDtu351  *Req = &Requests[i];
        Req->m_pUrb = NULL;
        Req->m_IsValid = FALSE;
    }

    // First initialize all requests
    for (i=0; i<NumAsyncRequests && !FatalError; i++)
    {
        Bool  InitOk = FALSE;
        AsyncRequestDtu351  *Req = &Requests[i];
#ifdef WINBUILD
        InitOk = Dtu351InitAsyncRequest(Req, Buf + NextBufIdx*SingleDataBufSize,
                                                    SingleDataBufSize,
                                                    wdmhUSBPipe, Pipe, UrbSize, IoTarget);
#else
        InitOk = Dtu351InitAsyncRequest(Req, Buf + NextBufIdx*SingleDataBufSize,
                                                    SingleDataBufSize,
                                                    pDvcData, Pipe);
#endif
        Req->m_DataHeaders = DataBufHeaders;
        Req->m_FirstIdx = NextBufIdx;
        NextBufIdx++;
        if (!InitOk)
        {
            FatalError = TRUE;
            break;
        }
    }
    if (!FatalError)
    {
        // Now start all requests for the first time
        for (i=0; i<NumAsyncRequests && !FatalError; i++)
        {
            AsyncRequestDtu351  *Req = &Requests[i];
#ifdef WINBUILD
            if (WdfRequestSend(Req->m_WdfRequest, IoTarget, &SendOptions) == FALSE)
            {
                DtDbgOut(ERR, DTU, "WdfRequestSend failed");
                WdfObjectDelete(Req->m_WdfRequest);
                WdfObjectDelete(Req->m_UrbMemory);
                Req->m_pUrb = NULL;
                FatalError = TRUE;
                break;
            }
#else
            int res = usb_submit_urb(Req->m_pUrb, GFP_ATOMIC);
            if (res != 0)
            {
                DtDbgOut(ERR, DTU, "usb_submit_urb failed. Res = %d", res);
                usb_free_coherent(Req->m_pUrb->dev, Req->m_pUrb->transfer_buffer_length,
                                 Req->m_pUrb->transfer_buffer, Req->m_pUrb->transfer_dma);
                usb_free_urb(Req->m_pUrb);
                Req->m_pUrb = NULL;
                FatalError = TRUE;
                break;
            }
#endif
            Req->m_IsValid = TRUE;
        }
    }
    DtDbgOut(AVG, DTU, "Request initialization done");
    if (FatalError)
    {
        DtDbgOut(ERR, DTU, "FatalError was set");
#ifdef WINBUILD
        DtDbgOut(ERR, DTU, "NtStatus: 0x%0X", NtStatus);
#endif
    }

    if (!FatalError)
    {
        // Set FPGA register RxMode
        Status = Dtu35xRegWrite(pDvcData, DTU_USB3_DEV_FPGA, 0x06, 
                                                             (UInt32)pPort->m_InitRxMode);
        DT_ASSERT(DT_SUCCESS(Status));
    }
    // We're fully initialized. Allow changes to the RxMode register from other threads.
    pPort->m_AllowRxModeChanges = TRUE;
    
    BufHeader->m_FirstBuf = 0;
    
    while (DtEventWait(&pPort->m_StateChanged, 0)==DT_STATUS_TIMEOUT && !FatalError)
    {
#ifdef WINBUILD
        WDF_REQUEST_REUSE_PARAMS  ReusePars;
        WDF_REQUEST_REUSE_PARAMS_INIT(&ReusePars, WDF_REQUEST_REUSE_NO_FLAGS, 
                                                                          STATUS_SUCCESS);
#endif
        LoopCtr++;
        for (i=0; i<NumAsyncRequests 
                        && DtEventWait(&pPort->m_StateChanged, 0)==DT_STATUS_TIMEOUT; i++)
        {
            AsyncRequestDtu351  *Req = &Requests[i];
            Bool  ReinitOk;
            while (TRUE)
            {
                Status = DtEventWait(&Req->m_EvtRequestDone, 100);
                if (!DT_SUCCESS(Status))
                {
                    DtDbgOut(ERR, DTU, "DtEventWait failed: %x", Status);
                    pPort->m_DataLoss = TRUE;
                } else {
                    pPort->m_DataLoss = FALSE;
                    break;
                }
            }
#ifdef WINBUILD
            NtStatus = WdfRequestReuse(Req->m_WdfRequest, &ReusePars);
            if (!NT_SUCCESS(NtStatus))
            {
                DtDbgOut(ERR, DTU, "WdfRequestReuse failed");
                FatalError = TRUE;
                break;
            }
#endif
            
#ifdef WINBUILD
            ReinitOk = Dtu351ReInitAsyncRequest(Req, Buf + NextBufIdx*SingleDataBufSize,
                                 SingleDataBufSize, wdmhUSBPipe, Pipe, UrbSize, IoTarget);
#else
            ReinitOk = Dtu351ReInitAsyncRequest(Req, Buf + NextBufIdx*SingleDataBufSize,
                                                       SingleDataBufSize, pDvcData, Pipe);
#endif

            Req->m_DataHeaders = DataBufHeaders;
            Req->m_FirstIdx = NextBufIdx;
            if (DataBufHeaders[NextBufIdx].m_Flags != 0)
            {
                if (BufHeader->m_Overflow == 0)
                {
                    DtDbgOut(ERR, DTU, "Overflow in kernel<>dtapi buffer (%d) (Loop %d)", NextBufIdx, LoopCtr);
                    BufHeader->m_Overflow = 1;
                }
                DataBufHeaders[NextBufIdx].m_Flags = 0;
            }
            NextBufIdx++;
            if (NextBufIdx == BufHeader->m_NumBuffers)
                NextBufIdx = 0;
            if (!ReinitOk)
            {
                FatalError = TRUE;
                break;
            }
#ifdef WINBUILD
            if (WdfRequestSend(Req->m_WdfRequest, IoTarget, &SendOptions) == FALSE)
            {
                DtDbgOut(ERR, DTU, "WdfRequestSend failed");
                FatalError = TRUE;
                break;
            }
#else
            if (usb_submit_urb(Req->m_pUrb, GFP_ATOMIC) != 0)
            {
                DtDbgOut(ERR, DTU, "usb_submit_urb failed");
                FatalError = TRUE;
                break;
            }
#endif
        }
    }
    pPort->m_DataLoss = FALSE;
    Dtu351DoVidStdDetect(pPort);

    // While in the process of shutting down don't allow access to the RxMode register
    // on the FPGA.
    pPort->m_AllowRxModeChanges = FALSE;

    DtDbgOut(AVG, DTU, "Cleaning up DTU-351 driver thread");
    // Try to make sure all events are handled.
    for (i=0; i<NumAsyncRequests; i++)
    {
        AsyncRequestDtu351  *Req = &Requests[i];
        if (!Req->m_IsValid)
            continue;
#ifdef WINBUILD
        //WdfRequestCancelSentRequest(Req->m_WdfRequest);
        // Wait until request is either cancelled or completed.
        DtEventWait(&Req->m_EvtRequestDone, -1);
        WdfObjectDelete(Req->m_WdfRequest);
        WdfObjectDelete(Req->m_UrbMemory);
#else
        if (Req->m_pUrb == NULL)
            continue;
        // Wait until request is either cancelled or completed.
        usb_kill_urb(Req->m_pUrb);
        usb_free_coherent(Req->m_pUrb->dev, Req->m_pUrb->transfer_buffer_length,
                                 Req->m_pUrb->transfer_buffer, Req->m_pUrb->transfer_dma);
        usb_free_urb(Req->m_pUrb);
        Req->m_pUrb = NULL;
#endif
    }

    {
        UInt16  Regs[2];
        Status = Dtu35xRegRead(pDvcData, DTU_USB3_DEV_FX3, 0x08, &Regs[0]);
        Status = Dtu35xRegRead(pDvcData, DTU_USB3_DEV_FX3, 0x0C, &Regs[1]);
        if (Regs[0]!=0 || Regs[1]!=0)
            DtDbgOut(ERR, DTU, "PhyErrors: %d, LinkErrors: %d", Regs[0], Regs[1]);
    }
    
    // Set the FPGA to idle mode by using the DVC_RESET function. This will result in a
    // reset of the FPGA and also a flush of the FX3 buffers. This is important since
    // it means the next data transfer will actually start with a frame header.
    DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_PNP_CMD,
                            DTU_PNP_CMD_RESET, DTU_RESET_DVC_STATE, DT_USB_HOST_TO_DEVICE,
                            NULL, 0, NULL, MAX_USB_REQ_TIMEOUT);

    // Set gennum register RATE_SEL to auto
    Dtu35xRegWrite(pDvcData, DTU_USB3_DEV_GS1661, 0x24, 0x04);

    DtMemFreePool(Requests, DTU_TAG);

    DtDbgOut(MAX, DTU, "Exit");
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu315WorkerThreadWriteData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  Dtu315WorkerThreadWriteData(
    DtuDeviceData*  pDvcData,
    DtuNonIpPort*  pPort)
{
#ifdef WINBUILD // TODOTM: DTU-315 linux implementation
    NTSTATUS  NtStatus = STATUS_SUCCESS;
    DtStatus  Status = DT_STATUS_OK;
    Int  i;
    Dtu315BufHdr*  pBufHdr;
    WDF_REQUEST_SEND_OPTIONS  SendOptions;
    UInt8*  pBuf;
    Int  NumAsyncRequests;
    AsyncRequestDtu315*  Requests;
    USBD_PIPE_HANDLE wdmhUSBPipe;
    WDFUSBPIPE  Pipe=NULL;
    WDFIOTARGET  IoTarget;
    Int  UrbSize = -1;
    Bool  FatalError = FALSE;
    Int  SingleDataBufSize;
    Int  NextReq = 0;
    UInt  ReadPtr = 0;                      // Offset of byte to transfer next
    Bool  Started = FALSE;

    DtDbgOut(AVG, DTU, "Start");

    DT_ASSERT(pPort->m_SharedBuffer.m_Initialised);

    pBufHdr = (Dtu315BufHdr*)pPort->m_SharedBuffer.m_pBuffer;
    pBuf = pPort->m_SharedBuffer.m_pBuffer + pBufHdr->m_DataOffset;
    NumAsyncRequests = pBufHdr->m_MaxSimTransfers;
    SingleDataBufSize = pBufHdr->m_SingleTransferSize;

    if ((UInt32)(NumAsyncRequests*SingleDataBufSize) > pBufHdr->m_FifoSize)
    {
        DtDbgOut(ERR, DTU, "NumAsyncRequests*SingleDataBufSize > FifoSize");
        return;
    }

    // Initialize send options used for URB timeouts
    WDF_REQUEST_SEND_OPTIONS_INIT(&SendOptions, WDF_REQUEST_SEND_OPTION_TIMEOUT);
    // 500ms Timeout in 100ns units
    WDF_REQUEST_SEND_OPTIONS_SET_TIMEOUT(&SendOptions, -10*1000*500);
    
    Requests = DtMemAllocPool(DtPoolNonPaged, sizeof(AsyncRequestDtu315)*NumAsyncRequests,
                                                                                 DTU_TAG);

    if (Requests == NULL)
    {
        DtDbgOut(ERR, DTU, "Failed to allocate internal buffer");
        return;
    }

    Pipe = WdfUsbInterfaceGetConfiguredPipe(pDvcData->m_Device.m_UsbInterface, 0, NULL);
    wdmhUSBPipe = WdfUsbTargetPipeWdmGetPipeHandle(Pipe);
    IoTarget = WdfUsbTargetDeviceGetIoTarget(pDvcData->m_Device.m_UsbDevice);

    UrbSize = sizeof(struct _URB_BULK_OR_INTERRUPT_TRANSFER);

    // Default-initialize all request, so we can clean up properly later.
    for (i=0; i<NumAsyncRequests; i++)
    {
        AsyncRequestDtu315  *Req = &Requests[i];
        Req->m_pUrb = NULL;
        Req->m_IsValid = FALSE;
    }

    // First initialize all requests
    for (i=0; i<NumAsyncRequests && !FatalError; i++)
    {
        AsyncRequestDtu315  *Req = &Requests[i];

        NtStatus = WdfMemoryCreate(WDF_NO_OBJECT_ATTRIBUTES, NonPagedPool, DTU_TAG,
                                        UrbSize, &Req->m_UrbMemory, (void**)&Req->m_pUrb);
        if (!NT_SUCCESS(NtStatus))
        {
            DtDbgOut(ERR, DTU, "WdfMemoryCreate failed");
            Req->m_pUrb = NULL;
            break;
        }

        NtStatus = WdfRequestCreate(WDF_NO_OBJECT_ATTRIBUTES, IoTarget, &Req->m_WdfRequest);
        if (!NT_SUCCESS(NtStatus))
        {
            DtDbgOut(ERR, DTU, "WdfRequestCreate failed");
            WdfObjectDelete(Req->m_UrbMemory);
            Req->m_pUrb = NULL;
            break;
        }

        DtMemZero(Req->m_pUrb, UrbSize);
        Req->m_pUrb->UrbBulkOrInterruptTransfer.Hdr.Length = (USHORT)UrbSize;
        Req->m_pUrb->UrbBulkOrInterruptTransfer.Hdr.Function = URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER;
        Req->m_pUrb->UrbBulkOrInterruptTransfer.PipeHandle = wdmhUSBPipe;
        Req->m_pUrb->UrbBulkOrInterruptTransfer.TransferFlags = USBD_TRANSFER_DIRECTION_OUT;

        Req->m_Result = DT_STATUS_OK;
        Req->m_OffsetStart = 0;
        Req->m_Size = 0;
        
        NtStatus = WdfUsbTargetPipeFormatRequestForUrb(Pipe, Req->m_WdfRequest,
                                                                  Req->m_UrbMemory, NULL);
        if (!NT_SUCCESS(NtStatus))
        {
            DtDbgOut(ERR, DTU, "WdfUsbTargetPipeFormatRequestForUrb failed");
            WdfObjectDelete(Req->m_WdfRequest);
            WdfObjectDelete(Req->m_UrbMemory);
            Req->m_pUrb = NULL;
            NtStatus = STATUS_UNSUCCESSFUL;
            break;
        }
        if (!DT_SUCCESS(DtEventInit(&Requests[i].m_EvtRequestDone, TRUE)))
        {
            DtDbgOut(ERR, DTU, "DtEventInit failed");
            WdfObjectDelete(Req->m_WdfRequest);
            WdfObjectDelete(Req->m_UrbMemory);
            Req->m_pUrb = NULL;
            NtStatus = STATUS_UNSUCCESSFUL;
            break;
        }
        WdfRequestSetCompletionRoutine(Req->m_WdfRequest, Dtu315EvtComplete, Req);
        Req->m_Result = DT_STATUS_OK;
        DtEventSet(&Req->m_EvtRequestDone);
        Req->m_IsValid = TRUE;
    }
    // At this point all m_EvtRequestDone events are in the signalled state
    if (!NT_SUCCESS(NtStatus))
    {
        DtDbgOut(ERR, DTU, "NtStatus not OK");
        FatalError = TRUE;
    } else {
        // Initialization done, set hardware TX mode to SEND
        //TODO: replace 0x80 by offset of FifoMemoryLessUsbDvc block, should come from XML
        Dtu3RegWrite(pDvcData, DTU315_FIFO_BLOCK_OFFSET, 
                        &FwbFifoMemoryLessUsbDvc.Control_TxCtrl, DTFWB_MLFIFOTXCTRL_HOLD);
        // Set led control to on (2)
        Dtu3RegWrite(pDvcData, DTU3_FX3_BLOCK_OFFSET, &FwbFx3.LedCtrl, 2);
    }
    DtDbgOut(AVG, DTU, "Init done");
    while (DtEventWait(&pPort->m_StateChanged, 0)==DT_STATUS_TIMEOUT && !FatalError)
    {
        Bool  ShouldStop = FALSE;
        AsyncRequestDtu315  *Req = &Requests[NextReq];
        WDF_REQUEST_REUSE_PARAMS  ReusePars;
        WDF_REQUEST_REUSE_PARAMS_INIT(&ReusePars, WDF_REQUEST_REUSE_NO_FLAGS, STATUS_SUCCESS);
        
        do {
            Status = DtEventWait(&Req->m_EvtRequestDone, 50);
            ShouldStop = DT_SUCCESS(DtEventWait(&pPort->m_StateChanged, 0));
        } while (!DT_SUCCESS(Status) && !ShouldStop);
        if (ShouldStop)
        {
            // m_EvtRequestDone has been reset by the wait above but code belows
            // expects it to be set.
            if (DT_SUCCESS(Status))
                DtEventSet(&Req->m_EvtRequestDone);
            break;
        }

        if (!DT_SUCCESS(Req->m_Result))
        {
            DtDbgOut(ERR, DTU, "Write request failed, DtStatus=0x%08X", Req->m_Result);
            DtEventSet(&Req->m_EvtRequestDone);
            break;
        }
        if (Req->m_OffsetStart + Req->m_Size == pBufHdr->m_FifoSize)
            pBufHdr->m_ReadPtr = 0;
        else
            pBufHdr->m_ReadPtr = Req->m_OffsetStart + Req->m_Size;
        
        // Wait for enough data to be available in the shared buffer
        do {
            // Create local copy to prevent inconsistent reads
            UInt32  WritePtr = pBufHdr->m_WritePtr;
            UInt32  LoadAvail = (WritePtr >= ReadPtr) ? (WritePtr - ReadPtr) : 
                                               (pBufHdr->m_FifoSize - ReadPtr + WritePtr);
            // Wait until either we can transfer the max size or we can transfer
            // everything until the end of the buffer
            if (LoadAvail>=pBufHdr->m_SingleTransferSize ||
                                                   LoadAvail>=pBufHdr->m_FifoSize-ReadPtr)
                break;
        } while (DtEventWait(&pPort->m_StateChanged, 5)==DT_STATUS_TIMEOUT);
        if (DT_SUCCESS(DtEventWait(&pPort->m_StateChanged, 0)))
        {
            DtEventSet(&Req->m_EvtRequestDone);
            break;
        }
        Req->m_OffsetStart = ReadPtr;
        Req->m_Size = pBufHdr->m_SingleTransferSize;
        if (Req->m_OffsetStart + Req->m_Size > pBufHdr->m_FifoSize)
            Req->m_Size = pBufHdr->m_FifoSize - Req->m_OffsetStart;
        ReadPtr += Req->m_Size;
        if (ReadPtr == pBufHdr->m_FifoSize)
            ReadPtr = 0;

        NtStatus = WdfRequestReuse(Req->m_WdfRequest, &ReusePars);
        if (!NT_SUCCESS(NtStatus))
        {
            DtDbgOut(ERR, DTU, "WdfRequestReuse failed");
            FatalError = TRUE;
            break;
        }
        Req->m_pUrb->UrbBulkOrInterruptTransfer.Hdr.Length = (USHORT)UrbSize;
        Req->m_pUrb->UrbBulkOrInterruptTransfer.Hdr.Function = URB_FUNCTION_BULK_OR_INTERRUPT_TRANSFER;
        Req->m_pUrb->UrbBulkOrInterruptTransfer.PipeHandle = wdmhUSBPipe;
        Req->m_pUrb->UrbBulkOrInterruptTransfer.TransferFlags = USBD_TRANSFER_DIRECTION_OUT;
        Req->m_pUrb->UrbBulkOrInterruptTransfer.TransferBuffer = pBuf+Req->m_OffsetStart;
        Req->m_pUrb->UrbBulkOrInterruptTransfer.TransferBufferLength = Req->m_Size;

        NtStatus = WdfUsbTargetPipeFormatRequestForUrb(Pipe, Req->m_WdfRequest,
                                                                Req->m_UrbMemory, NULL);
        if (!NT_SUCCESS(NtStatus))
        {
            DtDbgOut(ERR, DTU, "WdfUsbTargetPipeFormatRequestForUrb failed");
            FatalError = TRUE;
            break;
        }
        WdfRequestSetCompletionRoutine(Req->m_WdfRequest, Dtu315EvtComplete, Req);
        if (WdfRequestSend(Req->m_WdfRequest, IoTarget, &SendOptions) == FALSE)
        {
            DtDbgOut(ERR, DTU, "WdfRequestSend failed");
            FatalError = TRUE;
            break;
        }
        NextReq++;
        if (NextReq == (Int)pBufHdr->m_MaxSimTransfers)
            NextReq = 0;
        if (!Started)
        {
            Dtu3RegWrite(pDvcData, DTU315_FIFO_BLOCK_OFFSET, 
                        &FwbFifoMemoryLessUsbDvc.Control_TxCtrl, DTFWB_MLFIFOTXCTRL_SEND);
            Started = TRUE;
        }
    }
    // All m_EvtRequestDone events should still be in the signalled state here

    DtDbgOut(AVG, DTU, "Cleaning up DTU-315 driver thread");

    //TODO: replace 0x80 by offset of FifoMemoryLessUsbDvc block, should come from XML
    Dtu3RegWrite(pDvcData, DTU315_FIFO_BLOCK_OFFSET, 
                        &FwbFifoMemoryLessUsbDvc.Control_TxCtrl, DTFWB_MLFIFOTXCTRL_IDLE);
    // Set led control to fade (1)
    Dtu3RegWrite(pDvcData, DTU3_FX3_BLOCK_OFFSET, &FwbFx3.LedCtrl, 1);

    // Try to make sure all callbacks are finished
    for (i=0; i<NumAsyncRequests && !FatalError; i++)
    {
        AsyncRequestDtu315  *Req = &Requests[i];
        if (!Req->m_IsValid)
            continue;
        // Wait until request is completed.
        DtEventWait(&Req->m_EvtRequestDone, -1);
        WdfObjectDelete(Req->m_WdfRequest);
        WdfObjectDelete(Req->m_UrbMemory);
    }

    DtMemFreePool(Requests, DTU_TAG);
    
    DtDbgOut(AVG, DTU, "Exit");
#endif
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDevicePowerUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called when the device must enter the active (D0) state. It should
// wake up the corresponding device and initialize it into active state. It should restore
// any saved settings. These settings can be saved during power down.
// This function can be executed in two different scenarios. The first one is during the
// initial powerup. In this case it checks whether the device is supported and allocates
// all the software resources, for example continuous memory buffers for DMA and DPC
// objects. The second scenario is the powerup after powerdown. In this case the
// resources are already allocated. The device hardware only needs to be initialized.
// Since we don't support streams to resume after a powerdown, the channel HW doesn't
// have to be initialized, but the ethernet for example must be re-inintialized with the
// same setttings (like multicast list) that it was using before the power down.
// It is possible that resources have changed during powerdown, therefore the register
// mapping to userspace must be destroyed during powerdown. Also the register pointers
// in the driver must be reinitialized durint the powerup.
//
DtStatus  DtuDevicePowerUp(DtuDeviceData* pDvcData)
{
    char  Buffer[64];
    UInt  BufLen = sizeof(Buffer);
    UInt32  Value = 0;
    Int64  ForcedHardwareRevision;
    DtStatus  Status;
    Bool  ReEnumerate = FALSE;
    DtPropertyData*  pPropData = &pDvcData->m_PropData;

    DtDbgOut(MAX, DTU, "Start");

    // Make sure property not found counter is reset after a power resume
    DtResetPropertiesNotFoundCounter(&pDvcData->m_PropData);

    if (pDvcData->m_InitialPowerup)
    {
        // Initialize property data object
        pPropData->m_pPropertyStore = NULL;
        pPropData->m_PropertyNotFoundCounter = 0;
        //pPropData->m_PropertyNotFoundString = ?;
        pPropData->m_pTableStore = NULL;
        pPropData->m_TypeName = "DTU";
        pPropData->m_TypeNumber = pDvcData->m_DevInfo.m_TypeNumber;
        pPropData->m_SubDvc = 0; // no USB devs with sub-devices yet
        pPropData->m_FirmwareVersion = 0;
        pPropData->m_FirmwareVariant = 0;
        pPropData->m_HardwareRevision = 0;

        // (Re-)initialise the property store
        Status = DtuPropertiesInit(pDvcData);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTU, "failed to initialise the property store");
            return Status;
        }

        // Initialise the table store
        Status = DtTablesInit(pPropData);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTU, "failed to initialise the property table store");
            return Status;
        }

        pDvcData->m_StateFlags = 0;
    }

    // Init EzUsb module
    ReEnumerate = FALSE;
    Status = EzUsbInit(pDvcData, &ReEnumerate);
    if (!DT_SUCCESS(Status))
        return Status;
    if (ReEnumerate)
        return Status;  // No futher action required, device will re-enumerate now


    if (pDvcData->m_InitialPowerup)
    {   
        // Init I2C module
        Status = DtuI2cInit(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;
     }

    // Check watchdog flag
    if (pDvcData->m_DevInfo.m_TypeNumber == 351)
    {
        UInt16  Watchdog;
        Status = Dtu35xRegRead(pDvcData, DTU_USB3_DEV_FX3, 0x02, &Watchdog);
        if (DT_SUCCESS(Status))
        {
            if (Watchdog == 1)
            {
                DtEvtLogReport(&pDvcData->m_Device.m_EvtObject, 
                                      DTU_LOG_WATCHDOG_TRIGGERED_RESET, NULL, NULL, NULL);
                DtDbgOut(ERR, DTU, "Reboot was triggered by watchdog timer");
                // Reset watchdog register
                Dtu35xRegWrite(pDvcData, DTU_USB3_DEV_FX3, 0x02, 1);
            }
        } else
            DtDbgOut(ERR, DTU, "Failed to read watchdog register");
    }
    else if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
    {
        UInt32  Watchdog;
        Status = Dtu3RegRead(pDvcData, DTU3_FX3_BLOCK_OFFSET, &FwbFx3.WatchDog, &Watchdog);
        if (DT_SUCCESS(Status))
        {
            if (Watchdog == 1)
            {
                DtEvtLogReport(&pDvcData->m_Device.m_EvtObject, 
                                    DTU_LOG_WATCHDOG_TRIGGERED_RESET, NULL, NULL, NULL);
                DtDbgOut(ERR, DTU, "Reboot was triggered by watchdog timer");
                // Reset watchdog register
                Dtu3RegClear(pDvcData, DTU3_FX3_BLOCK_OFFSET, &FwbFx3.WatchDog);
            }
        } else
            DtDbgOut(ERR, DTU, "Failed to read watchdog register");
    }

    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
    {
        Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_PNP_CMD, 
                                                   DTU_PNP_CMD_RESET, DTU_RESET_DVC_STATE,
                                                   DT_USB_HOST_TO_DEVICE, NULL, 0, NULL,
                                                   MAX_USB_REQ_TIMEOUT);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    if (pDvcData->m_InitialPowerup)
    {
        // Get USB speed
        Status = DtuGetUsbSpeed(pDvcData, &Value);
        if (!DT_SUCCESS(Status))
            return Status;
        pDvcData->m_DevInfo.m_UsbSpeed = (Int)Value;
        if (pDvcData->m_DevInfo.m_UsbSpeed!=2 && pDvcData->m_DevInfo.m_TypeNumber>=300
                                                  && pDvcData->m_DevInfo.m_TypeNumber<400)
        {
            DtDbgOut(ERR, DTU, "USB3 device not connected via USB3, UsbSpeed=0x%X",
                                                          pDvcData->m_DevInfo.m_UsbSpeed);
            pDvcData->m_StateFlags |= DTU_DEV_FLAG_NO_USB3;
        }
    }

    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
    {
        UInt32  PnpCmdStat = 0;
        if (pDvcData->m_DevInfo.m_TypeNumber == 351)
        {
            UInt16  Value = 0;
            Status = Dtu35xRegRead(pDvcData, DTU_USB3_DEV_FX3, 0x08, &Value);
            PnpCmdStat = Value;
        }
        else
            Status = Dtu3RegRead(pDvcData, DTU3_FX3_BLOCK_OFFSET, &FwbFx3.PnpCmdStat,
                                                                             &PnpCmdStat);
        if (!DT_SUCCESS(Status))
            return Status;
        if (PnpCmdStat != 0)
        {
            DtString  DtStrCmdStat;
            DtStringChar  DtStrBuffer[32];
            DT_STRING_INIT(DtStrCmdStat, DtStrBuffer, 32);

            Status = DtStringUIntegerToDtStringAppend(&DtStrCmdStat, 16, PnpCmdStat);
            if (!DT_SUCCESS(Status))
                return Status;
            DtEvtLogReport(&pDvcData->m_Device.m_EvtObject, 
                               DTU_LOG_DRIVER_TRIGGERED_RESET, &DtStrCmdStat, NULL, NULL);
        }
        if ((PnpCmdStat&1) != 0)
            DtDbgOut(MIN, DTU, "Last reset caused by driver");
        if ((PnpCmdStat&2) != 0)
            DtDbgOut(MIN, DTU, "Last reset caused by driver (vbus toggle)");
        // Reset the 2 reset bits
        if (pDvcData->m_DevInfo.m_TypeNumber == 351)
            Dtu35xRegWrite(pDvcData, DTU_USB3_DEV_FX3, 0x08, PnpCmdStat&3);
        else
            Dtu3RegWrite(pDvcData, DTU3_FX3_BLOCK_OFFSET, &FwbFx3.PnpCmdStat, 
                                                                            PnpCmdStat&3);
        if (PnpCmdStat==0 && pDvcData->m_DevInfo.m_UsbSpeed!=2)
        {
            DtDbgOut(MIN, DTU, "USB3 device currently not in USB3 mode. re-enumerating");
            // Not in USB3 mode. Last reboot of device was not triggered by driver.
            // Sleep for 500ms and reboot the device.
            DtSleep(500);
            // We expect this to succeed but will return in any case since 
            // a re-enumeration is coming up.
            return DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_PNP_CMD, 
                                                  DTU_PNP_CMD_RESET, DTU_RESET_BOOTLOADER,
                                                  DT_USB_HOST_TO_DEVICE, NULL, 0, NULL,
                                                  MAX_USB_REQ_TIMEOUT);
        }
    }

    // Switch on power
    Status = DtuDvcPowerSupplyInit(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    // Initialize PLD
    Status = DtuPldInit(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

    if (pDvcData->m_DevInfo.m_TypeNumber==351 && pDvcData->m_DevInfo.m_UsbSpeed==2)
    {
        UInt16  RegRateSel = 0;

        // Set RxMode register to IDLE
        Status = Dtu35xRegWrite(pDvcData, DTU_USB3_DEV_FPGA, 0x06, 0);
        if (!DT_SUCCESS(Status))
            return Status;
        
        // Turn on automatic rate detection in gennum
        Status = Dtu35xRegRead(pDvcData, DTU_USB3_DEV_GS1661, 0x24, &RegRateSel);
        if (!DT_SUCCESS(Status))
            return Status;
        RegRateSel |= 0x04;
        Status = Dtu35xRegWrite(pDvcData, DTU_USB3_DEV_GS1661, 0x24, RegRateSel);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    if (pDvcData->m_InitialPowerup)
    {
        // Get hardware revision
        pDvcData->m_DevInfo.m_HardwareRevision = 0;
        // Get firmware variant
        pDvcData->m_DevInfo.m_FirmwareVariant = 0;

        // Get USB address
        Status = DtuGetUsbAddress(pDvcData, &Value);
        if (!DT_SUCCESS(Status))
            return Status;
        pDvcData->m_DevInfo.m_UsbAddress = (Int)Value;

        // Get Firmware version
        if (pDvcData->m_DevInfo.m_TypeNumber>=300 &&
                                                     pDvcData->m_DevInfo.m_TypeNumber<400)
        {
            // For DTU-3 devices, the current firmware version is equivalent to the
            // property FIRMWARE_LAST_VER. This removes the need for versioning in the
            // FX3 or FPGA (the FPGA may not be loaded at this point, e.g. for DTU-315)
            UInt  NotFoundCounter = pPropData->m_PropertyNotFoundCounter;
            Int  FwVer = DtPropertiesGetInt(pPropData, "FIRMWARE_LAST_VER", -1);
            if (NotFoundCounter == pPropData->m_PropertyNotFoundCounter)
            {
                pDvcData->m_DevInfo.m_FirmwareVersion = FwVer;
            } else {
                // Couldn't find property FIRMWARE_LAST_VER. Set firmware version to -1.
                DtDbgOut(ERR, DTU, "No property FIRMWARE_LAST_VER found for DTU-%d. "\
                                   "Firmware version set to -1",
                                                        pDvcData->m_DevInfo.m_TypeNumber);
                pDvcData->m_DevInfo.m_FirmwareVersion = -1;
            }
        } else {
            Status = DtuRegRead(pDvcData, DT_GEN_REG_GENCTRL, &Value);
            if (!DT_SUCCESS(Status))
                return Status;
            pDvcData->m_DevInfo.m_FirmwareVersion = (Value & DT_GENCTRL_FWREV_MSK)  >>
                                                                    DT_GENCTRL_FWREV_SH;
        }

        // Initialise the number and type of ports the card supports. 
        Status = DtuCalculateAndCreatePortsStruct(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;

         // Check if no property errors occurred
        Status = DtuPropertiesReportDriverErrors(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;
        
        // Init VPD module
        Status = DtuVpdInit(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;

        // Get serial number
        pDvcData->m_DevInfo.m_Serial = -1;
        Status = DtuVpdReadItemRo(pDvcData, 2, "SN", Buffer, &BufLen);
        if (DT_SUCCESS(Status))
            pDvcData->m_DevInfo.m_Serial = DtAnsiCharArrayToUInt64(Buffer, BufLen, 10);
        else {
            DtDbgOut(ERR, DTU, "Failed to read serial from VPD (0x%x)", Status);
            DtEvtLogReport(&pDvcData->m_Device.m_EvtObject, DTU_LOG_SERIAL_FAILED, NULL,
                                                                              NULL, NULL);
            pDvcData->m_StateFlags |= DTU_DEV_FLAG_NO_SERIAL;
        }

        // Get hardware revision from VPD "EC" resource
        // Initialize hardware revision to -1, indicating that it is not defined yet
        pDvcData->m_DevInfo.m_HardwareRevision = -1;
        BufLen = sizeof(Buffer);
        Status = DtuVpdReadItemRo(pDvcData, 2, "EC", Buffer, &BufLen);
        if (DT_SUCCESS(Status))
        {
            // VPD "EC" resource can be read.
            Status = DtUtilitiesDeduceHardwareRevision(Buffer, BufLen, 
                                                 &pDvcData->m_DevInfo.m_HardwareRevision); 
            if (DT_SUCCESS(Status))
            {
                DtDbgOut(MIN, DTU, "'EC' is %s, hence deduced hardware rev is %d", 
                                          Buffer, pDvcData->m_DevInfo.m_HardwareRevision);
            } else {
                // VPD "EC" resource is formatted incorrectly
                // Consider this a non-critical error
                pDvcData->m_DevInfo.m_HardwareRevision = 0;
                DtDbgOut(MIN, DTU, "'EC' resouce is formatted incorrectly (%s), "
                                   "hence hardware rev set to 0", Buffer);
            }
        } else {
            // VPD "EC" resource cannot be read, probably because this is a newly 
            // produced card with blank VPD PROM.
            // Check for registry key which defines the hardware revision
            Status = DtNonVolatileManufSettingsRead(&pDvcData->m_Driver, 
                              pDvcData->m_PropData.m_TypeNumber, "ForcedHardwareRevision", 
                                                                 &ForcedHardwareRevision);
            if (DT_SUCCESS(Status))
            {
                pDvcData->m_DevInfo.m_HardwareRevision = (Int)(ForcedHardwareRevision);
                DtDbgOut(MIN, DTU, "Hardware revision set to %d from registry key "
                                   "'ForcedHardwareRevision'", 
                                                  pDvcData->m_DevInfo.m_HardwareRevision);
            } else {
                DtDbgOut(MIN, DTU, "Can't find registry key 'ForcedHardwareRevision'");
            }
        }

        // Verify the hardware revision 
        if (pDvcData->m_DevInfo.m_HardwareRevision < 0)
        {
            // Hardware revision could not be set (no 'EC' resource and no registry key)
            // Consider this a non-critical error
            DtDbgOut(MIN, DTU, "Hardware revision could not be set (no 'EC' resource "
                                "and no registry key 'ForcedHardwareRevision'), "
                                "hence hardware rev set to -1");
            pDvcData->m_DevInfo.m_HardwareRevision = -1;
        }

        pDvcData->m_PropData.m_HardwareRevision = pDvcData->m_DevInfo.m_HardwareRevision;
        pPropData->m_HardwareRevision = pDvcData->m_DevInfo.m_HardwareRevision;

        // *** Set device subtype (-1=undefined, 0=none, 1=A, ...)

        // Initialize subtype to -1, indicating that subtype is not defined yet
        pDvcData->m_DevInfo.m_SubType = -1;

        Status = DtUtilitiesDeriveSubType(&pDvcData->m_PropData, 
                  pDvcData->m_DevInfo.m_HardwareRevision, &pDvcData->m_DevInfo.m_SubType);
        if (DT_SUCCESS(Status))
            DtDbgOut(MIN, DTU, "SubType set to %d", pDvcData->m_DevInfo.m_SubType);
        else
            // Consider this a non-critical error
            DtDbgOut(MIN, DTU, "SubType could not be set");


        // Update Property data
        pPropData->m_FirmwareVersion = pDvcData->m_DevInfo.m_FirmwareVersion;

        // Initialize ports (SW)
        Status = DtuPortsInit(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    // Initialise demodulator
    Status = DtuDemodInit(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;
    // Perform IO configurations
    Status = DtuIoConfigInit(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;
    // TODO remove typenumber check
    if (pDvcData->m_DevInfo.m_TypeNumber == 315)
    {
        // Set default firmware variant
        Int FwVariant = 0;
        DtuIoConfigValue  CfgValue;
        DtuNonIpIoConfigGet(&pDvcData->m_pNonIpPorts[0], DT_IOCONFIG_PWRMODE, &CfgValue);
        if (CfgValue.m_Value == DT_IOCONFIG_MODHQ)
            FwVariant = 2;
        pDvcData->m_PropData.m_FirmwareVariant = FwVariant;
        pDvcData->m_DevInfo.m_FirmwareVariant = FwVariant;
    }

    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
    {
        Int  i;
        Int  PortIndex;
        for (i=0; i<pDvcData->m_NumNonIpPorts && DT_SUCCESS(Status); i++)
        {
            if (DT_SUCCESS(DtuGetPortIndexNonIp(pDvcData, i, &PortIndex)))
            {
                DtuNonIpPort*  pNonIpPort = &pDvcData->m_pNonIpPorts[i];
                pNonIpPort->m_State = DTU3_STATE_IDLE;
                DtEventReset(&pNonIpPort->m_StateChanged);
                DtEventReset(&pNonIpPort->m_StateChangeCmpl);

                if (pDvcData->m_DevInfo.m_TypeNumber == 351)
                {
                    pNonIpPort->m_State = DTU3_STATE_DET_VIDSTD;
                    pNonIpPort->m_DetVidStd = DT_VIDSTD_UNKNOWN;
                    pNonIpPort->m_InitRxMode = 0;
                    pNonIpPort->m_AllowRxModeChanges = FALSE;
                    pNonIpPort->m_DataLoss = FALSE;
                }

                if (pDvcData->m_DevInfo.m_UsbSpeed == 2)
                    Status = DtThreadStart(&pNonIpPort->m_DataThread);
            }
        }
    }
    if (!DT_SUCCESS(Status))
        return Status;

    // First powerup is done
    pDvcData->m_InitialPowerup = FALSE;

    // Set power up event
    DtuEventsSet(pDvcData, NULL, DTU_EVENT_TYPE_POWER, DTU_EVENT_VALUE1_POWER_UP, 0);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Dtu3Shutdown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Execute shutdown scenario for USB3 devices
//
void  Dtu3Shutdown(DtuDeviceData* pDvcData)
{
#ifdef WINBUILD
    // Check if the device is still connected to the USB bus and if not, return
    if (!NT_SUCCESS(WdfUsbTargetDeviceIsConnectedSynchronous(pDvcData->m_Device.m_UsbDevice)))
        return;
#endif

    // Reset FX3
    DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_PNP_CMD,
                            DTU_PNP_CMD_RESET, DTU_RESET_DVC_STATE, DT_USB_HOST_TO_DEVICE,
                            NULL, 0, NULL, MAX_USB_REQ_TIMEOUT);
    // Turn off FPGA
    DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_PNP_CMD,
                          DTU_PNP_CMD_DVC_POWER, DTU_DVC_POWER_OFF, DT_USB_HOST_TO_DEVICE,
                          NULL, 0, NULL, MAX_USB_REQ_TIMEOUT);

    if (pDvcData->m_DevInfo.m_TypeNumber == 315)
        // DTU-315 specific: Disable LED
        Dtu3RegWrite(pDvcData, DTU3_FX3_BLOCK_OFFSET, &FwbFx3.LedCtrl, 0);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDevicePowerDown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called when the device must leave the active (D0) state. It
// should stop receive / transmit threads, store the device state and put the device in
// low power mode.
//
DtStatus  DtuDevicePowerDown(DtuDeviceData* pDvcData, Int TargetState)
{
    Int  i;
    DtDbgOut(MAX, DTU, "Start");
  
    // Set power down pending to prevent new IOCTL's
    DtFastMutexAcquire(&pDvcData->m_FileHandleInfoMutex);
    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
    {
        if (pDvcData->m_FileHandleInfo[i].m_pHandle != NULL) 
            pDvcData->m_FileHandleInfo[i].m_PowerDownPending = TRUE;
    }
    DtFastMutexRelease(&pDvcData->m_FileHandleInfoMutex);

    // Set power down event
    DtuEventsSet(pDvcData, NULL, DTU_EVENT_TYPE_POWER, DTU_EVENT_VALUE1_POWER_DOWN, 0);

    if (pDvcData->m_DevInfo.m_TypeNumber>=300 && pDvcData->m_DevInfo.m_TypeNumber<400)
    {
        for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
        {
            if (!pDvcData->m_pNonIpPorts[i].m_DataThread.m_Started)
                continue;
            pDvcData->m_pNonIpPorts[i].m_NextState = DTU3_STATE_EXIT;
            DtEventSet(&pDvcData->m_pNonIpPorts[i].m_StateChanged);
            DtEventWaitUnInt(&pDvcData->m_pNonIpPorts[i].m_StateChangeCmpl, -1);
            DtThreadStop(&pDvcData->m_pNonIpPorts[i].m_DataThread);
        }

        if (TargetState == DT_STATE_D1)
        {
            // Go to sleep
            // Turn off FPGA
            DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_PNP_CMD,
                          DTU_PNP_CMD_DVC_POWER, DTU_DVC_POWER_OFF, DT_USB_HOST_TO_DEVICE,
                          NULL, 0, NULL, MAX_USB_REQ_TIMEOUT);

            if (pDvcData->m_DevInfo.m_TypeNumber == 315)
                // DTU-315 specific: Disable LED
                Dtu3RegWrite(pDvcData, DTU3_FX3_BLOCK_OFFSET, &FwbFx3.LedCtrl, 0);
        }
        else if (TargetState == DT_STATE_D3)
        {
            if (pDvcData->m_BootState!=DTU_BOOTSTATE_COLD && 
                                        pDvcData->m_BootState!=DTU_BOOTSTATE_FACTORY_COLD)
                Dtu3Shutdown(pDvcData);
        }
    }

    DtDbgOut(MAX, DTU, "Exit");

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDeviceExit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called by the IAL when the device is being disconnected. This
// function should undo everything done by DeviceInitPost and DeviceInit.
//
void  DtuDeviceExit(DtuDeviceData* pDvcData)
{
    // Cleanup ports
    DtuPortsCleanUp(pDvcData);

    // Cleanup port structs
    DtuCleanupPortStructs(pDvcData);
    
    // Cleanup I2C module
    DtuI2cCleanup(pDvcData);

    // Cleanup VPD module
    DtuVpdCleanup(pDvcData);

    // Cleanup Properties module
    DtPropertiesCleanup(&pDvcData->m_PropData);

    // Cleanup events
    DtuEventsCleanup(pDvcData);
}



//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDeviceClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called by the IAL when an application closes a device which it has
// opened before. All allocated resources which were not deallocated synchronously must
// be checked by this function. If a resource was not deallocated yet it must be done by
// this function. Also exclusive access for a channel held by the current handle to be
// closed will be released.
//
DtStatus  DtuDeviceClose(DtuDeviceData* pDvcData, DtFileObject* pFile)
{
    Int  i;

    // Unlock all recursive I2C locks for this file object
    if (pDvcData->m_I2c.m_IsSupported)
        DtuI2cUnlock(pDvcData, pFile, TRUE);

    // Remove file handle from the filehandle info array
    DtFastMutexAcquire(&pDvcData->m_FileHandleInfoMutex);
    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
    {
        if (pDvcData->m_FileHandleInfo[i].m_pHandle == DtFileGetHandle(pFile))
        {
            pDvcData->m_FileHandleInfo[i].m_pHandle = NULL;
            break;
        }
    }
    DtFastMutexRelease(&pDvcData->m_FileHandleInfoMutex);

    // TODOTD: Replace TypeNumber==315 with property
    // Switch off power when not in use
    if (pDvcData->m_DevInfo.m_TypeNumber == 315)
    {
        DtFastMutexAcquire(&pDvcData->m_ExclAccessMutex);
        if (!DtuDeviceIsInuse(pDvcData))
        {
            DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_PNP_CMD,
                          DTU_PNP_CMD_DVC_POWER, DTU_DVC_POWER_OFF, DT_USB_HOST_TO_DEVICE,
                          NULL, 0, NULL, MAX_USB_REQ_TIMEOUT);
        }
        DtFastMutexRelease(&pDvcData->m_ExclAccessMutex);
    }

    // Unregister events
    DtuEventsUnregister(pDvcData, pFile); 

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDeviceIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called for every IO request an application sends to the device
// driver. It handles all the functional requests.
//
DtStatus  DtuDeviceIoctl(
    DtuDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoctlObject*  pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pIoctlStr = NULL;
    UInt  InReqSize = 0;
    UInt  OutReqSize = 0;
    DtuIoctlInputData*  pInBuf = (DtuIoctlInputData*)pIoctl->m_pInputBuffer;
    DtuIoctlOutputData*  pOutBuf = (DtuIoctlOutputData*)pIoctl->m_pOutputBuffer;
    DtPropertyData*  pPropData = &pDvcData->m_PropData;

    Int  i;
    Bool  PowerDownPending = FALSE;

    DT_ASSERT(!pDvcData->m_InitialPowerup);

    // Get powerdown pending flag for this file handle from the filehandle info array
    DtFastMutexAcquire(&pDvcData->m_FileHandleInfoMutex);
    for (i=0; i<MAX_NUM_FILE_HANDLES; i++)
    {
        if (pDvcData->m_FileHandleInfo[i].m_pHandle == DtFileGetHandle(pFile))
        {
            PowerDownPending = pDvcData->m_FileHandleInfo[i].m_PowerDownPending;
            break;
        }
    }
    DtFastMutexRelease(&pDvcData->m_FileHandleInfoMutex);

    // Get minimal input/output buffer sizes
    switch (pIoctl->m_IoctlCode)
    {
    case DTU_IOCTL_GET_PROPERTY:
        pIoctlStr = "DTU_IOCTL_GET_PROPERTY";
        InReqSize = sizeof(DtuIoctlGetPropertyInput);
        OutReqSize = sizeof(DtuIoctlGetPropertyOutput);
        break;
    case DTU_IOCTL_GET_DEV_INFO:
        pIoctlStr = "DTU_IOCTL_GET_DEV_INFO";
        InReqSize = 0;
        OutReqSize = sizeof(DtuIoctlGetDevInfoOutput);
        break;
    case DTU_IOCTL_VPD_CMD:
        pIoctlStr = "DTU_IOCTL_VPD_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTU_IOCTL_GET_DRIVER_VERSION:
        pIoctlStr = "DTU_IOCTL_GET_DRIVER_VERSION";
        InReqSize = 0;
        OutReqSize = sizeof(DtuIoctlGetDriverVersionOutput);
        break;
    case DTU_IOCTL_I2C_CMD:
        pIoctlStr = "DTU_IOCTL_I2C_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTU_IOCTL_REG_READ:
        pIoctlStr = "DTU_IOCTL_REG_READ";
        InReqSize = sizeof(DtuIoctlRegReadInput);
        OutReqSize = sizeof(DtuIoctlRegReadOutput);
        break;
    case DTU_IOCTL_REG_WRITE_MASKED:
        pIoctlStr = "DTU_IOCTL_REG_WRITE_MASKED";
        InReqSize = sizeof(DtuIoctlRegWriteMaskedInput);
        OutReqSize = 0;
        break;
    case DTU_IOCTL_GET_EVENT:
        pIoctlStr = "DTU_IOCTL_GET_EVENT";
        InReqSize = 0;
        OutReqSize = sizeof(DtuIoctlGetEventOutput);
        break;
    case DTU_IOCTL_REGISTER_EVENTS:
        pIoctlStr = "DTU_IOCTL_REGISTER_EVENTS";
        InReqSize = sizeof(DtuIoctlRegisterEventsInput);
        OutReqSize = 0;
        break;
    case DTU_IOCTL_UNREGISTER_EVENTS:
        pIoctlStr = "DTU_IOCTL_UNREGISTER_EVENTS";
        InReqSize = 0;
        OutReqSize = 0;
        break;
    case DTU_IOCTL_READ_DATA:
        pIoctlStr = "DTU_IOCTL_READ_DATA";
        InReqSize = sizeof(DtuIoctlReadDataInput);
#ifdef WINBUILD
        OutReqSize = 0;
#else
        OutReqSize = sizeof(DtuIoctlReadDataOutput);
#endif
        break;
    case DTU_IOCTL_WRITE_DATA:
        pIoctlStr = "DTU_IOCTL_WRITE_DATA";
        InReqSize = sizeof(DtuIoctlWriteDataInput);
        OutReqSize = 0;
        break;
    case DTU_IOCTL_SH_BUF_CMD:
        pIoctlStr = "DTU_IOCTL_SH_BUF_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTU_IOCTL_GET_IOCONFIG:
        pIoctlStr = "DTU_IOCTL_GET_IOCONFIG";
        InReqSize = OFFSETOF(DtuIoctlGetIoConfigInput, m_IoCfgId);
        OutReqSize = OFFSETOF(DtuIoctlGetIoConfigOutput, m_IoCfgValue);
        break;
    case DTU_IOCTL_SET_IOCONFIG:
        pIoctlStr = "DTU_IOCTL_SET_IOCONFIG";
        InReqSize = OFFSETOF(DtuIoctlSetIoConfigInput, m_IoConfig);
        OutReqSize = 0;
        break;
    case DTU_IOCTL_NONIP_CMD:
        pIoctlStr = "DTU_IOCTL_NONIP_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTU_IOCTL_NONIP_RX_CMD:
        pIoctlStr = "DTU_IOCTL_NONIP_RX_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTU_IOCTL_NONIP_TX_CMD:
        pIoctlStr = "DTU_IOCTL_NONIP_TX_CMD";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTU_IOCTL_RESET_SOFT:
        pIoctlStr = "DTU_IOCTL_RESET_SOFT";
        InReqSize = 0; // Checked later
        OutReqSize = 0; // Checked later
        break;
    case DTU_IOCTL_GET_TABLE:
        pIoctlStr = "DTU_IOCTL_GET_TABLE";
        InReqSize = sizeof(DtuIoctlGetTableInput);
        OutReqSize = sizeof(DtuIoctlGetTableOutput);
        break;
    case DTU_IOCTL_GET_TABLE2:
        pIoctlStr = "DTU_IOCTL_GET_TABLE2";
        InReqSize = sizeof(DtuIoctlGetTable2Input);
        OutReqSize = sizeof(DtuIoctlGetTableOutput);
        break;
    case DTU_IOCTL_GET_STR_PROPERTY:
        pIoctlStr = "DTU_IOCTL_GET_STR_PROPERTY";
        InReqSize = sizeof(DtuIoctlGetStrPropertyInput);
        OutReqSize = sizeof(DtuIoctlGetStrPropertyOutput);
        break;
    case DTU_IOCTL_GET_DEV_SUBTYPE:
        pIoctlStr = "DTU_IOCTL_GET_DEV_SUBTYPE";
        InReqSize = 0;
        OutReqSize = sizeof(DtuIoctlGetDevSubTypeOutput);
        break;
    case DTU_IOCTL_GET_PROPERTY2:
        pIoctlStr = "DTU_IOCTL_GET_PROPERTY2";
        InReqSize = sizeof(DtuIoctlGetProperty2Input);
        OutReqSize = sizeof(DtuIoctlGetPropertyOutput);
        break;
    case DTU_IOCTL_REENUMERATE:
        pIoctlStr = "DTU_IOCTL_REENUMERATE";
        InReqSize = 0;
        OutReqSize = 0;
        break;
    case DTU_IOCTL_VENDOR_REQUEST:
        pIoctlStr = "DTU_IOCTL_VENDOR_REQUEST";
        InReqSize = sizeof(DtuIoctlVendorRequestInput);
        OutReqSize = sizeof(DtuIoctlVendorRequestOutput);
        break;
    case DTU_IOCTL_GET_STATE_FLAGS:
        pIoctlStr = "DTU_IOCTL_GET_STATE_FLAGS";
        InReqSize = sizeof(DtuIoctlGetStateFlagsInput);
        OutReqSize = sizeof(DtuIoctlGetStateFlagsOutput);
        break;
    case DTU_IOCTL_TRIGGER_WATCHDOG:
        pIoctlStr = "DTU_IOCTL_TRIGGER_WATCHDOG";
        InReqSize = 0;
        OutReqSize = 0;
        break;
    case DTU_IOCTL_SET_RX_MODE:
        pIoctlStr = "DTU_IOCTL_SET_RX_MODE";
        InReqSize = sizeof(DtuIoctlSetRxModeInput);
        OutReqSize = sizeof(DtuIoctlSetRxModeOutput);
        break;
    case DTU_IOCTL_UPLOAD_FPGA_FW:
        pIoctlStr = "DTU_IOCTL_UPLOAD_FPGA_FW";
        InReqSize = sizeof(DtuIoctlUploadFpgaFwInput);
        OutReqSize = 0;
        break;
    case DTU_IOCTL_GET_DATA_BUF_SIZE:
        pIoctlStr = "DTU_IOCTL_GET_DATA_BUF_SIZE";
        InReqSize = sizeof(DtuIoctlGetDataBufSizeInput);
        OutReqSize = sizeof(DtuIoctlGetDataBufSizeOutput);
        break;
    case DTU_IOCTL_SET_TX_CTRL:
        pIoctlStr = "DTU_IOCTL_SET_TX_CTRL";
        InReqSize = sizeof(DtuIoctlSetTxCtrlInput);
        OutReqSize = 0;
        break;
    case DTU_IOCTL_UPLOAD_FPGA_FW_VARIANT:
        pIoctlStr = "DTU_IOCTL_UPLOAD_FPGA_FW_VARIANT";
        InReqSize = sizeof(DtuIoctlUploadFpgaFwVariantInput);
        OutReqSize = 0;
        break;
    case DTU_IOCTL_POWER_CTRL:
        pIoctlStr = "DTU_IOCTL_POWER_CTRL";
        InReqSize = sizeof(DtuIoctlPowerCtrlInput);
        OutReqSize = 0;
        break;
    case DTU_IOCTL_REG_WRITE_BULK:
        pIoctlStr = "DTU_IOCTL_REG_WRITE_BULK";
        InReqSize = sizeof(DtuIoctlRegWriteBulkInput);
        OutReqSize = 0;
        break;
    default:
        Status = DT_STATUS_NOT_SUPPORTED;
        break;
    }

    if (PowerDownPending)
    {
        // Only skip channel specific IOCTL's
        switch (pIoctl->m_IoctlCode) 
        {
            case DTU_IOCTL_REG_READ:
            case DTU_IOCTL_REG_WRITE_MASKED:
            case DTU_IOCTL_READ_DATA:
            case DTU_IOCTL_WRITE_DATA:
            case DTU_IOCTL_NONIP_RX_CMD:
            case DTU_IOCTL_NONIP_TX_CMD:
                DtDbgOut(ERR, DTU, "%s: Skipping IOCTL because powerdown  occured!",
                                                                               pIoctlStr);
                return DT_STATUS_POWERDOWN;
        } 
    }

     // Check if buffer sizes satisfy
    if (DT_SUCCESS(Status))
    {
        if (pIoctl->m_InputBufferSize < InReqSize) 
        {
            DtDbgOut(ERR, DTU, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) INPUT BUFFER TOO SMALL!",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
            Status = DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize) 
        {
            DtDbgOut(ERR, DTU, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) OUTPUT BUFFER TOO"
                                          " SMALL!",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
            Status = DT_STATUS_INVALID_PARAMETER;
        }
        if (DT_SUCCESS(Status))
            DtDbgOut(MAX, DTU, "%s: In=%d (Rq=%d) Out=%d (Rq=%d)",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    // Handle IOCTLS
    if (DT_SUCCESS(Status))
    {
        switch (pIoctl->m_IoctlCode)
        {
        case DTU_IOCTL_GET_PROPERTY:

            // Get for specific type or for the attached devices
            if (pInBuf->m_GetProperty.m_TypeNumber == -1)
            {
                DtPropertyValue  Value;
                DtPropertyValueType  Type;
                DtPropertyScope  Scope;
                
                // Get the property for the current device
                Status = DtPropertiesGet(pPropData, pInBuf->m_GetProperty.m_Name,
                                                    pInBuf->m_GetProperty.m_PortIndex,
                                                    &Value, &Type, &Scope,
                                                    0, 0, 0);
                pOutBuf->m_GetProperty.m_Value = Value;
                pOutBuf->m_GetProperty.m_Type = Type;
                pOutBuf->m_GetProperty.m_Scope = Scope;
            }
            else
            {
                DtPropertyValue  Value;
                DtPropertyValueType  Type;
                DtPropertyScope  Scope;

                // Property for a specific type was requested
                Status = DtPropertiesGetForType("DTU",
                                                 pInBuf->m_GetProperty.m_TypeNumber,
                                                 0, // no USB devs with sub-devices yet
                                                 pInBuf->m_GetProperty.m_HardwareRevision, 
                                                 pInBuf->m_GetProperty.m_FirmwareVersion,
                                                 -1,
                                                 pInBuf->m_GetProperty.m_Name,
                                                 pInBuf->m_GetProperty.m_PortIndex,
                                                 &Value, &Type, &Scope,
                                                 0, 0, 0);
                pOutBuf->m_GetProperty.m_Value = Value;
                pOutBuf->m_GetProperty.m_Type = Type;
                pOutBuf->m_GetProperty.m_Scope = Scope;
            }

            if (DT_SUCCESS(Status))
            {
                DT_ASSERT((pOutBuf->m_GetProperty.m_Scope&PROPERTY_SCOPE_DTAPI) ==
                                                                    PROPERTY_SCOPE_DTAPI);
            }
            break;
         case DTU_IOCTL_GET_DEV_INFO:
            pOutBuf->m_GetDevInfo.m_ProductId = pDvcData->m_DevInfo.m_ProductId;
            pOutBuf->m_GetDevInfo.m_VendorId = pDvcData->m_DevInfo.m_VendorId;
            pOutBuf->m_GetDevInfo.m_UsbAddress = pDvcData->m_DevInfo.m_UsbAddress; 
            pOutBuf->m_GetDevInfo.m_TypeNumber = pDvcData->m_DevInfo.m_TypeNumber;
            pOutBuf->m_GetDevInfo.m_HardwareRevision =
                                                   pDvcData->m_DevInfo.m_HardwareRevision; 
            pOutBuf->m_GetDevInfo.m_FirmwareVersion =
                                                    pDvcData->m_DevInfo.m_FirmwareVersion;
            pOutBuf->m_GetDevInfo.m_FirmwareVariant =
                                                    pDvcData->m_DevInfo.m_FirmwareVariant;
            pOutBuf->m_GetDevInfo.m_Serial = pDvcData->m_DevInfo.m_Serial;
            pOutBuf->m_GetDevInfo.m_UsbSpeed = pDvcData->m_DevInfo.m_UsbSpeed;
            break;
        case DTU_IOCTL_VPD_CMD:
            Status = DtuVpdIoctl(pDvcData, pIoctl);
            break;
        case DTU_IOCTL_GET_DRIVER_VERSION:
            pOutBuf->m_GetDriverVersion.m_Major = DTU_VERSION_MAJOR;
            pOutBuf->m_GetDriverVersion.m_Minor = DTU_VERSION_MINOR;
            pOutBuf->m_GetDriverVersion.m_Micro = DTU_VERSION_MICRO;
            pOutBuf->m_GetDriverVersion.m_Build = DTU_VERSION_BUILD;
            break;
        case DTU_IOCTL_I2C_CMD:
            Status = DtuI2cIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTU_IOCTL_REG_READ:
            if (pDvcData->m_DevInfo.m_TypeNumber == 315)
                // Only supported for DTU-315 for now
                Status = Dtu3RegReadRaw(pDvcData, pInBuf->m_RegRead.m_RegAddr, 
                                                             &pOutBuf->m_RegRead.m_Value);
            else
                Status = DtuRegRead(pDvcData, pInBuf->m_RegRead.m_RegAddr, 
                                                             &pOutBuf->m_RegRead.m_Value);
            break;
        case DTU_IOCTL_REG_WRITE_MASKED:
            if (pDvcData->m_DevInfo.m_TypeNumber >= 315)
                // Only supported for DTU-315 for now
                Status = Dtu3RegWriteMaskedRaw(pDvcData, pInBuf->m_RegWriteMasked.m_RegAddr,
                                                     pInBuf->m_RegWriteMasked.m_FieldMask,
                                                    pInBuf->m_RegWriteMasked.m_FieldShift,
                                                   pInBuf->m_RegWriteMasked.m_FieldValue);
            else
                Status = DtuRegWriteMasked(pDvcData, pInBuf->m_RegWriteMasked.m_RegAddr,
                                                     pInBuf->m_RegWriteMasked.m_FieldMask,
                                                    pInBuf->m_RegWriteMasked.m_FieldShift,
                                                   pInBuf->m_RegWriteMasked.m_FieldValue);
            break;
        case DTU_IOCTL_GET_EVENT:
            Status = DtuEventsGet(pDvcData, pFile, &pOutBuf->m_GetEvent.m_EventType,
                                                    &pOutBuf->m_GetEvent.m_Value1,
                                                    &pOutBuf->m_GetEvent.m_Value2, FALSE);
            break;
        case DTU_IOCTL_REGISTER_EVENTS:
            Status = DtuEventsRegister(pDvcData, pFile,
                                                pInBuf->m_RegisterEvents.m_EventTypeMask);
            break;
        case DTU_IOCTL_UNREGISTER_EVENTS:
            DtuEventsUnregister(pDvcData, pFile);
            break;
        case DTU_IOCTL_READ_DATA:
            {
                UInt8*  pBuffer = NULL;
                Int  ReqSize = 0;
                Int  NumRead = 0;
#if defined(WINBUILD)
                PMDL  pMdl;
                NTSTATUS  NtStatus;
                // Retrieve MDL and buffer from request object
                NtStatus = WdfRequestRetrieveOutputWdmMdl(pIoctl->m_WdfRequest, &pMdl);
                if (NtStatus != STATUS_SUCCESS)
                {
                    DtDbgOut(ERR, DTU, "WdfRequestRetrieveOutputWdmMdl error: %08x", 
                                                                                NtStatus);
                    Status = DT_STATUS_OUT_OF_RESOURCES;
                }
                if (DT_SUCCESS(Status))
                {
                    pBuffer = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);
                    if (pBuffer == NULL)
                        Status = DT_STATUS_OUT_OF_MEMORY;
                    else
                        ReqSize = MmGetMdlByteCount(pMdl);
                }
#else  // LINBUILD
                ReqSize = pInBuf->m_ReadData.m_NumBytesToRead;
#if defined(LIN32)
                pBuffer = (char*)(UInt32)pInBuf->m_ReadData.m_BufferAddr;
#else
                pBuffer = (char*)(UInt64)pInBuf->m_ReadData.m_BufferAddr;
#endif
#endif
                if (DT_SUCCESS(Status))
                    Status = DtuRead(pDvcData, pInBuf->m_ReadData.m_PortIndex,
                                                              pBuffer, ReqSize, &NumRead);
                
#if defined(WINBUILD)
                if (DT_SUCCESS(Status))
                    pIoctl->m_OutputBufferBytesWritten = NumRead;
                else
                    pIoctl->m_OutputBufferBytesWritten = 0;
#else // LINBUILD
                // For Linux, the pIoctl->m_OutputBufferBytesWritten contains the output
                // struct size
                if (DT_SUCCESS(Status))
                    pOutBuf->m_ReadData.m_NumBytesRead = NumRead;
                else
                    pOutBuf->m_ReadData.m_NumBytesRead = 0;
#endif
            }
            break;
        case DTU_IOCTL_WRITE_DATA:
            {
                UInt8*  pBuffer = NULL;
                Int  Size = 0;
#if defined(WINBUILD)
                PMDL  pMdl;
                NTSTATUS  NtStatus;
                // Retrieve MDL and buffer from request object
                NtStatus = WdfRequestRetrieveOutputWdmMdl(pIoctl->m_WdfRequest, &pMdl);
                if (NtStatus != STATUS_SUCCESS)
                {
                    DtDbgOut(ERR, DTU, "WdfRequestRetrieveOutputWdmMdl error: %08x", 
                                                                                NtStatus);
                    Status = DT_STATUS_OUT_OF_RESOURCES;
                }
                if (DT_SUCCESS(Status))
                {
                    pBuffer = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);
                    if (pBuffer == NULL)
                        Status = DT_STATUS_OUT_OF_MEMORY;
                    else
                        Size = MmGetMdlByteCount(pMdl);
                }
#else // LINBUILD
                Size = pInBuf->m_WriteData.m_NumBytesToWrite;
#if defined(LIN32)
                pBuffer = (char*)(UInt32)pInBuf->m_WriteData.m_BufferAddr;
#else
                pBuffer = (char*)(UInt64)pInBuf->m_WriteData.m_BufferAddr;
#endif
#endif
                if (DT_SUCCESS(Status))
                    Status = DtuWrite(pDvcData, pInBuf->m_WriteData.m_PortIndex, 
                                                                           pBuffer, Size);
            }
            break;

        case DTU_IOCTL_SH_BUF_CMD:
            Status = DtuShBufferIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTU_IOCTL_GET_IOCONFIG:
            // Update required buffer sizes
            InReqSize += pInBuf->m_GetIoConfig.m_IoConfigCount * sizeof(DtIoConfigId);
            OutReqSize += pInBuf->m_GetIoConfig.m_IoConfigCount * sizeof(DtIoConfigValue);

            // Check input+output parameter size
            if (pIoctl->m_InputBufferSize < InReqSize)
            {
                DtDbgOut(ERR, DTU, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) OUTPUT BUFFER TOO"
                                          " SMALL!",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
                Status = DT_STATUS_INVALID_PARAMETER;
            }
            // check output buffer size
            else if (pIoctl->m_OutputBufferSize < OutReqSize)
            {
                DtDbgOut(ERR, DTU, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) OUTPUT BUFFER TOO"
                                          " SMALL!",
                                          pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                          pIoctl->m_OutputBufferSize, OutReqSize);
                Status = DT_STATUS_INVALID_PARAMETER;

            }
            else
            {
                Status = DtuIoConfigGet(pDvcData, pFile, 
                                                   pInBuf->m_GetIoConfig.m_IoCfgId,
                                                   pOutBuf->m_GetIoConfig.m_IoCfgValue,
                                                   pInBuf->m_GetIoConfig.m_IoConfigCount);
                // Set actual number of bytes written/returned
                if (DT_SUCCESS(Status))
                    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

            }
            break;
        case DTU_IOCTL_SET_IOCONFIG:
            if (pIoctl->m_InputBufferSize < sizeof(DtuIoctlSetIoConfigInput) +
                                pInBuf->m_SetIoConfig.m_IoConfigCount*sizeof(DtIoConfigPars))
                Status = DT_STATUS_INVALID_PARAMETER;
            else
                Status = DtuIoConfigSet(pDvcData, pFile,
                                                   pInBuf->m_SetIoConfig.m_IoConfig,
                                                   pInBuf->m_SetIoConfig.m_IoConfigCount);
            break;
        case DTU_IOCTL_NONIP_CMD:
            Status = DtuNonIpIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTU_IOCTL_NONIP_RX_CMD:
            Status = DtuNonIpRxIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTU_IOCTL_NONIP_TX_CMD:
            Status = DtuNonIpTxIoctl(pDvcData, pFile, pIoctl);
            break;
        case DTU_IOCTL_RESET_SOFT:
            Status = DtuDvcReset(pDvcData);
            break;
        case DTU_IOCTL_GET_TABLE: 
            Status = DtTableGet(pPropData, pInBuf->m_GetTable.m_Name,
                     pInBuf->m_GetTable.m_PortIndex, pInBuf->m_GetTable.m_MaxNumEntries,
                     &pOutBuf->m_GetTable.m_NumEntries, pOutBuf->m_GetTable.m_TableEntry,
               pIoctl->m_OutputBufferSize-OFFSETOF(DtuIoctlGetTableOutput, m_TableEntry));
            if (DT_SUCCESS(Status)) {
                if (pInBuf->m_GetTable.m_MaxNumEntries>=pOutBuf->m_GetTable.m_NumEntries)
                    pIoctl->m_OutputBufferBytesWritten += 
                                  pOutBuf->m_GetTable.m_NumEntries * sizeof(DtTableEntry);
            }
            break;
        case DTU_IOCTL_GET_TABLE2:
            if (pInBuf->m_GetTable2.m_TypeNumber==-1 && 
                                                pInBuf->m_GetTable2.m_FirmwareVariant==-1)
            {
                Status = DtTableGet(
                            &pDvcData->m_PropData,
                            pInBuf->m_GetTable2.m_Name,
                            pInBuf->m_GetTable2.m_PortIndex,
                            pInBuf->m_GetTable2.m_MaxNumEntries,
                            &pOutBuf->m_GetTable.m_NumEntries,
                            pOutBuf->m_GetTable.m_TableEntry,
                            pIoctl->m_OutputBufferSize-OFFSETOF(DtuIoctlGetTableOutput, 
                                                                           m_TableEntry));
            }
            else  if (pInBuf->m_GetTable2.m_TypeNumber==-1)
            {
                // Get table for a specific firmware variant
                Status = DtTableGetForType(
                            "DTU",
                            pDvcData->m_PropData.m_TypeNumber,
                            0,  // No USB devs with sub-devices yet
                            pDvcData->m_PropData.m_HardwareRevision,
                            pDvcData->m_PropData.m_FirmwareVersion,
                            pInBuf->m_GetTable2.m_FirmwareVariant,
                            pInBuf->m_GetTable2.m_Name,
                            pInBuf->m_GetTable2.m_PortIndex,
                            pInBuf->m_GetTable2.m_MaxNumEntries,
                            &pOutBuf->m_GetTable.m_NumEntries,
                            pOutBuf->m_GetTable.m_TableEntry,
                            pIoctl->m_OutputBufferSize-OFFSETOF(DtuIoctlGetTableOutput, 
                                                                           m_TableEntry));
            }
            else
            {
                // Get table for a specific device
                Status = DtTableGetForType(
                            "DTU",
                            pInBuf->m_GetTable2.m_TypeNumber,
                            0,  // No USB devs with sub-devices yet
                            pInBuf->m_GetTable2.m_HardwareRevision,
                            pInBuf->m_GetTable2.m_FirmwareVersion,
                            pInBuf->m_GetTable2.m_FirmwareVariant,
                            pInBuf->m_GetTable2.m_Name,
                            pInBuf->m_GetTable2.m_PortIndex,
                            pInBuf->m_GetTable2.m_MaxNumEntries,
                            &pOutBuf->m_GetTable.m_NumEntries,
                            pOutBuf->m_GetTable.m_TableEntry,
                            pIoctl->m_OutputBufferSize-OFFSETOF(DtuIoctlGetTableOutput, 
                                                                           m_TableEntry));
            }
            if (DT_SUCCESS(Status)) 
            {
                if (pInBuf->m_GetTable2.m_MaxNumEntries>=pOutBuf->m_GetTable.m_NumEntries)
                    pIoctl->m_OutputBufferBytesWritten += 
                                  pOutBuf->m_GetTable.m_NumEntries * sizeof(DtTableEntry);
            }
            break;
        case DTU_IOCTL_GET_STR_PROPERTY:
            // Get for specific type or for the attached devices
            if (pInBuf->m_GetProperty.m_TypeNumber == -1)
            {
                // Get the property for the current device
                Status = DtPropertiesStrGet(pPropData, pInBuf->m_GetStrProperty.m_Name,
                                                     pInBuf->m_GetStrProperty.m_PortIndex,
                                                     pOutBuf->m_GetStrProperty.m_Str,
                                                     &pOutBuf->m_GetStrProperty.m_Scope,
                                                     0, 0, 0);
            }
            else
            {
                // Property for a specific type was requested
                Status = DtPropertiesStrGetForType("DTU",
                                              pInBuf->m_GetStrProperty.m_TypeNumber,
                                              0, // no USB devs with sub-devices yet
                                              pInBuf->m_GetStrProperty.m_HardwareRevision, 
                                              pInBuf->m_GetStrProperty.m_FirmwareVersion,
                                              -1,
                                              pInBuf->m_GetStrProperty.m_Name,
                                              pInBuf->m_GetStrProperty.m_PortIndex,
                                              pOutBuf->m_GetStrProperty.m_Str,
                                              &pOutBuf->m_GetStrProperty.m_Scope,
                                              0, 0, 0);
            }

            if (DT_SUCCESS(Status))
            {
                DT_ASSERT((pOutBuf->m_GetProperty.m_Scope&PROPERTY_SCOPE_DTAPI) ==
                                                                    PROPERTY_SCOPE_DTAPI);
            }
            break;
        case DTU_IOCTL_GET_DEV_SUBTYPE:
            pOutBuf->m_GetDevSubType.m_SubType = pDvcData->m_DevInfo.m_SubType;
            break;

        case DTU_IOCTL_GET_PROPERTY2:

            // Get for specific type or for the attached devices
            if (pInBuf->m_GetProperty.m_TypeNumber == -1)
            {
                DtPropertyValue  Value;
                DtPropertyValueType  Type;
                DtPropertyScope  Scope;
                
                // Get the property for the current device
                Status = DtPropertiesGet(pPropData, pInBuf->m_GetProperty2.m_Name,
                                                    pInBuf->m_GetProperty2.m_PortIndex,
                                                    &Value, &Type, &Scope,
                                                    pInBuf->m_GetProperty2.m_DtapiMaj,
                                                    pInBuf->m_GetProperty2.m_DtapiMin,
                                                    pInBuf->m_GetProperty2.m_DtapiBugfix);
                pOutBuf->m_GetProperty.m_Value = Value;
                pOutBuf->m_GetProperty.m_Type = Type;
                pOutBuf->m_GetProperty.m_Scope = Scope;
            }
            else
            {
                DtPropertyValue  Value;
                DtPropertyValueType  Type;
                DtPropertyScope  Scope;

                // Property for a specific type was requested
                Status = DtPropertiesGetForType("DTU",
                                                pInBuf->m_GetProperty2.m_TypeNumber,
                                                0, // no USB devs with sub-devices yet
                                                pInBuf->m_GetProperty2.m_HardwareRevision, 
                                                pInBuf->m_GetProperty2.m_FirmwareVersion,
                                                -1,
                                                pInBuf->m_GetProperty2.m_Name,
                                                pInBuf->m_GetProperty2.m_PortIndex,
                                                &Value, &Type, &Scope,
                                                pInBuf->m_GetProperty2.m_DtapiMaj,
                                                pInBuf->m_GetProperty2.m_DtapiMin,
                                                pInBuf->m_GetProperty2.m_DtapiBugfix);
                pOutBuf->m_GetProperty.m_Value = Value;
                pOutBuf->m_GetProperty.m_Type = Type;
                pOutBuf->m_GetProperty.m_Scope = Scope;
            }

            if (DT_SUCCESS(Status))
            {
                DT_ASSERT((pOutBuf->m_GetProperty.m_Scope&PROPERTY_SCOPE_DTAPI) ==
                                                                    PROPERTY_SCOPE_DTAPI);
            }
            break;
        case DTU_IOCTL_REENUMERATE:
            Dtu3Shutdown(pDvcData);
            DtUsbVendorRequest(&pDvcData->m_Device, NULL, DTU_USB3_PNP_CMD,
                           DTU_PNP_CMD_RESET, DTU_RESET_BOOTLOADER, DT_USB_HOST_TO_DEVICE,
                           NULL, 0, NULL, MAX_USB_REQ_TIMEOUT);
            break;
        case DTU_IOCTL_VENDOR_REQUEST:
        {
            UInt8*  pBuffer = NULL;
            if (pInBuf->m_VendorRequest.m_Dir == DT_USB_HOST_TO_DEVICE)
            {
                InReqSize += pInBuf->m_VendorRequest.m_BufLen;
                pBuffer = pInBuf->m_VendorRequest.m_Buf;
            } else {
                OutReqSize += pInBuf->m_VendorRequest.m_BufLen;
                pBuffer = pOutBuf->m_VendorRequest.m_Buf;
            }
            if (pIoctl->m_InputBufferSize < InReqSize) 
            {
                DtDbgOut(ERR, DTU, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) INPUT BUFFER TOO"
                                            " SMALL!",
                                            pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                            pIoctl->m_OutputBufferSize, OutReqSize);
                Status = DT_STATUS_INVALID_PARAMETER;
            }
            if (pIoctl->m_OutputBufferSize < OutReqSize) 
            {
                DtDbgOut(ERR, DTU, "%s: In=%d (Rq=%d) Out=%d (Rq=%d) OUTPUT BUFFER TOO"
                                            " SMALL!",
                                            pIoctlStr, pIoctl->m_InputBufferSize, InReqSize,
                                            pIoctl->m_OutputBufferSize, OutReqSize);
                Status = DT_STATUS_INVALID_PARAMETER;
            }

            if (DT_SUCCESS(Status))
                Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL,
                                                         pInBuf->m_VendorRequest.m_Code,
                                                         pInBuf->m_VendorRequest.m_Value,
                                                         pInBuf->m_VendorRequest.m_Index,
                                                         pInBuf->m_VendorRequest.m_Dir,
                                                         pBuffer,
                                                         pInBuf->m_VendorRequest.m_BufLen,
                                          &pOutBuf->m_VendorRequest.m_NumBytesTransferred,
                                                         MAX_USB_REQ_TIMEOUT);
        }   break;
        case DTU_IOCTL_GET_STATE_FLAGS:
            if (pInBuf->m_GetStateFlags.m_PortIndex == -1)
            {
                pOutBuf->m_GetStateFlags.m_Flags = pDvcData->m_StateFlags;
            } else {
                Int  NonIpPortIndex;        // Index in the nonip port struct
                // Validate port index
                Status = DtuGetNonIpPortIndex(pDvcData,
                                    pInBuf->m_GetStateFlags.m_PortIndex, &NonIpPortIndex);
                if (DT_SUCCESS(Status))
                {
                    pOutBuf->m_GetStateFlags.m_Flags =
                                     pDvcData->m_pNonIpPorts[NonIpPortIndex].m_StateFlags;
                }
            }
            break;
        case DTU_IOCTL_TRIGGER_WATCHDOG:
            if (pDvcData->m_DevInfo.m_TypeNumber == 351)
                Status = Dtu35xRegWrite(pDvcData, DTU_USB3_DEV_FX3, 0x02, 2);
            else if (pDvcData->m_DevInfo.m_TypeNumber>=300 
                                                   &&pDvcData->m_DevInfo.m_TypeNumber<400)
                Status = Dtu3RegWrite(pDvcData, DTU3_FX3_BLOCK_OFFSET, &FwbFx3.WatchDog, 
                                                                                       2);
            else
                Status = DT_STATUS_NOT_SUPPORTED;
            break;
        case DTU_IOCTL_SET_RX_MODE:
        {
            Int  NonIpPortIndex;        // Index in the nonip port struct
            DtuNonIpPort*  pPort;
            
            // This command is specific for the DTU-351 for now
            if (pDvcData->m_DevInfo.m_TypeNumber != 351)
            {
                Status = DT_STATUS_NOT_SUPPORTED;
                break;
            }

            if (pDvcData->m_DevInfo.m_UsbSpeed != 2)
            {
                Status = DT_STATUS_FAIL;
                break;
            }

            // Validate port index
            Status = DtuGetNonIpPortIndex(pDvcData,
                                           pInBuf->m_RxMode.m_PortIndex, &NonIpPortIndex);
            if (!DT_SUCCESS(Status))
                break;
            pPort = &pDvcData->m_pNonIpPorts[NonIpPortIndex];
            if (pInBuf->m_RxMode.m_RxMode == 0)
                pPort->m_NextState = DTU3_STATE_DET_VIDSTD;
            else {
                pPort->m_InitRxMode = pInBuf->m_RxMode.m_RxMode;
                pPort->m_NextState = DTU3_STATE_READ351;
            }

            if (pPort->m_NextState != pPort->m_State)
            {
                DtDbgOut(AVG, DTU, "New RX state");
                DtEventSet(&pPort->m_StateChanged);
                Status = DtEventWait(&pPort->m_StateChangeCmpl, -1);
                if (!DT_SUCCESS(Status))
                    DtDbgOut(ERR, DTU, "DtEventWait(m_StateChangeCmpl) failed");
            } else if (pPort->m_AllowRxModeChanges) {
                DtDbgOut(AVG, DTU, "Live RX mode change");
                // Only write the register if we're already reading data
                // If the RX thread is still initializing setting the m_InitRxMode
                // variable is enough and that's already done above.
                Status = Dtu35xRegWrite(pDvcData, DTU_USB3_DEV_FPGA, 0x06,
                                                       (UInt16)pInBuf->m_RxMode.m_RxMode);
                if (!DT_SUCCESS(Status))
                    DtDbgOut(ERR, DTU, "Dtu35xRegWrite setting rxmode failed");
            }
            if (DT_SUCCESS(Status))
            {
                UInt16  RegVal;
                Status = Dtu35xRegRead(pDvcData, DTU_USB3_DEV_FPGA, 0x0A, &RegVal);
                if (DT_SUCCESS(Status))
                    pOutBuf->m_RxMode.m_FrameIdNewRxMode = RegVal;
            }
        }   break;
        case DTU_IOCTL_UPLOAD_FPGA_FW:
            {
                UInt8*  pBuffer = NULL;
                Int  Size = 0;
#if defined(WINBUILD)
                PMDL  pMdl;
                NTSTATUS  NtStatus;
                // Retrieve MDL and buffer from request object
                NtStatus = WdfRequestRetrieveOutputWdmMdl(pIoctl->m_WdfRequest, &pMdl);
                if (NtStatus != STATUS_SUCCESS)
                {
                    DtDbgOut(ERR, DTU, "WdfRequestRetrieveOutputWdmMdl error: %08x", 
                                                                                NtStatus);
                    Status = DT_STATUS_OUT_OF_RESOURCES;
                }
                if (DT_SUCCESS(Status))
                {
                    pBuffer = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);
                    if (pBuffer == NULL)
                        Status = DT_STATUS_OUT_OF_MEMORY;
                    else
                        Size = MmGetMdlByteCount(pMdl);
                }
#else // LINBUILD
                Size = pInBuf->m_WriteData.m_NumBytesToWrite;
#if defined(LIN32)
                pBuffer = (char*)(UInt32)pInBuf->m_WriteData.m_BufferAddr;
#else
                pBuffer = (char*)(UInt64)pInBuf->m_WriteData.m_BufferAddr;
#endif
#endif
                if (DT_SUCCESS(Status))
                {
                    if (pDvcData->m_DevInfo.m_TypeNumber>=300
                                                  && pDvcData->m_DevInfo.m_TypeNumber<400)
                        Status = DtuFx3LoadPldFirmware(pDvcData, pBuffer, Size);
                    else
                        Status = DtuLoadPldFirmware(pDvcData, pBuffer, Size);
                }
            }
            break;
        case DTU_IOCTL_GET_DATA_BUF_SIZE:
        {
            Int  NonIpPortIndex;        // Index in the nonip port struct
            DtuNonIpPort*  pPort;

            if (pDvcData->m_DevInfo.m_UsbSpeed != 2)
            {
                Status = DT_STATUS_FAIL;
                break;
            }

            // Validate port index
            Status = DtuGetNonIpPortIndex(pDvcData,
                                      pInBuf->m_DataBufSize.m_PortIndex, &NonIpPortIndex);
            if (!DT_SUCCESS(Status))
                break;

            pPort = &pDvcData->m_pNonIpPorts[NonIpPortIndex];
            Status = DtuGetuFrameSize(pPort, &pOutBuf->m_DataBufSize.m_BufSize);
        }
            break;
        case DTU_IOCTL_SET_TX_CTRL:
        {
            Int  NonIpPortIndex;        // Index in the nonip port struct
            DtuNonIpPort*  pPort;
            
            // This command is specific for the DTU-315 for now
            if (pDvcData->m_DevInfo.m_TypeNumber!=315 &&
                                                    pDvcData->m_DevInfo.m_TypeNumber!=350)
            {
                Status = DT_STATUS_NOT_SUPPORTED;
                break;
            }

            if (pDvcData->m_DevInfo.m_UsbSpeed != 2)
            {
                Status = DT_STATUS_FAIL;
                break;
            }

            // Validate port index
            Status = DtuGetNonIpPortIndex(pDvcData, pInBuf->m_SetTxCtrl.m_PortIndex,
                                                                         &NonIpPortIndex);
            if (!DT_SUCCESS(Status))
                break;

            pPort = &pDvcData->m_pNonIpPorts[NonIpPortIndex];
            Status = Dtu315SetTxCtrl(pPort, pInBuf->m_SetTxCtrl.m_TxCtrl);
        }
            break;
        case DTU_IOCTL_UPLOAD_FPGA_FW_VARIANT:
            {
                Int  PldFwSize = 0;
                const UInt8*  pPldFirmware = NULL;
                Int  FirmwareVariant = pInBuf->m_UploadFpgaFwVariant.m_FirmwareVariant;
                // Clear current loaded FW variant
                pDvcData->m_DevInfo.m_FirmwareVariant = -1;
                // Find firmware variant in the firmware store
                pPldFirmware = DtuGetPldFirmware(pDvcData->m_DevInfo.m_ProductId, 
                     FirmwareVariant, pDvcData->m_DevInfo.m_HardwareRevision, &PldFwSize);
                if (pPldFirmware == NULL)
                {
                    DtDbgOut(ERR, DTU, "No PLD firmware variant #%s found for DTU-%d", 
                                       FirmwareVariant, pDvcData->m_DevInfo.m_TypeNumber);
                    Status = DT_STATUS_FAIL;
                    break;
                }
                // Upload PLD firmware to device
                if (pDvcData->m_DevInfo.m_TypeNumber>=300 
                                                  && pDvcData->m_DevInfo.m_TypeNumber<400)
                    Status = DtuFx3LoadPldFirmware(pDvcData, pPldFirmware, PldFwSize);
                else
                    Status = DtuLoadPldFirmware(pDvcData, pPldFirmware, PldFwSize);
                if (!DT_SUCCESS(Status))
                {
                    DtDbgOut(ERR, DTU, "Failed to upload PLD firmware (Status=0x%08X)", 
                                                                                  Status);
                    Status = DT_STATUS_FAIL;
                    break;
                }
                // Remember current loaded FW variant
                pDvcData->m_PropData.m_FirmwareVariant = FirmwareVariant;
                pDvcData->m_DevInfo.m_FirmwareVariant = FirmwareVariant;
            }
            break;
        case DTU_IOCTL_POWER_CTRL:
            {
                UInt16  SubValue = 0;      // Computed below

                // Compute power control value
                Status = DT_STATUS_OK;
                switch (pInBuf->m_PowerCtrl.m_Segment)
                {
                case DTU_POWER_CTRL_SEGMENT_ALL:
                    SubValue = (pInBuf->m_PowerCtrl.m_State) ? DTU_DVC_POWER_ON 
                                                             : DTU_DVC_POWER_OFF;
                    break;

                case DTU_POWER_CTRL_SEGMENT_FPGA:
                    SubValue = (pInBuf->m_PowerCtrl.m_State) ? DTU_DVC_FPGA_POWER_ON 
                                                             : DTU_DVC_FPGA_POWER_OFF;
                    break;

                case DTU_POWER_CTRL_SEGMENT_FRONTEND:
                    SubValue = (pInBuf->m_PowerCtrl.m_State) ? DTU_DVC_FRONTEND_POWER_ON 
                                                             : DTU_DVC_FRONTEND_POWER_OFF;
                    break;

                default:
                    Status = DT_STATUS_INVALID_PARAMETER;
                    break;
                }

                if (DT_SUCCESS(Status))
                    Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, 
                               DTU_USB3_PNP_CMD, DTU_PNP_CMD_DVC_POWER, SubValue, 
                               DT_USB_HOST_TO_DEVICE, NULL, 0, NULL, MAX_USB_REQ_TIMEOUT);  
            }
            break;
        case DTU_IOCTL_REG_WRITE_BULK:
            {
                UInt8*  pBuffer = NULL;
                Int  Size = 0;
#if defined(WINBUILD)
                PMDL  pMdl;
                NTSTATUS  NtStatus;
                // Retrieve MDL and buffer from request object
                NtStatus = WdfRequestRetrieveOutputWdmMdl(pIoctl->m_WdfRequest, &pMdl);
                if (NtStatus != STATUS_SUCCESS)
                {
                    DtDbgOut(ERR, DTU, "WdfRequestRetrieveOutputWdmMdl error: %08x", 
                                                                                NtStatus);
                    Status = DT_STATUS_OUT_OF_RESOURCES;
                }
                if (DT_SUCCESS(Status))
                {
                    pBuffer = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);
                    if (pBuffer == NULL)
                        Status = DT_STATUS_OUT_OF_MEMORY;
                    else
                        Size = MmGetMdlByteCount(pMdl);
                }
#else // LINBUILD
                Size = pInBuf->m_WriteData.m_NumBytesToWrite;
#if defined(LIN32)
                pBuffer = (char*)(UInt32)pInBuf->m_WriteData.m_BufferAddr;
#else
                pBuffer = (char*)(UInt64)pInBuf->m_WriteData.m_BufferAddr;
#endif
#endif
                if (DT_SUCCESS(Status))
                    // Clear endpoint buffers
                    Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, 
                               DTU_USB3_PNP_CMD, DTU_PNP_CMD_RESET, DTU_RESET_DMA_OUT, 
                               DT_USB_HOST_TO_DEVICE, NULL, 0, NULL, MAX_USB_REQ_TIMEOUT);
                if (DT_SUCCESS(Status))
                {
                    // Transmit bulk write vendor request
                    Int  MaxChunkSize = DtUsbGetCtrlMaxPacketSize(&pDvcData->m_Device,
                                                          pDvcData->m_DevInfo.m_UsbSpeed);
                    Int  SizeRemain = Size;
                    
                    while (SizeRemain > 0)
                    {
                        Int  Len = (SizeRemain<MaxChunkSize) ? SizeRemain : MaxChunkSize;
                        Int  Dummy;

                        Status = DtUsbVendorRequest(&pDvcData->m_Device, NULL, 
                                         DTU_USB3_WRITE_BULK, 0, 0, DT_USB_HOST_TO_DEVICE, 
                                         pBuffer, Len, &Dummy, MAX_USB_REQ_TIMEOUT);
                        if (!DT_SUCCESS(Status))
                            break;

                        pBuffer += Len;
                        SizeRemain -= Len;
                    }
                }
            }
            break;
        default:
            Status = DT_STATUS_NOT_SUPPORTED;
            break;
        }
    }

    // If we failed, no data has te be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED) 
            DtDbgOut(MIN, DTU, "Ioctl: %xh NOT SUPPORTED", pIoctl->m_IoctlCode);
    }
    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public interface +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuGetNonIpPortIndex -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuGetNonIpPortIndex(
    DtuDeviceData*  pDvcData,
    Int  PortIndex,
    Int*  pNonIpPortIndex)
{
    if (PortIndex >= pDvcData->m_NumPorts)
        return DT_STATUS_NOT_FOUND;
    if (PortIndex < 0)
        return DT_STATUS_NOT_FOUND;
    if (pDvcData->m_pPortLookup[PortIndex].m_PortType != DTU_PORT_TYPE_NONIP)
        return DT_STATUS_NOT_FOUND;

    *pNonIpPortIndex = pDvcData->m_pPortLookup[PortIndex].m_Index;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuGetPortIndexNonIp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuGetPortIndexNonIp(
    DtuDeviceData*  pDvcData, 
    Int  NonIpPortIndex,
    Int*  pPortIndex)
{
    Int i;
    Int NumNonIpPorts = 0;

    if (NonIpPortIndex >= pDvcData->m_NumNonIpPorts)
        return DT_STATUS_NOT_FOUND;
    if (NonIpPortIndex < 0)
        return DT_STATUS_NOT_FOUND;
    
    for (i=0; i<pDvcData->m_NumPorts;i++)
    {
        if (pDvcData->m_pPortLookup[i].m_PortType == DTU_PORT_TYPE_NONIP)
        {
            if (NumNonIpPorts == NonIpPortIndex)
            {
                *pPortIndex = i;
                return DT_STATUS_OK;
            }
            NumNonIpPorts++;
        }
    }
    return DT_STATUS_NOT_FOUND;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuGetPortNumber -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuGetPortNumber(DtuDeviceData* pDvcData, Int PortIndex, Int* pPortNumber)
{
    if (PortIndex >= pDvcData->m_NumPorts)
        return DT_STATUS_NOT_FOUND;
    if (PortIndex < 0)
        return DT_STATUS_NOT_FOUND;

    *pPortNumber = pDvcData->m_pPortLookup[PortIndex].m_PortNumber;

    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuCalculateAndCreatePortsStruct -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function calculates the number of NonIp and IpPorts, allocates the port structures
// and fills the reverse lookup table
//
DtStatus  DtuCalculateAndCreatePortsStruct(DtuDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  PortCount;
    Bool  CapAsi,  CapSdi, CapHdSdi, CapMod, CapDemod;
    Int  i;
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    
    pDvcData->m_NumNonIpPorts = 0;
    pDvcData->m_NumIpPorts = 0; // Always 0 for now 
    
    // Get number of ports property
    PortCount = DtPropertiesGetInt(pPropData, "PORT_COUNT", -1);

    DT_ASSERT(pDvcData->m_pPortLookup == NULL);
    pDvcData->m_pPortLookup = (DtuPortLookup*)
               DtMemAllocPool(DtPoolNonPaged, sizeof(DtuPortLookup) * PortCount, DTU_TAG);
    if (pDvcData->m_pPortLookup == NULL)
        return DT_STATUS_OUT_OF_MEMORY;

    // Initialize reverse lookup structures
    for (i=0; i<PortCount; i++)
    {
        CapAsi = DtPropertiesGetBool(pPropData, "CAP_ASI", i);
        CapSdi = DtPropertiesGetBool(pPropData, "CAP_SDI", i);
        CapHdSdi = DtPropertiesGetBool(pPropData, "CAP_HDSDI", i);
        CapMod = DtPropertiesGetBool(pPropData, "CAP_MOD", i);
        CapDemod = DtPropertiesGetBool(pPropData, "CAP_DEMOD", i);
        
        if (CapAsi || CapSdi || CapHdSdi || CapDemod || CapMod)
        {
            // Setup reverse lookup
            pDvcData->m_pPortLookup[i].m_PortType = DTU_PORT_TYPE_NONIP;
            pDvcData->m_pPortLookup[i].m_Index = pDvcData->m_NumNonIpPorts;
            pDvcData->m_pPortLookup[i].m_PortNumber = i + 1;

            // We found one, increment number nonip ports
            pDvcData->m_NumNonIpPorts++;
        } else {
            Status = DT_STATUS_NOT_SUPPORTED;
            DtDbgOut(ERR, DTU, "[%d] Port type not supported", i);
            pDvcData->m_NumNonIpPorts = 0;
            break;
        }
    }
    pDvcData->m_NumPorts = pDvcData->m_NumNonIpPorts + pDvcData->m_NumIpPorts;

    // Allocate memory for NonIp port array
    DT_ASSERT(pDvcData->m_pNonIpPorts == NULL);
    if (pDvcData->m_NumNonIpPorts != 0)
    {
        pDvcData->m_pNonIpPorts = (DtuNonIpPort*)DtMemAllocPool(DtPoolNonPaged, 
                               sizeof(DtuNonIpPort) * pDvcData->m_NumNonIpPorts, DTU_TAG);
        if (pDvcData->m_pNonIpPorts == NULL)
        {
            pDvcData->m_NumNonIpPorts = 0;
            return DT_STATUS_OUT_OF_MEMORY;
        }
        DtMemZero(pDvcData->m_pNonIpPorts, 
                                        sizeof(DtuNonIpPort) * pDvcData->m_NumNonIpPorts);
    }

    // Allocate memory for Ip port array
    /*DT_ASSERT(pDvcData->m_IpDevice.m_pIpPorts == NULL);
    if (pDvcData->m_NumIpPorts != 0)
    {
        pDvcData->m_IpDevice.m_pIpPorts = (DtuIpPort*)DtMemAllocPool(DtPoolNonPaged, 
                                     sizeof(DtuIpPort) * pDvcData->m_NumIpPorts, DTA_TAG);
        if (pDvcData->m_IpDevice.m_pIpPorts == NULL)
        {
            pDvcData->m_NumIpPorts = 0;
            return DT_STATUS_OUT_OF_MEMORY;
        }
        DtMemZero(pDvcData->m_IpDevice.m_pIpPorts, 
                                              sizeof(DtuIpPort) * pDvcData->m_NumIpPorts);
    }*/

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuPortsCleanUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtuPortsCleanUp(DtuDeviceData* pDvcData)
{ 
    Int  i;
    Int  PortIndex;
    
    // Cleanup NonIp port structures
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        if (DT_SUCCESS(DtuGetPortIndexNonIp(pDvcData, i, &PortIndex)))
            DtuNonIpCleanup(pDvcData, PortIndex, &pDvcData->m_pNonIpPorts[i]);
        else
            DT_ASSERT(FALSE);
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuCleanupPortStructs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtuCleanupPortStructs(DtuDeviceData* pDvcData)
{
    pDvcData->m_NumNonIpPorts = 0;
    pDvcData->m_NumIpPorts = 0;
    pDvcData->m_NumPorts = 0;

    if (pDvcData->m_pNonIpPorts != NULL)
        DtMemFreePool(pDvcData->m_pNonIpPorts, DTU_TAG);
    pDvcData->m_pNonIpPorts = NULL;
    
    /*if (pDvcData->m_IpDevice.m_pIpPorts != NULL)
        DtMemFreePool(pDvcData->m_IpDevice.m_pIpPorts, DTU_TAG);
    pDvcData->m_IpDevice.m_pIpPorts = NULL;
    */
    if (pDvcData->m_pPortLookup != NULL)
        DtMemFreePool(pDvcData->m_pPortLookup, DTU_TAG);
    pDvcData->m_pPortLookup = NULL;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuPortsInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtuPortsInit(DtuDeviceData* pDvcData)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  i;
    Int  PortIndex;
    
    // Initialize NonIp port structures
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        if (DT_SUCCESS(DtuGetPortIndexNonIp(pDvcData, i, &PortIndex)))
        {
            Status = DtuNonIpInit(pDvcData, PortIndex, &pDvcData->m_pNonIpPorts[i]);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, DTU, "[%d] Error initialising NonIp Port %i.", i, 
                                                                               PortIndex);
                return Status;
            }
        } else
            DT_ASSERT(FALSE);
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuDeviceIsInuse -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Checks whether the device is in use; one of the ports has exclusive access set 
// PRE: m_ExclAccessMutex is acquired
//
Bool  DtuDeviceIsInuse(DtuDeviceData* pDvcData)
{
    Int i;
    Bool  Inuse = FALSE;
    for (i = 0; i<pDvcData->m_NumNonIpPorts; i++) 
    {
        if (pDvcData->m_pNonIpPorts[i].m_ExclAccess)
        {
            Inuse = TRUE;
            break;
        }
    }
    return Inuse;
}
