// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfMbcReboot.h *#*#*#*#*#*#*#*#*#* (C) 2024 DekTec
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

#ifndef __DT_DF_MBCREBOOT_H
#define __DT_DF_MBCREBOOT_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcFMBC.h"

// Driver function that reboots the FPGA through the FPGA-Mailbox Client.

// Name + short-name for the MBCREBOOT function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_MBCREBOOT_NAME          "MbcReboot"
#define DT_DF_MBCREBOOT_SHORTNAME     "MBCREBOOT"

// MACRO: to init an driver-function-ID for the MBCREBOOT-DF
#define DT_DF_MBCREBOOT_INIT_ID(ID, ROLE, INSTANCE, UUID)                                \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_MBCREBOOT_NAME, DT_DF_MBCREBOOT_SHORTNAME, ROLE,             \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfMbcReboot definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfMbcReboot -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
typedef struct  _DtDfMbcReboot
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;
    DtBcFMBC* m_pBcFmbc;            // FMBC block controller
    Bool  m_Reboot;                 // Reboot on disable children
    UInt64  m_ImageAddress;         // New image address on reboot

}  DtDfMbcReboot;

typedef struct  _DtDfMbcRebootStatus
{
    UInt64  m_CurrentImage;         // Current running application image
    UInt64  m_FailingImage;         // Offset of failing application image (0 = no error)
    UInt32  m_State;                // Reserved, for internal use only
    UInt32  m_Version;              // Reserved, for internal use only
    UInt32  m_ErrorLocation;        // Reserved, for internal use only
    UInt32  m_ErrorDetails;         // Reserved, for internal use only
    UInt32  m_RetryCounter;         // Reserved, for internal use only
} DtDfMbcRebootStatus;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfMbcReboot_Close(DtDf*);
DtDfMbcReboot* DtDfMbcReboot_Open(DtCore*, DtPt*, const char* pRole, Int  Instance,
                                                             Int  Uuid, Bool  CreateStub);
DtStatus  DtDfMbcReboot_Reboot(DtDfMbcReboot*, UInt64 Address);
DtStatus  DtDfMbcReboot_GetSptAddress(DtDfMbcReboot*, UInt64 *pAddress1,
                                                          UInt64 *pAddress2);
DtStatus  DtDfMbcReboot_GetStatus(DtDfMbcReboot*, DtDfMbcRebootStatus* pStatus);



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfMbcReboot definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcReboot -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an TEMPLATE Driver-Function
typedef struct _DtIoStubDfMbcReboot
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfMbcReboot;

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfMbcReboot public functions -.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubDfMbcReboot_Close(DtIoStub*);
DtIoStubDfMbcReboot* DtIoStubDfMbcReboot_Open(DtDf*);


#endif  // #ifndef __DT_DF_MBCREBOOT_H

