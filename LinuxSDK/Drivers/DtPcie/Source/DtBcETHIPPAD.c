//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcETHIPPAD.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
// This file is not part of the PCIe driver. It can be used as template to create a new
// block controller. Replace "ETHIPPAD" by the building block's shortname (all uppercase)
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
#include "DtBcETHIPPAD.h"
#include "DtBcETHIPPAD_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcETHIPPAD implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_ETHIPPAD_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcETHIPPAD))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_ETHIPPAD_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(ETHIPPAD, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcETHIPPAD_Init(DtBc*);
static DtStatus  DtBcETHIPPAD_OnEnable(DtBc*, Bool  Enable);
static void  DtBcETHIPPAD_SetControlRegs(DtBcETHIPPAD*, Bool BlkEna, Int OpMode);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcETHIPPAD - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcETHIPPAD_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcETHIPPAD_Close(DtBc*  pBc)
{
    BC_ETHIPPAD_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcETHIPPAD_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcETHIPPAD*  DtBcETHIPPAD_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                                                const char* pRole, Int Instance, Int Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_ETHIPPAD_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcETHIPPAD, Id, DT_BLOCK_TYPE_ETHIPPAD,
                                                              Address, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcETHIPPAD_Close;
    OpenParams.m_InitFunc = DtBcETHIPPAD_Init;
    OpenParams.m_OnEnableFunc = DtBcETHIPPAD_OnEnable;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcETHIPPAD*)DtBc_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcETHIPPAD_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcETHIPPAD_GetOperationalMode(DtBcETHIPPAD* pBc, Int* pOpMode)
{
    // Sanity check
    BC_ETHIPPAD_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ETHIPPAD_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcETHIPPAD_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcETHIPPAD_SetOperationalMode(DtBcETHIPPAD* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_ETHIPPAD_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY 
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ETHIPPAD_MUST_BE_ENABLED(pBc);
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Change Operational Mode
    if (OpMode == DT_BLOCK_OPMODE_RUN)
    {
        // IDLE or STANDBY -> RUN
        
    }
    else if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    {
        // RUN -> STANDBY or IDLE
    }

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcETHIPPAD_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcETHIPPAD - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcETHIPPAD_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcETHIPPAD_Init(DtBc* pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcETHIPPAD* pBc = (DtBcETHIPPAD*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    // Set control register
    DtBcETHIPPAD_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcETHIPPAD_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcETHIPPAD_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcETHIPPAD* pBc = (DtBcETHIPPAD*)pBcBase;

    // Sanity check
    BC_ETHIPPAD_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, ETHIPPAD, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, ETHIPPAD, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcETHIPPAD_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, ETHIPPAD, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcETHIPPAD_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcETHIPPAD_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcETHIPPAD_SetControlRegs(DtBcETHIPPAD* pBc, Bool BlkEna, Int OpMode)
{
    UInt32  RegData=0, FwBlkEna=0, FwOpMode=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? ETHIPPAD_BLKENA_ENABLED : ETHIPPAD_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = ETHIPPAD_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = ETHIPPAD_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = ETHIPPAD_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = ETHIPPAD_Control_READ(pBc);
    RegData = ETHIPPAD_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = ETHIPPAD_Control_SET_OperationalMode(RegData, FwOpMode);
    ETHIPPAD_Control_WRITE(pBc, RegData);
}
