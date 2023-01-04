//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQUNPCK.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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
#include "DtBcIQUNPCK.h"
#include "DtBcIQUNPCK_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQUNPCK implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQUNPCK_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQUNPCK))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQUNPCK_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQUNPCK, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQUNPCK_Init(DtBc*);
static DtStatus  DtBcIQUNPCK_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIQUNPCK_OnEnable(DtBc*, Bool  Enable);
static void  DtBcIQUNPCK_SetControlRegs(DtBcIQUNPCK*, Bool BlkEna, Int OpMode, 
                                                                          Int Packetmode);
static DtStatus  DtBcIQUNPCK_SetPacketMode(DtBcIQUNPCK*, Int PacketMode);
static DtStatus  DtBcIQUNPCK_GetPacketMode(DtBcIQUNPCK*, Int * pPacketMode);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQUNPCK - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUNPCK_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQUNPCK_Close(DtBc*  pBc)
{
    BC_IQUNPCK_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUNPCK_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIQUNPCK*  DtBcIQUNPCK_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_IQUNPCK_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQUNPCK, Id, DT_BLOCK_TYPE_IQUNPCK,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQUNPCK_Close;
    OpenParams.m_InitFunc = DtBcIQUNPCK_Init;
    OpenParams.m_OnEnableFunc = DtBcIQUNPCK_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIQUNPCK_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQUNPCK*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUNPCK_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQUNPCK_GetOperationalMode(DtBcIQUNPCK* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IQUNPCK_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQUNPCK_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUNPCK_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQUNPCK_SetOperationalMode(DtBcIQUNPCK* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_IQUNPCK_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY 
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQUNPCK_MUST_BE_ENABLED(pBc);
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;

    // Set control register
    DtBcIQUNPCK_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                       pBc->m_PacketMode);

    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQUNPCK - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUNPCK_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQUNPCK_Init(DtBc* pBcBase)
{
    DtBcIQUNPCK* pBc = (DtBcIQUNPCK*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_PacketMode = DT_IQUNPCK_PCKMODE_IQSAMPLES;

    // Set control register
    DtBcIQUNPCK_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                       pBc->m_PacketMode);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUNPCK_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQUNPCK_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQUNPCK* pBc = (DtBcIQUNPCK*)pBcBase;

    // Sanity check
    BC_IQUNPCK_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IQUNPCK, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IQUNPCK, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcIQUNPCK_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQUNPCK, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIQUNPCK_SetControlRegs(pBc,pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                       pBc->m_PacketMode);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUNPCK_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQUNPCK_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQUNPCK* pBc = (DtBcIQUNPCK*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IQUNPCK_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IQUNPCK, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIQUNPCK_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQUNPCK, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUNPCK_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQUNPCK_SetControlRegs(DtBcIQUNPCK* pBc, Bool BlkEna, Int OpMode, 
                                                                           Int PacketMode)
{
    UInt32  RegData=0, FwBlkEna=0, FwOpMode=0, PcktMode=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IQUNPCK_BLKENA_ENABLED : IQUNPCK_BLKENA_DISABLED;

    // Convert packet mode to firmware type
    PcktMode = (PacketMode==DT_IQUNPCK_PCKMODE_IQSAMPLES) ? IQUNPCK_PCKMODE_IQSAMPLES :
                                                                IQUNPCK_PCKMODE_IQPACKETS;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = IQUNPCK_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = IQUNPCK_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = IQUNPCK_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = IQUNPCK_Control_READ(pBc);
    RegData = IQUNPCK_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IQUNPCK_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = IQUNPCK_Control_SET_PacketMode(RegData, PcktMode);
    IQUNPCK_Control_WRITE(pBc, RegData);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUNPCK_SetPacketMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQUNPCK_SetPacketMode(DtBcIQUNPCK* pBc, Int PacketMode)
{
    // Sanity check
    BC_IQUNPCK_DEFAULT_PRECONDITIONS(pBc);
    BC_IQUNPCK_MUST_BE_ENABLED(pBc);

    if(PacketMode != DT_IQUNPCK_PCKMODE_IQSAMPLES && 
                                               PacketMode != DT_IQUNPCK_PCKMODE_IQPACKETS)
        return DT_STATUS_INVALID_PARAMETER;

    pBc->m_PacketMode = PacketMode;
    DtBcIQUNPCK_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                              PacketMode);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQUNPCK_GetPacketMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQUNPCK_GetPacketMode(DtBcIQUNPCK* pBc, Int * pPacketMode)
{
    // Sanity check
    BC_IQUNPCK_DEFAULT_PRECONDITIONS(pBc);
    BC_IQUNPCK_MUST_BE_ENABLED(pBc);
    *pPacketMode = pBc->m_PacketMode;

    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQUNPCK implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IQUNPCK_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQUNPCK))
// Helper macro to cast stub's DtBc member to DtBcIQUNPCK
#define IQUNPCK_STUB   ((DtIoStubBcIQUNPCK*)pStub)
#define IQUNPCK_BC  ((DtBcIQUNPCK*)IQUNPCK_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQUNPCK_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcIQUNPCK_OnCmdGetOperationalMode(
                                                       const DtIoStubBcIQUNPCK*, 
                                                       DtIoctlIqUnpckCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcIQUNPCK_OnCmdGetPacketMode(
                                              const DtIoStubBcIQUNPCK*,
                                              DtIoctlIqUnpckCmdGetPacketModeOutput*);

static DtStatus  DtIoStubBcIQUNPCK_OnCmdSetOperationalMode(
                                                  const DtIoStubBcIQUNPCK*,
                                                  const DtIoctlIqUnpckCmdSetOpModeInput*);
static DtStatus  DtIoStubBcIQUNPCK_OnCmdSetPacketMode(
                                              const DtIoStubBcIQUNPCK*,
                                              const DtIoctlIqUnpckCmdSetPacketModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQUNPCK;

static const DtIoctlProperties  IOSTUB_BC_IQUNPCK_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQUNPCK_CMD(
        DtIoStubBcIQUNPCK_OnCmd,
        NULL, NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQUNPCK - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUNPCK_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcIQUNPCK_Close(DtIoStub* pStub)
{
    IQUNPCK_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUNPCK_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcIQUNPCK*  DtIoStubBcIQUNPCK_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IQUNPCK_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQUNPCK, pBc, NULL, 
                                                            DtIoStubBcIQUNPCK_Close,
                                                            NULL,  // Use default IOCTL
                                                            IOSTUB_BC_IQUNPCK_IOCTLS);
    return (DtIoStubBcIQUNPCK*)DtIoStubBc_Open(&OpenParams);
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQUNPCK - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUNPCK_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQUNPCK_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqUnpckCmdInput*  pInData = NULL;
    DtIoctlIqUnpckCmdOutput*  pOutData = NULL;

    IQUNPCK_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQUNPCK_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQUNPCK, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqUnpckCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqUnpckCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IQUNPCK_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQUNPCK_OnCmdGetOperationalMode(IQUNPCK_STUB,  
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_IQUNPCK_CMD_GET_PACKET_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQUNPCK_OnCmdGetPacketMode(IQUNPCK_STUB,  
                                                              &pOutData->m_GetPacketMode);
        break;
    case DT_IQUNPCK_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQUNPCK_OnCmdSetOperationalMode(IQUNPCK_STUB,  
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_IQUNPCK_CMD_SET_PACKET_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQUNPCK_OnCmdSetPacketMode(IQUNPCK_STUB,  
                                                               &pInData->m_SetPacketMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUNPCK_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQUNPCK_OnCmdGetOperationalMode(
    const DtIoStubBcIQUNPCK* pStub,
    DtIoctlIqUnpckCmdGetOpModeOutput* pOutData)
{
    IQUNPCK_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQUNPCK_GetOperationalMode(IQUNPCK_BC, &pOutData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUNPCK_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQUNPCK_OnCmdSetOperationalMode(
    const DtIoStubBcIQUNPCK* pStub,
    const DtIoctlIqUnpckCmdSetOpModeInput* pInData)
{
    IQUNPCK_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQUNPCK_SetOperationalMode(IQUNPCK_BC, pInData->m_OpMode);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUNPCK_OnCmdGetPacketMode -.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtIoStubBcIQUNPCK_OnCmdGetPacketMode(
    const DtIoStubBcIQUNPCK* pStub,
    DtIoctlIqUnpckCmdGetPacketModeOutput* pOutData)
{
    IQUNPCK_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);
    return DtBcIQUNPCK_GetPacketMode(IQUNPCK_BC, &pOutData->m_PacketMode);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQUNPCK_OnCmdSetPacketMode -.-.-.-.-.-.-.-.-.-.-.-.-
//
static DtStatus  DtIoStubBcIQUNPCK_OnCmdSetPacketMode(
    const DtIoStubBcIQUNPCK* pStub,
    const DtIoctlIqUnpckCmdSetPacketModeInput* pInpData)
{
    IQUNPCK_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInpData != NULL);
    return DtBcIQUNPCK_SetPacketMode(IQUNPCK_BC, pInpData->m_PacketMode);
}
