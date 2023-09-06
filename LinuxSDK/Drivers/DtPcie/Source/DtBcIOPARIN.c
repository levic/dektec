// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIOPARIN.c *#*#*#*#*#*#*#*#*#*# (C) 2022 DekTec
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2022 DekTec Digital Video B.V.
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
#include "DtBcIOPARIN.h"
#include "DtBcIOPARIN_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIOPARIN implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Helper macro to cast a DtBc* to a DtBcIOPARIN*
#define  BC_IOPARIN         ((DtBcIOPARIN*)pBc)

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IOPARIN_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IOPARIN, pBc)

// Macro with default precondition checks for the BcIOPARIN block-controller
#define BC_IOPARIN_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIOPARIN))

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIOPARIN_Init(DtBc*);
static DtStatus  DtBcIOPARIN_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIOPARIN_OnEnable(DtBc*, Bool  Enable);
static void  DtBcIOPARIN_SetControlRegs(DtBcIOPARIN*, Bool BlkEna, Int OpMode);


// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIOPARIN - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOPARIN_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIOPARIN_Close(DtBc*  pBc)
{
    BC_IOPARIN_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOPARIN_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIOPARIN*  DtBcIOPARIN_Open(Int  Address, DtCore*  pCore, DtPt*  pPt,
                                                        const char*  pRole, Int  Instance,
                                                        Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_IOPARIN_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIOPARIN, Id, DT_BLOCK_TYPE_IOPARIN, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIOPARIN_Close;
    OpenParams.m_InitFunc = DtBcIOPARIN_Init; 
    OpenParams.m_OnEnableFunc = DtBcIOPARIN_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIOPARIN_OnCloseFile;

    // Use base function to allocate and perform standard initialization of block data
    return (DtBcIOPARIN*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOPARIN_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIOPARIN_GetOperationalMode(DtBcIOPARIN* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IOPARIN_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IOPARIN_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOPARIN_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIOPARIN_SetOperationalMode(DtBcIOPARIN* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_IOPARIN_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
                                                         && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IOPARIN_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    DtBcIOPARIN_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode);

    // Cache value
    pBc->m_OperationalMode = OpMode;
    return Status;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIOPARIN - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOPARIN_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIOPARIN_Init(DtBc* pBcBase)
{
    DtBcIOPARIN* pBc = (DtBcIOPARIN*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    // Set control register
    DtBcIOPARIN_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOPARIN_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIOPARIN_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIOPARIN* pBc = (DtBcIOPARIN*)pBcBase;

    // Sanity check
    BC_IOPARIN_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IOPARIN, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IOPARIN, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcIOPARIN_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IOPARIN, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIOPARIN_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOPARIN_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIOPARIN_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIOPARIN* pBc = (DtBcIOPARIN*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IOPARIN_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IOPARIN, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIOPARIN_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IOPARIN, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIOPARIN_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIOPARIN_SetControlRegs(DtBcIOPARIN* pBc, Bool BlkEna, Int OpMode)
{
    UInt32 RegData=0, FwBlkEna = 0, FwOpMode = 0, FwClkReset = 0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IOPARIN_BLKENA_ENABLED : IOPARIN_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode=IOPARIN_OPMODE_IDLE;    FwClkReset=1; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode=IOPARIN_OPMODE_STANDBY; FwClkReset=0; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode=IOPARIN_OPMODE_RUN;     FwClkReset=0; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control
    RegData = IOPARIN_Control_READ(pBc);
    RegData = IOPARIN_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IOPARIN_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = IOPARIN_Control_SET_DataInClkReset(RegData, FwClkReset);
    IOPARIN_Control_WRITE(pBc, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIOPARIN implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IOPARIN_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIOPARIN))
// Helper macro to cast stub's DtBc member to DtBcIOPARIN
#define IOPARIN_STUB   ((DtIoStubBcIOPARIN*)pStub)
#define IOPARIN_BC  ((DtBcIOPARIN*)IOPARIN_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIOPARIN_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcIOPARIN_OnCmdGetOpMode( const DtIoStubBcIOPARIN*,
                                                       DtIoctlIoParInCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcIOPARIN_OnCmdSetOpMode(const DtIoStubBcIOPARIN*,
                                                  const DtIoctlIoParInCmdSetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IOPARIN;

static const DtIoctlProperties  IOSTUB_BC_IOPARIN_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IOPARIN_CMD(
        DtIoStubBcIOPARIN_OnCmd,
        NULL, NULL),
};

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIOPARIN - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIOPARIN_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcIOPARIN_Close(DtIoStub* pStub)
{
    IOPARIN_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIOPARIN_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcIOPARIN*  DtIoStubBcIOPARIN_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IOPARIN_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIOPARIN, pBc, NULL,
        DtIoStubBcIOPARIN_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_IOPARIN_IOCTLS);
    return (DtIoStubBcIOPARIN*)DtIoStubBc_Open(&OpenParams);
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIOPARIN - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIOPARIN_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIOPARIN_OnCmd(const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIoParInCmdInput*  pInData = NULL;
    DtIoctlIoParInCmdOutput*  pOutData = NULL;

    IOPARIN_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IOPARIN_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IOPARIN, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IoParInCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IoParInCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IOPARIN_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIOPARIN_OnCmdGetOpMode(IOPARIN_STUB, &pOutData->m_GetOpMode);
        break;
    case DT_IOPARIN_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIOPARIN_OnCmdSetOpMode(IOPARIN_STUB, &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIOPARIN_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIOPARIN_OnCmdGetOpMode( const DtIoStubBcIOPARIN* pStub,
                                               DtIoctlIoParInCmdGetOpModeOutput* pOutData)
{
    IOPARIN_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIOPARIN_GetOperationalMode(IOPARIN_BC, &pOutData->m_OpMode);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIOPARIN_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIOPARIN_OnCmdSetOpMode( const DtIoStubBcIOPARIN* pStub,
                                           const DtIoctlIoParInCmdSetOpModeInput* pInData)
{
    IOPARIN_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIOPARIN_SetOperationalMode(IOPARIN_BC, pInData->m_OpMode);
}
