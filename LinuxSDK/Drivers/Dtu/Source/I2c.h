//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* I2c.h *#*#*#*#*#*#*#*#*#*# (C) 2011-2016 DekTec
//
// Dtu driver - Dtu I2C read/write routines.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2016 DekTec Digital Video B.V.
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
// DtuI2c
typedef struct _DtuI2c
{
    // I2c properties
    Bool  m_IsSupported;

    // Implementation data
    DtFastMutex  m_AccessMutex;
    DtMutex  m_LockMutex;
    DtEvent m_LockEvent;

    void*  m_pLockOwner;
    Int  m_LockCount;
} DtuI2c;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
DtStatus  DtuI2cInit(DtuDeviceData* pDvcData);
void  DtuI2cCleanup(DtuDeviceData* pDvcData);
DtStatus  DtuI2cIoctl(DtuDeviceData* pDvcData, DtFileObject* pFile,
                                                                   DtIoctlObject* pIoctl);
DtStatus  DtuI2cLock(DtuDeviceData* pDvcData, DtFileObject* pFile, Int TimeoutMS);
DtStatus  DtuI2cUnlock(DtuDeviceData* pDvcData, DtFileObject* pFile, Bool Recursive);
DtStatus  DtuI2cRead(DtuDeviceData* pDvcData, DtFileObject* pFile, UInt DvcAddr,
                                                                UInt Length, UInt8* pBuf);
DtStatus  DtuI2cWrite(DtuDeviceData* pDvcData, DtFileObject* pFile, UInt DvcAddr,
                                                                UInt Length, UInt8* pBuf);

#endif // __I2C_H

