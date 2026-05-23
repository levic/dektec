//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcGPIO.c *#*#*#*#*#*#*#*#*#*#*# (C) 2024 DekTec
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
#include "DtBc.h"
#include "DtBcGPIO.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcGPIO implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Defines / Constants -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// MACRO with default precondition checks for the Bc
#define BC_GPIO_DEFAULT_PRECONDITIONS(pBc)      \
    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcGPIO))

// Helper macro to cast a DtBc* to a DtBcGPIO*
#define BC_GPIO         ((DtBcGPIO*)pBc)

// DTA-2128 LNB Power Control GPIO Input/Outputs 
static const DtBcGPIO_IoProps Dta2128_LnbPowerCtrloProps[] =
{
    // Field mnemonic           IsOutput    Mask           Offset
    {"Ext12V_Detected",         FALSE,      0x00000001,    0x04},
    {"Lnb_EnableHighCurrent",   TRUE,       0x00000001,    0x08},
};


// GPIO instances
static const DtBcGPIO_GPIO DtBcGPIO_GpioProps[] =
{
    // GPIO-role                Input/Output properties         Number of inputs/outputs
    {"DTA2128_LNB_POWER_CTRL",  Dta2128_LnbPowerCtrloProps,     DT_SIZEOF_ARRAY(Dta2128_LnbPowerCtrloProps)},
};
static Int DtBcGPIO_GpioPropsSize = DT_SIZEOF_ARRAY(DtBcGPIO_GpioProps);


