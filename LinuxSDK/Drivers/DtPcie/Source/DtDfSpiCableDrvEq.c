// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSpiCableDrvEq.c *#*#*#*#*#*#*#*#* (C) 2019 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2019 DekTec Digital Video B.V.
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
#include "DtDfSpiCableDrvEq.h"

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Types -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
        
#define SPIM_ROLE_NONE      NULL

#define GSXX90_MAX_ADDRESS  0xBF    // Highest GS3590/GS12090 register address

// Cable drivers device ids (bits 15..8 and 3..0)
#define CBL_DRVEQ_UNDEFINED   -1     // Unknown
#define CBL_DRVEQ_GS3590      0x6A1  // Gennum GS3590 3G-SDI/ASI Cable Driver/Equalizer
#define CBL_DRVEQ_GS12090     0x6A0  // Gennum GS12090 12G-SDI/ASI Cable Driver/Equalizer
#define CBL_DRVEQ_GS12190     0xAA0  // Gennum GS12190 12G-SDI/ASI Cable Driver/Equalizer

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfSpiCableDrvEq implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiTx function
#define DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf)      \
                            DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfSpiCableDrvEq))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_SPICABLEDRVEQ_MUST_BE_ENABLED(pDf)  DF_MUST_BE_ENABLED_IMPL(SPICABLEDRVEQ, pDf)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtDfSpiCableDrvEq_Init(DtDf*);
static DtStatus  DtDfSpiCableDrvEq_LoadParameters(DtDf*);
static DtStatus  DtDfSpiCableDrvEq_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfSpiCableDrvEq_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfSpiCableDrvEq_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfSpiCableDrvEq_OpenChildren(DtDfSpiCableDrvEq*);
// Cable driver/equalizer type specific operations

static DtStatus  DtDfSpiCableDrvEq_CheckSdiRate(DtDfSpiCableDrvEq*, Int SdiRate);
static DtStatus  DtDfSpiCableDrvEq_MaximizeLoopBandwidth(DtDfSpiCableDrvEq*, Bool MaxLbw);
static DtStatus  DtDfSpiCableDrvEq_ReadGsXx90DeviceId(DtDfSpiCableDrvEq*, Int* pDeviceId,
                                                                         Int* pRevision);
static DtStatus  DtDfSpiCableDrvEq_SetGsXx90Direction(DtDfSpiCableDrvEq*, Int Direction);
static DtStatus  DtDfSpiCableDrvEq_SetGsXx90SdiRate(DtDfSpiCableDrvEq*, Int SdiRate);
static DtStatus  DtDfSpiCableDrvEq_SetGsXx90PllLoopBandwidth(DtDfSpiCableDrvEq*,
                                                                             Int SdiRate);
static DtStatus  DtDfSpiCableDrvEq_SetGsXx90PreEmphasisControl(DtDfSpiCableDrvEq*,
                                                                             Int SdiRate);
static DtStatus  DtDfSpiCableDrvEq_SetGsXx90AmplitudeControl(DtDfSpiCableDrvEq*,
                                                                             Int SdiRate);
static DtStatus  DtDfSpiCableDrvEq_SetGsXx90TraceEqualizer(DtDfSpiCableDrvEq*,
                                                                             Int TraceEq);
static Bool  DtDfSpiCableDrvEq_CheckGsXx90(DtDfSpiCableDrvEq*, Int StartAddress,
                                                      Int NumToCheck, const UInt16* pBuf);
static DtStatus  DtDfSpiCableDrvEq_ReadGsXx90(DtDfSpiCableDrvEq*, Int StartAddress,
                                                            Int NumToRead, UInt16 * pBuf);
static DtStatus  DtDfSpiCableDrvEq_WriteGsXx90(DtDfSpiCableDrvEq*, Int StartAddress,
                                                     Int NumToWrite, const UInt16 * pBuf);
