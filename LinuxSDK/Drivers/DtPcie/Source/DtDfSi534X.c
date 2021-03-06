//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSi534X.c *#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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
#include "DtDfSi534X.h"
#include "DtDfSi534X_ConfigData.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// I2C Master role
#define I2CM_ROLE_NONE        NULL
#define TXPLLMGR_ROLE_NONE    NULL
#define DVC_TYPE_SI5342       5342
#define DVC_TYPE_SI5344       5344

// Si534X Frequency offset range
#define SI5354X_MIN_OFFSET_PPT  (-200*1000*1000)     // -200 ppm
#define SI5354X_MAX_OFFSET_PPT  (200*1000*1000)      // +200 ppm

#define  DIV64(x, y)      DtDivideS64((x), (y))

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSi534X implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtDfSi534X function
#define DF_SI534X_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfSi534X))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_SI534X_MUST_BE_ENABLED(pDf)    DF_MUST_BE_ENABLED_IMPL(SI534X, pDf)
static DtStatus  DtDfSi534X_SetConfigInt(DtDfSi534X*, DtDfSi534XConfig);
static DtStatus  DtDfSi534X_Init(DtDf*);
static DtStatus  DtDfSi534X_CleanUp(DtDfSi534X*);
static DtStatus  DtDfSi534X_FindConfigData(DtDfSi534X*, DtDfSi534XConfig,
                                            const DtDfSi534XRegister**, Int* pNumItems);
static void  DtDfSi534X_InitNxNumerators(DtDfSi534X * pDf);
static DtStatus  DtDfSi534X_SetNxNumerator(DtDfSi534X * pDf, Int ClockIdx, Int64 NxNum);
static DtStatus  DtDfSi534X_LoadParameters(DtDf*);
static DtStatus  DtDfSi534X_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfSi534X_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfSi534X_OpenChildren(DtDfSi534X*);
static DtStatus  DtDfSi534X_HardReset(DtDfSi534X*);
static DtStatus  DtDfSi534X_ReadRegister(DtDfSi534X*, DtDfSi534XRegister*);
static DtStatus  DtDfSi534X_WriteConfig(DtDfSi534X*, const DtDfSi534XRegister*, 
                                                         Int  NumItems,  Bool UpdateOnly);
static DtStatus  DtDfSi534X_WriteRegister(DtDfSi534X* , const DtDfSi534XRegister*);


