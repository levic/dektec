//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDITXP.c *#*#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
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
#include "DtDf.h"
#include "DtBcSDITXP.h"
#include "DtBcSDITXP_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXP implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_SDITXP_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDITXP))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_SDITXP_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(SDITXP, pBc)

// Helper macro to cast a DtBc* to a DtBcSDITXP*
#define BC_SDITXP         ((DtBcSDITXP*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcSDITXP_Init(DtBc*);
static DtStatus  DtBcSDITXP_OnEnable(DtBc*, Bool);
static DtStatus  DtBcSDITXP_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcSDITXP_CheckSdiRate(DtBcSDITXP*, Int SdiRate);
static void  DtBcSDITXP_SetControlRegs(DtBcSDITXP*, Bool BlkEna, Int OpMode,
                                Int SdiRate, Bool ClampEna, Bool AdpEna, Bool LineCrcEna,
                                Bool LvlA2BEna);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXP - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcSDITXP_Close(DtBc*  pBc)
{
    BC_SDITXP_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));
   
    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcSDITXP*  DtBcSDITXP_Open(Int  Address, DtCore*  pCore, DtPt*  pPt, DtBcType  Type,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(Type== DT_BLOCK_TYPE_SDITXP || Type==DT_BLOCK_TYPE_SDITXP6G12G);
    
    // Init open parameters
    DT_BC_SDITXP_INIT_ID(Id, Type, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcSDITXP, Id, Type, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcSDITXP_Close;
    OpenParams.m_InitFunc = DtBcSDITXP_Init;
    OpenParams.m_OnEnableFunc = DtBcSDITXP_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcSDITXP_OnCloseFile;
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcSDITXP*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP_GetClampedSymsCount -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXP_GetClampedSymsCount(DtBcSDITXP*  pBc, UInt*  pClampedCount)
{
    // Sanity check
    BC_SDITXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pClampedCount == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXP_MUST_BE_ENABLED(pBc);

    // Return  the number of symbols clamped
    *pClampedCount = SDITXP_NumSymClamped_READ(pBc);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP_GetGenerationMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXP_GetGenerationMode(DtBcSDITXP*  pBc, Bool*  pClampEna, 
                                               Bool*  pAdpChecksumEna, Bool*  pLineCrcEna)
{
    // Sanity check
    BC_SDITXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pClampEna==NULL || pAdpChecksumEna==NULL || pLineCrcEna==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXP_MUST_BE_ENABLED(pBc);

    // Return cached generation mode
    *pClampEna = pBc->m_ClampEnable;
    *pAdpChecksumEna = pBc->m_AdpChecksumEnable;
    *pLineCrcEna = pBc->m_LineCrcEnable;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP_GetMaxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXP_GetMaxSdiRate(DtBcSDITXP*  pBc, Int*  pMaxSdiRate)
{
    // Sanity check
    BC_SDITXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pMaxSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXP_MUST_BE_ENABLED(pBc);

    // Return cached maximum SDI-rate
    *pMaxSdiRate = pBc->m_MaxSdiRate;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXP_GetOperationalMode(DtBcSDITXP*  pBc, Int*  pOpMode)
{
    // Sanity check
    BC_SDITXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXP_MUST_BE_ENABLED(pBc);

    // Return cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP_GetSdiRateAndLvlA2BEna -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXP_GetSdiRateAndLvlA2BEna(DtBcSDITXP* pBc, Int* pSdiRate, 
                                                                          Int* pLvlA2BEna)
{
    // Sanity check
    BC_SDITXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pSdiRate==NULL || pLvlA2BEna==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXP_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pSdiRate = pBc->m_SdiRate;
    *pLvlA2BEna = pBc->m_LevelA2BEnable;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP_SetGenerationMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXP_SetGenerationMode(DtBcSDITXP*  pBc, Bool  ClampEna, 
                                                   Bool  AdpChecksumEna, Bool  LineCrcEna)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_SDITXP_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDITXP_MUST_BE_ENABLED(pBc);

    // Operational mode must be IDLE
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
    {
        DtDbgOutBc(ERR, SDITXP, pBc, "Operational mode not in idle");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // No change?
    if (pBc->m_ClampEnable==ClampEna && pBc->m_AdpChecksumEnable==AdpChecksumEna 
                                                      && pBc->m_LineCrcEnable==LineCrcEna)
        return DT_STATUS_OK;

    // Set new control register value
    DtBcSDITXP_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                     pBc->m_SdiRate, ClampEna, AdpChecksumEna,LineCrcEna,
                                     pBc->m_LevelA2BEnable);
    // Save new setting
    pBc->m_ClampEnable = ClampEna;
    pBc->m_AdpChecksumEnable = AdpChecksumEna;
    pBc->m_LineCrcEnable = LineCrcEna;

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXP_SetOperationalMode(DtBcSDITXP* pBc, Int OpMode)
{
    DtStatus  Status=DT_STATUS_OK;
    // Sanity check
    BC_SDITXP_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXP_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Set new control register value
    DtBcSDITXP_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode, pBc->m_SdiRate,
                      pBc->m_ClampEnable, pBc->m_AdpChecksumEnable, pBc->m_LineCrcEnable,
                      pBc->m_LevelA2BEnable);

    // Save new settings
    pBc->m_OperationalMode = OpMode;
    
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP_SetSdiRateAndLvlA2BEna -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXP_SetSdiRateAndLvlA2BEna(DtBcSDITXP* pBc, Int SdiRate, Int LvlA2BEna)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity check
    BC_SDITXP_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    DT_RETURN_ON_ERROR(DtBcSDITXP_CheckSdiRate(pBc, SdiRate));
    if (LvlA2BEna!=FALSE && LvlA2BEna!=TRUE)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXP_MUST_BE_ENABLED(pBc);

    // Operational mode must be IDLE
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_IDLE)
    {
        DtDbgOutBc(ERR, SDITXP, pBc, "Operational mode not in idle");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // No change?
    if (pBc->m_SdiRate==SdiRate && pBc->m_LevelA2BEnable==(LvlA2BEna==TRUE))
        return DT_STATUS_OK;

    // Set new control register value
    DtBcSDITXP_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, SdiRate,
                      pBc->m_ClampEnable, pBc->m_AdpChecksumEnable, pBc->m_LineCrcEnable,
                      LvlA2BEna==TRUE);
    // Save new setting
    pBc->m_SdiRate = SdiRate;
    pBc->m_LevelA2BEnable = (LvlA2BEna==TRUE);

    return Status;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXP - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSDITXP_Init(DtBc*  pBcBase)
{
    DtBcSDITXP* pBc = (DtBcSDITXP*)pBcBase;
    UInt32  FwMaxSdiRate;

    // Sanity check
    BC_SDITXP_DEFAULT_PRECONDITIONS(pBc);

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    pBc->m_SdiRate = DT_DRV_SDIRATE_HD;
    pBc->m_ClampEnable = TRUE;
    pBc->m_AdpChecksumEnable = TRUE;
    pBc->m_LineCrcEnable = TRUE;
    pBc->m_LevelA2BEnable = FALSE;

    // Get maximum supported rate
    FwMaxSdiRate = SDITXP_Config_READ_MaxSdiRate(pBc);
    switch (FwMaxSdiRate)
    {
    case SDITXP_SDIMODE_SD:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_SD; break;
    case SDITXP_SDIMODE_HD:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_HD; break;
    case SDITXP_SDIMODE_3G:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_3G; break;
    case SDITXP_SDIMODE_6G:     pBc->m_MaxSdiRate = DT_DRV_SDIRATE_6G; break;
    case SDITXP_SDIMODE_12G:    pBc->m_MaxSdiRate = DT_DRV_SDIRATE_12G; break;
    default: DT_ASSERT(FALSE);  return DT_STATUS_FAIL;
    }

    DtBcSDITXP_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode, 
                                          pBc->m_SdiRate, pBc->m_ClampEnable, 
                                          pBc->m_AdpChecksumEnable, pBc->m_LineCrcEnable,
                                          pBc->m_LevelA2BEnable);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXP_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcSDITXP* pBc = (DtBcSDITXP*)pBcBase;

    // Sanity check
    BC_SDITXP_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, SDITXP, pBc, "Block Enable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
        pBc->m_SdiRate = DT_DRV_SDIRATE_SD;
        pBc->m_ClampEnable = TRUE;
        pBc->m_AdpChecksumEnable = TRUE;
        pBc->m_LineCrcEnable = TRUE;
        pBc->m_LevelA2BEnable = FALSE;
        DtBcSDITXP_SetControlRegs(pBc, Enable, pBc->m_OperationalMode, 
                                          pBc->m_SdiRate, pBc->m_ClampEnable, 
                                          pBc->m_AdpChecksumEnable, pBc->m_LineCrcEnable,
                                          pBc->m_LevelA2BEnable);
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, SDITXP, pBc, "Block Enable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcSDITXP_SetOperationalMode(BC_SDITXP, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SDITXP, pBc, 
                                         "ERROR: DtBcSDITXP_SetOperationalMode failed");
            DT_ASSERT(FALSE);
            return Status;
        }

        // Disable block
        DtBcSDITXP_SetControlRegs(pBc, Enable, pBc->m_OperationalMode, 
                                          pBc->m_SdiRate, pBc->m_ClampEnable, 
                                          pBc->m_AdpChecksumEnable, pBc->m_LineCrcEnable,
                                          pBc->m_LevelA2BEnable);
    }
    // Save new setting
    pBc->m_BlockEnabled = Enable;

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcSDITXP_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_SDITXP_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled(pBc))
    {
        DtDbgOutBc(AVG, SDITXP, pBc, "Go to IDLE");

        // Go to IDLE
        Status = DtBcSDITXP_SetOperationalMode(BC_SDITXP, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SDITXP, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP_CheckSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXP_CheckSdiRate(DtBcSDITXP* pBc, Int SdiRate)
{
    // Check whether it is a valid SDI-rate
    if (   SdiRate!=DT_DRV_SDIRATE_SD && SdiRate!=DT_DRV_SDIRATE_HD 
        && SdiRate!=DT_DRV_SDIRATE_3G && SdiRate!=DT_DRV_SDIRATE_6G 
        && SdiRate!=DT_DRV_SDIRATE_12G)
        return DT_STATUS_INVALID_PARAMETER;

    // Assumption we can keep the comparison simple
    DT_ASSERT(DT_DRV_SDIRATE_SD<DT_DRV_SDIRATE_HD 
              && DT_DRV_SDIRATE_HD<DT_DRV_SDIRATE_3G
              && DT_DRV_SDIRATE_3G<DT_DRV_SDIRATE_6G
              && DT_DRV_SDIRATE_6G<DT_DRV_SDIRATE_12G);
    
    // Check whether the SDI-rate is supported
    if (SdiRate > pBc->m_MaxSdiRate)
        return DT_STATUS_NOT_SUPPORTED;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXP_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcSDITXP_SetControlRegs(DtBcSDITXP* pBc, Bool BlkEna, Int OpMode, Int SdiRate,
                                              Bool ClampEna, Bool AdpEna, Bool LineCrcEna,
                                              Bool LvlA2BEna)
{
    UInt32 FwBlkEna=0, FwOpMode=0, FwSdiRate=0, RegData=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? SDITXP_BLKENA_ENABLED :SDITXP_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = SDITXP_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = SDITXP_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }
    // Convert SDI rate to BB-type
    switch (SdiRate)
    {
    case DT_DRV_SDIRATE_SD:   FwSdiRate = SDITXP_SDIMODE_SD; break;
    case DT_DRV_SDIRATE_HD:   FwSdiRate = SDITXP_SDIMODE_HD; break;
    case DT_DRV_SDIRATE_3G:   FwSdiRate = SDITXP_SDIMODE_3G; break;
    case DT_DRV_SDIRATE_6G:   FwSdiRate = SDITXP_SDIMODE_6G; break;
    case DT_DRV_SDIRATE_12G:  FwSdiRate = SDITXP_SDIMODE_12G; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control
    RegData = SDITXP_Control_READ(pBc);
    RegData = SDITXP_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = SDITXP_Control_SET_OperationalMode(RegData, FwOpMode);
    RegData = SDITXP_Control_SET_SdiRate(RegData, FwSdiRate);
    RegData = SDITXP_Control_SET_ClampEnable(RegData, ClampEna ? 1 : 0);
    RegData = SDITXP_Control_SET_AdpChecksumEnable(RegData, AdpEna ? 1 : 0);
    RegData = SDITXP_Control_SET_LineCrcEnable(RegData, LineCrcEna ? 1 : 0);
    RegData = SDITXP_Control_SET_LevelA2BEnable(RegData, LvlA2BEna ? 1 : 0);
    SDITXP_Control_WRITE(pBc, RegData);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDITXP implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcSDITXP
#define SDITXP_STUB   ((DtIoStubBcSDITXP*)pStub)
#define SDITXP_BC  ((DtBcSDITXP*)SDITXP_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcSDITXP_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus DtIoStubBcSDITXP_OnCmdGetClampedSymsCount(const DtIoStubBcSDITXP*, 
                                            DtIoctlSdiTxPCmdGetClampedSymsCountOutput*);
static DtStatus DtIoStubBcSDITXP_OnCmdGetGenerationMode(const DtIoStubBcSDITXP*,
                                                     DtIoctlSdiTxPCmdGetGenModeOutput*);
static DtStatus DtIoStubBcSDITXP_OnCmdGetMaxSdiRate(const DtIoStubBcSDITXP*,
                                                    DtIoctlSdiTxPCmdGetMaxSdiRateOutput*);
static DtStatus DtIoStubBcSDITXP_OnCmdGetOperationalMode(const DtIoStubBcSDITXP*,
                                                      DtIoctlSdiTxPCmdGetOpModeOutput*);
static DtStatus DtIoStubBcSDITXP_OnCmdGetSdiRateAndLvlA2BEna(const DtIoStubBcSDITXP*,
                                         DtIoctlSdiTxPCmdGetSdiRateAndLvlA2BEnaOutput*);
static DtStatus DtIoStubBcSDITXP_OnCmdSetGenerationMode(const DtIoStubBcSDITXP*,
                                                const DtIoctlSdiTxPCmdSetGenModeInput*);
static DtStatus DtIoStubBcSDITXP_OnCmdSetOperationalMode(const DtIoStubBcSDITXP*,
                                                 const DtIoctlSdiTxPCmdSetOpModeInput*);
static DtStatus DtIoStubBcSDITXP_OnCmdSetSdiRateAndLvlA2BEna(const DtIoStubBcSDITXP*,
                                    const DtIoctlSdiTxPCmdSetSdiRateAndLvlA2BEnaInput*);


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_SDITXP;

static const DtIoctlProperties  IOSTUB_BC_SDITXP_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_SDITXP_CMD(
        DtIoStubBcSDITXP_OnCmd,
        NULL,  NULL),
};

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDITXP - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXP_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcSDITXP_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXP));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXP_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcSDITXP*  DtIoStubBcSDITXP_Open(DtBc*  pBc)
{
    DtIoStubBcSDITXP*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDITXP));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcSDITXP, pBc, NULL, 
                                                             DtIoStubBcSDITXP_Close,
                                                             NULL,  // Use default IOCTL
                                                             IOSTUB_BC_SDITXP_IOCTLS);
    pStub = (DtIoStubBcSDITXP*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDITXP- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXP_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcSDITXP_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
                                                                           Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlSdiTxPCmdInput*  pInData = NULL;
    DtIoctlSdiTxPCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXP));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_SDITXP_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, SDITXP, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_SdiTxPCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_SdiTxPCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
     case DT_SDITXP_CMD_GET_CLAMPED_SYMS_COUNT:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDITXP_OnCmdGetClampedSymsCount(SDITXP_STUB, 
                                                              &pOutData->m_GetClampedCnt);
        break;
    case DT_SDITXP_CMD_GET_GENERATION_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDITXP_OnCmdGetGenerationMode(SDITXP_STUB,
                                                                 &pOutData->m_GetGenMode);
        break;
     case DT_SDITXP_CMD_GET_MAX_SDIRATE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDITXP_OnCmdGetMaxSdiRate(SDITXP_STUB, 
                                                              &pOutData->m_GetMaxSdiRate);
        break;
     case DT_SDITXP_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDITXP_OnCmdGetOperationalMode(SDITXP_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_SDITXP_CMD_GET_SDIRATE_AND_LVLA2BENA:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDITXP_OnCmdGetSdiRateAndLvlA2BEna(SDITXP_STUB,
                                                     &pOutData->m_GetSdiRateAndLvlA2BEna);
        break;
    case DT_SDITXP_CMD_SET_GENERATION_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcSDITXP_OnCmdSetGenerationMode(SDITXP_STUB,
                                                                  &pInData->m_SetGenMode);
        break;
    case DT_SDITXP_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSDITXP_OnCmdSetOperationalMode(SDITXP_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;
    case DT_SDITXP_CMD_SET_SDIRATE_AND_LVLA2BENA:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcSDITXP_OnCmdSetSdiRateAndLvlA2BEna(SDITXP_STUB, 
                                                      &pInData->m_SetSdiRateAndLvlA2BEna);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXP_OnCmdGetClampedSymsCount -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcSDITXP_OnCmdGetClampedSymsCount(
    const DtIoStubBcSDITXP*  pStub,
    DtIoctlSdiTxPCmdGetClampedSymsCountOutput*  pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXP));
    DT_ASSERT(pOutData != NULL);

    // Get clamped symbols count
    return DtBcSDITXP_GetClampedSymsCount(SDITXP_BC, &pOutData->m_ClampedSymsCount);
}


