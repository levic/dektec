// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcS12GTO3G.c *#*#*#*#*#*#*#*#*#*# (C) 2020 DekTec
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2020 DekTec Digital Video B.V.
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
#include "DtBcS12GTO3G.h"
#include "DtBcS12GTO3G_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS12GTO3G implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_S12GTO3G_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcS12GTO3G))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_S12GTO3G_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(S12GTO3G, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcS12GTO3G_Init(DtBc*);
static DtStatus  DtBcS12GTO3G_OnEnable(DtBc*, Bool);
static DtStatus DtBcS12GTO3G_OnCloseFile(DtBc*, const DtFileObject*);
static void  DtBcS12GTO3G_SetControlRegs(DtBcS12GTO3G*, Bool BlkEna, Int OpMode, 
                                                                           Bool ScaleEna);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS12GTO3G - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS12GTO3G_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcS12GTO3G_Close(DtBc*  pBc)
{
    BC_S12GTO3G_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS12GTO3G_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcS12GTO3G*  DtBcS12GTO3G_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_S12GTO3G_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcS12GTO3G, Id, DT_BLOCK_TYPE_S12GTO3G, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcS12GTO3G_Close;
    OpenParams.m_InitFunc = DtBcS12GTO3G_Init;
    OpenParams.m_OnCloseFileFunc = DtBcS12GTO3G_OnCloseFile;
    OpenParams.m_OnEnableFunc = DtBcS12GTO3G_OnEnable;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcS12GTO3G*)DtBc_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS12GTO3G_GetScalingEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS12GTO3G_GetScalingEnable(DtBcS12GTO3G* pBc, Int* pEnable)
{
    // Sanity check
    BC_S12GTO3G_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pEnable == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S12GTO3G_MUST_BE_ENABLED(pBc);

    // Return cached position
    *pEnable = pBc->m_ScalingEnabled ? TRUE : FALSE;
    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS12GTO3G_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcS12GTO3G_GetOperationalMode(DtBcS12GTO3G* pBc, Int* pOpMode)
{
    // Sanity check
    BC_S12GTO3G_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S12GTO3G_MUST_BE_ENABLED(pBc);

    // Return last cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS12GTO3G_SetScalingEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS12GTO3G_SetScalingEnable(DtBcS12GTO3G* pBc, Int Enable)
{
    // Sanity check
    BC_S12GTO3G_DEFAULT_PRECONDITIONS(pBc);


    // Must be enabled
    BC_S12GTO3G_MUST_BE_ENABLED(pBc);

    // Must be in IDLE
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    // No change?
    if (pBc->m_ScalingEnabled == (Bool)Enable)
        return DT_STATUS_OK;

    // Save new setting
    pBc->m_ScalingEnabled = (Bool)Enable;
    DtBcS12GTO3G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                    pBc->m_ScalingEnabled);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS12GTO3G_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcS12GTO3G_SetOperationalMode(DtBcS12GTO3G* pBc, Int OpMode)
{
    // Sanity check
    BC_S12GTO3G_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, S12GTO3G, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_S12GTO3G_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Save new setting
    pBc->m_OperationalMode = OpMode;
    DtBcS12GTO3G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                   pBc->m_ScalingEnabled);

    return DT_STATUS_OK;
}


// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS12GTO3G - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS12GTO3G_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcS12GTO3G_Init(DtBc*  pBcBase)
{
    DtBcS12GTO3G* pBc = (DtBcS12GTO3G*)pBcBase;

    // Sanity checks
    BC_S12GTO3G_DEFAULT_PRECONDITIONS(pBc);

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_ScalingEnabled = FALSE;
    DtBcS12GTO3G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                   pBc->m_ScalingEnabled);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS12GTO3G_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcS12GTO3G_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcS12GTO3G* pBc = (DtBcS12GTO3G*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_S12GTO3G_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, S12GTO3G, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcS12GTO3G_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, S12GTO3G, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS12GTO3G_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS12GTO3G_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcS12GTO3G* pBc = (DtBcS12GTO3G*)pBcBase;

    // Sanity check
    BC_S12GTO3G_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, S12GTO3G, pBc, "Block Enable from disable -> enable");
        // Save new setting
        pBc->m_BlockEnabled = Enable;
        DtBcS12GTO3G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                   pBc->m_ScalingEnabled);
    } else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, S12GTO3G, pBc, "Block Enable from enable -> disable");
        Status = DtBcS12GTO3G_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        DT_ASSERT(DT_SUCCESS(Status));
        // Save new setting
        pBc->m_BlockEnabled = Enable;
        DtBcS12GTO3G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                   pBc->m_ScalingEnabled);
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS12GTO3G_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcS12GTO3G_SetControlRegs(DtBcS12GTO3G* pBc, Bool BlkEnable, Int OpMode,
                                                                            Bool ScaleEna)
{
     UInt RegData, FwOpMode, FwBlkEnable;
     //Sanity check
    BC_S12GTO3G_DEFAULT_PRECONDITIONS(pBc);

    // Convert block enable to BB-type
    FwBlkEnable = BlkEnable ? S12GTO3G_BLKENA_ENABLED : S12GTO3G_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = S12GTO3G_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = S12GTO3G_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = S12GTO3G_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE); return;
    }

    // Invalid combination?
    DT_ASSERT(BlkEnable || OpMode==DT_BLOCK_OPMODE_IDLE);

    // Update S12GTO3G control register
    RegData = S12GTO3G_Control_READ(pBc);
    RegData = S12GTO3G_Control_SET_BlockEnable(RegData, FwBlkEnable);
    RegData = S12GTO3G_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = S12GTO3G_Control_SET_EnaScaling(RegData, ScaleEna ? 1 : 0);
    S12GTO3G_Control_WRITE(pBc, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcS12GTO3G implementation +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcS12GTO3G
#define S12GTO3G_STUB   ((DtIoStubBcS12GTO3G*)pStub)
#define S12GTO3G_BC  ((DtBcS12GTO3G*)S12GTO3G_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcS12GTO3G_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);

static DtStatus  DtIoStubBcS12GTO3G_OnCmdGetOperationalMode(const DtIoStubBcS12GTO3G*, 
                                                      DtIoctlS12Gto3GCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcS12GTO3G_OnCmdGetScalingEnable(const DtIoStubBcS12GTO3G*, 
                                               DtIoctlS12Gto3GCmdGetScalingEnableOutput*);
static DtStatus  DtIoStubBcS12GTO3G_OnCmdSetOperationalMode(const DtIoStubBcS12GTO3G*, 
                                                 const DtIoctlS12Gto3GCmdSetOpModeInput*);
static DtStatus  DtIoStubBcS12GTO3G_OnCmdSetScalingEnable(const DtIoStubBcS12GTO3G*, 
                                          const DtIoctlS12Gto3GCmdSetScalingEnableInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_S12GTO3G;

static const DtIoctlProperties  IOSTUB_BC_S12GTO3G_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_S12GTO3G_CMD(
        DtIoStubBcS12GTO3G_OnCmd,
        NULL,  NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcS12GTO3G - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS12GTO3G_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcS12GTO3G_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcS12GTO3G));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS12GTO3G_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcS12GTO3G*  DtIoStubBcS12GTO3G_Open(DtBc*  pBc)
{
    DtIoStubBcS12GTO3G*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcS12GTO3G));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcS12GTO3G, pBc, NULL, 
                                                             DtIoStubBcS12GTO3G_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_BC_S12GTO3G_IOCTLS);
    pStub = (DtIoStubBcS12GTO3G*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcS12GTO3G- Private functions +=+=+=+=+=+=+=+=+=+=+=+=


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS12GTO3G_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcS12GTO3G_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlS12Gto3GCmdInput*  pInData = NULL;
    DtIoctlS12Gto3GCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcS12GTO3G));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_S12GTO3G_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, S12GTO3G, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_S12Gto3GCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_S12Gto3GCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_S12GTO3G_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcS12GTO3G_OnCmdGetOperationalMode(S12GTO3G_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_S12GTO3G_CMD_GET_SCALING_ENABLE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcS12GTO3G_OnCmdGetScalingEnable(S12GTO3G_STUB, 
                                                              &pOutData->m_GetScalingEna);
        break;
    case DT_S12GTO3G_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcS12GTO3G_OnCmdSetOperationalMode(S12GTO3G_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_S12GTO3G_CMD_SET_SCALING_ENABLE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcS12GTO3G_OnCmdSetScalingEnable(S12GTO3G_STUB, 
                                                               &pInData->m_SetScalingEna);
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS12GTO3G_OnCmdGetScalingEnable -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcS12GTO3G_OnCmdGetScalingEnable(
    const DtIoStubBcS12GTO3G* pStub,
    DtIoctlS12Gto3GCmdGetScalingEnableOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcS12GTO3G));
    DT_ASSERT(pOutData != NULL);

    return DtBcS12GTO3G_GetScalingEnable(S12GTO3G_BC, &pOutData->m_ScalingEnable);
}


// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcS12GTO3G_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcS12GTO3G_OnCmdGetOperationalMode(
    const DtIoStubBcS12GTO3G* pStub,
    DtIoctlS12Gto3GCmdGetOpModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcS12GTO3G));
    DT_ASSERT(pOutData != NULL);

    return DtBcS12GTO3G_GetOperationalMode(S12GTO3G_BC, &pOutData->m_OpMode);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS12GTO3G_OnCmdSetScalingEnable -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcS12GTO3G_OnCmdSetScalingEnable(
    const DtIoStubBcS12GTO3G* pStub,
    const DtIoctlS12Gto3GCmdSetScalingEnableInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcS12GTO3G));
    DT_ASSERT(pInData != NULL);

    return DtBcS12GTO3G_SetScalingEnable(S12GTO3G_BC, pInData->m_ScalingEnable);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcS12GTO3G_OnCmdSetPosition -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcS12GTO3G_OnCmdSetOperationalMode(
    const DtIoStubBcS12GTO3G* pStub,
    const DtIoctlS12Gto3GCmdSetOpModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcS12GTO3G));
    DT_ASSERT(pInData != NULL);

    return DtBcS12GTO3G_SetOperationalMode(S12GTO3G_BC, pInData->m_OpMode);
}

