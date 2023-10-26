// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtIoctlProperties.h *#*#*#*#*#*# (C) 2017-2022 DekTec
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

#ifndef __DT_IOCTL_PROPERTIES_H
#define __DT_IOCTL_PROPERTIES_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtCommon.h"
#include "DtIoStub.h"

#define DT_IOCTL_DYNAMIC_SIZEOF(STRUCT)    -(Int)sizeof(STRUCT)

#define INIT_DT_IOCTL_CMDEX_PROPS(CMD, STR, INSZ, OUTSZ)                                 \
    {                                                                                    \
        CMD,                                                                             \
        STR,                                                                             \
        INSZ,                                                                            \
        OUTSZ                                                                            \
    }

#define INIT_DT_IOCTL_CMD_PROPS(CMD, STR, SKIP, EXCL, INSZ, OUTSZ)                       \
    {                                                                                    \
        CMD,                                                                             \
        STR,                                                                             \
        SKIP,                                                                            \
        EXCL,                                                                            \
        INSZ,                                                                            \
        OUTSZ,                                                                           \
        0, NULL                                                                          \
    }
#define INIT_DT_IOCTL_CMD_PROPS_EX(CMD, STR, SKIP, EXCL, CMDEXS)                         \
    {                                                                                    \
        CMD,                                                                             \
        STR,                                                                             \
        SKIP,                                                                            \
        EXCL,                                                                            \
        0,                                                                               \
        0,                                                                               \
        DT_SIZEOF_ARRAY(CMDEXS),                                                         \
        CMDEXS,                                                                          \
    }

#define INIT_DT_IOCTL_PROPS(IOCTL, STR, CMDS, ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)    \
    {                                                                                    \
        IOCTL,                                                                           \
        DT_IOCTL_TO_FUNCTION(IOCTL),                                                     \
        STR,                                                                             \
        DT_SIZEOF_ARRAY(CMDS),                                                           \
        CMDS,                                                                            \
        ON_IOCTL_FUNC,                                                                   \
        DYN_SZ_FUNC,                                                                     \
        CHILD_FUNC                                                                       \
    }



typedef DtStatus  (*DtIoctlOnIoctlFunc)(const DtIoStub*, DtIoStubIoParams*, Int*  pOutSize);
typedef DtStatus  (*DtIoctlAppendDynamicSize)(const DtIoStub*, DtIoStubIoParams*);
typedef DtIoStub*  (*DtIoctlGetChildStubFunc)(const DtIoStub*, const DtIoStubIoParams*);


typedef struct _DtIoctlPropertiesCmdEx
{
    const Int  m_CmdEx;
    const char*  m_pStr;
    const Int  m_InReqSize;
    const Int  m_OutReqSize;
} DtIoctlPropertiesCmdEx;

typedef struct _DtIoctlPropertiesCmd
{
    const Int  m_Cmd;
    const char*  m_pStr;
    const Bool  m_SkipOnPowerDown;
    const Bool  m_ExclAccessIsRequired;
    const Int  m_InReqSize;
    const Int  m_OutReqSize;
    const Int  m_NumCmdExs;
    const DtIoctlPropertiesCmdEx*  m_CmdExProps;
} DtIoctlPropertiesCmd;

typedef struct  _DtIoctlProperties
{
    const UInt32  m_IoctlCode;
    const UInt32  m_FunctionCode;
    const char*  m_pStr;
    const Int  m_NumCmds;
    const DtIoctlPropertiesCmd*  m_CmdProps;
    const DtIoctlOnIoctlFunc  m_OnIoctlFunc;
    const DtIoctlAppendDynamicSize  m_AppendDynamicSizeFunc;
    const DtIoctlGetChildStubFunc  m_GetChildStubFunc;
}  DtIoctlProperties;


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_ACCUFIFO_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_ACCUFIFO_GET_ACCU_PERIOD                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ACCUFIFO_CMD_GET_ACCU_PERIOD,                                                 \
        "GET_ACCU_PERIOD",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAccuFifoCmdGetAccuPeriodInput),                                    \
        sizeof(DtIoctlAccuFifoCmdGetAccuPeriodOutput))

#define DT_IOCTL_CMD_PROPS_ACCUFIFO_GET_MAX_NUM_MEASM                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ACCUFIFO_CMD_GET_MAX_NUM_MEASM,                                               \
        "GET_MAX_NUM_MEASM",                                                             \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAccuFifoCmdGetMaxNumMeasmInput),                                   \
        sizeof(DtIoctlAccuFifoCmdGetMaxNumMeasmOutput))

#define DT_IOCTL_CMD_PROPS_ACCUFIFO_GET_MEASM_STATUS                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ACCUFIFO_CMD_GET_MEASM_STATUS,                                                \
        "GET_MEASM_STATUS",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAccuFifoCmdGetMeasmStatusInput),                                   \
        sizeof(DtIoctlAccuFifoCmdGetMeasmStatusOutput))

#define DT_IOCTL_CMD_PROPS_ACCUFIFO_GET_NUM_MEASM                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ACCUFIFO_CMD_GET_NUM_MEASM,                                                   \
        "GET_NUM_MEASM",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAccuFifoCmdGetNumMeasmInput),                                      \
        sizeof(DtIoctlAccuFifoCmdGetNumMeasmOutput))

#define DT_IOCTL_CMD_PROPS_ACCUFIFO_READ_MEASMS                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ACCUFIFO_CMD_READ_MEASMS,                                                     \
        "READ_MEASMS",                                                                   \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlAccuFifoCmdReadMeasmsInput),                                       \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlAccuFifoCmdReadMeasmsOutput))

#define DT_IOCTL_CMD_PROPS_ACCUFIFO_RESTART_MEASM                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ACCUFIFO_CMD_RESTART_MEASM,                                                   \
        "SET_CHECK_DATA",                                                                \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAccuFifoCmdRestartMeasmInput),                                     \
        0)

#define DT_IOCTL_CMD_PROPS_ACCUFIFO_SET_ACCU_PERIOD                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ACCUFIFO_CMD_SET_ACCU_PERIOD,                                                 \
        "SET_ACCU_PERIOD",                                                               \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAccuFifoCmdSetAccuPeriodInput),                                    \
        0)

#define DT_IOCTL_CMD_PROPS_ACCUFIFO_SET_NUM_MEASM                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ACCUFIFO_CMD_SET_NUM_MEASM,                                                   \
        "SET_NUM_MEASM",                                                                 \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAccuFifoCmdSetNumMeasmInput),                                      \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_ACCUFIFO                                                 \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_ACCUFIFO[] =                   \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_ACCUFIFO_GET_ACCU_PERIOD,                                     \
        DT_IOCTL_CMD_PROPS_ACCUFIFO_GET_MAX_NUM_MEASM,                                   \
        DT_IOCTL_CMD_PROPS_ACCUFIFO_GET_MEASM_STATUS,                                    \
        DT_IOCTL_CMD_PROPS_ACCUFIFO_GET_NUM_MEASM,                                       \
        DT_IOCTL_CMD_PROPS_ACCUFIFO_READ_MEASMS,                                         \
        DT_IOCTL_CMD_PROPS_ACCUFIFO_RESTART_MEASM,                                       \
        DT_IOCTL_CMD_PROPS_ACCUFIFO_SET_ACCU_PERIOD,                                     \
        DT_IOCTL_CMD_PROPS_ACCUFIFO_SET_NUM_MEASM,                                       \
    }

#define DT_IOCTL_PROPS_ACCUFIFO_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)              \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_ACCUFIFO_CMD,                                                           \
        "DT_IOCTL_ACCUFIFO_CMD",                                                         \
        DT_IOCTL_CMD_PROPS_ACCUFIFO,                                                     \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_AD9789ITF_CMD +=+=+=+=+=+=+=+=+=+= +=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_AD9789ITF_GET_OPERATIONAL_MODE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_AD9789ITF_CMD_GET_OPERATIONAL_MODE,                                           \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAd9789ItfCmdGetOpModeInput),                                       \
        sizeof(DtIoctlAd9789ItfCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_AD9789ITF_SET_OPERATIONAL_MODE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_AD9789ITF_CMD_SET_OPERATIONAL_MODE,                                           \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAd9789ItfCmdSetOpModeInput),                                       \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_AD9789ITF                                                \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_AD9789ITF[] =                  \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_AD9789ITF_GET_OPERATIONAL_MODE,                               \
        DT_IOCTL_CMD_PROPS_AD9789ITF_SET_OPERATIONAL_MODE                                \
    }

#define DT_IOCTL_PROPS_AD9789ITF_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)             \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_AD9789ITF_CMD,                                                          \
        "DT_IOCTL_AD9789ITF_CMD",                                                        \
        DT_IOCTL_CMD_PROPS_AD9789ITF,                                                    \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_ASIRX_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_ASIRX_GET_OPERATIONAL_STATUS                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASIRX_CMD_GET_OPERATIONAL_STATUS,                                             \
        "GET_OPERATIONAL_STATUS",                                                        \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAsiRxCmdGetOpStatusInput),                                         \
        sizeof(DtIoctlAsiRxCmdGetOpStatusOutput))

#define DT_IOCTL_CMD_PROPS_ASIRX_GET_PACKET_MODE                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASIRX_CMD_GET_PACKET_MODE,                                                    \
        "GET_PACKET_MODE",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAsiRxCmdGetPckModeInput),                                          \
        sizeof(DtIoctlAsiRxCmdGetPckModeOutput))

#define DT_IOCTL_CMD_PROPS_ASIRX_GET_POLARITY_CTRL                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASIRX_CMD_GET_POLARITY_CTRL,                                                  \
        "GET_POLARITY_CTRL",                                                             \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAsiRxCmdGetPolarityCtrlInput),                                     \
        sizeof(DtIoctlAsiRxCmdGetPolarityCtrlOutput))

#define DT_IOCTL_CMD_PROPS_ASIRX_GET_STATUS                                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASIRX_CMD_GET_STATUS,                                                         \
        "GET_STATUS",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAsiRxCmdGetStatusInput),                                           \
        sizeof(DtIoctlAsiRxCmdGetStatusOutput))

#define DT_IOCTL_CMD_PROPS_ASIRX_GET_SYNC_MODE                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASIRX_CMD_GET_SYNC_MODE,                                                      \
        "GET_SYNC_MODE",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAsiRxCmdGetSyncModeInput),                                         \
        sizeof(DtIoctlAsiRxCmdGetSyncModeOutput))

#define DT_IOCTL_CMD_PROPS_ASIRX_GET_TS_BITRATE                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASIRX_CMD_GET_TS_BITRATE,                                                     \
        "GET_TS_BITRATE",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAsiRxCmdGetTsBitrateInput),                                        \
        sizeof(DtIoctlAsiRxCmdGetTsBitrateOutput))


#define DT_IOCTL_CMD_PROPS_ASIRX_GET_VIOL_COUNT                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASIRX_CMD_GET_VIOL_COUNT,                                                     \
        "GET_VIOL_COUNT",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAsiRxCmdGetViolCountInput),                                        \
        sizeof(DtIoctlAsiRxCmdGetViolCountOutput))

#define DT_IOCTL_CMD_PROPS_ASIRX_SET_OPERATIONAL_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASIRX_CMD_SET_OPERATIONAL_MODE,                                               \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAsiRxCmdSetOpModeInput),                                           \
        0)

#define DT_IOCTL_CMD_PROPS_ASIRX_SET_PACKET_MODE                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASIRX_CMD_SET_PACKET_MODE,                                                    \
        "SET_PACKET_MODE",                                                               \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAsiRxCmdSetPckModeInput),                                          \
        0)

#define DT_IOCTL_CMD_PROPS_ASIRX_SET_POLARITY_CTRL                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASIRX_CMD_SET_POLARITY_CTRL,                                                  \
        "SET_POLARITY_CTRL",                                                             \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAsiRxCmdSetPolarityCtrlInput),                                     \
        0)

#define DT_IOCTL_CMD_PROPS_ASIRX_SET_SYNC_MODE                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASIRX_CMD_SET_SYNC_MODE,                                                      \
        "SET_SYNC_MODE",                                                                 \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAsiRxCmdSetSyncModeInput),                                         \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_ASIRX                                                    \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_ASIRX[] =                      \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_ASIRX_GET_OPERATIONAL_STATUS,                                 \
        DT_IOCTL_CMD_PROPS_ASIRX_GET_PACKET_MODE,                                        \
        DT_IOCTL_CMD_PROPS_ASIRX_GET_POLARITY_CTRL,                                      \
        DT_IOCTL_CMD_PROPS_ASIRX_GET_STATUS,                                             \
        DT_IOCTL_CMD_PROPS_ASIRX_GET_SYNC_MODE,                                          \
        DT_IOCTL_CMD_PROPS_ASIRX_GET_TS_BITRATE,                                         \
        DT_IOCTL_CMD_PROPS_ASIRX_GET_VIOL_COUNT,                                         \
        DT_IOCTL_CMD_PROPS_ASIRX_SET_OPERATIONAL_MODE,                                   \
        DT_IOCTL_CMD_PROPS_ASIRX_SET_PACKET_MODE,                                        \
        DT_IOCTL_CMD_PROPS_ASIRX_SET_POLARITY_CTRL,                                      \
        DT_IOCTL_CMD_PROPS_ASIRX_SET_SYNC_MODE,                                          \
    }

#define DT_IOCTL_PROPS_ASIRX_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                 \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_ASIRX_CMD,                                                              \
        "DT_IOCTL_ASIRX_CMD",                                                            \
        DT_IOCTL_CMD_PROPS_ASIRX,                                                        \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_ASITXG_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


#define DT_IOCTL_CMD_PROPS_ASITXG_CLEAR_INPUT_STATE                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASITXG_CMD_CLEAR_INPUT_STATE,                                                  \
        "CLEAR_INPUT_STATE",                                                              \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAsiTxGCmdClearInputStateInput),                                     \
        0)

#define DT_IOCTL_CMD_PROPS_ASITXG_GET_ASI_POLARITY                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASITXG_CMD_GET_ASI_POLARITY,                                                  \
        "GET_ASI_POLARITY",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAsiTxGCmdGetAsiPolarityInput),                                     \
        sizeof(DtIoctlAsiTxGCmdGetAsiPolarityOutput))

#define DT_IOCTL_CMD_PROPS_ASITXG_GET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASITXG_CMD_GET_OPERATIONAL_MODE,                                              \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAsiTxGCmdGetOpModeInput),                                          \
        sizeof(DtIoctlAsiTxGCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_ASITXG_SET_ASI_POLARITY                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASITXG_CMD_SET_ASI_POLARITY,                                                  \
        "SET_ASI_POLARITY",                                                              \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAsiTxGCmdSetAsiPolarityInput),                                     \
        0)

#define DT_IOCTL_CMD_PROPS_ASITXG_SET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASITXG_CMD_SET_OPERATIONAL_MODE,                                              \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlAsiTxGCmdSetOpModeInput),                                          \
        0)


#define DECL_DT_IOCTL_CMD_PROPS_ASITXG                                                   \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_ASITXG[] =                     \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_ASITXG_CLEAR_INPUT_STATE,                                     \
        DT_IOCTL_CMD_PROPS_ASITXG_GET_ASI_POLARITY,                                      \
        DT_IOCTL_CMD_PROPS_ASITXG_GET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_ASITXG_SET_ASI_POLARITY,                                      \
        DT_IOCTL_CMD_PROPS_ASITXG_SET_OPERATIONAL_MODE,                                  \
    }

#define DT_IOCTL_PROPS_ASITXG_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_ASITXG_CMD,                                                             \
        "DT_IOCTL_ASITXG_CMD",                                                           \
        DT_IOCTL_CMD_PROPS_ASITXG,                                                       \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_ASITXSER_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_ASITXSER_GET_OPERATIONAL_MODE                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASITXSER_CMD_GET_OPERATIONAL_MODE,                                            \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAsiTxSerCmdGetOpModeInput),                                        \
        sizeof(DtIoctlAsiTxSerCmdGetOpModeOutput))


#define DT_IOCTL_CMD_PROPS_ASITXSER_SET_OPERATIONAL_MODE                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ASITXSER_CMD_SET_OPERATIONAL_MODE,                                            \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAsiTxSerCmdSetOpModeInput),                                        \
        0)


#define DECL_DT_IOCTL_CMD_PROPS_ASITXSER                                                 \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_ASITXSER[] =                   \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_ASITXSER_GET_OPERATIONAL_MODE,                                \
        DT_IOCTL_CMD_PROPS_ASITXSER_SET_OPERATIONAL_MODE,                                \
    }

#define DT_IOCTL_PROPS_ASITXSER_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)              \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_ASITXSER_CMD,                                                           \
        "DT_IOCTL_ASITXSER_CMD",                                                         \
        DT_IOCTL_CMD_PROPS_ASITXSER,                                                     \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_BURSTFIFO_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_BURSTFIFO_CLEAR_FIFO_MAX                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_BURSTFIFO_CMD_CLEAR_FIFO_MAX,                                                 \
        "CLEAR_FIFO_MAX",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlBurstFifoCmdClearFifoMaxInput),                                    \
        0)

#define DT_IOCTL_CMD_PROPS_BURSTFIFO_GET_DIRECTION                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_BURSTFIFO_CMD_GET_DIRECTION,                                                  \
        "GET_DIRECTION",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlBurstFifoCmdGetDirectionInput),                                    \
        sizeof(DtIoctlBurstFifoCmdGetDirectionOutput))


#define DT_IOCTL_CMD_PROPS_BURSTFIFO_GET_FIFO_STATUS                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_BURSTFIFO_CMD_GET_FIFO_STATUS,                                                \
        "GET_FIFO_STATUS",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlBurstFifoCmdGetFifoStatusInput),                                   \
        sizeof(DtIoctlBurstFifoCmdGetFifoStatusOutput))


#define DT_IOCTL_CMD_PROPS_BURSTFIFO_GET_OPERATIONAL_MODE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_BURSTFIFO_CMD_GET_OPERATIONAL_MODE,                                           \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlBurstFifoCmdGetOpModeInput),                                       \
        sizeof(DtIoctlBurstFifoCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_BURSTFIFO_GET_OVFL_UFL_COUNT                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_BURSTFIFO_CMD_GET_OVFL_UFL_COUNT,                                             \
        "GET_OVFL_UFL_COUNT",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlBurstFifoCmdGetOvfUflCountInput),                                  \
        sizeof(DtIoctlBurstFifoCmdGetOvfUflCountOutput))

#define DT_IOCTL_CMD_PROPS_BURSTFIFO_GET_PROPERTIES                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_BURSTFIFO_CMD_GET_PROPERTIES,                                                 \
        "GET_PROPERTIES",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlBurstFifoCmdGetPropertiesInput),                                   \
        sizeof(DtIoctlBurstFifoCmdGetPropertiesOutput))

#define DT_IOCTL_CMD_PROPS_BURSTFIFO_GET_RXFLOWCONTROL                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_BURSTFIFO_CMD_GET_RXFLOWCONTROL,                                              \
        "GET_PROPERTIES",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlBurstFifoCmdGetRxFlowControlInput),                                \
        sizeof(DtIoctlBurstFifoCmdGetRxFlowControlOutput))

#define DT_IOCTL_CMD_PROPS_BURSTFIFO_SET_DIRECTION                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_BURSTFIFO_CMD_SET_DIRECTION,                                                  \
        "SET_DIRECTION",                                                                 \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlBurstFifoCmdSetDirectionInput),                                    \
        0)


#define DT_IOCTL_CMD_PROPS_BURSTFIFO_SET_OPERATIONAL_MODE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_BURSTFIFO_CMD_SET_OPERATIONAL_MODE,                                           \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlBurstFifoCmdSetOpModeInput),                                       \
        0)

#define DT_IOCTL_CMD_PROPS_BURSTFIFO_SET_RXFLOWCONTROL                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_BURSTFIFO_CMD_SET_RXFLOWCONTROL,                                              \
        "SET_RXFLOWCONTROL",                                                             \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlBurstFifoCmdSetRxFlowControlInput),                                \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_BURSTFIFO                                                \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_BURSTFIFO[] =                  \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_BURSTFIFO_CLEAR_FIFO_MAX,                                     \
        DT_IOCTL_CMD_PROPS_BURSTFIFO_GET_DIRECTION,                                      \
        DT_IOCTL_CMD_PROPS_BURSTFIFO_GET_FIFO_STATUS,                                    \
        DT_IOCTL_CMD_PROPS_BURSTFIFO_GET_OPERATIONAL_MODE,                               \
        DT_IOCTL_CMD_PROPS_BURSTFIFO_GET_OVFL_UFL_COUNT,                                 \
        DT_IOCTL_CMD_PROPS_BURSTFIFO_GET_PROPERTIES,                                     \
        DT_IOCTL_CMD_PROPS_BURSTFIFO_GET_RXFLOWCONTROL,                                  \
        DT_IOCTL_CMD_PROPS_BURSTFIFO_SET_DIRECTION,                                      \
        DT_IOCTL_CMD_PROPS_BURSTFIFO_SET_OPERATIONAL_MODE,                               \
        DT_IOCTL_CMD_PROPS_BURSTFIFO_GET_RXFLOWCONTROL,                                  \
    }

#define DT_IOCTL_PROPS_BURSTFIFO_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)             \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_BURSTFIFO_CMD,                                                          \
        "DT_IOCTL_BURSTFIFO_CMD",                                                        \
        DT_IOCTL_CMD_PROPS_BURSTFIFO,                                                    \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_DATAFIFO_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_DATAFIFO_CLEAR_FIFO_MAX                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DATAFIFO_CMD_CLEAR_FIFO_MAX,                                                  \
        "CLEAR_FIFO_MAX",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDataFifoCmdClearFifoMaxInput),                                     \
        0)

#define DT_IOCTL_CMD_PROPS_DATAFIFO_GET_DIRECTION                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DATAFIFO_CMD_GET_DIRECTION,                                                   \
        "GET_DIRECTION",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDataFifoCmdGetDirectionInput),                                     \
        sizeof(DtIoctlDataFifoCmdGetDirectionOutput))


#define DT_IOCTL_CMD_PROPS_DATAFIFO_GET_FIFO_STATUS                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DATAFIFO_CMD_GET_FIFO_STATUS,                                                 \
        "GET_FIFO_STATUS",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDataFifoCmdGetFifoStatusInput),                                    \
        sizeof(DtIoctlDataFifoCmdGetFifoStatusOutput))


#define DT_IOCTL_CMD_PROPS_DATAFIFO_GET_OPERATIONAL_MODE                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DATAFIFO_CMD_GET_OPERATIONAL_MODE,                                            \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDataFifoCmdGetOpModeInput),                                        \
        sizeof(DtIoctlDataFifoCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_DATAFIFO_GET_OVFL_COUNT                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DATAFIFO_CMD_GET_OVFL_COUNT,                                                  \
        "GET_OVFL_COUNT",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDataFifoCmdGetOvfCountInput),                                      \
        sizeof(DtIoctlDataFifoCmdGetOvfCountOutput))

#define DT_IOCTL_CMD_PROPS_DATAFIFO_GET_UFL_COUNT                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DATAFIFO_CMD_GET_UFL_COUNT,                                                   \
        "GET_UFL_COUNT",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDataFifoCmdGetUflCountInput),                                      \
        sizeof(DtIoctlDataFifoCmdGetUflCountOutput))


#define DT_IOCTL_CMD_PROPS_DATAFIFO_GET_PROPERTIES                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DATAFIFO_CMD_GET_PROPERTIES,                                                  \
        "GET_PROPERTIES",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDataFifoCmdGetPropertiesInput),                                    \
        sizeof(DtIoctlDataFifoCmdGetPropertiesOutput))


#define DT_IOCTL_CMD_PROPS_DATAFIFO_SET_DIRECTION                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DATAFIFO_CMD_SET_DIRECTION,                                                   \
        "SET_DIRECTION",                                                                 \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlDataFifoCmdSetDirectionInput),                                     \
        0)


#define DT_IOCTL_CMD_PROPS_DATAFIFO_SET_OPERATIONAL_MODE                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DATAFIFO_CMD_SET_OPERATIONAL_MODE,                                            \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlDataFifoCmdSetOpModeInput),                                        \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_DATAFIFO                                                 \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_DATAFIFO[] =                   \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_DATAFIFO_CLEAR_FIFO_MAX,                                      \
        DT_IOCTL_CMD_PROPS_DATAFIFO_GET_DIRECTION,                                       \
        DT_IOCTL_CMD_PROPS_DATAFIFO_GET_FIFO_STATUS,                                     \
        DT_IOCTL_CMD_PROPS_DATAFIFO_GET_OPERATIONAL_MODE,                                \
        DT_IOCTL_CMD_PROPS_DATAFIFO_GET_OVFL_COUNT,                                      \
        DT_IOCTL_CMD_PROPS_DATAFIFO_GET_UFL_COUNT,                                       \
        DT_IOCTL_CMD_PROPS_DATAFIFO_GET_PROPERTIES,                                      \
        DT_IOCTL_CMD_PROPS_DATAFIFO_SET_DIRECTION,                                       \
        DT_IOCTL_CMD_PROPS_DATAFIFO_SET_OPERATIONAL_MODE,                                \
    }

#define DT_IOCTL_PROPS_DATAFIFO_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)              \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_DATAFIFO_CMD,                                                           \
        "DT_IOCTL_DATAFIFO_CMD",                                                         \
        DT_IOCTL_CMD_PROPS_DATAFIFO,                                                     \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_DDRFIFO_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_DDRFIFO_GET_FIFO_LOAD                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DDRFIFO_CMD_GET_FIFO_LOAD,                                                    \
        "GET_FIFO_LOAD",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDdrFifoCmdGetFifoLoadInput),                                       \
        sizeof(DtIoctlDdrFifoCmdGetFifoLoadOutput))

#define DT_IOCTL_CMD_PROPS_DDRFIFO_GET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DDRFIFO_CMD_GET_OPERATIONAL_MODE,                                             \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDdrFifoCmdGetOpModeInput),                                         \
        sizeof(DtIoctlDdrFifoCmdGetOpModeOutput))


#define DT_IOCTL_CMD_PROPS_DDRFIFO_GET_PROPERTIES                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DDRFIFO_CMD_GET_PROPERTIES,                                                   \
        "GET_PROPERTIES",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDdrFifoCmdGetPropertiesInput),                                     \
        sizeof(DtIoctlDdrFifoCmdGetPropertiesOutput))


#define DT_IOCTL_CMD_PROPS_DDRFIFO_SET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DDRFIFO_CMD_SET_OPERATIONAL_MODE,                                             \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlDdrFifoCmdSetOpModeInput),                                         \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_DDRFIFO                                                  \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_DDRFIFO[] =                    \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_DDRFIFO_GET_FIFO_LOAD,                                        \
        DT_IOCTL_CMD_PROPS_DDRFIFO_GET_OPERATIONAL_MODE,                                 \
        DT_IOCTL_CMD_PROPS_DDRFIFO_GET_PROPERTIES,                                       \
        DT_IOCTL_CMD_PROPS_DDRFIFO_SET_OPERATIONAL_MODE,                                 \
    }

#define DT_IOCTL_PROPS_DDRFIFO_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)              \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_DDRFIFO_CMD,                                                           \
        "DT_IOCTL_DDRFIFO_CMD",                                                         \
        DT_IOCTL_CMD_PROPS_DDRFIFO,                                                     \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_CDMAC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_CDMAC_ALLOCATE_BUFFER                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CDMAC_CMD_ALLOCATE_BUFFER,                                                    \
        "ALLOCATE_BUFFER",                                                               \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlCDmaCCmdAllocateBufferInput),                                      \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlCDmaCCmdAllocateBufferOutput))