//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtBcGPIO_Init(DtBc*);
static Int DtBcGPIO_StringCompare(const char* Str1, const char* Str2);
static DtStatus  DtBcGPIO_OnEnable(DtBc*, Bool  Enable);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcGPIO - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPIO_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtBcGPIO_Close(DtBc*  pBc)
{
    BC_GPIO_DEFAULT_PRECONDITIONS(pBc);

    // Let base function perform final clean-up
    DtBc_Close(pBc);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPIO_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtBcGPIO*  DtBcGPIO_Open(Int  Address, DtCore*  pCore, DtPt* pPt,
                           const char*  pRole, Int  Instance, Int  Uuid, Bool  CreateStub)
{
    DtBcId  Id;
    DtBcOpenParams  OpenParams;

    DT_ASSERT(pCore!=NULL && pCore->m_Size>=sizeof(DtCore));

    // Init open parameters
    DT_BC_GPIO_INIT_ID(Id, pRole, Instance, Uuid);
    DT_BC_INIT_OPEN_PARAMS(OpenParams, DtBcGPIO, Id, DT_BLOCK_TYPE_GPIO, Address,
                                                                  pPt, CreateStub, pCore);
    // Register the callbacks
    OpenParams.m_CloseFunc = DtBcGPIO_Close;
    OpenParams.m_InitFunc = DtBcGPIO_Init;
    OpenParams.m_OnEnableFunc = DtBcGPIO_OnEnable;

    // Use base function to allocate and perform standard initialization of block data
    return (DtBcGPIO*)DtBc_Open(&OpenParams);
}

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPIO_Read -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcGPIO_Read(DtBcGPIO* pBc, const char* FieldMnem, UInt32* pValue)
{
    Int PropIdx=0;
    UInt32 Value;

    // Find the Field
    for (PropIdx=0; PropIdx<pBc->m_NumIoProps; PropIdx++)
    {
        if (DtBcGPIO_StringCompare(pBc->m_pIoProps[PropIdx].m_Mnemonic, FieldMnem) == 0)
            break;
    }
    if (PropIdx >= pBc->m_NumIoProps)
    {
        DtDbgOutBc(ERR, GPIO, pBc, "Input: %s not found", FieldMnem);
        return DT_STATUS_NOT_FOUND;
    }

    // Read the register
    Value =  DtBc_RegRead32((DtBc*)pBc, pBc->m_pIoProps[PropIdx].m_RegisterOffset);
    *pValue = Value & pBc->m_pIoProps[PropIdx].m_Mask;
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPIO_Write -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus DtBcGPIO_Write(DtBcGPIO* pBc, const char* FieldMnem, UInt32 Value)
{
    Int PropIdx=0;

    // Find the Field
    for (PropIdx=0; PropIdx<pBc->m_NumIoProps; PropIdx++)
    {
        if (DtBcGPIO_StringCompare(pBc->m_pIoProps[PropIdx].m_Mnemonic, FieldMnem) == 0)
            break;
    }
    if (PropIdx >= pBc->m_NumIoProps)
    {
        DtDbgOutBc(ERR, GPIO, pBc, "Output: %s not found", FieldMnem);
        return DT_STATUS_NOT_FOUND;
    }
    // Check parameters
    if (!pBc->m_pIoProps[PropIdx].m_IsOutput
                                          || (Value& ~pBc->m_pIoProps[PropIdx].m_Mask)!=0)
    {
        DtDbgOutBc(ERR, GPIO, pBc, "Output: %s invalid value 0x%x", FieldMnem, Value);
        return DT_STATUS_INVALID_PARAMETER;
    }

    // Write the register
    DtBc_RegWrite32((DtBc*)pBc, pBc->m_pIoProps[PropIdx].m_RegisterOffset, Value);

    return DT_STATUS_OK;
}


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcGPIO - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPIO_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcGPIO_Init(DtBc*  pBcBase)
{
    DtBcGPIO* pBc = (DtBcGPIO*) pBcBase;
    Int i=0;

    pBc->m_pIoProps = NULL;
    pBc->m_NumIoProps = 0;

    // Get the GPIO properties
    if (pBc->m_Id.m_pRole == NULL)
    {
        DtDbgOutBc(ERR, GPIO, pBc, "Role is missing");
        return DT_STATUS_CONFIG_ERROR;
    }

    for (i=0; i<DtBcGPIO_GpioPropsSize; i++)
    {
        if (DtBcGPIO_StringCompare(pBc->m_Id.m_pRole, DtBcGPIO_GpioProps[i].m_Role) == 0)
        {
            pBc->m_pIoProps = DtBcGPIO_GpioProps[i].m_IoProps;
            pBc->m_NumIoProps = DtBcGPIO_GpioProps[i].m_NumIoProps;
            break;
        }
    }

    if (pBc->m_pIoProps == NULL)
    {
        DtDbgOutBc(ERR, GPIO, pBc, "Unsuported GPIO role: %s", pBc->m_Id.m_pRole);
        return DT_STATUS_CONFIG_ERROR;
    }
    return DT_STATUS_OK;
}

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPIO_StringCompare -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Compares two C strings.
//
Int DtBcGPIO_StringCompare(const char* Str1, const char* Str2)
{
    // Prevent BSOD
    if (Str1==NULL || Str2==NULL)
        return 1;

    while (*Str1 && (*Str1 == *Str2)) {
        Str1++;
        Str2++;
    }
    return *(UInt8*)Str1 - *(UInt8*)Str2;
}


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcGPIO_OnEnable -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtBcGPIO_OnEnable(DtBc* pBc, Bool  Enable)
{
    // NOTHING SPECIAL TODO FOR NOW
    return DT_STATUS_OK;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcGPIO implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Helper macro to cast stub's DtBc member to DtBcGPIO
#define GPIO_STUB   ((DtIoStubBcGPIO*)pStub)
#define GPIO_BC  ((DtBcGPIO*)GPIO_STUB->m_pBc)

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards of private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
static DtStatus  DtIoStubBcGPIO_OnCmd(const DtIoStub*, DtIoStubIoParams*, Int* pOutSize);
static DtStatus  DtIoStubBcGPIO_OnCmdRead(const DtIoStubBcGPIO*,
                                                      const DtIoctlGpioCmdReadInput*,
                                                      DtIoctlGpioCmdReadOutput*);
static DtStatus  DtIoStubBcGPIO_OnCmdWrite(const DtIoStubBcGPIO*,
                                                    const DtIoctlGpioCmdWriteInput*);

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- List of supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// First declare IOCTL commands
DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS;
DECL_DT_IOCTL_CMD_PROPS_GPIO;

static const DtIoctlProperties  IOSTUB_BC_GPIO_IOCTLS[] =
{
    DT_IOCTL_PROPS_EXCL_ACCESS_CMD(
        DtIoStubBc_OnExclAccessCmd,     // Use default handler
        NULL, NULL),
    DT_IOCTL_PROPS_GPIO_CMD(
        DtIoStubBcGPIO_OnCmd,
        NULL,
        NULL),
};


//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcGPIO - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcGPIO_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
void  DtIoStubBcGPIO_Close(DtIoStub*  pStub)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcGPIO));

    // Let base function perform final clean-up
    DtIoStubBc_Close(pStub);
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcGPIO_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStubBcGPIO*  DtIoStubBcGPIO_Open(DtBc*  pBc)
{
    DtIoStubBcGPIO*  pStub = NULL;
    DtIoStubBcOpenParams  OpenParams;

    DT_ASSERT(pBc!=NULL && pBc->m_Size==sizeof(DtBcGPIO));

    // Use base function to allocate and perform standard initialisation of stub data
    DT_IOSTUBBC_INIT_OPEN_PARAMS(OpenParams, DtIoStubBcGPIO, pBc, NULL,
        DtIoStubBcGPIO_Close,
        NULL,  // Use default IOCTL
        IOSTUB_BC_GPIO_IOCTLS);
    pStub = (DtIoStubBcGPIO*)DtIoStubBc_Open(&OpenParams);
    if (pStub == NULL)
        return NULL;
    return pStub;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcGPIO - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcGPIO_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcGPIO_OnCmd(const DtIoStub*  pStub, DtIoStubIoParams*  pIoParams,
    Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlGpioCmdInput*  pInData = NULL;
    DtIoctlGpioCmdOutput*  pOutData = NULL;

    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcGPIO));
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_GPIO_CMD);
    DT_ASSERT(*pOutSize == pIoParams->m_OutReqSize);

    // Do we need exlusive access?
    if (pIoParams->m_ExclAccessIsRequired)
    {
        Status = DtBc_ExclAccessCheck(((DtIoStubBc*)pStub)->m_pBc,
            &pIoParams->m_ExclAccessObj);
        if (Status != DT_STATUS_OK)
        {
            DtDbgOutIoStubBc(ERR, GPIO, pStub, "ERROR: block is not locked by me");
            return Status;
        }
    }

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_GpioCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_GpioCmd;
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-
    switch (pIoParams->m_Cmd)
    {
    case DT_GPIO_CMD_READ:
        DT_ASSERT(pOutData != NULL);
        Status = DtIoStubBcGPIO_OnCmdRead(GPIO_STUB, &pInData->m_Read,
                                                                  &pOutData->m_Read);
        break;
    case DT_GPIO_CMD_WRITE:
        Status = DtIoStubBcGPIO_OnCmdWrite(GPIO_STUB, &pInData->m_Write);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}


// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcGPIO_OnCmdRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubBcGPIO_OnCmdRead(
    const DtIoStubBcGPIO*  pStub,
    const DtIoctlGpioCmdReadInput*  pInData,
    DtIoctlGpioCmdReadOutput*  pOutData)
{

    DtStatus  Status = DT_STATUS_OK;
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcGPIO));
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);

    // Assume the worst
    Status = DtBcGPIO_Read(GPIO_BC, pInData->m_FieldMnem, &pOutData->m_Value);
    return  Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcGPIO_OnCmdWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubBcGPIO_OnCmdWrite(
    const DtIoStubBcGPIO*  pStub,
    const DtIoctlGpioCmdWriteInput*  pInData)
{
    DT_ASSERT(pStub!=NULL && pStub->m_Size==sizeof(DtIoStubBcGPIO));
    DT_ASSERT(pInData != NULL);

    return DtBcGPIO_Write(GPIO_BC, pInData->m_FieldMnem, pInData->m_Value);
}
