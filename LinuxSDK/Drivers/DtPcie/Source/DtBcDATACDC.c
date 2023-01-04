//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcDATACDC.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
// This file is not part of the PCIe driver. It can be used as template to create a new
// block controller. Replace "DATACDC" by the building block's shortname (all uppercase)
// and replace "DataCdc" by the building block's shortname in camel case. 
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
#include "DtBcDATACDC.h"
#include "DtBcDATACDC_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDATACDC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_DATACDC_DEFAULT_PRECONDITIONS(pBc)      \
                                  DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcDATACDC))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_DATACDC_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(DATACDC, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcDATACDC_GetOperationalMode(DtBcDATACDC* pBc, Int* pOpMode);
static DtStatus  DtBcDATACDC_Init(DtBc*);
static DtStatus  DtBcDATACDC_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcDATACDC_OnEnable(DtBc*, Bool  Enable);
static void  DtBcDATACDC_SetControlRegs(DtBcDATACDC*, Bool BlkEna, Int OpMode);
static DtStatus  DtBcDATACDC_SetOperationalMode(DtBcDATACDC* pBc, Int OpMode);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDATACDC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATACDC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcDATACDC_Close(DtBc*  pBc)
{
    BC_DATACDC_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATACDC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcDATACDC*  DtBcDATACDC_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                              const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_DATACDC_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcDATACDC, Id, DT_BLOCK_TYPE_DATACDC,
                                                      Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcDATACDC_Close;
    OpenParams.m_InitFunc = DtBcDATACDC_Init;
    OpenParams.m_OnEnableFunc = DtBcDATACDC_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcDATACDC_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcDATACDC*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATACDC_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDATACDC_GetOperationalMode(DtBcDATACDC* pBc, Int* pOpMode)
{
    // Sanity check
    BC_DATACDC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_DATACDC_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATACDC_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDATACDC_SetOperationalMode(DtBcDATACDC* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_DATACDC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
        && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_DATACDC_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcDATACDC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDATACDC - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATACDC_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcDATACDC_Init(DtBc* pBcBase)
{
    DtBcDATACDC* pBc = (DtBcDATACDC*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    // Set control register
    DtBcDATACDC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATACDC_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcDATACDC_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcDATACDC* pBc = (DtBcDATACDC*)pBcBase;

    // Sanity check
    BC_DATACDC_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, DATACDC, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, DATACDC, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcDATACDC_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, DATACDC, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcDATACDC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATACDC_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcDATACDC_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcDATACDC* pBc = (DtBcDATACDC*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_DATACDC_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, DATACDC, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcDATACDC_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, DATACDC, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATACDC_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcDATACDC_SetControlRegs(DtBcDATACDC* pBc, Bool BlkEna, Int OpMode)
{
    UInt32  RegData = 0, FwBlkEna = 0, FwOpMode = 0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? DATACDC_BLKENA_ENABLED : DATACDC_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = DATACDC_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = DATACDC_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = DATACDC_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = DATACDC_Control_READ(pBc);
    RegData = DATACDC_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = DATACDC_Control_SET_OperationalMode(RegData, FwOpMode);
    DATACDC_Control_WRITE(pBc, RegData);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDATACDC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define DATACDC_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcDATACDC))
// Helper macro to cast stub's DtBc member to DtBcDATACDC
#define DATACDC_STUB   ((DtIoStubBcDATACDC*)pStub)
#define DATACDC_BC  ((DtBcDATACDC*)DATACDC_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcDATACDC_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcDATACDC_OnCmdGetOperationalMode(const DtIoStubBcDATACDC*,
                                                       DtIoctlDataCdcCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcDATACDC_OnCmdSetOperationalMode(const DtIoStubBcDATACDC*,
                                                  const DtIoctlDataCdcCmdSetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_DATACDC;

static const DtIoctlProperties  IOSTUB_BC_DATACDC_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_DATACDC_CMD(
        DtIoStubBcDATACDC_OnCmd,
        NULL, NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDATACDC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDATACDC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcDATACDC_Close(DtIoStub* pStub)
{
    DATACDC_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDATACDC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcDATACDC*  DtIoStubBcDATACDC_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_DATACDC_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcDATACDC, pBc, NULL,
        DtIoStubBcDATACDC_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_DATACDC_IOCTLS);
    return (DtIoStubBcDATACDC*)DtIoStubBc_Open(&OpenParams);
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDATACDC - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDATACDC_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcDATACDC_OnCmd(const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlDataCdcCmdInput*  pInData = NULL;
    DtIoctlDataCdcCmdOutput*  pOutData = NULL;

    DATACDC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_DATACDC_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, DATACDC, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_DataCdcCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_DataCdcCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_DATACDC_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcDATACDC_OnCmdGetOperationalMode(DATACDC_STUB,
            &pOutData->m_GetOpMode);
        break;
    case DT_DATACDC_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcDATACDC_OnCmdSetOperationalMode(DATACDC_STUB,
            &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDATACDC_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcDATACDC_OnCmdGetOperationalMode(
    const DtIoStubBcDATACDC* pStub,
    DtIoctlDataCdcCmdGetOpModeOutput* pOutData)
{
    DATACDC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcDATACDC_GetOperationalMode(DATACDC_BC, &pOutData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDATACDC_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcDATACDC_OnCmdSetOperationalMode(
    const DtIoStubBcDATACDC* pStub,
    const DtIoctlDataCdcCmdSetOpModeInput* pInData)
{
    DATACDC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcDATACDC_SetOperationalMode(DATACDC_BC, pInData->m_OpMode);
}