#define DT_IOCTL_CMD_PROPS_CDMAC_CLEAR_REORDER_BUF_MIN_MAX                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CDMAC_CMD_CLEAR_REORDER_BUF_MIN_MAX,                                          \
        "CLEAR_REORDER_BUF_MIN_MAX",                                                     \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlCDmaCCmdClearReorderBufMinMaxInput),                               \
        0)

#define DT_IOCTL_CMD_PROPS_CDMAC_CLEAR_PROFILING                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CDMAC_CMD_CLEAR_PROFILING,                                                    \
        "CLEAR_PROFILING",                                                               \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlCDmaCCmdClearProfiling),                                           \
        0)

#define DT_IOCTL_CMD_PROPS_CDMAC_FREE_BUFFER                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CDMAC_CMD_FREE_BUFFER,                                                        \
        "FREE_BUFFER",                                                                   \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlCDmaCCmdFreeBufferInput),                                          \
        0)

#define DT_IOCTL_CMD_PROPS_CDMAC_GET_REORDER_BUF_STATUS                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CDMAC_CMD_GET_REORDER_BUF_STATUS,                                             \
        "GET_REORDER_BUF_STATUS",                                                        \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlCDmaCCmdGetReorderBufStatusInput),                                 \
        sizeof(DtIoctlCDmaCCmdGetReorderBufStatusOutput))

#define DT_IOCTL_CMD_PROPS_CDMAC_GET_PROFILING                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CDMAC_CMD_GET_PROFILING,                                                      \
        "GET_PROPROFILING",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlCDmaCCmdGetProfilingInput),                                        \
        sizeof(DtIoctlCDmaCCmdGetProfilingOutput))

#define DT_IOCTL_CMD_PROPS_CDMAC_GET_PROPERTIES                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CDMAC_CMD_GET_PROPERTIES,                                                     \
        "GET_PROPERTIES",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlCDmaCCmdGetPropertiesInput),                                       \
        sizeof(DtIoctlCDmaCCmdGetPropertiesOutput))

#define DT_IOCTL_CMD_PROPS_CDMAC_GET_RX_WRITE_OFFSET                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CDMAC_CMD_GET_RX_WRITE_OFFSET,                                                \
        "GET_RX_WRITE_OFFSET",                                                           \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlCDmaCCmdGetRxWrOffsetInput),                                       \
        sizeof(DtIoctlCDmaCCmdGetRxWrOffsetOutput))

#define DT_IOCTL_CMD_PROPS_CDMAC_GET_STATUS                                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CDMAC_CMD_GET_STATUS,                                                         \
        "GET_STATUS",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlCDmaCCmdGetStatusInput),                                           \
        sizeof(DtIoctlCDmaCCmdGetStatusOutput))

#define DT_IOCTL_CMD_PROPS_CDMAC_GET_TX_READ_OFFSET                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CDMAC_CMD_GET_TX_READ_OFFSET,                                                 \
        "GET_TX_READ_OFFSET",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlCDmaCCmdGetTxRdOffsetInput),                                       \
        sizeof(DtIoctlCDmaCCmdGetTxRdOffsetOutput))

#define DT_IOCTL_CMD_PROPS_CDMAC_ISSUE_CHANNEL_FLUSH                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CDMAC_CMD_ISSUE_CHANNEL_FLUSH,                                                \
        "ISSUE_CHANNEL_FLUSH",                                                           \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlCDmaCCmdIssueChannelFlushInput),                                   \
        0)

#define DT_IOCTL_CMD_PROPS_CDMAC_SET_OPERATIONAL_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CDMAC_CMD_SET_OPERATIONAL_MODE,                                               \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlCDmaCCmdSetOpModeInput),                                           \
        0)

#define DT_IOCTL_CMD_PROPS_CDMAC_SET_RX_READ_OFFSET                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CDMAC_CMD_SET_RX_READ_OFFSET,                                                 \
        "SET_RX_READ_OFFSET",                                                            \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlCDmaCCmdSetRxRdOffsetInput),                                       \
        0)

#define DT_IOCTL_CMD_PROPS_CDMAC_SET_TEST_MODE                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CDMAC_CMD_SET_TEST_MODE,                                                      \
        "SET_TEST_MODE",                                                                 \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlCDmaCCmdSetTestModeInput),                                         \
        0)

#define DT_IOCTL_CMD_PROPS_CDMAC_SET_TX_WRITE_OFFSET                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CDMAC_CMD_SET_TX_WRITE_OFFSET,                                                \
        "SET_TX_WRITE_OFFSET",                                                           \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlCDmaCCmdSetTxWrOffsetInput),                                       \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_CDMAC                                                    \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_CDMAC[] =                      \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_CDMAC_ALLOCATE_BUFFER,                                        \
        DT_IOCTL_CMD_PROPS_CDMAC_CLEAR_REORDER_BUF_MIN_MAX,                              \
        DT_IOCTL_CMD_PROPS_CDMAC_FREE_BUFFER,                                            \
        DT_IOCTL_CMD_PROPS_CDMAC_GET_REORDER_BUF_STATUS,                                 \
        DT_IOCTL_CMD_PROPS_CDMAC_GET_PROFILING,                                          \
        DT_IOCTL_CMD_PROPS_CDMAC_GET_PROPERTIES,                                         \
        DT_IOCTL_CMD_PROPS_CDMAC_GET_RX_WRITE_OFFSET,                                    \
        DT_IOCTL_CMD_PROPS_CDMAC_GET_STATUS,                                             \
        DT_IOCTL_CMD_PROPS_CDMAC_GET_TX_READ_OFFSET,                                     \
        DT_IOCTL_CMD_PROPS_CDMAC_ISSUE_CHANNEL_FLUSH,                                    \
        DT_IOCTL_CMD_PROPS_CDMAC_SET_OPERATIONAL_MODE,                                   \
        DT_IOCTL_CMD_PROPS_CDMAC_SET_RX_READ_OFFSET,                                     \
        DT_IOCTL_CMD_PROPS_CDMAC_SET_TEST_MODE,                                          \
        DT_IOCTL_CMD_PROPS_CDMAC_SET_TX_WRITE_OFFSET,                                    \
        DT_IOCTL_CMD_PROPS_CDMAC_CLEAR_PROFILING,                                        \
    }

#define DT_IOCTL_PROPS_CDMAC_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                 \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_CDMAC_CMD,                                                              \
        "DT_IOCTL_CDMAC_CMD",                                                            \
        DT_IOCTL_CMD_PROPS_CDMAC,                                                        \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_DATAC_CMD +=+=+=+=+=+=+=+=+=+= +=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_DATAC_GET_OPERATIONAL_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DATAC_CMD_GET_OPERATIONAL_MODE,                                               \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDataCCmdGetOpModeInput),                                           \
        sizeof(DtIoctlDataCCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_DATAC_SET_OPERATIONAL_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DATAC_CMD_SET_OPERATIONAL_MODE,                                               \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlDataCCmdSetOpModeInput),                                           \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_DATAC                                                    \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_DATAC[] =                      \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_DATAC_GET_OPERATIONAL_MODE,                                   \
        DT_IOCTL_CMD_PROPS_DATAC_SET_OPERATIONAL_MODE                                    \
    }

#define DT_IOCTL_PROPS_DATAC_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                 \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_DATAC_CMD,                                                              \
        "DT_IOCTL_DATAC_CMD",                                                            \
        DT_IOCTL_CMD_PROPS_DATAC,                                                        \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_DATACDC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_DATACDC_GET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DATACDC_CMD_GET_OPERATIONAL_MODE,                                             \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDataCdcCmdGetOpModeInput),                                         \
        sizeof(DtIoctlDataCdcCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_DATACDC_SET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DATACDC_CMD_SET_OPERATIONAL_MODE,                                             \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlDataCdcCmdSetOpModeInput),                                         \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_DATACDC                                                  \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_DATACDC[] =                    \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_DATACDC_GET_OPERATIONAL_MODE,                                 \
        DT_IOCTL_CMD_PROPS_DATACDC_SET_OPERATIONAL_MODE                                  \
    }

#define DT_IOCTL_PROPS_DATACDC_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)               \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_DATACDC_CMD,                                                            \
        "DT_IOCTL_DATACDC_CMD",                                                          \
        DT_IOCTL_CMD_PROPS_DATACDC,                                                      \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_DDRFRONT_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// DDRFRONT ioctl for communicating with Network driver
#define DT_IOCTL_CMD_PROPS_DDRFRONT_READ_ADDRESS                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DDRFRONT_CMD_READ_ADDRESS,                                                    \
        "READ_ADDRESS",                                                                  \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDdrFrontCmdReadAddressInput),                                      \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlDdrFrontCmdReadAddressOutput))

#define DT_IOCTL_CMD_PROPS_DDRFRONT_WRITE_ADDRESS                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DDRFRONT_CMD_WRITE_ADDRESS,                                                   \
        "WRITE_ADDRESS",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlDdrFrontCmdWriteAddressInput),                    \
        0)

#define DT_IOCTL_CMD_PROPS_DDRFRONT_GET_PROPERTIES                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DDRFRONT_CMD_GET_PROPERTIES,                                                  \
        "GET_PROPERTIES",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDdrFrontCmdGetPropertiesInput),                                    \
        sizeof(DtIoctlDdrFrontCmdGetPropertiesOutput))

#define DECL_DT_IOCTL_CMD_PROPS_DDRFRONT                                                 \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_DDRFRONT[] =                   \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_DDRFRONT_READ_ADDRESS,                                        \
        DT_IOCTL_CMD_PROPS_DDRFRONT_WRITE_ADDRESS,                                       \
        DT_IOCTL_CMD_PROPS_DDRFRONT_GET_PROPERTIES,                                      \
    }

#define DT_IOCTL_PROPS_DDRFRONT_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)              \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_DDRFRONT_CMD,                                                           \
        "DT_IOCTL_DDRFRONT_CMD",                                                         \
        DT_IOCTL_CMD_PROPS_DDRFRONT,                                                     \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_DEBUG_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_DEBUG_REGISTER_READ                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DEBUG_CMD_REGISTER_READ,                                                      \
        "REGISTER_READ",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlDebugCmdRegReadInput),                            \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlDebugCmdRegReadOutput))

#define DT_IOCTL_CMD_PROPS_DEBUG_REGISTER_WRITE                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DEBUG_CMD_REGISTER_WRITE,                                                     \
        "REGISTER_WRITE",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlDebugCmdRegWriteInput),                           \
        0)

#define DT_IOCTL_CMD_PROPS_DEBUG_BULK_READ                                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DEBUG_CMD_BULK_READ,                                                          \
        "BULK_READ",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDebugCmdBulkReadInput),                                            \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlDebugCmdBulkReadOutput))

#define DT_IOCTL_CMD_PROPS_DEBUG_FORCE_RELEASE                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DEBUG_CMD_FORCE_RELEASE,                                                      \
        "FORCE_RELEASE",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDebugCmdForceReleaseInput),                                        \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_DEBUG                                                    \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_DEBUG[] =                      \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_DEBUG_BULK_READ,                                              \
        DT_IOCTL_CMD_PROPS_DEBUG_REGISTER_READ,                                          \
        DT_IOCTL_CMD_PROPS_DEBUG_REGISTER_WRITE,                                         \
        DT_IOCTL_CMD_PROPS_DEBUG_FORCE_RELEASE,                                          \
    }

#define DT_IOCTL_PROPS_DEBUG_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                 \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_DEBUG_CMD,                                                              \
        "DT_IOCTL_DEBUG_CMD",                                                            \
        DT_IOCTL_CMD_PROPS_DEBUG,                                                        \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_EMAC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// Generic MAC interface for 10G/25G/100G

#define DT_IOCTL_CMD_PROPS_EMAC_GET_OPERATIONAL_MODE                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_GET_OPERATIONAL_MODE,                                                \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlEMACCmdGetOpModeInput),                                            \
        sizeof(DtIoctlEMACCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_EMAC_SET_OPERATIONAL_MODE                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_SET_OPERATIONAL_MODE,                                                \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlEMACCmdSetOpModeInput),                                            \
        0)
#define DT_IOCTL_CMD_PROPS_EMAC_GET_MACADDRESS_PERM                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_GET_MACADDRESS_PERM,                                                 \
        "GET_MACADDRESS_PERM",                                                           \
        TRUE,                                                                            \
        FALSE,                                                                           \
        0,                                        \
        sizeof(DtIoctlEMACCmdGetMacAddressPermOutput))
#define DT_IOCTL_CMD_PROPS_EMAC_SET_MACADDRESS                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_SET_MACADDRESS,                                                      \
        "SET_MACADDRESS",                                                                \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlEMACCmdSetMacAddressInput),                                        \
        0)
#define DT_IOCTL_CMD_PROPS_EMAC_GET_MACADDRESS                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_GET_MACADDRESS,                                                      \
        "GET_MACADDRESS",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlEMACCmdGetMacAddressInput),                                        \
        sizeof(DtIoctlEMACCmdGetMacAddressOutput))
#define DT_IOCTL_CMD_PROPS_EMAC_GET_MAX_MULTICAST_LIST                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_GET_MAX_MULTICAST_LIST,                                              \
        "GET_MAX_MULTICAST_LIST",                                                        \
        TRUE,                                                                            \
        FALSE,                                                                           \
        0,                                                                               \
        sizeof(DtIoctlEMACCmdGetMaxMulticastListOutput))
#define DT_IOCTL_CMD_PROPS_EMAC_SET_MULTICAST_LIST                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_SET_MULTICAST_LIST,                                                  \
        "SET_MULTICAST_LIST",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlEMACCmdSetMulticastListInput),                                     \
        0)
#define DT_IOCTL_CMD_PROPS_EMAC_SET_PACKET_FILTER                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_SET_PACKET_FILTER,                                                   \
        "SET_PACKET_FILTER",                                                             \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlEMACCmdSetPacketFilterInput),                                      \
        0)
#define DT_IOCTL_CMD_PROPS_EMAC_GET_PACKET_FILTER                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_GET_PACKET_FILTER,                                                   \
        "GET_PACKET_FILTER",                                                             \
        TRUE,                                                                            \
        FALSE,                                                                           \
        0,                                                                               \
        sizeof(DtIoctlEMACCmdGetPacketFilterOutput))
#define DT_IOCTL_CMD_PROPS_EMAC_GET_COUNTER                                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_GET_COUNTER,                                                         \
        "GET_COUNTER",                                                                   \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlEMACCmdGetCounterInput),                                           \
        sizeof(DtIoctlEMACCmdGetCounterOutput))
#define DT_IOCTL_CMD_PROPS_EMAC_GET_LINKSTATUS                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_GET_LINKSTATUS,                                                      \
        "GET_LINKSTATUS",                                                                \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlEMACCmdGetLinkStatusInput),                                        \
        0)
#define DT_IOCTL_CMD_PROPS_EMAC_GET_PHY_SPEED                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_GET_PHY_SPEED,                                                       \
        "GET_PHY_SPEED",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        0,                                                                               \
        sizeof(DtIoctlEMACCmdGetPhySpeedOutput))
#define DT_IOCTL_CMD_PROPS_EMAC_SET_PHY_SPEED                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_SET_PHY_SPEED,                                                       \
        "SET_PHY_SPEED",                                                                 \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlEMACCmdSetPhySpeedInput),                                          \
        0)
#define DT_IOCTL_CMD_PROPS_EMAC_REGISTER_CALLBACK                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_REGISTER_CALLBACK,                                                   \
        "REGISTER_CALLBACK",                                                             \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlEMACCmdRegisterCallbackInput),                                     \
        0)
#define DT_IOCTL_CMD_PROPS_EMAC_UNREGISTER_CALLBACK                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_UNREGISTER_CALLBACK,                                                 \
        "UNREGISTER_CALLBACK",                                                           \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlEMACCmdUnregisterCallbackInput),                                   \
        0)
#define DT_IOCTL_CMD_PROPS_EMAC_RESET_NW_STATUS                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EMAC_CMD_RESET_NW_STATUS,                                                     \
        "RESET_NW_STATUS",                                                               \
        TRUE,                                                                            \
        TRUE,                                                                           \
        0,                                                                               \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_EMAC                                                     \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_EMAC[] =                       \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_EMAC_GET_OPERATIONAL_MODE,                                    \
        DT_IOCTL_CMD_PROPS_EMAC_SET_OPERATIONAL_MODE,                                    \
        DT_IOCTL_CMD_PROPS_EMAC_GET_MACADDRESS_PERM,                                     \
        DT_IOCTL_CMD_PROPS_EMAC_SET_MACADDRESS,                                          \
        DT_IOCTL_CMD_PROPS_EMAC_GET_MACADDRESS,                                          \
        DT_IOCTL_CMD_PROPS_EMAC_GET_MAX_MULTICAST_LIST,                                  \
        DT_IOCTL_CMD_PROPS_EMAC_SET_MULTICAST_LIST,                                      \
        DT_IOCTL_CMD_PROPS_EMAC_SET_PACKET_FILTER,                                       \
        DT_IOCTL_CMD_PROPS_EMAC_GET_PACKET_FILTER,                                       \
        DT_IOCTL_CMD_PROPS_EMAC_GET_COUNTER,                                             \
        DT_IOCTL_CMD_PROPS_EMAC_GET_LINKSTATUS,                                          \
        DT_IOCTL_CMD_PROPS_EMAC_GET_PHY_SPEED,                                           \
        DT_IOCTL_CMD_PROPS_EMAC_SET_PHY_SPEED,                                           \
        DT_IOCTL_CMD_PROPS_EMAC_REGISTER_CALLBACK,                                       \
        DT_IOCTL_CMD_PROPS_EMAC_UNREGISTER_CALLBACK,                                     \
        DT_IOCTL_CMD_PROPS_EMAC_RESET_NW_STATUS                                          \
    }

#define DT_IOCTL_PROPS_EMAC_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                  \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_EMAC_CMD,                                                               \
        "DT_IOCTL_EMAC_CMD",                                                             \
        DT_IOCTL_CMD_PROPS_EMAC,                                                         \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_DISEQC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_DISEQC_ENABLE_TONE                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DISEQC_CMD_ENABLE_TONE,                                                       \
        "ENABLE_TONE",                                                                   \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDiseqcCmd_EnableToneInput),                                        \
        0)

#define DT_IOCTL_CMD_PROPS_DISEQC_SEND_TONEBURST                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DISEQC_CMD_SEND_TONEBURST,                                                    \
        "SEND_TONEBURST",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDiseqcCmd_SendToneBurstInput),                                     \
        0)

#define DT_IOCTL_CMD_PROPS_DISEQC_SEND_MESSAGE                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DISEQC_CMD_SEND_MESSAGE,                                                      \
        "SEND_MESSAGE",                                                                  \
        TRUE,                                                                            \
        FALSE,                                                                           \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlDiseqcCmd_SendMessageInput),                      \
        0)

#define DT_IOCTL_CMD_PROPS_DISEQC_SEND_RECEIVE_MESSAGE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DISEQC_CMD_SEND_RECEIVE_MESSAGE,                                              \
        "SEND_RECEIVE_MESSAGE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlDiseqcCmd_SendReceiveMessageInput),               \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlDiseqcCmd_SendReceiveMessageOutput))


#define DECL_DT_IOCTL_CMD_PROPS_DISEQC                                                   \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_DISEQC[] =                     \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_DISEQC_ENABLE_TONE,                                           \
        DT_IOCTL_CMD_PROPS_DISEQC_SEND_TONEBURST,                                        \
        DT_IOCTL_CMD_PROPS_DISEQC_SEND_MESSAGE,                                          \
        DT_IOCTL_CMD_PROPS_DISEQC_SEND_RECEIVE_MESSAGE                                   \
    }

#define DT_IOCTL_PROPS_DISEQC_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_DISEQC_CMD,                                                             \
        "DT_IOCTL_DISEQC_CMD",                                                           \
        DT_IOCTL_CMD_PROPS_DISEQC,                                                       \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_EVENT_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_EVENT_GET                                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
         DT_EVENT_CMD_GET_EVENT,                                                         \
        "GET",                                                                           \
        FALSE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlEventCmdGetEventInput),                                            \
        sizeof(DtIoctlEventCmdGetEventOutput))

#define DT_IOCTL_CMD_PROPS_EVENT_REGISTER                                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EVENT_CMD_REGISTER,                                                           \
        "REGISTER",                                                                      \
        FALSE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlEventCmdRegisterEventInput),                                       \
        0)

#define DT_IOCTL_CMD_PROPS_EVENT_SET                                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EVENT_CMD_SET_EVENT,                                                          \
        "GET",                                                                           \
        FALSE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlEventCmdSetEventInput),                                            \
        0)

#define DT_IOCTL_CMD_PROPS_EVENT_UNREGISTER                                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EVENT_CMD_UNREGISTER,                                                         \
        "UNREGISTER",                                                                    \
        FALSE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlEventCmdUnregisterEventInput),                                     \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_EVENT                                                    \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_EVENT[] =                      \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_EVENT_GET,                                                    \
        DT_IOCTL_CMD_PROPS_EVENT_REGISTER,                                               \
        DT_IOCTL_CMD_PROPS_EVENT_SET,                                                    \
        DT_IOCTL_CMD_PROPS_EVENT_UNREGISTER,                                             \
    }

#define DT_IOCTL_PROPS_EVENT_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                 \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_EVENT_CMD,                                                              \
        "DT_IOCTL_EVENT_CMD",                                                            \
        DT_IOCTL_CMD_PROPS_EVENT,                                                        \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_EXCL_ACCESS_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_EXCL_ACCESS_ACQUIRE                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EXCLUSIVE_ACCESS_CMD_ACQUIRE,                                                 \
        "ACQUIRE",                                                                       \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlExclAccessCmdInput),                                               \
        0)

#define DT_IOCTL_CMD_PROPS_EXCL_ACCESS_RELEASE                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EXCLUSIVE_ACCESS_CMD_RELEASE,                                                 \
        "RELEASE",                                                                       \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlExclAccessCmdInput),                                               \
        0)

#define DT_IOCTL_CMD_PROPS_EXCL_ACCESS_PROBE                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EXCLUSIVE_ACCESS_CMD_PROBE,                                                   \
        "PROBE",                                                                         \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlExclAccessCmdInput),                                               \
        0)

#define DT_IOCTL_CMD_PROPS_EXCL_ACCESS_CHECK                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_EXCLUSIVE_ACCESS_CMD_CHECK,                                                   \
        "CHECK",                                                                         \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlExclAccessCmdInput),                                               \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_EXCL_ACCESS                                              \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_EXCL_ACCESS[] =                \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_EXCL_ACCESS_ACQUIRE,                                          \
        DT_IOCTL_CMD_PROPS_EXCL_ACCESS_RELEASE,                                          \
        DT_IOCTL_CMD_PROPS_EXCL_ACCESS_PROBE,                                            \
        DT_IOCTL_CMD_PROPS_EXCL_ACCESS_CHECK,                                            \
    }

#define DT_IOCTL_PROPS_EXCL_ACCESS_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)           \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_EXCL_ACCESS_CMD,                                                        \
        "DT_IOCTL_EXCL_ACCESS_CMD",                                                      \
        DT_IOCTL_CMD_PROPS_EXCL_ACCESS,                                                  \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_GENLOCKCTRL_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_GENLOCKCTRL_GET_STATE                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_GENLOCKCTRL_CMD_GET_STATE,                                                    \
        "GET_STATE",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlGenLockCtrlCmdGetStateInput),                                      \
        sizeof(DtIoctlGenLockCtrlCmdGetStateOutput))

#define DT_IOCTL_CMD_PROPS_GENLOCKCTRL_GET_STATE2                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_GENLOCKCTRL_CMD_GET_STATE2,                                                   \
        "GET_STATE2",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlGenLockCtrlCmdGetState2Input),                                     \
        sizeof(DtIoctlGenLockCtrlCmdGetState2Output))


#define DT_IOCTL_CMD_PROPS_GENLOCKCTRL_RELOCK                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_GENLOCKCTRL_CMD_RELOCK,                                                       \
        "RELOCK",                                                                        \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlGenLockCtrlCmdReLockInput),                                        \
        0)

#define DT_IOCTL_CMD_PROPS_GENLOCKCTRL_GET_DCO_CLK_PROPS                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_GENLOCKCTRL_CMD_GET_DCO_CLK_PROPS,                                            \
        "GET_DCO_CLK_PROPS",                                                             \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlGenLockCtrlCmdGetDcoClockPropsInput),                              \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlGenLockCtrlCmdGetDcoClockPropsOutput))

#define DT_IOCTL_CMD_PROPS_GENLOCKCTRL_GET_DCO_FREQ_OFFSET                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_GENLOCKCTRL_CMD_GET_DCO_FREQ_OFFSET,                                          \
        "GET_DCO_FREQ_OFFSET",                                                           \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlGenLockCtrlCmdGetDcoFreqOffsetInput),                              \
        sizeof(DtIoctlGenLockCtrlCmdGetDcoFreqOffsetOutput))

#define DT_IOCTL_CMD_PROPS_GENLOCKCTRL_SET_DCO_FREQ_OFFSET                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_GENLOCKCTRL_CMD_SET_DCO_FREQ_OFFSET,                                          \
        "SET_DCO_FREQ_OFFSET",                                                           \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlGenLockCtrlCmdSetDcoFreqOffsetInput),                              \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_GENLOCKCTRL                                              \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_GENLOCKCTRL[] =                \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_GENLOCKCTRL_GET_STATE,                                        \
        DT_IOCTL_CMD_PROPS_GENLOCKCTRL_GET_STATE2,                                       \
        DT_IOCTL_CMD_PROPS_GENLOCKCTRL_RELOCK,                                           \
        DT_IOCTL_CMD_PROPS_GENLOCKCTRL_GET_DCO_CLK_PROPS,                                \
        DT_IOCTL_CMD_PROPS_GENLOCKCTRL_GET_DCO_FREQ_OFFSET,                              \
        DT_IOCTL_CMD_PROPS_GENLOCKCTRL_SET_DCO_FREQ_OFFSET,                              \
    }

#define DT_IOCTL_PROPS_GENLOCKCTRL_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)           \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_GENLOCKCTRL_CMD,                                                        \
        "DT_IOCTL_GENLOCKCTRL_CMD",                                                      \
        DT_IOCTL_CMD_PROPS_GENLOCKCTRL,                                                  \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_GET_DEV_INFO +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define IOCTL_CMD_PROPS_GET_DEV_INFO                                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IOCTL_CMD_NOP,                                                                \
        "NULL",                                                                          \
        FALSE,                                                                           \
        FALSE,                                                                           \
        sizeof(DtIoctlGetDevInfoInput),                                                  \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlGetDevInfoCommonOutput))

#define DECL_DT_IOCTL_CMD_PROPS_GET_DEV_INFO                                             \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_GET_DEV_INFO[] =               \
    {                                                                                    \
        IOCTL_CMD_PROPS_GET_DEV_INFO,                                                    \
    }

#define DT_IOCTL_PROPS_GET_DEV_INFO(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)              \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_GET_DEV_INFO,                                                           \
        "DT_IOCTL_GET_DEV_INFO",                                                         \
        DT_IOCTL_CMD_PROPS_GET_DEV_INFO,                                                 \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_GET_DRIVER_VERSION +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define IOCTL_CMD_PROPS_GET_DRIVER_VERSION                                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IOCTL_CMD_NOP,                                                                \
        "NULL",                                                                          \
        FALSE,                                                                           \
        FALSE,                                                                           \
        sizeof(DtIoctlGetDriverVersionInput),                                            \
        sizeof(DtIoctlGetDriverVersionOutput))

