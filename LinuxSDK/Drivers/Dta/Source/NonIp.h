//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NonIp.h *#*#*#*#*#*#*#*#*#* (C) 2010-2016 DekTec
//
// Dta driver - Non IP functionality - Declaration of generic non IP port functionality
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

#ifndef _NON_IP_H
#define _NON_IP_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Target ID Detection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Target-ID Detection (for DTA-102/DTA-122)
//

// Target-detection states
#define  DTA_TD_INITIAL          0
#define  DTA_TD_FIND_TARGETID    1
#define  DTA_TD_OPEN_CIRCUIT     2
#define  DTA_TD_SPI_SINK         3
#define  DTA_TD_TARGETID_STABLE  4
#define  DTA_TD_TARGET_ERROR     5

// TargetIdDetection context struct
typedef struct _TargetIdDetection {
    Int  m_TargetId;            // Current best guess for target-adapter ID
    Int  m_Present;             // Current best guess for target-adapter presence
    Int  m_TdState;             // Target-detection state
    Int  m_LastTargetId;        // Last measurement for target-adapter ID
    Int  m_NumTimesInFind;      // #times in state DTA_TD_FIND_TARGETID
    Int  m_StableCount;         // Counter to assess whether measurement is stable
    Bool  m_TargetError;        // Target Error, set in target-detection state 
                                // machine, cleared in DtaTxProcessStatusFlags
} TargetIdDetection;

// Bitrate measurement
#define  NUM_BITRATE_MEASURE_PERIODS            250

typedef struct _BitrateMeasure
{
    Int  m_ValidDiff[NUM_BITRATE_MEASURE_PERIODS];
                                // Number of Valid bytes received per period
                                // (=difference between successive Valid counts)
    Int  m_LastIndex;           // Index in ValidCnt of last sample
    Int  m_LastValidCnt;        // Last absolute value of Valid counter
    Int  m_NumBytesTotal;       // Total #valid bytes in measurement period
    Int  m_NumValidSamps;       // Number of "valid" samples in m_ValidCnt[]
    Int  m_ValidCount256;       // #Valid bytes per interval, times 256
    Int  m_ValidCountSample;    // Valid count sample; read in interrupt routine
} BitrateMeasure;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Frame buffer configuration -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

#define  DTA_FRMBUF_NUM_SECTIONS    2  // number of section in frame buffer
#define  DTA_FRMBUF_SECT_HANC       0  // HANC section number
#define  DTA_FRMBUF_SECT_VID        1  // Video section number
#define  DTA_FRMBUF_SECT_VANC       DTA_FRMBUF_SECT_VID // VANC shares section with video

#define  DTA_FRMBUF_MAX_FRAMES      32  // Maximum # of frames stored in frame buffer
#define  DTA_FRMBUF_HOLD_FRAME      -6789LL  // Special frame seq. number indicating the 
                                        // HOLD frame

// DtaFrameBufSectionConfig
typedef struct _DtaFrameBufSectionConfig
{
    // Line settings
    Int  m_LineNumSymbols;      // Number of (actual) symbols per line
    Int  m_LineNumBytesInMem;   // Number of bytes per line (incl. padding)

    // Frame settings
    Int  m_NumFrames;           // Number of frames stored in section
    Int  m_FrameSizeInMem;      // Size of frame in memory

    // Global settings
    UInt32  m_Size;             // Size of the section
    UInt32  m_BaseAddr;         // Section base address (byte address)
    UInt32  m_BeginAddr;        // Begin address for data  (byte address)
    UInt32  m_EndAddr;          // End address for data (byte address)

    UInt32  m_FrameStartAddr[DTA_FRMBUF_MAX_FRAMES];    // Start addresses of frames

} DtaFrameBufSectionConfig;

// DtaFrameBufConfig
typedef struct _DtaFrameBufConfig
{
    Int  m_NumFrames;           // # of frames stored in buffer. NOTE: last frame 
                                // is reserved for black frame playout in HOLD state
    volatile Int  m_NumUsableFrames;  // # of frames that can be used (i.e. excluding any 
                                // frames at the end of the frames buffer reserved for 
                                // the HOLD state)
    DtaFrameBufSectionConfig  m_Sections[DTA_FRMBUF_NUM_SECTIONS];
} DtaFrameBufConfig;

