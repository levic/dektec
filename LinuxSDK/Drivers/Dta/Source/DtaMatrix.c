//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaMatrix.c *#*#*#*#*#*#*#*#* (C) 2012-2016 DekTec
//
// Dta driver - Implements Matrix-API (i.e. HD-SDI) related functions
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2012-2016 DekTec Digital Video B.V.
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
#include <DtaIncludes.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtaMatrixSofFrameIntDpc(DtDpcArgs* pArgs);
DtStatus  DtaMatrixSyncInfoGet(DtaDeviceData*  pDvcData, 
                                              DtaIoctlMatrixCmdGetSyncInfoOutput*  pInfo);
DtStatus  DtaMatrixWaitFrame(DtaDeviceData*  pDvcData, DtaNonIpPort*  pNonIpPort,
                                          Int64  WaitFrame, Int  Timeout, Int64*  pFrame,
                                          Int64*  pRefClkStart, Int64*  pRefClkEnd,
                                          Int64*  pFrmIntCnt, Int*  pTopHalf);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMatrixInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// NOTE: call init after having initialised ports (SW-init)
//
DtStatus  DtaMatrixInit(DtaDeviceData*  pDvcData)
{

    Int  i;
    DtStatus  Status = DT_STATUS_OK;
    DtaMatrix*  pMatrix = &pDvcData->m_Matrix;
    DtPropertyData* pPropData = &pDvcData->m_PropData;

    // Matrix-API is supported if we have at-least one port with the MATRIX capability
    pMatrix->m_IsSupported = FALSE;
    for (i=0; i<pDvcData->m_NumNonIpPorts && !pMatrix->m_IsSupported; i++)
        pMatrix->m_IsSupported = pDvcData->m_pNonIpPorts[i].m_CapMatrix;
  
    // Get Matrix properties
    if (pMatrix->m_IsSupported)
    {
        pMatrix->m_RamSize = DtPropertiesGetInt(pPropData, "MATRIX_RAM_SIZE", -1);
        pMatrix->m_MemPckSize = DtPropertiesGetInt(pPropData, "MATRIX_MEM_PCK_SIZE", -1);
        pMatrix->m_MemPckNumBitsUsed = DtPropertiesGetInt(pPropData, 
                                                      "MATRIX_MEM_PCK_NUM_BITS_USED", -1);
        pMatrix->m_MemLineAlignment = DtPropertiesGetInt(pPropData, 
                                                         "MATRIX_MEM_LINE_ALIGNMENT", -1);

        // Check if no property error occurred
        Status = DtaPropertiesReportDriverErrors(pDvcData);
        if (!DT_SUCCESS(Status))
            return Status;

        DtDbgOut(MIN, MATRIX, "Matrix Mem. Props: sz=%dMB, psz=%d, pusd=%d, align=%d", 
                               pMatrix->m_RamSize, pMatrix->m_MemPckSize, 
                               pMatrix->m_MemPckNumBitsUsed, pMatrix->m_MemLineAlignment);
    }
    else
    {
        pMatrix->m_RamSize = -1;
        pMatrix->m_MemPckSize = -1;
        pMatrix->m_MemPckNumBitsUsed = -1;
        pMatrix->m_MemLineAlignment = -1;
    }
               
    pMatrix->m_SofFrame = 0;

    pMatrix->m_SyncInfo.m_IsValid = FALSE;
    DtSpinLockInit(&pMatrix->m_SyncInfoSpinLock);

    DtDpcInit(&pMatrix->m_SofFrameIntDpc, DtaMatrixSofFrameIntDpc, TRUE);
    DtEventInit(&pMatrix->m_SofFrameIntEvent, FALSE);
    DtEventInit(&pMatrix->m_SofFrameSyncEvent, FALSE);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMatrixIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaMatrixIoctl(
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
    DtaNonIpPort*  pNonIpPort = NULL;
    DtaIoctlMatrixCmdInput* pMatrixCmdInput = 
                                         (DtaIoctlMatrixCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlMatrixCmdOutput* pMatrixCmdOutput =
                                       (DtaIoctlMatrixCmdOutput*)pIoctl->m_pOutputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlMatrixCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlMatrixCmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    pNonIpPort = NULL;  // Assume a board level request
    if (pMatrixCmdInput->m_PortIndex == -1)
        NonIpPortIndex = -1;    // Is a board level request
    else if (DT_SUCCESS(DtaGetNonIpPortIndex(pDvcData, pMatrixCmdInput->m_PortIndex,
                                                                        &NonIpPortIndex)))
        pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpPortIndex];
    else
         return DT_STATUS_INVALID_PARAMETER;
           
    // Determine final required output/input sizes
    switch (pMatrixCmdInput->m_Cmd)
    {
    case DTA_MATRIX_CMD_WAIT_FRAME:
        pCmdStr = "DTA_MATRIX_CMD_WAIT_FRAME";
        InReqSize += sizeof(DtaIoctlMatrixCmdWaitFrameInput);
        OutReqSize += sizeof(DtaIoctlMatrixCmdWaitFrameOutput);
        break;

    case DTA_MATRIX_CMD_GET_SYNC_INFO:
        pCmdStr = "DTA_MATRIX_CMD_GET_SYNC_INFO";
        OutReqSize += sizeof(DtaIoctlMatrixCmdGetSyncInfoOutput);
        // We expect no additional data in the input buffer
        InReqSize += 0;
        break;

    case DTA_MATRIX_CMD_ATTACH_TO_ROW:
        pCmdStr = "DTA_MATRIX_CMD_ATTACH_TO_ROW";
        InReqSize += sizeof(DtaIoctlMatrixCmdAttachToRowInput);
        // There is no additional out data
        OutReqSize += 0;        
        break;

    case DTA_MATRIX_CMD_START:
        pCmdStr = "DTA_MATRIX_CMD_START";
        InReqSize += sizeof(DtaIoctlMatrixCmdStartInput);
        // There is no additional out data
        OutReqSize += 0;        
        break;

    case DTA_MATRIX_CMD_STOP:
        pCmdStr = "DTA_MATRIX_CMD_STOP";
        // There is no additional in or out data
        OutReqSize += 0; InReqSize += 0;
        break;

    case DTA_MATRIX_CMD_GET_CURR_FRAME:
        pCmdStr = "DTA_MATRIX_CMD_GET_CURR_FRAME";
        OutReqSize += sizeof(DtaIoctlMatrixCmdGetCurrFrameOutput);
        // We expect no additional data in the input buffer
        InReqSize += 0;
        break;

     case DTA_MATRIX_CMD_DMA_WRITE:
        pCmdStr = "DTA_MATRIX_CMD_DMA_WRITE";
        InReqSize += sizeof(DtaIoctlMatrixCmdWriteDmaInput);
#ifdef WINBUILD
        OutReqSize = 0; // for write there is no output header
#else
        OutReqSize += sizeof(DtaIoctlMatrixCmdWriteDmaOutput);
#endif
        break;

    case DTA_MATRIX_CMD_DMA_READ:
        pCmdStr = "DTA_MATRIX_CMD_DMA_READ";
        InReqSize += sizeof(DtaIoctlMatrixCmdReadDmaInput);
#ifdef WINBUILD
        OutReqSize = 0; // for read there is no output header
#else
        OutReqSize += sizeof(DtaIoctlMatrixCmdReadDmaOutput);
#endif
        break;

    case DTA_MATRIX_CMD_GET_FIFOLOAD:
        pCmdStr = "DTA_MATRIX_CMD_GET_FIFOLOAD";
        OutReqSize += sizeof(DtaIoctlMatrixCmdGetFifoLoadOutput);
        // We expect no additional data in the input buffer
        InReqSize += 0;
        break;

    case DTA_MATRIX_CMD_GET_FIFOSIZE:
        pCmdStr = "DTA_MATRIX_CMD_GET_FIFOSIZE";
        OutReqSize += sizeof(DtaIoctlMatrixCmdGetFifoSizeOutput);
        // We expect no additional data in the input buffer
        InReqSize += 0;
        break;

    case DTA_MATRIX_CMD_GET_FIFOSIZE_MAX:
        pCmdStr = "DTA_MATRIX_CMD_GET_FIFOSIZE_MAX";
        OutReqSize += sizeof(DtaIoctlMatrixCmdGetFifoSizeMaxOutput);
        // We expect no additional data in the input buffer
        InReqSize += 0;
        break;

    case DTA_MATRIX_CMD_SET_FIFOSIZE:
        pCmdStr = "DTA_MATRIX_CMD_SET_FIFOSIZE";
        InReqSize += sizeof(DtaIoctlMatrixCmdSetFifoSizeInput);
        // We expect no additional data in the output buffer
        OutReqSize += 0;
        break;

    case DTA_MATRIX_CMD_SET_ASI_CTRL:
        pCmdStr = "DTA_MATRIX_CMD_SET_ASI_CTRL";
        InReqSize += sizeof(DtaIoctlMatrixCmdSetAsiCtrlInput);
        // We expect no additional data in the output buffer
        OutReqSize += 0;
        break;

    case DTA_MATRIX_CMD_GET_BUF_CONFIG:
        pCmdStr = "DTA_MATRIX_CMD_GET_BUF_CONFIG";
        OutReqSize += sizeof(DtaIoctlMatrixCmdGetBufConfigOutput);
        // We expect no additional data in the input buffer
        break;

    case DTA_MATRIX_CMD_GET_REQ_DMA_SIZE:
        pCmdStr = "DTA_MATRIX_CMD_GET_REQ_DMA_SIZE";
        InReqSize += sizeof(DtaIoctlMatrixCmdGetReqDmaSizeInput);
        OutReqSize += sizeof(DtaIoctlMatrixCmdGetReqDmaSizeOutput);
        break;

    case DTA_MATRIX_CMD_GET_FRM_INFO:
        pCmdStr = "DTA_MATRIX_CMD_GET_FRM_INFO";
        InReqSize += sizeof(DtaIoctlMatrixCmdGetFrmInfoInput);
        OutReqSize += sizeof(DtaIoctlMatrixCmdGetFrmInfoOutput);
        break;

    case DTA_MATRIX_CMD_GET_FRM_PROPS:
        pCmdStr = "DTA_MATRIX_CMD_GET_FRM_PROPS";
        InReqSize += sizeof(DtaIoctlMatrixCmdGetFrmPropsInput);
        OutReqSize += sizeof(DtaIoctlMatrixCmdGetFrmPropsOutput);
        break;

    case DTA_MATRIX_CMD_START_MAN:
        pCmdStr = "DTA_MATRIX_CMD_START_MAN";
        InReqSize += sizeof(DtaIoctlMatrixCmdStartInput);
        // There is no additional out data
        OutReqSize += 0;
        break;

    case DTA_MATRIX_CMD_SET_NEXT_FRAME:
        pCmdStr = "DTA_MATRIX_CMD_SET_NEXT_FRAME";
        InReqSize += sizeof(DtaIoctlMatrixCmdSetNextFrmInput);
        // There is no additional out data
        OutReqSize += 0;
        break;

    case DTA_MATRIX_CMD_GET_FRM_INFO2:
        pCmdStr = "DTA_MATRIX_CMD_GET_FRM_INFO2";
        InReqSize += sizeof(DtaIoctlMatrixCmdGetFrmInfoInput);
        OutReqSize += sizeof(DtaIoctlMatrixCmdGetFrmInfo2Output);
        break;

    case DTA_MATRIX_CMD_WAIT_FRAME2:
        pCmdStr = "DTA_MATRIX_CMD_WAIT_FRAME2";
        InReqSize += sizeof(DtaIoctlMatrixCmdWaitFrameInput);
        OutReqSize += sizeof(DtaIoctlMatrixCmdWaitFrame2Output);
        break;

    case DTA_MATRIX_CMD_START2:
        pCmdStr = "DTA_MATRIX_CMD_START2";
        InReqSize += sizeof(DtaIoctlMatrixCmdStart2Input);
        // There is no additional out data
        OutReqSize += 0;
        break;

    case DTA_MATRIX_CMD_START3:
        pCmdStr = "DTA_MATRIX_CMD_START3";
        InReqSize += sizeof(DtaIoctlMatrixCmdStart3Input);
        // There is no additional out data
        OutReqSize += 0;
        break;

    case DTA_MATRIX_CMD_WAIT_FRAME3:
        pCmdStr = "DTA_MATRIX_CMD_WAIT_FRAME3";
        InReqSize += sizeof(DtaIoctlMatrixCmdWaitFrameInput);
        OutReqSize += sizeof(DtaIoctlMatrixCmdWaitFrame3Output);
        break;

    case DTA_MATRIX_CMD_GET_VPID:
        pCmdStr = "DTA_MATRIX_CMD_GET_VPID";
        OutReqSize += sizeof(DtaIoctlMatrixCmdGetVpidOutput);
        // We expect no additional data in the input buffer
        InReqSize += 0;
        break;

    default:
        pCmdStr = "??UNKNOWN MATRIXCMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (PowerDownPending)
    {
        // Only skip channel specific IOCTL's
        switch (pMatrixCmdInput->m_Cmd)
        {
            case DTA_MATRIX_CMD_DMA_WRITE:
            case DTA_MATRIX_CMD_DMA_READ:
                DtDbgOut(ERR, DTA, "%s: Matrix cmd because powerdown  occured!", pCmdStr);
                return DT_STATUS_POWERDOWN;
        } 
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, MATRIX, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr,
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, MATRIX, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, MATRIX, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
            pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        // Execute cmd
        switch (pMatrixCmdInput->m_Cmd)
        {
        case DTA_MATRIX_CMD_WAIT_FRAME: {
            Int64  RefClkStart, RefClkEnd;
            Int64  FrmIntCnt;
            Status = DtaMatrixWaitFrame(pDvcData, pNonIpPort, 
                                          pMatrixCmdInput->m_Data.m_WaitFrame.m_FrmIntCnt,
                                          pMatrixCmdInput->m_Data.m_WaitFrame.m_Timeout,
                                          &pMatrixCmdOutput->m_Data.m_WaitFrame.m_Frame,
                                          &RefClkStart, &RefClkEnd, &FrmIntCnt, NULL);
        }   break;

        case DTA_MATRIX_CMD_GET_SYNC_INFO:
            Status = DtaMatrixSyncInfoGet(pDvcData, &pMatrixCmdOutput->m_Data.m_SyncInfo);
            break;

        case DTA_MATRIX_CMD_ATTACH_TO_ROW:
            Status = DtaNonIpMatrixAttachToRow(pNonIpPort,
                                          pMatrixCmdInput->m_Data.m_AttachToRow.m_RowIdx);
            break;
            
        case DTA_MATRIX_CMD_START:
            pNonIpPort->m_Matrix.m_Vpid1 = 0;
            pNonIpPort->m_Matrix.m_Vpid2 = 0;
            Status = DtaNonIpMatrixStart(pNonIpPort, 
                                             pMatrixCmdInput->m_Data.m_Start.m_StartFrame,
                                             TRUE, FALSE);
            break;

        case DTA_MATRIX_CMD_STOP:
            Status = DtaNonIpMatrixStop(pNonIpPort);
            break;

        case DTA_MATRIX_CMD_GET_CURR_FRAME:
            Status = DtaNonIpMatrixGetCurrentFrame(pNonIpPort, 
                                    &pMatrixCmdOutput->m_Data.m_GetCurrFrame.m_CurrFrame);
            break;

        case DTA_MATRIX_CMD_DMA_WRITE:
        {
            char*  pBuffer;
            Int  Size;
            DtPageList*  pPageList = NULL;
            UInt8*  pLocalAddress;
            DmaChannel*  pDmaCh = NULL;
            DtaMatrixMemTrSetup  MemTrSetup;
                
#if defined(WINBUILD)
            DtPageList  PageList;
            PMDL  pMdl;
            NTSTATUS  NtStatus;
            DtaWdfRequestContext*  pRequestContext;
            WDF_OBJECT_ATTRIBUTES  ObjAttr;

            // Retrieve MDL and virtual buffer from request object
            NtStatus = WdfRequestRetrieveOutputWdmMdl(pIoctl->m_WdfRequest, &pMdl);
            if (NtStatus != STATUS_SUCCESS)
            {
                DtDbgOut(ERR, DTA, "WdfRequestRetrieveOutputWdmMdl error: %08x", 
                                                                                NtStatus);
                Status = DT_STATUS_OUT_OF_RESOURCES;
            }
            if (DT_SUCCESS(Status))
            {
                pBuffer = MmGetMdlVirtualAddress(pMdl);
                if (pBuffer == NULL)
                {
                    DtDbgOut(ERR, DTA, "MmGetMdlVirtualAddress failed");
                    Status = DT_STATUS_OUT_OF_MEMORY;
                }
                
                Size = MmGetMdlByteCount(pMdl);

                // Build pagelist object for user space buffer
                pPageList = &PageList;
                pPageList->m_BufType = DT_BUFTYPE_USER;
                pPageList->m_OwnedByOs = TRUE;
                pPageList->m_pMdl = pMdl;
                pPageList->m_pVirtualKernel = NULL;
            }
#else // LINBUILD
            Size = pMatrixCmdInput->m_Data.m_DmaWrite.m_NumBytesToWrite;
#if defined(LIN32)
            pBuffer = (char*)(UInt32)pMatrixCmdInput->m_Data.m_DmaWrite.m_BufferAddr;
#else
            pBuffer = (char*)(UInt64)pMatrixCmdInput->m_Data.m_DmaWrite.m_BufferAddr;
#endif
#endif
            // Prep channel (memory) for DMA
            MemTrSetup.m_IsWrite = TRUE;
            MemTrSetup.m_Frame = pMatrixCmdInput->m_Data.m_DmaWrite.m_Common.m_Frame;
            MemTrSetup.m_StartLine = pMatrixCmdInput->m_Data.m_DmaWrite.m_Common.m_Line;
            MemTrSetup.m_NumLines = 
                                   pMatrixCmdInput->m_Data.m_DmaWrite.m_Common.m_NumLines;
            MemTrSetup.m_TrCmd = pMatrixCmdInput->m_Data.m_DmaWrite.m_Common.m_TrCmd;
            MemTrSetup.m_DataFormat = 
                                 pMatrixCmdInput->m_Data.m_DmaWrite.m_Common.m_DataFormat;
            MemTrSetup.m_RgbMode = pMatrixCmdInput->m_Data.m_DmaWrite.m_Common.m_RgbMode;
            MemTrSetup.m_SymFlt = pMatrixCmdInput->m_Data.m_DmaWrite.m_Common.m_SymFlt;
            MemTrSetup.m_Scaling = pMatrixCmdInput->m_Data.m_DmaWrite.m_Common.m_Scaling;
            MemTrSetup.m_AncFlt = pMatrixCmdInput->m_Data.m_DmaWrite.m_Common.m_AncFlt;
            MemTrSetup.m_Stride = pMatrixCmdInput->m_Data.m_DmaWrite.m_Common.m_Stride;
            
            if (MemTrSetup.m_TrCmd==DT_MEMTR_TRCMD_ASIWR)
            {
                Int  BytesLeftFlat = pNonIpPort->m_Matrix.m_AsiFifoSize
                                                    - pNonIpPort->m_Matrix.m_AsiDmaOffset;
                
                if (Size > BytesLeftFlat)
                {
                    DtDbgOut(MAX, DMA, "ASI buf wrap, will truncate wr-transfer: %d=>%d", 
                                                                     Size, BytesLeftFlat);
                    Size = BytesLeftFlat;
                }
            }
            
            if (DT_SUCCESS(Status))
                Status = DtaNonIpMatrixPrepForDma(pNonIpPort, Size, &MemTrSetup, &Size);
            
            // If prepped, start DMA
            if (DT_SUCCESS(Status))
            {
                pDmaCh = &pNonIpPort->m_DmaChannel;
                pLocalAddress = (UInt8*)(size_t)pNonIpPort->m_FifoOffset;

#ifdef WINBUILD
                // Store the NonIpPort pointer in the request object
                WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&ObjAttr, DtaWdfRequestContext);
                if (!NT_SUCCESS(WdfObjectAllocateContext(pIoctl->m_WdfRequest, 
                                                     &ObjAttr, (PVOID*)&pRequestContext)))
                    Status = DT_STATUS_OUT_OF_RESOURCES;
                else {
                    pRequestContext->m_pData =  &pDvcData->m_pNonIpPorts[NonIpPortIndex];
                    pRequestContext->m_Value = MemTrSetup.m_TrCmd;
                }

                if (DT_SUCCESS(Status))
                {
                    // Register a cancel routine
                    if (!NT_SUCCESS(WdfRequestMarkCancelableEx(pIoctl->m_WdfRequest,
                                                    DtaNonIpMatrixTxEvtRequestCancelDma)))
                        Status = DT_STATUS_CANCELLED;
                        
                    // Store the WdfRequest for the completion routine
                    DtaDmaReInitCallback(pDmaCh, DtaNonIpMatrixDmaCompletedWindows, 
                                                                pIoctl->m_WdfRequest);
                }
#endif
            }
            if (DT_SUCCESS(Status))
            {
                Status = DtaDmaStartTransfer(pDmaCh, pPageList, DT_BUFTYPE_USER, 
                                             DT_DMA_DIRECTION_TO_DEVICE, pBuffer, Size, 0,
                                             pLocalAddress, 0, 0, FALSE, &Size);
            }
#ifdef WINBUILD
            // Mark the IO request pending, we complete the request in the DMA 
            // completion routine. This is an internal status and will be handled by
            // the IAL
            if (DT_SUCCESS(Status))
                Status = DT_STATUS_IO_PENDING;
            else {
                WdfRequestUnmarkCancelable(pIoctl->m_WdfRequest);
                pIoctl->m_OutputBufferBytesWritten = 0;
            }
#else
            if (DT_SUCCESS(Status))
                pMatrixCmdOutput->m_Data.m_DmaWrite.m_NumBytesWritten = 
                          DtaNonIpMatrixDmaWriteFinished(pNonIpPort,  MemTrSetup.m_TrCmd);
            else
                pMatrixCmdOutput->m_Data.m_DmaWrite.m_NumBytesWritten = 0;
#endif
            break;
        }
        
        case DTA_MATRIX_CMD_DMA_READ:
        {
            char*  pBuffer;
            Int  Size;
            DtPageList*  pPageList = NULL;
            UInt8*  pLocalAddress;
            DmaChannel*  pDmaCh = NULL;
            DtaMatrixMemTrSetup  MemTrSetup;

#if defined(WINBUILD)
            DtPageList  PageList;
            PMDL  pMdl;
            NTSTATUS  NtStatus;
            DtaWdfRequestContext*  pRequestContext;
            WDF_OBJECT_ATTRIBUTES  ObjAttr;

            // Retrieve MDL and virtual buffer from request object
            NtStatus = WdfRequestRetrieveOutputWdmMdl(pIoctl->m_WdfRequest, &pMdl);
            if (NtStatus != STATUS_SUCCESS)
            {
                DtDbgOut(ERR, DTA, "WdfRequestRetrieveOutputWdmMdl error: %08x", 
                                                                                NtStatus);
                Status = DT_STATUS_OUT_OF_RESOURCES;
            }
            if (DT_SUCCESS(Status))
            {
                pBuffer = MmGetMdlVirtualAddress(pMdl);
                if (pBuffer == NULL)
                 {
                    DtDbgOut(ERR, DTA, "MmGetMdlVirtualAddress failed");
                    Status = DT_STATUS_OUT_OF_MEMORY;
                }
                Size = MmGetMdlByteCount(pMdl);

                // Build pagelist object for user space buffer
                pPageList = &PageList;
                pPageList->m_BufType = DT_BUFTYPE_USER;
                pPageList->m_OwnedByOs = TRUE;
                pPageList->m_pMdl = pMdl;
                pPageList->m_pVirtualKernel = NULL;
            }
#else // LINBUILD
            Size = pMatrixCmdInput->m_Data.m_DmaRead.m_NumBytesToRead;
#if defined(LIN32)
            pBuffer = (char*)(UInt32)pMatrixCmdInput->m_Data.m_DmaRead.m_BufferAddr;
#else
            pBuffer = (char*)(UInt64)pMatrixCmdInput->m_Data.m_DmaRead.m_BufferAddr;
#endif
#endif
            // Prep channel (memory) for DMA
            MemTrSetup.m_IsWrite = FALSE;
            MemTrSetup.m_Frame = pMatrixCmdInput->m_Data.m_DmaRead.m_Common.m_Frame;
            MemTrSetup.m_StartLine = pMatrixCmdInput->m_Data.m_DmaRead.m_Common.m_Line;
            MemTrSetup.m_NumLines = pMatrixCmdInput->m_Data.m_DmaRead.m_Common.m_NumLines;
            MemTrSetup.m_TrCmd = pMatrixCmdInput->m_Data.m_DmaRead.m_Common.m_TrCmd;
            MemTrSetup.m_DataFormat = 
                                  pMatrixCmdInput->m_Data.m_DmaRead.m_Common.m_DataFormat;
            MemTrSetup.m_RgbMode = pMatrixCmdInput->m_Data.m_DmaRead.m_Common.m_RgbMode;
            MemTrSetup.m_SymFlt = pMatrixCmdInput->m_Data.m_DmaRead.m_Common.m_SymFlt;
            MemTrSetup.m_Scaling = pMatrixCmdInput->m_Data.m_DmaRead.m_Common.m_Scaling;
            MemTrSetup.m_AncFlt = pMatrixCmdInput->m_Data.m_DmaRead.m_Common.m_AncFlt;
            MemTrSetup.m_Stride = pMatrixCmdInput->m_Data.m_DmaRead.m_Common.m_Stride;
            
            if (MemTrSetup.m_TrCmd==DT_MEMTR_TRCMD_ASIRD)
            {
                Int  BytesLeftFlat = pNonIpPort->m_Matrix.m_AsiFifoSize
                                                    - pNonIpPort->m_Matrix.m_AsiDmaOffset;

                if (Size > BytesLeftFlat)
                {
                    DtDbgOut(MAX, DMA, "ASI buf wrap, will truncate rd-transfer: %d=>%d", 
                                                                     Size, BytesLeftFlat);
                    Size = BytesLeftFlat;
                }
            }

            if (((UInt64)pBuffer)%8 != 0)
                Status = DT_STATUS_INVALID_PARAMETER;

            if (DT_SUCCESS(Status))
                Status = DtaNonIpMatrixPrepForDma(pNonIpPort, Size, &MemTrSetup, &Size);

            // If prepped, start DMA
            if (DT_SUCCESS(Status))
            {           
                pDmaCh = &pNonIpPort->m_DmaChannel;
                pLocalAddress = (UInt8*)(size_t)pNonIpPort->m_FifoOffset;
#ifdef WINBUILD
                // Store the NonIpPort pointer in the request object
                WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&ObjAttr, DtaWdfRequestContext);
                if (!NT_SUCCESS(WdfObjectAllocateContext(pIoctl->m_WdfRequest, 
                                                     &ObjAttr, (PVOID*)&pRequestContext)))
                    Status = DT_STATUS_OUT_OF_RESOURCES;
                else
                {
                    pRequestContext->m_pData = &pDvcData->m_pNonIpPorts[NonIpPortIndex];
                    pRequestContext->m_Value = MemTrSetup.m_TrCmd;
                }

                if (DT_SUCCESS(Status))
                {
                    // Register a cancel routine
                    if (!NT_SUCCESS(WdfRequestMarkCancelableEx(pIoctl->m_WdfRequest,
                                                    DtaNonIpMatrixRxEvtRequestCancelDma)))
                        Status = DT_STATUS_CANCELLED;
                        
                    // Store the WdfRequest for the completion routine
                    DtaDmaReInitCallback(pDmaCh, DtaNonIpMatrixDmaCompletedWindows, 
                                                                    pIoctl->m_WdfRequest);
                }
#endif
            }
            if (DT_SUCCESS(Status))
            {
                Status = DtaDmaStartTransfer(pDmaCh, pPageList, DT_BUFTYPE_USER, 
                                           DT_DMA_DIRECTION_FROM_DEVICE, pBuffer, Size, 0,
                                           pLocalAddress, 0, 0, FALSE, &Size);
            }
#ifdef WINBUILD
            // Mark the IO request pending, we complete the request in the DMA 
            // completion routine. This is an internal status and will be handled by
            // the IAL
            if (DT_SUCCESS(Status))
                Status = DT_STATUS_IO_PENDING;
            else {
                WdfRequestUnmarkCancelable(pIoctl->m_WdfRequest);
                pIoctl->m_OutputBufferBytesWritten = 0;
            }
#else
            if (DT_SUCCESS(Status))
                pMatrixCmdOutput->m_Data.m_DmaRead.m_NumBytesRead = 
                            DtaNonIpMatrixDmaReadFinished(pNonIpPort, MemTrSetup.m_TrCmd);
            else
                pMatrixCmdOutput->m_Data.m_DmaRead.m_NumBytesRead = 0;
#endif

            break;
        }

        case DTA_MATRIX_CMD_GET_FIFOLOAD:
            // Only for ASI
            DT_ASSERT(pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_ASI);
            if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_INPUT)
            {
                Status = DtaMatrixAsiRxGetFifoLoad(pNonIpPort,
                                      &pMatrixCmdOutput->m_Data.m_GetFifoLoad.m_FifoLoad);
            }
            else 
            {
                Int64  NumBytesPlayed = DtaRegHdAsiByteCountGet(pNonIpPort->m_pTxRegs);
                pMatrixCmdOutput->m_Data.m_GetFifoLoad.m_FifoLoad = 
                            (Int)(pNonIpPort->m_Matrix.m_AsiDmaNumBytes - NumBytesPlayed);
            }
            break;

        case DTA_MATRIX_CMD_GET_FIFOSIZE:
            DT_ASSERT(pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_ASI);
            pMatrixCmdOutput->m_Data.m_GetFifoSize.m_FifoSize =
                                                       pNonIpPort->m_Matrix.m_AsiFifoSize;
            break;

        case DTA_MATRIX_CMD_GET_FIFOSIZE_MAX:
            DT_ASSERT(pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_ASI);
            pMatrixCmdOutput->m_Data.m_GetFifoSizeMax.m_FifoSize = 16*1024*1024;
            break;

        case DTA_MATRIX_CMD_SET_FIFOSIZE:
            DT_ASSERT(pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_ASI);
            if (pMatrixCmdInput->m_Data.m_SetFifoSize.m_FifoSize%32 != 0)
                Status = DT_STATUS_INVALID_PARAMETER;
            else
            {
                pNonIpPort->m_Matrix.m_AsiFifoSize =
                                         pMatrixCmdInput->m_Data.m_SetFifoSize.m_FifoSize;
                Status =  DtaNonIpMatrixConfigure(pNonIpPort, FALSE);
            }
            break;

        case DTA_MATRIX_CMD_SET_ASI_CTRL:
            DT_ASSERT(pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_ASI);
            Status = DtaNonIpMatrixSetAsiCtrl(pNonIpPort,
                                          pMatrixCmdInput->m_Data.m_SetAsiCtrl.m_AsiCtrl);
            break;

        case DTA_MATRIX_CMD_GET_BUF_CONFIG:

            if (!pNonIpPort->m_CapMatrix)
                Status = DT_STATUS_NOT_SUPPORTED;
            else
            {
                pMatrixCmdOutput->m_Data.m_GetBufConfig.m_VidStd = 
                                               pNonIpPort->m_Matrix.m_FrameProps.m_VidStd;
                pMatrixCmdOutput->m_Data.m_GetBufConfig.m_NumFrames = 
                                       pNonIpPort->m_Matrix.m_BufConfig.m_NumUsableFrames;

                Status = DT_STATUS_OK;
            }
            break;

        case DTA_MATRIX_CMD_GET_REQ_DMA_SIZE:
        {
            DtaMatrixMemTrSetup  MemTrSetup;
            
            // If it is an output we are writing
            MemTrSetup.m_IsWrite = 
                   (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_OUTPUT);
            MemTrSetup.m_Frame = pMatrixCmdInput->m_Data.m_GetReqDmaSize.m_Frame;
            MemTrSetup.m_StartLine = pMatrixCmdInput->m_Data.m_GetReqDmaSize.m_Line;
            MemTrSetup.m_NumLines = pMatrixCmdInput->m_Data.m_GetReqDmaSize.m_NumLines;
            MemTrSetup.m_TrCmd = pMatrixCmdInput->m_Data.m_GetReqDmaSize.m_TrCmd;
            MemTrSetup.m_DataFormat = 
                                     pMatrixCmdInput->m_Data.m_GetReqDmaSize.m_DataFormat;
            MemTrSetup.m_RgbMode = pMatrixCmdInput->m_Data.m_GetReqDmaSize.m_RgbMode;
            MemTrSetup.m_SymFlt = pMatrixCmdInput->m_Data.m_GetReqDmaSize.m_SymFlt;
            MemTrSetup.m_Scaling = pMatrixCmdInput->m_Data.m_GetReqDmaSize.m_Scaling;
            MemTrSetup.m_AncFlt = pMatrixCmdInput->m_Data.m_GetReqDmaSize.m_AncFlt;
            MemTrSetup.m_Stride = pMatrixCmdInput->m_Data.m_GetReqDmaSize.m_Stride;

            Status = DtaNonIpMatrixGetReqDmaSize(pNonIpPort, &MemTrSetup,
                                     &pMatrixCmdOutput->m_Data.m_GetReqDmaSize.m_ReqSize);
            break;
        }

        case DTA_MATRIX_CMD_GET_FRM_INFO:
        {
            Int  FrameIdx = DtaNonIpMatrixFrame2Index(pNonIpPort,
                                            pMatrixCmdInput->m_Data.m_GetFrmInfo.m_Frame);
            pMatrixCmdOutput->m_Data.m_GetFrmInfo.m_RfClkLatched =
                                 pNonIpPort->m_Matrix.m_FrameInfo[FrameIdx].m_RefClkStart;
            break;
        }

        case DTA_MATRIX_CMD_GET_FRM_PROPS:
        {
            Status = DtAvGetFrameProps(pMatrixCmdInput->m_Data.m_GetFrmProps.m_VidStd,
                                                 &pMatrixCmdOutput->m_Data.m_GetFrmProps);
            break;
        }

        case DTA_MATRIX_CMD_START_MAN:
            pNonIpPort->m_Matrix.m_Vpid1 = 0;
            pNonIpPort->m_Matrix.m_Vpid2 = 0;
            Status = DtaNonIpMatrixStart(pNonIpPort, 
                                             pMatrixCmdInput->m_Data.m_Start.m_StartFrame,
                                             FALSE, FALSE);
            break;

        case DTA_MATRIX_CMD_SET_NEXT_FRAME:
            Status = DtaNonIpMatrixSetNextFrame(pNonIpPort, 
                                        pMatrixCmdInput->m_Data.m_SetNextFrm.m_NextFrame);
            break;

        case DTA_MATRIX_CMD_GET_FRM_INFO2:
        {
            Int  FrameIdx = DtaNonIpMatrixFrame2Index(pNonIpPort,
                                            pMatrixCmdInput->m_Data.m_GetFrmInfo.m_Frame);
            pMatrixCmdOutput->m_Data.m_GetFrmInfo2.m_RfClkLatchedStart =
                                 pNonIpPort->m_Matrix.m_FrameInfo[FrameIdx].m_RefClkStart;
            pMatrixCmdOutput->m_Data.m_GetFrmInfo2.m_RfClkLatchedEnd =
                                   pNonIpPort->m_Matrix.m_FrameInfo[FrameIdx].m_RefClkEnd;
            break;
        }

        case DTA_MATRIX_CMD_WAIT_FRAME2:
            Status = DtaMatrixWaitFrame(pDvcData, pNonIpPort, 
                               pMatrixCmdInput->m_Data.m_WaitFrame.m_FrmIntCnt,
                               pMatrixCmdInput->m_Data.m_WaitFrame.m_Timeout,
                               &pMatrixCmdOutput->m_Data.m_WaitFrame2.m_Frame,
                               &pMatrixCmdOutput->m_Data.m_WaitFrame2.m_RfClkLatchedStart,
                               &pMatrixCmdOutput->m_Data.m_WaitFrame2.m_RfClkLatchedEnd,
                               &pMatrixCmdOutput->m_Data.m_WaitFrame2.m_FrmIntCnt, NULL);
            break;
            
        case DTA_MATRIX_CMD_START2: {
            Int  Flags = pMatrixCmdInput->m_Data.m_Start2.m_StartFlags;
            Bool  AutoMode = (Flags&DTA_MATRIX_STARTFLAGS_MANUAL)==0;
            Bool  ForceRestart = (Flags&DTA_MATRIX_STARTFLAGS_FORCE_RESTART)!=0;
            pNonIpPort->m_Matrix.m_Vpid1 = pMatrixCmdInput->m_Data.m_Start2.m_Vpid;
            pNonIpPort->m_Matrix.m_Vpid2 = 0;
            Status = DtaNonIpMatrixStart(pNonIpPort,
                                            pMatrixCmdInput->m_Data.m_Start2.m_StartFrame,
                                            AutoMode, ForceRestart);
            break;
        }
            
        case DTA_MATRIX_CMD_START3: {
            Int  Flags = pMatrixCmdInput->m_Data.m_Start3.m_StartFlags;
            Bool  AutoMode = (Flags&DTA_MATRIX_STARTFLAGS_MANUAL)==0;
            Bool  ForceRestart = (Flags&DTA_MATRIX_STARTFLAGS_FORCE_RESTART)!=0;
            pNonIpPort->m_Matrix.m_Vpid1 = pMatrixCmdInput->m_Data.m_Start3.m_Vpid;
            pNonIpPort->m_Matrix.m_Vpid2 = pMatrixCmdInput->m_Data.m_Start3.m_Vpid2;
            pNonIpPort->m_Matrix.m_ExtraPixelDelay = 
                                       pMatrixCmdInput->m_Data.m_Start3.m_ExtraPixelDelay;
            Status = DtaNonIpMatrixStart(pNonIpPort,
                                            pMatrixCmdInput->m_Data.m_Start3.m_StartFrame,
                                            AutoMode, ForceRestart);
            break;

        case DTA_MATRIX_CMD_WAIT_FRAME3:
            Status = DtaMatrixWaitFrame(pDvcData, pNonIpPort, 
                               pMatrixCmdInput->m_Data.m_WaitFrame.m_FrmIntCnt,
                               pMatrixCmdInput->m_Data.m_WaitFrame.m_Timeout,
                               &pMatrixCmdOutput->m_Data.m_WaitFrame3.m_Frame,
                               &pMatrixCmdOutput->m_Data.m_WaitFrame3.m_RfClkLatchedStart,
                               &pMatrixCmdOutput->m_Data.m_WaitFrame3.m_RfClkLatchedEnd,
                               &pMatrixCmdOutput->m_Data.m_WaitFrame3.m_FrmIntCnt,
                               &pMatrixCmdOutput->m_Data.m_WaitFrame3.m_TopHalf);
            break;

        case DTA_MATRIX_CMD_GET_VPID:
            Status = DtaMatrixGetVpid(pNonIpPort,
                                             &pMatrixCmdOutput->m_Data.m_GetVpid.m_Vpid,
                                             &pMatrixCmdOutput->m_Data.m_GetVpid.m_Vpid2);
            break;
        }

        default:
            Status = DT_STATUS_NOT_SUPPORTED;
        }
    }

    // If we failed, no data has to be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED)
            DtDbgOut(MIN, MATRIX, "MatrixCmd=0x%x: NOT SUPPORTED", 
                                                                 pMatrixCmdInput->m_Cmd);
        else if (Status == DT_STATUS_IO_PENDING)
            DtDbgOut(MAX, MATRIX, "%s: ERROR %xh", pCmdStr, Status); // NOT A REAL ERROR
        else
            DtDbgOut(MIN, MATRIX, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMatrixSyncInfoGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaMatrixSyncInfoGet(
    DtaDeviceData*  pDvcData,
    DtaIoctlMatrixCmdGetSyncInfoOutput*  pInfo)
{
    DT_ASSERT(pDvcData->m_Matrix.m_IsSupported);

    // Get lock, before copying info
    DtSpinLockAcquire(&pDvcData->m_Matrix.m_SyncInfoSpinLock);
    *pInfo = pDvcData->m_Matrix.m_SyncInfo;
    DtSpinLockRelease(&pDvcData->m_Matrix.m_SyncInfoSpinLock);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMatrixWaitFrame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaMatrixWaitFrame(
    DtaDeviceData*  pDvcData,
    DtaNonIpPort*  pNonIpPort,
    Int64  WaitFrame, 
    Int  Timeout,
    Int64*  pFrame,
    Int64*  pRefClkStart,
    Int64*  pRefClkEnd,
    Int64*  pFrmIntCnt,
    Int*  pTopHalf)
{
    DtStatus  Status = DT_STATUS_OK;
    DtEvent* pWaitEv = NULL;
    static const Int  MAX_WAIT_TIME_MS = 100;
    volatile Int64*  pSofOrLastFrame = NULL;
    Int  FrmBufIdx;

    DT_ASSERT(pDvcData != NULL);

    if (pNonIpPort == NULL)
    {
        pWaitEv = &pDvcData->m_Matrix.m_SofFrameIntEvent;
        pSofOrLastFrame = &pDvcData->m_Matrix.m_SofFrame;
    }
    else
    {
        pWaitEv = &pNonIpPort->m_Matrix.m_LastFrameIntEvent;
        pSofOrLastFrame = &pNonIpPort->m_Matrix.m_LastFrame;
    }

    // Clear the event in case we have a previous unhandled event
    DtEventReset(pWaitEv);

    if (WaitFrame==-1 || pNonIpPort->m_Matrix.m_FrmIntCnt<WaitFrame)
    {
        while (Timeout>0)
        {
            Status = DtEventWait(pWaitEv, MAX_WAIT_TIME_MS);
            if (Status==DT_STATUS_TIMEOUT && Timeout<=MAX_WAIT_TIME_MS)
            {
                *pFrame = -1;   // Set frame to -1 to signal timeout
                return DT_STATUS_TIMEOUT;
            }
            else if (Status == DT_STATUS_TIMEOUT)
                Timeout -= MAX_WAIT_TIME_MS;
            else {
                Timeout -= 40;  // subtract the typical frame rate
                // -1 indicates we wait for the next frame
                if (WaitFrame == -1)
                    break;
            }
            if (WaitFrame!=-1 && *pSofOrLastFrame>=WaitFrame)
                break;
        }
    }
    
    // Return the current frame
    *pFrame = *pSofOrLastFrame;

    FrmBufIdx = DtaNonIpMatrixFrame2Index(pNonIpPort, *pFrame);
    *pRefClkStart = pNonIpPort->m_Matrix.m_FrameInfo[FrmBufIdx].m_RefClkStart;
    *pRefClkEnd = pNonIpPort->m_Matrix.m_FrameInfo[FrmBufIdx].m_RefClkEnd;
    *pFrmIntCnt = pNonIpPort->m_Matrix.m_FrmIntCnt;
    if (pTopHalf != NULL)
        *pTopHalf = pNonIpPort->m_Matrix.m_FrameInfo[FrmBufIdx].m_TopHalf;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMatrixGetVpid -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaMatrixGetVpid(DtaNonIpPort*  pNonIpPort, UInt* pVpid, UInt* pVpid2)
{
    if (!pNonIpPort->m_CapMatrix2)
    {
        *pVpid = 0;
        *pVpid2 = 0;
    } else {
        *pVpid = DtaRegHdSdiFormat1Get(pNonIpPort->m_pRxRegs);
        *pVpid2 = DtaRegHdSdiFormat2Get(pNonIpPort->m_pRxRegs);
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMatrixAsiRxGetFifoLoad -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Get the fifoload for a matrix port configured as ASI input and set the overflow
// flag if required.
//
DtStatus  DtaMatrixAsiRxGetFifoLoad(DtaNonIpPort*  pNonIpPort, Int*  pFifoLoad)
{
    Int64  RealLoad = (DtaRegHdAsiByteCountGet(pNonIpPort->m_pTxRegs) -
                                        pNonIpPort->m_Matrix.m_AsiDmaNumBytes);
                
    // Can only transfer multiple of 32-byte, only count multiples of 32-byte
    RealLoad &= ~0x1F;
    // Worst case 32kB could be stuck in pipeline of the hardware and is not 
    // ready for reading yet so do not include the last 32kB in the reported 
    // FIFO load
    if (RealLoad > 32*1024)
    {
        RealLoad -= 32*1024;
        if (RealLoad > pNonIpPort->m_Matrix.m_AsiFifoSize)
        {
            RealLoad = pNonIpPort->m_Matrix.m_AsiFifoSize;
            DtSpinLockAcquire(&pNonIpPort->m_FlagsSpinLock);
            pNonIpPort->m_Flags |= DTA_RX_FIFO_OVF;
            pNonIpPort->m_FlagsLatched |= DTA_RX_FIFO_OVF;
            DtSpinLockRelease(&pNonIpPort->m_FlagsSpinLock);
        }
    } else
        RealLoad = 0;

    *pFifoLoad = (Int)RealLoad;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaMatrixSofFrameIntDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaMatrixSofFrameIntDpc(DtDpcArgs* pArgs)
{
    Int  i, n;

    DtaDeviceData*  pDvcData = (DtaDeviceData*)pArgs->m_pContext;
    Int64  SofFrame = (Int64)pArgs->m_Data1.m_UInt64;

    DtDbgOut(MAX, GENL, "New Start-of-Frame=%lld", SofFrame);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Latch SOF sync info -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    DtSpinLockAcquireAtDpc(&pDvcData->m_Matrix.m_SyncInfoSpinLock);

    pDvcData->m_Matrix.m_SyncInfo.m_SofFrame = SofFrame;
    pDvcData->m_Matrix.m_SyncInfo.m_NumChannels = 0;

    for (i=0, n=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        DtaNonIpPort*  pNonIpPort = &pDvcData->m_pNonIpPorts[i];
        if (!pNonIpPort->m_CapMatrix)
            continue;
    
        pDvcData->m_Matrix.m_SyncInfo.m_Channels[n].m_PortIndex = pNonIpPort->m_PortIndex;
        pDvcData->m_Matrix.m_SyncInfo.m_Channels[n].m_SofFrame = 
                                                          pNonIpPort->m_Matrix.m_SofFrame;
        pDvcData->m_Matrix.m_SyncInfo.m_Channels[n].m_SofLine = 
                                                           pNonIpPort->m_Matrix.m_SofLine;

        n++;
    }
    pDvcData->m_Matrix.m_SyncInfo.m_NumChannels = n;
    pDvcData->m_Matrix.m_SyncInfo.m_IsValid = TRUE;

    DtSpinLockReleaseFromDpc(&pDvcData->m_Matrix.m_SyncInfoSpinLock);
    
    // Finaly: set SOF frame event and sync event
    DtEventSet(&pDvcData->m_Matrix.m_SofFrameIntEvent);
    DtEventSet(&pDvcData->m_Matrix.m_SofFrameSyncEvent);
}

