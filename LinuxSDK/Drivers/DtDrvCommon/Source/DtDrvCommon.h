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
#define  LOG_LEVEL_CORE             LOG_ERR
// TABLE
#define  LOG_LEVEL_TABLE            LOG_ERR
// Properties related
#define  LOG_LEVEL_PROP             LOG_ERR
// AV related
#define  LOG_LEVEL_AV               LOG_ERR
// IoConfig
#define  LOG_LEVEL_IOCONFIG         LOG_ERR
// VPD related
#define  LOG_LEVEL_VPD              LOG_ERR
// PIPE related
#define  LOG_LEVEL_PIPE             LOG_ERR

// Core-function related
#define  LOG_LEVEL_CM_EVENTS        LOG_ERR
#define  LOG_LEVEL_CM_IOCONFIG      LOG_ERR
#define  LOG_LEVEL_CM_SETTINGS      LOG_ERR

#define  LOG_LEVEL_CF_EVT           LOG_ERR
#define  LOG_LEVEL_CF_INT           LOG_ERR
#define  LOG_LEVEL_CF_IOCFG         LOG_ERR
#define  LOG_LEVEL_CF_PROPS         LOG_ERR
#define  LOG_LEVEL_CF_SETTINGS      LOG_ERR
#define  LOG_LEVEL_CF_TOD           LOG_ERR

// Block-controller related
#define  LOG_LEVEL_BC_ACCUFIFO      LOG_ERR
#define  LOG_LEVEL_BC_ASIRXP        LOG_ERR
#define  LOG_LEVEL_BC_ASITXG        LOG_ERR
#define  LOG_LEVEL_BC_ASITXSER      LOG_ERR
#define  LOG_LEVEL_BC_BURSTFIFO     LOG_ERR
#define  LOG_LEVEL_BC_CDMAC         LOG_ERR
#define  LOG_LEVEL_BC_CDMACTO       LOG_ERR
#define  LOG_LEVEL_BC_CONSTSINK     LOG_ERR
#define  LOG_LEVEL_BC_CONSTSOURCE   LOG_ERR
#define  LOG_LEVEL_BC_COMMON        LOG_ERR
#define  LOG_LEVEL_BC_DATACDC       LOG_ERR
#define  LOG_LEVEL_BC_DATACDC2      LOG_ERR
#define  LOG_LEVEL_BC_DATAFIFO      LOG_ERR
#define  LOG_LEVEL_BC_DDRFIFO       LOG_ERR
#define  LOG_LEVEL_BC_DDRFRONT      LOG_ERR
#define  LOG_LEVEL_BC_DCXO39XX      LOG_ERR
#define  LOG_LEVEL_BC_DCXO39XX      LOG_ERR
#define  LOG_LEVEL_BC_DISEQC        LOG_ERR
#define  LOG_LEVEL_BC_EMAC10G       LOG_ERR
#define  LOG_LEVEL_BC_ETHIPPAD      LOG_ERR
#define  LOG_LEVEL_BC_FANC          LOG_ERR
#define  LOG_LEVEL_BC_FPGATEMP      LOG_ERR
#define  LOG_LEVEL_BC_GENL          LOG_ERR
#define  LOG_LEVEL_BC_GPSTIME       LOG_ERR
#define  LOG_LEVEL_BC_I2CM          LOG_ERR
#define  LOG_LEVEL_BC_IPFIFO        LOG_ERR
#define  LOG_LEVEL_BC_IPROUT        LOG_ERR
#define  LOG_LEVEL_BC_IPSCHED       LOG_ERR
#define  LOG_LEVEL_BC_IPSECG        LOG_ERR
#define  LOG_LEVEL_BC_IPSUMCHK      LOG_ERR
#define  LOG_LEVEL_BC_IPSUMINS      LOG_ERR
#define  LOG_LEVEL_BC_IOSERIN       LOG_ERR
#define  LOG_LEVEL_BC_IPSECG        LOG_ERR
#define  LOG_LEVEL_BC_IQUNPCK       LOG_ERR
#define  LOG_LEVEL_BC_DATACDC       LOG_ERR
#define  LOG_LEVEL_BC_IQFIR         LOG_ERR
#define  LOG_LEVEL_BC_IQNOISE       LOG_ERR
#define  LOG_LEVEL_BC_IQMISC        LOG_ERR
#define  LOG_LEVEL_BC_IQSYNC        LOG_ERR
#define  LOG_LEVEL_BC_IQUP          LOG_ERR
#define  LOG_LEVEL_BC_IQJESD        LOG_ERR
#define  LOG_LEVEL_BC_ATTNCTRL_2116 LOG_ERR
#define  LOG_LEVEL_BC_IO_2116       LOG_ERR
#define  LOG_LEVEL_BC_KA            LOG_ERR
#define  LOG_LEVEL_BC_LEDB          LOG_ERR
#define  LOG_LEVEL_BC_LMH1981       LOG_ERR
#define  LOG_LEVEL_BC_MSIX          LOG_ERR
#define  LOG_LEVEL_BC_REBOOT        LOG_ERR
#define  LOG_LEVEL_BC_S12GTO3G      LOG_ERR
#define  LOG_LEVEL_BC_SDIDMX12G     LOG_ERR
#define  LOG_LEVEL_BC_SDIMUX12G     LOG_ERR
#define  LOG_LEVEL_BC_SDIXCFG       LOG_ERR
#define  LOG_LEVEL_BC_SDIRXF        LOG_ERR
#define  LOG_LEVEL_BC_SDIRXP        LOG_ERR
#define  LOG_LEVEL_BC_SDIRXPHY      LOG_ERR
#define  LOG_LEVEL_BC_SDITXF        LOG_ERR
#define  LOG_LEVEL_BC_SDITXP        LOG_ERR
#define  LOG_LEVEL_BC_SDITXPHY      LOG_ERR
#define  LOG_LEVEL_BC_SDITXPLL      LOG_ERR
#define  LOG_LEVEL_BC_SDIXCVR3GCV   LOG_ERR
#define  LOG_LEVEL_BC_SI2166ITF     LOG_ERR
#define  LOG_LEVEL_BC_SPIM          LOG_ERR
#define  LOG_LEVEL_BC_SPIMF         LOG_ERR
#define  LOG_LEVEL_BC_ST425LR       LOG_ERR
#define  LOG_LEVEL_BC_SWITCH        LOG_ERR
#define  LOG_LEVEL_BC_TOD           LOG_ERR
#define  LOG_LEVEL_BC_TSRXFMT       LOG_ERR
#define  LOG_LEVEL_BC_VVI           LOG_ERR
#define  LOG_LEVEL_BC_XPLL          LOG_ERR

