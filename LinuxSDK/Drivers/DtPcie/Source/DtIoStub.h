//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoStub.h *#*#*#*#*#*#*#*#*#*#*# (C) 2017 DekTec
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

#ifndef __DT_IO_STUB_H
#define __DT_IO_STUB_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtCommon.h"
#include "DtBcDfCommon.h"
#include "DtIoctlProperties.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStub definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Max size for name
#define DT_IOSTUB_NAME_MAX_SIZE     DT_BCORDF_NAME_MAX_SIZE

// Check and report flags
#define DT_IOSTUB_CHECK_IOCTL       0x0001
#define DT_IOSTUB_CHECK_CMD         0x0002
#define DT_IOSTUB_CHECK_CMDEX       0x0004
#define DT_IOSTUB_CHECK_PROP        (DT_IOSTUB_CHECK_IOCTL | DT_IOSTUB_CHECK_CMD  \
                                                                  | DT_IOSTUB_CHECK_CMDEX)
#define DT_IOSTUB_CHECK_INSIZE      0x0010
#define DT_IOSTUB_CHECK_OUTSIZE     0x0020
#define DT_IOSTUB_CHECK_SIZE        (DT_IOSTUB_CHECK_INSIZE|DT_IOSTUB_CHECK_OUTSIZE)
#define DT_IOSTUB_CHECK_ALL         (DT_IOSTUB_CHECK_SIZE|DT_IOSTUB_CHECK_PROP)
#define DT_IOSTUB_CHECK_REPORT      0x1000
// By default check size + ioctl and command + report
#define DT_IOSTUB_CHECK_DEFAULT     (DT_IOSTUB_CHECK_SIZE | DT_IOSTUB_CHECK_IOCTL    \
                                                                 | DT_IOSTUB_CHECK_CMD  \
                                                                 | DT_IOSTUB_CHECK_REPORT)

// Prefix IoStub debug logging with the stub name
#define DtDbgOutIoStub(Level, Module, pStub, Msg, ...)                                   \
            DtDbgOut(Level, Module, "[%s] " Msg, pStub->m_Name, ##__VA_ARGS__)
#define DtDbgOutIoStub_Prefix(Level, Prefix, Module, pStub, Msg, ...)                    \
            DtDbgOut_Prefix(Level, Prefix, Module, "[%s] " Msg, pStub->m_Name,           \
                                                                            ##__VA_ARGS__)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubIoParams -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//
struct  _DtIoStubIoParams
{
    // Raw ioctl data
    const DtIoctlObject*  m_pIoctl;
    const DtFileObject*  m_pFile;
    // 
    DtExclAccessObject  m_ExclAccessObj;
    // State info
    Bool  m_PowerDownPending;
    // Decoded io-control data
    Int  m_Uuid;
    Int  m_UuidIndex;
    Int  m_PortIndex;
    Int  m_Cmd;
    Int  m_CmdEx;
    const DtIoctlInputData*  m_pInData;  // IOCTL input data buffer
    DtIoctlOutputData*  m_pOutData; // IOCTL output data buffer

    // Information added by the IO-stub
    Bool  m_InReqSizeIsDynamic;
    Int  m_InReqSize;               // Required min-size for the input data
    Bool  m_OutReqSizeIsDynamic;
    Int  m_OutReqSize;              // Required min-size for the output data
    Bool  m_ExclAccessIsRequired;   // Exclusive access is required     
    // Ioctl properties
    const DtIoctlProperties*  m_pIoctlProps;
    const DtIoctlPropertiesCmd*  m_pCmdProps;
    const DtIoctlPropertiesCmdEx*  m_pCmdExProps;

};

////-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubCheckParams -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//// Parameters for the DtIoStubCheckFunc callback
//typedef struct _DtIoStubCheckParams
//{
//    DtIoStubIoParams  m_Ioctl;
//    Int  m_ReqInSize;
//    Int  m_ReqOutSize;
//    const char*  m_pIoctlStr;
//}  DtIoStubCheckParams;

// Ioctl stub callback functions
typedef void  (*DtIoStubCloseFunc)(DtIoStub*);
typedef DtStatus  (*DtIoStubIoctlFunc)(DtIoStub*, DtIoStubIoParams*, Int*  pOutSize);

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubOpenParams -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Common data for all io-stub open params
#define  DT_IOSTUB_INIT_OPEN_PARAMS_COMMON_DATA                                          \
    Int  m_Size;                    /* Size of the IO-stub struct */                     \
    DtObjectType  m_ObjectType;     /* Type of object */                                 \
    DtCore*  m_pCore;               /* Core driver services */                           \
    const char*  m_pName;           /* Friendly name of the io-stub */                   \
    Int  m_NumIoctls;           /* Number of IOCTLs the stub supports */                 \
    const DtIoctlProperties*  m_pIoctls;   /* Properties of supported IOCTLs */          \
    DtIoStubCloseFunc  m_CloseFunc; /* Callback for close-function */                    \
    DtIoStubIoctlFunc  m_IoctlFunc  /* Callback for ioctl-function */
// The struct
typedef struct _DtIoStubOpenParams
{
    DT_IOSTUB_INIT_OPEN_PARAMS_COMMON_DATA;
}  DtIoStubOpenParams;
// MACRO to initialise open parameters
#define DT_IOSTUB_INIT_OPEN_PARAMS(OP, STRUCT, CORE, NAME, CLOSE_F, IOCTL_F, IOCTLS)     \
do                                                                                       \
{                                                                                        \
    DtMemZero(&OP, sizeof(OP));                                                          \
    OP.m_Size = sizeof(STRUCT);                                                          \
    OP.m_ObjectType = DT_OBJECT_TYPE_IOSTUB;                                             \
    OP.m_pCore = CORE;                                                                   \
    OP.m_pName = NAME;                                                                   \
    OP.m_CloseFunc = CLOSE_F;                                                            \
    OP.m_IoctlFunc = IOCTL_F;                                                            \
    OP.m_NumIoctls = DT_SIZEOF_ARRAY(IOCTLS);                                            \
    OP.m_pIoctls = IOCTLS;                                                               \
}                                                                                        \
while (0)

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStub -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Common data for all io-control stubs
#define DT_IOSTUB_COMMON_DATA                                                            \
    DT_OBJECT_COMMON_DATA;      /* Derive from DtObject */                               \
    DtCore*  m_pCore;           /* Shortcut to core driver services */                   \
    char  m_Name[DT_IOSTUB_NAME_MAX_SIZE+1];  /* Friendly name of the stub */            \
    Int  m_NumIoctls;           /* Number of IOCTLs the stub supports */                 \
    const DtIoctlProperties*  m_pIoctls;   /* Properties of supported IOCTLs */          \
    /* Callback functions */                                                             \
    DtIoStubCloseFunc  m_CloseFunc;                                                      \
    DtIoStubIoctlFunc  m_IoctlFunc
// The struct
struct _DtIoStub
{
    DT_IOSTUB_COMMON_DATA;
};

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public DtIoStub functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStub_Close(DtIoStub*);
DtIoStub*  DtIoStub_Open(const DtIoStubOpenParams*);
DtStatus  DtIoStub_Ioctl(DtIoStub*, DtIoStubIoParams*, Int*  pOutSize);
DtStatus  DtIoStub_IoctlCheckAndReport(const DtIoStub*, const DtIoStubIoParams*, 
                                                                              Int  Flags);


#endif // #ifndef __DT_IO_STUB_H
