// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfMbcTemp.c *#*#*#*#*#*#*#*#*#*# (C) 2024 DekTec
//
//
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2024 DekTec Digital Video B.V.
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
#include "DtDfMbcTemp.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// FMBC Master role
#define FMBC_ROLE_NONE        NULL


// Poll loop time
#define MBCTEMP_POLL_LOOP_TIME      100

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMbcTemp implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtDfMbcTemp function
#define DF_MBCTEMP_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfMbcTemp))


// Helper macro to cast a DtDf* to a DtDfMbcTemp*
#define DF_MBCTEMP      ((DtDfMbcTemp*)pDf)

static DtStatus  DtDfMbcTemp_Init(DtDf*);
static DtStatus  DtDfMbcTemp_LoadParameters(DtDf*);
static DtStatus  DtDfMbcTemp_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfMbcTemp_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfMbcTemp_OpenChildren(DtDfMbcTemp*);
static DtStatus  DtDfMbcTemp_ReadTemperature(DtDfMbcTemp * pDf, Int * pTemperature);
static UInt32 DtDfMbcTemp_CountNumOnes(UInt32 Word);
static void  DtDfMbcTemp_ReadTempThread(DtThread * pThread, void * pContext);


// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMbcTemp - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcTemp_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfMbcTemp_Close(DtDf*  pDf)
{
    DF_MBCTEMP_DEFAULT_PRECONDITIONS(pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcTemp_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfMbcTemp*  DtDfMbcTemp_Open(DtCore*  pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    // No stub available
    
    // Init open parameters
    DT_DF_MBCTEMP_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfMbcTemp, Id, DT_FUNC_TYPE_MBCTEMP, pPt,
                                                                            FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfMbcTemp_Close;
    OpenParams.m_InitFunc = DtDfMbcTemp_Init;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfMbcTemp_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfMbcTemp_OnEnablePreChildren;
    OpenParams.m_LoadParsFunc = DtDfMbcTemp_LoadParameters;

    // Use base function to allocate and perform standard initialization of function data
    return (DtDfMbcTemp*)DtDf_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcTemp_GetTemperature -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfMbcTemp_GetTemperature(DtDfMbcTemp*  pDf, Int* pTemperature)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_MBCTEMP_DEFAULT_PRECONDITIONS(pDf);

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

// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMbcTemp - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcTemp_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfMbcTemp_Init(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMbcTemp*  pDf = (DtDfMbcTemp*)pDfBase;

    // Sanity checks
    DF_MBCTEMP_DEFAULT_PRECONDITIONS(pDf);

    // Initialize the spin lock
    DtSpinLockInit(&pDf->m_SpinLock);

    // Init the temperature read thread
    DT_RETURN_ON_ERROR(DtThreadInit(&pDf->m_TempReadThread, 
                                                        DtDfMbcTemp_ReadTempThread, pDf));
    DtEventInit(&pDf->m_TempReadStopEvent, FALSE);

    // Init the temperature status
    pDf->m_TempReadStatus = DT_STATUS_NOT_ENABLED;
    pDf->m_Temperature = 0;
    // m_TempSensorLocations Bits[15:0]: Specifies the sensors to read.
    pDf->m_NumTempSensors = DtDfMbcTemp_CountNumOnes(pDf->m_TempSensorLocations & 0xFFFF);

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfMbcTemp_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, MBCTEMP, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcTemp_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfMbcTemp_LoadParameters(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMbcTemp* pDf = (DtDfMbcTemp*)pDfBase;

    // List of Template function parameters
    DtDfParameters  DFMBCTEMP_PARS[] =
    {
        // Name,  Value Type,  Value*
        { "LOCATIONS", PROPERTY_VALUE_TYPE_UINT32, &(pDf->m_TempSensorLocations) },
    };

    // Sanity checks
    DF_MBCTEMP_DEFAULT_PRECONDITIONS(pDf);

    // Set defaults
    pDf->m_TempSensorLocations = 0;

    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFMBCTEMP_PARS),
                                                                          DFMBCTEMP_PARS);
    if (!DT_SUCCESS(Status))
        return Status;

    return DT_STATUS_OK;
}
 
// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcTemp_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfMbcTemp_OnEnablePostChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMbcTemp* pDf = (DtDfMbcTemp*)pDfBase;

    // Sanity checks
    DF_MBCTEMP_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        DtStatus TempReadStatus;
        Int Temperature; 

        // Update temperature and status
        TempReadStatus = DtDfMbcTemp_ReadTemperature(pDf, &Temperature);
        DtSpinLockAcquire(&pDf->m_SpinLock);
        pDf->m_TempReadStatus = TempReadStatus;
        pDf->m_Temperature = Temperature;
        DtSpinLockRelease(&pDf->m_SpinLock);

        // Start the temperature read thread
        DtEventReset(&pDf->m_TempReadStopEvent);
        Status = DtThreadStart(&pDf->m_TempReadThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, MBCTEMP, pDf, "ERROR: failed to start TempReadThread");
            return Status;
        }
    }
    return Status;
}
// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcTemp_OnEnablePreChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfMbcTemp_OnEnablePreChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMbcTemp* pDf = (DtDfMbcTemp*)pDfBase;

    // Sanity checks
    DF_MBCTEMP_DEFAULT_PRECONDITIONS(pDf);

    if (!Enable)
    {
        // Stop temperature read thread
        DtEventSet(&pDf->m_TempReadStopEvent);
        Status = DtThreadStop(&pDf->m_TempReadThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, MBCTEMP, pDf, "ERROR: failed to stop TempReadThread");
            return Status;
        }
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcTemp_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfMbcTemp_OpenChildren(DtDfMbcTemp*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

     // List of children supported by the the MBCTEMP function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_FMBC, DT_BC_FMBC_NAME, 
                               FMBC_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcFmbc), TRUE}
    };

    DF_MBCTEMP_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
     if (!DT_SUCCESS(Status))
        return Status;

    // Check mandatory children have been loaded (i.e. shortcut is valid)
    DT_ASSERT(pDf->m_pBcFmbc != NULL);
    return DT_STATUS_OK;
}


// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcTemp_ReadTemperature -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfMbcTemp_ReadTemperature(DtDfMbcTemp* pDf, Int* pTemperature)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt32  WrData[1];
    UInt32  RdData[16];
    UInt32  i = 0;
    Int MaxTemp = 0;
    
    // Get temperature
    WrData[0] = pDf->m_TempSensorLocations;
    Status = DtBcFMBC_WriteRead(pDf->m_pBcFmbc, DT_BC_FMBC_CMD_GET_TEMPERATURE,
                                           1, WrData, pDf->m_NumTempSensors, RdData, 150);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, MBCTEMP, pDf, "ERROR: failed to read temperature");
        return Status;
    }
    
    for (i=0; i<pDf->m_NumTempSensors; i++)
    {
        // Compute the temperature in degrees Celsius. Save the maximum temperature.
        Int Temp = ((Int)RdData[i] + 128)/256;
        if (i==0 || Temp>MaxTemp)
            MaxTemp = Temp;
        DtDbgOutDf(MIN, MBCTEMP, pDf, "Temperature[%d] : %d degrees", i, Temp);
    }

    *pTemperature = MaxTemp;
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcTemp_CountNumOnes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Function to count the number of 1s in a 32-bit word
//
UInt32 DtDfMbcTemp_CountNumOnes(UInt32 Word)
{
    Word = Word - ((Word >> 1) & 0x55555555);
    Word = (Word & 0x33333333) + ((Word >> 2) & 0x33333333);
    Word = (Word + (Word >> 4)) & 0x0F0F0F0F;
    Word = Word + (Word >> 8);
    Word = Word + (Word >> 16);
    return Word & 0x0000003F;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcTemp_ReadTempThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
static void  DtDfMbcTemp_ReadTempThread(DtThread* pThread, void* pContext)
{
    DtDfMbcTemp*  pDf = (DtDfMbcTemp*)pContext;
    Bool StopThread = FALSE;

    while (!StopThread)
    {
        DtStatus Status, TempReadStatus;
        Int Temperature; 

        // Wait for stop or poll-loop timeout
        Status = DtEventWaitUnInt(&pDf->m_TempReadStopEvent, MBCTEMP_POLL_LOOP_TIME);
        if (Status==DT_STATUS_OK || pThread->m_StopThread)
        { 
            StopThread = TRUE;
            continue;
        }

        // Update temperature and status
        TempReadStatus = DtDfMbcTemp_ReadTemperature(pDf, &Temperature);
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
