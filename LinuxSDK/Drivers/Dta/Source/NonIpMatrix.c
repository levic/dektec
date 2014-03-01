//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NonIpMatrix.c *#*#*#*#*#*#*#*#*#*#* (C) 2013 DekTec
//
// Dta driver - Non IP Matrix channel functionality - Implements matrix-API specific code
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2013 DekTec Digital Video B.V.
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
#include <DtaIncludes.h>

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Internal functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
static void  DtaNonIpMatrixLastFrameIntDpc(DtDpcArgs* pArgs);
static void  DtaNonIpMatrixRxFlagsIntDpc(DtDpcArgs* pArgs);
static DtStatus  DtaNonIpMatrixConfigureForAsi(DtaNonIpPort* pNonIpPort,
                                                                       Bool  ForceConfig);
static DtStatus  DtaNonIpMatrixConfigureForSdi(DtaNonIpPort* pNonIpPort, 
                                                    Int64  StartFrame, Bool  ForceConfig);
static DtStatus  DtaNonIpMatrixApplyConfig(DtaNonIpPort* pNonIpPort);
static DtStatus  DtaNonIpMatrixApplyBufferConfig(DtaNonIpPort* pNonIpPort);
static DtStatus  DtaNonIpMatrixInitFrameProps(DtaNonIpPort* pNonIpPort);
static DtStatus  DtaNonIpMatrixComputeBufferConfig(DtaNonIpPort* pNonIpPort);
static DtStatus  DtaNonIpMatrixInitFrameBufSectionConfig(DtaNonIpPort* pNonIpPort, 
                                      DtaFrameBufSectionConfig* pSection, UInt32 BaseAddr,  
                                      Int NumFrames, Int  NumLines,  Int  LineNumSymbols);
static Int  DtaNonIpMatrixNumSymbols2LineSizeInMem(DtaNonIpPort* pNonIpPort, 
                                                                         Int  NumSymbols);
static Int  DtaNonIpMatrixNumSymbols2DmaSize(Int  NumSymbols, Int DataFormat, 
                                                                            Int  RgbMode);
static Int  DtaNonIpMatrixGetDmaSize(Int  LineNumS, Int  NumLines, 
                                              Int DataFormat, Int  Scaling, Int  RgbMode);
static DtStatus  DtaNonIpMatrixVidStd2SdiFormat(Int  VidStd, Int* pVideoId,
                                                     Int*  pPictRate, Int*  pProgressive);
static DtStatus  DtaNonIpMatrixSdiFormat2VidStd(Int VideoId, Int  PictRate, 
                                                         Int  Progressive, Int*  pVidStd);
static Bool  DtaNonIpMatrixValidateStartLineAndNumLines(
                               DtaFrameProps* pFrameProps, Int  StartLine, Int  NumLines);
static Int  DtaNonIpMatrixCountMatrixPorts(DtaNonIpPort* pNonIpPort);
static DtStatus  DtaNonIpMatrixWriteBlackFrame(DtaNonIpPort*  pNonIpPort, Int64  Frame);

#ifdef _DEBUG
static const char*  DtaNonIpMatrixState2Str(DtaMatrixPortState State);
#endif

