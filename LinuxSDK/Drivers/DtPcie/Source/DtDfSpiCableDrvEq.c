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

static DtStatus  DtDfSpiCableDrvEq_OnCloseFile(DtDf*, const DtFileObject*);
static DtStatus  DtDfSpiCableDrvEq_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfSpiCableDrvEq_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfSpiCableDrvEq_OpenChildren(DtDfSpiCableDrvEq*);
// Cable driver/equalizer type specific operations
static DtStatus  DtDfSpiCableDrvEq_SetGsXx90Direction(DtDfSpiCableDrvEq*, Int Direction);
static DtStatus  DtDfSpiCableDrvEq_SetGsXx90SdiRate(DtDfSpiCableDrvEq*, Int SdiRate);
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

    // Set new configuration
    switch (pDf->m_SpiDeviceId)
    {
    case DT_SPIM_SPIDVC_GS3590:
    case DT_SPIM_SPIDVC_GS12090:
        Status = DtDfSpiCableDrvEq_SetGsXx90Direction(pDf, Direction);
        break;
    case DT_SPIM_SPIDVC_UNDEFINED:
    default:
        DT_ASSERT(FALSE);
        Status = DT_STATUS_NOT_SUPPORTED;
        break;
    }
    // Save new Direction
    if (DT_SUCCESS(Status))
        pDf->m_Direction = Direction;

    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_SetSdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfSpiCableDrvEq_SetSdiRate(DtDfSpiCableDrvEq* pDf, Int SdiRate)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_SPICABLEDRVEQ_DEFAULT_PRECONDITIONS(pDf);

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

    // Must be enabled
    DF_SPICABLEDRVEQ_MUST_BE_ENABLED(pDf);

    // No change?
    if (pDf->m_SdiRate == SdiRate)
        return DT_STATUS_OK;

    // Set new configuration
    switch (pDf->m_SpiDeviceId)
    {
    case DT_SPIM_SPIDVC_GS3590:
    case DT_SPIM_SPIDVC_GS12090:
        Status = DtDfSpiCableDrvEq_SetGsXx90SdiRate(pDf, SdiRate);
        break;
    case DT_SPIM_SPIDVC_UNDEFINED:
    default:
        DT_ASSERT(FALSE);
        Status = DT_STATUS_NOT_SUPPORTED;
        break;
    }
    // Save new SdiRate
    if (DT_SUCCESS(Status))
        pDf->m_SdiRate = SdiRate;

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
    pDf->m_SpiDeviceId = DT_SPIM_SPIDVC_UNDEFINED;
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

        Int DuplexMode, MaxTfTime, SpiClockRate;
        // DISABLE -> ENABLE
        DtDbgOutDf(AVG, SPICABLEDRVEQ, pDf, "DISABLE -> ENABLE");
        DT_RETURN_ON_ERROR(DtBcSPIM_GetProperties(pDf->m_pBcSpiM, &pDf->m_SpiDeviceId,
                                                 &DuplexMode, &MaxTfTime, &SpiClockRate));

        switch (pDf->m_SpiDeviceId)
        {
        case DT_SPIM_SPIDVC_GS3590:
            pDf->m_Supports12G = FALSE;
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetGsXx90Direction(pDf, 
                                                                       pDf->m_Direction));
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetGsXx90SdiRate(pDf, pDf->m_SdiRate));
            break;
        case DT_SPIM_SPIDVC_GS12090:
            pDf->m_Supports12G = TRUE;
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetGsXx90Direction(pDf, 
                                                                       pDf->m_Direction));
            DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_SetGsXx90SdiRate(pDf, pDf->m_SdiRate));
            break;
        case DT_SPIM_SPIDVC_UNDEFINED:
        default:
            DT_ASSERT(FALSE);
            Status = DT_STATUS_NOT_SUPPORTED;
            break;
        }
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

// .-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_SetGsXx90Direction -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfSpiCableDrvEq_SetGsXx90Direction(DtDfSpiCableDrvEq* pDf, Int Direction)
{
    const Int INPUT_SELECT_CTRL = 0x14;
    UInt16  RegData[1];
    UInt16  RegDataRb[1];
    // RSVD:        b15..b11: 0
    // LOOP_BACK:   b10: 0
    // RSVD:        b9..b3: 1100000
    // DIR_SELMODE: b2..b1: 01 (Host Interface Mode)
    // DIR_SEL:     b0: 0 Cable Equalizer/ 1 Cable Driver
    RegData[0] =  (Direction == DT_DF_SPICABLEDRVEQ_DIR_RX) ? 0x0302 : 0x0303;
    DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_WriteGsXx90(pDf, INPUT_SELECT_CTRL, 1, RegData));
    DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_ReadGsXx90(pDf, INPUT_SELECT_CTRL, 1,RegDataRb));
    DT_ASSERT((RegData[0] == RegDataRb[0]));
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq_SetGsXx90SdiRate -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfSpiCableDrvEq_SetGsXx90SdiRate(DtDfSpiCableDrvEq* pDf, Int SdiRate)
{
    const Int RATE_DETECT_MODE = 0x06;
    UInt16  RegData[1] = {0};
    UInt16  RegDataRb[1] = {0};
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
    DT_RETURN_ON_ERROR(DtDfSpiCableDrvEq_ReadGsXx90(pDf, RATE_DETECT_MODE, 1, RegDataRb));
    DT_ASSERT((RegData[0] == RegDataRb[0]));
    return DT_STATUS_OK;
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
    // Protect SPICABLEDRVEQ against concurrent access
    DtFastMutexAcquire(&pDf->m_AccessMutex);
    
    // Write read command and read back data
    Status = DtBcSPIM_WriteRead(pDf->m_pBcSpiM, sizeof(CmdBuffer), (UInt8*)CmdBuffer, 
                                                  NumToRead*sizeof(UInt16), (UInt8*)pBuf);
    // Release SPICABLEDRVEQ mutex
    DtFastMutexRelease(&pDf->m_AccessMutex);
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
    // Protect SPICABLEDRVEQ against concurrent access
    DtFastMutexAcquire(&pDf->m_AccessMutex);

    // Write write command and write data
    Status = DtBcSPIM_WriteWrite(pDf->m_pBcSpiM, sizeof(CmdBuffer), (UInt8*)CmdBuffer, 
                                                 NumToWrite*sizeof(UInt16), (UInt8*)pBuf);
    // Release SPICABLEDRVEQ mutex
    DtFastMutexRelease(&pDf->m_AccessMutex);

    return Status;
}
