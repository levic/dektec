//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfMxDs75Temp.c *#*#*#*#*#*#*# (C) 2017-2018 DekTec
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
#include "DtDfMxDs75Temp.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// I2C Master role
#define I2CM_ROLE_NONE        NULL

// MAXIM DS75 registers
#define MXDS75TEMP_REG_TEMPERATURE      0
#define MXDS75TEMP_REG_CONFIGURATION    1

// Poll loop time
#define MXDS75TEMP_POLL_LOOP_TIME      100

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMxDs75Temp implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DtDfMxDs75Temp function
#define DF_MXDS75TEMP_DEFAULT_PRECONDITIONS(pDf)      \
    DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfMxDs75Temp))


// Helper macro to cast a DtDf* to a DtDfMxDs75Temp*
#define DF_MXDS75TEMP      ((DtDfMxDs75Temp*)pDf)


static DtStatus  DtDfMxDs75Temp_Init(DtDf*);
static DtStatus  DtDfMxDs75Temp_LoadParameters(DtDf*);
static DtStatus  DtDfMxDs75Temp_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfMxDs75Temp_OnEnablePreChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfMxDs75Temp_OpenChildren(DtDfMxDs75Temp*);
static DtStatus  DtDfMxDs75Temp_ConfigureDs75(DtDfMxDs75Temp * pDf);
static DtStatus  DtDfMxDs75Temp_ReadTemperature(DtDfMxDs75Temp * pDf, Int * pTemperature);
static void  DtDfMxDs75Temp_ReadTempThread(DtThread * pThread, void * pContext);


