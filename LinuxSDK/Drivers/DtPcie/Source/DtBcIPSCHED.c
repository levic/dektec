// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIPSCHED.c *#*#*#*#*#*#*#* (C) 2018-2021 DekTec
//
// This file is not part of the PCIe driver. It can be used as template to create a new
// block controller. Replace "IPSCHED" by the building block's shortname (all uppercase)
// and replace "Template" by the building block's shortname in camel case. 
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
#include "DtBcIPSCHED.h"
#include "DtBcIPSCHED_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIPSCHED implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//#define MAX_DELAY_MS 0          // 0=Disabe no max delay
#define SCHED_OFFSET_NS 2000      // 1200ns: Measured by sending TX on time and 
                                  // receiving TX timestamp back from MAC.
                                  // 800ns extra margin

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IPSCHED_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIPSCHED))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IPSCHED_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IPSCHED, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIPSCHED_Init(DtBc*);
static DtStatus  DtBcIPSCHED_OnEnable(DtBc*, Bool  Enable);
static void  DtBcIPSCHED_SetControlRegs(DtBcIPSCHED*, Bool BlkEna);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIPSCHED - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSCHED_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIPSCHED_Close(DtBc*  pBc)
{
    BC_IPSCHED_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSCHED_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIPSCHED*  DtBcIPSCHED_Open(Int  Address, DtCore* pCore, DtPt* pPt, const char* pRole, 
                                                                   Int Instance, Int Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_IPSCHED_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIPSCHED, Id, DT_BLOCK_TYPE_IPSCHED,
                                                              Address, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIPSCHED_Close;
    OpenParams.m_InitFunc = DtBcIPSCHED_Init;
    OpenParams.m_OnEnableFunc = DtBcIPSCHED_OnEnable;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIPSCHED*)DtBc_Open(&OpenParams);
}


// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIPSCHED - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSCHED_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIPSCHED_Init(DtBc* pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIPSCHED* pBc = (DtBcIPSCHED*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    // Set control register
    DtBcIPSCHED_SetControlRegs(pBc, pBc->m_BlockEnabled);
    IPSCHED_SchedulingOffset_WRITE(pBc, SCHED_OFFSET_NS);

    pBc->m_NumInputs = IPSCHED_Config_READ_NumRtInputs(pBc);


    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSCHED_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIPSCHED_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIPSCHED* pBc = (DtBcIPSCHED*)pBcBase;
    Int i;

    // Sanity check
    BC_IPSCHED_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IPSCHED, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

        // Set priority and enables
        // In future, this can be set with parameters if needed
        // Input0: RTQ
        for (i=0; i<pBc->m_NumInputs; i++)
        {
            UInt32  RtPars = IPSCHED_RtPars_READ(pBc, i);
            RtPars = IPSCHED_RtPars_SET_Priority(RtPars, i);
            RtPars = IPSCHED_RtPars_SET_Enable(RtPars, 1);
            IPSCHED_RtPars_WRITE(pBc, i, RtPars);
        }
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IPSCHED, pBc, "OnEnable from enable -> disable");

        // Disable channels
        for (i=0; i< pBc->m_NumInputs; i++)
        {
            UInt32  RtPars = IPSCHED_RtPars_READ(pBc, i);
            RtPars = IPSCHED_RtPars_SET_Enable(RtPars, 0);
            IPSCHED_RtPars_WRITE(pBc, i, RtPars);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIPSCHED_SetControlRegs(pBc, pBc->m_BlockEnabled);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSCHED_SetMaxDelay -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIPSCHED_SetMaxDelay(DtBcIPSCHED* pBc, Int Index, UInt32 MaxDelay)
{
    UInt32  RtPars;
    DT_ASSERT(Index < pBc->m_NumInputs);
    if (Index >= pBc->m_NumInputs)
        return DT_STATUS_INVALID_PARAMETER;
    RtPars = IPSCHED_RtPars_READ(pBc, Index);
    RtPars = IPSCHED_RtPars_SET_MaxDelay(RtPars, MaxDelay);
    IPSCHED_RtPars_WRITE(pBc, Index, RtPars);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSCHED_SetErrorBehavior -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIPSCHED_SetErrorBehavior(DtBcIPSCHED* pBc, Int Index, Bool Discard)
{
    UInt32  RtPars;
    DT_ASSERT(Index < pBc->m_NumInputs);
    if (Index >= pBc->m_NumInputs)
        return DT_STATUS_INVALID_PARAMETER;
    RtPars = IPSCHED_RtPars_READ(pBc, Index);
    RtPars = IPSCHED_RtPars_SET_ErrorBehavior(RtPars, (Discard?IPSCHED_ERRBEHAV_Discard:
                                                                 IPSCHED_ERRBEHAV_Fatal));
    IPSCHED_RtPars_WRITE(pBc, Index, RtPars);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSCHED_Flush -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIPSCHED_Flush(DtBcIPSCHED* pBc, Int Index)
{
    UInt32  RtPars;
    DT_ASSERT(Index < pBc->m_NumInputs);
    if (Index >= pBc->m_NumInputs)
        return DT_STATUS_INVALID_PARAMETER;
    RtPars = IPSCHED_RtPars_READ(pBc, Index);
    RtPars = IPSCHED_RtPars_SET_Flush(RtPars, 1);
    IPSCHED_RtPars_WRITE(pBc, Index, RtPars);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSCHED_GetOutputStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIPSCHED_GetOutputStatus(DtBcIPSCHED* pBc, Int Index, Bool* Waiting, 
                                       Bool* Ready, Bool* Sending, Bool* InvalidTimeError)
{
    UInt32  RtStat;
    DT_ASSERT(Index < pBc->m_NumInputs);
    if (Index >= pBc->m_NumInputs)
        return DT_STATUS_INVALID_PARAMETER;
    RtStat = IPSCHED_RtStat_READ(pBc, Index);
    *Waiting = IPSCHED_RtStat_GET_PacketWaiting(RtStat) != 0;
    *Ready = IPSCHED_RtStat_GET_PacketReady(RtStat) != 0;
    *Sending = IPSCHED_RtStat_GET_PacketSending(RtStat) != 0;
    *InvalidTimeError = IPSCHED_RtStat_GET_InvalidTimeError(RtStat) != 0;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSCHED_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIPSCHED_SetControlRegs(DtBcIPSCHED* pBc, Bool BlkEna)
{
    UInt32  RegData=0, FwBlkEna=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IPSCHED_BLKENA_ENABLED : IPSCHED_BLKENA_DISABLED;

    // Update control register
    RegData = IPSCHED_Control_READ(pBc);
    RegData = IPSCHED_Control_SET_BlockEnable(RegData, FwBlkEna);
    //RegData = IPSCHED_Control_SET_MaxDelay(RegData, MAX_DELAY_MS);
    IPSCHED_Control_WRITE(pBc, RegData);
}

