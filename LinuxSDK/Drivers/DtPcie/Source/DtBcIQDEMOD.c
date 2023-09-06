// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQDEMOD.c *#*#*#*#*#*#*#*#*#*# (C) 2022 DekTec
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
#include "DtBcIQDEMOD.h"
#include "DtBcIQDEMOD_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQDEMOD implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQDEMOD_DEFAULT_PRECONDITIONS(pBc)                                            \
                                  DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQDEMOD))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQDEMOD_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQDEMOD, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQDEMOD_Init(DtBc*);
static DtStatus  DtBcIQDEMOD_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIQDEMOD_OnEnable(DtBc*, Bool);
static void  DtBcIQDEMOD_SetControlRegs(DtBcIQDEMOD*, Bool, Int , Bool);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQDEMOD - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDEMOD_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQDEMOD_Close(DtBc* pBc)
{
    BC_IQDEMOD_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDEMOD_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIQDEMOD*  DtBcIQDEMOD_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_IQDEMOD_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQDEMOD, Id, DT_BLOCK_TYPE_IQDEMOD,
        Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQDEMOD_Close;
    OpenParams.m_InitFunc = DtBcIQDEMOD_Init;
    OpenParams.m_OnEnableFunc = DtBcIQDEMOD_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIQDEMOD_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQDEMOD*)DtBc_Open(&OpenParams);
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDEMOD_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQDEMOD_GetOperationalMode(DtBcIQDEMOD* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IQDEMOD_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQDEMOD_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDEMOD_GetNcoPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQDEMOD_GetNcoPars(DtBcIQDEMOD* pBc, Int* pBypass, Int* pNcoPhaseIncr)
{
    // Sanity check
    BC_IQDEMOD_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pBypass==NULL || pNcoPhaseIncr==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQDEMOD_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pBypass = pBc->m_Bypass;
    *pNcoPhaseIncr = pBc->m_NcoPhaseIncr;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDEMOD_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQDEMOD_SetOperationalMode(DtBcIQDEMOD* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_IQDEMOD_DEFAULT_PRECONDITIONS(pBc);


    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
        && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQDEMOD_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcIQDEMOD_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                           pBc->m_Bypass);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDEMOD_SetNcoPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQDEMOD_SetNcoPars(DtBcIQDEMOD* pBc, Int Bypass, Int NcoPhaseIncr)
{
    // Sanity check
    BC_IQDEMOD_DEFAULT_PRECONDITIONS(pBc);

     // Must be enabled
    BC_IQDEMOD_MUST_BE_ENABLED(pBc);

    // Parameter check
    if (Bypass!=FALSE &&  Bypass!=TRUE)
        return DT_STATUS_INVALID_PARAMETER;
    if (NcoPhaseIncr<0 || NcoPhaseIncr>0x3FFFF)
        return DT_STATUS_INVALID_PARAMETER;

    // Cache value
    pBc->m_Bypass = (Bool)Bypass;
    pBc->m_NcoPhaseIncr = NcoPhaseIncr;
    // Set registers
    IQDEMOD_NcoPhaseIncr_WRITE(pBc,(UInt32)pBc->m_NcoPhaseIncr);
    DtBcIQDEMOD_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                           pBc->m_Bypass);
    return DT_STATUS_OK;
}


// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQDEMOD - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDEMOD_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQDEMOD_Init(DtBc* pBcBase)
{
    DtBcIQDEMOD* pBc = (DtBcIQDEMOD*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_Bypass = FALSE;
    pBc->m_NcoPhaseIncr = 0;

    // Set control register
    IQDEMOD_NcoPhaseIncr_WRITE(pBc,(UInt32)pBc->m_NcoPhaseIncr);
    DtBcIQDEMOD_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                           pBc->m_Bypass);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDEMOD_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQDEMOD_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQDEMOD* pBc = (DtBcIQDEMOD*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IQDEMOD_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IQDEMOD, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIQDEMOD_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQDEMOD, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDEMOD_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQDEMOD_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQDEMOD* pBc = (DtBcIQDEMOD*)pBcBase;

    // Sanity check
    BC_IQDEMOD_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IQDEMOD, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IQDEMOD, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcIQDEMOD_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQDEMOD, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIQDEMOD_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                           pBc->m_Bypass);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDEMOD_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQDEMOD_SetControlRegs(DtBcIQDEMOD* pBc, Bool BlkEna, Int OpMode, Bool Bypass)
{
    UInt32  RegData = 0, FwBlkEna = 0, FwOpMode = 0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IQDEMOD_BLKENA_ENABLED : IQDEMOD_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = IQDEMOD_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = IQDEMOD_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = IQDEMOD_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = IQDEMOD_Control_READ(pBc);
    RegData = IQDEMOD_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IQDEMOD_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = IQDEMOD_Control_SET_Bypass(RegData, Bypass ? 1 : 0);
    IQDEMOD_Control_WRITE(pBc, RegData);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQDEMOD implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IQDEMOD_STUB_DEFAULT_PRECONDITIONS(pStub)                                        \
                        DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQDEMOD))
// Helper macro to cast stub's DtBc member to DtBcIQDEMOD
#define IQDEMOD_STUB   ((DtIoStubBcIQDEMOD*)pStub)
#define IQDEMOD_BC  ((DtBcIQDEMOD*)IQDEMOD_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQDEMOD_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);

static DtStatus  DtIoStubBcIQDEMOD_OnCmdGetNcoPars(const DtIoStubBcIQDEMOD*,
                                                     DtIoctlIqDemodCmdGetNcoParsOutput*);
static DtStatus  DtIoStubBcIQDEMOD_OnCmdGetOperationalMode(const DtIoStubBcIQDEMOD*,
                                                       DtIoctlIqDemodCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcIQDEMOD_OnCmdSetNcoPars(const DtIoStubBcIQDEMOD*,
                                                const DtIoctlIqDemodCmdSetNcoParsInput*);
static DtStatus  DtIoStubBcIQDEMOD_OnCmdSetOperationalMode(const DtIoStubBcIQDEMOD*,
                                                  const DtIoctlIqDemodCmdSetOpModeInput*);



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQDEMOD;

static const DtIoctlProperties  IOSTUB_BC_IQDEMOD_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQDEMOD_CMD(
        DtIoStubBcIQDEMOD_OnCmd,
        NULL, NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQDEMOD - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDEMOD_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcIQDEMOD_Close(DtIoStub* pStub)
{
    IQDEMOD_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDEMOD_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcIQDEMOD*  DtIoStubBcIQDEMOD_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IQDEMOD_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQDEMOD, pBc, NULL,
        DtIoStubBcIQDEMOD_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_IQDEMOD_IOCTLS);
    return (DtIoStubBcIQDEMOD*)DtIoStubBc_Open(&OpenParams);
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQDEMOD - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDEMOD_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQDEMOD_OnCmd(const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqDemodCmdInput*  pInData = NULL;
    DtIoctlIqDemodCmdOutput*  pOutData = NULL;

    IQDEMOD_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQDEMOD_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQDEMOD, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqDemodCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqDemodCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IQDEMOD_CMD_GET_NCO_PARS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQDEMOD_OnCmdGetNcoPars(IQDEMOD_STUB, &pOutData->m_GetNcoPars);
        break;
    case DT_IQDEMOD_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQDEMOD_OnCmdGetOperationalMode(IQDEMOD_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_IQDEMOD_CMD_SET_NCO_PARS:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQDEMOD_OnCmdSetNcoPars(IQDEMOD_STUB, &pInData->m_SetNcoPars);
        break;
    case DT_IQDEMOD_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQDEMOD_OnCmdSetOperationalMode(IQDEMOD_STUB,
                                                                   &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDEMOD_OnCmdGetNcoPars -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQDEMOD_OnCmdGetNcoPars(const DtIoStubBcIQDEMOD* pStub,
                                             DtIoctlIqDemodCmdGetNcoParsOutput* pOutData)
{
    IQDEMOD_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQDEMOD_GetNcoPars(IQDEMOD_BC, &pOutData->m_Bypass,
                                                               &pOutData->m_NcoPhaseIncr);
}


// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDEMOD_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQDEMOD_OnCmdGetOperationalMode(
    const DtIoStubBcIQDEMOD* pStub,
    DtIoctlIqDemodCmdGetOpModeOutput* pOutData)
{
    IQDEMOD_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQDEMOD_GetOperationalMode(IQDEMOD_BC, &pOutData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDEMOD_OnCmdSetNcoPars -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQDEMOD_OnCmdSetNcoPars(const DtIoStubBcIQDEMOD* pStub,
                                         const DtIoctlIqDemodCmdSetNcoParsInput* pInData)
{
    IQDEMOD_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQDEMOD_SetNcoPars(IQDEMOD_BC, pInData->m_Bypass, pInData->m_NcoPhaseIncr);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDEMOD_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQDEMOD_OnCmdSetOperationalMode(
    const DtIoStubBcIQDEMOD* pStub,
    const DtIoctlIqDemodCmdSetOpModeInput* pInData)
{
    IQDEMOD_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQDEMOD_SetOperationalMode(IQDEMOD_BC, pInData->m_OpMode);
}