//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMxDs75Temp - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMxDs75Temp_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtDfMxDs75Temp_Close(DtDf*  pDf)
{
    DF_MXDS75TEMP_DEFAULT_PRECONDITIONS(pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMxDs75Temp_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtDfMxDs75Temp*  DtDfMxDs75Temp_Open(DtCore*  pCore, DtPt*  pPt, 
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    // No stub available
    
    // Init open parameters
    DT_DF_MXDS75TEMP_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfMxDs75Temp, Id, DT_FUNC_TYPE_MXDS75TEMP, pPt,
                                                                            FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfMxDs75Temp_Close;
    OpenParams.m_InitFunc = DtDfMxDs75Temp_Init;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfMxDs75Temp_OnEnablePostChildren;
    OpenParams.m_OnEnablePreChildrenFunc = DtDfMxDs75Temp_OnEnablePreChildren;
    OpenParams.m_LoadParsFunc = DtDfMxDs75Temp_LoadParameters;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfMxDs75Temp*)DtDf_Open(&OpenParams);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMxDs75Temp_GetTemperature -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfMxDs75Temp_GetTemperature(DtDfMxDs75Temp*  pDf, Int* pTemperature)
{
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_MXDS75TEMP_DEFAULT_PRECONDITIONS(pDf);

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

//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMxDs75Temp - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMxDs75Temp_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfMxDs75Temp_Init(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMxDs75Temp*  pDf = (DtDfMxDs75Temp*)pDfBase;

    // Sanity checks
    DF_MXDS75TEMP_DEFAULT_PRECONDITIONS(pDf);

    // Check paramaters have been loaded succesfully
    DT_ASSERT(pDf->m_MxDs75Address >= 0);

    // Initialize the spin lock
    DtSpinLockInit(&pDf->m_SpinLock);

    // Init the temperature read thread
    DT_RETURN_ON_ERROR(DtThreadInit(&pDf->m_TempReadThread, 
                                                     DtDfMxDs75Temp_ReadTempThread, pDf));
    DtEventInit(&pDf->m_TempReadStopEvent, FALSE);

    // Init the temperature status
    pDf->m_TempReadStatus = DT_STATUS_NOT_ENABLED;
    pDf->m_Temperature = 0;

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfMxDs75Temp_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, MXDS75TEMP, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMxDs75Temp_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfMxDs75Temp_LoadParameters(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMxDs75Temp* pDf = (DtDfMxDs75Temp*)pDfBase;

    // List of MAXIM DS75  function parameters
    DtDfParameters  DFMXDS75TEMP_PARS[] =
    {
        // Name,  Value Type,  Value*
        { "I2C_ADDR", PROPERTY_VALUE_TYPE_INT, &(pDf->m_MxDs75Address) },
    };

    // Sanity checks
    DF_MXDS75TEMP_DEFAULT_PRECONDITIONS(pDf);

 
    // Init parameters to their defaults
    pDf->m_MxDs75Address = -1;

    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFMXDS75TEMP_PARS),
                                                                       DFMXDS75TEMP_PARS);
    if (!DT_SUCCESS(Status))
        return Status;
 
    // Check paramaters have been loaded succesfully
    DT_ASSERT(pDf->m_MxDs75Address >= 0);
    return DT_STATUS_OK;
}
 
//-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMxDs75Temp_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfMxDs75Temp_OnEnablePostChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMxDs75Temp* pDf = (DtDfMxDs75Temp*)pDfBase;

    // Sanity checks
    DF_MXDS75TEMP_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        DtStatus TempReadStatus;
        Int Temperature; 
        // Configure the Maxim DS75
        DT_RETURN_ON_ERROR(DtDfMxDs75Temp_ConfigureDs75(pDf));

        // Update temperature and status
        TempReadStatus = DtDfMxDs75Temp_ReadTemperature(pDf, &Temperature);
        DtSpinLockAcquire(&pDf->m_SpinLock);
        pDf->m_TempReadStatus = TempReadStatus;
        pDf->m_Temperature = Temperature;
        DtSpinLockRelease(&pDf->m_SpinLock);

        // Start the temperature read thread
        DtEventReset(&pDf->m_TempReadStopEvent);
        Status = DtThreadStart(&pDf->m_TempReadThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, MXDS75TEMP, pDf, "ERROR: failed to start TempReadThread");
            return Status;
        }

    }
    return Status;
}
//-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMxDs75Temp_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfMxDs75Temp_OnEnablePreChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfMxDs75Temp* pDf = (DtDfMxDs75Temp*)pDfBase;

    // Sanity checks
    DF_MXDS75TEMP_DEFAULT_PRECONDITIONS(pDf);

    if (!Enable)
    {
        // Stop temperature read thread
        DtEventSet(&pDf->m_TempReadStopEvent);
        Status = DtThreadStop(&pDf->m_TempReadThread);
        if (!DT_SUCCESS(Status))
        {
            DtDbgOutDf(ERR, MXDS75TEMP, pDf, "ERROR: failed to stop TempReadThread");
            return Status;
        }
    }
    return Status;
}


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMxDs75Temp_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfMxDs75Temp_OpenChildren(DtDfMxDs75Temp*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

     // List of children supported by the the MXDS75TEMP function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_I2CM, DT_BC_I2CM_NAME, 
                               I2CM_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcI2Cm), TRUE}
    };

    DF_MXDS75TEMP_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
     if (!DT_SUCCESS(Status))
        return Status;

    // Check mandatory children have been loaded (i.e. shortcut is valid)
    DT_ASSERT(pDf->m_pBcI2Cm != NULL);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMxDs75Temp_ConfigureDs75 -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfMxDs75Temp_ConfigureDs75(DtDfMxDs75Temp*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;
    // Configure the MAXIM DS75
    // Configuration register
    // Bit 7:  Reserved = 0
    // Bit 6,5 Conversion Resolution: 
    //         00   9 bit / 150ms
    //         01  10 bit / 300ms
    //         10  11 bit / 600ms
    //         11  12 bit / 1200ms
    // Bit 4,3 Thermostat fault tolerance; #out-of-limits to trigger output
    //          00 : 1; 01 : 2; 10 : 4; 11 : 6
    // Bit 2   Thermostat Output Polarity;  0 : active low; 1 : active high
    // Bit 1   Thermostat Operating Mode; 0 : comparitor mode ; 1 : interrupt mode
    // Bit 0   Shutdown; 0 : active; 1 : shutdown
    UInt8  Buffer[2];
    Buffer[0] = MXDS75TEMP_REG_CONFIGURATION;
    Buffer[1] = 0x00;   // Using 9bit resolution 
    Status = DtBcI2CM_Write(pDf->m_pBcI2Cm, pDf->m_MxDs75Address, sizeof(Buffer), Buffer);
    if (!DT_SUCCESS(Status))
        DtDbgOutDf(ERR, MXDS75TEMP, pDf, "ERROR: failed to configure DS75");

    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMxDs75Temp_ReadTemperature -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfMxDs75Temp_ReadTemperature(DtDfMxDs75Temp*  pDf, Int* pTemperature)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8  WrBuffer[1];
    UInt8  RdBuffer[2];

    // Get temperature
    WrBuffer[0] = MXDS75TEMP_REG_TEMPERATURE;
    Status = DtBcI2CM_WriteRead(pDf->m_pBcI2Cm, pDf->m_MxDs75Address,
                                  sizeof(WrBuffer), WrBuffer, sizeof(RdBuffer), RdBuffer);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, MXDS75TEMP, pDf, "ERROR: failed to read temperature");
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


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMxDs75Temp_ReadTempThread -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
static void  DtDfMxDs75Temp_ReadTempThread(DtThread* pThread, void* pContext)
{
    DtDfMxDs75Temp*  pDf = (DtDfMxDs75Temp*)pContext;
    Bool StopThread = FALSE;

    while (!StopThread)
    {
        DtStatus Status, TempReadStatus;
        Int Temperature; 

        // Wait for stop or poll-loop timeout
        Status = DtEventWaitUnInt(&pDf->m_TempReadStopEvent, MXDS75TEMP_POLL_LOOP_TIME);
        if (Status==DT_STATUS_OK || pThread->m_StopThread)
        { 
            StopThread = TRUE;
            continue;
        }

        // Update temperature and status
        TempReadStatus = DtDfMxDs75Temp_ReadTemperature(pDf, &Temperature);
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
