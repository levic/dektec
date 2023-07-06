//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* SdiAvRx.c *#*#*#*#*#*#*#*#*# (C) 2015-2016 DekTec
//
// Dta driver -  SDI Audio/Video Receiver read/write operations
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Local functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
static void  DtaNonIpSdiAvRxIntDpc(DtDpcArgs* pArgs);
static DtStatus DtaSdiAvRxGetAudSelect(DtaNonIpPort*  pNonIpPort,
                                              DtaIoctSdiAvRxCmdGetAudSelectOutput* pData);
static DtStatus DtaSdiAvRxGetAudSelect2(DtaNonIpPort*  pNonIpPort,
                                             DtaIoctSdiAvRxCmdGetAudSelect2Output* pData);
static DtStatus DtaSdiAvRxSetAudSelect(DtaNonIpPort*  pNonIpPort,
                                               DtaIoctSdiAvRxCmdSetAudSelectInput* pData);
static DtStatus DtaSdiAvRxSetAudSelect2(DtaNonIpPort*  pNonIpPort,
                                               DtaIoctSdiAvRxCmdSetAudSelect2Input* pData);
static DtStatus DtaSdiAvRxReadSmpte2020(DtaNonIpPort*  pNonIpPort,
                                             DtaIoctSdiAvRxCmdReadSmpte2020Output* pData);

