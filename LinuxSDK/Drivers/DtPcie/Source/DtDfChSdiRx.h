// #*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfChSdiRx.h *#*#*#*#*#*#*#*#*#*# (C) 2022 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2022 DekTec Digital Video B.V.
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

#ifndef __DT_DF_CHSDIRX_H
#define __DT_DF_CHSDIRX_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDfCh.h"
#include "DtBcBURSTFIFO.h"          // Burst-FIFO block controller
#include "DtBcCDMAC.h"              // CDMAC block controller
#include "DtBcSDIMUX12G.h"          // SdiMux12G block controller
#include "DtBcSDIRXF.h"             // SdiRxFmtSimple block controller
#include "DtDfSdiRx.h"              // SdiRx driver function
#include "DtBcST425LR.h"            // St425LinkReorder block controller
#include "DtBcSWITCH.h"             // Switch block controller

// Name + short-name for the CHSDIRX function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_CHSDIRX_NAME         "SdiRxChannel"
#define DT_DF_CHSDIRX_SHORTNAME    "CHSDIRX"

// MACRO: to init an driver-function-ID for the CHSDIRX-DF
#define DT_DF_CHSDIRX_INIT_ID(ID, ROLE, INSTANCE, UUID)                                 \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_CHSDIRX_NAME, DT_DF_CHSDIRX_SHORTNAME,                     \
                                                               ROLE, INSTANCE, UUID);    \
}                                                                                        \
while (0)

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtDfChSdiRx definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct DtDfChSdiRxUser -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
// Defines a user for an SDI receive channel
typedef struct _DtDfChSdiRxUser
{
    // NOTE: common chan-user-data must be the first member to allow casting to DtDfChUser
    DT_DF_CH_USER_COMMON_DATA;
    UInt64* m_pMappedDmaAddr;       // Address channels DMA buffer mapped in the users 
                                    // memory space. NULL, when not mapped yet
    UInt32 m_DmaReadOffset;         // Read offset in the DMA buffer
    Bool m_IsConfigured;            // User has successfully applied its configuration
    DtEvent m_FmtEvent;             // Event used to indicate formatter event
    Bool m_IsSlow;                  // User is to far behind with each read-offset and 
                                    // it's read offset is ignored until the user has 
                                    // caught up. While in this state all format events to
                                    // this user will be marked as being out-of-sync.
} DtDfChSdiRxUser;

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct DtDfChSdiRxConfig -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
// Defines the configuration of an SDI receive channel
typedef struct _DtDfChSdiRxConfig
{
    // DMA buffer configuration
    struct
    {
        Int m_MinSize;              // Minimum size for the DMA buffer, requested by 
                                    // the user.
    } m_DmaBuf;

    // Format event configuration
    struct
    {
        Int m_IntInterval;          // Interval between format-event interrupts in us
        Int m_IntDelay;             // Delay between SOF and the interrupt in us
        Int m_NumIntsPerFrame;      // Number of format-event interrupts per frame
    } m_FmtEvt;

    // Frame properties
    struct
    {
        Int m_NumSymsHanc;          // Number of HANC symbols per line 
        Int m_NumSymsVidVanc;       // Number of Video/VANC symbols per line  
        Int m_NumLines;             // Number of lines per frame 
        Int m_SdiRate;              // SDI-Rate
        Int m_AssumeInterlaced;     // Assume interlaced (TRUE or FALSE) 
    } m_FrameProps;

    Int m_NumPhysicalPorts;         // Number of valid physical port. Valid values: 1 or 4
    Int m_PhysicalPorts[4];         // List physical ports (indexes) used for receiving 
                                    // signal. Typically, this will include just the one 
                                    // port, but for a quad-link setup all four port are 
                                    // used.
                                    // Note: the order of the ports determines the logical
                                    // link order setting for the ST425 link re-orderer

} DtDfChSdiRxConfig;

// .-.-.-.-.-.-.-.-.-.-.-.- enum DtDfChSdiRxQuadlinkConfiguration -.-.-.-.-.-.-.-.-.-.-.-.
// Quad-link configuration modes for an SDI receve channel
typedef enum _DtDfChSdiRxQuadlinkConfiguration
{
    SDIRXCHAN_QL_CONFIG_SINGLE,     // Configured for single-link operation
    SDIRXCHAN_QL_CONFIG_QUAD,       // Configured for quad-link operation
} DtDfChSdiRxQuadlinkConfiguration;