//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXP_OnCmdGetGenerationMode -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcSDITXP_OnCmdGetGenerationMode(
    const DtIoStubBcSDITXP*  pStub,
    DtIoctlSdiTxPCmdGetGenModeOutput*  pOutData)
{
    DtStatus Status = DT_STATUS_OK;
    Bool ClampEna, AdpChecksumEna, LineCrcEna;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXP));
    DT_ASSERT(pOutData != NULL);

    // Get generation mode
    Status = DtBcSDITXP_GetGenerationMode(SDITXP_BC, &ClampEna, &AdpChecksumEna,
                                                                             &LineCrcEna);
    if (DT_SUCCESS(Status))
    { 
        pOutData->m_ClampEnable = ClampEna;
        pOutData->m_AdpChecksumEnable = AdpChecksumEna;
        pOutData->m_LineCrcEnable = LineCrcEna;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXP_OnCmdGetMaxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcSDITXP_OnCmdGetMaxSdiRate(
    const DtIoStubBcSDITXP* pStub,
    DtIoctlSdiTxPCmdGetMaxSdiRateOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXP));
    DT_ASSERT(pOutData != NULL);

    // Get maximum SDI-rate
    return DtBcSDITXP_GetMaxSdiRate(SDITXP_BC, &pOutData->m_MaxSdiRate);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXP_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDITXP_OnCmdGetOperationalMode(
    const DtIoStubBcSDITXP* pStub,
    DtIoctlSdiTxPCmdGetOpModeOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXP));
    DT_ASSERT(pOutData != NULL);

    // Get operational mode
    return DtBcSDITXP_GetOperationalMode(SDITXP_BC, &pOutData->m_OpMode);
}

