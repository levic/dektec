// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQPWR.c *#*#*#*#*#*#*#*#*#*#* (C) 2022 DekTec
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
#include "DtBcIQPWR.h"
#include "DtBcIQPWR_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQPWR implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQPWR_DEFAULT_PRECONDITIONS(pBc)                                              \
                                    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQPWR))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQPWR_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQPWR, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQPWR_Init(DtBc*);
static DtStatus  DtBcIQPWR_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIQPWR_OnEnable(DtBc*, Bool);
static void  DtBcIQPWR_SetControlRegs(DtBcIQPWR*, Bool, Int);

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQPWR - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcIQPWR_Close(DtBc* pBc)
{
    BC_IQPWR_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcIQPWR*  DtBcIQPWR_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_IQPWR_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQPWR, Id, DT_BLOCK_TYPE_IQPWR,
        Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQPWR_Close;
    OpenParams.m_InitFunc = DtBcIQPWR_Init;
    OpenParams.m_OnEnableFunc = DtBcIQPWR_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIQPWR_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQPWR*)DtBc_Open(&OpenParams);
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQPWR_GetOperationalMode(DtBcIQPWR* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IQPWR_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQPWR_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_GetSumOfSquares -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQPWR_GetSumOfSquares(DtBcIQPWR* pBc, UInt32* pSumOfSquares)
{
    // Sanity check
    BC_IQPWR_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pSumOfSquares == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQPWR_MUST_BE_ENABLED(pBc);

    *pSumOfSquares = IQPWR_SumOfSquares_READ(pBc);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_GetWindowSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQPWR_GetWindowSize(DtBcIQPWR* pBc, Int* pWindowSize)
{
    // Sanity check
    BC_IQPWR_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pWindowSize == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQPWR_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pWindowSize = pBc->m_WindowSize;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQPWR_SetOperationalMode(DtBcIQPWR* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_IQPWR_DEFAULT_PRECONDITIONS(pBc);


    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
        && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQPWR_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcIQPWR_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_SetInvert -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQPWR_SetWindowSize(DtBcIQPWR* pBc, Int WindowSize)
{
    // Sanity check
    BC_IQPWR_DEFAULT_PRECONDITIONS(pBc);

     // Must be enabled
    BC_IQPWR_MUST_BE_ENABLED(pBc);

    if (WindowSize<0x00 || WindowSize>0x1F)
        return DT_STATUS_INVALID_PARAMETER;

    // Cache value
    pBc->m_WindowSize = WindowSize;
    // Set register
    IQPWR_WindowSize_WRITE(pBc,(UInt32)pBc->m_WindowSize);
    return DT_STATUS_OK;
}


// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQPWR - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQPWR_Init(DtBc* pBcBase)
{
    DtBcIQPWR* pBc = (DtBcIQPWR*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_WindowSize = 8;

    // Set control register
    DtBcIQPWR_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);
    IQPWR_WindowSize_WRITE(pBc,(UInt32)pBc->m_WindowSize);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQPWR_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQPWR* pBc = (DtBcIQPWR*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IQPWR_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IQPWR, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIQPWR_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQPWR, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQPWR_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQPWR* pBc = (DtBcIQPWR*)pBcBase;

    // Sanity check
    BC_IQPWR_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IQPWR, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IQPWR, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcIQPWR_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQPWR, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIQPWR_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQPWR_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQPWR_SetControlRegs(DtBcIQPWR* pBc, Bool BlkEna, Int OpMode)
{
    UInt32  RegData = 0, FwBlkEna = 0, FwOpMode = 0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IQPWR_BLKENA_ENABLED : IQPWR_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = IQPWR_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = IQPWR_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = IQPWR_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = IQPWR_Control_READ(pBc);
    RegData = IQPWR_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IQPWR_Control_SET_OperationalMode(RegData, FwOpMode);
    IQPWR_Control_WRITE(pBc, RegData);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQPWR implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IQPWR_STUB_DEFAULT_PRECONDITIONS(pStub)                                          \
                          DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQPWR))
// Helper macro to cast stub's DtBc member to DtBcIQPWR
#define IQPWR_STUB   ((DtIoStubBcIQPWR*)pStub)
#define IQPWR_BC  ((DtBcIQPWR*)IQPWR_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQPWR_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);

static DtStatus  DtIoStubBcIQPWR_OnCmdGetOperationalMode(const DtIoStubBcIQPWR*,
                                                         DtIoctlIqPwrCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcIQPWR_OnCmdGetSumOfSquares(const DtIoStubBcIQPWR*,
                                                   DtIoctlIqPwrCmdGetSumOfSquaresOutput*);
static DtStatus  DtIoStubBcIQPWR_OnCmdGetWindowSize(const DtIoStubBcIQPWR*,
                                                     DtIoctlIqPwrCmdGetWindowSizeOutput*);
static DtStatus  DtIoStubBcIQPWR_OnCmdSetOperationalMode(const DtIoStubBcIQPWR*,
                                                    const DtIoctlIqPwrCmdSetOpModeInput*);
static DtStatus  DtIoStubBcIQPWR_OnCmdSetWindowSize(const DtIoStubBcIQPWR*,
                                                const DtIoctlIqPwrCmdSetWindowSizeInput*);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQPWR;

static const DtIoctlProperties  IOSTUB_BC_IQPWR_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQPWR_CMD(
        DtIoStubBcIQPWR_OnCmd,
        NULL, NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQPWR - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcIQPWR_Close(DtIoStub* pStub)
{
    IQPWR_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcIQPWR*  DtIoStubBcIQPWR_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IQPWR_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQPWR, pBc, NULL,
        DtIoStubBcIQPWR_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_IQPWR_IOCTLS);
    return (DtIoStubBcIQPWR*)DtIoStubBc_Open(&OpenParams);
}

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQPWR - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQPWR_OnCmd(const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqPwrCmdInput*  pInData = NULL;
    DtIoctlIqPwrCmdOutput*  pOutData = NULL;

    IQPWR_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQPWR_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQPWR, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqPwrCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqPwrCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IQPWR_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQPWR_OnCmdGetOperationalMode(IQPWR_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_IQPWR_CMD_GET_SUM_OF_SQUARES:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQPWR_OnCmdGetSumOfSquares(IQPWR_STUB,
                                                            &pOutData->m_GetSumOfSquares);
        break;
    case DT_IQPWR_CMD_GET_WINDOW_SIZE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQPWR_OnCmdGetWindowSize(IQPWR_STUB, 
                                                              &pOutData->m_GetWindowSize);
        break;
    case DT_IQPWR_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQPWR_OnCmdSetOperationalMode(IQPWR_STUB,
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_IQPWR_CMD_SET_WINDOW_SIZE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQPWR_OnCmdSetWindowSize(IQPWR_STUB,
                                                               &pInData->m_SetWindowSize);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQPWR_OnCmdGetOperationalMode(
    const DtIoStubBcIQPWR* pStub,
    DtIoctlIqPwrCmdGetOpModeOutput* pOutData)
{
    IQPWR_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQPWR_GetOperationalMode(IQPWR_BC, &pOutData->m_OpMode);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_OnCmdGetSumOfSquares -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQPWR_OnCmdGetSumOfSquares(const DtIoStubBcIQPWR* pStub,
                                           DtIoctlIqPwrCmdGetSumOfSquaresOutput* pOutData)
{
    IQPWR_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQPWR_GetSumOfSquares(IQPWR_BC, &pOutData->m_SumOfSquares);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_OnCmdGetWindowSize -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQPWR_OnCmdGetWindowSize(const DtIoStubBcIQPWR* pStub,
                                             DtIoctlIqPwrCmdGetWindowSizeOutput* pOutData)
{
    IQPWR_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQPWR_GetWindowSize(IQPWR_BC, &pOutData->m_WindowSize);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQPWR_OnCmdSetOperationalMode(
    const DtIoStubBcIQPWR* pStub,
    const DtIoctlIqPwrCmdSetOpModeInput* pInData)
{
    IQPWR_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQPWR_SetOperationalMode(IQPWR_BC, pInData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQPWR_OnCmdSetWindowSize -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQPWR_OnCmdSetWindowSize(const DtIoStubBcIQPWR* pStub,
                                         const DtIoctlIqPwrCmdSetWindowSizeInput* pInData)
{
    IQPWR_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQPWR_SetWindowSize(IQPWR_BC, pInData->m_WindowSize);
}