// DtaMatrixMemTrSetup
typedef struct _DtaMatrixMemTrSetup
{
    Bool  m_IsWrite;            // Transfer direction (rd=false, wr=true)
    Int64  m_Frame;             // Frame to transfer
    Int  m_StartLine;           // First line in transfer
    Int  m_NumLines;            // Number of lines to write
    Int  m_TrCmd;               // Transfer type
    Int  m_DataFormat;          // Format of data (8-, 10-, 16-bit)
    Int  m_RgbMode;             // RGB mode
    Int  m_SymFlt;              // Symbol filter mode
    Int  m_Scaling;             // Scaling mode
    Int  m_AncFlt;              // Anc filter mode
    Int  m_Stride;              // -1 means no stride
} DtaMatrixMemTrSetup;

typedef enum _DtaMatrixPortState
{
    MATRIX_PORT_UNINIT = 0,     // Port is uninitialised
    MATRIX_PORT_CONFIGURING,    // Port is being configured
    MATRIX_PORT_IDLE,           // Port is IDLE state (not receiving/transmitting)
    MATRIX_PORT_HOLD,           // Port is in HOLD state (playing black-frames)
    MATRIX_PORT_RUN_AUTO,       // Port is running (transmitting/receiving), frame idx
                                // is automatically increased by driver
    MATRIX_PORT_RUN_MAN,        // Port is running (transmitting/receiving), next frame
                                // is under user control
} DtaMatrixPortState;

typedef struct _DtaMatrixFrameInfo
{
    Int64  m_RefClkStart;       // Latched version of reference clock at new frame int
    Int64  m_RefClkEnd;         // Latched version of reference clock at end of frame
    Int  m_TopHalf;             // True for first part of 3g level B frame
} DtaMatrixFrameInfo;

// DtaMatrixPort
typedef struct _DtaMatrixPort
{
    volatile DtaMatrixPortState  m_State;  // Channel state
    DtMutex  m_StateLock;       // Mutex to protect MATRIX state (m_State)
    DtaMatrixPortState  m_LastStateAtInt;  
                                // Last state seen by interrupt routine. NOTE: only 
                                // interrupt routine may write to this variable

    DtAvFrameProps  m_FrameProps;  // Frame properties
    DtaFrameBufConfig  m_BufConfig;  // Frame buffer configuration
    Int  m_RowIdx;                // Matrix row the port is associated with
    
    DtDpc  m_LastFrameIntDpc;   // DPC for last-frame-interrupt 
    DtEvent  m_LastFrameIntEvent;  // Event to signal last-frame-interrupt
    
    volatile Int64  m_CurFrame; // Frame currently being transmitted/received
    volatile Int64  m_LastFrame;  // Last transmitted/received frame
    volatile Int64  m_NextFrame;  // Forced next frame to transmit/receive
    volatile Int64  m_SofFrame; // Frame transmitted/received @SOF-interrupt-event
    volatile Int  m_SofLine;    // Line transmitted/received @SOF-interrupt-event
    volatile Int64  m_FrmIntCnt; // Frame interrupt counter
    Int  m_ForceRestart;        // Force restart in interrupt routine

    UInt32  m_Vpid1;            // VPID forced by API
    UInt32  m_Vpid2;            // VPID forced by API (for 3G lvl B 2nd link)

    Int  m_ExtraPixelDelay;     // Additional delay (in #pixels) to apply to the output.
                                // NOTE: the dleay is relative to the GENREF TOF

    DtaMatrixFrameInfo  m_FrameInfo[DTA_FRMBUF_MAX_FRAMES];

    Int64  m_AsiDmaNumBytes;
    Int  m_AsiFifoSize;
    Int  m_AsiDmaOffset;
    Int  m_AsiCtrl;

    UInt64  m_LastIoReset;

} DtaMatrixPort;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaRs422Port -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtaRs422Port
{
    DtSpinLock  m_StateLock;       // Spinlock to protect state
    DtDpc  m_IntDpc;
    DtEvent  m_TxCmplEvent;
    DtEvent  m_RxDataAvailEvent;
    UInt8  m_RxBuf[256];
    Int  m_RxNumBytes;
    Int  m_TxState;
} DtaRs422Port;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpPort -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
struct _DtaNonIpPort
{
    Int  m_PortIndex;                   // Physical port index
    DtaDeviceData*  m_pDvcData;

