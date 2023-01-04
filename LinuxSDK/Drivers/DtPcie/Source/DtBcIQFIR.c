//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQFIR.c *#*#*#*#*#*#*#*#*#*# (C) 2021 DekTec

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
#include "DtBcIQFIR.h"
#include "DtBcIQFIR_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQFIR implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQFIR_DEFAULT_PRECONDITIONS(pBc)      \
                                    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQFIR))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQFIR_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQFIR, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQFIR_Init(DtBc*);
static DtStatus  DtBcIQFIR_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIQFIR_OnEnable(DtBc*, Bool Enable);
static void  DtBcIQFIR_SetControlRegs(DtBcIQFIR*, Bool BlkEna, Int OpMode);
DtStatus  DtBcIQFIR_GetConfig(DtBcIQFIR* , Int* pNumCoefs, Int* pCoefSize);
DtStatus  DtBcIQFIR_GetCoefficients(DtBcIQFIR*, Int* pCoeffs, Int NumToGet);
DtStatus  DtBcIQFIR_SetCoefficients(DtBcIQFIR*, const Int* pCoeffs, Int NumToSet);
DtStatus DtBcIQFIR_SetCoefControl(DtBcIQFIR* pBc, Bool ClearFifo,  Bool Start, 
                                                                           Int Direction);
DtStatus DtBcIQFIR_SetPrescale(DtBcIQFIR* pBc, Int PrescaleValue);


// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQFIR - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcIQFIR_Close(DtBc*  pBc)
{
    BC_IQFIR_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcIQFIR*  DtBcIQFIR_Open(Int  Address, DtCore* pCore, DtPt* pPt,
    const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore != NULL && pCore->m_Size >= sizeof(DtCore));

    // Init open parameters
    DT_BC_IQFIR_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQFIR, Id, DT_BLOCK_TYPE_IQFIR,
        Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQFIR_Close;
    OpenParams.m_InitFunc = DtBcIQFIR_Init;
    OpenParams.m_OnEnableFunc = DtBcIQFIR_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIQFIR_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQFIR*)DtBc_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQFIR_GetOperationalMode(DtBcIQFIR* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IQFIR_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQFIR_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQFIR_SetOperationalMode(DtBcIQFIR* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_IQFIR_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode != DT_BLOCK_OPMODE_IDLE && OpMode != DT_BLOCK_OPMODE_STANDBY
                                                         && OpMode != DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQFIR_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcIQFIR_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_GetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQFIR_GetConfig(DtBcIQFIR* pBc, Int* pNumCoefs, Int* pCoefSize)
{
    // Sanity check
    BC_IQFIR_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pNumCoefs==NULL || pCoefSize==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return cached configuration
    *pNumCoefs = (pBc->m_NumberTaps%2)==0 ? pBc->m_NumberTaps/2 : (pBc->m_NumberTaps+1)/2;
    *pCoefSize = pBc->m_CoefficientSize;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_GetCoefficients -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQFIR_GetCoefficients(DtBcIQFIR* pBc, Int* pCoeffs, Int NumToGet)
{
    DtStatus  Status = DT_STATUS_OK;
    Int i, NumShift, NumCoefficients;

    // Sanity check
    BC_IQFIR_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    // number of coefficients to read or write to IqFirFilter is 
    // NumCoefs/2 if NumCoefs is even, or (NumCoefs+1)/2 if NumCoefs is odd.
    NumCoefficients = (pBc->m_NumberTaps%2)==0 ? pBc->m_NumberTaps/2 :
                                                 (pBc->m_NumberTaps+1)/2;
    if (pCoeffs==NULL || ( NumToGet!=NumCoefficients))
        return DT_STATUS_INVALID_PARAMETER;

    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Clear FIFO
    Status = DtBcIQFIR_SetCoefControl(pBc, TRUE, FALSE, IQFIR_CMD_READ);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, IQFIR, pBc, "Clear FIFO failed");
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return Status;
    }

    // Start transfer of filter data
    Status = DtBcIQFIR_SetCoefControl(pBc, FALSE, TRUE, IQFIR_CMD_READ);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, IQFIR, pBc, "Start transfer failed");
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return Status;
    }

    // Read coefficients (convert to signed integers)
    NumShift = (sizeof(Int)*8) - pBc->m_CoefficientSize;
    for (i=0; i<NumToGet; i++)
    {
        // Read with sign extension
        UInt32  Coef = IQFIR_CoefReadFifo_READ(pBc);
        pCoeffs[i] = ((Int)Coef << NumShift) >> NumShift;
    }

    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_SetCoefficients -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQFIR_SetCoefficients(DtBcIQFIR* pBc, const Int* pCoeffs, Int NumToSet)
{
    UInt32  Mask;
    Int  i, NumCoefficients;
    DtStatus  Status = DT_STATUS_OK;

    // Sanity check
    BC_IQFIR_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    // number of coefficients to read or write to IqFirFilter is 
    // NumCoefs/2 if NumCoefs is even, or (NumCoefs+1)/2 if NumCoefs is odd.
    NumCoefficients = (pBc->m_NumberTaps%2)==0 ? pBc->m_NumberTaps/2 :
                                                 (pBc->m_NumberTaps+1)/2;

    if (pCoeffs==NULL || NumToSet!=NumCoefficients)
        return DT_STATUS_INVALID_PARAMETER;

    DtFastMutexAcquire(&pBc->m_AccessMutex);

    // Clear FIFO
    Status = DtBcIQFIR_SetCoefControl(pBc, TRUE, FALSE, IQFIR_CMD_WRITE);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutBc(ERR, IQFIR, pBc, "Clear FIFO failed");
        DtFastMutexRelease(&pBc->m_AccessMutex);
        return Status;
    }

    // Write coefficients (apply mask)
    Mask = (1 << pBc->m_CoefficientSize) - 1;
    for (i=0; i<NumToSet; i++)
        IQFIR_CoefWriteFifo_WRITE(pBc, pCoeffs[i] & Mask);

    // Start transfer of filter data
    Status = DtBcIQFIR_SetCoefControl(pBc, FALSE, TRUE, IQFIR_CMD_WRITE);
    if (!DT_SUCCESS(Status))
        DtDbgOutBc(ERR, IQFIR, pBc, "Start transfer failed");

    DtFastMutexRelease(&pBc->m_AccessMutex);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_SetPrescale -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQFIR_SetPrescale(DtBcIQFIR* pBc, Int PrescaleValue)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_IQFIR_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (PrescaleValue < 0 || PrescaleValue > 32768)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQFIR_MUST_BE_ENABLED(pBc);

    // Set control register
    IQFIR_Prescale_WRITE(pBc, (UInt32)PrescaleValue);
    return Status;
}

// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQFIR - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQFIR_Init(DtBc* pBcBase)
{
    DtBcIQFIR* pBc = (DtBcIQFIR*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    // Set control register
    DtBcIQFIR_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    // Read config
    pBc->m_NumberTaps = IQFIR_Config_READ_NumCoefs(pBc);
    pBc->m_CoefficientSize =  IQFIR_Config_READ_CoefBitDepth(pBc);

    // Init mutex
    DtFastMutexInit(&pBc->m_AccessMutex);
    
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQFIR_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQFIR* pBc = (DtBcIQFIR*)pBcBase;

    // Sanity check
    BC_IQFIR_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IQFIR, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IQFIR, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcIQFIR_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQFIR, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIQFIR_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQFIR_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQFIR* pBc = (DtBcIQFIR*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IQFIR_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IQFIR, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIQFIR_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQFIR, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcIQFIR_SetControlRegs(DtBcIQFIR* pBc, Bool BlkEna, Int OpMode)
{
    UInt32  RegData = 0, FwBlkEna = 0, FwOpMode = 0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IQFIR_BLKENA_ENABLED : IQFIR_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
        case DT_BLOCK_OPMODE_IDLE:    FwOpMode = IQFIR_OPMODE_IDLE; break;
        case DT_BLOCK_OPMODE_STANDBY: FwOpMode = IQFIR_OPMODE_STANDBY; break;
        case DT_BLOCK_OPMODE_RUN:     FwOpMode = IQFIR_OPMODE_RUN; break;
        default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = IQFIR_Control_READ(pBc);
    RegData = IQFIR_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IQFIR_Control_SET_OperationalMode(RegData, FwOpMode);
    IQFIR_Control_WRITE(pBc, RegData);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQFIR_SetCoefControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQFIR_SetCoefControl(
    DtBcIQFIR*  pBc,
    Bool  ClearFifo,
    Bool  Start, 
    Int  Direction)
{
    UInt32 FldReady, RegControl;
    Int i;

    // Parameters should be valid
    DT_ASSERT(ClearFifo || Start && !(ClearFifo && Start));
    DT_ASSERT(Direction==IQFIR_CMD_READ || Direction==IQFIR_CMD_WRITE);
    
    // Filter should be ready for transfer
    FldReady = IQFIR_CoefControl_READ_Ready(pBc);
    if (FldReady != 1)
        return DT_STATUS_BUSY;

    // Set control
    RegControl = IQFIR_CoefControl_SET_ClearFifo(0, ClearFifo ? 1 : 0);
    RegControl = IQFIR_CoefControl_SET_Direction(RegControl, Direction);
    RegControl = IQFIR_CoefControl_SET_Start(RegControl, Start ? 1 : 0);
    IQFIR_CoefControl_WRITE(pBc, RegControl);

    // Wait for ready
    FldReady = IQFIR_CoefControl_READ_Ready(pBc);
    for (i=0; i<10 && FldReady!=1; i++)
    {
        DtSleep(1);
        FldReady = IQFIR_CoefControl_READ_Ready(pBc);
    }

    // Not ready?
    if (FldReady != 1)
        return DT_STATUS_TIMEOUT;
    else
        return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQFIR implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IQFIR_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR))
// Helper macro to cast stub's DtBc member to DtBcIQFIR
#define IQFIR_STUB   ((DtIoStubBcIQFIR*)pStub)
#define IQFIR_BC  ((DtBcIQFIR*)IQFIR_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQFIR_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams *);
static DtStatus  DtIoStubBcIQFIR_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcIQFIR_OnCmdGetOperationalMode(const DtIoStubBcIQFIR*,
                                                         DtIoctlIqFirCmdGetOpModeOutput*);
static DtStatus DtIoStubBcIQFIR_OnCmdGetConfig(const DtIoStubBcIQFIR * , 
                                                         DtIoctlIqFirCmdGetConfigOutput*);
static DtStatus DtIoStubBcIQFIR_OnCmdGetCoefficients(const DtIoStubBcIQFIR*, 
                                                      const DtIoctlIqFirCmdGetCoefsInput*,
                                                      DtIoctlIqFirCmdGetCoefsOutput*);
static DtStatus DtIoStubBcIQFIR_OnCmdSetCoefficients(const DtIoStubBcIQFIR*,
                                                     const DtIoctlIqFirCmdSetCoefsInput*);
static DtStatus  DtIoStubBcIQFIR_OnCmdSetOperationalMode(const DtIoStubBcIQFIR*,
                                                    const DtIoctlIqFirCmdSetOpModeInput*);
static DtStatus  DtIoStubBcIQFIR_OnCmdSetPrescale(const DtIoStubBcIQFIR*, 
                                                  const DtIoctlIqFirCmdSetPrescaleInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQFIR;

static const DtIoctlProperties  IOSTUB_BC_IQFIR_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQFIR_CMD(
        DtIoStubBcIQFIR_OnCmd,
        DtIoStubBcIQFIR_AppendDynamicSize, 
        NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQFIR - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcIQFIR_Close(DtIoStub* pStub)
{
    IQFIR_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcIQFIR*  DtIoStubBcIQFIR_Open(DtBc* pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IQFIR_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQFIR, pBc, NULL,
        DtIoStubBcIQFIR_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_IQFIR_IOCTLS);
    return (DtIoStubBcIQFIR*)DtIoStubBc_Open(&OpenParams);
}

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQFIR - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQFIR_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqFirCmdInput*  pInData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR));
    DT_ASSERT(pIoParams != NULL);                 
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQFIR_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqFirCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_IQFIR_CMD_GET_COEFS:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlIqFirCmdGetCoefsInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlIqFirCmdGetCoefsOutput));
        // Add dynamic size (i.e. #bytes to get)
        pIoParams->m_OutReqSize += (pInData->m_GetCoefs.m_NumCoefsToGet * sizeof(Int));
        break;

    case DT_IQFIR_CMD_SET_COEFS:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlIqFirCmdSetCoefsInput));
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

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQFIR_OnCmd(
    const  DtIoStub* pStub,
    DtIoStubIoParams*  pIoParams, 
    Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqFirCmdInput*  pInData = NULL;
    DtIoctlIqFirCmdOutput*  pOutData = NULL;

    IQFIR_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQFIR_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQFIR, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqFirCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqFirCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IQFIR_CMD_GET_CONFIG:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQFIR_OnCmdGetConfig(IQFIR_STUB, &pOutData->m_GetConfig);
        break;
    case DT_IQFIR_CMD_GET_COEFS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQFIR_OnCmdGetCoefficients(IQFIR_STUB, &pInData->m_GetCoefs, 
                                                                   &pOutData->m_GetCoefs);
        break;
    case DT_IQFIR_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQFIR_OnCmdGetOperationalMode(IQFIR_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_IQFIR_CMD_SET_COEFS:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQFIR_OnCmdSetCoefficients(IQFIR_STUB, &pInData->m_SetCoefs);
        break;
    case DT_IQFIR_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQFIR_OnCmdSetOperationalMode(IQFIR_STUB,
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_IQFIR_CMD_SET_PRESCALE:
        DT_ASSERT(pInData != NULL);

        Status = DtIoStubBcIQFIR_OnCmdSetPrescale(IQFIR_STUB,  &pInData->m_SetPrescale);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQFIR_OnCmdGetOperationalMode(
    const DtIoStubBcIQFIR*  pStub,
    DtIoctlIqFirCmdGetOpModeOutput*  pOutData)
{
    IQFIR_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQFIR_GetOperationalMode(IQFIR_BC, &pOutData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQFIR_OnCmdSetOperationalMode(
    const DtIoStubBcIQFIR*  pStub,
    const DtIoctlIqFirCmdSetOpModeInput*  pInData)
{
    IQFIR_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQFIR_SetOperationalMode(IQFIR_BC, pInData->m_OpMode);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_OnCmdGetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQFIR_OnCmdGetConfig(
    const DtIoStubBcIQFIR* pStub,
    DtIoctlIqFirCmdGetConfigOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR));
    DT_ASSERT(pOutData != NULL);

    return DtBcIQFIR_GetConfig(IQFIR_BC, &pOutData->m_NumberCoefs, &pOutData->m_CoefSize);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_OnCmdGetCoefficients -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQFIR_OnCmdGetCoefficients(
    const DtIoStubBcIQFIR* pStub,
    const DtIoctlIqFirCmdGetCoefsInput* pInData,
    DtIoctlIqFirCmdGetCoefsOutput* pOutData)
{
    DtStatus Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    Status = DtBcIQFIR_GetCoefficients(IQFIR_BC, pOutData->m_pCoefs, 
                                                                pInData->m_NumCoefsToGet);
    if (DT_SUCCESS(Status))
        pOutData->m_NumCoefs = pInData->m_NumCoefsToGet;
    else
        pOutData->m_NumCoefs = 0;

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_OnCmdSetCoefficients -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQFIR_OnCmdSetCoefficients(
    const DtIoStubBcIQFIR* pStub,
    const DtIoctlIqFirCmdSetCoefsInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQFIR));
    DT_ASSERT(pInData != NULL);

    return DtBcIQFIR_SetCoefficients(IQFIR_BC, pInData->m_pCoefs, 
                                                                pInData->m_NumCoefsToSet);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQFIR_OnCmdSetPrescale -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQFIR_OnCmdSetPrescale(
    const DtIoStubBcIQFIR* pStub,
    const DtIoctlIqFirCmdSetPrescaleInput* pInData)
{
    DT_ASSERT(pStub != NULL && pStub->m_Size == sizeof(DtIoStubBcIQFIR));
    DT_ASSERT(pInData != NULL);

    return DtBcIQFIR_SetPrescale(IQFIR_BC, pInData->m_PrescaleValue);
}


