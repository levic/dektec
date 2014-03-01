//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtaNwIal.h *#*#*#*#*#*#*#*#* (C) 2011-2012 DekTec
//
// DtaNw driver - Definition of the Linux Network driver interface.
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

#ifndef __DTANW_IAL_H
#define __DTANW_IAL_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/skbuff.h>
#include <linux/ethtool.h>
#include "DtaIal.h" // needed for child devices 
//-.-.-.-.-.-.-.-.-.-.- Detect kernel version and check if supported -.-.-.-.-.-.-.-.-.-.-
//
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,14) && LINUX_VERSION_CODE < KERNEL_VERSION(2,6,23)
    #define PERM_ADDR_SUPPORT
#endif

// The HAVE_NETDEV_PRIV define has been removed from new kernel versions
// The function is still there.
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,40)
#ifndef HAVE_NETDEV_PRIV
#define HAVE_NETDEV_PRIV
static inline void *netdev_priv(struct net_device *dev)
{
    return dev->priv;
}
#endif // HAVE_NETDEV_PRIV
#endif // LINUX_VERSION_CODE < KERNEL_VERSION(2,6,40)

#ifndef netdev_mc_count
    #define netdev_mc_count(dev) ((dev)->mc_count)
#endif

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Type definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//

// DtaNwIalData
typedef struct _DtaNwIalData
{
    UInt  m_DvcIndex;
    Int  m_Id;
    DtaDeviceItf*  m_pDeviceItf;
    struct net_device*  m_pNwDevice;
    struct net_device_stats  m_NetStats;    // Network statistics
    DtDrvObject  m_Driver;
} DtaNwIalData;

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Function callbacks to IAL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtaNwEvtLinkStatusCallback(DtaNwDeviceData* pDvcData);
void  DtaNwEvtNewPacketRxCallback(DtaNwDeviceData* pDvcData, UInt8* pPacket, 
                                                                       UInt PacketLength);

#endif // __DTANW_IAL_H
