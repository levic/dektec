//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcASITXSER.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
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
#include "DtBcASITXSER.h"
#include "DtBcASITXSER_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcASITXSER implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_ASITXSER_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcASITXSER))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_ASITXSER_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(ASITXSER, pBc)

// Helper macro to cast a DtBc* to a DtBcASITXSER*
#define BC_ASITXSER         ((DtBcASITXSER*)pBc)

DtStatus DtBcASITXSER_SetOperationalMode(DtBcASITXSER * pBc, Int OpMode);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcASITXSER_Init(DtBc*);
static DtStatus  DtBcASITXSER_OnEnable(DtBc*, Bool);
static DtStatus  DtBcASITXSER_OnCloseFile(DtBc*, const DtFileObject*);
static void  DtBcASITXSER_SetControlRegs(DtBcASITXSER*, Bool BlkEna, Int OpMode);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcASITXSER - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXSER_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcASITXSER_Close(DtBc*  pBc)
{
    BC_ASITXSER_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXSER_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcASITXSER*  DtBcASITXSER_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_ASITXSER_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcASITXSER, Id, DT_BLOCK_TYPE_ASITXSER, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcASITXSER_Close;
    OpenParams.m_InitFunc = DtBcASITXSER_Init;
    OpenParams.m_OnEnableFunc = DtBcASITXSER_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcASITXSER_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcASITXSER*)DtBc_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXSER_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcASITXSER_GetOperationalMode(DtBcASITXSER* pBc, Int* pOpMode)
{
    // Sanity check
    BC_ASITXSER_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ASITXSER_MUST_BE_ENABLED(pBc);

    // Return last cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXSER_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcASITXSER_SetOperationalMode(DtBcASITXSER* pBc, Int OpMode)
{
    // Sanity check
    BC_ASITXSER_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, ASITXSER, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_ASITXSER_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Clear underflow when changing from IDLE->STANDBY or IDLE->RUN
    if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_IDLE)
        ASITXSER_CdcFifoStatus_CLEAR_Underflow(BC_ASITXSER);
    // Set control registers
    DtBcASITXSER_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode);

    // Save new setting
    pBc->m_OperationalMode = OpMode;

    return DT_STATUS_OK;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcASITXSER - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXSER_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcASITXSER_Init(DtBc*  pBc)
{
    // Sanity checks
    BC_ASITXSER_DEFAULT_PRECONDITIONS(pBc);

    // Set defaults
    BC_ASITXSER->m_BlockEnabled = FALSE;
    BC_ASITXSER->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    DtBcASITXSER_SetControlRegs(BC_ASITXSER, FALSE, DT_BLOCK_OPMODE_IDLE);

    // Clear underflow
    ASITXSER_CdcFifoStatus_CLEAR_Underflow(BC_ASITXSER);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXSER_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcASITXSER_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcASITXSER* pBc = (DtBcASITXSER*)pBcBase;

    // Sanity check
    BC_ASITXSER_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    { 
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, ASITXSER, pBc, "Block Enable from disable -> enable");
        DtBcASITXSER_SetControlRegs(pBc, Enable, pBc->m_OperationalMode);
    }
    else
    { 
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, ASITXSER, pBc, "Block Enable from enable -> disable");
        Status = DtBcASITXSER_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        DT_ASSERT(DT_SUCCESS(Status));
        DtBcASITXSER_SetControlRegs(pBc, Enable, pBc->m_OperationalMode);
    }

    // Save new setting
    pBc->m_BlockEnabled = Enable;

    return DT_STATUS_OK;
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXSER_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcASITXSER_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_ASITXSER_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled(pBc))
    {
        DtDbgOutBc(AVG, ASITXSER, pBc, "Set operational mode to IDLE");

        // Set operational mode to IDLE
        Status = DtBcASITXSER_SetOperationalMode(BC_ASITXSER, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, ASITXSER, pBc, "ERROR: failed to set modes");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcASITXSER_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcASITXSER_SetControlRegs(DtBcASITXSER* pBc, Bool BlkEnable, Int OpMode)
{
    UInt RegData, FwOpMode, FwBlkEnable;

    // Sanity check
    BC_ASITXSER_DEFAULT_PRECONDITIONS(pBc);

    // Convert block enable to BB-type
    FwBlkEnable = BlkEnable ? ASITXSER_BLKENA_ENABLED : ASITXSER_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = ASITXSER_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = ASITXSER_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = ASITXSER_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE); return;
    }


    // Invalid combination?
    DT_ASSERT(BlkEnable || OpMode==DT_BLOCK_OPMODE_IDLE);

    // Update ASITXSER control register
    RegData = ASITXSER_Control_READ(pBc);
    RegData = ASITXSER_Control_SET_BlockEnable(RegData, FwBlkEnable);
    RegData = ASITXSER_Control_SET_OperationalMode(RegData, FwOpMode);
    ASITXSER_Control_WRITE(BC_ASITXSER, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcASITXSER implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcASITXSER
#define ASITXSER_STUB   ((DtIoStubBcASITXSER*)pStub)
#define ASITXSER_BC  ((DtBcASITXSER*)ASITXSER_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcASITXSER_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcASITXSER_OnCmdGetOperationalMode(const DtIoStubBcASITXSER*, 
                                                      DtIoctlAsiTxSerCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcASITXSER_OnCmdSetOperationalMode(const DtIoStubBcASITXSER*, 
                                                 const DtIoctlAsiTxSerCmdSetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_ASITXSER;

static const DtIoctlProperties  IOSTUB_BC_ASITXSER_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_ASITXSER_CMD(
        DtIoStubBcASITXSER_OnCmd,
        NULL,  NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcASITXSER - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcASITXSER_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcASITXSER_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcASITXSER));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcASITXSER_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcASITXSER*  DtIoStubBcASITXSER_Open(DtBc*  pBc)
{
    DtIoStubBcASITXSER*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcASITXSER));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcASITXSER, pBc, NULL, 
                                                             DtIoStubBcASITXSER_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_BC_ASITXSER_IOCTLS);
    pStub = (DtIoStubBcASITXSER*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcASITXSER- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcASITXSER_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcASITXSER_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlAsiTxSerCmdInput*  pInData = NULL;
    DtIoctlAsiTxSerCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcASITXSER));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_ASITXSER_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, ASITXSER, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_AsiTxSerCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_AsiTxSerCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_ASITXSER_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcASITXSER_OnCmdGetOperationalMode(ASITXSER_STUB,  
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_ASITXSER_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcASITXSER_OnCmdSetOperationalMode(ASITXSER_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcASITXSER_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcASITXSER_OnCmdGetOperationalMode(
    const DtIoStubBcASITXSER* pStub,
    DtIoctlAsiTxSerCmdGetOpModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcASITXSER));
    DT_ASSERT(pOutData != NULL);

    return DtBcASITXSER_GetOperationalMode(ASITXSER_BC, &pOutData->m_OpMode);
}


//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcASITXSER_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcASITXSER_OnCmdSetOperationalMode(
    const DtIoStubBcASITXSER* pStub,
    const DtIoctlAsiTxSerCmdSetOpModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcASITXSER));
    DT_ASSERT(pInData != NULL);

    return DtBcASITXSER_SetOperationalMode(ASITXSER_BC, pInData->m_OpMode);
}
