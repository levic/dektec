//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcADS4246_2132.c *#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
#include "DtBcADS4246_2132.h"
#include "DtBcADS4246_2132_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcADS4246_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_ADS4246_2132_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcADS4246_2132))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_ADS4246_2132_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(ADS4246_2132, pBc)

// Helper macro to cast a DtBc* to a DtBcADS4246_2132*
#define BC_ADS4246_2132         ((DtBcADS4246_2132*)pBc)

DtStatus DtBcADS4246_2132_SetOperationalMode(DtBcADS4246_2132 * pBc, Int OpMode);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcADS4246_2132_Init(DtBc*);
static DtStatus  DtBcADS4246_2132_OnEnable(DtBc*, Bool);
static DtStatus  DtBcADS4246_2132_OnCloseFile(DtBc*, const DtFileObject*);
static void  DtBcADS4246_2132_SetControlRegs(DtBcADS4246_2132* pBc, 
                                                                 Bool BlkEna, Int OpMode);

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcADS4246_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcADS4246_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcADS4246_2132_Close(DtBc*  pBc)
{
    BC_ADS4246_2132_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcADS4246_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcADS4246_2132*  DtBcADS4246_2132_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_ADS4246_2132_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcADS4246_2132, Id, DT_BLOCK_TYPE_ADS4246_2132, 
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcADS4246_2132_Close;
    OpenParams.m_InitFunc = DtBcADS4246_2132_Init;
    OpenParams.m_OnEnableFunc = DtBcADS4246_2132_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcADS4246_2132_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcADS4246_2132*)DtBc_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtBcADS4246_2132_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcADS4246_2132_GetOperationalMode(DtBcADS4246_2132* pBc, Int* pOpMode)
{
    // Sanity check
    BC_ADS4246_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_ADS4246_2132_MUST_BE_ENABLED(pBc);

    // Return last cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtBcADS4246_2132_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcADS4246_2132_SetOperationalMode(DtBcADS4246_2132* pBc, Int OpMode)
{
    // Sanity check
    BC_ADS4246_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, ADS4246_2132, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_ADS4246_2132_MUST_BE_ENABLED(pBc);

    DtBcADS4246_2132_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode);
    
    // Store new cached settings
    pBc->m_OperationalMode = OpMode;

    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcADS4246_2132 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcADS4246_2132_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcADS4246_2132_Init(DtBc*  pBc)
{
    // Sanity checks
    BC_ADS4246_2132_DEFAULT_PRECONDITIONS(pBc);

    // Set defaults
    BC_ADS4246_2132->m_BlockEnabled = FALSE;
    BC_ADS4246_2132->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    DtBcADS4246_2132_SetControlRegs(BC_ADS4246_2132, FALSE, DT_BLOCK_OPMODE_IDLE);

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcADS4246_2132_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcADS4246_2132_OnEnable(DtBc* pBcBase, Bool  Enable)
{
    DtBcADS4246_2132* pBc = (DtBcADS4246_2132*)pBcBase;

    // Sanity check
    BC_ADS4246_2132_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    { 
        // DISABLE -> ENABLE' set cached values and enable
        DtDbgOutBc(AVG, ADS4246_2132, pBc, "Block Enable from disable -> enable");
        DtBcADS4246_2132_SetControlRegs(pBc, TRUE, pBc->m_OperationalMode);
    }
    else
    { 
        // ENABLE -> DISABLE'
        DtDbgOutBc(AVG, ADS4246_2132, pBc, "Block Enable from enable -> disable");
        // Go to IDLE
        DtBcADS4246_2132_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        // and disable block
        DtBcADS4246_2132_SetControlRegs(pBc, FALSE, pBc->m_OperationalMode);
    }

    // Store new cached setting
    pBc->m_BlockEnabled = Enable;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcADS4246_2132_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcADS4246_2132_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_ADS4246_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutBc(AVG, ADS4246_2132, pBc, "Set operational mode to IDLE");

        // Set operational mode to IDLE
        Status = DtBcADS4246_2132_SetOperationalMode(BC_ADS4246_2132,
                                                                    DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, ADS4246_2132, pBc, "ERROR: failed to set opmode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcADS4246_2132_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void DtBcADS4246_2132_SetControlRegs(DtBcADS4246_2132* pBc, Bool BlkEnable, Int OpMode)
{
    UInt32 RegControl, FldOpMode, FldBlkEnable;

    // Convert block enable to BB-type
    if (BlkEnable)
        FldBlkEnable = ADS4246_BLKENA_ENABLED;
    else
        FldBlkEnable = ADS4246_BLKENA_DISABLED;

    // TODOTD update this when ADS4246 follows the operational mode / block enable model
    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FldOpMode = ADS4246_BLKENA_DISABLED; break;
    case DT_BLOCK_OPMODE_RUN:     FldOpMode = ADS4246_BLKENA_ENABLED; break;
    default: DT_ASSERT(FALSE); return;
    }

    // Invalid combination?
    DT_ASSERT(BlkEnable || OpMode==DT_BLOCK_OPMODE_IDLE);

    // Update ADS4246_2132 control register
    RegControl = ADS4246_Control_READ(pBc);
    // RegControl = ADS4246_2132_Control_SET_BlockEnable(RegControl, FldBlkEnable);
    RegControl = ADS4246_Control_SET_OperationalMode(RegControl, FldOpMode);
    ADS4246_Control_WRITE(pBc, RegControl);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcADS4246_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcADS4246_2132
#define ADS4246_2132_STUB   ((DtIoStubBcADS4246_2132*)pStub)
#define ADS4246_2132_BC  ((DtBcADS4246_2132*)ADS4246_2132_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcADS4246_2132_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcADS4246_2132_OnCmdGetOperationalMode(
                                                  const DtIoStubBcADS4246_2132*, 
                                                  DtIoctlAds4246Cmd_2132GetOpModeOutput*);
static DtStatus  DtIoStubBcADS4246_2132_OnCmdSetOperationalMode(
                                             const DtIoStubBcADS4246_2132*, 
                                             const DtIoctlAds4246Cmd_2132SetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_ADS4246_2132;

static const DtIoctlProperties  IOSTUB_BC_ADS4246_2132_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_ADS4246_CMD_2132(
        DtIoStubBcADS4246_2132_OnCmd,
        NULL,  NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcADS4246_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcADS4246_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcADS4246_2132_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcADS4246_2132));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcADS4246_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcADS4246_2132*  DtIoStubBcADS4246_2132_Open(DtBc*  pBc)
{
    DtIoStubBcADS4246_2132*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcADS4246_2132));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcADS4246_2132, pBc, NULL, 
                                                           DtIoStubBcADS4246_2132_Close,
                                                           NULL,  // Use default IOCTL
                                                           IOSTUB_BC_ADS4246_2132_IOCTLS);
    pStub = (DtIoStubBcADS4246_2132*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcADS4246_2132- Private functions +=+=+=+=+=+=+=+=+=+=+=+


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcADS4246_2132_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcADS4246_2132_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlAds4246CmdInput_2132*  pInData = NULL;
    DtIoctlAds4246CmdOutput_2132*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcADS4246_2132));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_ADS4246_CMD_2132);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, ADS4246_2132, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_Ads4246Cmd_2132;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_Ads4246Cmd_2132;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_ADS4246_CMD_2132_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcADS4246_2132_OnCmdGetOperationalMode(ADS4246_2132_STUB,  
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_ADS4246_CMD_2132_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcADS4246_2132_OnCmdSetOperationalMode(ADS4246_2132_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.- DtIoStubBcADS4246_2132_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcADS4246_2132_OnCmdGetOperationalMode(
    const DtIoStubBcADS4246_2132* pStub,
    DtIoctlAds4246Cmd_2132GetOpModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcADS4246_2132));
    DT_ASSERT(pOutData != NULL);

    return DtBcADS4246_2132_GetOperationalMode(ADS4246_2132_BC, &pOutData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.- DtIoStubBcADS4246_2132_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcADS4246_2132_OnCmdSetOperationalMode(
    const DtIoStubBcADS4246_2132* pStub,
    const DtIoctlAds4246Cmd_2132SetOpModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcADS4246_2132));
    DT_ASSERT(pInData != NULL);

    return DtBcADS4246_2132_SetOperationalMode(ADS4246_2132_BC, pInData->m_OpMode);
}