    // Capabilities
    // IODIR (I/O direction) - Capabilities
    Bool  m_CapDisabled;
    Bool  m_CapInput;
    Bool  m_CapIntInput;
    Bool  m_CapMonitor;
    Bool  m_CapOutput;
    // IODIR - INPUT (Uni-directional input) - Sub capabilities
    Bool  m_CapSharedAnt;
    // IODIR - OUTPUT (Uni-directional output) - Sub capabilities
    Bool  m_CapDblBuf;
    Bool  m_CapLoopS2L3;
    Bool  m_CapLoopS2Ts;
    Bool  m_CapLoopThr;
    // IOPROPS (I/O properties) - Capabilities
    Bool  m_CapMatrix;
    Bool  m_CapMatrix2;
    Bool  m_CapVirtual;
    Bool  m_CapGenRefSlave;
    // IOSTD (I/O standard) - Capabilities
    Bool  m_Cap3GSdi;
    Bool  m_CapAsi;
    Bool  m_CapAvEnc;
    Bool  m_CapDemod;
    Bool  m_CapGpsTime;
    Bool  m_CapHdmi;
    Bool  m_CapHdSdi;
    Bool  m_CapIfAdc;
    Bool  m_CapIp;
    Bool  m_CapMod;
    Bool  m_CapPhaseNoise;
    Bool  m_CapSdi;
    Bool  m_CapSdiRx;
    Bool  m_CapSpi;
    Bool  m_CapSpiSdi;
    // IOSTD - SDI (SD-SDI) - Sub capabilities
    Bool  m_Cap525I59_94;
    Bool  m_Cap625I50;
    // IOSTD - SPISDI - Sub capabilities
    Bool  m_CapSpi525I59_94;
    Bool  m_CapSpi625I50;
    // IOSTD - SDI (HD-SDI) - Sub capabilities
    Bool  m_Cap1080I50;
    Bool  m_Cap1080I59_94;
    Bool  m_Cap1080I60;
    Bool  m_Cap1080P23_98;
    Bool  m_Cap1080P24;
    Bool  m_Cap1080P25;
    Bool  m_Cap1080P29_97;
    Bool  m_Cap1080P30;
    Bool  m_Cap1080Psf23_98;
    Bool  m_Cap1080Psf24;
    Bool  m_Cap1080Psf25;
    Bool  m_Cap1080Psf29_97;
    Bool  m_Cap1080Psf30;
    Bool  m_Cap720P23_98;
    Bool  m_Cap720P24;
    Bool  m_Cap720P25;
    Bool  m_Cap720P29_97;
    Bool  m_Cap720P30;
    Bool  m_Cap720P50;
    Bool  m_Cap720P59_94;
    Bool  m_Cap720P60;
    // IOSTD - SDI (3G-SDI) - Sub capabilities
    Bool  m_Cap1080P50;
    Bool  m_Cap1080P50B;
    Bool  m_Cap1080P59_94;
    Bool  m_Cap1080P59_94B;
    Bool  m_Cap1080P60;
    Bool  m_Cap1080P60B;
    // RFCLKSEL (RF clock source selection) - Capabilities
    Bool  m_CapRfClkExt;
    Bool  m_CapRfClkInt;
    // SPICLKSEL (Parallel port clock source selection) - Capabilities
    Bool  m_CapSpiClkExt;
    Bool  m_CapSpiClkInt;
    // SPIMODE (Parallel port mode) - Capabilities
    Bool  m_CapSpiFixedClk;
    Bool  m_CapSpiDvbMode;
    Bool  m_CapSpiSer8B;
    Bool  m_CapSpiSer10B;
    // SPISTD (Parallel port I/O standard) - Capabilities
    Bool  m_CapSpiLvds1;
    Bool  m_CapSpiLvds2;
    Bool  m_CapSpiLvttl;
    // TSRATESEL (Transport-stream rate selection) - Capabilities
    Bool  m_CapExtTsRate;
    Bool  m_CapExtRatio;
    Bool  m_CapIntTsRate;
    Bool  m_CapLock2Inp;
    // BOOLIO (Boolean I/O capabilities) - Capabilities
    Bool  m_CapFracMode;
    Bool  m_CapFailSafe;
    Bool  m_CapGenLocked;
    Bool  m_CapGenRef;
    Bool  m_CapSwS2Apsk;
    Bool  m_CapRs422;

    // Properties
    Bool  m_IsNonFuntional;     // A non-functional port (i.e. not an input or output)
    Bool  m_IsBidir;

