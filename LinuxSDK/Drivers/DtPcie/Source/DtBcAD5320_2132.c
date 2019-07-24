//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcAD5320_2132.c *#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
#include "DtBcAD5320_2132.h"
#include "DtBcAD5320_2132_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcAD5320_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_AD5320_2132_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcAD5320_2132))

// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_AD5320_2132_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(AD5320_2132, pBc)

// Helper macro to cast a DtBc* to a DtBcAD5320_2132*
#define BC_AD5320_2132         ((DtBcAD5320_2132*)pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcAD5320_2132_Init(DtBc*);
static DtStatus  DtBcAD5320_2132_OnEnable(DtBc*, Bool);
static DtStatus  DtBcAD5320_2132_OnCloseFile(DtBc*, const DtFileObject*);

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcAD5320_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD5320_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcAD5320_2132_Close(DtBc*  pBc)
{
    BC_AD5320_2132_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD5320_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcAD5320_2132*  DtBcAD5320_2132_Open(Int  Address, DtCore* pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_AD5320_2132_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcAD5320_2132, Id, DT_BLOCK_TYPE_AD5320_2132, 
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcAD5320_2132_Close;
    OpenParams.m_InitFunc = DtBcAD5320_2132_Init;
    OpenParams.m_OnEnableFunc = DtBcAD5320_2132_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcAD5320_2132_OnCloseFile;

    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcAD5320_2132*)DtBc_Open(&OpenParams);
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD5320_2132_GetGainControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcAD5320_2132_GetGainControl(DtBcAD5320_2132* pBc, Int* pGainControl, 
                                                                         Int* pManualGain)
{
    // Sanity check
    BC_AD5320_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pGainControl==NULL || pManualGain==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_AD5320_2132_MUST_BE_ENABLED(pBc);

    // return cached values
    *pGainControl =  pBc->m_GainControl;
    *pManualGain = pBc->m_ManualGain;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD5320_2132_GetGainCtrlPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcAD5320_2132_GetGainCtrlPars(DtBcAD5320_2132* pBc, Int* pStepSizeUnlocked,
                                            Int* pStepSizeLocked, Int* pOverflowThreshold,
                                            Int* pWorkingThreshold, UInt* pTimeWindow)
{
    // Sanity check
    BC_AD5320_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pStepSizeUnlocked==NULL || pStepSizeLocked==NULL || pOverflowThreshold==NULL 
                                          || pWorkingThreshold==NULL || pTimeWindow==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_AD5320_2132_MUST_BE_ENABLED(pBc);

    // return cached values
    *pStepSizeUnlocked =  pBc->m_StepSizeUnlocked;
    *pStepSizeLocked = pBc->m_StepSizeLocked;
    *pOverflowThreshold =  pBc->m_OverflowThreshold;
    *pWorkingThreshold =  pBc->m_WorkingThreshold;
    *pTimeWindow = pBc->m_TimeWindow;

    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD5320_2132_GetGainStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcAD5320_2132_GetGainStatus(DtBcAD5320_2132* pBc, Int* pStatus, Int* pGain)
{
    UInt32 RegData;

    // Sanity check
    BC_AD5320_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pStatus==NULL || pGain==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_AD5320_2132_MUST_BE_ENABLED(pBc);

    // Must be in run mode
    if (pBc->m_OperationalMode != DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, AD5320_2132, pBc, "Block not in run mode");
        return DT_STATUS_INVALID_IN_OPMODE;
    }

    // Read auto gain status
    RegData = AD5320_ControlStatus_READ_AutoGainStatus(pBc);
    switch (RegData)
    {
    case AD5320_GAIN_NOLOCK: *pStatus = DT_AD5320_2132_GAIN_NOLOCK; break;
    case AD5320_GAIN_LOCKED: *pStatus = DT_AD5320_2132_GAIN_LOCKED; break;
    default: DT_ASSERT(FALSE); return DT_STATUS_FAIL;
    }

    // Read Gain readback
    *pGain = (Int)AD5320_GainReadback_READ(pBc);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD5320_2132_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcAD5320_2132_GetOperationalMode(DtBcAD5320_2132* pBc, Int* pOpMode)
{
    // Sanity check
    BC_AD5320_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_AD5320_2132_MUST_BE_ENABLED(pBc);

    // Return last cached operational mode
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD5320_2132_SetGainControl -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcAD5320_2132_SetGainControl(DtBcAD5320_2132 * pBc, Int GainControl, 
                                                                           Int ManualGain)
{
    UInt32 RegData;

    // Sanity check
    BC_AD5320_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (GainControl!=DT_AD5320_2132_GAIN_AUTO && GainControl==DT_AD5320_2132_GAIN_MANUAL)
        return DT_STATUS_INVALID_PARAMETER;
    if (ManualGain<0 || ManualGain>=4096)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_AD5320_2132_MUST_BE_ENABLED(pBc);

    // Update gain control
    RegData = AD5320_ControlStatus_READ(pBc);
    RegData = AD5320_ControlStatus_SET_GainControl(RegData, 
           GainControl==DT_AD5320_2132_GAIN_AUTO ? AD5320_GAIN_AUTO : AD5320_GAIN_MANUAL);
    AD5320_ControlStatus_WRITE(pBc, RegData);
    pBc->m_GainControl = GainControl;

    // Update manual gain
    AD5320_ManualGain_WRITE(pBc, (UInt32)ManualGain);
    pBc->m_ManualGain = ManualGain;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD5320_2132_SetGainCtrlPars -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcAD5320_2132_SetGainCtrlPars(DtBcAD5320_2132* pBc, Int StepSizeUnlocked, 
                                                Int StepSizeLocked, Int OverflowThreshold, 
                                                Int WorkingThreshold, UInt TimeWindow)
{
    UInt32 RegData;

    // Sanity check
    BC_AD5320_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (StepSizeUnlocked<0 || StepSizeUnlocked>=256)
        return DT_STATUS_INVALID_PARAMETER;
    if (StepSizeLocked<0 || StepSizeLocked>=256)
        return DT_STATUS_INVALID_PARAMETER;
    if (OverflowThreshold<-0x2000 || OverflowThreshold>0X1FFFF)
        return DT_STATUS_INVALID_PARAMETER;
    if (WorkingThreshold<-0x2000 || WorkingThreshold>0X1FFFF)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_AD5320_2132_MUST_BE_ENABLED(pBc);

    // Update stepsizes
    RegData = AD5320_StepSizes_READ(pBc);
    RegData = AD5320_StepSizes_SET_StepSizeUnlocked(RegData, StepSizeUnlocked);
    RegData = AD5320_StepSizes_SET_StepSizeLocked(RegData, StepSizeLocked);
    AD5320_StepSizes_WRITE(pBc, RegData);
    pBc->m_StepSizeUnlocked = StepSizeUnlocked;
    pBc->m_StepSizeLocked = StepSizeLocked;

    // Update overflow threshold
    AD5320_OverflowThreshold_WRITE(pBc, OverflowThreshold&0x3FFFF);
    pBc->m_OverflowThreshold = OverflowThreshold;

    // Update working threshold
    AD5320_WorkingThreshold_WRITE(pBc, WorkingThreshold&0x3FFFF);
    pBc->m_WorkingThreshold = WorkingThreshold;

    // Update time window
    AD5320_TimeWindow_WRITE(pBc, TimeWindow);
    pBc->m_TimeWindow = TimeWindow;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD5320_2132_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcAD5320_2132_SetOperationalMode(DtBcAD5320_2132* pBc, Int OpMode)
{
    // Sanity check
    BC_AD5320_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
    { 
        DtDbgOutBc(ERR, AD5320_2132, pBc, "Invalid operational mode");
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Must be enabled
    BC_AD5320_2132_MUST_BE_ENABLED(pBc);

    // Nothing to do yet
    pBc->m_OperationalMode = OpMode;

    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcAD5320_2132 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD5320_2132_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcAD5320_2132_Init(DtBc*  pBc)
{
    DtStatus  Status=DT_STATUS_OK;

    // Sanity checks
    BC_AD5320_2132_DEFAULT_PRECONDITIONS(pBc);
       

    // Set defaults
    BC_AD5320_2132->m_BlockEnabled = FALSE;
    BC_AD5320_2132->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    BC_AD5320_2132->m_GainControl = DT_AD5320_2132_GAIN_AUTO;
    BC_AD5320_2132->m_StepSizeUnlocked = 15;
    BC_AD5320_2132->m_StepSizeLocked = 1;
    BC_AD5320_2132->m_OverflowThreshold = 0xFFF0;
    BC_AD5320_2132->m_WorkingThreshold = 0x9000;
    BC_AD5320_2132->m_TimeWindow = 40000;
    BC_AD5320_2132->m_ManualGain = 0;

    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD5320_2132_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcAD5320_2132_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtBcAD5320_2132* pBc = (DtBcAD5320_2132*)pBcBase;
    UInt32 RegData;
    
    // Sanity checks
    BC_AD5320_2132_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE' set cached values and enable
        DtDbgOutBc(AVG, AD5320_2132, pBc, "Block Enable from disable -> enable");

        // Update gain control
        RegData = AD5320_ControlStatus_READ(pBc);
        RegData = AD5320_ControlStatus_SET_GainControl(RegData,
                                              pBc->m_GainControl==DT_AD5320_2132_GAIN_AUTO 
                                              ? AD5320_GAIN_AUTO : AD5320_GAIN_MANUAL);
        AD5320_ControlStatus_WRITE(pBc, RegData);

        // Update manual gain
        AD5320_ManualGain_WRITE(pBc, (UInt32)pBc->m_ManualGain);

        // Update stepsizes
        RegData = AD5320_StepSizes_READ(pBc);
        RegData = AD5320_StepSizes_SET_StepSizeUnlocked(RegData, pBc->m_StepSizeUnlocked);
        RegData = AD5320_StepSizes_SET_StepSizeLocked(RegData, pBc->m_StepSizeLocked);
        AD5320_StepSizes_WRITE(pBc, RegData);

        // Update overflow threshold
        AD5320_OverflowThreshold_WRITE(pBc, pBc->m_OverflowThreshold&0x3FFFF);

        // Update working threshold
        AD5320_WorkingThreshold_WRITE(pBc, pBc->m_WorkingThreshold&0x3FFFF);

        // Update time window
        AD5320_TimeWindow_WRITE(pBc, pBc->m_TimeWindow);
    }
    else
    { 
        // ENABLE -> DISABLE' disable and set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    pBc->m_BlockEnabled = Enable;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcAD5320_2132_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcAD5320_2132_OnCloseFile(DtBc*  pBc, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_AD5320_2132_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck(pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status))
    {
        DtDbgOutBc(AVG, AD5320_2132, pBc, "Set operational mode to IDLE");

        // Set operational mode to IDLE
        Status = DtBcAD5320_2132_SetOperationalMode(BC_AD5320_2132, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, AD5320_2132, pBc, "ERROR: failed to set opmode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcAD5320_2132 implementation +=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcAD5320_2132
#define AD5320_2132_STUB   ((DtIoStubBcAD5320_2132*)pStub)
#define AD5320_2132_BC  ((DtBcAD5320_2132*)AD5320_2132_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcAD5320_2132_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcAD5320_2132_OnCmdGetGainControl(
                                              const DtIoStubBcAD5320_2132*, 
                                              DtIoctlAd5320Cmd_2132GetGainControlOutput*);
static DtStatus  DtIoStubBcAD5320_2132_OnCmdGetGainCtrlPars(
                                             const DtIoStubBcAD5320_2132*, 
                                             DtIoctlAd5320Cmd_2132GetGainCtrlParsOutput*);
static DtStatus  DtIoStubBcAD5320_2132_OnCmdGetGainStatus(
                                               const DtIoStubBcAD5320_2132*, 
                                               DtIoctlAd5320Cmd_2132GetGainStatusOutput*);
static DtStatus  DtIoStubBcAD5320_2132_OnCmdGetOperationalMode(
                                                  const DtIoStubBcAD5320_2132*, 
                                                  DtIoctlAd5320Cmd_2132GetOpModeOutput*);
static DtStatus  DtIoStubBcAD5320_2132_OnCmdSetGainControl(
                                         const DtIoStubBcAD5320_2132*, 
                                         const DtIoctlAd5320Cmd_2132SetGainControlInput*);
static DtStatus  DtIoStubBcAD5320_2132_OnCmdSetGainCtrlPars(
                                        const DtIoStubBcAD5320_2132*, 
                                        const DtIoctlAd5320Cmd_2132SetGainCtrlParsInput*);
static DtStatus  DtIoStubBcAD5320_2132_OnCmdSetOperationalMode(
                                             const DtIoStubBcAD5320_2132*, 
                                             const DtIoctlAd5320Cmd_2132SetOpModeInput*);
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_AD5320_2132;

static const DtIoctlProperties  IOSTUB_BC_AD5320_2132_IOCTLS[] = 
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_AD5320_CMD_2132(
        DtIoStubBcAD5320_2132_OnCmd,
        NULL,  NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcAD5320_2132 - Public functions +=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD5320_2132_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcAD5320_2132_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcAD5320_2132));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD5320_2132_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcAD5320_2132*  DtIoStubBcAD5320_2132_Open(DtBc*  pBc)
{
    DtIoStubBcAD5320_2132*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcAD5320_2132));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcAD5320_2132, pBc, NULL, 
                                                           DtIoStubBcAD5320_2132_Close,
                                                           NULL,  // Use default IOCTL
                                                           IOSTUB_BC_AD5320_2132_IOCTLS);
    pStub = (DtIoStubBcAD5320_2132*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcAD5320_2132- Private functions +=+=+=+=+=+=+=+=+=+=+=+


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD5320_2132_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcAD5320_2132_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlAd5320CmdInput_2132*  pInData = NULL;
    DtIoctlAd5320CmdOutput_2132*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcAD5320_2132));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_AD5320_CMD_2132);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, AD5320_2132, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_Ad5320Cmd_2132;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_Ad5320Cmd_2132;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_AD5320_CMD_2132_GET_GAIN_CONTROL:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcAD5320_2132_OnCmdGetGainControl(AD5320_2132_STUB,  
                                                                &pOutData->m_GetGainCtrl);
        break;
    case DT_AD5320_CMD_2132_GET_GAIN_CTRL_PARS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcAD5320_2132_OnCmdGetGainCtrlPars(AD5320_2132_STUB,  
                                                                &pOutData->m_GetGainPars);
        break;
    case DT_AD5320_CMD_2132_GET_GAIN_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcAD5320_2132_OnCmdGetGainStatus(AD5320_2132_STUB,  
                                                              &pOutData->m_GetGainStatus);
        break;
    case DT_AD5320_CMD_2132_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcAD5320_2132_OnCmdGetOperationalMode(AD5320_2132_STUB,  
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_AD5320_CMD_2132_SET_GAIN_CONTROL:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcAD5320_2132_OnCmdSetGainControl(AD5320_2132_STUB, 
                                                                 &pInData->m_SetGainCtrl);
        break;
    case DT_AD5320_CMD_2132_SET_GAIN_CTRL_PARS:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcAD5320_2132_OnCmdSetGainCtrlPars(AD5320_2132_STUB, 
                                                                 &pInData->m_SetGainPars);
        break;
    case DT_AD5320_CMD_2132_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcAD5320_2132_OnCmdSetOperationalMode(AD5320_2132_STUB, 
                                                                   &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD5320_2132_OnCmdGetGainControl -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcAD5320_2132_OnCmdGetGainControl(
    const DtIoStubBcAD5320_2132* pStub,
    DtIoctlAd5320Cmd_2132GetGainControlOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcAD5320_2132));
    DT_ASSERT(pOutData != NULL);

    return DtBcAD5320_2132_GetGainControl(AD5320_2132_BC, &pOutData->m_GainControl,
                                                                 &pOutData->m_ManualGain);
}

DtStatus DtIoStubBcAD5320_2132_OnCmdGetGainCtrlPars(
    const DtIoStubBcAD5320_2132* pStub,
    DtIoctlAd5320Cmd_2132GetGainCtrlParsOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcAD5320_2132));
    DT_ASSERT(pOutData != NULL);

    return DtBcAD5320_2132_GetGainCtrlPars(AD5320_2132_BC, &pOutData->m_StepSizeUnlocked,
                              &pOutData->m_StepSizeLocked, &pOutData->m_OverflowThreshold,
                              &pOutData->m_WorkingThreshold, &pOutData->m_TimeWindow);
}

DtStatus DtIoStubBcAD5320_2132_OnCmdGetGainStatus(
    const DtIoStubBcAD5320_2132* pStub,
    DtIoctlAd5320Cmd_2132GetGainStatusOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcAD5320_2132));
    DT_ASSERT(pOutData != NULL);

    return DtBcAD5320_2132_GetGainStatus(AD5320_2132_BC, &pOutData->m_AutoGainStatus,
                                                               &pOutData->m_GainReadBack);
}

//.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD5320_2132_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcAD5320_2132_OnCmdGetOperationalMode(
    const DtIoStubBcAD5320_2132* pStub,
    DtIoctlAd5320Cmd_2132GetOpModeOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcAD5320_2132));
    DT_ASSERT(pOutData != NULL);

    return DtBcAD5320_2132_GetOperationalMode(AD5320_2132_BC, &pOutData->m_OpMode);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD5320_2132_OnCmdSetGainControl -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcAD5320_2132_OnCmdSetGainControl(
    const DtIoStubBcAD5320_2132* pStub,
    const DtIoctlAd5320Cmd_2132SetGainControlInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcAD5320_2132));
    DT_ASSERT(pInData != NULL);

    return DtBcAD5320_2132_SetGainControl(AD5320_2132_BC, pInData->m_GainControl,
                                                                   pInData->m_ManualGain);
}

//.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD5320_2132_OnCmdSetGainCtrlPars -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcAD5320_2132_OnCmdSetGainCtrlPars(
    const DtIoStubBcAD5320_2132* pStub,
    const DtIoctlAd5320Cmd_2132SetGainCtrlParsInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcAD5320_2132));
    DT_ASSERT(pInData != NULL);

    return DtBcAD5320_2132_SetGainCtrlPars(AD5320_2132_BC, pInData->m_StepSizeUnlocked,
                                  pInData->m_StepSizeLocked, pInData->m_OverflowThreshold,
                                  pInData->m_WorkingThreshold, pInData->m_TimeWindow);
}

//.-.-.-.-.-.-.-.-.-.- DtIoStubBcAD5320_2132_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcAD5320_2132_OnCmdSetOperationalMode(
    const DtIoStubBcAD5320_2132* pStub,
    const DtIoctlAd5320Cmd_2132SetOpModeInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcAD5320_2132));
    DT_ASSERT(pInData != NULL);

    return DtBcAD5320_2132_SetOperationalMode(AD5320_2132_BC, pInData->m_OpMode);
}