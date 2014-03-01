//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* NonIp.h *#*#*#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//
// Dtu driver - Non IP functionality - Declaration of generic non IP port functionality
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2012 DekTec Digital Video B.V.
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

#define  MAX_NONIP_PORTS  10

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
    Bool  m_CapAsi;
    Bool  m_CapDemod;
    Bool  m_CapIp;
    Bool  m_CapMod;
    Bool  m_CapSdi;
    Bool  m_CapSpi;
    Bool  m_CapSpiSdi;
    // IOSTD - SDI (SD-SDI) - Sub capabilities
    Bool  m_Cap525I59_94;
    Bool  m_Cap625I50;
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
    
    //DtSpinLock  m_FlagsSpinLock;

} DtuNonIpPort;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtuNonIpCleanup(DtuDeviceData* pDvcData, Int PortIndex, DtuNonIpPort* pNonIpPort);
void  DtuNonIpCleanupPre(DtuDeviceData* pDvcData, Int PortIndex,
                                                                DtuNonIpPort* pNonIpPort);
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
DtStatus  Dtu236DemodModeSet(DtuDeviceData*  pDvcData, Bool DemodMode);


//DtStatus  DtuNonIpPowerdownPre(DtuNonIpPort* pNonIpPort);
//void  DtuNonIpEstimateRate(DtuNonIpPort* pNonIpPort);
//Int  DtuNonIpGetEstimatedRate(DtuNonIpPort* pNonIpPort);

#endif // _NON_IP_H
