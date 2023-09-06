// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcAD9789ITF.c *#*#*#*#*#*#*#*#*#* (C) 2023 DekTec
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
#include "DtBcAD9789ITF.h"
#include "DtBcAD9789ITF_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcAD9789ITF implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_AD9789ITF_DEFAULT_PRECONDITIONS(pBc)      \
                                DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcAD9789ITF))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_AD9789ITF_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(AD9789ITF, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcAD9789ITF_GetOperationalMode(DtBcAD9789ITF* pBc, Int* pOpMode);
static DtStatus  DtBcAD9789ITF_Init(DtBc*);
static DtStatus  DtBcAD9789ITF_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcAD9789ITF_OnEnable(DtBc*, Bool  Enable);
static void  DtBcAD9789ITF_SetControlRegs(DtBcAD9789ITF*, Bool BlkEna, Int OpMode);
static DtStatus  DtBcAD9789ITF_SetOperationalMode(DtBcAD9789ITF* pBc, Int OpMode);

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcAD9789ITF - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD9789ITF_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcAD9789ITF_Close(DtBc*  pBc)
{
    BC_AD9789ITF_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD9789ITF_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcAD9789ITF*  DtBcAD9789ITF_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                              const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_AD9789ITF_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcAD9789ITF, Id, DT_BLOCK_TYPE_AD9789ITF,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcAD9789ITF_Close;
    OpenParams.m_InitFunc = DtBcAD9789ITF_Init;
    OpenParams.m_OnEnableFunc = DtBcAD9789ITF_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcAD9789ITF_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcAD9789ITF*)DtBc_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD9789ITF_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcAD9789ITF_GetOperationalMode(DtBcAD9789ITF* pBc, Int* pOpMode)
{
    // Sanity check
    BC_AD9789ITF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_AD9789ITF_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD9789ITF_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcAD9789ITF_SetOperationalMode(DtBcAD9789ITF* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_AD9789ITF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
        && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_AD9789ITF_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcAD9789ITF_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcAD9789ITF - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD9789ITF_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcAD9789ITF_Init(DtBc* pBcBase)
{
    DtBcAD9789ITF* pBc = (DtBcAD9789ITF*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    // Set control register
    DtBcAD9789ITF_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD9789ITF_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcAD9789ITF_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcAD9789ITF* pBc = (DtBcAD9789ITF*)pBcBase;

    // Sanity check
    BC_AD9789ITF_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, AD9789ITF, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, AD9789ITF, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcAD9789ITF_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, AD9789ITF, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcAD9789ITF_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD9789ITF_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcAD9789ITF_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcAD9789ITF* pBc = (DtBcAD9789ITF*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_AD9789ITF_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, AD9789ITF, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcAD9789ITF_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, AD9789ITF, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD9789ITF_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcAD9789ITF_SetControlRegs(DtBcAD9789ITF* pBc, Bool BlkEna, Int OpMode)
{
    UInt32  RegData = 0, FwBlkEna = 0, FwOpMode = 0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? AD9789ITF_BLKENA_ENABLED : AD9789ITF_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = AD9789ITF_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = AD9789ITF_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = AD9789ITF_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = AD9789ITF_Control_READ(pBc);
    RegData = AD9789ITF_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = AD9789ITF_Control_SET_OperationalMode(RegData, FwOpMode);
    AD9789ITF_Control_WRITE(pBc, RegData);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcAD9789ITF implementation +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define AD9789ITF_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                      DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcAD9789ITF))
// Helper macro to cast stub's DtBc member to DtBcAD9789ITF
#define AD9789ITF_STUB   ((DtIoStubBcAD9789ITF*)pStub)
#define AD9789ITF_BC  ((DtBcAD9789ITF*)AD9789ITF_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcAD9789ITF_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcAD9789ITF_OnCmdGetOperationalMode(const DtIoStubBcAD9789ITF*,
                                                     DtIoctlAd9789ItfCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcAD9789ITF_OnCmdSetOperationalMode(const DtIoStubBcAD9789ITF*,
                                                const DtIoctlAd9789ItfCmdSetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_AD9789ITF;

static const DtIoctlProperties  IOSTUB_BC_AD9789ITF_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_AD9789ITF_CMD(
        DtIoStubBcAD9789ITF_OnCmd,
        NULL, NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcAD9789ITF - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD9789ITF_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcAD9789ITF_Close(DtIoStub* pStub)
{
    AD9789ITF_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD9789ITF_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcAD9789ITF*  DtIoStubBcAD9789ITF_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_AD9789ITF_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcAD9789ITF, pBc, NULL,
        DtIoStubBcAD9789ITF_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_AD9789ITF_IOCTLS);
    return (DtIoStubBcAD9789ITF*)DtIoStubBc_Open(&OpenParams);
}

// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcAD9789ITF - Private functions +=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD9789ITF_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcAD9789ITF_OnCmd(const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlAd9789ItfCmdInput*  pInData = NULL;
    DtIoctlAd9789ItfCmdOutput*  pOutData = NULL;

    AD9789ITF_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_AD9789ITF_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, AD9789ITF, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_Ad9789ItfCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_Ad9789ItfCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_AD9789ITF_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcAD9789ITF_OnCmdGetOperationalMode(AD9789ITF_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_AD9789ITF_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcAD9789ITF_OnCmdSetOperationalMode(AD9789ITF_STUB,
                                                                   &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD9789ITF_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcAD9789ITF_OnCmdGetOperationalMode(
    const DtIoStubBcAD9789ITF* pStub,
    DtIoctlAd9789ItfCmdGetOpModeOutput* pOutData)
{
    AD9789ITF_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcAD9789ITF_GetOperationalMode(AD9789ITF_BC, &pOutData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD9789ITF_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcAD9789ITF_OnCmdSetOperationalMode(
    const DtIoStubBcAD9789ITF* pStub,
    const DtIoctlAd9789ItfCmdSetOpModeInput* pInData)
{
    AD9789ITF_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcAD9789ITF_SetOperationalMode(AD9789ITF_BC, pInData->m_OpMode);
}
