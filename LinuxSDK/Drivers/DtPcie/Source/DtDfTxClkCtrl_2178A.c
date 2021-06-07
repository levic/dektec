// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfTxClkCtrl_2178A.c *#*#*#*#*#*#*#*# (C) 2021 DekTec
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
#include "DtDfTxClkCtrl_2178A.h"
#include "DtDfSi534X.h"
#include "DtDfSi534X_ConfigData.h"

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// I2C Master role
#define I2CM_ROLE_NONE        NULL
#define TXPLLMGR_ROLE_NONE    NULL

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfTxClkCtrl_2178A implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// MACRO with default precondition checks for the DfAsiTx function
#define DF_TXCLKCTRL_2178A_DEFAULT_PRECONDITIONS(pDf)      \
                          DT_ASSERT(pDf!=NULL && pDf->m_Size==sizeof(DtDfTxClkCtrl_2178A))

// MACRO that checks the function has been enbled, if NOT return DT_STATUS_NOT_ENABLED
#define DF_TXCLKCTRL_2178A_MUST_BE_ENABLED(pDf)             \
                                             DF_MUST_BE_ENABLED_IMPL(TXCLKCTRL_2178A, pDf)
static DtStatus  DtDfTxClkCtrl_2178A_Init(DtDf*);
static DtStatus  DtDfTxClkCtrl_2178A_LoadParameters(DtDf*);
static DtStatus  DtDfTxClkCtrl_2178A_OnEnablePostChildren(DtDf*, Bool  Enable);
static DtStatus  DtDfTxClkCtrl_2178A_OpenChildren(DtDfTxClkCtrl_2178A*);
static DtStatus  DtDfTxClkCtrl_2178A_GetStatus(DtDfTxClkCtrl_2178A*, Bool* pLocked,
                                                                        Bool* pCalibDone);
static DtStatus  DtDfTxClkCtrl_2178A_SetSi534XConfig(DtDfTxClkCtrl_2178A*);
static DtStatus  DtDfTxClkCtrl_2178A_ReadRegister(DtDfTxClkCtrl_2178A*, 
                                                        DtDfSi534XRegister* pConfigItem);
static DtStatus  DtDfTxClkCtrl_2178A_WriteConfig(DtDfTxClkCtrl_2178A*,
                                                const DtDfSi534XRegister*, Int  NumItems);
static DtStatus  DtDfTxClkCtrl_2178A_WriteRegister(DtDfTxClkCtrl_2178A*,
                                                               const DtDfSi534XRegister*);

