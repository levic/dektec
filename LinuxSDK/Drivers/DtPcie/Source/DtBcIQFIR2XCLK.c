// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQFIR2XCLK.c *#*#*#*#*#*#*#*#*#* (C) 2022 DekTec
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
#include "DtBcIQFIR2XCLK.h"
#include "DtBcIQFIR2XCLK_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQFIR2XCLK implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQFIR2XCLK_DEFAULT_PRECONDITIONS(pBc)      \
                               DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQFIR2XCLK))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQFIR2XCLK_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQFIR2XCLK, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQFIR2XCLK_Init(DtBc*);
static DtStatus  DtBcIQFIR2XCLK_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIQFIR2XCLK_OnEnable(DtBc*, Bool Enable);
static void  DtBcIQFIR2XCLK_SetControlRegs(DtBcIQFIR2XCLK*, Bool BlkEna, Int OpMode,
                                                                             Bool Bypass);
DtStatus DtBcIQFIR2XCLK_SetCoefControl(DtBcIQFIR2XCLK* pBc, Bool ClearFifo,  Bool Start, 
                                                                           Int Direction);

// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQFIR2XCLK - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR2XCLK_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcIQFIR2XCLK_Close(DtBc*  pBc)
{
    BC_IQFIR2XCLK_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR2XCLK_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIQFIR2XCLK*  DtBcIQFIR2XCLK_Open(Int  Address, DtCore* pCore, DtPt* pPt,
    const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_IQFIR2XCLK_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQFIR2XCLK, Id, DT_BLOCK_TYPE_IQFIR2XCLK,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQFIR2XCLK_Close;
    OpenParams.m_InitFunc = DtBcIQFIR2XCLK_Init;
    OpenParams.m_OnEnableFunc = DtBcIQFIR2XCLK_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIQFIR2XCLK_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQFIR2XCLK*)DtBc_Open(&OpenParams);
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR2XCLK_GetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQFIR2XCLK_GetConfig(DtBcIQFIR2XCLK* pBc, Int* pNumCoefs, Int* pCoefSize)
{
    // Sanity check
    BC_IQFIR2XCLK_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pNumCoefs==NULL || pCoefSize==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return cached configuration
    *pNumCoefs = (pBc->m_NumberTaps%2)==0 ? pBc->m_NumberTaps/2 : (pBc->m_NumberTaps+1)/2;
    *pCoefSize = pBc->m_CoefficientSize;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR2XCLK_GetCoefficients -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQFIR2XCLK_GetCoefficients(DtBcIQFIR2XCLK* pBc, Int* pCoeffs, Int NumToGet)
{
    DtStatus  Status = DT_STATUS_OK;
    Int i, NumShift, NumCoefficients;

    // Sanity check
    BC_IQFIR2XCLK_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    // number of coefficients to read or write to IqFirFilter2xClk is 
    // NumCoefs/2 if NumCoefs is even, or (NumCoefs+1)/2 if NumCoefs is odd.
    NumCoefficients = (pBc->m_NumberTaps%2)==0 ? pBc->m_NumberTaps/2 :
                                                 (pBc->m_NumberTaps+1)/2;
    if (pCoeffs==NULL || ( NumToGet!=NumCoefficients))
        return DT_STATUS_INVALID_PARAMETER;

    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Clear FIFO
    Status = DtBcIQFIR2XCLK_SetCoefControl(pBc, TRUE, FALSE, IQFIR2XCLK_CMD_READ);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, IQFIR2XCLK, pBc, "Clear FIFO failed");
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return Status;
    }

    // Start transfer of filter data
    Status = DtBcIQFIR2XCLK_SetCoefControl(pBc, FALSE, TRUE, IQFIR2XCLK_CMD_READ);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, IQFIR2XCLK, pBc, "Start transfer failed");
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return Status;
    }

    // Read coefficients (convert to signed integers)
    NumShift = (sizeof(Int)*8) - pBc->m_CoefficientSize;
    for (i=0; i<NumToGet; i++)
    {
        // Read with sign extension
        UInt32  Coef = IQFIR2XCLK_CoefReadFifo_READ(pBc);
        pCoeffs[i] = ((Int)Coef << NumShift) >> NumShift;
    }

    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR2XCLK_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQFIR2XCLK_GetOperationalMode(DtBcIQFIR2XCLK* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IQFIR2XCLK_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQFIR2XCLK_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR2XCLK_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQFIR2XCLK_SetOperationalMode(DtBcIQFIR2XCLK* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_IQFIR2XCLK_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
                                                         && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQFIR2XCLK_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcIQFIR2XCLK_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                                   FALSE);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR2XCLK_SetCoefficients -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQFIR2XCLK_SetCoefficients(DtBcIQFIR2XCLK* pBc, const Int* pCoeffs,
                                                                             Int NumToSet)
{
    UInt32  Mask;
    Int  i, NumCoefficients;
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    BC_IQFIR2XCLK_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    // number of coefficients to read or write to IqFirFilter2xClk is 
    // NumCoefs/2 if NumCoefs is even, or (NumCoefs+1)/2 if NumCoefs is odd.
    NumCoefficients = (pBc->m_NumberTaps%2)==0 ? pBc->m_NumberTaps/2 :
                                                 (pBc->m_NumberTaps+1)/2;

    if (pCoeffs==NULL || NumToSet!=NumCoefficients)
        return DT_STATUS_INVALID_PARAMETER;

    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Clear FIFO
    Status = DtBcIQFIR2XCLK_SetCoefControl(pBc, TRUE, FALSE, IQFIR2XCLK_CMD_WRITE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, IQFIR2XCLK, pBc, "Clear FIFO failed");
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return Status;
    }

    // Write coefficients (apply mask)
    Mask = (1 << pBc->m_CoefficientSize) - 1;
    for (i=0; i<NumToSet; i++)
        IQFIR2XCLK_CoefWriteFifo_WRITE(pBc, pCoeffs[i] & Mask);

    // Start transfer of filter data
    Status = DtBcIQFIR2XCLK_SetCoefControl(pBc, FALSE, TRUE, IQFIR2XCLK_CMD_WRITE);
    if (!DT_SUCCESS(Status))
        DtDbgOutBc(ERR, IQFIR2XCLK, pBc, "Start transfer failed");

    DtFastMutexRelease(&pBc->m_AccessMutex);
    return Status;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQFIR2XCLK - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR2XCLK_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQFIR2XCLK_Init(DtBc* pBcBase)
{
    DtBcIQFIR2XCLK* pBc = (DtBcIQFIR2XCLK*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    // Set control register
    DtBcIQFIR2XCLK_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                                                                   FALSE);

    // Read config
    pBc->m_NumberTaps = IQFIR2XCLK_Config_READ_NumCoefs(pBc);
    pBc->m_CoefficientSize =  IQFIR2XCLK_Config_READ_CoefBitDepth(pBc);

    // Init mutex
    DtFastMutexInit(&pBc->m_AccessMutex);
    
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR2XCLK_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQFIR2XCLK_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQFIR2XCLK* pBc = (DtBcIQFIR2XCLK*)pBcBase;

    // Sanity check
    BC_IQFIR2XCLK_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IQFIR2XCLK, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IQFIR2XCLK, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcIQFIR2XCLK_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQFIR2XCLK, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIQFIR2XCLK_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode,
                                                                                   FALSE);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR2XCLK_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQFIR2XCLK_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQFIR2XCLK* pBc = (DtBcIQFIR2XCLK*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IQFIR2XCLK_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IQFIR2XCLK, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIQFIR2XCLK_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQFIR2XCLK, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR2XCLK_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQFIR2XCLK_SetControlRegs(DtBcIQFIR2XCLK* pBc, Bool BlkEna, Int OpMode,
                                                                              Bool Bypass)
{
    UInt32  RegData = 0, FwBlkEna = 0, FwOpMode = 0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IQFIR2XCLK_BLKENA_ENABLED : IQFIR2XCLK_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
        case DT_BLOCK_OPMODE_IDLE:    FwOpMode = IQFIR2XCLK_OPMODE_IDLE; break;
        case DT_BLOCK_OPMODE_STANDBY: FwOpMode = IQFIR2XCLK_OPMODE_STANDBY; break;
        case DT_BLOCK_OPMODE_RUN:     FwOpMode = IQFIR2XCLK_OPMODE_RUN; break;
        default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = IQFIR2XCLK_Control_READ(pBc);
    RegData = IQFIR2XCLK_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IQFIR2XCLK_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = IQFIR2XCLK_Control_SET_Bypass(RegData,  Bypass ? 1 : 0);
    IQFIR2XCLK_Control_WRITE(pBc, RegData);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR2XCLK_SetCoefControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQFIR2XCLK_SetCoefControl(
    DtBcIQFIR2XCLK*  pBc,
    Bool  ClearFifo,
    Bool  Start, 
    Int  Direction)
{
    UInt32 FldReady, RegControl;
    Int i;

    // Parameters should be valid
    DT_ASSERT(ClearFifo || Start && !(ClearFifo && Start));
    DT_ASSERT(Direction==IQFIR2XCLK_CMD_READ || Direction==IQFIR2XCLK_CMD_WRITE);
    
    // Filter should be ready for transfer
    FldReady = IQFIR2XCLK_CoefControl_READ_Ready(pBc);
    if (FldReady != 1)
        return DT_STATUS_BUSY;

    // Set control
    RegControl = IQFIR2XCLK_CoefControl_SET_ClearFifo(0, ClearFifo ? 1 : 0);
    RegControl = IQFIR2XCLK_CoefControl_SET_Direction(RegControl, Direction);
    RegControl = IQFIR2XCLK_CoefControl_SET_Start(RegControl, Start ? 1 : 0);
    IQFIR2XCLK_CoefControl_WRITE(pBc, RegControl);

    // Wait for ready
    FldReady = IQFIR2XCLK_CoefControl_READ_Ready(pBc);
    for (i=0; i<10 && FldReady!=1; i++)
    {
        DtSleep(1);
        FldReady = IQFIR2XCLK_CoefControl_READ_Ready(pBc);
    }

    // Not ready?
    if (FldReady != 1)
        return DT_STATUS_TIMEOUT;
    else
        return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQFIR2XCLK implementation +=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IQFIR2XCLK_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                     DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR2XCLK))
// Helper macro to cast stub's DtBc member to DtBcIQFIR2XCLK
#define IQFIR2XCLK_STUB   ((DtIoStubBcIQFIR2XCLK*)pStub)
#define IQFIR2XCLK_BC  ((DtBcIQFIR2XCLK*)IQFIR2XCLK_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQFIR2XCLK_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams *);
static DtStatus  DtIoStubBcIQFIR2XCLK_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcIQFIR2XCLK_OnCmdGetOperationalMode(const DtIoStubBcIQFIR2XCLK*,
                                                    DtIoctlIqFir2xClkCmdGetOpModeOutput*);
static DtStatus DtIoStubBcIQFIR2XCLK_OnCmdGetConfig(const DtIoStubBcIQFIR2XCLK * , 
                                                    DtIoctlIqFir2xClkCmdGetConfigOutput*);
static DtStatus DtIoStubBcIQFIR2XCLK_OnCmdGetCoefficients(const DtIoStubBcIQFIR2XCLK*, 
                                                 const DtIoctlIqFir2xClkCmdGetCoefsInput*,
                                                     DtIoctlIqFir2xClkCmdGetCoefsOutput*);
static DtStatus DtIoStubBcIQFIR2XCLK_OnCmdSetCoefficients(const DtIoStubBcIQFIR2XCLK*,
                                                const DtIoctlIqFir2xClkCmdSetCoefsInput*);
static DtStatus  DtIoStubBcIQFIR2XCLK_OnCmdSetOperationalMode(const DtIoStubBcIQFIR2XCLK*,
                                               const DtIoctlIqFir2xClkCmdSetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQFIR2XCLK;

static const DtIoctlProperties  IOSTUB_BC_IQFIR2XCLK_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQFIR2XCLK_CMD(
        DtIoStubBcIQFIR2XCLK_OnCmd,
        DtIoStubBcIQFIR2XCLK_AppendDynamicSize, 
        NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQFIR2XCLK - Public functions +=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR2XCLK_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcIQFIR2XCLK_Close(DtIoStub* pStub)
{
    IQFIR2XCLK_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR2XCLK_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcIQFIR2XCLK*  DtIoStubBcIQFIR2XCLK_Open(DtBc* pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IQFIR2XCLK_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQFIR2XCLK, pBc, NULL,
        DtIoStubBcIQFIR2XCLK_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_IQFIR2XCLK_IOCTLS);
    return (DtIoStubBcIQFIR2XCLK*)DtIoStubBc_Open(&OpenParams);
}

// =+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQFIR2XCLK - Private functions +=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR2XCLK_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQFIR2XCLK_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqFir2xClkCmdInput*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR2XCLK));
    DT_ASSERT(pIoParams != NULL);                 
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQFIR2XCLK_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqFir2xClkCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_IQFIR2XCLK_CMD_GET_COEFS:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlIqFir2xClkCmdGetCoefsInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlIqFir2xClkCmdGetCoefsOutput));
        // Add dynamic size (i.e. #bytes to get)
        pIoParams->m_OutReqSize += (pInData->m_GetCoefs.m_NumCoefsToGet * sizeof(Int));
        break;

    case DT_IQFIR2XCLK_CMD_SET_COEFS:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlIqFir2xClkCmdSetCoefsInput));
        // Add dynamic size (i.e. #bytes to set)
        pIoParams->m_InReqSize += (pInData->m_SetCoefs.m_NumCoefsToSet * sizeof(Int));
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR2XCLK_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQFIR2XCLK_OnCmd(
    const  DtIoStub* pStub,
    DtIoStubIoParams*  pIoParams, 
    Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqFir2xClkCmdInput*  pInData = NULL;
    DtIoctlIqFir2xClkCmdOutput*  pOutData = NULL;

    IQFIR2XCLK_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQFIR2XCLK_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQFIR2XCLK, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqFir2xClkCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqFir2xClkCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IQFIR2XCLK_CMD_GET_CONFIG:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQFIR2XCLK_OnCmdGetConfig(IQFIR2XCLK_STUB, &pOutData->m_GetConfig);
        break;
    case DT_IQFIR2XCLK_CMD_GET_COEFS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQFIR2XCLK_OnCmdGetCoefficients(IQFIR2XCLK_STUB, &pInData->m_GetCoefs, 
                                                                   &pOutData->m_GetCoefs);
        break;
    case DT_IQFIR2XCLK_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQFIR2XCLK_OnCmdGetOperationalMode(IQFIR2XCLK_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_IQFIR2XCLK_CMD_SET_COEFS:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQFIR2XCLK_OnCmdSetCoefficients(IQFIR2XCLK_STUB, &pInData->m_SetCoefs);
        break;
    case DT_IQFIR2XCLK_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQFIR2XCLK_OnCmdSetOperationalMode(IQFIR2XCLK_STUB,
                                                                   &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR2XCLK_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQFIR2XCLK_OnCmdGetOperationalMode(
    const DtIoStubBcIQFIR2XCLK*  pStub,
    DtIoctlIqFir2xClkCmdGetOpModeOutput*  pOutData)
{
    IQFIR2XCLK_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQFIR2XCLK_GetOperationalMode(IQFIR2XCLK_BC, &pOutData->m_OpMode);
}

// .-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR2XCLK_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQFIR2XCLK_OnCmdSetOperationalMode(
    const DtIoStubBcIQFIR2XCLK*  pStub,
    const DtIoctlIqFir2xClkCmdSetOpModeInput*  pInData)
{
    IQFIR2XCLK_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQFIR2XCLK_SetOperationalMode(IQFIR2XCLK_BC, pInData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR2XCLK_OnCmdGetConfig -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQFIR2XCLK_OnCmdGetConfig(
    const DtIoStubBcIQFIR2XCLK* pStub,
    DtIoctlIqFir2xClkCmdGetConfigOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR2XCLK));
    DT_ASSERT(pOutData != NULL);

    return DtBcIQFIR2XCLK_GetConfig(IQFIR2XCLK_BC, &pOutData->m_NumberCoefs,
                                                                   &pOutData->m_CoefSize);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR2XCLK_OnCmdGetCoefficients -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQFIR2XCLK_OnCmdGetCoefficients(
    const DtIoStubBcIQFIR2XCLK* pStub,
    const DtIoctlIqFir2xClkCmdGetCoefsInput* pInData,
    DtIoctlIqFir2xClkCmdGetCoefsOutput* pOutData)
{
    DtStatus Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR2XCLK));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    Status = DtBcIQFIR2XCLK_GetCoefficients(IQFIR2XCLK_BC, pOutData->m_pCoefs, 
                                                                pInData->m_NumCoefsToGet);
    if (DT_SUCCESS(Status))
        pOutData->m_NumCoefs = pInData->m_NumCoefsToGet;
    else
        pOutData->m_NumCoefs = 0;

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR2XCLK_OnCmdSetCoefficients -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQFIR2XCLK_OnCmdSetCoefficients(
    const DtIoStubBcIQFIR2XCLK* pStub,
    const DtIoctlIqFir2xClkCmdSetCoefsInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR2XCLK));
    DT_ASSERT(pInData != NULL);

    return DtBcIQFIR2XCLK_SetCoefficients(IQFIR2XCLK_BC, pInData->m_pCoefs, 
                                                                pInData->m_NumCoefsToSet);
}

