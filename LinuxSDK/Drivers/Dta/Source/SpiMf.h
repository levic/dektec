//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* SpiMf.h *#*#*#*#*#*#*#*#*#* (C) 2015-2016 DekTec
//
// Dta driver - Declares SpiMasterFlash related functions
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

#ifndef __SPIMF_H
#define __SPIMF_H


// Callback typedef
typedef DtStatus  (*pSpiMfCallback)(DtaNonIpPort* pNonIpPort, DtFileObject*  pFile);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaSpiMf -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
typedef struct
{
    const DtFwbSpiMasterFlash*  m_pFwbSpiMf;
    volatile UInt8*  m_pFwbRegs;
    DtaDeviceData* m_pDvcData;
    DtDpc  m_IntDpc;
    DtEvent  m_OpComplEvent; // Event used to indicate the operation is completed
    Bool m_IsSupported;     // True if this device/port supports spi master flash
    Bool m_IsInitialized;   // True if correctly initialized

    // Callback function for preparation and release of SpiMasterflash usage
    pSpiMfCallback m_pSpiMfPrepare;
    pSpiMfCallback m_pSpiMfRelease;

    // SpiMasterFlash properties
    Int  m_SpiClockRate;    // In units of 100kHz
    Int  m_MemoryId;        // Identifier for the Flash/Prom
    Int  m_PageSize;        // Page size in #bytes
    Int  m_SectorSize;      // Sector size in #bytes; 1 means the device has no sectors
    Int  m_MemorySize;      // Memory size in #bytes 
    Int  m_TxFifoSize;      // Transmit FIFO size in #bytes alsways multiple of 4
    Int  m_RxFifoSize;      // Receive FIFO size in #bytes alsways multiple of 4

    // Read/Write/Erase  state
    Int  m_State;           // Operation in progress
    Int  m_NumToDo;         // Number of bytes to do
    UInt32* m_pData;        // Data pointer
    DtStatus m_Result;      // Current result
    

    // Exclusive access
    Bool  m_ExclAccess;
    DtFileObject  m_ExclAccessOwner;
    DtSpinLock  m_ExclAccessLock;
} DtaSpiMf;


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Used from device-level
DtStatus  DtaSpiMfDvcInitPowerup(DtaDeviceData* pDvcData);
DtStatus  DtaSpiMfDvcInit(DtaDeviceData* pDvcData);
// Used from port-level
DtStatus  DtaSpiMfInitPowerup(DtaNonIpPort* pNonIpPort);
DtStatus  DtaSpiMfInit(DtaSpiMf* pSpiMf, pSpiMfCallback pSpiMfPrepare, 
                      pSpiMfCallback pSpiMfRelease, const DtFwbSpiMasterFlash* pFwbSpiMf);

// Used from port and device level
DtStatus  DtaSpiMfIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile,
                                           DtIoctlObject* pIoctl, Bool PowerDownPending);
DtStatus  DtaSpiMfClose(DtaSpiMf* pSpiMf, DtaNonIpPort* pNonIpPort, DtFileObject* pFile);
Bool      DtaSpiMfInterrupt(DtaSpiMf*  pSpiMf);

#endif // __SPIMF_H
