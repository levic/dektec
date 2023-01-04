// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcDDRFRONT.c *#*#*#*#*#*#*#*# (C) 2018-2021 DekTec
//
// This file is not part of the PCIe driver. It can be used as template to create a new
// block controller. Replace "DDRFRONT" by the building block's shortname (all uppercase)
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
#include "DtBcDDRFRONT.h"
#include "DtBcDDRFRONT_RegAccess.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDDRFRONT implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_DDRFRONT_DEFAULT_PRECONDITIONS(pBc)      \
                                 DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcDDRFRONT))
// MACRO that checks the BC has been enabled, if NOT return DT_STATUS_NOT_ENABLED
#define BC_DDRFRONT_MUST_BE_ENABLED(pBc)    BC_MUST_BE_ENABLED_IMPL(DDRFRONT, pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcDDRFRONT_Init(DtBc*);
static DtStatus  DtBcDDRFRONT_OnCloseFile(DtBc*, const DtFileObject*);
static DtStatus  DtBcDDRFRONT_OnEnable(DtBc*, Bool  Enable);
static void  DtBcDDRFRONT_SetControlRegs(DtBcDDRFRONT*, Bool BlkEna);
static DtStatus  DtBcDDRFRONT_ReadAddress(DtBcDDRFRONT* pBc, UInt32 Address, Int size, 
                                                                           UInt32* pData);
static DtStatus  DtBcDDRFRONT_WriteAddress(DtBcDDRFRONT* pBc, UInt32 Address, Int size,
                                                                      const UInt32* Data);
