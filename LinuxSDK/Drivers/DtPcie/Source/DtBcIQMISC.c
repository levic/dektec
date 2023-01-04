//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQMISC.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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
#include "DtBcIQMISC.h"
#include "DtBcIQMISC_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQMISC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQMISC_DEFAULT_PRECONDITIONS(pBc)      \
                                   DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQMISC))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQMISC_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQMISC, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQMISC_GetIqMuteState(DtBcIQMISC*, Bool* pMuteI, Bool* pMuteQ);
static DtStatus  DtBcIQMISC_GetNoiseLevel(DtBcIQMISC* pBc, Int* pNoiseLevel);
static DtStatus  DtBcIQMISC_GetOffset(DtBcIQMISC*, Int* pOffset);
static DtStatus  DtBcIQMISC_GetOutputLevel(DtBcIQMISC*, Int* pOutputLevel);
static DtStatus  DtBcIQMISC_GetTestPattern(DtBcIQMISC*, Int* pTestPattern);
static DtStatus  DtBcIQMISC_GetSpectrumInversion(DtBcIQMISC*, Bool* pSpectrumInv);
static DtStatus  DtBcIQMISC_Init(DtBc*);
static DtStatus  DtBcIQMISC_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcIQMISC_OnEnable(DtBc*, Bool  Enable);
static void  DtBcIQMISC_SetControlRegs(DtBcIQMISC*, Bool BlkEna, Int OpMode, Bool MuteI, 
                                           Bool MuteQ, Bool SpectrumInv, Int TestPattern);
static DtStatus  DtBcIQMISC_SetSpectrumInversion(DtBcIQMISC*, Bool SpectrumInv);
static DtStatus  DtBcIQMISC_SetIqMuteState(DtBcIQMISC*, Bool MuteI, Bool MuteQ);
static DtStatus  DtBcIQMISC_SetNoiseLevel(DtBcIQMISC*, Int NoiseLevel);
static DtStatus  DtBcIQMISC_SetOffset(DtBcIQMISC*, Int Offset);
static DtStatus  DtBcIQMISC_SetOutputLevel(DtBcIQMISC*, Int OutputLevel);
static DtStatus  DtBcIQMISC_SetTestPattern(DtBcIQMISC*, Int TestPattern);


// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQMISC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcIQMISC_Close(DtBc* pBc)
{
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIQMISC*  DtBcIQMISC_Open(Int Address, DtCore* pCore, DtPt* pPt, const char* pRole, 
                                                  Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    // Init open parameters
    DT_BC_IQMISC_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQMISC, Id, DT_BLOCK_TYPE_IQMISC,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQMISC_Close;
    OpenParams.m_InitFunc = DtBcIQMISC_Init;
    OpenParams.m_OnEnableFunc = DtBcIQMISC_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcIQMISC_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQMISC*)DtBc_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQMISC_GetOperationalMode(DtBcIQMISC* pBc, Int* pOpMode)
{
    // Sanity check
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQMISC_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQMISC_SetOperationalMode(DtBcIQMISC* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY 
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_IQMISC_MUST_BE_ENABLED(pBc);
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcIQMISC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                      pBc->m_MuteI, pBc->m_MuteQ, pBc->m_SpectrumInv, pBc->m_TestPattern);

    return Status;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQMISC - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQMISC_Init(DtBc* pBcBase)
{
    DtBcIQMISC* pBc = (DtBcIQMISC*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_MuteI = FALSE;
    pBc->m_MuteQ = FALSE;
    pBc->m_SpectrumInv = FALSE;
    pBc->m_TestPattern = DT_IQMISC_TESTPATT_NONE;
    // Set control register
    DtBcIQMISC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                      pBc->m_MuteI, pBc->m_MuteQ, pBc->m_SpectrumInv, pBc->m_TestPattern);

    pBc->m_NoiseLevel= 0;
    IQMISC_NoiseLevel_WRITE(pBc, 0);
    pBc->m_Offset = 0;
    IQMISC_Offset_WRITE(pBc, 0);
    pBc->m_OutputLevel = 0;
    IQMISC_OutputLevel_WRITE(pBc, 0);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcIQMISC_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQMISC* pBc = (DtBcIQMISC*)pBcBase;

    // Sanity check
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, IQMISC, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, IQMISC, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcIQMISC_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQMISC, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcIQMISC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                      pBc->m_MuteI, pBc->m_MuteQ, pBc->m_SpectrumInv, pBc->m_TestPattern);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQMISC_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcIQMISC* pBc = (DtBcIQMISC*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, IQMISC, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcIQMISC_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, IQMISC, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIQMISC_SetControlRegs(DtBcIQMISC* pBc, Bool BlkEna, Int OpMode, Bool MuteI, 
                                            Bool MuteQ, Bool SpectrumInv, Int TestPattern)
{
    UInt32  RegData=0, FwBlkEna=0, FwOpMode=0, FwTestPattern=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? IQMISC_BLKENA_ENABLED : IQMISC_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
        case DT_BLOCK_OPMODE_IDLE:    FwOpMode = IQMISC_OPMODE_IDLE; break;
        case DT_BLOCK_OPMODE_STANDBY: FwOpMode = IQMISC_OPMODE_STANDBY; break;
        case DT_BLOCK_OPMODE_RUN:     FwOpMode = IQMISC_OPMODE_RUN; break;
        default: DT_ASSERT(FALSE);
    }

    switch (TestPattern)
    {
        case DT_IQMISC_TESTPATT_NONE:    FwTestPattern = IQMISC_TESTPATT_NONE; break;
        case DT_IQMISC_TESTPATT_NYQ:     FwTestPattern = IQMISC_TESTPATT_NYQ; break;
        case DT_IQMISC_TESTPATT_HALFNYQ: FwTestPattern = IQMISC_TESTPATT_HALFNYQ; break;
        case DT_IQMISC_TESTPATT_CW:      FwTestPattern = IQMISC_TESTPATT_CW; break;
        case DT_IQMISC_TESTPATT_CI:      FwTestPattern = IQMISC_TESTPATT_CI; break;
        case DT_IQMISC_TESTPATT_CQ:      FwTestPattern = IQMISC_TESTPATT_CQ; break;
        default: DT_ASSERT(FALSE);
    }
   
    // Update control register
    RegData = IQMISC_Control_READ(pBc);
    RegData = IQMISC_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = IQMISC_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = IQMISC_Control_SET_TestPattern(RegData, FwTestPattern);
    RegData = IQMISC_Control_SET_MuteI(RegData, MuteI ? 1:0);
    RegData = IQMISC_Control_SET_MuteQ(RegData, MuteQ ? 1:0);
    RegData = IQMISC_Control_SET_SpecInv(RegData, SpectrumInv ? 1:0);
    IQMISC_Control_WRITE(pBc, RegData);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_SetTestPattern -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQMISC_SetTestPattern(DtBcIQMISC* pBc, Int TestPattern)
{
    // Sanity check
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQMISC_MUST_BE_ENABLED(pBc);

    if(TestPattern!=DT_IQMISC_TESTPATT_NONE && TestPattern!=DT_IQMISC_TESTPATT_NYQ &&
       TestPattern!=DT_IQMISC_TESTPATT_HALFNYQ && TestPattern!=DT_IQMISC_TESTPATT_CW &&
       TestPattern!=DT_IQMISC_TESTPATT_CI && TestPattern!=DT_IQMISC_TESTPATT_CQ)
    return DT_STATUS_INVALID_PARAMETER;

    pBc->m_TestPattern = TestPattern;
    DtBcIQMISC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                      pBc->m_MuteI, pBc->m_MuteQ, pBc->m_SpectrumInv, pBc->m_TestPattern);

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_GetTestPattern -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQMISC_GetTestPattern(DtBcIQMISC* pBc, Int* pTestPattern)
{
    // Sanity check
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQMISC_MUST_BE_ENABLED(pBc);
    DT_ASSERT(pTestPattern != NULL);

    *pTestPattern = pBc->m_TestPattern;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_SetIqMuteState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQMISC_SetIqMuteState(DtBcIQMISC* pBc, Bool MuteI, Bool MuteQ)
{
    // Sanity check
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQMISC_MUST_BE_ENABLED(pBc);

    pBc->m_MuteI = MuteI;
    pBc->m_MuteQ = MuteQ;
    DtBcIQMISC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                      pBc->m_MuteI, pBc->m_MuteQ, pBc->m_SpectrumInv, pBc->m_TestPattern);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_GetIqMuteState -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQMISC_GetIqMuteState(DtBcIQMISC* pBc, Bool* pMuteI, Bool* pMuteQ)
{
    // Sanity check
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQMISC_MUST_BE_ENABLED(pBc);
    DT_ASSERT(pMuteI != NULL && pMuteQ != NULL);

    *pMuteI = pBc->m_MuteI;
    *pMuteQ = pBc->m_MuteI;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_SetSpectrumInversion -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQMISC_SetSpectrumInversion(DtBcIQMISC* pBc, Bool SpectrumInv)
{
    // Sanity check
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQMISC_MUST_BE_ENABLED(pBc);

    pBc->m_SpectrumInv = SpectrumInv;
    DtBcIQMISC_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                      pBc->m_MuteI, pBc->m_MuteQ, pBc->m_SpectrumInv, pBc->m_TestPattern);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_GetSpectrumInversion -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQMISC_GetSpectrumInversion(DtBcIQMISC* pBc, Bool* pSpectrumInv)
{
    // Sanity check
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQMISC_MUST_BE_ENABLED(pBc);
    DT_ASSERT(pSpectrumInv != NULL);

    *pSpectrumInv = pBc->m_SpectrumInv;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_SetnoiseLevel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQMISC_SetNoiseLevel(DtBcIQMISC* pBc, Int NoiseLevel)
{
    // Sanity check
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQMISC_MUST_BE_ENABLED(pBc);


    // 18 bits, 2.16 format
    if (NoiseLevel > 0x3FFFF)
        return DT_STATUS_INVALID_PARAMETER;

    pBc->m_NoiseLevel= NoiseLevel;
    IQMISC_NoiseLevel_WRITE(pBc, NoiseLevel);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_GetNoiseLevel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQMISC_GetNoiseLevel(DtBcIQMISC* pBc, Int* pNoiseLevel)
{
    // Sanity check
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQMISC_MUST_BE_ENABLED(pBc);

    *pNoiseLevel=pBc->m_NoiseLevel;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_SetOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQMISC_SetOffset(DtBcIQMISC* pBc, Int Offset)
{
    // Sanity check
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQMISC_MUST_BE_ENABLED(pBc);
    
    if (Offset < -128 || Offset > 127)
        return DT_STATUS_INVALID_PARAMETER;

    pBc->m_Offset = Offset;
    IQMISC_Offset_WRITE(pBc, Offset);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_GetOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIQMISC_GetOffset(DtBcIQMISC* pBc, Int* pOffset)
{
    // Sanity check
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQMISC_MUST_BE_ENABLED(pBc);

    *pOffset = pBc->m_Offset;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_SetOutputLevel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQMISC_SetOutputLevel(DtBcIQMISC* pBc, Int OutputLevel)
{
    // Sanity check
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQMISC_MUST_BE_ENABLED(pBc);


    // 18 bits, 2.16 format
    if (OutputLevel > 0x3FFFF)
        return DT_STATUS_INVALID_PARAMETER;

    pBc->m_OutputLevel= OutputLevel;
    IQMISC_OutputLevel_WRITE(pBc, OutputLevel);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQMISC_GetOutputLevel -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQMISC_GetOutputLevel(DtBcIQMISC* pBc, Int* pOutputLevel)
{
    // Sanity check
    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);
    BC_IQMISC_MUST_BE_ENABLED(pBc);


    *pOutputLevel = pBc->m_OutputLevel;
    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQMISC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQMISC))
// Helper macro to cast stub's DtBc member to DtBcIQMISC
#define IQMISC_STUB   ((DtIoStubBcIQMISC*)pStub)
#define IQMISC_BC  ((DtBcIQMISC*)IQMISC_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQMISC_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcIQMISC_OnCmdGetIqMute(const DtIoStubBcIQMISC*,
                                                        DtIoctlIqMiscCmdGetIqMuteOutput*);
static DtStatus  DtIoStubBcIQMISC_OnCmdGetNoiseLevel(const DtIoStubBcIQMISC*,
                                                    DtIoctlIqMiscCmdGetNoiseLevelOutput*);
static DtStatus  DtIoStubBcIQMISC_OnCmdGetOffset(const DtIoStubBcIQMISC*,
                                                        DtIoctlIqMiscCmdGetOffsetOutput*);
static DtStatus  DtIoStubBcIQMISC_OnCmdGetOperationalMode(const DtIoStubBcIQMISC*, 
                                                        DtIoctlIqMiscCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcIQMISC_OnCmdGetOutputLevel(const DtIoStubBcIQMISC*,
                                                   DtIoctlIqMiscCmdGetOutputLevelOutput*);
static DtStatus  DtIoStubBcIQMISC_OnCmdGetSpectrumInversion(const DtIoStubBcIQMISC*,
                                                   DtIoctlIqMiscCmdGetSpectrumInvOutput*);
static DtStatus  DtIoStubBcIQMISC_OnCmdGetTestPattern(const DtIoStubBcIQMISC*,
                                                   DtIoctlIqMiscCmdGetTestPatternOutput*);
static DtStatus  DtIoStubBcIQMISC_OnCmdSetIqMute(const DtIoStubBcIQMISC*,
                                                   const DtIoctlIqMiscCmdSetIqMuteInput*);
static DtStatus  DtIoStubBcIQMISC_OnCmdSetNoiseLevel(const DtIoStubBcIQMISC*,
                                               const DtIoctlIqMiscCmdSetNoiseLevelInput*);
static DtStatus  DtIoStubBcIQMISC_OnCmdSetOperationalMode(const DtIoStubBcIQMISC*,
                                                   const DtIoctlIqMiscCmdSetOpModeInput*);
static DtStatus  DtIoStubBcIQMISC_OnCmdSetOffset(const DtIoStubBcIQMISC*,
                                                   const DtIoctlIqMiscCmdSetOffsetInput*);
static DtStatus  DtIoStubBcIQMISC_OnCmdSetOutputLevel(const DtIoStubBcIQMISC*,
                                              const DtIoctlIqMiscCmdSetOutputLevelInput*);
static DtStatus  DtIoStubBcIQMISC_OnCmdSetSpectrumInversion(const DtIoStubBcIQMISC*,
                                              const DtIoctlIqMiscCmdSetSpectrumInvInput*);
static DtStatus  DtIoStubBcIQMISC_OnCmdSetTestPattern(const DtIoStubBcIQMISC*,
                                              const DtIoctlIqMiscCmdSetTestPatternInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQMISC;

static const DtIoctlProperties  IOSTUB_BC_IQMISC_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQMISC_CMD(
        DtIoStubBcIQMISC_OnCmd,
        NULL, NULL),
};

// +=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQMISC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcIQMISC_Close(DtIoStub* pStub)
{
    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcIQMISC*  DtIoStubBcIQMISC_Open(DtBc* pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IQMISC_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQMISC, pBc, NULL, 
                                                               DtIoStubBcIQMISC_Close,
                                                               NULL,  // Use default IOCTL
                                                               IOSTUB_BC_IQMISC_IOCTLS);
    return (DtIoStubBcIQMISC*)DtIoStubBc_Open(&OpenParams);
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQMISC - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQMISC_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams* pIoParams, 
                                                                            Int* pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqMiscCmdInput*  pInData = NULL;
    DtIoctlIqMiscCmdOutput*  pOutData = NULL;

    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQMISC_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQMISC, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqMiscCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqMiscCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IQMISC_CMD_GET_IQMUTE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQMISC_OnCmdGetIqMute(IQMISC_STUB, &pOutData->m_GetIqMute);
        break;
    case DT_IQMISC_CMD_GET_NOISE_LEVEL:
        Status = DtIoStubBcIQMISC_OnCmdGetNoiseLevel(IQMISC_STUB, 
                                                              &pOutData->m_GetNoiseLevel);
        break;
    case DT_IQMISC_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQMISC_OnCmdGetOperationalMode(IQMISC_STUB,  
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_IQMISC_CMD_GET_OFFSET:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQMISC_OnCmdGetOffset(IQMISC_STUB, &pOutData->m_GetOffset);
        break;
    case DT_IQMISC_CMD_GET_OUTPUT_LEVEL:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQMISC_OnCmdGetOutputLevel(IQMISC_STUB, 
                                                             &pOutData->m_GetOutputLevel);
        break;
    case DT_IQMISC_CMD_GET_SPECTRUM_INVERSION:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQMISC_OnCmdGetSpectrumInversion(IQMISC_STUB, 
                                                             &pOutData->m_GetSpectrumInv);
        break;
    case DT_IQMISC_CMD_GET_TEST_PATTERN:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQMISC_OnCmdGetTestPattern(IQMISC_STUB, 
                                                             &pOutData->m_GetTestPattern);
        break;
    case DT_IQMISC_CMD_SET_IQMUTE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQMISC_OnCmdSetIqMute(IQMISC_STUB, &pInData->m_SetIqMute);
        break;
    case DT_IQMISC_CMD_SET_NOISE_LEVEL:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQMISC_OnCmdSetNoiseLevel(IQMISC_STUB, 
                                                               &pInData->m_SetNoiseLevel);
        break;
    case DT_IQMISC_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQMISC_OnCmdSetOperationalMode(IQMISC_STUB,  
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_IQMISC_CMD_SET_OFFSET:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQMISC_OnCmdSetOffset(IQMISC_STUB, &pInData->m_SetOffset);
        break;
    case DT_IQMISC_CMD_SET_OUTPUT_LEVEL:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQMISC_OnCmdSetOutputLevel(IQMISC_STUB, 
                                                              &pInData->m_SetOutputLevel);
        break;
    case DT_IQMISC_CMD_SET_SPECTRUM_INVERSION:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQMISC_OnCmdSetSpectrumInversion(IQMISC_STUB,
                                                              &pInData->m_SetSpectrumInv);
        break;
    case DT_IQMISC_CMD_SET_TEST_PATTERN:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQMISC_OnCmdSetTestPattern(IQMISC_STUB,
                                                              &pInData->m_SetTestPattern);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_OnCmdGetIqMute -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQMISC_OnCmdGetIqMute(const DtIoStubBcIQMISC* pStub,
                                                DtIoctlIqMiscCmdGetIqMuteOutput* pOutData)
{
    DtStatus Result = DT_STATUS_OK;
    Bool MuteI = FALSE;
    Bool MuteQ = FALSE;

    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    Result =  DtBcIQMISC_GetIqMuteState(IQMISC_BC, &MuteI, &MuteQ);
    pOutData->m_MuteI = MuteI? 1: 0;
    pOutData->m_MuteQ = MuteQ ? 1: 0;
    return Result;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_OnCmdGetNoiseLevel -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQMISC_OnCmdGetNoiseLevel(const DtIoStubBcIQMISC* pStub,
                                            DtIoctlIqMiscCmdGetNoiseLevelOutput* pOutData)
{
    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQMISC_GetNoiseLevel(IQMISC_BC, &pOutData->m_NoiseLevel);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_OnCmdGetOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQMISC_OnCmdGetOffset(const DtIoStubBcIQMISC* pStub,
                                                DtIoctlIqMiscCmdGetOffsetOutput* pOutData)
{
    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQMISC_GetOffset(IQMISC_BC, &pOutData->m_Offset);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQMISC_OnCmdGetOperationalMode(const DtIoStubBcIQMISC* pStub,
                                                DtIoctlIqMiscCmdGetOpModeOutput* pOutData)
{
    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQMISC_GetOperationalMode(IQMISC_BC, &pOutData->m_OpMode);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_OnCmdGetOutputLevel -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQMISC_OnCmdGetOutputLevel(const DtIoStubBcIQMISC* pStub,
                                           DtIoctlIqMiscCmdGetOutputLevelOutput* pOutData)
{
    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQMISC_GetOutputLevel(IQMISC_BC, &pOutData->m_OutputLevel);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_OnCmdGetSpectrumInversion -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQMISC_OnCmdGetSpectrumInversion(const DtIoStubBcIQMISC* pStub,
                                           DtIoctlIqMiscCmdGetSpectrumInvOutput* pOutData)
{
    DtStatus Result = DT_STATUS_OK;
    Bool SpectrumInversion = FALSE;

    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    Result = DtBcIQMISC_GetSpectrumInversion(IQMISC_BC, &SpectrumInversion);
    pOutData->m_SpectrumInversion = SpectrumInversion?1:0;
    return Result;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_OnCmdGetTestPattern -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQMISC_OnCmdGetTestPattern(const DtIoStubBcIQMISC* pStub,
                                           DtIoctlIqMiscCmdGetTestPatternOutput* pOutData)
{
    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcIQMISC_GetTestPattern(IQMISC_BC, &pOutData->m_TestPattern);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_OnCmdSetIqMute -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQMISC_OnCmdSetIqMute(const DtIoStubBcIQMISC* pStub,
                                            const DtIoctlIqMiscCmdSetIqMuteInput* pInData)
{
    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQMISC_SetIqMuteState(IQMISC_BC, (pInData->m_MuteI!=0), 
                                                                   (pInData->m_MuteQ!=0));
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_OnCmdSetNoiseLevel -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQMISC_OnCmdSetNoiseLevel(const DtIoStubBcIQMISC* pStub,
                                        const DtIoctlIqMiscCmdSetNoiseLevelInput* pInData)
{
    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQMISC_SetNoiseLevel(IQMISC_BC, pInData->m_NoiseLevel);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_OnCmdSetOffset -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQMISC_OnCmdSetOffset(const DtIoStubBcIQMISC* pStub,
                                            const DtIoctlIqMiscCmdSetOffsetInput* pInData)
{
    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQMISC_SetOffset(IQMISC_BC, pInData->m_Offset);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQMISC_OnCmdSetOperationalMode(const DtIoStubBcIQMISC* pStub,
                                            const DtIoctlIqMiscCmdSetOpModeInput* pInData)
{
    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQMISC_SetOperationalMode(IQMISC_BC, pInData->m_OpMode);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_OnCmdSetOutputLevel -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQMISC_OnCmdSetOutputLevel(const DtIoStubBcIQMISC* pStub,
                                       const DtIoctlIqMiscCmdSetOutputLevelInput* pInData)
{
    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQMISC_SetOutputLevel(IQMISC_BC, pInData->m_OutputLevel);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_OnCmdSetSpectrumInversion -.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtIoStubBcIQMISC_OnCmdSetSpectrumInversion(const DtIoStubBcIQMISC* pStub,
                                       const DtIoctlIqMiscCmdSetSpectrumInvInput* pInData)
{
    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQMISC_SetSpectrumInversion(IQMISC_BC, (pInData->m_SpectrumInversion!=0));
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQMISC_OnCmdSetTestPattern -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQMISC_OnCmdSetTestPattern(const DtIoStubBcIQMISC* pStub,
                                       const DtIoctlIqMiscCmdSetTestPatternInput* pInData)
{
    IQMISC_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);

    return DtBcIQMISC_SetTestPattern(IQMISC_BC, pInData->m_TestPattern);
}