//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSi534X - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfSi534X_Close(DtDf*  pDf)
{
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);

    // Clean-up
    DtDfSi534X_CleanUp((DtDfSi534X*)pDf);
    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfSi534X*  DtDfSi534X_Open(DtCore*  pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    // No stub available
    
    // Init open parameters
    DT_DF_SI534X_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfSi534X, Id, DT_FUNC_TYPE_SI534X, pPt,
                                                                            FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfSi534X_Close;
    OpenParams.m_InitFunc = DtDfSi534X_Init;
    OpenParams.m_OnCloseFileFunc = DtDfSi534X_OnCloseFile;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfSi534X_OnEnablePostChildren;
    OpenParams.m_LoadParsFunc = DtDfSi534X_LoadParameters;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfSi534X*)DtDf_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_GetClockProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSi534X_GetClockProperties(DtDfSi534X* pDf, Int* pNumElems,
                                                      const DtDfSi534XClockProps** pProps)
{
    Bool  SingleClock = (pDf->m_FracClkPortIdx == pDf->m_NonFracClkPortIdx);

    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);
    DF_SI534X_MUST_BE_ENABLED(pDf);

    // Check parameters
    if (pNumElems==NULL || pProps==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Determine which clock properties are used
    if (SingleClock && pDf->m_DeviceType == DVC_TYPE_SI5342)
    {
        *pProps = SI5342_CONFIG_FREE_RUN_SINGLE_CLOCK_PROPS;
        *pNumElems = DT_SIZEOF_ARRAY(SI5342_CONFIG_FREE_RUN_SINGLE_CLOCK_PROPS);
    }
    else if (!SingleClock && pDf->m_DeviceType == DVC_TYPE_SI5342)
    {
        *pProps = SI5342_CONFIG_FREE_RUN_DUAL_CLOCK_PROPS;
        *pNumElems = DT_SIZEOF_ARRAY(SI5342_CONFIG_FREE_RUN_DUAL_CLOCK_PROPS);
    }
    else if (!SingleClock && pDf->m_DeviceType == DVC_TYPE_SI5344)
    {
        *pProps = SI5344_CONFIG_FREE_RUN_DUAL_CLOCK_PROPS;
        *pNumElems = DT_SIZEOF_ARRAY(SI5344_CONFIG_FREE_RUN_DUAL_CLOCK_PROPS);
    }
    else
    {
        DT_ASSERT(FALSE);
        *pNumElems = 0;
        return DT_STATUS_NOT_IMPLEMENTED;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_GetFreqOffsetPpt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSi534X_GetFreqOffsetPpt(DtDfSi534X* pDf, Bool FracClk, Int* pOffsetPpt)
{
    Int  ClkPortIdx = 0;
    Int64  InitNxNum, CurNxNum, MaxMult, Offset;
    static const Int64  Exp12 = 1000LL*1000LL*1000LL*1000LL;

    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);
    DF_SI534X_MUST_BE_ENABLED(pDf);

    // Check parameters
    if (pOffsetPpt == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Protect SI-534X against concurrent access
    DtFastMutexAcquire(&pDf->m_AccessMutex);
    // Convert Clock into ClockIdx
    ClkPortIdx = FracClk ? pDf->m_FracClkPortIdx : pDf->m_NonFracClkPortIdx;
    InitNxNum = pDf->m_InitNxNum[ClkPortIdx];
    CurNxNum = pDf->m_CurNxNum[ClkPortIdx];
    // Release SI-534X mutex
    DtFastMutexRelease(&pDf->m_AccessMutex);

    // Offset = (CurNxNum - InitNxNum)*1E12/ (CurNxNum - 2*InitNxNum)
    // To prevent Int64 overflows:
    // Offset = ((1E12*MaxMult)/(CurNxNum - 2*InitNxNum)) * (CurNxNum - InitNxNum)/MaxMult
    // And prevent division by zero
    MaxMult = DIV64((Int64)((1ULL<<63) - 1), Exp12);
    Offset = 0;
    if (CurNxNum != 2*InitNxNum)
        Offset = DIV64(DIV64(Exp12*MaxMult, (CurNxNum - 2*InitNxNum)) 
                                                       * (CurNxNum - InitNxNum), MaxMult);
    *pOffsetPpt = (Int)Offset;
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_GetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSi534X_GetStatus(DtDfSi534X* pDf, Bool* pLocked, Bool* pCalibDone)
{
    DtDfSi534XRegister SI5344_INTERNAL_STATUS = {0x000C, 0};
    DtDfSi534XRegister SI5344_HOLDOVER_LOL = {0x000E, 0};

    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (pLocked==NULL || pCalibDone==NULL)
        return DT_STATUS_INVALID_PARAMETER;


    DT_RETURN_ON_ERROR(DtDfSi534X_ReadRegister(pDf, &SI5344_INTERNAL_STATUS));
    DT_RETURN_ON_ERROR(DtDfSi534X_ReadRegister(pDf, &SI5344_HOLDOVER_LOL));

    *pCalibDone = ((SI5344_INTERNAL_STATUS.m_Data&(1<<0)) == 0); // Sys in calib == false
    *pLocked = ((SI5344_HOLDOVER_LOL.m_Data&(1<<1)) == 0);     // Loss of lock == false
    return DT_STATUS_OK;

}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_SetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSi534X_SetConfig(DtDfSi534X*  pDf, DtDfSi534XConfig  Config)
{
    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);
    DF_SI534X_MUST_BE_ENABLED(pDf);
    return DtDfSi534X_SetConfigInt(pDf, Config);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_SetFreqOffsetPpt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSi534X_SetFreqOffsetPpt(DtDfSi534X* pDf, Int OffsetPpt, Bool FracClk)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  ClkPortIdx = 0;
    Int64  InitNxNum, NewNxNum, MaxMult;
    static const Int64  Exp12 = 1000LL*1000LL*1000LL*1000LL;

    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);
    DF_SI534X_MUST_BE_ENABLED(pDf);

    // Check parameters
    if (OffsetPpt<SI5354X_MIN_OFFSET_PPT || OffsetPpt>SI5354X_MAX_OFFSET_PPT)
        return DT_STATUS_INVALID_PARAMETER;

    // If we don't have a dual clock, the configuration must match
    if (FracClk && pDf->m_CurConfig==DT_DF_SI534X_CFG_FREE_RUN_NONFRAC_CLOCK)
        return DT_STATUS_INVALID_PARAMETER;
    else if (!FracClk && pDf->m_CurConfig==DT_DF_SI534X_CFG_FREE_RUN_FRAC_CLOCK)
        return DT_STATUS_INVALID_PARAMETER;

    // Protect SI-534X against concurrent access
    DtFastMutexAcquire(&pDf->m_AccessMutex);

    // Convert Clock into ClockIdx
    ClkPortIdx = FracClk ? pDf->m_FracClkPortIdx : pDf->m_NonFracClkPortIdx;

    // Compute new Nx Numerator
    // NewNxNum = InitNxNum + (-OffsetPpt*InitNxNum)/(Exp12 - OffsetPpt)
    // To prevent Int64 overflow using MaxMult (this gives less than 0.01Hz error)
    InitNxNum = pDf->m_InitNxNum[ClkPortIdx];
    DT_ASSERT(InitNxNum > 0);
    if (InitNxNum <= 0)
    {
        DtFastMutexRelease(&pDf->m_AccessMutex);
        return DT_STATUS_FAIL;
    }
    MaxMult = DIV64((Int64)((1ULL<<63) - 1), InitNxNum);
    NewNxNum = InitNxNum + DIV64(DIV64(InitNxNum * MaxMult, (Exp12 - OffsetPpt))
                                                                   * -OffsetPpt, MaxMult);
    // Set new Nx Numerator
    Status =  DtDfSi534X_SetNxNumerator(pDf, ClkPortIdx, NewNxNum);

    // Release SI-534X mutex
    DtFastMutexRelease(&pDf->m_AccessMutex);
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_SetConfigInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set Config for internal use (without IsEnabled check)
// 
DtStatus DtDfSi534X_SetConfigInt(DtDfSi534X*  pDf, DtDfSi534XConfig  Config)
{
    DtStatus  Status = DT_STATUS_OK;
    const DtDfSi534XRegister*  pConfigItems = NULL;
    Int  NumItems=0;
    Int  Timeout;
    Bool Locked=FALSE, CalibDone=FALSE;

    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);

    // Find the configuration data
    Status = DtDfSi534X_FindConfigData(pDf, Config, &pConfigItems, &NumItems);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SI534X, pDf, "ERROR: Could not find configuation: %d", Config);
        return Status;
    }
    DT_ASSERT(pConfigItems!=NULL && NumItems>0);

    // Protect SI-534X against concurrent access
    DtFastMutexAcquire(&pDf->m_AccessMutex);

    // No change?
    if (Config == pDf->m_CurConfig)
    {
        DtFastMutexRelease(&pDf->m_AccessMutex);
        return DT_STATUS_OK;
    }

    // Write preamble
    Status = DtDfSi534X_WriteConfig(pDf, SI534X_CONFIG_PREAMBLE,
                                          DT_SIZEOF_ARRAY(SI534X_CONFIG_PREAMBLE), FALSE);
    if (DT_SUCCESS(Status))
    {
        // Delay 300 msec
        //   Delay is worst case time for device to complete any calibration
        //   that is running due to device state change previous to this script
        //   being processed.
        DtSleep(300);
        // Update registers
        Status = DtDfSi534X_WriteConfig(pDf, pConfigItems, NumItems, TRUE);
    }
    if (DT_SUCCESS(Status))
    { 
        // Write postamble
        Status = DtDfSi534X_WriteConfig(pDf, SI534X_CONFIG_POSTAMBLE,
                                         DT_SIZEOF_ARRAY(SI534X_CONFIG_POSTAMBLE), FALSE);
    }

    // Save new config
    pDf->m_CurConfig = Config;
    pDf->m_pCurConfigItems = pConfigItems;
    pDf->m_CurConfigNumItems = NumItems;

    // Initialize the NX Numerators
    DtDfSi534X_InitNxNumerators(pDf);


    // Wait for clock to become stable; Poll for locked
    Status = DtDfSi534X_GetStatus(pDf, &Locked, &CalibDone);
    Timeout = 200;
    while (DT_SUCCESS(Status) && Timeout>0 && !(Locked && CalibDone))
    {
        Status = DtDfSi534X_GetStatus(pDf, &Locked, &CalibDone);
        DtSleep(10);
        Timeout--;
    }
    if (!DT_SUCCESS(Status))
        DtDbgOutDf(ERR, SI534X, pDf, "Get status failed (status=0x%X)", Status);
    if (Timeout == 0)
    { 
        DtDbgOutDf(ERR, SI534X, pDf, "Lock timeout");
        Status = DT_STATUS_TIMEOUT;
    }

    // Release SI-534X mutex
    DtFastMutexRelease(&pDf->m_AccessMutex);

    return Status;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSi534X - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSi534X_Init(DtDf*  pDfBase)
{
    DtDfSi534X*  pDf = (DtDfSi534X*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);

    // Initialize SI-534X mutex against concurrent access
    DtFastMutexInit(&pDf->m_AccessMutex);
    // Set defaults
    pDf->m_CurConfig = DT_DF_SI534X_CFG_UNDEFINED;
    pDf->m_pCurConfigItems = NULL;
    pDf->m_CurConfigNumItems = 0;

    // Check paramaters have been loaded succesfully
    DT_ASSERT(pDf->m_Si534XAddress >= 0);
    switch (pDf->m_DeviceType)
    {
        case DVC_TYPE_SI5342:       pDf->m_NumClockOutputs = 2; break;
        case DVC_TYPE_SI5344:       pDf->m_NumClockOutputs = 4; break;
        default: DT_ASSERT(FALSE);  pDf->m_NumClockOutputs = 0; break;
    }
    DT_ASSERT(pDf->m_FracClkPortIdx < pDf->m_NumClockOutputs);
    DT_ASSERT(pDf->m_NonFracClkPortIdx < pDf->m_NumClockOutputs);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfSi534X_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SI534X, pDf, "ERROR: failed to open children");
        DtDfSi534X_CleanUp(pDf);
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_CleanUp -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSi534X_CleanUp(DtDfSi534X* pDf)
{
    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_FindConfigData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSi534X_FindConfigData(DtDfSi534X*  pDf, DtDfSi534XConfig  Config,
                                const DtDfSi534XRegister**  pConfigItems, Int*  pNumItems)

{
    DT_ASSERT(pConfigItems!=NULL && pNumItems!=NULL);

    // Set defaults
    *pConfigItems = NULL;
    *pNumItems =  0;

    // SI-534X configurations
    switch (Config)
    {
    case DT_DF_SI534X_CFG_FREE_RUN_DUAL_CLOCK:
        if (pDf->m_FracClkPortIdx == pDf->m_NonFracClkPortIdx)
            return DT_STATUS_INVALID_PARAMETER;
        if (pDf->m_DeviceType == DVC_TYPE_SI5342)
        { 
            *pConfigItems = SI5342_CONFIG_FREE_RUN_DUAL_CLOCK;
            *pNumItems = DT_SIZEOF_ARRAY(SI5342_CONFIG_FREE_RUN_DUAL_CLOCK);
        } else if (pDf->m_DeviceType == DVC_TYPE_SI5344)
        {
            *pConfigItems = SI5344_CONFIG_FREE_RUN_DUAL_CLOCK;
            *pNumItems = DT_SIZEOF_ARRAY(SI5344_CONFIG_FREE_RUN_DUAL_CLOCK);
        } 
        else
            DT_ASSERT(FALSE);
        break;
    case DT_DF_SI534X_CFG_FREE_RUN_NONFRAC_CLOCK:
        *pConfigItems = SI534X_CONFIG_FREE_RUN_148_5MHZ;
        *pNumItems = DT_SIZEOF_ARRAY(SI534X_CONFIG_FREE_RUN_148_5MHZ);
        break;
    case DT_DF_SI534X_CFG_FREE_RUN_FRAC_CLOCK:
        *pConfigItems = SI534X_CONFIG_FREE_RUN_148_35MHZ;
        *pNumItems = DT_SIZEOF_ARRAY(SI534X_CONFIG_FREE_RUN_148_35MHZ);
        break;
    default:
        return DT_STATUS_INVALID_PARAMETER;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_InitNxNumerators -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Assumption: called when having exclusive access
//
void  DtDfSi534X_InitNxNumerators(DtDfSi534X* pDf)
{
    Int ClockIdx, i, j;

    // Save configured Nx Numerators
    for (ClockIdx=0; ClockIdx<pDf->m_NumClockOutputs; ClockIdx++)
    { 
        Int64  NxNum;
        const DtDfSi534XRegister* pCurConf = pDf->m_pCurConfigItems;
        // Nx Numerator registers
        DtDfSi534XRegister SI5344_CONFIG_NX_NUMERATOR[] =
        {
            { 0x0302+ClockIdx*0xB, 0},{ 0x0303+ClockIdx*0xB, 0},{ 0x0304+ClockIdx*0xB, 0},
            { 0x0305+ClockIdx*0xB, 0},{ 0x0306+ClockIdx*0xB, 0},{ 0x0307+ClockIdx*0xB, 0},
        };
        // Get current configured Nx Numerator values
        for (i=0; i<pDf->m_CurConfigNumItems; i++)
            for (j=0; j<DT_SIZEOF_ARRAY(SI5344_CONFIG_NX_NUMERATOR); j++)
                if (pCurConf[i].m_BankAddr == SI5344_CONFIG_NX_NUMERATOR[j].m_BankAddr)
                    SI5344_CONFIG_NX_NUMERATOR[j].m_Data = pCurConf[i].m_Data;

        NxNum = SI5344_CONFIG_NX_NUMERATOR[0].m_Data 
                + ((Int64)(SI5344_CONFIG_NX_NUMERATOR[1].m_Data) << 8)
                + ((Int64)(SI5344_CONFIG_NX_NUMERATOR[2].m_Data) << 16)
                + ((Int64)(SI5344_CONFIG_NX_NUMERATOR[3].m_Data) << 24)
                + ((Int64)(SI5344_CONFIG_NX_NUMERATOR[4].m_Data) << 32)
                + ((Int64)(SI5344_CONFIG_NX_NUMERATOR[5].m_Data&0x0F) << 40);
        // Set initial value
        pDf->m_InitNxNum[ClockIdx] = NxNum;
        // Set cached current value
        pDf->m_CurNxNum[ClockIdx] = NxNum;

    }
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_SetNxNumerator -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSi534X_SetNxNumerator(DtDfSi534X* pDf, Int ClockIdx, Int64 NxNum)
{
    Int i;
    Bool UpdateNeeded = FALSE;
    // Nx Numerator registers
    DtDfSi534XRegister SI5344_CONFIG_NX_NUM[] =
    {
        { 0x0302+ClockIdx*0xB, 0},{ 0x0303+ClockIdx*0xB, 0},{ 0x0304+ClockIdx*0xB, 0},
        { 0x0305+ClockIdx*0xB, 0},{ 0x0306+ClockIdx*0xB, 0},{ 0x0307+ClockIdx*0xB, 0},
    };
    // Nx Update register
    DtDfSi534XRegister SI5344_CONFIG_NX_NUM_UPDATE = { 0x030C+ClockIdx*0xB, 1};
    Int64  CachedNxNum = pDf->m_CurNxNum[ClockIdx];

    SI5344_CONFIG_NX_NUM[0].m_Data = (UInt8)(NxNum&0xFF);
    SI5344_CONFIG_NX_NUM[1].m_Data = (UInt8)((NxNum>>8)&0xFF);
    SI5344_CONFIG_NX_NUM[2].m_Data = (UInt8)((NxNum>>16)&0xFF);
    SI5344_CONFIG_NX_NUM[3].m_Data = (UInt8)((NxNum>>24)&0xFF);
    SI5344_CONFIG_NX_NUM[4].m_Data = (UInt8)((NxNum>>32)&0xFF);
    SI5344_CONFIG_NX_NUM[5].m_Data = (UInt8)((NxNum>>40)&0xFF);

    for (i = 0; i<DT_SIZEOF_ARRAY(SI5344_CONFIG_NX_NUM); i++)
    {
        Int64 Mask = 0xFFLL << (8*i);
        if ((NxNum&Mask) != (CachedNxNum&Mask))
        {
            DT_RETURN_ON_ERROR(DtDfSi534X_WriteRegister(pDf, &SI5344_CONFIG_NX_NUM[i]));
            UpdateNeeded = TRUE;
        }
    }
    if (UpdateNeeded)
        DT_RETURN_ON_ERROR(DtDfSi534X_WriteRegister(pDf, &SI5344_CONFIG_NX_NUM_UPDATE));

    // Update cached value
    pDf->m_CurNxNum[ClockIdx] = NxNum;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSi534X_LoadParameters(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfSi534X* pDf = (DtDfSi534X*)pDfBase;

    // List of SI-534X  function parameters
    DtDfParameters  DFSI534X_PARS[] =
    {
        // Name,  Value Type,  Value*
        { "I2C_ADDR", PROPERTY_VALUE_TYPE_INT, &(pDf->m_Si534XAddress) },
        { "NON_FRAC_CLK_PORT_IDX", PROPERTY_VALUE_TYPE_INT, &(pDf->m_NonFracClkPortIdx) },
        { "FRAC_CLK_PORT_IDX", PROPERTY_VALUE_TYPE_INT, &(pDf->m_FracClkPortIdx) },
        { "DEVICE_TYPE", PROPERTY_VALUE_TYPE_INT, &(pDf->m_DeviceType) },
    };

    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);
 
    // Init parameters to their defaults
    pDf->m_Si534XAddress = -1;
    pDf->m_NonFracClkPortIdx = -1;
    pDf->m_FracClkPortIdx = -1;
    pDf->m_DeviceType = DVC_TYPE_SI5342;

    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFSI534X_PARS), DFSI534X_PARS);
    if (!DT_SUCCESS(Status))
        return Status;
 
    // Check paramaters have been loaded succesfully
    DT_ASSERT(pDf->m_Si534XAddress >= 0);
    DT_ASSERT(pDf->m_FracClkPortIdx>=0 && pDf->m_NonFracClkPortIdx>=0);
    DT_ASSERT(pDf->m_DeviceType==DVC_TYPE_SI5342 || pDf->m_DeviceType==DVC_TYPE_SI5344);
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtDfSi534X_OnCloseFile(DtDf* pDf, const DtFileObject* pFile)
{
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);

    // Use base function to release exclusive access
    return DtDf_OnCloseFile(pDf, pFile);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSi534X_OnEnablePostChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfSi534X* pDf = (DtDfSi534X*)pDfBase;

    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);

    // If enable, reset the SI-534X
    if (Enable)
    {
        // Perform hard reset
        if (DT_SUCCESS(Status))
        Status = DtDfSi534X_HardReset(pDf);
        if (DT_SUCCESS(Status))
        {
            // Sleep 100ms
            DtSleep(100);

            // Configure free-running clock(s)
            if (pDf->m_NonFracClkPortIdx == pDf->m_FracClkPortIdx)
                Status = DtDfSi534X_SetConfigInt(pDf, 
                                                 DT_DF_SI534X_CFG_FREE_RUN_NONFRAC_CLOCK);
            else
                Status = DtDfSi534X_SetConfigInt(pDf, 
                                                    DT_DF_SI534X_CFG_FREE_RUN_DUAL_CLOCK);
        }
        if (DT_SUCCESS(Status))
        { 
            // In case the TXPLLs are children of the Si534X driver function,
            // reset/recalibrate the TXPLLs

            // Wait shortly, before resetting/calibrating the PLLs, to allow ALTERA 
            // power-up calibration to complete. Starting a user calibration, while 
            // power-up calibration is busy results in calibration to timeout (fail).
            DtSleep(500);

            if (pDf->m_pDfTxPllMgr != NULL)
                Status = DtDfTxPllMgr_Calibrate(pDf->m_pDfTxPllMgr);
        }
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSi534X_OpenChildren(DtDfSi534X*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

     // List of children supported by the the SI534X function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_I2CM, DT_BC_I2CM_NAME, 
                               I2CM_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcI2Cm), TRUE},
        { DT_OBJECT_TYPE_DF, DT_FUNC_TYPE_TXPLLMGR, DT_DF_TXPLLMGR_NAME, 
                       TXPLLMGR_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pDfTxPllMgr), FALSE}
    };

    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
     if (!DT_SUCCESS(Status))
        return Status;

    // Check mandatory children have been loaded (i.e. shortcut is valid)
    DT_ASSERT(pDf->m_pBcI2Cm != NULL);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_HardReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus   DtDfSi534X_HardReset(DtDfSi534X* pDf)
{
    DtStatus Status = DT_STATUS_OK;

    // Write hard reset
    Status = DtDfSi534X_WriteConfig(pDf, SI534X_CONFIG_HARDRST,
                                           DT_SIZEOF_ARRAY(SI534X_CONFIG_HARDRST), FALSE);

    // Ignore result? wait for Andrew's investigation

    // Configuration becomes undefined
    pDf->m_PrevBank = -1;
    pDf->m_CurConfig = DT_DF_SI534X_CFG_UNDEFINED;
    pDf->m_pCurConfigItems = NULL;
    pDf->m_CurConfigNumItems = 0;

    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_ReadRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSi534X_ReadRegister(DtDfSi534X* pDf, DtDfSi534XRegister*  pConfigItem)
{
    UInt8 WrBuffer[1];
    UInt8 RdBuffer[1];
    UInt8 Bank = (pConfigItem->m_BankAddr >> 8);

    DT_ASSERT(pDf->m_Si534XAddress >= 0);

    if (Bank != pDf->m_PrevBank)
    {
        UInt8 Buffer[2];
        // Change bank
        Buffer[0] = 0x01;
        Buffer[1] = Bank;
        DT_RETURN_ON_ERROR(DtBcI2CM_Write(pDf->m_pBcI2Cm, pDf->m_Si534XAddress,
                                                                 sizeof(Buffer), Buffer));
        pDf->m_PrevBank = Bank;
    }
    // Write address and read data
    WrBuffer[0] = (UInt8)pConfigItem->m_BankAddr;
    DT_RETURN_ON_ERROR(DtBcI2CM_WriteRead(pDf->m_pBcI2Cm, pDf->m_Si534XAddress,
                                                               1, WrBuffer, 1, RdBuffer));
    pConfigItem->m_Data = RdBuffer[0];
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_WriteConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSi534X_WriteConfig(DtDfSi534X* pDf,const DtDfSi534XRegister* 
                                             pConfigItems, Int NumItems, Bool UpdateOnly)
{
    Int i;
    for (i=0; i<NumItems; i++)
    { 
        // Do we need to update the item?
        if (!UpdateOnly || pDf->m_pCurConfigItems==NULL || pDf->m_CurConfigNumItems<i
            || pDf->m_pCurConfigItems[i].m_BankAddr!=pConfigItems[i].m_BankAddr
            || pDf->m_pCurConfigItems[i].m_Data!=pConfigItems[i].m_Data)
        { 
            DT_RETURN_ON_ERROR(DtDfSi534X_WriteRegister(pDf, &pConfigItems[i]));
        }
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_WriteRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSi534X_WriteRegister(DtDfSi534X* pDf, const DtDfSi534XRegister* pConfigItem)
{
    UInt8 Buffer[2];
    UInt8 Bank = (pConfigItem->m_BankAddr >> 8);

    DT_ASSERT(pDf->m_Si534XAddress >= 0);

    if (Bank != pDf->m_PrevBank)
    {
        // Change bank
        Buffer[0] = 0x01;
        Buffer[1] = Bank;
        DT_RETURN_ON_ERROR(DtBcI2CM_Write(pDf->m_pBcI2Cm, pDf->m_Si534XAddress,
                                                                 sizeof(Buffer), Buffer));
        pDf->m_PrevBank = Bank;
    }
    // Write data
    Buffer[0] = (UInt8)pConfigItem->m_BankAddr;
    Buffer[1] = pConfigItem->m_Data;
    return DtBcI2CM_Write(pDf->m_pBcI2Cm, pDf->m_Si534XAddress, sizeof(Buffer), Buffer);
}
