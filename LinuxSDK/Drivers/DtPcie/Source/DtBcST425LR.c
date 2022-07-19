// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcST425LR.c *#*#*#*#*#*#*#*#*#*# (C) 2019 DekTec
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
#include "DtBcST425LR.h"
#include "DtBcST425LR_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcST425LR implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_ST425LR_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcST425LR))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_ST425LR_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(ST425LR, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcST425LR_Init(DtBc*);
static DtStatus  DtBcST425LR_OnEnable(DtBc*, Bool);
static DtStatus  DtBcST425LR_OnCloseFile(DtBc*, const DtFileObject*);
static void  DtBcST425LR_SetControlRegs(DtBcST425LR*, Bool BlkEna, Int OpMode, 
                                                                  const UInt8 pLinkIn[4]);
static Bool DtBcST425LR_AreQuadLinksSwapped(DtBcST425LR* pBc);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcST425LR - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcST425LR_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcST425LR_Close(DtBc*  pBc)
{
    BC_ST425LR_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcST425LR_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcST425LR*  DtBcST425LR_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_ST425LR_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcST425LR, Id, DT_BLOCK_TYPE_ST425LR,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcST425LR_Close;
    OpenParams.m_InitFunc = DtBcST425LR_Init;
    OpenParams.m_OnCloseFileFunc = DtBcST425LR_OnCloseFile;
    OpenParams.m_OnEnableFunc = DtBcST425LR_OnEnable;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcST425LR*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcST425LR_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcST425LR_GetOperationalMode(DtBcST425LR* pBc, Int* pOpMode)
{
    // Sanity check
    BC_ST425LR_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ST425LR_MUST_BE_ENABLED(pBc);

    // Return last cached operational mode
    *pOpMode = pBc->m_OperationalMode;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcST425LR_GetLinkOrder -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcST425LR_GetLinkOrder(DtBcST425LR* pBc, UInt8 pLinkIn[4])
{
    Int i;
    // Sanity check
    BC_ST425LR_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pLinkIn == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ST425LR_MUST_BE_ENABLED(pBc);

    // Return cached link order
    for (i=0; i<4; i++)
        pLinkIn[i] = pBc->m_pLinkIn[i];
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcST425LR_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcST425LR_SetOperationalMode(DtBcST425LR* pBc, Int OpMode)
{
    // Sanity check
    BC_ST425LR_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, ST425LR, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_ST425LR_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Save new setting
    pBc->m_OperationalMode = OpMode;
    // Set control registers
    DtBcST425LR_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                          pBc->m_pLinkIn);
    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcST425LR_SetLinkOrder -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcST425LR_SetLinkOrder(DtBcST425LR* pBc, const UInt8  pLinkIn[4])
{
    Int i;
    // Sanity check
    BC_ST425LR_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pLinkIn == NULL) 
        return DT_STATUS_INVALID_PARAMETER;
    for (i=0; i<4; i++)
        if (pLinkIn[i] > 3)
            return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ST425LR_MUST_BE_ENABLED(pBc);

    // Operational mode must be IDLE
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
    {
        DtDbgOutBc(ERR, ST425LR, pBc, "Operational mode not in idle");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // Save new setting
    for (i=0; i<4; i++)
        pBc->m_pLinkIn[i] = pLinkIn[i];

    // Set control registers
    DtBcST425LR_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                          pBc->m_pLinkIn);
    return DT_STATUS_OK;
}


// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcST425LR - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcST425LR_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcST425LR_Init(DtBc*  pBcBase)
{
    DtBcST425LR* pBc = (DtBcST425LR*)pBcBase;
    Int i;

    // Sanity checks
    BC_ST425LR_DEFAULT_PRECONDITIONS(pBc);

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    for (i=0; i<4; i++)
        pBc->m_pLinkIn[i] = (UInt8)i;
    // Set control registers
    DtBcST425LR_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                          pBc->m_pLinkIn);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcST425LR_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcST425LR_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcST425LR* pBc = (DtBcST425LR*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_ST425LR_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, ST425LR, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcST425LR_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, ST425LR, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcST425LR_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcST425LR_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcST425LR* pBc = (DtBcST425LR*)pBcBase;

    // Sanity check
    BC_ST425LR_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    { 
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, ST425LR, pBc, "Block Enable from disable -> enable");

        // Save new setting
        pBc->m_BlockEnabled = Enable;
        // Set control registers
        DtBcST425LR_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                          pBc->m_pLinkIn);
    }
    else
    { 
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, ST425LR, pBc, "Block Enable from enable -> disable");
        Status = DtBcST425LR_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        DT_ASSERT(DT_SUCCESS(Status));
        // Save new setting
        pBc->m_BlockEnabled = Enable;
        // Set control registers
        DtBcST425LR_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                          pBc->m_pLinkIn);
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcST425LR_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtBcST425LR_SetControlRegs(DtBcST425LR* pBc, Bool BlkEnable, Int OpMode, 
                                                                   const UInt8 pLinkIn[4])
{
     UInt RegData, FwOpMode, FwBlkEnable;
     //Sanity check
    BC_ST425LR_DEFAULT_PRECONDITIONS(pBc);


    // Convert block enable to BB-type
    FwBlkEnable = BlkEnable ? ST425LR_BLKENA_ENABLED : ST425LR_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = ST425LR_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = ST425LR_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE); return;
    }

    // Invalid combination?
    DT_ASSERT(BlkEnable || OpMode==DT_BLOCK_OPMODE_IDLE);

    // Update ST425LR control register
    RegData = ST425LR_Control_READ(pBc);
    RegData = ST425LR_Control_SET_BlockEnable(RegData, FwBlkEnable);
    RegData = ST425LR_Control_SET_OperationalMode(RegData, FwOpMode);
    if (DtBcST425LR_AreQuadLinksSwapped(pBc))
    {
        // Workaround for TT#3422 and TT#3423
        RegData = ST425LR_Control_SET_LinkIn0(RegData, pLinkIn[3]);
        RegData = ST425LR_Control_SET_LinkIn1(RegData, pLinkIn[2]);
        RegData = ST425LR_Control_SET_LinkIn2(RegData, pLinkIn[1]);
        RegData = ST425LR_Control_SET_LinkIn3(RegData, pLinkIn[0]);
    } else {
        RegData = ST425LR_Control_SET_LinkIn0(RegData, pLinkIn[0]);
        RegData = ST425LR_Control_SET_LinkIn1(RegData, pLinkIn[1]);
        RegData = ST425LR_Control_SET_LinkIn2(RegData, pLinkIn[2]);
        RegData = ST425LR_Control_SET_LinkIn3(RegData, pLinkIn[3]);
    }
    ST425LR_Control_WRITE(pBc, RegData);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcST425LR_AreQuadLinksSwapped -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtBcST425LR_AreQuadLinksSwapped(DtBcST425LR* pBc)
{
    // See TT#3422 and TT#3423
    // Odd and even lines from a SMPTE 425-5 quad-link input are swapped
    // in the resulting 4k picture
    Int  TypeNumber = DtCore_DEVICE_GetTypeNumber(pBc->m_pCore);
    Int  SubType = DtCore_DEVICE_GetSubType(pBc->m_pCore);
    Int  Variant =  DtCore_DEVICE_GetFirmwareVariant(pBc->m_pCore);
    Int  FwVersion =  DtCore_DEVICE_GetFirmwareVersion(pBc->m_pCore);
    if (   (TypeNumber==2174 && SubType==2 && Variant==2 && FwVersion==0)
        || (TypeNumber==2178 && SubType==0 && Variant==2 && FwVersion==0))
    {
        return TRUE;
    }
    return FALSE;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcST425LR implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define ST425LR_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                      DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcST425LR))
