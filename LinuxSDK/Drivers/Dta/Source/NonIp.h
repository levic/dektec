//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NonIp.h *#*#*#*#*#*#*#*#*#* (C) 2010-2012 DekTec
//
// Dta driver - Non IP functionality - Declaration of generic non IP port functionality
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2010-2012 DekTec Digital Video B.V.
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
    Int  m_LastIndex;                   // Index in ValidCnt of last sample
    Int  m_LastValidCnt;                // Last absolute value of Valid counter
    Int  m_NumBytesTotal;               // Total #valid bytes in measurement period
    Int  m_NumValidSamps;               // Number of "valid" samples in m_ValidCnt[]
    Int  m_ValidCount256;               // #Valid bytes per interval, times 256
    Int  m_ValidCountSample;            // Valid count sample; read in interrupt routine
} BitrateMeasure;

// DtaMatrixPort
typedef struct _DtaMatrixPort
{
    DtDpc  m_LastFrameIntDpc;   // DPC for last-frame-interrupt 
    DtEvent  m_LastFrameIntEvent;  // Event to signal last-frame-interrupt

    volatile Int64  m_LastFrame;  // Last transmitted/received frame
    volatile Int64  m_SofFrame; // Frame transmitted/received @SOF-interrupt-event
    volatile Int  m_SofLine;    // Line transmitted/received @SOF-interrupt-event
                
} DtaMatrixPort;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaNonIpPort -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtaNonIpPort
{
    Int  m_PortIndex;                   // Physical port index
    DtaDeviceData*  m_pDvcData;

    // Capabilities
    // IODIR (I/O direction) - Capabilities
    Bool  m_CapDisabled;
    Bool  m_CapInput;
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
    // IOSTD (I/O standard) - Capabilities
    Bool  m_Cap3GSdi;
    Bool  m_CapAsi;
    Bool  m_CapDemod;
    Bool  m_CapHdSdi;
    Bool  m_CapIfAdc;
    Bool  m_CapIp;
    Bool  m_CapMod;
    Bool  m_CapSdi;
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
    Bool  m_Cap1080P59_94;
    Bool  m_Cap1080P60;
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

    // Properties
    Bool  m_IsNonFuntional;     // A non-functional port (i.e. not an input or output)
    Bool  m_IsBidir;

    // ASI/SDI interface
    Int  m_AsiSdiDeserItfType;
    Int  m_AsiSdiSerItfType;
    
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
    UInt16  m_RxRegsOffset;
    UInt16  m_TxRegsOffset;
    UInt16  m_SpiRegsOffset;
    UInt16  m_FifoOffset;
    volatile UInt8*  m_pRxRegs;
    volatile UInt8*  m_pTxRegs;
    volatile UInt8*  m_pSpiRegs;

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


} DtaNonIpPort;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtaNonIpCleanup(DtaDeviceData* pDvcData, Int PortIndex, DtaNonIpPort* pNonIpPort);
void  DtaNonIpCleanupPre(DtaDeviceData* pDvcData, Int PortIndex,
                                                                DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpInit(DtaDeviceData* pDvcData, Int PortIndex, DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpPowerup(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpDetectVidStd(DtaNonIpPort* pNonIpPort, Int*  pVidStd);
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
void  DtaNonIpPowerdown(DtaNonIpPort* pNonIpPort);
DtStatus  DtaNonIpPowerdownPre(DtaNonIpPort* pNonIpPort);
void  DtaNonIpEstimateRate(DtaNonIpPort* pNonIpPort);
Int  DtaNonIpGetEstimatedRate(DtaNonIpPort* pNonIpPort);
Bool  DtaNonIpIsVidStdSupported(DtaNonIpPort* pNonIpPort, Int  VidStd);


#endif // _NON_IP_H
