//#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#*#* DtDrvCommon.h *#*#*#*#*#*#*#*# (C) 2011-2017 DekTec
//

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- License -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// Copyright (C) 2011-2017 DekTec Digital Video B.V.
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


#ifndef __DT_DRV_COMMON_H
#define __DT_DRV_COMMON_H

//-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- Includes -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-

#include <DtSal.h>
#include <DtObject.h>
#include <DtProperties.h>
#include <DtTables.h>
#include <DtUtilities.h>
#include <DtIoCaps.h>
#include <DtIoConfigDefs.h>
#include <IoConfigCodes.h>
#include <DtIoConfig.h>
#include <DtAudioVideo.h>
#include <VpdCommon.h>
#include <DtVector.h>
#include <DtExclAccessObject.h>

// TAG for memory allocated by the comon driver code
#define COMMON_TAG      0x4E4D4D43      // 'NMMC'

//.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.- LOG levels -.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.-.

// CORE driver services related
#define  LOG_LEVEL_CORE             LOG_MIN
// TABLE
#define  LOG_LEVEL_TABLE            LOG_MIN
// Properties related
#define  LOG_LEVEL_PROP             LOG_MIN
// AV related
#define  LOG_LEVEL_AV               LOG_MIN
// IoConfig
#define  LOG_LEVEL_IOCONFIG         LOG_MIN
// VPD related
#define  LOG_LEVEL_VPD              LOG_MIN

// Core-function related
#define  LOG_LEVEL_CM_EVENTS        LOG_MIN
#define  LOG_LEVEL_CM_IOCONFIG      LOG_MIN
#define  LOG_LEVEL_CM_SETTINGS      LOG_MIN

#define  LOG_LEVEL_CF_EVT           LOG_MIN
#define  LOG_LEVEL_CF_INT           LOG_MIN
#define  LOG_LEVEL_CF_IOCFG         LOG_MIN
#define  LOG_LEVEL_CF_PROPS         LOG_MIN
#define  LOG_LEVEL_CF_SETTINGS      LOG_MIN
#define  LOG_LEVEL_CF_TOD           LOG_MIN

// Block-controller related
#define  LOG_LEVEL_BC_ACCUFIFO      LOG_MAX
#define  LOG_LEVEL_BC_ASIRXP        LOG_MAX
#define  LOG_LEVEL_BC_ASITXG        LOG_MAX
#define  LOG_LEVEL_BC_ASITXSER      LOG_MAX
#define  LOG_LEVEL_BC_BURSTFIFO     LOG_MAX
#define  LOG_LEVEL_BC_CDMAC         LOG_MAX
#define  LOG_LEVEL_BC_CONSTSINK     LOG_MAX
#define  LOG_LEVEL_BC_CONSTSOURCE   LOG_MAX
#define  LOG_LEVEL_BC_COMMON        LOG_MAX
#define  LOG_LEVEL_BC_DCXO39XX      LOG_MAX
#define  LOG_LEVEL_BC_FANC          LOG_MAX
#define  LOG_LEVEL_BC_FPGATEMP      LOG_MAX
#define  LOG_LEVEL_BC_I2CM          LOG_MIN
#define  LOG_LEVEL_BC_IPSECG        LOG_MAX
#define  LOG_LEVEL_BC_KA            LOG_MIN
#define  LOG_LEVEL_BC_LEDB          LOG_MIN
#define  LOG_LEVEL_BC_LMH1981       LOG_MIN
#define  LOG_LEVEL_BC_MSIX          LOG_MIN
#define  LOG_LEVEL_BC_REBOOT        LOG_MAX
#define  LOG_LEVEL_BC_SDIXCFG       LOG_MAX
#define  LOG_LEVEL_BC_SDIRXF        LOG_MAX
#define  LOG_LEVEL_BC_SDIRXP        LOG_MAX
#define  LOG_LEVEL_BC_SDIRXPHY      LOG_MAX
#define  LOG_LEVEL_BC_SDITXF        LOG_MAX
#define  LOG_LEVEL_BC_SDITXP        LOG_MAX
#define  LOG_LEVEL_BC_SDITXPHY      LOG_MAX
#define  LOG_LEVEL_BC_SDITXPLL      LOG_MAX
#define  LOG_LEVEL_BC_SDIXCVR3GCV   LOG_MAX
#define  LOG_LEVEL_BC_SPIM          LOG_MAX
#define  LOG_LEVEL_BC_SPIMF         LOG_MAX
#define  LOG_LEVEL_BC_SWITCH        LOG_MAX
#define  LOG_LEVEL_BC_TOD           LOG_MAX
#define  LOG_LEVEL_BC_TSRXFMT       LOG_MAX
#define  LOG_LEVEL_BC_VVI           LOG_MAX

// Local block-controller related
#define  LOG_LEVEL_BC_AD5320_2132   LOG_MAX
#define  LOG_LEVEL_BC_ADS4246_2132  LOG_MAX
#define  LOG_LEVEL_BC_DATASTMUX_2132  LOG_MAX
#define  LOG_LEVEL_BC_IQCGRAB_2132  LOG_MAX
#define  LOG_LEVEL_BC_IQFIR_2132    LOG_MAX
#define  LOG_LEVEL_BC_IQPWR_2132    LOG_MAX
#define  LOG_LEVEL_BC_IQSRC2132_2132  LOG_MAX
#define  LOG_LEVEL_BC_LNBH25_2132   LOG_MAX
#define  LOG_LEVEL_BC_S2DEC_2132    LOG_MAX
#define  LOG_LEVEL_BC_S2DEMOD_2132  LOG_MAX
#define  LOG_LEVEL_BC_S2STATS_2132  LOG_MAX

// Driver-function related
#define  LOG_LEVEL_DF_ASIRX         LOG_MAX
#define  LOG_LEVEL_DF_COMMON        LOG_MAX
#define  LOG_LEVEL_DF_DEBUG         LOG_MAX
#define  LOG_LEVEL_DF_EVENT         LOG_MAX
#define  LOG_LEVEL_DF_IOCONFIG      LOG_MAX
#define  LOG_LEVEL_DF_MXDS75TEMP    LOG_MAX
#define  LOG_LEVEL_DF_PROPS         LOG_MAX
#define  LOG_LEVEL_DF_SDITXPHY      LOG_MAX
#define  LOG_LEVEL_DF_SDIRX         LOG_MAX
#define  LOG_LEVEL_DF_SDITX3G       LOG_MAX
#define  LOG_LEVEL_DF_SDIXCFGMGR    LOG_MAX
#define  LOG_LEVEL_DF_SENSTEMP      LOG_MAX
#define  LOG_LEVEL_DF_SI534X        LOG_MAX
#define  LOG_LEVEL_DF_SETTINGS      LOG_MAX
#define  LOG_LEVEL_DF_SPIPROM       LOG_MAX
#define  LOG_LEVEL_DF_SPICABLEDRVEQ LOG_MAX
#define  LOG_LEVEL_DF_TEMPFANMGR    LOG_MAX
#define  LOG_LEVEL_DF_VPD           LOG_MAX

// Port related
#define  LOG_LEVEL_PT_COMMON        LOG_MAX
#define  LOG_LEVEL_PT_ASISDIRX      LOG_MAX
#define  LOG_LEVEL_PT_ASISDIRXTX    LOG_MAX
#define  LOG_LEVEL_PT_ASISDITX      LOG_MAX

// Local driver-function related
#define  LOG_LEVEL_DF_S2CRDEMOD_2132    LOG_MAX

#endif // __DT_DRV_COMMON_H