static DtStatus  DtBcDDRFRONT_GetProperties(DtBcDDRFRONT* pBc, Int *pDataWidth, 
                                                                      Int64* pMemorySize);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDDRFRONT - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFRONT_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcDDRFRONT_Close(DtBc*  pBc)
{
    BC_DDRFRONT_DEFAULT_PRECONDITIONS(pBc);
    DT_ASSERT(!DtBc_IsEnabled(pBc));

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFRONT_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcDDRFRONT*  DtBcDDRFRONT_Open(Int  Address, DtCore* pCore, DtPt* pPt,
                               const char* pRole, Int Instance, Int Uuid, Bool CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;
    
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));
    
    // Init open parameters
    DT_BC_DDRFRONT_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcDDRFRONT, Id, DT_BLOCK_TYPE_DDRFRONT,
                                                         Address, pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcDDRFRONT_Close;
    OpenParams.m_InitFunc = DtBcDDRFRONT_Init;
    OpenParams.m_OnEnableFunc = DtBcDDRFRONT_OnEnable;
    OpenParams.m_OnCloseFileFunc = DtBcDDRFRONT_OnCloseFile;
    
    // Use base function to allocate and perform standard initialisation of block data
    return (DtBcDDRFRONT*)DtBc_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFRONT_GetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDDRFRONT_GetOperationalMode(DtBcDDRFRONT* pBc, Int* pOpMode)
{
    // Sanity check
    BC_DDRFRONT_DEFAULT_PRECONDITIONS(pBc);

    // Check parameter
    if (pOpMode == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_DDRFRONT_MUST_BE_ENABLED(pBc);

    // Return cached value
    *pOpMode = pBc->m_OperationalMode;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFRONT_SetOperationalMode -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcDDRFRONT_SetOperationalMode(DtBcDDRFRONT* pBc, Int OpMode)
{
    DtStatus Status = DT_STATUS_OK;

    // Sanity check
    BC_DDRFRONT_DEFAULT_PRECONDITIONS(pBc);

    // Check parameters
    if (OpMode!=DT_BLOCK_OPMODE_IDLE && OpMode!=DT_BLOCK_OPMODE_STANDBY 
                                                           && OpMode!=DT_BLOCK_OPMODE_RUN)
        return DT_STATUS_INVALID_PARAMETER;

    // Must be enabled
    BC_DDRFRONT_MUST_BE_ENABLED(pBc);
    
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

    // Firware does not have an operational mode. So only for local reference
    return Status;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcDDRFRONT - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFRONT_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcDDRFRONT_Init(DtBc* pBcBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcDDRFRONT* pBc = (DtBcDDRFRONT*)pBcBase;
    UInt32  RegData = 0;
    UInt32  MemSize = 0;

    // Set defaults
    pBc->m_BlockEnabled = FALSE;
    pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;

    RegData = DDRFRONT_Config_READ(pBc);
    pBc->m_WordSize = DDRFRONT_Config_GET_WordSize(RegData);
    MemSize = DDRFRONT_Config_GET_MemorySize(RegData);
    pBc->m_MemorySizeBytes = (((Int64)1<<MemSize)*pBc->m_WordSize)/8;

   
    // Set control register
    DtBcDDRFRONT_SetControlRegs(pBc, pBc->m_BlockEnabled);

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFRONT_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcDDRFRONT_OnEnable(DtBc* pBcBase, Bool Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcDDRFRONT* pBc = (DtBcDDRFRONT*)pBcBase;
    Int Timeout = 0;
    // Sanity check
    BC_DDRFRONT_DEFAULT_PRECONDITIONS(pBc);

    if (Enable)
    {
        // DISABLE -> ENABLE
        DtDbgOutBc(AVG, DDRFRONT, pBc, "OnEnable from disable -> enable");

        // Set defaults
        pBc->m_OperationalMode = DT_BLOCK_OPMODE_IDLE;
    }
    else
    {
        // ENABLE -> DISABLE
        DtDbgOutBc(AVG, DDRFRONT, pBc, "OnEnable from enable -> disable");

        // Operational mode to IDLE
        Status = DtBcDDRFRONT_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, DDRFRONT, pBc, "ERROR: SetOperationalMode failed");
            DT_ASSERT(FALSE);
        }
    }

    // Cache value
    pBc->m_BlockEnabled = Enable;
    // Set control register
    DtBcDDRFRONT_SetControlRegs(pBc, pBc->m_BlockEnabled);

    if (DDRFRONT_Status_READ_IsWriteBusy(pBc) != 0)
        DtDbgOutBc(ERR, DDRFRONT, pBc, "Write is still busy");
    if (DDRFRONT_Status_READ_IsReadBusy(pBc) != 0)
        DtDbgOutBc(ERR, DDRFRONT, pBc, "Read is still busy");
    
    Timeout = 0;
    while (DDRFRONT_Status_READ_DdrLocalCalSuccess(pBc)==0 && 
                                             DDRFRONT_Status_READ_DdrLocalCalFail(pBc)==0)
    {
        DtSleep(10);
        Timeout++;
        if (Timeout > 500)
            break;
    }
    if (DDRFRONT_Status_READ_DdrLocalCalFail(pBc) != 0)
        DtDbgOutBc(ERR, DDRFRONT, pBc, "DDR Calibration FAIL");
    if (DDRFRONT_Status_READ_DdrLocalCalSuccess(pBc) == 0)
    {
        DtDbgOutBc(ERR, DDRFRONT, pBc,
            "DDR Calibration not SUCCESS after wait period of 5 seconds");
        //Status = DT_STATUS_FAIL;
    }
    return Status;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFRONT_OnCloseFile -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcDDRFRONT_OnCloseFile(DtBc* pBcBase, const DtFileObject* pFile)
{
    DtStatus  Status = DT_STATUS_OK;
    DtBcDDRFRONT* pBc = (DtBcDDRFRONT*)pBcBase;
    DECL_EXCL_ACCESS_OBJECT_FILE(ExclAccessObj, pFile);

    BC_DDRFRONT_DEFAULT_PRECONDITIONS(pBc);

    // Check if the owner closed the file handle
    Status = DtBc_ExclAccessCheck((DtBc*)pBc, &ExclAccessObj);
    if (DT_SUCCESS(Status) && DtBc_IsEnabled((DtBc*)pBc))
    {
        DtDbgOutBc(AVG, DDRFRONT, pBc, "Go to IDLE");

        // Go to idle
        Status = DtBcDDRFRONT_SetOperationalMode(pBc, DT_BLOCK_OPMODE_IDLE);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutBc(ERR, DDRFRONT, pBc, "ERROR: failed to set operational mode");
        }
    }
    // Use base function to release exclusive access
    return DtBc_OnCloseFile(pBcBase, pFile);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFRONT_SetControlRegs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcDDRFRONT_SetControlRegs(DtBcDDRFRONT* pBc, Bool BlkEna)
{
    UInt32  RegData=0, FwBlkEna=0;

    // Convert block enable to BB-type
    FwBlkEna = BlkEna ? DDRFRONT_BLKENA_ENABLED : DDRFRONT_BLKENA_DISABLED;

    // Update control register
    RegData = DDRFRONT_Control_READ(pBc);
    RegData = DDRFRONT_Control_SET_BlockEnable(RegData, FwBlkEna);
    DDRFRONT_Control_WRITE(pBc, RegData);
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcBURSTFIFO_GetProperties -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtBcDDRFRONT_GetProperties(DtBcDDRFRONT* pBc, Int *pDataWidth, Int64* pMemorySize)
{
    *pDataWidth = pBc->m_WordSize;
    *pMemorySize = pBc->m_MemorySizeBytes;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFRONT_ReadAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtBcDDRFRONT_ReadAddress(DtBcDDRFRONT* pBc, UInt32 Address, Int Size32, 
                                                                            UInt32* pData)
{
    Int i = 0;
    Int j = 0;
    Int iBuf = 0;
    UInt  WaitTimeoutUs = 1000;
    Int Num32bWords = pBc->m_WordSize / 32; // i.e. 256/32 = 8 32-bit words
    Int NumDataWords = Size32 / Num32bWords; // i.e. number of 256-bit words


    // The size must be a multiple of memory words
    if (Size32%Num32bWords != 0)
        return DT_STATUS_INVALID_PARAMETER;

    for (i = 0; i < NumDataWords; i++)
    {
        DtDbgOutBc(AVG, DDRFRONT, pBc, "Read address: 0x%x", Address);
        DDRFRONT_ReadAddress_WRITE(pBc, Address++);
        while (DDRFRONT_Status_READ_IsReadBusy(pBc) != 0)
        {
            WaitTimeoutUs--;
            if (WaitTimeoutUs == 0)
            {
                Size32 = 0;
                return DT_STATUS_TIMEOUT;
            }
            DtWaitBlock(1);
        }

        // FIFO contains one word, read 32-bits for each one
        for (j = 0; j < Num32bWords; j++)
        {
            pData[iBuf++] = DDRFRONT_ReadData_READ(pBc);
            DtDbgOutBc(MAX, DDRFRONT, pBc, "Read value 0x%x", pData[iBuf-1]);
        }
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcDDRFRONT_WriteAddress -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcDDRFRONT_WriteAddress(DtBcDDRFRONT* pBc, UInt32 Address, Int Size32, 
                                                                      const UInt32* pData)
{
    Int i = 0;
    Int j = 0;
    Int iBuf = 0;
    UInt  WaitTimeoutUs = 1000;
    Int Num32bWords = pBc->m_WordSize / 32; // i.e. 256/32 = 8 32-bit words
    Int NumDataWords = Size32/ Num32bWords; // i.e. number of 256-bit words

    // Must be a multiple. i.e. for 256 bit:  Size32 must be a multiple of 8
    if (Size32 % Num32bWords != 0)
        return DT_STATUS_INVALID_PARAMETER;

    for (i = 0; i < NumDataWords; i++)
    {

        while (DDRFRONT_Status_READ_IsWriteBusy(pBc) != 0)
        {
            WaitTimeoutUs--;
            if (WaitTimeoutUs == 0)
                return DT_STATUS_TIMEOUT;
            DtWaitBlock(1);
        }

        // FIFO contains one word, write 32-bit values
        for (j = 0; j < Num32bWords; j++)
        {
            DtDbgOutBc(MAX, DDRFRONT, pBc, "Write value 0x%x", pData[iBuf]);
            DDRFRONT_WriteData_WRITE(pBc, pData[iBuf++]);
        }
        DtDbgOutBc(AVG, DDRFRONT, pBc, "Write address: 0x%x", Address);
        DDRFRONT_WriteAddress_WRITE(pBc, Address++);
    }
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDDRFRONT implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the Bc's stub
#define DDRFRONT_STUB_DEFAULT_PRECONDITIONS(pStub)      \
                       DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcDDRFRONT))
// Helper macro to cast stub's DtBc member to DtBcDDRFRONT
#define DDRFRONT_STUB  ((DtIoStubBcDDRFRONT*)pStub)
#define DDRFRONT_BC  ((DtBcDDRFRONT*)DDRFRONT_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcDDRFRONT_AppendDynamicSize(const DtIoStub*, DtIoStubIoParams*);
static DtStatus  DtIoStubBcDDRFRONT_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int*);
static DtStatus  DtIoStubBcDDRFRONT_OnCmdGetProperties(const DtIoStubBcDDRFRONT* pStub,
                                           DtIoctlDdrFrontCmdGetPropertiesOutput* pOutData);
static DtStatus  DtIoStubBcDDRFRONT_OnCmdReadAddress(const DtIoStubBcDDRFRONT* pStub,
                                          const DtIoctlDdrFrontCmdReadAddressInput* pInData,
                                           DtIoctlDdrFrontCmdReadAddressOutput* pOutData);
static DtStatus  DtIoStubBcDDRFRONT_OnCmdWriteAddress(const DtIoStubBcDDRFRONT* pStub,
                                        const DtIoctlDdrFrontCmdWriteAddressInput* pInData);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_DDRFRONT;

static const DtIoctlProperties  IOSTUB_BC_DDRFRONT_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_DDRFRONT_CMD(
        DtIoStubBcDDRFRONT_OnCmd,
        DtIoStubBcDDRFRONT_AppendDynamicSize,
        NULL),
};

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDDRFRONT - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDDRFRONT_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcDDRFRONT_Close(DtIoStub* pStub)
{
    DDRFRONT_STUB_DEFAULT_PRECONDITIONS(pStub);

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDDRFRONT_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcDDRFRONT*  DtIoStubBcDDRFRONT_Open(DtBc*  pBc)
{
    DtIoStubBcOpenParams  OpenParams;

    BC_DDRFRONT_DEFAULT_PRECONDITIONS(pBc);

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcDDRFRONT, pBc, NULL, 
                                                            DtIoStubBcDDRFRONT_Close,
                                                            NULL,  // Use default IOCTL
                                                            IOSTUB_BC_DDRFRONT_IOCTLS);
    return (DtIoStubBcDDRFRONT*)DtIoStubBc_Open(&OpenParams);
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcDDRFRONT - Private functions +=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDDRFRONT_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcDDRFRONT_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlDdrFrontCmdInput*  pInData = NULL;

    DDRFRONT_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_DDRFRONT_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_DdrFrontCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_DDRFRONT_CMD_READ_ADDRESS:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlDdrFrontCmdReadAddressInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlDdrFrontCmdReadAddressOutput));
        // Add dynamic size (i.e. #bytes to read)
        pIoParams->m_OutReqSize += pInData->m_ReadAddress.m_Num32bItemsToRead;
        break;

    case DT_DDRFRONT_CMD_WRITE_ADDRESS:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlDdrFrontCmdWriteAddressInput));
        // Add dynamic size (i.e. #bytes to write)
        pIoParams->m_InReqSize += pInData->m_WriteAddress.m_Num32bItemsToWrite;
        break;

    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDDRFRONT_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcDDRFRONT_OnCmd(const DtIoStub*  pStub, 
                                             DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlDdrFrontCmdInput*  pInData = NULL;
    DtIoctlDdrFrontCmdOutput*  pOutData = NULL;

    DDRFRONT_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_DDRFRONT_CMD);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc, 
                                                             &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, DDRFRONT, pStub,"ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_DdrFrontCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_DdrFrontCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_DDRFRONT_CMD_READ_ADDRESS:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcDDRFRONT_OnCmdReadAddress(DDRFRONT_STUB, &pInData->m_ReadAddress,
                                                                &pOutData->m_ReadAddress);
        break;
    case DT_DDRFRONT_CMD_WRITE_ADDRESS:
        DT_ASSERT(pInData != NULL);
        Status = DtIoStubBcDDRFRONT_OnCmdWriteAddress(DDRFRONT_STUB, 
                                                                &pInData->m_WriteAddress);
        break;
    case DT_DDRFRONT_CMD_GET_PROPERTIES:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcDDRFRONT_OnCmdGetProperties(DDRFRONT_STUB, 
                                                              &pOutData->m_GetProperties);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}


// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDDRFRONT_OnCmdGetProperties -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtIoStubBcDDRFRONT_OnCmdGetProperties(
    const DtIoStubBcDDRFRONT* pStub,
    DtIoctlDdrFrontCmdGetPropertiesOutput* pOutData)
{
    DDRFRONT_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pOutData != NULL);

    return DtBcDDRFRONT_GetProperties(DDRFRONT_BC, &pOutData->m_DataWidth,
                                                                 &pOutData->m_MemorySize);
}


// .-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDDRFRONT_OnCmdReadAddress -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcDDRFRONT_OnCmdReadAddress(
    const DtIoStubBcDDRFRONT* pStub,
    const DtIoctlDdrFrontCmdReadAddressInput* pInData,
    DtIoctlDdrFrontCmdReadAddressOutput* pOutData)
{
    DtStatus Status = DT_STATUS_OK;
    DDRFRONT_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    DT_ASSERT(pOutData != NULL);
    Status = DtBcDDRFRONT_ReadAddress(DDRFRONT_BC, pInData->m_Address, 
                                           pInData->m_Num32bItemsToRead, pOutData->m_Buf);
    pOutData->m_Num32bItemsRead = pInData->m_Num32bItemsToRead;

    if (Status != DT_STATUS_OK)
        pOutData->m_Num32bItemsRead = 0;
    return Status;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcDDRFRONT_OnCmdWriteAddress -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcDDRFRONT_OnCmdWriteAddress(
    const DtIoStubBcDDRFRONT* pStub,
    const DtIoctlDdrFrontCmdWriteAddressInput* pInData)
{
    DDRFRONT_STUB_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);


    return DtBcDDRFRONT_WriteAddress(DDRFRONT_BC, pInData->m_Address, 
                                           pInData->m_Num32bItemsToWrite, pInData->m_Buf);
}

