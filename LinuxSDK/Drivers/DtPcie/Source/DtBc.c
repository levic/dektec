// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBc.c *#*#*#*#*#*#*#*#*#* (C) 2017-2022 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2017-2022 DekTec Digital Video B.V.
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
#define DT_BC_ADD_BLOCK_INCLUDES
#include "DtPt.h"
#include "DtDf.h"
#include "DtBc.h"
#include "DtBc_RegAccess.h"
#include "Messages.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBc implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc
#define BC_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size>=sizeof(DtBc))

#ifdef DT_BC_ADD_BLOCK_INCLUDES

// List of all blocks we know. NOTE: Role, Instance and UUID are not used
const DtBcId  DT_BC_KNOWN[] = 
{
    // Name,                      Short-name,                     Role, Instance, Uuid
    { DT_BC_ACCUFIFO_NAME,        DT_BC_ACCUFIFO_SHORTNAME,        NULL, -1, -1 },
    { DT_BC_ASIRXP_NAME,          DT_BC_ASIRXP_SHORTNAME,          NULL, -1, -1 },
    { DT_BC_ASITXG_NAME,          DT_BC_ASITXG_SHORTNAME,          NULL, -1, -1 },
    { DT_BC_ASITXSER_NAME,        DT_BC_ASITXSER_SHORTNAME,        NULL, -1, -1 },
    { DT_BC_BURSTFIFO_NAME,       DT_BC_BURSTFIFO_SHORTNAME,       NULL, -1, -1 },
    { DT_BC_CDMAC_NAME,           DT_BC_CDMAC_SHORTNAME,           NULL, -1, -1 },
    { DT_BC_CONSTSINK_NAME,       DT_BC_CONSTSINK_SHORTNAME,       NULL, -1, -1 },
    { DT_BC_CONSTSOURCE_NAME,     DT_BC_CONSTSOURCE_SHORTNAME,     NULL, -1, -1 },
    { DT_BC_DATACDC_NAME,         DT_BC_DATACDC_SHORTNAME,         NULL, -1, -1 },
    { DT_BC_DDRFIFO_NAME,         DT_BC_DDRFIFO_SHORTNAME,         NULL, -1, -1 },
    { DT_BC_DDRFRONT_NAME,        DT_BC_DDRFRONT_SHORTNAME,        NULL, -1, -1 },
    { DT_BC_DISEQC_NAME,          DT_BC_DISEQC_SHORTNAME,          NULL, -1, -1 },
    { DT_BC_EMAC10G_NAME,         DT_BC_EMAC10G_SHORTNAME,         NULL, -1, -1 },
    { DT_BC_ETHIPPAD_NAME,        DT_BC_ETHIPPAD_SHORTNAME,        NULL, -1, -1 },
    { DT_BC_FANC_NAME,            DT_BC_FANC_SHORTNAME,            NULL, -1, -1 },
    { DT_BC_FPGATEMP_NAME,        DT_BC_FPGATEMP_SHORTNAME,        NULL, -1, -1 },
    { DT_BC_GENL_NAME,            DT_BC_GENL_SHORTNAME,            NULL, -1, -1 },
    { DT_BC_GPSTIME_NAME,         DT_BC_GPSTIME_SHORTNAME,         NULL, -1, -1 },
    { DT_BC_GS2988_NAME,          DT_BC_GS2988_SHORTNAME,          NULL, -1, -1 },
    { DT_BC_I2CM_NAME,            DT_BC_I2CM_SHORTNAME,            NULL, -1, -1 },
    { DT_BC_IOSERIN_NAME,         DT_BC_IOSERIN_SHORTNAME,         NULL, -1, -1 },
    { DT_BC_IPFIFO_NAME,          DT_BC_IPFIFO_SHORTNAME,          NULL, -1, -1,},
    { DT_BC_IPSCHED_NAME,         DT_BC_IPSCHED_SHORTNAME,         NULL, -1, -1 },
    { DT_BC_IPSECG_NAME,          DT_BC_IPSECG_SHORTNAME,          NULL, -1, -1 },
    { DT_BC_IPSUMCHK_NAME,        DT_BC_IPSUMCHK_SHORTNAME,        NULL, -1, -1 },
    { DT_BC_IPSUMINS_NAME,        DT_BC_IPSUMINS_SHORTNAME,        NULL, -1, -1 },
    { DT_BC_IPROUT_NAME,          DT_BC_IPROUT_SHORTNAME,          NULL, -1, -1 },
    { DT_BC_IQUNPCK_NAME,         DT_BC_IQUNPCK_SHORTNAME,         NULL, -1, -1 },
    { DT_BC_IQFIR_NAME,           DT_BC_IQFIR_SHORTNAME,           NULL, -1, -1 },
    { DT_BC_IQNOISE_NAME,         DT_BC_IQNOISE_SHORTNAME,         NULL, -1, -1 },
    { DT_BC_IQMISC_NAME,          DT_BC_IQMISC_SHORTNAME,          NULL, -1, -1 },
    { DT_BC_IQSYNC_NAME,          DT_BC_IQSYNC_SHORTNAME,          NULL, -1, -1 },
    { DT_BC_IQUP_NAME,            DT_BC_IQUP_SHORTNAME,            NULL, -1, -1 },
    { DT_BC_IQJESD_NAME,          DT_BC_IQJESD_SHORTNAME,          NULL, -1, -1 },
    { DT_BC_KA_NAME,              DT_BC_KA_SHORTNAME,              NULL, -1, -1 },
    { DT_BC_LEDB_NAME,            DT_BC_LEDB_SHORTNAME,            NULL, -1, -1 },
    { DT_BC_LMH1981_NAME,         DT_BC_LMH1981_SHORTNAME,         NULL, -1, -1 },
    { DT_BC_MSIX_NAME,            DT_BC_MSIX_SHORTNAME,            NULL, -1, -1 },
    { DT_BC_REBOOT_NAME,          DT_BC_REBOOT_SHORTNAME,          NULL, -1, -1 },
    { DT_BC_S12GTO3G_NAME,        DT_BC_S12GTO3G_SHORTNAME,        NULL, -1, -1 },
    { DT_BC_SDIDMX12G_NAME,       DT_BC_SDIDMX12G_SHORTNAME,       NULL, -1, -1 },
    { DT_BC_SDIMUX12G_NAME,       DT_BC_SDIMUX12G_SHORTNAME,       NULL, -1, -1 },
    { DT_BC_SDIXCFG_NAME,         DT_BC_SDIXCFG_SHORTNAME,         NULL, -1, -1 },
    { DT_BC_SDIRXF_NAME,          DT_BC_SDIRXF_SHORTNAME,          NULL, -1, -1 },
    { DT_BC_SDIRXP_NAME,          DT_BC_SDIRXP_SHORTNAME,          NULL, -1, -1 },
    { DT_BC_SDIRXPHY_NAME,        DT_BC_SDIRXPHY_SHORTNAME,        NULL, -1, -1 },
    { DT_BC_SDITXF_NAME,          DT_BC_SDITXF_SHORTNAME,          NULL, -1, -1 },
    { DT_BC_SDITXP_NAME,          DT_BC_SDITXP_SHORTNAME,          NULL, -1, -1 },
    { DT_BC_SDITXPHY_NAME,        DT_BC_SDITXPHY_SHORTNAME,        NULL, -1, -1 },
    { DT_BC_SDITXPLL_NAME,        DT_BC_SDITXPLL_SHORTNAME,        NULL, -1, -1 },
    { DT_BC_SI2166ITF_NAME,       DT_BC_SI2166ITF_SHORTNAME,       NULL, -1, -1 },
    { DT_BC_SPIM_NAME,            DT_BC_SPIM_SHORTNAME,            NULL, -1, -1 },
    { DT_BC_SPIMF_NAME,           DT_BC_SPIMF_SHORTNAME,           NULL, -1, -1 },
    { DT_BC_ST425LR_NAME,         DT_BC_ST425LR_SHORTNAME,         NULL, -1, -1 },
    { DT_BC_SWITCH_NAME,          DT_BC_SWITCH_SHORTNAME,          NULL, -1, -1 },
    { DT_BC_TOD_NAME,             DT_BC_TOD_SHORTNAME,             NULL, -1, -1 },
    { DT_BC_TSRXFMT_NAME,         DT_BC_TSRXFMT_SHORTNAME,         NULL, -1, -1 },
    { DT_BC_VVI_NAME,             DT_BC_VVI_SHORTNAME,             NULL, -1, -1 },
    { DT_BC_XPLL_NAME,            DT_BC_XPLL_SHORTNAME,            NULL, -1, -1 },

    // Local blocks
    { DT_BC_ATTNCTRL_2116_NAME,   DT_BC_ATTNCTRL_2116_SHORTNAME,   NULL, -1, -1 },
    { DT_BC_IO_2116_NAME,         DT_BC_IO_2116_SHORTNAME,         NULL, -1, -1 },
    { DT_BC_AD5320_2132_NAME,     DT_BC_AD5320_2132_SHORTNAME,     NULL, -1, -1 },
    { DT_BC_ADS4246_2132_NAME,    DT_BC_ADS4246_2132_SHORTNAME,    NULL, -1, -1 },
    { DT_BC_DATASTMUX_2132_NAME,  DT_BC_DATASTMUX_2132_SHORTNAME,  NULL, -1, -1 },
    { DT_BC_IQCGRAB_2132_NAME,    DT_BC_IQCGRAB_2132_SHORTNAME,    NULL, -1, -1 },
    { DT_BC_IQFIR_2132_NAME,      DT_BC_IQFIR_2132_SHORTNAME,      NULL, -1, -1 },
    { DT_BC_IQPWR_2132_NAME,      DT_BC_IQPWR_2132_SHORTNAME,      NULL, -1, -1 },
    { DT_BC_IQSRC2132_2132_NAME,  DT_BC_IQSRC2132_2132_SHORTNAME,  NULL, -1, -1 },
    { DT_BC_LNBH25_2132_NAME,     DT_BC_LNBH25_2132_SHORTNAME,     NULL, -1, -1 },
    { DT_BC_S2DEC_2132_NAME,      DT_BC_S2DEC_2132_SHORTNAME,      NULL, -1, -1 },
    { DT_BC_S2DEMOD_2132_NAME,    DT_BC_S2DEMOD_2132_SHORTNAME,    NULL, -1, -1 },
    { DT_BC_S2STATS_2132_NAME,    DT_BC_S2STATS_2132_SHORTNAME,    NULL, -1, -1 },

};
const Int  DT_BC_NUM_KNOWN = DT_SIZEOF_ARRAY(DT_BC_KNOWN);

