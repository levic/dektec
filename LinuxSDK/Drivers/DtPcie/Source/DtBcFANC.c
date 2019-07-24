//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcFANC.c *#*#*#*#*#*#*#*#*#*#*# (C) 2018 DekTec
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
#include "DtBcFANC.h"
#include "DtBcFANC_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcFANC implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_FANC_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcFANC))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_FANC_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(FANC, pBc)

// Helper macro to cast a DtBc* to a DtBcFANC*
#define BC_FANC         ((DtBcFANC*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcFANC_Init(DtBc*);
static Bool  DtBcFANC_IsFanLess(DtBcFANC*);
void  DtBcFANC_SetControlReg(DtBcFANC * pBc, Int OpState, Bool WdEnable, Int FanSpeed);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcFANC - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFANC_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcFANC_Close(DtBc*  pBc)
{
    BC_FANC_DEFAULT_PRECONDITIONS(pBc);

    // Go back to IDLE
    DtBcFANC_SetControlReg(BC_FANC, FANC_OPSTATE_IDLE, FALSE, 0);
    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFANC_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcFANC*  DtBcFANC_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_FANC_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcFANC, Id, DT_BLOCK_TYPE_FANC,
                                                              Address, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcFANC_Close;
    OpenParams.m_InitFunc = DtBcFANC_Init;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcFANC*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFANC_GetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcFANC_GetConfig(DtBcFANC* pBc, Bool *pHasNoFan, Int* pMeasPeriod,
                                                         Int* pWdTimeout, Int* pInitSpeed)
{
    // Sanity check
    BC_FANC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pHasNoFan==NULL || pMeasPeriod==NULL || pWdTimeout==NULL || pInitSpeed==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return cached configuration
    *pHasNoFan = pBc->m_HasNoFan;
    *pMeasPeriod = pBc->m_MeasurementPeriod;
    *pWdTimeout = pBc->m_WatchdogTimeout;
    *pInitSpeed = pBc->m_InitialFanSpeed;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFANC_GetFanSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcFANC_GetFanSpeed(DtBcFANC* pBc, Int* pFanSpeed)
{
    // Sanity check
    BC_FANC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pFanSpeed == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return cached speed
    *pFanSpeed = pBc->m_FanSpeed;
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFANC_GetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcFANC_GetStatus(DtBcFANC* pBc, Int* pFanSpeedRpm, Bool* pIsWdTimeout, 
                                                                        Bool* pIsFanStuck)
{
    UInt32 StatusReg;

    // Sanity check
    BC_FANC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pFanSpeedRpm==NULL || pIsWdTimeout==NULL || pIsFanStuck==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Get current status
    StatusReg = FANC_Status_READ(pBc);
    *pFanSpeedRpm = (Int)FANC_Status_GET_MeasuredFanSpeedRpm(StatusReg);
    *pIsWdTimeout = (FANC_Status_GET_IsWatchdogTimeOut(StatusReg) != 0);
    *pIsFanStuck = (FANC_Status_GET_IsWatchdogTimeOut(StatusReg) != 0);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFANC_SetFanSpeed -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcFANC_SetFanSpeed(DtBcFANC* pBc, Int FanSpeed)
{
    // Sanity check
    BC_FANC_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (FanSpeed<DT_BC_FANC_MIN_SPEED || FanSpeed>DT_BC_FANC_MAX_SPEED || pBc->m_HasNoFan)
        return DT_STATUS_INVALID_PARAMETER;

    // Set new fan speed
    DtBcFANC_SetControlReg(pBc, FANC_OPSTATE_NORMAL, TRUE, FanSpeed);
    pBc->m_FanSpeed = FanSpeed;
    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcFANC - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFANC_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcFANC_Init(DtBc*  pBcBase)
{
    DtBcFANC* pBc = (DtBcFANC*)pBcBase;

    // Sanity checks
    BC_FANC_DEFAULT_PRECONDITIONS(pBc);

    // Get configuration
    pBc->m_HasNoFan = DtBcFANC_IsFanLess(pBc);
    pBc->m_MeasurementPeriod = FANC_Config_READ_MeasurementPeriod(pBc);
    pBc->m_WatchdogTimeout = FANC_Config_READ_WatchdogTimeOut(pBc);
    // From version 1 the initial fan speed can be read from the configuration
    if (pBc->m_Version >= 1)
        pBc->m_InitialFanSpeed = FANC_Config_READ_InitialFanSpeed(pBc);
    else
        pBc->m_InitialFanSpeed = DT_BC_FANC_DEFAULT_SPEED;
    // Set initial fan speed
    if (!pBc->m_HasNoFan)
    { 
         pBc->m_FanSpeed = pBc->m_InitialFanSpeed;
         DtBcFANC_SetControlReg(pBc, FANC_OPSTATE_NORMAL, TRUE, pBc->m_FanSpeed);
    }
    else
         DtBcFANC_SetControlReg(pBc, FANC_OPSTATE_NO_FAN, FALSE, 0);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFANC_IsFanLess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtBcFANC_IsFanLess(DtBcFANC* pBc)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8  ItemBuf[64];
    Int  NumRead = 0;
    UInt  NoFan = 0;
    // Sanity checks
    DT_ASSERT(pBc->m_pCore->m_pDfVpd != NULL);

    // Check for fanless variant ("NF" keyword with value "1" in VPD)
    NumRead = 0;
    Status = DtCore_VPD_ReadItemRo(pBc->m_pCore,"NF", ItemBuf, DT_SIZEOF_ARRAY(ItemBuf),
                                                                                &NumRead);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutBc(MIN, FANC,pBc, "No-fan '%s'", (const char*)ItemBuf);

        NoFan = DtAnsiCharArrayToUInt((const char*)ItemBuf, NumRead, 10);
    }
    return NoFan==1;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcFANC_SetControlReg -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcFANC_SetControlReg(DtBcFANC* pBc, Int OpState, Bool WdEnable, Int FanSpeed)
{
    UInt32  RegData = FANC_Control_READ(pBc);
    RegData = FANC_Control_SET_EnableWatchdog(RegData, WdEnable);
    RegData = FANC_Control_SET_OperationalState(RegData, OpState);
    RegData = FANC_Control_SET_FanSpeed(RegData, FanSpeed);
    FANC_Control_WRITE(pBc, RegData);
}
