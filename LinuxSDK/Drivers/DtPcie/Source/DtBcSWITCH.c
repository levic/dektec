// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSWITCH.c *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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
#include "DtBcSWITCH.h"
#include "DtBcSWITCH_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSWITCH implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_SWITCH_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSWITCH))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_SWITCH_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(SWITCH, pBc)

// Helper macro to cast a DtBc* to a DtBcSWITCH*
#define BC_SWITCH         ((DtBcSWITCH*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcSWITCH_Init(DtBc*);
static DtStatus  DtBcSWITCH_OnEnable(DtBc*, Bool);
static void  DtBcSWITCH_SetControlRegs(DtBcSWITCH*, Bool BlkEna, Int OpMode, Int InIdx, 
                                                                              Int OutIdx);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSWITCH - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSWITCH_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcSWITCH_Close(DtBc*  pBc)
{
    BC_SWITCH_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSWITCH_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcSWITCH*  DtBcSWITCH_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_SWITCH_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcSWITCH, Id, DT_BLOCK_TYPE_SWITCH, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcSWITCH_Close;
    OpenParams.m_InitFunc = DtBcSWITCH_Init;
    OpenParams.m_OnEnableFunc = DtBcSWITCH_OnEnable;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcSWITCH*)DtBc_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSWITCH_Clear -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSWITCH_Clear(DtBcSWITCH * pBc)
{
    // Sanity check
    BC_SWITCH_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SWITCH_MUST_BE_ENABLED(pBc);

    // In IDLE it is continuously cleared
    if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    {
        // Toggle between IDLE and RUN
        DtBcSWITCH_SetControlRegs(pBc, pBc->m_BlockEnabled, DT_BLOCK_OPMODE_IDLE,
                                                   pBc->m_InputIndex, pBc->m_OutputIndex);
        DtBcSWITCH_SetControlRegs(pBc, pBc->m_BlockEnabled, DT_BLOCK_OPMODE_RUN,
                                                   pBc->m_InputIndex, pBc->m_OutputIndex);
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSWITCH_GetConfiguration -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSWITCH_GetConfiguration(DtBcSWITCH* pBc, Int* pNumInputs, Int* pNumOutputs)
{
    // Sanity check
    BC_SWITCH_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pNumInputs==NULL || pNumOutputs==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return cached configuration
    *pNumInputs = pBc->m_NumInputPorts;
    *pNumOutputs = pBc->m_NumOutputPorts;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSWITCH_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSWITCH_GetOperationalMode(DtBcSWITCH* pBc, Int* pOpMode)
{
    // Sanity check
    BC_SWITCH_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SWITCH_MUST_BE_ENABLED(pBc);

    // Return last cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSWITCH_GetPosition -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSWITCH_GetPosition(DtBcSWITCH* pBc, Int* pInputIdx, Int* pOutputIdx)
{
    // Sanity check
    BC_SWITCH_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pInputIdx==NULL || pOutputIdx==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SWITCH_MUST_BE_ENABLED(pBc);

    // Return cached position
    *pInputIdx = pBc->m_InputIndex;
    *pOutputIdx = pBc->m_OutputIndex;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSWITCH_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSWITCH_SetOperationalMode(DtBcSWITCH* pBc, Int OpMode)
{
    // Sanity check
    BC_SWITCH_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SWITCH, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_SWITCH_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    DtBcSWITCH_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode, pBc->m_InputIndex,
                                                                      pBc->m_OutputIndex);
    // Save new setting
    pBc->m_OperationalMode = OpMode;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSWITCH_SetPosition -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSWITCH_SetPosition(DtBcSWITCH* pBc, Int InputIdx, Int OutputIdx)
{
    // Sanity check
    BC_SWITCH_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (InputIdx<0 || InputIdx>(pBc->m_NumInputPorts-1))
        return DT_STATUS_INVALID_PARAMETER;
    if (OutputIdx<0 || OutputIdx>(pBc->m_NumOutputPorts-1))
        return DT_STATUS_INVALID_PARAMETER;
    
    // Must be enabled
    BC_SWITCH_MUST_BE_ENABLED(pBc);

    // Only 1:M or N:1 
    DT_ASSERT(InputIdx==0 || OutputIdx==0);
    DT_ASSERT(pBc->m_NumInputPorts==1 || pBc->m_NumOutputPorts==1);

    DtBcSWITCH_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, InputIdx,
                                                                               OutputIdx);
    // Save cached position
    pBc->m_InputIndex = InputIdx;
    pBc->m_OutputIndex = OutputIdx;

    return DT_STATUS_OK;
}


// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSWITCH - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSWITCH_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSWITCH_Init(DtBc*  pBc)
{
    UInt32 RegData;

    // Sanity checks
    BC_SWITCH_DEFAULT_PRECONDITIONS(pBc);

    // Get configuration
    RegData = SWITCH_Config_READ(BC_SWITCH);
    BC_SWITCH->m_NumInputPorts = SWITCH_Config_GET_M(RegData);
    BC_SWITCH->m_NumOutputPorts = SWITCH_Config_GET_N(RegData);
    DT_ASSERT(BC_SWITCH->m_NumInputPorts==1 || BC_SWITCH->m_NumOutputPorts==1);

    // Set defaults
    BC_SWITCH->m_BlockEnabled = FALSE;
    BC_SWITCH->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    BC_SWITCH->m_InputIndex = 0;
    BC_SWITCH->m_OutputIndex = 0;
    DtBcSWITCH_SetControlRegs(BC_SWITCH, FALSE, DT_BLOCK_OPMODE_IDLE, 0, 0);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSWITCH_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSWITCH_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcSWITCH* pBc = (DtBcSWITCH*)pBcBase;

    // Sanity check
    BC_SWITCH_DEFAULT_PRECONDITIONS(pBc);


    if (Enable)
    { 
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, SWITCH, pBc, "Block Enable from disable -> enable");
        DtBcSWITCH_SetControlRegs(pBc, Enable, pBc->m_OperationalMode, pBc->m_InputIndex,
                                                                      pBc->m_OutputIndex);
    }
    else
    { 
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, SWITCH, pBc, "Block Enable from enable -> disable");
        Status = DtBcSWITCH_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        DT_ASSERT(DT_SUCCESS(Status));
        DtBcSWITCH_SetControlRegs(pBc, Enable, pBc->m_OperationalMode, pBc->m_InputIndex,
                                                                      pBc->m_OutputIndex);
    }

    // Save new setting
    pBc->m_BlockEnabled = Enable;

    return DT_STATUS_OK;
}
//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSWITCH_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void DtBcSWITCH_SetControlRegs(DtBcSWITCH* pBc, Bool BlkEnable, Int OpMode, 
                                                                    Int InIdx, Int OutIdx)
{
     UInt RegData, FwOpMode, FwBlkEnable, SwitchPos;
     //Sanity check
    BC_SWITCH_DEFAULT_PRECONDITIONS(pBc);

    // Only 1:M or N:1 
    // When the number of input ports is 1, SwitchPos determines which output port is 
    // connected to the input port. When the number of output ports is 1, SwitchPos 
    // determines which input port is connected to the output port.
    DT_ASSERT(InIdx==0 || OutIdx==0);
    SwitchPos =  InIdx==0 ? OutIdx : InIdx;

    // Convert block enable to BB-type
    FwBlkEnable = BlkEnable ? SWITCH_BLKENA_ENABLED : SWITCH_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = SWITCH_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = SWITCH_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE); return;
    }

    // Invalid combination?
    DT_ASSERT(BlkEnable || OpMode==DT_BLOCK_OPMODE_IDLE);

    // Update SWITCH control register
    RegData = SWITCH_Control_READ(pBc);
    RegData = SWITCH_Control_SET_BlockEnable(RegData, FwBlkEnable);
    RegData = SWITCH_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = SWITCH_Control_SET_SwitchPos(RegData, SwitchPos);
    SWITCH_Control_WRITE(BC_SWITCH, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSWITCH implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcSWITCH
#define SWITCH_STUB   ((DtIoStubBcSWITCH*)pStub)
#define SWITCH_BC  ((DtBcSWITCH*)SWITCH_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcSWITCH_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcSWITCH_OnCmdClear(const DtIoStubBcSWITCH*);
static DtStatus  DtIoStubBcSWITCH_OnCmdGetConfiguration(const DtIoStubBcSWITCH*, 
                                                 DtIoctlSwitchCmdGetConfigurationOutput*);
static DtStatus  DtIoStubBcSWITCH_OnCmdGetOperationalMode(const DtIoStubBcSWITCH*, 
                                                        DtIoctlSwitchCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcSWITCH_OnCmdGetPosition(const DtIoStubBcSWITCH*, 
                                                      DtIoctlSwitchCmdGetPositionOutput*);
static DtStatus  DtIoStubBcSWITCH_OnCmdSetOperationalMode(const DtIoStubBcSWITCH*, 
                                                   const DtIoctlSwitchCmdSetOpModeInput*);
static DtStatus  DtIoStubBcSWITCH_OnCmdSetPosition(const DtIoStubBcSWITCH*, 
                                                 const DtIoctlSwitchCmdSetPositionInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_SWITCH;

static const DtIoctlProperties  IOSTUB_BC_SWITCH_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_SWITCH_CMD(
        DtIoStubBcSWITCH_OnCmd,
        NULL,  NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSWITCH - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSWITCH_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcSWITCH_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSWITCH));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSWITCH_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcSWITCH*  DtIoStubBcSWITCH_Open(DtBc*  pBc)
{
    DtIoStubBcSWITCH*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSWITCH));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcSWITCH, pBc, NULL, 
                                                             DtIoStubBcSWITCH_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_BC_SWITCH_IOCTLS);
    pStub = (DtIoStubBcSWITCH*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSWITCH- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSWITCH_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSWITCH_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSwitchCmdInput*  pInData = NULL;
    DtIoctlSwitchCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSWITCH));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_SWITCH_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, SWITCH, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SwitchCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_SwitchCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_SWITCH_CMD_CLEAR:
        Status = DtIoStubBcSWITCH_OnCmdClear(SWITCH_STUB);
        break;
    case DT_SWITCH_CMD_GET_CONFIGURATION:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSWITCH_OnCmdGetConfiguration(SWITCH_STUB,
                                                           &pOutData->m_GetConfiguration);
        break;
    case DT_SWITCH_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSWITCH_OnCmdGetOperationalMode(SWITCH_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_SWITCH_CMD_GET_POSITION:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSWITCH_OnCmdGetPosition(SWITCH_STUB, &pOutData->m_GetPosition);
        break;
    case DT_SWITCH_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSWITCH_OnCmdSetOperationalMode(SWITCH_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_SWITCH_CMD_SET_POSITION:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSWITCH_OnCmdSetPosition(SWITCH_STUB, &pInData->m_SetPosition);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSWITCH_OnCmdClear -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSWITCH_OnCmdClear(
    const DtIoStubBcSWITCH* pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSWITCH));
    return DtBcSWITCH_Clear(SWITCH_BC);
}

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSWITCH_OnCmdGetConfiguration -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSWITCH_OnCmdGetConfiguration(
    const DtIoStubBcSWITCH* pStub,
    DtIoctlSwitchCmdGetConfigurationOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSWITCH));
    DT_ASSERT(pOutData != NULL);

    return DtBcSWITCH_GetConfiguration(SWITCH_BC, &pOutData->m_NumInputPorts,
                                                             &pOutData->m_NumOutputPorts);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSWITCH_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSWITCH_OnCmdGetOperationalMode(
    const DtIoStubBcSWITCH* pStub,
    DtIoctlSwitchCmdGetOpModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSWITCH));
    DT_ASSERT(pOutData != NULL);

    return DtBcSWITCH_GetOperationalMode(SWITCH_BC, &pOutData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSWITCH_OnCmdGetPosition -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSWITCH_OnCmdGetPosition(
    const DtIoStubBcSWITCH* pStub,
    DtIoctlSwitchCmdGetPositionOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSWITCH));
    DT_ASSERT(pOutData != NULL);

    return DtBcSWITCH_GetPosition(SWITCH_BC, &pOutData->m_InputIndex,
                                                                &pOutData->m_OutputIndex);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSWITCH_OnCmdSetPosition -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSWITCH_OnCmdSetOperationalMode(
    const DtIoStubBcSWITCH* pStub,
    const DtIoctlSwitchCmdSetOpModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSWITCH));
    DT_ASSERT(pInData != NULL);

    return DtBcSWITCH_SetOperationalMode(SWITCH_BC, pInData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSWITCH_OnCmdSetPosition -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSWITCH_OnCmdSetPosition(
    const DtIoStubBcSWITCH* pStub,
    const DtIoctlSwitchCmdSetPositionInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSWITCH));
    DT_ASSERT(pInData != NULL);

    return DtBcSWITCH_SetPosition(SWITCH_BC, pInData->m_InputIndex,
                                                                  pInData->m_OutputIndex);
}
