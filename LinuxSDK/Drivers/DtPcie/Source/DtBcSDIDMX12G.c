// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDIDMX12G.c *#*#*#*#*#*#*#*#*#* (C) 2019 DekTec
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2019 DekTec Digital Video B.V.
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
#include "DtBcSDIDMX12G.h"
#include "DtBcSDIDMX12G_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIDMX12G implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_SDIDMX12G_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDIDMX12G))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_SDIDMX12G_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(SDIDMX12G, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcSDIDMX12G_Init(DtBc*);
static DtStatus  DtBcSDIDMX12G_OnEnable(DtBc*, Bool);
static void  DtBcSDIDMX12G_SetControlRegs(DtBcSDIDMX12G*, Bool BlkEna, Int OpMode, 
                                                                             Int SdiRate);

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIDMX12G - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIDMX12G_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcSDIDMX12G_Close(DtBc*  pBc)
{
    BC_SDIDMX12G_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIDMX12G_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcSDIDMX12G*  DtBcSDIDMX12G_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_SDIDMX12G_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcSDIDMX12G, Id, DT_BLOCK_TYPE_SDIDMX12G,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcSDIDMX12G_Close;
    OpenParams.m_InitFunc = DtBcSDIDMX12G_Init;
    OpenParams.m_OnEnableFunc = DtBcSDIDMX12G_OnEnable;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcSDIDMX12G*)DtBc_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIDMX12G_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIDMX12G_GetOperationalMode(DtBcSDIDMX12G* pBc, Int* pOpMode)
{
    // Sanity check
    BC_SDIDMX12G_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIDMX12G_MUST_BE_ENABLED(pBc);

    // Return last cached operational mode
    *pOpMode = pBc->m_OperationalMode;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIDMX12G_GetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIDMX12G_GetSdiRate(DtBcSDIDMX12G* pBc, Int* pSdiRate)
{
    // Sanity check
    BC_SDIDMX12G_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIDMX12G_MUST_BE_ENABLED(pBc);

    // Return cached SdiRate
    *pSdiRate = pBc->m_SdiRate;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIDMX12G_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIDMX12G_SetOperationalMode(DtBcSDIDMX12G* pBc, Int OpMode)
{
    // Sanity check
    BC_SDIDMX12G_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDIDMX12G, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_SDIDMX12G_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Save new setting
    pBc->m_OperationalMode = OpMode;
    // Set control registers
    DtBcSDIDMX12G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                          pBc->m_SdiRate);
    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIDMX12G_SetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIDMX12G_SetSdiRate(DtBcSDIDMX12G* pBc, Int SdiRate)
{
    // Sanity check
    BC_SDIDMX12G_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (   SdiRate!=DT_DRV_SDIRATE_SD && SdiRate!=DT_DRV_SDIRATE_HD 
        && SdiRate!=DT_DRV_SDIRATE_3G && SdiRate!=DT_DRV_SDIRATE_6G 
        && SdiRate!=DT_DRV_SDIRATE_12G)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Must be enabled
    BC_SDIDMX12G_MUST_BE_ENABLED(pBc);

    // Operational mode must be IDLE
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
    {
        DtDbgOutBc(ERR, SDIDMX12G, pBc, "Operational mode not in idle");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // No change?
    if (pBc->m_SdiRate == SdiRate)
        return DT_STATUS_OK;

    // Save new setting
    pBc->m_SdiRate = SdiRate;
    // Set control registers
    DtBcSDIDMX12G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                          pBc->m_SdiRate);
    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIDMX12G - Private functions -.-.-.-.-.-.-.-.-.-.-.-.-.

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIDMX12G_Init +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
DtStatus  DtBcSDIDMX12G_Init(DtBc*  pBcBase)
{
    DtBcSDIDMX12G* pBc = (DtBcSDIDMX12G*)pBcBase;

    // Sanity checks
    BC_SDIDMX12G_DEFAULT_PRECONDITIONS(pBc);

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_SdiRate = DT_DRV_SDIRATE_SD;
    // Set control registers
    DtBcSDIDMX12G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                          pBc->m_SdiRate);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIDMX12G_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIDMX12G_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcSDIDMX12G* pBc = (DtBcSDIDMX12G*)pBcBase;

    // Sanity check
    BC_SDIDMX12G_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    { 
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, SDIDMX12G, pBc, "Block Enable from disable -> enable");

        // Save new setting
        pBc->m_BlockEnabled = Enable;
        // Set control registers
        DtBcSDIDMX12G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                          pBc->m_SdiRate);
    }
    else
    { 
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, SDIDMX12G, pBc, "Block Enable from enable -> disable");
        Status = DtBcSDIDMX12G_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        DT_ASSERT(DT_SUCCESS(Status));
        // Save new setting
        pBc->m_BlockEnabled = Enable;
        // Set control registers
        DtBcSDIDMX12G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                          pBc->m_SdiRate);
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIDMX12G_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcSDIDMX12G_SetControlRegs(DtBcSDIDMX12G* pBc, Bool BlkEnable, Int OpMode, 
                                                                              Int SdiRate)
{
     UInt RegData, FwOpMode, FwBlkEnable, FwSdiRate;
     //Sanity check
    BC_SDIDMX12G_DEFAULT_PRECONDITIONS(pBc);


    // Convert block enable to BB-type
    FwBlkEnable = BlkEnable ? SDIDMX12G_BLKENA_ENABLED : SDIDMX12G_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = SDIDMX12G_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = SDIDMX12G_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE); return;
    }
    // Convert SDI-rate to BB-type
    switch (SdiRate)
    {
    case DT_DRV_SDIRATE_SD:   FwSdiRate = SDIDMX12G_SDIMODE_SD; break;
    case DT_DRV_SDIRATE_HD:   FwSdiRate = SDIDMX12G_SDIMODE_HD; break;
    case DT_DRV_SDIRATE_3G:   FwSdiRate = SDIDMX12G_SDIMODE_3G; break;
    case DT_DRV_SDIRATE_6G:   FwSdiRate = SDIDMX12G_SDIMODE_6G; break;
    case DT_DRV_SDIRATE_12G:  FwSdiRate = SDIDMX12G_SDIMODE_12G;  break;
    default: DT_ASSERT(FALSE);  return;
    }

    // Invalid combination?
    DT_ASSERT(BlkEnable || OpMode==DT_BLOCK_OPMODE_IDLE);

    // Update SDIDMX12G control register
    RegData = SDIDMX12G_Control_READ(pBc);
    RegData = SDIDMX12G_Control_SET_BlockEnable(RegData, FwBlkEnable);
    RegData = SDIDMX12G_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = SDIDMX12G_Control_SET_SdiRate(RegData, FwSdiRate);
    SDIDMX12G_Control_WRITE(pBc, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIDMX12G implementation +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define SDIDMX12G_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                      DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIDMX12G))