// Local block-controller related
#define  LOG_LEVEL_BC_AD5320_2132   LOG_ERR
#define  LOG_LEVEL_BC_ADS4246_2132  LOG_ERR
#define  LOG_LEVEL_BC_DATASTMUX_2132  LOG_ERR
#define  LOG_LEVEL_BC_IQCGRAB_2132  LOG_ERR
#define  LOG_LEVEL_BC_IQFIR_2132    LOG_ERR
#define  LOG_LEVEL_BC_IQPWR_2132    LOG_ERR
#define  LOG_LEVEL_BC_IQSRC2132_2132  LOG_ERR
#define  LOG_LEVEL_BC_LNBH25_2132   LOG_ERR
#define  LOG_LEVEL_BC_S2DEC_2132    LOG_ERR
#define  LOG_LEVEL_BC_S2DEMOD_2132  LOG_ERR
#define  LOG_LEVEL_BC_S2STATS_2132  LOG_ERR

// Driver-function related
#define  LOG_LEVEL_DF_ASIRX         LOG_ERR
#define  LOG_LEVEL_DF_CHSDIRXPHYONLY  LOG_ERR
#define  LOG_LEVEL_DF_CHSDIRX       LOG_ERR
#define  LOG_LEVEL_DF_COMMON        LOG_MIN
#define  LOG_LEVEL_DF_CLKGEN        LOG_ERR
#define  LOG_LEVEL_DF_DATAFIFO      LOG_ERR
#define  LOG_LEVEL_DF_DEBUG         LOG_ERR
#define  LOG_LEVEL_DF_EVENT         LOG_ERR
#define  LOG_LEVEL_DF_IOCONFIG      LOG_ERR
#define  LOG_LEVEL_DF_GENLOCKCTRL   LOG_ERR
#define  LOG_LEVEL_DF_IPFIFO        LOG_ERR
#define  LOG_LEVEL_DF_MXDS75TEMP    LOG_ERR
#define  LOG_LEVEL_DF_NW            LOG_ERR
#define  LOG_LEVEL_DF_PROPS         LOG_ERR
#define  LOG_LEVEL_DF_SDITXPHY      LOG_ERR
#define  LOG_LEVEL_DF_SDIRX         LOG_ERR
#define  LOG_LEVEL_DF_SDITX3G       LOG_ERR
#define  LOG_LEVEL_DF_SDIXCFGMGR    LOG_ERR
#define  LOG_LEVEL_DF_SENSTEMP      LOG_ERR
#define  LOG_LEVEL_DF_SI534X        LOG_ERR
#define  LOG_LEVEL_DF_SETTINGS      LOG_ERR
#define  LOG_LEVEL_DF_SPIPROM       LOG_ERR
#define  LOG_LEVEL_DF_SPICABLEDRVEQ LOG_ERR
#define  LOG_LEVEL_DF_TEMPFANMGR    LOG_ERR
#define  LOG_LEVEL_DF_TODCLOCKCTRL  LOG_ERR
#define  LOG_LEVEL_DF_TXPLLMGR      LOG_ERR
#define  LOG_LEVEL_DF_VIRTGENREF    LOG_ERR
#define  LOG_LEVEL_DF_VPD           LOG_ERR

// Port related
#define  LOG_LEVEL_PT_COMMON        LOG_ERR
#define  LOG_LEVEL_PT_ASISDIMON     LOG_ERR
#define  LOG_LEVEL_PT_ASISDIRXTX    LOG_ERR
#define  LOG_LEVEL_PT_IP            LOG_ERR
#define  LOG_LEVEL_PT_SDIGENREF     LOG_ERR 
#define  LOG_LEVEL_PT_SDIPHYONLYRXTX  LOG_ERR



// Local driver-function related
#define  LOG_LEVEL_DF_CLKCTRL_2116      LOG_ERR
#define  LOG_LEVEL_DF_S2CRDEMOD_2132    LOG_ERR
#define  LOG_LEVEL_DF_TXCLKCTRL_2178    LOG_ERR

#endif // __DT_DRV_COMMON_H