static DtStatus DtaSdiAvRxReadAvStatus(DtaNonIpPort*  pNonIpPort, Int Force);
static DtStatus DtaSdiAvRxClearAvStatus(DtaSdiAvRxPort* pSdiAvRx);
static DtStatus DtaSdiAvRxClearVidStatus(DtaSdiAvRxPort* pSdiAvRx);
static DtStatus DtaSdiAvRxClearAudStatus(DtaSdiAvRxPort* pSdiAvRx, Int ChanPair);
static DtStatus  DtaNonIpSdiAvRxDoDetectVidStd(DtaSdiAvRxPort* pSdiAvRx, Int*  pVidStd,
                                                             UInt*  pVpid, UInt*  pVpid2);


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSdiAvRxInitPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtaSdiAvRxInitPowerup(DtaNonIpPort*  pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    Int i;
    DtaSdiAvRxPort*  pSdiAvRx = &pNonIpPort->m_SdiAvRx;
    volatile UInt8* pFwbRegs = pNonIpPort->m_pFwbRegs;

    // Get the audio extractor mode support
    pSdiAvRx->m_SuppAudioExtMode = 
                               (pNonIpPort->m_pDvcData->m_DevInfo.m_FirmwareVersion >= 2);

    for (i=0; i<pSdiAvRx->m_NumAudioExtractors; i++)
    {
        // Initialize the audio channel pair selection
        DtaFwbRegWrite(pFwbRegs, 
                             &FwbSdiAvReceiver.AudioExtract[i].Control_ChanPairSelect, i);
        // Initialize the audio extraction mode
        if (pSdiAvRx->m_SuppAudioExtMode)
            DtaFwbRegWrite(pFwbRegs, 
                             &FwbSdiAvReceiver.AudioExtract[i].Control_ChanExtractMode, 
                             DTFWB_AUDEXT_EXTRACT_BOTH);
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSdiAvRxInterrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtaSdiAvRxInterrupt(DtaNonIpPort*  pNonIpPort)
{
    Bool  IrqHandled = FALSE;
    DtDpcArgs  DpcArgs;

    // Check for frame interrupt
    if (DtaFwbRegRead(pNonIpPort->m_pFwbRegs, &FwbSdiAvReceiver.Status_FrameIntStatus))
    {
        // This is the SDI-AV-RX interrupt. Clear it
        DtaFwbRegClear(pNonIpPort->m_pFwbRegs, &FwbSdiAvReceiver.Status_FrameIntStatus);

        // Schedule DPC to handle the interrupt.
        DpcArgs.m_pContext = pNonIpPort;
        DtDpcSchedule(&pNonIpPort->m_SdiAvRx.m_IntDpc, &DpcArgs);

        // Interrupt was ours
        IrqHandled = TRUE;
    }
    return IrqHandled;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpSdiAvRxInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpSdiAvRxInit(DtaNonIpPort*  pNonIpPort)
{
    DtaSdiAvRxPort*  pSdiAvRx = &pNonIpPort->m_SdiAvRx;
    Int  i;
    DT_ASSERT(pNonIpPort->m_CapSdiRx);

    // Get periodic timer interrupt interval
    pSdiAvRx->m_PerIntItvUs = (Int)DtaGetPerIntItvUS(pNonIpPort->m_pDvcData);
    
    // Disable timeout
    pSdiAvRx->m_FrameIntTimeoutUs = -1;

    // Initialize extraction mode support
    pSdiAvRx->m_SuppAudioExtMode = FALSE;

    // Set the maximum number of audio extractors
    pSdiAvRx->m_NumAudioExtractors = DTA_SDIAVRX_MAX_NUM_AUDPAIRS;
    
    // Initialize the audio channel pair selection
    for (i=0; i<pSdiAvRx->m_NumAudioExtractors; i++)
        pSdiAvRx->m_AudioStatus[i].m_ChanPairSelect = i;

    // Clear AV status
    DtaSdiAvRxClearAvStatus(pSdiAvRx);

    // All status data should be updated
    pSdiAvRx->m_UpdateAll = 1;
    
    DtSpinLockInit(&pSdiAvRx->m_StateLock);
    DtDpcInit(&pSdiAvRx->m_IntDpc, DtaNonIpSdiAvRxIntDpc, TRUE);

    pSdiAvRx->m_NewVidStdCb2 = NULL;

    DtSpinLockInit(&pSdiAvRx->m_AncEx.m_Lock);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpSdiAvRxInterruptEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Enables Non-IP SDI Audio/Video Receiver port specific interrupts
//
DtStatus  DtaNonIpSdiAvRxInterruptEnable(DtaNonIpPort* pNonIpPort)
{
    DtIntEnableState  IntEnaState;
    DtaSdiAvRxPort*  pSdiAvRx = &pNonIpPort->m_SdiAvRx;
    volatile UInt8* pFwbRegs = pNonIpPort->m_pFwbRegs;
    DT_ASSERT(pNonIpPort->m_CapSdiRx);

    IntEnaState = pNonIpPort->m_pDvcData->m_IntEnableState;
   
    // Initialize the number of audio extractors
    pSdiAvRx->m_NumAudioExtractors = DtaFwbRegRead(pFwbRegs, 
                                                    &FwbSdiAvReceiver.Config_NumAudExtrs);
    DT_ASSERT(pSdiAvRx->m_NumAudioExtractors <= DTA_SDIAVRX_MAX_NUM_AUDPAIRS);

    // Do nothing if master state is not enabled or enabling
    if (IntEnaState!=INT_ENABLING && IntEnaState!=INT_ENABLED)
    {
        DtDbgOut(AVG, NONIP, "Master-interupt state (%d) is not enabled", IntEnaState);
        return DT_STATUS_OK;
    }
    
    // Clear status
    DtaSdiAvRxClearAvStatus(pSdiAvRx);

    // All status data should be updated
    pSdiAvRx->m_UpdateAll = 1;

    // Enable ANC extractor
    DtaFwbRegWrite(pFwbRegs, &FwbSdiAvReceiver.AncExtract.Control_PacketDid, 0x45);
    DtaFwbRegWrite(pFwbRegs, &FwbSdiAvReceiver.AncExtract.Control_ExtractEnable, 1);

    // Enable timeout
    pSdiAvRx->m_FrameIntTimeoutUs = DTA_SDIAVRX_FRAMEINT_TIMEOUT;

    DtaFwbRegWrite(pFwbRegs, &FwbSdiAvReceiver.Control_FrameIntEnable, 1);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpSdiAvRxInterruptDisable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Disables Non-IP SDI Audio/Video Receiver port specific interrupts
//
DtStatus  DtaNonIpSdiAvRxInterruptDisable(DtaNonIpPort* pNonIpPort)
{
    volatile UInt8* pFwbRegs = pNonIpPort->m_pFwbRegs;
    DT_ASSERT(pNonIpPort->m_CapSdiRx);
    
    DtaFwbRegWrite(pFwbRegs, &FwbSdiAvReceiver.AncExtract.Control_ExtractEnable, 0);

    DtaFwbRegWrite(pFwbRegs, &FwbSdiAvReceiver.Control_FrameIntEnable, 0);

    // Disable timeout
    pNonIpPort->m_SdiAvRx.m_FrameIntTimeoutUs = -1;
    
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpSdiAvRxPeriodicInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Checks whether a frame-interrupt has occurred 
//
//
DtStatus  DtaNonIpSdiAvRxPeriodicInt(DtaNonIpPort* pNonIpPort)
{
    DtaSdiAvRxPort*  pSdiAvRx = &pNonIpPort->m_SdiAvRx;
    DtSpinLockAcquireAtDpc(&pSdiAvRx->m_StateLock);
    
    if (pSdiAvRx->m_FrameIntTimeoutUs >= 0)
    {
        pSdiAvRx->m_FrameIntTimeoutUs -= pSdiAvRx->m_PerIntItvUs;
        if (pSdiAvRx->m_FrameIntTimeoutUs < 0)
        {
            Int  OldVidStd = DT_VIDSTD_UNKNOWN;
            Int  NewVidStd = DT_VIDSTD_UNKNOWN;
            DtaNonIpSdiAvRxDoDetectVidStd(pSdiAvRx, &OldVidStd, NULL, NULL);
            // Clear AV status
            DtaSdiAvRxClearAvStatus(pSdiAvRx);

            // All status data should be updated
            pSdiAvRx->m_UpdateAll = 1;
            
            DtaNonIpSdiAvRxDoDetectVidStd(pSdiAvRx, &NewVidStd, NULL, NULL);

            if (OldVidStd != NewVidStd)
            {
                pSdiAvRx->m_NewVidStdCb(pSdiAvRx->m_pPortCb, pNonIpPort->m_PortIndex,
                                                                               NewVidStd);
                if (pSdiAvRx->m_NewVidStdCb2 != NULL)
                    pSdiAvRx->m_NewVidStdCb2(pSdiAvRx->m_pPortCb2, pNonIpPort->m_PortIndex,
                                                                               NewVidStd);
            }

            DtDbgOut(MAX, SDIAVRX, "Frame interrupt timeout");
        }
    }
    DtSpinLockReleaseFromDpc(&pSdiAvRx->m_StateLock);

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpSdiAvRxIntDpc -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaNonIpSdiAvRxIntDpc(DtDpcArgs* pArgs)
{
    DtaNonIpPort*  pNonIpPort = (DtaNonIpPort*)pArgs->m_pContext;
    DtaSdiAvRxPort*  pSdiAvRx = &pNonIpPort->m_SdiAvRx;
    Int  FifoLoad;
    Int  i;
    Int  BufSize;
    Int  NewVidStd = DT_VIDSTD_UNKNOWN;

    DtSpinLockAcquireAtDpc(&pSdiAvRx->m_StateLock);
    
    // Reset timeout
    pSdiAvRx->m_FrameIntTimeoutUs = DTA_SDIAVRX_FRAMEINT_TIMEOUT;
    
    // Update audio video status
    DtaSdiAvRxReadAvStatus(pNonIpPort, pSdiAvRx->m_UpdateAll);
    pSdiAvRx->m_UpdateAll = 0;
    DtaNonIpSdiAvRxDoDetectVidStd(pSdiAvRx, &NewVidStd, NULL, NULL);
    DtSpinLockReleaseFromDpc(&pSdiAvRx->m_StateLock);
    pSdiAvRx->m_NewVidStdCb(pSdiAvRx->m_pPortCb, pNonIpPort->m_PortIndex, NewVidStd);
    if (pSdiAvRx->m_NewVidStdCb2 != NULL)
        pSdiAvRx->m_NewVidStdCb2(pSdiAvRx->m_pPortCb2, pNonIpPort->m_PortIndex, NewVidStd);
    
    // Read ANC extract FIFO
    DtSpinLockAcquireAtDpc(&pSdiAvRx->m_AncEx.m_Lock);
    FifoLoad = DtaFwbRegRead(pNonIpPort->m_pFwbRegs,
                                     &FwbSdiAvReceiver.AncExtract.Status_FifoLoadLatched);
    BufSize = sizeof(pSdiAvRx->m_AncEx.m_Buf);
    for (i=0; i*4<FifoLoad && i<BufSize; i++)
    {
        pSdiAvRx->m_AncEx.m_Buf[i] = DtaFwbRegRead(pNonIpPort->m_pFwbRegs,
                                                   &FwbSdiAvReceiver.AncExtract.ReadData);
    }
    pSdiAvRx->m_AncEx.m_BufLoad = i*sizeof(UInt32);
    DtSpinLockReleaseFromDpc(&pSdiAvRx->m_AncEx.m_Lock);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSdiAvRxIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaSdiAvRxIoctl(
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
    DtaIoctlSdiAvRxCmdInput*  pSdiAvRxCmdInput = 
                                         (DtaIoctlSdiAvRxCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlSdiAvRxCmdOutput*  pSdiAvRxCmdOutput =
                                       (DtaIoctlSdiAvRxCmdOutput*)pIoctl->m_pOutputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlSdiAvRxCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlSdiAvRxCmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    pNonIpPort = NULL;  // Assume a board level request
    if (DT_SUCCESS(DtaGetNonIpPortIndex(pDvcData, pSdiAvRxCmdInput->m_PortIndex,
                                                                        &NonIpPortIndex)))
        pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpPortIndex];
    else
         return DT_STATUS_INVALID_PARAMETER;
    if (!pNonIpPort->m_CapSdiRx)
        return DT_STATUS_NOT_SUPPORTED;
    
    // Determine final required output/input sizes
    switch (pSdiAvRxCmdInput->m_Cmd)
    {
    case DTA_SDIAVRX_CMD_GET_AUDIOSELECT:
        pCmdStr = "DTA_SDIAVRX_CMD_GET_AUDIOSELECT";
        InReqSize += 0;  // We expect no additional data in the input buffer
        OutReqSize += sizeof(DtaIoctSdiAvRxCmdGetAudSelectOutput);
        break;
    case DTA_SDIAVRX_CMD_GET_AUDIOSELECT2:
        pCmdStr = "DTA_SDIAVRX_CMD_GET_AUDIOSELECT2";
        InReqSize += 0;  // We expect no additional data in the input buffer
        OutReqSize += sizeof(DtaIoctSdiAvRxCmdGetAudSelect2Output);
        break;
    case DTA_SDIAVRX_CMD_SET_AUDIOSELECT:
        pCmdStr = "DTA_SDIAVRX_CMD_SET_AUDIOSELECT";
        InReqSize += sizeof(DtaIoctSdiAvRxCmdSetAudSelectInput);
        OutReqSize += 0; // We expect no additional data in the output buffer
        break;
    case DTA_SDIAVRX_CMD_SET_AUDIOSELECT2:
        pCmdStr = "DTA_SDIAVRX_CMD_SET_AUDIOSELECT2";
        InReqSize += sizeof(DtaIoctSdiAvRxCmdSetAudSelect2Input);
        OutReqSize += 0; // We expect no additional data in the output buffer
        break;
    case DTA_SDIAVRX_CMD_READ_SMPTE2020:
        pCmdStr = "DTA_SDIAVRX_CMD_READ_SMPTE2020";
        InReqSize += 0;  // We expect no additional data in the input buffer
        OutReqSize += sizeof(DtaIoctSdiAvRxCmdReadSmpte2020Output);
        break;

    default:
        pCmdStr = "??UNKNOWN SDIAVRXCMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, SDIAVRX, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr,
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, SDIAVRX, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, SDIAVRX, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
            pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {

        switch (pSdiAvRxCmdInput->m_Cmd)
        {
        case DTA_SDIAVRX_CMD_GET_AUDIOSELECT:
            Status = DtaSdiAvRxGetAudSelect(pNonIpPort,
                                                  &pSdiAvRxCmdOutput->m_Data.m_AudSelect);
            break;

        case DTA_SDIAVRX_CMD_GET_AUDIOSELECT2:
            Status = DtaSdiAvRxGetAudSelect2(pNonIpPort,
                                                 &pSdiAvRxCmdOutput->m_Data.m_AudSelect2);
            break;

        case DTA_SDIAVRX_CMD_SET_AUDIOSELECT:
            Status = DtaSdiAvRxSetAudSelect(pNonIpPort,
                                                   &pSdiAvRxCmdInput->m_Data.m_AudSelect);
            break;

        case DTA_SDIAVRX_CMD_SET_AUDIOSELECT2:
            Status = DtaSdiAvRxSetAudSelect2(pNonIpPort,
                                                  &pSdiAvRxCmdInput->m_Data.m_AudSelect2);
            break;

        case DTA_SDIAVRX_CMD_READ_SMPTE2020:
            Status = DtaSdiAvRxReadSmpte2020(pNonIpPort,
                                              &pSdiAvRxCmdOutput->m_Data.m_ReadSmpte2020);
            break;
        }
    }

    // If we failed, no data has to be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED)
            DtDbgOut(MIN, SDIAVRX, "SDIAVRXCMD=0x%x: NOT SUPPORTED", pSdiAvRxCmdInput->m_Cmd);
        else if (Status == DT_STATUS_IO_PENDING)
            DtDbgOut(MAX, SDIAVRX, "%s: ERROR %xh", pCmdStr, Status); // NOT A REAL ERROR
        else
            DtDbgOut(MIN, SDIAVRX, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpSdiAvRxDetectVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// PRE: pVidStd is set to DT_VIDSTD_UNKNOWN
// pVpid/pVpid2 may be NULL. Only need to be overwritten when not NULL and valid.
DtStatus  DtaNonIpSdiAvRxDetectVidStd(
    DtaNonIpPort*  pNonIpPort,
    Int*  pVidStd,
    UInt*  pVpid,
    UInt*  pVpid2)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaSdiAvRxPort* pSdiAvRx = &pNonIpPort->m_SdiAvRx;

    DtSpinLockAcquire(&pSdiAvRx->m_StateLock);
    Status = DtaNonIpSdiAvRxDoDetectVidStd(pSdiAvRx, pVidStd, pVpid, pVpid2);
    DtSpinLockRelease(&pSdiAvRx->m_StateLock);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpSdiAvRxDetectVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// PRE: pSdiAvRx->m_StateLock is acquired
// PRE: pVidStd is set to DT_VIDSTD_UNKNOWN
// pVpid/pVpid2 may be NULL. Only need to be overwritten when not NULL and valid.
DtStatus  DtaNonIpSdiAvRxDoDetectVidStd(
    DtaSdiAvRxPort* pSdiAvRx,
    Int*  pVidStd,
    UInt*  pVpid,
    UInt*  pVpid2)
{
    DtStatus  Status = DT_STATUS_OK;

    if (pSdiAvRx->m_VideoStatus.m_IsSdiValid)
    {
        UInt  Vpid =  pSdiAvRx->m_VideoStatus.m_IsVpidValid ?
                                                       pSdiAvRx->m_VideoStatus.m_Vpid : 0;
        switch (pSdiAvRx->m_SdiStandard)
        {
        case DTFWB_SDIDES_SDISTD_SD:
            switch (pSdiAvRx->m_VideoStatus.m_VideoFormat)
            {
            case DTFWB_SDIDES_VIDFRMT_525I:
                *pVidStd = DT_VIDSTD_525I59_94;
                break;
            case DTFWB_SDIDES_VIDFRMT_625I:
                *pVidStd = DT_VIDSTD_625I50;
                break;
            }
            break;

        case DTFWB_SDIDES_SDISTD_HD:
            switch (pSdiAvRx->m_VideoStatus.m_VideoFormat)
            {
            case DTFWB_SDIDES_VIDFRMT_720P:
                switch (pSdiAvRx->m_VideoStatus.m_PictureRate)
                {
                case DTFWB_SDIDES_RATE_23_98:
                    *pVidStd = DT_VIDSTD_720P23_98;
                    break;
                case DTFWB_SDIDES_RATE_24:
                    *pVidStd = DT_VIDSTD_720P24;
                    break;
                case DTFWB_SDIDES_RATE_25:
                    *pVidStd = DT_VIDSTD_720P25;
                    break;
                case DTFWB_SDIDES_RATE_29_97:
                    *pVidStd = DT_VIDSTD_720P29_97;
                    break;
                case DTFWB_SDIDES_RATE_30:
                    *pVidStd = DT_VIDSTD_720P30;
                    break;
                case DTFWB_SDIDES_RATE_50:
                    *pVidStd = DT_VIDSTD_720P50;
                    break;
                case DTFWB_SDIDES_RATE_59_94:
                    *pVidStd = DT_VIDSTD_720P59_94;
                    break;
                case DTFWB_SDIDES_RATE_60:
                    *pVidStd = DT_VIDSTD_720P60;
                    break;
                }
                break;
            case DTFWB_SDIDES_VIDFRMT_1080I:
                switch (pSdiAvRx->m_VideoStatus.m_PictureRate)
                {
                case DTFWB_SDIDES_RATE_23_98:
                    if ((Vpid&0xCFFF) == 0x4285)
                        *pVidStd = DT_VIDSTD_1080PSF23_98;
                    break;
                case DTFWB_SDIDES_RATE_24:
                    if ((Vpid&0xCFFF) == 0x4385)
                        *pVidStd = DT_VIDSTD_1080PSF24;
                    break;
                case DTFWB_SDIDES_RATE_25:
                    *pVidStd = DT_VIDSTD_1080I50;
                    if ((Vpid&0xCFFF) == 0x4585)
                        *pVidStd = DT_VIDSTD_1080PSF25;
                    break;
                case DTFWB_SDIDES_RATE_29_97:
                    *pVidStd = DT_VIDSTD_1080I59_94;
                    if ((Vpid&0xCFFF) == 0x4685)
                        *pVidStd = DT_VIDSTD_1080PSF29_97;
                    break;
                case DTFWB_SDIDES_RATE_30:
                    *pVidStd = DT_VIDSTD_1080I60;
                    if ((Vpid&0xCFFF) == 0x4785)
                        *pVidStd = DT_VIDSTD_1080PSF30;
                    break;
                }
                break;
            case DTFWB_SDIDES_VIDFRMT_1080P:
                switch (pSdiAvRx->m_VideoStatus.m_PictureRate)
                {
                case DTFWB_SDIDES_RATE_23_98:
                    *pVidStd = DT_VIDSTD_1080P23_98;
                    break;
                case DTFWB_SDIDES_RATE_24:
                    *pVidStd = DT_VIDSTD_1080P24;
                    break;
                case DTFWB_SDIDES_RATE_25:
                    *pVidStd = DT_VIDSTD_1080P25;
                    break;
                case DTFWB_SDIDES_RATE_29_97:
                    *pVidStd = DT_VIDSTD_1080P29_97;
                    break;
                case DTFWB_SDIDES_RATE_30:
                    *pVidStd = DT_VIDSTD_1080P30;
                    break;
                }
                break;
            }
            break;

        case DTFWB_SDIDES_SDISTD_3GLVLB:
            if (pSdiAvRx->m_VideoStatus.m_VideoFormat != DTFWB_SDIDES_VIDFRMT_1080I)
                break;
            switch (pSdiAvRx->m_VideoStatus.m_PictureRate)
            {
            case DTFWB_SDIDES_RATE_25:
                *pVidStd = DT_VIDSTD_1080P50B;
                break;
            case DTFWB_SDIDES_RATE_29_97:
                *pVidStd = DT_VIDSTD_1080P59_94B;
                break;
            case DTFWB_SDIDES_RATE_30:
                *pVidStd = DT_VIDSTD_1080P60B;
                break;
            }
            break;
        case DTFWB_SDIDES_SDISTD_3GLVLA:
            if (pSdiAvRx->m_VideoStatus.m_VideoFormat != DTFWB_SDIDES_VIDFRMT_1080P)
                break;
            switch (pSdiAvRx->m_VideoStatus.m_PictureRate)
            {
            case DTFWB_SDIDES_RATE_50:
                *pVidStd = DT_VIDSTD_1080P50;
                break;
            case DTFWB_SDIDES_RATE_59_94:
                *pVidStd = DT_VIDSTD_1080P59_94;
                break;
            case DTFWB_SDIDES_RATE_60:
                *pVidStd = DT_VIDSTD_1080P60;
                break;
            }
            break;
        }

        if (pSdiAvRx->m_VideoStatus.m_IsVpidValid && pVpid!=NULL)
            *pVpid = pSdiAvRx->m_VideoStatus.m_Vpid;
        if (pSdiAvRx->m_VideoStatus.m_IsVpidBValid && pVpid2!=NULL)
            *pVpid2 = pSdiAvRx->m_VideoStatus.m_VpidB;
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpSdiAvRxGetAudioStatus2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpSdiAvRxGetAudioStatus2(
    DtaNonIpPort*  pNonIpPort,
    DtaIoctlNonIpCmdGetAudioStatus2Output*  pOut)
{
    DtaSdiAvRxPort*  pSdiAvRx = &pNonIpPort->m_SdiAvRx;
    Int  i;
    const Int  MaxAudioStatus = 
                   (sizeof(pOut->m_AudioChanStatus) / sizeof(pOut->m_AudioChanStatus[0]));

    DtSpinLockAcquire(&pSdiAvRx->m_StateLock);
    pOut->m_NumAudioChannels = 0;
    for (i=0; i<pSdiAvRx->m_NumAudioExtractors; i++)
    {
        if (!pSdiAvRx->m_AudioStatus[i].m_IsAcpPresent)
            continue;
        if (pSdiAvRx->m_AudioStatus[i].m_IsChan1Active)
        {
            pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_ChanIdx =
                                            pSdiAvRx->m_AudioStatus[i].m_ChanPairSelect*2;
            pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_IsAsynchronous =
                                              pSdiAvRx->m_AudioStatus[i].m_IsAsynchronous;
            switch (pSdiAvRx->m_AudioStatus[i].m_Rate)
            {
            case 0:
                pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_Rate = 48000;
                break;
            case 1:
                pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_Rate = 44100;
                break;
            case 2:
                pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_Rate = 32000;
                break;
            default:
                pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_Rate = -1;
                break;
            }
            pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_Content =
                                                     pSdiAvRx->m_AudioStatus[i].m_Content;
            pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_ChanStatusNumValid = 24;
            DtMemCopy(pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_ChanStatusData,
                                    pSdiAvRx->m_AudioStatus[i].m_Chan1StatusData,
                                    sizeof(pSdiAvRx->m_AudioStatus[i].m_Chan1StatusData));
            pOut->m_NumAudioChannels++;
            if (pOut->m_NumAudioChannels == MaxAudioStatus)
                break;
        }
        if (pSdiAvRx->m_AudioStatus[i].m_IsChan2Active)
        {
            pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_ChanIdx =
                                          pSdiAvRx->m_AudioStatus[i].m_ChanPairSelect*2+1;
            pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_IsAsynchronous =
                                              pSdiAvRx->m_AudioStatus[i].m_IsAsynchronous;
            switch (pSdiAvRx->m_AudioStatus[i].m_Rate)
            {
            case 0:
                pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_Rate = 48000;
                break;
            case 1:
                pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_Rate = 44100;
                break;
            case 2:
                pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_Rate = 32000;
                break;
            default:
                pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_Rate = -1;
                break;
            }
            pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_Content =
                                                     pSdiAvRx->m_AudioStatus[i].m_Content;

            pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_ChanStatusNumValid = 24;
            DtMemCopy(pOut->m_AudioChanStatus[pOut->m_NumAudioChannels].m_ChanStatusData,
                                    pSdiAvRx->m_AudioStatus[i].m_Chan2StatusData,
                                    sizeof(pSdiAvRx->m_AudioStatus[i].m_Chan2StatusData));
            pOut->m_NumAudioChannels++;
            if (pOut->m_NumAudioChannels == MaxAudioStatus)
                break;
        }
    }
    DtSpinLockRelease(&pSdiAvRx->m_StateLock);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSdiAvRxGetAudSelect -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus DtaSdiAvRxGetAudSelect(DtaNonIpPort*  pNonIpPort,
                                               DtaIoctSdiAvRxCmdGetAudSelectOutput* pData)
{
    DtaIoctSdiAvRxCmdGetAudSelect2Output Out2;
    DtStatus  Status = DtaSdiAvRxGetAudSelect2(pNonIpPort, &Out2);

    // Copy data
    pData->m_NumChanPairs = Out2.m_NumChanPairs;
    memcpy(pData->m_ChanPairSelect, Out2.m_ChanPairSelect, 
                                                         sizeof(pData->m_ChanPairSelect));
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSdiAvRxGetAudSelect2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus DtaSdiAvRxGetAudSelect2(DtaNonIpPort*  pNonIpPort,
                                              DtaIoctSdiAvRxCmdGetAudSelect2Output* pData)
{
    Int i;
    DtStatus  Status = DT_STATUS_OK;
    DtaSdiAvRxPort* pSdiAvRx = &pNonIpPort->m_SdiAvRx;
    volatile UInt8* pFwbRegs = pNonIpPort->m_pFwbRegs;

    DtSpinLockAcquire(&pSdiAvRx->m_StateLock);
    pData->m_NumChanPairs = pSdiAvRx->m_NumAudioExtractors;

    // Get the current audio selection setting
    for (i=0; i<pSdiAvRx->m_NumAudioExtractors; i++)
    { 
        pData->m_ChanPairSelect[i] = DtaFwbRegRead(pFwbRegs, 
                                &FwbSdiAvReceiver.AudioExtract[i].Control_ChanPairSelect);
        // Get the audio extraction mode
        if (pSdiAvRx->m_SuppAudioExtMode)
            pData->m_ChanExtractMode[i] = DtaFwbRegRead(pFwbRegs, 
                               &FwbSdiAvReceiver.AudioExtract[i].Control_ChanExtractMode);
        else
            pData->m_ChanExtractMode[i] = DTFWB_AUDEXT_EXTRACT_BOTH;

    }
    DtSpinLockRelease(&pSdiAvRx->m_StateLock);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSdiAvRxGetAudSelect -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus DtaSdiAvRxSetAudSelect(DtaNonIpPort*  pNonIpPort,
                                                DtaIoctSdiAvRxCmdSetAudSelectInput* pData)
{
    DtaIoctSdiAvRxCmdSetAudSelect2Input In2;
    Int i;
    
    // Copy data and set default extraction mode
    In2.m_NumChanPairs = pData->m_NumChanPairs;
    memcpy(In2.m_ChanPairSelect, pData->m_ChanPairSelect,sizeof(pData->m_ChanPairSelect));
    for (i=0; i<DTA_SDIAVRX_MAX_NUM_AUDPAIRS; i++)
        In2.m_ChanExtractMode[i] = DTFWB_AUDEXT_EXTRACT_BOTH; 

    return DtaSdiAvRxSetAudSelect2(pNonIpPort, &In2);

}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSdiAvRxGetAudSelect2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus DtaSdiAvRxSetAudSelect2(DtaNonIpPort*  pNonIpPort,
                                               DtaIoctSdiAvRxCmdSetAudSelect2Input* pData)
{
    Int i;
    DtaSdiAvRxPort* pSdiAvRx = &pNonIpPort->m_SdiAvRx;
    volatile UInt8* pFwbRegs = pNonIpPort->m_pFwbRegs;

   // Check whether selection is valid
   if (pData->m_NumChanPairs > pSdiAvRx->m_NumAudioExtractors)
        return DT_STATUS_INVALID_PARAMETER;  
   for (i=0; i<pData->m_NumChanPairs; i++)
       if (pData->m_ChanPairSelect[i] > 31)
                   return DT_STATUS_INVALID_PARAMETER;

    DtSpinLockAcquire(&pSdiAvRx->m_StateLock);
    pData->m_NumChanPairs = pSdiAvRx->m_NumAudioExtractors;

    // Set the audio selection setting
    for (i=0; i<pData->m_NumChanPairs; i++)
    {
        if (pData->m_ChanPairSelect[i] != DtaFwbRegRead(pFwbRegs, 
                                &FwbSdiAvReceiver.AudioExtract[i].Control_ChanPairSelect))
        {
            // Change selection
            DtaFwbRegWrite(pFwbRegs, 
                                &FwbSdiAvReceiver.AudioExtract[i].Control_ChanPairSelect, 
                                pData->m_ChanPairSelect[i]);
            // Clear status
            DtaSdiAvRxClearAudStatus(pSdiAvRx, i);
        }

        // Set the audio extraction mode
        if (pSdiAvRx->m_SuppAudioExtMode)
            DtaFwbRegWrite(pFwbRegs, 
                              &FwbSdiAvReceiver.AudioExtract[i].Control_ChanExtractMode,
                              pData->m_ChanExtractMode[i]);
    }

    // Set remaining to their default
    for (; i<pSdiAvRx->m_NumAudioExtractors; i++)
    {
        if (i != DtaFwbRegRead(pFwbRegs, 
                                &FwbSdiAvReceiver.AudioExtract[i].Control_ChanPairSelect))
        {
            // Change selection
            DtaFwbRegWrite(pFwbRegs, 
                             &FwbSdiAvReceiver.AudioExtract[i].Control_ChanPairSelect, i);
            // Clear status
            DtaSdiAvRxClearAudStatus(pSdiAvRx, i);
        }
        // Set the audio extraction mode
        if (pSdiAvRx->m_SuppAudioExtMode)
            DtaFwbRegWrite(pFwbRegs, 
                              &FwbSdiAvReceiver.AudioExtract[i].Control_ChanExtractMode,
                              DTFWB_AUDEXT_EXTRACT_BOTH);
    }

    // All status data should be updated again
    pSdiAvRx->m_UpdateAll = 1;

    DtSpinLockRelease(&pSdiAvRx->m_StateLock);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSdiAvRxReadSmpte2020 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaSdiAvRxReadSmpte2020(DtaNonIpPort*  pNonIpPort,
                                              DtaIoctSdiAvRxCmdReadSmpte2020Output* pData)
{
    DtaSdiAvRxPort* pSdiAvRx = &pNonIpPort->m_SdiAvRx;
    DtaSdiAvRxAncExStatus* pAncEx = &pSdiAvRx->m_AncEx;

    DtSpinLockAcquire(&pAncEx->m_Lock);
    pData->m_BufSize = pAncEx->m_BufLoad;
    DtMemCopy(pData->m_Buf, pAncEx->m_Buf, pAncEx->m_BufLoad);
    DtSpinLockRelease(&pAncEx->m_Lock);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSdiAvRxReadAvStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtaSdiAvRxReadAvStatus(DtaNonIpPort*  pNonIpPort, Int Force)
{
    Int i ,j;
    DtaSdiAvRxPort* pSdiAvRx = &pNonIpPort->m_SdiAvRx;
    volatile UInt8* pFwbRegs = pNonIpPort->m_pFwbRegs;

    // New video status?
    if (Force || DtaFwbRegRead(pFwbRegs,  &FwbSdiAvReceiver.Status_IsVideoStatusNew))
    {
        // Clear new status
        DtaFwbRegClear(pFwbRegs,  &FwbSdiAvReceiver.Status_IsVideoStatusNew);
        // Read SDI standard
        pSdiAvRx->m_SdiStandard = DtaFwbRegRead(pFwbRegs, 
                                &FwbSdiAvReceiver.SdiDeserializer.SdiStatus_Standard);
        // Read video status
        pSdiAvRx->m_VideoStatus.m_VideoFormat = DtaFwbRegRead(pFwbRegs, 
                                &FwbSdiAvReceiver.SdiDeserializer.SdiStatus_VidFormat);
        pSdiAvRx->m_VideoStatus.m_PictureRate = DtaFwbRegRead(pFwbRegs, 
                                &FwbSdiAvReceiver.SdiDeserializer.SdiStatus_PictRate);
        pSdiAvRx->m_VideoStatus.m_IsSdiValid = DtaFwbRegRead(pFwbRegs, 
                                &FwbSdiAvReceiver.SdiDeserializer.SdiStatus_IsSdiValid);
        pSdiAvRx->m_VideoStatus.m_IsTrsLocked = DtaFwbRegRead(pFwbRegs, 
                                &FwbSdiAvReceiver.SdiDeserializer.SdiStatus_IsTrsLocked);
        pSdiAvRx->m_VideoStatus.m_IsVpidValid = DtaFwbRegRead(pFwbRegs, 
                                &FwbSdiAvReceiver.SdiDeserializer.SdiStatus_IsVpidValid);
        pSdiAvRx->m_VideoStatus.m_IsVpidBValid = DtaFwbRegRead(pFwbRegs, 
                                &FwbSdiAvReceiver.SdiDeserializer.SdiStatus_IsVpidBValid);
        pSdiAvRx->m_VideoStatus.m_Vpid = DtaFwbRegRead(pFwbRegs, 
                                &FwbSdiAvReceiver.SdiDeserializer.VideoPayloadId);
        pSdiAvRx->m_VideoStatus.m_VpidB = DtaFwbRegRead(pFwbRegs, 
                                &FwbSdiAvReceiver.SdiDeserializer.VideoPayloadIdB);
        pSdiAvRx->m_VideoStatus.m_CrcErrorCount = DtaFwbRegRead(pFwbRegs,
                                &FwbSdiAvReceiver.SdiDeserializer.CrcErrorCount);
    }

    // Copy Audio Presence
    pSdiAvRx->m_AudioPresence = DtaFwbRegRead(pFwbRegs, &FwbSdiAvReceiver.AudioPresence);

    // New audio status?
    if (Force || DtaFwbRegRead(pFwbRegs, &FwbSdiAvReceiver.Status_IsAudioStatusNew))
    {
        // Check for all audio extractors
        DT_ASSERT(pSdiAvRx->m_NumAudioExtractors <= DTA_SDIAVRX_MAX_NUM_AUDPAIRS);
        for (i=0; i<pSdiAvRx->m_NumAudioExtractors; i++)
        {
            // This audio extractor reported a change
            if (pSdiAvRx->m_UpdateAll || DtaFwbRegRead(pFwbRegs, 
                                &FwbSdiAvReceiver.AudioExtract[i].Status_IsStatusNew))
            {
                // Clear new status flag
                DtaFwbRegClear(pFwbRegs, 
                                &FwbSdiAvReceiver.AudioExtract[i].Status_IsStatusNew);

                // Read audio status                   
                pSdiAvRx->m_AudioStatus[i].m_ChanPairSelect = DtaFwbRegRead(pFwbRegs, 
                                &FwbSdiAvReceiver.AudioExtract[i].Control_ChanPairSelect);
                pSdiAvRx->m_AudioStatus[i].m_IsAcpPresent = DtaFwbRegRead(pFwbRegs, 
                                   &FwbSdiAvReceiver.AudioExtract[i].Status_IsAcpPresent);
                pSdiAvRx->m_AudioStatus[i].m_IsAsynchronous = DtaFwbRegRead(pFwbRegs, 
                                 &FwbSdiAvReceiver.AudioExtract[i].Status_IsAsynchronous);
                pSdiAvRx->m_AudioStatus[i].m_Rate = DtaFwbRegRead(pFwbRegs, 
                                           &FwbSdiAvReceiver.AudioExtract[i].Status_Rate);
                // Get the audio extraction content
                if (pSdiAvRx->m_SuppAudioExtMode)
                    pSdiAvRx->m_AudioStatus[i].m_Content = DtaFwbRegRead(pFwbRegs, 
                                  &FwbSdiAvReceiver.AudioExtract[i].Status_ChanPcmOrData);
                else 
                    pSdiAvRx->m_AudioStatus[i].m_Content = DTFWB_AUDEXT_CONTENT_UNSUP;
                pSdiAvRx->m_AudioStatus[i].m_IsChan1Active = DtaFwbRegRead(pFwbRegs, 
                                  &FwbSdiAvReceiver.AudioExtract[i].Status_IsChan1Active);
                pSdiAvRx->m_AudioStatus[i].m_IsChan2Active = DtaFwbRegRead(pFwbRegs, 
                                  &FwbSdiAvReceiver.AudioExtract[i].Status_IsChan2Active);
                for (j=0; j<6; j++)
                    pSdiAvRx->m_AudioStatus[i].m_Chan1StatusData[j] = 
                            DtaFwbRegRead(pFwbRegs, 
                                        &FwbSdiAvReceiver.AudioExtract[i].ChanStatusData);
                for (j=0; j<6; j++)
                    pSdiAvRx->m_AudioStatus[i].m_Chan2StatusData[j] = 
                            DtaFwbRegRead(pFwbRegs, 
                                        &FwbSdiAvReceiver.AudioExtract[i].ChanStatusData);
            }
        }
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSdiAvRxClearAvStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtaSdiAvRxClearAvStatus(DtaSdiAvRxPort* pSdiAvRx)
{
    Int i;

    // Clear video status
    DtaSdiAvRxClearVidStatus(pSdiAvRx);

    // Check the number audio extractors
    DT_ASSERT(pSdiAvRx->m_NumAudioExtractors <= DTA_SDIAVRX_MAX_NUM_AUDPAIRS);

    pSdiAvRx->m_AudioPresence = 0;

    for (i=0; i<pSdiAvRx->m_NumAudioExtractors; i++)
    {
        // Clear audio status
        DtaSdiAvRxClearAudStatus(pSdiAvRx, i);     
    }
    
    // Clear ANC extract FIFO
    pSdiAvRx->m_AncEx.m_BufLoad = 0;

    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSdiAvRxClearVidStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtaSdiAvRxClearVidStatus(DtaSdiAvRxPort* pSdiAvRx)
{
    // Clear video status
    pSdiAvRx->m_VideoStatus.m_VideoFormat = DTFWB_SDIDES_VIDFRMT_UNDEF;
    pSdiAvRx->m_VideoStatus.m_PictureRate = DTFWB_SDIDES_RATE_UNDEF;
    pSdiAvRx->m_VideoStatus.m_IsSdiValid = 0;
    pSdiAvRx->m_VideoStatus.m_IsTrsLocked = 0;
    pSdiAvRx->m_VideoStatus.m_IsVpidValid = 0;
    pSdiAvRx->m_VideoStatus.m_IsVpidBValid = 0;
    pSdiAvRx->m_VideoStatus.m_Vpid = 0;
    pSdiAvRx->m_VideoStatus.m_VpidB = 0;
    pSdiAvRx->m_VideoStatus.m_CrcErrorCount = 0;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSdiAvRxClearAudStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtaSdiAvRxClearAudStatus(DtaSdiAvRxPort* pSdiAvRx, Int ChpIdx)
{
    Int j;

    // Check extractor
    if (ChpIdx >= pSdiAvRx->m_NumAudioExtractors)
        return DT_STATUS_INVALID_PARAMETER;

    // Clear audio status                   
    pSdiAvRx->m_AudioStatus[ChpIdx].m_IsAcpPresent = 0;
    pSdiAvRx->m_AudioStatus[ChpIdx].m_IsAsynchronous = 0;
    pSdiAvRx->m_AudioStatus[ChpIdx].m_Rate = 0;
    pSdiAvRx->m_AudioStatus[ChpIdx].m_IsChan1Active = 0;
    pSdiAvRx->m_AudioStatus[ChpIdx].m_IsChan2Active = 0;
    for (j=0; j<6; j++)
        pSdiAvRx->m_AudioStatus[ChpIdx].m_Chan1StatusData[j] = 0;
    for (j=0; j<6; j++)
        pSdiAvRx->m_AudioStatus[ChpIdx].m_Chan2StatusData[j] = 0;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpSdiAvRxSetNewVidStdCb -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpSdiAvRxSetNewVidStdCb(DtaNonIpPort* pNonIpPort,
                                        pDtaEnDecNewInputVidStd Cb, DtaNonIpPort* pPortCb)
{
    DtaSdiAvRxPort*  pSdiAvRx = &pNonIpPort->m_SdiAvRx;
    pSdiAvRx->m_NewVidStdCb = Cb;
    pSdiAvRx->m_pPortCb = pPortCb;
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpSdiAvRxCopyStatus2Old -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaNonIpSdiAvRxCopyStatus2Old(DtaIoctlNonIpCmdGetAudioStatus2Output* pSrc,
                                               DtaIoctlNonIpCmdGetAudioStatusOutput* pDst)
{
    // Copy DtaIoctlNonIpCmdGetAudioStatusOutput2 to DtaIoctlNonIpCmdGetAudioStatusOutput
    Int i;
    DT_ASSERT(pDst!=NULL && pSrc!=NULL);

    pDst->m_NumAudioChannels = pSrc->m_NumAudioChannels;
    for (i=0; i < sizeof(pDst->m_AudioChanStatus)/sizeof(pDst->m_AudioChanStatus[0]); i++)
    {
        pDst->m_AudioChanStatus[i].m_ChanIdx = pSrc->m_AudioChanStatus[i].m_ChanIdx;
        pDst->m_AudioChanStatus[i].m_IsAsynchronous = 
                                              pSrc->m_AudioChanStatus[i].m_IsAsynchronous;
        pDst->m_AudioChanStatus[i].m_Rate = pSrc->m_AudioChanStatus[i].m_Rate;
        pDst->m_AudioChanStatus[i].m_ChanStatusNumValid = 
                                          pSrc->m_AudioChanStatus[i].m_ChanStatusNumValid;
        memcpy(pDst->m_AudioChanStatus[i].m_ChanStatusData,
                                     pSrc->m_AudioChanStatus[i].m_ChanStatusData,
                                     sizeof(pDst->m_AudioChanStatus[i].m_ChanStatusData));

    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpSdiAvRxSetNewVidStdCb2 -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpSdiAvRxSetNewVidStdCb2(DtaNonIpPort* pNonIpPort,
                                        pDtaEnDecNewInputVidStd Cb, DtaNonIpPort* pPortCb)
{
    DtaSdiAvRxPort*  pSdiAvRx = &pNonIpPort->m_SdiAvRx;
    pSdiAvRx->m_NewVidStdCb2 = Cb;
    pSdiAvRx->m_pPortCb2 = pPortCb;
    return DT_STATUS_OK;
}

