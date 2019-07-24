//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDIRXPHY.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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
#include "DtBcSDIRXPHY.h"
#include "DtBcSDIRXPHY_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIRXPHY implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDIRXPHY))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_SDIRXPHY_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(SDIRXPHY, pBc)

// Helper macro to cast a DtBc* to a DtBcSDIRXPHY*
#define BC_SDIRXPHY         ((DtBcSDIRXPHY*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcSDIRXPHY_Init(DtBc*);
static DtStatus  DtBcSDIRXPHY_OnEnable(DtBc*, Bool);
static DtStatus  DtBcSDIRXPHY_OnCloseFile(DtBc*, const DtFileObject*);
static void  DtBcSDIRXPHY_SetControlRegs(DtBcSDIRXPHY*, Bool BlkEnable, 
                                              Int OpMode, Bool RxClkReset,  Bool PllReset,
                                              Int LockMode, Bool  DownsamplerEnable);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIRXPHY - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSDIRXPHY_Close(DtBc*  pBc)
{
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));
       
    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcSDIRXPHY*  DtBcSDIRXPHY_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_SDIRXPHY_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcSDIRXPHY, Id, DT_BLOCK_TYPE_SDIRXPHY,
                                                              Address, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcSDIRXPHY_Close;
    OpenParams.m_InitFunc = DtBcSDIRXPHY_Init;
    OpenParams.m_OnEnableFunc = DtBcSDIRXPHY_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcSDIRXPHY_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcSDIRXPHY*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_GetClockReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_GetClockReset(DtBcSDIRXPHY * pBc, Bool* pClkReset)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pClkReset == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);

    // Return last cached clock reset
    *pClkReset = pBc->m_ClockReset;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_GetOperationalMode(DtBcSDIRXPHY* pBc, Int * pOpMode)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);

    // Return last cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_GetLockMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_GetLockMode(DtBcSDIRXPHY* pBc, Int* pLockMode)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pLockMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
    
    // Return cached value
    *pLockMode = pBc->m_LockMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_IsCarrierDetect -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_IsCarrierDetect(DtBcSDIRXPHY* pBc, Bool* pCarrier)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pCarrier == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);

    // Return carrier detect status
    *pCarrier =  (SDIRXPHY_Status_READ_CarrierDetect(pBc) != 0);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_IsLockedToData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_IsLockedToData(DtBcSDIRXPHY* pBc, Bool* pLocked)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pLocked == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
    
    // Check lock mode
    if (pBc->m_LockMode != DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_DATA)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "ERROR: Not supported for this lockmode");
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Return locked to data
    *pLocked = (SDIRXPHY_Status_READ_LockedToData(pBc) != 0);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_IsLockedToRef -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_IsLockedToRef(DtBcSDIRXPHY* pBc, Bool* pLocked)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pLocked == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
    
    // Check lock mode
    if (pBc->m_LockMode != DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_REF)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "ERROR: Not supported for this lockmode");
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Return locked to reference
    *pLocked = (SDIRXPHY_Status_READ_LockedToRef(pBc) != 0);

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_ResetPll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_ResetPll(DtBcSDIRXPHY* pBc)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
    
    // Operational mode must be IDLE or STANDBY
    if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "Operational mode not in idle/standby");
        return DT_STATUS_INVALID_IN_OPMODE;
    }
    // Make settings in register
    DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                                         pBc->m_OperationalMode, pBc->m_ClockReset, 
                                         TRUE, pBc->m_LockMode, pBc->m_DownsamplerEnable);
    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_SetClockReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_SetClockReset(DtBcSDIRXPHY* pBc, Bool ClkReset)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
    
    // Operational mode must be IDLE or STANDBY
    if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "Operational mode not in idle/standby");
        return DT_STATUS_INVALID_IN_OPMODE;
    }
    // Make settings in register
    DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                                               pBc->m_OperationalMode, ClkReset, FALSE,
                                               pBc->m_LockMode, pBc->m_DownsamplerEnable);
    // Update cache
    pBc->m_ClockReset = ClkReset;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_SetDownsamplerEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSDIRXPHY_SetDownsamplerEnable(DtBcSDIRXPHY* pBc, Bool Enable)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
    
    // Operational mode must be IDLE/STANDBY
    if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "Operational mode not in idle/standby");
        return DT_STATUS_INVALID_IN_OPMODE;
    }
    // Make settings in register
    DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                                                pBc->m_OperationalMode, pBc->m_ClockReset,
                                                FALSE, pBc->m_LockMode, Enable);

    // Update cache
    pBc->m_DownsamplerEnable = Enable;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDIRXPHY_SetOperationalMode(DtBcSDIRXPHY * pBc, Int OpMode)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY 
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
   
    // Clock reset should be de-asserted when going to run
    if (pBc->m_ClockReset && OpMode==DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "Clock reset busy");
        return DT_STATUS_BUSY;
    }
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Make setting in control register
    DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled, OpMode, pBc->m_ClockReset, 
                                        FALSE, pBc->m_LockMode, pBc->m_DownsamplerEnable);
    // Save operational mode
    pBc->m_OperationalMode = OpMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_SetLockMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_SetLockMode(DtBcSDIRXPHY* pBc, Int LockMode)
{
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (LockMode!=DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_DATA && 
                                        LockMode!=DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_REF)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "Invalid lock mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_SDIRXPHY_MUST_BE_ENABLED(pBc);
    
    // Operational mode must be IDLE or STANDBY
    if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, SDIRXPHY, pBc, "Operational mode not in idle");
        return DT_STATUS_INVALID_IN_OPMODE;
    }
    // Make settings in register
    DtBcSDIRXPHY_SetControlRegs(pBc, pBc->m_BlockEnabled,
                                         pBc->m_OperationalMode, pBc->m_ClockReset, FALSE,
                                         LockMode, pBc->m_DownsamplerEnable);
    // Update cache
    pBc->m_LockMode = LockMode;
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIRXPHY - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSDIRXPHY_Init(DtBc*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity checks
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Clear overflow
    SDIRXPHY_CdcFifoStatus_CLEAR_Overflow(BC_SDIRXPHY);

    // Set defaults
    BC_SDIRXPHY->m_BlockEnabled = FALSE;
    BC_SDIRXPHY->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    BC_SDIRXPHY->m_ClockReset = TRUE;
    BC_SDIRXPHY->m_LockMode = DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_REF;
    BC_SDIRXPHY->m_DownsamplerEnable = FALSE;

    // Make settings in register
    DtBcSDIRXPHY_SetControlRegs(BC_SDIRXPHY, BC_SDIRXPHY->m_BlockEnabled,
                                                 BC_SDIRXPHY->m_OperationalMode,
                                                 BC_SDIRXPHY->m_ClockReset,
                                                 FALSE,
                                                 BC_SDIRXPHY->m_LockMode,
                                                 BC_SDIRXPHY->m_DownsamplerEnable);
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDIRXPHY_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status=DT_STATUS_OK;
    DtBcSDIRXPHY* pBc = (DtBcSDIRXPHY*) pBcBase;
 
    // Sanity check
    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);


    if (Enable)
    {
        // DISABLE-> ENABLE
        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
        pBc->m_ClockReset = TRUE;
        pBc->m_LockMode = DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_REF;
        pBc->m_DownsamplerEnable = FALSE;
    }
    else
    {
        // ENABLED -> DISABLED?
        // Operational mode to IDLE
        Status = DtBcSDIRXPHY_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SDIRXPHY, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Make setting in control register
    DtBcSDIRXPHY_SetControlRegs(pBc, Enable, pBc->m_OperationalMode, pBc->m_ClockReset,
                                        FALSE, pBc->m_LockMode, pBc->m_DownsamplerEnable);
    // Save block enable
    pBc->m_BlockEnabled = Enable;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSDIRXPHY_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_SDIRXPHY_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled(pBc))
    {
        DtDbgOutBc(AVG, SDIRXPHY, pBc, "Set operational mode to IDLE");

        // Set operational mode to IDLE
        Status = DtBcSDIRXPHY_SetOperationalMode(BC_SDIRXPHY,
                                                                    DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, SDIRXPHY, pBc, "ERROR: failed to set opmode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXPHY_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSDIRXPHY_SetControlRegs(DtBcSDIRXPHY* pBc, Bool BlkEnable,
                                               Int OpMode, Bool RxClkReset, Bool PllReset,
                                               Int LockMode, Bool DownsamplerEnable)
{
    UInt RegData=0, FldOpMode=0, FldBlkEnable=0, FldLockMode=0, FldRxClkReset=0;

    // Convert block enable to BB-type
    FldBlkEnable = BlkEnable ? SDIRXPHY_BLKENA_ENABLED : SDIRXPHY_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FldOpMode = SDIRXPHY_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FldOpMode = SDIRXPHY_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FldOpMode = SDIRXPHY_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }
    // Convert lockmode to BB-type
    switch (LockMode)
    {
    case DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_DATA:
        FldLockMode= SDIRXPHY_LOCKMODE_LOCK_TO_DATA;
        break;
    case DT_BC_SDIRXPHY_LOCKMODE_LOCK_TO_REF:
        FldLockMode= SDIRXPHY_LOCKMODE_LOCK_TO_REF;
        break;
    default: DT_ASSERT(FALSE);
    }

    // Convert clock reset
    FldRxClkReset = RxClkReset ?  1 : 0;

    // Invalid combination?
    DT_ASSERT(BlkEnable || OpMode==DT_BLOCK_OPMODE_IDLE);

    // Update SDIRXPHY control register
    RegData = SDIRXPHY_Control_READ(pBc);
    RegData = SDIRXPHY_Control_SET_BlockEnable(RegData, FldBlkEnable);
    RegData = SDIRXPHY_Control_SET_OperationalMode(RegData, FldOpMode);
    RegData = SDIRXPHY_Control_SET_RxClkReset(RegData, FldRxClkReset);
    RegData = SDIRXPHY_Control_SET_LockMode(RegData, FldLockMode);
    RegData = SDIRXPHY_Control_SET_PllReset(RegData, PllReset ? 1 : 0);
    RegData = SDIRXPHY_Control_SET_SrcFactor(RegData, DownsamplerEnable ? 
                                          SDIRXPHY_SDIMODE_x11 : SDIRXPHY_SDIMODE_BYPASS);
    SDIRXPHY_Control_WRITE(BC_SDIRXPHY, RegData);
}

