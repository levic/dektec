//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIPSUMINS.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
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
#include "DtBcIPSUMINS.h"
#include "DtBcIPSUMINS_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIPSUMINS implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IPSUMINS_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIPSUMINS))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IPSUMINS_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IPSUMINS, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIPSUMINS_Init(DtBc*);
static DtStatus  DtBcIPSUMINS_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIPSUMINS_OnEnable(DtBc*, Bool  Enable);
static void  DtBcIPSUMINS_SetControlRegs(DtBcIPSUMINS*, Bool BlkEna, Int OpMode);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIPSUMINS - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSUMINS_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIPSUMINS_Close(DtBc*  pBc)
{
    BC_IPSUMINS_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSUMINS_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIPSUMINS*  DtBcIPSUMINS_Open(Int  Address, DtCore* pCore, DtPt* pPt, 
                                                const char* pRole, Int Instance, Int Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_IPSUMINS_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIPSUMINS, Id, DT_BLOCK_TYPE_IPSUMINS,
                                                              Address, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIPSUMINS_Close;
    OpenParams.m_InitFunc = DtBcIPSUMINS_Init;
    OpenParams.m_OnEnableFunc = DtBcIPSUMINS_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIPSUMINS_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIPSUMINS*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSUMINS_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIPSUMINS_GetOperationalMode(DtBcIPSUMINS* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IPSUMINS_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IPSUMINS_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSUMINS_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIPSUMINS_SetOperationalMode(DtBcIPSUMINS* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_IPSUMINS_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY 
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IPSUMINS_MUST_BE_ENABLED(pBc);
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcIPSUMINS_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIPSUMINS - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSUMINS_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIPSUMINS_Init(DtBc* pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIPSUMINS* pBc = (DtBcIPSUMINS*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    // Set control register
    DtBcIPSUMINS_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSUMINS_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIPSUMINS_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIPSUMINS* pBc = (DtBcIPSUMINS*)pBcBase;

    // Sanity check
    BC_IPSUMINS_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IPSUMINS, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IPSUMINS, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcIPSUMINS_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IPSUMINS, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIPSUMINS_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSUMINS_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIPSUMINS_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIPSUMINS* pBc = (DtBcIPSUMINS*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IPSUMINS_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IPSUMINS, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIPSUMINS_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IPSUMINS, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIPSUMINS_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIPSUMINS_SetControlRegs(DtBcIPSUMINS* pBc, Bool BlkEna, Int OpMode)
{
    UInt32  RegData=0, FwBlkEna=0, FwOpMode=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IPSUMINS_BLKENA_ENABLED : IPSUMINS_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = IPSUMINS_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = IPSUMINS_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = IPSUMINS_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = IPSUMINS_Control_READ(pBc);
    RegData = IPSUMINS_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IPSUMINS_Control_SET_EnableIpv4Checksum(RegData, FwBlkEna);
    RegData = IPSUMINS_Control_SET_EnableUdpChecksum(RegData, FwBlkEna);
    RegData = IPSUMINS_Control_SET_EnableTcpChecksum(RegData, FwBlkEna);
    RegData = IPSUMINS_Control_SET_OperationalMode(RegData, FwOpMode);
    IPSUMINS_Control_WRITE(pBc, RegData);
}
