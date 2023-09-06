// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfMcTc72Temp.c *#*#*#*#*#*#*#*#*#* (C) 2022 DekTec
//
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
#include "DtDfMcTc72Temp.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// SPI Master role
#define SPIM_ROLE_NONE        NULL

// Microchip TC72 registers
#define MCTC72TEMP_REG_CONFIGURATION    0
#define MCTC72TEMP_REG_TEMPERATURE_LSB  1
#define MCTC72TEMP_REG_TEMPERATURE_MSB  2
#define MCTC72TEMP_REG_MANUFACTURER_ID  3

// Poll loop time
#define MCTC72TEMP_POLL_LOOP_TIME      100

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMcTc72Temp implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtDfMcTc72Temp function
#define DF_MCTC72TEMP_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfMcTc72Temp))


// Helper macro to cast a DtDf* to a DtDfMcTc72Temp*
#define DF_MCTC72TEMP      ((DtDfMcTc72Temp*)pDf)

static DtStatus  DtDfMcTc72Temp_Init(DtDf*);
static DtStatus  DtDfMcTc72Temp_LoadParameters(DtDf*);
static DtStatus  DtDfMcTc72Temp_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfMcTc72Temp_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfMcTc72Temp_OpenChildren(DtDfMcTc72Temp*);
static DtStatus  DtDfMcTc72Temp_ConfigureTc72(DtDfMcTc72Temp * pDf);
static DtStatus  DtDfMcTc72Temp_ReadTemperature(DtDfMcTc72Temp * pDf, Int * pTemperature);
static void  DtDfMcTc72Temp_ReadTempThread(DtThread * pThread, void * pContext);


