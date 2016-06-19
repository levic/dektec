//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtuIal.h *#*#*#*#*#*#*#*#*# (C) 2011-2016 DekTec
//
// Dtu driver - Implementation of the Linux driver interface.
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

#ifndef __DTU_IAL_H
#define __DTU_IAL_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <linux/usb.h>

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

// DtuIalDeviceStartSeqState
typedef enum _DtuIalDeviceStartSeqState
{
    DEVICE_STARTSEQ_STATE_IDLE = 0,
    DEVICE_STARTSEQ_STATE_RES_INITIALIZED,
    DEVICE_STARTSEQ_STATE_COMMON_INITIALIZED,
    DEVICE_STARTSEQ_STATE_POWERED_UP,
    DEVICE_STARTSEQ_STATE_USBDEV_REGISTERED
} DtuIalDeviceStartSeqState;

// DtuIalDevicePowerSeqState
typedef enum _DtuIalDevicePowerSeqState
{
    DEVICE_POWERSEQ_STATE_DOWN = 0,
    DEVICE_POWERSEQ_STATE_UP,
} DtuIalDevicePowerSeqState;

// DtuIalData
typedef struct _DtuIalData
{
    UInt  m_DvcIndex;
    DtuIalDeviceStartSeqState  m_StartSeqState;
    DtuIalDevicePowerSeqState  m_PowerSeqState;
    struct kref  m_KRef;                            // Keep track of reference counts
    
} DtuIalData;

#endif // __DTU_IAL_H
