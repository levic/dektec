//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDITXF.h *#*#*#*#*#*#*#*#*#*#* (C) 2018 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2018 DekTec Digital Video B.V.
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

#ifndef __DT_BC_SDITXF_H
#define __DT_BC_SDITXF_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the SdiTxFmtSimple block (must match block ID)
#define DT_BC_SDITXF_NAME        "SdiTxFmtSimple"
#define DT_BC_SDITXF_SHORTNAME   "SDITXF"
#define DT_BC_SDITXF6G12G_NAME        "SdiTxFmtSimple6G12G"
#define DT_BC_SDITXF6G12G_SHORTNAME   "SDITXF6G12G"

// MACRO: to init an block-controller-ID for the SDITXF-BC
#define DT_BC_SDITXF_INIT_ID(ID, TYPE, ROLE, INSTANCE, UUID)                             \
do                                                                                       \
{                                                                                        \
    if (TYPE == DT_BLOCK_TYPE_SDITXF)                                                    \
        DT_BC_INIT_ID(ID, DT_BC_SDITXF_NAME, DT_BC_SDITXF_SHORTNAME, ROLE, INSTANCE,     \
                                                                                 UUID);  \
    else                                                                                 \
        DT_BC_INIT_ID(ID, DT_BC_SDITXF6G12G_NAME, DT_BC_SDITXF6G12G_SHORTNAME, ROLE,     \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDITXF definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcSDITXF
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;


    Bool  m_FramePropsSet;          // True if the user has set frame properties
    Bool  m_FmtEvtTimingSet;        // True if the user has set format event timing 

    // Cached settings
    Int  m_InterruptsPerFrame;      // Number of format-event interrupts per frame
    Bool  m_BlockEnabled;           // Block enabled
    Int  m_OperationalMode;         // Operational mode
    Bool  m_UlfEnabled;             // Underflow detection enabled

    Int  m_NumLinesPerEvent;        // Number of lines between format events (0=at SOF)
    Int  m_NumSofsBetweenTod;       // Number of frames between TOD samples
    Int  m_MaxSdiRate;              // Maximum SDI-rate
    Int  m_StreamAlignment;         // Stream bit-alignment

    // Interrupts related
    DtDpc  m_IntDpc;
    Int  m_FmtIntId;                // Format event interrupt ID
    DtEvent  m_FmtEvent;            // Event used to indicate formatter event
    Bool  m_FmtIntEnabled;          // Flag indicating format interrupt event is enabled
    DtSpinLock  m_IntDataSpinLock;  // Access protection for interrupt data
    Int  m_FrameId;                 // Frame ID (8 least significant bits)
    Int  m_SeqNumber;               // Sequence number
    Int  m_SofCount;                // Start-of-Frame counter
    DtTodTime  m_SofTime;           // Timestamp of SOF
    Bool   m_SofTimeValid;          // SofTime is valid
    Bool  m_Underflow;              // Underflow detected flag
    Bool  m_UnderflowLatched;       // Underflow detected flag latched
    Int  m_UlfEnableDelayCount;     // Underflow detection delay

}  DtBcSDITXF;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDITXF public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcSDITXF_Close(DtBc*);
DtBcSDITXF*  DtBcSDITXF_Open(Int  Address, DtCore*, DtPt*  pPt, DtBcType  Type,
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);

DtStatus  DtBcSDITXF_GetFmtEventSetting(DtBcSDITXF* pBc, Int* pNumLinesPerEvent, 
                                                           Int* pNumSofsBetweenTimestamp);
DtStatus  DtBcSDITXF_GetMaxSdiRate(DtBcSDITXF* pBc, Int* pMaxSdiRate);
DtStatus  DtBcSDITXF_GetOperationalMode(DtBcSDITXF* pBc,  Int* pOpMode);
DtStatus  DtBcSDITXF_GetStreamAlignment(DtBcSDITXF* pBc, Int* pStreamAlinment);
DtStatus  DtBcSDITXF_SetFmtEventSetting(DtBcSDITXF* pBc, Int NumLinesPerEvent,
                                                             Int NumSofsBetweenTimestamp);
DtStatus  DtBcSDITXF_SetOperationalMode(DtBcSDITXF* pBc,  Int OpMode);
DtStatus  DtBcSDITXF_WaitForFmtEvent(DtBcSDITXF* pBc, Int Timeout, Int* pFrameId, 
                           Int* pSeqNumber, Int* pUnderflow, DtTodTime*, Int* pTimeValid);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDITXF definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXF -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an SdiTxTsSel Block
typedef struct _DtIoStubBcSDITXF
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcSDITXF;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDITXF public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcSDITXF_Close(DtIoStub*);
DtIoStubBcSDITXF*  DtIoStubBcSDITXF_Open(DtBc*);


#endif  // #ifndef __DT_BC_SDITXF_H
