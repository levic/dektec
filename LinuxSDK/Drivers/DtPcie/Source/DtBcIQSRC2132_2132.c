//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIQSRC2132_2132.c *#*#*#*#*#*#*#*#* (C) 2018 DekTec
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
#include "DtBcIQSRC2132_2132.h"
#include "DtBcIQSRC2132_2132_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQSRC2132_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IQSRC2132_2132_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQSRC2132_2132))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IQSRC2132_2132_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IQSRC2132_2132,\
                                                                                      pBc)

// Helper macro to cast a DtBc* to a DtBcIQSRC2132_2132*
#define BC_IQSRC2132_2132         ((DtBcIQSRC2132_2132*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIQSRC2132_2132_Init(DtBc*);

//=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQSRC2132_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSRC2132_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtBcIQSRC2132_2132_Close(DtBc*  pBc)
{
    BC_IQSRC2132_2132_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSRC2132_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtBcIQSRC2132_2132*  DtBcIQSRC2132_2132_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_IQSRC2132_2132_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIQSRC2132_2132, Id,
                           DT_BLOCK_TYPE_IQSRC2132_2132, Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIQSRC2132_2132_Close;
    OpenParams.m_InitFunc = DtBcIQSRC2132_2132_Init;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIQSRC2132_2132*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.- DtBcIQSRC2132_2132_GetSampleRateConversion -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQSRC2132_2132_GetSampleRateConversion(DtBcIQSRC2132_2132* pBc, 
                                               Int* pEnabled, UInt32* pSampleRateFraction)
{
    // Sanity check
    BC_IQSRC2132_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pEnabled==NULL || pSampleRateFraction==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Get the cached sample rate convertor settings
    *pEnabled = (Int)pBc->m_SrcEnabled;
    *pSampleRateFraction = pBc->m_SrcFraction;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.- DtBcIQSRC2132_2132_SetSampleRateConversion -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcIQSRC2132_2132_SetSampleRateConversion(DtBcIQSRC2132_2132* pBc, Int Enable, 
                                                                UInt32 SampleRateFraction)
{
    UInt32  RegControl;
    // Sanity check
    BC_IQSRC2132_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (Enable!=FALSE && Enable!=TRUE)
        return DT_STATUS_INVALID_PARAMETER;
    if (Enable==TRUE && SampleRateFraction!=DT_IQSRC2132_2132_FRAC_0_5 
                     && SampleRateFraction!=DT_IQSRC2132_2132_FRAC_0_25 
                     && SampleRateFraction!=DT_IQSRC2132_2132_FRAC_0_125
                     && SampleRateFraction!=DT_IQSRC2132_2132_FRAC_0_0625
                     && SampleRateFraction!=DT_IQSRC2132_2132_FRAC_0_03125)
        return DT_STATUS_INVALID_PARAMETER;

    // Update control register
    RegControl = IQSRC2132_Control_READ(pBc);
    RegControl = IQSRC2132_Control_SET_Enable(RegControl, Enable==TRUE ? 1: 0);
    IQSRC2132_Control_WRITE(pBc, RegControl);

    if (Enable == TRUE)
    { 
        UInt32 RegSrFrac = 0;
        switch (SampleRateFraction)
        {
        case DT_IQSRC2132_2132_FRAC_0_5:     RegSrFrac = 0X80000000; break;
        case DT_IQSRC2132_2132_FRAC_0_25:    RegSrFrac = 0X40000000; break;
        case DT_IQSRC2132_2132_FRAC_0_125:   RegSrFrac = 0X20000000; break;
        case DT_IQSRC2132_2132_FRAC_0_0625:  RegSrFrac = 0X10000000; break;
        case DT_IQSRC2132_2132_FRAC_0_03125: RegSrFrac = 0X08000000; break;
        }
        IQSRC2132_SampleRateFraction_WRITE(pBc, RegSrFrac);
    }

    pBc->m_SrcEnabled = (Bool)Enable;
    pBc->m_SrcFraction = SampleRateFraction;

    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIQSRC2132_2132 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIQSRC2132_2132_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIQSRC2132_2132_Init(DtBc*  pBcBase)
{
    DtBcIQSRC2132_2132* pBc = (DtBcIQSRC2132_2132*)pBcBase;
    UInt32 RegControl;
    DtStatus  Status=DT_STATUS_OK;

    // Sanity checks
    BC_IQSRC2132_2132_DEFAULT_PRECONDITIONS(pBc);

    // Initialize the sample rate convertor
    BC_IQSRC2132_2132->m_SrcEnabled = FALSE;
    BC_IQSRC2132_2132->m_SrcFraction = DT_IQSRC2132_2132_FRAC_0_5;
    RegControl = IQSRC2132_Control_READ(pBc);
    RegControl = IQSRC2132_Control_SET_Enable(RegControl, 0);
    IQSRC2132_Control_WRITE(pBc, RegControl);
    IQSRC2132_SampleRateFraction_WRITE(BC_IQSRC2132_2132, 0X80000000);

    return Status;
}



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQSRC2132_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcIQSRC2132_2132
#define IQSRC2132_2132_STUB   ((DtIoStubBcIQSRC2132_2132*)pStub)
#define IQSRC2132_2132_BC  ((DtBcIQSRC2132_2132*)IQSRC2132_2132_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIQSRC2132_2132_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);

static DtStatus DtIoStubBcIQSRC2132_2132_OnCmdGetSampleRateConversion(
                                  const DtIoStubBcIQSRC2132_2132*, 
                                  DtIoctlIqSrc2132Cmd_2132GetSampleRateConversionOutput*);
static DtStatus DtIoStubBcIQSRC2132_2132_OnCmdSetSampleRateConversion(
                             const DtIoStubBcIQSRC2132_2132*,
                             const DtIoctlIqSrc2132Cmd_2132SetSampleRateConversionInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IQSRC2132_2132;


static const DtIoctlProperties  IOSTUB_BC_IQSRC2132_2132_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IQSRC2132_CMD_2132(
        DtIoStubBcIQSRC2132_2132_OnCmd,
        NULL, NULL),
};

//+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQSRC2132_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSRC2132_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStubBcIQSRC2132_2132_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQSRC2132_2132));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSRC2132_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubBcIQSRC2132_2132*  DtIoStubBcIQSRC2132_2132_Open(DtBc*  pBc)
{
    DtIoStubBcIQSRC2132_2132*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIQSRC2132_2132));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIQSRC2132_2132, pBc, NULL, 
                                                         DtIoStubBcIQSRC2132_2132_Close,
                                                         NULL,  // Use default IOCTL
                                                         IOSTUB_BC_IQSRC2132_2132_IOCTLS);
    pStub = (DtIoStubBcIQSRC2132_2132*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIQSRC2132_2132- Private functions +=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIQSRC2132_2132_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIQSRC2132_2132_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIqSrc2132CmdInput_2132*  pInData = NULL;
    DtIoctlIqSrc2132CmdOutput_2132*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQSRC2132_2132));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IQSRC2132_CMD_2132);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IQSRC2132_2132, pStub,
                                                      "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_IqSrc2132Cmd_2132;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_IqSrc2132Cmd_2132;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_IQSRC2132_CMD_2132_GET_SAMPLE_RATE_CONVERSION:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIQSRC2132_2132_OnCmdGetSampleRateConversion(
                                                IQSRC2132_2132_STUB, &pOutData->m_GetSrc);
        break;
    case DT_IQSRC2132_CMD_2132_SET_SAMPLE_RATE_CONVERSION:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIQSRC2132_2132_OnCmdSetSampleRateConversion(
                                                 IQSRC2132_2132_STUB, &pInData->m_SetSrc);
        break;

    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.- DtIoStubBcIQSRC2132_2132_OnCmdGetSampleRateConversion -.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQSRC2132_2132_OnCmdGetSampleRateConversion(
    const DtIoStubBcIQSRC2132_2132* pStub,
    DtIoctlIqSrc2132Cmd_2132GetSampleRateConversionOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQSRC2132_2132));
    DT_ASSERT(pOutData != NULL);

    return DtBcIQSRC2132_2132_GetSampleRateConversion(IQSRC2132_2132_BC,
                                       &pOutData->m_SrcEnabled, &pOutData->m_SrcFraction);
}

//.-.-.-.-.-.-.-.- DtIoStubBcIQSRC2132_2132_OnCmdSetSampleRateConversion -.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIQSRC2132_2132_OnCmdSetSampleRateConversion(
    const DtIoStubBcIQSRC2132_2132* pStub,
    const DtIoctlIqSrc2132Cmd_2132SetSampleRateConversionInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIQSRC2132_2132));
    DT_ASSERT(pInData != NULL);

    return DtBcIQSRC2132_2132_SetSampleRateConversion(IQSRC2132_2132_BC, 
                                            pInData->m_SrcEnable, pInData->m_SrcFraction);
}