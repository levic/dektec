// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcBURSTFIFO.c *#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
//
// This file is not part of the PCIe driver. It can be used as template to create a new
// block controller. Replace "BURSTFIFO" by the building block's shortname (all uppercase)
// and replace "BurstFifo" by the building block's shortname in camel case. 
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
#include "DtBcBURSTFIFO.h"
#include "DtBcBURSTFIFO_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcBURSTFIFO implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_BURSTFIFO_DEFAULT_PRECONDITIONS(pBc)      \
                                DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcBURSTFIFO))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_BURSTFIFO_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(BURSTFIFO, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcBURSTFIFO_Init(DtBc*);
static DtStatus  DtBcBURSTFIFO_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcBURSTFIFO_OnEnable(DtBc*, Bool  Enable);
void  DtBcBURSTFIFO_SetControlRegs(DtBcBURSTFIFO*, Bool BlkEna, Int OpMode, Int Direction,
                                                                         Int FlowControl);

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcBURSTFIFO - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcBURSTFIFO_Close(DtBc*  pBc)
{
    BC_BURSTFIFO_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcBURSTFIFO*  DtBcBURSTFIFO_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_BURSTFIFO_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcBURSTFIFO, Id, DT_BLOCK_TYPE_BURSTFIFO,
                           Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcBURSTFIFO_Close;
    OpenParams.m_InitFunc = DtBcBURSTFIFO_Init;
    OpenParams.m_OnEnableFunc = DtBcBURSTFIFO_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcBURSTFIFO_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcBURSTFIFO*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_ClearFifoMax -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcBURSTFIFO_ClearFifoMax(DtBcBURSTFIFO* pBc, Bool ClrMaxFree, Bool ClrMaxLoad)
{
    // Sanity check
    BC_BURSTFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_BURSTFIFO_MUST_BE_ENABLED(pBc);

    // Clear maximum free-space 
    if (ClrMaxFree)
    { 
        UInt32 RegData = BURSTFIFO_FreeSpaceMax_SET_Clear(0, 1);
        BURSTFIFO_FreeSpaceMax_WRITE(pBc, RegData);
    }
    // Clear maximum load
    if (ClrMaxLoad)
    {
        UInt32 RegData = BURSTFIFO_LoadMax_SET_Clear(0, 1);
        BURSTFIFO_LoadMax_WRITE(pBc, RegData);
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_GetDirection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcBURSTFIFO_GetDirection(DtBcBURSTFIFO* pBc, Int* pDirection)
{
    // Sanity check
    BC_BURSTFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pDirection == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_BURSTFIFO_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pDirection = pBc->m_Direction;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_GetFifoStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcBURSTFIFO_GetFifoStatus(DtBcBURSTFIFO* pBc, Int* pCurFree, Int* pCurLoad,
                                                             Int* pMaxFree, Int* pMaxLoad)
{
    // Sanity check
    BC_BURSTFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pCurFree==NULL || pCurLoad==NULL || pMaxFree==NULL || pMaxLoad==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_BURSTFIFO_MUST_BE_ENABLED(pBc);

    // Read registers
    *pCurFree = (Int)BURSTFIFO_FreeSpace_READ(pBc) * pBc->m_NumBytesPerDataWord;
    *pCurLoad = (Int)BURSTFIFO_Load_READ(pBc) * pBc->m_NumBytesPerDataWord;
    *pMaxFree = (Int)BURSTFIFO_FreeSpaceMax_READ_Maximum(pBc)*pBc->m_NumBytesPerDataWord;
    *pMaxLoad = (Int)BURSTFIFO_LoadMax_READ_Maximum(pBc) * pBc->m_NumBytesPerDataWord;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcBURSTFIFO_GetOperationalMode(DtBcBURSTFIFO* pBc, Int* pOpMode)
{
    // Sanity check
    BC_BURSTFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_BURSTFIFO_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_GetOverUnderflowCount -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcBURSTFIFO_GetOverUnderflowCount(DtBcBURSTFIFO* pBc, UInt32* pOvflUflCount)
{
    // Sanity check
    BC_BURSTFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOvflUflCount == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_BURSTFIFO_MUST_BE_ENABLED(pBc);

    // Read register
    *pOvflUflCount = BURSTFIFO_OverUnderflowCnt_READ(pBc) * pBc->m_NumBytesPerDataWord;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_GetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcBURSTFIFO_GetProperties(DtBcBURSTFIFO* pBc, UInt32* pCapabilities, 
                                                     Int* pDataWidth, Int* pBurstFifoSize)
{
    // Sanity check
    BC_BURSTFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pCapabilities== NULL || pDataWidth==NULL || pBurstFifoSize==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_BURSTFIFO_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pCapabilities = pBc->m_Capabilities;
    *pDataWidth = pBc->m_DataWidth;
    *pBurstFifoSize = pBc->m_BurstFifoSize;
    
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_GetRxFlowControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcBURSTFIFO_GetRxFlowControl(DtBcBURSTFIFO* pBc, Int* pFlowControl)
{
    // Sanity check
    BC_BURSTFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pFlowControl == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_BURSTFIFO_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pFlowControl = pBc->m_RxFlowControl;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_SetDirection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcBURSTFIFO_SetDirection(DtBcBURSTFIFO* pBc, Int Direction)
{
    // Sanity check
    BC_BURSTFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (Direction!=DT_BURSTFIFO_DIR_RX && Direction!=DT_BURSTFIFO_DIR_TX)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_BURSTFIFO_MUST_BE_ENABLED(pBc);

    // Must be IDLE
    if(pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Check capability
    if (Direction==DT_BURSTFIFO_DIR_RX && (pBc->m_Capabilities&DT_BURSTFIFO_CAP_RX)==0)
        return DT_STATUS_NOT_SUPPORTED;
    if (Direction==DT_BURSTFIFO_DIR_TX && (pBc->m_Capabilities&DT_BURSTFIFO_CAP_TX)==0)
        return DT_STATUS_NOT_SUPPORTED;

    // No change?
    if (pBc->m_Direction == Direction)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_Direction = Direction;
    // Set control register
    DtBcBURSTFIFO_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                  pBc->m_Direction, pBc->m_RxFlowControl);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcBURSTFIFO_SetOperationalMode(DtBcBURSTFIFO* pBc, Int OpMode)
{
    // Sanity check
    BC_BURSTFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
        && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_BURSTFIFO_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcBURSTFIFO_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                  pBc->m_Direction, pBc->m_RxFlowControl);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_SetRxFlowControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcBURSTFIFO_SetRxFlowControl(DtBcBURSTFIFO* pBc, Int FlowControl)
{
    // Sanity check
    BC_BURSTFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (FlowControl!=DT_BURSTFIFO_FLOW_PUSHPULL 
                                               && FlowControl!=DT_BURSTFIFO_FLOW_VIRTPUSH)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_BURSTFIFO_MUST_BE_ENABLED(pBc);

    // Must be IDLE
    if(pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    // No change?
    if (pBc->m_RxFlowControl == FlowControl)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_RxFlowControl = FlowControl;
    // Set control register
    DtBcBURSTFIFO_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                  pBc->m_Direction, pBc->m_RxFlowControl);

    return DT_STATUS_OK;
}

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcBURSTFIFO - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcBURSTFIFO_Init(DtBc* pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcBURSTFIFO* pBc = (DtBcBURSTFIFO*)pBcBase;
    UInt32  RegConfig;

    // Read configuration registers
    RegConfig = BURSTFIFO_Config_READ(pBc);
    // Get RX/TX capabilities
    pBc->m_Capabilities = 0;
    if (BURSTFIFO_Config_GET_RxTxCapability(RegConfig) == BURSTFIFO_RXTXCAP_RX_ONLY)
        pBc->m_Capabilities = DT_BURSTFIFO_CAP_RX;
    else if (BURSTFIFO_Config_GET_RxTxCapability(RegConfig) == BURSTFIFO_RXTXCAP_TX_ONLY)
        pBc->m_Capabilities = DT_BURSTFIFO_CAP_TX;
    else if (BURSTFIFO_Config_GET_RxTxCapability(RegConfig) == BURSTFIFO_RXTXCAP_RX_OR_TX)
        pBc->m_Capabilities = DT_BURSTFIFO_CAP_RX | DT_BURSTFIFO_CAP_TX;
    // Get data width (#bits)
    pBc->m_DataWidth = (Int)BURSTFIFO_Config_GET_DataWidth(RegConfig);
    pBc->m_NumBytesPerDataWord = pBc->m_DataWidth / 8;
    // Get BurstFifoSize and convert it to #bytes
    pBc->m_BurstFifoSize = (Int)BURSTFIFO_Config_GET_FifoSize(RegConfig) 
                                                             * pBc->m_NumBytesPerDataWord;
    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_Direction  = ((pBc->m_Capabilities&DT_BURSTFIFO_CAP_RX) != 0) ?
                                                 DT_BURSTFIFO_DIR_RX :DT_BURSTFIFO_DIR_TX;
    pBc->m_RxFlowControl = DT_BURSTFIFO_FLOW_VIRTPUSH;

    // Set control register
    DtBcBURSTFIFO_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                  pBc->m_Direction, pBc->m_RxFlowControl);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcBURSTFIFO_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcBURSTFIFO* pBc = (DtBcBURSTFIFO*)pBcBase;

    // Sanity check
    BC_BURSTFIFO_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, BURSTFIFO, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
        pBc->m_Direction = ((pBc->m_Capabilities&DT_BURSTFIFO_CAP_RX) != 0) ?
                                                 DT_BURSTFIFO_DIR_RX :DT_BURSTFIFO_DIR_TX;
        pBc->m_RxFlowControl = DT_BURSTFIFO_FLOW_VIRTPUSH;
    } else {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, BURSTFIFO, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcBURSTFIFO_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, BURSTFIFO, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcBURSTFIFO_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                  pBc->m_Direction, pBc->m_RxFlowControl);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcBURSTFIFO_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcBURSTFIFO* pBc = (DtBcBURSTFIFO*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_BURSTFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, BURSTFIFO, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcBURSTFIFO_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, BURSTFIFO, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcBURSTFIFO_SetControlRegs(DtBcBURSTFIFO* pBc, Bool BlkEna, Int OpMode,
                                                           Int Direction, Int FlowControl)
{
    UInt32  RegData=0, FwBlkEna=0, FwOpMode=0, FwDir=0, FwFlCtrl=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? BURSTFIFO_BLKENA_ENABLED : BURSTFIFO_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
        case DT_BLOCK_OPMODE_IDLE:    FwOpMode = BURSTFIFO_OPMODE_IDLE; break;
        case DT_BLOCK_OPMODE_STANDBY: FwOpMode = BURSTFIFO_OPMODE_STANDBY; break;
        case DT_BLOCK_OPMODE_RUN:     FwOpMode = BURSTFIFO_OPMODE_RUN; break;
        default: DT_ASSERT(FALSE);
    }
    // Convert direction to BB-type
    switch (Direction)
    {
        case DT_BURSTFIFO_DIR_RX:    FwDir = BURSTFIFO_DIR_RX; break;
        case DT_BURSTFIFO_DIR_TX:    FwDir = BURSTFIFO_DIR_TX; break;
        default: DT_ASSERT(FALSE);
    }
    // Convert flow-control to BB-type
    switch (FlowControl)
    {
        case DT_BURSTFIFO_FLOW_PUSHPULL:  FwFlCtrl = DT_BURSTFIFO_FLOW_PUSHPULL; break;
        case DT_BURSTFIFO_FLOW_VIRTPUSH:  FwFlCtrl = DT_BURSTFIFO_FLOW_VIRTPUSH; break;
        default: DT_ASSERT(FALSE);
    }
    // Update control register
    RegData = BURSTFIFO_Control_READ(pBc);
    RegData = BURSTFIFO_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = BURSTFIFO_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = BURSTFIFO_Control_SET_RxOrTx(RegData, FwDir);
    RegData = BURSTFIFO_Control_SET_RxInputFlowControl(RegData, FwFlCtrl);
    BURSTFIFO_Control_WRITE(pBc, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcBURSTFIFO implementation +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define BURSTFIFO_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                      DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcBURSTFIFO))
// Helper macro to cast stub's DtBc member to DtBcBURSTFIFO
#define BURSTFIFO_STUB   ((DtIoStubBcBURSTFIFO*)pStub)
#define BURSTFIFO_BC  ((DtBcBURSTFIFO*)BURSTFIFO_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcBURSTFIFO_AppendDynamicSize(const DtIoStub*,DtIoStubIoParams*);
static DtStatus  DtIoStubBcBURSTFIFO_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcBURSTFIFO_OnCmdClearFifoMax(const DtIoStubBcBURSTFIFO*,
                                         const DtIoctlBurstFifoCmdClearFifoMaxInput*);
static DtStatus  DtIoStubBcBURSTFIFO_OnCmdGetDirection(const DtIoStubBcBURSTFIFO*,
                                         DtIoctlBurstFifoCmdGetDirectionOutput*);
static DtStatus  DtIoStubBcBURSTFIFO_OnCmdGetFifoStatus(const DtIoStubBcBURSTFIFO*,
                                         DtIoctlBurstFifoCmdGetFifoStatusOutput*);
static DtStatus  DtIoStubBcBURSTFIFO_OnCmdGetOperationalMode(const DtIoStubBcBURSTFIFO*,
                                         DtIoctlBurstFifoCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcBURSTFIFO_OnCmdGetOverUnderflowCount(
                    const DtIoStubBcBURSTFIFO*, DtIoctlBurstFifoCmdGetOvfUflCountOutput*);
static DtStatus  DtIoStubBcBURSTFIFO_OnCmdGetProperties(const DtIoStubBcBURSTFIFO*,
                                         DtIoctlBurstFifoCmdGetPropertiesOutput*);
static DtStatus  DtIoStubBcBURSTFIFO_OnCmdGetRxFlowControl(const DtIoStubBcBURSTFIFO*,
                                         DtIoctlBurstFifoCmdGetRxFlowControlOutput*);
static DtStatus  DtIoStubBcBURSTFIFO_OnCmdSetDirection(const DtIoStubBcBURSTFIFO*,
                                         const DtIoctlBurstFifoCmdSetDirectionInput*);
static DtStatus  DtIoStubBcBURSTFIFO_OnCmdSetOperationalMode(const DtIoStubBcBURSTFIFO*,
                                         const DtIoctlBurstFifoCmdSetOpModeInput*);
static DtStatus  DtIoStubBcBURSTFIFO_OnCmdSetRxFlowControl(const DtIoStubBcBURSTFIFO*,
                                         const DtIoctlBurstFifoCmdSetRxFlowControlInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_BURSTFIFO;

static const DtIoctlProperties  IOSTUB_BC_BURSTFIFO_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_BURSTFIFO_CMD(
        DtIoStubBcBURSTFIFO_OnCmd,
        DtIoStubBcBURSTFIFO_AppendDynamicSize,
        NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcBURSTFIFO - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcBURSTFIFO_Close(DtIoStub* pStub)
{
    BURSTFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcBURSTFIFO*  DtIoStubBcBURSTFIFO_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_BURSTFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcBURSTFIFO, pBc, NULL,
                                 DtIoStubBcBURSTFIFO_Close,
                                 NULL,  // Use default IOCTL
                                 IOSTUB_BC_BURSTFIFO_IOCTLS);
    return (DtIoStubBcBURSTFIFO*)DtIoStubBc_Open(&OpenParams);
}

// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcBURSTFIFO - Private functions +=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcBURSTFIFO_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlBurstFifoCmdInput*  pInData = NULL;

    BURSTFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_BURSTFIFO_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_BurstFifoCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.
    switch (pIoParams->m_Cmd)
    {
        default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcBURSTFIFO_OnCmd(const DtIoStub*  pStub,
                                   DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlBurstFifoCmdInput*  pInData = NULL;
    DtIoctlBurstFifoCmdOutput*  pOutData = NULL;

    BURSTFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_BURSTFIFO_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
                                      &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, BURSTFIFO, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_BurstFifoCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_BurstFifoCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_BURSTFIFO_CMD_CLEAR_FIFO_MAX:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcBURSTFIFO_OnCmdClearFifoMax(BURSTFIFO_STUB,
                                                                &pInData->m_ClearFifoMax);

        break;
    case DT_BURSTFIFO_CMD_GET_DIRECTION:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcBURSTFIFO_OnCmdGetDirection(BURSTFIFO_STUB,
                                                               &pOutData->m_GetDirection);
        break;
    case DT_BURSTFIFO_CMD_GET_FIFO_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcBURSTFIFO_OnCmdGetFifoStatus(BURSTFIFO_STUB,
                                                              &pOutData->m_GetFifoStatus);
        break;
    case DT_BURSTFIFO_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcBURSTFIFO_OnCmdGetOperationalMode(BURSTFIFO_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_BURSTFIFO_CMD_GET_OVFL_UFL_COUNT:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcBURSTFIFO_OnCmdGetOverUnderflowCount(BURSTFIFO_STUB,
                                                               &pOutData->m_GetOvfUflCnt);
        break;
    case DT_BURSTFIFO_CMD_GET_PROPERTIES:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcBURSTFIFO_OnCmdGetProperties(BURSTFIFO_STUB,
                                                              &pOutData->m_GetProperties);
        break;
    case DT_BURSTFIFO_CMD_GET_RXFLOWCONTROL:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcBURSTFIFO_OnCmdGetRxFlowControl(BURSTFIFO_STUB,
                                                                &pOutData->m_GetRxFlCtrl);
        break;
    case DT_BURSTFIFO_CMD_SET_DIRECTION:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcBURSTFIFO_OnCmdSetDirection(BURSTFIFO_STUB,
                                                             &pInData->m_SetDirection);
        break;
    case DT_BURSTFIFO_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcBURSTFIFO_OnCmdSetOperationalMode(BURSTFIFO_STUB,
                                                             &pInData->m_SetOpMode);
        break;
    case DT_BURSTFIFO_CMD_SET_RXFLOWCONTROL:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcBURSTFIFO_OnCmdSetRxFlowControl(BURSTFIFO_STUB,
                                                                 &pInData->m_SetRxFlCtrl);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO_OnCmdClearFifoMax -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcBURSTFIFO_OnCmdClearFifoMax(
    const DtIoStubBcBURSTFIFO* pStub,
    const DtIoctlBurstFifoCmdClearFifoMaxInput* pInData)
{
    BURSTFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcBURSTFIFO_ClearFifoMax(BURSTFIFO_BC, pInData->m_ClearMaxFree!=0,
                                                              pInData->m_ClearMaxLoad!=0);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO_OnCmdGetDirection -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcBURSTFIFO_OnCmdGetDirection(
    const DtIoStubBcBURSTFIFO* pStub,
    DtIoctlBurstFifoCmdGetDirectionOutput* pOutData)
{
    BURSTFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcBURSTFIFO_GetDirection(BURSTFIFO_BC, &pOutData->m_Direction);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO_OnCmdGetFifoStatus -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcBURSTFIFO_OnCmdGetFifoStatus(
    const DtIoStubBcBURSTFIFO* pStub,
    DtIoctlBurstFifoCmdGetFifoStatusOutput* pOutData)
{
    BURSTFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcBURSTFIFO_GetFifoStatus(BURSTFIFO_BC, &pOutData->m_CurFree,
                        &pOutData->m_CurLoad, &pOutData->m_MaxFree, &pOutData->m_MaxLoad);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcBURSTFIFO_OnCmdGetOperationalMode(
    const DtIoStubBcBURSTFIFO* pStub,
    DtIoctlBurstFifoCmdGetOpModeOutput* pOutData)
{
    BURSTFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcBURSTFIFO_GetOperationalMode(BURSTFIFO_BC, &pOutData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO_OnCmdGetOverUnderflowCount -.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcBURSTFIFO_OnCmdGetOverUnderflowCount(
    const DtIoStubBcBURSTFIFO* pStub,
    DtIoctlBurstFifoCmdGetOvfUflCountOutput* pOutData)
{
    BURSTFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcBURSTFIFO_GetOverUnderflowCount(BURSTFIFO_BC, &pOutData->m_OvfUflCount);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO_OnCmdGetProperties -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcBURSTFIFO_OnCmdGetProperties(
    const DtIoStubBcBURSTFIFO* pStub,
    DtIoctlBurstFifoCmdGetPropertiesOutput* pOutData)
{
    BURSTFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcBURSTFIFO_GetProperties(BURSTFIFO_BC, &pOutData->m_Capabilities, 
                                      &pOutData->m_DataWidth, &pOutData->m_BurstFifoSize);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO_OnCmdGetRxFlowControl -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcBURSTFIFO_OnCmdGetRxFlowControl(
    const DtIoStubBcBURSTFIFO* pStub,
    DtIoctlBurstFifoCmdGetRxFlowControlOutput* pOutData)
{
    BURSTFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcBURSTFIFO_GetRxFlowControl(BURSTFIFO_BC, &pOutData->m_RxFlowControl);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO_OnCmdSetDirection -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcBURSTFIFO_OnCmdSetDirection(
    const DtIoStubBcBURSTFIFO* pStub,
    const DtIoctlBurstFifoCmdSetDirectionInput* pInData)
{
    BURSTFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcBURSTFIFO_SetDirection(BURSTFIFO_BC, pInData->m_Direction);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcBURSTFIFO_OnCmdSetOperationalMode(
    const DtIoStubBcBURSTFIFO* pStub,
    const DtIoctlBurstFifoCmdSetOpModeInput* pInData)
{
    BURSTFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcBURSTFIFO_SetOperationalMode(BURSTFIFO_BC, pInData->m_OpMode);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcBURSTFIFO_OnCmdSetRxFlowControl -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcBURSTFIFO_OnCmdSetRxFlowControl(
    const DtIoStubBcBURSTFIFO* pStub,
    const DtIoctlBurstFifoCmdSetRxFlowControlInput* pInData)
{
    BURSTFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcBURSTFIFO_SetDirection(BURSTFIFO_BC, pInData->m_RxFlowControl);
}