#define DECL_DT_IOCTL_CMD_PROPS_GET_DRIVER_VERSION                                       \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_GET_DRIVER_VERSION[] =         \
    {                                                                                    \
        IOCTL_CMD_PROPS_GET_DRIVER_VERSION,                                              \
    }

#define DT_IOCTL_PROPS_GET_DRIVER_VERSION(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)        \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_GET_DRIVER_VERSION,                                                     \
        "DT_IOCTL_GET_DRIVER_VERSION",                                                   \
        DT_IOCTL_CMD_PROPS_GET_DRIVER_VERSION,                                           \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_GET_NWDRIVER_VERSION +=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define IOCTL_CMD_PROPS_GET_NWDRIVER_VERSION                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IOCTL_CMD_NOP,                                                                \
        "NULL",                                                                          \
        FALSE,                                                                           \
        FALSE,                                                                           \
        sizeof(DtIoctlGetNwDriverVersionInput),                                          \
        sizeof(DtIoctlGetNwDriverVersionOutput))

#define DECL_DT_IOCTL_CMD_PROPS_GET_NWDRIVER_VERSION                                     \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_GET_NWDRIVER_VERSION[] =       \
    {                                                                                    \
        IOCTL_CMD_PROPS_GET_NWDRIVER_VERSION,                                            \
    }

#define DT_IOCTL_PROPS_GET_NWDRIVER_VERSION(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)      \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_GET_NWDRIVER_VERSION,                                                   \
        "DT_IOCTL_GET_NWDRIVER_VERSION",                                                 \
        DT_IOCTL_CMD_PROPS_GET_NWDRIVER_VERSION,                                         \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SET_NWDRIVER_VERSION +=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define IOCTL_CMD_PROPS_SET_NWDRIVER_VERSION                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IOCTL_CMD_NOP,                                                                \
        "NULL",                                                                          \
        FALSE,                                                                           \
        FALSE,                                                                           \
        sizeof(DtIoctlSetNwDriverVersionInput),                                          \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_SET_NWDRIVER_VERSION                                     \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SET_NWDRIVER_VERSION[] =       \
    {                                                                                    \
        IOCTL_CMD_PROPS_SET_NWDRIVER_VERSION,                                            \
    }

#define DT_IOCTL_PROPS_SET_NWDRIVER_VERSION(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)      \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SET_NWDRIVER_VERSION,                                                   \
        "DT_IOCTL_SET_NWDRIVER_VERSION",                                                 \
        DT_IOCTL_CMD_PROPS_SET_NWDRIVER_VERSION,                                         \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_I2CM_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_I2CM_READ                                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_I2CM_CMD_READ,                                                                \
        "READ",                                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlI2cMCmdReadInput),                                                 \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlI2cMCmdReadOutput))

#define DT_IOCTL_CMD_PROPS_I2CM_WRITE                                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_I2CM_CMD_WRITE,                                                               \
        "WRITE",                                                                         \
        TRUE,                                                                            \
        TRUE,                                                                            \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlI2cMCmdWriteInput),                               \
        0)

#define DT_IOCTL_CMD_PROPS_I2CM_WRITE_READ                                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_I2CM_CMD_WRITE_READ,                                                          \
        "WRITE_READ",                                                                    \
        TRUE,                                                                            \
        TRUE,                                                                            \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlI2cMCmdWriteReadInput),                           \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlI2cMCmdWriteReadOutput))

#define DECL_DT_IOCTL_CMD_PROPS_I2CM                                                     \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_I2CM[] =                       \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_I2CM_READ,                                                    \
        DT_IOCTL_CMD_PROPS_I2CM_WRITE,                                                   \
        DT_IOCTL_CMD_PROPS_I2CM_WRITE_READ,                                              \
    }

#define DT_IOCTL_PROPS_I2CM_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                  \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_I2CM_CMD,                                                               \
        "DT_IOCTL_I2CM_CMD",                                                             \
        DT_IOCTL_CMD_PROPS_I2CM,                                                         \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IPSECG_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_IPSECG_GET_STATUS                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IPSECG_CMD_GET_STATUS,                                                        \
        "GET_STATUS",                                                                    \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIpSecGCmdGetStatusInput),                                          \
        sizeof(DtIoctlIpSecGCmdGetStatusOutput))

#define DT_IOCTL_CMD_PROPS_IPSECG_CHECK                                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IPSECG_CMD_CHECK,                                                             \
        "CHECK",                                                                         \
        TRUE,                                                                            \
        TRUE,                                                                            \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlIpSecGCmdCheckInput),                             \
        0)

#define DT_IOCTL_CMD_PROPS_IPSECG_CHALLENGE                                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IPSECG_CMD_CHALLENGE,                                                         \
        "CHALLENGE",                                                                     \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIpSecGCmdChallengeInput),                                          \
        sizeof(DtIoctlIpSecGCmdChallengeOutput))


#define DECL_DT_IOCTL_CMD_PROPS_IPSECG_CMD                                               \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IPSECG[] =                     \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IPSECG_GET_STATUS,                                            \
        DT_IOCTL_CMD_PROPS_IPSECG_CHECK,                                                 \
        DT_IOCTL_CMD_PROPS_IPSECG_CHALLENGE,                                             \
    }

#define DT_IOCTL_PROPS_IPSECG_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IPSECG_CMD,                                                             \
        "DT_IOCTL_IPSECG_CMD",                                                           \
        DT_IOCTL_CMD_PROPS_IPSECG,                                                       \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_IOCONFIG_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define IOCTL_CMD_PROPS_IOCONFIG_GET                                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IOCONFIG_CMD_GET_IOCONFIG,                                                    \
        "GET",                                                                           \
        TRUE,                                                                            \
        FALSE,                                                                           \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlIoConfigCmdGetIoConfigInput),                     \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlIoConfigCmdGetIoConfigOutput))

#define IOCTL_CMD_PROPS_IOCONFIG_SET                                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IOCONFIG_CMD_SET_IOCONFIG,                                                    \
        "SET",                                                                           \
        TRUE,                                                                            \
        FALSE,                                                                           \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlIoConfigCmdSetIoConfigInput),                     \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_IOCONFIG                                                 \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IOCONFIG[] =                   \
    {                                                                                    \
        IOCTL_CMD_PROPS_IOCONFIG_GET,                                                    \
        IOCTL_CMD_PROPS_IOCONFIG_SET,                                                    \
    }

#define DT_IOCTL_PROPS_IOCONFIG_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)              \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IOCONFIG_CMD,                                                           \
        "DT_IOCTL_IOCONFIG_CMD",                                                         \
        DT_IOCTL_CMD_PROPS_IOCONFIG,                                                     \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_NW_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// Nw ioctl for communicating with Network driver
#define DT_IOCTL_CMD_PROPS_NW_GET_OPERATIONAL_MODE                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_NW_CMD_GET_OPERATIONAL_MODE,                                                  \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlNwCmdGetOpModeInput),                                              \
        sizeof(DtIoctlNwCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_NW_SET_OPERATIONAL_MODE                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_NW_CMD_SET_OPERATIONAL_MODE,                                                  \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlNwCmdSetOpModeInput),                                              \
        0)

#define DT_IOCTL_CMD_PROPS_NW_PIPE_OPEN                                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_NW_CMD_PIPE_OPEN,                                                             \
        "PIPE_OPEN",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlNwCmdPipeOpenInput),                                               \
        sizeof(DtIoctlNwCmdPipeOpenOutput))

#define DT_IOCTL_CMD_PROPS_NW_PIPE_CLOSE                                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_NW_CMD_PIPE_CLOSE,                                                            \
        "PIPE_CLOSE",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlNwCmdPipeCloseInput),                                              \
        0)

#define DT_IOCTL_CMD_PROPS_NW_REGISTER_OPMODE_CALLBACK                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_NW_CMD_REGISTER_OPMODE_CALLBACK,                                              \
        "REGISTER_OPMODE_CALLBACK",                                                      \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlNwCmdRegisterOpModeCallbackInput),                                 \
        0)


#define DECL_DT_IOCTL_CMD_PROPS_NW                                                       \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_NW[] =                         \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_NW_GET_OPERATIONAL_MODE,                                      \
        DT_IOCTL_CMD_PROPS_NW_SET_OPERATIONAL_MODE,                                      \
        DT_IOCTL_CMD_PROPS_NW_PIPE_OPEN,                                                 \
        DT_IOCTL_CMD_PROPS_NW_PIPE_CLOSE,                                                \
        DT_IOCTL_CMD_PROPS_NW_REGISTER_OPMODE_CALLBACK,                                  \
    }

#define DT_IOCTL_PROPS_NW_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                    \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_NW_CMD,                                                                 \
        "DT_IOCTL_NW_CMD",                                                               \
        DT_IOCTL_CMD_PROPS_NW,                                                           \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_PIPE_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Nw ioctl for communicating with Network driver
#define DT_IOCTL_CMD_PROPS_PIPE_SET_SHARED_BUFFER                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PIPE_CMD_SET_SHARED_BUFFER,                                                   \
        "SET_SHARED_BUFFER",                                                             \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPipeCmdSetSharedBufferInput),                                      \
        0)

#define DT_IOCTL_CMD_PROPS_PIPE_SET_SHAREDK_BUFFER                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PIPE_CMD_SET_SHAREDK_BUFFER,                                                  \
        "SET_SHAREDK_BUFFER",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPipeCmdSetSharedKBufferInput),                                     \
        sizeof(DtIoctlPipeCmdSetSharedKBufferOutput))

#define DT_IOCTL_CMD_PROPS_PIPE_RELEASE_SHARED_BUFFER                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PIPE_CMD_RELEASE_SHARED_BUFFER,                                               \
        "RELEASE_SHARED_BUFFER",                                                         \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPipeCmdReleaseSharedBufferInput),                                  \
        0)

#define DT_IOCTL_CMD_PROPS_PIPE_GET_PROPERTIES                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PIPE_CMD_GET_PROPERTIES,                                                      \
        "GET_PROPERTIES",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPipeCmdGetPropertiesInput),                                        \
        sizeof(DtIoctlPipeCmdGetPropertiesOutput))

#define DT_IOCTL_CMD_PROPS_PIPE_GET_STATUS                                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PIPE_CMD_GET_STATUS,                                                          \
        "GET_STATUS",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPipeCmdGetStatusInput),                                            \
        sizeof(DtIoctlPipeCmdGetStatusOutput))

#define DT_IOCTL_CMD_PROPS_PIPE_ISSUE_PIPE_FLUSH                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PIPE_CMD_ISSUE_PIPE_FLUSH,                                                    \
        "ISSUE_PIPE_FLUSH",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPipeCmdIssuePipeFlushInput),                                       \
        0)

#define DT_IOCTL_CMD_PROPS_PIPE_SET_OPERATIONAL_MODE                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PIPE_CMD_SET_OPERATIONAL_MODE,                                                \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPipeCmdSetOpModeInput),                                            \
        0)

#define DT_IOCTL_CMD_PROPS_PIPE_SET_RX_READ_OFFSET                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PIPE_CMD_SET_RX_READ_OFFSET,                                                  \
        "SET_RX_READ_OFFSET",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPipeCmdSetRxReadOffsetInput),                                      \
        0)
#define DT_IOCTL_CMD_PROPS_PIPE_GET_RX_WRITE_OFFSET                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PIPE_CMD_GET_RX_WRITE_OFFSET,                                                 \
        "GET_RX_WRITE_OFFSET",                                                           \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPipeCmdGetRxWriteOffsetInput),                                     \
        sizeof(DtIoctlPipeCmdGetRxWriteOffsetOutput))
#define DT_IOCTL_CMD_PROPS_PIPE_SET_TX_WRITE_OFFSET                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PIPE_CMD_SET_TX_WRITE_OFFSET,                                                 \
        "SET_TX_WRITE_OFFSET",                                                           \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPipeCmdSetTxWriteOffsetInput),                                     \
        0)
#define DT_IOCTL_CMD_PROPS_PIPE_GET_TX_READ_OFFSET                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PIPE_CMD_GET_TX_READ_OFFSET,                                                  \
        "GET_TX_READ_OFFSET",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPipeCmdGetTxReadOffsetInput),                                      \
        sizeof(DtIoctlPipeCmdGetTxReadOffsetOutput))
#define DT_IOCTL_CMD_PROPS_PIPE_REGISTER_EVENT                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PIPE_CMD_REGISTER_EVENT,                                                      \
        "REGISTER_EVENT",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPipeCmdRegisterEventInput),                                        \
        0)
#define DT_IOCTL_CMD_PROPS_PIPE_DEREGISTER_EVENT                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PIPE_CMD_DEREGISTER_EVENT,                                                    \
        "DEREGISTER_EVENT",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPipeCmdDeregisterEventInput),                                      \
        0)
#define DT_IOCTL_CMD_PROPS_PIPE_SET_IP_FILTER                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PIPE_CMD_SET_IPFILTER,                                                       \
        "SET_IP_FILTER",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPipeCmdSetIpFilterInput),                                          \
        0)


#define DECL_DT_IOCTL_CMD_PROPS_PIPE                                                     \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_PIPE[] =                       \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_PIPE_SET_SHARED_BUFFER,                                       \
        DT_IOCTL_CMD_PROPS_PIPE_SET_SHAREDK_BUFFER,                                      \
        DT_IOCTL_CMD_PROPS_PIPE_RELEASE_SHARED_BUFFER,                                   \
        DT_IOCTL_CMD_PROPS_PIPE_GET_PROPERTIES,                                          \
        DT_IOCTL_CMD_PROPS_PIPE_GET_STATUS,                                              \
        DT_IOCTL_CMD_PROPS_PIPE_ISSUE_PIPE_FLUSH,                                        \
        DT_IOCTL_CMD_PROPS_PIPE_SET_OPERATIONAL_MODE,                                    \
        DT_IOCTL_CMD_PROPS_PIPE_SET_RX_READ_OFFSET,                                      \
        DT_IOCTL_CMD_PROPS_PIPE_GET_RX_WRITE_OFFSET,                                     \
        DT_IOCTL_CMD_PROPS_PIPE_SET_TX_WRITE_OFFSET,                                     \
        DT_IOCTL_CMD_PROPS_PIPE_GET_TX_READ_OFFSET,                                      \
        DT_IOCTL_CMD_PROPS_PIPE_REGISTER_EVENT,                                          \
        DT_IOCTL_CMD_PROPS_PIPE_DEREGISTER_EVENT,                                        \
        DT_IOCTL_CMD_PROPS_PIPE_SET_IP_FILTER,                                           \
    }

#define DT_IOCTL_PROPS_PIPE_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                 \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_PIPE_CMD,                                                              \
        "DT_IOCTL_PIPE_CMD",                                                            \
        DT_IOCTL_CMD_PROPS_PIPE,                                                        \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IOPARIN_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

#define DT_IOCTL_CMD_PROPS_IOPARIN_GET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IOPARIN_CMD_GET_OPERATIONAL_MODE,                                             \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIoParInCmdGetOpModeInput),                                         \
        sizeof(DtIoctlIoParInCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_IOPARIN_SET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IOPARIN_CMD_SET_OPERATIONAL_MODE,                                             \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIoParInCmdSetOpModeInput),                                         \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_IOPARIN                                                  \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IOPARIN[] =                    \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IOPARIN_GET_OPERATIONAL_MODE,                                 \
        DT_IOCTL_CMD_PROPS_IOPARIN_SET_OPERATIONAL_MODE,                                 \
    }

#define DT_IOCTL_PROPS_IOPARIN_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)               \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IOPARIN_CMD,                                                            \
        "DT_IOCTL_IOPARIN_CMD",                                                          \
        DT_IOCTL_CMD_PROPS_IOPARIN,                                                      \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IOSERIN_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

#define DT_IOCTL_CMD_PROPS_IOSERIN_GET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IOSERIN_CMD_GET_OPERATIONAL_MODE,                                             \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIoSerInCmdGetOpModeInput),                                         \
        sizeof(DtIoctlIoSerInCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_IOSERIN_SET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IOSERIN_CMD_SET_OPERATIONAL_MODE,                                             \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIoSerInCmdSetOpModeInput),                                         \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_IOSERIN                                                  \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IOSERIN[] =                    \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IOSERIN_GET_OPERATIONAL_MODE,                                 \
        DT_IOCTL_CMD_PROPS_IOSERIN_SET_OPERATIONAL_MODE,                                 \
    }

#define DT_IOCTL_PROPS_IOSERIN_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)               \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IOSERIN_CMD,                                                            \
        "DT_IOCTL_IOSERIN_CMD",                                                          \
        DT_IOCTL_CMD_PROPS_IOSERIN,                                                      \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_GPSTIME_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_GPSTIME_GET_STATUS                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_GPSTIME_CMD_GET_STATUS,                                                       \
        "GET_STATUS",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlGpsTimeCmdGetStatusInput),                                         \
        sizeof(DtIoctlGpsTimeCmdGetStatusOutput))

#define DT_IOCTL_CMD_PROPS_GPSTIME_GET_TIME                                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_GPSTIME_CMD_GET_TIME,                                                         \
        "GET_TIME",                                                                      \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlGpsTimeCmdGetTimeInput),                                           \
        sizeof(DtIoctlGpsTimeCmdGetTimeOutput))

#define DT_IOCTL_CMD_PROPS_GPSTIME_GET_HOLDOFF                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_GPSTIME_CMD_GET_HOLDOFF,                                                      \
        "GET_HOLDOFF",                                                                   \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlGpsTimeCmdGetHoldOffInput),                                        \
        sizeof(DtIoctlGpsTimeCmdGetHoldOffOutput))

#define DT_IOCTL_CMD_PROPS_GPSTIME_SET_HOLDOFF                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_GPSTIME_CMD_SET_HOLDOFF,                                                      \
        "SET_HOLDOFF",                                                                   \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlGpsTimeCmdSetHoldOffInput),                                        \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_GPSTIME                                                  \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_GPSTIME[] =                    \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_GPSTIME_GET_STATUS,                                           \
        DT_IOCTL_CMD_PROPS_GPSTIME_GET_TIME,                                             \
        DT_IOCTL_CMD_PROPS_GPSTIME_GET_HOLDOFF,                                          \
        DT_IOCTL_CMD_PROPS_GPSTIME_SET_HOLDOFF                                           \
    }

#define DT_IOCTL_PROPS_GPSTIME_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)               \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_GPSTIME_CMD,                                                            \
        "DT_IOCTL_GPSTIME_CMD",                                                          \
        DT_IOCTL_CMD_PROPS_GPSTIME,                                                      \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQAGC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_IQAGC_GET_CONFIG                                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQAGC_CMD_GET_CONFIG,                                                         \
        "GET_CONFIG",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqAgcCmdGetConfigInput),                                           \
        sizeof(DtIoctlIqAgcCmdGetConfigOutput))

#define DT_IOCTL_CMD_PROPS_IQAGC_GET_GAIN_CONTROL                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQAGC_CMD_GET_GAIN_CONTROL,                                                   \
        "GET_GAIN_CONTROL",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqAgcCmdGetGainControlInput),                                      \
        sizeof(DtIoctlIqAgcCmdGetGainControlOutput))

#define DT_IOCTL_CMD_PROPS_IQAGC_GET_GAIN_CTRL_PARS                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQAGC_CMD_GET_GAIN_CTRL_PARS,                                                 \
        "GET_GAIN_CTRL_PARS",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqAgcCmdGetGainCtrlParsInput),                                     \
        sizeof(DtIoctlIqAgcCmdGetGainCtrlParsOutput))

#define DT_IOCTL_CMD_PROPS_IQAGC_GET_GAIN_STATUS                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQAGC_CMD_GET_GAIN_STATUS,                                                    \
        "GET_GAIN_STATUS",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqAgcCmdGetGainStatusInput),                                       \
        sizeof(DtIoctlIqAgcCmdGetGainStatusOutput))

#define DT_IOCTL_CMD_PROPS_IQAGC_GET_OPERATIONAL_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQAGC_CMD_GET_OPERATIONAL_MODE,                                               \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqAgcCmdGetOpModeInput),                                           \
        sizeof(DtIoctlIqAgcCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_IQAGC_SET_GAIN_CONTROL                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQAGC_CMD_SET_GAIN_CONTROL,                                                   \
        "SET_GAIN_CONTROL",                                                              \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqAgcCmdSetGainControlInput),                                      \
        0)

#define DT_IOCTL_CMD_PROPS_IQAGC_SET_GAIN_CTRL_PARS                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQAGC_CMD_SET_GAIN_CTRL_PARS,                                                 \
        "SET_GAIN_CTRL_PARS",                                                            \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqAgcCmdSetGainCtrlParsInput),                                     \
        0)

#define DT_IOCTL_CMD_PROPS_IQAGC_SET_OPERATIONAL_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQAGC_CMD_SET_OPERATIONAL_MODE,                                               \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqAgcCmdSetOpModeInput),                                           \
        0)
        
#define DECL_DT_IOCTL_CMD_PROPS_IQAGC                                                    \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQAGC[] =                      \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQAGC_GET_CONFIG,                                             \
        DT_IOCTL_CMD_PROPS_IQAGC_GET_GAIN_CONTROL,                                       \
        DT_IOCTL_CMD_PROPS_IQAGC_GET_GAIN_CTRL_PARS,                                     \
        DT_IOCTL_CMD_PROPS_IQAGC_GET_GAIN_STATUS,                                        \
        DT_IOCTL_CMD_PROPS_IQAGC_GET_OPERATIONAL_MODE,                                   \
        DT_IOCTL_CMD_PROPS_IQAGC_SET_GAIN_CONTROL,                                       \
        DT_IOCTL_CMD_PROPS_IQAGC_SET_GAIN_CTRL_PARS,                                     \
        DT_IOCTL_CMD_PROPS_IQAGC_SET_OPERATIONAL_MODE                                    \
    }

#define DT_IOCTL_PROPS_IQAGC_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                 \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQAGC_CMD,                                                              \
        "DT_IOCTL_IQAGC_CMD",                                                            \
        DT_IOCTL_CMD_PROPS_IQAGC,                                                        \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQDEMOD_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_IQDEMOD_GET_NCO_PARS                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQDEMOD_CMD_GET_NCO_PARS,                                                     \
        "GET_NCO_PARS",                                                                  \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqDemodCmdGetNcoParsInput),                                        \
        sizeof(DtIoctlIqDemodCmdGetNcoParsOutput))

#define DT_IOCTL_CMD_PROPS_IQDEMOD_GET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQDEMOD_CMD_GET_OPERATIONAL_MODE,                                             \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqDemodCmdGetOpModeInput),                                         \
        sizeof(DtIoctlIqDemodCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_IQDEMOD_SET_NCO_PARS                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQDEMOD_CMD_SET_NCO_PARS,                                                     \
        "SET_NCO_PARS",                                                                  \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqDemodCmdSetNcoParsInput),                                        \
        0)


#define DT_IOCTL_CMD_PROPS_IQDEMOD_SET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQDEMOD_CMD_SET_OPERATIONAL_MODE,                                             \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqDemodCmdSetOpModeInput),                                         \
        0)
        
#define DECL_DT_IOCTL_CMD_PROPS_IQDEMOD                                                  \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQDEMOD[] =                    \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQDEMOD_GET_NCO_PARS,                                         \
        DT_IOCTL_CMD_PROPS_IQDEMOD_GET_OPERATIONAL_MODE,                                 \
        DT_IOCTL_CMD_PROPS_IQDEMOD_SET_NCO_PARS,                                         \
        DT_IOCTL_CMD_PROPS_IQDEMOD_SET_OPERATIONAL_MODE                                  \
    }

#define DT_IOCTL_PROPS_IQDEMOD_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)               \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQDEMOD_CMD,                                                            \
        "DT_IOCTL_IQDEMOD_CMD",                                                          \
        DT_IOCTL_CMD_PROPS_IQDEMOD,                                                      \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQDOWNSRC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_IQDOWNSRC_GET_CONV_RATE_PARS                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQDOWNSRC_CMD_GET_CONV_RATE_PARS,                                             \
        "GET_CONV_RATE_PARS",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqDownSrcCmdGetConvRateParsInput),                                 \
        sizeof(DtIoctlIqDownSrcCmdGetConvRateParsOutput))

#define DT_IOCTL_CMD_PROPS_IQDOWNSRC_GET_OPERATIONAL_MODE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQDOWNSRC_CMD_GET_OPERATIONAL_MODE,                                           \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqDownSrcCmdGetOpModeInput),                                       \
        sizeof(DtIoctlIqDownSrcCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_IQDOWNSRC_SET_CONV_RATE_PARS                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQDOWNSRC_CMD_SET_CONV_RATE_PARS,                                             \
        "SET_CONV_RATE_PARS",                                                            \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqDownSrcCmdSetConvRateParsInput),                                 \
        0)

#define DT_IOCTL_CMD_PROPS_IQDOWNSRC_SET_OPERATIONAL_MODE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQDOWNSRC_CMD_SET_OPERATIONAL_MODE,                                           \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqDownSrcCmdSetOpModeInput),                                       \
        0)


#define DECL_DT_IOCTL_CMD_PROPS_IQDOWNSRC                                                \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQDOWNSRC[] =                  \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQDOWNSRC_GET_CONV_RATE_PARS,                                 \
        DT_IOCTL_CMD_PROPS_IQDOWNSRC_GET_OPERATIONAL_MODE,                               \
        DT_IOCTL_CMD_PROPS_IQDOWNSRC_SET_CONV_RATE_PARS,                                 \
        DT_IOCTL_CMD_PROPS_IQDOWNSRC_SET_OPERATIONAL_MODE                                \
    }

#define DT_IOCTL_PROPS_IQDOWNSRC_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)             \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQDOWNSRC_CMD,                                                          \
        "DT_IOCTL_IQDOWNSRC_CMD",                                                        \
        DT_IOCTL_CMD_PROPS_IQDOWNSRC,                                                    \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQFIR_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_IQFIR_GET_CONFIG                                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQFIR_CMD_GET_CONFIG,                                                         \
        "GET_CONFIG",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqFirCmdGetConfigInput),                                           \
        sizeof(DtIoctlIqFirCmdGetConfigOutput))

#define DT_IOCTL_CMD_PROPS_IQFIR_GET_COEFS                                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQFIR_CMD_GET_COEFS,                                                          \
        "GET_COEFS",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqFirCmdGetCoefsInput),                                            \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlIqFirCmdGetCoefsOutput))

#define DT_IOCTL_CMD_PROPS_IQFIR_GET_OPERATIONAL_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQFIR_CMD_GET_OPERATIONAL_MODE,                                               \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqFirCmdGetOpModeInput),                                           \
        sizeof(DtIoctlIqFirCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_IQFIR_SET_COEFS                                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQFIR_CMD_SET_COEFS,                                                          \
        "SET_COEFS",                                                                     \
        TRUE,                                                                            \
        TRUE,                                                                            \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlIqFirCmdSetCoefsInput),                           \
        0)

#define DT_IOCTL_CMD_PROPS_IQFIR_SET_OPERATIONAL_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQFIR_CMD_SET_OPERATIONAL_MODE,                                               \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqFirCmdSetOpModeInput),                                           \
        0)