//-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXP_OnCmdGetSdiRateAndLvlA2BEna -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDITXP_OnCmdGetSdiRateAndLvlA2BEna(
    const DtIoStubBcSDITXP* pStub,
    DtIoctlSdiTxPCmdGetSdiRateAndLvlA2BEnaOutput* pOutData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXP));
    DT_ASSERT(pOutData != NULL);

    // Get SDI mode
    return DtBcSDITXP_GetSdiRateAndLvlA2BEna(SDITXP_BC, &pOutData->m_SdiRate,
                                                             &pOutData->m_LevelA2BEnable);
}
//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXP_OnCmdSetGenerationMode -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtIoStubBcSDITXP_OnCmdSetGenerationMode(
    const DtIoStubBcSDITXP*  pStub,
    const DtIoctlSdiTxPCmdSetGenModeInput*  pInData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXP));
    DT_ASSERT(pInData != NULL);

    // Set generation mode
    return DtBcSDITXP_SetGenerationMode(SDITXP_BC, (Int)pInData->m_ClampEnable,
                       (Int)pInData->m_AdpChecksumEnable,  (Int)pInData->m_LineCrcEnable);
}

//-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXP_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDITXP_OnCmdSetOperationalMode(
    const DtIoStubBcSDITXP* pStub,
    const DtIoctlSdiTxPCmdSetOpModeInput* pInData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXP));
    DT_ASSERT(pInData != NULL);

    // Set operational mode
    return DtBcSDITXP_SetOperationalMode(SDITXP_BC, pInData->m_OpMode);
}

//-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXP_OnCmdSetSdiRateAndLvlA2BEna -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcSDITXP_OnCmdSetSdiRateAndLvlA2BEna(
    const DtIoStubBcSDITXP* pStub,
    const DtIoctlSdiTxPCmdSetSdiRateAndLvlA2BEnaInput* pInData)
{

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcSDITXP));
    DT_ASSERT(pInData != NULL);

    // Set SDI mode
    return DtBcSDITXP_SetSdiRateAndLvlA2BEna(SDITXP_BC, pInData->m_SdiRate,
                                                               pInData->m_LevelA2BEnable);
}