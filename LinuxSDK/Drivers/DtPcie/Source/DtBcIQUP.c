//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQUP.c *#*#*#*#*#*#*#*#*#*# (C) 2021 DekTec
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
#include "DtBcIQUP.h"
#include "DtBcIQUP_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQUP implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQUP_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQUP))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQUP_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQUP, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQUP_GetNcoRatio(DtBcIQUP*, UInt32*, UInt32*, UInt32*);
static DtStatus  DtBcIQUP_GetNcoOffset(DtBcIQUP*, UInt32*);
static DtStatus  DtBcIQUP_Init(DtBc*);
static DtStatus  DtBcIQUP_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIQUP_OnEnable(DtBc*, Bool);
static void  DtBcIQUP_SetControlRegs(DtBcIQUP*, Bool, Int );
static DtStatus  DtBcIQUP_SetNcoRatio(DtBcIQUP*, UInt32, UInt32, UInt32);
static DtStatus  DtBcIQUP_SetNcoOffset(DtBcIQUP*, UInt32);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQUP - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUP_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQUP_Close(DtBc* pBc)
{
    BC_IQUP_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUP_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIQUP*  DtBcIQUP_Open(Int  Address, DtCore* pCore, DtPt* pPt,
    const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_IQUP_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQUP, Id, DT_BLOCK_TYPE_IQUP,
        Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQUP_Close;
    OpenParams.m_InitFunc = DtBcIQUP_Init;
    OpenParams.m_OnEnableFunc = DtBcIQUP_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIQUP_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQUP*)DtBc_Open(&OpenParams);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQUP - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUP_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQUP_Init(DtBc* pBcBase)
{
    DtBcIQUP* pBc = (DtBcIQUP*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    // Set defaults
    pBc->m_NcoOffset = 1;
    IQUP_NcoOffset_WRITE(pBc, pBc->m_NcoOffset);

    pBc->m_NcoRatioDenominator = 1;
    pBc->m_NcoRatioNumerator= 0;
    pBc->m_NcoRatioInteger= 0x80000000;
    IQUP_NcoRatioInteger_WRITE(pBc, pBc->m_NcoRatioInteger);
    IQUP_NcoRatioNumerator_WRITE(pBc, pBc->m_NcoRatioNumerator);
    IQUP_NcoRatioDenominator_WRITE(pBc, pBc->m_NcoRatioDenominator);

    // Set control register
    DtBcIQUP_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUP_GetNcoRatio -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQUP_GetNcoRatio(DtBcIQUP* pBc, UInt32* pNcoRatioInteger, 
                                 UInt32* pNcoRatioNumerator, UInt32* pNcoRatioDenominator)
{
    // Sanity check
    BC_IQUP_DEFAULT_PRECONDITIONS(pBc);

    DT_ASSERT(pNcoRatioInteger!=NULL && pNcoRatioNumerator!=NULL && 
                                                              pNcoRatioDenominator!=NULL);

    // Must be enabled
    BC_IQUP_MUST_BE_ENABLED(pBc);

    *pNcoRatioInteger = pBc->m_NcoRatioInteger;
    *pNcoRatioNumerator = pBc->m_NcoRatioNumerator;
    *pNcoRatioDenominator = pBc->m_NcoRatioDenominator;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUP_GetNcoOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQUP_GetNcoOffset(DtBcIQUP* pBc, UInt32* pNcoOffset)
{
    BC_IQUP_DEFAULT_PRECONDITIONS(pBc);

    DT_ASSERT(pNcoOffset!=NULL);

    // Must be enabled
    BC_IQUP_MUST_BE_ENABLED(pBc);

    *pNcoOffset = pBc->m_NcoOffset;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUP_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQUP_GetOperationalMode(DtBcIQUP* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IQUP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQUP_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUP_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQUP_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQUP* pBc = (DtBcIQUP*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IQUP_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IQUP, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIQUP_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQUP, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUP_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQUP_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQUP* pBc = (DtBcIQUP*)pBcBase;

    // Sanity check
    BC_IQUP_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IQUP, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IQUP, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcIQUP_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQUP, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIQUP_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUP_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQUP_SetControlRegs(DtBcIQUP* pBc, Bool BlkEna, Int OpMode)
{
    UInt32  RegData = 0, FwBlkEna = 0, FwOpMode = 0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IQUP_BLKENA_ENABLED : IQUP_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = IQUP_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = IQUP_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = IQUP_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = IQUP_Control_READ(pBc);
    RegData = IQUP_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IQUP_Control_SET_OperationalMode(RegData, FwOpMode);
    IQUP_Control_WRITE(pBc, RegData);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUP_SetNcoRatio -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQUP_SetNcoRatio(DtBcIQUP* pBc, UInt32 NcoRatioInteger, 
                                     UInt32 NcoRatioNumerator, UInt32 NcoRatioDenominator)
{
    // Sanity check
    BC_IQUP_DEFAULT_PRECONDITIONS(pBc);

     // Must be enabled
    BC_IQUP_MUST_BE_ENABLED(pBc);

    pBc->m_NcoRatioInteger = NcoRatioInteger;
    pBc->m_NcoRatioNumerator = NcoRatioNumerator;
    pBc->m_NcoRatioDenominator = NcoRatioDenominator;

    // NOTE: Integer must be written as last register!
    IQUP_NcoRatioNumerator_WRITE(pBc, NcoRatioNumerator);
    IQUP_NcoRatioDenominator_WRITE(pBc, NcoRatioDenominator);
    IQUP_NcoRatioInteger_WRITE(pBc, NcoRatioInteger);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUP_SetNcoOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQUP_SetNcoOffset(DtBcIQUP* pBc, UInt32 NcoOffset)
{
    // Sanity check
    BC_IQUP_DEFAULT_PRECONDITIONS(pBc);

     // Must be enabled
    BC_IQUP_MUST_BE_ENABLED(pBc);

    pBc->m_NcoOffset = NcoOffset;

    IQUP_NcoOffset_WRITE(pBc, NcoOffset);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUP_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQUP_SetOperationalMode(DtBcIQUP* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_IQUP_DEFAULT_PRECONDITIONS(pBc);


    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
        && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQUP_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcIQUP_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQUP implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IQUP_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQUP))
// Helper macro to cast stub's DtBc member to DtBcIQUP
#define IQUP_STUB   ((DtIoStubBcIQUP*)pStub)
#define IQUP_BC  ((DtBcIQUP*)IQUP_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQUP_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcIQUP_OnCmdGetNcoRatio(const DtIoStubBcIQUP*,
                                                        DtIoctlIqUpCmdGetNcoRatioOutput*);
static DtStatus  DtIoStubBcIQUP_OnCmdGetNcoOffset(const DtIoStubBcIQUP*,
                                                       DtIoctlIqUpCmdGetNcoOffsetOutput*);
static DtStatus  DtIoStubBcIQUP_OnCmdGetOperationalMode(const DtIoStubBcIQUP*,
                                                          DtIoctlIqUpCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcIQUP_OnCmdSetOperationalMode(const DtIoStubBcIQUP*,
                                                     const DtIoctlIqUpCmdSetOpModeInput*);
static DtStatus  DtIoStubBcIQUP_OnCmdSetNcoRatio(const DtIoStubBcIQUP*,
                                                   const DtIoctlIqUpCmdSetNcoRatioInput*);
static DtStatus  DtIoStubBcIQUP_OnCmdSetNcoOffset(const DtIoStubBcIQUP*,
                                                  const DtIoctlIqUpCmdSetNcoOffsetInput*);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQUP;

static const DtIoctlProperties  IOSTUB_BC_IQUP_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQUP_CMD(
        DtIoStubBcIQUP_OnCmd,
        NULL, NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQUP - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUP_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcIQUP_Close(DtIoStub* pStub)
{
    IQUP_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUP_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcIQUP*  DtIoStubBcIQUP_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IQUP_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQUP, pBc, NULL,
        DtIoStubBcIQUP_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_IQUP_IOCTLS);
    return (DtIoStubBcIQUP*)DtIoStubBc_Open(&OpenParams);
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQUP - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUP_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQUP_OnCmd(const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqUpCmdInput*  pInData = NULL;
    DtIoctlIqUpCmdOutput*  pOutData = NULL;

    IQUP_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQUP_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQUP, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqUpCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqUpCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IQUP_CMD_GET_NCO_OFFSET:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQUP_OnCmdGetNcoOffset(IQUP_STUB, &pOutData->m_NcoOffset);
        break;
    case DT_IQUP_CMD_GET_NCO_RATIO:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQUP_OnCmdGetNcoRatio(IQUP_STUB, &pOutData->m_NcoRatio);
        break;
    case DT_IQUP_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
                                Status = DtIoStubBcIQUP_OnCmdGetOperationalMode(IQUP_STUB,
                                &pOutData->m_GetOpMode);
        break;
    case DT_IQUP_CMD_SET_NCO_OFFSET:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQUP_OnCmdSetNcoOffset(IQUP_STUB, &pInData->m_NcoOffset);
        break;
    case DT_IQUP_CMD_SET_NCO_RATIO:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQUP_OnCmdSetNcoRatio(IQUP_STUB, &pInData->m_NcoRatio);
        break;
    case DT_IQUP_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQUP_OnCmdSetOperationalMode(IQUP_STUB,
            &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUP_OnCmdGetNcoRatio -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQUP_OnCmdGetNcoRatio(const DtIoStubBcIQUP* pStub,
                                                DtIoctlIqUpCmdGetNcoRatioOutput* pOutData)
{
    IQUP_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQUP_GetNcoRatio(IQUP_BC, &pOutData->m_NcoRatioInteger, 
                                                        &pOutData->m_NcoRatioNumerator,
                                                        &pOutData->m_NcoRatioDenominator);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUP_OnCmdGetNcoOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQUP_OnCmdGetNcoOffset(const DtIoStubBcIQUP* pStub,
                                               DtIoctlIqUpCmdGetNcoOffsetOutput* pOutData)
{
    IQUP_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQUP_GetNcoOffset(IQUP_BC, &pOutData->m_NcoOffset);
}


//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUP_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQUP_OnCmdGetOperationalMode(
    const DtIoStubBcIQUP* pStub,
    DtIoctlIqUpCmdGetOpModeOutput* pOutData)
{
    IQUP_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQUP_GetOperationalMode(IQUP_BC, &pOutData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUP_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQUP_OnCmdSetOperationalMode(
    const DtIoStubBcIQUP* pStub,
    const DtIoctlIqUpCmdSetOpModeInput* pInData)
{
    IQUP_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQUP_SetOperationalMode(IQUP_BC, pInData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUP_OnCmdSetNcoRatio -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQUP_OnCmdSetNcoRatio(const DtIoStubBcIQUP* pStub,
                                            const DtIoctlIqUpCmdSetNcoRatioInput* pInData)
{
    IQUP_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQUP_SetNcoRatio(IQUP_BC, pInData->m_NcoRatioInteger, 
                                                          pInData->m_NcoRatioNumerator, 
                                                          pInData->m_NcoRatioDenominator);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUP_OnCmdSetNcoOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQUP_OnCmdSetNcoOffset(const DtIoStubBcIQUP* pStub,
                                           const DtIoctlIqUpCmdSetNcoOffsetInput* pInData)
{
    IQUP_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQUP_SetNcoOffset(IQUP_BC, pInData->m_NcoOffset);
}