#define DT_IOCTL_CMD_PROPS_IQFIR_SET_PRESCALE                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQFIR_CMD_SET_PRESCALE,                                                       \
        "SET_PRESCALE",                                                                  \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqFirCmdSetPrescaleInput),                                         \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_IQFIR                                                    \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQFIR[] =                      \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQFIR_GET_CONFIG,                                             \
        DT_IOCTL_CMD_PROPS_IQFIR_GET_COEFS,                                              \
        DT_IOCTL_CMD_PROPS_IQFIR_GET_OPERATIONAL_MODE,                                   \
        DT_IOCTL_CMD_PROPS_IQFIR_SET_COEFS,                                              \
        DT_IOCTL_CMD_PROPS_IQFIR_SET_OPERATIONAL_MODE,                                   \
        DT_IOCTL_CMD_PROPS_IQFIR_SET_PRESCALE                                            \
    }

#define DT_IOCTL_PROPS_IQFIR_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                 \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQFIR_CMD,                                                              \
        "DT_IOCTL_IQFIR_CMD",                                                            \
        DT_IOCTL_CMD_PROPS_IQFIR,                                                        \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQFIR2XCLK_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_IQFIR2XCLK_GET_CONFIG                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQFIR2XCLK_CMD_GET_CONFIG,                                                    \
        "GET_CONFIG",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqFir2xClkCmdGetConfigInput),                                      \
        sizeof(DtIoctlIqFir2xClkCmdGetConfigOutput))

#define DT_IOCTL_CMD_PROPS_IQFIR2XCLK_GET_COEFS                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQFIR2XCLK_CMD_GET_COEFS,                                                     \
        "GET_COEFS",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqFir2xClkCmdGetCoefsInput),                                       \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlIqFir2xClkCmdGetCoefsOutput))

#define DT_IOCTL_CMD_PROPS_IQFIR2XCLK_GET_OPERATIONAL_MODE                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQFIR2XCLK_CMD_GET_OPERATIONAL_MODE,                                          \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqFir2xClkCmdGetOpModeInput),                                      \
        sizeof(DtIoctlIqFir2xClkCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_IQFIR2XCLK_SET_COEFS                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQFIR2XCLK_CMD_SET_COEFS,                                                     \
        "SET_COEFS",                                                                     \
        TRUE,                                                                            \
        TRUE,                                                                            \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlIqFir2xClkCmdSetCoefsInput),                      \
        0)

#define DT_IOCTL_CMD_PROPS_IQFIR2XCLK_SET_OPERATIONAL_MODE                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQFIR2XCLK_CMD_SET_OPERATIONAL_MODE,                                          \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqFir2xClkCmdSetOpModeInput),                                      \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_IQFIR2XCLK                                               \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQFIR2XCLK[] =                 \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQFIR2XCLK_GET_CONFIG,                                        \
        DT_IOCTL_CMD_PROPS_IQFIR2XCLK_GET_COEFS,                                         \
        DT_IOCTL_CMD_PROPS_IQFIR2XCLK_GET_OPERATIONAL_MODE,                              \
        DT_IOCTL_CMD_PROPS_IQFIR2XCLK_SET_COEFS,                                         \
        DT_IOCTL_CMD_PROPS_IQFIR2XCLK_SET_OPERATIONAL_MODE,                              \
    }

#define DT_IOCTL_PROPS_IQFIR2XCLK_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)            \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQFIR2XCLK_CMD,                                                         \
        "DT_IOCTL_IQFIR2XCLK_CMD",                                                       \
        DT_IOCTL_CMD_PROPS_IQFIR2XCLK,                                                   \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQINV_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_IQINV_GET_OPERATIONAL_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQINV_CMD_GET_OPERATIONAL_MODE,                                               \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqInvCmdGetOpModeInput),                                           \
        sizeof(DtIoctlIqInvCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_IQINV_GET_INVERT                                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQINV_CMD_GET_INVERT,                                                         \
        "GET_INVERT",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqInvCmdGetInvertInput),                                           \
        sizeof(DtIoctlIqInvCmdGetInvertOutput))


#define DT_IOCTL_CMD_PROPS_IQINV_SET_OPERATIONAL_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQINV_CMD_SET_OPERATIONAL_MODE,                                               \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqInvCmdSetOpModeInput),                                           \
        0)
        

#define DT_IOCTL_CMD_PROPS_IQINV_SET_INVERT                                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQINV_CMD_SET_INVERT,                                                         \
        "SET_INVERT",                                                                    \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqInvCmdSetInvertInput),                                           \
        0)



#define DECL_DT_IOCTL_CMD_PROPS_IQINV                                                    \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQINV[] =                      \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQINV_GET_INVERT,                                             \
        DT_IOCTL_CMD_PROPS_IQINV_GET_OPERATIONAL_MODE,                                   \
        DT_IOCTL_CMD_PROPS_IQINV_SET_INVERT,                                             \
        DT_IOCTL_CMD_PROPS_IQINV_SET_OPERATIONAL_MODE                                    \
    }

#define DT_IOCTL_PROPS_IQINV_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                 \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQINV_CMD,                                                              \
        "DT_IOCTL_IQINV_CMD",                                                            \
        DT_IOCTL_CMD_PROPS_IQINV,                                                        \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQJESD_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_IQJESD_CALIBRATE_ATX_PLL                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQJESD_CMD_CALIBRATE_ATX_PLL,                                                 \
        "CALIBRATE_ATX_PLL",                                                             \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqJesdCmdCalibrateAtxPllInput),                                    \
        0)

#define DT_IOCTL_CMD_PROPS_IQJESD_GET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQJESD_CMD_GET_OPERATIONAL_MODE,                                              \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqJesdCmdGetOpModeInput),                                          \
        sizeof(DtIoctlIqJesdCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_IQJESD_GET_SYSREF_ALWAYS_ON                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQJESD_CMD_GET_SYSREF_ALWAYS_ON,                                              \
        "GET_SYSREF_ALWAYS_ON",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqJesdCmdGetSysrefAlwaysOnInput),                                  \
        sizeof(DtIoctlIqJesdCmdGetSysrefAlwaysOnOutput))

#define DT_IOCTL_CMD_PROPS_IQJESD_RE_INITIALIZE_CSR_LINK                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQJESD_CMD_RE_INITIALIZE_CSR_LINK,                                            \
        "RE_INITIALIZE_CSR_LINK",                                                        \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqJesdCmdReInitializeCsrlinkInput),                                \
        0)

#define DT_IOCTL_CMD_PROPS_IQJESD_SET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQJESD_CMD_SET_OPERATIONAL_MODE,                                              \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqJesdCmdSetOpModeInput),                                          \
        0)
        
#define DT_IOCTL_CMD_PROPS_IQJESD_SET_SYSREF_ALWAYS_ON                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQJESD_CMD_SET_SYSREF_ALWAYS_ON,                                              \
        "SET_SYSREF_ALWAYS_ON",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqJesdCmdSetSysrefAlwaysOnInput),                                  \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_IQJESD                                                   \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQJESD[] =                     \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQJESD_CALIBRATE_ATX_PLL,                                     \
        DT_IOCTL_CMD_PROPS_IQJESD_GET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_IQJESD_GET_SYSREF_ALWAYS_ON,                                  \
        DT_IOCTL_CMD_PROPS_IQJESD_RE_INITIALIZE_CSR_LINK,                                \
        DT_IOCTL_CMD_PROPS_IQJESD_SET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_IQJESD_SET_SYSREF_ALWAYS_ON                                   \
    }

#define DT_IOCTL_PROPS_IQJESD_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQJESD_CMD,                                                             \
        "DT_IOCTL_IQJESD_CMD",                                                           \
        DT_IOCTL_CMD_PROPS_IQJESD,                                                       \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQMISC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_IQMISC_GET_IQMUTE                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQMISC_CMD_GET_IQMUTE,                                                        \
        "GET_IQMUTE",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqMiscCmdGetIqMuteInput),                                          \
        sizeof(DtIoctlIqMiscCmdGetIqMuteOutput))

#define DT_IOCTL_CMD_PROPS_IQMISC_GET_NOISE_LEVEL                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQMISC_CMD_GET_NOISE_LEVEL,                                                   \
        "GET_NOISE_LEVEL",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqMiscCmdGetNoiseLevelInput),                                      \
        sizeof(DtIoctlIqMiscCmdGetNoiseLevelOutput))

#define DT_IOCTL_CMD_PROPS_IQMISC_GET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQMISC_CMD_GET_OPERATIONAL_MODE,                                              \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqMiscCmdGetOpModeInput),                                          \
        sizeof(DtIoctlIqMiscCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_IQMISC_GET_OFFSET                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQMISC_CMD_GET_OFFSET,                                                        \
        "GET_OFFSET",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqMiscCmdGetOffsetInput),                                          \
        sizeof(DtIoctlIqMiscCmdGetOffsetOutput))

#define DT_IOCTL_CMD_PROPS_IQMISC_GET_OUTPUT_LEVEL                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQMISC_CMD_GET_OUTPUT_LEVEL,                                                  \
        "GET_OUTPUT_LEVEL",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqMiscCmdGetOutputLevelInput),                                     \
        sizeof(DtIoctlIqMiscCmdGetOutputLevelOutput))

#define DT_IOCTL_CMD_PROPS_IQMISC_GET_SPECTRUM_INVERSION                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQMISC_CMD_GET_SPECTRUM_INVERSION,                                            \
        "GET_SPECTRUM_INVERSION",                                                        \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqMiscCmdGetSpectrumInvInput),                                     \
        sizeof(DtIoctlIqMiscCmdGetSpectrumInvOutput))

#define DT_IOCTL_CMD_PROPS_IQMISC_GET_TEST_PATTERN                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQMISC_CMD_GET_TEST_PATTERN,                                                  \
        "GET_TEST_PATTERN",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqMiscCmdGetTestPatternInput),                                     \
        sizeof(DtIoctlIqMiscCmdGetTestPatternOutput))

#define DT_IOCTL_CMD_PROPS_IQMISC_SET_IQMUTE                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQMISC_CMD_SET_IQMUTE,                                                        \
        "SET_IQMUTE",                                                                    \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqMiscCmdSetIqMuteInput),                                          \
        0)

#define DT_IOCTL_CMD_PROPS_IQMISC_SET_NOISE_LEVEL                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQMISC_CMD_SET_NOISE_LEVEL,                                                   \
        "SET_NOISE_LEVEL",                                                               \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqMiscCmdSetNoiseLevelInput),                                      \
        0)

#define DT_IOCTL_CMD_PROPS_IQMISC_SET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQMISC_CMD_SET_OPERATIONAL_MODE,                                              \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqMiscCmdSetOpModeInput),                                          \
        0)

#define DT_IOCTL_CMD_PROPS_IQMISC_SET_OFFSET                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQMISC_CMD_SET_OFFSET,                                                        \
        "SET_OFFSET",                                                                    \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqMiscCmdSetOffsetInput),                                          \
        0)

#define DT_IOCTL_CMD_PROPS_IQMISC_SET_OUTPUT_LEVEL                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQMISC_CMD_SET_OUTPUT_LEVEL,                                                  \
        "SET_OUTPUT_LEVEL",                                                              \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqMiscCmdSetOutputLevelInput),                                     \
        0)

#define DT_IOCTL_CMD_PROPS_IQMISC_SET_SPECTRUM_INVERSION                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQMISC_CMD_SET_SPECTRUM_INVERSION,                                            \
        "SET_SPECTRUM_INVERSION",                                                        \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqMiscCmdSetSpectrumInvInput),                                     \
        0)

#define DT_IOCTL_CMD_PROPS_IQMISC_SET_TEST_PATTERN                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQMISC_CMD_SET_TEST_PATTERN,                                                  \
        "SET_TEST_PATTERN",                                                              \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqMiscCmdSetTestPatternInput),                                     \
        0)
                
#define DECL_DT_IOCTL_CMD_PROPS_IQMISC                                                   \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQMISC[] =                     \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQMISC_GET_IQMUTE,                                            \
        DT_IOCTL_CMD_PROPS_IQMISC_GET_NOISE_LEVEL,                                       \
        DT_IOCTL_CMD_PROPS_IQMISC_GET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_IQMISC_GET_OFFSET,                                            \
        DT_IOCTL_CMD_PROPS_IQMISC_GET_OUTPUT_LEVEL,                                      \
        DT_IOCTL_CMD_PROPS_IQMISC_GET_SPECTRUM_INVERSION,                                \
        DT_IOCTL_CMD_PROPS_IQMISC_GET_TEST_PATTERN,                                      \
        DT_IOCTL_CMD_PROPS_IQMISC_SET_IQMUTE,                                            \
        DT_IOCTL_CMD_PROPS_IQMISC_SET_NOISE_LEVEL,                                       \
        DT_IOCTL_CMD_PROPS_IQMISC_SET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_IQMISC_SET_OFFSET,                                            \
        DT_IOCTL_CMD_PROPS_IQMISC_SET_OUTPUT_LEVEL,                                      \
        DT_IOCTL_CMD_PROPS_IQMISC_SET_SPECTRUM_INVERSION,                                \
        DT_IOCTL_CMD_PROPS_IQMISC_SET_TEST_PATTERN                                       \
    }

#define DT_IOCTL_PROPS_IQMISC_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQMISC_CMD,                                                             \
        "DT_IOCTL_IQMISC_CMD",                                                           \
        DT_IOCTL_CMD_PROPS_IQMISC,                                                       \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQNOISE_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_IQNOISE_GET_BIT_DEPTH                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQNOISE_CMD_GET_BIT_DEPTH,                                                    \
        "GET_BIT_DEPTH",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqNoiseCmdGetBitDepthInput),                                       \
        sizeof(DtIoctlIqNoiseCmdGetBitDepthOutput))

#define DT_IOCTL_CMD_PROPS_IQNOISE_GET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQNOISE_CMD_GET_OPERATIONAL_MODE,                                             \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqNoiseCmdGetOpModeInput),                                         \
        sizeof(DtIoctlIqNoiseCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_IQNOISE_SET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQNOISE_CMD_SET_OPERATIONAL_MODE,                                             \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlIqNoiseCmdSetOpModeInput),                                         \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_IQNOISE                                                  \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQNOISE[] =                    \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQNOISE_GET_BIT_DEPTH,                                        \
        DT_IOCTL_CMD_PROPS_IQNOISE_GET_OPERATIONAL_MODE,                                 \
        DT_IOCTL_CMD_PROPS_IQNOISE_SET_OPERATIONAL_MODE                                  \
    }

#define DT_IOCTL_PROPS_IQNOISE_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)               \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQNOISE_CMD,                                                            \
        "DT_IOCTL_IQNOISE_CMD",                                                          \
        DT_IOCTL_CMD_PROPS_IQNOISE,                                                      \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQPWR_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_IQPWR_GET_OPERATIONAL_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQPWR_CMD_GET_OPERATIONAL_MODE,                                               \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqPwrCmdGetOpModeInput),                                           \
        sizeof(DtIoctlIqPwrCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_IQPWR_GET_SUM_OF_SQUARES                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQPWR_CMD_GET_SUM_OF_SQUARES,                                                 \
        "GET_SUM_OF_SQUARES",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqPwrCmdGetSumOfSquaresInput),                                     \
        sizeof(DtIoctlIqPwrCmdGetSumOfSquaresOutput))

#define DT_IOCTL_CMD_PROPS_IQPWR_GET_WINDOW_SIZE                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQPWR_CMD_GET_WINDOW_SIZE,                                                    \
        "GET_WINDOW_SIZE",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqPwrCmdGetWindowSizeInput),                                       \
        sizeof(DtIoctlIqPwrCmdGetWindowSizeOutput))

#define DT_IOCTL_CMD_PROPS_IQPWR_SET_OPERATIONAL_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQPWR_CMD_SET_OPERATIONAL_MODE,                                               \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqPwrCmdSetOpModeInput),                                           \
        0)
        
#define DT_IOCTL_CMD_PROPS_IQPWR_SET_WINDOW_SIZE                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQPWR_CMD_SET_WINDOW_SIZE,                                                    \
        "SET_WINDOW_SIZE",                                                               \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqPwrCmdSetWindowSizeInput),                                       \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_IQPWR                                                    \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQPWR[] =                      \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQPWR_GET_OPERATIONAL_MODE,                                   \
        DT_IOCTL_CMD_PROPS_IQPWR_GET_SUM_OF_SQUARES,                                     \
        DT_IOCTL_CMD_PROPS_IQPWR_GET_WINDOW_SIZE,                                        \
        DT_IOCTL_CMD_PROPS_IQPWR_SET_OPERATIONAL_MODE,                                   \
        DT_IOCTL_CMD_PROPS_IQPWR_SET_WINDOW_SIZE                                         \
    }

#define DT_IOCTL_PROPS_IQPWR_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                 \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQPWR_CMD,                                                              \
        "DT_IOCTL_IQPWR_CMD",                                                            \
        DT_IOCTL_CMD_PROPS_IQPWR,                                                        \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQSYNC_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
    
#define DT_IOCTL_CMD_PROPS_IQSYNC_GET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQSYNC_CMD_GET_OPERATIONAL_MODE,                                              \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqSyncCmdGetOpModeInput),                                          \
        sizeof(DtIoctlIqSyncCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_IQSYNC_GET_SAMPLE_COUNTER                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQSYNC_CMD_GET_SAMPLE_COUNTERS,                                               \
        "GET_SAMPLE_COUNTERS",                                                           \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqSyncCmdGetSampleCountersInput),                                  \
        sizeof(DtIoctlIqSyncCmdGetSampleCountersOutput))

#define DT_IOCTL_CMD_PROPS_IQSYNC_GET_STATUS                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQSYNC_CMD_GET_STATUS,                                                        \
        "GET_STATUS",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqSyncCmdGetStatusInput),                                          \
        sizeof(DtIoctlIqSyncCmdGetStatusOutput))

#define DT_IOCTL_CMD_PROPS_IQSYNC_GET_SYNC_MODE                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQSYNC_CMD_GET_SYNC_MODE,                                                     \
        "GET_SYNC_MODE",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqSyncCmdGetSyncModeInput),                                        \
        sizeof(DtIoctlIqSyncCmdGetSyncModeOutput))
                
#define DT_IOCTL_CMD_PROPS_IQSYNC_GET_TIME_TOLERANCE                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQSYNC_CMD_GET_TIME_TOLERANCE,                                                \
        "GET_TIME_TOLERANCE",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqSyncCmdGetTimeToleranceInput),                                   \
        sizeof(DtIoctlIqSyncCmdGetTimeToleranceOutput))

#define DT_IOCTL_CMD_PROPS_IQSYNC_GET_TRIGGER_OFFSET                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQSYNC_CMD_GET_TRIGGER_OFFSET,                                                \
        "GET_TRIGGER_OFFSET",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqSyncCmdGetTriggerOffsetInput),                                   \
        sizeof(DtIoctlIqSyncCmdGetTriggerOffsetOutput))

#define DT_IOCTL_CMD_PROPS_IQSYNC_SET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQSYNC_CMD_SET_OPERATIONAL_MODE,                                              \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlIqSyncCmdSetOpModeInput),                                          \
        0)

#define DT_IOCTL_CMD_PROPS_IQSYNC_SET_SAMPLE_COUNTER                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQSYNC_CMD_SET_SAMPLE_COUNTERS,                                                \
        "SET_SAMPLE_COUNTER",                                                            \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlIqSyncCmdSetSampleCountersInput),                                   \
        0)

#define DT_IOCTL_CMD_PROPS_IQSYNC_SET_SYNC_MODE                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQSYNC_CMD_SET_SYNC_MODE,                                                     \
        "SET_SYNC_MODE",                                                                 \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlIqSyncCmdSetSyncModeInput),                                        \
        0)

#define DT_IOCTL_CMD_PROPS_IQSYNC_SET_TIME_TOLERANCE                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQSYNC_CMD_SET_TIME_TOLERANCE,                                                \
        "SET_TIME_TOLERANCE",                                                            \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlIqSyncCmdSetTimeToleranceInput),                                   \
        0)

#define DT_IOCTL_CMD_PROPS_IQSYNC_SET_TRIGGER_OFFSET                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQSYNC_CMD_SET_TRIGGER_OFFSET,                                                \
        "SET_TRIGGER_OFFSET",                                                            \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlIqSyncCmdSetTriggerOffsetInput),                                   \
        0)
       
#define DECL_DT_IOCTL_CMD_PROPS_IQSYNC                                                   \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQSYNC[] =                     \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQSYNC_GET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_IQSYNC_GET_SAMPLE_COUNTER,                                    \
        DT_IOCTL_CMD_PROPS_IQSYNC_GET_STATUS,                                            \
        DT_IOCTL_CMD_PROPS_IQSYNC_GET_SYNC_MODE,                                         \
        DT_IOCTL_CMD_PROPS_IQSYNC_GET_TIME_TOLERANCE,                                    \
        DT_IOCTL_CMD_PROPS_IQSYNC_GET_TRIGGER_OFFSET,                                    \
        DT_IOCTL_CMD_PROPS_IQSYNC_SET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_IQSYNC_SET_SAMPLE_COUNTER,                                    \
        DT_IOCTL_CMD_PROPS_IQSYNC_SET_SYNC_MODE,                                         \
        DT_IOCTL_CMD_PROPS_IQSYNC_SET_TIME_TOLERANCE,                                    \
        DT_IOCTL_CMD_PROPS_IQSYNC_SET_TRIGGER_OFFSET,                                    \
    }

#define DT_IOCTL_PROPS_IQSYNC_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQSYNC_CMD,                                                             \
        "DT_IOCTL_IQSYNC_CMD",                                                           \
        DT_IOCTL_CMD_PROPS_IQSYNC,                                                       \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQUNPCK_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_IQUNPCK_GET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQUNPCK_CMD_GET_OPERATIONAL_MODE,                                             \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqUnpckCmdGetOpModeInput),                                         \
        sizeof(DtIoctlIqUnpckCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_IQUNPCK_GET_PACKET_MODE                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQUNPCK_CMD_GET_PACKET_MODE,                                                  \
        "GET_PACKET_MODE",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqUnpckCmdGetPacketModeInput),                                     \
        sizeof(DtIoctlIqUnpckCmdGetPacketModeOutput))

#define DT_IOCTL_CMD_PROPS_IQUNPCK_SET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S12GTO3G_CMD_SET_OPERATIONAL_MODE,                                            \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlIqUnpckCmdSetOpModeInput),                                         \
        0)
        
#define DT_IOCTL_CMD_PROPS_IQUNPCK_SET_PACKET_MODE                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQUNPCK_CMD_SET_PACKET_MODE,                                                  \
        "SET_PACKET_MODE",                                                               \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlIqUnpckCmdSetPacketModeInput),                                     \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_IQUNPCK                                                  \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQUNPCK[] =                    \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQUNPCK_GET_OPERATIONAL_MODE,                                 \
        DT_IOCTL_CMD_PROPS_IQUNPCK_GET_PACKET_MODE,                                      \
        DT_IOCTL_CMD_PROPS_IQUNPCK_SET_OPERATIONAL_MODE,                                 \
        DT_IOCTL_CMD_PROPS_IQUNPCK_SET_PACKET_MODE                                       \
    }

#define DT_IOCTL_PROPS_IQUNPCK_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)               \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQUNPCK_CMD,                                                            \
        "DT_IOCTL_IQUNPCK_CMD",                                                          \
        DT_IOCTL_CMD_PROPS_IQUNPCK,                                                      \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQUP_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_IQUP_GET_NCO_RATIO                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQUP_CMD_GET_NCO_RATIO,                                                       \
        "GET_NCO_RATIO",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqUpCmdGetNcoRatioInput),                                          \
        sizeof(DtIoctlIqUpCmdGetNcoRatioOutput))

#define DT_IOCTL_CMD_PROPS_IQUP_GET_NCO_OFFSET                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQUP_CMD_GET_NCO_OFFSET,                                                      \
        "GET_NCO_OFFSET",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqUpCmdGetNcoOffsetInput),                                         \
        sizeof(DtIoctlIqUpCmdGetNcoOffsetOutput))

#define DT_IOCTL_CMD_PROPS_IQUP_GET_OPERATIONAL_MODE                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQUP_CMD_GET_OPERATIONAL_MODE,                                                \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqUpCmdGetOpModeInput),                                            \
        sizeof(DtIoctlIqUpCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_IQUP_SET_NCO_RATIO                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQUP_CMD_SET_NCO_RATIO,                                                       \
        "SET_NCO_RATIO",                                                                 \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlIqUpCmdSetNcoRatioInput),                                          \
        0)

#define DT_IOCTL_CMD_PROPS_IQUP_SET_NCO_OFFSET                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQUP_CMD_SET_NCO_OFFSET,                                                      \
        "SET_NCO_OFFSET",                                                                \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlIqUpCmdSetNcoOffsetInput),                                         \
        0)

#define DT_IOCTL_CMD_PROPS_IQUP_SET_OPERATIONAL_MODE                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQUP_CMD_SET_OPERATIONAL_MODE,                                                \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                           \
        sizeof(DtIoctlIqUpCmdSetOpModeInput),                                            \
        0)
        
#define DECL_DT_IOCTL_CMD_PROPS_IQUP                                                     \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQUP[] =                       \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQUP_GET_NCO_RATIO,                                           \
        DT_IOCTL_CMD_PROPS_IQUP_GET_NCO_OFFSET,                                          \
        DT_IOCTL_CMD_PROPS_IQUP_GET_OPERATIONAL_MODE,                                    \
        DT_IOCTL_CMD_PROPS_IQUP_SET_NCO_RATIO,                                           \
        DT_IOCTL_CMD_PROPS_IQUP_SET_NCO_OFFSET,                                          \
        DT_IOCTL_CMD_PROPS_IQUP_SET_OPERATIONAL_MODE                                     \
    }

#define DT_IOCTL_PROPS_IQUP_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                  \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQUP_CMD,                                                               \
        "DT_IOCTL_IQUP_CMD",                                                             \
        DT_IOCTL_CMD_PROPS_IQUP,                                                         \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_KA_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_KA_GET_FAILSAFE_INFO                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_KA_CMD_GET_FAILSAFE_INFO,                                                     \
        "GET_FAILSAFE_INFO",                                                             \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlKaCmdGetFailSafeInfoInput),                                        \
        sizeof(DtIoctlKaCmdGetFailSafeInfoOutput))

#define DT_IOCTL_CMD_PROPS_KA_PULSE                                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_KA_CMD_PULSE,                                                                 \
        "SET_PULSE",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlKaCmdPulseInput),                                                  \
        0)

#define DT_IOCTL_CMD_PROPS_KA_SET_FAILSAFE_CONFIG                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_KA_CMD_SET_FAILSAFE_CONFIG,                                                   \
        "SET_FAILSAFE_CONFIG",                                                           \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlKaCmdSetFailSafeConfigInput),                                      \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_KA                                                       \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_KA[] =                         \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_KA_GET_FAILSAFE_INFO,                                         \
        DT_IOCTL_CMD_PROPS_KA_PULSE,                                                     \
        DT_IOCTL_CMD_PROPS_KA_SET_FAILSAFE_CONFIG,                                       \
    }

#define DT_IOCTL_PROPS_KA_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                    \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_KA_CMD,                                                                 \
        "DT_IOCTL_KA_CMD",                                                               \
        DT_IOCTL_CMD_PROPS_KA,                                                           \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_LEDB_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_LEDB_GET_LED_CONTROL                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_LEDB_CMD_GET_LED_CONTROL,                                                     \
        "GET_LED_CONTROL",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlLedBCmdGetLedControlInput),                                        \
        sizeof(DtIoctlLedBCmdGetLedControlOutput))

