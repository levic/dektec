//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* I2c.h *#*#*#*#*#*#*#*#*#*# (C) 2010-2016 DekTec
//
// Dta driver - Dta I2C read/write routines.
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

#ifndef __I2C_H
#define __I2C_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// DtaI2c
typedef struct _DtaI2c
{
    // I2c properties
    Bool  m_IsSupported;

    
    UInt16  m_RegOffset;
    // Start adress of the I2c controller
    volatile UInt8*  m_pI2cRegs;

    // Additional tasks to be peformed upon succesfull (un)lock. Runs inside m_I2cMutex
    // critcal section and must be fast.
    void  (*m_pI2cLockFunction)(DtaDeviceData* pDvcData, Int PortIndex, Int Lock);

    // Implementation data
    DtFastMutex  m_AccessMutex;
    DtMutex  m_LockMutex;
    DtEvent  m_LockEvent;

    DtEvent  m_I2cDoneEvent;
    volatile Bool  m_Done;
    DtDpc  m_I2cCompletedDpc;

    void*  m_pLockOwner;
    Int  m_LockCount;
    Int  m_ClockFreq;
} DtaI2c;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtaI2cInit(DtaDeviceData* pDvcData);
DtStatus  DtaI2cLockCallBackSet(DtaDeviceData* pDvcData, Int PortIndex, 
                         void (*pFunc)(DtaDeviceData* pDvcData, Int PortIndex, Int Lock));
void  DtaI2cCleanup(DtaDeviceData* pDvcData);
DtStatus  DtaI2cPowerUp(DtaDeviceData* pDvcData);
DtStatus  DtaI2cIoctl(DtaDeviceData* pDvcData, DtFileObject* pFile,
                                                                   DtIoctlObject* pIoctl);
DtStatus  DtaI2cLock(DtaDeviceData* pDvcData, Int PortIndex, DtFileObject* pFile, 
                                                                           Int TimeoutMS);
DtStatus  DtaI2cUnlock(DtaDeviceData* pDvcData, Int PortIndex, DtFileObject* pFile, 
                                                                          Bool Recursive);
DtStatus  DtaI2cRead(DtaDeviceData* pDvcData, Int PortIndex, DtFileObject* pFile, 
                                                  UInt DvcAddr, UInt Length, UInt8* pBuf);
DtStatus  DtaI2cWrite(DtaDeviceData* pDvcData, Int PortIndex, DtFileObject* pFile, 
                                                  UInt DvcAddr, UInt Length, UInt8* pBuf);
DtStatus  DtaI2cWriteRead(DtaDeviceData* pDvcData, Int PortIndex, DtFileObject* pFile,
                  UInt DvcWriteAddr, UInt WriteLength, UInt8* pWriteBuf, UInt DvcReadAddr, 
                  UInt ReadLength, UInt8* pReadBuf);


//-.-.-.-.-.-.-.-.-.-.-.-.- I2CM firmware block type definitions -.-.-.-.-.-.-.-.-.-.-.-.-


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaI2cm -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtaI2cm
{
    const DtFwbI2cMaster*  m_pFwbI2cm;
    volatile UInt8*  m_pFwbRegs;

    DtEvent  m_DoneEvent;
    DtDpc  m_CompletedDpc;

    // For now we assume that access control on this block is not required. When it
    // becomes necessary we'll need to add it here analogous to the DtaI2c implementation.
} DtaI2cm;

DtStatus  DtaI2cmInit(DtaI2cm* pI2cm, const DtFwbI2cMaster* pFwbI2cm);
DtStatus  DtaI2cmInitPowerup(DtaI2cm* pI2cm, volatile UInt8* pFwbRegs);
DtStatus  DtaI2cmInterruptEnable(DtaI2cm* pNonIpPort);
DtStatus  DtaI2cmInterruptDisable(DtaI2cm* pNonIpPort);
Bool  DtaI2cmInterrupt(DtaI2cm*  pNonIpPort);
DtStatus  DtaI2cmRead(DtaI2cm* pI2cm, UInt DvcAddr, UInt Length, UInt8* pBuf);
DtStatus  DtaI2cmWrite(DtaI2cm* pI2cm, UInt DvcAddr, UInt Length, const UInt8* pBuf);
DtStatus  DtaI2cmWriteRead(DtaI2cm* pI2cm, UInt DvcAddr,
                                                 UInt WriteLength, const UInt8* pWriteBuf,
                                                 UInt ReadLength, UInt8* pReadBuf);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- I2CM firmware block functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#endif // __I2C_H