// Helper macro to cast stub's DtBc member to DtBcSDIDMX12G
#define SDIDMX12G_STUB   ((DtIoStubBcSDIDMX12G*)pStub)
#define SDIDMX12G_BC  ((DtBcSDIDMX12G*)SDIDMX12G_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcSDIDMX12G_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);

static DtStatus  DtIoStubBcSDIDMX12G_OnCmdGetOperationalMode(const DtIoStubBcSDIDMX12G*, 
                                                     DtIoctlSdiDmx12GCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcSDIDMX12G_OnCmdGetSdiRate(const DtIoStubBcSDIDMX12G*, 
                                                    DtIoctlSdiDmx12GCmdGetSdiRateOutput*);
static DtStatus  DtIoStubBcSDIDMX12G_OnCmdSetOperationalMode(const DtIoStubBcSDIDMX12G*, 
                                                const DtIoctlSdiDmx12GCmdSetOpModeInput*);
static DtStatus  DtIoStubBcSDIDMX12G_OnCmdSetSdiRate(const DtIoStubBcSDIDMX12G*, 
                                               const DtIoctlSdiDmx12GCmdSetSdiRateInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_SDIDMX12G;

static const DtIoctlProperties  IOSTUB_BC_SDIDMX12G_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_SDIDMX12G_CMD(
        DtIoStubBcSDIDMX12G_OnCmd,
        NULL,  NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIDMX12G - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIDMX12G_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcSDIDMX12G_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIDMX12G));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIDMX12G_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcSDIDMX12G*  DtIoStubBcSDIDMX12G_Open(DtBc*  pBc)
{
    DtIoStubBcSDIDMX12G*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    BC_SDIDMX12G_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcSDIDMX12G, pBc, NULL, 
                                                             DtIoStubBcSDIDMX12G_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_BC_SDIDMX12G_IOCTLS);
    pStub = (DtIoStubBcSDIDMX12G*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIDMX12G- Private functions +=+=+=+=+=+=+=+=+=+=+=+=


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIDMX12G_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDIDMX12G_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSdiDmx12GCmdInput*  pInData = NULL;
    DtIoctlSdiDmx12GCmdOutput*  pOutData = NULL;

    SDIDMX12G_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_SDIDMX12G_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, SDIDMX12G, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SdiDmx12GCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_SdiDmx12GCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_SDIDMX12G_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDIDMX12G_OnCmdGetOperationalMode(SDIDMX12G_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_SDIDMX12G_CMD_GET_SDIRATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDIDMX12G_OnCmdGetSdiRate(SDIDMX12G_STUB, 
                                                                 &pOutData->m_GetSdiRate);
        break;
    case DT_SDIDMX12G_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSDIDMX12G_OnCmdSetOperationalMode(SDIDMX12G_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_SDIDMX12G_CMD_SET_SDIRATE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSDIDMX12G_OnCmdSetSdiRate(SDIDMX12G_STUB, 
                                                                  &pInData->m_SetSdiRate);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIDMX12G_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIDMX12G_OnCmdGetOperationalMode(
    const DtIoStubBcSDIDMX12G* pStub,
    DtIoctlSdiDmx12GCmdGetOpModeOutput* pOutData)
{
    SDIDMX12G_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcSDIDMX12G_GetOperationalMode(SDIDMX12G_BC, &pOutData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIDMX12G_OnCmdGetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIDMX12G_OnCmdGetSdiRate(
    const DtIoStubBcSDIDMX12G* pStub,
    DtIoctlSdiDmx12GCmdGetSdiRateOutput* pOutData)
{
    SDIDMX12G_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcSDIDMX12G_GetSdiRate(SDIDMX12G_BC, &pOutData->m_SdiRate);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIDMX12G_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIDMX12G_OnCmdSetOperationalMode(
    const DtIoStubBcSDIDMX12G* pStub,
    const DtIoctlSdiDmx12GCmdSetOpModeInput* pInData)
{
    SDIDMX12G_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcSDIDMX12G_SetOperationalMode(SDIDMX12G_BC, pInData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIDMX12G_OnCmdSetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIDMX12G_OnCmdSetSdiRate(
    const DtIoStubBcSDIDMX12G* pStub,
    const DtIoctlSdiDmx12GCmdSetSdiRateInput* pInData)
{
    SDIDMX12G_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcSDIDMX12G_SetSdiRate(SDIDMX12G_BC, pInData->m_SdiRate);
}