// -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct DtDfChSdiRx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
//
typedef struct _DtChSdiRx
{
    // NOTE: common channel data must be the first members to allow casting to DtCh
    DT_DF_CH_COMMON_DATA;

    Bool m_IsConfigured;            // Channel has been configured
    DtDfChSdiRxConfig m_Config;     // Currently active configuration

    // Kernel buffer used for DMAs. 
    // The channel users can map this buffer into their memory space for direct access to 
    // its content.
    struct
    {
        Int m_Size;                 // Size of the DMA buffer
        Int m_MaxLoad;              // Maximum allowed load in the DMA buffer. Typically,
                                    // one data-word is reserved and cannot be written to.
        Int m_SlowLoad;             // If this load is exceeded a user is considered to
                                    // be to slow with updating its read offset.
        UInt32 m_ReadOffset;        // Current read offset in the DMA buffer
        DtPageList* m_pPageList;    // Page list for the DMA buffer
        UInt8* m_pBuffer;           // Virtual kernel pointer for our buffer
    } m_DmaBuf;

    // Cached capabilities
    Bool m_CapScale12To3G;          // Channel supports 12G-to-3G scaling
    Bool m_CapQuadlink;             // Channel supports quad-link configurations


    // Format event data
    struct
    {
        DtSpinLock m_SpinLock;      // Access protection for format event data
        Int m_FrameId;              // Frame ID (8 least significant bits)
        Int m_SeqNumber;            // Sequence number
        Bool m_InSync;              // True if in-sync
    } m_FmtEvent;

    // =+=+=+=+=+=+=+=+=+ BCs and DFs needed to control an SDI channel +=+=+=+=+=+=+=+=+=+
    
    // Core block controllers and driver functions
    DtBcCDMAC* m_pBcCDMAC;
    DtBcBURSTFIFO* m_pBcBURSTFIFO;
    DtBcSDIRXF* m_pBcSDIRXF;
    DtDfSdiRx* m_pDfSdiRx;

    // Quad-link related block controllers and driver function
    struct
    {
        DtDfChSdiRxQuadlinkConfiguration m_Config;

        DtBcSWITCH* m_pBcSWITCH_SdiMuxIn;
        DtBcST425LR* m_pBcST425LR;
        DtBcSDIMUX12G* m_pBcSDIMUX12G;
        DtBcSWITCH* m_pBcSWITCH_SdiMuxOut;
    } m_Quadlink;                   // Quad-link extension. Only relevant for channels 
                                    // with quad link support

} DtDfChSdiRx;

// -.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfChSdiRx - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
void DtDfChSdiRx_Close(DtDf*);
DtStatus DtDfChSdiRx_Lock(DtDfChSdiRx*, int Timeout);
DtDfChSdiRx* DtDfChSdiRx_Open(DtCore*, DtPt*  pPt, const char*  pRole, 
                                              Int  Instance, Int  Uuid, Bool  CreateStub);
void DtDfChSdiRx_Unlock(DtDfChSdiRx*);

// -.-.-.-.-.-.-.-.-.- DtDfChSdiRx - Public functions - Linux specific -.-.-.-.-.-.-.-.-.-
#ifdef LINBUILD
DtStatus DtDfChSdiRx_Mmap(DtDfChSdiRx*, const DtFileObject*, DtVma*);
#endif // #ifdef LINBUILD

// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
// =+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubDfChSdiRx definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=
// +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.- struct DtIoStubDfChSdiRx -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an SDIRXCHAN Driver-Function
typedef struct _DtIoStubDfChSdiRx
{
    // NOTE: common stub data must be the first members to allow casting to DtIoStubDf
    DT_IOSTUB_DF_COMMON_DATA;
}  DtIoStubDfChSdiRx;

// .-.-.-.-.-.-.-.-.-.-.-.- DtIoStubDfChSdiRx - Public functions -.-.-.-.-.-.-.-.-.-.-.-.-
void DtIoStubDfChSdiRx_Close(DtIoStub*);
DtIoStubDfChSdiRx* DtIoStubDfChSdiRx_Open(DtDf*);

#endif // #ifndef __DT_DF_CHSDIRX_H
