//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NonIp.h *#*#*#*#*#*#*#*#*#* (C) 2011-2015 DekTec
//
// Dtu driver - Non IP functionality - Declaration of generic non IP port functionality
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

#ifndef _NON_IP_H
#define _NON_IP_H

// 16 LSB are not used for port flags to prevent any possible confusion with
// the device-level flags
#define DTU_PORT_FLAG_INSUFF_USB_BW  0x010000
#define DTU_PORT_FLAG_SDI_NO_LOCK    0x020000
#define DTU_PORT_FLAG_SDI_INVALID    0x040000

// State for the data-copy thread for USB3 devices. Not all states are valid
// for all devices.
#define DTU3_STATE_IDLE              0        // Do nothing
#define DTU3_STATE_EXIT              1        // Temporary state to signal thread to exit
#define DTU3_STATE_DET_VIDSTD        2        // Mostly idle, check for signal every 0.5s
#define DTU3_STATE_READ351           3        // Read-loop specific for DTU-351
#define DTU3_STATE_WRITE315          4        // Write-loop specific for DTU-315

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtuNonIpPort -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct _DtuNonIpPort
{
    Int  m_PortIndex;                   // Physical port index
    DtuDeviceData*  m_pDvcData;

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
    Bool  m_CapFailSafe;
    Bool  m_CapGenLocked;
    Bool  m_CapGenRef;
    Bool  m_CapSwS2Apsk;
    Bool  m_CapIsoBw;
    // PWRMODE (Power mode) - Capabilities
    Bool  m_CapModHq;
    Bool  m_CapLowPwr;

    // Properties
    Bool  m_IsBidir;

    // Current IO configuration
    DtuIoConfigValue  m_IoCfg[DT_IOCONFIG_COUNT];

    // Exclusive access
    Bool  m_ExclAccess;
    DtFileObject  m_ExclAccessOwner;

    // Register offsets
    Int  m_RxRegsOffset;
    Int  m_TxRegsOffset;

    // Temporary Buffer
    UInt8*  m_pTempBuf;
    Int  m_TempBufWrIndex;
    Int  m_TempBufRdIndex;
    
    // Shared buffer with DTAPI
    DtuShBuffer  m_SharedBuffer;

    // DTU-3xx variables
    volatile Int  m_State;          // Current state of m_DataThread. Should only be changed
                                    // inside the thread itself.
    volatile Int  m_NextState;      // The next state of the Rx thread.
    DtEvent  m_StateChanged;        // Fire after changing m_NextState to signify the
                                    // change to the worker thread.
    DtEvent  m_StateChangeCmpl;     // Thread state change completed
    DtThread  m_DataThread;
    
    // DTU-351 variables
    Int  m_InitRxMode;              // Initial RX mode
    Bool  m_AllowRxModeChanges;
    Int  m_DetVidStd;
    Bool  m_DataLoss;
    
    // Port-level flags on USB3 devices
    Int  m_StateFlags;

    //DtSpinLock  m_FlagsSpinLock;

} DtuNonIpPort;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtuNonIpCleanup(DtuDeviceData* pDvcData, Int PortIndex, DtuNonIpPort* pNonIpPort);
void  DtuNonIpCleanupPre(DtuDeviceData* pDvcData, Int PortIndex,
                                                                DtuNonIpPort* pNonIpPort);
DtStatus  DtuNonIpDetectVidStd(DtuNonIpPort* pNonIpPort, Int*  pVidStd);
DtStatus  DtuNonIpInit(DtuDeviceData* pDvcData, Int PortIndex, DtuNonIpPort* pNonIpPort);

DtStatus  DtuNonIpIoctl(DtuDeviceData* pDvcData, DtFileObject* pFile,
                                                                   DtIoctlObject* pIoctl);
DtStatus  DtuNonIpRxIoctl(DtuDeviceData*  pDvcData, DtFileObject*  pFile, 
                                                                  DtIoctlObject*  pIoctl);
DtStatus  DtuNonIpTxIoctl(DtuDeviceData*  pDvcData, DtFileObject*  pFile, 
                                                                  DtIoctlObject*  pIoctl);
DtStatus  DtuNonIpHasAccess(DtuNonIpPort* pNonIpPort, DtFileObject* pFile);
DtStatus  DtuNonIpExclusiveAccess(DtuNonIpPort* pNonIpPort, DtFileObject* pFile, Int Cmd);
DtStatus  DtuNonIpIoConfigGet(DtuNonIpPort* pNonIpPort, Int Code,
                                                             DtuIoConfigValue* pCfgValue);
DtStatus  DtuNonIpIoConfigSet(DtuNonIpPort* pNonIpPort, Int Code,
                                                               DtuIoConfigValue CfgValue);
DtStatus  DtuNonIpReleaseResourceFromFileObject(DtuDeviceData* pDvcData, 
                                                                     DtFileObject* pFile);
DtStatus  DtuNonIpRxRegWriteMasked(DtuNonIpPort* pNonIpPort,UInt32 Addr, UInt32 Mask,
                                                              UInt32 Shift, UInt32 Value);
DtStatus  DtuNonIpTxRegWriteMasked(DtuNonIpPort* pNonIpPort,UInt32 Addr, UInt32 Mask,
                                                              UInt32 Shift, UInt32 Value);
DtStatus  DtuNonIpRxReset( DtuDeviceData*  pDvcData, DtuNonIpPort*  pNonIpPort, 
                                                                           Int ResetMode);
DtStatus  DtuNonIpTxReset( DtuDeviceData*  pDvcData, DtuNonIpPort*  pNonIpPort, 
                                                                           Int ResetMode);
DtStatus  DtuGetuFrameSize(DtuNonIpPort*  pNonIpPort, Int*  pUFrameSize);
DtStatus  DtuDemodModeSet(DtuDeviceData*  pDvcData, Bool DemodMode);

void  Dtu3WorkerThread(DtThread* pThread, void* pContext);
DtStatus  Dtu315SetTxCtrl(DtuNonIpPort*  pNonIpPort, Int  TxCtrl);


//DtStatus  DtuNonIpPowerdownPre(DtuNonIpPort* pNonIpPort);
//void  DtuNonIpEstimateRate(DtuNonIpPort* pNonIpPort);
//Int  DtuNonIpGetEstimatedRate(DtuNonIpPort* pNonIpPort);

#endif // _NON_IP_H
