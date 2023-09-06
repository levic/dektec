// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQDOWNSRC.c *#*#*#*#*#*#*#*#*#* (C) 2022 DekTec
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
#include "DtBcIQDOWNSRC.h"
#include "DtBcIQDOWNSRC_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQDOWNSRC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQDOWNSRC_DEFAULT_PRECONDITIONS(pBc)                                          \
                                DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQDOWNSRC))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQDOWNSRC_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQDOWNSRC, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQDOWNSRC_Init(DtBc*);
static DtStatus  DtBcIQDOWNSRC_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIQDOWNSRC_OnEnable(DtBc*, Bool);
static void  DtBcIQDOWNSRC_SetControlRegs(DtBcIQDOWNSRC*, Bool, Int , Bool);

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQDOWNSRC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDOWNSRC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcIQDOWNSRC_Close(DtBc* pBc)
{
    BC_IQDOWNSRC_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDOWNSRC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcIQDOWNSRC*  DtBcIQDOWNSRC_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_IQDOWNSRC_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQDOWNSRC, Id, DT_BLOCK_TYPE_IQDOWNSRC,
        Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQDOWNSRC_Close;
    OpenParams.m_InitFunc = DtBcIQDOWNSRC_Init;
    OpenParams.m_OnEnableFunc = DtBcIQDOWNSRC_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIQDOWNSRC_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQDOWNSRC*)DtBc_Open(&OpenParams);
}


// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDOWNSRC_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQDOWNSRC_GetOperationalMode(DtBcIQDOWNSRC* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IQDOWNSRC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQDOWNSRC_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDOWNSRC_GetConvRatePars -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQDOWNSRC_GetConvRatePars(DtBcIQDOWNSRC* pBc, Int* pBypass, UInt32* pSrRatio)
{
    // Sanity check
    BC_IQDOWNSRC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pBypass==NULL || pSrRatio==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQDOWNSRC_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pBypass = pBc->m_Bypass;
    *pSrRatio = pBc->m_SampleRateRatio;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDOWNSRC_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQDOWNSRC_SetOperationalMode(DtBcIQDOWNSRC* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_IQDOWNSRC_DEFAULT_PRECONDITIONS(pBc);


    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
        && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQDOWNSRC_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcIQDOWNSRC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                           pBc->m_Bypass);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDOWNSRC_SetConvRatePars -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQDOWNSRC_SetConvRatePars(DtBcIQDOWNSRC* pBc, Int Bypass, UInt32 SrRatio)
{
    // Sanity check
    BC_IQDOWNSRC_DEFAULT_PRECONDITIONS(pBc);

     // Must be enabled
    BC_IQDOWNSRC_MUST_BE_ENABLED(pBc);

    // Parameter check
    if (Bypass!=FALSE &&  Bypass!=TRUE)
        return DT_STATUS_INVALID_PARAMETER;

    // Cache value
    pBc->m_Bypass = (Bool)Bypass;
    pBc->m_SampleRateRatio = SrRatio;
    // Set registers
    IQDOWNSRC_Ratio_WRITE(pBc,(UInt32)pBc->m_SampleRateRatio);
    DtBcIQDOWNSRC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                           pBc->m_Bypass);
    return DT_STATUS_OK;
}


// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQDOWNSRC - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDOWNSRC_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQDOWNSRC_Init(DtBc* pBcBase)
{
    DtBcIQDOWNSRC* pBc = (DtBcIQDOWNSRC*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_Bypass = FALSE;
    pBc->m_SampleRateRatio = 0;

    // Set control register
    IQDOWNSRC_Ratio_WRITE(pBc,(UInt32)pBc->m_SampleRateRatio);
    DtBcIQDOWNSRC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                           pBc->m_Bypass);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDOWNSRC_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQDOWNSRC_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQDOWNSRC* pBc = (DtBcIQDOWNSRC*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IQDOWNSRC_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IQDOWNSRC, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIQDOWNSRC_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQDOWNSRC, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDOWNSRC_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQDOWNSRC_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQDOWNSRC* pBc = (DtBcIQDOWNSRC*)pBcBase;

    // Sanity check
    BC_IQDOWNSRC_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IQDOWNSRC, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IQDOWNSRC, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcIQDOWNSRC_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQDOWNSRC, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIQDOWNSRC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                           pBc->m_Bypass);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQDOWNSRC_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcIQDOWNSRC_SetControlRegs(DtBcIQDOWNSRC* pBc, Bool BlkEna, Int OpMode,
                                                                              Bool Bypass)
{
    UInt32  RegData = 0, FwBlkEna = 0, FwOpMode = 0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IQDOWNSRC_BLKENA_ENABLED : IQDOWNSRC_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = IQDOWNSRC_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = IQDOWNSRC_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = IQDOWNSRC_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = IQDOWNSRC_Control_READ(pBc);
    RegData = IQDOWNSRC_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IQDOWNSRC_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = IQDOWNSRC_Control_SET_Bypass(RegData, Bypass ? 1 : 0);
    IQDOWNSRC_Control_WRITE(pBc, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQDOWNSRC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IQDOWNSRC_STUB_DEFAULT_PRECONDITIONS(pStub)                                      \
                      DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQDOWNSRC))
// Helper macro to cast stub's DtBc member to DtBcIQDOWNSRC
#define IQDOWNSRC_STUB   ((DtIoStubBcIQDOWNSRC*)pStub)
#define IQDOWNSRC_BC  ((DtBcIQDOWNSRC*)IQDOWNSRC_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQDOWNSRC_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);

static DtStatus  DtIoStubBcIQDOWNSRC_OnCmdGetConvRatePars(const DtIoStubBcIQDOWNSRC*,
                                              DtIoctlIqDownSrcCmdGetConvRateParsOutput*);
static DtStatus  DtIoStubBcIQDOWNSRC_OnCmdGetOperationalMode(const DtIoStubBcIQDOWNSRC*,
                                                     DtIoctlIqDownSrcCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcIQDOWNSRC_OnCmdSetConvRatePars(const DtIoStubBcIQDOWNSRC*,
                                          const DtIoctlIqDownSrcCmdSetConvRateParsInput*);
static DtStatus  DtIoStubBcIQDOWNSRC_OnCmdSetOperationalMode(const DtIoStubBcIQDOWNSRC*,
                                                const DtIoctlIqDownSrcCmdSetOpModeInput*);



//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQDOWNSRC;

static const DtIoctlProperties  IOSTUB_BC_IQDOWNSRC_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQDOWNSRC_CMD(
        DtIoStubBcIQDOWNSRC_OnCmd,
        NULL, NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQDOWNSRC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDOWNSRC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcIQDOWNSRC_Close(DtIoStub* pStub)
{
    IQDOWNSRC_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDOWNSRC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcIQDOWNSRC*  DtIoStubBcIQDOWNSRC_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IQDOWNSRC_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQDOWNSRC, pBc, NULL,
        DtIoStubBcIQDOWNSRC_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_IQDOWNSRC_IOCTLS);
    return (DtIoStubBcIQDOWNSRC*)DtIoStubBc_Open(&OpenParams);
}

// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQDOWNSRC - Private functions +=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDOWNSRC_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQDOWNSRC_OnCmd(const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqDownSrcCmdInput*  pInData = NULL;
    DtIoctlIqDownSrcCmdOutput*  pOutData = NULL;

    IQDOWNSRC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQDOWNSRC_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQDOWNSRC, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqDownSrcCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqDownSrcCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IQDOWNSRC_CMD_GET_CONV_RATE_PARS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQDOWNSRC_OnCmdGetConvRatePars(IQDOWNSRC_STUB,
                                                            &pOutData->m_GetConvRatePars);
        break;
    case DT_IQDOWNSRC_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQDOWNSRC_OnCmdGetOperationalMode(IQDOWNSRC_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_IQDOWNSRC_CMD_SET_CONV_RATE_PARS:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQDOWNSRC_OnCmdSetConvRatePars(IQDOWNSRC_STUB,
                                                             &pInData->m_SetConvRatePars);
        break;
    case DT_IQDOWNSRC_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQDOWNSRC_OnCmdSetOperationalMode(IQDOWNSRC_STUB,
                                                                   &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDOWNSRC_OnCmdGetConvRatePars -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQDOWNSRC_OnCmdGetConvRatePars(const DtIoStubBcIQDOWNSRC* pStub,
                                     DtIoctlIqDownSrcCmdGetConvRateParsOutput* pOutData)
{
    IQDOWNSRC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQDOWNSRC_GetConvRatePars(IQDOWNSRC_BC, &pOutData->m_Bypass,
                                                            &pOutData->m_SampleRateRatio);
}


// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDOWNSRC_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQDOWNSRC_OnCmdGetOperationalMode(
    const DtIoStubBcIQDOWNSRC* pStub,
    DtIoctlIqDownSrcCmdGetOpModeOutput* pOutData)
{
    IQDOWNSRC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQDOWNSRC_GetOperationalMode(IQDOWNSRC_BC, &pOutData->m_OpMode);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDOWNSRC_OnCmdSetConvRatePars -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQDOWNSRC_OnCmdSetConvRatePars(const DtIoStubBcIQDOWNSRC* pStub,
                                 const DtIoctlIqDownSrcCmdSetConvRateParsInput* pInData)
{
    IQDOWNSRC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQDOWNSRC_SetConvRatePars(IQDOWNSRC_BC, pInData->m_Bypass,
                                                              pInData->m_SampleRateRatio);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQDOWNSRC_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQDOWNSRC_OnCmdSetOperationalMode(
    const DtIoStubBcIQDOWNSRC* pStub,
    const DtIoctlIqDownSrcCmdSetOpModeInput* pInData)
{
    IQDOWNSRC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQDOWNSRC_SetOperationalMode(IQDOWNSRC_BC, pInData->m_OpMode);
}

