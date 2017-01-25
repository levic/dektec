//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NonIpMatrix.c *#*#*#*#*#*#*#*# (C) 2013-2016 DekTec
//
// Dta driver - Non IP Matrix channel functionality - Implements matrix-API specific code
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2013-2016 DekTec Digital Video B.V.
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Internal functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
static void  DtaNonIpMatrixLastFrameIntDpc(DtDpcArgs* pArgs);
static DtStatus  DtaNonIpMatrixLastFrameIntHandler(DtaNonIpPort*  pNonIpPort);
static DtStatus  DtaNonIpMatrixConfigureForAsi(DtaNonIpPort* pNonIpPort, Int ConfigMode);
static DtStatus  DtaNonIpMatrixConfigureForSdi(DtaNonIpPort* pNonIpPort, 
                                                       Int64  StartFrame, Int ConfigMode);
static DtStatus  DtaNonIpMatrixApplyConfig(DtaNonIpPort* pNonIpPort);
static DtStatus  DtaNonIpMatrixSetVpidRegs(DtaNonIpPort* pNonIpPort);
static DtStatus  DtaNonIpMatrixApplyBufferConfig(DtaNonIpPort* pNonIpPort);
static DtStatus  DtaNonIpMatrixInitFrameProps(DtaNonIpPort* pNonIpPort);
static DtStatus  DtaNonIpMatrixComputeBufferConfig(DtaNonIpPort* pNonIpPort);
static DtStatus  DtaNonIpMatrixInitFrameBufSectionConfig(DtaNonIpPort* pNonIpPort, 
                                      DtaFrameBufSectionConfig* pSection, UInt32 BaseAddr,  
                                      Int NumFrames, Int  NumLines,  Int  LineNumSymbols);
static Int  DtaNonIpMatrixNumSymbols2LineSizeInMem(DtaNonIpPort* pNonIpPort, 
                                                                         Int  NumSymbols);
static Int  DtaNonIpMatrixGetDmaSize(DtaNonIpPort* pNonIpPort,
                                                            Int  LineNumS, Int  NumLines,
                                                            Int  DataFormat, Int  RgbMode,
                                                            Int  AncFlt, Int  Stride);
static DtStatus  DtaNonIpMatrixVidStd2SdiFormat(Int  VidStd, Int* pVideoId,
                                                     Int*  pPictRate, Int*  pProgressive);
static DtStatus  DtaNonIpMatrixSdiFormat2VidStd(Int VideoId, Int  PictRate, 
                                                         Int  Progressive, Int*  pVidStd);
static Bool  DtaNonIpMatrixValidateStartLineAndNumLines(
                               DtAvFrameProps* pFrameProps, Int  StartLine, Int  NumLines);
static Int  DtaNonIpMatrixCountMatrixPorts(DtaNonIpPort* pNonIpPort);
static DtStatus  DtaNonIpMatrixWriteBlackFrame(DtaNonIpPort*  pNonIpPort, Int64  Frame);
static DtStatus  DtaLmh0387WriteRegister(DtaNonIpPort*  pNonIpPort, 
                                                                 Int Addr, UInt32  Value);
#ifdef _DEBUG
static DtStatus  DtaLmh0387ReadRegister(DtaNonIpPort*  pNonIpPort, 
                                                               Int Addr, UInt32*  pValue);
#endif

#ifdef _DEBUG
static const char*  DtaNonIpMatrixState2Str(DtaMatrixPortState State);
#endif

