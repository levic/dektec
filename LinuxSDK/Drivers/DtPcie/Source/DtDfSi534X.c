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
#define DVC_TYPE_SI5392       5392

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
static DtStatus  DtDfSi534X_DetermineDeviceType(DtDfSi534X*);
static DtStatus  DtDfSi534X_FindConfigData(DtDfSi534X*, DtDfSi534XConfig,
                                const DtDfSi534XRegister**, Int* pNumItems,
                                const DtDfSi534XClockProps** pClockProps, Int* pNumProps);
static DtStatus DtDfSi534X_GetStatus(DtDfSi534X* pDf, Bool* pLocked, Bool* pCalibDone);
static void  DtDfSi534X_InitNxNumerators(DtDfSi534X * pDf);
static DtStatus  DtDfSi534X_SetNxNumerator(DtDfSi534X * pDf, Int ClockIdx, Int64 NxNum);
static DtStatus  DtDfSi534X_LoadParameters(DtDf*);
static DtStatus  DtDfSi534X_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfSi534X_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfSi534X_OpenChildren(DtDfSi534X*);
static DtStatus  DtDfSi534X_HardReset(DtDfSi534X*);
static DtStatus  DtDfSi534X_ReadRegister(DtDfSi534X*, DtDfSi534XRegister*);

static DtStatus  DtDfSi534X_StoreCurConfigItems(DtDfSi534X*, const DtDfSi534XRegister*,
                                                                                     Int);
static DtStatus  DtDfSi534X_WriteCurrentConfig(DtDfSi534X*);
static DtStatus  DtDfSi534X_WriteConfigItems(DtDfSi534X*, const DtDfSi534XRegister*, 
                                                         Int  NumItems,  Bool UpdateOnly);