#define DT_IOCTL_CMD_PROPS_LEDB_SET_LED_CONTROL                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_LEDB_CMD_SET_LED_CONTROL,                                                     \
        "SET_LED_CONTROL",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlLedBCmdSetLedControlInput),                                        \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_LEDB                                                     \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_LEDB[] =                       \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_LEDB_GET_LED_CONTROL,                                         \
        DT_IOCTL_CMD_PROPS_LEDB_SET_LED_CONTROL,                                         \
    }

#define DT_IOCTL_PROPS_LEDB_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                  \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_LEDB_CMD,                                                               \
        "DT_IOCTL_LEDB_CMD",                                                             \
        DT_IOCTL_CMD_PROPS_LEDB,                                                         \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_LMH1981_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_LMH1981_GET_INPUT_STATUS                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_LMH1981_CMD_GET_INPUT_STATUS,                                                 \
        "GET_LED_CONTROL",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlLmh1981CmdGetInputStatusInput),                                    \
        sizeof(DtIoctlLmh1981CmdGetInputStatusOutput))


#define DECL_DT_IOCTL_CMD_PROPS_LMH1981                                                  \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_LMH1981[] =                    \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_LMH1981_GET_INPUT_STATUS,                                     \
    }

#define DT_IOCTL_PROPS_LMH1981_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)               \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_LMH1981_CMD,                                                            \
        "DT_IOCTL_LMH1981_CMD",                                                          \
        DT_IOCTL_CMD_PROPS_LMH1981,                                                      \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_PROPERTY_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_PROPERTY_GET_VALUE                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PROP_CMD_GET_VALUE,                                                           \
        "GET_VALUE",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPropCmdGetValueInput),                                             \
        sizeof(DtIoctlPropCmdGetValueOutput))

#define DT_IOCTL_CMD_PROPS_PROPERTY_GET_STR                                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PROP_CMD_GET_STR,                                                             \
        "GET_STR",                                                                       \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPropCmdGetStrInput),                                               \
        sizeof(DtIoctlPropCmdGetStrOutput))

#define DT_IOCTL_CMD_PROPS_PROPERTY_GET_TABLE                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_PROP_CMD_GET_TABLE,                                                           \
        "GET_TABLE",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlPropCmdGetTableInput),                                             \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlPropCmdGetTableOutput))

#define DECL_DT_IOCTL_CMD_PROPS_PROPERTY                                                 \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_PROPERTY[] =                   \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_PROPERTY_GET_VALUE,                                           \
        DT_IOCTL_CMD_PROPS_PROPERTY_GET_STR,                                             \
        DT_IOCTL_CMD_PROPS_PROPERTY_GET_TABLE,                                           \
    }

#define DT_IOCTL_PROPS_PROPERTY_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)              \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_PROPERTY_CMD,                                                           \
        "DT_IOCTL_PROPERTY_CMD",                                                         \
        DT_IOCTL_CMD_PROPS_PROPERTY,                                                     \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_REBOOT_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define IOCTL_CMD_PROPS_REBOOT_FPGA                                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_REBOOT_CMD_FPGA_REBOOT,                                                       \
        "REBOOT_FPGA",                                                                   \
        FALSE,                                                                           \
        FALSE,                                                                           \
        sizeof(DtIoctlRebootCmdFpgaRebootInput),                                         \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_REBOOT                                                   \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_REBOOT[] =                     \
    {                                                                                    \
        IOCTL_CMD_PROPS_REBOOT_FPGA,                                                     \
    }

#define DT_IOCTL_PROPS_REBOOT_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_REBOOT_CMD,                                                             \
        "DT_IOCTL_REBOOT_CMD",                                                           \
        DT_IOCTL_CMD_PROPS_REBOOT,                                                       \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_S12GTO3G_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_S12GTO3G_GET_OPERATIONAL_MODE                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S12GTO3G_CMD_GET_OPERATIONAL_MODE,                                            \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlS12Gto3GCmdGetOpModeInput),                                        \
        sizeof(DtIoctlS12Gto3GCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_S12GTO3G_GET_SCALING_ENABLE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S12GTO3G_CMD_GET_SCALING_ENABLE,                                              \
        "GET_SCALING_ENABLE",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlS12Gto3GCmdGetScalingEnableInput),                                 \
        sizeof(DtIoctlS12Gto3GCmdGetScalingEnableOutput))

#define DT_IOCTL_CMD_PROPS_S12GTO3G_SET_OPERATIONAL_MODE                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S12GTO3G_CMD_SET_OPERATIONAL_MODE,                                            \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlS12Gto3GCmdSetOpModeInput),                                        \
        0)

#define DT_IOCTL_CMD_PROPS_S12GTO3G_SET_SCALING_ENABLE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S12GTO3G_CMD_SET_SCALING_ENABLE,                                              \
        "SET_SCALING_ENABLE",                                                            \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlS12Gto3GCmdSetScalingEnableInput),                                 \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_S12GTO3G                                                 \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_S12GTO3G[] =                   \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_S12GTO3G_GET_OPERATIONAL_MODE,                                \
        DT_IOCTL_CMD_PROPS_S12GTO3G_GET_SCALING_ENABLE,                                  \
        DT_IOCTL_CMD_PROPS_S12GTO3G_SET_OPERATIONAL_MODE,                                \
        DT_IOCTL_CMD_PROPS_S12GTO3G_SET_SCALING_ENABLE,                                  \
    }

#define DT_IOCTL_PROPS_S12GTO3G_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)              \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_S12GTO3G_CMD,                                                           \
        "DT_IOCTL_S12GTO3G_CMD",                                                         \
        DT_IOCTL_CMD_PROPS_S12GTO3G,                                                     \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SDIDMX12G_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_SDIDMX12G_GET_OPERATIONAL_MODE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIDMX12G_CMD_GET_OPERATIONAL_MODE,                                           \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiDmx12GCmdGetOpModeInput),                                      \
        sizeof(DtIoctlSdiDmx12GCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_SDIDMX12G_GET_SDIRATE                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIDMX12G_CMD_GET_SDIRATE,                                                    \
        "GET_SDIRATE",                                                                   \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiDmx12GCmdGetSdiRateInput),                                     \
        sizeof(DtIoctlSdiDmx12GCmdGetSdiRateOutput))

#define DT_IOCTL_CMD_PROPS_SDIDMX12G_SET_OPERATIONAL_MODE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIDMX12G_CMD_SET_OPERATIONAL_MODE,                                           \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSdiDmx12GCmdSetOpModeInput),                                      \
        0)

#define DT_IOCTL_CMD_PROPS_SDIDMX12G_SET_SDIRATE                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIDMX12G_CMD_SET_SDIRATE,                                                    \
        "SET_SDIRATE",                                                                   \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSdiDmx12GCmdSetSdiRateInput),                                     \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_SDIDMX12G                                                \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SDIDMX12G[] =                  \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_SDIDMX12G_GET_OPERATIONAL_MODE,                               \
        DT_IOCTL_CMD_PROPS_SDIDMX12G_GET_SDIRATE,                                        \
        DT_IOCTL_CMD_PROPS_SDIDMX12G_SET_OPERATIONAL_MODE,                               \
        DT_IOCTL_CMD_PROPS_SDIDMX12G_SET_SDIRATE,                                        \
    }

#define DT_IOCTL_PROPS_SDIDMX12G_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)             \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SDIDMX12G_CMD,                                                          \
        "DT_IOCTL_SDIDMX12G_CMD",                                                        \
        DT_IOCTL_CMD_PROPS_SDIDMX12G,                                                    \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SDIMUX12G_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_SDIMUX12G_GET_OPERATIONAL_MODE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIMUX12G_CMD_GET_OPERATIONAL_MODE,                                           \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiMux12GCmdGetOpModeInput),                                      \
        sizeof(DtIoctlSdiMux12GCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_SDIMUX12G_SET_OPERATIONAL_MODE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIMUX12G_CMD_SET_OPERATIONAL_MODE,                                           \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSdiMux12GCmdSetOpModeInput),                                      \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_SDIMUX12G                                                \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SDIMUX12G[] =                  \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_SDIMUX12G_GET_OPERATIONAL_MODE,                               \
        DT_IOCTL_CMD_PROPS_SDIMUX12G_SET_OPERATIONAL_MODE,                               \
    }

#define DT_IOCTL_PROPS_SDIMUX12G_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)             \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SDIMUX12G_CMD,                                                          \
        "DT_IOCTL_SDIMUX12G_CMD",                                                        \
        DT_IOCTL_CMD_PROPS_SDIMUX12G,                                                    \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )



//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SDIXCFG_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_SDIXCFG_GET_CONFIG                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIXCFG_CMD_GET_CONFIG,                                                       \
        "GET_CONFIG",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiXCfgCmdGetConfigInput),                                         \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlSdiXCfgCmdGetConfigOutput))

#define DT_IOCTL_CMD_PROPS_SDIXCFG_SET_MODE                                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIXCFG_CMD_SET_MODE,                                                         \
        "SET_MODE",                                                                      \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiXCfgCmdSetModeInput),                                           \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_SDIXCFG                                                  \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SDIXCFG[] =                    \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_SDIXCFG_GET_CONFIG,                                           \
        DT_IOCTL_CMD_PROPS_SDIXCFG_SET_MODE,                                             \
    }

#define DT_IOCTL_PROPS_SDIXCFG_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)               \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SDIXCFG_CMD,                                                            \
        "DT_IOCTL_SDIXCFG_CMD",                                                          \
        DT_IOCTL_CMD_PROPS_SDIXCFG,                                                      \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SDIRX_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_SDIRX_GET_MAX_SDIRATE                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRX_CMD_GET_MAX_SDIRATE,                                                    \
        "GET_MAX_SDIRATE",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxCmdGetMaxSdiRateInput),                                       \
        sizeof(DtIoctlSdiRxCmdGetMaxSdiRateOutput))

#define DT_IOCTL_CMD_PROPS_SDIRX_GET_OPERATIONAL_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRX_CMD_GET_OPERATIONAL_MODE,                                               \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxCmdGetOpModeInput),                                           \
        sizeof(DtIoctlSdiRxCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_SDIRX_GET_RXMODE                                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRX_CMD_GET_RXMODE,                                                         \
        "GET_RXMODE",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxCmdGetRxModeInput),                                           \
        sizeof(DtIoctlSdiRxCmdGetRxModeOutput))

#define DT_IOCTL_CMD_PROPS_SDIRX_GET_SDIRATE                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRX_CMD_GET_SDIRATE,                                                        \
        "GET_SDIRATE",                                                                   \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxCmdGetSdiRateInput),                                          \
        sizeof(DtIoctlSdiRxCmdGetSdiRateOutput))

#define DT_IOCTL_CMD_PROPS_SDIRX_GET_SDI_STATUS                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRX_CMD_GET_SDI_STATUS,                                                     \
        "GET_SDI_STATUS",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxCmdGetSdiStatusInput),                                        \
        sizeof(DtIoctlSdiRxCmdGetSdiStatusOutput))

#define DT_IOCTL_CMD_PROPS_SDIRX_GET_SDI_STATUS2                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRX_CMD_GET_SDI_STATUS2,                                                    \
        "GET_SDI_STATUS2",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxCmdGetSdiStatusInput),                                        \
        sizeof(DtIoctlSdiRxCmdGetSdiStatusOutput2))

#define DT_IOCTL_CMD_PROPS_SDIRX_SET_OPERATIONAL_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRX_CMD_SET_OPERATIONAL_MODE,                                               \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxCmdSetOpModeInput),                                           \
        0)

#define DT_IOCTL_CMD_PROPS_SDIRX_SET_RXMODE                                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRX_CMD_SET_RXMODE,                                                         \
        "SET_RXMODE",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxCmdSetRxModeInput),                                           \
        0)

#define DT_IOCTL_CMD_PROPS_SDIRX_SET_SDIRATE                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRX_CMD_SET_SDIRATE,                                                        \
        "SET_SDIRATE",                                                                   \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxCmdSetSdiRateInput),                                          \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_SDIRX                                                    \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SDIRX[] =                      \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_SDIRX_GET_MAX_SDIRATE,                                        \
        DT_IOCTL_CMD_PROPS_SDIRX_GET_OPERATIONAL_MODE,                                   \
        DT_IOCTL_CMD_PROPS_SDIRX_GET_RXMODE,                                             \
        DT_IOCTL_CMD_PROPS_SDIRX_GET_SDIRATE,                                            \
        DT_IOCTL_CMD_PROPS_SDIRX_GET_SDI_STATUS,                                         \
        DT_IOCTL_CMD_PROPS_SDIRX_GET_SDI_STATUS2,                                        \
        DT_IOCTL_CMD_PROPS_SDIRX_SET_OPERATIONAL_MODE,                                   \
        DT_IOCTL_CMD_PROPS_SDIRX_SET_RXMODE,                                             \
        DT_IOCTL_CMD_PROPS_SDIRX_SET_SDIRATE,                                            \
    }

#define DT_IOCTL_PROPS_SDIRX_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                 \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SDIRX_CMD,                                                              \
        "DT_IOCTL_SDIRX_CMD",                                                            \
        DT_IOCTL_CMD_PROPS_SDIRX,                                                        \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_CHSDIRX_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_CHSDIRX_ATTACH                                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRX_CMD_ATTACH,                                                           \
        "ATTACH",                                                                        \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxCmdAttachInput),                                            \
        0)

#define DT_IOCTL_CMD_PROPS_CHSDIRX_CONFIGURE                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRX_CMD_CONFIGURE,                                                        \
        "CONFIGURE",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxCmdConfigureInput),                                         \
        0)

#define DT_IOCTL_CMD_PROPS_CHSDIRX_DETACH                                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRX_CMD_DETACH,                                                           \
        "DETACH",                                                                        \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxCmdDetachInput),                                            \
        0)

#define DT_IOCTL_CMD_PROPS_CHSDIRX_GET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRX_CMD_GET_OPERATIONAL_MODE,                                             \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxCmdGetOpModeInput),                                         \
        sizeof(DtIoctlChSdiRxCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_CHSDIRX_SET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRX_CMD_SET_OPERATIONAL_MODE,                                             \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxCmdSetOpModeInput),                                         \
        0)

#define DT_IOCTL_CMD_PROPS_CHSDIRX_WAIT_FOR_FMT_EVENT                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRX_CMD_WAIT_FOR_FMT_EVENT,                                               \
        "WAIT_FOR_FMT_EVENT",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxCmdWaitForFmtEventInput),                                   \
        sizeof(DtIoctlChSdiRxCmdWaitForFmtEventOutput))

#define DT_IOCTL_CMD_PROPS_CHSDIRX_GET_WRITE_OFFSET                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRX_CMD_GET_WRITE_OFFSET,                                                 \
        "GET_WRITE_OFFSET",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxCmdGetWrOffsetInput),                                       \
        sizeof(DtIoctlChSdiRxCmdGetWrOffsetOutput))

#define DT_IOCTL_CMD_PROPS_CHSDIRX_SET_READ_OFFSET                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRX_CMD_SET_READ_OFFSET,                                                  \
        "SET_READ_OFFSET",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxCmdSetRdOffsetInput),                                       \
        0)

#define DT_IOCTL_CMD_PROPS_CHSDIRX_GET_PROPS                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRX_CMD_GET_PROPS,                                                        \
        "GET_PROPS",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxCmdGetPropsInput),                                          \
        sizeof(DtIoctlChSdiRxCmdGetPropsOutput))

#define DT_IOCTL_CMD_PROPS_CHSDIRX_GET_SDI_STATUS                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRX_CMD_GET_SDI_STATUS,                                                   \
        "GET_SDI_STATUS",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxCmdGetSdiStatusInput),                                      \
        sizeof(DtIoctlChSdiRxCmdGetSdiStatusOutput))

#define DT_IOCTL_CMD_PROPS_CHSDIRX_MAP_DMA_BUF_TO_USER                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRX_CMD_MAP_DMA_BUF_TO_USER,                                              \
        "MAP_DMA_BUF_TO_USER",                                                           \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxCmdMapDmaBufToUserInput),                                   \
        sizeof(DtIoctlChSdiRxCmdMapDmaBufToUserOutput))


#define DECL_DT_IOCTL_CMD_PROPS_CHSDIRX                                                  \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_CHSDIRX[] =                    \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_CHSDIRX_ATTACH,                                               \
        DT_IOCTL_CMD_PROPS_CHSDIRX_CONFIGURE,                                            \
        DT_IOCTL_CMD_PROPS_CHSDIRX_DETACH,                                               \
        DT_IOCTL_CMD_PROPS_CHSDIRX_GET_OPERATIONAL_MODE,                                 \
        DT_IOCTL_CMD_PROPS_CHSDIRX_SET_OPERATIONAL_MODE,                                 \
        DT_IOCTL_CMD_PROPS_CHSDIRX_WAIT_FOR_FMT_EVENT,                                   \
        DT_IOCTL_CMD_PROPS_CHSDIRX_GET_WRITE_OFFSET,                                     \
        DT_IOCTL_CMD_PROPS_CHSDIRX_SET_READ_OFFSET,                                      \
        DT_IOCTL_CMD_PROPS_CHSDIRX_GET_PROPS,                                            \
        DT_IOCTL_CMD_PROPS_CHSDIRX_GET_SDI_STATUS,                                       \
        DT_IOCTL_CMD_PROPS_CHSDIRX_MAP_DMA_BUF_TO_USER,                                  \
    }

#define DT_IOCTL_PROPS_CHSDIRX_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)               \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_CHSDIRX_CMD,                                                            \
        "DT_IOCTL_CHSDIRX_CMD",                                                          \
        DT_IOCTL_CMD_PROPS_CHSDIRX,                                                      \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_CHSDIRXPHYONLY_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY_ATTACH                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRXPHYONLY_CMD_ATTACH,                                                    \
        "ATTACH",                                                                        \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxPhyOnlyCmdAttachInput),                                     \
        0)

#define DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY_CONFIGURE                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRXPHYONLY_CMD_CONFIGURE,                                                 \
        "CONFIGURE",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxPhyOnlyCmdConfigureInput),                                  \
        0)

#define DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY_DETACH                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRXPHYONLY_CMD_DETACH,                                                    \
        "DETACH",                                                                        \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxPhyOnlyCmdDetachInput),                                     \
        0)

#define DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY_GET_OPERATIONAL_MODE                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRXPHYONLY_CMD_GET_OPERATIONAL_MODE,                                      \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxPhyOnlyCmdGetOpModeInput),                                  \
        sizeof(DtIoctlChSdiRxPhyOnlyCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY_SET_OPERATIONAL_MODE                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRXPHYONLY_CMD_SET_OPERATIONAL_MODE,                                      \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxPhyOnlyCmdSetOpModeInput),                                  \
        0)

#define DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY_GET_SDI_STATUS                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CHSDIRXPHYONLY_CMD_GET_SDI_STATUS,                                            \
        "GET_SDI_STATUS",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlChSdiRxPhyOnlyCmdGetSdiStatusInput),                               \
        sizeof(DtIoctlChSdiRxPhyOnlyCmdGetSdiStatusOutput))


#define DECL_DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY                                           \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY[] =             \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY_ATTACH,                                        \
        DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY_CONFIGURE,                                     \
        DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY_DETACH,                                        \
        DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY_GET_OPERATIONAL_MODE,                          \
        DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY_SET_OPERATIONAL_MODE,                          \
        DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY_GET_SDI_STATUS,                                \
    }

#define DT_IOCTL_PROPS_CHSDIRXPHYONLY_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)        \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_CHSDIRXPHYONLY_CMD,                                                     \
        "DT_IOCTL_CHSDIRXPHYONLY_CMD",                                                   \
        DT_IOCTL_CMD_PROPS_CHSDIRXPHYONLY,                                               \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SDIRXF_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_SDIRXF_GET_FMT_EVENT_TIMING                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXF_CMD_GET_FMT_EVENT_TIMING,                                              \
        "GET_FMT_EVENT_TIMING",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxFCmdGetFmtEventTimingInput),                                  \
        sizeof(DtIoctlSdiRxFCmdGetFmtEventTimingOutput))

#define DT_IOCTL_CMD_PROPS_SDIRXF_GET_FRAME_PROPERTIES                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXF_CMD_GET_FRAME_PROPERTIES,                                              \
        "GET_FRAME_PROPERTIES",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxFCmdGetFramePropertiesInput),                                 \
        sizeof(DtIoctlSdiRxFCmdGetFramePropertiesOutput))

#define DT_IOCTL_CMD_PROPS_SDIRXF_GET_MAX_SDIRATE                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXF_CMD_GET_MAX_SDIRATE,                                                   \
        "GET_MAX_SDIRATE",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxFCmdGetMaxSdiRateInput),                                      \
        sizeof(DtIoctlSdiRxFCmdGetMaxSdiRateOutput))

#define DT_IOCTL_CMD_PROPS_SDIRXF_GET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXF_CMD_GET_OPERATIONAL_MODE,                                              \
        "OPERATIONAL_MODE",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxFCmdGetOpModeInput),                                          \
        sizeof(DtIoctlSdiRxFCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_SDIRXF_GET_STREAM_ALIGNMENT                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXF_CMD_GET_STREAM_ALIGNMENT,                                              \
        "GET_STREAM_ALIGNMENT",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxFCmdGetStreamAlignmentInput),                                 \
        sizeof(DtIoctlSdiRxFCmdGetStreamAlignmentOutput))

#define DT_IOCTL_CMD_PROPS_SDIRXF_SET_FMT_EVENT_TIMING                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXF_CMD_SET_FMT_EVENT_TIMING,                                              \
        "SET_FMT_EVENT_TIMING",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSdiRxFCmdSetFmtEventTimingInput),                                  \
        0)

#define DT_IOCTL_CMD_PROPS_SDIRXF_SET_FRAME_PROPERTIES                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXF_CMD_SET_FRAME_PROPERTIES,                                              \
        "SET_FRAME_PROPERTIES",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSdiRxFCmdSetFramePropertiesInput),                                 \
        0)

#define DT_IOCTL_CMD_PROPS_SDIRXF_SET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXF_CMD_SET_OPERATIONAL_MODE,                                              \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSdiRxFCmdSetOpModeInput),                                          \
        0)

#define DT_IOCTL_CMD_PROPS_SDIRXF_WAIT_FOR_FMT_EVENT                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXF_CMD_WAIT_FOR_FMT_EVENT,                                                \
        "WAIT_FOR_FMT_EVENT",                                                            \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSdiRxFCmdWaitForFmtEventInput),                                    \
        sizeof(DtIoctlSdiRxFCmdWaitForFmtEventOutput))

#define DECL_DT_IOCTL_CMD_PROPS_SDIRXF                                                   \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SDIRXF[] =                     \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_SDIRXF_GET_FMT_EVENT_TIMING,                                  \
        DT_IOCTL_CMD_PROPS_SDIRXF_GET_FRAME_PROPERTIES,                                  \
        DT_IOCTL_CMD_PROPS_SDIRXF_GET_MAX_SDIRATE,                                       \
        DT_IOCTL_CMD_PROPS_SDIRXF_GET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_SDIRXF_GET_STREAM_ALIGNMENT,                                  \
        DT_IOCTL_CMD_PROPS_SDIRXF_SET_FMT_EVENT_TIMING,                                  \
        DT_IOCTL_CMD_PROPS_SDIRXF_SET_FRAME_PROPERTIES,                                  \
        DT_IOCTL_CMD_PROPS_SDIRXF_SET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_SDIRXF_WAIT_FOR_FMT_EVENT,                                    \
    }

#define DT_IOCTL_PROPS_SDIRXF_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)              \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SDIRXF_CMD,                                                           \
        "DT_IOCTL_SDIRXF_CMD",                                                         \
        DT_IOCTL_CMD_PROPS_SDIRXF,                                                     \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SDIRXP_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_SDIRXP_GET_CRC_ERROR_CNT                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXP_CMD_GET_CRC_ERROR_CNT,                                                 \
        "GET_CRC_ERROR_CNT",                                                             \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxPCmdGetCrcErrorCountInput),                                   \
        sizeof(DtIoctlSdiRxPCmdGetCrcErrorCountOutput))

#define DT_IOCTL_CMD_PROPS_SDIRXP_GET_MAX_SDIRATE                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXP_CMD_GET_MAX_SDIRATE,                                                   \
        "GET_MAX_SDIRATE",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxPCmdGetMaxSdiRateInput),                                      \
        sizeof(DtIoctlSdiRxPCmdGetMaxSdiRateOutput))

#define DT_IOCTL_CMD_PROPS_SDIRXP_GET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXP_CMD_GET_OPERATIONAL_MODE,                                              \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxPCmdGetOpModeInput),                                          \
        sizeof(DtIoctlSdiRxPCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_SDIRXP_GET_SDIRATE                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXP_CMD_GET_SDIRATE,                                                       \
        "GET_SDIRATE",                                                                   \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxPCmdGetSdiRateInput),                                         \
        sizeof(DtIoctlSdiRxPCmdGetSdiRateOutput))

#define DT_IOCTL_CMD_PROPS_SDIRXP_GET_SDI_STATUS                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXP_CMD_GET_SDI_STATUS,                                                    \
        "GET_SDI_STATUS",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxPCmdGetSdiStatusInput),                                       \
        sizeof(DtIoctlSdiRxPCmdGetSdiStatusOutput))

#define DT_IOCTL_CMD_PROPS_SDIRXP_GET_SDI_STATUS2                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXP_CMD_GET_SDI_STATUS2,                                                   \
        "GET_SDI_STATUS2",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxPCmdGetSdiStatusInput),                                       \
        sizeof(DtIoctlSdiRxPCmdGetSdiStatusOutput2))

#define DT_IOCTL_CMD_PROPS_SDIRXP_SET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXP_CMD_SET_OPERATIONAL_MODE,                                              \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxPCmdSetOpModeInput),                                          \
        0)

#define DT_IOCTL_CMD_PROPS_SDIRXP_SET_SDIRATE                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDIRXP_CMD_SET_SDIRATE,                                                       \
        "SET_SDIRATE",                                                                   \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiRxPCmdSetSdiRateInput),                                         \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_SDIRXP                                                   \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SDIRXP[] =                     \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_SDIRXP_GET_CRC_ERROR_CNT,                                     \
        DT_IOCTL_CMD_PROPS_SDIRXP_GET_MAX_SDIRATE,                                       \
        DT_IOCTL_CMD_PROPS_SDIRXP_GET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_SDIRXP_GET_SDIRATE,                                           \
        DT_IOCTL_CMD_PROPS_SDIRXP_GET_SDI_STATUS,                                        \
        DT_IOCTL_CMD_PROPS_SDIRXP_GET_SDI_STATUS2,                                        \
        DT_IOCTL_CMD_PROPS_SDIRXP_SET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_SDIRXP_SET_SDIRATE,                                           \
    }

#define DT_IOCTL_PROPS_SDIRXP_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SDIRXP_CMD,                                                             \
        "DT_IOCTL_SDIRXP_CMD",                                                           \
        DT_IOCTL_CMD_PROPS_SDIRXP,                                                       \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SDITXF_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_SDITXF_GET_FMT_EVENT_SETTING                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXF_CMD_GET_FMT_EVENT_SETTING,                                             \
        "GET_FMT_EVENT_SETTING",                                                         \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxFCmdGetFmtEventSettingInput),                                 \
        sizeof(DtIoctlSdiTxFCmdGetFmtEventSettingOutput))

