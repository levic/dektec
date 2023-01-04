// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIal.h *#*#*#*#*#*#*#*#*# (C) 2018-2022 DekTec
//
// Dta driver - Implementation of the Linux driver interface.
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018-2022 DekTec Digital Video B.V.
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

#ifndef __DT_IAL_H
#define __DT_IAL_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtTypes.h"
#include <linux/poll.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(3,0,0)
#include <linux/ptp_clock_kernel.h>
#define LINUX_KERNEL_PTP_SUPPORT
#endif



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

// DtDeviceStartSeqState
typedef enum _DtDeviceStartSeqState
{
    DEVICE_STARTSEQ_STATE_IDLE = 0,
    DEVICE_STARTSEQ_STATE_RES_INITIALIZED,
    DEVICE_STARTSEQ_STATE_COMMON_INITIALIZED,
    DEVICE_STARTSEQ_STATE_RES_MAPPED,
    DEVICE_STARTSEQ_STATE_POWERED_UP,
    DEVICE_STARTSEQ_STATE_CDEV_ADDED,
    DEVICE_STARTSEQ_STATE_DEV_CREATED
} DtDeviceStartSeqState;

// DtDevicePowerSeqState
typedef enum _DtDevicePowerSeqState
{
    DEVICE_POWERSEQ_STATE_DOWN = 0,
    DEVICE_POWERSEQ_STATE_UP,
    DEVICE_POWERSEQ_STATE_INT_ENABLED,
    DEVICE_POWERSEQ_STATE_UP_POST
} DtDevicePowerSeqState;

// Child devices
typedef struct _DtDeviceItf
{
    struct device*  m_pParentDevice;
    struct _DtDriverItf*  m_pDriverItf;
} DtDeviceItf;

typedef struct _DtDriverItf
{
    const char*  m_pName;            // Child driver name
    void*  m_pContext;               // Parent context. Used with the IoCtrl function
    Int*  m_pPtpClockIndex;        // Pointer to Ptp Clock Index
    
    // Child driver functions
    Int  (*Probe)(DtDeviceItf* pDev, Int Id);
    void  (*Remove)(DtDeviceItf* pDev, Int Id);
   
    // Parent driver functions
    IoCtrlFunc  IoCtrl;     // IoCtrl function    
} DtDriverItf;

typedef struct _DtChildDriver
{
    //DtSpinLock  m_SpinLock;
    DtDriverItf*  m_pDriverItf;
} DtChildDriver;


// DtIalData
typedef struct _DtIalData
{
    UInt  m_DvcIndex;
    DtDeviceStartSeqState  m_StartSeqState;
    DtDevicePowerSeqState  m_PowerSeqState;
    Bool  m_ISRRegistered;
    Int  m_NumIrqVectors;
#if LINUX_VERSION_CODE < KERNEL_VERSION(4,8,0)
    struct msix_entry  MsixEntry[20];               // IRQ Vector store
#endif
#ifdef LINUX_KERNEL_PTP_SUPPORT
    // PTP Clock Driver Interface
    struct ptp_clock_info  m_PtpClockInfo;
    struct ptp_clock*  m_pPtpClock;
#endif
    Int  m_PtpClockIndex;
} DtIalData;

typedef struct _DtIalDataChild
{
    DtDeviceItf  m_DeviceItf;
} DtIalDataChild;

// Exported functions for registering child driver
void  DtCorePcie_IAL_RegisterChildDriver(DtDriverItf* pDriverItf);
void  DtCorePcie_IAL_UnRegisterChildDriver(DtDriverItf* pDriverItf);

#endif // __DT_IAL_H