// =+=+=+=+=+=+=+=+=+=+=+=+ DtDfSpiCableDrvEq - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfSpiCableDrvEq_Close(DtDf*  pDf)
{

    DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfSpiCableDrvEq*  DtDfSpiCableDrvEq_Open(DtCore* pCore, DtPt* pPt, const char* pRole, 
                                                  Int Instance, Int Uuid, Bool CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // There is no stub
    CreateStub = FALSE; // TODOTD driver scope only

    // Init open parameters
    DT_DF_SPICABLEDRVEQ_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfSpiCableDrvEq, Id, DT_FUNC_TYPE_SPICABLEDRVEQ, 
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfSpiCableDrvEq_Close;
    OpenParams.m_InitFunc = DtDfSpiCableDrvEq_Init;
    OpenParams.m_LoadParsFunc = DtDfSpiCableDrvEq_LoadParameters;
    OpenParams.m_OnCloseFileFunc = DtDfSpiCableDrvEq_OnCloseFile;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfSpiCableDrvEq_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfSpiCableDrvEq_OnEnablePreChildren;
    
    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfSpiCableDrvEq*)DtDf_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_GetDirection -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSpiCableDrvEq_GetDirection(DtDfSpiCableDrvEq* pDf, Int* pDirection)
{
    // Sanity check
    DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pDirection == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SPICABLEDRVEQ_MUST_BE_ENABLED(pDf);

    // Return cached operational mode
    *pDirection = pDf->m_Direction;

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_GetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSpiCableDrvEq_GetSdiRate(DtDfSpiCableDrvEq* pDf, Int* pSdiRate)
{
    // Sanity check
    DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf);

    // Check parameter
    if (pSdiRate == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    DF_SPICABLEDRVEQ_MUST_BE_ENABLED(pDf);

    // Return cached SDI-rate
    *pSdiRate = pDf->m_SdiRate;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_SetDirection -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSpiCableDrvEq_SetDirection(DtDfSpiCableDrvEq* pDf, Int Direction)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (Direction!=DT_DF_SPICABLEDRVEQ_DIR_RX && Direction!=DT_DF_SPICABLEDRVEQ_DIR_TX)
    {
        DtDbgOutDf(ERR, SPICABLEDRVEQ, pDf, "Invalid direction");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    DF_SPICABLEDRVEQ_MUST_BE_ENABLED(pDf);

    // No change?
    if (pDf->m_Direction == Direction)
        return DT_STATUS_OK;

    // Protect SPICABLEDRVEQ against concurrent access
    DtFastMutexAcquire(&pDf->m_AccessMutex);

    // Set new direction
    Status = DtDfSpiCableDrvEq_SetGsXx90Direction(pDf, Direction);
    
    // Set defaults
    if (DT_SUCCESS(Status))
    { 
        // Reset SDI-rate and maximize loop bandwidth
        pDf->m_SdiRate = DT_DRV_SDIRATE_UNKNOWN;
        pDf->m_MaxLoopBandwidth = FALSE;
    }
    if (DT_SUCCESS(Status))
        Status = DtDfSpiCableDrvEq_SetGsXx90SdiRate(pDf, pDf->m_SdiRate);
    if (DT_SUCCESS(Status) && pDf->m_SpiDeviceId==CBL_DRVEQ_GS12190)
        Status = DtDfSpiCableDrvEq_MaximizeLoopBandwidth(pDf, pDf->m_MaxLoopBandwidth);
    if (DT_SUCCESS(Status))
        Status = DtDfSpiCableDrvEq_SetGsXx90PllLoopBandwidth(pDf, pDf->m_SdiRate);
    if (DT_SUCCESS(Status))
        Status = DtDfSpiCableDrvEq_SetGsXx90PreEmphasisControl(pDf, pDf->m_SdiRate);
    if (DT_SUCCESS(Status))
        Status = DtDfSpiCableDrvEq_SetGsXx90AmplitudeControl(pDf, pDf->m_SdiRate);

    // Save new Direction
    if (DT_SUCCESS(Status))
        pDf->m_Direction = Direction;
    
    // Release SPICABLEDRVEQ mutex
    DtFastMutexRelease(&pDf->m_AccessMutex);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_SetRxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSpiCableDrvEq_SetRxSdiRate(DtDfSpiCableDrvEq* pDf, Int SdiRate)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool MaxLoopBandwidth = FALSE;

    // Sanity checks
    DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf);

    // Check SDI-rate
    DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_CheckSdiRate(pDf, SdiRate));

    // Direction must be RX
    if (pDf->m_Direction != DT_DF_SPICABLEDRVEQ_DIR_RX)
        return DT_STATUS_NOT_SUPPORTED;

    // Must be enabled
    DF_SPICABLEDRVEQ_MUST_BE_ENABLED(pDf);

    // No change?
    if (pDf->m_SdiRate == SdiRate)
        return DT_STATUS_OK;

    // Protect SPICABLEDRVEQ against concurrent access
    DtFastMutexAcquire(&pDf->m_AccessMutex);

    // SDI-rate stays on automatic detect.
    // Cable equalizer is configured for auto rate detection. This is much faster than the
    // manual rate since it will try to relock after each change.

    // Update maximize loop bandwidth for the GS12190
    if (pDf->m_SpiDeviceId==CBL_DRVEQ_GS12190)
    { 
        MaxLoopBandwidth = (SdiRate==DT_DRV_SDIRATE_HD);
        if (MaxLoopBandwidth != pDf->m_MaxLoopBandwidth)
            Status = DtDfSpiCableDrvEq_MaximizeLoopBandwidth(pDf, MaxLoopBandwidth);
    }

    // Save new SdiRate
    if (DT_SUCCESS(Status))
    { 
        pDf->m_SdiRate = SdiRate;
        pDf->m_MaxLoopBandwidth = MaxLoopBandwidth;
    }

    // Release SPICABLEDRVEQ mutex
    DtFastMutexRelease(&pDf->m_AccessMutex);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_SetTxSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSpiCableDrvEq_SetTxSdiRate(DtDfSpiCableDrvEq* pDf, Int SdiRate)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf);

    // Check SDI-rate
    DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_CheckSdiRate(pDf, SdiRate));
    
    // Direction must be TX
    if (pDf->m_Direction != DT_DF_SPICABLEDRVEQ_DIR_TX)
        return DT_STATUS_NOT_SUPPORTED;

    // Must be enabled
    DF_SPICABLEDRVEQ_MUST_BE_ENABLED(pDf);

    // No change?
    if (pDf->m_SdiRate == SdiRate)
        return DT_STATUS_OK;

    // Protect SPICABLEDRVEQ against concurrent access
    DtFastMutexAcquire(&pDf->m_AccessMutex);

    Status = DtDfSpiCableDrvEq_SetGsXx90SdiRate(pDf, SdiRate);
    if (DT_SUCCESS(Status))
        Status = DtDfSpiCableDrvEq_SetGsXx90PllLoopBandwidth(pDf, SdiRate);
    if (DT_SUCCESS(Status))
        Status = DtDfSpiCableDrvEq_SetGsXx90PreEmphasisControl(pDf, SdiRate);
    if (DT_SUCCESS(Status))
        Status = DtDfSpiCableDrvEq_SetGsXx90AmplitudeControl(pDf, SdiRate);

    // Save new SdiRate
    if (DT_SUCCESS(Status))
        pDf->m_SdiRate = SdiRate;

    // Release SPICABLEDRVEQ mutex
    DtFastMutexRelease(&pDf->m_AccessMutex);

    return Status;
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtDfSpiCableDrvEq - Private functions +=+=+=+=+=+=+=+=+=+=+=+=
//

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSpiCableDrvEq_Init(DtDf* pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfSpiCableDrvEq* pDf = (DtDfSpiCableDrvEq*)pDfBase;
    // Sanity checks
    DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf);

    // Initialize SPI mutex against concurrent access
    DtFastMutexInit(&pDf->m_AccessMutex);

    // Set defaults
    pDf->m_Direction = DT_DF_SPICABLEDRVEQ_DIR_TX;
    pDf->m_SdiRate = DT_DRV_SDIRATE_UNKNOWN;
    pDf->m_MaxLoopBandwidth = FALSE;
    pDf->m_SpiDeviceId = CBL_DRVEQ_UNDEFINED;
    pDf->m_Supports12G = FALSE;

    //Open children
    Status = DtDfSpiCableDrvEq_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SPICABLEDRVEQ, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Note: LoadParameters() is called before the Init(). The loaded parameters can be used