#define DT_IOCTL_CMD_PROPS_SDITXF_GET_MAX_SDIRATE                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXF_CMD_GET_MAX_SDIRATE,                                                   \
        "GET_MAX_SDIRATE",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxFCmdGetMaxSdiRateInput),                                      \
        sizeof(DtIoctlSdiTxFCmdGetMaxSdiRateOutput))

#define DT_IOCTL_CMD_PROPS_SDITXF_GET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXF_CMD_GET_OPERATIONAL_MODE,                                              \
        "OPERATIONAL_MODE",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxFCmdGetOpModeInput),                                          \
        sizeof(DtIoctlSdiTxFCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_SDITXF_GET_STREAM_ALIGNMENT                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXF_CMD_GET_STREAM_ALIGNMENT,                                              \
        "GET_STREAM_ALIGNMENT",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxFCmdGetStreamAlignmentInput),                                 \
        sizeof(DtIoctlSdiTxFCmdGetStreamAlignmentOutput))

#define DT_IOCTL_CMD_PROPS_SDITXF_SET_FMT_EVENT_SETTING                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXF_CMD_SET_FMT_EVENT_SETTING,                                             \
        "SET_FMT_EVENT_SETTING",                                                         \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSdiTxFCmdSetFmtEventSettingInput),                                 \
        0)

#define DT_IOCTL_CMD_PROPS_SDITXF_SET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXF_CMD_SET_OPERATIONAL_MODE,                                              \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSdiTxFCmdSetOpModeInput),                                          \
        0)

#define DT_IOCTL_CMD_PROPS_SDITXF_WAIT_FOR_FMT_EVENT                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXF_CMD_WAIT_FOR_FMT_EVENT,                                                \
        "WAIT_FOR_FMT_EVENT",                                                            \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSdiTxFCmdWaitForFmtEventInput),                                    \
        sizeof(DtIoctlSdiTxFCmdWaitForFmtEventOutput))

#define DECL_DT_IOCTL_CMD_PROPS_SDITXF                                                   \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SDITXF[] =                     \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_SDITXF_GET_FMT_EVENT_SETTING,                                 \
        DT_IOCTL_CMD_PROPS_SDITXF_GET_MAX_SDIRATE,                                       \
        DT_IOCTL_CMD_PROPS_SDITXF_GET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_SDITXF_GET_STREAM_ALIGNMENT,                                  \
        DT_IOCTL_CMD_PROPS_SDITXF_SET_FMT_EVENT_SETTING,                                 \
        DT_IOCTL_CMD_PROPS_SDITXF_SET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_SDITXF_WAIT_FOR_FMT_EVENT,                                    \
    }

#define DT_IOCTL_PROPS_SDITXF_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SDITXF_CMD,                                                             \
        "DT_IOCTL_SDITXF_CMD",                                                           \
        DT_IOCTL_CMD_PROPS_SDITXF,                                                       \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SDITXP_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_SDITXP_GET_CLAMPED_SYMS_COUNT                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXP_CMD_GET_CLAMPED_SYMS_COUNT,                                            \
        "GET_CLAMPED_SYMS_COUNT",                                                        \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPCmdGetClampedSymsCountInput),                                \
        sizeof(DtIoctlSdiTxPCmdGetClampedSymsCountOutput))

#define DT_IOCTL_CMD_PROPS_SDITXP_GET_GENERATION_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXP_CMD_GET_GENERATION_MODE,                                               \
        "GET_GENERATION_MODE",                                                           \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPCmdGetGenModeInput),                                         \
        sizeof(DtIoctlSdiTxPCmdGetGenModeOutput))

#define DT_IOCTL_CMD_PROPS_SDITXP_GET_MAX_SDIRATE                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXP_CMD_GET_MAX_SDIRATE,                                                   \
        "GET_MAX_SDIRATE",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPCmdGetMaxSdiRateInput),                                      \
        sizeof(DtIoctlSdiTxPCmdGetMaxSdiRateOutput))

#define DT_IOCTL_CMD_PROPS_SDITXP_GET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXP_CMD_GET_OPERATIONAL_MODE,                                              \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPCmdGetOpModeInput),                                          \
        sizeof(DtIoctlSdiTxPCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_SDITXP_GET_SDIRATE_AND_LVLA2BENA                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXP_CMD_GET_SDIRATE_AND_LVLA2BENA,                                        \
        "GET_SDIRATE_AND_LVLA2BENA",                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPCmdGetSdiRateAndLvlA2BEnaInput),                             \
        sizeof(DtIoctlSdiTxPCmdGetSdiRateAndLvlA2BEnaOutput))

#define DT_IOCTL_CMD_PROPS_SDITXP_SET_GENERATION_MODE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXP_CMD_SET_GENERATION_MODE,                                               \
        "SET_GENERATION_MODE",                                                           \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPCmdSetGenModeInput),                                         \
        0)

#define DT_IOCTL_CMD_PROPS_SDITXP_SET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXP_CMD_SET_OPERATIONAL_MODE,                                              \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPCmdSetOpModeInput),                                          \
        0)

#define DT_IOCTL_CMD_PROPS_SDITXP_SET_SDIRATE_AND_LVLA2BENA                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXP_CMD_SET_SDIRATE_AND_LVLA2BENA,                                         \
        "SET_SDIRATE_AND_LVLA2BENA",                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPCmdSetSdiRateAndLvlA2BEnaInput),                             \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_SDITXP                                                   \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SDITXP[] =                     \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_SDITXP_GET_CLAMPED_SYMS_COUNT,                                \
        DT_IOCTL_CMD_PROPS_SDITXP_GET_GENERATION_MODE,                                   \
        DT_IOCTL_CMD_PROPS_SDITXP_GET_MAX_SDIRATE,                                       \
        DT_IOCTL_CMD_PROPS_SDITXP_GET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_SDITXP_GET_SDIRATE_AND_LVLA2BENA,                             \
        DT_IOCTL_CMD_PROPS_SDITXP_SET_GENERATION_MODE,                                   \
        DT_IOCTL_CMD_PROPS_SDITXP_SET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_SDITXP_SET_SDIRATE_AND_LVLA2BENA,                             \
    }

#define DT_IOCTL_PROPS_SDITXP_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SDITXP_CMD,                                                             \
        "DT_IOCTL_SDITXP_CMD",                                                           \
        DT_IOCTL_CMD_PROPS_SDITXP,                                                       \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SDITXPHY_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_SDITXPHY_CLEAR_UNDERFLOW_FLAG                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXPHY_CMD_CLEAR_UNDERFLOW_FLAG,                                            \
        "CLEAR_UNDERFLOW_FLAG",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSdiTxPhyCmdClearUnderflowFlagInput),                               \
        0)

#define DT_IOCTL_CMD_PROPS_SDITXPHY_GET_GENLOCK_STATUS                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXPHY_CMD_GET_GENLOCK_STATUS,                                              \
        "GET_GENLOCK_STATUS",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPhyCmdGetGenLockStatusInput),                                 \
        sizeof(DtIoctlSdiTxPhyCmdGetGenLockStatusOutput))

#define DT_IOCTL_CMD_PROPS_SDITXPHY_GET_MAX_SDIRATE                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXPHY_CMD_GET_MAX_SDIRATE,                                                 \
        "GET_MAX_SDIRATE",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPhyCmdGetMaxSdiRateInput),                                    \
        sizeof(DtIoctlSdiTxPhyCmdGetMaxSdiRateOutput))

#define DT_IOCTL_CMD_PROPS_SDITXPHY_GET_OPERATIONAL_MODE                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXPHY_CMD_GET_OPERATIONAL_MODE,                                            \
        "OPERATIONAL_MODE",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPhyCmdGetOpModeInput),                                        \
        sizeof(DtIoctlSdiTxPhyCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_SDITXPHY_GET_SDIRATE                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXPHY_CMD_GET_SDIRATE,                                                     \
        "GET_SDIRATE",                                                                   \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPhyCmdGetSdiRateInput),                                       \
        sizeof(DtIoctlSdiTxPhyCmdGetSdiRateOutput))


#define DT_IOCTL_CMD_PROPS_SDITXPHY_GET_SDI_STATUS                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXPHY_CMD_GET_SDI_STATUS,                                                  \
        "GET_SDI_STATUS",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPhyCmdGetSdiStatusInput),                                     \
        sizeof(DtIoctlSdiTxPhyCmdGetSdiStatusOutput))

#define DT_IOCTL_CMD_PROPS_SDITXPHY_GET_START_OF_FRAME_OFFSET                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXPHY_CMD_GET_START_OF_FRAME_OFFSET,                                       \
        "GET_START_OF_FRAME_OFFSET",                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPhyCmdGetStartOfFrameOffsetInput),                            \
        sizeof(DtIoctlSdiTxPhyCmdGetStartOfFrameOffsetOutput))

#define DT_IOCTL_CMD_PROPS_SDITXPHY_GET_TXMODE                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXPHY_CMD_GET_TXMODE,                                                      \
        "GET_TXMODE",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPhyCmdGetTxModeInput),                                        \
        sizeof(DtIoctlSdiTxPhyCmdGetTxModeOutput))

#define DT_IOCTL_CMD_PROPS_SDITXPHY_GET_UNDERFLOW_FLAG                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXPHY_CMD_GET_UNDERFLOW_FLAG,                                              \
        "GET_UNDERFLOW_FLAG",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPhyCmdGetUnderflowFlagInput),                                 \
        sizeof(DtIoctlSdiTxPhyCmdGetUnderflowFlagOutput))


#define DT_IOCTL_CMD_PROPS_SDITXPHY_SET_OPERATIONAL_MODE                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXPHY_CMD_SET_OPERATIONAL_MODE,                                            \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSdiTxPhyCmdSetOpModeInput),                                        \
        0)

#define DT_IOCTL_CMD_PROPS_SDITXPHY_SET_OPERATIONAL_MODE_TIMED                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXPHY_CMD_SET_OPERATIONAL_MODE_TIMED,                                      \
        "SET_OPERATIONAL_MODE_TIMED",                                                    \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSdiTxPhyCmdSetOpModeTimedInput),                                   \
        0)

#define DT_IOCTL_CMD_PROPS_SDITXPHY_SET_SDIRATE                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXPHY_CMD_SET_SDIRATE,                                                     \
        "SET_SDIRATE",                                                                   \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPhyCmdSetSdiRateInput),                                       \
        0)

#define DT_IOCTL_CMD_PROPS_SDITXPHY_SET_START_OF_FRAME_OFFSET                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXPHY_CMD_SET_START_OF_FRAME_OFFSET,                                       \
        "SET_START_OF_FRAME_OFFSET",                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPhyCmdSetStartOfFrameOffsetInput),                            \
        0)

#define DT_IOCTL_CMD_PROPS_SDITXPHY_SET_TXMODE                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SDITXPHY_CMD_SET_TXMODE,                                                      \
        "SET_TXMODE",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSdiTxPhyCmdSetTxModeInput),                                        \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_SDITXPHY                                                 \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SDITXPHY[] =                   \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_SDITXPHY_CLEAR_UNDERFLOW_FLAG,                                \
        DT_IOCTL_CMD_PROPS_SDITXPHY_GET_GENLOCK_STATUS,                                  \
        DT_IOCTL_CMD_PROPS_SDITXPHY_GET_MAX_SDIRATE,                                     \
        DT_IOCTL_CMD_PROPS_SDITXPHY_GET_OPERATIONAL_MODE,                                \
        DT_IOCTL_CMD_PROPS_SDITXPHY_SET_OPERATIONAL_MODE_TIMED,                          \
        DT_IOCTL_CMD_PROPS_SDITXPHY_GET_SDIRATE,                                         \
        DT_IOCTL_CMD_PROPS_SDITXPHY_GET_SDI_STATUS,                                      \
        DT_IOCTL_CMD_PROPS_SDITXPHY_GET_START_OF_FRAME_OFFSET,                           \
        DT_IOCTL_CMD_PROPS_SDITXPHY_GET_TXMODE,                                          \
        DT_IOCTL_CMD_PROPS_SDITXPHY_GET_UNDERFLOW_FLAG,                                  \
        DT_IOCTL_CMD_PROPS_SDITXPHY_SET_OPERATIONAL_MODE,                                \
        DT_IOCTL_CMD_PROPS_SDITXPHY_SET_SDIRATE,                                         \
        DT_IOCTL_CMD_PROPS_SDITXPHY_SET_START_OF_FRAME_OFFSET,                           \
        DT_IOCTL_CMD_PROPS_SDITXPHY_SET_TXMODE,                                          \
    }

#define DT_IOCTL_PROPS_SDITXPHY_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)              \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SDITXPHY_CMD,                                                           \
        "DT_IOCTL_SDITXPHY_CMD",                                                         \
        DT_IOCTL_CMD_PROPS_SDITXPHY,                                                     \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SENSTEMP_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_SENSTEMP_GET_PROPERTIES                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SENSTEMP_CMD_GET_PROPERTIES,                                                  \
        "GET_PROPERTIES",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSensTempCmdGetPropertiesInput),                                    \
        sizeof(DtIoctlSensTempCmdGetPropertiesOutput))

#define DT_IOCTL_CMD_PROPS_SENSTEMP_GET_TEMPERATURE                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SENSTEMP_CMD_GET_TEMPERATURE,                                                 \
        "GET_TEMPERATURE",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSensTempCmdGetTemperatureInput),                                   \
        sizeof(DtIoctlSensTempCmdGetTemperatureOutput))

#define DECL_DT_IOCTL_CMD_PROPS_SENSTEMP                                                 \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SENSTEMP[] =                   \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_SENSTEMP_GET_PROPERTIES,                                      \
        DT_IOCTL_CMD_PROPS_SENSTEMP_GET_TEMPERATURE,                                     \
    }

#define DT_IOCTL_PROPS_SENSTEMP_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)              \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SENSTEMP_CMD,                                                           \
        "DT_IOCTL_SENSTEMP_CMD",                                                         \
        DT_IOCTL_CMD_PROPS_SENSTEMP,                                                     \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SI2166ITF_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

#define DT_IOCTL_CMD_PROPS_SI2166ITF_GET_CONFIGUARTION                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SI2166ITF_CMD_GET_CONFIGURATION,                                              \
        "GET_CONFIGUARTION",                                                             \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSi2166ItfCmdGetConfigInput),                                       \
        sizeof(DtIoctlSi2166ItfCmdGetConfigOutput))

#define DT_IOCTL_CMD_PROPS_SI2166ITF_SET_CONFIGUARTION                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SI2166ITF_CMD_SET_CONFIGURATION,                                              \
        "SET_CONFIGUARTION",                                                             \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSi2166ItfCmdSetConfigInput),                                       \
        0)

#define DT_IOCTL_CMD_PROPS_SI2166ITF_GET_OPERATIONAL_MODE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SI2166ITF_CMD_GET_OPERATIONAL_MODE,                                           \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSi2166ItfCmdGetOpModeInput),                                       \
        sizeof(DtIoctlSi2166ItfCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_SI2166ITF_SET_OPERATIONAL_MODE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SI2166ITF_CMD_SET_OPERATIONAL_MODE,                                           \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSi2166ItfCmdSetOpModeInput),                                       \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_SI2166ITF                                                \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SI2166ITF[] =                  \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_SI2166ITF_GET_CONFIGUARTION,                                  \
        DT_IOCTL_CMD_PROPS_SI2166ITF_GET_OPERATIONAL_MODE,                               \
        DT_IOCTL_CMD_PROPS_SI2166ITF_SET_CONFIGUARTION,                                  \
        DT_IOCTL_CMD_PROPS_SI2166ITF_SET_OPERATIONAL_MODE,                               \
    }

#define DT_IOCTL_PROPS_SI2166ITF_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)             \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SI2166ITF_CMD,                                                          \
        "DT_IOCTL_SI2166ITF_CMD",                                                        \
        DT_IOCTL_CMD_PROPS_SI2166ITF,                                                    \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SPIM_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_SPIM_GET_PROPERTIES                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SPIM_CMD_GET_PROPERTIES,                                                      \
        "GET_PROPERTIES",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSpiMCmdGetPropertiesInput),                                        \
        sizeof(DtIoctlSpiMCmdGetPropertiesOutput))

#define DT_IOCTL_CMD_PROPS_SPIM_READ                                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SPIM_CMD_READ,                                                                \
        "READ",                                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSpiMCmdReadInput),                                                 \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlSpiMCmdReadOutput))

#define DT_IOCTL_CMD_PROPS_SPIM_WRITE                                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SPIM_CMD_WRITE,                                                               \
        "WRITE",                                                                         \
        TRUE,                                                                            \
        TRUE,                                                                            \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlSpiMCmdWriteInput),                               \
        0)

#define DT_IOCTL_CMD_PROPS_SPIM_WRITE_READ                                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SPIM_CMD_WRITE_READ,                                                          \
        "WRITE_READ",                                                                    \
        TRUE,                                                                            \
        TRUE,                                                                            \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlSpiMCmdWriteReadInput),                           \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlSpiMCmdWriteReadOutput))

#define DECL_DT_IOCTL_CMD_PROPS_SPIM                                                     \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SPIM[] =                       \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_SPIM_GET_PROPERTIES,                                          \
        DT_IOCTL_CMD_PROPS_SPIM_READ,                                                    \
        DT_IOCTL_CMD_PROPS_SPIM_WRITE,                                                   \
        DT_IOCTL_CMD_PROPS_SPIM_WRITE_READ,                                              \
    }

#define DT_IOCTL_PROPS_SPIM_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                  \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SPIM_CMD,                                                               \
        "DT_IOCTL_SPIM_CMD",                                                             \
        DT_IOCTL_CMD_PROPS_SPIM,                                                         \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SPIMF_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMDEX_PROPS_SPIMF_LOCK                                                  \
    INIT_DT_IOCTL_CMDEX_PROPS(                                                           \
        DT_SPIMF_CMDEX_LOCK,                                                             \
        "LOCK",                                                                          \
        sizeof(DtIoctlSpiMfCmdLockDeviceInput),                                          \
        0)

#define DT_IOCTL_CMDEX_PROPS_SPIMF_UNLOCK                                                \
    INIT_DT_IOCTL_CMDEX_PROPS(                                                           \
        DT_SPIMF_CMDEX_UNLOCK,                                                           \
        "UNLOCK",                                                                        \
        sizeof(DtIoctlSpiMfCmdLockDeviceInput),                                          \
        0)

#define DT_IOCTL_CMD_PROPS_SPIMF_LOCK_DEVICE                                             \
    INIT_DT_IOCTL_CMD_PROPS_EX(                                                          \
        DT_SPIMF_CMD_LOCK_DEVICE,                                                        \
        "LOCK_DEVICE",                                                                   \
        TRUE,                                                                            \
        TRUE,                                                                            \
        DT_IOCTL_CMDEX_PROPS_LOCK_DEVICE)

#define DT_IOCTL_CMD_PROPS_SPIMF_GET_PROPERTIES                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SPIMF_CMD_GET_PROPERTIES,                                                     \
        "GET_PROPERTIES",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSpiMfCmdGetPropertiesInput),                                       \
        sizeof(DtIoctlSpiMfCmdGetPropertiesOutput))

#define DT_IOCTL_CMD_PROPS_SPIMF_ERASE                                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SPIMF_CMD_ERASE,                                                              \
        "ERASE",                                                                         \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSpiMfCmdEraseInput),                                               \
        0)

#define DT_IOCTL_CMD_PROPS_SPIMF_READ                                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SPIMF_CMD_READ,                                                               \
        "READ",                                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSpiMfCmdReadInput),                                                \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlSpiMfCmdReadOutput))

#define DT_IOCTL_CMD_PROPS_SPIMF_WRITE                                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SPIMF_CMD_WRITE,                                                              \
        "WRITE",                                                                         \
        TRUE,                                                                            \
        TRUE,                                                                            \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlSpiMfCmdWriteInput),                              \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_SPIMF                                                    \
    static const DtIoctlPropertiesCmdEx  DT_IOCTL_CMDEX_PROPS_LOCK_DEVICE[] =            \
    {                                                                                    \
        DT_IOCTL_CMDEX_PROPS_SPIMF_LOCK,                                                 \
        DT_IOCTL_CMDEX_PROPS_SPIMF_UNLOCK,                                               \
    };                                                                                   \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SPIMF[] =                      \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_SPIMF_LOCK_DEVICE,                                            \
        DT_IOCTL_CMD_PROPS_SPIMF_GET_PROPERTIES,                                         \
        DT_IOCTL_CMD_PROPS_SPIMF_ERASE,                                                  \
        DT_IOCTL_CMD_PROPS_SPIMF_READ,                                                   \
        DT_IOCTL_CMD_PROPS_SPIMF_WRITE,                                                  \
    }

#define DT_IOCTL_PROPS_SPIMF_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                 \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SPIMF_CMD,                                                              \
        "DT_IOCTL_SPIMF_CMD",                                                            \
        DT_IOCTL_CMD_PROPS_SPIMF,                                                        \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SPIPROM_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+= 

#define DT_IOCTL_CMDEX_PROPS_SPIPROM_LOCK                                                \
    INIT_DT_IOCTL_CMDEX_PROPS(                                                           \
        DT_SPIPROM_CMDEX_LOCK,                                                           \
        "LOCK",                                                                          \
        sizeof(DtIoctlSpiPromCmdLockDeviceInput),                                        \
        0)

#define DT_IOCTL_CMDEX_PROPS_SPIPROM_UNLOCK                                              \
    INIT_DT_IOCTL_CMDEX_PROPS(                                                           \
        DT_SPIPROM_CMDEX_UNLOCK,                                                         \
        "UNLOCK",                                                                        \
        sizeof(DtIoctlSpiPromCmdLockDeviceInput),                                        \
        0)

#define DT_IOCTL_CMD_PROPS_SPIPROM_LOCK_DEVICE                                           \
    INIT_DT_IOCTL_CMD_PROPS_EX(                                                          \
        DT_SPIPROM_CMD_LOCK_DEVICE,                                                      \
        "LOCK_DEVICE",                                                                   \
        TRUE,                                                                            \
        TRUE,                                                                            \
        DT_IOCTL_CMDEX_PROPS_LOCK_DEVICE)

#define DT_IOCTL_CMD_PROPS_SPIPROM_GET_PROPERTIES                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SPIPROM_CMD_GET_PROPERTIES,                                                   \
        "GET_PROPERTIES",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSpiPromCmdGetPropertiesInput),                                     \
        sizeof(DtIoctlSpiPromCmdGetPropertiesOutput))


#define DT_IOCTL_CMD_PROPS_SPIPROM_READ                                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SPIPROM_CMD_READ,                                                             \
        "READ",                                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSpiPromCmdReadInput),                                              \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlSpiPromCmdReadOutput))

#define DT_IOCTL_CMD_PROPS_SPIPROM_WRITE                                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SPIPROM_CMD_WRITE,                                                            \
        "WRITE",                                                                         \
        TRUE,                                                                            \
        TRUE,                                                                            \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlSpiPromCmdWriteInput),                            \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_SPIPROM                                                  \
    static const DtIoctlPropertiesCmdEx  DT_IOCTL_CMDEX_PROPS_LOCK_DEVICE[] =            \
    {                                                                                    \
        DT_IOCTL_CMDEX_PROPS_SPIPROM_LOCK,                                               \
        DT_IOCTL_CMDEX_PROPS_SPIPROM_UNLOCK,                                             \
    };                                                                                   \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SPIPROM[] =                    \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_SPIPROM_LOCK_DEVICE,                                          \
        DT_IOCTL_CMD_PROPS_SPIPROM_GET_PROPERTIES,                                       \
        DT_IOCTL_CMD_PROPS_SPIPROM_READ,                                                 \
        DT_IOCTL_CMD_PROPS_SPIPROM_WRITE,                                                \
    }

#define DT_IOCTL_PROPS_SPIPROM_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)               \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SPIPROM_CMD,                                                            \
        "DT_IOCTL_SPIPROM_CMD",                                                          \
        DT_IOCTL_CMD_PROPS_SPIPROM,                                                      \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_ST425LR_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_ST425LR_GET_LINK_ORDER                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ST425LR_CMD_GET_LINK_ORDER,                                                   \
        "GET_LINK_ORDER",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSt425LrCmdGetLinkOrderInput),                                      \
        sizeof(DtIoctlSt425LrCmdGetLinkOrderOutput))

#define DT_IOCTL_CMD_PROPS_ST425LR_GET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ST425LR_CMD_GET_OPERATIONAL_MODE,                                             \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSt425LrCmdGetOpModeInput),                                         \
        sizeof(DtIoctlSt425LrCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_ST425LR_SET_LINK_ORDER                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ST425LR_CMD_SET_LINK_ORDER,                                                   \
        "SET_LINK_ORDER",                                                                \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSt425LrCmdSetLinkOrderInput),                                      \
        0)

#define DT_IOCTL_CMD_PROPS_ST425LR_SET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ST425LR_CMD_SET_OPERATIONAL_MODE,                                             \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSt425LrCmdSetOpModeInput),                                         \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_ST425LR                                                  \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_ST425LR[] =                    \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_ST425LR_GET_OPERATIONAL_MODE,                                 \
        DT_IOCTL_CMD_PROPS_ST425LR_GET_LINK_ORDER,                                       \
        DT_IOCTL_CMD_PROPS_ST425LR_SET_OPERATIONAL_MODE,                                 \
        DT_IOCTL_CMD_PROPS_ST425LR_SET_LINK_ORDER,                                       \
    }

#define DT_IOCTL_PROPS_ST425LR_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)               \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_ST425LR_CMD,                                                            \
        "DT_IOCTL_ST425LR_CMD",                                                          \
        DT_IOCTL_CMD_PROPS_ST425LR,                                                      \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )




//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_SWITCH_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_SWITCH_CLEAR                                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SWITCH_CMD_CLEAR,                                                             \
        "CLEAR",                                                                         \
        TRUE,                                                                            \
        TRUE,                                                                            \
        0,                                                                               \
        0)

#define DT_IOCTL_CMD_PROPS_SWITCH_GET_CONFIGURATION                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SWITCH_CMD_GET_CONFIGURATION,                                                 \
        "GET_CONFIGURATION",                                                             \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSwitchCmdGetConfigurationInput),                                   \
        sizeof(DtIoctlSwitchCmdGetConfigurationOutput))

#define DT_IOCTL_CMD_PROPS_SWITCH_GET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SWITCH_CMD_GET_OPERATIONAL_MODE,                                              \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSwitchCmdGetOpModeInput),                                          \
        sizeof(DtIoctlSwitchCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_SWITCH_GET_POSITION                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SWITCH_CMD_GET_POSITION,                                                      \
        "GET_POSITION",                                                                  \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlSwitchCmdGetPositionInput),                                        \
        sizeof(DtIoctlSwitchCmdGetPositionOutput))

#define DT_IOCTL_CMD_PROPS_SWITCH_SET_OPERATIONAL_MODE                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SWITCH_CMD_SET_OPERATIONAL_MODE,                                              \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSwitchCmdSetOpModeInput),                                          \
        0)

#define DT_IOCTL_CMD_PROPS_SWITCH_SET_POSITION                                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_SWITCH_CMD_SET_POSITION,                                                      \
        "SET_POSITION",                                                                  \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlSwitchCmdSetPositionInput),                                        \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_SWITCH                                                   \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_SWITCH[] =                     \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_SWITCH_CLEAR,                                                 \
        DT_IOCTL_CMD_PROPS_SWITCH_GET_CONFIGURATION,                                     \
        DT_IOCTL_CMD_PROPS_SWITCH_GET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_SWITCH_GET_POSITION,                                          \
        DT_IOCTL_CMD_PROPS_SWITCH_SET_OPERATIONAL_MODE,                                  \
        DT_IOCTL_CMD_PROPS_SWITCH_SET_POSITION,                                          \
    }

