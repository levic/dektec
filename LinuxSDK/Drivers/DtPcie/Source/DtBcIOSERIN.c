// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIOSERIN.c *#*#*#*#*#*#*#*#*#*# (C) 2020 DekTec
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
#include "DtBc.h"
#include "DtBcIOSERIN.h"
#include "DtBcIOSERIN_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIOSERIN implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Helper macro to cast a DtBc* to a DtBcIOSERIN*
#define  BC_IOSERIN         ((DtBcIOSERIN*)pBc)

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IOSERIN_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IOSERIN, pBc)

// Macro with default precondition checks for the BcIOSERIN block-controller
#define BC_IOSERIN_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIOSERIN))

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIOSERIN_Init(DtBc*);
static DtStatus  DtBcIOSERIN_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIOSERIN_OnEnable(DtBc*, Bool  Enable);
static void  DtBcIOSERIN_SetControlRegs(DtBcIOSERIN*, Bool BlkEna, Int OpMode);


// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIOSERIN - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOSERIN_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIOSERIN_Close(DtBc*  pBc)
{
    BC_IOSERIN_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOSERIN_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIOSERIN*  DtBcIOSERIN_Open(Int  Address, DtCore*  pCore, DtPt*  pPt,
                                                        const char*  pRole, Int  Instance,
                                                        Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_IOSERIN_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIOSERIN, Id, DT_BLOCK_TYPE_IOSERIN, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIOSERIN_Close;
    OpenParams.m_InitFunc = DtBcIOSERIN_Init; 
    OpenParams.m_OnEnableFunc = DtBcIOSERIN_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIOSERIN_OnCloseFile;

    // Use base function to allocate and perform standard initialization of block data
    return (DtBcIOSERIN*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOSERIN_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIOSERIN_GetOperationalMode(DtBcIOSERIN* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IOSERIN_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IOSERIN_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOSERIN_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIOSERIN_SetOperationalMode(DtBcIOSERIN* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_IOSERIN_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
                                                         && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IOSERIN_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    DtBcIOSERIN_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode);

    // Cache value
    pBc->m_OperationalMode = OpMode;
    return Status;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIOSERIN - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOSERIN_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIOSERIN_Init(DtBc* pBcBase)
{
    DtBcIOSERIN* pBc = (DtBcIOSERIN*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    // Set control register
    DtBcIOSERIN_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOSERIN_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIOSERIN_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIOSERIN* pBc = (DtBcIOSERIN*)pBcBase;

    // Sanity check
    BC_IOSERIN_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IOSERIN, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IOSERIN, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcIOSERIN_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IOSERIN, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIOSERIN_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOSERIN_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIOSERIN_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIOSERIN* pBc = (DtBcIOSERIN*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IOSERIN_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IOSERIN, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIOSERIN_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IOSERIN, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOSERIN_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIOSERIN_SetControlRegs(DtBcIOSERIN* pBc, Bool BlkEna, Int OpMode)
{
    UInt32 RegData=0, FwBlkEna = 0, FwOpMode = 0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IOSERIN_BLKENA_ENABLED : IOSERIN_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = IOSERIN_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = IOSERIN_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = IOSERIN_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control
    RegData = IOSERIN_Control_READ(pBc);
    RegData = IOSERIN_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IOSERIN_Control_SET_OperationalMode(RegData, FwOpMode);
    IOSERIN_Control_WRITE(pBc, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIOSERIN implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IOSERIN_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIOSERIN))
// Helper macro to cast stub's DtBc member to DtBcIOSERIN
#define IOSERIN_STUB   ((DtIoStubBcIOSERIN*)pStub)
#define IOSERIN_BC  ((DtBcIOSERIN*)IOSERIN_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIOSERIN_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcIOSERIN_OnCmdGetOpMode( const DtIoStubBcIOSERIN*,
                                                       DtIoctlIoSerInCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcIOSERIN_OnCmdSetOpMode(const DtIoStubBcIOSERIN*,
                                                  const DtIoctlIoSerInCmdSetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IOSERIN;

static const DtIoctlProperties  IOSTUB_BC_IOSERIN_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IOSERIN_CMD(
        DtIoStubBcIOSERIN_OnCmd,
        NULL, NULL),
};

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIOSERIN - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIOSERIN_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcIOSERIN_Close(DtIoStub* pStub)
{
    IOSERIN_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIOSERIN_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcIOSERIN*  DtIoStubBcIOSERIN_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IOSERIN_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIOSERIN, pBc, NULL,
        DtIoStubBcIOSERIN_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_IOSERIN_IOCTLS);
    return (DtIoStubBcIOSERIN*)DtIoStubBc_Open(&OpenParams);
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIOSERIN - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIOSERIN_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIOSERIN_OnCmd(const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIoSerInCmdInput*  pInData = NULL;
    DtIoctlIoSerInCmdOutput*  pOutData = NULL;

    IOSERIN_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IOSERIN_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IOSERIN, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IoSerInCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IoSerInCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IOSERIN_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIOSERIN_OnCmdGetOpMode(IOSERIN_STUB, &pOutData->m_GetOpMode);
        break;
    case DT_IOSERIN_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIOSERIN_OnCmdSetOpMode(IOSERIN_STUB, &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIOSERIN_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIOSERIN_OnCmdGetOpMode( const DtIoStubBcIOSERIN* pStub,
                                               DtIoctlIoSerInCmdGetOpModeOutput* pOutData)
{
    IOSERIN_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIOSERIN_GetOperationalMode(IOSERIN_BC, &pOutData->m_OpMode);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIOSERIN_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIOSERIN_OnCmdSetOpMode( const DtIoStubBcIOSERIN* pStub,
                                           const DtIoctlIoSerInCmdSetOpModeInput* pInData)
{
    IOSERIN_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIOSERIN_SetOperationalMode(IOSERIN_BC, pInData->m_OpMode);
}
