//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoStub.c *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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
#include "DtSal.h"
#include "DtDrvCommon.h"
#include "DtCore.h"
#include "DtIoStub.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStub implementation +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ Public functions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStub_Close -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
void  DtIoStub_Close(DtIoStub*  pStub)
{
    if (pStub == NULL)
        return;
    DT_ASSERT(pStub->m_Size>=sizeof(DtIoStub));

    // Free stub data
    DtObject_Close((DtObject*)pStub, CORE_TAG);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStub_Open -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtIoStub*  DtIoStub_Open(const DtIoStubOpenParams*  pParams)
{
    Int i=0;
    DtIoStub*  pStub = NULL;

    DT_ASSERT(pParams->m_Size >= sizeof(DtIoStub));
    DT_ASSERT(pParams->m_pCore!=NULL && pParams->m_pCore->m_Size>=sizeof(DtCore));
    DT_ASSERT(pParams->m_pName != NULL);
    DT_ASSERT(pParams->m_CloseFunc != NULL);

    //-.-.-.-.-.-.-.-.- Step 1: Allocate memory for ioctl stub structure -.-.-.-.-.-.-.-.-

    // Allocate memory for io-control stub
    DT_ASSERT(pParams->m_ObjectType==DT_OBJECT_TYPE_IOSTUB
                                      || pParams->m_ObjectType==DT_OBJECT_TYPE_IOSTUB_BC
                                      || pParams->m_ObjectType==DT_OBJECT_TYPE_IOSTUB_DF);

    pStub = (DtIoStub*)DtObject_Open(pParams->m_ObjectType, pParams->m_Size, CORE_TAG);
    if (pStub == NULL)
        return NULL;

    //-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 2: Init stub attributes -.-.-.-.-.-.-.-.-.-.-.-.-.-

    // Set shortcut to core driver services
    pStub->m_pCore = pParams->m_pCore;
    // Copy name
    if (pParams->m_pName != NULL)
    {
        pStub->m_Name[0] = '\0';
        for (i=0; i<DT_IOSTUB_NAME_MAX_SIZE; i++)
        {
            pStub->m_Name[i] = pParams->m_pName[i];
            if (pParams->m_pName[i] == '\0')
                break;      // Reach end of string
        }
    }

    // Set close and ioctl function callbacks
    pStub->m_CloseFunc = pParams->m_CloseFunc;
    // Does derived class provide its own IOCTL handler?
    if (pParams->m_IoctlFunc != NULL)
        pStub->m_IoctlFunc = pParams->m_IoctlFunc;
    else
        pStub->m_IoctlFunc = DtIoStub_Ioctl;    // Use default implementation

    // Set supported IOCTL properties
    DT_ASSERT(pParams->m_NumIoctls>0 && pParams->m_pIoctls!=NULL);
    pStub->m_NumIoctls = pParams->m_NumIoctls;
    pStub->m_pIoctls = pParams->m_pIoctls;

    // Check IOCTL commands are listed in assending order starting at 0. Exception 
    // is a single command which may be a NOP_CMD
#ifdef DEBUG
    for (i=0; i<pStub->m_NumIoctls; i++)
    {
        Int  n=0, m=0;
        const DtIoctlProperties*  pIoCtlProps = &pStub->m_pIoctls[i];
        const Int  NumCmds = pIoCtlProps->m_NumCmds;
        DT_ASSERT(NumCmds >= 1);    // Must have atleast one command (maybe the NOP)
        DT_ASSERT(pIoCtlProps->m_pStr != NULL);
        for (n=0; n<NumCmds; n++)
        {
            const DtIoctlPropertiesCmd*  pCmdProps = &pStub->m_pIoctls[i].m_CmdProps[n];
            DT_ASSERT(pCmdProps->m_pStr != NULL);
            if (pCmdProps->m_Cmd!=n || (NumCmds==1 && pCmdProps->m_Cmd!=DT_IOCTL_CMD_NOP))
            {
                DtDbgOutIoStub(ERR, CORE, pStub, "ERROR: '%s->%s' command props not "
                                                  "sequential", 
                                                  pIoCtlProps->m_pStr, pCmdProps->m_pStr);
                DT_ASSERT(FALSE);
            }

            // Also check command-ex sequence
            const  Int  NumCmdExs = pCmdProps->m_NumCmdExs;
            DT_ASSERT((NumCmdExs==0 && pCmdProps->m_Cmd==DT_IOCTL_CMD_NOP) 
                                                   || pCmdProps->m_Cmd!=DT_IOCTL_CMD_NOP);
            for (m=0; m<NumCmdExs; m++)
            {
                const DtIoctlPropertiesCmdEx*  pCmdExProps = &pCmdProps->m_CmdExProps[m];
                DT_ASSERT(pCmdExProps->m_pStr != NULL);
                if (pCmdExProps->m_CmdEx != m)
                {
                    DtDbgOutIoStub(ERR, CORE, pStub, "ERROR: '%s->%s.%s' command-ex props"
                                                  "not sequential", 
                                                  pIoCtlProps->m_pStr, pCmdProps->m_pStr
                                                  pCmdExProps->m_pStr);
                    DT_ASSERT(FALSE);
                }
            }
        }
    }
#endif // #ifdef DEBUG
    
    return pStub;
}

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStub_Ioctl -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
DtStatus  DtIoStub_Ioctl(DtIoStub*  pStub, DtIoStubIoParams*  pIoParams, Int*  pOutSize)
{
    Int  i=0, j=0;
    DtStatus  Status=DT_STATUS_OK;
    Int  CheckFlags = DT_IOSTUB_CHECK_IOCTL;

    //.-.-.-.-.-.-.-.-.-.-.-.- Step 1: Check for supported IOCTL -.-.-.-.-.-.-.-.-.-.-.-.-

    for (i=0; i<pStub->m_NumIoctls; i++)
    {
       if (pStub->m_pIoctls[i].m_FunctionCode != pIoParams->m_pIoctl->m_FunctionCode)
            continue;
       // SANITY CHECK: make sure function code and IOCTL code are consistent
       DT_ASSERT(DT_IOCTL_TO_FUNCTION(pIoParams->m_pIoctl->m_IoctlCode) == 
                                                      pStub->m_pIoctls[i].m_FunctionCode);

        pIoParams->m_pIoctlProps = &pStub->m_pIoctls[i];
        break;
    }
    if (pIoParams->m_pIoctlProps == NULL)
    {
        // Report unknown IOCTL
        CheckFlags |= DT_IOSTUB_CHECK_REPORT;
        return DtIoStub_IoctlCheckAndReport(pStub, pIoParams, CheckFlags);
    }

    // Should we pass the IOCTL on to a child stub?
    if (pIoParams->m_pIoctlProps->m_GetChildStubFunc != NULL)
    {
        DtIoStub*  pChildStub = pIoParams->m_pIoctlProps->m_GetChildStubFunc(
                                                                        pStub, pIoParams);
        DT_ASSERT(pChildStub!=NULL && pChildStub->m_IoctlFunc!=NULL);
        if (pChildStub==NULL || pChildStub->m_IoctlFunc==NULL)
        {
            DtDbgOutIoStub(ERR, CORE, pStub, "ERROR: '%s' ioctl-function for child",
                                                        pIoParams->m_pIoctlProps->m_pStr);
            return DT_STATUS_NOT_IMPLEMENTED;
        }
        // Let the child stub handle the command
        return pChildStub->m_IoctlFunc(pChildStub, pIoParams, pOutSize);
    }
    
    // Must have a handler
    DT_ASSERT(pIoParams->m_pIoctlProps->m_OnIoctlFunc != NULL);
    if (pIoParams->m_pIoctlProps->m_OnIoctlFunc == NULL)
    {
        DtDbgOutIoStub(ERR, CORE, pStub, "ERROR: no ioctl-handler-function for '%s'",
                                                        pIoParams->m_pIoctlProps->m_pStr);
        return DT_STATUS_NOT_IMPLEMENTED;
    }

    //-.-.-.-.-.-.-.-.-.-.-.- Step 2: Check for supported command -.-.-.-.-.-.-.-.-.-.-.-.

    // Does the IOCTL have a command?
    if (pIoParams->m_pIoctlProps->m_NumCmds > 0)
        CheckFlags |= DT_IOSTUB_CHECK_CMD;  // Has command

    for (i=0; i<pIoParams->m_pIoctlProps->m_NumCmds; i++)
    {
        const DtIoctlPropertiesCmd*  pCmdProps = &pIoParams->m_pIoctlProps->m_CmdProps[i];
        if (pIoParams->m_Cmd != pCmdProps->m_Cmd)
            continue;
        pIoParams->m_pCmdProps = pCmdProps;

        // Init required size if no extended command
        if (pCmdProps->m_NumCmdExs == 0)
        {
            pIoParams->m_InReqSize = abs(pCmdProps->m_InReqSize);
            pIoParams->m_InReqSizeIsDynamic = (pCmdProps->m_InReqSize < 0);
            pIoParams->m_OutReqSize = abs(pCmdProps->m_OutReqSize);
            pIoParams->m_OutReqSizeIsDynamic = (pCmdProps->m_OutReqSize < 0);
            pIoParams->m_ExclAccessIsRequired = pCmdProps->m_ExclAccessIsRequired;
            
            CheckFlags |= DT_IOSTUB_CHECK_SIZE;  // Have our sizes

            break;      // And we are done
        }

        CheckFlags |= DT_IOSTUB_CHECK_CMDEX;  // Has command-ex
        
        for (j=0; j<pCmdProps->m_NumCmdExs; j++)
        {
            const DtIoctlPropertiesCmdEx*  pCmdExProps = &pCmdProps->m_CmdExProps[j];
            if (pIoParams->m_CmdEx != pCmdExProps->m_CmdEx)
                continue;
            pIoParams->m_pCmdExProps = pCmdExProps;

            // Overwrite sizes with extended command size
            pIoParams->m_InReqSize = abs(pCmdProps->m_InReqSize);
            pIoParams->m_InReqSizeIsDynamic = (pCmdProps->m_InReqSize < 0);
            pIoParams->m_OutReqSize = abs(pCmdProps->m_OutReqSize);
            pIoParams->m_OutReqSizeIsDynamic = (pCmdProps->m_OutReqSize < 0);
            pIoParams->m_ExclAccessIsRequired = pCmdProps->m_ExclAccessIsRequired;

            CheckFlags |= DT_IOSTUB_CHECK_SIZE;  // Have our sizes
            break;
        }
        break;
    }
    if (pIoParams->m_pCmdProps==NULL && pIoParams->m_pIoctlProps->m_NumCmds>0)
    {
        // Report unknown command
        CheckFlags |= DT_IOSTUB_CHECK_REPORT;
        return DtIoStub_IoctlCheckAndReport(pStub, pIoParams, CheckFlags);
    }

    //-.-.-.-.-.-.-.-.-.-.-.- Step 3: Check for pending power down -.-.-.-.-.-.-.-.-.-.-.-

    // Should we skip the command on power-down
    DT_ASSERT(pIoParams->m_pIoctlProps!=NULL && pIoParams->m_pIoctlProps->m_pStr!=NULL);
    DT_ASSERT(pIoParams->m_pCmdProps!=NULL && pIoParams->m_pCmdProps->m_pStr!=NULL);
    if (pIoParams->m_PowerDownPending && pIoParams->m_pCmdProps->m_SkipOnPowerDown)
    {
        DtDbgOutIoStub(ERR, CORE, pStub, "'%s->%s': Skipping because powerdown occured!",
                                                         pIoParams->m_pIoctlProps->m_pStr,
                                                         pIoParams->m_pCmdProps->m_pStr);
        return DT_STATUS_POWERDOWN;
    }

    //.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 4: check size -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    // Do we have a command with a dynamic size?
    if (pIoParams->m_InReqSizeIsDynamic || pIoParams->m_OutReqSizeIsDynamic)
    {
        // First check static sizes are OK
        Status = DtIoStub_IoctlCheckAndReport(pStub, pIoParams, DT_IOSTUB_CHECK_SIZE);
        if (!DT_SUCCESS(Status))
            return Status;

        // Append the dynamic size
        DT_ASSERT(pIoParams->m_pIoctlProps->m_AppendDynamicSizeFunc != NULL);
        if (pIoParams->m_pIoctlProps->m_AppendDynamicSizeFunc == NULL)
        {
            DtDbgOutIoStub(ERR, CORE, pStub, "ERROR: no dyn-size-function for '%s->%s'",
                                                         pIoParams->m_pIoctlProps->m_pStr,
                                                         pIoParams->m_pCmdProps->m_pStr);
            return DT_STATUS_FAIL;
        }
        Status = pIoParams->m_pIoctlProps->m_AppendDynamicSizeFunc(pStub, pIoParams);
        if (!DT_SUCCESS(Status))
            return Status;
    }

    
    // Check and report IOCTL-command
    CheckFlags |= DT_IOSTUB_CHECK_REPORT;
    Status = DtIoStub_IoctlCheckAndReport(pStub, pIoParams, CheckFlags);
    if (!DT_SUCCESS(Status))
        return Status;
    DT_ASSERT(pIoParams->m_InReqSize>=0 && pIoParams->m_OutReqSize>=0);

    // Finaly call the IOCTL handler. But first, set default output size
    *pOutSize = pIoParams->m_OutReqSize;    // Handler, must update size if needed
    return pIoParams->m_pIoctlProps->m_OnIoctlFunc(pStub, pIoParams, pOutSize);
}

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStub_IoctlCheckAndReport -.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
DtStatus  DtIoStub_IoctlCheckAndReport(const DtIoStub*  pStub, 
                                                       const DtIoStubIoParams*  pIoParams,
                                                       Int  Flags)
{
    DtStatus  Status = DT_STATUS_OK;
    Int  InReqSize=-1, OutReqSize=-1;
    const DtIoctlObject*  pIoctl=NULL;
    const char*  pIoctlStr = NULL;
    const char*  pCmdStr = NULL;
    const char*  pCmdExStr = NULL;
            
    DT_ASSERT(pStub!=NULL && pStub->m_Size>=sizeof(DtIoStub));
    DT_ASSERT(pIoParams != NULL);

    pIoctl = pIoParams->m_pIoctl;
    InReqSize = pIoParams->m_InReqSize;
    DT_ASSERT(InReqSize >= -1);     // NOTE: -1=uninitialised
    OutReqSize = pIoParams->m_OutReqSize;
    DT_ASSERT(OutReqSize >= -1);    // NOTE: -1=uninitialised
    pIoctlStr = (pIoParams->m_pIoctlProps != NULL) ? pIoParams->m_pIoctlProps->m_pStr 
                                                                                   : NULL;
    pCmdStr = (pIoParams->m_pCmdProps != NULL) ? pIoParams->m_pCmdProps->m_pStr : NULL;
    pCmdExStr = (pIoParams->m_pCmdExProps != NULL) ? pIoParams->m_pCmdExProps->m_pStr 
                                                                                   : NULL;

    //-.-.-.-.-.-.-.-.-.-.-.- Step 1: check we have a known IOCTL -.-.-.-.-.-.-.-.-.-.-.-.

    if ((Flags&DT_IOSTUB_CHECK_IOCTL)!=0 && pIoctlStr==NULL)
    {
        DtDbgOutIoStub(ERR, CORE, pStub, "UNKNOWN IOCTL: 0x%08X, FC=%d",
                                             pIoctl->m_IoctlCode, pIoctl->m_FunctionCode);
        return DT_STATUS_NOT_SUPPORTED;
    }
    
    //.-.-.-.-.-.-.-.-.-.-.-.- Step 2: check for a known command -.-.-.-.-.-.-.-.-.-.-.-.-

    if ((Flags&DT_IOSTUB_CHECK_CMD)!=0 && pCmdStr==NULL)
    {
        DtDbgOutIoStub(ERR, CORE, pStub, "UNKNOWN CMD: %s.%d", 
                                                             pIoctlStr, pIoParams->m_Cmd);
        return DT_STATUS_NOT_SUPPORTED;
    }

    // Does the command have an extended-command
    if ((Flags&DT_IOSTUB_CHECK_CMDEX)!=0 && pIoParams->m_CmdEx!=DT_IOCTL_CMD_NOP 
                                                                       && pCmdExStr==NULL)
    {
        DtDbgOutIoStub(ERR, CORE, pStub, "UNKNOWN CMDEX: %s.%s.%d",
                                                  pIoctlStr, pCmdStr, pIoParams->m_CmdEx);
        return DT_STATUS_NOT_SUPPORTED;
    }
    //-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Step 3: check sizes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

    if ((Flags&DT_IOSTUB_CHECK_INSIZE)!=0 && (Int)pIoctl->m_InputBufferSize<InReqSize)
    {
        DtDbgOutIoStub(ERR, CORE, pStub, "IOCTL=%s", pIoctlStr);
        DtDbgOutIoStub(ERR, CORE, pStub, "Cmd=%s%s%s, In=%d (Rq=%d) Out=%d (Rq=%d) INPUT "
                                                  "BUFFER TOO SMALL!",
                                                  pCmdStr,
                                                  (pCmdExStr!=NULL) ? "." : "",
                                                  (pCmdExStr!=NULL) ? pCmdExStr : "",
                                                  pIoctl->m_InputBufferSize, InReqSize,
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
        return DT_STATUS_INVALID_PARAMETER;
    }
    if ((Flags&DT_IOSTUB_CHECK_OUTSIZE)!=0 && (Int)pIoctl->m_OutputBufferSize<OutReqSize) 
    {
        DtDbgOutIoStub(ERR, CORE, pStub, "IOCTL=%s", pIoctlStr);
        DtDbgOutIoStub(ERR, CORE, pStub, "Cmd=%s%s%s, In=%d (Rq=%d) Out=%d (Rq=%d) "
                                                  "OUTPUT BUFFER TOO SMALL!",
                                                  pCmdStr,
                                                  (pCmdExStr!=NULL) ? "." : "",
                                                  (pCmdExStr!=NULL) ? pCmdExStr : "",
                                                  pIoctl->m_InputBufferSize, InReqSize,
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
        return DT_STATUS_INVALID_PARAMETER;
    }
    
    // Should we report which command was received?
    if ((Flags&DT_IOSTUB_CHECK_REPORT)!=0 && DT_SUCCESS(Status))
    {
        DtDbgOutIoStub(MAX, CORE, pStub, "IOCTL=%s",pIoctlStr);
        DtDbgOutIoStub(MAX, CORE, pStub, "Cmd=%s%s%s, In=%d (Rq=%d) Out=%d (Rq=%d)",
                                                  pCmdStr,
                                                  (pCmdExStr!=NULL) ? "." : "",
                                                  (pCmdExStr!=NULL) ? pCmdExStr : "",
                                                  pIoctl->m_InputBufferSize, InReqSize,
                                                  pIoctl->m_OutputBufferSize, OutReqSize);
    }
    return DT_STATUS_OK;
}
