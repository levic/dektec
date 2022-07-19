// *#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDfSpiCableDrvEq.h *#*#*#*#*#*#*#*#* (C) 2019 DekTec
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

#ifndef __DT_DF_SPICABLEDRVEQ_H
#define __DT_DF_SPICABLEDRVEQ_H

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Include files -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
#include "DtDf.h"
#include "DtBcSPIM.h"

// Name + short-name for the SPICABLEDRVEQ function. NOTE: must match names in 
// FunctionDescriptionsXxx.xml
#define DT_DF_SPICABLEDRVEQ_NAME          "SpiCableDrvEqCtrl"
#define DT_DF_SPICABLEDRVEQ_SHORTNAME     "SPICABLEDRVEQ"

// MACRO: to init an driver-function-ID for the SPICABLEDRVEQ-DF
#define DT_DF_SPICABLEDRVEQ_INIT_ID(ID, ROLE, INSTANCE, UUID)                            \
do                                                                                       \
{                                                                                        \
    DT_DF_INIT_ID(ID, DT_DF_SPICABLEDRVEQ_NAME, DT_DF_SPICABLEDRVEQ_SHORTNAME, ROLE,     \
                                                                       INSTANCE, UUID);  \
}                                                                                        \
while (0)


//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=
// .-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq definitions -.-.-.-.-.-.-.-.-.-.-.-.-.-.
//+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=

// Cable Equalizer/Driver direction
#define DT_DF_SPICABLEDRVEQ_DIR_RX         0   // Receive (equalizer)
#define DT_DF_SPICABLEDRVEQ_DIR_TX         1   // Transmit (driver)

// .-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- DtDfSpiCableDrvEq -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.
typedef struct  _DtDfSpiCableDrvEq
{
    // NOTE: common func data must be the first members to allow casting to DtDf
    DT_DF_COMMON_DATA;

    // SPI-Master
    DtBcSPIM*  m_pBcSpiM;               // SPI master block controller
    DtFastMutex  m_AccessMutex;         // Access protection for SPI-Prom

    // Cached values
    Int  m_SdiRate;                     // Selected SDI-rate
    Int  m_Direction;                   // Direction TX-driver / RX-equalizer
    Int  m_SpiDeviceId;                 // Cable driver/equalizer SPI device ID
    Bool  m_Supports12G;                // Supports 12G
    Int   m_TraceEqualizer;             // Trace equalizer setting
    Bool  m_MaxLoopBandwidth;           // Maximize loop bandwidth of PLL
}  DtDfSpiCableDrvEq;

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Public functions -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-
void  DtDfSpiCableDrvEq_Close(DtDf*);
DtDfSpiCableDrvEq*  DtDfSpiCableDrvEq_Open(DtCore*, DtPt*  pPt, const char*  pRole,
                                             Int  Instance,  Int  Uuid, Bool  CreateStub);
DtStatus  DtDfSpiCableDrvEq_GetDirection(DtDfSpiCableDrvEq*, Int* pDirection);
DtStatus  DtDfSpiCableDrvEq_GetSdiRate(DtDfSpiCableDrvEq*, Int* pSdiRate);
DtStatus  DtDfSpiCableDrvEq_SetDirection(DtDfSpiCableDrvEq*, Int Direction);
DtStatus  DtDfSpiCableDrvEq_SetRxSdiRate(DtDfSpiCableDrvEq*, Int SdiRate);
DtStatus  DtDfSpiCableDrvEq_SetTxSdiRate(DtDfSpiCableDrvEq*, Int SdiRate);


#endif  // #ifndef __DT_DF_SPICABLEDRVEQ_H

