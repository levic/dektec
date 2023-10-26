// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcDDRFIFO.c *#*#*#*#*#*#*#* (C) 2018-2023 DekTec
//
// This file is not part of the PCIe driver. It can be used as template to create a new
// block controller. Replace "DDRFIFO" by the building block's shortname (all uppercase)
// and replace "Template" by the building block's shortname in camel case. 
// 

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018 DekTec Digital Video B.V.
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
#include "DtBcDDRFIFO.h"
#include "DtBcDDRFIFO_RegAccess.h"
#include "Messages.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDDRFIFO implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_DDRFIFO_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcDDRFIFO))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_DDRFIFO_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(DDRFIFO, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcDDRFIFO_Init(DtBc*);
static DtStatus  DtBcDDRFIFO_OnEnable(DtBc*, Bool  Enable);
static void  DtBcDDRFIFO_SetControlRegs(DtBcDDRFIFO*, Bool BlkEna, Int OpMode);

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDDRFIFO - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFIFO_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcDDRFIFO_Close(DtBc*  pBc)
{
    BC_DDRFIFO_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFIFO_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcDDRFIFO*  DtBcDDRFIFO_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_DDRFIFO_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcDDRFIFO, Id, DT_BLOCK_TYPE_DDRFIFO,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcDDRFIFO_Close;
    OpenParams.m_InitFunc = DtBcDDRFIFO_Init;
    OpenParams.m_OnEnableFunc = DtBcDDRFIFO_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcDDRFIFO_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcDDRFIFO*)DtBc_Open(&OpenParams);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFIFO_GetFifoLoad -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcDDRFIFO_GetFifoLoad(DtBcDDRFIFO* pBc, Int* pFifoLoad)
{
    // Sanity check
    BC_DDRFIFO_DEFAULT_PRECONDITIONS(pBc);

    if (pFifoLoad == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_DDRFIFO_MUST_BE_ENABLED(pBc);

    *pFifoLoad = DDRFIFO_FifoLoad_READ(pBc);
    *pFifoLoad *= (pBc->m_DataWidth/8);     // #bytes

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFIFO_GetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcDDRFIFO_GetProperties(DtBcDDRFIFO* pBc, Int* pBurstSize, Int* pFifoSize,
                                                                          Int* pDataWidth)
{
    // Sanity check
    BC_DDRFIFO_DEFAULT_PRECONDITIONS(pBc);

    if (pBurstSize==NULL || pFifoSize==NULL || pDataWidth==NULL)
        return DT_STATUS_INVALID_PARAMETER;

    *pBurstSize = pBc->m_BurstSize;
    *pFifoSize = pBc->m_FifoSize;
    *pDataWidth = pBc->m_DataWidth;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFIFO_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDDRFIFO_GetOperationalMode(DtBcDDRFIFO* pBc, Int* pOpMode)
{
    // Sanity check
    BC_DDRFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_DDRFIFO_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFIFO_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDDRFIFO_SetOperationalMode(DtBcDDRFIFO* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;
    Int TimeoutCount = 0;
    UInt32  ExpectFwOpStatus = 0, FwOpStatus = 0;

    // Sanity check
    BC_DDRFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY 
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_DDRFIFO_MUST_BE_ENABLED(pBc);
    
    // No change?
    if (pBc->m_OperationalMode == OpMode)
        return DT_STATUS_OK;

    // Change Operational Mode
    if (OpMode == DT_BLOCK_OPMODE_RUN)
    {
        // IDLE or STANDBY -> RUN
        
    }
    else if (pBc->m_OperationalMode == DT_BLOCK_OPMODE_RUN)
    {
        // RUN -> STANDBY or IDLE
    }

    // Cache value
    pBc->m_OperationalMode = OpMode;
    // Set control register
    DtBcDDRFIFO_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    // Convert operational mode to BB-type
    switch (OpMode)
    {
     case DT_BLOCK_OPMODE_IDLE:    ExpectFwOpStatus = DDRFIFO_OPMODE_IDLE; break;
     case DT_BLOCK_OPMODE_STANDBY: ExpectFwOpStatus = DDRFIFO_OPMODE_STANDBY; break;
     case DT_BLOCK_OPMODE_RUN:     ExpectFwOpStatus = DDRFIFO_OPMODE_RUN; break;
     default: DT_ASSERT(FALSE); break;
    }

    // Wait till the DDRFIFO has reached the requested operational mode  
    FwOpStatus = DDRFIFO_Status_READ_OperationalStatus(pBc);
    for (TimeoutCount = 100; TimeoutCount > 0 && FwOpStatus != ExpectFwOpStatus;
                                                                           TimeoutCount--)
    {
        DtWaitBlock(10);
        FwOpStatus = DDRFIFO_Status_READ_OperationalStatus(pBc);
    }

    if (FwOpStatus == ExpectFwOpStatus)
        DtDbgOutBc(AVG, DDRFIFO, pBc, "Entered new operational mode %d", OpMode);
    else
    {
        DtDbgOutBc(ERR, DDRFIFO, pBc, "ERROR: timeout set new OpMode %d", OpMode);
        Status = DT_STATUS_TIMEOUT;
    }
    return Status;
}

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDDRFIFO - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+


// -.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFIFO_CheckMemoryControllerStatus -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcDDRFIFO_CheckMemoryControllerStatus(DtBcDDRFIFO* pBc)
{
    DtStatus  Status = DT_STATUS_OK;
    Int TimeoutCount = 0;
    const Int MaxNumRetries = 2;
    Int RetryCount = MaxNumRetries;

    // Wait for PLL in lock
    TimeoutCount = 0;
    while (TimeoutCount<500 && DDRFIFO_Status_READ_MemPllLocked(pBc)==0)
    {
        DtWaitBlock(10);
        TimeoutCount++;
    }
    if (DDRFIFO_Status_READ_MemPllLocked(pBc) == 0)
    {
        DtString  Str;
        DtDbgOutBc(ERR, DDRFIFO, pBc, "PLL memory not in lock");
        DtStringAlloc(&Str, 256);

        DtStringAppendChars(&Str, "[SN=");
        DtStringUInt64ToDtStringAppend(&Str, 10, pBc->m_pCore->m_pDevInfo->m_Serial);
        DtStringAppendChars(&Str, "] ");
        DtStringAppendChars(&Str, "PLL memory not in lock");
        DtEvtLogReport(&pBc->m_pCore->m_Device.m_EvtObject, DTPCIE_LOG_ERROR_GENERIC, 
                                                                        &Str, NULL, NULL);
        DtStringFree(&Str);
        Status = DT_STATUS_FAIL;
    }
    if (DT_SUCCESS(Status))
    {
        if (DDRFIFO_Status_READ_MemCalSuccess(pBc) == 0)
        {
            DtString  Str;
            DtDbgOutBc(ERR, DDRFIFO, pBc, "Mem calibration not successfull after"
                                                                              " startup");
            DtStringAlloc(&Str, 256);
            DtStringAppendChars(&Str, "[SN=");
            DtStringUInt64ToDtStringAppend(&Str, 10, pBc->m_pCore->m_pDevInfo->m_Serial);
            DtStringAppendChars(&Str, "] ");
            DtStringAppendChars(&Str, "Mem calibration not successfull");
            DtEvtLogReport(&pBc->m_pCore->m_Device.m_EvtObject, DTPCIE_LOG_ERROR_GENERIC, 
                                                                        &Str, NULL, NULL);
            DtStringFree(&Str);
            //Status = DT_STATUS_FAIL;
            DT_ASSERT(FALSE);
        }
    }
    if (DT_SUCCESS(Status))
    {
        // Wait for mem. calibration success/fail
        while (RetryCount>0 && DDRFIFO_Status_READ_MemCalFail(pBc)==0 &&
                                                DDRFIFO_Status_READ_MemCalSuccess(pBc)==0)
        {
            TimeoutCount = 0;
            while (TimeoutCount<100 && DDRFIFO_Status_READ_MemCalFail(pBc)==0 &&
                                                DDRFIFO_Status_READ_MemCalSuccess(pBc)==0)
            {
                DtWaitBlock(10);
                TimeoutCount++;
            }
            if (DDRFIFO_Status_READ_MemCalFail(pBc)==0 &&
                                                DDRFIFO_Status_READ_MemCalSuccess(pBc)==0)
            {
                UInt32 MemControl;
                // Do reset
                RetryCount--;
                if (RetryCount != 0)
                {
                    DtDbgOutBc(ERR, DDRFIFO, pBc, "Mem calibration not started/"
                                                        "finished. DoReset. #Retries: %i",
                                                        MaxNumRetries-RetryCount);
                    MemControl = DDRFIFO_MemControl_READ(pBc);
                    DDRFIFO_MemControl_SET_DoReset(MemControl, 1);
                    DDRFIFO_MemControl_WRITE(pBc, MemControl);
                }
            }
        }
        if (DDRFIFO_Status_READ_MemCalSuccess(pBc) != 0)
            DtDbgOutBc(MIN, DDRFIFO, pBc, "Mem calibration success. #Retries: %i",
                                                                MaxNumRetries-RetryCount);
        else if (DDRFIFO_Status_READ_MemCalFail(pBc) != 0)
        {
            DtString  Str;
            DtDbgOutBc(ERR, DDRFIFO, pBc, "Mem calibration failed. #Retries: %i",
                                                                MaxNumRetries-RetryCount);
            DtStringAlloc(&Str, 256);

            DtStringAppendChars(&Str, "[SN=");
            DtStringUInt64ToDtStringAppend(&Str, 10, pBc->m_pCore->m_pDevInfo->m_Serial);
            DtStringAppendChars(&Str, "] ");
            DtStringAppendChars(&Str, "Mem calibration error. Failed");
            DtEvtLogReport(&pBc->m_pCore->m_Device.m_EvtObject, DTPCIE_LOG_ERROR_GENERIC, 
                                                                        &Str, NULL, NULL);
            DtStringFree(&Str);
            Status = DT_STATUS_FAIL;
            DT_ASSERT(FALSE);
        }
        else
        {
            DtString  Str;
            DtDbgOutBc(ERR, DDRFIFO, pBc, "Mem calibration not started/finished.");
            DtStringAlloc(&Str, 256);

            DtStringAppendChars(&Str, "[SN=");
            DtStringUInt64ToDtStringAppend(&Str, 10, pBc->m_pCore->m_pDevInfo->m_Serial);
            DtStringAppendChars(&Str, "] ");
            DtStringAppendChars(&Str, "Mem calibration error. Not started/finished");
            DtEvtLogReport(&pBc->m_pCore->m_Device.m_EvtObject, DTPCIE_LOG_ERROR_GENERIC, 
                                                                        &Str, NULL, NULL);

            DtStringFree(&Str);
            Status = DT_STATUS_FAIL;
            DT_ASSERT(FALSE);
        }
    }
    // Always succeed. Otherwise the driver is not loaded. 
    // We'll check the success/failed state later
    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFIFO_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcDDRFIFO_Init(DtBc* pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32  Properties = 0;
    DtBcDDRFIFO* pBc = (DtBcDDRFIFO*)pBcBase;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    // Set control register
    DtBcDDRFIFO_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    // cache properties
    Properties = DDRFIFO_Config_READ(pBc);

    pBc->m_DataWidth = 1 << DDRFIFO_Config_GET_DataWidth(Properties);            // #bits
    pBc->m_BurstSize = (1 << DDRFIFO_Config_GET_BurstSize(Properties)) * 
                                                           (pBc->m_DataWidth/8); // #bytes

    pBc->m_FifoSize = 1 << DDRFIFO_Config_GET_FifoSize(Properties);              // #bytes
    
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFIFO_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcDDRFIFO_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcDDRFIFO* pBc = (DtBcDDRFIFO*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_DDRFIFO_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, DDRFIFO, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcDDRFIFO_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, DDRFIFO, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFIFO_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcDDRFIFO_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcDDRFIFO* pBc = (DtBcDDRFIFO*)pBcBase;
    
    // Sanity check
    BC_DDRFIFO_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, DDRFIFO, pBc, "OnEnable from disable -> enable");
        Status = DtBcDDRFIFO_CheckMemoryControllerStatus(pBc);

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, DDRFIFO, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcDDRFIFO_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, DDRFIFO, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcDDRFIFO_SetControlRegs(pBc, pBc->m_BlockEnabled, pBc->m_OperationalMode);

    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFIFO_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcDDRFIFO_SetControlRegs(DtBcDDRFIFO* pBc, Bool BlkEna, Int OpMode)
{
    UInt32  RegData=0, FwBlkEna=0, FwOpMode=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? DDRFIFO_BLKENA_ENABLED : DDRFIFO_BLKENA_DISABLED;

    // Convert operational mode to BB-type
    switch (OpMode)
    {
    case DT_BLOCK_OPMODE_IDLE:    FwOpMode = DDRFIFO_OPMODE_IDLE; break;
    case DT_BLOCK_OPMODE_STANDBY: FwOpMode = DDRFIFO_OPMODE_STANDBY; break;
    case DT_BLOCK_OPMODE_RUN:     FwOpMode = DDRFIFO_OPMODE_RUN; break;
    default: DT_ASSERT(FALSE);
    }

    // Update control register
    RegData = DDRFIFO_Control_READ(pBc);
    RegData = DDRFIFO_Control_SET_BlockEnable(RegData, FwBlkEna);
    RegData = DDRFIFO_Control_SET_OperationalMode(RegData, FwOpMode);
    DDRFIFO_Control_WRITE(pBc, RegData);
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDDRFIFO implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcDDRFIFO
#define DDRFIFO_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                      DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcDDRFIFO))
#define DDRFIFO_STUB   ((DtIoStubBcDDRFIFO*)pStub)
#define DDRFIFO_BC  ((DtBcDDRFIFO*)DDRFIFO_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcDDRFIFO_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcDDRFIFO_OnCmdGetProperties(const DtIoStubBcDDRFIFO*,
                                                   DtIoctlDdrFifoCmdGetPropertiesOutput*);
static DtStatus  DtIoStubBcDDRFIFO_OnCmdGetFifoLoad(const DtIoStubBcDDRFIFO*,
                                                     DtIoctlDdrFifoCmdGetFifoLoadOutput*);
static DtStatus  DtIoStubBcDDRFIFO_OnCmdGetOperationalMode(const DtIoStubBcDDRFIFO*,
                                                       DtIoctlDdrFifoCmdGetOpModeOutput*);
static DtStatus  DtIoStubBcDDRFIFO_OnCmdSetOperationalMode(const DtIoStubBcDDRFIFO*,
                                                  const DtIoctlDdrFifoCmdSetOpModeInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_DDRFIFO;

static const DtIoctlProperties  IOSTUB_BC_DDRFIFO_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_DDRFIFO_CMD(
        DtIoStubBcDDRFIFO_OnCmd,
        NULL,  NULL),
};

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDDRFIFO - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDDRFIFO_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcDDRFIFO_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub != NULL && pStub->m_Size == sizeof(DtIoStubBcDDRFIFO));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDDRFIFO_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcDDRFIFO*  DtIoStubBcDDRFIFO_Open(DtBc*  pBc)
{
    DtIoStubBcDDRFIFO*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc != NULL && pBc->m_Size == sizeof(DtBcDDRFIFO));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcDDRFIFO, pBc, NULL,
        DtIoStubBcDDRFIFO_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_DDRFIFO_IOCTLS);
    pStub = (DtIoStubBcDDRFIFO*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFIFO_IsMemCalibrated -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
Bool  DtBcDDRFIFO_IsMemCalibrated(DtBcDDRFIFO* pBc)
{
    return DDRFIFO_Status_READ_MemCalSuccess(pBc) != 0;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFIFO_IsMemPllLocked -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
Bool  DtBcDDRFIFO_IsMemPllLocked(DtBcDDRFIFO* pBc)
{
    return DDRFIFO_Status_READ_MemPllLocked(pBc) != 0;
}

// =+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDDRFIFO- Private functions +=+=+=+=+=+=+=+=+=+=+=+=+


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDDRFIFO_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcDDRFIFO_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
    Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlDdrFifoCmdInput*  pInData = NULL;
    DtIoctlDdrFifoCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub != NULL && pStub->m_Size == sizeof(DtIoStubBcDDRFIFO));
    DT_ASSERT(pIoParams != NULL && pOutSize != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_DDRFIFO_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, DDRFIFO, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_DdrFifoCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_DdrFifoCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_DDRFIFO_CMD_GET_PROPERTIES:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcDDRFIFO_OnCmdGetProperties(DDRFIFO_STUB,
                                                              &pOutData->m_GetProperties);
        break;
    case DT_DDRFIFO_CMD_GET_FIFO_LOAD:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcDDRFIFO_OnCmdGetFifoLoad(DDRFIFO_STUB, 
                                                                &pOutData->m_GetFifoLoad);
        break;
    case DT_DDRFIFO_CMD_GET_OPERATIONAL_MODE:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcDDRFIFO_OnCmdGetOperationalMode(DDRFIFO_STUB,
                                                                  &pOutData->m_GetOpMode);
        break;
    case DT_DDRFIFO_CMD_SET_OPERATIONAL_MODE:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcDDRFIFO_OnCmdSetOperationalMode(DDRFIFO_STUB,
                                                                   &pInData->m_SetOpMode);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDDRFIFO_OnCmdGetFifoLoad -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcDDRFIFO_OnCmdGetFifoLoad(
    const DtIoStubBcDDRFIFO* pStub,
    DtIoctlDdrFifoCmdGetFifoLoadOutput* pOutData)
{
    DDRFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcDDRFIFO_GetFifoLoad(DDRFIFO_BC, &pOutData->m_FifoLoad);
}


// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDDRFIFO_OnCmdGetProperties -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcDDRFIFO_OnCmdGetProperties(
    const DtIoStubBcDDRFIFO* pStub,
    DtIoctlDdrFifoCmdGetPropertiesOutput* pOutData)
{
    DDRFIFO_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcDDRFIFO_GetProperties(DDRFIFO_BC, &pOutData->m_BurstSize,
                                           &pOutData->m_FifoSize, &pOutData->m_DataWidth);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDDRFIFO_OnCmdGetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcDDRFIFO_OnCmdGetOperationalMode(
    const DtIoStubBcDDRFIFO* pStub,
    DtIoctlDdrFifoCmdGetOpModeOutput* pOutData)
{
    DT_ASSERT(pStub != NULL && pStub->m_Size == sizeof(DtIoStubBcDDRFIFO));
    DT_ASSERT(pOutData != NULL);

    return DtBcDDRFIFO_GetOperationalMode(DDRFIFO_BC, &pOutData->m_OpMode);
}

// .-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDDRFIFO_OnCmdSetOperationalMode -.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcDDRFIFO_OnCmdSetOperationalMode(
    const DtIoStubBcDDRFIFO* pStub,
    const DtIoctlDdrFifoCmdSetOpModeInput* pInData)
{
    DT_ASSERT(pStub != NULL && pStub->m_Size == sizeof(DtIoStubBcDDRFIFO));
    DT_ASSERT(pInData != NULL);

    return DtBcDDRFIFO_SetOperationalMode(DDRFIFO_BC, pInData->m_OpMode);
}
