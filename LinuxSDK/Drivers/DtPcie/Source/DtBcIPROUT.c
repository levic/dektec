//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIPROUT.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"
#include "DtBcIPROUT.h"
#include "DtBcIPROUT_RegAccess.h"
#include "EthPrtcls.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIPROUT implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IPROUT_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIPROUT))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IPROUT_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IPROUT, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIPROUT_Init(DtBc*);
static DtStatus  DtBcIPROUT_OnCloseFile(DtBc*, const DtFileObject*);

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIPROUT - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPROUT_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcIPROUT_Close(DtBc*  pBc)
{
    BC_IPROUT_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPROUT_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIPROUT*  DtBcIPROUT_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_IPROUT_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIPROUT, Id, DT_BLOCK_TYPE_IPROUT,
                                                              Address, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIPROUT_Close;
    OpenParams.m_InitFunc = DtBcIPROUT_Init;
    OpenParams.m_OnCloseFileFunc = DtBcIPROUT_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIPROUT*)DtBc_Open(&OpenParams);
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIPROUT - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPROUT_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIPROUT_Init(DtBc* pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIPROUT* pBc = (DtBcIPROUT*)pBcBase;
    
    DtDbgOutBc(MAX, IPROUT, pBc, "DtBcIPROUT_Init");

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_Flags = 0;

    // Disable filter
    IPROUT_Control_WRITE(pBc, 0);

    DtDbgOutBc(MAX, IPROUT, pBc, "DtBcIPROUT_Init done");

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPROUT_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIPROUT_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIPROUT* pBc = (DtBcIPROUT*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IPROUT_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IPROUT, pBc, "Go to IDLE");

        // Disable filter
        IPROUT_Control_WRITE(pBc, 0);
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPROUT_GetIpfilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIPROUT_GetIpfilter(DtBcIPROUT* pBc, UInt8 DstIp[16], UInt16 DstPort[3],
    UInt8 SrcIp[16], UInt16  SrcPort[3], Int* pVlanId, Int* pFlags)
{
    Int  i;
    // Get local settings
    for (i = 0; i < 4; i++)
    {
        ((UInt32*)DstIp)[i] = ((UInt32*)pBc->m_DstIp)[i];
        ((UInt32*)SrcIp)[i] = ((UInt32*)pBc->m_SrcIp)[i];
    }
    for (i = 0; i < 3; i++)
    {
        DstPort[i] = pBc->m_DstPort[i];
        SrcPort[i] = pBc->m_SrcPort[i];
    }
    pVlanId[0] = pBc->m_VlanId[0];
    pVlanId[1] = pBc->m_VlanId[1];
    *pFlags = pBc->m_Flags;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPROUT_SetIpfilter -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIPROUT_SetIpfilter(DtBcIPROUT* pBc, UInt8 DstIp[16], UInt16 DstPort[3],
    UInt8 SrcIp[16], UInt16  SrcPort[3], Int  VlanId[2], Int Flags)
{
    UInt32  ControlReg = IPROUT_Control_READ(pBc);
    Int  i;
    UInt32  MatchPort;
    UInt32  VLanMatch;
    UInt32  VLanReg;

    // First check if we have to disable the filter
    if ((Flags & DT_IPROUT_FLAG_EN_FILT) == 0)
    {
        // Disable filter
        ControlReg = IPROUT_Control_SET_FilterEnable(ControlReg, 0);
        ControlReg = IPROUT_Control_SET_MatchSrcIp(ControlReg, 0);
        ControlReg = IPROUT_Control_SET_MatchDestIp(ControlReg, 0);
        ControlReg = IPROUT_Control_SET_MatchSrcPort(ControlReg, 0);
        ControlReg = IPROUT_Control_SET_MatchDestPort(ControlReg, 0);
        ControlReg = IPROUT_Control_SET_MatchVlan(ControlReg, 0);
        ControlReg = IPROUT_Control_SET_Duplicate(ControlReg, 0);
        IPROUT_Control_WRITE(pBc, ControlReg);
    }

    // Write port numbers
    for (i = 0; i < 3; i++)
    {
        UInt32  PortData = IPROUT_Port_Indexed_READ(pBc, i);
        PortData = IPROUT_Port_Indexed_SET_SrcPort(PortData, SrcPort[i]);
        PortData = IPROUT_Port_Indexed_SET_DestPort(PortData, DstPort[i]);
        IPROUT_Port_Indexed_WRITE(pBc, PortData, i);
    }
    // Write dst IP addresses
    for (i = 0; i < 4; i++)
    {
        IPROUT_DestIp_Indexed_WRITE_DestIp(pBc, NWCDW(((UInt32*)DstIp)[i]), i);
    }
    // Write src IP addresses
    for (i = 0; i < 4; i++)
    {
        IPROUT_SrcIp_Indexed_WRITE_SrcIp(pBc, NWCDW(((UInt32*)SrcIp)[i]), i);
    }

    // src IP enable
    ControlReg = 0;
    if ((Flags & DT_IPROUT_FLAG_EN_SRCIP_IPV4) != 0)
        ControlReg = IPROUT_Control_SET_MatchSrcIp(ControlReg, IPROUT_IPMATCH_IPV4);
    else if ((Flags & DT_IPROUT_FLAG_EN_SRCIP_IPV6) != 0)
        ControlReg = IPROUT_Control_SET_MatchSrcIp(ControlReg, IPROUT_IPMATCH_IPV6);
    else
        ControlReg = IPROUT_Control_SET_MatchSrcIp(ControlReg, IPROUT_IPMATCH_IGNORE);

    // dst IP enable
    if ((Flags & DT_IPROUT_FLAG_EN_DSTIP_IPV4) != 0)
        ControlReg = IPROUT_Control_SET_MatchDestIp(ControlReg, IPROUT_IPMATCH_IPV4);
    else if ((Flags & DT_IPROUT_FLAG_EN_DSTIP_IPV6) != 0)
        ControlReg = IPROUT_Control_SET_MatchDestIp(ControlReg, IPROUT_IPMATCH_IPV6);
    else
        ControlReg = IPROUT_Control_SET_MatchDestIp(ControlReg, IPROUT_IPMATCH_IGNORE);

    // src port
    MatchPort = 0;
    if ((Flags & DT_IPROUT_FLAG_EN_SRCPORT0) != 0)
        MatchPort |= (1 << 0);
    if ((Flags & DT_IPROUT_FLAG_EN_SRCPORT1) != 0)
        MatchPort |= (1 << 1);
    if ((Flags & DT_IPROUT_FLAG_EN_SRCPORT2) != 0)
        MatchPort |= (1 << 2);
    ControlReg = IPROUT_Control_SET_MatchSrcPort(ControlReg, MatchPort);

    // dst port
    MatchPort = 0;
    if ((Flags & DT_IPROUT_FLAG_EN_DSTPORT0) != 0)
        MatchPort |= (1 << 0);
    if ((Flags & DT_IPROUT_FLAG_EN_DSTPORT1) != 0)
        MatchPort |= (1 << 1);
    if ((Flags & DT_IPROUT_FLAG_EN_DSTPORT2) != 0)
        MatchPort |= (1 << 2);
    ControlReg = IPROUT_Control_SET_MatchDestPort(ControlReg, MatchPort);

    // duplicate
    if ((Flags & DT_IPROUT_FLAG_DUPLICATE) != 0)
        ControlReg = IPROUT_Control_SET_Duplicate(ControlReg, 1);

    // enable filter
    if (Flags & DT_IPROUT_FLAG_EN_FILT)
        ControlReg = IPROUT_Control_SET_FilterEnable(ControlReg, 1);


    // Vlan setup
    VLanReg = 0;
    VLanMatch = IPROUT_VLANMATCH_NONE;
    DT_ASSERT(VlanId[0] >= 0);
    if (VlanId[0] != 0)
    {
        VLanMatch = IPROUT_VLANMATCH_ONELEVEL;
        VLanReg = IPROUT_Vlan_SET_Id1(VLanReg, VlanId[0]);
        if ((Flags & DT_IPROUT_FLAG_VLAN0_1AD) != 0)
            VLanReg = IPROUT_Vlan_SET_Tpid1(VLanReg, IPROUT_TPID_V802_1ad);
        else
            VLanReg = IPROUT_Vlan_SET_Tpid1(VLanReg, IPROUT_TPID_V802_1Q);

        DT_ASSERT(VlanId[1] >= 0);

        if (VlanId[1] != 0)
        {
            VLanMatch = IPROUT_VLANMATCH_TWOLEVELS;
            VLanReg = IPROUT_Vlan_SET_Id2(VLanReg, VlanId[1]);
            if ((Flags & DT_IPROUT_FLAG_VLAN1_1AD) != 0)
                VLanReg = IPROUT_Vlan_SET_Tpid2(VLanReg, IPROUT_TPID_V802_1ad);
            else
                VLanReg = IPROUT_Vlan_SET_Tpid2(VLanReg, IPROUT_TPID_V802_1Q);
        }
    }
    IPROUT_Vlan_WRITE(pBc, VLanReg);
    ControlReg = IPROUT_Control_SET_MatchVlan(ControlReg, VLanMatch);
    
    // Write the enables
    IPROUT_Control_WRITE(pBc, ControlReg);


    // Store settings local
    for (i = 0; i < 4; i++)
    {
        ((UInt32*)pBc->m_DstIp)[i] = ((UInt32*)DstIp)[i];
        ((UInt32*)pBc->m_SrcIp)[i] = ((UInt32*)SrcIp)[i];
    }
    for (i = 0; i < 3; i++)
    {
        pBc->m_DstPort[i] = DstPort[i];
        pBc->m_SrcPort[i] = SrcPort[i];
    }
    pBc->m_VlanId[0] = VlanId[0];
    pBc->m_VlanId[1] = VlanId[1];
    pBc->m_Flags = Flags;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPROUT_SetDuplicate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIPROUT_SetDuplicate(DtBcIPROUT* pBc, Bool Duplicate)
{
    UInt32  ControlReg = IPROUT_Control_READ(pBc);

    if (Duplicate)
    {
        pBc->m_Flags |= DT_IPROUT_FLAG_DUPLICATE;
        ControlReg = IPROUT_Control_SET_Duplicate(ControlReg, 1);
    }
    else
    {
        pBc->m_Flags &= ~DT_IPROUT_FLAG_DUPLICATE;
        ControlReg = IPROUT_Control_SET_Duplicate(ControlReg, 0);
    }
    IPROUT_Control_WRITE(pBc, ControlReg);

    return DT_STATUS_OK;
}
