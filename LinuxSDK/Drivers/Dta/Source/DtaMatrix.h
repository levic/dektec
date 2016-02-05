//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaMatrix.h *#*#*#*#*#*#*#*#* (C) 2012-2016 DekTec
//
// Dta driver - Declares Matrix-API (i.e. HD-SDI) related functions

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

#ifndef __DTA_MATRIX_H
#define __DTA_MATRIX_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// DtaMatrix
typedef struct _DtaMatrix
{
    // Matrix properties
    Bool  m_IsSupported;

    Int  m_RamSize;             // Size of the onboard RAM (SDRAM/DDR) buffer in MB
    Int  m_MemPckSize;          // Size (in # bits) of super packets stored in memory
    Int  m_MemPckNumBitsUsed;   // Number of bits used per super packet
    Int  m_MemLineAlignment;    // Aligment (in # bits) used for each line stored in the 
                                // frame buffer 

    // Status/Control attributes
    volatile Int64  m_SofFrame;  // Latched-SOF frame

    DtaIoctlMatrixCmdGetSyncInfoOutput  m_SyncInfo;
    DtSpinLock  m_SyncInfoSpinLock;

    DtDpc  m_SofFrameIntDpc;
    DtEvent  m_SofFrameIntEvent;
    DtEvent  m_SofFrameSyncEvent;
    
} DtaMatrix;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaMatrixInit(DtaDeviceData*  pDvcData);
DtStatus  DtaMatrixIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile,
                                           DtIoctlObject* pIoctl, Bool  PowerDownPending);
DtStatus  DtaMatrixAsiRxGetFifoLoad(DtaNonIpPort*  pNonIpPort, Int*  pFifoLoad);
DtStatus  DtaMatrixGetVpid(DtaNonIpPort*  pNonIpPort, UInt* pVpid, UInt* pVpid2);

#endif // __DTA_MATRIX_H
