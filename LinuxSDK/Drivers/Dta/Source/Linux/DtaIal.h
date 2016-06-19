//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaIal.h *#*#*#*#*#*#*#*#*# (C) 2010-2016 DekTec
//
// Dta driver - Implementation of the Linux driver interface.
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

#ifndef __DTA_IAL_H
#define __DTA_IAL_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#ifdef CONFIG_COMPAT
    // Includes for 32-bit applications using 64-bit driver
    #if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,28)
    #include <asm/compat.h>             // compat_ptr
    #else 
        #if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24)
        #include <asm-x86/compat.h>     // compat_ptr
        #else
        #include <asm-x86_64/compat.h>  // compat_ptr
        #endif
    #endif
#endif // #ifdef CONFIG_COMPAT

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

// DtaIalDeviceStartSeqState
typedef enum _DtaIalDeviceStartSeqState
{
    DEVICE_STARTSEQ_STATE_IDLE = 0,
    DEVICE_STARTSEQ_STATE_RES_INITIALIZED,
    DEVICE_STARTSEQ_STATE_COMMON_INITIALIZED,
    DEVICE_STARTSEQ_STATE_RES_MAPPED,
    DEVICE_STARTSEQ_STATE_POWERED_UP,
    DEVICE_STARTSEQ_STATE_CDEV_ADDED,
    DEVICE_STARTSEQ_STATE_DEV_CREATED
} DtaIalDeviceStartSeqState;

// DtaIalDevicePowerSeqState
typedef enum _DtaIalDevicePowerSeqState
{
    DEVICE_POWERSEQ_STATE_DOWN = 0,
    DEVICE_POWERSEQ_STATE_UP,
    DEVICE_POWERSEQ_STATE_INT_ENABLED,
    DEVICE_POWERSEQ_STATE_UP_POST
} DtaIalDevicePowerSeqState;

// Child devices
typedef struct _DtaDeviceItf
{
    struct device*  m_pParentDevice;
    struct _DtaDriverItf*  m_pDriverItf;
} DtaDeviceItf;

typedef struct _DtaDriverItf
{
    void*  m_pContext;
    const char*  m_pName;               // Child driver name
    // Child driver functions
    Int  (*Probe)(DtaDeviceItf* pDev, Int Id);
    void  (*Remove)(DtaDeviceItf* pDev, Int Id);
    
    // Parent driver functions
    IoCtrlFunc  IoCtrl;     // IoCtrl function
    
} DtaDriverItf;

typedef struct _DtaChildDriver
{
    //DtSpinLock  m_SpinLock;
    DtaDriverItf*  m_pDriverItf;
} DtaChildDriver;

// DtaIalData
typedef struct _DtaIalData
{
    UInt  m_DvcIndex;
    DtaIalDeviceStartSeqState  m_StartSeqState;
    DtaIalDevicePowerSeqState  m_PowerSeqState;
    Bool  m_ISRRegistered;

} DtaIalData;

typedef struct _DtaIalDataChild
{
    DtaDeviceItf  m_DeviceItf;
} DtaIalDataChild;

void  DektecRegisterChildDriver(DtaDriverItf* pDriverItf);
void  DektecUnRegisterChildDriver(DtaDriverItf* pDriverItf);

#endif // __DTA_IAL_H
