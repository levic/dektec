// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcEMAC25G.h *#*#*#*#*#*#*#* (C) 2018-2023 DekTec
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

#ifndef __DT_BC_EMAC25G_H
#define __DT_BC_EMAC25G_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Forward declaration of SfpIntFunc
typedef void (*DtEMAC25GSfpIntFunc)(UInt SfpStatus, void* pContext);

// Name and short-name of the DtBcEMAC25G block (must match block ID)
#define DT_BC_EMAC25G_NAME        "EthMac25G"
#define DT_BC_EMAC25G_SHORTNAME   "EMAC25G"

// MACRO: to init an block-controller-ID for the EMAC25G-BC
#define DT_BC_EMAC25G_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_EMAC25G_NAME, DT_BC_EMAC25G_SHORTNAME, ROLE,               \
                                                                        INSTANCE, UUID); \
}                                                                                        \
while (0)

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcEMAC25G definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
typedef enum  _LinkDetectStateEmac25G
{
    IDLE,             // Not yet started
    INIT,             // Lets start
    WAIT_FOR_SFP,     // Wait for SFP module
    READ_SFP,         // SFP detected, read SFP EEPROM module
    WAIT_FOR_CAP,     // Wait for capability read and set speed
    SET_SPEED,        // Program new linespeed
    WAIT_FOR_LINKUP,  // Wait for link or timeout
    LINKUP,           // Ready, link is up
    CONFIG_ERROR,     // Cap SFP and manual link does not match
}  LinkDetectStateEmac25G;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcEMAC25G -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcEMAC25G
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;

    // Cached settings
    Bool  m_BlockEnabled;
    Int  m_OperationalMode;
    //Int  m_DisableMac;

    // Interrupts related
    DtDpc  m_LinkChangeDpc;
    DtDpc  m_SetNetworkRateDpc;
    
    // MAC address
    UInt8  m_MacAddrPer[6];             // Network MAC address retrieved from VPD
    UInt8  m_MacAddrCur[6];             // Current Network MAC address. Can be changed

    // Link status function callback
    DtLinkStatFunc  m_LinkStatCB;
    void*  m_pContexLinkStatCB;
    DtSpinLock  m_LinkStatCBSpinlock;
    UInt  m_LinkSpeedTry;                // Set link speed to this one and check if link is up
    UInt  m_LinkSpeedTryCount;           // 
    Bool  m_SfpAbsentChanged;

    UInt  m_LinkSpeedCurr;               // Current link speed detected
    UInt  m_LinkSpeedNw;                 // Link speed set by user/network driver
    UInt  m_LinkStatNw;                  // Current link status to network driver
    UInt  m_LinkStat;                    // Current link status up/down
    UInt  m_SfpCap;                      // Sfp capability
    LinkDetectStateEmac25G  m_LinkDetectState;
    DtEMAC25GSfpIntFunc  m_SfpIntCB;     // SFP interrupt callback function
    void*  m_pContexSfpIntCB;            // SFP callback context
    DtWorkItem  m_SfpIntWorkItem;        // SFP interrupt work item
} DtBcEMAC25G;

//.-.-.-.-.-.-.-.-.-.-.-.-.-. DtBcEMAC25G public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-

void  DtBcEMAC25G_Close(DtBc*);
DtBcEMAC25G*  DtBcEMAC25G_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                            const char*  pRole, Int  Instance, Int  Uuid);
DtStatus DtBcEMAC25G_GetOperationalMode(DtBcEMAC25G* pBc, Int* pOpMode);
DtStatus DtBcEMAC25G_SetOperationalMode(DtBcEMAC25G* pBc, Int OpMode);
DtStatus  DtBcEMAC25G_GetCounter(DtBcEMAC25G* pBc, const UInt  CounterId,
                                                                          UInt64* pValue);
DtStatus  DtBcEMAC25G_GetSupportedCounters(DtBcEMAC25G* pBc, UInt MaxCounterIds, 
                                                 UInt* pNumCounterIds, UInt* pCounterIds);
DtStatus  DtBcEMAC25G_GetPhySpeed(DtBcEMAC25G* pBc, UInt* pPhySpeed);
DtStatus  DtBcEMAC25G_SetPhySpeed(DtBcEMAC25G* pBc, UInt PhySpeed);
DtStatus  DtBcEMAC25G_GetMacAddressPerm(DtBcEMAC25G* pBc, UInt8* pMacAddress);
DtStatus  DtBcEMAC25G_GetMacAddressCurr(DtBcEMAC25G* pBc, UInt8* pMacAddress);
DtStatus  DtBcEMAC25G_SetMacAddress(DtBcEMAC25G* pBc, UInt8* pMacAddress);
DtStatus  DtBcEMAC25G_RegisterLinkCallback(DtBcEMAC25G* pBc, DtLinkStatFunc LinkStatCB, 
                                                                          void* pContext);
DtStatus  DtBcEMAC25G_UnregisterLinkCallback(DtBcEMAC25G* pBc, void* pContext);
DtStatus  DtBcEMAC25G_GetLinkStatus(DtBcEMAC25G* pBc);
void DtBcEMAC25G_SetSfpCapabilities(DtBcEMAC25G* pBc, UInt Capabilities);
void DtBcEMAC25G_SetSfpIntCallbackFunc(DtBcEMAC25G* pBc, DtEMAC25GSfpIntFunc SfpIntCB,
                                                                          void* pContext);
DtStatus DtBcEMAC25G_EnableJumboFrames(DtBcEMAC25G* pBc, Bool Enable);
Bool DtBcEMAC25G_SupportsJumboFrames(DtBcEMAC25G* pBc);

#endif  // #ifndef __DT_BC_EMAC25G_H
