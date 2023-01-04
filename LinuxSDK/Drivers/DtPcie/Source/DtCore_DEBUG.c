// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCore_DEBUG.c *#*#*#*#*#*#*#* (C) 2017-2022 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2017-2022 DekTec Digital Video B.V.
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
#include "DtCoreIncludes.h"            // Core driver includes

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCore_DEBUG implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtStatus  DtIoStubCore_DEBUG_OnCmdRegRead(const DtIoStubCore*, 
                                                       const DtIoctlDebugCmdRegReadInput*,
                                                       DtIoctlDebugCmdRegReadOutput*);
static DtStatus  DtIoStubCore_DEBUG_OnCmdRegWrite(const DtIoStubCore*, 
                                                     const DtIoctlDebugCmdRegWriteInput*);
static DtStatus  DtIoStubCore_DEBUG_OnCmdBulkRead(const DtIoStubCore*, 
                                                      const DtIoctlDebugCmdBulkReadInput*,
                                                      DtIoctlDebugCmdBulkReadOutput*);

//+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCore_DEBUG - Public fuctions +=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCore_DEBUG_AppendDynamicSize -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCore_DEBUG_AppendDynamicSize(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlDebugCmdInput*  pInData = NULL;

    STUB_CORE_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_DEBUG_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_DebugCmd;

    //-.-.-.-.-.-.- Step 1: Append dynamic part to required size of command -.-.-.-.-.-.-.

    switch (pIoParams->m_Cmd)
    {
    case DT_DEBUG_CMD_REGISTER_READ:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlDebugCmdRegReadInput));
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlDebugCmdRegReadOutput));
        // Add dynamic size (i.e. #regs to read)
        pIoParams->m_InReqSize += (pInData->m_RegRead.m_NumToRead * sizeof(UInt32));
        pIoParams->m_OutReqSize += (pInData->m_RegRead.m_NumToRead 
                                                    * sizeof(DtIoctlRegAddressValuePair));
        break;

    case DT_DEBUG_CMD_REGISTER_WRITE:
        // Sanity checks
        DT_ASSERT(pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_InReqSize >= sizeof(DtIoctlDebugCmdRegWriteInput));
        // Add dynamic size (i.e. #regs to write)
        pIoParams->m_InReqSize += (pInData->m_RegWrite.m_NumToWrite
                                                    * sizeof(DtIoctlRegAddressValuePair));
        break;

    case DT_DEBUG_CMD_BULK_READ:
        // Sanity checks
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && pIoParams->m_OutReqSizeIsDynamic);
        DT_ASSERT(pIoParams->m_OutReqSize >= sizeof(DtIoctlDebugCmdBulkReadOutput));
        // Add dynamic size (i.e. #regs to read)
        pIoParams->m_OutReqSize += (pInData->m_BulkRead.m_NumToRead * sizeof(UInt32));
        break;


    default:
        DT_ASSERT(!pIoParams->m_InReqSizeIsDynamic && !pIoParams->m_OutReqSizeIsDynamic);
        if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
            Status = DT_STATUS_FAIL;
        break;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCore_DEBUG_OnCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCore_DEBUG_OnCmd(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams,
    Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlDebugCmdInput*  pInData = NULL;
    DtIoctlDebugCmdOutput*  pOutData = NULL;
    
    STUB_CORE_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_FunctionCode == DT_FUNC_CODE_DEBUG_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_DebugCmd;
    if (pIoParams->m_OutReqSize > 0)
    {
        DT_ASSERT(pIoParams->m_pOutData != NULL);
        pOutData = &pIoParams->m_pOutData->m_DebugCmd;
    }



    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_DEBUG_CMD_REGISTER_READ:
        Status = DtIoStubCore_DEBUG_OnCmdRegRead(STUB_CORE, &pInData->m_RegRead, 
                                                                    &pOutData->m_RegRead);
        break;
    case DT_DEBUG_CMD_REGISTER_WRITE:
        Status = DtIoStubCore_DEBUG_OnCmdRegWrite(STUB_CORE, &pInData->m_RegWrite);
        break;
    case DT_DEBUG_CMD_BULK_READ:
        Status = DtIoStubCore_DEBUG_OnCmdBulkRead(STUB_CORE, &pInData->m_BulkRead, 
                                                                   &pOutData->m_BulkRead);
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCore_DEBUG - Private fuctions +=+=+=+=+=+=+=+=+=+=+=+=+


//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCore_DEBUG_OnCmdRegRead -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubCore_DEBUG_OnCmdRegRead(
    const DtIoStubCore*  pStub,
    const DtIoctlDebugCmdRegReadInput*  pInData,
    DtIoctlDebugCmdRegReadOutput*  pOutData)
{
    Int  i=0;
    DtCore*  pCore = NULL;

    STUB_CORE_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);
    pCore = pStub->m_pCore;
    DT_ASSERT(pCore!=NULL && pCore->m_RegRead32Func!=NULL);

    // Read all the registers
    pOutData->m_NumRead = 0;
    for (i=0; i<pInData->m_NumToRead; i++)
    {
        pOutData->m_NumRead++;
        pOutData->m_Regs[i].m_Address = pInData->m_Address[i];
        pOutData->m_Regs[i].m_Value = pCore->m_RegRead32Func(pCore, 
                                                           pOutData->m_Regs[i].m_Address);
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCore_DEBUG_OnCmdRegWrite -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStubCore_DEBUG_OnCmdRegWrite(
    const DtIoStubCore*  pStub, 
    const DtIoctlDebugCmdRegWriteInput*  pInData)
{
    Int  i=0;
    DtCore*  pCore = NULL;

    STUB_CORE_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData != NULL);
    pCore = pStub->m_pCore;
    DT_ASSERT(pCore!=NULL && pCore->m_RegWrite32Func!=NULL);

    // Write to all the registers
    for (i=0; i<pInData->m_NumToWrite; i++)
    {
        const UInt32  Address = pInData->m_Regs[i].m_Address; 
        const UInt32  Value = pInData->m_Regs[i].m_Value;
        pCore->m_RegWrite32Func(pCore, Address, Value);
    }
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCore_DEBUG_OnCmdBulkRead -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCore_DEBUG_OnCmdBulkRead(
    const DtIoStubCore*  pStub,
    const DtIoctlDebugCmdBulkReadInput*  pInData,
    DtIoctlDebugCmdBulkReadOutput*  pOutData)
{
    DtCore*  pCore = NULL;
    DtStatus  Status = DT_STATUS_OK;
    STUB_CORE_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pInData!=NULL && pOutData!=NULL);
    pCore = pStub->m_pCore;
    DT_ASSERT(pCore!=NULL && pCore->m_BulkRead32Func!=NULL);

    // Read all the registers
    Status = pCore->m_BulkRead32Func(pCore, pInData->m_StartAddress, pInData->m_NumToRead,
                                                                   pOutData->m_RegValues);
    if (DT_SUCCESS(Status))
        pOutData->m_NumRead = pInData->m_NumToRead;
    else
        pOutData->m_NumRead = 0;

    return Status;
}