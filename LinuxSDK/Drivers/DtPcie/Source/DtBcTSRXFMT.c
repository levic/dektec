//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcTSRXFMT.c *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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
#include "DtDf.h"
#include "DtBcTSRXFMT.h"
#include "DtBcTSRXFMT_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcTSRXFMT implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_TSRXFMT_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcTSRXFMT))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_TSRXFMT_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(TSRXFMT, pBc)

// Helper macro to cast a DtBc* to a DtBcTSRXFMT*
#define BC_TSRXFMT         ((DtBcTSRXFMT*)pBc)

// Helper macro to get the BcTOD block-controller from a DtCore object
#define BC_TOD(pCore)         ((DtBcTOD*)(pCore->m_pBcTod))

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus DtBcTSRXFMT_Init(DtBc*);
static DtStatus DtBcTSRXFMT_OnEnable(DtBc*, Bool  Enable);
static void  DtBcTSRXFMT_SetControlRegs(DtBcTSRXFMT*, Bool BlkEna, Int OpMode, 
                                                               Int PckMode, Int SyncMode);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcTSRXFMT - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTSRXFMT_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcTSRXFMT_Close(DtBc*  pBc)
{
    BC_TSRXFMT_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTSRXFMT_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcTSRXFMT*  DtBcTSRXFMT_Open(Int  Address, DtCore*  pCore, DtPt*  pPt,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_TSRXFMT_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcTSRXFMT, Id, DT_BLOCK_TYPE_TSRXFMT, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcTSRXFMT_Close;
    OpenParams.m_InitFunc = DtBcTSRXFMT_Init;
    OpenParams.m_OnEnableFunc = DtBcTSRXFMT_OnEnable;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcTSRXFMT*)DtBc_Open(&OpenParams);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTSRXFMT_GetOperationalStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcTSRXFMT_GetOperationalStatus(DtBcTSRXFMT* pBc, Int* pOpStatus)
{
    UInt FwOpStatus;

    // Sanity check
    BC_TSRXFMT_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpStatus == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_TSRXFMT_MUST_BE_ENABLED(pBc);

    // Get current operatational status and convert to BC-type
    FwOpStatus = TSRXFMT_Status_READ_OperationalStatus(pBc);
    switch (FwOpStatus)
    {
    case TSRXFMT_OPSTATUS_IDLE:   *pOpStatus = DT_BLOCK_OPSTATUS_IDLE; break;
    case TSRXFMT_OPSTATUS_RUN:    *pOpStatus = DT_BLOCK_OPSTATUS_RUN; break;
    case TSRXFMT_OPSTATUS_ERROR:  *pOpStatus = DT_BLOCK_OPSTATUS_ERROR; break;
    default: DT_ASSERT(FALSE); return DT_STATUS_FAIL;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTSRXFMT_GetPacketMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcTSRXFMT_GetPacketMode(DtBcTSRXFMT* pBc, Int* pPckMode)
{
    // Sanity check
    BC_TSRXFMT_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pPckMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_TSRXFMT_MUST_BE_ENABLED(pBc);

    // Return cached packet-mode
    *pPckMode = pBc->m_PacketMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTSRXFMT_GetSyncMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcTSRXFMT_GetSyncMode(DtBcTSRXFMT* pBc, Int* pSyncMode)
{
    // Sanity check
    BC_TSRXFMT_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pSyncMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_TSRXFMT_MUST_BE_ENABLED(pBc);

    // Return cached sync-mode
    *pSyncMode = pBc->m_SyncMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTSRXFMT_GetTsPacketSize -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcTSRXFMT_GetTsPacketSize(DtBcTSRXFMT* pBc, Int* pTsPckSize)
{
    UInt FwPckSize;

    // Sanity check
    BC_TSRXFMT_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pTsPckSize == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_TSRXFMT_MUST_BE_ENABLED(pBc);

    // Must be in standby or run
    if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_IDLE)
    { 
        DtDbgOutBc(ERR, TSRXFMT, pBc, "Block in IDLE");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // Get current packet-size and convert to BC-type
    FwPckSize = TSRXFMT_Status_READ_PacketSize(pBc);
    switch (FwPckSize)
    {
    case TSRXFMT_PCKSIZE_188:       *pTsPckSize = DT_TSRXFMT_PCKSIZE_188; break;
    case TSRXFMT_PCKSIZE_204:       *pTsPckSize = DT_TSRXFMT_PCKSIZE_204; break;
    case TSRXFMT_PCKSIZE_UNKNOWN:   *pTsPckSize = DT_TSRXFMT_PCKSIZE_UNKNOWN; break;
    default: DT_ASSERT(FALSE);      *pTsPckSize = DT_TSRXFMT_PCKSIZE_UNKNOWN; break;
    }

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTSRXFMT_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcTSRXFMT_SetOperationalMode(DtBcTSRXFMT* pBc, Int OpMode)
{
    // Sanity check
    BC_TSRXFMT_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, TSRXFMT, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }
    
    // Must be enabled
    BC_TSRXFMT_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Update control registers
    DtBcTSRXFMT_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode, pBc->m_PacketMode,
                                                                         pBc->m_SyncMode);
    // Update cache
    pBc->m_OperationalMode = OpMode;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTSRXFMT_SetPacketMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcTSRXFMT_SetPacketMode(DtBcTSRXFMT* pBc, Int PckMode)
{
    // Sanity check
    BC_TSRXFMT_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (PckMode!=DT_TSRXFMT_PCKMODE_AUTO && PckMode!=DT_TSRXFMT_PCKMODE_RAW)
    { 
        DtDbgOutBc(ERR, TSRXFMT, pBc, "Invalid packet mode");
        return DT_STATUS_INVALID_PARAMETER;
    }
    
    // Must be enabled
    BC_TSRXFMT_MUST_BE_ENABLED(pBc);

    // Must be in idle
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
    { 
        DtDbgOutBc(ERR, TSRXFMT, pBc, "Block not in IDLE");
    }

    // No change?
    if (pBc->m_PacketMode == PckMode)
        return DT_STATUS_OK;

    // Update control registers
    DtBcTSRXFMT_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, PckMode,
                                                                         pBc->m_SyncMode);
    // Update cache
    pBc->m_PacketMode = PckMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTSRXFMT_SetSyncMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcTSRXFMT_SetSyncMode(DtBcTSRXFMT* pBc, Int SyncMode)
{
    // Sanity check
    BC_TSRXFMT_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (SyncMode!=DT_TSRXFMT_SYNCMODE_188 && SyncMode!=DT_TSRXFMT_SYNCMODE_204
                                                    && SyncMode!=DT_TSRXFMT_SYNCMODE_AUTO)
    { 
        DtDbgOutBc(ERR, TSRXFMT, pBc, "Invalid sync mode");
        return DT_STATUS_INVALID_PARAMETER;
    }
    
    // Must be enabled
    BC_TSRXFMT_MUST_BE_ENABLED(pBc);

    // Must be in idle
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
    { 
        DtDbgOutBc(ERR, TSRXFMT, pBc, "Block not in IDLE");
    }

    // No change?
    if (pBc->m_SyncMode == SyncMode)
        return DT_STATUS_OK;

    // Update control registers
    DtBcTSRXFMT_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                             pBc->m_PacketMode, SyncMode);
    // Update cache
    pBc->m_SyncMode = SyncMode;

    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcTSRXFMT - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTSRXFMT_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcTSRXFMT_Init(DtBc*  pBc)
{
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcTSRXFMT));

    // Set default modes
    BC_TSRXFMT->m_BlockEnabled = FALSE;
    BC_TSRXFMT->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    BC_TSRXFMT->m_PacketMode = DT_TSRXFMT_PCKMODE_AUTO;
    BC_TSRXFMT->m_SyncMode = DT_TSRXFMT_SYNCMODE_AUTO;
    DtBcTSRXFMT_SetControlRegs(BC_TSRXFMT, BC_TSRXFMT->m_BlockEnabled,
                                  BC_TSRXFMT->m_OperationalMode, BC_TSRXFMT->m_PacketMode,
                                  BC_TSRXFMT->m_SyncMode);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTSRXFMT_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcTSRXFMT_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcTSRXFMT* pBc = (DtBcTSRXFMT*)pBcBase;

    // Sanity check
    BC_TSRXFMT_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, TSRXFMT, pBc, "Block Enable from disable -> enable");
        
        // Enable block
        DtBcTSRXFMT_SetControlRegs(pBc, Enable, pBc->m_OperationalMode, 
                                                      pBc->m_PacketMode, pBc->m_SyncMode);
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, TSRXFMT, pBc, "Block Enable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcTSRXFMT_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, TSRXFMT, pBc, "ERROR: DtBcTSRXFMT_SetOperationalMode failed");
            return Status;
        }

        // Disable block
        DtBcTSRXFMT_SetControlRegs(pBc, Enable, pBc->m_OperationalMode, 
                                                      pBc->m_PacketMode, pBc->m_SyncMode);
    }
    // Save new setting
    pBc->m_BlockEnabled = Enable;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcTSRXFMT_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcTSRXFMT_SetControlRegs(DtBcTSRXFMT* pBc, Bool BlkEna, Int OpMode, Int PckMode,
                                                                             Int SyncMode)
{
    UInt FwControl=0, FwBlkEna=0, FwOpMode=0, FwPckMode=0, FwSyncMode=0;
    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? TSRXFMT_BLKENA_ENABLED : TSRXFMT_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:      FwOpMode = TSRXFMT_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY:   FwOpMode = TSRXFMT_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:       FwOpMode = TSRXFMT_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Convert packet mode to BB-type
    switch (PckMode)
    {
    case DT_TSRXFMT_PCKMODE_AUTO: FwPckMode = TSRXFMT_PCKMODE_AUTO; break;
    case DT_TSRXFMT_PCKMODE_RAW:  FwPckMode = TSRXFMT_PCKMODE_RAW; break;
    default: DT_ASSERT(FALSE);
    }
    // Convert sync mode to BB-type
    switch (SyncMode)
    {
    case DT_TSRXFMT_SYNCMODE_AUTO: FwSyncMode = TSRXFMT_SYNC_AUTO; break;
    case DT_TSRXFMT_SYNCMODE_188:  FwSyncMode = TSRXFMT_SYNC_188; break;
    case DT_TSRXFMT_SYNCMODE_204:  FwSyncMode = TSRXFMT_SYNC_204; break;
    default: DT_ASSERT(FALSE);
    }

    // Update TSRXFMT control register
    FwControl = TSRXFMT_Control_READ(pBc);
    FwControl = TSRXFMT_Control_SET_BlockEnable(FwControl, FwBlkEna);
    FwControl = TSRXFMT_Control_SET_OperationalMode(FwControl, FwOpMode);
    FwControl = TSRXFMT_Control_SET_PacketMode(FwControl, FwPckMode);
    FwControl = TSRXFMT_Control_SET_SyncMode(FwControl, FwSyncMode);
    TSRXFMT_Control_WRITE(pBc, FwControl);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcTSRXFMT implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcTSRXFMT
#define TSRXFMT_STUB   ((DtIoStubBcTSRXFMT*)pStub)
#define TSRXFMT_BC  ((DtBcTSRXFMT*)TSRXFMT_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus DtIoStubBcTSRXFMT_OnCmd(const DtIoStub* pStub, 
                                             DtIoStubIoParams* pIoParams, Int * pOutSize);
static DtStatus  DtIoStubBcTSRXFMT_OnCmdGetOperationalStatus(const DtIoStubBcTSRXFMT*,
                                                     DtIoctlTsRxFmtCmdGetOpStatusOutput*);
static DtStatus  DtIoStubBcTSRXFMT_OnCmdGetPacketMode(const DtIoStubBcTSRXFMT*, 
                                                      DtIoctlTsRxFmtCmdGetPckModeOutput*);
static DtStatus  DtIoStubBcTSRXFMT_OnCmdGetPacketSize(const DtIoStubBcTSRXFMT*, 
                                                   DtIoctlTsRxFmtCmdGetPacketSizeOutput*);
static DtStatus  DtIoStubBcTSRXFMT_OnCmdGetSyncMode(const DtIoStubBcTSRXFMT*, 
                                                     DtIoctlTsRxFmtCmdGetSyncModeOutput*);
static DtStatus  DtIoStubBcTSRXFMT_OnCmdSetOperationalMode(const DtIoStubBcTSRXFMT*,
                                                  const DtIoctlTsRxFmtCmdSetOpModeInput*);
static DtStatus  DtIoStubBcTSRXFMT_OnCmdSetPacketMode(const DtIoStubBcTSRXFMT*, 
                                                 const DtIoctlTsRxFmtCmdSetPckModeInput*);
static DtStatus DtIoStubBcTSRXFMT_OnCmdSetSyncMode(const DtIoStubBcTSRXFMT*, 
                                                const DtIoctlTsRxFmtCmdSetSyncModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_TSRXFMT;

static const DtIoctlProperties  IOTSRXFMT_STUB_TSRXFMT_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,
        NULL, NULL),
    DT_IOCTL_PROPS_TSRXFMT_CMD(
        DtIoStubBcTSRXFMT_OnCmd, 
        NULL, NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcTSRXFMT - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTSRXFMT_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcTSRXFMT_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcTSRXFMT));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTSRXFMT_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcTSRXFMT*  DtIoStubBcTSRXFMT_Open(DtBc*  pBc)
{
    DtIoStubBcTSRXFMT*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    BC_TSRXFMT_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcTSRXFMT, pBc, NULL, 
                                                           DtIoStubBcTSRXFMT_Close,
                                                           NULL,  // Use default IOCTL
                                                           IOTSRXFMT_STUB_TSRXFMT_IOCTLS);
    pStub = (DtIoStubBcTSRXFMT*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcTSRXFMT - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTSRXFMT_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcTSRXFMT_OnCmd(
    const DtIoStub*  pStub, 
    DtIoStubIoParams*  pIoParams, 
    Int* pOutSize)
{
    DtStatus Status= DT_STATUS_OK;
    const DtIoctlTsRxFmtCmdInput*  pInData = NULL;
    DtIoctlTsRxFmtCmdOutput*  pOutData = NULL;
    
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcTSRXFMT));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_TSRXFMT_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, TSRXFMT, pStub, "ERROR: function is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_TsRxFmtCmd;
    if (pIoParams->m_OutReqSize > 0)
    { 
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_TsRxFmtCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_TSRXFMT_CMD_GET_OPERATIONAL_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcTSRXFMT_OnCmdGetOperationalStatus(TSRXFMT_STUB, 
                                                                &pOutData->m_GetOpStatus);
        break;
    case DT_TSRXFMT_CMD_GET_PACKET_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcTSRXFMT_OnCmdGetPacketMode(TSRXFMT_STUB,
                                                              &pOutData->m_GetPacketMode);
        break;
    case DT_TSRXFMT_CMD_GET_PACKET_SIZE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcTSRXFMT_OnCmdGetPacketSize(TSRXFMT_STUB, 
                                                              &pOutData->m_GetPacketSize);
        break;
    case DT_TSRXFMT_CMD_GET_SYNC_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcTSRXFMT_OnCmdGetSyncMode(TSRXFMT_STUB, 
                                                                &pOutData->m_GetSyncMode);
        break;
    case DT_TSRXFMT_CMD_SET_OPERATIONAL_MODE:
        Status = DtIoStubBcTSRXFMT_OnCmdSetOperationalMode(TSRXFMT_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_TSRXFMT_CMD_SET_PACKET_MODE:
        Status = DtIoStubBcTSRXFMT_OnCmdSetPacketMode(TSRXFMT_STUB,
                                                                  &pInData->m_SetPckMode);
        break;
    case DT_TSRXFMT_CMD_SET_SYNC_MODE:
        Status = DtIoStubBcTSRXFMT_OnCmdSetSyncMode(TSRXFMT_STUB, 
                                                                 &pInData->m_SetSyncMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTSRXFMT_OnCmdGetOperationalStatus -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcTSRXFMT_OnCmdGetOperationalStatus(
    const DtIoStubBcTSRXFMT*  pStub, 
    DtIoctlTsRxFmtCmdGetOpStatusOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcTSRXFMT));
    DT_ASSERT(pOutData != NULL);

    return DtBcTSRXFMT_GetOperationalStatus(TSRXFMT_BC, &pOutData->m_OpStatus);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTSRXFMT_OnCmdGetPacketMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcTSRXFMT_OnCmdGetPacketMode(
    const DtIoStubBcTSRXFMT*  pStub, 
    DtIoctlTsRxFmtCmdGetPckModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcTSRXFMT));
    DT_ASSERT(pOutData != NULL);

    return DtBcTSRXFMT_GetPacketMode(TSRXFMT_BC, &pOutData->m_PckMode);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTSRXFMT_OnCmdGetPacketSize -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcTSRXFMT_OnCmdGetPacketSize(
    const DtIoStubBcTSRXFMT*  pStub, 
    DtIoctlTsRxFmtCmdGetPacketSizeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcTSRXFMT));
    DT_ASSERT(pOutData != NULL);

    return DtBcTSRXFMT_GetTsPacketSize(TSRXFMT_BC, &pOutData->m_PacketSize);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTSRXFMT_OnCmdGetSyncMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcTSRXFMT_OnCmdGetSyncMode(
    const DtIoStubBcTSRXFMT*  pStub, 
    DtIoctlTsRxFmtCmdGetSyncModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcTSRXFMT));
    DT_ASSERT(pOutData != NULL);

    return DtBcTSRXFMT_GetSyncMode(TSRXFMT_BC, &pOutData->m_SyncMode);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTSRXFMT_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcTSRXFMT_OnCmdSetOperationalMode(
    const DtIoStubBcTSRXFMT*  pStub, 
    const DtIoctlTsRxFmtCmdSetOpModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcTSRXFMT));
    DT_ASSERT(pInData != NULL);

    return DtBcTSRXFMT_SetOperationalMode(TSRXFMT_BC, pInData->m_OpMode);
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTSRXFMT_OnCmdSetPacketMode -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcTSRXFMT_OnCmdSetPacketMode(
    const DtIoStubBcTSRXFMT*  pStub, 
    const DtIoctlTsRxFmtCmdSetPckModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcTSRXFMT));
    DT_ASSERT(pInData != NULL);

    return DtBcTSRXFMT_SetPacketMode(TSRXFMT_BC, pInData->m_PckMode);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcTSRXFMT_OnCmdSetSyncMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcTSRXFMT_OnCmdSetSyncMode(
    const DtIoStubBcTSRXFMT*  pStub, 
    const DtIoctlTsRxFmtCmdSetSyncModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcTSRXFMT));
    DT_ASSERT(pInData != NULL);

    return DtBcTSRXFMT_SetSyncMode(TSRXFMT_BC, pInData->m_SyncMode);
}