// +=+=+=+=+=+=+=+=+=+=+=+ DtDfTxClkCtrl_2178A - Public functions +=+=+=+=+=+=+=+=+=+=+=+=

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxClkCtrl_2178A_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtDfTxClkCtrl_2178A_Close(DtDf*  pDf)
{
    DF_TXCLKCTRL_2178A_DEFAULT_PRECONDITIONS(pDf);

    // Let base function perform final clean-up
    DtDf_Close(pDf);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxClkCtrl_2178A_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtDfTxClkCtrl_2178A*  DtDfTxClkCtrl_2178A_Open(DtCore*  pCore, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid)
{
    DtDfId  Id;
    DtDfOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    // No stub available
    // Init open parameters
    DT_DF_TXCLKCTRL_2178A_INIT_ID(Id, pRole, Instance, Uuid);
    DT_DF_INIT_OPEN_PARAMS(OpenParams, DtDfTxClkCtrl_2178A, Id, 
                                          DT_FUNC_TYPE_TXCLKCTRL_2178, pPt, FALSE, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtDfTxClkCtrl_2178A_Close;
    OpenParams.m_InitFunc = DtDfTxClkCtrl_2178A_Init;
    OpenParams.m_OnEnablePostChildrenFunc = DtDfTxClkCtrl_2178A_OnEnablePostChildren;
    OpenParams.m_LoadParsFunc = DtDfTxClkCtrl_2178A_LoadParameters;

    // Use base function to allocate and perform standard initialisation of function data
    return (DtDfTxClkCtrl_2178A*)DtDf_Open(&OpenParams);
}


// +=+=+=+=+=+=+=+=+=+=+=+ DtDfTxClkCtrl_2178A - Private functions +=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxClkCtrl_2178A_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfTxClkCtrl_2178A_Init(DtDf*  pDfBase)
{
    DtDfTxClkCtrl_2178A*  pDf = (DtDfTxClkCtrl_2178A*)pDfBase;
    DtStatus  Status = DT_STATUS_OK;

    // Sanity checks
    DF_TXCLKCTRL_2178A_DEFAULT_PRECONDITIONS(pDf);

    // Check paramaters have been loaded succesfully
    DT_ASSERT(pDf->m_Si534XAddress >= 0);
    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Open children -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    Status = DtDfTxClkCtrl_2178A_OpenChildren(pDf);
    if (!DT_SUCCESS(Status))
    {
        DtDbgOutDf(ERR, SI534X, pDf, "ERROR: failed to open children");
        return DT_STATUS_FAIL;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxClkCtrl_2178A_LoadParameters -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfTxClkCtrl_2178A_LoadParameters(DtDf*  pDfBase)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfTxClkCtrl_2178A* pDf = (DtDfTxClkCtrl_2178A*)pDfBase;

    // List of SI-534X  function parameters
    DtDfParameters  DFSI534X_PARS[] =
    {
        // Name,  Value Type,  Value*
        { "I2C_ADDR", PROPERTY_VALUE_TYPE_INT, &(pDf->m_Si534XAddress) },
    };

    // Sanity checks
    DF_TXCLKCTRL_2178A_DEFAULT_PRECONDITIONS(pDf);
 
    // Init parameters to their defaults
    pDf->m_Si534XAddress = -1;

    // Load parameters from property store
    Status = DtDf_LoadParameters(pDfBase, DT_SIZEOF_ARRAY(DFSI534X_PARS), DFSI534X_PARS);
    if (!DT_SUCCESS(Status))
        return Status;
 
    // Check paramaters have been loaded succesfully
    DT_ASSERT(pDf->m_Si534XAddress >= 0);
    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.- DtDfTxClkCtrl_2178A_OnEnablePostChildren -.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfTxClkCtrl_2178A_OnEnablePostChildren(DtDf*  pDfBase, Bool  Enable)
{
    DtStatus  Status = DT_STATUS_OK;
    DtDfTxClkCtrl_2178A* pDf = (DtDfTxClkCtrl_2178A*)pDfBase;

    // Sanity checks
    DF_TXCLKCTRL_2178A_DEFAULT_PRECONDITIONS(pDf);

    if (Enable)
    {
        // Firmware version 0 uses the SI5342 to generate the TX-clock and must be
        // configured.
        // DTA-2178-ASI HW Rev 2.1 doesn't have a SI5342 and therefore can only work 
        // with firmware version 1.
        // Unfortunately you cannot specify a max_fw property in the property store.
        // Therefore, this ugly solution.
        if (DtCore_DEVICE_GetFirmwareVersion(pDf->m_pCore) == 0)
        {
            // Configure free-running clock(s)
            DT_RETURN_ON_ERROR(DtDfTxClkCtrl_2178A_SetSi534XConfig(pDf));
        }
        // Wait shortly, before resetting/calibrating the PLLs, to allow ALTERA 
        // power-up calibration to complete. Starting a user calibration, while 
        // power-up calibration is busy results in calibration to timeout (fail).
        DtSleep(500);

        // Calibrate TXPLLs
        Status = DtDfTxPllMgr_Calibrate(pDf->m_pDfTxPllMgr);
    }
    return Status;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxClkCtrl_2178A_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfTxClkCtrl_2178A_OpenChildren(DtDfTxClkCtrl_2178A*  pDf)
{
    DtStatus  Status = DT_STATUS_OK;

     // List of children supported by the the SI534X function
    const DtDfSupportedChild  SUPPORTED_CHILDREN[] = 
    {
        //  ObjectType,  BC or DF/CF Type,  Name,  Role,  Shortcut,  IsMandatory
        { DT_OBJECT_TYPE_BC, DT_BLOCK_TYPE_I2CM, DT_BC_I2CM_NAME, 
                               I2CM_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pBcI2Cm), TRUE},
        { DT_OBJECT_TYPE_DF, DT_FUNC_TYPE_TXPLLMGR, DT_DF_TXPLLMGR_NAME, 
                       TXPLLMGR_ROLE_NONE, (DtObjectBcOrDf**)(&pDf->m_pDfTxPllMgr), TRUE}
    };

    DF_TXCLKCTRL_2178A_DEFAULT_PRECONDITIONS(pDf);

    // Use base function get all the children
    Status = DtDf_OpenChildren((DtDf*)pDf, SUPPORTED_CHILDREN,
                                                     DT_SIZEOF_ARRAY(SUPPORTED_CHILDREN));
     if (!DT_SUCCESS(Status))
        return Status;

    // Check mandatory children have been loaded (i.e. shortcut is valid)
    DT_ASSERT(pDf->m_pBcI2Cm != NULL);
    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxClkCtrl_2178A_GetStatus -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfTxClkCtrl_2178A_GetStatus(DtDfTxClkCtrl_2178A* pDf, Bool* pLocked,
                                                                         Bool* pCalibDone)
{
    DtDfSi534XRegister SI5344_INTERNAL_STATUS = {0x000C, 0};
    DtDfSi534XRegister SI5344_HOLDOVER_LOL = {0x000E, 0};

    // Sanity checks
    DF_TXCLKCTRL_2178A_DEFAULT_PRECONDITIONS(pDf);

    // Check parameters
    if (pLocked==NULL || pCalibDone==NULL)
        return DT_STATUS_INVALID_PARAMETER;


    DT_RETURN_ON_ERROR(DtDfTxClkCtrl_2178A_ReadRegister(pDf, &SI5344_INTERNAL_STATUS));
    DT_RETURN_ON_ERROR(DtDfTxClkCtrl_2178A_ReadRegister(pDf, &SI5344_HOLDOVER_LOL));

    *pCalibDone = ((SI5344_INTERNAL_STATUS.m_Data&(1<<0)) == 0); // Sys in calib == false
    *pLocked = ((SI5344_HOLDOVER_LOL.m_Data&(1<<1)) == 0);     // Loss of lock == false
    return DT_STATUS_OK;

}

// -.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxClkCtrl_2178A_SetSi534XConfig -.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus DtDfTxClkCtrl_2178A_SetSi534XConfig(DtDfTxClkCtrl_2178A*  pDf)
{
    Int  Timeout;
    Bool Locked=FALSE, CalibDone=FALSE;

    // Sanity checks
    DF_TXCLKCTRL_2178A_DEFAULT_PRECONDITIONS(pDf);

    // Perform hard reset. Ignore result!
    DtDfTxClkCtrl_2178A_WriteConfig(pDf, SI534X_CONFIG_HARDRST,
                                                  DT_SIZEOF_ARRAY(SI534X_CONFIG_HARDRST));
    pDf->m_PrevBank = -1;

    // Sleep 100ms
    DtSleep(100);
         
    // Write preamble
    DT_RETURN_ON_ERROR(DtDfTxClkCtrl_2178A_WriteConfig(pDf, SI534X_CONFIG_PREAMBLE,
                                                DT_SIZEOF_ARRAY(SI534X_CONFIG_PREAMBLE)));
    // Delay 300 msec
    //   Delay is worst case time for device to complete any calibration
    //   that is running due to device state change previous to this script
    //   being processed.
    DtSleep(300);
    // Update registers
    DT_RETURN_ON_ERROR(DtDfTxClkCtrl_2178A_WriteConfig(pDf,
                                     SI5342_CONFIG_FREE_RUN_DUAL_CLOCK, 
                                     DT_SIZEOF_ARRAY(SI5342_CONFIG_FREE_RUN_DUAL_CLOCK)));
 
     // Write postamble
     DT_RETURN_ON_ERROR(DtDfTxClkCtrl_2178A_WriteConfig(pDf, SI534X_CONFIG_POSTAMBLE,
                                               DT_SIZEOF_ARRAY(SI534X_CONFIG_POSTAMBLE)));

    // Wait for clock to become stable; Poll for locked
    DT_RETURN_ON_ERROR(DtDfTxClkCtrl_2178A_GetStatus(pDf, &Locked, &CalibDone));
    Timeout = 200;
    while (Timeout>0 && !(Locked && CalibDone))
    {
        DT_RETURN_ON_ERROR(DtDfTxClkCtrl_2178A_GetStatus(pDf, &Locked, &CalibDone));
        DtSleep(10);
        Timeout--;
    }
    if (Timeout == 0)
    { 
        DtDbgOutDf(ERR, SI534X, pDf, "Lock timeout");
        return DT_STATUS_TIMEOUT;
    }
    return DT_STATUS_OK;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxClkCtrl_2178A_ReadRegister -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfTxClkCtrl_2178A_ReadRegister(DtDfTxClkCtrl_2178A* pDf, 
                                                         DtDfSi534XRegister*  pConfigItem)
{
    UInt8 WrBuffer[1];
    UInt8 RdBuffer[1];
    UInt8 Bank = (pConfigItem->m_BankAddr >> 8);

    DT_ASSERT(pDf->m_Si534XAddress >= 0);

    if (Bank != pDf->m_PrevBank)
    {
        UInt8 Buffer[2];
        // Change bank
        Buffer[0] = 0x01;
        Buffer[1] = Bank;
        DT_RETURN_ON_ERROR(DtBcI2CM_Write(pDf->m_pBcI2Cm, pDf->m_Si534XAddress,
                                                                 sizeof(Buffer), Buffer));
        pDf->m_PrevBank = Bank;
    }
    // Write address and read data
    WrBuffer[0] = (UInt8)pConfigItem->m_BankAddr;
    DT_RETURN_ON_ERROR(DtBcI2CM_WriteRead(pDf->m_pBcI2Cm, pDf->m_Si534XAddress,
                                                               1, WrBuffer, 1, RdBuffer));
    pConfigItem->m_Data = RdBuffer[0];
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxClkCtrl_2178A_WriteConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtDfTxClkCtrl_2178A_WriteConfig(DtDfTxClkCtrl_2178A* pDf,
                                     const DtDfSi534XRegister* pConfigItems, Int NumItems)
{
    Int i;
    for (i=0; i<NumItems; i++)
        DT_RETURN_ON_ERROR(DtDfTxClkCtrl_2178A_WriteRegister(pDf, &pConfigItems[i]));

    return DT_STATUS_OK;
}

// .-.-.-.-.-.-.-.-.-.-.-.-.- DtDfTxClkCtrl_2178A_WriteRegister -.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtDfTxClkCtrl_2178A_WriteRegister(DtDfTxClkCtrl_2178A* pDf, 
                                                    const DtDfSi534XRegister* pConfigItem)
{
    UInt8 Buffer[2];
    UInt8 Bank = (pConfigItem->m_BankAddr >> 8);

    DT_ASSERT(pDf->m_Si534XAddress >= 0);

    if (Bank != pDf->m_PrevBank)
    {
        // Change bank
        Buffer[0] = 0x01;
        Buffer[1] = Bank;
        DT_RETURN_ON_ERROR(DtBcI2CM_Write(pDf->m_pBcI2Cm, pDf->m_Si534XAddress,
                                                                 sizeof(Buffer), Buffer));
        pDf->m_PrevBank = Bank;
    }
    // Write data
    Buffer[0] = (UInt8)pConfigItem->m_BankAddr;
    Buffer[1] = pConfigItem->m_Data;
    return DtBcI2CM_Write(pDf->m_pBcI2Cm, pDf->m_Si534XAddress, sizeof(Buffer), Buffer);
}