#define DT_IOCTL_PROPS_SWITCH_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_SWITCH_CMD,                                                             \
        "DT_IOCTL_SWITCH_CMD",                                                           \
        DT_IOCTL_CMD_PROPS_SWITCH,                                                       \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_TEMPFANMGR_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_TEMPFANMGR_GET_FAN_STATUS                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TEMPFANMGR_CMD_GET_FAN_STATUS,                                                \
        "GET_FAN_STATUS",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTempFanMgrCmdGetFanStatusInput),                                   \
        sizeof(DtIoctlTempFanMgrCmdGetFanStatusOutput))

#define DT_IOCTL_CMD_PROPS_TEMPFANMGR_GET_NUM_FANS                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TEMPFANMGR_CMD_GET_NUM_FANS,                                                  \
        "GET_NUM_FANS",                                                                  \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTempFanMgrCmdGetNumFansInput),                                     \
        sizeof(DtIoctlTempFanMgrCmdGetNumFansOutput))

#define DT_IOCTL_CMD_PROPS_TEMPFANMGR_GET_NUM_TEMPSENS                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TEMPFANMGR_CMD_GET_NUM_TEMPSENS,                                              \
        "GET_NUM_TEMPSENS",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTempFanMgrCmdGetNumTempSensorsInput),                              \
        sizeof(DtIoctlTempFanMgrCmdGetNumTempSensorsOutput))

#define DT_IOCTL_CMD_PROPS_TEMPFANMGR_GET_TEMP_PROPS                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TEMPFANMGR_CMD_GET_TEMP_PROPS,                                                \
        "GET_TEMP_PROPS",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTempFanMgrCmdGetTempPropertiesInput),                              \
        sizeof(DtIoctlTempFanMgrCmdGetTempPropertiesOutput))

#define DT_IOCTL_CMD_PROPS_TEMPFANMGR_GET_TEMPERATURE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TEMPFANMGR_CMD_GET_TEMPERATURE,                                               \
        "GET_TEMPERATURE",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTempFanMgrCmdGetTemperatureInput),                                 \
        sizeof(DtIoctlTempFanMgrCmdGetTemperatureOutput))

#define DECL_DT_IOCTL_CMD_PROPS_TEMPFANMGR                                               \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_TEMPFANMGR[] =                 \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_TEMPFANMGR_GET_FAN_STATUS,                                    \
        DT_IOCTL_CMD_PROPS_TEMPFANMGR_GET_NUM_FANS,                                      \
        DT_IOCTL_CMD_PROPS_TEMPFANMGR_GET_NUM_TEMPSENS,                                  \
        DT_IOCTL_CMD_PROPS_TEMPFANMGR_GET_TEMP_PROPS,                                    \
        DT_IOCTL_CMD_PROPS_TEMPFANMGR_GET_TEMPERATURE,                                   \
    }

#define DT_IOCTL_PROPS_TEMPFANMGR_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)            \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_TEMPFANMGR_CMD,                                                         \
        "DT_IOCTL_TEMPFANMGR_CMD",                                                       \
        DT_IOCTL_CMD_PROPS_TEMPFANMGR,                                                   \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_TOD_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_TOD_ADJUST_TIME                                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TOD_CMD_ADJUST_TIME,                                                          \
        "ADJUST_TIME",                                                                   \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTodCmdAdjustTimeInput),                                            \
        0)

#define DT_IOCTL_CMD_PROPS_TOD_GET_PHASE_INCR                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TOD_CMD_GET_PHASE_INCR,                                                       \
        "GET_PHASE_INCR",                                                                \
        FALSE,                                                                           \
        FALSE,                                                                           \
        sizeof(DtIoctlTodCmdGetPhaseIncrInput),                                          \
        sizeof(DtIoctlTodCmdGetPhaseIncrOutput))

#define DT_IOCTL_CMD_PROPS_TOD_GET_PROPERTIES                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TOD_CMD_GET_PROPERTIES,                                                       \
        "GET_PROPERTIES",                                                                \
        FALSE,                                                                           \
        FALSE,                                                                           \
        sizeof(DtIoctlTodCmdGetPropertiesInput),                                         \
        sizeof(DtIoctlTodCmdGetPropertiesOutput))

#define DT_IOCTL_CMD_PROPS_TOD_GET_TIME                                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TOD_CMD_GET_TIME,                                                             \
        "GET_TIME",                                                                      \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTodCmdGetTimeInput),                                               \
        sizeof(DtIoctlTodCmdGetTimeOutput))

#define DT_IOCTL_CMD_PROPS_TOD_SET_PHASE_INCR                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TOD_CMD_SET_PHASE_INCR,                                                       \
        "SET_PHASE_INCR",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTodCmdSetPhaseIncrInput),                                          \
        0)

#define DT_IOCTL_CMD_PROPS_TOD_SET_TIME                                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TOD_CMD_SET_TIME,                                                             \
        "SET_TIME",                                                                      \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTodCmdSetTimeInput),                                               \
        0)

#define DT_IOCTL_CMD_PROPS_TOD_ADJUST_PPM                                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TOD_CMD_ADJUST_PPM,                                                           \
        "ADJUST_PPM",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTodCmdAdjustPpmInput),                                             \
        0)


#define DECL_DT_IOCTL_CMD_PROPS_TOD                                                      \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_TOD[] =                        \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_TOD_ADJUST_TIME,                                              \
        DT_IOCTL_CMD_PROPS_TOD_GET_PHASE_INCR,                                           \
        DT_IOCTL_CMD_PROPS_TOD_GET_PROPERTIES,                                           \
        DT_IOCTL_CMD_PROPS_TOD_GET_TIME,                                                 \
        DT_IOCTL_CMD_PROPS_TOD_SET_PHASE_INCR,                                           \
        DT_IOCTL_CMD_PROPS_TOD_SET_TIME,                                                 \
        DT_IOCTL_CMD_PROPS_TOD_ADJUST_PPM,                                               \
    }

#define DT_IOCTL_PROPS_TOD_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                   \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_TOD_CMD,                                                                \
        "DT_IOCTL_TOD_CMD",                                                              \
        DT_IOCTL_CMD_PROPS_TOD,                                                          \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_TODCLOCKCTRL_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_TODCLOCKCTRL_GET_STATE                                        \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TODCLOCKCTRL_CMD_GET_STATE,                                                   \
        "GET_STATE",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTodClockCtrlCmdGetStateInput),                                     \
        sizeof(DtIoctlTodClockCtrlCmdGetStateOutput))

#define DT_IOCTL_CMD_PROPS_TODCLOCKCTRL_SET_REFERENCE                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TODCLOCKCTRL_CMD_SET_REFERENCE,                                               \
        "SET_PHASE_INCR",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTodClockCtrlCmdSetTodReferenceInput),                              \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_TODCLOCKCTRL                                             \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_TODCLOCKCTRL[] =               \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_TODCLOCKCTRL_GET_STATE,                                       \
        DT_IOCTL_CMD_PROPS_TODCLOCKCTRL_SET_REFERENCE,                                   \
    }

#define DT_IOCTL_PROPS_TODCLOCKCTRL_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)          \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_TODCLOCKCTRL_CMD,                                                       \
        "DT_IOCTL_TODCLOCKCTRL_CMD",                                                     \
        DT_IOCTL_CMD_PROPS_TODCLOCKCTRL,                                                 \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_TSRXFMT_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_TSRXFMT_GET_OPERATIONAL_STATUS                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TSRXFMT_CMD_GET_OPERATIONAL_STATUS,                                           \
        "GET_OPERATIONAL_STATUS",                                                        \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTsRxFmtCmdGetOpStatusInput),                                       \
        sizeof(DtIoctlTsRxFmtCmdGetOpStatusOutput))

#define DT_IOCTL_CMD_PROPS_TSRXFMT_GET_PACKET_MODE                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TSRXFMT_CMD_GET_PACKET_MODE,                                                  \
        "GET_PACKET_MODE",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTsRxFmtCmdGetPckModeInput),                                        \
        sizeof(DtIoctlTsRxFmtCmdGetPckModeOutput))


#define DT_IOCTL_CMD_PROPS_TSRXFMT_GET_PACKET_SIZE                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TSRXFMT_CMD_GET_PACKET_SIZE,                                                  \
        "GET_PACKET_SIZE",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTsRxFmtCmdGetPacketSizeInput),                                     \
        sizeof(DtIoctlTsRxFmtCmdGetPacketSizeOutput))

#define DT_IOCTL_CMD_PROPS_TSRXFMT_GET_SYNC_MODE                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TSRXFMT_CMD_GET_SYNC_MODE,                                                    \
        "GET_SYNC_MODE",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlTsRxFmtCmdGetSyncModeInput),                                       \
        sizeof(DtIoctlTsRxFmtCmdGetSyncModeOutput))

#define DT_IOCTL_CMD_PROPS_TSRXFMT_SET_OPERATIONAL_MODE                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TSRXFMT_CMD_SET_OPERATIONAL_MODE,                                             \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlTsRxFmtCmdSetOpModeInput),                                         \
        0)

#define DT_IOCTL_CMD_PROPS_TSRXFMT_SET_PACKET_MODE                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TSRXFMT_CMD_SET_PACKET_MODE,                                                  \
        "SET_PACKET_MODE",                                                               \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlTsRxFmtCmdSetPckModeInput),                                        \
        0)

#define DT_IOCTL_CMD_PROPS_TSRXFMT_SET_SYNC_MODE                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_TSRXFMT_CMD_SET_SYNC_MODE,                                                    \
        "SET_SYNC_MODE",                                                                 \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlTsRxFmtCmdSetSyncModeInput),                                       \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_TSRXFMT                                                  \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_TSRXFMT[] =                    \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_TSRXFMT_GET_OPERATIONAL_STATUS,                               \
        DT_IOCTL_CMD_PROPS_TSRXFMT_GET_PACKET_MODE,                                      \
        DT_IOCTL_CMD_PROPS_TSRXFMT_GET_PACKET_SIZE,                                      \
        DT_IOCTL_CMD_PROPS_TSRXFMT_GET_SYNC_MODE,                                        \
        DT_IOCTL_CMD_PROPS_TSRXFMT_SET_OPERATIONAL_MODE,                                 \
        DT_IOCTL_CMD_PROPS_TSRXFMT_SET_PACKET_MODE,                                      \
        DT_IOCTL_CMD_PROPS_TSRXFMT_SET_SYNC_MODE,                                        \
    }

#define DT_IOCTL_PROPS_TSRXFMT_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)               \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_TSRXFMT_CMD,                                                            \
        "DT_IOCTL_TSRXFMT_CMD",                                                          \
        DT_IOCTL_CMD_PROPS_TSRXFMT,                                                      \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_VPD_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_VPD_FORMAT                                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_VPD_CMD_FORMAT,                                                               \
        "FORMAT",                                                                        \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlVpdCmdFormatInput),                                                \
        0)

#define DT_IOCTL_CMD_PROPS_VPD_GET_PROPERTIES                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_VPD_CMD_GET_PROPERTIES,                                                       \
        "GET_PROPERTIES",                                                                \
        FALSE,                                                                           \
        FALSE,                                                                           \
        sizeof(DtIoctlVpdCmdGetPropertiesInput),                                         \
        sizeof(DtIoctlVpdCmdGetPropertiesOutput))


#define DT_IOCTL_CMD_PROPS_VPD_ITEM_READ                                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_VPD_CMD_ITEM_READ,                                                            \
        "ITEM_READ",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlVpdCmdItemReadInput),                                              \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlVpdCmdItemReadOutput))

#define DT_IOCTL_CMD_PROPS_VPD_ITEM_WRITE                                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_VPD_CMD_ITEM_WRITE,                                                           \
        "ITEM_WRITE",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlVpdCmdItemWriteInput),                            \
        0)

#define DT_IOCTL_CMD_PROPS_VPD_ITEM_DELETE                                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_VPD_CMD_ITEM_DELETE,                                                          \
        "ITEM_DELETE",                                                                   \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlVpdCmdItemDeleteInput),                                            \
        0)

#define DT_IOCTL_CMD_PROPS_VPD_RAW_READ                                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_VPD_CMD_RAW_READ,                                                             \
        "RAW_READ",                                                                      \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlVpdCmdRawReadInput),                                               \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlVpdCmdRawReadOutput))

#define DT_IOCTL_CMD_PROPS_VPD_RAW_WRITE                                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_VPD_CMD_RAW_WRITE,                                                            \
        "RAW_WRITE",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlVpdCmdRawWriteInput),                             \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_VPD                                                      \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_VPD[] =                        \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_VPD_FORMAT,                                                   \
        DT_IOCTL_CMD_PROPS_VPD_GET_PROPERTIES,                                           \
        DT_IOCTL_CMD_PROPS_VPD_ITEM_READ,                                                \
        DT_IOCTL_CMD_PROPS_VPD_ITEM_WRITE,                                               \
        DT_IOCTL_CMD_PROPS_VPD_ITEM_DELETE,                                              \
        DT_IOCTL_CMD_PROPS_VPD_RAW_READ,                                                 \
        DT_IOCTL_CMD_PROPS_VPD_RAW_WRITE,                                                \
    }

#define DT_IOCTL_PROPS_VPD_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)                   \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_VPD_CMD,                                                                \
        "DT_IOCTL_VPD_CMD",                                                              \
        DT_IOCTL_CMD_PROPS_VPD,                                                          \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA-2116 Local Blocks +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_ATTNCTRL_CMD_2116=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_ATTNCTRL_2116_GET_ATTENUATORS                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ATTNCTRL_CMD_2116_GET_ATTENUATORS,                                            \
        "GET_ATTENUATORS",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAttenuatorsCtrlCmd_2116GetAttenuatorsInput),                       \
        sizeof(DtIoctlAttenuatorsCtrlCmd_2116GetAttenuatorsOutput))

#define DT_IOCTL_CMD_PROPS_ATTNCTRL_2116_SET_ATTENUATORS                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ATTNCTRL_CMD_2116_SET_ATTENUATORS,                                            \
        "SET_ATTENUATORS",                                                               \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAttenuatorsCtrlCmd_2116SetAttenuatorsInput),                       \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_ATTNCTRL_2116                                            \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_ATTNCTRL_2116[] =              \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_ATTNCTRL_2116_GET_ATTENUATORS,                                \
        DT_IOCTL_CMD_PROPS_ATTNCTRL_2116_SET_ATTENUATORS                                 \
    }

#define DT_IOCTL_PROPS_ATTNCTRL_2116_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)         \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_ATTNCTRL_CMD_2116,                                                      \
        "DT_IOCTL_ATTNCTRL_CMD_2116",                                                    \
        DT_IOCTL_CMD_PROPS_ATTNCTRL_2116,                                                \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IO_CMD_2116 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_IO_2116_DAC_RESET                                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IO_CMD_2116_DAC_RESET,                                                        \
        "DAC_RESET",                                                                     \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIoCmd_2116DacResetInput),                                          \
        0)

#define DT_IOCTL_CMD_PROPS_IO_2116_GET_10MHZ_CLOCK_SELECT                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IO_CMD_2116_GET_10MHZ_CLOCK_SELECT,                                           \
        "GET_10MHZ_CLOCK_SELECT",                                                        \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIoCmd_2116Get10MhzClockSelectInput),                               \
        sizeof(DtIoctlIoCmd_2116Get10MhzClockSelectOutput))

#define DT_IOCTL_CMD_PROPS_IO_2116_GET_CALIBRATION_SWITCH                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IO_CMD_2116_GET_CALIBRATION_SWITCH,                                           \
        "GET_CALIBRATION_SWITCH",                                                        \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIoCmd_2116GetCalibrationSwitchInput),                              \
        sizeof(DtIoctlIoCmd_2116GetCalibrationSwitchOutput))

#define DT_IOCTL_CMD_PROPS_IO_2116_GET_CLOCKGEN_STATUS                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IO_CMD_2116_GET_CLOCKGEN_STATUS,                                              \
        "GET_CLOCKGEN_STATUS",                                                           \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIoCmd_2116GetClockGenStatusInput),                                 \
        sizeof(DtIoctlIoCmd_2116GetClockGenStatusOutput))

#define DT_IOCTL_CMD_PROPS_IO_2116_GET_DAC_TX_ENABLE                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IO_CMD_2116_GET_DAC_TX_ENABLE,                                                \
        "GET_DAC_TX_ENABLE",                                                             \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIoCmd_2116GetDacTxEnableInput),                                    \
        sizeof(DtIoctlIoCmd_2116GetDacTxEnableOutput))

#define DT_IOCTL_CMD_PROPS_IO_2116_GET_ONEPPS_TERMINATION                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IO_CMD_2116_GET_ONEPPS_TERMINATION,                                           \
        "GET_ONEPPS_TERMINATION",                                                        \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIoCmd_2116GetOnePpsTerminationInput),                              \
        sizeof(DtIoctlIoCmd_2116GetOnePpsTerminationOutput))

#define DT_IOCTL_CMD_PROPS_IO_2116_SHUTDOWN_CLOCK_GEN                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IO_CMD_2116_INIT_CLOCKGEN,                                                    \
        "SHUTDOWN_CLOCK_GEN",                                                            \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIoCmd_2116InitClockGenInput),                                      \
        0)

#define DT_IOCTL_CMD_PROPS_IO_2116_SHUTDOWN_CLOCK_GEN                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IO_CMD_2116_INIT_CLOCKGEN,                                                    \
        "SHUTDOWN_CLOCK_GEN",                                                            \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIoCmd_2116InitClockGenInput),                                      \
        0)

#define DT_IOCTL_CMD_PROPS_IO_2116_SET_CALIBRATION_SWITCH                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IO_CMD_2116_SET_CALIBRATION_SWITCH,                                           \
        "SET_CALIBRATION_SWITCH",                                                        \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIoCmd_2116SetCalibrationSwitchInput),                              \
        0)

#define DT_IOCTL_CMD_PROPS_IO_2116_SET_DAC_TX_ENABLE                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IO_CMD_2116_SET_DAC_TX_ENABLE,                                                \
        "SET_DAC_TX_ENABLE",                                                             \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIoCmd_2116SetDacTxEnableInput),                                    \
        0)

#define DT_IOCTL_CMD_PROPS_IO_2116_SET_ONEPPS_TERMINATION                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IO_CMD_2116_SET_ONEPPS_TERMINATION,                                           \
        "SET_ONEPPS_TERMINATION",                                                        \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIoCmd_2116SetOnePpsTerminationInput),                              \
        0)
        
#define DECL_DT_IOCTL_CMD_PROPS_IO_2116                                                  \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IO_2116[] =                    \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IO_2116_DAC_RESET,                                            \
        DT_IOCTL_CMD_PROPS_IO_2116_GET_10MHZ_CLOCK_SELECT,                               \
        DT_IOCTL_CMD_PROPS_IO_2116_GET_CALIBRATION_SWITCH,                               \
        DT_IOCTL_CMD_PROPS_IO_2116_GET_CLOCKGEN_STATUS,                                  \
        DT_IOCTL_CMD_PROPS_IO_2116_GET_DAC_TX_ENABLE,                                    \
        DT_IOCTL_CMD_PROPS_IO_2116_GET_ONEPPS_TERMINATION,                               \
        DT_IOCTL_CMD_PROPS_IO_2116_SHUTDOWN_CLOCK_GEN,                                   \
        DT_IOCTL_CMD_PROPS_IO_2116_SET_CALIBRATION_SWITCH,                               \
        DT_IOCTL_CMD_PROPS_IO_2116_SET_DAC_TX_ENABLE,                                    \
        DT_IOCTL_CMD_PROPS_IO_2116_SET_ONEPPS_TERMINATION                                \
    }

#define DT_IOCTL_PROPS_IO_2116_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)               \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IO_CMD_2116,                                                            \
        "DT_IOCTL_IO_CMD_2116",                                                          \
        DT_IOCTL_CMD_PROPS_IO_2116,                                                      \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DTA-2132 Local Blocks +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_AD5320_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


#define DT_IOCTL_CMD_PROPS_AD5320_2132_GET_GAIN_CONTROL                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_AD5320_CMD_2132_GET_GAIN_CONTROL,                                             \
        "GET_GAIN_CONTROL",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAd5320Cmd_2132GetGainControlInput),                                \
        sizeof(DtIoctlAd5320Cmd_2132GetGainControlOutput))

#define DT_IOCTL_CMD_PROPS_AD5320_2132_GET_GAIN_CTRL_PARS                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_AD5320_CMD_2132_GET_GAIN_CTRL_PARS,                                           \
        "GET_GAIN_CTRL_PARS",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAd5320Cmd_2132GetGainCtrlParsInput),                               \
        sizeof(DtIoctlAd5320Cmd_2132GetGainCtrlParsOutput))

#define DT_IOCTL_CMD_PROPS_AD5320_2132_GET_GAIN_STATUS                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_AD5320_CMD_2132_GET_GAIN_STATUS,                                              \
        "GET_GAIN_STATUS",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAd5320Cmd_2132GetGainStatusInput),                                 \
        sizeof(DtIoctlAd5320Cmd_2132GetGainStatusOutput))

#define DT_IOCTL_CMD_PROPS_AD5320_2132_GET_OPERATIONAL_MODE                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_AD5320_CMD_2132_GET_OPERATIONAL_MODE,                                         \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAd5320Cmd_2132GetOpModeInput),                                     \
        sizeof(DtIoctlAd5320Cmd_2132GetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_AD5320_2132_SET_GAIN_CONTROL                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_AD5320_CMD_2132_SET_GAIN_CONTROL,                                             \
        "SET_GAIN_CONTROL",                                                              \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAd5320Cmd_2132SetGainControlInput),                                \
        0)


#define DT_IOCTL_CMD_PROPS_AD5320_2132_SET_GAIN_CTRL_PARS                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_AD5320_CMD_2132_SET_GAIN_CTRL_PARS,                                           \
        "SET_GAIN_CTRL_PARS",                                                            \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAd5320Cmd_2132SetGainCtrlParsInput),                               \
        0)

#define DT_IOCTL_CMD_PROPS_AD5320_2132_SET_OPERATIONAL_MODE                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_AD5320_CMD_2132_SET_OPERATIONAL_MODE,                                         \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAd5320Cmd_2132SetOpModeInput),                                     \
        0)


#define DECL_DT_IOCTL_CMD_PROPS_AD5320_2132                                              \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_AD5320_2132[] =                \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_AD5320_2132_GET_GAIN_CONTROL,                                 \
        DT_IOCTL_CMD_PROPS_AD5320_2132_GET_GAIN_CTRL_PARS,                               \
        DT_IOCTL_CMD_PROPS_AD5320_2132_GET_GAIN_STATUS,                                  \
        DT_IOCTL_CMD_PROPS_AD5320_2132_GET_OPERATIONAL_MODE,                             \
        DT_IOCTL_CMD_PROPS_AD5320_2132_SET_GAIN_CONTROL,                                 \
        DT_IOCTL_CMD_PROPS_AD5320_2132_SET_GAIN_CTRL_PARS,                               \
        DT_IOCTL_CMD_PROPS_AD5320_2132_SET_OPERATIONAL_MODE,                             \
    }

#define DT_IOCTL_PROPS_AD5320_CMD_2132(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)           \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_AD5320_CMD_2132,                                                        \
        "DT_IOCTL_AD5320_CMD_2132",                                                      \
        DT_IOCTL_CMD_PROPS_AD5320_2132,                                                  \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_ADS4246_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


#define DT_IOCTL_CMD_PROPS_ADS4246_2132_GET_OPERATIONAL_MODE                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ADS4246_CMD_2132_GET_OPERATIONAL_MODE,                                        \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlAds4246Cmd_2132GetOpModeInput),                                    \
        sizeof(DtIoctlAds4246Cmd_2132GetOpModeOutput))


#define DT_IOCTL_CMD_PROPS_ADS4246_2132_SET_OPERATIONAL_MODE                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_ADS4246_CMD_2132_SET_OPERATIONAL_MODE,                                        \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlAds4246Cmd_2132SetOpModeInput),                                    \
        0)


#define DECL_DT_IOCTL_CMD_PROPS_ADS4246_2132                                             \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_ADS4246_2132[] =               \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_ADS4246_2132_GET_OPERATIONAL_MODE,                            \
        DT_IOCTL_CMD_PROPS_ADS4246_2132_SET_OPERATIONAL_MODE,                            \
    }

#define DT_IOCTL_PROPS_ADS4246_CMD_2132(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)          \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_ADS4246_CMD_2132,                                                       \
        "DT_IOCTL_ADS4246_CMD_2132",                                                     \
        DT_IOCTL_CMD_PROPS_ADS4246_2132,                                                 \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_DATASTMUX_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


#define DT_IOCTL_CMD_PROPS_DATASTMUX_2132_GET_SELECTION                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DATASTMUX_CMD_2132_GET_SELECTION,                                             \
        "GET_SELECTION",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlDataStMuxCmd_2132GetSelectionInput),                               \
        sizeof(DtIoctlDataStMuxCmd_2132GetSelectionOutput))

#define DT_IOCTL_CMD_PROPS_DATASTMUX_2132_SET_SELECTION                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_DATASTMUX_CMD_2132_SET_SELECTION,                                             \
        "SET_SELECTION",                                                                 \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlDataStMuxCmd_2132SetSelectionInput),                               \
        0)


#define DECL_DT_IOCTL_CMD_PROPS_DATASTMUX_2132                                           \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_DATASTMUX_2132[] =             \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_DATASTMUX_2132_GET_SELECTION,                                 \
        DT_IOCTL_CMD_PROPS_DATASTMUX_2132_SET_SELECTION,                                 \
    }

#define DT_IOCTL_PROPS_DATASTMUX_CMD_2132(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)        \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_DATASTMUX_CMD_2132,                                                     \
        "DT_IOCTL_DATASTMUX_CMD_2132",                                                   \
        DT_IOCTL_CMD_PROPS_DATASTMUX_2132,                                               \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQCGRAB_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


#define DT_IOCTL_CMD_PROPS_IQCGRAB_2132_GET_IQ_POINTS                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQCGRAB_CMD_2132_GET_IQ_POINTS,                                               \
        "GET_COEFS",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqCGrabCmd_2132GetIqPointsInput),                                  \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlIqCGrabCmd_2132GetIqPointsOutput))


#define DECL_DT_IOCTL_CMD_PROPS_IQCGRAB_2132                                             \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQCGRAB_2132[] =               \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQCGRAB_2132_GET_IQ_POINTS,                                   \
    }

#define DT_IOCTL_PROPS_IQCGRAB_CMD_2132(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)          \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQCGRAB_CMD_2132,                                                       \
        "DT_IOCTL_IQCGRAB_CMD_2132",                                                     \
        DT_IOCTL_CMD_PROPS_IQCGRAB_2132,                                                 \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQFIR_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


#define DT_IOCTL_CMD_PROPS_IQFIR_2132_GET_CONFIG                                         \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQFIR_CMD_2132_GET_CONFIG,                                                    \
        "GET_CONFIG",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqFirCmd_2132GetConfigInput),                                      \
        sizeof(DtIoctlIqFirCmd_2132GetConfigOutput))

#define DT_IOCTL_CMD_PROPS_IQFIR_2132_GET_COEFS                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQFIR_CMD_2132_GET_COEFS,                                                     \
        "GET_COEFS",                                                                     \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqFirCmd_2132GetCoefsInput),                                       \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlIqFirCmd_2132GetCoefsOutput))