static DtStatus  DtDfSi534X_WriteRegister(DtDfSi534X* , const DtDfSi534XRegister*);
static int DtDfSi534X_GetClockIdx(DtDfSi534X* pDf, Bool Fractional);
static int DtDfSi534X_GetNumClocks(DtDfSi534X* pDf);
static Bool DtDfSi534X_IsInFreeRunOnlyMode(DtDfSi534X* pDf);

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
    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);
    DF_SI534X_MUST_BE_ENABLED(pDf);

    // Check parameters
    if (pNumElems==NULL || pProps==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    *pProps = pDf->m_ClockProps;
    *pNumElems =  pDf->m_NumClockProps;
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
    
    // Frequency cannot be set in free-run-only-mode
    if (DtDfSi534X_IsInFreeRunOnlyMode(pDf))
    {
        *pOffsetPpt = 0;
        return DT_STATUS_OK;
    }

    // Protect SI-534X against concurrent access
    DtFastMutexAcquire(&pDf->m_AccessMutex);
    // Convert Clock into ClockIdx
    ClkPortIdx = DtDfSi534X_GetClockIdx(pDf, FracClk);
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
    if (CurNxNum!=2*InitNxNum && InitNxNum>0)
        Offset = DIV64(DIV64(Exp12*MaxMult, (CurNxNum - 2*InitNxNum)) 
                                                       * (CurNxNum - InitNxNum), MaxMult);
    *pOffsetPpt = (Int)Offset;
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

    // If we don't have a dual clock, the configuration must match
    if (FracClk && pDf->m_CurConfig==DT_DF_SI534X_CFG_NON_FRAC_SDI_CLOCK)
        return DT_STATUS_INVALID_PARAMETER;
    else if (!FracClk && pDf->m_CurConfig==DT_DF_SI534X_CFG_FRAC_SDI_CLOCK)
        return DT_STATUS_INVALID_PARAMETER;

    // Frequency cannot be set in free-run-only-mode
    if (DtDfSi534X_IsInFreeRunOnlyMode(pDf))
    {
        if (OffsetPpt == 0)
            return DT_STATUS_OK;
        else
            return DT_STATUS_INVALID_PARAMETER;
    }

    // Protect SI-534X against concurrent access
    DtFastMutexAcquire(&pDf->m_AccessMutex);

    // Convert Clock into ClockIdx
    ClkPortIdx = DtDfSi534X_GetClockIdx(pDf, FracClk);

    // Check range
    if (OffsetPpt<-pDf->m_ClockProps[ClkPortIdx].m_RangePpt 
                                    || OffsetPpt>pDf->m_ClockProps[ClkPortIdx].m_RangePpt)
    {
        DtFastMutexRelease(&pDf->m_AccessMutex);
        return DT_STATUS_INVALID_PARAMETER;
    }
    
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
    Int  NumItems=0, NumProps=0, i=0;
    const DtDfSi534XRegister*  pConfigItems = NULL;
    const DtDfSi534XClockProps*  pClockProps = NULL;

    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);

    // Find the configuration data
    Status = DtDfSi534X_FindConfigData(pDf, Config, &pConfigItems, &NumItems,
                                                    &pClockProps, &NumProps);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SI534X, pDf, "ERROR: Could not find configuration: %d", Config);
        return Status;
    }
    DT_ASSERT(pConfigItems!=NULL && NumItems>0);
    DT_ASSERT(pClockProps!=NULL && NumProps>0 && NumProps<=DT_DF_SI534X_MAX_NUM_CLOCKS);
    
    // No change?
    if (Config == pDf->m_CurConfig)
    {
        return DT_STATUS_OK;
    }
    // Protect SI-534X against concurrent access
    DtFastMutexAcquire(&pDf->m_AccessMutex);

    // Save new config
    Status = DtDfSi534X_StoreCurConfigItems(pDf, pConfigItems, NumItems);
    if (!DT_SUCCESS(Status))
    {
        DtFastMutexRelease(&pDf->m_AccessMutex);
        return Status;
    }
    for (i=0; i<NumProps; i++)
       pDf->m_ClockProps[i] = pClockProps[i];
    pDf->m_NumClockProps = NumProps;

    // Initialize the NX Numerators
    DtDfSi534X_InitNxNumerators(pDf);

    // Now write the current configuration to the Si534X device
    Status = DtDfSi534X_WriteCurrentConfig(pDf);
    if (DT_SUCCESS(Status))
        pDf->m_CurConfig = Config;

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
    pDf->m_NumClockProps = 0;
    pDf->m_DeviceType = -1;

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

    if (pDf->m_pCurConfigItems != NULL)
    {
        DtMemFreePool(pDf->m_pCurConfigItems, DF_TAG);
        pDf->m_pCurConfigItems = NULL;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_DetermineDeviceProps -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Reads device type from the device and initializes the device properties.
//
DtStatus  DtDfSi534X_DetermineDeviceType(DtDfSi534X* pDf)
{
    Int TypeNr, Grade, Revision;
    DtDfSi534XRegister SI534X_PN_BASE1 = {0x0002, 0};
    DtDfSi534XRegister SI534X_PN_BASE2 = {0x0003, 0};
    DtDfSi534XRegister SI534X_GRADE = {0x0004, 0};
    DtDfSi534XRegister SI534X_DEVICE_REV = {0x0005, 0};

    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);

    DT_RETURN_ON_ERROR(DtDfSi534X_ReadRegister(pDf, &SI534X_PN_BASE1));
    DT_RETURN_ON_ERROR(DtDfSi534X_ReadRegister(pDf, &SI534X_PN_BASE2));
    DT_RETURN_ON_ERROR(DtDfSi534X_ReadRegister(pDf, &SI534X_GRADE));
    DT_RETURN_ON_ERROR(DtDfSi534X_ReadRegister(pDf, &SI534X_DEVICE_REV));

    TypeNr = (SI534X_PN_BASE2.m_Data << 8) + SI534X_PN_BASE1.m_Data;
    Grade = SI534X_GRADE.m_Data;
    Revision = SI534X_DEVICE_REV.m_Data;
    switch (TypeNr)
    {
    case 0x5342:  pDf->m_DeviceType = DVC_TYPE_SI5342; break;
    case 0x5392:  pDf->m_DeviceType = DVC_TYPE_SI5392; break;
    default:    return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_FindConfigData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSi534X_FindConfigData(DtDfSi534X*  pDf, DtDfSi534XConfig  Config,
    const DtDfSi534XRegister**  pConfigItems, Int*  pNumItems,
    const DtDfSi534XClockProps**  pClockProps, Int* pNumProps)
{
    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pConfigItems!=NULL && pNumItems!=NULL);
    DT_ASSERT(pDf->m_DeviceType==DVC_TYPE_SI5342 || pDf->m_DeviceType==DVC_TYPE_SI5392);

    // Set defaults
    *pConfigItems = NULL;
    *pNumItems = 0;
    *pClockProps = NULL;
    *pNumProps = 0;
    // SI-534X configurations
    switch (Config)
    {
    case DT_DF_SI534X_CFG_DUAL_SDI_CLOCK:
        switch (pDf->m_ClockArchitecture)
        {
        case SI534X_DTA2172_LIKE:
            if (pDf->m_DeviceType == DVC_TYPE_SI5342)
            { 
                // SI5342
                *pConfigItems = SI5342_CONFIG_DUAL_SDI_CLOCK_DTA2172_LIKE;
                *pNumItems = DT_SIZEOF_ARRAY(SI5342_CONFIG_DUAL_SDI_CLOCK_DTA2172_LIKE);
                *pClockProps = SI5342_CONFIG_SDI_CLOCK_PROPS_DTA2172_LIKE;
                *pNumProps = DT_SIZEOF_ARRAY(SI5342_CONFIG_SDI_CLOCK_PROPS_DTA2172_LIKE);
            } else {
                // SI5392
                *pConfigItems = SI5392_CONFIG_DUAL_SDI_CLOCK_DTA2172_LIKE;
                *pNumItems = DT_SIZEOF_ARRAY(SI5392_CONFIG_DUAL_SDI_CLOCK_DTA2172_LIKE);
                *pClockProps = SI5392_CONFIG_SDI_CLOCK_PROPS_DTA2172_LIKE;
                *pNumProps = DT_SIZEOF_ARRAY(SI5392_CONFIG_SDI_CLOCK_PROPS_DTA2172_LIKE);
            }
            break;
        default:
            return DT_STATUS_INVALID_PARAMETER;
        }
        break;
    case DT_DF_SI534X_CFG_NON_FRAC_SDI_CLOCK:
        switch (pDf->m_ClockArchitecture)
        {
        case SI534X_DTA2127_LIKE:
            if (pDf->m_DeviceType == DVC_TYPE_SI5342)
            {
                // SI5342
                *pConfigItems = SI5342_CONFIG_NON_FRAC_SDI_CLOCK_DTA2127_LIKE;
                *pNumItems = DT_SIZEOF_ARRAY(SI5342_CONFIG_NON_FRAC_SDI_CLOCK_DTA2127_LIKE);
                *pClockProps = SI5342_CONFIG_NON_FRAC_SDI_CLOCK_PROPS_DTA2127_LIKE;
                *pNumProps = DT_SIZEOF_ARRAY(SI5342_CONFIG_NON_FRAC_SDI_CLOCK_PROPS_DTA2127_LIKE);
            } else {
                // SI5392
                *pConfigItems = SI5392_CONFIG_NON_FRAC_SDI_CLOCK_DTA2127_LIKE;
                *pNumItems = DT_SIZEOF_ARRAY(SI5392_CONFIG_NON_FRAC_SDI_CLOCK_DTA2127_LIKE);
                *pClockProps = SI5392_CONFIG_NON_FRAC_SDI_CLOCK_PROPS_DTA2127_LIKE;
                *pNumProps = DT_SIZEOF_ARRAY(SI5392_CONFIG_NON_FRAC_SDI_CLOCK_PROPS_DTA2127_LIKE);
            }
            break;
        case SI534X_DTA2175_LIKE:
            if (pDf->m_DeviceType == DVC_TYPE_SI5342)
            {
                // SI5342
                *pConfigItems = SI5342_CONFIG_NON_FRAC_SDI_CLOCK_DTA2175_LIKE;
                *pNumItems = DT_SIZEOF_ARRAY(SI5342_CONFIG_NON_FRAC_SDI_CLOCK_DTA2175_LIKE);
                *pClockProps = SI5342_CONFIG_NON_FRAC_SDI_CLOCK_PROPS_DTA2175_LIKE;
                *pNumProps = DT_SIZEOF_ARRAY(SI5342_CONFIG_NON_FRAC_SDI_CLOCK_PROPS_DTA2175_LIKE);
            } else {
                // SI5392
                *pConfigItems = SI5392_CONFIG_NON_FRAC_SDI_CLOCK_DTA2175_LIKE;
                *pNumItems = DT_SIZEOF_ARRAY(SI5392_CONFIG_NON_FRAC_SDI_CLOCK_DTA2175_LIKE);
                *pClockProps = SI5392_CONFIG_NON_FRAC_SDI_CLOCK_PROPS_DTA2175_LIKE;
                *pNumProps = DT_SIZEOF_ARRAY(SI5392_CONFIG_NON_FRAC_SDI_CLOCK_PROPS_DTA2175_LIKE);
            }
            break;
        default:
            return DT_STATUS_INVALID_PARAMETER;
        }
        break;
    case DT_DF_SI534X_CFG_FRAC_SDI_CLOCK:
        switch (pDf->m_ClockArchitecture)
        {
        case SI534X_DTA2127_LIKE:
            if (pDf->m_DeviceType == DVC_TYPE_SI5342)
            {
                // SI5342
                *pConfigItems = SI5342_CONFIG_FRAC_SDI_CLOCK_DTA2127_LIKE;
                *pNumItems = DT_SIZEOF_ARRAY(SI5342_CONFIG_FRAC_SDI_CLOCK_DTA2127_LIKE);
                *pClockProps = SI5342_CONFIG_FRAC_SDI_CLOCK_PROPS_DTA2127_LIKE;
                *pNumProps = DT_SIZEOF_ARRAY(SI5342_CONFIG_FRAC_SDI_CLOCK_PROPS_DTA2127_LIKE);
            } else {
                // SI5392
                *pConfigItems = SI5392_CONFIG_FRAC_SDI_CLOCK_DTA2127_LIKE;
                *pNumItems = DT_SIZEOF_ARRAY(SI5392_CONFIG_FRAC_SDI_CLOCK_DTA2127_LIKE);
                *pClockProps = SI5392_CONFIG_FRAC_SDI_CLOCK_PROPS_DTA2127_LIKE;
                *pNumProps = DT_SIZEOF_ARRAY(SI5392_CONFIG_FRAC_SDI_CLOCK_PROPS_DTA2127_LIKE);
            }
            break;
        case SI534X_DTA2175_LIKE:
            if (pDf->m_DeviceType == DVC_TYPE_SI5342)
            {
                // SI5342
                *pConfigItems = SI5342_CONFIG_FRAC_SDI_CLOCK_DTA2175_LIKE;
                *pNumItems = DT_SIZEOF_ARRAY(SI5342_CONFIG_FRAC_SDI_CLOCK_DTA2175_LIKE);
                *pClockProps = SI5342_CONFIG_FRAC_SDI_CLOCK_PROPS_DTA2175_LIKE;
                *pNumProps = DT_SIZEOF_ARRAY(SI5342_CONFIG_FRAC_SDI_CLOCK_PROPS_DTA2175_LIKE);
            } else {
                // SI5392
                *pConfigItems = SI5392_CONFIG_FRAC_SDI_CLOCK_DTA2175_LIKE;
                *pNumItems = DT_SIZEOF_ARRAY(SI5392_CONFIG_FRAC_SDI_CLOCK_DTA2175_LIKE);
                *pClockProps = SI5392_CONFIG_FRAC_SDI_CLOCK_PROPS_DTA2175_LIKE;
                *pNumProps = DT_SIZEOF_ARRAY(SI5392_CONFIG_FRAC_SDI_CLOCK_PROPS_DTA2175_LIKE);
            }
            break;
        default:
            return DT_STATUS_INVALID_PARAMETER;
        }
        break;
        // Non-SDI clock outputs
    case DT_DF_SI534X_CFG_DUAL_CLOCK:
        switch (pDf->m_ClockArchitecture)
        {
        case SI534X_DTA2131B_LIKE:
            if (pDf->m_DeviceType != DVC_TYPE_SI5392)
                return DT_STATUS_INVALID_PARAMETER;

            // SI5392
            *pConfigItems = SI5392_CONFIG_CLOCK_DTA2131B_LIKE;
            *pNumItems = DT_SIZEOF_ARRAY(SI5392_CONFIG_CLOCK_DTA2131B_LIKE);
            *pClockProps = SI5392_CONFIG_CLOCK_PROPS_DTA2131B_LIKE;
            *pNumProps = DT_SIZEOF_ARRAY(SI5392_CONFIG_CLOCK_PROPS_DTA2131B_LIKE);
            break;
        default:
            return DT_STATUS_INVALID_PARAMETER;
        }
        break;
    default:
        return DT_STATUS_INVALID_PARAMETER;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_GetDefaultConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSi534X_GetDefaultConfig(DtDfSi534X* pDf, DtDfSi534XConfig* pConfig)
{
    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pConfig != NULL);

    switch (pDf->m_ClockArchitecture)
    {
    case SI534X_DTA2127_LIKE:
    case SI534X_DTA2175_LIKE:
        *pConfig = DT_DF_SI534X_CFG_NON_FRAC_SDI_CLOCK;
        break;
    case SI534X_DTA2131B_LIKE:
        *pConfig = DT_DF_SI534X_CFG_DUAL_CLOCK;
        break;
    case SI534X_DTA2172_LIKE:
        *pConfig = DT_DF_SI534X_CFG_DUAL_SDI_CLOCK;
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_IMPLEMENTED;
    }
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

    // Not supported in free-run-only-mode
    DT_ASSERT(!DtDfSi534X_IsInFreeRunOnlyMode(pDf));

    // Check parameters
    if (pLocked==NULL || pCalibDone==NULL)
        return DT_STATUS_INVALID_PARAMETER;


    DT_RETURN_ON_ERROR(DtDfSi534X_ReadRegister(pDf, &SI5344_INTERNAL_STATUS));
    DT_RETURN_ON_ERROR(DtDfSi534X_ReadRegister(pDf, &SI5344_HOLDOVER_LOL));

    *pCalibDone = ((SI5344_INTERNAL_STATUS.m_Data&(1<<0)) == 0); // Sys in calib == false
    *pLocked = ((SI5344_HOLDOVER_LOL.m_Data&(1<<1)) == 0);     // Loss of lock == false
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
    for (ClockIdx=0; ClockIdx<DtDfSi534X_GetNumClocks(pDf); ClockIdx++)
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
        { "ARCHITECTURE", PROPERTY_VALUE_TYPE_INT, &(pDf->m_ClockArchitecture) },
    };

    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);
 
    // Init parameters to their defaults
    pDf->m_Si534XAddress = -1;
    pDf->m_ClockArchitecture = -1;

    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFSI534X_PARS), DFSI534X_PARS);
    if (!DT_SUCCESS(Status))
        return Status;

    // Check paramaters have been loaded succesfully
    DT_ASSERT(pDf->m_Si534XAddress >= 0);
    DT_ASSERT(pDf->m_ClockArchitecture >= 0);
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

        // Determine device properties
        if (DT_SUCCESS(Status))
           Status = DtDfSi534X_DetermineDeviceType(pDf);

        if (DT_SUCCESS(Status))
        {
            DtDfSi534XConfig  DefaultConfig =  DT_DF_SI534X_CFG_UNDEFINED;
            // Sleep 100ms
            DtSleep(100);

            // Configure default clock(s)
            Status = DtDfSi534X_GetDefaultConfig(pDf, &DefaultConfig);
            if (DT_SUCCESS(Status))
                Status = DtDfSi534X_SetConfigInt(pDf, DefaultConfig);
        }
        if (DT_SUCCESS(Status))
        { 
            // In case the TXPLLs are children of the Si534X driver function,
            // reset/recalibrate the TXPLLs

            // Wait shortly, before resetting/calibrating the PLLs, to allow ALTERA 
            // power-up calibration to complete. Starting a user calibration, while 
            // power-up calibration is busy results in calibration to timeout (fail).
            DtSleep(500);
            // DTA-2100 preparation
            //if (pDf->m_NumClockProps>0 && pDf->m_ClockProps!=NULL)
            //    RefFreq = DIV64(pDf->m_ClockProps[0].m_FrequencyuHz, 1000000);
            //if (DT_SUCCESS(Status) && pDf->m_pDfTxPllMgr!=NULL)
            //    Status = DtDfTxPllMgr_Calibrate(pDf->m_pDfTxPllMgr, RefFreq);
            if (DT_SUCCESS(Status) && pDf->m_pDfTxPllMgr!=NULL)
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
    Status = DtDfSi534X_WriteConfigItems(pDf, SI534X_CONFIG_HARDRST,
                                           DT_SIZEOF_ARRAY(SI534X_CONFIG_HARDRST), FALSE);

    // Ignore result? wait for Andrew's investigation
    
    DtSleep(10);
 
    // Configuration becomes undefined
    pDf->m_PrevBank = -1;
    pDf->m_CurConfig = DT_DF_SI534X_CFG_UNDEFINED;
    pDf->m_pCurConfigItems = NULL;
    pDf->m_CurConfigNumItems = 0;
    pDf->m_NumClockProps = 0;

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

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_StoreCurConfigItems -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSi534X_StoreCurConfigItems(DtDfSi534X* pDf, 
                                   const DtDfSi534XRegister* pConfigItems,  Int  NumItems)
{
    Int i=0;
    // If necessary allocate new configuration storage
    if (pDf->m_pCurConfigItems!=NULL && NumItems!=pDf->m_CurConfigNumItems)
    {
        DtMemFreePool(pDf->m_pCurConfigItems, DF_TAG);
        pDf->m_pCurConfigItems = NULL;
    }
    if (pDf->m_pCurConfigItems == NULL)
    {
        Int AllocSize = sizeof(DtDfSi534XRegister) * NumItems;

        pDf->m_pCurConfigItems = (DtDfSi534XRegister*)DtMemAllocPool(DtPoolNonPaged,
                                                                       AllocSize, DF_TAG);
    }
    if (pDf->m_pCurConfigItems == NULL)
        return DT_STATUS_OUT_OF_MEMORY;

    // Copy the config
    for (i=0; i<NumItems; i++)
        pDf->m_pCurConfigItems[i] = pConfigItems[i];
    pDf->m_CurConfigNumItems = NumItems;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_WriteCurrentConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSi534X_WriteCurrentConfig(DtDfSi534X*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  Timeout;
    Bool Locked=FALSE, CalibDone=FALSE;


    // Write preamble
    Status = DtDfSi534X_WriteConfigItems(pDf, SI534X_CONFIG_PREAMBLE,
                                          DT_SIZEOF_ARRAY(SI534X_CONFIG_PREAMBLE), FALSE);
    if (DT_SUCCESS(Status))
    {
        // Delay 300 msec
        //   Delay is worst case time for device to complete any calibration
        //   that is running due to device state change previous to this script
        //   being processed.
        DtSleep(300);
        // Write registers
        Status = DtDfSi534X_WriteConfigItems(pDf, pDf->m_pCurConfigItems,
                                                         pDf->m_CurConfigNumItems, FALSE);
    }
    if (DT_SUCCESS(Status))
    { 
        // Write postamble
        Status = DtDfSi534X_WriteConfigItems(pDf, SI534X_CONFIG_POSTAMBLE,
                                         DT_SIZEOF_ARRAY(SI534X_CONFIG_POSTAMBLE), FALSE);
    }

    // Don't wait for lock if in free run only  mode
    if (DtDfSi534X_IsInFreeRunOnlyMode(pDf))
        return Status;

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
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_WriteConfigItems -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSi534X_WriteConfigItems(DtDfSi534X* pDf,const DtDfSi534XRegister* 
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

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_GetClockIdx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Returns the clock index of the fractional/non-fractional clock output
//
int DtDfSi534X_GetClockIdx(DtDfSi534X* pDf, Bool Fractional)
{
    switch (pDf->m_ClockArchitecture)
    {
    case SI534X_DTA2127_LIKE:   return 0;
    case SI534X_DTA2131B_LIKE:  return Fractional ? 0 : 1;
    case SI534X_DTA2172_LIKE:   return Fractional ? 0 : 1;
    case SI534X_DTA2175_LIKE:   return 0;
    default: DT_ASSERT(FALSE);  return 0;
    }
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_GetNumClocks -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Returns the number of output clocks
//
int DtDfSi534X_GetNumClocks(DtDfSi534X* pDf)
{
    switch (pDf->m_ClockArchitecture)
    {
    case SI534X_DTA2127_LIKE:   return 1;
    case SI534X_DTA2131B_LIKE:  return 2;
    case SI534X_DTA2172_LIKE:   return 2;
    case SI534X_DTA2175_LIKE:   return 1;
    default: DT_ASSERT(FALSE);  return 0;
    }
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_IsInFreeRunOnlyMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// When configured in free-run-mode. The DSPLL is locked tot the crystal or XO. No 
// inputs (INx) are configured. LOL and HOLD will always be asserted.
//
Bool DtDfSi534X_IsInFreeRunOnlyMode(DtDfSi534X* pDf)
{
    switch (pDf->m_ClockArchitecture)
    {
    case SI534X_DTA2127_LIKE:   return TRUE;
    case SI534X_DTA2131B_LIKE:  return FALSE;
    case SI534X_DTA2172_LIKE:   return FALSE;
    case SI534X_DTA2175_LIKE:   return FALSE;
    default: DT_ASSERT(FALSE);  return FALSE;
    }
}
