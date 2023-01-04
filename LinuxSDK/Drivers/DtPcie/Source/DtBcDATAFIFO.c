// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcDATAFIFO.c *#*#*#*#*#*#*#*#*#* (C) 2022 DekTec
//
// This file is not part of the PCIe driver. It can be used as template to create a new
// block controller. Replace "DATAFIFO" by the building block's shortname (all uppercase)
// and replace "DataDdrFifo" by the building block's shortname in camel case. 
// 

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2022 DekTec Digital Video B.V.
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
#include "DtBcDATAFIFO.h"
#include "DtBcDATAFIFO_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDATAFIFO implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_DATAFIFO_DEFAULT_PRECONDITIONS(pBc)      \
                                DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcDATAFIFO))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_DATAFIFO_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(DATAFIFO, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcDATAFIFO_Init(DtBc*);
static DtStatus  DtBcDATAFIFO_OnEnable(DtBc*, Bool  Enable);
void  DtBcDATAFIFO_SetControlRegs(DtBcDATAFIFO*, Bool BlkEna, Int OpMode);


// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDATAFIFO - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcDATAFIFO_Close(DtBc*  pBc)
{
    BC_DATAFIFO_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcDATAFIFO*  DtBcDATAFIFO_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_DATAFIFO_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcDATAFIFO, Id, DT_BLOCK_TYPE_DATAFIFO,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcDATAFIFO_Close;
    OpenParams.m_InitFunc = DtBcDATAFIFO_Init;
    OpenParams.m_OnEnableFunc = DtBcDATAFIFO_OnEnable;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcDATAFIFO*)DtBc_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_ClearFifoMax -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDATAFIFO_ClearFifoMax(DtBcDATAFIFO* pBc, Bool ClrMaxFree, Bool ClrMaxLoad)
{
    // Sanity check
    BC_DATAFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_DATAFIFO_MUST_BE_ENABLED(pBc);

    // Clear maximum free-space 
    if (ClrMaxFree)
    { 
        UInt32 RegData = DATAFIFO_FreeSpaceMax_SET_Clear(0, 1);
        DATAFIFO_FreeSpaceMax_WRITE(pBc, RegData);
    }
    // Clear maximum load
    if (ClrMaxLoad)
    {
        UInt32 RegData = DATAFIFO_LoadMax_SET_Clear(0, 1);
        DATAFIFO_LoadMax_WRITE(pBc, RegData);
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_GetDirection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDATAFIFO_GetDirection(DtBcDATAFIFO* pBc, Int* pDirection)
{
    // Sanity check
    BC_DATAFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pDirection == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_DATAFIFO_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pDirection = pBc->m_Direction;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_GetFifoStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDATAFIFO_GetFifoStatus(DtBcDATAFIFO* pBc, Int* pCurLoad,Int* pMaxFree, 
                                                                            Int* pMaxLoad)
{
    // Sanity check
    BC_DATAFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pCurLoad==NULL || pMaxFree==NULL || pMaxLoad==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_DATAFIFO_MUST_BE_ENABLED(pBc);

    // Read registers
    *pCurLoad = (Int)DATAFIFO_FifoLoad_READ(pBc);
    *pMaxFree = (Int)DATAFIFO_FreeSpaceMax_READ_Maximum(pBc);
    *pMaxLoad = (Int)DATAFIFO_LoadMax_READ_Maximum(pBc);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcDATAFIFO_GetOperationalMode(DtBcDATAFIFO* pBc, Int* pOpMode)
{
    // Sanity check
    BC_DATAFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_DATAFIFO_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_GetUnderflowCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDATAFIFO_GetUnderflowCount(DtBcDATAFIFO* pBc, UInt32* pUflCount)
{
    // Sanity check
    BC_DATAFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pUflCount == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_DATAFIFO_MUST_BE_ENABLED(pBc);

    // Read register
    *pUflCount = DATAFIFO_UnderflowCnt_READ(pBc);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_GetOverflowCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDATAFIFO_GetOverflowCount(DtBcDATAFIFO* pBc, UInt32* pOvlCount)
{
    // Sanity check
    BC_DATAFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOvlCount == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_DATAFIFO_MUST_BE_ENABLED(pBc);

    // Read register
    *pOvlCount = DATAFIFO_OverflowCnt_READ(pBc);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_GetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDATAFIFO_GetProperties(DtBcDATAFIFO* pBc, UInt32* pCapabilities, 
                                      Int* pBurstSize, Int* pMaxFifoSize, Int* pDataWidth)
{
    // Sanity check
    BC_DATAFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pCapabilities== NULL || pBurstSize ==NULL || pMaxFifoSize==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_DATAFIFO_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pCapabilities = pBc->m_Capabilities;
    *pBurstSize = pBc->m_BurstSize;
    *pMaxFifoSize = pBc->m_MaxFifoSize;
    *pDataWidth = pBc->m_DataWidth;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_SetDirection -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// 
DtStatus DtBcDATAFIFO_SetDirection(DtBcDATAFIFO* pBc, Int Direction)
{
    // Sanity check
    BC_DATAFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (Direction!=DT_DATAFIFO_DIR_RX && Direction!=DT_DATAFIFO_DIR_TX)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_DATAFIFO_MUST_BE_ENABLED(pBc);

    // Must be IDLE
    if(pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
        return DT_STATUS_INVALID_IN_OPMODE;

    // Check capability
    if (Direction==DT_DATAFIFO_DIR_RX && (pBc->m_Capabilities&DT_DATAFIFO_CAP_RX)==0)
        return DT_STATUS_NOT_SUPPORTED;
    if (Direction==DT_DATAFIFO_DIR_TX && (pBc->m_Capabilities&DT_DATAFIFO_CAP_TX)==0)
        return DT_STATUS_NOT_SUPPORTED;

    // No change?
    if (pBc->m_Direction == Direction)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_Direction = Direction;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcDATAFIFO_SetOperationalMode(DtBcDATAFIFO* pBc, Int OpMode)
{
    // Sanity check
    BC_DATAFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
        && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_DATAFIFO_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcDATAFIFO_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);
    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_SetFifoAddr -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcDATAFIFO_SetFifoAddr(DtBcDATAFIFO* pBc, UInt32 FifoAddr)
{
    BC_DATAFIFO_DEFAULT_PRECONDITIONS(pBc);
    DATAFIFO_FifoAddr_WRITE(pBc, FifoAddr);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_SetFifoSizer -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcDATAFIFO_SetFifoSize(DtBcDATAFIFO* pBc, UInt32 FifoSize)
{
    BC_DATAFIFO_DEFAULT_PRECONDITIONS(pBc);
    DATAFIFO_FifoSize_WRITE(pBc, FifoSize);
    return DT_STATUS_OK;
}


// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDATAFIFO - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcDATAFIFO_Init(DtBc* pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcDATAFIFO* pBc = (DtBcDATAFIFO*)pBcBase;
    UInt32  RegConfig;

    // Read configuration registers
    RegConfig = DATAFIFO_Config_READ(pBc);
    pBc->m_DataWidth = DATAFIFO_Config_GET_MemDataWidth(RegConfig);
    // Size of a single burst
    pBc->m_BurstSize = DATAFIFO_Config_GET_BurstSize(RegConfig);
    // Max number of burst transfers
    pBc->m_MaxFifoSize = DATAFIFO_Config_GET_MaxFifoSize(RegConfig);

    // Set TX capability
    // Note: for now we only support TX
    pBc->m_Capabilities = DT_DATAFIFO_CAP_TX;
    pBc->m_Direction = DT_DATAFIFO_DIR_TX;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    // Set control register
    DtBcDATAFIFO_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDATAFIFO_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcDATAFIFO* pBc = (DtBcDATAFIFO*)pBcBase;

    // Sanity check
    BC_DATAFIFO_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, DATAFIFO, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
        pBc->m_Direction = DT_DATAFIFO_DIR_TX;
    } else {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, DATAFIFO, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcDATAFIFO_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, DATAFIFO, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcDATAFIFO_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDATAFIFO_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcDATAFIFO_SetControlRegs(DtBcDATAFIFO* pBc, Bool BlkEna, Int OpMode)
{
    UInt32  RegData=0, FwBlkEna=0, FwOpMode=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? DATAFIFO_BLKENA_ENABLED : DATAFIFO_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
        case DT_BLOCK_OPMODE_IDLE:    FwOpMode = DATAFIFO_OPMODE_IDLE; break;
        case DT_BLOCK_OPMODE_STANDBY: FwOpMode = DATAFIFO_OPMODE_STANDBY; break;
        case DT_BLOCK_OPMODE_RUN:     FwOpMode = DATAFIFO_OPMODE_RUN; break;
        default: DT_ASSERT(FALSE);
    }
   
    // Update control register
    RegData = DATAFIFO_Control_READ(pBc);
    RegData = DATAFIFO_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = DATAFIFO_Control_SET_OperationalMode(RegData, FwOpMode);
    DATAFIFO_Control_WRITE(pBc, RegData);
}

