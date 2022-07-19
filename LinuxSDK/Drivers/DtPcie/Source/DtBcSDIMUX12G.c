// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDIMUX12G.c *#*#*#*#*#*#*#*#*#* (C) 2019 DekTec
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2019 DekTec Digital Video B.V.
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
#include "DtBcSDIMUX12G.h"
#include "DtBcSDIMUX12G_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIMUX12G implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_SDIMUX12G_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDIMUX12G))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_SDIMUX12G_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(SDIMUX12G, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcSDIMUX12G_Init(DtBc*);
static DtStatus DtBcSDIMUX12G_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcSDIMUX12G_OnEnable(DtBc*, Bool);
static void  DtBcSDIMUX12G_SetControlRegs(DtBcSDIMUX12G*, Bool BlkEna, Int OpMode);

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIMUX12G - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIMUX12G_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcSDIMUX12G_Close(DtBc*  pBc)
{
    BC_SDIMUX12G_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIMUX12G_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcSDIMUX12G*  DtBcSDIMUX12G_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_SDIMUX12G_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcSDIMUX12G, Id, DT_BLOCK_TYPE_SDIMUX12G,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcSDIMUX12G_Close;
    OpenParams.m_InitFunc = DtBcSDIMUX12G_Init;
    OpenParams.m_OnCloseFileFunc = DtBcSDIMUX12G_OnCloseFile;
    OpenParams.m_OnEnableFunc = DtBcSDIMUX12G_OnEnable;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcSDIMUX12G*)DtBc_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIMUX12G_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIMUX12G_GetOperationalMode(DtBcSDIMUX12G* pBc, Int* pOpMode)
{
    // Sanity check
    BC_SDIMUX12G_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIMUX12G_MUST_BE_ENABLED(pBc);

    // Return last cached operational mode
    *pOpMode = pBc->m_OperationalMode;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIMUX12G_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIMUX12G_SetOperationalMode(DtBcSDIMUX12G* pBc, Int OpMode)
{
    // Sanity check
    BC_SDIMUX12G_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDIMUX12G, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_SDIMUX12G_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Save new setting
    pBc->m_OperationalMode = OpMode;
    // Set control registers
    DtBcSDIMUX12G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);
    return DT_STATUS_OK;
}



// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIMUX12G - Private functions -.-.-.-.-.-.-.-.-.-.-.-.-.

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIMUX12G_Init +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//
DtStatus  DtBcSDIMUX12G_Init(DtBc*  pBcBase)
{
    DtBcSDIMUX12G* pBc = (DtBcSDIMUX12G*)pBcBase;

    // Sanity checks
    BC_SDIMUX12G_DEFAULT_PRECONDITIONS(pBc);

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    // Set control registers
    DtBcSDIMUX12G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIMUX12G_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSDIMUX12G_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcSDIMUX12G* pBc = (DtBcSDIMUX12G*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_SDIMUX12G_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, SDIMUX12G, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcSDIMUX12G_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SDIMUX12G, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}



// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIMUX12G_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIMUX12G_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcSDIMUX12G* pBc = (DtBcSDIMUX12G*)pBcBase;

    // Sanity check
    BC_SDIMUX12G_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    { 
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, SDIMUX12G, pBc, "Block Enable from disable -> enable");

        // Save new setting
        pBc->m_BlockEnabled = Enable;
        // Set control registers
        DtBcSDIMUX12G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);
    }
    else
    { 
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, SDIMUX12G, pBc, "Block Enable from enable -> disable");
        Status = DtBcSDIMUX12G_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        DT_ASSERT(DT_SUCCESS(Status));
        // Save new setting
        pBc->m_BlockEnabled = Enable;
        // Set control registers
        DtBcSDIMUX12G_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIMUX12G_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcSDIMUX12G_SetControlRegs(DtBcSDIMUX12G* pBc, Bool BlkEnable, Int OpMode)
{
     UInt RegData, FwOpMode, FwBlkEnable;
     //Sanity check
    BC_SDIMUX12G_DEFAULT_PRECONDITIONS(pBc);


    // Convert block enable to BB-type
    FwBlkEnable = BlkEnable ? SDIMUX12G_BLKENA_ENABLED : SDIMUX12G_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = SDIMUX12G_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = SDIMUX12G_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE); return;
    }
    // Invalid combination?
    DT_ASSERT(BlkEnable || OpMode==DT_BLOCK_OPMODE_IDLE);

    // Update SDIMUX12G control register
    RegData = SDIMUX12G_Control_READ(pBc);
    RegData = SDIMUX12G_Control_SET_BlockEnable(RegData, FwBlkEnable);
    RegData = SDIMUX12G_Control_SET_OperationalMode(RegData, FwOpMode);
    SDIMUX12G_Control_WRITE(pBc, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIMUX12G implementation +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define SDIMUX12G_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                      DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIMUX12G))
// Helper macro to cast stub's DtBc member to DtBcSDIMUX12G
#define SDIMUX12G_STUB   ((DtIoStubBcSDIMUX12G*)pStub)
#define SDIMUX12G_BC  ((DtBcSDIMUX12G*)SDIMUX12G_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcSDIMUX12G_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);

static DtStatus  DtIoStubBcSDIMUX12G_OnCmdGetOperationalMode(const DtIoStubBcSDIMUX12G*, 
                                                     DtIoctlSdiMux12GCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcSDIMUX12G_OnCmdSetOperationalMode(const DtIoStubBcSDIMUX12G*, 
                                                const DtIoctlSdiMux12GCmdSetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_SDIMUX12G;

static const DtIoctlProperties  IOSTUB_BC_SDIMUX12G_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_SDIMUX12G_CMD(
        DtIoStubBcSDIMUX12G_OnCmd,
        NULL,  NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIMUX12G - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIMUX12G_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcSDIMUX12G_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDIMUX12G));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIMUX12G_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcSDIMUX12G*  DtIoStubBcSDIMUX12G_Open(DtBc*  pBc)
{
    DtIoStubBcSDIMUX12G*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    BC_SDIMUX12G_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcSDIMUX12G, pBc, NULL, 
                                                             DtIoStubBcSDIMUX12G_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_BC_SDIMUX12G_IOCTLS);
    pStub = (DtIoStubBcSDIMUX12G*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIMUX12G- Private functions +=+=+=+=+=+=+=+=+=+=+=+=


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIMUX12G_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDIMUX12G_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSdiMux12GCmdInput*  pInData = NULL;
    DtIoctlSdiMux12GCmdOutput*  pOutData = NULL;

    SDIMUX12G_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_SDIMUX12G_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, SDIMUX12G, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SdiMux12GCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_SdiMux12GCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_SDIMUX12G_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDIMUX12G_OnCmdGetOperationalMode(SDIMUX12G_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_SDIMUX12G_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSDIMUX12G_OnCmdSetOperationalMode(SDIMUX12G_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIMUX12G_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIMUX12G_OnCmdGetOperationalMode(
    const DtIoStubBcSDIMUX12G* pStub,
    DtIoctlSdiMux12GCmdGetOpModeOutput* pOutData)
{
    SDIMUX12G_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcSDIMUX12G_GetOperationalMode(SDIMUX12G_BC, &pOutData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIMUX12G_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDIMUX12G_OnCmdSetOperationalMode(
    const DtIoStubBcSDIMUX12G* pStub,
    const DtIoctlSdiMux12GCmdSetOpModeInput* pInData)
{
    SDIMUX12G_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcSDIMUX12G_SetOperationalMode(SDIMUX12G_BC, pInData->m_OpMode);
}
