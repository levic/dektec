//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQNOISE.c *#*#*#*#*#*#*#*#*#*# (C) 2021 DekTec

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
#include "DtBcIQNOISE.h"
#include "DtBcIQNOISE_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQNOISE implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQNOISE_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQNOISE))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQNOISE_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQNOISE, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQNOISE_Init(DtBc*);
static DtStatus  DtBcIQNOISE_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIQNOISE_OnEnable(DtBc*, Bool  Enable);
static DtStatus  DtBcIQNOISE_GetBitDepth(DtBcIQNOISE* pBc, UInt8* pBitDepth);
static void  DtBcIQNOISE_SetControlRegs(DtBcIQNOISE*, Bool BlkEna, Int OpMode);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQNOISE - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQNOISE_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQNOISE_Close(DtBc*  pBc)
{
    BC_IQNOISE_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQNOISE_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIQNOISE*  DtBcIQNOISE_Open(Int  Address, DtCore* pCore, DtPt* pPt,
    const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_IQNOISE_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQNOISE, Id, DT_BLOCK_TYPE_IQNOISE,
        Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQNOISE_Close;
    OpenParams.m_InitFunc = DtBcIQNOISE_Init;
    OpenParams.m_OnEnableFunc = DtBcIQNOISE_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIQNOISE_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQNOISE*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQNOISE_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQNOISE_GetOperationalMode(DtBcIQNOISE* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IQNOISE_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQNOISE_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQNOISE_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQNOISE_SetOperationalMode(DtBcIQNOISE* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_IQNOISE_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
        && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQNOISE_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcIQNOISE_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQNOISE - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQNOISE_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQNOISE_Init(DtBc* pBcBase)
{
    DtBcIQNOISE* pBc = (DtBcIQNOISE*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_BitDepth = (UInt8)IQNOISE_Config_READ_BitDepth(pBc);

    // Set control register
    DtBcIQNOISE_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_GetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQNOISE_GetBitDepth(DtBcIQNOISE* pBc, UInt8* pBitDepth)
{
    // Sanity check
    BC_IQNOISE_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pBitDepth==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return cached configuration
    *pBitDepth = pBc->m_BitDepth;
    return DT_STATUS_OK;
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQNOISE_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQNOISE_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQNOISE* pBc = (DtBcIQNOISE*)pBcBase;

    // Sanity check
    BC_IQNOISE_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IQNOISE, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IQNOISE, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcIQNOISE_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQNOISE, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIQNOISE_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQNOISE_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQNOISE_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQNOISE* pBc = (DtBcIQNOISE*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IQNOISE_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IQNOISE, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIQNOISE_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQNOISE, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQNOISE_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQNOISE_SetControlRegs(DtBcIQNOISE* pBc, Bool BlkEna, Int OpMode)
{
    UInt32  RegData = 0, FwBlkEna = 0, FwOpMode = 0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IQNOISE_BLKENA_ENABLED : IQNOISE_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = IQNOISE_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = IQNOISE_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = IQNOISE_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = IQNOISE_Control_READ(pBc);
    RegData = IQNOISE_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IQNOISE_Control_SET_OperationalMode(RegData, FwOpMode);
    IQNOISE_Control_WRITE(pBc, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQNOISE implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IQNOISE_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQNOISE))
// Helper macro to cast stub's DtBc member to DtBcIQNOISE
#define IQNOISE_STUB   ((DtIoStubBcIQNOISE*)pStub)
#define IQNOISE_BC  ((DtBcIQNOISE*)IQNOISE_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQNOISE_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
DtStatus  DtIoStubBcIQNOISE_OnCmdGetBitDepth(const DtIoStubBcIQNOISE* pStub,
                                            DtIoctlIqNoiseCmdGetBitDepthOutput* pOutData);
static DtStatus  DtIoStubBcIQNOISE_OnCmdGetOperationalMode(const DtIoStubBcIQNOISE*,
                                                       DtIoctlIqNoiseCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcIQNOISE_OnCmdSetOperationalMode(const DtIoStubBcIQNOISE*,
                                                  const DtIoctlIqNoiseCmdSetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQNOISE;

static const DtIoctlProperties  IOSTUB_BC_IQNOISE_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQNOISE_CMD(
        DtIoStubBcIQNOISE_OnCmd,
        NULL, NULL),
};

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQNOISE - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQNOISE_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcIQNOISE_Close(DtIoStub* pStub)
{
    IQNOISE_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQNOISE_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcIQNOISE*  DtIoStubBcIQNOISE_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IQNOISE_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQNOISE, pBc, NULL,
        DtIoStubBcIQNOISE_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_IQNOISE_IOCTLS);
    return (DtIoStubBcIQNOISE*)DtIoStubBc_Open(&OpenParams);
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQNOISE - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQNOISE_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQNOISE_OnCmd(const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqNoiseCmdInput*  pInData = NULL;
    DtIoctlIqNoiseCmdOutput*  pOutData = NULL;

    IQNOISE_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQNOISE_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQNOISE, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqNoiseCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqNoiseCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-
    switch (pIoParams->m_Cmd)
    {
    case DT_IQNOISE_CMD_GET_BIT_DEPTH:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQNOISE_OnCmdGetBitDepth(IQNOISE_STUB, 
                                                                &pOutData->m_GetBitDepth);
        break;
    case DT_IQNOISE_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQNOISE_OnCmdGetOperationalMode(IQNOISE_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_IQNOISE_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQNOISE_OnCmdSetOperationalMode(IQNOISE_STUB,
                                                                   &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQNOISE_OnCmdGetBitDepth -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQNOISE_OnCmdGetBitDepth(
    const DtIoStubBcIQNOISE* pStub,
    DtIoctlIqNoiseCmdGetBitDepthOutput* pOutData)
{
    DtStatus Status = DT_STATUS_OK;
    UInt8  BitDepth=0;

    IQNOISE_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    
    Status = DtBcIQNOISE_GetBitDepth(IQNOISE_BC, &BitDepth);
    pOutData->m_BitDepth = BitDepth;
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQNOISE_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQNOISE_OnCmdGetOperationalMode(
    const DtIoStubBcIQNOISE* pStub,
    DtIoctlIqNoiseCmdGetOpModeOutput* pOutData)
{
    IQNOISE_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQNOISE_GetOperationalMode(IQNOISE_BC, &pOutData->m_OpMode);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQNOISE_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQNOISE_OnCmdSetOperationalMode(
    const DtIoStubBcIQNOISE* pStub,
    const DtIoctlIqNoiseCmdSetOpModeInput* pInData)
{
    IQNOISE_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQNOISE_SetOperationalMode(IQNOISE_BC, pInData->m_OpMode);
}
