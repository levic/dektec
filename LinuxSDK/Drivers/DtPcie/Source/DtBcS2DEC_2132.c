//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcS2DEC_2132.c *#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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
#include "DtBcS2DEC_2132.h"
#include "DtBcS2DEC_2132_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS2DEC_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_S2DEC_2132_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcS2DEC_2132))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_S2DEC_2132_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(S2DEC_2132, pBc)

// Helper macro to cast a DtBc* to a DtBcS2DEC_2132*
#define BC_S2DEC_2132         ((DtBcS2DEC_2132*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcS2DEC_2132_Init(DtBc*);
static DtStatus  DtBcS2DEC_2132_OnEnable(DtBc * pBc, Bool Enable);
static DtStatus  DtBcS2DEC_2132_OnCloseFile(DtBc*, const DtFileObject*);

//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS2DEC_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEC_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcS2DEC_2132_Close(DtBc*  pBc)
{
    BC_S2DEC_2132_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEC_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcS2DEC_2132*  DtBcS2DEC_2132_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_S2DEC_2132_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcS2DEC_2132, Id, DT_BLOCK_TYPE_S2DEC_2132, 
                                                              Address, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcS2DEC_2132_Close;
    OpenParams.m_InitFunc = DtBcS2DEC_2132_Init;
    OpenParams.m_OnEnableFunc = DtBcS2DEC_2132_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcS2DEC_2132_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcS2DEC_2132*)DtBc_Open(&OpenParams);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEC_2132_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcS2DEC_2132_GetOperationalMode(DtBcS2DEC_2132* pBc, Int* pOpMode)
{
    // Sanity check
    BC_S2DEC_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S2DEC_2132_MUST_BE_ENABLED(pBc);

    // Return last cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEC_2132_GetSnrInfo -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcS2DEC_2132_GetRollOff(DtBcS2DEC_2132* pBc, Int* pRollOff)
{
    UInt32 RegData;

    // Sanity check
    BC_S2DEC_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pRollOff == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S2DEC_2132_MUST_BE_ENABLED(pBc);

    // Read detected roll-off
    RegData = S2DEC_BBRollOffDetector_READ(pBc);
    if (S2DEC_BBRollOffDetector_GET_RollOff0Valid(RegData)==0
                                 || S2DEC_BBRollOffDetector_GET_RollOff1Valid(RegData)==0)
    {
        *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_UNKNOWN;
    }
    else
    {
        UInt32 RollOff = (S2DEC_BBRollOffDetector_GET_RollOff1(RegData)<<2)
                                          | S2DEC_BBRollOffDetector_GET_RollOff0(RegData);
        // According ETSI EN 302 307-2 section 5.1.6.  
        switch (RollOff)
        {
        case 0x0: /* 00 00 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_0_35; break;
        case 0x1: /* 00 01 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_UNKNOWN; break;
        case 0x2: /* 00 10 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_UNKNOWN; break;
        case 0x3: /* 00 11 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_0_15; break;
        case 0x4: /* 01 00 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_UNKNOWN; break;
        case 0x5: /* 01 01 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_0_25; break;
        case 0x6: /* 01 10 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_UNKNOWN; break;
        case 0x7: /* 01 11 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_0_10; break;
        case 0x8: /* 10 00 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_UNKNOWN; break;
        case 0x9: /* 10 01 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_UNKNOWN; break;
        case 0xA: /* 10 10 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_0_20; break;
        case 0xB: /* 10 11 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_0_05; break;
        case 0xC: /* 11 00 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_0_15; break;
        case 0xD: /* 11 01 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_0_10; break;
        case 0xE: /* 11 10 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_0_05; break;
        case 0xF: /* 11 11 */   *pRollOff = DT_BC_S2DEC_2132_ROLL_OFF_UNKNOWN; break;
        default: DT_ASSERT(FALSE);
        }
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEC_2132_GetCounters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcS2DEC_2132_GetCounters(DtBcS2DEC_2132* pBc, UInt* pFrameCount,
    UInt* pFrameSkipCount, UInt* pLdcpSuccessCount, UInt* pLdcpIterationsCount,
    UInt* pBchSuccessCount)
{
    // Sanity check
    BC_S2DEC_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pFrameCount == NULL || pFrameSkipCount == NULL || pLdcpSuccessCount == NULL || 
                                 pLdcpIterationsCount == NULL || pBchSuccessCount == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_S2DEC_2132_MUST_BE_ENABLED(pBc);

    // Freeze counters
    S2DEC_CounterLock_WRITE(pBc, 0x01);

    // Get values
    *pFrameCount = S2DEC_FrameCounter_READ(pBc);
    *pFrameSkipCount = S2DEC_FrameSkipCounter_READ(pBc);
    *pLdcpSuccessCount = S2DEC_LdpcSuccessCounter_READ(pBc);
    *pLdcpIterationsCount = S2DEC_LdpcItCnt_READ(pBc);
    *pBchSuccessCount = S2DEC_BchSuccessCounter_READ(pBc);

    // Reset counters
    S2DEC_FrameCounter_WRITE(pBc, 0);
    S2DEC_FrameSkipCounter_WRITE(pBc, 0);
    S2DEC_LdpcSuccessCounter_WRITE(pBc, 0);
    S2DEC_LdpcItCnt_WRITE(pBc, 0);
    S2DEC_BchSuccessCounter_WRITE(pBc, 0);

    // Unfreeze counters
    S2DEC_CounterLock_WRITE(pBc, 0x00);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEC_2132_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcS2DEC_2132_SetOperationalMode(DtBcS2DEC_2132* pBc, Int OpMode)
{
    // Sanity check
    BC_S2DEC_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, S2DEC_2132, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_S2DEC_2132_MUST_BE_ENABLED(pBc);

    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // IDLE -> RUN
    if (OpMode == DT_BLOCK_OPMODE_RUN)
        DtDbgOutBc(AVG, S2DEC_2132, pBc, "OpMode from IDLE -> RUN");
    else
        // RUN->IDLE
        DtDbgOutBc(AVG, S2DEC_2132, pBc, "OpMode from RUN -> IDLE");

    // No further action needed yet

    // Save new setting
    pBc->m_OperationalMode = OpMode;


    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcS2DEC_2132 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEC_2132_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcS2DEC_2132_Init(DtBc*  pBc)
{
    // Sanity checks
    BC_S2DEC_2132_DEFAULT_PRECONDITIONS(pBc);


    // Set defaults
    BC_S2DEC_2132->m_BlockEnabled = FALSE;
    BC_S2DEC_2132->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    // Currently there are no associated registers

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEC_2132_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcS2DEC_2132_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcS2DEC_2132* pBc = (DtBcS2DEC_2132*)pBcBase;

    // Sanity check
    BC_S2DEC_2132_DEFAULT_PRECONDITIONS(pBc);

    // DISABLE -> ENABLE
    if (Enable )
    { 
        // DISABLE -> ENABLE' set cached values and enable
        DtDbgOutBc(AVG, S2DEC_2132, pBc, "Block Enable from disable -> enable");

    }
    else
    { 
        // ENABLE -> DISABLE' disable and set defaults
        DtDbgOutBc(AVG, S2DEC_2132, pBc, "Block Enable from enable -> disable");
        // Operational mode to IDLE
        Status = DtBcS2DEC_2132_SetOperationalMode(BC_S2DEC_2132, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DT_ASSERT(FALSE);
            DtDbgOutBc(ERR, S2DEC_2132, pBc, "Failed to set opmode to idle");
        }  
    }

    // Save new setting
    pBc->m_BlockEnabled = Enable;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcS2DEC_2132_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcS2DEC_2132_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_S2DEC_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutBc(AVG, S2DEC_2132, pBc, "Set operational mode to IDLE");

        // Set operational mode to IDLE
        Status = DtBcS2DEC_2132_SetOperationalMode(BC_S2DEC_2132, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, S2DEC_2132, pBc, "ERROR: failed to set opmode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}
