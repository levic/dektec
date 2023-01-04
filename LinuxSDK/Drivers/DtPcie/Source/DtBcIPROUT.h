//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIPROUTER.h *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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

#ifndef __DT_BC_IPROUTER_H
#define __DT_BC_IPROUTER_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the DtBcIPROUT block (must match block ID)
#define DT_BC_IPROUT_NAME        "IpRouter"
#define DT_BC_IPROUT_SHORTNAME   "IPROUT"

// MACRO: to init an block-controller-ID for the EMAC10G-BC
#define DT_BC_IPROUT_INIT_ID(ID, ROLE, INSTANCE, UUID)                                   \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_IPROUT_NAME, DT_BC_IPROUT_SHORTNAME, ROLE,                   \
                                                                        INSTANCE, UUID); \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIPROUT definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IPROUT_FLAG_EN_FILT         0x0001
#define DT_IPROUT_FLAG_EN_SRCIP_IPV4   0x0002
#define DT_IPROUT_FLAG_EN_SRCIP_IPV6   0x0004
#define DT_IPROUT_FLAG_EN_SRCPORT0     0x0008
#define DT_IPROUT_FLAG_EN_SRCPORT1     0x0010
#define DT_IPROUT_FLAG_EN_SRCPORT2     0x0020
#define DT_IPROUT_FLAG_EN_DSTIP_IPV4   0x0040
#define DT_IPROUT_FLAG_EN_DSTIP_IPV6   0x0080
#define DT_IPROUT_FLAG_EN_DSTPORT0     0x0100
#define DT_IPROUT_FLAG_EN_DSTPORT1     0x0200
#define DT_IPROUT_FLAG_EN_DSTPORT2     0x0400
#define DT_IPROUT_FLAG_EN_VLAN         0x0800
#define DT_IPROUT_FLAG_DUPLICATE       0x1000
#define DT_IPROUT_FLAG_VLAN0_1AD       0x2000
#define DT_IPROUT_FLAG_VLAN1_1AD       0x4000


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPROUT -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcIPROUT
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached settings
    Bool  m_BlockEnabled;

    UInt8  m_DstIp[16];
    UInt16  m_DstPort[3];
    UInt8  m_SrcIp[16];
    UInt16   m_SrcPort[3];
    Int  m_VlanId[2];
    Int  m_Flags;
} DtBcIPROUT;

//.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcIPROUT public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-

void  DtBcIPROUT_Close(DtBc*);
DtBcIPROUT*  DtBcIPROUT_Open(Int  Address, DtCore*, DtPt*  pPt, const char*  pRole, 
                                                                Int  Instance, Int  Uuid);
DtStatus  DtBcIPROUT_GetIpfilter(DtBcIPROUT* pBc, UInt8 DstIp[16], UInt16 DstPort[3],
                          UInt8 SrcIp[16], UInt16  SrcPort[3], Int* pVlanId, Int* pFlags);
DtStatus  DtBcIPROUT_SetIpfilter(DtBcIPROUT* pBc, UInt8 DstIp[16], UInt16 DstPort[3],
                          UInt8 SrcIp[16], UInt16  SrcPort[3], Int  VlanId[2], Int Flags);
DtStatus  DtBcIPROUT_SetDuplicate(DtBcIPROUT* pBc, Bool Duplicate);
#endif  // #ifndef __DT_BC_IPROUT_H
