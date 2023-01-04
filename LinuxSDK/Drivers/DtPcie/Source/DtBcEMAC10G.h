//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcEMAC10GE.h *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
// This file is not part of the PCIe driver. It can be used as template to create a new
// block controller. Replace "TEMPLATE" by the building block's shortname (all uppercase)
// and replace "Template" by the building block's (long) name.
// 

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018 DekTec Digital Video B.V.
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

#ifndef __DT_BC_EMAC10G_H
#define __DT_BC_EMAC10G_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the DtBcEMAC10G block (must match block ID)
#define DT_BC_EMAC10G_NAME        "EthMac10G"
#define DT_BC_EMAC10G_SHORTNAME   "EMAC10G"

// MACRO: to init an block-controller-ID for the EMAC10G-BC
#define DT_BC_EMAC10G_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_EMAC10G_NAME, DT_BC_EMAC10G_SHORTNAME, ROLE,               \
                                                                        INSTANCE, UUID); \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcEMAC10G definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC10G -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcEMAC10G
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached settings
    Bool  m_BlockEnabled;
    Int  m_OperationalMode;

    // Interrupts related
    DtDpc  m_IntDpc;
    DtEvent  m_DoneEvent;

    // MAC address
    UInt8  m_MacAddrPer[6];             // Network MAC address retrieved from VPD
    UInt8  m_MacAddrCur[6];             // Current Network MAC address. Can be changed

    // Link status function callback
    DtLinkStatFunc  m_LinkStatCB;
    void*  m_pContexLinkStatCB;
    DtSpinLock  m_LinkStatCBSpinlock;
} DtBcEMAC10G;

//.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcEMAC10G public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-

void  DtBcEMAC10G_Close(DtBc*);
DtBcEMAC10G*  DtBcEMAC10G_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                            const char*  pRole, Int  Instance, Int  Uuid);
DtStatus DtBcEMAC10G_GetOperationalMode(DtBcEMAC10G* pBc, Int* pOpMode);
DtStatus DtBcEMAC10G_SetOperationalMode(DtBcEMAC10G* pBc, Int OpMode);
DtStatus  DtBcEMAC10G_GetCounter(DtBcEMAC10G* pBc, const UInt  CounterId,
                                                                          UInt64* pValue);
DtStatus  DtBcEMAC10G_GetPhySpeed(DtBcEMAC10G* pBc, UInt* pPhySpeed);
DtStatus  DtBcEMAC10G_GetMacAddressPerm(DtBcEMAC10G* pBc, UInt8* pMacAddress);
DtStatus  DtBcEMAC10G_GetMacAddressCurr(DtBcEMAC10G* pBc, UInt8* pMacAddress);
DtStatus  DtBcEMAC10G_SetMacAddress(DtBcEMAC10G* pBc, UInt8* pMacAddress);
DtStatus  DtBcEMAC10G_RegisterCallback(DtBcEMAC10G* pBc, DtLinkStatFunc LinkStatCB, 
                                                                          void* pContext);
DtStatus  DtBcEMAC10G_UnregisterCallback(DtBcEMAC10G* pBc, void* pContext);
DtStatus  DtBcEMAC10G_GetLinkStatus(DtBcEMAC10G* pBc);
#endif  // #ifndef __DT_BC_EMAC10G_H