#define DT_IOCTL_CMD_PROPS_IQFIR_2132_SET_COEFS                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQFIR_CMD_2132_SET_COEFS,                                                     \
        "SET_COEFS",                                                                     \
        TRUE,                                                                            \
        TRUE,                                                                            \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlIqFirCmd_2132SetCoefsInput),                      \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_IQFIR_2132                                               \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQFIR_2132[] =                 \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQFIR_2132_GET_CONFIG,                                        \
        DT_IOCTL_CMD_PROPS_IQFIR_2132_GET_COEFS,                                         \
        DT_IOCTL_CMD_PROPS_IQFIR_2132_SET_COEFS,                                         \
    }

#define DT_IOCTL_PROPS_IQFIR_CMD_2132(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)            \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQFIR_CMD_2132,                                                         \
        "DT_IOCTL_IQFIR_CMD_2132",                                                       \
        DT_IOCTL_CMD_PROPS_IQFIR_2132,                                                   \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQPWR_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


#define DT_IOCTL_CMD_PROPS_IQPWR_2132_GET_EST_POWER                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQPWR_CMD_2132_GET_EST_POWER,                                                 \
        "GET_EST_POWER",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqPwrCmd_2132GetEstPowerInput),                                    \
        sizeof(DtIoctlIqPwrCmd_2132GetEstPowerOutput))


#define DT_IOCTL_CMD_PROPS_IQPWR_2132_GET_EST_WINDOW                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQPWR_CMD_2132_GET_EST_WINDOW,                                                \
        "GET_EST_WINDOW",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqPwrCmd_2132GetEstWindowInput),                                   \
        sizeof(DtIoctlIqPwrCmd_2132GetEstWindowOutput))

#define DT_IOCTL_CMD_PROPS_IQPWR_2132_SET_EST_WINDOW                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQPWR_CMD_2132_SET_EST_WINDOW,                                                \
        "SET_EST_WINDOW",                                                                \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqPwrCmd_2132SetEstWindowInput),                                   \
        0)


#define DECL_DT_IOCTL_CMD_PROPS_IQPWR_2132                                               \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQPWR_2132[] =                 \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQPWR_2132_GET_EST_POWER,                                     \
        DT_IOCTL_CMD_PROPS_IQPWR_2132_GET_EST_WINDOW,                                    \
        DT_IOCTL_CMD_PROPS_IQPWR_2132_SET_EST_WINDOW,                                    \
    }

#define DT_IOCTL_PROPS_IQPWR_CMD_2132(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)            \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQPWR_CMD_2132,                                                         \
        "DT_IOCTL_IQPWR_CMD_2132",                                                       \
        DT_IOCTL_CMD_PROPS_IQPWR_2132,                                                   \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_IQSRC2132_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


#define DT_IOCTL_CMD_PROPS_IQSRC2132_2132_GET_SAMPLE_RATE_CONVERSION                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQSRC2132_CMD_2132_GET_SAMPLE_RATE_CONVERSION,                                \
        "GET_SAMPLE_RATE_CONVERSION",                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlIqSrc2132Cmd_2132GetSampleRateConversionInput),                    \
        sizeof(DtIoctlIqSrc2132Cmd_2132GetSampleRateConversionOutput))

#define DT_IOCTL_CMD_PROPS_IQSRC2132_2132_SET_SAMPLE_RATE_CONVERSION                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_IQSRC2132_CMD_2132_SET_SAMPLE_RATE_CONVERSION,                                \
        "SET_SAMPLE_RATE_CONVERSION",                                                    \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlIqSrc2132Cmd_2132SetSampleRateConversionInput),                    \
        0)


#define DECL_DT_IOCTL_CMD_PROPS_IQSRC2132_2132                                           \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_IQSRC2132_2132[] =             \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_IQSRC2132_2132_GET_SAMPLE_RATE_CONVERSION,                    \
        DT_IOCTL_CMD_PROPS_IQSRC2132_2132_SET_SAMPLE_RATE_CONVERSION,                    \
    }

#define DT_IOCTL_PROPS_IQSRC2132_CMD_2132(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)        \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_IQSRC2132_CMD_2132,                                                     \
        "DT_IOCTL_IQSRC2132_CMD_2132",                                                   \
        DT_IOCTL_CMD_PROPS_IQSRC2132_2132,                                               \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_LNBH25_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_LNBH25_2132_ENABLE_TONE                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_LNBH25_CMD_2132_ENABLE_TONE,                                                  \
        "ENABLE_TONE",                                                                   \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlLnbh25Cmd_2132EnableToneInput),                                    \
        0)

#define DT_IOCTL_CMD_PROPS_LNBH25_2132_SEND_TONEBURST                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_LNBH25_CMD_2132_SEND_TONEBURST,                                               \
        "SEND_TONEBURST",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlLnbh25Cmd_2132SendToneBurstInput),                                 \
        0)

#define DT_IOCTL_CMD_PROPS_LNBH25_2132_SEND_MESSAGE                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_LNBH25_CMD_2132_SEND_MESSAGE,                                                 \
        "SEND_MESSAGE",                                                                  \
        TRUE,                                                                            \
        FALSE,                                                                           \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlLnbh25Cmd_2132SendMessageInput),                  \
        0)

#define DT_IOCTL_CMD_PROPS_LNBH25_2132_SEND_RECEIVE_MESSAGE                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_LNBH25_CMD_2132_SEND_RECEIVE_MESSAGE,                                         \
        "SEND_RECEIVE_MESSAGE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlLnbh25Cmd_2132SendReceiveMessageInput),           \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlLnbh25Cmd_2132SendReceiveMessageOutput))




#define DECL_DT_IOCTL_CMD_PROPS_LNBH25_2132                                              \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_LNBH25_2132[] =                \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_LNBH25_2132_ENABLE_TONE,                                      \
        DT_IOCTL_CMD_PROPS_LNBH25_2132_SEND_TONEBURST,                                   \
        DT_IOCTL_CMD_PROPS_LNBH25_2132_SEND_MESSAGE,                                     \
        DT_IOCTL_CMD_PROPS_LNBH25_2132_SEND_RECEIVE_MESSAGE                              \
    }

#define DT_IOCTL_PROPS_LNBH25_CMD_2132(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)           \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_LNBH25_CMD_2132,                                                        \
        "DT_IOCTL_LNBH25_CMD_2132",                                                      \
        DT_IOCTL_CMD_PROPS_LNBH25_2132,                                                  \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )




//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_S2CRDEMOD_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_DEMOD_RESET                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2CRDEMOD_CMD_2132_DEMOD_RESET,                                               \
        "DEMOD_RESET",                                                                   \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlS2CrDemodCmd_2132DemodResetInput),                                 \
        0)

#define DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_PL_INFORMATION                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2CRDEMOD_CMD_2132_GET_PL_INFORMATION,                                        \
        "GET_PL_INFORMATION",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlS2CrDemodCmd_2132GetPlInformationInput),                           \
        sizeof(DtIoctlS2CrDemodCmd_2132GetPlInformationOutput))

#define DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_OPERATIONAL_MODE                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2CRDEMOD_CMD_2132_GET_OPERATIONAL_MODE,                                      \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlS2CrDemodCmd_2132GetOpModeInput),                                  \
        sizeof(DtIoctlS2CrDemodCmd_2132GetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_ROLL_OFF                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2CRDEMOD_CMD_2132_GET_ROLL_OFF,                                              \
        "GET_ROLL_OFF",                                                                  \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlS2CrDemodCmd_2132GetRollOffInput),                                 \
        sizeof(DtIoctlS2CrDemodCmd_2132GetRollOffOutput))

#define DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_SNR_INFO                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2CRDEMOD_CMD_2132_GET_SNR_INFO,                                              \
        "GET_SNR_INFO",                                                                  \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlS2CrDemodCmd_2132GetSnrInfoInput),                                 \
        sizeof(DtIoctlS2CrDemodCmd_2132GetSnrInfoOutput))

#define DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_SPECTRUM_INV                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2CRDEMOD_CMD_2132_GET_SPECTRUM_INV,                                          \
        "GET_SPECTRUM_INV",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlS2CrDemodCmd_2132GetSpectrumInvInput),                             \
        sizeof(DtIoctlS2CrDemodCmd_2132GetSpectrumInvOutput))

#define DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_STATUS                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2CRDEMOD_CMD_2132_GET_STATUS,                                                \
        "GET_STATUS",                                                                    \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlS2CrDemodCmd_2132GetStatusInput),                                  \
        sizeof(DtIoctlS2CrDemodCmd_2132GetStatusOutput))

#define DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_SYMBOL_RATE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2CRDEMOD_CMD_2132_GET_SYMBOL_RATE,                                           \
        "GET_SYMBOL_RATE",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlS2CrDemodCmd_2132GetSymbolRateInput),                              \
        sizeof(DtIoctlS2CrDemodCmd_2132GetSymbolRateOutput))

#define DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_SET_OPERATIONAL_MODE                           \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2CRDEMOD_CMD_2132_SET_OPERATIONAL_MODE,                                      \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlS2CrDemodCmd_2132SetOpModeInput),                                  \
        0)

#define DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_SET_PL_INFORMATION                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2CRDEMOD_CMD_2132_SET_PL_INFORMATION,                                        \
        "GET_PL_INFORMATION",                                                            \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlS2CrDemodCmd_2132SetPlInformationInput),                           \
        0)

#define DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_SET_SPECTRUM_INV                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2CRDEMOD_CMD_2132_SET_SPECTRUM_INV,                                          \
        "SET_SPECTRUM_INV",                                                              \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlS2CrDemodCmd_2132SetSpectrumInvInput),                             \
        0)

#define DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_SET_SYMBOL_RATE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2CRDEMOD_CMD_2132_SET_SYMBOL_RATE,                                           \
        "SET_SYMBOL_RATE",                                                               \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlS2CrDemodCmd_2132SetSymbolRateInput),                              \
        0)

#define DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_SOFT_RESET                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2CRDEMOD_CMD_2132_SOFT_RESET,                                                \
        "SOFT_RESET",                                                                    \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlS2CrDemodCmd_2132SoftResetInput),                                  \
        0)

#define DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_COUNTERS                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2CRDEMOD_CMD_2132_GET_COUNTERS,                                              \
        "GET_COUNTERS",                                                                  \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlS2CrDemodCmd_2132GetCountersInput),                                \
        sizeof(DtIoctlS2CrDemodCmd_2132GetCountersOutput))

#define DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_FREQ_OFFSET                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2CRDEMOD_CMD_2132_GET_FREQ_OFFSET,                                           \
        "GET_FREQ_OFFSET",                                                               \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlS2CrDemodCmd_2132GetFreqOffsetInput),                              \
        sizeof(DtIoctlS2CrDemodCmd_2132GetFreqOffsetOutput))

#define DECL_DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132                                           \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132[] =             \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_DEMOD_RESET,                                   \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_OPERATIONAL_MODE,                          \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_PL_INFORMATION,                            \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_ROLL_OFF,                                  \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_SNR_INFO,                                  \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_SPECTRUM_INV,                              \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_STATUS,                                    \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_SYMBOL_RATE,                               \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_SET_OPERATIONAL_MODE,                          \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_SET_PL_INFORMATION,                            \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_SET_SPECTRUM_INV,                              \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_SET_SYMBOL_RATE,                               \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_SOFT_RESET,                                    \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_COUNTERS,                                  \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132_GET_FREQ_OFFSET                                \
    }

#define DT_IOCTL_PROPS_S2CRDEMOD_CMD_2132(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)        \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_S2CRDEMOD_CMD_2132,                                                     \
        "DT_IOCTL_S2CRDEMOD_CMD_2132",                                                   \
        DT_IOCTL_CMD_PROPS_S2CRDEMOD_2132,                                               \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_S2STATS_CMD_2132 +=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_S2STATS_2132_CLEAR_STATISTICS                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2STATS_CMD_2132_CLEAR_STATISTICS,                                            \
        "CLEAR_STATISTICS",                                                              \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlS2StatsCmd_2132ClearStatisticsInput),                              \
        0)

#define DT_IOCTL_CMD_PROPS_S2STATS_2132_GET_STATISTICS_STATUS                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2STATS_CMD_2132_GET_STATISTICS_STATUS,                                       \
        "GET_STATISTICS_STATUS",                                                         \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlS2StatsCmd_2132GetStatisticsStatusInput),                          \
        sizeof(DtIoctlS2StatsCmd_2132GetStatisticsStatusOutput))

#define DT_IOCTL_CMD_PROPS_S2STATS_2132_GET_OPERATIONAL_MODE                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2STATS_CMD_2132_GET_OPERATIONAL_MODE,                                        \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlS2StatsCmd_2132GetOpModeInput),                                    \
        sizeof(DtIoctlS2StatsCmd_2132GetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_S2STATS_2132_READ_STATISTICS                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2STATS_CMD_2132_READ_STATISTICS,                                             \
        "READ_STATISTICS",                                                               \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlS2StatsCmd_2132ReadStatisticsInput),                               \
        DT_IOCTL_DYNAMIC_SIZEOF(DtIoctlS2StatsCmd_2132ReadStatisticsOutput))


#define DT_IOCTL_CMD_PROPS_S2STATS_2132_SET_OPERATIONAL_MODE                             \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_S2STATS_CMD_2132_SET_OPERATIONAL_MODE,                                        \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlS2StatsCmd_2132SetOpModeInput),                                    \
        0)


#define DECL_DT_IOCTL_CMD_PROPS_S2STATS_2132                                             \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_S2STATS_2132[] =               \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_S2STATS_2132_CLEAR_STATISTICS,                                \
        DT_IOCTL_CMD_PROPS_S2STATS_2132_GET_STATISTICS_STATUS,                           \
        DT_IOCTL_CMD_PROPS_S2STATS_2132_GET_OPERATIONAL_MODE,                            \
        DT_IOCTL_CMD_PROPS_S2STATS_2132_READ_STATISTICS,                                 \
        DT_IOCTL_CMD_PROPS_S2STATS_2132_SET_OPERATIONAL_MODE,                            \
    }

#define DT_IOCTL_PROPS_S2STATS_CMD_2132(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)          \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_S2STATS_CMD_2132,                                                       \
        "DT_IOCTL_S2STATS_CMD_2132",                                                     \
        DT_IOCTL_CMD_PROPS_S2STATS_2132,                                                 \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+= DT_IOCTL_PROPS_CONSTSINK_CMD +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_CONSTSINK_GET_CHECK_DATA                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSINK_CMD_GET_CHECK_DATA,                                                 \
        "GET_CHECK_DATA",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlConstSinkCmdGetCheckDataInput),                                    \
        sizeof(DtIoctlConstSinkCmdGetCheckDataOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSINK_GET_DATA_ERROR_CNT                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSINK_CMD_GET_DATA_ERROR_CNT,                                             \
        "GET_DATA_ERROR_CNT",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlConstSinkCmdGetDataErrorCntInput),                                 \
        sizeof(DtIoctlConstSinkCmdGetDataErrorCntOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSINK_GET_DATA_PATTERN                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSINK_CMD_GET_DATA_PATTERN,                                               \
        "GET_DATA_PATTERN",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlConstSinkCmdGetDataPatternInput),                                  \
        sizeof(DtIoctlConstSinkCmdGetDataPatternOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSINK_GET_DATA_RATE                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSINK_CMD_GET_DATA_RATE,                                                  \
        "GET_DATA_RATE",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlConstSinkCmdGetDataRateInput),                                     \
        sizeof(DtIoctlConstSinkCmdGetDataRateOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSINK_GET_OPERATIONAL_MODE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSINK_CMD_GET_OPERATIONAL_MODE,                                           \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlConstSinkCmdGetOpModeInput),                                       \
        sizeof(DtIoctlConstSinkCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSINK_GET_TEST_INTERVAL                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSINK_CMD_GET_TEST_INTERVAL,                                              \
        "GET_TEST_INTERVAL",                                                             \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlConstSinkCmdGetTestIntervalInput),                                 \
        sizeof(DtIoctlConstSinkCmdGetTestIntervalOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSINK_GET_UNDERFLOW_CNT                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSINK_CMD_GET_UNDERFLOW_CNT,                                              \
        "GET_UNDERFLOW_CNT",                                                             \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlConstSinkCmdGetUnderflowCntInput),                                 \
        sizeof(DtIoctlConstSinkCmdGetUnderflowCntOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSINK_RUN_TEST                                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSINK_CMD_RUN_TEST,                                                       \
        "RUN_TEST",                                                                      \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSinkCmdRunTestInput),                                         \
        sizeof(DtIoctlConstSinkCmdRunTestOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSINK_SET_CHECK_DATA                                      \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSINK_CMD_SET_CHECK_DATA,                                                 \
        "SET_CHECK_DATA",                                                                \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSinkCmdSetCheckDataInput),                                    \
        0)

#define DT_IOCTL_CMD_PROPS_CONSTSINK_SET_DATA_PATTERN                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSINK_CMD_SET_DATA_PATTERN,                                               \
        "SET_DATA_PATTERN",                                                              \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSinkCmdSetDataPatternInput),                                  \
        0)

#define DT_IOCTL_CMD_PROPS_CONSTSINK_SET_DATA_RATE                                       \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSINK_CMD_SET_DATA_RATE,                                                  \
        "SET_DATA_RATE",                                                                 \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSinkCmdSetDataRateInput),                                     \
        0)

#define DT_IOCTL_CMD_PROPS_CONSTSINK_SET_OPERATIONAL_MODE                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSINK_CMD_SET_OPERATIONAL_MODE,                                           \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSinkCmdSetOpModeInput),                                       \
        0)

#define DT_IOCTL_CMD_PROPS_CONSTSINK_SET_TEST_INTERVAL                                   \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSINK_CMD_SET_TEST_INTERVAL,                                              \
        "SET_TEST_INTERVAL",                                                             \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSinkCmdSetTestIntervalInput),                                 \
        0)

#define DT_IOCTL_CMD_PROPS_CONSTSINK_START_TEST_INTERVAL                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSINK_CMD_START_TEST_INTERVAL,                                            \
        "START_TEST_INTERVAL",                                                           \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSinkCmdStartTestItvInput),                                    \
        0)

#define DT_IOCTL_CMD_PROPS_CONSTSINK_WAIT_FOR_TEST_ITV_DONE                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSINK_CMD_WAIT_FOR_TEST_ITV_DONE,                                         \
        "WAIT_FOR_TEST_ITV_DONE",                                                        \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSinkCmdWaitForTestItvDoneInput),                              \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_CONSTSINK                                                \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_CONSTSINK[] =                  \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_CONSTSINK_GET_CHECK_DATA,                                     \
        DT_IOCTL_CMD_PROPS_CONSTSINK_GET_DATA_ERROR_CNT,                                 \
        DT_IOCTL_CMD_PROPS_CONSTSINK_GET_DATA_PATTERN,                                   \
        DT_IOCTL_CMD_PROPS_CONSTSINK_GET_DATA_RATE,                                      \
        DT_IOCTL_CMD_PROPS_CONSTSINK_GET_OPERATIONAL_MODE,                               \
        DT_IOCTL_CMD_PROPS_CONSTSINK_GET_TEST_INTERVAL,                                  \
        DT_IOCTL_CMD_PROPS_CONSTSINK_GET_UNDERFLOW_CNT,                                  \
        DT_IOCTL_CMD_PROPS_CONSTSINK_RUN_TEST,                                           \
        DT_IOCTL_CMD_PROPS_CONSTSINK_SET_CHECK_DATA,                                     \
        DT_IOCTL_CMD_PROPS_CONSTSINK_SET_DATA_PATTERN,                                   \
        DT_IOCTL_CMD_PROPS_CONSTSINK_SET_DATA_RATE,                                      \
        DT_IOCTL_CMD_PROPS_CONSTSINK_SET_OPERATIONAL_MODE,                               \
        DT_IOCTL_CMD_PROPS_CONSTSINK_SET_TEST_INTERVAL,                                  \
        DT_IOCTL_CMD_PROPS_CONSTSINK_START_TEST_INTERVAL,                                \
        DT_IOCTL_CMD_PROPS_CONSTSINK_WAIT_FOR_TEST_ITV_DONE,                             \
    }

#define DT_IOCTL_PROPS_CONSTSINK_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)             \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_CONSTSINK_CMD,                                                          \
        "DT_IOCTL_CONSTSINK_CMD",                                                        \
        DT_IOCTL_CMD_PROPS_CONSTSINK,                                                    \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+ DT_IOCTL_PROPS_CONSTSOURCE_CMD =+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

#define DT_IOCTL_CMD_PROPS_CONSTSOURCE_GET_CHECK_DATA                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSOURCE_CMD_GET_CHECK_DATA,                                               \
        "GET_CHECK_DATA",                                                                \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlConstSourceCmdGetCheckDataInput),                                  \
        sizeof(DtIoctlConstSourceCmdGetCheckDataOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSOURCE_GET_DATA_ERROR_CNT                                \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSOURCE_CMD_GET_DATA_ERROR_CNT,                                           \
        "GET_DATA_ERROR_CNT",                                                            \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlConstSourceCmdGetDataErrorCntInput),                               \
        sizeof(DtIoctlConstSourceCmdGetDataErrorCntOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSOURCE_GET_DATA_PATTERN                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSOURCE_CMD_GET_DATA_PATTERN,                                             \
        "GET_DATA_PATTERN",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlConstSourceCmdGetDataPatternInput),                                \
        sizeof(DtIoctlConstSourceCmdGetDataPatternOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSOURCE_GET_DATA_RATE                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSOURCE_CMD_GET_DATA_RATE,                                                \
        "GET_DATA_RATE",                                                                 \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlConstSourceCmdGetDataRateInput),                                   \
        sizeof(DtIoctlConstSourceCmdGetDataRateOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSOURCE_GET_OPERATIONAL_MODE                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSOURCE_CMD_GET_OPERATIONAL_MODE,                                         \
        "GET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlConstSourceCmdGetOpModeInput),                                     \
        sizeof(DtIoctlConstSourceCmdGetOpModeOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSOURCE_GET_OVERFLOW_CNT                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSOURCE_CMD_GET_OVERFLOW_CNT,                                             \
        "GET_OVERFLOW_CNT",                                                              \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlConstSourceCmdGetOverflowCntInput),                                \
        sizeof(DtIoctlConstSourceCmdGetOverflowCntOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSOURCE_GET_TEST_INTERVAL                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSOURCE_CMD_GET_TEST_INTERVAL,                                            \
        "GET_TEST_INTERVAL",                                                             \
        TRUE,                                                                            \
        FALSE,                                                                           \
        sizeof(DtIoctlConstSourceCmdGetTestIntervalInput),                               \
        sizeof(DtIoctlConstSourceCmdGetTestIntervalOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSOURCE_RUN_TEST                                          \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSOURCE_CMD_RUN_TEST,                                                     \
        "RUN_TEST",                                                                      \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSourceCmdRunTestInput),                                       \
        sizeof(DtIoctlConstSourceCmdRunTestOutput))

#define DT_IOCTL_CMD_PROPS_CONSTSOURCE_SET_CHECK_DATA                                    \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSOURCE_CMD_SET_CHECK_DATA,                                               \
        "SET_CHECK_DATA",                                                                \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSourceCmdSetCheckDataInput),                                  \
        0)

#define DT_IOCTL_CMD_PROPS_CONSTSOURCE_SET_DATA_PATTERN                                  \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSOURCE_CMD_SET_DATA_PATTERN,                                             \
        "SET_DATA_PATTERN",                                                              \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSourceCmdSetDataPatternInput),                                \
        0)

#define DT_IOCTL_CMD_PROPS_CONSTSOURCE_SET_DATA_RATE                                     \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSOURCE_CMD_SET_DATA_RATE,                                                \
        "SET_DATA_RATE",                                                                 \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSourceCmdSetDataRateInput),                                   \
        0)

#define DT_IOCTL_CMD_PROPS_CONSTSOURCE_SET_OPERATIONAL_MODE                              \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSOURCE_CMD_SET_OPERATIONAL_MODE,                                         \
        "SET_OPERATIONAL_MODE",                                                          \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSourceCmdSetOpModeInput),                                     \
        0)

#define DT_IOCTL_CMD_PROPS_CONSTSOURCE_SET_TEST_INTERVAL                                 \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSOURCE_CMD_SET_TEST_INTERVAL,                                            \
        "SET_TEST_INTERVAL",                                                             \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSourceCmdSetTestIntervalInput),                               \
        0)

#define DT_IOCTL_CMD_PROPS_CONSTSOURCE_START_TEST_INTERVAL                               \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSOURCE_CMD_START_TEST_INTERVAL,                                          \
        "START_TEST_INTERVAL",                                                           \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSourceCmdStartTestItvInput),                                  \
        0)

#define DT_IOCTL_CMD_PROPS_CONSTSOURCE_WAIT_FOR_TEST_ITV_DONE                            \
    INIT_DT_IOCTL_CMD_PROPS(                                                             \
        DT_CONSTSOURCE_CMD_WAIT_FOR_TEST_ITV_DONE,                                       \
        "WAIT_FOR_TEST_ITV_DONE",                                                        \
        TRUE,                                                                            \
        TRUE,                                                                            \
        sizeof(DtIoctlConstSourceCmdWaitForTestItvDoneInput),                            \
        0)

#define DECL_DT_IOCTL_CMD_PROPS_CONSTSOURCE                                              \
    static const DtIoctlPropertiesCmd  DT_IOCTL_CMD_PROPS_CONSTSOURCE[] =                \
    {                                                                                    \
        DT_IOCTL_CMD_PROPS_CONSTSOURCE_GET_DATA_PATTERN,                                 \
        DT_IOCTL_CMD_PROPS_CONSTSOURCE_GET_DATA_RATE,                                    \
        DT_IOCTL_CMD_PROPS_CONSTSOURCE_GET_OPERATIONAL_MODE,                             \
        DT_IOCTL_CMD_PROPS_CONSTSOURCE_GET_OVERFLOW_CNT,                                 \
        DT_IOCTL_CMD_PROPS_CONSTSOURCE_GET_TEST_INTERVAL,                                \
        DT_IOCTL_CMD_PROPS_CONSTSOURCE_RUN_TEST,                                         \
        DT_IOCTL_CMD_PROPS_CONSTSOURCE_SET_DATA_PATTERN,                                 \
        DT_IOCTL_CMD_PROPS_CONSTSOURCE_SET_DATA_RATE,                                    \
        DT_IOCTL_CMD_PROPS_CONSTSOURCE_SET_OPERATIONAL_MODE,                             \
        DT_IOCTL_CMD_PROPS_CONSTSOURCE_SET_TEST_INTERVAL,                                \
        DT_IOCTL_CMD_PROPS_CONSTSOURCE_START_TEST_INTERVAL,                              \
        DT_IOCTL_CMD_PROPS_CONSTSOURCE_WAIT_FOR_TEST_ITV_DONE,                           \
    }

#define DT_IOCTL_PROPS_CONSTSOURCE_CMD(ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC)           \
    INIT_DT_IOCTL_PROPS(                                                                 \
        DT_IOCTL_CONSTSOURCE_CMD,                                                        \
        "DT_IOCTL_CONSTSOURCE_CMD",                                                      \
        DT_IOCTL_CMD_PROPS_CONSTSOURCE,                                                  \
        ON_IOCTL_FUNC, DYN_SZ_FUNC, CHILD_FUNC                                           \
    )

#endif // #ifndef __DT_IOCTL_PROPERTIES_H