#endif  // #ifdef DT_BC_ADD_BLOCK_INCLUDES

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtBcInterruptProps*  DtBc_FindInterruptProps(DtBc*, Int  Id);
static DtStatus  DtBc_InitInterruptProps(DtBc*);
static DtStatus  DtBc_Enable(DtBc*, Bool  Enable);
static void DtBc_EvtLog3Txt1Par(DtBc* pBc, const Char* Txt1, const Char* Txt2,
                                                              const Char* Txt3, Int Par1);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBc - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_CheckBlockId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBc_CheckBlockId(DtBc*  pBc)
{
    Int  i=0;
    DtString  Str1, Str2;
    DtStatus Status = DT_STATUS_OK;
    DtStringChar  Str1Buf[DT_BC_ID_MAX_SIZE+1], Str2Buf[DT_BC_ID_MAX_SIZE+1];
    char  BlockId[DT_BC_ID_MAX_SIZE+1];
    BC_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pBc->m_Id.m_pName != NULL);
        
    for(i=0; i<(sizeof(BlockId)-1); i++)
    {
        // Get next char
        BC_BlockId_WRITE_IdIndex(pBc, i);
        BlockId[i] = (char)BC_BlockId_READ_IdChar(pBc);
        if (BlockId[i] == '\0')
            break;      // Reach the end of the block id
    }
    BlockId[i] = '\0';

    DtDbgOutBc(MIN, COMMON, pBc, "BlockId='%s', Name='%s' and Address=0x%0X", BlockId, 
                                                       pBc->m_Id.m_pName, pBc->m_Address);

    DT_STRING_INIT(Str1, Str1Buf, sizeof(Str1Buf)/sizeof(Str1Buf[0]));
    DtStringAppendChars(&Str1, BlockId);
    DT_STRING_INIT(Str2, Str2Buf, sizeof(Str2Buf)/sizeof(Str2Buf[0]));
    DtStringAppendChars(&Str2, pBc->m_Id.m_pName);

    Status = DtStringCompare(&Str1, &Str2) ? DT_STATUS_OK : DT_STATUS_FAIL;
    if (!DT_SUCCESS(Status))
    {
        // Log expected block
        DtBc_EvtLog3Txt1Par(pBc, "BlockId check failed for: ", pBc->m_Id.m_pName,
                                                         "; Address: 0x", pBc->m_Address);
    }

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBc_Close(DtBc*  pBc)
{
    if (pBc == NULL)
        return;
    DT_ASSERT(pBc->m_Size>=sizeof(DtBc));

    DtDbgOutBc(MIN, COMMON, pBc, "Closing BC");

    // Close stub
    if (pBc->m_pIoStub != NULL)
        DtIoStubBc_Close((DtIoStub*)pBc->m_pIoStub);

    // Unregister all interrupt handlers
    DtBc_IntHandlerUnregisterAll(pBc);

    // Free interrupt property data
    if (pBc->m_IntProps != NULL)
        DtMemFreePool(pBc->m_IntProps, BC_TAG);

    // Free BC object
    DtObject_Close((DtObject*)pBc, BC_TAG);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_ExclAccessAcquire -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Acquire exclusive access to the block
//
DtStatus  DtBc_ExclAccessAcquire(DtBc*  pBc, const DtExclAccessObject*  pObject)
{
    DtStatus  Status=DT_STATUS_OK;
    BC_DEFAULT_PRECONDITIONS(pBc);

    // Acquire exclusive access state lock
    if(DtMutexAcquire(&pBc->m_ExclAccessLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;

    // Check if someone else already has access 
    if (!pBc->m_ExclAccess)
    {
        DT_ASSERT(pObject != NULL);
        pBc->m_ExclAccess = TRUE;
        pBc->m_ExclAccessOwner = *pObject;
    }
    else
        Status = DT_STATUS_IN_USE;

    DtMutexRelease(&pBc->m_ExclAccessLock);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_ExclAccessCheck -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Check if we (pObject) have exclusive access
//
DtStatus  DtBc_ExclAccessCheck(DtBc*  pBc, const DtExclAccessObject*  pObject)
{
    DtStatus  Status=DT_STATUS_OK;
    BC_DEFAULT_PRECONDITIONS(pBc);

    // Acquire exclusive access state lock
    if(DtMutexAcquire(&pBc->m_ExclAccessLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;

    // Does someone has exclusive access. If yes check if it is us
    if(pBc->m_ExclAccess)
    {
        if (!DtExclAccessObject_Compare(&pBc->m_ExclAccessOwner, pObject))
            Status = DT_STATUS_IN_USE;  // Someone else has access
    }
    else
        Status = DT_STATUS_EXCL_ACCESS_REQD;  // No one has access yet => must acquire it

    DtMutexRelease(&pBc->m_ExclAccessLock);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_ExclAccessProbe -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Probe if someone has exclusive access
//
DtStatus  DtBc_ExclAccessProbe(DtBc*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;
    BC_DEFAULT_PRECONDITIONS(pBc);

    if (DtMutexAcquire(&pBc->m_ExclAccessLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;

    if (pBc->m_ExclAccess)
        Status = DT_STATUS_IN_USE;

    DtMutexRelease(&pBc->m_ExclAccessLock);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_ExclAccessRelease -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Release the block access lock
//
DtStatus  DtBc_ExclAccessRelease(DtBc*  pBc, const DtExclAccessObject*  pObject)
{
    DtStatus  Status=DT_STATUS_OK;
    BC_DEFAULT_PRECONDITIONS(pBc);

    // Acquire exclusive access state lock
    if(DtMutexAcquire(&pBc->m_ExclAccessLock, 100) != DT_STATUS_OK)
        return DT_STATUS_TIMEOUT;

    // Does someone has exclusive access. If yes check if it is us
    if(pBc->m_ExclAccess)
    {
        if(DtExclAccessObject_Compare(&pBc->m_ExclAccessOwner, pObject))
            pBc->m_ExclAccess = FALSE;
        else
            Status = DT_STATUS_IN_USE;  // Someone else has access
    }

    DtMutexRelease(&pBc->m_ExclAccessLock);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBc_OnCloseFile(DtBc*  pBc, const DtFileObject*  pFile)
{
    DECL_EXCL_ACCESS_OBJECT_FILE(EAO, pFile);

    BC_DEFAULT_PRECONDITIONS(pBc);

    DtDbgOutBc(MAX, COMMON, pBc, "Releasing resources held by File");

    // Release exclusive access lock the File may be holding
    DtBc_ExclAccessRelease(pBc, &EAO);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_SetOpState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBc_SetOpState(DtBc*  pBc, DtBcState  NewState)
{

    DT_UNUSED DtBcState  OldState = pBc->m_OpState;
    pBc->m_OpState = NewState;
    DtDbgOutBc(MIN, COMMON, pBc, "State change: %s->%s", DtBc_OpStateToString(OldState),
                                                          DtBc_OpStateToString(NewState));
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_OpStateToString -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
const char*  DtBc_OpStateToString(DtBcState  State)
{
    switch (State)
    {
    case BC_STATE_ENABLED:          return "ENABLED";
    case BC_STATE_INITIALISED:      return "INITIALISED";
    case BC_STATE_UNDEFINED:        return "UNDEFINED";
    }
    return "???";
}
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_IsEnabled -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtBc_IsEnabled(const DtBc*  pBc)
{
    BC_DEFAULT_PRECONDITIONS(pBc);
    return (pBc->m_OpState == BC_STATE_ENABLED);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_IsInitialised -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtBc_IsInitialised(const DtBc*  pBc)
{
    BC_DEFAULT_PRECONDITIONS(pBc);
    return (pBc->m_OpState == BC_STATE_INITIALISED);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_InterruptDisable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBc_InterruptDisable(DtBc*  pBc, Int  Id)
{
    Int  AddrOffset=0x00;
    DtBcInterruptProps*  pIntProps = NULL;
    BC_DEFAULT_PRECONDITIONS(pBc);

    pIntProps = DtBc_FindInterruptProps(pBc, Id);
    DT_ASSERT(pBc->m_NumInterrupts==0 || pIntProps!=NULL);
    if(pIntProps == NULL)
        return DT_STATUS_FAIL;
    DT_ASSERT(pIntProps->m_Index >= 0);

    // Enable the interrupt
    AddrOffset = pIntProps->m_AddrOffset;
    DT_ASSERT(BC_InterruptStatus_READ_InterruptId(pBc, AddrOffset) == Id);
    BC_InterruptStatus_WRITE_Enable(pBc, AddrOffset, 0);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_InterruptEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBc_InterruptEnable(DtBc*  pBc, Int  Id)
{
    Int  AddrOffset=0x00;
    DtBcInterruptProps*  pIntProps = NULL;
    BC_DEFAULT_PRECONDITIONS(pBc);

    pIntProps = DtBc_FindInterruptProps(pBc, Id);
    DT_ASSERT(pBc->m_NumInterrupts==0 || pIntProps!=NULL);
    if(pIntProps == NULL)
        return DT_STATUS_FAIL;
    DT_ASSERT(pIntProps->m_Index >= 0);

    // Disable the interrupt
    AddrOffset = pIntProps->m_AddrOffset;
    DT_ASSERT(BC_InterruptStatus_READ_InterruptId(pBc, AddrOffset) == Id);
    BC_InterruptStatus_WRITE_Enable(pBc, AddrOffset, 1);
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_InterruptsDisable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Disable all of the blocks interrupts
//
DtStatus  DtBc_InterruptsDisable(DtBc*  pBc)
{
    Int  i=0;
    BC_DEFAULT_PRECONDITIONS(pBc);

    // No interrupts to disable
    if (pBc->m_NumInterrupts == 0)
        return DT_STATUS_OK;

    DT_ASSERT(pBc->m_IntProps != NULL);
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  AddrOffset = pBc->m_IntProps[i].m_AddrOffset;
    #ifdef _DEBUG
        const Int  Id = pBc->m_IntProps[i].m_Id;
        const Int  Index = pBc->m_IntProps[i].m_Index;
        DT_ASSERT(BC_InterruptStatus_READ_InterruptId(pBc, AddrOffset)==Id);
        DT_ASSERT(BC_InterruptStatus_READ_InterruptIndex(pBc, AddrOffset)==Index);
    #endif  // #ifdef _DEBUG
        BC_InterruptStatus_WRITE_Enable(pBc, AddrOffset, 0);
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_InterruptsEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Enable all of the blocks interrupts
//
DtStatus  DtBc_InterruptsEnable(DtBc*  pBc)
{
    Int  i=0;
    BC_DEFAULT_PRECONDITIONS(pBc);

    // No interrupts to enable
    if (pBc->m_NumInterrupts == 0)
        return DT_STATUS_OK;

    DT_ASSERT(pBc->m_IntProps != NULL);
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  AddrOffset = pBc->m_IntProps[i].m_AddrOffset;
    #ifdef _DEBUG
        const Int  Id = pBc->m_IntProps[i].m_Id;
        const Int  Index = pBc->m_IntProps[i].m_Index;
        DT_ASSERT(BC_InterruptStatus_READ_InterruptId(pBc, AddrOffset)==Id);
        DT_ASSERT(BC_InterruptStatus_READ_InterruptIndex(pBc, AddrOffset)==Index);
    #endif  // #ifdef _DEBUG
        BC_InterruptStatus_WRITE_Enable(pBc, AddrOffset, 1);
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_IntHandlerRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBc_IntHandlerRegister(
    DtBc*  pBc,
    Int  Id,
    DtBcIntHandlerFunc  HandlerFunc,
    void*  pContext)
{
    DtBcInterruptProps*  pIntProps=NULL;
    DtBcIntHandlerRegData  RegData;
    DtCore*  pCore=NULL;

    BC_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pBc->m_pCore != NULL);
    pCore = pBc->m_pCore;

    // Find properties for specified interrupt ID
    pIntProps = DtBc_FindInterruptProps(pBc, Id);
    DT_ASSERT(pIntProps!=NULL && pIntProps->m_Index>=0);
    
    // Init registration data
    INIT_DT_BC_INT_HANDLER_REG_DATA(RegData, Id, pIntProps->m_Index, pBc, 
                                                                   HandlerFunc, pContext);
    return DtCore_INTERRUPT_Register(pCore, (DtIntHandlerRegData*)&RegData);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_IntHandlerUnregister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBc_IntHandlerUnregister(DtBc*  pBc, Int  Id)
{
    DtBcInterruptProps*  pIntProps=NULL;
    DtCore*  pCore=NULL;
    
    BC_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pBc->m_pCore != NULL);
    pCore = pBc->m_pCore;

    // Find properties for specified interrupt ID
    pIntProps = DtBc_FindInterruptProps(pBc, Id);
    DT_ASSERT(pIntProps!=NULL && pIntProps->m_Index>=0);
        
    return DtCore_INTERRUPT_Unregister(pCore, pIntProps->m_Index, pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_IntHandlerUnregisterAll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBc_IntHandlerUnregisterAll(DtBc*  pBc)
{
    Int  i=0;
    BC_DEFAULT_PRECONDITIONS(pBc);

    // No interrupts, means no handlers to unregister
    if (pBc->m_NumInterrupts == 0)
        return DT_STATUS_OK;

    DT_ASSERT(pBc->m_IntProps != NULL);
    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        const Int  Id = pBc->m_IntProps[i].m_Id;
        DtStatus  Status = DtBc_IntHandlerUnregister(pBc, Id);
        if (!DT_SUCCESS(Status) && Status!=DT_STATUS_NOT_FOUND)
            DT_ASSERT(FALSE);
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Openes a block and perform base initilisation
//
DtBc*  DtBc_Open(const  DtBcOpenParams*  pParams)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBc*  pBc = NULL;
    Int  PortIndex = -1;

    DT_ASSERT(pParams != NULL);
    DT_ASSERT(pParams->m_Address >= 0x00);
    DT_ASSERT(pParams->m_Size >= sizeof(DtBc));
    DT_ASSERT(pParams->m_pCore!=NULL && pParams->m_pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(pParams->m_CloseFunc != NULL);

    //.-.-.-.-.-.-.-.-.-.-.- Step 1: Allocate memory for BC object -.-.-.-.-.-.-.-.-.-.-.-

    pBc = (DtBc*)DtObject_Open(DT_OBJECT_TYPE_BC, pParams->m_Size, BC_TAG);
    if (pBc == NULL)
        return NULL;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 2: Init BC attributes -.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // Init exclusive access mutex
    DtMutexInit(&pBc->m_ExclAccessLock);
    pBc->m_ExclAccess = FALSE;

    // Set name, short-name, role, type, and address
    DT_ASSERT(pParams->m_Id.m_pName != NULL);
    DT_ASSERT(pParams->m_Id.m_pShortName != NULL);
    DT_ASSERT(pParams->m_Id.m_Instance > 0);
    DT_ASSERT(pParams->m_Id.m_Uuid >= 0);
    pBc->m_Id = pParams->m_Id;
    pBc->m_Type = pParams->m_Type;
    pBc->m_Address = pParams->m_Address;

    // Init instance-ID string
    Status = DtBc_ToInstanceIdFromStrings(pBc->m_Id.m_pShortName, pBc->m_Id.m_Instance,
                                                      pBc->m_InstanceId, 
                                                      DT_SIZEOF_ARRAY(pBc->m_InstanceId));
    DT_ASSERT(DT_SUCCESS(Status));

    // Set shortcut to device data
    pBc->m_pCore = pParams->m_pCore;
    // Set port
    pBc->m_pPt = pParams->m_pPt;
    // Set init function callback
    pBc->m_InitFunc = pParams->m_InitFunc;
    // Set close function callback
    pBc->m_CloseFunc = pParams->m_CloseFunc;

    // Did the derived class define it's own Enable callback?
    if (pParams->m_EnableFunc != NULL)
        pBc->m_EnableFunc = pParams->m_EnableFunc;
    else
        pBc->m_EnableFunc = DtBc_Enable;
    // Set OnEnable callback
    pBc->m_OnEnableFunc = pParams->m_OnEnableFunc;
    
    // Did the derived class define it's onw File-Close callback?
    if (pParams->m_OnCloseFileFunc != NULL)
        pBc->m_OnCloseFileFunc = pParams->m_OnCloseFileFunc;
    else
        pBc->m_OnCloseFileFunc = DtBc_OnCloseFile;      // Use default function

    // Check block ID
    Status = DtBc_CheckBlockId(pBc);
    if(!DT_SUCCESS(Status))
    {
        DT_ASSERT(FALSE);
        DtDbgOutBc(ERR, COMMON, pBc, "ERROR: BlockId does not match block-name '%s'", 
                                                                       pBc->m_Id.m_pName);
        // This is critical error => close BC and retuirn NULL
        DtBc_Close(pBc);
        return NULL;
    }

    // Get block version
    pBc->m_Version = BC_BlockId_READ_Version(pBc);

    // Init interrupt properties
    if(DT_SUCCESS(Status))
    {
        Status = DtBc_InitInterruptProps(pBc);
        DT_ASSERT(DT_SUCCESS(Status));
    }

    // Perform block specific initialisation
    if (pBc->m_InitFunc != NULL)
    {
        Status = pBc->m_InitFunc(pBc);
        if (!DT_SUCCESS(Status))
        {
            DT_ASSERT(pBc->m_CloseFunc != NULL);
            pBc->m_CloseFunc(pBc);
            return NULL;
        }
    }

    // Finally: open an io-stub for the block (if needed)
    if (pParams->m_CreateStub)
    {
        pBc->m_pIoStub = DtIoStubBc_OpenType(pBc);
        DT_ASSERT(pBc->m_pIoStub != NULL);
    }

    // Print block information
    PortIndex = (pBc->m_pPt!=NULL) ? pBc->m_pPt->m_PortIndex : -1;
    DtDbgOutBc(MIN, COMMON, pBc, "version=%d, address=0x%04X, port-index=%d",
                                                                        pBc->m_Version,
                                                                        pBc->m_Address,
                                                                        PortIndex);

    // Set the state to initialised
    Status = DtBc_SetOpState(pBc, BC_STATE_INITIALISED);
    DT_ASSERT(DT_SUCCESS(Status));

    return pBc;
}

#ifdef DT_BC_ADD_BLOCK_INCLUDES
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_OpenType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Open a specific block type
//
DtBc*  DtBc_OpenType(DtBcType  Type, Int  Address, DtCore*  pCore, 
                                 DtPt*  pPt, const char*  pRole, Int  Instance, Int  Uuid,
                                 Bool  CreateStub)
{
    DT_ASSERT(Address >= 0x00);
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    switch(Type)
    {
    case DT_BLOCK_TYPE_ACCUFIFO:
        return (DtBc*)DtBcACCUFIFO_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_ASIRXP:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcASIRXP_Open(Address, pCore, pPt, pRole, Instance, Uuid);
    case DT_BLOCK_TYPE_ASITXG:
        return (DtBc*)DtBcASITXG_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_ASITXSER:
        return (DtBc*)DtBcASITXSER_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_BURSTFIFO:
        return (DtBc*)DtBcBURSTFIFO_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_CDMAC:
        return (DtBc*)DtBcCDMAC_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_CDMACTO:
        return (DtBc*)DtBcCDMACTO_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_CONSTSINK:
        return (DtBc*)DtBcCONSTSINK_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_CONSTSOURCE:
        return (DtBc*)DtBcCONSTSOURCE_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_DATACDC:
        return (DtBc*)DtBcDATACDC_Open(Address, pCore, pPt, pRole, Instance, Uuid, 
                                                                              CreateStub);
    case DT_BLOCK_TYPE_DDRFIFO:
        return (DtBc*)DtBcDDRFIFO_Open(Address, pCore, pPt, pRole, Instance, Uuid,
                                                                              CreateStub);
    case DT_BLOCK_TYPE_DDRFRONT:
        return (DtBc*)DtBcDDRFRONT_Open(Address, pCore, pPt, pRole, Instance, Uuid,
                                                                              CreateStub);
    case DT_BLOCK_TYPE_EMAC10G:
        return (DtBc*)DtBcEMAC10G_Open(Address, pCore, pPt, pRole, Instance, Uuid);
    case DT_BLOCK_TYPE_ETHIPPAD:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcETHIPPAD_Open(Address, pCore, pPt, pRole, Instance, Uuid);
    case DT_BLOCK_TYPE_DISEQC:
        return (DtBc*)DtBcDISEQC_Open(Address, pCore, pPt, pRole, Instance, Uuid,
                                                                              CreateStub);
    case DT_BLOCK_TYPE_FANC:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcFANC_Open(Address, pCore, pPt, pRole, Instance, Uuid);
    case DT_BLOCK_TYPE_FPGATEMP:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcFPGATEMP_Open(Address, pCore, pPt, pRole, Instance, Uuid);
    case DT_BLOCK_TYPE_GENL:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcGENL_Open(Address, pCore, pPt, pRole, Instance, Uuid, FALSE);
    case DT_BLOCK_TYPE_GPSTIME:
        return (DtBc*)DtBcGPSTIME_Open(Address, pCore, pPt, pRole, Instance, Uuid, 
                                                                              CreateStub);
    case DT_BLOCK_TYPE_GS2988:
        return (DtBc*)DtBcGS2988_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_I2CM:
        return (DtBc*)DtBcI2CM_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_IOSERIN:
        return (DtBc*)DtBcIOSERIN_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_IPFIFO:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcIPFIFO_Open(Address, pCore, pPt, pRole, Instance, Uuid);
    case DT_BLOCK_TYPE_IPROUT:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcIPROUT_Open(Address, pCore, pPt, pRole, Instance, Uuid);
    case DT_BLOCK_TYPE_IPSCHED:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcIPSCHED_Open(Address, pCore, pPt, pRole, Instance, Uuid);
    case DT_BLOCK_TYPE_IPSECG:
        return (DtBc*)DtBcIPSECG_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_IPSUMCHK:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcIPSUMCHK_Open(Address, pCore, pPt, pRole, Instance, Uuid);
    case DT_BLOCK_TYPE_IPSUMINS:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcIPSUMINS_Open(Address, pCore, pPt, pRole, Instance, Uuid);
    case DT_BLOCK_TYPE_IQUNPCK:
        return (DtBc*)DtBcIQUNPCK_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_IQFIR:
        return (DtBc*)DtBcIQFIR_Open(Address, pCore, pPt, pRole, Instance,
                                                                       Uuid, CreateStub);
    case DT_BLOCK_TYPE_IQNOISE:
        return (DtBc*)DtBcIQNOISE_Open(Address, pCore, pPt, pRole, Instance,
                                                                       Uuid, CreateStub);
    case DT_BLOCK_TYPE_IQMISC:
        return (DtBc*)DtBcIQMISC_Open(Address, pCore, pPt, pRole, Instance,
                                                                       Uuid, CreateStub);
    case DT_BLOCK_TYPE_IQSYNC:
        return (DtBc*)DtBcIQSYNC_Open(Address, pCore, pPt, pRole, Instance,
                                                                       Uuid, CreateStub);   
    case DT_BLOCK_TYPE_IQUP:
        return (DtBc*)DtBcIQUP_Open(Address, pCore, pPt, pRole, Instance,
                                                                       Uuid, CreateStub); 
    case DT_BLOCK_TYPE_IQJESD:
        return (DtBc*)DtBcIQJESD_Open(Address, pCore, pPt, pRole, Instance,
                                                                       Uuid, CreateStub);

    case DT_BLOCK_TYPE_KA:
        return (DtBc*)DtBcKA_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_LEDB:
        return (DtBc*)DtBcLEDB_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_LMH1981:
        return (DtBc*)DtBcLMH1981_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_MSIX:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcMSIX_Open(Address, pCore, pPt, pRole, Instance, Uuid);
    case DT_BLOCK_TYPE_REBOOT:
        return (DtBc*)DtBcREBOOT_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_S12GTO3G:
        return (DtBc*)DtBcS12GTO3G_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_SPIM:
        return (DtBc*)DtBcSPIM_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_SPIMF:
        return (DtBc*)DtBcSPIMF_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_SDIDMX12G:
        return (DtBc*)DtBcSDIDMX12G_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_SDIMUX12G:
        return (DtBc*)DtBcSDIMUX12G_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_SDIXCFG:
        return (DtBc*)DtBcSDIXCFG_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_SDIRXF:
        return (DtBc*)DtBcSDIRXF_Open(Address, pCore, pPt, Type, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_SDIRXP:
        return (DtBc*)DtBcSDIRXP_Open(Address, pCore, pPt, Type, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_SDIRXPHY:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcSDIRXPHY_Open(Address, pCore, pPt, pRole, Instance, Uuid);
    case DT_BLOCK_TYPE_SDITXF:
        return (DtBc*)DtBcSDITXF_Open(Address, pCore, pPt, Type, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_SDITXP:
        return (DtBc*)DtBcSDITXP_Open(Address, pCore, pPt, Type, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_SDITXPHY:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcSDITXPHY_Open(Address, pCore, pPt, pRole, Instance, Uuid);

    case DT_BLOCK_TYPE_SDITXPLL:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcSDITXPLL_Open(Address, pCore, pPt, pRole, Instance, Uuid);

    case DT_BLOCK_TYPE_SI2166ITF:
        return (DtBc*)DtBcSI2166ITF_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);

    case DT_BLOCK_TYPE_ST425LR:
        return (DtBc*)DtBcST425LR_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_SWITCH:
        return (DtBc*)DtBcSWITCH_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_TOD:
        return (DtBc*)DtBcTOD_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_TSRXFMT:
        return (DtBc*)DtBcTSRXFMT_Open(Address, pCore, pPt, pRole, Instance,
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_VVI:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcVVI_Open(Address, pCore, pPt, pRole, Instance, Uuid);

        // Local blocks
    case DT_BLOCK_TYPE_ATTNCTRL_2116:
        return (DtBc*)DtBcATTNCTRL_2116_Open(Address, pCore, pPt, pRole, Instance,
                                                                       Uuid, CreateStub);
    case DT_BLOCK_TYPE_IO_2116:
        return (DtBc*)DtBcIO_2116_Open(Address, pCore, pPt, pRole, Instance,
                                                                       Uuid, CreateStub);
    case DT_BLOCK_TYPE_AD5320_2132:
        return (DtBc*)DtBcAD5320_2132_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_ADS4246_2132:
        return (DtBc*)DtBcADS4246_2132_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_DATASTMUX_2132:
        return (DtBc*)DtBcDATASTMUX_2132_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_IQCGRAB_2132:
        return (DtBc*)DtBcIQCGRAB_2132_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_IQFIR_2132:
        return (DtBc*)DtBcIQFIR_2132_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_IQPWR_2132:
        return (DtBc*)DtBcIQPWR_2132_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_IQSRC2132_2132:
        return (DtBc*)DtBcIQSRC2132_2132_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_LNBH25_2132:
        return (DtBc*)DtBcLNBH25_2132_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_S2DEC_2132:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcS2DEC_2132_Open(Address, pCore, pPt, pRole, Instance, Uuid);
    case DT_BLOCK_TYPE_S2DEMOD_2132:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcS2DEMOD_2132_Open(Address, pCore, pPt, pRole, Instance, Uuid);
    case DT_BLOCK_TYPE_S2STATS_2132:
        return (DtBc*)DtBcS2STATS_2132_Open(Address, pCore, pPt, pRole, Instance, 
                                                                        Uuid, CreateStub);
    case DT_BLOCK_TYPE_XPLL:
        DT_ASSERT(CreateStub == FALSE);
        return (DtBc*)DtBcXPLL_Open(Address, pCore, pPt, pRole, Instance, Uuid);
    default:
        DT_ASSERT(FALSE);
        break;
    }
    return NULL;
}
#endif  // #ifdef DT_BC_ADD_BLOCK_INCLUDES

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_ToInstanceId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBc_ToInstanceId(const DtBc*  pBc, char*  pInstanceId, Int  MaxLength)
{
    BC_DEFAULT_PRECONDITIONS(pBc);

    // Let the string version do the heavy lifting
    return DtBc_ToInstanceIdFromStrings(pBc->m_Id.m_pShortName, pBc->m_Id.m_Instance, 
                                                                  pInstanceId, MaxLength);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_ToInstanceIdFromStrings -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBc_ToInstanceIdFromStrings(const char*  pShortName, Int  Instance,
                                                       char*  pInstanceId, Int  MaxLength)
{
    DtString  TheString;
    DtStatus  Status = DT_STATUS_OK;

    DT_ASSERT(pInstanceId!=NULL && MaxLength>0);
       
    // Allocate string buffer
    Status = DtStringAlloc(&TheString, MaxLength);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, BC_COMMON, "ERROR: failed to allocate string-object");
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Construct Instance ID -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // Format: BC_<short_name>#<instance> (e.g. BC_SPIMF#1)
    
#define  _CHECK_RESULT(Func)                    \
    do {                                        \
        Status = Func;                          \
        if(!DT_SUCCESS(Status))                 \
        {                                       \
            /* Free string buffer */            \
            DtStringFree(&TheString);           \
            return Status;                      \
        }                                       \
    } while(0)

    // Part 1: add fixed "BC_" prefix
    _CHECK_RESULT(DtStringAppendChars(&TheString, "BC_"));

    // Part 2: add the short-name. NOTE: short-name is expected to be all capitals
    DT_ASSERT(pShortName!=NULL && pShortName[0]!='\0');
    _CHECK_RESULT(DtStringAppendChars(&TheString, pShortName));

    // Part 3: add instance number
    DT_ASSERT(Instance > 0);
    _CHECK_RESULT(DtStringAppendChars(&TheString, "#"));
    _CHECK_RESULT(DtStringUIntegerToDtStringAppend(&TheString, 10, Instance));
    
    // convert to a 'normal' character string
    _CHECK_RESULT(DtStringToCharString(&TheString, pInstanceId, MaxLength));

    // Free string buffer
    DtStringFree(&TheString);
#undef  _CHECK_RESULT

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_ToPropertyName -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBc_ToPropertyName(const DtBc*  pBc, const char*  pProp, 
                                                     char*  pFullPropName, Int  MaxLength)
{
    BC_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pFullPropName!=NULL && MaxLength>0);
    
    // Let string version do the heavy lifting
    return DtBc_ToPropertyNameFromStrings(pBc->m_InstanceId, pProp, pFullPropName, 
                                                                               MaxLength);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_ToPropertyNameFromStrings -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBc_ToPropertyNameFromStrings(
    const char*  pInstanceId,
    const char*  pProp,
    char*  pFullPropName,
    Int  MaxLength)
{
    DtString  TheString;
    DtStatus  Status = DT_STATUS_OK;
    
    DT_ASSERT(pInstanceId!=NULL && pInstanceId[0]!='\0');
    DT_ASSERT(pFullPropName!=NULL && MaxLength>0);

    // Allocate string buffer
    Status = DtStringAlloc(&TheString, MaxLength);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOut(ERR, BC_COMMON, "ERROR: failed to allocate string-object");
        return Status;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Construct full property name -.-.-.-.-.-.-.-.-.-.-.-.-.-
    // Format: <bc_instance_id>[_<property>] (e.g. BC_SPIMF#1_ADDRESS)
    
#define  _CHECK_RESULT(Func)                    \
    do {                                        \
        Status = Func;                          \
        if(!DT_SUCCESS(Status))                 \
        {                                       \
            /* Free string buffer */            \
            DtStringFree(&TheString);           \
            return Status;                      \
        }                                       \
    } while(0)

    // Part 1: Append instance ID
    _CHECK_RESULT(DtStringAppendChars(&TheString, pInstanceId));

    // Part 2: add the optional property itself
    // NOTE1: property already is an upper case string
    // NOTE2: can be an empty string
    if (pProp!=NULL && pProp[0] != '\0')
    {
        _CHECK_RESULT(DtStringAppendChars(&TheString, "_"));
        _CHECK_RESULT(DtStringAppendChars(&TheString, pProp));
    }
    
    // convert to a 'normal' character string
    _CHECK_RESULT(DtStringToCharString(&TheString, pFullPropName, MaxLength));

    // Free string buffer
    DtStringFree(&TheString);
#undef  _CHECK_RESULT

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_RegRead32 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
UInt32  DtBc_RegRead32(DtBc*  pBc, Int  Offset)
{
    BC_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pBc->m_pCore!=NULL && pBc->m_pCore->m_RegRead32Func!=NULL);
    DT_ASSERT(pBc->m_Address>=0 && Offset>=0);
    // Call registered register read function
    return pBc->m_pCore->m_RegRead32Func(pBc->m_pCore, (pBc->m_Address+Offset));
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_RegWrite32 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBc_RegWrite32(DtBc*  pBc, Int  Offset, UInt32  Value)
{
    BC_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(pBc->m_pCore!=NULL && pBc->m_pCore->m_RegWrite32Func!=NULL);
    DT_ASSERT(pBc->m_Address>=0 && Offset>=0);
    // Call registered register write function
    pBc->m_pCore->m_RegWrite32Func(pBc->m_pCore, (pBc->m_Address+Offset), Value);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBc - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_Enable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBc_Enable(DtBc*  pBc, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;

    BC_DEFAULT_PRECONDITIONS(pBc);

    if ((pBc->m_OpState==BC_STATE_ENABLED && Enable) 
                                     || (pBc->m_OpState==BC_STATE_INITIALISED && !Enable))
        return DT_STATUS_OK;

    // Does the BC have a OnEnable callback
    if (pBc->m_OnEnableFunc != NULL)
    {
        Status = pBc->m_OnEnableFunc(pBc, Enable);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, COMMON, pBc, "ERROR: OnEnable failed");
            return Status;
        }
    }

    // Set new state
    Status = DtBc_SetOpState(pBc, Enable ? BC_STATE_ENABLED : BC_STATE_INITIALISED);
    return Status;

}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_FindInterruptProps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Finds the interrupt properties for the specified interrupt ID
//
DtBcInterruptProps*  DtBc_FindInterruptProps(DtBc* pBc, Int  Id)
{
    Int  i=0;
    BC_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(Id >= 0);

    if(pBc->m_NumInterrupts == 0)
    {
        DtDbgOutBc(ERR, COMMON, pBc, "ERROR: BC has no interrupts");
        return NULL;
    }
    DT_ASSERT(pBc->m_IntProps != NULL);

    for(i=0; i<pBc->m_NumInterrupts; i++)
    {
        if (pBc->m_IntProps[i].m_Id != Id)
            continue;
        return &pBc->m_IntProps[i];
    }
    DtDbgOutBc(ERR, COMMON, pBc, "ERROR: BC has no interrupt with ID=%d", Id);
    return NULL;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_InitInterruptProps -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBc_InitInterruptProps(DtBc*  pBc)
{
    Int  i=0, Size=0, NumInterrupts=0;
    BC_DEFAULT_PRECONDITIONS(pBc);

    // Read number of interrupts from block-id register
    NumInterrupts = BC_BlockId_READ_NumInterrupts(pBc);
    DT_ASSERT(NumInterrupts>=0 && NumInterrupts<=2048);

    // Free existing properties, we will re-init the properties
    if(pBc->m_IntProps != NULL)
    {
        DtMemFreePool(pBc->m_IntProps, BC_TAG);
        pBc->m_IntProps = NULL;
    }
    pBc->m_NumInterrupts = 0;
    if (NumInterrupts == 0)
        return DT_STATUS_OK;
    
    // Allocate memory for interrupt properties
    Size = sizeof(DtBcInterruptProps)*NumInterrupts;
    pBc->m_IntProps = DtMemAllocPool(DtPoolNonPaged, Size, BC_TAG);
    if(pBc->m_IntProps == NULL)
    {
        DtDbgOutBc(ERR, COMMON, pBc, "ERROR: failed to allocate memory for int-props");
        return DT_STATUS_OUT_OF_MEMORY;
    }
    DtMemZero(pBc->m_IntProps, Size);

    // Init the properties
    for(i=0; i<NumInterrupts; i++)
    {
        // Offset to block's next interrupt status register. NOTE: the first interrupt 
        // status register is always located at offset 0x04
        UInt32  AddrOffset = 0x04 + (i*4);
        // Cache the interrupt-status register
        UInt32  Reg_IntStatus = BC_InterruptStatus_READ(pBc, AddrOffset);
        
        pBc->m_IntProps[i].m_AddrOffset = AddrOffset;
        pBc->m_IntProps[i].m_Id = BC_InterruptStatus_GET_InterruptId(Reg_IntStatus);
        pBc->m_IntProps[i].m_Index = BC_InterruptStatus_GET_InterruptIndex(Reg_IntStatus);
    }
    pBc->m_NumInterrupts = NumInterrupts;

    // Print information about interrupts
    for(i=0; i<NumInterrupts; i++)
    {
        DT_ASSERT(pBc->m_Id.m_pName != NULL);
        DtDbgOutBc(AVG, COMMON, pBc, "Interrupt%d: id=%d, index=%d, offset=0x%04X",
                                                         i,
                                                         pBc->m_IntProps[i].m_Id,
                                                         pBc->m_IntProps[i].m_Index,
                                                         pBc->m_IntProps[i].m_AddrOffset);
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBc_EvtLog3Txt1Par -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBc_EvtLog3Txt1Par(DtBc* pBc, const Char* Txt1, const Char* Txt2, const Char* Txt3,
                                                                                 Int Par1)
{
#ifdef WINBUILD
    DtString  Str;
    DtStringAlloc(&Str, 256);
    DtStringAppendChars(&Str, "[SN=");
    DtStringUInt64ToDtStringAppend(&Str, 10, pBc->m_pCore->m_pDevInfo->m_Serial);
    DtStringAppendChars(&Str, "]  Port:");
    DtStringUInt64ToDtStringAppend(&Str, 10, DtCore_PT_GetPortIndex(pBc->m_pPt)+1);
    DtStringAppendChars(&Str, " BC ");
    DtStringAppendChars(&Str, Txt1);
    DtStringAppendChars(&Str, Txt2);
    DtStringAppendChars(&Str, Txt3);
    DtStringUInt64ToDtStringAppend(&Str, 16, Par1);
    DtEvtLogReport(&pBc->m_pCore->m_Device.m_EvtObject, DTPCIE_LOG_INFO_GENERIC, &Str,
                                                                              NULL, NULL);
    DtStringFree(&Str);
#else
    printk("DtPcie: [SN=%lld] Port:%d BC %s %s %s %x",
                                                     pBc->m_pCore->m_pDevInfo->m_Serial,
                                                     DtCore_PT_GetPortIndex(pBc->m_pPt)+1,
                                                     Txt1, Txt2, Txt3, Par1);
#endif
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBc implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO to cast a io-stub-pointer (pStub) to a DtIoStubBc-pointer
#define IOSTUB_BC    ((DtIoStubBc*)pStub)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBc_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBc_Close(DtIoStub*  pStub)
{
    if (pStub == NULL)
        return;
    DT_ASSERT(pStub->m_Size>=sizeof(DtIoStubBc));

    // Let the base implementation do the heavy lifting 
    DtIoStub_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBc_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBc*  DtIoStubBc_Open(DtIoStubBcOpenParams*  pParams)
{
    DtStatus  Status = DT_STATUS_OK;
    DtIoStubBc*  pStub = NULL;
    const DtBcId* pId = NULL;
    char  Name[DT_IOSTUB_NAME_MAX_SIZE];
    DtStringChar  TempStrBuf[DT_IOSTUB_NAME_MAX_SIZE];
    DtString  TempStr;

    DT_ASSERT(pParams->m_Size >= sizeof(DtIoStubBc));
    DT_ASSERT(pParams->m_pBc!=NULL && pParams->m_pBc->m_Size>=sizeof(DtBc));
    DT_ASSERT(pParams->m_CloseFunc != NULL);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 1: Get name -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    // Use the instance ID + UUID index as the friendly-name of the stub ('BC_VVI#1:1')
    pId = &pParams->m_pBc->m_Id;

    DT_STRING_INIT(TempStr, TempStrBuf, DT_SIZEOF_ARRAY(TempStrBuf));
    
    Status = DtStringAppendChars(&TempStr, pParams->m_pBc->m_InstanceId);
    DT_ASSERT(DT_SUCCESS(Status));
    Status = DtStringAppendChars(&TempStr, ":");
    DT_ASSERT(DT_SUCCESS(Status));
    Status = DtStringUIntegerToDtStringAppend(&TempStr, 10, 
                                                          pId->m_Uuid&DT_UUID_INDEX_MASK);
    DT_ASSERT(DT_SUCCESS(Status));
    Status = DtStringToCharString(&TempStr, Name, DT_SIZEOF_ARRAY(Name));
    DT_ASSERT(DT_SUCCESS(Status));

    //-.-.-.-.-.-.-.-.- Step 2: Allocate memory for ioctl stub structure -.-.-.-.-.-.-.-.-

    // Did the derived struct set its own name
    if (pParams->m_pName == NULL)
        pParams->m_pName = Name;
    
    // Let the base implementation do the heavy lifting
    pStub = (DtIoStubBc*)DtIoStub_Open((DtIoStubOpenParams*)pParams);
    if (pStub == NULL)
        return NULL;
    
    //-.-.-.-.-.-.-.-.-.-.-.-.- Step 3: Init BC stub attributes -.-.-.-.-.-.-.-.-.-.-.-.-.
    // Base attributes are already initialised, so only need to init BC specfic ones

    // Set the block we control
    pStub->m_pBc = pParams->m_pBc;
    
    // Print stub information
    DT_ASSERT(pParams->m_pBc->m_Id.m_pName != NULL);
    DtDbgOutIoStubBc(MIN, COMMON, pStub, "Created io-control stub for '%s:0x%04X' block",
                                                             pParams->m_pBc->m_Id.m_pName,
                                                             pParams->m_pBc->m_Address);
    return pStub;
}

#ifdef DT_BC_ADD_BLOCK_INCLUDES

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBc_OpenType -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBc*  DtIoStubBc_OpenType(DtBc*  pBc)
{
    BC_DEFAULT_PRECONDITIONS(pBc);

    switch(pBc->m_Type)
    {
    case DT_BLOCK_TYPE_ACCUFIFO:
        return (DtIoStubBc*)DtIoStubBcACCUFIFO_Open(pBc);
    case DT_BLOCK_TYPE_ASIRXP:
        break;
    case DT_BLOCK_TYPE_ASITXG:
        return (DtIoStubBc*)DtIoStubBcASITXG_Open(pBc);
    case DT_BLOCK_TYPE_ASITXSER:
        return (DtIoStubBc*)DtIoStubBcASITXSER_Open(pBc);
    case DT_BLOCK_TYPE_BURSTFIFO:
        return (DtIoStubBc*)DtIoStubBcBURSTFIFO_Open(pBc);
    case DT_BLOCK_TYPE_CDMAC:
        return (DtIoStubBc*)DtIoStubBcCDMAC_Open(pBc);
    case DT_BLOCK_TYPE_CDMACTO:
        return (DtIoStubBc*)DtIoStubBcCDMACTO_Open(pBc);
    case DT_BLOCK_TYPE_CONSTSINK:
        return (DtIoStubBc*)DtIoStubBcCONSTSINK_Open(pBc);
    case DT_BLOCK_TYPE_CONSTSOURCE:
        return (DtIoStubBc*)DtIoStubBcCONSTSOURCE_Open(pBc);
    case DT_BLOCK_TYPE_DATACDC:
        return (DtIoStubBc*)DtIoStubBcDATACDC_Open(pBc);
    case DT_BLOCK_TYPE_DDRFIFO:
        return (DtIoStubBc*)DtIoStubBcDDRFIFO_Open(pBc);
    case DT_BLOCK_TYPE_DDRFRONT:
        return (DtIoStubBc*)DtIoStubBcDDRFRONT_Open(pBc);
    case DT_BLOCK_TYPE_DISEQC:
        return (DtIoStubBc*)DtIoStubBcDISEQC_Open(pBc);
    case DT_BLOCK_TYPE_GPSTIME:
        return (DtIoStubBc*)DtIoStubBcGPSTIME_Open(pBc);
    case DT_BLOCK_TYPE_I2CM:
        return (DtIoStubBc*)DtIoStubBcI2CM_Open(pBc);
    case DT_BLOCK_TYPE_IOSERIN:
        return (DtIoStubBc*)DtIoStubBcIOSERIN_Open(pBc);
    case DT_BLOCK_TYPE_IPSECG:
        return (DtIoStubBc*)DtIoStubBcIPSECG_Open(pBc);
    case DT_BLOCK_TYPE_IQUNPCK:
        return (DtIoStubBc*)DtIoStubBcIQUNPCK_Open(pBc);
    case DT_BLOCK_TYPE_IQFIR:
        return (DtIoStubBc*)DtIoStubBcIQFIR_Open(pBc);
    case DT_BLOCK_TYPE_IQNOISE:
        return (DtIoStubBc*)DtIoStubBcIQNOISE_Open(pBc);
    case DT_BLOCK_TYPE_IQMISC:
        return (DtIoStubBc*)DtIoStubBcIQMISC_Open(pBc);
    case DT_BLOCK_TYPE_IQSYNC:
        return (DtIoStubBc*)DtIoStubBcIQSYNC_Open(pBc);
    case DT_BLOCK_TYPE_IQUP:
        return (DtIoStubBc*)DtIoStubBcIQUP_Open(pBc);
    case DT_BLOCK_TYPE_IQJESD:
        return (DtIoStubBc*)DtIoStubBcIQJESD_Open(pBc);
    case DT_BLOCK_TYPE_KA:
        return (DtIoStubBc*)DtIoStubBcKA_Open(pBc);
    case DT_BLOCK_TYPE_LEDB:
        return (DtIoStubBc*)DtIoStubBcLEDB_Open(pBc);
    case DT_BLOCK_TYPE_LMH1981:
        return (DtIoStubBc*)DtIoStubBcLMH1981_Open(pBc);
    case DT_BLOCK_TYPE_MSIX:
        break;
    case DT_BLOCK_TYPE_REBOOT:
        return (DtIoStubBc*)DtIoStubBcREBOOT_Open(pBc);
    case DT_BLOCK_TYPE_S12GTO3G:
        return (DtIoStubBc*)DtIoStubBcS12GTO3G_Open(pBc);
    case DT_BLOCK_TYPE_SDIDMX12G:
        return (DtIoStubBc*)DtIoStubBcSDIDMX12G_Open(pBc);
    case DT_BLOCK_TYPE_SDIMUX12G:
        return (DtIoStubBc*)DtIoStubBcSDIMUX12G_Open(pBc);
    case DT_BLOCK_TYPE_SDIXCFG:
        return (DtIoStubBc*)DtIoStubBcSDIXCFG_Open(pBc);
    case DT_BLOCK_TYPE_SDIRXF:
        return (DtIoStubBc*)DtIoStubBcSDIRXF_Open(pBc);
    case DT_BLOCK_TYPE_SDIRXP:
        return (DtIoStubBc*)DtIoStubBcSDIRXP_Open(pBc);
    case DT_BLOCK_TYPE_SDIRXPHY:
        break;
    case DT_BLOCK_TYPE_SDITXF:
        return (DtIoStubBc*)DtIoStubBcSDITXF_Open(pBc);
    case DT_BLOCK_TYPE_SDITXP:
        return (DtIoStubBc*)DtIoStubBcSDITXP_Open(pBc);
    case DT_BLOCK_TYPE_SDITXPHY:
        break;
    case DT_BLOCK_TYPE_SDITXPLL:
        break;
    case DT_BLOCK_TYPE_SI2166ITF:
        return (DtIoStubBc*)DtIoStubBcSI2166ITF_Open(pBc);
    case DT_BLOCK_TYPE_SPIM:
        return (DtIoStubBc*)DtIoStubBcSPIM_Open(pBc);
    case DT_BLOCK_TYPE_SPIMF:
        return (DtIoStubBc*)DtIoStubBcSPIMF_Open(pBc);
    case DT_BLOCK_TYPE_ST425LR:
        return (DtIoStubBc*)DtIoStubBcST425LR_Open(pBc);
    case DT_BLOCK_TYPE_SWITCH:
        return (DtIoStubBc*)DtIoStubBcSWITCH_Open(pBc);
    case DT_BLOCK_TYPE_TOD:
        return (DtIoStubBc*)DtIoStubBcTOD_Open(pBc);
    case DT_BLOCK_TYPE_TSRXFMT:
        return (DtIoStubBc*)DtIoStubBcTSRXFMT_Open(pBc);
    case DT_BLOCK_TYPE_VVI:
        break;

        // Local blocks
    case DT_BLOCK_TYPE_ATTNCTRL_2116:
        return (DtIoStubBc*)DtIoStubBcATTNCTRL_2116_Open(pBc);
    case DT_BLOCK_TYPE_IO_2116:
        return (DtIoStubBc*)DtIoStubBcIO_2116_Open(pBc);
    case DT_BLOCK_TYPE_AD5320_2132:
        return (DtIoStubBc*)DtIoStubBcAD5320_2132_Open(pBc);
    case DT_BLOCK_TYPE_ADS4246_2132:
        return (DtIoStubBc*)DtIoStubBcADS4246_2132_Open(pBc);
    case DT_BLOCK_TYPE_DATASTMUX_2132:
        return (DtIoStubBc*)DtIoStubBcDATASTMUX_2132_Open(pBc);
    case DT_BLOCK_TYPE_IQCGRAB_2132:
        return (DtIoStubBc*)DtIoStubBcIQCGRAB_2132_Open(pBc);
    case DT_BLOCK_TYPE_IQFIR_2132:
        return (DtIoStubBc*)DtIoStubBcIQFIR_2132_Open(pBc);
    case DT_BLOCK_TYPE_IQPWR_2132:
        return (DtIoStubBc*)DtIoStubBcIQPWR_2132_Open(pBc);
    case DT_BLOCK_TYPE_IQSRC2132_2132:
        return (DtIoStubBc*)DtIoStubBcIQSRC2132_2132_Open(pBc);
    case DT_BLOCK_TYPE_LNBH25_2132:
        return (DtIoStubBc*)DtIoStubBcLNBH25_2132_Open(pBc);
    case DT_BLOCK_TYPE_S2DEC_2132:
        break;
    case DT_BLOCK_TYPE_S2DEMOD_2132:
        break;
    case DT_BLOCK_TYPE_S2STATS_2132:
        return (DtIoStubBc*)DtIoStubBcS2STATS_2132_Open(pBc);

    default:
        DT_ASSERT(FALSE);
        break;
    }
    return NULL;
}

#endif  // #ifdef DT_BC_ADD_BLOCK_INCLUDES

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBc_OnExclAccessCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBc_OnExclAccessCmd(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams,
    Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtExclAccessObject*  pExclAccessObj = NULL;
    const DtIoctlExclAccessCmdInput*  pInData = NULL;
        
    DT_ASSERT(pStub!=NULL && pStub->m_Size>=sizeof(DtIoStubBc));
    DT_ASSERT(IOSTUB_BC->m_pBc!=NULL);
    DT_ASSERT((IOSTUB_BC->m_pBc->m_pPt==NULL && pIoParams->m_PortIndex==-1)
              || (IOSTUB_BC->m_pBc->m_pPt!=NULL 
                      && IOSTUB_BC->m_pBc->m_pPt->m_PortIndex == pIoParams->m_PortIndex));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_EXCL_ACCESS_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_ExclAccessCmd;
    
    // Get exclusive access object
    pExclAccessObj = &pIoParams->m_ExclAccessObj;
    
    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_EXCLUSIVE_ACCESS_CMD_ACQUIRE:
        Status = DtBc_ExclAccessAcquire(IOSTUB_BC->m_pBc, pExclAccessObj);
        break;
    case DT_EXCLUSIVE_ACCESS_CMD_CHECK:
        Status = DtBc_ExclAccessCheck(IOSTUB_BC->m_pBc, pExclAccessObj);
        break;
    case DT_EXCLUSIVE_ACCESS_CMD_PROBE:
        Status = DtBc_ExclAccessProbe(IOSTUB_BC->m_pBc);
        break;
    case DT_EXCLUSIVE_ACCESS_CMD_RELEASE:
        Status = DtBc_ExclAccessRelease(IOSTUB_BC->m_pBc, pExclAccessObj);
        break;
    default:
        DT_ASSERT(FALSE);
        Status = DT_STATUS_NOT_SUPPORTED;
        break;
    }
    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcCommonProps implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCommonProps_Load -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcCommonProps_Load(
    DtBcCommonProps*  pBcProps, 
    const DtBcId*  pId,
    Int  PortIndex,
    DtCore*  pCore)
{
    DtStatus  Status = DT_STATUS_OK;
    char  InstanceId[PROPERTY_NAME_MAX_SIZE];
    
    DT_ASSERT(pBcProps!=NULL && pCore!=NULL && pId!=NULL);

    // Generate an instance ID (e.g. 'DF_VPD#1')
    Status = DtBc_ToInstanceIdFromStrings(pId->m_pShortName, pId->m_Instance, 
                                                 InstanceId, DT_SIZEOF_ARRAY(InstanceId));
    if (!DT_SUCCESS(Status))
        return Status;

    // Let string version do the heavy lifting
    return DtBcCommonProps_LoadFromString(pBcProps, InstanceId, PortIndex, pCore);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcCommonProps_LoadFromString -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcCommonProps_LoadFromString(
    DtBcCommonProps*  pBcProps, 
    const char*  pInstanceId,
    Int  PortIndex,
    DtCore*  pCore)
{
    DtStatus  Status = DT_STATUS_OK;
    char  PropName[PROPERTY_NAME_MAX_SIZE+1];
    const  DtProperty*  pProperty=NULL;
    DtPropertyFilterCriteria  Flt;

    static const DtBcType  DEF_TYPE = DT_BLOCK_TYPE_UNKNOWN;
    static const Int  DEF_ADDRESS = -1;
    static const Int  DEF_UUID = -1;

    DT_ASSERT(pInstanceId!=NULL && pInstanceId[0]!='\0');
    DT_ASSERT(pBcProps!=NULL && pCore!=NULL);

    // Init to 'safe' initial values
    pBcProps->m_Address = DEF_ADDRESS;
    pBcProps->m_pRole = NULL;
    pBcProps->m_Type = DEF_TYPE;
    pBcProps->m_Uuid = DEF_UUID;
    pBcProps->m_CreateStub = FALSE;

    DT_PROPERTY_FILTER_CRITERIA_INIT(Flt, PortIndex);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Get block-role property -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    Status = DtBc_ToPropertyNameFromStrings(pInstanceId, NULL, PropName,
                                                               DT_SIZEOF_ARRAY(PropName));
    DT_ASSERT(DT_SUCCESS(Status));

    // Find the property for the role of the block
    pProperty = NULL;
    Status = DtCore_PROPS_Find(pCore, PropName, Flt, &pProperty);
    if (!DT_SUCCESS(Status))
        return Status;

    DT_ASSERT(pProperty->m_Type == PROPERTY_VALUE_TYPE_STRING);
    pBcProps->m_pRole = pProperty->m_pStr;
    // If the property has a DTAPI scope a stub must be created
    pBcProps->m_CreateStub = (pProperty->m_Scope&PROPERTY_SCOPE_DTAPI)!=0;

    //.-.-.-.-.-.-.-.-.-.-.-.-.- Get the block-address property -.-.-.-.-.-.-.-.-.-.-.-.-.

    Status = DtBc_ToPropertyNameFromStrings(pInstanceId, "ADDRESS", PropName, 
                                                               DT_SIZEOF_ARRAY(PropName));
    DT_ASSERT(DT_SUCCESS(Status));

    pBcProps->m_Address = DtCore_PROPS_GetInt(pCore, PropName, PortIndex, DEF_ADDRESS);
    if (pBcProps->m_Address == DEF_ADDRESS)
    {
        DtDbgOut(ERR, BC_COMMON, "Failed to get block-address property '%s'", PropName);
        return DT_STATUS_FAIL;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Get block-type property -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    Status = DtBc_ToPropertyNameFromStrings(pInstanceId, "TYPE", PropName, 
                                                               DT_SIZEOF_ARRAY(PropName));
    DT_ASSERT(DT_SUCCESS(Status));

    pBcProps->m_Type = (DtBcType)DtCore_PROPS_GetInt(pCore, PropName, 
                                                                     PortIndex, DEF_TYPE);
    if (pBcProps->m_Type == DEF_TYPE)
    {
        DtDbgOut(ERR, BC_COMMON, "Failed to get block-type property '%s'", PropName);
        return DT_STATUS_FAIL;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Get block-UUID property -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    Status = DtBc_ToPropertyNameFromStrings(pInstanceId, "UUID", PropName, 
                                                               DT_SIZEOF_ARRAY(PropName));
    DT_ASSERT(DT_SUCCESS(Status));

    pBcProps->m_Uuid = DtCore_PROPS_GetInt(pCore, PropName, PortIndex, DEF_UUID);
    if (pBcProps->m_Uuid == DEF_UUID)
    {
        DtDbgOut(ERR, BC_COMMON, "Failed to get block-UUID property '%s'", PropName);
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}