// Table used to convert between video standard and video format
static const  Int  DTA_NONIP_MATRIX_VIDSTD_2_FORMAT[][4] =
{
    // SD Formats
    { DT_VIDSTD_525I59_94, DT_HD_SDIFMT_VIDEOID_SD, DT_HD_SDIFMT_PICTRATE_29_97, 
                                                           DT_HD_SDIFMT_PROGRESSIVE_OFF },
    { DT_VIDSTD_625I50, DT_HD_SDIFMT_VIDEOID_SD, DT_HD_SDIFMT_PICTRATE_25, 
                                                           DT_HD_SDIFMT_PROGRESSIVE_OFF },

    // HD 720 formats
    { DT_VIDSTD_720P23_98, DT_HD_SDIFMT_VIDEOID_HD720, DT_HD_SDIFMT_PICTRATE_23_98, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_720P24, DT_HD_SDIFMT_VIDEOID_HD720, DT_HD_SDIFMT_PICTRATE_24, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_720P25, DT_HD_SDIFMT_VIDEOID_HD720, DT_HD_SDIFMT_PICTRATE_25, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_720P29_97, DT_HD_SDIFMT_VIDEOID_HD720, DT_HD_SDIFMT_PICTRATE_29_97, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_720P30, DT_HD_SDIFMT_VIDEOID_HD720, DT_HD_SDIFMT_PICTRATE_30, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_720P50, DT_HD_SDIFMT_VIDEOID_HD720, DT_HD_SDIFMT_PICTRATE_50, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_720P59_94, DT_HD_SDIFMT_VIDEOID_HD720, DT_HD_SDIFMT_PICTRATE_59_94, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_720P60, DT_HD_SDIFMT_VIDEOID_HD720, DT_HD_SDIFMT_PICTRATE_60, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },

    // HD 1080 formats
    { DT_VIDSTD_1080I50, DT_HD_SDIFMT_VIDEOID_HD1080, DT_HD_SDIFMT_PICTRATE_25, 
                                                           DT_HD_SDIFMT_PROGRESSIVE_OFF },
    { DT_VIDSTD_1080I59_94, DT_HD_SDIFMT_VIDEOID_HD1080, DT_HD_SDIFMT_PICTRATE_29_97, 
                                                           DT_HD_SDIFMT_PROGRESSIVE_OFF },
    { DT_VIDSTD_1080I60, DT_HD_SDIFMT_VIDEOID_HD1080, DT_HD_SDIFMT_PICTRATE_30, 
                                                           DT_HD_SDIFMT_PROGRESSIVE_OFF },
    { DT_VIDSTD_1080P23_98, DT_HD_SDIFMT_VIDEOID_HD1080, DT_HD_SDIFMT_PICTRATE_23_98, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_1080P24, DT_HD_SDIFMT_VIDEOID_HD1080, DT_HD_SDIFMT_PICTRATE_24, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_1080P25, DT_HD_SDIFMT_VIDEOID_HD1080, DT_HD_SDIFMT_PICTRATE_25, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_1080P29_97, DT_HD_SDIFMT_VIDEOID_HD1080, DT_HD_SDIFMT_PICTRATE_29_97, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_1080P30, DT_HD_SDIFMT_VIDEOID_HD1080, DT_HD_SDIFMT_PICTRATE_30, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    // 3G 1080 formats
    { DT_VIDSTD_1080P50, DT_HD_SDIFMT_VIDEOID_3GLVLA, DT_HD_SDIFMT_PICTRATE_50, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_1080P59_94, DT_HD_SDIFMT_VIDEOID_3GLVLA, DT_HD_SDIFMT_PICTRATE_59_94, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_1080P60, DT_HD_SDIFMT_VIDEOID_3GLVLA, DT_HD_SDIFMT_PICTRATE_60, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
};
static const  Int  DTA_NONIP_MATRIX_VIDSTD_2_FORMAT_NUM_ENTRIES = 
sizeof(DTA_NONIP_MATRIX_VIDSTD_2_FORMAT) / sizeof(DTA_NONIP_MATRIX_VIDSTD_2_FORMAT[0]);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

#ifdef WINBUILD
//-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixTxEvtRequestCancelDma -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaNonIpMatrixTxEvtRequestCancelDma(WDFREQUEST WdfRequest)
{
    DtStatus  Status;
    DtaWdfRequestContext*  pRequestContext;
    DtaNonIpPort*  pNonIpPort = NULL;
    DmaChannel*  pDmaCh = NULL;

    DtDbgOut(MIN, NONIP, "Init");
    pRequestContext = WdfObjectGet_DtaWdfRequestContext(WdfRequest);
    DT_ASSERT(pRequestContext != NULL);
    
    pNonIpPort = (DtaNonIpPort*)pRequestContext->m_pData;
    pDmaCh = &pNonIpPort->m_DmaChannel;

    // Possibly do some extra actions:
    // Reset channel/clear fifo, set bitrate, TxCtrl send??
    // ToDo
    if (pDmaCh != NULL)
        Status = DtaDmaAbortDma(pDmaCh);
    DtDbgOut(MIN, NONIP, "Exit");
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixRxEvtRequestCancelDma -.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaNonIpMatrixRxEvtRequestCancelDma(WDFREQUEST WdfRequest)
{
    DtStatus  Status;
    DtaWdfRequestContext*  pRequestContext;
    DtaNonIpPort*  pNonIpPort = NULL;
    DmaChannel*  pDmaCh = NULL;

    DtDbgOut(MIN, NONIP, "Init");
    pRequestContext = WdfObjectGet_DtaWdfRequestContext(WdfRequest);
    DT_ASSERT(pRequestContext != NULL);
    
    pNonIpPort = (DtaNonIpPort*)pRequestContext->m_pData;
    pDmaCh = &pNonIpPort->m_DmaChannel;

    // Possibly do some extra actions:
    // Reset channel/clear fifo, RxCtrl RCV
    // ToDo
    if (pDmaCh != NULL)
        Status = DtaDmaAbortDma(pDmaCh);
    DtDbgOut(MIN, NONIP, "Exit");
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixDmaCompletedWindows -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is executed after the DMA is completed (Windows ONLY!)
// The WdfRequest is completed here
//
void  DtaNonIpMatrixDmaCompletedWindows(DmaChannel* pDmaCh, void* pContext)
{
    WDFREQUEST  WdfRequest = pContext;
    DtaWdfRequestContext*  pRequestContext;
    DtaNonIpPort*  pNonIpPort;
    DtStatus  Status = DT_STATUS_OK;
    UInt  BytesRead = 0;
    Int  TrCmd = 0;
    
    DT_ASSERT(pContext != NULL);
    
    pRequestContext = WdfObjectGet_DtaWdfRequestContext(WdfRequest);
    DT_ASSERT(pRequestContext != NULL);
    
    WdfRequestUnmarkCancelable(WdfRequest);

    if (DtaDmaIsAbortActive(pDmaCh))
        Status = DT_STATUS_CANCELLED;
    
    pNonIpPort = (DtaNonIpPort*)pRequestContext->m_pData;
    TrCmd = pRequestContext->m_Value;

    if (!DT_SUCCESS(Status))
        BytesRead = 0;
    else 
    {
        if (pDmaCh->m_DmaDirection == DT_DMA_DIRECTION_FROM_DEVICE)
            BytesRead = DtaNonIpMatrixDmaReadFinished(pNonIpPort, TrCmd);
        else
            BytesRead = DtaNonIpMatrixDmaWriteFinished(pNonIpPort, TrCmd);
    }
    WdfRequestCompleteWithInformation(WdfRequest, (NTSTATUS)Status, (ULONG_PTR)BytesRead);
}
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpMatrixClose(DtaNonIpPort* pNonIpPort, DtFileObject* pFile)
{
    DtStatus  HasAccess;
    DT_ASSERT(pNonIpPort!=NULL && pNonIpPort->m_CapMatrix);

    DtFastMutexAcquire(&pNonIpPort->m_pDvcData->m_ExclAccessMutex);
    HasAccess = DtaNonIpHasAccess(pNonIpPort, pFile);
    DtFastMutexRelease(&pNonIpPort->m_pDvcData->m_ExclAccessMutex);

    if (DT_SUCCESS(HasAccess))
    {
        DtMutexAcquire(&pNonIpPort->m_Matrix.m_StateLock, -1);
    
        if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_ASI)
        {
            if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_INPUT)
                DtaNonIpMatrixSetAsiCtrl(pNonIpPort, DT_RXCTRL_IDLE);
            else
                DtaNonIpMatrixSetAsiCtrl(pNonIpPort, DT_TXCTRL_IDLE);
        } else {
            // Reset the state to HOLD when running
            if (pNonIpPort->m_Matrix.m_State == MATRIX_PORT_RUN)
                DtaNonIpMatrixSetState(pNonIpPort, MATRIX_PORT_HOLD);
        }

        DtMutexRelease(&pNonIpPort->m_Matrix.m_StateLock);
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixConfigure -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpMatrixConfigure(DtaNonIpPort* pNonIpPort, Bool  ForceConfig)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  IoStdValue = pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value;

    DT_ASSERT(pNonIpPort->m_CapMatrix);

    DtDbgOut(MIN, NONIP, "Configure HD-Channel: DTA-%d, PortIdx %d", 
                                           pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber,
                                                                 pNonIpPort->m_PortIndex);

    if (IoStdValue == DT_IOCONFIG_ASI)
        Status = DtaNonIpMatrixConfigureForAsi(pNonIpPort, ForceConfig);
    else if (IoStdValue == DT_IOCONFIG_SDI || IoStdValue == DT_IOCONFIG_HDSDI
                                                       || IoStdValue == DT_IOCONFIG_3GSDI)
    {
        DtMutexAcquire(&pNonIpPort->m_Matrix.m_StateLock, -1);
        Status = DtaNonIpMatrixConfigureForSdi(pNonIpPort, DTA_FRMBUF_HOLD_FRAME, 
                                                                             ForceConfig);
        DtMutexRelease(&pNonIpPort->m_Matrix.m_StateLock);
    }
    else
        DT_ASSERT(1 == 0);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixDetectVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpMatrixDetectVidStd(DtaNonIpPort* pNonIpPort, Int*  pVidStd)
{
    DtStatus  Status = DT_STATUS_OK;
    volatile UInt8*  pHdRegs = NULL;
    DtaMatrixPort*  pMatrix = NULL;
    
    DT_ASSERT(pNonIpPort!=NULL && pVidStd!=NULL);
    DT_ASSERT(pNonIpPort->m_CapMatrix);

    pMatrix = &pNonIpPort->m_Matrix;
    pHdRegs = pNonIpPort->m_pRxRegs;
    
    // Init to 'safe' value
    *pVidStd = DT_VIDSTD_UNKNOWN;

    // Must be matrix capable
    if (!pNonIpPort->m_CapMatrix)
    {
        DtDbgOut(ERR, NONIP, "Port %d is not matrix capable", pNonIpPort->m_PortIndex+1);
        return DT_STATUS_NOT_SUPPORTED;
    }
    // Port must be configured as an input
    if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value!=DT_IOCONFIG_INPUT || 
                     pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue!=DT_IOCONFIG_INPUT)
    {
        DtDbgOut(ERR, NONIP, "Port %d is not configured as input", 
                                                               pNonIpPort->m_PortIndex+1);
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Do we have a GS2961 chip?
    if (pNonIpPort->m_AsiSdiDeserItfType == ASI_SDI_DESER_ITF_GS2961)
    {
        // Get the video standard from the GS2961 chip 
        Status = DtaGs2961GetVideoStd(pNonIpPort, pVidStd);
        if (!DT_SUCCESS(Status))
            return Status;
    }
    else if (pNonIpPort->m_AsiSdiDeserItfType == ASI_SDI_DESER_ITF_FPGA_BASED)
    {
        // FPGA based => get the video standard from SDI status register
        Int  VideoId = DtaRegHdSdiStatusGetVideoId(pHdRegs);
        Int  PictRate = DtaRegHdSdiStatusGetPictureRate(pHdRegs);
        Int  Progressive = DtaRegHdSdiStatusGetProgressive(pHdRegs);
               
        // Convert SMPTE video standard to our video standard
        DtaNonIpMatrixSdiFormat2VidStd(VideoId, PictRate, Progressive, pVidStd);
        
        // HW can not detect fractional formats while set to non-fractional mode and vice
        // versa, so toggle between the two and check again. NOTE: only  if the channel 
        // is not in started and we have a carrier (no carrier => no use to look for a
        // a signal)
        if (*pVidStd==DT_VIDSTD_UNKNOWN && DtaRegHdStatGetCarrierDetect(pHdRegs)!=0)
        {
            Int  l, VidStdToggle = DT_VIDSTD_UNKNOWN;
            DtaMatrixPortState  State;

            DtDbgOut(AVG, NONIP, "Cannot detect vidstd; will try toggling frac-mode"); 

            //.-.- Step 1: get the state lock and confirm the channel is not running -.-.-
            
            // Get the state lock (do not wait for lock)
            Status = DtMutexAcquire(&pMatrix->m_StateLock, 0);
            if (Status != DT_STATUS_OK)
                return DT_STATUS_OK;

            State = pMatrix->m_State;

            // Check state, to see port is configured and not running; if not donot 
            // toggle fractional mode
            if (State!=MATRIX_PORT_IDLE && State!=MATRIX_PORT_HOLD)
            {
                DtMutexRelease(&pMatrix->m_StateLock);
                return DT_STATUS_OK;
            }

            //-.-.-.-.-.-.- Step 2: check if current standard is fractional -.-.-.-.-.-.-.

            // Check if the current video standard is fractional
            if (DtaVidStdIsFractional(pMatrix->m_FrameProps.m_VidStd))
            {
                // Toggle to a non-fractional format
                DT_ASSERT(pNonIpPort->m_Cap1080I50);
                VidStdToggle = DT_VIDSTD_1080I50;
            }
            else
            {
                // Toggle to a fractional format
                DT_ASSERT(pNonIpPort->m_Cap1080I59_94);
                VidStdToggle = DT_VIDSTD_1080I59_94;
            }

            //.-.-.-.-.-.-.-.-.-.- Step 3: toggle the fractional mode -.-.-.-.-.-.-.-.-.-.

            DtaNonIpMatrixVidStd2SdiFormat(VidStdToggle, &VideoId, &PictRate, 
                                                                            &Progressive);

            DtaRegHdSdiFormatSetVideoId(pHdRegs, VideoId);
            DtaRegHdSdiFormatSetPictureRate(pHdRegs, PictRate);
            DtaRegHdSdiFormatSetProgressive(pHdRegs, Progressive);

            DtDbgOut(AVG, NONIP, "Toggling SDI format to: id=%d, r=%d, p=%d", VideoId, 
                                                                   PictRate, Progressive);

            //-.-.-.-.-.- Step 4: wait shortly and check which standard again  -.-.-.-.-.-

            // Initial wait to allow channel time to settle
            DtSleep(100);
            
            // Check if we can detect a valid video standard (Max. 10x 40ms)
            for (l=0; l<10; l++)
            {
                VideoId = DtaRegHdSdiStatusGetVideoId(pHdRegs);
                PictRate = DtaRegHdSdiStatusGetPictureRate(pHdRegs);
                Progressive = DtaRegHdSdiStatusGetProgressive(pHdRegs);
               
                // Convert SMPTE video standard to our video standard
                DtaNonIpMatrixSdiFormat2VidStd(VideoId, PictRate, Progressive, pVidStd);
                DtDbgOut(MAX, NONIP, "Loop %d: id=%d, r=%d, p=%d => std=%d", 
                                             l, VideoId, PictRate, Progressive, *pVidStd);

                if (*pVidStd != DT_VIDSTD_UNKNOWN)
                    break;

                // Nothing yet, so wait a little more and try again
                DtSleep(40);
            }

            //-.-.-.-.-.-.-.- Step 5: finally restore the original setting -.-.-.-.-.-.-.-

            // Restore original SDI format
            DtaNonIpMatrixVidStd2SdiFormat(pMatrix->m_FrameProps.m_VidStd, 
                                                       &VideoId, &PictRate, &Progressive);
            
            DtaRegHdSdiFormatSetVideoId(pHdRegs, VideoId);
            DtaRegHdSdiFormatSetPictureRate(pHdRegs, PictRate);
            DtaRegHdSdiFormatSetProgressive(pHdRegs, Progressive);

            DtDbgOut(MAX, NONIP, "Restore SDI format to: id=%d, r=%d, p=%d", VideoId, 
                                                                   PictRate, Progressive);

            // Release lock
            DtMutexRelease(&pMatrix->m_StateLock);
        }
#ifdef _DEBUG
        else if (DtaRegHdStatGetCarrierDetect(pHdRegs)==0)
        {
            DtDbgOut(AVG, NONIP, "No carrier => skip toggling frac-mode"); 
        }
#endif
    }
    else
    {
        // For now this function is not supported for any other boards
        DtDbgOut(ERR, NONIP, "Not supported for this port %d", pNonIpPort->m_PortIndex+1);
        return DT_STATUS_FAIL;  
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpMatrixInit(DtaNonIpPort* pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaMatrixPort*  pMatrix = &pNonIpPort->m_Matrix;
    DtaFrameProps*  pFrameProps = &pNonIpPort->m_Matrix.m_FrameProps;
    
    DT_ASSERT(pNonIpPort->m_CapMatrix);
    
    Status = DtDpcInit(&pMatrix->m_LastFrameIntDpc, DtaNonIpMatrixLastFrameIntDpc, TRUE);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtEventInit(&pMatrix->m_LastFrameIntEvent, FALSE);
    if (!DT_SUCCESS(Status))
        return Status;

    Status = DtDpcInit(&pMatrix->m_RxErrIntDpc, DtaNonIpMatrixRxFlagsIntDpc, TRUE);
    if (!DT_SUCCESS(Status))
        return Status;
        
    pMatrix->m_LastFrame = 0;
    pMatrix->m_SofFrame = 0;
    pMatrix->m_SofLine = 0;
    // By default associate port to its own row
    pMatrix->m_RowIdx = pNonIpPort->m_PortIndex;

    // Initial state is uninitialised
    DtMutexInit(&pMatrix->m_StateLock);
    pMatrix->m_State = MATRIX_PORT_UNINIT;
    // One time intialisation of last state seen by interrupt routine
    pMatrix->m_LastStateAtInt = MATRIX_PORT_UNINIT;  

    pMatrix->m_AsiFifoSize = 16 * 1024 * 1024;

    // Set to unknown video standard
    pFrameProps->m_VidStd = DT_VIDSTD_UNKNOWN;  
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixInterruptEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Enables Non-IP Matrix port specific interrupts
//
DtStatus  DtaNonIpMatrixInterruptEnable(DtaNonIpPort* pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaDeviceData*  pDvcData = NULL;
    Bool  IsAsi=FALSE, IsSdi=FALSE;
    DtIntEnableState  IntEnaState;

    DT_ASSERT(pNonIpPort != NULL);
    DT_ASSERT(pNonIpPort->m_CapMatrix);
        
    pDvcData =  pNonIpPort->m_pDvcData;
    
    IntEnaState = pDvcData->m_IntEnableState;

    // Do nothing if master state is not enabled or enabling
    if (IntEnaState!=INT_ENABLING && IntEnaState!=INT_ENABLED)
    {
        DtDbgOut(AVG, NONIP, "Master-interupt state (%d) is not enabled", IntEnaState);
        return DT_STATUS_OK;
    }

    // Check port configuration
    IsAsi = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_ASI);
    IsSdi = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value==DT_IOCONFIG_SDI
                    || pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value==DT_IOCONFIG_HDSDI
                    || pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value==DT_IOCONFIG_3GSDI);
    DT_ASSERT(IsAsi != IsSdi);  // They are mutual exclusive

    // Last-frame interrupt (enable if port is configued for SDI
    DtaRegHdCtrl1SetLastFrameIntEn(pNonIpPort->m_pRxRegs, IsSdi ? 1 : 0);

    // For ports that support ASI also enable the RX sync and RX ovf interrupts
    if (pNonIpPort->m_CapAsi)
    {
        DtaRegHdCtrl1SetRxSyncErrIntEn(pNonIpPort->m_pRxRegs, IsAsi ? 1 : 0);
        DtaRegHdCtrl1SetRxOvfErrIntEn(pNonIpPort->m_pRxRegs, IsAsi ? 1 : 0);
    }
    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixInterruptDisable -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Disables Non-IP Matrix port specific interrupts
//
DtStatus  DtaNonIpMatrixInterruptDisable(DtaNonIpPort* pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;

    DT_ASSERT(pNonIpPort != NULL);
    DT_ASSERT(pNonIpPort->m_CapMatrix);
    
    // Last-frame interrupts
    DtaRegHdCtrl1SetLastFrameIntEn(pNonIpPort->m_pRxRegs, 0);
        
    // For ports that support ASI also disable the RX sync and RX ovf interrupts
    if (pNonIpPort->m_CapAsi)
    {
        DtaRegHdCtrl1SetRxSyncErrIntEn(pNonIpPort->m_pRxRegs, 0);
        DtaRegHdCtrl1SetRxOvfErrIntEn(pNonIpPort->m_pRxRegs, 0);
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixAttachToRow -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpMatrixAttachToRow(DtaNonIpPort* pNonIpPort, Int  RowIdx)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  NumMatrixPorts = 0;
    DtaMatrixPort*  pMatrix = NULL;

    DT_ASSERT(pNonIpPort != NULL);
    DT_ASSERT(pNonIpPort->m_CapMatrix);

    pMatrix = &pNonIpPort->m_Matrix;

    // State must be on IDLE or HOLD
    if (pMatrix->m_State!=MATRIX_PORT_IDLE && pMatrix->m_State!=MATRIX_PORT_HOLD)
    {
        DtDbgOut(ERR, NONIP, "Cannot attach to a row in current state: %s", 
                                               DtaNonIpMatrixState2Str(pMatrix->m_State));
        return DT_STATUS_FAIL;
    }
    
    // Check ROW index is valid.
    NumMatrixPorts = DtaNonIpMatrixCountMatrixPorts(pNonIpPort);
    if (RowIdx<0 || RowIdx>=NumMatrixPorts)
    {
        DtDbgOut(ERR, NONIP, "Invalid row index (%d)", RowIdx);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Check if row index has changed
    if (RowIdx != pMatrix->m_RowIdx)
    {
        DtDbgOut(MIN, NONIP, "Row index changes (%d->%d); will re-configuring", 
                                                               pMatrix->m_RowIdx, RowIdx);
        
        // Set new row and force a re-configure the port
        pMatrix->m_RowIdx = RowIdx;
        Status = DtaNonIpMatrixConfigure(pNonIpPort, TRUE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, NONIP, "Failed to reconfigure port (Status=0x%08X)", Status);
            return Status;
        }
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixSetAsiCtrl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpMatrixSetAsiCtrl(DtaNonIpPort* pNonIpPort, Int  AsiCtrl)
{
    Int  RxTxMode=0;
    if (pNonIpPort->m_Matrix.m_AsiCtrl == AsiCtrl)
        return DT_STATUS_OK;
   
    if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_INPUT)
    {
        switch (AsiCtrl)
        {
        case DT_RXCTRL_IDLE: RxTxMode = 0; break;
        case DT_RXCTRL_RCV: RxTxMode = 3; break;
        default: return DT_STATUS_INVALID_PARAMETER;
        }
    } else { // Output
        switch (AsiCtrl)
        {
        case DT_TXCTRL_IDLE: RxTxMode = 0; break;
        case DT_TXCTRL_HOLD: RxTxMode = 1; break;
        case DT_TXCTRL_SEND: RxTxMode = 3; break;
        default: return DT_STATUS_INVALID_PARAMETER;
        }
    }

    if (pNonIpPort->m_Matrix.m_AsiCtrl == DT_TXCTRL_IDLE)
    {
        // From IDLE to HOLD or SEND/RCV: reset fifo buffer
        pNonIpPort->m_Matrix.m_AsiDmaNumBytes = 0;
        pNonIpPort->m_Matrix.m_AsiDmaOffset = 0;
        // Channel reset to clear internal fifos
        DtaRegHdCtrl1SetIoReset(pNonIpPort->m_pTxRegs, 1);
        // WORKAROUND FOR FW BUG: clear ASI num bytes written count, to prevent that FW 
        // thinks we already have written any data
        DtaRegHdSetAsiControl1(pNonIpPort->m_pTxRegs, 0);
        // Clear ASI byte count register
        DtaRegHdAsiByteCountSet(pNonIpPort->m_pTxRegs, 0);
        // Initialize S0 next frame address.
        DtaRegHdS0NextFrmAddrSet(pNonIpPort->m_pTxRegs,
                                           DtaRegHdS0BeginAddrGet(pNonIpPort->m_pTxRegs));
    }

    DtDbgOut(AVG, NONIP, "New Asi ctrl: %d", AsiCtrl);
    pNonIpPort->m_Matrix.m_AsiCtrl = AsiCtrl;
    DtaRegHdCtrl1SetRxTxCtrl(pNonIpPort->m_pTxRegs, RxTxMode);

    if (AsiCtrl != DT_TXCTRL_SEND)
    {
        // Reset flags but not the latched flags
        DtSpinLockAcquire(&pNonIpPort->m_FlagsSpinLock);
        pNonIpPort->m_Flags = 0;
        pNonIpPort->m_TxUfl = FALSE;
        DtSpinLockRelease(&pNonIpPort->m_FlagsSpinLock);
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixSetState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// PRE: pNonIpPort->m_Matrix.m_StateLock has been acquired
//
DtStatus  DtaNonIpMatrixSetState(DtaNonIpPort* pNonIpPort, DtaMatrixPortState  NewState)
{
    DtaMatrixPortState OldState;
    DtaMatrixPort*  pMatrix = NULL;

    DT_ASSERT(pNonIpPort!=NULL && pNonIpPort->m_CapMatrix);
    
    pMatrix = &pNonIpPort->m_Matrix;

    OldState = pMatrix->m_State;
    pMatrix->m_State = NewState;

    if (OldState != NewState)
        DtDbgOut(AVG, NONIP, "State change %s => %s", DtaNonIpMatrixState2Str(OldState),
                                                       DtaNonIpMatrixState2Str(NewState));
    else
        DtDbgOut(MAX, NONIP, "State remains unchanged: %s",
                                                       DtaNonIpMatrixState2Str(NewState));

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixStart -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpMatrixStart(DtaNonIpPort* pNonIpPort, Int64  StartFrame)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaMatrixPort*  pMatrix = NULL;

    DT_ASSERT(pNonIpPort != NULL);
    DT_ASSERT(pNonIpPort->m_CapMatrix);

    pMatrix = &pNonIpPort->m_Matrix;
    
    DtMutexAcquire(&pMatrix->m_StateLock, -1);
        
    // State cannot be unintialised or configuring
    if (pMatrix->m_State==MATRIX_PORT_UNINIT || pMatrix->m_State==MATRIX_PORT_CONFIGURING)
    {
        DtMutexRelease(&pMatrix->m_StateLock);
        return DT_STATUS_FAIL;
    }

    // For legacy HD channel interface, we need to re-configure and (re-)start the channel
    if (DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
    {
        Status = DtaNonIpMatrixConfigureForSdi(pNonIpPort, StartFrame, TRUE);
        if (!DT_SUCCESS(Status))
        {
            DtMutexRelease(&pMatrix->m_StateLock);
            return Status;
        }
    }
    else
    {
        // Set start frame as forced next frame
        pMatrix->m_NextFrame = StartFrame;
    }

    // Move to new state
    DtaNonIpMatrixSetState(pNonIpPort, MATRIX_PORT_RUN);
    
    DtMutexRelease(&pMatrix->m_StateLock);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixStop -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpMatrixStop(DtaNonIpPort* pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool  IsSdi=FALSE;
    DtaMatrixPort*  pMatrix = NULL;

    DT_ASSERT(pNonIpPort != NULL);
    DT_ASSERT(pNonIpPort->m_CapMatrix);

    pMatrix = &pNonIpPort->m_Matrix;

    DtMutexAcquire(&pMatrix->m_StateLock, -1);

    IsSdi = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_SDI)
                 || (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_HDSDI)
                 || (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_3GSDI);

    // For legacy HD channel interface, we need to re-configure and (re-)start the channel
    if (IsSdi && DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
    {
        // Set the black frame as 'start-frame'; this will result in the channel to
        // start in FREEZE mode
        Status = DtaNonIpMatrixConfigureForSdi(pNonIpPort, DTA_FRMBUF_HOLD_FRAME, TRUE);
        if (!DT_SUCCESS(Status))
        {
            DtMutexRelease(&pMatrix->m_StateLock);
            return Status;
        }
    }
    
    // Finally, simple set the state to HOLD
    DtaNonIpMatrixSetState(pNonIpPort, MATRIX_PORT_HOLD);

    DtMutexRelease(&pMatrix->m_StateLock);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixGetCurrentFrame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpMatrixGetCurrentFrame(DtaNonIpPort* pNonIpPort, Int64*  pFrame)
{
    DtStatus  Status = DT_STATUS_OK;

    DT_ASSERT(pNonIpPort!=NULL && pFrame!=NULL);
    DT_ASSERT(pNonIpPort->m_CapMatrix);

    // For legacy HD-channel: read the current frame directly from the hardware
    if (DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
        *pFrame = DtaRegHdCurrentFrameGet(pNonIpPort->m_pRxRegs);
    else
        *pFrame = pNonIpPort->m_Matrix.m_CurFrame;
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixFrame2Index -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaNonIpMatrixFrame2Index(DtaNonIpPort* pNonIpPort, Int64  Frame)
{
    DT_ASSERT(pNonIpPort != NULL);

    // Are we targetting the hold frame?
    if (Frame == DTA_FRMBUF_HOLD_FRAME)
        return pNonIpPort->m_Matrix.m_BufConfig.m_NumFrames-1; // Return last frame buffer
        
    // NOTE: last frame is reserved for HOLD frame
    return ((UInt32)Frame & 0xFFFFFFFF) % 
                                     (pNonIpPort->m_Matrix.m_BufConfig.m_NumUsableFrames);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixGetFrameAddrInMem -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt32  DtaNonIpMatrixGetFrameAddrInMem(
    DtaNonIpPort* pNonIpPort,
    Int  Section,
    Int64  Frame, 
    Int  Line)
{
    Int  Idx = 0;
    UInt32  FrameAddr = 0;
    DtaFrameProps*  pFrameProps = NULL;
    DtaFrameBufSectionConfig*  pSection = NULL;

    DT_ASSERT(pNonIpPort != NULL);
    pFrameProps = &pNonIpPort->m_Matrix.m_FrameProps;

    // Check a valid line is specified
    if (Line<1 || Line>pFrameProps->m_NumLines)
    {
        DtDbgOut(ERR, NONIP, "Invalid line number: %d", Line);
        return DT_STATUS_FAIL;
    }
        
    DT_ASSERT(Section>=0 && Section<DTA_FRMBUF_NUM_SECTIONS);
    pSection = &pNonIpPort->m_Matrix.m_BufConfig.m_Sections[Section];
    
    Idx = DtaNonIpMatrixFrame2Index(pNonIpPort, Frame);

    DT_ASSERT(Idx>=0 && Idx<pSection->m_NumFrames);

    FrameAddr = (UInt32)(pSection->m_FrameStartAddr[Idx] + 
                                              ((Line-1) * pSection->m_LineNumBytesInMem));

    DtDbgOut(MAX, NONIP, "Address for frame=%lld & line=%d: 0x%08X", Frame, Line, 
                                                                               FrameAddr);
    return FrameAddr;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixPrepForAsiDma -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpMatrixPrepForAsiDma(
    DtaNonIpPort* pNonIpPort,
    Int  BufSize,
    const DtaMatrixMemTrSetup*  pMemTrSetup)
{
    UInt32  AsiStartAddr = 0;
    volatile UInt8*  pHdRegs = pNonIpPort->m_pRxRegs;

    DT_ASSERT(pMemTrSetup->m_TrCmd==DT_MEMTR_TRCMD_ASIRD
                                           || pMemTrSetup->m_TrCmd==DT_MEMTR_TRCMD_ASIWR);

    // Check that tr-size is a multiple of 32
    if (BufSize==0 || (BufSize%32)!=0)
    {
        DtDbgOut(ERR, NONIP, "ASI transfer size (=%d) is not a multiple of 32", BufSize);
        return DT_STATUS_INVALID_PARAMETER;
    }

    
    // Step 1: set the transfer command to IDLE
    DtaRegHdMemTrControlSetTrCmd(pHdRegs, DT_MEMTR_TRCMD_IDLE);
    // Step 2: Set section 0 start address
    AsiStartAddr = pNonIpPort->m_Matrix.m_BufConfig.m_Sections[0].m_BeginAddr
                                                    + pNonIpPort->m_Matrix.m_AsiDmaOffset;
    DtaRegHdS0StartAddrSet(pHdRegs, AsiStartAddr);
    // Step 3: Set number of bytes to transfer
    DtaRegHdSetAsiControl1(pHdRegs, BufSize);
    //DtDbgOut(ERR, DTA, "new %s dma: start=0x%X, size=0x%X", 
    //                          pMemTrSetup->m_TrCmd==DT_MEMTR_TRCMD_ASIRD?"read":"write",
    //                                                             AsiStartAddr, BufSize);
    // Finally: set the actual transfer command to start the transfer
    DtaRegHdMemTrControlSetTrCmd(pHdRegs, pMemTrSetup->m_TrCmd);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixPrepForDma -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpMatrixPrepForDma(
    DtaNonIpPort* pNonIpPort,                   
    Int  BufSize,                               // Size of DMA buffer
    const DtaMatrixMemTrSetup*  pMemTrSetup,    // DMA transfer setup parameters
    Int*  pDmaSize)                             // Actual # of bytes to be DMA-ed
{
    DtStatus  Status = DT_STATUS_OK;
    Int  ReqDmaSize = 0;
    UInt32  MemStartAddr = 0;
    DtaFrameProps*  pFrameProps = NULL;
    DtaFrameBufConfig*  pBufConf = NULL;
    volatile UInt8*  pHdRegs = NULL;
    
    DT_ASSERT(pNonIpPort!=NULL && pNonIpPort->m_CapMatrix);
    DT_ASSERT(pMemTrSetup != NULL);
    DT_ASSERT(pDmaSize != NULL);

    // Init to 'safe' value
    *pDmaSize = 0;

    if (pMemTrSetup->m_TrCmd==DT_MEMTR_TRCMD_ASIRD
                                            || pMemTrSetup->m_TrCmd==DT_MEMTR_TRCMD_ASIWR)
    {
        *pDmaSize = BufSize;
        return DtaNonIpMatrixPrepForAsiDma(pNonIpPort, BufSize, pMemTrSetup);
    }

    pHdRegs = pNonIpPort->m_pRxRegs;
    pFrameProps = &pNonIpPort->m_Matrix.m_FrameProps;
    pBufConf = &pNonIpPort->m_Matrix.m_BufConfig;

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Validate parameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // Check: transfer command
    if (pMemTrSetup->m_TrCmd!=DT_MEMTR_TRCMD_IDLE 
                                            && pMemTrSetup->m_TrCmd!=DT_MEMTR_TRCMD_SECT0 
                                            && pMemTrSetup->m_TrCmd!=DT_MEMTR_TRCMD_SECT1
                                            && pMemTrSetup->m_TrCmd!=DT_MEMTR_TRCMD_FRAME)
    {
        DtDbgOut(ERR, NONIP, "Unsupported transfer command (%d)", pMemTrSetup->m_TrCmd);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check: data format
    if (pMemTrSetup->m_DataFormat!=DT_MEMTR_DATAFMT_8B
                                       && pMemTrSetup->m_DataFormat!=DT_MEMTR_DATAFMT_10B 
                                       && pMemTrSetup->m_DataFormat!=DT_MEMTR_DATAFMT_16B)
    {
        DtDbgOut(ERR, NONIP, "Unsupported data format (%d)", pMemTrSetup->m_DataFormat);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check: RGB-mode
    if (pMemTrSetup->m_RgbMode!=DT_MEMTR_RGBMODE_OFF
                                           && pMemTrSetup->m_RgbMode!=DT_MEMTR_RGBMODE_ON)
    {
        DtDbgOut(ERR, NONIP, "Unsupported data format (%d)", pMemTrSetup->m_DataFormat);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check: symbol filter mode
    if (pMemTrSetup->m_SymFlt!=DT_MEMTR_SYMFLTMODE_ALL
                                       && pMemTrSetup->m_SymFlt!=DT_MEMTR_SYMFLTMODE_CHROM
                                       && pMemTrSetup->m_SymFlt!=DT_MEMTR_SYMFLTMODE_LUM
                                       && pMemTrSetup->m_SymFlt!=DT_MEMTR_SYMFLTMODE_SWAP)
    {
        DtDbgOut(ERR, NONIP, "Unsupported symbol filter mode (%d)", 
                                                                   pMemTrSetup->m_SymFlt);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check: scaling mode
    if (pMemTrSetup->m_Scaling!=DT_MEMTR_SCMODE_OFF
                                          && pMemTrSetup->m_Scaling!=DT_MEMTR_SCMODE_1_4
                                          && pMemTrSetup->m_Scaling!=DT_MEMTR_SCMODE_1_16)
    {
        DtDbgOut(ERR, NONIP, "Unsupported scaling mode (%d)", pMemTrSetup->m_Scaling);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check: ancillary filter mode
    if (pMemTrSetup->m_AncFlt!=DT_MEMTR_ANCFLTMODE_OFF
                                    && pMemTrSetup->m_AncFlt!=DT_MEMTR_ANCFLTMODE_HANCALL
                                    && pMemTrSetup->m_AncFlt!=DT_MEMTR_ANCFLTMODE_VANCALL)
    {
        DtDbgOut(ERR, NONIP, "Unsupported ancillary filter mode (%d)", 
                                                                   pMemTrSetup->m_AncFlt);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check: stride mode
    if (pMemTrSetup->m_Stride > 0)
    {
        DtDbgOut(ERR, NONIP, "Unsupported stride (%d)", pMemTrSetup->m_Stride);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check: line numbers
    if (!DtaNonIpMatrixValidateStartLineAndNumLines(pFrameProps, pMemTrSetup->m_StartLine, 
                                                                 pMemTrSetup->m_NumLines))
    {
        DtDbgOut(ERR, NONIP, "Invalid start line (%d) and/or number of lines (%d)",
                                       pMemTrSetup->m_StartLine, pMemTrSetup->m_NumLines);
        return DT_STATUS_INVALID_PARAMETER;
    }

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Compute req. DMA size -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    Status = DtaNonIpMatrixGetReqDmaSize(pNonIpPort, pMemTrSetup, &ReqDmaSize);
    if (!DT_SUCCESS(Status))
        return Status;
    
    if (pMemTrSetup->m_IsWrite)
    {
        // With the anc filter disabled BufSize>=ReqDmaSize, with anc filter enabled 
        // BufSize<=ReqDmaSize
        if (pMemTrSetup->m_AncFlt == DT_MEMTR_ANCFLTMODE_OFF)
        {
            if (BufSize < ReqDmaSize)
                return DT_STATUS_BUF_TOO_SMALL;
        }
        else
        {
            // compressed size cannot be bigger than uncompressed size
            if (BufSize > ReqDmaSize)
                return DT_STATUS_BUF_TOO_LARGE; 
            ReqDmaSize = BufSize;   // Set DMA size to buffer size
        }
    }
    else
    {
        // For reading the buffer always must be >= ReqDmaSize
        if (BufSize < ReqDmaSize)
            return DT_STATUS_BUF_TOO_SMALL;
    }
    *pDmaSize = ReqDmaSize;
    
    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Initiate Mem Transfer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    
    DtDbgOut(MAX, NONIP, "MemTr: cmd=%d, #l=%d, fmt=%d, rgb=%d, sym=%d, scale=%d, "
                         "ancf=%d, stride=%d; bufsz=%d, reqsz=%d", 
                         pMemTrSetup->m_TrCmd, pMemTrSetup->m_NumLines, 
                         pMemTrSetup->m_DataFormat, pMemTrSetup->m_RgbMode, 
                         pMemTrSetup->m_SymFlt, pMemTrSetup->m_Scaling,
                         pMemTrSetup->m_AncFlt, 
                                              pMemTrSetup->m_Stride, BufSize, ReqDmaSize);
    
    // Step 1: set the transfer command to IDLE
    DtaRegHdMemTrControlSetTrCmd(pHdRegs, DT_MEMTR_TRCMD_IDLE);
    // Step 2: set number of lines
    DtaRegHdMemTrControlSetNumLines(pHdRegs, pMemTrSetup->m_NumLines);
    // Step 3: set data format
    DtaRegHdMemTrControlSetDataFormat(pHdRegs, pMemTrSetup->m_DataFormat);
    // Step 4: set RGB mode
    DtaRegHdMemTrControlSetRgbMode(pHdRegs, pMemTrSetup->m_RgbMode);
    // Step 5: set symbol filter mode
    DtaRegHdMemTrControlSetSymFltMode(pHdRegs, pMemTrSetup->m_SymFlt);
    // Step 6: set scaling mode
    DtaRegHdMemTrControlSetScalingMode(pHdRegs, pMemTrSetup->m_Scaling);
    // Step 7: set ancillary filter mode 
    DtaRegHdMemTrControlSetAncFltMode(pHdRegs, pMemTrSetup->m_AncFlt);
    // Step 8: set stride mode
    DtaRegHdMemTrControlSetStrideMode(pHdRegs, (pMemTrSetup->m_Stride>0) ? 1 : 0);

    // Step 9: set section start address(-es)
    if (pMemTrSetup->m_TrCmd == DT_MEMTR_TRCMD_SECT0)
    {
        MemStartAddr = DtaNonIpMatrixGetFrameAddrInMem(pNonIpPort, 0, 
                                          pMemTrSetup->m_Frame, pMemTrSetup->m_StartLine);
        DtaRegHdS0StartAddrSet(pHdRegs, MemStartAddr);

        DtDbgOut(MAX, NONIP, "Section 0 Start Address: 0x%08X", MemStartAddr);
    }
    else if (pMemTrSetup->m_TrCmd == DT_MEMTR_TRCMD_SECT1)
    {
        MemStartAddr = DtaNonIpMatrixGetFrameAddrInMem(pNonIpPort, 1, 
                                          pMemTrSetup->m_Frame, pMemTrSetup->m_StartLine);
        DtaRegHdS1StartAddrSet(pHdRegs, MemStartAddr);

        DtDbgOut(MAX, NONIP, "Section 1 Start Address: 0x%08X", MemStartAddr);
    }
    else if (pMemTrSetup->m_TrCmd == DT_MEMTR_TRCMD_FRAME)
    {
        MemStartAddr = DtaNonIpMatrixGetFrameAddrInMem(pNonIpPort, 0,
                                          pMemTrSetup->m_Frame, pMemTrSetup->m_StartLine);
        DtaRegHdS0StartAddrSet(pHdRegs, MemStartAddr);
        DtDbgOut(MAX, NONIP, "Section 0 Start Address: 0x%08X", MemStartAddr);
        MemStartAddr = DtaNonIpMatrixGetFrameAddrInMem(pNonIpPort, 1, 
                                          pMemTrSetup->m_Frame, pMemTrSetup->m_StartLine);
        DtaRegHdS1StartAddrSet(pHdRegs, MemStartAddr);
        DtDbgOut(MAX, NONIP, "Section 1 Start Address: 0x%08X", MemStartAddr);
    }

    // For legacy HD-channel we also need to setup number of symbols and number of bytes 
    // to transfer
    if (DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
    {
        Int  LineNumS=0, TotalNumS=0, ScalingFactor=1;

        if (pMemTrSetup->m_Scaling == DT_MEMTR_SCMODE_OFF)
            ScalingFactor = 1;
        else if (pMemTrSetup->m_Scaling == DT_MEMTR_SCMODE_1_4)
            ScalingFactor = 2;
        else if (pMemTrSetup->m_Scaling == DT_MEMTR_SCMODE_1_16)
            ScalingFactor = 4;
        else
            DT_ASSERT(1 == 0);

        // Get number of symbols per line
        if (pMemTrSetup->m_TrCmd == DT_MEMTR_TRCMD_SECT0)
            LineNumS = pBufConf->m_Sections[0].m_LineNumSymbols;
        else if (pMemTrSetup->m_TrCmd == DT_MEMTR_TRCMD_SECT1)
            LineNumS = pBufConf->m_Sections[1].m_LineNumSymbols;
        else if (pMemTrSetup->m_TrCmd == DT_MEMTR_TRCMD_FRAME)
        {
            LineNumS = pBufConf->m_Sections[0].m_LineNumSymbols;
            LineNumS += pBufConf->m_Sections[1].m_LineNumSymbols;
        }

        // Writing compressed ANC is a special case, where we compute the number of 
        // symbols from the DMA size
        if (pMemTrSetup->m_IsWrite && pMemTrSetup->m_AncFlt!=DT_MEMTR_ANCFLTMODE_OFF)
        {
            if (pMemTrSetup->m_DataFormat != DT_MEMTR_DATAFMT_16B)
            {
                DtDbgOut(ERR, NONIP, "Unsupported data format (%d) for compressed ANC",
                                                               pMemTrSetup->m_DataFormat);
                return DT_STATUS_INVALID_PARAMETER;
            }
            TotalNumS = ReqDmaSize / 2; // Compute number of symbols
        }
        else
            TotalNumS = (LineNumS/ScalingFactor) * (pMemTrSetup->m_NumLines/ScalingFactor);
        
        // Set # of symbols + #bytes
        DtaRegHdMemTrNumSymbolsSet(pHdRegs, TotalNumS);
        DtaRegHdMemTrNumBytesSet(pHdRegs, ReqDmaSize);

        // For legacy we should divide the #lines by the scaling factor
        DtaRegHdMemTrControlSetNumLines(pHdRegs, pMemTrSetup->m_NumLines/ScalingFactor);
    }

    // Finally: set the actual transfer command to start the transfer
    DtaRegHdMemTrControlSetTrCmd(pHdRegs, pMemTrSetup->m_TrCmd);
    
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixGetReqDmaSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpMatrixGetReqDmaSize(
    DtaNonIpPort* pNonIpPort, 
    const DtaMatrixMemTrSetup*  pMemTrSetup,
    Int*  pReqDmaSize)
{
    Int  LineNumS = 0;
    DtaFrameProps*  pFrameProps = NULL;
    DtaFrameBufConfig*  pBufConf = NULL;

    DT_ASSERT(pNonIpPort!=NULL && pNonIpPort->m_CapMatrix);
    DT_ASSERT(pMemTrSetup != NULL);

    pFrameProps = &pNonIpPort->m_Matrix.m_FrameProps;
    pBufConf = &pNonIpPort->m_Matrix.m_BufConfig;
    
    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Validate parameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // Check: transfer command
    if (pMemTrSetup->m_TrCmd!=DT_MEMTR_TRCMD_IDLE 
                                            && pMemTrSetup->m_TrCmd!=DT_MEMTR_TRCMD_SECT0 
                                            && pMemTrSetup->m_TrCmd!=DT_MEMTR_TRCMD_SECT1
                                            && pMemTrSetup->m_TrCmd!=DT_MEMTR_TRCMD_FRAME)
    {
        DtDbgOut(ERR, NONIP, "Unsupported transfer command (%d)", pMemTrSetup->m_TrCmd);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check: data format
    if (pMemTrSetup->m_DataFormat!=DT_MEMTR_DATAFMT_8B
                                       && pMemTrSetup->m_DataFormat!=DT_MEMTR_DATAFMT_10B 
                                       && pMemTrSetup->m_DataFormat!=DT_MEMTR_DATAFMT_16B)
    {
        DtDbgOut(ERR, NONIP, "Unsupported data format (%d)", pMemTrSetup->m_DataFormat);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check: RGB-mode
    if (pMemTrSetup->m_RgbMode!=DT_MEMTR_RGBMODE_OFF
                                           && pMemTrSetup->m_RgbMode!=DT_MEMTR_RGBMODE_ON)
    {
        DtDbgOut(ERR, NONIP, "Unsupported data format (%d)", pMemTrSetup->m_DataFormat);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check: symbol filter mode
    if (pMemTrSetup->m_SymFlt!=DT_MEMTR_SYMFLTMODE_ALL
                                       && pMemTrSetup->m_SymFlt!=DT_MEMTR_SYMFLTMODE_CHROM
                                       && pMemTrSetup->m_SymFlt!=DT_MEMTR_SYMFLTMODE_LUM
                                       && pMemTrSetup->m_SymFlt!=DT_MEMTR_SYMFLTMODE_SWAP)
    {
        DtDbgOut(ERR, NONIP, "Unsupported symbol filter mode (%d)", 
                                                                   pMemTrSetup->m_SymFlt);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check: scaling mode
    if (pMemTrSetup->m_Scaling!=DT_MEMTR_SCMODE_OFF
                                          && pMemTrSetup->m_Scaling!=DT_MEMTR_SCMODE_1_4
                                          && pMemTrSetup->m_Scaling!=DT_MEMTR_SCMODE_1_16)
    {
        DtDbgOut(ERR, NONIP, "Unsupported scaling mode (%d)", pMemTrSetup->m_Scaling);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check: ancillary filter mode
    if (pMemTrSetup->m_AncFlt!=DT_MEMTR_ANCFLTMODE_OFF
                                    && pMemTrSetup->m_AncFlt!=DT_MEMTR_ANCFLTMODE_HANCALL
                                    && pMemTrSetup->m_AncFlt!=DT_MEMTR_ANCFLTMODE_VANCALL)
    {
        DtDbgOut(ERR, NONIP, "Unsupported ancillary filter mode (%d)", 
                                                                   pMemTrSetup->m_AncFlt);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check: stride mode
    if (pMemTrSetup->m_Stride > 0)
    {
        DtDbgOut(ERR, NONIP, "Unsupported stride (%d)", pMemTrSetup->m_Stride);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Check: line numbers
    if (!DtaNonIpMatrixValidateStartLineAndNumLines(pFrameProps, pMemTrSetup->m_StartLine, 
                                                                 pMemTrSetup->m_NumLines))
    {
        DtDbgOut(ERR, NONIP, "Invalid start line (%d) and/or number of lines (%d)",
                                       pMemTrSetup->m_StartLine, pMemTrSetup->m_NumLines);
        return DT_STATUS_INVALID_PARAMETER;
    }
    
    // Get number of symbols per line
    if (pMemTrSetup->m_TrCmd == DT_MEMTR_TRCMD_SECT0)
        LineNumS = pBufConf->m_Sections[0].m_LineNumSymbols;
    else if (pMemTrSetup->m_TrCmd == DT_MEMTR_TRCMD_SECT1)
        LineNumS = pBufConf->m_Sections[1].m_LineNumSymbols;
    else if (pMemTrSetup->m_TrCmd == DT_MEMTR_TRCMD_FRAME)
    {
        LineNumS = pBufConf->m_Sections[0].m_LineNumSymbols;
        LineNumS += pBufConf->m_Sections[1].m_LineNumSymbols;
    }
    // Compute the expected DMA size
    *pReqDmaSize = DtaNonIpMatrixGetDmaSize(LineNumS, pMemTrSetup->m_NumLines, 
                                          pMemTrSetup->m_DataFormat, 
                                          pMemTrSetup->m_Scaling, pMemTrSetup->m_RgbMode);

    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixLastFrameIntDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaNonIpMatrixLastFrameIntDpc(DtDpcArgs* pArgs)
{
    DtaNonIpPort*  pNonIpPort = (DtaNonIpPort*)pArgs->m_pContext;
#ifdef _DEBUG
    Int64  LastFrame = (Int64)pArgs->m_Data1.m_UInt64;

    DtDbgOut(MAX, NONIP, "New last-frame=%lld", LastFrame);
#endif

    // Fire last frame event
    DtEventSet(&pNonIpPort->m_Matrix.m_LastFrameIntEvent);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixProcessRxFlags -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// PRE: pNonIpPort->m_FlagsSpinLock has been acquired
static void  DtaNonIpMatrixProcessRxFlags(DtaNonIpPort* pNonIpPort)
{
    Int  Status = 0;
    
    if (DtaRegHdStatGetRxSyncErrInt(pNonIpPort->m_pRxRegs) != 0)
    {
        Status |= DTA_RX_SYNC_ERR;
        DtaRegHdStatClrRxSyncErrInt(pNonIpPort->m_pRxRegs);
    }
    if (DtaRegHdStatGetRxOvfErrInt(pNonIpPort->m_pRxRegs) != 0)
    {
        // This interrupt works both as RX overflow and as TX underflow depending on
        // the current configuration.
        if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_INPUT)
            Status |= DTA_RX_FIFO_OVF;
        else {
            Status |= DTA_TX_FIFO_UFL;
            pNonIpPort->m_TxUfl = TRUE;
        }
        DtaRegHdStatClrRxOvfErrInt(pNonIpPort->m_pRxRegs);
    }

    pNonIpPort->m_FlagsLatched |= Status;
    pNonIpPort->m_Flags = Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixProcessRxFlagsFromUser -.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaNonIpMatrixProcessRxFlagsFromUser(DtaNonIpPort* pNonIpPort)
{
    DtSpinLockAcquire(&pNonIpPort->m_FlagsSpinLock);
    DtaNonIpMatrixProcessRxFlags(pNonIpPort);
    DtSpinLockRelease(&pNonIpPort->m_FlagsSpinLock);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixRxFlagsIntDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaNonIpMatrixRxFlagsIntDpc(DtDpcArgs* pArgs)
{
    DtaNonIpPort*  pNonIpPort = (DtaNonIpPort*)pArgs->m_pContext;
    DtSpinLockAcquire(&pNonIpPort->m_FlagsSpinLock);
    DtaNonIpMatrixProcessRxFlags(pNonIpPort);
    DtSpinLockRelease(&pNonIpPort->m_FlagsSpinLock);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixConfigureForAsi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNonIpMatrixConfigureForAsi(
    DtaNonIpPort* pNonIpPort, 
    Bool  ForceConfig)
{
    DtStatus  Status = DT_STATUS_OK;
    volatile UInt8*  pHdRegs;
    DtaFrameBufSectionConfig*  pSect0;
    Int  SizeOfChannelRam = 0;
    Int  NumMatrixPorts = DtaNonIpMatrixCountMatrixPorts(pNonIpPort);

    DtDbgOut(MIN, NONIP, "Configuring for ASI: DTA-%d, PortIdx %d", 
                                           pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber,
                                                                 pNonIpPort->m_PortIndex);

    // Must have ASI capability
    if (!pNonIpPort->m_CapAsi)
        return DT_STATUS_NOT_SUPPORTED;

    DT_ASSERT(pNonIpPort->m_pDvcData->m_Matrix.m_RamSize>1 && NumMatrixPorts>0);
    SizeOfChannelRam = (pNonIpPort->m_pDvcData->m_Matrix.m_RamSize * 1024 * 1024)  
                                                                         / NumMatrixPorts;

    pHdRegs = pNonIpPort->m_pTxRegs;
    pSect0 = &pNonIpPort->m_Matrix.m_BufConfig.m_Sections[0];


    //=+=+=+=+=+=+=+=+=+=+=+=+=+ TT #1637: BEGIN OF WORKAROUND +=+=+=+=+=+=+=+=+=+=+=+=+=+
    //
    // NOTE: the following is a workaround for TT defect #1637 (2154,FW V1: fractional 
    // SDI to ASI switch, results in an invalid ASI signal)
    //
    if (!DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
    {
        Int  VideoId=0, PictRate=0, Progressive=0;
        
        // First switch to a non-fractional HD-standard. 
        DtaRegHdCtrl1SetOpMode(pHdRegs, DT_HD_OPMODE_DISABLE);
        DtaRegHdCtrl1SetRxTxCtrl(pHdRegs, DT_HD_RXTXCTRL_IDLE);

        DtaNonIpMatrixVidStd2SdiFormat(DT_VIDSTD_1080I50, &VideoId, &PictRate,
                                                                            &Progressive);
        DtaRegHdSdiFormatSetVideoId(pHdRegs, VideoId);
        DtaRegHdSdiFormatSetPictureRate(pHdRegs, PictRate);
        DtaRegHdSdiFormatSetProgressive(pHdRegs, Progressive);

        DtaRegHdCtrl1SetOpMode(pHdRegs, DT_HD_OPMODE_HD);
    }
    //
    //+=+=+=+=+=+=+=+=+=+=+=+=+=+ TT #1637: END OF WORKAROUND +=+=+=+=+=+=+=+=+=+=+=+=+=+=

    // Disable channel before configuring 
    DtaRegHdCtrl1SetOpMode(pHdRegs, DT_HD_OPMODE_DISABLE);
    DtaRegHdCtrl1SetRxTxCtrl(pHdRegs, DT_HD_RXTXCTRL_IDLE);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Configure IO-direction -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_INPUT)
        DtaRegHdCtrl1SetIoDir(pHdRegs, DT_HD_IODIR_INPUT);
    else if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_OUTPUT)
        DtaRegHdCtrl1SetIoDir(pHdRegs, DT_HD_IODIR_OUTPUT);
    else
        DT_ASSERT(1 == 0);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Configure "FIFO" buffer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    pSect0->m_BeginAddr = SizeOfChannelRam * pNonIpPort->m_Matrix.m_RowIdx;
    DT_ASSERT((pSect0->m_BeginAddr % 32) == 0);  // Must be multiple of 32
    pSect0->m_BaseAddr = pSect0->m_BeginAddr;

    DT_ASSERT((pNonIpPort->m_Matrix.m_AsiFifoSize % 32) == 0);  // Must be mutiple of 32
    DT_ASSERT(pNonIpPort->m_Matrix.m_AsiFifoSize < SizeOfChannelRam);
    pSect0->m_EndAddr = pSect0->m_BeginAddr + pNonIpPort->m_Matrix.m_AsiFifoSize - 1;
    DT_ASSERT(((pSect0->m_EndAddr+1) % 32) == 0);  // Must be multiple of 32
    
    DtaRegHdS0BeginAddrSet(pHdRegs, pSect0->m_BeginAddr);
    DtaRegHdS0EndAddrSet(pHdRegs, pSect0->m_EndAddr);

    DtDbgOut(AVG, NONIP, "Frame Buffer Config  (PortIdx %d):", pNonIpPort->m_PortIndex);
    DtDbgOut(AVG, NONIP, "- S0: sz=%d, begin,end[0x%08X:0x%08X]", 
              pNonIpPort->m_Matrix.m_AsiFifoSize, pSect0->m_BeginAddr, pSect0->m_EndAddr);   
        
    //.-.-.-.-.-.-.-.-.-.-.-.-.-.- Enable relevant interrupts -.-.-.-.-.-.-.-.-.-.-.-.-.-.

    Status = DtaNonIpMatrixInterruptEnable(pNonIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, NONIP, "Failed to enable relevant interrupts");
        return Status;
    }

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Set operation mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    DtaRegHdCtrl1SetOpMode(pHdRegs, DT_HD_OPMODE_ASI);
        
    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Finally: init IO-interface  -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    DtaRegHdCtrl1SetIoEnable(pHdRegs, 1);

    // Issue a IO reset
    DtaRegHdCtrl1SetIoReset(pHdRegs, 1);
    DtSleep(5);
    DtaRegHdCtrl1SetIoReset(pHdRegs, 0);

    pNonIpPort->m_Matrix.m_AsiDmaNumBytes = 0;
    pNonIpPort->m_Matrix.m_AsiDmaOffset = 0;
    pNonIpPort->m_Matrix.m_AsiCtrl = DT_TXCTRL_IDLE; // Is the same as DT_RXCTRL_IDLE
    DtaRegHdAsiByteCountSet(pHdRegs, 0);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixConfigureForSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// PRE: pNonIpPort->m_Matrix.m_StateLock has been acquired
//
static DtStatus  DtaNonIpMatrixConfigureForSdi(
    DtaNonIpPort* pNonIpPort, 
    Int64  StartFrame,
    Bool  ForceConfig)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  n, NewVidStd = DT_VIDSTD_UNKNOWN;
    Bool  ConfigRequired=FALSE, IsInput=FALSE, IsOutput=FALSE, IsLegacy=FALSE;
    volatile UInt8* pHdRegs =  pNonIpPort->m_pRxRegs;
    DtaFrameBufConfig*  pBufConfig = &pNonIpPort->m_Matrix.m_BufConfig;
    DtaFrameBufSectionConfig*  pSections = pBufConfig->m_Sections;
    
    DtDbgOut(MIN, NONIP, "Configuring for (XX-)SDI: DTA-%d, PortIdx %d", 
                                           pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber,
                                                                 pNonIpPort->m_PortIndex);

    // Must have (XX-)SDI capabilty
    if (!pNonIpPort->m_CapSdi && !pNonIpPort->m_CapHdSdi && !pNonIpPort->m_Cap3GSdi)
        return DT_STATUS_NOT_SUPPORTED;

    // Check if we have a legacy interface
    IsLegacy = DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort);

    // Check if reconfiguration is required
    NewVidStd = DtaIoStd2VidStd(pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value,
                                       pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue);
    if (pNonIpPort->m_Matrix.m_State==MATRIX_PORT_UNINIT || 
                                    pNonIpPort->m_Matrix.m_State==MATRIX_PORT_CONFIGURING)
        ConfigRequired = TRUE;  // Port not yet intialised => configure required
    else if (NewVidStd != pNonIpPort->m_Matrix.m_FrameProps.m_VidStd)
        ConfigRequired = TRUE;  // New video standard => configure required
    else
    {
        // Check if io-direction has changed
        Int  CurrentIoDir = DtaRegHdCtrl1GetIoDir(pHdRegs);
        if (CurrentIoDir==DT_HD_IODIR_INPUT && 
                        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value!=DT_IOCONFIG_INPUT)
            ConfigRequired = TRUE; // Change direction to output => configure required
        else if (CurrentIoDir==DT_HD_IODIR_OUTPUT && 
                       pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value!=DT_IOCONFIG_OUTPUT)
            ConfigRequired = TRUE; // Change direction to input => configure required
    }

    if (!ConfigRequired && !ForceConfig)
    {
        DtDbgOut(MIN, NONIP, "No re-configuration of channel required");

        // Set to HOLD state
        DtaNonIpMatrixSetState(pNonIpPort, MATRIX_PORT_HOLD);
        return DT_STATUS_OK;
    }
    
    DtaNonIpMatrixSetState(pNonIpPort, MATRIX_PORT_CONFIGURING);
    
    // Disable channel before configuring 
    DtaRegHdCtrl1SetOpMode(pHdRegs, DT_HD_OPMODE_DISABLE);
    DtaRegHdCtrl1SetRxTxCtrl(pHdRegs, DT_HD_RXTXCTRL_IDLE);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Configure IO-direction -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    IsInput = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_INPUT);
    IsOutput = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_OUTPUT);
    if (IsInput)
        DtaRegHdCtrl1SetIoDir(pHdRegs, DT_HD_IODIR_INPUT);
    else if (IsOutput)
        DtaRegHdCtrl1SetIoDir(pHdRegs, DT_HD_IODIR_OUTPUT);
    else
        DT_ASSERT(1 == 0);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.- Apply frame configuration -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    Status = DtaNonIpMatrixApplyConfig(pNonIpPort);
    if (!DT_SUCCESS(Status))
        return Status;

    // Apply buffer configuration
    Status = DtaNonIpMatrixApplyBufferConfig(pNonIpPort);
    if (!DT_SUCCESS(Status))
        return Status;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.- Enable relevant interrupts -.-.-.-.-.-.-.-.-.-.-.-.-.-.

    Status = DtaNonIpMatrixInterruptEnable(pNonIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, NONIP, "Failed to enable relevant interrupts");
        return Status;
    }
    
    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Set operation mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    
    if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_SDI)
        DtaRegHdCtrl1SetOpMode(pHdRegs, DT_HD_OPMODE_SD);
    else if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_HDSDI)
        DtaRegHdCtrl1SetOpMode(pHdRegs, DT_HD_OPMODE_HD);
    else if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_3GSDI)
        DtaRegHdCtrl1SetOpMode(pHdRegs, DT_HD_OPMODE_3G);
    else
        DT_ASSERT(1 == 0);
        
    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Finally: init IO-interface  -.-.-.-.-.-.-.-.-.-.-.-.-.-.

    DtaRegHdCtrl1SetIoEnable(pHdRegs, 1);

    if (IsInput)
    {
        if (IsLegacy)
        {
            // TODO: Disable auto EAV & SAV repair
        }

        // Check for GS2961 deserialiser
        if (pNonIpPort->m_AsiSdiDeserItfType == ASI_SDI_DESER_ITF_GS2961)
            Status = DtaGs2961Enable(pNonIpPort);
        else
        {
            // Issue a IO reset
            DtaRegHdCtrl1SetIoReset(pHdRegs, 1);
            DtSleep(5);
            DtaRegHdCtrl1SetIoReset(pHdRegs, 0);
        }
    }
    else if (IsOutput)
    {
        if (IsLegacy)
        {
            // TODO: Enable auto EAV & SAV repair
        }

        // Check for GS2962 serialiser
        if (pNonIpPort->m_AsiSdiSerItfType == ASI_SDI_SER_ITF_GS2962)
            Status = DtaGs2962Enable(pNonIpPort);
        else
        {
            // Issue a IO reset
            DtaRegHdCtrl1SetIoReset(pHdRegs, 1);
            DtSleep(5);
            DtaRegHdCtrl1SetIoReset(pHdRegs, 0);
        }
    }

    // For outputs: write black-frame
    // NOTE: we need the interrupts (i.e. DMA done) to be enabled
    if (pNonIpPort->m_pDvcData->m_IntEnableState==INT_ENABLED && IsOutput)
    {
        // NOTE: for legacy HD-channels only write black frame if the desired start-frame
        // is the black frame
        if (!IsLegacy || (IsLegacy && StartFrame==DTA_FRMBUF_HOLD_FRAME))
        {
            Status = DtaNonIpMatrixWriteBlackFrame(pNonIpPort, DTA_FRMBUF_HOLD_FRAME);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, NONIP, "Failed to init HOLD buffer with black-frame");

                return Status;
            }
        }
    }
    
    // Start transmission/reception @ specified frame. NOTE: for non-legacy channels, 
    // if the black-frame is specified start at frame 0
    if (!IsLegacy && StartFrame==DTA_FRMBUF_HOLD_FRAME)
        StartFrame = 0;

    pNonIpPort->m_Matrix.m_CurFrame = StartFrame;
    n = DtaNonIpMatrixFrame2Index(pNonIpPort, StartFrame);
    
    // In the legacy HD-channel register interface, the starting frame must be writen to 
    // Start-Address register as opposed to the Next-Frame register
    if (IsLegacy)
    {
        DT_ASSERT(pSections[0].m_FrameStartAddr[n] != -1);
        DtaRegHdS0StartAddrSet(pNonIpPort->m_pRxRegs, pSections[0].m_FrameStartAddr[n]);
        DT_ASSERT(pSections[1].m_FrameStartAddr[n] != -1);
        DtaRegHdS1StartAddrSet(pNonIpPort->m_pRxRegs, pSections[1].m_FrameStartAddr[n]);

        // Also init the current frame counter in the hardware
        if (StartFrame == DTA_FRMBUF_HOLD_FRAME)
            DtaRegHdCurrentFrameSet(pNonIpPort->m_pRxRegs, (pBufConfig->m_NumFrames-1));
        else
            DtaRegHdCurrentFrameSet(pNonIpPort->m_pRxRegs, StartFrame);
    }
    else
    {
        DT_ASSERT(pSections[0].m_FrameStartAddr[n] != -1);
        DtaRegHdS0NextFrmAddrSet(pNonIpPort->m_pRxRegs, pSections[0].m_FrameStartAddr[n]);
        DT_ASSERT(pSections[1].m_FrameStartAddr[n] != -1);
        DtaRegHdS1NextFrmAddrSet(pNonIpPort->m_pRxRegs, pSections[1].m_FrameStartAddr[n]);
    }

    // Start transmission/reception. 
    // NOTE: a legacy HD channel is started in froozen mode (if the start frame is the 
    // black frame), so that it continuously plays our black frame
    if (IsLegacy && StartFrame==DTA_FRMBUF_HOLD_FRAME)
    {
        // The last frame is not usable anymore (reserved for the black frame)
        pBufConfig->m_NumUsableFrames = pBufConfig->m_NumFrames-1;
        DtaRegHdCtrl1SetRxTxCtrl(pHdRegs, DT_HD_RXTXCTRL_FREEZE);
    }
    else
    {
        // For legacy HD-channels, mark all frames as usable again
        if (IsLegacy)
            pBufConfig->m_NumUsableFrames = pBufConfig->m_NumFrames;

        DtaRegHdCtrl1SetRxTxCtrl(pHdRegs, DT_HD_RXTXCTRL_RUN);
    }

    // Set frame for after the first one, but NOT for the legacy interface
    if (!IsLegacy)
    {
        n = DtaNonIpMatrixFrame2Index(pNonIpPort, StartFrame+1);
        DT_ASSERT(pSections[0].m_FrameStartAddr[n] != -1);
        DtaRegHdS0NextFrmAddrSet(pNonIpPort->m_pRxRegs, pSections[0].m_FrameStartAddr[n]);
        DT_ASSERT(pSections[1].m_FrameStartAddr[n] != -1);
        DtaRegHdS1NextFrmAddrSet(pNonIpPort->m_pRxRegs, pSections[1].m_FrameStartAddr[n]);
    }

    DtaNonIpMatrixSetState(pNonIpPort, MATRIX_PORT_HOLD);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixPowerUpPost -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpMatrixPowerUpPost(DtaNonIpPort* pNonIpPort)
{

    DtStatus  Status = DT_STATUS_OK;
    Bool  IsOutput = FALSE, IsSdi=FALSE;

    DT_ASSERT(pNonIpPort != NULL);
    DT_ASSERT(pNonIpPort->m_CapMatrix);
    
    IsOutput = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_OUTPUT);
    IsSdi = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_SDI)
                 || (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_HDSDI)
                 || (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_3GSDI);

    // Interrupts are enabled now, so reconfigure (SDI) output ports one more time to init
    // the black frame buffer (can DMA now that we have interrupts)
    if (!IsOutput || !IsSdi)
        return DT_STATUS_OK;    // not an output or not configured for SDI
    
    DT_ASSERT(pNonIpPort->m_pDvcData->m_IntEnableState == INT_ENABLED);
    DtMutexAcquire(&pNonIpPort->m_Matrix.m_StateLock, -1);
    Status = DtaNonIpMatrixConfigureForSdi(pNonIpPort, DTA_FRMBUF_HOLD_FRAME, TRUE);
    DtMutexRelease(&pNonIpPort->m_Matrix.m_StateLock);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixApplyConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpMatrixApplyConfig(DtaNonIpPort* pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaFrameProps*  pFrameProps = NULL;
    volatile UInt8* pHdRegs =  pNonIpPort->m_pRxRegs;
    Int  VideoId, PictRate, Progressive;
    
    // First init frame porperties
    Status = DtaNonIpMatrixInitFrameProps(pNonIpPort);
    if (!DT_SUCCESS(Status))
        return Status;
    pFrameProps = &pNonIpPort->m_Matrix.m_FrameProps;

    DtDbgOut(AVG, NONIP, "Frame Properties (PortIdx %d):", pNonIpPort->m_PortIndex);
    DtDbgOut(AVG, NONIP, "- # lines: %d", pFrameProps->m_NumLines);
    DtDbgOut(AVG, NONIP, "- Field1: f[%d:%d], v[%d:%d]", 
                        pFrameProps->m_Field1Start, pFrameProps->m_Field1End,
                        pFrameProps->m_Field1ActVidStart, pFrameProps->m_Field1ActVidEnd);
    DtDbgOut(AVG, NONIP, "- Field2: f[%d:%d], v[%d:%d]", 
                        pFrameProps->m_Field2Start, pFrameProps->m_Field2End,
                        pFrameProps->m_Field2ActVidStart, pFrameProps->m_Field2ActVidEnd);

    // 1. Set num lines (and for legacy hardware set interlaced)
    DtaRegHdFrameConfig1SetNumLines(pHdRegs, pFrameProps->m_NumLines);
    if (DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
        DtaRegHdFrameConfig1SetInterlaced(pHdRegs, pFrameProps->m_IsInterlaced ? 1 : 0);
    
    // 2. Set field start lines
    DtaRegHdFrameConfig2SetFld1Start(pHdRegs, pFrameProps->m_Field1Start);
    DtaRegHdFrameConfig2SetFld2Start(pHdRegs, pFrameProps->m_Field2Start);
    // 3. Set field  end lines
    DtaRegHdFrameConfig3SetFld1End(pHdRegs, pFrameProps->m_Field1End);
    DtaRegHdFrameConfig3SetFld2End(pHdRegs, pFrameProps->m_Field2End);
    
    // 4. Set video start lines
    DtaRegHdFrameConfig4SetVid1Start(pHdRegs, pFrameProps->m_Field1ActVidStart);
    DtaRegHdFrameConfig4SetVid2Start(pHdRegs, pFrameProps->m_Field2ActVidStart);
    // 5. Set video end lines
    DtaRegHdFrameConfig5SetVid1End(pHdRegs, pFrameProps->m_Field1ActVidEnd);
    DtaRegHdFrameConfig5SetVid2End(pHdRegs, pFrameProps->m_Field2ActVidEnd);

    // 6. Set video format; except for legacy interface
    if (!DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
    {
        DtaNonIpMatrixVidStd2SdiFormat(pFrameProps->m_VidStd, 
                                                       &VideoId, &PictRate, &Progressive);
        DtaRegHdSdiFormatSetVideoId(pHdRegs, VideoId);
        DtaRegHdSdiFormatSetPictureRate(pHdRegs, PictRate);
        DtaRegHdSdiFormatSetProgressive(pHdRegs, Progressive);
    }

    // (re-)compute buffer configuration
    return DtaNonIpMatrixComputeBufferConfig(pNonIpPort);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixApplyBufferConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpMatrixApplyBufferConfig(DtaNonIpPort* pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    volatile UInt8* pHdRegs =  pNonIpPort->m_pRxRegs;
    DtaFrameBufSectionConfig*  pSect = pNonIpPort->m_Matrix.m_BufConfig.m_Sections;

    // Write configuration to card

    // Section 0
    DtaRegHdS0BeginAddrSet(pHdRegs, pSect[0].m_BeginAddr);
    DtaRegHdS0EndAddrSet(pHdRegs, pSect[0].m_EndAddr);
    // For legacy channels; set start address to section begin
    if (DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
        DtaRegHdS0StartAddrSet(pHdRegs, pSect[0].m_BeginAddr);
    else
        DtaRegHdS0NextFrmAddrSet(pHdRegs, pSect[0].m_BeginAddr);
    // Set row configuration
    DtaRegHdS0RowConfigSetNumSymbols(pHdRegs, pSect[0].m_LineNumSymbols);
    DtaRegHdS0RowConfigSetNumBytes(pHdRegs, pSect[0].m_LineNumBytesInMem);

    // Section 1
    DtaRegHdS1BeginAddrSet(pHdRegs, pSect[1].m_BeginAddr);
    DtaRegHdS1EndAddrSet(pHdRegs, pSect[1].m_EndAddr);
    // For legacy channels; set start address to section begin
    if (DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
        DtaRegHdS1StartAddrSet(pHdRegs, pSect[1].m_BeginAddr);
    else
        DtaRegHdS1NextFrmAddrSet(pHdRegs, pSect[1].m_BeginAddr);
    // Set row configuration
    DtaRegHdS1RowConfigSetNumSymbols(pHdRegs, pSect[1].m_LineNumSymbols);
    DtaRegHdS1RowConfigSetNumBytes(pHdRegs, pSect[1].m_LineNumBytesInMem);

    DtDbgOut(AVG, NONIP, "Frame Buffer Config  (PortIdx %d):", pNonIpPort->m_PortIndex);
    DtDbgOut(AVG, NONIP, "- S0: #f,fsz=[%d:%d], begin,end[0x%08X:0x%08X], #s,#b[%d:%d]", 
                                 pSect[0].m_NumFrames, pSect[0].m_FrameSizeInMem, 
                                 pSect[0].m_BeginAddr,  pSect[0].m_EndAddr, 
                                 pSect[0].m_LineNumSymbols, pSect[0].m_LineNumBytesInMem);
    DtDbgOut(AVG, NONIP, "- S1: #f,fsz=[%d:%d], begin,end[0x%08X:0x%08X], #s,#b[%d:%d]", 
                                 pSect[1].m_NumFrames, pSect[1].m_FrameSizeInMem, 
                                 pSect[1].m_BeginAddr, pSect[1].m_EndAddr,
                                 pSect[1].m_LineNumSymbols, pSect[1].m_LineNumBytesInMem);   
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixInitFrameProps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Init frame properties based on io-standard
//
DtStatus  DtaNonIpMatrixInitFrameProps(DtaNonIpPort* pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaFrameProps*  pProps = &pNonIpPort->m_Matrix.m_FrameProps;
    Int  VidStd = DtaIoStd2VidStd(pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value, 
                                       pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue);

    switch (VidStd)
    {
    case DT_VIDSTD_525I59_94:
        pProps->m_NumLines = 525;
        pProps->m_IsFractional = TRUE;

        pProps->m_Field1Start = 1;
        pProps->m_Field1End = 262;
        pProps->m_Field1ActVidStart = 17;
        pProps->m_Field1ActVidEnd = 260;
        //pProps->m_SwitchingLines[0] = 10;

        pProps->m_Field2Start = 263; 
        pProps->m_Field2End = 525;
        pProps->m_Field2ActVidStart = 280;  
        pProps->m_Field2ActVidEnd = 522;
        //pProps->m_SwitchingLines[1] = 273;

        pProps->m_VancNumS = pProps->m_ActVidNumS = 720*2;
        pProps->m_HancNumS = 268;
        pProps->m_SavNumS = 4;
        pProps->m_EavNumS = 4;
        pProps->m_IsInterlaced = TRUE;
        break;

    case DT_VIDSTD_625I50:
        pProps->m_NumLines = 625;
        pProps->m_IsFractional = FALSE;

        pProps->m_Field1Start = 1;    
        pProps->m_Field1End = 312;
        pProps->m_Field1ActVidStart = 23;   
        pProps->m_Field1ActVidEnd = 310;
        //pProps->m_SwitchingLines[0] = 6;

        pProps->m_Field2Start = 313;  
        pProps->m_Field2End = 625;
        pProps->m_Field2ActVidStart = 336;  
        pProps->m_Field2ActVidEnd  = 623;
        //pProps->m_SwitchingLines[1] = 319;

        pProps->m_VancNumS = pProps->m_ActVidNumS = 720*2;
        pProps->m_HancNumS = 280;
        pProps->m_SavNumS = 4;
        pProps->m_EavNumS = 4;
        pProps->m_IsInterlaced = TRUE;
        break;

    case DT_VIDSTD_1080P60:
    case DT_VIDSTD_1080P59_94:
    case DT_VIDSTD_1080P50:
        pProps->m_NumLines = 1125;
        pProps->m_IsFractional = (VidStd == DT_VIDSTD_1080P59_94);

        pProps->m_Field1Start = 1;
        pProps->m_Field1End = 1125;

        pProps->m_Field1ActVidStart = 42;
        pProps->m_Field1ActVidEnd = 1121;

        //pProps->m_SwitchingLines[0] = 7;
        //pProps->m_SwitchingLines[1] = 569;

        pProps->m_Field2Start = 0;
        pProps->m_Field2End = 0;
        
        pProps->m_Field2ActVidStart = 0;
        pProps->m_Field2ActVidEnd  = 0;
        
        pProps->m_VancNumS = pProps->m_ActVidNumS = 1920*2;
        if (VidStd==DT_VIDSTD_1080P60 || VidStd==DT_VIDSTD_1080P59_94)
            pProps->m_HancNumS = 268*2;
        else if (VidStd == DT_VIDSTD_1080P50)
            pProps->m_HancNumS = 708*2;
        else
            DT_ASSERT(1==0);

        pProps->m_EavNumS = 8*2;
        pProps->m_SavNumS = 4*2;

        pProps->m_IsInterlaced = FALSE;
        break;

    case DT_VIDSTD_1080P30:
    case DT_VIDSTD_1080P29_97:
    case DT_VIDSTD_1080P25:
    case DT_VIDSTD_1080P24:
    case DT_VIDSTD_1080P23_98:
        pProps->m_NumLines = 1125;
        pProps->m_IsFractional = (VidStd==DT_VIDSTD_1080P29_97 
                                                         || VidStd==DT_VIDSTD_1080P23_98);

        pProps->m_Field1Start = 1;
        pProps->m_Field1End = 1125;

        pProps->m_Field1ActVidStart = 42;
        pProps->m_Field1ActVidEnd = 1121;
        //pProps->m_SwitchingLines[0] = 7;

        pProps->m_Field2Start = 0;
        pProps->m_Field2End = 0;
        pProps->m_Field2ActVidStart = 0;
        pProps->m_Field2ActVidEnd = 0;
        //pProps->m_SwitchingLines[1] = -1;

        pProps->m_VancNumS = pProps->m_ActVidNumS = 1920*2;
        if (VidStd==DT_VIDSTD_1080P30 || VidStd==DT_VIDSTD_1080P29_97)
            pProps->m_HancNumS = 268*2;
        else if (VidStd == DT_VIDSTD_1080P25)
            pProps->m_HancNumS = 708*2;
        else if (VidStd==DT_VIDSTD_1080P24 || VidStd==DT_VIDSTD_1080P23_98)
            pProps->m_HancNumS = 818*2;
        else
            DT_ASSERT(1==0);

        pProps->m_EavNumS = 8*2;
        pProps->m_SavNumS = 4*2;

        pProps->m_IsInterlaced = FALSE;
        break;

    case DT_VIDSTD_1080I60:
    case DT_VIDSTD_1080I59_94:
    case DT_VIDSTD_1080I50:
        pProps->m_NumLines = 1125;
        pProps->m_IsFractional = (VidStd == DT_VIDSTD_1080I59_94);

        pProps->m_Field1Start = 1; 
        pProps->m_Field1End = 563;
        pProps->m_Field1ActVidStart = 21; 
        pProps->m_Field1ActVidEnd = 560;
        //pProps->m_SwitchingLines[0] = 7;

        pProps->m_Field2Start = 564;
        pProps->m_Field2End = 1125;
        pProps->m_Field2ActVidStart = 584;
        pProps->m_Field2ActVidEnd = 1123;
        //pProps->m_SwitchingLines[1] = 569;

        pProps->m_VancNumS = pProps->m_ActVidNumS = 1920*2;
        if (VidStd==DT_VIDSTD_1080I60 || VidStd==DT_VIDSTD_1080I59_94)
            pProps->m_HancNumS = 268*2;
        else if (VidStd == DT_VIDSTD_1080I50)
            pProps->m_HancNumS = 708*2;
        else
            DT_ASSERT(1==0);
        
        pProps->m_EavNumS = 8*2;
        pProps->m_SavNumS = 4*2;

        pProps->m_IsInterlaced = TRUE;
        break;

    case DT_VIDSTD_720P60:
    case DT_VIDSTD_720P59_94:
    case DT_VIDSTD_720P50:
    case DT_VIDSTD_720P30:
    case DT_VIDSTD_720P29_97:
    case DT_VIDSTD_720P25:
    case DT_VIDSTD_720P24:
    case DT_VIDSTD_720P23_98:
        pProps->m_NumLines = 750;
        pProps->m_IsFractional = (VidStd==DT_VIDSTD_720P59_94 
                                                          || VidStd==DT_VIDSTD_720P29_97 
                                                          || VidStd==DT_VIDSTD_720P23_98);

        pProps->m_Field1Start = 1;
        pProps->m_Field1End = 750;
        pProps->m_Field1ActVidStart = 26; 
        pProps->m_Field1ActVidEnd = 745;
        //pProps->m_SwitchingLines[0] = 7;

        pProps->m_Field2Start = 0;
        pProps->m_Field2End = 0;
        pProps->m_Field2ActVidStart = 0; 
        pProps->m_Field2ActVidEnd = 0;
        //pProps->m_SwitchingLines[1] = -1;

        pProps->m_VancNumS = pProps->m_ActVidNumS = 1280*2;
        if (VidStd==DT_VIDSTD_720P60 || VidStd==DT_VIDSTD_720P59_94)
            pProps->m_HancNumS = 358*2;
        else if (VidStd==DT_VIDSTD_720P50)
            pProps->m_HancNumS = 688*2;
        else if (VidStd==DT_VIDSTD_720P30 || VidStd==DT_VIDSTD_720P29_97)
            pProps->m_HancNumS = 2008*2;
        else if (VidStd==DT_VIDSTD_720P25)
            pProps->m_HancNumS = 2668*2;
        else if (VidStd==DT_VIDSTD_720P24 || VidStd==DT_VIDSTD_720P23_98)
            pProps->m_HancNumS = 2833*2;
        else
            DT_ASSERT(1==0);

        pProps->m_EavNumS = 8*2;
        pProps->m_SavNumS = 4*2;

        pProps->m_IsInterlaced = FALSE;
        break;

    default:
        DtDbgOut(ERR, NONIP, "Unknown IO-standard");
        return DT_STATUS_CONFIG_ERROR;
    }
    pProps->m_VidStd = VidStd;

    // Set HD flags
    if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value==DT_IOCONFIG_HDSDI
                     || pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value==DT_IOCONFIG_3GSDI)
        pProps->m_IsHd = TRUE;
    else
        pProps->m_IsHd = FALSE;


    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixComputeBufferConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNonIpMatrixComputeBufferConfig(DtaNonIpPort* pNonIpPort)
{   
    DtStatus  Status = DT_STATUS_OK;
    DtaFrameProps*  pFrameProps = &pNonIpPort->m_Matrix.m_FrameProps;
    DtaFrameBufConfig*  pBufConf = &pNonIpPort->m_Matrix.m_BufConfig;
    UInt32  BaseAddr=0;
    Int  SizeOfChannelRam, NumFrames=0, NumLines=0, LineNumSymbols=0, LineSizeBytes=0; 
    Int  NumMatrixPorts = DtaNonIpMatrixCountMatrixPorts(pNonIpPort);
    
    DT_ASSERT(pNonIpPort->m_pDvcData->m_Matrix.m_RamSize>1 && NumMatrixPorts>0);
    SizeOfChannelRam = (pNonIpPort->m_pDvcData->m_Matrix.m_RamSize * 1024*1024) 
                                                                         / NumMatrixPorts;
                        
    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Compute number of frames -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    LineSizeBytes = DtaNonIpMatrixNumSymbols2LineSizeInMem(pNonIpPort, 
                                                               pFrameProps->m_ActVidNumS);
    LineSizeBytes += DtaNonIpMatrixNumSymbols2LineSizeInMem(pNonIpPort, 
                                          pFrameProps->m_HancNumS + pFrameProps->m_EavNumS 
                                                                + pFrameProps->m_SavNumS);

    NumFrames = SizeOfChannelRam / (LineSizeBytes*pFrameProps->m_NumLines);
    if (NumFrames>DTA_FRMBUF_MAX_FRAMES)
        NumFrames = DTA_FRMBUF_MAX_FRAMES;  // Limit to max # frames 
    
    // Store max number of frames in buffer
    pBufConf->m_NumFrames = NumFrames;
    
    // The legacy HD-channel can not skip a specific frame buffer, so all frames have to 
    // be usable
    if (DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
        pBufConf->m_NumUsableFrames = NumFrames;
    else
        pBufConf->m_NumUsableFrames = NumFrames-1; // last frame is reserved for HOLD mode

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.- Init section configuration -.-.-.-.-.-.-.-.-.-.-.-.-.-.
           
    // Init HANC section size configuration
    BaseAddr = SizeOfChannelRam * pNonIpPort->m_Matrix.m_RowIdx;
    NumLines = pFrameProps->m_NumLines;
    LineNumSymbols = pFrameProps->m_EavNumS + pFrameProps->m_HancNumS +  
                                                                 + pFrameProps->m_SavNumS;
    DtaNonIpMatrixInitFrameBufSectionConfig(pNonIpPort, 
                                           &pBufConf->m_Sections[DTA_FRMBUF_SECT_HANC],
                                           BaseAddr, NumFrames, NumLines, LineNumSymbols); 
    
    // Init VID/VANC section size configuration
    BaseAddr += pBufConf->m_Sections[DTA_FRMBUF_SECT_HANC].m_Size; // Starts after HANC
    NumLines = pFrameProps->m_NumLines;
    LineNumSymbols = pFrameProps->m_ActVidNumS;
    
    DtaNonIpMatrixInitFrameBufSectionConfig(pNonIpPort, 
                                           &pBufConf->m_Sections[DTA_FRMBUF_SECT_VID],
                                           BaseAddr, NumFrames, NumLines, LineNumSymbols); 
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixInitFrameBufSectionConfig -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpMatrixInitFrameBufSectionConfig(
    DtaNonIpPort* pNonIpPort,
    DtaFrameBufSectionConfig* pSection, 
    UInt32 BaseAddr, 
    Int NumFrames,
    Int  NumLines,
    Int  LineNumSymbols)
{
    Int  n;
    pSection->m_NumFrames = NumFrames;
    pSection->m_LineNumSymbols = LineNumSymbols;
    
    // Compute line size in memory
    pSection->m_LineNumBytesInMem = DtaNonIpMatrixNumSymbols2LineSizeInMem(pNonIpPort, 
                                                                          LineNumSymbols);
    
    // Compute size of section in memory
    pSection->m_FrameSizeInMem = pSection->m_LineNumBytesInMem * NumLines;
    pSection->m_Size = pSection->m_FrameSizeInMem * NumFrames;
    
    // Init addresses
    pSection->m_BaseAddr = BaseAddr;
    pSection->m_BeginAddr = BaseAddr;

    // In the legacy HD-channel register interface, the end address is specified as upto 
    // as opposed to upto and including
    if (DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
        pSection->m_EndAddr = BaseAddr + pSection->m_Size;
    else
        pSection->m_EndAddr = BaseAddr + pSection->m_Size - 1;

    // Compute start addresses of frames
    for (n=0; n<NumFrames; n++)
        pSection->m_FrameStartAddr[n] = BaseAddr + (pSection->m_FrameSizeInMem * n);
    for (; n<DTA_FRMBUF_MAX_FRAMES; n++)
        pSection->m_FrameStartAddr[n] = -1; // Not used => set to uninitialised value

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixNumSymbols2LineSizeInMem -.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtaNonIpMatrixNumSymbols2LineSizeInMem(
    DtaNonIpPort* pNonIpPort,
    Int  NumSymbols)
{
    Int  MemPckSize = pNonIpPort->m_pDvcData->m_Matrix.m_MemPckSize;
    Int  MemPckNumBitsUsed = pNonIpPort->m_pDvcData->m_Matrix.m_MemPckNumBitsUsed;
    Int  MemLineAlignment = pNonIpPort->m_pDvcData->m_Matrix.m_MemLineAlignment;
    Int  NumSymbolBits=0, NumBitsInMem=0, NumPackets=0;

    DT_ASSERT(pNonIpPort->m_pDvcData->m_Matrix.m_IsSupported);

    // Compute total # of bits per line
    NumSymbolBits = NumSymbols * 10;
    // Compute # of packets per line
    NumPackets = (NumSymbolBits + MemPckNumBitsUsed - 1) / MemPckNumBitsUsed;

    // Compute # of bits used per line (account for line alignment)
    NumBitsInMem = NumPackets * MemPckSize;
    NumBitsInMem = (NumBitsInMem + MemLineAlignment - 1) / MemLineAlignment;
    NumBitsInMem *= MemLineAlignment;

    return NumBitsInMem / 8; // convert to number of bytes
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixNumSymbols2DmaSize -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaNonIpMatrixNumSymbols2DmaSize(
    Int  NumSymbols,
    Int  DataFormat,
    Int  RgbMode)
{
    Int  NumSymbolBits=0, NumDmaBits=0;

    // All DMA transfers must be 64-bit aligned
    static const Int  DMA_TRANSFER_ALIGN_NUM_BITS = 64;
    
    if (DataFormat == DT_MEMTR_DATAFMT_16B)
        NumSymbolBits = NumSymbols * 16;
    else if (DataFormat == DT_MEMTR_DATAFMT_10B)
        NumSymbolBits = NumSymbols * 10;
    else if (DataFormat == DT_MEMTR_DATAFMT_8B)
        NumSymbolBits = NumSymbols * 8;

    // with RGB mode enabled we get 3 symbols (R+G+B) for each YUV-sample (Y+Cb or Y+Cr)
    if (RgbMode == DT_MEMTR_RGBMODE_ON)
        NumSymbolBits = (NumSymbolBits*3) / 2;
         

    // All DMA transfers must be aligned
    NumDmaBits = (NumSymbolBits + DMA_TRANSFER_ALIGN_NUM_BITS-1) 
                                                            / DMA_TRANSFER_ALIGN_NUM_BITS;
    NumDmaBits *= DMA_TRANSFER_ALIGN_NUM_BITS;

    // Return number of bytes
    return (NumDmaBits / 8);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixGetDmaSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaNonIpMatrixGetDmaSize(
    Int  LineNumS,
    Int  NumLines,
    Int DataFormat,
    Int  Scaling,
    Int  RgbMode)
{
    Int  ScalingFactor=1, NumSymbols=0;
    NumSymbols = LineNumS * NumLines;
    if (Scaling == DT_MEMTR_SCMODE_OFF)
        ScalingFactor = 1;
    else if (Scaling == DT_MEMTR_SCMODE_1_4)
        ScalingFactor = 2;
    else if (Scaling == DT_MEMTR_SCMODE_1_16)
        ScalingFactor = 4;
    else
        DT_ASSERT(1 == 0);

    NumSymbols /= ScalingFactor;
    
    return DtaNonIpMatrixNumSymbols2DmaSize(NumSymbols, DataFormat, RgbMode);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixVidStd2SdiFormat -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Converts a video standard to a SDI format
//
DtStatus  DtaNonIpMatrixVidStd2SdiFormat(
    Int  VidStd,
    Int* pVideoId,
    Int*  pPictRate,
    Int*  pProgressive)
{
    Int  i;
    Bool  Found = FALSE;
    DT_ASSERT(pVideoId!=NULL && pPictRate!=NULL && pProgressive!=NULL);

    // Find format for specified standard
    for (i=0; i<DTA_NONIP_MATRIX_VIDSTD_2_FORMAT_NUM_ENTRIES; i++)
    {
        if (DTA_NONIP_MATRIX_VIDSTD_2_FORMAT[i][0] != VidStd)
            continue;

        *pVideoId = DTA_NONIP_MATRIX_VIDSTD_2_FORMAT[i][1];
        *pPictRate = DTA_NONIP_MATRIX_VIDSTD_2_FORMAT[i][2];
        *pProgressive = DTA_NONIP_MATRIX_VIDSTD_2_FORMAT[i][3];

        Found = TRUE;
        break;            
    }

    if (!Found)
    {
        DtDbgOut(ERR, NONIP, "Unknown video standard: %d", VidStd);
    }
    return Found ? DT_STATUS_OK : DT_STATUS_FAIL;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixSdiFormat2VidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Converts a SDI format to a video standard
//
DtStatus  DtaNonIpMatrixSdiFormat2VidStd(
    Int VideoId,
    Int  PictRate,
    Int  Progressive, 
    Int*  pVidStd)
{
    Int  i;
    Bool  Found = FALSE;

    DT_ASSERT(pVidStd != NULL);

    // Set to 'safe' initial value
    *pVidStd = DT_VIDSTD_UNKNOWN;

    // Find a matching video standard for the specifiued format
    for (i=0; i<DTA_NONIP_MATRIX_VIDSTD_2_FORMAT_NUM_ENTRIES; i++)
    {
        Int  Id = DTA_NONIP_MATRIX_VIDSTD_2_FORMAT[i][1];
        Int  Rate = DTA_NONIP_MATRIX_VIDSTD_2_FORMAT[i][2];
        Int  Prog = DTA_NONIP_MATRIX_VIDSTD_2_FORMAT[i][3];

        if (VideoId!=Id || PictRate!=Rate || Progressive!=Prog)
            continue;

        *pVidStd = DTA_NONIP_MATRIX_VIDSTD_2_FORMAT[i][0];
        Found = TRUE;
        break;            
    }
    
    if (!Found)
    {
        DtDbgOut(ERR, NONIP, "Unknown SDI format: id=%d, r=%d, p=%d", VideoId, PictRate,
                                                                             Progressive);
    }
    return Found ? DT_STATUS_OK : DT_STATUS_FAIL;
}

//.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixValidateStartLineAndNumLines -.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtaNonIpMatrixValidateStartLineAndNumLines(
    DtaFrameProps* pFrameProps, 
    Int  StartLine, 
    Int  NumLines)
{
    DT_ASSERT(pFrameProps != NULL);
    return (StartLine>=1 && (StartLine+NumLines-1)<=pFrameProps->m_NumLines);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixCountMatrixPorts -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtaNonIpMatrixCountMatrixPorts(DtaNonIpPort* pNonIpPort)
{
    Int  i, NumMatrixPorts = 0;
    DtaDeviceData*  pDvcData = NULL;    
    DT_ASSERT(pNonIpPort != NULL);
    
    pDvcData = pNonIpPort->m_pDvcData;
       
    // Check ROW index is valid. First count # of matrix ports
    for (i=0; i<pDvcData->m_NumNonIpPorts; i++)
    {
        if (!pDvcData->m_pNonIpPorts[i].m_CapMatrix)
            continue;
        NumMatrixPorts++;
    }

    return NumMatrixPorts;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixWriteBlackFrame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Writes a black-frame to the specified frame buffer
//
// NOTE: this function generates and writes one line at a time, so that we do not need to 
// allocate a kernel buffer large enough for a full frame (may not succeed to allocate
// such a large buffer)
//
DtStatus  DtaNonIpMatrixWriteBlackFrame(DtaNonIpPort*  pNonIpPort, Int64  Frame)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool  VSync;
    DtaMatrixMemTrSetup  MemTrSetup;
    DtaFrameProps*  pProps = NULL;
    UInt8*  pLocalAddress = NULL;
    UInt16 *pLineBuf=NULL, *pEav=NULL, *pSav=NULL;
    DtPageList* pPageList = NULL;
    Int  s, f, l, LineNumS=0, LineDmaSize=0, DmaSize=0;
    
    // Status words (Field 1, Field 2)
    static const  UInt16  EAV_VSYNC[] = { 0x2D8, 0x3C4 };
    static const  UInt16  SAV_VSYNC[] = { 0x2AC, 0x3B0 };
    static const  UInt16  EAV_VIDEO[] = { 0x274, 0x368 };
    static const  UInt16  SAV_VIDEO[] = { 0x200, 0x31C };

    DtDbgOut(AVG, NONIP, "Writing black-frame to frame-buffer: %lld", Frame);

    DT_ASSERT(pNonIpPort != NULL);
    DT_ASSERT(pNonIpPort->m_CapMatrix);
        
    pProps = &pNonIpPort->m_Matrix.m_FrameProps;
    pLocalAddress = (UInt8*)(size_t)pNonIpPort->m_FifoOffset;

    //.-.-.-.-.-.-.-.-.- Step 1: Allocate buffer for black-frame lines -.-.-.-.-.-.-.-.-.-

    // Get size of a single line (in # symbols)
    LineNumS = pProps->m_EavNumS + pProps->m_HancNumS + pProps->m_SavNumS + 
                                                                       pProps->m_VancNumS;

    // Get DMA size for a single line
    MemTrSetup.m_IsWrite = TRUE;
    MemTrSetup.m_Frame = Frame;
    MemTrSetup.m_StartLine = 1;
    MemTrSetup.m_NumLines = 1;  // We will transfer one line at a time
    MemTrSetup.m_TrCmd = DT_MEMTR_TRCMD_FRAME;
    MemTrSetup.m_DataFormat = DT_MEMTR_DATAFMT_16B; // 16-bit format
    MemTrSetup.m_RgbMode = DT_MEMTR_RGBMODE_OFF;
    MemTrSetup.m_SymFlt = DT_MEMTR_SYMFLTMODE_ALL;
    MemTrSetup.m_Scaling = DT_MEMTR_SCMODE_OFF;
    MemTrSetup.m_AncFlt = DT_MEMTR_ANCFLTMODE_OFF;
    MemTrSetup.m_Stride = 0;

    Status = DtaNonIpMatrixGetReqDmaSize(pNonIpPort, &MemTrSetup, &LineDmaSize);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTA, "DtaNonIpMatrixGetReqDmaSize failed (ERROR=0x%08X)", Status);
        return Status;
    }

    // Allocate buffer for line
    Status = DtaDmaCreateKernelBuffer(&pNonIpPort->m_DmaChannel, 
                                            DT_DMA_DIRECTION_TO_DEVICE, (void**)&pLineBuf, 
                                                        &pPageList, LineDmaSize, DTA_TAG);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, DTA, "Cannot allocate DMA buffer for black-frame lines");
        return DT_STATUS_OUT_OF_MEMORY;
    }

    //-.-.-.-.-.-.-.-.-.- Step 2: Generate lines and write to hardware -.-.-.-.-.-.-.-.-.-

    // Init line buffer with blanking
    for (s=0; s<LineNumS; s++)
        pLineBuf[s] = (s%2)==0 ? 0x200 : 0x040;

    // Generate EAVs and SAVs
    for (l=1; l<=pProps->m_NumLines; l++)
    {
        pEav = pLineBuf;
        pSav = pLineBuf + (pProps->m_EavNumS + pProps->m_HancNumS);

        // Are we in field 1 or 2
        f = (l<=pProps->m_Field1End) ? 0 : 1;

        if (f == 0)
            VSync = l<pProps->m_Field1ActVidStart || l>pProps->m_Field1ActVidEnd;
        else
            VSync = l<pProps->m_Field2ActVidStart || l>pProps->m_Field2ActVidEnd;

        if (pProps->m_IsHd)
        {
            UInt16 LN0, LN1;
            
            // Generate EAV (both streams)
            *pEav++ = 0x3FF; *pEav++ = 0x3FF; 
            *pEav++ = 0x000; *pEav++ = 0x000;
            *pEav++ = 0x000; *pEav++ = 0x000;
            *pEav++ = VSync ? EAV_VSYNC[f] : EAV_VIDEO[f];
            *pEav++ = VSync ? EAV_VSYNC[f] : EAV_VIDEO[f];
            // Line number
            LN0 = ((l&0x80)==0 ? 0x200 : 0x000) | ((l&0x07F)<<2);
            LN1 = 0x200 | ((l>>5)&0x3C);
            *pEav++ = LN0; *pEav++ = LN1; 
            *pEav++ = LN0; *pEav++ = LN1; 
            // CRC (just a place holder, HW will compute real CRC)
            *pEav++ = 0x000; *pEav++ = 0x000; 
            *pEav++ = 0x000; *pEav++ = 0x000; 

            // Generate SAV (both steams)
            *pSav++ = 0x3FF; *pSav++ = 0x3FF; 
            *pSav++ = 0x000; *pSav++ = 0x000;
            *pSav++ = 0x000; *pSav++ = 0x000;
            *pSav++ = VSync ? SAV_VSYNC[f] : SAV_VIDEO[f];
            *pSav++ = VSync ? SAV_VSYNC[f] : SAV_VIDEO[f];
        }
        else
        {
            // Generate EAV
            *pEav++ = 0x3FF; *pEav++ = 0x000; *pEav++ = 0x000;
            *pEav++ = VSync ? EAV_VSYNC[f] : EAV_VIDEO[f];

            // Generate SAV
            *pSav++ = 0x3FF; *pSav++ = 0x000; *pSav++ = 0x000;
            *pSav++ = VSync ? SAV_VSYNC[f] : SAV_VIDEO[f];
        }

        // Prep for DMA
        MemTrSetup.m_StartLine = l; // set next line to write
        DmaSize = 0;
        Status = DtaNonIpMatrixPrepForDma(pNonIpPort, LineDmaSize, &MemTrSetup, &DmaSize);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTA, "Prep-DMA for black-frame (l=%d) failed (ERROR=0x%08X)",
                                                                               l, Status);
            DtaDmaCleanupKernelBuffer(&pNonIpPort->m_DmaChannel, pLineBuf, pPageList, 
                                                                                 DTA_TAG);
            return Status;
        }
        DT_ASSERT(LineDmaSize >= DmaSize);

#ifdef WINBUILD
        // We don't want a DMA completed callback function, we block in this functions
        // waiting for the DMA done event
        DtaDmaReInitCallback(&pNonIpPort->m_DmaChannel, NULL, NULL);
#endif

        // Write line to card
        Status = DtaDmaStartKernelBufTransfer(&pNonIpPort->m_DmaChannel, (UInt8*)pLineBuf,
                                                         DmaSize, 0, pLocalAddress, 0, 0);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTA, "DMA-transfer for black-frame (l=%d) failed (ERROR=0x%08X)",
                                                                               l, Status);
            DtaDmaCleanupKernelBuffer(&pNonIpPort->m_DmaChannel, pLineBuf, pPageList, 
                                                                                 DTA_TAG);
            return Status;
        }
#ifdef WINBUILD
        else {
            // Wait for DMA done event. The DMA channel is initialised for non-blocking
            // for Windows
            DtEvent*  pEvent = DtaDmaGetDoneEvent(&pNonIpPort->m_DmaChannel);
            DT_ASSERT((pNonIpPort->m_DmaChannel.m_DmaFlags&DTA_DMA_FLAGS_BLOCKING) == 0);
            Status = DtEventWait(pEvent, -1);
        }
#endif
    }
    
    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 3: cleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    
    // Free line buffer
    DtaDmaCleanupKernelBuffer(&pNonIpPort->m_DmaChannel, pLineBuf, pPageList, DTA_TAG);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixUsesLegacyHdChannelInterface -.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtaNonIpMatrixUsesLegacyHdChannelInterface(DtaNonIpPort* pNonIpPort)
{
    DT_ASSERT(pNonIpPort != NULL);
    // DTA-2152;FW0: uses the legacy HD-channel register interface
    return (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber==2152 && 
                                  pNonIpPort->m_pDvcData->m_DevInfo.m_FirmwareVersion==0);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixDmaReadFinished -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
UInt  DtaNonIpMatrixDmaReadFinished(DtaNonIpPort* pNonIpPort, Int TrCmd)
{
    // Read-back actual number of bytes DMA-ed
    DmaChannel*  pDmaCh = &pNonIpPort->m_DmaChannel;
    UInt  DmaNumBytes = READ_UINT32(pDmaCh->m_pRegBase, DT_DMA_REG_TOTTR_CNT);

    if (TrCmd != DT_MEMTR_TRCMD_ASIRD)
        return DmaNumBytes; // Nothing else to do for non-ASI transfers

    // For ASI transfers update state counters
    DT_ASSERT(pNonIpPort->m_Matrix.m_AsiFifoSize > 0);
    pNonIpPort->m_Matrix.m_AsiDmaOffset += DmaNumBytes;
    pNonIpPort->m_Matrix.m_AsiDmaOffset %= pNonIpPort->m_Matrix.m_AsiFifoSize;
    pNonIpPort->m_Matrix.m_AsiDmaNumBytes += DmaNumBytes;
    return DmaNumBytes;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixDmaWriteFinished -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt  DtaNonIpMatrixDmaWriteFinished(DtaNonIpPort* pNonIpPort, Int TrCmd)
{
    // Read-back actual number of bytes DMA-ed
    DmaChannel*  pDmaCh = &pNonIpPort->m_DmaChannel;
    UInt  DmaNumBytes = READ_UINT32(pDmaCh->m_pRegBase, DT_DMA_REG_TOTTR_CNT);

    if (TrCmd != DT_MEMTR_TRCMD_ASIWR)
        return DmaNumBytes; // Nothing else to do for non-ASI transfers
                
    // Write number of bytes transfered to AsiNumBytes register
    DtaRegHdSetAsiControl1(pNonIpPort->m_pTxRegs, DmaNumBytes);
    // Issue ASIRD command to card. This signals that the data has been DMA-ed and can
    // be transmitted.
    DtaRegHdMemTrControlSetTrCmd(pNonIpPort->m_pTxRegs, DT_MEMTR_TRCMD_ASIRD);
    
    // For ASI transfers update state counters
    DT_ASSERT(pNonIpPort->m_Matrix.m_AsiFifoSize > 0);
    pNonIpPort->m_Matrix.m_AsiDmaOffset += DmaNumBytes;
    pNonIpPort->m_Matrix.m_AsiDmaOffset %= pNonIpPort->m_Matrix.m_AsiFifoSize;
    pNonIpPort->m_Matrix.m_AsiDmaNumBytes += DmaNumBytes;
    return DmaNumBytes;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixPeriodicInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaNonIpMatrixPeriodicInt(DtaNonIpPort* pNonIpPort)
{
    Int  FifoLoad;
    // The GetFifoLoad function will also set the overflow flag if an overflow occurred.
    DtaMatrixAsiRxGetFifoLoad(pNonIpPort, &FifoLoad);
}

#ifdef _DEBUG
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixState2Str -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
const char*  DtaNonIpMatrixState2Str(DtaMatrixPortState State)
{
    switch (State)
    {
    case MATRIX_PORT_UNINIT:        return "UNINIT";
    case MATRIX_PORT_CONFIGURING:   return "CONFIGURING";
    case MATRIX_PORT_IDLE:          return "IDLE";
    case MATRIX_PORT_HOLD:          return "HOLD";
    case MATRIX_PORT_RUN:           return "RUN";
    default:                        return "UNKNOWN";
    }
}
#endif
