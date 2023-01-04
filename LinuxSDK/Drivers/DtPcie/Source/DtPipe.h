// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtPipe.h *#*#*#*#*#*#*#*#*#*#*#* (C) 2021 DekTec
//
#ifndef __DT_PIPE_H
#define __DT_PIPE_H

#include "DtBcBURSTFIFO.h"
#include "DtBcCDMACTO.h"
#include "DtDfIpFifo.h"
#include "DtBcIPROUT.h"
#include "DtBcDATACDC.h"
#include "DtBcETHIPPAD.h"
#include "DtBcDDRFIFO.h"
#include "DtBcIPSCHED.h"

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtPipe definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Common data for a DtPipe object
#define DT_PIPE_COMMON_DATA                                                              \
    DT_OBJECT_COMMON_DATA;                                                               \
    DtFileObject m_FileObject;                                                           \
    Bool m_InUse;                                                                        \
    Bool m_Locked;                                                                       \
    DtPipeType m_PipeType;                                                               \
    Int m_Id;                                                                            \
    Int m_Index;                                                                         \
    DtObject* m_pPipeOwner;                                                              \
    DtTrigEvt m_TrigEvt;                                                                 \
    DtFastMutex m_FastMutex;                                                             \
    UInt32 m_Capabilities;                                                               \
    Int m_OpMode;                                                                        \
    Int m_ErrorFlag;                                                                     \
    UInt64 m_TimeNs;                                                                     \
    Bool m_SharedBufferSet

typedef struct _DtPipe
{
    DT_PIPE_COMMON_DATA;
} DtPipe;

#define DT_PIPE_HWP_COMMON                                                               \
    DT_PIPE_COMMON_DATA;                                                                 \
    DtBcCDMACTO* m_pBcCdma;                                                              \
    DtDfIpFifo* m_pDfIpFifo;                                                             \
    UInt m_OffsetCache
// m_OffsetCache: RxReadOffset, TxWriteOffset

typedef struct _DtPipeHwp
{
    DT_PIPE_HWP_COMMON;
} DtPipeHwp;

#define DT_PIPE_HWP_KBUF                                                                 \
    DT_PIPE_HWP_COMMON;                                                                  \
    DtPageList* m_pPageListK;                                                            \
    UInt8* m_pBuffer;                                                                    \
    UInt m_BufSize;                                                                      \
    UInt m_MaxLoad

typedef struct _DtPipeHwpKbuf
{
    DT_PIPE_HWP_KBUF;
}DtPipeHwpKbuf;

typedef struct _DtPipeTxRtHwp
{
    DT_PIPE_HWP_COMMON;
    DtBcBURSTFIFO* m_pBurstFifo;
    DtBcIPSCHED* m_pIpSched;
} DtPipeTxRtHwp;

typedef struct _DtPipeRxRtHwp
{
    DT_PIPE_HWP_COMMON;
    DtBcIPROUT* m_pIpRouter;
    Bool m_IpParsSet;
    UInt m_ReadOffsetCache;
} DtPipeRxRtHwp;

typedef struct _DtPipeRxHwq
{
    DT_PIPE_HWP_KBUF;
    UInt m_ReadOffsetCache;
    DtBcETHIPPAD* m_pEthIpPad;
    DtBcDDRFIFO* m_pDdrFifo;
} DtPipeRxHwq;

#define DT_PIPE_SWP_COMMON                                                               \
    DT_PIPE_COMMON_DATA;                                                                 \
    UInt m_ReadOffset;                                                                   \
    UInt m_WriteOffset;                                                                  \
    DtPageList m_PageListU;                                                              \
    UInt8* m_pBuffer;                                                                    \
    UInt m_TrigEvtCnt;                                                                   \
    UInt m_TrigEvtMaxCnt;                                                                \
    UInt m_BufSize

typedef struct _DtPipeSwp
{
    DT_PIPE_SWP_COMMON;
} DtPipeSwp;

typedef struct _DtPipeRxRtSwp
{
    DT_PIPE_SWP_COMMON;
    UInt8 m_DstIp[16];
    UInt16 m_DstPort[3];
    UInt8 m_SrcIp[16];
    UInt16 m_SrcPort[3];
    Int m_VlanId[2];
    Int m_Flags;
    Bool m_IpParsSet;
} DtPipeRxRtSwp;

#define DT_PIPE_SWP_KBUF                                                                 \
    DT_PIPE_SWP_COMMON;                                                                  \
    DtPageList* m_pPageListK;                                                            \
    UInt m_MaxLoad


