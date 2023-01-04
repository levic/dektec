//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcIO_2116.c *#*#*#*#*#*#*#*#*#*# (C) 2021 DekTec
//
//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Copyright (C) 2021 DekTec Digital Video B.V.
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
#include "DtBc.h"
#include "DtBcIO_2116.h"
#include "DtBcIO_2116_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIO_2116 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_IO_2116_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcIO_2116))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_IO_2116_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(IO_2116, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcIO_2116_Init(DtBc*);
static DtStatus  DtBcIO_2116_OnCloseFile(DtBc*, const DtFileObject*);


// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIO_2116 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIO_2116_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcIO_2116_Close(DtBc* pBc)
{
    BC_IO_2116_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIO_2116_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcIO_2116*  DtBcIO_2116_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_IO_2116_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcIO_2116, Id, DT_BLOCK_TYPE_IO_2116,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcIO_2116_Close;
    OpenParams.m_InitFunc = DtBcIO_2116_Init;
    OpenParams.m_OnCloseFileFunc = DtBcIO_2116_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcIO_2116*)DtBc_Open(&OpenParams);
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcIO_2116 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIO_2116_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIO_2116_Init(DtBc* pBcBase)
{
    DtBcIO_2116* pBc = (DtBcIO_2116*)pBcBase;

    // Sanity checks
    BC_IO_2116_DEFAULT_PRECONDITIONS(pBc);

    // Initialize IO mutex against concurrent access
    DtFastMutexInit(&pBc->m_AccessMutex);

    // Initialize clock generator
    DtBcIO_2116_InitClockGen(pBc);
    DtBcIO_2116_SetCalibrationSwitch(pBc, FALSE, FALSE);
    DtBcIO_2116_SetDacTxEnable(pBc, FALSE);
    DtBcIO_2116_SetOnePpsTermination(pBc, FALSE);
    DtBcIO_2116_Set10MhzClockSelect(pBc, DT_IO_2116_CLKSELECT_INTERNAL);

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIO_2116_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIO_2116_OnCloseFile(DtBc* pBc, const DtFileObject* pFile)
{
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_IO_2116_DEFAULT_PRECONDITIONS(pBc);
   
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBc, pFile);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBCIO_2116_DacReset -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// TODO: JO sure this is OK ?
DtStatus  DtBCIO_2116_DacReset(DtBcIO_2116* pBc)
{
    UInt32 RegData=0;
    BC_IO_2116_DEFAULT_PRECONDITIONS(pBc);

    // Protect IO access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    RegData = IO_2116_DacCtrl_READ(pBc);
    
    // reset is active low
    RegData = IO_2116_DacCtrl_SET_Resetn(RegData, 0);
    IO_2116_DacCtrl_WRITE(pBc, RegData);
    RegData = IO_2116_DacCtrl_SET_Resetn(RegData, 1);
    IO_2116_DacCtrl_WRITE(pBc, RegData);

    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIO_2116_Get10MhzClockSelect -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIO_2116_Get10MhzClockSelect(DtBcIO_2116* pBc, Int* pSelect)
{
    BC_IO_2116_DEFAULT_PRECONDITIONS(pBc);

    // Protect IO access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);
    *pSelect = pBc->m_10MhzClkSelect;
    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIO_2116_GetCalibrationSwitch -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIO_2116_GetCalibrationSwitch(DtBcIO_2116* pBc, Bool* pSw1, Bool* pSw2)
{
    BC_IO_2116_DEFAULT_PRECONDITIONS(pBc);

    // Protect IO access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);
    *pSw1 = pBc->m_CalibSwitch1;
    *pSw2 = pBc->m_CalibSwitch2;
    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIO_2116_GetClockGenStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIO_2116_GetClockGenStatus(DtBcIO_2116* pBc, Bool* pStatus)
{
    BC_IO_2116_DEFAULT_PRECONDITIONS(pBc);

    // Protect IO access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);
    *pStatus = (IO_2116_ClkGenCtrl_READ_Status(pBc) == 1) ? TRUE: FALSE;
    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIO_2116_GetDacTxEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIO_2116_GetDacTxEnable(DtBcIO_2116* pBc, Bool* pState)
{
    BC_IO_2116_DEFAULT_PRECONDITIONS(pBc);

    // Protect IO access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);
    *pState = pBc->m_DacTxEnable;
    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIO_2116_GetOnePpsTermination -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIO_2116_GetOnePpsTermination(DtBcIO_2116* pBc, Bool* pState)
{
    BC_IO_2116_DEFAULT_PRECONDITIONS(pBc);

    // Protect IO access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);
    *pState = pBc->m_OnePpsTermination;
    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIO_2116_Set10MhzClockSelect -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIO_2116_Set10MhzClockSelect(DtBcIO_2116* pBc, Int Select)
{
    UInt32 RegData=0;
    BC_IO_2116_DEFAULT_PRECONDITIONS(pBc);
    
    if(Select!=DT_IO_2116_CLKSELECT_INTERNAL && Select!=DT_IO_2116_CLKSELECT_EXTERNAL)
        return DT_STATUS_INVALID_PARAMETER;

    // Protect IO access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);
    pBc->m_10MhzClkSelect = Select;
    RegData = IO_2116_ClkGenCtrl_READ(pBc);
    RegData = IO_2116_ClkGenCtrl_SET_ClkSelect(RegData, 
                    (Select==DT_IO_2116_CLKSELECT_INTERNAL) ? IO_2116_CLKSELECT_INTERNAL :
                                                              IO_2116_CLKSELECT_EXTERNAL);
    IO_2116_ClkGenCtrl_WRITE(pBc, RegData);

    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIO_2116_InitClockGen -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIO_2116_InitClockGen(DtBcIO_2116* pBc)
{
    UInt32 RegData=0;
    BC_IO_2116_DEFAULT_PRECONDITIONS(pBc);

    // Protect IO access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);
    
    RegData = IO_2116_ClkGenCtrl_READ(pBc);
    // Active low
    RegData = IO_2116_ClkGenCtrl_SET_ShutDown(RegData, 0);
    IO_2116_ClkGenCtrl_WRITE(pBc, RegData);
    RegData = IO_2116_ClkGenCtrl_SET_ShutDown(RegData, 1);
    IO_2116_ClkGenCtrl_WRITE(pBc, RegData);

    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIO_2116_SetCalibrationSwitch -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIO_2116_SetCalibrationSwitch(DtBcIO_2116* pBc, Bool Sw1, Bool Sw2)
{
    UInt32 RegData=0;
    BC_IO_2116_DEFAULT_PRECONDITIONS(pBc);

    // Protect IO access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    pBc->m_CalibSwitch1 = Sw1;
    pBc->m_CalibSwitch2 = Sw2;
    RegData = IO_2116_CalibCtrl_READ(pBc);
    RegData = IO_2116_CalibCtrl_SET_CalibSw1(RegData, (Sw1==TRUE)?1:0);
    RegData = IO_2116_CalibCtrl_SET_CalibSw2(RegData, (Sw2==TRUE)?1:0);
    IO_2116_CalibCtrl_WRITE(pBc, RegData);

    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIO_2116_SetDacTxEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcIO_2116_SetDacTxEnable(DtBcIO_2116* pBc, Bool State)
{
    UInt32 RegData=0;
    BC_IO_2116_DEFAULT_PRECONDITIONS(pBc);

    // Protect IO access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    pBc->m_DacTxEnable = State;
    RegData = IO_2116_DacCtrl_READ(pBc);
    RegData = IO_2116_DacCtrl_SET_TxEnable(RegData, (State==TRUE)? 1:0);
    IO_2116_DacCtrl_WRITE(pBc, RegData);

    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtBcIO_2116_SetOnePpsTermination -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcIO_2116_SetOnePpsTermination(DtBcIO_2116* pBc, Bool State)
{
    UInt32 RegData=0;
    BC_IO_2116_DEFAULT_PRECONDITIONS(pBc);

    // Protect IO access against concurrent access
    DtFastMutexAcquire(&pBc->m_AccessMutex);

    pBc->m_OnePpsTermination = State;
    RegData = IO_2116_OnePpsCtrl_READ(pBc);
    RegData = IO_2116_OnePpsCtrl_SET_OnePpsTermEn(RegData, (State==TRUE)?1:0);
    IO_2116_OnePpsCtrl_WRITE(pBc, RegData);

    DtFastMutexRelease(&pBc->m_AccessMutex);
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIO_2116 implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define IO_2116_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIO_2116))
// Helper macro to cast stub's DtBc member to DtBcIO_2116
#define IO_2116_STUB   ((DtIoStubBcIO_2116*)pStub)
#define IO_2116_BC  ((DtBcIO_2116*)IO_2116_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcIO_2116_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcIO_2116_OnCmdDacReset(const DtIoStubBcIO_2116*);
static DtStatus  DtIoStubBcIO_2116_OnCmdGet10MhzClockSelect(const DtIoStubBcIO_2116*, 
                                             DtIoctlIoCmd_2116Get10MhzClockSelectOutput*);
static DtStatus  DtIoStubBcIO_2116_OnCmdGetCalibrationSwitch(const DtIoStubBcIO_2116*, 
                                            DtIoctlIoCmd_2116GetCalibrationSwitchOutput*);
static DtStatus  DtIoStubBcIO_2116_OnCmdGetClockGenStatus(const DtIoStubBcIO_2116*, 
                                              DtIoctlIoCmd_2116GetClockGenStatusOutput* );
static DtStatus  DtIoStubBcIO_2116_OnCmdGetDacTxEnable(const DtIoStubBcIO_2116*, 
                                                  DtIoctlIoCmd_2116GetDacTxEnableOutput*);
static DtStatus  DtIoStubBcIO_2116_OnCmdGetOnePpsTermination(const DtIoStubBcIO_2116*, 
                                            DtIoctlIoCmd_2116GetOnePpsTerminationOutput*);
static DtStatus  DtIoStubBcIO_2116_OnCmdSet10MhzClockSelect(const DtIoStubBcIO_2116*, 
                                        const DtIoctlIoCmd_2116Set10MhzClockSelectInput*);
static DtStatus  DtIoStubBcIO_2116_OnCmdSetCalibrationSwitch(const DtIoStubBcIO_2116*, 
                                       const DtIoctlIoCmd_2116SetCalibrationSwitchInput*);
static DtStatus  DtIoStubBcIO_2116_OnCmdSetDacTxEnable(const DtIoStubBcIO_2116*, 
                                             const DtIoctlIoCmd_2116SetDacTxEnableInput*);
static DtStatus  DtIoStubBcIO_2116_OnCmdSetOnePpsTermination(const DtIoStubBcIO_2116*, 
                                       const DtIoctlIoCmd_2116SetOnePpsTerminationInput*);
static DtStatus  DtIoStubBcIO_2116_OnCmdInitClockGen(const DtIoStubBcIO_2116*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_IO_2116;

static const DtIoctlProperties  IOSTUB_BC_IO_2116_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_IO_2116_CMD(
        DtIoStubBcIO_2116_OnCmd,
        NULL, NULL),
};

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIO_2116 - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcIO_2116_Close(DtIoStub* pStub)
{
    IO_2116_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcIO_2116*  DtIoStubBcIO_2116_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_IO_2116_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcIO_2116, pBc, NULL, 
                                                            DtIoStubBcIO_2116_Close,
                                                            NULL,  // Use default IOCTL
                                                            IOSTUB_BC_IO_2116_IOCTLS);
    return (DtIoStubBcIO_2116*)DtIoStubBc_Open(&OpenParams);
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcIO_2116 - Private functions +=+=+=+=+=+=+=+=+=+=+=+=


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIO_2116_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlIoCmdInput_2116*  pInData = NULL;
    DtIoctlIoCmdOutput_2116*  pOutData = NULL;

    IO_2116_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_IO_CMD_2116);
    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, IO_2116, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_GpioCmd_2116;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_GpioCmd_2116;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-
    switch (pIoParams->m_Cmd)
    {
    case DT_IO_CMD_2116_DAC_RESET:
        Status = DtIoStubBcIO_2116_OnCmdDacReset(IO_2116_STUB);
        break;
    case DT_IO_CMD_2116_GET_10MHZ_CLOCK_SELECT:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIO_2116_OnCmdGet10MhzClockSelect(IO_2116_STUB, 
                                                        &pOutData->m_Get10MhzClockSelect);
        break;
    case DT_IO_CMD_2116_GET_CALIBRATION_SWITCH:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIO_2116_OnCmdGetCalibrationSwitch(IO_2116_STUB, 
                                                       &pOutData->m_GetCalibrationSwitch);
        break;
    case DT_IO_CMD_2116_GET_CLOCKGEN_STATUS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIO_2116_OnCmdGetClockGenStatus(IO_2116_STUB, 
                                                          &pOutData->m_GetClockGenStatus);
        break;
    case DT_IO_CMD_2116_GET_DAC_TX_ENABLE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIO_2116_OnCmdGetDacTxEnable(IO_2116_STUB, 
                                                             &pOutData->m_GetDacTxEnable);
        break;
    case DT_IO_CMD_2116_GET_ONEPPS_TERMINATION:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIO_2116_OnCmdGetOnePpsTermination(IO_2116_STUB, 
                                                       &pOutData->m_GetOnePpsTermination);
        break;
    case DT_IO_CMD_2116_SET_10MHZ_CLOCK_SELECT:
        DT_ASSERT(pInData != NULL);
        DtIoStubBcIO_2116_OnCmdSet10MhzClockSelect(IO_2116_STUB, 
                                                         &pInData->m_Set10MhzClockSelect);
        break;
    case DT_IO_CMD_2116_SET_CALIBRATION_SWITCH:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIO_2116_OnCmdSetCalibrationSwitch(IO_2116_STUB, 
                                                        &pInData->m_SetCalibrationSwitch);
        break;
    case DT_IO_CMD_2116_SET_DAC_TX_ENABLE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcIO_2116_OnCmdSetDacTxEnable(IO_2116_STUB, 
                                                              &pInData->m_SetDacTxEnable);
        break;
    case DT_IO_CMD_2116_SET_ONEPPS_TERMINATION:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcIO_2116_OnCmdSetOnePpsTermination(IO_2116_STUB, 
                                                        &pInData->m_SetOnePpsTermination);
        break;
    case DT_IO_CMD_2116_INIT_CLOCKGEN:
        Status = DtIoStubBcIO_2116_OnCmdInitClockGen(IO_2116_STUB);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116_OnCmdDacReset -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIO_2116_OnCmdDacReset(const DtIoStubBcIO_2116* pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIO_2116));
    return DtBCIO_2116_DacReset(IO_2116_BC);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116_OnCmdGet10MhzClockSelect -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIO_2116_OnCmdGet10MhzClockSelect(const DtIoStubBcIO_2116* pStub, 
                                   DtIoctlIoCmd_2116Get10MhzClockSelectOutput* pOutData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIO_2116));
    DT_ASSERT(pOutData!=NULL);
    return DtBcIO_2116_Get10MhzClockSelect(IO_2116_BC, &pOutData->m_Select);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116_OnCmdGetCalibrationSwitch -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIO_2116_OnCmdGetCalibrationSwitch(const DtIoStubBcIO_2116* pStub, 
                                  DtIoctlIoCmd_2116GetCalibrationSwitchOutput* pOutData)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool Sw1, Sw2;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIO_2116));
    DT_ASSERT(pOutData!=NULL);    

    Status = DtBcIO_2116_GetCalibrationSwitch(IO_2116_BC, &Sw1, &Sw2);
    pOutData->m_Switch1 = Sw1;
    pOutData->m_Switch2 = Sw2;
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116_OnCmdGetClockGenStatus -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIO_2116_OnCmdGetClockGenStatus(const DtIoStubBcIO_2116* pStub,
                                    DtIoctlIoCmd_2116GetClockGenStatusOutput*  pOutData)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool  GenStatus;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIO_2116));
    DT_ASSERT(pOutData!=NULL);
    Status = DtBcIO_2116_GetClockGenStatus(IO_2116_BC, &GenStatus);
    pOutData->m_GenStatus = GenStatus;
    return  Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116_OnCmdGetDacTxEnable -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIO_2116_OnCmdGetDacTxEnable(const DtIoStubBcIO_2116* pStub, 
                                        DtIoctlIoCmd_2116GetDacTxEnableOutput* pOutData)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool State;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIO_2116));
    DT_ASSERT(pOutData!=NULL);
    Status = DtBcIO_2116_GetDacTxEnable(IO_2116_BC, &State);
    pOutData->m_Enable=State;
    return  Status;
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116_OnCmdGetOnePpsTermination -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIO_2116_OnCmdGetOnePpsTermination(const DtIoStubBcIO_2116* pStub, 
                                  DtIoctlIoCmd_2116GetOnePpsTerminationOutput* pOutData)
{
    DtStatus  Status = DT_STATUS_OK;
    Bool State;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIO_2116));
    DT_ASSERT(pOutData!=NULL);
    Status = DtBcIO_2116_GetOnePpsTermination(IO_2116_BC, &State);
    pOutData->m_Terminated = State;
    return  Status;
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116_OnCmdSet10MhzClockSelect -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIO_2116_OnCmdSet10MhzClockSelect(const DtIoStubBcIO_2116* pStub,
                               const DtIoctlIoCmd_2116Set10MhzClockSelectInput* pInData)
{
    // Must be set through IO-config.
    return DT_STATUS_NOT_SUPPORTED;
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116_OnCmdSetCalibrationSwitch -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIO_2116_OnCmdSetCalibrationSwitch(const DtIoStubBcIO_2116* pStub, 
                              const DtIoctlIoCmd_2116SetCalibrationSwitchInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIO_2116));
    DT_ASSERT(pInData!=NULL);
    return DtBcIO_2116_SetCalibrationSwitch(IO_2116_BC, 
                                                      (pInData->m_Switch1==1)?TRUE:FALSE, 
                                                      (pInData->m_Switch2==1)?TRUE:FALSE);
}

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116_OnCmdSetDacTxEnable -.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcIO_2116_OnCmdSetDacTxEnable(const DtIoStubBcIO_2116* pStub, 
                                    const DtIoctlIoCmd_2116SetDacTxEnableInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIO_2116));
    DT_ASSERT(pInData!=NULL);
    return DtBcIO_2116_SetDacTxEnable(IO_2116_BC, (pInData->m_Enable==1)?TRUE:FALSE);
}

// -.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116_OnCmdSetOnePpsTermination -.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIO_2116_OnCmdSetOnePpsTermination(const DtIoStubBcIO_2116* pStub, 
                              const DtIoctlIoCmd_2116SetOnePpsTerminationInput* pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIO_2116));
    DT_ASSERT(pInData!=NULL);
    return DtBcIO_2116_SetOnePpsTermination(IO_2116_BC, 
                                                   (pInData->m_Terminated==1)?TRUE:FALSE);
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcIO_2116_OnCmdInitClockGen -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcIO_2116_OnCmdInitClockGen(const DtIoStubBcIO_2116* pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcIO_2116));
    return DtBcIO_2116_InitClockGen(IO_2116_BC);
}