// Helper macro to cast stub's DtBc member to DtBcST425LR
#define ST425LR_STUB   ((DtIoStubBcST425LR*)pStub)
#define ST425LR_BC  ((DtBcST425LR*)ST425LR_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcST425LR_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);

static DtStatus  DtIoStubBcST425LR_OnCmdGetOperationalMode(const DtIoStubBcST425LR*, 
                                                     DtIoctlSt425LrCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcST425LR_OnCmdGetLinkOrder(const DtIoStubBcST425LR*, 
                                                    DtIoctlSt425LrCmdGetLinkOrderOutput*);
static DtStatus  DtIoStubBcST425LR_OnCmdSetOperationalMode(const DtIoStubBcST425LR*, 
                                                const DtIoctlSt425LrCmdSetOpModeInput*);
static DtStatus  DtIoStubBcST425LR_OnCmdSetLinkOrder(const DtIoStubBcST425LR*, 
                                               const DtIoctlSt425LrCmdSetLinkOrderInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_ST425LR;

static const DtIoctlProperties  IOSTUB_BC_ST425LR_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_ST425LR_CMD(
        DtIoStubBcST425LR_OnCmd,
        NULL,  NULL),
};

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcST425LR - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcST425LR_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcST425LR_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcST425LR));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcST425LR_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcST425LR*  DtIoStubBcST425LR_Open(DtBc*  pBc)
{
    DtIoStubBcST425LR*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    BC_ST425LR_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcST425LR, pBc, NULL, 
                                                             DtIoStubBcST425LR_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_BC_ST425LR_IOCTLS);
    pStub = (DtIoStubBcST425LR*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcST425LR- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcST425LR_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcST425LR_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSt425LrCmdInput*  pInData = NULL;
    DtIoctlSt425LrCmdOutput*  pOutData = NULL;

    ST425LR_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_ST425LR_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, ST425LR, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_St425LrCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_St425LrCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_ST425LR_CMD_GET_LINK_ORDER:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcST425LR_OnCmdGetLinkOrder(ST425LR_STUB, 
                                                               &pOutData->m_GetLinkOrder);
        break;
    case DT_ST425LR_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcST425LR_OnCmdGetOperationalMode(ST425LR_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_ST425LR_CMD_SET_LINK_ORDER:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcST425LR_OnCmdSetLinkOrder(ST425LR_STUB, 
                                                                &pInData->m_SetLinkOrder);
        break;
    case DT_ST425LR_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcST425LR_OnCmdSetOperationalMode(ST425LR_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcST425LR_OnCmdGetLinkOrder -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcST425LR_OnCmdGetLinkOrder(
    const DtIoStubBcST425LR* pStub,
    DtIoctlSt425LrCmdGetLinkOrderOutput* pOutData)
{
    ST425LR_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcST425LR_GetLinkOrder(ST425LR_BC, pOutData->m_pLinkIn);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcST425LR_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcST425LR_OnCmdGetOperationalMode(
    const DtIoStubBcST425LR* pStub,
    DtIoctlSt425LrCmdGetOpModeOutput* pOutData)
{
    ST425LR_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcST425LR_GetOperationalMode(ST425LR_BC, &pOutData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcST425LR_OnCmdSetLinkOrder -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcST425LR_OnCmdSetLinkOrder(
    const DtIoStubBcST425LR* pStub,
    const DtIoctlSt425LrCmdSetLinkOrderInput* pInData)
{
    ST425LR_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcST425LR_SetLinkOrder(ST425LR_BC, pInData->m_pLinkIn);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcST425LR_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcST425LR_OnCmdSetOperationalMode(
    const DtIoStubBcST425LR* pStub,
    const DtIoctlSt425LrCmdSetOpModeInput* pInData)
{
    ST425LR_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcST425LR_SetOperationalMode(ST425LR_BC, pInData->m_OpMode);
}