    // ASI/SDI interface
    Int  m_AsiSdiDeserItfType;
    Int  m_AsiSdiSerItfType;
    Int  m_AsiSdiSerDelayNsSd, m_AsiSdiSerDelayNsHd, m_AsiSdiSerDelayNs3g;
                                // Specifies the pipeline delay (in ns) of the serial 
                                // interface for SDI signals. This delay must be 
                                // compensated for by the genlock logic to properly align
                                // to the genlock reference signal.

    
    // Current IO configuration
    DtaIoConfigValue  m_IoCfg[DT_IOCONFIG_COUNT];

    // Exclusive access
    Bool  m_ExclAccess;
    DtFileObject  m_ExclAccessOwner;

    // Status flags
    Int  m_Flags;
    Int  m_FlagsLatched;
    DtSpinLock  m_FlagsSpinLock;

    // Firmware register mapping
    UInt16  m_ModRegsOffset;
    UInt16  m_RfRegsOffset;
    UInt16  m_RxRegsOffset;
    UInt16  m_TxRegsOffset;
    UInt16  m_SpiRegsOffset;
    UInt16  m_Rs422RegsOffset;
    UInt16  m_FwbRegsOffset;
    UInt16  m_FifoOffset;
    volatile UInt8*  m_pModRegs;
    volatile UInt8*  m_pRfRegs;
    volatile UInt8*  m_pRxRegs;
    volatile UInt8*  m_pTxRegs;
    volatile UInt8*  m_pSpiRegs;
    volatile UInt8*  m_pRs422Regs;
    volatile UInt8*  m_pFwbRegs;

    // DMA Channel
    DmaChannel  m_DmaChannel;

    // Failsafe
    Bool  m_FailsafeEnable;     // Failsafe counters enabled
    UInt32  m_FailsafeCnt;      // Current counter value
    UInt32  m_FailsafeTimeout;  // Timeout set by application
    UInt32  m_FailsafeTimeoutCnt;
                                // Calculated counter value when timeout is reached

    TargetIdDetection  m_TrgIdDet;
                                // For DTA-102/DTA-122 target-ID detection
    // High performance buffer
    DtaShBuffer  m_HpBuffer;

    // Bitrate measurement
    BitrateMeasure  m_BitrateMeasure;

    // Matrix-API (HD-SDI)
    DtaMatrixPort  m_Matrix;

    // Port level I2C
    DtaI2c  m_I2c;

    // RS-422 API
    DtaRs422Port  m_Rs422;

    // SDI-AV-RX API 
    DtaSdiAvRxPort  m_SdiAvRx;

    // HDMI port
    DtaHdmi  m_Hdmi;

    // D7Pro encoder port
    DtaEncD7ProPort  m_EncD7Pro;
    
    // Common encoder/decoder fields
    DtaEnDecPort  m_EnDec;

    // SPI master flash controller
    DtaSpiMf  m_SpiMf;

