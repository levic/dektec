//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDITXPLL.c *#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
//
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
#include "DtBcSDITXPLL.h"
#include "DtBcSDITXPLL_RegAccess.h"
#include "Messages.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXPLL implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_SDITXPLL_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcSDITXPLL))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_SDITXPLL_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(SDITXPLL, pBc)

// Helper macro to cast a DtBc* to a DtBcSDITXPLL*
#define BC_SDITXPLL         ((DtBcSDITXPLL*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcSDITXPLL_Init(DtBc*);
static DtStatus  DtBcSDITXPLL_OnEnable(DtBc*, Bool);
static DtStatus  DtBcSDITXPLL_CV_ResetClock(DtBcSDITXPLL* pBc);
static DtStatus  DtBcSDITXPLL_C10A10_ResetClock(DtBcSDITXPLL* pBc);
static DtStatus  DtBcSDITXPLL_C10A10_Calibrate(DtBcSDITXPLL* pBc);
static void  DtBcSDITXPLL_SetControlRegs(DtBcSDITXPLL*, Bool BlkEnable, Bool TxClkReset);


//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXPLL - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPLL_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSDITXPLL_Close(DtBc*  pBc)
{
    BC_SDITXPLL_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));
       
    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPLL_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcSDITXPLL*  DtBcSDITXPLL_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_SDITXPLL_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcSDITXPLL, Id, DT_BLOCK_TYPE_SDITXPLL,
                                                              Address, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcSDITXPLL_Close;
    OpenParams.m_InitFunc = DtBcSDITXPLL_Init;
    OpenParams.m_OnEnableFunc = DtBcSDITXPLL_OnEnable;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcSDITXPLL*)DtBc_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPLL_GetPllId -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXPLL_GetPllId(DtBcSDITXPLL* pBc, Int * pPllId)
{
    // Sanity check
    BC_SDITXPLL_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pPllId == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXPLL_MUST_BE_ENABLED(pBc);

    // Return cached PLL-Id
    *pPllId = pBc->m_PllId;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPLL_IsPllLocked -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXPLL_IsPllLocked(DtBcSDITXPLL* pBc, Bool* pLocked)
{
    // Sanity check
    BC_SDITXPLL_DEFAULT_PRECONDITIONS(pBc);

    // Parameter check
    if (pLocked == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_SDITXPLL_MUST_BE_ENABLED(pBc);

    // Return PLL-locked status
    *pLocked = (SDITXPLL_Status_READ_Locked(pBc) != 0);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPLL_ResetClock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXPLL_ResetClock(DtBcSDITXPLL* pBc)
{
     // Sanity check
    BC_SDITXPLL_DEFAULT_PRECONDITIONS(pBc);

    // Must be enabled
    BC_SDITXPLL_MUST_BE_ENABLED(pBc);

    switch (pBc->m_DeviceFamily)
    {
    case DT_BC_SDITXPLL_FAMILY_A10: return DtBcSDITXPLL_C10A10_ResetClock(pBc);
    case DT_BC_SDITXPLL_FAMILY_C10: return DtBcSDITXPLL_C10A10_ResetClock(pBc);
    case DT_BC_SDITXPLL_FAMILY_CV:  return DtBcSDITXPLL_CV_ResetClock(pBc);
    default: DT_ASSERT(FALSE);  return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPLL_CV_ResetClock -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcSDITXPLL_CV_ResetClock(DtBcSDITXPLL* pBc)
{
     // Sanity check
    BC_SDITXPLL_DEFAULT_PRECONDITIONS(pBc);

    // Make settings in register
    DtBcSDITXPLL_SetControlRegs(pBc, pBc->m_BlockEnabled, TRUE);

    return DT_STATUS_OK;
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPLL_C10A10_ResetClock -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
//  This function reset and (re-)calibrates the PLL-clock
//
DtStatus DtBcSDITXPLL_C10A10_ResetClock(DtBcSDITXPLL* pBc)
{
    DtStatus Status = DT_STATUS_FAIL;

    // Sanity check
    BC_SDITXPLL_DEFAULT_PRECONDITIONS(pBc);

    // Reset the PLL prior to performing calibration. Use enable to reset so that we can 
    // hold the PLL in reset for a minimum of 70us.
    DtBcSDITXPLL_SetControlRegs(pBc, FALSE, FALSE);
    DtWaitBlock(70);
    DtBcSDITXPLL_SetControlRegs(pBc, pBc->m_BlockEnabled, FALSE);

    // Now that the block is in reset (disabled) perform the calibration
    Status = DtBcSDITXPLL_C10A10_Calibrate(pBc);

    // Return the status from the calibration
    return Status;
};

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPLL_C10A10_Calibrate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// This function performs the calibration process. Note, it assumes the PLL is held in 
// reset already
//
DtStatus  DtBcSDITXPLL_C10A10_Calibrate(DtBcSDITXPLL* pBc)
{
    UInt32 RegCalEnable;
    UInt64  TimeStart=0, TimeEnd=0;

     // Sanity check
    BC_SDITXPLL_DEFAULT_PRECONDITIONS(pBc);

     // Sanity check
    BC_SDITXPLL_DEFAULT_PRECONDITIONS(pBc);

    // Request user access to the bus (do not use a read-modify-write)
    SDITXPLL_C10A10_PllCalibrationControl_WRITE(pBc, SDITXPLL_OP_USER);
    

    // Wait a maximum of 1ms
    TimeStart = TimeEnd = DtGetTickCountUSec();
    while ((TimeEnd-TimeStart) < (1*1000))
    {   
        if (SDITXPLL_C10A10_PllCalibrationStatus_READ_ConfigBusBusy(pBc) == 0)
            break;

        DtWaitBlock(1);
        TimeEnd  = DtGetTickCountUSec();
    }
    TimeEnd  = DtGetTickCountUSec();

    // Still busy?
    if (SDITXPLL_C10A10_PllCalibrationStatus_READ_ConfigBusBusy(pBc) != 0)
    {
        SDITXPLL_C10A10_PllCalibrationControl_WRITE(pBc, 3);
        return DT_STATUS_TIMEOUT;
    }

    // Enable calibration (must be done through read-modify-write)
    RegCalEnable = SDITXPLL_C10A10_PllCalibrationEnable_READ(pBc);
    if (pBc->m_PllType == DT_BC_SDITXPLL_PLLTYPE_ATX)
        RegCalEnable = SDITXPLL_C10A10_PllCalibrationEnable_SET_Atx(RegCalEnable, 1);
    else
       RegCalEnable = SDITXPLL_C10A10_PllCalibrationEnable_SET_Fpll(RegCalEnable, 1);
    SDITXPLL_C10A10_PllCalibrationEnable_WRITE(pBc, RegCalEnable);

    // Request PreSICE calibration start (do not use a read-modify-write)
    SDITXPLL_C10A10_PllCalibrationControl_WRITE(pBc, SDITXPLL_OP_CAL);

    // Wait shortly to allow calibration request to be acknowledged (takes a few clocks)
    DtWaitBlock(5);

    // Wait till calibration is done
    TimeStart = DtGetTickCountUSec();

    // Wait a maximum of 100ms. 
    // NOTE: typical calibration times are: fPLL=1.3ms and ATX=25.5ms
    TimeStart = TimeEnd = DtGetTickCountUSec();
    while ((TimeEnd-TimeStart) < (100*1000))
    {   
        if (SDITXPLL_Status_READ_CalBusy(pBc) == 0)
            break;
        DtSleep(1);
        TimeEnd  = DtGetTickCountUSec();
    }
    TimeEnd  = DtGetTickCountUSec();

    // Still busy?
    if (SDITXPLL_Status_READ_CalBusy(pBc) != 0)
    {
        DtString  Str;
        DtStringAlloc(&Str, 256);

        DtStringAppendChars(&Str, "[SN=");
        DtStringUInt64ToDtStringAppend(&Str, 10, pBc->m_pCore->m_pDevInfo->m_Serial);
        DtStringAppendChars(&Str, "] ");
        DtStringAppendChars(&Str, "SDI TX PLL calibration has timed out");
        DtEvtLogReport(&pBc->m_pCore->m_Device.m_EvtObject, 
                                              DTPCIE_LOG_ERROR_GENERIC, &Str, NULL, NULL);

        return DT_STATUS_TIMEOUT;
    }
    else
    {
        DtString  Str;
        DtStringAlloc(&Str, 256);

        DtStringAppendChars(&Str, "[SN=");
        DtStringUInt64ToDtStringAppend(&Str, 10, pBc->m_pCore->m_pDevInfo->m_Serial);
        DtStringAppendChars(&Str, "] ");
        DtStringAppendChars(&Str, "SDI TX PLL calibration has been completed in: ");
        DtStringUInt64ToDtStringAppend(&Str, 10, ((UInt32)(TimeEnd-TimeStart))/1000);
        DtStringAppendChars(&Str, "ms");

        DtEvtLogReport(&pBc->m_pCore->m_Device.m_EvtObject, 
                                              DTPCIE_LOG_INFO_GENERIC, &Str, NULL, NULL);
    }
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXPLL - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPLL_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcSDITXPLL_Init(DtBc*  pBcBase)
{
    DtBcSDITXPLL* pBc = (DtBcSDITXPLL*)pBcBase;
    DtStatus  Status=DT_STATUS_OK;
    UInt32  RegData;

    // Sanity checks
    BC_SDITXPLL_DEFAULT_PRECONDITIONS(pBc);

    // Get PLL-Id
    pBc->m_PllId = (Int)SDITXPLL_Config_READ_PllId(pBc);

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_DeviceFamily = DT_BC_SDITXPLL_FAMILY_CV;
    pBc->m_PllType = DT_BC_SDITXPLL_PLLTYPE_CMU;

    // For block version >=1 the device family/pll-type can be read from the config 
    // registers
    if (pBc->m_Version >= 1)
    { 
        // Get device family
        RegData = SDITXPLL_Config_READ_DeviceFamily(pBc);
        switch (RegData)
        {
        case SDITXPLL_FAMILY_A10:  pBc->m_DeviceFamily = DT_BC_SDITXPLL_FAMILY_A10; break;
        case SDITXPLL_FAMILY_C10:  pBc->m_DeviceFamily = DT_BC_SDITXPLL_FAMILY_C10; break;
        case SDITXPLL_FAMILY_CV:   pBc->m_DeviceFamily = DT_BC_SDITXPLL_FAMILY_CV;  break;
        default: DT_ASSERT(FALSE);  return DT_STATUS_FAIL;
        }
        // Get PLL-type
        RegData = SDITXPLL_Config_READ_PllType(pBc);
        switch (RegData)
        {
        case SDITXPLL_PLLTYPE_CMU:  pBc->m_PllType = DT_BC_SDITXPLL_PLLTYPE_CMU; break;
        case SDITXPLL_PLLTYPE_FPLL: pBc->m_PllType = DT_BC_SDITXPLL_PLLTYPE_FPLL; break;
        case SDITXPLL_PLLTYPE_ATX:  pBc->m_PllType = DT_BC_SDITXPLL_PLLTYPE_ATX; break;
        default: DT_ASSERT(FALSE);  return DT_STATUS_FAIL;
        }
    }

    // Make settings in register
    DtBcSDITXPLL_SetControlRegs(pBc, pBc->m_BlockEnabled, FALSE);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPLL_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcSDITXPLL_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtBcSDITXPLL* pBc = (DtBcSDITXPLL*) pBcBase;
 
    // Sanity check
    BC_SDITXPLL_DEFAULT_PRECONDITIONS(pBc);

    // Make setting in control register
    if (Enable)
        DtBcSDITXPLL_SetControlRegs(pBc, Enable, TRUE);
    else
        DtBcSDITXPLL_SetControlRegs(pBc, Enable, FALSE);

    // Save block enable
    pBc->m_BlockEnabled = Enable;

    return DT_STATUS_OK;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXPLL_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcSDITXPLL_SetControlRegs(DtBcSDITXPLL* pBc, Bool BlkEnable, Bool TxClkReset)
{
    UInt32 RegData;
    // Update SDITXPLL control register
    RegData = SDITXPLL_Control_READ(pBc);
    RegData = SDITXPLL_Control_SET_Enable(RegData, BlkEnable ? 1: 0);
    RegData = SDITXPLL_Control_SET_Reset(RegData, TxClkReset ?  1 : 0);
    SDITXPLL_Control_WRITE(BC_SDITXPLL, RegData);
}

