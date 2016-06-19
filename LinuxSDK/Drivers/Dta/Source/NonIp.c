//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NonIp.c *#*#*#*#*#*#*#*#*#* (C) 2010-2016 DekTec
//
// Dta driver - Non IP functionality - Implementation of generic non IP port functionality
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2016 DekTec Digital Video B.V.
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
DtStatus  DtaNonIpDetermineDmaRegsOffset(DtaDeviceData* pDvcData, Int PortIndex, 
                                                                   UInt*  pDmaRegsOffset);
Int  DtaNonIpGetMaxDmaBurstSize(DtaNonIpPort* pNonIpPort);
Int  DtaNonIpGetMaxFifoSize(DtaNonIpPort* pNonIpPort);
static DtStatus  DtaNonIpGetAudioStatus(DtaNonIpPort* pNonIpPort,
                                              DtaIoctlNonIpCmdGetAudioStatusOutput* pOut);
static DtStatus  DtaNonIpIoConfigSetIoDir(DtaNonIpPort* pNonIpPort,  Int Group,
                                                               DtaIoConfigValue CfgValue);
static DtStatus  DtaNonIpIoConfigSetIoStd(DtaNonIpPort* pNonIpPort,  Int Group,
                                                               DtaIoConfigValue CfgValue);
static DtStatus  DtaNonIpIoConfigSetRfClkSel(DtaNonIpPort* pNonIpPort, Int Group,
                                                               DtaIoConfigValue CfgValue);
static DtStatus  DtaNonIpIoConfigSetSpiClkSel(DtaNonIpPort* pNonIpPort, Int Group,
                                                               DtaIoConfigValue CfgValue);
static DtStatus  DtaNonIpIoConfigSetSpiMode(DtaNonIpPort* pNonIpPort, Int Group,
                                                               DtaIoConfigValue CfgValue);
static DtStatus  DtaNonIpIoConfigSetSpiStd(DtaNonIpPort* pNonIpPort,  Int Group,
                                                               DtaIoConfigValue CfgValue);
static DtStatus  DtaNonIpIoConfigSetTsRateSel(DtaNonIpPort* pNonIpPort,  Int Group,
                                                               DtaIoConfigValue CfgValue);
static DtStatus  DtaNonIpIoConfigSetSwS2Apsk(DtaNonIpPort* pNonIpPort,  Int Group,
                                                               DtaIoConfigValue CfgValue);
static DtStatus  DtaNonIpIoConfigSetFailSafe(DtaNonIpPort* pNonIpPort,  Int Group,
                                                               DtaIoConfigValue CfgValue);
static DtStatus  DtaNonIpIoConfigSetGenLocked(DtaNonIpPort* pNonIpPort,  Int Group,
                                                               DtaIoConfigValue CfgValue);
static DtStatus  DtaNonIpIoConfigSetGenRef(DtaNonIpPort* pNonIpPort, Int Group,
                                                               DtaIoConfigValue CfgValue);
static DtStatus  DtaNonIpIoConfigSetFracMode(DtaNonIpPort* pNonIpPort, Int Group,
                                                               DtaIoConfigValue CfgValue);