// Configuration modes
#define  DTA_MATRIX_CMODE_FULL        0         // Full re-configuration
#define  DTA_MATRIX_CMODE_RESTART     1         // Re-start only (skip black-frame step)
#define  DTA_MATRIX_CMODE_FORCE       0x8000    // OR with flags above to force re-config

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
    { DT_VIDSTD_1080PSF23_98, DT_HD_SDIFMT_VIDEOID_HD1080, DT_HD_SDIFMT_PICTRATE_23_98, 
                                                           DT_HD_SDIFMT_PROGRESSIVE_PSF },
    { DT_VIDSTD_1080PSF24, DT_HD_SDIFMT_VIDEOID_HD1080, DT_HD_SDIFMT_PICTRATE_24, 
                                                           DT_HD_SDIFMT_PROGRESSIVE_PSF },
    { DT_VIDSTD_1080PSF25, DT_HD_SDIFMT_VIDEOID_HD1080, DT_HD_SDIFMT_PICTRATE_25, 
                                                           DT_HD_SDIFMT_PROGRESSIVE_PSF },
    { DT_VIDSTD_1080PSF29_97, DT_HD_SDIFMT_VIDEOID_HD1080, DT_HD_SDIFMT_PICTRATE_29_97, 
                                                           DT_HD_SDIFMT_PROGRESSIVE_PSF },
    { DT_VIDSTD_1080PSF30, DT_HD_SDIFMT_VIDEOID_HD1080, DT_HD_SDIFMT_PICTRATE_30, 
                                                           DT_HD_SDIFMT_PROGRESSIVE_PSF },
    // 3G 1080 formats (LEVEL A)
    { DT_VIDSTD_1080P50, DT_HD_SDIFMT_VIDEOID_3GLVLA, DT_HD_SDIFMT_PICTRATE_50, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_1080P59_94, DT_HD_SDIFMT_VIDEOID_3GLVLA, DT_HD_SDIFMT_PICTRATE_59_94, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_1080P60, DT_HD_SDIFMT_VIDEOID_3GLVLA, DT_HD_SDIFMT_PICTRATE_60, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    // 3G 1080 formats (LEVEL B)
    { DT_VIDSTD_1080P50B, DT_HD_SDIFMT_VIDEOID_3GLVLB, DT_HD_SDIFMT_PICTRATE_50, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_1080P59_94B, DT_HD_SDIFMT_VIDEOID_3GLVLB, DT_HD_SDIFMT_PICTRATE_59_94, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
    { DT_VIDSTD_1080P60B, DT_HD_SDIFMT_VIDEOID_3GLVLB, DT_HD_SDIFMT_PICTRATE_60, 
                                                            DT_HD_SDIFMT_PROGRESSIVE_ON },
};
static const  Int  DTA_NONIP_MATRIX_VIDSTD_2_FORMAT_NUM_ENTRIES = 
sizeof(DTA_NONIP_MATRIX_VIDSTD_2_FORMAT) / sizeof(DTA_NONIP_MATRIX_VIDSTD_2_FORMAT[0]);


static const  UInt  DTA_NONIP_MATRIX_VIDSTD_2_SMPTE_VIDID[][2] =
{
    {DT_VIDSTD_525I59_94,                       0x81060001 },
    {DT_VIDSTD_625I50,                          0x81050001 },
    {DT_VIDSTD_720P23_98,                       0x84420001 },
    {DT_VIDSTD_720P24,                          0x84430001 },
    {DT_VIDSTD_720P25,                          0x84450001 },
    {DT_VIDSTD_720P29_97,                       0x84460001 },
    {DT_VIDSTD_720P30,                          0x84470001 },
    {DT_VIDSTD_720P50,                          0x84490001 },
    {DT_VIDSTD_720P59_94,                       0x844A0001 },
    {DT_VIDSTD_720P60,                          0x844B0001 },
    {DT_VIDSTD_1080P23_98,                      0x85C20001 },
    {DT_VIDSTD_1080P24,                         0x85C30001 },
    {DT_VIDSTD_1080P25,                         0x85C50001 },
    {DT_VIDSTD_1080P29_97,                      0x85C60001 },
    {DT_VIDSTD_1080P30,                         0x85C70001 },
    {DT_VIDSTD_1080PSF23_98,                    0x85420001 },
    {DT_VIDSTD_1080PSF24,                       0x85430001 },
    {DT_VIDSTD_1080PSF25,                       0x85450001 },
    {DT_VIDSTD_1080PSF29_97,                    0x85460001 },
    {DT_VIDSTD_1080PSF30,                       0x85470001 },
    {DT_VIDSTD_1080I50,                         0x85050001 },
    {DT_VIDSTD_1080I59_94,                      0x85060001 },
    {DT_VIDSTD_1080I60,                         0x85070001 },
    // 3G-SDI level A
    {DT_VIDSTD_1080P50,                         0x89C90001 },
    {DT_VIDSTD_1080P59_94,                      0x89CA0001 },
    {DT_VIDSTD_1080P60,                         0x89CB0001 },
    // 3G-SDI level B
    {DT_VIDSTD_1080P50B,                        0x8A490001 },
    {DT_VIDSTD_1080P59_94B,                     0x8A4A0001 },
    {DT_VIDSTD_1080P60B,                        0x8A4B0001 }
    
};
static const  Int  DTA_NONIP_MATRIX_VIDSTD_2_SMPTE_VIDID_LEN =
                                         sizeof(DTA_NONIP_MATRIX_VIDSTD_2_SMPTE_VIDID) /
                                         sizeof(DTA_NONIP_MATRIX_VIDSTD_2_SMPTE_VIDID[0]);

static const UInt16  PARITY_TABLE256_DATA[256] =
{
    #define P0(n, v) ((((!(n))+1)<<8) | (v))
    #define P2(n, v) P0(n,v), P0(n^1,v+1), P0(n^1,v+2), P0(n,v+3)
    #define P4(n, v) P2(n,v), P2(n^1,v+4), P2(n^1,v+8), P2(n,v+12)
    #define P6(n, v) P4(n,v), P4(n^1,v+16), P4(n^1,v+32), P4(n,v+48)
            P6(0,0), P6(1,64), P6(1,128), P6(0,192)
};
#undef P0
#undef P2
#undef P4
#undef P6

static DtStatus  GetSmptePayloadId(Int  VidStd, UInt*  pPayloadId)
{
    Int  i;
    for (i=0; i<DTA_NONIP_MATRIX_VIDSTD_2_SMPTE_VIDID_LEN; i++)
    {
        if ((Int)DTA_NONIP_MATRIX_VIDSTD_2_SMPTE_VIDID[i][0] == VidStd)
        {
            *pPayloadId = DTA_NONIP_MATRIX_VIDSTD_2_SMPTE_VIDID[i][1];
            return DT_STATUS_OK;
        }
    }
    return DT_STATUS_NOT_FOUND;
}

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

    DtaDeviceAcquireExclAccess(pNonIpPort->m_pDvcData);
    HasAccess = DtaNonIpHasAccess(pNonIpPort, pFile);
    DtaDeviceReleaseExclAccess(pNonIpPort->m_pDvcData);

    if (DT_SUCCESS(HasAccess))
    {
        DtMutexAcquire(&pNonIpPort->m_Matrix.m_StateLock, -1);
    
        if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_ASI)
        {
            if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_INPUT)
                DtaNonIpMatrixSetAsiCtrl(pNonIpPort, DT_RXCTRL_IDLE);
            else
                DtaNonIpMatrixSetAsiCtrl(pNonIpPort, DT_TXCTRL_IDLE);
            DtMutexRelease(&pNonIpPort->m_Matrix.m_StateLock);
        } else {
            DtMutexRelease(&pNonIpPort->m_Matrix.m_StateLock);

            // m_Matrix.m_StateLock must not be held when calling AttachToRow and 
            // DtaNonIpMatrixStop, since it'll call configure which takes that lock 
            // (and recursive locking is not supported under linux).
            DtaNonIpMatrixStop(pNonIpPort); // Stop channel
            DtaNonIpMatrixAttachToRow(pNonIpPort, pNonIpPort->m_PortIndex);
        }
    }

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixConfigure -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpMatrixConfigure(DtaNonIpPort* pNonIpPort, Bool  ForceConfig)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  ConfigMode=0, IoStdValue=pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value;
    Bool  IsInput=FALSE, IsOutput=FALSE, IsDblBuf=FALSE;

    DT_ASSERT(pNonIpPort->m_CapMatrix);

    DtDbgOutPort(MIN, NONIP, pNonIpPort, "Configure HD-Channel");

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Common configuration -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

    IsInput = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_INPUT) &&
                 (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue == DT_IOCONFIG_INPUT);
    IsOutput = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_OUTPUT) &&
                (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue == DT_IOCONFIG_OUTPUT);
    IsDblBuf = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_OUTPUT) &&
                (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue == DT_IOCONFIG_DBLBUF);
    DT_ASSERT((IsInput && !IsOutput && !IsDblBuf) || (!IsInput && IsOutput && !IsDblBuf) 
                                                  || (!IsInput && !IsOutput && IsDblBuf));

    // Set double buffered configuration (if channel supports double-buffering)
    if (pNonIpPort->m_CapDblBuf)
    {
        Int  Buddy = DT_HD_DBLBUF_NONE;
        if (IsDblBuf)
        {
            Status = DtaGetPortNumber(pNonIpPort->m_pDvcData,
                        (Int)pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0], &Buddy);
            DT_ASSERT(DT_SUCCESS(Status));
            if (!DT_SUCCESS(Status))
                Buddy = DT_HD_DBLBUF_NONE;
        }
        DtaRegHdCtrl1SetDblBuf(pNonIpPort->m_pRxRegs, Buddy);
    }
    
    //-.-.-.-.-.-.-.-.-.- ASI / (SD/HD/3G)-SDI specific configuration -.-.-.-.-.-.-.-.-.-.

    ConfigMode = DTA_MATRIX_CMODE_FULL | (ForceConfig ? DTA_MATRIX_CMODE_FORCE : 0);
    if (IoStdValue == DT_IOCONFIG_ASI)
    {
        Status = DtaNonIpMatrixConfigureForAsi(pNonIpPort, ConfigMode);
        // DTA-2152 FW2, DTA-2154 FW5 and DTA-2174 FW2 have a firmware bug:
        // double-buffering for ASI depends on the TxCtrl register. We set it to SEND 
        // immediately. In this mode applications cannot attach to the port and change it.
        if (DT_SUCCESS(Status) && IsDblBuf &&
                               ((pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber==2152 &&
                                pNonIpPort->m_pDvcData->m_DevInfo.m_FirmwareVersion==2) ||
                               (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber==2154 &&
                                pNonIpPort->m_pDvcData->m_DevInfo.m_FirmwareVersion==5) ||
                               (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber==2174 &&
                                pNonIpPort->m_pDvcData->m_DevInfo.m_FirmwareVersion==2)))
        {
            DtaRegHdCtrl1SetRxTxCtrl(pNonIpPort->m_pTxRegs, 3);
        }
    }
    else if (IoStdValue == DT_IOCONFIG_SDI || IoStdValue == DT_IOCONFIG_HDSDI
                                                       || IoStdValue == DT_IOCONFIG_3GSDI)
    {
        DtMutexAcquire(&pNonIpPort->m_Matrix.m_StateLock, -1);
        Status = DtaNonIpMatrixConfigureForSdi(pNonIpPort, DTA_FRMBUF_HOLD_FRAME, 
                                                                              ConfigMode);
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
    if (!pNonIpPort->m_CapMatrix && !pNonIpPort->m_CapMatrix2)
    {
        DtDbgOutPort(ERR, NONIP, pNonIpPort, "Port is not matrix capable");
        return DT_STATUS_NOT_SUPPORTED;
    }
    // Port must be configured as an input
    if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value!=DT_IOCONFIG_INPUT || 
                     pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue!=DT_IOCONFIG_INPUT)
    {
        DtDbgOutPort(ERR, NONIP, pNonIpPort, "Port is not configured as input");
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
    else if (pNonIpPort->m_AsiSdiDeserItfType==ASI_SDI_DESER_ITF_FPGA_BASED
                       || pNonIpPort->m_AsiSdiDeserItfType==ASI_SDI_DESER_ITF_FPGA_LMH0387
                       || pNonIpPort->m_AsiSdiDeserItfType==ASI_SDI_DESER_ITF_FPGA_GS3490)
    {
        // FPGA based => get the video standard from SDI status register
        Int  VideoId = DtaRegHdSdiStatusGetVideoId(pHdRegs);
        Int  PictRate = DtaRegHdSdiStatusGetPictureRate(pHdRegs);
        Int  Progressive = DtaRegHdSdiStatusGetProgressive(pHdRegs);
               
        // Convert SMPTE video standard to our video standard
        DtaNonIpMatrixSdiFormat2VidStd(VideoId, PictRate, Progressive, pVidStd);

        if (pNonIpPort->m_CapMatrix2)
        {
            UInt32  Vpid = DtaRegHdSdiFormat1Get(pHdRegs);
            if (*pVidStd==DT_VIDSTD_1080I50 && ((Vpid&0x0000CFFF) == 0x00004585))
                *pVidStd = DT_VIDSTD_1080PSF25;
            else if (*pVidStd==DT_VIDSTD_1080I59_94 && ((Vpid&0x0000CFFF) == 0x00004685))
                *pVidStd = DT_VIDSTD_1080PSF29_97;
            else if (*pVidStd==DT_VIDSTD_1080I60 && ((Vpid&0x0000CFFF) == 0x00004785))
                *pVidStd = DT_VIDSTD_1080PSF30;
        }
        
        // HW, WITHOUT THE MATRIX2 CAP, cannot detect fractional formats while 
        // set to non-fractional mode and vice versa, so toggle between the two and 
        // check again. NOTE: only  if the channel is not in started and we have a 
        // carrier (no carrier => no use to look for a signal)
        if (!pNonIpPort->m_CapMatrix2 && *pVidStd==DT_VIDSTD_UNKNOWN 
                                              && DtaRegHdStatGetCarrierDetect(pHdRegs)!=0)
        {
            Int  i, l, NumVidStdToggle = 0;
            Int  VidStdToggle[2] = {DT_VIDSTD_UNKNOWN, DT_VIDSTD_UNKNOWN};
            DtaMatrixPortState  State;

            DtDbgOutPort(AVG, NONIP, pNonIpPort, 
                                     "Cannot detect vidstd; will try toggling frac-mode");

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

            //-.-.-.-.-.-.-.-.- Step 2: check what we have to toggle to -.-.-.-.-.-.-.-.-.

            // Check if we're configured as SD input
            if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_SDI)
            {
                // Toggle to both a fractional and a non-fractional HD format
                DT_ASSERT(pNonIpPort->m_Cap1080I59_94);
                DT_ASSERT(pNonIpPort->m_Cap1080I50);
                VidStdToggle[0] = DT_VIDSTD_1080I59_94;
                VidStdToggle[1] = DT_VIDSTD_1080I50;
                NumVidStdToggle = 2;
            }
            // Check if the current video standard is fractional
            else if (DtaVidStdIsFractional(pMatrix->m_FrameProps.m_VidStd))
            {
                // Toggle to a non-fractional format
                DT_ASSERT(pNonIpPort->m_Cap1080I50);
                VidStdToggle[0] = DT_VIDSTD_1080I50;
                NumVidStdToggle = 1;
            }
            else
            {
                // Toggle to a fractional format
                DT_ASSERT(pNonIpPort->m_Cap1080I59_94);
                VidStdToggle[0] = DT_VIDSTD_1080I59_94;
                NumVidStdToggle = 1;
            }

            //.-.-.-.-.-.-.-.-.-.- Step 3: toggle the video standard -.-.-.-.-.-.-.-.-.-.-

            for (i=0; i<NumVidStdToggle && *pVidStd==DT_VIDSTD_UNKNOWN; i++)
            {
                DtaNonIpMatrixVidStd2SdiFormat(VidStdToggle[i], &VideoId, &PictRate, 
                                                                            &Progressive);
                
                DtaRegHdSdiFormat1SetVideoId(pHdRegs, VideoId);
                DtaRegHdSdiFormat1SetPictureRate(pHdRegs, PictRate);
                DtaRegHdSdiFormat1SetProgressive(pHdRegs, Progressive);

                DtDbgOutPort(AVG, NONIP, pNonIpPort, 
                                     "Toggling SDI format to: id=%d, r=%d, p=%d", VideoId,
                                                                   PictRate, Progressive);

                //-.-.-.-.- Step 4: wait shortly and check which standard again -.-.-.-.-.

                // Initial wait to allow channel time to settle
                DtSleep(100);
            
                // Check if we can detect a valid video standard (Max. 10x 40ms)
                for (l=0; l<10; l++)
                {
                    VideoId = DtaRegHdSdiStatusGetVideoId(pHdRegs);
                    PictRate = DtaRegHdSdiStatusGetPictureRate(pHdRegs);
                    Progressive = DtaRegHdSdiStatusGetProgressive(pHdRegs);
               
                    // Convert SMPTE video standard to our video standard
                    DtaNonIpMatrixSdiFormat2VidStd(VideoId, PictRate, Progressive,
                                                                                 pVidStd);
                    DtDbgOut(MAX, NONIP, "Loop %d: id=%d, r=%d, p=%d => std=%d", 
                                             l, VideoId, PictRate, Progressive, *pVidStd);

                    if (*pVidStd != DT_VIDSTD_UNKNOWN)
                        break;

                    // Nothing yet, so wait a little more and try again
                    DtSleep(40);
                }
            }

            //-.-.-.-.-.-.-.- Step 5: finally restore the original setting -.-.-.-.-.-.-.-

            // Restore original SDI format
            DtaNonIpMatrixVidStd2SdiFormat(pMatrix->m_FrameProps.m_VidStd, 
                                                       &VideoId, &PictRate, &Progressive);
            
            DtaRegHdSdiFormat1SetVideoId(pHdRegs, VideoId);
            DtaRegHdSdiFormat1SetPictureRate(pHdRegs, PictRate);
            DtaRegHdSdiFormat1SetProgressive(pHdRegs, Progressive);

            DtDbgOut(MAX, NONIP, "Restore SDI format to: id=%d, r=%d, p=%d", VideoId, 
                                                                   PictRate, Progressive);

            // Release lock
            DtMutexRelease(&pMatrix->m_StateLock);
        }
#ifdef _DEBUG
        else if (DtaRegHdStatGetCarrierDetect(pHdRegs)==0)
        {
            DtDbgOutPort(AVG, NONIP, pNonIpPort, "No carrier => skip toggling frac-mode");
        }
#endif

        //=+=+=+=+=+=+=+=+=+=+=+=+=+=+ WORKAROUND FOR TT2186 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
        //
        // Resetting the IO-serdes-interface helps when the SERDES has entered a state in 
        // which it fails to detect a valid signal on it's input
        //
        // NOTE:
        //  1. Only reset when we have a carrier
        //  2. Initially reset no more than once per second, but after 3 attempts lower
        //     the reset frequency to no more that once per 5-seconds
        //

        if (pNonIpPort->m_CapMatrix2 
                  && (   pNonIpPort->m_AsiSdiDeserItfType==ASI_SDI_DESER_ITF_FPGA_BASED
                      || pNonIpPort->m_AsiSdiDeserItfType==ASI_SDI_DESER_ITF_FPGA_LMH0387
                      || pNonIpPort->m_AsiSdiDeserItfType==ASI_SDI_DESER_ITF_FPGA_GS3490))
        {
            UInt64  CurTime = DtGetTickCount();
            if (DtaRegHdStatGetCarrierDetect(pHdRegs)!=0 && *pVidStd==DT_VIDSTD_UNKNOWN)
            {
                // Should we use short or long interval?
                const Int  ResetInterval = 
                               (pNonIpPort->m_Matrix.m_NumIoSerDesReset>=3) ? 5000 : 1000;
                if ((CurTime - pNonIpPort->m_Matrix.m_LastIoSerDesReset)>=ResetInterval)
                {
                    DtDbgOutPort(ERR, NONIP, pNonIpPort, "Resetting SerDes interface");

                    DtaRegHdCtrl2SetIoSerDesReset(pHdRegs, 1);
                    DtSleep(5);
                    DtaRegHdCtrl2SetIoSerDesReset(pHdRegs, 0);
                    DtSleep(5);

                    // Store time of last reset and update reset count
                    pNonIpPort->m_Matrix.m_LastIoSerDesReset = CurTime;
                    pNonIpPort->m_Matrix.m_NumIoSerDesReset++;
                }
            }
            else
            {
                // No carier or valid signal => restart timeout counter
                pNonIpPort->m_Matrix.m_LastIoSerDesReset = CurTime;
                pNonIpPort->m_Matrix.m_NumIoSerDesReset = 0;
            }
        }

        //
        //=+=+=+=+=+=+=+=+=+=+=+=+=+=+ WORKAROUND FOR TT2186 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
    }
    else
    {
        // For now this function is not supported for any other boards
        DtDbgOutPort(ERR, NONIP, pNonIpPort, "Not supported for this port");
        return DT_STATUS_FAIL;  
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixInterrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtaNonIpMatrixInterrupt(DtaNonIpPort* pNonIpPort)
{
    if (DtaRegHdStatGetLastFrameInt(pNonIpPort->m_pRxRegs) != 0)
    {
        // Call handler. NOTE: the handler will clear the interrupt flag
        DtaNonIpMatrixLastFrameIntHandler(pNonIpPort);
        // Interrupt was ours
        return TRUE;
    }
    return FALSE;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpMatrixInit(DtaNonIpPort* pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaMatrixPort*  pMatrix = &pNonIpPort->m_Matrix;
    DtAvFrameProps*  pFrameProps = &pNonIpPort->m_Matrix.m_FrameProps;
    
    DT_ASSERT(pNonIpPort->m_CapMatrix);
    
    Status = DtDpcInit(&pMatrix->m_LastFrameIntDpc, DtaNonIpMatrixLastFrameIntDpc, TRUE);
    if (!DT_SUCCESS(Status))
        return Status;
    Status = DtEventInit(&pMatrix->m_LastFrameIntEvent, FALSE);
    if (!DT_SUCCESS(Status))
        return Status;

    pMatrix->m_LastIoSerDesReset = 0;
    pMatrix->m_NumIoSerDesReset = 0;
    
    pMatrix->m_FrmIntCnt = 0;
    pMatrix->m_ForceRestart = 0;
        
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

    pMatrix->m_ExtraPixelDelay = 0; // By default, no extra delay

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
        DtDbgOutPort(ERR, NONIP, pNonIpPort,"Cannot attach to a row in current state: %s",
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
        DtDbgOutPort(MIN, NONIP, pNonIpPort, 
                                        "Row index changes (%d->%d); will re-configuring",
                                                               pMatrix->m_RowIdx, RowIdx);
        
        // Set new row and force a re-configure the port
        pMatrix->m_RowIdx = RowIdx;
        Status = DtaNonIpMatrixConfigure(pNonIpPort, TRUE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutPort(ERR, NONIP, pNonIpPort, 
                                    "Failed to reconfigure port (Status=0x%08X)", Status);
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
        // Workaround for TT#2152, TT#2153: no K28.5 symbols in IDLE. Set port to hold
        // instead of idle. No practical difference.
        case DT_TXCTRL_IDLE: RxTxMode = 1; break;
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
        DtaRegHdSetMemTrNumBAsi(pNonIpPort->m_pTxRegs, 0);
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
        DtDbgOutPort(AVG, NONIP, pNonIpPort, "State change %s => %s",
                                                       DtaNonIpMatrixState2Str(OldState),
                                                       DtaNonIpMatrixState2Str(NewState));
    else
        DtDbgOutPort(MAX, NONIP, pNonIpPort, "State remains unchanged: %s",
                                                       DtaNonIpMatrixState2Str(NewState));

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixStart -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpMatrixStart(
    DtaNonIpPort*  pNonIpPort,
    Int64  StartFrame,
    Bool  AutoMode,
    Bool  ForceRestart)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaMatrixPort*  pMatrix = NULL;
    DtaDeviceData*  pDvcData = NULL;
    Int  ConfigMode = 0;
    Bool  NeedReConfig=FALSE, IsOutput=FALSE, IsGenlocked=FALSE;

    DT_ASSERT(pNonIpPort != NULL);
    DT_ASSERT(pNonIpPort->m_CapMatrix);

    pDvcData = pNonIpPort->m_pDvcData;
    DT_ASSERT(pDvcData != NULL);
    pMatrix = &pNonIpPort->m_Matrix;
    DT_ASSERT(pMatrix != NULL);

    // Is this port an output?
    IsOutput = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_OUTPUT);
    // Trying to genlock to active genlock reference?
    if (pNonIpPort->m_CapGenLocked)
    {
        IsGenlocked =
                 (pNonIpPort->m_IoCfg[DT_IOCONFIG_GENLOCKED].m_Value == DT_IOCONFIG_TRUE);
    }

    // Manual frame index control is not supported on legacy interface
    if (DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort) && !AutoMode)
        return DT_STATUS_NOT_SUPPORTED;
    
    DtMutexAcquire(&pMatrix->m_StateLock, -1);
        
    // State cannot be unintialised or configuring
    if (pMatrix->m_State==MATRIX_PORT_UNINIT || pMatrix->m_State==MATRIX_PORT_CONFIGURING)
    {
        DtMutexRelease(&pMatrix->m_StateLock);
        return DT_STATUS_FAIL;
    }

    //.-.-.-.-.-.-.-.-.- Check if we need to re-config/re-start channel -.-.-.-.-.-.-.-.-.
    //
    // There are two reasons namely:
    // 1: For a legacy HD channel interface, we need to re-configure and (re-)start 
    //    the channel by definition
    // 2: A genlockable output needs to be re-started when a genlock reference is used 
    //    to make sure the output aligns/locks to the reference
    //
    //    NOTE: MIGHT WANT TO ACTUALLY USE THE DTAPI_IOCONFIG_GENLOCKED STATE HERE???

    if (DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
    {
        DtDbgOut(MIN, NONIP, "Legacy HD-channel => force re-config of channel");
        NeedReConfig = TRUE;
        ConfigMode = DTA_MATRIX_CMODE_FULL;
    }
    else if (IsOutput && IsGenlocked)
    {
        DtDbgOut(MIN, NONIP, "Genlockable output => force a re-start of channel");
        NeedReConfig = TRUE;
        ConfigMode = DTA_MATRIX_CMODE_RESTART;
    }
    else if (ForceRestart)
    {
        DtDbgOut(MIN, NONIP, "API requested restart => force a re-start of channel");
        NeedReConfig = TRUE;
        ConfigMode = DTA_MATRIX_CMODE_RESTART;
    }
    else
        NeedReConfig = FALSE;

    if (NeedReConfig)
    {
        ConfigMode |= DTA_MATRIX_CMODE_FORCE;   // Force the reconfig/restart
        Status = DtaNonIpMatrixConfigureForSdi(pNonIpPort, StartFrame, ConfigMode);
        if (!DT_SUCCESS(Status))
        {
            DtMutexRelease(&pMatrix->m_StateLock);
            return Status;
        }
    }
    
    DtaNonIpMatrixSetVpidRegs(pNonIpPort);

    // NOTE: A legacy channel is re-configured (see above) and next-frame is set there
    if (!DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
        pMatrix->m_NextFrame = StartFrame;  // Set start frame as forced next frame
    
    // Move to new state
    DtaNonIpMatrixSetState(pNonIpPort, AutoMode ? MATRIX_PORT_RUN_AUTO :
                                                                     MATRIX_PORT_RUN_MAN);
    // Make sure the interrupt routine knows a start was executed
    DtAtomicSet(&pNonIpPort->m_Matrix.m_ForceRestart, 1);
    
    DtMutexRelease(&pMatrix->m_StateLock);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixStop -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpMatrixStop(DtaNonIpPort* pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool  IsSdi=FALSE;
    Int  ConfigMode = 0;
    DtaMatrixPort*  pMatrix = NULL;

    DT_ASSERT(pNonIpPort != NULL);
    DT_ASSERT(pNonIpPort->m_CapMatrix);

    pMatrix = &pNonIpPort->m_Matrix;
    
    pMatrix->m_Vpid1 = 0;
    pMatrix->m_Vpid2 = 0;

    DtMutexAcquire(&pMatrix->m_StateLock, -1);

    // Are we running?
    if (pNonIpPort->m_Matrix.m_State == MATRIX_PORT_HOLD)
    {
        // Already in HOLD => nothing TODO
        DtMutexRelease(&pMatrix->m_StateLock);
        return DT_STATUS_OK;
    }
    
    IsSdi = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_SDI)
                 || (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_HDSDI)
                 || (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_3GSDI);

    // For legacy HD channel interface, we need to re-configure and (re-)start the channel
    if (IsSdi && DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
    {
        // Set the black frame as 'start-frame'; this will result in the channel to
        // start in FREEZE mode
        ConfigMode = DTA_MATRIX_CMODE_FULL | DTA_MATRIX_CMODE_FORCE;
        Status = DtaNonIpMatrixConfigureForSdi(pNonIpPort, DTA_FRMBUF_HOLD_FRAME, 
                                                                              ConfigMode);
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixSetNextFrame -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpMatrixSetNextFrame(DtaNonIpPort* pNonIpPort, Int64  NextFrame)
{
    Int  NextFrmIdx;
    DtaFrameBufSectionConfig*  pSections;
    DT_ASSERT(pNonIpPort != NULL);
    DT_ASSERT(pNonIpPort->m_CapMatrix);

    pSections = pNonIpPort->m_Matrix.m_BufConfig.m_Sections;

    pNonIpPort->m_Matrix.m_NextFrame = NextFrame;
    // Set address for the next frame to be transmited/received
    NextFrmIdx = DtaNonIpMatrixFrame2Index(pNonIpPort, NextFrame);

    DT_ASSERT(pSections[0].m_FrameStartAddr[NextFrmIdx] != -1);
    DtaRegHdS0NextFrmAddrSet(pNonIpPort->m_pRxRegs, 
                                        pSections[0].m_FrameStartAddr[NextFrmIdx]);
    DT_ASSERT(pSections[1].m_FrameStartAddr[NextFrmIdx] != -1);
    DtaRegHdS1NextFrmAddrSet(pNonIpPort->m_pRxRegs, 
                                        pSections[1].m_FrameStartAddr[NextFrmIdx]);
    return DT_STATUS_OK;
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
    DtAvFrameProps*  pFrameProps = NULL;
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
    DtaRegHdSetMemTrNumBAsi(pHdRegs, BufSize);
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
    DtAvFrameProps*  pFrameProps = NULL;
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
                                   && pMemTrSetup->m_DataFormat!=DT_MEMTR_DATAFMT_16B
                                   && pMemTrSetup->m_DataFormat!=DT_MEMTR_DATAFMT_10B_NBO)
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
    // Check: line numbers
    if (!DtaNonIpMatrixValidateStartLineAndNumLines(pFrameProps, pMemTrSetup->m_StartLine, 
                                                                 pMemTrSetup->m_NumLines))
    {
        DtDbgOut(ERR, NONIP, "Invalid start line (%d) and/or number of lines (%d)",
                                       pMemTrSetup->m_StartLine, pMemTrSetup->m_NumLines);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // Stride mode is checked in DtaNonIpMatrixGetReqDmaSize()

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
    
    DtDbgOutPort(MAX, NONIP, pNonIpPort, 
                         "MemTr: cmd=%d, #l=%d, fmt=%d, rgb=%d, sym=%d, scale=%d, "
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
    else if (pMemTrSetup->m_Stride > 0)
    {
        DtaRegHdMemTrNumBytesSet(pHdRegs, pMemTrSetup->m_Stride);
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
    Int  NumLines = 0;
    DtAvFrameProps*  pFrameProps = NULL;
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
    // Check: data format
    if (pMemTrSetup->m_DataFormat!=DT_MEMTR_DATAFMT_8B
                                   && pMemTrSetup->m_DataFormat!=DT_MEMTR_DATAFMT_10B 
                                   && pMemTrSetup->m_DataFormat!=DT_MEMTR_DATAFMT_16B
                                   && pMemTrSetup->m_DataFormat!=DT_MEMTR_DATAFMT_10B_NBO)
    {
        DtDbgOut(ERR, NONIP, "Unsupported data format (%d)", pMemTrSetup->m_DataFormat);
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
    NumLines = pMemTrSetup->m_NumLines;
    
    // Check: scaling mode
    if (pMemTrSetup->m_Scaling == DT_MEMTR_SCMODE_1_4)
    {
        NumLines /= 2;
        LineNumS /= 2;
    }
    else if (pMemTrSetup->m_Scaling == DT_MEMTR_SCMODE_1_16)
    {
        NumLines /= 4;
        LineNumS /= 4;
    }
    else if (pMemTrSetup->m_Scaling != DT_MEMTR_SCMODE_OFF)
    {
        DtDbgOut(ERR, NONIP, "Unsupported scaling mode (%d)", pMemTrSetup->m_Scaling);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Check: symbol filter (for CHROM and LUM-only div #symbols by 2)
    if (pMemTrSetup->m_SymFlt==DT_MEMTR_SYMFLTMODE_CHROM || 
                                           pMemTrSetup->m_SymFlt==DT_MEMTR_SYMFLTMODE_LUM)
    {
        if ((LineNumS % 2) != 0)
            LineNumS += 1;
        LineNumS /= 2;
    }

    // Add 8 symbols for SAV per line iv VANC filter mode is enabled.
    if (pMemTrSetup->m_AncFlt == DT_MEMTR_ANCFLTMODE_VANCALL)
        LineNumS += 8;
    
    // Check: stride mode
    if (pMemTrSetup->m_Stride > 0)
    {
        Int  TransferMinNumBits;
        if (DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
        {
            DtDbgOut(ERR, NONIP, "Stride is not supported on legacy interface");
            return DT_STATUS_INVALID_PARAMETER;
        }

        if ((pMemTrSetup->m_Stride % 16) != 0)
        {
            DtDbgOut(ERR, NONIP, "Stride must be a multiple of 16");
            return DT_STATUS_INVALID_PARAMETER;
        }
    
        if (pMemTrSetup->m_DataFormat == DT_MEMTR_DATAFMT_16B)
            TransferMinNumBits = LineNumS * 16;
        else if (pMemTrSetup->m_DataFormat==DT_MEMTR_DATAFMT_10B 
                                   || pMemTrSetup->m_DataFormat==DT_MEMTR_DATAFMT_10B_NBO)
            TransferMinNumBits = LineNumS * 10;
        else // if (pMemTrSetup->m_DataFormat == DT_MEMTR_DATAFMT_8B)
            TransferMinNumBits = LineNumS * 8;
        
        // With RGB mode enabled we get 3 symbols (R+G+B) for each YUV
        // sample (Y+Cb or Y+Cr).
        if (pMemTrSetup->m_RgbMode == DT_MEMTR_RGBMODE_ON)
            TransferMinNumBits = (TransferMinNumBits*3) / 2;

        if ((TransferMinNumBits+7)/8 > pMemTrSetup->m_Stride)
        {
            DtDbgOut(ERR, NONIP, "Stride too small (%d, min: %d)", pMemTrSetup->m_Stride,
                                                                (TransferMinNumBits+7)/8);
            return DT_STATUS_INVALID_PARAMETER;
        }
    }

    // Compute the expected DMA size
    *pReqDmaSize = DtaNonIpMatrixGetDmaSize(pNonIpPort, LineNumS, NumLines,
                                                                pMemTrSetup->m_DataFormat,
                                                                pMemTrSetup->m_RgbMode,
                                                                pMemTrSetup->m_AncFlt,
                                                                pMemTrSetup->m_Stride);

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

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixLastFrameIntHandler -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpMatrixLastFrameIntHandler(DtaNonIpPort*  pNonIpPort)
{
    DtDpcArgs  DpcArgs;
    Int  CurFrmIdx=0, NextFrmIdx=0, LastFrmIdx = 0;
    Bool  IsRunning = TRUE;
    const DtAvFrameProps*  pFrameProps = NULL;
    DtaFrameBufSectionConfig*  pSections = pNonIpPort->m_Matrix.m_BufConfig.m_Sections;

    DT_ASSERT(pNonIpPort->m_CapMatrix);
    pFrameProps = &pNonIpPort->m_Matrix.m_FrameProps;

    // If the port is configured for ASI => "ignore" this interrupt
    if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value == DT_IOCONFIG_ASI)
    {
        // Clear interrupt flag and return
        DtaRegHdStatClrLastFrameInt(pNonIpPort->m_pRxRegs);
        return DT_STATUS_OK;
    }

    // In the legacy HD-channel register interface, the last/current frame counters are
    // maintained by the hardware as opposed to being maintained in the driver
    if (DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
    {
        // Store last frame
        //
        // NOTE: Due to a firmware bug hardware does not latch the last frame, but the 
        // current frame => last-frame is effectively the same as the current frame.
        //
        // AS WORKARROUND, we therefore now read the current frame register and subtract 
        // 1 to get the last frame
        Int64  CurFrame = DtaRegHdCurrentFrameGet(pNonIpPort->m_pRxRegs);
        UInt64  RefClk = DtaRegRefClkCntGet64(pNonIpPort->m_pDvcData->m_pGenRegs);
        pNonIpPort->m_Matrix.m_LastFrame = CurFrame - 1;
        // Set reference clock for current frame
        CurFrmIdx = DtaNonIpMatrixFrame2Index(pNonIpPort, CurFrame);
        LastFrmIdx = DtaNonIpMatrixFrame2Index(pNonIpPort, 
                                                        pNonIpPort->m_Matrix.m_LastFrame);
        // Set end time reference clock for just the rx-/tx-ed frame
        pNonIpPort->m_Matrix.m_FrameInfo[CurFrmIdx].m_RefClkStart = RefClk;
        pNonIpPort->m_Matrix.m_FrameInfo[LastFrmIdx].m_RefClkEnd = RefClk;
    }
    else
    {
        Int64  NextFrame = DTA_FRMBUF_HOLD_FRAME;
        Bool  ChangeToRun=FALSE, ChangeToHold=FALSE;
        DtaMatrixPortState  NewState = pNonIpPort->m_Matrix.m_State;

        // Set just completed frame as last received/transmitted
        pNonIpPort->m_Matrix.m_LastFrame = pNonIpPort->m_Matrix.m_CurFrame;

        // Check for state change
        IsRunning = (pNonIpPort->m_Matrix.m_LastStateAtInt >= MATRIX_PORT_RUN_AUTO);
        ChangeToRun = !IsRunning && (NewState>=MATRIX_PORT_RUN_AUTO);
        ChangeToHold = NewState==MATRIX_PORT_HOLD && 
                                (pNonIpPort->m_Matrix.m_LastStateAtInt!=MATRIX_PORT_HOLD);

        if (DtAtomicCompareExchange(&pNonIpPort->m_Matrix.m_ForceRestart, 1, 0)==1 &&
                                                         (NewState>=MATRIX_PORT_RUN_AUTO))
        {
            IsRunning = FALSE;
            ChangeToRun = TRUE;
        }

        if (ChangeToRun)
        {
            NextFrame = pNonIpPort->m_Matrix.m_NextFrame;
            DT_ASSERT(NextFrame != -1);
            pNonIpPort->m_Matrix.m_CurFrame = NextFrame - 1;

            DtDbgOutPort(AVG, NONIP, pNonIpPort,
                                 "Enter run state; Starting with frame: %lld", NextFrame);
        }
        else if (NewState == MATRIX_PORT_RUN_AUTO)
        {
            // Update current frame
            pNonIpPort->m_Matrix.m_CurFrame++;  
            NextFrame = pNonIpPort->m_Matrix.m_CurFrame+1;  // set next
        }
        else if (NewState == MATRIX_PORT_RUN_MAN)
        {
            // Update current frame
            pNonIpPort->m_Matrix.m_CurFrame = pNonIpPort->m_Matrix.m_NextFrame;
        }
        else
        {
            // Next frame shall be the hold frame (i.e. a black frame) 
            NextFrame = DTA_FRMBUF_HOLD_FRAME;

            if (ChangeToHold)
                DtDbgOutPort(AVG, NONIP, pNonIpPort,
                                    "Enter hold state; Repeating frame: %lld", NextFrame);
        }

        // Get Index of current and last frames
        CurFrmIdx = DtaNonIpMatrixFrame2Index(pNonIpPort,
                                                         pNonIpPort->m_Matrix.m_CurFrame);
        LastFrmIdx = DtaNonIpMatrixFrame2Index(pNonIpPort,
                                                        pNonIpPort->m_Matrix.m_LastFrame);

        if (NewState >= MATRIX_PORT_RUN_AUTO)
        {
            UInt64  RefClk;
            // Read time the hardware started transmitting/receiving this frame. Use
            // latched register if available, otherwise read the clock here.
            if (pNonIpPort->m_CapMatrix2)
            {
                RefClk = DtaRegHdFrmTimeGet(pNonIpPort->m_pRxRegs);
                //.-.-.-.-.-.-.-.-.-.-.-.- Workaround for TT#2182 -.-.-.-.-.-.-.-.-.-.-.-.
                // Firmware latches start of frame timestamp instead of end of frame
                // timestamp. Compute expected time of one frame and add that to the
                // read value.
                if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber==2154 &&
                                   pNonIpPort->m_pDvcData->m_DevInfo.m_FirmwareVersion<=5)
                {
                    UInt  FrameTime = pNonIpPort->m_pDvcData->m_DevInfo.m_RefClk / 
                                                                       pFrameProps->m_Fps;
                    if (pFrameProps->m_IsFractional)
                    {
                        FrameTime = FrameTime * 1000 / 1001;
                    }
                    RefClk += FrameTime;
                }
                //.-.-.-.-.-.-.-.-.-.-.- End of TT#2182 workaround -.-.-.-.-.-.-.-.-.-.-.-
            } else {
                RefClk = DtaRegRefClkCntGet64(pNonIpPort->m_pDvcData->m_pGenRegs);
            }

            if (DtaVidStdIs3glvlBSdi(pFrameProps->m_VidStd) &&
                        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value==DT_IOCONFIG_INPUT)
            {
                // For 3G level B input the interrupts are 1126 and 1124 lines apart. To
                // correct for this we adjust time we read from the firmware by one
                // line every odd frame.
                UInt  ExpectedFrameTime = 0, LineTime;
                Int64  RealFrameTime;
                switch (pFrameProps->m_VidStd)
                {
                default:
                    DT_ASSERT(FALSE);
                case DT_VIDSTD_1080P50B:
                    ExpectedFrameTime = pNonIpPort->m_pDvcData->m_DevInfo.m_RefClk/50;
                    break;
                case DT_VIDSTD_1080P59_94B:
                    ExpectedFrameTime = pNonIpPort->m_pDvcData->m_DevInfo.m_RefClk/60;
                    ExpectedFrameTime = ExpectedFrameTime * 1001 / 1000;
                    break;
                case DT_VIDSTD_1080P60B:
                    ExpectedFrameTime = pNonIpPort->m_pDvcData->m_DevInfo.m_RefClk/60;
                    break;
                }
                LineTime = ExpectedFrameTime / 1125;
                RealFrameTime = RefClk -
                               pNonIpPort->m_Matrix.m_FrameInfo[LastFrmIdx].m_RefClkStart;
                pNonIpPort->m_Matrix.m_FrameInfo[LastFrmIdx].m_TopHalf = DTA_3GB_UNKNOWN;
                if (RealFrameTime > ExpectedFrameTime+LineTime/2 &&
                                             RealFrameTime < ExpectedFrameTime+LineTime*2)
                {
                    // Frame time is beteen +0.5 line and +2 lines of expected time
                    RefClk -= LineTime;
                    pNonIpPort->m_Matrix.m_FrameInfo[LastFrmIdx].m_TopHalf=DTA_3GB_BOTTOM;
                }
                else if (RealFrameTime > ExpectedFrameTime-LineTime/2 && 
                                             RealFrameTime < ExpectedFrameTime+LineTime/2)
                {
                    // Frame time is beteen -0.5 line and +0.5 lines of expected time
                    pNonIpPort->m_Matrix.m_FrameInfo[LastFrmIdx].m_TopHalf = DTA_3GB_TOP;
                }
            }
            else if (DtaVidStdIs3glvlBSdi(pFrameProps->m_VidStd) &&
                       pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value==DT_IOCONFIG_OUTPUT)
            {
                if (DtaRegHdStatGetCurLvlAToBFrame(pNonIpPort->m_pRxRegs) == 0)
                {
                    pNonIpPort->m_Matrix.m_FrameInfo[LastFrmIdx].m_TopHalf = DTA_3GB_TOP;
                } else {
                    pNonIpPort->m_Matrix.m_FrameInfo[LastFrmIdx].m_TopHalf=DTA_3GB_BOTTOM;
                }
            } else {
                pNonIpPort->m_Matrix.m_FrameInfo[LastFrmIdx].m_TopHalf = DTA_3GB_UNKNOWN;
            }

            // For the start time of the current frame there are two options, namely:
            // 1. Repeat the same frame => use the previous end time as start time
            // 2. New frame => end time of just rx-/tx-ed frame is start time
            if (CurFrmIdx == LastFrmIdx)
                pNonIpPort->m_Matrix.m_FrameInfo[CurFrmIdx].m_RefClkStart = 
                pNonIpPort->m_Matrix.m_FrameInfo[LastFrmIdx].m_RefClkEnd;
            else
                pNonIpPort->m_Matrix.m_FrameInfo[CurFrmIdx].m_RefClkStart = RefClk;
            // Set end time reference clock for just the rx-/tx-ed frame
            pNonIpPort->m_Matrix.m_FrameInfo[LastFrmIdx].m_RefClkEnd = RefClk;
        }

        if (NewState>=MATRIX_PORT_IDLE && (ChangeToRun || NewState!=MATRIX_PORT_RUN_MAN))
        {
            // Set address for the next frame to be transmited/received
            NextFrmIdx = DtaNonIpMatrixFrame2Index(pNonIpPort, NextFrame);

            DT_ASSERT(pSections[0].m_FrameStartAddr[NextFrmIdx] != -1);
            DtaRegHdS0NextFrmAddrSet(pNonIpPort->m_pRxRegs, 
                                               pSections[0].m_FrameStartAddr[NextFrmIdx]);
            DT_ASSERT(pSections[1].m_FrameStartAddr[NextFrmIdx] != -1);
            DtaRegHdS1NextFrmAddrSet(pNonIpPort->m_pRxRegs, 
                                               pSections[1].m_FrameStartAddr[NextFrmIdx]);
        }

        // Save last state seen by interrupt
        pNonIpPort->m_Matrix.m_LastStateAtInt = NewState;
    }

    pNonIpPort->m_Matrix.m_FrmIntCnt++;

    // Schedule DPC for handling of "low-prio" part of Last Frame interrupt, burt only if 
    // the channel is in running state
    if (IsRunning)
    {
        DpcArgs.m_pContext = pNonIpPort;
        DpcArgs.m_Data1.m_UInt64 = (UInt64)pNonIpPort->m_Matrix.m_LastFrame;
        DtDpcSchedule(&pNonIpPort->m_Matrix.m_LastFrameIntDpc, &DpcArgs);
    }

    // Finaly: clear last frame interrupt
    DtaRegHdStatClrLastFrameInt(pNonIpPort->m_pRxRegs);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixConfigureForAsi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNonIpMatrixConfigureForAsi(
    DtaNonIpPort* pNonIpPort, 
    Int  ConfigMode)
{
    DtStatus  Status = DT_STATUS_OK;
    volatile UInt8*  pHdRegs;
    Bool  IsInput=FALSE, IsOutput=FALSE;
    DtaFrameBufSectionConfig*  pSect0;
    Int  SizeOfChannelRam = 0;
    Int  NumMatrixPorts = DtaNonIpMatrixCountMatrixPorts(pNonIpPort);

    DtDbgOutPort(MIN, NONIP, pNonIpPort, "Configuring for ASI");

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

        // NOTE: must wait 100us after setting going to IDLE to allow HW to abort any 
        // processing it was doing
        DtWaitBlock(100);

        DtaNonIpMatrixVidStd2SdiFormat(DT_VIDSTD_1080I50, &VideoId, &PictRate,
                                                                            &Progressive);
        DtaRegHdSdiFormat1SetVideoId(pHdRegs, VideoId);
        DtaRegHdSdiFormat1SetPictureRate(pHdRegs, PictRate);
        DtaRegHdSdiFormat1SetProgressive(pHdRegs, Progressive);

        DtaRegHdCtrl1SetOpMode(pHdRegs, DT_HD_OPMODE_HD);
    }
    //
    //+=+=+=+=+=+=+=+=+=+=+=+=+=+ TT #1637: END OF WORKAROUND +=+=+=+=+=+=+=+=+=+=+=+=+=+=

    // Disable channel before configuring 
    DtaRegHdCtrl1SetOpMode(pHdRegs, DT_HD_OPMODE_DISABLE);
    DtaRegHdCtrl1SetRxTxCtrl(pHdRegs, DT_HD_RXTXCTRL_IDLE);

    // NOTE: must wait 100us after setting going to IDLE to allow HW to abort any 
    // processing it was doing
    DtWaitBlock(100);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Configure IO-direction -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    IsInput = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_INPUT);
    IsOutput = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_OUTPUT);
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

    DtDbgOutPort(AVG, NONIP, pNonIpPort, "Frame Buffer Config:");
    DtDbgOutPort(AVG, NONIP, pNonIpPort, "- S0: sz=%d, begin,end[0x%08X:0x%08X]", 
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

    // Issue a IO reset. NOTE: reset before enabling serialiser/de-serialisers
    DtaRegHdCtrl1SetIoReset(pHdRegs, 1);
    DtSleep(5);
    DtaRegHdCtrl1SetIoReset(pHdRegs, 0);
    DtSleep(5);

    if (IsInput)
    {
        // Check for GS2961 deserialiser
        if (pNonIpPort->m_AsiSdiDeserItfType == ASI_SDI_DESER_ITF_GS2961)
        {
            Status = DtaGs2961Enable(pNonIpPort, TRUE/*ASI MODE*/, FALSE);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, NONIP, "Failed to enabled GS2961 for ASI");
                return Status;
            }
        }
        // Check for FPGA based deser +  LMH0387
        else if (pNonIpPort->m_AsiSdiDeserItfType == ASI_SDI_DESER_ITF_FPGA_LMH0387)
        {
            // Must set the lanch amplitude to recommended default value (30h). 
            // See LMH0387 datasheet, LAUNCH AMPLITUDE OPTIMIZATION (REGISTER 02h)
            Status = DtaLmh0387WriteRegister(pNonIpPort, 0x02, 0x30);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, NONIP, "Failed set LMH0387 launch amplitude");
                return Status;
            }
        }
    }
    else if (IsOutput)
    {
        // Check for GS2962 serialiser
        if (pNonIpPort->m_AsiSdiSerItfType == ASI_SDI_SER_ITF_GS2962)
        {
            Status = DtaGs2962Enable(pNonIpPort);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, NONIP, "Failed to enabled GS2962 for ASI");
                return Status;
            }
        }
    }

    pNonIpPort->m_Matrix.m_AsiDmaNumBytes = 0;
    pNonIpPort->m_Matrix.m_AsiDmaOffset = 0;
    pNonIpPort->m_Matrix.m_AsiCtrl = DT_TXCTRL_IDLE; // Is the same as DT_RXCTRL_IDLE
    DtaRegHdAsiByteCountSet(pHdRegs, 0);
    DtaRegHdSetMemTrNumBAsi(pHdRegs, 0);
    DtaRegHdS0NextFrmAddrSet(pHdRegs, DtaRegHdS0BeginAddrGet(pHdRegs));

    if (IsOutput)
    {
        // Workaround for TT#2152, TT#2153: no K28.5 symbols in IDLE. Set port to hold
        // instead of idle. No practical difference.
        DtaRegHdCtrl1SetRxTxCtrl(pHdRegs, 1);
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixConfigureForSdi -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// PRE: pNonIpPort->m_Matrix.m_StateLock has been acquired
//
static DtStatus  DtaNonIpMatrixConfigureForSdi(
    DtaNonIpPort* pNonIpPort, 
    Int64  StartFrame,
    Int  ConfigMode)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  n, NewVidStd = DT_VIDSTD_UNKNOWN;
    Bool  ConfigRequired=FALSE, IsInput=FALSE, IsOutput=FALSE, IsLegacy=FALSE;
    Bool  EnableVpidProc=TRUE, ForceConfig=FALSE;
    volatile UInt8* pHdRegs =  pNonIpPort->m_pRxRegs;
    DtaFrameBufConfig*  pBufConfig = &pNonIpPort->m_Matrix.m_BufConfig;
    DtaFrameBufSectionConfig*  pSections = pBufConfig->m_Sections;
    
    DtDbgOutPort(MIN, NONIP, pNonIpPort, "Configuring (mode=0x%04X) for (XX-)SDI",
                                                                              ConfigMode);

    // Must have (XX-)SDI capabilty
    if (!pNonIpPort->m_CapSdi && !pNonIpPort->m_CapHdSdi && !pNonIpPort->m_Cap3GSdi)
        return DT_STATUS_NOT_SUPPORTED;

    // Check for forced configuration
    ForceConfig = (ConfigMode & DTA_MATRIX_CMODE_FORCE)!=0 ? TRUE : FALSE;
    ConfigMode &= ~DTA_MATRIX_CMODE_FORCE;  // Remove force flag, leaving just the mode

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
        DtDbgOutPort(MIN, NONIP, pNonIpPort, "No re-configuration of channel required");
        
        DtaNonIpMatrixSetVpidRegs(pNonIpPort);

        // Set to HOLD state
        DtaNonIpMatrixSetState(pNonIpPort, MATRIX_PORT_HOLD);
        return DT_STATUS_OK;
    }
    
    // Enter configuring state
    DtaNonIpMatrixSetState(pNonIpPort, MATRIX_PORT_CONFIGURING);

    // Disable channel before configuring 
    DtaRegHdCtrl1SetOpMode(pHdRegs, DT_HD_OPMODE_DISABLE);
    DtaRegHdCtrl1SetRxTxCtrl(pHdRegs, DT_HD_RXTXCTRL_IDLE);
    
    // NOTE: must wait 50us after going to IDLE to allow HW to abort any 
    // processing it was doing
    DtWaitBlock(50);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.- Configure IO-direction -.-.-.-.-.-.-.-.-.-.-.-.-.-.

    IsInput = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_INPUT);
    IsOutput = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_OUTPUT);
    if (IsInput)
        DtaRegHdCtrl1SetIoDir(pHdRegs, DT_HD_IODIR_INPUT);
    else if (IsOutput)
        DtaRegHdCtrl1SetIoDir(pHdRegs, DT_HD_IODIR_OUTPUT);
    else
        DT_ASSERT(1 == 0);

    //.-.-.-.-.-.-.-.-.-.-.-.-.- Apply frame configuration -.-.-.-.-.-.-.-.-.-.-.-.-.-

    Status = DtaNonIpMatrixApplyConfig(pNonIpPort);
    if (!DT_SUCCESS(Status))
        return Status;

    // Apply buffer configuration
    Status = DtaNonIpMatrixApplyBufferConfig(pNonIpPort);
    if (!DT_SUCCESS(Status))
        return Status;

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Set VPID processing bit -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // Disabled for inputs, enabled for outputs. 3G-level B inputs are a special
    // case, we enable the processing for those.
    EnableVpidProc = IsOutput;
    if (IsInput && DtaVidStdIs3glvlBSdi(NewVidStd))
    {
        EnableVpidProc = TRUE;
    }
    DtaRegHdCtrl1SetNoVpidProc(pHdRegs, EnableVpidProc ? 0 : 1);

    //.-.-.-.-.-.-.-.-.-.-.-.-.- Enable relevant interrupts -.-.-.-.-.-.-.-.-.-.-.-.-.

    Status = DtaNonIpMatrixInterruptEnable(pNonIpPort);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, NONIP, "Failed to enable relevant interrupts");
        return Status;
    }
    
    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Set operation mode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

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

    // Issue a IO reset. NOTE: reset before enabling serialiser/de-serialisers
    DtaRegHdCtrl1SetIoReset(pHdRegs, 1);
    DtSleep(5);
    DtaRegHdCtrl1SetIoReset(pHdRegs, 0);

    if (IsInput)
    {
        // Check for GS2961 deserialiser
        if (pNonIpPort->m_AsiSdiDeserItfType == ASI_SDI_DESER_ITF_GS2961)
        {
            Status = DtaGs2961Enable(pNonIpPort, FALSE/* SDI MODE*/, IsLegacy);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, NONIP, "Failed to enabled GS2961 for SDI");
                return Status;
            }
        }
        // Check for FPGA based deser +  LMH0387
        else if (pNonIpPort->m_AsiSdiDeserItfType == ASI_SDI_DESER_ITF_FPGA_LMH0387)
        {
            // Must set the lanch amplitude to recommended default value (30h). 
            // See LMH0387 datasheet, LAUNCH AMPLITUDE OPTIMIZATION (REGISTER 02h)
            Status = DtaLmh0387WriteRegister(pNonIpPort, 0x02, 0x30);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, NONIP, "Failed set LMH0387 launch amplitude");
                return Status;
            }
        }
    }
    else if (IsOutput)
    {
        // Check for GS2962 serialiser
        if (pNonIpPort->m_AsiSdiSerItfType == ASI_SDI_SER_ITF_GS2962)
        {
            Status = DtaGs2962Enable(pNonIpPort);
            if (!DT_SUCCESS(Status))
            {
                DtDbgOut(ERR, NONIP, "Failed to enabled GS2962 for SDI");
                return Status;
            }
        }
    }

    // For a full config of outputs: write a black-frame
    // NOTE1: we need the interrupts (i.e. DMA done) to be enabled
    // NOTE2: for a quick restart we skip this step as the black-frame has already been 
    //        written to the card and DMA-ing a black-frame is time consuming task
    if (IsOutput && ConfigMode==DTA_MATRIX_CMODE_FULL)
    {
        if (pNonIpPort->m_pDvcData->m_IntEnableState == INT_ENABLED)
        {
            // NOTE: for legacy HD-channels only write black frame if the desired
            // start-frame is the black frame
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
    }
          
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
        // If we start with the hold frame => next frame should also be the hold frame
        if (StartFrame == DTA_FRMBUF_HOLD_FRAME)
            n = DtaNonIpMatrixFrame2Index(pNonIpPort, StartFrame);
        else
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
    Status = DtaNonIpMatrixConfigureForSdi(pNonIpPort, DTA_FRMBUF_HOLD_FRAME, 
                                          DTA_MATRIX_CMODE_FULL | DTA_MATRIX_CMODE_FORCE);
    DtMutexRelease(&pNonIpPort->m_Matrix.m_StateLock);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixApplyConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpMatrixApplyConfig(DtaNonIpPort* pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    DtAvFrameProps*  pFrameProps = NULL;
    volatile UInt8* pHdRegs =  pNonIpPort->m_pRxRegs;
    Bool  IsInput=FALSE, IsOutput=FALSE;

    // Do we have an input or output
    IsInput = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_INPUT) &&
                 (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue == DT_IOCONFIG_INPUT);
    IsOutput = (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_OUTPUT) &&
                (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue == DT_IOCONFIG_OUTPUT);
        
    // First init frame porperties
    Status = DtaNonIpMatrixInitFrameProps(pNonIpPort);
    if (!DT_SUCCESS(Status))
        return Status;
    pFrameProps = &pNonIpPort->m_Matrix.m_FrameProps;
    
    DtDbgOutPort(AVG, NONIP, pNonIpPort, "Frame Properties");
    DtDbgOutPort(AVG, NONIP, pNonIpPort, "- # lines: %d", pFrameProps->m_NumLines);
    DtDbgOutPort(AVG, NONIP, pNonIpPort, "- Field1: f[%d:%d], v[%d:%d]", 
                        pFrameProps->m_Field1Start, pFrameProps->m_Field1End,
                        pFrameProps->m_Field1ActVidStart, pFrameProps->m_Field1ActVidEnd);
    DtDbgOutPort(AVG, NONIP, pNonIpPort, "- Field2: f[%d:%d], v[%d:%d]", 
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
        Int  VidStd = pFrameProps->m_VidStd;
        DtaNonIpMatrixSetVpidRegs(pNonIpPort);

        // Set interlaced flag
        DtaRegHdFrameConfig1SetInterlaced(pHdRegs, pFrameProps->m_IsInterlaced ? 1 : 0);
        // Select fractional or non-fractional clock
        // NOTE: For SD, always select the non-fractional clock
        if (DtaVidStdIsSdSdi(pFrameProps->m_VidStd))
            DtaRegHdCtrl2SetFracClockSel(pHdRegs, 0);
        else
            DtaRegHdCtrl2SetFracClockSel(pHdRegs, pFrameProps->m_IsFractional ? 1 : 0);
        // Enable level A<=>B converter when in 3G-level-B 'mode'
        if (DtaVidStdIs3glvlBSdi(VidStd))
            DtaRegHdCtrl2SetLvlBConvEn(pHdRegs, 1);
        else
            DtaRegHdCtrl2SetLvlBConvEn(pHdRegs, 0);
    }

    // 7. Set pixel delay for to tune gen lock offset
    if (!DtaNonIpMatrixUsesLegacyHdChannelInterface(pNonIpPort))
    {
        Int  PixelDelay=0;
        
        if (IsInput)
            PixelDelay = 0; // For inputs set the delay to 0 by definition
        else
        {
            Int  OutDelayPs, DelayNs, PixelsPerLine, Fps, LineDurationNs, PixelDurationPs;

            Fps = DtaVidStd2Fps(pFrameProps->m_VidStd);
            DT_ASSERT(Fps > 0);
            LineDurationNs = (1000000000 / Fps) / pFrameProps->m_NumLines;
            if (pFrameProps->m_IsFractional)
                LineDurationNs = (LineDurationNs*1001) / 1000;

            PixelsPerLine = DtAvGetNumPixelsPerLine(pFrameProps->m_VidStd);
            if (PixelsPerLine <= 0)
            {
                DT_ASSERT(PixelsPerLine > 0);
                return DT_STATUS_FAIL;
            }
            PixelDurationPs = (LineDurationNs*1000) / PixelsPerLine;

            // FW starts tx with EAV of first line, but timing sync-point leis 
            // "halfway through" the HANC, so need to compensate for this delay
            OutDelayPs = (pFrameProps->m_SyncPointPixelOff) * PixelDurationPs;
            DT_ASSERT(OutDelayPs >= 0);

            // Add the serialiser delay (depends on the video standard)
            if (DtaVidStdIs3gSdi(pFrameProps->m_VidStd))
                OutDelayPs  += (pNonIpPort->m_AsiSdiSerDelayNs3g * 1000);
            else if (DtaVidStdIsHdSdi(pFrameProps->m_VidStd))
                OutDelayPs  += (pNonIpPort->m_AsiSdiSerDelayNsHd * 1000);
            else if (DtaVidStdIsSdSdi(pFrameProps->m_VidStd))
                OutDelayPs  += (pNonIpPort->m_AsiSdiSerDelayNsSd * 1000);
            DT_ASSERT(OutDelayPs >= 0);
                       
            // Compute the delay we need to add to get in genlock
            DelayNs = (pNonIpPort->m_TofAlignOffsetNs) - (OutDelayPs/1000);
            // Convert to #pixels
            PixelDelay = ((DelayNs*1000) + (PixelDurationPs/2)) / PixelDurationPs;

            // Add extra pixel delay
            PixelDelay += pNonIpPort->m_Matrix.m_ExtraPixelDelay;
            // Clip pixel delay to (0..65535)
            if (PixelDelay < 0)            PixelDelay = 0;
            else if (PixelDelay > 0xFFFF)  PixelDelay = 0xFFFF;
                
            DtDbgOutPort(MIN, NONIP, pNonIpPort, "Pixel delay settings");
            DtDbgOutPort(MIN, NONIP, pNonIpPort, "- TofAlignmentOffset=%dns, OutDelay=%dns",
                                       pNonIpPort->m_TofAlignOffsetNs, (OutDelayPs/1000));
            DtDbgOutPort(MIN, NONIP, pNonIpPort, "- Pixel delay=%dns:%dpx; Extra=%dpx", DelayNs,
                                      PixelDelay, pNonIpPort->m_Matrix.m_ExtraPixelDelay);
        }

        // Apply pixel delay setting
        DtaRegHdFrameConfig7SetPixelDelay(pHdRegs, PixelDelay);
    }

    // (re-)compute buffer configuration
    return DtaNonIpMatrixComputeBufferConfig(pNonIpPort);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixSetVpidRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpMatrixSetVpidRegs(DtaNonIpPort* pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    DtAvFrameProps*  pFrameProps = &pNonIpPort->m_Matrix.m_FrameProps;
    volatile UInt8* pHdRegs =  pNonIpPort->m_pRxRegs;

    Int  TypeNumber = pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber;
    Int  FirmwareVersion = pNonIpPort->m_pDvcData->m_DevInfo.m_FirmwareVersion;
    Int  VidStd = pFrameProps->m_VidStd;
    UInt  Vpid1, Vpid2;
    UInt  Line1=0, Line2=0;
    Status = GetSmptePayloadId(VidStd, &Vpid1);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, NONIP, "Unknown video standard (0x%X)", pFrameProps->m_VidStd);
        return Status;
    }
    Vpid1 = ((Vpid1&0xFF)<<24) | ((Vpid1&0xFF00)<<8) | ((Vpid1&0xFF0000)>>8) |(Vpid1>>24);
    Vpid2 = Vpid1 | 0x40000000;
    if ((TypeNumber==2154 && FirmwareVersion<5) ||
                                                (TypeNumber==2174 && FirmwareVersion<2))
    {
        // Temporary workaround: firmware only accepts 0 or 3 for those bits.
        if ((Vpid1&0xC000) != 0)
            Vpid1 |= 0xC000;
        if ((Vpid2&0xC000) != 0)
            Vpid2 |= 0xC000;
    }
    else if (pNonIpPort->m_CapMatrix2)
    {
        // Allow VPID overrides by API in case the card can handle it
        if (pNonIpPort->m_Matrix.m_Vpid1 != 0)
            Vpid1 = pNonIpPort->m_Matrix.m_Vpid1;
        if (pNonIpPort->m_Matrix.m_Vpid2 != 0)
            Vpid2 = pNonIpPort->m_Matrix.m_Vpid2;
    }

    Line1 = pFrameProps->m_SwitchingLines[0] + 3;
    if (pFrameProps->m_SwitchingLines[1] != -1)
        Line2 = pFrameProps->m_SwitchingLines[1] + 3;
    else if (DtaVidStdIs3glvlBSdi(VidStd))
        Line2 = 572;

    if (DtaVidStdIs3glvlBSdi(VidStd) &&
                        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value==DT_IOCONFIG_INPUT)
    {
        // Firmware ignores line numbers and simply extracts any valid VPID from the
        // correct link.
        Line1 = 0xFFFF;
        Line2 = 0xFFFF;
    }
        
    DtaRegHdSdiFormat1Set(pHdRegs, Vpid1);
    DtaRegHdSdiFormat2Set(pHdRegs, Vpid2);
    DtaRegHdFrameConfig6SetVpidLine1(pHdRegs, Line1);
    DtaRegHdFrameConfig6SetVpidLine2(pHdRegs, Line2);

    return Status;
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

    DtDbgOutPort(AVG, NONIP, pNonIpPort, "Frame Buffer Config");
    DtDbgOutPort(AVG, NONIP, pNonIpPort,
                           "- S0: #f,fsz=[%d:%d], begin,end[0x%08X:0x%08X], #s,#b[%d:%d]",
                           pSect[0].m_NumFrames, pSect[0].m_FrameSizeInMem, 
                           pSect[0].m_BeginAddr,  pSect[0].m_EndAddr, 
                           pSect[0].m_LineNumSymbols, pSect[0].m_LineNumBytesInMem);
    DtDbgOutPort(AVG, NONIP, pNonIpPort,
                           "- S1: #f,fsz=[%d:%d], begin,end[0x%08X:0x%08X], #s,#b[%d:%d]",
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
    Int  VidStd = DtaIoStd2VidStd(pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value,
                                       pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue);
    return DtAvGetFrameProps(VidStd, &pNonIpPort->m_Matrix.m_FrameProps);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixComputeBufferConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNonIpMatrixComputeBufferConfig(DtaNonIpPort* pNonIpPort)
{   
    DtStatus  Status = DT_STATUS_OK;
    DtAvFrameProps*  pFrameProps = &pNonIpPort->m_Matrix.m_FrameProps;
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpMatrixGetDmaSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaNonIpMatrixGetDmaSize(
    DtaNonIpPort* pNonIpPort,
    Int  LineNumS,
    Int  NumLines,
    Int DataFormat,
    Int  RgbMode,
    Int  AncFlt,
    Int  Stride)
{
    Int  NumSymbolBits=0, NumDmaBits=0;

    // All DMA transfers must be aligned
    const Int  DMA_TRANSFER_ALIGN_NUM_BITS = pNonIpPort->m_DmaChannel.m_BufAlignment * 8;

    if (Stride > 0)
    {
        NumSymbolBits = Stride * 8 * NumLines;
    } else {
        Int  NumSymbols = LineNumS * NumLines;

        if (AncFlt==DT_MEMTR_ANCFLTMODE_HANCALL || AncFlt==DT_MEMTR_ANCFLTMODE_VANCALL)
            NumSymbols += 8;

        if (DataFormat == DT_MEMTR_DATAFMT_16B)
            NumSymbolBits = NumSymbols * 16;
        else if (DataFormat==DT_MEMTR_DATAFMT_10B || DataFormat==DT_MEMTR_DATAFMT_10B_NBO)
            NumSymbolBits = NumSymbols * 10;
        else if (DataFormat == DT_MEMTR_DATAFMT_8B)
            NumSymbolBits = NumSymbols * 8;

        // with RGB mode enabled we get 3 symbols (R+G+B) for each YUV-sample (Y+Cb or Y+Cr)
        if (RgbMode == DT_MEMTR_RGBMODE_ON)
            NumSymbolBits = (NumSymbolBits*3) / 2;
    }
         

    // All DMA transfers must be aligned
    NumDmaBits = (NumSymbolBits + DMA_TRANSFER_ALIGN_NUM_BITS-1) 
                                                            / DMA_TRANSFER_ALIGN_NUM_BITS;
    NumDmaBits *= DMA_TRANSFER_ALIGN_NUM_BITS;

    // Return number of bytes
    return (NumDmaBits / 8);
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
        // Firmware will never report PSF in SdiStatus register. Instead it'll report
        // the standard as interlaced. We check the VPID to differentiate between the two.
        if (Prog == DT_HD_SDIFMT_PROGRESSIVE_PSF)
            Prog = DT_HD_SDIFMT_PROGRESSIVE_OFF;

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
    DtAvFrameProps* pFrameProps, 
    Int  StartLine, 
    Int  NumLines)
{
    DT_ASSERT(pFrameProps != NULL);
    return StartLine>=1 && NumLines>=1 && (StartLine+NumLines-1)<=pFrameProps->m_NumLines;
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

#ifdef _DEBUG
static char*  VidStdName(Int VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_525I59_94:
        return "525i59.94";
        break;
    case DT_VIDSTD_625I50:
        return "625i50";
        break;
    case DT_VIDSTD_720P23_98:
        return "720p23.98";
        break;
    case DT_VIDSTD_720P24:
        return "720p24";
        break;
    case DT_VIDSTD_720P25:
        return "720p25";
        break;
    case DT_VIDSTD_720P29_97:
        return "720p29.97";
        break;
    case DT_VIDSTD_720P30:
        return "720p30";
        break;
    case DT_VIDSTD_720P50:
        return "720p50";
        break;
    case DT_VIDSTD_720P59_94:
        return "720p59.94";
        break;
    case DT_VIDSTD_720P60:
        return "720p60";
        break;
    case DT_VIDSTD_1080P23_98:
        return "1080p23.98";
        break;
    case DT_VIDSTD_1080P24:
        return "1080p24";
        break;
    case DT_VIDSTD_1080P25:
        return "1080p25";
        break;
    case DT_VIDSTD_1080P29_97:
        return "1080p29.97";
        break;
    case DT_VIDSTD_1080P30:
        return "1080p30";
        break;
    case DT_VIDSTD_1080PSF23_98:
        return "1080psf23.98";
        break;
    case DT_VIDSTD_1080PSF24:
        return "1080psf24";
        break;
    case DT_VIDSTD_1080PSF25:
        return "1080psf25";
        break;
    case DT_VIDSTD_1080PSF29_97:
        return "1080psf29.97";
        break;
    case DT_VIDSTD_1080PSF30:
        return "1080psf30";
        break;
    case DT_VIDSTD_1080I50:
        return "1080i50";
        break;
    case DT_VIDSTD_1080I59_94:
        return "1080i59.94";
        break;
    case DT_VIDSTD_1080I60:
        return "1080i60";
        break;
    case DT_VIDSTD_1080P50:
        return "1080p50";
        break;
    case DT_VIDSTD_1080P50B:
        return "1080p50b";
        break;
    case DT_VIDSTD_1080P59_94:
        return "1080p59.94";
        break;
    case DT_VIDSTD_1080P59_94B:
        return "1080p59.94b";
        break;
    case DT_VIDSTD_1080P60:
        return "1080p60";
        break;
    case DT_VIDSTD_1080P60B:
        return "1080p60b";
        break;
    }
    return "Unknown";
}
#endif

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- AncPacketChecksum -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Calculates and inserts the checksum of an ancillary packet
//
static void  AncPacketChecksum(UInt16* pAncPacketData, Int SymbolOffset)
{
    UInt  Checksum = 0;
    UInt16  ChecksumDataIndex, DataIndex;

    // Check for 0x00 0x3FF 0x3FF header
    if (*pAncPacketData!=0 && *(pAncPacketData+SymbolOffset)!=0x3FF && 
                                                *(pAncPacketData+(SymbolOffset*2))!=0x3FF)
    {
        DtDbgOut(ERR, DTA, "Invalid format for ANC packet");
        return;
    }
    // The length of the ANC data packet is encoded in the fifth word of the packet.
    DataIndex = 5 * SymbolOffset;
    DtDbgOut(AVG, NONIP, "Data index for DC word = %hu", DataIndex);
    ChecksumDataIndex = (pAncPacketData[DataIndex]&0xFF) * SymbolOffset;
    DtDbgOut(AVG, NONIP, "Data word count = %hu", ChecksumDataIndex / SymbolOffset);
    // Add 3 words for header and add 3 words for DID, SDID/DBN and DC.
    ChecksumDataIndex += 6 * SymbolOffset;
    // First word to include in checksum is the first word after the header
    DataIndex = 3 * SymbolOffset;
    DtDbgOut(AVG, NONIP, "Start data index for checksum = %hu", DataIndex);
    while (DataIndex < ChecksumDataIndex)
    {
        Checksum += pAncPacketData[DataIndex];
        DataIndex += (UInt16)SymbolOffset;
    }
    Checksum %= 0x200; // Checksum is sum of data (excl. header) modulo 512

    //  Bit 9 of the checksum word is then defined as the inverse of bit 8.
    if (!(Checksum & 0x100))
        Checksum |= 0x200;
    DtDbgOut(AVG, NONIP, "Checksum for ANC packet = 0x%03X", (UInt16)Checksum);
    DtDbgOut(AVG, NONIP, "Checksum data index = %hu", DataIndex);
    pAncPacketData[ChecksumDataIndex] = (UInt16)Checksum;
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
    DtAvFrameProps*  pProps = NULL;
    UInt8*  pLocalAddress = NULL;
    UInt16 *pLineBuf=NULL, *pEav=NULL, *pSav=NULL, *pHanc=NULL;
    DtPageList* pPageList = NULL;
    Int  s, f, l = 0, LineNumS=0, LineDmaSize=0, DmaSize=0, LineBlockStart;
    
    // Status words (Field 1, Field 2)
    static const  UInt16  EAV_VSYNC[] = { 0x2D8, 0x3C4 };
    static const  UInt16  SAV_VSYNC[] = { 0x2AC, 0x3B0 };
    static const  UInt16  EAV_VIDEO[] = { 0x274, 0x368 };
    static const  UInt16  SAV_VIDEO[] = { 0x200, 0x31C };

    DtDbgOutPort(AVG, NONIP, pNonIpPort, "Writing black-frame to frame-buffer: %lld",
                                                                                   Frame);

    DT_ASSERT(pNonIpPort != NULL);
    DT_ASSERT(pNonIpPort->m_CapMatrix);
        
    pProps = &pNonIpPort->m_Matrix.m_FrameProps;
    pLocalAddress = (UInt8*)(size_t)pNonIpPort->m_FifoOffset;

    DtDbgOutPort(AVG, NONIP, pNonIpPort, "Black frame vid std = %s",
                                                            VidStdName(pProps->m_VidStd));

    //.-.-.-.-.-.-.-.-.- Step 1: Allocate buffer for black-frame lines -.-.-.-.-.-.-.-.-.-

    // Get size of a single line (in # symbols)
    LineNumS = pProps->m_EavNumS + pProps->m_HancNumS + pProps->m_SavNumS + 
                                                                       pProps->m_VancNumS;
    DtDbgOut(AVG, NONIP, "One black frame line has %d symbols", LineNumS);

    // Get DMA size for a single line
    MemTrSetup.m_IsWrite = TRUE;
    MemTrSetup.m_Frame = Frame;
    MemTrSetup.m_StartLine = 1;
    
    MemTrSetup.m_NumLines = 5;  // We will transfer five lines at a time
    DT_ASSERT((pProps->m_NumLines % MemTrSetup.m_NumLines) == 0);
    
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
        DtDbgOut(ERR, DTA, "Cannot allocate DMA buffer for five black-frame lines");
        return DT_STATUS_OUT_OF_MEMORY;
    }

    //-.-.-.-.-.-.-.-.-.- Step 2: Generate lines and write to hardware -.-.-.-.-.-.-.-.-.-

    // Init line buffer with blanking
    for (s=0; s<(LineNumS * 5); s++)
        pLineBuf[s] = (s%2)==0 ? 0x200 : 0x040;

    for (LineBlockStart=1; LineBlockStart<=pProps->m_NumLines; LineBlockStart+=5)
    {
        for (l = LineBlockStart; l < LineBlockStart+5; l++)
        {
            UInt  LineStartOffset;

            LineStartOffset = ((l - 1) % 5) * LineNumS;

            // Generate EAVs and SAVs
            pEav = pLineBuf + LineStartOffset;
            pSav = pLineBuf + (pProps->m_EavNumS + pProps->m_HancNumS) + LineStartOffset;
            pHanc = pLineBuf + pProps->m_EavNumS + LineStartOffset;

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
                *pEav++ = LN0; *pEav++ = LN0; 
                *pEav++ = LN1; *pEav++ = LN1; 
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

            if (!pNonIpPort->m_CapMatrix2 && l>3 && 
                (l-3==pProps->m_SwitchingLines[0] || l-3==pProps->m_SwitchingLines[1]))
            {
                // Insert SMPTE 352M video payload ID 3 lines after switching line
                Int  SymbolOffset = 1;
                UInt  PayloadId;
                if (pProps->m_IsHd)
                {
                    SymbolOffset = 2;
                    // Store packet in odd samples (luminance stream)
                    pHanc++;
                }
                if (DT_SUCCESS(GetSmptePayloadId(pProps->m_VidStd, &PayloadId)))
                {
                    UInt16*  pVpidAncPacket = pHanc;

                    *pHanc = 0;
                    pHanc += SymbolOffset;
                    *pHanc = 0x3FF;
                    pHanc += SymbolOffset;
                    *pHanc = 0x3FF;
                    pHanc += SymbolOffset;
                    *pHanc = 0x241; // DID 0x41
                    pHanc += SymbolOffset;
                    *pHanc = 0x101; // SID 0x01
                    pHanc += SymbolOffset;
                    *pHanc = 0x104; // Packet size 4
                    pHanc += SymbolOffset;

                    *pHanc = PARITY_TABLE256_DATA[(PayloadId>>24) & 0xFF];
                    pHanc += SymbolOffset;
                    *pHanc = PARITY_TABLE256_DATA[(PayloadId>>16) & 0xFF];
                    pHanc += SymbolOffset;
                    *pHanc = PARITY_TABLE256_DATA[(PayloadId>>8) & 0xFF];
                    pHanc += SymbolOffset;
                    *pHanc = PARITY_TABLE256_DATA[PayloadId & 0xFF];
                    pHanc += SymbolOffset;

                    // calculate and insert VPID checksum
                    *pHanc = 0x100;  // Temporary checksum
                    pHanc += SymbolOffset;
                    AncPacketChecksum(pVpidAncPacket, SymbolOffset);
                }
            } else {
                // Overwrite possible smpte 352m data with blanking
                for (s=0; s<10*2; s++)
                    *pHanc++ = (s%2)==0 ? 0x200 : 0x040;
            }
        }
        // Prep for DMA
        MemTrSetup.m_StartLine = l - 5; // set next line to write
        DmaSize = 0;
        Status = DtaNonIpMatrixPrepForDma(pNonIpPort, LineDmaSize, &MemTrSetup, &DmaSize);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOut(ERR, DTA, "Prep-DMA for black-frame (l=%d-%d) failed (ERROR=0x%08X)",
                              MemTrSetup.m_StartLine, MemTrSetup.m_StartLine + 5, Status);
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
            DtDbgOut(ERR, DTA, "DMA-transfer for black-frame (l=%d-%d) failed "
            "(ERROR=0x%08X)", MemTrSetup.m_StartLine, MemTrSetup.m_StartLine + 5, Status);
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

    // Finalize transfer before cleanup
    DtaDmaFinalTransfer(&pNonIpPort->m_DmaChannel);
    
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
    DtaRegHdSetMemTrNumBAsi(pNonIpPort->m_pTxRegs, DmaNumBytes);
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
    DtaIoConfigValue  CfgValue;
    DT_ASSERT(pNonIpPort != NULL);

    // The GetFifoLoad function will also set the overflow flag if an overflow occurred.
    DtaMatrixAsiRxGetFifoLoad(pNonIpPort, &FifoLoad);

    DtaNonIpIoConfigGet(pNonIpPort, DT_IOCONFIG_IODIR, &CfgValue);
    if (CfgValue.m_Value == DT_IOCONFIG_OUTPUT)
    {
        // Process TX flags
        DtaNonIpTxProcessFlagsFromDpc(pNonIpPort);
    }
    else if (CfgValue.m_Value == DT_IOCONFIG_INPUT)
    {
        // Process RX flags
        DtaNonIpRxProcessFlagsFromDpc(pNonIpPort);
    }
}

#ifdef _DEBUG
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh0387ReadRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh0387ReadRegister(DtaNonIpPort*  pNonIpPort, Int Addr, UInt32*  pValue)
{
    UInt  Timeout=100, SpiCmd=0;

    // Port must has a matrix-API register interface
    if (!pNonIpPort->m_CapMatrix)
        return DT_STATUS_NOT_SUPPORTED;

    // Step 1: write read command to card
    SpiCmd = (Addr << DT_HD_LMH0387SPI_ADDR_SH) & DT_HD_LMH0387SPI_ADDR_MSK;
    SpiCmd |= (1<<DT_HD_LMH0387SPI_READ_SH) & DT_HD_LMH0387SPI_READ_MSK;
    SpiCmd |= (1<<DT_HD_LMH0387SPI_START_SH) & DT_HD_LMH0387SPI_START_MSK;

    WRITE_UINT(SpiCmd, pNonIpPort->m_pRxRegs, DT_HD_REG_LMH0387SPI);

    // Step 2: wait for done bit
    SpiCmd = READ_UINT(pNonIpPort->m_pRxRegs, DT_HD_REG_LMH0387SPI);
    while ((SpiCmd & DT_HD_LMH0387SPI_DONE_MSK) == 0)
    {
        DtSleep(1);
        SpiCmd = READ_UINT(pNonIpPort->m_pRxRegs, DT_HD_REG_LMH0387SPI);
        Timeout--;
        if (Timeout <= 0)
            return DT_STATUS_TIMEOUT;
    }
    *pValue = (UInt32)((SpiCmd & DT_HD_LMH0387SPI_DATA_MSK) >> DT_HD_LMH0387SPI_DATA_SH);
    return DT_STATUS_OK;
}
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaLmh0387WriteRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaLmh0387WriteRegister(DtaNonIpPort*  pNonIpPort, Int Addr, UInt32  Value)
{
    UInt  Timeout=100, SpiCmd=0;

    // Port must has a matrix-API register interface
    if (!pNonIpPort->m_CapMatrix)
        return DT_STATUS_NOT_SUPPORTED;

    // Step 1: write write-command to card
    SpiCmd = (Value << DT_HD_LMH0387SPI_DATA_SH) & DT_HD_LMH0387SPI_DATA_MSK;
    SpiCmd |= (Addr << DT_HD_LMH0387SPI_ADDR_SH) & DT_HD_LMH0387SPI_ADDR_MSK;
    SpiCmd |= (0<<DT_HD_LMH0387SPI_READ_SH) & DT_HD_LMH0387SPI_READ_MSK;
    SpiCmd |= (1<<DT_HD_LMH0387SPI_START_SH) & DT_HD_LMH0387SPI_START_MSK;

    DtDbgOut(MAX, DTA, "Addr=0x%04X, Value=0x%04X, SpiCmd=0x%08X", Addr, Value, SpiCmd);
    
    WRITE_UINT(SpiCmd, pNonIpPort->m_pRxRegs, DT_HD_REG_LMH0387SPI);

    // Step 2: wait for done bit
    SpiCmd = READ_UINT(pNonIpPort->m_pRxRegs, DT_HD_REG_LMH0387SPI);
    while ((SpiCmd & DT_HD_LMH0387SPI_DONE_MSK) == 0)
    {
        DtSleep(1);
        SpiCmd = READ_UINT(pNonIpPort->m_pRxRegs, DT_HD_REG_LMH0387SPI);
        Timeout--;
        if (Timeout <= 0)
            return DT_STATUS_TIMEOUT;
    }
    return DT_STATUS_OK;
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
    case MATRIX_PORT_RUN_AUTO:      return "RUN_AUTO";
    case MATRIX_PORT_RUN_MAN:       return "RUN_MAN";
    default:                        return "UNKNOWN";
    }
}
#endif