    // Genlocking
    Int  m_TofAlignOffsetNs;    // Offset in ns with which the TOF arrive at serialiser

};

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtaNonIpCleanup(DtaDeviceData* pDvcData, Int PortIndex, DtaNonIpPort* pNonIpPort);
void  DtaNonIpCleanupPre(DtaDeviceData* pDvcData, Int PortIndex,
                                                                DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpClose(DtaNonIpPort* pNonIpPort, DtFileObject* pFile);
DtStatus  DtaNonIpInit(DtaDeviceData* pDvcData, Int PortIndex, DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpInterruptEnable(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpInterruptDisable(DtaNonIpPort* pNonIpPort);
Bool  DtaNonIpInterrupt(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpPowerup(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpPowerUpPost(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpDetectVidStd(DtaNonIpPort* pNonIpPort, Int* pVidStd,
                                            UInt* pVpid, UInt* pVpid2, Int* pAspectRatio);
DtStatus  DtaNonIpIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile,
                                                                   DtIoctlObject* pIoctl);
DtStatus  DtaNonIpHasAccess(DtaNonIpPort* pNonIpPort, DtFileObject* pFile);
DtStatus  DtaNonIpExclusiveAccess(DtaNonIpPort* pNonIpPort, DtFileObject* pFile, Int Cmd);
DtStatus  DtaNonIpTargetIdGetId(DtaNonIpPort* pNonIpPort, Int* pPresent, Int* pTargetId);
void  DtaNonIpTargetIdDetectStateMachine(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpIoConfigGet(DtaNonIpPort* pNonIpPort, Int Code,
                                                             DtaIoConfigValue* pCfgValue);
DtStatus  DtaNonIpIoConfigSet(DtaNonIpPort* pNonIpPort, Int Code,
                                                               DtaIoConfigValue CfgValue);
DtStatus  DtaNonIpReleaseResourceFromFileObject(DtaDeviceData* pDvcData, 
                                                                     DtFileObject* pFile);
DtStatus  DtaNonIpGetGenRefProps(DtaNonIpPort* pNonIpPort, 
                                                       DtaIoctlNonIpGenRefProps*  pProps);
DtStatus  DtaNonIpNotifyGenRefProp(DtaNonIpPort* pNonIpPort, 
                                                       DtaIoctlNonIpGenRefProps*  pProps);
void  DtaNonIpPowerdown(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpPowerdownPre(DtaNonIpPort* pNonIpPort);
void  DtaNonIpEstimateRate(DtaNonIpPort* pNonIpPort);
Int  DtaNonIpGetEstimatedRate(DtaNonIpPort* pNonIpPort);
Bool  DtaNonIpIsVidStdSupported(DtaNonIpPort* pNonIpPort, Int  VidStd);

DtStatus  DtaNonIpMatrixClose(DtaNonIpPort* pNonIpPort, DtFileObject* pFile);
DtStatus  DtaNonIpMatrixConfigure(DtaNonIpPort* pNonIpPort, Bool  ForceConfig);
DtStatus  DtaNonIpMatrixDetectVidStd(DtaNonIpPort* pNonIpPort, Int*  pVidStd);
DtStatus  DtaNonIpMatrixInit(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpMatrixInterruptEnable(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpMatrixInterruptDisable(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpMatrixPowerUpPost(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpMatrixAttachToRow(DtaNonIpPort* pNonIpPort, Int  RowIdx);
DtStatus  DtaNonIpMatrixSetAsiCtrl(DtaNonIpPort* pNonIpPort, Int  AsiCtrl);
DtStatus  DtaNonIpMatrixSetState(DtaNonIpPort* pNonIpPort, DtaMatrixPortState  NewState);
DtStatus  DtaNonIpMatrixStart(DtaNonIpPort* pNonIpPort, Int64  StartFrame, Bool  AutoMode,
                                                                      Bool  ForceRestart);
DtStatus  DtaNonIpMatrixStop(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpMatrixGetCurrentFrame(DtaNonIpPort* pNonIpPort, Int64*  pFrame);
DtStatus  DtaNonIpMatrixSetNextFrame(DtaNonIpPort* pNonIpPort, Int64  NextFrame);
DtStatus  DtaNonIpMatrixPrepForDma(DtaNonIpPort* pNonIpPort, Int  BufSize, 
                                             const DtaMatrixMemTrSetup*,  Int*  pDmaSize);
DtStatus  DtaNonIpMatrixGetReqDmaSize(DtaNonIpPort* pNonIpPort, 
                               const DtaMatrixMemTrSetup* pMemTrSetup, Int*  pReqDmaSize);

UInt32  DtaNonIpMatrixGetFrameAddrInMem(DtaNonIpPort* pNonIpPort, Int  Section, 
                                                                 Int64  Frame, Int  Line);
Int  DtaNonIpMatrixFrame2Index(DtaNonIpPort* pNonIpPort, Int64  Frame);
Bool  DtaNonIpMatrixUsesLegacyHdChannelInterface(DtaNonIpPort* pNonIpPort);
UInt  DtaNonIpMatrixDmaReadFinished(DtaNonIpPort* pNonIpPort, Int TrCmd);
UInt  DtaNonIpMatrixDmaWriteFinished(DtaNonIpPort* pNonIpPort, Int TrCmd);
void  DtaNonIpMatrixPeriodicInt(DtaNonIpPort* pNonIpPort);

DtStatus  DtaNonIpRs422Init(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpRs422InterruptEnable(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpRs422InterruptDisable(DtaNonIpPort* pNonIpPort);



#ifdef WINBUILD
void  DtaNonIpDmaCompletedWindows(DmaChannel* pDmaChannel, void* pContext);
void  DtaNonIpRxEvtRequestCancelDma(WDFREQUEST WdfRequest);
void  DtaNonIpTxEvtRequestCancelDma(WDFREQUEST WdfRequest);
void  DtaNonIpMatrixDmaCompletedWindows(DmaChannel* pDmaChannel, void* pContext);
void  DtaNonIpMatrixRxEvtRequestCancelDma(WDFREQUEST WdfRequest);
void  DtaNonIpMatrixTxEvtRequestCancelDma(WDFREQUEST WdfRequest);

#endif

#endif // _NON_IP_H