#ifdef WINBUILD
//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpTxEvtRequestCancelDma -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaNonIpTxEvtRequestCancelDma(WDFREQUEST WdfRequest)
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpRxEvtRequestCancelDma -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaNonIpRxEvtRequestCancelDma(WDFREQUEST WdfRequest)
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
    // Reset channel/clear fifo, RxCtrl Rx??
    // ToDo
    if (pDmaCh != NULL)
        Status = DtaDmaAbortDma(pDmaCh);
    DtDbgOut(MIN, NONIP, "Exit");
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpDmaCompletedWindows -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is executed after the DMA is completed (Windows ONLY!)
// The WdfRequest is completed here
//
void  DtaNonIpDmaCompletedWindows(DmaChannel* pDmaCh, void* pContext)
{
    WDFREQUEST  WdfRequest = pContext;
    DtStatus  Status = DT_STATUS_OK;
    UInt  BytesRead = 0;
    
    DT_ASSERT(pContext != NULL);
    
    WdfRequestUnmarkCancelable(WdfRequest);
    if (DtaDmaIsAbortActive(pDmaCh))
        Status = DT_STATUS_CANCELLED;
    
    if (DT_SUCCESS(Status))
    {
        if (pDmaCh->m_DmaDirection == DT_DMA_DIRECTION_FROM_DEVICE)
            BytesRead = pDmaCh->m_NumBytesRead;
    }
    WdfRequestCompleteWithInformation(WdfRequest, (NTSTATUS)Status, (ULONG_PTR)BytesRead);
}
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpInit(
    DtaDeviceData*  pDvcData,
    Int  PortIndex,                     // Physical port index
    DtaNonIpPort*  pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  IoConfig, ParXtra, DefIoStd=-1, OldPropertyNotFoundCounter=0;
    UInt  DmaRegsOffset;
    Bool  HasIc2RfPwrMeas;
    
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    
    // Initialize common members
    pNonIpPort->m_pDvcData = pDvcData;
    pNonIpPort->m_PortIndex = PortIndex;

    // Capabilities
    // IODIR (I/O direction) - Capabilities
    pNonIpPort->m_CapDisabled = DtPropertiesGetBool(pPropData, "CAP_DISABLED",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapInput = DtPropertiesGetBool(pPropData, "CAP_INPUT",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapIntInput = DtPropertiesGetBool(pPropData, "CAP_INTINPUT",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapMonitor = DtPropertiesGetBool(pPropData, "CAP_MONITOR",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapOutput = DtPropertiesGetBool(pPropData, "CAP_OUTPUT",
                                                                 pNonIpPort->m_PortIndex);
    // IODIR - INPUT (Uni-directional input) - Sub capabilities
    pNonIpPort->m_CapSharedAnt = DtPropertiesGetBool(pPropData, "CAP_SHAREDANT",
                                                                 pNonIpPort->m_PortIndex);
    // IODIR - OUTPUT (Uni-directional output) - Sub capabilities
    pNonIpPort->m_CapDblBuf = DtPropertiesGetBool(pPropData, "CAP_DBLBUF",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapLoopS2L3 = DtPropertiesGetBool(pPropData, "CAP_LOOPS2L3",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapLoopS2Ts = DtPropertiesGetBool(pPropData, "CAP_LOOPS2TS",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapLoopThr = DtPropertiesGetBool(pPropData, "CAP_LOOPTHR",
                                                                 pNonIpPort->m_PortIndex);
    // IOPROPS (I/O properties) - Capabilities
    pNonIpPort->m_CapMatrix = DtPropertiesGetBool(pPropData, "CAP_MATRIX",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapMatrix2 = DtPropertiesGetBool(pPropData, "CAP_MATRIX2",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapVirtual = DtPropertiesGetBool(pPropData, "CAP_VIRTUAL",
                                                                 pNonIpPort->m_PortIndex);
    // IOSTD (I/O standard) - Capabilities
    pNonIpPort->m_Cap3GSdi = DtPropertiesGetBool(pPropData, "CAP_3GSDI",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapAsi = DtPropertiesGetBool(pPropData, "CAP_ASI",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapAvEnc = DtPropertiesGetBool(pPropData, "CAP_AVENC",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapDemod = DtPropertiesGetBool(pPropData, "CAP_DEMOD",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapGpsTime = DtPropertiesGetBool(pPropData, "CAP_GPSTIME",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapHdmi = DtPropertiesGetBool(pPropData, "CAP_HDMI",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapHdSdi = DtPropertiesGetBool(pPropData, "CAP_HDSDI",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapIfAdc = DtPropertiesGetBool(pPropData, "CAP_IFADC",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapIp = DtPropertiesGetBool(pPropData, "CAP_IP",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapMod = DtPropertiesGetBool(pPropData, "CAP_MOD",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapPhaseNoise = DtPropertiesGetBool(pPropData, "CAP_PHASENOISE",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapRs422 = DtPropertiesGetBool(pPropData, "CAP_RS422",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSdi = DtPropertiesGetBool(pPropData, "CAP_SDI",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSdiRx = DtPropertiesGetBool(pPropData, "CAP_SDIRX",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpi = DtPropertiesGetBool(pPropData, "CAP_SPI",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpiSdi = DtPropertiesGetBool(pPropData, "CAP_SPISDI",
                                                                 pNonIpPort->m_PortIndex);
    // IOSTD - SDI (SD-SDI) - Sub capabilities
    pNonIpPort->m_Cap525I59_94 = DtPropertiesGetBool(pPropData, "CAP_525I59_94",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap625I50 = DtPropertiesGetBool(pPropData, "CAP_625I50",
                                                                 pNonIpPort->m_PortIndex);
    // IOSTD - SDISDI - Sub capabilities
    pNonIpPort->m_CapSpi525I59_94 = DtPropertiesGetBool(pPropData, "CAP_SPI525I59_94",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpi625I50 = DtPropertiesGetBool(pPropData, "CAP_SPI625I50",
                                                                 pNonIpPort->m_PortIndex);
    // IOSTD - SDI (HD-SDI) - Sub capabilities
    pNonIpPort->m_Cap1080I50 = DtPropertiesGetBool(pPropData, "CAP_1080I50",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080I59_94 = DtPropertiesGetBool(pPropData, "CAP_1080I59_94",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080I60 = DtPropertiesGetBool(pPropData, "CAP_1080I60",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080P23_98 = DtPropertiesGetBool(pPropData, "CAP_1080P23_98",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080P24 = DtPropertiesGetBool(pPropData, "CAP_1080P24",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080P25 = DtPropertiesGetBool(pPropData, "CAP_1080P25",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080P29_97 = DtPropertiesGetBool(pPropData, "CAP_1080P29_97",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080P30 = DtPropertiesGetBool(pPropData, "CAP_1080P30",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080Psf23_98 = DtPropertiesGetBool(pPropData, "CAP_1080PSF23_98",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080Psf24 = DtPropertiesGetBool(pPropData, "CAP_1080PSF24",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080Psf25 = DtPropertiesGetBool(pPropData, "CAP_1080PSF25",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080Psf29_97 = DtPropertiesGetBool(pPropData, "CAP_1080PSF29_97",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080Psf30 = DtPropertiesGetBool(pPropData, "CAP_1080PSF30",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap720P23_98 = DtPropertiesGetBool(pPropData, "CAP_720P23_98",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap720P24 = DtPropertiesGetBool(pPropData, "CAP_720P24",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap720P25 = DtPropertiesGetBool(pPropData, "CAP_720P25",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap720P29_97 = DtPropertiesGetBool(pPropData, "CAP_720P29_97",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap720P30 = DtPropertiesGetBool(pPropData, "CAP_720P30",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap720P50 = DtPropertiesGetBool(pPropData, "CAP_720P50",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap720P59_94 = DtPropertiesGetBool(pPropData, "CAP_720P59_94",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap720P60 = DtPropertiesGetBool(pPropData, "CAP_720P60",
                                                                 pNonIpPort->m_PortIndex);
    // IOSTD - SDI (3G-SDI) - Sub capabilities
    pNonIpPort->m_Cap1080P50 = DtPropertiesGetBool(pPropData, "CAP_1080P50",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080P50B = DtPropertiesGetBool(pPropData, "CAP_1080P50B",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080P59_94 = DtPropertiesGetBool(pPropData, "CAP_1080P59_94",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080P59_94B = DtPropertiesGetBool(pPropData, "CAP_1080P59_94B",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080P60 = DtPropertiesGetBool(pPropData, "CAP_1080P60",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_Cap1080P60B = DtPropertiesGetBool(pPropData, "CAP_1080P60B",
                                                                 pNonIpPort->m_PortIndex);
    // RFCLKSEL (RF clock source selection) - Capabilities
    pNonIpPort->m_CapRfClkExt = DtPropertiesGetBool(pPropData, "CAP_RFCLKEXT",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapRfClkInt = DtPropertiesGetBool(pPropData, "CAP_RFCLKINT",
                                                                 pNonIpPort->m_PortIndex);
    // SPICLKSEL (Parallel port clock source selection) - Capabilities
    pNonIpPort->m_CapSpiClkExt = DtPropertiesGetBool(pPropData, "CAP_SPICLKEXT",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpiClkInt = DtPropertiesGetBool(pPropData, "CAP_SPICLKINT",
                                                                 pNonIpPort->m_PortIndex);
    // SPIMODE (Parallel port mode) - Capabilities
    pNonIpPort->m_CapSpiFixedClk = DtPropertiesGetBool(pPropData, "CAP_SPIFIXEDCLK",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpiDvbMode = DtPropertiesGetBool(pPropData, "CAP_SPIDVBMODE",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpiSer8B = DtPropertiesGetBool(pPropData, "CAP_SPISER8B",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpiSer10B = DtPropertiesGetBool(pPropData, "CAP_SPISER10B",
                                                                 pNonIpPort->m_PortIndex);
    // SPISTD (Parallel port I/O standard) - Capabilities
    pNonIpPort->m_CapSpiLvds1 = DtPropertiesGetBool(pPropData, "CAP_SPILVDS1",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpiLvds2 = DtPropertiesGetBool(pPropData, "CAP_SPILVDS2",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSpiLvttl = DtPropertiesGetBool(pPropData, "CAP_SPILVTTL",
                                                                 pNonIpPort->m_PortIndex);
    // TSRATESEL (Transport-stream rate selection) - Capabilities
    pNonIpPort->m_CapExtTsRate = DtPropertiesGetBool(pPropData, "CAP_EXTTSRATE",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapExtRatio = DtPropertiesGetBool(pPropData, "CAP_EXTRATIO",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapIntTsRate = DtPropertiesGetBool(pPropData, "CAP_INTTSRATE",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapLock2Inp = DtPropertiesGetBool(pPropData, "CAP_LOCK2INP",
                                                                 pNonIpPort->m_PortIndex);
    // BOOLIO (Boolean I/O capabilities) - Capabilities
    pNonIpPort->m_CapFailSafe = DtPropertiesGetBool(pPropData, "CAP_FAILSAFE",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapFracMode = DtPropertiesGetBool(pPropData, "CAP_FRACMODE",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapGenLocked = DtPropertiesGetBool(pPropData, "CAP_GENLOCKED",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapGenRef = DtPropertiesGetBool(pPropData, "CAP_GENREF",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapGenRefSlave = DtPropertiesGetBool(pPropData, "CAP_GENREFSLAVE",
                                                                 pNonIpPort->m_PortIndex);
    pNonIpPort->m_CapSwS2Apsk = DtPropertiesGetBool(pPropData, "CAP_SWS2APSK",
                                                                 pNonIpPort->m_PortIndex);

    // Implementation details properties

    // Get ASI/SDI deserialiser interface type
    pNonIpPort->m_AsiSdiDeserItfType = ASI_SDI_DESER_ITF_NONE;
    if (pNonIpPort->m_CapInput && (pNonIpPort->m_Cap3GSdi || pNonIpPort->m_CapAsi 
                                       || pNonIpPort->m_CapHdSdi || pNonIpPort->m_CapSdi))
    {
        pNonIpPort->m_AsiSdiDeserItfType = DtPropertiesGetInt(pPropData, 
                                         "ASI_SDI_HW_DESER_ITF", pNonIpPort->m_PortIndex);
    }
    
    // Get ASI/SDI serialiser interface type
    pNonIpPort->m_AsiSdiSerItfType = ASI_SDI_SER_ITF_NONE;
    if (pNonIpPort->m_CapOutput && (pNonIpPort->m_Cap3GSdi || pNonIpPort->m_CapAsi 
                                       || pNonIpPort->m_CapHdSdi || pNonIpPort->m_CapSdi))
    {
        pNonIpPort->m_AsiSdiSerItfType = DtPropertiesGetInt(pPropData, 
                                           "ASI_SDI_HW_SER_ITF", pNonIpPort->m_PortIndex);
    }
    // Get pipeline delays for relevant interfaces
    pNonIpPort->m_AsiSdiSerDelayNsSd = pNonIpPort->m_AsiSdiSerDelayNsHd = 
                                                     pNonIpPort->m_AsiSdiSerDelayNs3g = 0;
    if (pNonIpPort->m_AsiSdiSerItfType==ASI_SDI_SER_ITF_FPGA_GS3490
                          || pNonIpPort->m_AsiSdiSerItfType==ASI_SDI_SER_ITF_GS2962
                          || pNonIpPort->m_AsiSdiSerItfType==ASI_SDI_SER_ITF_FPGA_LMH0387)
    {
        if (pNonIpPort->m_CapSdi)
        {
            pNonIpPort->m_AsiSdiSerDelayNsSd = DtPropertiesGetInt(pPropData, 
                                  "ASI_SDI_HW_SER_ITF_DELAY_SD", pNonIpPort->m_PortIndex);
            DT_ASSERT(pNonIpPort->m_AsiSdiSerDelayNsSd >= 0);
        }
        if (pNonIpPort->m_CapHdSdi)
        {
            pNonIpPort->m_AsiSdiSerDelayNsHd = DtPropertiesGetInt(pPropData, 
                                  "ASI_SDI_HW_SER_ITF_DELAY_HD", pNonIpPort->m_PortIndex);
            DT_ASSERT(pNonIpPort->m_AsiSdiSerDelayNsHd >= 0);
        }
        if (pNonIpPort->m_Cap3GSdi)
        {
            pNonIpPort->m_AsiSdiSerDelayNs3g = DtPropertiesGetInt(pPropData, 
                                  "ASI_SDI_HW_SER_ITF_DELAY_3G", pNonIpPort->m_PortIndex);
            DT_ASSERT(pNonIpPort->m_AsiSdiSerDelayNs3g >= 0);
        }
    }

    // A non-functional port has no input and output capability
    pNonIpPort->m_IsNonFuntional = (!pNonIpPort->m_CapInput && !pNonIpPort->m_CapOutput);
    // Is it a bidir port?
    pNonIpPort->m_IsBidir = (pNonIpPort->m_CapInput && pNonIpPort->m_CapOutput);
    



    // Initial default IO configurations
    // Initialze all parxtra to -1 as default value to prevent SetIoConfig from detecting
    // ParXtra changes while parxtra is not used in the current IO configuration
    for (IoConfig=0; IoConfig<DT_IOCONFIG_COUNT; IoConfig++)
    {
        pNonIpPort->m_IoCfg[IoConfig].m_Value = DT_IOCONFIG_NONE;
        pNonIpPort->m_IoCfg[IoConfig].m_SubValue = DT_IOCONFIG_NONE;
        for (ParXtra=0; ParXtra<DT_MAX_PARXTRA_COUNT; ParXtra++)
        {
            pNonIpPort->m_IoCfg[IoConfig].m_ParXtra[ParXtra] = -1;
        }
    }
    // DTA_IOCONFIG_IOCONFIG
    DT_ASSERT(pNonIpPort->m_IsNonFuntional || pNonIpPort->m_CapInput
                                  || pNonIpPort->m_CapOutput 
                                  || pNonIpPort->m_CapDblBuf || pNonIpPort->m_CapLoopThr);
    
    // If the port is not a configurable input or output port,
    // we don't have a DEFAULT_IOCONFIG property.
    if (pNonIpPort->m_CapDblBuf || pNonIpPort->m_CapLoopThr || pNonIpPort->m_IsBidir)
    {
        Int ParXtra = -1;
        Int IodirVal = DtPropertiesGetInt(pPropData, "DEFAULT_IODIR", 
                                                                 pNonIpPort->m_PortIndex);
        switch (IodirVal)
        {
        case DT_IOCONFIG_DISABLED:
            DT_ASSERT(pNonIpPort->m_CapDisabled);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_DISABLED;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_DISABLED;
            break;
        case DT_IOCONFIG_INPUT:
            DT_ASSERT(pNonIpPort->m_CapInput);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_INPUT;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_INPUT;
            break;
        case DT_IOCONFIG_INTINPUT:
            DT_ASSERT(pNonIpPort->m_CapInput);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_INTINPUT;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_INTINPUT;
            break;
        case DT_IOCONFIG_MONITOR:
            DT_ASSERT(pNonIpPort->m_CapInput);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_MONITOR;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_MONITOR;
            ParXtra = DtPropertiesGetInt(pPropData, "DEFAULT_PARXTRA0", 
                                                                 pNonIpPort->m_PortIndex);
            DT_ASSERT(ParXtra!=-1);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            break;
        case DT_IOCONFIG_SHAREDANT:
            DT_ASSERT(pNonIpPort->m_CapSharedAnt);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_INPUT;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_SHAREDANT;
            ParXtra = DtPropertiesGetInt(pPropData, "DEFAULT_PARXTRA0", 
                                                                 pNonIpPort->m_PortIndex);
            DT_ASSERT(ParXtra!=-1);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            break;
        case DT_IOCONFIG_OUTPUT:
            DT_ASSERT(pNonIpPort->m_CapOutput);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_OUTPUT;
            break;
        case DT_IOCONFIG_DBLBUF:
            DT_ASSERT(pNonIpPort->m_CapDblBuf);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_DBLBUF;
            ParXtra = DtPropertiesGetInt(pPropData, "DEFAULT_PARXTRA0", 
                                                                 pNonIpPort->m_PortIndex);
            DT_ASSERT(ParXtra!=-1);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            break;
        case DT_IOCONFIG_LOOPS2L3:
            DT_ASSERT(pNonIpPort->m_CapLoopS2L3);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_LOOPS2L3;
            ParXtra = DtPropertiesGetInt(pPropData, "DEFAULT_PARXTRA0", 
                                                                 pNonIpPort->m_PortIndex);
            DT_ASSERT(ParXtra!=-1);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            break;
        case DT_IOCONFIG_LOOPS2TS:
            DT_ASSERT(pNonIpPort->m_CapLoopS2Ts);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_LOOPS2TS;
            ParXtra = DtPropertiesGetInt(pPropData, "DEFAULT_PARXTRA0", 
                                                                 pNonIpPort->m_PortIndex);
            DT_ASSERT(ParXtra!=-1);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            ParXtra = DtPropertiesGetInt(pPropData, "DEFAULT_PARXTRA1", 
                                                                 pNonIpPort->m_PortIndex);
            DT_ASSERT(ParXtra!=-1);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_ParXtra[1] = ParXtra;
            break;
        case DT_IOCONFIG_LOOPTHR:
            DT_ASSERT(pNonIpPort->m_CapLoopThr);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_LOOPTHR;
            ParXtra = DtPropertiesGetInt(pPropData, "DEFAULT_PARXTRA0", 
                                                                 pNonIpPort->m_PortIndex);
            DT_ASSERT(ParXtra!=-1);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
            break;
        default:
            DtDbgOut(ERR, NONIP, "Unsupported DEFAULT_IODIR for board type: %d port: %d",
                                      pPropData->m_TypeNumber, pNonIpPort->m_PortIndex+1);

            DT_ASSERT(FALSE);
            break;
        }
    }
    else if (pNonIpPort->m_CapInput)
    {
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_INPUT;
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_INPUT;
    }
    else if (pNonIpPort->m_CapOutput)
    {
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_OUTPUT;
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_OUTPUT;
    }
    else if (pNonIpPort->m_CapDisabled)
    {
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_DISABLED;
    }
    else if (pNonIpPort->m_CapIntInput)
    {
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_INTINPUT;
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_INTINPUT;
    }
    else if (pNonIpPort->m_CapMonitor)
    {
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value = DT_IOCONFIG_MONITOR;
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue = DT_IOCONFIG_MONITOR;
        ParXtra = DtPropertiesGetInt(pPropData, "DEFAULT_PARXTRA0", 
                                                                 pNonIpPort->m_PortIndex);
        DT_ASSERT(ParXtra!=-1);
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_ParXtra[0] = ParXtra;
    }
    else if (!pNonIpPort->m_IsNonFuntional)
    {
        // IODIR must have a value
        DT_ASSERT(FALSE);
    }
    
    // DT_IOCONFIG_IOSTD; first check if a default has been defined
    // NOTE: this default property is not required, so do not incr the not found counter
    OldPropertyNotFoundCounter = pPropData->m_PropertyNotFoundCounter;
    DefIoStd = DtPropertiesGetInt(pPropData, "DEFAULT_IOSTD", pNonIpPort->m_PortIndex);
    pPropData->m_PropertyNotFoundCounter = OldPropertyNotFoundCounter;
    if (DefIoStd != -1)
    {
        DtDbgOut(MIN, NONIP, "[%d:%d] default IO-STD = %d", 
                                           pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber, 
                                                       pNonIpPort->m_PortIndex, DefIoStd);
        switch (DefIoStd)
        {
        case DT_IOCONFIG_ASI:
            DT_ASSERT(pNonIpPort->m_CapAsi);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_ASI;
            break;
        
        case DT_IOCONFIG_AVENC:
            DT_ASSERT(pNonIpPort->m_CapAvEnc);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_AVENC;
            break;

        case DT_IOCONFIG_DEMOD:
            DT_ASSERT(pNonIpPort->m_CapDemod);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_DEMOD;
            break;

        case DT_IOCONFIG_GPSTIME:
            DT_ASSERT(pNonIpPort->m_CapGpsTime);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_GPSTIME;
            break;

        case DT_IOCONFIG_HDMI:
            DT_ASSERT(pNonIpPort->m_CapHdmi);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDMI;
            break;

        case DT_IOCONFIG_IFADC:
            DT_ASSERT(pNonIpPort->m_CapIfAdc);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_IFADC;
            break;

        case DT_IOCONFIG_IP:
            DT_ASSERT(pNonIpPort->m_CapIp);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_IP;
            break;

        case DT_IOCONFIG_MOD:
            DT_ASSERT(pNonIpPort->m_CapMod);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_MOD;
            break;

        case DT_IOCONFIG_PHASENOISE:
            DT_ASSERT(pNonIpPort->m_CapPhaseNoise);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_PHASENOISE;
            break;

        case DT_IOCONFIG_SDIRX:
            DT_ASSERT(pNonIpPort->m_CapSdiRx);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SDIRX;
            break;

        case DT_IOCONFIG_SPI:
            DT_ASSERT(pNonIpPort->m_CapSpi);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SPI;
            break;

        case DT_IOCONFIG_3GSDI:
            DT_ASSERT(pNonIpPort->m_Cap3GSdi);

            // Select any of the 3G-SDI standards
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
            if (pNonIpPort->m_Cap1080P60)
                pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P60;
            else if (pNonIpPort->m_Cap1080P50)
                pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P50;
            else if (pNonIpPort->m_Cap1080P59_94)
                pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue =
                                                                   DT_IOCONFIG_1080P59_94;
            else if (pNonIpPort->m_Cap1080P60B)
                pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P60B;
            else if (pNonIpPort->m_Cap1080P50B)
                pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P50B;
            else if (pNonIpPort->m_Cap1080P59_94B)
                pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue =
                                                                  DT_IOCONFIG_1080P59_94B;
            else
                DT_ASSERT(1==0);
            break;
        case DT_IOCONFIG_1080P50:
            DT_ASSERT(pNonIpPort->m_Cap1080P50);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P50;
            break;
        case DT_IOCONFIG_1080P50B:
            DT_ASSERT(pNonIpPort->m_Cap1080P50B);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P50B;
            break;
        case DT_IOCONFIG_1080P59_94:
            DT_ASSERT(pNonIpPort->m_Cap1080P59_94);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P59_94;
            break;
        case DT_IOCONFIG_1080P59_94B:
            DT_ASSERT(pNonIpPort->m_Cap1080P59_94B);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P59_94B;
            break;
        case DT_IOCONFIG_1080P60:
            DT_ASSERT(pNonIpPort->m_Cap1080P60);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P60;
            break;
        case DT_IOCONFIG_1080P60B:
            DT_ASSERT(pNonIpPort->m_Cap1080P60B);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P60B;
            break;

        case DT_IOCONFIG_HDSDI:
            DT_ASSERT(pNonIpPort->m_CapHdSdi);

            // Select any of the HD-SDI standards
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            if (pNonIpPort->m_Cap1080I50)
                pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080I50;
            else if (pNonIpPort->m_Cap1080I59_94)
                pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = 
                                                                   DT_IOCONFIG_1080I59_94;
            else if (pNonIpPort->m_Cap720P60)
                pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P60;
            else
                DT_ASSERT(1 == 0);
            break;
        case DT_IOCONFIG_1080I50:
            DT_ASSERT(pNonIpPort->m_Cap1080I50);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080I50;
            break;
        case DT_IOCONFIG_1080I59_94:
            DT_ASSERT(pNonIpPort->m_Cap1080I59_94);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080I59_94;
            break;
        case DT_IOCONFIG_1080I60:
            DT_ASSERT(pNonIpPort->m_Cap1080I60);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080I60;
            break;
        case DT_IOCONFIG_1080P23_98:
            DT_ASSERT(pNonIpPort->m_Cap1080P23_98);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P23_98;
            break;
        case DT_IOCONFIG_1080P24:
            DT_ASSERT(pNonIpPort->m_Cap1080P24);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P24;
            break;
        case DT_IOCONFIG_1080P25:
            DT_ASSERT(pNonIpPort->m_Cap1080P25);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P25;
            break;
        case DT_IOCONFIG_1080P29_97:
            DT_ASSERT(pNonIpPort->m_Cap1080P29_97);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P29_97;
            break;
        case DT_IOCONFIG_1080P30:
            DT_ASSERT(pNonIpPort->m_Cap1080P30);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P30;
            break;
        case DT_IOCONFIG_1080PSF23_98:
            DT_ASSERT(pNonIpPort->m_Cap1080Psf23_98);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080PSF23_98;
            break;
        case DT_IOCONFIG_1080PSF24:
            DT_ASSERT(pNonIpPort->m_Cap1080Psf24);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080PSF24;
            break;
        case DT_IOCONFIG_1080PSF25:
            DT_ASSERT(pNonIpPort->m_Cap1080Psf25);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080PSF25;
            break;
        case DT_IOCONFIG_1080PSF29_97:
            DT_ASSERT(pNonIpPort->m_Cap1080Psf29_97);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080PSF29_97;
            break;
        case DT_IOCONFIG_1080PSF30:
            DT_ASSERT(pNonIpPort->m_Cap1080Psf30);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080PSF30;
            break;
        case DT_IOCONFIG_720P23_98:
            DT_ASSERT(pNonIpPort->m_Cap720P23_98);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P23_98;
            break;
        case DT_IOCONFIG_720P24:
            DT_ASSERT(pNonIpPort->m_Cap720P24);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P24;
            break;
        case DT_IOCONFIG_720P25:
            DT_ASSERT(pNonIpPort->m_Cap720P25);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P25;
            break;
        case DT_IOCONFIG_720P29_97:
            DT_ASSERT(pNonIpPort->m_Cap720P29_97);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P29_97;
            break;
        case DT_IOCONFIG_720P30:
            DT_ASSERT(pNonIpPort->m_Cap720P30);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P30;
            break;
        case DT_IOCONFIG_720P50:
            DT_ASSERT(pNonIpPort->m_Cap720P50);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P50;
            break;
        case DT_IOCONFIG_720P59_94:
            DT_ASSERT(pNonIpPort->m_Cap720P59_94);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P59_94;
            break;
        case DT_IOCONFIG_720P60:
            DT_ASSERT(pNonIpPort->m_Cap720P60);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_720P60;
            break;

        case DT_IOCONFIG_SDI:
            DT_ASSERT(pNonIpPort->m_CapSdi);

            // Select any of the SDI standards
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SDI;
            if (pNonIpPort->m_Cap625I50)
                pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_625I50;
            else if (pNonIpPort->m_Cap525I59_94)
                pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_525I59_94;
            else
                DT_ASSERT(1==0);
            break;
        case DT_IOCONFIG_525I59_94:
            DT_ASSERT(pNonIpPort->m_Cap525I59_94);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_525I59_94;
            break;
        case DT_IOCONFIG_625I50:
            DT_ASSERT(pNonIpPort->m_Cap625I50);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_625I50;
            break;

        case DT_IOCONFIG_RS422:
            DT_ASSERT(pNonIpPort->m_CapRs422);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_RS422;
            break;

        case DT_IOCONFIG_SPISDI:
            DT_ASSERT(pNonIpPort->m_CapSpiSdi);

            // Select any of the SPISDI standards
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SPISDI;
            if (pNonIpPort->m_CapSpi625I50)
                pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_SPI625I50;
            else if (pNonIpPort->m_CapSpi525I59_94)
                pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = 
                                                                 DT_IOCONFIG_SPI525I59_94;
            else
                DT_ASSERT(1==0);
            break;
        case DT_IOCONFIG_SPI525I59_94:
            DT_ASSERT(pNonIpPort->m_CapSpi525I59_94);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SPISDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_SPI525I59_94;
            break;
        case DT_IOCONFIG_SPI625I50:
            DT_ASSERT(pNonIpPort->m_CapSpi625I50);
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SPISDI;
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_SPI625I50;
            break;

        default:
            DtDbgOut(ERR, NONIP, "Unsupported DEFAULT_IOSTD for board type: %d port: %d",
                                      pPropData->m_TypeNumber, pNonIpPort->m_PortIndex+1);
            DT_ASSERT(1==0);
            break;
        }
    }
    else if (pNonIpPort->m_CapAsi)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_ASI;
    else if (pNonIpPort->m_CapAvEnc)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_AVENC;
    else if (pNonIpPort->m_Cap3GSdi && pNonIpPort->m_Cap1080P50)
    {
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_3GSDI;
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080P50;
    }
    else if (pNonIpPort->m_CapHdSdi && pNonIpPort->m_Cap1080I50)
    {
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDSDI;
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_1080I50;
    }
    else if (pNonIpPort->m_CapDemod)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_DEMOD;
    else if (pNonIpPort->m_CapGpsTime)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_GPSTIME;
    else if (pNonIpPort->m_CapHdmi)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_HDMI;
    else if (pNonIpPort->m_CapIfAdc)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_IFADC;
    else if (pNonIpPort->m_CapIp)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_IP;
    else if (pNonIpPort->m_CapMod)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_MOD;
    else if (pNonIpPort->m_CapPhaseNoise)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_PHASENOISE;
    else if (pNonIpPort->m_CapRs422)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_RS422;
    else if (pNonIpPort->m_CapSdi)
    {
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SDI;
        if (!pNonIpPort->m_Cap625I50 && pNonIpPort->m_Cap525I59_94)
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_525I59_94;
        else
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_625I50;
    }
    else if (pNonIpPort->m_CapSdiRx)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SDIRX;
    else if (pNonIpPort->m_CapSpi)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SPI;
    else if (pNonIpPort->m_CapSpiSdi)
    {
        pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_Value = DT_IOCONFIG_SPISDI;
        if (!pNonIpPort->m_CapSpi625I50 && pNonIpPort->m_CapSpi525I59_94)
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_SPI525I59_94;
        else
            pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue = DT_IOCONFIG_SPI625I50;
    }
    else  if (!pNonIpPort->m_IsNonFuntional)
    {
        // IOSTD must have a value
        DT_ASSERT(FALSE);
    }

    // DT_IOCONFIG_RFCLKSEL
    if (pNonIpPort->m_CapRfClkInt) 
        pNonIpPort->m_IoCfg[DT_IOCONFIG_RFCLKSEL].m_Value = DT_IOCONFIG_RFCLKINT;
    else if (pNonIpPort->m_CapRfClkExt)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_RFCLKSEL].m_Value = DT_IOCONFIG_RFCLKEXT;

    // DT_IOCONFIG_SPICLKSEL
    if (pNonIpPort->m_CapSpiClkInt)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SPICLKSEL].m_Value = DT_IOCONFIG_SPICLKINT;
    else if (pNonIpPort->m_CapSpiClkExt)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SPICLKSEL].m_Value = DT_IOCONFIG_SPICLKEXT;

    // DT_IOCONFIG_SPIMODE
    if (pNonIpPort->m_CapSpiDvbMode)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SPIMODE].m_Value = DT_IOCONFIG_SPIDVBMODE;
    else if (pNonIpPort->m_CapSpiFixedClk)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SPIMODE].m_Value = DT_IOCONFIG_SPIFIXEDCLK;
    else if (pNonIpPort->m_CapSpiSer8B)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SPIMODE].m_Value = DT_IOCONFIG_SPISER8B;
    else if (pNonIpPort->m_CapSpiSer10B)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SPIMODE].m_Value = DT_IOCONFIG_SPISER10B;

    // DT_IOCONFIG_SPISTD
    if (pNonIpPort->m_CapSpi)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SPISTD].m_Value = DtPropertiesGetInt(
                                    pPropData, "DEFAULT_SPISTD", pNonIpPort->m_PortIndex);

    // DT_IOCONFIG_TSRATESEL
    if (pNonIpPort->m_CapIntTsRate)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_TSRATESEL].m_Value = DT_IOCONFIG_INTTSRATE;
    else if (pNonIpPort->m_CapExtTsRate)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_TSRATESEL].m_Value = DT_IOCONFIG_EXTTSRATE;
    else if (pNonIpPort->m_CapExtRatio)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_TSRATESEL].m_Value = DT_IOCONFIG_EXTRATIO;
    else if (pNonIpPort->m_CapLock2Inp)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_TSRATESEL].m_Value = DT_IOCONFIG_LOCK2INP;

    // DT_IOCONFIG_SWS2APSK
    if (pNonIpPort->m_CapSwS2Apsk)   
        pNonIpPort->m_IoCfg[DT_IOCONFIG_SWS2APSK].m_Value = DT_IOCONFIG_FALSE;
    
    // DT_IOCONFIG_FAILSAFE
    if (pNonIpPort->m_CapFailSafe)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_FAILSAFE].m_Value = DT_IOCONFIG_FALSE;

    // DT_IOCONFIG_GENREF
    if (pNonIpPort->m_CapGenRef)
    {
        pNonIpPort->m_IoCfg[DT_IOCONFIG_GENREF].m_Value = 
                 DtPropertiesGetInt(pPropData, "DEFAULT_GENREF", pNonIpPort->m_PortIndex);
    }
    
    // DT_IOCONFIG_GENLOCK
    if (pNonIpPort->m_CapGenLocked)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_GENLOCKED].m_Value = DT_IOCONFIG_FALSE;

    // DT_IOCONFIG_FRACMODE
    if (pNonIpPort->m_CapFracMode)
        pNonIpPort->m_IoCfg[DT_IOCONFIG_FRACMODE].m_Value = DT_IOCONFIG_FALSE;

    // Initialize exclusive access
    pNonIpPort->m_ExclAccess = FALSE;

    // Initialize status flags
    pNonIpPort->m_Flags = 0;
    pNonIpPort->m_FlagsLatched = 0;
    DtSpinLockInit(&pNonIpPort->m_FlagsSpinLock);

    // Init failsafe fields
    if (pNonIpPort->m_CapFailSafe)
        DtaNonIpTxSetFailsafeCfg(pNonIpPort, FALSE, 10);

    // Init target detection
    pNonIpPort->m_TrgIdDet.m_TargetId = -1;
    pNonIpPort->m_TrgIdDet.m_Present = DTA_TARGET_UNKNOWN;
    pNonIpPort->m_TrgIdDet.m_TdState = DTA_TD_INITIAL;
    pNonIpPort->m_TrgIdDet.m_LastTargetId = -1;
    pNonIpPort->m_TrgIdDet.m_NumTimesInFind = 0;
    pNonIpPort->m_TrgIdDet.m_StableCount = 0;
    pNonIpPort->m_TrgIdDet.m_TargetError = FALSE;

    // Init TOF alignment offset
    pNonIpPort->m_TofAlignOffsetNs = 0;     // Assume a zero offset
       
    // Initialise bitrate measurement
    pNonIpPort->m_BitrateMeasure.m_NumValidSamps = 0;       // No valid samples yet
    pNonIpPort->m_BitrateMeasure.m_ValidCount256 = 0;       // Initial bitrate is 0

    // Initialise SPI-Masterflash support
    pNonIpPort->m_SpiMf.m_IsSupported = FALSE;
    
    // Initialize register mappings
    pNonIpPort->m_pModRegs = NULL;
    pNonIpPort->m_pRfRegs = NULL;
    pNonIpPort->m_pRxRegs = NULL;
    pNonIpPort->m_pTxRegs = NULL;
    if (pNonIpPort->m_CapInput)
    {
        pNonIpPort->m_RxRegsOffset = DtPropertiesGetUInt16(pPropData,
                                                 "REGISTERS_RX", pNonIpPort->m_PortIndex);

        // NOTE: matrix channels use a different offset for the local DMA address
        if (pNonIpPort->m_CapMatrix)
            pNonIpPort->m_FifoOffset = pNonIpPort->m_RxRegsOffset + DT_HD_REG_FIFO_FIRST;
        else
            pNonIpPort->m_FifoOffset = pNonIpPort->m_RxRegsOffset+DTA_LOCALADDR_FIFODATA;
    } else
        pNonIpPort->m_RxRegsOffset = (UInt16)-1;
    if (pNonIpPort->m_CapOutput)
    {
        pNonIpPort->m_TxRegsOffset = DtPropertiesGetUInt16(pPropData,
                                                 "REGISTERS_TX", pNonIpPort->m_PortIndex);
        // NOTE: matrix channels use a different offset for the local DMA address
        if (pNonIpPort->m_CapMatrix)
            pNonIpPort->m_FifoOffset = pNonIpPort->m_TxRegsOffset + DT_HD_REG_FIFO_FIRST;
        else
            pNonIpPort->m_FifoOffset = pNonIpPort->m_TxRegsOffset+DTA_LOCALADDR_FIFODATA;
    } else
        pNonIpPort->m_TxRegsOffset = (UInt16)-1;
    if (pNonIpPort->m_CapMod)
    {
        pNonIpPort->m_ModRegsOffset = DtPropertiesGetUInt16(pPropData,
                                                "REGISTERS_MOD", pNonIpPort->m_PortIndex);
        pNonIpPort->m_RfRegsOffset = DtPropertiesGetUInt16(pPropData,
                                                "REGISTERS_RF", pNonIpPort->m_PortIndex);
    } else {
        pNonIpPort->m_ModRegsOffset = (UInt16)-1;
        pNonIpPort->m_RfRegsOffset = (UInt16)-1;
    }
    if (pNonIpPort->m_CapSpi)
    {
        pNonIpPort->m_SpiRegsOffset = DtPropertiesGetUInt16(pPropData,
                                                "REGISTERS_SPI", pNonIpPort->m_PortIndex);
    } else
        pNonIpPort->m_SpiRegsOffset = (UInt16)-1;
    if (pNonIpPort->m_CapRs422)
    {
        pNonIpPort->m_Rs422RegsOffset = DtPropertiesGetUInt16(pPropData,
                                              "REGISTERS_RS422", pNonIpPort->m_PortIndex);
    } else
        pNonIpPort->m_Rs422RegsOffset = (UInt16)-1;
    
    if (pNonIpPort->m_CapSdiRx || pNonIpPort->m_CapHdmi || pNonIpPort->m_CapAvEnc)
    {
        pNonIpPort->m_FwbRegsOffset = DtPropertiesGetUInt16(pPropData,
                                                "REGISTERS_FWB", pNonIpPort->m_PortIndex);
    } else
        pNonIpPort->m_FwbRegsOffset = (UInt16)-1;

    // Report configuration errors
    if (DT_SUCCESS(Status))
        // Check if no property error occurred
        Status = DtaPropertiesReportDriverErrors(pNonIpPort->m_pDvcData);

    // Skip DMA initialisation for non-functional ports
    if (!pNonIpPort->m_IsNonFuntional)
    {

        if (DT_SUCCESS(Status))
            Status = DtaNonIpDetermineDmaRegsOffset(pDvcData, pNonIpPort->m_PortIndex, 
                                                                          &DmaRegsOffset);

        if (DT_SUCCESS(Status) && !pNonIpPort->m_CapAvEnc)
            Status = DtaDmaInitCh(pDvcData, PortIndex, 
                                                   DtaNonIpGetMaxDmaBurstSize(pNonIpPort),
                                                   DTA_DMA_MODE_DEFAULT, DmaRegsOffset, 
#ifdef WINBUILD
                                                   DTA_DMA_FLAGS_NONE, -1,
                                                   DtaNonIpDmaCompletedWindows, NULL,
#else
                                                   DTA_DMA_FLAGS_BLOCKING, -1,
                                                   NULL, NULL,
#endif
                                                   &pNonIpPort->m_DmaChannel,
                                                   TRUE);
        if (DT_SUCCESS(Status) && pNonIpPort->m_CapAvEnc)
            Status = DtaDmaInitCh(pDvcData, PortIndex, 
                                                  DtaNonIpGetMaxDmaBurstSize(pNonIpPort),
                                                  DTA_DMA_MODE_RING_BUFFER, DmaRegsOffset,
                                                  DTA_DMA_FLAGS_DATA_BUF_NO_COPY, -1,
                                                  NULL, NULL,
                                                  &pNonIpPort->m_DmaChannel,
                                                  TRUE);
        if (!DT_SUCCESS(Status))
            return Status;
    }

     // Demod specific initialisation
    if (pNonIpPort->m_CapDemod)
    {
        // Set callback function for additional I2c (un)lock 
        HasIc2RfPwrMeas = DtPropertiesGetBool(pPropData, "DEMOD_HW_HAS_I2C_RFPWR_MEAS",
                                                                 pNonIpPort->m_PortIndex);
        if (HasIc2RfPwrMeas)
        {
            // Allow a I2c calback for the main device only
            Status = DtaI2cLockCallBackSet(pDvcData, -1, DtaRfPwrMeasLock);
            if (!DT_SUCCESS(Status))
                return Status;
        }
    }
    
    // Matrix-API initialisation
    if (pNonIpPort->m_CapMatrix)
    {
        Status = DtaNonIpMatrixInit(pNonIpPort); 
        if (!DT_SUCCESS(Status))
            return Status;
    }

    // RS-422 ports initialization
    if (pNonIpPort->m_CapRs422)
    {
        Status = DtaNonIpRs422Init(pNonIpPort);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    // SDI AV RX ports initialization
    if (pNonIpPort->m_CapSdiRx)
    {
        Status = DtaNonIpSdiAvRxInit(pNonIpPort);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    // HDMI port initialization
    if (pNonIpPort->m_CapHdmi)
    {
        Status = DtaNonIpHdmiInit(pNonIpPort);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    // Audio video encoder port initialization
    if (pNonIpPort->m_CapAvEnc)
    {
        Status = DtaEnDecInit(pNonIpPort);
        if (!DT_SUCCESS(Status))
            return Status;
    }
 
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpInterruptEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Enables Non-IP port specific interrupts. 
//
// PRE-CONDITION: Interrupt-Enable-State lock (pDvcData->m_IntEnaStateLock) must have 
// been acquired
//
DtStatus  DtaNonIpInterruptEnable(DtaNonIpPort* pNonIpPort)
{
    DtaDeviceData*  pDvcData = NULL;
    DtIntEnableState  IntEnaState;

    DT_ASSERT(pNonIpPort != NULL);

    pDvcData =  pNonIpPort->m_pDvcData;
    IntEnaState = pDvcData->m_IntEnableState;

    // Do nothing if master state is not enabled or enabling
    if (IntEnaState!=INT_ENABLING && IntEnaState!=INT_ENABLED)
    {
        DtDbgOut(AVG, NONIP, "Master-interupt state (%d) is not enabled", IntEnaState);
        return DT_STATUS_OK;
    }
    
    // Enable I2c interrupt on port level
    if (pNonIpPort->m_I2c.m_IsSupported)
        DtaRegI2cCtrlSetRdyIntEn(pNonIpPort->m_I2c.m_pI2cRegs, 1);

    // Enable Matrix specific interrupts
    if (pNonIpPort->m_CapMatrix)
        DT_RETURN_ON_ERROR(DtaNonIpMatrixInterruptEnable(pNonIpPort));

    // Enable RS-422 specific interrupts
    if (pNonIpPort->m_CapRs422)
        DT_RETURN_ON_ERROR(DtaNonIpRs422InterruptEnable(pNonIpPort));

    // Enable SDI-AV specific interrupts
    if (pNonIpPort->m_CapSdiRx)
        DT_RETURN_ON_ERROR(DtaNonIpSdiAvRxInterruptEnable(pNonIpPort));

    // Enable HDMI specific interrupts
    if (pNonIpPort->m_CapHdmi)
        DT_RETURN_ON_ERROR(DtaNonIpHdmiInterruptEnable(pNonIpPort));

    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpInterruptDisable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Disables Non-IP port specific interrupts
//
// PRE-CONDITION: Interrupt-Enable-State lock (pDvcData->m_IntEnaStateLock) must have 
// been acquired
//
DtStatus  DtaNonIpInterruptDisable(DtaNonIpPort* pNonIpPort)
{
    DT_ASSERT(pNonIpPort != NULL);

    // Disable I2c interrupt on port level
    if (pNonIpPort->m_I2c.m_IsSupported)
        DtaRegI2cCtrlSetRdyIntEn(pNonIpPort->m_I2c.m_pI2cRegs, 0);

    // Disable Matrix specific interrupts
    if (pNonIpPort->m_CapMatrix)
        DT_RETURN_ON_ERROR(DtaNonIpMatrixInterruptDisable(pNonIpPort));

    // Disable RS-422 specific interrupts
    if (pNonIpPort->m_CapRs422)
        DT_RETURN_ON_ERROR(DtaNonIpRs422InterruptDisable(pNonIpPort));

    // Disable SDI-AV RX specific interrupts
    if (pNonIpPort->m_CapSdiRx)
        DT_RETURN_ON_ERROR(DtaNonIpSdiAvRxInterruptDisable(pNonIpPort));

    // Disable HDMI specific interrupts
    if (pNonIpPort->m_CapHdmi)
        DT_RETURN_ON_ERROR(DtaNonIpHdmiInterruptDisable(pNonIpPort));

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpInterrupt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaNonIpInterrupt(DtaNonIpPort* pNonIpPort)
{
    Bool  IrqHandled = FALSE;

    if (pNonIpPort->m_CapSdiRx)
        IrqHandled |= DtaSdiAvRxInterrupt(pNonIpPort);
    
    if (pNonIpPort->m_CapHdmi)
        IrqHandled |= DtaNonIpHdmiInterrupt(pNonIpPort);

    if (pNonIpPort->m_CapAvEnc)
        IrqHandled |= DtaEncD7proInterrupt(pNonIpPort);

    return IrqHandled;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpDetermineDmaRegsOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpDetermineDmaRegsOffset(
    DtaDeviceData*  pDvcData,
    Int  PortIndex,         // Physical port index
    UInt*  pDmaRegsOffset)  // out:The DMA register offset for fpga controlled DMA,
                            // or the DMA controller index if PLX is used.
{
    DtStatus  Status = DT_STATUS_OK;
    DmaOpt*  pDmaOptions = &pDvcData->m_DmaOptions;
    DtPropertyData*  pPropData = &pDvcData->m_PropData;

    if (pDmaOptions->m_UseDmaInFpga)
        *pDmaRegsOffset = DtPropertiesGetUInt16(pPropData, "REGISTERS_DMA", PortIndex);
    else {
        // If only the DMA controller in the PLX is used, the RegsOffset must indicate
        // which DMA controller in the PLX is used for the specified port.
        if (pDmaOptions->m_PlxDmaChannelPort[0] == PortIndex)
            *pDmaRegsOffset = 0;
        else if (pDmaOptions->m_PlxDmaChannelPort[1] == PortIndex)
            *pDmaRegsOffset = 1;
        else
            DT_ASSERT(FALSE);
    }

    Status = DtaPropertiesReportDriverErrors(pDvcData);
    if (!DT_SUCCESS(Status))
        return Status;

#ifdef _DEBUG
    if (!pDmaOptions->m_UseDmaInFpga)
    {
        UInt DmaRegsOffset = 0;

        // Just check for a clean XML-file. REGISTERS_DMA is not used if only the PLX is
        // used for DMA.
        DmaRegsOffset = DtPropertiesGetUInt16(pPropData, "REGISTERS_DMA", PortIndex);
        DT_ASSERT(DmaRegsOffset==0 || DmaRegsOffset==-1 || 
                                                 pPropData->m_PropertyNotFoundCounter==1);
        pPropData->m_PropertyNotFoundCounter = 0;
    }
#endif

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpCleanupPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaNonIpCleanupPre(
    DtaDeviceData*  pDvcData, 
    Int  PortIndex,
    DtaNonIpPort*  pNonIpPort)
{
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpCleanup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtaNonIpCleanup(DtaDeviceData* pDvcData, Int PortIndex, DtaNonIpPort* pNonIpPort)
{
    if (pNonIpPort->m_IsNonFuntional)
        return; // Nothing todo for non-functional ports
        
    DtaDmaCleanupCh(pDvcData, &pNonIpPort->m_DmaChannel);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpClose(
    DtaNonIpPort* pNonIpPort,
    DtFileObject* pFile)
{
    DtStatus  HasAccess;
    DT_ASSERT(pNonIpPort != NULL);

    // Unlock all recursive I2C locks for this file object
    if (pNonIpPort->m_I2c.m_IsSupported)
        DtaI2cUnlock(pNonIpPort->m_pDvcData, pNonIpPort->m_PortIndex, pFile, TRUE);

    DtaDeviceAcquireExclAccess(pNonIpPort->m_pDvcData);
    HasAccess = DtaNonIpHasAccess(pNonIpPort, pFile);
    DtaDeviceReleaseExclAccess(pNonIpPort->m_pDvcData);

    if (DT_SUCCESS(HasAccess))
    {
#ifndef WINBUILD
        // Under windows the DMA is aborted by cancelling the outstanding IoCtl
        if (!pNonIpPort->m_IsNonFuntional)
        {
            if (DtaDmaAbortDma(&pNonIpPort->m_DmaChannel) != DT_STATUS_NOT_STARTED)
                DtEventWait(&pNonIpPort->m_DmaChannel.m_DmaDoneEvent, -1);
        }
#endif
        DtaDmaClearAbortFlag(&pNonIpPort->m_DmaChannel);

        // Call matrix specific close
        if (pNonIpPort->m_CapMatrix)
            DtaNonIpMatrixClose(pNonIpPort, pFile);
    }

    // Call encoder specific close
    if (pNonIpPort->m_CapAvEnc)
        DtaEnDecClose(pNonIpPort, pFile);

    // Call SPI master flash specific close
    if (pNonIpPort->m_SpiMf.m_IsSupported)
        DtaSpiMfClose(&pNonIpPort->m_SpiMf, pNonIpPort, pFile);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpPowerup -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called everytime the hardware is powered up. Resources like registers
// can be remapped, so we need to recalculate the register pointers.
//
DtStatus  DtaNonIpPowerup(DtaNonIpPort* pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;
    // Recalculate registers
    if (pNonIpPort->m_ModRegsOffset != (UInt16)-1)
        pNonIpPort->m_pModRegs = pNonIpPort->m_pDvcData->m_pGenRegs +
                                                              pNonIpPort->m_ModRegsOffset;
    if (pNonIpPort->m_RfRegsOffset != (UInt16)-1)
        pNonIpPort->m_pRfRegs = pNonIpPort->m_pDvcData->m_pGenRegs +
                                                               pNonIpPort->m_RfRegsOffset;
    if (pNonIpPort->m_RxRegsOffset != (UInt16)-1)
        pNonIpPort->m_pRxRegs = pNonIpPort->m_pDvcData->m_pGenRegs +
                                                               pNonIpPort->m_RxRegsOffset;
    if (pNonIpPort->m_TxRegsOffset != (UInt16)-1)
        pNonIpPort->m_pTxRegs = pNonIpPort->m_pDvcData->m_pGenRegs +
                                                               pNonIpPort->m_TxRegsOffset;
    if (pNonIpPort->m_SpiRegsOffset != (UInt16)-1)
        pNonIpPort->m_pSpiRegs = pNonIpPort->m_pDvcData->m_pGenRegs +
                                                              pNonIpPort->m_SpiRegsOffset;
    if (pNonIpPort->m_Rs422RegsOffset != (UInt16)-1)
        pNonIpPort->m_pRs422Regs = pNonIpPort->m_pDvcData->m_pGenRegs +
                                                            pNonIpPort->m_Rs422RegsOffset;
    if (pNonIpPort->m_FwbRegsOffset != (UInt16)-1)
        pNonIpPort->m_pFwbRegs = pNonIpPort->m_pDvcData->m_pGenRegs +
                                                              pNonIpPort->m_FwbRegsOffset;

    // Skip init of DMA resources for a non-functional port
    if (!pNonIpPort->m_IsNonFuntional)
    {
        Status = DtaDmaInitChPowerup(&pNonIpPort->m_DmaChannel);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    // SDI AV RX power-up initialisation
    if (pNonIpPort->m_CapSdiRx)
    {
        Status = DtaSdiAvRxInitPowerup(pNonIpPort);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    // HDMI power-up initialisation
    if (pNonIpPort->m_CapHdmi)
    {
        Status = DtaNonIpHdmiInitPowerup(pNonIpPort);
        if (!DT_SUCCESS(Status))
            return Status;
    }
    
    // Encoder/decoder power-up
    if (pNonIpPort->m_CapAvEnc)
    {
        Status = DtaEnDecInitPowerup(pNonIpPort);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    // SPI master flash controller post power-up initialisation
    if (pNonIpPort->m_SpiMf.m_IsSupported)
    {
        Status = DtaSpiMfInitPowerup(pNonIpPort);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    // Clear flags
    pNonIpPort->m_Flags = 0;
    pNonIpPort->m_FlagsLatched = 0;

    // Initialise bitrate measurement
    pNonIpPort->m_BitrateMeasure.m_NumValidSamps = 0;       // No valid samples yet
    pNonIpPort->m_BitrateMeasure.m_ValidCount256 = 0;       // Initial bitrate is 0
    
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpPowerUpPost -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// This function is called after the DtaNonIpPowerup and the after device interrupts 
// have been enabled
//
DtStatus  DtaNonIpPowerUpPost(DtaNonIpPort* pNonIpPort)
{
    DtStatus  Status = DT_STATUS_OK;

    if (pNonIpPort->m_CapMatrix)
    {
        Status = DtaNonIpMatrixPowerUpPost(pNonIpPort);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    if (pNonIpPort->m_CapHdmi)
    {
        Status = DtaNonIpHdmiInitPowerUpPost(pNonIpPort);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    if (pNonIpPort->m_CapAvEnc)
    {
        Status = DtaEnDecPowerUpPost(pNonIpPort);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpIoctl(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoctlObject*  pIoctl)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    Int  NonIpPortIndex;        // Index in the nonip port struct
    DtaNonIpPort*  pNonIpPort=NULL;
    DtaIoctlNonIpCmdInput* pNonIpCmdInput = 
                                           (DtaIoctlNonIpCmdInput*)pIoctl->m_pInputBuffer;
    DtaIoctlNonIpCmdOutput* pNonIpCmdOutput =
                                         (DtaIoctlNonIpCmdOutput*)pIoctl->m_pOutputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlNonIpCmdInput, m_Data);
    OutReqSize = OFFSETOF(DtaIoctlNonIpCmdOutput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate port index
    if (!DT_SUCCESS(DtaGetNonIpPortIndex(pDvcData, pNonIpCmdInput->m_PortIndex,
                                                                        &NonIpPortIndex)))
        return DT_STATUS_INVALID_PARAMETER;
    
    // Get the NonIpPort
    pNonIpPort = &pDvcData->m_pNonIpPorts[NonIpPortIndex];

    // Determine final required output/input sizes
    switch (pNonIpCmdInput->m_Cmd)
    {
    case DTA_NONIP_CMD_EXCLUSIVE_ACCESS:
        pCmdStr = "DTA_NONIP_CMD_EXCLUSIVE_ACCESS";
        InReqSize += sizeof(DtaIoctlNonIpCmdExclusiveAccessInput);
        OutReqSize = 0;
        break;

    case DTA_NONIP_CMD_GET_TARGET_ID:
        pCmdStr = "DTA_NONIP_CMD_GET_TARGET_ID";
        OutReqSize += sizeof(DtaIoctlNonIpCmdGetTargetIdOutput);
        // We expect no input buffer at all
        InReqSize = 0;
        break;

    case DTA_NONIP_CMD_DETECT_VIDSTD:
        pCmdStr = "DTA_NONIP_CMD_DETECT_VIDSTD";
        OutReqSize += sizeof(DtaIoctlNonIpCmdDetectVidStdOutput);
        // We expect no additional input data
        InReqSize += 0;
        break;

     case DTA_NONIP_CMD_GET_GENREF_PROPS:
        pCmdStr = "DTA_NONIP_CMD_GET_GENREF_PROPS";
        OutReqSize += sizeof(DtaIoctlNonIpGetGenRefPropsOutput);
        // We expect no additional input data
        InReqSize += 0;
        break;

    case DTA_NONIP_CMD_NOTIFY_GENREF_PROPS:
        pCmdStr = "DTA_NONIP_CMD_NOTIFY_GENREF_PROPS";
        InReqSize += sizeof(DtaIoctlNonIpNotifyGenRefPropsInput);
        // We expect no additional output data
        OutReqSize += 0;
        break;

    case DTA_NONIP_CMD_DETECT_VIDSTD2:
        pCmdStr = "DTA_NONIP_CMD_DETECT_VIDSTD2";
        OutReqSize += sizeof(DtaIoctlNonIpCmdDetectVidStd2Output);
        // We expect no additional input data
        InReqSize += 0;
        break;

    case DTA_NONIP_CMD_GET_AUDIO_STATUS:
        pCmdStr = "DTA_NONIP_CMD_GET_AUDIO_STATUS";
        OutReqSize += sizeof(DtaIoctlNonIpCmdGetAudioStatusOutput);
        // We expect no additional input data
        InReqSize += 0;
        break;

    case DTA_NONIP_CMD_GET_DMA_STATS:
        pCmdStr = "DTA_NONIP_CMD_GET_DMA_STATS";
        OutReqSize += sizeof(DtaIoctlNonIpCmdGetDmaStatsOutput);
        // We expect no additional input data
        InReqSize += 0;
        break;

    default:
        pCmdStr = "??UNKNOWN NONIPCMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, NONIP, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, NONIP, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, NONIP, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
                pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    }
    
    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        // Execute cmd
        switch (pNonIpCmdInput->m_Cmd)
        {
        case DTA_NONIP_CMD_EXCLUSIVE_ACCESS:
            Status = DtaNonIpExclusiveAccess(pNonIpPort, pFile, 
                                          pNonIpCmdInput->m_Data.m_ExclusiveAccess.m_Cmd);
            break;

        case DTA_NONIP_CMD_GET_TARGET_ID:
            Status = DtaNonIpTargetIdGetId(pNonIpPort,
                                         &pNonIpCmdOutput->m_Data.m_GetTargetId.m_Present,
                                         &pNonIpCmdOutput->m_Data.m_GetTargetId.m_Id);
            break;

        case DTA_NONIP_CMD_DETECT_VIDSTD:
            Status = DtaNonIpDetectVidStd(pNonIpPort,
                                           &pNonIpCmdOutput->m_Data.m_DetVidStd.m_VidStd,
                                           NULL, NULL, NULL);
            break;

         case DTA_NONIP_CMD_GET_GENREF_PROPS:
             Status = DtaNonIpGetGenRefProps(pNonIpPort, 
                                               &pNonIpCmdOutput->m_Data.m_GetGenRefProps);
            break;

        case DTA_NONIP_CMD_NOTIFY_GENREF_PROPS:
            Status = DtaNonIpNotifyGenRefProp(pNonIpPort,
                                             &pNonIpCmdInput->m_Data.m_NotifyGenRefProps);
            break;

        case DTA_NONIP_CMD_DETECT_VIDSTD2: 
            Status = DtaNonIpDetectVidStd(pNonIpPort,
                                     &pNonIpCmdOutput->m_Data.m_DetVidStd2.m_VidStd,
                                     &pNonIpCmdOutput->m_Data.m_DetVidStd2.m_Vpid,
                                     &pNonIpCmdOutput->m_Data.m_DetVidStd2.m_Vpid2,
                                     &pNonIpCmdOutput->m_Data.m_DetVidStd2.m_AspectRatio);
            break;

        case DTA_NONIP_CMD_GET_AUDIO_STATUS:
            Status = DtaNonIpGetAudioStatus(pNonIpPort,
                                               &pNonIpCmdOutput->m_Data.m_GetAudioStatus);
            break;

        case DTA_NONIP_CMD_GET_DMA_STATS:
            pNonIpCmdOutput->m_Data.m_GetDmaStats.m_Time =
                                                   pNonIpPort->m_DmaChannel.m_EndTime
                                                   - pNonIpPort->m_DmaChannel.m_StartTime;
            pNonIpCmdOutput->m_Data.m_GetDmaStats.m_Direction =
                                                  pNonIpPort->m_DmaChannel.m_DmaDirection;
            if (pNonIpPort->m_DmaChannel.m_DmaDirection == DT_DMA_DIRECTION_FROM_DEVICE)
            {
                pNonIpCmdOutput->m_Data.m_GetDmaStats.m_NumBytesTransferred =
                                                  pNonIpPort->m_DmaChannel.m_NumBytesRead;
            } else {
                pNonIpCmdOutput->m_Data.m_GetDmaStats.m_NumBytesTransferred =
                                         pNonIpPort->m_DmaChannel.m_CurrentTransferLength;
            }
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
            DtDbgOut(MIN, NONIP, "NonIpCmd=0x%x: NOT SUPPORTED", pNonIpCmdInput->m_Cmd);
        else 
            DtDbgOut(MIN, NONIP, "%s: ERROR %xh", pCmdStr, Status);
    }

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Exclusive access -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpHasAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// PRE: ExclAccessMutex mutex must be acquired!!!
//
DtStatus  DtaNonIpHasAccess(DtaNonIpPort* pNonIpPort, DtFileObject* pFile)
{
    if (pFile == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    if (pNonIpPort->m_ExclAccess)
    {
        if (!DtFileCompare(&pNonIpPort->m_ExclAccessOwner, pFile))
            return DT_STATUS_IN_USE;
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpExclusiveAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpExclusiveAccess(
    DtaNonIpPort*  pNonIpPort,
    DtFileObject*  pFile,
    Int  Cmd)
{
    DtStatus  Result = DT_STATUS_OK;

    DtaDeviceAcquireExclAccess(pNonIpPort->m_pDvcData);

    if (Cmd == DTA_EXCLUSIVE_ACCESS_CMD_ACQUIRE)
    {
        if (!pNonIpPort->m_ExclAccess)
        {
            pNonIpPort->m_ExclAccess = TRUE;
            pNonIpPort->m_ExclAccessOwner = *pFile;
        } else
            Result = DT_STATUS_IN_USE;
    } else if (Cmd == DTA_EXCLUSIVE_ACCESS_CMD_RELEASE)
    {
        if (pNonIpPort->m_ExclAccess)
        {
            if (DtFileCompare(&pNonIpPort->m_ExclAccessOwner, pFile))
                pNonIpPort->m_ExclAccess = FALSE;
            else
                Result = DT_STATUS_IN_USE;
        }
    } else if (Cmd == DTA_EXCLUSIVE_ACCESS_CMD_PROBE)
    {
        if (pNonIpPort->m_ExclAccess)
            Result = DT_STATUS_IN_USE;
    } else if (Cmd == DTA_EXCLUSIVE_ACCESS_CMD_CHECK)
    {
        if (!pNonIpPort->m_ExclAccess)
            Result = DT_STATUS_EXCL_ACCESS_REQD;
        else if (!DtFileCompare(&pNonIpPort->m_ExclAccessOwner, pFile))
            Result = DT_STATUS_IN_USE;
    } else
        Result = DT_STATUS_NOT_SUPPORTED;

    DtaDeviceReleaseExclAccess(pNonIpPort->m_pDvcData);

    return Result;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Target ID detection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Constants
#define  GROUNDED      0        // DVB-SPI cable connected => sense input is grounded
#define  OPEN_CIRCUIT  36       // Nothing connected to 25p subD connector

// Forward declarations
static void  DtaTargetIdReadId(volatile UInt8* pRegBase, Int* pnTargetId);
static void  DtaTargetIdSenseAtoDUsingDacA(volatile UInt8* pRegBase, Int* pAdcValue);
static void  DtaTargetIdSenseAtoDUsingDacB(volatile UInt8* pRegBase, Int* pAdcValue);
static void  DtaTargetIdSetWindow(volatile UInt8* pRegBase);
static void  DtaTargetIdSetWindowForGrounded(volatile UInt8* pRegBase);
static void  DtaTargetIdSetWindowForOpenCircuit(volatile UInt8* pRegBase);
static void  DtaTargetIdWaitForUpdateDone(volatile UInt8* pRegBase);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpTargetIdGetId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Read results of the target-ID detection process.
//
DtStatus  DtaNonIpTargetIdGetId(
    DtaNonIpPort*  pNonIpPort,  // Port struct
    Int*  pPresent,             // Target adapter is present (best guess)
    Int*  pTargetId)            // Target adapter ID (best guess)
{
    *pPresent = pNonIpPort->m_TrgIdDet.m_Present;
    *pTargetId = pNonIpPort->m_TrgIdDet.m_TargetId;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpTargetIdDetectStateMachine -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Main state machine for target-adapter detection.
// Called from periodic interrupt DPC (=every 77ms).
//
// See DTA-102 Design Document for graphical state diagram.
//
void  DtaNonIpTargetIdDetectStateMachine(DtaNonIpPort* pNonIpPort)
{
    Int  NewTdState, TargetIdRead;
    const char*  StrState;
    volatile UInt8*  pRegBase = NULL;
    TargetIdDetection*  pTrgIdDet = &pNonIpPort->m_TrgIdDet;
    DtaIoConfigValue  CfgValue;

    // Get base register pointer
    DtaNonIpIoConfigGet(pNonIpPort, DT_IOCONFIG_IODIR, &CfgValue);
    if (CfgValue.m_Value == DT_IOCONFIG_OUTPUT)
        pRegBase = pNonIpPort->m_pTxRegs;
    if (CfgValue.m_Value == DT_IOCONFIG_INPUT)
        pRegBase = pNonIpPort->m_pRxRegs;
    if (pRegBase == NULL)
        return;

    // Default for new state: same as old state
    NewTdState = pTrgIdDet->m_TdState;

    switch (pTrgIdDet->m_TdState)
    {
    case DTA_TD_INITIAL:
to_find:
        NewTdState = DTA_TD_FIND_TARGETID;
        pTrgIdDet->m_Present = DTA_TARGET_UNKNOWN;
        pTrgIdDet->m_NumTimesInFind = 0;
        pTrgIdDet->m_StableCount = 0;
        DtaTargetIdReadId(pRegBase, &pTrgIdDet->m_LastTargetId);
        break;

    case DTA_TD_FIND_TARGETID:
        // If we are at >8 times in this state, TARGET_ADAPTER becomes unknown
        if (pTrgIdDet->m_NumTimesInFind++ > 8)
            pTrgIdDet->m_Present = DTA_TARGET_UNKNOWN;

        // Read target ID and check whether it is stable
        DtaTargetIdReadId(pRegBase, &TargetIdRead);
        if (TargetIdRead != pTrgIdDet->m_LastTargetId)
        {
            pTrgIdDet->m_LastTargetId = TargetIdRead; // Not stable
            pTrgIdDet->m_StableCount = 0;
        }
        
        // Stable?
        if (pTrgIdDet->m_StableCount++ > 3)
        {
            if (TargetIdRead == GROUNDED)
            {
                NewTdState = DTA_TD_SPI_SINK;
                pTrgIdDet->m_Present = DTA_DVB_SPI_SINK;
                DtaTargetIdSetWindowForGrounded(pRegBase);
            } else if (TargetIdRead == OPEN_CIRCUIT)
            {
                NewTdState = DTA_TD_OPEN_CIRCUIT;
                pTrgIdDet->m_Present = DTA_NO_CONNECTION;
                DtaTargetIdSetWindowForOpenCircuit(pRegBase);
            } else {
                NewTdState = DTA_TD_TARGETID_STABLE;
                pTrgIdDet->m_StableCount = 0;
                pTrgIdDet->m_Present = DTA_TARGET_PRESENT;
                pTrgIdDet->m_TargetId = TargetIdRead;
                DtaTargetIdSetWindow(pRegBase);
            }
        }
        break;

    case DTA_TD_OPEN_CIRCUIT:
        if (DtaRegThresHCtrlGetCmpA(pRegBase) == 0)
                                // Window crossed?
            goto to_find;
        break;

    case DTA_TD_SPI_SINK:
        if (DtaRegThresHCtrlGetCmpA(pRegBase) != 0)
                                // Window crossed?
            goto to_find;
        break;

    case DTA_TD_TARGETID_STABLE:
        if (DtaRegThresHCtrlGetCmpB(pRegBase) != 0)
                                // Window crossed (high)?
            goto to_find;
        else if (DtaRegThresHCtrlGetCmpA(pRegBase) == 0)
                                // Window crossed (low)?
        {
            if (pTrgIdDet->m_StableCount++ > 1) 
            {                   // Stable?
                DtaTargetIdReadId(pRegBase, &TargetIdRead);
                if (TargetIdRead == 0) 
                {
                    NewTdState = DTA_TD_TARGET_ERROR;
                    DtaTargetIdSetWindowForGrounded(pRegBase);
                    // Note: target-error flag is cleared in DtaTxProcessStatusFlags
                    pTrgIdDet->m_TargetError = TRUE;
                } else
                    goto to_find;
            }
        } else
            pTrgIdDet->m_StableCount = 0;
        break;

    case DTA_TD_TARGET_ERROR:
        // Window crossed?
        if (DtaRegThresHCtrlGetCmpA(pRegBase) != 0)
        {
            DtaTargetIdReadId(pRegBase, &TargetIdRead);
            if (TargetIdRead == pTrgIdDet->m_TargetId)
            {
                NewTdState = DTA_TD_TARGETID_STABLE;
                pTrgIdDet->m_StableCount = 0;
                DtaTargetIdSetWindow(pRegBase);
            } else
                goto to_find;
        }
        break;
    }

    if (NewTdState != pTrgIdDet->m_TdState)
    {
        StrState = NULL;
        switch (NewTdState)
        {
        case DTA_TD_FIND_TARGETID:   StrState = "DTA_TD_FIND_TARGETID";   break;
        case DTA_TD_SPI_SINK:        StrState = "DTA_TD_SPI_SINK";        break;
        case DTA_TD_OPEN_CIRCUIT:    StrState = "DTA_TD_OPEN_CIRCUIT";    break;
        case DTA_TD_TARGETID_STABLE: StrState = "DTA_TD_TARGETID_STABLE"; break;
        case DTA_TD_TARGET_ERROR:    StrState = "DTA_TD_TARGET_ERROR";    break;
        }
        if (StrState == NULL)
            DtDbgOut(MAX, TRGDET, "Target detection, state := UNKNOWN (%d)", NewTdState);
        else
            DtDbgOut(MAX, TRGDET, "Target detection, state := %s", StrState);
    }
    pTrgIdDet->m_TdState = NewTdState;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaTargetIdReadId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtaTargetIdReadId(
    volatile UInt8*  pRegBase,  // Channel register base
    Int*  pTargetId)            // Target ID 1..35; 0=GROUNDED; 36=OPEN_CIRCUIT
{
    Int  AtoDVal, TargetId;

    DtaTargetIdSenseAtoDUsingDacA(pRegBase, &AtoDVal);
    TargetId = AtoDVal / 6;
    if (TargetId > OPEN_CIRCUIT)
        TargetId = OPEN_CIRCUIT;
    *pTargetId = TargetId;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaTargetIdSenseAtoDUsingDacA -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Emulate A-to-D conversion on sense input, by performing a binary search with the DAC
// and monitoring the comparator output. Uses DAC A.
//
void  DtaTargetIdSenseAtoDUsingDacA(
    volatile UInt8*  pRegBase,  // Channel register base
    Int*  pAdcValue)            // Measured value for voltage on sense input
                                // 0=0V ... 255=5V
{
    Int  Mask;
    Int  Val = 0;

    for (Mask=0x80; Mask!=0; Mask>>=1)
    {
        DtaRegThresHCtrlSetThresHoldA(pRegBase, Val | Mask);
        DtaTargetIdWaitForUpdateDone(pRegBase);
        DtWaitBlock(8);         // Wait 8us (max DAC settling time)
        if (DtaRegThresHCtrlGetCmpA(pRegBase) != 0)
            Val |= Mask;
    }
    *pAdcValue = Val;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaTargetIdSenseAtoDUsingDacB -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Emulate A-to-D conversion on sense input, by performing a binary search with the DAC
// and monitoring the comparator output. Uses DAC B.
//
void  DtaTargetIdSenseAtoDUsingDacB(
    volatile UInt8*  pRegBase,  // Channel register base
    Int*  pAdcValue)            // Measured value for voltage on sense input
                                // 0=0V ... 255=5V
{
    Int  Mask;
    Int  Val = 0;

    for (Mask=0x80; Mask!=0; Mask>>=1)
    {
        DtaRegThresHCtrlSetThresHoldB(pRegBase, Val | Mask);
        DtaTargetIdWaitForUpdateDone(pRegBase);
        DtWaitBlock(8);         // Wait 8us (max DAC settling time)
        if (DtaRegThresHCtrlGetCmpB(pRegBase) != 0)
            Val |= Mask;
    }

    // Return result in pAdcValue
    *pAdcValue = Val;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaTargetIdSetWindow -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set a window of +/- 3 LSBs around currently measured voltage on sense pin.
// Comparator A implements lower threshold. 1=ok, 0=below threshold.
// Comparator B implements upper threshold. 0=ok, 1=above threshold.
//
void  DtaTargetIdSetWindow(
    volatile UInt8*  pRegBase)  // Channel register base
{
    Int  ValDacA, ValDacB, AbsDiff;

    DtaTargetIdSenseAtoDUsingDacA(pRegBase, &ValDacA);
    DtaTargetIdSenseAtoDUsingDacB(pRegBase, &ValDacB);

    // For robustness:
    AbsDiff = (ValDacA>ValDacB ? ValDacA-ValDacB : ValDacB-ValDacA);
    if (AbsDiff > 4) ValDacA = ValDacB;
    if (ValDacA < 5) ValDacA = 5;
    if (ValDacB < 5) ValDacB = 5;
    if (ValDacA > 240) ValDacA = 240;
    if (ValDacB > 240) ValDacB = 240;

    // Set window
    DtaRegThresHCtrlSetThresHoldA(pRegBase, ValDacB - 3);
    DtaTargetIdWaitForUpdateDone(pRegBase);

    DtaRegThresHCtrlSetThresHoldB(pRegBase, ValDacB + 3);
    DtaTargetIdWaitForUpdateDone(pRegBase);

    DtDbgOut(MAX, TRGDET, "A-threshold=%d, B-threshold=%d",
                                                                    ValDacA-3, ValDacB+3);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaTargetIdSetWindowForGrounded -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Set comparator A, such that 0=GROUNDED, 1=not grounded anymore
//
void  DtaTargetIdSetWindowForGrounded(
    volatile UInt8*  pRegBase)  // Channel register base
{
    DtaRegThresHCtrlSetThresHoldA(pRegBase, 6);
    DtaTargetIdWaitForUpdateDone(pRegBase);
    DtDbgOut(MAX, TRGDET, "DtaTargetIdSetWindowForGrounded A-threshold=6");
}


//.-.-.-.-.-.-.-.-.-.-.-.-.- DtaTargetIdSetWindowForOpenCircuit -.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Set comparator A, such that 1=OPEN_CIRCUIT, 0=something connected
//
void  DtaTargetIdSetWindowForOpenCircuit(
    volatile UInt8* pRegBase)  // Channel register base
{
    DtaRegThresHCtrlSetThresHoldA(pRegBase, OPEN_CIRCUIT*6);
    DtaTargetIdWaitForUpdateDone(pRegBase);
    DtDbgOut(MAX, TRGDET, "Dta: DtaSetWindowForOpenCircuit A-threshold=%d",
                                                                          OPEN_CIRCUIT*6);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaTargetIdWaitForUpdateDone -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Wait until DAC has been updated. Don't wait too long, to avoid system crash if
// something is wrong with the DAC...
//
void  DtaTargetIdWaitForUpdateDone(
    volatile UInt8*  pRegBase)   // Channel register base
{
    Int  TimeOut = 0;
    // Busy wait until busy bit is cleared
    while (DtaRegThresHCtrlGetUpdateBsy(pRegBase)!=0 && TimeOut++<100);
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Detect video standard +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpDetectVidStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpDetectVidStd(
    DtaNonIpPort*  pNonIpPort,
    Int*  pVidStd,
    UInt*  pVpid,
    UInt*  pVpid2,
    Int*  pAspectRatio)
{
    DT_ASSERT(pNonIpPort != NULL);

    // Init to 'safe' value
    *pVidStd = DT_VIDSTD_UNKNOWN;
    if (pVpid != NULL)
        *pVpid = 0;
    if (pVpid2 != NULL)
        *pVpid2 = 0;
    if (pAspectRatio != NULL)
        *pAspectRatio = DTA_AR_UNKNOWN;

    // Port must be configured as an input except for HDMI and SDI-RX ports
    if ((pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value!=DT_IOCONFIG_INPUT || 
                     pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_SubValue!=DT_IOCONFIG_INPUT)
                     && !pNonIpPort->m_CapHdmi && !pNonIpPort->m_CapSdiRx)
    {
        DtDbgOut(ERR, NONIP, "Port %d is not configured as input", 
                                                               pNonIpPort->m_PortIndex+1);
        return DT_STATUS_FAIL;
    }

    // For matrix capable ports forward to matrix function
    if (pNonIpPort->m_CapMatrix)
    {
        DT_RETURN_ON_ERROR(DtaNonIpMatrixDetectVidStd(pNonIpPort, pVidStd));
        if (pVpid!=NULL && pVpid2!=NULL)
            DT_RETURN_ON_ERROR(DtaMatrixGetVpid(pNonIpPort, pVpid, pVpid2));
        return DT_STATUS_OK;
    }
    else if (pNonIpPort->m_CapSdiRx)
        return DtaNonIpSdiAvRxDetectVidStd(pNonIpPort, pVidStd, pVpid, pVpid2);
    else if (pNonIpPort->m_CapHdmi)
        return DtaNonIpHdmiDetectVidStd(pNonIpPort, pVidStd, pAspectRatio);
    else
    {
        // For now this function is not supported for any other boards
        DtDbgOut(ERR, NONIP, "Not supported for this port %d", pNonIpPort->m_PortIndex+1);
        return DT_STATUS_NOT_SUPPORTED;  
    }
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpGetAudioStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpGetAudioStatus(
    DtaNonIpPort*  pNonIpPort,
    DtaIoctlNonIpCmdGetAudioStatusOutput*  pOut)
{
    DT_ASSERT(pNonIpPort != NULL);
    if (pNonIpPort->m_CapSdiRx)
        return DtaNonIpSdiAvRxGetAudioStatus(pNonIpPort, pOut);
    else if (pNonIpPort->m_CapHdmi)
        return DtaNonIpHdmiGetAudioStatus(pNonIpPort, pOut);
    else
    {
        // For now this function is not supported for any other boards
        DtDbgOut(ERR, NONIP, "Not supported for this port %d", pNonIpPort->m_PortIndex+1);
        return DT_STATUS_NOT_SUPPORTED;  
    }
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ GENREF properties +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpGetGenRefProps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpGetGenRefProps(DtaNonIpPort* pNonIpPort, 
                                                        DtaIoctlNonIpGenRefProps*  pProps)
{
    DtaGenlock*  pGenlock=NULL;

    DT_ASSERT(pNonIpPort != NULL);
    DT_ASSERT(pProps != NULL);

    // Port must be GENREF capable
    if (!pNonIpPort->m_CapGenRef)
    {
        DtDbgOut(ERR, NONIP, "Port %d is not GENREF capable", pNonIpPort->m_PortIndex+1);
        return DT_STATUS_NOT_SUPPORTED;
    }
    // Port must be configured as GENREF
    if (pNonIpPort->m_IoCfg[DT_IOCONFIG_GENREF].m_Value != DT_IOCONFIG_TRUE)
    {
        DtDbgOut(ERR, NONIP, "Port %d is not configured as GENREF", 
                                                               pNonIpPort->m_PortIndex+1);
        return DT_STATUS_CONFIG_ERROR;
    }
    pGenlock = &pNonIpPort->m_pDvcData->m_Genlock;
    DT_ASSERT(pGenlock!=NULL && pGenlock->m_IsSupported);
    
    // Set the port index of the GENREF port (is the port itself)
    pProps->m_RefPortIndex = pNonIpPort->m_PortIndex;
    // Set type
    if (pNonIpPort->m_PortIndex == pGenlock->m_AsyncPortIndex)
        pProps->m_RefPortType = DTA_GENREF_PORTTYPE_ANALOG;
    else if (pNonIpPort->m_PortIndex == pGenlock->m_IntGenrefPortIndex)
        pProps->m_RefPortType = DTA_GENREF_PORTTYPE_INTERNAL;
    else
        pProps->m_RefPortType = DTA_GENREF_PORTTYPE_DIGITAL;

    // Get the group of port under control of this controller
    pProps->m_Group = pGenlock->m_PortGroup;

    // Ref- and out-video-standard
    pProps->m_RefVidStd = pGenlock->m_RefVidStd;
    pProps->m_OutVidStd = pGenlock->m_OutVidStd;
    //TOF alignment offset
    pProps->m_TofAlignOffsetNs = pGenlock->m_TofAlignOffsetNs;
                
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpNotifyGenRefProp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpNotifyGenRefProp(DtaNonIpPort* pNonIpPort, 
                                                        DtaIoctlNonIpGenRefProps*  pProps)
{
    DtaDeviceData*  pDvcData = NULL;

    DT_ASSERT(pNonIpPort != NULL);
    DT_ASSERT(pProps != NULL);

    pDvcData = pNonIpPort->m_pDvcData;
    DT_ASSERT(pDvcData != NULL);
    
    // Must be a genlock and output capable port
    if (!pNonIpPort->m_CapGenLocked || !pNonIpPort->m_CapOutput)
    {
        DtDbgOut(ERR, NONIP, "Not supported for this port %d", pNonIpPort->m_PortIndex+1);
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Update the TOF alignment offset 
    pNonIpPort->m_TofAlignOffsetNs = pProps->m_TofAlignOffsetNs;

    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ IO configuration +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpUpdateSdiModes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Helper function to update the SDI line mode and SDI enable after switching the 
// direction of a Bi-dir port or switching from ASI to SDI mode
//
DtStatus  DtaNonIpUpdateSdiModes(
    DtaNonIpPort*  pNonIpPort,
    Bool  IsInput,
    Bool  IsSdi)
{
    // Do nothing for ports that support matrix API interface
    if (pNonIpPort->m_CapMatrix)
        return DT_STATUS_OK;
    
    if (IsInput)
    {
        // Update TS Stream format
        if (IsSdi)
            DtaRegRxCtrlSetSdiMode(pNonIpPort->m_pRxRegs, 1);
        else
            DtaRegRxCtrlSetSdiMode(pNonIpPort->m_pRxRegs, 0);
    } else {   
        // Update TS Stream format
        if (IsSdi)
        {
            DtaRegTxCtrlSetSdiMode(pNonIpPort->m_pTxRegs, 1);

            // Update SDI-line mode
            if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue==DT_IOCONFIG_525I59_94
                    || pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD].m_SubValue
                                                              == DT_IOCONFIG_SPI525I59_94)
                DtaRegTxCtrlSetSdiLineMode(pNonIpPort->m_pTxRegs, 1);
            else
                DtaRegTxCtrlSetSdiLineMode(pNonIpPort->m_pTxRegs, 0);   
        } else
            DtaRegTxCtrlSetSdiMode(pNonIpPort->m_pTxRegs, 0);
    }
    return DT_STATUS_OK;
}
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIoConfigGet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpIoConfigGet(
    DtaNonIpPort*  pNonIpPort,
    Int  Code,
    DtaIoConfigValue*  pCfgValue)
{
    *pCfgValue = pNonIpPort->m_IoCfg[Code];
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIoConfigSet -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpIoConfigSet(
    DtaNonIpPort*  pNonIpPort,
    Int  Group,
    DtaIoConfigValue  CfgValue)
{
    DtStatus  Status = DT_STATUS_OK;
    
    switch (Group)
    {
        // IO-direction
    case DT_IOCONFIG_IODIR:
        Status = DtaNonIpIoConfigSetIoDir(pNonIpPort, Group, CfgValue);
        break;

         // I/O standard
    case DT_IOCONFIG_IOSTD:
        Status = DtaNonIpIoConfigSetIoStd(pNonIpPort, Group, CfgValue);
        break;
    
        // RF clock source selection
    case DT_IOCONFIG_RFCLKSEL:
        Status = DtaNonIpIoConfigSetRfClkSel(pNonIpPort, Group, CfgValue);
        break;

        // Parallel port clock source selection
    case DT_IOCONFIG_SPICLKSEL:
        Status = DtaNonIpIoConfigSetSpiClkSel(pNonIpPort, Group, CfgValue);
        break;

        // Parallel port mode
    case DT_IOCONFIG_SPIMODE:
        Status = DtaNonIpIoConfigSetSpiMode(pNonIpPort, Group, CfgValue);
        break;
    
        // Parallel port I/O standard
    case DT_IOCONFIG_SPISTD:
        Status = DtaNonIpIoConfigSetSpiStd(pNonIpPort, Group, CfgValue);
        break;

        // Transport-stream rate selection
    case DT_IOCONFIG_TSRATESEL:
        Status = DtaNonIpIoConfigSetTsRateSel(pNonIpPort, Group, CfgValue);
        break;
         
        // Fail-over relais available
    case DT_IOCONFIG_FAILSAFE:
        Status = DtaNonIpIoConfigSetFailSafe(pNonIpPort, Group, CfgValue);
        break;

        // Locked to a genlock reference
    case DT_IOCONFIG_GENLOCKED:
        Status = DtaNonIpIoConfigSetGenLocked(pNonIpPort, Group, CfgValue);
        break;

        // Genlock reference
    case DT_IOCONFIG_GENREF:
        Status = DtaNonIpIoConfigSetGenRef(pNonIpPort, Group, CfgValue);
        break;

        // Fractional mode
    case DT_IOCONFIG_FRACMODE:
        Status = DtaNonIpIoConfigSetFracMode(pNonIpPort, Group, CfgValue);
        break;

        // DVB-S2 APSK mode
    case DT_IOCONFIG_SWS2APSK:
        Status = DtaNonIpIoConfigSetSwS2Apsk(pNonIpPort, Group, CfgValue);
        break;

    default:
        DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
        Status = DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIoConfigSetIoDir -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNonIpIoConfigSetIoDir(
    DtaNonIpPort* pNonIpPort,
    Int Group,
    DtaIoConfigValue CfgValue)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoConfigValue  OldCfgValue = pNonIpPort->m_IoCfg[Group];
    Bool  ReApplyIoStd = FALSE;

    switch (CfgValue.m_Value)
    {
    case DT_IOCONFIG_DISABLED:
        // New value (DT_IOCONFIG_DISABLED) is saved at the end...
        break;

    case DT_IOCONFIG_INTINPUT:
        // New value (DT_IOCONFIG_INTINPUT) is saved at the end...
        break;

    case DT_IOCONFIG_MONITOR:
        // New value (DT_IOCONFIG_MONITOR) is saved at the end...
        break;

    case DT_IOCONFIG_INPUT:
        DT_ASSERT(pNonIpPort->m_CapInput);
        if (pNonIpPort->m_IsBidir)
        {
            if (pNonIpPort->m_CapSdi || pNonIpPort->m_CapSpiSdi)
                ReApplyIoStd = TRUE;    // Must Re-apply IOSTD when IODIR changes
        }

        switch (CfgValue.m_SubValue)
        {
        case DT_IOCONFIG_INPUT:
            if (pNonIpPort->m_IsBidir)
            {
                // Calculate fifo offset
                pNonIpPort->m_FifoOffset = pNonIpPort->m_RxRegsOffset +
                                                                   DTA_LOCALADDR_FIFODATA;

                // Check for Matrix-API interface
                if (pNonIpPort->m_CapMatrix)
                {
                    // Save new config, before apply-ing
                    pNonIpPort->m_IoCfg[Group] = CfgValue;
                    Status =  DtaNonIpMatrixConfigure(pNonIpPort, FALSE);
                    // Restore original config, if failed to apply new one
                    if (!DT_SUCCESS(Status))
                        pNonIpPort->m_IoCfg[Group] = OldCfgValue;
                }
                else
                {
                    // Disable output enable on bidir ports
                    DtaRegTxCtrlSetOutputEn(pNonIpPort->m_pTxRegs, 0); 
                }
            }
                        
            break;

        case DT_IOCONFIG_SHAREDANT:
            // No actions required. It will be set by DtapiService through I2c.  
            break;

        default:
            DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
            return DT_STATUS_NOT_SUPPORTED;
        }
        break;

    case DT_IOCONFIG_OUTPUT:
        DT_ASSERT(pNonIpPort->m_CapOutput);

        // For non-Matrix ports: route output to ourselves
        if (!pNonIpPort->m_CapMatrix)
        {
            if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber==105
                                && pNonIpPort->m_pDvcData->m_DevInfo.m_FirmwareVersion==0)
                DtaRegControl0SetDblBufEnable(pNonIpPort->m_pDvcData->m_pGenRegs, 0);
            else
                DtaRegTxCtrl2SetSrcSel(pNonIpPort->m_pTxRegs, pNonIpPort->m_PortIndex);
        }

        // Enable output enable on bidir ports
        if (pNonIpPort->m_IsBidir)
        {
            // Calculate fifo offset
            pNonIpPort->m_FifoOffset = pNonIpPort->m_TxRegsOffset+DTA_LOCALADDR_FIFODATA;

            // Check for Matrix-API interface
            if (pNonIpPort->m_CapMatrix)
            {
                // Save new config, before apply-ing
                pNonIpPort->m_IoCfg[Group] = CfgValue;
                Status =  DtaNonIpMatrixConfigure(pNonIpPort, FALSE);
                // Restore original config, if failed to apply new one
                if (!DT_SUCCESS(Status))
                    pNonIpPort->m_IoCfg[Group] = OldCfgValue;
            }
            else
            {
                // Enable output enable on bidir ports
                DtaRegTxCtrlSetOutputEn(pNonIpPort->m_pTxRegs, 1);
            }

            if (pNonIpPort->m_CapSdi || pNonIpPort->m_CapSpiSdi)
                ReApplyIoStd = TRUE;    // Must Re-apply IOSTD when IODIR changes
        }
                
        switch (CfgValue.m_SubValue)
        {
        case DT_IOCONFIG_OUTPUT:
            // No further actions
            break;

        case DT_IOCONFIG_DBLBUF:
            DT_ASSERT(pNonIpPort->m_CapDblBuf);
            
            // For matrix port, nothing TODO. All work was done in DtaNonIpMatrixConfigure 
            // call above
            if (pNonIpPort->m_CapMatrix)
                break;  

            // Route output to double-buffered-buddy
            if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber==105
                                && pNonIpPort->m_pDvcData->m_DevInfo.m_FirmwareVersion==0)
                DtaRegControl0SetDblBufEnable(pNonIpPort->m_pDvcData->m_pGenRegs, 1);
            else
                DtaRegTxCtrl2SetSrcSel(pNonIpPort->m_pTxRegs,(UInt)CfgValue.m_ParXtra[0]);
            break;

        case DT_IOCONFIG_LOOPS2L3:
            DT_ASSERT(pNonIpPort->m_CapLoopThr);
            // Route output to loop-through-buddy
            DtaRegTxCtrl2SetSrcSel(pNonIpPort->m_pTxRegs, (UInt)CfgValue.m_ParXtra[0]);

            // Set S2LoopMode to L3
            DtaRegTxCtrl2SetS2Loopmode(pNonIpPort->m_pTxRegs, 1);
            break;

        case DT_IOCONFIG_LOOPS2TS:
            DT_ASSERT(pNonIpPort->m_CapLoopThr);
            // Route output to loop-through-buddy
            DtaRegTxCtrl2SetSrcSel(pNonIpPort->m_pTxRegs, (UInt)CfgValue.m_ParXtra[0]);

            // Set S2LoopMode to TS
            DtaRegTxCtrl2SetS2Loopmode(pNonIpPort->m_pTxRegs, 0);
            // Select ISI
            DtaRegTxCtrl2SetS2ISI(pNonIpPort->m_pTxRegs, (UInt)CfgValue.m_ParXtra[1]);
            break;

        case DT_IOCONFIG_LOOPTHR:
            DT_ASSERT(pNonIpPort->m_CapLoopThr);
            // Route output to loop-through-buddy
            DtaRegTxCtrl2SetSrcSel(pNonIpPort->m_pTxRegs, (UInt)CfgValue.m_ParXtra[0]);
            break;

        default:
            DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
            return DT_STATUS_NOT_SUPPORTED;
        }
        break;
    default:
        DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Do we need to re-apply IOSTD setting
    if (ReApplyIoStd)
    {
        // Store current value
        OldCfgValue = pNonIpPort->m_IoCfg[Group];
        // Save new IO-dir as IOSTD depends on it
        pNonIpPort->m_IoCfg[Group] = CfgValue;

        // Re-apply IO-std
        Status = DtaNonIpIoConfigSetIoStd(pNonIpPort, DT_IOCONFIG_IOSTD, 
                                                  pNonIpPort->m_IoCfg[DT_IOCONFIG_IOSTD]);

        // If re-applying of IOSTD failed => restore original IODIR
        if (!DT_SUCCESS(Status))
            pNonIpPort->m_IoCfg[Group] = OldCfgValue;
    }
    else
        pNonIpPort->m_IoCfg[Group] = CfgValue;  // Save new config to the cache

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIoConfigSetIoStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNonIpIoConfigSetIoStd(
    DtaNonIpPort* pNonIpPort,
    Int Group,
    DtaIoConfigValue CfgValue)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool  ForceConfig=FALSE, IsSdiIoConfig=FALSE, IsTheGenref=FALSE;
    DtaIoConfigValue  OldCfgValue = pNonIpPort->m_IoCfg[Group];
    DtaIoConfigValue  IoDirValue;
    DtaNonIpIoConfigGet(pNonIpPort, DT_IOCONFIG_IODIR, &IoDirValue);

    switch (CfgValue.m_Value)
    {
    case DT_IOCONFIG_ASI:
    case DT_IOCONFIG_SPI:
        DT_ASSERT(pNonIpPort->m_CapAsi || pNonIpPort->m_CapSpi);

        // Configuration for Matrix capable ports
        if (pNonIpPort->m_CapMatrix)
        {
            // Force a reconfig if IOSTD was not already ASI
            ForceConfig = (OldCfgValue.m_Value != DT_IOCONFIG_ASI);

            // Save new config, before apply-ing
            pNonIpPort->m_IoCfg[Group] = CfgValue;
            Status =  DtaNonIpMatrixConfigure(pNonIpPort, ForceConfig);
            // Restore original config, if failed to apply new one
            if (!DT_SUCCESS(Status))
                pNonIpPort->m_IoCfg[Group] = OldCfgValue;   

            break;
        }
        
        // "Normal ASI" ports
        if (IoDirValue.m_Value == DT_IOCONFIG_OUTPUT)
            DtaRegTxCtrlSetSdiMode(pNonIpPort->m_pTxRegs, 0);
        else if (IoDirValue.m_Value == DT_IOCONFIG_INPUT)
            DtaRegRxCtrlSetSdiMode(pNonIpPort->m_pRxRegs, 0);
        break;

    case DT_IOCONFIG_3GSDI:
        DT_ASSERT(pNonIpPort->m_Cap3GSdi);
        IsSdiIoConfig = TRUE;

        if (!pNonIpPort->m_CapMatrix)
            break;  // Nothing to do for none matrix ports

        // Force a reconfig if IOSTD was not already 3G-SDI
        ForceConfig = (OldCfgValue.m_Value != DT_IOCONFIG_3GSDI);

        // Save new config, before apply-ing
        pNonIpPort->m_IoCfg[Group] = CfgValue;
        Status =  DtaNonIpMatrixConfigure(pNonIpPort, ForceConfig);
        // Restore original config, if failed to apply new one
        if (!DT_SUCCESS(Status))
            pNonIpPort->m_IoCfg[Group] = OldCfgValue;   
        break;

    case DT_IOCONFIG_HDSDI:
        DT_ASSERT(pNonIpPort->m_CapHdSdi);
        IsSdiIoConfig = TRUE;
        
        if (!pNonIpPort->m_CapMatrix)
            break;  // Nothing to do for none matrix ports
        
        // Force a reconfig if IOSTD was not already HD-SDI
        ForceConfig = (OldCfgValue.m_Value != DT_IOCONFIG_HDSDI);

        // Save new config, before apply-ing
        pNonIpPort->m_IoCfg[Group] = CfgValue;
        Status =  DtaNonIpMatrixConfigure(pNonIpPort, ForceConfig);
        // Restore original config, if failed to apply new one
        if (!DT_SUCCESS(Status))
            pNonIpPort->m_IoCfg[Group] = OldCfgValue;   
        break;

    case DT_IOCONFIG_SDI:
        DT_ASSERT(pNonIpPort->m_CapSdi);
        IsSdiIoConfig = TRUE;

        if (pNonIpPort->m_CapMatrix)
        {
            // Force a reconfig if IOSTD was not already SDI
            ForceConfig = (OldCfgValue.m_Value != DT_IOCONFIG_SDI);

            // Save new config, before apply-ing
            pNonIpPort->m_IoCfg[Group] = CfgValue;
            Status =  DtaNonIpMatrixConfigure(pNonIpPort, ForceConfig);
            // Restore original config, if failed to apply new one
            if (!DT_SUCCESS(Status))
                pNonIpPort->m_IoCfg[Group] = OldCfgValue;   
            break;
        }
        if (pNonIpPort->m_IsNonFuntional)
            break; // Prevent register writes on ports without RX/TX registers

        DtaNonIpUpdateSdiModes(pNonIpPort, IoDirValue.m_Value==DT_IOCONFIG_INPUT, TRUE);
        switch (CfgValue.m_SubValue)
        {
        case DT_IOCONFIG_525I59_94:
            DT_ASSERT(pNonIpPort->m_Cap525I59_94);
            DT_ASSERT(IoDirValue.m_Value == DT_IOCONFIG_OUTPUT ||
                                                 IoDirValue.m_Value == DT_IOCONFIG_INPUT);

            // Set 525 line mode
            // Get base register pointer
            if (IoDirValue.m_Value == DT_IOCONFIG_OUTPUT )
                DtaRegTxCtrlSetSdiLineMode(pNonIpPort->m_pTxRegs, 1);
            break;

        case DT_IOCONFIG_625I50:
            DT_ASSERT(pNonIpPort->m_Cap625I50);
            DT_ASSERT(IoDirValue.m_Value == DT_IOCONFIG_OUTPUT ||
                IoDirValue.m_Value == DT_IOCONFIG_INPUT);
            // Set 625 line mode
            if (IoDirValue.m_Value == DT_IOCONFIG_OUTPUT)
                DtaRegTxCtrlSetSdiLineMode(pNonIpPort->m_pTxRegs, 0);
            break;

        default:
            DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
            return DT_STATUS_NOT_SUPPORTED;
        }
        break;

    case DT_IOCONFIG_SPISDI:
        DT_ASSERT(pNonIpPort->m_CapSpiSdi);
        IsSdiIoConfig = TRUE;

        // For devices with matrix API interface there is nothing to do
        if (pNonIpPort->m_CapMatrix)
            break;

        // TOOD: verify if this call is required for SPISDI (DTA-2142)
        DtaNonIpUpdateSdiModes(pNonIpPort, IoDirValue.m_Value==DT_IOCONFIG_INPUT, TRUE);
        switch (CfgValue.m_SubValue)
        {
        case DT_IOCONFIG_SPI525I59_94:
            DT_ASSERT(pNonIpPort->m_CapSpi525I59_94);
            DT_ASSERT(IoDirValue.m_Value == DT_IOCONFIG_OUTPUT ||
                                                 IoDirValue.m_Value == DT_IOCONFIG_INPUT);

            // Set 525 line mode
            // Get base register pointer
            if (IoDirValue.m_Value == DT_IOCONFIG_OUTPUT )
                DtaRegTxCtrlSetSdiLineMode(pNonIpPort->m_pTxRegs, 1);
            break;

        case DT_IOCONFIG_SPI625I50:
            DT_ASSERT(pNonIpPort->m_CapSpi625I50);
            DT_ASSERT(IoDirValue.m_Value == DT_IOCONFIG_OUTPUT ||
                                                 IoDirValue.m_Value == DT_IOCONFIG_INPUT);
            // Set 625 line mode
            if (IoDirValue.m_Value == DT_IOCONFIG_OUTPUT)
                DtaRegTxCtrlSetSdiLineMode(pNonIpPort->m_pTxRegs, 0);
            break;

        default:
            DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
            return DT_STATUS_NOT_SUPPORTED;
        }
        break;

    case DT_IOCONFIG_AVENC:
    case DT_IOCONFIG_DEMOD:
    case DT_IOCONFIG_GPSTIME:
    case DT_IOCONFIG_HDMI:
    case DT_IOCONFIG_IFADC:
    case DT_IOCONFIG_IP:
    case DT_IOCONFIG_MOD:
    case DT_IOCONFIG_PHASENOISE:
    case DT_IOCONFIG_RS422:
    case DT_IOCONFIG_SDIRX:
        // No action required
        break;

    default:
        DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
        return DT_STATUS_NOT_SUPPORTED;
    }

    pNonIpPort->m_IoCfg[Group] = CfgValue;  // Save new config to the cache

    // If the port is a genlock reference we need to (re-)init the genlock engine to
    // account for IO standard change (i.e. reference format change)
    IsTheGenref = pNonIpPort->m_CapGenRef && 
                      (pNonIpPort->m_IoCfg[DT_IOCONFIG_GENREF].m_Value==DT_IOCONFIG_TRUE);
    if (IsTheGenref && IsSdiIoConfig)
    {
        // Apply genref configuration
        Status = DtaGenlockApplyGenRefConfig(pNonIpPort->m_pDvcData);
        if (!DT_SUCCESS(Status))
            DtDbgOut(ERR, NONIP, "Failed to apply genref config after io-std change");
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIoConfigSetRfClkSel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNonIpIoConfigSetRfClkSel(
    DtaNonIpPort* pNonIpPort,
    Int Group,
    DtaIoConfigValue CfgValue)
{
    switch (CfgValue.m_Value)
    {
    case DT_IOCONFIG_RFCLKINT:
        // No actions required if only internal RF clock is supported 
        DT_ASSERT(pNonIpPort->m_CapRfClkInt);
        if (!pNonIpPort->m_CapRfClkExt)
            break;
        // Select internal clock
        DtaRegRfCtrl3SetRfClkSel(pNonIpPort->m_pRfRegs, 1);
        break;
    case DT_IOCONFIG_RFCLKEXT:
        DT_ASSERT(pNonIpPort->m_CapRfClkExt);
        // Select external clock
        DtaRegRfCtrl3SetRfClkSel(pNonIpPort->m_pRfRegs, 0);
        break;
    default:
        DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
        return DT_STATUS_NOT_SUPPORTED;
    }

    pNonIpPort->m_IoCfg[Group] = CfgValue;  // Save new config to the cache

    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIoConfigSetSpiClkSel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNonIpIoConfigSetSpiClkSel(
    DtaNonIpPort* pNonIpPort,
    Int Group,
    DtaIoConfigValue CfgValue)
{
    DT_ASSERT(pNonIpPort->m_CapSpi);
    
    switch (CfgValue.m_Value)
    {
    case DT_IOCONFIG_SPICLKINT:
        DT_ASSERT(pNonIpPort->m_CapSpiClkInt);
        // No actions required if only internal SPI clock is  supported 
        if (!pNonIpPort->m_CapSpiClkExt)
            break;
        // Select internal clock
        DtaRegDvbSpiCtrlSetExtClkSelect(pNonIpPort->m_pSpiRegs, 0);

        break;
    case DT_IOCONFIG_SPICLKEXT:
        DT_ASSERT(pNonIpPort->m_CapSpiClkExt);
        // Select external clock
        DtaRegDvbSpiCtrlSetExtClkSelect(pNonIpPort->m_pSpiRegs, 1);
        break;
    default:
        DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
        return DT_STATUS_NOT_SUPPORTED;
    }

    pNonIpPort->m_IoCfg[Group] = CfgValue;  // Save new config to the cache

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIoConfigSetSpiMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpIoConfigSetSpiMode(
    DtaNonIpPort* pNonIpPort,
    Int Group,
    DtaIoConfigValue CfgValue)
{
    DT_ASSERT(pNonIpPort->m_CapSpi);
    switch (CfgValue.m_Value)
    {
    case DT_IOCONFIG_SPIDVBMODE:
        DT_ASSERT(pNonIpPort->m_CapSpiDvbMode);
        // No actions required if only DVBMODE SPI mode is  supported 
        if (!pNonIpPort->m_CapSpiFixedClk && !pNonIpPort->m_CapSpiSer8B
                                                           && !pNonIpPort->m_CapSpiSer10B)
            break;
        DtaRegDvbSpiCtrlSetMode10B(pNonIpPort->m_pSpiRegs, 0);
        DtaRegDvbSpiCtrlSetSerialMode(pNonIpPort->m_pSpiRegs, 0);
        break;
    case DT_IOCONFIG_SPIFIXEDCLK:
        DT_ASSERT(pNonIpPort->m_CapSpiFixedClk);
        DtaRegDvbSpiCtrlSetMode10B(pNonIpPort->m_pSpiRegs, 0);
        DtaRegDvbSpiCtrlSetSerialMode(pNonIpPort->m_pSpiRegs, 0);
        break;
    case DT_IOCONFIG_SPISER8B:
        DT_ASSERT(pNonIpPort->m_CapSpiSer8B);
        DtaRegDvbSpiCtrlSetMode10B(pNonIpPort->m_pSpiRegs, 0);
        DtaRegDvbSpiCtrlSetSerialMode(pNonIpPort->m_pSpiRegs, 1);
        break;
    case DT_IOCONFIG_SPISER10B:
        DT_ASSERT(pNonIpPort->m_CapSpiSer10B);
        DtaRegDvbSpiCtrlSetMode10B(pNonIpPort->m_pSpiRegs, 1);
        DtaRegDvbSpiCtrlSetSerialMode(pNonIpPort->m_pSpiRegs, 1);
        break;
    default:
        DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
        return DT_STATUS_NOT_SUPPORTED;
    }
    DtaRegDvbSpiCtrlSetTestMode(pNonIpPort->m_pSpiRegs, 0);
    DtaRegDvbSpiCtrlSetPrngMode(pNonIpPort->m_pSpiRegs, 0);

    pNonIpPort->m_IoCfg[Group] = CfgValue;  // Save new config to the cache

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIoConfigSetSpiStd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNonIpIoConfigSetSpiStd(
    DtaNonIpPort* pNonIpPort,
    Int Group,
    DtaIoConfigValue CfgValue)
{
    Int  NumSpiStd = 0;
    
    DT_ASSERT(pNonIpPort->m_CapSpi);
    
    if (pNonIpPort->m_CapSpiLvds1) NumSpiStd++;
    if (pNonIpPort->m_CapSpiLvds2) NumSpiStd++;
    if (pNonIpPort->m_CapSpiLvttl) NumSpiStd++;
    switch (CfgValue.m_Value)
    {
    case DT_IOCONFIG_SPILVDS1:
        DT_ASSERT(pNonIpPort->m_CapSpiLvds1);

        // Tristate port for DTA 102 is default always enabled
        if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber == 102)
            DtaRegTxCtrlSetTxDis(pNonIpPort->m_pTxRegs, 0);

        // Skip SPI std setting if lvds1 is the only supported option
        if (NumSpiStd == 1)
            break;
        DtaRegDvbSpiCtrlSetIoEnable(pNonIpPort->m_pSpiRegs, 1);
        DtaRegDvbSpiCtrlSetFailsafeEnable(pNonIpPort->m_pSpiRegs, 0);
        DtaRegDvbSpiCtrlSetLvttlSelect(pNonIpPort->m_pSpiRegs, 0);
        break;
    case DT_IOCONFIG_SPILVDS2:
        DT_ASSERT(pNonIpPort->m_CapSpiLvds2);
        // Skip SPI std setting if lvds2 is the only supported option
        if (NumSpiStd == 1)
            break;
        DtaRegDvbSpiCtrlSetIoEnable(pNonIpPort->m_pSpiRegs, 1);
        DtaRegDvbSpiCtrlSetFailsafeEnable(pNonIpPort->m_pSpiRegs, 1);
        DtaRegDvbSpiCtrlSetLvttlSelect(pNonIpPort->m_pSpiRegs, 0);
        break;
    case DT_IOCONFIG_SPILVTTL:
        DT_ASSERT(pNonIpPort->m_CapSpiLvttl);
        // Skip SPI std setting if lvttl is the only supported option
        if (NumSpiStd == 1)
            break;
        DtaRegDvbSpiCtrlSetIoEnable(pNonIpPort->m_pSpiRegs, 1);
        DtaRegDvbSpiCtrlSetFailsafeEnable(pNonIpPort->m_pSpiRegs, 1);
        DtaRegDvbSpiCtrlSetLvttlSelect(pNonIpPort->m_pSpiRegs, 1);
        break;
    default:
        DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
        return DT_STATUS_NOT_SUPPORTED;
    }

    pNonIpPort->m_IoCfg[Group] = CfgValue;  // Save new config to the cache

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIoConfigSetTsRateSel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNonIpIoConfigSetTsRateSel(
    DtaNonIpPort* pNonIpPort,
    Int Group,
    DtaIoConfigValue CfgValue)
{
    switch (CfgValue.m_Value)
    {
    case DT_IOCONFIG_INTTSRATE:
        DT_ASSERT(pNonIpPort->m_CapIntTsRate);
        // No actions required if only internal TS rate selection  supported 
        if (!pNonIpPort->m_CapExtTsRate && !pNonIpPort->m_CapExtRatio
            && !pNonIpPort->m_CapLock2Inp)
            break;

        // Select internal TS rate clock source
        if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber == 102)
            DtaRegTxCtrlSetUseExtClk(pNonIpPort->m_pTxRegs, 0);
        else
        {
            DtaRegTxCtrl2SetRateSelInt(pNonIpPort->m_pTxRegs);
            // Disable lock2input
            if (pNonIpPort->m_CapLock2Inp)
            {
                DtaRegTxCtrlSetLock2Input(pNonIpPort->m_pTxRegs, 0);
                DtaRegTxCtrl2SetLockSel(pNonIpPort->m_pTxRegs, pNonIpPort->m_PortIndex);
            }
        }
        break;
    case DT_IOCONFIG_EXTTSRATE:
    case DT_IOCONFIG_EXTRATIO:
        DT_ASSERT(pNonIpPort->m_CapExtTsRate || pNonIpPort->m_CapExtRatio);
        // Select external TS rate clock source
        if (pNonIpPort->m_pDvcData->m_DevInfo.m_TypeNumber == 102)
            DtaRegTxCtrlSetUseExtClk(pNonIpPort->m_pTxRegs, 1);
        else
        {
            DtaRegTxCtrl2SetRateSelExt(pNonIpPort->m_pTxRegs);
            // Disable lock2input
            if (pNonIpPort->m_CapLock2Inp)
            {
                DtaRegTxCtrlSetLock2Input(pNonIpPort->m_pTxRegs, 0);
                DtaRegTxCtrl2SetLockSel(pNonIpPort->m_pTxRegs, pNonIpPort->m_PortIndex);
            }
            // The ratio is set to 1:1 if external clock is selected in the DTAPI
        }
        break;

    case DT_IOCONFIG_LOCK2INP:
        DT_ASSERT(pNonIpPort->m_CapLock2Inp);
        // Enable lock2inp and set the buddy port
        DtaRegTxCtrlSetLock2Input(pNonIpPort->m_pTxRegs, 1);
        DtaRegTxCtrl2SetLockSel(pNonIpPort->m_pTxRegs, (UInt)CfgValue.m_ParXtra[0]);
        break;
    default:
        DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Save new config to the cache
    pNonIpPort->m_IoCfg[Group] = CfgValue;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIoConfigSetSwS2Apsk -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNonIpIoConfigSetSwS2Apsk(
    DtaNonIpPort* pNonIpPort,
    Int Group,
    DtaIoConfigValue CfgValue)
{
    // The actual initialisation of single or dual mode is done in the dtapiservice
    pNonIpPort->m_IoCfg[Group] = CfgValue;  // Save new config to the cache
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIoConfigSetFailSafe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtaNonIpIoConfigSetFailSafe(
    DtaNonIpPort* pNonIpPort,
    Int Group,
    DtaIoConfigValue CfgValue)
{
    switch (CfgValue.m_Value)
    {
    case DT_IOCONFIG_FALSE:
        DT_ASSERT(pNonIpPort->m_CapFailSafe);
        // Failsafe off requires no additional actions besides caching the failsafe
        // off state.
        break;
    case DT_IOCONFIG_TRUE:
        DT_ASSERT(pNonIpPort->m_CapFailSafe);
        // Reset timeout value to default 200ms
        pNonIpPort->m_FailsafeTimeout = 10;
        pNonIpPort->m_FailsafeTimeoutCnt = (UInt32)DtDivide64((UInt64)200*1000,
            pNonIpPort->m_pDvcData->m_DevInfo.m_PerIntItvUS, NULL); 
        // New value (DTA_FAILSAFE_WATCHDOG) is saved at the end...
        break;
    default:
        DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
        return DT_STATUS_NOT_SUPPORTED;
    }
    
    pNonIpPort->m_IoCfg[Group] = CfgValue;  // Save new config to the cache

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIoConfigSetGenLocked -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtaNonIpIoConfigSetGenLocked(
    DtaNonIpPort* pNonIpPort,
    Int Group,
    DtaIoConfigValue CfgValue)
{
    // Check port supports genlocking
    switch (CfgValue.m_Value)
    {
    case DT_IOCONFIG_FALSE:
        DT_ASSERT(pNonIpPort->m_CapGenLocked);
        // Genlocking off requires no additional actions besides caching the genlocked
        // off state.
        break;
    case DT_IOCONFIG_TRUE:
        DT_ASSERT(pNonIpPort->m_CapGenLocked);
        // Genlocking on requires no additional actions besides caching the genlocked
        // on state.
        break;
    default:
        DtDbgOut(ERR, NONIP, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
        return DT_STATUS_NOT_SUPPORTED;
    }

    pNonIpPort->m_IoCfg[Group] = CfgValue;  // Save new config to the cache

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIoConfigSetGenRef -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpIoConfigSetGenRef(DtaNonIpPort* pNonIpPort, Int Group,
                                                               DtaIoConfigValue CfgValue)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoConfigValue OldCfgValue;
    DtaDeviceData*  pDvcData = pNonIpPort->m_pDvcData;

    DT_ASSERT(pDvcData->m_Genlock.m_IsSupported);   // Must support genlocking    
    switch (CfgValue.m_Value)
    {
    case DT_IOCONFIG_FALSE:
        DT_ASSERT(pNonIpPort->m_CapGenRef);
        // NOTHING TODO HERE; CONFIG IS APPLIED BELOW
        break;

    case DT_IOCONFIG_TRUE:
        DT_ASSERT(pNonIpPort->m_CapGenRef);
        // NOTHING TODO HERE; CONFIG IS APPLIED BELOW
        break;

    default:
        DtDbgOut(ERR, DTA, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Backup current io-config
    OldCfgValue = pNonIpPort->m_IoCfg[Group];
    pNonIpPort->m_IoCfg[Group] = CfgValue;  // Save new config to the cache
        
    // Apply genref configuration
    Status = DtaGenlockApplyGenRefConfig(pDvcData);
    if (!DT_SUCCESS(Status))
        pNonIpPort->m_IoCfg[Group] = OldCfgValue;   // Restore original config
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIoConfigSetFracMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaNonIpIoConfigSetFracMode(DtaNonIpPort* pNonIpPort, Int Group,
                                                               DtaIoConfigValue CfgValue)
{
    DtStatus  Status = DT_STATUS_OK;
    DtaIoConfigValue OldCfgValue;
    DtaDeviceData*  pDvcData = pNonIpPort->m_pDvcData;
    
    DT_ASSERT(pDvcData->m_Genlock.m_IsSupported);   // Must support genlocking
        
    switch (CfgValue.m_Value)
    {
    case DT_IOCONFIG_FALSE:
        DT_ASSERT(pNonIpPort->m_CapFracMode);
        // NOTHING TODO HERE; CONFIG IS APPLIED BELOW
        break;

    case DT_IOCONFIG_TRUE:
        DT_ASSERT(pNonIpPort->m_CapFracMode);
        // NOTHING TODO HERE; CONFIG IS APPLIED BELOW
        break;

    default:
        DtDbgOut(ERR, DTA, "Invalid Config. Group: %d, Value: %d, SubValue: %d",
                                            Group, CfgValue.m_Value, CfgValue.m_SubValue);
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Backup current io-config
    OldCfgValue = pNonIpPort->m_IoCfg[Group];
    pNonIpPort->m_IoCfg[Group] = CfgValue;  // Save new config to the cache
    
    // Apply genref configuration
    Status = DtaGenlockApplyFracModeConfig(pDvcData);
    if (!DT_SUCCESS(Status))
        pNonIpPort->m_IoCfg[Group] = OldCfgValue;   // Restore original config
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpReleaseResourceFromFileObject -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaNonIpReleaseResourceFromFileObject(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile)
{
    Int i;
    for (i = 0; i < pDvcData->m_NumNonIpPorts; i++) 
    {
        // Cleanup HP buffer if initialised
        if (DtFileCompare(&pDvcData->m_pNonIpPorts[i].m_HpBuffer.m_Owner, pFile) &&
                                      pDvcData->m_pNonIpPorts[i].m_HpBuffer.m_Initialised)
        {
            DtDbgOut(AVG, DTA, "Cleanup shared buffer resource for port %i", 
                                                  pDvcData->m_pNonIpPorts[i].m_PortIndex);
            DtaShBufferClose(&pDvcData->m_pNonIpPorts[i].m_HpBuffer);
        }

        // Release exclusive access
        if (pDvcData->m_pNonIpPorts[i].m_ExclAccess)
        {
            DtaDeviceAcquireExclAccess(pDvcData);
            if (DtFileCompare(&pDvcData->m_pNonIpPorts[i].m_ExclAccessOwner, pFile) && 
                                                  pDvcData->m_pNonIpPorts[i].m_ExclAccess)
            {
                pDvcData->m_pNonIpPorts[i].m_ExclAccess = FALSE;
                DtDbgOut(AVG, DTA, "Release exclusive access for port %i", 
                                                  pDvcData->m_pNonIpPorts[i].m_PortIndex);
            }
            DtaDeviceReleaseExclAccess(pDvcData);
        }
    }
    return DT_STATUS_OK; 
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpPowerdownPre -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called everytime the hardware is power downed just before interrupts
// are disabled.
//
DtStatus  DtaNonIpPowerdownPre(DtaNonIpPort* pNonIpPort)
{
    if (pNonIpPort->m_CapHdmi)
        DtaNonIpHdmiPowerdownPre(pNonIpPort);
    
    if (pNonIpPort->m_CapAvEnc)
        DtaEnDecPowerdownPre(pNonIpPort);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpPowerdown -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function is called everytime the hardware is power downed.
//
void  DtaNonIpPowerdown(DtaNonIpPort* pNonIpPort)
{
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpEstimateRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Estimate rate 
//
void  DtaNonIpEstimateRate(DtaNonIpPort* pNonIpPort)
{
    Int  dDiff, EldestIndex, NewIndex, ValidCnt, ValidDiff;
    Int64  ValidCountTemp;
    BitrateMeasure*  pBrm = &pNonIpPort->m_BitrateMeasure;
    
    // Get Valid Counter and compute difference with last difference
    ValidCnt = pBrm->m_ValidCountSample;
    ValidDiff = ValidCnt - pBrm->m_LastValidCnt;
    pBrm->m_LastValidCnt = ValidCnt;

    // Compute index of location where to store new sample, with wrap-around
    NewIndex = pBrm->m_LastIndex + 1;
    if (NewIndex >= NUM_BITRATE_MEASURE_PERIODS)
        NewIndex = 0;

    // Compute index of eldest sample
    EldestIndex = NewIndex - pBrm->m_NumValidSamps;
    if (EldestIndex < 0)
        EldestIndex += NUM_BITRATE_MEASURE_PERIODS;

    // Compute delta between new and last difference
    dDiff = ValidDiff - pBrm->m_ValidDiff[pBrm->m_LastIndex];

    // TODO: In burst mode, this will not work...
    if (dDiff<-2 || dDiff>2) 
    {
        // New difference differs significantly from previous difference
        // Start all over again
        pBrm->m_NumBytesTotal = 0;
        pBrm->m_NumValidSamps = 1;
    } else {
        // New difference is close to previous difference 
        // If buffer is full => subtract eldest element
        if (pBrm->m_NumValidSamps == NUM_BITRATE_MEASURE_PERIODS)
            pBrm->m_NumBytesTotal -= pBrm->m_ValidDiff[EldestIndex];
        else // Buffer not full => we have an extra sample
            pBrm->m_NumValidSamps++;
    }

    // Process the Valid-Counter differential value
    pBrm->m_ValidDiff[NewIndex] = ValidDiff;
    pBrm->m_NumBytesTotal += ValidDiff;

    // Next time, last index is current new index
    pBrm->m_LastIndex = NewIndex;

    // Compute estimate for Valid Count
    ValidCountTemp  = (Int64) pBrm->m_NumBytesTotal;
    ValidCountTemp <<= 8; // = *256
    ValidCountTemp = (Int64)DtDivide64((UInt64)ValidCountTemp, 
                                                     (UInt64)pBrm->m_NumValidSamps, NULL);
    pBrm->m_ValidCount256 = (int) ValidCountTemp;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpGetEstimatedRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Int  DtaNonIpGetEstimatedRate(DtaNonIpPort* pNonIpPort)
{
    DtaDeviceInfo*  pDevInfo = &pNonIpPort->m_pDvcData->m_DevInfo;
    Int  PerIntBits = 21;   // Default value for cards which has a fixed per int. interval
    if (pDevInfo->m_HasPerIntItvReg)
        PerIntBits = pDevInfo->m_PerIntClkBit;
    return (Int)(((Int64)pNonIpPort->m_BitrateMeasure.m_ValidCount256 * pDevInfo->m_RefClk)
                                                                  >> (PerIntBits - 3 + 8));
    // +3 = From bytes to bits
    // -8 - device by 256. See DtaNonIpEstimateRate.
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpGetMaxDmaBurstSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtaNonIpGetMaxDmaBurstSize(DtaNonIpPort* pNonIpPort)
{
    // For matrix-IP port use a maximum transfer size of 16MB, for other use the max 
    // FIFO size
    if (pNonIpPort->m_CapMatrix)
        return 16*1024*1024;    // We process one frame at a time (16MB should be enough)
    else
        return DtaNonIpGetMaxFifoSize(pNonIpPort);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpGetMaxFifoSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Int  DtaNonIpGetMaxFifoSize(DtaNonIpPort* pNonIpPort)
{
    Int  SdramSize;
    // We have to calculate the registers pointers because this function is used before 
    // the channel pointers are calculated
    if (pNonIpPort->m_IoCfg[DT_IOCONFIG_IODIR].m_Value == DT_IOCONFIG_OUTPUT)
    {
        DT_ASSERT(pNonIpPort->m_TxRegsOffset != (UInt16)-1);
        SdramSize = DtaRegTxStatGetSdramSize((pNonIpPort->m_pDvcData->m_pGenRegs +
                                                             pNonIpPort->m_TxRegsOffset));
    } else {
        DT_ASSERT(pNonIpPort->m_RxRegsOffset != (UInt16)-1);
        SdramSize = DtaRegRxStatGetSdramSize((pNonIpPort->m_pDvcData->m_pGenRegs +
                                                             pNonIpPort->m_RxRegsOffset));
    }
    switch (SdramSize)
    {
    case DT_SDRAM_SIZE_8MB: return 8*1024*1024;
    case DT_SDRAM_SIZE_16MB: return 16*1024*1024;
    case DT_SDRAM_SIZE_32MB: return 32*1024*1024;
    case DT_SDRAM_SIZE_64MB: return 64*1024*1024;
    case DT_SDRAM_SIZE_128MB: return 128*1024*1024;
    }
    return 8*1024*1024;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpIsVidStdSupported -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtaNonIpIsVidStdSupported(DtaNonIpPort* pNonIpPort, Int  VidStd)
{
    switch (VidStd)
    {
    case DT_VIDSTD_525I59_94:  return pNonIpPort->m_Cap525I59_94;
    case DT_VIDSTD_625I50:     return pNonIpPort->m_Cap625I50;
    case DT_VIDSTD_720P23_98:  return pNonIpPort->m_Cap720P23_98;
    case DT_VIDSTD_720P24:     return pNonIpPort->m_Cap720P24;
    case DT_VIDSTD_720P25:     return pNonIpPort->m_Cap720P25;
    case DT_VIDSTD_720P29_97:  return pNonIpPort->m_Cap720P29_97;
    case DT_VIDSTD_720P30:     return pNonIpPort->m_Cap720P30;
    case DT_VIDSTD_720P50:     return pNonIpPort->m_Cap720P50;
    case DT_VIDSTD_720P59_94:  return pNonIpPort->m_Cap720P59_94;
    case DT_VIDSTD_720P60:     return pNonIpPort->m_Cap720P60;
    case DT_VIDSTD_1080P23_98: return pNonIpPort->m_Cap1080P23_98;
    case DT_VIDSTD_1080P24:    return pNonIpPort->m_Cap1080P24;
    case DT_VIDSTD_1080P25:    return pNonIpPort->m_Cap1080P25;
    case DT_VIDSTD_1080P29_97: return pNonIpPort->m_Cap1080P29_97;
    case DT_VIDSTD_1080P30:    return pNonIpPort->m_Cap1080P30;
    case DT_VIDSTD_1080PSF23_98: return pNonIpPort->m_Cap1080Psf23_98;
    case DT_VIDSTD_1080PSF24:    return pNonIpPort->m_Cap1080Psf24;
    case DT_VIDSTD_1080PSF25:    return pNonIpPort->m_Cap1080Psf25;
    case DT_VIDSTD_1080PSF29_97: return pNonIpPort->m_Cap1080Psf29_97;
    case DT_VIDSTD_1080PSF30:    return pNonIpPort->m_Cap1080Psf30;
    case DT_VIDSTD_1080P50:      return pNonIpPort->m_Cap1080P50;
    case DT_VIDSTD_1080P50B:     return pNonIpPort->m_Cap1080P50B;
    case DT_VIDSTD_1080P59_94:   return pNonIpPort->m_Cap1080P59_94;
    case DT_VIDSTD_1080P59_94B:  return pNonIpPort->m_Cap1080P59_94B;
    case DT_VIDSTD_1080P60:    return pNonIpPort->m_Cap1080P60;
    case DT_VIDSTD_1080P60B:   return pNonIpPort->m_Cap1080P60B;
    case DT_VIDSTD_1080I50:    return pNonIpPort->m_Cap1080I50;
    case DT_VIDSTD_1080I59_94: return pNonIpPort->m_Cap1080I59_94;
    case DT_VIDSTD_1080I60:    return pNonIpPort->m_Cap1080I60;

    default:        
        DT_ASSERT(1 == 0);  // should never come here
        return FALSE;
    }
    return FALSE;
}


