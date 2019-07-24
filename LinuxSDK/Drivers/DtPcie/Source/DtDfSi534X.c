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


// Helper macro to cast a DtDf* to a DtDfSi534X*
#define DF_SI534X      ((DtDfSi534X*)pDf)


static DtStatus  DtDfSi534X_Init(DtDf*);
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

    if (pNumClocks == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    *pNumClocks = pDf->m_NumClockOutputs;
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_SetConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSi534X_SetConfig(DtDfSi534X*  pDf, DtDfSi534XConfig  Config)
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

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSi534X - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSi534X_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSi534X_Init(DtDf*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);

    // Initialize SI-534X mutex against concurrent access
    DtFastMutexInit(&DF_SI534X->m_AccessMutex);

    // Set defaults
    DF_SI534X->m_CurConfig = DT_DF_SI534X_CFG_UNDEFINED;
    DF_SI534X->m_pCurConfigItems = NULL;
    DF_SI534X->m_CurConfigNumItems = 0;

    // Check paramaters have been loaded succesfully
    DT_ASSERT(DF_SI534X->m_Si534XAddress >= 0);
    DT_ASSERT(DF_SI534X->m_NumClockOutputs==1 || DF_SI534X->m_NumClockOutputs==2);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfSi534X_OpenChildren(DF_SI534X);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SI534X, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }
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
        *pConfigItems = SI534X_CONFIG_FREE_RUN_DUAL_CLOCK;
        *pNumItems = DT_SIZEOF_ARRAY(SI534X_CONFIG_FREE_RUN_DUAL_CLOCK);
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
    };

    // Sanity checks
    DF_SI534X_DEFAULT_PRECONDITIONS(pDf);

 
    // Init parameters to their defaults
    pDf->m_Si534XAddress = -1;
    pDf->m_NumClockOutputs = 0;

    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFSI534X_PARS), DFSI534X_PARS);
    if (!DT_SUCCESS(Status))
        return Status;
 
    // Check paramaters have been loaded succesfully
    DT_ASSERT(pDf->m_Si534XAddress >= 0);
    DT_ASSERT(pDf->m_NumClockOutputs==1 || pDf->m_NumClockOutputs==2);
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
        Status = DtDfSi534X_HardReset(pDf);
        if (DT_SUCCESS(Status))
        {
            // Sleep 100ms
            DtSleep(100);

            // Configure free-running
            if (pDf->m_NumClockOutputs == 1)
                Status = DtDfSi534X_SetConfig(pDf, 
                                                 DT_DF_SI534X_CFG_FREE_RUN_NONFRAC_CLOCK);
            else
                Status = DtDfSi534X_SetConfig(pDf, DT_DF_SI534X_CFG_FREE_RUN_DUAL_CLOCK);
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
                               I2CM_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcI2Cm), TRUE}
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
