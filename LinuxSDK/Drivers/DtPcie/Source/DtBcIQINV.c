// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQINV.c *#*#*#*#*#*#*#*#*#*#* (C) 2022 DekTec
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Copyright (C) 2021 DekTec Digital Video B.V.
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
#include "DtBcIQINV.h"
#include "DtBcIQINV_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQINV implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQINV_DEFAULT_PRECONDITIONS(pBc)                                              \
                                    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQINV))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQINV_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQINV, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQINV_Init(DtBc*);
static DtStatus  DtBcIQINV_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIQINV_OnEnable(DtBc*, Bool);
static void  DtBcIQINV_SetControlRegs(DtBcIQINV*, Bool, Int , Bool);

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQINV - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQINV_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcIQINV_Close(DtBc* pBc)
{
    BC_IQINV_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQINV_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcIQINV*  DtBcIQINV_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_IQINV_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQINV, Id, DT_BLOCK_TYPE_IQINV,
        Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQINV_Close;
    OpenParams.m_InitFunc = DtBcIQINV_Init;
    OpenParams.m_OnEnableFunc = DtBcIQINV_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIQINV_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQINV*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQINV_GetInvert -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQINV_GetInvert(DtBcIQINV* pBc, Int* pInvert)
{
    // Sanity check
    BC_IQINV_DEFAULT_PRECONDITIONS(pBc);

    DT_ASSERT(pInvert!=NULL);

    // Must be enabled
    BC_IQINV_MUST_BE_ENABLED(pBc);

    *pInvert = (Int)pBc->m_Invert;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQINV_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQINV_GetOperationalMode(DtBcIQINV* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IQINV_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQINV_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQINV_SetInvert -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQINV_SetInvert(DtBcIQINV* pBc, Int Invert)
{
    // Sanity check
    BC_IQINV_DEFAULT_PRECONDITIONS(pBc);

     // Must be enabled
    BC_IQINV_MUST_BE_ENABLED(pBc);

    if (Invert!=FALSE && Invert!=TRUE)
        return DT_STATUS_INVALID_PARAMETER;

    // Cache value
    pBc->m_Invert = (Bool)Invert;
    // Set control register
    DtBcIQINV_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                           pBc->m_Invert);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQINV_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQINV_SetOperationalMode(DtBcIQINV* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_IQINV_DEFAULT_PRECONDITIONS(pBc);


    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
        && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQINV_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcIQINV_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                           pBc->m_Invert);
    return Status;
}


// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQINV - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQINV_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQINV_Init(DtBc* pBcBase)
{
    DtBcIQINV* pBc = (DtBcIQINV*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_Invert = FALSE;

    // Set control register
    DtBcIQINV_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                           pBc->m_Invert);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQINV_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQINV_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQINV* pBc = (DtBcIQINV*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IQINV_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IQINV, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIQINV_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQINV, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQINV_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQINV_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQINV* pBc = (DtBcIQINV*)pBcBase;

    // Sanity check
    BC_IQINV_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IQINV, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IQINV, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcIQINV_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQINV, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIQINV_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                           pBc->m_Invert);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQINV_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQINV_SetControlRegs(DtBcIQINV* pBc, Bool BlkEna, Int OpMode, Bool Invert)
{
    UInt32  RegData = 0, FwBlkEna = 0, FwOpMode = 0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IQINV_BLKENA_ENABLED : IQINV_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = IQINV_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = IQINV_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = IQINV_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = IQINV_Control_READ(pBc);
    RegData = IQINV_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IQINV_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = IQINV_Control_SET_Invert(RegData, Invert ? 1 : 0);
    IQINV_Control_WRITE(pBc, RegData);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQINV implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IQINV_STUB_DEFAULT_PRECONDITIONS(pStub)                                          \
                          DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQINV))
// Helper macro to cast stub's DtBc member to DtBcIQINV
#define IQINV_STUB   ((DtIoStubBcIQINV*)pStub)
#define IQINV_BC  ((DtBcIQINV*)IQINV_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQINV_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcIQINV_OnCmdGetInvert(const DtIoStubBcIQINV*,
                                                         DtIoctlIqInvCmdGetInvertOutput*);
static DtStatus  DtIoStubBcIQINV_OnCmdGetOperationalMode(const DtIoStubBcIQINV*,
                                                          DtIoctlIqInvCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcIQINV_OnCmdSetOperationalMode(const DtIoStubBcIQINV*,
                                                     const DtIoctlIqInvCmdSetOpModeInput*);
static DtStatus  DtIoStubBcIQINV_OnCmdSetInvert(const DtIoStubBcIQINV*,
                                                   const DtIoctlIqInvCmdSetInvertInput*);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQINV;

static const DtIoctlProperties  IOSTUB_BC_IQINV_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQINV_CMD(
        DtIoStubBcIQINV_OnCmd,
        NULL, NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQINV - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQINV_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcIQINV_Close(DtIoStub* pStub)
{
    IQINV_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQINV_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcIQINV*  DtIoStubBcIQINV_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IQINV_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQINV, pBc, NULL,
        DtIoStubBcIQINV_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_IQINV_IOCTLS);
    return (DtIoStubBcIQINV*)DtIoStubBc_Open(&OpenParams);
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQINV - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQINV_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQINV_OnCmd(const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqInvCmdInput*  pInData = NULL;
    DtIoctlIqInvCmdOutput*  pOutData = NULL;

    IQINV_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQINV_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQINV, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqInvCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqInvCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IQINV_CMD_GET_INVERT:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQINV_OnCmdGetInvert(IQINV_STUB, &pOutData->m_GetInvert);
        break;
    case DT_IQINV_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQINV_OnCmdGetOperationalMode(IQINV_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_IQINV_CMD_SET_INVERT:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQINV_OnCmdSetInvert(IQINV_STUB, &pInData->m_SetInvert);
        break;
    case DT_IQINV_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQINV_OnCmdSetOperationalMode(IQINV_STUB,
                                                                   &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQINV_OnCmdGetInvert -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQINV_OnCmdGetInvert(const DtIoStubBcIQINV* pStub,
                                                 DtIoctlIqInvCmdGetInvertOutput* pOutData)
{
    IQINV_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQINV_GetInvert(IQINV_BC, &pOutData->m_Invert);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQINV_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQINV_OnCmdGetOperationalMode(
    const DtIoStubBcIQINV* pStub,
    DtIoctlIqInvCmdGetOpModeOutput* pOutData)
{
    IQINV_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQINV_GetOperationalMode(IQINV_BC, &pOutData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQINV_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQINV_OnCmdSetOperationalMode(
    const DtIoStubBcIQINV* pStub,
    const DtIoctlIqInvCmdSetOpModeInput* pInData)
{
    IQINV_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQINV_SetOperationalMode(IQINV_BC, pInData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQINV_OnCmdSetInvert -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQINV_OnCmdSetInvert(const DtIoStubBcIQINV* pStub,
                                            const DtIoctlIqInvCmdSetInvertInput* pInData)
{
    IQINV_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQINV_SetInvert(IQINV_BC, pInData->m_Invert);
}

