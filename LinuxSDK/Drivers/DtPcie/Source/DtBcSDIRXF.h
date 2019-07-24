//*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtBcSDIRXF.h *#*#*#*#*#*#*#*# (C) 2017-2018 DekTec
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

#ifndef __DT_BC_SDIRXF_H
#define __DT_BC_SDIRXF_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtBc.h"

// Name and short-name of the SdiRxFmtSimple block (must match block ID)
#define DT_BC_SDIRXF_NAME        "SdiRxFmtSimple"
#define DT_BC_SDIRXF_SHORTNAME   "SDIRXF"

// MACRO: to init an block-controller-ID for the SDIRXF-BC
#define DT_BC_SDIRXF_INIT_ID(ID, ROLE, INSTANCE, UUID)                                   \
do                                                                                       \
{                                                                                        \
    DT_BC_INIT_ID(ID, DT_BC_SDIRXF_NAME, DT_BC_SDIRXF_SHORTNAME, ROLE, INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtBcSDIRXF definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=


//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef  struct _DtBcSDIRXF
{
    // NOTE: common block data must be the first members to allow casting to DtBc
    DT_BC_COMMON_DATA;


    Bool  m_FramePropsSet;          // True if the user has set frame properties
    Bool  m_FmtEvtTimingSet;        // True if the user has set format event timing 

    // Cached settings
    Int  m_NumSymsHanc;             // Number of HANC symbols per line
    Int  m_NumSymsVidVanc;          // Number of Video/VANC symbols per line
    Int  m_NumLines;                // Number of lines per frame
    Int  m_SdiRate;                 // SDI-rate
    Bool  m_AssumeInterlaced;       // Assume interlaced
    Int  m_InterruptInterval;       // Interval between format-event interrupts in us
    Int  m_InterruptDelay;          // Delay between SOF and the interrupt in us
    Int  m_InterruptsPerFrame;      // Number of format-event interrupts per frame
    Bool  m_BlockEnabled;           // Block enabled
    Int  m_OperationalMode;         // Operational mode
    Int  m_MaxSdiRate;              // Maximum SDI-rate

    // Interrupts related
    DtDpc  m_IntDpc;
    DtEvent  m_FmtEvent;            // Event used to indicate formatter event
    Bool  m_FmtIntEnabled;          // Flag indicating format interrupt event is enabled
    DtSpinLock  m_IntDataSpinLock;  // Access protection for interrupt data
    Int  m_FrameId;                 // Frame ID (8 least significant bits)
    Int  m_SeqNumber;               // Sequence number
    Bool m_InSync;                  // True if in-sync

}  DtBcSDIRXF;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtBcSDIRXF public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
void  DtBcSDIRXF_Close(DtBc*);
DtBcSDIRXF*  DtBcSDIRXF_Open(Int  Address, DtCore*, DtPt*  pPt, 
                                             const char*  pRole, Int  Instance, Int  Uuid,
                                             Bool  CreateStub);

DtStatus  DtBcSDIRXF_GetFmtEventTiming(DtBcSDIRXF* pBc, Int* pInterval, Int* pDelay, 
                                                                       Int* pIntPerFrame);
DtStatus  DtBcSDIRXF_GetFrameProperties(DtBcSDIRXF*, Int*  pNumSymsHanc,
                                                   Int* pNumSymsVidVanc, Int* pNumLines,
                                                   Int* pSdiRate, Int* pAssumeInterlaced);
DtStatus  DtBcSDIRXF_GetMaxSdiRate(DtBcSDIRXF* pBc, Int* pMaxSdiRate);
DtStatus  DtBcSDIRXF_GetOperationalMode(DtBcSDIRXF* pBc,  Int* pOpMode);
DtStatus  DtBcSDIRXF_SetFmtEventTiming(DtBcSDIRXF* pBc, Int Interval, Int Delay, 
                                                                         Int IntPerFrame);
DtStatus  DtBcSDIRXF_SetFrameProperties(DtBcSDIRXF*, Int  NumSymsHanc,
                                                      Int NumSymsVidVanc, Int NumLines,
                                                      Int AssumeSd, Int AssumeInterlaced);
DtStatus  DtBcSDIRXF_SetOperationalMode(DtBcSDIRXF* pBc,  Int OpMode);
DtStatus  DtBcSDIRXF_WaitForFmtEvent(DtBcSDIRXF* pBc, Int Timeout, Int* pFrameId,
                                                           Int* pSeqNumber, Int* pInSync);

//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+ DtIoStubBcSDIRXF definitions +=+=+=+=+=+=+=+=+=+=+=+=+=+=+
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXF -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
//  Io-control stub for an SdiRxTsSel Block
typedef struct _DtIoStubBcSDIRXF
{
    // NOTE: common stub data must be the first members to allow casting to 
    // DtBcIoCtl
    DT_IOSTUB_BC_COMMON_DATA;
}  DtIoStubBcSDIRXF;

//.-.-.-.-.-.-.-.-.-.-.-.-.- DtIoStubBcSDIRXF public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtIoStubBcSDIRXF_Close(DtIoStub*);
DtIoStubBcSDIRXF*  DtIoStubBcSDIRXF_Open(DtBc*);


#endif  // #ifndef __DT_BC_SDIRXF_H