typedef struct _DtPipeSwpKbuf
{
    DT_PIPE_SWP_KBUF;

} DtPipeSwpKbuf;

typedef struct _DtPipeRxNrt
{
    DT_PIPE_SWP_KBUF;
} DtPipeRxNrt;

typedef struct _DtPipeTxNrt
{
    DT_PIPE_HWP_KBUF;
} DtPipeTxNrt;

typedef struct _DtPipeTxRtSwp
{
    DT_PIPE_SWP_COMMON;
} DtPipeTxRtSwp;

typedef struct _DtPipeTxRtHwq
{
    DT_PIPE_HWP_KBUF;
    DtBcBURSTFIFO* m_pBurstFifo;
} DtPipeTxRtHwq;


void DtPipe_Close(DtPipe* pPipe);
DtPipe* DtPipe_Open(const DtFileObject* pFileObject, DtObject* pPipeOwner, 
                                                  DtPipeType PipeType, Int Id, Int Index);
DtStatus DtPipe_SetSharedBuffer(DtPipe* pPipe, UInt8* pBuffer, Int BufferSize);
DtStatus DtPipe_SetSharedBufferKernel(DtPipe* pPipe, Int RequestSize, Int* pActualSize,
                                                         Int* pMaxLoad, UInt8** ppBuffer);
DtStatus DtPipe_ReleaseSharedBuffer(DtPipe* pPipe);
DtStatus DtPipe_GetProperties(DtPipe* pPipe, UInt32* pCapabilities, Int* pPrefetchSize,
                                                     Int* pPipeDataWidth, Int* pPipeType);
DtStatus DtPipe_GetStatus(DtPipe* pPipe, Int* pOpStatus, UInt32* pStatusFlags,
                                                                     UInt32* pErrorFlags);
DtStatus DtPipe_IssuePipeFlush(DtPipe* pPipe);
DtStatus DtPipe_SetOperationalMode(DtPipe* pPipe, Int OpMode);
DtStatus DtPipe_GetOperationalMode(DtPipe* pPipe, Int* pOpMode);
DtStatus DtPipe_SetRxReadOffset(DtPipe* pPipe, UInt RxReadOffset);
DtStatus DtPipe_GetRxWriteOffset(DtPipe* pPipe, UInt* pRxWriteOffset);
DtStatus DtPipe_SetTxWriteOffset(DtPipe* pPipe, UInt TxWriteOffset);
DtStatus DtPipe_GetTxReadOffset(DtPipe* pPipe, UInt* pTxReadOffset);
DtStatus DtPipe_RegisterEvent(DtPipe* pPipe, DtTrigEvtDriver* pTrigEvtDriver, UInt Count);
DtStatus DtPipe_DeregisterEvent(DtPipe* pPipe, DtTrigEvtDriver* pTrigEvtDriver);
DtStatus DtPipe_SetIpFilter(DtPipe* pPipe, UInt8 DstIp[16], UInt16 DstPort[3],
                          UInt8 SrcIp[16], UInt16  SrcPort[3], Int  VlanId[2], Int Flags);
DtStatus DtPipe_GetIpFilter(DtPipe* pPipe, UInt8 DstIp[16], UInt16 DstPort[3],
                          UInt8 SrcIp[16], UInt16  SrcPort[3], Int* pVlanId, Int* pFlags);
DtStatus DtPipe_SetIpFilterDuplicate(DtPipe* pPipe, Bool Duplicate);
void DtPipe_ReleaseResources(DtPipe* pPipe);

DtStatus DtPipe_CopyData(DtPipe* pSrc, UInt SrcOffset, DtPipe* pDst, Int BytesToCopy);
DtStatus DtPipe_TriggerEvent(DtPipe* pPipe);
Bool DtPipe_IsPerIntEvent(DtPipe* pPipe);
Bool DtPipe_IsDataAvailCntEvent(DtPipe* pPipe);
Bool DtPipe_IsDataAvailThrEvent(DtPipe* pPipe);
Bool DtPipe_IsPacketForPipe(DtPipeRxRtSwp* pPipe, UInt8* DstIp, UInt16 DstPort,
                       UInt8* SrcIp, UInt16 SrcPort, Int VlanId1, Int VlanId2, Int Flags);
const char* PipeType2Str(DtPipeType Type);
Bool DtPipe_IsDuplicateFilter(DtPipe* pPipeD, DtPipe* pPipeC);


#endif