// in the Init().
//
DtStatus  DtDfSpiCableDrvEq_LoadParameters(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfSpiCableDrvEq* pDf = (DtDfSpiCableDrvEq*)pDfBase;

    // List of SpiCableDrvEq function parameters
    DtDfParameters  DFSPICABLEDRVEQ_PARS[] =
    {
        // Name,  Value Type,  Value*
        { "TRACE_EQUALIZER", PROPERTY_VALUE_TYPE_INT, &(pDf->m_TraceEqualizer) },
    };
    // Sanity checks
    DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf);

    // Set defaults
    pDf->m_TraceEqualizer = -1;
    
    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFSPICABLEDRVEQ_PARS), 
                                                                    DFSPICABLEDRVEQ_PARS);
    if (!DT_SUCCESS(Status))
        return Status;

    if (pDf->m_TraceEqualizer<-1 || pDf->m_TraceEqualizer>8)
        return DT_STATUS_CONFIG_ERROR;

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSpiCableDrvEq_OnEnablePostChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfSpiCableDrvEq* pDf = (DtDfSpiCableDrvEq*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;
     
    // Sanity check
    DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        Int Revision;
        // DISABLE -> ENABLE
        DtDbgOutDf(AVG, SPICABLEDRVEQ, pDf, "DISABLE -> ENABLE");
        DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_ReadGsXx90DeviceId(pDf, &pDf->m_SpiDeviceId,
                                                                              &Revision));
        switch (pDf->m_SpiDeviceId)
        {
        case CBL_DRVEQ_GS3590:      pDf->m_Supports12G = FALSE; break;
        case CBL_DRVEQ_GS12090:     pDf->m_Supports12G = TRUE; break;
        case CBL_DRVEQ_GS12190:     pDf->m_Supports12G = TRUE; break;
        case CBL_DRVEQ_UNDEFINED:
        default: DT_ASSERT(FALSE);  return DT_STATUS_NOT_SUPPORTED;
        }
        // Initialize cable driver/equalizer
        DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetGsXx90Direction(pDf, pDf->m_Direction));
        DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetGsXx90SdiRate(pDf, pDf->m_SdiRate));
        // Cable driver setttings
        DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetGsXx90PllLoopBandwidth(pDf,
                                                                         pDf->m_SdiRate));
        if (pDf->m_SpiDeviceId==CBL_DRVEQ_GS12190)
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_MaximizeLoopBandwidth(pDf,
                                                                pDf->m_MaxLoopBandwidth));
        DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetGsXx90PreEmphasisControl(pDf,
                                                                         pDf->m_SdiRate));
         DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetGsXx90AmplitudeControl(pDf, 
                                                                         pDf->m_SdiRate));
         if (pDf->m_TraceEqualizer >= 0)
             DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetGsXx90TraceEqualizer(pDf,
                                                                  pDf->m_TraceEqualizer));
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_OnEnablePreChildren -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSpiCableDrvEq_OnEnablePreChildren(DtDf* pDfBase, Bool Enable)
{
    DtDfSpiCableDrvEq* pDf = (DtDfSpiCableDrvEq*)pDfBase;
    // Sanity check
    DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf);
    if (!Enable)
    {
        // ENABLE -> DISABLE
        DtDbgOutDf(AVG, SPICABLEDRVEQ, pDf, "ENABLE -> DISABLE");
    }

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static DtStatus  DtDfSpiCableDrvEq_OnCloseFile(DtDf* pDfBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfSpiCableDrvEq* pDf = (DtDfSpiCableDrvEq*)pDfBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);
    
    // Sanity checks
    DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf);

    // Check if the owner closed the file handle
    Status = DtDf_ExclAccessCheck((DtDf*)pDf, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutDf(AVG, SPICABLEDRVEQ, pDf, "Go to IDLE");
    }
    // Use base function to release exclusive access
    return DtDf_OnCloseFile((DtDf*)pDf, pFile);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSpiCableDrvEq_OpenChildren(DtDfSpiCableDrvEq*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

    // List of children supported by the the SPICABLEDRVEQ function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_SPIM, DT_BC_SPIM_NAME,
                               SPIM_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcSpiM), TRUE},
    };

    DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
    if (!DT_SUCCESS(Status))
        return Status;

    // BLKCTRL should be present
    DT_ASSERT(pDf->m_pBcSpiM != NULL);

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_CheckSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSpiCableDrvEq_CheckSdiRate(DtDfSpiCableDrvEq* pDf, Int SdiRate)
{
    // Check parameters
    if (pDf->m_Supports12G && 
                         (   SdiRate!=DT_DRV_SDIRATE_UNKNOWN && SdiRate!=DT_DRV_SDIRATE_SD
                          && SdiRate!=DT_DRV_SDIRATE_HD && SdiRate!=DT_DRV_SDIRATE_3G
                          && SdiRate!=DT_DRV_SDIRATE_6G && SdiRate!=DT_DRV_SDIRATE_12G))
    {
        DtDbgOutDf(ERR, SPICABLEDRVEQ, pDf, "Invalid  SDI-rate");
        return DT_STATUS_INVALID_PARAMETER;
    }
    else if (!pDf->m_Supports12G && 
                         (   SdiRate!=DT_DRV_SDIRATE_UNKNOWN && SdiRate!=DT_DRV_SDIRATE_SD
                          && SdiRate!=DT_DRV_SDIRATE_HD && SdiRate!=DT_DRV_SDIRATE_3G))
    {
        DtDbgOutDf(ERR, SPICABLEDRVEQ, pDf, "Invalid SDI-rate");
        return DT_STATUS_INVALID_PARAMETER;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_MaximizeLoopBandwidth -.-.-.-.-.-.-.-.-.-.-.-
//
// The register that we need to set to fix the HD pathological is register 0x09.
// We need to set bit 1 of 0x09 to 1 for the GS12190.
//
DtStatus DtDfSpiCableDrvEq_MaximizeLoopBandwidth(DtDfSpiCableDrvEq* pDf, Bool MaxLbw)
{
    const Int CDR_PARAMS = 0x09;
    UInt16  RegData[1] = {0};
    DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_ReadGsXx90(pDf, CDR_PARAMS, 1, RegData));
    if (MaxLbw)
        RegData[0] |= 0x0002;
    else
        RegData[0] &= ~0x0002;
    DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_WriteGsXx90(pDf, CDR_PARAMS, 1, RegData));
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_ReadGsXx90DeviceId -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSpiCableDrvEq_ReadGsXx90DeviceId(DtDfSpiCableDrvEq* pDf, Int* pDeviceId,
                                                                           Int* pRevision)
{
    const Int DEVICE_ID = 0x01;
    UInt16  RegData[1] = {0};
    DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_ReadGsXx90(pDf, DEVICE_ID, 1, RegData));
    *pDeviceId =  ((RegData[0] >> 4) & 0x0FF0) | (RegData[0] & 0X000F); 
    *pRevision = ((RegData[0] >> 4) & 0x000F); 
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_SetGsXx90Direction -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSpiCableDrvEq_SetGsXx90Direction(DtDfSpiCableDrvEq* pDf, Int Direction)
{
    const Int INPUT_SELECT_CTRL = 0x14;
    UInt16  RegData[1];
    // RSVD:        b15..b11: 0
    // LOOP_BACK:   b10: 0
    // RSVD:        b9..b3: 1100000
    // DIR_SELMODE: b2..b1: 01 (Host Interface Mode)
    // DIR_SEL:     b0: 0 Cable Equalizer/ 1 Cable Driver
    RegData[0] =  (Direction == DT_DF_SPICABLEDRVEQ_DIR_RX) ? 0x0302 : 0x0303;
    DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_WriteGsXx90(pDf, INPUT_SELECT_CTRL, 1, RegData));
    DT_ASSERT(DtDfSpiCableDrvEq_CheckGsXx90(pDf, INPUT_SELECT_CTRL, 1, RegData));
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_SetGsXx90SdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSpiCableDrvEq_SetGsXx90SdiRate(DtDfSpiCableDrvEq* pDf, Int SdiRate)
{
    const Int RATE_DETECT_MODE = 0x06;
    UInt16  RegData[1] = {0};
    // RSVD:        b15..b14: 0
    // ENA_12G      b13: 1
    // ENA_6G       b12: 1
    // ENA_3G       b11: 1
    // ENA_HD       b10: 1
    // ENA_SD       b9: 1
    // ENA_MADI     b8: 0
    // RSVD:        b7..b5: 0
    // MANUAL_RATE  b4..b1: 010(SD), 011 (HD), 100 (3G), 101 (6G), 110 (12G)
    // AUTO_RATE    b0: 0 (Disable), 1 (Enable)
    switch (SdiRate)
    {
        default:
        case DT_DRV_SDIRATE_UNKNOWN: if (pDf->m_Supports12G)
                                        RegData[0] = 0x3E01;
                                     else
                                        RegData[0] = 0x0E01;
                                     break;
        case DT_DRV_SDIRATE_SD:      RegData[0] = 0x0004; break;
        case DT_DRV_SDIRATE_HD:      RegData[0] = 0x0006; break;
        case DT_DRV_SDIRATE_3G:      RegData[0] = 0x0008; break;
        case DT_DRV_SDIRATE_6G:      RegData[0] = 0x000A; break;
        case DT_DRV_SDIRATE_12G:     RegData[0] = 0x000C; break;
    }
    DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_WriteGsXx90(pDf, RATE_DETECT_MODE, 1, RegData));
    DT_ASSERT(DtDfSpiCableDrvEq_CheckGsXx90(pDf, RATE_DETECT_MODE, 1, RegData));
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_SetGsXx90PllLoopBandwidth -.-.-.-.-.-.-.-.-.-.-
//
// Only supported by 12G; Setting is ignored for other devices.
//
DtStatus  DtDfSpiCableDrvEq_SetGsXx90PllLoopBandwidth(DtDfSpiCableDrvEq* pDf, Int SdiRate)
{
    const Int PLL_LOOP_BW = 0x0A;
    UInt16  RegData[1] = {0};
    // RSVD:        b15..b13: 0
    // LBW_12G      b12..b8: 1:0.0625x; 8:0.5x
    // RSVD:         b7..b5: 0
    // LBW_6G        b4..b0: 8
    
    if (!pDf->m_Supports12G)
        return DT_STATUS_OK;

    switch (SdiRate)
    {
        default:
        case DT_DRV_SDIRATE_UNKNOWN: RegData[0] = 0x0808; break;
        case DT_DRV_SDIRATE_SD:      RegData[0] = 0x0808; break;
        case DT_DRV_SDIRATE_HD:      RegData[0] = 0x0808; break;
        case DT_DRV_SDIRATE_3G:      RegData[0] = 0x0808; break;
        case DT_DRV_SDIRATE_6G:      RegData[0] = 0x0808; break;
        case DT_DRV_SDIRATE_12G:     RegData[0] = 0x0108; break;
    }
    DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_WriteGsXx90(pDf, PLL_LOOP_BW, 1, RegData));
    DT_ASSERT(DtDfSpiCableDrvEq_CheckGsXx90(pDf, PLL_LOOP_BW, 1, RegData));
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_SetGsXx90PreEmphasisControl -.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSpiCableDrvEq_SetGsXx90PreEmphasisControl(DtDfSpiCableDrvEq* pDf, 
                                                                              Int SdiRate)
{
    const Int PRE_EMPHAS_CTRL = 0x32;
    UInt16  RegData[1] = {0};
    // RSVD:        b15..b13: 0
    // WIDTH         b12..b8: 7
    // RSVD:              b7: 0
    // PWRDWN:            b6: 0 : enable pre-emphasis;  1 : disable pre-emphasis
    // AMPL:          b5..b0: B
    
    switch (SdiRate)
    {
        default:
        case DT_DRV_SDIRATE_UNKNOWN: RegData[0] = 0x070B; break;    // Enable
        case DT_DRV_SDIRATE_SD:      RegData[0] = 0x070B; break;    // Enable
        case DT_DRV_SDIRATE_HD:      RegData[0] = 0x070B; break;    // Enable
        case DT_DRV_SDIRATE_3G:      RegData[0] = 0x070B; break;    // Enable
        case DT_DRV_SDIRATE_6G:      RegData[0] = 0x074B; break;    // Disable
        case DT_DRV_SDIRATE_12G:     RegData[0] = 0x074B; break;    // Disable
    }
    DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_WriteGsXx90(pDf, PRE_EMPHAS_CTRL, 1, RegData));
    DT_ASSERT(DtDfSpiCableDrvEq_CheckGsXx90(pDf, PRE_EMPHAS_CTRL, 1, RegData));
    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_SetGsXx90AmplitudeControl -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSpiCableDrvEq_SetGsXx90AmplitudeControl(DtDfSpiCableDrvEq* pDf, Int SdiRate)
{
    const Int AMPL_CTRL = 0x33;
    UInt16  RegData[1] = {0};
    // RSVD:    b15..b14: 0
    // SWING    b13..b8: 0x18 : 800mVpp 30mVpp per step
    // RSVD:    b7..b0: 0x60
    switch (SdiRate)
    {
        default:
        case DT_DRV_SDIRATE_UNKNOWN: RegData[0] = 0x1860; break;    // 800mVpp
        case DT_DRV_SDIRATE_SD:      RegData[0] = 0x1860; break;    // 800mVpp
        case DT_DRV_SDIRATE_HD:      RegData[0] = 0x1860; break;    // 800mVpp
        case DT_DRV_SDIRATE_3G:      RegData[0] = 0x1860; break;    // 800mVpp
        case DT_DRV_SDIRATE_6G:      RegData[0] = 0x1760; break;    // 770mVpp
        case DT_DRV_SDIRATE_12G:     RegData[0] = 0x1960; break;    // 830mVpp
    }
    DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_WriteGsXx90(pDf, AMPL_CTRL, 1, RegData));
    DT_ASSERT(DtDfSpiCableDrvEq_CheckGsXx90(pDf, AMPL_CTRL, 1, RegData));
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_SetGsXx90TraceEqualizer -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtDfSpiCableDrvEq_SetGsXx90TraceEqualizer(DtDfSpiCableDrvEq* pDf, Int TraceEq)
{
    const Int TREQ0_INP_BOOST = 0x1E;
    UInt16  RegData[1] = {0};
    // RSVD:            b15..b5: 0
    // TREQ0_BOOST      b4..b1: Trace equalizer boost 0..8
    // TREQ0_CD_BOOST:  b0: Trace equalizer boost for carrier detect : 0= boost level 8

    DT_ASSERT(TraceEq>=0 && TraceEq<=8);
    
    // Set boost level
    RegData[0] = TraceEq<<1;

    DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_WriteGsXx90(pDf, TREQ0_INP_BOOST, 1, RegData));
    DT_ASSERT(DtDfSpiCableDrvEq_CheckGsXx90(pDf, TREQ0_INP_BOOST, 1, RegData));
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_CheckGsXx90 -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool DtDfSpiCableDrvEq_CheckGsXx90(DtDfSpiCableDrvEq* pDf, Int StartAddress,
                                                       Int NumToCheck, const UInt16* pBuf)
{
    UInt16 Data[16];
    Int i;
    if (DtDfSpiCableDrvEq_ReadGsXx90(pDf, StartAddress, NumToCheck, Data) != DT_STATUS_OK)
        return FALSE;
    for (i=0; i<NumToCheck; i++)
        if (Data[i] != pBuf[i])
            return FALSE;
    return TRUE;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_ReadGsXx90 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSpiCableDrvEq_ReadGsXx90(DtDfSpiCableDrvEq* pDf, Int StartAddress, 
                                                              Int NumToRead, UInt16* pBuf)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt16 CmdBuffer[2] = {0, 0};

    // Sanity checks
    DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (StartAddress<0 || (StartAddress + NumToRead)>(GSXX90_MAX_ADDRESS+1) || NumToRead<1 
                                                                            || pBuf==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Create read command
    // Read:      b15: 1
    // BCAST ALL: b14: 0
    // EMEM:      b13: 1
    // AUTO_INCR: b12: 0 if NumToRead==1 else 1
    // UINT_ADDR: b11..b7: 0
    // REG_ADDR:  B6..b0
    CmdBuffer[0] |=  0xA000 | (NumToRead==1 ? 0 : 0x1000) | (UInt16)(StartAddress>>16);
    CmdBuffer[1] = (UInt16)(StartAddress);

    DtDbgOutDf(MAX, SPICABLEDRVEQ, pDf, "Read from Address: %d, NumToRead: %d", 
                                                                 StartAddress, NumToRead);
    // Write read command and read back data
    Status = DtBcSPIM_WriteRead(pDf->m_pBcSpiM, sizeof(CmdBuffer), (UInt8*)CmdBuffer, 
                                                  NumToRead*sizeof(UInt16), (UInt8*)pBuf);
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_WriteGsXx90 -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSpiCableDrvEq_WriteGsXx90(DtDfSpiCableDrvEq* pDf, Int StartAddress, 
                                                       Int NumToWrite, const UInt16* pBuf)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt16 CmdBuffer[2] = {0, 0};

    // Sanity checks
    DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (StartAddress<0 || (StartAddress + NumToWrite)>(GSXX90_MAX_ADDRESS+1) 
                                                            || NumToWrite<1 || pBuf==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // No multiple write allowed starting at 0
    if (StartAddress==0 && NumToWrite>1)
        return DT_STATUS_INVALID_PARAMETER;

    // Create write command
    // Write:     b15: 0
    // BCAST ALL: b14: 0
    // EMEM:      b13: 1
    // AUTO_INCR: b12: 0 if NumToRead==1 else 1
    // UINT_ADDR: b11..b7: 0
    // REG_ADDR:  B6..b0
    CmdBuffer[0] |=  0x2000 | (NumToWrite==1 ? 0 : 0x1000) | (UInt16)(StartAddress>>16);
    CmdBuffer[1] = (UInt16)(StartAddress);

    DtDbgOutDf(MAX, SPICABLEDRVEQ, pDf, "Write to Address: %d, NumToWrite: %d", 
                                                                StartAddress, NumToWrite);


    // Write write command and write data
    Status = DtBcSPIM_WriteWrite(pDf->m_pBcSpiM, sizeof(CmdBuffer), (UInt8*)CmdBuffer, 
                                                 NumToWrite*sizeof(UInt16), (UInt8*)pBuf);

    return Status;
}
