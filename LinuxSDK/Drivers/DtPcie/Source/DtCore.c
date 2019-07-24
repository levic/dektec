//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtCore.c *#*#*#*#*#*#*#*#*#*#*#* (C) 2017 DekTec
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
#include "DtCoreIncludes.h"            // Core driver includes

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
static DtIoStubIoParams  DtCore_ToIoctlParams(DtCore*, DtFileObject*, DtIoctlObject*);

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
// Called by IAL when an application closes a handle to a device
//
DtStatus  DtCore_Close(DtCore*  pCore, DtFileObject*  pFile)
{
    Int  i=0, Num=0;
    DtFileHandleInfo*  pFileHandleInfo;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    // Notify all PT's that the file is closed. 
    Num = DtVectorPt_Size(pCore->m_pPtList);
    for (i=0; i<Num; i++)
    {
        DtPt*  pPt = DtVectorPt_At(pCore->m_pPtList, i);
        if (pPt==NULL || pPt->m_OnCloseFileFunc==NULL)
            continue;
        pPt->m_OnCloseFileFunc(pPt, pFile);
    }

    // Notify all DF's and BC's that the file is closed. 
    // NOTE: first notify all DFs, than the BCs
    Num = DtVectorDf_Size(pCore->m_pDfList);
    for (i=0; i<Num; i++)
    {
        DtDf*  pDf = DtVectorDf_At(pCore->m_pDfList, i);
        if (pDf==NULL || pDf->m_OnCloseFileFunc==NULL)
            continue;
        pDf->m_OnCloseFileFunc(pDf, pFile);
    }

    Num = DtVectorBc_Size(pCore->m_pBcList);
    for (i=0; i<Num; i++)
    {
        DtBc*  pBc = DtVectorBc_At(pCore->m_pBcList, i);
        if (pBc==NULL || pBc->m_OnCloseFileFunc==NULL)
            continue;
        pBc->m_OnCloseFileFunc(pBc, pFile);
    }
    
    // Remove file handle from the filehandle info array
    DtFastMutexAcquire(&pCore->m_FileHandleInfoMutex);
    pFileHandleInfo = pCore->m_pFileHandleInfo;
    while (pFileHandleInfo != NULL)
    {
        // Search in list and remove entry if found
        if (pFileHandleInfo->m_pHandle == DtFileGetHandle(pFile))
        {
            if (pFileHandleInfo->m_pPrev != NULL)
                pFileHandleInfo->m_pPrev->m_pNext = pFileHandleInfo->m_pNext;
            else
                pCore->m_pFileHandleInfo = pFileHandleInfo->m_pNext;
            if (pFileHandleInfo->m_pNext != NULL)
                pFileHandleInfo->m_pNext->m_pPrev = pFileHandleInfo->m_pPrev;
            DtMemFreePool(pFileHandleInfo, CORE_TAG);
            // No duplicate filehandles are in the list
            break;
        }
        pFileHandleInfo = pFileHandleInfo->m_pNext;
    }
    
    DtFastMutexRelease(&pCore->m_FileHandleInfoMutex);

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_CloseChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_CloseChildren(DtCore*  pCore)
{
    Int  i=0;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 1: close child PTs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // NOTE: we close them in reverse order as they were loaded

    if (pCore->m_pPtList != NULL)
    {
        Int  NumPt = DtVectorPt_Size(pCore->m_pPtList);
        for (i=(NumPt-1); i>=0; i--)
        {
            DtPt*  pChildPt = DtVectorPt_At(pCore->m_pPtList, i);
            if (pChildPt==NULL)
                continue;
            DtCore_PT_Close(pChildPt);
        }
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 1: close child DFs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // NOTE: we close them in reverse order as they were loaded

    if (pCore->m_pDfList != NULL)
    {
        Int  NumDf = DtVectorDf_Size(pCore->m_pDfList);
        for (i=(NumDf-1); i>=0; i--)
        {
            DtDf*  pChildDf = DtVectorDf_At(pCore->m_pDfList, i);
            if (pChildDf==NULL || (pChildDf->m_Id.m_Uuid&DT_UUID_CORE_FLAG)==0)
                continue;
            DtCore_DF_Close(pChildDf);
        }
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 2: close child BCs -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    // NOTE: we close them in reverse order as they were loaded

    if (pCore->m_pBcList != NULL)
    {
        Int  NumBc = DtVectorBc_Size(pCore->m_pBcList);
        for (i=(NumBc-1); i>=0; i--)
        {
            DtBc*  pChildBc = DtVectorBc_At(pCore->m_pBcList, i);
            if (pChildBc==NULL || (pChildBc->m_Id.m_Uuid&DT_UUID_CORE_FLAG)==0)
                continue;
            DtCore_BC_Close(pChildBc);
        }
    }
    
    //.-.-.-.-.-.-.-.-.-.-.-.- Step 3: Reset DF and BC shortcuts -.-.-.-.-.-.-.-.-.-.-.-.-

    pCore->m_pDfVpd = NULL;
    pCore->m_pCfIoCfg = NULL;
    pCore->m_pCfTod = NULL;
    pCore->m_pCfEvt = NULL;
    pCore->m_pCfInt = NULL;
    pCore->m_pCfProps = NULL;
    pCore->m_pBcVvi = NULL;

    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
// Called by IAL when an application opens a handle to a device
//
DtStatus  DtCore_Open(DtCore*  pCore, DtFileObject*  pFile)
{
    DtFileHandleInfo*  pFileHandleInfo=NULL;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    // Add file handle to the filehandle info array
    DtFastMutexAcquire(&pCore->m_FileHandleInfoMutex);
    pFileHandleInfo = (DtFileHandleInfo*)DtMemAllocPool(DtPoolPaged, 
                                                      sizeof(DtFileHandleInfo), CORE_TAG);
    if (pFileHandleInfo == NULL)
    {
        DtDbgOut(ERR, CORE, "Out of memory creating new FileHandleInfo struct");
        DtFastMutexRelease(&pCore->m_FileHandleInfoMutex);
        return DT_STATUS_OUT_OF_MEMORY;
    }
    pFileHandleInfo->m_pHandle = DtFileGetHandle(pFile);
    pFileHandleInfo->m_PowerDownPending = FALSE;

    // Insert at start of list
    pFileHandleInfo->m_pPrev = NULL;
    pFileHandleInfo->m_pNext = NULL;
    if (pCore->m_pFileHandleInfo != NULL) 
    {
        pFileHandleInfo->m_pNext = pCore->m_pFileHandleInfo;
        pFileHandleInfo->m_pNext->m_pPrev = pFileHandleInfo;
    }
    pCore->m_pFileHandleInfo = pFileHandleInfo;
    
    DtFastMutexRelease(&pCore->m_FileHandleInfoMutex);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_OpenChildren -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtCore_OpenChildren(DtCore*  pCore)
{
    DtDfId  DfId;
    DtStatus   Status = DT_STATUS_OK;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    // VVI block must already have been loaded
    DT_ASSERT(pCore->m_pBcVvi != NULL);
    
    
    // -.-.-.-.-.-.-.-.-.-.-.-.-.- Step 1: load PROPS function -.-.-.-.-.-.-.-.-.-.-.-.-.-
    // Load property function before all others. Others need access to property store

    DT_CF_PROPS_INIT_ID(DfId, NULL, 1, CORE_DF_UUID);
    pCore->m_pCfProps = (DtCfProps*)DtCore_DF_Open(pCore, DT_FUNC_TYPE_CF_PROPS, 
                                                                       NULL, &DfId, TRUE);
    if (pCore->m_pCfProps == NULL)
    {
        DtDbgOut(ERR, CORE, "Failed to open PROPS core-function");
        return DT_STATUS_FAIL;
    }

    // =+=+=+=+=+=+ NOTE: FROM HERE ONWARDS WE CAN ACCESS THE PROPERTY STORE +=+=+=+=+=+=+
    // NOTE: we have not loaded the VPD yet, so we have not been able to determine the 
    // boards hardware revision yet, therefor we cannot properly load hw-rev dependant
    // properties yet and the next thing we must do is load the VPD and derive the 
    // hw-rev so that the property-store is full usable

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 2: load VPD function -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    // NOTE: we donot use the UUID from the property store
    
    DT_DF_VPD_INIT_ID(DfId, NULL, 1, CORE_DF_UUID);
    pCore->m_pDfVpd = (DtDfVpd*)DtCore_DF_Open(pCore, DT_FUNC_TYPE_VPD, NULL,
                                                                             &DfId, TRUE);
    if (pCore->m_pDfVpd == NULL)
    {
        DtDbgOut(ERR, CORE, "Failed to open VPD core-function");
        return DT_STATUS_FAIL;
    }

    Status = DtCore_InitSerialAndHwRev(pCore);
    DT_ASSERT(DT_SUCCESS(Status));

    // Re-init property store to update it with hardware revision
    Status = DtCore_PROPS_ReInit(pCore);
    DT_ASSERT(DT_SUCCESS(Status));

    // Get the number of ports
    pCore->m_NumPorts = DtCore_PROPS_GetInt(pCore, "PORT_COUNT", -1, 0);


    // +=+=+=+=+ NOTE: FROM HERE ONWARDS THE PROPERTY STORE IS FULLY FUNCTIONAL +=+=+=+=+=
    
    // .-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 3: load INT function -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    
    DT_CF_INT_INIT_ID(DfId, NULL, 1, CORE_DF_UUID);
    pCore->m_pCfInt = (DtCfInt*)DtCore_DF_Open(pCore, DT_FUNC_TYPE_CF_INT,
                                                                      NULL, &DfId, FALSE);
    if (pCore->m_pCfInt == NULL)
    {
        DtDbgOut(ERR, CORE, "Failed to open INT core-function");
        return DT_STATUS_FAIL;
    }


    // +=+=+=+=+=+ NOTE: FROM HERE ONWARDS WE CAN REGISTER INTERRUPT HANDLERS +=+=+=+=+=+=
    
    // .-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 4: load EVT function -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    
    DT_CF_EVT_INIT_ID(DfId, NULL, 1, CORE_DF_UUID);
    pCore->m_pCfEvt = (DtCfEvt*)DtCore_DF_Open(pCore, DT_FUNC_TYPE_CF_EVT,
                                                                       NULL, &DfId, TRUE);
    if (pCore->m_pCfEvt == NULL)
    {
        DtDbgOut(ERR, CORE, "Failed to open EVT core-function");
        return DT_STATUS_FAIL;
    }

    // .-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 5: load TOD function -.-.-.-.-.-.-.-.-.-.-.-.-.-.
    
    DT_CF_TOD_INIT_ID(DfId, NULL, 1, CORE_DF_UUID);
    pCore->m_pCfTod = (DtCfTod*)DtCore_DF_Open(pCore, DT_FUNC_TYPE_CF_TOD,
                                                                      NULL, &DfId, TRUE);
    if (pCore->m_pCfTod == NULL)
    {
        DtDbgOut(ERR, CORE, "Failed to open TOD core-function");
        return DT_STATUS_FAIL;
    }
    
    // -.-.-.-.-.-.-.-.-.-.-.-.-.- Step 6: load IOCFG function -.-.-.-.-.-.-.-.-.-.-.-.-.-
    
    DT_CF_IOCFG_INIT_ID(DfId, NULL, 1, CORE_DF_UUID);
    pCore->m_pCfIoCfg = (DtCfIoCfg*)DtCore_DF_Open(pCore, DT_FUNC_TYPE_CF_IOCFG,
                                                                       NULL, &DfId, TRUE);
    if (pCore->m_pCfIoCfg == NULL)
    {
        DtDbgOut(ERR, CORE, "Failed to open IOCFG core-function");
        return DT_STATUS_FAIL;
    }
    
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_Ioctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_Ioctl(DtCore*  pCore, DtFileObject*  pFile, DtIoctlObject*  pIoctl)
{
    DtStatus  Status=DT_STATUS_OK;
    DtIoStubIoParams  IoParams;
    DtIoStub*  pIoStub=NULL;
    
    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pCore->m_pIoStub != NULL);

    //-.-.-.-.-.-.-.-.-.-.-.- Step 1: convert to ioctl parameters -.-.-.-.-.-.-.-.-.-.-.-.

    // Check for minimum input data size
    if (pIoctl->m_InputBufferSize < DT_IOCTL_INDATA_MINSIZE)
    {
        DtDbgOut(ERR, CORE, "IOCTL=0x%08X, In=%d (Min=%d) INPUT BUFFER TO SMALL!",
                                                                pIoctl->m_IoctlCode, 
                                                                pIoctl->m_InputBufferSize,
                                                                DT_IOCTL_INDATA_MINSIZE);
        return DT_STATUS_INVALID_PARAMETER;
    }
    // 'Decode' io-control object
    IoParams = DtCore_ToIoctlParams(pCore, pFile, pIoctl);

    //-.-.-.-.-.-.-.-.-.- Step 2: resolve which DF or BC is targetted -.-.-.-.-.-.-.-.-.-.

    // Is the core targetted?
    pIoStub = NULL;
    if (IoParams.m_Uuid == DT_UUID_CORE)
    {
        // Must be targetted at the device-level
        if (IoParams.m_PortIndex != -1)
        {
            DT_ASSERT(IoParams.m_PortIndex == -1);
            DtDbgOut(ERR, CORE, "Invalid PortIndex (%d) used for DT_UUID_CORE", 
                                                                 IoParams.m_PortIndex);

            Status = DT_STATUS_INVALID_PARAMETER;
        }
        else
            pIoStub = (DtIoStub*)pCore->m_pIoStub;     // Use the core stub
    }
    // Is a BC targetted
    else if ((IoParams.m_Uuid & DT_UUID_BC_FLAG)!=0)
    {
        // Find the BC 
        DtBc*  pBc = NULL;
        DT_ASSERT(pCore->m_pUuidToBc != NULL);
        pBc = DtVectorBc_At(pCore->m_pUuidToBc, IoParams.m_UuidIndex);

        if (pBc != NULL)
            pIoStub = (DtIoStub*)pBc->m_pIoStub;
    }
    // Is a DF targetted
    else if ((IoParams.m_Uuid & DT_UUID_DF_FLAG)!=0)
    {
        // Find the DF
        DtDf*  pDf = NULL;
        DT_ASSERT(pCore->m_pUuidToDf != NULL);
        pDf = DtVectorDf_At(pCore->m_pUuidToDf, IoParams.m_UuidIndex);

        if (pDf != NULL)
            pIoStub = (DtIoStub*)pDf->m_pIoStub;
    }
    
    if (pIoStub == NULL)
    {
        DtDbgOut(ERR, CORE, "Unknown UUID (0x%08X) is used for IOCTL=0x%08X", 
                                                    IoParams.m_Uuid, pIoctl->m_IoctlCode);
        Status = DT_STATUS_NO_IOSTUB;
    }
    
    if (DT_SUCCESS(Status))
    {
    }
    
    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 3: Dispatch IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // Set #bytes retured to 'safe' initial value
    pIoctl->m_OutputBufferBytesWritten = 0;

    if (DT_SUCCESS(Status))
    {
        // Let the io-stub handle the command
        if (pIoStub!=NULL && pIoStub->m_IoctlFunc!=NULL)
        {
            Status = pIoStub->m_IoctlFunc(pIoStub, &IoParams, 
                                                     &pIoctl->m_OutputBufferBytesWritten);
        }
        else
        {
            DT_ASSERT(FALSE);
            Status = DT_STATUS_NOT_SUPPORTED;
        }
    }

    // If we failed, no data has the be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED) 
            DtDbgOut(MIN, CORE, "Ioctl: %xh NOT SUPPORTED", pIoctl->m_IoctlCode);
        else
            DtDbgOut(MIN, CORE, "Ioctl: %xh ERROR %xh", pIoctl->m_IoctlCode, Status);
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_InitSerialAndHwRev -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtCore_InitSerialAndHwRev(DtCore*  pCore)
{
    DtStatus  Status = DT_STATUS_OK;
    UInt8  ItemBuf[64];
    Int  NumRead;
    Int64  ForcedHardwareRevision;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Read serial from VPD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
    NumRead = 0;
    Status = DtCore_VPD_ReadItemRo(pCore, "SN", ItemBuf, sizeof(ItemBuf), &NumRead);
    if (DT_SUCCESS(Status))
    {
        DtDbgOut(MIN, CORE, "Serial number read from VPD is '%s'", (const char*)ItemBuf);

        pCore->m_pDevInfo->m_Serial = (Int64)DtAnsiCharArrayToUInt64(
                                                       (const char*)ItemBuf, NumRead, 10);
    }

    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Read HwRev from VPD -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
    NumRead = 0;
    Status = DtCore_VPD_ReadItemRo(pCore, "EC", ItemBuf, sizeof(ItemBuf), &NumRead);
    if (DT_SUCCESS(Status))
    {
        DtDbgOut(MIN, CORE, "HwRev read from VPD is '%s'", (const char*)ItemBuf);
        Status = DtUtilitiesDeduceHardwareRevision((char*)ItemBuf, NumRead,
                                                  &pCore->m_pDevInfo->m_HardwareRevision);
    }

    //.-.-.-.-.-.-.-.-.-.-.-.- Read HwRev from VPD from registry -.-.-.-.-.-.-.-.-.-.-.-.-
    if (Status == DT_STATUS_NOT_FOUND)
    {
        Status = DtCore_SETTINGS_ManufRead(pCore, "ForcedHardwareRevision",
                                                                 &ForcedHardwareRevision);
        if (DT_SUCCESS(Status))
        {
            DtDbgOut(MIN, CORE, "Hardware revision set to %d from registry key "
                                      "'ForcedHardwareRevision'", ForcedHardwareRevision);
            pCore->m_pDevInfo->m_HardwareRevision = (Int)ForcedHardwareRevision;
        }
    }
    return DT_STATUS_OK;
}

//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtCore - Private functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtCore_ToIoctlParams -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStubIoParams  DtCore_ToIoctlParams(DtCore*  pCore, DtFileObject*  pFile, 
                                                                   DtIoctlObject*  pIoctl)
{
    DtIoStubIoParams  Params;
    DtIoctlInputDataHdr*  pHdr = NULL;
    DtFileHandleInfo*  pFileHandleInfo = NULL;

    // Sanity checks
    CORE_DEFAULT_PRECONDITIONS(pCore);
    DT_ASSERT(pIoctl!=NULL && pIoctl->m_pInputBuffer!=NULL);
    DT_ASSERT(pIoctl->m_InputBufferSize >= DT_IOCTL_INDATA_MINSIZE);

    DtMemZero(&Params, sizeof(Params));
    
    Params.m_pIoctl = pIoctl;
    Params.m_pFile = pFile;
    Params.m_pInData = (DtIoctlInputData*)pIoctl->m_pInputBuffer;
    Params.m_pOutData = (DtIoctlOutputData*)pIoctl->m_pOutputBuffer;

    // Init the exclusive access object as a file object
    Params.m_ExclAccessObj.m_Type = DT_EXCL_OBJECT_TYPE_IS_FILE;
    Params.m_ExclAccessObj.m_Owner.m_File = *pFile;

    // Get Uuid, port-index and command
    pHdr = (DtIoctlInputDataHdr*)Params.m_pInData;
    Params.m_Uuid = pHdr->m_Uuid;
    Params.m_UuidIndex = Params.m_Uuid & DT_UUID_INDEX_MASK;
    Params.m_PortIndex = pHdr->m_PortIndex;
    Params.m_Cmd = pHdr->m_Cmd;
    Params.m_CmdEx = pHdr->m_CmdEx;
    
     // Get powerdown pending flag for this file handle from the filehandle info array
    DtFastMutexAcquire(&pCore->m_FileHandleInfoMutex);
    pFileHandleInfo = pCore->m_pFileHandleInfo;
    while (pFileHandleInfo != NULL)
    {
        if (pFileHandleInfo->m_pHandle == DtFileGetHandle(pFile))
        {
            Params.m_PowerDownPending = pFileHandleInfo->m_PowerDownPending;
            break;
        }
        pFileHandleInfo = pFileHandleInfo->m_pNext;
    }
    DtFastMutexRelease(&pCore->m_FileHandleInfoMutex);

    // String and required size are determined by the io-stub
    Params.m_InReqSize = Params.m_OutReqSize = -1;  // -1, means not initialised
    Params.m_pIoctlProps = NULL;
    Params.m_pCmdProps = NULL;
    Params.m_pCmdExProps = NULL;

    return Params;
}

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCore implmentation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forwards for private functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.

//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubCore - Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCore_Init -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCore_Init(DtIoStubCore*  pStub)
{
    // NOTHING TODO FOR NOW
    return DT_STATUS_OK;
}

////-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCore_OnExclAccessCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
////
//DtStatus  DtIoStubCore_OnExclAccessCmd(const DtIoStub*  pStub, Int PortIndex, 
//                                                            Int Cmd, DtFileObject*  pFile)
//{
//    DtStatus  Status=DT_STATUS_OK;
//    STUB_CORE_DEFAULT_PRECONDITIONS(pStub);
//    
//    switch(Cmd)
//    {
//    case DT_EXCLUSIVE_ACCESS_CMD_ACQUIRE:
//        Status = DT_STATUS_NOT_SUPPORTED;
//        break;
//    case DT_EXCLUSIVE_ACCESS_CMD_CHECK:
//        Status = DT_STATUS_NOT_SUPPORTED;
//        break;
//    case DT_EXCLUSIVE_ACCESS_CMD_PROBE:
//        Status = DT_STATUS_NOT_SUPPORTED;
//        break;
//    case DT_EXCLUSIVE_ACCESS_CMD_RELEASE:
//        Status = DT_STATUS_NOT_SUPPORTED;
//        break;
//
//    default:
//        DT_ASSERT(FALSE);
//        Status = DT_STATUS_NOT_SUPPORTED;
//        break;
//    }
//    return Status;
//}

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCore_OnExclAccessCmd -.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStubCore_OnExclAccessCmd(
    const DtIoStub*  pStub,
    DtIoStubIoParams*  pIoParams,
    Int*  pOutSize)
{
    DtStatus Status = DT_STATUS_OK;
    const DtIoctlExclAccessCmdInput*  pInData = NULL;
        
    STUB_CORE_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams!=NULL && pOutSize!=NULL);
    DT_ASSERT(pIoParams->m_pIoctl->m_IoctlCode == DT_IOCTL_EXCL_ACCESS_CMD);

    // Get in-/out-data
    DT_ASSERT(pIoParams->m_pInData != NULL);
    pInData = &pIoParams->m_pInData->m_ExclAccessCmd;
    
    //-.-.-.-.-.-.-.-.-.-.-.-.- Call appropriate command handler -.-.-.-.-.-.-.-.-.-.-.-.-

    switch (pIoParams->m_Cmd)
    {
    case DT_EXCLUSIVE_ACCESS_CMD_ACQUIRE:
        Status = DT_STATUS_NOT_IMPLEMENTED;
        break;
    case DT_EXCLUSIVE_ACCESS_CMD_CHECK:
        Status = DT_STATUS_NOT_IMPLEMENTED;
        break;
    case DT_EXCLUSIVE_ACCESS_CMD_PROBE:
        Status = DT_STATUS_NOT_IMPLEMENTED;
        break;
    case DT_EXCLUSIVE_ACCESS_CMD_RELEASE:
        Status = DT_STATUS_NOT_IMPLEMENTED;
        break;
    default:
        DT_ASSERT(FALSE);
        return DT_STATUS_NOT_SUPPORTED;
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCore_GetChildStub -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtIoStub*  DtIoStubCore_GetChildStub(
                               const DtIoStub*  pStub, const DtIoStubIoParams*  pIoParams)
{
    DtCore*  pCore = NULL;
    DtIoStub*  pChildStub = NULL;

    STUB_CORE_DEFAULT_PRECONDITIONS(pStub);
    DT_ASSERT(pIoParams != NULL);

    pCore = pStub->m_pCore;
    DT_ASSERT(pCore!=NULL && pCore->m_Size>=(sizeof(DtCore)));

    switch (pIoParams->m_pIoctl->m_IoctlCode)
    {
    case DT_IOCTL_EVENT_CMD:
        DT_ASSERT(pCore->m_pCfEvt != NULL);
        if (pCore->m_pCfEvt != NULL)
            pChildStub = (DtIoStub*)pCore->m_pCfEvt->m_pIoStub;
        break;

    case DT_IOCTL_IOCONFIG_CMD:
        DT_ASSERT(pCore->m_pCfIoCfg != NULL);
        if (pCore->m_pCfIoCfg != NULL)
            pChildStub = (DtIoStub*)pCore->m_pCfIoCfg->m_pIoStub;
        break;

    case DT_IOCTL_PROPERTY_CMD:
        DT_ASSERT(pCore->m_pCfProps != NULL);
        if (pCore->m_pCfProps != NULL)
            pChildStub = (DtIoStub*)pCore->m_pCfProps->m_pIoStub;
        break;

    case DT_IOCTL_TOD_CMD:
        DT_ASSERT(pCore->m_pCfTod != NULL);
        if (pCore->m_pCfTod != NULL)
            pChildStub = (DtIoStub*)pCore->m_pCfTod->m_pIoStub;
        break;

    case DT_IOCTL_VPD_CMD:
        DT_ASSERT(pCore->m_pDfVpd != NULL);
        if (pCore->m_pDfVpd != NULL)
            pChildStub = (DtIoStub*)pCore->m_pDfVpd->m_pIoStub;
        break;
    default:
        DT_ASSERT(FALSE);
        pChildStub = NULL;
        break;
    }
    return pChildStub;
}
