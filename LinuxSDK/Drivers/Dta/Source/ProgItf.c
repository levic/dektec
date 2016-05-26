//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* ProgItf.c *#*#*#*#*#*#*#*#*#*#*#* (C) 2016 DekTec
//
// Dta driver - Implements programmin interface control support
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2012-2016 DekTec Digital Video B.V.
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

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include <DtaIncludes.h>

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Forward declarations -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+


//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaProgItfIoctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaProgItfIoctl(
    DtaDeviceData*  pDvcData,
    DtFileObject*  pFile,
    DtIoctlObject*  pIoctl,
    Bool  PowerDownPending)
{
    DtStatus  Status = DT_STATUS_OK;
    char*  pCmdStr;             // Mnemonic string for Command
    UInt  InReqSize = 0;        // Required length of input buffer
    UInt  OutReqSize = 0;       // Required length of output buffer
    Int  ProgItfIdx;            // Index of the programming interface
    Int  ProgItf;               // Programming interface type
    DtaProgItf*  pProgItf = NULL;
    DtaIoctlProgItfCmdInput*  pProgItfCmdInput = 
                                         (DtaIoctlProgItfCmdInput*)pIoctl->m_pInputBuffer;
    
    // Default require at least the size of the header preceding the data
    InReqSize = OFFSETOF(DtaIoctlProgItfCmdInput, m_Data);

    // Check if we can read m_Cmd / m_PortIndex
    if (pIoctl->m_InputBufferSize < InReqSize)
        return DT_STATUS_INVALID_PARAMETER;

    // Validate programming interface index
    ProgItfIdx = pProgItfCmdInput->m_ItfIndex;
    if (ProgItfIdx >= sizeof(pDvcData->m_ProgItfs)/sizeof(pDvcData->m_ProgItfs[0]))
        return DT_STATUS_INVALID_PARAMETER;

    // Check interface type, SPI-MF doesn't use this lock mechanism
    ProgItf = pDvcData->m_ProgItfs[ProgItfIdx].m_InterfaceType;
    if (ProgItf==PROG_ITF_NOT_SUPP || ProgItf==PROG_ITF_SPI_MF)
        return DT_STATUS_NOT_SUPPORTED;

    pProgItf = &pDvcData->m_ProgItfs[ProgItfIdx];
      
    // Determine final required output/input sizes
    switch (pProgItfCmdInput->m_Cmd)
    {
    case DTA_PROGITF_CMD_EXCLUSIVE_ACCESS:
        pCmdStr = "DTA_PROGITF_CMD_EXCLUSIVE_ACCESS";
        InReqSize += sizeof(DtaIoctlNonIpCmdExclusiveAccessInput);
        OutReqSize = 0;
        break;
    default:
        pCmdStr = "??UNKNOWN PROGITF_CMD CODE??";
        Status = DT_STATUS_NOT_SUPPORTED;
    }

    if (DT_SUCCESS(Status))
    {
        // Check buffer sizes
        if (pIoctl->m_InputBufferSize < InReqSize)
        {
            DtDbgOut(ERR, PROGITF, "%s: INPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr,
                                                    pIoctl->m_InputBufferSize, InReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }
        if (pIoctl->m_OutputBufferSize < OutReqSize)
        {
            DtDbgOut(ERR, PROGITF, "%s: OUTPUT BUFFER TOO SMALL Size=%d Req=%d", pCmdStr, 
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
            return DT_STATUS_INVALID_PARAMETER;
        }

        DtDbgOut(MAX, PROGITF, "%s: In=%d (Rq=%d), Out=%d (Rq=%d)", pCmdStr,
            pIoctl->m_InputBufferSize, InReqSize, pIoctl->m_OutputBufferSize, OutReqSize);
    }

    // The bytes written will be updated if needed. Set the default value here.
    pIoctl->m_OutputBufferBytesWritten = OutReqSize;

    if (DT_SUCCESS(Status))
    {
        switch (pProgItfCmdInput->m_Cmd)
        {
        case DTA_PROGITF_CMD_EXCLUSIVE_ACCESS:
            Status = DtaProgItfExclusiveAccess(pProgItf, pFile,
                                        pProgItfCmdInput->m_Data.m_ExclusiveAccess.m_Cmd);
            break;

        }
    }

    // If we failed, no data has to be copied to user space
    if (!DT_SUCCESS(Status))
    {
        pIoctl->m_OutputBufferBytesWritten = 0;
        if (Status == DT_STATUS_NOT_SUPPORTED)
            DtDbgOut(MIN, PROGITF, "ProgItfCmd=0x%x: NOT SUPPORTED", pProgItfCmdInput->m_Cmd);
        else if (Status == DT_STATUS_IO_PENDING)
            DtDbgOut(MAX, PROGITF, "%s: ERROR %xh", pCmdStr, Status); // NOT A REAL ERROR
        else
            DtDbgOut(MIN, PROGITF, "%s: ERROR %xh", pCmdStr, Status);
    }
    return Status;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaProgItfInit -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaProgItfInit(DtaDeviceData* pDvcData)
{
    DtPropertyData*  pPropData = &pDvcData->m_PropData;
    UInt  i;
    for (i=0; i<sizeof(pDvcData->m_ProgItfs)/sizeof(pDvcData->m_ProgItfs[0]); i++)
    {
        pDvcData->m_ProgItfs[i].m_InterfaceType = PROG_ITF_NOT_SUPP;
        pDvcData->m_ProgItfs[i].m_ExclAccess = FALSE;
        DtSpinLockInit(&pDvcData->m_ProgItfs[i].m_ExclAccessLock);
    }
    // Get first programming interface
    if (i > 0)
        pDvcData->m_ProgItfs[0].m_InterfaceType = 
                                     DtPropertiesGetInt(pPropData, "PROGRAMMING_ITF", -1);
    // Get second programming interface
    if (i > 1)
        pDvcData->m_ProgItfs[1].m_InterfaceType = 
                                 DtPropertiesGetInt(pPropData, "PROGRAMMING_SEC_ITF", -1);
    return DT_STATUS_OK;
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaProgItfClose -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtaProgItfClose(DtaDeviceData* pDvcData, DtFileObject* pFile)
{
    DtaProgItf*  pProgItf;
    UInt  i;
    for (i=0; i<sizeof(pDvcData->m_ProgItfs)/sizeof(pDvcData->m_ProgItfs[0]); i++)
    {
        pProgItf = &pDvcData->m_ProgItfs[i];
        if (pProgItf->m_InterfaceType!=PROG_ITF_NOT_SUPP
                                            && pProgItf->m_InterfaceType!=PROG_ITF_SPI_MF)
        {
            DtSpinLockAcquire(&pProgItf->m_ExclAccessLock);
            if (pProgItf->m_ExclAccess 
                                    && DtFileCompare(&pProgItf->m_ExclAccessOwner, pFile))
            {
                pProgItf->m_ExclAccess = FALSE;
            }
            DtSpinLockRelease(&pProgItf->m_ExclAccessLock);
        }
    }
    return DT_STATUS_OK;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtaProgItfExclusiveAccess -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtaProgItfExclusiveAccess(
    DtaProgItf*  pProgItf,  
    DtFileObject*  pFile,
    Int  Cmd)
{
    DtStatus  Result = DT_STATUS_OK;

    // Skip if called from the driver directly
    if (pFile == DT_INVALID_FILE_OBJECT_PTR)
        return DT_STATUS_OK;

    DtSpinLockAcquire(&pProgItf->m_ExclAccessLock);

    if (Cmd == DTA_EXCLUSIVE_ACCESS_CMD_ACQUIRE)
    {
        if (!pProgItf->m_ExclAccess)
        {
            pProgItf->m_ExclAccess = TRUE;
            pProgItf->m_ExclAccessOwner = *pFile;
        } else
            Result = DT_STATUS_IN_USE;
    } else if (Cmd == DTA_EXCLUSIVE_ACCESS_CMD_RELEASE)
    {
        if (pProgItf->m_ExclAccess)
        {
            if (DtFileCompare(&pProgItf->m_ExclAccessOwner, pFile))
            {
                pProgItf->m_ExclAccess = FALSE;
            }
            else
                Result = DT_STATUS_IN_USE;
        }
    } else if (Cmd == DTA_EXCLUSIVE_ACCESS_CMD_PROBE)
    {
        if (pProgItf->m_ExclAccess)
            Result = DT_STATUS_IN_USE;
    } else if (Cmd == DTA_EXCLUSIVE_ACCESS_CMD_CHECK)
    {
        if (!pProgItf->m_ExclAccess)
            Result = DT_STATUS_EXCL_ACCESS_REQD;
        else if (!DtFileCompare(&pProgItf->m_ExclAccessOwner, pFile))
            Result = DT_STATUS_IN_USE;
    } else
        Result = DT_STATUS_NOT_SUPPORTED;
    
    DtSpinLockRelease(&pProgItf->m_ExclAccessLock);

    return Result;
}


