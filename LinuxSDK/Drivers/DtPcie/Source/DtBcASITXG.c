// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcASITXG.c *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2017 DekTec Digital Video B.V.
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
#include "DtBcASITXG.h"
#include "DtBcASITXG_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcASITXG implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_ASITXG_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcASITXG))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_ASITXG_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(ASITXG, pBc)

// Helper macro to cast a DtBc* to a DtBcASITXG*
#define BC_ASITXG         ((DtBcASITXG*)pBc)

DtStatus DtBcASITXG_SetOperationalMode(DtBcASITXG * pBc, Int OpMode);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcASITXG_Init(DtBc*);
static DtStatus  DtBcASITXG_OnEnable(DtBc*, Bool);
static DtStatus  DtBcASITXG_OnCloseFile(DtBc*, const DtFileObject*);
static void  DtBcASITXG_SetControlRegs(DtBcASITXG*, Bool BlkEna, Int OpMode, Int Pol,
                                                                        Bool ClrInpState);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcASITXG - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXG_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcASITXG_Close(DtBc*  pBc)
{
    BC_ASITXG_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXG_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcASITXG*  DtBcASITXG_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_ASITXG_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcASITXG, Id, DT_BLOCK_TYPE_ASITXG, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcASITXG_Close;
    OpenParams.m_InitFunc = DtBcASITXG_Init;
    OpenParams.m_OnEnableFunc = DtBcASITXG_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcASITXG_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcASITXG*)DtBc_Open(&OpenParams);
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXG_ClearInputState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcASITXG_ClearInputState(DtBcASITXG* pBc)
{
    // Sanity check
    BC_ASITXG_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_ASITXG_MUST_BE_ENABLED(pBc);

    // Not allowed in RUN
    if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_IN_OPMODE;

    DtBcASITXG_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                pBc->m_AsiPolarity, TRUE);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXG_GetAsiPolarity -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcASITXG_GetAsiPolarity(DtBcASITXG* pBc, Int * pPolarity)
{
    // Sanity check
    BC_ASITXG_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pPolarity == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    
    // Must be enabled
    BC_ASITXG_MUST_BE_ENABLED(pBc);

    // Return last cached polarity
    *pPolarity = pBc->m_AsiPolarity;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXG_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcASITXG_GetOperationalMode(DtBcASITXG* pBc, Int* pOpMode)
{
    // Sanity check
    BC_ASITXG_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

   // Must be enabled
    BC_ASITXG_MUST_BE_ENABLED(pBc);

    // Return last cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXG_SetAsiPolarity -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcASITXG_SetAsiPolarity(DtBcASITXG* pBc, Int Polarity)
{
    // Sanity check
    BC_ASITXG_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (Polarity!=DT_ASITXG_POL_INVERT && Polarity!=DT_ASITXG_POL_NORMAL)
    { 
        DtDbgOutBc(ERR, ASITXG, pBc, "Invalid polarity");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_ASITXG_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_AsiPolarity == Polarity)
        return DT_STATUS_OK;

    // Save new setting
    pBc->m_AsiPolarity = Polarity;
    DtBcASITXG_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                               pBc->m_AsiPolarity, FALSE);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXG_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcASITXG_SetOperationalMode(DtBcASITXG* pBc, Int OpMode)
{
    // Sanity check
    BC_ASITXG_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, ASITXG, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_ASITXG_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Save new setting
    pBc->m_OperationalMode = OpMode;
    DtBcASITXG_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                               pBc->m_AsiPolarity, FALSE);
    return DT_STATUS_OK;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcASITXG - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXG_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcASITXG_Init(DtBc*  pBcBase)
{
    DtBcASITXG* pBc = (DtBcASITXG*)pBcBase;

    // Sanity checks
    BC_ASITXG_DEFAULT_PRECONDITIONS(pBc);

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_AsiPolarity = DT_ASITXG_POL_NORMAL;
    DtBcASITXG_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                               pBc->m_AsiPolarity, FALSE);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXG_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcASITXG_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcASITXG* pBc = (DtBcASITXG*)pBcBase;

    // Sanity check
    BC_ASITXG_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    { 
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, ASITXG, pBc, "Block Enable from disable -> enable");
    }
    else
    { 
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, ASITXG, pBc, "Block Enable from enable -> disable");
        Status = DtBcASITXG_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        DT_ASSERT(DT_SUCCESS(Status));
    }

    // Save new setting
    pBc->m_BlockEnabled = Enable;
    DtBcASITXG_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                               pBc->m_AsiPolarity, FALSE);

    return DT_STATUS_OK;
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXG_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcASITXG_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_ASITXG_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled(pBc))
    {
        DtDbgOutBc(AVG, ASITXG, pBc, "Set operational mode to IDLE");

        // Set operational mode to IDLE
        Status = DtBcASITXG_SetOperationalMode(BC_ASITXG, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, ASITXG, pBc, "ERROR: failed to set modes");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXG_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcASITXG_SetControlRegs(DtBcASITXG* pBc, Bool BlkEnable, Int OpMode, Int Polarity,
                                                                         Bool ClrInpState)
{
    UInt RegData, FwOpMode, FwBlkEnable, FwPolarity;

    // Sanity check
    BC_ASITXG_DEFAULT_PRECONDITIONS(pBc);

    // Convert block enable to BB-type
    FwBlkEnable = BlkEnable ? ASITXG_BLKENA_ENABLED : ASITXG_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = ASITXG_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = ASITXG_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = ASITXG_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE); return;
    }

    // Convert polarity to BB-type
    switch (Polarity)
    {
    case DT_ASITXG_POL_INVERT: FwPolarity = ASITXG_POL_INVERT; break;
    case DT_ASITXG_POL_NORMAL: FwPolarity = ASITXG_POL_NORMAL; break;
    default: DT_ASSERT(FALSE); return;
    }

    // Invalid combination?
    DT_ASSERT(BlkEnable || OpMode==DT_BLOCK_OPMODE_IDLE);

    // Update ASITXG control register
    RegData = ASITXG_Control_READ(pBc);
    RegData = ASITXG_Control_SET_BlockEnable(RegData, FwBlkEnable);
    RegData = ASITXG_Control_SET_ClearInputState(RegData, ClrInpState ? 1 : 0);
    RegData = ASITXG_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = ASITXG_Control_SET_AsiPolarity(RegData, FwPolarity);
    ASITXG_Control_WRITE(BC_ASITXG, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcASITXG implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcASITXG
#define ASITXG_STUB   ((DtIoStubBcASITXG*)pStub)
#define ASITXG_BC  ((DtBcASITXG*)ASITXG_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcASITXG_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcASITXG_OnCmdClearInputState(const DtIoStubBcASITXG*);
static DtStatus  DtIoStubBcASITXG_OnCmdGetAsiPolarity(const DtIoStubBcASITXG*, 
                                                   DtIoctlAsiTxGCmdGetAsiPolarityOutput*);
static DtStatus  DtIoStubBcASITXG_OnCmdGetOperationalMode(const DtIoStubBcASITXG*, 
                                                        DtIoctlAsiTxGCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcASITXG_OnCmdSetAsiPolarity(const DtIoStubBcASITXG*, 
                                              const DtIoctlAsiTxGCmdSetAsiPolarityInput*);
static DtStatus  DtIoStubBcASITXG_OnCmdSetOperationalMode(const DtIoStubBcASITXG*, 
                                                   const DtIoctlAsiTxGCmdSetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_ASITXG;

static const DtIoctlProperties  IOSTUB_BC_ASITXG_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_ASITXG_CMD(
        DtIoStubBcASITXG_OnCmd,
        NULL,  NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcASITXG - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcASITXG_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcASITXG_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcASITXG));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcASITXG_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcASITXG*  DtIoStubBcASITXG_Open(DtBc*  pBc)
{
    DtIoStubBcASITXG*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcASITXG));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcASITXG, pBc, NULL, 
                                                             DtIoStubBcASITXG_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_BC_ASITXG_IOCTLS);
    pStub = (DtIoStubBcASITXG*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcASITXG- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcASITXG_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcASITXG_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlAsiTxGCmdInput*  pInData = NULL;
    DtIoctlAsiTxGCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcASITXG));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_ASITXG_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, ASITXG, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_AsiTxGCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_AsiTxGCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_ASITXG_CMD_CLEAR_INPUT_STATE:
        Status = DtIoStubBcASITXG_OnCmdClearInputState(ASITXG_STUB);
        break;
    case DT_ASITXG_CMD_GET_ASI_POLARITY:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcASITXG_OnCmdGetAsiPolarity(ASITXG_STUB,
                                                             &pOutData->m_GetAsiPolarity);
        break;
    case DT_ASITXG_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcASITXG_OnCmdGetOperationalMode(ASITXG_STUB,  
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_ASITXG_CMD_SET_ASI_POLARITY:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcASITXG_OnCmdSetAsiPolarity(ASITXG_STUB,
                                                              &pInData->m_SetAsiPolarity);
        break;
    case DT_ASITXG_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcASITXG_OnCmdSetOperationalMode(ASITXG_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcASITXG_OnCmdClearInputState -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcASITXG_OnCmdClearInputState(
    const DtIoStubBcASITXG* pStub)
{
    return DtBcASITXG_ClearInputState(ASITXG_BC);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcASITXG_OnCmdGetAsiPolarity -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcASITXG_OnCmdGetAsiPolarity(
    const DtIoStubBcASITXG* pStub,
    DtIoctlAsiTxGCmdGetAsiPolarityOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcASITXG));
    DT_ASSERT(pOutData != NULL);

    return DtBcASITXG_GetAsiPolarity(ASITXG_BC, &pOutData->m_AsiPolarity);
}


//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcASITXG_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcASITXG_OnCmdGetOperationalMode(
    const DtIoStubBcASITXG* pStub,
    DtIoctlAsiTxGCmdGetOpModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcASITXG));
    DT_ASSERT(pOutData != NULL);

    return DtBcASITXG_GetOperationalMode(ASITXG_BC, &pOutData->m_OpMode);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcASITXG_OnCmdSetAsiPolarity -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcASITXG_OnCmdSetAsiPolarity(
    const DtIoStubBcASITXG* pStub,
    const DtIoctlAsiTxGCmdSetAsiPolarityInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcASITXG));
    DT_ASSERT(pInData != NULL);

    return DtBcASITXG_SetAsiPolarity(ASITXG_BC, pInData->m_AsiPolarity);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcASITXG_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcASITXG_OnCmdSetOperationalMode(
    const DtIoStubBcASITXG* pStub,
    const DtIoctlAsiTxGCmdSetOpModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcASITXG));
    DT_ASSERT(pInData != NULL);

    return DtBcASITXG_SetOperationalMode(ASITXG_BC, pInData->m_OpMode);
}