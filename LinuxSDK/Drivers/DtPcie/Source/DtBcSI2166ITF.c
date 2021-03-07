// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSI2166ITF.c *#*#*#*#*#*#*#*#*#* (C) 2020 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2020 DekTec Digital Video B.V.
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
#include "DtBcSI2166ITF.h"
#include "DtBcSI2166ITF_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSI2166ITF implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Helper macro to cast a DtBc* to a DtBcSI2166ITF*
#define  BC_SI2166ITF         ((DtBcSI2166ITF*)pBc)

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_SI2166ITF_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(SI2166ITF, pBc)

// Macro with default precondition checks for the BcSI2166ITF block-controller
#define BC_SI2166ITF_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSI2166ITF))

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcSI2166ITF_Init(DtBc*);
static DtStatus  DtBcSI2166ITF_OnEnable(DtBc * pBc, Bool Enable);
static DtStatus  DtBcSI2166ITF_OnCloseFile(DtBc*, const DtFileObject*);
static void  DtBcSI2166ITF_SetControlRegs(DtBcSI2166ITF*, Bool BlkEna, Int OpMode, 
                                                            Int Si2166Mode, Int DataType);

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSI2166ITF - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSI2166ITF_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcSI2166ITF_Close(DtBc*  pBc)
{
    BC_SI2166ITF_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSI2166ITF_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcSI2166ITF*  DtBcSI2166ITF_Open(Int  Address, DtCore*  pCore, DtPt*  pPt,
                                                        const char*  pRole, Int  Instance,
                                                        Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_SI2166ITF_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcSI2166ITF, Id, DT_BLOCK_TYPE_SI2166ITF, 
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcSI2166ITF_Close;
    OpenParams.m_InitFunc = DtBcSI2166ITF_Init;
    OpenParams.m_OnEnableFunc = DtBcSI2166ITF_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcSI2166ITF_OnCloseFile;

    // Use base function to allocate and perform standard initialization of block data
    return (DtBcSI2166ITF*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSI2166ITF_GetConfiguration -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSI2166ITF_GetConfiguration(
                                     DtBcSI2166ITF* pBc, Int* pSi2166Mode, Int* pDataType)
{
    // Sanity check
    BC_SI2166ITF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pSi2166Mode==NULL || pDataType==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return configuration settings, such as Si2166 mode
    *pSi2166Mode = pBc->m_Si2166Mode;
    *pDataType = pBc->m_DataType;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSI2166ITF_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSI2166ITF_GetOperationalMode(DtBcSI2166ITF* pBc, Int* pOpMode)
{
    // Sanity check
    BC_SI2166ITF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SI2166ITF_MUST_BE_ENABLED(pBc);

    // Return cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSI2166ITF_SetConfiguration -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSI2166ITF_SetConfiguration(DtBcSI2166ITF* pBc, Int Si2166Mode, Int DataType)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_SI2166ITF_DEFAULT_PRECONDITIONS(pBc);

     // Check parameters
    if (Si2166Mode!=DT_SI2166ITF_SI2166MODE_GSE && Si2166Mode!=DT_SI2166ITF_SI2166MODE_TS)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SI2166ITF_MUST_BE_ENABLED(pBc);

    // Must be IDLE
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Set new control setting
    DtBcSI2166ITF_SetControlRegs(pBc, pBc->m_BlockEnabled, 
                                            pBc->m_OperationalMode, Si2166Mode, DataType);

    // Save new settings
    pBc->m_Si2166Mode = Si2166Mode;
    pBc->m_DataType = DataType;

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSI2166ITF_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSI2166ITF_SetOperationalMode(DtBcSI2166ITF* pBc, Int OpMode)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_SI2166ITF_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY 
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SI2166ITF_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Set new control register value
    DtBcSI2166ITF_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode, 
                                                      pBc->m_Si2166Mode, pBc->m_DataType);

    // Save new settings
    pBc->m_OperationalMode = OpMode;
    
    return Status;
}

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSI2166ITF - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSI2166ITF_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSI2166ITF_Init(DtBc*  pBcBase)
{
    DtBcSI2166ITF* pBc = (DtBcSI2166ITF*)pBcBase;
    
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSI2166ITF));

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_Si2166Mode = DT_SI2166ITF_SI2166MODE_TS;
    pBc->m_DataType = DT_PACKETSTREAM_DATATYPE_UNSEPEC;
    
    DtBcSI2166ITF_SetControlRegs(BC_SI2166ITF, BC_SI2166ITF->m_BlockEnabled, 
                                                          BC_SI2166ITF->m_OperationalMode, 
                                                          BC_SI2166ITF->m_Si2166Mode,
                                                          BC_SI2166ITF->m_DataType);

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSI2166ITF_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSI2166ITF_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcSI2166ITF* pBc = (DtBcSI2166ITF*)pBcBase;

    // Sanity check
    BC_SI2166ITF_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, SI2166ITF, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, SI2166ITF, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcSI2166ITF_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SI2166ITF, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcSI2166ITF_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                      pBc->m_Si2166Mode, pBc->m_DataType);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSI2166ITF_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSI2166ITF_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcSI2166ITF* pBc = (DtBcSI2166ITF*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_SI2166ITF_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, SI2166ITF, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcSI2166ITF_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SI2166ITF, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSI2166ITF_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcSI2166ITF_SetControlRegs(DtBcSI2166ITF* pBc, Bool BlkEna, Int OpMode, 
                                                             Int Si2166Mode, Int DataType)
{
    UInt32 FwBlkEna=0, FwOpMode=0, FwSi2166Mode=0, RegData=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? SI2166ITF_BLKENA_ENABLED : SI2166ITF_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = SI2166ITF_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = SI2166ITF_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = SI2166ITF_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }
    // Convert SDI mode to BB-type
    switch (Si2166Mode)
    {
    case DT_SI2166ITF_SI2166MODE_GSE:  FwSi2166Mode = SI2166ITF_SI2166MODE_GSE; break;
    case DT_SI2166ITF_SI2166MODE_TS:   FwSi2166Mode = SI2166ITF_SI2166MODE_TS; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control
    RegData = SI2166ITF_Control_READ(pBc);
    RegData = SI2166ITF_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = SI2166ITF_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = SI2166ITF_Control_SET_Si2166Mode(RegData, FwSi2166Mode);
    RegData = SI2166ITF_Control_SET_DataType(RegData, DataType);
    SI2166ITF_Control_WRITE(pBc, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSI2166ITF implementation +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define SI2166ITF_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                      DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSI2166ITF))
// Helper macro to cast stub's DtBc member to DtBcSI2166ITF
#define SI2166ITF_STUB   ((DtIoStubBcSI2166ITF*)pStub)
#define SI2166ITF_BC  ((DtBcSI2166ITF*)SI2166ITF_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcSI2166ITF_OnCmd(
                                       const DtIoStub*, DtIoStubIoParams*, Int* pOutSize);
static DtStatus DtIoStubBcSI2166ITF_OnCmdGetConfig(const DtIoStubBcSI2166ITF*, 
                                                     DtIoctlSi2166ItfCmdGetConfigOutput*);
static DtStatus DtIoStubBcSI2166ITF_OnCmdGetOpMode(const DtIoStubBcSI2166ITF*, 
                                                     DtIoctlSi2166ItfCmdGetOpModeOutput*);
static DtStatus DtIoStubBcSI2166ITF_OnCmdSetConfig(const DtIoStubBcSI2166ITF*, 
                                                const DtIoctlSi2166ItfCmdSetConfigInput*);
static DtStatus DtIoStubBcSI2166ITF_OnCmdSetOpMode(const DtIoStubBcSI2166ITF*, 
                                                const DtIoctlSi2166ItfCmdSetOpModeInput*);
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_SI2166ITF;

static const DtIoctlProperties  IOSTUB_BC_SI2166ITF_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_SI2166ITF_CMD(
        DtIoStubBcSI2166ITF_OnCmd, 
        NULL, NULL),
};


// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSI2166ITF - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSI2166ITF_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcSI2166ITF_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSI2166ITF));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSI2166ITF_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcSI2166ITF*  DtIoStubBcSI2166ITF_Open(DtBc*  pBc)
{
    DtIoStubBcSI2166ITF*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSI2166ITF));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcSI2166ITF, pBc, NULL,
        DtIoStubBcSI2166ITF_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_SI2166ITF_IOCTLS);
    pStub = (DtIoStubBcSI2166ITF*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSI2166ITF - Private functions +=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSI2166ITF_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSI2166ITF_OnCmd(const DtIoStub* pStub,
                                               DtIoStubIoParams* pIoParams, Int* pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSi2166ItfCmdInput*  pInData = NULL;
    DtIoctlSi2166ItfCmdOutput*  pOutData = NULL;

    SI2166ITF_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_SI2166ITF_CMD);

    // Do we need exclusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, SI2166ITF, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_Si2166ItfCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_Si2166ItfCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_SI2166ITF_CMD_GET_CONFIGURATION:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSI2166ITF_OnCmdGetConfig(SI2166ITF_STUB, 
                                                                  &pOutData->m_GetConfig);
        break;
    case DT_SI2166ITF_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSI2166ITF_OnCmdGetOpMode(SI2166ITF_STUB, 
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_SI2166ITF_CMD_SET_CONFIGURATION:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSI2166ITF_OnCmdSetConfig(SI2166ITF_STUB,&pInData->m_SetConfig);
        break;
    case DT_SI2166ITF_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSI2166ITF_OnCmdSetOpMode(SI2166ITF_STUB,&pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSI2166ITF_OnCmdGetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcSI2166ITF_OnCmdGetConfig(const DtIoStubBcSI2166ITF* pStub, 
                                             DtIoctlSi2166ItfCmdGetConfigOutput* pOutData)
{
    SI2166ITF_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    // Get configuration
    return DtBcSI2166ITF_GetConfiguration(
                            SI2166ITF_BC, &pOutData->m_Si2166Mode, &pOutData->m_DataType);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSI2166ITF_OnCmdGetOpMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcSI2166ITF_OnCmdGetOpMode(const DtIoStubBcSI2166ITF* pStub, 
                                             DtIoctlSi2166ItfCmdGetOpModeOutput* pOutData)
{
    SI2166ITF_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    // Get operational mode
    return DtBcSI2166ITF_GetOperationalMode(SI2166ITF_BC, &pOutData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSI2166ITF_OnCmdSetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcSI2166ITF_OnCmdSetConfig(const DtIoStubBcSI2166ITF* pStub, 
                                         const DtIoctlSi2166ItfCmdSetConfigInput* pInData)
{
    SI2166ITF_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    // Apply configuration
    return DtBcSI2166ITF_SetConfiguration(
                                SI2166ITF_BC, pInData->m_Si2166Mode, pInData->m_DataType);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSI2166ITF_OnCmdSetOpMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcSI2166ITF_OnCmdSetOpMode(const DtIoStubBcSI2166ITF* pStub, 
                                         const DtIoctlSi2166ItfCmdSetOpModeInput* pInData)
{
    SI2166ITF_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    // Apply operational mode
    return DtBcSI2166ITF_SetOperationalMode(SI2166ITF_BC, pInData->m_OpMode);
}
