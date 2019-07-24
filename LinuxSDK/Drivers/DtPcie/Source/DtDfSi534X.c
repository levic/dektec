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
#define DVC_TYPE_SI5342       5342
#define DVC_TYPE_SI5344       5344

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
static DtStatus  DtDfSi534X_FindSdiTxPll(DtDfSi534X*, Int PllId, DtBcSDITXPLL**);
static DtStatus  DtDfSi534X_InitSdiTxPllLookupTable(DtDfSi534X*);
static DtStatus  DtDfSi534X_ResetSdiTxPlls(DtDfSi534X*);
static DtStatus  DtDfSi534X_FindConfigData(DtDfSi534X*, DtDfSi534XConfig, 
                                            const DtDfSi534XConfigItem**, Int* pNumItems);
static DtStatus  DtDfSi534X_LoadParameters(DtDf*);
static DtStatus  DtDfSi534X_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfSi534X_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfSi534X_OpenChildren(DtDfSi534X*);
static DtStatus  DtDfSi534X_HardReset(DtDfSi534X*);
static DtStatus  DtDfSi534X_WriteConfig(DtDfSi534X*, const DtDfSi534XConfigItem*, 
                                                         Int  NumItems,  Bool UpdateOnly);
static DtStatus  DtDfSi534X_WriteConfigItem(DtDfSi534X* , const DtDfSi534XConfigItem*);

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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_GetNumClocks -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSi534X_GetNumClocks(DtDfSi534X*  pDf, Int* pNumClocks)
{
  // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);

    DF_SI534X_MUST_BE_ENABLED(pDf);
    if (pNumClocks == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    *pNumClocks = pDf->m_NumClockOutputs;
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
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_SetConfigInt -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Set Config for internal use (without is enabled check)
// 
DtStatus DtDfSi534X_SetConfigInt(DtDfSi534X*  pDf, DtDfSi534XConfig  Config)
{
    DtStatus  Status = DT_STATUS_OK;
    const DtDfSi534XConfigItem*  pConfigItems = NULL;
    Int  NumItems = 0;

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

    // Wait for clock to become stable
    DtSleep(50);

    // Release SI-534X mutex
    DtFastMutexRelease(&pDf->m_AccessMutex);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_IsPllLocked -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSi534X_IsPllLocked(DtDfSi534X* pDf, Int PllId, Bool* pLocked)
{
    DtBcSDITXPLL*  pBcSdiTxPll = NULL;
    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);
    DF_SI534X_MUST_BE_ENABLED(pDf);
    if (pLocked == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    // Find the SDITXPLL
    DT_RETURN_ON_ERROR(DtDfSi534X_FindSdiTxPll(pDf, PllId, &pBcSdiTxPll));
    // Get the locked status
    return DtBcSDITXPLL_IsPllLocked(pBcSdiTxPll, pLocked);
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
    // Create list for the SDITXPLLs
    DT_ASSERT(pDf->m_pBcSdiTxPlls == NULL);
    pDf->m_pBcSdiTxPlls = DtVectorBc_Create(4, 4);
    DT_ASSERT(pDf->m_pBcSdiTxPlls != NULL);
    // Create SDITXPLL look-up table
    DT_ASSERT(pDf->m_pSdiTxPllTable == NULL);
    pDf->m_pSdiTxPllTable = DtVector_Create(4, sizeof(DtDfSi534XSdiTxPll), 4);
    DT_ASSERT(pDf->m_pSdiTxPllTable != NULL);
    // Check paramaters have been loaded succesfully
    DT_ASSERT(pDf->m_Si534XAddress >= 0);
    DT_ASSERT(pDf->m_NumClockOutputs==1 || pDf->m_NumClockOutputs==2);
    DT_ASSERT(pDf->m_DeviceType==DVC_TYPE_SI5342 || pDf->m_DeviceType==DVC_TYPE_SI5344);

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
    // Clean-up SDITXPLLs
    if (pDf->m_pBcSdiTxPlls != NULL)
    {
        DtVectorBc_Cleanup(pDf->m_pBcSdiTxPlls);
        pDf->m_pBcSdiTxPlls = NULL;
    }
    // Clean-up SDITXPLL look-up table
    if (pDf->m_pSdiTxPllTable != NULL)
    {
        DtVector_Cleanup(pDf->m_pSdiTxPllTable);
        pDf->m_pSdiTxPllTable = NULL;
    }
    return DT_STATUS_OK;
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_FindSdiTxPll -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSi534X_FindSdiTxPll(DtDfSi534X* pDf, Int PllId, DtBcSDITXPLL** pSdiTxPll)
{
    Int i;
    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);
    // Parameter check
    if (pSdiTxPll == NULL)
        return DT_STATUS_INVALID_PARAMETER;
    // Find the SDITXPLL
    *pSdiTxPll = NULL;
    if (pDf->m_pSdiTxPllTable==NULL || PllId==-1)
        return DT_STATUS_NOT_FOUND;
    for (i=0; i<DtVector_Size(pDf->m_pSdiTxPllTable); i++)
    {
        DtDfSi534XSdiTxPll* pItem;
        pItem = (DtDfSi534XSdiTxPll*)DtVector_At(pDf->m_pSdiTxPllTable, i);
        if (pItem->m_PllId == PllId)
        {
            // Found
            *pSdiTxPll = pItem->m_pSdiTxPll;
            return DT_STATUS_OK;
        }
    }
    return DT_STATUS_NOT_FOUND;
}
// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_InitSdiTxPllLookupTable -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSi534X_InitSdiTxPllLookupTable(DtDfSi534X* pDf)
{
    Int i;
    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_pBcSdiTxPlls!=NULL && pDf->m_pSdiTxPllTable!=NULL);
    // Resize look-up table
    DT_RETURN_ON_ERROR(DtVector_Resize(pDf->m_pSdiTxPllTable,
                                                   DtVectorBc_Size(pDf->m_pBcSdiTxPlls)));
    // Fill the look-up table
    for (i=0; i<DtVectorBc_Size(pDf->m_pBcSdiTxPlls); i++)
    {
        DtDfSi534XSdiTxPll  Item;
        Item.m_pSdiTxPll = (DtBcSDITXPLL*)DtVectorBc_At(pDf->m_pBcSdiTxPlls, i);
        if (Item.m_pSdiTxPll != NULL)
            DT_RETURN_ON_ERROR(DtBcSDITXPLL_GetPllId(Item.m_pSdiTxPll, &Item.m_PllId));
        else
            Item.m_PllId= -1;
        DT_RETURN_ON_ERROR(DtVector_Set(pDf->m_pSdiTxPllTable, i, &Item));
    }
    return DT_STATUS_OK;
}
// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_ResetSdiTxPlls -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSi534X_ResetSdiTxPlls(DtDfSi534X* pDf)
{
    Int i;
    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);
    DT_ASSERT(pDf->m_pBcSdiTxPlls!=NULL && pDf->m_pSdiTxPllTable!=NULL);
    // Issue reset to all SDITXPLL
    for (i=0; i<DtVectorBc_Size(pDf->m_pBcSdiTxPlls); i++)
    {
        DtBcSDITXPLL* pSdiTxPll = (DtBcSDITXPLL*)DtVectorBc_At(pDf->m_pBcSdiTxPlls, i);
        if (pSdiTxPll != NULL)
            DT_RETURN_ON_ERROR(DtBcSDITXPLL_ResetClock(pSdiTxPll));
    }
    // TODOTD
    // Sleep
    // Check result
    // Redo if necessary
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_FindConfigData -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSi534X_FindConfigData(DtDfSi534X*  pDf, DtDfSi534XConfig  Config,
                              const DtDfSi534XConfigItem**  pConfigItems, Int*  pNumItems)

{
    DT_ASSERT(pConfigItems!=NULL && pNumItems!=NULL);

    // Set defaults
    *pConfigItems = NULL;
    *pNumItems =  0;

    // SI-534X configurations
    switch (Config)
    {
    case DT_DF_SI534X_CFG_FREE_RUN_DUAL_CLOCK:
        if (pDf->m_NumClockOutputs != 2)
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
        { "NUM_CLOCKS", PROPERTY_VALUE_TYPE_INT, &(pDf->m_NumClockOutputs) },
        { "DEVICE_TYPE", PROPERTY_VALUE_TYPE_INT, &(pDf->m_DeviceType) },
    };

    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);
 
    // Init parameters to their defaults
    pDf->m_Si534XAddress = -1;
    pDf->m_NumClockOutputs = 0;
    pDf->m_DeviceType = DVC_TYPE_SI5342;

    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFSI534X_PARS), DFSI534X_PARS);
    if (!DT_SUCCESS(Status))
        return Status;
 
    // Check paramaters have been loaded succesfully
    DT_ASSERT(pDf->m_Si534XAddress >= 0);
    DT_ASSERT(pDf->m_NumClockOutputs==1 || pDf->m_NumClockOutputs==2);
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
        // Init SDITXPLL look-up table
         Status = DtDfSi534X_InitSdiTxPllLookupTable(pDf);
        // Perform hard reset
        if (DT_SUCCESS(Status))
        Status = DtDfSi534X_HardReset(pDf);
        if (DT_SUCCESS(Status))
        {
            // Sleep 100ms
            DtSleep(100);

            // Configure free-running
            if (pDf->m_NumClockOutputs == 1)
                Status = DtDfSi534X_SetConfigInt(pDf, 
                                                 DT_DF_SI534X_CFG_FREE_RUN_NONFRAC_CLOCK);
            else
                Status = DtDfSi534X_SetConfigInt(pDf, 
                                                    DT_DF_SI534X_CFG_FREE_RUN_DUAL_CLOCK);
        }
        if (DT_SUCCESS(Status))
            Status = DtDfSi534X_ResetSdiTxPlls(pDf);
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
                               I2CM_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcI2Cm), TRUE}
    };

    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
     if (!DT_SUCCESS(Status))
        return Status;

    // Get the temperature sensors
    Status = DtDf_OpenChildrenOfBcType((DtDf*)pDf, DT_BLOCK_TYPE_SDITXPLL, 
                                                                     pDf->m_pBcSdiTxPlls);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SI534X, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }
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

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_WriteConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSi534X_WriteConfig(DtDfSi534X* pDf,const DtDfSi534XConfigItem* 
                                             pConfigItems, Int NumItems, Bool UpdateOnly)
{
    Int i;
    for (i=0; i<NumItems; i++)
    { 
        // Do we need to update the item?
        if (!UpdateOnly || pDf->m_pCurConfigItems==NULL || pDf->m_CurConfigNumItems<i
            || pDf->m_pCurConfigItems[i].m_BankData0!=pConfigItems[i].m_BankData0
            || pDf->m_pCurConfigItems[i].m_Data1!=pConfigItems[i].m_Data1)
        { 
            DT_RETURN_ON_ERROR(DtDfSi534X_WriteConfigItem(pDf, &pConfigItems[i]));
        }
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_WriteConfigItem -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSi534X_WriteConfigItem(DtDfSi534X* pDf, const DtDfSi534XConfigItem* 
                                                                              pConfigItem)
{
    UInt8 Buffer[2];
    UInt8 Bank = (pConfigItem->m_BankData0 >> 8);

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
    Buffer[0] = (UInt8)pConfigItem->m_BankData0;
    Buffer[1] = pConfigItem->m_Data1;
    return DtBcI2CM_Write(pDf->m_pBcI2Cm, pDf->m_Si534XAddress, sizeof(Buffer), Buffer);
}