//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMcTc72Temp - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMcTc72Temp_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfMcTc72Temp_Close(DtDf*  pDf)
{
    DF_MCTC72TEMP_DEFAULT_PRECONDITIONS(pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMcTc72Temp_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfMcTc72Temp*  DtDfMcTc72Temp_Open(DtCore*  pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    // No stub available
    
    // Init open parameters
    DT_DF_MCTC72TEMP_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfMcTc72Temp, Id, DT_FUNC_TYPE_MCTC72TEMP, pPt,
                                                                            FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfMcTc72Temp_Close;
    OpenParams.m_InitFunc = DtDfMcTc72Temp_Init;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfMcTc72Temp_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfMcTc72Temp_OnEnablePreChildren;
    OpenParams.m_LoadParsFunc = DtDfMcTc72Temp_LoadParameters;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfMcTc72Temp*)DtDf_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMcTc72Temp_GetTemperature -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfMcTc72Temp_GetTemperature(DtDfMcTc72Temp*  pDf, Int* pTemperature)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_MCTC72TEMP_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (pTemperature == NULL)
        return DT_STATUS_INVALID_PARAMETER;

    // Return last known temperature
    DtSpinLockAcquire(&pDf->m_SpinLock);
    Status= pDf->m_TempReadStatus;
    *pTemperature = pDf->m_Temperature;
    DtSpinLockRelease(&pDf->m_SpinLock);

    return Status;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMcTc72Temp - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMcTc72Temp_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfMcTc72Temp_Init(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMcTc72Temp*  pDf = (DtDfMcTc72Temp*)pDfBase;

    // Sanity checks
    DF_MCTC72TEMP_DEFAULT_PRECONDITIONS(pDf);

    // Initialize the spin lock
    DtSpinLockInit(&pDf->m_SpinLock);

    // Init the temperature read thread
    DT_RETURN_ON_ERROR(DtThreadInit(&pDf->m_TempReadThread, 
                                                     DtDfMcTc72Temp_ReadTempThread, pDf));
    DtEventInit(&pDf->m_TempReadStopEvent, FALSE);

    // Init the temperature status
    pDf->m_TempReadStatus = DT_STATUS_NOT_ENABLED;
    pDf->m_Temperature = 0;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfMcTc72Temp_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, MCTC72TEMP, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMcTc72Temp_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfMcTc72Temp_LoadParameters(DtDf*  pDfBase)
{
    return DT_STATUS_OK;
}
 
//-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMcTc72Temp_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfMcTc72Temp_OnEnablePostChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMcTc72Temp* pDf = (DtDfMcTc72Temp*)pDfBase;

    // Sanity checks
    DF_MCTC72TEMP_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        DtStatus TempReadStatus;
        Int Temperature; 
        // Configure the Maxim TC72
        DT_RETURN_ON_ERROR(DtDfMcTc72Temp_ConfigureTc72(pDf));

        // Update temperature and status
        TempReadStatus = DtDfMcTc72Temp_ReadTemperature(pDf, &Temperature);
        DtSpinLockAcquire(&pDf->m_SpinLock);
        pDf->m_TempReadStatus = TempReadStatus;
        pDf->m_Temperature = Temperature;
        DtSpinLockRelease(&pDf->m_SpinLock);

        // Start the temperature read thread
        DtEventReset(&pDf->m_TempReadStopEvent);
        Status = DtThreadStart(&pDf->m_TempReadThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, MCTC72TEMP, pDf, "ERROR: failed to start TempReadThread");
            return Status;
        }

    }
    return Status;
}
//-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMcTc72Temp_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfMcTc72Temp_OnEnablePreChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMcTc72Temp* pDf = (DtDfMcTc72Temp*)pDfBase;

    // Sanity checks
    DF_MCTC72TEMP_DEFAULT_PRECONDITIONS(pDf);

    if (!Enable)
    {
        // Stop temperature read thread
        DtEventSet(&pDf->m_TempReadStopEvent);
        Status = DtThreadStop(&pDf->m_TempReadThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, MCTC72TEMP, pDf, "ERROR: failed to stop TempReadThread");
            return Status;
        }
    }
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMcTc72Temp_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfMcTc72Temp_OpenChildren(DtDfMcTc72Temp*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

     // List of children supported by the the MCTC72TEMP function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_SPIM, DT_BC_SPIM_NAME, 
                               SPIM_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcSpim), TRUE}
    };

    DF_MCTC72TEMP_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
     if (!DT_SUCCESS(Status))
        return Status;

    // Check mandatory children have been loaded (i.e. shortcut is valid)
    DT_ASSERT(pDf->m_pBcSpim != NULL);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMcTc72Temp_ConfigureTc72 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfMcTc72Temp_ConfigureTc72(DtDfMcTc72Temp*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8  WrBuffer[2];
    UInt8  RdBuffer[1];

    // Configure the TC72
    // Bit 7-5  Reserved = 000
    // Bit 4:   0  Continuous temperature conversion 
    //          1  if shutdown==1 one-shot else continuous temperature conversion
    // Bit 3-1: Reserved = 010
    // Bit 0    Shutdown; 0 : active; 1 : shutdown
    WrBuffer[0] = MCTC72TEMP_REG_CONFIGURATION | 0x80;
    WrBuffer[1] = 0x04; 
    Status = DtBcSPIM_Write(pDf->m_pBcSpim, 2, WrBuffer);
    if (!DT_SUCCESS(Status)) 
    { 
        DtDbgOutDf(ERR, MCTC72TEMP, pDf, "ERROR: failed to configure TC72");
        return Status;
    }
    // Read manufacturer ID
    WrBuffer[0] = MCTC72TEMP_REG_MANUFACTURER_ID;
    Status = DtBcSPIM_WriteRead(pDf->m_pBcSpim, 1, WrBuffer, 1, RdBuffer);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, MCTC72TEMP, pDf, "ERROR: failed to read manufacturer ID");
        return Status;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMcTc72Temp_ReadTemperature -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfMcTc72Temp_ReadTemperature(DtDfMcTc72Temp*  pDf, Int* pTemperature)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8  WrBuffer[1];
    UInt8  RdBuffer[1];

    // Get temperature
    WrBuffer[0] = MCTC72TEMP_REG_TEMPERATURE_MSB;
    Status = DtBcSPIM_WriteRead(pDf->m_pBcSpim, 1, WrBuffer, 1, RdBuffer);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, MCTC72TEMP, pDf, "ERROR: failed to read temperature");
        return Status;
    }

    // Convert to integer degrees Celsius
    // Check sign bit
    if ((RdBuffer[0] & 0x80) != 0)
         *pTemperature = ~0xFF | RdBuffer[0];
    else
         *pTemperature = RdBuffer[0];
    return Status;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMcTc72Temp_ReadTempThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtDfMcTc72Temp_ReadTempThread(DtThread* pThread, void* pContext)
{
    DtDfMcTc72Temp*  pDf = (DtDfMcTc72Temp*)pContext;
    Bool StopThread = FALSE;

    while (!StopThread)
    {
        DtStatus Status, TempReadStatus;
        Int Temperature; 

        // Wait for stop or poll-loop timeout
        Status = DtEventWaitUnInt(&pDf->m_TempReadStopEvent, MCTC72TEMP_POLL_LOOP_TIME);
        if (Status==DT_STATUS_OK || pThread->m_StopThread)
        { 
            StopThread = TRUE;
            continue;
        }

        // Update temperature and status
        TempReadStatus = DtDfMcTc72Temp_ReadTemperature(pDf, &Temperature);
        DtSpinLockAcquire(&pDf->m_SpinLock);
        pDf->m_TempReadStatus = TempReadStatus;
        pDf->m_Temperature = Temperature;
        DtSpinLockRelease(&pDf->m_SpinLock);
    }
    // Temperature is not updated anymore
    pDf->m_TempReadStatus = DT_STATUS_NOT_ENABLED;
    
    // We have to wait until the thread received a stop command.
    DtThreadWaitForStop(pThread);
}
